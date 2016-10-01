#include "TLGlobleClass.h"
#include "TLTestTriangle.h"
#include "TLString.h"
#include "TLLoopCtrl.h"
#include "TLGUIFontManager.h"

namespace TLunaEngine{
	GlobleClass* Singleton<GlobleClass>::m_Ptr = 0;

	GlobleClass::GlobleClass(TVOID) : m_pTri(0), m_strResDir(""), /*m_pDebugFont(0), */m_bShowDebugInfo(TFALSE), m_bEditor(TFALSE), mLoopCtrl(0)
	{
	}

	GlobleClass::~GlobleClass(TVOID)
	{
	}

	TBOOL GlobleClass::InitGlobleClass()
	{
		// Triangle
		m_pTri = new TestTriangle();
		if(!m_pTri->InitTriangle())
			return TFALSE;
		// Debug信息
		String strAll = m_strResDir + "gui\\font\\simkai.ttf";
		if(!GUIFontManager::getSingletonPtr()->initDebugFont(strAll.GetString(),24,256))
		{
			return TFALSE;
		}
		// init loop control
		mLoopCtrl = new LoopCtrl();
		mLoopCtrl->Init();
		mLoopCtrl->StartTime();

		return TTRUE;
	}

	TVOID GlobleClass::DestroyGlobleClass()
	{
		// destroy loop control
		if (mLoopCtrl)
		{
			mLoopCtrl->StopTime();
			delete mLoopCtrl;
			mLoopCtrl = 0;
		}
		// Debug信息
		GUIFontManager::getSingletonPtr()->deleteDebugFont();
		// Triangle
		if (m_pTri)
		{
			delete m_pTri;
			m_pTri = 0;
		}
	}

	TVOID GlobleClass::updateLoopCtrl()
	{
		if (mLoopCtrl)
		{
			mLoopCtrl->UpdateTime();
		}
	}

	TBOOL GlobleClass::getLoopCtrlCanRender()
	{
		if (mLoopCtrl)
		{
			return mLoopCtrl->CanRender();
		}
		return TFALSE;
	}

	TVOID GlobleClass::calcFPS()
	{
		if (mLoopCtrl)
		{
			mLoopCtrl->CalFPS();
		}
	}

	TF32 GlobleClass::getElapsedTime()
	{
		if (mLoopCtrl)
		{
			return (TF32)mLoopCtrl->GetElapsedTime();
		}
		return 0.0f;
	}

	TBOOL GlobleClass::OnUpdate(TF32 fTimeElapsed)
	{
		return TTRUE;
	}

	TBOOL GlobleClass::OnRender(TF32 fTimeElapsed)
	{
		// Debug信息
		if (m_bShowDebugInfo && mLoopCtrl)
		{
			//m_pDebugFont->FontPrint(LoopCtrl::getSingletonPtr()->GetFPSString(),0,0,D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),100,32,TLunaEngine::FA_LEFT);
			GUIFontManager::getSingletonPtr()->RenderDebugFont(mLoopCtrl->GetFPSString(),
				mLoopCtrl->GetFPSStringLen(),0,0,Vector4<TF32>(0.0f,1.0f,0.0f,1.0f));
		}
		// Triangle
		m_pTri->OnRender();
		return TTRUE;
	}

}