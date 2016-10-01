#ifndef _TLARRAY_H_
#define _TLARRAY_H_

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include "TLCommonTypeDef.h"

namespace TLunaEngine{

	template<typename T>
	class Array
	{
	public:
		Array(TVOID);
		~Array(TVOID);
	protected:
		// ------------------- 成员 -------------------------

		// 指向第一个元素
		T* m_pFront;
		// 一共有几个有效
		TS32 m_nUsed;
		// 一共开辟的个数
		TS32 m_nAlloc;
		// T的大小
		TSIZE m_eleSize;
		// 当前开辟的空间大小
		TSIZE m_nowSize;

		// --------------------------------------------------
	public:
		// ------------------ 公共函数 ---------------------

		// 构造函数
		// elementSize:T的大小
		// nAlloc:预先开辟几个
		// eleInit:初始化的值
		inline Array(TSIZE elementSize,TS32 nAlloc,T& eleInit)
		{
			m_eleSize = elementSize;
			m_nowSize = m_eleSize * nAlloc;
			m_nAlloc = nAlloc;
			m_pFront = (T*)malloc(m_nowSize);
			for(TS32 i=0;i<nAlloc;i++)
			{
				T* pSeek = m_pFront + i;
				memcpy(pSeek,&eleInit,m_eleSize);
			}
			m_nUsed = 0;
		};

		// 构造函数,只初始化T的大小
		// elementSize:T的大小
		inline Array(TSIZE elementSize)
		{
			m_eleSize = elementSize;
			m_nowSize = 0;
			m_nAlloc = 0;
			m_nUsed = 0;
			m_pFront = 0;
		}

		// 拷贝构造
		inline Array(Array& other)
		{
			m_pFront = 0;
			Clone(other);
		}

		// 克隆
		inline TVOID Clone(Array<T>& other)
		{
			if(m_pFront)
			{
				free(m_pFront);
			}
			TS32 origAlloc = other.GetLength();
			TS32 eleSize = other.GetElementSize();
			m_eleSize = eleSize;
			m_nowSize = m_eleSize * origAlloc;
			m_nAlloc = origAlloc;
			m_pFront = (T*)malloc(m_nowSize);
			memcpy(m_pFront,other.GetBuffer(),m_nowSize);
			m_nUsed = origAlloc;
		}

		// 重写赋值操作
		inline Array<T>& operator=(Array<T>& other)
		{
			Clone(other);
			return *this;
		}

		// 由数组构造
		inline Array(T* pArray, TSIZE elementSize,TS32 arrayLen)
		{
			TS32 origAlloc = arrayLen;
			TS32 eleSize = elementSize;
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
		inline TBOOL ReAlloc(TS32 nAlloc,T& eleInit)
		{
			if(m_eleSize==0 || nAlloc<=0)
				return TFALSE;
			// 先分配
			TSIZE destSize = m_eleSize*nAlloc;
			T* pNewMem = (T*)malloc(destSize);
			// 对内存赋值默认构造的T
			for(TS32 i=0;i<nAlloc;i++)
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
			return TTRUE;
		};

		// 得到元素的方法
		inline T& GetElement(TS32 iPos)
		{
			T* pSeek = m_pFront + iPos;
			return *pSeek;
		}

		// 设置元素的方法
		inline TVOID SetElement(TS32 iPos,T& ele)
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
		inline T& operator[](TS32 iPos)
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
		inline TS32 GetLength()
		{
			return m_nUsed;
		};
		
		// 得到现在开辟的长度
		inline TS32 GetAlloc()
		{
			return m_nAlloc;
		};

		// 得到元素大小
		inline TSIZE GetElementSize()
		{
			return m_eleSize;
		};

		// 得到对应的内存
		inline T* GetBuffer()
		{
			return m_pFront;
		};

		// 得到普通数组
		inline TS32 GetBuffer(T** ppBuffer)
		{
			if(!ppBuffer)
				return -1;
			if(m_nUsed <= 0 || !m_pFront)
				return -1;
			(*ppBuffer) = new T[m_nUsed];
			for(TS32 i=0;i<m_nUsed;i++)
				(*ppBuffer)[i] = GetElement(i);
			return m_nUsed;
		}

		// 清空数组
		inline TVOID Clear()
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
		inline TBOOL IsFull()
		{
			return m_nUsed == m_nAlloc;
		}

		// 从末尾添加一个元素
		inline TVOID Push_Back(T& ele)
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
	Array<T>::Array(TVOID)
	{
		m_eleSize = 0;
		m_nowSize = 0;
		m_pFront = 0;
		m_nUsed = 0;
		m_nAlloc = 0;
	}

	template<typename T>
	Array<T>::~Array(TVOID)
	{
		if(m_pFront)
		{
			free(m_pFront);
			m_pFront = 0;
		}
	}


}

#endif