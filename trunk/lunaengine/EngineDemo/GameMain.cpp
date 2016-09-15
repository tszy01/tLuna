#include "GameMain.h"
#include "GameInput.h"
#include "InputMsgMgr.h"
#include "GUISceneMgr.h"

#include "MainWindow.h"
#include "LuaInitApp.h"
#include "TLGlobleClass.h"
#include "TLGUIMgr.h"

#include "TLString.h"
#include "TLEditorFunc.h"

int MainExampleGame(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmLine, int nCmdShow)
{
	// ����һ����ʼ���ṹ��
	_INITPARAM init;
	init.hInst = hInstance;
	init.nCmdShow = nCmdShow;
	// initapp.lua�����У����ҷ���exeĿ¼����
	sprintf_s(init.szScriptFile,sizeof(char)*256,"initapp.lua");
	// ���ó�ʼ��
	if(InitGame(&init))
	{
		// ��Ϸѭ��
		GameLoop(&ExampleGameLoop);
		// ����
		DestroyGame();
	}
	return 1;
}

bool InitGame(const _INITPARAM* pInitParam)
{
	if(!pInitParam)
		return false;
	// ��ȡ��ʼ������
	if(!LuaInit::getSingletonPtr()->InitWindowScript(pInitParam->szScriptFile))
		return false;
	LuaInit::getSingletonPtr()->LoadParameters();
	// ����������
	// ���ýṹ��
	_MAINWNDCONFIG wndConfig;
	wndConfig.bufferHeight = LuaInit::getSingletonPtr()->m_bufferHeight;
	wndConfig.bufferWidth = LuaInit::getSingletonPtr()->m_bufferWidth;
	wndConfig.bWnd = LuaInit::getSingletonPtr()->m_bWnd;
	memcpy_s(wndConfig.szWindowText,sizeof(char)*256,LuaInit::getSingletonPtr()->m_szWindowText,sizeof(char)*256);
	// ������ʼ��
	MainWindow* mainWnd = MainWindow::getSingletonPtr();
	if(mainWnd->InitWindow(pInitParam->hInst,pInitParam->nCmdShow,&wndConfig)!=S_OK)
		return false;
	mainWnd->CalcRectPro();
	// ��ʼ��Engine
	if(!TLunaEngine::Init(mainWnd->GetHwnd(),pInitParam->hInst,LuaInit::getSingletonPtr()->m_bWnd,
		LuaInit::getSingletonPtr()->m_bufferWidth,LuaInit::getSingletonPtr()->m_bufferHeight,1.0f/LuaInit::getSingletonPtr()->m_controlFps,
		LuaInit::getSingletonPtr()->m_szResDir,LuaInit::getSingletonPtr()->m_bShowDebugInfo))
		return false;
	// GUI��������
	GUISceneMgr* pGUISceneMgr = GUISceneMgr::getSingletonPtr();
	pGUISceneMgr->InitGUISceneMgr();
	TLunaEngine::GUIMgr::getSingletonPtr()->SetSceneListener(pGUISceneMgr);
	// ����Input�豸
	GameInput* pInput = GameInput::getSingletonPtr();
	if(!pInput->InitInput(mainWnd->GetHwnd(),pInitParam->hInst,LuaInit::getSingletonPtr()->m_bUseJoystick))
		return false;
	InputMsgMgr::m_CalledFuncList.push_back(&OnCatchInputMsg);
	// �������GUI
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
	// ����Input���պ���
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
	// ����Input
	GameInput::getSingletonPtr()->DestroyInput();
	GameInput::delSingletonPtr();
	// ��������
	GUISceneMgr::getSingletonPtr()->DestroyGUISceneMgr();
	GUISceneMgr::delSingletonPtr();
	// Engine����
	TLunaEngine::Destroy();
	// ����������
	MainWindow::getSingletonPtr()->DestroyWindow();
	MainWindow::delSingletonPtr();
}

bool GameLoop(TLunaEngine::UserLoop pLoop)
{
	MSG msg;
	BOOL bDrawCursor = FALSE;
	// ѭ����������Ϣ
	while(true)
	{
		//һ����˵��GetMessage��������� ��Ч�Ĵ���Ϣ
		//�����л�ȡ��Ϣ,���������û����Ϣ����ô����
		//�߳�˯��״̬,��PeekMessage���ᵼ�£�������
		//����0��
		if(PeekMessage(&msg,NULL,0U,0U,PM_REMOVE))
			// ����ϵͳ��Ϣ(&msgΪMSG���͵���Ϣ�ṹ�壬NULL
			//�Ǵ��ھ����0��0�Ǳ�ʾ�������д��ڵ���Ϣ)
		{
			// ����һЩ��Ϣ
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
			TranslateMessage(&msg); //ת����Ϣ
			DispatchMessage(&msg);  // 
		}
		else
		{
			// ��Ϸѭ��
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
	// GUI����
	GUISceneMgr::getSingletonPtr()->CatchInputMsg(yType,param);
	// ȫ�ֲ���
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
			int re = TLunaEngine::LaunchEditor(hInstance, 1280, 800, "../../demores/");
			return re;
		}
	}
	return MainExampleGame(hInstance, hPrevInstance, lpCmLine, nCmdShow);
}