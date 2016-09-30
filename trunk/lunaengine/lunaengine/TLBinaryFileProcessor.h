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
		static TBOOL writeBytes(const TVOID* buffer,FILE* pStream,int sizeWrite);
		// write boolean
		static TBOOL writeBoolean(const TBOOL* abWrite,FILE* pStream,int nCount);
		// write integer
		static TBOOL writeInteger(const int* aiWrite,FILE* pStream,int nCount);
		// write uinteger
		static TBOOL writeUInteger(const unsigned int* auWrite,FILE* pStream,int nCount);
		// write short
		static TBOOL writeShort(const short* asWrite,FILE* pStream,int nCount);
		// write ushort
		static TBOOL writeUShort(const unsigned short* ausWrite,FILE* pStream,int nCount);
		// write long
		static TBOOL writeLong(const long* alWrite,FILE* pStream,int nCount);
		// write ulong
		static TBOOL writeULong(const unsigned long* aulWrite,FILE* pStream,int nCount);
		// write float
		static TBOOL writeFloat(const float* afWrite,FILE* pStream,int nCount);
		// write double
		static TBOOL writeDouble(const double* adWrite,FILE* pStream,int nCount);
		// write string
		static TBOOL writeString(const TCHAR* strWrite,FILE* pStream,size_t count);
		// read byte
		static TBOOL readBytes(TVOID* buffer,FILE* pStream,int sizeRead);
		// read boolean
		static TBOOL readBoolean(TBOOL* abResult,FILE* pStream,int nCount);
		// read integer
		static TBOOL readInteger(int* aiResult,FILE* pStream,int nCount);
		// read uinteger
		static TBOOL readUInteger(unsigned int* auResult,FILE* pStream,int nCount);
		// read short
		static TBOOL readShort(short* asResult,FILE* pStream,int nCount);
		// read ushort
		static TBOOL readUShort(unsigned short* ausResult,FILE* pStream,int nCount);
		// read long
		static TBOOL readLong(long* alResult,FILE* pStream,int nCount);
		// read ulong
		static TBOOL readULong(unsigned long* aulResult,FILE* pStream,int nCount);
		// read float
		static TBOOL readFloat(float* afResult,FILE* pStream,int nCount);
		// read double
		static TBOOL readDouble(double* adResult,FILE* pStream,int nCount);
		// read string
		static TBOOL readString(TCHAR* strResult,FILE* pStream,const TCHAR* strCmp,TBOOL* bEqual,int nCount,int* pReadCount);
	};
}

#endif