#include "StdAfx.h"
#include "TLGUISceneMgr.h"
#include "TLGUITextureMgr.h"
#include "TLGUIMgr.h"
#include "TLGUIFontManager.h"
#include "KeyCodeDef.h"
using namespace TLunaEngine;

T_SINGLETON_IMP(TLGUISceneMgr);

TLGUISceneMgr::TLGUISceneMgr(void)
{
}

TLGUISceneMgr::~TLGUISceneMgr(void)
{
	DestroyGUISceneMgr();
}

bool TLGUISceneMgr::InitGUISceneMgr()
{
	return true;
}

void TLGUISceneMgr::DestroyGUISceneMgr()
{
	// UI管理
	TLGUIMgr::getSingletonPtr()->DestroyRootContainer();
	// UI渲染
	TLGUITextureMgr::getSingletonPtr()->DestroyAllTex();
	// 字体管理
	TLGUIFontManager::getSingletonPtr()->UseFont(-1);
	TLGUIFontManager::getSingletonPtr()->DestroyAllFont();
}

void TLGUISceneMgr::ShowContainer(int containerID, bool bShow)
{
	TLGUIMgr* pUIMgr = TLGUIMgr::getSingletonPtr();
	TLGUIContainer* pContainer = pUIMgr->FindContainer(containerID);
	if (pContainer)
	{
		pContainer->ShowContainer(bShow);
	}
}

void TLGUISceneMgr::UseFont(int iID)
{
	TLGUIFontManager::getSingletonPtr()->UseFont(iID);
}

bool TLGUISceneMgr::AddFont(const char* guiFontFile)
{
	if (!TLGUIFontManager::getSingletonPtr()->AddFontFromFile(guiFontFile))
	{
		return false;
	}
	return true;
}

bool TLGUISceneMgr::InitGUIRender(const char* guiRenderFile)
{
	if(!TLGUITextureMgr::getSingletonPtr()->LoadTexFromFile(guiRenderFile))
	{
		return false;
	}
	return true;
}

bool TLGUISceneMgr::InitGUI(const char* guiFile)
{
	if (!TLGUIMgr::getSingletonPtr()->LoadFromFile(guiFile))
	{
		return false;
	}
	return true;
}

void TLGUISceneMgr::CatchInputMsg(BYTE yType,void* param)
{
	if(yType == (BYTE)MSG_TYPE_KEY)
	{
		_MSG_KEY_INFO* keyInfo = (_MSG_KEY_INFO*)param;
		// 如果在起始UI中按下回车，就跳转到游戏UI
		if (keyInfo->btnInfo[KEY_RETURN].btnType == BUTTON_TYPE_UP)
		{
		}
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

void TLGUISceneMgr::OnAnimePlayedOver(int iContainerID,BYTE yAnimeType)
{
}