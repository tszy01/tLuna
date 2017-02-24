#ifndef _TLGUITEXT_H_
#define _TLGUITEXT_H_
#include "TLGUICtrl.h"
#include "TSString.h"
#include "TSVector4.h"

namespace TLunaEngine{

	template class TSun::Vector4<TSun::TF32>;

	/*
	*	处理文字的控件
	*/
	class GUIText : public GUICtrl
	{
	public:
		GUIText(TSun::TVOID);
		virtual ~GUIText(TSun::TVOID);
	protected:
		TSun::TS32 m_iFontID;	// 使用的字体的ID
		TSun::Vector4<TSun::TF32> m_color;	// 文字渲染的颜色
		TSun::WString m_strText;		// 文字内容
		TSun::TU64 m_textID;	// text id in system language dictionary
	public:
		// ------- 以下是重写父类方法 ------------
		// 销毁
		virtual TSun::TVOID DestroyCtrl();
		// 更新
		virtual TSun::TBOOL Update(TSun::TF32 fTimeElapsed);
		// 渲染
		virtual TSun::TBOOL Render(TSun::TF32 fTimeElapsed);
	public:
		// ------ 以下是本类方法 ----------------
		// 初始化
		TSun::TBOOL InitGUIText(TSun::TS32 iIndex,GUIContainer* pContainer,TSun::TS32 x,TSun::TS32 y,TSun::TS32 width,TSun::TS32 height,TSun::TS32 iFontID,TSun::Vector4<TSun::TF32>& color,TSun::TU64 textID);
		// 设置文字内容
		inline TSun::TVOID SetText(const TSun::TWCHAR* pText)
		{
			m_strText = TSun::WString(pText);
		}
		// 设置文字颜色
		inline TSun::TVOID SetColor(TSun::Vector4<TSun::TF32>& color)
		{
			m_color = color;
		}
		// 设置ALPHA
		virtual TSun::TVOID SetAlpha(TSun::TF32 fAlpha)
		{
			GUICtrl::SetAlpha(fAlpha);
			m_color.W = fAlpha;
		}
	};

}

#endif