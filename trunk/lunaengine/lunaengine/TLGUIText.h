#pragma once
#include "TLGUICtrl.h"
#include "TString.h"
#include "TVector4.h"
using namespace TLunaEngine;
using namespace TLunaEngine;

namespace TLunaEngine{

	template class TVector4<float>;

	/*
	*	处理文字的控件
	*/
	class TLGUIText : public TLGUICtrl
	{
	public:
		TLGUIText(void);
		virtual ~TLGUIText(void);
	protected:
		int m_iFontID;	// 使用的字体的ID
		TVector4<float> m_color;	// 文字渲染的颜色
		TString m_strText;		// 文字内容
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
		bool InitGUIText(int iIndex,TLGUIContainer* pContainer,LONG x,LONG y,LONG width,LONG height,int iFontID,TVector4<float>& color);
		// 设置文字内容
		inline void SetText(const char* pText)
		{
			m_strText = TString(pText);
		}
		// 设置文字颜色
		inline void SetColor(TVector4<float>& color)
		{
			m_color = color;
		}
		// 设置ALPHA
		virtual void SetAlpha(float fAlpha)
		{
			TLGUICtrl::SetAlpha(fAlpha);
			m_color.W = fAlpha;
		}
	};

}