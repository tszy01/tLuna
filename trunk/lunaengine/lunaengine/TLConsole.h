#ifndef _TLCONSOLE_H_
#define _TLCONSOLE_H_

#include "TLSingleton.h"
#include "TLConsoleInput.h"

namespace TLunaEngine
{
	class ConsoleOutput;

	// Console class for input command and output result or log
	class Console : public Singleton<Console>, ConsoleInputListener
	{
		friend class Singleton<Console>;
	protected:
		Console(TVOID);
		virtual ~Console(TVOID);
	protected:
		// input ref
		// need to maintance the memory by self
		ConsoleInput* mInput;
		// output ref
		// need to maintance the memory by self
		ConsoleOutput* mOutput;
	public:
		// called when return cmd
		virtual TVOID onReturn(const String& str);
	public:
		// set input and output ref
		TVOID setInputAndOutput(ConsoleInput* input, ConsoleOutput* output);
		// output to console
		TVOID outputToConsole(const WString& text);
	};
}

#endif
