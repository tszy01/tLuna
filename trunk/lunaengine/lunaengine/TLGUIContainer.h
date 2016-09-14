#pragma once

#include "TLGUIDefine.h"
#include <map>
#include "TLGUICtrl.h"
#include <list>
using namespace std;
namespace TLunaEngine{

	class TLGUICtrl;
	class TLGUIContainer;
	class TLIGUIListener;

	/*
	 *	�����࣬���Ա��̳У������������������������ؼ�
	 */
	class TLGUIContainer
	{
	public:
		TLGUIContainer(void);
		~TLGUIContainer(void);
	private:
		std::map<int,TLGUIContainer*> m_SubContainerTable;		// ����������
		std::vector<TLGUICtrl*> m_CtrlList;								// �ؼ�����
		int m_iID;													// ����ID
		TLGUIContainer* m_pParent;									// ������
		TLIGUIListener* m_pListener;								// ��Ϣ�ϲ㲶��
		bool m_bShow;												// �Ƿ���ʾ

		// ------ ����������ڸ�������λ�úʹ�С --------
		// ���������û�У��Ǿ���������������ڵ�
		LONG m_posX;
		LONG m_posY;
		LONG m_width;
		LONG m_height;

		// ------ �����Ǽ�����ľ���λ�úʹ�С ----------
		LONG m_posXParent;
		LONG m_posYParent;
		LONG m_posXFinal;
		LONG m_posYFinal;
		bool m_bReCal;		// �Ƿ����¼���

		// ----- �����Ƕ�����չ���� ---------------------
		// ----- ����AnimeTypeʣ�µĶ��������˺������� --
		// ��������
		// ���Ǹ�ö�ٲ��������ܺ��ж��ֶ���
		BYTE m_yAnimeType;
		// �������Ŵ���
		// -1����ѭ����0ͣ�ڵ�һ֡���������ͣ�����һ֡
		// CATE_COUNT 0Ϊ���뵭����1ΪͼƬ������2Ϊλ�ö���
		int m_nAnimeTimes[CATE_COUNT];
		// ��ǰ�����ǵڼ��β��ţ���0��ʼ������һ��+1
		int m_iNowAnimeTimes[CATE_COUNT];
		// ÿ��任�ĸ�����ֵ
		// ÿ��ALPHA�仯ֵ -- ���뵭��Ч��
		// ÿ��ͼƬ�任���� -- ͼƬ֡����
		// ÿ��λ�øı伸������ -- λ��֡����
		float m_nAnimeChangedPerSec[CATE_COUNT];
		// ÿ������仯
		// ��������������Ͳ�ͬ�����õķ�ʽҲ��ͬ��Ҫ����������С��λ+1ʱ������
		float m_nAnimeChangedTimeCount[CATE_COUNT];
		// �����Ƿ񲥷����
		bool m_bAnimePlayedOver[CATE_COUNT];

