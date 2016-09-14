#pragma once

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

namespace TLunaEngine{

	template<typename T>
	class TArray
	{
	public:
		TArray(void);
		~TArray(void);
	protected:
		// ------------------- ��Ա -------------------------

		// ָ���һ��Ԫ��
		T* m_pFront;
		// һ���м�����Ч
		int m_nUsed;
		// һ�����ٵĸ���
		int m_nAlloc;
		// T�Ĵ�С
		size_t m_eleSize;
		// ��ǰ���ٵĿռ��С
		size_t m_nowSize;

		// --------------------------------------------------
	public:
		// ------------------ �������� ---------------------

		// ���캯��
		// elementSize:T�Ĵ�С
		// nAlloc:Ԥ�ȿ��ټ���
		// eleInit:��ʼ����ֵ
		inline TArray(size_t elementSize,int nAlloc,T& eleInit)
		{
			m_eleSize = elementSize;
			m_nowSize = m_eleSize * nAlloc;
			m_nAlloc = nAlloc;
			m_pFront = (T*)malloc(m_nowSize);;
			for(int i=0;i<nAlloc;i++)
			{
				T* pSeek = m_pFront + i;
				memcpy(pSeek,&eleInit,m_eleSize);
			}
			m_nUsed = 0;
		};

		// ���캯��,ֻ��ʼ��T�Ĵ�С
		// elementSize:T�Ĵ�С
		inline TArray(size_t elementSize)
		{
			m_eleSize = elementSize;
			m_nowSize = 0;
			m_nAlloc = 0;
			m_nUsed = 0;
			m_pFront = 0;
		}

		// ��������
		inline TArray(TArray& other)
		{
			m_pFront = 0;
			Clone(other);
		}

		// ��¡
		inline void Clone(TArray<T>& other)
		{
			if(m_pFront)
			{
				free(m_pFront);
			}
			int origAlloc = other.GetLength();
			int eleSize = other.GetElementSize();
			m_eleSize = eleSize;
			m_nowSize = m_eleSize * origAlloc;
			m_nAlloc = origAlloc;
			m_pFront = (T*)malloc(m_nowSize);
			memcpy(m_pFront,other.GetBuffer(),m_nowSize);
			m_nUsed = origAlloc;
		}

		// ��д��ֵ����
		inline TArray<T>& operator=(TArray<T>& other)
		{
			Clone(other);
			return *this;
		}

		// �����鹹��
		inline TArray(T* pArray,size_t elementSize,int arrayLen)
		{
			int origAlloc = arrayLen;
			int eleSize = elementSize;
			m_eleSize = eleSize;
			m_nowSize = m_eleSize * origAlloc;
			m_nAlloc = origAlloc;
			m_pFront = (T*)malloc(m_nowSize);
			memcpy(m_pFront,pArray,m_nowSize);
			m_nUsed = origAlloc;
		}

		// �ط���ռ�
		// �����ԭ���٣�ֻ����һ��������
		// �����ԭ���࣬������������
		// nAlloc:�µĿռ��м���Ԫ��
		// eleInit:��ʼ����ֵ
		inline bool ReAlloc(int nAlloc,T& eleInit)
		{
			if(m_eleSize==0 || nAlloc<=0)
				return false;
			// �ȷ���
			size_t destSize = m_eleSize*nAlloc;
			T* pNewMem = (T*)malloc(destSize);
			// ���ڴ渳ֵĬ�Ϲ����T
			for(int i=0;i<nAlloc;i++)
			{
				T* pSeek = pNewMem + i;
				memcpy(pSeek,&eleInit,m_eleSize);
			}
			// �õ���
			if(m_nUsed <= nAlloc)
			{
				// ��ʹ�õĸ��ƽ�ȥ
				if(m_nUsed > 0 && m_pFront)
				{
					memcpy(pNewMem,m_pFront,m_nUsed*m_eleSize);
				}
			}
			// �õĶ�
			else
			{
				// ����nAlloc��
				if(m_pFront)
				{
					memcpy(pNewMem,m_pFront,nAlloc*m_eleSize);
				}
				// ʹ����nAlloc��
				m_nUsed = nAlloc;
			}
			// ��Ա��ֵ������ԭ����
			if(m_pFront)
			{
				free(m_pFront);
			}
			m_pFront = pNewMem;
			m_nowSize = destSize;
			m_nAlloc = nAlloc;
			return true;
		};

		// �õ�Ԫ�صķ���
		inline T& GetElement(int iPos)
		{
			T* pSeek = m_pFront + iPos;
			return *pSeek;
		}

		// ����Ԫ�صķ���
		inline void SetElement(int iPos,T& ele)
		{
			if(iPos<0)
				return;
			if(iPos>=m_nAlloc)
			{
				ReAlloc(iPos+1,ele);
			}
			T* pSeek = m_pFront + iPos;
			memcpy(pSeek,&ele,m_eleSize);
			if(iPos>=m_nUsed)
			{
				m_nUsed = iPos+1;
			}
		};

		// ����[]
		inline T& operator[](int iPos)
		{
			if(iPos<0)
				return (*m_pFront);
			T value;
			if(iPos>=m_nAlloc)
			{
				ReAlloc(iPos+1,value);
			}
			if(iPos>=m_nUsed)
			{
				m_nUsed = iPos+1;
			}
			T* pSeek = m_pFront + iPos;
			return (*pSeek);
		}

		// �õ�����
		inline int GetLength()
		{
			return m_nUsed;
		};
		
		// �õ����ڿ��ٵĳ���
		inline int GetAlloc()
		{
			return m_nAlloc;
		};

		// �õ�Ԫ�ش�С
		inline size_t GetElementSize()
		{
			return m_eleSize;
		};

		// �õ���Ӧ���ڴ�
		inline T* GetBuffer()
		{
			return m_pFront;
		};

		// �õ���ͨ����
		inline int GetBuffer(T** ppBuffer)
		{
			if(!ppBuffer)
				return -1;
			if(m_nUsed <= 0 || !m_pFront)
				return -1;
			(*ppBuffer) = new T[m_nUsed];
			for(int i=0;i<m_nUsed;i++)
				(*ppBuffer)[i] = GetElement(i);
			return m_nUsed;
		}

		// �������
		inline void Clear()
		{
			if(m_pFront)
			{
				free(m_pFront);
				m_pFront=0;
			}
			m_nowSize = 0;
			m_nUsed = 0;
			m_nAlloc = 0;
		};

		// �ռ��Ƿ��Ѿ���
		inline bool IsFull()
		{
			return m_nUsed == m_nAlloc;
		}

		// ��ĩβ���һ��Ԫ��
		inline void Push_Back(T& ele)
		{
			// ����ռ䲻���ȿ��ٿռ�
			if(IsFull())
			{
				ReAlloc(m_nAlloc+1,ele);
			}
			T* pSeek = m_pFront + m_nUsed;
			memcpy(pSeek,&ele,m_eleSize);
			m_nUsed++;
		};
		// -------------------------------------------------
	};

	template<typename T>
	TArray<T>::TArray(void)
	{
		m_eleSize = 0;
		m_nowSize = 0;
		m_pFront = 0;
		m_nUsed = 0;
		m_nAlloc = 0;
	}

	template<typename T>
	TArray<T>::~TArray(void)
	{
		if(m_pFront)
		{
			free(m_pFront);
			m_pFront = 0;
		}
	}


}