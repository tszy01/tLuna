#ifndef _TLLOG_H_
#define _TLLOG_H_

#include "TLString.h"

namespace TLunaEngine{

	class Log
	{
	public:
		Log(TVOID);
		~Log(TVOID);
	public:
		enum LOG_LEVEL
		{
			LOG_LEVEL_INFO	= 1,	// 信息日志
			LOG_LEVEL_WARNING	= 2,// 警告日志
			LOG_LEVEL_ERROR	= 3,	// 错误日志
		};
	protected:
		// 保存剔除路径列表
		// 含有剔除路径的代码文件名将不被输出到日志
		static String* m_filterArray;
		// 剔除列表大小
		static TS32 m_filterCount;
		// 保存日志路径
		static TCHAR m_logPath[256];
		// 从配置文件读取是否打开写日志
		static TBOOL m_bOpen;
		// 从配置文件读取的最低等级
		static LOG_LEVEL m_minLevel;
		// 输出窗口句柄
		static /*HANDLE*/TVOID* m_hConsole;
		// 是否输出到系统控制台
		static TBOOL m_bToSysConsole;
		// output to engine console
		static TBOOL m_bToConsole;
		// output to file
		static TBOOL m_bToFile;
	public:
		// 初始化
		// configFile:配置文件
		// logPath:日志保存路径
		static TBOOL InitLogSystem(const TCHAR* configFile, const TCHAR* logPath);
		// 销毁日志系统
		static TVOID DestroyLogSystem();
		// 写成文件，日志文件名格式:YY_MM_DD.log，文本文件
		static TVOID WriteFile(LOG_LEVEL level,TBOOL bTrue, const TWCHAR* content, const TCHAR* codeName = TNULL,TS32 codeLine = 0);
		// Output to system console
		static TVOID WriteSysConsole(LOG_LEVEL level,TBOOL bTrue, const TWCHAR* content);
		// Output to Engine console
		static TVOID WriteEngineConsole(LOG_LEVEL level, TBOOL bTrue, const TWCHAR* content);
		// 写日志
		// level:本日志的等级
		// bTrue:一个表达式，为真时写入
		// codeName:一般传__FILE__，文件名
		// condLine:一般传__LINE__，所在行数
		// content:日志内容，日志结尾自动加\n形成一行
		static TVOID WriteLine(LOG_LEVEL level, TBOOL bTrue, const TWCHAR* content, const TCHAR* codeName = TNULL, TS32 codeLine = 0);
	};

}

#endif