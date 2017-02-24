#include "TLGlobalFunc.h"
#include "TLRenderMgr.h"
#include "TLRenderDevice.h"
#include "TLResMgr.h"
#include "TSLog.h"
#include "TLGUIFontManager.h"
#include "TLGUITextureMgr.h"
#include "TLGUIMgr.h"

#include "TLGlobleClass.h"
#include "TSString.h"

namespace TLunaEngine{

	// Initialize Engine
	TSun::TBOOL Init(HWND hWnd,HINSTANCE hInst,TSun::TBOOL bWnd,TSun::TS32 lWidth,TSun::TS32 lHeight,
		TSun::TF32 fSecsPerFrame,const TSun::TCHAR* szResDir, const TSun::TCHAR* szSysLangDictFile, 
		TSun::TBOOL bShowDebugInfo)
	{
		// 记录资源根目录
		GlobleClass::getSingletonPtr()->m_strResDir = TSun::String(szResDir);
		GlobleClass::getSingletonPtr()->mLangDictFile = TSun::String(szSysLangDictFile);
		// 初始化日志
		TSun::String strLogConfig = GlobleClass::getSingletonPtr()->m_strResDir + "config\\logconfig.txt";
		TSun::String strLogDir = GlobleClass::getSingletonPtr()->m_strResDir + "log\\";
		if (!TSun::Log::InitLogSystem(strLogConfig.GetString(), strLogDir.GetString()))
			return TSun::TFALSE;
		// 建立D3D设备
		TLunaEngine::RenderDevice* device = TLunaEngine::RenderMgr::getSingletonPtr()->createRenderDevice(TLunaEngine::RenderMgr::DT_D3D11);
		if(!device)
			return TSun::TFALSE;
		if(!device->InitDevice(hWnd,bWnd,lWidth,lHeight))
			return TSun::TFALSE;
		// 初始化资源操作管理
		ResMgr* pResMgr = ResMgr::getSingletonPtr();

		// ---------------- GUI部分 -------------------------
		// 字体管理
		GUIFontManager* pFontMgr = GUIFontManager::getSingletonPtr();
		TSun::String strFontEffect = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if(!pFontMgr->Init(strFontEffect.GetString(),lWidth,lHeight))
			return TSun::TFALSE;
		// UI渲染
		GUITextureMgr* pGUIRender = GUITextureMgr::getSingletonPtr();
		TSun::String strGUIRenderEffect = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if (!pGUIRender->Init(lWidth,lHeight,strGUIRenderEffect.GetString()))
		{
			return TSun::TFALSE;
		}
		// UI管理
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();

		// GlobleClass
		if (!GlobleClass::getSingletonPtr()->InitGlobleClass())
		{
			return TSun::TFALSE;
		}
		GlobleClass::getSingletonPtr()->m_bShowDebugInfo = bShowDebugInfo;

		return TSun::TTRUE;
	}

	TSun::TBOOL OnLoopCtrlUpdate(TSun::TF32* pTimeElapsed)
	{
		// 时间控制
		GlobleClass::getSingletonPtr()->updateLoopCtrl();
		// 如果时间控制中就返回TSun::TFALSE
		if (GlobleClass::getSingletonPtr()->getLoopCtrlCanRender()==TSun::TFALSE)
			return TSun::TTRUE;
		// 计算fps
		GlobleClass::getSingletonPtr()->calcFPS();
		if (pTimeElapsed)
		{
			*pTimeElapsed = (TSun::TF32)GlobleClass::getSingletonPtr()->getElapsedTime();
		}
		return TSun::TTRUE;
	}

	// Loop Engine
	TSun::TBOOL OnSceneUpdate(TSun::TF32 fTimeElapsed)
	{
		// -------- GUI部分 -------------------------
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();
		pGUIMgr->Update(fTimeElapsed);
		// GlobleClass
		GlobleClass::getSingletonPtr()->OnUpdate(fTimeElapsed);
		return TSun::TTRUE;
	}

	TSun::TBOOL OnSceneRender(TSun::TF32 fTimeElapsed)
	{
		// 渲染循环
		TLunaEngine::RenderDevice* device = TLunaEngine::RenderMgr::getSingletonPtr()->getDevice();
		device->BeginRender();
		// -------- GUI部分 -------------------------
		// GUI部分要求最后再画，GUI取消的深度处理
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();
		pGUIMgr->Render(fTimeElapsed);
		// GlobleClass
		GlobleClass::getSingletonPtr()->OnRender(fTimeElapsed);
		// 结束渲染
		device->EndRender();
		return TSun::TTRUE;
	}

	TSun::TBOOL OnGameLoop(UserLoop pLoop)
	{
		// 先计算帧控制
		TSun::TF32 timeElapsed = 0;
		if (TLunaEngine::OnLoopCtrlUpdate(&timeElapsed))
		{
			// 调用上层逻辑
			if(pLoop)
				pLoop(timeElapsed);
			// 引擎循环
			if(TLunaEngine::OnSceneUpdate(timeElapsed))
			{
				if(TLunaEngine::OnSceneRender(timeElapsed))
					return TSun::TTRUE;
			}
		}
		return TSun::TFALSE;
	}

