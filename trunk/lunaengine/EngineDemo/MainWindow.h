#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_
#include <Windows.h>
#include "TLSingleton.h"

// ��ʼ�������ýṹ��
struct _MAINWNDCONFIG
{
	bool bWnd;	// �Ƿ�Ϊ����
	UINT bufferWidth;	// �󻺳��
	UINT bufferHeight;	// �󻺳��
	char szWindowText[256];	// ��������

	_MAINWNDCONFIG()
	{
		szWindowText[0] = '\0';
	}
};

class MainWindow : public TLunaEngine::Singleton<MainWindow>
{
	friend class TLunaEngine::Singleton<MainWindow>;
protected:
	MainWindow(void);
	~MainWindow(void);
private:
	// Ӧ�ó���ʵ��
	HINSTANCE m_hInst;
	// ���ھ��
	HWND m_hWnd;
	// ��ȱ�
	float m_widthPro;
	// �߶ȱ�
	float m_heightPro;
	// �Ƿ�Ϊ����
	bool m_bWnd;
	// ���ڿ��
	LONG m_windowWidth;
	LONG m_windowHeight;
	// �ͻ������
	LONG m_clientWidth;
	LONG m_clientHeight;
public:
	// ��ʼ������
	HRESULT InitWindow(HINSTANCE hInst,int nCmdShow,const _MAINWNDCONFIG* config);
	// ���ٴ���
	void DestroyWindow();
	// �������
	void CalcRectPro();
	// �õ����ھ��
	inline HWND GetHwnd(){return m_hWnd;}
	// �Ƿ�Ϊ����ģʽ
	inline bool IsWnd(){return m_bWnd;}
	// �õ�����ڻ�������λ��
	void GetCursorPosInBuffer(float* x,float* y);
	// on size changed
	void onSizeChanged(UINT width, UINT height);
	// �õ���ֵ
	inline LONG GetWindowWidth(){return m_windowWidth;}
	inline LONG GetWindowHeight(){return m_windowHeight;}
	inline LONG GetClientWidth(){return m_clientWidth;}
	inline LONG GetClientHeight(){return m_clientHeight;}
	inline float GetWidthPro(){return m_widthPro;}
	inline float GetHeightPro(){return m_heightPro;}
};

#endif