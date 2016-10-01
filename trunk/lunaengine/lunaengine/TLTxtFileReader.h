#ifndef _TLTXTFILEREADER_H_
#define _TLTXTFILEREADER_H_

#include <stdio.h>
#include "TLCommonTypeDef.h"
namespace TLunaEngine{

	class TxtFileReader
	{
	public:
		TxtFileReader(TVOID);
		~TxtFileReader(TVOID);
	public:
		// 打开一个TMesh的文件
		// 返回：是否成功
		// in filename:文件名
		// out ppStream:返回的文件指针
		static TBOOL OpenTxtFile(const TCHAR* filename,FILE** ppStream);

		// 关闭一个TMesh的文件
		// 返回：无
		// in pStream:文件指针
		static TVOID CloseTxtFile(FILE* pStream);

		// 读取一行字符串
		// 返回：是否成功
		// out strResult:返回的结果字符串
		// in pStream:文件指针
		// in strCmp:比较的字符串，如果不需要比较，传0
		// out bEqual:是否和比较的字符段匹配，如果没有比较，传0
		// in nCount:一共可以装几个字符
		// out pReadCount:返回实际读取的字符
		static TBOOL ReadLineString(TCHAR* strResult,FILE* pStream,TCHAR* strCmp,TBOOL* bEqual, TU32 nCount, TU32* pReadCount);

		// 读取一行整数
		// 返回：是否成功
		// out aiResult:返回的整形数组
		// in pStream:文件指针
		// in nCount:需要读几个整形数据
		// in splice:用于分割N个数据的字符
		static TBOOL ReadLineInteger(TS32* aiResult,FILE* pStream, TU32 nCount,TCHAR splice);

		// 读取一行long long
		// 返回：是否成功
		// out alResult:返回的整形数组
		// in pStream:文件指针
		// in nCount:需要读几个整形数据
		// in splice:用于分割N个数据的字符
		static TBOOL ReadLineLongLong(TS64* allResult,FILE* pStream, TU32 nCount,TCHAR splice);

		// 读取一行无符号整数
		// 返回：是否成功
		// out auResult:返回的整形数组
		// in pStream:文件指针
		// in nCount:需要读几个整形数据
		// in splice:用于分割N个数据的字符
		static TBOOL ReadLineUInteger(TU32* auResult,FILE* pStream, TU32 nCount,TCHAR splice);

		// 读取一行无符号long long
		// 返回：是否成功
		// out aulResult:返回的整形数组
		// in pStream:文件指针
		// in nCount:需要读几个整形数据
		// in splice:用于分割N个数据的字符
		static TBOOL ReadLineULongLong(TU64* aullResult,FILE* pStream, TU32 nCount,TCHAR splice);

		// read short
		static TBOOL ReadLineShort(TS16* asResult,FILE* pStream, TU32 nCount,TCHAR splice);

		// read ushort
		static TBOOL ReadLineUShort(TU16* ausResult,FILE* pStream, TU32 nCount,TCHAR splice);

		// 读取一行浮点数
		// 返回：是否成功
		// out afResult:返回的浮点数组
		// in pStream:文件指针
		// in nCount:需要读几个浮点数数据
		// in splice:用于分割N个数据的字符
		static TBOOL ReadLineFloat(TF32* afResult,FILE* pStream, TU32 nCount,TCHAR splice);

		// 读取一行双精度
		// 返回：是否成功
		// out adResult:返回的浮点数组
		// in pStream:文件指针
		// in nCount:需要读几个浮点数数据
		// in splice:用于分割N个数据的字符
		static TBOOL ReadLineDouble(TF64* adResult,FILE* pStream, TU32 nCount,TCHAR splice);

		// 读取全部文本内容
		// 返回：是否成功
		// buffer,out,receivs read data
		// readByte,out,receivs read data size
		// szFile,in,file name to read
		// mode,in,read mode
		// note:函数本身会为buffer分配内存，请在外部释放
		static TBOOL ReadAllFile(const TCHAR* szFile,const TCHAR* mode,TVOID** buffer,TU64* pCount);
	};

}

#endif