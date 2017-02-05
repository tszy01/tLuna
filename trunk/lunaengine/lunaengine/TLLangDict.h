#ifndef _TLLANGDICT_H_
#define _TLLANGDICT_H_

#include "TLCommonTypeDef.h"
#include "TLMap.h"
#include "TLString.h"

namespace TLunaEngine
{
	// Language dictionary class
	class LangDict
	{
	public:
		// constructor
		LangDict();
		// destructor
		~LangDict();
	protected:
		// key string list
		Map<TU64, WString> mStringList;
	public:
		// load from file
		// note : this will call clear
		TBOOL loadFromFile(const TCHAR* filename);
		// save to file
		TBOOL saveToFile(const TCHAR* filename);
		// clear strings
		TVOID clearStrings();
		// find string by key
		TBOOL findByKey(TU64 key, WString& value);
	};
}

#endif