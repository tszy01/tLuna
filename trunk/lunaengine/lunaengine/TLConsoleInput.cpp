#include "TLConsoleInput.h"

namespace TLunaEngine
{
	ConsoleInput::ConsoleInput() : _insertPos(0), _maxLineWidth(20), _displayStartPos(0), _defaultCharWidth(5)
	{

	}

	ConsoleInput::ConsoleInput(TU32 maxLineWidth, TU32 defaultCharWidth) :
		_insertPos(0), _maxLineWidth(maxLineWidth), _displayStartPos(0), _defaultCharWidth(defaultCharWidth)
	{

	}

	ConsoleInput::~ConsoleInput()
	{
		clearListener();
		clear();
	}

	void ConsoleInput::insertChar(TCHAR ascii, TU32 charWidth)
	{
		if (ascii < 32 || ascii > 126 || charWidth <= 0)
		{
			return;
		}
		Char newChar;
		newChar._posX = -5;
		newChar._charWidth = charWidth;
		newChar._ascii = ascii;

		if (_insertPos == _charList.size())
		{
			_charList.push_back(newChar);
			rightInsertPos();
			return;
		}

		List<Char>::Iterator itr = _charList.begin();
		TU32 index = 0;

		for (; itr != _charList.end(); ++itr, ++index)
		{
			if (index == _insertPos)
			{
				_charList.insert_before(itr, newChar);
				rightInsertPos();
				break;
			}
		}
	}

	void ConsoleInput::removeChar()
	{
		List<Char>::Iterator itr = _charList.begin();
		TU32 index = 0;

		for (; itr != _charList.end(); ++itr, ++index)
		{
			if (index == _insertPos - 1)
			{
				_charList.erase(itr);
				leftInsertPos();
				break;
			}
		}
	}

	void ConsoleInput::runCommand(CMD_TYPE cmd)
	{
		switch (cmd)
		{
		case ConsoleInput::CMD_TYPE_LEFT:
			{
				leftInsertPos();
			}
			break;
		case ConsoleInput::CMD_TYPE_RIGHT:
			{
				rightInsertPos();
			}
			break;
		case ConsoleInput::CMD_TYPE_BACK:
			{
				if (_insertPos > 0 && _insertPos <= (TU32)_charList.size())
				{
					removeChar();
				}
			}
			break;
		case ConsoleInput::CMD_TYPE_RETURN:
			{
				String finalStr = getFinalStr();
				List<ConsoleInputListener*>::Iterator itrListener = _listenerList.begin();
				for (; itrListener != _listenerList.end(); ++itrListener)
				{
					(*itrListener)->onReturn(finalStr);
				}
				clear();
			}
			break;
		default:
			break;
		}
	}

	void ConsoleInput::refreshPosX()
	{
		List<Char>::Iterator itrDisplayStart = _charList.begin();
		List<Char>::Iterator itr = _charList.begin();
		TU32 index = 0;
		TS32 tmpX = 0;

		for (; itr != _charList.end(); ++itr, ++index)
		{
			if (index < _displayStartPos)
			{
				++itrDisplayStart;
			}
			else if (index >= _displayStartPos)
			{
				itr->_posX = tmpX;
				tmpX += (TS32)itr->_charWidth;
			}
		}

		tmpX = 0;
		do
		{
			if (itrDisplayStart == _charList.begin())
			{
				break;
			}

			--itrDisplayStart;

			tmpX -= (TS32)itrDisplayStart->_charWidth;
			itrDisplayStart->_posX = tmpX;
		} while (TTRUE);
	}

	String ConsoleInput::getFinalStr()
	{
		String result;
		List<Char>::Iterator itr = _charList.begin();
		for (; itr != _charList.end(); ++itr)
		{
			result += itr->_ascii;
		}
		return result;
	}

	void ConsoleInput::clear()
	{
		_charList.clear();
		_insertPos = 0;
		_displayStartPos = 0;
	}

	void ConsoleInput::leftInsertPos()
	{
		if (_insertPos > 0)
		{
			_insertPos -= 1;
			if (_insertPos < _displayStartPos)
			{
				_displayStartPos -= 1;
			}
		}
	}

	void ConsoleInput::rightInsertPos()
	{
		if (_insertPos < (TU32)_charList.size())
		{
			_insertPos += 1;
			while (getWidthFromDisplayStartToInsert() >= _maxLineWidth)
			{
				_displayStartPos += 1;
			}
		}
	}

	TU32 ConsoleInput::getWidthFromDisplayStartToInsert()
	{
		TU32 index = 0;
		TU32 result = 0;
		List<Char>::Iterator itr = _charList.begin();
		for (; itr != _charList.end(); ++itr, ++index)
		{
			if (index >= _displayStartPos && index <= _insertPos)
			{
				result += itr->_charWidth;
			}
		}
		if (_insertPos == _charList.size())
		{
			result += _defaultCharWidth;
		}
		return result;
	}

	void ConsoleInput::getInsertPosXAndWidth(TS32& posX, TS32& width)
	{
		TU32 index = 0;
		posX = 0;
		width = _defaultCharWidth;
		List<Char>::Iterator itr = _charList.begin();
		for (; itr != _charList.end(); ++itr, ++index)
		{
			if (index >= _displayStartPos && index < _insertPos)
			{
				posX += (TS32)itr->_charWidth;
			}
			else if (index == _insertPos)
			{
				width = (TS32)itr->_charWidth;
			}
		}
	}

	String ConsoleInput::getDisplayStr()
	{
		String result;
		TU32 index = 0;
		TU32 totalWidth = 0;
		List<Char>::Iterator itr = _charList.begin();
		for (; itr != _charList.end(); ++itr, ++index)
		{
			if (index >= _displayStartPos)
			{
				result += itr->_ascii;
				totalWidth += itr->_charWidth;

				if (totalWidth >= _maxLineWidth)
				{
					break;
				}
			}
		}
		return result;
	}

	TVOID ConsoleInput::addListener(ConsoleInputListener* listener)
	{
		if (listener == 0)
			return;
		List<ConsoleInputListener*>::Iterator itr = _listenerList.find(listener);
		if (itr != _listenerList.end())
		{
			return;
		}
		_listenerList.push_back(listener);
	}

	TVOID ConsoleInput::removeListener(ConsoleInputListener* listener)
	{
		if (listener == 0)
			return;
		List<ConsoleInputListener*>::Iterator itr = _listenerList.find(listener);
		if (itr != _listenerList.end())
		{
			_listenerList.erase(itr);
		}
	}

	TVOID ConsoleInput::clearListener()
	{
		_listenerList.clear();
	}
}