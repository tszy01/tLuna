#ifndef _TLZIP_H_
#define _TLZIP_H_
#include "TLMemFile.h"


namespace TLunaEngine{

	class Zip
	{
	public:
		Zip(TVOID);
		~Zip(TVOID);
	public:
		// --------- 以下为静态方法 --------------

		// 压缩
		static TS32 def(TUByte* src, TU32 srcSize, TUByte* dest, TU32* destSize,TS32 level);
		// 解压缩
		static TS32 inf(TUByte* src, TU32 srcSize, TUByte* dest, TU32 origSize);
		// 添加待压缩文件到内存
		static TS32 AddCompressFile(TCHAR* filename);
		// 生成压缩文件
		static TS32 CompressToFile(TCHAR* zipname);
		// 解压缩文件到内存
		static TU64 DecompressToMem(TCHAR* zipname, TUByte** buf);
		// 分割解压缩得文件
		static TS32 CopyMemToFile(TCHAR* dirname,TBOOL bBuildFile,MemFile** ppMemFile);
		// 清除数据
		static TVOID ClearBuffer();

		// ---------------------------------------
	protected:
		// -------- 以下为静态成员 ---------------
		// 缓冲区
		static TUByte* m_TempBuf;
		// 缓冲区大小
		static TSIZE m_TempSize;
		// 文件数量
		static TU32 m_FileNum;
		// ---------------------------------------

		static const TU32 ZIP_READ_FILE_LENGTH = 16384;
	};

}

#endif