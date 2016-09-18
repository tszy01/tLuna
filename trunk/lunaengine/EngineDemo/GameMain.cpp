#include "GameMain.h"
#include "GameInput.h"
#include "InputMsgMgr.h"
#include "GUISceneMgr.h"

#include "MainWindow.h"
#include "LuaInitApp.h"
#include "TLGlobleClass.h"
#include "TLGUIMgr.h"

#include "TLString.h"

//#include "EditorApp.h"
#include "EditorMgr.h"

int MainExampleGame(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmLine, int nCmdShow)
{
	// 构造一个初始化结构体
	_INITPARAM init;
	init.hInst = hInstance;
	init.nCmdShow = nCmdShow;
	// initapp.lua必须有，并且放在exe目录下面
	sprintf_s(init.szScriptFile,sizeof(char)*256,"initapp.lua");
	// 调用初始化
	if(InitGame(&init))
	{
		// 游戏循环
		GameLoop(&ExampleGameLoop);
		// 销毁
		DestroyGame();
	}
	return 1;
}

bool InitGame(const _INITPARAM* pInitParam)
{
	if(!pInitParam)
		return false;
	// 读取初始化配置
	if(!LuaInit::getSingletonPtr()->InitWindowScript(pInitParam->szScriptFile))
		return false;
	LuaInit::getSingletonPtr()->LoadParameters();
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

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmLine, int nCmdShow)
{
	TLunaEngine::String strCmd(lpCmLine);
	int nCmd = 1;
	std::vector<TLunaEngine::String> cmds = strCmd.Split(' ', &nCmd);
	if (cmds.size() >= 1)
	{
		if (cmds[0] == "editor")
		{
			if (!EditorMgr::getSingletonPtr()->InitEditorMgr(hInstance, 1280, 800, "../../../demores/"))
				return 1;
			int re = EditorMgr::getSingletonPtr()->RunEditor();
			EditorMgr::getSingletonPtr()->DestroyEditorMgr();
			EditorMgr::delSingletonPtr();
			return re;
		}
	}
	return MainExampleGame(hInstance, hPrevInstance, lpCmLine, nCmdShow);
}