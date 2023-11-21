// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowAuctionRegist.cpp
//	created:	2008/2/26   Time:10:59
//	purpose:	
//*********************************************************************/


#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"

#include "SPCheckManager.h"

#include "SPWindowToolTip.h"

#include "PacketID.h"

#include "SPAuctionDef.h"
#include "SPAuctionAttr.h"
#include "SPAuctionArchive.h"
#include "SPAuctionManager.h"

#include "SPCheckManager.h"
#include "SPWindowAuctionListUnit.h"
#include "GlobalDefines_Share.h"
#include "_Error_log.h"


SPWindowAuctionListUNIT::SPWindowAuctionListUNIT(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_AUCTION , InstanceID, iX, iY, iCX, iCY, pParent )	
, m_pBGTexture(NULL)
, m_pDisTexture(NULL)
, m_pAuctionAttr(NULL)
, m_pItemInfo(NULL)
{
	Init();
}

SPWindowAuctionListUNIT::~SPWindowAuctionListUNIT()
{
	Clean();
}

void SPWindowAuctionListUNIT::Init()
{
	m_strToolTip	=	"[SPWindowAuctionListUNIT]";
	m_bShowToolTip	= false ;
	m_bSelectAble	= true ;
	m_bExpired		= false ;
	m_bUsable		= true ;
	m_iBidEnable	= BID_ALLENABLE ;
	m_iPriceType	= 0 ;	// by Ely

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , &m_pBGTexture );
	ReposOwnImage();
	SetRect( &m_rcBGSrc		, 83 , 43 , 115 , 45 );		//	선택배경
	SetRect( &m_rcLineSrc	, 83 , 47 , 115 , 48 );		//	Line
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" , &m_pDisTexture );
	SetRect( &m_rcDisSrc	, 434 , 37 , 436 , 39);

	int	iInstanceID	=	m_InstanceID + 1;
	m_iStartID		=	iInstanceID;

	SPWindowButton*	pWindowButton	=	NULL;
	SPWindowStatic*	pWindowStatic	=	NULL;
	SPWindowStatic* pWindowItem		=	NULL ;
	SPWindowStatic*	pWindowStaticBG	=	NULL;

	pWindowStaticBG	=	new	SPWindowStatic( iInstanceID , 8 , 5 , 32 , 32 , this );		// Item BG (+1) == m_iStart
	pWindowStaticBG->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 1 , 34 );

	iInstanceID++;
	pWindowItem	=	new	SPWindowStatic( iInstanceID , 0 , 0 , 32 , 32 , pWindowStaticBG );		// Item (+2)
	m_iItemIcon = iInstanceID ;

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , -2 , 20 , 9 , 10 , pWindowItem );		// ItemCount10(+3)
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 0, 0);	// 0
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 9, 0);	// 1
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 18, 0);	// 2
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 27, 0);	// 3
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 36, 0);	// 4
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 45, 0);	// 5
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 54, 0);	// 6
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 63, 0);	// 7
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 72, 0);	// 8
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 81, 0);	// 9
	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 5 , 20 , 9, 10, pWindowItem );		// ItemCount01(+4)
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 0, 0);	// 0
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 9, 0);	// 1
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 18, 0);	// 2
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 27, 0);	// 3
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 36, 0);	// 4
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 45, 0);	// 5
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 54, 0);	// 6
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 63, 0);	// 7
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 72, 0);	// 8
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_num.PNG" , 81, 0);	// 9


	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 49 , 16 , 155 , 12 , this );	// Item Text(+5)
	pWindowStatic->SetFormat(DT_TOP | DT_CENTER) ;
	pWindowStatic->SetWindowText("Hello~~") ;

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 150 , 16 , 17, 11 , this );	// Money Icon(+6)
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 157 , 22 );	// Ely
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 193 , 22 );	// Oz
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 211 , 22 );	// Point
	pWindowStatic->SetShowEnable(false) ;

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 213 , 16 , 42 , 12 , this );	// Level Text(+7)
	pWindowStatic->SetFontColor( RGBA(0, 0, 0, 255) ) ;
	pWindowStatic->SetFormat(DT_TOP | DT_CENTER) ;
	pWindowStatic->SetWindowText("Lv90") ;

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 259 , 16 , 60 , 12 , this );	// Duration Text(+8)
	pWindowStatic->SetFontColor( RGBA(0, 0, 0, 255) ) ;
	pWindowStatic->SetFormat(DT_TOP | DT_CENTER) ;
	pWindowStatic->SetWindowText("Good") ;

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 323 , 16 , 67 , 12 , this );	// FinishTime Text(+9)
	pWindowStatic->SetFontColor( RGBA(0, 0, 0, 255) ) ;
	pWindowStatic->SetFormat(DT_TOP | DT_CENTER) ;
	pWindowStatic->SetWindowText("VeryLong") ;

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 394 , 16 , 112 , 12 , this );	// Seller Text(+10)
	pWindowStatic->SetFontColor( RGBA(0, 0, 0, 255) ) ;
	pWindowStatic->SetFormat(DT_TOP | DT_CENTER) ;
	pWindowStatic->SetWindowText("Sellers") ;

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 510 , 8 , 74 , 13 , this );	// BidUser Text(+11)
	pWindowStatic->SetFontColor( RGBA(143, 99, 99, 255) ) ;
	pWindowStatic->SetFormat(DT_TOP | DT_LEFT) ;

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 587 , 9 , 83 , 12 , this );	// BidPrice Text(+12)
	pWindowStatic->SetFontColor( RGBA(143, 99, 99, 255) ) ;
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT) ;
	pWindowStatic->SetWindowText("999,999,999,999") ;
	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 674 , 8 , 17 , 11 , this );	// BidPrice Icon(+13)
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 157 , 22 );	// Ely
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 193 , 22 );	// Oz
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 211 , 22 );	// Point

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 587 , 28 , 83 , 12 , this );	// SpotPrice Text(+14)
	pWindowStatic->SetFontColor( RGBA(0, 0, 0, 255) ) ;
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT) ;
	pWindowStatic->SetWindowText("999,999,999,999") ;
	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 674 , 27 , 17 , 11 , this );	// SpotPrice Icon(+15)
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 157 , 22 );	// Ely
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 193 , 22 );	// Oz
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 211 , 22 );	// Point

	iInstanceID++;
	pWindowButton	=	new	SPWindowButton( iInstanceID , 700 , 3 , 68 , 17 , this );	// Bid Button (+16)
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 289 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 307 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 325 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 70 , 343 );
	m_iBidButton = iInstanceID ;
	iInstanceID++;
	pWindowButton	=	new	SPWindowButton( iInstanceID , 700 , 22 , 68 , 17 , this );	// SpotBuy Button Icon(+17)
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 187 , 358 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 187 , 376 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 187 , 394 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 187 , 412  );
	m_iSpotButton = iInstanceID ;

	m_iEndID	=	iInstanceID; // (+17)
}

