#ifndef _TLEDITORMGR_H_
#define _TLEDITORMGR_H_

#include "TSingleton.h"
#include "TString.h"

namespace TLunaEngine
{
	class TLEditorMgr
	{
	T_SINGLETON_DEF(TLEditorMgr);
	public:
		TLEditorMgr();
		~TLEditorMgr();
	public:
		bool InitEditorMgr(HINSTANCE hInst,LONG lWidth,LONG lHeight,const char* szResDir);
		void DestroyEditorMgr();
		bool InitEngine(HWND hWnd);
		bool LoopEditor();
		void DestroyEngine();
	private:
		HINSTANCE mInst;
		LONG mWidth;
		LONG mHeight;
		TString mResDir;
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

		const TString& GetResDir()
		{
			return mResDir;
		}
	};
}
#endif