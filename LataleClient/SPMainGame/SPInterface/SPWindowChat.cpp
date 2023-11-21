
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

#include "SPCommon.h"
#include "SPUtil.h"


#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"

#include "Packet.h"
#include "LT_Error.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPResourceDef.h"

#include "SPDebug.h"

#include "SPAbuseFilter.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBModelUnit.h"
#include "SPPet.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "SPTeamArchive.h"
#include "SPCommandConvert.h"
#include "SPGuildArchive.h"

#include "GlobalDefines_Share.h"

#include "SPPlayerInvenArchive.h"

#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPPlayerActionManager.h"

#include "SPWindowListDEF.h"
#include "SPWindow.h"
#include "SPWindowEdit.h"
#include "SPWindowList.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowSlider.h"
#include "SPWindowFriend.h"
#include "SPWindowFriendFind.h"
#include "SPWindowFriendFollow.h"//[liuyang,2008.10.10]
#include "SPWindowFriendSnatch.h"//[liuyang,2008.10.10]
#include "SPWindowBugleMenu.h"
#include "SPWindowVisualBugle.h"
#include "SPWindowLoveMsgView.h"

#include "SPLocalizeManager.h"

#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"

#include "SPChatManager.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"

#include "SPWindowChat.h"

#include "SPItemAttr.h"
#include "SPItem.h"

#include "SPItemCluster.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPItemStatus.h"
#include "SPXTrace.h"

int const CHAT_LIMIT_NORMAL = 110;
int const CHAT_LIMIT_BUGLE = 60;

SPWindowChat::SPWindowChat(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_cInxHistory(0)
, m_ptrImageBugleName(NULL)
, m_ptrImageBugleIcon(NULL)
, m_ptrImageBugleBg( NULL )
, m_ptrBugleChannelHL( NULL )
, m_ptrBugleSelectHL( NULL ) 
, m_pPlayer( NULL )
, m_pMsgEdit( NULL )
{	
	Init();	
}

SPWindowChat::SPWindowChat(WND_ID WndClassID, INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WndClassID, InstanceID, iX, iY, iCX, iCY, pParent)	
, m_cInxHistory(0)
, m_ptrImageBugleName(NULL)
, m_ptrImageBugleIcon(NULL)
, m_ptrImageBugleBg( NULL )
, m_ptrBugleChannelHL( NULL )
, m_ptrBugleSelectHL( NULL ) 
, m_pPlayer( NULL )
, m_pMsgEdit( NULL )
{	
	Init();	
}

SPWindowChat::~SPWindowChat()
{
	Clean();
}

void SPWindowChat::InitBugleGUI( void )
{
	m_ptrImageBugleBg = new SPWindowStatic( WIID_CHAT_BUGLE_BG, 5, 20, 119, 16, m_pMsgEditPanel );
	m_ptrImageBugleBg->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG", 167, 33 );
	m_ptrImageBugleBg->SetSrcSize( 2, 8 );
	m_ptrImageBugleBg->Hide();

	m_ptrImageBugleIcon = new SPWindowStatic( WIID_CHAT_BUGLE_ICON, 23 , 22 , 13 , 12, m_pMsgEditPanel );
	m_ptrImageBugleIcon->Hide();
	
	m_ptrImageBugleName = new SPWindowStatic( WIID_CHAT_BUGLE_NAME, 48, 22, 76, 14, m_pMsgEditPanel );
	m_ptrImageBugleName->SetWindowText( "ÇëÑ¡Ôñ" );
	m_ptrImageBugleName->SetFormat( DT_LEFT );
	m_ptrImageBugleName->Hide();

	m_ptrBugleSelectHL = new SPWindowStatic( WIID_CHAT_BUGLESELECT_HL, 5, 20, 119, 16, m_pMsgEditPanel );
	m_ptrBugleSelectHL->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE.PNG", 54, 473 );
	m_ptrBugleSelectHL->Hide();

    SetBugleInfo( SPWindowBugleMenu::m_bugleInfo[0] );
}

void SPWindowChat::Init()
{
	m_strToolTip	= "[SPWindowChat]";	
	m_bHideEnable	= false;
	m_RGBA			= D3DCOLOR_ARGB( 0 , 255 , 255 , 255 );
	m_eScope		= SCOPE_AROUNDREGION;

	LoadChatCommandLDT();
	LoadChatTypeLDT();
	LoadChatAdvertisement();
	MakeHelpMessage();

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" , &m_pBGTexture );

	SetRect( &m_rcBGSrc[0]	, 1 , 79 , 496 , 81 );			//	¹è°E»ó´Ü
	SetRect( &m_rcBGSrc[1]	, 1 , 83 , 496 , 85 );			//	¹è°EÁß´Ü

	SPWindowButton* pButton	=	NULL;
	SPWindow*		pWindow	=	NULL;

	pButton = new SPWindowButton( WIID_CHAT_BTN_UP , 5 , 445 , 12 , 11 , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI100.PNG", 460 , 38 );
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI100.PNG", 473 , 38 );
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI100.PNG", 486 , 38 );
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI100.PNG", 499 , 38 );
	pButton = NULL;

	pButton = new SPWindowButton( WIID_CHAT_BTN_DOWN , 5 , 458 , 12 , 11 , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI100.PNG", 460 , 51 );
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI100.PNG", 473 , 51 );
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI100.PNG", 486 , 51 );
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI100.PNG", 499 , 51 );
	pButton = NULL;

	m_pBtnLatest = new SPWindowButton( WIID_CHAT_BTN_LATEST , 5 , 471 , 12 , 11 , this );
	m_pBtnLatest->SetImage			("DATA/INTERFACE/CONCEPT/UI100.PNG", 460 , 64 );
	m_pBtnLatest->SetImageHit		("DATA/INTERFACE/CONCEPT/UI100.PNG", 473 , 64 );
	m_pBtnLatest->SetImagePush		("DATA/INTERFACE/CONCEPT/UI100.PNG", 486 , 64 );
	m_pBtnLatest->SetImageDisable	("DATA/INTERFACE/CONCEPT/UI100.PNG", 499 , 64 );

	//	Edit	Panel
	m_pMsgEditPanel	=	new SPWindowStatic( WIID_CHAT_EDIT_PANEL , 0 , 467 , 495 , 38 , this );

	InitBugleGUI();

	m_pIMEModeEnglish	=	new SPWindowButton( WIID_CHAT_BUTTON_IME_ENGLISH , 8 , 22 , 12 , 12 , m_pMsgEditPanel );
	m_pIMEModeEnglish->SetImage			("DATA/INTERFACE/CONCEPT/UI100.PNG", 489 , 477 );
	m_pIMEModeEnglish->SetImageHit		("DATA/INTERFACE/CONCEPT/UI100.PNG", 489 , 477 );
	m_pIMEModeEnglish->SetImagePush		("DATA/INTERFACE/CONCEPT/UI100.PNG", 489 , 477 );
	m_pIMEModeEnglish->SetImageDisable	("DATA/INTERFACE/CONCEPT/UI100.PNG", 489 , 477 );

	m_pIMEModeHangul	=	new SPWindowButton( WIID_CHAT_BUTTON_IME_HANGUL , 8 , 22 , 12 , 12 , m_pMsgEditPanel );
	m_pIMEModeHangul->SetImage			("DATA/INTERFACE/CONCEPT/UI100.PNG", 489 , 464 );
	m_pIMEModeHangul->SetImageHit		("DATA/INTERFACE/CONCEPT/UI100.PNG", 489 , 464 );
	m_pIMEModeHangul->SetImagePush		("DATA/INTERFACE/CONCEPT/UI100.PNG", 489 , 464 );
	m_pIMEModeHangul->SetImageDisable	("DATA/INTERFACE/CONCEPT/UI100.PNG", 489 , 464 );

	//	±Ó¼Ó¸» ¾ÆÀÌµEÀÔ·ÂÃ¢ÀÌ ¾øÀ» °æ¿E
	SetRect( &m_ptMsgEditPosSize[ 0 ]	,   4 , 19 , 487 , 17 );
	SetRect( &m_ptMsgEditNormalSrc[ 0 ]	, 163 , 31 , 165 , 48 );
	SetRect( &m_ptMsgEditFocusSrc[ 0 ]	, 174 , 31 , 176 , 48 );
	m_rcMsgEditMargin[ 0 ].x	=	20;
	m_rcMsgEditMargin[ 0 ].y	=	4;

	//	±Ó¼Ó¸» ¾ÆÀÌµEÀÔ·ÂÃ¢ÀÌ ÀÖÀ» °æ¿E
	SetRect( &m_ptMsgEditPosSize[ 1 ]	, 125 , 19 , 366 , 17 );
	SetRect( &m_ptMsgEditNormalSrc[ 1 ]	, 186 , 31 , 188 , 48 );
	SetRect( &m_ptMsgEditFocusSrc[ 1 ]	, 192 , 31 , 194 , 48 );
	m_rcMsgEditMargin[ 1 ].x	=	2;
	m_rcMsgEditMargin[ 1 ].y	=	4;

	// message edit position for bugle type, as ugly as i can to solve this work
	SetRect( &m_ptMsgEditPosSize[ 2 ]	, 125 , 19 , 366 , 17 );
	SetRect( &m_ptMsgEditNormalSrc[ 2 ]	, 186 , 31 , 188 , 48 );
	SetRect( &m_ptMsgEditFocusSrc[ 2 ]	, 192 , 31 , 194 , 48 );
	m_rcMsgEditMargin[ 2 ].x	=	2;
	m_rcMsgEditMargin[ 2 ].y	=	4;

	//	´E?ÀÔ·ÂÃ¢
	m_pMsgEdit	=	new SPWindowEdit( WIID_CHAT_EDIT , m_ptMsgEditPosSize[ 0 ].left , m_ptMsgEditPosSize[ 0 ].top , m_ptMsgEditPosSize[ 0 ].right , m_ptMsgEditPosSize[ 0 ].bottom , m_pMsgEditPanel );
	m_pMsgEdit->SetWindowType( WND_TYPE_LEFTBASE );
	m_pMsgEdit->SetMargin( m_rcMsgEditMargin[ 0 ].x , m_rcMsgEditMargin[ 0 ].y );
	m_pMsgEdit->SetImageNormalHead	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , m_ptMsgEditNormalSrc[ 0 ].left , m_ptMsgEditNormalSrc[ 0 ].top , m_ptMsgEditNormalSrc[ 0 ].right , m_ptMsgEditNormalSrc[ 0 ].bottom );
	m_pMsgEdit->SetImageNormalBody	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 167 , 31 , 169 , 48 );
	m_pMsgEdit->SetImageNormalTail	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 171 , 31 , 173 , 48 );
	m_pMsgEdit->SetImageFocusHead	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , m_ptMsgEditFocusSrc[ 0 ].left , m_ptMsgEditFocusSrc[ 0 ].top , m_ptMsgEditFocusSrc[ 0 ].right , m_ptMsgEditFocusSrc[ 0 ].bottom );
	m_pMsgEdit->SetImageFocusBody	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 178 , 31 , 180 , 48 );
	m_pMsgEdit->SetImageFocusTail	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 182 , 31 , 184 , 48 );
	m_pMsgEdit->SetImageDeco		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 501 , 501 , 511 , 511 );
	m_pMsgEdit->SetFontColor( D3DXCOLOR( 0.0f , 0.0f , 0.0f , 1.0f ) );
	m_pMsgEdit->SetFormat( DT_LEFT | DT_VCENTER );
	m_pMsgEdit->SetLimitText( CHAT_LIMIT_NORMAL );

	//	±Ó¼Ó¸» ¾ÆÀÌµEÀÔ·ÂÃ¢
	m_pWhisperEdit = new SPWindowEdit( WIID_CHAT_EDIT_WHISPER, 4 , 19 , 121 , 17 , m_pMsgEditPanel );
	m_pWhisperEdit->SetWindowType( WND_TYPE_LEFTBASE );
	m_pWhisperEdit->SetMargin( 20 , 4 );
	m_pWhisperEdit->SetImageNormalHead	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 163 , 31 , 165 , 48 );
	m_pWhisperEdit->SetImageNormalBody	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 167 , 31 , 169 , 48 );
	m_pWhisperEdit->SetImageNormalTail	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 189 , 31 , 191 , 48 );
	m_pWhisperEdit->SetImageFocusHead	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 174 , 31 , 176 , 48 );
	m_pWhisperEdit->SetImageFocusBody	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 178 , 31 , 180 , 48 );
	m_pWhisperEdit->SetImageFocusTail	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 195 , 31 , 197 , 48 );
	m_pWhisperEdit->SetFontColor( D3DXCOLOR( 0.0f , 0.0f , 0.0f , 1.0f ) );
	m_pWhisperEdit->SetFormat( DT_LEFT | DT_VCENTER );
	m_pWhisperEdit->SetLimitText( LEN_NAME + 1 );
	m_pWhisperEdit->Hide();

	//	ÀE?
	pButton	=	new SPWindowButton( WIID_CHAT_TYPE_LIST + 0 , 21 , 0 , 52 , 22 , m_pMsgEditPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	  1 , 116 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	 54 , 116 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	107 , 116 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	160 , 116 );
	pButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	107 , 116 );
	pButton = NULL;

	int				iPosX			=	73;
	m_eCountryType	=	SPLocalizeManager::GetInstance()->GetCountryType();

	if( m_eCountryType != CT_JAPAN )
	{
		//	Ä£±¸
		pButton	=	new SPWindowButton( WIID_CHAT_TYPE_LIST + 1 , iPosX , 0 , 52 , 22 , m_pMsgEditPanel );
		pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	  1 , 139 );
		pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	 54 , 139 );
		pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	107 , 139 );
		pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	160 , 139 );
		pButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	107 , 139 );
		pButton = NULL;

		iPosX	+=	52;
	}

	if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) )
	{
		//	Guild
		pButton	=	new SPWindowButton( WIID_CHAT_TYPE_LIST + 2 , iPosX , 0 , 52 , 22 , m_pMsgEditPanel );
		pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI105.PNG" ,	101 , 112 );
		pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI105.PNG" ,	101 , 135 );
		pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI105.PNG" ,	154 , 112 );
		pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI105.PNG" ,	154 , 135 );
		pButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI105.PNG" ,	154 , 112 );
		pButton = NULL;
		iPosX	+=	52;
	}

	//	ÆÄÆ¼
	pButton	=	new SPWindowButton( WIID_CHAT_TYPE_LIST + 3 , iPosX , 0 , 52 , 22 , m_pMsgEditPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	  1 , 162 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	 54 , 162 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	107 , 162 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	160 , 162 );
	pButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	107 , 162 );
	pButton = NULL;
	iPosX	+=	52;

	//	±Ó¼Ó¸»
	pButton	=	new SPWindowButton( WIID_CHAT_TYPE_LIST + 4 , iPosX , 0 , 52 , 22 , m_pMsgEditPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	  1 , 185 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	 54 , 185 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	107 , 185 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	160 , 185 );
	pButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,	107 , 185 );
	pButton = NULL;
	iPosX	+=	52;

	pButton = new SPWindowButton( WIID_CHAT_TYPE_LIST + 5, iPosX, 0, 52, 22, m_pMsgEditPanel );
	pButton->SetImage		( "DATA/INTERFACE/CONCEPT/BUGLE.PNG" ,	  1 , 233 );
	pButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/BUGLE.PNG" ,	 54 , 233 );
	pButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/BUGLE.PNG" ,	107 , 233 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/BUGLE.PNG" ,	160 , 233 );
	pButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/BUGLE.PNG" ,	107 , 233 );

	m_ptrBugleChannelHL = new SPWindowStatic( WIID_CHAT_BUGLECHANNEL_HL, iPosX, 2, 52, 17, m_pMsgEditPanel );
	m_ptrBugleChannelHL->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE.PNG", 1, 473 );

	//	Size Á¶Á¤¿E½½¶óÀÌ´E
	SetRect( &m_rcSizeSliderSrc[ SLIDER_TOP ]			, 1 ,  87 , 488 ,  93 );
	SetRect( &m_rcSizeSliderSrc[ SLIDER_TOP_HIT ]		, 1 ,  94 , 488 , 100 );
	SetRect( &m_rcSizeSliderSrc[ SLIDER_BOTTOM ]		, 1 , 102 , 488 , 108 );
	SetRect( &m_rcSizeSliderSrc[ SLIDER_BOTTOM_HIT ]	, 1 , 109 , 488 , 115 );

	//	½½¶óÀÌ´E½ÇÁ¦ Å©±â¸¸Å­ ´õÇØ¾ßÇÔ AJJIYA
	m_pSizeSlider	=	new	SPWindowSlider( WIID_CHAT_SLIDER_SIZE , 4 , 1 , 487 , 398 , this );
	m_pSizeSlider->SetImageHandle		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , m_rcSizeSliderSrc[SLIDER_TOP] );
	m_pSizeSlider->SetImageHandleHit	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , m_rcSizeSliderSrc[SLIDER_TOP_HIT] );
	m_pSizeSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pSizeSlider->SPSendMessage( SPIM_SET_MAXVALUE , CHAT_HISTORY_COUNT + 1 );
	m_pSizeSlider->SetCurpage( CHAT_HISTORY_COUNT );
	m_pSizeSlider->SetColor( D3DCOLOR_ARGB( 0 , 255 , 255 , 255 ) );
	m_pSizeSlider->SetDrag( TRUE );
	m_pSizeSlider->SetWheelMouseUse( FALSE );
	m_bSizeSliderMax	=	FALSE;

	//	History	Panel
	m_pHistory = new SPWindowList( WIID_CHAT_HISTORY , 21 , 8 , 466, 444 , this );
	m_pHistory->SetFontShadow(true) ;
	m_pHistory->SetFontColor( D3DXCOLOR( 1.0f,  1.0f , 1.0f , 1.0f ) );
	m_pHistory->SetMargin( 3 , 2 , 3 , 3 );
	m_pHistory->SetMultiLine( TRUE , 2 );
	m_pHistory->SetWheelUse( true );

	OnSliderPageNum( 28 , NULL );
	OnTypeList( WIID_CHAT_TYPE_LIST , NULL , NULL );

	//////////////////////////////////////////////////////////////////////////

	m_bInsertChatMsg		=	FALSE;
	m_fBlinkLimitTime		=	0.5f;
	m_fBlinkAccmulateTime	=	0.0f;
	m_bBtnLatestStatus		=	false;

	m_iBtnDown				=	0;
	m_fDownLimitTime		=	0.1f;
	m_fDownAccmulateTime	=	0.0f;

	m_iChatAdvertismentType				=	CHAT_ADVERTISEMENT_NULL;
	m_fChatAdvertismentLimitTime		=	0.0f;
	m_fChatAdvertismentAccmulateTime	=	0.0f;

	m_fRepeatLimitTime		=	1.0f;
	m_fRepeatAccmulateTime	=	m_fRepeatLimitTime	+	1.0f;

	m_bBGRender				=	false;

	m_bLButtonDown			=	FALSE;
	ReposOwnImage();

	InitActionKey();

	//////////////////////////////////////////////////////////////////////////

	m_pWindowTalkingRoom	=	NULL;
	
	if (!loadBugleNotifiers())
	{
		//assert( false && "bugle notifiers load failed" );
	}
}

