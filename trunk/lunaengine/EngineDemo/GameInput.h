#ifndef _GAMEINPUT_H_
#define _GAMEINPUT_H_
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "KeyCodeDef.h"
#include "TLSingleton.h"

class GameInput : public TLunaEngine::Singleton<GameInput>
{
	friend class TLunaEngine::Singleton<GameInput>;
private:
	//INPUT�豸ָ��
	LPDIRECTINPUT8			m_pDInput;
	//���̵��豸ָ��
	LPDIRECTINPUTDEVICE8	m_pDInputKB;
	//���ָ��
	LPDIRECTINPUTDEVICE8	m_pDInputMouse;
	//�ֱ��豸
	LPDIRECTINPUTDEVICE8	m_pDInputJoystick;
	//����ϵ�����
	DIMOUSESTATE			m_MouseStateOld;
	//��굱ʱ������
	DIMOUSESTATE			m_MouseState;
	//�ֱ������
	DIJOYSTATE2				m_JoyStateOld;
	//�ֱ���ʱ����
	DIJOYSTATE2				m_JoyState;
	//���̵�ʱ����
	char	m_strKeyState[256];
	//����������
	char	m_strKeyStateOld[256]; 

	bool m_bUseJoystick;	// �Ƿ������ֱ�
public:
	void DestroyInput();
	//�������X��Y,Z
	inline LONG GetMouseX() {return m_MouseState.lX;};
	inline LONG GetMouseY() {return m_MouseState.lY;};
	inline LONG GetMouseZ() {return m_MouseState.lZ;};
	//��þɵ������Ϣ
	inline LONG GetOldMouseX() {return m_MouseStateOld.lX;};
	inline LONG GetOldMouseY() {return m_MouseStateOld.lY;};
	inline LONG GetOldMouseZ() {return m_MouseStateOld.lZ;};
	//��ʼ���豸,���ھ������ʵ�����
	bool InitInput(HWND hWnd,HINSTANCE hInst,bool bUseJoystick);
	//��ü��̰��µ���Ϣ
	int IsKeyDown(BYTE keycode);
	// �����갴�µ���Ϣ
	int IsMouseDown(BYTE i);
	// �ֱ�������
	int IsJoystickDown(BYTE i);
	inline LONG GetJoystickX(){return m_bUseJoystick ? m_JoyState.lX : 0;}
	inline LONG GetJoystickY(){return m_bUseJoystick ? m_JoyState.lY : 0;}
	//����INPUT
	LRESULT UpdateInput(ProcessInput pFunc);
	LRESULT UpdateKeyboard(ProcessInput pFunc);
	LRESULT UpdateMouse(ProcessInput pFunc);
	LRESULT UpdateJoystick(ProcessInput pFunc);

	inline bool IsUseJoystick(){return m_bUseJoystick;}
	inline LPDIRECTINPUT8 GetDInput(){return m_pDInput;}
	inline LPDIRECTINPUTDEVICE8 GetJoystick(){return m_pDInputJoystick;}
	inline LPDIRECTINPUTDEVICE8* GetJoystickPtr(){return &m_pDInputJoystick;}
protected:
	GameInput(void);
	~GameInput(void);
};

#endif