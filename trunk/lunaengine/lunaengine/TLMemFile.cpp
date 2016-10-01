#include "TLMemFile.h"

#include "TLString.h"
#include <stdio.h>

namespace TLunaEngine{

	MemFile::MemFile(TVOID)
	{
		ResetMem();
	}

	MemFile::~MemFile(TVOID)
	{
		ResetMem();
	}

	TVOID MemFile::ResetMem()
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

	TVOID MemFile::SetContent(TUByte *pContent, TSIZE size)
	{
		if(!pContent || size<=0)
			return;
		if(m_ContentSize>0)
		{
			free(m_pContent);
		}
		m_pContent = (TUByte*)malloc(size);
		memcpy(m_pContent,pContent,size);
		m_ContentSize=size;
	}

	TSIZE MemFile::GetContent(TUByte **pContent)
	{
		if(!pContent)
			return 0;
		if(m_ContentSize>0)
		{
			(*pContent) = (TUByte*)malloc(m_ContentSize);
			memcpy(*pContent,m_pContent,m_ContentSize);
			return m_ContentSize;
		}
		return 0;
	}

	TVOID MemFile::SetFullName(TCHAR* fullname)
	{
		if(!fullname)
			return;
		TCHAR szPath[1024]={0};
		TCHAR szName[1024]={0};
		TLunaEngine::String::CutFilePath(fullname,szName,szPath);
		SetPath(szPath);
		SetName(szName);
	}

	TVOID MemFile::GetFullName(TCHAR* fullname)
	{
		if(!fullname)
			return;
		sprintf_s(fullname,1024,"%s%s",m_szPath,m_szName);
	}

	TVOID MemFile::SetPath(TCHAR* path)
	{
		if(path)
		{
			memset(m_szPath,0,1024);
			strcpy_s(m_szPath,1024,path);
		}
	}

	TVOID MemFile::GetPath(TCHAR* path)
	{
		if(path)
		{
			strcpy_s(path,1024,m_szPath);
		}
	}

	TVOID MemFile::SetName(TCHAR* name)
	{
		if(name)
		{
			memset(m_szName,0,1024);
			strcpy_s(m_szName,1024,name);
		}
	}

	TVOID MemFile::GetName(TCHAR* name)
	{
		if(name)
		{
			strcpy_s(name,1024,m_szName);
		}
	}

}