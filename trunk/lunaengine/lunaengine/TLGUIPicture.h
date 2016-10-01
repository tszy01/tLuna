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
		TS32 m_iTexID;	// 使用的纹理的ID
		// ------------ 以下是纹理映射部分 -------
		TF32 m_texX;
		TF32 m_texY;
		TF32 m_texR;
		TF32 m_texB;
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
		TBOOL InitGUIPicture(TS32 iIndex,GUIContainer* pContainer,TS32 x,TS32 y,TS32 width,TS32 height,TS32 iTexID,TF32 texX,TF32 texY,TF32 texR,TF32 texB);
	};

}

#endif