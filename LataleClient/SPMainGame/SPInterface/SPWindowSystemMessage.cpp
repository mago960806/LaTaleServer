// ***************************************************************
//  SPWindowSystemMessage   version:  1.0   ¡¤  date: 07/09/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindowListDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowList.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"

#include "SPWindowSystemMessage.h"

SPWindowSystemMessage::SPWindowSystemMessage( WND_ID WndClassID , INSTANCE_ID InstanceID )
: SPWindow( WndClassID , InstanceID )
{	
	Init();	
}

SPWindowSystemMessage::SPWindowSystemMessage( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent /* = NULL  */ )
: SPWindow( WNDID_SYSTEM_MESSAGE , InstanceID , iX , iY , iCX , iCY , pParent )
{	
	Init();	
}

SPWindowSystemMessage::~SPWindowSystemMessage()
{
	Clean();
}

void SPWindowSystemMessage::Init()
{
	m_strToolTip	= "[SPWindowSystemMessage]";

	SPWindowButton* pButton			=	NULL;
	SPWindow*		pWindow			=	NULL;
	SPWindow*		pWindowParent	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	SPWindow*		pWindowMinimized	=	new SPWindowStatic( WIID_SYSTEM_MESSAGE_MINIMIZE , 0 , 0 , 28 , 18 , this );

	pWindowParent	= new SPWindowStatic( WIID_SYSTEM_MESSAGE_MINIMIZE_BG , 0 , 0 , 28 , 18 , pWindowMinimized );
	pWindowParent->SetImage		("DATA/INTERFACE/CONCEPT/UI106.PNG", 331 , 16 );

	pButton = new SPWindowButton( WIID_SYSTEM_MESSAGE_MINIMIZE_TO_MAXIMIZE , 12 , 2 , 14 , 14 , pWindowParent );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI106.PNG", 304 , 222 );
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI106.PNG", 319 , 222 );
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI106.PNG", 304 , 237 );
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI106.PNG", 319 , 237 );

	//////////////////////////////////////////////////////////////////////////

	SPWindow*		pWindowMaxinized	=	new SPWindowStatic( WIID_SYSTEM_MESSAGE_MAXIMIZE , 0 , 0 , 286 , 80 , this );

	//////////////////////////////////////////////////////////////////////////

	pWindow	= new SPWindowStatic( WIID_SYSTEM_MESSAGE_MAXIMIZE_TITLE_LEFT , 0 , 0 , 3 , 18 , pWindowMaxinized );
	pWindow->SetImage		("DATA/INTERFACE/CONCEPT/UI107.PNG", 501 , 164 );
	pWindow->SetImage		( (SPTexture*)NULL );

	pWindow	= new SPWindowStatic( WIID_SYSTEM_MESSAGE_MAXIMIZE_TITLE_RIGHT , 283 , 0 , 3 , 18 , pWindowMaxinized );
	pWindow->SetImage		("DATA/INTERFACE/CONCEPT/UI107.PNG", 508 , 164 );
	pWindow->SetImage		( (SPTexture*)NULL );

	pWindowParent	= new SPWindowStatic( WIID_SYSTEM_MESSAGE_MAXIMIZE_TITLE_CENTER , 3 , 0 , 280 , 18 , pWindowMaxinized );
	pWindowParent->SetImage		("DATA/INTERFACE/CONCEPT/UI107.PNG", 505 , 164 );
	pWindowParent->SetImage		( (SPTexture*)NULL );
	pWindowParent->SetSrcSize( 2 , 18 );

	//////////////////////////////////////////////////////////////////////////

	pWindow	= new SPWindowStatic( WIID_SYSTEM_MESSAGE_MAXIMIZE_TITLE_IMG , 6 - 3 , 7 , 80 , 5 , pWindowParent );
	pWindow->SetImage		("DATA/INTERFACE/CONCEPT/UI106.PNG", 171 , 207 );
	pWindow->SetImage		( (SPTexture*)NULL );

	pButton = new SPWindowButton( WIID_SYSTEM_MESSAGE_MAXIMIZE_TO_MINIMIZE , 268 - 3 , 2 , 14 , 14 , pWindowParent );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI106.PNG", 301 , 16 );
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI106.PNG", 316 , 16 );
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI106.PNG", 301 , 31 );
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI106.PNG", 316 , 31 );

	//////////////////////////////////////////////////////////////////////////

	pWindowParent	= new SPWindowStatic( WIID_SYSTEM_MESSAGE_MAXIMIZE_WINDOW_TOP , 0 , 18 , 286 , 59 , pWindowMaxinized );
	pWindowParent->SetImage		("DATA/INTERFACE/CONCEPT/UI107.PNG", 186 , 2 );
	pWindowParent->SetImage		( (SPTexture*)NULL );
	pWindowParent->SetSrcSize( 286 , 3 );

	pWindow	= new SPWindowStatic( WIID_SYSTEM_MESSAGE_MAXIMIZE_WINDOW_BOTTOM , 0 , 77 , 286 , 3 , pWindowMaxinized );
	pWindow->SetImage			("DATA/INTERFACE/CONCEPT/UI107.PNG", 186 , 10 );
	pWindow->SetImage			( (SPTexture*)NULL );

	//////////////////////////////////////////////////////////////////////////

	pButton = new SPWindowButton( WIID_SYSTEM_MESSAGE_MAXIMIZE_UP , 10 , 42 - 18 , 12 , 11 , pWindowParent );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI100.PNG", 460 , 38 );
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI100.PNG", 473 , 38 );
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI100.PNG", 486 , 38 );
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI100.PNG", 499 , 38 );
	pButton = NULL;

	pButton = new SPWindowButton( WIID_SYSTEM_MESSAGE_MAXIMIZE_DOWN , 10 , 54 - 18 , 12 , 11 , pWindowParent );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI100.PNG", 460 , 51 );
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI100.PNG", 473 , 51 );
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI100.PNG", 486 , 51 );
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI100.PNG", 499 , 51 );
	pButton = NULL;

	m_pBtnLatest = new SPWindowButton( WIID_SYSTEM_MESSAGE_MAXIMIZE_LATEST , 10 , 66 - 18 , 12 , 11 , pWindowParent );
	m_pBtnLatest->SetImage			("DATA/INTERFACE/CONCEPT/UI100.PNG", 460 , 64 );
	m_pBtnLatest->SetImageHit		("DATA/INTERFACE/CONCEPT/UI100.PNG", 473 , 64 );
	m_pBtnLatest->SetImagePush		("DATA/INTERFACE/CONCEPT/UI100.PNG", 486 , 64 );
	m_pBtnLatest->SetImageDisable	("DATA/INTERFACE/CONCEPT/UI100.PNG", 499 , 64 );

	//	History	Panel
	m_pHistory = new SPWindowList( WIID_SYSTEM_MESSAGE_MAXIMIZE_HISTORY , 27 , 22 - 18 , 255 , 55 , pWindowParent );
	m_pHistory->SetFontShadow(true) ;
	m_pHistory->SetFontColor( D3DXCOLOR( 1.0f,  1.0f , 1.0f , 1.0f ) );
	m_pHistory->SetMargin( 0 , 0 , 0 , 0 );
	m_pHistory->SetMultiLine( TRUE , 2 );
	m_pHistory->SetWheelUse( true );

	//////////////////////////////////////////////////////////////////////////

	m_bInsertChatMsg		=	FALSE;
	m_fBlinkLimitTime		=	0.5f;
	m_fBlinkAccmulateTime	=	0.0f;
	m_bBtnLatestStatus		=	false;

	m_iBtnDown				=	0;
	m_fDownLimitTime		=	0.1f;
	m_fDownAccmulateTime	=	0.0f;

	m_iInitPosX				=	m_iAX;
	m_iInitPosY				=	m_iAY;

	//////////////////////////////////////////////////////////////////////////

	UpdateWindow( false );
}

