// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-8-30    11:30 
//***************************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowSlider.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
//#include "SPWindowList.h"
#include "SPWindowFriendUser.h"
#include "SPWindowChat.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include <algorithm>

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

#include "SPWindowFriend.h"

SPWindowFriend::SPWindowFriend(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowFriend::SPWindowFriend(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_FRIEND, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowFriend::~SPWindowFriend()
{
	Clean();
}

void SPWindowFriend::Init()
{
	m_strToolTip		=	"[SPWindowFriend]";
	m_eFriendType		=	FRIEND_TYPE_WHITE;
	m_bDeletePacketSend	=	false;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , &m_pBGTexture );

	ReposOwnImage(); // metalgeni [5/25/2006]

	SetRect( &m_rcBGSrc[0]	, 1 , 31 , 233 , 35 );			//	¹è°æ »ó´Ü
	SetRect( &m_rcBGSrc[1]	, 1 , 37 , 233 , 39 );			//	¹è°æ Áß´Ü
	SetRect( &m_rcBGSrc[2]	, 1 , 41 , 233 , 45 );			//	¹è°æ ÇÏ´Ü
	SetRect( &m_rcBGSrc[3]	, 349 , 312 , 355 , 318 );		//	¹è°æ ¿¶Àº ÃÊ·Ï»ö
	SetRect( &m_rcBGSrc[4]	, 349 , 329 , 355 , 330 );		//	Â£Àº ÃÊ·Ï °æ°è¼± 1
	SetRect( &m_rcBGSrc[5]	, 349 , 329 , 355 , 330 );		//	Â£Àº ÃÊ·Ï °æ°è¼± 2
	SetRect( &m_rcBGSrc[6]	, 349 , 321 , 355 , 327 );		//	Áß¾Ó Èò»ö ¼­ºê ¹é±×¶ó¿îµå

	SPWindowStatic*	pWindowStatic;

	pWindowStatic	=	new SPWindowStatic( WIID_FRIEND_TITLE_IMG , 6 , 3 , 32 , 23 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 125 , 343 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_FRIEND_TITLE_TEXT , 42 , 7 , 63 , 15 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 265 , 367 );
	pWindowStatic	=	NULL;

	SPWindowButton*	pWindowButton;

	pWindowButton	=	new	SPWindowButton( WIID_FRIEND_CLOSE , 211 , 9 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 217 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 230 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 243 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 256 );
	pWindowButton	=	NULL;

	m_pWhiteButton	=	new	SPWindowButton( WIID_FRIEND_WHITE_LIST_BUTTON , 10 , 55 , 64 , 15 , this );
	m_pWhiteButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 384 );
	m_pWhiteButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 400 );
	m_pWhiteButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 416 );
	m_pWhiteButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 432 );
	m_pWhiteButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 196 , 416 );

	m_pBlackButton	=	new	SPWindowButton( WIID_FRIEND_BLACK_LIST_BUTTON , 73 , 55 , 64 , 15 , this );
	m_pBlackButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 261 , 384 );
	m_pBlackButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 261 , 400 );
	m_pBlackButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 261 , 416 );
	m_pBlackButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 261 , 432 );
	m_pBlackButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 261 , 416 );

	pWindowButton	=	new	SPWindowButton( WIID_FRIEND_SLIDER_UP , 210 , 85 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_FRIEND_SLIDER_DOWN , 210 , 246 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );
	pWindowButton =	NULL;

	RECT rtRect;
	m_pSlider	=	new	SPWindowSlider( WIID_FRIEND_SLIDER , 210 , 96 , 12 , 150 , this );

	SetRect( &rtRect , 460 , 470 , 472 , 494 );	//	ÀÏ¹Ý
	m_pSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 473 , 470 , 485 , 494 );	//	¸¶¿ì½º ¿À¹ö
	m_pSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 486 , 470 , 498 , 494 );	//	¸¶¿ì½º Å¬¸¯
	m_pSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 499 , 470 , 511 , 494 );
	m_pSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 460 , 496 , 472 , 498 );	//	ÀÏ¹Ý
	m_pSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 473 , 496 , 485 , 498 );	//	¸¶¿ì½º ¿À¹ö
	m_pSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 486 , 496 , 498 , 498 );	//	¸¶¿ì½º Å¬¸¯
	m_pSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 499 , 496 , 511 , 498 );
	m_pSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );


	m_pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pSlider->SetCurpage( 0 );
	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );

	m_pEdit	=	new SPWindowEdit( WIID_FRIEND_EDIT , 10 , 34 , 140 , 17 , this );
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

	m_iSendFriendPos	=	0;
}


void SPWindowFriend::ReposOwnImage()
{
	SetRect( &m_rcBGDest[0]	, m_iAX					, m_iAY					, m_iAX + 232			, m_iAY + 4 );						//	¹è°æ »ó´Ü
	SetRect( &m_rcBGDest[1]	, m_rcBGDest[0].left	, m_rcBGDest[0].bottom	, m_rcBGDest[0].right	, m_rcBGDest[0].bottom + 287 );		//	¹è°æ Áß´Ü
	SetRect( &m_rcBGDest[2]	, m_rcBGDest[1].left	, m_rcBGDest[1].bottom	, m_rcBGDest[1].right	, m_rcBGDest[1].bottom + 4 );		//	¹è°æ ÇÏ´Ü
	SetRect( &m_rcBGDest[3]	, m_iAX + 2				, m_iAY + 26			, m_iAX + 2 + 228		, m_iAY + 26	+ 48 );				//	¹è°æ ¿¶Àº ÃÊ·Ï»ö
	SetRect( &m_rcBGDest[4]	, m_iAX + 2				, m_iAY + 74			, m_iAX + 2 + 228		, m_iAY + 74	+ 1 );				//	Â£Àº ÃÊ·Ï °æ°è¼± 1
	SetRect( &m_rcBGDest[5]	, m_iAX + 2				, m_iAY + 267			, m_iAX + 2 + 228		, m_iAY + 267	+ 1 );				//	Â£Àº ÃÊ·Ï °æ°è¼± 2
	SetRect( &m_rcBGDest[6]	, m_iAX + 2				, m_iAY + 75			, m_iAX + 2 + 228		, m_iAY + 75	+ 192 );			//	Áß¾Ó Èò»ö ¼­ºê ¹é±×¶ó¿îµå
}


