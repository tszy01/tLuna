#ifndef _TLSTRING_H_
#define _TLSTRING_H_

#include "TLSharedPtr.h"
#include <list>
#include <vector>
namespace TLunaEngine{

	class String
	{
	protected:
		//TCHAR* m_szContent;
		SharedPtr<TCHAR> m_szPtr;
		int m_nLength;
	public:
		String(TVOID);
		~String(TVOID);
	public:
		// 重载函数
		String(const TCHAR* szContent);
		String(const TWCHAR *szContent);
		String(const String &strContent);
		String(const TCHAR cContent);
		String(const TWCHAR cContent);
		String(const int iRight);
		String(const unsigned int uiRight);
		String(const float fRight);
		String(const double dRight);
		String(const TBOOL bRight);
		String(const short sRight);
		String(const unsigned short usRight);
		String(const long lRight);
		String(const unsigned long ulRight);
		String& operator =(const TCHAR* szRight);
		String& operator =(const TWCHAR *szRight);
		String& operator =(const String &strRight);
		String& operator =(const TCHAR cRight);
		String& operator =(const int iRight);
		String& operator =(const unsigned int uiRight);
		String& operator =(const float fRight);
		String& operator =(const double dRight);
		String& operator =(const TBOOL bRight);
		String& operator =(const short sRight);
		String& operator =(const unsigned short usRight);
		String& operator =(const long lRight);
		String& operator =(const unsigned long ulRight);
		String operator +(const TCHAR* szRight);
		String operator +(const TWCHAR *szRight);
		String operator +(const String &strRight);
		String operator +(const TCHAR cRight);
		String operator +(const TWCHAR cRight);
		String operator +(const int iRight);
		String operator +(const unsigned int uiRight);
		String operator +(const float fRight);
		String operator +(const double dRight);
		String operator +(const TBOOL bRight);
		String operator +(const short sRight);
		String operator +(const unsigned short usRight);
		String operator +(const long lRight);
		String operator +(const unsigned long ulRight);
		String& operator +=(const TCHAR* szRight);
		String& operator +=(const TWCHAR *szRight);
		String& operator +=(const String &strRight);
		String& operator +=(const TCHAR cRight);
		String& operator +=(const TWCHAR cRight);
		String& operator +=(const int iRight);
		String& operator +=(const unsigned int uiRight);
		String& operator +=(const float fRight);
		String& operator +=(const double dRight);
		String& operator +=(const TBOOL bRight);
		String& operator +=(const short sRight);
		String& operator +=(const unsigned short usRight);
		String& operator +=(const long lRight);
		String& operator +=(const unsigned long ulRight);
		TBOOL operator ==(const TCHAR* szRight) const;
		TBOOL operator ==(const TWCHAR *szRight) const;
		TBOOL operator ==(const String &strRight) const;
		TBOOL operator !=(const TCHAR* szRight) const;
		TBOOL operator !=(const TWCHAR *szRight) const;
		TBOOL operator !=(const String &strRight) const;
		TBOOL operator >=(const TCHAR* szRight) const;
		TBOOL operator >=(const TWCHAR *szRight) const;
		TBOOL operator >=(const String &strRight) const;
		TBOOL operator >(const TCHAR* szRight) const;
		TBOOL operator >(const TWCHAR *szRight) const;
		TBOOL operator >(const String &strRight) const; 
		TBOOL operator <=(const TCHAR* szRight) const;
		TBOOL operator <=(const TWCHAR *szRight) const;
		TBOOL operator <=(const String &strRight) const;
		TBOOL operator <(const TCHAR* szRight) const;
		TBOOL operator <(const TWCHAR *szRight) const;
		TBOOL operator <(const String &strRight) const; 
		TCHAR operator [](int pos) const;
	public:
		// 将字符串变成各种类型
		const TCHAR*  GetString() const;
		SharedPtr<TWCHAR> GetWString() const;
		int GetStringAsInt() const;
		float GetStringAsFloat() const;
		unsigned int GetStringAsUInt() const;
		long GetStringAsLong() const;
		unsigned long GetStringAsULong() const;
		double GetStringAsDouble() const;
		TBOOL GetStringAsBool() const;
		// 得到字符串长度
		int GetLength() const{return m_nLength;}
		// 隔断字符串
		String Mid(int startPos,int count) const;
		String Mid(int startPos) const;
		// 生成字符串
		TVOID Format(const TCHAR* format, ...);
		// 查找字符串位置
		int Find(const TCHAR* szFind,int startPos,TBOOL bRight) const;
		int Find(const TWCHAR *szFind,int startPos,TBOOL bRight) const;
		int Find(const String& strFind,int startPos,TBOOL bRight) const;
		// 分解字符串
		std::vector<String> Split(TCHAR sz,int* pCount) const;
		std::vector<String> Split(TWCHAR sz, int* pCount) const;
		//// 删除分配的空间用
		//// 这个函数解决跨DLL释放空间问题
		//inline TVOID ClearSplitList(std::list<String>& list)
		//{
		//	list.clear();
		//}
		// 字符串大写化
		String& MakeUpper();
		// 字符串小写化
		String& MakeLower();
		// set TCHAR using index
		String& SetCharAt(int pos, TCHAR value);
		String& SetCharAt(int pos,TWCHAR value);
	public:
		// -------------- 一些和字符串有关的全局方法 --------
		static TVOID CutFilePath(TCHAR* fullname,TCHAR* filename=0,TCHAR* pathname=0);
		// --------------------------------------------------
	};

}

#endif