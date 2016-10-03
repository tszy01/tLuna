#include "LuaInitApp.h"

LuaInit* TLunaEngine::Singleton<LuaInit>::m_Ptr = 0;

LuaInit::LuaInit(void) :
m_bWnd(false),
m_bufferHeight(480),
m_bufferWidth(852),
m_bShowDebugInfo(false),
m_controlFps(60.0f),
m_bUseJoystick(false),
m_bOpenConsole(false)
{
	m_szWindowText[0] = '\0';
	m_szResDir[0] = '\0';
}

LuaInit::~LuaInit(void)
{
}

bool LuaInit::InitWindowScript(const char *scriptFile)
{
	if(!InitLuaBase(scriptFile))
		return false;
	return true;
}

void LuaInit::LoadParameters()
{
	// 得到结果
	GetVariable("consoleWidth");
	GetVariable("consoleHeight");
	GetVariable("bOpenConsole");
	GetVariable("bUseJoystick");
	GetVariable("controlFps");
	GetVariable("bShowDebugInfo");
	GetVariable("bWnd");
	GetVariable("bufferWidth");
	GetVariable("bufferHeight");
	GetVariable("mainWindowText");
	GetVariable("resDir");

	const char* szResDir = Lua_tostring(-1);
	memcpy_s(m_szResDir,sizeof(char)*256,szResDir,sizeof(char)*256);
	const char* szTmp=Lua_tostring(-2);
	memcpy_s(m_szWindowText,sizeof(char)*256,szTmp,sizeof(char)*256);
	m_bufferHeight = (unsigned int)Lua_tonumber(-3);
	m_bufferWidth = (unsigned int)Lua_tonumber(-4);
	int b=Lua_toboolean(-5);
	if(b!=0) m_bWnd=true;
	b=Lua_toboolean(-6);
	if(b!=0) m_bShowDebugInfo=true;
	m_controlFps=(float)Lua_tonumber(-7);
	b=Lua_toboolean(-8);
	if(b!=0) m_bUseJoystick=true;
	b = Lua_toboolean(-9);
	if (b != 0) m_bOpenConsole = true;
	m_consoleHeight = (unsigned int)Lua_tonumber(-10);
	m_consoleWidth = (unsigned int)Lua_tonumber(-11);

	Pop(11);
}