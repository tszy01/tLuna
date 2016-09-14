#pragma once


namespace TLunaEngine{

	// 键盘虚拟码
	const BYTE KEY_ESCAPE		   = 0x01;
	const BYTE KEY_1			   = 0x02;
	const BYTE KEY_2               = 0x03;
	const BYTE KEY_3               = 0x04;
	const BYTE KEY_4               = 0x05;
	const BYTE KEY_5               = 0x06;
	const BYTE KEY_6               = 0x07;
	const BYTE KEY_7               = 0x08;
	const BYTE KEY_8               = 0x09;
	const BYTE KEY_9               = 0x0A;
	const BYTE KEY_0               = 0x0B;
	const BYTE KEY_MINUS           = 0x0C;    /* - on main keyboard */
	const BYTE KEY_EQUALS          = 0x0D;
	const BYTE KEY_BACK            = 0x0E;    /* backspace */
	const BYTE KEY_TAB             = 0x0F;
	const BYTE KEY_Q               = 0x10;
	const BYTE KEY_W               = 0x11;
	const BYTE KEY_E               = 0x12;
	const BYTE KEY_R               = 0x13;
	const BYTE KEY_T               = 0x14;
	const BYTE KEY_Y               = 0x15;
	const BYTE KEY_U               = 0x16;
	const BYTE KEY_I               = 0x17;
	const BYTE KEY_O               = 0x18;
	const BYTE KEY_P               = 0x19;
	const BYTE KEY_LBRACKET        = 0x1A;
	const BYTE KEY_RBRACKET        = 0x1B;
	const BYTE KEY_RETURN          = 0x1C;   /* Enter on main keyboard */
	const BYTE KEY_LCONTROL        = 0x1D;
	const BYTE KEY_A               = 0x1E;
	const BYTE KEY_S               = 0x1F;
	const BYTE KEY_D               = 0x20;
	const BYTE KEY_F               = 0x21;
	const BYTE KEY_G               = 0x22;
	const BYTE KEY_H               = 0x23;
	const BYTE KEY_J               = 0x24;
	const BYTE KEY_K               = 0x25;
	const BYTE KEY_L               = 0x26;
	const BYTE KEY_SEMICOLON       = 0x27;
	const BYTE KEY_APOSTROPHE      = 0x28;
	const BYTE KEY_GRAVE           = 0x29;    /* accent grave */
	const BYTE KEY_LSHIFT          = 0x2A;
	const BYTE KEY_BACKSLASH       = 0x2B;
	const BYTE KEY_Z               = 0x2C;
	const BYTE KEY_X               = 0x2D;
	const BYTE KEY_C               = 0x2E;
	const BYTE KEY_V               = 0x2F;
	const BYTE KEY_B               = 0x30;
	const BYTE KEY_N               = 0x31;
	const BYTE KEY_M               = 0x32;
	const BYTE KEY_COMMA           = 0x33;
	const BYTE KEY_PERIOD          = 0x34;   /* . on main keyboard */
	const BYTE KEY_SLASH           = 0x35;    /* / on main keyboard */
	const BYTE KEY_RSHIFT          = 0x36;
	const BYTE KEY_MULTIPLY        = 0x37;    /* * on numeric keypad */
	const BYTE KEY_LMENU           = 0x38;    /* left Alt */
	const BYTE KEY_SPACE           = 0x39;
	const BYTE KEY_CAPITAL         = 0x3A;
	const BYTE KEY_F1              = 0x3B;
	const BYTE KEY_F2              = 0x3C;
	const BYTE KEY_F3              = 0x3D;
	const BYTE KEY_F4              = 0x3E;
	const BYTE KEY_F5              = 0x3F;
	const BYTE KEY_F6              = 0x40;
	const BYTE KEY_F7              = 0x41;
	const BYTE KEY_F8              = 0x42;
	const BYTE KEY_F9              = 0x43;
	const BYTE KEY_F10             = 0x44;
	const BYTE KEY_NUMLOCK         = 0x45;
	const BYTE KEY_SCROLL          = 0x46;    /* Scroll Lock */
	const BYTE KEY_NUMPAD7         = 0x47;
	const BYTE KEY_NUMPAD8         = 0x48;
	const BYTE KEY_NUMPAD9         = 0x49;
	const BYTE KEY_SUBTRACT        = 0x4A;    /* - on numeric keypad */
	const BYTE KEY_NUMPAD4         = 0x4B;
	const BYTE KEY_NUMPAD5         = 0x4C;
	const BYTE KEY_NUMPAD6         = 0x4D;
	const BYTE KEY_ADD             = 0x4E;    /* + on numeric keypad */
	const BYTE KEY_NUMPAD1         = 0x4F;
	const BYTE KEY_NUMPAD2         = 0x50;
	const BYTE KEY_NUMPAD3         = 0x51;
	const BYTE KEY_NUMPAD0         = 0x52;
	const BYTE KEY_DECIMAL         = 0x53;    /* . on numeric keypad */
	const BYTE KEY_OEM_102         = 0x56;    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
	const BYTE KEY_F11             = 0x57;
	const BYTE KEY_F12             = 0x58;
	const BYTE KEY_F13             = 0x64;    /*                     (NEC PC98) */
	const BYTE KEY_F14             = 0x65;    /*                     (NEC PC98) */
	const BYTE KEY_F15             = 0x66;    /*                     (NEC PC98) */
	const BYTE KEY_KANA            = 0x70;    /* (Japanese keyboard)            */
	const BYTE KEY_ABNT_C1         = 0x73;    /* /? on Brazilian keyboard */
	const BYTE KEY_CONVERT         = 0x79;    /* (Japanese keyboard)            */
	const BYTE KEY_NOCONVERT       = 0x7B;    /* (Japanese keyboard)            */
	const BYTE KEY_YEN             = 0x7D;    /* (Japanese keyboard)            */
	const BYTE KEY_ABNT_C2         = 0x7E;    /* Numpad . on Brazilian keyboard */
	const BYTE KEY_NUMPADEQUALS    = 0x8D;    /* = on numeric keypad (NEC PC98) */
	const BYTE KEY_PREVTRACK       = 0x90;    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
	const BYTE KEY_AT              = 0x91;    /*                     (NEC PC98) */
	const BYTE KEY_COLON           = 0x92;    /*                     (NEC PC98) */
	const BYTE KEY_UNDERLINE       = 0x93;    /*                     (NEC PC98) */
	const BYTE KEY_KANJI           = 0x94;    /* (Japanese keyboard)            */
	const BYTE KEY_STOP            = 0x95;    /*                     (NEC PC98) */
	const BYTE KEY_AX              = 0x96;    /*                     (Japan AX) */
	const BYTE KEY_UNLABELED       = 0x97;    /*                        (J3100) */
	const BYTE KEY_NEXTTRACK       = 0x99;    /* Next Track */
	const BYTE KEY_NUMPADENTER     = 0x9C;    /* Enter on numeric keypad */
	const BYTE KEY_RCONTROL        = 0x9D;
	const BYTE KEY_MUTE            = 0xA0;    /* Mute */
	const BYTE KEY_CALCULATOR      = 0xA1;    /* Calculator */
	const BYTE KEY_PLAYPAUSE       = 0xA2;    /* Play / Pause */
	const BYTE KEY_MEDIASTOP       = 0xA4;    /* Media Stop */
	const BYTE KEY_VOLUMEDOWN      = 0xAE;    /* Volume - */
	const BYTE KEY_VOLUMEUP        = 0xB0;    /* Volume + */
	const BYTE KEY_WEBHOME         = 0xB2;    /* Web home */
	const BYTE KEY_NUMPADCOMMA     = 0xB3;    /* , on numeric keypad (NEC PC98) */
	const BYTE KEY_DIVIDE          = 0xB5;    /* / on numeric keypad */
	const BYTE KEY_SYSRQ           = 0xB7;
	const BYTE KEY_RMENU           = 0xB8;    /* right Alt */
	const BYTE KEY_PAUSE           = 0xC5;    /* Pause */
	const BYTE KEY_HOME            = 0xC7;    /* Home on arrow keypad */
	const BYTE KEY_UP              = 0xC8;    /* UpArrow on arrow keypad */
	const BYTE KEY_PRIOR           = 0xC9;    /* PgUp on arrow keypad */
	const BYTE KEY_LEFT            = 0xCB;   /* LeftArrow on arrow keypad */
	const BYTE KEY_RIGHT           = 0xCD;    /* RightArrow on arrow keypad */
	const BYTE KEY_END             = 0xCF;    /* End on arrow keypad */
	const BYTE KEY_DOWN            = 0xD0;    /* DownArrow on arrow keypad */
	const BYTE KEY_NEXT            = 0xD1;    /* PgDn on arrow keypad */
	const BYTE KEY_INSERT          = 0xD2;    /* Insert on arrow keypad */
	const BYTE KEY_DELETE          = 0xD3;    /* Delete on arrow keypad */
	const BYTE KEY_LWIN            = 0xDB;    /* Left Windows key */
	const BYTE KEY_RWIN            = 0xDC;   /* Right Windows key */
	const BYTE KEY_APPS            = 0xDD;    /* AppMenu key */
	const BYTE KEY_POWER           = 0xDE;    /* System Power */
	const BYTE KEY_SLEEP           = 0xDF;    /* System Sleep */
	const BYTE KEY_WAKE            = 0xE3;    /* System Wake */
	const BYTE KEY_WEBSEARCH       = 0xE5;    /* Web Search */
	const BYTE KEY_WEBFAVORITES    = 0xE6;    /* Web Favorites */
	const BYTE KEY_WEBREFRESH      = 0xE7;    /* Web Refresh */
	const BYTE KEY_WEBSTOP         = 0xE8;    /* Web Stop */
	const BYTE KEY_WEBFORWARD      = 0xE9;    /* Web Forward */
	const BYTE KEY_WEBBACK         = 0xEA;    /* Web Back */
	const BYTE KEY_MYCOMPUTER      = 0xEB;    /* My Computer */
	const BYTE KEY_MAIL            = 0xEC;    /* Mail */
	const BYTE KEY_MEDIASELECT     = 0xED;    /* Media Select */

