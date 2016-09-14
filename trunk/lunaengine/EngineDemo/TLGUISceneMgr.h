#pragma once
#include "TSingleton.h"
#include "TLIGUIListener.h"

/*
 *	游戏部分，GUI场景管理器
 */
class TLGUISceneMgr : public TLunaEngine::TLIGUIListener
{
T_SINGLETON_DEF(TLGUISceneMgr);
public:
	TLGUISceneMgr(void);
	~TLGUISceneMgr(void);
private:
public:
	// ---------- 外部调用方法 -------------------
	// 初始化
	bool InitGUISceneMgr();
	// 销毁
	void DestroyGUISceneMgr();
	// 接受消息
	void CatchInputMsg(BYTE yType,void* param);
	// 设置容器可见
	void ShowContainer(int containerID,bool bShow = true);
	// 设置使用哪种字体
	void UseFont(int iID);
	// 加载字体
	bool AddFont(const char* guiFontFile);
	// 加载GUI渲染
	bool InitGUIRender(const char* guiRenderFile);
	// 加载GUI控件
	bool InitGUI(const char* guiFile);
public:
	// --------- 以下重写TLIGUIListener -------------------
	// 动画播放完毕发送
	virtual void OnAnimePlayedOver(int iContainerID,BYTE yAnimeType);
};