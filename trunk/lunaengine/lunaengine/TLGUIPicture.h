#pragma once
#include "TLGUICtrl.h"

namespace TLunaEngine{
	/*
	 *	处理图片的控件
	 */
	class TLGUIPicture : public TLGUICtrl
	{
	public:
		TLGUIPicture(void);
		virtual ~TLGUIPicture(void);
	protected:
		int m_iTexID;	// 使用的纹理的ID
		// ------------ 以下是纹理映射部分 -------
		float m_texX;
		float m_texY;
		float m_texR;
		float m_texB;
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
		bool InitGUIPicture(int iIndex,TLGUIContainer* pContainer,LONG x,LONG y,LONG width,LONG height,int iTexID,float texX,float texY,float texR,float texB);
	};

}