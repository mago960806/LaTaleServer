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
#include "SPWindowFriendFollowUser.h"
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

#include "SPWindowFriendFollow.h"
#include "SPPlayerInvenArchive.h"
#include "SPWindowEditMultiLine.h"
#include "SPWindowWorldMap.h"
#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTimer.h"

SPWindowFriendFollow::SPWindowFriendFollow(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

//------------------------------------------------------------------------------------------------------
SPWindowFriendFollow::SPWindowFriendFollow(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_FRIEND_FOLLOW, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

//------------------------------------------------------------------------------------------------------
SPWindowFriendFollow::~SPWindowFriendFollow()
{
	Clean();
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendFollow::Init()
{
	m_strToolTip		=	"[SPWindowFriendFollow]";
	m_eFriendType		=	FRIEND_FOLLOW_TYPE_WHITE;
	m_bDeletePacketSend	=	false;

	m_DeqRecentName.clear();
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

	pWindowStatic	=	new SPWindowStatic( WIID_FRIEND_FOLLOW_TITLE_IMG , 8 , 7 , 32 , 23 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 72 , 133 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_FRIEND_FOLLOW_TITLE_TEXT , 42 , 7 , 108 , 17 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 110 , 134 );
	pWindowStatic	=	NULL;

	SPWindowButton*	pWindowButton;

	pWindowButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_CLOSE , 211 , 9 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 217 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 230 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 243 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 256 );
	pWindowButton	=	NULL;

	m_pWhiteButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_WHITE_LIST_BUTTON , 10 , 55 , 64 , 15 , this );
	m_pWhiteButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 384 );
	m_pWhiteButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 400 );
	m_pWhiteButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 416 );
	m_pWhiteButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 432 );
	m_pWhiteButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 416 );

	m_pBlackButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_BLACK_LIST_BUTTON , 73 , 55 , 64 , 15 , this );
	m_pBlackButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 261 , 384 );
	m_pBlackButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 261 , 400 );
	m_pBlackButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 261 , 416 );
	m_pBlackButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 261 , 432 );
	m_pBlackButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 261 , 416 );

	//Recent
	m_pRecentButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_RECENT_LIST_BUTTON , 137 , 55 , 64 , 15 , this );
	m_pRecentButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 443 , 0 );//change
	m_pRecentButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 443 , 17 );
	m_pRecentButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 443 , 34 );
	m_pRecentButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 443 , 51 );
	m_pRecentButton->SetImageCheck		( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 443 , 34 );

	pWindowButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_SLIDER_UP , 210 , 85 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_SLIDER_DOWN , 210 , 246 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );
	pWindowButton =	NULL;

	RECT rtRect;
	m_pSlider	=	new	SPWindowSlider( WIID_FRIEND_FOLLOW_SLIDER , 210 , 96 , 12 , 150 , this );

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

	m_pEdit	=	new SPWindowEdit( WIID_FRIEND_FOLLOW_EDIT , 10 , 34 , 140 , 17 , this );
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

	InitWhite();
	InitBlack();
	InitRecent();
	InitFile();
	//add
	m_iSendFriendPos	=	0;

}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendFollow::ReposOwnImage()
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
void SPWindowFriendFollow::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendFollow::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );

	DelAllWhiteList();
	DelAllBlackList();
	DelAllRecentList();
	//add
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendFollow::Show(bool bWithChild)
{
	if(m_bShow)
	{
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	}
	
	SPWindow::Show(bWithChild);

	HideWhiteList( 0 );
	HideBlackList( 0 );
	HideRecentList( 0 );
	//add
	if( m_eFriendType == FRIEND_FOLLOW_TYPE_WHITE )
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
	else if( m_eFriendType == FRIEND_FOLLOW_TYPE_BLACK )
	{
		OnBlackList( 1 , NULL );
	}
	else if( m_eFriendType == FRIEND_FOLLOW_TYPE_RECENT )
	{
		OnRecentList( 1, NULL );
	}
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendFollow::Hide(bool bSendServer)
{
	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	if(g_pGOBManager->GetLocalPlayer()) 
	{

		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();

		if(pInven) 
		{
			pInven->ClearFollowItem();
		}
	}

	SPWindow::Hide(bSendServer);
	//add

	if( m_eFriendType == FRIEND_FOLLOW_TYPE_WHITE )	//	WhiteList
	{
		OnWhiteLButtonUp( 0 , NULL , NULL );
	}
	else if( m_eFriendType == FRIEND_FOLLOW_TYPE_BLACK )	//	BlackList
	{
		OnBlackLButtonUp( 0 , NULL , NULL );
	}
	else if( m_eFriendType == FRIEND_FOLLOW_TYPE_RECENT ) //recent
	{
		OnRecentLButtonUp( 0, NULL, NULL );
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
void SPWindowFriendFollow::Process(float fTime)
{
	SPWindow::Process( fTime );
	if( m_bShow == false || m_eFriendType != FRIEND_FOLLOW_TYPE_WHITE ||
		m_fRefreshLimit == 0.0f || m_fRefreshAccmulateTime < 0.0f )
		return;

	m_fRefreshAccmulateTime	+=	fTime;

	if( m_fRefreshAccmulateTime >= m_fRefreshLimit )
		RequestStatusFriend();
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendFollow::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < FRIEND_FOLLOW_BG_COUNT ; i++ )
		{
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	SPWindow::Render(fTime);
}

//------------------------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowFriendFollow )
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

//SPIMESSAGE_COMMAND( SPIM_USER_ADD_FRIEND	,										OnUserAddFriend		)
//SPIMESSAGE_COMMAND( SPIM_USER_DEL_FRIEND	,										OnUserDelFriend		)
//SPIMESSAGE_COMMAND( SPIM_USER_CHECK_FRIEND	,									OnUserCheckFriend	)

SPIMESSAGE_COMMAND( SPIM_FOLLOW_FAIL		,										OnFollowResult		)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_CLOSE				,	OnClose				)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_WHITE_LIST_BUTTON	,	OnWhiteList			)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_BLACK_LIST_BUTTON	,	OnBlackList			)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_RECENT_LIST_BUTTON	,   OnRecentList		)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_SLIDER_UP			,	OnSliderUp			)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_SLIDER_DOWN			,	OnSliderDown		)

SPIMESSAGE_CONTROL( SPIM_SLIDER_UP			,	WIID_FRIEND_FOLLOW_SLIDER				,	OnSlider			)
SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN		,	WIID_FRIEND_FOLLOW_SLIDER				,	OnSlider			)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP		,	WIID_FRIEND_FOLLOW_SLIDER				,	OnSlider			)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN	,	WIID_FRIEND_FOLLOW_SLIDER				,	OnSlider			)
SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE		,	WIID_FRIEND_FOLLOW_SLIDER				,	OnSlider			)

SPIMESSAGE_CONTROL(	SPIM_WHEEL_UP			,	WIID_FRIEND_FOLLOW_WHITE_LIST_PANEL	,	OnSliderUp			)
SPIMESSAGE_CONTROL(	SPIM_WHEEL_DOWN			,	WIID_FRIEND_FOLLOW_WHITE_LIST_PANEL	,	OnSliderDown		)

SPIMESSAGE_CONTROL(	SPIM_WHEEL_UP			,	WIID_FRIEND_FOLLOW_RECENT_LIST_PANEL	,	OnSliderUp			)
SPIMESSAGE_CONTROL(	SPIM_WHEEL_DOWN			,	WIID_FRIEND_FOLLOW_RECENT_LIST_PANEL	,	OnSliderDown		)

SPIMESSAGE_CONTROL(	SPIM_SETFOCUS			,	WIID_FRIEND_FOLLOW_EDIT				,	OnEditSetFocus		)
SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS			,	WIID_FRIEND_FOLLOW_EDIT				,	OnEditKillFocus		)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_WHITE_LIST_FIND			,	OnWhitePeopleFind		)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_WHITE_LIST_DEL			,	OnWhiteListDel		)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_WHITE_LIST_INVITATION	,	OnWhiteListInvitation)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_WHITE_LIST_WHISPER		,	OnWhiteListWhisper	)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	,		WIID_FRIEND_FOLLOW_WHITE_LIST_USER_WINDOW	,	WIID_FRIEND_FOLLOW_WHITE_LIST_USER_WINDOW + WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER * WIID_FRIEND_FOLLOW_WHITE_LIST_USER_STEP	,	OnWhiteLButtonUp	)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DBLCLICK	,	WIID_FRIEND_FOLLOW_WHITE_LIST_USER_WINDOW	,	WIID_FRIEND_FOLLOW_WHITE_LIST_USER_WINDOW + WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER * WIID_FRIEND_FOLLOW_WHITE_LIST_USER_STEP	,	OnWhiteLButtonDoubleUp	)

SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP		,	WIID_FRIEND_FOLLOW_WHITE_LIST_USER_WINDOW	,	WIID_FRIEND_FOLLOW_WHITE_LIST_USER_WINDOW + WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER * WIID_FRIEND_FOLLOW_WHITE_LIST_USER_STEP	,	OnWhiteSliderUp		)
SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN	,	WIID_FRIEND_FOLLOW_WHITE_LIST_USER_WINDOW	,	WIID_FRIEND_FOLLOW_WHITE_LIST_USER_WINDOW + WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER * WIID_FRIEND_FOLLOW_WHITE_LIST_USER_STEP	,	OnWhiteSliderDown	)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_RECENT_LIST_FIND		,	OnRecentPeopleFind		)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_RECENT_LIST_ADD		,	OnRecentListAdd		)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_RECENT_LIST_INVITATION ,	OnRecentListInvitation)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_RECENT_LIST_WHISPER	,	OnRecentListWhisper	)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,		WIID_FRIEND_FOLLOW_RECENT_LIST_USER_WINDOW	,	WIID_FRIEND_FOLLOW_RECENT_LIST_USER_WINDOW + WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER * WIID_FRIEND_FOLLOW_RECENT_LIST_USER_STEP	,	OnRecentLButtonUp	)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DBLCLICK		,	WIID_FRIEND_FOLLOW_RECENT_LIST_USER_WINDOW	,	WIID_FRIEND_FOLLOW_RECENT_LIST_USER_WINDOW + WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER * WIID_FRIEND_FOLLOW_RECENT_LIST_USER_STEP	,	OnRecentLButtonDoubleUp	)

SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP			,	WIID_FRIEND_FOLLOW_RECENT_LIST_USER_WINDOW	,	WIID_FRIEND_FOLLOW_RECENT_LIST_USER_WINDOW + WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER * WIID_FRIEND_FOLLOW_RECENT_LIST_USER_STEP	,	OnRecentSliderUp		)
SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN		,	WIID_FRIEND_FOLLOW_RECENT_LIST_USER_WINDOW	,	WIID_FRIEND_FOLLOW_RECENT_LIST_USER_WINDOW + WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER * WIID_FRIEND_FOLLOW_RECENT_LIST_USER_STEP	,	OnRecentSliderDown	)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_BLACK_LIST_FIND		,	OnBlackPeopleFind		)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_FOLLOW_BLACK_LIST_DEL		,	OnBlackListDel		)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_FRIEND_FOLLOW_BLACK_LIST_USER_WINDOW	,	WIID_FRIEND_FOLLOW_BLACK_LIST_USER_WINDOW + WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER * WIID_FRIEND_FOLLOW_BLACK_LIST_USER_STEP	,	OnBlackLButtonUp	)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DBLCLICK	,	WIID_FRIEND_FOLLOW_BLACK_LIST_USER_WINDOW	,	WIID_FRIEND_FOLLOW_BLACK_LIST_USER_WINDOW + WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER * WIID_FRIEND_FOLLOW_BLACK_LIST_USER_STEP	,	OnBlackLButtonDoubleUp	)

SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP		,	WIID_FRIEND_FOLLOW_BLACK_LIST_USER_WINDOW	,	WIID_FRIEND_FOLLOW_BLACK_LIST_USER_WINDOW + WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER * WIID_FRIEND_FOLLOW_BLACK_LIST_USER_STEP	,	OnBlackSliderUp		)
SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN	,	WIID_FRIEND_FOLLOW_BLACK_LIST_USER_WINDOW	,	WIID_FRIEND_FOLLOW_BLACK_LIST_USER_WINDOW + WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER * WIID_FRIEND_FOLLOW_BLACK_LIST_USER_STEP	,	OnBlackSliderDown	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnCursorEnter( WPARAM wParam, LPARAM lParam )
{
	if( m_eFriendType == FRIEND_FOLLOW_TYPE_WHITE )
	{
	}

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnLButtonUp( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnLButtonDown( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnRButtonUp( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnKeyReturn( WPARAM wParam, LPARAM lParam )//暂时去掉此功能
{
	//if( m_pEdit->IsFocus() == TRUE )
	//{
	//	OnUserFindFriend( (WPARAM)m_eFriendType , NULL );

	//	return 1;
	//}

	return -1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnPurge( WPARAM wParam, LPARAM lParam )
{
	m_eFriendType	=	FRIEND_FOLLOW_TYPE_WHITE;

	DelAllWhiteList();
	DelAllBlackList();
	DelAllRecentList();
	//add
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnGetFriend( WPARAM wParam, LPARAM lParam )
{
	FRIEND_FOLLOW_TYPE		eFriendType	=	(FRIEND_FOLLOW_TYPE)wParam;
	CPacket		pPacketTemp		=	*(CPacket*)lParam;
	CPacket*		pPacket		=	&pPacketTemp;    
	UINT8			uiCount		=	0;
	FRIEND_INFO*	pFriendInfo	=	NULL;
	DECLARE_VAR( TCHAR , szBlockName , MAX_LEN_NAME );

	if( eFriendType == FRIEND_FOLLOW_TYPE_WHITE )
	{
		DelAllWhiteList();
	}
	else if( eFriendType == FRIEND_FOLLOW_TYPE_BLACK )
	{
		DelAllBlackList();
	}
	//add
	pPacket->ExtractUINT8( &uiCount );

	for( UINT8 i = 0 ; i < uiCount ; ++i )
	{
		switch( eFriendType )
		{
		case FRIEND_FOLLOW_TYPE_WHITE:
			pFriendInfo	=	new FRIEND_INFO;
			pPacket->ExtractStruct( pFriendInfo , sizeof(FRIEND_INFO) );
			AddWhiteList( pFriendInfo );
			break;
		case FRIEND_FOLLOW_TYPE_BLACK:
			pPacket->ExtractData( (TCHAR*)&szBlockName , sizeof(TCHAR) * LEN_NAME );
			AddBlackList( szBlockName );
			break;
		}
	}

	if( m_eFriendType == FRIEND_FOLLOW_TYPE_WHITE )
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
	else if( m_eFriendType == FRIEND_FOLLOW_TYPE_BLACK )
	{
		UpdateBlackList( m_iBlackPage );
	}

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnAddFriend( WPARAM wParam, LPARAM lParam )
{
	//add
	FRIEND_FOLLOW_TYPE			eFriendType			=	(FRIEND_FOLLOW_TYPE)wParam;
	CPacket		pPacketTemp		=	*(CPacket*)lParam;
	CPacket*		pPacket		=	&pPacketTemp;  
	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	if( pPacket == NULL || eFriendType == FRIEND_FOLLOW_TYPE_NULL )
		return 1;

	pPacket->Extract( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		//PrintErrorMsg( uiGlobalStringID , m_strLastAddName.c_str() );
		return 1;
	}

	if( eFriendType == FRIEND_FOLLOW_TYPE_WHITE )
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
	else if( eFriendType == FRIEND_FOLLOW_TYPE_BLACK )
	{
		UINT8				uiStringLen			=	0;
		TCHAR				szFriendName [LEN_NAME+1] = {0,};

		pPacket->ExtractUINT8( &uiStringLen );

		if( uiStringLen > 0 )
		{
			pPacket->ExtractData( szFriendName , uiStringLen );
		}

		std::string	strFriendName	=	szFriendName;

		if( strFriendName.empty() == true )
			return 1;

		//		PrintErrorMsg( GS_FRIEND_BLACK_ADD , strFriendName.c_str() );
		AddBlackList( strFriendName.c_str() );
		UpdateBlackList( m_iBlackPage );
	}

	m_strLastAddName.clear();

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnAcceptFriend( WPARAM wParam, LPARAM lParam )
{
	//add
	FRIEND_FOLLOW_TYPE			eFriendType			=	(FRIEND_FOLLOW_TYPE)wParam;
	CPacket		pPacketTemp		=	*(CPacket*)lParam;
	CPacket*		pPacket		=	&pPacketTemp;  
	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	if( pPacket == NULL || eFriendType == FRIEND_FOLLOW_TYPE_NULL )
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

	if( eFriendType == FRIEND_FOLLOW_TYPE_WHITE )
	{
		//		PrintErrorMsg( GS_FRIEND_WHITE_ACCEPT_YES , pFriendInfo->szCharName );
		AddWhiteList( pFriendInfo );
		UpdateWhiteList( m_iWhitePage );
		m_fRefreshAccmulateTime	=	0.0f;
	}

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnDelFriend( WPARAM wParam, LPARAM lParam )
{
	FRIEND_FOLLOW_TYPE			eFriendType			=	(FRIEND_FOLLOW_TYPE)wParam;
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
	if( eFriendType == FRIEND_FOLLOW_TYPE_WHITE )	//	WhiteList
	{
		//PrintErrorMsg( GS_FRIEND_WHITE_DEL , m_strLastDelName.c_str() );
		DelWhiteList( m_strLastDelName.c_str() );
		UpdateWhiteList( m_iWhitePage );
		OnWhiteLButtonUp( 0 , NULL , NULL );

		int	iWhiteCount	=	(int)m_mpWhiteListOnline.size() + (int)m_mpWhiteListOffline.size();

		if( iWhiteCount <= 0 )
			m_fRefreshAccmulateTime	=	-1.0f;
	}
	else if( eFriendType == FRIEND_FOLLOW_TYPE_BLACK )	//	BlackList
	{
		//PrintErrorMsg( GS_FRIEND_BLACK_DEL , m_strLastDelName.c_str() );
		DelBlackList( m_strLastDelName.c_str() );
		UpdateBlackList( m_iBlackPage );
		OnBlackLButtonUp( 0 , NULL , NULL );
	}

	m_strLastDelName.clear();

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnStatusFriend( WPARAM wParam, LPARAM lParam )
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
int SPWindowFriendFollow::OnNoticeYes( WPARAM wParam, LPARAM lParam )
{
	StartToFollow();
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnNoticeNo( WPARAM wParam, LPARAM lParam )
{
	int	iForceHide	=	(int)lParam;

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnWhiteList( WPARAM wParam, LPARAM lParam )
{
	int	iType	=	(int)wParam;

	if( iType == 0 )
	{
		CPacket	FriendPacket;
		FriendPacket.AddUINT32( FRIEND_CS_GETLIST );
		( g_pNetworkManager->GetPacketHandler() )->PostMsg( &FriendPacket );
	}

	m_pBlackListPanel->Hide();
	m_pRecentListPanel->Hide();//recent
	m_pWhiteListPanel->Show();

	m_pBlackButton->SPSendMessage( SPIM_BTN_SELECT , FALSE );
	m_pRecentButton->SPSendMessage( SPIM_BTN_SELECT, FALSE );//recent
	m_pWhiteButton->SPSendMessage( SPIM_BTN_SELECT , TRUE );

	m_pEdit->SetWindowText( NULL );

	m_eFriendType				=	FRIEND_FOLLOW_TYPE_WHITE;
	m_iWhiteListSelectNumber	=	-1;
	m_strLastAddName.clear();
	m_strLastDelName.clear();

	UpdateWhiteList( m_iWhitePage );

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnBlackList( WPARAM wParam, LPARAM lParam )
{
	int	iType	=	(int)wParam;

	if( iType == 0 )
	{
		CPacket	BlockPacket;
		BlockPacket.AddUINT32( BLOCK_CS_GETLIST );
		( g_pNetworkManager->GetPacketHandler() )->PostMsg( &BlockPacket );
	}

	m_pWhiteListPanel->Hide();
	m_pRecentListPanel->Hide();
	m_pBlackListPanel->Show();

	m_pWhiteButton->SPSendMessage( SPIM_BTN_SELECT , FALSE );
	m_pRecentButton->SPSendMessage( SPIM_BTN_SELECT, FALSE);
	m_pBlackButton->SPSendMessage( SPIM_BTN_SELECT , TRUE );
	m_pEdit->SetWindowText( NULL );

	m_eFriendType				=	FRIEND_FOLLOW_TYPE_BLACK;
	m_iBlackListSelectNumber	=	-1;
	m_strLastAddName.clear();
	m_strLastDelName.clear();

	UpdateBlackList( m_iBlackPage );

	return 1;
}

//-------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnRecentList( WPARAM wParam, LPARAM lParam )
{	
	m_pWhiteListPanel->Hide();
	m_pBlackListPanel->Hide();
	m_pRecentListPanel->Show();

	m_pWhiteButton->SPSendMessage( SPIM_BTN_SELECT , FALSE );
	m_pBlackButton->SPSendMessage( SPIM_BTN_SELECT , FALSE );
	m_pRecentButton->SPSendMessage( SPIM_BTN_SELECT, TRUE );

	m_pEdit->SetWindowText( NULL );

	m_eFriendType				=	FRIEND_FOLLOW_TYPE_RECENT;
	m_iWhiteListSelectNumber	=	-1;
	m_strLastAddName.clear();
	m_strLastDelName.clear();

	UpdateRecentList( m_iRecentPage );

	return 1;
}
//-------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnSliderUp( WPARAM wParam, LPARAM lParam )
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnSliderDown( WPARAM wParam, LPARAM lParam )
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnSlider( WPARAM wParam, LPARAM lParam )
{
	int	iNewPage	=	(int)wParam;

	if( m_eFriendType == FRIEND_FOLLOW_TYPE_WHITE )
	{
		m_iWhiteListSelectNumber	=	-1;
		UpdateWhiteList( iNewPage );
		OnWhiteLButtonUp( 0 , NULL , NULL );
	}
	else if( m_eFriendType == FRIEND_FOLLOW_TYPE_BLACK )
	{
		m_iBlackListSelectNumber	=	-1;
		UpdateBlackList( iNewPage );
		OnBlackLButtonUp( 0 , NULL , NULL );
	}
	else if ( m_eFriendType == FRIEND_FOLLOW_TYPE_RECENT )
	{
		m_iRecentListSelectNumber	=	-1;
		UpdateRecentList( iNewPage );
		OnRecentLButtonUp( 0, NULL, NULL );
	}

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}

//------------------------------------------------------------------------------------------------------
//被屏蔽
int SPWindowFriendFollow::OnWhiteListAdd( WPARAM wParam, LPARAM lParam )
{
	DECLARE_VAR( TCHAR , szFriendName , MAX_LEN_NAME );

	int	iType				=	(int)wParam;
	const char*	pszAddName	=	(const char*)lParam;

	if( pszAddName != NULL && iType != 0 )
	{
		strncpy( szFriendName , pszAddName, LEN_NAME );
	}
	else
	{
		strncpy( szFriendName , m_pEdit->GetWindowText() ,LEN_NAME);
		m_pEdit->SetWindowText( NULL );
	}

	UINT8	len = min( (UINT8)_tcslen( szFriendName ) , LEN_NAME );

	if( len <= 0 )
		return 1;

	std::string	strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , szFriendName );

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		char		szLocalName[ LEN_NAME + 1 ];
		std::string	strLocalName;

		pLocalPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szLocalName );

		strLocalName.clear();
		GetUpperString3( strLocalName , szLocalName );

		if( strFriendName.compare( strLocalName ) == 0 )
		{
			PrintErrorMsg( GS_FRIEND_WHITE_MYSELF );
			return 1;
		}
	}

	if( OnUserCheckFriend( (WPARAM)FRIEND_FOLLOW_TYPE_WHITE , (LPARAM)strFriendName.c_str() ) == 1 )
	{
		PrintErrorMsg( GS_FRIEND_WHITE_ALREADY );
		return 1;
	}

	if( OnUserCheckFriend( (WPARAM)FRIEND_FOLLOW_TYPE_BLACK , (LPARAM)strFriendName.c_str() ) == 1 )
	{
		PrintErrorMsg( GS_FRIEND_BLACK_ALREADY );
		return 1;
	}

	CPacket	packet;

	m_strLastAddName	=	szFriendName;

	packet.AddUINT32( FRIEND_CS_ADD );
	packet.AddUINT8( len );
	packet.AddData( &szFriendName , len );

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &packet );

	return 1;
}

//------------------------------------------------------------------------------------------------------
//被屏蔽
int SPWindowFriendFollow::OnWhiteListDel( WPARAM wParam, LPARAM lParam )
{
	if( m_bDeletePacketSend == true )
		return 1;

	const char*	pszName	=	(const char*)wParam;

	if( m_iWhiteListSelectNumber < 0 && pszName == NULL )
		return 1;

	if( pszName != NULL )
		m_strLastDelName	=	pszName;

	if( m_strLastDelName.empty() == true )
	{
		pszName	=	GetSelectFriendUserName( FRIEND_FOLLOW_TYPE_WHITE );

		if( pszName != NULL )
		{
			m_strLastDelName	=	pszName;
		}
	}

	if( m_strLastDelName.empty() == true )
		return 1;

	DECLARE_VAR( TCHAR , szFriendName , MAX_LEN_NAME );

	strncpy( szFriendName , m_strLastDelName.c_str(),LEN_NAME );

	UINT8	len = min( (UINT8)_tcslen( szFriendName ) , LEN_NAME );

	if( len <= 0 )
	{
		m_strLastDelName.clear();
		return 1;
	}

	//g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND, SPIM_FRIEND_NAME, 1, (LPARAM)&m_strLastDelName );

	//add 将删除的名字传过去
	CPacket	packet;

	packet.AddUINT32( FRIEND_CS_DELETE );
	packet.AddUINT8( len );
	packet.AddData( &szFriendName , len );

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &packet );

	m_bDeletePacketSend	=	true;

	return 1;
}

//------------------------------------------------------------------------------------------------------
//被屏蔽
int SPWindowFriendFollow::OnWhiteListInvitation( WPARAM wParam, LPARAM lParam )
{
	const char*	pszName	=	GetSelectFriendUserName( FRIEND_FOLLOW_TYPE_WHITE );

	if( pszName == NULL )
		return 1;

	std::string	strName		=	pszName;
	SPPlayer* pkLocalPlayer =	g_pGOBManager->GetLocalPlayer();

	if( pkLocalPlayer != NULL && strName.empty() == FALSE )
		pkLocalPlayer->SPGOBPerformMessage( SPGM_TEAM_INVITE_REQ , (LPARAM)strName.c_str() );

	return 1;
}

//------------------------------------------------------------------------------------------------------
//被屏蔽
int SPWindowFriendFollow::OnWhiteListWhisper( WPARAM wParam, LPARAM lParam )
{
	const char*	pszName	=	GetSelectFriendUserName( FRIEND_FOLLOW_TYPE_WHITE );

	if( pszName == NULL )
		return 1;

	std::string	strName		=	pszName;

	if( strName.empty() == FALSE )
	{
		OnEndMove( NULL , NULL );
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_INSERT_MSGEDIT , CHAT_STRING_WHISPER , (LPARAM)strName.c_str() );
	}

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnWhiteLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	unsigned int	uiInstanceID;
	SPWindow*		pWindow;

	m_iWhiteListSelectNumber	=	-1;

	for( int i = 0 ; i < WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER ; ++i )
	{
		uiInstanceID = WIID_FRIEND_FOLLOW_WHITE_LIST_USER_WINDOW + ( i * WIID_FRIEND_FOLLOW_WHITE_LIST_USER_STEP );

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
int	SPWindowFriendFollow::OnWhiteLButtonDoubleUp( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	const char*	pszName	=	(const char*)wParam;


	if( pszName != NULL )
	{
		m_pEdit->SetWindowText( pszName );
	}
	pszName	=	GetSelectFriendUserName( FRIEND_FOLLOW_TYPE_WHITE );

	if( pszName != NULL )
	{
		m_pEdit->SetWindowText( pszName );
	}
	return 1;
}

//------------------------------------------------------------------------------------------------------
int	SPWindowFriendFollow::OnBlackLButtonDoubleUp( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	const char*	pszName	=	(const char*)wParam;


	if( pszName != NULL )
	{
		m_pEdit->SetWindowText( pszName );
	}
	pszName	=	GetSelectFriendUserName( FRIEND_FOLLOW_TYPE_BLACK );

	if( pszName != NULL )
	{
		m_pEdit->SetWindowText( pszName );
	}
	return 1;
}

//------------------------------------------------------------------------------------------------------
int	SPWindowFriendFollow::OnRecentLButtonDoubleUp( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	const char*	pszName	=	(const char*)wParam;


	if( pszName != NULL )
	{
		m_pEdit->SetWindowText( pszName );
	}
	pszName	=	GetSelectFriendUserName( FRIEND_FOLLOW_TYPE_RECENT );

	if( pszName != NULL )
	{
		m_pEdit->SetWindowText( pszName );
	}
	return 1;
}
//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnWhiteSliderUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnSliderUp( wParam , lParam );
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnWhiteSliderDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnSliderDown( wParam , lParam );
	return 1;
}

//-------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnWhitePeopleFind( WPARAM wParam, LPARAM lParam )
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

	m_FollowName = strName;

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
			std::string strMsg = "不能自己跟随自己";				//~
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() ); //~~
			return 1;
		}
	}

	g_pInterfaceManager->SetNotice( "确定要跟随该玩家吗？" , this );
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnBlackPeopleFind( WPARAM wParam, LPARAM lParam )
{
	std::string strName = m_pEdit->GetWindowText();
	std::string strTest;
	GetUpperString3( strTest , m_pEdit->GetWindowText() );

	if( strName.empty() )
	{
		return 1;
	}
	int iNameLen = static_cast<int>(strName.size());

	if( iNameLen <= 0 || iNameLen > LEN_NAME )
	{
		return 1;
	}

	m_FollowName = strName;

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
			std::string strMsg = "不能自己跟随自己";				//~
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() ); //~~
			return 1;
		}
	}

	g_pInterfaceManager->SetNotice( "确定要跟随该玩家吗？" , this );
	return 1;
}

int SPWindowFriendFollow::OnRecentPeopleFind( WPARAM wParam, LPARAM lParam )
{
	std::string strName = m_pEdit->GetWindowText();

	std::string strTest;
	GetUpperString3( strTest , m_pEdit->GetWindowText() );

	if( strName.empty() )
	{
		return 1;
	}
	int iNameLen = static_cast<int>(strName.size());

	if( iNameLen <= 0 || iNameLen > LEN_NAME )
	{
		return 1;
	}

	m_FollowName = strName;

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
			std::string strMsg = "不能自己跟随自己";				//~
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() ); //~~
			return 1;
		}
	}

	g_pInterfaceManager->SetNotice( "确定要跟随该玩家吗？" , this );

	return 1;
}

