#ifndef _TLLIST_H_
#define _TLLIST_H_

namespace TLunaEngine{

	template<typename T>
	class List
	{
	public:
		// ----------------- �ڵ� -----------------------
		struct LIST_NODE
		{
			T Value;
			LIST_NODE* Next;
			LIST_NODE* Prev;
			LIST_NODE() : Next(0),Prev(0){}
		};
		// ----------------------------------------------
		// ---------------- ������ ----------------------
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
			T & operator * () { return Current->Value; }
			T * operator ->() { return &Current->Value; }
		private:
			Iterator(LIST_NODE* begin) : Current(begin) {}
			LIST_NODE* Current;
			friend class List<T>;
		};
		// ----------------------------------------------
	public:
		List(void)
		{
			m_First = 0;
			m_Last = 0;
			m_Size = 0;
		};
		~List(void)
		{
			clear();
		};
	protected:
		// ��ʼָ��
		LIST_NODE* m_First;
		// ĩβָ��
		LIST_NODE* m_Last;
		// ��С
		int m_Size;
	public:
		// ��������
		List(const List<T>& other) : m_First(0), m_Last(0), m_Size(0)
		{
			Clone(other);
		}
		// -------------- ��Ա --------------------------

		// ��¡
		inline void Clone(const List<T>& other)
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

		// ����=
		inline List<T>& operator=(const List<T>& other)
		{
			Clone(other);
			return *this;
		}
		// �õ���С
		inline int size() const
		{
			return m_Size;
		}
		// ���
		inline void clear()
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
		// �Ƿ�Ϊ������
		inline bool empty() const
		{
			return (m_First == 0);
		}
		// �Ӻ󷽲���
		inline void push_back(const T& value)
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
		// ��ǰ�����
		inline void push_front(const T& value)
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
		// ����m_Firstָ��
		inline Iterator begin()
		{
			return Iterator(m_First);
		}
		// ����0
		inline Iterator end()
		{
			return Iterator(0);
		}
		// ����lastָ��
		inline Iterator last()
		{
			return Iterator(m_Last);
		}
		// ��ĳ�ڵ�֮�����
		inline void insert_after(const Iterator& it, const T& value)
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
		// ��ĳ�ڵ�֮ǰ����
		inline void insert_before(const Iterator& it, const T& value)
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
		// ɾ��ĳ���ڵ�
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
		// ����[]
		inline T& operator[](int iPos)
		{
			Iterator itr = begin();
			int count = 0;
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
		// �õ�ֵ
		inline bool GetValue(int iPos,T& value)
		{
			Iterator itr = begin();
			int count = 0;
			for(;itr!=end();++itr)
			{
				if(count == iPos)
				{
					value = (*itr);
					return true;
				}
				count++;
			}
			return false;
		}

		// ----------------------------------------------
	};

}

#endif