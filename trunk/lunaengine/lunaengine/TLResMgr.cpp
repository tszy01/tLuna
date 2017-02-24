#include "TLResMgr.h"
#include "TSLog.h"
#include <crtdbg.h>

TLunaEngine::ResMgr* TSun::Singleton<TLunaEngine::ResMgr>::m_Ptr = 0;

namespace TLunaEngine{

	ResMgr::ResMgr(TSun::TVOID) : m_eListState(RES_LIST_STATE_NONE)
	{
	}

	ResMgr::~ResMgr(TSun::TVOID)
	{
		// 这个链表的资源要在外部删除
		TSun::List<ResObject*>::Iterator itr = m_ResObjList.begin();
		for(;itr!=m_ResObjList.end();itr++)
		{
			ResObject* pObj = (*itr);
			pObj = 0;
		}
		m_ResObjList.clear();
	}

	TSun::TVOID ResMgr::OperateList()
	{
		if(m_eListState!=RES_LIST_STATE_NONE)
			return ;
		if(m_ResObjList.size()<=0)
			return ;
		m_eListState = RES_LIST_STATE_OPERATING;
		// 迭代操作
		TSun::List<ResObject*>::Iterator itr = m_ResObjList.begin();
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
						TSun::Log::WriteLine(TSun::Log::LOG_LEVEL_ERROR,TSun::TTRUE,L"InitResObject Error!",__FILE__,__LINE__);
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