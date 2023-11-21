// ***************************************************************
//  SPWindowTalkingRoom   version:  1.0   ·  date: 03/06/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
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
#include "SPWindowEdit.h"
#include "SPWindowEditMultiLine.h"
#include "SPWindowSlider.h"

#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"
#include "SPTreeBoxItemQuest.h"

#include "SPWindowTreeBox.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include "SPNoticeBox.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPInterfaceManager.h"

#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"

#include "SPItemAttr.h"
#include "SPManager.h"
#include "SPItem.h"
#include "SPItemCluster.h"		//[2005/6/16]

#include "SPSubGameManager.h"
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
#include "SPGuildArchive.h"
#include "SPPlayer.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPLocalizeManager.h"

#include "SPCheckManager.h"
#include "GlobalDefines_Share.h"

#include "SPAbuseFilter.h"

#include "SPWindowChat.h"			// GetRandom
#include "SPWindowFriend.h"
#include "SPWindowFriendFind.h"
#include "SPWindowFriendFollow.h"//[liuyang,2008.10.10]
#include "SPWindowFriendSnatch.h"//[liuyang,2008.10.10]

#include "SPIndun.h"
#include "SPIndunManager.h"

#include "SPChatManager.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPWindowTalkingRoom.h"

SPWindowTalkingRoom::SPWindowTalkingRoom( WND_ID WndClassID , INSTANCE_ID InstanceID )
: SPWindow( WndClassID , InstanceID )
{	
	Init();	
}

SPWindowTalkingRoom::SPWindowTalkingRoom( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent /* = NULL  */ )
: SPWindow( WNDID_TALKING_ROOM , InstanceID , iX , iY , iCX , iCY , pParent )
{	
	Init();	
}

SPWindowTalkingRoom::~SPWindowTalkingRoom()
{
	Clean();
}

void SPWindowTalkingRoom::Init()
{
	m_strToolTip	= "[SPWindowTalkingRoom]";

	//////////////////////////////////////////////////////////////////////////

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , &m_pBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_LEFT_TOP ]			, 482 , 467 , 486 , 471 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_CENTER_TOP ]		, 487 , 467 , 491 , 471 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_RIGHT_TOP ]		, 492 , 467 , 496 , 471 );

	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_LEFT_MIDDLE ]		, 482 , 472 , 486 , 476 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_CENTER_MIDDLE ]	, 487 , 472 , 491 , 476 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_RIGHT_MIDDLE ]		, 492 , 472 , 496 , 476 );

	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_LEFT_BOTTOM ]		, 482 , 477 , 486 , 481 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_CENTER_BOTTOM ]	, 487 , 477 , 491 , 481 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_RIGHT_BOTTOM ]		, 492 , 477 , 496 , 481 );

	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_LINE_1 ]			, 508 , 450 , 510 , 451 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_SUB ]				, 502 , 472 , 506 , 476 );

	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_ADVERTISING_LEFT_TOP ]			, 452 , 482 , 456 , 486 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_ADVERTISING_CENTER_TOP ]		, 457 , 482 , 461 , 486 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_ADVERTISING_RIGHT_TOP ]		, 462 , 482 , 466 , 486 );

	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_ADVERTISING_LEFT_MIDDLE ]		, 452 , 487 , 456 , 491 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_ADVERTISING_CENTER_MIDDLE ]	, 457 , 487 , 461 , 491 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_ADVERTISING_RIGHT_MIDDLE ]		, 462 , 487 , 466 , 491 );

	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_ADVERTISING_LEFT_BOTTOM ]		, 452 , 492 , 456 , 496 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_ADVERTISING_CENTER_BOTTOM ]	, 457 , 492 , 461 , 496 );
	SetRect( &m_rcBGSrc[ TALKING_ROOM_BG_ADVERTISING_RIGHT_BOTTOM ]		, 462 , 492 , 466 , 496 );

	//////////////////////////////////////////////////////////////////////////

	SPWindowButton*			pWindowButton;
	SPWindowStatic*			pWindowStatic;
	SPWindowEdit*			pWindowEdit;
	SPWindowSlider*			pWindowSlider;
	SPWindowList*			pWindowList;
	RECT					rtRect;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_ADVERTISING_PANEL , 0 , 30 , 524 , m_iSY , this );
	pWindowStatic->SetShowEnable( false );
	InitWindowAdvertising( pWindowStatic );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MY_INFO_SETUP_PANEL , 0 , 30 , 524 , m_iSY , this );
	pWindowStatic->SetShowEnable( false );
	InitWindowMyInfoSetup( pWindowStatic );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_PANEL , 0 , 30 , 524 , m_iSY , this );
	pWindowStatic->SetShowEnable( false );
	InitWindowRoomSetup( pWindowStatic );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_SUBJECT_TEXT , 100 , 36 , 414 , 12 , this );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_NORMAL );
	pWindowStatic->SetWindowText( "일이삼사오육칠팔구십일이삼사오육칠팔구십일이삼사오육칠팔구십일이삼사오육칠팔구십" );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_PANEL , 373 , 0 , 151 , 377 , this );
	pWindowStatic->SetWindowType( WND_TYPE_NINEBASE );
	pWindowStatic->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 452 , 467 , 4 , 1 );
	InitWindowMax( pWindowStatic );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_TITLE_IMG , 2 , 2 , 187 , 24 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 1 , 313 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_CLOSE , 354 , 9 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 27 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 27 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 40 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 40 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_TYPE_ICON , 10 , 33 , 16 , 16 , this );
	pWindowStatic->SetImage( (SPTexture*)NULL );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 346 , 252 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 363 , 252 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 346 , 269 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 363 , 269 );
	pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_NUMBER_100 , 27 , 41 , 6 , 7 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 363 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 370 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 377 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 391 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 398 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 405 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 412 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 419 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 426 , 93 );
	pWindowStatic->SetImage( (SPTexture*)NULL );
	pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , 10 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_NUMBER_010 , 33 , 41 , 6 , 7 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 363 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 370 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 377 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 391 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 398 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 405 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 412 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 419 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 426 , 93 );
	pWindowStatic->SetImage( (SPTexture*)NULL );
	pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , 10 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_NUMBER_001 , 39 , 41 , 6 , 7 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 363 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 370 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 377 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 391 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 398 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 405 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 412 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 419 , 93 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 426 , 93 );
	pWindowStatic->SetImage( (SPTexture*)NULL );
	pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , 10 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_TYPE_TEXT , 48 , 36 , 52 , 12 , this );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 70 , 140 , 124 ) );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_BOLD );
	pWindowStatic->SetWindowText("[일반]");
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_SEND_ALL , 6 , 341 , 18 , 14 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 309 , 419 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 309 , 434 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 309 , 449 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 309 , 464 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 309 , 449 );
	pWindowButton->SPSendMessage( SPIM_BTN_SELECT , 0 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_SEND_WHISPER , 23 , 341 , 18 , 14 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 328 , 419 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 328 , 434 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 328 , 449 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 328 , 464 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 328 , 449 );
	pWindowButton->SPSendMessage( SPIM_BTN_SELECT , 1 );
	pWindowButton	=	NULL;

	pWindowEdit	=	new SPWindowEdit( WIID_TALKING_ROOM_EDIT_ALL , 6 , 354 , 357 , 17 , this );
	pWindowEdit->SetWindowType( WND_TYPE_LEFTBASE );
	pWindowEdit->SetMargin( 6 , 3 );
	pWindowEdit->SetImageNormalHead	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 349 , 384 , 366 );
	pWindowEdit->SetImageNormalBody	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 349 , 388 , 366 );
	pWindowEdit->SetImageNormalTail	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 349 , 392 , 366 );
	pWindowEdit->SetImageFocusHead	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 349 , 384 , 366 );
	pWindowEdit->SetImageFocusBody	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 349 , 388 , 366 );
	pWindowEdit->SetImageFocusTail	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 349 , 392 , 366 );
//	pWindowEdit->SetImageDeco		( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 501 , 501 , 511 , 511 );
	pWindowEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowEdit->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowEdit->SetLimitText( 54 );
//	pWindowEdit->SetEnableTabStop( true );
//	pWindowEdit->SetWindowText( "일반대화" );

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_WHISPER_IMG , 11 , 358 , 17 , 10 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 277 , 115 );
	pWindowStatic	=	NULL;

	pWindowEdit	=	new SPWindowEdit( WIID_TALKING_ROOM_EDIT_WHISPER , 6 , 354 , 129 , 17 , this );
	pWindowEdit->SetWindowType( WND_TYPE_LEFTBASE );
	pWindowEdit->SetMargin( 28 , 3 );
	pWindowEdit->SetImageNormalHead	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 349 , 384 , 366 );
	pWindowEdit->SetImageNormalBody	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 349 , 388 , 366 );
	pWindowEdit->SetImageNormalTail	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 349 , 392 , 366 );
	pWindowEdit->SetImageFocusHead	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 349 , 384 , 366 );
	pWindowEdit->SetImageFocusBody	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 349 , 388 , 366 );
	pWindowEdit->SetImageFocusTail	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 349 , 392 , 366 );
//	pWindowEdit->SetImageDeco		( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 501 , 501 , 511 , 511 );
	pWindowEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowEdit->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowEdit->SetLimitText( LEN_NAME + 1 );
//	pWindowEdit->SetEnableTabStop( true );
//	pWindowEdit->SetWindowText( "귓속말" );

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_ADVERTISING_TEXT_IMG , 10 , 53 , 88 , 16 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 221 , 416 );
	pWindowStatic	=	NULL;

	pWindowList = new SPWindowList( WIID_TALKING_ROOM_ADVERTISING_LIST , 12 , 71 , 339, 99 , this );
	pWindowList->SetFontShadow( false );
	pWindowList->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowList->SetMargin( 2 , 4 , 0 , 4 );
	pWindowList->SetMultiLine( TRUE , 3 );
	pWindowList->SetWheelUse( false );

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_ADVERTISING_UP , 351 , 67 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 229 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 229 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 241 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 241 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 241 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_ADVERTISING_DOWN , 351 , 67 + 96 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 303 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 303 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 315 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 315 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 315 );
	pWindowButton	=	NULL;

	pWindowSlider	=	new	SPWindowSlider( WIID_TALKING_ROOM_ADVERTISING_SLIDER , 351 , 67 + 11 , 12 , 85 , this );

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
	pWindowSlider->SPSendMessage( SPIM_SET_MAXVALUE , 1 );
	pWindowSlider->SPSendMessage( SPIM_CURSOR_MOVE );
	pWindowSlider->Refresh();

	//////////////////////////////////////////////////////////////////////////

	pWindowList = new SPWindowList( WIID_TALKING_ROOM_CHAT_LIST , 10 + 7 , 179 + 2 , 332, 154 , this );
	pWindowList->SetFontShadow( false );
	pWindowList->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowList->SetMargin( 0 , 0 , 0 , 0 );
	pWindowList->SetMultiLine( TRUE , 2 );
	pWindowList->SetWheelUse( false );

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_CHAT_UP , 351 , 179 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 229 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 229 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 241 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 241 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 241 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_CHAT_DOWN , 351 , 337 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 303 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 303 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 315 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 315 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 315 );
	pWindowButton	=	NULL;

	pWindowSlider	=	new	SPWindowSlider( WIID_TALKING_ROOM_CHAT_SLIDER , 351 , 190 , 12 , 147 , this );

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
	pWindowSlider->SPSendMessage( SPIM_SET_MAXVALUE , 1 );
	pWindowSlider->SPSendMessage( SPIM_CURSOR_MOVE );
	pWindowSlider->Refresh();

	//////////////////////////////////////////////////////////////////////////

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX , 365 , 181 , 6 , 41 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 181 , 241 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 188 , 241 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 195 , 241 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 202 , 241 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 195 , 241 );
	pWindowButton	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	m_bNextLine		=	false;
	m_bUse			=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_CHATTINGROOM );
	m_cInxHistory	=	0;

	OnNoticeNo( NULL , NULL );

	//////////////////////////////////////////////////////////////////////////

	m_cUserListTextColor[ USER_LIST_COLOR_DEFAULT ]	=	D3DCOLOR_ARGB( 255 , 49 , 76 , 97 );
	m_cUserListTextColor[ USER_LIST_COLOR_ME ]		=	D3DCOLOR_ARGB( 255 , 49 , 76 , 97 );

	m_fBanLimitTime		=	3.0f;

	ResetBan();
}

void SPWindowTalkingRoom::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
}

void SPWindowTalkingRoom::Process(float fTime)
{
	if( !m_bShow )	return;

	SPWindow::Process( fTime );

	if( m_bRoomOutBan == true )
		ProcessBan( fTime );
}

void SPWindowTalkingRoom::Render(float fTime)
{
	if( !m_bShow )	return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < TALKING_ROOM_BG_COUNT ; ++i )
		{
			m_pBGTexture->SetColor( m_RGBA );
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	SPWindow::RenderReverse( fTime );

	g_pVideo->Flush();
}

void SPWindowTalkingRoom::Show(bool bWithChild)
{
	if( m_bUse == false )
		return;

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		if( pChatManager->IsChatRoom() == false )
			return;
	}

	ForceShow( true );
}

void SPWindowTalkingRoom::Hide(bool bSendServer /*= true*/)
{
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		if( pChatManager->IsChatRoom() == true )
			return;
	}

	ForceShow( false );
}

void SPWindowTalkingRoom::Close()
{
	Hide();
}

void SPWindowTalkingRoom::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowTalkingRoom::ForceShow( bool bShow )
{
	if( bShow == true )
	{
		SPWindow::Show();
//		OnPurge( NULL , NULL );
	}
	else
	{
		SPWindow::Hide();
	}
}

