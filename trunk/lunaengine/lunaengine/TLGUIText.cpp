#include "StdAfx.h"
#include "TLGUIText.h"
#include "TLGUIFontManager.h"

namespace TLunaEngine{
	TLGUIText::TLGUIText(void) : TLGUICtrl(),
	m_iFontID(-1),
	m_color(),
	m_strText("")
	{
	}

	TLGUIText::~TLGUIText(void)
	{
	}

	bool TLGUIText::InitGUIText(int iIndex, TLunaEngine::TLGUIContainer *pContainer, LONG x, LONG y, LONG width, LONG height, int iFontID, TVector4<float>& color)
	{
		if (!TLGUICtrl::InitCtrlBase(iIndex,pContainer,x,y,width,height))
		{
			return false;
		}
		m_eCtrlType = GUI_CTRL_TEXT;
		m_iFontID = iFontID;
		m_color = color;
		return true;
	}

	void TLGUIText::DestroyCtrl()
	{
		m_eCtrlType = GUI_CTRL_NONE;
		m_iFontID = -1;
		m_color = 0;
		TLGUICtrl::DestroyCtrl();
	}

	bool TLGUIText::Update(float fTimeElapsed)
	{
		return TLGUICtrl::Update(fTimeElapsed);
	}

	bool TLGUIText::Render(float fTimeElapsed)
	{
		if (!TLGUICtrl::Render(fTimeElapsed))
		{
			return false;
		}
		TLGUIFontManager* pFontMgr = TLGUIFontManager::getSingletonPtr();
		pFontMgr->UseFont(m_iFontID);
		pFontMgr->Render(m_strText.GetString(),(size_t)m_strText.GetLength()+1,m_posXFinal,m_posYFinal,m_color);
		return true;
	}

}