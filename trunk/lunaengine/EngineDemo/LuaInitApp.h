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
	bool m_bWnd;			// 是否是窗口
	unsigned int m_bufferHeight;	// 后缓冲高
	unsigned int m_bufferWidth;		// 后缓冲宽
	float m_controlFps;		// 控制帧速率
	bool m_bShowDebugInfo;	// 是否显示Debug信息
	bool m_bUseJoystick;	// 是否适用手柄
	char m_szWindowText[256];	// 主窗口名称
	char m_szResDir[256];	// 资源根目录
public:
	bool InitWindowScript(const char *scriptFile);
	void LoadParameters();
};

#endif