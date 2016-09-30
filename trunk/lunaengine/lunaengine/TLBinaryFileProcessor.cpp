#include "TLBinaryFileProcessor.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
namespace TLunaEngine{
	TBOOL BinaryFileProcessor::openFile(const TCHAR* file,FILE** ppStream,const TCHAR* mode)
	{
		if(!file || !ppStream)
			return TFALSE;
		// 打开文件
		*ppStream = fopen(file,mode);
		if(*ppStream==0)
		{
			return TFALSE;
		}
		return TTRUE;
	}

	TVOID BinaryFileProcessor::closeFile(FILE* pStream)
	{
		fclose(pStream);
	}

	TBOOL BinaryFileProcessor::writeBytes(const TVOID* buffer,FILE* pStream,int sizeWrite)
	{
		if(!buffer || !pStream || sizeWrite<=0)
			return TFALSE;
		if(fwrite(buffer,1,sizeWrite,pStream)!=sizeWrite)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeBoolean(const TBOOL* abWrite,FILE* pStream,int nCount)
	{
		if(!abWrite || !pStream || nCount<=0)
			return TFALSE;
		if(fwrite(abWrite,sizeof(TBOOL),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeInteger(const int* aiWrite,FILE* pStream,int nCount)
	{
		if(!aiWrite || !pStream || nCount<=0)
			return TFALSE;
		if(fwrite(aiWrite,sizeof(int),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeUInteger(const unsigned int* auWrite,FILE* pStream,int nCount)
	{
		if(!auWrite || !pStream || nCount<=0)
			return TFALSE;
		if(fwrite(auWrite,sizeof(unsigned int),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeShort(const short* asWrite,FILE* pStream,int nCount)
	{
		if(!asWrite || !pStream || nCount<=0)
			return TFALSE;
		if(fwrite(asWrite,sizeof(short),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeUShort(const unsigned short* ausWrite,FILE* pStream,int nCount)
	{
		if(!ausWrite || !pStream || nCount<=0)
			return TFALSE;
		if(fwrite(ausWrite,sizeof(unsigned short),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeLong(const long* alWrite,FILE* pStream,int nCount)
	{
		if(!alWrite || !pStream || nCount<=0)
			return TFALSE;
		if(fwrite(alWrite,sizeof(long),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeULong(const unsigned long* aulWrite,FILE* pStream,int nCount)
	{
		if(!aulWrite || !pStream || nCount<=0)
			return TFALSE;
		if(fwrite(aulWrite,sizeof(unsigned long),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeFloat(const float* afWrite,FILE* pStream,int nCount)
	{
		if(!afWrite || !pStream || nCount<=0)
			return TFALSE;
		if(fwrite(afWrite,sizeof(float),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeDouble(const double* adWrite,FILE* pStream,int nCount)
	{
		if(!adWrite || !pStream || nCount<=0)
			return TFALSE;
		if(fwrite(adWrite,sizeof(double),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeString(const TCHAR* strWrite,FILE* pStream,size_t count)
	{
		if(!strWrite || !pStream)
			return TFALSE;
		// 先写数量
		size_t charCount = count;
		if(fwrite(&charCount,sizeof(size_t),1,pStream)!=1)
			return TFALSE;
		// 写内容
		if(fwrite(strWrite,sizeof(TCHAR),count,pStream) < count)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::readBytes(TVOID* buffer,FILE* pStream,int sizeRead)
	{
		if(!buffer || !pStream || sizeRead<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if(fread(buffer,1,sizeRead,pStream)!=sizeRead)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readBoolean(TBOOL* abResult,FILE* pStream,int nCount)
	{
		if(!abResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if(fread(abResult,sizeof(TBOOL),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readInteger(int* aiResult,FILE* pStream,int nCount)
	{
		if(!aiResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if(fread(aiResult,sizeof(int),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readUInteger(unsigned int* auResult,FILE* pStream,int nCount)
	{
		if(!auResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if(fread(auResult,sizeof(unsigned int),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readShort(short* asResult,FILE* pStream,int nCount)
	{
		if(!asResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if(fread(asResult,sizeof(short),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readUShort(unsigned short* ausResult,FILE* pStream,int nCount)
	{
		if(!ausResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if(fread(ausResult,sizeof(unsigned short),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readLong(long* alResult,FILE* pStream,int nCount)
	{
		if(!alResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if(fread(alResult,sizeof(long),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readULong(unsigned long* aulResult,FILE* pStream,int nCount)
	{
		if(!aulResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if(fread(aulResult,sizeof(unsigned long),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readFloat(float* afResult,FILE* pStream,int nCount)
	{
		if(!afResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if(fread(afResult,sizeof(float),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readDouble(double* adResult,FILE* pStream,int nCount)
	{
		if(!adResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if(fread(adResult,sizeof(double),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readString(TCHAR* strResult,FILE* pStream,const TCHAR* strCmp,TBOOL* bEqual,int nCount,int* pReadCount)
	{
		if(!strResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		memset(strResult,0,nCount);
		// 读取大小
		size_t strLen = 0;
		if(fread(&strLen,sizeof(size_t),1,pStream)!=1)
			return TFALSE;
		if(strLen<=0)
		{
			if(pReadCount)
				*pReadCount = (int)strLen;
			return TTRUE;
		}
		TCHAR* szRead = new TCHAR[strLen];
		memset(szRead,0,strLen);
		if(fread(szRead,sizeof(TCHAR),strLen,pStream)!=strLen)
		{
			delete [] szRead;
			return TFALSE;
		}
		if(nCount < (int)strLen)
		{
			delete [] szRead;
			return TFALSE;
		}
		strcpy(strResult,szRead);
		if(pReadCount)
			*pReadCount = (int)strLen;
		if(strLen>0 && strCmp && bEqual)
		{
			if(strcmp(strResult,strCmp)==0)
			{
				*bEqual=TTRUE;
			}
			else
			{
				*bEqual=TFALSE;
			}
		}
		delete [] szRead;
		return TTRUE;
	}
}