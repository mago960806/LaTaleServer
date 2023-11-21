// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD LIST WINDOW
// Comment     : 
// Creation    : DURAGON 2007-4-04    11:18 
//***************************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowSlider.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowEditMultiLine.h"
#include "SPWindowChat.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPPlayerInvenArchive.h"
#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"

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
#include "SPGOBManager.h"
#include "SPGOBStatus.h"
#include "SPGOBModel.h"
#include "SPGOBModelUnit.h"
#include "SPMonsterModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPGOBCluster.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBModelUnit.h"
#include "SPPlayer.h"
#include "SPMOBCluster.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"
#include "SPNoticeBox.h"

#include "SPUtil.h"
#include "SPDebug.h"

#include "SPGuildLDT.h"
#include "SPGuildArchive.h"
#include "SPCropArchive.h"
#include "SPWindowGuildUser.h"
#include "SPWindowGuildSList.h"
#include "SPWindowGuild.h"
#include "SPWindowToolTip.h"
#include "_Error_log.h"
#include "GlobalDefines_Share.h"
#include "SPLocalizeManager.h"

/**
Vector Sort에 사용될 Generic algorithm 비교함수
*/
bool GuildSortByGrade( PGUILD_MEMBER lv, PGUILD_MEMBER rv ) {
	return lv->byGrade < rv->byGrade;
}


SPWindowGuildInfo::SPWindowGuildInfo(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_bListInfo(false)
, m_bShowOffline(true)
, m_iCurPage(0)
{
	Init();
}

SPWindowGuildInfo::SPWindowGuildInfo(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_GUILD_INFO, InstanceID, iX, iY, iCX, iCY, pParent)	
, m_bListInfo(false)
, m_bShowOffline(true)
, m_iCurPage(0)
, m_pBGTexture(NULL)
{
	Init();
}

SPWindowGuildInfo::~SPWindowGuildInfo()
{
	Clean();
}

void SPWindowGuildInfo::Init()
{
	m_strToolTip		=	"[SPWindowGuildInfo]";
	m_iNoticeState		= GUILD_NOTICE_EMPTY ;

	m_pGuildArchive		= NULL ;
	m_pGuildListWnd		= NULL ;
	m_pGuildStatusWnd	= NULL ;
	m_pGuildCorpsWnd	= NULL ;
	m_pPointNum			= NULL ;
	m_pLevelTooltip		= NULL ;
	m_iCurMenu			= GUILD_MENU_LIST ;
	m_iGuildProperty	= GUILD_PROPERTY_NORMAL ;
	m_iGNoticeState		= 0 ;
	m_iPreSelected		= 0 ;
	m_iCurSelected		= 0 ;
	m_iCurSelecteIndex	= 0 ;
	m_bShowSlide		= false ;
	m_bShowMSTSlide		= false ;
	m_bShowAbilToolTip	= false ;
	m_strChangeMasterName.clear() ;
	m_strExpelName.clear() ;
	m_stLevelAbility.Clear() ;

	for( int i = 0; i < MAX_GUILD_LIST_VIEW; i++)
	{
		m_pGuildMember[i] = NULL ;
	}
	m_pGuildGradePopup = NULL ;
	for( int i = 0; i < MAX_GUILD_GRADE_VIEW; i++)
	{
		m_pGuildPopupList[i] = NULL ;
	}


	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[0]	, 482 , 316 , 486 , 320 );		// BG 7
	SetRect( &m_rcBGSrc[1]	, 488 , 316 , 490 , 320 );		// BG 8
	SetRect( &m_rcBGSrc[2]	, 492 , 316 , 496 , 320 );		// BG 9
	SetRect( &m_rcBGSrc[3]	, 482 , 322 , 486 , 324 );		// BG 4
	SetRect( &m_rcBGSrc[4]	, 488 , 322 , 490 , 324 );		// BG 5
	SetRect( &m_rcBGSrc[5]	, 492 , 322 , 496 , 324 );		// BG 6
	SetRect( &m_rcBGSrc[6]	, 482 , 311 , 486 , 315 );		// BG 1
	SetRect( &m_rcBGSrc[7]	, 488 , 311 , 490 , 315 );		// BG 2
	SetRect( &m_rcBGSrc[8]	, 492 , 311 , 496 , 315 );		// BG 3

	SetRect( &m_rcBGSrc[9]	, 486 , 387 , 488 , 388 );		// 서브 백그라운드 경계라인
	SetRect( &m_rcBGSrc[10]	, 488 , 307 , 490 , 309 );		// 흰색 서브 백그라운드
	SetRect( &m_rcBGSrc[11]	, 445 ,  92 , 449 , 109 );		// Invite TEXT BG
	SetRect( &m_rcBGSrc[12]	, 449 ,  92 , 453 , 109 );		// Invite TEXT BG
	SetRect( &m_rcBGSrc[13]	, 453 ,  92 , 457 , 109 );		// Invite TEXT BG
	SetRect( &m_rcBGSrc[14]	, 291 , 375 , 293 , 376 );		// 탭경계라인

	//SetRect( &m_rcBGSrc[0]	, 486 , 387 , 488 , 388 );		// 서브 백그라운드 경계라인
	//SetRect( &m_rcBGSrc[1]	, 488 , 307 , 490 , 309 );		// 흰색 서브 백그라운드
	//SetRect( &m_rcBGSrc[2]	, 445 ,  92 , 449 , 109 );		// Invite TEXT BG
	//SetRect( &m_rcBGSrc[3]	, 449 ,  92 , 453 , 109 );		// Invite TEXT BG
	//SetRect( &m_rcBGSrc[4]	, 453 ,  92 , 457 , 109 );		// Invite TEXT BG
	//SetRect( &m_rcBGSrc[5]	, 291 , 375 , 293 , 376 );		// 탭경계라인


	SPWindowStatic*	pWindowStatic;
	SPWindowButton*	pWindowButton;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_BASE_TITLE , 8 , 3 , 137 , 23 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 1 , 344 );
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_BASE_TITLE_TEXT1 , 110 , 8 , 6 , 13 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 244 , 92 );
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_BASE_TITLE_ICON2 , 120 , 7 , 16 , 16 , this );
	pWindowStatic->SetSrcSize(24, 24) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL;
	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_BASE_TITLE_ICON1 , 120 , 7 , 16 , 16 , this );
	pWindowStatic->SetSrcSize(24, 24) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_BASE_TITLE_TEXT2 , 141 , 9 , 152 , 12 , this );
	pWindowStatic->SetFormat(DT_VCENTER | DT_LEFT);
	pWindowStatic->SetFont(FONT_12_BOLD) ;
	pWindowStatic->SetFontColor(RGBA(255, 255, 255, 255)) ;
	pWindowStatic->SetFontShadowColor(RGBA(70, 140, 124, 255));
	pWindowStatic->SetFontShadow(true) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_BASE_CLOSE , 362 , 9 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 217 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 230 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 243 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 256 );
	pWindowButton->Hide() ;
	pWindowButton	=	NULL;

	//////////////////////////////////////////////////////////////////////////
	// Guild Invite
	m_pEditInviteName		= new SPWindowEdit(WIID_GUILD_BASE_INVITE_NAME, 54, 39, 185, 12, this) ;
	m_pEditInviteName->SetFontColor(RGBA(70, 140, 124, 255));
	//m_pEditInviteName->SetEnableTabStop(true);
	m_pEditInviteName->SetLimitText(MAX_LEN_ID);
	m_pEditInviteName->Hide() ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_BASE_INVITE_BTN , 264 , 35 , 68 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 70 , 368 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 70 , 386 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 70 , 404 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 70 , 422 );
	pWindowButton->Hide() ;
	//SetEnterFocusWindow(pWindowButton) ;

	//////////////////////////////////////////////////////////////////////////
	// Menu Button
	pWindowButton	=	new	SPWindowButton( WIID_GUILD_MENU_LIST , 10 , 60 , 80 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 208 , 375 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 208 , 391 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 208 , 407 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 208 , 423 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 208 , 407 );
	pWindowButton->Hide() ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_MENU_STATUS , 102 , 60 , 80 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 288 , 375 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 288 , 391 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 288 , 407 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 288 , 423 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 288 , 407 );
	pWindowButton->Hide() ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_MENU_CROPS , 194 , 60 , 80 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 368 , 375 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 368 , 391 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 368 , 407 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 368 , 423 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 368 , 407 );
	pWindowButton->Hide() ;

	InitGuildList() ;
	InitGuildStatus() ;
	InitGuildCorps() ;
}


/**
Guild List창 내부에 들어가는 컨트롤들 초기화
*/
void SPWindowGuildInfo::InitGuildList()
{
	SPWindowStatic*	pWindowStatic;
	SPWindowButton*	pWindowButton;
	SPWindowEditMultiLine*	pWindowMEdit; 

	m_pGuildListWnd = new SPWindowStatic( WIID_GUILD_LIST_PANNEL , 0 , 0 , m_iSX , m_iSY , this );
	m_pGuildListWnd->SetToolTipStr("[GuildListPannel]") ;

	SetRect( &m_rcListBGSrc[0]	, 441 , 448 , 479 , 463 );		// 유형
	SetRect( &m_rcListBGSrc[1]	, 375 , 166 , 491 , 181 );		// 아이디
	SetRect( &m_rcListBGSrc[2]	, 441 , 464 , 479 , 479 );		// 레벨
	SetRect( &m_rcListBGSrc[3]	, 374 ,  92 , 431 , 107 );		// 기여도
	SetRect( &m_rcListBGSrc[4]	, 441 , 480 , 479 , 495 );		// 채널
	SetRect( &m_rcListBGSrc[5]	, 441 , 496 , 479 , 511 );		// 작위

	SetRect( &m_rcListBGSrc[6]	, 488 , 323 , 490 , 324 );		// 리스트 하단 라인
	SetRect( &m_rcListBGSrc[7]	,   1 , 330 , 138 , 343 );		// OffLine 표시 Text
	SetRect( &m_rcListBGSrc[8]	,   1 , 314 , 138 , 329 );		// 슬로건 Title
	SetRect( &m_rcListBGSrc[9]	, 502 , 152 , 504 , 209 );		// 슬로건 BG 좌
	SetRect( &m_rcListBGSrc[10]	, 505 , 152 , 507 , 209 );		// 슬로건 BG 중
	SetRect( &m_rcListBGSrc[11]	, 509 , 152 , 511 , 209 );		// 슬로건 BG 우

	//////////////////////////////////////////////////////////////////////////
	// User List - 7개 생성
	for( int i = 0; i < MAX_GUILD_LIST_VIEW; i++)
	{
		m_pGuildMember[i] = new SPWindowGuildUser( WIID_GUILD_USER_LIST + ( i * MAX_GUILD_USERLIST_STEP ) , 10 , 104 + i*17  , 284 , 16 , m_pGuildListWnd );
	}

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_LIST_SHOWOFFLINE , 146 , 234 , 12 , 12 , m_pGuildListWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 204 , 211 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 217 , 211 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 230 , 211 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 239 , 211 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 230 , 211 );
	pWindowButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	pWindowButton->Hide() ;
	pWindowButton = NULL ;

	pWindowMEdit		=	new SPWindowEditMultiLine(WIID_GUILD_LIST_SLOGUN_EDIT, 44, 287, 286, 40, m_pGuildListWnd) ;
	pWindowMEdit->SetFontColor(RGBA(49, 76, 97, 255));
	//pWindowMEdit->SetFormat( DT_RIGHT | DT_VCENTER | DT_WORDBREAK );		// DT_RIGHT | DT_VCENTER | DT_WORDBREAK | DT_NOCLIP
	//pWindowMEdit->SetEnableTabStop(true);
	pWindowMEdit->SetLimitText(GUILD_MAX_SLOGAN);
	pWindowMEdit->Hide() ;
	pWindowMEdit = NULL ;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_LIST_SLOGUN_STC , 44 , 287 , 286 , 40 , m_pGuildListWnd );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_LEFT | DT_WORDBREAK);
	pWindowStatic->SetTextPeriod(false) ;
	pWindowStatic->SetMultiLine( true, 3 );
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_LIST_SLOGUNBTN , 279 , 340 , 68 , 17 , m_pGuildListWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 301 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 301 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 301 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 301 , 494 );
	pWindowButton->Hide() ;
	pWindowButton = NULL ;

	//////////////////////////////////////////////////////////////////////////
	// Slider
	pWindowButton	=	new	SPWindowButton( WIID_GUILD_LIST_SLIDERUP , 361 , 104 , 12 , 11 , m_pGuildListWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_LIST_SLIDERDOWN , 361 , 211 , 12 , 11 , m_pGuildListWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	RECT rtRect;
	m_pUserListSlider	=	new	SPWindowSlider( WIID_GUILD_LIST_SLIDER , 361 , 115 , 12 , 96 , m_pGuildListWnd );
	SetRect( &rtRect , 460 , 470 , 472 , 494 );			//	일반
	m_pUserListSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 473 , 470 , 485 , 494 );			//	마우스 오버
	m_pUserListSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 486 , 470 , 498 , 494 );			//	마우스 클릭
	m_pUserListSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 499 , 470 , 511 , 494 );
	m_pUserListSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 460 , 496 , 472 , 498 );			//	일반
	m_pUserListSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 473 , 496 , 485 , 498 );			//	마우스 오버
	m_pUserListSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 486 , 496 , 498 , 498 );			//	마우스 클릭
	m_pUserListSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 499 , 496 , 511 , 498 );
	m_pUserListSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	m_pUserListSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pUserListSlider->SetCurpage( 0 );
	m_pUserListSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );
	m_pUserListSlider->Hide() ;

	//////////////////////////////////////////////////////////////////////////
	// Bottom Button
	pWindowButton	=	new	SPWindowButton( WIID_GUILD_LIST_INVITE_PARTY , 65 , 373 , 76 , 17 , m_pGuildListWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 1 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 1 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 1 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 1 , 494 );
	pWindowButton = NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_LIST_WHISPER , 149 , 373 , 76 , 17 , m_pGuildListWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 78 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 78 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 78 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 78 , 494 );
	pWindowButton = NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_LIST_LEAVE , 233 , 373 , 76 , 17 , m_pGuildListWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 494 );
	pWindowButton = NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_LIST_EXPEL , 271 , 233 , 76 , 17 , m_pGuildListWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 208 , 302 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 208 , 320 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 285 , 302 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 285 , 320 );
	pWindowButton = NULL ;

	//////////////////////////////////////////////////////////////////////////
	// Popup Window
	//if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_SANDA )
	//{
	//	m_pGuildGradePopup = new SPWindowStatic( WIID_GUILD_LIST_POPUP_BG	, 0 , 0 , 85 , 93 , m_pGuildListWnd );
	//	m_pGuildGradePopup->SetWindowType(WND_TYPE_TOPBASE);
	//	m_pGuildGradePopup->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI105.PNG", 131 ,  1, 216, 3);
	//	m_pGuildGradePopup->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI105.PNG", 131 ,  7, 216, 9);
	//	m_pGuildGradePopup->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI105.PNG", 131 , 14, 216, 16);
	//	m_pGuildGradePopup->SetShowEnable(false) ;
	//	ShowRankPopup(false) ;

	//	for( int i = 0; i < MAX_GUILD_GRADE_VIEW; i++)
	//	{
	//		m_pGuildPopupList[i] = new SPWindowGuildSimpleList( WIID_GUILD_LIST_POPUP_DLG + (i * MAX_GUILD_SIMPLIST_STEP), 1, 7 + i*16, 83, 15, m_pGuildGradePopup) ;
	//		m_pGuildPopupList[i]->SetToolTipStr("PopUp List") ;
	//	}
	//}
	//else
	//{
	m_pGuildGradePopup = new SPWindowStatic( WIID_GUILD_LIST_POPUP_BG	, 0 , 0 , 141 , 93 , m_pGuildListWnd );
	m_pGuildGradePopup->SetWindowType(WND_TYPE_TOPBASE);
	m_pGuildGradePopup->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI107.PNG", 70 , 403, 211, 405);
	m_pGuildGradePopup->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI107.PNG", 70 , 407, 211, 409);
	m_pGuildGradePopup->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI107.PNG", 70 , 411, 211, 413);
	m_pGuildGradePopup->SetShowEnable(false) ;
	ShowRankPopup(false) ;

	for( int i = 0; i < MAX_GUILD_GRADE_VIEW; i++)
	{
		m_pGuildPopupList[i] = new SPWindowGuildSimpleList( WIID_GUILD_LIST_POPUP_DLG + (i * MAX_GUILD_SIMPLIST_STEP), 1, 7 + i*16, 141, 15, m_pGuildGradePopup) ;
		m_pGuildPopupList[i]->SetToolTipStr("PopUp List") ;
	}
	//}

}