void SPWindowAuctionListUNIT::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
	SAFE_RELEASE( m_pDisTexture );
}

void SPWindowAuctionListUNIT::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorEnter( NULL , NULL );
	}
}

void SPWindowAuctionListUNIT::Hide(bool bSendServer)
{
	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorOut( NULL , NULL );
	}
}

void SPWindowAuctionListUNIT::ReposOwnImage()
{
	SetRect( &m_rcBGDest	, m_iAX		, m_iAY		, m_iAX + 776	, m_iAY + 42 );		//	BG
	SetRect( &m_rcLineDest	, m_iAX		, m_iAY+42	, m_iAX + 776	, m_iAY + 43 );		//	Line
	SetRect( &m_rcDisDest	, m_iAX+8	, m_iAY+5	, m_iAX + 40	, m_iAY + 37);		//	Disable
}

void SPWindowAuctionListUNIT::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowAuctionListUNIT::Process(float fTime)
{
	SPWindow::Process( fTime );
}

void SPWindowAuctionListUNIT::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_WindowState == WND_STATE_CLICK || m_WindowState == WND_STATE_HITTEST )
	{
		if( m_pBGTexture )
		{
			m_pBGTexture->SetColor();
			m_pBGTexture->RenderTexture( &m_rcBGDest , &m_rcBGSrc );
			g_pVideo->Flush();
		}
	}
	if( m_pBGTexture )
	{
		m_pBGTexture->RenderTexture( &m_rcLineDest , &m_rcLineSrc );
		g_pVideo->Flush();
	}

	SPWindow::Render(fTime);

	if( !m_bUsable || m_bExpired )	{
		if( m_pDisTexture )	{
			m_pDisTexture->RenderTexture( &m_rcDisDest , &m_rcDisSrc );
		}
	}

	g_pVideo->Flush();
}



