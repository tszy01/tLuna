#include "TLTxtFileReader.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
namespace TLunaEngine{

	TxtFileReader::TxtFileReader(TVOID)
	{
	}

	TxtFileReader::~TxtFileReader(TVOID)
	{
	}

	TBOOL TxtFileReader::OpenTxtFile(const TCHAR* filename, FILE **ppStream)
	{
		if(!filename || !ppStream)
		{
			assert(TFALSE);
			return TFALSE;
		}
		// 打开文件
		int re = fopen_s(ppStream,filename,"r+");
		if(*ppStream==0 || re!=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		return TTRUE;
	}

	TVOID TxtFileReader::CloseTxtFile(FILE *pStream)
	{
		fclose(pStream);
	}

	TBOOL TxtFileReader::ReadLineString(TCHAR* strResult, FILE *pStream, TCHAR* strCmp, TBOOL *bEqual, int nCount, int *pReadCount)
	{
		if(!strResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		memset(strResult,0,nCount);

		// 读取一行
		int count = 0;
		TCHAR c;
		while(count<nCount)
		{
			int numRead = fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				break;
			}
			strResult[count++] = c;
		}
		if(pReadCount)
			*pReadCount = count;

		// 比较
		if(count>0 && strCmp && bEqual)
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
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineInteger(int *aiResult, FILE *pStream, int nCount, TCHAR splice)
	{
		if(!aiResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		int aCount = 0;
		int cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					aiResult[aCount++] = atoi(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				aiResult[aCount++] = atoi(szTmp);
				break;
			}
			if(c==splice)
			{
				aiResult[aCount++] = atoi(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineLong(long* alResult,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!alResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		int aCount = 0;
		int cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					alResult[aCount++] = atol(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				alResult[aCount++] = atol(szTmp);
				break;
			}
			if(c==splice)
			{
				alResult[aCount++] = atol(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineShort(short* asResult,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!asResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		int aCount = 0;
		int cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					asResult[aCount++] = (short)atoi(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				asResult[aCount++] = (short)atoi(szTmp);
				break;
			}
			if(c==splice)
			{
				asResult[aCount++] = (short)atoi(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineUInteger(unsigned int* auResult,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!auResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		int aCount = 0;
		int cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					auResult[aCount++] = (unsigned int)atoi(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				auResult[aCount++] = (unsigned int)atoi(szTmp);
				break;
			}
			if(c==splice)
			{
				auResult[aCount++] = (unsigned int)atoi(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineULong(unsigned long* aulResult,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!aulResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		int aCount = 0;
		int cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					aulResult[aCount++] = (unsigned long)atol(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				aulResult[aCount++] = (unsigned long)atol(szTmp);
				break;
			}
			if(c==splice)
			{
				aulResult[aCount++] = (unsigned long)atol(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineUShort(unsigned short* ausResult,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!ausResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		int aCount = 0;
		int cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					ausResult[aCount++] = (unsigned short)atoi(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				ausResult[aCount++] = (unsigned short)atoi(szTmp);
				break;
			}
			if(c==splice)
			{
				ausResult[aCount++] = (unsigned short)atoi(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineFloat(float *afResult, FILE *pStream, int nCount, TCHAR splice)
	{
		if(!afResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		int aCount = 0;
		int cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;
		float tmpRe;
		TBOOL hasDot = TFALSE;
		int dotPos = -1;

		// 循环读取一行的所有浮点数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					if(hasDot)
					{
						szTmp[dotPos+4] = '\0';
						hasDot = TFALSE;
						dotPos = -1;
					}
					tmpRe = (float)atof(szTmp);
					afResult[aCount++] = tmpRe;
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				if(hasDot)
				{
					szTmp[dotPos+4] = '\0';
					hasDot = TFALSE;
					dotPos = -1;
				}
				tmpRe = (float)atof(szTmp);
				afResult[aCount++] = tmpRe;
				break;
			}
			if(c==splice)
			{
				if(hasDot)
				{
					szTmp[dotPos+4] = '\0';
					hasDot = TFALSE;
					dotPos = -1;
				}
				tmpRe = (float)atof(szTmp);
				afResult[aCount++] = tmpRe;
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			if(c=='.')
			{
				hasDot = TTRUE;
				dotPos = cCount;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineDouble(double* adResult,FILE* pStream,int nCount,TCHAR splice)
	{
		if(!adResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		int aCount = 0;
		int cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;
		double tmpRe;
		TBOOL hasDot = TFALSE;
		int dotPos = -1;

		// 循环读取一行的所有浮点数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					if(hasDot)
					{
						szTmp[dotPos+4] = '\0';
						hasDot = TFALSE;
						dotPos = -1;
					}
					tmpRe = atof(szTmp);
					adResult[aCount++] = tmpRe;
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				if(hasDot)
				{
					szTmp[dotPos+4] = '\0';
					hasDot = TFALSE;
					dotPos = -1;
				}
				tmpRe = atof(szTmp);
				adResult[aCount++] = tmpRe;
				break;
			}
			if(c==splice)
			{
				if(hasDot)
				{
					szTmp[dotPos+4] = '\0';
					hasDot = TFALSE;
					dotPos = -1;
				}
				tmpRe = atof(szTmp);
				adResult[aCount++] = tmpRe;
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			if(c=='.')
			{
				hasDot = TTRUE;
				dotPos = cCount;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadAllFile(const TCHAR* szFile,const TCHAR* mode,TVOID** buffer,size_t* readByte)
	{
		if(!szFile || !mode || !buffer || !readByte || (*buffer)!=0)
		{
			return TFALSE;
		}
		FILE* file = 0;
		int re = fopen_s(&file,szFile,mode);
		if(re!=0)
		{
			return TFALSE;
		}
		int readByteA = 0;
		TUByte filebuf[1024] ={0};
		TUByte* tmpbuf = 0;
		int tmpsize = 0;
		do
		{
			readByteA=(int)fread(filebuf,1,1024,file);
			if(readByteA!=1024)
			{
				if(ferror(file))
				{
					fclose(file);
					free(tmpbuf);
					return TFALSE;
				}
				if(readByteA <= 0)
				{
					break;
				}
			}
			int lasttmpsize = tmpsize;
			if(lasttmpsize==0)
			{
				tmpbuf=(TUByte*)malloc(readByteA);
				memset(tmpbuf,0,readByteA);
				tmpsize+=readByteA;
			}
			else
			{
				tmpbuf=(TUByte*)realloc(tmpbuf,tmpsize+readByteA);
				memset(tmpbuf+lasttmpsize,0,readByteA);
				tmpsize+=readByteA;
			}
			memcpy(tmpbuf+lasttmpsize,filebuf,readByteA);
			memset(filebuf,0,1024);
			if(feof(file))
				break;
		}while(TTRUE);
		fclose(file);
		(*buffer) = (TVOID*)tmpbuf;
		(*readByte) = tmpsize;
		return TTRUE;
	}
}