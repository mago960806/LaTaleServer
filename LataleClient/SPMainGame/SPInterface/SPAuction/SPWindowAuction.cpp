// Copyright (C) DURAGON 
//********************************************************************
//	filename: 	SPWindowAuction.cpp
//	created:	2008/2/26   Time:10:59
//	purpose:	
//*********************************************************************/


#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindowListDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowList.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPGameObjectDEF.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPGOBCluster.h"
#include "SPMOBCluster.h"
#include "SPGOBModelUnit.h"
#include "SPGOBStatus.h"
#include "SPMonsterModelUnit.h"
#include "SPGOBManager.h"
#include "SPPlayer.h"
#include "SPPlayerInvenArchive.h"

#include "SPEventDEF.h"			// for close
#include "SPEventManager.h"

#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPAuctionDef.h"
#include "SPAuctionAttr.h"
#include "SPAuctionArchive.h"
#include "SPAuctionManager.h"

#include "SPLocalizeManager.h"

#include "SPTimer.h"
#include "_Error_log.h"

#include "SPWindowAuction.h"


SPWindowAuction::SPWindowAuction( WND_ID WndClassID , INSTANCE_ID InstanceID )
: SPWindow( WndClassID , InstanceID )
, m_pWindowSearch(NULL)
, m_pWindowSearchPopup(NULL)
, m_pWindowBuy(NULL)
, m_pWindowSale(NULL)
, m_pWindowSaleTip(NULL)
, m_pTextureBG(NULL)
, m_pSaleTypeInfo(NULL)
, m_pAuctionMsg(NULL)
, m_pSaleTimer(NULL)
, m_pSaleTipTimer(NULL)
, m_pSaleItem(NULL)
, m_pSearchTimer(NULL)
, m_pSearchButton(NULL)
{	
	Init();	
}

SPWindowAuction::SPWindowAuction( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent /* = NULL  */ )
: SPWindow( WNDID_AUCTION , InstanceID , iX , iY , iCX , iCY , pParent )
, m_pWindowSearch(NULL)
, m_pWindowSearchPopup(NULL)
, m_pWindowBuy(NULL)
, m_pWindowSale(NULL)
, m_pWindowSaleTip(NULL)
, m_pTextureBG(NULL)
, m_pTextureGuide(NULL)
, m_pSaleTypeInfo(NULL)
, m_pAuctionMsg(NULL)
, m_pSaleTimer(NULL)
, m_pSaleTipTimer(NULL)
, m_pSaleItem(NULL)
, m_pSearchTimer(NULL)
, m_pSearchButton(NULL)
{	
	Init();	
}

SPWindowAuction::~SPWindowAuction()
{
	Clean();
}

void SPWindowAuction::Init()
{
	m_strToolTip		= "[SPWindowAuction]";
	m_iSearchMenuMax	= 0 ;
	m_iSearchPopMax		= 0 ;
	m_iPreShowPopup		= -1 ;
	m_iSelectedCategory = 0 ;
	m_iSelectedDetail	= 0 ;
	m_iSelectedTab		= AUCTION_SEARCH_TAB ;
	m_iPopMenuMode		= 0 ;		// Invisible
	m_iSearchCurPage	= 0 ;		// 페이지 초기화
	m_iBuyCurPage		= 0 ;		// 페이지 초기화
	m_iSaleCurPage		= 0 ;		// 페이지 초기화
	m_fMoneyUpdateTime	= 0 ;		// 머니 업데이트 타이머

	m_iSearchQuality	= 0 ;		// 검색 품질(전체)
	m_bSearchUsable		= false;	// 사용가능 레벨 체크
	m_bShowSearchGuide	= true ;
	m_bTest				= false ;

	m_pSearchKey = new AUCTION_SEARCH_KEY;
	m_pSearchKey->Clear() ;
	m_pSuccessKey = new AUCTION_SEARCH_KEY;
	m_pSuccessKey->Clear() ;

	m_iSaleType = AUCTION_TYPE_ITEM_BY_ELY;		// 판매유형( 아이템/캐쉬/포인트/엘리)
	m_iSaleIndex = 1 ;
	m_iSaleDuration = PERIOD_SHORT;				// 경매기간 선택( 단기/중기/장기)

	m_pWindowSearchEdit = NULL ;
	for( int i = 0; i < AUCTION_BUY_LIST; i++)
	{
		if( i < AUCTION_SEARCH_LIST )
			m_pWindowSearchList[i] = NULL ;
		m_pWindowBuyList[i] = NULL ;
		m_pWindowSaleList[i] = NULL ;
	}


	LoadMenuLDT() ;

	//////////////////////////////////////////////////////////////////////////
	// Windows
	InitMoneyFrame() ;			// 머니 출력 프레임

	InitAuctionFrame() ;		// 배경 출력 프레임
	InitBackground();

	InitAuctionSearch() ;		// 검색하기 창
	InitAuctionBuy() ;			// 입찰현황 창
	InitAuctionSale() ;			// 경매하기 창
	

	m_pSaleTimer = new SPTimer ;
	m_pSaleTimer->Clear() ;
	m_pSaleTipTimer = new SPTimer ;
	m_pSaleTipTimer->Clear() ;
	m_iSaleArrowIndex = 0 ;
	m_pSearchTimer = new SPTimer ;
	m_pSearchTimer->Clear() ;
}

