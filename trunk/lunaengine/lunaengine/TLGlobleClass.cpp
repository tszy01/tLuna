#include "TLGlobleClass.h"
#include "TLTestTriangle.h"
#include "TSString.h"
#include "TLLoopCtrl.h"
#include "TLGUIFontManager.h"
#include "TSLangDict.h"

TLunaEngine::GlobleClass* TSun::Singleton<TLunaEngine::GlobleClass>::m_Ptr = 0;

namespace TLunaEngine{
	GlobleClass::GlobleClass(TSun::TVOID) : m_pTri(0), m_strResDir(""), /*m_pDebugFont(0), */m_bShowDebugInfo(TSun::TFALSE), 
		m_bEditor(TSun::TFALSE), mLoopCtrl(0), mLangDict(0), mLangDictFile("")
	{
	}

	GlobleClass::~GlobleClass(TSun::TVOID)
	{
	}

	TSun::TBOOL GlobleClass::InitGlobleClass()
	{
		// init language dictionary
		if (mLangDictFile == "")
		{
			mLangDictFile = "sys_en.txt";
		}
		TSun::String strLangDictFullFile = m_strResDir + TSun::String("lang_dict\\") + mLangDictFile;
		mLangDict = new TSun::LangDict();
		if (!mLangDict->loadFromFile(strLangDictFullFile.GetString()))
		{
			delete mLangDict;
			mLangDict = 0;
			return TSun::TFALSE;
		}
		// Triangle
		m_pTri = new TestTriangle();
		if(!m_pTri->InitTriangle())
			return TSun::TFALSE;
		// Debug信息
		TSun::String strAll = m_strResDir + "gui\\font\\simkai.ttf";
		if(!GUIFontManager::getSingletonPtr()->initDebugFont(strAll.GetString(),24,256))
		{
			return TSun::TFALSE;
		}
		// init loop control
		mLoopCtrl = new LoopCtrl();
		mLoopCtrl->Init();
		mLoopCtrl->StartTime();

		return TSun::TTRUE;
	}

	TSun::TVOID GlobleClass::DestroyGlobleClass()
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
		// destroy language dictionary
		if (mLangDict)
		{
			delete mLangDict;
			mLangDict = 0;
		}
	}

	TSun::TVOID GlobleClass::updateLoopCtrl()
	{
		if (mLoopCtrl)
		{
			mLoopCtrl->UpdateTime();
		}
	}

	TSun::TBOOL GlobleClass::getLoopCtrlCanRender()
	{
		if (mLoopCtrl)
		{
			return mLoopCtrl->CanRender();
		}
		return TSun::TFALSE;
	}

	TSun::TVOID GlobleClass::calcFPS()
	{
		if (mLoopCtrl)
		{
			mLoopCtrl->CalFPS();
		}
	}

	TSun::TF32 GlobleClass::getElapsedTime()
	{
		if (mLoopCtrl)
		{
			return (TSun::TF32)mLoopCtrl->GetElapsedTime();
		}
		return 0.0f;
	}

	TSun::TBOOL GlobleClass::OnUpdate(TSun::TF32 fTimeElapsed)
	{
		return TSun::TTRUE;
	}

	TSun::TBOOL GlobleClass::OnRender(TSun::TF32 fTimeElapsed)
	{
		// Debug信息
		if (m_bShowDebugInfo && mLoopCtrl)
		{
			//m_pDebugFont->FontPrint(LoopCtrl::getSingletonPtr()->GetFPSString(),0,0,D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),100,32,TLunaEngine::FA_LEFT);
			GUIFontManager::getSingletonPtr()->RenderDebugFont(mLoopCtrl->GetFPSString(),
				mLoopCtrl->GetFPSStringLen(),0,0,TSun::Vector4<TSun::TF32>(0.0f,1.0f,0.0f,1.0f));
		}
		// Triangle
		m_pTri->OnRender();
		return TSun::TTRUE;
	}

}