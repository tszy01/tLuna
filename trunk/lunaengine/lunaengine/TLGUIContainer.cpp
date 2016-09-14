#include "StdAfx.h"
#include "TLGUIContainer.h"
#include "TLIGUIListener.h"

namespace TLunaEngine{
	TLGUIContainer::TLGUIContainer(void) : 
	m_iID(-1),
	m_posX(0),
	m_posY(0),
	m_width(0),
	m_height(0),
	m_pParent(NULL),
	m_posXFinal(0),
	m_posYFinal(0),
	m_posXParent(0),
	m_posYParent(0),
	m_bReCal(false),
	m_bShow(true),
	m_yAnimeType(0),
	m_pListener(NULL)
	{
	}

	TLGUIContainer::~TLGUIContainer(void)
	{
		DestroyContainer();
	}

	bool TLGUIContainer::InitContainer(int ID, TLunaEngine::TLGUIContainer *pParent, LONG x, LONG y, LONG width, LONG height,BYTE yAnimeType,TLIGUIListener* pListener)
	{
		m_iID = ID;
		m_pParent = pParent;
		m_pListener = pListener;
		m_posX = x;
		m_posY = y;
		m_width = width;
		m_height = height;
		m_posXFinal = x;
		m_posYFinal = y;
		m_posXParent = 0;
		m_posYParent = 0;
		m_yAnimeType = yAnimeType;
		ResetFadeAnime();
		ResetPicChangeAnime();
		ResetPosChangeAnime();
		// ʹ���¼���λ��
		m_bReCal = true;
		return true;
	}

	bool TLGUIContainer::InitFadeAnime(CONTAINER_FADE_TYPE eFadeType,float fChangedPerSec,int nTimes)
	{
		if (nTimes==0 || fChangedPerSec<=0 || eFadeType == CFF_NONE)
		{
			return false;
		}
		m_eAnimeFadeType = eFadeType;
		m_iNowFadeType = 0;
		m_fNowAlpha = 0;
		if (eFadeType == CFF_FADE_OUT)
		{
			m_fNowAlpha = 1;
			m_iNowFadeType = 1;
		}
		SetCtrlAlpha(m_fNowAlpha);
		m_nAnimeTimes[CATE_FADE] = nTimes;
		m_iNowAnimeTimes[CATE_FADE] = 0;
		m_nAnimeChangedPerSec[CATE_FADE] = fChangedPerSec;
		m_nAnimeChangedTimeCount[CATE_FADE] = 0;
		m_bAnimePlayedOver[CATE_FADE] = false;
		return true;
	}

	void TLGUIContainer::ResetFadeAnime()
	{
		m_eAnimeFadeType = CFF_NONE;
		m_iNowFadeType = -1;
		m_fNowAlpha = 0;
		m_nAnimeTimes[CATE_FADE] = 0;
		m_iNowAnimeTimes[CATE_FADE] = 0;
		m_nAnimeChangedPerSec[CATE_FADE] = 0;
		m_nAnimeChangedTimeCount[CATE_FADE] = 0;
		m_bAnimePlayedOver[CATE_FADE] = false;
	}