/**
	최상단 머니 출력 부분
*/
void SPWindowAuction::InitMoneyFrame()
{
	SPWindowStatic*	pWindowStaticFrame = NULL ;
	SPWindowStatic*	pWindowStatic = NULL ;
	SPWindowStatic*	pWindowEly = NULL ;
	SPWindowStatic*	pWindowPoint = NULL ;
	SPWindowStatic*	pWindowCash = NULL ;

	// OutLine Frame
	pWindowStaticFrame	=	new SPWindowStatic( WIID_AUCTION_MONEY_FRAME , 292 , 0 , 508, 32 , this );

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MONEY_FRAME_LEFT , 0 , 0 , 10 , 32 , pWindowStaticFrame );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 1 , 1 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MONEY_FRAME_RIGHT , 10 , 0 , 498, 32 , pWindowStaticFrame );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 13 , 1 );
	pWindowStatic->SetSrcSize(6, 32) ;
	pWindowStatic	=	NULL;

	// Ely
	pWindowEly = new SPWindowStatic( WIID_AUCTION_MONEY_ELY_BG	, 6 , 4 , 178, 21 , pWindowStaticFrame );
	pWindowEly->SetWindowType(WND_TYPE_LEFTBASE);
	pWindowEly->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 21 , 1, 25, 22);
	pWindowEly->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 27 , 1, 29, 22);
	pWindowEly->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 31 , 1, 35, 22);

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MONEY_ELY_TITLE , 2 , 5 , 52 , 13 , pWindowEly );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 459 , 15 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MONEY_ELY_ICON , 156, 5 , 17 , 11 , pWindowEly );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 157 , 22 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MONEY_ELY_TEXT , 54 , 6 , 98, 12 , pWindowEly );
	pWindowStatic->SetFont(FONT_12_BOLD) ;
	pWindowStatic->SetFontColor( RGBA(13, 24, 47, 255) );
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT);
	pWindowStatic->SetTextPeriod(false) ;
	pWindowStatic	=	NULL;

	// Point
	pWindowPoint = new SPWindowStatic( WIID_AUCTION_MONEY_POINT_BG	, 188, 4 , 156 , 21 , pWindowStaticFrame );
	pWindowPoint->SetWindowType(WND_TYPE_LEFTBASE);
	pWindowPoint->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 21 , 1, 25, 22);
	pWindowPoint->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 27 , 1, 29, 22);
	pWindowPoint->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 31 , 1, 35, 22);

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MONEY_POINT_TITLE , 2, 5 , 52 , 13 , pWindowPoint );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 459 , 1 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MONEY_POINT_ICON , 137, 5 , 17 , 11 , pWindowPoint );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 211 , 22 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MONEY_POINT_TEXT , 61, 6 , 72 , 12 , pWindowPoint );
	pWindowStatic->SetFont(FONT_12_BOLD) ;
	pWindowStatic->SetFontColor( RGBA(13, 24, 47, 255) );
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT);
	pWindowStatic->SetTextPeriod(false) ;
	pWindowStatic	=	NULL;


	// Oz
	pWindowCash = new SPWindowStatic( WIID_AUCTION_MONEY_OZ_BG	, 348, 4 , 156 , 21 , pWindowStaticFrame );
	pWindowCash->SetWindowType(WND_TYPE_LEFTBASE);
	pWindowCash->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 21 , 1, 25, 22);
	pWindowCash->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 27 , 1, 29, 22);
	pWindowCash->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 31 , 1, 35, 22);

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MONEY_OZ_TITLE , 2, 5 , 52 , 13 , pWindowCash );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 459 , 29 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MONEY_OZ_ICON , 137, 5 , 17 , 11 , pWindowCash );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 193 , 22 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MONEY_OZ_TEXT , 61, 6 , 72 , 12 , pWindowCash );
	pWindowStatic->SetFont(FONT_12_BOLD) ;
	pWindowStatic->SetFontColor( RGBA(13, 24, 47, 255) );
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT);
	pWindowStatic->SetTextPeriod(false) ;
	pWindowStatic	=	NULL;

	pWindowStaticFrame->Hide() ;

}


/**
	상단의 메뉴를 Localize에 따라서 동적으로 재배치 한다.
	3개: 292,0 - 508,32 && 188,4 - 156,21 && 348,4 - 156,21
	2개: 452,0 - 348,32 && 188,4 - 156,21
	1개: 612,0 - 188,32
	6, 4 // 188, 4 // 388, 4
	Frame/ Left&Right/ Ely&Point&Cash Repose
 */
void SPWindowAuction::UpdateMoneyFrame(CASH_TRADE eTrade)
{
	SPWindow* pWindowFrame = NULL ;
	SPWindow* pWindowPoint = NULL ;
	SPWindow* pWindowCash = NULL ;

	pWindowFrame = Find(WIID_AUCTION_MONEY_FRAME, true) ;				// Frame Repose
	pWindowPoint = Find(WIID_AUCTION_MONEY_POINT_BG, true) ;			// Repose & Show|Hide
	pWindowCash = Find(WIID_AUCTION_MONEY_OZ_BG, true) ;				// Repose & Show|Hide

	if( eTrade == CASH_TRADE_ALL)	{
		pWindowFrame->AdjustCoord( 292, 0, 508, 32);	// Ely && Cash && Point
		pWindowFrame->RefreshChildPos() ;
		pWindowPoint->SetShowEnable(true) ;
		pWindowPoint->AdjustCoord( 188,4 , 156,21);
		pWindowPoint->RefreshChildPos() ;
		pWindowCash->SetShowEnable(true) ;
		pWindowCash->AdjustCoord( 348,4 , 156,21);
		pWindowCash->RefreshChildPos() ;
	}
	else if( eTrade == CASH_TRADE_CASH )	{			// Ely && Cash
		pWindowFrame->AdjustCoord( 452, 0, 348, 32);
		pWindowFrame->RefreshChildPos() ;
		pWindowPoint->SetShowEnable(false) ;
		pWindowCash->SetShowEnable(true) ;
		pWindowCash->AdjustCoord( 188,4 , 156,21);
		pWindowCash->RefreshChildPos() ;

	}
	else if( eTrade == CASH_TRADE_POINT )	{			// Ely && Point
		pWindowFrame->AdjustCoord( 452, 0, 348, 32);
		pWindowFrame->RefreshChildPos() ;
		pWindowPoint->SetShowEnable(true) ;
		pWindowPoint->AdjustCoord( 188,4 , 156,21);
		pWindowPoint->RefreshChildPos() ;
		pWindowCash->SetShowEnable(false) ;
	}
	else	{	// eTrade == CASH_TRADE_NULL			// Ely only
		pWindowFrame->AdjustCoord( 612, 0, 188, 32);
		pWindowFrame->RefreshChildPos() ;
		pWindowPoint->SetShowEnable(false) ;
		pWindowCash->SetShowEnable(false) ;
	}
}