void SPWindowTalkingRoom::ReposOwnImage()
{
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_LEFT_TOP ]		, m_iAX					, m_iAY					, m_iAX + 4			, m_iAY + 4 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_CENTER_TOP ]		, m_iAX + 4				, m_iAY					, m_iAX + 4 + 367	, m_iAY + 4 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_RIGHT_TOP ]		, m_iAX + 371			, m_iAY					, m_iAX + 371 + 4	, m_iAY + 4 );

	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_LEFT_MIDDLE ]		, m_iAX					, m_iAY + 4				, m_iAX + 4			, m_iAY + 4 + 369 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_CENTER_MIDDLE ]	, m_iAX + 4				, m_iAY	+ 4				, m_iAX + 4 + 367	, m_iAY + 4 + 369 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_RIGHT_MIDDLE ]	, m_iAX + 371			, m_iAY + 4				, m_iAX + 371 + 4	, m_iAY + 4 + 369 );

	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_LEFT_BOTTOM ]		, m_iAX					, m_iAY + 373			, m_iAX + 4			, m_iAY + 373 + 4 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_CENTER_BOTTOM ]	, m_iAX + 4				, m_iAY	+ 373			, m_iAX + 4 + 367	, m_iAY + 373 + 4 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_RIGHT_BOTTOM ]	, m_iAX + 371			, m_iAY + 373			, m_iAX + 371 + 4	, m_iAY + 373 + 4 );

	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_LINE_1 ]			, m_iAX + 2				, m_iAY	+ 26			, m_iAX + 2 + 371	, m_iAY + 26 + 1 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_SUB ]				, m_iAX + 2				, m_iAY + 27			, m_iAX + 2 + 371	, m_iAY + 27 + 346 );

	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_ADVERTISING_LEFT_TOP ]		, m_iAX	+ 10			, m_iAY + 67			, m_iAX + 10 + 4	, m_iAY + 67 + 4 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_ADVERTISING_CENTER_TOP ]		, m_iAX + 14			, m_iAY + 67			, m_iAX + 14 + 346	, m_iAY + 67 + 4 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_ADVERTISING_RIGHT_TOP ]		, m_iAX + 359			, m_iAY	+ 67			, m_iAX + 359 + 4	, m_iAY + 67 + 4 );

	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_ADVERTISING_LEFT_MIDDLE ]		, m_iAX	+ 10			, m_iAY + 71			, m_iAX + 10 + 4	, m_iAY + 71 + 99 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_ADVERTISING_CENTER_MIDDLE ]	, m_iAX	+ 14			, m_iAY + 71			, m_iAX + 14 + 346	, m_iAY + 71 + 99 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_ADVERTISING_RIGHT_MIDDLE ]	, m_iAX	+ 359			, m_iAY + 71			, m_iAX + 359 + 4	, m_iAY + 71 + 99 );

	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_ADVERTISING_LEFT_BOTTOM ]		, m_iAX	+ 10			, m_iAY + 170			, m_iAX + 10 + 4	, m_iAY + 170 + 4 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_ADVERTISING_CENTER_BOTTOM ]	, m_iAX	+ 14			, m_iAY + 170			, m_iAX + 14 + 346	, m_iAY + 170 + 4 );
	SetRect( &m_rcBGDest[ TALKING_ROOM_BG_ADVERTISING_RIGHT_BOTTOM ]	, m_iAX	+ 359			, m_iAY + 170			, m_iAX + 359 + 4	, m_iAY + 170 + 4 );
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowTalkingRoom )

	//	Common
	SPIMESSAGE_COMMAND(	SPIM_PURGE					,																	OnPurge							)
	SPIMESSAGE_COMMAND(	SPIM_CLOSE					,																	OnForceClose					)
	SPIMESSAGE_COMMAND( SPIM_KEY_TAB				,																	OnTab							)

	SPIMESSAGE_COMMAND(	SPIM_KEY_RETURN				,																	OnKeyReturn						)
	SPIMESSAGE_COMMAND(	SPIM_KEY_ESCAPE				,																	OnKeyEscape						)

	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_MODIFY	,																	OnPacketModify					)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_OUT		,																	OnPacketOut						)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_FORCEOUT	,																	OnPacketForceOut				)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_CHANGEMASTER	,																OnPacketChangeMaster			)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_MESSAGE	,																	OnPacketMessage					)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_MEMBERLIST,																	OnPacketMemberList				)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_ADVERT	,																	OnPacketAdvert					)
	SPIMESSAGE_COMMAND(	SPIM_CHATTINGROOM_OPTION	,																	OnRefreshOption					)

	SPIMESSAGE_COMMAND(	SPIM_TALKING_ROOM_NEXTLINE	,																	OnListNextLine					)
	SPIMESSAGE_COMMAND(	SPIM_TALKING_ROOM_MESSAGE	,																	OnGetMessage					)

	SPIMESSAGE_COMMAND( SPIM_NOTICE_YES				,																	OnNoticeYes						)
	SPIMESSAGE_COMMAND( SPIM_NOTICE_NO				,																	OnNoticeNo						)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_CLOSE										,	OnClose							)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MAX_CLOSE									,	OnClose							)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MIN										,	OnMin							)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MAX										,	OnMax							)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_ADVERTISING_UP							,	OnAdvertisingListUp				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_ADVERTISING_DOWN							,	OnAdvertisingListDown			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_CHAT_UP									,	OnChatListUp					)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_CHAT_DOWN									,	OnChatListDown					)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MAX_PLAYER_PAGE_LEFT						,	OnUserListLeft					)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MAX_PLAYER_PAGE_RIGHT						,	OnUserListRight					)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MAX_ADVERTISING							,	OnBtnAdvertising				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MAX_SEARCH								,	OnBtnSearch						)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MAX_MY_INFO_SETUP							,	OnBtnMyInfoSetup				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MAX_ROOM_INFO_SETUP						,	OnBtnRoomInfoSetup				)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_ADVERTISING_OK							,	OnBtnAdvertisingOK				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_ADVERTISING_CANCEL						,	OnBtnAdvertisingCancel			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_ADVERTISING_CLOSE							,	OnBtnAdvertisingCancel			)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MY_INFO_SETUP_MY_INFO_SEARCH				,	OnBtnMyInfoSearch				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MY_INFO_SETUP_CHAT_REQUEST				,	OnBtnChatRequest				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MY_INFO_SETUP_MY_STYLE_OPEN				,	OnBtnMyStyleOpen				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MY_INFO_SETUP_OK							,	OnBtnMyInfoSetupOK				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MY_INFO_SETUP_CANCEL						,	OnBtnMyInfoSetupCancel			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_MY_INFO_SETUP_CLOSE						,	OnBtnMyInfoSetupCancel			)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_ARROW_LEFT		,	OnBtnInfoSetupGenderLeft		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_ARROW_RIGHT		,	OnBtnInfoSetupGenderRight		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_ARROW_LEFT			,	OnBtnInfoSetupAgeLeft			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_ARROW_RIGHT			,	OnBtnInfoSetupAgeRight			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_CHANGE			,	OnBtnInfoSetupPasswordChange	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_INFO_SETUP_OK								,	OnBtnInfoSetupOK				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_INFO_SETUP_CANCEL							,	OnBtnInfoSetupCancel			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_INFO_SETUP_CLOSE							,	OnBtnInfoSetupCancel			)

	SPIMESSAGE_CONTROL( SPIM_SLIDER_UP				,	WIID_TALKING_ROOM_ADVERTISING_SLIDER						,	OnAdvertisingSlider				)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN			,	WIID_TALKING_ROOM_ADVERTISING_SLIDER						,	OnAdvertisingSlider				)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP			,	WIID_TALKING_ROOM_ADVERTISING_SLIDER						,	OnAdvertisingSlider				)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN		,	WIID_TALKING_ROOM_ADVERTISING_SLIDER						,	OnAdvertisingSlider				)
	SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE			,	WIID_TALKING_ROOM_ADVERTISING_SLIDER						,	OnAdvertisingSlider				)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DBLCLICK		,	WIID_TALKING_ROOM_ADVERTISING_LIST							,	OnAdvertListLButtonDblUp		)
	SPIMESSAGE_CONTROL( SPIM_WHEEL_UP				,	WIID_TALKING_ROOM_ADVERTISING_LIST							,	OnAdvertisingListUp				)
	SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN				,	WIID_TALKING_ROOM_ADVERTISING_LIST							,	OnAdvertisingListDown			)

	SPIMESSAGE_CONTROL( SPIM_SLIDER_UP				,	WIID_TALKING_ROOM_CHAT_SLIDER								,	OnChatSlider					)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN			,	WIID_TALKING_ROOM_CHAT_SLIDER								,	OnChatSlider					)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP			,	WIID_TALKING_ROOM_CHAT_SLIDER								,	OnChatSlider					)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN		,	WIID_TALKING_ROOM_CHAT_SLIDER								,	OnChatSlider					)
	SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE			,	WIID_TALKING_ROOM_CHAT_SLIDER								,	OnChatSlider					)

	SPIMESSAGE_CONTROL( SPIM_WHEEL_UP				,	WIID_TALKING_ROOM_CHAT_LIST									,	OnChatListUp					)
	SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN				,	WIID_TALKING_ROOM_CHAT_LIST									,	OnChatListDown					)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_TALKING_ROOM_CHAT_LIST									,	OnChatListLButtonUp				)

	SPIMESSAGE_CONTROL(	SPIM_KEY_DOWN				,	WIID_TALKING_ROOM_EDIT_ALL									,	OnKeyDown						)

	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_TALKING_ROOM_EDIT_ALL									,	OnEditSetFocus					)
	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_TALKING_ROOM_EDIT_WHISPER								,	OnEditSetFocus					)
	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_TALKING_ROOM_ADVERTISING_EDIT							,	OnEditSetFocus					)
	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_EDIT					,	OnEditSetFocus					)
	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT				,	OnEditSetFocus					)
	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TEXT			,	OnRoomSetupPWEditSetFocus		)

	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_TALKING_ROOM_EDIT_ALL									,	OnEditKillFocus					)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_TALKING_ROOM_EDIT_WHISPER								,	OnEditKillFocus					)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_TALKING_ROOM_ADVERTISING_EDIT							,	OnEditKillFocus					)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_EDIT					,	OnEditKillFocus					)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT				,	OnRoomSetupNumberEditKillFocus	)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TEXT			,	OnEditKillFocus					)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_TALKING_ROOM_SEND_ALL					,	WIID_TALKING_ROOM_SEND_WHISPER										,	OnSend				)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_TALKING_ROOM_PLAYER_SELECT_ICON		,	WIID_TALKING_ROOM_PLAYER_SELECT_ICON + TALKING_ROOM_PLAYER_COUNT	,	OnPlayer			)
	SPIMESSAGE_CONTROL_RANGE( SPIM_RBUTTON_UP		,	WIID_TALKING_ROOM_PLAYER_SELECT_ICON		,	WIID_TALKING_ROOM_PLAYER_SELECT_ICON + TALKING_ROOM_PLAYER_COUNT	,	OnPlayerClear		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_TALKING_ROOM_MAX_PLAYER_ACTION_WHISPER	,	WIID_TALKING_ROOM_MAX_PLAYER_ACTION_FORCE_LEAVE						,	OnPlayerAction		)

SPIMESSAGE_OBJ_MAP_END(	SPWindow	)

int SPWindowTalkingRoom::OnPurge( WPARAM wParam, LPARAM lParam )
{
	OnNoticeNo( NULL , NULL );
	OnMax( NULL , NULL );
	OnSend( WIID_TALKING_ROOM_SEND_ALL , 1 , NULL );

	UpdateUserListPage( 0 );
	UpdateUserList();
	UpdateUserCount();
	UpdateSubject();

	ClearPlayerAction();

	SetModalWindowShow( Find( WIID_TALKING_ROOM_ADVERTISING_PANEL ) , false );
	SetModalWindowShow( Find( WIID_TALKING_ROOM_MY_INFO_SETUP_PANEL ) , false );
	SetModalWindowShow( Find( WIID_TALKING_ROOM_INFO_SETUP_PANEL ) , false );

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_TALKING_ROOM_CHAT_LIST , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_PURGE );

	pWindow	=	Find( WIID_TALKING_ROOM_CLOSE , true );

	if( pWindow != NULL )
		pWindow->SetEnable( true );

	pWindow	=	Find( WIID_TALKING_ROOM_MAX_CLOSE , true );

	if( pWindow != NULL )
		pWindow->SetEnable( true );

	pWindow	=	Find( WIID_TALKING_ROOM_ADVERTISING_OK , true );

	if( pWindow != NULL )
		pWindow->SetEnable( true );

	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_OK , true );

	if( pWindow != NULL )
		pWindow->SetEnable( true );

	bool	bSliderEnable;

	SPWindowSlider*	pWindowSlider;

	pWindowSlider	=	(SPWindowSlider*)Find( WIID_TALKING_ROOM_ADVERTISING_SLIDER , true );

	if( pWindowSlider != NULL )
	{
		bSliderEnable	=	true;

		SPWindowList*	pWindowList	=	(SPWindowList*)Find( WIID_TALKING_ROOM_ADVERTISING_LIST , true );

		if( pWindowList != NULL )
		{
			int	iEndLine	=	pWindowList->GetEndLine() + 1;

			if( iEndLine < 0 )
				iEndLine = 0;

			int	iCurLine	=	iEndLine - 1;

			if( iCurLine < 0 )
				iCurLine = 0;

			pWindowSlider->SPSendMessage( SPIM_SET_MAXVALUE , iEndLine );
			pWindowSlider->SPSendMessage( SPIM_SET_CURVALUE , iCurLine );
			pWindowList->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_SET_LINE , iCurLine );

			if( pWindowList->GetEndLine() <= 0 )
				bSliderEnable	=	false;
		}

		pWindowSlider->SetEnable( bSliderEnable );
	}

	pWindow	=	Find( WIID_TALKING_ROOM_ADVERTISING_UP , true );

	if( pWindow != NULL )
		pWindow->SetEnable( bSliderEnable );

	pWindow	=	Find( WIID_TALKING_ROOM_ADVERTISING_DOWN , true );

	if( pWindow != NULL )
		pWindow->SetEnable( bSliderEnable );

	pWindowSlider	=	(SPWindowSlider*)Find( WIID_TALKING_ROOM_CHAT_SLIDER , true );

	if( pWindowSlider != NULL )
	{
		pWindowSlider->SetEnable( false );
		pWindowSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
		pWindowSlider->SetCurpage( 0 );
		pWindowSlider->SPSendMessage( SPIM_SET_MAXVALUE , 1 );
		pWindowSlider->SPSendMessage( SPIM_CURSOR_MOVE );
		pWindowSlider->Refresh();
	}

	pWindow	=	Find( WIID_TALKING_ROOM_CHAT_UP , true );

	if( pWindow != NULL )
		pWindow->SetEnable( false );

	pWindow	=	Find( WIID_TALKING_ROOM_CHAT_DOWN , true );

	if( pWindow != NULL )
		pWindow->SetEnable( false );

	m_bMasterChange	=	false;
	m_bForceOut		=	false;

	m_strLastMsg.clear();
	m_iEqualMsgCount	=	0;

	return 1;
}

int SPWindowTalkingRoom::OnForceClose( WPARAM wParam, LPARAM lParam )
{
	ForceShow( false );
	return 1;
}

int SPWindowTalkingRoom::OnTab( WPARAM wParam, LPARAM lParam )
{
	if( g_pInterfaceManager == NULL )
		return 1;

	SPWindow*	pFocusWindow		=	g_pInterfaceManager->GetFocusWindow();
	SPWindow*	pWindowEditAll		=	Find( WIID_TALKING_ROOM_EDIT_ALL , true );
	SPWindow*	pWindowEditWhisper	=	Find( WIID_TALKING_ROOM_EDIT_WHISPER , true );

	if( pFocusWindow == NULL || pWindowEditAll == NULL || pWindowEditWhisper == NULL )
		return 1;

	if( pFocusWindow->GetInstanceID() == pWindowEditAll->GetInstanceID() )
	{
		if( pWindowEditAll->IsFocus() == true )
		{
			pWindowEditWhisper->SetFocus();
			return 1;
		}
	}

	if( pFocusWindow->GetInstanceID() == pWindowEditWhisper->GetInstanceID() )
	{
		if( pWindowEditWhisper->IsFocus() == true )
		{
			pWindowEditAll->SetFocus();
			return 1;
		}
	}

	return 1;
}

