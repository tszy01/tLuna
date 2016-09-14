#pragma once

#include <stdio.h>
namespace TLunaEngine{

	class TTxtFileReader
	{
	public:
		TTxtFileReader(void);
		~TTxtFileReader(void);
	public:
		// 打开一个TMesh的文件
		// 返回：是否成功
		// in filename:文件名
		// out ppStream:返回的文件指针
		static bool OpenTxtFile(const char* filename,FILE** ppStream);

		// 关闭一个TMesh的文件
		// 返回：无
		// in pStream:文件指针
		static void CloseTxtFile(FILE* pStream);

		// 读取一行字符串
		// 返回：是否成功
		// out strResult:返回的结果字符串
		// in pStream:文件指针
		// in strCmp:比较的字符串，如果不需要比较，传0
		// out bEqual:是否和比较的字符段匹配，如果没有比较，传0
		// in nCount:一共可以装几个字符
		// out pReadCount:返回实际读取的字符
		static bool ReadLineString(char* strResult,FILE* pStream,char* strCmp,bool* bEqual,int nCount,int* pReadCount);

		// 读取一行整数
		// 返回：是否成功
		// out aiResult:返回的整形数组
		// in pStream:文件指针
		// in nCount:需要读几个整形数据
		// in splice:用于分割N个数据的字符
		static bool ReadLineInteger(int* aiResult,FILE* pStream,int nCount,char splice);

		// 读取一行long
		// 返回：是否成功
		// out alResult:返回的整形数组
		// in pStream:文件指针
		// in nCount:需要读几个整形数据
		// in splice:用于分割N个数据的字符
		static bool ReadLineLong(long* alResult,FILE* pStream,int nCount,char splice);

		// 读取一行无符号整数
		// 返回：是否成功
		// out auResult:返回的整形数组
		// in pStream:文件指针
		// in nCount:需要读几个整形数据
		// in splice:用于分割N个数据的字符
		static bool ReadLineUInteger(unsigned int* auResult,FILE* pStream,int nCount,char splice);

		// 读取一行无符号long
		// 返回：是否成功
		// out aulResult:返回的整形数组
		// in pStream:文件指针
		// in nCount:需要读几个整形数据
		// in splice:用于分割N个数据的字符
		static bool ReadLineULong(unsigned long* aulResult,FILE* pStream,int nCount,char splice);

		// read short
		static bool ReadLineShort(short* asResult,FILE* pStream,int nCount,char splice);

		// read ushort
		static bool ReadLineUShort(unsigned short* ausResult,FILE* pStream,int nCount,char splice);

		// 读取一行浮点数
		// 返回：是否成功
		// out afResult:返回的浮点数组
		// in pStream:文件指针
		// in nCount:需要读几个浮点数数据
		// in splice:用于分割N个数据的字符
		static bool ReadLineFloat(float* afResult,FILE* pStream,int nCount,char splice);

		// 读取一行双精度
		// 返回：是否成功
		// out adResult:返回的浮点数组
		// in pStream:文件指针
		// in nCount:需要读几个浮点数数据
		// in splice:用于分割N个数据的字符
		static bool ReadLineDouble(double* adResult,FILE* pStream,int nCount,char splice);

		// 读取全部文本内容
		// 返回：是否成功
		// buffer,out,receivs read data
		// readByte,out,receivs read data size
		// szFile,in,file name to read
		// mode,in,read mode
		// note:函数本身会为buffer分配内存，请在外部释放
		static bool ReadAllFile(const char* szFile,const char* mode,void** buffer,size_t* readByte);
	};

}