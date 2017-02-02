#ifndef _TLCONSOLEOUTPUT_H_
#define _TLCONSOLEOUTPUT_H_

#include <Windows.h>
#include "TLString.h"

namespace TLunaEngine
{
	// console output listener
	class ConsoleOutputListener
	{
	public:
		ConsoleOutputListener() {};
		virtual ~ConsoleOutputListener() {}
	public:
		// on addText end
		virtual TVOID onAddTextEnd() {};
	};

	// console output assistant
	class ConsoleOutputAssistant
	{
	public:
		ConsoleOutputAssistant() {};
		virtual ~ConsoleOutputAssistant() {}
	public:
		// before mesure
		virtual TVOID beginMeasureTextSize() {};
		// mesure
		virtual TVOID measureTextSize(const WString& text, TU32& width, TU32& height) {};
		// after mesure
		virtual TVOID endMeasureTextSize() {};
	};

	// console output text class
	class ConsoleOutput
	{
	public:
		// Text Line struct
		struct TextLine
		{
			// line position y
			TS32 _posY;
			// line height
			TU32 _height;
			// text
			WString _text;
		};
	public:
		ConsoleOutput();
		~ConsoleOutput();
		ConsoleOutput(ConsoleOutputAssistant* assistant, TU32 maxLineCount, TU32 maxLineWidth, TU32 maxDisplayHeight);
	protected:
		// line list
		List<TextLine> _lineList;
		// max display height
		TU32 _maxDisplayHeight;
		// display start line index
		TU32 _displayStartLineNo;
		// max line count
		TU32 _maxLineCount;
		// max line width
		TU32 _maxLineWidth;
		// listener list
		// need to maintance the memory by self
		List<ConsoleOutputListener*> _listenerList;
		// assistant
		// need to maintance the memory by self
		ConsoleOutputAssistant* _assistant;
	public:
		// set max line count
		inline TVOID setMaxLineCount(TU32 maxLineCount)
		{
			_maxLineCount = maxLineCount;
		}
		// set max line width
		inline TVOID setMaxLineWidth(TU32 maxLineWidth)
		{
			_maxLineWidth = maxLineWidth;
		}
		// set max display height
		inline TVOID setMaxDisplayHeight(TU32 maxDisplayHeight)
		{
			_maxDisplayHeight = maxDisplayHeight;
		}
		// set display start line index
		inline TVOID setDisplayStartLineNo(TU32 displayStartLineNo)
		{
			_displayStartLineNo = displayStartLineNo;
		}
		// set assistant
		inline TVOID setAssistant(ConsoleOutputAssistant* assistant)
		{
			_assistant = assistant;
		}
		// get line list
		const List<TextLine>& getLineList() const
		{
			return _lineList;
		}

	public:
		// add text
		TVOID addText(const WString& text);
		// add text line
		TVOID addTextLine(const WString& text, TU32 height);
		// refresh pos y
		TVOID refreshPosY();
		// clear all content
		TVOID clear();
		// return display string
		WString getDisplayStr();
		// turn display start line down
		TVOID downDisplayStartLine();
		// turn display start line up
		TVOID upDisplayStartLine();
		// return height from display start
		TU32 getHeightFromDisplayStart();
		// add listener
		TVOID addListener(ConsoleOutputListener* listener);
		// remove listener
		TVOID removeListener(ConsoleOutputListener* listener);
		// clear all listeners
		TVOID clearListener();
	};
}

#endif