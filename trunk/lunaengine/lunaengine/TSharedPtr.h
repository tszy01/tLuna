/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __TSHAREDPTR_H__
#define __TSHAREDPTR_H__

#include "TLock.h"
#include <utility>
#include <assert.h>

namespace TLunaEngine {

	/// The method to use to free memory on destruction
	enum TSharedPtrFreeMethod
	{
		SPFM_DELETE,
		SPFM_DELETE_T,
		SPFM_FREE
	};
	template<class T> class TSharedPtr
	{
	protected:
		T* pRep;
		unsigned int* pUseCount;
		TSharedPtrFreeMethod useFreeMethod;
	public:
		TMutex mMutex;

		TSharedPtr() : pRep(0), pUseCount(0), useFreeMethod(SPFM_DELETE)
        {
			mMutex.SetNull();
        }

        template< class Y>
		explicit TSharedPtr(Y* rep, TSharedPtrFreeMethod freeMethod = SPFM_DELETE) 
			: pRep(rep)
			, pUseCount(rep ? new unsigned int() : 0)
			, useFreeMethod(freeMethod)
		{
			mMutex.SetNull();
			//OGRE_NEW_AUTO_SHARED_MUTEX
			// change ogre 1.7
			// 这里改成如果有指针，也就是如果有usecount，才构造，否则构造出来的mutex就不会被释放了
			if(rep)
			{
				(*pUseCount) = 1;
				mMutex.CreateMutexHandle();
			}
		}
		TSharedPtr(const TSharedPtr& r)
            : pRep(0), pUseCount(0), useFreeMethod(SPFM_DELETE)
		{
			// lock & copy other mutex pointer
            
			mMutex.SetNull();
			if(r.mMutex.IsNull()==false)
            {
				TLock lock;
				lock.LockMutex(r.mMutex);
				mMutex.CopyMutexHandle(r.mMutex);
			    pRep = r.pRep;
			    pUseCount = r.pUseCount; 
				useFreeMethod = r.useFreeMethod;
			    // Handle zero pointer gracefully to manage STL containers
			    if(pUseCount)
			    {
				    ++(*pUseCount); 
			    }
            }
		}
		TSharedPtr& operator=(const TSharedPtr& r) {
			if (pRep == r.pRep)
				return *this;
			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			TSharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}
		
		template< class Y>
		TSharedPtr(const TSharedPtr<Y>& r)
            : pRep(0), pUseCount(0), useFreeMethod(SPFM_DELETE)
		{
			// lock & copy other mutex pointer

			mMutex.SetNull();
			if(r.mMutex.IsNull()==false)
            {
				TLock lock;
				lock.LockMutex(r.mMutex);
				mMutex.CopyMutexHandle(r.mMutex);
			    pRep = r.getPointer();
			    pUseCount = r.useCountPointer();
				useFreeMethod = r.freeMethod();
			    // Handle zero pointer gracefully to manage STL containers
			    if(pUseCount)
			    {
				    ++(*pUseCount);
			    }
            }
		}
		template< class Y>
		TSharedPtr& operator=(const TSharedPtr<Y>& r) {
			if (pRep == r.getPointer())
				return *this;
			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			TSharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}
		virtual ~TSharedPtr() {
            release();
		}


		inline T& operator*() const { assert(pRep); return *pRep; }
		inline T* operator->() const { assert(pRep); return pRep; }
		inline T* get() const { return pRep; }

		/** Binds rep to the TSharedPtr.
			@remarks
				Assumes that the TSharedPtr is uninitialised!
		*/
		void bind(T* rep, TSharedPtrFreeMethod freeMethod = SPFM_DELETE) {
			assert(!pRep && !pUseCount && mMutex.IsNull());
			mMutex.CreateMutexHandle();
			TLock lock;
			lock.LockMutex(mMutex);
			pUseCount = new unsigned int();
			(*pUseCount) = 1;
			pRep = rep;
			useFreeMethod = freeMethod;
		}

		inline bool unique() const 
		{ 
			TLock lock;
			lock.LockMutex(mMutex); 
			assert(pUseCount); 
			return *pUseCount == 1; 
		}
		inline unsigned int useCount() const 
		{
			TLock lock;
			lock.LockMutex(mMutex); 
			assert(pUseCount); 
			return *pUseCount; 
		}
		inline unsigned int* useCountPointer() const { return pUseCount; }

		inline T* getPointer() const { return pRep; }
		inline TSharedPtrFreeMethod freeMethod() const { return useFreeMethod; }

		inline bool isNull(void) const { return pRep == 0; }

        inline void setNull(void) { 
			if (pRep)
			{
				// can't scope lock mutex before release in case deleted
				release();
				pRep = 0;
				pUseCount = 0;
			}
        }

    protected:

        inline void release(void)
        {
			bool destroyThis = false;

            /* If the mutex is not initialized to a non-zero value, then
               neither is pUseCount nor pRep.
             */

            if(mMutex.IsNull()==false)
			{
				// lock own mutex in limited scope (must unlock before destroy)
				TLock lock;
				lock.LockMutex(mMutex); 
				if (pUseCount)
				{
					if (--(*pUseCount) == 0) 
					{
						destroyThis = true;
	                }
				}
            }
			if (destroyThis)
				destroy();

			mMutex.SetNull();
        }

        virtual void destroy(void)
        {
            // IF YOU GET A CRASH HERE, YOU FORGOT TO FREE UP POINTERS
            // BEFORE SHUTTING OGRE DOWN
            // Use setNull() before shutdown or make sure your pointer goes
            // out of scope before OGRE shuts down to avoid this.
			switch(useFreeMethod)
			{
			case SPFM_DELETE:
				delete pRep;
				break;
			case SPFM_DELETE_T:
				delete [] pRep;
				break;
			case SPFM_FREE:
				free(pRep);
				break;
			};
			// use OGRE_FREE instead of OGRE_DELETE_T since 'unsigned int' isn't a destructor
			// we only used OGRE_NEW_T to be able to use constructor
			delete pUseCount;
			mMutex.CloseMutexHandle();
        }

		virtual void swap(TSharedPtr<T> &other) 
		{
			std::swap(pRep, other.pRep);
			std::swap(pUseCount, other.pUseCount);
			std::swap(useFreeMethod, other.useFreeMethod);
			mMutex.SwapMutexHandle(other.mMutex);
		}
	};

	template<class T, class U> inline bool operator==(TSharedPtr<T> const& a, TSharedPtr<U> const& b)
	{
		return a.get() == b.get();
	}

	template<class T, class U> inline bool operator!=(TSharedPtr<T> const& a, TSharedPtr<U> const& b)
	{
		return a.get() != b.get();
	}

	template<class T, class U> inline bool operator<(TSharedPtr<T> const& a, TSharedPtr<U> const& b)
	{
		return std::less<const void*>()(a.get(), b.get());
	}
	/** @} */
	/** @} */
}



#endif
