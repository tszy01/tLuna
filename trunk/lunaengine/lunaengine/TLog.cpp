#include "StdAfx.h"
#include "TLog.h"
#include "TConfigFile.h"
#include <Windows.h>

namespace TLunaEngine{

	// ��ʼ����̬��Ա
	TLunaEngine::TString* TLog::m_filterArray = 0;
	int TLog::m_filterCount = 0;
	char TLog::m_logPath[256] = {0};
	bool TLog::m_bOpen = false;
	TLog::LOG_LEVEL TLog::m_minLevel = LOG_LEVEL_INFO;
	/*HANDLE*/void* TLog::m_hConsole = 0;
	bool TLog::m_bUseConsole = false;

	TLog::TLog(void)
	{
	}

	TLog::~TLog(void)
	{
	}

	bool TLog::InitLogSystem(const char* configFile, const char* logPath)
	{
		if(!configFile || !logPath)
			return false;
		TConfigFile TConfigFile;
		if(!TConfigFile.OpenFile(configFile,TConfigFile::OPEN_READ))
			return false;
		sprintf_s(m_logPath,256,"%s",logPath);
		// ��ȡ�޳��б�
		TString strTmp;
		TConfigFile.GetParameter("FilterCount",&strTmp);
		m_filterCount = atoi(strTmp.GetString());
		if(m_filterCount > 0)
		{
			m_filterArray = new TString[m_filterCount];
		}
		for(int i=0;i<m_filterCount;i++)
		{
			char sz[128] ={0};
			strTmp = "";
			sprintf_s(sz,128,"Filter%d",i);
			TConfigFile.GetParameter(sz,&strTmp);
			m_filterArray[i] = strTmp;
		}
		// ��ȡ�Ƿ��д��־
		strTmp = "";
		TConfigFile.GetParameter("OpenWriteLog",&strTmp);
		int boolean = atoi(strTmp.GetString());
		if(boolean==0)
			m_bOpen=false;
		else
			m_bOpen=true;
		// ��ȡ�������ȼ�
		strTmp = "";
		TConfigFile.GetParameter("LogLevel",&strTmp);
		m_minLevel = (LOG_LEVEL)atoi(strTmp.GetString());
		// �Ƿ���������
		strTmp = "";
		TConfigFile.GetParameter("OutputConsole",&strTmp);
		boolean = atoi(strTmp.GetString());
		if(boolean==0)
			m_bUseConsole=false;
		else
			m_bUseConsole=true;
		TConfigFile.CloseFile();
		// ��ʼ������������
		if (m_bUseConsole)
		{
			::AllocConsole();
			m_hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
			if (!m_hConsole)
			{
				return false;
			}
			::SetConsoleTextAttribute(m_hConsole,FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_GREEN);
		}
		return true;
	}

	void TLog::DestroyLogSystem()
	{
		if(m_filterArray)
		{
			delete [] m_filterArray;
			m_filterArray = 0;
		}
		m_filterCount = 0;
		if(m_bUseConsole)
			::FreeConsole();
	}

	void TLog::WriteLine(LOG_LEVEL level,bool bTrue, char* codeName, int codeLine, char *content)
	{
		// ����û������
		// ���Ǳ�����Path
		if(!m_logPath)
			return;
		// ���û�п���
		if(!m_bOpen)
			return;
		// ����ȼ�����
		if(level < m_minLevel)
			return;
		// ������ʽ������
		if(!bTrue)
			return;
		// �����ļ����е�·��
		TString strCode(codeName);
		for(int i=0;i<m_filterCount&&m_filterArray;i++)
		{
			if(strCode.Find(m_filterArray[i],0,false)!=-1)
				return;
		}
		// �õ�����ʱ����ַ���
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		TString strTime;
		strTime.Format("%d:%d:%d",sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
		// ���д�������
		// codeInfo date time content\n
		TString strWrite;
		if(content)
		{
			strWrite.Format("%s(%d) %s %s\n",codeName,codeLine,strTime.GetString(),content);
		}
		else
		{
			strWrite.Format("%s(%d) %s\n",codeName,codeLine,strTime.GetString());
		}
		// ���д��
		// �ϳ��ļ���
		TString strLogFile;
		strLogFile.Format("%s%d_%d_%d.log",m_logPath,sysTime.wYear,sysTime.wMonth,sysTime.wDay);
		FILE* stream;
		if( fopen_s(&stream, strLogFile.GetString(), "a+" )!=0 )
		{
			return ;
		}
		int numwrite = fwrite(strWrite.GetString(),sizeof(char),strWrite.GetLength(),stream);
		if(ferror(stream))
		{
			fclose(stream);
			return;
		}
		fclose(stream);
	}

	void TLog::WriteTConsole(LOG_LEVEL level,bool bTrue,char* content)
	{
		if(!m_bUseConsole)
			return;
		// ����û������
		// ���Ǳ�����Path
		if(!m_hConsole)
			return;
		// ���û�п���
		if(!m_bOpen)
			return;
		// ����ȼ�����
		if(level < m_minLevel)
			return;
		// ������ʽ������
		if(!bTrue)
			return;
		// ���д�������
		// codeInfo date time content\n
		TString strWrite;
		if(content)
		{
			strWrite.Format("%s\n",content);
		}
		else
		{
			strWrite.Format("Nothing\n");
		}
		DWORD writtenNum = 0;
		::WriteConsoleA(m_hConsole,strWrite.GetString(),(DWORD)strWrite.GetLength(),&writtenNum,0);
	}

}