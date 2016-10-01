#ifndef _TLPAIR_H_
#define _TLPAIR_H_

#include "TLCommonTypeDef.h"
namespace TLunaEngine{

	template<typename TK,typename TV>
	class Pair
	{
	public:
		
	public:
		Pair(TVOID)
		{
		};
		~Pair(TVOID)
		{
			clear();
		};
	protected:
		// Key
		TK _key;
		// Value
		TV _value;
	public:
		// 拷贝构造
		Pair(const Pair<TK,TV>& other)
		{
			clone(other);
		}
		// Constructor
		Pair(const TK& key, const TV& value) : _key(key), _value(value)
		{

		}

		// ----------- 成员 --------------------------
		// 克隆
		inline TVOID clone(const Pair<TK,TV>& other)
		{
			if(&other == this)
			{
				return;
			}
			clear();
			_key = other._key;
			_value = other._value;
		}

		// 重载=
		inline Pair<TK,TV>& operator=(const Pair<TK,TV>& other)
		{
			clone(other);
			return *this;
		}
		// 清空所有
		inline TVOID clear()
		{
		}
		// Get Key
		inline const TK& key() const
		{
			return _key;
		}
		// Get Key Ref
		inline TK& key()
		{
			return _key;
		}
		// Get Value
		inline const TV& value() const
		{
			return _value;
		}
		// Get Value Ref
		inline TV& value()
		{
			return _value;
		}
		// -------------------------------------------
	};

}

#endif