//-------------------------------------------------------------------------------------------------
//被屏蔽
int SPWindowFriendFollow::OnRecentListAdd( WPARAM wParam, LPARAM lParam )
{
	DECLARE_VAR( TCHAR , szFriendName , MAX_LEN_NAME );

	int	iType				=	(int)wParam;
	const char*	pszAddName	=	(const char*)lParam;
	const char* pszName     =   GetSelectFriendUserName( FRIEND_FOLLOW_TYPE_RECENT );
	if ( NULL == pszName )
	{
		return 1;
	}
	if( pszAddName != NULL && iType != 0 )
	{
		strncpy( szFriendName , pszAddName, LEN_NAME );
	}
	else
	{
		strncpy( szFriendName , pszName, LEN_NAME);
		//m_pEdit->SetWindowText( NULL );
	}

	UINT8	len = min( (UINT8)_tcslen( szFriendName ) , LEN_NAME );

	if( len <= 0 )
		return 1;

	std::string	strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , szFriendName );

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		char		szLocalName[ LEN_NAME + 1 ];
		std::string	strLocalName;

		pLocalPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szLocalName );

		strLocalName.clear();
		GetUpperString3( strLocalName , szLocalName );

		if( strFriendName.compare( strLocalName ) == 0 )
		{
			PrintErrorMsg( GS_FRIEND_WHITE_MYSELF );
			return 1;
		}
	}

	if( OnUserCheckFriend( (WPARAM)FRIEND_FOLLOW_TYPE_WHITE , (LPARAM)strFriendName.c_str() ) == 1 )
	{
		PrintErrorMsg( GS_FRIEND_WHITE_ALREADY );
		return 1;
	}

	if( OnUserCheckFriend( (WPARAM)FRIEND_FOLLOW_TYPE_BLACK , (LPARAM)strFriendName.c_str() ) == 1 )
	{
		PrintErrorMsg( GS_FRIEND_BLACK_ALREADY );
		return 1;
	}



	CPacket	packet;

	m_strLastAddName	=	szFriendName;
	//g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND, SPIM_FRIEND_NAME, 1, (LPARAM)&m_strLastDelName );

	packet.AddUINT32( FRIEND_CS_ADD );
	packet.AddUINT8( len );
	packet.AddData( &szFriendName , len );

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &packet );

	return 1;
}