int SPWindowTalkingRoom::OnKeyReturn( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return -1;

	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_TALKING_ROOM_ADVERTISING_PANEL );

	if( pWindow != NULL )
	{
		if( pWindow->IsShow() == true )
		{
			OnBtnAdvertisingOK( wParam , lParam );
			return 1;
		}
	}

	SPWindow*	pWindowEditAll	=	Find( WIID_TALKING_ROOM_EDIT_ALL , true );

	if( pWindowEditAll == NULL )
		return -1;

	if( pWindowEditAll->IsFocus() == false )
	{
		pWindowEditAll->SetFocus();
		return 1;
	}

	std::string		strWhisper;
	std::string		strMsg;
	std::string		strSendMsg;
	const char*		pszBuf;

	pszBuf	=	pWindowEditAll->GetWindowText();

	if( pszBuf == NULL )
		return 0;

	strMsg	=	pszBuf;

	if( strMsg.empty() == true )
	{
		bool		bKillFocus		=	true;
		SPWindow*	pWindowFocus	=	g_pInterfaceManager->GetFocusWindow();

		if( pWindowFocus != NULL )
		{
			if( pWindowFocus->GetInstanceID() == pWindowEditAll->GetInstanceID() )
			{
				g_pInterfaceManager->SetFocusWindow( NULL );
				bKillFocus	=	false;
			}
		}

		if( bKillFocus == true )
			pWindowEditAll->SPSendMessage( SPIM_KILLFOCUS );

		return 0;
	}

	SPWindow*		pWindowEditWhisper	=	Find( WIID_TALKING_ROOM_EDIT_WHISPER , true );

	if( pWindowEditWhisper == NULL )
		return -1;

	CHAT_SEND_APPEND_DATA	stMsgData;

	stMsgData.m_eScope		=	SCOPE_AROUNDREGION;
	stMsgData.m_strWhisper.clear();

	if( pWindowEditWhisper->IsShow() == true )
	{
		//if( m_strCommand.empty() == true )
		//{
		//	char			szCommand[ _MAX_PATH ];

		//	g_pInterfaceManager->SPChildWndSendMessage( WIID_CHAT , SPIM_WNDCHAT_GET_COMMAND , (WPARAM)CHAT_STRING_WHISPER , (LPARAM)szCommand );

		//	m_strCommand	=	szCommand;
		//}

		//if( m_strCommand.empty() == true )
		//	return 1;

		pszBuf	=	pWindowEditWhisper->GetWindowText();

		if( pszBuf == NULL )
			return 1;

		//strWhisper	=	m_strCommand;
		//strWhisper	+=	" ";
		//strWhisper	+=	pszBuf;
		//strWhisper	+=	" ";
		strWhisper	=	pszBuf;

		stMsgData.m_eScope		=	SCOPE_WHISPER;
		stMsgData.m_strWhisper	=	pszBuf;
	}

	if( m_strLastMsg.compare( strMsg ) == 0 )
		++m_iEqualMsgCount;
	else
		m_iEqualMsgCount	=	0;

	if( m_iEqualMsgCount > 2 )
	{
		PrintErrorMsg( 5008134 );
		return 1;
	}

	m_strLastMsg			=	strMsg;
	strSendMsg				=	strMsg;

	//////////////////////////////////////////////////////////////////////////
	// Enter가 들엉輩을때 히스토리에 넣는다. [9/21/2006 AJJIYA]

	if( g_pInterfaceManager != NULL )
	{
		SPIMEPool*	pIMEPool	=	g_pInterfaceManager->GetIMEPool();

		if( pIMEPool != NULL )
		{
			string strMessage = pIMEPool->GetString();

			if(strMessage.length() > 0)
			{
				m_vstrHistoryBuffer.push_back( strMessage );

				if( m_vstrHistoryBuffer.size() > 10 )
					m_vstrHistoryBuffer.erase( m_vstrHistoryBuffer.begin() );

				m_cInxHistory = 0;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	//if( strWhisper.empty() == false )
	//{
	//	stMsgData.m_eScope		=	SCOPE_WHISPER;
	//	stMsgData.m_strWhisper	=	strWhisper;
	//	strSendMsg				=	strMsg;
	//	//stMsgData.m_strWhisper	=	strWhisper;
	//	//strSendMsg	=	strWhisper	+	strMsg;
	//}
	//else
	//{
	//	stMsgData.m_eScope	=	SCOPE_AROUNDREGION;
	//	strSendMsg			=	strMsg;
	//}

	SPWindow*	pWindowFocus	=	g_pInterfaceManager->GetFocusWindow();

	g_pInterfaceManager->SPChildWndSendMessage( WIID_CHAT , SPIM_WNDCHAT_SEND_MESSAGE , (WPARAM)strSendMsg.c_str() , (LPARAM)&stMsgData );

	g_pInterfaceManager->SetFocusWindow( pWindowFocus );

	if( pWindowEditAll != NULL )
		pWindowEditAll->SetWindowText( NULL );

	return 1;
}

int SPWindowTalkingRoom::OnKeyEscape( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 0;

	bool bResult	=	(bool)wParam;

	if( bResult == true )
		return 0;

	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_TALKING_ROOM_ADVERTISING_PANEL );

	if( pWindow != NULL )
	{
		if( pWindow->IsShow() == true )
		{
			SetModalWindowShow( pWindow , false );
			return 1;
		}
	}

	pWindow	=	Find( WIID_TALKING_ROOM_MY_INFO_SETUP_PANEL );

	if( pWindow != NULL )
	{
		if( pWindow->IsShow() == true )
		{
			SetModalWindowShow( pWindow , false );
			return 1;
		}
	}

	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_PANEL );

	if( pWindow != NULL )
	{
		if( pWindow->IsShow() == true )
		{
			SetModalWindowShow( pWindow , false );
			return 1;
		}
	}

	if( m_bRoomOut == true )
	{
		if( g_pInterfaceManager != NULL )
			g_pInterfaceManager->ForceReply();
	}
	else
	{
		OnClose( wParam , lParam );
	}

	return 1;
}

int SPWindowTalkingRoom::OnPacketModify( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_OK , true );

	if( pWindow != NULL )
		pWindow->SetEnable( true );

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

	CHATROOM_INFO		stCRInfo;						// 수정하려고하는 대화방의 정보

	pPacket->ExtractStruct( &stCRInfo , sizeof( CHATROOM_INFO ) );

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		pChatManager->SetChatRoomInfo( stCRInfo );

	UpdateSubject();
	UpdateUserCount();

	return 1;
}

int SPWindowTalkingRoom::OnPacketOut( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_TALKING_ROOM_CLOSE , true );

	if( pWindow != NULL )
		pWindow->SetEnable( true );

	pWindow	=	Find( WIID_TALKING_ROOM_MAX_CLOSE , true );

	if( pWindow != NULL )
		pWindow->SetEnable( true );

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

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		pChatManager->ClearChatRoomInfo();

	Hide();

	return 1;
}

int SPWindowTalkingRoom::OnPacketForceOut( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	m_bForceOut	=	false;

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

	UINT8	iFlag;						// 0: 방장의 강퇴 요청의 응답. 1:강제 퇴장 대상자.

	pPacket->ExtractUINT8( &iFlag );

	if( iFlag == 1 )
	{
		if( g_pInterfaceManager == NULL || g_pResourceManager == NULL )
			return 1;

		g_pInterfaceManager->ForceReply();

		m_bRoomOutBan	=	true;

		const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008152 );

		if( pszGlobalString != NULL )
			g_pInterfaceManager->SetNotice( pszGlobalString , this , NOTICE_TYPE_YES );
	}

	return 1;
}

int SPWindowTalkingRoom::OnPacketChangeMaster( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	m_bMasterChange	=	false;

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

	return 1;
}

int SPWindowTalkingRoom::OnPacketMessage( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	m_bMasterChange	=	false;

	int				iGlobalStringID	=	0;
	UINT8			iFlag			=	0;
	ROOM_MEMBER_UI	stRoomMember;

	pPacket->ExtractUINT8( &iFlag );
	pPacket->ExtractStruct( &( stRoomMember.m_stRoomMember ) , sizeof( ROOM_MEMBER ) );

	if( iFlag == 0 )
	{
		iGlobalStringID	=	5008112;

		SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

		bool	bLocalPlayer	=	false;

		if( g_pGOBManager != NULL )
		{
			SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

			if( pLocalPlayer != NULL )
			{
				char	szLocalName[ LEN_NAME + 1 ];

				pLocalPlayer->SPGOBPerformMessage( SPGM_GETGOBNAME , (LPARAM)szLocalName );

				std::string	strLocalName	=	szLocalName;

				if( strLocalName.compare( stRoomMember.m_stRoomMember.szCharName ) == 0 )
					bLocalPlayer	=	true;
			}
		}

		if( pChatManager != NULL )
		{
			if( pChatManager->GetJoin() == true && bLocalPlayer == true )
			{
				OnPurge( NULL , NULL );
				if( g_pInterfaceManager != NULL )
					g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_JOIN_CLEAR , NULL , NULL );
			}
		}

		if( bLocalPlayer == true )
			stRoomMember.m_eUserListColor	=	USER_LIST_COLOR_ME;
		else
			stRoomMember.m_eUserListColor	=	USER_LIST_COLOR_DEFAULT;

		DeleteRoomUser( stRoomMember );
		AddRommUser( stRoomMember , false );
	}
	else if( iFlag == 1 )
	{
		iGlobalStringID	=	5008113;
		DeleteRoomUser( stRoomMember );
	}
	else if( iFlag == 2 )
	{
		iGlobalStringID	=	5008151;
		DeleteRoomUser( stRoomMember );
	}
	else if( iFlag == 3 )
	{
		iGlobalStringID	=	5008114;
		ChangeLeaderRoomUser( stRoomMember );
	}

	char		szBuf[ _MAX_PATH ];
	const char*	pszGlobalString	=	g_pResourceManager->GetGlobalString( iGlobalStringID );

	if( pszGlobalString != NULL )
	{
		sprintf( szBuf , pszGlobalString , stRoomMember.m_stRoomMember.szCharName );

		CHAT_TOSS_DATA		stTossData;

		stTossData.m_eScope			=	SCOPE_SYSTEMMSG;
		stTossData.m_strMsg			=	szBuf;
		stTossData.m_cColor			=	D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

		OnGetMessage( (WPARAM)&stTossData , NULL );
	}

	UpdateUserListPage( m_iRoomUserCurPage );
	UpdateUserList();
	UpdateUserCount();

	return 1;
}

int SPWindowTalkingRoom::OnPacketMemberList( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	std::string	strLocalName;

	if( g_pGOBManager != NULL )
	{
		SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer != NULL )
		{
			char	szLocalName[ LEN_NAME + 1 ];

			pLocalPlayer->SPGOBPerformMessage( SPGM_GETGOBNAME , (LPARAM)szLocalName );

			strLocalName	=	szLocalName;
		}
	}

	m_vRoomUserList.clear();

	UINT32			uiMasterDBKey	=	0;
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		uiMasterDBKey	=	pChatManager->GetMasterDBKey();

	UINT32			uiSize;
	ROOM_MEMBER_UI	stRoomMember;
	bool			bMaster;

	pPacket->ExtractUINT32( &uiSize );

	for( int i = 0 ; i < (int)uiSize ; ++i )
	{
		pPacket->ExtractStruct( &( stRoomMember.m_stRoomMember ) , sizeof( ROOM_MEMBER ) );

		if( uiMasterDBKey == stRoomMember.m_stRoomMember.uiCharID )
			bMaster	=	true;
		else
			bMaster	=	false;

		if( strLocalName.compare( stRoomMember.m_stRoomMember.szCharName ) == 0 )
			stRoomMember.m_eUserListColor	=	USER_LIST_COLOR_ME;
		else
			stRoomMember.m_eUserListColor	=	USER_LIST_COLOR_DEFAULT;

		AddRommUser( stRoomMember , bMaster );
	}

	UpdateUserListPage( m_iRoomUserCurPage );
	UpdateUserList();
	UpdateUserCount();

	return 1;
}

int SPWindowTalkingRoom::OnPacketAdvert( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_TALKING_ROOM_ADVERTISING_OK , true );

	if( pWindow != NULL )
		pWindow->SetEnable( true );

	if( g_pCheckManager != NULL )
		g_pCheckManager->SetDBBlock( false );

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

	return 1;
}

int SPWindowTalkingRoom::OnRefreshOption( WPARAM wParam, LPARAM lParam )
{
	bool	bBtnEnable	=	true;
	int		iOnOffType	=	(WPARAM)wParam;

	if( iOnOffType == 0 )
	{
		SetDefaultWindowMyInfoSetup();
	}
	else
	{
		bBtnEnable	=	false;
	}

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_TALKING_ROOM_MY_INFO_SETUP_OK , true );

	if( pWindow != NULL )
		pWindow->SetEnable( bBtnEnable );

	return 1;
}

int SPWindowTalkingRoom::OnListNextLine( WPARAM wParam, LPARAM lParam )
{
	m_bNextLine	=	(bool)wParam;

	return 1;
}

int SPWindowTalkingRoom::OnGetMessage( WPARAM wParam, LPARAM lParam )
{
	CHAT_TOSS_DATA*		pTossData	=	(CHAT_TOSS_DATA*)wParam;

	if( pTossData == NULL )
		return 1;

	INSTANCE_ID		eActiveWindowLIstID			=	WIID_TALKING_ROOM_CHAT_LIST;
	INSTANCE_ID		eActiveWindowSliderID		=	WIID_TALKING_ROOM_CHAT_SLIDER;
	INSTANCE_ID		eActiveWindowUpID			=	WIID_TALKING_ROOM_CHAT_UP;
	INSTANCE_ID		eActiveWindowDownID			=	WIID_TALKING_ROOM_CHAT_DOWN;

	D3DXCOLOR		cColor						=	pTossData->m_cColor;

	if( pTossData->m_eScope == SCOPE_AROUNDREGION )
	{
		cColor	=	D3DCOLOR_ARGB( 255 , 49 , 76 , 97 );

		if( g_pGOBManager != NULL )
		{
			SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

			if( pLocalPlayer != NULL )
			{
				char	szLocalName[ LEN_NAME + 1 ];

				pLocalPlayer->SPGOBPerformMessage( SPGM_GETGOBNAME , (LPARAM)szLocalName );

				if( pTossData->m_strID.compare( szLocalName ) == 0 )
					cColor	=	D3DCOLOR_ARGB( 255 , 0 , 0 , 0 );
			}
		}
	}
	else if( pTossData->m_eScope == SCOPE_WHISPER )
	{
		cColor	=	D3DCOLOR_ARGB( 255 , 54 , 54 , 179 );
	}
	else if( pTossData->m_eScope == SCOPE_CHATROOM_ADVERT )
	{
		cColor	=	D3DCOLOR_ARGB( 255 , 49 , 76 , 97 );

		eActiveWindowLIstID			=	WIID_TALKING_ROOM_ADVERTISING_LIST;
		eActiveWindowSliderID		=	WIID_TALKING_ROOM_ADVERTISING_SLIDER;
		eActiveWindowUpID			=	WIID_TALKING_ROOM_ADVERTISING_UP;
		eActiveWindowDownID			=	WIID_TALKING_ROOM_ADVERTISING_DOWN;
	}
	else
	{
		cColor	=	D3DCOLOR_ARGB( 255 , 187 , 113 , 106 );
	}

	AddChatString( *pTossData , cColor , Find( eActiveWindowLIstID , true ) , Find( eActiveWindowSliderID , true ) , Find( eActiveWindowUpID , true ) , Find( eActiveWindowDownID , true ) );

	return 1;
}

