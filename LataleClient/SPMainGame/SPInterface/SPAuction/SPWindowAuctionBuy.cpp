// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowAuctionBuy.cpp
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
#include "SPWindowAuctionListUnit.h"
#include "SPWindowPageSelect.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPAuctionDef.h"
#include "SPAuctionAttr.h"
#include "SPAuctionArchive.h"
#include "SPAuctionManager.h"

#include "SPWindowAuction.h"
#include "SPWindowAuctionMsg.h"
#include "_Error_log.h"


void SPWindowAuction::InitAuctionBuy()
{
	int i ;
	SPWindowStatic*	pWindowStaticFrame;
	SPWindowStatic*	pWindowStaticBG;
	SPWindowStatic*	pWindowStatic;
	SPWindowButton*	pWindowButton;
	SPWindowPageSelect* pkPageSelect;

	pWindowStaticFrame	=	new SPWindowStatic( WIID_AUCTION_BUY_FRAME , 12 , 84 , 778 , 410 , this );
	m_pWindowBuy = (SPWindow*)pWindowStaticFrame ;

	// Sub BG
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_BUY_LIST_LINE , 0 , 11 , 776 , 1 , m_pWindowBuy );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 22 , 23 );
	pWindowStatic->SetSrcSize(1, 4);
	pWindowStaticFrame	=	new SPWindowStatic( WIID_AUCTION_BUY_LIST_BG , 0 , 12 , 776 , 368 , m_pWindowBuy );
	pWindowStaticFrame->SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 502 , 472 );
	pWindowStaticFrame->SetSrcSize(4, 4);

	// List Title (Quality)
	pWindowButton = new SPWindowButton( WIID_AUCTION_BUY_LIST_QUALITY	, 4 , 4 , 205 , 19 , pWindowStaticFrame );
	pWindowButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 66 , 294);
	pWindowButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 66 , 314);
	pWindowButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 66 , 334);
	pWindowStatic =	new SPWindowStatic( WIID_AUCTION_BUY_SORT_ARROW + AUCTION_SORT_QUALITY, 190 , 5, 10, 9, pWindowButton );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 450, 1);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 461, 1);
	m_pBuySort[AUCTION_SORT_QUALITY] = (SPWindow*)pWindowStatic ;
	pWindowButton = NULL ;
	pWindowStatic = NULL ;
	pWindowButton = new SPWindowButton( WIID_AUCTION_BUY_LIST_LEVEL	, 213 , 4 , 42, 19 , pWindowStaticFrame );
	pWindowButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 268, 85);
	pWindowButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 268, 105);
	pWindowButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 268, 125);
	pWindowStatic =	new SPWindowStatic( WIID_AUCTION_BUY_SORT_ARROW + AUCTION_SORT_ITEM_LV, 27 , 5, 10, 9, pWindowButton );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 450, 1);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 461, 1);
	m_pBuySort[AUCTION_SORT_ITEM_LV] = (SPWindow*)pWindowStatic ;
	pWindowButton = NULL ;
	pWindowStatic = NULL ;
	pWindowButton = new SPWindowButton( WIID_AUCTION_BUY_LIST_DURATION	, 259 , 4 , 60 , 19 , pWindowStaticFrame );
	pWindowButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 311, 85);
	pWindowButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 311, 105);
	pWindowButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 311, 125);
	pWindowStatic =	new SPWindowStatic( WIID_AUCTION_BUY_SORT_ARROW + AUCTION_SORT_ITEM_OPTPOINT, 45 , 5, 10, 9, pWindowButton );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 450, 1);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 461, 1);
	m_pBuySort[AUCTION_SORT_ITEM_OPTPOINT] = (SPWindow*)pWindowStatic ;
	pWindowButton = NULL ;
	pWindowStatic = NULL ;
	pWindowButton = new SPWindowButton( WIID_AUCTION_BUY_LIST_FINISHTIME	, 323 , 4, 67, 19 , pWindowStaticFrame );
	pWindowButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 372, 85);
	pWindowButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 372, 105);
	pWindowButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 372, 125);
	pWindowStatic =	new SPWindowStatic( WIID_AUCTION_BUY_SORT_ARROW + AUCTION_SORT_DURATION, 52, 5, 10, 9, pWindowButton );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 450, 1);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 461, 1);
	m_pBuySort[AUCTION_SORT_DURATION] = (SPWindow*)pWindowStatic ;
	pWindowButton = NULL ;
	pWindowStatic = NULL ;
	pWindowStatic =	new SPWindowStatic( WIID_AUCTION_BUY_LIST_SELLER , 394 , 4, 112, 19, pWindowStaticFrame );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 108 , 266);
	pWindowStatic = NULL ;
	pWindowButton = new SPWindowButton( WIID_AUCTION_BUY_LIST_PRICE	, 510 , 4 , 262 , 19 , pWindowStaticFrame );
	pWindowButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 108 , 146);
	pWindowButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 108 , 166);
	pWindowButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 108 , 186);
	pWindowStatic =	new SPWindowStatic( WIID_AUCTION_BUY_SORT_ARROW + AUCTION_SORT_PRICE, 247 , 5, 10, 9, pWindowButton );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 450, 1);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 461, 1);
	m_pBuySort[AUCTION_SORT_PRICE] = (SPWindow*)pWindowStatic ;
	pWindowButton = NULL ;
	pWindowStatic = NULL ;

	//pWindowStaticBG = new SPWindowStatic( WIID_AUCTION_BUY_LIST_QUALITY+1	, 4 , 4 , 205 , 19 , pWindowStaticFrame );
	//pWindowStaticBG->SetWindowType(WND_TYPE_LEFTBASE);
	//pWindowStaticBG->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 67 , 1, 71, 20);
	//pWindowStaticBG->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 72 , 1, 76, 20);
	//pWindowStaticBG->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 77 , 1, 81, 20);
	//pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_BUY_LIST_QUALITY+2 , 47 , 3 , 100 , 13 , pWindowStaticBG );
	//pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 1 , 123 );
	//pWindowStatic	=	NULL;
	//pWindowButton	=	new	SPWindowButton( WIID_AUCTION_BUY_LIST_QUALITY , 194 , 8 , 6 , 5 , pWindowStaticBG );
	//pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 472 ,  1 );
	//pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 479 ,  1 );
	//pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 486 ,  1 );
	//pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 493 ,  1 );
	//// List Title (Level)
	//pWindowStaticBG = new SPWindowStatic( WIID_AUCTION_BUY_LIST_LEVEL+1	, 213 , 4 , 42 , 19 , pWindowStaticFrame );
	//pWindowStaticBG->SetWindowType(WND_TYPE_LEFTBASE);
	//pWindowStaticBG->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 67 , 1, 71, 20);
	//pWindowStaticBG->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 72 , 1, 76, 20);
	//pWindowStaticBG->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 77 , 1, 81, 20);
	//pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_BUY_LIST_LEVEL+2 , 1 , 3 , 30 , 13 , pWindowStaticBG );
	//pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 1 , 193 );
	//pWindowStatic	=	NULL;
	//pWindowButton	=	new	SPWindowButton( WIID_AUCTION_BUY_LIST_LEVEL , 31 , 8 , 6 , 5 , pWindowStaticBG );
	//pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 472 ,  1 );
	//pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 479 ,  1 );
	//pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 486 ,  1 );
	//pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 493 ,  1 );
	//// List Title (Duration)
	//pWindowStaticBG = new SPWindowStatic( WIID_AUCTION_BUY_LIST_DURATION+1	, 259 , 4 , 60 , 19 , pWindowStaticFrame );
	//pWindowStaticBG->SetWindowType(WND_TYPE_LEFTBASE);
	//pWindowStaticBG->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 67 , 1, 71, 20);
	//pWindowStaticBG->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 72 , 1, 76, 20);
	//pWindowStaticBG->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 77 , 1, 81, 20);
	//pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_BUY_LIST_DURATION+2 , 1 , 3 , 48 , 13 , pWindowStaticBG );
	//pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 1 , 179 );
	//pWindowStatic	=	NULL;
	//pWindowButton	=	new	SPWindowButton( WIID_AUCTION_BUY_LIST_DURATION , 49 , 8 , 6 , 5 , pWindowStaticBG );
	//pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 472 ,  1 );
	//pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 479 ,  1 );
	//pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 486 ,  1 );
	//pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 493 ,  1 );
	//// List Title (FinishTime)
	//pWindowStaticBG = new SPWindowStatic( WIID_AUCTION_BUY_LIST_FINISHTIME+1	, 323 , 4 , 65 , 19 , pWindowStaticFrame );
	//pWindowStaticBG->SetWindowType(WND_TYPE_LEFTBASE);
	//pWindowStaticBG->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 67 , 1, 71, 20);
	//pWindowStaticBG->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 72 , 1, 76, 20);
	//pWindowStaticBG->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 77 , 1, 81, 20);
	//pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_BUY_LIST_FINISHTIME+2 , 1 , 3 , 55 , 13 , pWindowStaticBG );
	//pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 1 , 165 );
	//pWindowStatic	=	NULL;
	//pWindowButton	=	new	SPWindowButton( WIID_AUCTION_BUY_LIST_FINISHTIME , 56 , 8 , 6 , 5 , pWindowStaticBG );
	//pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 472 ,  1 );
	//pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 479 ,  1 );
	//pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 486 ,  1 );
	//pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 493 ,  1 );
	//// List Title (Seller)
	//pWindowStaticBG = new SPWindowStatic( WIID_AUCTION_BUY_LIST_SELLER+1	, 394 , 4 , 112 , 19 , pWindowStaticFrame );
	//pWindowStaticBG->SetWindowType(WND_TYPE_LEFTBASE);
	//pWindowStaticBG->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 67 , 1, 71, 20);
	//pWindowStaticBG->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 72 , 1, 76, 20);
	//pWindowStaticBG->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 77 , 1, 81, 20);
	//pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_BUY_LIST_SELLER+2 , 6 , 3 , 100 , 13 , pWindowStaticBG );
	//pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 1 , 137 );
	//pWindowStatic	=	NULL;
	//// List Title (Price)
	//pWindowStaticBG = new SPWindowStatic( WIID_AUCTION_BUY_LIST_PRICE+1	, 510 , 4 , 262 , 19 , pWindowStaticFrame );
	//pWindowStaticBG->SetWindowType(WND_TYPE_LEFTBASE);
	//pWindowStaticBG->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 67 , 1, 71, 20);
	//pWindowStaticBG->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 72 , 1, 76, 20);
	//pWindowStaticBG->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 77 , 1, 81, 20);
	//pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_BUY_LIST_PRICE+2 , 76 , 3 , 100 , 13 , pWindowStaticBG );
	//pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 1 , 151 );
	//pWindowStatic	=	NULL;
	//pWindowButton	=	new	SPWindowButton( WIID_AUCTION_BUY_LIST_PRICE , 251 , 8 , 6 , 5 , pWindowStaticBG );
	//pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 472 ,  1 );
	//pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 479 ,  1 );
	//pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 486 ,  1 );
	//pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 493 ,  1 );

	//////////////////////////////////////////////////////////////////////////
	// Search List
	for( i = 0; i < AUCTION_BUY_LIST; i++)
	{
		m_pWindowBuyList[i] = new SPWindowAuctionListUNIT( WIID_AUCTION_BUY_LIST + (i * AUCTION_MAX_LIST_ITEM), 0, 28 + i*43, 776, 43, pWindowStaticFrame) ;
		m_pWindowBuyList[i]->SetToolTipStr("Buy List") ;
	}


	// Page Number
	pkPageSelect = new SPWindowPageSelect(WIID_AUCTION_BUY_PAGE, 0, 344, 776, 14, pWindowStaticFrame);

	pkPageSelect->SetPageButtonSize(PAGE_SELECT_BUNDLE_PREV, 14, 14);
	pkPageSelect->SetPageButtonImage		(PAGE_SELECT_BUNDLE_PREV, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 454, 455);
	pkPageSelect->SetPageButtonImageHit		(PAGE_SELECT_BUNDLE_PREV, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 454, 469);
	pkPageSelect->SetPageButtonImagePush	(PAGE_SELECT_BUNDLE_PREV, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 454, 483);
	pkPageSelect->SetPageButtonImageDisable	(PAGE_SELECT_BUNDLE_PREV, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 454, 497);

	pkPageSelect->SetPageButtonSize(PAGE_SELECT_PREV, 14, 14);
	pkPageSelect->SetPageButtonImage		(PAGE_SELECT_PREV, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 484, 383);
	pkPageSelect->SetPageButtonImageHit		(PAGE_SELECT_PREV, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 484, 397);
	pkPageSelect->SetPageButtonImagePush	(PAGE_SELECT_PREV, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 484, 411);
	pkPageSelect->SetPageButtonImageDisable	(PAGE_SELECT_PREV, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 484, 425);

	pkPageSelect->SetPageButtonSize(PAGE_SELECT_NEXT, 14, 14);
	pkPageSelect->SetPageButtonImage		(PAGE_SELECT_NEXT, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 498, 383);
	pkPageSelect->SetPageButtonImageHit		(PAGE_SELECT_NEXT, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 498, 397);
	pkPageSelect->SetPageButtonImagePush	(PAGE_SELECT_NEXT, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 498, 411);
	pkPageSelect->SetPageButtonImageDisable	(PAGE_SELECT_NEXT, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 498, 425);

	pkPageSelect->SetPageButtonSize(PAGE_SELECT_BUNDLE_NEXT, 14, 14);
	pkPageSelect->SetPageButtonImage		(PAGE_SELECT_BUNDLE_NEXT, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 469, 455);
	pkPageSelect->SetPageButtonImageHit		(PAGE_SELECT_BUNDLE_NEXT, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 469, 469);
	pkPageSelect->SetPageButtonImagePush	(PAGE_SELECT_BUNDLE_NEXT, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 469, 483);
	pkPageSelect->SetPageButtonImageDisable	(PAGE_SELECT_BUNDLE_NEXT, "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 469, 497);

	pkPageSelect->SetNumberColor(RGBA(69, 94, 112, 255));
	pkPageSelect->SetControlInterval(12);
	pkPageSelect->SetNumberSize(21, 12);
	pkPageSelect->SetMaxPageInfo(1) ;		// 실제 보여질 페이지 설정


	// 하단 메뉴
	// [나가기]
	pWindowButton	=	new	SPWindowButton( WIID_AUCTION_BUY_BT_CLOSE , 700 , 386 , 68 , 18 , m_pWindowBuy );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 208 , 289 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 208 , 307 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 208 , 325 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 208 , 343 );
}


