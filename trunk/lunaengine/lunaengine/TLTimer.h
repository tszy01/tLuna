#ifndef TLTIMER_H
#define TLTIMER_H

#include "TLCommonTypeDef.h"
#include <Windows.h>
#include <time.h>
#include <string>

namespace TLunaEngine{

	class Timer
	{
	private:
		clock_t mZeroClock;

		TU32 mStartTick;
		LONGLONG mLastTime;
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
				On success, true is returned.
			@par
				On failure, false is returned.
		*/
		bool setOption( const std::string& strKey, const void* pValue );

		/** Resets timer */
		void reset();

		/** Returns milliseconds since initialisation or last reset */
		unsigned long getMilliseconds();

		/** Returns microseconds since initialisation or last reset */
		unsigned long getMicroseconds();

		/** Returns milliseconds since initialisation or last reset, only CPU time measured */
		unsigned long getMillisecondsCPU();

		/** Returns microseconds since initialisation or last reset, only CPU time measured */
		unsigned long getMicrosecondsCPU();
	};
}

#endif