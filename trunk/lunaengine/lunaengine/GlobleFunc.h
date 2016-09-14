#pragma once


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
	bool Init(HWND hWnd,HINSTANCE hInst,BOOL bWnd,LONG lWidth,LONG lHeight,
							 float fSecsPerFrame,const char* szResDir,bool bShowDebugInfo);

	// ����ѭ��
	bool OnLoopCtrlUpdate(float* pTimeElapsed);
	bool OnSceneUpdate(float fTimeElapsed);
	bool OnSceneRender(float fTimeElapsed);

	// ��Ϸѭ��
	bool OnGameLoop(UserLoop pLoop);

	// �����˳�
	bool Destroy();

	// �ػ�IO��Ϣ
	// yType:��Ϣ����
	// param:��Ϣ�ṹ��
	void CatchInputMsg(BYTE yType,void* param);

	// window size changed
	bool onWindowSizeChanged(UINT width,UINT height);

	// ------------- �����Ǹ��༭�����õķ�������Ϸ�в���ʹ�� --------------------------------

	// �����ʼ��
	// hWnd:���ھ��
	// hInst:Ӧ�ó�����
	// lWidth:�󻺳�����
	// lHeight:�󻺳�����
	// szResDir:��Դ��Ŀ¼
	bool InitForEditor(HWND hWnd,HINSTANCE hInst,LONG lWidth,LONG lHeight,const char* szResDir);

	// ����ѭ��
	bool OnSceneUpdateForEditor(float fTimeElapsed);
	bool OnSceneRenderForEditor(float fTimeElapsed);

	// �����˳�
	bool DestroyForEditor();

	// ---------------------------------------------------------------------------------------

}