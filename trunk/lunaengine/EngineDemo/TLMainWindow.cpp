#include "StdAfx.h"
#include "TLMainWindow.h"
#include "GlobleFunc.h"

T_SINGLETON_IMP(TLMainWindow);

// 消息处理函数
long FAR PASCAL WindowProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	    case WM_DESTROY: // 销毁窗口
			PostQuitMessage(0);
		case WM_SETCURSOR:	// 鼠标位于窗口上
			{
				SetCursor(NULL);
			}
			break;
		case WM_NCMOUSEMOVE: // 鼠标若是在非客户区上
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW)); //依然使用Win默认鼠标
			}
			break;
		case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				TLunaEngine::onWindowSizeChanged(width,height);
				TLMainWindow::getSingletonPtr()->onSizeChanged(width,height);
			}
			break;
		default:
			return (int)DefWindowProc(hWnd,message,wParam,lParam);
	}
	return 0;
}

TLMainWindow::TLMainWindow(void) : m_hInst(NULL),m_hWnd(NULL),m_widthPro(0),m_heightPro(0),m_bWnd(false)
{
}

TLMainWindow::~TLMainWindow(void)
{
}

HRESULT TLMainWindow::InitWindow(HINSTANCE hInst, int nCmdShow, const _MAINWNDCONFIG* config)
{
	if(!config)
		return E_FAIL;
	WNDCLASS  wc;    //定义窗口类
	//创建和设置窗口类
	wc.style         = CS_HREDRAW | CS_VREDRAW ; //支持水平和垂直重绘
    wc.lpfnWndProc   = WindowProc;               //相应信息的处理函数
    wc.cbClsExtra    = 0;                        //附加内存空间
	wc.cbWndExtra    = 0;                        //附加内存空间
	wc.hInstance     = hInst;					//窗口的实例化句柄
	wc.hIcon         = NULL;					//窗口的图标
	wc.hCursor       = NULL;                    //设置窗口鼠标的形状
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//背景刷
	wc.lpszMenuName  = NULL;                     //窗口菜单
	wc.lpszClassName = L"TLUNAEngine";          //窗口的名称
	RegisterClass(&wc);    //注册窗口句柄
	HWND hWnd = 0;
	if(config->bWnd)
	{
		hWnd  = CreateWindowExA( WS_EX_APPWINDOW, // 窗口总显示在顶部
		                    "TLUNAEngine",//窗口类名
							config->szWindowText, //窗口的标题
							WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,   //窗口的风格
							0, //X轴的初始设定坐标
							0, //Y轴的初始化设定坐标
							config->bufferWidth,//GetSystemMetrics(SM_CXSCREEN),//宽度初始化设定GetSystemMetrics(SM_CXSCREEN)
							config->bufferHeight,//GetSystemMetrics(SM_CYSCREEN),//高度初始化设定GetSystemMetrics(SM_CYSCREEN)
							NULL, //父窗口句柄
							NULL, //窗口菜单句柄
							hInst, //实例句柄
							NULL); //附加信息
	}
	else
	{
		hWnd  = CreateWindowExA( WS_EX_APPWINDOW, // 窗口总显示在顶部
		                    "TLUNAEngine",//窗口类名
							config->szWindowText, //窗口的标题
							WS_POPUP,   //窗口的风格
							0, //X轴的初始设定坐标
							0, //Y轴的初始化设定坐标
							config->bufferWidth,//GetSystemMetrics(SM_CXSCREEN),//宽度初始化设定GetSystemMetrics(SM_CXSCREEN)
							config->bufferHeight,//GetSystemMetrics(SM_CYSCREEN),//高度初始化设定GetSystemMetrics(SM_CYSCREEN)
							NULL, //父窗口句柄
							NULL, //窗口菜单句柄
							hInst, //实例句柄
							NULL); //附加信息
	}
	if(! hWnd)  //判断窗口是否建立成功
	{
		return E_FAIL;  
	}

	// 初始化全局对象
	ShowWindow(hWnd, nCmdShow); //显示窗口
	UpdateWindow(hWnd);         //更新窗口

	m_hInst = hInst;
	m_hWnd = hWnd;
	m_bWnd = config->bWnd;

	return S_OK;
}

void TLMainWindow::DestroyWindow()
{
	// 注销窗口
   UnregisterClass(L"TLUNAEngine",m_hInst); 
}

void TLMainWindow::CalcRectPro()
{
	if(m_bWnd)
	{
		RECT clientRect,windowRect;
		GetWindowRect(m_hWnd,&windowRect);
		GetClientRect(m_hWnd,&clientRect);
		LONG windowWidth=windowRect.right-windowRect.left;
		LONG windowHeight=windowRect.bottom-windowRect.top;
		LONG clientWidth=clientRect.right-clientRect.left;
		LONG clientHeight=clientRect.bottom-clientRect.top;
		m_widthPro=(float)(clientWidth+(windowWidth-clientWidth)/2)/(float)clientWidth;
		m_heightPro=(float)(clientHeight+(windowHeight-clientHeight)/2)/(float)clientHeight;
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;
		m_clientWidth = clientWidth;
		m_clientHeight = clientHeight;
	}
}

void TLMainWindow::onSizeChanged(UINT width, UINT height)
{
	CalcRectPro();
}

void TLMainWindow::GetCursorPosInBuffer(float *x, float *y)
{
	if(!x || !y)
		return;
	POINT pt;
	// 获得鼠标在客户区位置
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd,&pt);
	// 根据比例结算处鼠标在后缓冲区的位置
	if(m_bWnd)
	{
		float x=(float)pt.x * m_widthPro;
		x*=m_widthPro;
		pt.x=(LONG)(x+0.5f);
		float y=(float)pt.y * m_heightPro;
		y*=m_heightPro;
		pt.y=(LONG)(y+0.5f);
	}

	(*x) = (float)pt.x;
	(*y) = (float)pt.y;
}