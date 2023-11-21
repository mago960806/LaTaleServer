// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment : 
// Creation : metalgeni 2004-08-16 ¿ÀÈÄ 4:48:46
//***************************************************************************

#pragma once

#define INPUT_PRESS_REPEAT		0x10
#define INPUT_PRESS				0x11

#define INPUT_UNPRESS_REPEAT	0x00
#define INPUT_UNPRESS			0x01

#define INPUT_REPEAT			0x100
#define INPUT_UNREPEAT			0x101




enum SPKeyList {
	SPKL_NULL = 0x00,

	SPKL_BEGIN,

	SPKL_LEFT,
	SPKL_RIGHT,	
	SPKL_UP,
	SPKL_DOWN,
	SPKL_CTRL,
	SPKL_LCTRL,
	SPKL_RCTRL,
	SPKL_SHIFT,
	SPKL_LSHIFT,
	SPKL_RSHIFT,
	SPKL_ALT,
	SPKL_RALT,
	SPKL_LALT,
	SPKL_SPACE,
	SPKL_EQUALS,
	SPKL_BACKSPACE,
	SPKL_ENTER,
	SPKL_INSERT,
	SPKL_DELETE,
	SPKL_PGUP,
	SPKL_PGDN,
	SPKL_HOME,
	SPKL_END,
	SPKL_PLUS,
	SPKL_MINUS,
	SPKL_MULTIPLY,
	SPKL_ESCAPE,
	SPKL_SEMICOLON,
	SPKL_PERIOD,
	SPKL_COMMA,
	SPKL_DIVIDE,	
	SPKL_TAB,
	SPKL_CAPSLOCK,
	SPKL_SUBTRACT,
	SPKL_SLASH,
	SPKL_APOSTROPHE,
	SPKL_GRAVE,	
	SPKL_NUMPAD0,
	SPKL_NUMPAD1,
	SPKL_NUMPAD2,
	SPKL_NUMPAD3,
	SPKL_NUMPAD4,
	SPKL_NUMPAD5,
	SPKL_NUMPAD6,
	SPKL_NUMPAD7,
	SPKL_NUMPAD8,
	SPKL_NUMPAD9,
	SPKL_0,
	SPKL_1,
	SPKL_2,
	SPKL_3,
	SPKL_4,
	SPKL_5,
	SPKL_6,
	SPKL_7,
	SPKL_8,
	SPKL_9,
	SPKL_A,
	SPKL_B,
	SPKL_C,
	SPKL_D,
	SPKL_E,
	SPKL_F,
	SPKL_G,
	SPKL_H,
	SPKL_I,
	SPKL_J,
	SPKL_K,
	SPKL_L,
	SPKL_M,
	SPKL_N,
	SPKL_O,
	SPKL_P,
	SPKL_Q,
	SPKL_R,
	SPKL_S,
	SPKL_T,
	SPKL_U,
	SPKL_V,
	SPKL_W,
	SPKL_X,
	SPKL_Y,
	SPKL_Z,
	SPKL_F1,
	SPKL_F2,
	SPKL_F3,
	SPKL_F4,
	SPKL_F5,
	SPKL_F6,
	SPKL_F7,
	SPKL_F8,
	SPKL_F9,
	SPKL_F10,
	SPKL_F11,
	SPKL_F12,
	SPKL_PREVTRACK, /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
};

struct SPMInputKey {
	int		Key;
	int		InputState;
	//SPMInputKey()
	//	: Key(SPKL_NULL)
	//	, InputState(INPUT_UNPRESS_REPEAT)
	//{}
};


enum SPBtnList {
	SPBL_NULL = 0x00,

	SPBL_LX           ,
	SPBL_LY           ,
	SPBL_LZ           ,
	SPBL_LRX          ,
	SPBL_LRY          ,
	SPBL_LRZ          ,
	SPBL_RGLSLIDER    ,
	SPBL_RGDWPOV      ,
	SPBL_RGBBUTTONS	  ,
	SPBL_LVX          ,
	SPBL_LVY          ,
	SPBL_LVZ          ,
	SPBL_LVRX         ,
	SPBL_LVRY         ,
	SPBL_LVRZ         ,
	SPBL_RGLVSLIDER   ,
	SPBL_LAX          ,
	SPBL_LAY          ,
	SPBL_LAZ          ,
	SPBL_LARX         ,
	SPBL_LARY         ,
	SPBL_LARZ         ,
	SPBL_RGLASLIDER   ,
	SPBL_LFX          ,
	SPBL_LFY          ,
	SPBL_LFZ          ,
	SPBL_LFRX         ,
	SPBL_LFRY         ,
	SPBL_LFRZ         ,
	SPBL_RGLFSLIDER   ,
};


struct SPMInputBtn {
	int		iButton;
	int		iIndex;
	int		iInputState;
	int		iValue;
};



//typedef struct DIJOYSTATE2 {
//	LONG    lX;                     /* x-axis position              */
//	LONG    lY;                     /* y-axis position              */
//	LONG    lZ;                     /* z-axis position              */
//	LONG    lRx;                    /* x-axis rotation              */
//	LONG    lRy;                    /* y-axis rotation              */
//	LONG    lRz;                    /* z-axis rotation              */
//	LONG    rglSlider[2];           /* extra axes positions         */
//	DWORD   rgdwPOV[4];             /* POV directions               */
//	BYTE    rgbButtons[128];        /* 128 buttons                  */
//	LONG    lVX;                    /* x-axis velocity              */
//	LONG    lVY;                    /* y-axis velocity              */
//	LONG    lVZ;                    /* z-axis velocity              */
//	LONG    lVRx;                   /* x-axis angular velocity      */
//	LONG    lVRy;                   /* y-axis angular velocity      */
//	LONG    lVRz;                   /* z-axis angular velocity      */
//	LONG    rglVSlider[2];          /* extra axes velocities        */
//	LONG    lAX;                    /* x-axis acceleration          */
//	LONG    lAY;                    /* y-axis acceleration          */
//	LONG    lAZ;                    /* z-axis acceleration          */
//	LONG    lARx;                   /* x-axis angular acceleration  */
//	LONG    lARy;                   /* y-axis angular acceleration  */
//	LONG    lARz;                   /* z-axis angular acceleration  */
//	LONG    rglASlider[2];          /* extra axes accelerations     */
//	LONG    lFX;                    /* x-axis force                 */
//	LONG    lFY;                    /* y-axis force                 */
//	LONG    lFZ;                    /* z-axis force                 */
//	LONG    lFRx;                   /* x-axis torque                */
//	LONG    lFRy;                   /* y-axis torque                */
//	LONG    lFRz;                   /* z-axis torque                */
//	LONG    rglFSlider[2];          /* extra axes forces            */
//} DIJOYSTATE2, *LPDIJOYSTATE2;