/**
	배경 프레임
*/
void SPWindowAuction::InitAuctionFrame()
{
	SPWindowStatic*	pWindowStatic;
	SPWindowButton*	pWindowButton;

	pWindowButton	=	new	SPWindowButton( WIID_AUCTION_MAIN_TAB + AUCTION_SEARCH_TAB , 20 , 32 , 88 , 28 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 1 ,   1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 1 ,  30 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 1 ,  59 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 1 ,  88 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 1 , 117 );
	pWindowButton->Hide() ;
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_AUCTION_MAIN_TAB + AUCTION_BUY_TAB , 110 , 32 , 88 , 28 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 90 ,   1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 90 ,  30 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 90 ,  59 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 90 ,  88 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 90 , 117 );
	pWindowButton->Hide() ;
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_AUCTION_MAIN_TAB + AUCTION_SALE_TAB , 200 , 32 , 88 , 28 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 179 ,   1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 179 ,  30 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 179 ,  59 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 179 ,  88 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 179 , 117 );
	pWindowButton->Hide() ;
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_AUCTION_MAIN_EQUIP , 641 , 38 , 73 , 20 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 268 ,  1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 268 , 22 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 268 , 43 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 268 , 64 );
	pWindowButton->Hide() ;
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_AUCTION_MAIN_ITEM , 715 , 38 , 73 , 20 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 342 ,  1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 342 , 22 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 342 , 43 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 342 , 64 );
	pWindowButton->Hide() ;
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_AUCTION_MAIN_CLOSE , 770 , 67 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 ,  1 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 ,  1 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 484 , 14 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 497 , 14 );
	pWindowButton->Hide() ;
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MAIN_TITLE_ICON , 19 , 63 , 26 , 20 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 174 , 69 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MAIN_TITLE_TEXT , 44 , 60 , 100 , 24 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 142 , 38 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MAIN_TITLE_LINE , 12 , 464 , 776 , 1 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 22 , 23 );
	pWindowStatic->SetSrcSize(6, 1) ;
	pWindowStatic	=	NULL;
}


void SPWindowAuction::InitBackground()
{
	// Texture
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , &m_pTextureBG );
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , &m_pTextureGuide);

	// Source
	SetRect( &m_rcBGSrc[0]	, 467 , 482 , 471 , 486 );
	SetRect( &m_rcBGSrc[1]	, 472 , 482 , 476 , 486 );
	SetRect( &m_rcBGSrc[2]	, 477 , 482 , 481 , 486 );
	SetRect( &m_rcBGSrc[3]	, 467 , 487 , 471 , 491 );
	SetRect( &m_rcBGSrc[4]	, 472 , 487 , 476 , 491 );
	SetRect( &m_rcBGSrc[5]	, 477 , 487 , 481 , 491 );
	SetRect( &m_rcBGSrc[6]	, 467 , 492 , 471 , 496 );
	SetRect( &m_rcBGSrc[7]	, 472 , 492 , 476 , 496 );
	SetRect( &m_rcBGSrc[8]	, 477 , 492 , 481 , 496 );
	SetRect( &m_rcBGSrc[9]	, 472 , 457 , 476 , 461 );		// Orange

	// Destination
	SetRect( &m_rcBGDest[0]	,  10,  58 ,  14 ,  62 );
	SetRect( &m_rcBGDest[1]	,  14,  58 , 786 ,  62 );
	SetRect( &m_rcBGDest[2]	, 786,  58 , 790 ,  62 );
	SetRect( &m_rcBGDest[3]	,  10,  62 ,  14 , 490 );
	SetRect( &m_rcBGDest[4]	,  14,  62 , 786 , 490 );
	SetRect( &m_rcBGDest[5]	, 786,  62 , 790 , 490 );
	SetRect( &m_rcBGDest[6]	,  10, 490 ,  14 , 494 );
	SetRect( &m_rcBGDest[7]	,  14, 490 , 786 , 494 );
	SetRect( &m_rcBGDest[8]	, 786, 490 , 790 , 494 );
	SetRect( &m_rcBGDest[9]	,  12,  84 , 788 , 464 );
	
	// Guild Image
	SetRect( &m_rcGuideSrc	,	 1,  123 , 336, 372);
	SetRect( &m_rcGuideDest	,  212,  181 , 547, 430);
}


void SPWindowAuction::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pTextureBG );
	SAFE_RELEASE( m_pTextureGuide );
	SAFE_DELETE( m_pSaleTimer ) ;
	SAFE_DELETE( m_pSaleTipTimer ) ;
	SAFE_DELETE( m_pSearchTimer ) ;
	SAFE_DELETE( m_pSearchKey ) ;
	SAFE_DELETE( m_pSuccessKey ) ;
	for(int i = 0; i < AUCTION_MAX_MENU; i++)
		m_stMenu[i].Clear() ;
}


