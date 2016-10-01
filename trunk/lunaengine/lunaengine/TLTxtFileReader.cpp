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
		TS32 re = fopen_s(ppStream,filename,"r+");
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

	TBOOL TxtFileReader::ReadLineString(TCHAR* strResult, FILE *pStream, TCHAR* strCmp, TBOOL *bEqual, TU32 nCount, TU32 *pReadCount)
	{
		if(!strResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		memset(strResult,0,nCount);

		// 读取一行
		TU32 count = 0;
		TCHAR c;
		while(count<nCount)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TCHAR),1,pStream);
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

	TBOOL TxtFileReader::ReadLineInteger(TS32 *aiResult, FILE *pStream, TU32 nCount, TCHAR splice)
	{
		if(!aiResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TCHAR),1,pStream);
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

	TBOOL TxtFileReader::ReadLineLongLong(TS64* allResult,FILE* pStream, TU32 nCount,TCHAR splice)
	{
		if(!allResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					allResult[aCount++] = atoll(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				allResult[aCount++] = atoll(szTmp);
				break;
			}
			if(c==splice)
			{
				allResult[aCount++] = atoll(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineShort(TS16* asResult,FILE* pStream, TU32 nCount,TCHAR splice)
	{
		if(!asResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					asResult[aCount++] = (TS16)atoi(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				asResult[aCount++] = (TS16)atoi(szTmp);
				break;
			}
			if(c==splice)
			{
				asResult[aCount++] = (TS16)atoi(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineUInteger(TU32* auResult,FILE* pStream, TU32 nCount,TCHAR splice)
	{
		if(!auResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					auResult[aCount++] = (TU32)atoi(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				auResult[aCount++] = (TU32)atoi(szTmp);
				break;
			}
			if(c==splice)
			{
				auResult[aCount++] = (TU32)atoi(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineULongLong(TU64* aullResult,FILE* pStream,TU32 nCount,TCHAR splice)
	{
		if(!aullResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					aullResult[aCount++] = (TU64)atoll(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				aullResult[aCount++] = (TU64)atoll(szTmp);
				break;
			}
			if(c==splice)
			{
				aullResult[aCount++] = (TU64)atoll(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineUShort(TU16* ausResult,FILE* pStream,TU32 nCount,TCHAR splice)
	{
		if(!ausResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					ausResult[aCount++] = (TU16)atoi(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c=='\n')
			{
				ausResult[aCount++] = (TU16)atoi(szTmp);
				break;
			}
			if(c==splice)
			{
				ausResult[aCount++] = (TU16)atoi(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL TxtFileReader::ReadLineFloat(TF32 *afResult, FILE *pStream, TU32 nCount, TCHAR splice)
	{
		if(!afResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;
		TF32 tmpRe;
		TBOOL hasDot = TFALSE;
		TS32 dotPos = -1;

		// 循环读取一行的所有浮点数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TCHAR),1,pStream);
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
					tmpRe = (TF32)atof(szTmp);
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
				tmpRe = (TF32)atof(szTmp);
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
				tmpRe = (TF32)atof(szTmp);
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

	TBOOL TxtFileReader::ReadLineDouble(TF64* adResult,FILE* pStream, TU32 nCount,TCHAR splice)
	{
		if(!adResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TCHAR szTmp[32] = {0};
		TCHAR c;
		TF64 tmpRe;
		TBOOL hasDot = TFALSE;
		TS32 dotPos = -1;

		// 循环读取一行的所有浮点数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TCHAR),1,pStream);
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

	TBOOL TxtFileReader::ReadAllFile(const TCHAR* szFile,const TCHAR* mode,TVOID** buffer, TU64* pCount)
	{
		if(!szFile || !mode || !buffer || !pCount || (*buffer)!=0)
		{
			return TFALSE;
		}
		FILE* file = 0;
		TS32 re = fopen_s(&file,szFile,mode);
		if(re!=0)
		{
			return TFALSE;
		}
		TU64 readByteA = 0;
		TUByte filebuf[1024] ={0};
		TUByte* tmpbuf = 0;
		TU64 tmpsize = 0;
		do
		{
			readByteA=(TU64)fread(filebuf,1,1024,file);
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
			TU64 lasttmpsize = tmpsize;
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
		(*pCount) = tmpsize;
		return TTRUE;
	}
}