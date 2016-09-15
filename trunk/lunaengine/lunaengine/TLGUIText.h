#ifndef _TLGUITEXT_H_
#define _TLGUITEXT_H_
#include "TLGUICtrl.h"
#include "TLString.h"
#include "TLVector4.h"

namespace TLunaEngine{

	template class Vector4<float>;

	/*
	*	�������ֵĿؼ�
	*/
	class GUIText : public GUICtrl
	{
	public:
		GUIText(void);
		virtual ~GUIText(void);
	protected:
		int m_iFontID;	// ʹ�õ������ID
		Vector4<float> m_color;	// ������Ⱦ����ɫ
		String m_strText;		// ��������
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
		bool InitGUIText(int iIndex,GUIContainer* pContainer,TS32 x,TS32 y,TS32 width,TS32 height,int iFontID,Vector4<float>& color);
		// ������������
		inline void SetText(const char* pText)
		{
			m_strText = String(pText);
		}
		// ����������ɫ
		inline void SetColor(Vector4<float>& color)
		{
			m_color = color;
		}
		// ����ALPHA
		virtual void SetAlpha(float fAlpha)
		{
			GUICtrl::SetAlpha(fAlpha);
			m_color.W = fAlpha;
		}
	};

}

#endif