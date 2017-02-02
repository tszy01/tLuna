#include "TLConsoleOutput.h"

namespace TLunaEngine
{
	ConsoleOutput::ConsoleOutput() : _assistant(TNULL), _maxDisplayHeight(10), _displayStartLineNo(0), _maxLineCount(1000), _maxLineWidth(20)
	{

	}

	ConsoleOutput::ConsoleOutput(ConsoleOutputAssistant* assistant, TU32 maxLineCount, TU32 maxLineWidth, TU32 maxDisplayHeight) :
		_assistant(assistant), _maxDisplayHeight(maxDisplayHeight), _displayStartLineNo(0), _maxLineCount(maxLineCount), _maxLineWidth(maxLineWidth)
	{

	}

	ConsoleOutput::~ConsoleOutput()
	{
		clearListener();
		clear();
	}

	TVOID ConsoleOutput::addText(const WString& text)
	{
		if (text.GetLength() == 0 || !_assistant)
			return;

		_assistant->beginMeasureTextSize();

		WString tmpStr;
		TU32 measureWidth, measureHeight;

		// split to lines and add
		for (TU32 i = 0; i < text.GetLength(); ++i)
		{
			if (text[i] == L'\n')
			{
				if (tmpStr.GetLength() > 0)
				{
					_assistant->measureTextSize(tmpStr, measureWidth, measureHeight);
					addTextLine(tmpStr, measureHeight);
					tmpStr = L"";
				}
			}
			else
			{
				tmpStr += text[i];
				_assistant->measureTextSize(tmpStr, measureWidth, measureHeight);

				if (measureWidth >= _maxLineWidth)
				{
					addTextLine(tmpStr, measureHeight);
					tmpStr = L"";
				}
			}
		}

		_assistant->measureTextSize(tmpStr, measureWidth, measureHeight);
		addTextLine(tmpStr, measureHeight);

		_assistant->endMeasureTextSize();

		List<ConsoleOutputListener*>::Iterator itrListener = _listenerList.begin();
		for (; itrListener != _listenerList.end(); ++itrListener)
		{
			(*itrListener)->onAddTextEnd();
		}
	}

	TVOID ConsoleOutput::addTextLine(const WString& text, TU32 height)
	{
		if (text.GetLength() == 0)
			return;

		TextLine line;
		line._posY = 0;
		line._height = height;
		line._text = text;

		// pop front
		while ((TU32)_lineList.size() >= _maxLineCount)
		{
			_lineList.pop_front();
		}
		_lineList.push_back(line);

		// calc start display no
		while (getHeightFromDisplayStart() >= _maxDisplayHeight)
		{
			downDisplayStartLine();
		}
	}

	TVOID ConsoleOutput::refreshPosY()
	{
		List<TextLine>::Iterator itrDisplayStart = _lineList.begin();
		List<TextLine>::Iterator itr = _lineList.begin();
		TU32 index = 0;
		TS32 tmpY = 0;

		for (; itr != _lineList.end(); ++itr, ++index)
		{
			if (index < _displayStartLineNo)
			{
				++itrDisplayStart;
			}
			else if (index >= _displayStartLineNo)
			{
				itr->_posY = tmpY;
				tmpY += (TS32)itr->_height;
			}
		}

		tmpY = 0;
		do
		{
			if (itrDisplayStart == _lineList.begin())
			{
				break;
			}

			--itrDisplayStart;

			tmpY -= (TS32)itrDisplayStart->_height;
			itrDisplayStart->_posY = tmpY;
		} while (true);
	}

	TVOID ConsoleOutput::clear()
	{
		_lineList.clear();
		_displayStartLineNo = 0;
	}

	WString ConsoleOutput::getDisplayStr()
	{
		WString result;
		TU32 index = 0;
		TU32 totalHeight = 0;

		List<TextLine>::Iterator itr = _lineList.begin();
		for (; itr != _lineList.end(); ++itr, ++index)
		{
			if (index >= _displayStartLineNo)
			{
				result += itr->_text;
				result += L'\n';
				totalHeight += itr->_height;

				if (totalHeight >= _maxDisplayHeight)
				{
					break;
				}
			}
		}
		return result;
	}

	TVOID ConsoleOutput::downDisplayStartLine()
	{
		if (_displayStartLineNo < _lineList.size())
		{
			_displayStartLineNo += 1;
		}
	}

	TVOID ConsoleOutput::upDisplayStartLine()
	{
		if (_displayStartLineNo > 0)
		{
			_displayStartLineNo -= 1;
		}
	}

	TU32 ConsoleOutput::getHeightFromDisplayStart()
	{
		TU32 index = 0;
		TU32 result = 0;
		List<TextLine>::Iterator itr = _lineList.begin();
		for (; itr != _lineList.end(); ++itr, ++index)
		{
			if (index >= _displayStartLineNo)
			{
				result += itr->_height;
			}
		}
		return result;
	}

	TVOID ConsoleOutput::addListener(ConsoleOutputListener* listener)
	{
		if (listener == 0)
			return;
		List<ConsoleOutputListener*>::Iterator itr = _listenerList.find(listener);
		if (itr != _listenerList.end())
		{
			return;
		}
		_listenerList.push_back(listener);
	}

	TVOID ConsoleOutput::removeListener(ConsoleOutputListener* listener)
	{
		if (listener == 0)
			return;
		List<ConsoleOutputListener*>::Iterator itr = _listenerList.find(listener);
		if (itr != _listenerList.end())
		{
			_listenerList.erase(itr);
		}
	}

	TVOID ConsoleOutput::clearListener()
	{
		_listenerList.clear();
	}
}