	// Destroy Engine
	TSun::TBOOL Destroy()
	{
		// GlobleClass
		GlobleClass::getSingletonPtr()->DestroyGlobleClass();
		GlobleClass::delSingletonPtr();
		// UI管理
		GUIMgr::delSingletonPtr();
		// UI渲染
		GUITextureMgr::delSingletonPtr();
		// 字体管理
		GUIFontManager::getSingletonPtr()->UseFont(-1);
		GUIFontManager::delSingletonPtr();
		// 资源操作管理
		ResMgr::delSingletonPtr();
		// 销毁设备
		TLunaEngine::RenderMgr::getSingletonPtr()->getDevice()->DestroyDevice();
		TLunaEngine::RenderMgr::getSingletonPtr()->destroyRenderDevice();
		TLunaEngine::RenderMgr::delSingletonPtr();
		// 日志系统
		TSun::Log::DestroyLogSystem();
		return TSun::TTRUE;
	}

	// window size changed msg
	TSun::TBOOL onWindowSizeChanged(TSun::TU32 width,TSun::TU32 height)
	{
		if(!RenderMgr::getSingletonPtr()->resizeDeviceBuffer(width,height))
			return TSun::TFALSE;
		return TSun::TTRUE;
	}

	// ------------- 以下是给编辑器调用的方法，游戏中不能使用 --------------------------------

	// 初始化
	TSun::TBOOL InitForEditor(HWND hWnd,HINSTANCE hInst,TSun::TS32 lWidth,TSun::TS32 lHeight,const TSun::TCHAR* szResDir)
	{
		// 记录资源根目录
		GlobleClass::getSingletonPtr()->m_strResDir = TSun::String(szResDir);
		// 初始化日志
		TSun::String strLogConfig = GlobleClass::getSingletonPtr()->m_strResDir + "config\\logconfig.txt";
		TSun::String strLogDir = GlobleClass::getSingletonPtr()->m_strResDir + "log\\";
		if (!TSun::Log::InitLogSystem(strLogConfig.GetString(), strLogDir.GetString()))
			return TSun::TFALSE;
		// 建立D3D设备
		TLunaEngine::RenderDevice* device = TLunaEngine::RenderMgr::getSingletonPtr()->createRenderDevice(TLunaEngine::RenderMgr::DT_D3D11);
		if(!device)
			return TSun::TFALSE;
		if(!device->InitDevice(hWnd,TSun::TTRUE,lWidth,lHeight))
			return TSun::TFALSE;
		// 初始化资源操作管理
		ResMgr* pResMgr = ResMgr::getSingletonPtr();

		// ---------------- GUI部分 -------------------------
		// 字体管理
		GUIFontManager* pFontMgr = GUIFontManager::getSingletonPtr();
		TSun::String strFontEffect = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if(!pFontMgr->Init(strFontEffect.GetString(),lWidth,lHeight))
			return TSun::TFALSE;
		// UI渲染
		GUITextureMgr* pGUIRender = GUITextureMgr::getSingletonPtr();
		TSun::String strGUIRenderEffect = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if (!pGUIRender->Init(lWidth,lHeight,strGUIRenderEffect.GetString()))
		{
			return TSun::TFALSE;
		}
		// UI管理
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();

		// GlobleClass
		if (!GlobleClass::getSingletonPtr()->InitGlobleClass())
		{
			return TSun::TFALSE;
		}
		GlobleClass::getSingletonPtr()->m_bShowDebugInfo = TSun::TFALSE;
		GlobleClass::getSingletonPtr()->m_bEditor = TSun::TTRUE;
		return TSun::TTRUE;
	}

	// 循环
	TSun::TBOOL OnSceneUpdateForEditor(TSun::TF32 fTimeElapsed)
	{
		// -------- GUI部分 -------------------------
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();
		pGUIMgr->Update(fTimeElapsed);
		// GlobleClass
		GlobleClass::getSingletonPtr()->OnUpdate(fTimeElapsed);
		return TSun::TTRUE;
	}

	TSun::TBOOL OnSceneRenderForEditor(TSun::TF32 fTimeElapsed)
	{
		// 渲染循环
		TLunaEngine::RenderDevice* device = TLunaEngine::RenderMgr::getSingletonPtr()->getDevice();
		device->BeginRender();
		// -------- GUI部分 -------------------------
		// GUI部分要求最后再画，GUI取消的深度处理
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();
		pGUIMgr->Render(fTimeElapsed);
		// GlobleClass
		GlobleClass::getSingletonPtr()->OnRender(fTimeElapsed);
		// 结束渲染
		device->EndRender();
		return TSun::TTRUE;
	}

	// 销毁
	TSun::TBOOL DestroyForEditor()
	{
		// GlobleClass
		GlobleClass::getSingletonPtr()->DestroyGlobleClass();
		GlobleClass::delSingletonPtr();
		// UI管理
		GUIMgr::delSingletonPtr();
		// UI渲染
		GUITextureMgr::delSingletonPtr();
		// 字体管理
		GUIFontManager::getSingletonPtr()->UseFont(-1);
		GUIFontManager::delSingletonPtr();
		// 资源操作管理
		ResMgr::delSingletonPtr();
		// 销毁设备
		TLunaEngine::RenderMgr::getSingletonPtr()->getDevice()->DestroyDevice();
		TLunaEngine::RenderMgr::getSingletonPtr()->destroyRenderDevice();
		TLunaEngine::RenderMgr::delSingletonPtr();
		// 日志系统
		TSun::Log::DestroyLogSystem();
		return TSun::TTRUE;
	}

}