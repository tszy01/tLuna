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

	TBOOL TxtFileWriter::OpenTxtFile(const TCHAR* filename, FILE **ppStream)
	{
		if(!filename || !ppStream)
		{
			assert(TFALSE);
			return TFALSE;
		}
		// 打开文件
		int re = fopen_s(ppStream,filename,"wt");
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

	TBOOL TxtFileWriter::WriteLineString(const TCHAR* strWrite,FILE* pStream,int count)
	{
		if(!strWrite || !pStream)
			return TFALSE;
		if((int)fwrite(strWrite,sizeof(TCHAR),count,pStream) < count)
			return TFALSE;
		const TCHAR* szTmp = "\n";
		fwrite(szTmp,sizeof(TCHAR),1,pStream);
		return TTRUE;
	}

	TBOOL TxtFileWriter::WriteLineInteger(const int* aiWrite,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!aiWrite || !pStream || nCount<=0)
			return TFALSE;
		for(int i=0;i<nCount;i++)
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

	TBOOL TxtFileWriter::WriteLineLong(const long* alWrite,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!alWrite || !pStream || nCount<=0)
			return TFALSE;
		for(int i=0;i<nCount;i++)
		{
			TCHAR szTmp[64] = {0};
			sprintf(szTmp,"%ld",alWrite[i]);
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

	TBOOL TxtFileWriter::WriteLineShort(const short* asWrite,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!asWrite || !pStream || nCount<=0)
			return TFALSE;
		for(int i=0;i<nCount;i++)
		{
			TCHAR szTmp[64] = {0};
			sprintf(szTmp,"%d",asWrite[i]);
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

	TBOOL TxtFileWriter::WriteLineUInteger(const unsigned int* auWrite,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!auWrite || !pStream || nCount<=0)
			return TFALSE;
		for(int i=0;i<nCount;i++)
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

	TBOOL TxtFileWriter::WriteLineULong(const unsigned long* aulWrite,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!aulWrite || !pStream || nCount<=0)
			return TFALSE;
		for(int i=0;i<nCount;i++)
		{
			TCHAR szTmp[64] = {0};
			sprintf(szTmp,"%lu",aulWrite[i]);
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

	TBOOL TxtFileWriter::WriteLineUShort(const unsigned short* ausWrite,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!ausWrite || !pStream || nCount<=0)
			return TFALSE;
		for(int i=0;i<nCount;i++)
		{
			TCHAR szTmp[64] = {0};
			sprintf(szTmp,"%u",ausWrite[i]);
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

	TBOOL TxtFileWriter::WriteLineFloat(const float* afWrite,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!afWrite || !pStream || nCount<=0)
			return TFALSE;
		for(int i=0;i<nCount;i++)
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

	TBOOL TxtFileWriter::WriteLineDouble(const double* adWrite,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!adWrite || !pStream || nCount<=0)
			return TFALSE;
		for(int i=0;i<nCount;i++)
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

	TBOOL TxtFileWriter::WriteAllFile(const TCHAR* szFile,const TCHAR* mode,const TVOID* buffer,size_t writeByte)
	{
		if(!szFile || !mode || !buffer || writeByte<=0)
			return TFALSE;
		FILE* file = 0;
		int re = fopen_s(&file,szFile,mode);
		if(re!=0)
		{
			return TFALSE;
		}
		if(fwrite(buffer,1,writeByte,file)!=writeByte)
		{
			fclose(file);
			return TFALSE;
		}
		fclose(file);
		return TTRUE;
	}
}