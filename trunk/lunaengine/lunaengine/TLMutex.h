#ifndef _TLMUTEX_H_
#define _TLMUTEX_H_
#include <Windows.h>
#include "TLCommonTypeDef.h"
namespace TLunaEngine{
	class Mutex
	{
	public:

		Mutex(TVOID):mMutex(TNULL)
		{
		}

		~Mutex(TVOID)
		{
			CloseMutexHandle();
		}

	private:
		HANDLE mMutex;

	public:
		TBOOL CreateMutexHandle()
		{
			if(!mMutex)
			{
				mMutex = ::CreateMutex(TNULL,FALSE, TNULL);
				if(!mMutex)
					return TFALSE;
				return TTRUE;
			}
			return TFALSE;
		}

		TVOID CloseMutexHandle()
		{
			if(mMutex)
			{
				::CloseHandle(mMutex);
				mMutex = TNULL;
			}
		}

		HANDLE GetMutexHandle() const
		{
			return mMutex;
		}

		TBOOL IsNull() const
		{
			return mMutex == TNULL;
		}

		TVOID SetNull()
		{
			mMutex = TNULL;
		}

		TVOID SwapMutexHandle(Mutex& right)
		{
			HANDLE rightHandle = right.mMutex;
			right.mMutex = mMutex;
			mMutex = rightHandle;
		}

		TVOID CopyMutexHandle(const Mutex& right)
		{
			mMutex = right.mMutex;
		}

		Mutex& operator =(const Mutex& right)
		{
			mMutex = right.mMutex;
			return *this;
		}
	};

}

#endif