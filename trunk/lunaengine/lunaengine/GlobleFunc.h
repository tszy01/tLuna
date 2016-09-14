#pragma once


namespace TLunaEngine{

	// 定义函数指针，上层游戏循环时调用
	typedef void(*UserLoop)(float fTimeElapsed);

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
	bool Init(HWND hWnd,HINSTANCE hInst,BOOL bWnd,LONG lWidth,LONG lHeight,
							 float fSecsPerFrame,const char* szResDir,bool bShowDebugInfo);

	// 引擎循环
	bool OnLoopCtrlUpdate(float* pTimeElapsed);
	bool OnSceneUpdate(float fTimeElapsed);
	bool OnSceneRender(float fTimeElapsed);

	// 游戏循环
	bool OnGameLoop(UserLoop pLoop);

	// 引擎退出
	bool Destroy();

	// 截获IO消息
	// yType:消息类型
	// param:消息结构体
	void CatchInputMsg(BYTE yType,void* param);

	// window size changed
	bool onWindowSizeChanged(UINT width,UINT height);

	// ------------- 以下是给编辑器调用的方法，游戏中不能使用 --------------------------------

	// 引擎初始化
	// hWnd:窗口句柄
	// hInst:应用程序句柄
	// lWidth:后缓冲区宽
	// lHeight:后缓冲区高
	// szResDir:资源根目录
	bool InitForEditor(HWND hWnd,HINSTANCE hInst,LONG lWidth,LONG lHeight,const char* szResDir);

	// 引擎循环
	bool OnSceneUpdateForEditor(float fTimeElapsed);
	bool OnSceneRenderForEditor(float fTimeElapsed);

	// 引擎退出
	bool DestroyForEditor();

	// ---------------------------------------------------------------------------------------

}