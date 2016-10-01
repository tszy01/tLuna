#ifndef _TLHASHTABLE_H_
#define _TLHASHTABLE_H_

#include "TLCommonTypeDef.h"
namespace TLunaEngine{

	template<typename TK,typename TV>
	class Hashtable
	{
	public:
		// ----------------- 节点 -----------------------
		struct HASHTABLE_NODE
		{
			TK Key;
			TV Value;
			HASHTABLE_NODE* Next;
			HASHTABLE_NODE* Prev;
			HASHTABLE_NODE() : Next(0),Prev(0){}
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
			HASHTABLE_NODE & operator * () { return *Current; }
			HASHTABLE_NODE * operator ->() { return Current; }
		private:
			Iterator(HASHTABLE_NODE* begin) : Current(begin) {}
			HASHTABLE_NODE* Current;
			friend class Hashtable<TK,TV>;
		};
		// ----------------------------------------------
	public:
		Hashtable(TVOID)
		{
			m_First = 0;
			m_Last = 0;
			m_Size = 0;
		};
		~Hashtable(TVOID)
		{
			clear();
		};
	protected:
		// 起始指针
		HASHTABLE_NODE* m_First;
		// 末尾指针
		HASHTABLE_NODE* m_Last;
		// 大小
		TS32 m_Size;
	public:
		// 拷贝构造
		Hashtable(const Hashtable<TK,TV>& other) : m_First(0), m_Last(0), m_Size(0)
		{
			Clone(other);
		}

		// ----------- 成员 --------------------------
		// 克隆
		inline TVOID Clone(const Hashtable<TK,TV>& other)
		{
			if(&other == this)
			{
				return;
			}
			clear();
			HASHTABLE_NODE* node = other.m_First;
			while(node)
			{
				push_back(node->Key,node->Value);
				node = node->Next;
			}
		}

		// 重载=
		inline Hashtable<TK,TV>& operator=(const Hashtable<TK,TV>& other)
		{
			Clone(other);
			return *this;
		}
		// 返回m_First指针
		inline Iterator begin()
		{
			return Iterator(m_First);
		}
		// 返回0指针
		inline Iterator end()
		{
			return Iterator(0);
		}
		// 返回last指针
		inline Iterator last()
		{
			return Iterator(m_Last);
		}
		// 返回大小
		inline TS32 size()
		{
			return m_Size;
		}
		// 清空所有
		inline TVOID clear()
		{
			while(m_First)
			{
				HASHTABLE_NODE * next = m_First->Next;
				delete m_First;
				m_First = next;
			}
			m_Last = 0;
			m_Size = 0;
		}
		// 现在是否空
		inline TBOOL empty()
		{
			return (m_First == 0);
		}
		// 根据迭代器删除
		inline Iterator erase(Iterator& itr)
		{
			Iterator returnIterator(itr);
			++returnIterator;
			if(itr.Current == m_First)
			{
				m_First = itr.Current->Next;
			}
			else
			{
				itr.Current->Prev->Next = itr.Current->Next;
			}
			if(itr.Current == m_Last)
			{
				m_Last = itr.Current->Prev;
			}
			else
			{
				itr.Current->Next->Prev = itr.Current->Prev;
			}
			delete itr.Current;
			itr.Current = 0;
			--m_Size;
			return returnIterator;
		}
		// 从后面插入一个
		inline TVOID push_back(TK key,TV& value)
		{
			HASHTABLE_NODE* node = new HASHTABLE_NODE;
			node->Key = key;
			node->Value = value;
			++m_Size;
			if (m_First == 0)
				m_First = node;
			node->Prev = m_Last;
			if (m_Last != 0)
				m_Last->Next = node;
			m_Last = node;
		}
		// 是否有该key
		inline TBOOL find(TK key)
		{
			Iterator itr = begin();
			for(;itr!=end();++itr)
			{
				if(itr->Key == key)
				{
					return TTRUE;
				}
			}
			return TFALSE;
		}
		// 删除指定key
		inline TVOID remove(TK key)
		{
			Iterator itr = begin();
			for(;itr!=end();++itr)
			{
				if(itr->Key == key)
				{
					erase(itr);
					return;
				}
			}
		}
		// 得到值
		inline TBOOL GetValue(TK key,TV& value)
		{
			Iterator itr = begin();
			for(;itr!=end();++itr)
			{
				if(itr->Key == key)
				{
					value = itr->Value;
					return TTRUE;
				}
			}
			return TFALSE;
		}
		// 设置值
		inline TVOID SetValue(TK key,TV& value)
		{
			Iterator itr = begin();
			for(;itr!=end();++itr)
			{
				if(itr->Key == key)
				{
					itr->Value = value;
					return;
				}
			}
			push_back(key,value);
		}
		// 重载[]
		inline TV& operator[](TK key)
		{
			Iterator itr = begin();
			for(;itr!=end();++itr)
			{
				if(itr->Key == key)
				{
					return itr->Value;
				}
			}
			TV value;
			push_back(key,value);
			return m_Last->Value;
		}
		// -------------------------------------------
	};

}

#endif