int SPWindowTalkingRoom::OnNoticeYes( WPARAM wParam, LPARAM lParam )
{
	if( m_bRoomOut == true || m_bRoomOutBan == true )
	{
		SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

		if( pChatManager == NULL )
			return 1;

		CHATROOM_INFO*	pChatRoomInfo	=	pChatManager->GetChatRoomInfo();

		if( pChatRoomInfo == NULL )
			return 1;

		UINT8	iBanOut	=	0;

		if( m_bRoomOutBan == true )
			iBanOut	=	1;

		SendPacketOut( pChatRoomInfo->uiID , pChatRoomInfo->uiRoomNo , 0 , iBanOut );
	}
	else
	{
		GU_ID				ObjectGUID		=	0;

		if( g_pGOBManager != NULL )
		{
			SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

			if( pLocalPlayer != NULL )
				ObjectGUID	=	pLocalPlayer->GetGUID();
		}

		CPacket	Packet;
		//	Packet.AddUINT32( CHATROOM_SC_ROOMINFO_CHECK );
		Packet.AddUINT64( ObjectGUID );
		Packet.AddUINT32( 0 );
		Packet.AddData( &( m_stAdvertChatRoomInfo ) , sizeof( CHATROOM_INFO ) );

		if( g_pInterfaceManager != NULL )
			g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_ROOM_CHECK , (WPARAM)1 ,(LPARAM)&Packet );
	}

	OnNoticeNo( NULL , NULL );

	return 1;
}

int SPWindowTalkingRoom::OnNoticeNo( WPARAM wParam, LPARAM lParam )
{
	m_stAdvertChatRoomInfo.iCategory	=	0;
	m_stAdvertChatRoomInfo.uiID			=	0;
	m_stAdvertChatRoomInfo.uiRoomNo		=	0;

	m_bRoomOut							=	false;
	m_bRoomOutBan						=	false;

	ResetBan();

	return 1;
}

int SPWindowTalkingRoom::OnClose( WPARAM wParam, LPARAM lParam )
{
	if( g_pInterfaceManager == NULL || g_pResourceManager == NULL )
		return 1;

	g_pInterfaceManager->ForceReply();

	m_bRoomOut	=	true;

	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008149 );

	if( pszGlobalString != NULL )
		g_pInterfaceManager->SetNotice( pszGlobalString , this , NOTICE_TYPE_YESNO );

	return 1;
}

int SPWindowTalkingRoom::OnMin( WPARAM wParam, LPARAM lParam )
{
	SPWindow*	pWindow	=	Find( WIID_TALKING_ROOM_MAX_PANEL , true );

	if( pWindow != NULL )
	{
		pWindow->Hide();
		pWindow->SetShowEnable( false );
	}

	pWindow = Find( WIID_TALKING_ROOM_SUBJECT_TEXT , true );

	if( pWindow != NULL )
	{
		pWindow->SetRectSize( 263 ,12 );
	}

	ClearPlayerAction();

	return 1;
}

int SPWindowTalkingRoom::OnMax( WPARAM wParam, LPARAM lParam )
{
	SPWindow*	pWindow	=	Find( WIID_TALKING_ROOM_MAX_PANEL , true );

	if( pWindow != NULL )
	{
		pWindow->SetShowEnable( true );
		pWindow->Show();
	}

	pWindow = Find( WIID_TALKING_ROOM_SUBJECT_TEXT , true );

	if( pWindow != NULL )
	{
		pWindow->SetRectSize( 414 , 12 );
	}

	return 1;
}

int SPWindowTalkingRoom::OnKeyDown( WPARAM wParam, LPARAM lParam )
{
	if(wParam != VK_UP && wParam != VK_DOWN ) return 1;
	if(m_vstrHistoryBuffer.empty()) return 1;

	SPWindow*	pWindowEdit	=	Find( WIID_TALKING_ROOM_EDIT_ALL , true );

	if( pWindowEdit != NULL )
	{
		if( pWindowEdit->IsShow() == false || pWindowEdit->IsFocus() == false )
			return 1;
	}

	if(m_cInxHistory < 0 ) {
		m_cInxHistory = m_vstrHistoryBuffer.size() -1;
	}
	if(m_cInxHistory >= m_vstrHistoryBuffer.size() ) {
		m_cInxHistory = 0;
	}
	vector<string>::reverse_iterator rIter = m_vstrHistoryBuffer.rbegin() + m_cInxHistory;

	if(pWindowEdit) {	
		pWindowEdit->SetWindowText( (*rIter).c_str() );
		pWindowEdit->SPSendMessage( SPIM_SET_CARET_POS, CPOS_END, 0);
	}

	if(wParam == VK_UP) {
		++m_cInxHistory;
	} else if( wParam == VK_DOWN) {	
		--m_cInxHistory;
	}
	return 1;
}

int SPWindowTalkingRoom::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );
	return 1;
}

int SPWindowTalkingRoom::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );
	return 1;
}

int SPWindowTalkingRoom::OnRoomSetupPWEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	OnEditSetFocus( wParam , lParam );

	SPWindow*	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TEXT , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( NULL );

	return 1;
}

int SPWindowTalkingRoom::OnRoomSetupNumberEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	OnEditKillFocus( NULL , NULL );

	SPWindow*	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT , true );

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

int SPWindowTalkingRoom::OnAdvertListLButtonDblUp( WPARAM wParam, LPARAM lParam )
{
	SPWindowList*	pWindowList	=	(SPWindowList*)Find( WIID_TALKING_ROOM_ADVERTISING_LIST , true );

	if( pWindowList == NULL )
		return 1;

	int	iXPos	=	LOWORD( lParam );
	int	iYPos	=	HIWORD( lParam );

	LIST_RETURNVALUE*	pFindData	=	pWindowList->GetReturnValue( iXPos , iYPos );

	if( pFindData == NULL )
		return 1;

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager == NULL )
		return 1;

	CHATROOM_INFO*	pChatRoomInfo	=	pChatManager->GetAdvert( pFindData->m_iReturnValue );

	if( pChatRoomInfo == NULL )
		return 1;

	CHATROOM_INFO*	pMyChatRoomInfo	=	pChatManager->GetChatRoomInfo();

	if( pMyChatRoomInfo != NULL )
	{
		if( pMyChatRoomInfo->uiRoomNo	== pChatRoomInfo->uiRoomNo	&&
			pMyChatRoomInfo->uiID		== pChatRoomInfo->uiID		&&
			pMyChatRoomInfo->iCategory	== pChatRoomInfo->iCategory )
		{
			return 1;
		}
	}

	if( g_pInterfaceManager == NULL || g_pResourceManager == NULL )
		return 1;

	g_pInterfaceManager->ForceReply();

	m_stAdvertChatRoomInfo.iCategory	=	pChatRoomInfo->iCategory;
	m_stAdvertChatRoomInfo.uiID			=	pChatRoomInfo->uiID;
	m_stAdvertChatRoomInfo.uiRoomNo		=	pChatRoomInfo->uiRoomNo;

	strncpy( m_stAdvertChatRoomInfo.szPassword , pChatRoomInfo->szPassword , MAX_CHATROOM_PASSWORD_LEN );

	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008139 );

	if( pszGlobalString != NULL )
		g_pInterfaceManager->SetNotice( pszGlobalString , this , NOTICE_TYPE_YESNO );

	return 1;
}

int SPWindowTalkingRoom::OnAdvertisingListUp( WPARAM wParam, LPARAM lParam )
{
	SPWindow*	pWindow;

	pWindow	=	Find( WIID_TALKING_ROOM_ADVERTISING_SLIDER , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SLIDER_UP );

	return 1;
}

int SPWindowTalkingRoom::OnAdvertisingListDown( WPARAM wParam, LPARAM lParam )
{
	SPWindow*	pWindow;

	pWindow	=	Find( WIID_TALKING_ROOM_ADVERTISING_SLIDER , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SLIDER_DOWN );

	return 1;
}

int SPWindowTalkingRoom::OnAdvertisingSlider( WPARAM wParam, LPARAM lParam )
{
	SPWindowSlider*	pWindowSlider	=	(SPWindowSlider*)Find( WIID_TALKING_ROOM_ADVERTISING_SLIDER , true );

	if( pWindowSlider != NULL )
	{
		int iCurPage = pWindowSlider->GetCurPage();

		SPWindow*	pWindow	=	Find( WIID_TALKING_ROOM_ADVERTISING_LIST , true );

		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_SET_LINE , iCurPage );
	}

	return 1;
}

int SPWindowTalkingRoom::OnChatListUp( WPARAM wParam, LPARAM lParam )
{
	SPWindow*	pWindow;

	pWindow	=	Find( WIID_TALKING_ROOM_CHAT_SLIDER , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SLIDER_UP );

	return 1;
}

int SPWindowTalkingRoom::OnChatListDown( WPARAM wParam, LPARAM lParam )
{
	SPWindow*	pWindow;

	pWindow	=	Find( WIID_TALKING_ROOM_CHAT_SLIDER , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SLIDER_DOWN );

	return 1;
}

int SPWindowTalkingRoom::OnChatListLButtonUp( WPARAM wParam, LPARAM lParam )
{
	int	iXPos	=	LOWORD( lParam );
	int	iYPos	=	HIWORD( lParam );

	SPWindowList*	pWindowListChat	=	(SPWindowList*)Find( WIID_TALKING_ROOM_CHAT_LIST , true );

	if( pWindowListChat == NULL )
		return 1;

	LIST_RETURNVALUE*	pFindData	=	pWindowListChat->GetReturnValue( iXPos , iYPos );

	if( pFindData != NULL )
	{
		if( pFindData->m_strReturnValue.empty() == false )
		{
			SetWhisperMode( pFindData->m_strReturnValue.c_str() );
		}
	}

	return 1;
}

int SPWindowTalkingRoom::OnUserListLeft( WPARAM wParam, LPARAM lParam )
{
	UpdateUserListPage( --m_iRoomUserCurPage );
	UpdateUserList();
	return 1;
}

int SPWindowTalkingRoom::OnUserListRight( WPARAM wParam, LPARAM lParam )
{
	UpdateUserListPage( ++m_iRoomUserCurPage );
	UpdateUserList();
	return 1;
}

int SPWindowTalkingRoom::OnBtnAdvertising( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_TALKING_ROOM_ADVERTISING_PANEL ) , true );
	g_pInterfaceManager->SetFocusWindow(  Find( WIID_TALKING_ROOM_ADVERTISING_EDIT , true ) );
	return 1;
}

int SPWindowTalkingRoom::OnBtnSearch( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->ShowWindow( WIID_CHATTING_ROOM , true );
	g_pInterfaceManager->SetFocusWindow( g_pInterfaceManager->FindWindow( WIID_CHATTING_ROOM ) );
	return 1;
}

int SPWindowTalkingRoom::OnBtnMyInfoSetup( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_TALKING_ROOM_MY_INFO_SETUP_PANEL ) , true );
	return 1;
}

int SPWindowTalkingRoom::OnBtnRoomInfoSetup( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_TALKING_ROOM_INFO_SETUP_PANEL ) , true );
	return 1;
}

int SPWindowTalkingRoom::OnBtnAdvertisingOK( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_TALKING_ROOM_ADVERTISING_PANEL ) , false );

	//////////////////////////////////////////////////////////////////////////

	if( g_pGOBManager == NULL )
		return 1;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	SPPlayerInvenArchive*	pLocalPlayerInvenArchive = pLocalPlayer->GetInvenArchive();

	if( pLocalPlayerInvenArchive == NULL )
		return 1;

	int				iRequireItem	=	0;
	CHATROOM_INFO*	pChatRoomInfo	=	NULL;
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		iRequireItem	=	pChatManager->GetRequire( CHAT_LDT_REQUIRE_ITEM );
		pChatRoomInfo	=	pChatManager->GetChatRoomInfo();
	}

	if( pChatRoomInfo == NULL )
		return 1;

	TRASH_ITEM	stConsumeItem;
	SPItemAttr*	pItemAttr	=	NULL;

	if( g_pItemCluster->GetItemInfo( iRequireItem , pItemAttr ) == false )
		return 1;

	if( pItemAttr == NULL )
		return 1;

	CONTAINER_ITEM	Item;
	ZeroMemory( &Item , sizeof( CONTAINER_ITEM ) );

	CONTAINER_TYPE	eItemType = (CONTAINER_TYPE)( pItemAttr->m_eType - 1 );
	SPItem* pRequireItem = pLocalPlayerInvenArchive->GetSPItem( eItemType , iRequireItem );

	if( pRequireItem != NULL )
	{
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

	SPAbuseFilter*	pSPAbuseFilter	=	SPAbuseFilter::GetInstance();

	SPWindow*		pWindow;
	const char*		pStringBuf;
	std::string		strBuf;

	UINT8			iChatRoomAdvertLen;
	TCHAR			szAdvert[ MAX_CHATROOM_ADVERT_LEN + 1 ];

	memset( &( szAdvert )	, 0 , sizeof( szAdvert )	);

	pWindow	=	Find( WIID_TALKING_ROOM_ADVERTISING_EDIT , true );

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

		if( iChatRoomAdvertLen < 4 )
		{
			PrintErrorMsg( 5008102 );
			return 1;
		}

		strncpy( szAdvert , strBuf.c_str() , MAX_CHATROOM_ADVERT_LEN );
	}

	CPacket	Packet;
	Packet.AddUINT32( CHATROOM_CS_ADVERT );
	Packet.AddUINT32( pChatRoomInfo->uiID );
	Packet.AddUINT32( pChatRoomInfo->uiRoomNo );
	Packet.AddData( &( stConsumeItem ) , sizeof( TRASH_ITEM ) );
	Packet.AddUINT16( iChatRoomAdvertLen );
	Packet.Add( (TCHAR*)szAdvert , iChatRoomAdvertLen );

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

			if( g_pCheckManager != NULL )
				g_pCheckManager->SetDBBlock( true );

			pWindow	=	Find( WIID_TALKING_ROOM_ADVERTISING_OK , true );

			if( pWindow != NULL )
				pWindow->SetEnable( false );
		}
	}

	return 1;
}

int SPWindowTalkingRoom::OnBtnAdvertisingCancel( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_TALKING_ROOM_ADVERTISING_PANEL ) , false );

	return 1;
}

