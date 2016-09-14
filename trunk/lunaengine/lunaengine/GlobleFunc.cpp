#include "stdafx.h"
#include "GlobleFunc.h"
#include "TLRenderMgr.h"
#include "TLIRenderDevice.h"
#include "TLResMgr.h"
#include "TLog.h"
#include "TLLoopCtrl.h"
#include "TLGUIFontManager.h"
#include "TLGUITextureMgr.h"
#include "TLGUIMgr.h"
#include "TLInputMsgMgr.h"

#include "TLGlobleClass.h"
#include "TString.h"

namespace TLunaEngine{

	// Initialize Engine
	bool Init(HWND hWnd,HINSTANCE hInst,BOOL bWnd,LONG lWidth,LONG lHeight,
							 float fSecsPerFrame,const char* szResDir,bool bShowDebugInfo)
	{
		// ��¼��Դ��Ŀ¼
		TLGlobleClass::getSingletonPtr()->m_strResDir = TLunaEngine::TString(szResDir);
		// ��ʼ����־
		TLunaEngine::TString strLogConfig = TLGlobleClass::getSingletonPtr()->m_strResDir + "config\\logconfig.txt";
		TLunaEngine::TString strLogDir = TLGlobleClass::getSingletonPtr()->m_strResDir + "log\\";
		if(!TLunaEngine::TLog::InitLogSystem(strLogConfig.GetString(),strLogDir.GetString()))
			return false;
		// ����D3D�豸
		TLunaEngine::TLIRenderDevice* device = TLunaEngine::TLRenderMgr::getSingletonPtr()->createRenderDevice(TLunaEngine::TLRenderMgr::DT_D3D11);
		if(!device)
			return false;
		if(!device->InitDevice(hWnd,bWnd,lWidth,lHeight))
			return false;
		// ��ʼ����Դ��������
		TLResMgr* pResMgr = TLResMgr::getSingletonPtr();
		// ����ʱ�����
		TLLoopCtrl* pTimeCtrl = TLLoopCtrl::getSingletonPtr();
		pTimeCtrl->Init();
		pTimeCtrl->StartTime();

		// ---------------- GUI���� -------------------------
		// �������
		TLGUIFontManager* pFontMgr = TLGUIFontManager::getSingletonPtr();
		TLunaEngine::TString strFontEffect = TLGlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if(!pFontMgr->Init(strFontEffect.GetString(),lWidth,lHeight))
			return false;
		// UI��Ⱦ
		TLGUITextureMgr* pGUIRender = TLGUITextureMgr::getSingletonPtr();
		TLunaEngine::TString strGUIRenderEffect = TLGlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if (!pGUIRender->Init(lWidth,lHeight,strGUIRenderEffect.GetString()))
		{
			return false;
		}
		// UI����
		TLGUIMgr* pGUIMgr = TLGUIMgr::getSingletonPtr();

		// ��ʼ��IO
		// ��Ӳ�����Ϣ
		ProcessInput triFunc = &CatchInputMsg;
		TLInputMsgMgr::m_CalledFuncList.push_back(triFunc);

		// GlobleClass
		if (!TLGlobleClass::getSingletonPtr()->InitGlobleClass())
		{
			return false;
		}
		TLGlobleClass::getSingletonPtr()->m_bShowDebugInfo = bShowDebugInfo;

		return true;
	}

	bool OnLoopCtrlUpdate(float* pTimeElapsed)
	{
		// ʱ�����
		TLLoopCtrl* pTimeCtrl = TLLoopCtrl::getSingletonPtr();
		pTimeCtrl->UpdateTime();
		// ���ʱ������оͷ���false
		if(pTimeCtrl->CanRender()==false)
			return false;
		// ����fps
		pTimeCtrl->CalFPS();
		if (pTimeElapsed)
		{
			*pTimeElapsed = (float)pTimeCtrl->GetElapsedTime();
		}
		return true;
	}