void SPWindowFriend::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowFriend::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );

	DelAllWhiteList();
	DelAllBlackList();
}

void SPWindowFriend::Show(bool bWithChild)
{
	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Show(bWithChild);

	HideWhiteList( 0 );
	HideBlackList( 0 );

	if( m_eFriendType == FRIEND_TYPE_WHITE )
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
	else if( m_eFriendType == FRIEND_TYPE_BLACK )
	{
		OnBlackList( 1 , NULL );
	}
}

void SPWindowFriend::Hide(bool bSendServer)
{
	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide(bSendServer);

	if( m_eFriendType == FRIEND_TYPE_WHITE )	//	WhiteList
	{
		OnWhiteLButtonUp( 0 , NULL , NULL );
	}
	else if( m_eFriendType == FRIEND_TYPE_BLACK )	//	BlackList
	{
		OnBlackLButtonUp( 0 , NULL , NULL );
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

void SPWindowFriend::Process(float fTime)
{
	SPWindow::Process( fTime );

	if( m_bShow == false || m_eFriendType != FRIEND_TYPE_WHITE ||
		m_fRefreshLimit == 0.0f || m_fRefreshAccmulateTime < 0.0f )
		return;
        
	m_fRefreshAccmulateTime	+=	fTime;

	if( m_fRefreshAccmulateTime >= m_fRefreshLimit )
		RequestStatusFriend();
}

void SPWindowFriend::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < FRIEND_BG_COUNT ; i++ )
		{
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	SPWindow::Render(fTime);
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowFriend )
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
	SPIMESSAGE_COMMAND( SPIM_ADD_RECV_FRIEND	,										OnAddRecvFriend		)
	SPIMESSAGE_COMMAND( SPIM_ACCEPT_FRIEND		,										OnAcceptFriend		)
	SPIMESSAGE_COMMAND( SPIM_DEL_FRIEND			,										OnDelFriend			)
	SPIMESSAGE_COMMAND( SPIM_FRIEND_NAME 	    ,										OnDelFriendName		)

	SPIMESSAGE_COMMAND( SPIM_REFRESH_STATUS_FRIEND	,									OnStatusFriend		)

	SPIMESSAGE_COMMAND( SPIM_NOTICE_YES			,										OnNoticeYes			)
	SPIMESSAGE_COMMAND( SPIM_NOTICE_NO			,										OnNoticeNo			)

	SPIMESSAGE_COMMAND( SPIM_WHEEL_UP			,										OnSliderUp			)
	SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN			,										OnSliderDown		)

	SPIMESSAGE_COMMAND( SPIM_USER_ADD_FRIEND	,										OnUserAddFriend		)
	SPIMESSAGE_COMMAND( SPIM_USER_DEL_FRIEND	,										OnUserDelFriend		)
	SPIMESSAGE_COMMAND( SPIM_USER_CHECK_FRIEND	,										OnUserCheckFriend	)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_CLOSE				,	OnClose				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_WHITE_LIST_BUTTON	,	OnWhiteList			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_BLACK_LIST_BUTTON	,	OnBlackList			)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_SLIDER_UP			,	OnSliderUp			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_SLIDER_DOWN			,	OnSliderDown		)

	SPIMESSAGE_CONTROL( SPIM_SLIDER_UP			,	WIID_FRIEND_SLIDER				,	OnSlider			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN		,	WIID_FRIEND_SLIDER				,	OnSlider			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP		,	WIID_FRIEND_SLIDER				,	OnSlider			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN	,	WIID_FRIEND_SLIDER				,	OnSlider			)
	SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE		,	WIID_FRIEND_SLIDER				,	OnSlider			)

	SPIMESSAGE_CONTROL(	SPIM_WHEEL_UP			,	WIID_FRIEND_WHITE_LIST_PANEL	,	OnSliderUp			)
	SPIMESSAGE_CONTROL(	SPIM_WHEEL_DOWN			,	WIID_FRIEND_WHITE_LIST_PANEL	,	OnSliderDown		)

	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS			,	WIID_FRIEND_EDIT				,	OnEditSetFocus		)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS			,	WIID_FRIEND_EDIT				,	OnEditKillFocus		)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_WHITE_LIST_ADD		,	OnWhiteListAdd		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_WHITE_LIST_DEL		,	OnWhiteListDel		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_WHITE_LIST_INVITATION,	OnWhiteListInvitation)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_WHITE_LIST_WHISPER	,	OnWhiteListWhisper	)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	,	WIID_FRIEND_WHITE_LIST_USER_WINDOW	,	WIID_FRIEND_WHITE_LIST_USER_WINDOW + WIID_FRIEND_WHITE_LIST_MAX_USER * WIID_FRIEND_WHITE_LIST_USER_STEP	,	OnWhiteLButtonUp	)

	SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP		,	WIID_FRIEND_WHITE_LIST_USER_WINDOW	,	WIID_FRIEND_WHITE_LIST_USER_WINDOW + WIID_FRIEND_WHITE_LIST_MAX_USER * WIID_FRIEND_WHITE_LIST_USER_STEP	,	OnWhiteSliderUp		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN	,	WIID_FRIEND_WHITE_LIST_USER_WINDOW	,	WIID_FRIEND_WHITE_LIST_USER_WINDOW + WIID_FRIEND_WHITE_LIST_MAX_USER * WIID_FRIEND_WHITE_LIST_USER_STEP	,	OnWhiteSliderDown	)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_BLACK_LIST_ADD		,	OnBlackListAdd		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FRIEND_BLACK_LIST_DEL		,	OnBlackListDel		)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	,	WIID_FRIEND_BLACK_LIST_USER_WINDOW	,	WIID_FRIEND_BLACK_LIST_USER_WINDOW + WIID_FRIEND_BLACK_LIST_MAX_USER * WIID_FRIEND_BLACK_LIST_USER_STEP	,	OnBlackLButtonUp	)

	SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP		,	WIID_FRIEND_BLACK_LIST_USER_WINDOW	,	WIID_FRIEND_BLACK_LIST_USER_WINDOW + WIID_FRIEND_BLACK_LIST_MAX_USER * WIID_FRIEND_BLACK_LIST_USER_STEP	,	OnBlackSliderUp		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN	,	WIID_FRIEND_BLACK_LIST_USER_WINDOW	,	WIID_FRIEND_BLACK_LIST_USER_WINDOW + WIID_FRIEND_BLACK_LIST_MAX_USER * WIID_FRIEND_BLACK_LIST_USER_STEP	,	OnBlackSliderDown	)


SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowFriend::OnCursorEnter( WPARAM wParam, LPARAM lParam )
{
	if( m_eFriendType == FRIEND_TYPE_WHITE )
	{
	}

	return 1;
}

int SPWindowFriend::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowFriend::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowFriend::OnLButtonUp( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowFriend::OnLButtonDown( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowFriend::OnRButtonUp( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowFriend::OnKeyReturn( WPARAM wParam, LPARAM lParam )
{
	if( m_pEdit->IsFocus() == TRUE )
	{
		OnUserAddFriend( (WPARAM)m_eFriendType , NULL );

		return 1;
	}

	return -1;
}

int SPWindowFriend::OnPurge( WPARAM wParam, LPARAM lParam )
{
	m_eFriendType	=	FRIEND_TYPE_WHITE;

	DelAllWhiteList();
	DelAllBlackList();

	return 1;
}

int SPWindowFriend::OnGetFriend( WPARAM wParam, LPARAM lParam )
{
	FRIEND_TYPE		eFriendType	=	(FRIEND_TYPE)wParam;
	CPacket*		pPacket		=	(CPacket*)lParam;
	UINT8			uiCount		=	0;
	FRIEND_INFO*	pFriendInfo	=	NULL;
	DECLARE_VAR( TCHAR , szBlockName , MAX_LEN_NAME );

	if( eFriendType == FRIEND_TYPE_WHITE )
	{
		DelAllWhiteList();
	}
	else if( eFriendType == FRIEND_TYPE_BLACK )
	{
		DelAllBlackList();
	}

	pPacket->ExtractUINT8( &uiCount );

	for( UINT8 i = 0 ; i < uiCount ; ++i )
	{
		switch( eFriendType )
		{
		case FRIEND_TYPE_WHITE:
			pFriendInfo	=	new FRIEND_INFO;
			pPacket->ExtractStruct( pFriendInfo , sizeof(FRIEND_INFO) );
			AddWhiteList( pFriendInfo );
			break;
		case FRIEND_TYPE_BLACK:
			pPacket->ExtractData( (TCHAR*)&szBlockName , sizeof(TCHAR) * LEN_NAME );
			AddBlackList( szBlockName );
			break;
		}
	}

	if( m_eFriendType == FRIEND_TYPE_WHITE )
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
	else if( m_eFriendType == FRIEND_TYPE_BLACK )
	{
		UpdateBlackList( m_iBlackPage );
	}

	return 1;
}

int SPWindowFriend::OnAddFriend( WPARAM wParam, LPARAM lParam )
{
	FRIEND_TYPE			eFriendType			=	(FRIEND_TYPE)wParam;
	CPacket*			pPacket				=	(CPacket*)lParam;
	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	if( pPacket == NULL || eFriendType == FRIEND_TYPE_NULL )
		return 1;

	pPacket->Extract( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		PrintErrorMsg( uiGlobalStringID , m_strLastAddName.c_str() );
		return 1;
	}

	if( eFriendType == FRIEND_TYPE_WHITE )
	{
		if( 0 )
		{
			FRIEND_INFO*		pFriendInfo			=	new	FRIEND_INFO;

			pPacket->ExtractStruct( pFriendInfo , sizeof(FRIEND_INFO) );

			PrintErrorMsg( GS_FRIEND_WHITE_ADD , pFriendInfo->szCharName );
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

			PrintErrorMsg( GS_FRIEND_WHITE_ADD , szAccepter );
		}
	}
	else if( eFriendType == FRIEND_TYPE_BLACK )
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

		PrintErrorMsg( GS_FRIEND_BLACK_ADD , strFriendName.c_str() );
		AddBlackList( strFriendName.c_str() );
		UpdateBlackList( m_iBlackPage );
	}

	m_strLastAddName.clear();

	return 1;
}

int SPWindowFriend::OnAddRecvFriend( WPARAM wParam, LPARAM lParam )
{
	FRIEND_TYPE			eFriendType			=	(FRIEND_TYPE)wParam;
	CPacket*			pPacket				=	(CPacket*)lParam;
	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	if( pPacket == NULL || eFriendType == FRIEND_TYPE_NULL )
		return 1;

	pPacket->Extract( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		PrintErrorMsg( uiGlobalStringID , m_strLastAddName.c_str() );
		return 1;
	}

	if( eFriendType == FRIEND_TYPE_WHITE )
	{
		g_pInterfaceManager->ForceReply();

		UINT8				uiRequesterLen				=	0;

		memset( m_szRequester , 0 , sizeof( m_szRequester ) );
		m_iRequesterUserID	=	0;
		m_iRequesterCharID	=	0;

		pPacket->ExtractStruct( &m_iRequesterUserID , sizeof( m_iRequesterUserID ) );
		pPacket->ExtractStruct( &m_iRequesterCharID , sizeof( m_iRequesterCharID ) );

		pPacket->ExtractUINT8( &uiRequesterLen );

		if( uiRequesterLen > 0 )
			pPacket->ExtractData( m_szRequester , uiRequesterLen );

		DECLARE_VAR( TCHAR , szNoticeMsg , MAX_CHATMSG_LEN );

		const char*	pszGlobalString	=	g_pResourceManager->GetGlobalString( GS_FRIEND_WHITE_ACCEPT );

		if( pszGlobalString != NULL )
			sprintf( szNoticeMsg , pszGlobalString , m_szRequester );

		g_pInterfaceManager->SetNotice( szNoticeMsg , this );
	}

	return 1;
}

int SPWindowFriend::OnAcceptFriend( WPARAM wParam, LPARAM lParam )
{
	FRIEND_TYPE			eFriendType			=	(FRIEND_TYPE)wParam;
	CPacket*			pPacket				=	(CPacket*)lParam;
	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	if( pPacket == NULL || eFriendType == FRIEND_TYPE_NULL )
		return 1;

	pPacket->Extract( &uiGlobalStringID );

	FRIEND_INFO*		pFriendInfo			=	new	FRIEND_INFO;

	pPacket->ExtractStruct( pFriendInfo , sizeof(FRIEND_INFO) );

	if( uiGlobalStringID != 0 )
	{
		PrintErrorMsg( uiGlobalStringID , pFriendInfo->szCharName );
		delete pFriendInfo;
		return 1;
	}

	if( eFriendType == FRIEND_TYPE_WHITE )//¼Ó
	{
		PrintErrorMsg( GS_FRIEND_WHITE_ACCEPT_YES , pFriendInfo->szCharName );
		//Ìí¼ÓºÃÓÑ
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->OnCommunityEventA(ceAddFriend, pFriendInfo->szCharName );
		}
		AddWhiteList( pFriendInfo );
		UpdateWhiteList( m_iWhitePage );

		m_fRefreshAccmulateTime	=	0.0f;
	}

	return 1;
}

int SPWindowFriend::OnDelFriend( WPARAM wParam, LPARAM lParam )
{
	FRIEND_TYPE			eFriendType			=	(FRIEND_TYPE)wParam;
	CPacket*			pPacket				=	(CPacket*)lParam;
	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	m_bDeletePacketSend						=	false;
	
	pPacket->Extract( &uiGlobalStringID );
	
	if( uiGlobalStringID != 0 )
	{
		PrintErrorMsg( uiGlobalStringID , m_strLastDelName.c_str() );
		return 1;
	}
	
	if( eFriendType == FRIEND_TYPE_WHITE )	//	WhiteList
	{
		PrintErrorMsg( GS_FRIEND_WHITE_DEL , m_strLastDelName.c_str() );
		//É¾³ýºÃÓÑ
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->OnCommunityEventA(ceRemoveFriend, m_strLastDelName.c_str() );
		}
		DelWhiteList( m_strLastDelName.c_str() );
		UpdateWhiteList( m_iWhitePage );
		OnWhiteLButtonUp( 0 , NULL , NULL );

		int	iWhiteCount	=	(int)m_mpWhiteListOnline.size() + (int)m_mpWhiteListOffline.size();

		if( iWhiteCount <= 0 )
			m_fRefreshAccmulateTime	=	-1.0f;
	}
	else if( eFriendType == FRIEND_TYPE_BLACK )	//	BlackList
	{
		PrintErrorMsg( GS_FRIEND_BLACK_DEL , m_strLastDelName.c_str() );
		DelBlackList( m_strLastDelName.c_str() );
		UpdateBlackList( m_iBlackPage );
		OnBlackLButtonUp( 0 , NULL , NULL );
	}

	m_strLastDelName.clear();

	return 1;
}

