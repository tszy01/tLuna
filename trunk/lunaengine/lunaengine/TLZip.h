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
		static int def(TUByte* src,int srcSize, TUByte* dest,int* destSize,int level);
		// 解压缩
		static int inf(TUByte* src,int srcSize, TUByte* dest,int origSize);
		// 添加待压缩文件到内存
		static int AddCompressFile(TCHAR* filename);
		// 生成压缩文件
		static int CompressToFile(TCHAR* zipname);
		// 解压缩文件到内存
		static int DecompressToMem(TCHAR* zipname, TUByte** buf);
		// 分割解压缩得文件
		static int CopyMemToFile(TCHAR* dirname,TBOOL bBuildFile,MemFile** ppMemFile);
		// 清除数据
		static TVOID ClearBuffer();

		// ---------------------------------------
	protected:
		// -------- 以下为静态成员 ---------------
		// 缓冲区
		static TUByte* m_TempBuf;
		// 缓冲区大小
		static int m_TempSize;
		// 文件数量
		static int m_FileNum;
		// ---------------------------------------

		static const int ZIP_READ_FILE_LENGTH = 16384;
	};

}

#endif