//------------------------------------------------------------------------------------------------------
//被屏蔽
int SPWindowFriendFollow::OnRecentListInvitation( WPARAM wParam, LPARAM lParam )
{
	const char*	pszName	=	GetSelectFriendUserName( FRIEND_FOLLOW_TYPE_RECENT );

	if( pszName == NULL )
		return 1;

	std::string	strName		=	pszName;
	SPPlayer* pkLocalPlayer =	g_pGOBManager->GetLocalPlayer();

	if( pkLocalPlayer != NULL && strName.empty() == FALSE )
		pkLocalPlayer->SPGOBPerformMessage( SPGM_TEAM_INVITE_REQ , (LPARAM)strName.c_str() );

	return 1;
}

//------------------------------------------------------------------------------------------------------
//被屏蔽
int SPWindowFriendFollow::OnRecentListWhisper( WPARAM wParam, LPARAM lParam )
{
	const char*	pszName	=	GetSelectFriendUserName( FRIEND_FOLLOW_TYPE_RECENT );

	if( pszName == NULL )
		return 1;

	std::string	strName		=	pszName;

	if( strName.empty() == FALSE )
	{
		OnEndMove( NULL , NULL );
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_INSERT_MSGEDIT , CHAT_STRING_WHISPER , (LPARAM)strName.c_str() );
	}

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnRecentLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	unsigned int	uiInstanceID;
	SPWindow*		pWindow;

	m_iRecentListSelectNumber	=	-1;

	for( int i = 0 ; i < WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER ; ++i )
	{
		uiInstanceID = WIID_FRIEND_FOLLOW_RECENT_LIST_USER_WINDOW + ( i * WIID_FRIEND_FOLLOW_RECENT_LIST_USER_STEP );

		if( uiInstanceID == iID )
		{
			m_iRecentListSelectNumber	=	i;
			continue;
		}

		pWindow	=	Find( uiInstanceID , true );

		if( pWindow == NULL )
			continue;

		pWindow->SPSendMessage( SPIM_CURSOR_OUT , 1 );
	}

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnRecentSliderUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnSliderUp( wParam , lParam );
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnRecentSliderDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnSliderDown( wParam , lParam );
	return 1;
}

//-------------------------------------------------------------------------------------------------
//被屏蔽
int SPWindowFriendFollow::OnBlackListAdd( WPARAM wParam, LPARAM lParam )
{
	DECLARE_VAR( TCHAR , szFriendName , MAX_LEN_NAME );

	int	iType				=	(int)wParam;
	const char*	pszAddName	=	(const char*)lParam;

	if( pszAddName != NULL && iType != 0 )
	{
		strncpy( szFriendName , pszAddName, LEN_NAME );
	}
	else
	{
		strncpy( szFriendName , m_pEdit->GetWindowText() ,LEN_NAME);
		m_pEdit->SetWindowText( NULL );
	}

	UINT8	len = min( (UINT8)_tcslen( szFriendName ) , LEN_NAME );

	if( len <= 0 )
		return 1;

	std::string	strBlackListAdd;

	strBlackListAdd.clear();
	GetUpperString3( strBlackListAdd , szFriendName );

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		char		szLocalName[ LEN_NAME + 1 ];
		std::string	strLocalName;

		pLocalPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szLocalName );

		strLocalName.clear();
		GetUpperString3( strLocalName , szLocalName );

		if( strBlackListAdd.compare( strLocalName ) == 0 )
		{
			//			PrintErrorMsg( GS_FRIEND_BLACK_MYSELF );
			return 1;
		}
	}

	if( OnUserCheckFriend( (WPARAM)FRIEND_FOLLOW_TYPE_BLACK , (LPARAM)strBlackListAdd.c_str() ) == 1 )
	{
		//		PrintErrorMsg( GS_FRIEND_BLACK_ALREADY );
		return 1;
	}

	if( OnUserCheckFriend( (WPARAM)FRIEND_FOLLOW_TYPE_WHITE , (LPARAM)strBlackListAdd.c_str() ) == 1 )
	{
		//		PrintErrorMsg( GS_FRIEND_WHITE_ALREADY );
		return 1;
	}

	CPacket	packet;

	packet.AddUINT32( BLOCK_CS_ADD );
	packet.AddUINT8( len );
	packet.AddData( &szFriendName , len );

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &packet );

	m_strLastAddName	=	szFriendName;

	return 1;
}

//------------------------------------------------------------------------------------------------------
//被屏蔽
int SPWindowFriendFollow::OnBlackListDel( WPARAM wParam, LPARAM lParam )
{
	if( m_bDeletePacketSend == true )
		return 1;

	const char*	pszName	=	(const char*)wParam;

	if( m_iBlackListSelectNumber < 0 && pszName == NULL )
		return 1;

	if( pszName != NULL )
		m_strLastDelName	=	pszName;

	if( m_strLastDelName.empty() == true )
	{
		pszName	=	GetSelectFriendUserName( FRIEND_FOLLOW_TYPE_BLACK );

		if( pszName != NULL )
		{
			m_strLastDelName	=	pszName;
		}
	}

	if( m_strLastDelName.empty() == true )
		return 1;

	DECLARE_VAR( TCHAR , szFriendName , MAX_LEN_NAME );

	strncpy( szFriendName , m_strLastDelName.c_str() , LEN_NAME);

	UINT8	len = min( (UINT8)_tcslen( szFriendName ) , LEN_NAME );

	if( len <= 0 )
	{
		m_strLastDelName.clear();
		return 1;
	}

	//g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND, SPIM_FRIEND_NAME, 1, (LPARAM)&m_strLastDelName );
	CPacket	packet;

	packet.AddUINT32( BLOCK_CS_DELETE );
	packet.AddUINT8( len );
	packet.AddData( &szFriendName , len );

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &packet );

	m_bDeletePacketSend	=	true;

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnBlackLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	unsigned int	uiInstanceID;
	SPWindow*		pWindow;

	m_iBlackListSelectNumber	=	-1;

	for( int i = 0 ; i < WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER ; ++i )
	{
		uiInstanceID = WIID_FRIEND_FOLLOW_BLACK_LIST_USER_WINDOW + ( i * WIID_FRIEND_FOLLOW_BLACK_LIST_USER_STEP );

		if( uiInstanceID == iID )
		{
			m_iBlackListSelectNumber	=	i;
			continue;
		}

		pWindow	=	Find( uiInstanceID , true );

		if( pWindow == NULL )
			continue;

		pWindow->SPSendMessage( SPIM_CURSOR_OUT , 1 );
	}

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnBlackSliderUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnSliderUp( wParam , lParam );
	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFriendFollow::OnBlackSliderDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnSliderDown( wParam , lParam );
	return 1;
}

//------------------------------------------------------------------------------------------------------
//被屏蔽
int SPWindowFriendFollow::OnUserAddFriend( WPARAM wParam, LPARAM lParam )
{
	int	iType			=	(int)wParam;

	// If PVP Zone then Block
	if( g_pkPvpManager->IsPvpZone() )
	{
		char	szMsg[256];
		if( g_pResourceManager->GetGlobalString(51000021) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(51000021));
		else
			wsprintf(szMsg, "[Friend]You can't use it in PVP zone!!");
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
		return 0 ;
	}

	if( iType == FRIEND_FOLLOW_TYPE_WHITE )
		return	OnWhiteListAdd( wParam , lParam );
	else if( iType == FRIEND_FOLLOW_TYPE_BLACK )
		return	OnBlackListAdd( wParam , lParam );

	return 0;
}