int SPWindowFriend::OnStatusFriend( WPARAM wParam, LPARAM lParam )
{
	CPacket*			pPacket				=	(CPacket*)lParam;

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

int SPWindowFriend::OnNoticeYes( WPARAM wParam, LPARAM lParam )
{
	SendAcceptPacket( 2 );

	return 1;
}

int SPWindowFriend::OnNoticeNo( WPARAM wParam, LPARAM lParam )
{
	int	iForceHide	=	(int)lParam;

	if( iForceHide < 0 )		// °­Á¦·Î ´ÝÈú¶§ [4/24/2007 AJJIYA]
		SendAcceptPacket( 0 );
	else
		SendAcceptPacket( 1 );

	return 1;
}

int SPWindowFriend::OnClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

int SPWindowFriend::OnWhiteList( WPARAM wParam, LPARAM lParam )
{
	// Friend List
	//{
	//	CPacket packet;
	//	packet.AddUINT32(FRIEND_CS_GETLIST);
	//	(g_pNetworkManager->GetPacketHandler())->PostMsg(&packet);
	//}

	int	iType	=	(int)wParam;

	if( iType == 0 )
	{
		CPacket	FriendPacket;
		FriendPacket.AddUINT32( FRIEND_CS_GETLIST );
		( g_pNetworkManager->GetPacketHandler() )->PostMsg( &FriendPacket );
	}

	m_pBlackListPanel->Hide();
	m_pWhiteListPanel->Show();

	m_pBlackButton->SPSendMessage( SPIM_BTN_SELECT , FALSE );
	m_pWhiteButton->SPSendMessage( SPIM_BTN_SELECT , TRUE );

	m_pEdit->SetWindowText( NULL );

	m_eFriendType				=	FRIEND_TYPE_WHITE;
	m_iWhiteListSelectNumber	=	-1;
	m_strLastAddName.clear();
	m_strLastDelName.clear();

	UpdateWhiteList( m_iWhitePage );

	// for testing (ADD)
	//{
	//	static i = 0;
	//	DECLARE_VAR(TCHAR,szFriendName,MAX_LEN_NAME);
	//	CPacket packet;
	//	packet.AddUINT32(FRIEND_CS_ADD);
	//	_stprintf(szFriendName,"Ä£±¸_%d",i++);

	//	UINT8 len = min(_tcslen(szFriendName),LEN_NAME);
	//	packet.AddUINT8(len);
	//	packet.AddData(&szFriendName,len);

	//	(g_pNetworkManager->GetPacketHandler())->PostMsg(&packet);
	//}


	return 1;
}

int SPWindowFriend::OnBlackList( WPARAM wParam, LPARAM lParam )
{
	// Block List
	//CPacket packet;
	//packet.AddUINT32(BLOCK_CS_GETLIST);
	//(g_pNetworkManager->GetPacketHandler())->PostMsg(&packet);

	int	iType	=	(int)wParam;

	if( iType == 0 )
	{
		CPacket	BlockPacket;
		BlockPacket.AddUINT32( BLOCK_CS_GETLIST );
		( g_pNetworkManager->GetPacketHandler() )->PostMsg( &BlockPacket );
	}

	m_pWhiteListPanel->Hide();
	m_pBlackListPanel->Show();

	m_pWhiteButton->SPSendMessage( SPIM_BTN_SELECT , FALSE );
	m_pBlackButton->SPSendMessage( SPIM_BTN_SELECT , TRUE );

	m_pEdit->SetWindowText( NULL );

	m_eFriendType				=	FRIEND_TYPE_BLACK;
	m_iBlackListSelectNumber	=	-1;
	m_strLastAddName.clear();
	m_strLastDelName.clear();

	UpdateBlackList( m_iBlackPage );

	// for testing (ADD)
	//{
	//	static i = 0;
	//	DECLARE_VAR(TCHAR,szBlockName,MAX_LEN_NAME);
	//	CPacket packet;
	//	packet.AddUINT32(BLOCK_CS_ADD);
	//	_stprintf(szBlockName,"Â÷´Ü_%d",i++);

	//	UINT8 len = min(_tcslen(szBlockName),LEN_NAME);
	//	packet.AddUINT8(len);
	//	packet.AddData(&szBlockName,len);

	//	(g_pNetworkManager->GetPacketHandler())->PostMsg(&packet);
	//}

	return 1;
}

int SPWindowFriend::OnSliderUp( WPARAM wParam, LPARAM lParam )
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 1;
}