void SPWindowGuildInfo::InitGuildStatus()
{
	SPWindowStatic*	pWindowStatic;
	SPWindowButton*	pWindowButton;

	m_pGuildStatusWnd = new SPWindowStatic( WIID_GUILD_STATUS_PANNEL , 0 , 0 , m_iSX , m_iSY , this );
	m_pGuildStatusWnd->SetToolTipStr("[GuildStatusPannel]") ;

	SetRect( &m_rcStatusBGSrc[0]	,   1 , 266 , 138 , 281 );		// 길드 마스터
	SetRect( &m_rcStatusBGSrc[1]	, 484 , 254 , 486 , 281 );		// 마스터 BG
	SetRect( &m_rcStatusBGSrc[2]	, 488 , 254 , 490 , 281 );		// 마스터 BG
	SetRect( &m_rcStatusBGSrc[3]	, 492 , 254 , 494 , 281 );		// 마스터 BG
	SetRect( &m_rcStatusBGSrc[4]	,   1 , 298 , 138 , 313 );		// 길드 정보
	SetRect( &m_rcStatusBGSrc[5]	, 322 , 110 , 409 , 123 );		// 길드 이름
	SetRect( &m_rcStatusBGSrc[6]	, 322 , 138 , 409 , 151 );		// 길드 레벨
	SetRect( &m_rcStatusBGSrc[7]	, 322 , 124 , 409 , 137 );		// 길드 성향
	SetRect( &m_rcStatusBGSrc[8]	, 392 , 152 , 491 , 165 );		// 길드 포인트
	SetRect( &m_rcStatusBGSrc[9]	,   1 , 282 , 138 , 297 );		// 부길마 목록
	SetRect( &m_rcStatusBGSrc[10]	, 488 , 322 , 490 , 323 );		// 라인
	SetRect( &m_rcStatusBGSrc[11]	, 488 , 322 , 490 , 323 );		// 라인

	SetRect( &m_rcPointGageSrc[0]	, 220 , 1 , 222 , 10 );		// 길드 포인트 게이지
	SetRect( &m_rcPointGageSrc[1]	, 224 , 1 , 230 , 10 );		// 



	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_STUS_MASTER_JOB , 79 , 111 , 16 , 16 , m_pGuildStatusWnd );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_STUS_MASTER_INFO , 95 , 113 , 213 , 12 , m_pGuildStatusWnd );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_VCENTER | DT_CENTER);
	pWindowStatic->SetFont(FONT_12_BOLD) ;
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_STUS_NAME , 142 , 173 , 115 , 12 , m_pGuildStatusWnd );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_VCENTER | DT_LEFT);
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_STUS_LEVEL , 142 , 187 , 182 , 12 , m_pGuildStatusWnd );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_VCENTER | DT_LEFT);
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_STUS_PROPERTY , 142 , 201 , 182 , 12 , m_pGuildStatusWnd );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_VCENTER | DT_LEFT);
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_STUS_POINTBG , 157 , 215 , 138 , 11 , m_pGuildStatusWnd );
	pWindowStatic->SetWindowType(WND_TYPE_LEFTBASE);
	pWindowStatic->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 231, 1, 233, 12) ;
	pWindowStatic->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 235, 1, 237, 12) ;
	pWindowStatic->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 239, 1, 241, 12) ;

	// Number '0'~'9' & '%' :: 3자리 숫자 & '%'
	m_pPointNum	=	new SPWindowStatic( WIID_GUILD_STUS_POINTNUM , 209 , 216 , 7 , 9 , m_pGuildStatusWnd );
	m_pPointNum->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 131 , 1 );	// 0
	m_pPointNum->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 1 );	// 1
	m_pPointNum->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 147 , 1 );	// 2
	m_pPointNum->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 1 );	// 3
	m_pPointNum->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 163 , 1 );	// 4
	m_pPointNum->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 171 , 1 );	// 5
	m_pPointNum->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 179 , 1 );	// 6
	m_pPointNum->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 187 , 1 );	// 7
	m_pPointNum->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 195 , 1 );	// 8
	m_pPointNum->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 203 , 1 );	// 9
	m_pPointNum->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 211 , 1 );	// %

	//////////////////////////////////////////////////////////////////////////
	// MASTER User List - 4개 생성
	for( int i = 0; i < MAX_MASTER_LIST_VIEW; i++)
	{
		m_pGuildSubMaster[i] = new SPWindowGuildSimpleList( WIID_GUILD_STUS_MSTLIST + ( i * MAX_GUILD_SIMPLIST_STEP ) , 50 , 266 + i*17  , 261 , 16 , m_pGuildStatusWnd );
	}


	//////////////////////////////////////////////////////////////////////////
	// Slider
	pWindowButton	=	new	SPWindowButton( WIID_GUILD_STUS_MSTLIST_SLIDERUP , 319 , 263 , 12 , 11 , m_pGuildStatusWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_STUS_MSTLIST_SLIDERDOWN , 319 , 324 , 12 , 11 , m_pGuildStatusWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );
	pWindowButton =	NULL;

	RECT rtRect;
	m_pMasterListSlider	=	new	SPWindowSlider( WIID_GUILD_STUS_MSTLIST_SLIDER , 319 , 274 , 12 , 50 , m_pGuildStatusWnd );
	SetRect( &rtRect , 460 , 470 , 472 , 494 );			//	일반
	m_pMasterListSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 473 , 470 , 485 , 494 );			//	마우스 오버
	m_pMasterListSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 486 , 470 , 498 , 494 );			//	마우스 클릭
	m_pMasterListSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 499 , 470 , 511 , 494 );
	m_pMasterListSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 460 , 496 , 472 , 498 );			//	일반
	m_pMasterListSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 473 , 496 , 485 , 498 );			//	마우스 오버
	m_pMasterListSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 486 , 496 , 498 , 498 );			//	마우스 클릭
	m_pMasterListSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 499 , 496 , 511 , 498 );
	m_pMasterListSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	m_pMasterListSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pMasterListSlider->SetCurpage( 0 );
	m_pMasterListSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );

	//////////////////////////////////////////////////////////////////////////
	// Bottom Button
	pWindowButton	=	new	SPWindowButton( WIID_GUILD_STUS_MSTCHANGE , 256 , 344 , 68 , 17 , m_pGuildStatusWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 370 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 370 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 370 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 370 , 494 );
	pWindowButton = NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_STUS_WHISPER , 114 , 373 , 76 , 17 , m_pGuildStatusWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 78 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 78 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 78 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 78 , 494 );
	pWindowButton = NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_STUS_LEAVE , 198 , 373 , 76 , 17 , m_pGuildStatusWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 494 );
	pWindowButton = NULL ;

	// =======================================================
	// Level Tooltip
	m_stLevelAbility.Clear() ;
	m_pLevelTooltip	=	new SPWindowStatic( WIID_GUILD_STUS_LEVEL_TOOLTIP , 157 , 213 , 207 , 256 , m_pGuildStatusWnd );
	m_pLevelTooltip->SetWindowType(WND_TYPE_NINEBASE) ;
	m_pLevelTooltip->SetImageNine( "DATA/INTERFACE/CONCEPT/UI105.PNG", 482, 301, 4, 1) ;
	m_pLevelTooltip->SetShowEnable(false) ;
}



void SPWindowGuildInfo::InitGuildCorps()
{
	SPWindowStatic*	pWindowStatic;
	SPWindowStatic*	pWindowStaticBG;
	SPWindowButton*	pWindowButton;

	SetRect( &m_rcCorpsBGSrc[0]	, 369 , 198 , 491 , 213 );		// 작물정보 TEXT
	SetRect( &m_rcCorpsBGSrc[1]	, 369 , 227 , 491 , 238 );		// 작물성장시간 Text
	SetRect( &m_rcCorpsBGSrc[2]	, 369 , 239 , 491 , 250 );		// 비료 시간 Text
	SetRect( &m_rcCorpsBGSrc[3]	, 369 , 215 , 491 , 226 );		// 촉진재 시간 Text

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_NO_CROPS , 79 , 184 , 229 , 90 , this );
	pWindowStatic->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 205 );
	pWindowStatic	=	NULL;

	pWindowStaticBG	=	new SPWindowStatic( WIID_GUILD_CROP_REGISTRY_BG , 49 , 106 , 285 , 36 , this );
	pWindowStaticBG->SetWindowType(WND_TYPE_LEFTBASE);
	pWindowStaticBG->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 327, 152, 329, 188) ;
	pWindowStaticBG->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 330, 152, 332, 188) ;
	pWindowStaticBG->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 333, 152, 335, 188) ;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_CROP_REGISTRY_AREA , 28 , 2 , 32 , 32 , pWindowStaticBG );
	pWindowStatic->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 336 , 166 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_CROP_REGISTRY_ARROW , 117 , 119 , 11 , 11 , this );
	pWindowStatic->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 323 , 191 );
	pWindowStatic	=	NULL;
	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_CROP_REGISTRY_TEXT , 131 , 106 , 185 , 36 , this );
	pWindowStatic->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 0 , 0 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_CROPS_LEAVE , 154 , 373 , 76 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 155 , 494 );
	pWindowButton = NULL ;

	m_pGuildCorpsWnd = new SPWindowStatic( WIID_GUILD_CROPS_PANNEL , 0 , 0 , m_iSX , m_iSY , this );
	m_pGuildCorpsWnd->SetToolTipStr("[GuildCorpsPannel]") ;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_CROPS_ICON , 77 , 108 , 32 , 32 , m_pGuildCorpsWnd );
	pWindowStatic->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 205 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_CROPS_NAME , 117 , 119 , 197 , 12 , m_pGuildCorpsWnd );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_VCENTER | DT_CENTER);
	pWindowStatic->SetFont(FONT_12_BOLD) ;
	//pWindowStatic->SetWindowText(" Lv.1 작물이름나옴 ") ;
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_CROPS_TIME_BAR , 75 , 312 , 4 , 45 , m_pGuildCorpsWnd );
	pWindowStatic->SetWindowType(WND_TYPE_TOPBASE);
	pWindowStatic->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 120, 133, 124, 143) ;
	pWindowStatic->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 120, 147, 124, 158) ;
	pWindowStatic->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 120, 160, 124, 170) ;
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_CROPS_GROWTH_TIME , 179 , 310 , 126 , 12 , m_pGuildCorpsWnd );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_VCENTER | DT_LEFT);
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_CROPS_MANURE_TIME , 179 , 327 , 126 , 12 , m_pGuildCorpsWnd );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_VCENTER | DT_LEFT);
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_CROPS_HASTEN_TIME , 191 , 344 , 126 , 12 , m_pGuildCorpsWnd );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_VCENTER | DT_LEFT);
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUILD_CROPS_IMAGE , 63 , 56 , 256 , 256 , m_pGuildCorpsWnd );
	pWindowStatic	=	NULL;
}


void SPWindowGuildInfo::SetGuildLevelTooltip()
{
	if( m_pGuildArchive == NULL )
		return ;

	int iKey = m_pGuildArchive->GetGuildLevel() * 10 + m_pGuildArchive->GetGuildProperty() ;
	//int iKey = 1 * 10 + 1 ;
	int iStringID = 0 ;
	PGUILD_ATTR pGuildData = NULL;
	pGuildData = g_pkGuildManager->FindGuildAttribute(iKey) ;
	if( pGuildData == NULL )
		return ;

	int i ;
	int iCount = 0 ;
	TCHAR szAbility[64] ;
	// Current Level =================================================
	if( g_pResourceManager->GetGlobalString(27000098) )
		_stprintf(szAbility, _T(g_pResourceManager->GetGlobalString(27000098)), pGuildData->iGuildMaxNum ) ;		// 길드 인원
	else
		_stprintf(szAbility, _T("UserCount : %d"), pGuildData->iGuildMaxNum ) ;		// 길드 인원
	m_stLevelAbility.strCurAbility[0] = szAbility ;
	++iCount ;
	for( i = 0; i < GUILD_STATUS_COUNT; i++ )
	{
		if( pGuildData->m_dwStatusType[i] != 0 )		{
			iStringID = 1020000 + pGuildData->m_dwStatusType[i] ;
			if( g_pResourceManager->GetGlobalString(iStringID) )
				_stprintf(szAbility, _T(g_pResourceManager->GetGlobalString(iStringID)), pGuildData->m_iStatusValue[i]) ;
			else
				_stprintf(szAbility, _T("Current Ability %d : %d"), pGuildData->m_dwStatusType[i], pGuildData->m_iStatusValue[i]) ;
			m_stLevelAbility.strCurAbility[iCount] = szAbility ;
			++iCount ;
		}
	}
	m_stLevelAbility.iCurAbilCount = iCount ;

	// Next Level =================================================
	pGuildData = NULL;
	iCount = 0 ;
	iKey = (m_pGuildArchive->GetGuildLevel() + 1) * 10 + m_pGuildArchive->GetGuildProperty() ;
	//iKey = 2 * 10 + 1 ;
	pGuildData = g_pkGuildManager->FindGuildAttribute(iKey) ;
	if( pGuildData == NULL )	{
		m_stLevelAbility.iNextAbilCount = 0 ;
	}
	else
	{
		if( g_pResourceManager->GetGlobalString(27000098) )
			_stprintf(szAbility, _T(g_pResourceManager->GetGlobalString(27000098)), pGuildData->iGuildMaxNum ) ;		// 길드 인원
		else
			_stprintf(szAbility, _T("UserCount : %d"), pGuildData->iGuildMaxNum ) ;		// 길드 인원
		m_stLevelAbility.strNextAbility[0] = szAbility ;
		++iCount ;
		for( i = 0; i < GUILD_STATUS_COUNT; i++ )
		{
			if( pGuildData->m_dwStatusType[i] != 0 )		{
				iStringID = 1020000 + pGuildData->m_dwStatusType[i] ;
				if( g_pResourceManager->GetGlobalString(iStringID) )
					_stprintf(szAbility, _T(g_pResourceManager->GetGlobalString(iStringID)), pGuildData->m_iStatusValue[i]) ;
				else
					_stprintf(szAbility, _T("Current Ability %d : %d"), pGuildData->m_dwStatusType[i], pGuildData->m_iStatusValue[i]) ;
				m_stLevelAbility.strNextAbility[iCount] = szAbility ;
				++iCount ;
			}
		}
		m_stLevelAbility.iNextAbilCount = iCount ;
	}
}


