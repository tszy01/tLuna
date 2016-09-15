#ifndef _TLLOG_H_
#define _TLLOG_H_

#include "TLString.h"

namespace TLunaEngine{

	class Log
	{
	public:
		Log(void);
		~Log(void);
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
		static int m_filterCount;
		// 保存日志路径
		static char m_logPath[256];
		// 从配置文件读取是否打开写日志
		static bool m_bOpen;
		// 从配置文件读取的最低等级
		static LOG_LEVEL m_minLevel;
		// 输出窗口句柄
		static /*HANDLE*/void* m_hConsole;
		// 是否输出到窗口
		static bool m_bUseConsole;
	public:
		// 初始化
		// configFile:配置文件
		// logPath:日志保存路径
		static bool InitLogSystem(const char* configFile, const char* logPath);
		// 销毁日志系统
		static void DestroyLogSystem();
		// 写日志
		// 写成文件，日志文件名格式:YY_MM_DD.log，文本文件
		// level:本日志的等级
		// bTrue:一个表达式，为真时写入
		// codeName:一般传__FILE__，文件名
		// condLine:一般传__LINE__，所在行数
		// content:日志内容，日志结尾自动加\n形成一行
		static void WriteLine(LOG_LEVEL level,bool bTrue,char* codeName,int codeLine,char* content);
		static void WriteTConsole(LOG_LEVEL level,bool bTrue,char* content);
	};

}

#endif