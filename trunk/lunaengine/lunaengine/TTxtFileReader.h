#pragma once

#include <stdio.h>
namespace TLunaEngine{

	class TTxtFileReader
	{
	public:
		TTxtFileReader(void);
		~TTxtFileReader(void);
	public:
		// ��һ��TMesh���ļ�
		// ���أ��Ƿ�ɹ�
		// in filename:�ļ���
		// out ppStream:���ص��ļ�ָ��
		static bool OpenTxtFile(const char* filename,FILE** ppStream);

		// �ر�һ��TMesh���ļ�
		// ���أ���
		// in pStream:�ļ�ָ��
		static void CloseTxtFile(FILE* pStream);

		// ��ȡһ���ַ���
		// ���أ��Ƿ�ɹ�
		// out strResult:���صĽ���ַ���
		// in pStream:�ļ�ָ��
		// in strCmp:�Ƚϵ��ַ������������Ҫ�Ƚϣ���0
		// out bEqual:�Ƿ�ͱȽϵ��ַ���ƥ�䣬���û�бȽϣ���0
		// in nCount:һ������װ�����ַ�
		// out pReadCount:����ʵ�ʶ�ȡ���ַ�
		static bool ReadLineString(char* strResult,FILE* pStream,char* strCmp,bool* bEqual,int nCount,int* pReadCount);

		// ��ȡһ������
		// ���أ��Ƿ�ɹ�
		// out aiResult:���ص���������
		// in pStream:�ļ�ָ��
		// in nCount:��Ҫ��������������
		// in splice:���ڷָ�N�����ݵ��ַ�
		static bool ReadLineInteger(int* aiResult,FILE* pStream,int nCount,char splice);

		// ��ȡһ��long
		// ���أ��Ƿ�ɹ�
		// out alResult:���ص���������
		// in pStream:�ļ�ָ��
		// in nCount:��Ҫ��������������
		// in splice:���ڷָ�N�����ݵ��ַ�
		static bool ReadLineLong(long* alResult,FILE* pStream,int nCount,char splice);

		// ��ȡһ���޷�������
		// ���أ��Ƿ�ɹ�
		// out auResult:���ص���������
		// in pStream:�ļ�ָ��
		// in nCount:��Ҫ��������������
		// in splice:���ڷָ�N�����ݵ��ַ�
		static bool ReadLineUInteger(unsigned int* auResult,FILE* pStream,int nCount,char splice);

		// ��ȡһ���޷���long
		// ���أ��Ƿ�ɹ�
		// out aulResult:���ص���������
		// in pStream:�ļ�ָ��
		// in nCount:��Ҫ��������������
		// in splice:���ڷָ�N�����ݵ��ַ�
		static bool ReadLineULong(unsigned long* aulResult,FILE* pStream,int nCount,char splice);

		// read short
		static bool ReadLineShort(short* asResult,FILE* pStream,int nCount,char splice);

		// read ushort
		static bool ReadLineUShort(unsigned short* ausResult,FILE* pStream,int nCount,char splice);

		// ��ȡһ�и�����
		// ���أ��Ƿ�ɹ�
		// out afResult:���صĸ�������
		// in pStream:�ļ�ָ��
		// in nCount:��Ҫ����������������
		// in splice:���ڷָ�N�����ݵ��ַ�
		static bool ReadLineFloat(float* afResult,FILE* pStream,int nCount,char splice);

		// ��ȡһ��˫����
		// ���أ��Ƿ�ɹ�
		// out adResult:���صĸ�������
		// in pStream:�ļ�ָ��
		// in nCount:��Ҫ����������������
		// in splice:���ڷָ�N�����ݵ��ַ�
		static bool ReadLineDouble(double* adResult,FILE* pStream,int nCount,char splice);

		// ��ȡȫ���ı�����
		// ���أ��Ƿ�ɹ�
		// buffer,out,receivs read data
		// readByte,out,receivs read data size
		// szFile,in,file name to read
		// mode,in,read mode
		// note:���������Ϊbuffer�����ڴ棬�����ⲿ�ͷ�
		static bool ReadAllFile(const char* szFile,const char* mode,void** buffer,size_t* readByte);
	};

}