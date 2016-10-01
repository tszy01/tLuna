#include "TLZip.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "zlib.h"

#include "TLString.h"

namespace TLunaEngine{

	TU32 Zip::m_FileNum=0;
	TUByte* Zip::m_TempBuf=0;
	TSIZE Zip::m_TempSize=0;

	Zip::Zip(TVOID)
	{
	}

	Zip::~Zip(TVOID)
	{
	}

	TVOID Zip::ClearBuffer()
	{
		if(m_TempSize>0)
		{
			free(m_TempBuf);
			m_TempBuf=0;
		}
		m_TempSize=0;
		m_FileNum=0;
	}

	TS32 Zip::def(TUByte *src, TU32 srcSize, TUByte *dest, TU32 *destSize, TS32 level)
	{
		if(!src || !dest || !destSize)
			return Z_ERRNO;
		z_stream stream;
		TS32 err;
		stream.zalloc=Z_NULL;
		stream.zfree=Z_NULL;
		stream.opaque=Z_NULL;
		err=deflateInit(&stream,level);
		if(err!=Z_OK)
		{
			return err;
		}
		stream.avail_in = srcSize;
		stream.avail_out = srcSize;
		stream.next_in = (TUByte*)src;
		stream.next_out = (TUByte*)dest;
		err=deflate(&stream,Z_FINISH);
		if(err<0)
		{
			deflateEnd(&stream);
			return err;
		}
		(*destSize)=srcSize-stream.avail_out;
		deflateEnd(&stream);
		return Z_OK;
	}

	TS32 Zip::inf(TUByte* src, TU32 srcSize, TUByte* dest, TU32 origSize)
	{
		if(!src || !dest)
			return Z_ERRNO;
		z_stream stream;
		TS32 err;
		stream.zalloc=Z_NULL;
		stream.zfree=Z_NULL;
		stream.opaque=Z_NULL;
		err=inflateInit(&stream);
		if(err!=Z_OK)
		{
			return err;
		}
		stream.avail_in = srcSize;
		stream.avail_out = origSize;
		stream.next_in = (TUByte*)src;
		stream.next_out = (TUByte*)dest;
		err=inflate(&stream,Z_NO_FLUSH);
		if(err<0)
		{
			inflateEnd(&stream);
			return err;
		}
		inflateEnd(&stream);
		return Z_OK;
	}

	TS32 Zip::AddCompressFile(TCHAR* filename)
	{
		if(!filename)
			return 0;
		TCHAR szNoPath[1024] = {0};
		TLunaEngine::String::CutFilePath(filename,szNoPath);
		// 文件名部分
		TU32 namelen = (TU32)strlen(szNoPath);
		TSIZE lastsize = m_TempSize;
		if(lastsize==0)
		{
			m_TempBuf = (TUByte*)malloc(sizeof(TU32)*2+namelen);
			memset(m_TempBuf,0,sizeof(TU32)*2+namelen);
			m_FileNum=1;
			(*((TU32*)m_TempBuf))=m_FileNum;
			m_TempSize+=sizeof(TU32);
		}
		else
		{
			m_TempBuf = (TUByte*)realloc(m_TempBuf,m_TempSize+sizeof(TU32)+namelen);
			memset(m_TempBuf+lastsize,0,sizeof(TU32)+namelen);
			m_FileNum+=1;
			(*((TU32*)m_TempBuf))=m_FileNum;
		}
		memcpy(m_TempBuf+m_TempSize,&namelen,sizeof(TU32));
		m_TempSize+=sizeof(TU32);
		memcpy(m_TempBuf+m_TempSize,szNoPath,namelen);
		m_TempSize+=namelen;
		// 文件内容部分
		FILE* file = 0;
		TS32 re = fopen_s(&file,filename,"r");
		if(re!=0)
		{
			return 0;
		}
		TSIZE readByte = 0;
		TUByte filebuf[ZIP_READ_FILE_LENGTH] ={0};
		TUByte* tmpbuf = 0;
		TSIZE tmpsize = 0;
		do
		{
			readByte=(TSIZE)fread(filebuf,1,ZIP_READ_FILE_LENGTH,file);
			if(ferror(file))
			{
				fclose(file);
				return 0;
			}
			TSIZE lasttmpsize = tmpsize;
			if(lasttmpsize==0)
			{
				tmpbuf=(TUByte*)malloc(readByte);
				memset(tmpbuf,0,readByte);
				tmpsize+=readByte;
			}
			else
			{
				tmpbuf=(TUByte*)realloc(tmpbuf,tmpsize+readByte);
				memset(tmpbuf+lasttmpsize,0,readByte);
				tmpsize+=readByte;
			}
			memcpy(tmpbuf+lasttmpsize,filebuf,readByte);
			memset(filebuf,0,ZIP_READ_FILE_LENGTH);
			if(feof(file))
				break;
		}while(TTRUE);
		m_TempBuf = (TUByte*)realloc(m_TempBuf,m_TempSize+sizeof(TU64)+tmpsize);
		memset(m_TempBuf+m_TempSize,0,sizeof(TU64)+tmpsize);
		// 注意写到文件中的SIZE要强制变成TU64
		TU64 tmpsizeForWrite = (TU64)tmpsize;
		memcpy(m_TempBuf+m_TempSize,&tmpsizeForWrite,sizeof(TU64));
		m_TempSize+=sizeof(TU64);
		memcpy(m_TempBuf+m_TempSize,tmpbuf,tmpsize);
		m_TempSize+=tmpsize;
		fclose(file);
		free(tmpbuf);
		return 1;
	}

