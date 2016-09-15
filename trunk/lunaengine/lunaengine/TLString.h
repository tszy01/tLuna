#ifndef _TLSTRING_H_
#define _TLSTRING_H_

#include "TLSharedPtr.h"
#include <list>
#include <vector>
namespace TLunaEngine{

	class String
	{
	protected:
		//char *m_szContent;
		SharedPtr<char> m_szPtr;
		int m_nLength;
	public:
		String(void);
		~String(void);
	public:
		// ���غ���
		String(const char *szContent);
		String(const wchar_t *szContent);
		String(const String &strContent);
		String(const char cContent);
		String(const wchar_t cContent);
		String(const int iRight);
		String(const unsigned int uiRight);
		String(const float fRight);
		String(const double dRight);
		String(const bool bRight);
		String(const short sRight);
		String(const unsigned short usRight);
		String(const long lRight);
		String(const unsigned long ulRight);
		String& operator =(const char *szRight);
		String& operator =(const wchar_t *szRight);
		String& operator =(const String &strRight);
		String& operator =(const char cRight);
		String& operator =(const int iRight);
		String& operator =(const unsigned int uiRight);
		String& operator =(const float fRight);
		String& operator =(const double dRight);
		String& operator =(const bool bRight);
		String& operator =(const short sRight);
		String& operator =(const unsigned short usRight);
		String& operator =(const long lRight);
		String& operator =(const unsigned long ulRight);
		String operator +(const char *szRight);
		String operator +(const wchar_t *szRight);
		String operator +(const String &strRight);
		String operator +(const char cRight);
		String operator +(const wchar_t cRight);
		String operator +(const int iRight);
		String operator +(const unsigned int uiRight);
		String operator +(const float fRight);
		String operator +(const double dRight);
		String operator +(const bool bRight);
		String operator +(const short sRight);
		String operator +(const unsigned short usRight);
		String operator +(const long lRight);
		String operator +(const unsigned long ulRight);
		String& operator +=(const char *szRight);
		String& operator +=(const wchar_t *szRight);
		String& operator +=(const String &strRight);
		String& operator +=(const char cRight);
		String& operator +=(const wchar_t cRight);
		String& operator +=(const int iRight);
		String& operator +=(const unsigned int uiRight);
		String& operator +=(const float fRight);
		String& operator +=(const double dRight);
		String& operator +=(const bool bRight);
		String& operator +=(const short sRight);
		String& operator +=(const unsigned short usRight);
		String& operator +=(const long lRight);
		String& operator +=(const unsigned long ulRight);
		bool operator ==(const char *szRight) const;
		bool operator ==(const wchar_t *szRight) const;
		bool operator ==(const String &strRight) const;
		bool operator !=(const char *szRight) const;
		bool operator !=(const wchar_t *szRight) const;
		bool operator !=(const String &strRight) const;
		bool operator >=(const char *szRight) const;
		bool operator >=(const wchar_t *szRight) const;
		bool operator >=(const String &strRight) const;
		bool operator >(const char *szRight) const;
		bool operator >(const wchar_t *szRight) const;
		bool operator >(const String &strRight) const; 
		bool operator <=(const char *szRight) const;
		bool operator <=(const wchar_t *szRight) const;
		bool operator <=(const String &strRight) const;
		bool operator <(const char *szRight) const;
		bool operator <(const wchar_t *szRight) const;
		bool operator <(const String &strRight) const; 
		char operator [](int pos) const;
	public:
		// ���ַ�����ɸ�������
		const char * GetString() const;
		SharedPtr<wchar_t> GetWString() const;
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
		String Mid(int startPos,int count) const;
		String Mid(int startPos) const;
		// �����ַ���
		void Format(const char *format, ...);
		// �����ַ���λ��
		int Find(const char *szFind,int startPos,bool bRight) const;
		int Find(const wchar_t *szFind,int startPos,bool bRight) const;
		int Find(const String& strFind,int startPos,bool bRight) const;
		// �ֽ��ַ���
		std::vector<String> Split(char sz,int* pCount) const;
		std::vector<String> Split(wchar_t sz, int* pCount) const;
		//// ɾ������Ŀռ���
		//// ������������DLL�ͷſռ�����
		//inline void ClearSplitList(std::list<String>& list)
		//{
		//	list.clear();
		//}
		// �ַ�����д��
		String& MakeUpper();
		// �ַ���Сд��
		String& MakeLower();
		// set char using index
		String& SetCharAt(int pos,char value);
		String& SetCharAt(int pos,wchar_t value);
	public:
		// -------------- һЩ���ַ����йص�ȫ�ַ��� --------
		static void CutFilePath(char* fullname,char* filename=0,char* pathname=0);
		// --------------------------------------------------
	};

}

#endif