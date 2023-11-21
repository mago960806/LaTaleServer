// Copyright (C) liuyang
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : liuyang 2008-8-28
//***************************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowSlider.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
//#include "SPWindowList.h"
#include "SPWindowFriendSnatchUser.h"
#include "SPWindowChat.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include <algorithm>
#include <sstream>
#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "SPGOBStatus.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPResourceManager.h"
#include "SPPvpManager.h"

#include "SPUtil.h"
#include "SPDebug.h"
#include "LT_Error.h"

#include "SPWindowFriendSnatch.h"
#include "SPPlayerInvenArchive.h"
#include "SPWindowEditMultiLine.h"
#include "SPWindowWorldMap.h"
#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTimer.h"

SPWindowFriendSnatch::SPWindowFriendSnatch(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

//------------------------------------------------------------------------------------------------------
SPWindowFriendSnatch::SPWindowFriendSnatch(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_FRIEND_SNATCH, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

//------------------------------------------------------------------------------------------------------
SPWindowFriendSnatch::~SPWindowFriendSnatch()
{
	Clean();
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::Init()
{
	m_strToolTip		=	"[SPWindowFriendSnatch]";
	m_eFriendType		=	FRIEND_SNATCH_TYPE_WHITE;
	m_bDeletePacketSend	=	false;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , &m_pBGTexture );

	ReposOwnImage(); 

	SetRect( &m_rcBGSrc[0]	, 1 , 31 , 233 , 35 );		
	SetRect( &m_rcBGSrc[1]	, 1 , 37 , 233 , 39 );			
	SetRect( &m_rcBGSrc[2]	, 1 , 41 , 233 , 45 );			
	SetRect( &m_rcBGSrc[3]	, 349 , 312 , 355 , 318 );		
	SetRect( &m_rcBGSrc[4]	, 349 , 329 , 355 , 330 );		
	SetRect( &m_rcBGSrc[5]	, 349 , 329 , 355 , 330 );		
	SetRect( &m_rcBGSrc[6]	, 349 , 321 , 355 , 327 );		

	SPWindowStatic*	pWindowStatic;

	pWindowStatic	=	new SPWindowStatic( WIID_FRIEND_SNATCH_TITLE_IMG , 8 , 7 , 32 , 23 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 72 , 155 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_FRIEND_SNATCH_TITLE_TEXT , 42 , 7 , 108 , 17 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 110 , 155 );
	pWindowStatic	=	NULL;

	SPWindowButton*	pWindowButton;

	pWindowButton	=	new	SPWindowButton( WIID_FRIEND_SNATCH_CLOSE , 211 , 9 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 217 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 230 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 243 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 256 );
	pWindowButton	=	NULL;

	m_pWhiteButton	=	new	SPWindowButton( WIID_FRIEND_SNATCH_WHITE_LIST_BUTTON , 10 , 55 , 64 , 15 , this );
	m_pWhiteButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 384 );
	m_pWhiteButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 400 );
	m_pWhiteButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 416 );
	m_pWhiteButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 432 );
	m_pWhiteButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 416 );

	pWindowButton	=	new	SPWindowButton( WIID_FRIEND_SNATCH_SLIDER_UP , 210 , 85 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_FRIEND_SNATCH_SLIDER_DOWN , 210 , 246 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );
	pWindowButton =	NULL;

	RECT rtRect;
	m_pSlider	=	new	SPWindowSlider( WIID_FRIEND_SNATCH_SLIDER , 210 , 96 , 12 , 150 , this );

	SetRect( &rtRect , 460 , 470 , 472 , 494 );	
	m_pSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 473 , 470 , 485 , 494 );	
	m_pSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 486 , 470 , 498 , 494 );	
	m_pSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 499 , 470 , 511 , 494 );
	m_pSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 460 , 496 , 472 , 498 );	
	m_pSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 473 , 496 , 485 , 498 );	
	m_pSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 486 , 496 , 498 , 498 );	
	m_pSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 499 , 496 , 511 , 498 );
	m_pSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );


	m_pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pSlider->SetCurpage( 0 );
	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );

	m_pEdit	=	new SPWindowEdit( WIID_FRIEND_SNATCH_EDIT , 10 , 34 , 140 , 17 , this );
	m_pEdit->SetWindowType( WND_TYPE_LEFTBASE );
	m_pEdit->SetMargin( 2, 1 );
	m_pEdit->SetImageNormalHead	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 347 , 293 , 349 , 310 );
	m_pEdit->SetImageNormalBody	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 351 , 293 , 353 , 310 );
	m_pEdit->SetImageNormalTail	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 355 , 293 , 357 , 310 );
	m_pEdit->SetImageFocusHead	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 347 , 275 , 349 , 292 );
	m_pEdit->SetImageFocusBody	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 351 , 275 , 353 , 292 );
	m_pEdit->SetImageFocusTail	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 355 , 275 , 357 , 292 );
	m_pEdit->SetImageDeco		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 347 , 264 , 357 , 274 );
	m_pEdit->SetFontColor( D3DXCOLOR( 0.0f , 0.0f , 0.0f , 1.0f ) );
	m_pEdit->SetLimitText( LEN_NAME );
	m_pEdit->SetEnable(false);

	InitWhite();
	//add
	m_iSendFriendPos	=	0;

}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::ReposOwnImage()
{
	SetRect( &m_rcBGDest[0]	, m_iAX					, m_iAY					, m_iAX + 232			, m_iAY + 4 );						
	SetRect( &m_rcBGDest[1]	, m_rcBGDest[0].left	, m_rcBGDest[0].bottom	, m_rcBGDest[0].right	, m_rcBGDest[0].bottom + 287 );		
	SetRect( &m_rcBGDest[2]	, m_rcBGDest[1].left	, m_rcBGDest[1].bottom	, m_rcBGDest[1].right	, m_rcBGDest[1].bottom + 4 );		
	SetRect( &m_rcBGDest[3]	, m_iAX + 2				, m_iAY + 26			, m_iAX + 2 + 228		, m_iAY + 26	+ 48 );				
	SetRect( &m_rcBGDest[4]	, m_iAX + 2				, m_iAY + 74			, m_iAX + 2 + 228		, m_iAY + 74	+ 1 );				
	SetRect( &m_rcBGDest[5]	, m_iAX + 2				, m_iAY + 267			, m_iAX + 2 + 228		, m_iAY + 267	+ 1 );				
	SetRect( &m_rcBGDest[6]	, m_iAX + 2				, m_iAY + 75			, m_iAX + 2 + 228		, m_iAY + 75	+ 192 );			
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );

	DelAllWhiteList();
	//add
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::Show(bool bWithChild)
{
	if(m_bShow)
	{
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	}

	SPWindow::Show(bWithChild);

	HideWhiteList( 0 );
	//add
	if( m_eFriendType == FRIEND_SNATCH_TYPE_WHITE )
	{
		OnWhiteList( 1 , NULL );

		int	iWhiteCount	=	(int)m_mpWhiteListOnline.size() + (int)m_mpWhiteListOffline.size();

		if( iWhiteCount > 0 )
		{
			m_fRefreshAccmulateTime	=	0.0f;
			RequestStatusFriend();
		}
		else
		{
			CPacket	FriendPacket;
			FriendPacket.AddUINT32( FRIEND_CS_GETLIST );
			( g_pNetworkManager->GetPacketHandler() )->PostMsg( &FriendPacket );

			m_fRefreshAccmulateTime	=	-1.0f;
		}

	}
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::Hide(bool bSendServer)
{
	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	if(g_pGOBManager->GetLocalPlayer()) 
	{

		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();

		if(pInven) 
		{
			pInven->ClearSnatchItem();
		}
	}

	SPWindow::Hide(bSendServer);
	//add

	if( m_eFriendType == FRIEND_SNATCH_TYPE_WHITE )	//	WhiteList
	{
		OnWhiteLButtonUp( 0 , NULL , NULL );
	}

	OnEditKillFocus( NULL , NULL );

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		SPWindow*	pWindowToolTip	=	g_pInterfaceManager->GetToolTipWindow();

		if( pWindowToolTip != NULL )
			pWindowToolTip->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , -2 );
	}
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::Process(float fTime)
{
	SPWindow::Process( fTime );
	if( m_bShow == false || m_eFriendType != FRIEND_SNATCH_TYPE_WHITE ||
		m_fRefreshLimit == 0.0f || m_fRefreshAccmulateTime < 0.0f )
		return;

	m_fRefreshAccmulateTime	+=	fTime;

	if( m_fRefreshAccmulateTime >= m_fRefreshLimit )
		RequestStatusFriend();
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < FRIEND_SNATCH_BG_COUNT ; i++ )
		{
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	SPWindow::Render(fTime);
}

