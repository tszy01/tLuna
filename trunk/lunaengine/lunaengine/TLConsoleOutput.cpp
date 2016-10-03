#include "TLConsoleOutput.h"

namespace TLunaEngine
{
	ConsoleOutput::ConsoleOutput() : _hWnd(NULL), _maxDisplayHeight(10), _displayStartLineNo(0), _maxLineCount(1000), _maxLineWidth(20)
	{

	}

	ConsoleOutput::ConsoleOutput(HWND hWnd, TU32 maxLineCount, TU32 maxLineWidth, TU32 maxDisplayHeight) :
		_hWnd(hWnd), _maxDisplayHeight(maxDisplayHeight), _displayStartLineNo(0), _maxLineCount(maxLineCount), _maxLineWidth(maxLineWidth)
	{

	}

	ConsoleOutput::~ConsoleOutput()
	{
		clear();
	}

	TVOID ConsoleOutput::addText(const String& text)
	{
		if (text.GetLength() == 0 || _hWnd == NULL)
			return;

		HDC hdc = ::GetDC(_hWnd);
		SIZE strSize;
		HDC memDc = CreateCompatibleDC(hdc);

		String tmpStr;

		// split to lines and add
		for (TU32 i = 0; i < text.GetLength(); ++i)
		{
			if (text[i] == '\n')
			{
				if (tmpStr.GetLength() > 0)
				{
					::GetTextExtentPointA(memDc, tmpStr.GetString(), tmpStr.GetLength(), &strSize);

					addTextLine(tmpStr, strSize.cy);
					tmpStr = "";
				}
			}
			else
			{
				tmpStr += text[i];

				::GetTextExtentPointA(memDc, tmpStr.GetString(), tmpStr.GetLength(), &strSize);

				if (strSize.cx >= (TS32)_maxLineWidth)
				{
					addTextLine(tmpStr, strSize.cy);
					tmpStr = "";
				}
			}
		}

		::GetTextExtentPointA(memDc, tmpStr.GetString(), tmpStr.GetLength(), &strSize);
		addTextLine(tmpStr, strSize.cy);

		DeleteObject(memDc);
		::DeleteDC(hdc);
	}

	TVOID ConsoleOutput::addTextLine(const String& text, TU32 height)
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

	String ConsoleOutput::getDisplayStr()
	{
		String result;
		TU32 index = 0;
		TU32 totalHeight = 0;

		List<TextLine>::Iterator itr = _lineList.begin();
		for (; itr != _lineList.end(); ++itr, ++index)
		{
			if (index >= _displayStartLineNo)
			{
				result += itr->_text;
				result += '\n';
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
}