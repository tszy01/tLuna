#ifndef _TLCOMMONTYPEDEF_H_
#define _TLCOMMONTYPEDEF_H_

namespace TLunaEngine{

#ifdef _WIN64
	typedef float TF32;
	typedef double TF64;
	typedef __int32 TS32;
	typedef unsigned __int32 TU32;
	typedef __int64 TS64;
	typedef unsigned __int64 TU64;
	typedef __int16 TS16;
	typedef unsigned __int16 TU16;
	typedef char TByte;
	typedef unsigned char TUByte;
	typedef void TVOID;
	typedef __int64 TLP;
	typedef TUByte TBOOL;
	const TBOOL TTRUE = 1;
	const TBOOL TFALSE = 0;
	const TLP TNULL = 0;
#elif _WIN32
	typedef float TF32;
	typedef double TF64;
	typedef int TS32;
	typedef unsigned int TU32;
	typedef __int64 TS64;
	typedef unsigned __int64 TU64;
	typedef short TS16;
	typedef unsigned short TU16;
	typedef char TByte;
	typedef unsigned char TUByte;
	typedef void TVOID;
	typedef __int32 TLP;
	typedef TUByte TBOOL;
	const TBOOL TTRUE = 1;
	const TBOOL TFALSE = 0;
	const TLP TNULL = 0;
#else
	typedef float TF32;
	typedef double TF64;
	typedef int TS32;
	typedef unsigned int TU32;
	typedef __int64 TS64;
	typedef unsigned __int64 TU64;
	typedef short TS16;
	typedef unsigned short TU16;
	typedef char TByte;
	typedef unsigned char TUByte;
	typedef TS32 TBOOL;
	const TBOOL TTRUE = 1;
	const TBOOL TFALSE = 0;
	const TS32 TNULL = 0;
#endif
}

#endif