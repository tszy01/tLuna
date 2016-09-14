#pragma once
#include "TSingleton.h"
#include "TLIGUIListener.h"

/*
 *	��Ϸ���֣�GUI����������
 */
class TLGUISceneMgr : public TLunaEngine::TLIGUIListener
{
T_SINGLETON_DEF(TLGUISceneMgr);
public:
	TLGUISceneMgr(void);
	~TLGUISceneMgr(void);
private:
public:
	// ---------- �ⲿ���÷��� -------------------
	// ��ʼ��
	bool InitGUISceneMgr();
	// ����
	void DestroyGUISceneMgr();
	// ������Ϣ
	void CatchInputMsg(BYTE yType,void* param);
	// ���������ɼ�
	void ShowContainer(int containerID,bool bShow = true);
	// ����ʹ����������
	void UseFont(int iID);
	// ��������
	bool AddFont(const char* guiFontFile);
	// ����GUI��Ⱦ
	bool InitGUIRender(const char* guiRenderFile);
	// ����GUI�ؼ�
	bool InitGUI(const char* guiFile);
public:
	// --------- ������дTLIGUIListener -------------------
	// ����������Ϸ���
	virtual void OnAnimePlayedOver(int iContainerID,BYTE yAnimeType);
};