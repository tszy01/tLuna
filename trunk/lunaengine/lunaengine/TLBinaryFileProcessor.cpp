#include "TLBinaryFileProcessor.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
namespace TLunaEngine{
	bool BinaryFileProcessor::openFile(const char* file,FILE** ppStream,const char* mode)
	{
		if(!file || !ppStream)
			return false;
		// 打开文件
		*ppStream = fopen(file,mode);
		if(*ppStream==0)
		{
			return false;
		}
		return true;
	}

	void BinaryFileProcessor::closeFile(FILE* pStream)
	{
		fclose(pStream);
	}

	bool BinaryFileProcessor::writeBytes(const void* buffer,FILE* pStream,int sizeWrite)
	{
		if(!buffer || !pStream || sizeWrite<=0)
			return false;
		if(fwrite(buffer,1,sizeWrite,pStream)!=sizeWrite)
			return false;
		return true;
	}

	bool BinaryFileProcessor::writeBoolean(const bool* abWrite,FILE* pStream,int nCount)
	{
		if(!abWrite || !pStream || nCount<=0)
			return false;
		if(fwrite(abWrite,sizeof(bool),nCount,pStream)!=nCount)
			return false;
		return true;
	}

	bool BinaryFileProcessor::writeInteger(const int* aiWrite,FILE* pStream,int nCount)
	{
		if(!aiWrite || !pStream || nCount<=0)
			return false;
		if(fwrite(aiWrite,sizeof(int),nCount,pStream)!=nCount)
			return false;
		return true;
	}

	bool BinaryFileProcessor::writeUInteger(const unsigned int* auWrite,FILE* pStream,int nCount)
	{
		if(!auWrite || !pStream || nCount<=0)
			return false;
		if(fwrite(auWrite,sizeof(unsigned int),nCount,pStream)!=nCount)
			return false;
		return true;
	}

	bool BinaryFileProcessor::writeShort(const short* asWrite,FILE* pStream,int nCount)
	{
		if(!asWrite || !pStream || nCount<=0)
			return false;
		if(fwrite(asWrite,sizeof(short),nCount,pStream)!=nCount)
			return false;
		return true;
	}

	bool BinaryFileProcessor::writeUShort(const unsigned short* ausWrite,FILE* pStream,int nCount)
	{
		if(!ausWrite || !pStream || nCount<=0)
			return false;
		if(fwrite(ausWrite,sizeof(unsigned short),nCount,pStream)!=nCount)
			return false;
		return true;
	}

	bool BinaryFileProcessor::writeLong(const long* alWrite,FILE* pStream,int nCount)
	{
		if(!alWrite || !pStream || nCount<=0)
			return false;
		if(fwrite(alWrite,sizeof(long),nCount,pStream)!=nCount)
			return false;
		return true;
	}

	bool BinaryFileProcessor::writeULong(const unsigned long* aulWrite,FILE* pStream,int nCount)
	{
		if(!aulWrite || !pStream || nCount<=0)
			return false;
		if(fwrite(aulWrite,sizeof(unsigned long),nCount,pStream)!=nCount)
			return false;
		return true;
	}

	bool BinaryFileProcessor::writeFloat(const float* afWrite,FILE* pStream,int nCount)
	{
		if(!afWrite || !pStream || nCount<=0)
			return false;
		if(fwrite(afWrite,sizeof(float),nCount,pStream)!=nCount)
			return false;
		return true;
	}

	bool BinaryFileProcessor::writeDouble(const double* adWrite,FILE* pStream,int nCount)
	{
		if(!adWrite || !pStream || nCount<=0)
			return false;
		if(fwrite(adWrite,sizeof(double),nCount,pStream)!=nCount)
			return false;
		return true;
	}

	bool BinaryFileProcessor::writeString(const char* strWrite,FILE* pStream,size_t count)
	{
		if(!strWrite || !pStream)
			return false;
		// 先写数量
		size_t charCount = count;
		if(fwrite(&charCount,sizeof(size_t),1,pStream)!=1)
			return false;
		// 写内容
		if(fwrite(strWrite,sizeof(char),count,pStream) < count)
			return false;
		return true;
	}

