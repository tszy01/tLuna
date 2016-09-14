#pragma once

#include "TSingleton.h"
#include "TLIResObject.h"
using namespace TLunaEngine;
#include <list>
using namespace std;
namespace TLunaEngine{

	// ����״̬
	enum RES_LIST_STATE
	{
		RES_LIST_STATE_NONE	= 0,	// ��״̬
		RES_LIST_STATE_OPERATING= 1,// ����ѭ������
		RES_LIST_STATE_ADDING	= 2,// �������Ԫ��
	};

	/*
	 *	����ά��һ��ȫ����Դ�����������洢������������Դ����
	 *	��������ʱȫ������������������ʱȫ��ȥ��
	 */
	class TLResMgr
	{
	T_SINGLETON_DEF(TLResMgr);
	public:
		TLResMgr(void);
		~TLResMgr(void);
	private:
		// ------------- ��Ա ---------------------
		std::list<TLIResObject*> m_ResObjList;	// ����
		RES_LIST_STATE m_eListState;		// ��ǰ״̬
	public:
		// ------------- ���÷��� -----------------
		// ��ӵ���������
		// ���List�����ⲿ��װ��
		inline bool SetList(std::list<TLIResObject*>& list)
		{
			if(m_eListState!=RES_LIST_STATE_NONE)
				return false;
			m_eListState = RES_LIST_STATE_ADDING;
			m_ResObjList = list;
			m_eListState = RES_LIST_STATE_NONE;
			return true;
		}
		// �õ���ǰ״̬
		inline RES_LIST_STATE GetListState(){return m_eListState;}
		// ����������в���
		void OperateList();
	};

}