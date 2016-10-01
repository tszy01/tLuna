#ifdef DEMO_CHECK_MEM_LEAK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // DEMO_CHECK_MEM_LEAK

#include "GameMain.h"
#include "GameInput.h"
#include "InputMsgMgr.h"
#include "GUISceneMgr.h"

#include "MainWindow.h"
#include "LuaInitApp.h"
#include "TLGlobleClass.h"
#include "TLGUIMgr.h"

#include "TLString.h"

#include "ConfigDef.h"

#ifdef BUILD_EDITOR
#include "EditorMgr.h"
#endif // BUILD_EDITOR

int MainExampleGame(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmLine, int nCmdShow)
{
	// 构造一个初始化结构体
	_INITPARAM init;
	init.hInst = hInstance;
	init.nCmdShow = nCmdShow;
	// 调用初始化
	if(InitGame(&init))
	{
		// 游戏循环
		GameLoop(&ExampleGameLoop);
		// 销毁
		DestroyGame();
	}
	else
	{
		return 1;
	}
	return 0;
}

bool InitGame(const _INITPARAM* pInitParam)
{
	if(!pInitParam)
		return false;
	// 建立主窗口
	// 配置结构体
	_MAINWNDCONFIG wndConfig;
	wndConfig.bufferHeight = LuaInit::getSingletonPtr()->m_bufferHeight;
	wndConfig.bufferWidth = LuaInit::getSingletonPtr()->m_bufferWidth;
	wndConfig.bWnd = LuaInit::getSingletonPtr()->m_bWnd;
	memcpy_s(wndConfig.szWindowText,sizeof(char)*256,LuaInit::getSingletonPtr()->m_szWindowText,sizeof(char)*256);
	// 单例初始化
	MainWindow* mainWnd = MainWindow::getSingletonPtr();
	if(mainWnd->InitWindow(pInitParam->hInst,pInitParam->nCmdShow,&wndConfig)!=S_OK)
		return false;
	mainWnd->CalcRectPro();
	// 初始化Engine
	if(!TLunaEngine::Init(mainWnd->GetHwnd(),pInitParam->hInst,LuaInit::getSingletonPtr()->m_bWnd,
		LuaInit::getSingletonPtr()->m_bufferWidth,LuaInit::getSingletonPtr()->m_bufferHeight,1.0f/LuaInit::getSingletonPtr()->m_controlFps,
		LuaInit::getSingletonPtr()->m_szResDir,LuaInit::getSingletonPtr()->m_bShowDebugInfo))
		return false;
	// GUI场景管理
	GUISceneMgr* pGUISceneMgr = GUISceneMgr::getSingletonPtr();
	pGUISceneMgr->InitGUISceneMgr();
	TLunaEngine::GUIMgr::getSingletonPtr()->SetSceneListener(pGUISceneMgr);
	// 建立Input设备
	GameInput* pInput = GameInput::getSingletonPtr();
	if(!pInput->InitInput(mainWnd->GetHwnd(),pInitParam->hInst,LuaInit::getSingletonPtr()->m_bUseJoystick))
		return false;
	InputMsgMgr::m_CalledFuncList.push_back(&OnCatchInputMsg);
	// 建立入口GUI
	TLunaEngine::String strFile = TLunaEngine::String(LuaInit::getSingletonPtr()->m_szResDir) + "gui\\startscene\\guifont.txt";
	if (!pGUISceneMgr->AddFont(strFile.GetString()))
	{
		return false;
	}
	strFile = TLunaEngine::String(LuaInit::getSingletonPtr()->m_szResDir) + "gui\\startscene\\guitex.txt";
	if (!pGUISceneMgr->InitGUIRender(strFile.GetString()))
	{
		return false;
	}
	strFile = TLunaEngine::String(LuaInit::getSingletonPtr()->m_szResDir) + "gui\\startscene\\guictrl.txt";
	if (!pGUISceneMgr->InitGUI(strFile.GetString()))
	{
		return false;
	}
	return true;
}

void DestroyGame()
{
	// 撤销Input接收函数
	std::list<ProcessInput>::iterator itr = InputMsgMgr::m_CalledFuncList.begin();
	for(;itr!=InputMsgMgr::m_CalledFuncList.end();++itr)
	{
		ProcessInput p = (*itr);
		if(p==&OnCatchInputMsg)
		{
			p = 0;
			InputMsgMgr::m_CalledFuncList.erase(itr);
			break;
		}
	}
	// 销毁Input
	GameInput::getSingletonPtr()->DestroyInput();
	GameInput::delSingletonPtr();
	// 场景管理
	GUISceneMgr::getSingletonPtr()->DestroyGUISceneMgr();
	GUISceneMgr::delSingletonPtr();
	// Engine销毁
	TLunaEngine::Destroy();
	// 销毁主窗口
	MainWindow::getSingletonPtr()->DestroyWindow();
	MainWindow::delSingletonPtr();
}

