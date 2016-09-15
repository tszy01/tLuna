#ifndef _TGLOBALFUNC_H_
#define _TGLOBALFUNC_H_
#include "TLCommonTypeDef.h"
#include <Windows.h>

namespace TLunaEngine{

	// ���庯��ָ�룬�ϲ���Ϸѭ��ʱ����
	typedef void(*UserLoop)(float fTimeElapsed);

	// -------------------------- ����Ϊ����ȫ�ֺ������ϲ���� ------------------------------

	// �����ʼ��
	// hWnd:���ھ��
	// hInst:Ӧ�ó���ʵ�����
	// bWnd:�Ƿ�Ϊ����
	// lWidth:�󻺳�����
	// lHeight:�󻺳�����
	// fSecsPerFrame:֡���ʿ���(ÿ֡ʱ��)
	// szResDir:��Դ��Ŀ¼
	// bShowDebugInfo:�Ƿ���ʾDebug��Ϣ
	bool Init(HWND hWnd,HINSTANCE hInst,TBOOL bWnd,TS32 lWidth,TS32 lHeight,
							 float fSecsPerFrame,const char* szResDir,bool bShowDebugInfo);

	// ����ѭ��
	bool OnLoopCtrlUpdate(float* pTimeElapsed);
	bool OnSceneUpdate(float fTimeElapsed);
	bool OnSceneRender(float fTimeElapsed);

	// ��Ϸѭ��
	bool OnGameLoop(UserLoop pLoop);

	// �����˳�
	bool Destroy();

	// window size changed
	bool onWindowSizeChanged(TU32 width,TU32 height);

	// ------------- �����Ǹ��༭�����õķ�������Ϸ�в���ʹ�� --------------------------------

	// �����ʼ��
	// hWnd:���ھ��
	// hInst:Ӧ�ó�����
	// lWidth:�󻺳�����
	// lHeight:�󻺳�����
	// szResDir:��Դ��Ŀ¼
	bool InitForEditor(HWND hWnd,HINSTANCE hInst,TS32 lWidth,TS32 lHeight,const char* szResDir);

	// ����ѭ��
	bool OnSceneUpdateForEditor(float fTimeElapsed);
	bool OnSceneRenderForEditor(float fTimeElapsed);

	// �����˳�
	bool DestroyForEditor();

	// ---------------------------------------------------------------------------------------

}

#endif