void SPWindowAuction::Process(float fTime)
{
	if( !m_bShow )	return;

	// Search Menu Show
	if( m_iPopMenuMode == 1 )	{
		m_iPopMenuSize += (int)( m_iPopMenuDestSize * fTime * 5) ;	// 0.25sec
		if( m_iPopMenuSize > m_iPopMenuDestSize )	{
			m_iPopMenuSize = m_iPopMenuDestSize ;
			ShowPopMenu(m_iPreShowPopup) ;
		}
		m_pWindowSearchPopup->SetRectSize(109, m_iPopMenuSize) ;
	}

	// Cash, Point, Ely 정보 갱신 (외부 업데이트 실시간 적용)
	m_fMoneyUpdateTime += fTime ;
	if( m_fMoneyUpdateTime > 0.1f )	{
		m_fMoneyUpdateTime = 0 ;
		OnAuctionMoneyUpdate(0, 0) ;
	}

	SPWindow::Process( fTime );

	// SearchButton Timer
	if( AUCTION_SEARCH_TAB == m_iSelectedTab )
	{
		if( m_pSearchTimer->IsEnable() && m_pSearchTimer->CheckTimer(fTime) )
		{
			m_pSearchButton->SetEnable(true) ;
			m_pSearchTimer->Clear() ;
		}
	}

	// Sale Arrow Timer
	if( AUCTION_TYPE_ITEM_BY_ELY == m_iSaleType && AUCTION_SALE_TAB == m_iSelectedTab )
	{
		if( m_pSaleTimer->CheckTimer(fTime) )
		{
			m_iSaleArrowIndex = (m_iSaleArrowIndex + 1) % 5 ;
			SetSaleArrowIcon(m_iSaleArrowIndex) ;
			m_pSaleTimer->Start(1, 0.1f) ;
		}
	}
	// Sale TipMessage Timer
	if( AUCTION_SALE_TAB == m_iSelectedTab )
	{
		if( m_pSaleTipTimer->IsEnable() && m_pSaleTipTimer->CheckTimer(fTime) )
		{
			m_pWindowSaleTip->Hide() ;
			m_pWindowSaleTip->SetShowEnable(false) ;
			m_pSaleTipTimer->Clear() ;
		}
	}
}


void SPWindowAuction::Render(float fTime)
{
	if( !m_bShow )	return;

	int i ;
	g_pVideo->Flush();
	for(i = 0; i < AUCTION_BG_IMG; i++ )
		m_pTextureBG->RenderTexture(&m_rcBGDest[i], &m_rcBGSrc[i]) ;

	g_pVideo->Flush();
	SPWindow::Render( fTime );

	if( m_pTextureGuide && m_bShowSearchGuide && AUCTION_SEARCH_TAB == m_iSelectedTab )	{
		m_pTextureGuide->RenderTexture(&m_rcGuideDest, &m_rcGuideSrc) ;
	}

	if( m_iPopMenuMode )
		m_pWindowSearchPopup->Render(fTime) ;

	g_pVideo->Flush();
}

void SPWindowAuction::Show(bool bWithChild)
{
	m_pAuctionMsg = g_pInterfaceManager->GetAuctionMsgWindow() ;
	m_pAuctionMsg->SPSendMessage(SPIM_AUCTION_WAIT_CLOSE, 0, 0) ;

	CASH_TRADE	eCashDisplay = SPLocalizeManager::GetInstance()->GetCashTradeDisplay() ;
	UpdateMoneyFrame(eCashDisplay) ;

	SPWindow::Show( bWithChild );
	g_pInterfaceManager->SetFocusWindow(this) ;

	// Default Show is [Auction Search]
	m_fMoneyUpdateTime = 0 ;	// 머니 업데이트 타이머
	m_iSelectedTab = AUCTION_SEARCH_TAB ;
	m_iSelectedCategory = -1 ;	// 카테고리 선택 초기화
	m_iPreShowPopup = -1 ;
	m_iSelectedDetail = -1 ;	// 세부 선택 초기화
	SetSelectedTitle(true) ;	// 선택표시 초기화

	OnAuctionMoneyUpdate(0, 0) ;	// 머니 초기화
	InitShowAuctionSearch() ;	// 화면 초기화

	if( g_pkAuctionManager->GetServerOpen() == false )	{
		OnAuctionServerClose((WPARAM)52000075, 0) ;
	}
	
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MESSAGE);
	if( pWindow )
		pWindow->SPSendMessage(SPIM_SET_MESSAGE_SHOW, (WPARAM)false);
}


void SPWindowAuction::Hide(bool bSendServer)
{
	SPWindow* pWindow = Find(WIID_AUCTION_MAIN_TAB+m_iSelectedTab	, true); 
	if(pWindow) 
		pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);

	HidePopMenu() ;
	
	if( m_bShow )
		g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);		//[2007/10/4] - 컷인 상태를 해제하도록 한다 <br>

	SPWindow::Hide( bSendServer );

	//페이지 초기화
	m_iSearchCurPage	= 0 ;		// 검색에서 현재 보고있는 페이지
	m_iBuyCurPage		= 0 ;		// 입찰에서 현재 보고있는 페이지
	m_iSaleCurPage		= 0 ;		// 경매하기에서 현재 보고있는 페이지

	SetSaleItemInfo() ;			// DragItem 초기화
	if( g_pInterfaceManager->GetModalWindow() != NULL )
		g_pInterfaceManager->SetModalWindow(NULL) ;

	// Hide시 SearchArchive Clear
	if( g_pkAuctionManager )	{
		SPAuctionArchive* pkSearchArchive = g_pkAuctionManager->GetAuctionSearchArchive() ;
		if( pkSearchArchive != NULL )
			pkSearchArchive->ClearList() ;
	}

	//g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);		//[2007/10/4] - 컷인 상태를 해제하도록 한다 <br>
}



