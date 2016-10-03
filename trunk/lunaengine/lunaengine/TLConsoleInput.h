#ifndef _TLCONSOLEINPUT_H_
#define _TLCONSOLEINPUT_H_

#include "TLString.h"

namespace TLunaEngine
{
	// Input listener class
	class ConsoleInputListener
	{
	public:
		ConsoleInputListener() {};
		virtual ~ConsoleInputListener() {};
	public:
		// called when return cmd
		virtual TVOID onReturn(const String& str) {};
	};

	// Input class
	class ConsoleInput
	{
	public:
		// Command type
		enum CMD_TYPE
		{
			CMD_TYPE_LEFT,
			CMD_TYPE_RIGHT,
			CMD_TYPE_BACK,
			CMD_TYPE_RETURN,
		};
		// charater struct
		struct Char
		{
			// character width
			TU32 _charWidth;
			// character pos x
			TS32 _posX;
			// character
			TCHAR _ascii;
		};
	protected:
		// insert character index
		TU32 _insertPos;
		// max width to render characters
		TU32 _maxLineWidth;
		// display start character index
		TU32 _displayStartPos;
		// default character width
		TU32 _defaultCharWidth;
		// character list
		List<Char> _charList;
		// listener list
		// need to maintance the memory by self
		List<ConsoleInputListener*> _listenerList;
	public:
		ConsoleInput();
		~ConsoleInput();

		ConsoleInput(TU32 maxLineWidth, TU32 defaultCharWidth);
	public:
		inline TVOID setMaxLineWidth(TU32 maxLineWidth)
		{
			_maxLineWidth = maxLineWidth;
		}

		inline TVOID setDefaultCharWidth(TU32 defaultCharWidth)
		{
			_defaultCharWidth = defaultCharWidth;
		}

		const List<Char>& getCharList() const
		{
			return _charList;
		}

		inline TU32 getInsertPos() const
		{
			return _insertPos;
		}
	public:
		// insert character before insert pos
		TVOID insertChar(TCHAR ascii, TU32 charWidth);
		// remove character before insert pos
		TVOID removeChar();
		// run command
		TVOID runCommand(CMD_TYPE cmd);
		// calculate pos x
		TVOID refreshPosX();
		// return total string
		String getFinalStr();
		// clear all characters
		TVOID clear();
		// turn insert pos left
		TVOID leftInsertPos();
		// turn insert pos right
		TVOID rightInsertPos();
		// calculate width from display start character to instert pos
		TU32 getWidthFromDisplayStartToInsert();
		// get insert pos x and insert width
		TVOID getInsertPosXAndWidth(TS32& posX, TS32& width);
		// return display string
		String getDisplayStr();
		// add listener
		TVOID addListener(ConsoleInputListener* listener);
		// remove listener
		TVOID removeListener(ConsoleInputListener* listener);
		// clear all listeners
		TVOID clearListener();
	};
}

#endif
