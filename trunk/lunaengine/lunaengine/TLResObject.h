#ifndef _TLRESOBJECT_H_
#define _TLRESOBJECT_H_

#include "TSCommonTypeDef.h"
namespace TLunaEngine{

	/*
	 *	资源物体状态
	 */
	enum RES_OBJ_STATE
	{
		RES_OBJ_STATE_NONE	= 0,		// 当前没有任何状态
		RES_OBJ_STATE_WAIT_LOAD	= 1,	// 当前等待加载
		RES_OBJ_STATE_WAIT_DESTROY	= 2,// 当前等待销毁
		RES_OBJ_STATE_LOADING	= 3,	// 当前正在加载
		RES_OBJ_STATE_DESTROYING	= 4,// 当前正在销毁
		RES_OBJ_STATE_WAIT_DELETE	= 5,// 当前等待从链表中删除
		RES_OBJ_STATE_HAS_LOADED	= 6,// 当前加载完成
		RES_OBJ_STATE_HAS_DESTROYED	= 7,// 当前已经销毁
	};

	/*
	 *	资源物体接口
	 *	所有游戏中可以看到的实体都是一个资源物体，本类提供接口用于加载和释放
	 */
	class ResObject
	{
	public:
		ResObject(TSun::TVOID);
		virtual ~ResObject(TSun::TVOID);
	public:
		// --------------- 需要重写的方法 -------------------------

		// 加载资源
		virtual TSun::TBOOL InitResObject() = 0;
		// 释放资源
		virtual TSun::TVOID DestroyResObject() = 0;

	public:
		// --------------- 外部可调用方法 -------------------------
		// 得到当前状态
		inline RES_OBJ_STATE GetResState(){return m_eResState;}
	protected:
		// --------------- 子类调用方法 ---------------------------
		// 设置当前状态
		inline TSun::TVOID SetResState(RES_OBJ_STATE eResState){m_eResState = eResState;}
	private:
		// --------------- 成员 -----------------------------------
		RES_OBJ_STATE m_eResState;	// 资源当前状态
	};

}

#endif