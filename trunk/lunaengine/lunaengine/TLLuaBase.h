#ifndef _TLLUABASE_H_
#define _TLLUABASE_H_

extern "C"{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <iostream> /*unlike most LUA examples I've found we will be using c++ */
#include <string>

namespace TLunaEngine{

	// Lua上层封装类,继承本类,处理特定的脚本
	// 涉及到的lua基础库以及luabind库的函数,推荐直接使用库函数

	class LuaBase
	{
	public:
		LuaBase(void);
	public:
		virtual ~LuaBase(void);

	protected:
		// 成员变量
		lua_State *m_state;			// lua state
	public:
		// 成员方法
		bool InitLuaBase(const char *luaFile);
		void SetVariable(char *varName);
		void GetVariable(char *varName);
		void CloseLuaBase();
		void Pop(int n);
		// C++调用lua函数
		// 如果是用lua基本库调用，请使用本函数，并且在调用之后需要pop出N个返回值
		bool CallFunction(int nargs,int nresults);
		inline lua_State* GetState(){return m_state;}

	protected:

		// ---------------------------- 扩展lua全局函数 ------------------------

		/*
		** access functions (stack -> C)
		*/

		int				Lua_isnumber (int idx){return lua_isnumber(m_state,idx);};
		int             Lua_isstring (int idx){return lua_isstring(m_state,idx);};
		int             Lua_iscfunction (int idx){return lua_iscfunction(m_state,idx);};
		int             Lua_isuserdata (int idx){return lua_isuserdata(m_state,idx);};
		int             Lua_type (int idx){return lua_type(m_state,idx);};
		const char		*Lua_typename (int tp){return lua_typename(m_state,tp);};

		int				Lua_equal (int idx1, int idx2){return lua_equal(m_state,idx1,idx2);};
		int				Lua_rawequal (int idx1, int idx2){return lua_rawequal(m_state,idx1,idx2);};
		int				Lua_lessthan (int idx1, int idx2){return lua_lessthan(m_state,idx1,idx2);};

		lua_Number      Lua_tonumber (int idx){return lua_tonumber(m_state,idx);};
		int             Lua_toboolean (int idx){return lua_toboolean(m_state,idx);};
		const char		*Lua_tostring (int idx){return lua_tostring(m_state,idx);};
		size_t          Lua_strlen (int idx){return lua_strlen(m_state,idx);};
		lua_CFunction   Lua_tocfunction (int idx){return lua_tocfunction(m_state,idx);};
		void			*Lua_touserdata (int idx){return lua_touserdata(m_state,idx);};
		lua_State		*Lua_tothread (int idx){return lua_tothread(m_state,idx);};
		const void		*Lua_topointer (int idx){return lua_topointer(m_state,idx);};


		/*
		** push functions (C -> stack)
		*/
		void  Lua_pushnil (){return lua_pushnil(m_state);};
		void  Lua_pushnumber (lua_Number n){return lua_pushnumber(m_state,n);};
		void  Lua_pushlstring (const char *s, size_t l){return lua_pushlstring(m_state,s,l);};
		void  Lua_pushstring (const char *s){return lua_pushstring(m_state,s);};
		const char *Lua_pushvfstring (const char *fmt,va_list argp){return lua_pushvfstring(m_state,fmt,argp);};
		const char *Lua_pushfstring (const char *fmt, ...)
		{
			va_list args;
			va_start( args, fmt );
			const char* str = Lua_pushvfstring(fmt,args);
			va_end(args);
			return str;
		};
		void  Lua_pushcclosure (lua_CFunction fn, int n){return lua_pushcclosure(m_state,fn,n);};
		void  Lua_pushboolean (int b){return lua_pushboolean(m_state,b);};
		void  Lua_pushlightuserdata (void *p){return lua_pushlightuserdata(m_state,p);};


		/*
		** get functions (Lua -> stack)
		*/
		void  Lua_gettable (int idx){return lua_gettable(m_state,idx);};
		void  Lua_rawget (int idx){return lua_rawget(m_state,idx);};
		void  Lua_rawgeti (int idx, int n){return lua_rawgeti(m_state,idx,n);};
		void  Lua_newtable (){return lua_newtable(m_state);};
		void *Lua_newuserdata (size_t sz){return lua_newuserdata(m_state,sz);};
		int   Lua_getmetatable (int objindex){return lua_getmetatable(m_state,objindex);};
		void  Lua_getfenv (int idx){return lua_getfenv(m_state,idx);};


		/*
		** set functions (stack -> Lua)
		*/
		void  Lua_settable (int idx){return lua_settable(m_state,idx);};
		void  Lua_rawset (int idx){return lua_rawset(m_state,idx);};
		void  Lua_rawseti (int idx, int n){return lua_rawseti(m_state,idx,n);};
		int   Lua_setmetatable (int objindex){return lua_setmetatable(m_state,objindex);};
		int   Lua_setfenv (int idx){return lua_setfenv(m_state,idx);};

		// ---------------------------------------------------------------------
	};

}

#endif