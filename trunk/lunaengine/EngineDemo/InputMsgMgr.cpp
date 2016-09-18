#include "InputMsgMgr.h"

std::list<ProcessInput> InputMsgMgr::m_CalledFuncList/* = std::list<TLunaEngine::ProcessInput>()*/;

InputMsgMgr::InputMsgMgr(void)
{
}

InputMsgMgr::~InputMsgMgr(void)
{
}

void InputMsgMgr::CatchInputMsg(unsigned char yType, void *param)
{
	// 迭代调用函数指针
	std::list<ProcessInput>::iterator itr = m_CalledFuncList.begin();
	for(;itr!=m_CalledFuncList.end();++itr)
	{
		ProcessInput p = (*itr);
		p(yType,param);
	}
}