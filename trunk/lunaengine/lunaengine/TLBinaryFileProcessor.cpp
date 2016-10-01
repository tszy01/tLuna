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

	TBOOL BinaryFileProcessor::writeBytes(const TVOID* buffer,FILE* pStream, TU64 nCount)
	{
		if(!buffer || !pStream || nCount <=0)
			return TFALSE;
		if((TU64)fwrite(buffer,1, nCount,pStream)!= nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeBoolean(const TBOOL* abWrite,FILE* pStream, TU32 nCount)
	{
		if(!abWrite || !pStream || nCount<=0)
			return TFALSE;
		if((TU32)fwrite(abWrite,sizeof(TBOOL),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeInteger(const TS32* aiWrite,FILE* pStream, TU32 nCount)
	{
		if(!aiWrite || !pStream || nCount<=0)
			return TFALSE;
		if((TU32)fwrite(aiWrite,sizeof(TS32),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeUInteger(const TU32* auWrite,FILE* pStream, TU32 nCount)
	{
		if(!auWrite || !pStream || nCount<=0)
			return TFALSE;
		if((TU32)fwrite(auWrite,sizeof(TU32),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeShort(const TS16* asWrite,FILE* pStream, TU32 nCount)
	{
		if(!asWrite || !pStream || nCount<=0)
			return TFALSE;
		if(fwrite(asWrite,sizeof(TS16),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeUShort(const TU16* ausWrite,FILE* pStream, TU32 nCount)
	{
		if(!ausWrite || !pStream || nCount<=0)
			return TFALSE;
		if((TU32)fwrite(ausWrite,sizeof(TU16),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeLongLong(const TS64* allWrite,FILE* pStream, TU32 nCount)
	{
		if(!allWrite || !pStream || nCount<=0)
			return TFALSE;
		if((TU32)fwrite(allWrite,sizeof(TS64),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeULongLong(const TU64* aullWrite,FILE* pStream, TU32 nCount)
	{
		if(!aullWrite || !pStream || nCount<=0)
			return TFALSE;
		if((TU32)fwrite(aullWrite,sizeof(TU64),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeFloat(const TF32* afWrite,FILE* pStream, TU32 nCount)
	{
		if(!afWrite || !pStream || nCount<=0)
			return TFALSE;
		if((TU32)fwrite(afWrite,sizeof(TF32),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeDouble(const TF64* adWrite,FILE* pStream, TU32 nCount)
	{
		if(!adWrite || !pStream || nCount<=0)
			return TFALSE;
		if((TU32)fwrite(adWrite,sizeof(TF64),nCount,pStream)!=nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::writeString(const TCHAR* strWrite,FILE* pStream, TU32 nCount)
	{
		if(!strWrite || !pStream)
			return TFALSE;
		// 先写数量
		TU32 charCount = nCount;
		if(fwrite(&charCount,sizeof(TU32),1,pStream)!=1)
			return TFALSE;
		// 写内容
		if((TU32)fwrite(strWrite,sizeof(TCHAR), nCount,pStream) < nCount)
			return TFALSE;
		return TTRUE;
	}

	TBOOL BinaryFileProcessor::readBytes(TVOID* buffer,FILE* pStream, TU64 nCount)
	{
		if(!buffer || !pStream || nCount <=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if((TU64)fread(buffer,1, nCount,pStream)!= nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readBoolean(TBOOL* abResult,FILE* pStream, TU32 nCount)
	{
		if(!abResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if((TU32)fread(abResult,sizeof(TBOOL),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readInteger(TS32* aiResult,FILE* pStream, TU32 nCount)
	{
		if(!aiResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if((TU32)fread(aiResult,sizeof(TS32),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readUInteger(TU32* auResult,FILE* pStream, TU32 nCount)
	{
		if(!auResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if((TU32)fread(auResult,sizeof(TU32),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readShort(TS16* asResult,FILE* pStream, TU32 nCount)
	{
		if(!asResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if((TU32)fread(asResult,sizeof(TS16),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readUShort(TU16* ausResult,FILE* pStream, TU32 nCount)
	{
		if(!ausResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if((TU32)fread(ausResult,sizeof(TU16),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readLongLong(TS64* allResult,FILE* pStream, TU32 nCount)
	{
		if(!allResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if((TU32)fread(allResult,sizeof(TS64),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readULongLong(TU64* aullResult,FILE* pStream, TU32 nCount)
	{
		if(!aullResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if((TU32)fread(aullResult,sizeof(TU64),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readFloat(TF32* afResult,FILE* pStream, TU32 nCount)
	{
		if(!afResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if((TU32)fread(afResult,sizeof(TF32),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readDouble(TF64* adResult,FILE* pStream, TU32 nCount)
	{
		if(!adResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		if((TU32)fread(adResult,sizeof(TF64),nCount,pStream)!=nCount)
			return TFALSE;
		return TFALSE;
	}

	TBOOL BinaryFileProcessor::readString(TCHAR* strResult,FILE* pStream,const TCHAR* strCmp,TBOOL* bEqual, TU32 nCount, TU32* pReadCount)
	{
		if(!strResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		memset(strResult,0,nCount);
		// 读取大小
		TU32 strLen = 0;
		if(fread(&strLen,sizeof(TU32),1,pStream)!=1)
			return TFALSE;
		if(strLen<=0)
		{
			if(pReadCount)
				*pReadCount = strLen;
			return TTRUE;
		}
		TCHAR* szRead = new TCHAR[strLen];
		memset(szRead,0,strLen);
		if((TU32)fread(szRead,sizeof(TCHAR),strLen,pStream)!=strLen)
		{
			delete [] szRead;
			return TFALSE;
		}
		if(nCount < strLen)
		{
			delete [] szRead;
			return TFALSE;
		}
		strcpy(strResult,szRead);
		if(pReadCount)
			*pReadCount = strLen;
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