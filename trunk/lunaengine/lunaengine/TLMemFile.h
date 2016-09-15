#ifndef _TLMEMFILE_H_
#define _TLMEMFILE_H_

namespace TLunaEngine{

	class MemFile
	{
	public:
		MemFile(void);
		~MemFile(void);
	protected:
		char m_szPath[1024];	// ·��
		char m_szName[1024];	// ����
		unsigned char* m_pContent;		// ����
		int m_ContentSize;		// ���ݴ�С
	public:
		// ����ڴ�
		void ResetMem();
		// �õ����ݴ�С
		inline int GetContentSize(){return m_ContentSize;}
		// ��������
		void SetContent(unsigned char* pContent,int size);
		// �õ�����
		int GetContent(unsigned char** pContent);
		// ����ȫ��
		void SetFullName(char* fullname);
		// �õ�ȫ��
		void GetFullName(char* fullname);
		// ����·��
		void SetPath(char* path);
		// �õ�·��
		void GetPath(char* path);
		// ��������
		void SetName(char* name);
		// �õ�����
		void GetName(char* name);
	};

}

#endif