/**
BG로 출력되는 이미지의 좌표를 창 위치에 따라 업데이트한다.
*/
void SPWindowGuildInfo::ReposOwnImage()
{
	//if( m_pkModelUnit ) {
	//	float	fParsingX, fParsingY;
	//	WPARAM	wParsingX, wParsingY; 
	//	fParsingX = (float)m_iAX + 166.0f;
	//	fParsingY = (float)m_iAY + 300.0f;
	//	DataConvert<float,WPARAM>(fParsingX, wParsingX);
	//	DataConvert<float,WPARAM>(fParsingY, wParsingY);
	//	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSX, wParsingX );
	//	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSY, wParsingY );
	//}

	SetRect( &m_rcBGDest[0]	, m_iAX			, m_iAY			, m_iAX + 4		, m_iAY + 4 );			//
	SetRect( &m_rcBGDest[1]	, m_iAX + 4		, m_iAY			, m_iAX + 379	, m_iAY + 4 );			//
	SetRect( &m_rcBGDest[2]	, m_iAX + 379	, m_iAY 		, m_iAX + 383	, m_iAY + 4 );			//
	SetRect( &m_rcBGDest[3]	, m_iAX 		, m_iAY + 4		, m_iAX + 4		, m_iAY + 396 );		//
	SetRect( &m_rcBGDest[4]	, m_iAX + 4		, m_iAY + 4		, m_iAX + 379	, m_iAY + 396 );		//
	SetRect( &m_rcBGDest[5]	, m_iAX	+ 379	, m_iAY + 4		, m_iAX + 383	, m_iAY + 396 );		//
	SetRect( &m_rcBGDest[6]	, m_iAX 		, m_iAY + 396	, m_iAX + 4		, m_iAY + 400 );		//
	SetRect( &m_rcBGDest[7]	, m_iAX + 4		, m_iAY + 396	, m_iAX + 379	, m_iAY + 400 );		//
	SetRect( &m_rcBGDest[8]	, m_iAX + 379	, m_iAY + 396	, m_iAX + 383	, m_iAY + 400 );		//	
	SetRect( &m_rcBGDest[9]	, m_iAX + 2		, m_iAY + 26	, m_iAX + 381	, m_iAY + 27 );			//	짙은 초록 경계선
	SetRect( &m_rcBGDest[10], m_iAX + 2		, m_iAY + 27	, m_iAX + 381	, m_iAY + 396 );		//	중앙 흰색 서브 백그라운드
	SetRect( &m_rcBGDest[11], m_iAX + 50	, m_iAY + 35	, m_iAX + 54	, m_iAY + 52 );			//	Invite TEXT BG
	SetRect( &m_rcBGDest[12], m_iAX + 54	, m_iAY + 35	, m_iAX + 252	, m_iAY + 52 );			//	Invite TEXT BG
	SetRect( &m_rcBGDest[13], m_iAX + 251	, m_iAY + 35	, m_iAX + 255	, m_iAY + 52 );			//	Invite TEXT BG
	SetRect( &m_rcBGDest[14], m_iAX + 10	, m_iAY + 74	, m_iAX + 364	, m_iAY + 75 );			//	탭경계라인
	//SetRect( &m_rcBGDest[0]	, m_iAX + 2		, m_iAY + 26	, m_iAX + 381	, m_iAY + 27 );			//	짙은 초록 경계선
	//SetRect( &m_rcBGDest[1], m_iAX + 2		, m_iAY + 27	, m_iAX + 381	, m_iAY + 396 );		//	중앙 흰색 서브 백그라운드
	//SetRect( &m_rcBGDest[2], m_iAX + 50		, m_iAY + 35	, m_iAX + 54	, m_iAY + 52 );			//	Invite TEXT BG
	//SetRect( &m_rcBGDest[3], m_iAX + 54		, m_iAY + 35	, m_iAX + 252	, m_iAY + 52 );			//	Invite TEXT BG
	//SetRect( &m_rcBGDest[4], m_iAX + 251	, m_iAY + 35	, m_iAX + 255	, m_iAY + 52 );			//	Invite TEXT BG
	//SetRect( &m_rcBGDest[5], m_iAX + 10		, m_iAY + 74	, m_iAX + 364	, m_iAY + 75 );			//	탭경계라인


	SetRect( &m_rcListBGDst[0]	, m_iAX	+ 10	, m_iAY	+ 83	, m_iAX + 48	, m_iAY + 98 );		// 유형
	SetRect( &m_rcListBGDst[1]	, m_iAX	+ 52	, m_iAY	+ 83	, m_iAX + 168	, m_iAY + 98 );		// 아이디
	SetRect( &m_rcListBGDst[2]	, m_iAX	+ 172	, m_iAY	+ 83	, m_iAX + 210	, m_iAY + 98 );		// 레벨
	SetRect( &m_rcListBGDst[3]	, m_iAX	+ 214	, m_iAY	+ 83	, m_iAX + 271	, m_iAY + 98 );		// 기여도
	SetRect( &m_rcListBGDst[4]	, m_iAX	+ 275	, m_iAY	+ 83	, m_iAX + 313	, m_iAY + 98 );		// 채널
	SetRect( &m_rcListBGDst[5]	, m_iAX	+ 317	, m_iAY	+ 83	, m_iAX + 355	, m_iAY + 98 );		// 작위
	SetRect( &m_rcListBGDst[6]	, m_iAX	+ 10	, m_iAY	+ 230	, m_iAX + 355	, m_iAY + 231 );	// 리스트 하단 라인
	SetRect( &m_rcListBGDst[7]	, m_iAX	+ 2		, m_iAY	+ 234	, m_iAX + 139	, m_iAY + 247 );	// OffLine 표시 Text
	SetRect( &m_rcListBGDst[8]	, m_iAX	+ 15	, m_iAY	+ 260	, m_iAX + 152	, m_iAY + 275 );	// 슬로건 Title
	SetRect( &m_rcListBGDst[9]	, m_iAX	+ 27	, m_iAY	+ 279	, m_iAX + 29	, m_iAY + 336 );	// 슬로건 BG 좌
	SetRect( &m_rcListBGDst[10]	, m_iAX	+ 29	, m_iAY	+ 279	, m_iAX + 354	, m_iAY + 336 );	// 슬로건 BG 중
	SetRect( &m_rcListBGDst[11]	, m_iAX	+ 354	, m_iAY	+ 279	, m_iAX + 356	, m_iAY + 336 );	// 슬로건 BG 우

	SetRect( &m_rcStatusBGDst[0]	, m_iAX	+ 31	, m_iAY	+ 83	, m_iAX + 168	, m_iAY + 98 );		// 길드 마스터
	SetRect( &m_rcStatusBGDst[1]	, m_iAX	+ 47	, m_iAY	+ 106	, m_iAX + 49	, m_iAY + 133 );	// 마스터 BG
	SetRect( &m_rcStatusBGDst[2]	, m_iAX	+ 49	, m_iAY	+ 106	, m_iAX + 338	, m_iAY + 133 );	// 마스터 BG
	SetRect( &m_rcStatusBGDst[3]	, m_iAX	+ 338	, m_iAY	+ 106	, m_iAX + 340	, m_iAY + 133 );	// 마스터 BG
	SetRect( &m_rcStatusBGDst[4]	, m_iAX	+ 31	, m_iAY	+ 149	, m_iAX + 168	, m_iAY + 164 );	// 길드 정보
	SetRect( &m_rcStatusBGDst[5]	, m_iAX	+ 51	, m_iAY	+ 172	, m_iAX + 138	, m_iAY + 185 );	// 길드 이름
	SetRect( &m_rcStatusBGDst[6]	, m_iAX	+ 51	, m_iAY	+ 186	, m_iAX + 138	, m_iAY + 199 );	// 길드 레벨
	SetRect( &m_rcStatusBGDst[7]	, m_iAX	+ 51	, m_iAY	+ 200	, m_iAX + 138	, m_iAY + 213 );	// 길드 성향
	SetRect( &m_rcStatusBGDst[8]	, m_iAX	+ 51	, m_iAY	+ 214	, m_iAX + 150	, m_iAY + 227 );	// 길드 포인트
	SetRect( &m_rcStatusBGDst[9]	, m_iAX	+ 30	, m_iAY	+ 235	, m_iAX + 167	, m_iAY + 250 );	// 부길마 목록
	SetRect( &m_rcStatusBGDst[10]	, m_iAX	+ 46	, m_iAY	+ 258	, m_iAX + 340	, m_iAY + 259 );	// 라인
	SetRect( &m_rcStatusBGDst[11]	, m_iAX	+ 46	, m_iAY	+ 339	, m_iAX + 340	, m_iAY + 340 );	// 라인

	//if( m_pGuildArchive )	
	//	UpdateGuildPoint(m_pGuildArchive->GetGuildPointProtege()) ;

	SetRect( &m_rcCorpsBGDst[0]	, m_iAX	+ 45		, m_iAY	+ 82	, m_iAX + 167	, m_iAY + 97 );		// 작물정보 TEXT
	SetRect( &m_rcCorpsBGDst[1]	, m_iAX + 83		, m_iAY + 310	, m_iAX + 205	, m_iAY + 321 );	// 작물성장시간 Text
	SetRect( &m_rcCorpsBGDst[2]	, m_iAX + 83		, m_iAY + 327	, m_iAX + 205	, m_iAY + 338 );	// 비료 시간 Text
	SetRect( &m_rcCorpsBGDst[3]	, m_iAX + 83		, m_iAY + 344	, m_iAX + 205	, m_iAY + 355 );	// 촉진재 시간 Text

	if( m_pGuildArchive )
		UpdateGuildPoint(m_pGuildArchive->GetGuildPointProtege()) ;
}



/**
변경 가능으로 구성된 컨트롤들을 업데이트를 통해 보여주거나 감춘다.
*/
void SPWindowGuildInfo::UpdateRankControl()
{
	int i ;
	SPWindowStatic*	pWindowStatic;
	SPWindowButton*	pWindowButton;
	SPWindowEdit*	pWindowEdit; 
	SPWindowEditMultiLine*	pWindowMEdit; 

	// 초대 기능
	pWindowEdit		= (SPWindowEdit*)Find(WIID_GUILD_BASE_INVITE_NAME, true) ;
	if( pWindowEdit )	
	{
		(m_pGuildArchive->GetInvitable()) ? pWindowEdit->SetEnable(true) : pWindowEdit->SetEnable(false) ;
	}
	pWindowButton	= (SPWindowButton*)Find(WIID_GUILD_BASE_INVITE_BTN, true) ;
	if( pWindowButton )	
		(m_pGuildArchive->GetInvitable()) ? pWindowButton->SetEnable(true) : pWindowButton->SetEnable(false) ;

	if( m_iCurMenu == GUILD_MENU_LIST )
	{
		// 슬로건변경 가능
		pWindowStatic	= (SPWindowStatic*)Find(WIID_GUILD_LIST_SLOGUN_STC, true) ;				// Static
		pWindowMEdit	= (SPWindowEditMultiLine*)Find(WIID_GUILD_LIST_SLOGUN_EDIT, true) ;		// Edit
		pWindowButton	= (SPWindowButton*)Find(WIID_GUILD_LIST_SLOGUNBTN, true) ;				// Button
		if( m_pGuildArchive->GetSlogunChangable() ) {
			SetRect( &m_rcListBGSrc[9]	, 502 , 152 , 504 , 209 );		// 슬로건 BG 좌
			SetRect( &m_rcListBGSrc[10]	, 505 , 152 , 507 , 209 );		// 슬로건 BG 중
			SetRect( &m_rcListBGSrc[11]	, 509 , 152 , 511 , 209 );		// 슬로건 BG 우
			if( pWindowStatic )	pWindowStatic->Hide() ;
			if( pWindowMEdit )	{
				pWindowMEdit->SetWindowText("") ;
				pWindowMEdit->Show() ;
			}
			if( pWindowButton )	pWindowButton->Show() ;
		}
		else	{
			SetRect( &m_rcListBGSrc[9]	, 492 , 152 , 494 , 209 );		// 슬로건 BG 좌
			SetRect( &m_rcListBGSrc[10]	, 495 , 152 , 497 , 209 );		// 슬로건 BG 중
			SetRect( &m_rcListBGSrc[11]	, 499 , 152 , 501 , 209 );		// 슬로건 BG 우
			if( pWindowMEdit)	pWindowMEdit->Hide() ;
			if( pWindowButton)	pWindowButton->Hide() ;
			if( pWindowStatic)	pWindowStatic->Show() ;
		}

		bool bSet = m_pGuildArchive->GetRankChangable() ;
		for( i = 0; i < MAX_GUILD_GRADE_VIEW; i++)		{
			m_pGuildPopupList[i]->SetSelectAble(bSet) ;
		}

		pWindowButton	= (SPWindowButton*)Find(WIID_GUILD_LIST_EXPEL, true) ;
		if( pWindowButton )	
			(m_pGuildArchive->GetExpelable()) ? pWindowButton->Show() : pWindowButton->Hide() ;
	}
	else if ( m_iCurMenu == GUILD_MENU_STATUS )
	{
		// 권한 위임 가능
		pWindowButton	= (SPWindowButton*)Find(WIID_GUILD_STUS_MSTCHANGE, true) ;
		if( pWindowButton )	( m_pGuildArchive->IsMaster() ) ? pWindowButton->Show() : pWindowButton->Hide() ;
	}
	else if ( m_iCurMenu == GUILD_MENU_CROPS )
	{

	}
}


void SPWindowGuildInfo::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowGuildInfo::Clean()
{
	SPWindow::Clean();

	DeleteMemberList() ;
	SAFE_RELEASE( m_pBGTexture );
	// CleanCropMonster() ;
}



void SPWindowGuildInfo::Show(bool bWithChild)
{
	if( m_pGuildArchive == NULL )							return ;
	if( m_pGuildArchive->GetBasicAvailable() == false)		{
		if( g_pResourceManager->GetGlobalString(27000060) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000060) );
		return ;
	}

	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Show(bWithChild);

	ShowGuildName() ;			// Guild Name
	ShowGuildEmblem() ;			// Emblem
	OnGuildListButton(0, 0) ;	// Default Guild List Click
	OnShowOffline(0, 0) ;		// Default ShowOffLine
}

void SPWindowGuildInfo::Hide(bool bSendServer)
{
	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	m_pLevelTooltip->SetShowEnable(false) ;
	m_pLevelTooltip->Hide() ;
	m_bShowAbilToolTip = false ;

	OnToolTipOut(0, 0) ;

	//	CleanCropMonster() ;
	DeleteMemberList() ;
	SPWindow::Hide(bSendServer);

	OnEditKillFocus( NULL , NULL );
	m_pEditInviteName->KillFocus() ;
	KillFocus() ;
}

void SPWindowGuildInfo::Process(float fTime)
{
	//int iUpdate = 0 ;
	if( m_iCurMenu == GUILD_MENU_CROPS )
	{
		//if( m_pkModelUnit )		
		//{
		//	m_fAnimationTime -= fTime;
		//	if( m_fAnimationTime <= 0.0f )
		//		ChangeAnimation(m_iCropImageID);

		//	//m_iAX + 31 , m_iAY + 150
		//	float	fParsingX, fParsingY;
		//	WPARAM	wParsingX, wParsingY; 
		//	fParsingX = (float)m_iAX + 166.0f;
		//	fParsingY = (float)m_iAY + 300.0f;
		//	DataConvert<float,WPARAM>(fParsingX, wParsingX);
		//	DataConvert<float,WPARAM>(fParsingY, wParsingY);
		//	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSX, wParsingX );
		//	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSY, wParsingY );
		//	m_pkModelUnit->Process(fTime);
		//}
	}

	SPWindow::Process( fTime );
}



/**
길드명 보이기
*/
void SPWindowGuildInfo::ShowGuildName()
{
	char szTitle[64] ;
	if( m_pGuildArchive == NULL )	return ;
	if( m_pGuildArchive->GetGuildName() == NULL )	return ;
	SPWindowStatic* pStatic ;
	pStatic = (SPWindowStatic*)Find(WIID_GUILD_BASE_TITLE_TEXT2, true) ;
	if( pStatic )
	{
		sprintf( szTitle, "%s ]", m_pGuildArchive->GetGuildName()) ;
		pStatic->SetWindowText( szTitle ) ;
	}
}

/**
길드 타이틀 앰블럼 보이기
*/
void SPWindowGuildInfo::ShowGuildEmblem()
{
	int iPosX, iPosY ;
	int iEM, iBG ;
	SPWindowStatic* pStatic ;
	//EMBLEM_INFO* pEmblem = NULL ;

	if( m_pGuildArchive == NULL )	return ;

	iEM = m_pGuildArchive->GetGuildEmblem(0) ;
	iBG = m_pGuildArchive->GetGuildEmblem(1) ;

	// Emblem
	iPosX = iEM ? (iEM%21)*24 : 0 ;
	iPosY = iEM ? (iEM/21)*24 : 0 ;
	pStatic = (SPWindowStatic*)Find(WIID_GUILD_BASE_TITLE_ICON1, true) ;
	if( pStatic ) {
		pStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
		if( g_pkGuildManager->GetSymbolFilename() )
			pStatic->SetImage( g_pkGuildManager->GetSymbolFilename(), iPosX, iPosY) ;
	}

	// BG
	iPosX = iBG ? (iBG%21)*24 : 0 ;
	iPosY = iBG ? (iBG/21)*24 : 0 ;
	pStatic = (SPWindowStatic*)Find(WIID_GUILD_BASE_TITLE_ICON2, true) ;
	if( pStatic ) {
		pStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
		if( g_pkGuildManager->GetEmblemBGFilename() )
			pStatic->SetImage( g_pkGuildManager->GetEmblemBGFilename(), iPosX, iPosY) ;
	}
}


/**
Slider의 Show / Hide 정의
*/
void SPWindowGuildInfo::ShowSlider(bool bShow)
{
	SPWindowButton* pWindowButton = NULL ;
	if( m_iCurMenu == GUILD_MENU_LIST )
	{
		(bShow) ? m_pUserListSlider->Show() : m_pUserListSlider->Hide() ;
		pWindowButton = (SPWindowButton*)Find(WIID_GUILD_LIST_SLIDERUP, true) ;
		if( pWindowButton )		
			( bShow ) ? pWindowButton->Show() : pWindowButton->Hide() ;
		pWindowButton = (SPWindowButton*)Find(WIID_GUILD_LIST_SLIDERDOWN, true) ;
		if( pWindowButton )	
			( bShow ) ? pWindowButton->Show() : pWindowButton->Hide() ;
	}
	else if( m_iCurMenu == GUILD_MENU_STATUS )
	{
		( bShow ) ? m_pMasterListSlider->Show() : m_pMasterListSlider->Hide() ;
		pWindowButton = (SPWindowButton*)Find(WIID_GUILD_STUS_MSTLIST_SLIDERUP, true) ;
		if( pWindowButton )
			( bShow ) ? pWindowButton->Show() : pWindowButton->Hide() ;
		pWindowButton = (SPWindowButton*)Find(WIID_GUILD_STUS_MSTLIST_SLIDERDOWN, true) ;
		if( pWindowButton )	
			( bShow ) ? pWindowButton->Show() : pWindowButton->Hide() ;
	}
}

/**
NoCrop 이미지의 출력 여부 결정
*/
void SPWindowGuildInfo::ShowNoCrops(bool bShow)
{
	SPWindow* pWindow ;
	bool	bDragCrop = false ;
	if( (pWindow = Find(WIID_GUILD_NO_CROPS, true)) != NULL )
		(bShow) ? pWindow->Show() : pWindow->Hide() ;

	// Master이면 작물 등록 정보 출력
	if( m_pGuildArchive->IsMaster() && bShow )
		bDragCrop = true ;

	if( (pWindow = Find(WIID_GUILD_CROP_REGISTRY_BG, true)) != NULL )
		(bDragCrop) ? pWindow->Show() : pWindow->Hide() ;
	if( (pWindow = Find(WIID_GUILD_CROP_REGISTRY_AREA, true)) != NULL )
		(bDragCrop) ? pWindow->Show() : pWindow->Hide() ;
	if( (pWindow = Find(WIID_GUILD_CROP_REGISTRY_ARROW, true)) != NULL )
		(bDragCrop) ? pWindow->Show() : pWindow->Hide() ;
	if( (pWindow = Find(WIID_GUILD_CROP_REGISTRY_TEXT, true)) != NULL )
		(bDragCrop) ? pWindow->Show() : pWindow->Hide() ;
}




/**
Sub Master List를 Update한다.
*/
void SPWindowGuildInfo::SetShowMasterList(int iShowCount, std::vector<PGUILD_MEMBER>* vpShowMemver) 
{
	for( int i = 0; i < MAX_MASTER_LIST_VIEW; i++)
	{
		m_pGuildSubMaster[i]->SetWndState(WND_STATE_NORMAL) ;
		if( i < iShowCount )
		{
			m_pGuildSubMaster[i]->SetIconImagePos( 13 , 0 );
			m_pGuildSubMaster[i]->SetIconImageSize( 16 , 16 );
			m_pGuildSubMaster[i]->SetIClassImage( (*vpShowMemver)[i]->byClass ) ;

			m_pGuildSubMaster[i]->SetTextPos( 44 , 2 );
			m_pGuildSubMaster[i]->SetTextSize( 160 , 12 );
			m_pGuildSubMaster[i]->SetTextColor( RGBA(49, 76, 97, 255) );
			m_pGuildSubMaster[i]->SetTextString( (*vpShowMemver)[i]->szName ) ;

			m_pGuildSubMaster[i]->ShowToolTip( false );
			m_pGuildSubMaster[i]->RefreshRelationCoord() ;
			m_pGuildSubMaster[i]->SetShowEnable(true) ;
			m_pGuildSubMaster[i]->Show() ;
		}
		else
		{
			m_pGuildSubMaster[i]->SetShowEnable(false) ;
			m_pGuildSubMaster[i]->Hide() ;
		}
	}
	m_iCurSelected = m_iCurSelecteIndex - m_pMasterListSlider->GetCurPage() ;
	if( m_iCurSelected > -1 && m_iCurSelected < MAX_MASTER_LIST_VIEW)
		m_pGuildSubMaster[m_iCurSelected]->SetWndState(WND_STATE_CLICK) ;
}



