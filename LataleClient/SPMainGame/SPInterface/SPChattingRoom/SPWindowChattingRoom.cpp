// ***************************************************************
//  SPWindowChattingRoom   version:  1.0   ·  date: 02/25/2008
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
#include "SPWindowEditMultiLine.h"
#include "SPWindowSlider.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include "SPNoticeBox.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPItemAttr.h"
#include "SPManager.h"
#include "SPItem.h"
#include "SPItemCluster.h"		//[2005/6/16]

#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"

#include "SPCommandConvert.h"
#include "SPGOBStatus.h"
#include "SPGOBManager.h"
#include "SPGOBCoordPhysics.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPCheckManager.h"
#include "GlobalDefines_Share.h"

#include "SPWindowToolTip.h"

#include "SPIndun.h"
#include "SPIndunManager.h"

#include "SPAbuseFilter.h"

#include "SPWindowChat.h"			// GetRandom
#include "SPWindowChattingPassword.h"

#include "SPChatManager.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPWindowChattingRoom.h"

SPWindowChattingRoom::SPWindowChattingRoom( WND_ID WndClassID , INSTANCE_ID InstanceID )
: SPWindow( WndClassID , InstanceID )
{	
	Init();	
}

SPWindowChattingRoom::SPWindowChattingRoom( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent /* = NULL  */ )
: SPWindow( WNDID_CHATTING_ROOM , InstanceID , iX , iY , iCX , iCY , pParent )
{	
	Init();	
}

SPWindowChattingRoom::~SPWindowChattingRoom()
{
	Clean();
}

void SPWindowChattingRoom::Init()
{
	m_strToolTip	= "[SPWindowChattingRoom]";

	//////////////////////////////////////////////////////////////////////////

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , &m_pBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[ CHATTING_ROOM_BG_LEFT_TOP ]		, 482 , 467 , 486 , 471 );
	SetRect( &m_rcBGSrc[ CHATTING_ROOM_BG_CENTER_TOP ]		, 487 , 467 , 491 , 471 );
	SetRect( &m_rcBGSrc[ CHATTING_ROOM_BG_RIGHT_TOP ]		, 492 , 467 , 496 , 471 );

	SetRect( &m_rcBGSrc[ CHATTING_ROOM_BG_LEFT_MIDDLE ]		, 482 , 472 , 486 , 476 );
	SetRect( &m_rcBGSrc[ CHATTING_ROOM_BG_CENTER_MIDDLE ]	, 487 , 472 , 491 , 476 );
	SetRect( &m_rcBGSrc[ CHATTING_ROOM_BG_RIGHT_MIDDLE ]	, 492 , 472 , 496 , 476 );

	SetRect( &m_rcBGSrc[ CHATTING_ROOM_BG_LEFT_BOTTOM ]		, 482 , 477 , 486 , 481 );
	SetRect( &m_rcBGSrc[ CHATTING_ROOM_BG_CENTER_BOTTOM ]	, 487 , 477 , 491 , 481 );
	SetRect( &m_rcBGSrc[ CHATTING_ROOM_BG_RIGHT_BOTTOM ]	, 492 , 477 , 496 , 481 );

	SetRect( &m_rcBGSrc[ CHATTING_ROOM_BG_LINE_1 ]			, 508 , 450 , 510 , 451 );
	SetRect( &m_rcBGSrc[ CHATTING_ROOM_BG_SUB ]				, 502 , 472 , 506 , 476 );

	//////////////////////////////////////////////////////////////////////////

	SPWindowButton*			pWindowButton;
	SPWindowStatic*			pWindowStatic;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_WAITTING , 0 , 0 , m_iSX , m_iSY , this );
	pWindowStatic->SetShowEnable( false );
	InitWindowWaitting( pWindowStatic );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_STYLE , 0 , 0 , m_iSX , m_iSY , this );
	pWindowStatic->SetShowEnable( false );
	InitWindowStyle( pWindowStatic );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH , 0 , 0 , m_iSX , m_iSY , this );
	pWindowStatic->SetShowEnable( false );
	InitWindowPalSearch( pWindowStatic );

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_SUBJECT , 2 , 2 , 187 , 24 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 1 , 313 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_CLOSE , 304 , 9 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 27 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 27 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 40 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 40 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MAKE , 164 , 35 , 71 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 100 , 113 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 100 , 129 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 100 , 145 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 100 , 161 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 100 , 145 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_FIND , 14 , 35 , 71 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 172 , 113 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 172 , 129 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 172 , 145 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 172 , 161 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 172 , 145 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_PAL , 89 , 35 , 71 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 244 , 113 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 244 , 129 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 244 , 145 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 244 , 161 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 244 , 145 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_SETUP , 239 , 35 , 71 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 316 , 113 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 316 , 129 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 316 , 145 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 316 , 161 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 316 , 145 );
	pWindowButton	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_PANEL , 0 , 0 , m_iSX , m_iSY , this );
	InitWindowRoomMake( pWindowStatic );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_PANEL , 0 , 0 , m_iSX , m_iSY , this );
	InitWindowRoomFind( pWindowStatic );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_PANEL , 0 , 0 , m_iSX , m_iSY , this );
	InitWindowRoomPal( pWindowStatic );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_SETUP_PANEL , 0 , 0 , m_iSX , m_iSY , this );
	InitWindowRoomSetup( pWindowStatic );

	//////////////////////////////////////////////////////////////////////////

	m_pPalPlayer		=	new SPPlayer;

	if( m_pPalPlayer != NULL )
	{
		m_pPalPlayer->SetLobbyAvatar( LOBBY_INFO );
		m_pPalPlayer->Init();
		m_pPalPlayer->SetShowValue(false, false, false);
		m_pPalPlayer->SetPos((float)(m_iAX + 74), (float)(m_iAY + 364) );
	}

	m_bUse	=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_CHATTINGROOM );

	int	i;

	for( i = 0 ; i < ROOM_STYLE_MAX ; ++i )
	{
		m_iCurIndexRoomMake[ i ]		=	0;
	}

	for( i = 0 ; i < MY_STYLE_MAX ; ++i )
	{
		m_iCurIndexMyStyle[ i ]			=	0;
		m_iCurIndexPalSearchStyle[ i ]	=	0;
	}

	for( i = 0 ; i < SLIDER_SELECT_MAX ; ++i )
	{
		m_iSelectIndex[ i ]				=	-1;
	}

	m_bPalPlayerRender	=	false;

	//////////////////////////////////////////////////////////////////////////

	m_iRoomPalSex					=	0;
	m_iRoomPalMinLevel				=	1;
	m_iRoomPalMaxLevel				=	200;
	m_stRoomPalStyle.uiHair			=	0;
	m_stRoomPalStyle.uiFashion		=	0;
	m_stRoomPalStyle.uiPersonality	=	0;
	m_stRoomPalStyle.uiCondition	=	0;

	//////////////////////////////////////////////////////////////////////////

	m_stRoomMyStyle.uiHair			=	0;
	m_stRoomMyStyle.uiFashion		=	0;
	m_stRoomMyStyle.uiPersonality	=	0;
	m_stRoomMyStyle.uiCondition		=	0;

	//////////////////////////////////////////////////////////////////////////

	m_fAccumulateTime				=	0.0f;
	m_fAccumulateLimitTime			=	0.03f;
	m_iFrameIndex					=	0;
	m_iFrameMax						=	8;

	//////////////////////////////////////////////////////////////////////////

	m_vPrintNameBuffer.push_back( 5008107 );
	m_vPrintNameBuffer.push_back( 5008108 );
	m_vPrintNameBuffer.push_back( 5008109 );
	m_vPrintNameBuffer.push_back( 5008111 );

	//////////////////////////////////////////////////////////////////////////

	m_cRoomTextColor[ 0 ]	=	D3DCOLOR_ARGB(	255	,	49	,	76	,	97	);
	m_cRoomTextColor[ 1 ]	=	D3DCOLOR_ARGB(	255	,	187	,	113	,	106	);
}

void SPWindowChattingRoom::Clean()
{
	SPWindow::Clean();

	SAFE_DELETE( m_pPalPlayer );

	SAFE_RELEASE( m_pBGTexture );
}

void SPWindowChattingRoom::Process(float fTime)
{
	if( !m_bShow )	return;

	SPWindow::Process( fTime );

	m_fAccumulateTime	+=	fTime;

	if( m_fAccumulateTime >= m_fAccumulateLimitTime )
	{
		m_fAccumulateTime	=	0.0f;
		UpdateWheel();
	}

	if( m_pPalPlayer != NULL && m_pRoomPalWindow != NULL )
	{
		if( m_pRoomPalWindow->IsShow() == true && m_bPalPlayerRender == true )
		{
			m_pPalPlayer->SetPos((float)(m_iAX + 74), (float)(m_iAY + 364) );
			m_pPalPlayer->Process( fTime );
		}
	}
}

void SPWindowChattingRoom::Render(float fTime)
{
	if( !m_bShow )	return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < CHATTING_ROOM_BG_COUNT ; ++i )
		{
			m_pBGTexture->SetColor( m_RGBA );
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	if( m_pPalPlayer != NULL && m_pRoomPalWindow != NULL )
	{
		if( m_pRoomPalWindow->IsShow() == true && m_bPalPlayerRender == true )
		{
			m_pPalPlayer->Render( fTime );
			g_pVideo->Flush();
		}
	}

	SPWindow::RenderReverse( fTime );

	g_pVideo->Flush();
}

void SPWindowChattingRoom::Show(bool bWithChild)
{
	if( m_bUse == false )
		return;

	INSTANCE_ID	eActiveInstanceID	=	GetActiveMenuWindow( WIID_CHATTING_ROOM_MENU_START , WIID_CHATTING_ROOM_MENU_END , CHATTING_ROOM_MENU_STEP , false );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		bool		bMenuShow	=	!pChatManager->IsChatRoom();
		SPWindow*	pWindow;

		pWindow	=	Find( WIID_CHATTING_ROOM_MAKE , true );
		pWindow->SetShowEnable( bMenuShow );

		pWindow	=	Find( WIID_CHATTING_ROOM_SETUP , true );
		pWindow->SetShowEnable( bMenuShow );

		if( bMenuShow == false && ( eActiveInstanceID == WIID_CHATTING_ROOM_MAKE || eActiveInstanceID == WIID_CHATTING_ROOM_SETUP ) )
			eActiveInstanceID = 0;

		bMenuShow			=	true;
		SPIndunManager*	pIndunManager	=	SPIndunManager::GetInstance();

		if( pIndunManager != NULL )
			bMenuShow	= !( pIndunManager->IsInIndun() );

		if( pChatManager != NULL && bMenuShow == false )
			bMenuShow = pChatManager->IsChatRoom();

		pWindow	=	Find( WIID_CHATTING_ROOM_PAL_INVITE , true );

		if( pWindow != NULL )
			pWindow->SetEnable( bMenuShow );
	}

	SPWindow::Show( bWithChild );

	if( eActiveInstanceID == 0 )
		eActiveInstanceID = WIID_CHATTING_ROOM_FIND;

	OnRoomMenu( eActiveInstanceID , NULL , NULL );
}

void SPWindowChattingRoom::Hide( bool bSendServer /*= true*/ )
{
	if( m_bShow == true )
		g_pInterfaceManager->RegisterSoundUnit( "DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV" );

	SPWindow::Hide( bSendServer );

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		SPWindow*	pWindowToolTip	=	g_pInterfaceManager->GetToolTipWindow();

		if( pWindowToolTip != NULL )
			pWindowToolTip->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , -2 );
	}
}

//void SPWindowChattingRoom::Close()
//{
//	Hide();
//}

void SPWindowChattingRoom::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowChattingRoom::ReposOwnImage()
{
	SetRect( &m_rcBGDest[ CHATTING_ROOM_BG_LEFT_TOP ]		, m_iAX					, m_iAY					, m_iAX + 4			, m_iAY + 4 );
	SetRect( &m_rcBGDest[ CHATTING_ROOM_BG_CENTER_TOP ]		, m_iAX + 4				, m_iAY					, m_iAX + 4 + 317	, m_iAY + 4 );
	SetRect( &m_rcBGDest[ CHATTING_ROOM_BG_RIGHT_TOP ]		, m_iAX + 321			, m_iAY					, m_iAX + 321 + 4	, m_iAY + 4 );

	SetRect( &m_rcBGDest[ CHATTING_ROOM_BG_LEFT_MIDDLE ]	, m_iAX					, m_iAY + 4				, m_iAX + 4			, m_iAY + 4 + 394 );
	SetRect( &m_rcBGDest[ CHATTING_ROOM_BG_CENTER_MIDDLE ]	, m_iAX + 4				, m_iAY	+ 4				, m_iAX + 4 + 317	, m_iAY + 4 + 394 );
	SetRect( &m_rcBGDest[ CHATTING_ROOM_BG_RIGHT_MIDDLE ]	, m_iAX + 321			, m_iAY + 4				, m_iAX + 321 + 4	, m_iAY + 4 + 394 );

	SetRect( &m_rcBGDest[ CHATTING_ROOM_BG_LEFT_BOTTOM ]	, m_iAX					, m_iAY + 398			, m_iAX + 4			, m_iAY + 398 + 4 );
	SetRect( &m_rcBGDest[ CHATTING_ROOM_BG_CENTER_BOTTOM ]	, m_iAX + 4				, m_iAY	+ 398			, m_iAX + 4 + 317	, m_iAY + 398 + 4 );
	SetRect( &m_rcBGDest[ CHATTING_ROOM_BG_RIGHT_BOTTOM ]	, m_iAX + 321			, m_iAY + 398			, m_iAX + 321 + 4	, m_iAY + 398 + 4 );

	SetRect( &m_rcBGDest[ CHATTING_ROOM_BG_LINE_1 ]			, m_iAX + 2				, m_iAY	+ 26			, m_iAX + 2 + 321	, m_iAY + 26 + 1 );
	SetRect( &m_rcBGDest[ CHATTING_ROOM_BG_SUB ]			, m_iAX + 2				, m_iAY + 27			, m_iAX + 2 + 321	, m_iAY + 27 + 371 );
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowChattingRoom )

	//	Common
	SPIMESSAGE_COMMAND(	SPIM_PURGE					,																	OnPurge						)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGPASSWORD_OK	,																	OnPasswordOK				)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGPASSWORD_CANCEL,																	OnPasswordCancel			)
	SPIMESSAGE_COMMAND( SPIM_NOTICE_YES				,																	OnNoticeYes					)
	SPIMESSAGE_COMMAND( SPIM_NOTICE_NO				,																	OnNoticeNo					)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_JOIN_CLEAR,																	OnJoinClear					)

	//	Packet
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_CREATE	,																	OnPacketCreate				)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_ROOM_CHECK,																	OnPacketRoomCheck			)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_JOIN		,																	OnPacketJoin				)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_ROOMLIST	,																	OnPacketRoomList			)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_USERLIST	,																	OnPacketUserList			)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_INVITE_REQ,																	OnPacketInviteReq			)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_INVITE_ANS,																	OnPacketInviteAns			)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_OTHERPC_INFO,																	OnPacketOtherPCInfo			)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_GET_MYSTYLE,																	OnPacketMyStyle				)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_OPTION	,																	OnRefreshOption				)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_CLOSE									,	OnClose						)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_CHATTING_ROOM_MENU_START , WIID_CHATTING_ROOM_MENU_END	,	OnRoomMenu					)

	//	Room Make
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MAKE_RUN									,	OnRoomMakeRun				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MAKE_CLOSE								,	OnRoomMakeClose				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_USE					,	OnRoomMakeAdvUse			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_LEFT_ROOM_TYPE			,	OnRoomMakeTypeLeft			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_RIGHT_ROOM_TYPE		,	OnRoomMakeTypeRight			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_LEFT_GENDER			,	OnRoomMakeGenderLeft		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_RIGHT_GENDER			,	OnRoomMakeGenderRight		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_LEFT_AGE				,	OnRoomMakeAgeLeft			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_RIGHT_AGE				,	OnRoomMakeAgeRight			)

	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT						,	OnEditSetFocus				)
	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_EDIT					,	OnEditSetFocus				)
	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_NUMBER					,	OnEditSetFocus				)
	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_PASSWORD				,	OnEditSetFocus				)

	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT						,	OnEditKillFocus				)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_EDIT					,	OnEditKillFocus				)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_NUMBER					,	OnNumberEditKillFocus		)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_PASSWORD				,	OnEditKillFocus				)

	//	Room Find
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_FIND_CLOSE								,	OnRoomFindClose				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_FIND_JOIN								,	OnRoomFindJoin				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_FIND_SLIDER_UP							,	OnRoomFindSliderUp			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_FIND_SLIDER_DOWN							,	OnRoomFindSliderDown		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_FIND_REFRESH								,	OnRoomFindRefresh			)

	SPIMESSAGE_CONTROL( SPIM_SLIDER_UP				,	WIID_CHATTING_ROOM_FIND_SLIDER								,	OnRoomFindSlider			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN			,	WIID_CHATTING_ROOM_FIND_SLIDER								,	OnRoomFindSlider			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP			,	WIID_CHATTING_ROOM_FIND_SLIDER								,	OnRoomFindSlider			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN		,	WIID_CHATTING_ROOM_FIND_SLIDER								,	OnRoomFindSlider			)
	SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE			,	WIID_CHATTING_ROOM_FIND_SLIDER								,	OnRoomFindSlider			)

	SPIMESSAGE_CONTROL( SPIM_WHEEL_UP				,	WIID_CHATTING_ROOM_FIND_SLIDER								,	OnRoomFindSliderUp			)
	SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN				,	WIID_CHATTING_ROOM_FIND_SLIDER								,	OnRoomFindSliderDown		)

	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_CHATTING_ROOM_FIND_ROOM_NUMBER							,	OnEditSetFocus				)

	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_CHATTING_ROOM_FIND_ROOM_NUMBER							,	OnEditKillFocus				)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_CHATTING_ROOM_FIND_NORMAL , WIID_CHATTING_ROOM_FIND_MEETING	,	OnRoomFindTypeMenu	)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG , WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG + CHATTING_ROOM_FIND_LIST_COUNT	,	OnRoomFindBar				)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DBLCLICK	,	WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG , WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG + CHATTING_ROOM_FIND_LIST_COUNT	,	OnRoomFindBarDblClick		)

	SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP			,	WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG , WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG + CHATTING_ROOM_FIND_LIST_COUNT ,	OnRoomFindSliderWheelUp		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN		,	WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG , WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG + CHATTING_ROOM_FIND_LIST_COUNT ,	OnRoomFindSliderWheelDown	)

	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER		,	WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG , WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG + CHATTING_ROOM_FIND_LIST_COUNT ,	OnRoomFindToolTipEnter		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT		,	WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG , WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG + CHATTING_ROOM_FIND_LIST_COUNT ,	OnRoomFindToolTipOut		)