		// ------ �����ǵ��뵭����� --------------------
		// ���뵭������
		CONTAINER_FADE_TYPE m_eAnimeFadeType;
		// ��ǰ��������״̬
		// -1δ�ڶ����У�0���룬1����
		int m_iNowFadeType;
		// ��ǰ��ALPHAֵ
		float m_fNowAlpha;
		// ----- ͼƬ֡������� -------------------------
		// ��ʼ�ؼ�����
		int m_iPicChangeStartIndex;
		// �����ؼ��������Ϳ�ʼ֮��Ҫ��������
		int m_iPicChangeEndIndex;
		// ���ڲ��ŵĿؼ�����
		// -1δ�ڶ�����
		int m_iNowPicChangeIndex;
		// ---- λ��֡���� ------------------------------
		// ��ʼλ��
		LONG m_posChangeStartX;
		LONG m_posChangeStartY;
		// ����λ��
		LONG m_posChangeEndX;
		LONG m_posChangeEndY;
	public:
		// ------ �����Ƿ��� ----------------------------
		// ��ʼ��
		bool InitContainer(int ID,TLGUIContainer* pParent,LONG x,LONG y,LONG width,LONG height,BYTE yAnimeType,TLIGUIListener* pListener);
		// ��ʼ������
		bool InitFadeAnime(CONTAINER_FADE_TYPE eFadeType,float fChangedPerSec,int nTimes);
		bool InitPicChangeAnime(int iStartIndex,int iEndIndex,float fChangedPerSec,int nTimes);
		bool InitPosChangeAnime(LONG startX,LONG startY,LONG endX,LONG endY,float fChangedPerSec,int nTimes);
		// ���ö���
		void ResetFadeAnime();
		void ResetPicChangeAnime();
		void ResetPosChangeAnime();
		// ����֡���㶯��
		void UpdateFadeAnime(float fTimeElapsed);
		void UpdatePicChangeAnime(float fTimeElapsed);
		void UpdatePosChangeAnime(float fTimeElapsed);
		// ���¼����ӿؼ�����λ��
		void ReCalSubRect(LONG parentFinalX,LONG parentFinalY);
		// ��ӿؼ�
		bool AddCtrl(TLGUICtrl* pCtrl);
		// �������
		bool AddContainer(TLGUIContainer* pContainer);
		// ɾ������
		void RemoveSubContainer(int ID);
		// �õ�ID
		inline int GetID()
		{
			return m_iID;
		}
		// �õ��ؼ�
		inline TLGUICtrl* GetCtrl(int iIndex)
		{
			TLGUICtrl* pCtrl = NULL;
			if(iIndex<0 || iIndex>=(int)m_CtrlList.size())
				return pCtrl;
			pCtrl = m_CtrlList[iIndex];
			return pCtrl;
		}
		// �õ�����
		inline TLGUIContainer* GetSubContainer(int iID)
		{
			TLGUIContainer* pContainer = NULL;
			std::map<int,TLunaEngine::TLGUIContainer*>::iterator itr = m_SubContainerTable.find(iID);
			if(itr!=m_SubContainerTable.end())
				return itr->second;
			return pContainer;
		}
		// �ݹ�������
		inline TLGUIContainer* FindContainer(int iID)
		{
			// �ȱȽ��Լ�
			if (m_iID == iID)
			{
				return this;
			}
			// �����������ķ���
			std::map<int,TLGUIContainer*>::iterator itr = m_SubContainerTable.begin();
			for (;itr!=m_SubContainerTable.end();itr++)
			{
				TLGUIContainer* pSubContainer = itr->second;
				TLGUIContainer* pFind = pSubContainer->FindContainer(iID);
				if (pFind)
				{
					return pFind;
				}
			}
			return NULL;
		}
		// ɾ���ؼ�
		inline void ClearCtrls()
		{
			std::vector<TLGUICtrl*>::iterator itr = m_CtrlList.begin();
			for (;itr!=m_CtrlList.end();itr++)
			{
				TLGUICtrl* pCtrl = (*itr);
				SAFE_DELETE(pCtrl);
			}
			m_CtrlList.clear();
		}
		// ɾ��������
		inline void ClearSubContainers()
		{
			std::map<int,TLGUIContainer*>::iterator itr = m_SubContainerTable.begin();
			for (;itr!=m_SubContainerTable.end();itr++)
			{
				TLGUIContainer* pSubContainer = itr->second;
				SAFE_DELETE(pSubContainer);
			}
			m_SubContainerTable.clear();
		}
		// ���������¼������λ��
		inline void MakeReCal(bool bReCal = true)
		{
			m_bReCal = bReCal;
			std::map<int,TLGUIContainer*>::iterator itr = m_SubContainerTable.begin();
			for (;itr!=m_SubContainerTable.end();itr++)
			{
				TLGUIContainer* pSubContainer = itr->second;
				pSubContainer->MakeReCal(bReCal);
			}
		}
		// �õ��Ƿ���ʾ
		inline bool IsShow()
		{
			return m_bShow;
		}
		// �����Ƿ���ʾ
		void ShowContainer(bool bShow = true);
		// ����λ��
		// ֻ������λ�ã��������ô�С
		void SetPosition(LONG x,LONG y)
		{
			m_posX = x;
			m_posY = y;
			MakeReCal(true);
		}
		// ���ÿؼ�ALPHA
		void SetCtrlAlpha(float fAlpha);
		// ���ö�������
		inline void SetAnimeType(BYTE yType)
		{
			m_yAnimeType = yType;
		}
		// �õ���������
		inline BYTE GetAnimeType()
		{
			return m_yAnimeType;
		}
		// ��������ʱ���õĺ���
		void OnAnimePlayedOver();
	public:
		// ����
		void DestroyContainer();
		// ����
		bool Update(float fTimeElapsed);
		// ��Ⱦ
		bool Render(float fTimeElapsed);
	};

}