//------------------------------------------------------------------------------------------------------
//被屏蔽
int SPWindowFriendFollow::OnUserDelFriend( WPARAM wParam, LPARAM lParam )
{
	int	iType	=	(int)wParam;
	const char*	pszName	=	(const char*)lParam;

	if( iType == FRIEND_FOLLOW_TYPE_WHITE )
		return	OnWhiteListDel( (WPARAM)pszName , NULL );
	else if( iType == FRIEND_FOLLOW_TYPE_BLACK )
		return	OnBlackListDel( (WPARAM)pszName , NULL );

	return 0;
}

//------------------------------------------------------------------------------------------------------
//被屏蔽
int SPWindowFriendFollow::OnUserCheckFriend( WPARAM wParam, LPARAM lParam )
{
	FRIEND_FOLLOW_TYPE	eType		=	(FRIEND_FOLLOW_TYPE)wParam;
	const char*	pszName		=	(const char*)lParam;

	if( pszName == NULL )
		return 0;

	std::string	strUserName;

	strUserName.clear();
	GetUpperString3( strUserName , pszName );

	if( strUserName.empty() == true )
		return 0;

	if( eType == FRIEND_FOLLOW_TYPE_WHITE )
	{
		STD_MAP_FRIEND_FOLLOW::iterator	mIterOffline	=	m_mpWhiteListOffline.find( strUserName );
		STD_MAP_FRIEND_FOLLOW::iterator	mIterOnline		=	m_mpWhiteListOnline.find( strUserName );

		if( mIterOnline != m_mpWhiteListOnline.end() || mIterOffline != m_mpWhiteListOffline.end() )
			return 1;
	}
	else if( eType == FRIEND_FOLLOW_TYPE_BLACK )
	{
		STD_MAP_FRIEND_FOLLOW::iterator	mIter	=	m_mpBlackList.find( strUserName );

		if( mIter != m_mpBlackList.end() )
			return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendFollow::InitWhite()
{
	m_pWhiteListPanel	=	new SPWindowStatic( WIID_FRIEND_FOLLOW_WHITE_LIST_PANEL , 0 , 0 , m_iSX , m_iSY , this );

	SPWindowButton*		pButton;

	pButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_WHITE_LIST_FIND , 154 , 34 , 68 , 17 , m_pWhiteListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 305 , 1 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 305 , 19 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 305 , 37 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 305 , 55 );
	pButton	=	NULL;

	/*pButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_WHITE_LIST_DEL , 154 , 272 , 68 , 17 , m_pWhiteListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 208 , 269 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 208 , 287 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 208 , 305 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 208 , 323 );
	pButton	=	NULL;

	pButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_WHITE_LIST_INVITATION , 10 , 272 , 68 , 17 , m_pWhiteListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" ,   1 , 269 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" ,   1 , 287 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" ,   1 , 305 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" ,   1 , 323 );
	pButton	=	NULL;

	pButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_WHITE_LIST_WHISPER , 82 , 272 , 68 , 17 , m_pWhiteListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 70 , 269 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 70 , 287 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 70 , 305 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 70 , 323 );
	pButton	=	NULL;*/

	for( int i = 0 ; i < WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER ; i++ )
	{
		m_pWhiteUser[ i ]	=	new SPWindowFriendFollowUser( WIID_FRIEND_FOLLOW_WHITE_LIST_USER_WINDOW + ( i * WIID_FRIEND_FOLLOW_WHITE_LIST_USER_STEP ) , 10 , i * 27 + 85 , 192 , 23 , m_pWhiteListPanel );
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

//------------------------------------------------------------------------------------------------------
void SPWindowFriendFollow::InitBlack()
{
	m_pBlackListPanel	=	new SPWindowStatic( WIID_FRIEND_FOLLOW_BLACK_LIST_PANEL , 0 , 0 , m_iSX , m_iSY , this );

	SPWindowButton*		pButton;

	pButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_BLACK_LIST_FIND , 154 , 34 , 68 , 17 , m_pBlackListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 305 , 1 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 305 , 19 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 305 , 37 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 305 , 55 );
	pButton	=	NULL;

	/*pButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_BLACK_LIST_DEL , 154 , 272 , 68 , 17 , m_pBlackListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 277 , 269 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 277 , 287 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 277 , 305 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 277 , 323 );
	pButton	=	NULL;*/

	int	iInstanceID	=	0;
	int	iYPos		=	0;

	for( int i = 0 ; i < WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER ; i++ )
	{
		m_pBlackUser[ i ]	=	new SPWindowFriendFollowUser( WIID_FRIEND_FOLLOW_BLACK_LIST_USER_WINDOW + ( i * WIID_FRIEND_FOLLOW_BLACK_LIST_USER_STEP ) , 10 , i * 27 + 85 , 192 , 23 , m_pBlackListPanel );
		m_pBlackUser[ i ]->SetIconImagePos( 8 , 0 );
		m_pBlackUser[ i ]->SetIconImageSize( 40 , 21 );
		m_pBlackUser[ i ]->SetIconImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 256 , 166 );	//	OnLine
		m_pBlackUser[ i ]->SetIconImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 240 , 140 );	//	OffLine
		m_pBlackUser[ i ]->SetTextPos( 59 , 6 );
		m_pBlackUser[ i ]->SetTextSize( 120 , 12 );
		m_pBlackUser[ i ]->SetTextColorOnLine( D3DCOLOR_ARGB( 255 , 70 , 140 , 124 ) );
		m_pBlackUser[ i ]->SetTextColorOffLine( D3DCOLOR_ARGB( 255 , 189 , 189 , 189 ) );
		m_pBlackUser[ i ]->ShowToolTip( false );
		m_pBlackUser[ i ]->Hide();
	}

	m_iBlackPage				=	0;
	m_iBlackListSelectNumber	=	-1;
}

//-------------------------------------------------------------------------------------------------------------
void SPWindowFriendFollow::InitRecent()
{
	m_pRecentListPanel	=	new SPWindowStatic( WIID_FRIEND_FOLLOW_RECENT_LIST_PANEL , 0 , 0 , m_iSX , m_iSY , this );

	SPWindowButton*		pButton;

	pButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_RECENT_LIST_FIND , 154 , 34 , 68 , 17 , m_pRecentListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 305 , 1 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 305 , 19 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 305 , 37 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 305 , 55 );
	pButton	=	NULL;

	/*pButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_RECENT_LIST_ADD , 154 , 272 , 68 , 17 , m_pRecentListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 391 , 368 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 391 , 386 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 391 , 404 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 391 , 422 );
	pButton	=	NULL;

	pButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_RECENT_LIST_INVITATION , 10 , 272 , 68 , 17 , m_pRecentListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" ,   1 , 269 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" ,   1 , 287 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" ,   1 , 305 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" ,   1 , 323 );
	pButton	=	NULL;

	pButton	=	new	SPWindowButton( WIID_FRIEND_FOLLOW_RECENT_LIST_WHISPER , 82 , 272 , 68 , 17 , m_pRecentListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 70 , 269 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 70 , 287 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 70 , 305 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 70 , 323 );
	pButton	=	NULL;*/

	for( int i = 0 ; i < WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER ; i++ )
	{
		m_pRecentUser[ i ]	=	new SPWindowFriendFollowUser( WIID_FRIEND_FOLLOW_RECENT_LIST_USER_WINDOW + ( i * WIID_FRIEND_FOLLOW_RECENT_LIST_USER_STEP ) , 10 , i * 27 + 85 , 192 , 23 , m_pRecentListPanel );
		m_pRecentUser[ i ]->SetIconImagePos( 8 , 0 );
		m_pRecentUser[ i ]->SetIconImageSize( 40 , 21 );
		m_pRecentUser[ i ]->SetIconImage( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 1 , 137 );	//	OnLine
		m_pRecentUser[ i ]->SetIconImage( "DATA/INTERFACE/CONCEPT/UI109.PNG" , 1 , 137 );	//	OffLine
		m_pRecentUser[ i ]->SetTextPos( 59 , 6 );
		m_pRecentUser[ i ]->SetTextSize( 120 , 12 );
		m_pRecentUser[ i ]->SetTextColorOnLine( D3DCOLOR_ARGB( 255 , 70 , 140 , 124 ) );
		m_pRecentUser[ i ]->SetTextColorOffLine( D3DCOLOR_ARGB( 255 , 189 , 189 , 189 ) );
		m_pRecentUser[ i ]->ShowToolTip( false );
		m_pRecentUser[ i ]->Hide();
	}

	m_iRecentPage				=	0;
	m_iRecentListSelectNumber	=	-1;
}