//////////////////////////////////////////////////////////////////////////
// 아이템 설정
void	SPWindowAuctionListUNIT::SetItemField( SPItem* pItem)
{
	m_iAuctionType = AUCTION_TYPE_ITEM_BY_ELY ;
	m_iBidEnable = BID_ALLENABLE ;
	m_iPriceType = 0 ;		// Ely

	if( pItem == NULL )	{
		ErrorFLogs->Error_Log("SPWindowAuctionListUNIT::SetItemField ## Item is NULL!!!") ;
		return ;
	}

	m_pItemInfo = pItem ;		// 아이템 정보 저장

	// Resize Item Name
	// Hide Money Icon
	POINT ptIcon ;
	D3DXCOLOR cItemColor;
	SPTexture* pIcon ;
	SPWindowStatic* pWindowStatic ;
	pWindowStatic	=	(SPWindowStatic*)Find( m_iItemIcon, true ) ;		// 49 , 16 , 155 , 12 , this );	// Item (+2)
	pIcon = g_pItemCluster->GetItemTexture(m_pItemInfo->GetItemStatus()->GetItemID(), ptIcon) ;
	pWindowStatic->SPSendMessage(SPIM_SET_IMAGECLEAR) ;
	pWindowStatic->SetImage( pIcon, ptIcon.x, ptIcon.y ) ;

	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+4, true ) ;		// 49 , 16 , 155 , 12 , this );	// Item Text(+5)
	pWindowStatic->SetRectSize(155, 12) ;
	pWindowStatic->SetFormat(DT_CENTER) ;
	if( pItem->GetItemAttr()->m_bNameColor == true )
		cItemColor	=	pItem->GetItemAttr()->GetNameColor();
	else
		cItemColor	=	g_pCheckManager->GetItemColor( pItem->GetItemStatus()->GetRareLevel() );
	pWindowStatic->SetFontColor( cItemColor ) ;
	pWindowStatic->SetWindowText(pItem->GetItemStatus()->GetItemAttr()->m_strName.c_str() ) ;
	//ErrorFLogs->Error_Log("Auction Set Item [%s][%d]", pItem->GetItemStatus()->GetItemAttr()->m_strName.c_str(), m_pAuctionAttr->GetItemQuality() ) ;

	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+5, true ) ;		// 150 , 16 , 17 , 12 , this );	// MoneyIcon(+6)
	pWindowStatic->SetShowEnable(false) ;

	int iStackCount = pItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
	SetItemCount(iStackCount) ;

	// BidPrice, SpotPrice Icon
	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+12, true ) ;		// 입찰가 Icon (+13)
	pWindowStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+14, true ) ;		// 즉구가 Icon (+15)
	pWindowStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;

	// Item 상태 처리(Disable/ Enable)
	m_bUsable = g_pCheckManager->CheckItemStatus( m_pItemInfo->GetItemStatus()->GetItemID() ) ;
}

/**
	@param int iType	: 판매 머니 타입
	@param INT64 iMoney : 판매 머니
 */
