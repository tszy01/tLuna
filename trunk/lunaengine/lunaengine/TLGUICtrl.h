#pragma once

#include "TLGUIDefine.h"
namespace TLunaEngine{

	class TLGUIContainer;
	/*
	 *	GUI�ؼ����࣬��������
	 */
	class TLGUICtrl
	{
	public:
		TLGUICtrl(void);
		virtual ~TLGUICtrl(void);
	protected:
		GUI_CTRL_TYPE m_eCtrlType;	// �ؼ�����
		int m_iIndex;				// �ؼ�������������
		TLGUIContainer* m_pContainer;	// ָ������
		bool m_bShow;				// �Ƿ���ʾ
		float m_fAlpha;				// ALPHAֵ

		// ------ �����������������λ�úʹ�С --------
		LONG m_posX;
		LONG m_posY;
		LONG m_width;
		LONG m_height;
		// ------ �����Ǽ�����ľ���λ�úʹ�С ----------
		LONG m_posXFinal;
		LONG m_posYFinal;
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
		bool InitCtrlBase(int iIndex,TLGUIContainer* pContainer,LONG x,LONG y,LONG width,LONG height);
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
		void ReCalSubRect(LONG containerFinalX,LONG containerFinalY);
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