#ifndef _CONSOLEWINDOW_H_
#define _CONSOLEWINDOW_H_
#include <Windows.h>
#include "TLSingleton.h"
#include "TLConsoleInput.h"
#include "TLConsoleOutput.h"

class ConsoleWindow : public TLunaEngine::Singleton<ConsoleWindow>, 
	TLunaEngine::ConsoleInputListener, TLunaEngine::ConsoleOutputListener, TLunaEngine::ConsoleOutputAssistant
{
	friend class TLunaEngine::Singleton<ConsoleWindow>;
protected:
	ConsoleWindow(void);
	~ConsoleWindow(void);
private:
	// 应用程序实例
	HINSTANCE m_hInst;
	// 窗口句柄
	HWND m_hWnd;
	// 窗口宽高
	LONG m_windowWidth;
	LONG m_windowHeight;
	// 客户区宽高
	LONG m_clientWidth;
	LONG m_clientHeight;
	// console input
	TLunaEngine::ConsoleInput* m_consoleInput;
	// console output
	TLunaEngine::ConsoleOutput* m_consoleOutput;
	// gdi hdc
	HDC m_hdc;
	// gdi mem dc
	HDC m_memDc;
public:
	// 初始化窗口
	HRESULT InitWindow(HINSTANCE hInst,int nCmdShow,int width,int height);
	// 销毁窗口
	void DestroyWindow();
	// 得到窗口句柄
	inline HWND GetHwnd(){return m_hWnd;}
	// set client width and height
	inline void SetClientSize(LONG width, LONG height)
	{
		m_clientWidth = width;
		m_clientHeight = height;
	}
	// get client width
	inline LONG GetClientWidth() const
	{
		return m_clientWidth;
	}
	// get client height
	inline LONG GetClientHeight() const
	{
		return m_clientHeight;
	}
	// get console input
	inline TLunaEngine::ConsoleInput* GetConsoleInput()
	{
		return m_consoleInput;
	}
	// get console output
	inline TLunaEngine::ConsoleOutput* GetConsoleOutput()
	{
		return m_consoleOutput;
	}
public:
	// called when return cmd
	virtual TLunaEngine::TVOID onReturn(const TLunaEngine::String& str);
	// on addText end
	TLunaEngine::TVOID onAddTextEnd();
	// before mesure
	TLunaEngine::TVOID beginMeasureTextSize();
	// mesure
	TLunaEngine::TVOID measureTextSize(const TLunaEngine::String& text, TLunaEngine::TU32& width, TLunaEngine::TU32& height);
	// after mesure
	TLunaEngine::TVOID endMeasureTextSize();
};

#endif