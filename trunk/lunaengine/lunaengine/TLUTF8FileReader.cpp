#include "TLUTF8FileReader.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
namespace TLunaEngine{

	UTF8FileReader::UTF8FileReader(TVOID)
	{
	}

	UTF8FileReader::~UTF8FileReader(TVOID)
	{
	}

	TBOOL UTF8FileReader::OpenTxtFile(const TCHAR* filename, FILE **ppStream)
	{
		if(!filename || !ppStream)
		{
			assert(TFALSE);
			return TFALSE;
		}
		// 打开文件
		TS32 re = fopen_s(ppStream,filename, "r+, ccs=UTF-8");
		if(*ppStream==0 || re!=0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		return TTRUE;
	}

	TVOID UTF8FileReader::CloseTxtFile(FILE *pStream)
	{
		fclose(pStream);
	}

	TBOOL UTF8FileReader::ReadLineWString(TWCHAR* strResult, FILE *pStream, TWCHAR* strCmp, TBOOL *bEqual, TU32 nCount, TU32 *pReadCount)
	{
		if (!strResult || !pStream || nCount <= 0)
		{
			assert(TFALSE);
			return TFALSE;
		}
		memset(strResult, 0, nCount * sizeof(TWCHAR));

		// 读取一行
		TU32 count = 0;
		TWCHAR c;
		while (count<nCount)
		{
			TU32 numRead = (TU32)fread(&c, sizeof(TWCHAR), 1, pStream);
			if (numRead != 1)
			{
				if (feof(pStream))
				{
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if (c == L'\n')
			{
				break;
			}
			strResult[count++] = c;
		}
		if (pReadCount)
			*pReadCount = count;

		// 比较
		if (count>0 && strCmp && bEqual)
		{
			if (wcscmp(strResult, strCmp) == 0)
			{
				*bEqual = TTRUE;
			}
			else
			{
				*bEqual = TFALSE;
			}
		}
		return TTRUE;
	}

	TBOOL UTF8FileReader::ReadLineInteger(TS32 *aiResult, FILE *pStream, TU32 nCount, TWCHAR splice)
	{
		if(!aiResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TWCHAR szTmp[32] = {0};
		TWCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TWCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					aiResult[aCount++] = _wtoi(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c==L'\n')
			{
				aiResult[aCount++] = _wtoi(szTmp);
				break;
			}
			if(c==splice)
			{
				aiResult[aCount++] = _wtoi(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL UTF8FileReader::ReadLineLongLong(TS64* allResult,FILE* pStream, TU32 nCount, TWCHAR splice)
	{
		if(!allResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TWCHAR szTmp[32] = {0};
		TWCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TWCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					allResult[aCount++] = _wtoll(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c==L'\n')
			{
				allResult[aCount++] = _wtoll(szTmp);
				break;
			}
			if(c==splice)
			{
				allResult[aCount++] = _wtoll(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL UTF8FileReader::ReadLineShort(TS16* asResult,FILE* pStream, TU32 nCount, TWCHAR splice)
	{
		if(!asResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TWCHAR szTmp[32] = {0};
		TWCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TWCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					asResult[aCount++] = (TS16)_wtoi(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c==L'\n')
			{
				asResult[aCount++] = (TS16)_wtoi(szTmp);
				break;
			}
			if(c==splice)
			{
				asResult[aCount++] = (TS16)_wtoi(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL UTF8FileReader::ReadLineUInteger(TU32* auResult,FILE* pStream, TU32 nCount, TWCHAR splice)
	{
		if(!auResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TWCHAR szTmp[32] = {0};
		TWCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TWCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					auResult[aCount++] = (TU32)_wtoi(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c==L'\n')
			{
				auResult[aCount++] = (TU32)_wtoi(szTmp);
				break;
			}
			if(c==splice)
			{
				auResult[aCount++] = (TU32)_wtoi(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL UTF8FileReader::ReadLineULongLong(TU64* aullResult,FILE* pStream,TU32 nCount, TWCHAR splice)
	{
		if(!aullResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TWCHAR szTmp[32] = {0};
		TWCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TWCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					aullResult[aCount++] = (TU64)_wtoll(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c==L'\n')
			{
				aullResult[aCount++] = (TU64)_wtoll(szTmp);
				break;
			}
			if(c==splice)
			{
				aullResult[aCount++] = (TU64)_wtoll(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL UTF8FileReader::ReadLineUShort(TU16* ausResult,FILE* pStream,TU32 nCount, TWCHAR splice)
	{
		if(!ausResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TWCHAR szTmp[32] = {0};
		TWCHAR c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TWCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					ausResult[aCount++] = (TU16)_wtoi(szTmp);
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c==L'\n')
			{
				ausResult[aCount++] = (TU16)_wtoi(szTmp);
				break;
			}
			if(c==splice)
			{
				ausResult[aCount++] = (TU16)_wtoi(szTmp);
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL UTF8FileReader::ReadLineFloat(TF32 *afResult, FILE *pStream, TU32 nCount, TWCHAR splice)
	{
		if(!afResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TWCHAR szTmp[32] = {0};
		TWCHAR c;
		TF32 tmpRe;
		TBOOL hasDot = TFALSE;
		TS32 dotPos = -1;

		// 循环读取一行的所有浮点数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TWCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					if(hasDot)
					{
						szTmp[dotPos+4] = L'\0';
						hasDot = TFALSE;
						dotPos = -1;
					}
					tmpRe = (TF32)_wtof(szTmp);
					afResult[aCount++] = tmpRe;
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c==L'\n')
			{
				if(hasDot)
				{
					szTmp[dotPos+4] = L'\0';
					hasDot = TFALSE;
					dotPos = -1;
				}
				tmpRe = (TF32)_wtof(szTmp);
				afResult[aCount++] = tmpRe;
				break;
			}
			if(c==splice)
			{
				if(hasDot)
				{
					szTmp[dotPos+4] = L'\0';
					hasDot = TFALSE;
					dotPos = -1;
				}
				tmpRe = (TF32)_wtof(szTmp);
				afResult[aCount++] = tmpRe;
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			if(c==L'.')
			{
				hasDot = TTRUE;
				dotPos = cCount;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}

	TBOOL UTF8FileReader::ReadLineDouble(TF64* adResult,FILE* pStream, TU32 nCount, TWCHAR splice)
	{
		if(!adResult || !pStream || nCount<=0)
		{
			assert(TFALSE);
			return TFALSE;
		}

		TU32 aCount = 0;
		TU32 cCount = 0;
		TWCHAR szTmp[32] = {0};
		TWCHAR c;
		TF64 tmpRe;
		TBOOL hasDot = TFALSE;
		TS32 dotPos = -1;

		// 循环读取一行的所有浮点数数据
		while(aCount<nCount && cCount<32)
		{
			TU32 numRead = (TU32)fread(&c,sizeof(TWCHAR),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					if(hasDot)
					{
						szTmp[dotPos+4] = L'\0';
						hasDot = TFALSE;
						dotPos = -1;
					}
					tmpRe = _wtof(szTmp);
					adResult[aCount++] = tmpRe;
					break;
				}
				assert(TFALSE);
				return TFALSE;
			}
			if(c==L'\n')
			{
				if(hasDot)
				{
					szTmp[dotPos+4] = L'\0';
					hasDot = TFALSE;
					dotPos = -1;
				}
				tmpRe = _wtof(szTmp);
				adResult[aCount++] = tmpRe;
				break;
			}
			if(c==splice)
			{
				if(hasDot)
				{
					szTmp[dotPos+4] = L'\0';
					hasDot = TFALSE;
					dotPos = -1;
				}
				tmpRe = _wtof(szTmp);
				adResult[aCount++] = tmpRe;
				memset(szTmp,0,32);
				cCount = 0;
				continue;
			}
			if(c==L'.')
			{
				hasDot = TTRUE;
				dotPos = cCount;
			}
			szTmp[cCount++] = c;
		}
		return TTRUE;
	}
}