void	SPWindowAuctionListUNIT::SetItemField( int iType, INT64 iMoney )
{
	int iPriceIcon = 0 ;
	int iSaleIcon = 0 ;
	SPWindowStatic* pWindowStatic ;

	m_iAuctionType = iType ;
	m_iBidEnable = BID_ALLENABLE ;
	m_pItemInfo = NULL ;		// 아이템 정보초기화

	pWindowStatic	=	(SPWindowStatic*)Find( m_iItemIcon, true ) ;		// 49 , 16 , 155 , 12 , this );	// Item (+2)
	pWindowStatic->SPSendMessage(SPIM_SET_IMAGECLEAR) ;
	SetItemCount(0) ;

	// 타입별 머니 아이콘 셋팅
	if( m_iAuctionType == AUCTION_TYPE_ELY_BY_CASH ) {
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 204 , 63 );	// Ely
		iSaleIcon	= 0 ;		// Ely
		iPriceIcon	= 1 ;		// Cash
	}
	else if ( m_iAuctionType == AUCTION_TYPE_CASH_BY_ELY)	{
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 171 , 63 );	// Cash
		iSaleIcon	= 1 ;		// Cash
		iPriceIcon	= 0 ;		// Ely
	}
	else if ( m_iAuctionType == AUCTION_TYPE_ELY_BY_MILEAGE)	{
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 204 , 63 );	// Ely
		iSaleIcon	= 0 ;		// Ely
		iPriceIcon	= 2 ;		// Point
	}
	else	{	// m_iAuctionType == AUCTION_TYPE_MILEAGE_BY_ELY
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 237 , 63 );	// Point
		iSaleIcon	= 2 ;		// Point
		iPriceIcon	= 0 ;		// Ely
	}

	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+12, true ) ;		// 입찰가 Icon (+13)
	pWindowStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, iPriceIcon) ;
	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+14, true ) ;		// 즉구가 Icon (+15)
	pWindowStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, iPriceIcon) ;

	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+4, true );		// 49 , 16 , 97 , 12 , this );	// Item Text(+5)
	pWindowStatic->SetRectSize(97, 12) ;
	pWindowStatic->SetFormat(DT_RIGHT) ;
	pWindowStatic->SetFontColor( RGBA(0, 0, 0, 255) ) ;
	pWindowStatic->SetWindowText( ConvertMoneyToString(iMoney) ) ;

	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+5, true );		// 150 , 16 , 17 , 12 , this );	// MoneyIcon(+6)
	pWindowStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, iSaleIcon) ;
	pWindowStatic->SetShowEnable(true) ;
}


/**
	Item의 StackCount를 출력한다.
 */
void SPWindowAuctionListUNIT::SetItemCount(int iCount)
{
	SPWindow* pWindow10 = NULL ;
	SPWindow* pWindow01 = NULL ;
	int iTen = iCount / 10 ;
	int iOne = iCount % 10 ;
	pWindow10 = Find( m_iStartID + 2, true ) ;		// ItemCount(+3)
	pWindow01 = Find( m_iStartID + 3, true ) ;		// ItemCount(+4)

	if( iCount > 1 )	
	{
		if( iTen > 0 )	{
			pWindow10->SPSendMessage(SPIM_SET_IMAGEINDEX, iTen) ;	
			pWindow01->SPSendMessage(SPIM_SET_IMAGEINDEX, iOne) ;	
			pWindow10->SetShowEnable(true) ;
			pWindow01->SetShowEnable(true) ;
		}
		else	{	// 10단위가 없으면 1단위가 앞으로
			pWindow10->SPSendMessage(SPIM_SET_IMAGEINDEX, iOne) ;	
			pWindow10->SetShowEnable(true) ;
			pWindow01->SetShowEnable(false) ;
		}
	}
	else	
	{
		pWindow10->SetShowEnable(false) ;
		pWindow01->SetShowEnable(false) ;
	}
}


/**
	레벨 Text 및 Font Setting
*/
void	SPWindowAuctionListUNIT::SetLevel( int iLevel )
{
	TCHAR szBuf[16] ;
	sprintf( szBuf, "%d", iLevel) ;
	SPWindow* pWindow = Find( m_iStartID + 6, true ) ;		// Level Text(+7)
	if( pWindow )	{
		pWindow->SetWindowText(szBuf) ;
		//ErrorFLogs->Error_Log("Auction Set Level [%s]", szBuf) ;
	}
}

/**
	내구도 Text 및 Font Setting
*/
void	SPWindowAuctionListUNIT::SetDuration( int iDuration )
{
	TCHAR szBuf[16] ;
	sprintf( szBuf, "%d", iDuration) ;
	SPWindow* pWindow = Find(m_iStartID + 7, true) ;			// Duration Text(+8)
	if( pWindow )	{
		pWindow->SetWindowText(szBuf) ;
		//ErrorFLogs->Error_Log("Auction Set Duration [%s]", szBuf) ;
	}
}

