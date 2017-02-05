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
	TBOOL Init(HWND hWnd,HINSTANCE hInst,TBOOL bWnd,TS32 lWidth,TS32 lHeight,
		TF32 fSecsPerFrame,const TCHAR* szResDir, const TCHAR* szSysLangDictFile, 
		TBOOL bShowDebugInfo)
	{
		// 记录资源根目录
		GlobleClass::getSingletonPtr()->m_strResDir = TLunaEngine::String(szResDir);
		GlobleClass::getSingletonPtr()->mLangDictFile = TLunaEngine::String(szSysLangDictFile);
		// 初始化日志
		TLunaEngine::String strLogConfig = GlobleClass::getSingletonPtr()->m_strResDir + "config\\logconfig.txt";
		TLunaEngine::String strLogDir = GlobleClass::getSingletonPtr()->m_strResDir + "log\\";
		if (!TLunaEngine::Log::InitLogSystem(strLogConfig.GetString(), strLogDir.GetString()))
			return TFALSE;
		// 建立D3D设备
		TLunaEngine::RenderDevice* device = TLunaEngine::RenderMgr::getSingletonPtr()->createRenderDevice(TLunaEngine::RenderMgr::DT_D3D11);
		if(!device)
			return TFALSE;
		if(!device->InitDevice(hWnd,bWnd,lWidth,lHeight))
			return TFALSE;
		// 初始化资源操作管理
		ResMgr* pResMgr = ResMgr::getSingletonPtr();

		// ---------------- GUI部分 -------------------------
		// 字体管理
		GUIFontManager* pFontMgr = GUIFontManager::getSingletonPtr();
		TLunaEngine::String strFontEffect = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if(!pFontMgr->Init(strFontEffect.GetString(),lWidth,lHeight))
			return TFALSE;
		// UI渲染
		GUITextureMgr* pGUIRender = GUITextureMgr::getSingletonPtr();
		TLunaEngine::String strGUIRenderEffect = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if (!pGUIRender->Init(lWidth,lHeight,strGUIRenderEffect.GetString()))
		{
			return TFALSE;
		}
		// UI管理
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();

		// GlobleClass
		if (!GlobleClass::getSingletonPtr()->InitGlobleClass())
		{
			return TFALSE;
		}
		GlobleClass::getSingletonPtr()->m_bShowDebugInfo = bShowDebugInfo;

		return TTRUE;
	}

	TBOOL OnLoopCtrlUpdate(TF32* pTimeElapsed)
	{
		// 时间控制
		GlobleClass::getSingletonPtr()->updateLoopCtrl();
		// 如果时间控制中就返回TFALSE
		if (GlobleClass::getSingletonPtr()->getLoopCtrlCanRender()==TFALSE)
			return TTRUE;
		// 计算fps
		GlobleClass::getSingletonPtr()->calcFPS();
		if (pTimeElapsed)
		{
			*pTimeElapsed = (TF32)GlobleClass::getSingletonPtr()->getElapsedTime();
		}
		return TTRUE;
	}

	// Loop Engine
	TBOOL OnSceneUpdate(TF32 fTimeElapsed)
	{
		// -------- GUI部分 -------------------------
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();
		pGUIMgr->Update(fTimeElapsed);
		// GlobleClass
		GlobleClass::getSingletonPtr()->OnUpdate(fTimeElapsed);
		return TTRUE;
	}

	TBOOL OnSceneRender(TF32 fTimeElapsed)
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
		return TTRUE;
	}

	TBOOL OnGameLoop(UserLoop pLoop)
	{
		// 先计算帧控制
		TF32 timeElapsed = 0;
		if (TLunaEngine::OnLoopCtrlUpdate(&timeElapsed))
		{
			// 调用上层逻辑
			if(pLoop)
				pLoop(timeElapsed);
			// 引擎循环
			if(TLunaEngine::OnSceneUpdate(timeElapsed))
			{
				if(TLunaEngine::OnSceneRender(timeElapsed))
					return TTRUE;
			}
		}
		return TFALSE;
	}

	// Destroy Engine
	TBOOL Destroy()
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
		TLunaEngine::Log::DestroyLogSystem();
		return TTRUE;
	}

	// window size changed msg
	TBOOL onWindowSizeChanged(TU32 width,TU32 height)
	{
		if(!RenderMgr::getSingletonPtr()->resizeDeviceBuffer(width,height))
			return TFALSE;
		return TTRUE;
	}

	// ------------- 以下是给编辑器调用的方法，游戏中不能使用 --------------------------------

	// 初始化
	TBOOL InitForEditor(HWND hWnd,HINSTANCE hInst,TS32 lWidth,TS32 lHeight,const TCHAR* szResDir)
	{
		// 记录资源根目录
		GlobleClass::getSingletonPtr()->m_strResDir = TLunaEngine::String(szResDir);
		// 初始化日志
		TLunaEngine::String strLogConfig = GlobleClass::getSingletonPtr()->m_strResDir + "config\\logconfig.txt";
		TLunaEngine::String strLogDir = GlobleClass::getSingletonPtr()->m_strResDir + "log\\";
		if (!TLunaEngine::Log::InitLogSystem(strLogConfig.GetString(), strLogDir.GetString()))
			return TFALSE;
		// 建立D3D设备
		TLunaEngine::RenderDevice* device = TLunaEngine::RenderMgr::getSingletonPtr()->createRenderDevice(TLunaEngine::RenderMgr::DT_D3D11);
		if(!device)
			return TFALSE;
		if(!device->InitDevice(hWnd,TTRUE,lWidth,lHeight))
			return TFALSE;
		// 初始化资源操作管理
		ResMgr* pResMgr = ResMgr::getSingletonPtr();

		// ---------------- GUI部分 -------------------------
		// 字体管理
		GUIFontManager* pFontMgr = GUIFontManager::getSingletonPtr();
		TLunaEngine::String strFontEffect = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if(!pFontMgr->Init(strFontEffect.GetString(),lWidth,lHeight))
			return TFALSE;
		// UI渲染
		GUITextureMgr* pGUIRender = GUITextureMgr::getSingletonPtr();
		TLunaEngine::String strGUIRenderEffect = GlobleClass::getSingletonPtr()->m_strResDir + "shader\\gui\\guidefault.fx";
		if (!pGUIRender->Init(lWidth,lHeight,strGUIRenderEffect.GetString()))
		{
			return TFALSE;
		}
		// UI管理
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();

		// GlobleClass
		if (!GlobleClass::getSingletonPtr()->InitGlobleClass())
		{
			return TFALSE;
		}
		GlobleClass::getSingletonPtr()->m_bShowDebugInfo = TFALSE;
		GlobleClass::getSingletonPtr()->m_bEditor = TTRUE;
		return TTRUE;
	}

	// 循环
	TBOOL OnSceneUpdateForEditor(TF32 fTimeElapsed)
	{
		// -------- GUI部分 -------------------------
		GUIMgr* pGUIMgr = GUIMgr::getSingletonPtr();
		pGUIMgr->Update(fTimeElapsed);
		// GlobleClass
		GlobleClass::getSingletonPtr()->OnUpdate(fTimeElapsed);
		return TTRUE;
	}

	TBOOL OnSceneRenderForEditor(TF32 fTimeElapsed)
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
		return TTRUE;
	}

	// 销毁
	TBOOL DestroyForEditor()
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
		TLunaEngine::Log::DestroyLogSystem();
		return TTRUE;
	}

}