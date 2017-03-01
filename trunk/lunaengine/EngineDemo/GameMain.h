#ifndef _GAMEFUNC_H_
#define _GAMEFUNC_H_
#include <Windows.h>
#include "TLMemDef.h"
#include "TLGlobalFunc.h"

// 定义初始化结构体
struct _INITPARAM
{
	HINSTANCE hInst;			// 入口应用程序实例
	int nCmdShow;				// 主窗口的显示
};

// 游戏循环
bool GameLoop(TLunaEngine::UserLoop pLoop);

// 游戏启动入口
int MainExampleGame(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmLine, int nCmdShow);

// 主循环回调函数
void ExampleGameLoop(float fTimeElapsed);
// 监听消息回调函数
void OnCatchInputMsg(BYTE yType,void* param);

// 加载和销毁
bool InitGame(const _INITPARAM* pInitParam);
void DestroyGame();

#endif