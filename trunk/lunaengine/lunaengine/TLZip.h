#ifndef _TLZIP_H_
#define _TLZIP_H_
#include "TLMemFile.h"


namespace TLunaEngine{

	class Zip
	{
	public:
		Zip(void);
		~Zip(void);
	public:
		// --------- 以下为静态方法 --------------

		// 压缩
		static int def(unsigned char* src,int srcSize,unsigned char* dest,int* destSize,int level);
		// 解压缩
		static int inf(unsigned char* src,int srcSize,unsigned char* dest,int origSize);
		// 添加待压缩文件到内存
		static int AddCompressFile(char* filename);
		// 生成压缩文件
		static int CompressToFile(char* zipname);
		// 解压缩文件到内存
		static int DecompressToMem(char* zipname,unsigned char** buf);
		// 分割解压缩得文件
		static int CopyMemToFile(char* dirname,bool bBuildFile,MemFile** ppMemFile);
		// 清除数据
		static void ClearBuffer();

		// ---------------------------------------
	protected:
		// -------- 以下为静态成员 ---------------
		// 缓冲区
		static unsigned char* m_TempBuf;
		// 缓冲区大小
		static int m_TempSize;
		// 文件数量
		static int m_FileNum;
		// ---------------------------------------

		static const int ZIP_READ_FILE_LENGTH = 16384;
	};

}

#endif