//	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_MOVE		,	WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG , WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG + CHATTING_ROOM_FIND_LIST_COUNT ,	OnRoomFindSliderWheelDown	)

	//	Room Pal
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_PAL_CLOSE								,	OnRoomPalClose				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_PAL_REFRESH								,	OnRoomPalRefresh			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_PAL_SEARCH								,	OnRoomPalSearch				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_PAL_INVITE								,	OnRoomPalInvite				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_PAL_MY_STYLE								,	OnRoomPalMyStyle			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_PAL_SLIDER_UP							,	OnRoomPalSliderUp			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_PAL_SLIDER_DOWN							,	OnRoomPalSliderDown			)

	SPIMESSAGE_CONTROL( SPIM_SLIDER_UP				,	WIID_CHATTING_ROOM_PAL_SLIDER								,	OnRoomPalSlider				)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN			,	WIID_CHATTING_ROOM_PAL_SLIDER								,	OnRoomPalSlider				)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP			,	WIID_CHATTING_ROOM_PAL_SLIDER								,	OnRoomPalSlider				)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN		,	WIID_CHATTING_ROOM_PAL_SLIDER								,	OnRoomPalSlider				)
	SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE			,	WIID_CHATTING_ROOM_PAL_SLIDER								,	OnRoomPalSlider				)

	SPIMESSAGE_CONTROL( SPIM_WHEEL_UP				,	WIID_CHATTING_ROOM_PAL_SLIDER								,	OnRoomPalSliderUp			)
	SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN				,	WIID_CHATTING_ROOM_PAL_SLIDER								,	OnRoomPalSliderDown			)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG , WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG + CHATTING_ROOM_PAL_LIST_COUNT ,	OnRoomPalBar	)

	SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP			,	WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG , WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG + CHATTING_ROOM_PAL_LIST_COUNT ,	OnRoomPalSliderWheelUp		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN		,	WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG , WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG + CHATTING_ROOM_PAL_LIST_COUNT ,	OnRoomPalSliderWheelDown	)

	//	Room Setup
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_SETUP_SAVE								,	OnRoomSetupSave				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_SETUP_CLOSE								,	OnRoomSetupClose			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_SETUP_MY_INFO_SEARCH						,	OnRoomSetupMyInfoSearch		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_SETUP_CHAT_REQUEST						,	OnRoomSetupChatRequest		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_SETUP_MY_STYLE_OPEN						,	OnRoomSetupMyStyleOpen		)

	//	Modal MyStyle
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MODAL_STYLE_CLOSE						,	OnModalMyStyleClose			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MODAL_STYLE_CANCEL						,	OnModalMyStyleClose			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MODAL_STYLE_SAVE							,	OnModalMyStyleSave			)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_CHATTING_ROOM_MODAL_STYLE_RIGHT	, WIID_CHATTING_ROOM_MODAL_STYLE_RIGHT	+ CHATTING_ROOM_PAL_STYLE_COUNT	,	OnModalMyStyleArrowRight	)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_CHATTING_ROOM_MODAL_STYLE_LEFT		, WIID_CHATTING_ROOM_MODAL_STYLE_LEFT	+ CHATTING_ROOM_PAL_STYLE_COUNT	,	OnModalMyStyleArrowLeft		)

	//	Modal PalSearch
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_CLOSE					,	OnModalPalSearchClose		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_CANCEL					,	OnModalPalSearchClose		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_SEARCH					,	OnModalPalSearchSearch		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_ALL		, WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_WOMAN	,	OnModalPalSearchGender		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_RIGHT	, WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_RIGHT	+ CHATTING_ROOM_PAL_STYLE_COUNT	,	OnModalPalSearchArrowRight	)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LEFT	, WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LEFT	+ CHATTING_ROOM_PAL_STYLE_COUNT	,	OnModalPalSearchArrowLeft	)

	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_EDIT_MIN				,	OnEditSetFocus				)
	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_EDIT_MAX				,	OnEditSetFocus				)

	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_EDIT_MIN				,	OnEditKillFocus				)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_EDIT_MAX				,	OnEditKillFocus				)

SPIMESSAGE_OBJ_MAP_END(	SPWindow	)

int SPWindowChattingRoom::OnPurge( WPARAM wParam, LPARAM lParam )
{
	OnJoinClear( wParam , lParam );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		pChatManager->ClearChatRoomInfo();

	ClearRoomData( ROOM_TYPE_NULL );

	return 1;
}

int SPWindowChattingRoom::OnPasswordOK( WPARAM wParam, LPARAM lParam )
{
	CHATTING_PASSWORD_GET_DATA*	pGetData	=	(CHATTING_PASSWORD_GET_DATA*)wParam;

	if( pGetData == NULL )
		return 1;

	SendPacketRoomJoin( pGetData->m_iRoomIndex , pGetData->m_iRoomNumber , pGetData->m_iRoomCategory , (TCHAR*)( pGetData->m_strPassword.c_str() ) , pGetData->m_bLocalSend );

	return 1;
}

int SPWindowChattingRoom::OnPasswordCancel( WPARAM wParam, LPARAM lParam )
{
	CHATTING_PASSWORD_GET_DATA*	pGetData	=	(CHATTING_PASSWORD_GET_DATA*)wParam;

	if( pGetData == NULL )
		return 1;

	return 1;
}

int SPWindowChattingRoom::OnNoticeYes( WPARAM wParam, LPARAM lParam )
{
	SendPacketInviteAns( 1 );
	return 1;
}

int SPWindowChattingRoom::OnNoticeNo( WPARAM wParam, LPARAM lParam )
{
	//int	iForceHide	=	(int)lParam;

	//if( iForceHide < 0 )		// 강제로 닫힐때 [4/24/2007 AJJIYA]
	//	SendAcceptPacket( 0 );
	//else
	//	SendAcceptPacket( 1 );

	SendPacketInviteAns( 0 );

	return 1;
}

int SPWindowChattingRoom::OnJoinClear( WPARAM wParam, LPARAM lParam )
{
	SetWaittingShow( false , NULL );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		pChatManager->SetJoin( false );

	return 1;
}

int SPWindowChattingRoom::OnPacketCreate( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	UINT32	uiLocalDBKey	=	0;

	pPacket->ExtractUINT32( &uiLocalDBKey );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		pChatManager->SetLocalDBKey( uiLocalDBKey );

	UINT8	iAdvert;							// 광고 여부 (0, 1)

	pPacket->ExtractUINT8( &iAdvert );

	if( g_pCheckManager != NULL && iAdvert > 0 )
		g_pCheckManager->SetDBBlock( false );

	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	pPacket->ExtractUINT32( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		OnJoinClear( NULL , NULL );
		PrintErrorMsg( uiGlobalStringID );
		return 1;
	}

	CHATROOM_INFO	stCRInfo;							// 생성하려고하는 대화방의 정보

	pPacket->ExtractStruct( &stCRInfo , sizeof( CHATROOM_INFO ) );

	if( pChatManager != NULL )
	{
		pChatManager->SetJoin( true );
		pChatManager->SetChatRoomInfo( stCRInfo );
	}

	return 1;
}

int SPWindowChattingRoom::OnPacketRoomCheck( WPARAM wParam, LPARAM lParam )
{
	bool			bLocalSend	=	(bool)wParam;
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	if( bLocalSend == false )
		SetWaittingShow( false , NULL );

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	pPacket->ExtractUINT32( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		PrintErrorMsg( uiGlobalStringID );
		return 1;
	}

	CHATROOM_INFO	stCRInfo;							// 입장하려고하는 대화방의 정보
	int				iKey;

	pPacket->ExtractStruct( &stCRInfo , sizeof( CHATROOM_INFO ) );

	iKey	=	stCRInfo.iCategory + 1;

	CHATROOM_INFO*	pChatRoomInfo	=	GetRoomData( stCRInfo.uiRoomNo , iKey );

	UINT32 uiID	=	0;

	if( pChatRoomInfo != NULL )
		uiID	=	pChatRoomInfo->uiID;

	if( bLocalSend == false )
	{
		AddRoomData( iKey , stCRInfo );

		if( uiID != stCRInfo.uiID )
		{
			PrintErrorMsg( 5008106 );
			return 1;
		}
	}

	std::string	strPassword	=	stCRInfo.szPassword;

	if( strPassword.empty() == false && g_pInterfaceManager != NULL && g_pResourceManager != NULL )
	{
		const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008137 );

		CHATTING_PASSWORD_SET_DATA	stSetData;

		stSetData.m_iRoomIndex		=	stCRInfo.uiID;
		stSetData.m_iRoomNumber		=	stCRInfo.uiRoomNo;
		stSetData.m_iRoomCategory	=	stCRInfo.iCategory;
		stSetData.m_bLocalSend		=	bLocalSend;

		if( pszGlobalString != NULL )
			stSetData.m_strSubject	=	pszGlobalString;
		else
			stSetData.m_strSubject	=	"Password";

		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_PASSWORD , SPIM_CHATTINGPASSWORD_SET , (WPARAM)this , (LPARAM)&stSetData );
		return 1;
	}

	SendPacketRoomJoin( stCRInfo.uiID , stCRInfo.uiRoomNo , stCRInfo.iCategory , NULL , bLocalSend );
	return 1;
}

int SPWindowChattingRoom::OnPacketJoin( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	UINT32	uiLocalDBKey	=	0;

	pPacket->ExtractUINT32( &uiLocalDBKey );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		pChatManager->SetLocalDBKey( uiLocalDBKey );

	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	pPacket->ExtractUINT32( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		OnJoinClear( NULL , NULL );
		PrintErrorMsg( uiGlobalStringID );
		return 1;
	}

	CHATROOM_INFO	stCRInfo;							// 입장하려고하는 대화방의 정보

	pPacket->ExtractStruct( &stCRInfo , sizeof( CHATROOM_INFO ) );

	if( pChatManager != NULL )
	{
		pChatManager->SetJoin( true );
		pChatManager->SetChatRoomInfo( stCRInfo );
	}

	return 1;
}

int SPWindowChattingRoom::OnPacketRoomList( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	pPacket->ExtractUINT32( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		SetWaittingShow( false , NULL );
		PrintErrorMsg( uiGlobalStringID );
		return 1;
	}

	UINT8			iResetFlag;						// (0 or 1 = 리스트 초기화)
	UINT8			iRoomType;
	UINT32			uiCount;
	CHATROOM_INFO	stChatRoomInfo;

	pPacket->ExtractUINT8( &iResetFlag );
	pPacket->ExtractUINT8( &iRoomType );
	pPacket->ExtractUINT32( &uiCount );

	++iRoomType;

	if( uiCount <= 0 )
		m_bRoomDataZero[ iRoomType ]	=	true;
	else
		m_bRoomDataZero[ iRoomType ]	=	false;

	if( iResetFlag == 1 || iResetFlag == 3 )
		ClearRoomData( (int)iRoomType );
	
	if( iResetFlag == 2 || iResetFlag == 3 )
		SetWaittingShow( false , NULL );

	for( UINT32 ui = 0 ; ui < uiCount ; ++ui )
	{
		pPacket->ExtractStruct( &stChatRoomInfo , sizeof( CHATROOM_INFO ) );

		AddRoomData( iRoomType , stChatRoomInfo );
	}

	UpdateRoomFindData( (ROOM_TYPE_ID)( iRoomType ) );

	return 1;
}

int SPWindowChattingRoom::OnPacketUserList( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	SetWaittingShow( false , NULL );

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	pPacket->ExtractUINT32( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		PrintErrorMsg( uiGlobalStringID );
		return 1;
	}

	UINT32					uiCount;
	CHARACTER_BASICINFO		stCharInfo;

	pPacket->ExtractUINT32( &uiCount );

	m_vRoomPal.clear();
	ClearRoomPalData();

	for( UINT32 ui = 0 ; ui < uiCount ; ++ui )
	{
		pPacket->ExtractStruct( &stCharInfo , sizeof( CHARACTER_BASICINFO ) );

		m_vRoomPal.push_back( stCharInfo );
	}

	UpdateRoomPalData();

	return 1;
}

int SPWindowChattingRoom::OnPacketInviteReq( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	TCHAR	szTargetName[ LEN_NAME + 1 ];
	memset( &szTargetName	, 0 , sizeof( szTargetName )	);
	pPacket->ExtractData( szTargetName , sizeof( szTargetName ) );

	GLOBAL_STRING_ID	uiGlobalStringID	=	0;
	pPacket->ExtractUINT32( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		PrintErrorMsg( uiGlobalStringID , szTargetName );
		return 1;
	}

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->ForceReply();

	memset( &m_szActorName	, 0 , sizeof( m_szActorName )	);
	pPacket->ExtractData( m_szActorName , sizeof( m_szActorName ) );
	pPacket->ExtractUINT8( &m_uiInviteInRoom );
	pPacket->ExtractStruct( &m_stChatRoomInfo , sizeof( CHATROOM_INFO ) );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		if( pChatManager->GetJoin() == true )
		{
			SendPacketInviteAns( 0 );
			return 1;
		}
	}

	char		szBuf[ _MAX_PATH ];
	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008138 );

	if( pszGlobalString != NULL )
		sprintf( szBuf , pszGlobalString , m_szActorName );
	else
		sprintf( szBuf , "%s invite " , m_szActorName );

	g_pInterfaceManager->SetNotice( szBuf , this , NOTICE_TYPE_YESNO );

	return 1;
}

int SPWindowChattingRoom::OnPacketInviteAns( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	TCHAR	szTargetName[ LEN_NAME + 1 ];
	memset( &szTargetName	, 0 , sizeof( szTargetName )	);
	pPacket->ExtractData( szTargetName , sizeof( szTargetName ) );

	GLOBAL_STRING_ID	uiGlobalStringID	=	0;
	pPacket->ExtractUINT32( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		OnJoinClear( NULL , NULL );
		PrintErrorMsg( uiGlobalStringID , szTargetName );
		return 1;
	}

	return 1;
}

int SPWindowChattingRoom::OnPacketOtherPCInfo( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	SetWaittingShow( false , NULL );

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	pPacket->ExtractUINT32( &uiGlobalStringID );

	 UINT8				iFlag	=	0;								// 0: 정상, 1: 자신이 스타일 비공개, 2: 대상이 스타일 비공개

	 pPacket->ExtractUINT8( &iFlag );

	 if( iFlag == 2 )
		 m_bPalSytleOpen	=	false;
	 else
		 m_bPalSytleOpen	=	true;

	if( uiGlobalStringID != 0 )
	{
		PrintErrorMsg( uiGlobalStringID );
	}
	else
	{
		if( iFlag == 0 )
		{
			CHARACTER_INFO2	stCharInfo2;

			pPacket->ExtractStruct( (LPVOID)&stCharInfo2 , sizeof( CHARACTER_INFO2 ) );

			if( m_pPalPlayer != NULL )
			{
				m_pPalPlayer->InitFigureInfo();
				m_pPalPlayer->SetPlayerClass( stCharInfo2.uiClassType );

				UINT16	iTotalSize;
				pPacket->ExtractUINT16( &iTotalSize );

				FIGURE_ITEM		stFigureItem;

				for( UINT16 ui = 0 ; ui < iTotalSize ; ++ui )
				{
					pPacket->ExtractStruct( &stFigureItem , sizeof(FIGURE_ITEM) );
					m_pPalPlayer->ItemSetting( stFigureItem );
				}

				m_pPalPlayer->RefreshEquipItem();

				m_pPalPlayer->SPGOBPerformMessage( SPGM_SET_SC_CHARINFO , (WPARAM)&stCharInfo2 );
				m_pPalPlayer->SPGOBPerformMessage( SPGM_ITEM_SC_TOGGLE_USE_WEAPON_SET, (WPARAM)stCharInfo2.uiWeaponSet + 1 );

				m_pPalPlayer->SetWeaponSeqItem();

				m_pPalPlayer->SetState( GOB_STATE_STAND , true );

				//SPGOBModelUnit* pkModelUnit = m_pPalPlayer->GetModelUnit();

				//if( pkModelUnit != NULL )
				//{
				//	stCharInfo2.
				//	pkModelUnit->SPSendMessage( MV_SET_ANIMATION_INDEX , GetGOBSeqKey( ) );
				//	pkModelUnit->SPSendMessage( MV_SET_ANI_LOOP , true );
				//}
			}

			PCSTATUS_STYLE	stSelectPalStyle;

			pPacket->ExtractStruct( &stSelectPalStyle , sizeof( PCSTATUS_STYLE ) );

			m_iSelectPalStyle[ MY_STYLE_HAIR ]			=	stSelectPalStyle.uiHair;
			m_iSelectPalStyle[ MY_STYLE_FASHION ]		=	stSelectPalStyle.uiFashion;
			m_iSelectPalStyle[ MY_STYLE_PERSONALITY ]	=	stSelectPalStyle.uiPersonality;
			m_iSelectPalStyle[ MY_STYLE_STATE ]			=	stSelectPalStyle.uiCondition;

			m_bPalPlayerRender	=	true;
		}
	}

	SetRoomPalInfo();

	return 1;
}

