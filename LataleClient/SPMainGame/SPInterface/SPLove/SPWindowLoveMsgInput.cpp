// ***************************************************************
//  SPWindowLoveMsgInput		Version:  1.0		Date: 2008/11/03
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
#include "SPWindowEditMultiLine.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPInterfaceManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPLocalizeManager.h"

#include "SPCheckManager.h"
#include "GlobalDefines_Share.h"

#include "SPAbuseFilter.h"

#include "SPLoveManager.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPUIManager.h"

#include "SPWindowLoveMsgInput.h"

SPWindowLoveMsgInput::SPWindowLoveMsgInput( WND_ID WndClassID , INSTANCE_ID InstanceID )
: SPWindow( WndClassID , InstanceID )
{	
	Init();	
}

SPWindowLoveMsgInput::SPWindowLoveMsgInput( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent /* = NULL  */ )
: SPWindow( WNDID_LOVEMSG_INPUT , InstanceID , iX , iY , iCX , iCY , pParent )
{	
	Init();	
}

SPWindowLoveMsgInput::~SPWindowLoveMsgInput()
{
	Clean();
}

void SPWindowLoveMsgInput::Init()
{
	m_strToolTip	= "[SPWindowLoveMsgInput]";

	//////////////////////////////////////////////////////////////////////////

	SPWindowButton*			pWindowButton		=	NULL;
	SPWindowStatic*			pWindowStatic		=	NULL;
	SPWindowEditMultiLine*	pWindowMultiEdit	=	NULL;

	SPWindowStatic*			pWindowParent		=	NULL;

	//////////////////////////////////////////////////////////////////////////

	SPWindow_Base*			pattrwnd_Map		=	NULL;

	//////////////////////////////////////////////////////////////////////////

	m_bUse			=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_LOVE );

	//////////////////////////////////////////////////////////////////////////

	if( g_UIManager->Load( RES_FNAME_XML_UI_LOVEMSG ) == true )
	{
		//////////////////////////////////////////////////////////////////////////

		pWindowParent	=	(SPWindowStatic*)this;

		//////////////////////////////////////////////////////////////////////////

		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVEMSG_INPUT_BG	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		pWindowStatic->SetNullWindow( 1 );

		pWindowParent	=	pWindowStatic;

		pWindowMultiEdit	=	new SPWindowEditMultiLine( WIID_LOVEMSG_INPUT_EDIT , 17 , 50 , 145 , 45 , pWindowParent );
		pWindowMultiEdit->SetLimitText( 24 * 2 );
		pWindowMultiEdit->SetFormat( DT_LEFT | DT_VCENTER );
		pWindowMultiEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
		pWindowMultiEdit->SPSendMessage( SPIM_SET_MARGIN , 2 );

		//////////////////////////////////////////////////////////////////////////

		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVEMSG_INPUT_BTN_CLOSE		, SPWindowButton , pWindowButton , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVEMSG_INPUT_BTN_OK			, SPWindowButton , pWindowButton , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVEMSG_INPUT_BTN_CANCEL		, SPWindowButton , pWindowButton , pattrwnd_Map , pWindowParent );
		
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVEMSG_INPUT_TIME				, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		m_pReMainTimeWindow	=	pWindowStatic;

		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVEMSG_INPUT_TIME_BG			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVEMSG_INPUT_CAPTION_SMG		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

		//////////////////////////////////////////////////////////////////////////

		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVEMSG_INPUT_EDITBG_L			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

		if( pWindowStatic != NULL )
		{
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 372 , 353 );
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 385 , 353 );
		}

		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVEMSG_INPUT_EDITBG_M			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

		if( pWindowStatic != NULL )
		{
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 377 , 353 );
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 390 , 353 );
		}

		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVEMSG_INPUT_EDITBG_R			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

		if( pWindowStatic != NULL )
		{
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 382 , 353 );
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 395 , 353 );
		}

		//////////////////////////////////////////////////////////////////////////

		g_UIManager->Release();
	}

	m_fReMainTime	=	0.0f;
}

void SPWindowLoveMsgInput::Clean()
{
	SPWindow::Clean();
}

void SPWindowLoveMsgInput::Process(float fTime)
{
	if( !m_bShow )	return;

	SPWindow::Process( fTime );

	if( m_fReMainTime >= 0.0f )
		m_fReMainTime	-=	fTime;

	if( m_fReMainTime <= 0.0f )
	{
		m_fReMainTime	=	0.0f;
		TimeAction();
	}

	if( m_pReMainTimeWindow != NULL && m_fReMainTime >= 0.0f )
	{
		sprintf( m_szBuf , "%.0f" , m_fReMainTime );
		m_pReMainTimeWindow->SetWindowText( m_szBuf );
	}
}

void SPWindowLoveMsgInput::Render(float fTime)
{
	if( !m_bShow )	return;

	g_pVideo->Flush();

	SPWindow::RenderReverse( fTime , 0 );

	g_pVideo->Flush();
}

void SPWindowLoveMsgInput::Show(bool bWithChild)
{
	if( m_bUse == false )
		return;

	SPWindow::Show( bWithChild );

	CheckGender();

	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
		m_fReMainTime	=	(float)pLoveManager->GetRequire( LOVE_LDT_REQUIRE_WEDDING_SHOUT_REMAIN_TIME );
	else
        m_fReMainTime	=	60.0f;
}

