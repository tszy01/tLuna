#include "TLGlobalFunc.h"
#include "TLRenderMgr.h"
#include "TLRenderDevice.h"
#include "TLResMgr.h"
#include "TLLog.h"
#include "TLGUIFontManager.h"
#include "TLGUITextureMgr.h"
#include "TLGUIMgr.h"

#include "TLGlobleClass.h"
#include "TLString.h"

namespace TLunaEngine{

	// Initialize Engine
	bool Init(HWND hWnd,HINSTANCE hInst,TBOOL bWnd,TS32 lWidth,TS32 lHeight,
							 float fSecsPerFrame,const char* szResDir,bool bShowDebugInfo)
	{
		// ��¼��Դ��Ŀ¼
		GlobleClass::getSingletonPtr()->m_strResDir = TLunaEngine::String(szResDir);
		// ��ʼ����־
		TLunaEngine::String strLogConfig = GlobleClass::getSingletonPtr()->m_strResDir + "config\\logconfig.txt";
		TLunaEngine::String strLogDir = GlobleClass::getSingletonPtr()->m_strResDir + "log\\";
		if(!TLunaEngine::Log::InitLogSystem(strLogConfig.GetString(),strLogDir.GetString()))
			return false;
		// ����D3D�豸
		TLunaEngine::RenderDevice* device = TLunaEngine::RenderMgr::getSingletonPtr()->createRenderDevice(TLunaEngine::RenderMgr::DT_D3D11);
		if(!device)
			return false;
		if(!device->InitDevice(hWnd,bWnd,lWidth,lHeight))
			return false;
		// ��ʼ����Դ��������
		ResMgr* pResMgr = ResMgr::getSingletonPtr();

		// ---------------- GUI���� -------------------------
		// �������
		GUIFontManager* pFontMgr = GUIFontManager::getSingletonPtr();
		TLunaEngine::String strFontEffect = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if(!pFontMgr->Init(strFontEffect.GetString(),lWidth,lHeight))
			return false;
		// UI��Ⱦ
		GUITextureMgr* pGUIRender = GUITextureMgr::getSingletonPtr();
		TLunaEngine::String strGUIRenderEffect = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if (!pGUIRender->Init(lWidth,lHeight,strGUIRenderEffect.GetString()))
		{
			return false;
		}
		// UI����
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();

		// GlobleClass
		if (!GlobleClass::getSingletonPtr()->InitGlobleClass())
		{
			return false;
		}
		GlobleClass::getSingletonPtr()->m_bShowDebugInfo = bShowDebugInfo;

		return true;
	}

	bool OnLoopCtrlUpdate(float* pTimeElapsed)
	{
		// ʱ�����
		GlobleClass::getSingletonPtr()->updateLoopCtrl();
		// ���ʱ������оͷ���false
		if (GlobleClass::getSingletonPtr()->getLoopCtrlCanRender()==false)
			return true;
		// ����fps
		GlobleClass::getSingletonPtr()->calcFPS();
		if (pTimeElapsed)
		{
			*pTimeElapsed = (float)GlobleClass::getSingletonPtr()->getElapsedTime();
		}
		return true;
	}

	// Loop Engine
	bool OnSceneUpdate(float fTimeElapsed)
	{
		// -------- GUI���� -------------------------
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();
		pGUIMgr->Update(fTimeElapsed);
		// GlobleClass
		GlobleClass::getSingletonPtr()->OnUpdate(fTimeElapsed);
		return true;
	}

	bool OnSceneRender(float fTimeElapsed)
	{
		// ��Ⱦѭ��
		TLunaEngine::RenderDevice* device = TLunaEngine::RenderMgr::getSingletonPtr()->getDevice();
		device->BeginRender();
		// -------- GUI���� -------------------------
		// GUI����Ҫ������ٻ���GUIȡ������ȴ���
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();
		pGUIMgr->Render(fTimeElapsed);
		// GlobleClass
		GlobleClass::getSingletonPtr()->OnRender(fTimeElapsed);
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
		GlobleClass::getSingletonPtr()->DestroyGlobleClass();
		GlobleClass::delSingletonPtr();
		// UI����
		GUIMgr::delSingletonPtr();
		// UI��Ⱦ
		GUITextureMgr::delSingletonPtr();
		// �������
		GUIFontManager::getSingletonPtr()->UseFont(-1);
		GUIFontManager::delSingletonPtr();
		// ��Դ��������
		ResMgr::delSingletonPtr();
		// �����豸
		TLunaEngine::RenderMgr::getSingletonPtr()->getDevice()->DestroyDevice();
		TLunaEngine::RenderMgr::getSingletonPtr()->destroyRenderDevice();
		TLunaEngine::RenderMgr::delSingletonPtr();
		// ��־ϵͳ
		TLunaEngine::Log::DestroyLogSystem();
		return true;
	}

