#pragma once

#include <stdio.h>
namespace TLunaEngine{

	class TTxtFileWriter
	{
	public:
		TTxtFileWriter(void);
		~TTxtFileWriter(void);
	public:
		// ��һ��Txt���ļ�
		// ���أ��Ƿ�ɹ�
		// in filename:�ļ���
		// out ppStream:���ص��ļ�ָ��
		static bool OpenTxtFile(const char* filename,FILE** ppStream);

		// �ر�һ��TMesh���ļ�
		// ���أ���
		// in pStream:�ļ�ָ��
		static void CloseTxtFile(FILE* pStream);

		// д��һ���ַ���
		// ���أ��Ƿ�ɹ�
		// in strWrite:���صĽ���ַ���
		// in pStream:�ļ�ָ��
		// in nCount:д���ַ�����
		static bool WriteLineString(const char* strWrite,FILE* pStream,int nCount);

		// д��һ������
		// ���أ��Ƿ�ɹ�
		// int aiResult:��������
		// in pStream:�ļ�ָ��
		// in nCount:��Ҫ��������������
		// in splice:���ڷָ�N�����ݵ��ַ�
		static bool WriteLineInteger(const int* aiWrite,FILE* pStream,int nCount,char splice);

		// д��һ��long
		// ���أ��Ƿ�ɹ�
		// int alWrite:��������
		// in pStream:�ļ�ָ��
		// in nCount:��Ҫ��������������
		// in splice:���ڷָ�N�����ݵ��ַ�
		static bool WriteLineLong(const long* alWrite,FILE* pStream,int nCount,char splice);

		// д��һ���޷�������
		// ���أ��Ƿ�ɹ�
		// in auWrite:��������
		// in pStream:�ļ�ָ��
		// in nCount:��Ҫ��������������
		// in splice:���ڷָ�N�����ݵ��ַ�
		static bool WriteLineUInteger(const unsigned int* auWrite,FILE* pStream,int nCount,char splice);

		// д��һ���޷���long
		// ���أ��Ƿ�ɹ�
		// in aulWrite:��������
		// in pStream:�ļ�ָ��
		// in nCount:��Ҫ��������������
		// in splice:���ڷָ�N�����ݵ��ַ�
		static bool WriteLineULong(const unsigned long* aulWrite,FILE* pStream,int nCount,char splice);

		// write short
		static bool WriteLineShort(const short* asWrite,FILE* pStream,int nCount,char splice);

		// write ushort
		static bool WriteLineUShort(const unsigned short* ausWrite,FILE* pStream,int nCount,char splice);

		// д��һ�и�����
		// ���أ��Ƿ�ɹ�
		// in afWrite:��������
		// in pStream:�ļ�ָ��
		// in nCount:��Ҫ����������������
		// in splice:���ڷָ�N�����ݵ��ַ�
		static bool WriteLineFloat(const float* afWrite,FILE* pStream,int nCount,char splice);

		// д��һ��˫����
		// ���أ��Ƿ�ɹ�
		// in adWrite:��������
		// in pStream:�ļ�ָ��
		// in nCount:��Ҫ����������������
		// in splice:���ڷָ�N�����ݵ��ַ�
		static bool WriteLineDouble(const double* adWrite,FILE* pStream,int nCount,char splice);

		// д��ȫ���ı�����
		// ���أ��Ƿ�ɹ�
		// buffer,in,write data
		// writeByte,in,write size
		// szFile,in,file name to write
		// mode,in,write mode
		static bool WriteAllFile(const char* szFile,const char* mode,const void* buffer,size_t writeByte);
	};

}