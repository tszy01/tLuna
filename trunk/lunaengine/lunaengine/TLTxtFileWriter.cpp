#include "TLTxtFileWriter.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
namespace TLunaEngine{

	TxtFileWriter::TxtFileWriter(TVOID)
	{
	}

	TxtFileWriter::~TxtFileWriter(TVOID)
	{
	}

	TBOOL TxtFileWriter::OpenTxtFile(const TCHAR* filename, FILE **ppStream, TBOOL append)
	{
		if(!filename || !ppStream)
		{
			assert(TFALSE);
			return TFALSE;
		}
		// 打开文件
		const TCHAR* mode = append ? "at" : "wt";
		TS32 re = fopen_s(ppStream,filename,mode);
		if(*ppStream==0 || re!=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		return TTRUE;
	}

	TVOID TxtFileWriter::CloseTxtFile(FILE *pStream)
	{
		fclose(pStream);
	}

	TBOOL TxtFileWriter::WriteLineString(const TCHAR* strWrite,FILE* pStream, TU32 count)
	{
		if(!strWrite || !pStream)
			return TFALSE;
		if((TU32)fwrite(strWrite,sizeof(TCHAR),count,pStream) < count)
			return TFALSE;
		const TCHAR* szTmp = "\n";
		fwrite(szTmp,sizeof(TCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL TxtFileWriter::WriteLineWString(const TWCHAR* strWrite, FILE* pStream, TU32 count)
	{
		if (!strWrite || !pStream)
			return TFALSE;
		if ((TU32)fwrite(strWrite, sizeof(TWCHAR), count, pStream) < count)
			return TFALSE;
		const TWCHAR* szTmp = L"\n";
		fwrite(szTmp, sizeof(TWCHAR), 1, pStream);
		return TTRUE;
	}

	TBOOL TxtFileWriter::WriteLineInteger(const TS32* aiWrite,FILE* pStream, TU32 nCount,TCHAR splice)
	{
		if(!aiWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TCHAR szTmp[64] = {0};
			sprintf(szTmp,"%d",aiWrite[i]);
			fwrite(szTmp,sizeof(TCHAR),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(TCHAR),1,pStream);
			}
		}
		fwrite("\n",sizeof(TCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL TxtFileWriter::WriteLineLongLong(const TS64* allWrite,FILE* pStream, TU32 nCount,TCHAR splice)
	{
		if(!allWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TCHAR szTmp[64] = {0};
			sprintf(szTmp,"%lld",allWrite[i]);
			fwrite(szTmp,sizeof(TCHAR),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(TCHAR),1,pStream);
			}
		}
		fwrite("\n",sizeof(TCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL TxtFileWriter::WriteLineShort(const TS16* asWrite,FILE* pStream, TU32 nCount,TCHAR splice)
	{
		if(!asWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TCHAR szTmp[64] = {0};
			sprintf(szTmp,"%hd",asWrite[i]);
			fwrite(szTmp,sizeof(TCHAR),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(TCHAR),1,pStream);
			}
		}
		fwrite("\n",sizeof(TCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL TxtFileWriter::WriteLineUInteger(const TU32* auWrite,FILE* pStream, TU32 nCount,TCHAR splice)
	{
		if(!auWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TCHAR szTmp[64] = {0};
			sprintf(szTmp,"%u",auWrite[i]);
			fwrite(szTmp,sizeof(TCHAR),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(TCHAR),1,pStream);
			}
		}
		fwrite("\n",sizeof(TCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL TxtFileWriter::WriteLineULongLong(const TU64* aullWrite,FILE* pStream, TU32 nCount,TCHAR splice)
	{
		if(!aullWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TCHAR szTmp[64] = {0};
			sprintf(szTmp,"%llu",aullWrite[i]);
			fwrite(szTmp,sizeof(TCHAR),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(TCHAR),1,pStream);
			}
		}
		fwrite("\n",sizeof(TCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL TxtFileWriter::WriteLineUShort(const TU16* ausWrite,FILE* pStream, TU32 nCount,TCHAR splice)
	{
		if(!ausWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TCHAR szTmp[64] = {0};
			sprintf(szTmp,"%hu",ausWrite[i]);
			fwrite(szTmp,sizeof(TCHAR),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(TCHAR),1,pStream);
			}
		}
		fwrite("\n",sizeof(TCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL TxtFileWriter::WriteLineFloat(const TF32* afWrite,FILE* pStream, TU32 nCount,TCHAR splice)
	{
		if(!afWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TCHAR szTmp[64] = {0};
			sprintf(szTmp,"%.3f",afWrite[i]);
			fwrite(szTmp,sizeof(TCHAR),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(TCHAR),1,pStream);
			}
		}
		fwrite("\n",sizeof(TCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL TxtFileWriter::WriteLineDouble(const TF64* adWrite,FILE* pStream, TU32 nCount,TCHAR splice)
	{
		if(!adWrite || !pStream || nCount<=0)
			return TFALSE;
		for(TU32 i=0;i<nCount;i++)
		{
			TCHAR szTmp[64] = {0};
			sprintf(szTmp,"%.6lf",adWrite[i]);
			fwrite(szTmp,sizeof(TCHAR),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(TCHAR),1,pStream);
			}
		}
		fwrite("\n",sizeof(TCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL TxtFileWriter::WriteAllFile(const TCHAR* szFile,const TCHAR* mode,const TVOID* buffer, TU64 nCount)
	{
		if(!szFile || !mode || !buffer || nCount <=0)
			return TFALSE;
		FILE* file = 0;
		TS32 re = fopen_s(&file,szFile,mode);
		if(re!=0)
		{
			return TFALSE;
		}
		if((TU64)fwrite(buffer,1, nCount,file)!= nCount)
		{
			fclose(file);
			return TFALSE;
		}
		fclose(file);
		return TTRUE;
	}
}