	bool BinaryFileProcessor::readBytes(void* buffer,FILE* pStream,int sizeRead)
	{
		if(!buffer || !pStream || sizeRead<=0)
		{
			assert(false);
			return false;
		}
		if(fread(buffer,1,sizeRead,pStream)!=sizeRead)
			return false;
		return false;
	}

	bool BinaryFileProcessor::readBoolean(bool* abResult,FILE* pStream,int nCount)
	{
		if(!abResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}
		if(fread(abResult,sizeof(bool),nCount,pStream)!=nCount)
			return false;
		return false;
	}

	bool BinaryFileProcessor::readInteger(int* aiResult,FILE* pStream,int nCount)
	{
		if(!aiResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}
		if(fread(aiResult,sizeof(int),nCount,pStream)!=nCount)
			return false;
		return false;
	}

	bool BinaryFileProcessor::readUInteger(unsigned int* auResult,FILE* pStream,int nCount)
	{
		if(!auResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}
		if(fread(auResult,sizeof(unsigned int),nCount,pStream)!=nCount)
			return false;
		return false;
	}

	bool BinaryFileProcessor::readShort(short* asResult,FILE* pStream,int nCount)
	{
		if(!asResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}
		if(fread(asResult,sizeof(short),nCount,pStream)!=nCount)
			return false;
		return false;
	}

	bool BinaryFileProcessor::readUShort(unsigned short* ausResult,FILE* pStream,int nCount)
	{
		if(!ausResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}
		if(fread(ausResult,sizeof(unsigned short),nCount,pStream)!=nCount)
			return false;
		return false;
	}

	bool BinaryFileProcessor::readLong(long* alResult,FILE* pStream,int nCount)
	{
		if(!alResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}
		if(fread(alResult,sizeof(long),nCount,pStream)!=nCount)
			return false;
		return false;
	}

	bool BinaryFileProcessor::readULong(unsigned long* aulResult,FILE* pStream,int nCount)
	{
		if(!aulResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}
		if(fread(aulResult,sizeof(unsigned long),nCount,pStream)!=nCount)
			return false;
		return false;
	}

	bool BinaryFileProcessor::readFloat(float* afResult,FILE* pStream,int nCount)
	{
		if(!afResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}
		if(fread(afResult,sizeof(float),nCount,pStream)!=nCount)
			return false;
		return false;
	}

	bool BinaryFileProcessor::readDouble(double* adResult,FILE* pStream,int nCount)
	{
		if(!adResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}
		if(fread(adResult,sizeof(double),nCount,pStream)!=nCount)
			return false;
		return false;
	}

	bool BinaryFileProcessor::readString(char* strResult,FILE* pStream,const char* strCmp,bool* bEqual,int nCount,int* pReadCount)
	{
		if(!strResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}
		memset(strResult,0,nCount);
		// 读取大小
		size_t strLen = 0;
		if(fread(&strLen,sizeof(size_t),1,pStream)!=1)
			return false;
		if(strLen<=0)
		{
			if(pReadCount)
				*pReadCount = (int)strLen;
			return true;
		}
		char* szRead = new char[strLen];
		memset(szRead,0,strLen);
		if(fread(szRead,sizeof(char),strLen,pStream)!=strLen)
		{
			delete [] szRead;
			return false;
		}
		if(nCount < (int)strLen)
		{
			delete [] szRead;
			return false;
		}
		strcpy(strResult,szRead);
		if(pReadCount)
			*pReadCount = (int)strLen;
		if(strLen>0 && strCmp && bEqual)
		{
			if(strcmp(strResult,strCmp)==0)
			{
				*bEqual=true;
			}
			else
			{
				*bEqual=false;
			}
		}
		delete [] szRead;
		return true;
	}
}