void SPWindowLoveMsgInput::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide( bSendServer );

	SPWindow*	pWindow	=	Find( WIID_LOVEMSG_INPUT_EDIT , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( NULL );
}

void SPWindowLoveMsgInput::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowLoveMsgInput )

	//	Common
	SPIMESSAGE_COMMAND(	SPIM_KEY_RETURN				,																	OnKeyReturn				)

	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_LOVEMSG_INPUT_EDIT										,	OnEditSetFocus			)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_LOVEMSG_INPUT_EDIT										,	OnEditKillFocus			)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_LOVEMSG_INPUT_BTN_CLOSE								,	OnClose					)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_LOVEMSG_INPUT_BTN_CANCEL								,	OnClose					)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_LOVEMSG_INPUT_BTN_OK									,	OnOk					)

SPIMESSAGE_OBJ_MAP_END(	SPWindow	)

int SPWindowLoveMsgInput::OnKeyReturn( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return -1;

	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_LOVEMSG_INPUT_EDIT , true );

	if( pWindow == NULL )
		return 1;

	const char*	pszTargetName	=	pWindow->GetWindowText();

	if( pszTargetName == NULL )
		return 1;

	std::string	strTargetName	=	pszTargetName;

	if( strTargetName.empty() == true )
		return 1;

	return 1;
}

int SPWindowLoveMsgInput::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SetKeyInput( TRUE );

	SetGender( LOVEMSG_INPUT_GENDER_NULL );

	return 1;
}

int SPWindowLoveMsgInput::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SetKeyInput( FALSE );

	CheckGender();

	return 1;
}

int SPWindowLoveMsgInput::OnClose( WPARAM wParam, LPARAM lParam )
{
	SPWindow*	pWindow	=	Find( WIID_LOVEMSG_INPUT_EDIT , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( NULL );

	OnOk( NULL , NULL );
	return 1;
}

int SPWindowLoveMsgInput::OnOk( WPARAM wParam, LPARAM lParam )
{
	TimeAction();
	return 1;
}

void SPWindowLoveMsgInput::SetGender( LOVEMSG_INPUT_GENDER eGender )
{
	SPWindowStatic*			pWindowStatic		=	NULL;

	pWindowStatic	=	(SPWindowStatic*)Find( WIID_LOVEMSG_INPUT_EDITBG_L , true );

	if( pWindowStatic != NULL )
		pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , (WPARAM)eGender );

	pWindowStatic	=	(SPWindowStatic*)Find( WIID_LOVEMSG_INPUT_EDITBG_M , true );

	if( pWindowStatic != NULL )
		pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , (WPARAM)eGender );

	pWindowStatic	=	(SPWindowStatic*)Find( WIID_LOVEMSG_INPUT_EDITBG_R , true );

	if( pWindowStatic != NULL )
		pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , (WPARAM)eGender );
}

void SPWindowLoveMsgInput::CheckGender()
{
	if( g_pCheckManager != NULL )
	{
		if( g_pCheckManager->CheckPCStatus( CHECK_STATUS_GENDER , LOVEMSG_INPUT_GENDER_WOMAN , 0 ) == true )
			SetGender( LOVEMSG_INPUT_GENDER_WOMAN );
		else
			SetGender( LOVEMSG_INPUT_GENDER_MAN );
	}
}

void SPWindowLoveMsgInput::TimeAction()
{
	std::string	strLoveMsg;

	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5208126 );

	if( pszGlobalString != NULL )
	{
		if( strlen( pszGlobalString ) > 0 )
			strLoveMsg	=	pszGlobalString;
	}

	SPWindow*	pWindow	=	Find( WIID_LOVEMSG_INPUT_EDIT , true );

	if( pWindow != NULL )
	{
		pszGlobalString	=	pWindow->GetWindowText();

		if( pszGlobalString != NULL )
		{
			if( strlen( pszGlobalString ) > 0 )
				strLoveMsg	=	pszGlobalString;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	char szBuffer[1025];
	SPAbuseFilter::GetInstance()->Filter( strLoveMsg.c_str() , szBuffer , 1024 );
	strLoveMsg	=	szBuffer;

	//////////////////////////////////////////////////////////////////////////

	if( strLoveMsg.empty() == false )
	{
		UINT8	iMsgLen	=	(UINT8)strLoveMsg.size();							// 메세지 길이
		TCHAR	szMsg[ LEN_CASHITEM_NORMAL_MSG + 1 ] = {0,};

		if( iMsgLen > LEN_CASHITEM_NORMAL_MSG )
			iMsgLen	=	LEN_CASHITEM_NORMAL_MSG;

		CPacket		stPacket;

		stPacket.AddCommand( WEDDING_CS_CHAT );
		stPacket.Add( iMsgLen );
		stPacket.Add( (char*)strLoveMsg.c_str() , iMsgLen );

		if( g_pNetworkManager != NULL )
			g_pNetworkManager->GetPacketHandler()->PostMsg( &stPacket );
	}

	Hide();
}
