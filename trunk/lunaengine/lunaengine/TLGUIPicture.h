#ifndef _TLGUIPICTURE_H_
#define _TLGUIPICTURE_H_
#include "TLGUICtrl.h"

namespace TLunaEngine{
	/*
	 *	处理图片的控件
	 */
	class GUIPicture : public GUICtrl
	{
	public:
		GUIPicture(TVOID);
		virtual ~GUIPicture(TVOID);
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
		virtual TVOID DestroyCtrl();
		// 更新
		virtual TBOOL Update(float fTimeElapsed);
		// 渲染
		virtual TBOOL Render(float fTimeElapsed);
	public:
		// ------ 以下是本类方法 ----------------
		// 初始化
		TBOOL InitGUIPicture(int iIndex,GUIContainer* pContainer,TS32 x,TS32 y,TS32 width,TS32 height,int iTexID,float texX,float texY,float texR,float texB);
	};

}

#endif