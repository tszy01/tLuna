#include "TLZip.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "zlib.h"

#include "TLString.h"

namespace TLunaEngine{

	int Zip::m_FileNum=0;
	TUByte* Zip::m_TempBuf=0;
	int Zip::m_TempSize=0;

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

	int Zip::def(TUByte *src, int srcSize, TUByte *dest, int *destSize, int level)
	{
		if(!src || !dest || !destSize)
			return Z_ERRNO;
		z_stream stream;
		int err;
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

	int Zip::inf(TUByte* src,int srcSize, TUByte* dest,int origSize)
	{
		if(!src || !dest)
			return Z_ERRNO;
		z_stream stream;
		int err;
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

	int Zip::AddCompressFile(TCHAR* filename)
	{
		if(!filename)
			return 0;
		TCHAR szNoPath[1024] = {0};
		TLunaEngine::String::CutFilePath(filename,szNoPath);
		// 文件名部分
		int namelen = (int)strlen(szNoPath);
		int lastsize = m_TempSize;
		if(lastsize==0)
		{
			m_TempBuf = (TUByte*)malloc(sizeof(int)*2+namelen);
			memset(m_TempBuf,0,sizeof(int)*2+namelen);
			m_FileNum=1;
			(*((int*)m_TempBuf))=m_FileNum;
			m_TempSize+=sizeof(int);
		}
		else
		{
			m_TempBuf = (TUByte*)realloc(m_TempBuf,m_TempSize+sizeof(int)+namelen);
			memset(m_TempBuf+lastsize,0,sizeof(int)+namelen);
			m_FileNum+=1;
			(*((int*)m_TempBuf))=m_FileNum;
		}
		memcpy(m_TempBuf+m_TempSize,&namelen,sizeof(int));
		m_TempSize+=sizeof(int);
		memcpy(m_TempBuf+m_TempSize,szNoPath,namelen);
		m_TempSize+=namelen;
		// 文件内容部分
		FILE* file = 0;
		int re = fopen_s(&file,filename,"r");
		if(re!=0)
		{
			return 0;
		}
		int readByte = 0;
		TUByte filebuf[ZIP_READ_FILE_LENGTH] ={0};
		TUByte* tmpbuf = 0;
		int tmpsize = 0;
		do
		{
			readByte=(int)fread(filebuf,1,ZIP_READ_FILE_LENGTH,file);
			if(ferror(file))
			{
				fclose(file);
				return 0;
			}
			int lasttmpsize = tmpsize;
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
		m_TempBuf = (TUByte*)realloc(m_TempBuf,m_TempSize+sizeof(int)+tmpsize);
		memset(m_TempBuf+m_TempSize,0,sizeof(int)+tmpsize);
		memcpy(m_TempBuf+m_TempSize,&tmpsize,sizeof(int));
		m_TempSize+=sizeof(int);
		memcpy(m_TempBuf+m_TempSize,tmpbuf,tmpsize);
		m_TempSize+=tmpsize;
		fclose(file);
		free(tmpbuf);
		return 1;
	}

	int Zip::CompressToFile(TCHAR* zipname)
	{
		if(!zipname)
			return 0;
		if(m_TempSize<=0)
			return 0;
		FILE* output = 0;
		int re = fopen_s(&output,zipname,"w");
		if(re!=0)
			return 0;
		TUByte* outbuf = new TUByte[m_TempSize];
		int outsize = 0;
		if(def(m_TempBuf,m_TempSize,outbuf,&outsize,Z_DEFAULT_COMPRESSION)!=Z_OK)
		{
			fclose(output);
			delete [] outbuf;
			return 0;
		}
		fwrite(&m_TempSize,sizeof(int),1,output);
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

	int Zip::DecompressToMem(TCHAR* zipname, TUByte** buf)
	{
		if(!zipname)
			return 0;
		// 读ZIP文件
		FILE* file = 0;
		int re = fopen_s(&file,zipname,"r");
		if(re!=0)
			return 0;
		int readByte = 0;
		TUByte filebuf[ZIP_READ_FILE_LENGTH] ={0};
		TUByte* tmpbuf = 0;
		int tmpsize = 0;
		do
		{
			readByte=(int)fread(filebuf,1,ZIP_READ_FILE_LENGTH,file);
			if(ferror(file))
			{
				fclose(file);
				return 0;
			}
			int lasttmpsize = tmpsize;
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
		int origsize = *((int*)tmpbuf);
		if(origsize<=0)
			return 0;
		// 解压缩
		TUByte* out = new TUByte[origsize];
		if(inf(tmpbuf+sizeof(int),tmpsize-sizeof(int),out,origsize)!=Z_OK)
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
			m_TempSize = origsize;
		}
		return origsize;
	}

	int Zip::CopyMemToFile(TCHAR* dirname, TBOOL bBuildFile, MemFile **ppMemFile)
	{
		if(!dirname || m_TempSize<=0)
			return 0;
		int num=0;
		int offset=0;
		// 文件数量
		m_FileNum = (int)(*m_TempBuf);
		offset+=sizeof(int);
		if(ppMemFile && m_FileNum>0)
		{
			(*ppMemFile) = new MemFile[m_FileNum];
		}
		// 循环生成文件
		for(int i=0;i<m_FileNum;i++)
		{
			TCHAR szName[1024]={0};
			TCHAR szFull[1024]={0};
			TUByte* pContent=0;
			int conSize=0;

			// 文件名部分
			num=(int)(*(m_TempBuf+offset));
			offset+=sizeof(int);
			memcpy(szName,m_TempBuf+offset,num);
			sprintf_s(szFull,1024,"%s%s",dirname,szName);
			offset+=num;
			// 文件内容
			num=(int)(*(m_TempBuf+offset));
			offset+=sizeof(int);
			pContent=new TUByte[num];
			memcpy(pContent,m_TempBuf+offset,num);
			conSize=num;
			offset+=num;

			// 生成
			if(bBuildFile)
			{
				FILE* outfile = 0;
				int re = fopen_s(&outfile,szFull,"w");
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