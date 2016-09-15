#include "GameInput.h"

GameInput* TLunaEngine::Singleton<GameInput>::m_Ptr = 0;

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	HRESULT hr;

    // Obtain an interface to the enumerated joystick.
	LPDIRECTINPUTDEVICE8* pJoy = GameInput::getSingletonPtr()->GetJoystickPtr();
	hr = GameInput::getSingletonPtr()->GetDInput()->CreateDevice(pdidInstance->guidInstance,  
                                pJoy, NULL);
    if(FAILED(hr)) 
	{
		MessageBoxA(NULL,"初始化手柄失败","ERROR",NULL);
		PostQuitMessage(0);
		return FALSE;
	}

	return TRUE;
}

BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                   VOID* pContext )
{
    HWND hDlg = (HWND)pContext;

    static int nSliderCount = 0;  // Number of returned slider controls
    static int nPOVCount = 0;     // Number of returned POV controls

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 
    
        // Set the range for the axis
		if( FAILED( GameInput::getSingletonPtr()->GetJoystick()->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
            return DIENUM_STOP;
         
    }
	return TRUE;
}

GameInput::GameInput(void) : 
m_pDInput(NULL),
m_pDInputKB(NULL),
m_pDInputMouse(NULL),
m_pDInputJoystick(NULL),
m_bUseJoystick(false)
{
}

GameInput::~GameInput(void)
{
	DestroyInput();
}

void GameInput::DestroyInput()
{
	if(m_pDInputKB)
		m_pDInputKB->Unacquire();
	if (m_pDInputKB)
	{
		m_pDInputKB->Release();
		m_pDInputKB = 0;
	}
	if(m_pDInputMouse)
		m_pDInputMouse->Unacquire();
	if (m_pDInputMouse)
	{
		m_pDInputMouse->Release();
		m_pDInputMouse = 0;
	}
	if(m_pDInputJoystick)
		m_pDInputJoystick->Unacquire();
	if (m_pDInputJoystick)
	{
		m_pDInputJoystick->Release();
		m_pDInputJoystick = 0;
	}
	if (m_pDInput)
	{
		m_pDInput->Release();
		m_pDInput = 0;
	}
}

bool GameInput::InitInput(HWND hWnd, HINSTANCE hInst,bool bUseJoystick)
{
	//创建DX8.0设备，第一个是实例句炳，第2个是解释设备，第3个是ID号。第四个是锁定到设备指针上
	if(DI_OK!=DirectInput8Create(hInst,DIRECTINPUT_VERSION,IID_IDirectInput8,(LPVOID*)&m_pDInput,NULL))
	{
		MessageBoxA(hWnd,"创建DirectInput 对象失败!","ERROR",MB_ICONERROR|MB_OK);
		return false;
	}
	//创建键盘设备
	if(DI_OK!=m_pDInput->CreateDevice(GUID_SysKeyboard,&m_pDInputKB,NULL))
	{
		MessageBoxA(hWnd,"创建键盘设备失败!","ERROR",MB_ICONERROR|MB_OK);
		return false;
	}
	//设置数据格式		
	if(DI_OK!=	m_pDInputKB->SetDataFormat(&c_dfDIKeyboard)) 
	{
		MessageBoxA(hWnd,"设置键盘数据格式失败!","ERROR",MB_ICONERROR|MB_OK); 
		return false;
	}

	//如果失败，获取数据时再尝试获取
	m_pDInputKB->SetCooperativeLevel(hWnd,DISCL_NONEXCLUSIVE|DISCL_FOREGROUND); 
	m_pDInputKB->Acquire();
	//清除键盘状态
	memset(&m_strKeyState,0,sizeof(m_strKeyState));		
	memset(&m_strKeyStateOld,0,sizeof(m_strKeyState));

	//创建鼠标设备
	if(DI_OK!=m_pDInput->CreateDevice(GUID_SysMouse,&m_pDInputMouse,NULL))
	{
		MessageBoxA(hWnd,"创建鼠标设备失败!","ERROR",MB_ICONERROR|MB_OK);
		return false;
	}
	//设置数据格式
	if(DI_OK!=	m_pDInputMouse->SetDataFormat(&c_dfDIMouse)) 
	{
		MessageBoxA(hWnd,"设置鼠标数据格式失败!","ERROR",MB_ICONERROR|MB_OK); 
		return false;
	}

	m_pDInputMouse->SetCooperativeLevel(hWnd,DISCL_NONEXCLUSIVE|DISCL_FOREGROUND); 
	m_pDInputMouse->Acquire();	//如果失败，获取数据时再尝试获取

	//清除鼠标状态
	memset(&m_MouseState,0,sizeof(m_MouseState));		
	memset(&m_MouseStateOld,0,sizeof(m_MouseState));

	// 创建手柄
	m_bUseJoystick=bUseJoystick;
	if(m_bUseJoystick)
	{
		m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL,(LPDIENUMDEVICESCALLBACKW)EnumJoysticksCallback,NULL, DIEDFL_ATTACHEDONLY);
		if (FAILED(m_pDInputJoystick->SetDataFormat(&c_dfDIJoystick2)))
			return false;
		if (FAILED(m_pDInputJoystick->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
			return false;
		if (FAILED(m_pDInputJoystick->EnumObjects(EnumObjectsCallback, (VOID*)hWnd, DIDFT_AXIS)))
			return false;
		m_pDInputJoystick->Poll();
		m_pDInputJoystick->Acquire();
		//清除手柄状态
		memset(&m_JoyState,0,sizeof(m_JoyState));		
		memset(&m_JoyStateOld,0,sizeof(m_JoyState));
	}
	return true;
}

int GameInput::IsKeyDown(BYTE keycode)
{
	if(keycode<0x100)
		return (0x80 & keycode);
	else
		return 0;
}

int GameInput::IsMouseDown(BYTE i)
{
	if(i<=7 && i>=0)
		return (0x80 & i);
	else
		return 0;
}

int GameInput::IsJoystickDown(BYTE i)
{
	if(!m_bUseJoystick)
		return 0;
	if(i<=127 && i>=0)
		return (0x80 & i);
	else
		return 0;
}

LRESULT GameInput::UpdateKeyboard(ProcessInput pFunc)
{
	// 把当前键盘信息，拷贝到老的键盘信息
	memcpy(m_strKeyStateOld,m_strKeyState,sizeof(m_strKeyState));
	if(DI_OK!=m_pDInputKB->GetDeviceState(sizeof(m_strKeyState),m_strKeyState))
	{	
		memset(&m_strKeyState,0,sizeof(m_strKeyState));	//清除上一次的状态
		m_pDInputKB->Acquire();
		return E_FAIL;
	}
	// 是否发送
	bool bSend = false;
	// 遍历键盘消息生成结构体发送
	_MSG_KEY_INFO keyInfo;
	for(int i=0;i<256;i++)
	{
		keyInfo.btnInfo[i].btnCode = m_strKeyState[i];
		keyInfo.btnInfo[i].btnType = BUTTON_TYPE_NONE;
		// 如果是按下就直接生成消息
		if(IsKeyDown(m_strKeyState[i]))
		{
			keyInfo.btnInfo[i].btnType = BUTTON_TYPE_DOWN;
			bSend = true;
		}
		else if(!IsKeyDown(m_strKeyState[i]) && IsKeyDown(m_strKeyStateOld[i]))
		{
			keyInfo.btnInfo[i].btnType = BUTTON_TYPE_UP;
			bSend = true;
		}
	}
	// 发送键盘消息
	if(pFunc && bSend)
	{
		pFunc((BYTE)MSG_TYPE_KEY,&keyInfo);
	}
	return S_OK;
}

LRESULT GameInput::UpdateMouse(ProcessInput pFunc)
{
	bool bSend = false;
	// 把当前鼠标信息，拷贝到老的鼠标信息
	memcpy(&m_MouseStateOld,&m_MouseState,sizeof(m_MouseState));  
	if(DI_OK!=m_pDInputMouse->GetDeviceState(sizeof(m_MouseState),&m_MouseState))
	{	
		memset(&m_MouseState,0,sizeof(m_MouseState));	//清除上一次的状态
		m_pDInputMouse->Acquire();
		return E_FAIL;
	}
	// 鼠标消息
	_MSG_MOUSE_INFO mouseInfo;
	// 遍历4个按键
	for(int i=0;i<4;i++)
	{
		mouseInfo.btnInfo[i].btnCode = m_MouseState.rgbButtons[i];
		mouseInfo.btnInfo[i].btnType = BUTTON_TYPE_NONE;
		// 如果是按下就直接生成消息
		if(IsMouseDown(m_MouseState.rgbButtons[i]))
		{
			mouseInfo.btnInfo[i].btnType = BUTTON_TYPE_DOWN;
			bSend = true;
		}
		else if(!IsMouseDown(m_MouseState.rgbButtons[i]) && IsMouseDown(m_MouseStateOld.rgbButtons[i]))
		{
			mouseInfo.btnInfo[i].btnType = BUTTON_TYPE_UP;
			bSend = true;
		}
	}
	// 轴数值
	mouseInfo.lX = m_MouseState.lX;
	mouseInfo.lY = m_MouseState.lY;
	mouseInfo.lZ = m_MouseState.lZ;
	if(mouseInfo.lX!=0 || mouseInfo.lY!=0 || mouseInfo.lZ!=0)
		bSend = true;
	// 发送鼠标消息
	if(pFunc && bSend)
	{
		pFunc((BYTE)MSG_TYPE_MOUSE,&mouseInfo);
	}
	return S_OK;
}

LRESULT GameInput::UpdateJoystick(ProcessInput pFunc)
{
	bool bSend = false;
	memcpy(&m_JoyStateOld,&m_JoyState,sizeof(m_JoyState)); 
	//m_pDInputJoystick->Acquire();
	if(FAILED(m_pDInputJoystick->Poll()))
	{
		HRESULT hr = m_pDInputJoystick->Acquire();
		while( hr == DIERR_INPUTLOST )
			hr = m_pDInputJoystick->Acquire();
	}
	if(DI_OK!=m_pDInputJoystick->GetDeviceState(sizeof(m_JoyState),&m_JoyState))
	{	
		memset(&m_JoyState,0,sizeof(m_JoyState));	//清除上一次的状态
		m_JoyState.rgdwPOV[0] = -1;
		m_pDInputJoystick->Acquire();
		return E_FAIL;
	}
	// 手柄消息
	_MSG_JOYS_INFO joysInfo;
	// 遍历32个键
	for(int i=0;i<32;i++)
	{
		joysInfo.btnInfo[i].btnCode = m_JoyState.rgbButtons[i];
		joysInfo.btnInfo[i].btnType = BUTTON_TYPE_NONE;
		// 如果是按下就直接生成消息
		if(IsJoystickDown(m_JoyState.rgbButtons[i]))
		{
			joysInfo.btnInfo[i].btnType = BUTTON_TYPE_DOWN;
			bSend = true;
		}
		else if(!IsJoystickDown(m_JoyState.rgbButtons[i]) && IsJoystickDown(m_JoyStateOld.rgbButtons[i]))
		{
			joysInfo.btnInfo[i].btnType = BUTTON_TYPE_UP;
			bSend = true;
		}
	}
	// 左摇杆，右摇杆
	joysInfo.lX = m_JoyState.lX;
	joysInfo.lY = m_JoyState.lY;
	joysInfo.lZ = m_JoyState.lZ;
	joysInfo.lRZ = m_JoyState.lRz;
	if(joysInfo.lX!=0 || joysInfo.lY!=0 || joysInfo.lZ!=0 || joysInfo.lRZ!=0)
		bSend = true;
	// 如果左摇杆开启，四个方向键的实时数据
	joysInfo.rgdwPOV = m_JoyState.rgdwPOV[0];
	if(m_JoyState.rgdwPOV[0] != (DWORD)-1)
	{
		bSend = true;
	}
	else if(m_JoyState.rgdwPOV[0] == (DWORD)-1 && (DWORD)-1 != m_JoyStateOld.rgdwPOV[0])
	{
		bSend = true;
	}
	// 发送手柄消息
	if(pFunc && bSend)
	{
		pFunc((BYTE)MSG_TYPE_JOYS,&joysInfo);
	}
	return S_OK;
}

LRESULT GameInput::UpdateInput(ProcessInput pFunc)
{
	UpdateKeyboard(pFunc);
	UpdateMouse(pFunc);
	// 手柄
	if(m_bUseJoystick)
	{
		UpdateJoystick(pFunc);
	}
  	return DI_OK;
}