#ifndef _TLMEMFILE_H_
#define _TLMEMFILE_H_

#include "TLCommonTypeDef.h"
namespace TLunaEngine{

	class MemFile
	{
	public:
		MemFile(TVOID);
		~MemFile(TVOID);
	protected:
		TCHAR m_szPath[1024];	// 路径
		TCHAR m_szName[1024];	// 名字
		TUByte* m_pContent;		// 内容
		TSIZE m_ContentSize;	// 内容大小
	public:
		// 清空内存
		TVOID ResetMem();
		// 得到内容大小
		inline TSIZE GetContentSize(){return m_ContentSize;}
		// 设置内容
		TVOID SetContent(TUByte* pContent, TSIZE size);
		// 得到内容
		TSIZE GetContent(TUByte** pContent);
		// 设置全名
		TVOID SetFullName(TCHAR* fullname);
		// 得到全名
		TVOID GetFullName(TCHAR* fullname);
		// 设置路径
		TVOID SetPath(TCHAR* path);
		// 得到路径
		TVOID GetPath(TCHAR* path);
		// 设置名字
		TVOID SetName(TCHAR* name);
		// 得到名字
		TVOID GetName(TCHAR* name);
	};

}

#endif