#ifndef _TGLOBALFUNC_H_
#define _TGLOBALFUNC_H_
#include "TSCommonTypeDef.h"
#include <Windows.h>

namespace TLunaEngine{

	// 定义函数指针，上层游戏循环时调用
	typedef TSun::TVOID(*UserLoop)(TSun::TF32 fTimeElapsed);

	// -------------------------- 以下为导出全局函数，上层调用 ------------------------------

	// 引擎初始化
	// hWnd:窗口句柄
	// hInst:应用程序实例句柄
	// bWnd:是否为窗口
	// lWidth:后缓冲区宽
	// lHeight:后缓冲区高
	// fSecsPerFrame:帧速率控制(每帧时间)
	// szResDir:资源根目录
	// bShowDebugInfo:是否显示Debug信息
	TSun::TBOOL Init(HWND hWnd,HINSTANCE hInst,TSun::TBOOL bWnd,TSun::TS32 lWidth,TSun::TS32 lHeight,
		TSun::TF32 fSecsPerFrame,const TSun::TCHAR* szResDir,const TSun::TCHAR* szSysLangDictFile,
		TSun::TBOOL bShowDebugInfo);

	// 引擎循环
	TSun::TBOOL OnLoopCtrlUpdate(TSun::TF32* pTimeElapsed);
	TSun::TBOOL OnSceneUpdate(TSun::TF32 fTimeElapsed);
	TSun::TBOOL OnSceneRender(TSun::TF32 fTimeElapsed);

	// 游戏循环
	TSun::TBOOL OnGameLoop(UserLoop pLoop);

	// 引擎退出
	TSun::TBOOL Destroy();

	// window size changed
	TSun::TBOOL onWindowSizeChanged(TSun::TU32 width,TSun::TU32 height);

	// ------------- 以下是给编辑器调用的方法，游戏中不能使用 --------------------------------

	// 引擎初始化
	// hWnd:窗口句柄
	// hInst:应用程序句柄
	// lWidth:后缓冲区宽
	// lHeight:后缓冲区高
	// szResDir:资源根目录
	TSun::TBOOL InitForEditor(HWND hWnd,HINSTANCE hInst,TSun::TS32 lWidth,TSun::TS32 lHeight,const TSun::TCHAR* szResDir);

	// 引擎循环
	TSun::TBOOL OnSceneUpdateForEditor(TSun::TF32 fTimeElapsed);
	TSun::TBOOL OnSceneRenderForEditor(TSun::TF32 fTimeElapsed);

	// 引擎退出
	TSun::TBOOL DestroyForEditor();

	// ---------------------------------------------------------------------------------------

}

#endif