	// 手柄虚拟码
	const BYTE JOYS_BUTTON0		= 0x00;
	const BYTE JOYS_BUTTON1     = 0x01;
	const BYTE JOYS_BUTTON2     = 0x02;
	const BYTE JOYS_BUTTON3     = 0x03;
	const BYTE JOYS_BUTTON4     = 0x04;
	const BYTE JOYS_BUTTON5     = 0x05;
	const BYTE JOYS_BUTTON6     = 0x06;
	const BYTE JOYS_BUTTON7     = 0x07;
	const BYTE JOYS_BUTTON8     = 0x08;
	const BYTE JOYS_BUTTON9     = 0x09;
	const BYTE JOYS_BUTTON10    = 0x0A;
	const BYTE JOYS_BUTTON11    = 0x0B;
	const BYTE JOYS_BUTTON12    = 0x0C;
	const BYTE JOYS_BUTTON13    = 0x0D;
	const BYTE JOYS_BUTTON14    = 0x0E;
	const BYTE JOYS_BUTTON15    = 0x0F;
	const BYTE JOYS_BUTTON16    = 0x10;
	const BYTE JOYS_BUTTON17    = 0x11;
	const BYTE JOYS_BUTTON18    = 0x12;
	const BYTE JOYS_BUTTON19    = 0x13;
	const BYTE JOYS_BUTTON20    = 0x14;
	const BYTE JOYS_BUTTON21    = 0x15;
	const BYTE JOYS_BUTTON22    = 0x16;
	const BYTE JOYS_BUTTON23    = 0x17;
	const BYTE JOYS_BUTTON24    = 0x18;
	const BYTE JOYS_BUTTON25    = 0x19;
	const BYTE JOYS_BUTTON26    = 0x1A;
	const BYTE JOYS_BUTTON27    = 0x1B;
	const BYTE JOYS_BUTTON28    = 0x1C;
	const BYTE JOYS_BUTTON29    = 0x1D;
	const BYTE JOYS_BUTTON30    = 0x1E;
	const BYTE JOYS_BUTTON31    = 0x1F;