	// Loop Engine
	bool OnSceneUpdate(float fTimeElapsed)
	{
		// -------- GUI���� -------------------------
		TLGUIMgr* pGUIMgr = TLGUIMgr::getSingletonPtr();
		pGUIMgr->Update(fTimeElapsed);
		// GlobleClass
		TLGlobleClass::getSingletonPtr()->OnUpdate(fTimeElapsed);
		return true;
	}

	bool OnSceneRender(float fTimeElapsed)
	{
		// ��Ⱦѭ��
		TLunaEngine::TLIRenderDevice* device = TLunaEngine::TLRenderMgr::getSingletonPtr()->getDevice();
		device->BeginRender();
		// -------- GUI���� -------------------------
		// GUI����Ҫ������ٻ���GUIȡ������ȴ���
		TLGUIMgr* pGUIMgr = TLGUIMgr::getSingletonPtr();
		pGUIMgr->Render(fTimeElapsed);
		// GlobleClass
		TLGlobleClass::getSingletonPtr()->OnRender(fTimeElapsed);
		// ������Ⱦ
		device->EndRender();
		return true;
	}

	bool OnGameLoop(UserLoop pLoop)
	{
		// �ȼ���֡����
		float timeElapsed = 0;
		if (TLunaEngine::OnLoopCtrlUpdate(&timeElapsed))
		{
			// �����ϲ��߼�
			if(pLoop)
				pLoop(timeElapsed);
			// ����ѭ��
			if(TLunaEngine::OnSceneUpdate(timeElapsed))
			{
				if(TLunaEngine::OnSceneRender(timeElapsed))
					return true;
			}
		}
		return false;
	}

	// Destroy Engine
	bool Destroy()
	{
		// GlobleClass
		TLGlobleClass::getSingletonPtr()->DestroyGlobleClass();
		TLGlobleClass::delSingletonPtr();
		// UI����
		TLGUIMgr::delSingletonPtr();
		// UI��Ⱦ
		TLGUITextureMgr::delSingletonPtr();
		// �������
		TLGUIFontManager::getSingletonPtr()->UseFont(-1);
		TLGUIFontManager::delSingletonPtr();
		// ʱ�����
		TLLoopCtrl::getSingletonPtr()->StopTime();
		TLLoopCtrl::delSingletonPtr();
		// ��Դ��������
		TLResMgr::delSingletonPtr();
		// �����豸
		TLunaEngine::TLRenderMgr::getSingletonPtr()->getDevice()->DestroyDevice();
		TLunaEngine::TLRenderMgr::getSingletonPtr()->destroyRenderDevice();
		TLunaEngine::TLRenderMgr::delSingletonPtr();
		// ��־ϵͳ
		TLunaEngine::TLog::DestroyLogSystem();
		return true;
	}

