#include "TLLuaBase.h"

namespace TLunaEngine{

	LuaBase::LuaBase(TVOID) : m_state(0)
	{
	}

	LuaBase::~LuaBase(TVOID)
	{
		CloseLuaBase();
	}

	TBOOL LuaBase::InitLuaBase(const TCHAR* luaFile)
	{
		if(m_state)
			return TFALSE;

		m_state = lua_open();
		lua_gc(m_state, LUA_GCSTOP, 0);  /* stop collector during initialization */
		luaL_openlibs(m_state);  /* open libraries */
		lua_gc(m_state, LUA_GCRESTART, 0);

		TS32 temp_int; 
		temp_int = luaL_loadfile(m_state,luaFile);
		if (temp_int)
		{
			CloseLuaBase();
			return TFALSE;
		}
		temp_int = lua_pcall(m_state,0,0,0);
		
		if (temp_int)
		{
			lua_gc(m_state, LUA_GCCOLLECT, 0);
			CloseLuaBase();
			return TFALSE;
		}

		return TTRUE;
	}

	TVOID LuaBase::SetVariable(TCHAR* varName)
	{
		if(!m_state)
			return ;
		lua_setglobal(m_state,varName);
	}

	TVOID LuaBase::GetVariable(TCHAR* varName)
	{
		if(!m_state)
			return ;
		lua_getglobal(m_state,varName);
	}

	TBOOL LuaBase::CallFunction(TS32 nargs, TS32 nresults)
	{
		if(!m_state)
			return TFALSE;
		if (lua_pcall(m_state, nargs, nresults, 0) != 0)
		{
			lua_gc(m_state, LUA_GCCOLLECT, 0);
			return TFALSE;
		}
		return TTRUE;
	}

	TVOID LuaBase::CloseLuaBase()
	{
		if(m_state)
		{
			lua_close(m_state);
			m_state=0;
		}
	}

	TVOID LuaBase::Pop(TS32 n)
	{
		if(!m_state)
			return;
		lua_pop(m_state,n); 
	}

}