// ***************************************************************
//  SPWindowChattingPassword   version:  1.0   ¡¤  date: 04/04/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"

#include "SPLocalizeManager.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPWindowChattingPassword.h"

SPWindowChattingPassword::SPWindowChattingPassword( WND_ID WndClassID , INSTANCE_ID InstanceID )
: SPWindow( WndClassID , InstanceID )
{	
	Init();	
}

SPWindowChattingPassword::SPWindowChattingPassword( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent /* = NULL  */ )
: SPWindow( WNDID_CHATTING_PASSWORD , InstanceID , iX , iY , iCX , iCY , pParent )
{	
	Init();	
}

SPWindowChattingPassword::~SPWindowChattingPassword()
{
	Clean();
}

void SPWindowChattingPassword::Init()
{
	m_strToolTip	= "[SPWindowChattingPassword]";

	//////////////////////////////////////////////////////////////////////////

	SPWindowButton*			pWindowButton;
	SPWindowStatic*			pWindowStatic;
	SPWindowStatic*			pWindowStaticBG;
	SPWindowEdit*			pWindowEdit;

	//////////////////////////////////////////////////////////////////////////

	pWindowStaticBG	=	new SPWindowStatic( WIID_CHATTING_PASSWORD_BG , 0 , 0 , m_iSX , m_iSY , this );
	pWindowStaticBG->SetWindowType( WND_TYPE_NINEBASE );
	pWindowStaticBG->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 497 , 467 , 4 , 1 );

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_PASSWORD_CLOSE , 205 , 11 , 12 , 12 , pWindowStaticBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 53 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 53 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 66 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 66 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_PASSWORD_SUBJECT , 17 , 10 , 180 , 12 , pWindowStaticBG );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 2 , 47 , 117 ) );
	pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_BOLD );
	pWindowStatic->SetWindowText( "ÀÏÀÌ»ï»ç¿ÀÀ°Ä¥ÆÈ±¸½ÊÀÏÀÌ»ï»ç¿ÀÀ°Ä¥ÆÈ±¸½ÊÀÏÀÌ»ï»ç¿ÀÀ°Ä¥ÆÈ±¸½ÊÀÏÀÌ»ï»ç¿ÀÀ°Ä¥ÆÈ±¸½Ê" );
	pWindowStatic	=	NULL;

	pWindowEdit	=	new SPWindowEdit( WIID_CHATTING_PASSWORD_EDIT , 18 , 31 , 118 , 16 , pWindowStaticBG );
	pWindowEdit->SetWindowType( WND_TYPE_LEFTBASE );
	pWindowEdit->SetMargin( 19 , 2 );
	pWindowEdit->SetImageNormalHead	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 316 , 384 , 332 );
	pWindowEdit->SetImageNormalBody	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 316 , 388 , 332 );
	pWindowEdit->SetImageNormalTail	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 316 , 392 , 332 );
	pWindowEdit->SetImageFocusHead	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 316 , 384 , 332 );
	pWindowEdit->SetImageFocusBody	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 316 , 388 , 332 );
	pWindowEdit->SetImageFocusTail	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 316 , 392 , 332 );
	//	pWindowEdit->SetImageDeco		( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 501 , 501 , 511 , 511 );
	pWindowEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowEdit->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowEdit->SetLimitText( MAX_CHATROOM_PASSWORD_LEN );
	pWindowEdit->SetPassword( true );
	pWindowEdit->SetUseIME( false );

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_PASSWORD_OK , 140 , 30 , 68 , 17 , pWindowStaticBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 73 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 91 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 109 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 127 );
	pWindowButton	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	m_bUse			=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_CHATTINGROOM ) ;
	m_pWindowSender	=	NULL;

	//////////////////////////////////////////////////////////////////////////
}

void SPWindowChattingPassword::Clean()
{
	SPWindow::Clean();
}

void SPWindowChattingPassword::Process(float fTime)
{
	if( !m_bShow )	return;

	SPWindow::Process( fTime );
}

//void SPWindowChattingPassword::Render(float fTime)
//{
//	if( !m_bShow )	return;
//
//	g_pVideo->Flush();
//
//	SPWindow::Render( fTime );
//
//	g_pVideo->Flush();
//}

void SPWindowChattingPassword::Show(bool bWithChild)
{
	if( m_bUse == false )
		return;

	if( m_pWindowSender == NULL )
		return;

	SPWindow::Show( bWithChild );

	if( g_pInterfaceManager != NULL )
	{
		g_pInterfaceManager->SetFocusWindow( this );
		g_pInterfaceManager->SetActiveWindow( this );

		SPWindow*	pWindow	=	Find( WIID_CHATTING_PASSWORD_EDIT , true );

		g_pInterfaceManager->SetFocusWindow( pWindow );
		g_pInterfaceManager->SetActiveWindow( pWindow );
	}
}