void SPWindowGuildInfo::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )		// BackGround Image Process
	{
		m_pBGTexture->SetColor(RGBA(255, 255, 255, 255));
		for( int i = 0 ; i < GUILD_BG_IMG ; i++ )		{
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}
		g_pVideo->Flush();

		if( m_iCurMenu == GUILD_MENU_LIST ) {
			for( int i = 0 ; i < GUILD_LISTBG_IMG ; i++ )		{
				m_pBGTexture->RenderTexture( &m_rcListBGDst[i] , &m_rcListBGSrc[i] );
				//g_pVideo->GetSysLine()->DrawRect(m_rcListBGDst[i]);
			}
			g_pVideo->Flush();
		}

		if( m_iCurMenu == GUILD_MENU_STATUS ) {
			for( int i = 0 ; i < GUILD_STUSBG_IMG ; i++ )		{
				m_pBGTexture->RenderTexture( &m_rcStatusBGDst[i] , &m_rcStatusBGSrc[i] );
			}
			m_pBGTexture->RenderTexture( &m_rcPointGageDst[0] , &m_rcPointGageSrc[0] );		// Point Gage
			m_pBGTexture->RenderTexture( &m_rcPointGageDst[1] , &m_rcPointGageSrc[1] );
			g_pVideo->Flush();

			RenderGageNumber(fTime) ;	// Point Number
			g_pVideo->Flush();
		}

		if( m_iCurMenu == GUILD_MENU_CROPS && m_pGuildArchive->hasCorps() ) {
			for( int i = 0 ; i < GUILD_CROPSBG_IMG ; i++ )		{
				m_pBGTexture->RenderTexture( &m_rcCorpsBGDst[i] , &m_rcCorpsBGSrc[i] );
			}
			g_pVideo->Flush();
			//if(m_pkModelUnit)
			//	m_pkModelUnit->Render(fTime);
		}
		g_pVideo->Flush();
	}

	SPWindow::Render(fTime);

	if( m_bShowAbilToolTip )
	{
		RenderLevelTooltip(fTime) ;
	}
}


/**
길드 경험치 숫자 출력
*/
void SPWindowGuildInfo::RenderGageNumber(float fTime)
{
	int iNum[3] ;
	int iProtege = m_pGuildArchive->GetGuildPointProtege() ;
	if( iProtege == 0 )
		iNum[0] = iNum[1] = iNum[2] = 0 ;
	else	{
		iNum[0] = iProtege / 100 ;
		iNum[1] = iProtege / 10 ;
		iNum[2] = iProtege % 10 ;
	}
	if( iNum[0] != 0 )	{
		m_pPointNum->AdjustCoord(209, 216) ;
		m_pPointNum->SPSendMessage(SPIM_SET_IMAGEINDEX, iNum[0]);			// 100
		m_pPointNum->Render(fTime) ;
		m_pPointNum->AdjustCoord(216, 216) ;
		m_pPointNum->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);					// 100
		m_pPointNum->Render(fTime) ;
	}
	else if( iNum[1] != 0 )	{
		m_pPointNum->AdjustCoord(216, 216) ;
		m_pPointNum->SPSendMessage(SPIM_SET_IMAGEINDEX, iNum[1]);			// -1-
		m_pPointNum->Render(fTime) ;
	}

	m_pPointNum->AdjustCoord(223, 216) ;
	m_pPointNum->SPSendMessage(SPIM_SET_IMAGEINDEX, iNum[2]);			// --1
	m_pPointNum->Render(fTime) ;

	m_pPointNum->AdjustCoord(230, 216) ;
	m_pPointNum->SPSendMessage(SPIM_SET_IMAGEINDEX, 10);			// %
	m_pPointNum->Render(fTime) ;
}


/**
길드 레벨 툴팁 렌더링
*/
void SPWindowGuildInfo::RenderLevelTooltip(float fTime)
{
	if( m_stLevelAbility.iCurAbilCount < 1 )
		return ;

	SPFont* pTitleFont = g_pVideo->GetFont(FONT_14_BOLD) ;
	SPFont* pTextFont = g_pVideo->GetFont(FONT_12_NORMAL) ;
	bool bTitleShadow = pTitleFont->IsShadow() ;
	bool bTextShadow = pTextFont->IsShadow() ;
	int iToolTipHeight = 0 ;
	RECT rtStatic ;
	RECT rtText ;

	if( m_pLevelTooltip == NULL )
		return ;
	m_pLevelTooltip->GetWindowRect(rtStatic) ;
	pTitleFont->SetShadow(false) ;
	pTextFont->SetShadow(false) ;
	pTitleFont->SetColor(D3DXCOLOR(0.172f, 0.56, 0.207, 1.0f)) ;
	pTextFont->SetColor(D3DXCOLOR(0.192f, 0.298, 0.38, 1.0f)) ;

	int i ;
	DWORD dwAlign = DT_TOP | DT_CENTER ;
	SetRect(&rtText, rtStatic.left+11, rtStatic.top+11, rtStatic.right-22, rtStatic.top+25) ;
	if( g_pResourceManager->GetGlobalString(27000103) )
		pTitleFont->RenderText(g_pResourceManager->GetGlobalString(27000103), &rtText, dwAlign) ;
	else
		pTitleFont->RenderText("Current Guild Ability", &rtText, dwAlign) ;
	dwAlign = DT_TOP | DT_LEFT ;
	for( i = 0 ; i < m_stLevelAbility.iCurAbilCount; i++ )	{
		SetRect(&rtText, rtStatic.left+11, rtStatic.top+35+i*14, rtStatic.right-22, rtStatic.top+47+i*14 ) ;
		pTextFont->RenderText(m_stLevelAbility.strCurAbility[i].c_str(), &rtText, dwAlign) ;
	}
	iToolTipHeight = rtText.bottom + 10 ;

	if( m_stLevelAbility.iNextAbilCount > 1 )	{
		dwAlign = DT_TOP | DT_CENTER ;
		pTitleFont->SetColor(D3DXCOLOR(0.56f, 0.172, 0.282, 1.0f)) ;
		SetRect(&rtText, rtStatic.left+11, iToolTipHeight, rtStatic.right-22, iToolTipHeight+14) ;
		if( g_pResourceManager->GetGlobalString(27000104) )
			pTitleFont->RenderText(g_pResourceManager->GetGlobalString(27000104), &rtText, dwAlign) ;
		else
			pTitleFont->RenderText("Next Guild Ability", &rtText, dwAlign) ;
		dwAlign = DT_TOP | DT_LEFT ;
		for( i = 0 ; i < m_stLevelAbility.iNextAbilCount; i++ )	{
			SetRect(&rtText, rtStatic.left+11, iToolTipHeight+24+i*14, rtStatic.right-22, iToolTipHeight+36+i*14 ) ;
			pTextFont->RenderText(m_stLevelAbility.strNextAbility[i].c_str(), &rtText, dwAlign) ;
		}
		iToolTipHeight = rtText.bottom + 11 ;
	}
	m_pLevelTooltip->SetRectSize(rtStatic.right-rtStatic.left, iToolTipHeight-rtStatic.top) ;
	m_pLevelTooltip->ReposOwnImage() ;

	pTitleFont->SetShadow(bTitleShadow) ;
	pTextFont->SetShadow(bTextShadow) ;
}


/**
길드 속성 타입에 따라 Popup창을 설정한다.
속성 POPUP에 길마는 안나온다 (Rank2부터 나온다)
*/
void SPWindowGuildInfo::UpdatePopupWidthProperty(int iProperty)
{
	int iX, iY ;
	int	iStringID ;
	switch( iProperty )
	{
	case 1:				// 일반형
		iX = 410;		iY = 138;
		iStringID = 27000047 ;
		break ;
	case 2 :			// 천사형
		iX = 410;		iY = 110;
		iStringID = 27000035 ;
		break ;
	case 3:				// 악마형
		iX = 410;		iY = 124;
		iStringID = 27000041 ;
		break ;
	default:			// 일반형
		iX = 410;		iY = 138;
		iStringID = 27000047 ;
		break ;
	}

	for( int i = 0; i < MAX_GUILD_GRADE_VIEW; i++)
	{
		iX += 17  ;		// iRank 가 1부터 시작인경우
		m_pGuildPopupList[i]->SetIconImagePos( 8 , 1 );
		m_pGuildPopupList[i]->SetIconImageSize( 16 , 13 );
		m_pGuildPopupList[i]->SetIconImage( "DATA/INTERFACE/CONCEPT/UI105.PNG" , iX , iY );

		//if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_SANDA )
		//{
		//	m_pGuildPopupList[i]->SetTextPos( 30 , 2 );
		//	m_pGuildPopupList[i]->SetTextSize( 53 , 12 );
		//}
		//else
		//{
		m_pGuildPopupList[i]->SetTextPos( 28 , 2 );
		m_pGuildPopupList[i]->SetTextSize( 97 , 12 );
		//}

		m_pGuildPopupList[i]->SetTextColor( RGBA(49, 76, 97, 255) );
		if( g_pResourceManager->GetGlobalString(iStringID + i) ) {
			m_pGuildPopupList[i]->SetTextString(g_pResourceManager->GetGlobalString(iStringID+i)) ;
			//ErrorFLogs->Error_Log("UpdateProperty[%d] - [%s]", i, g_pResourceManager->GetGlobalString(iStringID+i)) ;
		}
		else	{
			m_pGuildPopupList[i]->SetTextString("Need") ;
			//ErrorFLogs->Error_Log("UpdateProperty[%d] - Icon[%d, %d] Text[%d, %d][%s]", i, 9, 8, 32, 9, "Need") ;
		}

		m_pGuildPopupList[i]->ShowToolTip( false );
		m_pGuildPopupList[i]->RefreshRelationCoord() ;
	}
}



/**
길드 슬로건 업데이트
*/
void SPWindowGuildInfo::UpdateSlogun()
{
	if( m_pGuildArchive->GetSlogunChangable() )
	{
		SPWindowEditMultiLine* pEdit ;
		pEdit = (SPWindowEditMultiLine*)Find(WIID_GUILD_LIST_SLOGUN_EDIT, true) ;
		if( m_pGuildArchive )
			pEdit->SetWindowText( m_pGuildArchive->GetGuildSlogun() ) ;
	}
	else 
	{
		SPWindowStatic* pStatic ;
		pStatic = (SPWindowStatic*)Find(WIID_GUILD_LIST_SLOGUN_STC, true) ;
		if( m_pGuildArchive )
			pStatic->SetWindowText( m_pGuildArchive->GetGuildSlogun() ) ;
	}
}



/**
[길드상태] 정보창의 업데이트
*/
void SPWindowGuildInfo::UpdateStatusInfo()
{
	TCHAR szText[32] ;
	SPWindowStatic*	pStatic ;

	if( m_pGuildArchive == NULL )		return ;

	UpdateMasterInfo() ;
	UpdateGuildPoint(m_pGuildArchive->GetGuildPointProtege()) ;

	pStatic = (SPWindowStatic*)Find(WIID_GUILD_STUS_NAME, true) ;
	if( pStatic )	pStatic->SetWindowText( m_pGuildArchive->GetGuildName() ) ;

	if( m_pGuildArchive->GetGuildLevel() > 0 || m_pGuildArchive->GetGuildLevel() <= g_pkGuildManager->GetLimitGuildLevel() )
	{
		wsprintf(szText, "%s", g_pResourceManager->GetGlobalString(27000120 + m_pGuildArchive->GetGuildLevel()) ) ;
		pStatic = (SPWindowStatic*)Find(WIID_GUILD_STUS_LEVEL, true) ;
		if( pStatic )	pStatic->SetWindowText( szText) ;
	}
	else
	{
		wsprintf(szText, "Lv.%d", m_pGuildArchive->GetGuildLevel() ) ;
		pStatic = (SPWindowStatic*)Find(WIID_GUILD_STUS_LEVEL, true) ;
		if( pStatic )	pStatic->SetWindowText( szText) ;
	}


	pStatic = (SPWindowStatic*)Find(WIID_GUILD_STUS_PROPERTY, true) ;
	switch( m_pGuildArchive->GetGuildProperty() )
	{
		//////////////////////////////////////////////////////////////////////////
		// Global String 으로 수정
	case GUILD_PROPERTY_NORMAL:
		if( pStatic )	
			pStatic->SetWindowText( g_pResourceManager->GetGlobalString(27000011)) ;
		break ;
	case GUILD_PROPERTY_ANGEL:
		if( pStatic )	
			pStatic->SetWindowText( g_pResourceManager->GetGlobalString(27000018) ) ;
		break ;
	case GUILD_PROPERTY_DAEMON:
		if( pStatic )	
			pStatic->SetWindowText( g_pResourceManager->GetGlobalString(27000020) ) ;
		break ;
	}
}



/**
[작물상태] 정보창의 업데이트
*/
void SPWindowGuildInfo::UpdateCropInfo(bool bSet)
{
	int		iCropLevel = -1 ;
	POINT	ptSrcPos;
	SPWindowStatic*	pStatic ;
	SPTexture* pTexture = NULL ;
	TCHAR	szText[64] ;

	if( m_pGuildArchive == NULL )		return ;
	pStatic = (SPWindowStatic*)Find(WIID_GUILD_CROPS_ICON, true) ;
	pStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
	if( bSet )	{
		pTexture = g_pItemCluster->GetItemTexture(m_pGuildArchive->GetCropArchive()->GetUseItemForIcon(), ptSrcPos);
		pStatic->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y) ;
	}

	//WIID_GUILD_CROPS_NAME
	iCropLevel = g_pGOBManager->GetGuildArchive()->GetCropArchive()->GetCropLevel() ;
	if( iCropLevel < 0 || iCropLevel >= CROP_LEVEL_MAX ){
		ErrorFLogs->Error_Log("##CropLevel is Invalid[%d]", iCropLevel) ;
		return ;
	}
	pStatic = (SPWindowStatic*)Find(WIID_GUILD_CROPS_NAME, true) ;
	sprintf(szText, "Lv.%d %s", iCropLevel+1, m_pGuildArchive->GetCropArchive()->GetCropName()) ;
	pStatic->SetWindowText(szText) ;

	// Crop Image
	if( bSet )	{
		UpdateCropImage(iCropLevel) ;
		UpdateCropTime() ;
	}
}

/**
CropImage Update가 필요한경우 CropMonster를 삭제후 재 생성한다.
*/
void SPWindowGuildInfo::UpdateCropImage(int iCropLevel)
{
	UINT32	iCropDlgImageID ;
	SPWindowStatic*	pStatic = NULL;
	SPTexture* pTexture = NULL ;

	iCropDlgImageID = g_pGOBManager->GetGuildArchive()->GetCropArchive()->GetCropDlgImageID(iCropLevel) ;
	if( iCropDlgImageID == 0 )	{
		ErrorFLogs->Error_Log("##CropImageID is Zero") ;
		return ;
	}
	pStatic = (SPWindowStatic*)Find(WIID_GUILD_CROPS_IMAGE, true) ;
	pStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
	const char* pCropImgName = g_pResourceManager->GetGlobalFileName(iCropDlgImageID) ;
	if( pCropImgName == NULL )
	{
		ErrorFLogs->Error_Log("##CropImage Filename is NULL [%d]", iCropDlgImageID) ;
		return ;
	}
	pStatic->SetImage(pCropImgName, 0, 0) ;

	//CleanCropMonster() ;
	//AddCropMonster() ;
}


/**
[작물상태] 작물 성장시간 업데이트
*/
void SPWindowGuildInfo::UpdateCropTime()
{
	char* pTimeStr ;
	char szTime[64] ;
	SPWindowStatic*	pStatic ;
	pTimeStr = szTime ;

	if( m_pGuildArchive == NULL )		return ;
	//WIID_GUILD_CROPS_GROWTH_TIME
	pStatic = (SPWindowStatic*)Find(WIID_GUILD_CROPS_GROWTH_TIME, true) ;
	ComputeTimeString(m_pGuildArchive->GetCropArchive()->GetRemainTime(), pTimeStr) ;
	pStatic->SetWindowText(pTimeStr);

	//WIID_GUILD_CROPS_MANURE_TIME
	pStatic = (SPWindowStatic*)Find(WIID_GUILD_CROPS_MANURE_TIME, true) ;
	ComputeTimeString(m_pGuildArchive->GetCropArchive()->GetNourishTime(), pTimeStr) ;
	pStatic->SetWindowText(pTimeStr);

	//WIID_GUILD_CROPS_HASTEN_TIME
	pStatic = (SPWindowStatic*)Find(WIID_GUILD_CROPS_HASTEN_TIME, true) ;
	ComputeTimeString(m_pGuildArchive->GetCropArchive()->GetHastenSumTime(), pTimeStr) ;
	pStatic->SetWindowText(pTimeStr);
}


