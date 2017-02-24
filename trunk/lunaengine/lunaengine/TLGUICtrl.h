#ifndef _TLGUICTRL_H_
#define _TLGUICTRL_H_

#include "TLGUIDefine.h"
namespace TLunaEngine{

	class GUIContainer;
	/*
	 *	GUI控件父类，用于派生
	 */
	class GUICtrl
	{
	public:
		GUICtrl(TSun::TVOID);
		virtual ~GUICtrl(TSun::TVOID);
	protected:
		GUI_CTRL_TYPE m_eCtrlType;	// 控件类型
		TSun::TS32 m_iIndex;				// 控件在容器中索引
		GUIContainer* m_pContainer;	// 指向容器
		TSun::TBOOL m_bShow;				// 是否显示
		TSun::TF32 m_fAlpha;				// ALPHA值

		// ------ 以下是相对于容器的位置和大小 --------
		TSun::TS32 m_posX;
		TSun::TS32 m_posY;
		TSun::TS32 m_width;
		TSun::TS32 m_height;
		// ------ 以下是计算出的绝对位置和大小 ----------
		TSun::TS32 m_posXFinal;
		TSun::TS32 m_posYFinal;
	public:
		// ------ 以下是需要子类重写方法 --------------
		// 销毁
		virtual TSun::TVOID DestroyCtrl();
		// 更新
		virtual TSun::TBOOL Update(TSun::TF32 fTimeElapsed);
		// 渲染
		virtual TSun::TBOOL Render(TSun::TF32 fTimeElapsed);
		// 设置ALPHA
		virtual TSun::TVOID SetAlpha(TSun::TF32 fAlpha)
		{
			m_fAlpha = fAlpha;
		}
	protected:
		// ------ 以下为子类调用 ----------------------
		// 初始化
		// 子类在初始化中必须调用此函数
		TSun::TBOOL InitCtrlBase(TSun::TS32 iIndex,GUIContainer* pContainer,TSun::TS32 x,TSun::TS32 y,TSun::TS32 width,TSun::TS32 height);
	public:
		// 得到索引
		inline TSun::TS32 GetIndex()
		{
			return m_iIndex;
		}
		// 类型
		inline GUI_CTRL_TYPE GetType()
		{
			return m_eCtrlType;
		}
		// 重新计算子控件绝对位置
		TSun::TVOID ReCalSubRect(TSun::TS32 containerFinalX,TSun::TS32 containerFinalY);
		// 设置是否显示
		inline TSun::TVOID ShowCtrl(TSun::TBOOL bShow = TSun::TTRUE)
		{
			m_bShow = bShow;
		}
		// 是否显示
		inline TSun::TBOOL IsShow()
		{
			return m_bShow;
		}
	};

}

#endif