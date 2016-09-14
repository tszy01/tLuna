#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "KeyCodeDef.h"
#include "TSingleton.h"
using namespace TLunaEngine;
using namespace TLunaEngine;

class TGameInput
{
T_SINGLETON_DEF(TGameInput);
private:
	//INPUT设备指针
	LPDIRECTINPUT8			m_pDInput;
	//键盘的设备指针
	LPDIRECTINPUTDEVICE8	m_pDInputKB;
	//鼠标指针
	LPDIRECTINPUTDEVICE8	m_pDInputMouse;
	//手柄设备
	LPDIRECTINPUTDEVICE8	m_pDInputJoystick;
	//鼠标老的数据
	DIMOUSESTATE			m_MouseStateOld;
	//鼠标当时的数据
	DIMOUSESTATE			m_MouseState;
	//手柄老鼠据
	DIJOYSTATE2				m_JoyStateOld;
	//手柄当时数据
	DIJOYSTATE2				m_JoyState;
	//键盘当时数据
	char	m_strKeyState[256];
	//键盘老数据
	char	m_strKeyStateOld[256]; 

	bool m_bUseJoystick;	// 是否适用手柄
public:
	TGameInput(void);
	void DestroyInput();
	//获得鼠标的X，Y,Z
	inline LONG GetMouseX() {return m_MouseState.lX;};
	inline LONG GetMouseY() {return m_MouseState.lY;};
	inline LONG GetMouseZ() {return m_MouseState.lZ;};
	//获得旧的鼠标信息
	inline LONG GetOldMouseX() {return m_MouseStateOld.lX;};
	inline LONG GetOldMouseY() {return m_MouseStateOld.lY;};
	inline LONG GetOldMouseZ() {return m_MouseStateOld.lZ;};
	//初始化设备,窗口句炳，和实例句炳
	bool InitInput(HWND hWnd,HINSTANCE hInst,bool bUseJoystick);
	//获得键盘按下的信息
	int IsKeyDown(BYTE keycode);
	// 获得鼠标按下的消息
	int IsMouseDown(BYTE i);
	// 手柄键按下
	int IsJoystickDown(BYTE i);
	inline LONG GetJoystickX(){return m_bUseJoystick ? m_JoyState.lX : 0;}
	inline LONG GetJoystickY(){return m_bUseJoystick ? m_JoyState.lY : 0;}
	//更新INPUT
	LRESULT UpdateInput(ProcessInput pFunc);
	LRESULT UpdateKeyboard(ProcessInput pFunc);
	LRESULT UpdateMouse(ProcessInput pFunc);
	LRESULT UpdateJoystick(ProcessInput pFunc);

	inline bool IsUseJoystick(){return m_bUseJoystick;}
	inline LPDIRECTINPUT8 GetDInput(){return m_pDInput;}
	inline LPDIRECTINPUTDEVICE8 GetJoystick(){return m_pDInputJoystick;}
	inline LPDIRECTINPUTDEVICE8* GetJoystickPtr(){return &m_pDInputJoystick;}
public:
	~TGameInput(void);
};