#ifndef _EDITORMGR_H_
#define _EDITORMGR_H_

#include "ConfigDef.h"

#ifdef BUILD_EDITOR
#include "TLSingleton.h"
#include "TLString.h"
#include <Windows.h>

class EditorMgr : public TLunaEngine::Singleton<EditorMgr>
{
	friend class TLunaEngine::Singleton<EditorMgr>;
protected:
	EditorMgr();
	~EditorMgr();
public:
	bool InitEditorMgr(HINSTANCE hInst,LONG lWidth,LONG lHeight,const char* szResDir);
	int RunEditor();
	void DestroyEditorMgr();
	bool InitEngine(HWND hWnd);
	bool LoopEditor();
	void DestroyEngine();
private:
	HINSTANCE mInst;
	LONG mWidth;
	LONG mHeight;
	TLunaEngine::String mResDir;
public:
	HINSTANCE GetInstance()
	{
		return mInst;
	}

	LONG GetWidth()
	{
		return mWidth;
	}

	LONG GetHeight()
	{
		return mHeight;
	}

	const TLunaEngine::String& GetResDir()
	{
		return mResDir;
	}
};

#endif // BUILD_EDITOR
#endif