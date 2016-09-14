#include "StdAfx.h"
#include "TLInputMsgMgr.h"

std::list<TLunaEngine::ProcessInput> TLunaEngine::TLInputMsgMgr::m_CalledFuncList/* = std::list<TLunaEngine::ProcessInput>()*/;

namespace TLunaEngine{

	TLInputMsgMgr::TLInputMsgMgr(void)
	{
	}

	TLInputMsgMgr::~TLInputMsgMgr(void)
	{
	}

	void TLInputMsgMgr::CatchInputMsg(BYTE yType, void *param)
	{
		// 迭代调用函数指针
		std::list<ProcessInput>::iterator itr = m_CalledFuncList.begin();
		for(;itr!=m_CalledFuncList.end();++itr)
		{
			ProcessInput p = (*itr);
			p(yType,param);
		}
	}

}