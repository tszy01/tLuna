#ifndef _TLGUIMGR_H_
#define _TLGUIMGR_H_

#include "TLSingleton.h"
#include <map>
#include "TLGUIContainer.h"
#include "TLGUIPicture.h"
#include "TLGUIText.h"
#include "TLGUIListener.h"

namespace TLunaEngine{

	/*
	 *	UI管理类
	 */
	class GUIMgr : public Singleton<GUIMgr>, public GUIListener
	{
		friend class Singleton<GUIMgr>;
	protected:
		GUIMgr(void);
		~GUIMgr(void);
	private:
		GUIContainer* m_pRootContainer;	// 跟容器，里面不要放控件
		GUIListener* m_pSceneListener;	// 上层接收器的指针
	public:
		// -------- 方法 --------------
		inline GUIContainer* GetRootContainer()
		{
			return m_pRootContainer;
		}
		inline void SetRootContainer(GUIContainer* pContainer)
		{
			m_pRootContainer = pContainer;
		}
		// 根据ID找到容器
		inline GUIContainer* FindContainer(int iID)
		{
			if (m_pRootContainer)
			{
				return m_pRootContainer->FindContainer(iID);
			}
			return TNULL;
		}
		// 销毁
		inline void DestroyRootContainer()
		{
			if (m_pRootContainer)
			{
				delete m_pRootContainer;
				m_pRootContainer = 0;
			}
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
		inline void SetSceneListener(GUIListener* pListener)
		{
			m_pSceneListener = pListener;
		}
	private:
		// 加载方法
		// 加载Container
		bool LoadContainer(FILE* stream,GUIContainer* pParentContainer,GUIContainer** ppContainer);
		// 加载Ctrl
		bool LoadCtrl(FILE* stream,GUIContainer* pParentContainer,GUICtrl** ppCtrl);
		// 加载动画
		bool LoadAnime(FILE* stream,GUIContainer* pNewContainer,TUByte yAnimeType);
	public:
		// ------------- 以下重写GUIListener -------------------
		// 动画播放完毕发送
		virtual void OnAnimePlayedOver(int iContainerID,TUByte yAnimeType)
		{
			if (m_pSceneListener)
			{
				m_pSceneListener->OnAnimePlayedOver(iContainerID,yAnimeType);
			}
		}
	};

}

#endif