// ***************************************************************
//  SPWindowSystemMessage   version:  1.0   ¡¤  date: 07/09/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

#define WIID_SYSTEM_MESSAGE_MINIMIZE				(WIID_SYSTEM_MESSAGE + 10)
#define WIID_SYSTEM_MESSAGE_MINIMIZE_BG				(WIID_SYSTEM_MESSAGE + 11)
#define WIID_SYSTEM_MESSAGE_MINIMIZE_TO_MAXIMIZE	(WIID_SYSTEM_MESSAGE + 12)

#define WIID_SYSTEM_MESSAGE_MAXIMIZE				(WIID_SYSTEM_MESSAGE + 20)
#define WIID_SYSTEM_MESSAGE_MAXIMIZE_TITLE_LEFT		(WIID_SYSTEM_MESSAGE + 21)
#define WIID_SYSTEM_MESSAGE_MAXIMIZE_TITLE_CENTER	(WIID_SYSTEM_MESSAGE + 22)
#define WIID_SYSTEM_MESSAGE_MAXIMIZE_TITLE_RIGHT	(WIID_SYSTEM_MESSAGE + 23)
#define WIID_SYSTEM_MESSAGE_MAXIMIZE_TITLE_IMG		(WIID_SYSTEM_MESSAGE + 24)

#define WIID_SYSTEM_MESSAGE_MAXIMIZE_WINDOW_TOP		(WIID_SYSTEM_MESSAGE + 25)
#define WIID_SYSTEM_MESSAGE_MAXIMIZE_WINDOW_BOTTOM	(WIID_SYSTEM_MESSAGE + 26)

#define WIID_SYSTEM_MESSAGE_MAXIMIZE_TO_MINIMIZE	(WIID_SYSTEM_MESSAGE + 31)

#define WIID_SYSTEM_MESSAGE_MAXIMIZE_UP				(WIID_SYSTEM_MESSAGE + 32)
#define WIID_SYSTEM_MESSAGE_MAXIMIZE_DOWN			(WIID_SYSTEM_MESSAGE + 33)
#define WIID_SYSTEM_MESSAGE_MAXIMIZE_LATEST			(WIID_SYSTEM_MESSAGE + 34)

#define WIID_SYSTEM_MESSAGE_MAXIMIZE_HISTORY		(WIID_SYSTEM_MESSAGE + 200)

#define WIID_SYSTEM_MESSAGE_START					WIID_SYSTEM_MESSAGE_MINIMIZE
#define WIID_SYSTEM_MESSAGE_END						WIID_SYSTEM_MESSAGE_MAXIMIZE_HISTORY

#define SYSTEM_MESSAGE_BG_COUNT					2

class SPWindow;
class SPWindowButton;
class SPWindowList;

class SPWindowSystemMessage : public SPWindow
{
public:
	SPWindowSystemMessage					( WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowSystemMessage					( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowSystemMessage			();

	virtual void	Init					();
	virtual void	Clean					();

	virtual void	Process					( float fTime );
	virtual void	Render					( float fTime );

	virtual void	Show					( bool bWithChild = true );

	void			SetMinimized			( bool bMinimized );
	bool			GetMinimized			();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	virtual int		OnSystemMessage			( WPARAM wParam, LPARAM lParam );

	virtual int		OnMininize				( WPARAM wParam, LPARAM lParam );
	virtual int		OnMaximize				( WPARAM wParam, LPARAM lParam );

	virtual int		OnScrollUp				( WPARAM wParam, LPARAM lParam );
	virtual int		OnScrollDown			( WPARAM wParam, LPARAM lParam );
	virtual int		OnScrollLatest			( WPARAM wParam, LPARAM lParam );

	virtual int		OnScrollUpLBDown		( WPARAM wParam, LPARAM lParam );
	virtual int		OnScrollDownLBDown		( WPARAM wParam, LPARAM lParam );

	virtual int		OnScrollUpCursorOut		( WPARAM wParam, LPARAM lParam );
	virtual int		OnScrollDownCursorOut	( WPARAM wParam, LPARAM lParam );
	
	virtual int		OnPurge					( WPARAM wParam, LPARAM lParam );

	virtual int		OnCursor				( WPARAM wParam, LPARAM lParam );
	virtual	int		OnCursor				( unsigned int iID, WPARAM wParam , LPARAM lParam );


	virtual int		OnWheelUp				( WPARAM wParam, LPARAM lParam );
	virtual int		OnWheelDown				( WPARAM wParam, LPARAM lParam );

	// }}

protected:

	void			ProcessBlink			( float fTime );

	void			CheckLatestMsg			();

	void			BtnPush					( int iBtnDown );

	void			AddString				( const char* pstrChat , D3DXCOLOR cColor );

	void			UpdateWindow			( bool bMinimized );


	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//

	SPWindowList*					m_pHistory;

	BOOL							m_bInsertChatMsg;
	float							m_fBlinkLimitTime;
	float							m_fBlinkAccmulateTime;
	SPWindowButton*					m_pBtnLatest;
	bool							m_bBtnLatestStatus;

	int								m_iBtnDown;
	float							m_fDownLimitTime;
	float							m_fDownAccmulateTime;

	bool							m_bMinimized;

	int								m_iInitPosX;
	int								m_iInitPosY;

};
