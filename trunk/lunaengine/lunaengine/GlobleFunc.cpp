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
		// 记录资源根目录
		TLGlobleClass::getSingletonPtr()->m_strResDir = TLunaEngine::TString(szResDir);
		// 初始化日志
		TLunaEngine::TString strLogConfig = TLGlobleClass::getSingletonPtr()->m_strResDir + "config\\logconfig.txt";
		TLunaEngine::TString strLogDir = TLGlobleClass::getSingletonPtr()->m_strResDir + "log\\";
		if(!TLunaEngine::TLog::InitLogSystem(strLogConfig.GetString(),strLogDir.GetString()))
			return false;
		// 建立D3D设备
		TLunaEngine::TLIRenderDevice* device = TLunaEngine::TLRenderMgr::getSingletonPtr()->createRenderDevice(TLunaEngine::TLRenderMgr::DT_D3D11);
		if(!device)
			return false;
		if(!device->InitDevice(hWnd,bWnd,lWidth,lHeight))
			return false;
		// 初始化资源操作管理
		TLResMgr* pResMgr = TLResMgr::getSingletonPtr();
		// 创建时间控制
		TLLoopCtrl* pTimeCtrl = TLLoopCtrl::getSingletonPtr();
		pTimeCtrl->Init();
		pTimeCtrl->StartTime();

		// ---------------- GUI部分 -------------------------
		// 字体管理
		TLGUIFontManager* pFontMgr = TLGUIFontManager::getSingletonPtr();
		TLunaEngine::TString strFontEffect = TLGlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if(!pFontMgr->Init(strFontEffect.GetString(),lWidth,lHeight))
			return false;
		// UI渲染
		TLGUITextureMgr* pGUIRender = TLGUITextureMgr::getSingletonPtr();
		TLunaEngine::TString strGUIRenderEffect = TLGlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if (!pGUIRender->Init(lWidth,lHeight,strGUIRenderEffect.GetString()))
		{
			return false;
		}
		// UI管理
		TLGUIMgr* pGUIMgr = TLGUIMgr::getSingletonPtr();

		// 初始化IO
		// 添加捕获消息
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
		// 时间控制
		TLLoopCtrl* pTimeCtrl = TLLoopCtrl::getSingletonPtr();
		pTimeCtrl->UpdateTime();
		// 如果时间控制中就返回false
		if(pTimeCtrl->CanRender()==false)
			return false;
		// 计算fps
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
		// -------- GUI部分 -------------------------
		TLGUIMgr* pGUIMgr = TLGUIMgr::getSingletonPtr();
		pGUIMgr->Update(fTimeElapsed);
		// GlobleClass
		TLGlobleClass::getSingletonPtr()->OnUpdate(fTimeElapsed);
		return true;
	}

	bool OnSceneRender(float fTimeElapsed)
	{
		// 渲染循环
		TLunaEngine::TLIRenderDevice* device = TLunaEngine::TLRenderMgr::getSingletonPtr()->getDevice();
		device->BeginRender();
		// -------- GUI部分 -------------------------
		// GUI部分要求最后再画，GUI取消的深度处理
		TLGUIMgr* pGUIMgr = TLGUIMgr::getSingletonPtr();
		pGUIMgr->Render(fTimeElapsed);
		// GlobleClass
		TLGlobleClass::getSingletonPtr()->OnRender(fTimeElapsed);
		// 结束渲染
		device->EndRender();
		return true;
	}

	bool OnGameLoop(UserLoop pLoop)
	{
		// 先计算帧控制
		float timeElapsed = 0;
		if (TLunaEngine::OnLoopCtrlUpdate(&timeElapsed))
		{
			// 调用上层逻辑
			if(pLoop)
				pLoop(timeElapsed);
			// 引擎循环
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
		// UI管理
		TLGUIMgr::delSingletonPtr();
		// UI渲染
		TLGUITextureMgr::delSingletonPtr();
		// 字体管理
		TLGUIFontManager::getSingletonPtr()->UseFont(-1);
		TLGUIFontManager::delSingletonPtr();
		// 时间控制
		TLLoopCtrl::getSingletonPtr()->StopTime();
		TLLoopCtrl::delSingletonPtr();
		// 资源操作管理
		TLResMgr::delSingletonPtr();
		// 销毁设备
		TLunaEngine::TLRenderMgr::getSingletonPtr()->getDevice()->DestroyDevice();
		TLunaEngine::TLRenderMgr::getSingletonPtr()->destroyRenderDevice();
		TLunaEngine::TLRenderMgr::delSingletonPtr();
		// 日志系统
		TLunaEngine::TLog::DestroyLogSystem();
		return true;
	}

	// 截获IO消息
	void CatchInputMsg(BYTE yType,void* param)
	{
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

	// window size changed msg
	bool onWindowSizeChanged(UINT width,UINT height)
	{
		if(!TLRenderMgr::getSingletonPtr()->resizeDeviceBuffer(width,height))
			return false;
		return true;
	}

	// ------------- 以下是给编辑器调用的方法，游戏中不能使用 --------------------------------

	// 初始化
	bool InitForEditor(HWND hWnd,HINSTANCE hInst,LONG lWidth,LONG lHeight,const char* szResDir)
	{
		// 记录资源根目录
		TLGlobleClass::getSingletonPtr()->m_strResDir = TLunaEngine::TString(szResDir);
		// 初始化日志
		TLunaEngine::TString strLogConfig = TLGlobleClass::getSingletonPtr()->m_strResDir + "config\\logconfig.txt";
		TLunaEngine::TString strLogDir = TLGlobleClass::getSingletonPtr()->m_strResDir + "log\\";
		if(!TLunaEngine::TLog::InitLogSystem(strLogConfig.GetString(),strLogDir.GetString()))
			return false;
		// 建立D3D设备
		TLunaEngine::TLIRenderDevice* device = TLunaEngine::TLRenderMgr::getSingletonPtr()->createRenderDevice(TLunaEngine::TLRenderMgr::DT_D3D11);
		if(!device)
			return false;
		if(!device->InitDevice(hWnd,TRUE,lWidth,lHeight))
			return false;
		// 初始化资源操作管理
		TLResMgr* pResMgr = TLResMgr::getSingletonPtr();
		// 创建时间控制
		TLLoopCtrl* pTimeCtrl = TLLoopCtrl::getSingletonPtr();
		pTimeCtrl->Init();
		pTimeCtrl->StartTime();

		// ---------------- GUI部分 -------------------------
		// 字体管理
		TLGUIFontManager* pFontMgr = TLGUIFontManager::getSingletonPtr();
		TLunaEngine::TString strFontEffect = TLGlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if(!pFontMgr->Init(strFontEffect.GetString(),lWidth,lHeight))
			return false;
		// UI渲染
		TLGUITextureMgr* pGUIRender = TLGUITextureMgr::getSingletonPtr();
		TLunaEngine::TString strGUIRenderEffect = TLGlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if (!pGUIRender->Init(lWidth,lHeight,strGUIRenderEffect.GetString()))
		{
			return false;
		}
		// UI管理
		TLGUIMgr* pGUIMgr = TLGUIMgr::getSingletonPtr();

		// 初始化IO
		// 添加捕获消息
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

	// 循环
	bool OnSceneUpdateForEditor(float fTimeElapsed)
	{
		// -------- GUI部分 -------------------------
		TLGUIMgr* pGUIMgr = TLGUIMgr::getSingletonPtr();
		pGUIMgr->Update(fTimeElapsed);
		// GlobleClass
		TLGlobleClass::getSingletonPtr()->OnUpdate(fTimeElapsed);
		return true;
	}

	bool OnSceneRenderForEditor(float fTimeElapsed)
	{
		// 渲染循环
		TLunaEngine::TLIRenderDevice* device = TLunaEngine::TLRenderMgr::getSingletonPtr()->getDevice();
		device->BeginRender();
		// -------- GUI部分 -------------------------
		// GUI部分要求最后再画，GUI取消的深度处理
		TLGUIMgr* pGUIMgr = TLGUIMgr::getSingletonPtr();
		pGUIMgr->Render(fTimeElapsed);
		// GlobleClass
		TLGlobleClass::getSingletonPtr()->OnRender(fTimeElapsed);
		// 结束渲染
		device->EndRender();
		return true;
	}

	// 销毁
	bool DestroyForEditor()
	{
		// GlobleClass
		TLGlobleClass::getSingletonPtr()->DestroyGlobleClass();
		TLGlobleClass::delSingletonPtr();
		// UI管理
		TLGUIMgr::delSingletonPtr();
		// UI渲染
		TLGUITextureMgr::delSingletonPtr();
		// 字体管理
		TLGUIFontManager::getSingletonPtr()->UseFont(-1);
		TLGUIFontManager::delSingletonPtr();
		// 时间控制
		TLLoopCtrl::getSingletonPtr()->StopTime();
		TLLoopCtrl::delSingletonPtr();
		// 资源操作管理
		TLResMgr::delSingletonPtr();
		// 销毁设备
		TLunaEngine::TLRenderMgr::getSingletonPtr()->getDevice()->DestroyDevice();
		TLunaEngine::TLRenderMgr::getSingletonPtr()->destroyRenderDevice();
		TLunaEngine::TLRenderMgr::delSingletonPtr();
		// 日志系统
		TLunaEngine::TLog::DestroyLogSystem();
		return true;
	}

}