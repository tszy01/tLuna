#ifndef _TLGUICONTAINER_H_
#define _TLGUICONTAINER_H_

#include "TLGUIDefine.h"
#include <map>
#include "TLGUICtrl.h"
#include <list>
#include <vector>
namespace TLunaEngine{

	class GUICtrl;
	class GUIContainer;
	class GUIListener;

	/*
	 *	�����࣬���Ա��̳У������������������������ؼ�
	 */
	class GUIContainer
	{
	public:
		GUIContainer(void);
		~GUIContainer(void);
	private:
		std::map<int,GUIContainer*> m_SubContainerTable;		// ����������
		std::vector<GUICtrl*> m_CtrlList;								// �ؼ�����
		int m_iID;													// ����ID
		GUIContainer* m_pParent;									// ������
		GUIListener* m_pListener;								// ��Ϣ�ϲ㲶��
		bool m_bShow;												// �Ƿ���ʾ

		// ------ ����������ڸ�������λ�úʹ�С --------
		// ���������û�У��Ǿ���������������ڵ�
		TS32 m_posX;
		TS32 m_posY;
		TS32 m_width;
		TS32 m_height;

		// ------ �����Ǽ�����ľ���λ�úʹ�С ----------
		TS32 m_posXParent;
		TS32 m_posYParent;
		TS32 m_posXFinal;
		TS32 m_posYFinal;
		bool m_bReCal;		// �Ƿ����¼���

		// ----- �����Ƕ�����չ���� ---------------------
		// ----- ����AnimeTypeʣ�µĶ��������˺������� --
		// ��������
		// ���Ǹ�ö�ٲ��������ܺ��ж��ֶ���
		TUByte m_yAnimeType;
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
		TS32 m_posChangeStartX;
		TS32 m_posChangeStartY;
		// ����λ��
		TS32 m_posChangeEndX;
		TS32 m_posChangeEndY;
	public:
		// ------ �����Ƿ��� ----------------------------
		// ��ʼ��
		bool InitContainer(int ID,GUIContainer* pParent,TS32 x,TS32 y,TS32 width,TS32 height,TUByte yAnimeType,GUIListener* pListener);
		// ��ʼ������
		bool InitFadeAnime(CONTAINER_FADE_TYPE eFadeType,float fChangedPerSec,int nTimes);
		bool InitPicChangeAnime(int iStartIndex,int iEndIndex,float fChangedPerSec,int nTimes);
		bool InitPosChangeAnime(TS32 startX,TS32 startY,TS32 endX,TS32 endY,float fChangedPerSec,int nTimes);
		// ���ö���
		void ResetFadeAnime();
		void ResetPicChangeAnime();
		void ResetPosChangeAnime();
		// ����֡���㶯��
		void UpdateFadeAnime(float fTimeElapsed);
		void UpdatePicChangeAnime(float fTimeElapsed);
		void UpdatePosChangeAnime(float fTimeElapsed);
		// ���¼����ӿؼ�����λ��
		void ReCalSubRect(TS32 parentFinalX,TS32 parentFinalY);
		// ��ӿؼ�
		bool AddCtrl(GUICtrl* pCtrl);
		// �������
		bool AddContainer(GUIContainer* pContainer);
		// ɾ������
		void RemoveSubContainer(int ID);
		// �õ�ID
		inline int GetID()
		{
			return m_iID;
		}
		// �õ��ؼ�
		inline GUICtrl* GetCtrl(int iIndex)
		{
			GUICtrl* pCtrl = TNULL;
			if(iIndex<0 || iIndex>=(int)m_CtrlList.size())
				return pCtrl;
			pCtrl = m_CtrlList[iIndex];
			return pCtrl;
		}
		// �õ�����
		inline GUIContainer* GetSubContainer(int iID)
		{
			GUIContainer* pContainer = TNULL;
			std::map<int,TLunaEngine::GUIContainer*>::iterator itr = m_SubContainerTable.find(iID);
			if(itr!=m_SubContainerTable.end())
				return itr->second;
			return pContainer;
		}
		// �ݹ�������
		inline GUIContainer* FindContainer(int iID)
		{
			// �ȱȽ��Լ�
			if (m_iID == iID)
			{
				return this;
			}
			// �����������ķ���
			std::map<int,GUIContainer*>::iterator itr = m_SubContainerTable.begin();
			for (;itr!=m_SubContainerTable.end();itr++)
			{
				GUIContainer* pSubContainer = itr->second;
				GUIContainer* pFind = pSubContainer->FindContainer(iID);
				if (pFind)
				{
					return pFind;
				}
			}
			return TNULL;
		}
		// ɾ���ؼ�
		inline void ClearCtrls()
		{
			std::vector<GUICtrl*>::iterator itr = m_CtrlList.begin();
			for (;itr!=m_CtrlList.end();itr++)
			{
				GUICtrl* pCtrl = (*itr);
				if (pCtrl)
				{
					delete pCtrl;
					pCtrl = 0;
				}
			}
			m_CtrlList.clear();
		}
		// ɾ��������
		inline void ClearSubContainers()
		{
			std::map<int,GUIContainer*>::iterator itr = m_SubContainerTable.begin();
			for (;itr!=m_SubContainerTable.end();itr++)
			{
				GUIContainer* pSubContainer = itr->second;
				if (pSubContainer)
				{
					delete pSubContainer;
					pSubContainer = 0;
				}
			}
			m_SubContainerTable.clear();
		}
		// ���������¼������λ��
		inline void MakeReCal(bool bReCal = true)
		{
			m_bReCal = bReCal;
			std::map<int,GUIContainer*>::iterator itr = m_SubContainerTable.begin();
			for (;itr!=m_SubContainerTable.end();itr++)
			{
				GUIContainer* pSubContainer = itr->second;
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
		void SetPosition(TS32 x,TS32 y)
		{
			m_posX = x;
			m_posY = y;
			MakeReCal(true);
		}
		// ���ÿؼ�ALPHA
		void SetCtrlAlpha(float fAlpha);
		// ���ö�������
		inline void SetAnimeType(TUByte yType)
		{
			m_yAnimeType = yType;
		}
		// �õ���������
		inline TUByte GetAnimeType()
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

#endif