//------------------------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowFriendSnatch )
SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER		,										OnCursorEnter		)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT			,										OnCursorOut			)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE		,										OnCursorMove		)

SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP			,										OnLButtonUp			)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN		,										OnLButtonDown		)
SPIMESSAGE_COMMAND(	SPIM_RBUTTON_UP			,										OnRButtonUp			)

SPIMESSAGE_COMMAND(	SPIM_KEY_RETURN			,										OnKeyReturn			)
SPIMESSAGE_COMMAND(	SPIM_PURGE				,										OnPurge				)

SPIMESSAGE_COMMAND( SPIM_GET_FRIEND			,										OnGetFriend			)
SPIMESSAGE_COMMAND( SPIM_ADD_FRIEND			,										OnAddFriend			)
SPIMESSAGE_COMMAND( SPIM_ACCEPT_FRIEND		,										OnAcceptFriend		)
SPIMESSAGE_COMMAND( SPIM_DEL_FRIEND			,										OnDelFriend			)
SPIMESSAGE_COMMAND( SPIM_REFRESH_STATUS_FRIEND	,									OnStatusFriend		)
SPIMESSAGE_COMMAND( SPIM_FRIEND_NAME		,										OnDelFriendName		)

SPIMESSAGE_COMMAND( SPIM_NOTICE_YES			,										OnNoticeYes			)
SPIMESSAGE_COMMAND( SPIM_NOTICE_NO			,										OnNoticeNo			)