int SPWindowTalkingRoom::OnBtnMyInfoSearch( WPARAM wParam, LPARAM lParam )
{
	SetButtonCheckInv( Find( WIID_TALKING_ROOM_MY_INFO_SETUP_MY_INFO_SEARCH , true ) );
	return 1;
}

int SPWindowTalkingRoom::OnBtnChatRequest( WPARAM wParam, LPARAM lParam )
{
	SetButtonCheckInv( Find( WIID_TALKING_ROOM_MY_INFO_SETUP_CHAT_REQUEST , true ) );
	return 1;
}

int SPWindowTalkingRoom::OnBtnMyStyleOpen( WPARAM wParam, LPARAM lParam )
{
	SetButtonCheckInv( Find( WIID_TALKING_ROOM_MY_INFO_SETUP_MY_STYLE_OPEN , true ) );
	return 1;
}

int SPWindowTalkingRoom::OnBtnMyInfoSetupOK( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_TALKING_ROOM_MY_INFO_SETUP_PANEL ) , false );

	//////////////////////////////////////////////////////////////////////////

	if( g_pClientOption == NULL )
		return 1;

	SERVER_SIDE_OPTION	stSrcSvrOption	=	g_pClientOption->GetServerSideOption();
	SERVER_SIDE_OPTION	stCurSvrOption	=	stSrcSvrOption;

	SPWindowButton*		pWindowButton;

	pWindowButton	=	(SPWindowButton*)Find( WIID_TALKING_ROOM_MY_INFO_SETUP_MY_INFO_SEARCH , true );

	if( pWindowButton != NULL )
	{
		if( pWindowButton->IsChecked() == true )
			stCurSvrOption	|=	SSO_REQ_CHATROOM_UNUSED;
		else
			stCurSvrOption	&=	~SSO_REQ_CHATROOM_UNUSED;
	}

	pWindowButton	=	(SPWindowButton*)Find( WIID_TALKING_ROOM_MY_INFO_SETUP_CHAT_REQUEST , true );

	if( pWindowButton != NULL )
	{
		if( pWindowButton->IsChecked() == true )
			stCurSvrOption	|=	SSO_REQ_CHATROOM_INVITE_DENIAL;
		else
			stCurSvrOption	&=	~SSO_REQ_CHATROOM_INVITE_DENIAL;
	}

	pWindowButton	=	(SPWindowButton*)Find( WIID_TALKING_ROOM_MY_INFO_SETUP_MY_STYLE_OPEN , true );

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

				if( g_pInterfaceManager != NULL )
					g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_OPTION , (WPARAM)1 , NULL );
			}	
		}
	}

	return 1;
}

int SPWindowTalkingRoom::OnBtnMyInfoSetupCancel( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_TALKING_ROOM_MY_INFO_SETUP_PANEL ) , false );

	return 1;
}

int SPWindowTalkingRoom::OnBtnInfoSetupGenderLeft( WPARAM wParam, LPARAM lParam )
{
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_GENDER , --m_iCurIndexRoomModify[ ROOM_STYLE_GENDER ] , Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TEXT , true ) );
	return 1;
}

int SPWindowTalkingRoom::OnBtnInfoSetupGenderRight( WPARAM wParam, LPARAM lParam )
{
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_GENDER , ++m_iCurIndexRoomModify[ ROOM_STYLE_GENDER ] , Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TEXT , true ) );
	return 1;
}

int SPWindowTalkingRoom::OnBtnInfoSetupAgeLeft( WPARAM wParam, LPARAM lParam )
{
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_AGE , --m_iCurIndexRoomModify[ ROOM_STYLE_AGE ] , Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TEXT , true ) );
	return 1;
}

int SPWindowTalkingRoom::OnBtnInfoSetupAgeRight( WPARAM wParam, LPARAM lParam )
{
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_AGE , ++m_iCurIndexRoomModify[ ROOM_STYLE_AGE ] , Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TEXT , true ) );
	return 1;
}

int SPWindowTalkingRoom::OnBtnInfoSetupPasswordChange( WPARAM wParam, LPARAM lParam )
{
	SetButtonCheckInv( Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_CHANGE , true ) );
	return 1;
}

int SPWindowTalkingRoom::OnBtnInfoSetupOK( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_TALKING_ROOM_INFO_SETUP_PANEL ) , false );

	//////////////////////////////////////////////////////////////////////////

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();
	SPIndunManager*	pIndunManager	=	SPIndunManager::GetInstance();

	if( pChatManager != NULL && pIndunManager != NULL )
	{
		if( pChatManager->IsChatRoom() == false && pIndunManager->IsInIndun() == true )
		{
			PrintErrorMsg( 5008100 );
			return 1;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	SPAbuseFilter*	pSPAbuseFilter	=	SPAbuseFilter::GetInstance();

	UINT8			iPassWord		=	0;							// 0: 패스워드 미변경, 1: 패스워드 변경 포함시.
	CHATROOM_INFO	stChatRoomInfo	=	{ 0, };

	SPWindow*		pWindow;
	const char*		pStringBuf;
	std::string		strBuf;

	if( pChatManager != NULL )
	{
		CHATROOM_INFO*	pChatRoomInfo	=	pChatManager->GetChatRoomInfo();

		if( pChatRoomInfo != NULL )
			CopyMemory( &stChatRoomInfo , pChatRoomInfo , sizeof( CHATROOM_INFO ) );
	}

	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_EDIT , true );

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

	stChatRoomInfo.iCategory	=	m_iCurIndexRoomModify[ ROOM_STYLE_TYPE ];
	stChatRoomInfo.iSex			=	m_iCurIndexRoomModify[ ROOM_STYLE_GENDER ];
	stChatRoomInfo.iAge			=	m_iCurIndexRoomModify[ ROOM_STYLE_AGE ];

	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT , true );

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

	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TEXT , true );

	if( pWindow != NULL )
	{
		SPWindowButton*		pWindowButton;

		pWindowButton	=	(SPWindowButton*)Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_CHANGE , true );

		if( pWindowButton != NULL )
		{
			if( pWindowButton->IsChecked() == true )
			{
				pStringBuf	=	pWindow->GetWindowText();

				if( pStringBuf != NULL )
					strBuf	=	pStringBuf;

				if( (int)strBuf.size() > 0 )
					strncpy( stChatRoomInfo.szPassword , strBuf.c_str() , MAX_CHATROOM_PASSWORD_LEN );
				else
					memset( &( stChatRoomInfo.szPassword )	, 0 , sizeof( stChatRoomInfo.szPassword )	);

				iPassWord	=	1;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	if( pChatManager != NULL )
	{
		CHATROOM_INFO*	pChatRoomInfo	=	pChatManager->GetChatRoomInfo();

		if( pChatRoomInfo != NULL )
		{
			if( _tcscmp( pChatRoomInfo->szTitle , stChatRoomInfo.szTitle ) == 0		&&
				pChatRoomInfo->iCategory	== stChatRoomInfo.iCategory				&&
				pChatRoomInfo->iAge			== stChatRoomInfo.iAge					&&
				pChatRoomInfo->iSex			== stChatRoomInfo.iSex					&&
				pChatRoomInfo->iLimitNum	== stChatRoomInfo.iLimitNum				&&
				_tcscmp( pChatRoomInfo->szPassword , stChatRoomInfo.szPassword ) == 0
				)
			{
				return 1;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	CPacket	Packet;
	Packet.AddUINT32( CHATROOM_CS_MODIFY );
	Packet.AddUINT8( iPassWord );
	Packet.AddData( &( stChatRoomInfo ) , sizeof( CHATROOM_INFO ) );

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

			SPWindow*	pWindow;

			pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_OK , true );

			if( pWindow != NULL )
				pWindow->SetEnable( false );

			PrintErrorMsg( 5008145 );
		}
	}

	return 1;
}

int SPWindowTalkingRoom::OnBtnInfoSetupCancel( WPARAM wParam, LPARAM lParam )
{
	SetModalWindowShow( Find( WIID_TALKING_ROOM_INFO_SETUP_PANEL ) , false );

	return 1;
}

int SPWindowTalkingRoom::OnChatSlider( WPARAM wParam, LPARAM lParam )
{
	SPWindowSlider*	pWindowSlider	=	(SPWindowSlider*)Find( WIID_TALKING_ROOM_CHAT_SLIDER , true );

	if( pWindowSlider != NULL )
	{
		int iCurPage = pWindowSlider->GetCurPage();

		SPWindow*	pWindow	=	Find( WIID_TALKING_ROOM_CHAT_LIST , true );

		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_SET_LINE , iCurPage );
	}

	return 1;
}

int SPWindowTalkingRoom::OnSend( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow*		pWindow;
	unsigned int	uiInstanceID	=	WIID_TALKING_ROOM_SEND_ALL;
	unsigned int	uiInstanceIDEnd	=	WIID_TALKING_ROOM_SEND_WHISPER;
	bool			bSelect;
	int				iForceShow		=	(int)wParam;

	if( iForceShow == 0 )
	{
		if( GetActiveMenuWindow( WIID_TALKING_ROOM_SEND_ALL , uiInstanceIDEnd , 1 ) == iID )
			return 1;
	}

	for( uiInstanceID = WIID_TALKING_ROOM_SEND_ALL ; uiInstanceID <= uiInstanceIDEnd ; uiInstanceID += 1 )
	{
		pWindow	=	Find( uiInstanceID );

		if( pWindow != NULL )
		{
			if( uiInstanceID != iID )
				bSelect	=	false;
			else
				bSelect =	true;

			pWindow->SPSendMessage( SPIM_BTN_SELECT , bSelect );

			if( bSelect == true )
				SetEditWindow( iID );
		}
	}

	return 1;
}

int SPWindowTalkingRoom::OnPlayer( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow*		pWindow;
	unsigned int	uiInstanceID	=	WIID_TALKING_ROOM_PLAYER_SELECT_ICON;
	unsigned int	uiInstanceIDEnd	=	WIID_TALKING_ROOM_PLAYER_SELECT_ICON	+	TALKING_ROOM_PLAYER_COUNT;
	bool			bSelect;

	if( GetActiveMenuWindow( WIID_TALKING_ROOM_PLAYER_SELECT_ICON , uiInstanceIDEnd , 1 ) == iID )
		return 1;

	for( uiInstanceID = WIID_TALKING_ROOM_PLAYER_SELECT_ICON ; uiInstanceID <= uiInstanceIDEnd ; uiInstanceID += 1 )
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

	pWindow	=	Find( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_PANEL , true );

	if( pWindow != NULL )
	{
		bool			bMaster			=	false;
		SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

		if( pChatManager != NULL )
			bMaster	=	pChatManager->IsMaster();

		SPWindow*	pBtnWindow;

		pBtnWindow	=	pWindow->Find( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_MASTER_CHANGE , true );

		if( pBtnWindow != NULL )
			pBtnWindow->SetEnable( bMaster & !m_bMasterChange );

		pBtnWindow	=	pWindow->Find( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_FORCE_LEAVE , true );

		if( pBtnWindow != NULL )
			pBtnWindow->SetEnable( bMaster & !m_bForceOut );

		//////////////////////////////////////////////////////////////////////////

		pWindow->SetShowEnable( true );
		pWindow->AdjustCoord( 149 , 53 + ( ( iID - WIID_TALKING_ROOM_PLAYER_SELECT_ICON ) * 20 ) - 3 );
		pWindow->RefreshRelationCoord();
		pWindow->Show();
		pWindow->SetShowEnable( false , false );
	}

	return 1;
}

int SPWindowTalkingRoom::OnPlayerClear( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	ClearPlayerAction();
	return 1;
}

int SPWindowTalkingRoom::OnPlayerAction( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	unsigned int	uiInstanceID	=	WIID_TALKING_ROOM_PLAYER_SELECT_ICON;
	unsigned int	uiInstanceIDEnd	=	WIID_TALKING_ROOM_PLAYER_SELECT_ICON	+	TALKING_ROOM_PLAYER_COUNT;

	int	iCurIndex	=	0;
	int	iUserCount	=	(int)m_vRoomUserList.size();

	iCurIndex		=	(int)( ( GetActiveMenuWindow( uiInstanceID , uiInstanceIDEnd , 1 ) - uiInstanceID ) + m_iRoomUserCurPage * TALKING_ROOM_PLAYER_COUNT );

	if( iCurIndex >= iUserCount )
		return 1;

	if( iCurIndex < 0 )
		return 1;

	ROOM_MEMBER_UI*	pRoomUser	=	&( m_vRoomUserList.at( iCurIndex ) );

	if( pRoomUser == NULL )
		return 1;

	ClearPlayerAction();

	std::string		strTargetName	=	pRoomUser->m_stRoomMember.szCharName;
	SPPlayer*		pLocalPlayer	=	NULL;
	SPGuildArchive*	pGuildArchive	=	NULL;
	CHATROOM_INFO*	pChatRoomInfo	=	NULL;

	if( g_pGOBManager != NULL )
	{
		pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		SPLocalizeManager*	pLocalizeManager	=	SPLocalizeManager::GetInstance();

		if( pLocalizeManager != NULL )
		{
			if( pLocalizeManager->IsEnableValue( LV_GUILD ) == true )
				pGuildArchive	=	g_pGOBManager->GetGuildArchive();
		}
	}

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		pChatRoomInfo	=	pChatManager->GetChatRoomInfo();

	//////////////////////////////////////////////////////////////////////////

	char	szLocalName[ LEN_NAME + 1 ];

	if( pLocalPlayer != NULL )
		pLocalPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szLocalName );

	if( strTargetName.compare( szLocalName ) == 0 )
		return 1;

	if( pChatManager != NULL )
	{
		if( pChatManager->GetLocalDBKey() == pRoomUser->m_stRoomMember.uiCharID )
			return 1;
	}

	//////////////////////////////////////////////////////////////////////////

	switch( iID )
	{
	case WIID_TALKING_ROOM_MAX_PLAYER_ACTION_WHISPER:
		SetWhisperMode( strTargetName.c_str() );
		break;
	case WIID_TALKING_ROOM_MAX_PLAYER_ACTION_PARTY_INVITE:
		if( pLocalPlayer != NULL )
			pLocalPlayer->SPGOBPerformMessage( SPGM_TEAM_INVITE_REQ , (LPARAM)strTargetName.c_str() );
		break;
	case WIID_TALKING_ROOM_MAX_PLAYER_ACTION_GUILD_INVITE:
		if( pGuildArchive != NULL )
			pGuildArchive->SendGuildInvite( strTargetName.c_str() );
		break;
	case WIID_TALKING_ROOM_MAX_PLAYER_ACTION_FRIEND_ADD:
		if( g_pInterfaceManager != NULL )
		{
			g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_USER_ADD_FRIEND , (WPARAM)FRIEND_TYPE_WHITE , (LPARAM)strTargetName.c_str() );
		}
		break;
	case WIID_TALKING_ROOM_MAX_PLAYER_ACTION_MASTER_CHANGE:
		if( pChatRoomInfo != NULL )
			SendChangeMaster( pChatRoomInfo->uiID , pChatRoomInfo->uiRoomNo , pRoomUser->m_stRoomMember.uiCharID );
	    break;
	case WIID_TALKING_ROOM_MAX_PLAYER_ACTION_FORCE_LEAVE:
		if( pChatRoomInfo != NULL )
			SendPacketOut( pChatRoomInfo->uiID , pChatRoomInfo->uiRoomNo , pRoomUser->m_stRoomMember.uiCharID , 0 );
		break;
	}

	return 1;
}