bool GameLoop(TLunaEngine::UserLoop pLoop)
{
	MSG msg;
	BOOL bDrawCursor = FALSE;
	// 循环主窗口消息
	while(true)
	{
		//一般来说，GetMessage被设计用于 高效的从消息
		//队列中获取消息,如果队列中没有消息，那么导致
		//线程睡眠状态,而PeekMessage不会导致，它马上
		//返回0；
		if(PeekMessage(&msg,NULL,0U,0U,PM_REMOVE))
			// 接收系统信息(&msg为MSG类型的信息结构体，NULL
			//是窗口句柄，0，0是表示接受所有窗口的消息)
		{
			// 捕获一些消息
			if(msg.message==WM_QUIT)
				break;
			switch(msg.message)
			{
			case WM_SETCURSOR:
				bDrawCursor = TRUE;
				break;
			case WM_NCMOUSEMOVE:
				bDrawCursor = FALSE;
				break;
			}
			TranslateMessage(&msg); //转换信息
			DispatchMessage(&msg);  // 
		}
		else
		{
			// 游戏循环
			if(!TLunaEngine::OnGameLoop(pLoop))
				return false;
		}
	}
	return true;
}

void ExampleGameLoop(float fTimeElapsed)
{
	// Update Input
	GameInput::getSingletonPtr()->UpdateInput(&InputMsgMgr::CatchInputMsg);
}

void OnCatchInputMsg(BYTE yType,void* param)
{
	// GUI捕获
	GUISceneMgr::getSingletonPtr()->CatchInputMsg(yType,param);
	// 全局捕获
	if(yType == (BYTE)MSG_TYPE_KEY)
	{
		_MSG_KEY_INFO* keyInfo = (_MSG_KEY_INFO*)param;
	}
	else if(yType == (BYTE)MSG_TYPE_MOUSE)
	{
		_MSG_MOUSE_INFO* mouseInfo = (_MSG_MOUSE_INFO*)param;
	}
	else if(yType == (BYTE)MSG_TYPE_JOYS)
	{
		_MSG_JOYS_INFO* joysInfo = (_MSG_JOYS_INFO*)param;
	}
}

#ifdef BUILD_TEST
int runTest()
{
	TLunaEngine::SharedPtr<int> a = TLunaEngine::SharedPtr<int>(new int);
	TLunaEngine::SharedPtr<int> b = TLunaEngine::SharedPtr<int>(new int);
	if (a < b)
	{
		printf("a < b\n");
	}
	return 0;
}
#endif // BUILD_TEST

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmLine, int nCmdShow)
{
#ifdef DEMO_CHECK_MEM_LEAK
	//_CrtSetBreakAlloc(538);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // DEMO_CHECK_MEM_LEAK
	// init variables
	TLunaEngine::String initScriptFile("initapp.lua");
	bool bEditor = false;
	bool bTest = false;

	// read command
	TLunaEngine::String strCmd(lpCmLine);
	TLunaEngine::TU32 nCmd = 1;
	TLunaEngine::List<TLunaEngine::String> cmds = strCmd.Split(' ', &nCmd);
	strCmd.~String();
	for (int i = 0;i < (int)cmds.size();++i)
	{
		if (cmds[i] == "-editor")
		{
			bEditor = true;
		}
		if (cmds[i] == "-initfile" && i + 1 < (int)cmds.size())
		{
			initScriptFile = cmds[i + 1];
		}
		if (cmds[i] == "-test")
		{
			bTest = true;
		}
	}

#ifdef BUILD_TEST
	if (bTest)
	{
		return runTest();
	}
#endif // BUILD_TEST

	// read init script
	if (!LuaInit::getSingletonPtr()->InitWindowScript(initScriptFile.GetString()))
		return 1;
	LuaInit::getSingletonPtr()->LoadParameters();

	// run
	int re = 0;
#ifdef BUILD_EDITOR
	if (bEditor)
	{
		if (!EditorMgr::getSingletonPtr()->InitEditorMgr(hInstance, LuaInit::getSingletonPtr()->m_bufferWidth, 
			LuaInit::getSingletonPtr()->m_bufferHeight, LuaInit::getSingletonPtr()->m_szResDir))
			return 1;
		re = EditorMgr::getSingletonPtr()->RunEditor();
		EditorMgr::getSingletonPtr()->DestroyEditorMgr();
		EditorMgr::delSingletonPtr();
	}
	else
#endif // BUILD_EDITOR
	{
		re = MainExampleGame(hInstance, hPrevInstance, lpCmLine, nCmdShow);
	}
	
	LuaInit::delSingletonPtr();
	return re;
}