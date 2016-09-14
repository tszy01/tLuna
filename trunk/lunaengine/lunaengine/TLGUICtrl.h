#pragma once

#include "TLGUIDefine.h"
namespace TLunaEngine{

	class TLGUIContainer;
	/*
	 *	GUI控件父类，用于派生
	 */
	class TLGUICtrl
	{
	public:
		TLGUICtrl(void);
		virtual ~TLGUICtrl(void);
	protected:
		GUI_CTRL_TYPE m_eCtrlType;	// 控件类型
		int m_iIndex;				// 控件在容器中索引
		TLGUIContainer* m_pContainer;	// 指向容器
		bool m_bShow;				// 是否显示
		float m_fAlpha;				// ALPHA值

		// ------ 以下是相对于容器的位置和大小 --------
		LONG m_posX;
		LONG m_posY;
		LONG m_width;
		LONG m_height;
		// ------ 以下是计算出的绝对位置和大小 ----------
		LONG m_posXFinal;
		LONG m_posYFinal;
	public:
		// ------ 以下是需要子类重写方法 --------------
		// 销毁
		virtual void DestroyCtrl();
		// 更新
		virtual bool Update(float fTimeElapsed);
		// 渲染
		virtual bool Render(float fTimeElapsed);
		// 设置ALPHA
		virtual void SetAlpha(float fAlpha)
		{
			m_fAlpha = fAlpha;
		}
	protected:
		// ------ 以下为子类调用 ----------------------
		// 初始化
		// 子类在初始化中必须调用此函数
		bool InitCtrlBase(int iIndex,TLGUIContainer* pContainer,LONG x,LONG y,LONG width,LONG height);
	public:
		// 得到索引
		inline int GetIndex()
		{
			return m_iIndex;
		}
		// 类型
		inline GUI_CTRL_TYPE GetType()
		{
			return m_eCtrlType;
		}
		// 重新计算子控件绝对位置
		void ReCalSubRect(LONG containerFinalX,LONG containerFinalY);
		// 设置是否显示
		inline void ShowCtrl(bool bShow = true)
		{
			m_bShow = bShow;
		}
		// 是否显示
		inline bool IsShow()
		{
			return m_bShow;
		}
	};

}