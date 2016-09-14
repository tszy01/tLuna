#pragma once

namespace TLunaEngine{

	template<typename TK,typename TV>
	class THashtable
	{
	public:
		// ----------------- 节点 -----------------------
		struct _T_HASHTABLE_NODE
		{
			TK Key;
			TV Value;
			_T_HASHTABLE_NODE* Next;
			_T_HASHTABLE_NODE* Prev;
			_T_HASHTABLE_NODE() : Next(0),Prev(0){}
		};
		// ----------------------------------------------
		// ---------------- 迭代器 ----------------------
		class Iterator
		{
		public:
			Iterator() : Current(0) {}
			Iterator& operator ++()    { Current = Current->Next; return *this; }
			Iterator& operator --()    { Current = Current->Prev; return *this; }
			Iterator  operator ++(int) { Iterator tmp = *this; Current = Current->Next; return tmp; }
			Iterator  operator --(int) { Iterator tmp = *this; Current = Current->Prev; return tmp; }
			bool operator ==(const Iterator&      other) const { return Current == other.Current; }
			bool operator !=(const Iterator&      other) const { return Current != other.Current; }
			_T_HASHTABLE_NODE & operator * () { return *Current; }
			_T_HASHTABLE_NODE * operator ->() { return Current; }
		private:
			Iterator(_T_HASHTABLE_NODE* begin) : Current(begin) {}
			_T_HASHTABLE_NODE* Current;
			friend class THashtable<TK,TV>;
		};
		// ----------------------------------------------
	public:
		THashtable(void)
		{
			m_First = 0;
			m_Last = 0;
			m_Size = 0;
		};
		~THashtable(void)
		{
			clear();
		};
	protected:
		// 起始指针
		_T_HASHTABLE_NODE* m_First;
		// 末尾指针
		_T_HASHTABLE_NODE* m_Last;
		// 大小
		int m_Size;
	public:
		// 拷贝构造
		THashtable(const THashtable<TK,TV>& other) : m_First(0), m_Last(0), m_Size(0)
		{
			Clone(other);
		}

		// ----------- 成员 --------------------------
		// 克隆
		inline void Clone(const THashtable<TK,TV>& other)
		{
			if(&other == this)
			{
				return;
			}
			clear();
			_T_HASHTABLE_NODE* node = other.m_First;
			while(node)
			{
				push_back(node->Key,node->Value);
				node = node->Next;
			}
		}

		// 重载=
		inline THashtable<TK,TV>& operator=(const THashtable<TK,TV>& other)
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
		inline int size()
		{
			return m_Size;
		}
		// 清空所有
		inline void clear()
		{
			while(m_First)
			{
				_T_HASHTABLE_NODE * next = m_First->Next;
				delete m_First;
				m_First = next;
			}
			m_Last = 0;
			m_Size = 0;
		}
		// 现在是否空
		inline bool empty()
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
		inline void push_back(TK key,TV& value)
		{
			_T_HASHTABLE_NODE* node = new _T_HASHTABLE_NODE;
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
		inline bool find(TK key)
		{
			Iterator itr = begin();
			for(;itr!=end();++itr)
			{
				if(itr->Key == key)
				{
					return true;
				}
			}
			return false;
		}
		// 删除指定key
		inline void remove(TK key)
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
		inline bool GetValue(TK key,TV& value)
		{
			Iterator itr = begin();
			for(;itr!=end();++itr)
			{
				if(itr->Key == key)
				{
					value = itr->Value;
					return true;
				}
			}
			return false;
		}
		// 设置值
		inline void SetValue(TK key,TV& value)
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