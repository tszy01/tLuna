#ifndef _TLGUITEXT_H_
#define _TLGUITEXT_H_
#include "TLGUICtrl.h"
#include "TLString.h"
#include "TLVector4.h"

namespace TLunaEngine{

	template class Vector4<float>;

	/*
	*	处理文字的控件
	*/
	class GUIText : public GUICtrl
	{
	public:
		GUIText(void);
		virtual ~GUIText(void);
	protected:
		int m_iFontID;	// 使用的字体的ID
		Vector4<float> m_color;	// 文字渲染的颜色
		String m_strText;		// 文字内容
	public:
		// ------- 以下是重写父类方法 ------------
		// 销毁
		virtual void DestroyCtrl();
		// 更新
		virtual bool Update(float fTimeElapsed);
		// 渲染
		virtual bool Render(float fTimeElapsed);
	public:
		// ------ 以下是本类方法 ----------------
		// 初始化
		bool InitGUIText(int iIndex,GUIContainer* pContainer,TS32 x,TS32 y,TS32 width,TS32 height,int iFontID,Vector4<float>& color);
		// 设置文字内容
		inline void SetText(const char* pText)
		{
			m_strText = String(pText);
		}
		// 设置文字颜色
		inline void SetColor(Vector4<float>& color)
		{
			m_color = color;
		}
		// 设置ALPHA
		virtual void SetAlpha(float fAlpha)
		{
			GUICtrl::SetAlpha(fAlpha);
			m_color.W = fAlpha;
		}
	};

}

#endif