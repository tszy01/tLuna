#ifndef _TLEDITORMGR_H_
#define _TLEDITORMGR_H_

#include "TLSingleton.h"
#include "TLString.h"
#include <Windows.h>

namespace TLunaEngine
{
	class TLEditorMgr : public Singleton<TLEditorMgr>
	{
		friend class Singleton<TLEditorMgr>;
	protected:
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
		String mResDir;
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

		const String& GetResDir()
		{
			return mResDir;
		}
	};
}
#endif