void SPWindowSystemMessage::Clean()
{
	SPWindow::Clean();
}

void SPWindowSystemMessage::Process(float fTime)
{
	if( !m_bShow )	return;

	SPWindow::Process( fTime );

	ProcessBlink( fTime );
}

void SPWindowSystemMessage::Render(float fTime)
{
	if( !m_bShow )	return;

	g_pVideo->Flush();

	SPWindow::Render( fTime );

	g_pVideo->Flush();
}

void SPWindowSystemMessage::Show(bool bWithChild)
{
	SPWindow::Show( bWithChild );

	UpdateWindow( m_bMinimized );
}

void SPWindowSystemMessage::SetMinimized( bool bMinimized )
{
	UpdateWindow( bMinimized );
}

bool SPWindowSystemMessage::GetMinimized()
{
	return m_bMinimized;
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowSystemMessage )
	SPIMESSAGE_COMMAND(	SPIM_SYSTEM_MESSAGE_ADD														,	OnSystemMessage			)
	SPIMESSAGE_COMMAND(	SPIM_PURGE																	,	OnPurge					)

	SPIMESSAGE_COMMAND( SPIM_CURSOR_ENTER															,	OnCursor				)			
	SPIMESSAGE_COMMAND( SPIM_CURSOR_OUT																,	OnCursor				)			

	SPIMESSAGE_COMMAND( SPIM_WHEEL_UP																,	OnWheelUp				)			
	SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN																,	OnWheelDown				)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_SYSTEM_MESSAGE_MINIMIZE_TO_MAXIMIZE		,	OnMininize				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_SYSTEM_MESSAGE_MAXIMIZE_TO_MINIMIZE		,	OnMaximize				)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_SYSTEM_MESSAGE_MAXIMIZE_UP					,	OnScrollUp				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_SYSTEM_MESSAGE_MAXIMIZE_DOWN				,	OnScrollDown			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_SYSTEM_MESSAGE_MAXIMIZE_LATEST				,	OnScrollLatest			)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN		,	WIID_SYSTEM_MESSAGE_MAXIMIZE_UP					,	OnScrollUpLBDown		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN		,	WIID_SYSTEM_MESSAGE_MAXIMIZE_DOWN				,	OnScrollDownLBDown		)

	SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT			,	WIID_SYSTEM_MESSAGE_MAXIMIZE_UP					,	OnScrollUpCursorOut		)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT			,	WIID_SYSTEM_MESSAGE_MAXIMIZE_DOWN				,	OnScrollDownCursorOut	)

	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER	,	WIID_SYSTEM_MESSAGE_START , WIID_SYSTEM_MESSAGE_END	,	OnCursor			)
	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT	,	WIID_SYSTEM_MESSAGE_START , WIID_SYSTEM_MESSAGE_END	,	OnCursor			)