//-------------------------------------------------------------------------------------------------------------
void SPWindowFriendFollow::AddWhiteList( FRIEND_INFO* pFriendInfo )
{
	if( pFriendInfo == NULL )
		return;

	std::string	strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , pFriendInfo->szCharName );

	DelWhiteList( strFriendName.c_str() );

	if( pFriendInfo->uiServerID == 0 )		
		m_mpWhiteListOffline.insert( STD_MAP_FRIEND_FOLLOW::value_type( strFriendName , pFriendInfo ) );
	else									
		m_mpWhiteListOnline.insert( STD_MAP_FRIEND_FOLLOW::value_type( strFriendName , pFriendInfo ) );


	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		FRIEND_INFO*	pPlayerFriendInfo	=	new	FRIEND_INFO;

		memcpy( pPlayerFriendInfo , pFriendInfo , sizeof( FRIEND_INFO )  );

		pLocalPlayer->AddFriendList( pPlayerFriendInfo );
	}
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendFollow::DelWhiteList( const char* pFriendName )
{
	std::string					strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , pFriendName );

	STD_MAP_FRIEND_FOLLOW::iterator	mIterOffline	=	m_mpWhiteListOffline.find( strFriendName );
	STD_MAP_FRIEND_FOLLOW::iterator	mIterOnline		=	m_mpWhiteListOnline.find( strFriendName );

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

void SPWindowFriendFollow::DelAllWhiteList()
{
	STD_MAP_FRIEND_FOLLOW::iterator	mIter	=	m_mpWhiteListOffline.begin();

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
void SPWindowFriendFollow::UpdateWhiteList( int iPage )
{
	int	iStartNum		=	iPage	*	WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER;
	int	iOnLineCount	=	(int)m_mpWhiteListOnline.size();
	int	iOffLineCount	=	(int)m_mpWhiteListOffline.size();
	int	iListCount		=	iOnLineCount	+	iOffLineCount;
	int	iMaxPage		=	iListCount / WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER;

	if( iListCount % WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER != 0 || iMaxPage == 0 )
		iMaxPage++;

	if( iPage >= iMaxPage )
	{
		iPage	=	iMaxPage - 1;
		iStartNum	=	iPage	*	WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER;
	}

	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , iMaxPage );

	BOOL						bOnLine	=	TRUE;
	STD_MAP_FRIEND_FOLLOW::iterator	mIter	=	m_mpWhiteListOnline.begin();

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

	for( i = 0 ; i < WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER ; i++ )
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
void SPWindowFriendFollow::HideWhiteList( int iStartNum )
{
	for( int i = iStartNum ; i < WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER ; i++ )
	{
		m_pWhiteUser[ i ]->SetFriendInfo( NULL );
		m_pWhiteUser[ i ]->SetWndState( WND_STATE_NORMAL );
		m_pWhiteUser[ i ]->Hide();
	}
}

//-----------------------------------------------------------------------------------------------------
//将查找的帐号保存在文件中,只保存最近10次的
void SPWindowFriendFollow::AddRecentList( const char* pstrFriendName )
{

	if ( NULL == pstrFriendName )
	{
		return;
	}
	FILE*	pfileSaveRFName = NULL;
	RFWName	playerName;
	int		result			= 0;
	pfileSaveRFName = fopen( "RFWName.dll","r+b" );
	if ( NULL == pfileSaveRFName )
	{	
		return;
	}

	result = fseek( pfileSaveRFName, 0, SEEK_END);
	int isize = (int)ftell( pfileSaveRFName );
	int icount = isize / sizeof( playerName );

	result = fseek( pfileSaveRFName, 0, SEEK_SET );

	sprintf( playerName.RecentFindName, "%s", pstrFriendName );

	string strName = playerName.RecentFindName;
	for( int g = 0; g < m_DeqRecentName.size(); g++ )
	{
		string strPush = m_DeqRecentName.at(g).RecentFindName;
		if( strPush.compare(strName) == 0 )
		{
			result = 1;
		}
	}
	if( 10 == m_DeqRecentName.size()&& 0 == result )
	{
		m_DeqRecentName.pop_front();
		m_DeqRecentName.push_back(playerName);
	}
	else if( 0 == result )
	{
		m_DeqRecentName.push_back(playerName);
	}
	else
	{

	}

	for( int k = 0; k < m_DeqRecentName.size(); k++ )
	{
		fwrite(&m_DeqRecentName.at(k), 1, sizeof(playerName), pfileSaveRFName);
	}
	fclose(pfileSaveRFName);
}

//------------------------------------------------------------------------------------------------------
//清空最近联系人,目前没什么用
void SPWindowFriendFollow::DelAllRecentList()//liuyang
{
	m_DeqRecentName.clear();
	return;
}

//------------------------------------------------------------------------------------------------------
//更新最近联系人表
void SPWindowFriendFollow::UpdateRecentList( int iPage )
{

	int	iStartNum	=	iPage	*	WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER;
	int	iListCount	=	(int)m_DeqRecentName.size();
	int	iMaxPage	=	iListCount / WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER;

	if( iListCount % WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER != 0 || iMaxPage == 0 )
		iMaxPage++;

	if( iPage >= iMaxPage )
	{
		iPage	=	iMaxPage - 1;
		iStartNum	=	iPage	*	WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER;
	}

	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , iMaxPage );

	bool	bPageUpdate	=	false;

	if( iPage != m_iBlackPage )
		bPageUpdate	=	true;

	m_iRecentPage	=	iPage;
	m_pSlider->SetCurpage( m_iRecentPage );
	m_pSlider->Refresh();

	std::deque<RFWName>::iterator mIter	=	m_DeqRecentName.begin();

	for( int i = 0 ; i < iStartNum ; i++ )
	{
		if( mIter == m_DeqRecentName.end() )	
			break;

		++mIter;
	}

	const char*	pFriendName = NULL;

	for( i = 0 ; i < WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER ; i++ )
	{
		if( mIter == m_DeqRecentName.end() )
			break;

		pFriendName	=	 (*mIter).RecentFindName;

		if( bPageUpdate == true )
			m_pRecentUser[ i ]->SetWndState( WND_STATE_NORMAL );

		m_pRecentUser[ i ]->SetName( pFriendName );
		m_pRecentUser[ i ]->Show();

		++mIter;
	}

	HideRecentList( i );
}

//------------------------------------------------------------------------------------------------------
void SPWindowFriendFollow::HideRecentList( int iStartNum )
{
	for( int i = iStartNum ; i < WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER ; i++ )
	{
		m_pRecentUser[ i ]->SetName( NULL );
		m_pRecentUser[ i ]->SetWndState( WND_STATE_NORMAL );
		m_pRecentUser[ i ]->Hide();
	}
}
//-----------------------------------------------------------------------------------------------------
//添加黑名单
void SPWindowFriendFollow::AddBlackList( const char* pstrFriendName )
{
	if( pstrFriendName == NULL )
		return;

	std::string					strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , pstrFriendName );

	DelBlackList( strFriendName.c_str() );

	FRIEND_INFO*	pFriendInfo	=	new FRIEND_INFO;

	pFriendInfo->uiServerID	=	0;
	pFriendInfo->uiLevel	=	0;

	strcpy( pFriendInfo->szCharName , pstrFriendName );

	m_mpBlackList.insert( STD_MAP_FRIEND_FOLLOW::value_type( strFriendName , pFriendInfo ) );
}