int SPWindowFriend::OnSliderDown( WPARAM wParam, LPARAM lParam )
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 1;
}

int SPWindowFriend::OnSlider( WPARAM wParam, LPARAM lParam )
{
	int	iNewPage	=	(int)wParam;

	if( m_eFriendType == FRIEND_TYPE_WHITE )
	{
		m_iWhiteListSelectNumber	=	-1;
        UpdateWhiteList( iNewPage );
		OnWhiteLButtonUp( 0 , NULL , NULL );
	}
	else if( m_eFriendType == FRIEND_TYPE_BLACK )
	{
		m_iBlackListSelectNumber	=	-1;
		UpdateBlackList( iNewPage );
		OnBlackLButtonUp( 0 , NULL , NULL );
	}

	return 1;
}

int SPWindowFriend::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}

int SPWindowFriend::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}

int SPWindowFriend::OnWhiteListAdd( WPARAM wParam, LPARAM lParam )
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

	if( OnUserCheckFriend( (WPARAM)FRIEND_TYPE_WHITE , (LPARAM)strFriendName.c_str() ) == 1 )
	{
		PrintErrorMsg( GS_FRIEND_WHITE_ALREADY );
		return 1;
	}

	if( OnUserCheckFriend( (WPARAM)FRIEND_TYPE_BLACK , (LPARAM)strFriendName.c_str() ) == 1 )
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

int SPWindowFriend::OnWhiteListDel( WPARAM wParam, LPARAM lParam )
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
		pszName	=	GetSelectFriendUserName( FRIEND_TYPE_WHITE );

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
	
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND, SPIM_FRIEND_NAME, 1, (LPARAM)&m_strLastDelName );//liuyang
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FOLLOW, SPIM_FRIEND_NAME, 1, (LPARAM)&m_strLastDelName );//liuyang
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_SNATCH, SPIM_FRIEND_NAME, 1, (LPARAM)&m_strLastDelName );//liuyang

	CPacket	packet;

	packet.AddUINT32( FRIEND_CS_DELETE );
	packet.AddUINT8( len );
	packet.AddData( &szFriendName , len );

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &packet );

	m_bDeletePacketSend	=	true;

	return 1;
}

int SPWindowFriend::OnWhiteListInvitation( WPARAM wParam, LPARAM lParam )
{
	const char*	pszName	=	GetSelectFriendUserName( FRIEND_TYPE_WHITE );

	if( pszName == NULL )
		return 1;

	std::string	strName		=	pszName;
	SPPlayer* pkLocalPlayer =	g_pGOBManager->GetLocalPlayer();

	if( pkLocalPlayer != NULL && strName.empty() == FALSE )
		pkLocalPlayer->SPGOBPerformMessage( SPGM_TEAM_INVITE_REQ , (LPARAM)strName.c_str() );

	return 1;
}

int SPWindowFriend::OnWhiteListWhisper( WPARAM wParam, LPARAM lParam )
{
	const char*	pszName	=	GetSelectFriendUserName( FRIEND_TYPE_WHITE );

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

int SPWindowFriend::OnWhiteLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	unsigned int	uiInstanceID;
	SPWindow*		pWindow;

	m_iWhiteListSelectNumber	=	-1;

	for( int i = 0 ; i < WIID_FRIEND_WHITE_LIST_MAX_USER ; ++i )
	{
		uiInstanceID = WIID_FRIEND_WHITE_LIST_USER_WINDOW + ( i * WIID_FRIEND_WHITE_LIST_USER_STEP );

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

int SPWindowFriend::OnWhiteSliderUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnSliderUp( wParam , lParam );
	return 1;
}

int SPWindowFriend::OnWhiteSliderDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnSliderDown( wParam , lParam );
	return 1;
}

