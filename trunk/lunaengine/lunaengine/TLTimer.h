#ifndef TLTIMER_H
#define TLTIMER_H

#include "TLCommonTypeDef.h"
#include <Windows.h>
#include <time.h>
#include "TLString.h"

namespace TLunaEngine{

	class Timer
	{
	private:
		clock_t mZeroClock;

		TU32 mStartTick;
		TS64 mLastTime;
		LARGE_INTEGER mStartTime;
		LARGE_INTEGER mFrequency;

		DWORD_PTR mTimerMask;
	public:
		/** Timer constructor.  MUST be called on same thread that calls getMilliseconds() */
		Timer();
		~Timer();

		/** Method for setting a specific option of the Timer. These options are usually
			specific for a certain implementation of the Timer class, and may (and probably
			will) not exist across different implementations.  reset() must be called after
			all setOption() calls.
			@par
			Current options supported are:
			<ul><li>"QueryAffinityMask" (TU32): Set the thread affinity mask to be used
			to check the timer. If 'reset' has been called already this mask should
			overlap with the process mask that was in force at that point, and should
			be a power of two (a single core).</li></ul>
			@param
				strKey The name of the option to set
			@param
				pValue A pointer to the value - the size should be calculated by the timer
				based on the key
			@return
				On success, TTRUE is returned.
			@par
				On failure, TFALSE is returned.
		*/
		TBOOL setOption( const String& strKey, const TVOID* pValue );

		/** Resets timer */
		TVOID reset();

		/** Returns milliseconds since initialisation or last reset */
		TU32 getMilliseconds();

		/** Returns microseconds since initialisation or last reset */
		TU32 getMicroseconds();

		/** Returns milliseconds since initialisation or last reset, only CPU time measured */
		TU32 getMillisecondsCPU();

		/** Returns microseconds since initialisation or last reset, only CPU time measured */
		TU32 getMicrosecondsCPU();
	};
}

#endif