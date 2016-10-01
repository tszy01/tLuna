#include "TLResMgr.h"
#include "TLLog.h"
#include <crtdbg.h>

namespace TLunaEngine{
	ResMgr* Singleton<ResMgr>::m_Ptr = 0;

	ResMgr::ResMgr(TVOID) : m_eListState(RES_LIST_STATE_NONE)
	{
	}

	ResMgr::~ResMgr(TVOID)
	{
		// 这个链表的资源要在外部删除
		List<ResObject*>::Iterator itr = m_ResObjList.begin();
		for(;itr!=m_ResObjList.end();itr++)
		{
			ResObject* pObj = (*itr);
			pObj = 0;
		}
		m_ResObjList.clear();
	}

	TVOID ResMgr::OperateList()
	{
		if(m_eListState!=RES_LIST_STATE_NONE)
			return ;
		if(m_ResObjList.size()<=0)
			return ;
		m_eListState = RES_LIST_STATE_OPERATING;
		// 迭代操作
		List<ResObject*>::Iterator itr = m_ResObjList.begin();
		for(;itr!=m_ResObjList.end();itr++)
		{
			ResObject* pObj = (*itr);
			RES_OBJ_STATE eState = pObj->GetResState();
			switch(eState)
			{
			case RES_OBJ_STATE_WAIT_LOAD:			// 等待加载
				{
					if(!pObj->InitResObject())
					{
						TLunaEngine::Log::WriteLine(TLunaEngine::Log::LOG_LEVEL_ERROR,TTRUE,__FILE__,__LINE__,"InitResObject Error!");
						_ASSERT(0);
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