int SPWindowChattingRoom::OnPacketMyStyle( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	SetWaittingShow( false , NULL );

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	pPacket->ExtractStruct( &m_stRoomMyStyle , sizeof( PCSTATUS_STYLE ) );

	m_iCurIndexMyStyle[ MY_STYLE_HAIR ]			=	m_stRoomMyStyle.uiHair;
	m_iCurIndexMyStyle[ MY_STYLE_FASHION ]		=	m_stRoomMyStyle.uiFashion;
	m_iCurIndexMyStyle[ MY_STYLE_PERSONALITY ]	=	m_stRoomMyStyle.uiPersonality;
	m_iCurIndexMyStyle[ MY_STYLE_STATE ]		=	m_stRoomMyStyle.uiCondition;

	return 1;
}

int SPWindowChattingRoom::OnRefreshOption( WPARAM wParam, LPARAM lParam )
{
	bool	bBtnEnable	=	true;
	int		iOnOffType	=	(int)wParam;

	if( iOnOffType == 0 )
	{
		SetDefaultWindowRoomSetup();
	}
	else
	{
		bBtnEnable	=	false;
	}

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_TALKING_ROOM , SPIM_CHATTINGROOM_OPTION , (WPARAM)iOnOffType , 0 );

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_CHATTING_ROOM_SETUP_SAVE , true );

	if( pWindow != NULL )
		pWindow->SetEnable( bBtnEnable );

	return 1;
}

int SPWindowChattingRoom::OnClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

int SPWindowChattingRoom::OnRoomMenu( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow*		pWindow;
	unsigned int	uiInstanceID;
	bool			bSelect;

	if( GetActiveMenuWindow( WIID_CHATTING_ROOM_MENU_START , WIID_CHATTING_ROOM_MENU_END , CHATTING_ROOM_MENU_STEP , true ) == iID )
		return 1;

	for( uiInstanceID = WIID_CHATTING_ROOM_MENU_START ; uiInstanceID <= WIID_CHATTING_ROOM_MENU_END ; uiInstanceID += CHATTING_ROOM_MENU_STEP )
	{
		pWindow	=	Find( uiInstanceID );

		if( pWindow != NULL )
		{
			if( uiInstanceID != iID )
				bSelect	=	false;
			else
				bSelect =	true;

			pWindow->SPSendMessage( SPIM_BTN_SELECT , bSelect );
			pWindow->Show();

			pWindow	=	Find( WIID_CHATTING_ROOM + ( uiInstanceID - WIID_CHATTING_ROOM ) * CHATTING_ROOM_MENU_PANEL_STEP );

			if( pWindow != NULL )
			{
				pWindow->SetShowEnable( bSelect );

				if( bSelect == true )
				{
					SetDefaultWindow( iID );
					pWindow->Show();
				}
				else
					pWindow->Hide();
			}
		}
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////

int SPWindowChattingRoom::OnRoomMakeRun( WPARAM wParam, LPARAM lParam )
{
	if( IsPlayerAction() == false )
	{
		PrintErrorMsg( 4000002 );
		return 1;
	}

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		if( pChatManager->IsChatRoom() == true )
		{
			PrintErrorMsg( 5008104 );
			return 1;
		}
	}

	SPIndunManager*	pIndunManager	=	SPIndunManager::GetInstance();

	if( pIndunManager != NULL )
	{
		if( pIndunManager->IsInIndun() == true )
		{
			PrintErrorMsg( 5008100 );
			return 1;
		}
	}

	if( g_pGOBManager == NULL )
		return 1;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	SPPlayerInvenArchive*	pLocalPlayerInvenArchive = pLocalPlayer->GetInvenArchive();

	if( pLocalPlayerInvenArchive == NULL )
		return 1;

	SPAbuseFilter*	pSPAbuseFilter	=	SPAbuseFilter::GetInstance();

	CHATROOM_INFO	stChatRoomInfo	=	{ 0, };
	
	SPWindow*		pWindow;
	const char*		pStringBuf;
	std::string		strBuf;

	memset( &( stChatRoomInfo.szTitle )	, 0 , sizeof( stChatRoomInfo.szTitle )	);

	pWindow	=	Find( WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT , true );

	if( pWindow != NULL )
	{
		strBuf.clear();
		pStringBuf	=	pWindow->GetWindowText();

		if( pStringBuf != NULL )
		{
			if( pSPAbuseFilter != NULL )
			{
				char szBufferSubject[ MAX_CHATROOM_TITLE_LEN + 1 ];
				pSPAbuseFilter->Filter( pStringBuf , szBufferSubject , MAX_CHATROOM_TITLE_LEN );
				strBuf	=	szBufferSubject;
			}
			else
			{
				strBuf	=	pStringBuf;
			}
		}

		if( (int)strBuf.size() < 4 )
		{
			PrintErrorMsg( 5008101 );
			return 1;
		}

		strncpy( stChatRoomInfo.szTitle , strBuf.c_str() , MAX_CHATROOM_TITLE_LEN );
	}

	UINT8			iAdvert				=	0;
	UINT8			iChatRoomAdvertLen	=	0;
	TCHAR			szAdvert[ MAX_CHATROOM_ADVERT_LEN + 1 ];
	TRASH_ITEM		stConsumeItem;

	memset( &( szAdvert )	, 0 , sizeof( szAdvert )	);

	CONTAINER_ITEM	Item;
	ZeroMemory( &Item , sizeof( CONTAINER_ITEM ) );

	pWindow	=	Find( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_EDIT , true );

	if( pWindow != NULL )
	{
		strBuf.clear();
		pStringBuf	=	pWindow->GetWindowText();

		if( pStringBuf != NULL )
		{
			if( pSPAbuseFilter != NULL )
			{
				char szBufferAdvert[ MAX_CHATROOM_ADVERT_LEN + 1 ];
				pSPAbuseFilter->Filter( pStringBuf , szBufferAdvert , MAX_CHATROOM_ADVERT_LEN );
				strBuf	=	szBufferAdvert;
			}
			else
			{
				strBuf	=	pStringBuf;
			}
		}

		iChatRoomAdvertLen	=	(int)strBuf.size();
	}

	int				iRequireItem	=	GetRequireItem();

	if( iRequireItem > 0 )
	{
		SPItemAttr*	pItemAttr	=	NULL;

		if( g_pItemCluster->GetItemInfo( iRequireItem , pItemAttr ) == false )
			return 1;

		if( pItemAttr == NULL )
			return 1;

		CONTAINER_TYPE	eItemType = (CONTAINER_TYPE)( pItemAttr->m_eType - 1 );
		SPItem* pRequireItem = pLocalPlayerInvenArchive->GetSPItem( eItemType , iRequireItem );

		if( pRequireItem != NULL )
		{
			iAdvert	=	1;
			pRequireItem->ConvertContainerItem( &Item );
			++Item.SlotInfo.SlotIndex;

			stConsumeItem.iContainerSlot.ContainerType	=	Item.SlotInfo.ContainerType;
			stConsumeItem.iContainerSlot.SlotIndex		=	Item.SlotInfo.SlotIndex;
			stConsumeItem.iCount						=	1;
		}
		else
		{
			//	해당 아이템 없음
			PrintErrorMsg( 5008103 );
			return 1;
		}

		if( iChatRoomAdvertLen < 4 )
		{
			PrintErrorMsg( 5008102 );
			return 1;
		}

		strncpy( szAdvert , strBuf.c_str() , MAX_CHATROOM_ADVERT_LEN );
	}
	else
	{
		if( iChatRoomAdvertLen > 0 )
		{
			PrintErrorMsg( 5008147 );
			return 1;
		}
	}

	stChatRoomInfo.iCategory	=	m_iCurIndexRoomMake[ ROOM_STYLE_TYPE ];
	stChatRoomInfo.iSex			=	m_iCurIndexRoomMake[ ROOM_STYLE_GENDER ];
	stChatRoomInfo.iAge			=	m_iCurIndexRoomMake[ ROOM_STYLE_AGE ];

	pWindow	=	Find( WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_NUMBER , true );

	if( pWindow != NULL )
	{
		strBuf.clear();
		pStringBuf	=	pWindow->GetWindowText();

		if( pStringBuf != NULL )
			strBuf	=	pStringBuf;

		if( (int)strBuf.size() > 0 )
		{
			stChatRoomInfo.iLimitNum	=	atoi( strBuf.c_str() );

			int	iRequireRoomMax	=	0;

			if( pChatManager != NULL )
				iRequireRoomMax	=	pChatManager->GetRequire( CHAT_LDT_REQUIRE_ROOM_MAX );

			if( stChatRoomInfo.iLimitNum > iRequireRoomMax )
				stChatRoomInfo.iLimitNum	=	iRequireRoomMax;
		}
	}

	pWindow	=	Find( WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_PASSWORD , true );

	if( pWindow != NULL )
	{
		pStringBuf	=	pWindow->GetWindowText();

		if( pStringBuf != NULL )
			strBuf	=	pStringBuf;

		if( (int)strBuf.size() > 0 )
			strncpy( stChatRoomInfo.szPassword , strBuf.c_str() , MAX_CHATROOM_PASSWORD_LEN );
	}

	if( g_pGOBManager != NULL )
	{
		SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer != NULL )
		{
			RECT	rcBound;

			rcBound.left	=	(LONG)pLocalPlayer->GetPosX()	-	128;
			rcBound.top		=	(LONG)pLocalPlayer->GetPosY()	-	227;
			rcBound.right	=	rcBound.left					+	256;
			rcBound.bottom	=	rcBound.top						+	227;

			if( g_pGOBManager->IsChatBoardRect( rcBound , false ) == true || g_pGOBManager->IsUserShopRect( rcBound ) == false )
			{
				PrintErrorMsg( 12000008 );
				return 1;
			}
		}
	}

	CPacket	Packet;
	Packet.AddUINT32( CHATROOM_CS_CREATE );
	Packet.AddData( &( stChatRoomInfo ) , sizeof( CHATROOM_INFO ) );
	Packet.AddUINT8( iAdvert );

	if( iAdvert > 0 )
	{
		Packet.AddData( &( stConsumeItem ) , sizeof( TRASH_ITEM ) );
		Packet.AddUINT8( iChatRoomAdvertLen );
		Packet.Add( (TCHAR*)szAdvert , iChatRoomAdvertLen );
	}

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true && pChatManager->GetJoin() == false )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

			if( g_pCheckManager != NULL && iAdvert > 0 )
				g_pCheckManager->SetDBBlock( true );

			pChatManager->SetJoin( true );

			const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008123 );

			if( pszGlobalString != NULL )
				SetWaittingShow( true , pszGlobalString );
			else
				SetWaittingShow( true , "Wait~~" );
		}
	}

	return 1;
}

int SPWindowChattingRoom::OnRoomMakeClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

int SPWindowChattingRoom::OnRoomMakeAdvUse( WPARAM wParam, LPARAM lParam )
{
	SetButtonCheckInv( Find( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_USE , true ) );
	return 1;
}

int SPWindowChattingRoom::OnRoomMakeTypeLeft( WPARAM wParam, LPARAM lParam )
{
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_TYPE , --m_iCurIndexRoomMake[ ROOM_STYLE_TYPE ] , Find( WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_ROOM_TYPE , true ) );
	SetAutoRoomSubject( Find( WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT , true ) , m_iCurIndexRoomMake[ ROOM_STYLE_TYPE ] + 1 );
	return 1;
}

int SPWindowChattingRoom::OnRoomMakeTypeRight( WPARAM wParam, LPARAM lParam )
{
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_TYPE , ++m_iCurIndexRoomMake[ ROOM_STYLE_TYPE ] , Find( WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_ROOM_TYPE , true ) );
	SetAutoRoomSubject( Find( WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT , true ) , m_iCurIndexRoomMake[ ROOM_STYLE_TYPE ] + 1 );
	return 1;
}

int SPWindowChattingRoom::OnRoomMakeGenderLeft( WPARAM wParam, LPARAM lParam )
{
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_GENDER , --m_iCurIndexRoomMake[ ROOM_STYLE_GENDER ] , Find( WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_GENDER , true ) );
	return 1;
}

int SPWindowChattingRoom::OnRoomMakeGenderRight( WPARAM wParam, LPARAM lParam )
{
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_GENDER , ++m_iCurIndexRoomMake[ ROOM_STYLE_GENDER ] , Find( WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_GENDER , true ) );
	return 1;
}

int SPWindowChattingRoom::OnRoomMakeAgeLeft( WPARAM wParam, LPARAM lParam )
{
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_AGE , --m_iCurIndexRoomMake[ ROOM_STYLE_AGE ] , Find( WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_AGE , true ) );
	return 1;
}

int SPWindowChattingRoom::OnRoomMakeAgeRight( WPARAM wParam, LPARAM lParam )
{
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_AGE , ++m_iCurIndexRoomMake[ ROOM_STYLE_AGE ] , Find( WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_AGE , true ) );
	return 1;
}

int SPWindowChattingRoom::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );
	return 1;
}

int SPWindowChattingRoom::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );
	return 1;
}

int SPWindowChattingRoom::OnNumberEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	OnEditKillFocus( wParam , lParam );

	SPWindow*	pWindow	=	Find( WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_NUMBER , true );

	if( pWindow != NULL )
	{
		SPChatManager*	pChatManager	=	SPChatManager::GetInstance();
		const char*		pStringBuf;
		std::string		strBuf;

		strBuf.clear();
		pStringBuf	=	pWindow->GetWindowText();

		if( pStringBuf != NULL )
			strBuf	=	pStringBuf;

		if( (int)strBuf.size() > 0 )
		{
			int	iRoomMemberCount	=	atoi( strBuf.c_str() );

			int	iRequireRoomMax	=	0;

			if( pChatManager != NULL )
				iRequireRoomMax	=	pChatManager->GetRequire( CHAT_LDT_REQUIRE_ROOM_MAX );

			if( iRoomMemberCount > iRequireRoomMax )
			{
				char szBuf[ _MAX_PATH ];
				sprintf( szBuf , "%d" , iRequireRoomMax );
				pWindow->SetWindowText( szBuf );
			}
		}
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////

int SPWindowChattingRoom::OnRoomFindClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

int SPWindowChattingRoom::OnRoomFindJoin( WPARAM wParam, LPARAM lParam )
{
	int	iRoomNumber	=	(int)wParam;

	const char*	pstrRoomNumber	=	NULL;
	SPWindow*	pWindow	=	Find( WIID_CHATTING_ROOM_FIND_ROOM_NUMBER , true );

	if( pWindow != NULL )
		pstrRoomNumber	=	pWindow->GetWindowText();

	 UINT32	uiRoomNo	=	0;

	if( pstrRoomNumber != NULL )
		uiRoomNo	=	(UINT32)( atoi( pstrRoomNumber ) );

	if( uiRoomNo <= 0 )
	{
		uiRoomNo	=	iRoomNumber;

		if( uiRoomNo <= 0 )
			return 1;
	}

	CPacket	Packet;
	Packet.AddUINT32( CHATROOM_CS_ROOMINFO_CHECK );
	Packet.AddUINT32( uiRoomNo );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

			const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008123 );

			if( pszGlobalString != NULL )
				SetWaittingShow( true , pszGlobalString );
			else
				SetWaittingShow( true , "Wait~~" );
		}
	}

	return 1;
}

int SPWindowChattingRoom::OnRoomFindSliderUp( WPARAM wParam, LPARAM lParam )
{
	SetSliderUpPage( WIID_CHATTING_ROOM_FIND_SLIDER );
	return 1;
}

int SPWindowChattingRoom::OnRoomFindSliderDown( WPARAM wParam, LPARAM lParam )
{
	SetSliderDownPage( WIID_CHATTING_ROOM_FIND_SLIDER );
	return 1;
}

int SPWindowChattingRoom::OnRoomFindRefresh( WPARAM wParam, LPARAM lParam )
{
	//	서버 패킷 요청 버튼 비활성화
	UINT8	iRoomType	=	0;

	iRoomType = (UINT8)( GetActiveMenuWindow( WIID_CHATTING_ROOM_FIND_MENU_START , WIID_CHATTING_ROOM_FIND_MENU_END , 1 , true ) - WIID_CHATTING_ROOM_FIND_MENU_START );

	CPacket	Packet;
	Packet.AddUINT32( CHATROOM_CS_ROOMLIST );
	Packet.AddUINT8( iRoomType );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

			const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008123 );

			if( pszGlobalString != NULL )
				SetWaittingShow( true , pszGlobalString );
			else
				SetWaittingShow( true , "Wait~~" );
		}
	}

	return 1;
}

int SPWindowChattingRoom::OnRoomFindSlider( WPARAM wParam, LPARAM lParam )
{
	int	iIndex	=	GetActiveMenuWindow( WIID_CHATTING_ROOM_FIND_MENU_START , WIID_CHATTING_ROOM_FIND_MENU_END , 1 , true ) - WIID_CHATTING_ROOM_FIND_MENU_START + 1;

	m_iCurRoomFindPage[ iIndex ]	=	GetSliderCurPage( WIID_CHATTING_ROOM_FIND_SLIDER );

	UpdateRoomFindData( (ROOM_TYPE_ID)( iIndex ) );
	return 1;
}

int SPWindowChattingRoom::OnRoomFindTypeMenu( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow*		pWindow;
	unsigned int	uiInstanceID;
	bool			bSelect;

	if( GetActiveMenuWindow( WIID_CHATTING_ROOM_FIND_MENU_START , WIID_CHATTING_ROOM_FIND_MENU_END , 1 , true ) == iID )
		return 1;

	for( uiInstanceID = WIID_CHATTING_ROOM_FIND_MENU_START ; uiInstanceID <= WIID_CHATTING_ROOM_FIND_MENU_END ; uiInstanceID += 1 )
	{
		pWindow	=	Find( uiInstanceID , true );

		if( pWindow != NULL )
		{
			if( uiInstanceID != iID )
				bSelect	=	false;
			else
				bSelect =	true;

			pWindow->SPSendMessage( SPIM_BTN_SELECT , bSelect );
		}
	}

//	m_iSelectIndex[ SLIDER_SELECT_ROOM_FIND ]	=	-1;

	int	iIndex	=	iID - WIID_CHATTING_ROOM_FIND_MENU_START + 1;

	m_bRoomDataZero[ iIndex ]	=	false;
	SetSliderCurPage( WIID_CHATTING_ROOM_FIND_SLIDER , CHATTING_ROOM_FIND_LIST_COUNT , m_iCurRoomFindPage[ iIndex ] , GetRoomDataCount( iIndex ) );
	UpdateRoomFindData( (ROOM_TYPE_ID)( iIndex ) );

	return 1;
}