int SPWindowFriend::OnBlackListAdd( WPARAM wParam, LPARAM lParam )
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
			PrintErrorMsg( GS_FRIEND_BLACK_MYSELF );
			return 1;
		}
	}

	if( OnUserCheckFriend( (WPARAM)FRIEND_TYPE_BLACK , (LPARAM)strBlackListAdd.c_str() ) == 1 )
	{
		PrintErrorMsg( GS_FRIEND_BLACK_ALREADY );
		return 1;
	}

	if( OnUserCheckFriend( (WPARAM)FRIEND_TYPE_WHITE , (LPARAM)strBlackListAdd.c_str() ) == 1 )
	{
		PrintErrorMsg( GS_FRIEND_WHITE_ALREADY );
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

int SPWindowFriend::OnBlackListDel( WPARAM wParam, LPARAM lParam )
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
		pszName	=	GetSelectFriendUserName( FRIEND_TYPE_BLACK );

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
	
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND, SPIM_FRIEND_NAME, 1, (LPARAM)&m_strLastDelName );//liuyang
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FOLLOW, SPIM_FRIEND_NAME, 1, (LPARAM)&m_strLastDelName );//liuyang

	CPacket	packet;

	packet.AddUINT32( BLOCK_CS_DELETE );
	packet.AddUINT8( len );
	packet.AddData( &szFriendName , len );

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &packet );

	m_bDeletePacketSend	=	true;

	return 1;
}

int SPWindowFriend::OnBlackLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	unsigned int	uiInstanceID;
	SPWindow*		pWindow;

	m_iBlackListSelectNumber	=	-1;

	for( int i = 0 ; i < WIID_FRIEND_BLACK_LIST_MAX_USER ; ++i )
	{
		uiInstanceID = WIID_FRIEND_BLACK_LIST_USER_WINDOW + ( i * WIID_FRIEND_BLACK_LIST_USER_STEP );

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

int SPWindowFriend::OnBlackSliderUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnSliderUp( wParam , lParam );
	return 1;
}

int SPWindowFriend::OnBlackSliderDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnSliderDown( wParam , lParam );
	return 1;
}

int SPWindowFriend::OnUserAddFriend( WPARAM wParam, LPARAM lParam )
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

	//	È­ÀÌÆ® ¸®½ºÆ® À¯Àú Ãß°¡
	if( iType == FRIEND_TYPE_WHITE )
		return	OnWhiteListAdd( wParam , lParam );
	//	ºí·¢ ¸®½ºÆ® À¯Àú Ãß°¡
	else if( iType == FRIEND_TYPE_BLACK )
		return	OnBlackListAdd( wParam , lParam );

	return 0;
}

int SPWindowFriend::OnUserDelFriend( WPARAM wParam, LPARAM lParam )
{
	int	iType	=	(int)wParam;
	const char*	pszName	=	(const char*)lParam;

	//	È­ÀÌÆ® ¸®½ºÆ® À¯Àú »èÁ¦
	if( iType == FRIEND_TYPE_WHITE )
		return	OnWhiteListDel( (WPARAM)pszName , NULL );
	//	ºí·¢ ¸®½ºÆ® À¯Àú »èÁ¦
	else if( iType == FRIEND_TYPE_BLACK )
		return	OnBlackListDel( (WPARAM)pszName , NULL );

	return 0;
}

int SPWindowFriend::OnUserCheckFriend( WPARAM wParam, LPARAM lParam )
{
	FRIEND_TYPE	eType		=	(FRIEND_TYPE)wParam;
	const char*	pszName		=	(const char*)lParam;

	if( pszName == NULL )
		return 0;

	std::string	strUserName;

	strUserName.clear();
	GetUpperString3( strUserName , pszName );

	if( strUserName.empty() == true )
		return 0;

	//	È­ÀÌÆ® ¸®½ºÆ® À¯Àú Ã¼Å©
	if( eType == FRIEND_TYPE_WHITE )
	{
		STD_MAP_FRIEND::iterator	mIterOffline	=	m_mpWhiteListOffline.find( strUserName );
		STD_MAP_FRIEND::iterator	mIterOnline		=	m_mpWhiteListOnline.find( strUserName );

		if( mIterOnline != m_mpWhiteListOnline.end() || mIterOffline != m_mpWhiteListOffline.end() )
			return 1;
	}
	//	ºí·¢ ¸®½ºÆ® À¯Àú Ã¼Å©
	else if( eType == FRIEND_TYPE_BLACK )
	{
		STD_MAP_FRIEND::iterator	mIter	=	m_mpBlackList.find( strUserName );

		if( mIter != m_mpBlackList.end() )
			return 1;
	}

	return 0;
}

