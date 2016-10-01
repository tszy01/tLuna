#ifndef _TLLUABASE_H_
#define _TLLUABASE_H_

extern "C"{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <iostream> /*unlike most LUA examples I've found we will be using c++ */
#include <string>
#include "TLCommonTypeDef.h"

namespace TLunaEngine{

	// Lua上层封装类,继承本类,处理特定的脚本
	// 涉及到的lua基础库以及luabind库的函数,推荐直接使用库函数

	class LuaBase
	{
	public:
		LuaBase(TVOID);
	public:
		virtual ~LuaBase(TVOID);

	protected:
		// 成员变量
		lua_State *m_state;			// lua state
	public:
		// 成员方法
		TBOOL InitLuaBase(const TCHAR* luaFile);
		TVOID SetVariable(TCHAR* varName);
		TVOID GetVariable(TCHAR* varName);
		TVOID CloseLuaBase();
		TVOID Pop(TS32 n);
		// C++调用lua函数
		// 如果是用lua基本库调用，请使用本函数，并且在调用之后需要pop出N个返回值
		TBOOL CallFunction(TS32 nargs,TS32 nresults);
		inline lua_State* GetState(){return m_state;}

	protected:

		// ---------------------------- 扩展lua全局函数 ------------------------

		/*
		** access functions (stack -> C)
		*/

		TS32				Lua_isnumber (TS32 idx){return lua_isnumber(m_state,idx);};
		TS32             Lua_isstring (TS32 idx){return lua_isstring(m_state,idx);};
		TS32             Lua_iscfunction (TS32 idx){return lua_iscfunction(m_state,idx);};
		TS32             Lua_isuserdata (TS32 idx){return lua_isuserdata(m_state,idx);};
		TS32             Lua_type (TS32 idx){return lua_type(m_state,idx);};
		const TCHAR*	Lua_typename (TS32 tp){return lua_typename(m_state,tp);};

		TS32				Lua_equal (TS32 idx1, TS32 idx2){return lua_equal(m_state,idx1,idx2);};
		TS32				Lua_rawequal (TS32 idx1, TS32 idx2){return lua_rawequal(m_state,idx1,idx2);};
		TS32				Lua_lessthan (TS32 idx1, TS32 idx2){return lua_lessthan(m_state,idx1,idx2);};

		lua_Number      Lua_tonumber (TS32 idx){return lua_tonumber(m_state,idx);};
		TS32             Lua_toboolean (TS32 idx){return lua_toboolean(m_state,idx);};
		const TCHAR*	Lua_tostring (TS32 idx){return lua_tostring(m_state,idx);};
		TSIZE          Lua_strlen (TS32 idx){return lua_strlen(m_state,idx);};
		lua_CFunction   Lua_tocfunction (TS32 idx){return lua_tocfunction(m_state,idx);};
		TVOID			*Lua_touserdata (TS32 idx){return lua_touserdata(m_state,idx);};
		lua_State		*Lua_tothread (TS32 idx){return lua_tothread(m_state,idx);};
		const TVOID		*Lua_topointer (TS32 idx){return lua_topointer(m_state,idx);};


		/*
		** push functions (C -> stack)
		*/
		TVOID  Lua_pushnil (){return lua_pushnil(m_state);};
		TVOID  Lua_pushnumber (lua_Number n){return lua_pushnumber(m_state,n);};
		TVOID  Lua_pushlstring (const TCHAR* s, TSIZE l){return lua_pushlstring(m_state,s,l);};
		TVOID  Lua_pushstring (const TCHAR* s){return lua_pushstring(m_state,s);};
		const TCHAR* Lua_pushvfstring (const TCHAR* fmt,va_list argp){return lua_pushvfstring(m_state,fmt,argp);};
		const TCHAR* Lua_pushfstring (const TCHAR* fmt, ...)
		{
			va_list args;
			va_start( args, fmt );
			const TCHAR* str = Lua_pushvfstring(fmt,args);
			va_end(args);
			return str;
		};
		TVOID  Lua_pushcclosure (lua_CFunction fn, TS32 n){return lua_pushcclosure(m_state,fn,n);};
		TVOID  Lua_pushboolean (TS32 b){return lua_pushboolean(m_state,b);};
		TVOID  Lua_pushlightuserdata (TVOID *p){return lua_pushlightuserdata(m_state,p);};


		/*
		** get functions (Lua -> stack)
		*/
		TVOID  Lua_gettable (TS32 idx){return lua_gettable(m_state,idx);};
		TVOID  Lua_rawget (TS32 idx){return lua_rawget(m_state,idx);};
		TVOID  Lua_rawgeti (TS32 idx, TS32 n){return lua_rawgeti(m_state,idx,n);};
		TVOID  Lua_newtable (){return lua_newtable(m_state);};
		TVOID *Lua_newuserdata (TSIZE sz){return lua_newuserdata(m_state,sz);};
		TS32   Lua_getmetatable (TS32 objindex){return lua_getmetatable(m_state,objindex);};
		TVOID  Lua_getfenv (TS32 idx){return lua_getfenv(m_state,idx);};


		/*
		** set functions (stack -> Lua)
		*/
		TVOID  Lua_settable (TS32 idx){return lua_settable(m_state,idx);};
		TVOID  Lua_rawset (TS32 idx){return lua_rawset(m_state,idx);};
		TVOID  Lua_rawseti (TS32 idx, TS32 n){return lua_rawseti(m_state,idx,n);};
		TS32   Lua_setmetatable (TS32 objindex){return lua_setmetatable(m_state,objindex);};
		TS32   Lua_setfenv (TS32 idx){return lua_setfenv(m_state,idx);};

		// ---------------------------------------------------------------------
	};

}

#endif