#ifndef _TLTXTFILEWRITER_H_
#define _TLTXTFILEWRITER_H_

#include <stdio.h>
#include "TLCommonTypeDef.h"
namespace TLunaEngine{

	class TxtFileWriter
	{
	public:
		TxtFileWriter(TVOID);
		~TxtFileWriter(TVOID);
	public:
		// 打开一个Txt的文件
		// 返回：是否成功
		// in filename:文件名
		// out ppStream:返回的文件指针
		static TBOOL OpenTxtFile(const TCHAR* filename,FILE** ppStream);

		// 关闭一个TMesh的文件
		// 返回：无
		// in pStream:文件指针
		static TVOID CloseTxtFile(FILE* pStream);

		// 写入一行字符串
		// 返回：是否成功
		// in strWrite:返回的结果字符串
		// in pStream:文件指针
		// in nCount:写入字符个数
		static TBOOL WriteLineString(const TCHAR* strWrite,FILE* pStream,TU32 nCount);

		// 写入一行整数
		// 返回：是否成功
		// in aiResult:整形数组
		// in pStream:文件指针
		// in nCount:需要读几个整形数据
		// in splice:用于分割N个数据的字符
		static TBOOL WriteLineInteger(const TS32* aiWrite,FILE* pStream, TU32 nCount,TCHAR splice);

		// 写入一行long long
		// 返回：是否成功
		// in alWrite:整形数组
		// in pStream:文件指针
		// in nCount:需要读几个整形数据
		// in splice:用于分割N个数据的字符
		static TBOOL WriteLineLongLong(const TS64* allWrite,FILE* pStream, TU32 nCount,TCHAR splice);

		// 写入一行无符号整数
		// 返回：是否成功
		// in auWrite:整形数组
		// in pStream:文件指针
		// in nCount:需要读几个整形数据
		// in splice:用于分割N个数据的字符
		static TBOOL WriteLineUInteger(const TU32* auWrite,FILE* pStream, TU32 nCount,TCHAR splice);

		// 写入一行无符号long long
		// 返回：是否成功
		// in aulWrite:整形数组
		// in pStream:文件指针
		// in nCount:需要读几个整形数据
		// in splice:用于分割N个数据的字符
		static TBOOL WriteLineULongLong(const TU64* aullWrite,FILE* pStream, TU32 nCount,TCHAR splice);

		// write short
		static TBOOL WriteLineShort(const TS16* asWrite,FILE* pStream, TU32 nCount,TCHAR splice);

		// write ushort
		static TBOOL WriteLineUShort(const TU16* ausWrite,FILE* pStream, TU32 nCount,TCHAR splice);

		// 写入一行浮点数
		// 返回：是否成功
		// in afWrite:浮点数组
		// in pStream:文件指针
		// in nCount:需要读几个浮点数数据
		// in splice:用于分割N个数据的字符
		static TBOOL WriteLineFloat(const TF32* afWrite,FILE* pStream, TU32 nCount,TCHAR splice);

		// 写入一行双精度
		// 返回：是否成功
		// in adWrite:浮点数组
		// in pStream:文件指针
		// in nCount:需要读几个浮点数数据
		// in splice:用于分割N个数据的字符
		static TBOOL WriteLineDouble(const TF64* adWrite,FILE* pStream, TU32 nCount,TCHAR splice);

		// 写入全部文本内容
		// 返回：是否成功
		// buffer,in,write data
		// writeByte,in,write size
		// szFile,in,file name to write
		// mode,in,write mode
		static TBOOL WriteAllFile(const TCHAR* szFile,const TCHAR* mode,const TVOID* buffer, TU64 nCount);
	};

}

#endif