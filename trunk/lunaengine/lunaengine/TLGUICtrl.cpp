#include "StdAfx.h"
#include "TLGUICtrl.h"
namespace TLunaEngine{

	TLGUICtrl::TLGUICtrl(void) : 
	m_eCtrlType(GUI_CTRL_NONE),
	m_iIndex(-1),
	m_pContainer(NULL),
	m_posX(0),
	m_posY(0),
	m_width(0),
	m_height(0),
	m_posXFinal(0),
	m_posYFinal(0),
	m_bShow(true),
	m_fAlpha(1)
	{
	}

	TLGUICtrl::~TLGUICtrl(void)
	{
		DestroyCtrl();
	}

	bool TLGUICtrl::InitCtrlBase(int iIndex, TLunaEngine::TLGUIContainer *pContainer, LONG x, LONG y, LONG width, LONG height)
	{
		m_iIndex = iIndex;
		m_pContainer = pContainer;
		m_posX = x;
		m_posY = y;
		m_width = width;
		m_height = height;
		m_posXFinal = x;
		m_posYFinal = y;
		return true;
	}

	void TLGUICtrl::DestroyCtrl()
	{
		m_iIndex = -1;
		m_pContainer = NULL;
		m_posX = 0;
		m_posY = 0;
		m_width = 0;
		m_height = 0;
		m_posXFinal = 0;
		m_posYFinal = 0;
	}

	void TLGUICtrl::ReCalSubRect(LONG containerFinalX,LONG containerFinalY)
	{
		if (m_pContainer)
		{
			m_posXFinal = containerFinalX + m_posX;
			m_posYFinal = containerFinalY + m_posY;
		}
		else
		{
			m_posXFinal = m_posX;
			m_posYFinal = m_posY;
		}
	}

	bool TLGUICtrl::Update(float fTimeElapsed)
	{
		if (!m_bShow)
		{
			return false;
		}
		return true;
	}

	bool TLGUICtrl::Render(float fTimeElapsed)
	{
		if (!m_bShow)
		{
			return false;
		}
		return true;
	}

}