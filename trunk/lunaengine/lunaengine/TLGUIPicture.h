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
		GUIPicture(void);
		virtual ~GUIPicture(void);
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
		bool InitGUIPicture(int iIndex,GUIContainer* pContainer,TS32 x,TS32 y,TS32 width,TS32 height,int iTexID,float texX,float texY,float texR,float texB);
	};

}

#endif