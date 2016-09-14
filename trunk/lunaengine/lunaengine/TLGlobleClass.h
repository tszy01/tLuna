#pragma once

#include "TString.h"
//#include "TD3DFont.h"
#include "TSingleton.h"

using namespace TLunaEngine;

namespace TLunaEngine{

	// ������
	class TLTriangle;
	/*
	 *	����洢ȫ�־�̬��������̬����
	 */
	class TLGlobleClass
	{
	T_SINGLETON_DEF(TLGlobleClass);
	public:
		TLGlobleClass(void);
		~TLGlobleClass(void);
	public:
		// ��ʱ�� -----------------------
		TLTriangle* m_pTri;
		// ������Դ��·��
		TLunaEngine::TString m_strResDir;
		// DEBUG��Ϣʹ������
		//TLunaEngine::TD3DFont* m_pDebugFont;
		// �Ƿ���ʾDEBUG��Ϣ
		bool m_bShowDebugInfo;
		// �Ƿ�Ϊ�༭��
		bool m_bEditor;
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
	};

}