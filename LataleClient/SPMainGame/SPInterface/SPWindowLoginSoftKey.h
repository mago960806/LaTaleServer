// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPWindowLoginVKeyboard.h
// Comment     : 
// Creation    : metalgeni 2004-08-08 오전 10:26:19
//***************************************************************************

#pragma once

#define WIID_LOGIN_SOFTKEY_UPPERBG		(WIID_SOFTKEY+1)
#define WIID_LOGIN_SOFTKEY_LOWERBG		(WIID_SOFTKEY+2)

#define WIID_LOGIN_SOFTKEY_NUM			(WIID_SOFTKEY+11)		// 1 ~ 0 (10)

#define WIID_LOGIN_SOFTKEY_UPPERKEY		(WIID_SOFTKEY+31)		// A ~ Z (26)
#define WIID_LOGIN_SOFTKEY_LOWERKEY		(WIID_SOFTKEY+61)		// a ~ z (26)

#define WIID_LOGIN_SOFTKEY_LSHIFT		(WIID_SOFTKEY+91)
#define WIID_LOGIN_SOFTKEY_RSHIFT		(WIID_SOFTKEY+93)
#define WIID_LOGIN_SOFTKEY_CAPS			(WIID_SOFTKEY+95)
#define WIID_LOGIN_SOFTKEY_BACK			(WIID_SOFTKEY+97)
#define WIID_LOGIN_SOFTKEY_ENTER		(WIID_SOFTKEY+99)
#define WIID_LOGIN_SOFTKEY_CLOSE		(WIID_SOFTKEY+101)
#define WIID_LOGIN_SOFTKEY_CAPS_LAMP	(WIID_SOFTKEY+103)


enum SOFTKEY_PRESS_TYPE
{
	PRESS_SOFTKEY = 0,
	PRESS_SOFTBACK,
	PRESS_SOFTENTER,
	PRESS_SOFTCLOSE,
};

class SPWindow;
class SPWindowVKeyboard : public SPWindow {

public:
	SPWindowVKeyboard(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowVKeyboard(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowVKeyboard();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Show(bool bWithChild = true);


public:
	bool	GetIsUpper()					{	return m_bIsUpper;			}


protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnCursorEnter	( WPARAM, LPARAM );
	virtual int OnCursorOut		( WPARAM, LPARAM );

	virtual int OnNumKeyButtonUp	(unsigned int uiID, WPARAM, LPARAM );
	virtual int OnUpKeyButtonUp	( unsigned int uiID, WPARAM, LPARAM );
	virtual int OnLoKeyButtonUp	( unsigned int uiID, WPARAM, LPARAM );
	virtual int OnFuncShiftUp	( unsigned int uiID, WPARAM, LPARAM );
	virtual int OnFuncCapsUp	( unsigned int uiID, WPARAM, LPARAM );
	virtual int OnFuncButtonUp	( unsigned int uiID, WPARAM, LPARAM );
	// }}

protected:
	void AlterUpperLiterial(bool bUpper);
	void AlterCapsKey(bool bToogle) ;
	void AlterShiftKey(bool bToogle) ;
	void PrintLogs(const TCHAR* fmt, ...);

	
protected:
	bool	m_bChangeFont ;	// Shift나 Caps가 눌렸는지 확인
	bool	m_bShift ;		// Shift선택시 - 1회
	bool	m_bCaps ;		// Caps 선택시 - 지속
	bool	m_bIsUpper ;	// 현재 대문자 인지여부
	char	m_strKey[62] ;	// 10 + 26 + 26

};




