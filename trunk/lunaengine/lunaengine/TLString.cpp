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
			TU32 n=0;
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
			TU32 n=0;
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
		TU32 n = 1;
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
		TU32 n = 1;
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
		TU32 len = strContent.m_nLength;
		m_nLength=len;
		//printf(m_szContent);
	}

	String::String(const TS32 iRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",iRight);
		TU32 n = (TU32)strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}
	String::String(const TU32 uiRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",uiRight);
		TU32 n = (TU32)strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}
	String::String(const TF32 fRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%f",fRight);
		TU32 n = (TU32)strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}
	String::String(const TF64 dRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%lf",dRight);
		TU32 n = (TU32)strlen(szTmp);
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
		TU32 n = 1;
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	String::String(const TS16 sRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%hd",sRight);
		TU32 n = (TU32)strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	String::String(const TU16 usRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%hu",usRight);
		TU32 n = (TU32)strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	String::String(const TS64 llRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%Ld", llRight);
		TU32 n = (TU32)strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
	}

	String::String(const TU64 ullRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%Lu",ullRight);
		TU32 n = (TU32)strlen(szTmp);
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
		TU32 len = strRight.m_nLength;
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
			TU32 n=0;
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
			TU32 n=0;
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

		TU32 n = 1;
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		TCHAR szTmp[2] = {0};
		szTmp[0] = cRight;
		memcpy(szMyContent,szTmp,n);
		szMyContent[n]='\0';
		m_nLength = n;

		return *this;
	}

	String& String::operator =(const TS32 iRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%d",iRight);
		TU32 n = (TU32)strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	String& String::operator =(const TU32 uiRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%u",uiRight);
		TU32 n = (TU32)strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	String& String::operator =(const TF32 fRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%f",fRight);
		TU32 n = (TU32)strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	String& String::operator =(const TF64 dRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%lf",dRight);
		TU32 n = (TU32)strlen(szTmp);
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
		TU32 n = 1;
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}

	String& String::operator =(const TS16 sRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%hd",sRight);
		TU32 n = (TU32)strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	String& String::operator =(const TU16 usRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%hu",usRight);
		TU32 n = (TU32)strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}

	String& String::operator =(const TS64 llRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%Ld",llRight);
		TU32 n = (TU32)strlen(szTmp);
		m_szPtr = SharedPtr<TCHAR>(new TCHAR[n+1],TLunaEngine::SPFM_DELETE_T);
		TCHAR* szMyContent = m_szPtr.getPointer();
		memcpy(szMyContent,szTmp,n);
		szMyContent[n] = '\0';
		m_nLength = n;
		return *this;
	}
	String& String::operator =(const TU64 ullRight)
	{
		m_szPtr.setNull();
		m_nLength = 0;
		TCHAR szTmp[64] = {0};
		sprintf_s(szTmp,64,"%Lu",ullRight);
		TU32 n = (TU32)strlen(szTmp);
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
			TU32 n = 0;
			const TCHAR* szTmp = szRight;
			while(*szTmp!='\0')
			{
				n++;
				szTmp++;
			}

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			szTmp=m_szPtr.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp=szRight;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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
			TU32 n = 0;
			const TWCHAR *szTmp = szRight;
			while(*szTmp!=L'\0')
			{
				n++;
				szTmp++;
			}

			TWCHAR *sz = new TWCHAR[n+m_nLength+1];
			SharedPtr<TWCHAR> pThis=GetWString();
			szTmp = pThis.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp=szRight;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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
			TU32 n = 1;
			TCHAR szTmp2[2] = {0};
			szTmp2[0] = cRight;

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			TCHAR* szTmp=m_szPtr.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp=szTmp2;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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
			TU32 n = 1;
			TWCHAR szTmp2[2] = {0};
			szTmp2[0] = cRight;

			TWCHAR *sz = new TWCHAR[n+m_nLength+1];
			SharedPtr<TWCHAR> pThis=GetWString();
			const TWCHAR *szTmp = pThis.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp=szTmp2;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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
			TU32 n = strRight.m_nLength;

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp=strRight.m_szPtr.getPointer();
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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

	String String::operator +(const TS32 iRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%d",iRight);
			TU32 n = (TU32)strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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
	String String::operator +(const TU32 uiRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%u",uiRight);
			TU32 n = (TU32)strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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
	String String::operator +(const TF32 fRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%f",fRight);
			TU32 n = (TU32)strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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
	String String::operator +(const TF64 dRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%lf",dRight);
			TU32 n = (TU32)strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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
			TCHAR szTmp2[1] = {0};
			if(bRight)
				szTmp2[0] = '1';
			else
				szTmp2[0] = '0';
			TU32 n = 1;

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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

	String String::operator +(const TS16 sRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%hd",sRight);
			TU32 n = (TU32)strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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
	String String::operator +(const TU16 usRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%hu",usRight);
			TU32 n = (TU32)strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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

	String String::operator +(const TS64 llRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%Ld", llRight);
			TU32 n = (TU32)strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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
	String String::operator +(const TU64 ullRight)
	{
		String result("");

		if(m_szPtr.isNull()==TFALSE)
		{
			TCHAR szTmp2[64] = {0};
			sprintf_s(szTmp2,64,"%Lu",ullRight);
			TU32 n = (TU32)strlen(szTmp2);

			TCHAR* sz = new TCHAR[n+m_nLength+1];
			const TCHAR* szTmp=m_szPtr.getPointer();
			for(TU32 i=0;i<m_nLength;i++)
			{
				sz[i]=*szTmp;
				szTmp++;
			}
			szTmp = szTmp2;
			for(TU32 i=m_nLength;i<n+m_nLength;i++)
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

	String& String::operator +=(const TS32 iRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + iRight;
		return *this;
	}
	String& String::operator +=(const TU32 uiRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + uiRight;
		return *this;
	}
	String& String::operator +=(const TF32 fRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + fRight;
		return *this;
	}
	String& String::operator +=(const TF64 dRight)
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

	String& String::operator +=(const TS16 sRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + sRight;
		return *this;
	}
	String& String::operator +=(const TU16 usRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + usRight;
		return *this;
	}

	String& String::operator +=(const TS64 llRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + llRight;
		return *this;
	}
	String& String::operator +=(const TU64 ullRight)
	{
		if(m_szPtr.isNull()) 
			return *this;

		*this = *this + ullRight;
		return *this;
	}

	TBOOL String::operator ==(const String &strRight) const
	{
		if(m_szPtr.isNull() || strRight.m_szPtr.isNull()) return TFALSE;
		if(m_nLength != strRight.m_nLength) return TFALSE;

		const TCHAR* szTmp = strRight.m_szPtr.getPointer();
		const TCHAR* szThis = m_szPtr.getPointer();
		for(TU32 i=0;i<m_nLength;i++)
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
		TU32 n = 0;
		while(*szTmp!='\0')
		{
			n++;
			szTmp++;
		}
		if(m_nLength != n) return TFALSE;

		szTmp = szRight;
		for(TU32 i=0;i<m_nLength;i++)
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
		TU32 n = 0;
		while(*szTmp!=L'\0')
		{
			n++;
			szTmp++;
		}
		if(m_nLength != n) return TFALSE;

		szTmp = szRight;
		for(TU32 i=0;i<m_nLength;i++)
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
		TU32 n = 0;
		while(*szTmp!='\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength < n)
		{
			szTmp = szRight;
			for(TU32 i=0;i<m_nLength;i++)
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
			for(TU32 i=0;i<n;i++)
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
		TU32 n = 0;
		while(*szTmp!=L'\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength < n)
		{
			szTmp = szRight;
			for(TU32 i=0;i<m_nLength;i++)
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
			for(TU32 i=0;i<n;i++)
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
		TU32 n = 0;
		while(*szTmp!='\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength <= n)
		{
			szTmp = szRight;
			for(TU32 i=0;i<m_nLength;i++)
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
			for(TU32 i=0;i<n;i++)
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
		TU32 n = 0;
		while(*szTmp!=L'\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength <= n)
		{
			szTmp = szRight;
			for(TU32 i=0;i<m_nLength;i++)
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
			for(TU32 i=0;i<n;i++)
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
		TU32 n = 0;
		while(*szTmp!='\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength > n)
		{
			szTmp = szRight;
			for(TU32 i=0;i<m_nLength;i++)
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
			for(TU32 i=0;i<n;i++)
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
		TU32 n = 0;
		while(*szTmp!=L'\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength > n)
		{
			szTmp = szRight;
			for(TU32 i=0;i<m_nLength;i++)
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
			for(TU32 i=0;i<n;i++)
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
		TU32 n = 0;
		while(*szTmp!='\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength >= n)
		{
			szTmp = szRight;
			for(TU32 i=0;i<m_nLength;i++)
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
			for(TU32 i=0;i<n;i++)
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
		TU32 n = 0;
		while(*szTmp!=L'\0')
		{
			n++;
			szTmp++;
		}

		if(m_nLength >= n)
		{
			szTmp = szRight;
			for(TU32 i=0;i<m_nLength;i++)
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
			for(TU32 i=0;i<n;i++)
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
		TU32 n = strRight.m_nLength;

		if(m_nLength < n)
		{
			for(TU32 i=0;i<m_nLength;i++)
			{
				if(szThis[i]>=*szTmp)
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			for(TU32 i=0;i<n;i++)
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
		TU32 n = strRight.m_nLength;

		if(m_nLength <= n)
		{
			for(TU32 i=0;i<m_nLength;i++)
			{
				if(szThis[i]>*szTmp)
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			for(TU32 i=0;i<n;i++)
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
		TU32 n = strRight.m_nLength;

		if(m_nLength > n)
		{
			for(TU32 i=0;i<m_nLength;i++)
			{
				if(szThis[i]<=*szTmp)
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			for(TU32 i=0;i<n;i++)
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
		TU32 n = strRight.m_nLength;

		if(m_nLength >= n)
		{
			for(TU32 i=0;i<m_nLength;i++)
			{
				if(szThis[i]<*szTmp)
					return TTRUE;
				szTmp++;
			}
			return TFALSE;
		}
		else
		{
			for(TU32 i=0;i<n;i++)
			{
				if(szThis[i]>=*szTmp)
					return TFALSE;
				szTmp++;
			}
			return TTRUE;
		}
	}

	TCHAR String::operator [](TU32 pos) const
	{
		if(pos<m_nLength && m_szPtr.isNull()==TFALSE && pos>=0)
		{
			const TCHAR* szThis = m_szPtr.getPointer();
			return szThis[pos];
		}

		return 0;
	}

	String String::Mid(TU32 startPos, TU32 count) const
	{
		String result("");

		if(startPos<m_nLength && m_szPtr.isNull()==TFALSE && count>0 && startPos>=0)
		{
			const TCHAR* szThis = m_szPtr.getPointer();
			TCHAR* szTmp = new TCHAR[count+1];
			TU32 i = 0;
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

	String String::Mid(TU32 startPos) const
	{
		String result("");

		if(startPos<m_nLength && m_szPtr.isNull()==TFALSE && startPos>=0)
		{
			const TCHAR* szThis = m_szPtr.getPointer();
			TU32 count = m_nLength - startPos;
			TCHAR* szTmp = new TCHAR[count+1];
			TU32 i = 0;
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
		TS32 len;

		va_start( args, format );
		len = _vscprintf( format, args ); 
		if (len >= 0)
		{
			m_szPtr = SharedPtr<TCHAR>(new TCHAR[len + 1], TLunaEngine::SPFM_DELETE_T);
			TCHAR* szMyContent = m_szPtr.getPointer();
			memset(szMyContent, 0, len + 1);
			vsprintf(szMyContent, format, args);
			szMyContent[len] = '\0';
		}
		va_end(args);
		if (len >= 0)
		{
			m_nLength = (TU32)len;
		}
	}

	TS32 String::Find(const TCHAR* szFind, TU32 startPos,TBOOL bRight) const
	{
		if(m_szPtr.isNull() || startPos<0 || startPos>=m_nLength || !szFind) return -1;

		String strFind(szFind);
		String strOrig(*this);
		TU32 len = strFind.GetLength();
		if(len<=0) return -1;

		if(bRight)
		{
			for(TU32 i=m_nLength-1;i>=0;i--)
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
			for(TU32 i=startPos;i<m_nLength;i++)
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

	TS32 String::Find(const TWCHAR *szFind, TU32 startPos,TBOOL bRight) const
	{
		if(m_szPtr.isNull() || startPos<0 || startPos>=m_nLength || !szFind) return -1;

		String strFind(szFind);
		String strOrig(*this);
		TU32 len = strFind.GetLength();
		if(len<=0) return -1;

		if(bRight)
		{
			for(TU32 i=m_nLength-1;i>=0;i--)
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
			for(TU32 i=startPos;i<m_nLength;i++)
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

	TS32 String::Find(const String& strFind, TU32 startPos,TBOOL bRight) const
	{
		if(m_szPtr.isNull() || startPos<0 || startPos>=m_nLength || strFind.m_szPtr.isNull()) return -1;

		//String strFind(szFind);
		String strOrig(*this);
		TU32 len = strFind.GetLength();
		if(len<=0) return -1;

		if(bRight)
		{
			for(TU32 i=m_nLength-1;i>=0;i--)
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
			for(TU32 i=startPos;i<m_nLength;i++)
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

	std::vector<String> String::Split(TCHAR sz, TU32* pCount) const
	{
		std::vector<String> aResult;
		if(m_szPtr.isNull() || !pCount) 
			return aResult;

		String strOrig(*this);
		TU32 count = 0;

		for(TU32 i=0;i<strOrig.GetLength();i++)
		{
			if(strOrig[i]==sz)
				count++;
		}

		TU32 index = 0;

		do{
			TS32 pos = strOrig.Find(&sz,0,TFALSE);
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

	std::vector<String> String::Split(TWCHAR sz, TU32* pCount) const
	{
		std::vector<String> aResult;
		if(m_szPtr.isNull() || !pCount) 
			return aResult;

		String strOrig(*this);
		String strFind(sz);
		TU32 count = 0;

		for(TU32 i=0;i<strOrig.GetLength();i++)
		{
			if(strOrig[i]==strFind[0])
				count++;
		}

		TU32 index = 0;

		do{
			TS32 pos = strOrig.Find(strFind,0,TFALSE);
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
				TS32 pos = strTmp.Find(pch,0,TTRUE);
				String path = strTmp.Mid(0,pos+1);
				strcpy(pathname,path.GetString());
			}
			else
			{
				(*pathname) = '\0';
			}
		}
	}

	TS32 String::GetStringAsInt() const
	{
		if(m_szPtr.isNull())
			return 0;
		const TCHAR* szThis = m_szPtr.getPointer();
		return atoi(szThis);
	}

	TF32 String::GetStringAsFloat() const
	{
		if(m_szPtr.isNull())
			return 0.0f;
		const TCHAR* szThis = m_szPtr.getPointer();
		return (TF32)atof(szThis);
	}

	TU32 String::GetStringAsUInt() const
	{
		if(m_szPtr.isNull())
			return 0;
		const TCHAR* szThis = m_szPtr.getPointer();
		return (TU32)atoi(szThis);
	}

	TS16 String::GetStringAsShort() const
	{
		if (m_szPtr.isNull())
			return 0;
		const TCHAR* szThis = m_szPtr.getPointer();
		return (TS16)atoi(szThis);
	}

	TU16 String::GetStringAsUShort() const
	{
		if (m_szPtr.isNull())
			return 0;
		const TCHAR* szThis = m_szPtr.getPointer();
		return (TU16)atoi(szThis);
	}

	TS64 String::GetStringAsLongLong() const
	{
		if(m_szPtr.isNull())
			return 0;
		const TCHAR* szThis = m_szPtr.getPointer();
		return atoll(szThis);
	}

	TU64 String::GetStringAsULongLong() const
	{
		if(m_szPtr.isNull())
			return 0;
		const TCHAR* szThis = m_szPtr.getPointer();
		return (TU64)atoll(szThis);
	}

	TF64 String::GetStringAsDouble() const
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
		TU32 n = GetStringAsUInt();
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
		for(TU32 i=0;i<m_nLength;i++)
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
		for(TU32 i=0;i<m_nLength;i++)
		{
			szThis[i] = toupper(szThis[i]);
		}
		return *this;
	}

	String& String::SetCharAt(TU32 pos,TCHAR value)
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

	String& String::SetCharAt(TU32 pos,TWCHAR value)
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