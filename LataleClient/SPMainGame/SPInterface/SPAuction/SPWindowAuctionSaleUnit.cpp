// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowAuctionSaleUnit.cpp
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
#include "SPWindowAuctionSaleUNIT.h"
#include "GlobalDefines_Share.h"
#include "_Error_log.h"


SPWindowAuctionSaleUNIT::SPWindowAuctionSaleUNIT(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_AUCTION , InstanceID, iX, iY, iCX, iCY, pParent )	
, m_pBGTexture(NULL)
, m_pDisTexture(NULL)
, m_pAuctionAttr(NULL)
, m_pItemInfo(NULL)
{
	Init();
}

SPWindowAuctionSaleUNIT::~SPWindowAuctionSaleUNIT()
{
	Clean();
}

void SPWindowAuctionSaleUNIT::Init()
{
	m_strToolTip	=	"[SPWindowAuctionSaleUNIT]";
	m_bShowToolTip	= false ;
	m_bSelectAble	= true ;
	m_bUsable		= true ;

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
	SPWindowStatic*	pWindowItem		=	NULL;

	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 8 , 5 , 32 , 32 , this );		// Item BG (+1) == m_iStart
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 1 , 34 );

	iInstanceID++;
	pWindowItem	=	new	SPWindowStatic( iInstanceID , 8 , 5 , 32 , 32 , this );		// Item (+2)
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
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 150 , 16 , 17 , 11 , this );	// MoneyIcon(+6)
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 157 , 22 );	// Ely
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 193 , 22 );	// Oz
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 211 , 22 );	// Point
	pWindowStatic->SetShowEnable(false) ;

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 213 , 16 , 67 , 12 , this );	// FinishTime Text(+7)
	pWindowStatic->SetFontColor( RGBA(0, 0, 0, 255) ) ;
	pWindowStatic->SetFormat(DT_TOP | DT_CENTER) ;
	pWindowStatic->SetWindowText("VeryLong") ;

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 284 , 16 , 112 , 12 , this );	// Bidder Text(+8)
	pWindowStatic->SetFontColor( RGBA(0, 0, 0, 255) ) ;
	pWindowStatic->SetFormat(DT_TOP | DT_CENTER) ;
	pWindowStatic->SetWindowText("Sellers") ;

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 411 , 9 , 86 , 12 , this );	// BidPrice Text(+9)
	pWindowStatic->SetFontColor( RGBA(143, 99, 99, 255) ) ;
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT) ;
	pWindowStatic->SetWindowText("999,999,999,999") ;
	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 501 , 8 , 17 , 11 , this );	// BidPrice Icon(+10)
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 157 , 22 );	// Ely
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 193 , 22 );	// Oz
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 211 , 22 );	// Point

	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 411 , 28 , 86 , 12 , this );	// SpotPrice Text(+11)
	pWindowStatic->SetFontColor( RGBA(0, 0, 0, 255) ) ;
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT) ;
	pWindowStatic->SetWindowText("999,999,999,999") ;
	iInstanceID++;
	pWindowStatic	=	new	SPWindowStatic( iInstanceID , 501 , 27 , 17 , 11 , this );	// SpotPrice Icon(+12)
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 157 , 22 );	// Ely
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 193 , 22 );	// Oz
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 211 , 22 );	// Point

	iInstanceID++;
	pWindowButton	=	new	SPWindowButton( iInstanceID , 527 , 3 , 41 , 36 , this );	// Cancel Button (+13)
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 66 , 146 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 66 , 183 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 66 , 220 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 66 , 257 );
	m_iCancelButton = iInstanceID ;

	iInstanceID++;
	m_iEndID	=	iInstanceID; // (+13)
}

void SPWindowAuctionSaleUNIT::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
	SAFE_RELEASE( m_pDisTexture );
}

void SPWindowAuctionSaleUNIT::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorEnter( NULL , NULL );
	}
}

void SPWindowAuctionSaleUNIT::Hide(bool bSendServer)
{
	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorOut( NULL , NULL );
	}
}

void SPWindowAuctionSaleUNIT::ReposOwnImage()
{
	SetRect( &m_rcBGDest	, m_iAX		, m_iAY		, m_iAX + 576	, m_iAY + 42 );		//	BG
	SetRect( &m_rcLineDest	, m_iAX		, m_iAY+42	, m_iAX + 576	, m_iAY + 43 );		//	Line
	//SetRect( &m_rcLineDest	, m_iAX+2	, m_iAY+42	, m_iAX + 576	, m_iAY + 43 );		//	Line
	SetRect( &m_rcDisDest	, m_iAX+8	, m_iAY+5	, m_iAX + 40	, m_iAY + 37);		//	Disable
}

void SPWindowAuctionSaleUNIT::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowAuctionSaleUNIT::Process(float fTime)
{
	SPWindow::Process( fTime );
}

void SPWindowAuctionSaleUNIT::Render(float fTime)
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

	if( !m_bUsable )	{
		if( m_pDisTexture )	{
			m_pDisTexture->RenderTexture( &m_rcDisDest , &m_rcDisSrc );
		}
	}

	g_pVideo->Flush();
}



