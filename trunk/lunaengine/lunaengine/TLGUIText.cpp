#include "TLGUIText.h"
#include "TLGUIFontManager.h"

namespace TLunaEngine{
	GUIText::GUIText(TVOID) : GUICtrl(),
	m_iFontID(-1),
	m_color(),
	m_strText("")
	{
	}

	GUIText::~GUIText(TVOID)
	{
	}

	TBOOL GUIText::InitGUIText(int iIndex, TLunaEngine::GUIContainer *pContainer, TS32 x, TS32 y, TS32 width, TS32 height, int iFontID, Vector4<float>& color)
	{
		if (!GUICtrl::InitCtrlBase(iIndex,pContainer,x,y,width,height))
		{
			return TFALSE;
		}
		m_eCtrlType = GUI_CTRL_TEXT;
		m_iFontID = iFontID;
		m_color = color;
		return TTRUE;
	}

	TVOID GUIText::DestroyCtrl()
	{
		m_eCtrlType = GUI_CTRL_NONE;
		m_iFontID = -1;
		m_color = 0;
		GUICtrl::DestroyCtrl();
	}

	TBOOL GUIText::Update(float fTimeElapsed)
	{
		return GUICtrl::Update(fTimeElapsed);
	}

	TBOOL GUIText::Render(float fTimeElapsed)
	{
		if (!GUICtrl::Render(fTimeElapsed))
		{
			return TFALSE;
		}
		GUIFontManager* pFontMgr = GUIFontManager::getSingletonPtr();
		pFontMgr->UseFont(m_iFontID);
		pFontMgr->Render(m_strText.GetString(),(size_t)m_strText.GetLength()+1,m_posXFinal,m_posYFinal,m_color);
		return TTRUE;
	}

}