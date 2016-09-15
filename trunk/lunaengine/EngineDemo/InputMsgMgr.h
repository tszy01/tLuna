#ifndef _INPUTMSGMGR_H_
#define _INPUTMSGMGR_H_

#include "KeyCodeDef.h"
#include <list>

/*
*	消息接收处理类，该类负责接收GameInput传递过来的各种消息
*/
class InputMsgMgr
{
public:
	InputMsgMgr(void);
	~InputMsgMgr(void);
public:
	// 回调函数接受消息
	static void CatchInputMsg(unsigned char yType, void* param);
	// 接收消息的函数指针
	static std::list<ProcessInput> m_CalledFuncList;
};

#endif