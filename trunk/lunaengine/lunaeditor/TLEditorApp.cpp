#include "TLEditorApp.h"
#include "TLEditorMainFrame.h"
#include "TLEditorMgr.h"
//#include "gb_wxcmdwnd.h"
//#include "gb_wxaddobjwnd.h"
//#include "gb_wxpropertywnd.h"
//#include "gb_wxnewterrainwnd.h"
//#include "gb_wxeditterrainwnd.h"
//#include "gb_wxeditfogwnd.h"
//#include "gb_wxeditskywnd.h"
//#include "gb_wxeditambientwnd.h"
//#include "gb_wxeditmeshbrushwnd.h"
//#include "gb_wxsettonemappingwnd.h"
//#include "gb_wxnavgenwnd.h"
//#include "gb_wxpropertyext.h"
//#include "gb_wxcameractrlwnd.h"

namespace TLunaEngine{

	TLEditorApp::TLEditorApp() : wxApp(), mMainFrame(0)/*,mCmdWnd(0),mAddObjWnd(0),mPropWnd(0),
		mNewTerrainWnd(0),mEditTerrainWnd(0),mEditSkyWnd(0),mEditFogWnd(0),mEditAmbientWnd(0), 
		mEditMeshBrushWnd(0), mSetToneMappingWnd(0), mEditNpcTemplateWnd(0), mEditResTemplateWnd(0), mNavGenWnd(0),mCameralCtrlWnd(0)*/
	{
	}

	TLEditorApp::~TLEditorApp()
	{

	}

	// The `main program' equivalent, creating the windows and returning the
	// main frame
	bool TLEditorApp::OnInit()
	{
		if ( !wxApp::OnInit() )
			return false;
		m_bUseGameLoop = true;

		// Create the main frame window
		TLEditorMainFrame* frame = new TLEditorMainFrame;
		frame->SetSize(TLEditorMgr::getSingletonPtr()->GetWidth(),TLEditorMgr::getSingletonPtr()->GetHeight());
		frame->Show(true);
		SetTopWindow(frame);
		mMainFrame = frame;
		// call listener
		if(!TLEditorMgr::getSingletonPtr()->InitEngine((HWND)frame->GetHWND()))
			return false;

		//openCameralCtrlWnd();
		//hideCameralCtrlWnd();

		//wxPropertyGrid::RegisterEditorClass(new GB_WxTerrainTexLayerButton());
		//wxPropertyGrid::RegisterEditorClass(new GB_WxSingleTerrainTexLayerButton());
		//wxPropertyGrid::RegisterEditorClass(new GB_WxPlusSubButton());

		//// cmd wnd
		//mCmdWnd = new GB_WxCmdWnd();
		//mCmdWnd->Show(true);
		//mCmdWnd->addListener(GB_SeAppMgr::getPtr());
		return true;
	}

	void TLEditorApp::openObjectProperty()
	{
		/*closeObjectProperty();
		mPropWnd = new GB_WxPropertyWnd();
		mPropWnd->buildPropertyWnd();
		GB_SeAppMgr::getPtr()->showPropertyWnd(mPropWnd);
		mPropWnd->finalizePanel();
		mPropWnd->Show(true);*/
	}

	void TLEditorApp::closeObjectProperty()
	{
		/*if(mPropWnd)
		{
			GB_SeAppMgr::getPtr()->closePropertyWnd();
			mPropWnd->Show(false);
			mPropWnd->Destroy();
			delete mPropWnd;
			mPropWnd = 0;
		}*/
	}

	void TLEditorApp::openAddObjWnd()
	{
		/*closeAddObjWnd();
		mAddObjWnd = new GB_WxAddObjWnd();
		mAddObjWnd->Show(true);*/
	}

	void TLEditorApp::closeAddObjWnd()
	{
		/*if(mAddObjWnd)
		{
			mAddObjWnd->Destroy();
			delete mAddObjWnd;
			mAddObjWnd = 0;
		}*/
	}

	void TLEditorApp::openNewTerrainWnd()
	{
		/*closeNewTerrainWnd();
		mNewTerrainWnd = new GB_WxNewTerrainWnd();
		mNewTerrainWnd->Show(true);*/
	}

	void TLEditorApp::closeNewTerrainWnd()
	{
		/*if(mNewTerrainWnd)
		{
			mNewTerrainWnd->Destroy();
			delete mNewTerrainWnd;
			mNewTerrainWnd = 0;
		}*/
	}

	void TLEditorApp::openEditTerrainWnd()
	{
		/*closeEditTerrainWnd();
		GB_SeAppMgr::getPtr()->setBrushTerrain(true);
		mEditTerrainWnd = new GB_WxEditTerrainWnd();
		mEditTerrainWnd->Show(true);*/
	}

	void TLEditorApp::closeEditTerrainWnd()
	{
		/*if(mEditTerrainWnd)
		{
			mEditTerrainWnd->Destroy();
			delete mEditTerrainWnd;
			mEditTerrainWnd = 0;
		}
		GB_SeAppMgr::getPtr()->setBrushTerrain(false);*/
	}

	void TLEditorApp::refreshEditTerrainWnd()
	{
		/*if(mEditTerrainWnd)
		{
			mEditTerrainWnd->onTerrainRefresh();
		}*/
	}

	void TLEditorApp::openEditFogWnd()
	{
		/*closeEditFogWnd();
		mEditFogWnd = new GB_WxEditFogWnd();
		mEditFogWnd->Show(true);*/
	}

