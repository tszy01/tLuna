#ifndef _KEYCODEDEF_H_
#define _KEYCODEDEF_H_


// 键盘虚拟码
const unsigned char KEY_ESCAPE		   = 0x01;
const unsigned char KEY_1			   = 0x02;
const unsigned char KEY_2               = 0x03;
const unsigned char KEY_3               = 0x04;
const unsigned char KEY_4               = 0x05;
const unsigned char KEY_5               = 0x06;
const unsigned char KEY_6               = 0x07;
const unsigned char KEY_7               = 0x08;
const unsigned char KEY_8               = 0x09;
const unsigned char KEY_9               = 0x0A;
const unsigned char KEY_0               = 0x0B;
const unsigned char KEY_MINUS           = 0x0C;    /* - on main keyboard */
const unsigned char KEY_EQUALS          = 0x0D;
const unsigned char KEY_BACK            = 0x0E;    /* backspace */
const unsigned char KEY_TAB             = 0x0F;
const unsigned char KEY_Q               = 0x10;
const unsigned char KEY_W               = 0x11;
const unsigned char KEY_E               = 0x12;
const unsigned char KEY_R               = 0x13;
const unsigned char KEY_T               = 0x14;
const unsigned char KEY_Y               = 0x15;
const unsigned char KEY_U               = 0x16;
const unsigned char KEY_I               = 0x17;
const unsigned char KEY_O               = 0x18;
const unsigned char KEY_P               = 0x19;
const unsigned char KEY_LBRACKET        = 0x1A;
const unsigned char KEY_RBRACKET        = 0x1B;
const unsigned char KEY_RETURN          = 0x1C;   /* Enter on main keyboard */
const unsigned char KEY_LCONTROL        = 0x1D;
const unsigned char KEY_A               = 0x1E;
const unsigned char KEY_S               = 0x1F;
const unsigned char KEY_D               = 0x20;
const unsigned char KEY_F               = 0x21;
const unsigned char KEY_G               = 0x22;
const unsigned char KEY_H               = 0x23;
const unsigned char KEY_J               = 0x24;
const unsigned char KEY_K               = 0x25;
const unsigned char KEY_L               = 0x26;
const unsigned char KEY_SEMICOLON       = 0x27;
const unsigned char KEY_APOSTROPHE      = 0x28;
const unsigned char KEY_GRAVE           = 0x29;    /* accent grave */
const unsigned char KEY_LSHIFT          = 0x2A;
const unsigned char KEY_BACKSLASH       = 0x2B;
const unsigned char KEY_Z               = 0x2C;
const unsigned char KEY_X               = 0x2D;
const unsigned char KEY_C               = 0x2E;
const unsigned char KEY_V               = 0x2F;
const unsigned char KEY_B               = 0x30;
const unsigned char KEY_N               = 0x31;
const unsigned char KEY_M               = 0x32;
const unsigned char KEY_COMMA           = 0x33;
const unsigned char KEY_PERIOD          = 0x34;   /* . on main keyboard */
const unsigned char KEY_SLASH           = 0x35;    /* / on main keyboard */
const unsigned char KEY_RSHIFT          = 0x36;
const unsigned char KEY_MULTIPLY        = 0x37;    /* * on numeric keypad */
const unsigned char KEY_LMENU           = 0x38;    /* left Alt */
const unsigned char KEY_SPACE           = 0x39;
const unsigned char KEY_CAPITAL         = 0x3A;
const unsigned char KEY_F1              = 0x3B;
const unsigned char KEY_F2              = 0x3C;
const unsigned char KEY_F3              = 0x3D;
const unsigned char KEY_F4              = 0x3E;
const unsigned char KEY_F5              = 0x3F;
const unsigned char KEY_F6              = 0x40;
const unsigned char KEY_F7              = 0x41;
const unsigned char KEY_F8              = 0x42;
const unsigned char KEY_F9              = 0x43;
const unsigned char KEY_F10             = 0x44;
const unsigned char KEY_NUMLOCK         = 0x45;
const unsigned char KEY_SCROLL          = 0x46;    /* Scroll Lock */
const unsigned char KEY_NUMPAD7         = 0x47;
const unsigned char KEY_NUMPAD8         = 0x48;
const unsigned char KEY_NUMPAD9         = 0x49;
const unsigned char KEY_SUBTRACT        = 0x4A;    /* - on numeric keypad */
const unsigned char KEY_NUMPAD4         = 0x4B;
const unsigned char KEY_NUMPAD5         = 0x4C;
const unsigned char KEY_NUMPAD6         = 0x4D;
const unsigned char KEY_ADD             = 0x4E;    /* + on numeric keypad */
const unsigned char KEY_NUMPAD1         = 0x4F;
const unsigned char KEY_NUMPAD2         = 0x50;
const unsigned char KEY_NUMPAD3         = 0x51;
const unsigned char KEY_NUMPAD0         = 0x52;
const unsigned char KEY_DECIMAL         = 0x53;    /* . on numeric keypad */
const unsigned char KEY_OEM_102         = 0x56;    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
const unsigned char KEY_F11             = 0x57;
const unsigned char KEY_F12             = 0x58;
const unsigned char KEY_F13             = 0x64;    /*                     (NEC PC98) */
const unsigned char KEY_F14             = 0x65;    /*                     (NEC PC98) */
const unsigned char KEY_F15             = 0x66;    /*                     (NEC PC98) */
const unsigned char KEY_KANA            = 0x70;    /* (Japanese keyboard)            */
const unsigned char KEY_ABNT_C1         = 0x73;    /* /? on Brazilian keyboard */
const unsigned char KEY_CONVERT         = 0x79;    /* (Japanese keyboard)            */
const unsigned char KEY_NOCONVERT       = 0x7B;    /* (Japanese keyboard)            */
const unsigned char KEY_YEN             = 0x7D;    /* (Japanese keyboard)            */
const unsigned char KEY_ABNT_C2         = 0x7E;    /* Numpad . on Brazilian keyboard */
const unsigned char KEY_NUMPADEQUALS    = 0x8D;    /* = on numeric keypad (NEC PC98) */
const unsigned char KEY_PREVTRACK       = 0x90;    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
const unsigned char KEY_AT              = 0x91;    /*                     (NEC PC98) */
const unsigned char KEY_COLON           = 0x92;    /*                     (NEC PC98) */
const unsigned char KEY_UNDERLINE       = 0x93;    /*                     (NEC PC98) */
const unsigned char KEY_KANJI           = 0x94;    /* (Japanese keyboard)            */
const unsigned char KEY_STOP            = 0x95;    /*                     (NEC PC98) */
const unsigned char KEY_AX              = 0x96;    /*                     (Japan AX) */
const unsigned char KEY_UNLABELED       = 0x97;    /*                        (J3100) */
const unsigned char KEY_NEXTTRACK       = 0x99;    /* Next Track */
const unsigned char KEY_NUMPADENTER     = 0x9C;    /* Enter on numeric keypad */
const unsigned char KEY_RCONTROL        = 0x9D;
const unsigned char KEY_MUTE            = 0xA0;    /* Mute */
const unsigned char KEY_CALCULATOR      = 0xA1;    /* Calculator */
const unsigned char KEY_PLAYPAUSE       = 0xA2;    /* Play / Pause */
const unsigned char KEY_MEDIASTOP       = 0xA4;    /* Media Stop */
const unsigned char KEY_VOLUMEDOWN      = 0xAE;    /* Volume - */
const unsigned char KEY_VOLUMEUP        = 0xB0;    /* Volume + */
const unsigned char KEY_WEBHOME         = 0xB2;    /* Web home */
const unsigned char KEY_NUMPADCOMMA     = 0xB3;    /* , on numeric keypad (NEC PC98) */
const unsigned char KEY_DIVIDE          = 0xB5;    /* / on numeric keypad */
const unsigned char KEY_SYSRQ           = 0xB7;
const unsigned char KEY_RMENU           = 0xB8;    /* right Alt */
const unsigned char KEY_PAUSE           = 0xC5;    /* Pause */
const unsigned char KEY_HOME            = 0xC7;    /* Home on arrow keypad */
const unsigned char KEY_UP              = 0xC8;    /* UpArrow on arrow keypad */
const unsigned char KEY_PRIOR           = 0xC9;    /* PgUp on arrow keypad */
const unsigned char KEY_LEFT            = 0xCB;   /* LeftArrow on arrow keypad */
const unsigned char KEY_RIGHT           = 0xCD;    /* RightArrow on arrow keypad */
const unsigned char KEY_END             = 0xCF;    /* End on arrow keypad */
const unsigned char KEY_DOWN            = 0xD0;    /* DownArrow on arrow keypad */
const unsigned char KEY_NEXT            = 0xD1;    /* PgDn on arrow keypad */
const unsigned char KEY_INSERT          = 0xD2;    /* Insert on arrow keypad */
const unsigned char KEY_DELETE          = 0xD3;    /* Delete on arrow keypad */
const unsigned char KEY_LWIN            = 0xDB;    /* Left Windows key */
const unsigned char KEY_RWIN            = 0xDC;   /* Right Windows key */
const unsigned char KEY_APPS            = 0xDD;    /* AppMenu key */
const unsigned char KEY_POWER           = 0xDE;    /* System Power */
const unsigned char KEY_SLEEP           = 0xDF;    /* System Sleep */
const unsigned char KEY_WAKE            = 0xE3;    /* System Wake */
const unsigned char KEY_WEBSEARCH       = 0xE5;    /* Web Search */
const unsigned char KEY_WEBFAVORITES    = 0xE6;    /* Web Favorites */
const unsigned char KEY_WEBREFRESH      = 0xE7;    /* Web Refresh */
const unsigned char KEY_WEBSTOP         = 0xE8;    /* Web Stop */
const unsigned char KEY_WEBFORWARD      = 0xE9;    /* Web Forward */
const unsigned char KEY_WEBBACK         = 0xEA;    /* Web Back */
const unsigned char KEY_MYCOMPUTER      = 0xEB;    /* My Computer */
const unsigned char KEY_MAIL            = 0xEC;    /* Mail */
const unsigned char KEY_MEDIASELECT     = 0xED;    /* Media Select */