	// 鼠标虚拟码
	const BYTE MOUSE_BUTTON0 = 0x00;
	const BYTE MOUSE_BUTTON1 = 0x01;
	const BYTE MOUSE_BUTTON2 = 0x02;
	const BYTE MOUSE_BUTTON3 = 0x03;

	// 按键状态枚举
	enum BUTTON_TYPE
	{
		BUTTON_TYPE_NONE	= 0,	// 没有改变
		BUTTON_TYPE_DOWN	= 1,	// 按下
		BUTTON_TYPE_UP		= 2,	// 按下后抬起
	};

	// 按键与状态的组合
	struct _BUTTON_INFO
	{
		BYTE btnCode;
		BUTTON_TYPE btnType;
	};

	// 鼠标消息结构体
	struct _MSG_MOUSE_INFO
	{
		LONG lX;	// X移动
		LONG lY;	// Y移动
		LONG lZ;	// 滚轮转动
		_BUTTON_INFO btnInfo[4];	// 最多4个键
	};

	// 按键消息结构体
	struct _MSG_KEY_INFO
	{
		_BUTTON_INFO btnInfo[256];	// 键盘有256个键
	};

	// 手柄消息结构体
	struct _MSG_JOYS_INFO
	{
		LONG lX;	// 左摇杆左右
		LONG lY;	// 左摇杆前后
		LONG lZ;	// 右摇杆前后
		//LONG lRX;	// 无作用
		//LONG lRY;	// 无作用
		LONG lRZ;	// 右摇杆左右
		DWORD rgdwPOV;	// 在开启左摇杆的情况下，四个方向键，0，45，90，135，180，225，270，315
		//LONG rglSlider[2];	// 无作用
		_BUTTON_INFO btnInfo[32];	// 最多32个按键
	};

	// 消息类型
	enum MSG_TYPE
	{
		MSG_TYPE_NONE	= 0,
		MSG_TYPE_KEY	= 1,	// 键盘消息
		MSG_TYPE_MOUSE	= 2,	// 鼠标消息
		MSG_TYPE_JOYS	= 3,	// 手柄消息
	};

	// 定义函数指针
	typedef void(*ProcessInput)(BYTE,void*);

}