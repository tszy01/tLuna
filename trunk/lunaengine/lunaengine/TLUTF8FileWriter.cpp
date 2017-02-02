#include "TLUTF8FileWriter.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
namespace TLunaEngine{

	UTF8FileWriter::UTF8FileWriter(TVOID)
	{
	}

	UTF8FileWriter::~UTF8FileWriter(TVOID)
	{
	}

	TBOOL UTF8FileWriter::OpenTxtFile(const TCHAR* filename, FILE **ppStream, TBOOL append)
	{
		if(!filename || !ppStream)
		{
			assert(TFALSE);
			return TFALSE;
		}
		// 打开文件
		const TCHAR* mode = append ? "at, ccs=UTF-8" : "wt, ccs=UTF-8";
		TS32 re = fopen_s(ppStream,filename,mode);
		if(*ppStream==0 || re!=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		return TTRUE;
	}

	TVOID UTF8FileWriter::CloseTxtFile(FILE *pStream)
	{
		fclose(pStream);
	}

	TBOOL UTF8FileWriter::WriteLineWString(const TWCHAR* strWrite, FILE* pStream, TU32 count)
	{
		if (!strWrite || !pStream)
			return TFALSE;
		if ((TU32)fwrite(strWrite, sizeof(TWCHAR), count, pStream) < count)
			return TFALSE;
		const TWCHAR* szTmp = L"\n";
		fwrite(szTmp, sizeof(TWCHAR), 1, pStream);
		return TTRUE;
	}

	TBOOL UTF8FileWriter::WriteLineInteger(const TS32* aiWrite,FILE* pStream, TU32 nCount, TWCHAR splice)
	{
		if(!aiWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TWCHAR szTmp[64] = {0};
			swprintf(szTmp,L"%d",aiWrite[i]);
			fwrite(szTmp,sizeof(TWCHAR),wcslen(szTmp),pStream);
			if(i<nCount-1)
			{
				swprintf(szTmp,L"%c",splice);
				fwrite(szTmp,sizeof(TWCHAR),1,pStream);
			}
		}
		fwrite(L"\n",sizeof(TWCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL UTF8FileWriter::WriteLineLongLong(const TS64* allWrite,FILE* pStream, TU32 nCount, TWCHAR splice)
	{
		if(!allWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TWCHAR szTmp[64] = {0};
			swprintf(szTmp,L"%lld",allWrite[i]);
			fwrite(szTmp,sizeof(TWCHAR),wcslen(szTmp),pStream);
			if(i<nCount-1)
			{
				swprintf(szTmp,L"%c",splice);
				fwrite(szTmp,sizeof(TWCHAR),1,pStream);
			}
		}
		fwrite(L"\n",sizeof(TWCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL UTF8FileWriter::WriteLineShort(const TS16* asWrite,FILE* pStream, TU32 nCount, TWCHAR splice)
	{
		if(!asWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TWCHAR szTmp[64] = {0};
			swprintf(szTmp,L"%hd",asWrite[i]);
			fwrite(szTmp,sizeof(TWCHAR),wcslen(szTmp),pStream);
			if(i<nCount-1)
			{
				swprintf(szTmp,L"%c",splice);
				fwrite(szTmp,sizeof(TWCHAR),1,pStream);
			}
		}
		fwrite(L"\n",sizeof(TWCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL UTF8FileWriter::WriteLineUInteger(const TU32* auWrite,FILE* pStream, TU32 nCount, TWCHAR splice)
	{
		if(!auWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TWCHAR szTmp[64] = {0};
			swprintf(szTmp,L"%u",auWrite[i]);
			fwrite(szTmp,sizeof(TWCHAR),wcslen(szTmp),pStream);
			if(i<nCount-1)
			{
				swprintf(szTmp,L"%c",splice);
				fwrite(szTmp,sizeof(TWCHAR),1,pStream);
			}
		}
		fwrite(L"\n",sizeof(TWCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL UTF8FileWriter::WriteLineULongLong(const TU64* aullWrite,FILE* pStream, TU32 nCount, TWCHAR splice)
	{
		if(!aullWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TWCHAR szTmp[64] = {0};
			swprintf(szTmp,L"%llu",aullWrite[i]);
			fwrite(szTmp,sizeof(TWCHAR),wcslen(szTmp),pStream);
			if(i<nCount-1)
			{
				swprintf(szTmp,L"%c",splice);
				fwrite(szTmp,sizeof(TWCHAR),1,pStream);
			}
		}
		fwrite(L"\n",sizeof(TWCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL UTF8FileWriter::WriteLineUShort(const TU16* ausWrite,FILE* pStream, TU32 nCount, TWCHAR splice)
	{
		if(!ausWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TWCHAR szTmp[64] = {0};
			swprintf(szTmp,L"%hu",ausWrite[i]);
			fwrite(szTmp,sizeof(TWCHAR),wcslen(szTmp),pStream);
			if(i<nCount-1)
			{
				swprintf(szTmp,L"%c",splice);
				fwrite(szTmp,sizeof(TWCHAR),1,pStream);
			}
		}
		fwrite(L"\n",sizeof(TWCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL UTF8FileWriter::WriteLineFloat(const TF32* afWrite,FILE* pStream, TU32 nCount, TWCHAR splice)
	{
		if(!afWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TWCHAR szTmp[64] = {0};
			swprintf(szTmp,L"%.3f",afWrite[i]);
			fwrite(szTmp,sizeof(TWCHAR),wcslen(szTmp),pStream);
			if(i<nCount-1)
			{
				swprintf(szTmp,L"%c",splice);
				fwrite(szTmp,sizeof(TWCHAR),1,pStream);
			}
		}
		fwrite(L"\n",sizeof(TWCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL UTF8FileWriter::WriteLineDouble(const TF64* adWrite,FILE* pStream, TU32 nCount, TWCHAR splice)
	{
		if(!adWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TWCHAR szTmp[64] = {0};
			swprintf(szTmp,L"%.6lf",adWrite[i]);
			fwrite(szTmp,sizeof(TWCHAR),wcslen(szTmp),pStream);
			if(i<nCount-1)
			{
				swprintf(szTmp,L"%c",splice);
				fwrite(szTmp,sizeof(TWCHAR),1,pStream);
			}
		}
		fwrite(L"\n",sizeof(TWCHAR),1,pStream);
		return TTRUE;
	}
}