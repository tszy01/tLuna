#include "TLLog.h"
#include "TLConfigFile.h"
#include "TLConsole.h"
#include <Windows.h>

namespace TLunaEngine{

	// 初始化静态成员
	TLunaEngine::String* Log::m_filterArray = 0;
	TS32 Log::m_filterCount = 0;
	TCHAR Log::m_logPath[256] = {0};
	TBOOL Log::m_bOpen = TFALSE;
	Log::LOG_LEVEL Log::m_minLevel = LOG_LEVEL_INFO;
	/*HANDLE*/TVOID* Log::m_hConsole = 0;
	TBOOL Log::m_bToSysConsole = TFALSE;
	TBOOL Log::m_bToConsole = TFALSE;
	TBOOL Log::m_bToFile = TFALSE;

	Log::Log(TVOID)
	{
	}

	Log::~Log(TVOID)
	{
	}

	TBOOL Log::InitLogSystem(const TCHAR* configFile, const TCHAR* logPath)
	{
		if(!configFile || !logPath)
			return TFALSE;
		ConfigFile ConfigFile;
		if(!ConfigFile.OpenFile(configFile,ConfigFile::OPEN_READ))
			return TFALSE;
		sprintf_s(m_logPath,256,"%s",logPath);
		// 读取剔除列表
		String strTmp;
		ConfigFile.GetParameter("FilterCount",&strTmp);
		m_filterCount = atoi(strTmp.GetString());
		if(m_filterCount > 0)
		{
			m_filterArray = new String[m_filterCount];
		}
		for(TS32 i=0;i<m_filterCount;i++)
		{
			TCHAR sz[128] ={0};
			strTmp = "";
			sprintf_s(sz,128,"Filter%d",i);
			ConfigFile.GetParameter(sz,&strTmp);
			m_filterArray[i] = strTmp;
		}
		// 读取是否打开写日志
		strTmp = "";
		ConfigFile.GetParameter("OpenWriteLog",&strTmp);
		TS32 boolean = atoi(strTmp.GetString());
		if(boolean==0)
			m_bOpen=TFALSE;
		else
			m_bOpen=TTRUE;
		// 读取最低输出等级
		strTmp = "";
		ConfigFile.GetParameter("LogLevel",&strTmp);
		m_minLevel = (LOG_LEVEL)atoi(strTmp.GetString());
		// Output to file
		strTmp = "";
		ConfigFile.GetParameter("ToFile", &strTmp);
		boolean = atoi(strTmp.GetString());
		if (boolean == 0)
			m_bToFile = TFALSE;
		else
			m_bToFile = TTRUE;
		// Output to system console
		strTmp = "";
		ConfigFile.GetParameter("ToSysConsole",&strTmp);
		boolean = atoi(strTmp.GetString());
		if(boolean==0)
			m_bToSysConsole=TFALSE;
		else
			m_bToSysConsole=TTRUE;
		// Output to engine console
		strTmp = "";
		ConfigFile.GetParameter("ToConsole", &strTmp);
		boolean = atoi(strTmp.GetString());
		if (boolean == 0)
			m_bToConsole = TFALSE;
		else
			m_bToConsole = TTRUE;

		ConfigFile.CloseFile();

		// 初始化输出窗口相关
		if (m_bToSysConsole)
		{
			::AllocConsole();
			m_hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
			if (!m_hConsole)
			{
				return TFALSE;
			}
			::SetConsoleTextAttribute(m_hConsole,FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_GREEN);
		}
		return TTRUE;
	}

	TVOID Log::DestroyLogSystem()
	{
		if(m_filterArray)
		{
			delete [] m_filterArray;
			m_filterArray = 0;
		}
		m_filterCount = 0;
		if(m_bToSysConsole)
			::FreeConsole();
	}

	TVOID Log::WriteFile(LOG_LEVEL level,TBOOL bTrue, const TWCHAR* content, const TCHAR* codeName, TS32 codeLine)
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
		if (codeName != TNULL)
		{
			String strCode(codeName);
			for (TS32 i = 0;i<m_filterCount&&m_filterArray;i++)
			{
				if (strCode.Find(m_filterArray[i], 0, TFALSE) != -1)
					return;
			}
		}
		// 得到现在时间的字符串
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		String strTime;
		strTime.Format("%d:%d:%d",sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
		// 最后写入的内容
		// codeInfo date time content\n
		WString strWrite;
		if (codeName != TNULL)
		{
			if (content)
			{
				strWrite.Format(L"%s(%d) %s %s\n", String(codeName).ToWString().getPointer(), 
					String(codeLine).ToWString().getPointer(), strTime.ToWString().getPointer(), content);
			}
			else
			{
				strWrite.Format(L"%s(%d) %s\n", String(codeName).ToWString().getPointer(), 
					String(codeLine).ToWString().getPointer(), strTime.ToWString().getPointer());
			}
		}
		else
		{
			if (content)
			{
				strWrite.Format(L"%s %s\n", strTime.ToWString().getPointer(), content);
			}
			else
			{
				strWrite.Format(L"%s\n", strTime.ToWString().getPointer());
			}
		}
		// 最后写入
		// 合成文件名
		String strLogFile;
		strLogFile.Format("%s%d_%d_%d.log",m_logPath,sysTime.wYear,sysTime.wMonth,sysTime.wDay);
		FILE* stream;
		if( fopen_s(&stream, strLogFile.GetString(), "a+, ccs=UTF-8" )!=0 )
		{
			return ;
		}
		TU32 numwrite = (TU32)fwrite(strWrite.GetWString(),sizeof(TWCHAR),strWrite.GetLength(),stream);
		if(ferror(stream))
		{
			fclose(stream);
			return;
		}
		fclose(stream);
	}

	TVOID Log::WriteSysConsole(LOG_LEVEL level,TBOOL bTrue, const TWCHAR* content)
	{
		if(!m_bToSysConsole)
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
		WString strWrite;
		if(content)
		{
			strWrite.Format(L"%s\n",content);
		}
		else
		{
			strWrite.Format(L"Nothing\n");
		}
		DWORD writtenNum = 0;
		::WriteConsoleW(m_hConsole,strWrite.GetWString(),strWrite.GetLength(),&writtenNum,0);
	}

	TVOID Log::WriteEngineConsole(LOG_LEVEL level, TBOOL bTrue, const TWCHAR* content)
	{
		if (!m_bToConsole)
			return;
		// 如果没有开启
		if (!m_bOpen)
			return;
		// 如果等级不够
		if (level < m_minLevel)
			return;
		// 如果表达式不成立
		if (!bTrue)
			return;
		// 最后写入的内容
		// codeInfo date time content\n
		WString strWrite;
		if (content)
		{
			strWrite.Format(L"%s\n", content);
		}
		else
		{
			strWrite.Format(L"Nothing\n");
		}
		Console::getSingletonPtr()->outputToConsole(strWrite);
	}

	TVOID Log::WriteLine(LOG_LEVEL level, TBOOL bTrue, const TWCHAR* content, const TCHAR* codeName, TS32 codeLine)
	{
		WriteFile(level, bTrue, content, codeName, codeLine);
		WriteSysConsole(level, bTrue, content);
		WriteEngineConsole(level, bTrue, content);
	}
}