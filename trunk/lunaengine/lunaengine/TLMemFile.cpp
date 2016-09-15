#include "TLMemFile.h"

#include "TLString.h"

namespace TLunaEngine{

	MemFile::MemFile(void)
	{
		ResetMem();
	}

	MemFile::~MemFile(void)
	{
		ResetMem();
	}

	void MemFile::ResetMem()
	{
		memset(m_szPath,0,1024);
		memset(m_szName,0,1024);
		if(m_ContentSize>0)
		{
			free(m_pContent);
		}
		m_pContent = 0;
		m_ContentSize = 0;
	}

	void MemFile::SetContent(unsigned char *pContent, int size)
	{
		if(!pContent || size<=0)
			return;
		if(m_ContentSize>0)
		{
			free(m_pContent);
		}
		m_pContent = (unsigned char*)malloc(size);
		memcpy(m_pContent,pContent,size);
		m_ContentSize=size;
	}

	int MemFile::GetContent(unsigned char **pContent)
	{
		if(!pContent)
			return 0;
		if(m_ContentSize>0)
		{
			(*pContent) = (unsigned char*)malloc(m_ContentSize);
			memcpy(*pContent,m_pContent,m_ContentSize);
			return m_ContentSize;
		}
		return 0;
	}

	void MemFile::SetFullName(char *fullname)
	{
		if(!fullname)
			return;
		char szPath[1024]={0};
		char szName[1024]={0};
		TLunaEngine::String::CutFilePath(fullname,szName,szPath);
		SetPath(szPath);
		SetName(szName);
	}

	void MemFile::GetFullName(char *fullname)
	{
		if(!fullname)
			return;
		sprintf_s(fullname,1024,"%s%s",m_szPath,m_szName);
	}

	void MemFile::SetPath(char *path)
	{
		if(path)
		{
			memset(m_szPath,0,1024);
			strcpy_s(m_szPath,1024,path);
		}
	}

	void MemFile::GetPath(char *path)
	{
		if(path)
		{
			strcpy_s(path,1024,m_szPath);
		}
	}

	void MemFile::SetName(char *name)
	{
		if(name)
		{
			memset(m_szName,0,1024);
			strcpy_s(m_szName,1024,name);
		}
	}

	void MemFile::GetName(char *name)
	{
		if(name)
		{
			strcpy_s(name,1024,m_szName);
		}
	}

}