/**
	각 탭별 Sort를 Archive로부터 읽어서 업데이트 한다.
 */
void SPWindowAuction::UpdateSortArrow(AUCTION_TAB eTab)
{
	SPWindowStatic* pWindowStatic = NULL ;
	SPAuctionArchive* pArchive = NULL ;
	if( eTab == AUCTION_SEARCH_TAB )	{		// Search -> 5
		pArchive = g_pkAuctionManager->GetAuctionSearchArchive() ;
		if( !pArchive )		return ;
		if( m_pSearchSort[AUCTION_SORT_QUALITY] )
			m_pSearchSort[AUCTION_SORT_QUALITY]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_QUALITY) ) ;
		if( m_pSearchSort[AUCTION_SORT_ITEM_LV] )
			m_pSearchSort[AUCTION_SORT_ITEM_LV]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_ITEM_LV) ) ;
		if( m_pSearchSort[AUCTION_SORT_ITEM_OPTPOINT] )
			m_pSearchSort[AUCTION_SORT_ITEM_OPTPOINT]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_ITEM_OPTPOINT) ) ;
		if( m_pSearchSort[AUCTION_SORT_DURATION] )
			m_pSearchSort[AUCTION_SORT_DURATION]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_DURATION) ) ;
		if( m_pSearchSort[AUCTION_SORT_PRICE] )
			m_pSearchSort[AUCTION_SORT_PRICE]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_PRICE) ) ;
	}
	else if( eTab == AUCTION_BUY_TAB )	{		// Buy -> 5
		pArchive = g_pkAuctionManager->GetAuctionBuyArchive() ;
		if( !pArchive )		return ;
		if( m_pBuySort[AUCTION_SORT_QUALITY] )
			m_pBuySort[AUCTION_SORT_QUALITY]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_QUALITY) ) ;
		if( m_pBuySort[AUCTION_SORT_ITEM_LV] )
			m_pBuySort[AUCTION_SORT_ITEM_LV]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_ITEM_LV) ) ;
		if( m_pBuySort[AUCTION_SORT_ITEM_OPTPOINT] )
			m_pBuySort[AUCTION_SORT_ITEM_OPTPOINT]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_ITEM_OPTPOINT) ) ;
		if( m_pBuySort[AUCTION_SORT_DURATION] )
			m_pBuySort[AUCTION_SORT_DURATION]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_DURATION) ) ;
		if( m_pBuySort[AUCTION_SORT_PRICE] )
			m_pBuySort[AUCTION_SORT_PRICE]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_PRICE) ) ;

	}
	else if( eTab == AUCTION_SALE_TAB )	{		// Sale -> 3
		pArchive = g_pkAuctionManager->GetAuctionSaleArchive() ;
		if( !pArchive )		return ;
		if( m_pSearchSort[AUCTION_SORT_QUALITY] )
			m_pSaleSort[AUCTION_SORT_QUALITY]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_QUALITY) ) ;
		if( m_pSearchSort[AUCTION_SORT_DURATION] )
			m_pSaleSort[AUCTION_SORT_DURATION]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_DURATION) ) ;
		if( m_pSearchSort[AUCTION_SORT_PRICE] )
			m_pSaleSort[AUCTION_SORT_PRICE]->SPSendMessage(SPIM_SET_IMAGEINDEX, pArchive->GetSortAscendKey(AUCTION_SORT_PRICE) ) ;
	}
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowAuction )

SPIMESSAGE_COMMAND(	SPIM_AUCTION_SERVER_CLOSE	, OnAuctionServerClose)
SPIMESSAGE_COMMAND(	SPIM_AUCTION_MONEY_UPDATE	, OnAuctionMoneyUpdate)
SPIMESSAGE_COMMAND(	SPIM_AUCTION_SET_SEARCHNAME	, OnAuctionSetSearchName)
SPIMESSAGE_COMMAND(	SPIM_AUCTION_SEND_BID	, OnSendAuctionBidBuy)
SPIMESSAGE_COMMAND(	SPIM_AUCTION_SEND_SPOT	, OnSendAuctionSpotBuy)
SPIMESSAGE_COMMAND(	SPIM_AUCTION_SEND_NEXT	, OnSendAuctionGetNext)
SPIMESSAGE_COMMAND(	SPIM_AUCTION_SEND_CANCEL, OnSendAuctionSaleCancel)
//SPIMESSAGE_COMMAND(	SPIM_AUCTION_ERROR_NOTICE	, OnAuctionErrorNotice)
SPIMESSAGE_COMMAND(	SPIM_SETFOCUS	, OnFocus)
SPIMESSAGE_COMMAND(	SPIM_ACTIVATE	, OnActivate)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_MAIN_TAB + AUCTION_SEARCH_TAB	, OnMainSearch	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_MAIN_TAB + AUCTION_BUY_TAB		, OnMainBuy	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_MAIN_TAB + AUCTION_SALE_TAB		, OnMainSale	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_MAIN_EQUIP		, OnMyEquip	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_MAIN_ITEM		, OnMyInven	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_MAIN_CLOSE		, OnClose	)

