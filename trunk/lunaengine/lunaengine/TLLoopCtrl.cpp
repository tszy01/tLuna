#include "stdafx.h"
#include "TLLoopCtrl.h"
#include "TTimer.h"

namespace TLunaEngine{
	T_SINGLETON_IMP(TLLoopCtrl);

	TLLoopCtrl::TLLoopCtrl(void) : 
	m_curTime(0),
	m_lastTime(0),
	m_lastUpdate(0),
	m_numFrames(0),
	m_runTime(0),
	m_timeElapsed(0),
	m_fps(0),
	m_bStop(TRUE),
	m_fSecsPerFrame(0.01f),
	m_controlTime(0),
	m_bRender(true),
	m_bTimeControl(false),
	mTimer(0),
	mCalcPeriod(1000)
	{
		m_szFPS[0] = '\0';
		sprintf(m_szFPS,"fps:0123456789\0");
	}

	TLLoopCtrl::~TLLoopCtrl(void)
	{
		if(mTimer)
		{
			delete mTimer;
			mTimer = 0;
		}
	}

	bool TLLoopCtrl::Init()
	{
		mTimer = new TTimer();
		return true;
	}

	void TLLoopCtrl::StartTime()
	{
		if ( !m_bStop )
		{
			return;
		}
		m_lastTime = mTimer->getMilliseconds();
		m_bStop = FALSE;
	}
	void TLLoopCtrl::StopTime()
	{
		if ( m_bStop )
		{
			return;
		}
		unsigned long stopTime = mTimer->getMilliseconds();
		m_runTime += stopTime - m_lastTime;
		m_bStop = TRUE;

	}
	void TLLoopCtrl::UpdateTime()
	{
		if ( m_bStop )
		{
			return;
		}
		// Get the current time
		m_curTime = mTimer->getMilliseconds();
	    
		if(m_bTimeControl)
		{
			unsigned long thisElapsed = 0;
			thisElapsed = m_curTime - m_lastTime;
			m_controlTime += thisElapsed;
			m_runTime += thisElapsed;
			if(!TimeControl())
			{
				m_timeElapsed = m_controlTime;
				m_controlTime=0;
				m_bRender=true;
			}
			else
			{
				m_bRender=false;
			}
		}
		else
		{
			m_timeElapsed = m_curTime - m_lastTime;
			m_runTime += m_timeElapsed;
			m_bRender=true;
		}

		m_lastTime = m_curTime;
	}

	bool TLLoopCtrl::TimeControl()
	{
		if((float)m_controlTime<m_fSecsPerFrame*1000.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void TLLoopCtrl::CalFPS()
	{
		// Update FPS
		m_numFrames++;
		if ( m_curTime - m_lastUpdate  >=  mCalcPeriod )
		{
			m_fps  =  (float)m_numFrames / (float)(m_curTime - m_lastUpdate) * 1000.0f;
			sprintf_s(m_szFPS,"fps:%.0f",m_fps);

			m_lastUpdate = m_curTime;
			m_numFrames = 0;
		}
	}

}