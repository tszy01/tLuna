#include "TLEditorFunc.h"
#include "TLEditorApp.h"
#include "TLEditorMgr.h"

namespace TLunaEngine
{
	// init wx
	IMPLEMENT_APP_NO_MAIN(TLEditorApp);

	int LaunchEditor(HINSTANCE hInst,LONG lWidth,LONG lHeight,const char* szResDir)
	{
		if(!TLEditorMgr::getSingletonPtr()->InitEditorMgr(hInst,lWidth,lHeight,szResDir))
			return false;
		int re = wxEntry(hInst);
		TLEditorMgr::getSingletonPtr()->DestroyEditorMgr();
		TLEditorMgr::delSingletonPtr();
		return re;
	}
}