#include "TLTxtFileReader.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
namespace TLunaEngine{

	TxtFileReader::TxtFileReader(void)
	{
	}

	TxtFileReader::~TxtFileReader(void)
	{
	}

	bool TxtFileReader::OpenTxtFile(const char *filename, FILE **ppStream)
	{
		if(!filename || !ppStream)
		{
			assert(false);
			return false;
		}
		// 打开文件
		int re = fopen_s(ppStream,filename,"r+");
		if(*ppStream==0 || re!=0)
		{
			assert(false);
			return false;
		}
		return true;
	}

	void TxtFileReader::CloseTxtFile(FILE *pStream)
	{
		fclose(pStream);
	}

	bool TxtFileReader::ReadLineString(char *strResult, FILE *pStream, char *strCmp, bool *bEqual, int nCount, int *pReadCount)
	{
		if(!strResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}
		memset(strResult,0,nCount);

		// 读取一行
		int count = 0;
		char c;
		while(count<nCount)
		{
			int numRead = fread(&c,sizeof(char),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					break;
				}
				assert(false);
				return false;
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
				*bEqual=true;
			}
			else
			{
				*bEqual=false;
			}
		}
		return true;
	}

	bool TxtFileReader::ReadLineInteger(int *aiResult, FILE *pStream, int nCount, char splice)
	{
		if(!aiResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}

		int aCount = 0;
		int cCount = 0;
		char szTmp[32] = {0};
		char c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(char),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					aiResult[aCount++] = atoi(szTmp);
					break;
				}
				assert(false);
				return false;
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
		return true;
	}

	bool TxtFileReader::ReadLineLong(long* alResult,FILE* pStream,int nCount,char splice)
	{
		if(!alResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}

		int aCount = 0;
		int cCount = 0;
		char szTmp[32] = {0};
		char c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(char),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					alResult[aCount++] = atol(szTmp);
					break;
				}
				assert(false);
				return false;
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
		return true;
	}

	bool TxtFileReader::ReadLineShort(short* asResult,FILE* pStream,int nCount,char splice)
	{
		if(!asResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}

		int aCount = 0;
		int cCount = 0;
		char szTmp[32] = {0};
		char c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(char),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					asResult[aCount++] = (short)atoi(szTmp);
					break;
				}
				assert(false);
				return false;
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
		return true;
	}

	bool TxtFileReader::ReadLineUInteger(unsigned int* auResult,FILE* pStream,int nCount,char splice)
	{
		if(!auResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}

		int aCount = 0;
		int cCount = 0;
		char szTmp[32] = {0};
		char c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(char),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					auResult[aCount++] = (unsigned int)atoi(szTmp);
					break;
				}
				assert(false);
				return false;
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
		return true;
	}

	bool TxtFileReader::ReadLineULong(unsigned long* aulResult,FILE* pStream,int nCount,char splice)
	{
		if(!aulResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}

		int aCount = 0;
		int cCount = 0;
		char szTmp[32] = {0};
		char c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(char),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					aulResult[aCount++] = (unsigned long)atol(szTmp);
					break;
				}
				assert(false);
				return false;
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
		return true;
	}

	bool TxtFileReader::ReadLineUShort(unsigned short* ausResult,FILE* pStream,int nCount,char splice)
	{
		if(!ausResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}

		int aCount = 0;
		int cCount = 0;
		char szTmp[32] = {0};
		char c;

		// 循环读取一行的所有整数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(char),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					ausResult[aCount++] = (unsigned short)atoi(szTmp);
					break;
				}
				assert(false);
				return false;
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
		return true;
	}

	bool TxtFileReader::ReadLineFloat(float *afResult, FILE *pStream, int nCount, char splice)
	{
		if(!afResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}

		int aCount = 0;
		int cCount = 0;
		char szTmp[32] = {0};
		char c;
		float tmpRe;
		bool hasDot = false;
		int dotPos = -1;

		// 循环读取一行的所有浮点数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(char),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					if(hasDot)
					{
						szTmp[dotPos+4] = '\0';
						hasDot = false;
						dotPos = -1;
					}
					tmpRe = (float)atof(szTmp);
					afResult[aCount++] = tmpRe;
					break;
				}
				assert(false);
				return false;
			}
			if(c=='\n')
			{
				if(hasDot)
				{
					szTmp[dotPos+4] = '\0';
					hasDot = false;
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
					hasDot = false;
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
				hasDot = true;
				dotPos = cCount;
			}
			szTmp[cCount++] = c;
		}
		return true;
	}

	bool TxtFileReader::ReadLineDouble(double* adResult,FILE* pStream,int nCount,char splice)
	{
		if(!adResult || !pStream || nCount<=0)
		{
			assert(false);
			return false;
		}

		int aCount = 0;
		int cCount = 0;
		char szTmp[32] = {0};
		char c;
		double tmpRe;
		bool hasDot = false;
		int dotPos = -1;

		// 循环读取一行的所有浮点数数据
		while(aCount<nCount && cCount<32)
		{
			int numRead = fread(&c,sizeof(char),1,pStream);
			if(numRead!=1)
			{
				if(feof(pStream))
				{
					if(hasDot)
					{
						szTmp[dotPos+4] = '\0';
						hasDot = false;
						dotPos = -1;
					}
					tmpRe = atof(szTmp);
					adResult[aCount++] = tmpRe;
					break;
				}
				assert(false);
				return false;
			}
			if(c=='\n')
			{
				if(hasDot)
				{
					szTmp[dotPos+4] = '\0';
					hasDot = false;
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
					hasDot = false;
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
				hasDot = true;
				dotPos = cCount;
			}
			szTmp[cCount++] = c;
		}
		return true;
	}

	bool TxtFileReader::ReadAllFile(const char* szFile,const char* mode,void** buffer,size_t* readByte)
	{
		if(!szFile || !mode || !buffer || !readByte || (*buffer)!=0)
		{
			return false;
		}
		FILE* file = 0;
		int re = fopen_s(&file,szFile,mode);
		if(re!=0)
		{
			return false;
		}
		int readByteA = 0;
		unsigned char filebuf[1024] ={0};
		unsigned char* tmpbuf = 0;
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
					return false;
				}
				if(readByteA <= 0)
				{
					break;
				}
			}
			int lasttmpsize = tmpsize;
			if(lasttmpsize==0)
			{
				tmpbuf=(unsigned char*)malloc(readByteA);
				memset(tmpbuf,0,readByteA);
				tmpsize+=readByteA;
			}
			else
			{
				tmpbuf=(unsigned char*)realloc(tmpbuf,tmpsize+readByteA);
				memset(tmpbuf+lasttmpsize,0,readByteA);
				tmpsize+=readByteA;
			}
			memcpy(tmpbuf+lasttmpsize,filebuf,readByteA);
			memset(filebuf,0,1024);
			if(feof(file))
				break;
		}while(true);
		fclose(file);
		(*buffer) = (void*)tmpbuf;
		(*readByte) = tmpsize;
		return true;
	}
}