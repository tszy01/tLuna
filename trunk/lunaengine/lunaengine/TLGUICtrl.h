#ifndef _TLGUICTRL_H_
#define _TLGUICTRL_H_

#include "TLGUIDefine.h"
namespace TLunaEngine{

	class GUIContainer;
	/*
	 *	GUI�ؼ����࣬��������
	 */
	class GUICtrl
	{
	public:
		GUICtrl(void);
		virtual ~GUICtrl(void);
	protected:
		GUI_CTRL_TYPE m_eCtrlType;	// �ؼ�����
		int m_iIndex;				// �ؼ�������������
		GUIContainer* m_pContainer;	// ָ������
		bool m_bShow;				// �Ƿ���ʾ
		float m_fAlpha;				// ALPHAֵ

		// ------ �����������������λ�úʹ�С --------
		TS32 m_posX;
		TS32 m_posY;
		TS32 m_width;
		TS32 m_height;
		// ------ �����Ǽ�����ľ���λ�úʹ�С ----------
		TS32 m_posXFinal;
		TS32 m_posYFinal;
	public:
		// ------ ��������Ҫ������д���� --------------
		// ����
		virtual void DestroyCtrl();
		// ����
		virtual bool Update(float fTimeElapsed);
		// ��Ⱦ
		virtual bool Render(float fTimeElapsed);
		// ����ALPHA
		virtual void SetAlpha(float fAlpha)
		{
			m_fAlpha = fAlpha;
		}
	protected:
		// ------ ����Ϊ������� ----------------------
		// ��ʼ��
		// �����ڳ�ʼ���б�����ô˺���
		bool InitCtrlBase(int iIndex,GUIContainer* pContainer,TS32 x,TS32 y,TS32 width,TS32 height);
	public:
		// �õ�����
		inline int GetIndex()
		{
			return m_iIndex;
		}
		// ����
		inline GUI_CTRL_TYPE GetType()
		{
			return m_eCtrlType;
		}
		// ���¼����ӿؼ�����λ��
		void ReCalSubRect(TS32 containerFinalX,TS32 containerFinalY);
		// �����Ƿ���ʾ
		inline void ShowCtrl(bool bShow = true)
		{
			m_bShow = bShow;
		}
		// �Ƿ���ʾ
		inline bool IsShow()
		{
			return m_bShow;
		}
	};

}

#endif