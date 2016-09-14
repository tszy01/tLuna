#include "StdAfx.h"
#include "TLResMgr.h"
#include "TLog.h"
#include <crtdbg.h>

namespace TLunaEngine{
	T_SINGLETON_IMP(TLResMgr);

	TLResMgr::TLResMgr(void) : m_eListState(RES_LIST_STATE_NONE)
	{
	}

	TLResMgr::~TLResMgr(void)
	{
		// 这个链表的资源要在外部删除
		std::list<TLIResObject*>::iterator itr = m_ResObjList.begin();
		for(;itr!=m_ResObjList.end();itr++)
		{
			TLIResObject* pObj = (*itr);
			pObj = NULL;
		}
		m_ResObjList.clear();
	}

	void TLResMgr::OperateList()
	{
		if(m_eListState!=RES_LIST_STATE_NONE)
			return ;
		if(m_ResObjList.size()<=0)
			return ;
		m_eListState = RES_LIST_STATE_OPERATING;
		// 迭代操作
		std::list<TLIResObject*>::iterator itr = m_ResObjList.begin();
		for(;itr!=m_ResObjList.end();itr++)
		{
			TLIResObject* pObj = (*itr);
			RES_OBJ_STATE eState = pObj->GetResState();
			switch(eState)
			{
			case RES_OBJ_STATE_WAIT_LOAD:			// 等待加载
				{
					if(!pObj->InitResObject())
					{
						TLunaEngine::TLog::WriteLine(TLunaEngine::TLog::LOG_LEVEL_ERROR,true,__FILE__,__LINE__,"InitResObject Error!");
						_ASSERT(FALSE);
					}
				}
				break;
			case RES_OBJ_STATE_WAIT_DESTROY:		// 等待销毁
				{
					pObj->DestroyResObject();
				}
				break;
			default:
				{
				}
				break;
			}
		}
		// 清空链表
		m_ResObjList.clear();
		m_eListState = RES_LIST_STATE_NONE;
	}

}