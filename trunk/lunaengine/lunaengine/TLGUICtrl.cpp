#include "TLGUICtrl.h"
namespace TLunaEngine{

	GUICtrl::GUICtrl(void) : 
	m_eCtrlType(GUI_CTRL_NONE),
	m_iIndex(-1),
	m_pContainer(TNULL),
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

	GUICtrl::~GUICtrl(void)
	{
		DestroyCtrl();
	}

	bool GUICtrl::InitCtrlBase(int iIndex, TLunaEngine::GUIContainer *pContainer, TS32 x, TS32 y, TS32 width, TS32 height)
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

	void GUICtrl::DestroyCtrl()
	{
		m_iIndex = -1;
		m_pContainer = TNULL;
		m_posX = 0;
		m_posY = 0;
		m_width = 0;
		m_height = 0;
		m_posXFinal = 0;
		m_posYFinal = 0;
	}

	void GUICtrl::ReCalSubRect(TS32 containerFinalX,TS32 containerFinalY)
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

	bool GUICtrl::Update(float fTimeElapsed)
	{
		if (!m_bShow)
		{
			return false;
		}
		return true;
	}

	bool GUICtrl::Render(float fTimeElapsed)
	{
		if (!m_bShow)
		{
			return false;
		}
		return true;
	}

}