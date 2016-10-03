#ifndef _TLMAP_H_
#define _TLMAP_H_

#include "TLCommonTypeDef.h"
#include "TLPair.h"
namespace TLunaEngine{

	template<typename TK,typename TV>
	class Map
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
			Iterator(HASHTABLE_NODE* begin) : Current(begin) {}
			HASHTABLE_NODE* Current;
			friend class Map<TK,TV>;
		};
		// ----------------------------------------------
	public:
		Map(TVOID)
		{
			m_First = 0;
			m_Last = 0;
			m_Size = 0;
		};
		~Map(TVOID)
		{
			clear();
		};
	protected:
		// 起始指针
		HASHTABLE_NODE* m_First;
		// 末尾指针
		HASHTABLE_NODE* m_Last;
		// 大小
		TU64 m_Size;
	public:
		// 拷贝构造
		Map(const Map<TK,TV>& other) : m_First(0), m_Last(0), m_Size(0)
		{
			clone(other);
		}

		// ----------- 成员 --------------------------
		// 克隆
		inline TVOID clone(const Map<TK,TV>& other)
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
		inline Map<TK,TV>& operator=(const Map<TK,TV>& other)
		{
			clone(other);
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
		inline TU64 size()
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
		// 根据值删除
		inline Iterator erase(const TK& key)
		{
			Iterator itr = find(key);
			if (itr != end())
			{
				return erase(itr);
			}
			return itr;
		}
		// 从前面插入
		inline TVOID push_front(const TK& key, const TV& value)
		{
			HASHTABLE_NODE* node = new HASHTABLE_NODE;
			node->Key = key;
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
		// 从前面插入
		inline TVOID push_front(const Pair<TK, TV>& pair)
		{
			HASHTABLE_NODE* node = new HASHTABLE_NODE;
			node->Key = pair.key();
			node->Value = pair.value();
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
		// 从后面插入一个
		inline TVOID push_back(const TK& key, const TV& value)
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
		// 从后面插入一个
		inline TVOID push_back(const Pair<TK,TV>& pair)
		{
			HASHTABLE_NODE* node = new HASHTABLE_NODE;
			node->Key = pair.key();
			node->Value = pair.value();
			++m_Size;
			if (m_First == 0)
				m_First = node;
			node->Prev = m_Last;
			if (m_Last != 0)
				m_Last->Next = node;
			m_Last = node;
		}
		// 在某节点之后插入
		inline TVOID insert_after(const Iterator& it, const TK& key, const TV& value)
		{
			HASHTABLE_NODE* node = new HASHTABLE_NODE;
			node->Key = key;
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
		// 在某节点之后插入
		inline TVOID insert_after(const Iterator& it, const Pair<TK, TV>& pair)
		{
			HASHTABLE_NODE* node = new HASHTABLE_NODE;
			node->Key = pair.key();
			node->Value = pair.value();
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
		inline TVOID insert_before(const Iterator& it, const TK& key, const TV& value)
		{
			HASHTABLE_NODE* node = new HASHTABLE_NODE;
			node->Key = key;
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
		// 在某节点之前插入
		inline TVOID insert_before(const Iterator& it, const Pair<TK, TV>& pair)
		{
			HASHTABLE_NODE* node = new HASHTABLE_NODE;
			node->Key = pair.key();
			node->Value = pair.value();
			node->Prev = it.Current->Prev;
			if (it.Current->Prev)
				it.Current->Prev->Next = node;
			node->Next = it.Current;
			it.Current->Prev = node;
			++m_Size;
			if (it.Current == m_First)
				m_First = node;
		}
		// 是否有该key
		inline TBOOL has(const TK& key)
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
		// 是否有该key
		inline Iterator find(const TK& key)
		{
			Iterator itr = begin();
			for (;itr != end();++itr)
			{
				if (itr->Key == key)
				{
					return itr;
				}
			}
			return Iterator(0);
		}
		// 删除指定key
		inline TVOID remove(const TK& key)
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
		inline TBOOL get(const TK& key,TV& value)
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
		// 得到值
		inline TV get(const TK& key)
		{
			TV result;
			get(key, result);
			return result;
		}
		// 设置值
		inline TVOID set(const TK& key,const TV& value)
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
		inline TV& operator[](const TK& key)
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
		// pop front element
		inline TVOID pop_front()
		{
			erase(begin());
		}
		// -------------------------------------------
	};

}

#endif