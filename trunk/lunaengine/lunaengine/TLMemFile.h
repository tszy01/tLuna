#ifndef _TLMEMFILE_H_
#define _TLMEMFILE_H_

namespace TLunaEngine{

	class MemFile
	{
	public:
		MemFile(void);
		~MemFile(void);
	protected:
		char m_szPath[1024];	// 路径
		char m_szName[1024];	// 名字
		unsigned char* m_pContent;		// 内容
		int m_ContentSize;		// 内容大小
	public:
		// 清空内存
		void ResetMem();
		// 得到内容大小
		inline int GetContentSize(){return m_ContentSize;}
		// 设置内容
		void SetContent(unsigned char* pContent,int size);
		// 得到内容
		int GetContent(unsigned char** pContent);
		// 设置全名
		void SetFullName(char* fullname);
		// 得到全名
		void GetFullName(char* fullname);
		// 设置路径
		void SetPath(char* path);
		// 得到路径
		void GetPath(char* path);
		// 设置名字
		void SetName(char* name);
		// 得到名字
		void GetName(char* name);
	};

}

#endif