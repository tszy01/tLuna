#ifndef _TLSTRING_H_
#define _TLSTRING_H_

#include "TLSharedPtr.h"
#include "TLList.h"
namespace TLunaEngine{

	enum STRING_ENCODING_TYPE
	{
		STRING_ENCODING_TYPE_ANSI = 1,
		STRING_ENCODING_TYPE_UTF8 = 2,
	};

	class String
	{
	protected:
		//TCHAR* m_szContent;
		SharedPtr<TCHAR> m_szPtr;
		TU32 m_nLength;
	public:
		String(TVOID);
		~String(TVOID);
	public:
		// 重载函数
		String(const TCHAR* szContent);
		String(const String &strContent);
		String(const TCHAR cContent);
		String(const TS32 iRight);
		String(const TU32 uiRight);
		String(const TF32 fRight);
		String(const TF64 dRight);
		String(const TBOOL bRight);
		String(const TS16 sRight);
		String(const TU16 usRight);
		String(const TS64 llRight);
		String(const TU64 ullRight);
		String& operator =(const TCHAR* szRight);
		String& operator =(const String &strRight);
		String& operator =(const TCHAR cRight);
		String& operator =(const TS32 iRight);
		String& operator =(const TU32 uiRight);
		String& operator =(const TF32 fRight);
		String& operator =(const TF64 dRight);
		String& operator =(const TBOOL bRight);
		String& operator =(const TS16 sRight);
		String& operator =(const TU16 usRight);
		String& operator =(const TS64 llRight);
		String& operator =(const TU64 ullRight);
		String operator +(const TCHAR* szRight);
		String operator +(const String &strRight);
		String operator +(const TCHAR cRight);
		String operator +(const TS32 iRight);
		String operator +(const TU32 uiRight);
		String operator +(const TF32 fRight);
		String operator +(const TF64 dRight);
		String operator +(const TBOOL bRight);
		String operator +(const TS16 sRight);
		String operator +(const TU16 usRight);
		String operator +(const TS64 llRight);
		String operator +(const TU64 ullRight);
		String& operator +=(const TCHAR* szRight);
		String& operator +=(const String &strRight);
		String& operator +=(const TCHAR cRight);
		String& operator +=(const TS32 iRight);
		String& operator +=(const TU32 uiRight);
		String& operator +=(const TF32 fRight);
		String& operator +=(const TF64 dRight);
		String& operator +=(const TBOOL bRight);
		String& operator +=(const TS16 sRight);
		String& operator +=(const TU16 usRight);
		String& operator +=(const TS64 llRight);
		String& operator +=(const TU64 ullRight);
		TBOOL operator ==(const TCHAR* szRight) const;
		TBOOL operator ==(const String &strRight) const;
		TBOOL operator !=(const TCHAR* szRight) const;
		TBOOL operator !=(const String &strRight) const;
		TBOOL operator >=(const TCHAR* szRight) const;
		TBOOL operator >=(const String &strRight) const;
		TBOOL operator >(const TCHAR* szRight) const;
		TBOOL operator >(const String &strRight) const; 
		TBOOL operator <=(const TCHAR* szRight) const;
		TBOOL operator <=(const String &strRight) const;
		TBOOL operator <(const TCHAR* szRight) const;
		TBOOL operator <(const String &strRight) const; 
		TCHAR operator [](TU32 pos) const;
	public:
		// 将字符串变成各种类型
		const TCHAR*  GetString() const;
		SharedPtr<TWCHAR> ToWString() const;
		TS32 ToInt() const;
		TF32 ToFloat() const;
		TU32 ToUInt() const;
		TS64 ToLongLong() const;
		TU64 ToULongLong() const;
		TF64 ToDouble() const;
		TBOOL ToBool() const;
		TS16 ToShort() const;
		TU16 ToUShort() const;
		// 得到字符串长度
		TU32 GetLength() const{return m_nLength;}
		// 隔断字符串
		String Mid(TU32 startPos, TU32 count) const;
		String Mid(TU32 startPos) const;
		// 生成字符串
		TVOID Format(const TCHAR* format, ...);
		// 查找字符串位置
		TS32 Find(const TCHAR* szFind, TU32 startPos,TBOOL bRight) const;
		TS32 Find(const String& strFind, TU32 startPos,TBOOL bRight) const;
		// 分解字符串
		List<String> Split(TCHAR sz, TU32* pCount) const;
		//// 删除分配的空间用
		//// 这个函数解决跨DLL释放空间问题
		//inline TVOID ClearSplitList(List<String>& list)
		//{
		//	list.clear();
		//}
		// 字符串大写化
		String& MakeUpper();
		// 字符串小写化
		String& MakeLower();
		// set TCHAR using index
		String& SetCharAt(TU32 pos, TCHAR value);
		//// convert to utf8
		//String& ConvertToUTF8();
		//// convert to ansi
		//String& ConvertToANSI();
	public:
		// -------------- 一些和字符串有关的全局方法 --------
		static TVOID CutFilePath(TCHAR* fullname,TCHAR* filename=0,TCHAR* pathname=0);
		// --------------------------------------------------
	};

