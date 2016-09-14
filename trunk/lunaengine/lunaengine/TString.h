#pragma once

#include "TSharedPtr.h"
#include <list>
using namespace std;
namespace TLunaEngine{

	class TString
	{
	protected:
		//char *m_szContent;
		TSharedPtr<char> m_szPtr;
		int m_nLength;
	public:
		TString(void);
		~TString(void);
	public:
		// ���غ���
		TString(const char *szContent);
		TString(const wchar_t *szContent);
		TString(const TString &strContent);
		TString(const char cContent);
		TString(const wchar_t cContent);
		TString(const int iRight);
		TString(const unsigned int uiRight);
		TString(const float fRight);
		TString(const double dRight);
		TString(const bool bRight);
		TString(const short sRight);
		TString(const unsigned short usRight);
		TString(const long lRight);
		TString(const unsigned long ulRight);
		TString& operator =(const char *szRight);
		TString& operator =(const wchar_t *szRight);
		TString& operator =(const TString &strRight);
		TString& operator =(const char cRight);
		TString& operator =(const int iRight);
		TString& operator =(const unsigned int uiRight);
		TString& operator =(const float fRight);
		TString& operator =(const double dRight);
		TString& operator =(const bool bRight);
		TString& operator =(const short sRight);
		TString& operator =(const unsigned short usRight);
		TString& operator =(const long lRight);
		TString& operator =(const unsigned long ulRight);
		TString operator +(const char *szRight);
		TString operator +(const wchar_t *szRight);
		TString operator +(const TString &strRight);
		TString operator +(const char cRight);
		TString operator +(const wchar_t cRight);
		TString operator +(const int iRight);
		TString operator +(const unsigned int uiRight);
		TString operator +(const float fRight);
		TString operator +(const double dRight);
		TString operator +(const bool bRight);
		TString operator +(const short sRight);
		TString operator +(const unsigned short usRight);
		TString operator +(const long lRight);
		TString operator +(const unsigned long ulRight);
		TString& operator +=(const char *szRight);
		TString& operator +=(const wchar_t *szRight);
		TString& operator +=(const TString &strRight);
		TString& operator +=(const char cRight);
		TString& operator +=(const wchar_t cRight);
		TString& operator +=(const int iRight);
		TString& operator +=(const unsigned int uiRight);
		TString& operator +=(const float fRight);
		TString& operator +=(const double dRight);
		TString& operator +=(const bool bRight);
		TString& operator +=(const short sRight);
		TString& operator +=(const unsigned short usRight);
		TString& operator +=(const long lRight);
		TString& operator +=(const unsigned long ulRight);
		bool operator ==(const char *szRight) const;
		bool operator ==(const wchar_t *szRight) const;
		bool operator ==(const TString &strRight) const;
		bool operator !=(const char *szRight) const;
		bool operator !=(const wchar_t *szRight) const;
		bool operator !=(const TString &strRight) const;
		bool operator >=(const char *szRight) const;
		bool operator >=(const wchar_t *szRight) const;
		bool operator >=(const TString &strRight) const;
		bool operator >(const char *szRight) const;
		bool operator >(const wchar_t *szRight) const;
		bool operator >(const TString &strRight) const; 
		bool operator <=(const char *szRight) const;
		bool operator <=(const wchar_t *szRight) const;
		bool operator <=(const TString &strRight) const;
		bool operator <(const char *szRight) const;
		bool operator <(const wchar_t *szRight) const;
		bool operator <(const TString &strRight) const; 
		char operator [](int pos) const;
	public:
		// ���ַ�����ɸ�������
		const char * GetString() const;
		TSharedPtr<wchar_t> GetWString() const;
		int GetStringAsInt() const;
		float GetStringAsFloat() const;
		unsigned int GetStringAsUInt() const;
		long GetStringAsLong() const;
		unsigned long GetStringAsULong() const;
		double GetStringAsDouble() const;
		bool GetStringAsBool() const;
		// �õ��ַ�������
		int GetLength() const{return m_nLength;}
		// �����ַ���
		TString Mid(int startPos,int count) const;
		TString Mid(int startPos) const;
		// �����ַ���
		void Format(const char *format, ...);
		// �����ַ���λ��
		int Find(const char *szFind,int startPos,bool bRight) const;
		int Find(const wchar_t *szFind,int startPos,bool bRight) const;
		int Find(const TString& strFind,int startPos,bool bRight) const;
		// �ֽ��ַ���
		std::list<TString> Split(char sz,int* pCount) const;
		std::list<TString> Split(wchar_t sz,int* pCount) const;
		//// ɾ������Ŀռ���
		//// ������������DLL�ͷſռ�����
		//inline void ClearSplitList(std::list<TString>& list)
		//{
		//	list.clear();
		//}
		// �ַ�����д��
		TString& MakeUpper();
		// �ַ���Сд��
		TString& MakeLower();
		// set char using index
		TString& SetCharAt(int pos,char value);
		TString& SetCharAt(int pos,wchar_t value);
	public:
		// -------------- һЩ���ַ����йص�ȫ�ַ��� --------
		static void CutFilePath(char* fullname,char* filename=0,char* pathname=0);
		// --------------------------------------------------
	};

}