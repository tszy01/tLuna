#include "stdafx.h"
#include "TLuaBase.h"

namespace TLunaEngine{

	TLuaBase::TLuaBase(void) : m_state(NULL)
	{
	}

	TLuaBase::~TLuaBase(void)
	{
		CloseLuaBase();
	}

	bool TLuaBase::InitLuaBase(const char *luaFile)
	{
		if(m_state)
			return false;

		m_state = lua_open();
		lua_gc(m_state, LUA_GCSTOP, 0);  /* stop collector during initialization */
		luaL_openlibs(m_state);  /* open libraries */
		lua_gc(m_state, LUA_GCRESTART, 0);

		int temp_int; 
		temp_int = luaL_loadfile(m_state,luaFile);
		if (temp_int)
		{
			CloseLuaBase();
			return false;
		}
		temp_int = lua_pcall(m_state,0,0,0);
		
		if (temp_int)
		{
			lua_gc(m_state, LUA_GCCOLLECT, 0);
			CloseLuaBase();
			return false;
		}

		return true;
	}

	void TLuaBase::SetVariable(char *varName)
	{
		if(!m_state)
			return ;
		lua_setglobal(m_state,varName);
	}

	void TLuaBase::GetVariable(char *varName)
	{
		if(!m_state)
			return ;
		lua_getglobal(m_state,varName);
	}

	bool TLuaBase::CallFunction(int nargs, int nresults)
	{
		if(!m_state)
			return false;
		if (lua_pcall(m_state, nargs, nresults, 0) != 0)
		{
			lua_gc(m_state, LUA_GCCOLLECT, 0);
			return false;
		}
		return true;
	}

	void TLuaBase::CloseLuaBase()
	{
		if(m_state)
		{
			lua_close(m_state);
			m_state=NULL;
		}
	}

	void TLuaBase::Pop(int n)
	{
		if(!m_state)
			return;
		lua_pop(m_state,n); 
	}

}