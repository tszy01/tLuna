#include "StdAfx.h"
#include ".\tstring.h"

#include <windows.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <string>
#include <ctype.h>
#include <string>
#include <limits>
#include <vector>
#include <stdio.h>        //-- c 语言的头文件
#include <stdlib.h>
#include <time.h>
#include <tchar.h>
#include <strsafe.h>

namespace TLunaEngine{

	TString::TString(void) : m_nLength(0)
	{
	}

	TString::~TString(void)
	{
		m_szPtr.setNull();
		m_nLength=0;
	}

	TString::TString(const char *szContent)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		if(szContent!=0)
		{
			int n=0;
			const char *szTmp = szContent;
			while(*szTmp!='\0')
			{
				n++;
				szTmp++;
			}
			szTmp=0;
			m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
			char* szMyContent = m_szPtr.getPointer();
			memcpy(szMyContent,szContent,n);
			szMyContent[n]='\0';
			m_nLength = n;
			//printf(m_szContent);
		}
	}

	TString::TString(const wchar_t *szContent)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		if(szContent!=0)
		{
			int n=0;
			const wchar_t *szTmp = szContent;
			while(*szTmp!=L'\0')
			{
				n++;
				szTmp++;
			}
			szTmp=0;
			m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
			char* szMyContent = m_szPtr.getPointer();
			wcstombs(szMyContent,szContent,n);
			szMyContent[n]='\0';
			m_nLength = n;
			//printf(m_szContent);
		}
	}

	TString::TString(const char cContent)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		int n = 1;
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		char szTmp[2] = {0};
		szTmp[0] = cContent;
		memcpy(szMyContent,szTmp,n);
		szMyContent[n]='\0';
		m_nLength = n;
	}

	TString::TString(const wchar_t cContent)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		int n = 1;
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		wchar_t szTmp[2] = {0};
		szTmp[0] = cContent;
		wcstombs(szMyContent,szTmp,n);
		szMyContent[n]='\0';
		m_nLength = n;
	}

	TString::TString(const TString &strContent)
	{
		if(strContent.m_szPtr.isNull()) return;
		m_szPtr.setNull();
		m_nLength = 0;
		m_szPtr = strContent.m_szPtr;
		int len = strContent.m_nLength;
		m_nLength=len;
		//printf(m_szContent);
	}

	TString::TString(const int iRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",iRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}
	TString::TString(const unsigned int uiRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",uiRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}
	TString::TString(const float fRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%f",fRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}
	TString::TString(const double dRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%f",dRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}
	TString::TString(const bool bRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[1] = {0};
		if(bRight)
			szTmp[0] = '1';
		else
			szTmp[0] = '0';
		int n = 1;
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	TString::TString(const short sRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",sRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	TString::TString(const unsigned short usRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",usRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	TString::TString(const long lRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",lRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	TString::TString(const unsigned long ulRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",ulRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	TString& TString::operator =(const TString &strRight)
	{
		if(strRight.m_szPtr.isNull()) 
			return *this;

		m_szPtr.setNull();
		m_nLength = 0;
		m_szPtr = strRight.m_szPtr;
		int len = strRight.m_nLength;
		m_nLength=len;
		//printf(m_szContent);
		return *this;
	}

	TString& TString::operator =(const char *szRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;

		if(szRight!=0)
		{
			int n=0;
			const char *szTmp = szRight;
			while(*szTmp!='\0')
			{
				n++;
				szTmp++;
			}
			szTmp=0;
			m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
			char* szMyContent = m_szPtr.getPointer();
			memcpy(szMyContent,szRight,n);
			szMyContent[n]='\0';
			m_nLength = n;
			//printf(m_szContent);
		}

		return *this;
	}

	TString& TString::operator =(const wchar_t *szRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;

		if(szRight!=0)
		{
			int n=0;
			const wchar_t *szTmp = szRight;
			while(*szTmp!=L'\0')
			{
				n++;
				szTmp++;
			}
			szTmp=0;
			m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
			char* szMyContent = m_szPtr.getPointer();
			wcstombs(szMyContent,szRight,n);
			szMyContent[n]='\0';
			m_nLength = n;
			//printf(m_szContent);
		}

		return *this;
	}

	TString& TString::operator =(const char cRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;

		int n = 1;
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		char szTmp[2] = {0};
		szTmp[0] = cRight;
		memcpy(szMyContent,szTmp,n);
		szMyContent[n]='\0';
		m_nLength = n;

		return *this;
	}

	TString& TString::operator =(const int iRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",iRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	TString& TString::operator =(const unsigned int uiRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",uiRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	TString& TString::operator =(const float fRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%f",fRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	TString& TString::operator =(const double dRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%f",dRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	TString& TString::operator =(const bool bRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[1] = {0};
		if(bRight)
			szTmp[0] = '1';
		else
			szTmp[0] = '0';
		int n = 1;
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}

	TString& TString::operator =(const short sRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",sRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	TString& TString::operator =(const unsigned short usRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",usRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}

	TString& TString::operator =(const long lRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",lRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	TString& TString::operator =(const unsigned long ulRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		char szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",ulRight);
		int n = strlen(szTmp);
		m_szPtr = TSharedPtr<char>(new char[n+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}

	TString TString::operator +(const char *szRight)
	{
		TString result("");

		if(szRight!=0 && m_szPtr.isNull()==false)
		{
			int n=0;
			const char *szTmp = szRight;
			while(*szTmp!='\0')
			{
				n++;
				szTmp++;
			}

			char *sz = new char[n+m_nLength+1];
			szTmp=m_szPtr.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp=szRight;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]='\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}

	TString TString::operator +(const wchar_t *szRight)
	{
		TString result("");

		if(szRight!=0 && m_szPtr.isNull()==false)
		{
			int n=0;
			const wchar_t *szTmp = szRight;
			while(*szTmp!=L'\0')
			{
				n++;
				szTmp++;
			}

			wchar_t *sz = new wchar_t[n+m_nLength+1];
			TSharedPtr<wchar_t> pThis=GetWString();
			szTmp = pThis.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp=szRight;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]=L'\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}

	TString TString::operator +(const char cRight)
	{
		TString result("");

		if(m_szPtr.isNull()==false)
		{
			int n=1;
			char szTmp2[2] = {0};
			szTmp2[0] = cRight;

			char *sz = new char[n+m_nLength+1];
			char *szTmp=m_szPtr.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp=szTmp2;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]='\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}

	TString TString::operator +(const wchar_t cRight)
	{
		TString result("");

		if(m_szPtr.isNull()==false)
		{
			int n=1;
			wchar_t szTmp2[2] = {0};
			szTmp2[0] = cRight;

			wchar_t *sz = new wchar_t[n+m_nLength+1];
			TSharedPtr<wchar_t> pThis=GetWString();
			const wchar_t *szTmp = pThis.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp=szTmp2;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]=L'\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}

	TString TString::operator +(const TString &strRight)
	{
		TString result("");

		if(m_szPtr.isNull()==false && strRight.m_szPtr.isNull()==false)
		{
			int n = strRight.m_nLength;

			char *sz = new char[n+m_nLength+1];
			const char *szTmp=m_szPtr.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp=strRight.m_szPtr.getPointer();
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]='\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}

	TString TString::operator +(const int iRight)
	{
		TString result("");

		if(m_szPtr.isNull()==false)
		{
			//int n = strRight.m_nLength;
			char szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%d",iRight);
			int n = strlen(szTmp2);

			char *sz = new char[n+m_nLength+1];
			const char *szTmp=m_szPtr.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]='\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}
	TString TString::operator +(const unsigned int uiRight)
	{
		TString result("");

		if(m_szPtr.isNull()==false)
		{
			//int n = strRight.m_nLength;
			char szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%u",uiRight);
			int n = strlen(szTmp2);

			char *sz = new char[n+m_nLength+1];
			const char *szTmp=m_szPtr.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]='\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}
	TString TString::operator +(const float fRight)
	{
		TString result("");

		if(m_szPtr.isNull()==false)
		{
			//int n = strRight.m_nLength;
			char szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%f",fRight);
			int n = strlen(szTmp2);

			char *sz = new char[n+m_nLength+1];
			const char *szTmp=m_szPtr.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]='\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}
	TString TString::operator +(const double dRight)
	{
		TString result("");

		if(m_szPtr.isNull()==false)
		{
			//int n = strRight.m_nLength;
			char szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%f",dRight);
			int n = strlen(szTmp2);

			char *sz = new char[n+m_nLength+1];
			const char *szTmp=m_szPtr.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]='\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}
	TString TString::operator +(const bool bRight)
	{
		TString result("");

		if(m_szPtr.isNull()==false)
		{
			//int n = strRight.m_nLength;
			char szTmp2[1] = {0};
			if(bRight)
				szTmp2[0] = '1';
			else
				szTmp2[0] = '0';
			int n = 1;

			char *sz = new char[n+m_nLength+1];
			const char *szTmp=m_szPtr.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]='\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}

	TString TString::operator +(const short sRight)
	{
		TString result("");

		if(m_szPtr.isNull()==false)
		{
			//int n = strRight.m_nLength;
			char szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%d",sRight);
			int n = strlen(szTmp2);

			char *sz = new char[n+m_nLength+1];
			const char *szTmp=m_szPtr.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]='\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}
	TString TString::operator +(const unsigned short usRight)
	{
		TString result("");

		if(m_szPtr.isNull()==false)
		{
			//int n = strRight.m_nLength;
			char szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%u",usRight);
			int n = strlen(szTmp2);

			char *sz = new char[n+m_nLength+1];
			const char *szTmp=m_szPtr.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]='\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}

	TString TString::operator +(const long lRight)
	{
		TString result("");

		if(m_szPtr.isNull()==false)
		{
			//int n = strRight.m_nLength;
			char szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%d",lRight);
			int n = strlen(szTmp2);

			char *sz = new char[n+m_nLength+1];
			const char *szTmp=m_szPtr.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]='\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}
	TString TString::operator +(const unsigned long ulRight)
	{
		TString result("");

		if(m_szPtr.isNull()==false)
		{
			//int n = strRight.m_nLength;
			char szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%u",ulRight);
			int n = strlen(szTmp2);

			char *sz = new char[n+m_nLength+1];
			const char *szTmp=m_szPtr.getPointer();
			for(int i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(int i=m_nLength;i<n+m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}

			szTmp=0;
			sz[n+m_nLength]='\0';
			result = sz;
			delete [] sz;
		}

		return result;
	}

	TString& TString::operator +=(const char *szRight)
	{
		if(m_szPtr.isNull() || !szRight) 
			return *this;

		*this = *this + szRight;
		return *this;
	}

	TString& TString::operator +=(const wchar_t *szRight)
	{
		if(m_szPtr.isNull() || !szRight) 
			return *this;

		*this = *this + szRight;
		return *this;
	}

	TString& TString::operator +=(const char cRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + cRight;
		return *this;
	}

	TString& TString::operator +=(const wchar_t cRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + cRight;
		return *this;
	}

	TString& TString::operator +=(const TString &strRight)
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) 
			return *this;

		*this = *this + strRight;
		return *this;
	}

	TString& TString::operator +=(const int iRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + iRight;
		return *this;
	}
	TString& TString::operator +=(const unsigned int uiRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + uiRight;
		return *this;
	}
	TString& TString::operator +=(const float fRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + fRight;
		return *this;
	}
	TString& TString::operator +=(const double dRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + dRight;
		return *this;
	}
	TString& TString::operator +=(const bool bRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + bRight;
		return *this;
	}

	TString& TString::operator +=(const short sRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + sRight;
		return *this;
	}
	TString& TString::operator +=(const unsigned short usRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + usRight;
		return *this;
	}

	TString& TString::operator +=(const long lRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + lRight;
		return *this;
	}
	TString& TString::operator +=(const unsigned long ulRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + ulRight;
		return *this;
	}

	bool TString::operator ==(const TString &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return false;
		if(m_nLength != strRight.m_nLength) return false;

		const char *szTmp = strRight.m_szPtr.getPointer();
		const char *szThis = m_szPtr.getPointer();
		for(int i=0;i<m_nLength;i++)
		{
			if(*szTmp!=szThis[i])
				return false;
			szTmp++;
		}

		return true;
	}

	bool TString::operator ==(const char *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return false;

		const char *szTmp = szRight;
		const char *szThis = m_szPtr.getPointer();
		int n = 0;
		while(*szTmp!='\0')
		{
			n++;
			szTmp++;
		}
		if(m_nLength != n) return false;

		szTmp = szRight;
		for(int i=0;i<m_nLength;i++)
		{
			if(*szTmp!=szThis[i])
				return false;
			szTmp++;
		}

		return true;
	}

	bool TString::operator ==(const wchar_t *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return false;

		const wchar_t *szTmp = szRight;
		TSharedPtr<wchar_t> pThis=GetWString();
		const wchar_t *szThis = pThis.getPointer();
		int n = 0;
		while(*szTmp!=L'\0')
		{
			n++;
			szTmp++;
		}
		if(m_nLength != n) return false;

		szTmp = szRight;
		for(int i=0;i<m_nLength;i++)
		{
			if(*szTmp!=szThis[i])
				return false;
			szTmp++;
		}

		return true;
	}

	bool TString::operator !=(const char *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return false;
		return !(*this==szRight);
	}

	bool TString::operator !=(const wchar_t *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return false;
		return !(*this==szRight);
	}

	bool TString::operator !=(const TString &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return false;
		return !(*this==strRight);
	}

	bool TString::operator >=(const char *szRight) const 
	{
		if(m_szPtr.isNull() || !szRight) return false;

		const char *szTmp = szRight;
		const char *szThis = m_szPtr.getPointer();
		int n = 0;
		while(*szTmp!='\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength < n)
		{
			szTmp = szRight;
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]>=*szTmp)
					return true;
				szTmp++;
			}
			return false;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]<*szTmp)
					return false;
				szTmp++;
			}
			return true;
		}
	}

	bool TString::operator >=(const wchar_t *szRight) const 
	{
		if(m_szPtr.isNull() || !szRight) return false;

		const wchar_t *szTmp = szRight;
		TSharedPtr<wchar_t> pThis=GetWString();
		const wchar_t *szThis = pThis.getPointer();
		int n = 0;
		while(*szTmp!=L'\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength < n)
		{
			szTmp = szRight;
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]>=*szTmp)
					return true;
				szTmp++;
			}
			return false;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]<*szTmp)
					return false;
				szTmp++;
			}
			return true;
		}
	}

	bool TString::operator >(const char *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return false;

		const char *szTmp = szRight;
		const char *szThis = m_szPtr.getPointer();
		int n = 0;
		while(*szTmp!='\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength <= n)
		{
			szTmp = szRight;
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]>*szTmp)
					return true;
				szTmp++;
			}
			return false;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]<=*szTmp)
					return false;
				szTmp++;
			}
			return true;
		}
	}

	bool TString::operator >(const wchar_t *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return false;

		const wchar_t *szTmp = szRight;
		TSharedPtr<wchar_t> pThis=GetWString();
		const wchar_t *szThis = pThis.getPointer();
		int n = 0;
		while(*szTmp!=L'\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength <= n)
		{
			szTmp = szRight;
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]>*szTmp)
					return true;
				szTmp++;
			}
			return false;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]<=*szTmp)
					return false;
				szTmp++;
			}
			return true;
		}
	}

	bool TString::operator <=(const char *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return false;

		const char *szTmp = szRight;
		const char *szThis = m_szPtr.getPointer();
		int n = 0;
		while(*szTmp!='\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength > n)
		{
			szTmp = szRight;
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]<=*szTmp)
					return true;
				szTmp++;
			}
			return false;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]>*szTmp)
					return false;
				szTmp++;
			}
			return true;
		}
	}

	bool TString::operator <=(const wchar_t *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return false;

		const wchar_t *szTmp = szRight;
		TSharedPtr<wchar_t> pThis=GetWString();
		const wchar_t *szThis = pThis.getPointer();
		int n = 0;
		while(*szTmp!=L'\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength > n)
		{
			szTmp = szRight;
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]<=*szTmp)
					return true;
				szTmp++;
			}
			return false;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]>*szTmp)
					return false;
				szTmp++;
			}
			return true;
		}
	}

	bool TString::operator <(const char *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return false;

		const char *szTmp = szRight;
		const char *szThis = m_szPtr.getPointer();
		int n = 0;
		while(*szTmp!='\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength >= n)
		{
			szTmp = szRight;
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]<*szTmp)
					return true;
				szTmp++;
			}
			return false;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]>=*szTmp)
					return false;
				szTmp++;
			}
			return true;
		}
	}

	bool TString::operator <(const wchar_t *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return false;

		const wchar_t *szTmp = szRight;
		TSharedPtr<wchar_t> pThis=GetWString();
		const wchar_t *szThis = pThis.getPointer();
		int n = 0;
		while(*szTmp!=L'\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength >= n)
		{
			szTmp = szRight;
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]<*szTmp)
					return true;
				szTmp++;
			}
			return false;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]>=*szTmp)
					return false;
				szTmp++;
			}
			return true;
		}
	}

	bool TString::operator >=(const TString &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return false;

		const char *szTmp = strRight.m_szPtr.getPointer();
		const char *szThis = m_szPtr.getPointer();
		int n = strRight.m_nLength;

		if(m_nLength < n)
		{
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]>=*szTmp)
					return true;
				szTmp++;
			}
			return false;
		}
		else
		{
			for(int i=0;i<n;i++)
			{
				if(szThis[i]<*szTmp)
					return false;
				szTmp++;
			}
			return true;
		}
	}

	bool TString::operator >(const TString &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return false;

		const char *szTmp = strRight.m_szPtr.getPointer();
		const char *szThis = m_szPtr.getPointer();
		int n = strRight.m_nLength;

		if(m_nLength <= n)
		{
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]>*szTmp)
					return true;
				szTmp++;
			}
			return false;
		}
		else
		{
			for(int i=0;i<n;i++)
			{
				if(szThis[i]<=*szTmp)
					return false;
				szTmp++;
			}
			return true;
		}
	}

	bool TString::operator <=(const TString &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return false;

		const char *szTmp = strRight.m_szPtr.getPointer();
		const char *szThis = m_szPtr.getPointer();
		int n = strRight.m_nLength;

		if(m_nLength > n)
		{
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]<=*szTmp)
					return true;
				szTmp++;
			}
			return false;
		}
		else
		{
			for(int i=0;i<n;i++)
			{
				if(szThis[i]>*szTmp)
					return false;
				szTmp++;
			}
			return true;
		}
	}

	bool TString::operator <(const TString &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return false;

		const char *szTmp = strRight.m_szPtr.getPointer();
		const char *szThis = m_szPtr.getPointer();
		int n = strRight.m_nLength;

		if(m_nLength >= n)
		{
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]<*szTmp)
					return true;
				szTmp++;
			}
			return false;
		}
		else
		{
			for(int i=0;i<n;i++)
			{
				if(szThis[i]>=*szTmp)
					return false;
				szTmp++;
			}
			return true;
		}
	}

	char TString::operator [](int pos) const
	{
		if(pos<m_nLength && m_szPtr.isNull()==false && pos>=0)
		{
			const char *szThis = m_szPtr.getPointer();
			return szThis[pos];
		}

		return 0;
	}

	TString TString::Mid(int startPos,int count) const
	{
		TString result("");

		if(startPos<m_nLength && m_szPtr.isNull()==false && count>0 && startPos>=0)
		{
			const char *szThis = m_szPtr.getPointer();
			char *szTmp = new char[count+1];
			int i = 0;
			for(;i<count;i++)
			{
				if(startPos+i>=m_nLength) break;
				szTmp[i] = szThis[startPos + i];
			}
			szTmp[i] = '\0';
			result = szTmp;
			delete [] szTmp;
		}

		return result;
	}

	TString TString::Mid(int startPos) const
	{
		TString result("");

		if(startPos<m_nLength && m_szPtr.isNull()==false && startPos>=0)
		{
			const char *szThis = m_szPtr.getPointer();
			int count = m_nLength - startPos;
			char *szTmp = new char[count+1];
			int i = 0;
			for(;i<count;i++)
			{
				if(startPos+i>=m_nLength) break;
				szTmp[i] = szThis[startPos + i];
			}
			szTmp[i] = '\0';
			result = szTmp;
			delete [] szTmp;
		}

		return result;
	}

	void TString::Format(const char *format, ...)
	{
		if(!format) return;

		m_szPtr.setNull();
		m_nLength = 0;

		va_list args;
		int len;

		va_start( args, format );
		len = _vscprintf( format, args ); 
		m_szPtr = TSharedPtr<char>(new char[len+1],TLunaEngine::SPFM_DELETE_T);
		char* szMyContent = m_szPtr.getPointer();
		memset(szMyContent,0,len+1);
		vsprintf( szMyContent, format, args );
		szMyContent[len] = '\0';
		va_end(args);
		m_nLength=len;
	}

	int TString::Find(const char *szFind,int startPos,bool bRight) const
	{
		if(m_szPtr.isNull() || startPos<0 || startPos>=m_nLength || !szFind) return -1;

		TString strFind(szFind);
		TString strOrig(*this);
		int len = strFind.GetLength();
		if(len<=0) return -1;

		if(bRight)
		{
			for(int i=m_nLength-1;i>=0;i--)
			{
				if(m_nLength-i<len)
					continue;
				TString strTmp = strOrig.Mid(i,len);
				if(strTmp == strFind)
				{
					return i;
				}
			}
		}
		else
		{
			for(int i=startPos;i<m_nLength;i++)
			{
				if(len+i>=m_nLength+1)
					break;
				TString strTmp = strOrig.Mid(i,len);
				if(strTmp == strFind)
				{
					return i;
				}
			}
		}

		return -1;
	}

	int TString::Find(const wchar_t *szFind,int startPos,bool bRight) const
	{
		if(m_szPtr.isNull() || startPos<0 || startPos>=m_nLength || !szFind) return -1;

		TString strFind(szFind);
		TString strOrig(*this);
		int len = strFind.GetLength();
		if(len<=0) return -1;

		if(bRight)
		{
			for(int i=m_nLength-1;i>=0;i--)
			{
				if(m_nLength-i<len)
					continue;
				TString strTmp = strOrig.Mid(i,len);
				if(strTmp == strFind)
				{
					return i;
				}
			}
		}
		else
		{
			for(int i=startPos;i<m_nLength;i++)
			{
				if(len+i>=m_nLength+1)
					break;
				TString strTmp = strOrig.Mid(i,len);
				if(strTmp == strFind)
				{
					return i;
				}
			}
		}

		return -1;
	}

	int TString::Find(const TString& strFind,int startPos,bool bRight) const
	{
		if(m_szPtr.isNull() || startPos<0 || startPos>=m_nLength || strFind.m_szPtr.isNull()) return -1;

		//TString strFind(szFind);
		TString strOrig(*this);
		int len = strFind.GetLength();
		if(len<=0) return -1;

		if(bRight)
		{
			for(int i=m_nLength-1;i>=0;i--)
			{
				if(m_nLength-i<len)
					continue;
				TString strTmp = strOrig.Mid(i,len);
				if(strTmp == strFind)
				{
					return i;
				}
			}
		}
		else
		{
			for(int i=startPos;i<m_nLength;i++)
			{
				if(len+i>=m_nLength+1)
					break;
				TString strTmp = strOrig.Mid(i,len);
				if(strTmp == strFind)
				{
					return i;
				}
			}
		}

		return -1;
	}

	std::list<TString> TString::Split(char sz, int* pCount) const
	{
		std::list<TString> aResult;
		if(m_szPtr.isNull() || !pCount) 
			return aResult;

		TString strOrig(*this);
		int count = 0;

		for(int i=0;i<strOrig.GetLength();i++)
		{
			if(strOrig[i]==sz)
				count++;
		}

		int index = 0;

		do{
			int pos = strOrig.Find(&sz,0,false);
			if(pos==-1)
			{
				aResult.push_back(strOrig);
				break;
			}
			else
			{
				aResult.push_back(strOrig.Mid(0,pos));
				strOrig = strOrig.Mid(pos+1);
			}
		}while(++index<count+1);

		*pCount = count+1;
		return aResult;
	}

	std::list<TString> TString::Split(wchar_t sz, int* pCount) const
	{
		std::list<TString> aResult;
		if(m_szPtr.isNull() || !pCount) 
			return aResult;

		TString strOrig(*this);
		TString strFind(sz);
		int count = 0;

		for(int i=0;i<strOrig.GetLength();i++)
		{
			if(strOrig[i]==strFind[0])
				count++;
		}

		int index = 0;

		do{
			int pos = strOrig.Find(strFind,0,false);
			if(pos==-1)
			{
				aResult.push_back(strOrig);
				break;
			}
			else
			{
				aResult.push_back(strOrig.Mid(0,pos));
				strOrig = strOrig.Mid(pos+1);
			}
		}while(++index<count+1);

		*pCount = count+1;
		return aResult;
	}

	void TString::CutFilePath(char *fullname, char *filename, char *pathname)
	{
		if(!fullname)
			return;
		// 去除前面的路经
		char *pch = strrchr( fullname, '\\');
		// 如果需要保存名字就保存
		if(filename!=0)
		{
			if(pch)
			{
				strcpy( filename, ++pch );
			}
			else
			{
				strcpy( filename, fullname );
			}
		}
		pch = strrchr( fullname, '\\');
		// 如果需要保存路径就保存
		if(pathname!=0)
		{
			if(pch)
			{
				TString strTmp(fullname);
				int pos = strTmp.Find(pch,0,true);
				TString path = strTmp.Mid(0,pos+1);
				strcpy(pathname,path.GetString());
			}
			else
			{
				(*pathname) = '\0';
			}
		}
	}

	int TString::GetStringAsInt() const
	{
		if(m_szPtr.isNull())
			return 0;
		const char *szThis = m_szPtr.getPointer();
		return atoi(szThis);
	}

	float TString::GetStringAsFloat() const
	{
		if(m_szPtr.isNull())
			return 0.0f;
		const char *szThis = m_szPtr.getPointer();
		return (float)atof(szThis);
	}

	unsigned int TString::GetStringAsUInt() const
	{
		if(m_szPtr.isNull())
			return 0;
		const char *szThis = m_szPtr.getPointer();
		return (unsigned int)atoi(szThis);
	}

	long TString::GetStringAsLong() const
	{
		if(m_szPtr.isNull())
			return 0;
		const char *szThis = m_szPtr.getPointer();
		return atol(szThis);
	}

	unsigned long TString::GetStringAsULong() const
	{
		if(m_szPtr.isNull())
			return 0;
		const char *szThis = m_szPtr.getPointer();
		return (unsigned long)atol(szThis);
	}

	double TString::GetStringAsDouble() const
	{
		if(m_szPtr.isNull())
			return 0.0f;
		const char *szThis = m_szPtr.getPointer();
		return atof(szThis);
	}

	bool TString::GetStringAsBool() const
	{
		if(m_szPtr.isNull())
			return false;
		int n = GetStringAsInt();
		if(n==0)
			return false;
		else
			return true;
	}

	const char* TString::GetString() const
	{
		return m_szPtr.getPointer();
	}

	TSharedPtr<wchar_t> TString::GetWString() const
	{
		if(m_szPtr.isNull() || m_nLength<=0)
			return TSharedPtr<wchar_t>();
		wchar_t* newBuff = new wchar_t[m_nLength+1];
		mbstowcs(newBuff,m_szPtr.getPointer(),m_nLength);
		newBuff[m_nLength] = L'\0';
		return TSharedPtr<wchar_t>(newBuff,TLunaEngine::SPFM_DELETE_T);
	}

	TString& TString::MakeLower()
	{
		if(m_szPtr.isNull() || m_nLength<=0)
			return *this;
		char* szThis = m_szPtr.getPointer();
		for(int i=0;i<m_nLength;i++)
		{
			szThis[i] = tolower(szThis[i]);
		}
		return *this;
	}

	TString& TString::MakeUpper()
	{
		if(m_szPtr.isNull() || m_nLength==0)
			return *this;
		char* szThis = m_szPtr.getPointer();
		for(int i=0;i<m_nLength;i++)
		{
			szThis[i] = toupper(szThis[i]);
		}
		return *this;
	}

	TString& TString::SetCharAt(int pos,char value)
	{
		if(m_szPtr.isNull() || m_nLength==0)
			return *this;
		if(pos<0 || pos>=m_nLength)
			return *this;
		char* szNew = new char[m_nLength+1];
		memcpy(szNew,m_szPtr.getPointer(),m_nLength);
		szNew[pos]=value;
		szNew[m_nLength]='\0';
		m_szPtr.setNull();
		m_szPtr.bind(szNew,TLunaEngine::SPFM_DELETE_T);
		return *this;
	}

	TString& TString::SetCharAt(int pos,wchar_t value)
	{
		if(m_szPtr.isNull() || m_nLength==0)
			return *this;
		if(pos<0 || pos>=m_nLength)
			return *this;
		char* szNew = new char[m_nLength+1];
		memcpy(szNew,m_szPtr.getPointer(),m_nLength);
		wcstombs(&szNew[pos],&value,1);
		szNew[m_nLength]='\0';
		m_szPtr.setNull();
		m_szPtr.bind(szNew,TLunaEngine::SPFM_DELETE_T);
		return *this;
	}
}