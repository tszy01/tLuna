#include "TLGUIPicture.h"
#include "TLGUITextureMgr.h"

namespace TLunaEngine{
	GUIPicture::GUIPicture(TSun::TVOID) : GUICtrl(),
	m_iTexID(-1),
	m_texX(0),
	m_texY(0),
	m_texR(0),
	m_texB(0)
	{
	}

	GUIPicture::~GUIPicture(TSun::TVOID)
	{
	}

	TSun::TBOOL GUIPicture::InitGUIPicture(TSun::TS32 iIndex,GUIContainer* pContainer,TSun::TS32 x,TSun::TS32 y,TSun::TS32 width,TSun::TS32 height,
									  TSun::TS32 iTexID, TSun::TF32 texX, TSun::TF32 texY, TSun::TF32 texR, TSun::TF32 texB)
	{
		if (!GUICtrl::InitCtrlBase(iIndex,pContainer,x,y,width,height))
		{
			return TSun::TFALSE;
		}
		m_eCtrlType = GUI_CTRL_PICTURE;
		m_iTexID = iTexID;
		m_texX = texX;
		m_texY = texY;
		m_texR = texR;
		m_texB = texB;
		return TSun::TTRUE;
	}

	TSun::TVOID GUIPicture::DestroyCtrl()
	{
		m_eCtrlType = GUI_CTRL_NONE;
		m_iTexID = -1;
		m_texX = 0;
		m_texY = 0;
		m_texR = 0;
		m_texB = 0;
		GUICtrl::DestroyCtrl();
	}

	TSun::TBOOL GUIPicture::Update(TSun::TF32 fTimeElapsed)
	{
		return GUICtrl::Update(fTimeElapsed);
	}

	TSun::TBOOL GUIPicture::Render(TSun::TF32 fTimeElapsed)
	{
		if (!GUICtrl::Render(fTimeElapsed))
		{
			return TSun::TFALSE;
		}
		GUITextureMgr::getSingletonPtr()->DrawGUICtrl(m_posXFinal,m_posYFinal,m_width,m_height,m_texX,m_texY,m_texR,m_texB,m_iTexID,m_fAlpha);
		return TSun::TTRUE;
	}

}