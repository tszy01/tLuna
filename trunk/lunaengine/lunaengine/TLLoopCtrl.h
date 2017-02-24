#ifndef TLLOOPCTRL_H
#define TLLOOPCTRL_H

#include "TSCommonTypeDef.h"

namespace TSun
{
	class Timer;
};

namespace TLunaEngine{
	/*
	*	渲染循环时间控制，帧速显示，等等
	*/
	class LoopCtrl
	{
	public:
		LoopCtrl(TSun::TVOID);
		~LoopCtrl(TSun::TVOID);
	private:
		//INT64       m_frequency;		// CPU频率
		TSun::TU32       m_curTime;			// 记录当前帧时间点
		TSun::TU32       m_lastTime;			// 记录上一帧的时间点,用于Update时间计算
		TSun::TU32       m_lastUpdate;		// 记录上一帧的时间点,用于FPS计算
		//INT64       m_FPSUpdateInterval;	// FPS用的
		TSun::TU32        m_numFrames;		// 计算FPS时用的针数
		TSun::TU32       m_runTime;		// 已经运行的时间
		TSun::TU32       m_timeElapsed;	// 渲染时的两帧之间的间隔时间
		TSun::TF32       m_fps;		// 计算出的FPS结果
		TSun::TBOOL        m_bStop;			// 是否停止了计时
		TSun::TU32		m_controlTime;	// 用于计算时间控制的时间差
		TSun::TF32	    m_fSecsPerFrame;	// 每帧的控制时间
		TSun::TBOOL	    m_bRender;			// 是否可以渲染了
		TSun::TBOOL		m_bTimeControl;		// 是否采用时间控制
		TSun::TWCHAR		m_szFPS[16];		// 转成字符串的FPS值
		TSun::Timer*		mTimer;
		TSun::TF32		mCalcPeriod;		// 计算周期
	public:
		// 初始化
		TSun::TBOOL Init();
		// 开始计时
		TSun::TVOID  StartTime();
		// 停止计时
		TSun::TVOID  StopTime();
		// 帧更新
		TSun::TVOID  UpdateTime();
		// 计算FPS
		TSun::TVOID  CalFPS();
		// 帧更新时检测时间控制
		TSun::TBOOL  TimeControl();

		TSun::TBOOL  IsStopped()        {  return   m_bStop;    }
		TSun::TF32 GetFPS()               {  return   m_fps;         }
		TSun::TWCHAR* GetFPSString()		{ return m_szFPS; }
		TSun::TS32 GetFPSStringLen()	{return 16;}
		TSun::TU32 GetRunTime()        {  return   m_runTime;  }
		TSun::TU32 GetElapsedTime()  {  return   m_bStop ? 0 : m_timeElapsed; }
		inline TSun::TBOOL CanRender(){return m_bRender;}
		inline TSun::TVOID ControlTime(TSun::TBOOL bControl, TSun::TF32 fSecsPerFrame)
		{
			m_bTimeControl = bControl;
			m_fSecsPerFrame = fSecsPerFrame;
		}
		inline TSun::TVOID SetFPSCalcPeriod(TSun::TF32 p)
		{
			mCalcPeriod = p;
		}
	};

}

#endif