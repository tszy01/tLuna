#pragma once

#include "TSingleton.h"
#include "TLIResObject.h"
using namespace TLunaEngine;
#include <list>
using namespace std;
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
	class TLResMgr
	{
	T_SINGLETON_DEF(TLResMgr);
	public:
		TLResMgr(void);
		~TLResMgr(void);
	private:
		// ------------- 成员 ---------------------
		std::list<TLIResObject*> m_ResObjList;	// 链表
		RES_LIST_STATE m_eListState;		// 当前状态
	public:
		// ------------- 调用方法 -----------------
		// 添加到链表里面
		// 这个List会在外部组装好
		inline bool SetList(std::list<TLIResObject*>& list)
		{
			if(m_eListState!=RES_LIST_STATE_NONE)
				return false;
			m_eListState = RES_LIST_STATE_ADDING;
			m_ResObjList = list;
			m_eListState = RES_LIST_STATE_NONE;
			return true;
		}
		// 得到当前状态
		inline RES_LIST_STATE GetListState(){return m_eListState;}
		// 迭代链表进行操作
		void OperateList();
	};

}