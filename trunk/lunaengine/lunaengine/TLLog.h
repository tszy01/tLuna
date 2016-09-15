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
			LOG_LEVEL_INFO	= 1,	// ��Ϣ��־
			LOG_LEVEL_WARNING	= 2,// ������־
			LOG_LEVEL_ERROR	= 3,	// ������־
		};
	protected:
		// �����޳�·���б�
		// �����޳�·���Ĵ����ļ����������������־
		static String* m_filterArray;
		// �޳��б��С
		static int m_filterCount;
		// ������־·��
		static char m_logPath[256];
		// �������ļ���ȡ�Ƿ��д��־
		static bool m_bOpen;
		// �������ļ���ȡ����͵ȼ�
		static LOG_LEVEL m_minLevel;
		// ������ھ��
		static /*HANDLE*/void* m_hConsole;
		// �Ƿ����������
		static bool m_bUseConsole;
	public:
		// ��ʼ��
		// configFile:�����ļ�
		// logPath:��־����·��
		static bool InitLogSystem(const char* configFile, const char* logPath);
		// ������־ϵͳ
		static void DestroyLogSystem();
		// д��־
		// д���ļ�����־�ļ�����ʽ:YY_MM_DD.log���ı��ļ�
		// level:����־�ĵȼ�
		// bTrue:һ�����ʽ��Ϊ��ʱд��
		// codeName:һ�㴫__FILE__���ļ���
		// condLine:һ�㴫__LINE__����������
		// content:��־���ݣ���־��β�Զ���\n�γ�һ��
		static void WriteLine(LOG_LEVEL level,bool bTrue,char* codeName,int codeLine,char* content);
		static void WriteTConsole(LOG_LEVEL level,bool bTrue,char* content);
	};

}

#endif