// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 오전 10:26:19
//***************************************************************************

#pragma once

#define WIID_LOGGIN_TITLE		(WIID_LOGIN+1)		// Title

#define WIID_LOGGIN_BG			(WIID_LOGIN+101)		//배경
#define WIID_LOGGIN_EDIT_ID		(WIID_LOGIN+102)
#define WIID_LOGGIN_EDIT_PW		(WIID_LOGIN+103)
#define WIID_LOGGIN_CONFIRM		(WIID_LOGIN+104)	
#define WIID_LOGGIN_EXIT		(WIID_LOGIN+105)	
#define WIID_HOMEPAGE			(WIID_LOGIN+106)
#define WIID_NEW_ACCOUNT		(WIID_LOGIN+107)
#define WIID_PASS_REVISION		(WIID_LOGIN+108)
#define WIID_PASS_FIND			(WIID_LOGIN+109)
#define WIID_LOGGIN_SAVE_ID		(WIID_LOGIN+110)		//로그인 ID Check Button
#define WIID_LOGGIN_SOFTKEY		(WIID_LOGIN+111)		//SoftKeyboard Dialog
#define WIID_LOGGIN_IDC			(WIID_LOGIN+112)

#define WIID_LOGGIN_SOFTKEYDLG	(WIID_LOGIN+113)

#define WIID_LOGGIN_EK_BG		(WIID_LOGIN+201)		//CopyRight 포함
#define WIID_LOGGIN_EK_INBG		(WIID_LOGIN+202)
#define WIID_LOGGIN_EK_CHGBG	(WIID_LOGIN+203)
#define WIID_LOGGIN_EK_IDKEY	(WIID_LOGIN+204)
#define WIID_LOGGIN_EK_CHGKEY	(WIID_LOGIN+205)
#define WIID_LOGIN_EK_COMMIT	(WIID_LOGIN+206)
#define WIID_LOGIN_EK_CANCEL	(WIID_LOGIN+207)

#define WIID_LOGGIN_SERIAL_BG			(WIID_LOGIN+301)		//CopyRight 포함
#define WIID_LOGGIN_SERIAL_INBG			(WIID_LOGIN+302)
#define WIID_LOGGIN_SERIAL_EDIT			(WIID_LOGIN+303)
#define WIID_LOGGIN_SERIAL_COMMIT		(WIID_LOGIN+304)
#define WIID_LOGGIN_SERIAL_CANCEL		(WIID_LOGIN+305)

enum LOGIN_STATE {
	LOGIN_DIALOG_NULL,
	LOGIN_DIALOG_WEBARG,			// Web Login (No Dialog)
	LOGIN_DIALOG_NORMAL,			// ID, Password Dialog
	LOGIN_DIALOG_EKEY,			// E-key Dialog
	LOGIN_DIALOG_SERIAL,			// Serial Dialog
	LOGIN_DIALOG_MAX,
} ;

class SPWindow;
class SPWindowEdit;
class SPWindowLogin : public SPWindow {

public:
	SPWindowLogin(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowLogin(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowLogin();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual int PerformMouseMove(int iX, int iY) ;

public:
	bool	SetEnableLoginDialog();
	bool	CloseLoginDialog() ;
	bool	SetEnableEKeyDialog(int iEKeyType, string strChallengeCode);
	bool	CloseEkeyDialog();
	bool	SetEnableSerialDialog() ;
	bool	CloseSerialDialog() ;

	void	CloseSoftKeyboard() ;
	void	PressEnterKey() ;

	// For SoftKeyboard
	SPWindowEdit*	GetIDEditWindow()	{	return m_pWindowEditID;	}
	SPWindowEdit*	GetPWEditWindow()	{	return m_pWindowEditPW;	}
	SPWindowEdit*	GetLastFocusEdit()	{	return m_pLastFocusEdit;	}
	int				GetCaretStart()		{	return m_iCaretStart;	}
	int				GetCaretEnd()		{	return m_iCaretEnd;		}
	bool			GetIsShowSoftKey()	{	return m_bShowSoftKey;	}

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnCursorEnter	( WPARAM, LPARAM );
	virtual int OnCursorOut		( WPARAM, LPARAM );
	virtual int OnLButtonDown	( WPARAM, LPARAM );
	virtual int OnLButtonUp		( WPARAM, LPARAM );
	virtual int OnKeyReturn		( WPARAM, LPARAM );
	virtual int OnEscape		( WPARAM, LPARAM );
	virtual int OnExit			( WPARAM, LPARAM );
	virtual int OnUpdateID		( WPARAM, LPARAM );		//ID Edit 에서 문자열 갱신시(종종 체크를 해제할때가 있다)
	virtual int OnSaveCheck		( WPARAM, LPARAM );		//체크 버튼 클릭시 이벤트
	virtual int OnSoftKeyboard	( WPARAM, LPARAM );		//SoftKeyboard
	virtual int OnNoticeYes		( WPARAM, LPARAM );		//Notice Yes

	int OnHomePage(WPARAM wParam, LPARAM lParam);	
	int OnNewAccount(WPARAM wParam, LPARAM lParam);
	int OnPasswordRevision(WPARAM wParam, LPARAM lParam);
	int OnPasswordFind(WPARAM wParam, LPARAM lParam);
	int OnEditFocus( unsigned int iID, WPARAM wParam, LPARAM lParam);
	int OnEditFocus(WPARAM wParam, LPARAM lParam);

	int OnEkeyCommit(WPARAM wParam, LPARAM lParam);
	int OnEkeyCencel(WPARAM wParam, LPARAM lParam);

	int OnSerialCommit(WPARAM wParam, LPARAM lParam);
	int OnSerialCencel(WPARAM wParam, LPARAM lParam);

	int OnIDC( WPARAM wParam , LPARAM lParam );
	// }}

protected:
	void InitLoginDialog() ;
	void InitEkeyDialog() ;
	void InitEkeySerialDialog() ;
	LOGIN_STATE	GetShowState() ;
	void SetShowState(LOGIN_STATE iState) ;
	//bool Confirm();
	
protected:
	bool			m_bIsEKEY;				// 현재 Ekey 상태인가
	bool			m_bSaveID;				/**< ID 저장 Flag <br> */
	bool			m_bShowSoftKey;			// SoftKey Show

	std::string		m_strLastID;			/**< 마지막으로 저장되어 있는 ID <br> */
	SPWindowEdit*	m_pWindowEditID ;
	SPWindowEdit*	m_pWindowEditPW ;
	SPWindowEdit*	m_pLastFocusEdit ;
	int				m_iCaretStart ;
	int				m_iCaretEnd ;
	LOGIN_STATE		m_iShowState ;

	SPWindow*		m_pLoginIDC;

};




