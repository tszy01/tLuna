#pragma once

namespace TLunaEngine{

	template<typename T>
	class TAutoPtr
	{
	public:
		TAutoPtr(void) : m_pPtr(0)
		{
		}

		TAutoPtr(const T* ptr) : m_pPtr(ptr)
		{
		}

		TAutoPtr(const TAutoPtr<T>& other) : m_pPtr(0)
		{
			m_pPtr = other.m_pPtr;
		}

		~TAutoPtr(void)
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
		inline TAutoPtr<T>& operator=(const TAutoPtr<T>& other)
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

		inline TAutoPtr<T>& operator=(const T* ptr)
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

		inline bool IsNullPtr() const
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