/**
시간 값으로 GlobalString의 TimeString을 만들어준다.
*/
bool SPWindowGuildInfo::ComputeTimeString(UINT32 iTime, char* &pstrTime)
{
	int iHour, iMin, iSec ;
	if( iTime == 0 )		{
		iHour	= 0 ;
		iMin	= 0 ;
		iSec	= 0 ;
	}
	else	{
		iHour	= iTime / 3600 ;
		iMin	= ( iTime % 3600 ) / 60 ;
		iSec	= iTime % 60 ;
	}
	sprintf(pstrTime, g_pResourceManager->GetGlobalString(27000076), iHour, iMin, iSec);
	return true ;
}



/**
[길드 상태]의 마스터 정보 설정
*/
void SPWindowGuildInfo::UpdateMasterInfo()
{
	TCHAR			szMasterInfo[64] ;
	SPWindowStatic*	pStatic ;
	PGUILD_MEMBER pMasterInfo ;
	if( m_pGuildArchive )
	{
		pMasterInfo = GetMasterInfo() ;
		if( pMasterInfo == NULL )	return ;

		pStatic = (SPWindowStatic*)Find(WIID_GUILD_STUS_MASTER_JOB, true) ;
		if( pStatic )	
		{
			int iClass = (int)pMasterInfo->byClass ;
			pStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);

			const char*	pTextureName;
			RECT*		pTextureSrc;
			pTextureName	=	g_pInterfaceManager->GetClassTextureName( (CLASS_TYPE)iClass );
			pTextureSrc		=	g_pInterfaceManager->GetClassTextureSrc( (CLASS_TYPE)iClass );
			if( pTextureName != NULL && pTextureSrc != NULL )
				pStatic->SetImage( pTextureName , pTextureSrc->left , pTextureSrc->top );
		}


		//WIID_GUILD_STUS_MASTER_INFO
		wsprintf(szMasterInfo, "Lv.%d %s", pMasterInfo->wLevel, pMasterInfo->szName) ;
		pStatic = (SPWindowStatic*)Find(WIID_GUILD_STUS_MASTER_INFO, true) ;
		if( pStatic )	pStatic->SetWindowText( szMasterInfo ) ;
	}
}


/**
[길드 상태]의 길드 경험치 게이지 설정
*/
void SPWindowGuildInfo::UpdateGuildPoint(int iProtege)
{
	// 게이지 전체 사이즈 : 137 - 꼬다리(6) == 131
	int iGageSize = 0 ;
	int iTailSize = 0 ;
	if( iProtege > 0 )
		iGageSize = (int)(iProtege * 1.31) ;
	else
		iProtege = 0 ;
	iTailSize = ( iProtege < 6 ) ? iProtege : 6 ;

	SetRect( &m_rcPointGageDst[0]	, m_iAX	+158	, m_iAY	+216	, m_iAX +158 + iGageSize	, m_iAY +225 );			//
	SetRect( &m_rcPointGageDst[1]	, m_rcPointGageDst[0].right		, m_rcPointGageDst[0].top	, m_rcPointGageDst[0].right+iTailSize	, m_rcPointGageDst[0].bottom );			//
}


/**
길드 작위 팝업 띄우기/ 닫기
*/
void SPWindowGuildInfo::ShowRankPopup(bool bShow)
{
	if( bShow)
	{
		m_pGuildGradePopup->SetShowEnable(true) ;
		m_pGuildGradePopup->Show() ;
		m_pGuildMember[m_iCurSelected]->ShowSelectBG(true) ;
		int iSelectedGrade = m_pGuildMember[m_iCurSelected]->GetGrade() ;
		if( iSelectedGrade > -1 )
		{
			m_iCurPopupSelect = iSelectedGrade - 2 ;
			m_pGuildPopupList[m_iCurPopupSelect]->SetWndState(WND_STATE_CLICK) ;
		}
		if( m_pUserListSlider->IsShow() )		ShowSlider(false) ;
	}
	else
	{
		m_pGuildGradePopup->SetShowEnable(false) ;
		m_pGuildGradePopup->Hide() ;
		m_pGuildMember[m_iPreSelected]->ShowSelectBG(false) ;
		if( m_pGuildArchive )
			if( m_iCurMenu == GUILD_MENU_LIST ) 
				if( GetGuildUserCount(ALL_USER_COUNT) > MAX_GUILD_LIST_VIEW )
					ShowSlider(true) ;
	}
}



/**
작물 아이템 Drag 체크
1) Drag Item 확인
2) Event Type 체크
@return 있으면:SPItem* , 없으면 NULLL
*/
SPItem* SPWindowGuildInfo::GetDragItem()
{
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return NULL;

	if(pInven->m_bPickup == false) {		//픽업 상태가 아닌경우 넘어간다
		return NULL;
	}	

	SPSelectItem SelectItem = pInven->GetPickupItem();
	SPItem* pItem = pInven->GetInventoryItem(SelectItem.m_iContainer, SelectItem.m_iSlotIndex);	

	// 아이템이 있는지
	if(pItem == NULL || pItem->GetItemStatus() == NULL || pItem->GetItemStatus()->GetItemID() == 0) {
		return NULL ;
	}

	// 이벤트아이템이 아닌경우
	if( SelectItem.GetContainerType() != INVENTORY_EVENT ) {
		return NULL ;
	}

	return pItem ;
}

//////////////////////////////////////////////////////////////////////////
//
//	Guild Member 
//
//////////////////////////////////////////////////////////////////////////
/**
길드원 목록 요청 패킷
*/
bool SPWindowGuildInfo::SendReqGuildMemberList()
{
	DeleteMemberList() ;

	CPacket packet;
	packet.AddUINT32( GUILD_CS_MEMBERLIST );	
	SENDPACKET(packet);
	return true ;
}

/**
[작위변경] 패킷 전송
*/
bool SPWindowGuildInfo::SendChangeGrade(const char* szSelName, int iGrade)
{
	if( GetGuildUserCount(ALL_USER_COUNT) < 1 )
	{
		return false ;
	}
	if( m_pGuildArchive->GetRankChangable() == false )
	{
		if( g_pResourceManager->GetGlobalString(27000006) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000006) );
		return false ;
	}

	INT32	iRcvCharID ;		// 변경 대상자의 DBNUM
	INT8	iRcvGrade ;
	int iSlot, iOn ;
	if( FindMemberIndex(szSelName, iSlot, iOn) == false )
		return false ;
	if( iOn )
		iRcvCharID = m_vpOnUsers[iSlot]->iCharID ;
	else
		iRcvCharID = m_vpOffUsers[iSlot]->iCharID ;
	iRcvGrade = (INT8)( iGrade ) ;		

	CPacket packet;
	packet.AddUINT32( GUILD_CS_CHANGEGRADE );	
	packet.AddUINT32( iRcvCharID ) ;
	packet.AddUINT8( iRcvGrade ) ;
	SENDPACKET(packet);
	return true ;
}


/**
길드 강퇴 패킷 보내기
*/
bool SPWindowGuildInfo::SendGuildExpel(const char* szCharName)
{
	PGUILD_MEMBER pUser ;
	if( m_pGuildArchive->GetBasicAvailable() == false)	return false ;		// 길드
	if( m_pGuildArchive->GetExpelable() == false )		return false ;		// 강퇴 가능
	pUser = FindGuildUser(szCharName) ;
	if( pUser == NULL )					return false ;		// 사용자 정보

	CPacket packet;
	packet.AddUINT32( GUILD_CS_EXPEL );	
	packet.AddUINT32( pUser->iCharID );	
	SENDPACKET(packet);
	return true ;
}

/**
[길드 마스터 위임] 패킷
*/
bool SPWindowGuildInfo::SendChangeManster(const char* szCharName)
{
	PGUILD_MEMBER pUser ;
	if( m_pGuildArchive->GetBasicAvailable() == false)	return false ;			// 길드
	if( m_pGuildArchive->IsMaster() == false )			return false ;			// 마스터
	pUser = FindGuildUser(szCharName) ;
	if( pUser == NULL )					return false ;			// 사용자 정보

	CPacket packet;
	packet.AddUINT32( GUILD_CS_CHANGEMASTER );	
	packet.AddUINT32( pUser->iCharID );
	SENDPACKET(packet);
	return true ;
}



/**
길드멤버정보 패킷을 받는 경우
*/
int	SPWindowGuildInfo::OnReceiveGuildUserList( WPARAM wParam , LPARAM lParam )
{
	CPacket* pPacket = NULL ;
	INT32				iMemberCnt;							// 길드 멤버수
	GUILD_MEMBER		stGuildMember;						// 길드 멤버정보

	pPacket = (CPacket*)lParam ;
	if( pPacket == NULL )
	{
		ErrorFLogs->Error_Log("#GuildInfo::OnReceiveGuildUserList is NULL") ;
		return 0 ;
	}

	if( m_pGuildArchive )	
		m_pGuildArchive->SetGuildTotalPoint(0) ;
	pPacket->ExtractUINT32((UINT32*)&iMemberCnt);
	for(int i=0; i<iMemberCnt; i++)
	{
		pPacket->ExtractStruct(&stGuildMember, sizeof(GUILD_MEMBER));
		AddGuildUser(&stGuildMember) ;
	}

	sort(m_vpOnUsers.begin(), m_vpOnUsers.end(), GuildSortByGrade) ;
	sort(m_vpOffUsers.begin(), m_vpOffUsers.end(), GuildSortByGrade) ;
	RemakeSubMasterList() ;
	SetEnableUserList() ;
	UpdateGuildUserList(0, LIST_UPDATE_USERLIST) ;
	return 1 ;
}


/**
길드원이 Join 하는 경우
*/
int	SPWindowGuildInfo::OnReceiveGuildUserJoin( WPARAM wParam , LPARAM lParam )
{
	GUILD_MEMBER*		pGuildMember = NULL ;						// 길드 멤버정보

	pGuildMember = (GUILD_MEMBER*)lParam ;
	if( pGuildMember == NULL )
	{
		ErrorFLogs->Error_Log("#GuildInfo::OnReceiveGuildUserJoin is NULL") ;
		return 0 ;
	}

	AddGuildUser(pGuildMember) ;
	sort(m_vpOnUsers.begin(), m_vpOnUsers.end(), GuildSortByGrade) ;
	RemakeSubMasterList() ;
	SetEnableUserList() ;
	if( m_iCurMenu == GUILD_MENU_LIST )
		UpdateGuildUserList(m_iCurPage, LIST_UPDATE_JOIN) ;
	else if( m_iCurMenu == GUILD_MENU_STATUS )
		UpdateGuildMasterList(m_iCurPage, LIST_UPDATE_USERLIST) ;
	return 1 ;
}


/**
길드원이 탈퇴 하는 경우
*/
int	SPWindowGuildInfo::OnReceiveGuildUserSecede( WPARAM wParam , LPARAM lParam )
{
	GUILD_MEMBER*		pGuildMember = NULL ;						// 길드 멤버정보
	pGuildMember = (GUILD_MEMBER*)lParam ;
	if( pGuildMember == NULL )
	{
		ErrorFLogs->Error_Log("#GuildInfo::OnReceiveGuildUserSecede is NULL") ;
		return 0 ;
	}

	DeleteGuildUser(pGuildMember->szName) ;
	if( m_iCurMenu == GUILD_MENU_LIST )
		UpdateGuildUserList(m_iCurPage, LIST_UPDATE_SECEDE) ;
	else if( m_iCurMenu == GUILD_MENU_STATUS )
		UpdateGuildMasterList(m_iCurPage, LIST_UPDATE_USERLIST) ;
	return 1 ;
}


/**
길드원의 멤버정보 변경(직위)
*/
int	SPWindowGuildInfo::OnReceiveChangeGuildUser( WPARAM wParam , LPARAM lParam )
{
	GUILD_MEMBER*		pGuildMember = NULL ;						// 길드 멤버정보
	pGuildMember = (GUILD_MEMBER*)lParam ;
	if( pGuildMember == NULL )
	{
		ErrorFLogs->Error_Log("#GuildInfo::OnReceiveGuildUserSecede is NULL") ;
		return 0 ;
	}
	// 1) DeleteUser
	// 2) AddGuildUser
	// 3) if View -> Update List
	DeleteGuildUser(pGuildMember->szName) ;
	AddGuildUser(pGuildMember) ;
	if( pGuildMember->wServerID <= 0 )
		sort(m_vpOnUsers.begin(), m_vpOnUsers.end(), GuildSortByGrade) ;
	else
		sort(m_vpOffUsers.begin(), m_vpOffUsers.end(), GuildSortByGrade) ;

	if( g_pNetworkManager->GetCharName() == pGuildMember->szName )	
		m_pGuildArchive->SetGuildAuthority((int)pGuildMember->byGrade) ;

	RemakeSubMasterList() ;
	if( m_iCurMenu == GUILD_MENU_LIST )
		UpdateGuildUserList(m_iCurPage, LIST_UPDATE_CHANGE) ;
	else if( m_iCurMenu == GUILD_MENU_STATUS )
		UpdateGuildMasterList(m_iCurPage, LIST_UPDATE_USERLIST) ;
	return 1 ;
}



/**
GuildUserList를 갱신한다.
*/
bool SPWindowGuildInfo::UpdateGuildUserList(int iNewPage, int iUpdateType)
{
	if( GetListAvailable() == false )
	{
		ErrorFLogs->Error_Log("There is No Guild User LIST !!!!!!!!!!!") ;
		SetShowGuildUserList(GUILD_MEMBER_USER, (NULL));
		return false ;
	}
	if( iUpdateType == LIST_UPDATE_SCROLL && m_iCurPage == iNewPage )		return false ;
	if( g_pInterfaceManager->GetGuildInfoWindow()->IsShow() == false )		return true ;
	m_iCurPage = iNewPage ;

	bool bOnline ;
	int i, iSlot ;
	int iTotalCount = ( m_bShowOffline ) ? GetGuildUserCount(ALL_USER_COUNT)  : GetGuildUserCount(ON_USER_COUNT) ;
	int iLoopCount = (iTotalCount > MAX_GUILD_LIST_VIEW) ? MAX_GUILD_LIST_VIEW : iTotalCount ;
	std::vector<PGUILD_MEMBER> vShowMemver ;
	PGUILD_MEMBER pGMember ;

	// Send Refresh Scroll (iTotalcount, iNewPage) ;
	UpdateListScroll(iTotalCount, iNewPage);

	for( i = 0; i < iLoopCount; i++ )
	{
		if( PageToIndex(iNewPage+i, bOnline, iSlot) == false )
		{
			ErrorFLogs->Error_Log("PageToIndex Fail Total[%d] Loop[%d] NewPage[%d], i[%d]", iTotalCount, iLoopCount, iNewPage, i) ;
			return false;
		}

		if( bOnline )	{
			pGMember = m_vpOnUsers[iSlot] ;
			vShowMemver.push_back(pGMember) ;
		}
		else			{
			pGMember = m_vpOffUsers[iSlot] ;
			vShowMemver.push_back(pGMember) ;
		}
		pGMember = NULL ;
	}

	// Send Show User List
	SetShowGuildUserList(GUILD_MEMBER_USER, (&vShowMemver));
	vShowMemver.clear() ;
	return true ;
}



/**
SubMaster 리스트 (GUILD_GRADE_02)
*/
bool SPWindowGuildInfo::UpdateGuildMasterList(int iNewPage, int iUpdateType) 
{
	if( GetListAvailable() == false )
	{
		ErrorFLogs->Error_Log("There is No MASTER LIST !!!!!!!!!!!") ;
		SetShowGuildUserList(GUILD_MEMBER_MASTER, (NULL));
		return false ;
	}
	if( iUpdateType == LIST_UPDATE_SCROLL && m_iCurPage == iNewPage )		return false ;
	m_iCurPage = iNewPage ;

	PGUILD_MEMBER pGMember ;
	std::vector<PGUILD_MEMBER> vSubMaster ;
	int iIndex ;
	//	int iSizeOnMaster = GetSubMasterCount(ON_USER_COUNT) ;
	int iTotalSubMaster = GetSubMasterCount(ALL_USER_COUNT) ;
	int iLoopCount = (iTotalSubMaster > MAX_MASTER_LIST_VIEW) ? MAX_MASTER_LIST_VIEW : iTotalSubMaster ;

	UpdateListScroll(iTotalSubMaster, iNewPage);

	for( int i = 0; i < iLoopCount; i++)
	{
		iIndex = iNewPage + i ;
		pGMember = m_vpSubMaster[iIndex] ;
		vSubMaster.push_back(pGMember) ;
		//ErrorFLogs->Error_Log("MASTER LIST >> Name[%s] Class[%d] Level[%d] Server[%d] Grade[%d]", pGMember->szName, (int)pGMember->byClass, pGMember->wLevel, pGMember->wServerID, (int)pGMember->byGrade) ;
		pGMember = NULL ;			
	}
	SetShowGuildUserList(GUILD_MEMBER_MASTER, (&vSubMaster));
	vSubMaster.clear() ;
	return true ;
}




