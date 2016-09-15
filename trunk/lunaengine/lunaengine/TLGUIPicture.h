#ifndef _TLGUIPICTURE_H_
#define _TLGUIPICTURE_H_
#include "TLGUICtrl.h"

namespace TLunaEngine{
	/*
	 *	����ͼƬ�Ŀؼ�
	 */
	class GUIPicture : public GUICtrl
	{
	public:
		GUIPicture(void);
		virtual ~GUIPicture(void);
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
		bool InitGUIPicture(int iIndex,GUIContainer* pContainer,TS32 x,TS32 y,TS32 width,TS32 height,int iTexID,float texX,float texY,float texR,float texB);
	};

}

#endif