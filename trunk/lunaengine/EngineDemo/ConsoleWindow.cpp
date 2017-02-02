#include "ConsoleWindow.h"
#include "TLConsole.h"
#include "TLLog.h"

ConsoleWindow* TLunaEngine::Singleton<ConsoleWindow>::m_Ptr = 0;

// 消息处理函数
LRESULT FAR PASCAL WindowProc2(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
{
	TLunaEngine::ConsoleInput* consoleInput = ConsoleWindow::getSingletonPtr()->GetConsoleInput();
	TLunaEngine::ConsoleOutput* consoleOutput = ConsoleWindow::getSingletonPtr()->GetConsoleOutput();
	switch(message)
	{
	    case WM_DESTROY: // 销毁窗口
			PostQuitMessage(0);
		case WM_SIZE:
			{
				RECT clientRect;
				GetClientRect(hWnd, &clientRect);

				ConsoleWindow::getSingletonPtr()->SetClientSize(clientRect.right, clientRect.bottom);

				LONG inputStartPosY = clientRect.bottom - 16 - 4;
				consoleInput->setMaxLineWidth((TLunaEngine::TU32)clientRect.right);

				//gOutputHeight = gInputStartPosY;
				consoleOutput->setMaxLineWidth((TLunaEngine::TU32)clientRect.right);
				consoleOutput->setMaxDisplayHeight((TLunaEngine::TU32)inputStartPosY);

				SetScrollRange(hWnd, SB_VERT, 0, 100, FALSE);
				SetScrollPos(hWnd, SB_VERT, 0, TRUE);
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code that uses hdc here...
				SetTextColor(ps.hdc, RGB(10, 0, 255));

				// draw output text
				ps.rcPaint.top = 0;
				TLunaEngine::WString strOutput = consoleOutput->getDisplayStr();
				if (strOutput.GetLength() > 0)
				{
					DrawTextW(ps.hdc, strOutput.GetWString(), -1, &(ps.rcPaint), DT_LEFT);
				}

				// draw input text
				LONG inputStartPosY = ConsoleWindow::getSingletonPtr()->GetClientHeight() - 16 - 4;
				ps.rcPaint.top = inputStartPosY;
				TLunaEngine::String strInput = consoleInput->getDisplayStr();
				if (strInput.GetLength() > 0)
				{
					DrawTextA(ps.hdc, strInput.GetString(), -1, &(ps.rcPaint), DT_LEFT);
				}

				// draw input cursor
				int inputCursorX = 0;
				int inputCursorWidth = 5;
				consoleInput->getInsertPosXAndWidth(inputCursorX, inputCursorWidth);

				HPEN hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
				HPEN old_hpen = (HPEN)SelectObject(hdc, hpen);
				MoveToEx(hdc, inputCursorX, inputStartPosY + 16 + 1, NULL);
				LineTo(hdc, inputCursorX + inputCursorWidth, inputStartPosY + 16 + 1);
				SelectObject(hdc, old_hpen);
				DeleteObject(hpen);

				EndPaint(hWnd, &ps);
			}
			break;
		case WM_KEYDOWN:
			{
				UINT keyCode = (UINT)wParam;
				if (keyCode == VK_LEFT)
				{
					consoleInput->runCommand(TLunaEngine::ConsoleInput::CMD_TYPE_LEFT);
					InvalidateRect(hWnd, NULL, TRUE);
				}
				if (keyCode == VK_RIGHT)
				{
					consoleInput->runCommand(TLunaEngine::ConsoleInput::CMD_TYPE_RIGHT);
					InvalidateRect(hWnd, NULL, TRUE);
				}
				if (keyCode == VK_BACK)
				{
					consoleInput->runCommand(TLunaEngine::ConsoleInput::CMD_TYPE_BACK);
					InvalidateRect(hWnd, NULL, TRUE);
				}
				if (keyCode == VK_UP)
				{
					consoleOutput->upDisplayStartLine();
					InvalidateRect(hWnd, NULL, TRUE);
				}
				if (keyCode == VK_DOWN)
				{
					consoleOutput->downDisplayStartLine();
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
			break;
		case WM_KEYUP:
			{
				UINT keyCode = (UINT)wParam;
				if (keyCode == VK_RETURN)
				{
					consoleInput->runCommand(TLunaEngine::ConsoleInput::CMD_TYPE_RETURN);

					SetScrollRange(hWnd, SB_VERT, 0, (int)consoleOutput->getLineList().size(), TRUE);
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
			break;
		case WM_CHAR:
			{
				char ascii_code = (char)wParam;

				HDC hdc = ::GetDC(hWnd);
				SIZE sz;
				HDC memDc = CreateCompatibleDC(hdc);

				::GetTextExtentPointA(memDc, &ascii_code, 1, &sz);

				DeleteObject(memDc);
				::DeleteDC(hdc);

				consoleInput->insertChar(ascii_code, (TLunaEngine::TU32)sz.cx);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case WM_MOUSEWHEEL://WM_MOUSEWHEEL
			{
				int pos = GetScrollPos(hWnd, SB_VERT);
				if ((INT)wParam < 0)
				{
					int scrollMax = (int)consoleOutput->getLineList().size();
					pos += 1;
					if (pos > scrollMax)
					{
						pos = scrollMax;
					}
				}
				else
				{
					pos -= 1;
					if (pos < 0)
					{
						pos = 0;
					}
				}
				SetScrollPos(hWnd, SB_VERT, pos, TRUE);
				consoleOutput->setDisplayStartLineNo((TLunaEngine::TU32)pos);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case WM_VSCROLL:
			{
				switch (LOWORD(wParam))
				{
				case SB_LINEUP:
					{
						int pos = GetScrollPos(hWnd, SB_VERT);
						pos -= 1;
						if (pos < 0)
						{
							pos = 0;
						}
						SetScrollPos(hWnd, SB_VERT, pos, TRUE);

						consoleOutput->setDisplayStartLineNo((TLunaEngine::TU32)pos);
						InvalidateRect(hWnd, NULL, TRUE);
					}
					break;
				case SB_LINEDOWN:
					{
						int pos = GetScrollPos(hWnd, SB_VERT);
						int scrollMax = (int)consoleOutput->getLineList().size();
						pos += 1;
						if (pos > scrollMax)
						{
							pos = scrollMax;
						}
						SetScrollPos(hWnd, SB_VERT, pos, TRUE);

						consoleOutput->setDisplayStartLineNo((TLunaEngine::TU32)pos);
						InvalidateRect(hWnd, NULL, TRUE);
					}
					break;
				case SB_THUMBTRACK:
				case SB_THUMBPOSITION:
					{
						int newPos = HIWORD(wParam);
						SetScrollPos(hWnd, SB_VERT, newPos, TRUE);

						consoleOutput->setDisplayStartLineNo((TLunaEngine::TU32)newPos);
						InvalidateRect(hWnd, NULL, TRUE);
					}
					break;
				default:
					break;
				}
			}
			break;
		default:
			return (int)DefWindowProc(hWnd,message,wParam,lParam);
	}
	return 0;
}

ConsoleWindow::ConsoleWindow(void) : m_hInst(NULL),m_hWnd(NULL), m_consoleInput(NULL), m_consoleOutput(NULL)
{
}

ConsoleWindow::~ConsoleWindow(void)
{
}

HRESULT ConsoleWindow::InitWindow(HINSTANCE hInst, int nCmdShow, int width, int height)
{
	WNDCLASS  wc;    //定义窗口类
	//创建和设置窗口类
	wc.style         = CS_HREDRAW | CS_VREDRAW ; //支持水平和垂直重绘
    wc.lpfnWndProc   = WindowProc2;               //相应信息的处理函数
    wc.cbClsExtra    = 0;                        //附加内存空间
	wc.cbWndExtra    = 0;                        //附加内存空间
	wc.hInstance     = hInst;					//窗口的实例化句柄
	wc.hIcon         = NULL;					//窗口的图标
	wc.hCursor       = NULL;                    //设置窗口鼠标的形状
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//背景刷
	wc.lpszMenuName  = NULL;                     //窗口菜单
	wc.lpszClassName = L"TLunaConsole";          //窗口的名称
	RegisterClass(&wc);    //注册窗口句柄
	HWND hWnd = 0;
	hWnd = CreateWindowExA(WS_EX_STATICEDGE, // 窗口总显示在顶部
		"TLunaConsole",//窗口类名
		"Console", //窗口的标题
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VSCROLL,   //窗口的风格
		0, //X轴的初始设定坐标
		0, //Y轴的初始化设定坐标
		width,//GetSystemMetrics(SM_CXSCREEN),//宽度初始化设定GetSystemMetrics(SM_CXSCREEN)
		height,//GetSystemMetrics(SM_CYSCREEN),//高度初始化设定GetSystemMetrics(SM_CYSCREEN)
		NULL, //父窗口句柄
		NULL, //窗口菜单句柄
		hInst, //实例句柄
		NULL); //附加信息
	if(! hWnd)  //判断窗口是否建立成功
	{
		return E_FAIL;  
	}

	m_hInst = hInst;
	m_hWnd = hWnd;
	m_windowWidth = width;
	m_windowHeight = height;

	m_consoleInput = new TLunaEngine::ConsoleInput(0, 5);
	m_consoleOutput = new TLunaEngine::ConsoleOutput(this, 1000, 0, 0);
	m_consoleInput->addListener(this);
	m_consoleOutput->addListener(this);
	TLunaEngine::Console::getSingletonPtr()->setInputAndOutput(m_consoleInput, m_consoleOutput);

	// 初始化全局对象
	ShowWindow(hWnd, nCmdShow); //显示窗口
	UpdateWindow(hWnd);         //更新窗口
	return S_OK;
}

void ConsoleWindow::DestroyWindow()
{
	TLunaEngine::Console::getSingletonPtr()->setInputAndOutput(0, 0);
	if (m_consoleInput)
	{
		delete m_consoleInput;
		m_consoleInput = NULL;
	}
	if (m_consoleOutput)
	{
		delete m_consoleOutput;
		m_consoleOutput = NULL;
	}
	TLunaEngine::Console::delSingletonPtr();
	// 注销窗口
   UnregisterClass(L"TLunaConsole",m_hInst); 
}

TLunaEngine::TVOID ConsoleWindow::onReturn(const TLunaEngine::String& str)
{
	//if (m_consoleOutput && m_consoleInput)
	//{
		//m_consoleOutput->addText(m_consoleInput->getFinalStr());
		//TLunaEngine::Log::WriteLine(TLunaEngine::Log::LOG_LEVEL_ERROR, TLunaEngine::TTRUE, m_consoleInput->getFinalStr().GetString());
	//}
}

TLunaEngine::TVOID ConsoleWindow::onAddTextEnd()
{
	InvalidateRect(m_hWnd, NULL, TRUE);
}

TLunaEngine::TVOID ConsoleWindow::beginMeasureTextSize()
{
	m_hdc = ::GetDC(m_hWnd);
	m_memDc = ::CreateCompatibleDC(m_hdc);
}

TLunaEngine::TVOID ConsoleWindow::measureTextSize(const TLunaEngine::String& text, TLunaEngine::TU32& width, TLunaEngine::TU32& height)
{
	SIZE strSize;
	::GetTextExtentPointA(m_memDc, text.GetString(), text.GetLength(), &strSize);
	width = (TLunaEngine::TU32)strSize.cx;
	height = (TLunaEngine::TU32)strSize.cy;
}

TLunaEngine::TVOID ConsoleWindow::endMeasureTextSize()
{
	::DeleteObject(m_memDc);
	::DeleteDC(m_hdc);

	m_memDc = NULL;
	m_hdc = NULL;
}