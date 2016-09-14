#pragma once
#include "TMutex.h"

namespace TLunaEngine{
	class TLock
	{
	public:

		TLock(void)
		{
		}

		~TLock(void)
		{
			UnlockMutex();
		}

	private:
		TMutex mMutex;
	public:
		bool LockMutex(const TMutex& mutex)
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