	void TLGUIContainer::UpdateFadeAnime(float fTimeElapsed)
	{
		if (m_iNowAnimeTimes[CATE_FADE] < m_nAnimeTimes[CATE_FADE] || m_nAnimeTimes[CATE_FADE] == -1)
		{
			// ���㵱ǰ֡��Ҫ���Ӷ���ֵ
			float fNeedPlusAlpha = m_nAnimeChangedPerSec[CATE_FADE] * fTimeElapsed;
			// �����ǰ���ŵĴ���С��һ��Ҫ���ģ�˵����Ҫ������
			switch (m_eAnimeFadeType)
			{
			case CFF_FADE_IN:
				{
					// ֡ʱ�����
					m_fNowAlpha += fNeedPlusAlpha;
					if (m_fNowAlpha >= 1.0f)
					{
						// ���ALPHA�ӵ�1�ˣ����ζ�������
						m_fNowAlpha = 1.0f;
						m_iNowAnimeTimes[CATE_FADE] += 1;
						if (m_iNowAnimeTimes[CATE_FADE] >= m_nAnimeTimes[CATE_FADE] && m_nAnimeTimes[CATE_FADE] >= 0)
						{
							// �������ָ��������
							m_bAnimePlayedOver[CATE_FADE] = true;
							m_iNowFadeType = -1;
							OnAnimePlayedOver();
						}
						else
						{
							// ׼����һ��ѭ��
							m_fNowAlpha = 0;
						}
					}
					// ����Alpha
					SetCtrlAlpha(m_fNowAlpha);
				}
				break;
			case CFF_FADE_OUT:
				{
					// ֡ʱ�����
					m_fNowAlpha -= fNeedPlusAlpha;
					if (m_fNowAlpha <= 0.0f)
					{
						// ���ALPHA�ӵ�1�ˣ����ζ�������
						m_fNowAlpha = 0.0f;
						m_iNowAnimeTimes[CATE_FADE] += 1;
						if (m_iNowAnimeTimes[CATE_FADE] >= m_nAnimeTimes[CATE_FADE] && m_nAnimeTimes[CATE_FADE] >= 0)
						{
							// �������ָ��������
							m_bAnimePlayedOver[CATE_FADE] = true;
							m_iNowFadeType = -1;
							OnAnimePlayedOver();
						}
						else
						{
							// ��һ��ѭ��
							m_fNowAlpha = 1.0f;
						}
					}
					// ����Alpha
					SetCtrlAlpha(m_fNowAlpha);
				}
				break;
			case CFF_FADE_IN_OUT:
				{
					// ֡ʱ�����
					if (m_iNowFadeType==0)
					{
						// ��������
						m_fNowAlpha += fNeedPlusAlpha;
						if (m_fNowAlpha >= 1.0f)
						{
							// ���ALPHA�ӵ�1�ˣ��͸ļ�����
							m_fNowAlpha = 1.0f;
							m_iNowFadeType = 1;
						}
					}
					else if(m_iNowFadeType==1)
					{
						// �ݼ�����
						m_fNowAlpha -= fNeedPlusAlpha;
						if (m_fNowAlpha <= 0.0f)
						{
							// ���ALPHA�ӵ�1�ˣ����ζ�������
							m_fNowAlpha = 0.0f;
							m_iNowAnimeTimes[CATE_FADE] += 1;
							if (m_iNowAnimeTimes[CATE_FADE] >= m_nAnimeTimes[CATE_FADE] && m_nAnimeTimes[CATE_FADE] >= 0)
							{
								// �������ָ��������
								m_bAnimePlayedOver[CATE_FADE] = true;
								m_iNowFadeType = -1;
								OnAnimePlayedOver();
							}
							else
							{
								// ��һ��ѭ��
								m_fNowAlpha = 0;
								m_iNowFadeType = 0;
							}
						}
					}
					// ����Alpha
					SetCtrlAlpha(m_fNowAlpha);
				}
				break;
			}
		}
	}

