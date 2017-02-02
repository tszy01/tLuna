#include "TLConsole.h"
#include "TLConsoleOutput.h"

namespace TLunaEngine
{
	Console* Singleton<Console>::m_Ptr = 0;

	Console::Console() : mInput(TNULL), mOutput(TNULL)
	{

	}

	Console::~Console()
	{

	}

	TVOID Console::onReturn(const String& str)
	{

	}

	TVOID Console::setInputAndOutput(ConsoleInput* input, ConsoleOutput* output)
	{
		if (mInput)
		{
			mInput->removeListener(this);
		}
		mInput = input;
		if (mInput)
		{
			mInput->addListener(this);
		}
		mOutput = output;
	}

	TVOID Console::outputToConsole(const WString& text)
	{
		if (mOutput)
		{
			mOutput->addText(text);
		}
	}
}