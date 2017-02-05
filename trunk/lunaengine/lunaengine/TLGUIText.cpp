#include "TLGUIText.h"
#include "TLGUIFontManager.h"
#include "TLGlobleClass.h"
#include "TLLangDict.h"

namespace TLunaEngine{
	GUIText::GUIText(TVOID) : GUICtrl(),m_iFontID(-1),m_color(),m_strText(L""),m_textID(0)
	{
	}

	GUIText::~GUIText(TVOID)
	{
	}

	TBOOL GUIText::InitGUIText(TS32 iIndex, TLunaEngine::GUIContainer *pContainer, TS32 x, 
		TS32 y, TS32 width, TS32 height, TS32 iFontID, Vector4<TF32>& color, TU64 textID)
	{
		if (!GUICtrl::InitCtrlBase(iIndex,pContainer,x,y,width,height))
		{
			return TFALSE;
		}
		m_eCtrlType = GUI_CTRL_TEXT;
		m_iFontID = iFontID;
		m_color = color;
		m_textID = textID;
		// get text from system dictionary
		if (GlobleClass::getSingletonPtr()->mLangDict)
		{
			GlobleClass::getSingletonPtr()->mLangDict->findByKey(m_textID, m_strText);
		}
		return TTRUE;
	}

	TVOID GUIText::DestroyCtrl()
	{
		m_eCtrlType = GUI_CTRL_NONE;
		m_iFontID = -1;
		m_color = 0;
		m_textID = 0;
		GUICtrl::DestroyCtrl();
	}

	TBOOL GUIText::Update(TF32 fTimeElapsed)
	{
		return GUICtrl::Update(fTimeElapsed);
	}

	TBOOL GUIText::Render(TF32 fTimeElapsed)
	{
		if (!GUICtrl::Render(fTimeElapsed))
		{
			return TFALSE;
		}
		GUIFontManager* pFontMgr = GUIFontManager::getSingletonPtr();
		pFontMgr->UseFont(m_iFontID);
		pFontMgr->Render(m_strText.GetWString(),m_strText.GetLength()+1,m_posXFinal,m_posYFinal,m_color);
		return TTRUE;
	}

}