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
#include "ConsoleWindow.h"
#include "TLConsole.h"

#include "ConfigDef.h"

#ifdef BUILD_EDITOR
#include "EditorMgr.h"
#endif // BUILD_EDITOR

#ifdef BUILD_TEST
#include "TLTxtFileReader.h"
#include "TLTxtFileWriter.h"
#include "TLUTF8FileReader.h"
#include "TLUTF8FileWriter.h"
#include "TLBinaryFileProcessor.h"
#include "TLLog.h"
#endif // BUILD_TEST

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
	// Build Console Window
	if (LuaInit::getSingletonPtr()->m_bOpenConsole)
	{
		ConsoleWindow* consoleWnd = ConsoleWindow::getSingletonPtr();
		if (consoleWnd->InitWindow(pInitParam->hInst, pInitParam->nCmdShow, LuaInit::getSingletonPtr()->m_consoleWidth, 
			LuaInit::getSingletonPtr()->m_consoleHeight) != S_OK)
			return false;
	}
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
		LuaInit::getSingletonPtr()->m_szResDir, LuaInit::getSingletonPtr()->m_szSysLangDict,LuaInit::getSingletonPtr()->m_bShowDebugInfo))
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
	// Destory Console Window
	if (LuaInit::getSingletonPtr()->m_bOpenConsole)
	{
		ConsoleWindow::getSingletonPtr()->DestroyWindow();
		ConsoleWindow::delSingletonPtr();
	}
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
bool TestLoop()
{
	//HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return true;
}

int runTest(HINSTANCE hInstance, LPSTR lpCmLine, int nCmdShow)
{
	/*TLunaEngine::SharedPtr<int> a = TLunaEngine::SharedPtr<int>(new int);
	TLunaEngine::SharedPtr<int> b = TLunaEngine::SharedPtr<int>(new int);
	if (a < b)
	{
		printf("a < b\n");
	}*/
	ConsoleWindow* consoleWnd = ConsoleWindow::getSingletonPtr();
	if (consoleWnd->InitWindow(hInstance, nCmdShow, 400, 400) != S_OK)
		return 1;

	consoleWnd->GetConsoleOutput()->addText(L"支払い");
	
	//char a[64] = { "こんにちは" };

	//wchar_t* newBuff = new wchar_t[64];
	////mbstowcs(newBuff,m_szPtr.getPointer(),m_nLength);
	//int toNum = ::MultiByteToWideChar(CP_ACP, 0, a, -1, 0, 0);
	//::MultiByteToWideChar(CP_ACP, 0, a, -1, newBuff, toNum);

	//FILE* fp = NULL;
	//if (fopen_s(&fp, "test2.txt", "w+b") == 0)
	//{
	//	/*char tmp[64] = { "こんにちは" };
	//	toNum = ::WideCharToMultiByte(CP_UTF8, 0, newBuff, -1, 0, 0, 0, 0);
	//	::WideCharToMultiByte(CP_UTF8, 0, newBuff, -1, tmp, toNum, 0, 0);*/

	//	char tmp[64] = { "こんにちは" };
	//	wchar_t tmpW[64] = { L"你好" };

	//	//::MultiByteToWideChar(CP_ACP, 0, tmp, -1, tmpW, 64);
	//	::WideCharToMultiByte(CP_ACP, 0, tmpW, -1, tmp, 64, 0, 0);

	//	fwrite(tmp, 1, strlen(tmp), fp);

	//	fclose(fp);
	//}

	//TLunaEngine::String str("你好");
	//TLunaEngine::SharedPtr<TLunaEngine::TWCHAR> wstr = str.GetWString();
	//TLunaEngine::TWCHAR wsz[64] = { L"しあわせ" };
	//TLunaEngine::String sz(wsz);

	FILE* fp = NULL;
	/*if (fopen_s(&fp, "test2.txt", "wt") == 0)
	{
		fwrite(sz.GetString(), 1, sz.GetLength(), fp);
		fclose(fp);
	}*/

	//if (fopen_s(&fp, "test2.txt", "r+, ccs=UTF-8") == 0)
	//{
	//	//char tmp[64] = { 0 };
	//	//fread(tmp, 1, 64, fp);
	//	//fclose(fp);

	//	//TLunaEngine::String strA(tmp);
	////	strA.ConvertToANSI();
	////	consoleWnd->GetConsoleOutput()->addText(strA);

	//	TLunaEngine::TWCHAR tmp[64] = { L"0" };
	//	fread(tmp, sizeof(TLunaEngine::TWCHAR), 64, fp);
	//	fclose(fp);

	//	consoleWnd->GetConsoleOutput()->addText(/*TLunaEngine::WString(strA.ToWString().getPointer())*/tmp);
	//}

	/*TLunaEngine::UTF8FileReader::OpenTxtFile("test2.txt", &fp);

	TLunaEngine::TWCHAR tmp[64] = { L"0" };
	TLunaEngine::TS32 sTmp[5] = { 0 };
	TLunaEngine::UTF8FileReader::ReadLineWString(tmp, fp, 0, 0, 64, 0);
	TLunaEngine::UTF8FileReader::ReadLineInteger(sTmp, fp, 5, L' ');
	consoleWnd->GetConsoleOutput()->addText(tmp);

	TLunaEngine::UTF8FileReader::CloseTxtFile(fp);*/

	/*TLunaEngine::UTF8FileReader::OpenTxtFile("test2.txt", &fp);

	TLunaEngine::TWCHAR tmp[64] = { L"0" };
	TLunaEngine::TBOOL re = TLunaEngine::TFALSE;
	TLunaEngine::UTF8FileReader::ReadLineWString(tmp, fp, L"你好你是谁是啊", &re, 64, 0);
	consoleWnd->GetConsoleOutput()->addText(tmp);

	TLunaEngine::UTF8FileReader::CloseTxtFile(fp);*/

	/*TLunaEngine::UTF8FileWriter::OpenTxtFile("test3.txt", &fp);

	TLunaEngine::UTF8FileWriter::WriteLineWString(tmp, fp, (TLunaEngine::TU32)wcslen(tmp));
	TLunaEngine::TS32 sTmp[5] = { 8,0,9,10,11 };
	TLunaEngine::UTF8FileWriter::WriteLineInteger(sTmp, fp, 5, L';');

	TLunaEngine::UTF8FileWriter::CloseTxtFile(fp);*/

	TLunaEngine::String strLogConfig = "../../../demores/config\\logconfig.txt";
	TLunaEngine::String strLogDir = "../../../demores/log\\";
	if (!TLunaEngine::Log::InitLogSystem(strLogConfig.GetString(), strLogDir.GetString()))
		return 0;

	TLunaEngine::Log::WriteLine(TLunaEngine::Log::LOG_LEVEL_ERROR, TLunaEngine::TTRUE, L"你好你是谁是啊");

	TestLoop();

	TLunaEngine::Log::DestroyLogSystem();

	ConsoleWindow::getSingletonPtr()->DestroyWindow();
	ConsoleWindow::delSingletonPtr();
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
		return runTest(hInstance, lpCmLine, nCmdShow);
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