// 手柄虚拟码
const unsigned char JOYS_BUTTON0		= 0x00;
const unsigned char JOYS_BUTTON1     = 0x01;
const unsigned char JOYS_BUTTON2     = 0x02;
const unsigned char JOYS_BUTTON3     = 0x03;
const unsigned char JOYS_BUTTON4     = 0x04;
const unsigned char JOYS_BUTTON5     = 0x05;
const unsigned char JOYS_BUTTON6     = 0x06;
const unsigned char JOYS_BUTTON7     = 0x07;
const unsigned char JOYS_BUTTON8     = 0x08;
const unsigned char JOYS_BUTTON9     = 0x09;
const unsigned char JOYS_BUTTON10    = 0x0A;
const unsigned char JOYS_BUTTON11    = 0x0B;
const unsigned char JOYS_BUTTON12    = 0x0C;
const unsigned char JOYS_BUTTON13    = 0x0D;
const unsigned char JOYS_BUTTON14    = 0x0E;
const unsigned char JOYS_BUTTON15    = 0x0F;
const unsigned char JOYS_BUTTON16    = 0x10;
const unsigned char JOYS_BUTTON17    = 0x11;
const unsigned char JOYS_BUTTON18    = 0x12;
const unsigned char JOYS_BUTTON19    = 0x13;
const unsigned char JOYS_BUTTON20    = 0x14;
const unsigned char JOYS_BUTTON21    = 0x15;
const unsigned char JOYS_BUTTON22    = 0x16;
const unsigned char JOYS_BUTTON23    = 0x17;
const unsigned char JOYS_BUTTON24    = 0x18;
const unsigned char JOYS_BUTTON25    = 0x19;
const unsigned char JOYS_BUTTON26    = 0x1A;
const unsigned char JOYS_BUTTON27    = 0x1B;
const unsigned char JOYS_BUTTON28    = 0x1C;
const unsigned char JOYS_BUTTON29    = 0x1D;
const unsigned char JOYS_BUTTON30    = 0x1E;
const unsigned char JOYS_BUTTON31    = 0x1F;

// 鼠标虚拟码
const unsigned char MOUSE_BUTTON0 = 0x00;
const unsigned char MOUSE_BUTTON1 = 0x01;
const unsigned char MOUSE_BUTTON2 = 0x02;
const unsigned char MOUSE_BUTTON3 = 0x03;

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
	unsigned char btnCode;
	BUTTON_TYPE btnType;
};

// 鼠标消息结构体
struct _MSG_MOUSE_INFO
{
	long lX;	// X移动
	long lY;	// Y移动
	long lZ;	// 滚轮转动
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
	long lX;	// 左摇杆左右
	long lY;	// 左摇杆前后
	long lZ;	// 右摇杆前后
	//long lRX;	// 无作用
	//long lRY;	// 无作用
	long lRZ;	// 右摇杆左右
	unsigned long rgdwPOV;	// 在开启左摇杆的情况下，四个方向键，0，45，90，135，180，225，270，315
	//long rglSlider[2];	// 无作用
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
typedef void(*ProcessInput)(unsigned char,void*);


#endif