void SPWindowFriend::InitWhite()
{
	m_pWhiteListPanel	=	new SPWindowStatic( WIID_FRIEND_WHITE_LIST_PANEL , 0 , 0 , m_iSX , m_iSY , this );

	SPWindowButton*		pButton;

	pButton	=	new	SPWindowButton( WIID_FRIEND_WHITE_LIST_ADD , 154 , 34 , 68 , 17 , m_pWhiteListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 391 , 368 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 391 , 386 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 391 , 404 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 391 , 422 );
	pButton	=	NULL;

	pButton	=	new	SPWindowButton( WIID_FRIEND_WHITE_LIST_DEL , 154 , 272 , 68 , 17 , m_pWhiteListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 208 , 269 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 208 , 287 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 208 , 305 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 208 , 323 );
	pButton	=	NULL;

	pButton	=	new	SPWindowButton( WIID_FRIEND_WHITE_LIST_INVITATION , 10 , 272 , 68 , 17 , m_pWhiteListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" ,   1 , 269 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" ,   1 , 287 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" ,   1 , 305 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" ,   1 , 323 );
	pButton	=	NULL;

	pButton	=	new	SPWindowButton( WIID_FRIEND_WHITE_LIST_WHISPER , 82 , 272 , 68 , 17 , m_pWhiteListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 70 , 269 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 70 , 287 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 70 , 305 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 70 , 323 );
	pButton	=	NULL;

	for( int i = 0 ; i < WIID_FRIEND_WHITE_LIST_MAX_USER ; i++ )
	{
		m_pWhiteUser[ i ]	=	new SPWindowFriendUser( WIID_FRIEND_WHITE_LIST_USER_WINDOW + ( i * WIID_FRIEND_WHITE_LIST_USER_STEP ) , 10 , i * 27 + 85 , 192 , 23 , m_pWhiteListPanel );
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

void SPWindowFriend::InitBlack()
{
	m_pBlackListPanel	=	new SPWindowStatic( WIID_FRIEND_BLACK_LIST_PANEL , 0 , 0 , m_iSX , m_iSY , this );

	SPWindowButton*		pButton;

	pButton	=	new	SPWindowButton( WIID_FRIEND_BLACK_LIST_ADD , 154 , 34 , 68 , 17 , m_pBlackListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 391 , 440 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 391 , 458 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 391 , 476 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 391 , 495 );
	pButton	=	NULL;

	pButton	=	new	SPWindowButton( WIID_FRIEND_BLACK_LIST_DEL , 154 , 272 , 68 , 17 , m_pBlackListPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 277 , 269 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 277 , 287 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 277 , 305 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 277 , 323 );
	pButton	=	NULL;

	int	iInstanceID	=	0;
	int	iYPos		=	0;

	for( int i = 0 ; i < WIID_FRIEND_BLACK_LIST_MAX_USER ; i++ )
	{
		m_pBlackUser[ i ]	=	new SPWindowFriendUser( WIID_FRIEND_BLACK_LIST_USER_WINDOW + ( i * WIID_FRIEND_BLACK_LIST_USER_STEP ) , 10 , i * 27 + 85 , 192 , 23 , m_pBlackListPanel );
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

void SPWindowFriend::AddWhiteList( FRIEND_INFO* pFriendInfo )
{
	if( pFriendInfo == NULL )
		return;

	std::string	strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , pFriendInfo->szCharName );
	
	DelWhiteList( strFriendName.c_str() );

	if( pFriendInfo->uiServerID == 0 )		//	¿ÀÇÁ¶óÀÎÀÏ°æ¿ì
		m_mpWhiteListOffline.insert( STD_MAP_FRIEND::value_type( strFriendName , pFriendInfo ) );
	else									//	¿Â¶óÀÎÀÏ °æ¿ì
		m_mpWhiteListOnline.insert( STD_MAP_FRIEND::value_type( strFriendName , pFriendInfo ) );


	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		FRIEND_INFO*	pPlayerFriendInfo	=	new	FRIEND_INFO;

		memcpy( pPlayerFriendInfo , pFriendInfo , sizeof( FRIEND_INFO )  );

		pLocalPlayer->AddFriendList( pPlayerFriendInfo );
	}
}

void SPWindowFriend::DelWhiteList( const char* pFriendName )
{
	std::string					strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , pFriendName );

	STD_MAP_FRIEND::iterator	mIterOffline	=	m_mpWhiteListOffline.find( strFriendName );
	STD_MAP_FRIEND::iterator	mIterOnline		=	m_mpWhiteListOnline.find( strFriendName );

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

void SPWindowFriend::DelAllWhiteList()
{
	STD_MAP_FRIEND::iterator	mIter	=	m_mpWhiteListOffline.begin();

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

void SPWindowFriend::UpdateWhiteList( int iPage )
{
	int	iStartNum		=	iPage	*	WIID_FRIEND_WHITE_LIST_MAX_USER;
	int	iOnLineCount	=	(int)m_mpWhiteListOnline.size();
	int	iOffLineCount	=	(int)m_mpWhiteListOffline.size();
	int	iListCount		=	iOnLineCount	+	iOffLineCount;
	int	iMaxPage		=	iListCount / WIID_FRIEND_WHITE_LIST_MAX_USER;

	if( iListCount % WIID_FRIEND_WHITE_LIST_MAX_USER != 0 || iMaxPage == 0 )
		iMaxPage++;

	if( iPage >= iMaxPage )
	{
		iPage	=	iMaxPage - 1;
		iStartNum	=	iPage	*	WIID_FRIEND_WHITE_LIST_MAX_USER;
	}

	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , iMaxPage );

	BOOL						bOnLine	=	TRUE;
	STD_MAP_FRIEND::iterator	mIter	=	m_mpWhiteListOnline.begin();

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

	for( i = 0 ; i < WIID_FRIEND_WHITE_LIST_MAX_USER ; i++ )
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

void SPWindowFriend::HideWhiteList( int iStartNum )
{
	for( int i = iStartNum ; i < WIID_FRIEND_WHITE_LIST_MAX_USER ; i++ )
	{
		m_pWhiteUser[ i ]->SetFriendInfo( NULL );
		m_pWhiteUser[ i ]->SetWndState( WND_STATE_NORMAL );
		m_pWhiteUser[ i ]->Hide();
	}
}

void SPWindowFriend::AddBlackList( const char* pstrFriendName )
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

	m_mpBlackList.insert( STD_MAP_FRIEND::value_type( strFriendName , pFriendInfo ) );
}

void SPWindowFriend::DelBlackList( const char* pFriendName )
{
	std::string					strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , pFriendName );

	STD_MAP_FRIEND::iterator	mIter	=	m_mpBlackList.find( strFriendName );

	if( mIter != m_mpBlackList.end() )
	{
		SAFE_DELETE( (*mIter).second );
		m_mpBlackList.erase( mIter );
	}
}

void SPWindowFriend::DelAllBlackList()
{
	STD_MAP_FRIEND::iterator	mIter	=	m_mpBlackList.begin();

	while ( mIter != m_mpBlackList.end() )
	{
		SAFE_DELETE( (*mIter).second );
		++mIter;
	}

	m_mpBlackList.clear();
}

void SPWindowFriend::UpdateBlackList( int iPage )
{
	int	iStartNum	=	iPage	*	WIID_FRIEND_BLACK_LIST_MAX_USER;
	int	iListCount	=	(int)m_mpBlackList.size();
	int	iMaxPage	=	iListCount / WIID_FRIEND_BLACK_LIST_MAX_USER;

	if( iListCount % WIID_FRIEND_BLACK_LIST_MAX_USER != 0 || iMaxPage == 0 )
		iMaxPage++;

	if( iPage >= iMaxPage )
	{
		iPage	=	iMaxPage - 1;
		iStartNum	=	iPage	*	WIID_FRIEND_BLACK_LIST_MAX_USER;
	}

	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , iMaxPage );

	bool	bPageUpdate	=	false;

	if( iPage != m_iBlackPage )
		bPageUpdate	=	true;

	m_iBlackPage	=	iPage;
	m_pSlider->SetCurpage( m_iBlackPage );
	m_pSlider->Refresh();

	STD_MAP_FRIEND::iterator	mIter	=	m_mpBlackList.begin();

	for( int i = 0 ; i < iStartNum ; i++ )
	{
		if( mIter == m_mpBlackList.end() )	
			break;

		++mIter;
	}

	FRIEND_INFO*	pFriendInfo;

	for( i = 0 ; i < WIID_FRIEND_BLACK_LIST_MAX_USER ; i++ )
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

void SPWindowFriend::HideBlackList( int iStartNum )
{
	for( int i = iStartNum ; i < WIID_FRIEND_BLACK_LIST_MAX_USER ; i++ )
	{
		m_pBlackUser[ i ]->SetFriendInfo( NULL );
		m_pBlackUser[ i ]->SetWndState( WND_STATE_NORMAL );
		m_pBlackUser[ i ]->Hide();
	}
}

void SPWindowFriend::PrintErrorMsg( GLOBAL_STRING_ID uiGlobalStringID , const char* pszName /* = NULL  */ )
{
	DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( uiGlobalStringID );

	if( pszGlobalString == NULL )
		return;

	if( uiGlobalStringID > FRIEND_GLOBAL_MSG_ONE_METHOD && pszName != NULL )
	{
		sprintf( szErrorMsg , pszGlobalString , pszName );
	}
	else
	{
		strcpy( szErrorMsg , pszGlobalString );
	}

	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szErrorMsg );
}

