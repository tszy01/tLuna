#include "TLConfigFile.h"
#include "TLString.h"

#include <memory.h>
#include <string.h>

namespace TLunaEngine{

	ConfigFile::ConfigFile(void) : m_stream(0),m_bOpen(false),m_openType(OPEN_NONE)
	{
		memset(m_szFileName,0,256);
	}

	ConfigFile::~ConfigFile(void)
	{
		// 如果外部忘记调用关闭文件
		if(m_bOpen)
		{
			CloseFile();
		}
	}

	bool ConfigFile::OpenFile(const char *file, ConfigFile::OPEN_FILE_TYPE type)
	{
		// 如果外部忘记调用关闭文件
		if(m_bOpen || !file)
		{
			return false;
		}

		if(type == OPEN_WRITE)
		{
			if( fopen_s(&m_stream, file, "w+t" )!=0 )
			{
				return false;
			}
		}
		else if(type == OPEN_READ)
		{
			if( fopen_s(&m_stream, file, "rt" )!=0 )
			{
				return false;
			}
		}
		else if(type == OPEN_WRITE_AND_READ)
		{
			if( fopen_s(&m_stream, file, "r+t" )!=0 )
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		m_openType = type;
		strcpy_s(m_szFileName,256,file);
		m_bOpen = true;

		if(!LoadFromFile())
			return false;

		return true;
	}

	bool ConfigFile::UpdateFile()
	{
		if(!m_bOpen)
			return false;
		if(m_openType == OPEN_READ)
			return false;
		
		// 清空文件
		fclose(m_stream);
		if( fopen_s(&m_stream,m_szFileName,"wt")!=0 )
		{
			CloseFile();
			return false;
		}

		// 写入文件
		int numWrite = 0;
		char c;
		size_t szLen = 0;
		int count = 0;
		StrList::iterator itr = m_list.begin();
		for(;itr!=m_list.end();itr++)
		{
			// 写入parameter
			szLen = (size_t)itr->m_strParameter.GetLength();
			numWrite = (int)fwrite(itr->m_strParameter.GetString(),sizeof(char),szLen,m_stream);
			if(numWrite!=(int)szLen)
			{
				CloseFile();
				return false;
			}

			// 写入等号
			c = '=';
			numWrite = (int)fwrite(&c,sizeof(char),1,m_stream);
			if(numWrite!=1)
			{
				CloseFile();
				return false;
			}

			// 写入值
			szLen = (size_t)itr->m_strValue.GetLength();
			numWrite = (int)fwrite(itr->m_strValue.GetString(),sizeof(char),szLen,m_stream);
			if(numWrite!=szLen)
			{
				CloseFile();
				return false;
			}

			count++;

			// 写入换行
			if(count < (int)m_list.size())
			{
				c = '\n';
				numWrite = (int)fwrite(&c,sizeof(char),1,m_stream);
				if(numWrite!=1)
				{
					CloseFile();
					return false;
				}
			}
		}
		
		return true;
	}

	void ConfigFile::CloseFile()
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
			m_bOpen = false;
		}
	}

	void ConfigFile::GetParameter(const char *paraName, String* pBuf)
	{
		if(!m_bOpen || !paraName || !pBuf)
			return;

		// 遍历找到parameter
		StrList::iterator itr = m_list.begin();
		for(;itr!=m_list.end();itr++)
		{
			if(strcmp(paraName,itr->m_strParameter.GetString())==0)
			{
				(*pBuf) = itr->m_strValue;
				break;
			}
		}
	}

	void ConfigFile::ChangeParameter(const char *paraName, const String* pBuf)
	{
		if(!m_bOpen || !paraName || !pBuf)
			return;

		// 遍历找到parameter
		StrList::iterator itr = m_list.begin();
		for(;itr!=m_list.end();itr++)
		{
			if(strcmp(paraName,itr->m_strParameter.GetString())==0)
			{
				itr->m_strValue = (*pBuf);
				break;
			}
		}
	}

	void ConfigFile::AddParameter(const char* paraName,const String* pBuf)
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

	bool ConfigFile::FindParameter(const char* paraName,int* pIndex)
	{
		if(!m_bOpen || !paraName)
			return false;
		// 遍历找到parameter
		StrList::iterator itr = m_list.begin();
		int count = 0;
		for(;itr!=m_list.end();itr++,count++)
		{
			if(strcmp(paraName,itr->m_strParameter.GetString())==0)
			{
				if(pIndex)
					*pIndex = count;
				return true;
			}
		}
		return false;
	}

	bool ConfigFile::LoadFromFile()
	{
		// 建一个结构体
		FILESTR filemap;
		filemap.m_strParameter = "";
		filemap.m_strValue = "";
		// 临时变量
		int numRead = 0;
		char c;
		char last;
		int count = 0;
		bool bIsPara = true;
		char szTmp[2] = {0};
		// 循环文件
		while(feof(m_stream)==0)
		{
			// 读一个字符
			numRead = (int)fread(&c,sizeof(char),1,m_stream);
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
				return false;
			}
			// 判断'='
			if(c == '=')
			{
				bIsPara = false;
				count = 0;
				last = c;
				continue;
			}
			else if(c == '\n')
			{
				bIsPara = true;
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
		return true;
	}

}