#include "TLGUICtrl.h"
namespace TLunaEngine{

	GUICtrl::GUICtrl(TSun::TVOID) : 
	m_eCtrlType(GUI_CTRL_NONE),
	m_iIndex(-1),
	m_pContainer(TSun::TNULL),
	m_posX(0),
	m_posY(0),
	m_width(0),
	m_height(0),
	m_posXFinal(0),
	m_posYFinal(0),
	m_bShow(TSun::TTRUE),
	m_fAlpha(1)
	{
	}

	GUICtrl::~GUICtrl(TSun::TVOID)
	{
		DestroyCtrl();
	}

	TSun::TBOOL GUICtrl::InitCtrlBase(TSun::TS32 iIndex, TLunaEngine::GUIContainer *pContainer, TSun::TS32 x, TSun::TS32 y, TSun::TS32 width, TSun::TS32 height)
	{
		m_iIndex = iIndex;
		m_pContainer = pContainer;
		m_posX = x;
		m_posY = y;
		m_width = width;
		m_height = height;
		m_posXFinal = x;
		m_posYFinal = y;
		return TSun::TTRUE;
	}

	TSun::TVOID GUICtrl::DestroyCtrl()
	{
		m_iIndex = -1;
		m_pContainer = TSun::TNULL;
		m_posX = 0;
		m_posY = 0;
		m_width = 0;
		m_height = 0;
		m_posXFinal = 0;
		m_posYFinal = 0;
	}

	TSun::TVOID GUICtrl::ReCalSubRect(TSun::TS32 containerFinalX,TSun::TS32 containerFinalY)
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

	TSun::TBOOL GUICtrl::Update(TSun::TF32 fTimeElapsed)
	{
		if (!m_bShow)
		{
			return TSun::TFALSE;
		}
		return TSun::TTRUE;
	}

	TSun::TBOOL GUICtrl::Render(TSun::TF32 fTimeElapsed)
	{
		if (!m_bShow)
		{
			return TSun::TFALSE;
		}
		return TSun::TTRUE;
	}

}