int SPWindowChattingRoom::OnRoomFindBar( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow*		pWindow;
	unsigned int	uiInstanceID	=	WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG;
	unsigned int	uiInstanceIDEnd	=	WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG + CHATTING_ROOM_FIND_LIST_COUNT;
	int				iSelect;

	if( GetActiveMenuWindow( uiInstanceID , uiInstanceIDEnd , 1 , true ) == iID )
		return 1;

	if( SetRoomFindRoomNo( iID - WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG ) == false )
		return 1;

	for( uiInstanceID = WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG ; uiInstanceID <= uiInstanceIDEnd ; uiInstanceID += 1 )
	{
		pWindow	=	Find( uiInstanceID , true );

		if( pWindow != NULL )
		{
			if( uiInstanceID != iID )
				iSelect	=	1;
			else
				iSelect =	0;

			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iSelect );
		}
	}

	return 1;
}

int SPWindowChattingRoom::OnRoomFindBarDblClick( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( SetRoomFindRoomNo( iID - WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG ) == false )
		return 1;

	OnRoomFindJoin( wParam , lParam );
	return 1;
}

int SPWindowChattingRoom::OnRoomFindSliderWheelUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnRoomFindSliderUp( wParam , lParam );
}

int SPWindowChattingRoom::OnRoomFindSliderWheelDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnRoomFindSliderDown( wParam , lParam );
}

int SPWindowChattingRoom::OnRoomFindToolTipEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	CHATROOM_INFO*	pChatRoomInfo	=	GetRoomData( iID - WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG ,	GetSliderCurPage( WIID_CHATTING_ROOM_FIND_SLIDER ) , GetActiveMenuWindow( WIID_CHATTING_ROOM_FIND_MENU_START , WIID_CHATTING_ROOM_FIND_MENU_END , 1 , true ) - WIID_CHATTING_ROOM_FIND_MENU_START + 1 );

	if( pChatRoomInfo == NULL )
		return 1;

	SPWindow*				pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();
	TOOLTIP_NORMAL_DATA		stToolTipData	=	{ 0, };

	stToolTipData.m_strName	=	pChatRoomInfo->szMasterName;
	stToolTipData.m_cName	=	D3DCOLOR_ARGB(	255	,	0	,	0	,	0	);

	stToolTipData.m_cDesc	=	D3DCOLOR_ARGB(	255	,	0	,	0	,	0	);
	stToolTipData.m_strDesc	=	pChatRoomInfo->szTitle;

	if( pToolTipWindow != NULL )
		pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)(&stToolTipData), SPWindowToolTip::TOOLTIP_SHOW_TYPE_NORMAL );

	return 1;
}

int SPWindowChattingRoom::OnRoomFindToolTipOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow*	pWindowToolTip	=	g_pInterfaceManager->GetToolTipWindow();

	if( pWindowToolTip != NULL )
		pWindowToolTip->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , -2 );

	return 1;
}

int SPWindowChattingRoom::OnRoomPalClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

int SPWindowChattingRoom::OnRoomPalRefresh( WPARAM wParam, LPARAM lParam )
{
	const char*		pszString;
	UINT16			iTempMinLevel	=	0;
	UINT16			iTempMaxLevel	=	0;

	SPWindow*		pWindow;
	SPWindowButton*	pWindowButton;

	for( int i = WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_ALL ; i <= WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_WOMAN ; ++i )
	{
		pWindowButton	=	(SPWindowButton*)Find( i , true );

		if( pWindowButton != NULL )
		{
			if( pWindowButton->IsChecked() == true )
			{
				m_iRoomPalSex	=	i - WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_ALL;
				break;
			}
		}
	}

	pWindow	=	Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_EDIT_MIN , true );

	if( pWindow != NULL )
	{
		pszString	=	pWindow->GetWindowText();

		if( pszString != NULL )
			iTempMinLevel	=	(UINT16)atoi( pszString );
	}

	pWindow	=	Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_EDIT_MAX , true );

	if( pWindow != NULL )
	{
		pszString	=	pWindow->GetWindowText();

		if( pszString != NULL )
			iTempMaxLevel	=	(UINT16)atoi( pszString );
	}

	m_iRoomPalMinLevel	=	min( iTempMinLevel , iTempMaxLevel );
	m_iRoomPalMaxLevel	=	max( iTempMinLevel , iTempMaxLevel );

	if( m_iRoomPalMinLevel <= 1 )
		m_iRoomPalMinLevel	=	1;

	if( m_iRoomPalMaxLevel > 200 )
		m_iRoomPalMaxLevel	=	200;

	m_stRoomPalStyle.uiHair			=	m_iCurIndexPalSearchStyle[ MY_STYLE_HAIR ];
	m_stRoomPalStyle.uiFashion		=	m_iCurIndexPalSearchStyle[ MY_STYLE_FASHION ];
	m_stRoomPalStyle.uiPersonality	=	m_iCurIndexPalSearchStyle[ MY_STYLE_PERSONALITY ];
	m_stRoomPalStyle.uiCondition	=	m_iCurIndexPalSearchStyle[ MY_STYLE_STATE ];

	//////////////////////////////////////////////////////////////////////////

	CPacket	Packet;
	Packet.AddUINT32( CHATROOM_CS_USERLIST );
	Packet.AddUINT8( m_iRoomPalSex );
	Packet.AddUINT16( m_iRoomPalMinLevel );
	Packet.AddUINT16( m_iRoomPalMaxLevel );
	Packet.AddData( &( m_stRoomPalStyle ) , sizeof( PCSTATUS_STYLE ) );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

			const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008125 );

			if( pszGlobalString != NULL )
				SetWaittingShow( true , pszGlobalString );
			else
				SetWaittingShow( true , "Wait~~" );
		}
	}

	return 1;
}

int SPWindowChattingRoom::OnRoomPalSearch( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH ) , true );
	return 1;
}

int SPWindowChattingRoom::OnRoomPalInvite( WPARAM wParam, LPARAM lParam )
{
	if( m_iSelectIndex[ SLIDER_SELECT_PAL ] < 0 )
		return 1;

	int	iMaxCount = (int)m_vRoomPal.size();

	if( m_iSelectIndex[ SLIDER_SELECT_PAL ] >= iMaxCount )
		return 1;

	CHARACTER_BASICINFO*	pCharBasicInfo	=	&( m_vRoomPal.at( m_iSelectIndex[ SLIDER_SELECT_PAL ] ) );

	if( pCharBasicInfo == NULL )
		return 1;

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	UINT8			uiInviteInRoom	=	0;
	CHATROOM_INFO	stChatRoomInfo	=	{ 0, };

	if( pChatManager != NULL )
	{
		if( pChatManager->IsChatRoom() == true )
		{
			uiInviteInRoom					=	1;
			CHATROOM_INFO*	pChatRoomInfo	=	pChatManager->GetChatRoomInfo();

			if( pChatRoomInfo != NULL )
				CopyMemory( &stChatRoomInfo , pChatRoomInfo , sizeof( CHATROOM_INFO ) );
		}
		else
		{
			ROOM_TYPE_ID	eRoomTypeID	=	ROOM_TYPE_NORMAL;

			STYLE_LDT*	pStyleLDT	=	pChatManager->GetStyle( CHAT_LDT_TYPE_ROOM_TYPE , eRoomTypeID , -1 );

			if( pStyleLDT != NULL )
				strncpy( stChatRoomInfo.szTitle , pStyleLDT->m_strTitle.c_str() , MAX_CHATROOM_TITLE_LEN );

			stChatRoomInfo.iCategory	=	(int)eRoomTypeID - 1;
			stChatRoomInfo.iLimitNum	=	2;
		}
	}

	CPacket	Packet;
	Packet.AddUINT32( CHATROOM_CS_INVITE_REQ );
	Packet.Add( pCharBasicInfo->szName , sizeof( pCharBasicInfo->szName ) );
	Packet.AddUINT8( uiInviteInRoom );
	Packet.AddData( &( stChatRoomInfo ) , sizeof( CHATROOM_INFO ) );

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );
		}
	}

	return 1;
}

int SPWindowChattingRoom::OnRoomPalMyStyle( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_CHATTING_ROOM_MODAL_STYLE ) , true );
	return 1;
}

int SPWindowChattingRoom::OnRoomPalSliderUp( WPARAM wParam, LPARAM lParam )
{
	SetSliderUpPage( WIID_CHATTING_ROOM_PAL_SLIDER );
	return 1;
}

int SPWindowChattingRoom::OnRoomPalSliderDown( WPARAM wParam, LPARAM lParam )
{
	SetSliderDownPage( WIID_CHATTING_ROOM_PAL_SLIDER );
	return 1;
}

int SPWindowChattingRoom::OnRoomPalSlider( WPARAM wParam, LPARAM lParam )
{
	UpdateRoomPalData();
	return 1;
}

int SPWindowChattingRoom::OnRoomPalBar( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow*		pWindow;
	unsigned int	uiInstanceID	=	WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG;
	unsigned int	uiInstanceIDEnd	=	WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG + CHATTING_ROOM_PAL_LIST_COUNT;
	int				iSelect;

	if( GetActiveMenuWindow( uiInstanceID , uiInstanceIDEnd , 1 , true ) == iID )
		return 1;

	if( SendPacketPCInfo( GetSliderCurPage( WIID_CHATTING_ROOM_PAL_SLIDER ) + iID - WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG ) == false )
		return 1;

	for( uiInstanceID = WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG ; uiInstanceID <= uiInstanceIDEnd ; uiInstanceID += 1 )
	{
		pWindow	=	Find( uiInstanceID , true );

		if( pWindow != NULL )
		{
			if( uiInstanceID != iID )
				iSelect	=	1;
			else
				iSelect =	0;

			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iSelect );
		}
	}

	return 1;
}

int SPWindowChattingRoom::OnRoomPalSliderWheelUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnRoomPalSliderUp( wParam , lParam );
}

int SPWindowChattingRoom::OnRoomPalSliderWheelDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnRoomPalSliderDown( wParam , lParam );
}

int SPWindowChattingRoom::OnRoomSetupSave( WPARAM wParam, LPARAM lParam )
{
	if( g_pClientOption == NULL )
		return 1;

	SERVER_SIDE_OPTION	stSrcSvrOption	=	g_pClientOption->GetServerSideOption();
	SERVER_SIDE_OPTION	stCurSvrOption	=	stSrcSvrOption;

	SPWindowButton*	pWindowButton;

	pWindowButton	=	(SPWindowButton*)Find( WIID_CHATTING_ROOM_SETUP_MY_INFO_SEARCH , true );

	if( pWindowButton != NULL )
	{
		if( pWindowButton->IsChecked() == true )
			stCurSvrOption	|=	SSO_REQ_CHATROOM_UNUSED;
		else
			stCurSvrOption	&=	~SSO_REQ_CHATROOM_UNUSED;
	}

	pWindowButton	=	(SPWindowButton*)Find( WIID_CHATTING_ROOM_SETUP_CHAT_REQUEST , true );

	if( pWindowButton != NULL )
	{
		if( pWindowButton->IsChecked() == true )
			stCurSvrOption	|=	SSO_REQ_CHATROOM_INVITE_DENIAL;
		else
			stCurSvrOption	&=	~SSO_REQ_CHATROOM_INVITE_DENIAL;
	}

	pWindowButton	=	(SPWindowButton*)Find( WIID_CHATTING_ROOM_SETUP_MY_STYLE_OPEN , true );

	if( pWindowButton != NULL )
	{
		if( pWindowButton->IsChecked() == true )
			stCurSvrOption	|=	SSO_REQ_CHATROOM_STYLE_SECRET;
		else
			stCurSvrOption	&=	~SSO_REQ_CHATROOM_STYLE_SECRET;
	}

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true )
		{
			if( stSrcSvrOption != stCurSvrOption )
			{
				g_pClientOption->SetServerSideOption( stCurSvrOption );
				g_pClientOption->SPPerformMessage( SPOAM_COMMIT_TO_SERVER );

				PrintErrorMsg( 5008145 );

				OnRefreshOption( (WPARAM)1 , 0 );
			}	
		}
	}

	return 1;
}

int SPWindowChattingRoom::OnRoomSetupClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

int SPWindowChattingRoom::OnRoomSetupMyInfoSearch( WPARAM wParam, LPARAM lParam )
{
	SetButtonCheckInv( Find( WIID_CHATTING_ROOM_SETUP_MY_INFO_SEARCH , true ) );
	return 1;
}

int SPWindowChattingRoom::OnRoomSetupChatRequest( WPARAM wParam, LPARAM lParam )
{
	SetButtonCheckInv( Find( WIID_CHATTING_ROOM_SETUP_CHAT_REQUEST , true ) );
	return 1;
}

int SPWindowChattingRoom::OnRoomSetupMyStyleOpen( WPARAM wParam, LPARAM lParam )
{
	SetButtonCheckInv( Find( WIID_CHATTING_ROOM_SETUP_MY_STYLE_OPEN , true ) );
	return 1;
}

int SPWindowChattingRoom::OnModalMyStyleClose( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_CHATTING_ROOM_MODAL_STYLE ) , false );
	return 1;
}

int SPWindowChattingRoom::OnModalMyStyleSave( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_CHATTING_ROOM_MODAL_STYLE ) , false );

	//////////////////////////////////////////////////////////////////////////

	PCSTATUS_STYLE	stStyle;

	stStyle.uiHair			=	m_iCurIndexMyStyle[ MY_STYLE_HAIR ];
	stStyle.uiFashion		=	m_iCurIndexMyStyle[ MY_STYLE_FASHION ];
	stStyle.uiPersonality	=	m_iCurIndexMyStyle[ MY_STYLE_PERSONALITY ];
	stStyle.uiCondition		=	m_iCurIndexMyStyle[ MY_STYLE_STATE ];

	//////////////////////////////////////////////////////////////////////////

	CPacket	Packet;
	Packet.AddUINT32( CHATROOM_CS_SET_MYSTYLE );
	Packet.AddData( &( stStyle ) , sizeof( PCSTATUS_STYLE ) );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

			const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008126 );

			if( pszGlobalString != NULL )
				SetWaittingShow( true , pszGlobalString );
			else
				SetWaittingShow( true , "Wait~~" );

			PrintErrorMsg( 5008145 );
		}
	}

	return 1;
}

int SPWindowChattingRoom::OnModalMyStyleArrowLeft( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	int	iInstance	=	iID - WIID_CHATTING_ROOM_MODAL_STYLE_LEFT;
 	int	iIndex		=	iInstance + 1;
	SetViewTitle( CHAT_LDT_TYPE_MY_STYLE , iIndex , --m_iCurIndexMyStyle[ iIndex ] , Find( WIID_CHATTING_ROOM_MODAL_STYLE_TEXT + iInstance , true ) );
	return 1;
}

int SPWindowChattingRoom::OnModalMyStyleArrowRight( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	int	iInstance	=	iID - WIID_CHATTING_ROOM_MODAL_STYLE_RIGHT;
	int	iIndex		=	iInstance + 1;
	SetViewTitle( CHAT_LDT_TYPE_MY_STYLE , iIndex , ++m_iCurIndexMyStyle[ iIndex ] , Find( WIID_CHATTING_ROOM_MODAL_STYLE_TEXT + iInstance , true ) );
	return 1;
}

int SPWindowChattingRoom::OnModalPalSearchClose( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH ) , false );
	return 1;
}

int SPWindowChattingRoom::OnModalPalSearchSearch( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH ) , false );
	OnRoomPalRefresh( wParam , lParam );
	return 1;
}

int SPWindowChattingRoom::OnModalPalSearchGender( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( iID != WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_ALL )
		SetButtonCheck( Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_ALL , true ) , false );

	if( iID != WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_MAN )
		SetButtonCheck( Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_MAN , true ) , false );

	if( iID != WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_WOMAN )
		SetButtonCheck( Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_WOMAN , true ) , false );

	SetButtonCheck( Find( iID , true ) , true );

	return 1;
}

int SPWindowChattingRoom::OnModalPalSearchArrowLeft( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	int	iInstance	=	iID - WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LEFT;
	int	iIndex		=	iInstance + 1;
	SetViewTitle( CHAT_LDT_TYPE_MY_STYLE , iIndex , --m_iCurIndexPalSearchStyle[ iIndex ] , Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT + iInstance , true ) );
	return 1;
}

int SPWindowChattingRoom::OnModalPalSearchArrowRight( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	int	iInstance	=	iID - WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_RIGHT;
	int	iIndex		=	iInstance + 1;
	SetViewTitle( CHAT_LDT_TYPE_MY_STYLE , iIndex , ++m_iCurIndexPalSearchStyle[ iIndex ] , Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT + iInstance , true ) );
	return 1;
}

//////////////////////////////////////////////////////////////////////////

