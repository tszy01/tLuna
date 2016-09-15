#include "TLResMgr.h"
#include "TLLog.h"
#include <crtdbg.h>

namespace TLunaEngine{
	ResMgr* Singleton<ResMgr>::m_Ptr = 0;

	ResMgr::ResMgr(void) : m_eListState(RES_LIST_STATE_NONE)
	{
	}

	ResMgr::~ResMgr(void)
	{
		// ����������ԴҪ���ⲿɾ��
		std::list<ResObject*>::iterator itr = m_ResObjList.begin();
		for(;itr!=m_ResObjList.end();itr++)
		{
			ResObject* pObj = (*itr);
			pObj = 0;
		}
		m_ResObjList.clear();
	}

	void ResMgr::OperateList()
	{
		if(m_eListState!=RES_LIST_STATE_NONE)
			return ;
		if(m_ResObjList.size()<=0)
			return ;
		m_eListState = RES_LIST_STATE_OPERATING;
		// ��������
		std::list<ResObject*>::iterator itr = m_ResObjList.begin();
		for(;itr!=m_ResObjList.end();itr++)
		{
			ResObject* pObj = (*itr);
			RES_OBJ_STATE eState = pObj->GetResState();
			switch(eState)
			{
			case RES_OBJ_STATE_WAIT_LOAD:			// �ȴ�����
				{
					if(!pObj->InitResObject())
					{
						TLunaEngine::Log::WriteLine(TLunaEngine::Log::LOG_LEVEL_ERROR,true,__FILE__,__LINE__,"InitResObject Error!");
						_ASSERT(0);
					}
				}
				break;
			case RES_OBJ_STATE_WAIT_DESTROY:		// �ȴ�����
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
		// �������
		m_ResObjList.clear();
		m_eListState = RES_LIST_STATE_NONE;
	}

}