void SPWindowFriend::RequestStatusFriend()
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

	//////////////////////////////////////////////////////////////////////////
/*
	UINT8	uiCount	=	0;

	for( int i = 0 ; i < WIID_FRIEND_WHITE_LIST_MAX_USER ; i++ )
	{
		if( m_pWhiteUser[ i ]->IsShow() == true && m_pWhiteUser[ i ]->IsFriendInfo() == true )
		{
			uiCount++;
		}
	}

	if( uiCount == 0 )
		return;

	DECLARE_VAR( TCHAR , szFriendName , MAX_LEN_NAME );

	CPacket	StatusFriend;
	StatusFriend.AddCommand( FRIEND_CS_REFRESH_STATUS );
	StatusFriend.AddUINT8( uiCount );

	for( i = 0 ; i < WIID_FRIEND_WHITE_LIST_MAX_USER ; i++ )
	{
		if( m_pWhiteUser[ i ]->IsShow() == true && m_pWhiteUser[ i ]->IsFriendInfo() == true )
		{
			_tcsncpy( szFriendName, m_pWhiteUser[ i ]->GetName() , LEN_NAME );
			StatusFriend.AddData( szFriendName , LEN_NAME );
		}
	}
*/

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &StatusFriend );

	m_fRefreshAccmulateTime	=	-1.0f;
}

const char* SPWindowFriend::GetSelectFriendUserName( FRIEND_TYPE eType )
{
	const char*	pszName	=	NULL;

	if( eType == FRIEND_TYPE_NULL )
		return pszName;

    int			iSelectNumber;
	INSTANCE_ID	eStartID;
	INSTANCE_ID	eStepID;

	if( eType == FRIEND_TYPE_WHITE )
	{
		iSelectNumber	=	m_iWhiteListSelectNumber;
		eStartID		=	WIID_FRIEND_WHITE_LIST_USER_WINDOW;
		eStepID			=	WIID_FRIEND_WHITE_LIST_USER_STEP;
	}
	else if( eType == FRIEND_TYPE_BLACK )
	{
		iSelectNumber	=	m_iBlackListSelectNumber;
		eStartID		=	WIID_FRIEND_BLACK_LIST_USER_WINDOW;
		eStepID			=	WIID_FRIEND_BLACK_LIST_USER_STEP;
	}

	INSTANCE_ID uiInstanceID = eStartID + ( iSelectNumber * eStepID );

	SPWindowFriendUser*	pWindowFriendUser	=	(SPWindowFriendUser*)( Find( uiInstanceID , true ) );

	if( pWindowFriendUser != NULL )
		pszName	=	pWindowFriendUser->GetName();

	return pszName;
}

void SPWindowFriend::SendAcceptPacket( UINT8 uiFlag )
{
	UINT8	len = min( (UINT8)_tcslen( m_szRequester ) , LEN_NAME );

	if( len <= 0 )
		return;

	CPacket	FriendPacket;

	FriendPacket.AddUINT32( FRIEND_CS_ACCEPT );
	FriendPacket.AddUINT8( uiFlag );
	FriendPacket.AddData( &m_iRequesterUserID , sizeof( m_iRequesterUserID ) );
	FriendPacket.AddData( &m_iRequesterCharID , sizeof( m_iRequesterCharID ) );
	FriendPacket.AddUINT8( len );
	FriendPacket.AddData( &m_szRequester , len );

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &FriendPacket );

	memset( m_szRequester , 0 , sizeof( m_szRequester ) );
	m_iRequesterUserID	=	0;
	m_iRequesterCharID	=	0;
}

//liuyang[2008.9.2]
int SPWindowFriend::OnDelFriendName( WPARAM wParam, LPARAM lParam )
{
	if ( NULL == lParam )
	{
		return 1;
	}

	m_strLastDelName = *(string*)lParam;
	return 1;
}
