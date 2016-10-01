#ifndef _TLSTRING_H_
#define _TLSTRING_H_

#include "TLSharedPtr.h"
#include "TLList.h"
namespace TLunaEngine{

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
		String(const TWCHAR *szContent);
		String(const String &strContent);
		String(const TCHAR cContent);
		String(const TWCHAR cContent);
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
		String& operator =(const TWCHAR *szRight);
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
		String operator +(const TWCHAR *szRight);
		String operator +(const String &strRight);
		String operator +(const TCHAR cRight);
		String operator +(const TWCHAR cRight);
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
		String& operator +=(const TWCHAR *szRight);
		String& operator +=(const String &strRight);
		String& operator +=(const TCHAR cRight);
		String& operator +=(const TWCHAR cRight);
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
		TCHAR operator [](TU32 pos) const;
	public:
		// 将字符串变成各种类型
		const TCHAR*  GetString() const;
		SharedPtr<TWCHAR> GetWString() const;
		TS32 GetStringAsInt() const;
		TF32 GetStringAsFloat() const;
		TU32 GetStringAsUInt() const;
		TS64 GetStringAsLongLong() const;
		TU64 GetStringAsULongLong() const;
		TF64 GetStringAsDouble() const;
		TBOOL GetStringAsBool() const;
		TS16 GetStringAsShort() const;
		TU16 GetStringAsUShort() const;
		// 得到字符串长度
		TU32 GetLength() const{return m_nLength;}
		// 隔断字符串
		String Mid(TU32 startPos, TU32 count) const;
		String Mid(TU32 startPos) const;
		// 生成字符串
		TVOID Format(const TCHAR* format, ...);
		// 查找字符串位置
		TS32 Find(const TCHAR* szFind, TU32 startPos,TBOOL bRight) const;
		TS32 Find(const TWCHAR *szFind, TU32 startPos,TBOOL bRight) const;
		TS32 Find(const String& strFind, TU32 startPos,TBOOL bRight) const;
		// 分解字符串
		List<String> Split(TCHAR sz, TU32* pCount) const;
		List<String> Split(TWCHAR sz, TU32* pCount) const;
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
		String& SetCharAt(TU32 pos,TWCHAR value);
	public:
		// -------------- 一些和字符串有关的全局方法 --------
		static TVOID CutFilePath(TCHAR* fullname,TCHAR* filename=0,TCHAR* pathname=0);
		// --------------------------------------------------
	};

}

#endif