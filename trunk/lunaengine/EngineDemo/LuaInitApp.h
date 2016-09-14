#pragma once
#include "tluabase.h"
#include "TSingleton.h"
using namespace TLunaEngine;

class CLuaInit :
	public TLuaBase
{
T_SINGLETON_DEF(CLuaInit);
public:
	CLuaInit(void);
	virtual ~CLuaInit(void);
public:
	bool m_bWnd;			// �Ƿ��Ǵ���
	UINT m_bufferHeight;	// �󻺳��
	UINT m_bufferWidth;		// �󻺳��
	float m_controlFps;		// ����֡����
	bool m_bShowDebugInfo;	// �Ƿ���ʾDebug��Ϣ
	bool m_bUseJoystick;	// �Ƿ������ֱ�
	char m_szWindowText[256];	// ����������
	char m_szResDir[256];	// ��Դ��Ŀ¼
public:
	bool InitWindowScript(const char *scriptFile);
	void LoadParameters();
};