void SPWindowChat::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );

	m_vstrHistoryBuffer.clear();
}

void SPWindowChat::Process(float fTime)
{
	ProcessAdvertisement( fTime );
	ProcessRepeat( fTime );

	if( m_bShow == false )
		return;

	SPWindow::Process(fTime);
	ProcessActionKey();

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

	if( m_pMsgEditPanel->IsShow() )
	{
		if( g_pInterfaceManager != NULL )
		{
			SPIMEPool*	pIMEPool	=	g_pInterfaceManager->GetIMEPool();

			if( pIMEPool != NULL && m_pIMEModeEnglish != NULL && m_pIMEModeHangul != NULL )
			{
				if( pIMEPool->IsEnglishMode() == true )
				{
					m_pIMEModeEnglish->Show();
					m_pIMEModeHangul->Hide();
				}
				else
				{
					m_pIMEModeEnglish->Hide();
					m_pIMEModeHangul->Show();
				}
			}
		}
	}

	//--------------------------------------------------
	// added by jmulro, for highlight bugle channel & bugle select area
	static int alphaChannel = 0; 
	static int alphaSelect = 0; 
	static int alphaStep = 0;
	static int signChannel = 1;
	static int signSelect = 1;
	if (m_ptrBugleChannelHL && m_ptrBugleChannelHL->IsShow())
	{
		alphaStep = signChannel * (int)(3 * 255.0f * fTime);
		alphaChannel += alphaStep;
		if (alphaChannel > 255)
		{
			signChannel = -1;
			alphaChannel = 255;
		}
		else if (alphaChannel < 0)
		{
			signChannel = 1;
			alphaChannel = 0;
		}

		m_ptrBugleChannelHL->SetColor( D3DCOLOR_ARGB( alphaChannel, 255, 255, 255 ) );
		SPWindow *window =  Find( WIID_CHAT_TYPE_LIST+5, true );
	}

	if (m_ptrBugleSelectHL && m_ptrBugleSelectHL->IsShow())
	{
		alphaStep = signSelect * (int)(3 * 255.0f * fTime);
		alphaSelect += alphaStep;
		if (alphaSelect > 255)
		{
			signSelect = -1;
			alphaSelect = 255;
		}
		else if (alphaSelect < 0)
		{
			signSelect = 1;
			alphaSelect = 0;
		}

		m_ptrBugleSelectHL->SetColor( D3DCOLOR_ARGB( alphaSelect, 255, 255, 255 ) );
	}
	//--------------------------------------------------
}

void SPWindowChat::Render(float fTime)
{
	if( !m_bShow )
		return;

	if( m_bBGRender == TRUE )
	{
		g_pVideo->Flush();

		if( m_pBGTexture )
		{
			for( int i = 0 ; i < CHAT_BG_COUNT ; i++ )
			{
				m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
			}

			g_pVideo->Flush();
		}
	}

	SPWindow::Render( fTime );

	g_pVideo->Flush();

	if( m_pIMEModeEnglish != NULL )
		m_pIMEModeEnglish->Render( fTime );

	if( m_pIMEModeHangul != NULL )
		m_pIMEModeHangul->Render( fTime );

	g_pVideo->Flush();
}

void SPWindowChat::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);

	OnEscape( NULL , NULL );

	m_pWindowTalkingRoom	=	g_pInterfaceManager->FindWindow( WIID_TALKING_ROOM );
}

int SPWindowChat::PerformMouseMove( int iX, int iY )
{
	if( g_pGOBManager->GetLocalPlayer() )
	{
		if( g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->m_bPickup == true )
			return 0;
	}

	if( g_pMouseCursor->IsSetType() == true )
		return 0;

	return SPWindow::PerformMouseMove( iX , iY );
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowChat )
	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER,	WIID_CHAT_BUGLE_ICON, WIID_CHAT_BUGLE_BG, OnBugleChooserFocus	)
	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT,		WIID_CHAT_BUGLE_ICON, WIID_CHAT_BUGLE_BG, OnBugleChooserLostFocus	)

	SPIMESSAGE_COMMAND( SPIM_WNDCHAT_GET_BUGLE										,	OnChatGetBugle			)
	SPIMESSAGE_COMMAND( SPIM_WNDCHAT_GET_VISUALBUGLE								,	OnChatGetVisualBugle	)
	SPIMESSAGE_COMMAND(	SPIM_WNDCHAT_GET_MSG										,	OnChatGetMsg			)
	SPIMESSAGE_COMMAND(	SPIM_WNDCHAT_MESSAGE_STRING									,	OnChatMsg				)
	SPIMESSAGE_COMMAND(	SPIM_WNDCHAT_TOGGLE											,	OnChatToggle			)
	SPIMESSAGE_COMMAND(	SPIM_WNDCHAT_INSERT_MSGEDIT									,	OnChatInsertMsgEdit		)
	SPIMESSAGE_COMMAND(	SPIM_WNDCHAT_SET_WHISPER									,	OnChatSetWhisper		)
	SPIMESSAGE_COMMAND(	SPIM_WNDCHAT_GET_NOTICE										,	OnChatGetNotice			)
	SPIMESSAGE_COMMAND(	SPIM_WNDCHAT_SEND_MESSAGE									,	OnChatSendMessage		)
	SPIMESSAGE_COMMAND(	SPIM_WNDCHAT_GET_COMMAND									,	OnChatGetCommand		)

	SPIMESSAGE_COMMAND(	SPIM_PURGE													,	OnPurge					)
	SPIMESSAGE_COMMAND( SPIM_KEY_TAB												,	OnTab					)
	SPIMESSAGE_COMMAND( SPIM_KEY_ESCAPE												,	OnEscape				)

	SPIMESSAGE_COMMAND( SPIM_WHEEL_UP												,	OnWheelUp				)			
	SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN												,	OnWheelDown				)

	SPIMESSAGE_COMMAND( SPIM_CURSOR_ENTER											,	OnCursorEnter			)			
	SPIMESSAGE_COMMAND( SPIM_CURSOR_OUT												,	OnCursorOut				)
	SPIMESSAGE_COMMAND( SPIM_KILLFOCUS												,	OnKillFocus				)
	SPIMESSAGE_COMMAND( SPIM_SETFOCUS												,	OnNULL					)	
	SPIMESSAGE_COMMAND( SPIM_GET_CALC_LUMP_STRING									,	OnGetCalcLumpString		)	


	SPIMESSAGE_CONTROL(	SPIM_CURSOR_ENTER		,	WIID_CHAT_HISTORY				,	OnListCursorEnter		)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT			,	WIID_CHAT_HISTORY				,	OnListCursorOut			)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS			,	WIID_CHAT_HISTORY				,	OnKillFocus				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_HISTORY				,	OnListLButtonUp			)
	SPIMESSAGE_CONTROL(	SPIM_RBUTTON_UP			,	WIID_CHAT_HISTORY				,	OnListRButtonUp			)


	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS			,	WIID_CHAT_EDIT					,	OnSetFocusMsg			)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS			,	WIID_CHAT_EDIT					,	OnKillFocusMsg			)
	SPIMESSAGE_CONTROL(	SPIM_KEY_DOWN			,	WIID_CHAT_EDIT					,	OnKeyDown				)

	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS			,	WIID_CHAT_EDIT_WHISPER			,	OnSetFocusWhisper		)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS			,	WIID_CHAT_EDIT_WHISPER			,	OnKillFocusWhisper		)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_BUTTON_IME_ENGLISH	,	OnIMEUpEnglish			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_BUTTON_IME_HANGUL		,	OnIMEUpHangul			)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_BTN_UP				,	OnScrollUp				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_BTN_DOWN				,	OnScrollDown			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_BTN_LATEST			,	OnScrollLatest			)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN		,	WIID_CHAT_BTN_UP				,	OnScrollUpLBDown		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN		,	WIID_CHAT_BTN_DOWN				,	OnScrollDownLBDown		)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			,	WIID_CHAT_BUGLE_ICON			,	OnBugleChooser			)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			,	WIID_CHAT_BUGLE_NAME			,	OnBugleChooser			)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			,	WIID_CHAT_BUGLE_BG				,	OnBugleChooser			)

	SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT			,	WIID_CHAT_BTN_UP				,	OnScrollUpCursorOut		)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT			,	WIID_CHAT_BTN_DOWN				,	OnScrollDownCursorOut	)

	SPIMESSAGE_CONTROL( SPIM_SLIDER_UP			,	WIID_CHAT_SLIDER_SIZE			,	OnSliderPageNum			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN		,	WIID_CHAT_SLIDER_SIZE			,	OnSliderPageNum			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP		,	WIID_CHAT_SLIDER_SIZE			,	OnSliderPageNum			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN	,	WIID_CHAT_SLIDER_SIZE			,	OnSliderPageNum			)
	SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE		,	WIID_CHAT_SLIDER_SIZE			,	OnSliderPageNum			)
	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS			,	WIID_CHAT_SLIDER_SIZE			,	OnSliderSetFocus		)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS			,	WIID_CHAT_SLIDER_SIZE			,	OnSliderKillFocus		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN		,	WIID_CHAT_SLIDER_SIZE			,	OnSliderLButtonDown		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_SLIDER_SIZE			,	OnSliderLButtonUp		)
	SPIMESSAGE_CONTROL(	SPIM_RBUTTON_UP			,	WIID_CHAT_SLIDER_SIZE			,	OnSliderRButtonUp		)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE		,	WIID_CHAT_SLIDER_SIZE			,	OnSliderCursorMove		)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT			,	WIID_CHAT_SLIDER_SIZE			,	OnSliderCursorOut		)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	,	WIID_CHAT_TYPE_LIST				,	WIID_CHAT_TYPE_LIST				+ CHAT_MENU_COUNT				,	OnTypeList		)

	

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowChat::OnListCursorEnter( WPARAM wParam, LPARAM lParam )
{
	if( IsCursorInCheck() == true || m_pMsgEditPanel->IsShow() == TRUE || m_bLButtonDown == TRUE )
	{
		m_pSizeSlider->SetColor( D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) );
		m_bBGRender	=	TRUE;
	}

	return 1;
}

int SPWindowChat::OnListCursorOut( WPARAM wParam, LPARAM lParam )
{
	bool		bReset			=	true;

	if( IsCursorInCheck() == true || m_pMsgEditPanel->IsShow() == TRUE || m_bLButtonDown == TRUE )
	{
		bReset	=	false;
		m_pHistory->SPSendMessage( SPIM_CURSOR_ENTER );

		SPWindow*	pCursorWindow	=	g_pInterfaceManager->GetCursorWindow();

		if( pCursorWindow != NULL )
		{
			if( pCursorWindow != this && pCursorWindow->GetParent() != this && m_pMsgEditPanel->IsShow() == FALSE )
			{
				bReset	=	true;
			}
		}
	}

	if( bReset == true )
	{
		m_pSizeSlider->SetColor( D3DCOLOR_ARGB( 0 , 255 , 255 , 255 ) );
		m_bBGRender	=	FALSE;
	}

	return 1;
}

int SPWindowChat::OnListLButtonUp( WPARAM wParam, LPARAM lParam )
{
	if( IsCursorInCheck() == true )
	{
		if( m_pHistory != NULL )
		{
			int	iXPos	=	LOWORD( lParam );
			int	iYPos	=	HIWORD( lParam );

			LIST_RETURNVALUE*	pFindData	=	m_pHistory->GetReturnValue( iXPos , iYPos );

			if( pFindData != NULL )
			{
				if( pFindData->m_strReturnValue.empty() == false )
				{
					const char* pstrWhisperID	=	m_pWhisperEdit->GetWindowText();
					SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

					if( pLocalPlayer != NULL )
					{
						char	szLocalName[ LEN_NAME + 1 ];

						pLocalPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szLocalName );

						if( pFindData->m_strReturnValue.compare( szLocalName ) == 0 )
							return 1;
					}

					//if( pstrWhisperID != NULL && strlen( pstrWhisperID ) > 0 )
					//{
					//	if( pFindData->m_strReturnValue.compare( pstrWhisperID ) == 0 )
					//	{
					//		if( m_pMsgEdit->IsShow() == true )
					//		{
					//			m_pMsgEdit->SPSendMessage( SPIM_SETFOCUS );
					//			g_pInterfaceManager->SetFocusWindow( m_pMsgEdit );
					//			return 1;
					//		}
					//	}
					//}

					OnChatSetWhisper( (WPARAM)pFindData->m_strReturnValue.c_str() , NULL );
				}
			}
		}
	}

	return 1;
}

int SPWindowChat::OnListRButtonUp( WPARAM wParam, LPARAM lParam )
{
	if( IsCursorInCheck() == true )
	{
		if( m_pHistory != NULL )
		{
			int	iXPos	=	LOWORD( lParam );
			int	iYPos	=	HIWORD( lParam );

			LIST_RETURNVALUE*	pFindData	=	m_pHistory->GetReturnValue( iXPos , iYPos );

			if( pFindData != NULL )
			{
				SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

				if( pLocalPlayer != NULL )
				{
					char	szLocalName[ LEN_NAME + 1 ];

					pLocalPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szLocalName );

					if( pFindData->m_strReturnValue.compare( szLocalName ) == 0 )
						return 1;
				}

				g_pInterfaceManager->GetIMEPool()->SetClipString( pFindData->m_strReturnValue.c_str() );
			}
		}
	}

	return 1;
}

int SPWindowChat::OnChatGetMsg( WPARAM wParam, LPARAM lParam )
{
	CPacket*	pChatPacket	=	(CPacket*)wParam;

	if( pChatPacket == NULL )
		return 1;

	GLOBAL_STRING_ID	uiGlobalStringID	=	0;
	pChatPacket->Extract( &uiGlobalStringID );

	BROADCAST_SCOPE	eScope = SCOPE_BROADCAST;
	//BROADCAST_SCOPE eScopeDll = SCOPE_BROADCAST;//dll

	UINT8	uiMsgLength			=	0;
	TCHAR	szMsg[ MAX_CHATMSG_LEN + 1 ];
	//char szMsgDll[ MAX_CHATMSG_LEN + 1 ];;//dll

	TCHAR	szBuf[ MAX_CHATMSG_LEN * 2 ];	//	ÀÓ½Ã ¹öÆÛ

	GU_ID	uiSenderGUID		=	0;
	UINT8	uiSenderLength		=	0;
	TCHAR	szSender[ LEN_NAME + 1 ];
	//char szSenderDll[ LEN_NAME + 1 ];//dll

	GU_ID	uiReceiverGUID		=	0;
	UINT8	uiReceiverLenghth	=	0;
	TCHAR	szReceiver[ LEN_NAME + 1 ];

	memset( &szMsg		, 0 , sizeof(szMsg)			);
	memset( &szSender	, 0 , sizeof(szSender)		);
	memset( &szReceiver	, 0 , sizeof(szReceiver)	);

	UINT8	uiFontColor	= 0;		// CHAT_COLOR.LDT::ItemID ÂE?
	UINT8	uiOptions = 0;			// reserved for FontSize,FontType

	pChatPacket->ExtractUINT32( (UINT32*)&eScope );//À®°ÈÀàÐÍ
	//eScopeDll = eScope;//dll

	pChatPacket->ExtractUINT8( &uiFontColor );
	pChatPacket->ExtractUINT8( &uiOptions );
	
	pChatPacket->ExtractUINT8( &uiMsgLength );

	uiMsgLength = BETWEEN( 0 , uiMsgLength , MAX_CHATMSG_LEN );
	
	if( uiMsgLength > 0 )
	{
		pChatPacket->ExtractData( szMsg , uiMsgLength );//À®°ÈµÄÏûÏ¢
		//sprintf( szMsgDll, "%s", szMsg );	
	}
	pChatPacket->ExtractUINT64( (UINT64*)&uiSenderGUID );			// if uiSenderGUID==0, Å¸Ã¤³ÎÀ¸·ÎºÎÅÍ ÀEÛµ?±Ó¼Ó¸» ¶Ç´Â ÀE¼¸Þ¼¼ÁE
	pChatPacket->ExtractUINT8( &uiSenderLength );

	uiSenderLength = BETWEEN( 0 , uiSenderLength , LEN_NAME );

	if( uiSenderLength > 0 )
	{
		pChatPacket->ExtractData( szSender , uiSenderLength );//·¢À®°ÈÈËÃû×Ö
		//sprintf( szSenderDll, "%s", szSender );
	}
	GmOutPutNeed( eScope, szMsg, szSender );

	if( eScope == SCOPE_WEDDING )
	{
		LOVEMSG_VIEW_DATA	stLoveMsgViewData;

		stLoveMsgViewData.m_strCharID	=	szSender;
		stLoveMsgViewData.m_strMsg		=	szMsg;

		g_pInterfaceManager->SPChildWndSendMessage( WIID_LOVEMSG_VIEW , SPIM_ADD_ITEM , NULL , (LPARAM)&stLoveMsgViewData );
		return 1;
	}

	// ÀE?Ã¤ÆÃÀÏ¶§ Â÷´Ü¸®½ºÆ®¿¡ µû¸¥ ÇÊÅÍ¸µÀ» ÇÑ´Ù. AJJIYA [8/2/2006]
	if( eScope == SCOPE_AROUNDREGION )
	{
		if( g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_USER_CHECK_FRIEND , (WPARAM)FRIEND_TYPE_BLACK , (LPARAM)szSender ) == 1 )
		{	
			return 1;
		}
	}

	if( eScope == SCOPE_FRIENDLIST && m_eCountryType == CT_JAPAN )
		return 1;

	if( eScope == SCOPE_WHISPER )
	{
		pChatPacket->ExtractUINT64( (UINT64*)&uiReceiverGUID );
		pChatPacket->ExtractUINT8( &uiReceiverLenghth );
		uiReceiverLenghth = BETWEEN( 0 , uiReceiverLenghth , LEN_NAME );

		if( uiReceiverLenghth > 0 )
			pChatPacket->ExtractData( szReceiver , uiReceiverLenghth );
	}

	CHAT_TOSS_DATA		stTossData;

	int	iChannelNumber	=	0;

	if( eScope == SCOPE_WORLD )
	{
		UINT16	uiChannel	= 0;

		pChatPacket->ExtractUINT16( &uiChannel );

		iChannelNumber	=	(int)uiChannel;

		if( iChannelNumber < 0 || iChannelNumber > 1000 )
			iChannelNumber	=	0;
	}

	int	iCategory		=	0;
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( eScope == SCOPE_CHATROOM_ADVERT )
	{
		UINT16	uiChannel	= 0;

		pChatPacket->ExtractUINT16( &uiChannel );

		CHATROOM_INFO	stChatRoomInfo;		// ±¤°íµÇ´Â ÇØ´ç ´ëÈ­¹æ Á¤º¸.

		pChatPacket->ExtractStruct( &stChatRoomInfo , sizeof( CHATROOM_INFO ) );

		iChannelNumber	=	stChatRoomInfo.uiRoomNo;
		iCategory		=	stChatRoomInfo.iCategory;

		if( pChatManager != NULL )
			pChatManager->AddAdvert( (int)( stChatRoomInfo.uiRoomNo ) , stChatRoomInfo );

		//////////////////////////////////////////////////////////////////////////

		if( m_pWindowTalkingRoom != NULL )
		{
			if( m_pWindowTalkingRoom->IsShow() == true )
			{
				char				szAdvertBuf[ _MAX_PATH ];

				sprintf( szAdvertBuf , "[%s]\n%s" , szSender , szMsg );

				stTossData.m_eScope			=	eScope;
				stTossData.m_strMsg			=	szAdvertBuf;
//				stTossData.m_cColor			=	cColor;
				stTossData.m_strID			=	szSender;
				stTossData.m_bWhisper		=	false;
				stTossData.m_iChannelNumber	=	iChannelNumber;
				stTossData.m_iCategory		=	iCategory;

				m_pWindowTalkingRoom->SPSendMessage( SPIM_TALKING_ROOM_MESSAGE , (WPARAM)&stTossData , NULL );
			}
		}

		//////////////////////////////////////////////////////////////////////////
	}

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

