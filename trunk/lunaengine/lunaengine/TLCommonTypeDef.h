#ifndef _TLCOMMONTYPEDEF_H_
#define _TLCOMMONTYPEDEF_H_

namespace TLunaEngine{

#ifdef _WIN64
	typedef float TF32;
	typedef double TF64;
	typedef int TS32;
	typedef unsigned int TU32;
	typedef long long TS64;
	typedef unsigned long long TU64;
	typedef short TS16;
	typedef unsigned short TU16;
	typedef char TByte;
	typedef unsigned char TUByte;
	typedef wchar_t TWCHAR;
	typedef char TCHAR;
	typedef void TVOID;
	typedef unsigned long long TLP;
	typedef size_t TSIZE;
	typedef TUByte TBOOL;
	const TBOOL TTRUE = 1;
	const TBOOL TFALSE = 0;
	const TLP TNULL = 0;
#elif _WIN32
	typedef float TF32;
	typedef double TF64;
	typedef int TS32;
	typedef unsigned int TU32;
	typedef long long TS64;
	typedef unsigned long long TU64;
	typedef short TS16;
	typedef unsigned short TU16;
	typedef char TByte;
	typedef unsigned char TUByte;
	typedef wchar_t TWCHAR;
	typedef char TCHAR;
	typedef void TVOID;
	typedef unsigned int TLP;
	typedef size_t TSIZE;
	typedef TUByte TBOOL;
	const TBOOL TTRUE = 1;
	const TBOOL TFALSE = 0;
	const TLP TNULL = 0;
#else
	
#endif
}

#endif