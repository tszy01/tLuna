#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_
#include <Windows.h>
#include "TSSingleton.h"

// 初始化窗口用结构体
struct _MAINWNDCONFIG
{
	bool bWnd;	// 是否为窗口
	UINT bufferWidth;	// 后缓冲宽
	UINT bufferHeight;	// 后缓冲高
	char szWindowText[256];	// 窗口名称

	_MAINWNDCONFIG()
	{
		szWindowText[0] = '\0';
	}
};

class MainWindow : public TSun::Singleton<MainWindow>
{
	friend class TSun::Singleton<MainWindow>;
protected:
	MainWindow(void);
	~MainWindow(void);
private:
	// 应用程序实例
	HINSTANCE m_hInst;
	// 窗口句柄
	HWND m_hWnd;
	// 宽度比
	float m_widthPro;
	// 高度比
	float m_heightPro;
	// 是否为窗口
	bool m_bWnd;
	// 窗口宽高
	LONG m_windowWidth;
	LONG m_windowHeight;
	// 客户区宽高
	LONG m_clientWidth;
	LONG m_clientHeight;
public:
	// 初始化窗口
	HRESULT InitWindow(HINSTANCE hInst,int nCmdShow,const _MAINWNDCONFIG* config);
	// 销毁窗口
	void DestroyWindow();
	// 计算比例
	void CalcRectPro();
	// 得到窗口句柄
	inline HWND GetHwnd(){return m_hWnd;}
	// 是否为窗口模式
	inline bool IsWnd(){return m_bWnd;}
	// 得到鼠标在缓冲区中位置
	void GetCursorPosInBuffer(float* x,float* y);
	// on size changed
	void onSizeChanged(UINT width, UINT height);
	// 得到数值
	inline LONG GetWindowWidth(){return m_windowWidth;}
	inline LONG GetWindowHeight(){return m_windowHeight;}
	inline LONG GetClientWidth(){return m_clientWidth;}
	inline LONG GetClientHeight(){return m_clientHeight;}
	inline float GetWidthPro(){return m_widthPro;}
	inline float GetHeightPro(){return m_heightPro;}
};

#endif