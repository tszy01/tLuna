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
		GUIPicture(TSun::TVOID);
		virtual ~GUIPicture(TSun::TVOID);
	protected:
		TSun::TS32 m_iTexID;	// 使用的纹理的ID
		// ------------ 以下是纹理映射部分 -------
		TSun::TF32 m_texX;
		TSun::TF32 m_texY;
		TSun::TF32 m_texR;
		TSun::TF32 m_texB;
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
		TSun::TBOOL InitGUIPicture(TSun::TS32 iIndex,GUIContainer* pContainer,TSun::TS32 x,TSun::TS32 y,TSun::TS32 width,TSun::TS32 height,TSun::TS32 iTexID,TSun::TF32 texX,TSun::TF32 texY,TSun::TF32 texR,TSun::TF32 texB);
	};

}

#endif