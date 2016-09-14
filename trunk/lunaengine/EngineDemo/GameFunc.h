#pragma once
#include "GlobleFunc.h"

// �����ʼ���ṹ��
struct _INITPARAM
{
	HINSTANCE hInst;			// ���Ӧ�ó���ʵ��
	int nCmdShow;				// �����ڵ���ʾ
	char szScriptFile[256];		// ��ʼ���ű��ļ���
};

// ��Ϸѭ��
bool GameLoop(TLunaEngine::UserLoop pLoop);

// ��Ϸ�������
int MainExampleGame(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmLine, int nCmdShow);

// ��ѭ���ص�����
void ExampleGameLoop(float fTimeElapsed);
// ������Ϣ�ص�����
void OnCatchInputMsg(BYTE yType,void* param);

// ���غ�����
bool InitGame(const _INITPARAM* pInitParam);
void DestroyGame();