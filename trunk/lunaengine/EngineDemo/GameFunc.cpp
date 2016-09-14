#include "stdafx.h"
#include "GameFunc.h"
#include "GameInput.h"
#include "TLInputMsgMgr.h"
#include "TLGUISceneMgr.h"

#include "TLMainWindow.h"
#include "LuaInitApp.h"
#include "GlobleFunc.h"
#include "TLGlobleClass.h"
#include "TLGUIMgr.h"

using namespace TLunaEngine;

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
	if(!CLuaInit::getSingletonPtr()->InitWindowScript(pInitParam->szScriptFile))
		return false;
	CLuaInit::getSingletonPtr()->LoadParameters();
	// 建立主窗口
	// 配置结构体
	_MAINWNDCONFIG wndConfig;
	wndConfig.bufferHeight = CLuaInit::getSingletonPtr()->m_bufferHeight;
	wndConfig.bufferWidth = CLuaInit::getSingletonPtr()->m_bufferWidth;
	wndConfig.bWnd = CLuaInit::getSingletonPtr()->m_bWnd;
	memcpy_s(wndConfig.szWindowText,sizeof(char)*256,CLuaInit::getSingletonPtr()->m_szWindowText,sizeof(char)*256);
	// 单例初始化
	TLMainWindow* mainWnd = TLMainWindow::getSingletonPtr();
	if(mainWnd->InitWindow(pInitParam->hInst,pInitParam->nCmdShow,&wndConfig)!=S_OK)
		return false;
	mainWnd->CalcRectPro();
	// 初始化Engine
	if(!TLunaEngine::Init(mainWnd->GetHwnd(),pInitParam->hInst,CLuaInit::getSingletonPtr()->m_bWnd,
		CLuaInit::getSingletonPtr()->m_bufferWidth,CLuaInit::getSingletonPtr()->m_bufferHeight,1.0f/CLuaInit::getSingletonPtr()->m_controlFps,
		CLuaInit::getSingletonPtr()->m_szResDir,CLuaInit::getSingletonPtr()->m_bShowDebugInfo))
		return false;
	// GUI场景管理
	TLGUISceneMgr* pGUISceneMgr = TLGUISceneMgr::getSingletonPtr();
	pGUISceneMgr->InitGUISceneMgr();
	TLunaEngine::TLGUIMgr::getSingletonPtr()->SetSceneListener(pGUISceneMgr);
	// 建立Input设备
	TGameInput* pInput = TGameInput::getSingletonPtr();
	if(!pInput->InitInput(mainWnd->GetHwnd(),pInitParam->hInst,CLuaInit::getSingletonPtr()->m_bUseJoystick))
		return false;
	TLunaEngine::TLInputMsgMgr::m_CalledFuncList.push_back(&OnCatchInputMsg);
	// 建立入口GUI
	TString strFile = TString(CLuaInit::getSingletonPtr()->m_szResDir) + "gui\\startscene\\guifont.txt";
	if (!pGUISceneMgr->AddFont(strFile.GetString()))
	{
		return false;
	}
	strFile = TString(CLuaInit::getSingletonPtr()->m_szResDir) + "gui\\startscene\\guitex.txt";
	if (!pGUISceneMgr->InitGUIRender(strFile.GetString()))
	{
		return false;
	}
	strFile = TString(CLuaInit::getSingletonPtr()->m_szResDir) + "gui\\startscene\\guictrl.txt";
	if (!pGUISceneMgr->InitGUI(strFile.GetString()))
	{
		return false;
	}
	return true;
}

void DestroyGame()
{
	// 撤销Input接收函数
	std::list<TLunaEngine::ProcessInput>::iterator itr = TLunaEngine::TLInputMsgMgr::m_CalledFuncList.begin();
	for(;itr!=TLunaEngine::TLInputMsgMgr::m_CalledFuncList.end();++itr)
	{
		ProcessInput p = (*itr);
		if(p==&OnCatchInputMsg)
		{
			p = 0;
			TLunaEngine::TLInputMsgMgr::m_CalledFuncList.erase(itr);
			break;
		}
	}
	// 销毁Input
	TGameInput::getSingletonPtr()->DestroyInput();
	TGameInput::delSingletonPtr();
	// 场景管理
	TLGUISceneMgr::getSingletonPtr()->DestroyGUISceneMgr();
	TLGUISceneMgr::delSingletonPtr();
	// Engine销毁
	TLunaEngine::Destroy();
	// 销毁主窗口
	TLMainWindow::getSingletonPtr()->DestroyWindow();
	TLMainWindow::delSingletonPtr();
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
	TGameInput::getSingletonPtr()->UpdateInput(&TLunaEngine::TLInputMsgMgr::CatchInputMsg);
}

void OnCatchInputMsg(BYTE yType,void* param)
{
	// GUI捕获
	TLGUISceneMgr::getSingletonPtr()->CatchInputMsg(yType,param);
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