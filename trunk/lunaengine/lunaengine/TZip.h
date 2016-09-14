#pragma once
#include "TMemFile.h"


namespace TLunaEngine{

	class TZip
	{
	public:
		TZip(void);
		~TZip(void);
	public:
		// --------- ����Ϊ��̬���� --------------

		// ѹ��
		static int def(unsigned char* src,int srcSize,unsigned char* dest,int* destSize,int level);
		// ��ѹ��
		static int inf(unsigned char* src,int srcSize,unsigned char* dest,int origSize);
		// ��Ӵ�ѹ���ļ����ڴ�
		static int AddCompressFile(char* filename);
		// ����ѹ���ļ�
		static int CompressToFile(char* zipname);
		// ��ѹ���ļ����ڴ�
		static int DecompressToMem(char* zipname,unsigned char** buf);
		// �ָ��ѹ�����ļ�
		static int CopyMemToFile(char* dirname,bool bBuildFile,TMemFile** ppMemFile);
		// �������
		static void ClearBuffer();

		// ---------------------------------------
	protected:
		// -------- ����Ϊ��̬��Ա ---------------
		// ������
		static unsigned char* m_TempBuf;
		// ��������С
		static int m_TempSize;
		// �ļ�����
		static int m_FileNum;
		// ---------------------------------------

		static const int ZIP_READ_FILE_LENGTH = 16384;
	};

}