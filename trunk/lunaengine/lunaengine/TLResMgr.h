#ifndef _TLRESMGR_H_
#define _TLRESMGR_H_

#include "TSSingleton.h"
#include "TLResObject.h"
#include "TSList.h"
namespace TLunaEngine{

	// 链表状态
	enum RES_LIST_STATE
	{
		RES_LIST_STATE_NONE	= 0,	// 无状态
		RES_LIST_STATE_OPERATING= 1,// 正在循环操作
		RES_LIST_STATE_ADDING	= 2,// 正在添加元素
	};

	/*
	 *	负责维护一个全局资源链表，这个链表存储整个场景的资源物体
	 *	场景创建时全部加入链表，场景销毁时全部去除
	 */
	class ResMgr : public TSun::Singleton<ResMgr>
	{
		friend class TSun::Singleton<ResMgr>;
	protected:
		ResMgr(TSun::TVOID);
		~ResMgr(TSun::TVOID);
	private:
		// ------------- 成员 ---------------------
		TSun::List<ResObject*> m_ResObjList;	// 链表
		RES_LIST_STATE m_eListState;		// 当前状态
	public:
		// ------------- 调用方法 -----------------
		// 添加到链表里面
		// 这个TSun::List会在外部组装好
		inline TSun::TBOOL SetList(TSun::List<ResObject*>& list)
		{
			if(m_eListState!=RES_LIST_STATE_NONE)
				return TSun::TFALSE;
			m_eListState = RES_LIST_STATE_ADDING;
			m_ResObjList = list;
			m_eListState = RES_LIST_STATE_NONE;
			return TSun::TTRUE;
		}
		// 得到当前状态
		inline RES_LIST_STATE GetListState(){return m_eListState;}
		// 迭代链表进行操作
		TSun::TVOID OperateList();
	};

}

#endif