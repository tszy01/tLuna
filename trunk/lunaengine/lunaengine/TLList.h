#ifndef _TLLIST_H_
#define _TLLIST_H_

#include "TLCommonTypeDef.h"
namespace TLunaEngine{

	template<typename T>
	class List
	{
	public:
		// ----------------- 节点 -----------------------
		struct LIST_NODE
		{
			T Value;
			LIST_NODE* Next;
			LIST_NODE* Prev;
			LIST_NODE() : Next(0),Prev(0){}
		};
		// ----------------------------------------------
		// ---------------- 迭代器 ----------------------
		class Iterator
		{
		public:
			Iterator() : Current(0) {}
			Iterator& operator ++()    { Current = Current->Next; return *this; }
			Iterator& operator --()    { Current = Current->Prev; return *this; }
			Iterator  operator ++(TS32) { Iterator tmp = *this; Current = Current->Next; return tmp; }
			Iterator  operator --(TS32) { Iterator tmp = *this; Current = Current->Prev; return tmp; }
			TBOOL operator ==(const Iterator&      other) const { return Current == other.Current; }
			TBOOL operator !=(const Iterator&      other) const { return Current != other.Current; }
			T & operator * () { return Current->Value; }
			T * operator ->() { return &Current->Value; }
			Iterator& advance(TU32 n)
			{
				for (TU32 i = 0;i < n;++i)
				{
					if (Current)
					{
						Current = Current->Next;
					}
				}
				return *this;
			}
		private:
			Iterator(LIST_NODE* begin) : Current(begin) {}
			LIST_NODE* Current;
			friend class List<T>;
		};
		// ----------------------------------------------
	public:
		List(TVOID)
		{
			m_First = 0;
			m_Last = 0;
			m_Size = 0;
		};
		~List(TVOID)
		{
			clear();
		};
	protected:
		// 起始指针
		LIST_NODE* m_First;
		// 末尾指针
		LIST_NODE* m_Last;
		// 大小
		TU64 m_Size;
	public:
		// 拷贝构造
		List(const List<T>& other) : m_First(0), m_Last(0), m_Size(0)
		{
			clone(other);
		}
		// -------------- 成员 --------------------------

		// 克隆
		inline TVOID clone(const List<T>& other)
		{
			if(&other == this)
			{
				return;
			}
			clear();
			LIST_NODE* node = other.m_First;
			while(node)
			{
				push_back(node->Value);
				node = node->Next;
			}
		}

		// 重载=
		inline List<T>& operator=(const List<T>& other)
		{
			clone(other);
			return *this;
		}
		// 得到大小
		inline TU64 size() const
		{
			return m_Size;
		}
		// 清空
		inline TVOID clear()
		{
			while(m_First)
			{
				LIST_NODE * next = m_First->Next;
				delete m_First;
				m_First = next;
			}
			m_Last = 0;
			m_Size = 0;
		}
		// 是否为空链表
		inline TBOOL empty() const
		{
			return (m_First == 0);
		}
		// 从后方插入
		inline TVOID push_back(const T& value)
		{
			LIST_NODE* node = new LIST_NODE;
			node->Value = value;
			++m_Size;
			if (m_First == 0)
				m_First = node;
			node->Prev = m_Last;
			if (m_Last != 0)
				m_Last->Next = node;

			m_Last = node;
		}
		// 从前面插入
		inline TVOID push_front(const T& value)
		{
			LIST_NODE* node = new LIST_NODE;
			node->Value = value;
			++m_Size;
			if (m_First == 0)
			{
				m_Last = node;
				m_First = node;
			}
			else
			{
				node->Next = m_First;
				m_First->Prev = node;
				m_First = node;
			}
		}
		// 返回m_First指针
		inline Iterator begin()
		{
			return Iterator(m_First);
		}
		// 返回0
		inline Iterator end()
		{
			return Iterator(0);
		}
		// 返回last指针
		inline Iterator last()
		{
			return Iterator(m_Last);
		}
		// 在某节点之后插入
		inline TVOID insert_after(const Iterator& it, const T& value)
		{
			LIST_NODE* node = new LIST_NODE;
			node->Value = value;
			node->Next = it.Current->Next;
			if (it.Current->Next)
				it.Current->Next->Prev = node;
			node->Prev = it.Current;
			it.Current->Next = node;
			++m_Size;
			if (it.Current == m_Last)
				m_Last = node;
		}
		// 在某节点之前插入
		inline TVOID insert_before(const Iterator& it, const T& value)
		{
			LIST_NODE* node = new LIST_NODE;
			node->Value = value;
			node->Prev = it.Current->Prev;
			if (it.Current->Prev)
				it.Current->Prev->Next = node;
			node->Next = it.Current;
			it.Current->Prev = node;
			++m_Size;
			if (it.Current == m_First)
				m_First = node;
		}
		// 删除某个节点
		inline Iterator erase(Iterator& it)
		{
			Iterator returnIterator(it);
			++returnIterator;
			if(it.Current == m_First)
			{
				m_First = it.Current->Next;
			}
			else
			{
				it.Current->Prev->Next = it.Current->Next;
			}
			if(it.Current == m_Last)
			{
				m_Last = it.Current->Prev;
			}
			else
			{
				it.Current->Next->Prev = it.Current->Prev;
			}
			delete it.Current;
			it.Current = 0;
			--m_Size;
			return returnIterator;
		}
		// 根据值删除
		inline Iterator erase(const T& value)
		{
			Iterator itr = find(value);
			if (itr != end())
			{
				return erase(itr);
			}
			return itr;
		}
		// 是否有该value
		inline TBOOL has(const T& value)
		{
			Iterator itr = begin();
			for (;itr != end();++itr)
			{
				if ((*itr) == value)
				{
					return TTRUE;
				}
			}
			return TFALSE;
		}
		// 是否有该value
		inline Iterator find(const T& value)
		{
			Iterator itr = begin();
			for (;itr != end();++itr)
			{
				if ((*itr) == value)
				{
					return itr;
				}
			}
			return Iterator(0);
		}
		// 重载[]
		inline T& operator[](TU64 iPos)
		{
			Iterator itr = begin();
			TU64 count = 0;
			for(;itr!=end();++itr)
			{
				if(count == iPos)
				{
					return (*itr);
				}
				count++;
			}
			return m_First->Value;
		}
		// 设置值
		inline TVOID set(TU64 iPos, const T& value)
		{
			Iterator itr = begin();
			TU64 count = 0;
			for (;itr != end();++itr)
			{
				if (count == iPos)
				{
					(*itr) == value;
				}
				count++;
			}
		}
		// 得到值
		inline TBOOL get(TU64 iPos,T& value)
		{
			Iterator itr = begin();
			TU64 count = 0;
			for(;itr!=end();++itr)
			{
				if(count == iPos)
				{
					value = (*itr);
					return TTRUE;
				}
				count++;
			}
			return TFALSE;
		}
		// 得到值
		inline T get(TU64 iPos)
		{
			T result;
			get(iPos, result);
			return result;
		}
		// pop front element
		inline TVOID pop_front()
		{
			erase(begin());
		}

		// ----------------------------------------------
	};

}

#endif