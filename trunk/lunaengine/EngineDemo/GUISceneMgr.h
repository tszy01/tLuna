#ifndef _GUISCENEMGR_H_
#define _GUISCENEMGR_H_
#include "TLSingleton.h"
#include "TLGUIListener.h"

/*
 *	游戏部分，GUI场景管理器
 */
class GUISceneMgr : public TLunaEngine::Singleton<GUISceneMgr>, public TLunaEngine::GUIListener
{
	friend class  TLunaEngine::Singleton<GUISceneMgr>;
protected:
	GUISceneMgr(void);
	~GUISceneMgr(void);
private:
public:
	// ---------- 外部调用方法 -------------------
	// 初始化
	bool InitGUISceneMgr();
	// 销毁
	void DestroyGUISceneMgr();
	// 接受消息
	void CatchInputMsg(unsigned char yType, void* param);
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
	virtual void OnAnimePlayedOver(int iContainerID, TLunaEngine::TUByte yAnimeType);
};

#endif