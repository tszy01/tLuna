#ifndef _TLMUTEX_H_
#define _TLMUTEX_H_
#include <Windows.h>
namespace TLunaEngine{
	class Mutex
	{
	public:

		Mutex(void):mMutex(NULL)
		{
		}

		~Mutex(void)
		{
			CloseMutexHandle();
		}

	private:
		HANDLE mMutex;

	public:
		bool CreateMutexHandle()
		{
			if(!mMutex)
			{
				mMutex = ::CreateMutex(NULL,FALSE,NULL);
				if(!mMutex)
					return false;
				return true;
			}
			return false;
		}

		void CloseMutexHandle()
		{
			if(mMutex)
			{
				::CloseHandle(mMutex);
				mMutex = NULL;
			}
		}

		HANDLE GetMutexHandle() const
		{
			return mMutex;
		}

		bool IsNull() const
		{
			return mMutex==NULL;
		}

		void SetNull()
		{
			mMutex=NULL;
		}

		void SwapMutexHandle(Mutex& right)
		{
			HANDLE rightHandle = right.mMutex;
			right.mMutex = mMutex;
			mMutex = rightHandle;
		}

		void CopyMutexHandle(const Mutex& right)
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