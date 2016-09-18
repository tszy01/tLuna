#ifndef TLLOOPCTRL_H
#define TLLOOPCTRL_H

#include "TLCommonTypeDef.h"

namespace TLunaEngine{

	class Timer;
	/*
	*	渲染循环时间控制，帧速显示，等等
	*/
	class LoopCtrl
	{
	public:
		LoopCtrl(void);
		~LoopCtrl(void);
	private:
		//INT64       m_frequency;		// CPU频率
		unsigned long       m_curTime;			// 记录当前帧时间点
		unsigned long       m_lastTime;			// 记录上一帧的时间点,用于Update时间计算
		unsigned long       m_lastUpdate;		// 记录上一帧的时间点,用于FPS计算
		//INT64       m_FPSUpdateInterval;	// FPS用的
		TU32        m_numFrames;		// 计算FPS时用的针数
		unsigned long       m_runTime;		// 已经运行的时间
		unsigned long       m_timeElapsed;	// 渲染时的两帧之间的间隔时间
		float       m_fps;		// 计算出的FPS结果
		TBOOL        m_bStop;			// 是否停止了计时
		unsigned long		m_controlTime;	// 用于计算时间控制的时间差
		float	    m_fSecsPerFrame;	// 每帧的控制时间
		bool	    m_bRender;			// 是否可以渲染了
		bool		m_bTimeControl;		// 是否采用时间控制
		char		m_szFPS[16];		// 转成字符串的FPS值
		Timer*		mTimer;
		float		mCalcPeriod;		// 计算周期
	public:
		// 初始化
		bool Init();
		// 开始计时
		void  StartTime();
		// 停止计时
		void  StopTime();
		// 帧更新
		void  UpdateTime();
		// 计算FPS
		void  CalFPS();
		// 帧更新时检测时间控制
		bool  TimeControl();

		TBOOL  IsStopped()        {  return   m_bStop;    }
		float GetFPS()               {  return   m_fps;         }
		char* GetFPSString()		{ return m_szFPS; }
		int GetFPSStringLen()	{return 16;}
		unsigned long GetRunTime()        {  return   m_runTime;  }
		unsigned long GetElapsedTime()  {  return   m_bStop ? 0 : m_timeElapsed; }
		inline bool CanRender(){return m_bRender;}
		inline void ControlTime(bool bControl,float fSecsPerFrame)
		{
			m_bTimeControl = bControl;
			m_fSecsPerFrame = fSecsPerFrame;
		}
		inline void SetFPSCalcPeriod(float p)
		{
			mCalcPeriod = p;
		}
	};

}

#endif