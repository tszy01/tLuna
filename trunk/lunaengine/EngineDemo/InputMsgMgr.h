#ifndef _INPUTMSGMGR_H_
#define _INPUTMSGMGR_H_

#include "KeyCodeDef.h"
#include <list>

/*
*	��Ϣ���մ����࣬���ฺ�����GameInput���ݹ����ĸ�����Ϣ
*/
class InputMsgMgr
{
public:
	InputMsgMgr(void);
	~InputMsgMgr(void);
public:
	// �ص�����������Ϣ
	static void CatchInputMsg(unsigned char yType, void* param);
	// ������Ϣ�ĺ���ָ��
	static std::list<ProcessInput> m_CalledFuncList;
};

#endif