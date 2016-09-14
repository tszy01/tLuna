#include "stdafx.h"
#include "TConfigFile.h"
#include "TString.h"

#include <memory.h>
#include <string.h>

using namespace TLunaEngine;

namespace TLunaEngine{

	TConfigFile::TConfigFile(void) : m_stream(0),m_bOpen(false),m_openType(OPEN_NONE)
	{
		memset(m_szFileName,0,256);
	}

	TConfigFile::~TConfigFile(void)
	{
		// ����ⲿ���ǵ��ùر��ļ�
		if(m_bOpen)
		{
			CloseFile();
		}
	}

	bool TConfigFile::OpenFile(const char *file, TConfigFile::OPEN_FILE_TYPE type)
	{
		// ����ⲿ���ǵ��ùر��ļ�
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

	bool TConfigFile::UpdateFile()
	{
		if(!m_bOpen)
			return false;
		if(m_openType == OPEN_READ)
			return false;
		
		// ����ļ�
		fclose(m_stream);
		if( fopen_s(&m_stream,m_szFileName,"wt")!=0 )
		{
			CloseFile();
			return false;
		}

		// д���ļ�
		int numWrite = 0;
		char c;
		size_t szLen = 0;
		int count = 0;
		StrList::iterator itr = m_list.begin();
		for(;itr!=m_list.end();itr++)
		{
			// д��parameter
			szLen = (size_t)itr->m_strParameter.GetLength();
			numWrite = (int)fwrite(itr->m_strParameter.GetString(),sizeof(char),szLen,m_stream);
			if(numWrite!=(int)szLen)
			{
				CloseFile();
				return false;
			}

			// д��Ⱥ�
			c = '=';
			numWrite = (int)fwrite(&c,sizeof(char),1,m_stream);
			if(numWrite!=1)
			{
				CloseFile();
				return false;
			}

			// д��ֵ
			szLen = (size_t)itr->m_strValue.GetLength();
			numWrite = (int)fwrite(itr->m_strValue.GetString(),sizeof(char),szLen,m_stream);
			if(numWrite!=szLen)
			{
				CloseFile();
				return false;
			}

			count++;

			// д�뻻��
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

	void TConfigFile::CloseFile()
	{
		if(m_bOpen)
		{
			// �ر��ļ�
			fclose(m_stream);
			m_stream = 0;
			// �ͷ�parameter��value
			m_list.clear();
			// ������Ա��ʼ��
			m_openType = OPEN_NONE;
			memset(m_szFileName,0,256);
			m_bOpen = false;
		}
	}

	void TConfigFile::GetParameter(const char *paraName, TString* pBuf)
	{
		if(!m_bOpen || !paraName || !pBuf)
			return;

		// �����ҵ�parameter
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

	void TConfigFile::ChangeParameter(const char *paraName, const TString* pBuf)
	{
		if(!m_bOpen || !paraName || !pBuf)
			return;

		// �����ҵ�parameter
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

	void TConfigFile::AddParameter(const char* paraName,const TString* pBuf)
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

	bool TConfigFile::FindParameter(const char* paraName,int* pIndex)
	{
		if(!m_bOpen || !paraName)
			return false;
		// �����ҵ�parameter
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

	bool TConfigFile::LoadFromFile()
	{
		// ��һ���ṹ��
		FILESTR filemap;
		filemap.m_strParameter = "";
		filemap.m_strValue = "";
		// ��ʱ����
		int numRead = 0;
		char c;
		char last;
		int count = 0;
		bool bIsPara = true;
		char szTmp[2] = {0};
		// ѭ���ļ�
		while(feof(m_stream)==0)
		{
			// ��һ���ַ�
			numRead = (int)fread(&c,sizeof(char),1,m_stream);
			// ��ȡ�Ƿ�ɹ�
			if(numRead!=1)
			{
				if(feof(m_stream))
				{
					// ���һ�е�
					if(last != '\n')
						m_list.push_back(filemap);
					break;
				}
				CloseFile();
				return false;
			}
			// �ж�'='
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
					// ��ӵ�����
					m_list.push_back(filemap);
					filemap.m_strParameter = "";
					filemap.m_strValue = "";
				}

				last = c;
				continue;
			}
			last = c;
			// ����������Para����Value��ֵ
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
			// ��count��һ
			count++;
		}
		return true;
	}

}