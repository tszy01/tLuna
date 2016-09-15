#include "TLEditorMgr.h"
#include "TLEditorApp.h"
#include "TLGlobalFunc.h"

namespace TLunaEngine{
	TLEditorMgr* Singleton<TLEditorMgr>::m_Ptr = 0;
	
	TLEditorMgr::TLEditorMgr():mInst(NULL),mWidth(1280),mHeight(800),mResDir("")
	{
	}

	TLEditorMgr::~TLEditorMgr()
	{
	}

	bool TLEditorMgr::InitEditorMgr(HINSTANCE hInst,LONG lWidth,LONG lHeight,const char* szResDir)
	{
		if(!szResDir)
			return false;
		mInst = hInst;
		mWidth = lWidth;
		mHeight = lHeight;
		mResDir = szResDir;
		return true;
	}

	void TLEditorMgr::DestroyEditorMgr()
	{
	}

	bool TLEditorMgr::InitEngine(HWND hWnd)
	{
		if(hWnd==NULL)
			return false;
		// init engine
		if(!TLunaEngine::InitForEditor(hWnd,mInst,mWidth,mHeight,mResDir.GetString()))
			return false;
		return true;
	}

	bool TLEditorMgr::LoopEditor()
	{
		float timeElapsed = 0;
		if (TLunaEngine::OnLoopCtrlUpdate(&timeElapsed))
		{
			// 调用上层逻辑
			// 引擎循环
			if(TLunaEngine::OnSceneUpdateForEditor(timeElapsed))
			{
				if(TLunaEngine::OnSceneRenderForEditor(timeElapsed))
					return true;
			}
		}
		return false;
	}

	void TLEditorMgr::DestroyEngine()
	{
		TLunaEngine::DestroyForEditor();
	}
}