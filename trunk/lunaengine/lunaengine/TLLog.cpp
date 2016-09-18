#include "TLLog.h"
#include "TLConfigFile.h"
#include <Windows.h>

namespace TLunaEngine{

	// 初始化静态成员
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
		// 读取剔除列表
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
		// 读取是否打开写日志
		strTmp = "";
		ConfigFile.GetParameter("OpenWriteLog",&strTmp);
		int boolean = atoi(strTmp.GetString());
		if(boolean==0)
			m_bOpen=false;
		else
			m_bOpen=true;
		// 读取最低输出等级
		strTmp = "";
		ConfigFile.GetParameter("LogLevel",&strTmp);
		m_minLevel = (LOG_LEVEL)atoi(strTmp.GetString());
		// 是否打开输出窗口
		strTmp = "";
		ConfigFile.GetParameter("OutputConsole",&strTmp);
		boolean = atoi(strTmp.GetString());
		if(boolean==0)
			m_bUseConsole=false;
		else
			m_bUseConsole=true;
		ConfigFile.CloseFile();
		// 初始化输出窗口相关
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
		// 可以没有内容
		// 但是必须有Path
		if(!m_logPath)
			return;
		// 如果没有开启
		if(!m_bOpen)
			return;
		// 如果等级不够
		if(level < m_minLevel)
			return;
		// 如果表达式不成立
		if(!bTrue)
			return;
		// 过滤文件名中的路径
		String strCode(codeName);
		for(int i=0;i<m_filterCount&&m_filterArray;i++)
		{
			if(strCode.Find(m_filterArray[i],0,false)!=-1)
				return;
		}
		// 得到现在时间的字符串
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		String strTime;
		strTime.Format("%d:%d:%d",sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
		// 最后写入的内容
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
		// 最后写入
		// 合成文件名
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
		// 可以没有内容
		// 但是必须有Path
		if(!m_hConsole)
			return;
		// 如果没有开启
		if(!m_bOpen)
			return;
		// 如果等级不够
		if(level < m_minLevel)
			return;
		// 如果表达式不成立
		if(!bTrue)
			return;
		// 最后写入的内容
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