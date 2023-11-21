// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment : 
// Creation : metalgeni 2004-08-16 ¿ÀÈÄ 4:48:46
//***************************************************************************

#pragma once

#define DIKS_SIZE 256

struct SPMInputKey;

class SPInput {

public:
	SPInput();
	~SPInput();
	
	HRESULT Init(HWND hWnd, DWORD dwCoopFlags = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	void FreeDirectInput();

	UINT GetMousePosX() { return m_uiMousePosX; }
	UINT GetMousePosY() { return m_uiMousePosY; }

	bool IsEquipJoystick() { return (m_pJoystick != NULL); }

	bool WndProc(UINT message, WPARAM wParam, LPARAM lParam);

	void SetKeyTable();

	HRESULT ReadImmediateData();	
	bool IsKey(int KeyCode, BYTE* pEvent = NULL);	
	bool KeyMsg(int KeyCode, SPMInputKey& IK);
	bool JSButtonMsg(int ButtonCode, SPMInputBtn& IB, int iIndex = 0);


	bool ConvertProcessKey(unsigned int* puiVirKeyCode);
	void UpdateControlKeyState();
	void KeyDown( int iVKCode, int iRepeatCount = 1 );
	void KeyUp( int iVKCode, int iRepeatCount = 1 );
	bool GetKeyState( int iSPKLCode )	{ return m_bKeyStates[ m_dwVKTable	[iSPKLCode]  ]; }
	
	LPCSTR GetStateLog()	{ return m_strNewText.c_str(); }
	LPCSTR GetStateDown()	{ return m_strKeyDown.c_str(); }
	LPCSTR GetStateUp()		{ return m_strKeyUp.c_str(); }
	LPCSTR GetStateLogJoystick()	{ return m_strLogJoyStick.c_str(); }

	// added joystick
	HRESULT InitJoyStick(HWND hWnd);
	BOOL CreateDevice(GUID guid);
	BOOL SetProperty(const DIDEVICEOBJECTINSTANCE* pdidoi);
	HRESULT ReadImmediateDataJoyStick();



protected:

	BYTE	m_DIKS			[ DIKS_SIZE ];
	BYTE	m_DIKSOld		[ DIKS_SIZE ];
	BYTE	m_DIKSState		[ DIKS_SIZE ];

	bool	m_bKeyStates	[ DIKS_SIZE ];
	bool	m_bKeyStatesOld	[ DIKS_SIZE ];

	DWORD	m_dwDIKSTable	[ DIKS_SIZE ];
	DWORD	m_dwVKTable		[ DIKS_SIZE ];

	DIJOYSTATE2 m_JS2;		// DInput Joystick state 
	DIJOYSTATE2 m_JS2Old;

	UINT	m_uiMousePosX;
	UINT	m_uiMousePosY;

	HWND	m_hWndMainWindow;

	LPDIRECTINPUT8       m_pDI;			// The DirectInput object
	LPDIRECTINPUTDEVICE8 m_pKeyboard;	// The keyboard device 
	LPDIRECTINPUTDEVICE8 m_pJoystick;	// The Joystick device 

	// Debug Log
	std::string		m_strNewText;
	std::string		m_strKeyDown;
	std::string		m_strKeyUp;
	std::string		m_strLogJoyStick;

};

extern SPInput* g_pInput;