void SPWindowChattingRoom::InitWindowRoomMake( SPWindow* pParentWindow )
{
	SPWindowStatic*			pWindowStatic;
	SPWindowButton*			pWindowButton;
	SPWindowEdit*			pWindowEdit;
	SPWindowEditMultiLine*	pWindowMultiEdit;
	SPWindowStatic*			pChildWindowStatic;
	int						iChildPosX , iChildPosY;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_ARROW , 193 , 52 , 12 , 7 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 328 , 93 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_SETUP_SUBJECT , 20 , 65 , 118 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 393 , 402 );
	pWindowStatic	=	NULL;

	iChildPosX	=	2;
	iChildPosY	=	85;

	pChildWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_SETUP_BG , iChildPosX , iChildPosY , 321 , 120 , pParentWindow );
	pChildWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 162 , 308 );
	pChildWindowStatic->SetSrcSize( 3 , 3 );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_SUBJECT_IMG , 11 - iChildPosX, 96 - iChildPosY , 118 , 13 , pChildWindowStatic );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 393 , 420 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT_BG_LEFT , 129 - iChildPosX, 93 - iChildPosY , 4 , 32 , pChildWindowStatic );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 367 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT_BG_CENTER , 133 - iChildPosX, 93 - iChildPosY , 154 , 32 , pChildWindowStatic );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 367 );
	pWindowStatic->SetSrcSize( 4 , 32 );

	pWindowMultiEdit	=	new SPWindowEditMultiLine( WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT , 138 - 133 , 97 - 93 , 144 , 28 , pWindowStatic );
	pWindowMultiEdit->SetLimitText( MAX_CHATROOM_TITLE_LEN );
	pWindowMultiEdit->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowMultiEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowMultiEdit->SPSendMessage( SPIM_SET_MARGIN , 2 );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT_BG_RIGHT , 287 - iChildPosX, 93 - iChildPosY , 4 , 32 , pChildWindowStatic );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 367 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_IMG , 11 - iChildPosX, 136 - iChildPosY , 118 , 13 , pChildWindowStatic );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 393 , 434 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_EDIT_BG_LEFT , 129 - iChildPosX, 133 - iChildPosY , 4 , 47 , pChildWindowStatic );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 400 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_EDIT_BG_CENTER , 133 - iChildPosX, 133 - iChildPosY , 154 , 47 , pChildWindowStatic );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 400 );
	pWindowStatic->SetSrcSize( 4 , 47 );

	pWindowMultiEdit	=	new SPWindowEditMultiLine( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_EDIT , 138 - 133 , 137 - 133 , 144 , 43 , pWindowStatic );
	pWindowMultiEdit->SetLimitText( MAX_CHATROOM_ADVERT_LEN );
	pWindowMultiEdit->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowMultiEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowMultiEdit->SPSendMessage( SPIM_SET_MARGIN , 2 );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_EDIT_BG_RIGHT , 287 - iChildPosX, 133 - iChildPosY , 4 , 47 , pChildWindowStatic );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 400 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_USE , 53 - iChildPosX , 156 - iChildPosY , 12 , 12 , pChildWindowStatic );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 105 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 105 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 118 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_USE_IMG , 72 - iChildPosX, 156 - iChildPosY , 48 , 13 , pChildWindowStatic );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 463 , 83 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_HELP , 80 - iChildPosX, 184 - iChildPosY , 213 , 14 , pChildWindowStatic );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 88 , 449 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_SETUP_IMG , 24 , 219 , 118 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 393 , 384 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BRACKET_LEFT_TOP , 22 , 240 , 4 , 4 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 322 , 449 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BRACKET_LEFT_MIDDLE , 22 , 244 , 4 , 108 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 322 , 454 );
	pWindowStatic->SetSrcSize( 4 , 4 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BRACKET_LEFT_BOTTOM , 22 , 352 , 4 , 4 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 322 , 459 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BRACKET_RIGHT_TOP , 298 , 240 , 4 , 4 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 327 , 449 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BRACKET_RIGHT_MIDDLE , 298 , 244 , 4 , 108 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 327 , 454 );
	pWindowStatic->SetSrcSize( 4 , 4 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BRACKET_RIGHT_BOTTOM , 298 , 352 , 4 , 4 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 327 , 459 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_TITLE_ROOM_TYPE , 42 , 242 , 89 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 91 , 496 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_TITLE_GENDER , 42 , 265 , 89 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 50 , 464 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_TITLE_AGE , 42 , 288 , 89 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 1 , 496 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_TITLE_NUMBER , 42 , 315 , 89 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 332 , 448 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_TITLE_PASSWORD , 42 , 338 , 89 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 422 , 448 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_LEFT_ROOM_TYPE , 137 , 243 , 13 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 383 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 397 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 425 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_RIGHT_ROOM_TYPE , 271 , 243 , 13 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 383 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 397 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 425 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BG_LEFT_ROOM_TYPE , 152 , 242 , 4 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 333 );
	pWindowStatic	=	NULL;

	iChildPosX	=	156;
	iChildPosY	=	242;

	pChildWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BG_CENTER_ROOM_TYPE , iChildPosX , iChildPosY , 109 , 15 , pParentWindow );
	pChildWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 333 );
	pChildWindowStatic->SetSrcSize( 4 , 15 );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_ROOM_TYPE , 156 - iChildPosX , 244 - iChildPosY , 109 , 12 , pChildWindowStatic );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_NORMAL );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BG_RIGHT_ROOM_TYPE , 265 , 242 , 4 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 333 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_LEFT_GENDER , 137 , 266 , 13 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 383 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 397 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 425 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_RIGHT_GENDER , 271 , 266 , 13 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 383 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 397 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 425 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BG_LEFT_GENDER , 152 , 265 , 4 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 333 );
	pWindowStatic	=	NULL;

	iChildPosX	=	156;
	iChildPosY	=	265;

	pChildWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BG_CENTER_GENDER , iChildPosX , iChildPosY , 109 , 15 , pParentWindow );
	pChildWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 333 );
	pChildWindowStatic->SetSrcSize( 4 , 15 );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_GENDER , 156 - iChildPosX , 267 - iChildPosY , 109 , 12 , pChildWindowStatic );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_NORMAL );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BG_RIGHT_GENDER , 265 , 265 , 4 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 333 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_LEFT_AGE , 137 , 289 , 13 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 383 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 397 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 425 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_RIGHT_AGE , 271 , 289 , 13 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 383 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 397 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 425 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BG_LEFT_AGE , 152 , 288 , 4 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 333 );
	pWindowStatic	=	NULL;

	iChildPosX	=	156;
	iChildPosY	=	288;

	pChildWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BG_CENTER_AGE , iChildPosX , iChildPosY , 109 , 15 , pParentWindow );
	pChildWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 333 );
	pChildWindowStatic->SetSrcSize( 4 , 15 );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_AGE , 156 - iChildPosX , 290 - iChildPosY , 109 , 12 , pChildWindowStatic );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_NORMAL );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BG_RIGHT_AGE , 265 , 288 , 4 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 333 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BG_LEFT_NUMBER , 139 , 314 , 4 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 349 );
	pWindowStatic	=	NULL;

	iChildPosX	=	143;
	iChildPosY	=	314;

	pChildWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BG_CENTER_NUMBER , iChildPosX , iChildPosY , 135 , 17 , pParentWindow );
	pChildWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 349 );
	pChildWindowStatic->SetSrcSize( 4 , 17 );

	pWindowEdit	=	new SPWindowEdit( WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_NUMBER , 144 - iChildPosX , 317 - iChildPosY , 63 , 12 , pChildWindowStatic );
	pWindowEdit->SetWindowType( WND_TYPE_LEFTBASE );
	pWindowEdit->SetMargin( 0 , 0  );
	pWindowEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowEdit->SetFormat( DT_RIGHT | DT_VCENTER );
	pWindowEdit->SetLimitText( 4 );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_NUMBER_LIMIT , 207 - iChildPosX , 317 - iChildPosY , 63 , 12 , pChildWindowStatic );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_NORMAL );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MAKE_DETAIL_BG_RIGHT_NUMBER , 278 , 314 , 4 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 349 );
	pWindowStatic	=	NULL;

	pWindowEdit	=	new SPWindowEdit( WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_PASSWORD , 139 , 337 , 143 , 17 , pParentWindow );
	pWindowEdit->SetWindowType( WND_TYPE_LEFTBASE );
	pWindowEdit->SetMargin( 5 , 3 );
	pWindowEdit->SetImageNormalHead	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 349 , 384 , 366 );
	pWindowEdit->SetImageNormalBody	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 349 , 388 , 366 );
	pWindowEdit->SetImageNormalTail	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 349 , 392 , 366 );
	pWindowEdit->SetImageFocusHead	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 349 , 384 , 366 );
	pWindowEdit->SetImageFocusBody	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 349 , 388 , 366 );
	pWindowEdit->SetImageFocusTail	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 349 , 392 , 366 );
//	pWindowEdit->SetImageDeco		( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 501 , 501 , 511 , 511 );
	pWindowEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowEdit->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowEdit->SetLimitText( MAX_CHATROOM_PASSWORD_LEN );
	pWindowEdit->SetPassword( true );
	pWindowEdit->SetUseIME( false );

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MAKE_RUN , 86 , 374 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 37 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MAKE_CLOSE , 170 , 374 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 37 );
	pWindowButton	=	NULL;
}

void SPWindowChattingRoom::InitWindowRoomFind( SPWindow* pParentWindow )
{
	SPWindowStatic*			pWindowStatic;
	SPWindowButton*			pWindowButton;
	SPWindowSlider*			pWindowSlider;
	SPWindowEdit*			pWindowEdit;
	int						i , iChildPosX , iChildPosY;
	RECT					rtRect;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_ARROW , 43 , 52 , 12 , 7 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 328 , 93 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_LIST_SUBJECT , 20 , 65 , 118 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 393 , 366 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_FIND_NORMAL , 10 , 85 , 49 , 15 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 388 , 113 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 388 , 129 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 388 , 145 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 388 , 161 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 388 , 145 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_FIND_MARKET , 58 , 85 , 49 , 15 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 438 , 113 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 438 , 129 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 438 , 145 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 438 , 161 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 438 , 145 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_FIND_WANTED , 106 , 85 , 49 , 15 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 364 , 177 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 364 , 193 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 364 , 209 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 364 , 225 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 364 , 209 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_FIND_MEETING , 154 , 85 , 49 , 15 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 414 , 177 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 414 , 193 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 414 , 209 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 414 , 225 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 414 , 209 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_LINE_1 , 10 , 99 , 305 , 1 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 508 , 450 );
	pWindowStatic->SetSrcSize( 2 , 1 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_LINE_2 , 10 , 317 , 305 , 1 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 508 , 450 );
	pWindowStatic->SetSrcSize( 2 , 1 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_TAB_NO , 10 , 105 , 25 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 172 , 480 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_TAB_ROOM_SUBJECT , 39 , 105 , 150 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 294 , 464 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_TAB_AGE , 193 , 105 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 240 , 464 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_TAB_NUMBER , 250 , 105 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 244 , 480 );
	pWindowStatic	=	NULL;

	for( i = 0 ; i < CHATTING_ROOM_FIND_LIST_COUNT ; ++i )
	{
		iChildPosX	=	10;
		iChildPosY	=	125 + i * 19;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG + i , iChildPosX , iChildPosY , 293 , 16 , pParentWindow );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 385 , 339 );
		pWindowStatic->SetImage( (SPTexture*)NULL );
		pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );
		pWindowStatic->SetSrcSize( 3 , 3 );

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_LIST_NO + i , iChildPosX + 3 , iChildPosY + 3 , 19 , 12 , pParentWindow );
		pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
		pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
//		pWindowStatic->SetWindowText( "000" );
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_LIST_LOCK + i , iChildPosX + 29 , iChildPosY + 5 , 7 , 7 , pParentWindow );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 341 , 93 );
		pWindowStatic->SetImage( (SPTexture*)NULL );
		pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_LIST_ROOM_SUBJECT + i , iChildPosX + 40 , iChildPosY + 3 , 135 , 12 , pParentWindow );
		pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
		pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
//		pWindowStatic->SetWindowText( "일이삼사오육칠팔구십일이" );
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_LIST_AGE + i , iChildPosX + 185 , iChildPosY + 3 , 49 , 12 , pParentWindow );
		pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
		pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
//		pWindowStatic->SetWindowText( "일이삼사" );
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_LIST_NUMBER + i , iChildPosX + 244 , iChildPosY + 3 , 45 , 12 , pParentWindow );
		pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
		pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
//		pWindowStatic->SetWindowText( "100/100" );
		pWindowStatic	=	NULL;
	}

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_FIND_SLIDER_UP , 303 , 124 , 12 , 11 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 229 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 229 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 241 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 241 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 241 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_FIND_SLIDER_DOWN , 303 , 302 , 12 , 11 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 303 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 303 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 315 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 315 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 315 );
	pWindowButton	=	NULL;

	pWindowSlider	=	new	SPWindowSlider( WIID_CHATTING_ROOM_FIND_SLIDER , 303 , 135 , 12 , 167 , pParentWindow );

	SetRect( &rtRect , 1 , 506 , 13 , 508 );	//	Area 일반
	pWindowSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 14 , 506 , 26 , 508 );	//	Area 마우스 오버
	pWindowSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 27 , 506 , 39 , 508 );	//	Area 마우스 클릭
	pWindowSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 40 , 506 , 52 , 508 );	//	Area Disable
	pWindowSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 484 , 253 , 496 , 277 );	//	일반
	pWindowSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 497 , 253 , 509 , 277 );	//	마우스 오버
	pWindowSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 484 , 278 , 496 , 302 );	//	마우스 클릭
	pWindowSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 497 , 278 , 509 , 302 );
	pWindowSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	pWindowSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	pWindowSlider->SetCurpage( 0 );
	pWindowSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );
	pWindowSlider->Refresh();

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_FIND_REFRESH , 10 , 322 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 139 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 139 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 139 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 139 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 139 , 37 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_FIND_ROOM_NUMBER_IMG , 170 , 328 , 13 , 6 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 349 , 93 );
	pWindowStatic	=	NULL;

	pWindowEdit	=	new SPWindowEdit( WIID_CHATTING_ROOM_FIND_ROOM_NUMBER , 191 , 322 , 48 , 17 , pParentWindow );
	pWindowEdit->SetWindowType( WND_TYPE_LEFTBASE );
	pWindowEdit->SetMargin( 5 , 4 );
	pWindowEdit->SetImageNormalHead	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 349 , 384 , 366 );
	pWindowEdit->SetImageNormalBody	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 349 , 388 , 366 );
	pWindowEdit->SetImageNormalTail	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 349 , 392 , 366 );
	pWindowEdit->SetImageFocusHead	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 349 , 384 , 366 );
	pWindowEdit->SetImageFocusBody	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 349 , 388 , 366 );
	pWindowEdit->SetImageFocusTail	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 349 , 392 , 366 );
//	pWindowEdit->SetImageDeco		( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 501 , 501 , 511 , 511 );
	pWindowEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowEdit->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowEdit->SetLimitText( 4 );
	pWindowEdit->SetNumber( true );

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_FIND_JOIN , 247 , 322 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 208 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 208 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 208 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 208 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 208 , 37 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_FIND_CLOSE , 128 , 374 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 37 );
	pWindowButton	=	NULL;
}

void SPWindowChattingRoom::InitWindowRoomPal( SPWindow* pParentWindow )
{
	SPWindowStatic*			pWindowStatic;
	SPWindowButton*			pWindowButton;
	SPWindowSlider*			pWindowSlider;
	SPWindowStatic*			pChildWindowStatic;
	int						i , j , iChildPosX , iChildPosY;
	RECT*					prcSrc;
	RECT					rtRect;

	m_pRoomPalWindow	=	pParentWindow;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_ARROW , 118 , 52 , 12 , 7 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 328 , 93 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_TAB_GENDER , 10 , 67 , 45 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 194 , 464 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_TAB_ID , 59 , 67 , 146 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 298 , 480 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_TAB_LV , 209 , 67 , 45 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 198 , 480 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_TAB_CLASS , 258 , 67 , 45 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 181 , 496 );
	pWindowStatic	=	NULL;

	for( i = 0 ; i < CHATTING_ROOM_PAL_LIST_COUNT ; ++i )
	{
		iChildPosX	=	10;
		iChildPosY	=	87 + i * 19;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG + i , iChildPosX , iChildPosY , 293 , 16 , pParentWindow );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 385 , 339 );
		pWindowStatic->SetImage( (SPTexture*)NULL );
		pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );
		pWindowStatic->SetSrcSize( 3 , 3 );

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_LIST_GENDER + i , iChildPosX + 4 , iChildPosY + 3 , 37 , 12 , pParentWindow );
		pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
		pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
//		pWindowStatic->SetWindowText( "남" );
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_LIST_ID + i , iChildPosX + 59 , iChildPosY + 3 , 138 , 12 , pParentWindow );
		pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
		pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
//		pWindowStatic->SetWindowText( "일이삼사오육칠팔구십" );
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_LIST_LV + i , iChildPosX + 203 , iChildPosY + 3 , 37 , 12 , pParentWindow );
		pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
		pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