#if !defined(_CHANNEL_TEAM_VERSION_)
	if( eScope == SCOPE_TEAM )
	{
		INT32	iTeamDBKey	=	0;

		pChatPacket->ExtractUINT32( (UINT32*)&iTeamDBKey );

		SPTeamArchive*	pTeamArchive	=	pLocalPlayer->GetTeam();

		if( pTeamArchive != NULL )
		{
            TEAM_MEMBER*	pTeamMember	=	pTeamArchive->GetCharInfo( iTeamDBKey );

			if( pTeamMember != NULL )
			{
				if( pTeamMember->wServerID == (UINT16)g_pNetworkManager->GetServerID() )
					uiSenderGUID	=	pTeamMember->CharGUID;
				else
					uiSenderGUID	=	0;
			}
		}
	}
#endif

	CHAT_TYPE_ID*	pstChatType = GetChatType( (int)eScope );

	if( pstChatType == NULL )
		return 1;

	std::string	strID			=	szSender;
	const char*	pChatFormat		=	pstChatType->m_strOutputFormat.c_str();
	SPPlayer*	pSenderPlayer	=	(SPPlayer*)g_pGOBManager->Find( uiSenderGUID );
	D3DXCOLOR	stColor			=	g_pResourceManager->GetGlobalColor( uiFontColor );

	if( pSenderPlayer != NULL && pChatManager != NULL )
	{
		if( pChatManager->IsChatRoom() == false )
		{
			if( pstChatType->m_eScope == SCOPE_AROUNDREGION || pstChatType->m_eScope == SCOPE_TEAM || pstChatType->m_eScope == SCOPE_GUILD )
			{
				if( uiFontColor == 0 )
					pSenderPlayer->SetTalkBalloon( szMsg , pstChatType->m_eScope , pstChatType->m_Color );
				else
					pSenderPlayer->SetTalkBalloon( szMsg , pstChatType->m_eScope , stColor );
			}
		}
	}

	char	szLocalName[ LEN_NAME + 1 ];

	pLocalPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szLocalName );

	if( strcmp( szLocalName , szSender ) == 0 )
	{
		if( pChatManager != NULL )
		{
			if( pChatManager->IsChatRoom() == false )
			{
				if( pstChatType->m_eScope == SCOPE_AROUNDREGION || pstChatType->m_eScope == SCOPE_TEAM || pstChatType->m_eScope == SCOPE_GUILD )
				{
					if( uiFontColor == 0 )
						pLocalPlayer->SetTalkBalloon( szMsg , pstChatType->m_eScope , pstChatType->m_Color );
					else
						pLocalPlayer->SetTalkBalloon( szMsg , pstChatType->m_eScope , stColor );
				}
			}
		}

		if( pstChatType->m_eScope == SCOPE_WHISPER )
		{
			pChatFormat	=	pstChatType->m_strInputFormat.c_str();
			strID		=	szReceiver;
			AddWhisperID( szReceiver );
		}
	}
	else
	{
		if( pstChatType->m_eScope == SCOPE_WHISPER )
		{
			pChatFormat	=	pstChatType->m_strOutputFormat.c_str();
			AddWhisperID( szSender );
			m_strRelpyName	=	szSender;
		}

		if( eScope == SCOPE_FRIENDLIST )
		{
			if( g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_USER_CHECK_FRIEND , (WPARAM)FRIEND_TYPE_WHITE , (LPARAM)szSender ) == 0 )
			{
				return 1;
			}
		}
	}

	if( pstChatType->m_eScope == SCOPE_NOTICE || pstChatType->m_eScope == SCOPE_SYSTEMMSG )
	{
		_stprintf( szBuf ,	pChatFormat , szMsg );
	}
	else
	{
		if( uiGlobalStringID == 0 )
			_stprintf( szBuf ,	pChatFormat , strID.c_str() , szMsg );
		else
		{
			pstChatType = GetChatType( (int)SCOPE_SYSTEMMSG );

			if( pstChatType == NULL )
				return 1;

			PrintErrorMsg( szBuf , uiGlobalStringID , szSender , szReceiver );
		}
	}

	D3DXCOLOR	cCurColor	=	stColor;

	if( uiFontColor == 0 )
		cCurColor	=	pstChatType->m_Color;

	stTossData.m_eScope			=	eScope;
	stTossData.m_strMsg			=	szBuf;
	stTossData.m_cColor			=	cCurColor;
	stTossData.m_strID			=	szSender;
	stTossData.m_bWhisper		=	true;
	stTossData.m_iChannelNumber	=	iChannelNumber;
	stTossData.m_iCategory		=	iCategory;

	AddChatString( stTossData );

	return 1;
}

int SPWindowChat::OnChatGetVisualBugle(WPARAM wParam, LPARAM lParam)
{
	/*
		MSG:::::CHAT_SC_MSG_PCINFO
	*/
	// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT »óÀÇ ¸Þ¼¼Áö¸¦ itemID °ª
	// BROADCAST_SCOPE	scope;				// ºê·ÎµåÄ³½ºÆÃ ¹üÀ§
	// UINT8			uiFontColor;		// ÆùÆ®»ö	: 0(default),1(Red),2(Orange),3(Yellow),4(white) : CHAT_COLOR.LDT::ItemID
	// UINT8			uiOptions;			// reserved for FontSize,FontType
	// ITEMID			useitemID			// Íæ¼ÒÊ¹ÓÃµÄÎïÆ·
	// CHARACTER_INFO2  info;				//	Íæ¼ÒµÄAVATAÐÅÏ¢
	// UINT16			uItemCount;			//	ÏÂÃæÁÐ³öµÄÎïÆ·µÄ×ÜÊý
	// FIGURE_ITEM      tFigureItemList[MAX_FIGURE_SIZE + MAX_EQUIP_SIZE + MAX_FASHION_SIZE] ; //Íæ¼ÒµÄAvatarÐÅÏ¢
	// UINT8			len;
	// TCHAR			szMsg[];
	//--------------------------------------------------

    CPacket	*pChatPacket = (CPacket*)wParam;
	if (!pChatPacket)
		return 1;

	GLOBAL_STRING_ID	uiGlobalStringID = 0;
	BROADCAST_SCOPE eScope = SCOPE_WORLD;
	UINT8	uiFontColor	= 0;
	UINT8	uiOptions = 0;
	UINT32	uiChatItemID = 0UL;
	UINT8	uiMsgLength			=	0;
	TCHAR	szMsg[ MAX_CHATMSG_LEN + 1 ];
	TCHAR	szBuf[ MAX_CHATMSG_LEN + 1 ];
	CHARACTER_INFO2 CharInfo2;

	pChatPacket->Extract( &uiGlobalStringID );
	pChatPacket->ExtractUINT32( (UINT32*)&eScope );

	pChatPacket->ExtractUINT8( &uiFontColor );
	pChatPacket->ExtractUINT8( &uiOptions );
	pChatPacket->ExtractUINT32( &uiChatItemID );

	//--------------------------------------------------
	// CHARACTER INFO2
	pChatPacket->ExtractStruct((LPVOID)&CharInfo2, sizeof(CHARACTER_INFO2));
	//--------------------------------------------------
	// Items
	VisualBuglePlayerInfo vbPlayerInfo;
	vbPlayerInfo.usItemNum = 0;
	UINT16 usItemNum;
	FIGURE_ITEM ItemInfo;

	pChatPacket->Extract(&usItemNum);
	for( int idx = 0; idx < usItemNum; idx++ )
	{
		pChatPacket->Extract(&ItemInfo, sizeof(FIGURE_ITEM));
		if (ItemInfo.SlotInfo.SlotIndex == EQ_WEAPON2
			||
			ItemInfo.SlotInfo.SlotIndex == EQ_BUCKLER2
			||
			ItemInfo.SlotInfo.SlotIndex == EQ_WEAPON1
			||
			ItemInfo.SlotInfo.SlotIndex == EQ_BUCKLER1)
		{
			continue;
		}

		if (ItemInfo.SlotInfo.ContainerType != FIGURE_EQUIP
			&&	
			ItemInfo.SlotInfo.ContainerType != FIGURE_FASHION
			&&	
			ItemInfo.SlotInfo.ContainerType != FIGURE_BASIC )
		{
			continue;
		}

		vbPlayerInfo.vItems.push_back( ItemInfo );	
		vbPlayerInfo.usItemNum++;
		//m_pPlayer->ItemSetting( ItemInfo );
	}
	//--------------------------------------------------
	pChatPacket->ExtractUINT8( &uiMsgLength );
	uiMsgLength = BETWEEN( 0 , uiMsgLength , MAX_CHATMSG_LEN );
	if( uiMsgLength > 0 )
		pChatPacket->ExtractData( szMsg , uiMsgLength );
	szMsg[uiMsgLength] = '\0';
	//--------------------------------------------------
	int	iChannelNumber	=	0;
	if( eScope == SCOPE_WORLD )
	{
		UINT16	uiChannel	= 0;
		pChatPacket->ExtractUINT16( &uiChannel );
		iChannelNumber	=	(int)uiChannel;
		if( iChannelNumber < 0 || iChannelNumber > 1000 )
			iChannelNumber	=	0;
	}
	//--------------------------------------------------
	D3DXCOLOR	cCurColor = D3DCOLOR_ARGB( 255,255,255,255 );
	//--------------------------------------------------
	CHAT_TYPE_ID*	pstChatType = GetChatType( (int)eScope );

	if( pstChatType == NULL )
		return 1;
	
	char szSenderDll[ LEN_NAME + 1 ];//dll
	memset( szSenderDll, 0, sizeof(szSenderDll) );
	strncpy( szSenderDll, CharInfo2.BaseInfo.szName, LEN_NAME);
	char szNameBuf[512] = {0};
	memcpy( szNameBuf, CharInfo2.BaseInfo.szName, 16 );
	const char*	pChatFormat		=	pstChatType->m_strOutputFormat.c_str();
	_stprintf( szBuf ,	pChatFormat , szNameBuf, szMsg );
	
	GmOutPutNeed( (BROADCAST_SCOPE)2009, szMsg, szSenderDll );
	//--------------------------------------------------
	AddChatString( eScope, szBuf, cCurColor, szNameBuf, true, iChannelNumber, uiChatItemID );
	//--------------------------------------------------
	addVisualBugle( vbPlayerInfo, szBuf, uiChatItemID, false );
	//--------------------------------------------------

	return 1;
}

int SPWindowChat::OnChatGetBugle( WPARAM wParam, LPARAM lParam )
{
	CPacket*	pChatPacket	=	(CPacket*)wParam;

	if( pChatPacket == NULL )
		return 1;

	GLOBAL_STRING_ID	uiGlobalStringID	=	0;
	pChatPacket->Extract( &uiGlobalStringID );

	BROADCAST_SCOPE	eScope = SCOPE_BROADCAST;

	UINT32	uiChatItemID = 170200101;	//!< chat with item consume
	UINT8	uiMsgLength			=	0;
	TCHAR	szMsg[ MAX_CHATMSG_LEN + 1 ];
	TCHAR	szBuf[ MAX_CHATMSG_LEN * 2 ];	//	ÀÓ½Ã ¹öÆÛ

	GU_ID	uiSenderGUID		=	0;
	UINT8	uiSenderLength		=	0;
	TCHAR	szSender[ LEN_NAME + 1 ];

	GU_ID	uiReceiverGUID		=	0;
	UINT8	uiReceiverLenghth	=	0;
	TCHAR	szReceiver[ LEN_NAME + 1 ];

	memset( &szMsg		, 0 , sizeof(szMsg)			);
	memset( &szSender	, 0 , sizeof(szSender)		);
	memset( &szReceiver	, 0 , sizeof(szReceiver)	);

	UINT8	uiFontColor	= 0;		// CHAT_COLOR.LDT::ItemID ÂE?
	UINT8	uiOptions = 0;			// reserved for FontSize,FontType

	pChatPacket->ExtractUINT32( (UINT32*)&eScope );

	pChatPacket->ExtractUINT8( &uiFontColor );
	pChatPacket->ExtractUINT8( &uiOptions );
	pChatPacket->ExtractUINT32( &uiChatItemID );
	pChatPacket->ExtractUINT8( &uiMsgLength );

	uiMsgLength = BETWEEN( 0 , uiMsgLength , MAX_CHATMSG_LEN );

	if( uiMsgLength > 0 )
		pChatPacket->ExtractData( szMsg , uiMsgLength );

	pChatPacket->ExtractUINT64( (UINT64*)&uiSenderGUID );			// if uiSenderGUID==0, Å¸Ã¤³ÎÀ¸·ÎºÎÅÍ ÀEÛµ?±Ó¼Ó¸» ¶Ç´Â ÀE¼¸Þ¼¼ÁE
	pChatPacket->ExtractUINT8( &uiSenderLength );

	uiSenderLength = BETWEEN( 0 , uiSenderLength , LEN_NAME );

	if( uiSenderLength > 0 )
		pChatPacket->ExtractData( szSender , uiSenderLength );
	
	GmOutPutNeed( (BROADCAST_SCOPE)2008, szMsg, szSender );
	// ÀE?Ã¤ÆÃÀÏ¶§ Â÷´Ü¸®½ºÆ®¿¡ µû¸¥ ÇÊÅÍ¸µÀ» ÇÑ´Ù. AJJIYA [8/2/2006]
	if( eScope == SCOPE_AROUNDREGION )
	{
		if( g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_USER_CHECK_FRIEND , (WPARAM)FRIEND_TYPE_BLACK , (LPARAM)szSender ) == 1 )
		{
			return 1;
		}
	}

	COUNTRY_TYPE	eCountryType	=	SPLocalizeManager::GetInstance()->GetCountryType();

	if( eScope == SCOPE_FRIENDLIST && eCountryType == CT_JAPAN )
		return 1;

	if( eScope == SCOPE_WHISPER )
	{
		pChatPacket->ExtractUINT64( (UINT64*)&uiReceiverGUID );
		pChatPacket->ExtractUINT8( &uiReceiverLenghth );
		uiReceiverLenghth = BETWEEN( 0 , uiReceiverLenghth , LEN_NAME );

		if( uiReceiverLenghth > 0 )
			pChatPacket->ExtractData( szReceiver , uiReceiverLenghth );
	}

	int	iChannelNumber	=	0;

	if( eScope == SCOPE_WORLD )
	{
		UINT16	uiChannel	= 0;

		pChatPacket->ExtractUINT16( &uiChannel );

		iChannelNumber	=	(int)uiChannel;

		if( iChannelNumber < 0 || iChannelNumber > 1000 )
			iChannelNumber	=	0;
	}

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

#if !defined(_CHANNEL_TEAM_VERSION_)
	if( eScope == SCOPE_TEAM )
	{
		INT32	iTeamDBKey	=	0;

		pChatPacket->ExtractUINT32( (UINT32*)&iTeamDBKey );

		SPTeamArchive*	pTeamArchive	=	pLocalPlayer->GetTeam();

		if( pTeamArchive != NULL )
		{
			TEAM_MEMBER*	pTeamMember	=	pTeamArchive->GetCharInfo( iTeamDBKey );

			if( pTeamMember != NULL )
			{
				if( pTeamMember->wServerID == (UINT16)g_pNetworkManager->GetServerID() )
					uiSenderGUID	=	pTeamMember->CharGUID;
				else
					uiSenderGUID	=	0;
			}
		}
	}
