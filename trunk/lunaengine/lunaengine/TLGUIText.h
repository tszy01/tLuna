#ifndef _TLGUITEXT_H_
#define _TLGUITEXT_H_
#include "TLGUICtrl.h"
#include "TLString.h"
#include "TLVector4.h"

namespace TLunaEngine{

	template class Vector4<TF32>;

	/*
	*	处理文字的控件
	*/
	class GUIText : public GUICtrl
	{
	public:
		GUIText(TVOID);
		virtual ~GUIText(TVOID);
	protected:
		TS32 m_iFontID;	// 使用的字体的ID
		Vector4<TF32> m_color;	// 文字渲染的颜色
		String m_strText;		// 文字内容
	public:
		// ------- 以下是重写父类方法 ------------
		// 销毁
		virtual TVOID DestroyCtrl();
		// 更新
		virtual TBOOL Update(TF32 fTimeElapsed);
		// 渲染
		virtual TBOOL Render(TF32 fTimeElapsed);
	public:
		// ------ 以下是本类方法 ----------------
		// 初始化
		TBOOL InitGUIText(TS32 iIndex,GUIContainer* pContainer,TS32 x,TS32 y,TS32 width,TS32 height,TS32 iFontID,Vector4<TF32>& color);
		// 设置文字内容
		inline TVOID SetText(const TCHAR* pText)
		{
			m_strText = String(pText);
		}
		// 设置文字颜色
		inline TVOID SetColor(Vector4<TF32>& color)
		{
			m_color = color;
		}
		// 设置ALPHA
		virtual TVOID SetAlpha(TF32 fAlpha)
		{
			GUICtrl::SetAlpha(fAlpha);
			m_color.W = fAlpha;
		}
	};

}

#endif