//		pWindowStatic->SetWindowText( "200" );
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_LIST_CLASS + i , iChildPosX + 264 , iChildPosY + 0 , 16 , 16 , pParentWindow );
		pWindowStatic->SetImage( (SPTexture*)NULL );
		for( j = CLASS_A ; j < CLASS_MAX ; ++j )
		{
			prcSrc	=	g_pInterfaceManager->GetClassTextureSrc( (CLASS_TYPE)j );

			if( prcSrc != NULL )
				pWindowStatic->SetImage( g_pInterfaceManager->GetClassTextureName( (CLASS_TYPE)j ) , prcSrc->left , prcSrc->top );
		}
		pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );
		pWindowStatic	=	NULL;
	}

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_SEARCH_BG , 67 , 118 , 187 , 29 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 1 , 393 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_PAL_SLIDER_UP , 303 , 86 , 12 , 11 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 229 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 229 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 241 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 241 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 241 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_PAL_SLIDER_DOWN , 303 , 169 , 12 , 11 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 303 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 303 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 315 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 315 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 315 );
	pWindowButton	=	NULL;

	pWindowSlider	=	new	SPWindowSlider( WIID_CHATTING_ROOM_PAL_SLIDER , 303 , 97 , 12 , 72 , pParentWindow );

	SetRect( &rtRect , 1 , 506 , 13 , 508 );	//	Area 일반
	pWindowSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 14 , 506 , 26 , 508 );	//	Area 마우스 오버
	pWindowSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 27 , 506 , 39 , 508 );	//	Area 마우스 클릭
	pWindowSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 40 , 506 , 52 , 508 );	//	Area Disable
	pWindowSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 484 , 253 , 496 , 277 );	//	일반
	pWindowSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 497 , 253 , 509 , 277 );	//	마우스 오버
	pWindowSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 484 , 278 , 496 , 302 );	//	마우스 클릭
	pWindowSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	SetRect( &rtRect , 497 , 278 , 509 , 302 );
	pWindowSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , rtRect );

	pWindowSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	pWindowSlider->SetCurpage( 0 );
	pWindowSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );
	pWindowSlider->Refresh();

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_LINE , 10 , 184 , 305 , 1 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 508 , 450 );
	pWindowStatic->SetSrcSize( 2 , 1 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_PAL_REFRESH , 10 , 189 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 139 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 139 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 139 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 139 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 139 , 37 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_PAL_SEARCH , 175 , 189 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 346 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 346 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 346 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 346 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 346 , 37 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_PAL_INVITE , 247 , 189 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 37 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_TITLE , 23 , 214 , 118 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 393 , 348 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_CHAR , 74 , 364 , 1 , 1 , pParentWindow );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_CLASS , 135 , 245 , 16 , 16 , pParentWindow );
	pWindowStatic->SetImage( (SPTexture*)NULL );
	for( j = CLASS_A ; j < CLASS_MAX ; ++j )
	{
		prcSrc	=	g_pInterfaceManager->GetClassTextureSrc( (CLASS_TYPE)j );

		if( prcSrc != NULL )
			pWindowStatic->SetImage( g_pInterfaceManager->GetClassTextureName( (CLASS_TYPE)j ) , prcSrc->left , prcSrc->top );
	}
	pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_LV_ID , 158 , 248 , 164 , 12 , pParentWindow );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowStatic->SetWindowText( "lv 200. 아찌야" );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_CHAR_SILHOUETTE , 41 , 254 , 38 , 112 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 386 , 103 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_STYLE_DEFAULT_BG , 85 , 287 , 221 , 29 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 1 , 65 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_STYLE_NOT_OPEN_PAL_BG , 94 , 287 , 187 , 29 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 1 , 363 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_STYLE_NOT_OPEN_ME_BG , 85 , 287 , 221 , 29 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 1 , 35 );
	pWindowStatic	=	NULL;

	// 머리 텍스트 이미지
	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_STYLE_IMG + 0 , 131 , 269 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 140 , 464 );
	pWindowStatic	=	NULL;

	// 패션 텍스트 이미지
	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_STYLE_IMG + 1 , 131 , 292 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 118 , 480 );
	pWindowStatic	=	NULL;

	// 성격 텍스트 이미지
	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_STYLE_IMG + 2 , 131 , 315 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 10 , 480 );
	pWindowStatic	=	NULL;

	// 상태 텍스트 이미지
	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_STYLE_IMG + 3 , 131 , 338 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 64 , 480 );
	pWindowStatic	=	NULL;

	for( i = 0 ; i < CHATTING_ROOM_PAL_STYLE_COUNT ; ++i )
	{
		iChildPosX	=	196;
		iChildPosY	=	269 + i * 23;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_STYLE_TEXT_BG_LEFT + i , 192 , iChildPosY , 4 , 15 , pParentWindow );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 333 );
		pWindowStatic	=	NULL;

		pChildWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_STYLE_TEXT_BG_CENTER + i , iChildPosX , iChildPosY , 101 , 15 , pParentWindow );
		pChildWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 333 );
		pChildWindowStatic->SetSrcSize( 4 , 15 );

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_STYLE_TEXT + i , 196 - iChildPosX , ( 271 + i * 23 ) - iChildPosY , 101 , 12 , pChildWindowStatic );
		pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
		pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
		pWindowStatic->SetFont( FONT_12_NORMAL );
		pWindowStatic->SetWindowText( "하하하하" );
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_PAL_STYLE_TEXT_BG_RIGHT + i , 297 , iChildPosY , 4 , 15 , pParentWindow );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 333 );
		pWindowStatic	=	NULL;
	}

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_PAL_MY_STYLE , 86 , 374 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 37 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_PAL_CLOSE , 170 , 374 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 37 );
	pWindowButton	=	NULL;
}

void SPWindowChattingRoom::InitWindowRoomSetup( SPWindow* pParentWindow )
{
	SPWindowButton*			pWindowButton;
	SPWindowStatic*			pWindowStatic;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_SETUP_ARROW , 268 , 52 , 12 , 7 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 328 , 93 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_SETUP_TITLE , 16 , 80 , 118 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 393 , 312 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_SETUP_MY_INFO_SEARCH , 34 , 113 , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 105 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 105 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 118 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_SETUP_MY_INFO_SEARCH_TEXT , 57 , 111 , 104 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 407 , 246 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_SETUP_CHAT_REQUEST , 34 , 133 , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 105 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 105 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 118 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_SETUP_CHAT_REQUEST_TEXT , 57 , 131 , 104 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 407 , 262 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_SETUP_MY_STYLE_OPEN , 34 , 153 , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 105 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 105 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 118 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_SETUP_MY_STYLE_OPEN_TEXT , 57 , 151 , 104 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 407 , 278 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_SETUP_SAVE , 86 , 374 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 73 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 91 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 109 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 129 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 109 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_SETUP_CLOSE , 170 , 374 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 37 );
	pWindowButton	=	NULL;
}

void SPWindowChattingRoom::InitWindowWaitting( SPWindow* pParentWindow )
{
	SPWindowStatic*			pWindowStatic;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_WAITTING_BG , 30 , 163 , 265 , 76 , pParentWindow );
	pWindowStatic->SetWindowType( WND_TYPE_NINEBASE );
	pWindowStatic->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 452 , 452 , 4 , 1 );

	pParentWindow = pWindowStatic;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_WAITTING_TEXT , 87 , 24 , 162 , 26 , pParentWindow );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) );
	pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_BOLD );
	pWindowStatic->SetWindowText( "하하하하하하하하하하하하하하하하하하하하하하하하하하하하" );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_WAITTING_WHEEL_1 , 12 , 5 , 35 , 35 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 224 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 260 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 296 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 332 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 368 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 404 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 440 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 476 , 476 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_WAITTING_WHEEL_2 , 31 , 26 , 45 , 45 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 476 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 440 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 404 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 368 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 332 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 296 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 260 , 476 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 224 , 476 );
	pWindowStatic->SetSrcSize( 35 , 35 );
	pWindowStatic	=	NULL;
}

void SPWindowChattingRoom::InitWindowStyle( SPWindow* pParentWindow )
{
	SPWindowButton*			pWindowButton;
	SPWindowStatic*			pWindowStatic;
	SPWindowStatic*			pChildWindowStatic;
	int						i;
	int						iChildPosX , iChildPosY;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_STYLE_BG , 35 , 122 , 254 , 158 , pParentWindow );
	pWindowStatic->SetWindowType( WND_TYPE_NINEBASE );
	pWindowStatic->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 497 , 467 , 4 , 1 );

	pParentWindow = pWindowStatic;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_STYLE_CLOSE , 233 , 11 , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 53 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 53 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 66 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 66 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 66 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_STYLE_TITLE , 17 , 6 , 118 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 393 , 312 );
	pWindowStatic	=	NULL;

	// 머리 텍스트 이미지
	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_STYLE_IMG + 0 , 25 , 35 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 140 , 464 );
	pWindowStatic	=	NULL;

	// 패션 텍스트 이미지
	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_STYLE_IMG + 1 , 25 , 58 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 118 , 480 );
	pWindowStatic	=	NULL;

	// 성격 텍스트 이미지
	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_STYLE_IMG + 2 , 25 , 81 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 10 , 480 );
	pWindowStatic	=	NULL;

	// 상태 텍스트 이미지
	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_STYLE_IMG + 3 , 25 , 104 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 64 , 480 );
	pWindowStatic	=	NULL;

	for( i = 0 ; i < CHATTING_ROOM_PAL_STYLE_COUNT ; ++i )
	{
		iChildPosX	=	103;
		iChildPosY	=	35 + i * 23;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_STYLE_TEXT_BG_LEFT + i , 99 , iChildPosY , 4 , 15 , pParentWindow );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 333 );
		pWindowStatic	=	NULL;

		pChildWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_STYLE_TEXT_BG_CENTER + i , iChildPosX , iChildPosY , 109 , 15 , pParentWindow );
		pChildWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 333 );
		pChildWindowStatic->SetSrcSize( 4 , 15 );

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_STYLE_TEXT + i , 103 - iChildPosX , ( 37 + i * 23 ) - iChildPosY , 109 , 12 , pChildWindowStatic );
		pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
		pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
		pWindowStatic->SetFont( FONT_12_NORMAL );
		pWindowStatic->SetWindowText( "하하하하" );
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_STYLE_TEXT_BG_RIGHT + i , 212 , iChildPosY , 4 , 15 , pParentWindow );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 333 );
		pWindowStatic	=	NULL;

		pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_STYLE_LEFT + i , 84 , iChildPosY + 1 , 13 , 13 , pParentWindow );
		pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 383 );
		pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 397 );
		pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
		pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 425 );
		pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
		pWindowButton	=	NULL;

		pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_STYLE_RIGHT + i , 218 , iChildPosY + 1 , 13 , 13 , pParentWindow );
		pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 383 );
		pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 397 );
		pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
		pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 425 );
		pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
		pWindowButton	=	NULL;
	}

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_STYLE_SAVE , 58 , 131 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 73 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 91 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 109 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 127 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 109 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_STYLE_CANCEL , 134 , 131 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 145 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 163 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 181 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 199 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 181 );
	pWindowButton	=	NULL;
}

void SPWindowChattingRoom::InitWindowPalSearch( SPWindow* pParentWindow )
{
	SPWindowButton*			pWindowButton;
	SPWindowEdit*			pWindowEdit;
	SPWindowStatic*			pWindowStatic;
	SPWindowStatic*			pChildWindowStatic;
	int						i;
	int						iChildPosX , iChildPosY;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_BG , 43 , 82 , 239 , 238 , pParentWindow );
	pWindowStatic->SetWindowType( WND_TYPE_NINEBASE );
	pWindowStatic->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 497 , 467 , 4 , 1 );

	pParentWindow = pWindowStatic;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_CLOSE , 218 , 11 , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 53 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 53 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 66 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 66 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 66 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TITLE , 18 , 6 , 118 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 393 , 294 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_IMG , 116 , 33 , 85 , 14 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 425 , 201 );
	pWindowStatic	=	NULL;

	iChildPosX	=	140;
	iChildPosY	=	50;

	pChildWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_BG , iChildPosX , iChildPosY , 69 , 17 , pParentWindow );
	pChildWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 310 , 414 );

	pWindowEdit	=	new SPWindowEdit( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_EDIT_MIN , 143 - iChildPosX , 54 - iChildPosY , 21 , 12 , pChildWindowStatic );
	pWindowEdit->SetWindowType( WND_TYPE_LEFTBASE );
	pWindowEdit->SetMargin( 0 , 0 );
	pWindowEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowEdit->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowEdit->SetLimitText( 3 );
	pWindowEdit->SetNumber( true );
	pWindowEdit->SetUseIME( false );

	pWindowEdit	=	new SPWindowEdit( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_EDIT_MAX , 185 - iChildPosX , 54 - iChildPosY , 21 , 12 , pChildWindowStatic );
	pWindowEdit->SetWindowType( WND_TYPE_LEFTBASE );
	pWindowEdit->SetMargin( 0 , 0 );
	pWindowEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowEdit->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowEdit->SetLimitText( 3 );
	pWindowEdit->SetNumber( true );
	pWindowEdit->SetUseIME( false );

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_IMG , 26 , 33 , 85 , 14 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 425 , 216 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_ALL , 31 , 54 , 10 , 10 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 440 , 433 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 451 , 433 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 462 , 433 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 473 , 433 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 462 , 433 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_MAN , 31 , 70 , 10 , 10 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 440 , 433 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 451 , 433 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 462 , 433 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 473 , 433 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 462 , 433 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_WOMAN , 31 , 86 , 10 , 10 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 440 , 433 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 451 , 433 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 462 , 433 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 473 , 433 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 462 , 433 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_IMG_ALL , 50 , 52 , 34 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 15 , 464 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_IMG_MAN , 50 , 68 , 34 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 310 , 432 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_IMG_WOMAN , 50 , 84 , 34 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 345 , 432 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_STYLE_IMG , 26 , 108 , 85 , 14 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 425 , 231 );
	pWindowStatic	=	NULL;

	// 머리 텍스트 이미지
	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_IMG + 0 , 25 , 128 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 140 , 464 );
	pWindowStatic	=	NULL;

	// 패션 텍스트 이미지
	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_IMG + 1 , 25 , 147 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 118 , 480 );
	pWindowStatic	=	NULL;

	// 성격 텍스트 이미지
	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_IMG + 2 , 25 , 166 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 10 , 480 );
	pWindowStatic	=	NULL;

	// 상태 텍스트 이미지
	pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_IMG + 3 , 25 , 185 , 53 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 64 , 480 );
	pWindowStatic	=	NULL;

	for( i = 0 ; i < CHATTING_ROOM_PAL_STYLE_COUNT ; ++i )
	{
		iChildPosX	=	103;
		iChildPosY	=	128 + i * 19;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT_BG_LEFT + i , 99 , iChildPosY , 4 , 15 , pParentWindow );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 333 );
		pWindowStatic	=	NULL;

		pChildWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT_BG_CENTER + i , iChildPosX , iChildPosY , 93 , 15 , pParentWindow );
		pChildWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 333 );
		pChildWindowStatic->SetSrcSize( 4 , 15 );

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT + i , 103 - iChildPosX , ( 130 + i * 19 ) - iChildPosY , 93 , 12 , pChildWindowStatic );
		pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
		pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
		pWindowStatic->SetFont( FONT_12_NORMAL );
		pWindowStatic->SetWindowText( "하하하하" );
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT_BG_RIGHT + i , 196 , iChildPosY , 4 , 15 , pParentWindow );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 333 );
		pWindowStatic	=	NULL;

		pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LEFT + i , 84 , iChildPosY + 1 , 13 , 13 , pParentWindow );
		pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 383 );
		pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 397 );
		pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
		pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 425 );
		pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
		pWindowButton	=	NULL;

		pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_RIGHT + i , 202 , iChildPosY + 1 , 13 , 13 , pParentWindow );
		pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 383 );
		pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 397 );
		pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
		pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 425 );
		pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
		pWindowButton	=	NULL;
	}

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_SEARCH , 48 , 208 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 346 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 346 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 346 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 346 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 346 , 37 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_CANCEL , 124 , 208 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 145 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 163 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 181 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 199 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 181 );
	pWindowButton	=	NULL;
}

void SPWindowChattingRoom::SetModalWindowShow( SPWindow* pWindow , bool bShow )
{
	if( pWindow == NULL )
		return;

	pWindow->SetShowEnable( bShow );

	if( bShow == true )
	{
		SetDefaultWindow( pWindow->GetInstanceID() );
		pWindow->Show();
	}
	else
	{
		pWindow->Hide();
	}
}

void SPWindowChattingRoom::SetButtonCheck( SPWindow* pWindow , bool bCheck )
{
	if( pWindow == NULL )
		return;

	pWindow->SPSendMessage( SPIM_BTN_SELECT , (WPARAM)bCheck );
}

void SPWindowChattingRoom::SetButtonCheckInv( SPWindow* pWindow )
{
	SPWindowButton*	pWindowButton	=	(SPWindowButton*)(pWindow);

	if( pWindowButton == NULL )
		return;

	SetButtonCheck( (SPWindow*)(pWindowButton) , !pWindowButton->IsChecked() );
}

void SPWindowChattingRoom::SetViewTitle( CHAT_LDT_TYPE eChatLDTType , int iStyleID , int& iIndex , SPWindow* pWindow )
{
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager == NULL )
		return;

	int	iCount	=	(int)pChatManager->GetStyleCount( eChatLDTType , iStyleID );

	if( iIndex < 0 )
		iIndex = iCount - 1;

	if( iIndex >= iCount )
		iIndex = 0;

	STYLE_LDT*	pStyleLDT = pChatManager->GetStyle( eChatLDTType , iStyleID ,iIndex );

	if( pStyleLDT != NULL && pWindow != NULL )
		pWindow->SetWindowText( pStyleLDT->m_strTitle.c_str() );
}

void SPWindowChattingRoom::SetShowWindow( SPWindow* pWindow , bool bShow )
{
	if( pWindow == NULL )
		return;

	pWindow->SetShowEnable( bShow );
	pWindow->Hide();

	if( bShow == true )
		pWindow->Show();
}

void SPWindowChattingRoom::SetDefaultWindow( INSTANCE_ID eInstanceID )
{
	switch( eInstanceID )
	{
		case WIID_CHATTING_ROOM_MAKE:				SetDefaultWindowRoomMake();			break;
		case WIID_CHATTING_ROOM_FIND:				SetDefaultWindowRoomFind();			break;
		case WIID_CHATTING_ROOM_PAL:				SetDefaultWindowRoomPal();			break;
		case WIID_CHATTING_ROOM_SETUP:				SetDefaultWindowRoomSetup();		break;
		case WIID_CHATTING_ROOM_MODAL_STYLE:		SetDefaultWindowModalStyle();		break;
		case WIID_CHATTING_ROOM_MODAL_PAL_SEARCH:	SetDefaultWindowModalPalSearch();	break;
	}
}

