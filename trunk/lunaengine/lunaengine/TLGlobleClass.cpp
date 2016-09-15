#include "TLGlobleClass.h"
#include "TLTestTriangle.h"
#include "TLString.h"
#include "TLLoopCtrl.h"
#include "TLGUIFontManager.h"

namespace TLunaEngine{
	GlobleClass* Singleton<GlobleClass>::m_Ptr = 0;

	GlobleClass::GlobleClass(void) : m_pTri(0), m_strResDir(""), /*m_pDebugFont(0), */m_bShowDebugInfo(false), m_bEditor(false), mLoopCtrl(0)
	{
	}

	GlobleClass::~GlobleClass(void)
	{
	}

	bool GlobleClass::InitGlobleClass()
	{
		// Triangle
		m_pTri = new TestTriangle();
		if(!m_pTri->InitTriangle())
			return false;
		// Debug信息
		String strAll = m_strResDir + "gui\\font\\simkai.ttf";
		if(!GUIFontManager::getSingletonPtr()->initDebugFont(strAll.GetString(),24,256))
		{
			return false;
		}
		// init loop control
		mLoopCtrl = new LoopCtrl();
		mLoopCtrl->Init();
		mLoopCtrl->StartTime();

		return true;
	}

	void GlobleClass::DestroyGlobleClass()
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

	void GlobleClass::updateLoopCtrl()
	{
		if (mLoopCtrl)
		{
			mLoopCtrl->UpdateTime();
		}
	}

	bool GlobleClass::getLoopCtrlCanRender()
	{
		if (mLoopCtrl)
		{
			return mLoopCtrl->CanRender();
		}
		return false;
	}

	void GlobleClass::calcFPS()
	{
		if (mLoopCtrl)
		{
			mLoopCtrl->CalFPS();
		}
	}

	float GlobleClass::getElapsedTime()
	{
		if (mLoopCtrl)
		{
			return (float)mLoopCtrl->GetElapsedTime();
		}
		return 0.0f;
	}

	bool GlobleClass::OnUpdate(float fTimeElapsed)
	{
		return true;
	}

	bool GlobleClass::OnRender(float fTimeElapsed)
	{
		// Debug信息
		if (m_bShowDebugInfo && mLoopCtrl)
		{
			//m_pDebugFont->FontPrint(LoopCtrl::getSingletonPtr()->GetFPSString(),0,0,D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),100,32,TLunaEngine::FA_LEFT);
			GUIFontManager::getSingletonPtr()->RenderDebugFont(mLoopCtrl->GetFPSString(),
				mLoopCtrl->GetFPSStringLen(),0,0,Vector4<float>(0.0f,1.0f,0.0f,1.0f));
		}
		// Triangle
		m_pTri->OnRender();
		return true;
	}

}