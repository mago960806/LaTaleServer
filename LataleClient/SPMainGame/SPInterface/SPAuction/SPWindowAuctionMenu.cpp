// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowAuctionRegist.cpp
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
#include "SPWindowRowUNIT.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPAuctionDef.h"
#include "SPAuctionAttr.h"
#include "SPAuctionArchive.h"
#include "SPAuctionManager.h"

#include "SPWindowAuction.h"


void SPWindowAuction::InitAuctionPopMenu()
{
	SPWindowStatic*	pWindowStaticBG;

	pWindowStaticBG	=	new SPWindowStatic( WIID_AUCTION_POPUP_FRAME , 200 , 52 , 109 , 50 , m_pWindowSearch );
	pWindowStaticBG->SetWindowType(WND_TYPE_NINEBASE) ;
	pWindowStaticBG->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 82, 1, 4, 1) ;
	m_pWindowSearchPopup = (SPWindow*)pWindowStaticBG ;
}

/**
	팝업으로 선택되는 배경 생성 Animation Setting
	이전 선택된 내용이 보여지도록 한다.
*/
void SPWindowAuction::SetPopMenu(int iSelCategory)
{
	// 이전 메뉴 삭제
	m_pWindowSearchPopup->CleanChildAll() ;
	m_vpPopMenu.clear() ;

	int iMenuStartX = 726 - m_iSearchMenuMax*64 ;		// 776 - m_iSearchMenuMax*64 ;
	int iPopStartX = iMenuStartX + iSelCategory * 64 ;
	int iPopSize = (int)m_stMenu[iSelCategory].vpDetail.size() ;

	m_iPopMenuMode = 1 ;		// PopMenu Increase
	m_iPopMenuSize = 10 ;
	m_iPopMenuDestSize = iPopSize*20 + 4 ;

	m_iPreShowPopup = iSelCategory ;		// 선택 저장

	m_pWindowSearchPopup->SetRectSize(109, m_iPopMenuSize) ;
	m_pWindowSearchPopup->AdjustCoord(iPopStartX, 52) ;
}


/**
	팝업으로 선택되는 메뉴출력
	이전 선택된 내용이 보여지도록 한다.
*/
void SPWindowAuction::ShowPopMenu(int iSelCategory)
{
	SPWindowRowUNIT* pWindowRowUnit;

	m_pWindowSearchPopup->CleanChildAll() ;
	m_vpPopMenu.clear() ;

	int iMenuStartX = 726 - m_iSearchMenuMax*64 ;
	int iPopStartX = iMenuStartX + iSelCategory * 64 ;
	int iPopSize = (int)m_stMenu[iSelCategory].vpDetail.size() ;
	RECT rcLeft, rcMiddle, rcRight ;

	// Set Select Box
	SetRect( &rcLeft,	49, 38, 53, 56) ;
	SetRect( &rcMiddle, 54, 38, 58, 56) ;
	SetRect( &rcRight,	59, 38, 63, 56) ;

	// Popup 창의 크기 및 위치 재설정
	m_pWindowSearchPopup->SetRectSize(109, iPopSize*20 + 4) ;
	m_pWindowSearchPopup->AdjustCoord(iPopStartX, 52) ;
	m_pWindowSearchPopup->RefreshChildPos() ;


	for( int i = 0; i < iPopSize; i++)
	{
		pWindowRowUnit = new SPWindowRowUNIT( WIID_AUCTION_POPUP_LIST + (i * 3), 4, 2 + i*20, 101, 18, m_pWindowSearchPopup) ;
		pWindowRowUnit->SetToolTipStr("PopUp List") ;

		pWindowRowUnit->SetIconImagePos( 8 , 1 );
		pWindowRowUnit->SetIconImageSize( 16 , 16 );
		pWindowRowUnit->SetIconImage( m_strMenuIconPath.c_str() , m_stMenu[iSelCategory].vpDetail[i]->rcIconSrc.left , m_stMenu[iSelCategory].vpDetail[i]->rcIconSrc.top );

		pWindowRowUnit->SetTextPos( 28 , 4 );
		pWindowRowUnit->SetTextSize( 73 , 12 );

		pWindowRowUnit->SetTextColor( RGBA(143, 99, 99, 255) );
		pWindowRowUnit->SetOnTextColor( RGBA(255, 255, 255, 255) );
		pWindowRowUnit->SetTextString( m_stMenu[iSelCategory].vpDetail[i]->strDetail.c_str()) ;

		pWindowRowUnit->SetSelectBG("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", &rcLeft, &rcMiddle, &rcRight) ;

		pWindowRowUnit->ShowToolTip( false );
		pWindowRowUnit->RefreshRelationCoord() ;

		m_vpPopMenu.push_back(pWindowRowUnit) ;
	}

	// 새로 생성해도 카테고리의 이전 선택은 보여준다.
	if( m_iSelectedCategory == iSelCategory )	{
		if( m_iSelectedDetail >= 0)	{
			pWindowRowUnit = m_vpPopMenu[m_iSelectedDetail] ;
			pWindowRowUnit->SetWndState(WND_STATE_CLICK) ;
		}
	}

	m_iPopMenuMode = 2 ;					// On Show
	m_iPreShowPopup = iSelCategory ;		// 현재 메뉴를 이전 선택 메뉴로 저장
	m_pWindowSearchPopup->Show() ;
}


void SPWindowAuction::HidePopMenu()
{
	m_iPopMenuMode = 0 ;					// Hide
	if( m_pWindowSearchPopup->IsShow() )
		m_pWindowSearchPopup->Hide() ;

	// 팝업이 닫히면 메뉴도 이전대로
	SPWindowButton* pButton ;
	SPWindowStatic* pStatic ;
	pButton = (SPWindowButton*)Find(WIID_AUCTION_SEARCH_MENU_BUTTON+m_iPreShowPopup, true) ;	// 이전선택 Check 해제
	if( pButton )		pButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	pStatic = (SPWindowStatic*)Find(WIID_AUCTION_SEARCH_MENU_TEXT+m_iPreShowPopup, true) ;
	if( pStatic )		pStatic->SetFontColor( RGBA(143, 99, 99, 255) );
}