/**
해당 길드멤버 제거
*/
bool SPWindowGuildInfo::DeleteGuildUser(const char* strUserName)
{
	if( GetListAvailable() == false )
		return false ;

	STD_VEC_GUILD::iterator	mIter;

	// 1) SubMaster List
	mIter	=	m_vpSubMaster.begin() ;
	while ( mIter != m_vpSubMaster.end() )
	{
		if( strcmp(strUserName, (*mIter)->szName) == 0 )
		{
			m_vpSubMaster.erase(mIter) ;
			break ;
		}
		++mIter ;
	}

	// 2) Online or Offline
	mIter	=	m_vpOnUsers.begin() ;
	while ( mIter != m_vpOnUsers.end() )
	{
		if( strcmp(strUserName, (*mIter)->szName) == 0 )
		{
			int iGPoint = (*mIter)->iPoint ;
			if( m_pGuildArchive && iGPoint != 0 ) 
				m_pGuildArchive->UpdateGuildTotalPoint(-iGPoint) ;
			SAFE_DELETE( (*mIter) );
			m_vpOnUsers.erase(mIter) ;
			return true ;
		}
		++mIter ;
	}

	mIter	=	m_vpOffUsers.begin() ;
	while ( mIter != m_vpOffUsers.end() )
	{
		if( strcmp(strUserName, (*mIter)->szName) == 0 )
		{
			int iGPoint = (*mIter)->iPoint ;
			if( m_pGuildArchive && iGPoint != 0 ) 
				m_pGuildArchive->UpdateGuildTotalPoint(-iGPoint) ;
			SAFE_DELETE( (*mIter) );
			m_vpOffUsers.erase(mIter) ;
			return true ;
		}
		++mIter ;
	}

	return false ;
}

/**
@return 길드정보가 있으면 size()
길드정보가 없으면 0
*/
int	SPWindowGuildInfo::GetGuildUserCount(int iMode)
{
	if( GetListAvailable() == false )	return 0 ;
	if( iMode == ON_USER_COUNT )
		return (int)m_vpOnUsers.size() ;
	else if( iMode == OFF_USER_COUNT )
		return (int)m_vpOffUsers.size() ;
	else
		return (int)( m_vpOnUsers.size() + m_vpOffUsers.size() ) ;
}

/**
가입한 길드의 부길마 수 가져오기
@param int iMode : 온라인만, 오프라인만, 전체
@return int : 부길마 수
*/
int SPWindowGuildInfo::GetSubMasterCount(int iMode)
{
	int iRetCnt = 0 ;
	int iOnCnt = 0 ;
	STD_VEC_GUILD::iterator	mIter;

	mIter	=	m_vpSubMaster.begin() ;
	while ( mIter != m_vpSubMaster.end() )
	{
		if( (*mIter)->wServerID <= 0 )
			++iOnCnt ;
		++mIter ;
	}

	if( iMode == ON_USER_COUNT)
		return iOnCnt ;
	else if( iMode == OFF_USER_COUNT )
		return (int)m_vpSubMaster.size() - iOnCnt ;
	else
		return (int)m_vpSubMaster.size() ;
}



/**
길드원 목록을 검사해서 사용자의 Vector Index를 넘겨준다.
*/
bool SPWindowGuildInfo::FindMemberIndex(const char* strUserName, int& iSlot, int& isOn)
{
	int iCount = 0 ;
	STD_VEC_GUILD::iterator	mIter;

	if( strUserName == NULL )		return false ;

	mIter	=	m_vpOnUsers.begin() ;
	while ( mIter != m_vpOnUsers.end() )
	{
		if( strcmp( (*mIter)->szName, strUserName ) == 0 )	{
			iSlot = iCount ;
			isOn  = 1 ;
			return true ;
		}
		++iCount ;		
		++mIter ;
	}

	iCount = 0 ;
	mIter	=	m_vpOffUsers.begin() ;
	while ( mIter != m_vpOffUsers.end() )
	{
		if( strcmp( (*mIter)->szName, strUserName ) == 0 )	{
			iSlot = iCount ;
			isOn  = 0 ;
			return true ;
		}
		++iCount ;
		++mIter ;
	}

	return false ;
}

/**
사용자를 찾아서 해당 포인터를 리턴한다.
*/
PGUILD_MEMBER SPWindowGuildInfo::FindGuildUser(const char* strUserName) 
{
	int iSlot, iOn ;
	if( strUserName == NULL )		return NULL ;
	if( FindMemberIndex(strUserName, iSlot, iOn) == false )
		return NULL ;
	if( iOn )
		return m_vpOnUsers[iSlot] ;
	else
		return m_vpOffUsers[iSlot] ;
}


/**
OffLine 사용자 보임 Option
보임 -> 안보임으로갈때는 Curpage를 재계산 해주어야 함.
*/
bool SPWindowGuildInfo::SetShowOffline(bool bShow)
{
	m_bShowOffline = bShow ;
	if( GetListAvailable() )
	{
		m_iCurPage = CheckCurrentPage(m_iCurPage) ;
		UpdateGuildUserList(m_iCurPage, LIST_UPDATE_CHANGE) ;
		return true ;
	}
	return false ;
}


/**
Online -> Offline일 경우 Page값을 체크해야 한다.
Page가 변경되어야 할 경우 0으로 셋팅한다.
*/
int SPWindowGuildInfo::CheckCurrentPage(int iPage)
{
	if( m_bShowOffline )	
		return iPage ;
	else
		return ( iPage > GetGuildUserCount(ON_USER_COUNT) - MAX_GUILD_LIST_VIEW ) ? 0 : iPage ;
}


/**
컬럼 번호를 Vector Index로 전환
*/
bool SPWindowGuildInfo::PageToIndex(int iPage, bool& bOnline, int& iSlot)
{
	int vSize ;

	int stPage = iPage - GetGuildUserCount(ON_USER_COUNT) ;
	if( stPage >= 0 )	
	{
		vSize = (int)m_vpOffUsers.size() ;
		if( stPage < vSize )
		{
			bOnline = false ;
			iSlot = stPage ;
			return true ;
		}
	}
	else
	{
		iSlot = iPage ;
		bOnline = true ;
		return true ;
	}
	return false ;
}

/**
해당 strUserName의 사용자가 Online인지 여부 확인
@param const char* strName : 캐릭터 명
@return bool : Online 여부
*/
bool SPWindowGuildInfo::IsOnline(const char* strUserName)
{
	if( GetListAvailable() == false )	return false ;
	if( strUserName == NULL)			return false ;

	int iSlot,iOn ;
	if( FindMemberIndex(strUserName, iSlot, iOn) == false )	return false ;

	return ( iOn ) ? true : false ;
}


/**
Guild Master 정보를 보내준다.
*/
PGUILD_MEMBER SPWindowGuildInfo::GetMasterInfo()
{
	if( GetListAvailable() )
	{
		if( (int)m_vpOnUsers.size() > 0 )	{
			if( m_vpOnUsers[0]->byGrade == GUILD_GRADE_01 )
				return m_vpOnUsers[0] ;
		}
		if( (int)m_vpOffUsers.size() > 0 ) {
			if( m_vpOffUsers[0]->byGrade == GUILD_GRADE_01 )
				return m_vpOffUsers[0] ;
		}
	}
	return NULL ;
}




/**
PGUILD_MEMBER 정보를 추가한다.
key = GradeInGuild * 10 + isOnline ;
*/
bool SPWindowGuildInfo::AddGuildUser(PGUILD_MEMBER guildUser)
{
	if( guildUser == NULL) 
	{
		ErrorFLogs->Error_Log("Guild Insert Fail to NULL") ;
		return false ;
	}
	int iAddGrade = (int)guildUser->byGrade ;
	if( iAddGrade < GUILD_GRADE_01 || iAddGrade >= _MAX_GUILD_GRADE )
	{
		ErrorFLogs->Error_Log("GradeInGuild is Out Range [%d]", iAddGrade) ;
		return false ;
	}

	PGUILD_MEMBER pMember = new GUILD_MEMBER ;
	memcpy(pMember, guildUser, sizeof(GUILD_MEMBER)) ;

	int iGPoint = guildUser->iPoint ;
	if( m_pGuildArchive && iGPoint != 0 ) 
		m_pGuildArchive->UpdateGuildTotalPoint(iGPoint) ;
	//ErrorFLogs->Error_Log("AddGuildUser: Name[%s] class[%d] level[%d] server[%d] grade[%d]", 
	//	pMember->szName , (int)pMember->byClass, pMember->wLevel, pMember->wServerID, iAddGrade) ;

	if( pMember->wServerID <= 0)	
	{
		pMember->wServerID = 0 ;
		m_vpOffUsers.push_back(pMember) ;
		//ErrorFLogs->Error_Log("OffInsert Map: [%s], Vec[%d]", guildUser->szName , (int)m_vpOffUsers.size()) ;
	}
	else
	{
		pMember->wServerID = guildUser->wServerID % 100 ;
		m_vpOnUsers.push_back(pMember) ;
		//ErrorFLogs->Error_Log("OnLine Insert Map: [%s], Vec[%d]", guildUser->szName ,(int)m_vpOnUsers.size()) ;
	}

	if( iAddGrade == GUILD_GRADE_02 )
	{
		m_vpSubMaster.push_back(pMember) ;
		//ErrorFLogs->Error_Log("SubMaster Insert Map: [%s, Vec[%d]", guildUser->szName , (int)m_vpSubMaster.size() ) ;
	}

	return true ;
}

/**
부마스터 목록을 재생성 한다.
*/
void SPWindowGuildInfo::RemakeSubMasterList()
{
	STD_VEC_GUILD::iterator	mIter;
	PGUILD_MEMBER	pMember = NULL ;

	m_vpSubMaster.clear() ;

	// Find Sub-master in OnUser
	mIter	=	m_vpOnUsers.begin() ;
	while ( mIter != m_vpOnUsers.end() )
	{
		pMember = (*mIter) ;
		if( pMember->byGrade == GUILD_GRADE_02 )
			m_vpSubMaster.push_back(pMember) ;
		++mIter ;
	}
	// Find Sub-master in OffUser
	mIter	=	m_vpOffUsers.begin() ;
	while ( mIter != m_vpOffUsers.end() )
	{
		pMember = (*mIter) ;
		if( pMember->byGrade == GUILD_GRADE_02 )
			m_vpSubMaster.push_back(pMember) ;
		++mIter ;
	}
}


void SPWindowGuildInfo::DeleteMemberList()
{
	STD_VEC_GUILD::iterator	mIter;

	m_bListInfo = false ;

	// OnUser
	mIter	=	m_vpOnUsers.begin() ;
	while ( mIter != m_vpOnUsers.end() )
	{
		SAFE_DELETE( (*mIter) );
		m_vpOnUsers.erase(mIter) ;
	}
	m_vpOnUsers.clear();

	// OffUser
	mIter	=	m_vpOffUsers.begin() ;
	while ( mIter != m_vpOffUsers.end() )
	{
		SAFE_DELETE( (*mIter) );
		m_vpOffUsers.erase(mIter) ;
	}
	m_vpOffUsers.clear();

	// SubMaster
	m_vpSubMaster.clear();
}



SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowGuildInfo )
//SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER		,	OnCursorEnter	)
//SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT			,	OnCursorOut		)
//SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE		,	OnCursorMove	)
//
//SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP			,	OnLButtonUp		)
//SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN		,	OnLButtonDown	)
//SPIMESSAGE_COMMAND(	SPIM_RBUTTON_UP			,	OnRButtonUp		)

SPIMESSAGE_COMMAND(	SPIM_KEY_RETURN				,	OnInviteKeyReturn	)
SPIMESSAGE_COMMAND(	SPIM_GUILD_INFO_ENABLE		,	OnRecvGuildInfoEnable		)

SPIMESSAGE_COMMAND(	SPIM_GUILD_MEMBER_LIST		,	OnReceiveGuildUserList	)
SPIMESSAGE_COMMAND(	SPIM_GUILD_MEMBER_JOIN		,	OnReceiveGuildUserJoin	)
SPIMESSAGE_COMMAND(	SPIM_GUILD_MEMBER_SECEDE	,	OnReceiveGuildUserSecede)
SPIMESSAGE_COMMAND(	SPIM_GUILD_MEMBER_INFOCHANGE,	OnReceiveChangeGuildUser	)
SPIMESSAGE_COMMAND(	SPIM_GUILD_REFRESH_COMMON	,	OnRecvGuildCommonInfo		)
SPIMESSAGE_COMMAND(	SPIM_GUILD_CROP_SET			,	OnRecvGuildCropSetting		)	// 작물 SET
SPIMESSAGE_COMMAND(	SPIM_GUILD_CROP_UPDATE		,	OnGuildCropUpdate			)	// 작물 UPdate

SPIMESSAGE_COMMAND( SPIM_NOTICE_YES				,	OnYes)
SPIMESSAGE_COMMAND( SPIM_NOTICE_NO				,	OnNo)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_BASE_CLOSE			,	OnClose			)
SPIMESSAGE_CONTROL(	SPIM_SETFOCUS			,	WIID_GUILD_BASE_INVITE_NAME		,	OnEditSetFocus		)
SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS			,	WIID_GUILD_BASE_INVITE_NAME		,	OnEditKillFocus		)
SPIMESSAGE_CONTROL(	SPIM_SETFOCUS			,	WIID_GUILD_LIST_SLOGUN_EDIT		,	OnMEditSetFocus		)
SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS			,	WIID_GUILD_LIST_SLOGUN_EDIT		,	OnMEditKillFocus	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_BASE_INVITE_BTN		,	OnInviteButton	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_MENU_LIST			,	OnGuildListButton	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_MENU_STATUS			,	OnGuildStatusButton	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_MENU_CROPS			,	OnGuildCorpsButton	)

SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE		,	WIID_GUILD_STUS_POINTBG		,	OnToolTipMove	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT		,	WIID_GUILD_STUS_POINTBG		,	OnToolTipOut	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_MOVE	,	WIID_GUILD_USER_LIST	, WIID_GUILD_USER_LIST + 56		,	OnToolTipMove	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_OUT	,	WIID_GUILD_USER_LIST	, WIID_GUILD_USER_LIST + 56		,	OnToolTipOut	)

// List Window
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP	, WIID_GUILD_USER_LIST		, WIID_GUILD_USER_LIST + 56		,	OnGuildListClick	)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP	, WIID_GUILD_LIST_POPUP_DLG	, WIID_GUILD_LIST_POPUP_DLG + 15,	OnPopupClick	)
SPIMESSAGE_CONTROL_RANGE(SPIM_GUILD_RANK_CHANGE	, WIID_GUILD_USER_LIST	, WIID_GUILD_USER_LIST + 56		,	OnShowRankChange	)
SPIMESSAGE_CONTROL_RANGE(SPIM_WHEEL_UP			, WIID_GUILD_USER_LIST	, WIID_GUILD_USER_LIST + 56		,	OnSliderUp		)
SPIMESSAGE_CONTROL_RANGE(SPIM_WHEEL_DOWN		, WIID_GUILD_USER_LIST	, WIID_GUILD_USER_LIST + 56		,	OnSliderDown	)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_LIST_SHOWOFFLINE		,	OnShowOffline	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_LIST_SLOGUNBTN		,	OnSlogunChgBtn	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_LIST_INVITE_PARTY	,	OnInvitePartyButton	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_LIST_WHISPER			,	OnWhisperButton	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_LIST_LEAVE			,	OnLeaveGuildButton	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_LIST_EXPEL			,	OnExpelGuildButton	)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_LIST_SLIDERUP		,	OnSliderUp		)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_LIST_SLIDERDOWN		,	OnSliderDown	)
SPIMESSAGE_CONTROL(	SPIM_WHEEL_UP			,	WIID_GUILD_LIST_PANNEL			,	OnSliderUp		)
SPIMESSAGE_CONTROL(	SPIM_WHEEL_DOWN			,	WIID_GUILD_LIST_PANNEL			,	OnSliderDown	)
SPIMESSAGE_CONTROL( SPIM_SLIDER_UP			,	WIID_GUILD_LIST_SLIDER			,	OnSlider		)
SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN		,	WIID_GUILD_LIST_SLIDER			,	OnSlider		)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP		,	WIID_GUILD_LIST_SLIDER			,	OnSlider		)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN	,	WIID_GUILD_LIST_SLIDER			,	OnSlider		)
SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE		,	WIID_GUILD_LIST_SLIDER			,	OnSlider		)

// Status Window
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP	, WIID_GUILD_STUS_MSTLIST		, WIID_GUILD_STUS_MSTLIST + 15		,	OnMasterListClick	)
SPIMESSAGE_CONTROL_RANGE(SPIM_WHEEL_UP		, WIID_GUILD_STUS_MSTLIST		, WIID_GUILD_STUS_MSTLIST + 15		,	OnSliderUp		)
SPIMESSAGE_CONTROL_RANGE(SPIM_WHEEL_DOWN	, WIID_GUILD_STUS_MSTLIST		, WIID_GUILD_STUS_MSTLIST + 15		,	OnSliderDown	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_STUS_MSTCHANGE				,	OnChangeMasterButton	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_STUS_WHISPER					,	OnWhisperButton	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_STUS_LEAVE					,	OnLeaveGuildButton	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_STUS_MSTLIST_SLIDERUP		,	OnSliderUp		)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_STUS_MSTLIST_SLIDERDOWN		,	OnSliderDown	)
SPIMESSAGE_CONTROL( SPIM_SLIDER_UP			,	WIID_GUILD_STUS_MSTLIST_SLIDER			,	OnSlider		)
SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN		,	WIID_GUILD_STUS_MSTLIST_SLIDER			,	OnSlider		)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP		,	WIID_GUILD_STUS_MSTLIST_SLIDER			,	OnSlider		)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN	,	WIID_GUILD_STUS_MSTLIST_SLIDER			,	OnSlider		)
SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE		,	WIID_GUILD_STUS_MSTLIST_SLIDER			,	OnSlider		)
SPIMESSAGE_CONTROL(	SPIM_WHEEL_UP			,	WIID_GUILD_STATUS_PANNEL				,	OnSliderUp		)
SPIMESSAGE_CONTROL(	SPIM_WHEEL_DOWN			,	WIID_GUILD_STATUS_PANNEL				,	OnSliderDown	)
SPIMESSAGE_CONTROL(	SPIM_CURSOR_ENTER		,	WIID_GUILD_STUS_LEVEL			,	OnLevelCursorEnter	)
SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT			,	WIID_GUILD_STUS_LEVEL			,	OnLevelCursorOut	)

