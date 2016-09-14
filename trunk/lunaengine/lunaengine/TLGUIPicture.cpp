#include "StdAfx.h"
#include "TLGUIPicture.h"
#include "TLGUITextureMgr.h"

namespace TLunaEngine{
	TLGUIPicture::TLGUIPicture(void) : TLGUICtrl(),
	m_iTexID(-1),
	m_texX(0),
	m_texY(0),
	m_texR(0),
	m_texB(0)
	{
	}

	TLGUIPicture::~TLGUIPicture(void)
	{
	}

	bool TLGUIPicture::InitGUIPicture(int iIndex,TLGUIContainer* pContainer,LONG x,LONG y,LONG width,LONG height,
									  int iTexID, float texX, float texY, float texR, float texB)
	{
		if (!TLGUICtrl::InitCtrlBase(iIndex,pContainer,x,y,width,height))
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

	void TLGUIPicture::DestroyCtrl()
	{
		m_eCtrlType = GUI_CTRL_NONE;
		m_iTexID = -1;
		m_texX = 0;
		m_texY = 0;
		m_texR = 0;
		m_texB = 0;
		TLGUICtrl::DestroyCtrl();
	}

	bool TLGUIPicture::Update(float fTimeElapsed)
	{
		return TLGUICtrl::Update(fTimeElapsed);
	}

	bool TLGUIPicture::Render(float fTimeElapsed)
	{
		if (!TLGUICtrl::Render(fTimeElapsed))
		{
			return false;
		}
		TLGUITextureMgr::getSingletonPtr()->DrawGUICtrl(m_posXFinal,m_posYFinal,m_width,m_height,m_texX,m_texY,m_texR,m_texB,m_iTexID,m_fAlpha);
		return true;
	}

}