void SPWindowChattingRoom::SetDefaultWindowRoomMake()
{
	SPWindow*	pParentWindow	=	Find( WIID_CHATTING_ROOM_MAKE_PANEL );

	if( pParentWindow == NULL )
		return;

	pParentWindow->Show();

	SPWindow*	pWindow;

	SetAutoRoomSubject( Find( WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT , true ) , ROOM_TYPE_NORMAL );

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_EDIT , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( NULL );

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_PASSWORD , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( NULL );

	SetButtonCheck( pParentWindow->Find( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_USE , true ) , false );

	for( int i = 0 ; i < ROOM_STYLE_MAX ; ++i )
	{
		m_iCurIndexRoomMake[ i ]	=	0;
	}

	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_TYPE	, m_iCurIndexRoomMake[ ROOM_STYLE_TYPE ]	, pParentWindow->Find( WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_ROOM_TYPE	, true ) );
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_GENDER	, m_iCurIndexRoomMake[ ROOM_STYLE_GENDER ]	, pParentWindow->Find( WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_GENDER		, true ) );
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_AGE		, m_iCurIndexRoomMake[ ROOM_STYLE_AGE ]		, pParentWindow->Find( WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_AGE			, true ) );

	int	iRequireRoomMax	=	0;

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		iRequireRoomMax	=	pChatManager->GetRequire( CHAT_LDT_REQUIRE_ROOM_MAX );

	SetRoomMakeMax( iRequireRoomMax );
}

void SPWindowChattingRoom::SetDefaultWindowRoomFind()
{
	SPWindow*	pParentWindow	=	Find( WIID_CHATTING_ROOM_FIND_PANEL );

	if( pParentWindow == NULL )
		return;

	pParentWindow->Show();

	ClearRoomFindRoomNo();

	for( int i = 0 ; i < ROOM_TYPE_MAX ; ++i )
	{
		m_iCurRoomFindPage[ i ] =	0;
		m_bRoomDataZero[ i ]	=	false;
	}

	OnRoomFindTypeMenu( WIID_CHATTING_ROOM_FIND_NORMAL , NULL , NULL);
}

void SPWindowChattingRoom::SetDefaultWindowRoomPal()
{
	SPWindow*	pParentWindow	=	Find( WIID_CHATTING_ROOM_PAL );

	if( pParentWindow == NULL )
		return;

	pParentWindow->Show();

	ClearRoomPalData();
	UpdateRoomPalData();
}

void SPWindowChattingRoom::SetDefaultWindowRoomSetup()
{
	SPWindow*	pParentWindow	=	Find( WIID_CHATTING_ROOM_SETUP );

	if( pParentWindow == NULL )
		return;

	pParentWindow->Show();

	if( g_pClientOption == NULL )
		return;

	SERVER_SIDE_OPTION	stSrcSvrOption	=	g_pClientOption->GetServerSideOption();

	SetButtonCheck( Find( WIID_CHATTING_ROOM_SETUP_MY_INFO_SEARCH	, true ) , ( ( stSrcSvrOption & SSO_REQ_CHATROOM_UNUSED			) == SSO_REQ_CHATROOM_UNUSED		) );
	SetButtonCheck( Find( WIID_CHATTING_ROOM_SETUP_CHAT_REQUEST		, true ) , ( ( stSrcSvrOption & SSO_REQ_CHATROOM_INVITE_DENIAL	) == SSO_REQ_CHATROOM_INVITE_DENIAL	) );
	SetButtonCheck( Find( WIID_CHATTING_ROOM_SETUP_MY_STYLE_OPEN	, true ) , ( ( stSrcSvrOption & SSO_REQ_CHATROOM_STYLE_SECRET	) == SSO_REQ_CHATROOM_STYLE_SECRET	) );
}

void SPWindowChattingRoom::SetDefaultWindowModalStyle()
{
	m_iCurIndexMyStyle[ MY_STYLE_HAIR ]			=	m_stRoomMyStyle.uiHair;
	m_iCurIndexMyStyle[ MY_STYLE_FASHION ]		=	m_stRoomMyStyle.uiFashion;
	m_iCurIndexMyStyle[ MY_STYLE_PERSONALITY ]	=	m_stRoomMyStyle.uiPersonality;
	m_iCurIndexMyStyle[ MY_STYLE_STATE ]		=	m_stRoomMyStyle.uiCondition;

	for( int i = MY_STYLE_HAIR ; i < MY_STYLE_MAX ; ++i )
	{
		SetViewTitle( CHAT_LDT_TYPE_MY_STYLE , i , m_iCurIndexMyStyle[ i ] , Find( WIID_CHATTING_ROOM_MODAL_STYLE_TEXT + i - 1 , true ) );
	}
}

void SPWindowChattingRoom::SetDefaultWindowModalPalSearch()
{
	OnModalPalSearchGender( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_ALL + m_iRoomPalSex , NULL , NULL );

	char	szBuf[ _MAX_PATH ];

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_EDIT_MIN , true );

	if( pWindow != NULL )
	{
		sprintf( szBuf , "%d" , m_iRoomPalMinLevel );
		pWindow->SetWindowText( szBuf );
	}

	pWindow	=	Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_EDIT_MAX , true );

	if( pWindow != NULL )
	{
		sprintf( szBuf , "%d" , m_iRoomPalMaxLevel );
		pWindow->SetWindowText( szBuf );
	}

	m_iCurIndexPalSearchStyle[ MY_STYLE_HAIR ]			=	m_stRoomPalStyle.uiHair;
	m_iCurIndexPalSearchStyle[ MY_STYLE_FASHION ]		=	m_stRoomPalStyle.uiFashion;
	m_iCurIndexPalSearchStyle[ MY_STYLE_PERSONALITY ]	=	m_stRoomPalStyle.uiPersonality;
	m_iCurIndexPalSearchStyle[ MY_STYLE_STATE ]			=	m_stRoomPalStyle.uiCondition;

	for( int i = MY_STYLE_HAIR ; i < MY_STYLE_MAX ; ++i )
	{
		SetViewTitle( CHAT_LDT_TYPE_MY_STYLE , i , m_iCurIndexPalSearchStyle[ i ] , Find( WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT + i - 1 , true ) );
	}
}

void SPWindowChattingRoom::SetRoomMakeMax( int iMax )
{
	SPWindow*	pWindow	=	Find( WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_NUMBER_LIMIT , true );

	char		szBuf[ _MAX_PATH ];

	if( pWindow != NULL )
	{
		const char*		pszGlobalString;

		pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008117 );

		if( pszGlobalString != NULL )
			sprintf( szBuf , pszGlobalString , iMax );
		else
			sprintf( szBuf , " / %d" , iMax );

		pWindow->SetWindowText( szBuf );
	}

	pWindow	=	Find( WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_NUMBER , true );

	if( pWindow != NULL )
	{
		sprintf( szBuf , "%d" , iMax );

		pWindow->SetWindowText( szBuf );
	}
}

INSTANCE_ID SPWindowChattingRoom::GetActiveMenuWindow( INSTANCE_ID eInstanceStart , INSTANCE_ID eInstanceEnd , int iStep , bool bShow )
{
	SPWindowButton*	pWindowButton;
	unsigned int	uiInstanceID;

	for( uiInstanceID = eInstanceStart ; uiInstanceID <= eInstanceEnd ; uiInstanceID += iStep )
	{
		pWindowButton	=	(SPWindowButton*)Find( uiInstanceID , true );

		if( pWindowButton != NULL )
		{
			if( pWindowButton->IsChecked() == true )
			{
				if( bShow == true )
				{
					if( pWindowButton->IsShow() == true )
						return uiInstanceID;
				}
				else
				{
					return uiInstanceID;
				}
			}
		}
	}

	return 0;
}

void SPWindowChattingRoom::SetAutoRoomSubject( SPWindow* pWindow , int iIndex )
{
	if( pWindow == NULL )
		return;

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager == NULL )
		return;

	STYLE_LDT*	pStyleLDT	=	pChatManager->GetStyle( CHAT_LDT_TYPE_ROOM_TYPE , iIndex , -1 );

	if( pStyleLDT != NULL )
		pWindow->SetWindowText( pStyleLDT->m_strTitle.c_str() );
	else
		pWindow->SetWindowText( NULL );
}

int SPWindowChattingRoom::GetRequireItem()
{
	int	iRequireItem	=	0;

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		iRequireItem	=	pChatManager->GetRequire( CHAT_LDT_REQUIRE_ITEM );

	SPWindowButton*	pWindowButton	=	(SPWindowButton*)Find( WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_USE , true );

	if( pWindowButton != NULL )
	{
		if( pWindowButton->IsChecked() == true && iRequireItem > 0 )
			return iRequireItem;
	}

	return 0;
}

void SPWindowChattingRoom::UpdateRoomFindData( ROOM_TYPE_ID eRoomTypeID )
{
	int	iIndex	=	(int)eRoomTypeID;
	int	iCount	=	GetRoomDataCount( iIndex );;

	if( iCount <= 0 && m_bRoomDataZero[ iIndex ] == false )
	{
		//	서버 리스트 요청
		OnRoomFindRefresh( NULL , NULL );
		return;
	}

	int		i			=	0;
	int		iStartIndex	=	GetSliderCurPage( WIID_CHATTING_ROOM_FIND_SLIDER );
	int		iCalIndex	=	iStartIndex + i;
	int		iSelect;

	if( iStartIndex >= iCount )
		iStartIndex	=	0;

	CHATROOM_INFO*	pRoomFindData;
	STD_MAP_ROOM::iterator	mIter , mIterEnd;

	bool	bResult	=	GetRoomData( mIter , mIterEnd , iCalIndex , iIndex );

	for( i = 0 ; i < CHATTING_ROOM_FIND_LIST_COUNT ; ++i )
	{
		iCalIndex	=	iStartIndex + i;

		if( iCalIndex >= 0 && iCalIndex < iCount && bResult == true && mIter != mIterEnd )
		{
			pRoomFindData	=	&( (*mIter).second );
			++mIter;
		}
		else
		{
			pRoomFindData	=	NULL;
		}

		iSelect	=	1;

		if( pRoomFindData != NULL )
		{
			if( pRoomFindData->uiRoomNo == m_iSelectIndex[ SLIDER_SELECT_ROOM_FIND ] )
				iSelect	=	0;
		}

		//if( iCalIndex == m_iSelectIndex[ SLIDER_SELECT_ROOM_FIND ] && pRoomFindData != NULL )
		//	iSelect = 0;
		//else
		//	iSelect = 1;

		SetRoomFindData( i , iSelect , pRoomFindData );
	}

	SetSliderCurPage( WIID_CHATTING_ROOM_FIND_SLIDER , CHATTING_ROOM_FIND_LIST_COUNT , iStartIndex , iCount );
}

void SPWindowChattingRoom::SetRoomFindData( int iIndex , int iSelect , CHATROOM_INFO* pRoomFindData )
{
	if( iIndex < 0 || iIndex >= CHATTING_ROOM_FIND_LIST_COUNT )
		return;

	SPWindow*	pParentWindow	=	Find( WIID_CHATTING_ROOM_FIND_PANEL );

	if( pParentWindow == NULL )
		return;

	int			iColorIndex	=	0;

	if( pRoomFindData != NULL )
	{
		if( pRoomFindData->iCurrentNum >= pRoomFindData->iLimitNum )
		{
			iColorIndex	=	1;
		}
	}

	char		szBuf[ _MAX_PATH ];
	int			iImageIndex;
	std::string	strBuf;

	SPWindow*	pWindow;

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG + iIndex , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iSelect );

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_FIND_LIST_NO + iIndex , true );

	if( pWindow != NULL )
	{
		if( pRoomFindData != NULL )
		{
			sprintf( szBuf , "%3d" , pRoomFindData->uiRoomNo );
			pWindow->SetWindowText( szBuf );
		}
		else
		{
			pWindow->SetWindowText( NULL );
		}
	}

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_FIND_LIST_LOCK + iIndex , true );

	if( pWindow != NULL )
	{
		if( pRoomFindData != NULL )
		{
			strBuf	=	pRoomFindData->szPassword;

			if( strBuf.empty() == false )
				iImageIndex	=	0;
			else
				iImageIndex	=	1;
		}
		else
		{
			iImageIndex	=	1;
		}

		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iImageIndex );
	}

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_FIND_LIST_ROOM_SUBJECT + iIndex , true );

	if( pWindow != NULL )
	{
		if( pRoomFindData != NULL )
			pWindow->SetWindowText( pRoomFindData->szTitle );
		else
			pWindow->SetWindowText( NULL );

		pWindow->SetFontColor( m_cRoomTextColor[ iColorIndex ] );
	}

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_FIND_LIST_AGE + iIndex , true );

	if( pWindow != NULL )
	{
		if( pRoomFindData != NULL )
		{
			STYLE_LDT*	pStyleLDT	=	SPChatManager::GetInstance()->GetStyle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_AGE , pRoomFindData->iAge );

			if( pStyleLDT != NULL )
				pWindow->SetWindowText( pStyleLDT->m_strTitle.c_str() );
		}
		else
		{
			pWindow->SetWindowText( NULL );
		}
	}

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_FIND_LIST_NUMBER + iIndex , true );

	if( pWindow != NULL )
	{
		if( pRoomFindData != NULL )
		{
			sprintf( szBuf , "%2d/%2d" , pRoomFindData->iCurrentNum , pRoomFindData->iLimitNum );
			pWindow->SetWindowText( szBuf );
		}
		else
		{
			pWindow->SetWindowText( NULL );
		}

		pWindow->SetFontColor( m_cRoomTextColor[ iColorIndex ] );
	}
}

bool SPWindowChattingRoom::SetRoomFindRoomNo( int iIndex )
{
	CHATROOM_INFO*	pChatRoomInfo	=	GetRoomData( iIndex , GetSliderCurPage( WIID_CHATTING_ROOM_FIND_SLIDER ) , GetActiveMenuWindow( WIID_CHATTING_ROOM_FIND_MENU_START , WIID_CHATTING_ROOM_FIND_MENU_END , 1 , true ) - WIID_CHATTING_ROOM_FIND_MENU_START + 1 );

	if( pChatRoomInfo == NULL )
		return false;

	m_iSelectIndex[ SLIDER_SELECT_ROOM_FIND ]	=	pChatRoomInfo->uiRoomNo;

	SPWindow*	pWindow	=	Find( WIID_CHATTING_ROOM_FIND_ROOM_NUMBER , true );

	if( pWindow != NULL )
	{
		char	szBuf[ _MAX_PATH ];

		sprintf( szBuf , "%d" , pChatRoomInfo->uiRoomNo );

		pWindow->SetWindowText( szBuf );
	}

	return true;
}

void SPWindowChattingRoom::ClearRoomFindRoomNo()
{
	m_iSelectIndex[ SLIDER_SELECT_ROOM_FIND ]	=	-1;

	SPWindow*	pWindow	=	Find( WIID_CHATTING_ROOM_FIND_ROOM_NUMBER , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( NULL );
}

int SPWindowChattingRoom::GetSliderCurPage( INSTANCE_ID eInstanceID )
{
	SPWindowSlider*		pWindowSlider	=	(SPWindowSlider*)Find( eInstanceID , true );

	if( pWindowSlider == NULL )
		return 0;

	return pWindowSlider->GetCurPage();
}

void SPWindowChattingRoom::SetSliderCurPage( INSTANCE_ID eInstanceID , int iScreenMaxCount , int iCurPage , int iMaxPage )
{
	SPWindowSlider*		pWindowSlider	=	(SPWindowSlider*)Find( eInstanceID , true );

	if( pWindowSlider == NULL )
		return;

	iMaxPage -= ( iScreenMaxCount - 1 );

	if( iMaxPage < 0 )
		iMaxPage = 0;

	pWindowSlider->SetCurpage( iCurPage );
	pWindowSlider->SPSendMessage( SPIM_SET_MAXVALUE , iMaxPage );
}

void SPWindowChattingRoom::SetSliderUpPage( INSTANCE_ID eInstanceID )
{
	SPWindow*	pWindow	=	Find( eInstanceID , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SLIDER_UP );
}

void SPWindowChattingRoom::SetSliderDownPage( INSTANCE_ID eInstanceID )
{
	SPWindow*	pWindow	=	Find( eInstanceID , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SLIDER_DOWN );
}

void SPWindowChattingRoom::UpdateRoomPalData()
{
	//////////////////////////////////////////////////////////////////////////

	SetRoomPalInfo();

	//////////////////////////////////////////////////////////////////////////

	int	iCount	=	(int)m_vRoomPal.size();

	//if( iCount <= 0 )
	//	return;

	int		i;
	int		iStartIndex	=	GetSliderCurPage( WIID_CHATTING_ROOM_PAL_SLIDER );
	int		iCalIndex;
	int		iSelect;

	CHARACTER_BASICINFO*	pRoomPalData;

	for( i = 0 ; i < CHATTING_ROOM_PAL_LIST_COUNT ; ++i )
	{
		iCalIndex	=	iStartIndex + i;

		if( iCalIndex < 0 || iCalIndex >= iCount )
			pRoomPalData	=	NULL;
		else
			pRoomPalData	=	&( m_vRoomPal.at( iCalIndex ) );

		if( iCalIndex == m_iSelectIndex[ SLIDER_SELECT_PAL ] && pRoomPalData != NULL )
			iSelect = 0;
		else
			iSelect = 1;

		SetRoomPalData( i , iSelect , pRoomPalData );
	}

	SetSliderCurPage( WIID_CHATTING_ROOM_PAL_SLIDER , CHATTING_ROOM_PAL_LIST_COUNT , iStartIndex , iCount );
}

void SPWindowChattingRoom::SetRoomPalData( int iIndex , int iSelect , CHARACTER_BASICINFO* pRoomPalData )
{
	if( iIndex < 0 || iIndex >= CHATTING_ROOM_PAL_LIST_COUNT )
		return;

	SPWindow*	pParentWindow	=	Find( WIID_CHATTING_ROOM_PAL_PANEL );

	if( pParentWindow == NULL )
		return;

	char	szBuf[ _MAX_PATH ];
	int		iImageIndex;

	SPWindow*	pWindow;

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG + iIndex , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iSelect );

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_PAL_LIST_GENDER + iIndex , true );

	if( pWindow != NULL )
	{
		const char*		pszGlobalString	=	NULL;

		if( pRoomPalData != NULL )
		{
			if( pRoomPalData->uiSex == 1)
				pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008119 );
			else
				pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008120 );
		}

		pWindow->SetWindowText( pszGlobalString );
	}

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_PAL_LIST_ID + iIndex , true );

	if( pWindow != NULL )
	{
		if( pRoomPalData != NULL )
		{
			pWindow->SetWindowText( pRoomPalData->szName );
		}
		else
		{
			pWindow->SetWindowText( NULL );
		}
	}

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_PAL_LIST_LV + iIndex , true );

	if( pWindow != NULL )
	{
		if( pRoomPalData != NULL )
		{
			sprintf( szBuf , "%d" , pRoomPalData->uiLevel );
			pWindow->SetWindowText( szBuf );
		}
		else
		{
			pWindow->SetWindowText( NULL );
		}
	}

	pWindow	=	pParentWindow->Find( WIID_CHATTING_ROOM_PAL_LIST_CLASS + iIndex , true );

	if( pWindow != NULL )
	{
		if( pRoomPalData != NULL )
		{
			iImageIndex	=	pRoomPalData->uiClassType;
		}
		else
		{
			iImageIndex	=	0;
		}

		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iImageIndex );
	}
}