SPIMESSAGE_OBJ_MAP_END(	SPWindow	)

int SPWindowSystemMessage::OnSystemMessage( WPARAM wParam, LPARAM lParam )
{
	D3DXCOLOR	cWhiteColor		=	D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
	D3DXCOLOR	cDefaultColor	=	cWhiteColor;
	D3DXCOLOR*	pColor			=	(D3DXCOLOR*)lParam;

	if( pColor != NULL )
	{
		if( pColor->r == 0.0f && pColor->g == 0.0f && pColor->b == 0.0f )
		{
			*pColor	=	cWhiteColor;
		}

		cDefaultColor	=	*pColor;
	}

	AddString( (const char*)wParam , cDefaultColor );

	return 1;
}

int SPWindowSystemMessage::OnMininize( WPARAM wParam, LPARAM lParam)
{
	UpdateWindow( false );

	return 1;
}

int SPWindowSystemMessage::OnMaximize( WPARAM wParam, LPARAM lParam)
{
	if( g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL )
		UpdateWindow( true );

	return 1;
}

int SPWindowSystemMessage::OnScrollUp( WPARAM wParam, LPARAM lParam)
{
	m_pHistory->SPSendMessage (SPIM_SCROLL , SUB_SPIM_SCROLL_UP );
	m_iBtnDown	=	0;
	return 1;
}

int SPWindowSystemMessage::OnScrollDown( WPARAM wParam, LPARAM lParam)
{
	m_pHistory->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_DOWN );
	m_iBtnDown	=	0;

	CheckLatestMsg();

	return 1;
}

int SPWindowSystemMessage::OnScrollLatest( WPARAM wParam, LPARAM lParam)
{
	m_pHistory->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_LATEST );

	CheckLatestMsg();

	return 1;
}

int SPWindowSystemMessage::OnScrollUpLBDown( WPARAM wParam, LPARAM lParam )
{
	m_iBtnDown	=	1;

	return 1;
}

int SPWindowSystemMessage::OnScrollDownLBDown( WPARAM wParam, LPARAM lParam )
{
	m_iBtnDown	=	2;

	return 1;
}

int SPWindowSystemMessage::OnScrollUpCursorOut( WPARAM wParam, LPARAM lParam )
{
	m_iBtnDown	=	0;

	return 1;
}

int SPWindowSystemMessage::OnScrollDownCursorOut( WPARAM wParam, LPARAM lParam )
{
	m_iBtnDown	=	0;

	return 1;
}

int SPWindowSystemMessage::OnPurge( WPARAM wParam, LPARAM lParam )
{
	m_pHistory->SPSendMessage( SPIM_PURGE );
	SetAbsCoord( m_iInitPosX , m_iInitPosY );
	RefreshChildPos();
	UpdateWindow( false );
	return 1;
}

