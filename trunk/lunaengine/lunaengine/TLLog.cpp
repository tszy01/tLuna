#include "TLLog.h"
#include "TLConfigFile.h"
#include <Windows.h>

namespace TLunaEngine{

	// ��ʼ����̬��Ա
	TLunaEngine::String* Log::m_filterArray = 0;
	int Log::m_filterCount = 0;
	char Log::m_logPath[256] = {0};
	bool Log::m_bOpen = false;
	Log::LOG_LEVEL Log::m_minLevel = LOG_LEVEL_INFO;
	/*HANDLE*/void* Log::m_hConsole = 0;
	bool Log::m_bUseConsole = false;

	Log::Log(void)
	{
	}

	Log::~Log(void)
	{
	}

	bool Log::InitLogSystem(const char* configFile, const char* logPath)
	{
		if(!configFile || !logPath)
			return false;
		ConfigFile ConfigFile;
		if(!ConfigFile.OpenFile(configFile,ConfigFile::OPEN_READ))
			return false;
		sprintf_s(m_logPath,256,"%s",logPath);
		// ��ȡ�޳��б�
		String strTmp;
		ConfigFile.GetParameter("FilterCount",&strTmp);
		m_filterCount = atoi(strTmp.GetString());
		if(m_filterCount > 0)
		{
			m_filterArray = new String[m_filterCount];
		}
		for(int i=0;i<m_filterCount;i++)
		{
			char sz[128] ={0};
			strTmp = "";
			sprintf_s(sz,128,"Filter%d",i);
			ConfigFile.GetParameter(sz,&strTmp);
			m_filterArray[i] = strTmp;
		}
		// ��ȡ�Ƿ��д��־
		strTmp = "";
		ConfigFile.GetParameter("OpenWriteLog",&strTmp);
		int boolean = atoi(strTmp.GetString());
		if(boolean==0)
			m_bOpen=false;
		else
			m_bOpen=true;
		// ��ȡ�������ȼ�
		strTmp = "";
		ConfigFile.GetParameter("LogLevel",&strTmp);
		m_minLevel = (LOG_LEVEL)atoi(strTmp.GetString());
		// �Ƿ���������
		strTmp = "";
		ConfigFile.GetParameter("OutputConsole",&strTmp);
		boolean = atoi(strTmp.GetString());
		if(boolean==0)
			m_bUseConsole=false;
		else
			m_bUseConsole=true;
		ConfigFile.CloseFile();
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

	void Log::DestroyLogSystem()
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

	void Log::WriteLine(LOG_LEVEL level,bool bTrue, char* codeName, int codeLine, char *content)
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
		String strCode(codeName);
		for(int i=0;i<m_filterCount&&m_filterArray;i++)
		{
			if(strCode.Find(m_filterArray[i],0,false)!=-1)
				return;
		}
		// �õ�����ʱ����ַ���
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		String strTime;
		strTime.Format("%d:%d:%d",sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
		// ���д�������
		// codeInfo date time content\n
		String strWrite;
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
		String strLogFile;
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

	void Log::WriteTConsole(LOG_LEVEL level,bool bTrue,char* content)
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
		String strWrite;
		if(content)
		{
			strWrite.Format("%s\n",content);
		}
		else
		{
			strWrite.Format("Nothing\n");
		}
		DWORD writtenNum = 0;
		::WriteConsoleA(m_hConsole,strWrite.GetString(),strWrite.GetLength(),&writtenNum,0);
	}

}