#endif

	CHAT_TYPE_ID*	pstChatType = GetChatType( (int)eScope );

	if( pstChatType == NULL )
		return 1;

	std::string	strID			=	szSender;
	const char*	pChatFormat		=	pstChatType->m_strOutputFormat.c_str();
	SPPlayer*	pSenderPlayer	=	(SPPlayer*)g_pGOBManager->Find( uiSenderGUID );
	D3DXCOLOR	stColor			=	g_pResourceManager->GetGlobalColor( uiFontColor );

	if( pSenderPlayer != NULL )
	{
		if( pstChatType->m_eScope == SCOPE_AROUNDREGION || pstChatType->m_eScope == SCOPE_TEAM || pstChatType->m_eScope == SCOPE_GUILD )
		{
			if( uiFontColor == 0 )
				pSenderPlayer->SetTalkBalloon( szMsg , pstChatType->m_eScope , pstChatType->m_Color );
			else
				pSenderPlayer->SetTalkBalloon( szMsg , pstChatType->m_eScope , stColor );

		}
	}

	char	szLocalName[ LEN_NAME + 1 ];

	pLocalPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szLocalName );

	if( strcmp( szLocalName , szSender ) == 0 )
	{
		if( pstChatType->m_eScope == SCOPE_AROUNDREGION || pstChatType->m_eScope == SCOPE_TEAM || pstChatType->m_eScope == SCOPE_GUILD )
		{
			if( uiFontColor == 0 )
				pLocalPlayer->SetTalkBalloon( szMsg , pstChatType->m_eScope , pstChatType->m_Color );
			else
				pLocalPlayer->SetTalkBalloon( szMsg , pstChatType->m_eScope , stColor );
		}

		if( pstChatType->m_eScope == SCOPE_WHISPER )
		{
			pChatFormat	=	pstChatType->m_strInputFormat.c_str();
			strID		=	szReceiver;
			AddWhisperID( szReceiver );
		}
	}
	else
	{
		if( pstChatType->m_eScope == SCOPE_WHISPER )
		{
			pChatFormat	=	pstChatType->m_strOutputFormat.c_str();
			AddWhisperID( szSender );
			m_strRelpyName	=	szSender;
		}

		if( eScope == SCOPE_FRIENDLIST )
		{
			if( g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_USER_CHECK_FRIEND , (WPARAM)FRIEND_TYPE_WHITE , (LPARAM)szSender ) == 0 )
			{
				return 1;
			}
		}
	}

	if( pstChatType->m_eScope == SCOPE_NOTICE || pstChatType->m_eScope == SCOPE_SYSTEMMSG )
	{
		_stprintf( szBuf ,	pChatFormat , szMsg );
	}
	else
	{
		if( uiGlobalStringID == 0 )
			_stprintf( szBuf ,	pChatFormat , strID.c_str() , szMsg );
		else
		{
			pstChatType = GetChatType( (int)SCOPE_SYSTEMMSG );

			if( pstChatType == NULL )
				return 1;

			PrintErrorMsg( szBuf , uiGlobalStringID , szSender , szReceiver );
		}
	}

	D3DXCOLOR	cCurColor	=	stColor;

	if( uiFontColor == 0 )
		cCurColor	=	pstChatType->m_Color;

	AddChatString( eScope , szBuf , cCurColor , szSender , true , iChannelNumber, uiChatItemID );

	return 1;
}

int SPWindowChat::OnChatMsg( WPARAM wParam, LPARAM lParam )
{
	const char* pstrString	=	(const char*)wParam;

	if( pstrString == NULL || strlen( pstrString ) <= 0 )
		return 1;

	LIST_ITEM	stAddListItem;
	D3DXCOLOR	cWhiteColor	=	D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
	D3DXCOLOR*	pColor		=	(D3DXCOLOR*)lParam;

	if( pColor == NULL )
	{
		CHAT_TYPE_ID*	pstChatType = GetChatType( (int)SCOPE_SYSTEMMSG );

		if( pstChatType != NULL )
			stAddListItem.m_stTextColor	=	pstChatType->m_Color;
		else
			stAddListItem.m_stTextColor	=	cWhiteColor;
	}
	else
	{
		if( pColor->r == 0.0f && pColor->g == 0.0f && pColor->b == 0.0f )
		{
			*pColor	=	cWhiteColor;
		}

		stAddListItem.m_stTextColor	=	*pColor;
	}

	stAddListItem.m_strText		=	pstrString;

	if( m_pWindowTalkingRoom != NULL )
	{
		if( m_pWindowTalkingRoom->IsShow() == true )
		{
			CHAT_TOSS_DATA		stTossData;

			stTossData.m_eScope			=	SCOPE_BROADCAST;
			stTossData.m_strMsg			=	stAddListItem.m_strText;
			stTossData.m_cColor			=	stAddListItem.m_stTextColor;

			m_pWindowTalkingRoom->SPSendMessage( SPIM_TALKING_ROOM_MESSAGE , (WPARAM)&stTossData , NULL );
		}
	}

	m_pHistory->InsertItem( &stAddListItem );

	CheckLatestMsg();

	return 1;
}

int SPWindowChat::OnChatToggle( WPARAM wParam, LPARAM lParam )
{
	int	iResult	=	0;

	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
	{
		if( pChatManager->IsChatRoom() == true )
			return iResult;
	}

	if( m_pWhisperEdit->IsFocus() == TRUE && m_pWhisperEdit->IsShow() == TRUE )
		m_pWhisperEdit->SPSendMessage( SPIM_KILLFOCUS );

	std::string	strChat	=	m_pMsgEdit->GetWindowText();

	if( m_pMsgEdit->IsFocus() )
	{
		if( strChat.empty() == false )
		{
			// Enter°¡ µé¾ûÛÔÀ»¶§ È÷½ºÅä¸®¿¡ ³Ö´Â´Ù. [9/21/2006 AJJIYA]
			string strMessage = g_pInterfaceManager->GetIMEPool()->GetString();
			if(strMessage.length() > 0) {
				m_vstrHistoryBuffer.push_back( strMessage );
				if(m_vstrHistoryBuffer.size() > 10) {
					m_vstrHistoryBuffer.erase( m_vstrHistoryBuffer.begin() );
				}
				m_cInxHistory = 0;
			}

			if ( strncmp( strChat.c_str(), "//", 2 ) == 0 )
			{
				// Server Cheat
				g_pSubGameManager->ServerCommandExcute( (strChat.c_str()) + 2 );
			} else if ( strncmp( strChat.c_str(), "`", 1 ) == 0 )
			{
				// Client Cheat
				g_pSubGameManager->ClientCommandExcute( (strChat.c_str()) + 1 );
			} else {
				//Chat
				SendChatMsg( strChat.c_str() );
			}
		}

		OnEscape( NULL , NULL );

		iResult	=	0;
	}
	else
	{
		if( OnEscape( NULL , NULL ) == 0 )
		{
			m_pMsgEditPanel->Show();

			if( m_eScope == SCOPE_WHISPER )
			{
				m_pWhisperEdit->SPSendMessage( SPIM_KILLFOCUS );

				if( m_strWhisperName.empty() == false )
				{
					SPIMEPool*	pIMEPool	=	g_pInterfaceManager->GetIMEPool();

					if( pIMEPool != NULL )
					{
						const char*	pszWhisper	=	m_pWhisperEdit->GetWindowText();

						if( pszWhisper != NULL )
						{
							bool	bSetting	=	true;

							if( strlen( pszWhisper ) > 0 )
							{
								if( m_strRelpyName.compare( pszWhisper ) == 0 )
								{
									bSetting = false;
								}
							}

							if( bSetting == true )
							{
								string strMessage = pIMEPool->GetString();
								m_pWhisperEdit->SetWindowText( m_strWhisperName.c_str() );
								pIMEPool->SetString( strMessage.c_str() );
							}

						}
					}
				}
			}
			else
			{
				m_pMsgEdit->SetWindowText( NULL );
				m_pWhisperEdit->Hide();
			}

			m_pHistory->SPSendMessage( SPIM_CURSOR_ENTER );
			m_pMsgEdit->SPSendMessage( SPIM_SETFOCUS );
			g_pInterfaceManager->SetFocusWindow( m_pMsgEdit );

			OnTypeList( WIID_CHAT_TYPE_LIST + m_eScope - 1, NULL, NULL );

			iResult	=	1;
		}
	}

	if( m_pHistory->IsCursorIn( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() ) == TRUE )
		m_pHistory->SPSendMessage( SPIM_CURSOR_ENTER );

	return iResult;
}

int SPWindowChat::OnChatInsertMsgEdit( WPARAM wParam, LPARAM lParam )
{
	CHAT_STRING_ID	eChatStringID	=	(CHAT_STRING_ID)wParam;

	if( eChatStringID == CHAT_STRING_NULL )
		return 0;

	int	iNewMenuItem	=	0;		//	0 ÀüÃ¼ 1 Ä£±¸ 2 ±æµå 3 ÆÄÆ¼ 4 ±Ó¼Ó¸»

	// Ã¤ÆÃ »óÅÂ¿¡ ¸Â´Â ¿¡µðÅÍ Ã¢À» º¸¿©ÁÖÀÚ [6/16/2006]
	switch( eChatStringID )
	{
		case CHAT_STRING_FRIENDLIST:	iNewMenuItem	=	1;		break;
		case CHAT_STRING_GUILD:			iNewMenuItem	=	2;		break;
		case CHAT_STRING_TEAM:			iNewMenuItem	=	3;		break;
		case CHAT_STRING_WHISPER:		iNewMenuItem	=	4;		break;
		case CHAT_STRING_BUGLE:			iNewMenuItem	=	5;		break;
	}

	OnTypeList( WIID_CHAT_TYPE_LIST + iNewMenuItem , NULL , NULL );

	m_pMsgEdit->SPSendMessage( SPIM_KILLFOCUS );
	g_pInterfaceManager->SetFocusWindow( NULL );

	const char*	pstrChatString	=	GetChatString( eChatStringID );

	if( pstrChatString == NULL )
		return 0;

	std::string	strChatString	=	pstrChatString;
	std::string	strCmd			=	(const char*)lParam;

	if( strCmd.empty() == TRUE )
		return 0;

	if( m_pMsgEditPanel->IsShow() == FALSE )
		OnChatToggle( NULL , NULL );
	else
	{
		m_pMsgEdit->SPSendMessage( SPIM_SETFOCUS );
		g_pInterfaceManager->SetFocusWindow( m_pMsgEdit );
	}

	if( strChatString.empty() == FALSE )
	{
		if( eChatStringID != CHAT_STRING_WHISPER )
		{
			strChatString	+=	" ";
			strChatString	+=	strCmd;
			strChatString	+=	" ";

			strCmd.clear();

			strCmd	=	strChatString;
		}
		else
		{
			m_pWhisperEdit->SetWindowText( strCmd.c_str() );
			strCmd.clear();
		}
	}

	g_pInterfaceManager->GetIMEPool()->Purge();

	if( strCmd.empty() == false )
	{
		g_pInterfaceManager->GetIMEPool()->SetString( strCmd.c_str() );
		m_pMsgEdit->SetWindowText( (char *)strCmd.c_str() );
	}

	return 1;
}

int SPWindowChat::OnChatSetWhisper( WPARAM wParam, LPARAM lParam )
{
	std::string	strName		=	(char*)wParam;

	if( strName.empty() == TRUE )
		return 0;

	OnChatInsertMsgEdit( CHAT_STRING_WHISPER , (LPARAM)strName.c_str() );

	return 1;
}

int SPWindowChat::OnChatGetNotice( WPARAM wParam, LPARAM lParam )
{
	STD_VECTOR_CHAT_ADVERTISEMENT** ppChatAdvertisement = 
		reinterpret_cast< STD_VECTOR_CHAT_ADVERTISEMENT** >(wParam);

	if( ppChatAdvertisement == NULL )
		return 1;

	(*ppChatAdvertisement) = GetAdvertisement(HIWORD(lParam), LOWORD(lParam));
	return 1;
}

int SPWindowChat::OnChatSendMessage( WPARAM wParam, LPARAM lParam )
{
	CHAT_SEND_APPEND_DATA*	pstChatSendMsg	=	(CHAT_SEND_APPEND_DATA*)lParam;
	const char*				pstrMessage		=	(const char*)wParam;

	if( pstrMessage == NULL )
		return 1;

	std::string	strMessage	=	pstrMessage;

	if( strMessage.empty() == TRUE )
		return 1;

	std::string	strBufWhisperName;

	//////////////////////////////////////////////////////////////////////////

	if( strncmp( strMessage.c_str(), "//", 2 ) == 0 )
	{
		// Server Cheat
		g_pSubGameManager->ServerCommandExcute( (strMessage.c_str()) + 2 );
		return 1;
	}
	else if ( strncmp( strMessage.c_str(), "`", 1 ) == 0 )
	{
		// Client Cheat
		g_pSubGameManager->ClientCommandExcute( (strMessage.c_str()) + 1 );
		return 1;
	}

	//////////////////////////////////////////////////////////////////////////

	if( pstChatSendMsg != NULL )
	{
		SendPacketMsg( pstChatSendMsg->m_eScope , strMessage.c_str() , pstChatSendMsg->m_strWhisper.c_str() , m_strMessage , strBufWhisperName );
	}
	else
	{
		SendChatMsg( strMessage.c_str() );
	}

	g_pInterfaceManager->SetFocusWindow( NULL );

	return 1;
}

int SPWindowChat::OnChatGetCommand( WPARAM wParam, LPARAM lParam )
{
	CHAT_STRING_ID	eStringID		=	(CHAT_STRING_ID)wParam;
	char*			pszCommand		=	(char*)lParam;
	const char*		pszGetCommand	=	GetChatString( eStringID );

	if( pszGetCommand != NULL )
		strcpy( pszCommand , pszGetCommand );

	return 1;
}

int SPWindowChat::OnSetFocusMsg( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );
	return 1;
}

int SPWindowChat::OnKillFocusMsg( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->GetIMEPool()->ClearBuffer();
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}

int SPWindowChat::OnKeyDown( WPARAM wParam, LPARAM lParam )
{
	if(wParam != VK_UP && wParam != VK_DOWN ) return 1;
	if(m_vstrHistoryBuffer.empty()) return 1;

	if( m_pMsgEdit != NULL )
	{
		if( m_pMsgEdit->IsShow() == false || m_pMsgEdit->IsFocus() == false )
			return 1;
	}

	if(m_cInxHistory < 0 ) {
		m_cInxHistory = m_vstrHistoryBuffer.size() -1;
	}
	if(m_cInxHistory >= m_vstrHistoryBuffer.size() ) {
		m_cInxHistory = 0;
	}
	vector<string>::reverse_iterator rIter = m_vstrHistoryBuffer.rbegin() + m_cInxHistory;
	
	if(m_pMsgEdit) {	
		m_pMsgEdit->SetWindowText( (*rIter).c_str() );
		m_pMsgEdit->SPSendMessage( SPIM_SET_CARET_POS, CPOS_END, 0);
	}

	if(wParam == VK_UP) {
		++m_cInxHistory;
	} else if( wParam == VK_DOWN) {	
		--m_cInxHistory;
	}
	return 1;
}

int SPWindowChat::OnSetFocusWhisper( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}

int SPWindowChat::OnKillFocusWhisper( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->GetIMEPool()->ClearBuffer();
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}

int SPWindowChat::OnIMEUpEnglish( WPARAM wParam, LPARAM lParam)
{
	bool	bEnglish	=	g_pInterfaceManager->GetIMEPool()->IsEnglishMode();

	g_pInterfaceManager->SetIMEModeEnglish( false , false );
	g_pInterfaceManager->GetIMEPool()->KillFocus();

	return 1;
}

int SPWindowChat::OnIMEUpHangul( WPARAM wParam, LPARAM lParam)
{
	bool	bEnglish	=	g_pInterfaceManager->GetIMEPool()->IsEnglishMode();

	g_pInterfaceManager->SetIMEModeEnglish( true , false );
	g_pInterfaceManager->GetIMEPool()->KillFocus();

	return 1;
}

int SPWindowChat::OnScrollUp( WPARAM wParam, LPARAM lParam)
{
	m_pHistory->SPSendMessage (SPIM_SCROLL , SUB_SPIM_SCROLL_UP );
	m_iBtnDown	=	0;
	return 1;
}

int SPWindowChat::OnScrollDown( WPARAM wParam, LPARAM lParam)
{
	m_pHistory->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_DOWN );
	m_iBtnDown	=	0;

	CheckLatestMsg();

	return 1;
}

int SPWindowChat::OnScrollLatest( WPARAM wParam, LPARAM lParam)
{
	m_pHistory->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_LATEST );

	CheckLatestMsg();

	return 1;
}

int SPWindowChat::OnScrollUpLBDown( WPARAM wParam, LPARAM lParam )
{
	m_iBtnDown	=	1;

	return 1;
}

int SPWindowChat::OnScrollDownLBDown( WPARAM wParam, LPARAM lParam )
{
	m_iBtnDown	=	2;

	return 1;
}

int SPWindowChat::OnScrollUpCursorOut( WPARAM wParam, LPARAM lParam )
{
	m_iBtnDown	=	0;

	return 1;
}

int SPWindowChat::OnScrollDownCursorOut( WPARAM wParam, LPARAM lParam )
{
	m_iBtnDown	=	0;

	return 1;
}

int SPWindowChat::OnSliderPageNum( WPARAM wParam, LPARAM lParam )
{
	int	iNewLine	=	(int)wParam;
	int	iMaxLine	=	CHAT_HISTORY_COUNT + CHAT_HISTORY_VIEW_COUNT;
	int	iCurLine	=	iMaxLine	-	iNewLine;
	int	iSizeY		=	iCurLine	*	CHAT_HISTORY_LINE_HEIGHT	+	5;
	int	iPosY		=	400  -	( ( iCurLine - CHAT_HISTORY_VIEW_COUNT ) * CHAT_HISTORY_LINE_HEIGHT );		// 411 Àº 4ÁÙÀÏ¶§ÀÇ List À§Ä¡ (»ó´EÂÇ? AJJIYA [6/16/2006]

	int	iPosX , iSizeX;
	int	iTemp;

	m_pHistory->GetAbsCoord( iPosX , iTemp );

	RECT	rcSize;

	m_pHistory->GetWindowRect( rcSize );

	iSizeX	=	rcSize.right	-	rcSize.left;

	m_pHistory->AdjustCoord( iPosX - m_iAX , iPosY , iSizeX , iSizeY );
	m_pHistory->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_LATEST );

	if( iNewLine != 0 && m_bSizeSliderMax == TRUE )
	{
		m_pSizeSlider->SetImageHandleRect( m_rcSizeSliderSrc[ SLIDER_TOP ] );
		m_pSizeSlider->SetImageHandleHitRect( m_rcSizeSliderSrc[ SLIDER_TOP_HIT ] );
		m_bSizeSliderMax	=	FALSE;
	}
	else if( iNewLine == 0 && m_bSizeSliderMax == FALSE )
	{
		m_pSizeSlider->SetImageHandleRect( m_rcSizeSliderSrc[ SLIDER_BOTTOM ] );
		m_pSizeSlider->SetImageHandleHitRect( m_rcSizeSliderSrc[ SLIDER_BOTTOM_HIT ] );
		m_bSizeSliderMax	=	TRUE;
	}

	OnScrollLatest( NULL , NULL );

	ReposOwnImage();

	return 1;
}

int SPWindowChat::OnSliderSetFocus( WPARAM wParam, LPARAM lParam )
{
	m_pSizeSlider->SetColor( D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) );
	return 1;
}

int SPWindowChat::OnSliderKillFocus( WPARAM wParam, LPARAM lParam )
{
	m_bLButtonDown	=	FALSE;

	int	iXPos	=	LOWORD(lParam);
	int	iYPos	=	HIWORD(lParam);

	if( m_pHistory->IsCursorIn( iXPos , iYPos ) == true )
		m_pHistory->SPSendMessage( SPIM_CURSOR_ENTER );
	else
		m_pHistory->SPSendMessage( SPIM_CURSOR_OUT );

	return 1;
}

