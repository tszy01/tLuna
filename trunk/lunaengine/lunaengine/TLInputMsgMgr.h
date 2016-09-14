#pragma once

#include "KeyCodeDef.h"
#include <list>
using namespace std;

namespace TLunaEngine{

	/*
	 *	��Ϣ���մ����࣬���ฺ�����GameInput���ݹ����ĸ�����Ϣ
	 */
	class TLInputMsgMgr
	{
	public:
		TLInputMsgMgr(void);
		~TLInputMsgMgr(void);
	public:
		// �ص�����������Ϣ
		static void CatchInputMsg(BYTE yType,void* param);
		// ������Ϣ�ĺ���ָ��
		static std::list<ProcessInput> m_CalledFuncList;
	};

}