void SPWindowChattingRoom::SetRoomPalInfo()
{
	SPWindow*		pWindow;
	SPWindowButton*	pWindowButton;

	//////////////////////////////////////////////////////////////////////////

	bool			bPalSearch		=	m_vRoomPal.empty();

	//////////////////////////////////////////////////////////////////////////

	bool			bMyStyleOpen	=	true;
	bool			bCharSilhouette	=	true;
	bool			bPalStyleOpen	=	false;

	//////////////////////////////////////////////////////////////////////////

	pWindowButton	=	(SPWindowButton*)Find( WIID_CHATTING_ROOM_SETUP_MY_STYLE_OPEN , true );

	if( pWindowButton != NULL )
		bMyStyleOpen	=	!(pWindowButton->IsChecked());

	//////////////////////////////////////////////////////////////////////////

	if( m_iSelectIndex[ SLIDER_SELECT_PAL ] >= 0 )
	{
		if( bMyStyleOpen == false )
		{
			if( m_bPalSytleOpen == false )
			{
				bCharSilhouette		=	true;
				bPalStyleOpen		=	true;
				m_bPalPlayerRender	=	!bCharSilhouette;
			}
			else
			{
				bCharSilhouette	=	!m_bPalPlayerRender;
				bPalStyleOpen	=	false;
			}
		}
		else
		{
			m_bPalPlayerRender	=	false;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	SetShowWindow( Find( WIID_CHATTING_ROOM_PAL_SEARCH_BG				, true ) , bPalSearch		);
	SetShowWindow( Find( WIID_CHATTING_ROOM_PAL_CHAR_SILHOUETTE			, true ) , bCharSilhouette	);
	SetShowWindow( Find( WIID_CHATTING_ROOM_PAL_STYLE_NOT_OPEN_ME_BG	, true ) , bMyStyleOpen		);
	SetShowWindow( Find( WIID_CHATTING_ROOM_PAL_STYLE_NOT_OPEN_PAL_BG	, true ) , bPalStyleOpen	);
	SetShowWindow( Find( WIID_CHATTING_ROOM_PAL_STYLE_DEFAULT_BG		, true ) , !( bMyStyleOpen | bPalStyleOpen ) && bCharSilhouette	);

	//////////////////////////////////////////////////////////////////////////

	int	iIndex	=	0;

	for( int i = MY_STYLE_HAIR ; i < MY_STYLE_MAX ; ++i )
	{
		if( bCharSilhouette == false )
			iIndex	=	m_iSelectPalStyle[ i ];
		else
			iIndex	=	0;

		pWindow	=	Find( WIID_CHATTING_ROOM_PAL_STYLE_TEXT + i - 1 , true );

		SetViewTitle( CHAT_LDT_TYPE_MY_STYLE , i , iIndex , pWindow );
		SetShowWindow( pWindow , !bCharSilhouette	);

		SetShowWindow( Find( WIID_CHATTING_ROOM_PAL_STYLE_IMG + i - 1			, true ) , !bCharSilhouette	);
		SetShowWindow( Find( WIID_CHATTING_ROOM_PAL_STYLE_TEXT_BG_LEFT + i - 1	, true ) , !bCharSilhouette	);
		SetShowWindow( Find( WIID_CHATTING_ROOM_PAL_STYLE_TEXT_BG_CENTER + i - 1, true ) , !bCharSilhouette	);
		SetShowWindow( Find( WIID_CHATTING_ROOM_PAL_STYLE_TEXT_BG_RIGHT + i - 1	, true ) , !bCharSilhouette	);
	}

	//////////////////////////////////////////////////////////////////////////

	char	szBuf[ _MAX_PATH ]	=	{ 0 , };
	int		iClassIndex			=	0;
	int		iMaxPalCount		=	(int)m_vRoomPal.size();

	if( m_iSelectIndex[ SLIDER_SELECT_PAL ] >= 0 && m_iSelectIndex[ SLIDER_SELECT_PAL ] < iMaxPalCount && bCharSilhouette == false )
	{
		CHARACTER_BASICINFO*	pCharBasicInfo	=	&( m_vRoomPal.at( m_iSelectIndex[ SLIDER_SELECT_PAL ] ) );

		if( pCharBasicInfo != NULL )
		{
			iClassIndex	=	pCharBasicInfo->uiClassType;
			sprintf( szBuf , "Lv.%d %s" , pCharBasicInfo->uiLevel , pCharBasicInfo->szName );
		}
	}

	pWindow	=	Find( WIID_CHATTING_ROOM_PAL_CLASS , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iClassIndex );

	pWindow	=	Find( WIID_CHATTING_ROOM_PAL_LV_ID , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( szBuf );
}

void SPWindowChattingRoom::ClearRoomPalData()
{
	m_iSelectIndex[ SLIDER_SELECT_PAL ]		=	-1;
	m_bPalPlayerRender						=	false;
	m_bPalSytleOpen							=	false;
	SetSliderCurPage( WIID_CHATTING_ROOM_PAL_SLIDER , CHATTING_ROOM_PAL_LIST_COUNT , 0 , (int)m_vRoomPal.size() );
}

void SPWindowChattingRoom::SetWaittingShow( bool bShow , const char* pszMsg )
{
	SPWindow*	pWindow	=	Find( WIID_CHATTING_ROOM_MODAL_WAITTING , true );

	if( pWindow == NULL )
		return;

	SetModalWindowShow( pWindow , bShow );

	if( bShow == true )
	{
		pWindow->Show();
	}
	else
	{
		pWindow->Hide();
	}

	SPWindow*	pWindowText	=	pWindow->Find( WIID_CHATTING_ROOM_MODAL_WAITTING_TEXT , true );

	if( pWindowText != NULL )
		pWindowText->SetWindowText( pszMsg );
}

void SPWindowChattingRoom::PrintErrorMsg( GLOBAL_STRING_ID uiGlobalStringID , const char* pszName /* = NULL  */ )
{
	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( uiGlobalStringID );

	if( pszGlobalString == NULL )
		return;

	DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

	bool	bShowName	=	false;

	std::vector< GLOBAL_STRING_ID >::iterator	Iter	=	m_vPrintNameBuffer.begin();

	while( Iter != m_vPrintNameBuffer.end() )
	{
		if( (*Iter) == uiGlobalStringID )
		{
			bShowName	=	true;
			break;
		}

		++Iter;
	}

	if( pszName != NULL && bShowName == true )
	{
		sprintf( szErrorMsg , pszGlobalString , pszName );
	}
	else
	{
		strcpy( szErrorMsg , pszGlobalString );
	}

	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szErrorMsg );
}

void SPWindowChattingRoom::SendPacketRoomJoin( UINT32 uiID , UINT32 uiRoomNo , UINT8 iCategory , TCHAR* pstrPassword , bool bLocalSend )
{
	if( uiID <= 0 || uiRoomNo <= 0 || iCategory < 0 )
		return;

	CHATROOM_INFO	stChatRoomInfo	=	{ 0, };

	if( bLocalSend == false )
	{
		CHATROOM_INFO*	pChatRoomInfo	=	GetRoomData( (int)uiRoomNo , (int)iCategory + 1 );

		if( pChatRoomInfo != NULL )
			CopyMemory( &stChatRoomInfo , pChatRoomInfo , sizeof( CHATROOM_INFO ) );
	}
	else
	{
		stChatRoomInfo.uiID			=	uiID;
		stChatRoomInfo.uiRoomNo		=	uiRoomNo;
		stChatRoomInfo.iCategory	=	iCategory;
	}

	if( pstrPassword != NULL )
		strncpy( stChatRoomInfo.szPassword , pstrPassword , MAX_CHATROOM_PASSWORD_LEN );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		CHATROOM_INFO*	pChatRoomInfo	=	pChatManager->GetChatRoomInfo();

		if( pChatRoomInfo != NULL )
		{
			if( stChatRoomInfo.uiID			== pChatRoomInfo->uiID		&&
				stChatRoomInfo.uiRoomNo		== pChatRoomInfo->uiRoomNo	&&
				stChatRoomInfo.iCategory	== pChatRoomInfo->iCategory	)
			{
				PrintErrorMsg( 5008144 );
				return;
			}
		}
	}

	CPacket	Packet;
	Packet.AddUINT32( CHATROOM_CS_JOIN );
	Packet.AddData( &( stChatRoomInfo ) , sizeof( CHATROOM_INFO ) );

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true && pChatManager->GetJoin() == false )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

			pChatManager->SetJoin( true );

			const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008124 );

			if( pszGlobalString != NULL )
				SetWaittingShow( true , pszGlobalString );
			else
				SetWaittingShow( true , "Wait~~" );
		}
	}
}

bool SPWindowChattingRoom::SendPacketPCInfo( int iSelectIndex )
{
	if( iSelectIndex < 0 )
		return false;

	int	iMaxCount = (int)m_vRoomPal.size();

	if( iSelectIndex >= iMaxCount )
		return false;

	CHARACTER_BASICINFO*	pCharBasicInfo	=	&( m_vRoomPal.at( iSelectIndex ) );

	if( pCharBasicInfo == NULL )
		return false;

	m_iSelectIndex[ SLIDER_SELECT_PAL ]	=	iSelectIndex;

	CPacket	Packet;
	Packet.AddUINT32( CHATROOM_CS_OTHERPC_INFO );
	Packet.Add( pCharBasicInfo->szName , sizeof( pCharBasicInfo->szName ) );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

			const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008123 );

			if( pszGlobalString != NULL )
				SetWaittingShow( true , pszGlobalString );
			else
				SetWaittingShow( true , "Wait~~" );
		}
	}

	return true;
}

void SPWindowChattingRoom::SendPacketInviteAns( UINT8 uiAllowed )
{
	CPacket	Packet;
	Packet.AddUINT32( CHATROOM_CS_INVITE_ANS );
	Packet.AddUINT8( uiAllowed );
	Packet.Add( m_szActorName , sizeof( m_szActorName ) );
	Packet.AddUINT8( m_uiInviteInRoom );
	Packet.AddData( &( m_stChatRoomInfo ) , sizeof( CHATROOM_INFO ) );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true && pChatManager->GetJoin() == false )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

			if( uiAllowed == 1 )
				pChatManager->SetJoin( true );
		}
	}
}

void SPWindowChattingRoom::AddRoomData( int iKey , CHATROOM_INFO& stCharRoomInfo )
{
	if( iKey <= ROOM_TYPE_NULL || iKey >= ROOM_TYPE_MAX )
		return;

	CHATROOM_INFO*	pChatRoomInfo	=	GetRoomData( stCharRoomInfo.uiRoomNo , iKey );

	if( pChatRoomInfo != NULL )
	{
		CopyMemory( pChatRoomInfo , &stCharRoomInfo , sizeof( CHATROOM_INFO ) );
		return;
	}

	m_vRoomData[ iKey ].insert( STD_MAP_ROOM::value_type( stCharRoomInfo.uiRoomNo , stCharRoomInfo ) );
}

CHATROOM_INFO* SPWindowChattingRoom::GetRoomData( int iKey , int iCategory )
{
	int	iStartIndex	=	(int)ROOM_TYPE_NULL	+ 1;
	int	iEndIndex	=	(int)ROOM_TYPE_MAX	- 1;

	if( iCategory > ROOM_TYPE_NULL && iCategory < ROOM_TYPE_MAX )
	{
		iStartIndex	=	iCategory;
		iEndIndex	=	iCategory;
	}

	STD_MAP_ROOM::iterator	mIter;

	for( int i = iStartIndex ; i <= iEndIndex ; ++i )
	{
		mIter	=	m_vRoomData[ i ].find( iKey );

		if( mIter != m_vRoomData[ i ].end() )
			return &( (*mIter).second );
	}

	return NULL;
}

bool SPWindowChattingRoom::GetRoomData( STD_MAP_ROOM::iterator& mOutIter , STD_MAP_ROOM::iterator& mOutIterEnd , int iIndex , int iCategory )
{
	int	iStartIndex	=	(int)ROOM_TYPE_NULL	+ 1;
	int	iEndIndex	=	(int)ROOM_TYPE_MAX	- 1;

	if( iCategory > ROOM_TYPE_NULL && iCategory < ROOM_TYPE_MAX )
	{
		iStartIndex	=	iCategory;
		iEndIndex	=	iCategory;
	}

	int						iCurIndex	=	0;
	STD_MAP_ROOM::iterator	mIter;

	for( int i = iStartIndex ; i <= iEndIndex ; ++i )
	{
		mIter	=	m_vRoomData[ i ].begin();

		while ( mIter != m_vRoomData[ i ].end() )
		{
			if( iCurIndex == iIndex )
			{
				mOutIter	=	mIter;
				mOutIterEnd	=	m_vRoomData[ i ].end();
				return true;
			}

			++mIter;
			++iCurIndex;
		}
	}

	return false;
}

CHATROOM_INFO* SPWindowChattingRoom::GetRoomData( int iIndex , int iCurPage , int iCategory )
{
	CHATROOM_INFO*	pChatRoomInfo	=	NULL;

	if( iIndex < 0 || iIndex >= CHATTING_ROOM_FIND_LIST_COUNT || iCategory < 0 || iCategory >= ROOM_TYPE_MAX )
		return pChatRoomInfo;

	int	iStartIndex	=	iCurPage + iIndex;
	int	iCount		=	GetRoomDataCount( iCategory );

	if( iStartIndex < 0 || iStartIndex >= iCount )
		return pChatRoomInfo;

	STD_MAP_ROOM::iterator	mIter , mIterEnd;

	if( GetRoomData( mIter , mIterEnd , iStartIndex , iCategory ) == false )
		return pChatRoomInfo;

	if( mIter != mIterEnd )
		pChatRoomInfo	=	&( (*mIter).second );

	return pChatRoomInfo;
}

void SPWindowChattingRoom::ClearRoomData( int iCategory )
{
	int	iStartIndex	=	(int)ROOM_TYPE_NULL	+ 1;
	int	iEndIndex	=	(int)ROOM_TYPE_MAX	- 1;

	if( iCategory > ROOM_TYPE_NULL && iCategory < ROOM_TYPE_MAX )
	{
		iStartIndex	=	iCategory;
		iEndIndex	=	iCategory;
	}

	for( int i = iStartIndex ; i <= iEndIndex ; ++i )
	{
		m_vRoomData[ i ].clear();
		m_iCurRoomFindPage[ i ]	=	0;
	}

	ClearRoomFindRoomNo();
}

int SPWindowChattingRoom::GetRoomDataCount( int iCategory )
{
	int	iStartIndex	=	(int)ROOM_TYPE_NULL	+ 1;
	int	iEndIndex	=	(int)ROOM_TYPE_MAX	- 1;

	if( iCategory > ROOM_TYPE_NULL && iCategory < ROOM_TYPE_MAX )
	{
		iStartIndex	=	iCategory;
		iEndIndex	=	iCategory;
	}

	int	iCount	=	0;

	for( int i = iStartIndex ; i <= iEndIndex ; ++i )
	{
		iCount	+=	(int)m_vRoomData[ i ].size();
	}

	return iCount;
}

void SPWindowChattingRoom::UpdateWheel()
{
	++m_iFrameIndex;

	if( m_iFrameIndex >= m_iFrameMax )
		m_iFrameIndex	=	0;

	SPWindow*	pWindowWheel	=	Find( WIID_CHATTING_ROOM_MODAL_WAITTING , true );

	if( pWindowWheel == NULL )
		return;

	SPWindow*	pWindow	=	NULL;

	pWindow	=	pWindowWheel->Find( WIID_CHATTING_ROOM_MODAL_WAITTING_WHEEL_1 , true );
	pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , m_iFrameIndex );

	pWindow	=	pWindowWheel->Find( WIID_CHATTING_ROOM_MODAL_WAITTING_WHEEL_2 , true );
	pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , m_iFrameIndex );
}

bool SPWindowChattingRoom::IsPlayerAction()
{
	if( g_pGOBManager == NULL )
		return false;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return false;

	UINT64 uiCurAction	=	pLocalPlayer->GetCurAction();

	if( uiCurAction	==	ACTION_MOVE_LEFT	||
		uiCurAction	==	ACTION_MOVE_RIGHT	||
		uiCurAction	==	ACTION_RUN_LEFT		||
		uiCurAction	==	ACTION_RUN_RIGHT	||
		uiCurAction	==	ACTION_RUN_STOP		||
		uiCurAction	==	ACTION_SIT_DOWN		||
		uiCurAction	==	ACTION_SIT_WAIT		||
		uiCurAction	==	ACTION_SIT_STAND	||
		uiCurAction	==	ACTION_STOP	)
		return true;

	return false;
}
