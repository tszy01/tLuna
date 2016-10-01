#include "TLTimer.h"
#include "TLFPU.h"
#include <algorithm>

namespace TLunaEngine{

	//-------------------------------------------------------------------------
	Timer::Timer()
		: mTimerMask( 0 )
	{
		reset();
	}

	//-------------------------------------------------------------------------
	Timer::~Timer()
	{
	}

	//-------------------------------------------------------------------------
	TBOOL Timer::setOption( const String& key, const TVOID * val )
	{
		if ( key == "QueryAffinityMask" )
		{
			// Telling timer what core to use for a timer read
			TU32 newTimerMask = * static_cast < const TU32 * > ( val );

			// Get the current process core mask
			DWORD_PTR procMask;
			DWORD_PTR sysMask;
			GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

			// If new mask is 0, then set to default behavior, otherwise check
			// to make sure new timer core mask overlaps with process core mask
			// and that new timer core mask is a power of 2 (i.e. a single core)
			if( ( newTimerMask == 0 ) ||
				( ( ( newTimerMask & procMask ) != 0 ) && isPO2<TU32>( newTimerMask ) ) )
			{
				mTimerMask = newTimerMask;
				return TTRUE;
			}
		}

		return TFALSE;
	}

	//-------------------------------------------------------------------------
	TVOID Timer::reset()
	{
		// Get the current process core mask
		DWORD_PTR procMask;
		DWORD_PTR sysMask;
		GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

		// If procMask is 0, consider there is only one core available
		// (using 0 as procMask will cause an infinite loop below)
		if (procMask == 0)
			procMask = 1;

		// Find the lowest core that this process uses
		if( mTimerMask == 0 )
		{
			mTimerMask = 1;
			while( ( mTimerMask & procMask ) == 0 )
			{
				mTimerMask <<= 1;
			}
		}

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

		// Get the constant frequency
		QueryPerformanceFrequency(&mFrequency);

		// Query the timer
		QueryPerformanceCounter(&mStartTime);
		mStartTick = GetTickCount();

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		mLastTime = 0;
		mZeroClock = clock();
	}

	//-------------------------------------------------------------------------
	TU32 Timer::getMilliseconds()
	{
		LARGE_INTEGER curTime;

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

		// Query the timer
		QueryPerformanceCounter(&curTime);

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		TS64 newTime = curTime.QuadPart - mStartTime.QuadPart;
    
		// scale by 1000 for milliseconds
		TU32 newTicks = (TU32) (1000 * newTime / mFrequency.QuadPart);

		// detect and compensate for performance counter leaps
		// (surprisingly common, see Microsoft KB: Q274323)
		TU32 check = GetTickCount() - mStartTick;
		TS32 msecOff = (TS32)(newTicks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			// We must keep the timer running forward :)
			TS64 adjust = min_(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
			mStartTime.QuadPart += adjust;
			newTime -= adjust;

			// Re-calculate milliseconds
			newTicks = (TU32) (1000 * newTime / mFrequency.QuadPart);
		}

		// Record last time for adjust
		mLastTime = newTime;

		return newTicks;
	}

	//-------------------------------------------------------------------------
	TU32 Timer::getMicroseconds()
	{
		LARGE_INTEGER curTime;

		HANDLE thread = GetCurrentThread();

		// Set affinity to the first core
		DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);

		// Query the timer
		QueryPerformanceCounter(&curTime);

		// Reset affinity
		SetThreadAffinityMask(thread, oldMask);

		TS64 newTime = curTime.QuadPart - mStartTime.QuadPart;
    
		// get milliseconds to check against GetTickCount
		TU32 newTicks = (TU32) (1000 * newTime / mFrequency.QuadPart);
    
		// detect and compensate for performance counter leaps
		// (surprisingly common, see Microsoft KB: Q274323)
		TU32 check = GetTickCount() - mStartTick;
		TS32 msecOff = (TS32)(newTicks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			// We must keep the timer running forward :)
			TS64 adjust = min_(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
			mStartTime.QuadPart += adjust;
			newTime -= adjust;
		}

		// Record last time for adjust
		mLastTime = newTime;

		// scale by 1000000 for microseconds
		TU32 newMicro = (TU32) (1000000 * newTime / mFrequency.QuadPart);

		return newMicro;
	}

	//-------------------------------------------------------------------------
	TU32 Timer::getMillisecondsCPU()
	{
		clock_t newClock = clock();
		return (TU32)( (TF32)( newClock - mZeroClock ) / ( (TF32)CLOCKS_PER_SEC / 1000.0 ) ) ;
	}

	//-------------------------------------------------------------------------
	TU32 Timer::getMicrosecondsCPU()
	{
		clock_t newClock = clock();
		return (TU32)( (TF32)( newClock - mZeroClock ) / ( (TF32)CLOCKS_PER_SEC / 1000000.0 ) ) ;
	}

}