	TS32 Zip::CompressToFile(TCHAR* zipname)
	{
		if(!zipname)
			return 0;
		if(m_TempSize<=0)
			return 0;
		FILE* output = 0;
		TS32 re = fopen_s(&output,zipname,"w");
		if(re!=0)
			return 0;
		TUByte* outbuf = new TUByte[m_TempSize];
		TU32 outsize = 0;
		if(def(m_TempBuf,(TU32)m_TempSize,outbuf,&outsize,Z_DEFAULT_COMPRESSION)!=Z_OK)
		{
			fclose(output);
			delete [] outbuf;
			return 0;
		}
		TU64 tempSizeForWrite = (TU64)m_TempSize;
		fwrite(&tempSizeForWrite,sizeof(TU64),1,output);
		if(ferror(output))
		{
			fclose(output);
			delete [] outbuf;
			return 0;
		}
		fwrite(outbuf,1,outsize,output);
		if(ferror(output))
		{
			fclose(output);
			delete [] outbuf;
			return 0;
		}
		delete [] outbuf;
		fclose(output);
		return 1;
	}

	TU64 Zip::DecompressToMem(TCHAR* zipname, TUByte** buf)
	{
		if(!zipname)
			return 0;
		// 读ZIP文件
		FILE* file = 0;
		TS32 re = fopen_s(&file,zipname,"r");
		if(re!=0)
			return 0;
		TSIZE readByte = 0;
		TUByte filebuf[ZIP_READ_FILE_LENGTH] ={0};
		TUByte* tmpbuf = 0;
		TSIZE tmpsize = 0;
		do
		{
			readByte=(TSIZE)fread(filebuf,1,ZIP_READ_FILE_LENGTH,file);
			if(ferror(file))
			{
				fclose(file);
				return 0;
			}
			TSIZE lasttmpsize = tmpsize;
			if(lasttmpsize==0)
			{
				tmpbuf=(TUByte*)malloc(readByte);
				memset(tmpbuf,0,readByte);
				tmpsize+=readByte;
			}
			else
			{
				tmpbuf=(TUByte*)realloc(tmpbuf,tmpsize+readByte);
				memset(tmpbuf+lasttmpsize,0,readByte);
				tmpsize+=readByte;
			}
			memcpy(tmpbuf+lasttmpsize,filebuf,readByte);
			memset(filebuf,0,ZIP_READ_FILE_LENGTH);
			if(feof(file))
				break;
		}while(TTRUE);
		fclose(file);
		// 读原尺寸
		TU64 origsize = *((TU64*)tmpbuf);
		if(origsize<=0)
			return 0;
		// 解压缩
		TUByte* out = new TUByte[origsize];
		if(inf(tmpbuf+sizeof(TU64),(TU32)(tmpsize-sizeof(TU64)),out,(TU32)origsize)!=Z_OK)
		{
			delete [] out;
			return 0;
		}
		if(buf)
		{
			TUByte* p = *buf;
			p = (TUByte*)malloc(origsize);
			memcpy(p,out,origsize);
		}
		else
		{
			ClearBuffer();
			m_TempBuf = (TUByte*)malloc(origsize);
			memcpy(m_TempBuf,out,origsize);
			m_TempSize = (TSIZE)origsize;
		}
		return origsize;
	}

	TS32 Zip::CopyMemToFile(TCHAR* dirname, TBOOL bBuildFile, MemFile **ppMemFile)
	{
		if(!dirname || m_TempSize<=0)
			return 0;
		TSIZE offset=0;
		// 文件数量
		m_FileNum = (TU32)(*m_TempBuf);
		offset+=sizeof(TU32);
		if(ppMemFile && m_FileNum>0)
		{
			(*ppMemFile) = new MemFile[m_FileNum];
		}
		// 循环生成文件
		for(TU32 i=0;i<m_FileNum;i++)
		{
			TU32 fileNameLen = 0;
			TCHAR szName[1024]={0};
			TCHAR szFull[1024]={0};
			TUByte* pContent=0;
			TSIZE conSize=0;

			// 文件名部分
			fileNameLen =(TU32)(*(m_TempBuf+offset));
			offset+=sizeof(TU32);
			memcpy(szName,m_TempBuf+offset, fileNameLen);
			sprintf_s(szFull,1024,"%s%s",dirname,szName);
			offset+= fileNameLen;
			// 文件内容
			TU64 fileSize=(TU64)(*(m_TempBuf+offset));
			offset+=sizeof(TU64);
			pContent=new TUByte[fileSize];
			memcpy(pContent,m_TempBuf+offset, (TSIZE)fileSize);
			conSize= (TSIZE)fileSize;
			offset+= (TSIZE)fileSize;

			// 生成
			if(bBuildFile)
			{
				FILE* outfile = 0;
				TS32 re = fopen_s(&outfile,szFull,"w");
				if(re==0)
				{
					fwrite(pContent,1,conSize,outfile);
					if(ferror(outfile))
					{
						fclose(outfile);
						delete [] pContent;
						return 0;
					}
					fclose(outfile);
				}
			}
			if(ppMemFile)
			{
				ppMemFile[i]->SetFullName(szFull);
				ppMemFile[i]->SetContent(pContent,conSize);
			}

			delete [] pContent;
		}
		return 1;
	}

}