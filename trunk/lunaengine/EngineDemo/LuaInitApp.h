#ifndef _LUAINITAPP_H_
#define _LUAINITAPP_H_
#include "tlluabase.h"
#include "TLSingleton.h"

class LuaInit : public TLunaEngine::Singleton<LuaInit>, public TLunaEngine::LuaBase
{
	friend class TLunaEngine::Singleton<LuaInit>;
protected:
	LuaInit(void);
	virtual ~LuaInit(void);
public:
	bool m_bWnd;			// �Ƿ��Ǵ���
	unsigned int m_bufferHeight;	// �󻺳��
	unsigned int m_bufferWidth;		// �󻺳��
	float m_controlFps;		// ����֡����
	bool m_bShowDebugInfo;	// �Ƿ���ʾDebug��Ϣ
	bool m_bUseJoystick;	// �Ƿ������ֱ�
	char m_szWindowText[256];	// ����������
	char m_szResDir[256];	// ��Դ��Ŀ¼
public:
	bool InitWindowScript(const char *scriptFile);
	void LoadParameters();
};

#endif