// Search
SPIMESSAGE_COMMAND(	SPIM_AUCTION_SEARCH_LIST	, OnReceiveAuctionSearchList	)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	, WIID_AUCTION_SEARCH_MENU_BUTTON	, WIID_AUCTION_SEARCH_MENU_BUTTON+30	, OnCategorySelect	)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	, WIID_AUCTION_POPUP_LIST			, WIID_AUCTION_POPUP_LIST + 60			, OnDetailSelect	)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER	, WIID_AUCTION_SEARCH_MENU_ICON	, WIID_AUCTION_SEARCH_MENU_ICON + 9		, OnCategoryIconCursorON	)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT	, WIID_AUCTION_SEARCH_MENU_ICON	, WIID_AUCTION_SEARCH_MENU_ICON + 9		, OnCategoryIconCursorOUT	)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER	, WIID_AUCTION_SEARCH_MENU_TEXT	, WIID_AUCTION_SEARCH_MENU_TEXT + 9		, OnCategoryTextCursorON	)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT	, WIID_AUCTION_SEARCH_MENU_TEXT	, WIID_AUCTION_SEARCH_MENU_TEXT + 9		, OnCategoryTextCursorOUT	)
SPIMESSAGE_CONTROL_RANGE( SPIM_AUCTION_BID_BUY	, WIID_AUCTION_SEARCH_LIST		, WIID_AUCTION_SEARCH_LIST + 108		, OnSearchListBidClick	)	// 입찰
SPIMESSAGE_CONTROL_RANGE( SPIM_AUCTION_SPOT_BUY	, WIID_AUCTION_SEARCH_LIST		, WIID_AUCTION_SEARCH_LIST + 108		, OnSearchListSpotClick	)	// 즉시 구매
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		, WIID_AUCTION_SEARCH_LIST		, WIID_AUCTION_SEARCH_LIST + 108		, OnSearchListClick	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_SEARCH_LIST_QUALITY		, OnSortSearchQuality	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_SEARCH_LIST_LEVEL		, OnSortSearchLevel	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_SEARCH_LIST_DURATION		, OnSortSearchDuration	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_SEARCH_LIST_FINISHTIME	, OnSortSearchFinish	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_SEARCH_LIST_PRICE		, OnSortSearchPrice	)
SPIMESSAGE_CONTROL( SPIM_PAGE					, WIID_AUCTION_SEARCH_PAGE			, OnSearchListPage )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_SEARCH_BT_USEABLE_BTN	, OnSearchUsableCheck	)
SPIMESSAGE_CONTROL( SPIM_COMBOBOX_CHANGE_VALUE	, WIID_AUCTION_SEARCH_BT_QUAL_COMBO	, OnChangeSearchCombo)
SPIMESSAGE_CONTROL( SPIM_SETFOCUS			, WIID_AUCTION_SEARCH_BT_NAME_EDIT		, OnSearchNameSetFocus	)
SPIMESSAGE_CONTROL( SPIM_KILLFOCUS			, WIID_AUCTION_SEARCH_BT_NAME_EDIT		, OnSearchNameKillFocus	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_SEARCH_BT_SEARCH			, OnSearchItem	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_SEARCH_BT_CLOSE			, OnClose	)

// Buy
SPIMESSAGE_COMMAND(	SPIM_AUCTION_BUY_LIST	, OnReceiveAuctionBuyList	)
//SPIMESSAGE_COMMAND(	SPIM_AUCTION_BUY_UPDATE	, OnReceiveAuctionBuyUpdate	)
SPIMESSAGE_CONTROL_RANGE( SPIM_AUCTION_BID_BUY	, WIID_AUCTION_BUY_LIST		, WIID_AUCTION_BUY_LIST + 126	, OnBuyListBidClick	)	// 입찰
SPIMESSAGE_CONTROL_RANGE( SPIM_AUCTION_SPOT_BUY	, WIID_AUCTION_BUY_LIST		, WIID_AUCTION_BUY_LIST + 126	, OnBuyListSpotClick)	// 즉시 구매
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_BUY_LIST_QUALITY			, OnSortBuyQuality	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_BUY_LIST_LEVEL			, OnSortBuyLevel	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_BUY_LIST_DURATION		, OnSortBuyDuration	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_BUY_LIST_FINISHTIME		, OnSortBuyFinish	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_BUY_LIST_PRICE			, OnSortBuyPrice	)
SPIMESSAGE_CONTROL( SPIM_PAGE				, WIID_AUCTION_BUY_PAGE					, OnBuyListPage )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_BUY_BT_CLOSE				, OnClose	)

//Sale
SPIMESSAGE_COMMAND(	SPIM_AUCTION_SALE_LIST		, OnReceiveAuctionSaleList	)
//SPIMESSAGE_COMMAND(	SPIM_AUCTION_SALE_UPDATE	, OnReceiveAuctionSaleUpdate	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_REGI_SALE_LEFT		, OnSaleRegistryLeft	)	// 판매 유형 좌측
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_REGI_SALE_RIGHT		, OnSaleRegistryRight	)	// 판매 유형 우측
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_REGI_ITEM			, OnSaleItemButtonUp	)	// Item Drag On
SPIMESSAGE_CONTROL( SPIM_TOOLTIP_ENTER		, WIID_AUCTION_REGI_ITEM			, OnSaleItemToolTipEnter)
SPIMESSAGE_CONTROL( SPIM_TOOLTIP_OUT		, WIID_AUCTION_REGI_ITEM			, OnSaleItemToolTipOut)
SPIMESSAGE_CONTROL( SPIM_TOOLTIP_MOVE		, WIID_AUCTION_REGI_ITEM			, OnSaleItemToolTipMove)
SPIMESSAGE_CONTROL( SPIM_KEY_DOWN			, WIID_AUCTION_REGI_MONEY_EDIT		, OnSaleMoneyKeyDown	)	// 경매 판매가 입력(바뀐뒤)
SPIMESSAGE_CONTROL( SPIM_REFRESH			, WIID_AUCTION_REGI_MONEY_EDIT		, OnSaleMoneyKeyDown	)	// 경매 판매가 입력(입력중)
SPIMESSAGE_CONTROL( SPIM_KEY_DOWN			, WIID_AUCTION_REGI_PRICE_ST_EDIT	, OnSaleMoneyKeyDown	)	// 경매 시작가 입력(바뀐뒤)
SPIMESSAGE_CONTROL( SPIM_REFRESH			, WIID_AUCTION_REGI_PRICE_ST_EDIT	, OnSaleMoneyKeyDown	)	// 경매 시작가 입력(입력중)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	, WIID_AUCTION_REGI_DURATION_BTN, WIID_AUCTION_REGI_DURATION_BTN+3	, OnSaleDuration	)		// 경매기간
SPIMESSAGE_CONTROL_RANGE( SPIM_AUCTION_CANCEL	, WIID_AUCTION_SALE_LIST	, WIID_AUCTION_SALE_LIST + 126		, OnSaleListCancelClick	)	// 경매 취소
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_SALE_LIST_QUALITY		, OnSortSaleQuality	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_SALE_LIST_FINISHTIME		, OnSortSaleFinish	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_SALE_LIST_PRICE			, OnSortSalePrice	)
SPIMESSAGE_CONTROL( SPIM_PAGE				, WIID_AUCTION_SALE_PAGE				, OnSaleListPage )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_REGI_OK_BUTTON			, OnSaleRegistItem	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_REGI_CANCEL_BUTTON		, OnSaleClearItem	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_SALE_BT_CLOSE			, OnClose	)