int SPWindowChat::OnSliderLButtonDown( WPARAM wParam, LPARAM lParam )
{
	m_bLButtonDown	=	TRUE;

	m_pHistory->SPSendMessage( SPIM_CURSOR_ENTER );

	return 1;
}

int SPWindowChat::OnSliderLButtonUp( WPARAM wParam, LPARAM lParam )
{
	m_bLButtonDown	=	FALSE;

	int	iXPos	=	LOWORD(lParam);
	int	iYPos	=	HIWORD(lParam);

	if( m_pHistory->IsCursorIn( iXPos , iYPos ) == true )
		m_pHistory->SPSendMessage( SPIM_CURSOR_ENTER );
	else
		m_pHistory->SPSendMessage( SPIM_CURSOR_OUT );

	RECT	rcSliderHandle	=	m_pSizeSlider->GetHandleTarget();

	if( rcSliderHandle.top <= iYPos && rcSliderHandle.bottom >= iYPos &&
		rcSliderHandle.left <= iXPos && rcSliderHandle.right >= iXPos )
	{
		m_pSizeSlider->SetColor( D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) );
		m_pHistory->SPSendMessage( SPIM_CURSOR_ENTER );
	}
	else
	{
		m_pSizeSlider->SPSendMessage( SPIM_KILLFOCUS );
		OnListLButtonUp( wParam , lParam );
	}

	return 1;
}

int SPWindowChat::OnSliderRButtonUp( WPARAM wParam, LPARAM lParam )
{
	int	iXPos	=	LOWORD(lParam);
	int	iYPos	=	HIWORD(lParam);

	RECT	rcSliderHandle	=	m_pSizeSlider->GetHandleTarget();

	if( rcSliderHandle.top > iYPos || rcSliderHandle.bottom < iYPos ||
		rcSliderHandle.left > iXPos || rcSliderHandle.right < iXPos )
	{
		OnListRButtonUp( wParam , lParam );
	}

	return 1;
}

int SPWindowChat::OnSliderCursorMove( WPARAM wParam, LPARAM lParam )
{
	int	iXPos	=	LOWORD(lParam);
	int	iYPos	=	HIWORD(lParam);

	if( m_pHistory->IsCursorIn( iXPos , iYPos ) == true )
		m_pHistory->SPSendMessage( SPIM_CURSOR_ENTER );
	else
		m_pHistory->SPSendMessage( SPIM_CURSOR_OUT );

	if( m_bLButtonDown == TRUE )
		m_pHistory->SPSendMessage( SPIM_CURSOR_ENTER );

	RECT	rcSliderHandle	=	m_pSizeSlider->GetHandleTarget();

	if( rcSliderHandle.top <= iYPos && rcSliderHandle.bottom >= iYPos &&
		rcSliderHandle.left <= iXPos && rcSliderHandle.right >= iXPos )
	{
		m_pSizeSlider->SetColor( D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) );
		m_pHistory->SPSendMessage( SPIM_CURSOR_ENTER );
	}

	return 0;
}

int SPWindowChat::OnSliderCursorOut( WPARAM wParam, LPARAM lParam )
{
	int	iXPos	=	LOWORD(lParam);
	int	iYPos	=	HIWORD(lParam);

	if( m_pHistory->IsCursorIn( iXPos , iYPos ) == true )
		m_pHistory->SPSendMessage( SPIM_CURSOR_ENTER );
	else
		m_pHistory->SPSendMessage( SPIM_CURSOR_OUT );

	if( m_bLButtonDown == TRUE )
		m_pHistory->SPSendMessage( SPIM_CURSOR_ENTER );

	return 1;
}

int SPWindowChat::OnPurge( WPARAM wParam, LPARAM lParam)
{
	m_pSizeSlider->SetCurpage( CHAT_HISTORY_COUNT );
	OnSliderPageNum( CHAT_HISTORY_COUNT , NULL );
	m_pSizeSlider->SPSendMessage( SPIM_POS_UPDATE );
	m_pHistory->SPSendMessage( SPIM_PURGE );
	return 1;
}

int SPWindowChat::OnTab( WPARAM wParam, LPARAM lParam )
{
	if( m_pWhisperEdit->IsShow() == true && m_pWhisperEdit->IsFocus() == true )
		g_pInterfaceManager->SetFocusWindow( m_pMsgEdit );

	return 1;
}

int SPWindowChat::OnEscape( WPARAM wParam, LPARAM lParam )
{
	DXUTOutputDebugString("OnEscape \n");
	int	iResult	=	0;
	
	if( g_pInterfaceManager->GetIMEPool()->IsComposition()) {
		return 1;
	}


	if( m_pMsgEditPanel->IsShow() == TRUE )
	{
		m_pMsgEdit->SPSendMessage( SPIM_KILLFOCUS );
		m_pMsgEdit->SetWindowText( NULL );
		m_pMsgEditPanel->Hide();
		m_pHistory->SPSendMessage( SPIM_CURSOR_OUT );
		SAFE_CALL_FUNC( g_pInterfaceManager->FindWindow( WIID_BUGLE_SELECT ), Hide());
		SAFE_CALL_FUNC( g_pInterfaceManager->FindWindow( WIID_BUGLE_MENU ), Hide() );
		iResult	=	1;
	}

	return iResult;
}

int SPWindowChat::OnWheelUp( WPARAM wParam, LPARAM lParam )
{
	OnScrollUp( NULL , NULL );

	return 1;
}

int SPWindowChat::OnWheelDown( WPARAM wParam, LPARAM lParam )
{
	OnScrollDown( NULL , NULL );

	return 1;
}

int SPWindowChat::OnCursorEnter( WPARAM wParam, LPARAM lParam )
{
	OnListCursorEnter( wParam ,lParam );
	return 1;
}

int SPWindowChat::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	OnListCursorOut( wParam , lParam );
	return 1;
}

int SPWindowChat::OnKillFocus( WPARAM wParam, LPARAM lParam )
{
	OnListCursorOut( wParam , lParam );
	return 1;
}

int SPWindowChat::OnTypeList( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	int				iSelectNum	=	iID	- WIID_CHAT_TYPE_LIST;
	CHAT_TYPE_ID tmpid;
	tmpid.m_eScope = SCOPE_BUGLE;

	CHAT_TYPE_ID*	pChatType	=	GetChatType( iSelectNum + 1 );

	if( pChatType == NULL )
	{
		//return 1;
		pChatType = &tmpid;
	}

	SPIMEPool*	pIMEPool	=	g_pInterfaceManager->GetIMEPool();

	INSTANCE_ID	eInstanceID;
	SPWindow*	pWindow;
	BOOL		bSelect;

	for( int i = 0 ; i < CHAT_MENU_COUNT ; ++i )
	{
		eInstanceID	=	WIID_CHAT_TYPE_LIST	+	i;

		pWindow	=	Find( eInstanceID , true );

		if( pWindow == NULL )
			continue;

		if( iSelectNum == i )
			bSelect	=	TRUE;
		else
			bSelect	=	FALSE;

		pWindow->SPSendMessage( SPIM_BTN_SELECT , bSelect );
	}

	int	iMsgEditStatus	=	0;

	assert( m_pWhisperEdit && m_ptrImageBugleIcon && m_ptrImageBugleName && m_ptrImageBugleBg );

	if (m_pWhisperEdit->IsShow())
	{
		m_pWhisperEdit->Hide();
	}
	if (m_ptrImageBugleIcon->IsShow())
	{
		m_ptrImageBugleIcon->Hide();
	}
	if (m_ptrImageBugleName->IsShow())
	{
		m_ptrImageBugleName->Hide();
	}
	if (m_ptrImageBugleBg->IsShow())
	{
		m_ptrImageBugleBg->Hide();
	}

	if (g_bNeedBugleHighlight)
	{
		SAFE_CALL_FUNC( m_ptrBugleSelectHL, Hide() );
		SAFE_CALL_FUNC( m_ptrBugleChannelHL, Show() ); 
	}
	else
	{
		SAFE_CALL_FUNC( m_ptrBugleSelectHL, Hide() );
		SAFE_CALL_FUNC( m_ptrBugleChannelHL, Hide() ); 
	}

	SAFE_CALL_FUNC( g_pInterfaceManager->FindWindow( WIID_BUGLE_SELECT ), Hide() );
	SAFE_CALL_FUNC( g_pInterfaceManager->FindWindow( WIID_BUGLE_MENU ), Hide() );
	m_pMsgEdit->SetLimitText( CHAT_LIMIT_NORMAL );

	switch (pChatType->m_eScope)
	{
	case SCOPE_WHISPER:
		{
			m_pWhisperEdit->Show();

			if( m_strWhisperName.empty() == false )
			{
				if( pIMEPool != NULL )
				{
					string strMessage = pIMEPool->GetString();
					m_pWhisperEdit->SetWindowText( m_strWhisperName.c_str() );
					pIMEPool->SetString( strMessage.c_str() );
				}
			}

			iMsgEditStatus	=	1;
		}
		break;
	case SCOPE_BUGLE:
		{
			// ÌáÊ¾Íæ¼ÒÔÚ´ËÆµµÀ·¢ÐÅÏ¢ÊÇÒªÖ±½Ó¿ÛÀ®°ÈÉõÖÁµã¾íµÄ
			if (!g_bWarningBugleChannel)
			{
				DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN * 4 );
				PrintErrorMsg( szErrorMsg , 60000001 , NULL , NULL );
				OnChatMsg( (WPARAM)szErrorMsg , NULL );
				g_bWarningBugleChannel = true;
			}

			m_ptrImageBugleName->Show();
			m_ptrImageBugleIcon->Show();
			m_ptrImageBugleBg->Show();
			if (g_bNeedBugleHighlight)	
			{
				m_ptrBugleSelectHL->Show();
				m_ptrBugleChannelHL->Hide();
			}

			iMsgEditStatus = 2;	//!< bugle type, adjust coord for message edit box.
			m_pMsgEdit->SetLimitText( CHAT_LIMIT_BUGLE );
		}
		break;
	default:
		break;
	}

	bool	bEnglishMode	=	true;

	if( pIMEPool != NULL )
	{
		bEnglishMode	=	pIMEPool->IsEnglishMode();
	}

	m_pMsgEdit->SPSendMessage( SPIM_SETFOCUS );
	g_pInterfaceManager->SetFocusWindow( m_pMsgEdit );

	g_pInterfaceManager->SetIMEModeEnglish( bEnglishMode , false );

	m_pMsgEdit->AdjustCoord( m_ptMsgEditPosSize[ iMsgEditStatus ].left , m_ptMsgEditPosSize[ iMsgEditStatus ].top , m_ptMsgEditPosSize[ iMsgEditStatus ].right , m_ptMsgEditPosSize[ iMsgEditStatus ].bottom );
	m_pMsgEdit->SetImageNormalHead	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , m_ptMsgEditNormalSrc[ iMsgEditStatus ].left , m_ptMsgEditNormalSrc[ iMsgEditStatus ].top , m_ptMsgEditNormalSrc[ iMsgEditStatus ].right , m_ptMsgEditNormalSrc[ iMsgEditStatus ].bottom );
	m_pMsgEdit->SetImageFocusHead	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , m_ptMsgEditFocusSrc[ iMsgEditStatus ].left , m_ptMsgEditFocusSrc[ iMsgEditStatus ].top , m_ptMsgEditFocusSrc[ iMsgEditStatus ].right , m_ptMsgEditFocusSrc[ iMsgEditStatus ].bottom );
	m_pMsgEdit->SetMargin( m_rcMsgEditMargin[ iMsgEditStatus ].x , m_rcMsgEditMargin[ iMsgEditStatus ].y );
	m_pMsgEdit->RefreshRelationCoord();

//	if( m_eScope == pChatType->m_eScope )
//		return 1;

	// ¹öÆ°À» ´­·¯¼­ ¿ÔÀ»°æ¿E?±Ó¼Ó¸»·Î º¸³»ÁÖ±EÀ§ÇØ
	m_eScope	=	pChatType->m_eScope;
//	m_eOldScope	=	m_eScope;
	m_pMsgEdit->SetWindowText( NULL );

	return 1;
}


int SPWindowChat::OnGetCalcLumpString( WPARAM wParam, LPARAM lParam )
{
	//SPWindow* pWindow = Find(WIID_CHAT_HISTORY, false); 

	return  m_pHistory->SPSendMessage(SPIM_GET_CALC_LUMP_STRING);
}



BOOL SPWindowChat::LoadChatCommandLDT()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_CHATCOMMAND , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->Chat Command Load Fail [%s] \n" , RES_FNAME_LDT_CHATCOMMAND );
#endif
		return false;
	}

	LDT_Field ldtFieldChatKey;
	LDT_Field ldtFieldChatType;
	LDT_Field ldtFieldChatTypeID;

	int				iRecordCount	=	pLDTFile->GetItemCount();
	int				iOldChatTypeID	=	0;
	int				iItemID;
	CHAT_COMMAND	eChatCommand;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetFieldFromLable( iItemID, "_ChatKey",		ldtFieldChatKey		);
		pLDTFile->GetFieldFromLable( iItemID, "_ChatType",		ldtFieldChatType	);
		pLDTFile->GetFieldFromLable( iItemID, "_ChatTypeID",	ldtFieldChatTypeID	);

		eChatCommand.m_eChatType	=	(CHAT_TYPE)ldtFieldChatType.uData.lValue;
		eChatCommand.m_iChatTypeID	=	(int)ldtFieldChatTypeID.uData.lValue;

		// ÀÏº»Àº Ä£±¸Ã¤ÆÃ ¸·´Â´Ù. [1/15/2007 AJJIYA]
		if( m_eCountryType == CT_JAPAN && eChatCommand.m_iChatTypeID == 2 )
			continue;

		m_hmChatCommand.insert( STDEXT_HASH_MAP_CHAT_COMMAND::value_type( ldtFieldChatKey.uData.pValue , eChatCommand ) );

		if( iOldChatTypeID != eChatCommand.m_iChatTypeID )
		{
			m_mChatString.insert( STD_MAP_CHAT_COMMAND_STRING::value_type( (CHAT_STRING_ID)eChatCommand.m_iChatTypeID , ldtFieldChatKey.uData.pValue ) );
			iOldChatTypeID	= eChatCommand.m_iChatTypeID;
		}
	}

	SAFE_RELEASE(pLDTFile);

	return TRUE;
}

BOOL SPWindowChat::LoadChatTypeLDT()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_CHATTYPE , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->Chat Command Load Fail [%s] \n" , RES_FNAME_LDT_CHATTYPE );
#endif
		return false;
	}

	LDT_Field ldtFieldChatName;
	LDT_Field ldtFieldChatScope;
	LDT_Field ldtFieldChatRed;
	LDT_Field ldtFieldChatGreen;
	LDT_Field ldtFieldChatBlue;
	LDT_Field ldtFieldChatInputFormat;
	LDT_Field ldtFieldChatOutputFormat;

	CHAT_TYPE_ID stTypeData;

	int		iRecordCount	= pLDTFile->GetItemCount();
	int		iItemID;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetFieldFromLable( iItemID, "_ChatName",		ldtFieldChatName			);
		pLDTFile->GetFieldFromLable( iItemID, "_ChatScope",		ldtFieldChatScope			);
		pLDTFile->GetFieldFromLable( iItemID, "_Red",			ldtFieldChatRed				);
		pLDTFile->GetFieldFromLable( iItemID, "_Green",			ldtFieldChatGreen			);
		pLDTFile->GetFieldFromLable( iItemID, "_Blue",			ldtFieldChatBlue			);
		pLDTFile->GetFieldFromLable( iItemID, "_InputFormat",	ldtFieldChatInputFormat		);
		pLDTFile->GetFieldFromLable( iItemID, "_OutputFormat",	ldtFieldChatOutputFormat	);

		stTypeData.m_strType			=	ldtFieldChatName.uData.pValue;
		stTypeData.m_eScope				=	(BROADCAST_SCOPE)(ldtFieldChatScope.uData.lValue);
		stTypeData.m_Color				=	D3DCOLOR_ARGB( 255 ,	ldtFieldChatRed.uData.lValue ,
																	ldtFieldChatGreen.uData.lValue	,
																	ldtFieldChatBlue.uData.lValue );
		//	D3DCOLOR DWORD ¼ø¼­ [8/26/2005]
		//r = f * (FLOAT) (unsigned char) (dw >> 16);
		//g = f * (FLOAT) (unsigned char) (dw >>  8);
		//b = f * (FLOAT) (unsigned char) (dw >>  0);
		//a = f * (FLOAT) (unsigned char) (dw >> 24);

		stTypeData.m_strInputFormat		=	ldtFieldChatInputFormat.uData.pValue;
		stTypeData.m_strOutputFormat	=	ldtFieldChatOutputFormat.uData.pValue;

		m_mChatTypeID.insert( STD_MAP_CHAT_TYPE_ID::value_type( (CHAT_STRING_ID)iItemID , stTypeData ) );
	}

	SAFE_RELEASE(pLDTFile);

	return TRUE;
}

