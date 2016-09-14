#pragma once
#include "TLGUICtrl.h"

namespace TLunaEngine{
	/*
	 *	����ͼƬ�Ŀؼ�
	 */
	class TLGUIPicture : public TLGUICtrl
	{
	public:
		TLGUIPicture(void);
		virtual ~TLGUIPicture(void);
	protected:
		int m_iTexID;	// ʹ�õ������ID
		// ------------ ����������ӳ�䲿�� -------
		float m_texX;
		float m_texY;
		float m_texR;
		float m_texB;
	public:
		// ------- ��������д���෽�� ------------
		// ����
		virtual void DestroyCtrl();
		// ����
		virtual bool Update(float fTimeElapsed);
		// ��Ⱦ
		virtual bool Render(float fTimeElapsed);
	public:
		// ------ �����Ǳ��෽�� ----------------
		// ��ʼ��
		bool InitGUIPicture(int iIndex,TLGUIContainer* pContainer,LONG x,LONG y,LONG width,LONG height,int iTexID,float texX,float texY,float texR,float texB);
	};

}