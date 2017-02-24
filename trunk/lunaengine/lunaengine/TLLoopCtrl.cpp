#include "TLLoopCtrl.h"
#include "TSTimer.h"
#include <stdio.h>

namespace TLunaEngine{
	LoopCtrl::LoopCtrl(TSun::TVOID) : 
	m_curTime(0),
	m_lastTime(0),
	m_lastUpdate(0),
	m_numFrames(0),
	m_runTime(0),
	m_timeElapsed(0),
	m_fps(0),
	m_bStop(TSun::TTRUE),
	m_fSecsPerFrame(0.01f),
	m_controlTime(0),
	m_bRender(TSun::TTRUE),
	m_bTimeControl(TSun::TFALSE),
	mTimer(0),
	mCalcPeriod(1000)
	{
		m_szFPS[0] = L'\0';
		swprintf(m_szFPS,L"fps:0123456789\0");
	}

	LoopCtrl::~LoopCtrl(TSun::TVOID)
	{
		if(mTimer)
		{
			delete mTimer;
			mTimer = 0;
		}
	}

	TSun::TBOOL LoopCtrl::Init()
	{
		mTimer = new TSun::Timer();
		return TSun::TTRUE;
	}

	TSun::TVOID LoopCtrl::StartTime()
	{
		if ( !m_bStop )
		{
			return;
		}
		m_lastTime = mTimer->getMilliseconds();
		m_bStop = TSun::TFALSE;
	}
	TSun::TVOID LoopCtrl::StopTime()
	{
		if ( m_bStop )
		{
			return;
		}
		TSun::TU32 stopTime = mTimer->getMilliseconds();
		m_runTime += stopTime - m_lastTime;
		m_bStop = TSun::TTRUE;

	}
	TSun::TVOID LoopCtrl::UpdateTime()
	{
		if ( m_bStop )
		{
			return;
		}
		// Get the current time
		m_curTime = mTimer->getMilliseconds();
	    
		if(m_bTimeControl)
		{
			TSun::TU32 thisElapsed = 0;
			thisElapsed = m_curTime - m_lastTime;
			m_controlTime += thisElapsed;
			m_runTime += thisElapsed;
			if(!TimeControl())
			{
				m_timeElapsed = m_controlTime;
				m_controlTime=0;
				m_bRender=TSun::TTRUE;
			}
			else
			{
				m_bRender=TSun::TFALSE;
			}
		}
		else
		{
			m_timeElapsed = m_curTime - m_lastTime;
			m_runTime += m_timeElapsed;
			m_bRender=TSun::TTRUE;
		}

		m_lastTime = m_curTime;
	}

	TSun::TBOOL LoopCtrl::TimeControl()
	{
		if((TSun::TF32)m_controlTime<m_fSecsPerFrame*1000.0f)
		{
			return TSun::TTRUE;
		}
		else
		{
			return TSun::TFALSE;
		}
	}

	TSun::TVOID LoopCtrl::CalFPS()
	{
		// Update FPS
		m_numFrames++;
		if ( m_curTime - m_lastUpdate  >=  mCalcPeriod )
		{
			m_fps  =  (TSun::TF32)m_numFrames / (TSun::TF32)(m_curTime - m_lastUpdate) * 1000.0f;
			swprintf_s(m_szFPS,L"fps:%.0f",m_fps);

			m_lastUpdate = m_curTime;
			m_numFrames = 0;
		}
	}

}