/**
	마감시간 Text 및 Font Setting
*/
void	SPWindowAuctionListUNIT::SetFinishTime()
{
	TCHAR szBuf[16] ;
	UINT32 iRemainTime ;

	iRemainTime =  m_pAuctionAttr->GetFinishTime() ;
	m_bExpired = false ;

	if( iRemainTime < 1 )		// 만료
	{
		if( g_pResourceManager->GetGlobalString(52000107) )
			sprintf( szBuf, "%s", g_pResourceManager->GetGlobalString(52000107)) ;
		else
			sprintf( szBuf, "%s", "Expired") ;
		m_bExpired = true ;
	}
	else if(iRemainTime <= g_pkAuctionManager->GetAuctionTimeShort() )			// (단기)
	{
		if( g_pResourceManager->GetGlobalString(52000046) )
			sprintf( szBuf, "%s", g_pResourceManager->GetGlobalString(52000046)) ;
		else
			sprintf( szBuf, "%s", "Short") ;
	}
	else if(iRemainTime <= g_pkAuctionManager->GetAuctionTimeMiddle() )		// (중기)
	{
		if( g_pResourceManager->GetGlobalString(52000056) )
			sprintf( szBuf, "%s", g_pResourceManager->GetGlobalString(52000056)) ;
		else
			sprintf( szBuf, "%s", "Middle") ;
	}
	else	// 장기
	{
		if( g_pResourceManager->GetGlobalString(52000047) )
			sprintf( szBuf, "%s", g_pResourceManager->GetGlobalString(52000047)) ;
		else
			sprintf( szBuf, "%s", "Long") ;
	}
	SPWindow* pWindow = Find(m_iStartID + 8, true) ;		// FinishTime(+9)
	if( pWindow )	{
		pWindow->SetWindowText(szBuf) ;
	}
}

/**
	판매자 Text 및 Font Setting
*/
void	SPWindowAuctionListUNIT::SetSeller( const char* szSeller, USERID iSellUser )
{
	TCHAR szBuf[16] ;

	if( szSeller != NULL )	{
		SPWindow* pWindow = Find(m_iStartID + 9, true) ;			// 판매자(+10)
		if( pWindow )	{
			pWindow->SetWindowText(szSeller) ;
			//ErrorFLogs->Error_Log("Auction Set Seller [%s]", szSeller) ;
		}

		// [본인물품] 표시
		if( g_pCheckManager->CheckBidEnable(szSeller) == false ) {
			m_iBidEnable = BID_OWN ;		// 본인 물품
			sprintf( szBuf, "%s", g_pResourceManager->GetGlobalString(52000045)) ;
		}
		else if( g_pCheckManager->CheckBidEnable(iSellUser) == false )	{
			m_iBidEnable = BID_OWN ;		// 본인 물품
			sprintf( szBuf, "%s", g_pResourceManager->GetGlobalString(52000045)) ;
		}
		else	{
			sprintf( szBuf, "") ;
		}

		pWindow = Find(m_iStartID + 10, true) ;		// Bid User(+11)
		if( pWindow )	{
			pWindow->SetWindowText(szBuf) ;
		}
	}
}

/**
	현재 입찰 가격 설정
*/
void	SPWindowAuctionListUNIT::SetPrice( INT64 iBidPrice, INT64 iSpotPrice, USERID iBidUser )
{
	TCHAR szBuf[32] ;
	SPWindow* pWindow = NULL ;
	//SPWindowButton* pButton = NULL ;
	SPWindowStatic* pStatic = NULL ;
	bool bBid = false ;
	bool bSpot = true ;

	// [본인입찰]표시
	if( g_pCheckManager->CheckBidEnable(iBidUser) == false )
	{
		m_iBidEnable = BID_SELF ;
		if( g_pResourceManager->GetGlobalString(52000044) )		// 본인 입찰
			sprintf( szBuf, "%s", g_pResourceManager->GetGlobalString(52000044)) ;
		else
			sprintf( szBuf, "%s", "My Bid") ;

		pWindow = Find(m_iStartID + 10, true) ;		// Bid User (+11)
		if( pWindow )	{
			pWindow->SetWindowText(szBuf) ;
		}
	}

	pWindow = Find(m_iStartID + 11, true) ;						// Bid Price(+12)
	//	입찰 가격이 있는 경우
	if( iBidPrice > 0 )
	{
		if( pWindow )	 pWindow->SetWindowText( ConvertMoneyToString(iBidPrice) ) ;
	}
	else
	{
		if( pWindow )	pWindow->SetWindowText( ConvertMoneyToString(0) ) ;
	}

	pWindow = Find(m_iStartID + 13, true) ;							// Spot Price(+14)
	// 즉시 구매가 있는 경우
	if( iSpotPrice > 0 )
	{
		if( pWindow ) pWindow->SetWindowText( ConvertMoneyToString(iSpotPrice) ) ;
	}
	else
	{
		if( pWindow ) pWindow->SetWindowText( ConvertMoneyToString(0) ) ;
		if( m_iBidEnable == BID_ALLENABLE )
			m_iBidEnable = BID_ENABLE ;
		else if( m_iBidEnable == BID_SELF )
			m_iBidEnable = BID_DISABLE ;
	}
}


