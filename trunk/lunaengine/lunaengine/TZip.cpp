#include "StdAfx.h"
#include "TZip.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "zlib.h"

#include "TString.h"
using namespace TLunaEngine;

namespace TLunaEngine{

	int TZip::m_FileNum=0;
	unsigned char* TZip::m_TempBuf=0;
	int TZip::m_TempSize=0;

	TZip::TZip(void)
	{
	}

	TZip::~TZip(void)
	{
	}

	void TZip::ClearBuffer()
	{
		if(m_TempSize>0)
		{
			free(m_TempBuf);
			m_TempBuf=NULL;
		}
		m_TempSize=0;
		m_FileNum=0;
	}

	int TZip::def(unsigned char *src, int srcSize, unsigned char *dest, int *destSize, int level)
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
		stream.next_in = (unsigned char*)src;
		stream.next_out = (unsigned char*)dest;
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

	int TZip::inf(unsigned char* src,int srcSize,unsigned char* dest,int origSize)
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
		stream.next_in = (unsigned char*)src;
		stream.next_out = (unsigned char*)dest;
		err=inflate(&stream,Z_NO_FLUSH);
		if(err<0)
		{
			inflateEnd(&stream);
			return err;
		}
		inflateEnd(&stream);
		return Z_OK;
	}

	int TZip::AddCompressFile(char *filename)
	{
		if(!filename)
			return 0;
		char szNoPath[1024] = {0};
		TLunaEngine::TString::CutFilePath(filename,szNoPath);
		// 文件名部分
		int namelen = (int)strlen(szNoPath);
		int lastsize = m_TempSize;
		if(lastsize==0)
		{
			m_TempBuf = (unsigned char*)malloc(sizeof(int)*2+namelen);
			memset(m_TempBuf,0,sizeof(int)*2+namelen);
			m_FileNum=1;
			(*((int*)m_TempBuf))=m_FileNum;
			m_TempSize+=sizeof(int);
		}
		else
		{
			m_TempBuf = (unsigned char*)realloc(m_TempBuf,m_TempSize+sizeof(int)+namelen);
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
		unsigned char filebuf[ZIP_READ_FILE_LENGTH] ={0};
		unsigned char* tmpbuf = 0;
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
				tmpbuf=(unsigned char*)malloc(readByte);
				memset(tmpbuf,0,readByte);
				tmpsize+=readByte;
			}
			else
			{
				tmpbuf=(unsigned char*)realloc(tmpbuf,tmpsize+readByte);
				memset(tmpbuf+lasttmpsize,0,readByte);
				tmpsize+=readByte;
			}
			memcpy(tmpbuf+lasttmpsize,filebuf,readByte);
			memset(filebuf,0,ZIP_READ_FILE_LENGTH);
			if(feof(file))
				break;
		}while(true);
		m_TempBuf = (unsigned char*)realloc(m_TempBuf,m_TempSize+sizeof(int)+tmpsize);
		memset(m_TempBuf+m_TempSize,0,sizeof(int)+tmpsize);
		memcpy(m_TempBuf+m_TempSize,&tmpsize,sizeof(int));
		m_TempSize+=sizeof(int);
		memcpy(m_TempBuf+m_TempSize,tmpbuf,tmpsize);
		m_TempSize+=tmpsize;
		fclose(file);
		free(tmpbuf);
		return 1;
	}

	int TZip::CompressToFile(char *zipname)
	{
		if(!zipname)
			return 0;
		if(m_TempSize<=0)
			return 0;
		FILE* output = 0;
		int re = fopen_s(&output,zipname,"w");
		if(re!=0)
			return 0;
		unsigned char* outbuf = new unsigned char[m_TempSize];
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

	int TZip::DecompressToMem(char *zipname,unsigned char** buf)
	{
		if(!zipname)
			return 0;
		// 读ZIP文件
		FILE* file = 0;
		int re = fopen_s(&file,zipname,"r");
		if(re!=0)
			return 0;
		int readByte = 0;
		unsigned char filebuf[ZIP_READ_FILE_LENGTH] ={0};
		unsigned char* tmpbuf = 0;
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
				tmpbuf=(unsigned char*)malloc(readByte);
				memset(tmpbuf,0,readByte);
				tmpsize+=readByte;
			}
			else
			{
				tmpbuf=(unsigned char*)realloc(tmpbuf,tmpsize+readByte);
				memset(tmpbuf+lasttmpsize,0,readByte);
				tmpsize+=readByte;
			}
			memcpy(tmpbuf+lasttmpsize,filebuf,readByte);
			memset(filebuf,0,ZIP_READ_FILE_LENGTH);
			if(feof(file))
				break;
		}while(true);
		fclose(file);
		// 读原尺寸
		int origsize = *((int*)tmpbuf);
		if(origsize<=0)
			return 0;
		// 解压缩
		unsigned char* out = new unsigned char[origsize];
		if(inf(tmpbuf+sizeof(int),tmpsize-sizeof(int),out,origsize)!=Z_OK)
		{
			delete [] out;
			return 0;
		}
		if(buf)
		{
			unsigned char* p = *buf;
			p = (unsigned char*)malloc(origsize);
			memcpy(p,out,origsize);
		}
		else
		{
			ClearBuffer();
			m_TempBuf = (unsigned char*)malloc(origsize);
			memcpy(m_TempBuf,out,origsize);
			m_TempSize = origsize;
		}
		return origsize;
	}

	int TZip::CopyMemToFile(char *dirname, bool bBuildFile, TMemFile **ppMemFile)
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
			(*ppMemFile) = new TMemFile[m_FileNum];
		}
		// 循环生成文件
		for(int i=0;i<m_FileNum;i++)
		{
			char szName[1024]={0};
			char szFull[1024]={0};
			unsigned char* pContent=0;
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
			pContent=new unsigned char[num];
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