void SPWindowTalkingRoom::InitWindowMax( SPWindow* pParentWindow )
{
	if( pParentWindow == NULL )
		return;

	SPWindowStatic*			pWindowStatic;
	SPWindowButton*			pWindowButton;
	int						iPosX , iPosY;

	iPosX	=	373;
	iPosY	=	0;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_CLOSE , 130 , 9 - iPosY , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 27 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 27 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 40 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 40 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MIN , 365 - iPosX , 181 - iPosY , 6 , 41 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 153 , 241 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 160 , 241 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 167 , 241 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 174 , 241 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 167 , 241 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_BLIND_LEFT_TOP , 351 - iPosX , 0 - iPosY , 22 , 4 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 487 , 467 );
	pWindowStatic->SetSrcSize( 4 , 4 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_BLIND_CENTER_TOP , 351 - iPosX , 4 - iPosY , 22 , 22 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 487 , 472 );
	pWindowStatic->SetSrcSize( 4 , 4 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_BLIND_LEFT_BOTTOM , 369 - iPosX , 373 - iPosY , 4 , 4 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 487 , 477 );
	pWindowStatic->SetSrcSize( 4 , 4 );
	pWindowStatic	=	NULL;

	iPosX	=	0;
	iPosY	=	27;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_WHITE_BG , iPosX , iPosY , 149 , 346 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 502 , 472 );
	pWindowStatic->SetSrcSize( 4 , 4 );
	pParentWindow	=	pWindowStatic;
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_LINE_1 , 0 - iPosX , 26 - iPosY , 149 , 1 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 508 , 450 );
	pWindowStatic->SetSrcSize( 2 , 1 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_JOIN_PERSON_COUNT , 0 - iPosX , 53 - iPosY , 88 , 14 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 221 , 433 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_LINE_2 , 0 - iPosX , 67 - iPosY , 145 , 2 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 180 , 308 );
	pWindowStatic->SetSrcSize( 2 , 2 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_JOIN_PERSON_TEXT , 104 - iPosX , 54 - iPosY , 41 , 12 , pParentWindow );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 104 , 113 , 145 ) );
	pWindowStatic->SetFormat( DT_RIGHT | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_NORMAL );
	pWindowStatic->SetWindowText("99/99");
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_PLAYER_PAGE_LEFT , 33 - iPosX , 285 - iPosY , 13 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 383 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 397 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 425 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_PLAYER_PAGE_RIGHT , 103 - iPosX , 285 - iPosY , 13 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 383 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 397 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 425 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_PLAYER_PAGE_TEXT , 48 - iPosX , 286 - iPosY , 53 , 12 , pParentWindow );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_BOLD );
	pWindowStatic->SetWindowText("1 / 5");
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	InitWindowPlayerList( pParentWindow );

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_SETUP_IMG , 0 - iPosX , 312 - iPosY , 88 , 14 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 2 , 433 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_SETUP_LINE , 0 - iPosX , 326 - iPosY , 145 , 2 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 180 , 308 );
	pWindowStatic->SetSrcSize( 2 , 2 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_ADVERTISING , 3 - iPosX , 332 - iPosY , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 1 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 1 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 1 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 1 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 1 , 37 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_SEARCH , 75 - iPosX , 332 - iPosY , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 70 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 70 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 70 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 70 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 70 , 37 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_MY_INFO_SETUP , 3 - iPosX , 353 - iPosY , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 139 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 139 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 139 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 139 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 139 , 37 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_ROOM_INFO_SETUP , 75 - iPosX , 353 - iPosY , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 208 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 208 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 208 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 208 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_003.PNG" , 208 , 37 );
	pWindowButton	=	NULL;

}

void SPWindowTalkingRoom::InitWindowPlayerList( SPWindow* pParentWindow )
{
	if( pParentWindow == NULL )
		return;

	SPWindowStatic*			pWindowStatic;
	SPWindowButton*			pWindowButton;
	RECT*					prcSrc;

	int						j;
	int						iPosY;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_PANEL , 149 , 53 , 111 , 118 , pParentWindow );
	pWindowStatic->SetWindowType( WND_TYPE_NINEBASE );
	pWindowStatic->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 437 , 467 , 4 , 1 );
	pWindowStatic->SetShowEnable( false );
	InitWindowPlayerAction( pWindowStatic );
	pWindowStatic	=	NULL;

	for( int i = 0 ; i < TALKING_ROOM_PLAYER_COUNT ; ++i )
	{
		iPosY	=	( 77 - 27 ) + i * 20;

		pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_PLAYER_SELECT_ICON + i , 0 , iPosY , 151 , 20 , pParentWindow );
//		pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 383 );
		pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 262 );
		pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 262 );
//		pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 425 );
		pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 241 );

		pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_PLAYER_CLASS_ICON + i , 8 , 2 + iPosY , 16 , 16 , pParentWindow );
		pWindowStatic->SetImage( (SPTexture*)NULL );
		for( j = CLASS_A ; j < CLASS_MAX ; ++j )
		{
			prcSrc	=	g_pInterfaceManager->GetClassTextureSrc( (CLASS_TYPE)j );

			if( prcSrc != NULL )
				pWindowStatic->SetImage( g_pInterfaceManager->GetClassTextureName( (CLASS_TYPE)j ) , prcSrc->left , prcSrc->top );
		}
		pWindowStatic->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_PLAYER_NAME_TEXT + i , 30 , 4 + iPosY , 110 , 12 , pParentWindow );
		pWindowStatic->SetFontColor( m_cUserListTextColor[ USER_LIST_COLOR_DEFAULT ] );
		pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
		pWindowStatic->SetFont( FONT_12_NORMAL );
		pWindowStatic->SetWindowText("일이삼사오육칠팔구십");
		pWindowStatic	=	NULL;
	}
}

void SPWindowTalkingRoom::InitWindowPlayerAction( SPWindow* pParentWindow )
{
	if( pParentWindow == NULL )
		return;

	SPWindowStatic*			pWindowStatic;
	SPWindowButton*			pWindowButton;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_WHISPER , 11 , 7 , 85 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 339 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 353 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 367 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 381 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 367 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_PARTY_INVITE , 11 , 25 , 85 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 283 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 297 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 311 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 325 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 1 , 311 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_GUILD_INVITE , 11 , 43 , 85 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 87 , 283 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 87 , 297 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 87 , 311 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 87 , 325 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 87 , 311 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_FRIEND_ADD , 11 , 61 , 85 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 173 , 283 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 173 , 297 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 173 , 311 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 173 , 325 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 173 , 311 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_MASTER_CHANGE , 11 , 79 , 85 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 259 , 283 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 259 , 297 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 259 , 311 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 259 , 325 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 259 , 311 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_FORCE_LEAVE , 11 , 97 , 85 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 345 , 283 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 345 , 297 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 345 , 311 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 345 , 325 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 345 , 311 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_LINE_1 , 11 , 22 , 85 , 1 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 162 , 309 );
	pWindowStatic->SetSrcSize( 3 , 1 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_LINE_2 , 11 , 40 , 85 , 1 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 162 , 309 );
	pWindowStatic->SetSrcSize( 3 , 1 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_LINE_3 , 11 , 58 , 85 , 1 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 162 , 309 );
	pWindowStatic->SetSrcSize( 3 , 1 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_LINE_4 , 11 , 76 , 85 , 1 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 162 , 309 );
	pWindowStatic->SetSrcSize( 3 , 1 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_LINE_5 , 11 , 94 , 85 , 1 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 162 , 309 );
	pWindowStatic->SetSrcSize( 3 , 1 );
	pWindowStatic	=	NULL;
}

void SPWindowTalkingRoom::InitWindowAdvertising( SPWindow* pParentWindow )
{
	if( pParentWindow == NULL )
		return;

	SPWindowStatic*			pWindowStatic;
	SPWindowButton*			pWindowButton;
	SPWindowEditMultiLine*	pWindowMultiEdit;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_ADVERTISING_BG , 149 , 133 - 30 , 225 , 111 , pParentWindow );
	pWindowStatic->SetWindowType( WND_TYPE_NINEBASE );
	pWindowStatic->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 497 , 467 , 4 , 1 );
	pParentWindow	=	pWindowStatic;
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_ADVERTISING_TITLE_IMG , 9 , 6 , 118 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 219 , 396 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_ADVERTISING_CLOSE , 204 , 10 , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 27 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 27 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 40 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 40 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_ADVERTISING_EDIT_BG_LEFT , 14 , 31 , 4 , 32 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 367 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_ADVERTISING_EDIT_BG_CENTER , 18 , 31 , 189 , 32 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 367 );
	pWindowStatic->SetSrcSize( 4 , 32 );

	pWindowMultiEdit	=	new SPWindowEditMultiLine( WIID_TALKING_ROOM_ADVERTISING_EDIT , 2 , 4 , 186 , 28 , pWindowStatic );
	pWindowMultiEdit->SetLimitText( MAX_CHATROOM_ADVERT_LEN );
	pWindowMultiEdit->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowMultiEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowMultiEdit->SPSendMessage( SPIM_SET_MARGIN , 2 );

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_ADVERTISING_EDIT_BG_RIGHT , 207 , 31 , 4 , 32 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 367 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_ADVERTISING_HELP_IMG , 6 , 68 , 213 , 14 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 88 , 449 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_ADVERTISING_OK , 43 , 85 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 73 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 91 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 109 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 127 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 109 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_ADVERTISING_CANCEL , 116 , 85 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 145 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 163 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 181 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 199 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 415 , 181 );
	pWindowButton	=	NULL;
}

void SPWindowTalkingRoom::InitWindowMyInfoSetup( SPWindow* pParentWindow )
{
	if( pParentWindow == NULL )
		return;

	SPWindowStatic*			pWindowStatic;
	SPWindowButton*			pWindowButton;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MY_INFO_SETUP_BG , 169 , 126 - 30 , 185 , 124 , pParentWindow );
	pWindowStatic->SetWindowType( WND_TYPE_NINEBASE );
	pWindowStatic->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 497 , 467 , 4 , 1 );
	pParentWindow	=	pWindowStatic;
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MY_INFO_SETUP_TITLE_IMG , 11 , 5 , 118 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 219 , 378 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MY_INFO_SETUP_CLOSE , 164 , 11 , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 27 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 27 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 40 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 40 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MY_INFO_SETUP_MY_INFO_SEARCH , 18 , 37 , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 105 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 105 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 118 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MY_INFO_SETUP_CHAT_REQUEST , 18 , 57 , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 105 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 105 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 118 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MY_INFO_SETUP_MY_STYLE_OPEN , 18 , 77 , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 105 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 105 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 118 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MY_INFO_SETUP_MY_INFO_SEARCH_IMG , 41 , 35 , 104 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 407 , 246 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MY_INFO_SETUP_CHAT_REQUEST_IMG , 41 , 55 , 104 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 407 , 262 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_MY_INFO_SETUP_MY_STYLE_OPEN_IMG , 41 , 75 , 104 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 407 , 278 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MY_INFO_SETUP_OK , 21 , 97 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 73 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 91 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 109 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 127 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 109 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_MY_INFO_SETUP_CANCEL , 97 , 97 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 37 );
	pWindowButton	=	NULL;
}

void SPWindowTalkingRoom::InitWindowRoomSetup( SPWindow* pParentWindow )
{
	if( pParentWindow == NULL )
		return;

	SPWindowStatic*			pWindowStatic;
	SPWindowButton*			pWindowButton;
	SPWindowEdit*			pWindowEdit;
	SPWindowEditMultiLine*	pWindowMultiEdit;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_BG , 148 , 71 - 30 , 228 , 215 , pParentWindow );
	pWindowStatic->SetWindowType( WND_TYPE_NINEBASE );
	pWindowStatic->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 497 , 467 , 4 , 1 );
	pParentWindow	=	pWindowStatic;
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_TITLE_IMG , 10 , 7 , 118 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 393 , 330 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_INFO_SETUP_CLOSE , 207 , 10 , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 27 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 27 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 40 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 40 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_IMG , 14 , 30 , 85 , 14 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 339 , 216 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_EDIT_BG_LEFT , 14 , 47 , 4 , 32 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 367 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_EDIT_BG_CENTER , 18 , 47 , 192 , 32 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 367 );
	pWindowStatic->SetSrcSize( 4 , 32 );

	pWindowMultiEdit	=	new SPWindowEditMultiLine( WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_EDIT , 19 - 18 , 51 - 47 , 190 , 28 , pWindowStatic );
	pWindowMultiEdit->SetLimitText( MAX_CHATROOM_TITLE_LEN );
	pWindowMultiEdit->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowMultiEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowMultiEdit->SPSendMessage( SPIM_SET_MARGIN , 2 );

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_EDIT_BG_RIGHT , 210 , 47 , 4 , 32 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 367 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_IMG , 14 , 86 , 85 , 14 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 339 , 231 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TITLE_LEFT , 14 , 103 , 2 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 50 , 464 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TITLE_CENTER , 16 , 103 , 69 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 60 , 464 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TITLE_RIGHT , 85 , 103 , 2 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 137 , 464 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_ARROW_LEFT , 93 , 104 , 13 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 383 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 397 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 425 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_ARROW_RIGHT , 203 , 104 , 13 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 383 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 397 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 425 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TEXT_BG_LEFT , 108 , 103 , 4 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 333 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TEXT_BG_CENTER , 112 , 103 , 85 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 333 );
	pWindowStatic->SetSrcSize( 4 , 15 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TEXT_BG_RIGHT , 197 , 103 , 4 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 333 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TEXT , 108 , 105 , 93 , 12 , pParentWindow );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_NORMAL );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TITLE_LEFT , 14 , 122 , 2 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 1 , 496 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TITLE_CENTER , 16 , 122 , 69 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 11 , 496 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TITLE_RIGHT , 85 , 122 , 2 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 88 , 496 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_ARROW_LEFT , 93 , 123 , 13 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 383 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 397 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 425 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 411 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_ARROW_RIGHT , 203 , 123 , 13 , 13 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 383 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 397 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 425 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 498 , 411 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TEXT_BG_LEFT , 108 , 122 , 4 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 333 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TEXT_BG_CENTER , 112 , 122 , 85 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 333 );
	pWindowStatic->SetSrcSize( 4 , 15 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TEXT_BG_RIGHT , 197 , 122 , 4 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 333 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TEXT , 108 , 124 , 93 , 12 , pParentWindow );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_NORMAL );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TITLE_LEFT , 14 , 145 , 2 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 332 , 448 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TITLE_CENTER , 16 , 145 , 69 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 342 , 448 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TITLE_RIGHT , 85 , 145 , 2 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 419 , 448 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT_BG_LEFT , 95 , 144 , 4 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 349 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT_BG_CENTER , 99 , 144 , 111 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 349 );
	pWindowStatic->SetSrcSize( 4 , 17 );

	pWindowEdit	=	new SPWindowEdit( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT , 123 - 99 , 148 - 144 , 28 , 12 , pWindowStatic );
	pWindowEdit->SetWindowType( WND_TYPE_LEFTBASE );
	pWindowEdit->SetMargin( 0 , 0  );
	pWindowEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowEdit->SetFormat( DT_RIGHT | DT_VCENTER );
	pWindowEdit->SetLimitText( 4 );

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT_BG_RIGHT , 210 , 144 , 4 , 17 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 349 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT_LIMIT , 155 , 148 , 42 , 12 , pParentWindow );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_NORMAL );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TITLE_LEFT , 14 , 163 , 2 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 422 , 448 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TITLE_CENTER , 16 , 163 , 69 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 432 , 448 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TITLE_RIGHT , 85 , 163 , 2 , 15 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 509 , 448 );
	pWindowStatic	=	NULL;

	pWindowEdit	=	new SPWindowEdit( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TEXT , 95 , 163 , 58 , 17 , pParentWindow );
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
	pWindowEdit->SetLimitText( 4 );
	pWindowEdit->SetPassword( true );
	pWindowEdit->SetUseIME( false );

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_CHANGE , 157 , 166 , 12 , 12 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 105 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 105 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 118 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 118 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_CHANGE_IMG , 172 , 165 , 46 , 14 , pParentWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 339 , 201 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_INFO_SETUP_OK , 42 , 188 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 73 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 91 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 109 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 127 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 109 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_TALKING_ROOM_INFO_SETUP_CANCEL , 118 , 188 , 68 , 17 , pParentWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 19 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 37 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 55 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 37 );
	pWindowButton	=	NULL;
}