/**
	상태에 따라 Bid/Spot 버튼을 설정한다.
 */
void SPWindowAuctionListUNIT::UpdateBidButtonEnable()
{
	bool bBid = false ;
	bool bSpot = false ;
	SPWindowButton* pButton = NULL ;

	if( m_iBidEnable <= BID_ENABLE  )
		bBid = true ;
	if( m_iBidEnable == BID_SELF || m_iBidEnable == BID_ALLENABLE )
		bSpot = true ;

	if( m_bExpired )	{
		bBid = false ;
		bSpot = false ;
	}

	pButton = (SPWindowButton*)Find(m_iStartID + 15, true) ;	// Bid Button(+16)
	if( pButton )	 pButton->SetShowEnable(bBid) ;

	pButton = (SPWindowButton*)Find( m_iStartID + 16, true ) ;		// Spot Button(+17)
	if( pButton )	 pButton->SetShowEnable(bSpot) ;
}



/**
	BuyList에서는 내가 입찰한것이 아니면 [상회입찰]이 출력되어야 한다.
	[상회입찰]출력을 위한 함수 -> Manager에서 호출 가능해야 한다.
 */
void SPWindowAuctionListUNIT::SetOutBid()
{
	TCHAR szBuf[32] ;
	SPWindow* pWindow = NULL ;

	// [상회입찰]표시
	if( g_pCheckManager->CheckBidEnable( m_pAuctionAttr->GetBidderUserID() ) == true)
	{
		m_iBidEnable = BID_OVER ;
		if( g_pResourceManager->GetGlobalString(52000033) )		// 상회입찰
			sprintf( szBuf, "%s", g_pResourceManager->GetGlobalString(52000033)) ;
		else
			sprintf( szBuf, "%s", "OutBid") ;
	}
	else
	{
		sprintf( szBuf, "") ;
	}

	pWindow = Find(m_iStartID + 10, true) ;		// Bid User (+11)
	if( pWindow )	{
		pWindow->SetWindowText(szBuf) ;
	}
}



/**
	Auction Attribute Setting	
*/
void SPWindowAuctionListUNIT::SetAuctionItem( SPAuctionAttr* pAuctionAttr )
{
	m_iAuctionID = pAuctionAttr->GetAuctionID() ;
	m_pAuctionAttr = pAuctionAttr ;

	if( pAuctionAttr->GetType() == AUCTION_TYPE_ITEM_BY_ELY )	{
		SetItemField( pAuctionAttr->GetItemInfo() ) ;	// Item
		SetLevel( pAuctionAttr->GetItemLevel() ) ;		// 경매 레벨 설정 >> 머니는 기본 0
	}
	else	{
		SetItemField( pAuctionAttr->GetType(), pAuctionAttr->GetSellPoint() ) ;	// Oz, Point, Ely
		SetLevel(0) ;									// 경매 레벨 설정 >> 머니는 기본 0
	}

	SetDuration( pAuctionAttr->GetItemDuration() ) ;	// 내구도 설정
	SetFinishTime() ;	// 마감시간
	SetSeller( pAuctionAttr->GetSellerName(), pAuctionAttr->GetSellerUserID() ) ;		// 판매자
	SetPrice( pAuctionAttr->GetCurBidPrice(), pAuctionAttr->GetSpotPrice(), pAuctionAttr->GetBidderUserID()) ;		// 금액설정
	UpdateBidButtonEnable() ;
}


/**
	Auction 정보 초기화
*/
void SPWindowAuctionListUNIT::ClearAuctionInfo()
{
	m_pAuctionAttr = NULL ;
	m_pItemInfo = NULL ;
	m_iAuctionID = 0 ;
	m_iAuctionType = 0 ;
}



/**
	선택 및 Mouse Over 이미지
*/
void SPWindowAuctionListUNIT::SetSelectBG(char* pTextureFile, RECT* prcSrc)
{
	if( m_pBGTexture )
		SAFE_RELEASE( m_pBGTexture ) ;

	g_pVideo->GetTextureMgr()->LoadTexture( pTextureFile , &m_pBGTexture );

	SetRect( &m_rcBGSrc	, prcSrc->left , prcSrc->top , prcSrc->right , prcSrc->bottom );			//	배경 왼쪽
}

