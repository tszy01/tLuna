#include "TLString.h"

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

	String::String(TVOID) : m_nLength(0)
	{
	}

	String::~String(TVOID)
	{
		m_szPtr.setNull();
		m_nLength=0;
	}

	String::String(const TCHAR* szContent)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		if(szContent!=0)
		{
			int n=0;
			const TCHAR* szTmp = szContent;
			while(*szTmp!='\0')
			{
				n++;
				szTmp++;
			}
			szTmp=0;
			m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
			TCHAR* szMyContent = m_szPtr.getPointer();
			memcpy(szMyContent,szContent,n);
			szMyContent[n]='\0';
			m_nLength = n;
			//printf(m_szContent);
		}
	}

	String::String(const TWCHAR *szContent)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		if(szContent!=0)
		{
			int n=0;
			const TWCHAR *szTmp = szContent;
			while(*szTmp!=L'\0')
			{
				n++;
				szTmp++;
			}
			szTmp=0;
			m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
			TCHAR* szMyContent = m_szPtr.getPointer();
			wcstombs(szMyContent,szContent,n);
			szMyContent[n]='\0';
			m_nLength = n;
			//printf(m_szContent);
		}
	}

	String::String(const TCHAR cContent)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		int n = 1;
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		TCHAR szTmp[2] = {0};
		szTmp[0] = cContent;
		memcpy(szMyContent,szTmp,n);
		szMyContent[n]='\0';
		m_nLength = n;
	}

	String::String(const TWCHAR cContent)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		int n = 1;
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		TWCHAR szTmp[2] = {0};
		szTmp[0] = cContent;
		wcstombs(szMyContent,szTmp,n);
		szMyContent[n]='\0';
		m_nLength = n;
	}

	String::String(const String &strContent)
	{
		if(strContent.m_szPtr.isNull()) return;
		m_szPtr.setNull();
		m_nLength = 0;
		m_szPtr = strContent.m_szPtr;
		int len = strContent.m_nLength;
		m_nLength=len;
		//printf(m_szContent);
	}

	String::String(const int iRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",iRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}
	String::String(const unsigned int uiRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",uiRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}
	String::String(const float fRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%f",fRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}
	String::String(const double dRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%f",dRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}
	String::String(const TBOOL bRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[1] = {0};
		if(bRight)
			szTmp[0] = '1';
		else
			szTmp[0] = '0';
		int n = 1;
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	String::String(const short sRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",sRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	String::String(const unsigned short usRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",usRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	String::String(const long lRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",lRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	String::String(const unsigned long ulRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",ulRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	String& String::operator =(const String &strRight)
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

	String& String::operator =(const TCHAR* szRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;

		if(szRight!=0)
		{
			int n=0;
			const TCHAR* szTmp = szRight;
			while(*szTmp!='\0')
			{
				n++;
				szTmp++;
			}
			szTmp=0;
			m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
			TCHAR* szMyContent = m_szPtr.getPointer();
			memcpy(szMyContent,szRight,n);
			szMyContent[n]='\0';
			m_nLength = n;
			//printf(m_szContent);
		}

		return *this;
	}

	String& String::operator =(const TWCHAR *szRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;

		if(szRight!=0)
		{
			int n=0;
			const TWCHAR *szTmp = szRight;
			while(*szTmp!=L'\0')
			{
				n++;
				szTmp++;
			}
			szTmp=0;
			m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
			TCHAR* szMyContent = m_szPtr.getPointer();
			wcstombs(szMyContent,szRight,n);
			szMyContent[n]='\0';
			m_nLength = n;
			//printf(m_szContent);
		}

		return *this;
	}

	String& String::operator =(const TCHAR cRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;

		int n = 1;
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		TCHAR szTmp[2] = {0};
		szTmp[0] = cRight;
		memcpy(szMyContent,szTmp,n);
		szMyContent[n]='\0';
		m_nLength = n;

		return *this;
	}

	String& String::operator =(const int iRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",iRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	String& String::operator =(const unsigned int uiRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",uiRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	String& String::operator =(const float fRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%f",fRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	String& String::operator =(const double dRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%f",dRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	String& String::operator =(const TBOOL bRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[1] = {0};
		if(bRight)
			szTmp[0] = '1';
		else
			szTmp[0] = '0';
		int n = 1;
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}

	String& String::operator =(const short sRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",sRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	String& String::operator =(const unsigned short usRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",usRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}

	String& String::operator =(const long lRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",lRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	String& String::operator =(const unsigned long ulRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",ulRight);
		int n = strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}

	String String::operator +(const TCHAR* szRight)
	{
		String result("");

		if(szRight!=0 && m_szPtr.isNull()==TFALSE)
		{
			int n=0;
			const TCHAR* szTmp = szRight;
			while(*szTmp!='\0')
			{
				n++;
				szTmp++;
			}

			TCHAR* sz = new TCHAR[n+m_nLength+1];
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

	String String::operator +(const TWCHAR *szRight)
	{
		String result("");

		if(szRight!=0 && m_szPtr.isNull()==TFALSE)
		{
			int n=0;
			const TWCHAR *szTmp = szRight;
			while(*szTmp!=L'\0')
			{
				n++;
				szTmp++;
			}

			TWCHAR *sz = new TWCHAR[n+m_nLength+1];
			SharedPtr<TWCHAR> pThis=GetWString();
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

	String String::operator +(const TCHAR cRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			int n=1;
			TCHAR szTmp2[2] = {0};
			szTmp2[0] = cRight;

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			TCHAR* szTmp=m_szPtr.getPointer();
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

	String String::operator +(const TWCHAR cRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			int n=1;
			TWCHAR szTmp2[2] = {0};
			szTmp2[0] = cRight;

			TWCHAR *sz = new TWCHAR[n+m_nLength+1];
			SharedPtr<TWCHAR> pThis=GetWString();
			const TWCHAR *szTmp = pThis.getPointer();
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

	String String::operator +(const String &strRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE && strRight.m_szPtr.isNull()==TFALSE)
		{
			int n = strRight.m_nLength;

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
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

	String String::operator +(const int iRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			//int n = strRight.m_nLength;
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%d",iRight);
			int n = strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
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
	String String::operator +(const unsigned int uiRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			//int n = strRight.m_nLength;
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%u",uiRight);
			int n = strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
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
	String String::operator +(const float fRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			//int n = strRight.m_nLength;
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%f",fRight);
			int n = strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
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
	String String::operator +(const double dRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			//int n = strRight.m_nLength;
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%f",dRight);
			int n = strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
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
	String String::operator +(const TBOOL bRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			//int n = strRight.m_nLength;
			TCHAR szTmp2[1] = {0};
			if(bRight)
				szTmp2[0] = '1';
			else
				szTmp2[0] = '0';
			int n = 1;

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
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

	String String::operator +(const short sRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			//int n = strRight.m_nLength;
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%d",sRight);
			int n = strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
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
	String String::operator +(const unsigned short usRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			//int n = strRight.m_nLength;
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%u",usRight);
			int n = strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
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

	String String::operator +(const long lRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			//int n = strRight.m_nLength;
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%d",lRight);
			int n = strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
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
	String String::operator +(const unsigned long ulRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			//int n = strRight.m_nLength;
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%u",ulRight);
			int n = strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
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

	String& String::operator +=(const TCHAR* szRight)
	{
		if(m_szPtr.isNull() || !szRight) 
			return *this;

		*this = *this + szRight;
		return *this;
	}

	String& String::operator +=(const TWCHAR *szRight)
	{
		if(m_szPtr.isNull() || !szRight) 
			return *this;

		*this = *this + szRight;
		return *this;
	}

	String& String::operator +=(const TCHAR cRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + cRight;
		return *this;
	}

	String& String::operator +=(const TWCHAR cRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + cRight;
		return *this;
	}

	String& String::operator +=(const String &strRight)
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) 
			return *this;

		*this = *this + strRight;
		return *this;
	}

	String& String::operator +=(const int iRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + iRight;
		return *this;
	}
	String& String::operator +=(const unsigned int uiRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + uiRight;
		return *this;
	}
	String& String::operator +=(const float fRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + fRight;
		return *this;
	}
	String& String::operator +=(const double dRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + dRight;
		return *this;
	}
	String& String::operator +=(const TBOOL bRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + bRight;
		return *this;
	}

	String& String::operator +=(const short sRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + sRight;
		return *this;
	}
	String& String::operator +=(const unsigned short usRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + usRight;
		return *this;
	}

	String& String::operator +=(const long lRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + lRight;
		return *this;
	}
	String& String::operator +=(const unsigned long ulRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + ulRight;
		return *this;
	}

	TBOOL String::operator ==(const String &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return TFALSE;
		if(m_nLength != strRight.m_nLength) return TFALSE;

		const TCHAR* szTmp = strRight.m_szPtr.getPointer();
		const TCHAR* szThis = m_szPtr.getPointer();
		for(int i=0;i<m_nLength;i++)
		{
			if(*szTmp!=szThis[i])
				return TFALSE;
			szTmp++;
		}

		return TTRUE;
	}

	TBOOL String::operator ==(const TCHAR* szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return TFALSE;

		const TCHAR* szTmp = szRight;
		const TCHAR* szThis = m_szPtr.getPointer();
		int n = 0;
		while(*szTmp!='\0')
		{
			n++;
			szTmp++;
		}
		if(m_nLength != n) return TFALSE;

		szTmp = szRight;
		for(int i=0;i<m_nLength;i++)
		{
			if(*szTmp!=szThis[i])
				return TFALSE;
			szTmp++;
		}

		return TTRUE;
	}

	TBOOL String::operator ==(const TWCHAR *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return TFALSE;

		const TWCHAR *szTmp = szRight;
		SharedPtr<TWCHAR> pThis=GetWString();
		const TWCHAR *szThis = pThis.getPointer();
		int n = 0;
		while(*szTmp!=L'\0')
		{
			n++;
			szTmp++;
		}
		if(m_nLength != n) return TFALSE;

		szTmp = szRight;
		for(int i=0;i<m_nLength;i++)
		{
			if(*szTmp!=szThis[i])
				return TFALSE;
			szTmp++;
		}

		return TTRUE;
	}

	TBOOL String::operator !=(const TCHAR* szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return TFALSE;
		return !(*this==szRight);
	}

	TBOOL String::operator !=(const TWCHAR *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return TFALSE;
		return !(*this==szRight);
	}

	TBOOL String::operator !=(const String &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return TFALSE;
		return !(*this==strRight);
	}

	TBOOL String::operator >=(const TCHAR* szRight) const 
	{
		if(m_szPtr.isNull() || !szRight) return TFALSE;

		const TCHAR* szTmp = szRight;
		const TCHAR* szThis = m_szPtr.getPointer();
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
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]<*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TBOOL String::operator >=(const TWCHAR *szRight) const 
	{
		if(m_szPtr.isNull() || !szRight) return TFALSE;

		const TWCHAR *szTmp = szRight;
		SharedPtr<TWCHAR> pThis=GetWString();
		const TWCHAR *szThis = pThis.getPointer();
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
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]<*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TBOOL String::operator >(const TCHAR* szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return TFALSE;

		const TCHAR* szTmp = szRight;
		const TCHAR* szThis = m_szPtr.getPointer();
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
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]<=*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TBOOL String::operator >(const TWCHAR *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return TFALSE;

		const TWCHAR *szTmp = szRight;
		SharedPtr<TWCHAR> pThis=GetWString();
		const TWCHAR *szThis = pThis.getPointer();
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
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]<=*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TBOOL String::operator <=(const TCHAR* szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return TFALSE;

		const TCHAR* szTmp = szRight;
		const TCHAR* szThis = m_szPtr.getPointer();
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
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]>*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TBOOL String::operator <=(const TWCHAR *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return TFALSE;

		const TWCHAR *szTmp = szRight;
		SharedPtr<TWCHAR> pThis=GetWString();
		const TWCHAR *szThis = pThis.getPointer();
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
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]>*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TBOOL String::operator <(const TCHAR* szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return TFALSE;

		const TCHAR* szTmp = szRight;
		const TCHAR* szThis = m_szPtr.getPointer();
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
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]>=*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TBOOL String::operator <(const TWCHAR *szRight) const
	{
		if(m_szPtr.isNull() || !szRight) return TFALSE;

		const TWCHAR *szTmp = szRight;
		SharedPtr<TWCHAR> pThis=GetWString();
		const TWCHAR *szThis = pThis.getPointer();
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
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			szTmp = szRight;
			for(int i=0;i<n;i++)
			{
				if(szThis[i]>=*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TBOOL String::operator >=(const String &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return TFALSE;

		const TCHAR* szTmp = strRight.m_szPtr.getPointer();
		const TCHAR* szThis = m_szPtr.getPointer();
		int n = strRight.m_nLength;

		if(m_nLength < n)
		{
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]>=*szTmp)
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			for(int i=0;i<n;i++)
			{
				if(szThis[i]<*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TBOOL String::operator >(const String &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return TFALSE;

		const TCHAR* szTmp = strRight.m_szPtr.getPointer();
		const TCHAR* szThis = m_szPtr.getPointer();
		int n = strRight.m_nLength;

		if(m_nLength <= n)
		{
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]>*szTmp)
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			for(int i=0;i<n;i++)
			{
				if(szThis[i]<=*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TBOOL String::operator <=(const String &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return TFALSE;

		const TCHAR* szTmp = strRight.m_szPtr.getPointer();
		const TCHAR* szThis = m_szPtr.getPointer();
		int n = strRight.m_nLength;

		if(m_nLength > n)
		{
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]<=*szTmp)
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			for(int i=0;i<n;i++)
			{
				if(szThis[i]>*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TBOOL String::operator <(const String &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return TFALSE;

		const TCHAR* szTmp = strRight.m_szPtr.getPointer();
		const TCHAR* szThis = m_szPtr.getPointer();
		int n = strRight.m_nLength;

		if(m_nLength >= n)
		{
			for(int i=0;i<m_nLength;i++)
			{
				if(szThis[i]<*szTmp)
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			for(int i=0;i<n;i++)
			{
				if(szThis[i]>=*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TCHAR String::operator [](int pos) const
	{
		if(pos<m_nLength && m_szPtr.isNull()==TFALSE && pos>=0)
		{
			const TCHAR* szThis = m_szPtr.getPointer();
			return szThis[pos];
		}

		return 0;
	}

	String String::Mid(int startPos,int count) const
	{
		String result("");

		if(startPos<m_nLength && m_szPtr.isNull()==TFALSE && count>0 && startPos>=0)
		{
			const TCHAR* szThis = m_szPtr.getPointer();
			TCHAR* szTmp = new TCHAR[count+1];
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

	String String::Mid(int startPos) const
	{
		String result("");

		if(startPos<m_nLength && m_szPtr.isNull()==TFALSE && startPos>=0)
		{
			const TCHAR* szThis = m_szPtr.getPointer();
			int count = m_nLength - startPos;
			TCHAR* szTmp = new TCHAR[count+1];
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

	TVOID String::Format(const TCHAR* format, ...)
	{
		if(!format) return;

		m_szPtr.setNull();
		m_nLength = 0;

		va_list args;
		int len;

		va_start( args, format );
		len = _vscprintf( format, args ); 
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[len+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memset(szMyContent,0,len+1);
		vsprintf( szMyContent, format, args );
		szMyContent[len] = '\0';
		va_end(args);
		m_nLength=len;
	}

	int String::Find(const TCHAR* szFind,int startPos,TBOOL bRight) const
	{
		if(m_szPtr.isNull() || startPos<0 || startPos>=m_nLength || !szFind) return -1;

		String strFind(szFind);
		String strOrig(*this);
		int len = strFind.GetLength();
		if(len<=0) return -1;

		if(bRight)
		{
			for(int i=m_nLength-1;i>=0;i--)
			{
				if(m_nLength-i<len)
					continue;
				String strTmp = strOrig.Mid(i,len);
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
				String strTmp = strOrig.Mid(i,len);
				if(strTmp == strFind)
				{
					return i;
				}
			}
		}

		return -1;
	}

	int String::Find(const TWCHAR *szFind,int startPos,TBOOL bRight) const
	{
		if(m_szPtr.isNull() || startPos<0 || startPos>=m_nLength || !szFind) return -1;

		String strFind(szFind);
		String strOrig(*this);
		int len = strFind.GetLength();
		if(len<=0) return -1;

		if(bRight)
		{
			for(int i=m_nLength-1;i>=0;i--)
			{
				if(m_nLength-i<len)
					continue;
				String strTmp = strOrig.Mid(i,len);
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
				String strTmp = strOrig.Mid(i,len);
				if(strTmp == strFind)
				{
					return i;
				}
			}
		}

		return -1;
	}

	int String::Find(const String& strFind,int startPos,TBOOL bRight) const
	{
		if(m_szPtr.isNull() || startPos<0 || startPos>=m_nLength || strFind.m_szPtr.isNull()) return -1;

		//String strFind(szFind);
		String strOrig(*this);
		int len = strFind.GetLength();
		if(len<=0) return -1;

		if(bRight)
		{
			for(int i=m_nLength-1;i>=0;i--)
			{
				if(m_nLength-i<len)
					continue;
				String strTmp = strOrig.Mid(i,len);
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
				String strTmp = strOrig.Mid(i,len);
				if(strTmp == strFind)
				{
					return i;
				}
			}
		}

		return -1;
	}

	std::vector<String> String::Split(TCHAR sz, int* pCount) const
	{
		std::vector<String> aResult;
		if(m_szPtr.isNull() || !pCount) 
			return aResult;

		String strOrig(*this);
		int count = 0;

		for(int i=0;i<strOrig.GetLength();i++)
		{
			if(strOrig[i]==sz)
				count++;
		}

		int index = 0;

		do{
			int pos = strOrig.Find(&sz,0,TFALSE);
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

	std::vector<String> String::Split(TWCHAR sz, int* pCount) const
	{
		std::vector<String> aResult;
		if(m_szPtr.isNull() || !pCount) 
			return aResult;

		String strOrig(*this);
		String strFind(sz);
		int count = 0;

		for(int i=0;i<strOrig.GetLength();i++)
		{
			if(strOrig[i]==strFind[0])
				count++;
		}

		int index = 0;

		do{
			int pos = strOrig.Find(strFind,0,TFALSE);
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

	TVOID String::CutFilePath(TCHAR* fullname, TCHAR* filename, TCHAR* pathname)
	{
		if(!fullname)
			return;
		// 去除前面的路经
		TCHAR* pch = strrchr( fullname, '\\');
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
				String strTmp(fullname);
				int pos = strTmp.Find(pch,0,TTRUE);
				String path = strTmp.Mid(0,pos+1);
				strcpy(pathname,path.GetString());
			}
			else
			{
				(*pathname) = '\0';
			}
		}
	}

	int String::GetStringAsInt() const
	{
		if(m_szPtr.isNull())
			return 0;
		const TCHAR* szThis = m_szPtr.getPointer();
		return atoi(szThis);
	}

	float String::GetStringAsFloat() const
	{
		if(m_szPtr.isNull())
			return 0.0f;
		const TCHAR* szThis = m_szPtr.getPointer();
		return (float)atof(szThis);
	}

	unsigned int String::GetStringAsUInt() const
	{
		if(m_szPtr.isNull())
			return 0;
		const TCHAR* szThis = m_szPtr.getPointer();
		return (unsigned int)atoi(szThis);
	}

	long String::GetStringAsLong() const
	{
		if(m_szPtr.isNull())
			return 0;
		const TCHAR* szThis = m_szPtr.getPointer();
		return atol(szThis);
	}

	unsigned long String::GetStringAsULong() const
	{
		if(m_szPtr.isNull())
			return 0;
		const TCHAR* szThis = m_szPtr.getPointer();
		return (unsigned long)atol(szThis);
	}

	double String::GetStringAsDouble() const
	{
		if(m_szPtr.isNull())
			return 0.0f;
		const TCHAR* szThis = m_szPtr.getPointer();
		return atof(szThis);
	}

	TBOOL String::GetStringAsBool() const
	{
		if(m_szPtr.isNull())
			return TFALSE;
		int n = GetStringAsInt();
		if(n==0)
			return TFALSE;
		else
			return TTRUE;
	}

	const TCHAR* String::GetString() const
	{
		return m_szPtr.getPointer();
	}

	SharedPtr<TWCHAR> String::GetWString() const
	{
		if(m_szPtr.isNull() || m_nLength<=0)
			return SharedPtr<TWCHAR>();
		TWCHAR* newBuff = new TWCHAR[m_nLength+1];
		mbstowcs(newBuff,m_szPtr.getPointer(),m_nLength);
		newBuff[m_nLength] = L'\0';
		return SharedPtr<TWCHAR>(newBuff,TLunaEngine::SPFM_DELETE_T);
	}

	String& String::MakeLower()
	{
		if(m_szPtr.isNull() || m_nLength<=0)
			return *this;
		TCHAR* szThis = m_szPtr.getPointer();
		for(int i=0;i<m_nLength;i++)
		{
			szThis[i] = tolower(szThis[i]);
		}
		return *this;
	}

	String& String::MakeUpper()
	{
		if(m_szPtr.isNull() || m_nLength==0)
			return *this;
		TCHAR* szThis = m_szPtr.getPointer();
		for(int i=0;i<m_nLength;i++)
		{
			szThis[i] = toupper(szThis[i]);
		}
		return *this;
	}

	String& String::SetCharAt(int pos,TCHAR value)
	{
		if(m_szPtr.isNull() || m_nLength==0)
			return *this;
		if(pos<0 || pos>=m_nLength)
			return *this;
		TCHAR* szNew = new TCHAR[m_nLength+1];
		memcpy(szNew,m_szPtr.getPointer(),m_nLength);
		szNew[pos]=value;
		szNew[m_nLength]='\0';
		m_szPtr.setNull();
		m_szPtr.bind(szNew,TLunaEngine::SPFM_DELETE_T);
		return *this;
	}

	String& String::SetCharAt(int pos,TWCHAR value)
	{
		if(m_szPtr.isNull() || m_nLength==0)
			return *this;
		if(pos<0 || pos>=m_nLength)
			return *this;
		TCHAR* szNew = new TCHAR[m_nLength+1];
		memcpy(szNew,m_szPtr.getPointer(),m_nLength);
		wcstombs(&szNew[pos],&value,1);
		szNew[m_nLength]='\0';
		m_szPtr.setNull();
		m_szPtr.bind(szNew,TLunaEngine::SPFM_DELETE_T);
		return *this;
	}
}