SPIMESSAGE_COMMAND( SPIM_WHEEL_UP			,										OnSliderUp			)
SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN			,										OnSliderDown		)

SPIMESSAGE_COMMAND( SPIM_SNATCH_FAIL		,										OnSnatchResult		)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_SNATCH_CLOSE				,	OnClose				)


SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_SNATCH_SLIDER_UP			,	OnSliderUp			)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_SNATCH_SLIDER_DOWN			,	OnSliderDown		)

SPIMESSAGE_CONTROL( SPIM_SLIDER_UP			,	WIID_FRIEND_SNATCH_SLIDER				,	OnSlider			)
SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN		,	WIID_FRIEND_SNATCH_SLIDER				,	OnSlider			)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP		,	WIID_FRIEND_SNATCH_SLIDER				,	OnSlider			)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN	,	WIID_FRIEND_SNATCH_SLIDER				,	OnSlider			)
SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE		,	WIID_FRIEND_SNATCH_SLIDER				,	OnSlider			)

SPIMESSAGE_CONTROL(	SPIM_WHEEL_UP			,	WIID_FRIEND_SNATCH_WHITE_LIST_PANEL	,	OnSliderUp			)
SPIMESSAGE_CONTROL(	SPIM_WHEEL_DOWN			,	WIID_FRIEND_SNATCH_WHITE_LIST_PANEL	,	OnSliderDown		)

SPIMESSAGE_CONTROL(	SPIM_SETFOCUS			,	WIID_FRIEND_SNATCH_EDIT				,	OnEditSetFocus		)
SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS			,	WIID_FRIEND_SNATCH_EDIT				,	OnEditKillFocus		)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_SNATCH_WHITE_LIST_FIND			,	OnWhitePeopleFind		)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	,		WIID_FRIEND_SNATCH_WHITE_LIST_USER_WINDOW	,	WIID_FRIEND_SNATCH_WHITE_LIST_USER_WINDOW + WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER * WIID_FRIEND_SNATCH_WHITE_LIST_USER_STEP	,	OnWhiteLButtonUp	)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DBLCLICK	,	WIID_FRIEND_SNATCH_WHITE_LIST_USER_WINDOW	,	WIID_FRIEND_SNATCH_WHITE_LIST_USER_WINDOW + WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER * WIID_FRIEND_SNATCH_WHITE_LIST_USER_STEP	,	OnWhiteLButtonDoubleUp	)

SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP		,	WIID_FRIEND_SNATCH_WHITE_LIST_USER_WINDOW	,	WIID_FRIEND_SNATCH_WHITE_LIST_USER_WINDOW + WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER * WIID_FRIEND_SNATCH_WHITE_LIST_USER_STEP	,	OnWhiteSliderUp		)
SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN	,	WIID_FRIEND_SNATCH_WHITE_LIST_USER_WINDOW	,	WIID_FRIEND_SNATCH_WHITE_LIST_USER_WINDOW + WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER * WIID_FRIEND_SNATCH_WHITE_LIST_USER_STEP	,	OnWhiteSliderDown	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnCursorEnter( WPARAM wParam, LPARAM lParam )
{
	if( m_eFriendType == FRIEND_SNATCH_TYPE_WHITE )
	{
	}

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnLButtonUp( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnLButtonDown( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnRButtonUp( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnKeyReturn( WPARAM wParam, LPARAM lParam )//暂时去掉此功能
{
	//if( m_pEdit->IsFocus() == TRUE )
	//{
	//	OnUserFindFriend( (WPARAM)m_eFriendType , NULL );

	//	return 1;
	//}

	return -1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnPurge( WPARAM wParam, LPARAM lParam )
{
	m_eFriendType	=	FRIEND_SNATCH_TYPE_WHITE;

	DelAllWhiteList();
	//add
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnGetFriend( WPARAM wParam, LPARAM lParam )
{
	FRIEND_SNATCH_TYPE		eFriendType	=	(FRIEND_SNATCH_TYPE)wParam;
	CPacket		pPacketTemp		=	*(CPacket*)lParam;
	CPacket*		pPacket		=	&pPacketTemp;    
	UINT8			uiCount		=	0;
	FRIEND_INFO*	pFriendInfo	=	NULL;
	DECLARE_VAR( TCHAR , szBlockName , MAX_LEN_NAME );

	if( eFriendType == FRIEND_SNATCH_TYPE_WHITE )
	{
		DelAllWhiteList();
	}
	//add
	pPacket->ExtractUINT8( &uiCount );

	for( UINT8 i = 0 ; i < uiCount ; ++i )
	{
		switch( eFriendType )
		{
		case FRIEND_SNATCH_TYPE_WHITE:
			pFriendInfo	=	new FRIEND_INFO;
			pPacket->ExtractStruct( pFriendInfo , sizeof(FRIEND_INFO) );
			AddWhiteList( pFriendInfo );
			break;
		}
	}

	if( m_eFriendType == FRIEND_SNATCH_TYPE_WHITE )
	{
		UpdateWhiteList( m_iWhitePage );
		UINT8	uiRefreshPeriodSec	=	0;

		pPacket->ExtractUINT8( &uiRefreshPeriodSec );

		if( uiRefreshPeriodSec < 0 || uiRefreshPeriodSec > 255 )
			uiRefreshPeriodSec = 0;

		m_fRefreshLimit			=	(float)uiRefreshPeriodSec;

		if( uiCount > 0 )
		{
			m_fRefreshAccmulateTime	=	0.0f;

			RequestStatusFriend();
		}
	}
	
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnAddFriend( WPARAM wParam, LPARAM lParam )
{
	//add
	FRIEND_SNATCH_TYPE			eFriendType			=	(FRIEND_SNATCH_TYPE)wParam;
	CPacket		pPacketTemp		=	*(CPacket*)lParam;
	CPacket*		pPacket		=	&pPacketTemp;  
	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	if( pPacket == NULL || eFriendType == FRIEND_SNATCH_TYPE_NULL )
		return 1;

	pPacket->Extract( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		//PrintErrorMsg( uiGlobalStringID , m_strLastAddName.c_str() );
		return 1;
	}

	if( eFriendType == FRIEND_SNATCH_TYPE_WHITE )
	{
		if( 0 )
		{
			FRIEND_INFO*		pFriendInfo			=	new	FRIEND_INFO;

			pPacket->ExtractStruct( pFriendInfo , sizeof(FRIEND_INFO) );

			//			PrintErrorMsg( GS_FRIEND_WHITE_ADD , pFriendInfo->szCharName );
			AddWhiteList( pFriendInfo );
			UpdateWhiteList( m_iWhitePage );
		}
		else
		{
			UINT8				uiAccepterLen				=	0;
			TCHAR				szAccepter [LEN_NAME+1]		=	{0,};

			pPacket->ExtractUINT8( &uiAccepterLen );

			if( uiAccepterLen > 0 )
				pPacket->ExtractData( szAccepter , uiAccepterLen );

			//			PrintErrorMsg( GS_FRIEND_WHITE_ADD , szAccepter );
		}
	}
	
	m_strLastAddName.clear();

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnAcceptFriend( WPARAM wParam, LPARAM lParam )
{
	//add
	FRIEND_SNATCH_TYPE			eFriendType			=	(FRIEND_SNATCH_TYPE)wParam;
	CPacket		pPacketTemp		=	*(CPacket*)lParam;
	CPacket*		pPacket		=	&pPacketTemp;  
	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	if( pPacket == NULL || eFriendType == FRIEND_SNATCH_TYPE_NULL )
		return 1;

	pPacket->Extract( &uiGlobalStringID );

	FRIEND_INFO*		pFriendInfo			=	new	FRIEND_INFO;

	pPacket->ExtractStruct( pFriendInfo , sizeof(FRIEND_INFO) );

	if( uiGlobalStringID != 0 )
	{
		//		PrintErrorMsg( uiGlobalStringID , pFriendInfo->szCharName );
		delete pFriendInfo;
		return 1;
	}

	if( eFriendType == FRIEND_SNATCH_TYPE_WHITE )
	{
		//		PrintErrorMsg( GS_FRIEND_WHITE_ACCEPT_YES , pFriendInfo->szCharName );
		AddWhiteList( pFriendInfo );
		UpdateWhiteList( m_iWhitePage );
		m_fRefreshAccmulateTime	=	0.0f;
	}

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnDelFriend( WPARAM wParam, LPARAM lParam )
{
	FRIEND_SNATCH_TYPE			eFriendType			=	(FRIEND_SNATCH_TYPE)wParam;
	CPacket		pPacketTemp		=	*(CPacket*)lParam;
	CPacket*		pPacket		=	&pPacketTemp;  
	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	m_bDeletePacketSend						=	false;

	pPacket->Extract( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		//PrintErrorMsg( uiGlobalStringID , m_strLastDelName.c_str() );
		return 1;
	}
	if( eFriendType == FRIEND_SNATCH_TYPE_WHITE )	//	WhiteList
	{
		//PrintErrorMsg( GS_FRIEND_WHITE_DEL , m_strLastDelName.c_str() );
		DelWhiteList( m_strLastDelName.c_str() );
		UpdateWhiteList( m_iWhitePage );
		OnWhiteLButtonUp( 0 , NULL , NULL );

		int	iWhiteCount	=	(int)m_mpWhiteListOnline.size() + (int)m_mpWhiteListOffline.size();

		if( iWhiteCount <= 0 )
			m_fRefreshAccmulateTime	=	-1.0f;
	}
	
	m_strLastDelName.clear();

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnStatusFriend( WPARAM wParam, LPARAM lParam )
{
	CPacket		pPacketTemp		=	*(CPacket*)lParam;
	CPacket*		pPacket		=	&pPacketTemp;  

	if( pPacket == NULL )
		return 0;

	UINT8	uiRefreshPeriodSec	=	0;

	pPacket->ExtractUINT8( &uiRefreshPeriodSec );

	UINT8	uiCount	=	0;

	pPacket->ExtractUINT8( &uiCount );

	FRIEND_INFO*	pFriendInfo	=	NULL;

	for( int i = 0 ; i < uiCount ; i++ )
	{
		pFriendInfo	=	new	FRIEND_INFO;
		pPacket->ExtractStruct( pFriendInfo , sizeof(FRIEND_INFO) );
		AddWhiteList( pFriendInfo );
	}

	UpdateWhiteList( m_iWhitePage );
	m_fRefreshAccmulateTime	=	0.0f;
	m_fRefreshLimit			=	(float)uiRefreshPeriodSec;

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnNoticeYes( WPARAM wParam, LPARAM lParam )
{
	StartToSnatch();
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnNoticeNo( WPARAM wParam, LPARAM lParam )
{
	int	iForceHide	=	(int)lParam;

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

//-------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnSliderUp( WPARAM wParam, LPARAM lParam )
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnSliderDown( WPARAM wParam, LPARAM lParam )
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnSlider( WPARAM wParam, LPARAM lParam )
{
	int	iNewPage	=	(int)wParam;

	if( m_eFriendType == FRIEND_SNATCH_TYPE_WHITE )
	{
		m_iWhiteListSelectNumber	=	-1;
		UpdateWhiteList( iNewPage );
		OnWhiteLButtonUp( 0 , NULL , NULL );
	}
	
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnWhiteLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	unsigned int	uiInstanceID;
	SPWindow*		pWindow;

	m_iWhiteListSelectNumber	=	-1;

	for( int i = 0 ; i < WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER ; ++i )
	{
		uiInstanceID = WIID_FRIEND_SNATCH_WHITE_LIST_USER_WINDOW + ( i * WIID_FRIEND_SNATCH_WHITE_LIST_USER_STEP );

		if( uiInstanceID == iID )
		{
			m_iWhiteListSelectNumber	=	i;
			continue;
		}

		pWindow	=	Find( uiInstanceID , true );

		if( pWindow == NULL )
			continue;

		pWindow->SPSendMessage( SPIM_CURSOR_OUT , 1 );
	}

	return 1;
}

//双击输入名字
//-----------------------------------------------------------------------------------------------
int	SPWindowFriendSnatch::OnWhiteLButtonDoubleUp( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	const char*	pszName	=	(const char*)wParam;


	if( pszName != NULL )
	{
		m_pEdit->SetWindowText( pszName );
	}
	pszName	=	GetSelectFriendUserName( FRIEND_SNATCH_TYPE_WHITE );

	if( pszName != NULL )
	{
		m_pEdit->SetWindowText( pszName );
	}
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnWhiteSliderUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnSliderUp( wParam , lParam );
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnWhiteSliderDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnSliderDown( wParam , lParam );
	return 1;
}

//-------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnWhitePeopleFind( WPARAM wParam, LPARAM lParam )
{
	std::string strName = m_pEdit->GetWindowText();
	std::string strTest;
	GetUpperString3( strTest , m_pEdit->GetWindowText() );

	if( strName.empty() )
	{																	//~~查找名字不能为空
		return 1;
	}
	int iNameLen = static_cast<int>(strName.size());

	if( iNameLen <= 0 || iNameLen > LEN_NAME )
	{
		return 1;
	}

	m_SnatchName = strName;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	//显示谁正在查找目标玩家
	if( pLocalPlayer != NULL )
	{
		char	szLocalName[ LEN_NAME + 1 ];
		std::string	strLocalName;

		pLocalPlayer->SPGOBPerformMessage( SPGM_GETGOBNAME, (LPARAM)szLocalName );

		strLocalName.clear();
		GetUpperString3( strLocalName , szLocalName );


		if( strTest.compare( strLocalName ) == 0 )
		{
			std::string strMsg = "不能自己召唤自己";				//~
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() ); //~~
			return 1;
		}
	}

	g_pInterfaceManager->SetNotice( "确定要召唤该玩家吗？" , this );
	return 1;
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::InitWhite()
{
	m_pWhiteListPanel	=	new SPWindowStatic( WIID_FRIEND_SNATCH_WHITE_LIST_PANEL , 0 , 0 , m_iSX , m_iSY , this );

	SPWindowButton*		pButton;

	pButton	=	new	SPWindowButton( WIID_FRIEND_SNATCH_WHITE_LIST_FIND , 154 , 34 , 68 , 17 , m_pWhiteListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 374 , 1 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 374 , 19 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 374 , 37 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 374 , 55 );
	pButton	=	NULL;

	for( int i = 0 ; i < WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER ; i++ )
	{
		m_pWhiteUser[ i ]	=	new SPWindowFriendSnatchUser( WIID_FRIEND_SNATCH_WHITE_LIST_USER_WINDOW + ( i * WIID_FRIEND_SNATCH_WHITE_LIST_USER_STEP ) , 10 , i * 27 + 85 , 192 , 23 , m_pWhiteListPanel );
		m_pWhiteUser[ i ]->SetIconImagePos( 8 , 0 );
		m_pWhiteUser[ i ]->SetIconImageSize( 40 , 21 );
		m_pWhiteUser[ i ]->SetIconImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 297 , 166 );	//	OnLine
		m_pWhiteUser[ i ]->SetIconImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 338 , 166 );	//	OffLine
		m_pWhiteUser[ i ]->SetTextPos( 59 , 6 );
		m_pWhiteUser[ i ]->SetTextSize( 120 , 12 );
		m_pWhiteUser[ i ]->SetTextColorOnLine( D3DCOLOR_ARGB( 255 , 70 , 140 , 124 ) );
		m_pWhiteUser[ i ]->SetTextColorOffLine( D3DCOLOR_ARGB( 255 , 189 , 189 , 189 ) );
		m_pWhiteUser[ i ]->ShowToolTip( true );
		m_pWhiteUser[ i ]->Hide();
	}

	m_iWhitePage				=	0;
	m_fRefreshLimit				=	1.0f;
	m_fRefreshAccmulateTime		=	0.0f;
	m_iWhiteListSelectNumber	=	-1;
}

//-------------------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::AddWhiteList( FRIEND_INFO* pFriendInfo )
{
	if( pFriendInfo == NULL )
		return;

	std::string	strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , pFriendInfo->szCharName );

	DelWhiteList( strFriendName.c_str() );

	if( pFriendInfo->uiServerID == 0 )		
		m_mpWhiteListOffline.insert( STD_MAP_FRIEND_SNATCH::value_type( strFriendName , pFriendInfo ) );
	else									
		m_mpWhiteListOnline.insert( STD_MAP_FRIEND_SNATCH::value_type( strFriendName , pFriendInfo ) );


	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		FRIEND_INFO*	pPlayerFriendInfo	=	new	FRIEND_INFO;

		memcpy( pPlayerFriendInfo , pFriendInfo , sizeof( FRIEND_INFO )  );

		pLocalPlayer->AddFriendList( pPlayerFriendInfo );
	}
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::DelWhiteList( const char* pFriendName )
{
	std::string					strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , pFriendName );

	STD_MAP_FRIEND_SNATCH::iterator	mIterOffline	=	m_mpWhiteListOffline.find( strFriendName );
	STD_MAP_FRIEND_SNATCH::iterator	mIterOnline		=	m_mpWhiteListOnline.find( strFriendName );

	if( mIterOnline != m_mpWhiteListOnline.end() )
	{
		SAFE_DELETE( (*mIterOnline).second );
		m_mpWhiteListOnline.erase( mIterOnline );
	}

	if( mIterOffline != m_mpWhiteListOffline.end() )
	{
		SAFE_DELETE( (*mIterOffline).second );
		m_mpWhiteListOffline.erase( mIterOffline );
	}

	if( g_pGOBManager == NULL )
		return;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		pLocalPlayer->DelFriendList( strFriendName.c_str() );
	}
}

void SPWindowFriendSnatch::DelAllWhiteList()
{
	STD_MAP_FRIEND_SNATCH::iterator	mIter	=	m_mpWhiteListOffline.begin();

	while ( mIter != m_mpWhiteListOffline.end() )
	{
		SAFE_DELETE( (*mIter).second );
		++mIter;
	}

	m_mpWhiteListOffline.clear();

	mIter	=	m_mpWhiteListOnline.begin();

	while ( mIter != m_mpWhiteListOnline.end() )
	{
		SAFE_DELETE( (*mIter).second );
		++mIter;
	}

	m_mpWhiteListOnline.clear();

	if( g_pGOBManager == NULL )
		return;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		pLocalPlayer->DelFriendListAll();
	}
}

//------------------------------------------------------------------------------------------------------
//更新朋友名单
void SPWindowFriendSnatch::UpdateWhiteList( int iPage )
{
	int	iStartNum		=	iPage	*	WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER;
	int	iOnLineCount	=	(int)m_mpWhiteListOnline.size();
	int	iOffLineCount	=	(int)m_mpWhiteListOffline.size();
	int	iListCount		=	iOnLineCount	+	iOffLineCount;
	int	iMaxPage		=	iListCount / WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER;

	if( iListCount % WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER != 0 || iMaxPage == 0 )
		iMaxPage++;

	if( iPage >= iMaxPage )
	{
		iPage	=	iMaxPage - 1;
		iStartNum	=	iPage	*	WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER;
	}

	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , iMaxPage );

	BOOL						bOnLine	=	TRUE;
	STD_MAP_FRIEND_SNATCH::iterator	mIter	=	m_mpWhiteListOnline.begin();

	for( int i = 0 ; i < iStartNum ; i++ )
	{
		if( bOnLine == TRUE )
		{
			if( mIter == m_mpWhiteListOnline.end() )
			{
				mIter	=	m_mpWhiteListOffline.begin();
				bOnLine	=	FALSE;
				i--;
				continue;
			}
		}
		else
			if( mIter == m_mpWhiteListOffline.end() )	break;

		++mIter;
	}

	bool	bPageUpdate	=	false;

	if( iPage != m_iWhitePage )
		bPageUpdate	=	true;

	m_iWhitePage	=	iPage;
	m_pSlider->SetCurpage( m_iWhitePage );
	m_pSlider->Refresh();

	FRIEND_INFO*	pFriendInfo;

	for( i = 0 ; i < WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER ; i++ )
	{
		if( bOnLine == TRUE )
		{
			if( mIter == m_mpWhiteListOnline.end() )
			{
				mIter	=	m_mpWhiteListOffline.begin();
				bOnLine	=	FALSE;
				i--;
				continue;
			}
		}
		else
			if( mIter == m_mpWhiteListOffline.end() )	break;

		pFriendInfo	=	(FRIEND_INFO*)( (*mIter).second );

		if( bPageUpdate == true )
			m_pWhiteUser[ i ]->SetWndState( WND_STATE_NORMAL );

		m_pWhiteUser[ i ]->SetFriendInfo( pFriendInfo );
		m_pWhiteUser[ i ]->Show();

		++mIter;
	}

	HideWhiteList( i );
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::HideWhiteList( int iStartNum )
{
	for( int i = iStartNum ; i < WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER ; i++ )
	{
		m_pWhiteUser[ i ]->SetFriendInfo( NULL );
		m_pWhiteUser[ i ]->SetWndState( WND_STATE_NORMAL );
		m_pWhiteUser[ i ]->Hide();
	}
}

//---------------------------------------------------------------------------------------------------------------
//输出错误
void SPWindowFriendSnatch::PrintErrorMsg( GLOBAL_STRING_ID uiGlobalStringID , const char* pszName /* = NULL  */ )
{
	DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( uiGlobalStringID );

	if( pszGlobalString == NULL )
		return;

	if( uiGlobalStringID > FRIEND_SNATCH_GLOBAL_MSG_ONE_METHOD && pszName != NULL )
	{
		sprintf( szErrorMsg , pszGlobalString , pszName );
	}
	else
	{
		strcpy( szErrorMsg , pszGlobalString );
	}

	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szErrorMsg );
}

//-------------------------------------------------------------------------------------------------
void SPWindowFriendSnatch::RequestStatusFriend()
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	int	iFriendListCount	=	pLocalPlayer->GetFriendListCount();

	if( m_iSendFriendPos >= iFriendListCount || m_iSendFriendPos < 0 )
		m_iSendFriendPos	=	0;

	FRIEND_INFO*	pFriendInfo;
	UINT8			uiCount			=	iFriendListCount - m_iSendFriendPos;
	UINT8			uiFriendStep	=	6;

	if( uiCount > uiFriendStep )
		uiCount	=	uiFriendStep;

	if( uiCount <= 0 )
	{
		CPacket	FriendPacket;
		FriendPacket.AddUINT32( FRIEND_CS_GETLIST );
		( g_pNetworkManager->GetPacketHandler() )->PostMsg( &FriendPacket );

		m_fRefreshAccmulateTime	=	-1.0f;
		return;
	}

	DECLARE_VAR( TCHAR , szFriendName , MAX_LEN_NAME );

	CPacket	StatusFriend;
	StatusFriend.AddCommand( FRIEND_CS_REFRESH_STATUS );
	StatusFriend.AddUINT8( uiCount );

	for( int i = 0 ; i < uiCount ; i++ )
	{
		pFriendInfo	=	pLocalPlayer->GetFriendListMember( m_iSendFriendPos + i );

		if( pFriendInfo != NULL )
		{
			_tcsncpy( szFriendName, pFriendInfo->szCharName , LEN_NAME );
			StatusFriend.AddData( szFriendName , LEN_NAME );
		}
	}

	m_iSendFriendPos	+=	(int)uiFriendStep;

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &StatusFriend );

	m_fRefreshAccmulateTime	=	-1.0f;
}

//---------------------------------------------------------------------------------------------------
//更具不同的类型,得到相应的选择的名字
const char* SPWindowFriendSnatch::GetSelectFriendUserName( FRIEND_SNATCH_TYPE eType )
{
	const char*	pszName	=	NULL;

	if( eType == FRIEND_SNATCH_TYPE_NULL )
		return pszName;

	int			iSelectNumber;
	INSTANCE_ID	eStartID;
	INSTANCE_ID	eStepID;

	if( eType == FRIEND_SNATCH_TYPE_WHITE )
	{
		iSelectNumber	=	m_iWhiteListSelectNumber;
		eStartID		=	WIID_FRIEND_SNATCH_WHITE_LIST_USER_WINDOW;
		eStepID			=	WIID_FRIEND_SNATCH_WHITE_LIST_USER_STEP;
	}
	
	INSTANCE_ID uiInstanceID = eStartID + ( iSelectNumber * eStepID );

	SPWindowFriendSnatchUser*	pWindowFriendSnatchUser	=	(SPWindowFriendSnatchUser*)( Find( uiInstanceID , true ) );

	if( pWindowFriendSnatchUser != NULL )
	{
		pszName	=	pWindowFriendSnatchUser->GetName();
	}
	return pszName;
}

//-------------------------------------------------------------------------------
//接受SPWindowFriend传过来的删除了的名字
int SPWindowFriendSnatch::OnDelFriendName( WPARAM wParam, LPARAM lParam )
{
	if ( NULL == lParam )
	{
		return 1;
	}

	m_strLastDelName = *(string*)lParam;
	return 1;
}

int SPWindowFriendSnatch::OnSnatchResult( WPARAM wParam, LPARAM lParam )
{
	CPacket* pPacket = (CPacket*)lParam;
	UINT32 iItemID  = 0;
	pPacket->ExtractUINT32( &iItemID );
	GLOBAL_STRING_ID iError = 0;
	pPacket->ExtractUINT32( &iError );

	if ( 170200603 == iItemID )
	{
		if ( iError )
		{
			std::string strMsg = "无法召唤对方";				//~
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() ); //~~
		}
		Hide();
	}

	return 1;
}

void SPWindowFriendSnatch::StartToSnatch()
{
	if(g_pGOBManager->GetLocalPlayer()) 
	{
		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();

		if(pInven)
		{
			pInven->SnatchFriend( m_SnatchName );
		}
	}
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendSnatch::OnWhiteList( WPARAM wParam, LPARAM lParam )
{
	int	iType	=	(int)wParam;

	if( iType == 0 )
	{
		CPacket	FriendPacket;
		FriendPacket.AddUINT32( FRIEND_CS_GETLIST );
		( g_pNetworkManager->GetPacketHandler() )->PostMsg( &FriendPacket );
	}

	m_pWhiteListPanel->Show();

	m_pWhiteButton->SPSendMessage( SPIM_BTN_SELECT , TRUE );

	m_pEdit->SetWindowText( NULL );

	m_eFriendType				=	FRIEND_SNATCH_TYPE_WHITE;
	m_iWhiteListSelectNumber	=	-1;
	m_strLastAddName.clear();

	UpdateWhiteList( m_iWhitePage );

	return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------
//暂时不用
int SPWindowFriendSnatch::OnUserFindFriend( WPARAM wParam, LPARAM lParam )
{
	int	iType			=	(int)wParam;

	if( iType == FRIEND_SNATCH_TYPE_WHITE )
	{
		return	OnWhitePeopleFind( wParam , lParam );
	}
	return 1;
}