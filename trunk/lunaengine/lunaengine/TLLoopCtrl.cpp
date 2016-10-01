#include "TLLoopCtrl.h"
#include "TLTimer.h"

namespace TLunaEngine{
	LoopCtrl::LoopCtrl(TVOID) : 
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
	m_bRender(TTRUE),
	m_bTimeControl(TFALSE),
	mTimer(0),
	mCalcPeriod(1000)
	{
		m_szFPS[0] = '\0';
		sprintf(m_szFPS,"fps:0123456789\0");
	}

	LoopCtrl::~LoopCtrl(TVOID)
	{
		if(mTimer)
		{
			delete mTimer;
			mTimer = 0;
		}
	}

	TBOOL LoopCtrl::Init()
	{
		mTimer = new Timer();
		return TTRUE;
	}

	TVOID LoopCtrl::StartTime()
	{
		if ( !m_bStop )
		{
			return;
		}
		m_lastTime = mTimer->getMilliseconds();
		m_bStop = TFALSE;
	}
	TVOID LoopCtrl::StopTime()
	{
		if ( m_bStop )
		{
			return;
		}
		TU32 stopTime = mTimer->getMilliseconds();
		m_runTime += stopTime - m_lastTime;
		m_bStop = TTRUE;

	}
	TVOID LoopCtrl::UpdateTime()
	{
		if ( m_bStop )
		{
			return;
		}
		// Get the current time
		m_curTime = mTimer->getMilliseconds();
	    
		if(m_bTimeControl)
		{
			TU32 thisElapsed = 0;
			thisElapsed = m_curTime - m_lastTime;
			m_controlTime += thisElapsed;
			m_runTime += thisElapsed;
			if(!TimeControl())
			{
				m_timeElapsed = m_controlTime;
				m_controlTime=0;
				m_bRender=TTRUE;
			}
			else
			{
				m_bRender=TFALSE;
			}
		}
		else
		{
			m_timeElapsed = m_curTime - m_lastTime;
			m_runTime += m_timeElapsed;
			m_bRender=TTRUE;
		}

		m_lastTime = m_curTime;
	}

	TBOOL LoopCtrl::TimeControl()
	{
		if((TF32)m_controlTime<m_fSecsPerFrame*1000.0f)
		{
			return TTRUE;
		}
		else
		{
			return TFALSE;
		}
	}

	TVOID LoopCtrl::CalFPS()
	{
		// Update FPS
		m_numFrames++;
		if ( m_curTime - m_lastUpdate  >=  mCalcPeriod )
		{
			m_fps  =  (TF32)m_numFrames / (TF32)(m_curTime - m_lastUpdate) * 1000.0f;
			sprintf_s(m_szFPS,"fps:%.0f",m_fps);

			m_lastUpdate = m_curTime;
			m_numFrames = 0;
		}
	}

}