//////////////////////////////////////////////////////////////////////////
// 아이템 설정
void	SPWindowAuctionSaleUNIT::SetItemField( SPItem* pItem)
{
	m_iAuctionType = AUCTION_TYPE_ITEM_BY_ELY ;

	if( pItem == NULL )	{
		ErrorFLogs->Error_Log("SPWindowAuctionSaleUNIT::SetItemField ## Item is NULL!!!") ;
		return ;
	}

	m_pItemInfo = pItem ;		// 아이템 정보 저장

	// Resize Item Name
	// Hide Money Icon
	POINT ptIcon ;
	D3DXCOLOR cItemColor ;
	SPTexture* pIcon ;
	SPWindowStatic* pWindowStatic ;
	pWindowStatic	=	(SPWindowStatic*)Find( m_iItemIcon, true ) ;		// 49 , 16 , 155 , 12 , this );	// Item (+2)
	pIcon = g_pItemCluster->GetItemTexture(m_pItemInfo->GetItemStatus()->GetItemID(), ptIcon) ;
	pWindowStatic->SPSendMessage(SPIM_SET_IMAGECLEAR) ;
	pWindowStatic->SetImage( pIcon, ptIcon.x, ptIcon.y ) ;
	int iStackCount = pItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);		// ItemCount
	SetItemCount(iStackCount) ;

	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+4, true ) ;		// 49 , 16 , 155 , 12 , this );	// Item Text(+5)
	pWindowStatic->SetRectSize(155, 12) ;
	pWindowStatic->SetFormat(DT_CENTER) ;
	if( pItem->GetItemAttr()->m_bNameColor == true )
		cItemColor	=	pItem->GetItemAttr()->GetNameColor();
	else
		cItemColor	=	g_pCheckManager->GetItemColor( pItem->GetItemStatus()->GetRareLevel() );
	pWindowStatic->SetFontColor( cItemColor) ;
	pWindowStatic->SetWindowText(pItem->GetItemStatus()->GetItemAttr()->m_strName.c_str() ) ;

	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+5, true ) ;		// 150 , 16 , 17 , 12 , this );	// MoneyIcon(+6)
	pWindowStatic->SetShowEnable(false) ;

	// BidPrice, SpotPrice Icon Setting
	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+9, true ) ;		// 입찰가 Icon (+10)
	pWindowStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+11, true ) ;		// 즉구가 Icon (+12)
	pWindowStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;

	// Item 상태 처리(Disable/ Enable)
	m_bUsable = g_pCheckManager->CheckItemStatus( m_pItemInfo->GetItemStatus()->GetItemID() ) ;
}


void	SPWindowAuctionSaleUNIT::SetItemField( int iType, INT64 iMoney )
{
	int iPriceIcon = 0 ;
	int iSaleIcon = 0 ;
	m_iAuctionType = iType ;
	m_pItemInfo = NULL ;		// 아이템 정보초기화

	// Resize Item Name
	// Hide Money Icon
	SPWindowStatic* pWindowStatic ;
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

	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+9, true ) ;		// 입찰가 Icon (+10)
	pWindowStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, iPriceIcon) ;
	pWindowStatic	=	(SPWindowStatic*)Find( m_iStartID+11, true ) ;		// 즉구가 Icon (+12)
	pWindowStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, iPriceIcon) ;

	// 판매 머니 정보
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
void SPWindowAuctionSaleUNIT::SetItemCount(int iCount)
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
	마감시간 Text 및 Font Setting