// Crops Window
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_GUILD_CROPS_LEAVE					,	OnLeaveGuildButton	)
SPIMESSAGE_CONTROL(	SPIM_CURSOR_ENTER,		WIID_GUILD_CROP_REGISTRY_AREA, 				OnCropCursorEnter )
SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT,		WIID_GUILD_CROP_REGISTRY_AREA, 				OnCropCursorOut )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_GUILD_CROP_REGISTRY_AREA, 				OnCropLButtonUp )

// Popup Close
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP	, WIID_GUILD_BASE_TITLE		, WIID_GUILD_STUS_MSTLIST 	,	OnClickTest	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)




/**
길드 초대 버튼
1) 길드 초대 권한 확인
2) 초대자 ID 길이 체크
3) 길드 초대 Send
*/
int SPWindowGuildInfo::OnInviteKeyReturn( WPARAM wParam, LPARAM lParam )
{
	if( m_pEditInviteName->IsFocus() == TRUE )
	{
		OnInviteButton(0, 0) ;
		return 1;
	}
	return -1;
}
int SPWindowGuildInfo::OnInviteButton( WPARAM wParam, LPARAM lParam )
{
	//ErrorFLogs->Error_Log("Request Invite Guild") ;
	std::string strName ;
	strName = m_pEditInviteName->GetWindowText() ;
	if( strName.length() < MIN_LEN_NAME )		return -1 ;

	if( m_pGuildArchive )
	{
		if( m_pGuildArchive->SendGuildInvite(strName.c_str()) )
		{
			m_pEditInviteName->SetWindowText("") ;
			// Send후 일정 시간동안 초대 delay
			return 1 ;
		}
	}

	return -1;
}


/**
닫기 버튼
*/
int SPWindowGuildInfo::OnClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}


//////////////////////////////////////////////////////////////////////////
//
//	메뉴 버튼
//
//////////////////////////////////////////////////////////////////////////
/**
[길드 목록] 버튼
1) 길드 목록 정보 Clear
2) 길드 목록 정보 요청
3) 길드 목록 정보 Update
*/
int	SPWindowGuildInfo::OnGuildListButton	( WPARAM wParam, LPARAM lParam )
{
	SPWindowButton* pCheckButton ;
	m_iCurMenu			= GUILD_MENU_LIST ;
	//ErrorFLogs->Error_Log("Request Guild List") ;

	m_iCurSelected		= 0 ;
	m_iCurSelecteIndex	= 0 ;

	SetShowGuildUserList(GUILD_MEMBER_USER, NULL) ;
	g_pGOBManager->GetGuildArchive()->SendGuildCommonInfo() ;
	SendReqGuildMemberList() ;

	pCheckButton = (SPWindowButton*)Find(WIID_GUILD_MENU_STATUS, true);
	if( pCheckButton )	pCheckButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	m_pGuildStatusWnd->Hide() ;
	pCheckButton = NULL ;
	pCheckButton = (SPWindowButton*)Find(WIID_GUILD_MENU_CROPS, true);
	if( pCheckButton )	pCheckButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	m_pGuildCorpsWnd->Hide() ;
	pCheckButton = (SPWindowButton*)Find(WIID_GUILD_CROPS_LEAVE, true) ;
	if( pCheckButton )	pCheckButton->Hide() ;
	ShowNoCrops(false) ;
	pCheckButton = NULL ;
	//CleanCropMonster() ;

	pCheckButton = (SPWindowButton*)Find(WIID_GUILD_MENU_LIST, true);
	if( pCheckButton )	pCheckButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);

	pCheckButton = (SPWindowButton*)Find(WIID_GUILD_LIST_SHOWOFFLINE, true);
	pCheckButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
	//if( m_pGuildArchive )	SetShowOffline( true ) ;

	m_pGuildListWnd->Hide() ;
	// Show [Receive Guild Member List...]

	//UpdateRankControl() ;		// 권한별 컨트롤 Enable
	ShowRankPopup(false) ;		// 팝업 처리


	return 1 ;
}
/**
[길드 상태] 버튼
1) Master 정보 설정
2) SubMaster List 설정
*/
int	SPWindowGuildInfo::OnGuildStatusButton	( WPARAM wParam, LPARAM lParam )
{
	SPWindowButton* pCheckButton ;
	m_iCurMenu			= GUILD_MENU_STATUS ;
	//ErrorFLogs->Error_Log("Request Guild Status") ;

	pCheckButton = (SPWindowButton*)Find(WIID_GUILD_MENU_LIST, true);
	if( pCheckButton )	pCheckButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	m_pGuildListWnd->Hide() ;
	pCheckButton = NULL ;
	pCheckButton = (SPWindowButton*)Find(WIID_GUILD_MENU_CROPS, true);
	if( pCheckButton )	pCheckButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	m_pGuildCorpsWnd->Hide() ;
	pCheckButton = (SPWindowButton*)Find(WIID_GUILD_CROPS_LEAVE, true) ;
	if( pCheckButton )	pCheckButton->Hide() ;
	ShowNoCrops(false) ;
	pCheckButton = NULL ;
	//CleanCropMonster() ;

	pCheckButton = (SPWindowButton*)Find(WIID_GUILD_MENU_STATUS, true);
	if( pCheckButton )	pCheckButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
	m_pGuildStatusWnd->Show() ;

	UpdateStatusInfo() ;		// 길드 정보
	UpdateRankControl() ;		// 권한 설정

	m_iCurSelected = -1 ;
	m_iCurSelecteIndex = -1 ;
	UpdateGuildMasterList(0, LIST_UPDATE_USERLIST) ;

	return 1 ;
}
/**
[길드 작물] 버튼
*/
int	SPWindowGuildInfo::OnGuildCorpsButton	( WPARAM wParam, LPARAM lParam )
{
	SPWindow* pWindow ;
	SPWindowButton* pCheckButton ;
	m_iCurMenu			= GUILD_MENU_CROPS ;

	pCheckButton = (SPWindowButton*)Find(WIID_GUILD_MENU_LIST, true);
	if( pCheckButton )	pCheckButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	m_pGuildListWnd->Hide() ;
	pCheckButton = NULL ;
	pCheckButton = (SPWindowButton*)Find(WIID_GUILD_MENU_STATUS, true);
	if( pCheckButton )	pCheckButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	m_pGuildStatusWnd->Hide() ;
	pCheckButton = NULL ;

	pCheckButton = (SPWindowButton*)Find(WIID_GUILD_MENU_CROPS, true);
	if( pCheckButton )	pCheckButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
	pWindow = Find(WIID_GUILD_CROPS_LEAVE, true) ;
	if( pWindow )	pWindow->Show() ;

	if( m_pGuildArchive->hasCorps() )	{	// 작물이 있는지 여부에 따른 화면
		UpdateCropInfo(true) ;
		m_pGuildCorpsWnd->Show() ;
		ShowNoCrops(false) ;
	}
	else	{
		ShowNoCrops(true) ;
		//CleanCropMonster() ;
	}

	UpdateRankControl() ;		// 권한 설정


	ErrorFLogs->Error_Log("Request Guild Plant") ;
	return 1 ;
}


//////////////////////////////////////////////////////////////////////////
//
//	List Window Message 처리
//
//////////////////////////////////////////////////////////////////////////
/**
SPGuildArchive 가 사용할수 있도록 정보가 설정되면
Property에 따른 PopWindow를 설정한다.
*/
int SPWindowGuildInfo::OnRecvGuildInfoEnable( WPARAM wParam , LPARAM lParam )
{
	m_pGuildArchive = g_pGOBManager->GetGuildArchive() ;
	m_iGuildProperty = m_pGuildArchive->GetGuildProperty() ;

	UpdatePopupWidthProperty( m_pGuildArchive->GetGuildProperty()) ;
	UpdateRankControl() ;	// 권한별 컨트롤 Enable
	SetGuildLevelTooltip() ;

	return 1 ;
}

/**
Scroll이 업데이트 되어야 하는 경우 받는다.
@param WPARAM : Total Guild User Count
@param LPARAM : Current Show Page (Current top)
*/
//int SPWindowGuildInfo::OnRecvGuildRefreshScroll( WPARAM wParam , LPARAM lParam )
void SPWindowGuildInfo::UpdateListScroll(int iTotalCount ,int iShowPage)
{
	int iScrollCount ;

	if( m_iCurMenu == GUILD_MENU_LIST ) 
	{
		if( m_pUserListSlider)	
		{
			if( iTotalCount <= MAX_GUILD_LIST_VIEW )		
			{
				m_bShowSlide = false ;
				m_pUserListSlider->SPSendMessage( SPIM_SET_MAXVALUE , 1 );
				m_pUserListSlider->SPSendMessage( SPIM_SET_CURVALUE , iShowPage);
			}
			else
			{
				m_bShowSlide = true ;
				iScrollCount = iTotalCount - MAX_GUILD_LIST_VIEW + 1  ;
				m_pUserListSlider->SPSendMessage( SPIM_SET_MAXVALUE , iScrollCount );
				m_pUserListSlider->SPSendMessage( SPIM_SET_CURVALUE , iShowPage);
			}
		}
	}
	else if( m_iCurMenu == GUILD_MENU_STATUS ) 
	{
		if( m_pMasterListSlider)	
		{
			if( iTotalCount <= MAX_MASTER_LIST_VIEW )	
			{
				m_bShowMSTSlide = false ;
				m_pUserListSlider->SPSendMessage( SPIM_SET_MAXVALUE , 1 );
				m_pUserListSlider->SPSendMessage( SPIM_SET_CURVALUE , iShowPage);
			}
			else
			{
				m_bShowMSTSlide = true ;
				iScrollCount = iTotalCount - MAX_MASTER_LIST_VIEW + 1 ;
				m_pMasterListSlider->SPSendMessage( SPIM_SET_MAXVALUE , iScrollCount );
				m_pMasterListSlider->SPSendMessage( SPIM_SET_CURVALUE , iShowPage);
			}
		}
	}
}

/**
Guild 사용자 목록을 받은경우 처리
@param WPARAM : 보여줄 갯수
@param LPARAM : std::vector<PGUILD_MEMBER>* 길드 목록
*/
void SPWindowGuildInfo::SetShowGuildUserList( int iCurList, std::vector<PGUILD_MEMBER>* vpShowMemver )
{
	int iShowCount = 0 ;
	if( vpShowMemver != 0 )
		iShowCount = (int)vpShowMemver->size() ;

	if( iCurList == GUILD_MEMBER_USER ) 
	{
		for( int i = 0; i < MAX_GUILD_LIST_VIEW; i++)
		{
			m_pGuildMember[i]->SetWndState(WND_STATE_NORMAL) ;
			if( i < iShowCount )
			{
				m_pGuildMember[i]->SetGuildInfo(i, m_iGuildProperty, (*vpShowMemver)[i] ) ;
				m_pGuildMember[i]->SetShowEnable(true) ;
				m_pGuildMember[i]->Show() ;
				if( m_pGuildArchive->GetRankChangable() )			// Master Enable button
					m_pGuildMember[i]->SetSelButton(true) ;
				else
					m_pGuildMember[i]->SetSelButton(false) ;
			}
			else
			{
				m_pGuildMember[i]->SetShowEnable(false) ;
				m_pGuildMember[i]->Hide() ;
				m_pGuildMember[i]->SetSelButton(false) ;
			}
		}
		m_iCurSelected = m_iCurSelecteIndex - m_pUserListSlider->GetCurPage() ;
		if( m_iCurSelected > -1 && m_iCurSelected < MAX_GUILD_LIST_VIEW)	{
			m_pGuildMember[m_iCurSelected]->SetWndState(WND_STATE_CLICK) ;
		}

		m_pGuildListWnd->Show() ;	// 창 보여주고 나서..
		UpdateRankControl() ;		// 권한별 컨트롤 Enable
		UpdateSlogun() ;			// 슬로건 업데이트
		ShowSlider(m_bShowSlide) ;
	}
	else if( iCurList == GUILD_MEMBER_MASTER ) 
	{
		SetShowMasterList(iShowCount, vpShowMemver) ;
		ShowSlider(m_bShowMSTSlide) ;
	}
}


int	SPWindowGuildInfo::OnRecvGuildCommonInfo( WPARAM wParam , LPARAM lParam )
{
	if( !IsShow() )		return 0 ;
	if( m_iCurMenu == GUILD_MENU_LIST) 
		UpdateSlogun() ;
	if( m_iCurMenu == GUILD_MENU_STATUS) 
		UpdateStatusInfo() ;

	return 1 ;
}


/**
작물 정보 및 이미지 업데이트
*/
int	SPWindowGuildInfo::OnRecvGuildCropSetting( WPARAM wParam , LPARAM lParam )
{
	if( !IsShow() )		return 0 ;
	if( m_iCurMenu == GUILD_MENU_CROPS) 
	{
		if( m_pGuildArchive->hasCorps() )	{	// 작물이 있는지 여부에 따른 화면
			UpdateCropInfo(true) ;
			m_pGuildCorpsWnd->Show() ;
			ShowNoCrops(false) ;
		}
		else	{
			UpdateCropInfo(false) ;
			ShowNoCrops(true) ;
		}

		UpdateRankControl() ;		// 권한 설정
	}

	return 1 ;
}


/**
작물 정보 및 이미지 업데이트
*/
int	SPWindowGuildInfo::OnGuildCropUpdate( WPARAM wParam , LPARAM lParam )
{
	if( !IsShow() )		return 0 ;
	if( m_iCurMenu == GUILD_MENU_CROPS && m_pGuildArchive->hasCorps() ) 
	{
		if( wParam == 1 )		UpdateCropTime() ;
		if( wParam == 2 )		UpdateCropInfo(true) ;		// Text & Image Update
	}
	return 1 ;
}

/**
길드 User List Click
1) 이전 선택을 찾아서 되돌린다
2) 현재 선택된 Index를 설정한다.
3) PopWindow를 닫는다.
*/
int SPWindowGuildInfo::OnGuildListClick( unsigned int iID, WPARAM wParam , LPARAM lParam  )
{
	if( m_iCurSelected > -1 && m_iCurSelected < MAX_GUILD_LIST_VIEW )
	{
		m_pGuildMember[m_iCurSelected]->SetWndState(WND_STATE_NORMAL) ;
	}

	for( int i = 0; i < MAX_GUILD_LIST_VIEW; i++ )
	{
		if( m_pGuildMember[i]->IsShow() == false )
			continue ;
		if( m_pGuildMember[i]->GetInstanceID() == iID )
		{
			m_iCurSelected = i ;
			m_iCurSelecteIndex = i + m_pUserListSlider->GetCurPage() ;
		}
	}
	ShowRankPopup(false) ;

	return 1;
}

/**
작위설정 버튼이 선택된 경우
1) 리스트에서 현재 선택된 컨트롤을 찾는다.
2) 현재 작위를 설정한다.
2) 하단에 팝업을 출력한다.
*/
int	SPWindowGuildInfo::OnShowRankChange	( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	for(int i = 0; i < MAX_GUILD_GRADE_VIEW; i++ )
	{
		m_pGuildPopupList[i]->SetWndState(WND_STATE_NORMAL) ;
	}

	m_iPreSelected = m_iCurSelected ;
	//if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_SANDA )
	//		m_pGuildGradePopup->AdjustCoord(264, m_iCurSelected * 16 + 104) ;
	//else
	//	m_pGuildGradePopup->AdjustCoord(348, m_iCurSelected * 17 + 104) ;
	m_pGuildGradePopup->AdjustCoord(348, m_iCurSelected * 17 + 104) ;
	m_pGuildGradePopup->RefreshRelationCoord() ;

	ShowRankPopup(true) ;
	return 1 ;
}



/**
[작위 변경]작위 팝업 정보창에서 클릭을 한경우
*/
int	SPWindowGuildInfo::OnPopupClick		( unsigned int iID, WPARAM wParam , LPARAM lParam  )
{
	int i ;
	for( i = 0; i < MAX_GUILD_GRADE_VIEW; i++)
		m_pGuildPopupList[i]->SetWndState(WND_STATE_NORMAL) ;		// UnSelect

	for( i = 0; i < MAX_GUILD_GRADE_VIEW; i++ )
	{
		if( m_pGuildPopupList[i]->IsShow() == false )
			continue ;
		if( m_pGuildPopupList[i]->GetInstanceID() == iID )
		{
			m_iCurPopupSelect = i ;
			if( m_pGuildArchive )	{
				if( m_pGuildMember[m_iCurSelected]->GetGrade() <= m_pGuildArchive->GetGuildGrade() )
				{
					if( g_pResourceManager->GetGlobalString(27000006) )
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000006) );
					ShowRankPopup(false) ;
					return 0 ;
				}

				if( m_pGuildMember[m_iCurSelected]->GetGrade() != (m_iCurPopupSelect + GUILD_GRADE_02) )	// Index + Grade Base
					SendChangeGrade( m_pGuildMember[m_iCurSelected]->GetName(),  m_iCurPopupSelect+GUILD_GRADE_02) ;
			}
			ShowRankPopup(false) ;
			return 1 ;
		}
	}
	return 0 ;
}