//--------------------------------------------------------------------------------------------------------
//删除黑名单
void SPWindowFriendFollow::DelBlackList( const char* pFriendName )
{
	std::string					strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , pFriendName );

	STD_MAP_FRIEND_FOLLOW::iterator	mIter	=	m_mpBlackList.find( strFriendName );

	if( mIter != m_mpBlackList.end() )
	{
		SAFE_DELETE( (*mIter).second );
		m_mpBlackList.erase( mIter );
	}
}

void SPWindowFriendFollow::DelAllBlackList()
{
	STD_MAP_FRIEND_FOLLOW::iterator	mIter	=	m_mpBlackList.begin();

	while ( mIter != m_mpBlackList.end() )
	{
		SAFE_DELETE( (*mIter).second );
		++mIter;
	}

	m_mpBlackList.clear();
}

//-------------------------------------------------------------------------------------------------------
//更新黑名单
void SPWindowFriendFollow::UpdateBlackList( int iPage )
{
	int	iStartNum	=	iPage	*	WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER;
	int	iListCount	=	(int)m_mpBlackList.size();
	int	iMaxPage	=	iListCount / WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER;

	if( iListCount % WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER != 0 || iMaxPage == 0 )
		iMaxPage++;

	if( iPage >= iMaxPage )
	{
		iPage	=	iMaxPage - 1;
		iStartNum	=	iPage	*	WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER;
	}

	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , iMaxPage );

	bool	bPageUpdate	=	false;

	if( iPage != m_iBlackPage )
		bPageUpdate	=	true;

	m_iBlackPage	=	iPage;
	m_pSlider->SetCurpage( m_iBlackPage );
	m_pSlider->Refresh();

	STD_MAP_FRIEND_FOLLOW::iterator	mIter	=	m_mpBlackList.begin();

	for( int i = 0 ; i < iStartNum ; i++ )
	{
		if( mIter == m_mpBlackList.end() )	
			break;

		++mIter;
	}

	FRIEND_INFO*	pFriendInfo;

	for( i = 0 ; i < WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER ; i++ )
	{
		if( mIter == m_mpBlackList.end() )
			break;

		pFriendInfo	=	(FRIEND_INFO*)( (*mIter).second );

		if( bPageUpdate == true )
			m_pBlackUser[ i ]->SetWndState( WND_STATE_NORMAL );

		m_pBlackUser[ i ]->SetFriendInfo( pFriendInfo );
		m_pBlackUser[ i ]->Show();

		++mIter;
	}

	HideBlackList( i );
}

void SPWindowFriendFollow::HideBlackList( int iStartNum )
{
	for( int i = iStartNum ; i < WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER ; i++ )
	{
		m_pBlackUser[ i ]->SetFriendInfo( NULL );
		m_pBlackUser[ i ]->SetWndState( WND_STATE_NORMAL );
		m_pBlackUser[ i ]->Hide();
	}
}

//---------------------------------------------------------------------------------------------------------------
//输出错误
void SPWindowFriendFollow::PrintErrorMsg( GLOBAL_STRING_ID uiGlobalStringID , const char* pszName /* = NULL  */ )
{
	DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( uiGlobalStringID );

	if( pszGlobalString == NULL )
		return;

	if( uiGlobalStringID > FRIEND_FOLLOW_GLOBAL_MSG_ONE_METHOD && pszName != NULL )
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
void SPWindowFriendFollow::RequestStatusFriend()
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
const char* SPWindowFriendFollow::GetSelectFriendUserName( FRIEND_FOLLOW_TYPE eType )
{
	const char*	pszName	=	NULL;

	if( eType == FRIEND_FOLLOW_TYPE_NULL )
		return pszName;

	int			iSelectNumber;
	INSTANCE_ID	eStartID;
	INSTANCE_ID	eStepID;

	if( eType == FRIEND_FOLLOW_TYPE_WHITE )
	{
		iSelectNumber	=	m_iWhiteListSelectNumber;
		eStartID		=	WIID_FRIEND_FOLLOW_WHITE_LIST_USER_WINDOW;
		eStepID			=	WIID_FRIEND_FOLLOW_WHITE_LIST_USER_STEP;
	}
	else if( eType == FRIEND_FOLLOW_TYPE_BLACK )
	{
		iSelectNumber	=	m_iBlackListSelectNumber;
		eStartID		=	WIID_FRIEND_FOLLOW_BLACK_LIST_USER_WINDOW;
		eStepID			=	WIID_FRIEND_FOLLOW_BLACK_LIST_USER_STEP;
	}
	else if( eType == FRIEND_FOLLOW_TYPE_RECENT )
	{
		iSelectNumber	=	m_iRecentListSelectNumber;
		eStartID		=	WIID_FRIEND_FOLLOW_RECENT_LIST_USER_WINDOW;
		eStepID			=	WIID_FRIEND_FOLLOW_RECENT_LIST_USER_STEP;
	}

	INSTANCE_ID uiInstanceID = eStartID + ( iSelectNumber * eStepID );

	SPWindowFriendFollowUser*	pWindowFriendFollowUser	=	(SPWindowFriendFollowUser*)( Find( uiInstanceID , true ) );

	if( pWindowFriendFollowUser != NULL )
	{
		pszName	=	pWindowFriendFollowUser->GetName();
	}
	return pszName;
}

//-------------------------------------------------------------------------------
//接受SPWindowFriend传过来的删除了的名字
int SPWindowFriendFollow::OnDelFriendName( WPARAM wParam, LPARAM lParam )
{
	if ( NULL == lParam )
	{
		return 1;
	}

	m_strLastDelName = *(string*)lParam;
	return 1;
}

//-------------------------------------------------------------------------------------------
//初始化最近查找保存的文件,并将文件里的数据储存
void SPWindowFriendFollow::InitFile()
{
	FILE*   pfileGetRFName = NULL;
	int		result        = 0;
	RFWName	playerName;

	HANDLE hFile = ::CreateFile( 
		"RFWName.dll",
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_WRITE, 
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL );
	::CloseHandle( hFile );

	pfileGetRFName = fopen( "RFWName.dll", "r+b");

	if ( NULL == pfileGetRFName )
	{
		return;
	}

	result = fseek( pfileGetRFName, 0, SEEK_END );
	int isize = (int)ftell( pfileGetRFName );

	result = fseek( pfileGetRFName, 0, SEEK_SET );
	int icount = isize / sizeof(playerName);

	for( int j = 0; j < icount ; j++)
	{
		result = fread( &playerName, sizeof(playerName), 1, pfileGetRFName );
		m_DeqRecentName.push_back( playerName );
	}
	fclose( pfileGetRFName );
	return;
}

//--------------------------------------------------------------------------------------------------------------------------------
//暂时不用
int SPWindowFriendFollow::OnUserFindFriend( WPARAM wParam, LPARAM lParam )
{
	int	iType			=	(int)wParam;

	if( iType == FRIEND_FOLLOW_TYPE_WHITE )
	{
		return	OnWhitePeopleFind( wParam , lParam );
	}
	else if( iType == FRIEND_FOLLOW_TYPE_BLACK )
	{
		return	OnBlackPeopleFind( wParam , lParam );
	}
	else if( iType == FRIEND_FOLLOW_TYPE_RECENT )
	{
		return  OnRecentPeopleFind( wParam, lParam );
	}
	return 1;
}

int SPWindowFriendFollow::OnFollowResult( WPARAM wParam, LPARAM lParam )
{
	CPacket pPacket = *(CPacket*)lParam;
	CPacket* pPacketCopy = &pPacket;
	UINT32 iItemID  = 0;
	pPacketCopy->ExtractUINT32( &iItemID );
	GLOBAL_STRING_ID iError = 0;
	pPacketCopy->ExtractUINT32( &iError );
	if ( 170200602 == iItemID )
	{
		if ( 0 == iError )
		{
			AddRecentList( m_FollowName.c_str() );
			UpdateRecentList( m_iRecentPage );
		}
		else
		{
			std::string strMsg = "无法跟随对方";				//~
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() ); //~~
		}
		Hide();
	}
	
	return 1;
}

void SPWindowFriendFollow::StartToFollow()
{
	if(g_pGOBManager->GetLocalPlayer()) 
	{
		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();

		if(pInven)
		{
			pInven->FollowPeople( m_FollowName );
		}
	}
}