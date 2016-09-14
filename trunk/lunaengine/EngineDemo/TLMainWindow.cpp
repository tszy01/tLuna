#include "StdAfx.h"
#include "TLMainWindow.h"
#include "GlobleFunc.h"

T_SINGLETON_IMP(TLMainWindow);

// ��Ϣ������
long FAR PASCAL WindowProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	    case WM_DESTROY: // ���ٴ���
			PostQuitMessage(0);
		case WM_SETCURSOR:	// ���λ�ڴ�����
			{
				SetCursor(NULL);
			}
			break;
		case WM_NCMOUSEMOVE: // ��������ڷǿͻ�����
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW)); //��Ȼʹ��WinĬ�����
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
	WNDCLASS  wc;    //���崰����
	//���������ô�����
	wc.style         = CS_HREDRAW | CS_VREDRAW ; //֧��ˮƽ�ʹ�ֱ�ػ�
    wc.lpfnWndProc   = WindowProc;               //��Ӧ��Ϣ�Ĵ�����
    wc.cbClsExtra    = 0;                        //�����ڴ�ռ�
	wc.cbWndExtra    = 0;                        //�����ڴ�ռ�
	wc.hInstance     = hInst;					//���ڵ�ʵ�������
	wc.hIcon         = NULL;					//���ڵ�ͼ��
	wc.hCursor       = NULL;                    //���ô���������״
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//����ˢ
	wc.lpszMenuName  = NULL;                     //���ڲ˵�
	wc.lpszClassName = L"TLUNAEngine";          //���ڵ�����
	RegisterClass(&wc);    //ע�ᴰ�ھ��
	HWND hWnd = 0;
	if(config->bWnd)
	{
		hWnd  = CreateWindowExA( WS_EX_APPWINDOW, // ��������ʾ�ڶ���
		                    "TLUNAEngine",//��������
							config->szWindowText, //���ڵı���
							WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,   //���ڵķ��
							0, //X��ĳ�ʼ�趨����
							0, //Y��ĳ�ʼ���趨����
							config->bufferWidth,//GetSystemMetrics(SM_CXSCREEN),//��ȳ�ʼ���趨GetSystemMetrics(SM_CXSCREEN)
							config->bufferHeight,//GetSystemMetrics(SM_CYSCREEN),//�߶ȳ�ʼ���趨GetSystemMetrics(SM_CYSCREEN)
							NULL, //�����ھ��
							NULL, //���ڲ˵����
							hInst, //ʵ�����
							NULL); //������Ϣ
	}
	else
	{
		hWnd  = CreateWindowExA( WS_EX_APPWINDOW, // ��������ʾ�ڶ���
		                    "TLUNAEngine",//��������
							config->szWindowText, //���ڵı���
							WS_POPUP,   //���ڵķ��
							0, //X��ĳ�ʼ�趨����
							0, //Y��ĳ�ʼ���趨����
							config->bufferWidth,//GetSystemMetrics(SM_CXSCREEN),//��ȳ�ʼ���趨GetSystemMetrics(SM_CXSCREEN)
							config->bufferHeight,//GetSystemMetrics(SM_CYSCREEN),//�߶ȳ�ʼ���趨GetSystemMetrics(SM_CYSCREEN)
							NULL, //�����ھ��
							NULL, //���ڲ˵����
							hInst, //ʵ�����
							NULL); //������Ϣ
	}
	if(! hWnd)  //�жϴ����Ƿ����ɹ�
	{
		return E_FAIL;  
	}

	// ��ʼ��ȫ�ֶ���
	ShowWindow(hWnd, nCmdShow); //��ʾ����
	UpdateWindow(hWnd);         //���´���

	m_hInst = hInst;
	m_hWnd = hWnd;
	m_bWnd = config->bWnd;

	return S_OK;
}

void TLMainWindow::DestroyWindow()
{
	// ע������
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
	// �������ڿͻ���λ��
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd,&pt);
	// ���ݱ������㴦����ں󻺳�����λ��
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