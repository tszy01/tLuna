#pragma once

#include <stdio.h>
namespace TLunaEngine{
	class TBinaryFileProcessor
	{
	public:
		// Open File
		static bool openFile(const char* file,FILE** ppStream,const char* mode);
		// Close File
		static void closeFile(FILE* pStream);
		// write byte
		static bool writeBytes(const void* buffer,FILE* pStream,int sizeWrite);
		// write boolean
		static bool writeBoolean(const bool* abWrite,FILE* pStream,int nCount);
		// write integer
		static bool writeInteger(const int* aiWrite,FILE* pStream,int nCount);
		// write uinteger
		static bool writeUInteger(const unsigned int* auWrite,FILE* pStream,int nCount);
		// write short
		static bool writeShort(const short* asWrite,FILE* pStream,int nCount);
		// write ushort
		static bool writeUShort(const unsigned short* ausWrite,FILE* pStream,int nCount);
		// write long
		static bool writeLong(const long* alWrite,FILE* pStream,int nCount);
		// write ulong
		static bool writeULong(const unsigned long* aulWrite,FILE* pStream,int nCount);
		// write float
		static bool writeFloat(const float* afWrite,FILE* pStream,int nCount);
		// write double
		static bool writeDouble(const double* adWrite,FILE* pStream,int nCount);
		// write string
		static bool writeString(const char* strWrite,FILE* pStream,size_t count);
		// read byte
		static bool readBytes(void* buffer,FILE* pStream,int sizeRead);
		// read boolean
		static bool readBoolean(bool* abResult,FILE* pStream,int nCount);
		// read integer
		static bool readInteger(int* aiResult,FILE* pStream,int nCount);
		// read uinteger
		static bool readUInteger(unsigned int* auResult,FILE* pStream,int nCount);
		// read short
		static bool readShort(short* asResult,FILE* pStream,int nCount);
		// read ushort
		static bool readUShort(unsigned short* ausResult,FILE* pStream,int nCount);
		// read long
		static bool readLong(long* alResult,FILE* pStream,int nCount);
		// read ulong
		static bool readULong(unsigned long* aulResult,FILE* pStream,int nCount);
		// read float
		static bool readFloat(float* afResult,FILE* pStream,int nCount);
		// read double
		static bool readDouble(double* adResult,FILE* pStream,int nCount);
		// read string
		static bool readString(char* strResult,FILE* pStream,const char* strCmp,bool* bEqual,int nCount,int* pReadCount);
	};
}