bool SPWindowChat::LoadChatAdvertisement()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_CHATADVERTISEMENT , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->Chat Advertisement Load Fail [%s] \n" , RES_FNAME_LDT_CHATADVERTISEMENT );
#endif
		return false;
	}

	LDT_Field ldtFieldChatStageID;
	LDT_Field ldtFieldChatMapGroupID;
	LDT_Field ldtFieldChatShow;
	LDT_Field ldtFieldChatLimitTime;
	LDT_Field ldtFieldChatColor;
	LDT_Field ldtFieldChatString;

	CHAT_ADVERTISEMENT				stAdvertismentData;
	STD_VECTOR_CHAT_ADVERTISEMENT	vAdvertismentData;

	int		iRecordCount	= pLDTFile->GetItemCount();
	int		iItemID;
	int		iKey;
	int		iOldKey;
	vAdvertismentData.clear();

	for( int iRecord = 0 ; iRecord < iRecordCount ; ++iRecord )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetFieldFromLable( iItemID, "_Stage",			ldtFieldChatStageID			);
		pLDTFile->GetFieldFromLable( iItemID, "_MapGroup",		ldtFieldChatMapGroupID		);
		pLDTFile->GetFieldFromLable( iItemID, "_Block",			ldtFieldChatShow			);
		pLDTFile->GetFieldFromLable( iItemID, "_Time",			ldtFieldChatLimitTime		);
		pLDTFile->GetFieldFromLable( iItemID, "_Color ID",		ldtFieldChatColor			);
		pLDTFile->GetFieldFromLable( iItemID, "_Message",		ldtFieldChatString			);

		iKey	=	ldtFieldChatStageID.uData.lValue * CHAT_ADVERTISEMENT_SEPARATOR_KEY + ldtFieldChatMapGroupID.uData.lValue;

		if( iRecord == 0 )
		{
			iOldKey	=	iKey;
		}

		if(	iKey != iOldKey	)
		{
			m_mChatAdvertisment.insert( STD_MAP_CHAT_ADVERTISEMENT::value_type( iOldKey , vAdvertismentData )	);
			vAdvertismentData.clear();
		}

		if( ldtFieldChatShow.uData.lValue == 1 )
			stAdvertismentData.m_bShow		=	true;
		else
			stAdvertismentData.m_bShow		=	false;

		stAdvertismentData.m_iStageID		=	ldtFieldChatStageID.uData.lValue;
		stAdvertismentData.m_iMapGroupID	=	ldtFieldChatMapGroupID.uData.lValue;
		stAdvertismentData.m_fLimitTime		=	(float)ldtFieldChatLimitTime.uData.lValue;
		stAdvertismentData.m_iColor			=	ldtFieldChatColor.uData.lValue;
		stAdvertismentData.m_strMessage		=	ldtFieldChatString.uData.pValue;
		stAdvertismentData.m_cColor			=	g_pResourceManager->GetGlobalColor( stAdvertismentData.m_iColor );

		if( stAdvertismentData.m_strMessage.empty() == true )
			continue;

		iOldKey	=	iKey;
		vAdvertismentData.push_back( stAdvertismentData );
	}

	SAFE_RELEASE(pLDTFile);

	time_t	tms;
	time(&tms);
	srand( (unsigned int)tms );

	return true;
}

CHAT_TYPE SPWindowChat::ParserChat( BROADCAST_SCOPE& eScope , const char* pstrChatMsg , const char* pszWhisperID , std::string& strMessage , std::string& strWhisperName )
{
	CHAT_TYPE	eChatType	=	CHAT_TYPE_NULL;

	if( pstrChatMsg == NULL )
		return eChatType;

	std::string	strChatMsg	=	pstrChatMsg;

	strWhisperName.clear();
	strMessage.clear();

	if( strChatMsg.empty() == TRUE )
		return eChatType;

	int	iFindSpace	=	(int)strChatMsg.find( " " );

	if( iFindSpace != -1 )
	{
		std::string	strCommand	=	strChatMsg.substr( 0 , iFindSpace );
		
		// 
		// exception test code by metalgeni
		if(strCommand.length() > 0 ) {
			WCHAR* pBuffer = new WCHAR[strCommand.length()+1];
			CHAR* pAnsiBuffer = new CHAR[strCommand.length() * 2 + 1];

			SPAbuseFilter::GetInstance()->ConvertAnsiStringToWide(pBuffer, strCommand.c_str() );		
			SPAbuseFilter::GetInstance()->ConvertWideToAnsiString(pAnsiBuffer, pBuffer );
			strCommand = pAnsiBuffer;

			delete [] pBuffer;
			delete [] pAnsiBuffer;
		}

		iFindSpace++;

		strMessage	=	strChatMsg.substr( iFindSpace , strChatMsg.length() - iFindSpace );

		if( strCommand.empty() == TRUE )
			return eChatType;

		eChatType	=	ChangeChatType( eScope , strCommand.c_str() , TRUE );

		if( eChatType == CHAT_TYPE_CHAT )
		{
			if( eScope == SCOPE_WHISPER )
			{
				int	iFindWhisperName	=	(int)strMessage.find( " " );

				if( iFindWhisperName != -1 )
				{
					strWhisperName	=	strMessage.substr( 0 , iFindWhisperName );
					iFindWhisperName++;
					strMessage		=	strMessage.substr( iFindWhisperName , strMessage.length() - iFindWhisperName );
				}
			}
		}
		else if( eChatType != CHAT_TYPE_EMOTICON )
		{
			if( strCommand.at( 0 ) == '/' )
			{
				strMessage.clear();
			}
			else
			{
				strCommand	+=	" ";
				strCommand	+=	strMessage;
				strMessage	=	strCommand;
			}
		}
	}
	else
	{
		strMessage	=	pstrChatMsg;

		if( strMessage.at( 0 ) == '/' )
		{
			eChatType	=	ChangeChatType( eScope , strMessage.c_str() , TRUE );

			if( eChatType != CHAT_TYPE_CHAT )
				strMessage.clear();
		}
		else
		{
			if( pszWhisperID != NULL )
			{
				if( strlen( pszWhisperID ) > 0 && eScope == SCOPE_WHISPER )
				{
					strWhisperName	=	pszWhisperID;
				}
			}
		}
	}

	return eChatType;
}

void SPWindowChat::SendChatMsg( const char* pstrChatMsg )
{
	SendPacketMsg( m_eScope , pstrChatMsg , m_pWhisperEdit->GetWindowText() , m_strMessage , m_strWhisperName );

	OnScrollLatest( NULL , NULL );

	//	ÇöÀçÀÇ m_eScope·Î Ã¤ÆÃÃ¢ º¯°EAJJIYA [6/16/2006]
	OnTypeList( WIID_CHAT_TYPE_LIST + m_eScope - 1 , NULL , NULL );

	//// ±Ó¼Ó¸»Àº ÀúÀåÇÏÁE¾Ê´Â´Ù. ÀÌÀE°ªÀ¸·Î º¹±Í [4/14/2006]
	//if( m_eScope == SCOPE_WHISPER )
	//{
	//	m_eScope	=	m_eOldScope;
	//}

	return;
}

void SPWindowChat::SendPacketMsg( BROADCAST_SCOPE& eScope , const char* pstrChatMsg , const char* pszWhisperID , std::string& strMessage , std::string& strWhisperName )
{
	char szBuffer[1025];
	SPAbuseFilter::GetInstance()->Filter( pstrChatMsg , szBuffer , 1024 );

	pstrChatMsg	=	szBuffer;

	if( IsChatCommand( eScope , pstrChatMsg , pszWhisperID , strMessage , strWhisperName ) == TRUE )
		return;

	if( m_eCountryType == CT_JAPAN && eScope == SCOPE_FRIENDLIST )
		return;

	if( eScope == SCOPE_AROUNDREGION || eScope == SCOPE_FRIENDLIST )
	{
		if( GetRepeat() == true )
		{
			DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

			PrintErrorMsg( szErrorMsg , 4000005 , NULL , NULL );

			OnChatMsg( (WPARAM)szErrorMsg , NULL );

			return;
		}

		SetRepeat();
	}

	if( eScope == SCOPE_TEAM )
	{
		SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer != NULL )
		{
			SPTeamArchive*	pTeamArchive	=	pLocalPlayer->GetTeam();

			if( pTeamArchive != NULL )
			{
				if( pTeamArchive->GetTeamCount() < 2 )
				{
					DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

					PrintErrorMsg( szErrorMsg , 5002000 , NULL , NULL );

					OnChatMsg( (WPARAM)szErrorMsg , NULL );

					return;
				}
			}
		}
	}

	if( eScope == SCOPE_GUILD )
	{
		SPGuildArchive*	pGuildArchive	=	g_pGOBManager->GetGuildArchive();

		if( pGuildArchive != NULL )
		{
			if( pGuildArchive->GetBasicAvailable() == false )
			{
				DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

				PrintErrorMsg( szErrorMsg , 27000060 , NULL , NULL );

				OnChatMsg( (WPARAM)szErrorMsg , NULL );

				return;
			}
		}
	}

	UINT32		uiDestination	=	0;
	UINT8		iMessageLength	=	(int)strMessage.size() + 1;
	CPacket		ChatPacket;

	if( iMessageLength <= 1 )
		return;

	// todo: modify here to process SCOPE_BUGLE
	if (SCOPE_BUGLE == m_eScope)
	{
		/*SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer == NULL )
			return;

		SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();

		if( pInvenArchive == NULL )
			return;

		SPItem*	pRequireItem	=	pInvenArchive->GetSPItem( INVENTORY_CONSUME , 170200101 );

		if( pRequireItem == NULL )
			return;

		if( pInvenArchive->SetMicItem( pRequireItem->GetContainerType() , pRequireItem->GetSlotIndex() ) == false )
			return;

		if( pInvenArchive->SendMicItemUseMsg( m_strMessage.c_str() ) == true )
			return;

		pInvenArchive->ClearMicItem();*/

		_sendBugleMsg( m_strMessage.c_str() );

		return;
	}


	ChatPacket.AddCommand( CHAT_CS_MSG );
	ChatPacket.Add( (UINT32)eScope );
	ChatPacket.Add( uiDestination );
	ChatPacket.Add( iMessageLength );
	ChatPacket.Add( (char*)strMessage.c_str() , iMessageLength );

	if( eScope == SCOPE_WHISPER )
	{
		UINT8		iWhisperLength	=	(int)strWhisperName.size() + 1;

		if( iWhisperLength < 2 || iWhisperLength > LEN_NAME + 1 )
		{
			DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

			PrintErrorMsg( szErrorMsg , 4000001 , NULL , NULL );

			OnChatMsg( (WPARAM)szErrorMsg , NULL );

			return;
		}

		SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer == NULL )
			return;

		char	szLocalName[ LEN_NAME + 1 ];

		pLocalPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szLocalName );

		if( strWhisperName.compare( szLocalName ) == 0 )
		{
			DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

			PrintErrorMsg( szErrorMsg , 5003000 , NULL , NULL );

			OnChatMsg( (WPARAM)szErrorMsg , NULL );

			return;
		}

		ChatPacket.Add( iWhisperLength );
		ChatPacket.Add( (char*)strWhisperName.c_str() , iWhisperLength );
	}

	g_pNetworkManager->SPPerformMessage( CHAT_CS_MSG , NULL , (LPARAM)&ChatPacket );
}

CHAT_TYPE_ID* SPWindowChat::GetChatType( int iTypeNumber )
{
	STD_MAP_CHAT_TYPE_ID::iterator	mIter	=	m_mChatTypeID.find( (CHAT_STRING_ID)iTypeNumber );

	if( mIter == m_mChatTypeID.end() )
		return NULL;

	return &((*mIter).second);
}

void SPWindowChat::AddWhisperID( const char* pstrID )
{
	std::string	strID	=	pstrID;

	if( strID.compare( m_pWhisperEdit->GetWindowText() ) == 0 || strID.empty() == TRUE )
		return;

//	m_pWhisperEdit->SetWindowText( (char*)pstrID );
}

CHAT_TYPE SPWindowChat::CheckChatCommand( CHAT_COMMAND& tChatCommand )
{
	CHAT_TYPE	eResult	=	CHAT_TYPE_NULL;

	if( tChatCommand.m_eChatType == CHAT_TYPE_EMOTICON )
	{
		SPPlayerActionManager::GetInstance()->AddAction( ACTION_SKILL );
		SPPlayerActionManager::GetInstance()->SetSkillID( tChatCommand.m_iChatTypeID );
		eResult	=	tChatCommand.m_eChatType;
	}

	return eResult;
}

CHAT_TYPE SPWindowChat::ChangeChatType( BROADCAST_SCOPE& eScope , const char* pstrCommand , BOOL bSendMsg )
{
	if( pstrCommand == NULL )
		return CHAT_TYPE_NULL;

	CHAT_COMMAND*	pChatCommand	=	GetChatCommand( pstrCommand );

	if( pChatCommand == NULL )
		return CHAT_TYPE_NULL;

	if( pChatCommand->m_eChatType != CHAT_TYPE_CHAT )
	{
		if( bSendMsg == TRUE )
			return CheckChatCommand( *pChatCommand );

		return CHAT_TYPE_NULL;
	}

	int				iChatTypeID		=	pChatCommand->m_iChatTypeID;
	CHAT_TYPE_ID*	pstChatType		=	GetChatType( iChatTypeID );

	if( pstChatType == NULL )	return CHAT_TYPE_NULL;

	if( pstChatType->m_eScope == SCOPE_AROUNDREGION	||
		pstChatType->m_eScope == SCOPE_FRIENDLIST	||
		pstChatType->m_eScope == SCOPE_GUILD		||
		pstChatType->m_eScope == SCOPE_TEAM			||
		pstChatType->m_eScope == SCOPE_GUILD		||
		pstChatType->m_eScope == SCOPE_WHISPER			)
	{
		// ±Ó¼Ó¸»Àº ÀúÀåÇÏÁE¾Ê´Â´Ù ÀÌÀEªÀ¸·?µ¹¸®±EÀ§ÇØ [4/14/2006]
//		m_eOldScope	=	m_eScope;
		eScope	=	pstChatType->m_eScope;
		//OnTypeList( WIID_CHAT_TYPE_LIST + eScope - 1 , NULL , NULL );
	}

	return CHAT_TYPE_CHAT;
}

void SPWindowChat::InitActionKey()
{
	g_pInputManager->SetActionMap( KEY_AM_CHAT_REPLY , SPKL_R );
}

void SPWindowChat::ProcessActionKey()
{
	SPMInputKey*	pActionKey	=	g_pInputManager->GetActionMap( KEY_AM_CHAT_REPLY );

	if( pActionKey == NULL )	return;

	if( g_pInterfaceManager->GetIMEPool()->IsFocus() == TRUE )
		return;

	if( pActionKey->InputState == INPUT_UNPRESS )
	{
		if( m_strRelpyName.empty() == TRUE )
			return;

		OnChatInsertMsgEdit( CHAT_STRING_WHISPER , (LPARAM)m_strRelpyName.c_str() );
	}
}

void SPWindowChat::PrintErrorMsg( TCHAR* pstrOutMsg , GLOBAL_STRING_ID uiGlobalStringID , const char* pstrSender , const char* pstrReceiver )
{
	if( pstrOutMsg == NULL )
		return;

	const char*	pszGlobalString	=	g_pResourceManager->GetGlobalString( uiGlobalStringID );

	if( pszGlobalString == NULL )
		return;

	if( uiGlobalStringID >= CHAT_GLOBAL_MSG_ONE_METHOD && uiGlobalStringID < CHAT_GLOBAL_MSG_TWO_METHOD )
	{
		sprintf( pstrOutMsg , pszGlobalString , pstrReceiver );
	}
	else if( uiGlobalStringID >= CHAT_GLOBAL_MSG_TWO_METHOD && uiGlobalStringID < CHAT_GLOBAL_MSG_END_METHOD )
	{
		sprintf( pstrOutMsg , pszGlobalString , pstrSender , pstrReceiver );
	}
	else
	{
		strcpy( pstrOutMsg , pszGlobalString );
	}
}

void SPWindowChat::CheckLatestMsg()
{
	if( m_pHistory->IsAddString() == true )
		m_bInsertChatMsg	=	TRUE;
	else
	{
		m_bInsertChatMsg	=	FALSE;
		m_pBtnLatest->SPSendMessage( SPIM_BTN_SELECT , false );
	}
}

void SPWindowChat::BtnPush( int iBtnDown )
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

CHAT_COMMAND* SPWindowChat::GetChatCommand( const char* pstrCommand )
{
	if( pstrCommand == NULL )
		return NULL;

	std::string	strCommand	=	pstrCommand;

	STDEXT_HASH_MAP_CHAT_COMMAND::iterator	hmIter	=	m_hmChatCommand.find( strCommand );

	if( hmIter != m_hmChatCommand.end() && ( strCommand == (*hmIter).first ) ) 
		return &((*hmIter).second);

	return NULL;
}

void SPWindowChat::MakeHelpMessage()
{
	const char*	pstrHelp	=	GetChatString( CHAT_STRING_HELP );

	if( pstrHelp == NULL )
		return;

	CHAT_COMMAND*	pChatCommand	=	GetChatCommand( pstrHelp );

	if( pChatCommand == NULL )
		return;

	CHAT_TYPE_ID*	pstChatType		=	GetChatType( pChatCommand->m_iChatTypeID );

	if( pstChatType == NULL )
		return;

	const char* ps = g_pResourceManager->GetGlobalString( 5007001 );

	if(ps)
	{
		std::string		strHelpString	=	ps;
		char*			pMsgHelp;

		pMsgHelp	=	new	char[ (int)pstChatType->m_strOutputFormat.size() + (int)strHelpString.size() ];

		sprintf( pMsgHelp , pstChatType->m_strOutputFormat.c_str() , strHelpString.c_str() );

		m_strHelpMessage	=	pMsgHelp;
		m_cHelpMessage		=	pstChatType->m_Color;
		m_strHelpCommand	=	pstrHelp;

		SAFE_DELETE_ARRAY( pMsgHelp );
	}
}

const char* SPWindowChat::GetChatString( CHAT_STRING_ID eStringID )
{
	STD_MAP_CHAT_COMMAND_STRING::iterator	mIter	=	m_mChatString.find( eStringID );

	if( mIter != m_mChatString.end() && ( eStringID == (*mIter).first ) )
		return (*mIter).second.c_str();

	return NULL;
}

