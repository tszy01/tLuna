#include "TLGUIText.h"
#include "TLGUIFontManager.h"

namespace TLunaEngine{
	GUIText::GUIText(void) : GUICtrl(),
	m_iFontID(-1),
	m_color(),
	m_strText("")
	{
	}

	GUIText::~GUIText(void)
	{
	}

	bool GUIText::InitGUIText(int iIndex, TLunaEngine::GUIContainer *pContainer, TS32 x, TS32 y, TS32 width, TS32 height, int iFontID, Vector4<float>& color)
	{
		if (!GUICtrl::InitCtrlBase(iIndex,pContainer,x,y,width,height))
		{
			return false;
		}
		m_eCtrlType = GUI_CTRL_TEXT;
		m_iFontID = iFontID;
		m_color = color;
		return true;
	}

	void GUIText::DestroyCtrl()
	{
		m_eCtrlType = GUI_CTRL_NONE;
		m_iFontID = -1;
		m_color = 0;
		GUICtrl::DestroyCtrl();
	}

	bool GUIText::Update(float fTimeElapsed)
	{
		return GUICtrl::Update(fTimeElapsed);
	}

	bool GUIText::Render(float fTimeElapsed)
	{
		if (!GUICtrl::Render(fTimeElapsed))
		{
			return false;
		}
		GUIFontManager* pFontMgr = GUIFontManager::getSingletonPtr();
		pFontMgr->UseFont(m_iFontID);
		pFontMgr->Render(m_strText.GetString(),(size_t)m_strText.GetLength()+1,m_posXFinal,m_posYFinal,m_color);
		return true;
	}

}