INSTANCE_ID SPWindowTalkingRoom::GetActiveMenuWindow( INSTANCE_ID eInstanceStart , INSTANCE_ID eInstanceEnd , int iStep )
{
	SPWindowButton*	pWindowButton;
	unsigned int	uiInstanceID;

	for( uiInstanceID = eInstanceStart ; uiInstanceID <= eInstanceEnd ; uiInstanceID += iStep )
	{
		pWindowButton	=	(SPWindowButton*)Find( uiInstanceID , true );

		if( pWindowButton != NULL )
		{
			if( pWindowButton->IsChecked() == true && pWindowButton->IsShow() == true )
				return uiInstanceID;
		}
	}

	return 0;
}

void SPWindowTalkingRoom::SetEditWindow( INSTANCE_ID eInstance )
{
	SPWindowEdit*	pWindowAll			=	(SPWindowEdit*)( Find( WIID_TALKING_ROOM_EDIT_ALL		, true ) );
	SPWindow*		pWindowWhisper		=	Find( WIID_TALKING_ROOM_EDIT_WHISPER	, true );
	SPWindow*		pWindowWhisperImg	=	Find( WIID_TALKING_ROOM_WHISPER_IMG	, true );

	int		iPosX , iPosY , iSizeX , iSizeY , iMarginX , iMarginY;

	if( eInstance == WIID_TALKING_ROOM_SEND_ALL )
	{
		iPosX		=	6;
		iPosY		=	354;
		iSizeX		=	357;
		iSizeY		=	17;
		iMarginX	=	6;
		iMarginY	=	3;

		if( pWindowWhisper != NULL )
		{
			pWindowWhisper->Hide();
			pWindowWhisper->SetShowEnable( false );
		}

		if( pWindowWhisperImg != NULL )
		{
			pWindowWhisperImg->Hide();
			pWindowWhisperImg->SetShowEnable( false );
		}

	}
	else if( eInstance == WIID_TALKING_ROOM_SEND_WHISPER )
	{
		iPosX		=	139;
		iPosY		=	354;
		iSizeX		=	224;
		iSizeY		=	17;
		iMarginX	=	4;
		iMarginY	=	3;

		if( pWindowWhisper != NULL )
		{
			pWindowWhisper->SetShowEnable( true );
			pWindowWhisper->Show();
		}

		if( pWindowWhisperImg != NULL )
		{
			pWindowWhisperImg->SetShowEnable( true );
			pWindowWhisperImg->Show();
		}
	}

	if( pWindowAll != NULL )
	{
		pWindowAll->AdjustCoord( iPosX , iPosY , iSizeX , iSizeY );
		pWindowAll->SetMargin( iMarginX , iMarginY );
		pWindowAll->RefreshRelationCoord();
		pWindowAll->Show();
	}
}

void SPWindowTalkingRoom::AddChatString( CHAT_TOSS_DATA& stChatData , D3DXCOLOR cColor , SPWindow* pWindowList , SPWindow* pWindowSlider , SPWindow* pWindowUp , SPWindow* pWindowDown )
{
	SPWindowList*	pNewWindowList		=	(SPWindowList*)pWindowList;
	SPWindowSlider*	pNewWindowSlider	=	(SPWindowSlider*)pWindowSlider;

	if( pNewWindowList == NULL ||pNewWindowSlider == NULL )
		return;

	LIST_ITEM	stAddListItem;
	LIST_ICON	stAddListIcon;

	if( stChatData.m_eScope != SCOPE_BROADCAST	&& stChatData.m_eScope != SCOPE_AROUNDREGION	&&
		stChatData.m_eScope != SCOPE_WHISPER	&& stChatData.m_eScope != SCOPE_CHATROOM_ADVERT	&&
		stChatData.m_eScope != SCOPE_NOTICE		&&	stChatData.m_eScope != SCOPE_SYSTEMMSG			)
		return;

	if( stChatData.m_eScope == SCOPE_CHATROOM_ADVERT )
	{
		stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";

		// 대화방 타입(0:일반, 1:장터, 2:구인, 3:미팅)
		switch( stChatData.m_iCategory )
		{
		case 0:
			stAddListIcon.m_rcSrc.left		=	263;
			stAddListIcon.m_rcSrc.top		=	142;
			break;
		case 1:
			stAddListIcon.m_rcSrc.left		=	278;
			stAddListIcon.m_rcSrc.top		=	142;
			break;
		case 2:
			stAddListIcon.m_rcSrc.left		=	293;
			stAddListIcon.m_rcSrc.top		=	142;
			break;
		case 3:
			stAddListIcon.m_rcSrc.left		=	308;
			stAddListIcon.m_rcSrc.top		=	142;
			break;
		}

		stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 14;
		stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		stAddListItem.m_vIconImage.push_back( stAddListIcon );

		stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
		stAddListIcon.m_rcSrc.left		=	300;
		stAddListIcon.m_rcSrc.top		=	345;
		stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 2;
		stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		stAddListItem.m_vIconImage.push_back( stAddListIcon );

		char	szBuf[_MAX_PATH]	=	{};
		itoa( stChatData.m_iChannelNumber , szBuf , 10 );

		int istrLength	=	(int)strlen(szBuf);
		int	iNumber;
		std::string	strNumber;

		for( int i = 0 ; i < istrLength ; ++i )
		{
			strNumber	=	szBuf[ i ];
			iNumber		=	atoi( strNumber.c_str() );

			stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
			stAddListIcon.m_rcSrc.left		=	189 + iNumber * 7;
			stAddListIcon.m_rcSrc.top		=	307;
			stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 6;
			stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
			stAddListItem.m_vIconImage.push_back( stAddListIcon );
		}

		stAddListItem.m_stReturnValue.m_iReturnValue	=	stChatData.m_iChannelNumber;

		m_bNextLine	=	true;
	}

	stAddListItem.m_stTextColor	=	cColor;

	if( stChatData.m_strMsg.empty() == false )
		stAddListItem.m_strText		=	stChatData.m_strMsg;

	if( stChatData.m_bWhisper == true && stChatData.m_strID.empty() == false )
		stAddListItem.m_stReturnValue.m_strReturnValue = stChatData.m_strID;

	pNewWindowList->SetNextLine( m_bNextLine );
	m_bNextLine	=	false;

	pNewWindowList->InsertItem( &stAddListItem );
	pNewWindowList->Show();

	int	iEndLine	=	pNewWindowList->GetEndLine() + 1;

	if( iEndLine < 0 )
		iEndLine = 0;

	pNewWindowSlider->SPSendMessage( SPIM_SET_MAXVALUE , iEndLine );

	if( pNewWindowList->GetLatest() == true )
	{
		int	iCurLine	=	iEndLine - 1;

		if( iCurLine < 0 )
			iCurLine = 0;

		pNewWindowSlider->SPSendMessage( SPIM_SET_CURVALUE , iCurLine );
	}

	pNewWindowSlider->Show();

	//////////////////////////////////////////////////////////////////////////

	bool	bEnable	=	true;

	if( pNewWindowList->GetEndLine() <= 0 )
		bEnable	=	false;

	pNewWindowSlider->SetEnable( bEnable );

	if( pWindowUp != NULL )
		pWindowUp->SetEnable( bEnable );

	if( pWindowDown != NULL )
		pWindowDown->SetEnable( bEnable );
}

void SPWindowTalkingRoom::ClearPlayerAction()
{
	SPWindow*		pWindow;
	unsigned int	uiInstanceID	=	WIID_TALKING_ROOM_PLAYER_SELECT_ICON;
	unsigned int	uiInstanceIDEnd	=	WIID_TALKING_ROOM_PLAYER_SELECT_ICON	+	TALKING_ROOM_PLAYER_COUNT;

	for( uiInstanceID = WIID_TALKING_ROOM_PLAYER_SELECT_ICON ; uiInstanceID <= uiInstanceIDEnd ; uiInstanceID += 1 )
	{
		pWindow	=	Find( uiInstanceID , true );

		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_BTN_SELECT , false );
	}

	pWindow	=	Find( WIID_TALKING_ROOM_MAX_PLAYER_ACTION_PANEL , true );

	if( pWindow != NULL )
	{
		pWindow->SetShowEnable( false );
		pWindow->Hide();
	}
}

void SPWindowTalkingRoom::SetModalWindowShow( SPWindow* pWindow , bool bShow )
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

void SPWindowTalkingRoom::SetDefaultWindow( INSTANCE_ID eInstanceID )
{
	switch( eInstanceID )
	{
		case WIID_TALKING_ROOM_ADVERTISING_PANEL:		SetDefaultWindowAdvertising();			break;
		case WIID_TALKING_ROOM_MY_INFO_SETUP_PANEL:		SetDefaultWindowMyInfoSetup();			break;
		case WIID_TALKING_ROOM_INFO_SETUP_PANEL:		SetDefaultWindowRoomInfoSetup();		break;
	}
}

void SPWindowTalkingRoom::SetDefaultWindowAdvertising()
{
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	TCHAR*	pstrRoomAdvert	=	NULL;

	if( pChatManager != NULL )
	{
		pstrRoomAdvert	=	pChatManager->GetCharRoomAdvert();
	}

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_TALKING_ROOM_ADVERTISING_EDIT , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( pstrRoomAdvert );
}

void SPWindowTalkingRoom::SetDefaultWindowMyInfoSetup()
{
	if( g_pClientOption == NULL )
		return;

	SERVER_SIDE_OPTION	stSrcSvrOption	=	g_pClientOption->GetServerSideOption();

	SetButtonCheck( Find( WIID_TALKING_ROOM_MY_INFO_SETUP_MY_INFO_SEARCH	, true ) , ( ( stSrcSvrOption & SSO_REQ_CHATROOM_UNUSED			) == SSO_REQ_CHATROOM_UNUSED		) );
	SetButtonCheck( Find( WIID_TALKING_ROOM_MY_INFO_SETUP_CHAT_REQUEST		, true ) , ( ( stSrcSvrOption & SSO_REQ_CHATROOM_INVITE_DENIAL	) == SSO_REQ_CHATROOM_INVITE_DENIAL	) );
	SetButtonCheck( Find( WIID_TALKING_ROOM_MY_INFO_SETUP_MY_STYLE_OPEN		, true ) , ( ( stSrcSvrOption & SSO_REQ_CHATROOM_STYLE_SECRET	) == SSO_REQ_CHATROOM_STYLE_SECRET	) );
}

void SPWindowTalkingRoom::SetDefaultWindowRoomInfoSetup()
{
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	TCHAR*	pstrRoomSubject		=	NULL;
	int		iLimitNumber		=	0;
	int		iRequireRoomMax		=	0;
	TCHAR*	pstrRoomPasswrod	=	NULL;

	char		szBuf[ _MAX_PATH ];
	SPWindow*	pWindow;

	if( pChatManager != NULL )
	{
		CHATROOM_INFO*	pChatRoomInfo	=	pChatManager->GetChatRoomInfo();

		if( pChatRoomInfo != NULL )
		{
			pstrRoomSubject								=	pChatRoomInfo->szTitle;

			m_iCurIndexRoomModify[ ROOM_STYLE_TYPE ]	=	pChatRoomInfo->iCategory;
			m_iCurIndexRoomModify[ ROOM_STYLE_GENDER ]	=	pChatRoomInfo->iSex;
			m_iCurIndexRoomModify[ ROOM_STYLE_AGE ]		=	pChatRoomInfo->iAge;

			iLimitNumber								=	pChatRoomInfo->iLimitNum;
			iRequireRoomMax								=	pChatManager->GetRequire( CHAT_LDT_REQUIRE_ROOM_MAX );

			pstrRoomPasswrod							=	pChatRoomInfo->szPassword;
		}
	}
	else
	{
		for( int i = 0 ; i < ROOM_STYLE_MAX ; ++i )
		{
			m_iCurIndexRoomModify[ i ]	=	0;
		}
	}

	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_EDIT , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( pstrRoomSubject );

	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_GENDER	, m_iCurIndexRoomModify[ ROOM_STYLE_GENDER ]	, Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TEXT	, true ) );
	SetViewTitle( CHAT_LDT_TYPE_ROOM_STYLE , ROOM_STYLE_AGE		, m_iCurIndexRoomModify[ ROOM_STYLE_AGE ]		, Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TEXT	, true ) );

	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT , true );

	if( pWindow != NULL )
	{
		sprintf( szBuf , "%d" , iLimitNumber );
		pWindow->SetWindowText( szBuf );
	}

	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT_LIMIT , true );

	if( pWindow != NULL )
	{
		const char*		pszGlobalString;

		pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008117 );

		if( pszGlobalString != NULL )
			sprintf( szBuf , pszGlobalString , iRequireRoomMax );
		else
			sprintf( szBuf , " / %d" , iRequireRoomMax );

		pWindow->SetWindowText( szBuf );
	}

	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TEXT , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( pstrRoomPasswrod );

	SetButtonCheck( Find( WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_CHANGE	, true ) , false );

	pWindow	=	Find( WIID_TALKING_ROOM_INFO_SETUP_OK , true );

	if( pWindow != NULL )
	{
		bool	bEnable	=	false;

		if( pChatManager != NULL )
			bEnable	=	pChatManager->IsMaster();

		pWindow->SetEnable( bEnable );
	}
}

