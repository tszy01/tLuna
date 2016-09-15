#include "TLLoopCtrl.h"
#include "TLTimer.h"

namespace TLunaEngine{
	LoopCtrl::LoopCtrl(void) : 
	m_curTime(0),
	m_lastTime(0),
	m_lastUpdate(0),
	m_numFrames(0),
	m_runTime(0),
	m_timeElapsed(0),
	m_fps(0),
	m_bStop(TTRUE),
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

	LoopCtrl::~LoopCtrl(void)
	{
		if(mTimer)
		{
			delete mTimer;
			mTimer = 0;
		}
	}

	bool LoopCtrl::Init()
	{
		mTimer = new Timer();
		return true;
	}

	void LoopCtrl::StartTime()
	{
		if ( !m_bStop )
		{
			return;
		}
		m_lastTime = mTimer->getMilliseconds();
		m_bStop = TFALSE;
	}
	void LoopCtrl::StopTime()
	{
		if ( m_bStop )
		{
			return;
		}
		unsigned long stopTime = mTimer->getMilliseconds();
		m_runTime += stopTime - m_lastTime;
		m_bStop = TTRUE;

	}
	void LoopCtrl::UpdateTime()
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

	bool LoopCtrl::TimeControl()
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

	void LoopCtrl::CalFPS()
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