	// window size changed msg
	bool onWindowSizeChanged(TU32 width,TU32 height)
	{
		if(!RenderMgr::getSingletonPtr()->resizeDeviceBuffer(width,height))
			return false;
		return true;
	}

	// ------------- �����Ǹ��༭�����õķ�������Ϸ�в���ʹ�� --------------------------------

	// ��ʼ��
	bool InitForEditor(HWND hWnd,HINSTANCE hInst,TS32 lWidth,TS32 lHeight,const char* szResDir)
	{
		// ��¼��Դ��Ŀ¼
		GlobleClass::getSingletonPtr()->m_strResDir = TLunaEngine::String(szResDir);
		// ��ʼ����־
		TLunaEngine::String strLogConfig = GlobleClass::getSingletonPtr()->m_strResDir + "config\\logconfig.txt";
		TLunaEngine::String strLogDir = GlobleClass::getSingletonPtr()->m_strResDir + "log\\";
		if(!TLunaEngine::Log::InitLogSystem(strLogConfig.GetString(),strLogDir.GetString()))
			return false;
		// ����D3D�豸
		TLunaEngine::RenderDevice* device = TLunaEngine::RenderMgr::getSingletonPtr()->createRenderDevice(TLunaEngine::RenderMgr::DT_D3D11);
		if(!device)
			return false;
		if(!device->InitDevice(hWnd,TTRUE,lWidth,lHeight))
			return false;
		// ��ʼ����Դ��������
		ResMgr* pResMgr = ResMgr::getSingletonPtr();

		// ---------------- GUI���� -------------------------
		// �������
		GUIFontManager* pFontMgr = GUIFontManager::getSingletonPtr();
		TLunaEngine::String strFontEffect = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if(!pFontMgr->Init(strFontEffect.GetString(),lWidth,lHeight))
			return false;
		// UI��Ⱦ
		GUITextureMgr* pGUIRender = GUITextureMgr::getSingletonPtr();
		TLunaEngine::String strGUIRenderEffect = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if (!pGUIRender->Init(lWidth,lHeight,strGUIRenderEffect.GetString()))
		{
			return false;
		}
		// UI����
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();

		// GlobleClass
		if (!GlobleClass::getSingletonPtr()->InitGlobleClass())
		{
			return false;
		}
		GlobleClass::getSingletonPtr()->m_bShowDebugInfo = false;
		GlobleClass::getSingletonPtr()->m_bEditor = true;
		return true;
	}

	// ѭ��
	bool OnSceneUpdateForEditor(float fTimeElapsed)
	{
		// -------- GUI���� -------------------------
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();
		pGUIMgr->Update(fTimeElapsed);
		// GlobleClass
		GlobleClass::getSingletonPtr()->OnUpdate(fTimeElapsed);
		return true;
	}

	bool OnSceneRenderForEditor(float fTimeElapsed)
	{
		// ��Ⱦѭ��
		TLunaEngine::RenderDevice* device = TLunaEngine::RenderMgr::getSingletonPtr()->getDevice();
		device->BeginRender();
		// -------- GUI���� -------------------------
		// GUI����Ҫ������ٻ���GUIȡ������ȴ���
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();
		pGUIMgr->Render(fTimeElapsed);
		// GlobleClass
		GlobleClass::getSingletonPtr()->OnRender(fTimeElapsed);
		// ������Ⱦ
		device->EndRender();
		return true;
	}

	// ����
	bool DestroyForEditor()
	{
		// GlobleClass
		GlobleClass::getSingletonPtr()->DestroyGlobleClass();
		GlobleClass::delSingletonPtr();
		// UI����
		GUIMgr::delSingletonPtr();
		// UI��Ⱦ
		GUITextureMgr::delSingletonPtr();
		// �������
		GUIFontManager::getSingletonPtr()->UseFont(-1);
		GUIFontManager::delSingletonPtr();
		// ��Դ��������
		ResMgr::delSingletonPtr();
		// �����豸
		TLunaEngine::RenderMgr::getSingletonPtr()->getDevice()->DestroyDevice();
		TLunaEngine::RenderMgr::getSingletonPtr()->destroyRenderDevice();
		TLunaEngine::RenderMgr::delSingletonPtr();
		// ��־ϵͳ
		TLunaEngine::Log::DestroyLogSystem();
		return true;
	}

}