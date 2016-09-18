#include "GUISceneMgr.h"
#include "TLGUITextureMgr.h"
#include "TLGUIMgr.h"
#include "TLGUIFontManager.h"
#include "KeyCodeDef.h"

GUISceneMgr* TLunaEngine::Singleton<GUISceneMgr>::m_Ptr = 0;

GUISceneMgr::GUISceneMgr(void)
{
}

GUISceneMgr::~GUISceneMgr(void)
{
	DestroyGUISceneMgr();
}

bool GUISceneMgr::InitGUISceneMgr()
{
	return true;
}

void GUISceneMgr::DestroyGUISceneMgr()
{
	// UI管理
	TLunaEngine::GUIMgr::getSingletonPtr()->DestroyRootContainer();
	// UI渲染
	TLunaEngine::GUITextureMgr::getSingletonPtr()->DestroyAllTex();
	// 字体管理
	TLunaEngine::GUIFontManager::getSingletonPtr()->UseFont(-1);
	TLunaEngine::GUIFontManager::getSingletonPtr()->DestroyAllFont();
}

void GUISceneMgr::ShowContainer(int containerID, bool bShow)
{
	TLunaEngine::GUIMgr* pUIMgr = TLunaEngine::GUIMgr::getSingletonPtr();
	TLunaEngine::GUIContainer* pContainer = pUIMgr->FindContainer(containerID);
	if (pContainer)
	{
		pContainer->ShowContainer(bShow);
	}
}

void GUISceneMgr::UseFont(int iID)
{
	TLunaEngine::GUIFontManager::getSingletonPtr()->UseFont(iID);
}

bool GUISceneMgr::AddFont(const char* guiFontFile)
{
	if (!TLunaEngine::GUIFontManager::getSingletonPtr()->AddFontFromFile(guiFontFile))
	{
		return false;
	}
	return true;
}

bool GUISceneMgr::InitGUIRender(const char* guiRenderFile)
{
	if (!TLunaEngine::GUITextureMgr::getSingletonPtr()->LoadTexFromFile(guiRenderFile))
	{
		return false;
	}
	return true;
}

bool GUISceneMgr::InitGUI(const char* guiFile)
{
	if (!TLunaEngine::GUIMgr::getSingletonPtr()->LoadFromFile(guiFile))
	{
		return false;
	}
	return true;
}

void GUISceneMgr::CatchInputMsg(unsigned char yType, void* param)
{
	if (yType == (TLunaEngine::TUByte)MSG_TYPE_KEY)
	{
		_MSG_KEY_INFO* keyInfo = (_MSG_KEY_INFO*)param;
		// 如果在起始UI中按下回车，就跳转到游戏UI
		if (keyInfo->btnInfo[KEY_RETURN].btnType == BUTTON_TYPE_UP)
		{
		}
	}
	else if (yType == (TLunaEngine::TUByte)MSG_TYPE_MOUSE)
	{
		_MSG_MOUSE_INFO* mouseInfo = (_MSG_MOUSE_INFO*)param;
	}
	else if (yType == (TLunaEngine::TUByte)MSG_TYPE_JOYS)
	{
		_MSG_JOYS_INFO* joysInfo = (_MSG_JOYS_INFO*)param;
	}
}

void GUISceneMgr::OnAnimePlayedOver(int iContainerID, TLunaEngine::TUByte yAnimeType)
{
}