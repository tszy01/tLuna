#include "StdAfx.h"
#include "TLGlobleClass.h"
#include "TLTriangle.h"
#include "TString.h"
#include "TLLoopCtrl.h"
#include "TLGUIFontManager.h"
using namespace TLunaEngine;

namespace TLunaEngine{
	T_SINGLETON_IMP(TLGlobleClass);

	TLGlobleClass::TLGlobleClass(void) : m_pTri(0), m_strResDir(""), /*m_pDebugFont(0), */m_bShowDebugInfo(false), m_bEditor(false)
	{
	}

	TLGlobleClass::~TLGlobleClass(void)
	{
	}

	bool TLGlobleClass::InitGlobleClass()
	{
		// Triangle
		m_pTri = new TLTriangle();
		if(!m_pTri->InitTriangle())
			return false;
		// Debug信息
		TString strAll = m_strResDir + "gui\\font\\simkai.ttf";
		if(!TLGUIFontManager::getSingletonPtr()->initDebugFont(strAll.GetString(),24,256))
		{
			return false;
		}

		return true;
	}

	void TLGlobleClass::DestroyGlobleClass()
	{
		// Debug信息
		TLGUIFontManager::getSingletonPtr()->deleteDebugFont();
		// Triangle
		SAFE_DELETE(m_pTri);
	}

	bool TLGlobleClass::OnUpdate(float fTimeElapsed)
	{
		return true;
	}

	bool TLGlobleClass::OnRender(float fTimeElapsed)
	{
		// Debug信息
		if (m_bShowDebugInfo)
		{
			//m_pDebugFont->FontPrint(TLLoopCtrl::getSingletonPtr()->GetFPSString(),0,0,D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),100,32,TLunaEngine::FA_LEFT);
			TLGUIFontManager::getSingletonPtr()->RenderDebugFont(TLLoopCtrl::getSingletonPtr()->GetFPSString(),
				TLLoopCtrl::getSingletonPtr()->GetFPSStringLen(),0,0,TVector4<float>(0.0f,1.0f,0.0f,1.0f));
		}
		// Triangle
		m_pTri->OnRender();
		return true;
	}

}