	// �ػ�IO��Ϣ
	void CatchInputMsg(BYTE yType,void* param)
	{
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

	// window size changed msg
	bool onWindowSizeChanged(UINT width,UINT height)
	{
		if(!TLRenderMgr::getSingletonPtr()->resizeDeviceBuffer(width,height))
			return false;
		return true;
	}

	// ------------- �����Ǹ��༭�����õķ�������Ϸ�в���ʹ�� --------------------------------

	// ��ʼ��
	bool InitForEditor(HWND hWnd,HINSTANCE hInst,LONG lWidth,LONG lHeight,const char* szResDir)
	{
		// ��¼��Դ��Ŀ¼
		TLGlobleClass::getSingletonPtr()->m_strResDir = TLunaEngine::TString(szResDir);
		// ��ʼ����־
		TLunaEngine::TString strLogConfig = TLGlobleClass::getSingletonPtr()->m_strResDir + "config\\logconfig.txt";
		TLunaEngine::TString strLogDir = TLGlobleClass::getSingletonPtr()->m_strResDir + "log\\";
		if(!TLunaEngine::TLog::InitLogSystem(strLogConfig.GetString(),strLogDir.GetString()))
			return false;
		// ����D3D�豸
		TLunaEngine::TLIRenderDevice* device = TLunaEngine::TLRenderMgr::getSingletonPtr()->createRenderDevice(TLunaEngine::TLRenderMgr::DT_D3D11);
		if(!device)
			return false;
		if(!device->InitDevice(hWnd,TRUE,lWidth,lHeight))
			return false;
		// ��ʼ����Դ��������
		TLResMgr* pResMgr = TLResMgr::getSingletonPtr();
		// ����ʱ�����
		TLLoopCtrl* pTimeCtrl = TLLoopCtrl::getSingletonPtr();
		pTimeCtrl->Init();
		pTimeCtrl->StartTime();

		// ---------------- GUI���� -------------------------
		// �������
		TLGUIFontManager* pFontMgr = TLGUIFontManager::getSingletonPtr();
		TLunaEngine::TString strFontEffect = TLGlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if(!pFontMgr->Init(strFontEffect.GetString(),lWidth,lHeight))
			return false;
		// UI��Ⱦ
		TLGUITextureMgr* pGUIRender = TLGUITextureMgr::getSingletonPtr();
		TLunaEngine::TString strGUIRenderEffect = TLGlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if (!pGUIRender->Init(lWidth,lHeight,strGUIRenderEffect.GetString()))
		{
			return false;
		}
		// UI����
		TLGUIMgr* pGUIMgr = TLGUIMgr::getSingletonPtr();

		// ��ʼ��IO
		// ��Ӳ�����Ϣ
		ProcessInput triFunc = &CatchInputMsg;
		TLInputMsgMgr::m_CalledFuncList.push_back(triFunc);

		// GlobleClass
		if (!TLGlobleClass::getSingletonPtr()->InitGlobleClass())
		{
			return false;
		}
		TLGlobleClass::getSingletonPtr()->m_bShowDebugInfo = false;
		TLGlobleClass::getSingletonPtr()->m_bEditor = true;
		return true;
	}

	// ѭ��
	bool OnSceneUpdateForEditor(float fTimeElapsed)
	{
		// -------- GUI���� -------------------------
		TLGUIMgr* pGUIMgr = TLGUIMgr::getSingletonPtr();
		pGUIMgr->Update(fTimeElapsed);
		// GlobleClass
		TLGlobleClass::getSingletonPtr()->OnUpdate(fTimeElapsed);
		return true;
	}

	bool OnSceneRenderForEditor(float fTimeElapsed)
	{
		// ��Ⱦѭ��
		TLunaEngine::TLIRenderDevice* device = TLunaEngine::TLRenderMgr::getSingletonPtr()->getDevice();
		device->BeginRender();
		// -------- GUI���� -------------------------
		// GUI����Ҫ������ٻ���GUIȡ������ȴ���
		TLGUIMgr* pGUIMgr = TLGUIMgr::getSingletonPtr();
		pGUIMgr->Render(fTimeElapsed);
		// GlobleClass
		TLGlobleClass::getSingletonPtr()->OnRender(fTimeElapsed);
		// ������Ⱦ
		device->EndRender();
		return true;
	}

	// ����
	bool DestroyForEditor()
	{
		// GlobleClass
		TLGlobleClass::getSingletonPtr()->DestroyGlobleClass();
		TLGlobleClass::delSingletonPtr();
		// UI����
		TLGUIMgr::delSingletonPtr();
		// UI��Ⱦ
		TLGUITextureMgr::delSingletonPtr();
		// �������
		TLGUIFontManager::getSingletonPtr()->UseFont(-1);
		TLGUIFontManager::delSingletonPtr();
		// ʱ�����
		TLLoopCtrl::getSingletonPtr()->StopTime();
		TLLoopCtrl::delSingletonPtr();
		// ��Դ��������
		TLResMgr::delSingletonPtr();
		// �����豸
		TLunaEngine::TLRenderMgr::getSingletonPtr()->getDevice()->DestroyDevice();
		TLunaEngine::TLRenderMgr::getSingletonPtr()->destroyRenderDevice();
		TLunaEngine::TLRenderMgr::delSingletonPtr();
		// ��־ϵͳ
		TLunaEngine::TLog::DestroyLogSystem();
		return true;
	}

}