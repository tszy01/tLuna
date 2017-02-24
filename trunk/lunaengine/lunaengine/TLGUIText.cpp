#include "TLGUIText.h"
#include "TLGUIFontManager.h"
#include "TLGlobleClass.h"
#include "TSLangDict.h"

namespace TLunaEngine{
	GUIText::GUIText(TSun::TVOID) : GUICtrl(),m_iFontID(-1),m_color(),m_strText(L""),m_textID(0)
	{
	}

	GUIText::~GUIText(TSun::TVOID)
	{
	}

	TSun::TBOOL GUIText::InitGUIText(TSun::TS32 iIndex, TLunaEngine::GUIContainer *pContainer, TSun::TS32 x, 
		TSun::TS32 y, TSun::TS32 width, TSun::TS32 height, TSun::TS32 iFontID, TSun::Vector4<TSun::TF32>& color, TSun::TU64 textID)
	{
		if (!GUICtrl::InitCtrlBase(iIndex,pContainer,x,y,width,height))
		{
			return TSun::TFALSE;
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
		return TSun::TTRUE;
	}

	TSun::TVOID GUIText::DestroyCtrl()
	{
		m_eCtrlType = GUI_CTRL_NONE;
		m_iFontID = -1;
		m_color = 0;
		m_textID = 0;
		GUICtrl::DestroyCtrl();
	}

	TSun::TBOOL GUIText::Update(TSun::TF32 fTimeElapsed)
	{
		return GUICtrl::Update(fTimeElapsed);
	}

	TSun::TBOOL GUIText::Render(TSun::TF32 fTimeElapsed)
	{
		if (!GUICtrl::Render(fTimeElapsed))
		{
			return TSun::TFALSE;
		}
		GUIFontManager* pFontMgr = GUIFontManager::getSingletonPtr();
		pFontMgr->UseFont(m_iFontID);
		pFontMgr->Render(m_strText.GetWString(),m_strText.GetLength()+1,m_posXFinal,m_posYFinal,m_color);
		return TSun::TTRUE;
	}

}