	bool TLGUIContainer::InitPicChangeAnime(int iStartIndex,int iEndIndex,float fChangedPerSec,int nTimes)
	{
		if (nTimes==0 || fChangedPerSec<=0 || iStartIndex == -1 || iEndIndex == -1)
		{
			return false;
		}
		m_iPicChangeStartIndex = iStartIndex;
		m_iPicChangeEndIndex = iEndIndex;
		m_iNowPicChangeIndex = iStartIndex;
		ShowContainer(false);
		m_CtrlList[iStartIndex]->ShowCtrl(true);
		m_nAnimeTimes[CATE_PIC_CHANGE] = nTimes;
		m_iNowAnimeTimes[CATE_PIC_CHANGE] = 0;
		m_nAnimeChangedPerSec[CATE_PIC_CHANGE] = fChangedPerSec;
		m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] = 0;
		m_bAnimePlayedOver[CATE_PIC_CHANGE] = false;
		return true;
	}

	void TLGUIContainer::ResetPicChangeAnime()
	{
		m_iPicChangeStartIndex = -1;
		m_iPicChangeEndIndex = -1;
		m_iNowPicChangeIndex = -1;
		m_nAnimeTimes[CATE_PIC_CHANGE] = 0;
		m_iNowAnimeTimes[CATE_PIC_CHANGE] = 0;
		m_nAnimeChangedPerSec[CATE_PIC_CHANGE] = 0;
		m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] = 0;
		m_bAnimePlayedOver[CATE_PIC_CHANGE] = false;
	}

	void TLGUIContainer::UpdatePicChangeAnime(float fTimeElapsed)
	{
		if (m_iNowAnimeTimes[CATE_PIC_CHANGE] < m_nAnimeTimes[CATE_PIC_CHANGE] || m_nAnimeTimes[CATE_PIC_CHANGE] == -1)
		{
			// �����ǰ���ŵĴ���С��һ��Ҫ���ģ�˵����Ҫ������
			// ֡ʱ�����
			m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] += fTimeElapsed;
			if (m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] >= 1.0f / m_nAnimeChangedPerSec[CATE_PIC_CHANGE])
			{
				// �������һ��ͼƬ����
				m_CtrlList[m_iNowPicChangeIndex]->ShowCtrl(false);
				m_iNowPicChangeIndex += 1;
				if (m_iNowPicChangeIndex >= m_iPicChangeEndIndex)
				{
					// �ӵ����һ�ŵĺ�һ���ˣ�˵��������
					m_iNowPicChangeIndex = m_iPicChangeEndIndex;
					m_iNowAnimeTimes[CATE_PIC_CHANGE] += 1;
					if (m_iNowAnimeTimes[CATE_PIC_CHANGE] >= m_nAnimeTimes[CATE_PIC_CHANGE] && m_nAnimeTimes[CATE_PIC_CHANGE] >= 0)
					{
						// ����ﵽ��������
						m_bAnimePlayedOver[CATE_PIC_CHANGE] = true;
						OnAnimePlayedOver();
					}
					else
					{
						// �´�ѭ��
						m_iNowPicChangeIndex = m_iPicChangeStartIndex;
					}
				}
				// ������ʾ�Ŀؼ�
				m_CtrlList[m_iNowPicChangeIndex]->ShowCtrl(true);
				m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] = 0;
			}
		}
	}

	bool TLGUIContainer::InitPosChangeAnime(LONG startX,LONG startY,LONG endX,LONG endY,float fChangedPerSec,int nTimes)
	{
		m_posChangeStartX = startX;
		m_posChangeStartY = startY;
		m_posChangeEndX = endX;
		m_posChangeEndY = endY;
		m_posX = startX;	// ����Ҫ���ö�����ֵ������
		m_posY = startY;
		MakeReCal(true);
		m_nAnimeTimes[CATE_POS_CHANGE] = nTimes;
		m_iNowAnimeTimes[CATE_POS_CHANGE] = 0;
		m_nAnimeChangedPerSec[CATE_POS_CHANGE] = fChangedPerSec;
		m_nAnimeChangedTimeCount[CATE_POS_CHANGE] = 0;
		m_bAnimePlayedOver[CATE_POS_CHANGE] = false;
		return true;
	}

	void TLGUIContainer::ResetPosChangeAnime()
	{
		m_posChangeStartX = 0;
		m_posChangeStartY = 0;
		m_posChangeEndX = 0;
		m_posChangeEndY = 0;
		m_nAnimeTimes[CATE_POS_CHANGE] = 0;
		m_iNowAnimeTimes[CATE_POS_CHANGE] = 0;
		m_nAnimeChangedPerSec[CATE_POS_CHANGE] = 0;
		m_nAnimeChangedTimeCount[CATE_POS_CHANGE] = 0;
		m_bAnimePlayedOver[CATE_POS_CHANGE] = false;
	}

	void TLGUIContainer::UpdatePosChangeAnime(float fTimeElapsed)
	{
		if (m_iNowAnimeTimes[CATE_POS_CHANGE] < m_nAnimeTimes[CATE_POS_CHANGE] || m_nAnimeTimes[CATE_POS_CHANGE] == -1)
		{
			// �����ǰ���ŵĴ���С��һ��Ҫ���ģ�˵����Ҫ������
			// ֡ʱ�����
			m_nAnimeChangedTimeCount[CATE_POS_CHANGE] += fTimeElapsed;
			if (m_nAnimeChangedTimeCount[CATE_POS_CHANGE] >= 1.0f / m_nAnimeChangedPerSec[CATE_POS_CHANGE])
			{
				// �����һ�����صĲ�ֵ��
				bool bOverX = false;
				bool bOverY = false;
				// ��X
				if (m_posChangeEndX > m_posChangeStartX)
				{
					m_posX += 1;
					if (m_posX >= m_posChangeEndX)
					{
						m_posX = m_posChangeEndX;
						bOverX = true;
					}
				}
				else
				{
					m_posX -= 1;
					if (m_posX <= m_posChangeEndX)
					{
						m_posX = m_posChangeEndX;
						bOverX = true;
					}
				}
				// ��Y
				if (m_posChangeEndY > m_posChangeStartY)
				{
					m_posY += 1;
					if (m_posY >= m_posChangeEndY)
					{
						m_posY = m_posChangeEndY;
						bOverY = true;
					}
				}
				else
				{
					m_posY -= 1;
					if (m_posY <= m_posChangeEndY)
					{
						m_posY = m_posChangeEndY;
						bOverY = true;
					}
				}
				if (bOverX && bOverY)
				{
					// ������һ��
					m_iNowAnimeTimes[CATE_POS_CHANGE] += 1;
					if (m_iNowAnimeTimes[CATE_POS_CHANGE] >= m_nAnimeTimes[CATE_POS_CHANGE] && m_nAnimeTimes[CATE_POS_CHANGE] >= 0)
					{
						// ����ﵽ��������
						m_bAnimePlayedOver[CATE_POS_CHANGE] = true;
						OnAnimePlayedOver();
					}
					else
					{
						// �´�ѭ��
						m_posX = m_posChangeStartX;
						m_posY = m_posChangeStartY;
					}
				}
				// ����Ҫ���¼������λ�õ�
				MakeReCal(true);
				m_nAnimeChangedTimeCount[CATE_POS_CHANGE] = 0;
			}
		}
	}

	void TLGUIContainer::ReCalSubRect(LONG parentFinalX,LONG parentFinalY)
	{
		if (m_bReCal)
		{
			// �����Լ���
			if (m_pParent)
			{
				m_posXParent = parentFinalX;
				m_posYParent = parentFinalY;
				m_posXFinal = parentFinalX + m_posX;
				m_posYFinal = parentFinalY + m_posY;
			}
			else
			{
				m_posXFinal = m_posX;
				m_posYFinal = m_posY;
			}
			// �����ӿؼ�
			std::vector<TLGUICtrl*>::iterator itr = m_CtrlList.begin();
			for (;itr!=m_CtrlList.end();itr++)
			{
				TLGUICtrl* pCtrl = (*itr);
				pCtrl->ReCalSubRect(m_posXFinal,m_posYFinal);
			}
			// ����������
			std::map<int,TLGUIContainer*>::iterator itrC = m_SubContainerTable.begin();
			for (;itrC!=m_SubContainerTable.end();itrC++)
			{
				TLGUIContainer* pSubContainer = itrC->second;
				pSubContainer->ReCalSubRect(m_posXFinal,m_posYFinal);
			}
			m_bReCal = false;
		}
	}

	bool TLGUIContainer::AddContainer(TLGUIContainer* pContainer)
	{
		if (!pContainer)
		{
			return false;
		}
		if (GetSubContainer(pContainer->GetID()))
		{
			return false;
		}
		m_SubContainerTable.insert(std::pair<int,TLGUIContainer*>(pContainer->GetID(),pContainer));
		return true;
	}

	bool TLGUIContainer::AddCtrl(TLGUICtrl* pCtrl)
	{
		if (!pCtrl)
		{
			return false;
		}
		if (GetCtrl(pCtrl->GetIndex()))
		{
			return false;
		}
		m_CtrlList.push_back(pCtrl);
		return true;
	}

	void TLGUIContainer::DestroyContainer()
	{
		m_iID = -1;
		m_pParent = NULL;
		m_pListener = NULL;
		m_posX = 0;
		m_posY = 0;
		m_width = 0;
		m_height = 0;
		m_posXFinal = 0;
		m_posYFinal = 0;
		m_posXParent = 0;
		m_posYParent = 0;
		m_yAnimeType = 0;
		ClearCtrls();
		ClearSubContainers();
	}

	void TLGUIContainer::RemoveSubContainer(int ID)
	{
		TLGUIContainer* pSubContainer = GetSubContainer(ID);
		if (pSubContainer)
		{
			// �ȴ�Table�����ָ��ȥ��
			m_SubContainerTable.erase(ID);
			// �ͷ��ڴ�
			SAFE_DELETE(pSubContainer);
		}
	}

	bool TLGUIContainer::Update(float fTimeElapsed)
	{
		if (!m_bShow)
		{
			return false;
		}
		// ���ݶ������¶���
		if (m_yAnimeType != 0)
		{
			if (m_yAnimeType & CAT_FADE)
			{
				UpdateFadeAnime(fTimeElapsed);
			}
			if (m_yAnimeType & CAT_PIC_CHANGE)
			{
				UpdatePicChangeAnime(fTimeElapsed);
			}
			if (m_yAnimeType & CAT_POS_CHANGE)
			{
				UpdatePosChangeAnime(fTimeElapsed);
			}
		}
		// ��������λ��
		if (m_bReCal)
		{
			ReCalSubRect(m_posXParent,m_posYParent);
		}
		// �����ӿؼ�
		std::vector<TLGUICtrl*>::iterator itr = m_CtrlList.begin();
		for (;itr!=m_CtrlList.end();itr++)
		{
			TLGUICtrl* pCtrl = (*itr);
			pCtrl->Update(fTimeElapsed);
		}
		// ����������
		std::map<int,TLGUIContainer*>::iterator itrC = m_SubContainerTable.begin();
		for (;itrC!=m_SubContainerTable.end();itrC++)
		{
			TLGUIContainer* pSubContainer = itrC->second;
			pSubContainer->Update(fTimeElapsed);
		}
		return true;
	}

	bool TLGUIContainer::Render(float fTimeElapsed)
	{
		if (!m_bShow)
		{
			return false;
		}
		// �����ӿؼ�
		std::vector<TLGUICtrl*>::iterator itr = m_CtrlList.begin();
		for (;itr!=m_CtrlList.end();itr++)
		{
			TLGUICtrl* pCtrl = (*itr);
			pCtrl->Render(fTimeElapsed);
		}
		// ����������
		std::map<int,TLGUIContainer*>::iterator itrC = m_SubContainerTable.begin();
		for (;itrC!=m_SubContainerTable.end();itrC++)
		{
			TLGUIContainer* pSubContainer = itrC->second;
			pSubContainer->Render(fTimeElapsed);
		}
		return true;
	}

	void TLGUIContainer::SetCtrlAlpha(float fAlpha)
	{
		// �����ӿؼ�
		std::vector<TLGUICtrl*>::iterator itr = m_CtrlList.begin();
		for (;itr!=m_CtrlList.end();itr++)
		{
			TLGUICtrl* pCtrl = (*itr);
			pCtrl->SetAlpha(fAlpha);
		}
		// ����������
		std::map<int,TLGUIContainer*>::iterator itrC = m_SubContainerTable.begin();
		for (;itrC!=m_SubContainerTable.end();itrC++)
		{
			TLGUIContainer* pSubContainer = itrC->second;
			pSubContainer->SetCtrlAlpha(fAlpha);
		}
	}

	void TLGUIContainer::ShowContainer(bool bShow /* = true */)
	{
		m_bShow = bShow;
		// �����ӿؼ�
		std::vector<TLGUICtrl*>::iterator itr = m_CtrlList.begin();
		for (;itr!=m_CtrlList.end();itr++)
		{
			TLGUICtrl* pCtrl = (*itr);
			pCtrl->ShowCtrl(bShow);
		}
		// ����������
		std::map<int,TLGUIContainer*>::iterator itrC = m_SubContainerTable.begin();
		for (;itrC!=m_SubContainerTable.end();itrC++)
		{
			TLGUIContainer* pSubContainer = itrC->second;
			pSubContainer->ShowContainer(bShow);
		}
	}

	void TLGUIContainer::OnAnimePlayedOver()
	{
		if (m_pListener)
		{
			m_pListener->OnAnimePlayedOver(this->m_iID,this->m_yAnimeType);
		}
	}
}