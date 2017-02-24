#ifndef _TLGUIMGR_H_
#define _TLGUIMGR_H_

#include "TSSingleton.h"
#include <map>
#include "TLGUIContainer.h"
#include "TLGUIPicture.h"
#include "TLGUIText.h"
#include "TLGUIListener.h"

namespace TLunaEngine{

	/*
	 *	UI管理类
	 */
	class GUIMgr : public TSun::Singleton<GUIMgr>, public GUIListener
	{
		friend class TSun::Singleton<GUIMgr>;
	protected:
		GUIMgr(TSun::TVOID);
		~GUIMgr(TSun::TVOID);
	private:
		GUIContainer* m_pRootContainer;	// 跟容器，里面不要放控件
		GUIListener* m_pSceneListener;	// 上层接收器的指针
	public:
		// -------- 方法 --------------
		inline GUIContainer* GetRootContainer()
		{
			return m_pRootContainer;
		}
		inline TSun::TVOID SetRootContainer(GUIContainer* pContainer)
		{
			m_pRootContainer = pContainer;
		}
		// 根据ID找到容器
		inline GUIContainer* FindContainer(TSun::TS32 iID)
		{
			if (m_pRootContainer)
			{
				return m_pRootContainer->FindContainer(iID);
			}
			return TSun::TNULL;
		}
		// 销毁
		inline TSun::TVOID DestroyRootContainer()
		{
			if (m_pRootContainer)
			{
				delete m_pRootContainer;
				m_pRootContainer = 0;
			}
		}
		// 帧更新
		inline TSun::TVOID Update(TSun::TF32 fTimeElapsed)
		{
			if (m_pRootContainer)
			{
				m_pRootContainer->Update(fTimeElapsed);
			}
		}
		// 帧渲染
		inline TSun::TVOID Render(TSun::TF32 fTimeElapsed)
		{
			if (m_pRootContainer)
			{
				m_pRootContainer->Render(fTimeElapsed);
			}
		}
		// 从文件加载
		TSun::TBOOL LoadFromFile(const TSun::TCHAR* file);
		// 设置上层接收器
		inline TSun::TVOID SetSceneListener(GUIListener* pListener)
		{
			m_pSceneListener = pListener;
		}
	private:
		// 加载方法
		// 加载Container
		TSun::TBOOL LoadContainer(FILE* stream,GUIContainer* pParentContainer,GUIContainer** ppContainer);
		// 加载Ctrl
		TSun::TBOOL LoadCtrl(FILE* stream,GUIContainer* pParentContainer,GUICtrl** ppCtrl);
		// 加载动画
		TSun::TBOOL LoadAnime(FILE* stream,GUIContainer* pNewContainer,TSun::TUByte yAnimeType);
	public:
		// ------------- 以下重写GUIListener -------------------
		// 动画播放完毕发送
		virtual TSun::TVOID OnAnimePlayedOver(TSun::TS32 iContainerID,TSun::TUByte yAnimeType)
		{
			if (m_pSceneListener)
			{
				m_pSceneListener->OnAnimePlayedOver(iContainerID,yAnimeType);
			}
		}
	};

}

#endif