/**
[OffLine 표시] 버튼
1) Archive에 목록 요청
2) 목록 정보 갱신
*/
int	SPWindowGuildInfo::OnShowOffline	( WPARAM wParam, LPARAM lParam )
{
	m_iCurMenu			= GUILD_MENU_LIST ;
	if( m_pGuildArchive == NULL )		return 0 ;
	bool bToggle = (bool)lParam ;
	bool bSetOffline = false ;

	if( bToggle )
		bSetOffline = !GetShowOffline() ;
	else
		bSetOffline = GetShowOffline() ;

	SPWindowButton* pCheckButton = (SPWindowButton*)Find(WIID_GUILD_LIST_SHOWOFFLINE, true);
	pCheckButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)bSetOffline );
	SetShowOffline( bSetOffline ) ;

	return 1 ;
}


/**
[슬로건 변경] 버튼
*/
int SPWindowGuildInfo::OnSlogunChgBtn( WPARAM wParam, LPARAM lParam )
{
	SPWindowEditMultiLine* pEdit = (SPWindowEditMultiLine*)Find(WIID_GUILD_LIST_SLOGUN_EDIT, true) ;
	if( m_pGuildArchive->GetSlogunChangable() == false )	return 0 ;
	if( pEdit )
		m_pGuildArchive->ChangeSlogun(0, pEdit->GetWindowText()) ;

	return 1 ;
}



/**
[파티 초대] 버튼
*/
int SPWindowGuildInfo::OnInvitePartyButton( WPARAM wParam, LPARAM lParam )
{
	//ErrorFLogs->Error_Log("Invite Party Button Pressed") ;
	if( m_iCurPopupSelect < 0 || m_iCurSelected >= MAX_GUILD_LIST_VIEW )
		return 0 ;
	if( m_pGuildMember[m_iCurSelected]->GetName() == NULL )
		return 0 ;

	std::string	strName		=	m_pGuildMember[m_iCurSelected]->GetName();
	SPPlayer* pkLocalPlayer =	g_pGOBManager->GetLocalPlayer();
	if( g_pNetworkManager->GetCharName() == strName.c_str() )	return 0 ;		// Self X
	if( !IsOnline(strName.c_str()) )							return 0 ;		// Offline X

	if( pkLocalPlayer != NULL && strName.empty() == FALSE )
		pkLocalPlayer->SPGOBPerformMessage( SPGM_TEAM_INVITE_REQ , (LPARAM)strName.c_str() );

	return 1;
}

/**
[귓속말] 버튼
*/
int SPWindowGuildInfo::OnWhisperButton( WPARAM wParam, LPARAM lParam )
{
	std::string	strName ;

	if( m_iCurMenu == GUILD_MENU_LIST ) 
	{
		if( m_iCurSelected < 0 || m_iCurSelected >= MAX_GUILD_LIST_VIEW )		// Not Selected
			return 0 ;
		strName = m_pGuildMember[m_iCurSelected]->GetName() ;
		if( strName.empty()	)	// Name Empty
			return 0 ;
	}
	else if( m_iCurMenu == GUILD_MENU_STATUS ) 
	{
		if( m_iCurSelected < 0 || m_iCurSelected >= MAX_MASTER_LIST_VIEW )		// Not Selected
			return 0 ;
		strName = m_pGuildSubMaster[m_iCurSelected]->GetText() ;
		if( strName.empty()	)	// Name Empty
			return 0 ;
	}

	if( g_pNetworkManager->GetCharName() == strName.c_str() )	return 0 ;		// Self X
	if( !IsOnline(strName.c_str()) )							return 0 ;		// Offline X

	if( strName.empty() == FALSE )
	{
		OnEndMove( NULL , NULL );
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_INSERT_MSGEDIT , CHAT_STRING_WHISPER , (LPARAM)strName.c_str() );
	}

	return 1;
}

/**
[길드 탈퇴] 버튼
*/
int SPWindowGuildInfo::OnLeaveGuildButton( WPARAM wParam, LPARAM lParam )
{
	if( m_pGuildArchive )
	{
		if( m_pGuildArchive->IsMaster() )
		{
			if( g_pResourceManager->GetGlobalString(27000010) )
				g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(27000010), this, NOTICE_TYPE_YES);
			else
				g_pInterfaceManager->SetNotice("Need GlobalString(27000010)", this, NOTICE_TYPE_YES);
		}
		else
		{
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GUILD_SECEDE_ME, (LPARAM)0);
		}
	}

	return 1;
}


/**
[길드 강퇴]
1) 마스터만 가능
2) 선택후 강퇴 버튼시 적용
*/
int SPWindowGuildInfo::OnExpelGuildButton( WPARAM wParam, LPARAM lParam )
{
	TCHAR szMessage[256] ;
	if( m_pGuildArchive->GetExpelable() )
	{
		if( m_iCurSelected < 0 || m_iCurSelected >= MAX_GUILD_LIST_VIEW )
			return 0 ;
		if( m_pGuildMember[m_iCurSelected]->GetName() == NULL )
			return 0 ;

		if( m_pGuildMember[m_iCurSelected]->GetGrade() <= m_pGuildArchive->GetGuildGrade() )
		{
			if( g_pResourceManager->GetGlobalString(27000006) )
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000006) );
			return 0 ;
		}

		m_strExpelName = m_pGuildMember[m_iCurSelected]->GetName() ;
		if( m_strExpelName == g_pNetworkManager->GetCharName() )	return 0 ;

		m_iNoticeState = GUILD_EXPEL_NOTICE ;
		if( g_pResourceManager->GetGlobalString(27000061) )
			wsprintf( szMessage, g_pResourceManager->GetGlobalString(27000061), m_strExpelName.c_str()) ;
		else
			wsprintf( szMessage, "Need GlobalString(27000061)") ;
		g_pInterfaceManager->SetNotice(szMessage, this) ;

	}
	return 0 ;
}

//////////////////////////////////////////////////////////////////////////
//
//	Status 창 Message
//
//////////////////////////////////////////////////////////////////////////
/**
[권한 위임]버튼이 클릭된 경우
*/
int	SPWindowGuildInfo::OnChangeMasterButton	( WPARAM wParam , LPARAM lParam )
{
	TCHAR szMessage[256] ;
	if( m_iCurSelected < 0 || m_iCurSelected >= MAX_GUILD_LIST_VIEW )	return 0 ;
	if( !m_pGuildSubMaster[m_iCurSelected]->IsShow() )					return 0 ;

	if( m_pGuildSubMaster[m_iCurSelected]->GetText() != NULL )
	{

		if( m_pGuildSubMaster[m_iCurSelected]->GetText() == NULL )		{
			ErrorFLogs->Error_Log("Master Select NULL[%d]", m_iCurSelected) ;
			return 1;
		}
		else 	{
			m_strChangeMasterName = m_pGuildSubMaster[m_iCurSelected]->GetText() ;
			ErrorFLogs->Error_Log("Master Change Select[%d][%s]", m_iCurSelected, m_pGuildSubMaster[m_iCurSelected]->GetText()) ;
		}

		m_iNoticeState = GUILD_MSTCHANGE_NOTICE ;
		if( g_pResourceManager->GetGlobalString(27000083) )
		{
			wsprintf( szMessage, g_pResourceManager->GetGlobalString(27000083), m_strExpelName.c_str()) ;
			g_pInterfaceManager->SetNotice(szMessage, this) ;
		}
		else
		{
			// GlobalString이 없으면 바로 위임이 되도록 한다
			// For China
			SendChangeManster( m_strChangeMasterName.c_str()) ;
			m_strChangeMasterName.clear() ;
		}
	}
	return 1 ;
}

/**
길드 Master List Click
1) 이전 선택을 찾아서 되돌린다
2) 현재 선택된 Index를 설정한다.
*/
int SPWindowGuildInfo::OnMasterListClick( unsigned int iID, WPARAM wParam , LPARAM lParam  )
{
	if( m_iCurSelected > -1 && m_iCurSelected < MAX_MASTER_LIST_VIEW )
	{
		m_pGuildSubMaster[m_iCurSelected]->SetWndState(WND_STATE_NORMAL) ;
	}

	for( int i = 0; i < MAX_MASTER_LIST_VIEW; i++ )
	{
		if( m_pGuildSubMaster[i]->IsShow() == false )
			continue ;
		if( m_pGuildSubMaster[i]->GetInstanceID() == iID )
		{
			m_iCurSelected = i ;
			m_iCurSelecteIndex = i + m_pMasterListSlider->GetCurPage() ;
		}
	}

	return 1;
}

int SPWindowGuildInfo::OnLevelCursorEnter( WPARAM wParam, LPARAM lParam )
{
	if( m_pLevelTooltip == NULL )
		return 0 ;

	m_pLevelTooltip->SetShowEnable(true) ;
	m_pLevelTooltip->Show() ;
	m_bShowAbilToolTip = true ;
	return 1;
}

int SPWindowGuildInfo::OnLevelCursorOut( WPARAM wParam, LPARAM lParam )
{
	if( m_pLevelTooltip == NULL )
		return 0 ;

	m_pLevelTooltip->SetShowEnable(false) ;
	m_pLevelTooltip->Hide() ;
	m_bShowAbilToolTip = false ;
	return 1;
}


//////////////////////////////////////////////////////////////////////////
//
//	Crop Drag&Drop
//
int SPWindowGuildInfo::OnCropCursorEnter( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowGuildInfo::OnCropCursorOut( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

/**
Drag&Drop된 아이템을 찾아서 Crop.LDT에서 CropID를 찾아서 심는다.
*/
int SPWindowGuildInfo::OnCropLButtonUp( WPARAM wParam, LPARAM lParam )
{
	int iRet = 0 ;
	if( m_pGuildArchive->hasCorps() )		return 0 ;

	SPItem* pDragItem = NULL ;
	pDragItem = GetDragItem() ;
	if( pDragItem != NULL )
	{
		if( (iRet = m_pGuildArchive->UseCropItem(pDragItem)) != 0 )
		{
			if( iRet == 4 ) {
				if( g_pResourceManager->GetGlobalString(27000067) )
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000067) );
			}
		}
	}

	return 1;
}


//////////////////////////////////////////////////////////////////////////
//
//	Slider 관련 처리
//
//////////////////////////////////////////////////////////////////////////
/**
Slider Up / Down 처리 // 주변 휠
// 슬라이드 상, 하 버튼
*/
int SPWindowGuildInfo::OnSliderUp( WPARAM wParam, LPARAM lParam )
{
	if( m_iCurMenu == GUILD_MENU_LIST ) {
		if( !m_pUserListSlider->IsShow() )		return 0 ;
		m_pUserListSlider->SPSendMessage( SPIM_SLIDER_UP );
	}
	else if( m_iCurMenu == GUILD_MENU_STATUS ) {
		if( !m_pMasterListSlider->IsShow() )		return 0 ;
		m_pMasterListSlider->SPSendMessage( SPIM_SLIDER_UP );
	}

	return 1;
}

int SPWindowGuildInfo::OnSliderDown( WPARAM wParam, LPARAM lParam )
{
	if( m_iCurMenu == GUILD_MENU_LIST ) 
	{
		if( !m_pUserListSlider->IsShow() )		return 0 ;
		m_pUserListSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	}
	else if( m_iCurMenu == GUILD_MENU_STATUS ) 
	{
		if( !m_pMasterListSlider->IsShow() )		return 0 ;
		m_pMasterListSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	}

	return 1;
}

int SPWindowGuildInfo::OnSliderDown( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	// List내부 컨트롤 휠 처리
	if( m_iCurMenu == GUILD_MENU_LIST ) 
	{
		if( !m_pUserListSlider->IsShow() )		return 0 ;
		m_pUserListSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	}
	else if( m_iCurMenu == GUILD_MENU_STATUS ) 
	{
		if( !m_pMasterListSlider->IsShow() )		return 0 ;
		m_pMasterListSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	}
	return 1;
}

int SPWindowGuildInfo::OnSliderUp( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	if( m_iCurMenu == GUILD_MENU_LIST ) {
		if( !m_pUserListSlider->IsShow() )		return 0 ;
		m_pUserListSlider->SPSendMessage( SPIM_SLIDER_UP );
	}
	else if( m_iCurMenu == GUILD_MENU_STATUS ) {
		if( !m_pMasterListSlider->IsShow() )		return 0 ;
		m_pMasterListSlider->SPSendMessage( SPIM_SLIDER_UP );
	}
	return 1;
}

/**
스크롤 이후 새로운 Top Index 를 가지고 List를 업데이트 한다.
// 슬라이드 Thumb 클릭 & 스크롤
*/
int SPWindowGuildInfo::OnSlider( WPARAM wParam, LPARAM lParam )
{
	int	iNewPage	=	(int)wParam;
	//ErrorFLogs->Error_Log("SPWindowGuildInfo::OnSlider iNewPage[%d]", iNewPage) ;

	if( m_iCurMenu == GUILD_MENU_LIST )
	{
		if( GetListAvailable() )
			UpdateGuildUserList(iNewPage, LIST_UPDATE_SCROLL) ;
	}
	else if( m_iCurMenu == GUILD_MENU_STATUS )
	{
		// Master List 갱신
		if( GetListAvailable() )
			UpdateGuildMasterList(iNewPage, LIST_UPDATE_SCROLL) ;
	}

	return 1;
}

/**
Edit Window에 포커스를 가지지 않으면 단축키가 발동된다.
*/
int SPWindowGuildInfo::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}

int SPWindowGuildInfo::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}
/**
Edit Window에 포커스를 가지지 않으면 단축키가 발동된다.
*/
int SPWindowGuildInfo::OnMEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}

int SPWindowGuildInfo::OnMEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}


int SPWindowGuildInfo::OnToolTipMove( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	if( wParam )
		OnToolTipMove(wParam, lParam) ;
	return 1 ;
}
int SPWindowGuildInfo::OnToolTipOut( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	if( wParam == 1 )
		OnToolTipOut((WPARAM)wParam, lParam) ;
	return 1 ;
}

int SPWindowGuildInfo::OnToolTipMove( WPARAM wParam, LPARAM lParam )
{
	SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();
	TOOLTIP_NORMAL_DATA stToolTipData ;
	TCHAR szPoint[64] ;

	if( pToolTipWindow != NULL )
	{
		TCHAR szMyPoint[32] ;
		TCHAR szTotalPoint[32] ;
		TCHAR szNextPoint[32] ;
		int iTotalPoint = m_pGuildArchive->GetGuildTotalPoint() ;
		int iNextPoint  = m_pGuildArchive->GetGuildNextPoint() ;
		stToolTipData.m_iContentsCount = 1 ;
		if( wParam )	{
			PGUILD_MEMBER pMember = (PGUILD_MEMBER)wParam ;
			stToolTipData.m_strName = g_pResourceManager->GetGlobalString(27000102) ? g_pResourceManager->GetGlobalString(27000102) : "Giving Point";	// 기여포인트
			strcpy(szMyPoint, ConvertMoneyToString(pMember->iPoint)) ;
			strcpy(szTotalPoint, ConvertMoneyToString(iTotalPoint)) ;
			_stprintf(szPoint, "%s/%s", szMyPoint, szTotalPoint) ;
		}
		else	{
			strcpy(szTotalPoint, ConvertMoneyToString(iTotalPoint)) ;
			strcpy(szNextPoint, ConvertMoneyToString(iNextPoint)) ;
			stToolTipData.m_strName = g_pResourceManager->GetGlobalString(27000101) ? g_pResourceManager->GetGlobalString(27000101) : "Current Point";	// 현재포인트
			_stprintf(szPoint, "%s/%s", szTotalPoint, szNextPoint) ;
		}
		stToolTipData.m_cName = D3DXCOLOR(0.172f, 0.56f, 0.207f, 1.0f) ;

		stToolTipData.m_strContents[0] = szPoint ;
		stToolTipData.m_cContents[0] = D3DXCOLOR(0.192f, 0.2980f, 0.38f, 1.0f) ;
		pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)(&stToolTipData), SPWindowToolTip::TOOLTIP_SHOW_TYPE_NORMAL );
	}

	return 1;
}

int SPWindowGuildInfo::OnToolTipOut( WPARAM wParam, LPARAM lParam )
{
	SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();

	if( pToolTipWindow != NULL )
		pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL , SPWindowToolTip::TOOLTIP_SHOW_TYPE_NORMAL );

	return 1;
}
//////////////////////////////////////////////////////////////////////////
//
int SPWindowGuildInfo::OnYes( WPARAM wParam, LPARAM lParam )
{
	//////////////////////////////////////////////////////////////////////////
	// 현재는 강퇴에서만 사용됨 -> 추후 필요에 따라 나눌것
	if( m_iNoticeState == GUILD_EXPEL_NOTICE )
	{
		if( m_strExpelName.empty() )		return 1 ;
		SendGuildExpel(m_strExpelName.c_str()) ;
		m_strExpelName.clear() ;
	}
	else if ( m_iNoticeState == GUILD_MSTCHANGE_NOTICE )
	{
		if( !m_strChangeMasterName.empty() )
		{
			SendChangeManster( m_strChangeMasterName.c_str()) ;
			m_strChangeMasterName.clear() ;
		}
	}

	return 1;
}

int SPWindowGuildInfo::OnNo( WPARAM wParam, LPARAM lParam )
{
	m_strExpelName.clear() ;
	m_strChangeMasterName.clear() ;
	m_iNoticeState = GUILD_NOTICE_EMPTY ;
	return 1;
}


/**
창의 다른 부분이 클릭되면 PopWindow를 Hide 시킨다.
*/
int SPWindowGuildInfo::OnClickTest( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	ShowRankPopup(false) ;
	return 1;
	//TCHAR szError[64] ;
	//wsprintf(szError, "[Guild]Destroy Fail[%d]", ErrorID) ;
	//g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szError );
}