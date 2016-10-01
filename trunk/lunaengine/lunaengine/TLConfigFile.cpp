#include "TLConfigFile.h"
#include "TLString.h"

#include <memory.h>
#include <string.h>

namespace TLunaEngine{

	ConfigFile::ConfigFile(TVOID) : m_stream(0),m_bOpen(TFALSE),m_openType(OPEN_NONE)
	{
		memset(m_szFileName,0,256);
	}

	ConfigFile::~ConfigFile(TVOID)
	{
		// 如果外部忘记调用关闭文件
		if(m_bOpen)
		{
			CloseFile();
		}
	}

	TBOOL ConfigFile::OpenFile(const TCHAR* file, ConfigFile::OPEN_FILE_TYPE type)
	{
		// 如果外部忘记调用关闭文件
		if(m_bOpen || !file)
		{
			return TFALSE;
		}

		if(type == OPEN_WRITE)
		{
			if( fopen_s(&m_stream, file, "w+t" )!=0 )
			{
				return TFALSE;
			}
		}
		else if(type == OPEN_READ)
		{
			if( fopen_s(&m_stream, file, "rt" )!=0 )
			{
				return TFALSE;
			}
		}
		else if(type == OPEN_WRITE_AND_READ)
		{
			if( fopen_s(&m_stream, file, "r+t" )!=0 )
			{
				return TFALSE;
			}
		}
		else
		{
			return TFALSE;
		}

		m_openType = type;
		strcpy_s(m_szFileName,256,file);
		m_bOpen = TTRUE;

		if(!LoadFromFile())
			return TFALSE;

		return TTRUE;
	}

	TBOOL ConfigFile::UpdateFile()
	{
		if(!m_bOpen)
			return TFALSE;
		if(m_openType == OPEN_READ)
			return TFALSE;
		
		// 清空文件
		fclose(m_stream);
		if( fopen_s(&m_stream,m_szFileName,"wt")!=0 )
		{
			CloseFile();
			return TFALSE;
		}

		// 写入文件
		TU32 numWrite = 0;
		TCHAR c;
		TU32 szLen = 0;
		TU32 count = 0;
		StrList::Iterator itr = m_list.begin();
		for(;itr!=m_list.end();itr++)
		{
			// 写入parameter
			szLen = itr->m_strParameter.GetLength();
			numWrite = (TU32)fwrite(itr->m_strParameter.GetString(),sizeof(TCHAR),szLen,m_stream);
			if(numWrite!=szLen)
			{
				CloseFile();
				return TFALSE;
			}

			// 写入等号
			c = '=';
			numWrite = (TU32)fwrite(&c,sizeof(TCHAR),1,m_stream);
			if(numWrite!=1)
			{
				CloseFile();
				return TFALSE;
			}

			// 写入值
			szLen = itr->m_strValue.GetLength();
			numWrite = (TU32)fwrite(itr->m_strValue.GetString(),sizeof(TCHAR),szLen,m_stream);
			if(numWrite!=szLen)
			{
				CloseFile();
				return TFALSE;
			}

			count++;

			// 写入换行
			if(count < (TU32)m_list.size())
			{
				c = '\n';
				numWrite = (TU32)fwrite(&c,sizeof(TCHAR),1,m_stream);
				if(numWrite!=1)
				{
					CloseFile();
					return TFALSE;
				}
			}
		}
		
		return TTRUE;
	}

	TVOID ConfigFile::CloseFile()
	{
		if(m_bOpen)
		{
			// 关闭文件
			fclose(m_stream);
			m_stream = 0;
			// 释放parameter和value
			m_list.clear();
			// 将各成员初始化
			m_openType = OPEN_NONE;
			memset(m_szFileName,0,256);
			m_bOpen = TFALSE;
		}
	}

	TVOID ConfigFile::GetParameter(const TCHAR* paraName, String* pBuf)
	{
		if(!m_bOpen || !paraName || !pBuf)
			return;

		// 遍历找到parameter
		StrList::Iterator itr = m_list.begin();
		for(;itr!=m_list.end();itr++)
		{
			if(strcmp(paraName,itr->m_strParameter.GetString())==0)
			{
				(*pBuf) = itr->m_strValue;
				break;
			}
		}
	}

	TVOID ConfigFile::ChangeParameter(const TCHAR* paraName, const String* pBuf)
	{
		if(!m_bOpen || !paraName || !pBuf)
			return;

		// 遍历找到parameter
		StrList::Iterator itr = m_list.begin();
		for(;itr!=m_list.end();itr++)
		{
			if(strcmp(paraName,itr->m_strParameter.GetString())==0)
			{
				itr->m_strValue = (*pBuf);
				break;
			}
		}
	}

	TVOID ConfigFile::AddParameter(const TCHAR* paraName,const String* pBuf)
	{
		if(!m_bOpen || !paraName || !pBuf)
			return;
		if(FindParameter(paraName,0))
			return;
		_FILESTR str;
		str.m_strParameter = paraName;
		str.m_strValue = *pBuf;
		m_list.push_back(str);
	}

	TBOOL ConfigFile::FindParameter(const TCHAR* paraName,TS32* pIndex)
	{
		if(!m_bOpen || !paraName)
			return TFALSE;
		// 遍历找到parameter
		StrList::Iterator itr = m_list.begin();
		TS32 count = 0;
		for(;itr!=m_list.end();itr++,count++)
		{
			if(strcmp(paraName,itr->m_strParameter.GetString())==0)
			{
				if(pIndex)
					*pIndex = count;
				return TTRUE;
			}
		}
		return TFALSE;
	}

	TBOOL ConfigFile::LoadFromFile()
	{
		// 建一个结构体
		FILESTR filemap;
		filemap.m_strParameter = "";
		filemap.m_strValue = "";
		// 临时变量
		TU32 numRead = 0;
		TCHAR c;
		TCHAR last;
		TU32 count = 0;
		TBOOL bIsPara = TTRUE;
		TCHAR szTmp[2] = {0};
		// 循环文件
		while(feof(m_stream)==0)
		{
			// 读一个字符
			numRead = (TU32)fread(&c,sizeof(TCHAR),1,m_stream);
			// 读取是否成功
			if(numRead!=1)
			{
				if(feof(m_stream))
				{
					// 最后一行的
					if(last != '\n')
						m_list.push_back(filemap);
					break;
				}
				CloseFile();
				return TFALSE;
			}
			// 判断'='
			if(c == '=')
			{
				bIsPara = TFALSE;
				count = 0;
				last = c;
				continue;
			}
			else if(c == '\n')
			{
				bIsPara = TTRUE;
				count = 0;

				if(last != '\n')
				{
					// 添加到链表
					m_list.push_back(filemap);
					filemap.m_strParameter = "";
					filemap.m_strValue = "";
				}

				last = c;
				continue;
			}
			last = c;
			// 根据现在是Para还是Value赋值
			if(bIsPara)
			{
				szTmp[0] = c;
				filemap.m_strParameter += szTmp;
			}
			else
			{
				szTmp[0] = c;
				filemap.m_strValue += szTmp;
			}
			// 把count加一
			count++;
		}
		return TTRUE;
	}

}