BOOL SPWindowChat::IsChatCommand( BROADCAST_SCOPE& eScope , const char* pstrCommand , const char* pszWhisperID , std::string& strMessage , std::string& strWhisperName )
{
	if( pstrCommand == NULL )
		return FALSE;

	std::string	strCommand		=	pstrCommand;
	std::string strChatString	=	m_strHelpCommand;
	int			iResult			=	-1;
	std::string strOnlyCommand;
	std::string strTargetName;

	if( strCommand.size() > 1 )
	{
		DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );
		PrintErrorMsg( szErrorMsg , 5007006 , NULL , NULL );

		//	/? Ä¿¸ÇµE°Ë»E
		if( strChatString.empty() == FALSE )
		{
			iResult	=	(int)strCommand.find( strChatString );

			if( iResult == 0 )
			{
				if( m_pWindowTalkingRoom != NULL )
				{
					if( m_pWindowTalkingRoom->IsShow() == true )
						m_pWindowTalkingRoom->SPSendMessage( SPIM_TALKING_ROOM_NEXTLINE , (WPARAM)true , NULL );

				}

				m_pHistory->SetNextLine( true );
				OnChatMsg( (WPARAM)m_strHelpMessage.c_str() , (LPARAM)&m_cHelpMessage );
				return TRUE;
			}

			iResult	=	-1;
		}

		iResult	=	(int)strCommand.find( " " );

		if( iResult != -1 )
		{
			strOnlyCommand	=	strCommand.substr( 0 , iResult );
			strTargetName	=	strCommand.substr( iResult + 1 , (int)strCommand.size() );
		}

		if( strOnlyCommand.size() <= 0 )
			strOnlyCommand = strCommand;

		CHAT_COMMAND*	pChatCommand	=	GetChatCommand( strOnlyCommand.c_str() );
		SPPlayer*		pkLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( g_pEventManager != NULL )
		{
			if( g_pEventManager->GetCutInState() == CUT_IN_STATE_LOVE )
				pChatCommand = NULL;
		}

		if( pChatCommand != NULL && pkLocalPlayer != NULL )
		{
			if( pChatCommand->m_eChatType != CHAT_TYPE_CHAT )
			{
				bool	bEmoticon	=	true;

				switch( pChatCommand->m_iChatTypeID )
				{
				case CHAT_STRING_TEAM_ADD:
					if( (int)strTargetName.size() > 0 )
					{
						pkLocalPlayer->SPGOBPerformMessage( SPGM_TEAM_INVITE_REQ , (LPARAM)strTargetName.c_str() );
						return TRUE;
					}
					else
					{
						bEmoticon	=	false;
					}
					break;
				case CHAT_STRING_TEAM_LEAVE:
					{
						char	szLocalName[ LEN_NAME + 1 ];
						pkLocalPlayer->SPGOBPerformMessage( SPGM_GETGOBNAME , (LPARAM)szLocalName );
						pkLocalPlayer->SPGOBPerformMessage( SPGM_TEAM_SECEDE_REQ , (LPARAM)szLocalName );
						return TRUE;
					}
					break;
				case CHAT_STRING_WHITE_FRIEND_ADD:
					if( (int)strTargetName.size() > 0 )
					{
						g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_USER_ADD_FRIEND , (WPARAM)FRIEND_TYPE_WHITE , (LPARAM)strTargetName.c_str() );
						return TRUE;
					}
					else
					{
						bEmoticon	=	false;
					}
					break;
				case CHAT_STRING_WHITE_FRIEND_DEL:
					if( (int)strTargetName.size() > 0 )
					{
						g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_USER_DEL_FRIEND , (WPARAM)FRIEND_TYPE_WHITE , (LPARAM)strTargetName.c_str() );
						return TRUE;
					}
					else
					{
						bEmoticon	=	false;
					}
					break;
				case CHAT_STRING_BLACK_FRIEND_ADD:
					if( (int)strTargetName.size() > 0 )
					{
						g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_USER_ADD_FRIEND , (WPARAM)FRIEND_TYPE_BLACK , (LPARAM)strTargetName.c_str() );
						return TRUE;
					}
					else
					{
						bEmoticon	=	false;
					}
					break;
				case CHAT_STRING_BLACK_FRIEND_DEL:
					if( (int)strTargetName.size() > 0 )
					{
						g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_USER_DEL_FRIEND , (WPARAM)FRIEND_TYPE_BLACK , (LPARAM)strTargetName.c_str() );
						return TRUE;
					}
					else
					{
						bEmoticon	=	false;
					}
					break;
				case CHAT_STRING_TRADE:
					if( (int)strTargetName.size() > 0 )
					{
						g_pInterfaceManager->SPChildWndSendMessage( WIID_PC_INFO , SPIM_TRADE_REQUEST , (WPARAM)strTargetName.c_str() , NULL );
						return TRUE;
					}
					else
					{
						bEmoticon	=	false;
					}
					break;
				case CHAT_STRING_GUILD_ADD:
					if( (int)strTargetName.size() > 0 )
					{
						if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) )
						{
							SPGuildArchive*	pkGuildArchive	=	g_pGOBManager->GetGuildArchive();

							if( pkGuildArchive != NULL )
								pkGuildArchive->SendGuildInvite( strTargetName.c_str() );

							return TRUE;
						}
					}
					else
					{
						bEmoticon	=	false;
					}
					break;
				case CHAT_STRING_GUILD_LEAVE:
					{
						if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) )
						{
							SPGuildArchive*	pkGuildArchive	=	g_pGOBManager->GetGuildArchive();

							if( pkGuildArchive != NULL )
								pkGuildArchive->SendGuildLeave();

							return TRUE;
						}
					}
					break;
				case CHAT_STRING_FATIGUE:
					{
						if( g_pNetworkManager != NULL )
						{
							g_pNetworkManager->FDS_QueryState();
							return TRUE;
						}
					}
					break;
				case CHAT_STRING_CHAT_MACRO:
					{
						if( g_pInterfaceManager != NULL )
						{
							g_pInterfaceManager->ShowWindow( WIID_CHAT_MACRO , true );
							return TRUE;
						}
					}
					break;
				}

				if( bEmoticon == false )
				{
					OnChatMsg( (WPARAM)szErrorMsg , NULL );
					return TRUE;
				}
			}
		}
	}

	//	/°Ë»E!!

	CHAT_TYPE	eChatType	=	ParserChat( eScope , pstrCommand , pszWhisperID , strMessage , strWhisperName );

	if( strMessage.empty() == TRUE && eChatType == CHAT_TYPE_NULL )
	{
		std::string	strChatMsg	=	pstrCommand;

		if( strChatMsg.length() != 0 )
		{
			if( strChatMsg.at( 0 ) == '/' )
			{
				DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );
				PrintErrorMsg( szErrorMsg , 5007000 , NULL , NULL );
				OnChatMsg( (WPARAM)szErrorMsg , NULL );
			}
		}

		return TRUE;
	}

	//	Ã¤ÆÃ »óÅÂ ÀÏ¶§
	if( eChatType == CHAT_TYPE_NULL && eScope == SCOPE_WHISPER )
	{
		if( pszWhisperID != NULL )
            strWhisperName	=	pszWhisperID;

		if( pstrCommand != NULL )
			strMessage	=	pstrCommand;
	}

	// ÀÌ¸ðÆ¼ÄÜÀÏ°æ¿E½ºÅµÇÏÀÚ
	if( eChatType == CHAT_TYPE_EMOTICON && strMessage.empty() == TRUE )
		return TRUE;

	//	¿ÀÁEÄ¿¸ÇµåÀÏ°æ¿E?±×³É ½ºÅµ
	strChatString	=	pstrCommand;
	if( eChatType == CHAT_TYPE_CHAT && strChatString.find( " " ) == -1 )
		return TRUE;

	return FALSE;
}

void SPWindowChat::ProcessAdvertisement( float fTime )
{
	m_fChatAdvertismentAccmulateTime	+=	fTime;

	if( m_fChatAdvertismentAccmulateTime >= m_fChatAdvertismentLimitTime )
	{
		m_fChatAdvertismentAccmulateTime	=	0.0f;
		RunAdvertisement();
	}
}

void SPWindowChat::RunAdvertisement()
{
	if( g_pClientOption != NULL )
	{
		SPOptionStruct*	pOptionStruct	=	g_pClientOption->GetOptionStruct();

		if( pOptionStruct != NULL )
		{
			if( pOptionStruct->bHelp == false )
				return;
		}
	}

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	if( m_mChatAdvertisment.size() == 0 )
		return;

	int	iCurStageID		=	-1;
	int	iCurMapGroupID	=	-1;

	if( ( m_iChatAdvertismentType & CHAT_ADVERTISEMENT_STAGE ) == CHAT_ADVERTISEMENT_STAGE )
	{
		iCurStageID		=	pLocalPlayer->GetCurStageID();
	}

	if( ( m_iChatAdvertismentType & CHAT_ADVERTISEMENT_MAPGROUP ) == CHAT_ADVERTISEMENT_MAPGROUP )
	{
		iCurMapGroupID	=	pLocalPlayer->GetCurMapGroupID();
	}

	m_iChatAdvertismentType++;

	if( m_iChatAdvertismentType >= CHAT_ADVERTISEMENT_END )
		m_iChatAdvertismentType	=	CHAT_ADVERTISEMENT_NULL;

	STD_VECTOR_CHAT_ADVERTISEMENT*	pvAdvertisement	=	GetAdvertisement( iCurStageID , iCurMapGroupID );

	if( pvAdvertisement == NULL )
	{
		RunAdvertisement();
		return;
	}

	std::vector< int >	vDice;
	int	iAdvertisementSize	=	(int)pvAdvertisement->size();

	if( iAdvertisementSize <= 0 )
		return;

	for( int i = 0 ; i < iAdvertisementSize ; ++i )
	{
		if( pvAdvertisement->at( i ).m_bShow == true )
		{
			vDice.push_back( i );
		}
	}

	int	iRandomDice	=	(int)vDice.size() - 1;

	if( iRandomDice < 0 )
		return;

	CHAT_ADVERTISEMENT*	pChatAdvertisementData	=	&( pvAdvertisement->at( vDice.at( GetRandom( iRandomDice ) ) ) );

	m_fChatAdvertismentLimitTime	=	pChatAdvertisementData->m_fLimitTime;

	if( g_pMainGameManager->GetSubGameID() != SUBGAME_LOADING && m_bShow == true )
		OnChatMsg( (WPARAM)pChatAdvertisementData->m_strMessage.c_str() , (LPARAM)&pChatAdvertisementData->m_cColor );
}

STD_VECTOR_CHAT_ADVERTISEMENT* SPWindowChat::GetAdvertisement(int iStageID , int iMapGroupID )
{
	int	iKey	=	iStageID * CHAT_ADVERTISEMENT_SEPARATOR_KEY + iMapGroupID;
	STD_MAP_CHAT_ADVERTISEMENT::iterator	mIter	=	m_mChatAdvertisment.find( iKey );

	if( mIter == m_mChatAdvertisment.end() || (*mIter).first != iKey )
		return NULL;

	return &((*mIter).second);
}

void SPWindowChat::ProcessRepeat( float fTime )
{
	if( m_bRepeat == true )
	{
		m_fRepeatAccmulateTime	+=	fTime;

		if( GetRepeat() == false )
			m_bRepeat	=	false;
	}
}

void SPWindowChat::SetRepeat()
{
	m_bRepeat				=	true;
	m_fRepeatAccmulateTime	=	0.0f;
}

bool SPWindowChat::GetRepeat()
{
	if( m_fRepeatAccmulateTime > m_fRepeatLimitTime )
		return false;

	return true;
}

void SPWindowChat::ReposOwnImage()
{
	if( m_pHistory == NULL )
		return;

	RECT	rcPos;

	m_pHistory->GetWindowRect( rcPos );

	m_rcBGDest[ 0 ].left	=	m_iAX;
	m_rcBGDest[ 0 ].top		=	rcPos.top - 7;		// 7Àº History¸®¿Í Slider¹öÆ°°£ÀÇ °£°Ý AJJIYA [6/16/2006]
	m_rcBGDest[ 0 ].right	=	m_iAX + m_iSX;
	m_rcBGDest[ 0 ].bottom	=	m_rcBGDest[ 0 ].top + 2;

	m_rcBGDest[ 1 ].left	=	m_iAX;
	m_rcBGDest[ 1 ].top		=	m_rcBGDest[ 0 ].bottom;
	m_rcBGDest[ 1 ].right	=	m_iAX + m_iSX;
	m_rcBGDest[ 1 ].bottom	=	m_iAY + m_iSY;
}

bool SPWindowChat::IsCursorInCheck()
{
	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	for( int i = 0 ; i < CHAT_BG_COUNT ; ++i )
	{
		if( m_rcBGDest[ i ].left	<= iCursorX &&
			m_rcBGDest[ i ].top		<= iCursorY	&&
			m_rcBGDest[ i ].right	>= iCursorX &&
			m_rcBGDest[ i ].bottom	>= iCursorY )
		{
			return true;
		}
	}

	return false;
}

void SPWindowChat::AddChatString( CHAT_TOSS_DATA& stChatData )
{
	LIST_ITEM	stAddListItem;
	LIST_ICON	stAddListIcon;

	if( stChatData.m_eScope == SCOPE_CHANNEL )
	{
		stAddListItem.m_stBGImage.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
		stAddListItem.m_stBGImage.m_rcSrc.left		=	413;
		stAddListItem.m_stBGImage.m_rcSrc.top		=	416;
		stAddListItem.m_stBGImage.m_rcSrc.right		=	415;
		stAddListItem.m_stBGImage.m_rcSrc.bottom	=	430;
		stAddListItem.m_stBGImage.m_iSX				=	468;
		stAddListItem.m_stBGImage.m_iSY				=	14;

		stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
		stAddListIcon.m_rcSrc.left		=	18;
		stAddListIcon.m_rcSrc.top		=	14;
		stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 20;
		stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		stAddListItem.m_vIconImage.push_back( stAddListIcon );

		stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
		stAddListIcon.m_rcSrc.left		=	50;
		stAddListIcon.m_rcSrc.top		=	14;
		stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 2;
		stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		stAddListItem.m_vIconImage.push_back( stAddListIcon );
	}
	else if( stChatData.m_eScope == SCOPE_WORLD )
	{
		AddChatString( stChatData.m_eScope, stChatData.m_strMsg.c_str(), stChatData.m_cColor,
			stChatData.m_strID.c_str(), stChatData.m_bWhisper, stChatData.m_iChannelNumber, 170200101 );
		return;
		//stAddListItem.m_stBGImage.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
		//stAddListItem.m_stBGImage.m_rcSrc.left		=	418;
		//stAddListItem.m_stBGImage.m_rcSrc.top		=	416;
		//stAddListItem.m_stBGImage.m_rcSrc.right		=	420;
		//stAddListItem.m_stBGImage.m_rcSrc.bottom	=	430;
		//stAddListItem.m_stBGImage.m_iSX				=	468;
		//stAddListItem.m_stBGImage.m_iSY				=	14;

		//stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/BUGLE.PNG";
		//stAddListIcon.m_rcSrc.left		=	18;
		//stAddListIcon.m_rcSrc.top		=	14;
		//stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 20;
		//stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		//stAddListItem.m_vIconImage.push_back( stAddListIcon );

		//char	szBuf[_MAX_PATH]	=	{};
		//itoa( stChatData.m_iChannelNumber , szBuf , 10 );
		//if (stChatData.m_iChannelNumber < 10)
		//{/* Èç¹ûchannelÖ»ÓÐÒ»Î»,ÔòÔÚÇ°²¹0µ½Á½Î» */
		//	szBuf[1] = szBuf[0];
		//	szBuf[0] = '0';
		//}

		//int istrLength	=	(int)strlen(szBuf);
		//int	iNumber;
		//std::string	strNumber;

		//for( int i = 0 ; i < istrLength ; i++ )
		//{
		//	strNumber	=	szBuf[ i ];
		//	iNumber		=	atoi( strNumber.c_str() );

		//	stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
		//	stAddListIcon.m_rcSrc.left		=	53 + iNumber * 7;
		//	stAddListIcon.m_rcSrc.top		=	14;
		//	stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 6;
		//	stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		//	stAddListItem.m_vIconImage.push_back( stAddListIcon );
		//}

		//stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
		//stAddListIcon.m_rcSrc.left		=	39;
		//stAddListIcon.m_rcSrc.top		=	14;
		//stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 10;
		//stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		//stAddListItem.m_vIconImage.push_back( stAddListIcon );

		//stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
		//stAddListIcon.m_rcSrc.left		=	50;
		//stAddListIcon.m_rcSrc.top		=	14;
		//stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 2;
		//stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		//stAddListItem.m_vIconImage.push_back( stAddListIcon );
	}
	else if( stChatData.m_eScope == SCOPE_CHATROOM_ADVERT )
	{
		stAddListItem.m_stBGImage.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
		stAddListItem.m_stBGImage.m_rcSrc.left		=	342;
		stAddListItem.m_stBGImage.m_rcSrc.top		=	271;
		stAddListItem.m_stBGImage.m_rcSrc.right		=	344;
		stAddListItem.m_stBGImage.m_rcSrc.bottom	=	285;
		stAddListItem.m_stBGImage.m_iSX				=	468;
		stAddListItem.m_stBGImage.m_iSY				=	14;

		stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
		stAddListIcon.m_rcSrc.left		=	295;
		stAddListIcon.m_rcSrc.top		=	112;
		stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 6;
		stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		stAddListItem.m_vIconImage.push_back( stAddListIcon );

		stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";

		// ´ëÈ­¹æ Å¸ÀÔ(0:ÀÏ¹Ý, 1:ÀåÅÍ, 2:±¸ÀÎ, 3:¹ÌÆÃ)
		switch( stChatData.m_iCategory )
		{
		case 0:
			stAddListIcon.m_rcSrc.left		=	308;
			stAddListIcon.m_rcSrc.top		=	112;
			break;
		case 1:
			stAddListIcon.m_rcSrc.left		=	277;
			stAddListIcon.m_rcSrc.top		=	127;
			break;
		case 2:
			stAddListIcon.m_rcSrc.left		=	292;
			stAddListIcon.m_rcSrc.top		=	127;
		    break;
		case 3:
			stAddListIcon.m_rcSrc.left		=	307;
			stAddListIcon.m_rcSrc.top		=	127;
		    break;
		}

		stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 14;
		stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		stAddListItem.m_vIconImage.push_back( stAddListIcon );

		stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
		stAddListIcon.m_rcSrc.left		=	302;
		stAddListIcon.m_rcSrc.top		=	112;
		stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 2;
		stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		stAddListItem.m_vIconImage.push_back( stAddListIcon );


		char	szBuf[_MAX_PATH]	=	{};
		itoa( stChatData.m_iChannelNumber , szBuf , 10 );

		int istrLength	=	(int)strlen(szBuf);
		int	iNumber;
		std::string	strNumber;

		for( int i = 0 ; i < istrLength ; i++ )
		{
			strNumber	=	szBuf[ i ];
			iNumber		=	atoi( strNumber.c_str() );

			stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
			stAddListIcon.m_rcSrc.left		=	53 + iNumber * 7;
			stAddListIcon.m_rcSrc.top		=	14;
			stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 6;
			stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
			stAddListItem.m_vIconImage.push_back( stAddListIcon );
		}

		stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
		stAddListIcon.m_rcSrc.left		=	305;
		stAddListIcon.m_rcSrc.top		=	112;
		stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 2;
		stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		stAddListItem.m_vIconImage.push_back( stAddListIcon );

//		stAddListItem.m_stReturnValue.m_iReturnValue	=	iChannelNumber;
	}

	stAddListItem.m_stTextColor	=	stChatData.m_cColor;

	if( stChatData.m_strMsg.empty() == false )
		stAddListItem.m_strText		=	stChatData.m_strMsg;

	if( stChatData.m_bWhisper == true && stChatData.m_strID.empty() == false )
		stAddListItem.m_stReturnValue.m_strReturnValue = stChatData.m_strID;

	if( m_pWindowTalkingRoom != NULL )
	{
		if( m_pWindowTalkingRoom->IsShow() == true && stChatData.m_eScope != SCOPE_CHATROOM_ADVERT )
		{
			m_pWindowTalkingRoom->SPSendMessage( SPIM_TALKING_ROOM_MESSAGE , (WPARAM)&stChatData , NULL );

			if( stChatData.m_eScope == SCOPE_AROUNDREGION )
				return;
		}
	}

	m_pHistory->InsertItem( &stAddListItem );

	CheckLatestMsg();
}
void SPWindowChat::AddChatString( BROADCAST_SCOPE eScope , const char* pstrChat , 
								 D3DXCOLOR cColor , const char* pstrID , 
								 bool bWhisper , int iChannelNumber,
								 UINT32 uiChatItemID )
{
	LIST_ITEM	stAddListItem;
	LIST_ICON	stAddListIcon;

	stAddListItem.m_stTextColor	=	cColor;
	if( eScope == SCOPE_CHANNEL )
	{
		stAddListItem.m_stBGImage.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
		stAddListItem.m_stBGImage.m_rcSrc.left		=	413;
		stAddListItem.m_stBGImage.m_rcSrc.top		=	416;
		stAddListItem.m_stBGImage.m_rcSrc.right		=	415;
		stAddListItem.m_stBGImage.m_rcSrc.bottom	=	430;
		stAddListItem.m_stBGImage.m_iSX				=	468;
		stAddListItem.m_stBGImage.m_iSY				=	14;

		stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
		stAddListIcon.m_rcSrc.left		=	18;
		stAddListIcon.m_rcSrc.top		=	14;
		stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 20;
		stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		stAddListItem.m_vIconImage.push_back( stAddListIcon );

		stAddListIcon.m_strImageName	=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
		stAddListIcon.m_rcSrc.left		=	50;
		stAddListIcon.m_rcSrc.top		=	14;
		stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 2;
		stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		stAddListItem.m_vIconImage.push_back( stAddListIcon );
	}
	else if( eScope == SCOPE_WORLD )
	{
		_addBugleString(uiChatItemID, iChannelNumber, stAddListItem, stAddListIcon);
	}

	if( pstrChat != NULL && strlen( pstrChat ) > 0 )
		stAddListItem.m_strText		=	pstrChat;

	if( bWhisper == true && pstrID != NULL && strlen( pstrID ) > 0 )
		stAddListItem.m_stReturnValue.m_strReturnValue = pstrID;

	m_pHistory->InsertItem( &stAddListItem );

	CheckLatestMsg();
}

