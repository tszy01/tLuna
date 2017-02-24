#include "TLGUIContainer.h"
#include "TLGUIListener.h"

namespace TLunaEngine{
	GUIContainer::GUIContainer(TSun::TVOID) : 
	m_iID(-1),
	m_posX(0),
	m_posY(0),
	m_width(0),
	m_height(0),
	m_pParent(TSun::TNULL),
	m_posXFinal(0),
	m_posYFinal(0),
	m_posXParent(0),
	m_posYParent(0),
	m_bReCal(TSun::TFALSE),
	m_bShow(TSun::TTRUE),
	m_yAnimeType(0),
	m_pListener(TSun::TNULL)
	{
	}

	GUIContainer::~GUIContainer(TSun::TVOID)
	{
		DestroyContainer();
	}

	TSun::TBOOL GUIContainer::InitContainer(TSun::TS32 ID, TLunaEngine::GUIContainer *pParent, TSun::TS32 x, TSun::TS32 y, TSun::TS32 width, TSun::TS32 height,TSun::TUByte yAnimeType,GUIListener* pListener)
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
		m_bReCal = TSun::TTRUE;
		return TSun::TTRUE;
	}

	TSun::TBOOL GUIContainer::InitFadeAnime(CONTAINER_FADE_TYPE eFadeType, TSun::TF32 fChangedPerSec,TSun::TS32 nTimes)
	{
		if (nTimes==0 || fChangedPerSec<=0 || eFadeType == CFF_NONE)
		{
			return TSun::TFALSE;
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
		m_bAnimePlayedOver[CATE_FADE] = TSun::TFALSE;
		return TSun::TTRUE;
	}

	TSun::TVOID GUIContainer::ResetFadeAnime()
	{
		m_eAnimeFadeType = CFF_NONE;
		m_iNowFadeType = -1;
		m_fNowAlpha = 0;
		m_nAnimeTimes[CATE_FADE] = 0;
		m_iNowAnimeTimes[CATE_FADE] = 0;
		m_nAnimeChangedPerSec[CATE_FADE] = 0;
		m_nAnimeChangedTimeCount[CATE_FADE] = 0;
		m_bAnimePlayedOver[CATE_FADE] = TSun::TFALSE;
	}

	TSun::TVOID GUIContainer::UpdateFadeAnime(TSun::TF32 fTimeElapsed)
	{
		if (m_iNowAnimeTimes[CATE_FADE] < m_nAnimeTimes[CATE_FADE] || m_nAnimeTimes[CATE_FADE] == -1)
		{
			// 计算当前帧需要增加多少值
			TSun::TF32 fNeedPlusAlpha = m_nAnimeChangedPerSec[CATE_FADE] * fTimeElapsed;
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
							m_bAnimePlayedOver[CATE_FADE] = TSun::TTRUE;
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
							m_bAnimePlayedOver[CATE_FADE] = TSun::TTRUE;
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
								m_bAnimePlayedOver[CATE_FADE] = TSun::TTRUE;
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

	TSun::TBOOL GUIContainer::InitPicChangeAnime(TSun::TS32 iStartIndex,TSun::TS32 iEndIndex, TSun::TF32 fChangedPerSec,TSun::TS32 nTimes)
	{
		if (nTimes==0 || fChangedPerSec<=0 || iStartIndex == -1 || iEndIndex == -1)
		{
			return TSun::TFALSE;
		}
		m_iPicChangeStartIndex = iStartIndex;
		m_iPicChangeEndIndex = iEndIndex;
		m_iNowPicChangeIndex = iStartIndex;
		ShowContainer(TSun::TFALSE);
		m_CtrlList[iStartIndex]->ShowCtrl(TSun::TTRUE);
		m_nAnimeTimes[CATE_PIC_CHANGE] = nTimes;
		m_iNowAnimeTimes[CATE_PIC_CHANGE] = 0;
		m_nAnimeChangedPerSec[CATE_PIC_CHANGE] = fChangedPerSec;
		m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] = 0;
		m_bAnimePlayedOver[CATE_PIC_CHANGE] = TSun::TFALSE;
		return TSun::TTRUE;
	}

	TSun::TVOID GUIContainer::ResetPicChangeAnime()
	{
		m_iPicChangeStartIndex = -1;
		m_iPicChangeEndIndex = -1;
		m_iNowPicChangeIndex = -1;
		m_nAnimeTimes[CATE_PIC_CHANGE] = 0;
		m_iNowAnimeTimes[CATE_PIC_CHANGE] = 0;
		m_nAnimeChangedPerSec[CATE_PIC_CHANGE] = 0;
		m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] = 0;
		m_bAnimePlayedOver[CATE_PIC_CHANGE] = TSun::TFALSE;
	}

	TSun::TVOID GUIContainer::UpdatePicChangeAnime(TSun::TF32 fTimeElapsed)
	{
		if (m_iNowAnimeTimes[CATE_PIC_CHANGE] < m_nAnimeTimes[CATE_PIC_CHANGE] || m_nAnimeTimes[CATE_PIC_CHANGE] == -1)
		{
			// 如果当前播放的次数小于一共要播的，说明需要播放了
			// 帧时间计数
			m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] += fTimeElapsed;
			if (m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] >= 1.0f / m_nAnimeChangedPerSec[CATE_PIC_CHANGE])
			{
				// 如果够换一张图片的了
				m_CtrlList[m_iNowPicChangeIndex]->ShowCtrl(TSun::TFALSE);
				m_iNowPicChangeIndex += 1;
				if (m_iNowPicChangeIndex >= m_iPicChangeEndIndex)
				{
					// 加到最后一张的后一张了，说明结束了
					m_iNowPicChangeIndex = m_iPicChangeEndIndex;
					m_iNowAnimeTimes[CATE_PIC_CHANGE] += 1;
					if (m_iNowAnimeTimes[CATE_PIC_CHANGE] >= m_nAnimeTimes[CATE_PIC_CHANGE] && m_nAnimeTimes[CATE_PIC_CHANGE] >= 0)
					{
						// 如果达到播放数量
						m_bAnimePlayedOver[CATE_PIC_CHANGE] = TSun::TTRUE;
						OnAnimePlayedOver();
					}
					else
					{
						// 下次循环
						m_iNowPicChangeIndex = m_iPicChangeStartIndex;
					}
				}
				// 设置显示的控件
				m_CtrlList[m_iNowPicChangeIndex]->ShowCtrl(TSun::TTRUE);
				m_nAnimeChangedTimeCount[CATE_PIC_CHANGE] = 0;
			}
		}
	}

	TSun::TBOOL GUIContainer::InitPosChangeAnime(TSun::TS32 startX,TSun::TS32 startY,TSun::TS32 endX,TSun::TS32 endY, TSun::TF32 fChangedPerSec,TSun::TS32 nTimes)
	{
		m_posChangeStartX = startX;
		m_posChangeStartY = startY;
		m_posChangeEndX = endX;
		m_posChangeEndY = endY;
		m_posX = startX;	// 这里要设置动画初值给容器
		m_posY = startY;
		MakeReCal(TSun::TTRUE);
		m_nAnimeTimes[CATE_POS_CHANGE] = nTimes;
		m_iNowAnimeTimes[CATE_POS_CHANGE] = 0;
		m_nAnimeChangedPerSec[CATE_POS_CHANGE] = fChangedPerSec;
		m_nAnimeChangedTimeCount[CATE_POS_CHANGE] = 0;
		m_bAnimePlayedOver[CATE_POS_CHANGE] = TSun::TFALSE;
		return TSun::TTRUE;
	}

	TSun::TVOID GUIContainer::ResetPosChangeAnime()
	{
		m_posChangeStartX = 0;
		m_posChangeStartY = 0;
		m_posChangeEndX = 0;
		m_posChangeEndY = 0;
		m_nAnimeTimes[CATE_POS_CHANGE] = 0;
		m_iNowAnimeTimes[CATE_POS_CHANGE] = 0;
		m_nAnimeChangedPerSec[CATE_POS_CHANGE] = 0;
		m_nAnimeChangedTimeCount[CATE_POS_CHANGE] = 0;
		m_bAnimePlayedOver[CATE_POS_CHANGE] = TSun::TFALSE;
	}

	TSun::TVOID GUIContainer::UpdatePosChangeAnime(TSun::TF32 fTimeElapsed)
	{
		if (m_iNowAnimeTimes[CATE_POS_CHANGE] < m_nAnimeTimes[CATE_POS_CHANGE] || m_nAnimeTimes[CATE_POS_CHANGE] == -1)
		{
			// 如果当前播放的次数小于一共要播的，说明需要播放了
			// 帧时间计数
			m_nAnimeChangedTimeCount[CATE_POS_CHANGE] += fTimeElapsed;
			if (m_nAnimeChangedTimeCount[CATE_POS_CHANGE] >= 1.0f / m_nAnimeChangedPerSec[CATE_POS_CHANGE])
			{
				// 如果够一个像素的差值了
				TSun::TBOOL bOverX = TSun::TFALSE;
				TSun::TBOOL bOverY = TSun::TFALSE;
				// 算X
				if (m_posChangeEndX > m_posChangeStartX)
				{
					m_posX += 1;
					if (m_posX >= m_posChangeEndX)
					{
						m_posX = m_posChangeEndX;
						bOverX = TSun::TTRUE;
					}
				}
				else
				{
					m_posX -= 1;
					if (m_posX <= m_posChangeEndX)
					{
						m_posX = m_posChangeEndX;
						bOverX = TSun::TTRUE;
					}
				}
				// 算Y
				if (m_posChangeEndY > m_posChangeStartY)
				{
					m_posY += 1;
					if (m_posY >= m_posChangeEndY)
					{
						m_posY = m_posChangeEndY;
						bOverY = TSun::TTRUE;
					}
				}
				else
				{
					m_posY -= 1;
					if (m_posY <= m_posChangeEndY)
					{
						m_posY = m_posChangeEndY;
						bOverY = TSun::TTRUE;
					}
				}
				if (bOverX && bOverY)
				{
					// 结束了一次
					m_iNowAnimeTimes[CATE_POS_CHANGE] += 1;
					if (m_iNowAnimeTimes[CATE_POS_CHANGE] >= m_nAnimeTimes[CATE_POS_CHANGE] && m_nAnimeTimes[CATE_POS_CHANGE] >= 0)
					{
						// 如果达到播放数量
						m_bAnimePlayedOver[CATE_POS_CHANGE] = TSun::TTRUE;
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
				MakeReCal(TSun::TTRUE);
				m_nAnimeChangedTimeCount[CATE_POS_CHANGE] = 0;
			}
		}
	}

	TSun::TVOID GUIContainer::ReCalSubRect(TSun::TS32 parentFinalX,TSun::TS32 parentFinalY)
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
			TSun::List<GUICtrl*>::Iterator itr = m_CtrlList.begin();
			for (;itr!=m_CtrlList.end();itr++)
			{
				GUICtrl* pCtrl = (*itr);
				pCtrl->ReCalSubRect(m_posXFinal,m_posYFinal);
			}
			// 计算子容器
			TSun::Map<TSun::TS32,GUIContainer*>::Iterator itrC = m_SubContainerTable.begin();
			for (;itrC!=m_SubContainerTable.end();itrC++)
			{
				GUIContainer* pSubContainer = itrC->Value;
				pSubContainer->ReCalSubRect(m_posXFinal,m_posYFinal);
			}
			m_bReCal = TSun::TFALSE;
		}
	}

	TSun::TBOOL GUIContainer::AddContainer(GUIContainer* pContainer)
	{
		if (!pContainer)
		{
			return TSun::TFALSE;
		}
		if (GetSubContainer(pContainer->GetID()))
		{
			return TSun::TFALSE;
		}
		m_SubContainerTable.push_back(pContainer->GetID(),pContainer);
		return TSun::TTRUE;
	}

	TSun::TBOOL GUIContainer::AddCtrl(GUICtrl* pCtrl)
	{
		if (!pCtrl)
		{
			return TSun::TFALSE;
		}
		if (GetCtrl(pCtrl->GetIndex()))
		{
			return TSun::TFALSE;
		}
		m_CtrlList.push_back(pCtrl);
		return TSun::TTRUE;
	}

	TSun::TVOID GUIContainer::DestroyContainer()
	{
		m_iID = -1;
		m_pParent = TSun::TNULL;
		m_pListener = TSun::TNULL;
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

	TSun::TVOID GUIContainer::RemoveSubContainer(TSun::TS32 ID)
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

	TSun::TBOOL GUIContainer::Update(TSun::TF32 fTimeElapsed)
	{
		if (!m_bShow)
		{
			return TSun::TFALSE;
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
		TSun::List<GUICtrl*>::Iterator itr = m_CtrlList.begin();
		for (;itr!=m_CtrlList.end();itr++)
		{
			GUICtrl* pCtrl = (*itr);
			pCtrl->Update(fTimeElapsed);
		}
		// 计算子容器
		TSun::Map<TSun::TS32,GUIContainer*>::Iterator itrC = m_SubContainerTable.begin();
		for (;itrC!=m_SubContainerTable.end();itrC++)
		{
			GUIContainer* pSubContainer = itrC->Value;
			pSubContainer->Update(fTimeElapsed);
		}
		return TSun::TTRUE;
	}

	TSun::TBOOL GUIContainer::Render(TSun::TF32 fTimeElapsed)
	{
		if (!m_bShow)
		{
			return TSun::TFALSE;
		}
		// 计算子控件
		TSun::List<GUICtrl*>::Iterator itr = m_CtrlList.begin();
		for (;itr!=m_CtrlList.end();itr++)
		{
			GUICtrl* pCtrl = (*itr);
			pCtrl->Render(fTimeElapsed);
		}
		// 计算子容器
		TSun::Map<TSun::TS32,GUIContainer*>::Iterator itrC = m_SubContainerTable.begin();
		for (;itrC!=m_SubContainerTable.end();itrC++)
		{
			GUIContainer* pSubContainer = itrC->Value;
			pSubContainer->Render(fTimeElapsed);
		}
		return TSun::TTRUE;
	}

	TSun::TVOID GUIContainer::SetCtrlAlpha(TSun::TF32 fAlpha)
	{
		// 计算子控件
		TSun::List<GUICtrl*>::Iterator itr = m_CtrlList.begin();
		for (;itr!=m_CtrlList.end();itr++)
		{
			GUICtrl* pCtrl = (*itr);
			pCtrl->SetAlpha(fAlpha);
		}
		// 计算子容器
		TSun::Map<TSun::TS32,GUIContainer*>::Iterator itrC = m_SubContainerTable.begin();
		for (;itrC!=m_SubContainerTable.end();itrC++)
		{
			GUIContainer* pSubContainer = itrC->Value;
			pSubContainer->SetCtrlAlpha(fAlpha);
		}
	}

	TSun::TVOID GUIContainer::ShowContainer(TSun::TBOOL bShow /* = TSun::TTRUE */)
	{
		m_bShow = bShow;
		// 计算子控件
		TSun::List<GUICtrl*>::Iterator itr = m_CtrlList.begin();
		for (;itr!=m_CtrlList.end();itr++)
		{
			GUICtrl* pCtrl = (*itr);
			pCtrl->ShowCtrl(bShow);
		}
		// 计算子容器
		TSun::Map<TSun::TS32,GUIContainer*>::Iterator itrC = m_SubContainerTable.begin();
		for (;itrC!=m_SubContainerTable.end();itrC++)
		{
			GUIContainer* pSubContainer = itrC->Value;
			pSubContainer->ShowContainer(bShow);
		}
	}

	TSun::TVOID GUIContainer::OnAnimePlayedOver()
	{
		if (m_pListener)
		{
			m_pListener->OnAnimePlayedOver(this->m_iID,this->m_yAnimeType);
		}
	}
}