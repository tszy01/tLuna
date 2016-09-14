#pragma once
#include "TLGUICtrl.h"
#include "TString.h"
#include "TVector4.h"
using namespace TLunaEngine;
using namespace TLunaEngine;

namespace TLunaEngine{

	template class TVector4<float>;

	/*
	*	�������ֵĿؼ�
	*/
	class TLGUIText : public TLGUICtrl
	{
	public:
		TLGUIText(void);
		virtual ~TLGUIText(void);
	protected:
		int m_iFontID;	// ʹ�õ������ID
		TVector4<float> m_color;	// ������Ⱦ����ɫ
		TString m_strText;		// ��������
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
		bool InitGUIText(int iIndex,TLGUIContainer* pContainer,LONG x,LONG y,LONG width,LONG height,int iFontID,TVector4<float>& color);
		// ������������
		inline void SetText(const char* pText)
		{
			m_strText = TString(pText);
		}
		// ����������ɫ
		inline void SetColor(TVector4<float>& color)
		{
			m_color = color;
		}
		// ����ALPHA
		virtual void SetAlpha(float fAlpha)
		{
			TLGUICtrl::SetAlpha(fAlpha);
			m_color.W = fAlpha;
		}
	};

}