/**
	현재 Window 상태 변경
	WND_STATE_CLICK, WND_STATE_HITTEST <--> WND_STATE_NORMAL
*/
void SPWindowAuctionListUNIT::SetWndState( WND_STATE eWndState )
{
	m_WindowState	=	eWndState;
}




SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowAuctionListUNIT )

SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER		,										OnCursorEnter		)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT			,										OnCursorOut			)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE		,										OnCursorMove		)

SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER	,	m_iStartID	, m_iEndID			,	OnCursorEnter		)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT	,	m_iStartID	, m_iEndID			,	OnCursorOut			)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_MOVE	,	m_iStartID	, m_iEndID			,	OnCursorMove		)

SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	m_iStartID	, m_iStartID + 1	,	OnToolTipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		m_iStartID	, m_iStartID + 1	,	OnToolTipOut)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	m_iStartID	, m_iStartID + 1	,	OnToolTipMove)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	,	m_iStartID	,	m_iEndID		,	OnLButtonUp			)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


/**
Mouse가 영역내에 있으면 m_WindowState를 변경해서 MouseOn Image 처리를 한다.
*/
int SPWindowAuctionListUNIT::OnCursorEnter( WPARAM wParam, LPARAM lParam )
{
	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == FALSE )
		return 1;

	if( m_bSelectAble)
	{
		if( m_WindowState != WND_STATE_CLICK )
			m_WindowState	=	WND_STATE_HITTEST;
	}

	return 1;
}
int SPWindowAuctionListUNIT::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	int	iType	=	(int)wParam;

	if( m_WindowState != WND_STATE_CLICK || iType != 0 )
		m_WindowState	=	WND_STATE_NORMAL;

	return 1;
}
int SPWindowAuctionListUNIT::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == FALSE )
		return 1;

	if( m_bSelectAble)
	{
		if( m_WindowState != WND_STATE_CLICK )
			m_WindowState	=	WND_STATE_HITTEST;
	}

	return 1;
}

int SPWindowAuctionListUNIT::OnCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( iID == m_iStartID || iID == m_iStartID+1 )		// Item or ItemBG
	{
		// Show ToolTip
	}
	OnCursorEnter( wParam , lParam );
	return 1;
}
int SPWindowAuctionListUNIT::OnCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( iID == m_iStartID || iID == m_iStartID+1 )		// Item or ItemBG
	{
		// Out ToolTip
	}
	OnCursorOut( wParam , lParam );
	return 1;
}
int SPWindowAuctionListUNIT::OnCursorMove( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( iID == m_iStartID || iID == m_iStartID+1 )		// Item or ItemBG
	{
		// Move ToolTip
	}
	OnCursorMove( wParam , lParam );
	return 1;
}

/**
	Tooltip
*/
int SPWindowAuctionListUNIT::OnToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	if( m_pItemInfo != NULL )
	{
		SPItemStatus* pItemStatus = m_pItemInfo->GetItemStatus();

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pItemStatus, lParam);
		pWindow->Show();
	}

	return 1;
}

int SPWindowAuctionListUNIT::OnToolTipOut(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	if( m_pItemInfo != NULL )
	{
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		pWindow->Show();
	}
	return 1;
}

int SPWindowAuctionListUNIT::OnToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	if( m_pItemInfo != NULL )
	{
		SPItemStatus* pItemStatus = m_pItemInfo->GetItemStatus();

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		if( !pWindow->IsShow() )	{
			pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)pItemStatus, lParam);
			pWindow->Show();
		}
	}
	return 1;
}


/**
	리스트 클릭 관련 처리
*/
int SPWindowAuctionListUNIT::OnLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( iID == m_iBidButton )			// 입찰
	{
		// 입찰 확인창
		//ErrorFLogs->Error_Log("Bid Button Click") ;
		SPSendMessageToParent( SPIM_AUCTION_BID_BUY , wParam , lParam , this );
	}
	else if ( iID == m_iSpotButton )	// 즉시구매
	{
		// 즉시 구매 확인창
		//ErrorFLogs->Error_Log("Spot Button Click") ;
		SPSendMessageToParent( SPIM_AUCTION_SPOT_BUY , wParam , lParam , this );
	}
	else
	{
		SPSendMessageToParent( SPIM_LBUTTON_UP , wParam , lParam , this );
	}

	return 1;
}