SPIMESSAGE_OBJ_MAP_END(	SPWindow	)


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
/**
	Auction Server Close
 */
 int SPWindowAuction::OnAuctionServerClose(WPARAM wParam, LPARAM lParam)
{
	Hide() ;
	return 0 ;
}


/**
	[검색하기] 메뉴가 선택된 경우
*/
int SPWindowAuction::OnMainSearch(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow ;
	if( m_iSelectedTab != AUCTION_SEARCH_TAB )	{
		pWindow = Find(WIID_AUCTION_MAIN_TAB+m_iSelectedTab	, true); 
		if(pWindow) 
			pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
		g_pInterfaceManager->ShowWindow(WIID_ITEM, SP_HIDE) ;
		g_pInterfaceManager->ShowWindow(WIID_EQUIP, SP_HIDE) ;
	}

	m_iSelectedTab = AUCTION_SEARCH_TAB ;
	ShowAuctionSearch() ;

	pWindow = Find(WIID_AUCTION_MAIN_TAB+m_iSelectedTab	, true); 
	if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE	);
	return 0 ;
}


/**
	[입찰 현황] 메뉴가 선택된 경우
*/
int SPWindowAuction::OnMainBuy(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow ;
	if( m_iSelectedTab != AUCTION_BUY_TAB )	{
		pWindow = Find(WIID_AUCTION_MAIN_TAB+m_iSelectedTab	, true); 
		if(pWindow) 
			pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
		g_pInterfaceManager->ShowWindow(WIID_ITEM, SP_HIDE) ;
		g_pInterfaceManager->ShowWindow(WIID_EQUIP, SP_HIDE) ;
	}

	// Show 이후 1회만 보내도록 한다.
	if( g_pkAuctionManager->CheckArchiveReset(AUCTION_BUY_TAB) )
	{
		m_iBuyCurPage = 0 ;
		g_pkAuctionManager->SendAuctionBuyRequest() ;
	}

	m_iSelectedTab = AUCTION_BUY_TAB ;
	ShowAuctionBuy() ;

	pWindow = Find(WIID_AUCTION_MAIN_TAB+m_iSelectedTab	, true); 
	if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE	);

	return 0 ;
}


/**
	[경매하기] 메뉴가 선택된 경우
*/
int SPWindowAuction::OnMainSale(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow ;
	if( m_iSelectedTab != AUCTION_SALE_TAB )	{
		pWindow = Find(WIID_AUCTION_MAIN_TAB+m_iSelectedTab	, true); 
		if(pWindow) 
			pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
		g_pInterfaceManager->ShowWindow(WIID_ITEM, SP_HIDE) ;
		g_pInterfaceManager->ShowWindow(WIID_EQUIP, SP_HIDE) ;
	}

	if( g_pkAuctionManager->CheckArchiveReset(AUCTION_SALE_TAB) )
	{
		m_iSaleCurPage = 0 ;
		g_pkAuctionManager->SendAuctionSaleRequest() ;
	}

	m_iSelectedTab = AUCTION_SALE_TAB ;
	ShowAuctionSale() ;

	pWindow = Find(WIID_AUCTION_MAIN_TAB+m_iSelectedTab	, true); 
	if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE	);

	return 0 ;
}


/**
	[장비] 메뉴가 선택된 경우 -> 내 장비 & 인벤토리
*/
int SPWindowAuction::OnMyEquip(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->ShowWindow(WIID_ITEM, SP_SHOW) ;
	g_pInterfaceManager->ShowWindow(WIID_EQUIP, SP_SHOW) ;

	SPWindow* pEquipWindow = NULL ;
	pEquipWindow = g_pInterfaceManager->GetEquipWindow() ;
	g_pInterfaceManager->SetFocusWindow(pEquipWindow) ;

	return 0 ;
}


/**
	[아이템] 메뉴가 선택된 경우 -> 내 인벤토리
*/
int SPWindowAuction::OnMyInven(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->ShowWindow(WIID_ITEM, SP_SHOW) ;

	SPWindow* pItemWindow = NULL ;
	pItemWindow = g_pInterfaceManager->GetInvenWindow() ;
	g_pInterfaceManager->SetFocusWindow(pItemWindow) ;
	return 0 ;
}


/**
	[닫기] or [나가기] 메뉴가 선택된 경우
*/
int SPWindowAuction::OnClose(WPARAM wParam, LPARAM lParam)
{
	Hide() ;
	return 0 ;
}