	class WString
	{
	protected:
		//TCHAR* m_szContent;
		SharedPtr<TWCHAR> m_szPtr;
		TU32 m_nLength;
	public:
		WString(TVOID);
		~WString(TVOID);
	public:
		// 重载函数
		WString(const TWCHAR *szContent);
		WString(const WString &strContent);
		WString(const TWCHAR cContent);
		WString(const TS32 iRight);
		WString(const TU32 uiRight);
		WString(const TF32 fRight);
		WString(const TF64 dRight);
		WString(const TBOOL bRight);
		WString(const TS16 sRight);
		WString(const TU16 usRight);
		WString(const TS64 llRight);
		WString(const TU64 ullRight);
		WString& operator =(const TWCHAR *szRight);
		WString& operator =(const WString &strRight);
		WString& operator =(const TWCHAR cRight);
		WString& operator =(const TS32 iRight);
		WString& operator =(const TU32 uiRight);
		WString& operator =(const TF32 fRight);
		WString& operator =(const TF64 dRight);
		WString& operator =(const TBOOL bRight);
		WString& operator =(const TS16 sRight);
		WString& operator =(const TU16 usRight);
		WString& operator =(const TS64 llRight);
		WString& operator =(const TU64 ullRight);
		WString operator +(const TWCHAR *szRight);
		WString operator +(const WString &strRight);
		WString operator +(const TWCHAR cRight);
		WString operator +(const TS32 iRight);
		WString operator +(const TU32 uiRight);
		WString operator +(const TF32 fRight);
		WString operator +(const TF64 dRight);
		WString operator +(const TBOOL bRight);
		WString operator +(const TS16 sRight);
		WString operator +(const TU16 usRight);
		WString operator +(const TS64 llRight);
		WString operator +(const TU64 ullRight);
		WString& operator +=(const TWCHAR *szRight);
		WString& operator +=(const WString &strRight);
		WString& operator +=(const TWCHAR cRight);
		WString& operator +=(const TS32 iRight);
		WString& operator +=(const TU32 uiRight);
		WString& operator +=(const TF32 fRight);
		WString& operator +=(const TF64 dRight);
		WString& operator +=(const TBOOL bRight);
		WString& operator +=(const TS16 sRight);
		WString& operator +=(const TU16 usRight);
		WString& operator +=(const TS64 llRight);
		WString& operator +=(const TU64 ullRight);
		TBOOL operator ==(const TWCHAR *szRight) const;
		TBOOL operator ==(const WString &strRight) const;
		TBOOL operator !=(const TWCHAR *szRight) const;
		TBOOL operator !=(const WString &strRight) const;
		TBOOL operator >=(const TWCHAR *szRight) const;
		TBOOL operator >=(const WString &strRight) const;
		TBOOL operator >(const TWCHAR *szRight) const;
		TBOOL operator >(const WString &strRight) const;
		TBOOL operator <=(const TWCHAR *szRight) const;
		TBOOL operator <=(const WString &strRight) const;
		TBOOL operator <(const TWCHAR *szRight) const;
		TBOOL operator <(const WString &strRight) const;
		TWCHAR operator [](TU32 pos) const;
	public:
		// 将字符串变成各种类型
		String ToString() const;
		const WCHAR* GetWString() const;
		TS32 ToInt() const;
		TF32 ToFloat() const;
		TU32 ToUInt() const;
		TS64 ToLongLong() const;
		TU64 ToULongLong() const;
		TF64 ToDouble() const;
		TBOOL ToBool() const;
		TS16 ToShort() const;
		TU16 ToUShort() const;
		// 得到字符串长度
		TU32 GetLength() const { return m_nLength; }
		// 隔断字符串
		WString Mid(TU32 startPos, TU32 count) const;
		WString Mid(TU32 startPos) const;
		// 生成字符串
		TVOID Format(const TWCHAR* format, ...);
		// 查找字符串位置
		TS32 Find(const TWCHAR *szFind, TU32 startPos, TBOOL bRight) const;
		TS32 Find(const WString& strFind, TU32 startPos, TBOOL bRight) const;
		// 分解字符串
		List<WString> Split(TWCHAR sz, TU32* pCount) const;
		// set TCHAR using index
		WString& SetCharAt(TU32 pos, TWCHAR value);
		//// convert to utf8
		//WString& ConvertToUTF8();
		//// convert to ansi
		//WString& ConvertToANSI();
	public:
		// -------------- 一些和字符串有关的全局方法 --------
		static TVOID CutFilePath(TWCHAR* fullname, TWCHAR* filename = 0, TWCHAR* pathname = 0);
		// --------------------------------------------------
	};

}

#endif