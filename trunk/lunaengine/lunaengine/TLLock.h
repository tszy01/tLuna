#ifndef _TLLOCK_H_
#define _TLLOCK_H_
#include "TLMutex.h"

namespace TLunaEngine{
	class Lock
	{
	public:

		Lock(void)
		{
		}

		~Lock(void)
		{
			UnlockMutex();
		}

	private:
		Mutex mMutex;
	public:
		bool LockMutex(const Mutex& mutex)
		{
			if(mMutex.IsNull()==false)
				return false;
			DWORD dwWaitResult = ::WaitForSingleObject( 
				mutex.GetMutexHandle(),    // handle to mutex
				INFINITE);  // no time-out interval
 
			switch (dwWaitResult) 
			{
				// The thread got ownership of the mutex
				case WAIT_OBJECT_0:
					{
						mMutex = mutex;
					}
					break;
				default:
					return false;
			}
			return true;
		}

		void UnlockMutex()
		{
			if(mMutex.IsNull()==false)
			{
				::ReleaseMutex(mMutex.GetMutexHandle());
				mMutex.SetNull();
			}
		}
	};

}

#endif