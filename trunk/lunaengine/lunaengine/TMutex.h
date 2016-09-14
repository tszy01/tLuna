#pragma once
#include <Windows.h>
namespace TLunaEngine{
	class TMutex
	{
	public:

		TMutex(void):mMutex(NULL)
		{
		}

		~TMutex(void)
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

		void SwapMutexHandle(TMutex& right)
		{
			HANDLE rightHandle = right.mMutex;
			right.mMutex = mMutex;
			mMutex = rightHandle;
		}

		void CopyMutexHandle(const TMutex& right)
		{
			mMutex = right.mMutex;
		}

		TMutex& operator =(const TMutex& right)
		{
			mMutex = right.mMutex;
			return *this;
		}
	};

}