/**
	Auction 상단의 머니 정보를 업데이트 한다.
*/
int SPWindowAuction::OnAuctionMoneyUpdate(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = NULL ;
	if( (pWindow = Find(WIID_AUCTION_MONEY_ELY_TEXT, true)) )	{
		pWindow->SetWindowText(ConvertMoneyToString(g_pkAuctionManager->GetPlayerEly())) ;
	}

	if( (pWindow = Find(WIID_AUCTION_MONEY_POINT_TEXT, true)) )	{
		pWindow->SetWindowText(ConvertMoneyToString((INT64)g_pkAuctionManager->GetPlayerMilige())) ;
	}

	if( (pWindow = Find(WIID_AUCTION_MONEY_OZ_TEXT, true)) )	{
		pWindow->SetWindowText(ConvertMoneyToString((INT64)g_pkAuctionManager->GetPlayerCash())) ;
	}
	return 0 ;
}


/**
	Shift+Click을 하는 경우 아이템 이름이 검색이름에 설정됨
	아이템 번호로 아이템을 찾을수 있고, 현재 Search 상태인 경우에만 보여줌
 */
int SPWindowAuction::OnAuctionSetSearchName(WPARAM wParam, LPARAM lParam)
{
	SPItemAttr* pItemAttr = NULL ;
	SPWindow* pWindow = NULL ;
	int iItemID = (int)wParam ;
	SPItem* pItem = (SPItem*)lParam ;

	if( m_iSelectedTab == AUCTION_SEARCH_TAB )	{
		g_pItemCluster->GetItemInfo(iItemID, pItemAttr);
		if( pItemAttr )	{
			pWindow = Find(WIID_AUCTION_SEARCH_BT_NAME_EDIT, true) ;
			if( pWindow )
				pWindow->SetWindowText( pItemAttr->m_strName.c_str() ) ;
		}
	}
	else if( m_iSelectedTab == AUCTION_SALE_TAB )	{
		if( pItem != NULL )
			CheckSaleItem(false, pItem) ;
		else	{
			// 아이템 포인터가 없으면 직접 찾는다.
			SPSelectItem SelectItem ;
			SelectItem.Clear() ;

			SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();
			if( pLocalPlayer == NULL )
				return false;

			SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();
			if( pInvenArchive == NULL )
				return false;
			
			if( pInvenArchive->FindInvenItem( iItemID, 1, &SelectItem) == false )
			pItem = pInvenArchive->GetInventoryItem(SelectItem.m_iContainer, SelectItem.m_iSlotIndex);	
			if( pItem )
				CheckSaleItem(false, pItem) ;
		}
	}
	return 0 ;
}


/**
	AuctionMsg에서 메시지를 받아서 해당 패킷을 보내도록 한다.
 */
int SPWindowAuction::OnSendAuctionSpotBuy(WPARAM wParam, LPARAM lParam)
{
	SPAuctionAttr* pBidItemAttr = (SPAuctionAttr*)lParam ;

	int iSearchIndex = m_iSearchCurPage * AUCTION_SEARCH_LIST ;
	g_pkAuctionManager->SendAuctionSpotBuy(m_pSuccessKey, iSearchIndex, pBidItemAttr ) ;
	return 0 ;
}

int SPWindowAuction::OnSendAuctionBidBuy(WPARAM wParam, LPARAM lParam)
{
	SPAuctionAttr* pBidItemAttr = (SPAuctionAttr*)lParam ;

	g_pkAuctionManager->SendAuctionBidBuy( pBidItemAttr );
	return 0 ;
}

int SPWindowAuction::OnSendAuctionGetNext(WPARAM wParam, LPARAM lParam)
{
	int iRequestIndex = m_iSearchCurPage * AUCTION_SEARCH_LIST ;
	g_pkAuctionManager->SendAuctionSearchNext(iRequestIndex, m_pSuccessKey) ;		// 현재페이지부터 다시 받는다.
	return 0 ;
}

int SPWindowAuction::OnSendAuctionSaleCancel(WPARAM wParam, LPARAM lParam)
{
	SPAuctionAttr* pSaleItemAttr = (SPAuctionAttr*)lParam ;

	g_pkAuctionManager->SendAuctionSaleCancel(pSaleItemAttr) ;
	return 0 ;
}


int SPWindowAuction::OnFocus(WPARAM wParam, LPARAM lParam)
{
	//ErrorFLogs->Error_Log("AuctionWindow OnFocus") ;
	if( m_bTest )	{
		SPWindow* pWindow = NULL ;
		if( g_pInterfaceManager->GetModalWindow() )
			return 0 ;

		pWindow = g_pInterfaceManager->GetEquipWindow() ;
		if( pWindow && pWindow->IsShow() )
			g_pInterfaceManager->SetActiveWindow(pWindow) ;

		pWindow = g_pInterfaceManager->GetInvenWindow() ;
		if( pWindow && pWindow->IsShow() )
			g_pInterfaceManager->SetActiveWindow(pWindow) ;
	}
	return 0 ;
}

int SPWindowAuction::OnActivate(WPARAM wParam, LPARAM lParam)
{
	//ErrorFLogs->Error_Log("AuctionWindow OnActivate") ;
	if( m_bTest )	{
		SPWindow* pWindow = NULL ;
		if( g_pInterfaceManager->GetModalWindow() )
			return 0 ;

		pWindow = g_pInterfaceManager->GetEquipWindow() ;
		if( pWindow && pWindow->IsShow() )
			g_pInterfaceManager->SetActiveWindow(pWindow) ;

		pWindow = g_pInterfaceManager->GetInvenWindow() ;
		if( pWindow && pWindow->IsShow() )
			g_pInterfaceManager->SetActiveWindow(pWindow) ;
	}

	return 0 ;
}
////////////////////////////////////////////////////////////////////////////
//// For Test
//int SPWindowAuction::OnTest(WPARAM wParam, LPARAM lParam)
//{
//	return 0 ;
//}