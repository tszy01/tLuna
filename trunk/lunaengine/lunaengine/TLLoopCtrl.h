#ifndef TLLOOPCTRL_H
#define TLLOOPCTRL_H

#include <Windows.h>
#include "TSingleton.h"

namespace TLunaEngine{

	class TTimer;
	/*
	*	��Ⱦѭ��ʱ����ƣ�֡����ʾ���ȵ�
	*/
	class TLLoopCtrl
	{
	T_SINGLETON_DEF(TLLoopCtrl);
	public:
		TLLoopCtrl(void);
		~TLLoopCtrl(void);
	private:
		//INT64       m_frequency;		// CPUƵ��
		unsigned long       m_curTime;			// ��¼��ǰ֡ʱ���
		unsigned long       m_lastTime;			// ��¼��һ֡��ʱ���,����Updateʱ�����
		unsigned long       m_lastUpdate;		// ��¼��һ֡��ʱ���,����FPS����
		//INT64       m_FPSUpdateInterval;	// FPS�õ�
		UINT        m_numFrames;		// ����FPSʱ�õ�����
		unsigned long       m_runTime;		// �Ѿ����е�ʱ��
		unsigned long       m_timeElapsed;	// ��Ⱦʱ����֮֡��ļ��ʱ��
		float       m_fps;		// �������FPS���
		BOOL        m_bStop;			// �Ƿ�ֹͣ�˼�ʱ
		unsigned long		m_controlTime;	// ���ڼ���ʱ����Ƶ�ʱ���
		float	    m_fSecsPerFrame;	// ÿ֡�Ŀ���ʱ��
		bool	    m_bRender;			// �Ƿ������Ⱦ��
		bool		m_bTimeControl;		// �Ƿ����ʱ�����
		char		m_szFPS[16];		// ת���ַ�����FPSֵ
		TTimer*		mTimer;
		float		mCalcPeriod;		// ��������
	public:
		// ��ʼ��
		bool Init();
		// ��ʼ��ʱ
		void  StartTime();
		// ֹͣ��ʱ
		void  StopTime();
		// ֡����
		void  UpdateTime();
		// ����FPS
		void  CalFPS();
		// ֡����ʱ���ʱ�����
		bool  TimeControl();

		BOOL  IsStopped()        {  return   m_bStop;    }
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