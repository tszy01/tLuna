#ifndef _GUISCENEMGR_H_
#define _GUISCENEMGR_H_
#include "TLSingleton.h"
#include "TLGUIListener.h"

/*
 *	��Ϸ���֣�GUI����������
 */
class GUISceneMgr : public TLunaEngine::Singleton<GUISceneMgr>, public TLunaEngine::GUIListener
{
	friend class  TLunaEngine::Singleton<GUISceneMgr>;
protected:
	GUISceneMgr(void);
	~GUISceneMgr(void);
private:
public:
	// ---------- �ⲿ���÷��� -------------------
	// ��ʼ��
	bool InitGUISceneMgr();
	// ����
	void DestroyGUISceneMgr();
	// ������Ϣ
	void CatchInputMsg(unsigned char yType, void* param);
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
	virtual void OnAnimePlayedOver(int iContainerID, TLunaEngine::TUByte yAnimeType);
};

#endif