#include "TLGUIPicture.h"
#include "TLGUITextureMgr.h"

namespace TLunaEngine{
	GUIPicture::GUIPicture(void) : GUICtrl(),
	m_iTexID(-1),
	m_texX(0),
	m_texY(0),
	m_texR(0),
	m_texB(0)
	{
	}

	GUIPicture::~GUIPicture(void)
	{
	}

	bool GUIPicture::InitGUIPicture(int iIndex,GUIContainer* pContainer,TS32 x,TS32 y,TS32 width,TS32 height,
									  int iTexID, float texX, float texY, float texR, float texB)
	{
		if (!GUICtrl::InitCtrlBase(iIndex,pContainer,x,y,width,height))
		{
			return false;
		}
		m_eCtrlType = GUI_CTRL_PICTURE;
		m_iTexID = iTexID;
		m_texX = texX;
		m_texY = texY;
		m_texR = texR;
		m_texB = texB;
		return true;
	}

	void GUIPicture::DestroyCtrl()
	{
		m_eCtrlType = GUI_CTRL_NONE;
		m_iTexID = -1;
		m_texX = 0;
		m_texY = 0;
		m_texR = 0;
		m_texB = 0;
		GUICtrl::DestroyCtrl();
	}

	bool GUIPicture::Update(float fTimeElapsed)
	{
		return GUICtrl::Update(fTimeElapsed);
	}

	bool GUIPicture::Render(float fTimeElapsed)
	{
		if (!GUICtrl::Render(fTimeElapsed))
		{
			return false;
		}
		GUITextureMgr::getSingletonPtr()->DrawGUICtrl(m_posXFinal,m_posYFinal,m_width,m_height,m_texX,m_texY,m_texR,m_texB,m_iTexID,m_fAlpha);
		return true;
	}

}