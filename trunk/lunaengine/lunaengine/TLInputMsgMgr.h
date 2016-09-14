#pragma once

#include "KeyCodeDef.h"
#include <list>
using namespace std;

namespace TLunaEngine{

	/*
	 *	消息接收处理类，该类负责接收GameInput传递过来的各种消息
	 */
	class TLInputMsgMgr
	{
	public:
		TLInputMsgMgr(void);
		~TLInputMsgMgr(void);
	public:
		// 回调函数接受消息
		static void CatchInputMsg(BYTE yType,void* param);
		// 接收消息的函数指针
		static std::list<ProcessInput> m_CalledFuncList;
	};

}