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
	bool m_bWnd;			// 是否是窗口
	UINT m_bufferHeight;	// 后缓冲高
	UINT m_bufferWidth;		// 后缓冲宽
	float m_controlFps;		// 控制帧速率
	bool m_bShowDebugInfo;	// 是否显示Debug信息
	bool m_bUseJoystick;	// 是否适用手柄
	char m_szWindowText[256];	// 主窗口名称
	char m_szResDir[256];	// 资源根目录
public:
	bool InitWindowScript(const char *scriptFile);
	void LoadParameters();
};