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
	if(!CLuaInit::getSingletonPtr()->InitWindowScript(pInitParam->szScriptFile))
		return false;
	CLuaInit::getSingletonPtr()->LoadParameters();
	// ����������
	// ���ýṹ��
	_MAINWNDCONFIG wndConfig;
	wndConfig.bufferHeight = CLuaInit::getSingletonPtr()->m_bufferHeight;
	wndConfig.bufferWidth = CLuaInit::getSingletonPtr()->m_bufferWidth;
	wndConfig.bWnd = CLuaInit::getSingletonPtr()->m_bWnd;
	memcpy_s(wndConfig.szWindowText,sizeof(char)*256,CLuaInit::getSingletonPtr()->m_szWindowText,sizeof(char)*256);
	// ������ʼ��
	TLMainWindow* mainWnd = TLMainWindow::getSingletonPtr();
	if(mainWnd->InitWindow(pInitParam->hInst,pInitParam->nCmdShow,&wndConfig)!=S_OK)
		return false;
	mainWnd->CalcRectPro();
	// ��ʼ��Engine
	if(!TLunaEngine::Init(mainWnd->GetHwnd(),pInitParam->hInst,CLuaInit::getSingletonPtr()->m_bWnd,
		CLuaInit::getSingletonPtr()->m_bufferWidth,CLuaInit::getSingletonPtr()->m_bufferHeight,1.0f/CLuaInit::getSingletonPtr()->m_controlFps,
		CLuaInit::getSingletonPtr()->m_szResDir,CLuaInit::getSingletonPtr()->m_bShowDebugInfo))
		return false;
	// GUI��������
	TLGUISceneMgr* pGUISceneMgr = TLGUISceneMgr::getSingletonPtr();
	pGUISceneMgr->InitGUISceneMgr();
	TLunaEngine::TLGUIMgr::getSingletonPtr()->SetSceneListener(pGUISceneMgr);
	// ����Input�豸
	TGameInput* pInput = TGameInput::getSingletonPtr();
	if(!pInput->InitInput(mainWnd->GetHwnd(),pInitParam->hInst,CLuaInit::getSingletonPtr()->m_bUseJoystick))
		return false;
	TLunaEngine::TLInputMsgMgr::m_CalledFuncList.push_back(&OnCatchInputMsg);
	// �������GUI
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
	// ����Input���պ���
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
	// ����Input
	TGameInput::getSingletonPtr()->DestroyInput();
	TGameInput::delSingletonPtr();
	// ��������
	TLGUISceneMgr::getSingletonPtr()->DestroyGUISceneMgr();
	TLGUISceneMgr::delSingletonPtr();
	// Engine����
	TLunaEngine::Destroy();
	// ����������
	TLMainWindow::getSingletonPtr()->DestroyWindow();
	TLMainWindow::delSingletonPtr();
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
	TGameInput::getSingletonPtr()->UpdateInput(&TLunaEngine::TLInputMsgMgr::CatchInputMsg);
}

void OnCatchInputMsg(BYTE yType,void* param)
{
	// GUI����
	TLGUISceneMgr::getSingletonPtr()->CatchInputMsg(yType,param);
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