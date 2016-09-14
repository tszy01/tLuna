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
		// ------------------- 成员 -------------------------

		// 指向第一个元素
		T* m_pFront;
		// 一共有几个有效
		int m_nUsed;
		// 一共开辟的个数
		int m_nAlloc;
		// T的大小
		size_t m_eleSize;
		// 当前开辟的空间大小
		size_t m_nowSize;

		// --------------------------------------------------
	public:
		// ------------------ 公共函数 ---------------------

		// 构造函数
		// elementSize:T的大小
		// nAlloc:预先开辟几个
		// eleInit:初始化的值
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

		// 构造函数,只初始化T的大小
		// elementSize:T的大小
		inline TArray(size_t elementSize)
		{
			m_eleSize = elementSize;
			m_nowSize = 0;
			m_nAlloc = 0;
			m_nUsed = 0;
			m_pFront = 0;
		}

		// 拷贝构造
		inline TArray(TArray& other)
		{
			m_pFront = 0;
			Clone(other);
		}

		// 克隆
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

		// 重写赋值操作
		inline TArray<T>& operator=(TArray<T>& other)
		{
			Clone(other);
			return *this;
		}

		// 由数组构造
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

		// 重分配空间
		// 如果比原来少，只复制一部分内容
		// 如果比原来多，复制所有内容
		// nAlloc:新的空间有几个元素
		// eleInit:初始化的值
		inline bool ReAlloc(int nAlloc,T& eleInit)
		{
			if(m_eleSize==0 || nAlloc<=0)
				return false;
			// 先分配
			size_t destSize = m_eleSize*nAlloc;
			T* pNewMem = (T*)malloc(destSize);
			// 对内存赋值默认构造的T
			for(int i=0;i<nAlloc;i++)
			{
				T* pSeek = pNewMem + i;
				memcpy(pSeek,&eleInit,m_eleSize);
			}
			// 用的少
			if(m_nUsed <= nAlloc)
			{
				// 把使用的复制进去
				if(m_nUsed > 0 && m_pFront)
				{
					memcpy(pNewMem,m_pFront,m_nUsed*m_eleSize);
				}
			}
			// 用的多
			else
			{
				// 复制nAlloc个
				if(m_pFront)
				{
					memcpy(pNewMem,m_pFront,nAlloc*m_eleSize);
				}
				// 使用了nAlloc个
				m_nUsed = nAlloc;
			}
			// 成员赋值，销毁原来的
			if(m_pFront)
			{
				free(m_pFront);
			}
			m_pFront = pNewMem;
			m_nowSize = destSize;
			m_nAlloc = nAlloc;
			return true;
		};

		// 得到元素的方法
		inline T& GetElement(int iPos)
		{
			T* pSeek = m_pFront + iPos;
			return *pSeek;
		}

		// 设置元素的方法
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

		// 重载[]
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

		// 得到长度
		inline int GetLength()
		{
			return m_nUsed;
		};
		
		// 得到现在开辟的长度
		inline int GetAlloc()
		{
			return m_nAlloc;
		};

		// 得到元素大小
		inline size_t GetElementSize()
		{
			return m_eleSize;
		};

		// 得到对应的内存
		inline T* GetBuffer()
		{
			return m_pFront;
		};

		// 得到普通数组
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

		// 清空数组
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

		// 空间是否已经满
		inline bool IsFull()
		{
			return m_nUsed == m_nAlloc;
		}

		// 从末尾添加一个元素
		inline void Push_Back(T& ele)
		{
			// 如果空间不够先开辟空间
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