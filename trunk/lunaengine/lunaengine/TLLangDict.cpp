#include "TLLangDict.h"
#include "TLUTF8FileReader.h"
#include "TLUTF8FileWriter.h"

namespace TLunaEngine
{
	LangDict::LangDict()
	{

	}

	LangDict::~LangDict()
	{
		clearStrings();
	}

	TBOOL LangDict::loadFromFile(const TCHAR* filename)
	{
		if (!filename)
		{
			return TFALSE;
		}
		FILE* fp = TNULL;
		if (UTF8FileReader::OpenTxtFile(filename, &fp))
		{
			// read count
			TU64 count = 0;
			UTF8FileReader::ReadLineULongLong(&count, fp, 1, L' ');
			// loop read key and value
			for (TU64 i = 0; i < count; i++)
			{
				TU64 key = 0;
				TWCHAR value[128] = { 0 };
				TU32 readCharCount = 0;
				UTF8FileReader::ReadLineULongLong(&key, fp, 1, L' ');
				UTF8FileReader::ReadLineWString(value, fp, 0, 0, 127, &readCharCount);
				mStringList.push_back(Pair<TU64, WString>(key, value));
			}
			UTF8FileReader::CloseTxtFile(fp);
		}
		else
		{
			return TFALSE;
		}
		return TTRUE;
	}

	TBOOL LangDict::saveToFile(const TCHAR* filename)
	{
		if (!filename)
		{
			return TFALSE;
		}
		FILE* fp = TNULL;
		if (UTF8FileWriter::OpenTxtFile(filename, &fp))
		{
			// write count
			TU64 count = mStringList.size();
			UTF8FileWriter::WriteLineULongLong(&count, fp, 1, L' ');
			// loop write key and value
			Map<TU64, WString>::Iterator itr = mStringList.begin();
			for (; itr != mStringList.end(); ++itr)
			{
				TU64 key = itr->Key;
				WString value = itr->Value;
				UTF8FileWriter::WriteLineULongLong(&key, fp, 1, L' ');
				UTF8FileWriter::WriteLineWString(value.GetWString(), fp, value.GetLength());
			}
			UTF8FileWriter::CloseTxtFile(fp);
		}
		else
		{
			return TFALSE;
		}
		return TTRUE;
	}

	TVOID LangDict::clearStrings()
	{
		mStringList.clear();
	}

	TBOOL LangDict::findByKey(TU64 key, WString& value)
	{
		Map<TU64, WString>::Iterator itr = mStringList.find(key);
		if (itr != mStringList.end())
		{
			value = itr->Value;
			return TTRUE;
		}
		return TFALSE;
	}
}