*/
void	SPWindowAuctionSaleUNIT::SetFinishTime()
{
	TCHAR szBuf[16] ;
	UINT32 iRemainTime ;
	SPWindow* pWindow = NULL ;

	iRemainTime =  m_pAuctionAttr->GetFinishTime() ;
	pWindow = Find(m_iCancelButton, true) ;
	if( pWindow )	pWindow->SetShowEnable(true) ;

	if( iRemainTime < 1 )		// 만료
	{
		if( g_pResourceManager->GetGlobalString(52000107) )
			sprintf( szBuf, "%s", g_pResourceManager->GetGlobalString(52000107)) ;
		else
			sprintf( szBuf, "%s", "Expired") ;
		if( pWindow )	pWindow->SetShowEnable(false) ;
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
	pWindow = Find(m_iStartID + 6, true) ;		// FinishTime(+7)
	if( pWindow )	{
		pWindow->SetWindowText(szBuf) ;
	}
}

/**
	판매자 Text 및 Font Setting
*/
void	SPWindowAuctionSaleUNIT::SetBidder( const char* szBidder)
{
	if( szBidder != NULL )	{
		SPWindow* pWindow = Find(m_iStartID + 7, true) ;	// Bidder(+8)
		if( pWindow )	{
			pWindow->SetWindowText(szBidder) ;
		}
	}
}

/**
	현재 입찰 가격 설정
*/
void	SPWindowAuctionSaleUNIT::SetPrice( INT64 iBidPrice, INT64 iSpotPrice, USERID iBidUser )
{
	SPWindow* pWindow = NULL ;

	pWindow = Find(m_iStartID + 8, true) ;						// Bid Price(+9)
	//	입찰 가격이 있는 경우
	if( iBidPrice < 0 )		iBidPrice = 0 ;
	if( pWindow )	{
		pWindow->SetWindowText( ConvertMoneyToString(iBidPrice) ) ;
	}

	pWindow = Find(m_iStartID + 10, true) ;						// Spot Price(+11)
	// 즉시 구매가 있는 경우
	if( iSpotPrice < 0 )	iSpotPrice = 0 ;
	if( pWindow )	{
		pWindow->SetWindowText( ConvertMoneyToString(iSpotPrice) ) ;
	}
}


/**
	아템 정보 설정
	@param SPAuctionAttr* pAuctionAttr	: Auction Attribute
*/
void SPWindowAuctionSaleUNIT::SetAuctionItem( SPAuctionAttr* pAuctionAttr )
{
	m_pAuctionAttr = pAuctionAttr ;
	m_iAuctionID = pAuctionAttr->GetAuctionID() ;

	if( pAuctionAttr->GetType() == AUCTION_TYPE_ITEM_BY_ELY )	{
		SetItemField( pAuctionAttr->GetItemInfo() ) ;	// Item
	}
	else	{
		SetItemField( pAuctionAttr->GetType(), pAuctionAttr->GetSellPoint() ) ;	// Oz, Point, Ely
	}

	SetFinishTime() ;	// 마감시간
	SetBidder( pAuctionAttr->GetBidderName() ) ;		// 입찰자
	SetPrice( pAuctionAttr->GetCurBidPrice(), pAuctionAttr->GetSpotPrice(), pAuctionAttr->GetBidderUserID()) ;		// 금액설정
}

/**
	Auction 정보 초기화
 */
void SPWindowAuctionSaleUNIT::ClearAuctionInfo()
{
	m_pAuctionAttr = NULL ;
	m_pItemInfo = NULL ;
	m_iAuctionID = 0 ;
	m_iAuctionType = 0 ;
}



/**
	선택 및 Mouse Over 이미지
*/
void SPWindowAuctionSaleUNIT::SetSelectBG(char* pTextureFile, RECT* prcSrc)
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
void SPWindowAuctionSaleUNIT::SetWndState( WND_STATE eWndState )
{
	m_WindowState	=	eWndState;
}




SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowAuctionSaleUNIT )

SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER									, OnCursorEnter		)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT										, OnCursorOut		)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE									, OnCursorMove		)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, m_iCancelButton			, OnSaleCancel		)		// 경매취소

SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER	, m_iStartID	, m_iEndID		, OnCursorEnter		)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT	, m_iStartID	, m_iEndID		, OnCursorOut		)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_MOVE	, m_iStartID	, m_iEndID		, OnCursorMove		)

SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	m_iStartID	, m_iStartID + 1	,	OnToolTipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		m_iStartID	, m_iStartID + 1	,	OnToolTipOut)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	m_iStartID	, m_iStartID + 1	,	OnToolTipMove)


SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


/**
Mouse가 영역내에 있으면 m_WindowState를 변경해서 MouseOn Image 처리를 한다.
*/
int SPWindowAuctionSaleUNIT::OnCursorEnter( WPARAM wParam, LPARAM lParam )
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
int SPWindowAuctionSaleUNIT::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	int	iType	=	(int)wParam;

	if( m_WindowState != WND_STATE_CLICK || iType != 0 )
		m_WindowState	=	WND_STATE_NORMAL;

	return 1;
}
int SPWindowAuctionSaleUNIT::OnCursorMove( WPARAM wParam, LPARAM lParam )
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

int SPWindowAuctionSaleUNIT::OnCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( iID == m_iStartID || iID == m_iStartID+1 )		// Item or ItemBG
	{
		// Show ToolTip
	}
	OnCursorEnter( wParam , lParam );
	return 1;
}
int SPWindowAuctionSaleUNIT::OnCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( iID == m_iItemIcon-1 || iID == m_iItemIcon )		// Item or ItemBG
	{
		// ToolTip Out
	}
	OnCursorOut( wParam , lParam );
	return 1;
}
int SPWindowAuctionSaleUNIT::OnCursorMove( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnCursorMove( wParam , lParam );
	return 1;
}



/**
Tooltip
*/
int SPWindowAuctionSaleUNIT::OnToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam)
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

int SPWindowAuctionSaleUNIT::OnToolTipOut(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	if( m_pItemInfo != NULL )
	{
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		pWindow->Show();
	}
	return 1;
}

int SPWindowAuctionSaleUNIT::OnToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam)
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
리스트 클릭 관련 처리(경매 취소)
*/
int SPWindowAuctionSaleUNIT::OnSaleCancel( WPARAM wParam, LPARAM lParam )
{
	SPSendMessageToParent( SPIM_AUCTION_CANCEL , wParam , lParam , this );
	return 1;
}


