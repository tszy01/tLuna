#include "EditorMgr.h"
#include "TLGlobalFunc.h"

#include "EditorApp.h"

EditorMgr* TLunaEngine::Singleton<EditorMgr>::m_Ptr = 0;
	
EditorMgr::EditorMgr():mInst(NULL),mWidth(1280),mHeight(800),mResDir("")
{
}

EditorMgr::~EditorMgr()
{
}

bool EditorMgr::InitEditorMgr(HINSTANCE hInst,LONG lWidth,LONG lHeight,const char* szResDir)
{
	if(!szResDir)
		return false;
	mInst = hInst;
	mWidth = lWidth;
	mHeight = lHeight;
	mResDir = szResDir;
	return true;
}

int EditorMgr::RunEditor()
{
	if (mInst == NULL)
		return 1;
	int re = wxEntry(mInst);
	return re;
}

void EditorMgr::DestroyEditorMgr()
{
}

bool EditorMgr::InitEngine(HWND hWnd)
{
	if(hWnd==NULL)
		return false;
	// init engine
	if(!TLunaEngine::InitForEditor(hWnd,mInst,mWidth,mHeight,mResDir.GetString()))
		return false;
	return true;
}

bool EditorMgr::LoopEditor()
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

void EditorMgr::DestroyEngine()
{
	TLunaEngine::DestroyForEditor();
}