void SPWindowChattingPassword::Hide(bool bSendServer /*= true*/)
{
	SendMsg( SPIM_CHATTINGPASSWORD_CANCEL );
}

//void SPWindowTalkingRoom::Close()
//{
//	m_bShow	=	false;
//}

//void SPWindowTalkingRoom::RefreshRelationCoord()
//{
//	SPWindow::RefreshRelationCoord();
//	ReposOwnImage();
//}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowChattingPassword )

	//	Common
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGPASSWORD_SET	,																OnSet						)
	SPIMESSAGE_COMMAND(	SPIM_KEY_RETURN				,																OnKeyReturn					)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_PASSWORD_CLOSE							,	OnClose						)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_PASSWORD_OK								,	OnOk						)
	SPIMESSAGE_CONTROL(	SPIM_KEY_RETURN				,	WIID_CHATTING_PASSWORD_EDIT								,	OnKeyReturn					)

	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_CHATTING_PASSWORD_EDIT								,	OnEditSetFocus				)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_CHATTING_PASSWORD_EDIT								,	OnEditKillFocus				)

SPIMESSAGE_OBJ_MAP_END(	SPWindow	)

int SPWindowChattingPassword::OnSet( WPARAM wParam, LPARAM lParam )
{
	if( m_pWindowSender != NULL )
		SendMsg( SPIM_CHATTINGPASSWORD_CANCEL );

	m_pWindowSender			=	(SPWindow*)wParam;
	m_pSetData				=	(CHATTING_PASSWORD_SET_DATA*)lParam;

	if( m_pSetData == NULL )
		return 1;

	m_GetData.m_iRoomIndex		=	m_pSetData->m_iRoomIndex;
	m_GetData.m_iRoomNumber		=	m_pSetData->m_iRoomNumber;
	m_GetData.m_iRoomCategory	=	m_pSetData->m_iRoomCategory;
	m_GetData.m_bLocalSend		=	m_pSetData->m_bLocalSend;

	SPWindow*	pWindow	=	Find( WIID_CHATTING_PASSWORD_SUBJECT , true );

	if( pWindow == NULL )
		return 1;

	pWindow->SetWindowText( m_pSetData->m_strSubject.c_str() );

	pWindow	=	Find( WIID_CHATTING_PASSWORD_EDIT , true );

	if( pWindow == NULL )
		return 1;

	pWindow->SetWindowText( NULL );

	Show();

	return 1;
}

int SPWindowChattingPassword::OnKeyReturn( WPARAM wParam, LPARAM lParam )
{
	SPWindow*	pWindow			=	Find( WIID_CHATTING_PASSWORD_EDIT , true );

	if( pWindow == NULL )
		return 1;

	const char*	pstrPassword	=	pWindow->GetWindowText();

	if( pstrPassword == NULL )
		return 1;

	OnOk( NULL , NULL );

	return 1;
}

int SPWindowChattingPassword::OnClose( WPARAM wParam, LPARAM lParam )
{
	SendMsg( SPIM_CHATTINGPASSWORD_CANCEL );

	return 1;
}

int SPWindowChattingPassword::OnOk( WPARAM wParam, LPARAM lParam )
{
	const char*	pstrPassword	=	NULL;
	SPWindow*	pWindow			=	Find( WIID_CHATTING_PASSWORD_EDIT , true );

	if( pWindow != NULL )
	{
		pstrPassword	=	pWindow->GetWindowText();

		if( pstrPassword != NULL )
			m_GetData.m_strPassword	=	pstrPassword;

		pWindow->SetWindowText( NULL );
	}

	SendMsg( SPIM_CHATTINGPASSWORD_OK );

	return 1;
}

int SPWindowChattingPassword::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );
	return 1;
}

int SPWindowChattingPassword::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );
	return 1;
}

void SPWindowChattingPassword::SendMsg( SPIM SpimID )
{
	SPWindow::Hide();

	if( m_pWindowSender != NULL )
		m_pWindowSender->SPSendMessage( SpimID , (WPARAM)(&m_GetData) );

	ClearData();
}

void SPWindowChattingPassword::ClearData()
{
	m_pWindowSender				=	NULL;

	m_GetData.m_iRoomIndex		=	0;
	m_GetData.m_iRoomNumber		=	0;
	m_GetData.m_iRoomCategory	=	0;
	m_GetData.m_bLocalSend		=	false;
	m_GetData.m_strPassword.clear();
}