	void TLEditorApp::closeEditFogWnd()
	{
		/*if(mEditFogWnd)
		{
			mEditFogWnd->Destroy();
			delete mEditFogWnd;
			mEditFogWnd = 0;
		}*/
	}

	void TLEditorApp::openEditSkyWnd()
	{
		/*closeEditSkyWnd();
		mEditSkyWnd = new GB_WxEditSkyWnd();
		mEditSkyWnd->Show(true);*/
	}

	void TLEditorApp::closeEditSkyWnd()
	{
		/*if(mEditSkyWnd)
		{
			mEditSkyWnd->Destroy();
			delete mEditSkyWnd;
			mEditSkyWnd = 0;
		}*/
	}

	void TLEditorApp::openEditAmbientWnd()
	{
		/*closeEditAmbientWnd();
		mEditAmbientWnd = new GB_WxEditAmbientWnd();
		mEditAmbientWnd->Show(true);*/
	}

	void TLEditorApp::closeEditAmbientWnd()
	{
		/*if(mEditAmbientWnd)
		{
			mEditAmbientWnd->Destroy();
			delete mEditAmbientWnd;
			mEditAmbientWnd = 0;
		}*/
	}

	void TLEditorApp::openEditMeshBrushWnd()
	{
		/*closeEditMeshBrushWnd();
		mEditMeshBrushWnd = new GB_WxEditMeshBrushWnd();
		mEditMeshBrushWnd->Show(true);*/
	}

	void TLEditorApp::closeEditMeshBrushWnd()
	{
		/*if(mEditMeshBrushWnd)
		{
			mEditMeshBrushWnd->Destroy();
			delete mEditMeshBrushWnd;
			mEditMeshBrushWnd = 0;
		}*/
	}

	void TLEditorApp::openSetToneMappingWnd()
	{
		/*closeSetToneMappingWnd();
		mSetToneMappingWnd = new GB_WxSetToneMappingWnd();
		mSetToneMappingWnd->Show(true);*/
	}

	void TLEditorApp::closeSetToneMappingWnd()
	{
		/*if(mSetToneMappingWnd)
		{
			mSetToneMappingWnd->Destroy();
			delete mSetToneMappingWnd;
			mSetToneMappingWnd = 0;
		}*/
	}

	void TLEditorApp::openNavGenWnd()
	{
		/*closeNavGenWnd();
		mNavGenWnd = new GB_WxNavGenWnd();
		mNavGenWnd->Show(true);*/
	}

	void TLEditorApp::closeNavGenWnd()
	{
		/*if (mNavGenWnd)
		{
			mNavGenWnd->Destroy();
			delete mNavGenWnd;
			mNavGenWnd = 0;
		}*/
	}

	void TLEditorApp::openCameralCtrlWnd()
	{
		/*closeCameralCtrlWnd();
		mCameralCtrlWnd = new GB_WxCameraCtrlWnd(mMainFrame);
		mCameralCtrlWnd->Show(true);*/
	}

	void TLEditorApp::closeCameralCtrlWnd()
	{
		/*if (mCameralCtrlWnd)
		{
			delete mCameralCtrlWnd;
			mCameralCtrlWnd = 0;
		}*/
	}

	void TLEditorApp::hideCameralCtrlWnd()
	{
		/*if (mCameralCtrlWnd)
		{
			mCameralCtrlWnd->Show(false);
		}*/
	}

	void TLEditorApp::showCameralCtrlWnd()
	{
		/*if (mCameralCtrlWnd)
		{
			mCameralCtrlWnd->Show(true);
		}*/
	}

	bool TLEditorApp::ProcessAppLoop()
	{
		if(!wxApp::ProcessAppLoop())
			return false;
		// Loop GlobleMgr
		if(!TLEditorMgr::getSingletonPtr()->LoopEditor())
			return false;
		return true;
	}

	void TLEditorApp::showMainFrameShortcutMenu()
	{
		if(mMainFrame)
		{
			((TLEditorMainFrame*)mMainFrame)->showShortcutMenu();
		}
	}

	void TLEditorApp::ProcessAppExit()
	{
		wxApp::ProcessAppExit();
	}

	void TLEditorApp::ProcessEditorDestroy()
	{
		//closeNewTerrainWnd();
		//closeEditTerrainWnd();
		//closeEditFogWnd();
		//closeEditSkyWnd();
		//closeEditAmbientWnd();
		//closeObjectProperty();
		//closeNavGenWnd();
		//closeSetToneMappingWnd();
		//closeCameralCtrlWnd();
		//if(mCmdWnd)
		//{
		//	mCmdWnd->clearListener();
		//	mCmdWnd->Destroy();
		//	delete mCmdWnd;
		//	mCmdWnd=0;
		//}
		//if(mAddObjWnd)
		//{
		//	mAddObjWnd->Destroy();
		//	delete mAddObjWnd;
		//	mAddObjWnd =0;
		//}
		// Exit GlobleMgr
		TLEditorMgr::getSingletonPtr()->DestroyEngine();
	}

	bool TLEditorApp::ProcessGameLoop()
	{
		if(!wxApp::ProcessGameLoop())
			return false;
		// Loop GlobleMgr
		if(!TLEditorMgr::getSingletonPtr()->LoopEditor())
			return false;
		return true;
	}
}