void SPWindowChat::_addBugleString(UINT32 uiChatItemID, int iChannelNumber, LIST_ITEM& stAddListItem, LIST_ICON& stAddListIcon)
{
	static char *strChannelBg[BUGLE_CHANNELBG_ENUM_NUM] =
	{
		"DATA/INTERFACE/CONCEPT/BUGLE.PNG",
			"DATA/INTERFACE/CONCEPT/BUGLE.PNG",
			"DATA/INTERFACE/CONCEPT/BUGLE.PNG",
			"DATA/INTERFACE/CONCEPT/BUGLE.PNG",
			"DATA/INTERFACE/CONCEPT/BUGLE.PNG"
	};

	static POINT ptChannelNum[BUGLE_CHANNELBG_ENUM_NUM] =
	{
		{0, 308},
		{0, 323},
		{0, 355},
		{0, 338},
		{0, 370},
	};

	static POINT ptChannelBg[BUGLE_CHANNELBG_ENUM_NUM] =
	{
		{101, 308},
		{101, 323},
		{101, 355},
		{101, 338},
		{101, 370}
	};

	BugleInfo info = SPWindowBugleSelect::GetBugleInfo( (int)uiChatItemID );

	stAddListItem.m_stTextColor = D3DCOLOR_ARGB( 255, info.m_iColorR, info.m_iColorG, info.m_iColorB );

	stAddListItem.m_stBGImage.m_strImageName	=	info.m_strBgFilename;
	stAddListItem.m_stBGImage.m_rcSrc.left		=	info.m_iBgX;
	stAddListItem.m_stBGImage.m_rcSrc.top		=	info.m_iBgY;
	stAddListItem.m_stBGImage.m_rcSrc.right		=	info.m_iBgX + info.m_iBgSizeX;
	stAddListItem.m_stBGImage.m_rcSrc.bottom	=	info.m_iBgY + info.m_iBgSizeY;
	stAddListItem.m_stBGImage.m_iSX				=	468;
	stAddListItem.m_stBGImage.m_iSY				=	14;

	//--------------------------------------------------
	// channel number
	BugleChannelBgType channelBgType = info.m_channelBgType;
	if (BUGLE_CHANNELBG_NULL == channelBgType)
	{
		channelBgType = BUGLE_CHANNELBG_WHITE;
	}

	// channel backgroud
	stAddListIcon.m_strImageName	=	strChannelBg[channelBgType];
	stAddListIcon.m_rcSrc.left		=	ptChannelBg[channelBgType].x;
	stAddListIcon.m_rcSrc.top		=	ptChannelBg[channelBgType].y;
	stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left + 41;
	stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top + 12;
	stAddListItem.m_imgChannel.m_iSX	=	34;
	stAddListItem.m_imgChannel.m_iSY	=	12;
	stAddListItem.m_imgChannel = stAddListIcon;

	stAddListIcon.m_strImageName	=	strChannelBg[channelBgType];
	stAddListIcon.m_rcSrc.left		=	ptChannelNum[channelBgType].x;	//18
	stAddListIcon.m_rcSrc.top		=	ptChannelNum[channelBgType].y;	//14
	stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 15;
	stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
	stAddListItem.m_vIconImage.push_back( stAddListIcon );

	char	szBuf[_MAX_PATH]	=	{};
	itoa( iChannelNumber , szBuf , 10 );

	if (iChannelNumber < 10)
	{/* Èç¹ûchannelÖ»ÓÐÒ»Î»,ÔòÔÚÇ°²¹0µ½Á½Î» */
		szBuf[1] = szBuf[0];
		szBuf[0] = '0';
	}
	int istrLength	=	(int)strlen(szBuf);
	int	iNumber;
	std::string	strNumber;

	for( int i = 0 ; i < istrLength ; i++ )
	{
		strNumber	=	szBuf[ i ];
		iNumber		=	atoi( strNumber.c_str() );

		stAddListIcon.m_strImageName	=	strChannelBg[channelBgType];
		stAddListIcon.m_rcSrc.left		=	ptChannelNum[channelBgType].x + 29 + iNumber * 7;
		stAddListIcon.m_rcSrc.top		=	ptChannelNum[channelBgType].y;
		stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 6;
		stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
		stAddListItem.m_vIconImage.push_back( stAddListIcon );
	}

	stAddListIcon.m_strImageName	=	strChannelBg[channelBgType];
	stAddListIcon.m_rcSrc.left		=	ptChannelNum[channelBgType].x + 15;
	stAddListIcon.m_rcSrc.top		=	ptChannelNum[channelBgType].y;
	stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 10;
	stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
	stAddListItem.m_vIconImage.push_back( stAddListIcon );

	stAddListIcon.m_strImageName	=	strChannelBg[channelBgType];
	stAddListIcon.m_rcSrc.left		=	ptChannelNum[channelBgType].x + 26;
	stAddListIcon.m_rcSrc.top		=	ptChannelNum[channelBgType].y;
	stAddListIcon.m_rcSrc.right		=	stAddListIcon.m_rcSrc.left	+ 2;
	stAddListIcon.m_rcSrc.bottom	=	stAddListIcon.m_rcSrc.top	+ 14;
	stAddListItem.m_vIconImage.push_back( stAddListIcon );

	// tail
	if (!info.m_strBgTail.empty())
	{
		stAddListIcon.m_strImageName	=	info.m_strBgTail;
		stAddListIcon.m_rcSrc.left		=	info.m_iBgTailX;
		stAddListIcon.m_rcSrc.top		=	info.m_iBgTailY;
		stAddListIcon.m_rcSrc.right		=	info.m_iBgTailX + info.m_iBgTailSizeX;
		stAddListIcon.m_rcSrc.bottom	=	info.m_iBgTailY + info.m_iBgTailSizeY;
		stAddListItem.m_imgTail.m_iSX	=	info.m_iBgTailSizeX;
		stAddListItem.m_imgTail.m_iSY	=	info.m_iBgTailSizeY;
		stAddListItem.m_imgTail = stAddListIcon;
	}

	// header
	if (!info.m_strBgHeader.empty())
	{
		stAddListIcon.m_strImageName	=	info.m_strBgHeader;
		stAddListIcon.m_rcSrc.left		=	info.m_iBgHeaderX;
		stAddListIcon.m_rcSrc.top		=	info.m_iBgHeaderY;
		stAddListIcon.m_rcSrc.right		=	info.m_iBgHeaderX + info.m_iBgHeaderSizeX;
		stAddListIcon.m_rcSrc.bottom	=	info.m_iBgHeaderY + info.m_iBgHeaderSizeY;
		stAddListItem.m_imgHeader.m_iSX	=	info.m_iBgHeaderSizeX;
		stAddListItem.m_imgHeader.m_iSY	=	info.m_iBgHeaderSizeY;
		stAddListItem.m_imgHeader = stAddListIcon;
	}

	// font effect
	stAddListItem.m_fontEffect = SPFONT_NULL;
}
//--------------------------------------------------
int SPWindowChat::OnBugleChooser( WPARAM wParam, LPARAM lParam )
{
	if (g_bNeedBugleHighlight)
	{
		g_bNeedBugleHighlight = false;
		char szBuf[64] = {0};
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szBuf);
		addNotifier( szBuf );
		SAFE_CALL_FUNC( m_ptrBugleSelectHL, Hide() );
		SAFE_CALL_FUNC( m_ptrBugleChannelHL, Hide() ); 
	}

    SPWindow *window = g_pInterfaceManager->FindWindow( WIID_BUGLE_MENU );
	if (window)
	{
		if (window->IsShow())
			window->Hide();
		else
			window->Show();
	}
	return 0;
}
bool SPWindowChat::SetBugleInfo(const BugleInfo &info)
{
	if (info.m_iItemShopID < 0)
	{
		return false;
	}

	m_bugleInfo = info;

	if (m_ptrImageBugleIcon && m_ptrImageBugleName)
	{
		m_ptrImageBugleIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_ptrImageBugleIcon->SetImage( m_bugleInfo.m_strIconFilename.c_str(),
			m_bugleInfo.m_iIconX, m_bugleInfo.m_iIconY );
		m_ptrImageBugleIcon->SetSrcSize( info.m_iIconSizeX, info.m_iIconSizeY );
		m_ptrImageBugleIcon->Show();

		m_ptrImageBugleName->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_ptrImageBugleName->SetWindowText( m_bugleInfo.m_strName.c_str() );

		if (m_pMsgEdit)
		{
			m_pMsgEdit->SPSendMessage( SPIM_SETFOCUS );
			g_pInterfaceManager->SetFocusWindow( m_pMsgEdit );
		}
	}

	return true;
}
bool SPWindowChat::_sendBugleMsg(const char *pstrChatMsg)
{
	if (m_bugleInfo.m_iItemShopID < 0)
	{
		DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );
		//PrintErrorMsg( szErrorMsg , 4000005 , NULL , NULL );
		_snprintf( szErrorMsg, MAX_CHATMSG_LEN-1, "%s", "ÇëÑ¡ÔñÐèÒªµÄÀ®°ÈÀàÐÍ." );
		OnChatMsg( (WPARAM)szErrorMsg , NULL );

		m_ptrImageBugleIcon->Hide();
		m_ptrImageBugleName->SetWindowText( "ÇëÑ¡Ôñ" );
		return false;
	}

	/* Èç¹ûÓÐÇé¾°À®°ÈÕýÔÚ·¢ËÍ,Ôò¾Ü¾øÐÂµÄÇé¾°À®°È */
	if (BUGLE_CHANNELBG_VISIBLE == m_bugleInfo.m_channelBgType)
	{
		SPWindowVisualBugle	*pWnd = (SPWindowVisualBugle *)g_pInterfaceManager->FindWindow( WIID_VISUAL_BUGLE );
		if (!pWnd || !pWnd->IsQueueReadyForNew())
		{
			DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );
			//PrintErrorMsg( szErrorMsg , 4000005 , NULL , NULL );
			_snprintf( szErrorMsg, MAX_CHATMSG_LEN-1, "%s", "Çé¾°µç»°ÆµµÀÄ¿Ç°Õ¼Ïß£¬ÇëÉÔºóÊ¹ÓÃ¡£" );
			OnChatMsg( (WPARAM)szErrorMsg , NULL );
			return false;
		}
	}

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
	if( pLocalPlayer == NULL )
		return false;

	SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();
	if( pInvenArchive == NULL )
		return false;

	SPItem*	pRequireItem	=	pInvenArchive->GetSPItem( INVENTORY_CONSUME , m_bugleInfo.m_iItemShopID );

	//--------------------------------------------------
	// Èç¹ûÊÇ´ÓÏûºÄÀ®°È×ª»»µ½ÏûºÄµã¾í,Ôò¸øÓÚÒ»´ÎÌáÊ¾.È«³Ì½öÒ»´Î.
	if (g_bWarningBugleCoinBegin[m_bugleInfo.m_iItemShopID] == FALSE)
	{
		if (!pRequireItem)
		{/* ÒÑ¾­Ã»ÓÐÁË,ÏÖÔÚ»¨µÄÊÇµã¾íÁË */
			DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );
			PrintErrorMsg( szErrorMsg , 60000003 , NULL , NULL );
			OnChatMsg( (WPARAM)szErrorMsg , NULL );
			g_bWarningBugleCoinBegin[m_bugleInfo.m_iItemShopID] = TRUE;
		}
	}
	
	if (pRequireItem 
		&& 
		(1 == pRequireItem->GetItemStatus()->GetStackCount()))
	{/* ×îºóÒ»¸ö,·¢ÁË¾ÍÃ»ÁË */
		DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );
		PrintErrorMsg( szErrorMsg , 60000002 , NULL , NULL );
		OnChatMsg( (WPARAM)szErrorMsg , NULL );
		g_bWarningBugleLastOne = true;
	}
	//--------------------------------------------------

	if( pRequireItem == NULL )
	{
		if (!pstrChatMsg)
			return false;

		CPacket Packet;
		UINT8 len = static_cast<UINT8>(strlen( pstrChatMsg ));

		CONTAINER_SLOT Slot;
		Slot.ContainerType = CONTAINER_TYPE_INVALID;
		Slot.SlotIndex = 0;

		Packet.Add((UINT32)MONEY_CS_CHAT);
		Packet.AddData(&Slot, sizeof(CONTAINER_SLOT));
		Packet.AddUINT64( 0 );
		Packet.AddUINT32(SCOPE_WORLD);
		Packet.AddUINT32( m_bugleInfo.m_iItemShopID );
		Packet.AddUINT8(len);
		Packet.AddData(pstrChatMsg, len);
		g_pNetworkManager->SPPerformMessage(MONEY_CS_CHAT, 0, (LPARAM)&Packet);	
	}
	else
	{
		if( pInvenArchive->SetMicItem( pRequireItem->GetContainerType() , pRequireItem->GetSlotIndex() ) == false )
			return false;

		if( pInvenArchive->SendBugle( m_strMessage ) == true )
			return false;

		pInvenArchive->ClearMicItem();
	}

	return true;
}
//--------------------------------------------------
bool SPWindowChat::addVisualBugle(VisualBuglePlayerInfo &playerInfo, const char *strMsg, UINT32 uiItemID, bool bPreview)
{
	BugleInfo info = SPWindowBugleSelect::GetBugleInfo( uiItemID );

	SPWindowVisualBugle	*pWnd = (SPWindowVisualBugle *)g_pInterfaceManager->FindWindow( WIID_VISUAL_BUGLE );
	if (pWnd)
	{
		pWnd->ShowVisualBugle( playerInfo, strMsg, info, bPreview );
		return true;
	}
	else
		return false;
}
//--------------------------------------------------
bool SPWindowChat::SetVisualBugleInfo(std::vector< CONTAINER_ITEM >& Items, const CHARACTER_INFO2 &info)
{
	if( m_pPlayer == NULL )
		return false;
	
	std::vector< CONTAINER_ITEM >::iterator iter = Items.begin();
	while (iter != Items.end())
	{
		if (g_pItemCluster->GetItemDuration( iter->Item.iItemID ) != ITEM_DURATION_NULL
			&&
			iter->Item.iLifeTime <= 0)
		{
			++iter;
			continue;
		}

		if (iter->SlotInfo.ContainerType == FIGURE_BASIC)
			m_pPlayer->SPGOBPerformMessage( SPGM_EQUIP_DEFAULT_CHANGE, (LPARAM)&(*iter));
		else
			m_pPlayer->SPGOBPerformMessage( SPGM_EQUIP_CHANGE, (LPARAM)&(*iter));

		++iter;
	}
	
	SPGOBModelUnit	*pGobModel = m_pPlayer->GetModelUnit();
	if (!pGobModel)
		return false;

	pGobModel->SPSendMessage( MV_SET_ANIMATION_INDEX, info.uiClassType * 10 );
	pGobModel->SPSendMessage( MV_SET_ANI_LOOP, false );
	return true;
}
//--------------------------------------------------
int SPWindowChat::OnBugleChooserFocus(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	m_ptrImageBugleBg->SPSendMessage( SPIM_SET_IMAGECLEAR );
	m_ptrImageBugleBg->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE.PNG", 3, 34 );
	m_ptrImageBugleBg->SetSrcSize( 4, 4 );
	return 1;
}
int SPWindowChat::OnBugleChooserLostFocus(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	m_ptrImageBugleBg->SPSendMessage( SPIM_SET_IMAGECLEAR );
	m_ptrImageBugleBg->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG", 167, 33 );
	m_ptrImageBugleBg->SetSrcSize( 2, 8 );
	return 1;
}
//--------------------------------------------------
int SPWindowChat::GmOutPutNeed( BROADCAST_SCOPE scope, char msg[], char SenderName[] )
{
	if ( scope != 2008 && scope != 2009 && scope != SCOPE_WORLD && scope != SCOPE_CHANNEL )
	{
		return 1;
	}
	HINSTANCE hinst;
	hinst = ::LoadLibrary("GmOutPut.dll");

	if( NULL == hinst )
	{
		return 1;
	}
    fnOutPut output = (fnOutPut)::GetProcAddress( hinst, "fnOutPut" );	
	if( NULL == output )
	{	
		::FreeLibrary( hinst );
		return 1;
	}
	output( scope, msg, SenderName );
	::FreeLibrary( hinst );
	return 1;
}
