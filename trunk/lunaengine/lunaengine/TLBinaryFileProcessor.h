#ifndef _TLBINARYFILEPROCESSOR_H_
#define _TLBINARYFILEPROCESSOR_H_

#include <stdio.h>
#include "TLCommonTypeDef.h"
namespace TLunaEngine{
	class BinaryFileProcessor
	{
	public:
		// Open File
		static TBOOL openFile(const TCHAR* file,FILE** ppStream,const TCHAR* mode);
		// Close File
		static TVOID closeFile(FILE* pStream);
		// write byte
		static TBOOL writeBytes(const TVOID* buffer,FILE* pStream,TU64 nCount);
		// write boolean
		static TBOOL writeBoolean(const TBOOL* abWrite,FILE* pStream, TU32 nCount);
		// write integer
		static TBOOL writeInteger(const TS32* aiWrite,FILE* pStream, TU32 nCount);
		// write uinteger
		static TBOOL writeUInteger(const TU32* auWrite,FILE* pStream, TU32 nCount);
		// write short
		static TBOOL writeShort(const TS16* asWrite,FILE* pStream, TU32 nCount);
		// write ushort
		static TBOOL writeUShort(const TU16* ausWrite,FILE* pStream, TU32 nCount);
		// write long long
		static TBOOL writeLongLong(const TS64* allWrite,FILE* pStream, TU32 nCount);
		// write ulonglong
		static TBOOL writeULongLong(const TU64* aullWrite,FILE* pStream, TU32 nCount);
		// write float
		static TBOOL writeFloat(const TF32* afWrite,FILE* pStream, TU32 nCount);
		// write double
		static TBOOL writeDouble(const TF64* adWrite,FILE* pStream, TU32 nCount);
		// write string
		static TBOOL writeString(const TCHAR* strWrite,FILE* pStream, TU32 nCount);
		// read byte
		static TBOOL readBytes(TVOID* buffer,FILE* pStream, TU64 nCount);
		// read boolean
		static TBOOL readBoolean(TBOOL* abResult,FILE* pStream, TU32 nCount);
		// read integer
		static TBOOL readInteger(TS32* aiResult,FILE* pStream, TU32 nCount);
		// read uinteger
		static TBOOL readUInteger(TU32* auResult,FILE* pStream, TU32 nCount);
		// read short
		static TBOOL readShort(TS16* asResult,FILE* pStream, TU32 nCount);
		// read ushort
		static TBOOL readUShort(TU16* ausResult,FILE* pStream, TU32 nCount);
		// read longlong
		static TBOOL readLongLong(TS64* allResult,FILE* pStream, TU32 nCount);
		// read ulonglong
		static TBOOL readULongLong(TU64* aullResult,FILE* pStream, TU32 nCount);
		// read float
		static TBOOL readFloat(TF32* afResult,FILE* pStream, TU32 nCount);
		// read double
		static TBOOL readDouble(TF64* adResult,FILE* pStream, TU32 nCount);
		// read string
		static TBOOL readString(TCHAR* strResult,FILE* pStream,const TCHAR* strCmp,TBOOL* bEqual, TU32 nCount, TU32* pReadCount);
	};
}

#endif