void SPWindowAuction::ShowAuctionBuy()
{
	m_pWindowSearch->Hide();
	HidePopMenu() ;
	m_pWindowSale->Hide();

	m_pWindowBuy->Show();

	// 페이지를 보여줄때 추가된 데이터가 있을수 있으므로 전체 Update
	OnReceiveAuctionBuyList(0, 0) ;
	InitTabStop() ;
}


/**
	Archive로부터 페이지에 해당하는 데이터를 가져와서 리스트에 출력한다.
	@param int iPage
*/
void SPWindowAuction::ShowBuyList(int iPage)
{
	SPAuctionArchive* pArchive ;
	std::vector<SPAuctionAttr*> vShowList ;
	pArchive = g_pkAuctionManager->GetAuctionBuyArchive() ;

	if( pArchive == NULL )	{
		ErrorFLogs->Error_Log("SPWindowAuction::ShowBuyList Archive is NULL") ;
		return ;
	}

	vShowList.clear() ;
	int iSize = pArchive->GetAuctionListPage(vShowList, iPage) ;
	if( iSize == 0 )	{
		ErrorFLogs->Error_Log("SPWindowAuction::OnReceiveAuctionBuyList has No data!!!") ;
	}
	//ErrorFLogs->Error_Log("SPWindowAuction::OnReceiveAuctionBuyList Show[%d], Page[%d]", iSize, iPage) ;
	for( int i = 0; i < AUCTION_BUY_LIST; i++ )
	{
		if( i < iSize )
		{
			m_pWindowBuyList[i]->SetAuctionItem(vShowList[i]) ;
			m_pWindowBuyList[i]->SetOutBid() ;
			m_pWindowBuyList[i]->SetShowEnable(true) ;
			m_pWindowBuyList[i]->Show() ;
		}
		else
		{
			m_pWindowBuyList[i]->ClearAuctionInfo() ;
			m_pWindowBuyList[i]->SetShowEnable(false) ;
			m_pWindowBuyList[i]->Hide() ;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
/**
	리스트 카데고리별 정렬
*/
int SPWindowAuction::OnSortBuyQuality(WPARAM wParam, LPARAM lParam)
{
	//ErrorFLogs->Error_Log("SPWindowAuction::OnSortBuyQuality") ;
	if( g_pkAuctionManager->GetAuctionBuyArchive()->GetListCount() > 0 )	{
		if( g_pkAuctionManager->GetAuctionBuyArchive()->SetSort(AUCTION_SORT_QUALITY) )
		{
			UpdateSortArrow(AUCTION_BUY_TAB) ;
			ShowBuyList(m_iBuyCurPage) ;
		}
	}
	return 0 ;
}

int SPWindowAuction::OnSortBuyLevel(WPARAM wParam, LPARAM lParam)
{
	//ErrorFLogs->Error_Log("SPWindowAuction::OnSortBuyLevel") ;
	if( g_pkAuctionManager->GetAuctionBuyArchive()->GetListCount() > 0 )	{
		if( g_pkAuctionManager->GetAuctionBuyArchive()->SetSort(AUCTION_SORT_ITEM_LV) )
		{
			UpdateSortArrow(AUCTION_BUY_TAB) ;
			ShowBuyList(m_iBuyCurPage) ;
		}
	}
	return 0 ;
}

int SPWindowAuction::OnSortBuyDuration(WPARAM wParam, LPARAM lParam)
{
	//ErrorFLogs->Error_Log("SPWindowAuction::OnSortBuyDuration") ;
	if( g_pkAuctionManager->GetAuctionBuyArchive()->GetListCount() > 0 )	{
		if( g_pkAuctionManager->GetAuctionBuyArchive()->SetSort(AUCTION_SORT_ITEM_OPTPOINT) )
		{
			UpdateSortArrow(AUCTION_BUY_TAB) ;
			ShowBuyList(m_iBuyCurPage) ;
		}
	}
	return 0 ;
}

int SPWindowAuction::OnSortBuyFinish(WPARAM wParam, LPARAM lParam)
{
	//ErrorFLogs->Error_Log("SPWindowAuction::OnSortBuyFinish") ;
	if( g_pkAuctionManager->GetAuctionBuyArchive()->GetListCount() > 0 )	{
		if( g_pkAuctionManager->GetAuctionBuyArchive()->SetSort(AUCTION_SORT_DURATION) )
		{
			UpdateSortArrow(AUCTION_BUY_TAB) ;
			ShowBuyList(m_iBuyCurPage) ;
		}
	}
	return 0 ;
}

int SPWindowAuction::OnSortBuyPrice(WPARAM wParam, LPARAM lParam)
{
	//ErrorFLogs->Error_Log("SPWindowAuction::OnSortBuyPrice") ;
	if( g_pkAuctionManager->GetAuctionBuyArchive()->GetListCount() > 0 )	{
		if( g_pkAuctionManager->GetAuctionBuyArchive()->SetSort(AUCTION_SORT_PRICE) )
		{
			UpdateSortArrow(AUCTION_BUY_TAB) ;
			ShowBuyList(m_iBuyCurPage) ;
		}
	}
	return 0 ;
}


/**
	리스트 [입찰]클릭
*/
int SPWindowAuction::OnBuyListBidClick(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	SPAuctionAttr* pItemAttr = NULL ;
	int iSelectRow = (iID - WIID_AUCTION_BUY_LIST) / AUCTION_MAX_LIST_ITEM ;
	if( iSelectRow < 0 || iSelectRow > AUCTION_BUY_LIST )
		return 0 ;

	if( iSelectRow < AUCTION_BUY_LIST && m_pWindowBuyList[iSelectRow]->GetAuctionAttr() == NULL )	{
		ErrorFLogs->Error_Log("SPWindowAuction::OnBuyListBidClick[%d] is NoDATA", iSelectRow) ;
		return 0 ;
	}
	else	{
		pItemAttr = m_pWindowBuyList[iSelectRow]->GetAuctionAttr() ;
		if( CheckAuctionBidable(pItemAttr, true) == false )		{
			m_pWindowBuyList[iSelectRow]->SetFinishTime() ;
			m_pWindowBuyList[iSelectRow]->UpdateBidButtonEnable() ;
			return 0 ;
		}

		if( pItemAttr->GetType() == AUCTION_TYPE_CASH_BY_ELY || pItemAttr->GetType() == AUCTION_TYPE_MILEAGE_BY_ELY )
			m_pAuctionMsg->SPSendMessage(SPIM_AUCTION_FEE_MSG, AUCTION_MSG_BID, (LPARAM)pItemAttr) ;
		else
			m_pAuctionMsg->SPSendMessage(SPIM_AUCTION_BID_MSG, AUCTION_MSG_BID, (LPARAM)pItemAttr);
	}

	return 0 ;
}
/**
	리스트 [즉시구매]클릭
*/
int SPWindowAuction::OnBuyListSpotClick(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	SPAuctionAttr* pItemAttr = NULL ;
	int iSelectRow = (iID - WIID_AUCTION_BUY_LIST) / AUCTION_MAX_LIST_ITEM ;
	if( iSelectRow < 0 || iSelectRow > AUCTION_BUY_LIST )
		return 0 ;

	int iSearchIndex = m_iSearchCurPage * AUCTION_SEARCH_LIST ;
	if( iSelectRow < AUCTION_BUY_LIST && m_pWindowBuyList[iSelectRow]->GetAuctionAttr() == NULL )	{
		ErrorFLogs->Error_Log("SPWindowAuction::OnBuyListSpotClick[%d] is NoDATA", iSelectRow) ;
		return 0 ;
	}
	else	{
		pItemAttr = m_pWindowBuyList[iSelectRow]->GetAuctionAttr() ;
		if( CheckAuctionBidable(pItemAttr, false) == false )	{
			m_pWindowBuyList[iSelectRow]->SetFinishTime() ;
			m_pWindowBuyList[iSelectRow]->UpdateBidButtonEnable() ;
			return 0 ;
		}

		if( pItemAttr->GetType() == AUCTION_TYPE_CASH_BY_ELY || pItemAttr->GetType() == AUCTION_TYPE_MILEAGE_BY_ELY )
			m_pAuctionMsg->SPSendMessage(SPIM_AUCTION_FEE_MSG, AUCTION_MSG_SPOT, (LPARAM)pItemAttr) ;
		else
			m_pAuctionMsg->SPSendMessage(SPIM_AUCTION_BID_MSG, AUCTION_MSG_SPOT, (LPARAM)pItemAttr);
	}
	return 0 ;
}


/**
	페이지 선택
*/
int SPWindowAuction::OnBuyListPage(WPARAM wParam, LPARAM lParam)
{
	int iPage = (int)wParam ;
	int iSelectBundle = (int)lParam ;
	int iSelectPage = iPage ;

	// 해당 페이지의 데이터를 가지고 있으면 Manager에서 가져오고
	// 없으면 패킷을 보내서 서버에 요청한다.
	if( g_pkAuctionManager->GetAuctionBuyArchive()->HasPageInfo(iSelectPage) )
	{
		m_iBuyCurPage = iSelectPage ;
		ShowBuyList(m_iBuyCurPage) ;		// BuyList Update
	}
	else
	{
		ErrorFLogs->Error_Log("BuyArchive Hasn't Info [%d]Page", iSelectPage) ;
	}

	return 0 ;
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
/**
	리스트 데이터 처리
*/
int SPWindowAuction::OnReceiveAuctionBuyList(WPARAM wParam, LPARAM lParam)
{
	SPAuctionArchive* pArchive ;
	std::vector<SPAuctionAttr*> vShowList ;

	m_pAuctionMsg->SPSendMessage(SPIM_AUCTION_WAIT_CLOSE, 0, 0) ;
	pArchive = g_pkAuctionManager->GetAuctionBuyArchive() ;

	UpdateSortArrow(AUCTION_BUY_TAB) ;

	// Page Setting
	int iMaxPage = pArchive->GetTotalPageCount() ;
	if( iMaxPage < 1 )		iMaxPage = 1 ;
	SPWindowPageSelect* pkPageSelect ;
	pkPageSelect = (SPWindowPageSelect*)Find(WIID_AUCTION_BUY_PAGE, true);
	if( pkPageSelect )	{
		//ErrorFLogs->Error_Log("Set Auction BUY Page[%d]", iMaxPage) ;
		pkPageSelect->SetMaxPageInfo(iMaxPage) ;		// 실제 보여질 페이지 설정
		pkPageSelect->SetSelectPage((m_iBuyCurPage%10)) ;
	}

	if( IsShow() && m_iSelectedTab == AUCTION_BUY_TAB )
	{
		// List Setting
		ShowBuyList(m_iBuyCurPage) ;
		return 1 ;
	}

	return 0 ;
}

///**
//	리스트 업데이트 처리
//*/
//int SPWindowAuction::OnReceiveAuctionBuyUpdate(WPARAM wParam, LPARAM lParam)
//{
//	//ErrorFLogs->Error_Log("SPWindowAuction::OnReceiveAuctionBuyUpdate") ;
//	return 0 ;
//}

