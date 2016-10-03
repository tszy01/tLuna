#ifndef _TLCONSOLEOUTPUT_H_
#define _TLCONSOLEOUTPUT_H_

#include <Windows.h>
#include "TLString.h"

namespace TLunaEngine
{
	class ConsoleOutput
	{
	public:
		struct TextLine
		{
			TS32 _posY;
			TU32 _height;
			String _text;
		};
	public:
		ConsoleOutput();
		~ConsoleOutput();

		ConsoleOutput(HWND hWnd, TU32 maxLineCount, TU32 maxLineWidth, TU32 maxDisplayHeight);

	protected:
		List<TextLine> _lineList;
		TU32 _maxDisplayHeight;
		TU32 _displayStartLineNo;
		TU32 _maxLineCount;
		TU32 _maxLineWidth;
		HWND _hWnd;
	public:
		inline TVOID setMaxLineCount(TU32 maxLineCount)
		{
			_maxLineCount = maxLineCount;
		}

		inline TVOID setMaxLineWidth(TU32 maxLineWidth)
		{
			_maxLineWidth = maxLineWidth;
		}

		inline TVOID setMaxDisplayHeight(TU32 maxDisplayHeight)
		{
			_maxDisplayHeight = maxDisplayHeight;
		}

		inline TVOID setDisplayStartLineNo(TU32 displayStartLineNo)
		{
			_displayStartLineNo = displayStartLineNo;
		}

		inline TVOID setHWND(HWND hWnd)
		{
			_hWnd = hWnd;
		}

		const List<TextLine>& getLineList() const
		{
			return _lineList;
		}

	public:
		TVOID addText(const String& text);
		TVOID addTextLine(const String& text, TU32 height);
		TVOID refreshPosY();
		TVOID clear();
		String getDisplayStr();
		TVOID downDisplayStartLine();
		TVOID upDisplayStartLine();
		TU32 getHeightFromDisplayStart();
	};
}

#endif