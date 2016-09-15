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
	 *	UI������
	 */
	class GUIMgr : public Singleton<GUIMgr>, public GUIListener
	{
		friend class Singleton<GUIMgr>;
	protected:
		GUIMgr(void);
		~GUIMgr(void);
	private:
		GUIContainer* m_pRootContainer;	// �����������治Ҫ�ſؼ�
		GUIListener* m_pSceneListener;	// �ϲ��������ָ��
	public:
		// -------- ���� --------------
		inline GUIContainer* GetRootContainer()
		{
			return m_pRootContainer;
		}
		inline void SetRootContainer(GUIContainer* pContainer)
		{
			m_pRootContainer = pContainer;
		}
		// ����ID�ҵ�����
		inline GUIContainer* FindContainer(int iID)
		{
			if (m_pRootContainer)
			{
				return m_pRootContainer->FindContainer(iID);
			}
			return TNULL;
		}
		// ����
		inline void DestroyRootContainer()
		{
			if (m_pRootContainer)
			{
				delete m_pRootContainer;
				m_pRootContainer = 0;
			}
		}
		// ֡����
		inline void Update(float fTimeElapsed)
		{
			if (m_pRootContainer)
			{
				m_pRootContainer->Update(fTimeElapsed);
			}
		}
		// ֡��Ⱦ
		inline void Render(float fTimeElapsed)
		{
			if (m_pRootContainer)
			{
				m_pRootContainer->Render(fTimeElapsed);
			}
		}
		// ���ļ�����
		bool LoadFromFile(const char* file);
		// �����ϲ������
		inline void SetSceneListener(GUIListener* pListener)
		{
			m_pSceneListener = pListener;
		}
	private:
		// ���ط���
		// ����Container
		bool LoadContainer(FILE* stream,GUIContainer* pParentContainer,GUIContainer** ppContainer);
		// ����Ctrl
		bool LoadCtrl(FILE* stream,GUIContainer* pParentContainer,GUICtrl** ppCtrl);
		// ���ض���
		bool LoadAnime(FILE* stream,GUIContainer* pNewContainer,TUByte yAnimeType);
	public:
		// ------------- ������дGUIListener -------------------
		// ����������Ϸ���
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