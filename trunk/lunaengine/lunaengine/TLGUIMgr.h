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
	 *	UI������
	 */
	class TLGUIMgr : public TLIGUIListener
	{
	T_SINGLETON_DEF(TLGUIMgr);
	public:
		TLGUIMgr(void);
		~TLGUIMgr(void);
	private:
		TLGUIContainer* m_pRootContainer;	// �����������治Ҫ�ſؼ�
		TLIGUIListener* m_pSceneListener;	// �ϲ��������ָ��
	public:
		// -------- ���� --------------
		inline TLGUIContainer* GetRootContainer()
		{
			return m_pRootContainer;
		}
		inline void SetRootContainer(TLGUIContainer* pContainer)
		{
			m_pRootContainer = pContainer;
		}
		// ����ID�ҵ�����
		inline TLGUIContainer* FindContainer(int iID)
		{
			if (m_pRootContainer)
			{
				return m_pRootContainer->FindContainer(iID);
			}
			return NULL;
		}
		// ����
		inline void DestroyRootContainer()
		{
			SAFE_DELETE(m_pRootContainer);
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
		inline void SetSceneListener(TLIGUIListener* pListener)
		{
			m_pSceneListener = pListener;
		}
	private:
		// ���ط���
		// ����Container
		bool LoadContainer(FILE* stream,TLGUIContainer* pParentContainer,TLGUIContainer** ppContainer);
		// ����Ctrl
		bool LoadCtrl(FILE* stream,TLGUIContainer* pParentContainer,TLGUICtrl** ppCtrl);
		// ���ض���
		bool LoadAnime(FILE* stream,TLGUIContainer* pNewContainer,BYTE yAnimeType);
	public:
		// ------------- ������дTLIGUIListener -------------------
		// ����������Ϸ���
		virtual void OnAnimePlayedOver(int iContainerID,BYTE yAnimeType)
		{
			if (m_pSceneListener)
			{
				m_pSceneListener->OnAnimePlayedOver(iContainerID,yAnimeType);
			}
		}
	};

}