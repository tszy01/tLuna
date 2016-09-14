#pragma once

#include "TSingleton.h"
#include <map>
#include "TLGUIContainer.h"
#include "TLGUIPicture.h"
#include "TLGUIText.h"
#include "TLIGUIListener.h"
using namespace std;
using namespace TLunaEngine;

namespace TLunaEngine{

	/*
	 *	UI管理类
	 */
	class TLGUIMgr : public TLIGUIListener
	{
	T_SINGLETON_DEF(TLGUIMgr);
	public:
		TLGUIMgr(void);
		~TLGUIMgr(void);
	private:
		TLGUIContainer* m_pRootContainer;	// 跟容器，里面不要放控件
		TLIGUIListener* m_pSceneListener;	// 上层接收器的指针
	public:
		// -------- 方法 --------------
		inline TLGUIContainer* GetRootContainer()
		{
			return m_pRootContainer;
		}
		inline void SetRootContainer(TLGUIContainer* pContainer)
		{
			m_pRootContainer = pContainer;
		}
		// 根据ID找到容器
		inline TLGUIContainer* FindContainer(int iID)
		{
			if (m_pRootContainer)
			{
				return m_pRootContainer->FindContainer(iID);
			}
			return NULL;
		}
		// 销毁
		inline void DestroyRootContainer()
		{
			SAFE_DELETE(m_pRootContainer);
		}
		// 帧更新
		inline void Update(float fTimeElapsed)
		{
			if (m_pRootContainer)
			{
				m_pRootContainer->Update(fTimeElapsed);
			}
		}
		// 帧渲染
		inline void Render(float fTimeElapsed)
		{
			if (m_pRootContainer)
			{
				m_pRootContainer->Render(fTimeElapsed);
			}
		}
		// 从文件加载
		bool LoadFromFile(const char* file);
		// 设置上层接收器
		inline void SetSceneListener(TLIGUIListener* pListener)
		{
			m_pSceneListener = pListener;
		}
	private:
		// 加载方法
		// 加载Container
		bool LoadContainer(FILE* stream,TLGUIContainer* pParentContainer,TLGUIContainer** ppContainer);
		// 加载Ctrl
		bool LoadCtrl(FILE* stream,TLGUIContainer* pParentContainer,TLGUICtrl** ppCtrl);
		// 加载动画
		bool LoadAnime(FILE* stream,TLGUIContainer* pNewContainer,BYTE yAnimeType);
	public:
		// ------------- 以下重写TLIGUIListener -------------------
		// 动画播放完毕发送
		virtual void OnAnimePlayedOver(int iContainerID,BYTE yAnimeType)
		{
			if (m_pSceneListener)
			{
				m_pSceneListener->OnAnimePlayedOver(iContainerID,yAnimeType);
			}
		}
	};

}