int SPWindowSystemMessage::OnCursor( WPARAM wParam, LPARAM lParam )
{
	SPWindow*	pWindow;
	int	iImageIndex = 1;

	if( IsCursorIn( LOWORD( lParam ) , HIWORD( lParam ) ) == true )
	{
		iImageIndex	=	0;

		pWindow	=	Find( WIID_SYSTEM_MESSAGE_MAXIMIZE_TO_MINIMIZE , true );

		if( pWindow != NULL )
			pWindow->Show();
	}
	else
	{
		pWindow	=	Find( WIID_SYSTEM_MESSAGE_MAXIMIZE_TO_MINIMIZE , true );

		if( pWindow != NULL )
			pWindow->Hide();
	}

	pWindow	=	Find( WIID_SYSTEM_MESSAGE_MAXIMIZE_TITLE_LEFT , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iImageIndex );

	pWindow	=	Find( WIID_SYSTEM_MESSAGE_MAXIMIZE_TITLE_CENTER , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iImageIndex );

	pWindow	=	Find( WIID_SYSTEM_MESSAGE_MAXIMIZE_TITLE_RIGHT , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iImageIndex );

	pWindow	=	Find( WIID_SYSTEM_MESSAGE_MAXIMIZE_TITLE_IMG , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iImageIndex );

	pWindow	=	Find( WIID_SYSTEM_MESSAGE_MAXIMIZE_WINDOW_TOP , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iImageIndex );

	pWindow	=	Find( WIID_SYSTEM_MESSAGE_MAXIMIZE_WINDOW_BOTTOM , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iImageIndex );

	return 1;
}

int SPWindowSystemMessage::OnCursor( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnCursor( wParam , lParam );
}

int SPWindowSystemMessage::OnWheelUp( WPARAM wParam, LPARAM lParam )
{
	OnScrollUp( NULL , NULL );

	return 1;
}

int SPWindowSystemMessage::OnWheelDown( WPARAM wParam, LPARAM lParam )
{
	OnScrollDown( NULL , NULL );

	return 1;
}

void SPWindowSystemMessage::ProcessBlink( float fTime )
{
	if( m_bInsertChatMsg == TRUE )
	{
		m_fBlinkAccmulateTime	+=	fTime;

		if( m_fBlinkAccmulateTime >= m_fBlinkLimitTime )
		{
			m_fBlinkAccmulateTime	=	0.0f;
			m_bBtnLatestStatus		=	!m_bBtnLatestStatus;
			m_pBtnLatest->SPSendMessage( SPIM_BTN_SELECT , m_bBtnLatestStatus );
		}
	}

	if( m_iBtnDown > 0 )
	{
		m_fDownAccmulateTime	+=	fTime;

		if( m_fDownAccmulateTime >= m_fDownLimitTime )
		{
			m_fDownAccmulateTime	=	0.0f;

			BtnPush( m_iBtnDown );
		}
	}
}

void SPWindowSystemMessage::CheckLatestMsg()
{
	if( m_pHistory->IsAddString() == true )
		m_bInsertChatMsg	=	TRUE;
	else
	{
		m_bInsertChatMsg	=	FALSE;
		m_pBtnLatest->SPSendMessage( SPIM_BTN_SELECT , false );
	}
}

void SPWindowSystemMessage::BtnPush( int iBtnDown )
{
	switch( iBtnDown )
	{
	case 1:
		m_pHistory->SPSendMessage (SPIM_SCROLL , SUB_SPIM_SCROLL_UP );
		break;
	case 2:
		m_pHistory->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_DOWN );
		CheckLatestMsg();
		break;
	}
}

void SPWindowSystemMessage::AddString( const char* pstrChat , D3DXCOLOR cColor )
{
	if( pstrChat == NULL )
		return;

	if( strlen( pstrChat ) <= 0 )
		return;

	LIST_ITEM	stAddListItem;

	stAddListItem.m_strText		=	pstrChat;
	stAddListItem.m_stTextColor	=	cColor;

	m_pHistory->InsertItem( &stAddListItem );

	CheckLatestMsg();
}

void SPWindowSystemMessage::UpdateWindow( bool bMinimized )
{
	SPWindow*	pWindowMinimize	=	Find( WIID_SYSTEM_MESSAGE_MINIMIZE );
	SPWindow*	pWindowMaximize	=	Find( WIID_SYSTEM_MESSAGE_MAXIMIZE );

	if( pWindowMinimize == NULL || pWindowMaximize == NULL )
		return;

	m_bMinimized = bMinimized;
	RECT	rcSize;

	if( m_bMinimized == true )
	{
		pWindowMinimize->Show();
		pWindowMaximize->Hide();

		pWindowMinimize->GetWindowRect( rcSize );
	}
	else
	{
		pWindowMaximize->Show();
		pWindowMinimize->Hide();

		pWindowMaximize->GetWindowRect( rcSize );
	}

	SetRectSize( rcSize.right - rcSize.left , rcSize.bottom - rcSize.top );

	OnCursor( 0 , MAKELONG( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() ) );
}
