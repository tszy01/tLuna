#ifndef _TLAUTOPTR_H_
#define _TLAUTOPTR_H_

namespace TLunaEngine{

	template<typename T>
	class AutoPtr
	{
	public:
		AutoPtr(TVOID) : m_pPtr(0)
		{
		}

		AutoPtr(const T* ptr) : m_pPtr(ptr)
		{
		}

		AutoPtr(const AutoPtr<T>& other) : m_pPtr(0)
		{
			m_pPtr = other.m_pPtr;
		}

		~AutoPtr(TVOID)
		{
			if(m_pPtr)
			{
				delete m_pPtr;
				m_pPtr = 0;
			}
		}
	protected:
		T* m_pPtr;
	public:
		inline AutoPtr<T>& operator=(const AutoPtr<T>& other)
		{
			if(other.GetPtr() == m_pPtr)
				return *this;

			if(m_pPtr)
			{
				delete m_pPtr;
				m_pPtr = 0;
			}
			m_pPtr = other.m_pPtr;
		}

		inline AutoPtr<T>& operator=(const T* ptr)
		{
			if(ptr == m_pPtr)
				return *this;

			if(m_pPtr)
			{
				delete m_pPtr;
				m_pPtr = 0;
			}
			m_pPtr = ptr;
		}

		inline TBOOL IsNullPtr() const
		{
			return m_pPtr==0;
		}

		inline T* GetPtr() const
		{
			return m_pPtr;
		}

		inline T* operator->() const
		{
			return m_pPtr;
		}

		inline T& operator*() const
		{
			return *m_pPtr;
		}
	};
}

#endif