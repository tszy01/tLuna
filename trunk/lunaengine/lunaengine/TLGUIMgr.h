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
		GUIMgr(TVOID);
		~GUIMgr(TVOID);
	private:
		GUIContainer* m_pRootContainer;	// 跟容器，里面不要放控件
		GUIListener* m_pSceneListener;	// 上层接收器的指针
	public:
		// -------- 方法 --------------
		inline GUIContainer* GetRootContainer()
		{
			return m_pRootContainer;
		}
		inline TVOID SetRootContainer(GUIContainer* pContainer)
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
		inline TVOID DestroyRootContainer()
		{
			if (m_pRootContainer)
			{
				delete m_pRootContainer;
				m_pRootContainer = 0;
			}
		}
		// 帧更新
		inline TVOID Update(float fTimeElapsed)
		{
			if (m_pRootContainer)
			{
				m_pRootContainer->Update(fTimeElapsed);
			}
		}
		// 帧渲染
		inline TVOID Render(float fTimeElapsed)
		{
			if (m_pRootContainer)
			{
				m_pRootContainer->Render(fTimeElapsed);
			}
		}
		// 从文件加载
		TBOOL LoadFromFile(const TCHAR* file);
		// 设置上层接收器
		inline TVOID SetSceneListener(GUIListener* pListener)
		{
			m_pSceneListener = pListener;
		}
	private:
		// 加载方法
		// 加载Container
		TBOOL LoadContainer(FILE* stream,GUIContainer* pParentContainer,GUIContainer** ppContainer);
		// 加载Ctrl
		TBOOL LoadCtrl(FILE* stream,GUIContainer* pParentContainer,GUICtrl** ppCtrl);
		// 加载动画
		TBOOL LoadAnime(FILE* stream,GUIContainer* pNewContainer,TUByte yAnimeType);
	public:
		// ------------- 以下重写GUIListener -------------------
		// 动画播放完毕发送
		virtual TVOID OnAnimePlayedOver(int iContainerID,TUByte yAnimeType)
		{
			if (m_pSceneListener)
			{
				m_pSceneListener->OnAnimePlayedOver(iContainerID,yAnimeType);
			}
		}
	};

}

#endif