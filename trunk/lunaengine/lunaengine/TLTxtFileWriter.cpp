#include "TLTxtFileWriter.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
namespace TLunaEngine{

	TxtFileWriter::TxtFileWriter(void)
	{
	}

	TxtFileWriter::~TxtFileWriter(void)
	{
	}

	bool TxtFileWriter::OpenTxtFile(const char *filename, FILE **ppStream)
	{
		if(!filename || !ppStream)
		{
			assert(false);
			return false;
		}
		// 打开文件
		int re = fopen_s(ppStream,filename,"wt");
		if(*ppStream==0 || re!=0)
		{
			assert(false);
			return false;
		}
		return true;
	}

	void TxtFileWriter::CloseTxtFile(FILE *pStream)
	{
		fclose(pStream);
	}

	bool TxtFileWriter::WriteLineString(const char* strWrite,FILE* pStream,int count)
	{
		if(!strWrite || !pStream)
			return false;
		if((int)fwrite(strWrite,sizeof(char),count,pStream) < count)
			return false;
		const char* szTmp = "\n";
		fwrite(szTmp,sizeof(char),1,pStream);
		return true;
	}

	bool TxtFileWriter::WriteLineInteger(const int* aiWrite,FILE* pStream,int nCount,char splice)
	{
		if(!aiWrite || !pStream || nCount<=0)
			return false;
		for(int i=0;i<nCount;i++)
		{
			char szTmp[64] = {0};
			sprintf(szTmp,"%d",aiWrite[i]);
			fwrite(szTmp,sizeof(char),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(char),1,pStream);
			}
		}
		fwrite("\n",sizeof(char),1,pStream);
		return true;
	}

	bool TxtFileWriter::WriteLineLong(const long* alWrite,FILE* pStream,int nCount,char splice)
	{
		if(!alWrite || !pStream || nCount<=0)
			return false;
		for(int i=0;i<nCount;i++)
		{
			char szTmp[64] = {0};
			sprintf(szTmp,"%ld",alWrite[i]);
			fwrite(szTmp,sizeof(char),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(char),1,pStream);
			}
		}
		fwrite("\n",sizeof(char),1,pStream);
		return true;
	}

	bool TxtFileWriter::WriteLineShort(const short* asWrite,FILE* pStream,int nCount,char splice)
	{
		if(!asWrite || !pStream || nCount<=0)
			return false;
		for(int i=0;i<nCount;i++)
		{
			char szTmp[64] = {0};
			sprintf(szTmp,"%d",asWrite[i]);
			fwrite(szTmp,sizeof(char),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(char),1,pStream);
			}
		}
		fwrite("\n",sizeof(char),1,pStream);
		return true;
	}

	bool TxtFileWriter::WriteLineUInteger(const unsigned int* auWrite,FILE* pStream,int nCount,char splice)
	{
		if(!auWrite || !pStream || nCount<=0)
			return false;
		for(int i=0;i<nCount;i++)
		{
			char szTmp[64] = {0};
			sprintf(szTmp,"%u",auWrite[i]);
			fwrite(szTmp,sizeof(char),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(char),1,pStream);
			}
		}
		fwrite("\n",sizeof(char),1,pStream);
		return true;
	}

	bool TxtFileWriter::WriteLineULong(const unsigned long* aulWrite,FILE* pStream,int nCount,char splice)
	{
		if(!aulWrite || !pStream || nCount<=0)
			return false;
		for(int i=0;i<nCount;i++)
		{
			char szTmp[64] = {0};
			sprintf(szTmp,"%lu",aulWrite[i]);
			fwrite(szTmp,sizeof(char),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(char),1,pStream);
			}
		}
		fwrite("\n",sizeof(char),1,pStream);
		return true;
	}

	bool TxtFileWriter::WriteLineUShort(const unsigned short* ausWrite,FILE* pStream,int nCount,char splice)
	{
		if(!ausWrite || !pStream || nCount<=0)
			return false;
		for(int i=0;i<nCount;i++)
		{
			char szTmp[64] = {0};
			sprintf(szTmp,"%u",ausWrite[i]);
			fwrite(szTmp,sizeof(char),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(char),1,pStream);
			}
		}
		fwrite("\n",sizeof(char),1,pStream);
		return true;
	}

	bool TxtFileWriter::WriteLineFloat(const float* afWrite,FILE* pStream,int nCount,char splice)
	{
		if(!afWrite || !pStream || nCount<=0)
			return false;
		for(int i=0;i<nCount;i++)
		{
			char szTmp[64] = {0};
			sprintf(szTmp,"%.3f",afWrite[i]);
			fwrite(szTmp,sizeof(char),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(char),1,pStream);
			}
		}
		fwrite("\n",sizeof(char),1,pStream);
		return true;
	}

	bool TxtFileWriter::WriteLineDouble(const double* adWrite,FILE* pStream,int nCount,char splice)
	{
		if(!adWrite || !pStream || nCount<=0)
			return false;
		for(int i=0;i<nCount;i++)
		{
			char szTmp[64] = {0};
			sprintf(szTmp,"%.6lf",adWrite[i]);
			fwrite(szTmp,sizeof(char),strlen(szTmp),pStream);
			if(i<nCount-1)
			{
				sprintf(szTmp,"%c",splice);
				fwrite(szTmp,sizeof(char),1,pStream);
			}
		}
		fwrite("\n",sizeof(char),1,pStream);
		return true;
	}

	bool TxtFileWriter::WriteAllFile(const char* szFile,const char* mode,const void* buffer,size_t writeByte)
	{
		if(!szFile || !mode || !buffer || writeByte<=0)
			return false;
		FILE* file = 0;
		int re = fopen_s(&file,szFile,mode);
		if(re!=0)
		{
			return false;
		}
		if(fwrite(buffer,1,writeByte,file)!=writeByte)
		{
			fclose(file);
			return false;
		}
		fclose(file);
		return true;
	}
}