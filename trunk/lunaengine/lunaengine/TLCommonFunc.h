#ifndef _TLCOMMONFUNC_H_
#define _TLCOMMONFUNC_H_

#include "TLCommonTypeDef.h"

namespace TLunaEngine {
	template<typename T> void swap(T& l, T& r)
	{
		T tmp = l;
		l = r;
		r = tmp;
	}

	template<typename T> TBOOL less(const T& l, const T& r)
	{
		if (l < r)
		{
			return TTRUE;
		}
		return TFALSE;
	}
}
#endif