void SPWindowTalkingRoom::SetButtonCheck( SPWindow* pWindow , bool bCheck )
{
	if( pWindow == NULL )
		return;

	pWindow->SPSendMessage( SPIM_BTN_SELECT , (WPARAM)bCheck );
}

void SPWindowTalkingRoom::SetButtonCheckInv( SPWindow* pWindow )
{
	SPWindowButton*	pWindowButton	=	(SPWindowButton*)(pWindow);

	if( pWindowButton == NULL )
		return;

	SetButtonCheck( (SPWindow*)(pWindowButton) , !pWindowButton->IsChecked() );
}

void SPWindowTalkingRoom::SetViewTitle( CHAT_LDT_TYPE eChatLDTType , int iStyleID , int& iIndex , SPWindow* pWindow )
{
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager == NULL )
		return;

	int	iCount	=	(int)pChatManager->GetStyleCount( eChatLDTType , iStyleID );

	if( iIndex < 0 )
		iIndex = iCount - 1;

	if( iIndex >= iCount )
		iIndex = 0;

	STYLE_LDT*	pStyleLDT = pChatManager->GetStyle( eChatLDTType , iStyleID , iIndex );

	if( pStyleLDT != NULL && pWindow != NULL )
		pWindow->SetWindowText( pStyleLDT->m_strTitle.c_str() );
}

void SPWindowTalkingRoom::SetAutoRoomSubject( SPWindow* pWindow , int iIndex )
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

void SPWindowTalkingRoom::PrintErrorMsg( GLOBAL_STRING_ID uiGlobalStringID )
{
	DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( uiGlobalStringID );

	if( pszGlobalString == NULL )
		return;

	//if( uiGlobalStringID > FRIEND_GLOBAL_MSG_ONE_METHOD && pszName != NULL )
	//{
	//	sprintf( szErrorMsg , pszGlobalString , pszName );
	//}
	//else
	//{
	strcpy( szErrorMsg , pszGlobalString );
	//}

	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szErrorMsg );
}

void SPWindowTalkingRoom::SendPacketOut(UINT32 uiID , UINT32 uiRoomNo , UINT32 uiCharID , UINT8 iBanOut )
{
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager == NULL )
		return;

	if( pChatManager->IsChatRoom() == false )
		return;

	CHATROOM_INFO*	pChatRoomInfo	=	pChatManager->GetChatRoomInfo();

	if( pChatRoomInfo != NULL )
	{
		if( uiID <= 0 )
			uiID	=	pChatRoomInfo->uiID;

		if( uiRoomNo <= 0 )
			uiRoomNo	=	pChatRoomInfo->uiRoomNo;
	}

	UINT32	uiPacketID	=	CHATROOM_CS_OUT;

	if( uiCharID > 0 )
		uiPacketID	=	CHATROOM_CS_FORCEOUT;

	CPacket	Packet;
	Packet.AddUINT32( uiPacketID );
	Packet.AddUINT32( uiID );
	Packet.AddUINT32( uiRoomNo );

	if( uiCharID > 0 )
		Packet.AddUINT32( uiCharID );
	else
		Packet.AddUINT8( iBanOut );

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

			if( uiPacketID == CHATROOM_CS_FORCEOUT )
				m_bForceOut	=	true;
			else if( uiPacketID == CHATROOM_CS_OUT )
			{
				SPWindow*	pWindow;

				pWindow	=	Find( WIID_TALKING_ROOM_CLOSE , true );

				if( pWindow != NULL )
					pWindow->SetEnable( false );

				pWindow	=	Find( WIID_TALKING_ROOM_MAX_CLOSE , true );

				if( pWindow != NULL )
					pWindow->SetEnable( false );
			}
		}
	}
}

void SPWindowTalkingRoom::SendChangeMaster( UINT32 uiRoomID , UINT32 uiRoomNo , UINT32 uiTargetCharID )
{
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager == NULL )
		return;

	if( pChatManager->IsChatRoom() == false )
		return;

	CHATROOM_INFO*	pChatRoomInfo	=	pChatManager->GetChatRoomInfo();

	if( pChatRoomInfo != NULL )
	{
		if( uiRoomID <= 0 )
			uiRoomID	=	pChatRoomInfo->uiID;

		if( uiRoomNo <= 0 )
			uiRoomNo	=	pChatRoomInfo->uiRoomNo;
	}

	CPacket	Packet;
	Packet.AddUINT32( CHATROOM_CS_CHANGEMASTER );
	Packet.AddUINT32( uiTargetCharID );
	Packet.AddUINT32( uiRoomID );
	Packet.AddUINT32( uiRoomNo );

	if( pChatManager != NULL )
	{
		if( pChatManager->IsSendPacket() == true )
		{
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

			m_bMasterChange	=	true;
		}
	}
}

void SPWindowTalkingRoom::UpdateUserList()
{
	ClearPlayerAction();

	ROOM_MEMBER_UI*	pRoomUser	=	NULL;

	int	iUserCount	=	(int)m_vRoomUserList.size();
	int	iStartPos	=	m_iRoomUserCurPage * TALKING_ROOM_PLAYER_COUNT;
	int	iNewPos;
	FONT_ENUMERATE	eFontType;

	for( int i = 0 ; i < TALKING_ROOM_PLAYER_COUNT ; ++i )
	{
		iNewPos	=	i + iStartPos;

		if( iUserCount > iNewPos )
			pRoomUser	=	&( m_vRoomUserList.at( iNewPos ) );
		else
			pRoomUser	=	NULL;

		if( iNewPos == 0 )
			eFontType	=	FONT_12_BOLD;
		else
			eFontType	=	FONT_12_NORMAL;

		SetUserList( i , pRoomUser , eFontType );
	}
}

void SPWindowTalkingRoom::SetUserList( int iIndex , ROOM_MEMBER_UI* pRoomUser , FONT_ENUMERATE eFontType )
{
	if( iIndex < 0 || iIndex > TALKING_ROOM_PLAYER_COUNT )
		return;

	int				iClassType	=	0;
	TCHAR*			pstrName	=	NULL;
	bool			bEnable		=	false;
	D3DXCOLOR		cTextColor	=	m_cUserListTextColor[ USER_LIST_COLOR_DEFAULT ];

	if( pRoomUser != NULL )
	{
		iClassType	=	(int)pRoomUser->m_stRoomMember.eClassType;
		pstrName	=	pRoomUser->m_stRoomMember.szCharName;
		cTextColor	=	m_cUserListTextColor[ pRoomUser->m_eUserListColor ];

		if( pRoomUser->m_eUserListColor != USER_LIST_COLOR_ME )
			bEnable	=	true;
	}

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_TALKING_ROOM_PLAYER_SELECT_ICON + iIndex , true );

	if( pWindow != NULL )
		pWindow->SetEnable( bEnable );

	pWindow	=	Find( WIID_TALKING_ROOM_PLAYER_CLASS_ICON + iIndex , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iClassType );
		 
	pWindow	=	Find( WIID_TALKING_ROOM_PLAYER_NAME_TEXT + iIndex , true );

	if( pWindow != NULL )
	{
		pWindow->SetFontColor( cTextColor );
		pWindow->SetWindowText( pstrName );
		pWindow->SetFont( eFontType );
	}
}

void SPWindowTalkingRoom::UpdateUserListPage( int iCurPage )
{
	int	iMaxPage		=	(int)m_vRoomUserList.size() / TALKING_ROOM_PLAYER_COUNT;
	int	iRemainder		=	(int)m_vRoomUserList.size() % TALKING_ROOM_PLAYER_COUNT;

	if( iRemainder > 0 )
		++iMaxPage;

	if( iCurPage >= iMaxPage )
		iCurPage	=	iMaxPage - 1;

	if( iCurPage <= 0 )
		iCurPage	=	0;

	m_iRoomUserCurPage		=	iCurPage;
	int	iRoomUserCurPage	=	m_iRoomUserCurPage + 1;

	bool	bLeftButtonEnable	=	true;
	bool	bRightButtonEnable	=	true;

	if( iRoomUserCurPage <= 1 )
		bLeftButtonEnable	=	false;

	if( iRoomUserCurPage >= iMaxPage )
		bRightButtonEnable	=	false;

	char	szBuf[ _MAX_PATH ];

	sprintf( szBuf , "%d / %d" , iRoomUserCurPage , iMaxPage );

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_TALKING_ROOM_MAX_PLAYER_PAGE_TEXT , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( szBuf );

	pWindow	=	Find( WIID_TALKING_ROOM_MAX_PLAYER_PAGE_LEFT , true );

	if( pWindow != NULL )
		pWindow->SetEnable( bLeftButtonEnable );

	pWindow	=	Find( WIID_TALKING_ROOM_MAX_PLAYER_PAGE_RIGHT , true );

	if( pWindow != NULL )
		pWindow->SetEnable( bRightButtonEnable );
}

void SPWindowTalkingRoom::UpdateUserCount()
{
	int	iUserCount	=	(int)m_vRoomUserList.size();
	int	iMaxCount	=	0;

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		CHATROOM_INFO*	pChatRoomInfo	=	pChatManager->GetChatRoomInfo();

		if( pChatRoomInfo != NULL )
			iMaxCount	=	pChatRoomInfo->iLimitNum;
	}

	char		szBuf[ _MAX_PATH ];
	SPWindow*	pWindow	=	Find( WIID_TALKING_ROOM_MAX_JOIN_PERSON_TEXT , true );

	if( pWindow != NULL )
	{
		const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008133 );

		if( pszGlobalString != NULL )
			sprintf( szBuf , pszGlobalString , iUserCount , iMaxCount );
		else
			sprintf( szBuf , "%d/%d" , iUserCount , iMaxCount );

		pWindow->SetWindowText( szBuf );
	}
}

void SPWindowTalkingRoom::UpdateSubject()
{
	CHATROOM_INFO*	pChatRoomInfo	=	NULL;
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		pChatRoomInfo	=	pChatManager->GetChatRoomInfo();

	SPWindow*	pWindow;
	std::string	strBuf;
	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008133 );

	//////////////////////////////////////////////////////////////////////////

	pWindow	=	Find( WIID_TALKING_ROOM_TYPE_ICON , true );

	if( pWindow != NULL )
	{
		int	iIndex	=	0;

		if( pChatRoomInfo != NULL )
			iIndex	=	pChatRoomInfo->iCategory	+	1;

		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iIndex );
	}

	//////////////////////////////////////////////////////////////////////////

	char	szBuf[_MAX_PATH]	=	{};
	itoa( (int)pChatRoomInfo->uiRoomNo , szBuf , 10 );

	int istrLength	=	(int)strlen( szBuf );
	int	iNumber;
	std::string	strNumber;

	for( int i = 0 ; i < istrLength ; ++i )
	{
		strNumber	=	szBuf[ i ];
		iNumber		=	atoi( strNumber.c_str() );

		pWindow	=	Find( WIID_TALKING_ROOM_NUMBER_100 + i , true );

		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iNumber );
	}

	for( ; i < 3 ; ++i )
	{
		pWindow	=	Find( WIID_TALKING_ROOM_NUMBER_100 + i , true );

		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 10 );
	}

	//////////////////////////////////////////////////////////////////////////

	pWindow	=	Find( WIID_TALKING_ROOM_TYPE_TEXT , true );

	if( pWindow != NULL )
	{
		strBuf.clear();

		if( pChatRoomInfo != NULL )
		{
			const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( 5008129 + pChatRoomInfo->iCategory );

			if( pszGlobalString != NULL )
				strBuf	=	pszGlobalString;
		}

		pWindow->SetWindowText( strBuf.c_str() );
	}

	//////////////////////////////////////////////////////////////////////////

	pWindow	=	Find( WIID_TALKING_ROOM_SUBJECT_TEXT , true );
	
	if( pWindow != NULL )
	{
		strBuf.clear();

		if( pChatRoomInfo != NULL )
			strBuf	=	pChatRoomInfo->szTitle;

		pWindow->SetWindowText( strBuf.c_str() );
	}
}

void SPWindowTalkingRoom::AddRommUser( ROOM_MEMBER_UI& stRoomUser , bool bInsertFirst )
{
	if( bInsertFirst == true )
		m_vRoomUserList.insert( m_vRoomUserList.begin() , stRoomUser );
	else
		m_vRoomUserList.push_back( stRoomUser );
}

bool SPWindowTalkingRoom::DeleteRoomUser( ROOM_MEMBER_UI& stRoomUser )
{
	STD_VECTOR_USER::iterator	vIter			=	m_vRoomUserList.begin();
	STD_VECTOR_USER::iterator	vEndIter		=	m_vRoomUserList.end();
	ROOM_MEMBER_UI*				pRoomUser		=	NULL;
	std::string					strRoomUserName	=	stRoomUser.m_stRoomMember.szCharName;

	while( vIter != vEndIter )
	{
		pRoomUser	=	&(*vIter);

		if( pRoomUser != NULL )
		{
			if( pRoomUser->m_stRoomMember.uiCharID == stRoomUser.m_stRoomMember.uiCharID && strRoomUserName.compare( pRoomUser->m_stRoomMember.szCharName ) == 0 && pRoomUser->m_stRoomMember.eClassType == stRoomUser.m_stRoomMember.eClassType )
			{
				m_vRoomUserList.erase( vIter );
				return true;
			}
		}

		++vIter;
	}

	return false;
}

void SPWindowTalkingRoom::ChangeLeaderRoomUser( ROOM_MEMBER_UI& stRoomUser )
{
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		pChatManager->SetMasterDBKey( stRoomUser.m_stRoomMember.uiCharID );
}

bool SPWindowTalkingRoom::SetWhisperMode( const char* pstrID )
{
	if( pstrID == NULL )
		return false;

	std::string	strID	=	pstrID;

	if( strID.empty() == true )
		return false;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		char	szLocalName[ LEN_NAME + 1 ];

		pLocalPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szLocalName );

		if( strID.compare( szLocalName ) == 0 )
			return false;
	}

	OnSend( WIID_TALKING_ROOM_SEND_WHISPER , 1 , NULL );

	SPWindow*	pWindowWhisper	=	Find( WIID_TALKING_ROOM_EDIT_WHISPER , true );

	if( pWindowWhisper != NULL )
		pWindowWhisper->SetWindowText( strID.c_str() );

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SetFocusWindow( Find( WIID_TALKING_ROOM_EDIT_ALL , true ) );

	return true;
}

void SPWindowTalkingRoom::ProcessBan( float fTime )
{
	m_fBanAccmulateTime	+=	fTime;

	if( m_fBanAccmulateTime >= m_fBanLimitTime )
	{
		ResetBan();
		OnNoticeYes( NULL , NULL );
	}
}

void SPWindowTalkingRoom::ResetBan()
{
	m_fBanAccmulateTime	=	0.0f;
}
