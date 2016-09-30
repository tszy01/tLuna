#include "TLGUIContainer.h"
#include "TLGUIListener.h"

namespace TLunaEngine{
	GUIContainer::GUIContainer(TVOID) : 
	m_iID(-1),
	m_posX(0),
	m_posY(0),
	m_width(0),
	m_height(0),
	m_pParent(TNULL),
	m_posXFinal(0),
	m_posYFinal(0),
	m_posXParent(0),
	m_posYParent(0),
	m_bReCal(TFALSE),
	m_bShow(TTRUE),
	m_yAnimeType(0),
	m_pListener(TNULL)
	{
	}

	GUIContainer::~GUIContainer(TVOID)
	{
		DestroyContainer();
	}

	TBOOL GUIContainer::InitContainer(int ID, TLunaEngine::GUIContainer *pParent, TS32 x, TS32 y, TS32 width, TS32 height,TUByte yAnimeType,GUIListener* pListener)
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
		// 使重新计算位置
		m_bReCal = TTRUE;
		return TTRUE;
	}

	TBOOL GUIContainer::InitFadeAnime(CONTAINER_FADE_TYPE eFadeType,float fChangedPerSec,int nTimes)
	{
		if (nTimes==0 || fChangedPerSec<=0 || eFadeType == CFF_NONE)
		{
			return TFALSE;
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
		m_bAnimePlayedOver[CATE_FADE] = TFALSE;
		return TTRUE;
	}

	TVOID GUIContainer::ResetFadeAnime()
	{
		m_eAnimeFadeType = CFF_NONE;
		m_iNowFadeType = -1;
		m_fNowAlpha = 0;
		m_nAnimeTimes[CATE_FADE] = 0;
		m_iNowAnimeTimes[CATE_FADE] = 0;
		m_nAnimeChangedPerSec[CATE_FADE] = 0;
		m_nAnimeChangedTimeCount[CATE_FADE] = 0;
		m_bAnimePlayedOver[CATE_FADE] = TFALSE;
	}

	TVOID GUIContainer::UpdateFadeAnime(float fTimeElapsed)
	{
		if (m_iNowAnimeTimes[CATE_FADE] < m_nAnimeTimes[CATE_FADE] || m_nAnimeTimes[CATE_FADE] == -1)
		{
			// 计算当前帧需要增加多少值
			float fNeedPlusAlpha = m_nAnimeChangedPerSec[CATE_FADE] * fTimeElapsed;
			// 如果当前播放的次数小于一共要播的，说明需要播放了
			switch (m_eAnimeFadeType)
			{
			case CFF_FADE_IN:
				{
					// 帧时间计数
					m_fNowAlpha += fNeedPlusAlpha;
					if (m_fNowAlpha >= 1.0f)
					{
						// 如果ALPHA加到1了，本次动画结束
						m_fNowAlpha = 1.0f;
						m_iNowAnimeTimes[CATE_FADE] += 1;
						if (m_iNowAnimeTimes[CATE_FADE] >= m_nAnimeTimes[CATE_FADE] && m_nAnimeTimes[CATE_FADE] >= 0)
						{
							// 如果到达指定数量了
							m_bAnimePlayedOver[CATE_FADE] = TTRUE;
							m_iNowFadeType = -1;
							OnAnimePlayedOver();
						}
						else
						{
							// 准备下一次循环
							m_fNowAlpha = 0;
						}
					}
					// 设置Alpha
					SetCtrlAlpha(m_fNowAlpha);
				}
				break;
			case CFF_FADE_OUT:
				{
					// 帧时间计数
					m_fNowAlpha -= fNeedPlusAlpha;
					if (m_fNowAlpha <= 0.0f)
					{
						// 如果ALPHA加到1了，本次动画结束
						m_fNowAlpha = 0.0f;
						m_iNowAnimeTimes[CATE_FADE] += 1;
						if (m_iNowAnimeTimes[CATE_FADE] >= m_nAnimeTimes[CATE_FADE] && m_nAnimeTimes[CATE_FADE] >= 0)
						{
							// 如果到达指定数量了
							m_bAnimePlayedOver[CATE_FADE] = TTRUE;
							m_iNowFadeType = -1;
							OnAnimePlayedOver();
						}
						else
						{
							// 下一次循环
							m_fNowAlpha = 1.0f;
						}
					}
					// 设置Alpha
					SetCtrlAlpha(m_fNowAlpha);
				}
				break;
			case CFF_FADE_IN_OUT:
				{
					// 帧时间计数
					if (m_iNowFadeType==0)
					{
						// 递增过程
						m_fNowAlpha += fNeedPlusAlpha;
						if (m_fNowAlpha >= 1.0f)
						{
							// 如果ALPHA加到1了，就改减少了
							m_fNowAlpha = 1.0f;
							m_iNowFadeType = 1;
						}
					}
					else if(m_iNowFadeType==1)
					{
						// 递减过程
						m_fNowAlpha -= fNeedPlusAlpha;
						if (m_fNowAlpha <= 0.0f)
						{
							// 如果ALPHA加到1了，本次动画结束
							m_fNowAlpha = 0.0f;
							m_iNowAnimeTimes[CATE_FADE] += 1;
							if (m_iNowAnimeTimes[CATE_FADE] >= m_nAnimeTimes[CATE_FADE] && m_nAnimeTimes[CATE_FADE] >= 0)
							{
								// 如果到达指定数量了
								m_bAnimePlayedOver[CATE_FADE] = TTRUE;
								m_iNowFadeType = -1;
								OnAnimePlayedOver();
							}
							else
							{
								// 下一次循环
								m_fNowAlpha = 0;
								m_iNowFadeType = 0;
							}
						}
					}
					// 设置Alpha
					SetCtrlAlpha(m_fNowAlpha);
				}
				break;
			}
		}
	}

	TBOOL GUIContainer::InitPicChangeAnime(int iStartIndex,int iEndIndex,float fChangedPerSec,int nTimes)
	{
		if (nTimes==0 || fChangedPerSec<=0 || iStartIndex == -1 || iEndIndex == -1)
		{
			return TFALSE;
		}
		m_iPicChangeStartIndex = iStartIndex;
		m_iPicChangeEndIndex = iEndIndex;
		m_iNowPicChangeIndex = iStartIndex;
		ShowContainer(TFALSE);
		m_CtrlList[iStartIndex]->ShowCtrl(TTRUE);
		m_nAnimeTimes[CATE_PIC_CHANGE] = nTimes;
		m_iNowAnimeTimes[CATE_PIC_CHANGE] = 0;
		m_nAnimeChangedPerSec[CATE_PIC_CHANGE] = fChangedPerSec;
		m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] = 0;
		m_bAnimePlayedOver[CATE_PIC_CHANGE] = TFALSE;
		return TTRUE;
	}

	TVOID GUIContainer::ResetPicChangeAnime()
	{
		m_iPicChangeStartIndex = -1;
		m_iPicChangeEndIndex = -1;
		m_iNowPicChangeIndex = -1;
		m_nAnimeTimes[CATE_PIC_CHANGE] = 0;
		m_iNowAnimeTimes[CATE_PIC_CHANGE] = 0;
		m_nAnimeChangedPerSec[CATE_PIC_CHANGE] = 0;
		m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] = 0;
		m_bAnimePlayedOver[CATE_PIC_CHANGE] = TFALSE;
	}

	TVOID GUIContainer::UpdatePicChangeAnime(float fTimeElapsed)
	{
		if (m_iNowAnimeTimes[CATE_PIC_CHANGE] < m_nAnimeTimes[CATE_PIC_CHANGE] || m_nAnimeTimes[CATE_PIC_CHANGE] == -1)
		{
			// 如果当前播放的次数小于一共要播的，说明需要播放了
			// 帧时间计数
			m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] += fTimeElapsed;
			if (m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] >= 1.0f / m_nAnimeChangedPerSec[CATE_PIC_CHANGE])
			{
				// 如果够换一张图片的了
				m_CtrlList[m_iNowPicChangeIndex]->ShowCtrl(TFALSE);
				m_iNowPicChangeIndex += 1;
				if (m_iNowPicChangeIndex >= m_iPicChangeEndIndex)
				{
					// 加到最后一张的后一张了，说明结束了
					m_iNowPicChangeIndex = m_iPicChangeEndIndex;
					m_iNowAnimeTimes[CATE_PIC_CHANGE] += 1;
					if (m_iNowAnimeTimes[CATE_PIC_CHANGE] >= m_nAnimeTimes[CATE_PIC_CHANGE] && m_nAnimeTimes[CATE_PIC_CHANGE] >= 0)
					{
						// 如果达到播放数量
						m_bAnimePlayedOver[CATE_PIC_CHANGE] = TTRUE;
						OnAnimePlayedOver();
					}
					else
					{
						// 下次循环
						m_iNowPicChangeIndex = m_iPicChangeStartIndex;
					}
				}
				// 设置显示的控件
				m_CtrlList[m_iNowPicChangeIndex]->ShowCtrl(TTRUE);
				m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] = 0;
			}
		}
	}

	TBOOL GUIContainer::InitPosChangeAnime(TS32 startX,TS32 startY,TS32 endX,TS32 endY,float fChangedPerSec,int nTimes)
	{
		m_posChangeStartX = startX;
		m_posChangeStartY = startY;
		m_posChangeEndX = endX;
		m_posChangeEndY = endY;
		m_posX = startX;	// 这里要设置动画初值给容器
		m_posY = startY;
		MakeReCal(TTRUE);
		m_nAnimeTimes[CATE_POS_CHANGE] = nTimes;
		m_iNowAnimeTimes[CATE_POS_CHANGE] = 0;
		m_nAnimeChangedPerSec[CATE_POS_CHANGE] = fChangedPerSec;
		m_nAnimeChangedTimeCount[CATE_POS_CHANGE] = 0;
		m_bAnimePlayedOver[CATE_POS_CHANGE] = TFALSE;
		return TTRUE;
	}

	TVOID GUIContainer::ResetPosChangeAnime()
	{
		m_posChangeStartX = 0;
		m_posChangeStartY = 0;
		m_posChangeEndX = 0;
		m_posChangeEndY = 0;
		m_nAnimeTimes[CATE_POS_CHANGE] = 0;
		m_iNowAnimeTimes[CATE_POS_CHANGE] = 0;
		m_nAnimeChangedPerSec[CATE_POS_CHANGE] = 0;
		m_nAnimeChangedTimeCount[CATE_POS_CHANGE] = 0;
		m_bAnimePlayedOver[CATE_POS_CHANGE] = TFALSE;
	}

	TVOID GUIContainer::UpdatePosChangeAnime(float fTimeElapsed)
	{
		if (m_iNowAnimeTimes[CATE_POS_CHANGE] < m_nAnimeTimes[CATE_POS_CHANGE] || m_nAnimeTimes[CATE_POS_CHANGE] == -1)
		{
			// 如果当前播放的次数小于一共要播的，说明需要播放了
			// 帧时间计数
			m_nAnimeChangedTimeCount[CATE_POS_CHANGE] += fTimeElapsed;
			if (m_nAnimeChangedTimeCount[CATE_POS_CHANGE] >= 1.0f / m_nAnimeChangedPerSec[CATE_POS_CHANGE])
			{
				// 如果够一个像素的差值了
				TBOOL bOverX = TFALSE;
				TBOOL bOverY = TFALSE;
				// 算X
				if (m_posChangeEndX > m_posChangeStartX)
				{
					m_posX += 1;
					if (m_posX >= m_posChangeEndX)
					{
						m_posX = m_posChangeEndX;
						bOverX = TTRUE;
					}
				}
				else
				{
					m_posX -= 1;
					if (m_posX <= m_posChangeEndX)
					{
						m_posX = m_posChangeEndX;
						bOverX = TTRUE;
					}
				}
				// 算Y
				if (m_posChangeEndY > m_posChangeStartY)
				{
					m_posY += 1;
					if (m_posY >= m_posChangeEndY)
					{
						m_posY = m_posChangeEndY;
						bOverY = TTRUE;
					}
				}
				else
				{
					m_posY -= 1;
					if (m_posY <= m_posChangeEndY)
					{
						m_posY = m_posChangeEndY;
						bOverY = TTRUE;
					}
				}
				if (bOverX && bOverY)
				{
					// 结束了一次
					m_iNowAnimeTimes[CATE_POS_CHANGE] += 1;
					if (m_iNowAnimeTimes[CATE_POS_CHANGE] >= m_nAnimeTimes[CATE_POS_CHANGE] && m_nAnimeTimes[CATE_POS_CHANGE] >= 0)
					{
						// 如果达到播放数量
						m_bAnimePlayedOver[CATE_POS_CHANGE] = TTRUE;
						OnAnimePlayedOver();
					}
					else
					{
						// 下次循环
						m_posX = m_posChangeStartX;
						m_posY = m_posChangeStartY;
					}
				}
				// 这里要重新计算绝对位置的
				MakeReCal(TTRUE);
				m_nAnimeChangedTimeCount[CATE_POS_CHANGE] = 0;
			}
		}
	}

	TVOID GUIContainer::ReCalSubRect(TS32 parentFinalX,TS32 parentFinalY)
	{
		if (m_bReCal)
		{
			// 计算自己的
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
			// 计算子控件
			std::vector<GUICtrl*>::iterator itr = m_CtrlList.begin();
			for (;itr!=m_CtrlList.end();itr++)
			{
				GUICtrl* pCtrl = (*itr);
				pCtrl->ReCalSubRect(m_posXFinal,m_posYFinal);
			}
			// 计算子容器
			std::map<int,GUIContainer*>::iterator itrC = m_SubContainerTable.begin();
			for (;itrC!=m_SubContainerTable.end();itrC++)
			{
				GUIContainer* pSubContainer = itrC->second;
				pSubContainer->ReCalSubRect(m_posXFinal,m_posYFinal);
			}
			m_bReCal = TFALSE;
		}
	}

	TBOOL GUIContainer::AddContainer(GUIContainer* pContainer)
	{
		if (!pContainer)
		{
			return TFALSE;
		}
		if (GetSubContainer(pContainer->GetID()))
		{
			return TFALSE;
		}
		m_SubContainerTable.insert(std::pair<int,GUIContainer*>(pContainer->GetID(),pContainer));
		return TTRUE;
	}

	TBOOL GUIContainer::AddCtrl(GUICtrl* pCtrl)
	{
		if (!pCtrl)
		{
			return TFALSE;
		}
		if (GetCtrl(pCtrl->GetIndex()))
		{
			return TFALSE;
		}
		m_CtrlList.push_back(pCtrl);
		return TTRUE;
	}

	TVOID GUIContainer::DestroyContainer()
	{
		m_iID = -1;
		m_pParent = TNULL;
		m_pListener = TNULL;
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

	TVOID GUIContainer::RemoveSubContainer(int ID)
	{
		GUIContainer* pSubContainer = GetSubContainer(ID);
		if (pSubContainer)
		{
			// 先从Table里面把指针去掉
			m_SubContainerTable.erase(ID);
			// 释放内存
			if (pSubContainer)
			{
				delete pSubContainer;
				pSubContainer = 0;
			}
		}
	}

	TBOOL GUIContainer::Update(float fTimeElapsed)
	{
		if (!m_bShow)
		{
			return TFALSE;
		}
		// 根据动画更新动画
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
		// 重新设置位置
		if (m_bReCal)
		{
			ReCalSubRect(m_posXParent,m_posYParent);
		}
		// 计算子控件
		std::vector<GUICtrl*>::iterator itr = m_CtrlList.begin();
		for (;itr!=m_CtrlList.end();itr++)
		{
			GUICtrl* pCtrl = (*itr);
			pCtrl->Update(fTimeElapsed);
		}
		// 计算子容器
		std::map<int,GUIContainer*>::iterator itrC = m_SubContainerTable.begin();
		for (;itrC!=m_SubContainerTable.end();itrC++)
		{
			GUIContainer* pSubContainer = itrC->second;
			pSubContainer->Update(fTimeElapsed);
		}
		return TTRUE;
	}

	TBOOL GUIContainer::Render(float fTimeElapsed)
	{
		if (!m_bShow)
		{
			return TFALSE;
		}
		// 计算子控件
		std::vector<GUICtrl*>::iterator itr = m_CtrlList.begin();
		for (;itr!=m_CtrlList.end();itr++)
		{
			GUICtrl* pCtrl = (*itr);
			pCtrl->Render(fTimeElapsed);
		}
		// 计算子容器
		std::map<int,GUIContainer*>::iterator itrC = m_SubContainerTable.begin();
		for (;itrC!=m_SubContainerTable.end();itrC++)
		{
			GUIContainer* pSubContainer = itrC->second;
			pSubContainer->Render(fTimeElapsed);
		}
		return TTRUE;
	}

	TVOID GUIContainer::SetCtrlAlpha(float fAlpha)
	{
		// 计算子控件
		std::vector<GUICtrl*>::iterator itr = m_CtrlList.begin();
		for (;itr!=m_CtrlList.end();itr++)
		{
			GUICtrl* pCtrl = (*itr);
			pCtrl->SetAlpha(fAlpha);
		}
		// 计算子容器
		std::map<int,GUIContainer*>::iterator itrC = m_SubContainerTable.begin();
		for (;itrC!=m_SubContainerTable.end();itrC++)
		{
			GUIContainer* pSubContainer = itrC->second;
			pSubContainer->SetCtrlAlpha(fAlpha);
		}
	}

	TVOID GUIContainer::ShowContainer(TBOOL bShow /* = TTRUE */)
	{
		m_bShow = bShow;
		// 计算子控件
		std::vector<GUICtrl*>::iterator itr = m_CtrlList.begin();
		for (;itr!=m_CtrlList.end();itr++)
		{
			GUICtrl* pCtrl = (*itr);
			pCtrl->ShowCtrl(bShow);
		}
		// 计算子容器
		std::map<int,GUIContainer*>::iterator itrC = m_SubContainerTable.begin();
		for (;itrC!=m_SubContainerTable.end();itrC++)
		{
			GUIContainer* pSubContainer = itrC->second;
			pSubContainer->ShowContainer(bShow);
		}
	}

	TVOID GUIContainer::OnAnimePlayedOver()
	{
		if (m_pListener)
		{
			m_pListener->OnAnimePlayedOver(this->m_iID,this->m_yAnimeType);
		}
	}
}