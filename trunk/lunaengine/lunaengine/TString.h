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
		// 重载函数
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
		// 将字符串变成各种类型
		const char * GetString() const;
		TSharedPtr<wchar_t> GetWString() const;
		int GetStringAsInt() const;
		float GetStringAsFloat() const;
		unsigned int GetStringAsUInt() const;
		long GetStringAsLong() const;
		unsigned long GetStringAsULong() const;
		double GetStringAsDouble() const;
		bool GetStringAsBool() const;
		// 得到字符串长度
		int GetLength() const{return m_nLength;}
		// 隔断字符串
		TString Mid(int startPos,int count) const;
		TString Mid(int startPos) const;
		// 生成字符串
		void Format(const char *format, ...);
		// 查找字符串位置
		int Find(const char *szFind,int startPos,bool bRight) const;
		int Find(const wchar_t *szFind,int startPos,bool bRight) const;
		int Find(const TString& strFind,int startPos,bool bRight) const;
		// 分解字符串
		std::list<TString> Split(char sz,int* pCount) const;
		std::list<TString> Split(wchar_t sz,int* pCount) const;
		//// 删除分配的空间用
		//// 这个函数解决跨DLL释放空间问题
		//inline void ClearSplitList(std::list<TString>& list)
		//{
		//	list.clear();
		//}
		// 字符串大写化
		TString& MakeUpper();
		// 字符串小写化
		TString& MakeLower();
		// set char using index
		TString& SetCharAt(int pos,char value);
		TString& SetCharAt(int pos,wchar_t value);
	public:
		// -------------- 一些和字符串有关的全局方法 --------
		static void CutFilePath(char* fullname,char* filename=0,char* pathname=0);
		// --------------------------------------------------
	};

}