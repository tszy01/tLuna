#ifndef _TLGLOBLECLASS_H_
#define _TLGLOBLECLASS_H_

#include "TLString.h"
//#include "TD3DFont.h"
#include "TLSingleton.h"

namespace TLunaEngine{

	// ������
	class TestTriangle;
	class LoopCtrl;
	/*
	 *	����洢ȫ�־�̬��������̬����
	 */
	class GlobleClass : public Singleton<GlobleClass>
	{
		friend class Singleton<GlobleClass>;
	protected:
		GlobleClass(void);
		~GlobleClass(void);
	public:
		// ��ʱ�� -----------------------
		TestTriangle* m_pTri;
		// ������Դ��·��
		TLunaEngine::String m_strResDir;
		// DEBUG��Ϣʹ������
		//TLunaEngine::TD3DFont* m_pDebugFont;
		// �Ƿ���ʾDEBUG��Ϣ
		bool m_bShowDebugInfo;
		// �Ƿ�Ϊ�༭��
		bool m_bEditor;
		// loop control
		LoopCtrl* mLoopCtrl;
	public:
		// ----- ���� -------------------
		// ��ʼ��
		bool InitGlobleClass();
		// ����
		void DestroyGlobleClass();
		// ֡����
		bool OnUpdate(float fTimeElapsed);
		// ֡��Ⱦ
		bool OnRender(float fTimeElapsed);
		// update loop control
		void updateLoopCtrl();
		// get loop control renderable
		bool getLoopCtrlCanRender();
		// calc fps
		void calcFPS();
		// get elapsed time
		float getElapsedTime();
	};

}

#endif