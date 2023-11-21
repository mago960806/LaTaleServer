// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowAuctionMsg.cpp
//	created:	2008/2/26   Time:10:59
//	purpose:	경매에서 확인을 위한 메시지 창 (경매 취소등)
//				0:입찰, 1:즉시구매, 3:일반메시지
//*********************************************************************/

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindowListDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPAuctionDef.h"
#include "SPAuctionAttr.h"
#include "SPAuctionArchive.h"
#include "SPAuctionManager.h"

#include "SPWindowAuction.h"
#include "SPWindowAuctionMSG.h"
#include "_Error_log.h"



SPWindowAuctionMsg::SPWindowAuctionMsg( WND_ID WndClassID , INSTANCE_ID InstanceID )
: SPWindow( WndClassID , InstanceID )
, m_pWindowAuction(NULL)
, m_pWindowFeeDialog(NULL)
, m_pWindowMessage(NULL)
, m_pWindowWait(NULL)
{	
	Init();	
}

SPWindowAuctionMsg::SPWindowAuctionMsg( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent /* = NULL  */ )
: SPWindow( WNDID_AUCTION_MSG , InstanceID , iX , iY , iCX , iCY , pParent )
, m_pWindowAuction(NULL)
, m_pWindowFeeDialog(NULL)
, m_pWindowMessage(NULL)
, m_pWindowWait(NULL)
{	
	Init();	
}

SPWindowAuctionMsg::~SPWindowAuctionMsg()
{
	Clean();
}


void SPWindowAuctionMsg::Clean()
{
}


void SPWindowAuctionMsg::Init()
{
	m_strToolTip		= "[SPWindowAuctionMsg]";

	InitAuctionMessage() ;
	InitAuctionFeeDialog() ;
	InitAuctionWait() ;

	m_pWindowFeeDialog->SetShowEnable(false) ;
	m_pWindowMessage->SetShowEnable(false) ;
	m_pWindowWait->SetShowEnable(false) ;
}


void SPWindowAuctionMsg::InitAuctionMessage()
{
	SPWindowStatic*	pWindowStaticFrame;
	SPWindowStatic*	pWindowStaticBG;
	SPWindowStatic*	pWindowStatic;
	SPWindowButton*	pWindowButton;


	pWindowStaticFrame	=	new SPWindowStatic( WIID_AUCTION_MESSAGE , 0 , 0 , 800 , 600 , this );
	pWindowStaticFrame->SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 500 , 500 );
	pWindowStaticFrame->SetSrcSize(8, 8);
	m_pWindowMessage = (SPWindow*)pWindowStaticFrame ;
	pWindowStaticFrame->Hide() ;

	// Dialog BG
	pWindowStaticBG	=	new SPWindowStatic( WIID_AUCTION_MESSAGE+1 , 271, 252 , 262, 102 , m_pWindowMessage );
	pWindowStaticBG->SetWindowType(WND_TYPE_NINEBASE) ;
	pWindowStaticBG->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 317, 1, 8, 1) ;


	// 메시지 TEXT
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_MESSAGE_TEXT , 7 , 18, 247, 40 , pWindowStaticBG );
	pWindowStatic->SetFontColor( RGBA(195, 57, 57, 255) );
	pWindowStatic->SetFormat(DT_VCENTER | DT_CENTER);
	pWindowStatic->SetMultiLine(true, 2) ;
	pWindowStatic->SetWindowText("message012message012message012message012message012") ;
	pWindowStatic	=	NULL;

	// 확인 & 취소
	pWindowButton	=	new	SPWindowButton( WIID_AUCTION_MESSAGE_OK , 56 , 69 , 68 , 17 , pWindowStaticBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 289 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 307 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 325 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 343 );
	pWindowButton	=	new	SPWindowButton( WIID_AUCTION_MESSAGE_CANCEL , 134 , 69 , 68 , 17 , pWindowStaticBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 361 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 379 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 397 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 415 );

	// 즉시구매 버튼
	pWindowButton	=	new	SPWindowButton( WIID_AUCTION_MESSAGE_BIDSPOT, 56 , 69 , 68 , 17 , pWindowStaticBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 187 , 358);
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 187 , 376 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 187 , 394 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_002.PNG" , 187 , 412 );

	m_bShowMessage = false ;
}


void SPWindowAuctionMsg::InitAuctionFeeDialog()
{
	SPWindowStatic*	pWindowStaticFrame;
	SPWindowStatic*	pWindowStaticBG;
	SPWindowStatic*	pWindowStatic;
	SPWindowButton*	pWindowButton;


	pWindowStaticFrame	=	new SPWindowStatic( WIID_AUCTION_DIALOG , 0 , 0 , 800 , 600 , this );
	pWindowStaticFrame->SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 500 , 500 );
	pWindowStaticFrame->SetSrcSize(8, 8);
	m_pWindowFeeDialog = (SPWindow*)pWindowStaticFrame ;
	pWindowStaticFrame->Hide() ;

	// Dialog BG
	pWindowStaticBG	=	new SPWindowStatic( WIID_AUCTION_DIALOG+1 , 284 , 195 , 236 , 215 , m_pWindowFeeDialog );
	pWindowStaticBG->SetWindowType(WND_TYPE_NINEBASE) ;
	pWindowStaticBG->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 317, 1, 8, 1) ;

	// TEXT Icons
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_DIALOG+2 , 6 , 22 , 65 , 12 , pWindowStaticBG );	// 낙찰금액
	pWindowStatic->SetFontColor( RGBA(143, 99, 99, 255) );
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT);
	pWindowStatic	=	NULL;
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_DIALOG+3 , 6 , 41 , 65 , 12 , pWindowStaticBG );	// 수수료
	pWindowStatic->SetFontColor( RGBA(143, 99, 99, 255) );
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT);
	pWindowStatic	=	NULL;
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_DIALOG+4 , 6 , 76 , 65 , 12 , pWindowStaticBG );	// 최종금액
	pWindowStatic->SetFontColor( RGBA(143, 99, 99, 255) );
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT);
	pWindowStatic	=	NULL;

	// 낙찰 금액
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_DIALOG_ACCEPT_MONEY , 107 , 22 , 83 , 12 , pWindowStaticBG );
	pWindowStatic->SetFont(FONT_12_NORMAL) ;
	pWindowStatic->SetFontColor( RGBA(143, 99, 99, 255) );
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT);
	pWindowStatic->SetWindowText("999999999999") ;
	pWindowStatic	=	NULL;
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_DIALOG_ACCEPT_MONEY+1 , 194 , 22 , 17 , 11 , pWindowStaticBG );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 157 , 22 );	// Ely
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 193 , 22 );	// Cash
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 211 , 22 );	// Point
	pWindowStatic	=	NULL;

	// 수수료
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_DIALOG_FEE_MONEY+2 , 107 , 39 , 85 , 14 , pWindowStaticBG );	// 배경
	pWindowStatic->SetWindowType(WND_TYPE_LEFTBASE) ;
	pWindowStatic->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 51 , 14, 55, 28);
	pWindowStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 56 , 14, 60, 28);
	pWindowStatic->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 61 , 14, 65, 28);
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_DIALOG_FEE_MONEY , 111 , 41 , 79 , 12 , pWindowStaticBG );
	pWindowStatic->SetFontColor( RGBA(195, 57, 57, 255) );
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT);
	pWindowStatic->SetWindowText("999999999999") ;
	pWindowStatic	=	NULL;
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_DIALOG_FEE_MONEY+1 , 194 , 41 , 17 , 11 , pWindowStaticBG );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 157 , 22 );	// Ely
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 193 , 22 );	// Cash
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 211 , 22 );	// Point
	pWindowStatic	=	NULL;

	// 최종금액
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_DIALOG_GIVEN_MONEY , 91 , 76 , 99 , 12 , pWindowStaticBG );
	pWindowStatic->SetFontColor( RGBA(143, 99, 99, 255) );
	pWindowStatic->SetFormat(DT_TOP | DT_RIGHT);
	pWindowStatic->SetWindowText("999999999999") ;
	pWindowStatic	=	NULL;
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_DIALOG_GIVEN_MONEY+1 , 194 , 76 , 17 , 11 , pWindowStaticBG );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 157 , 22 );	// Ely
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 193 , 22 );	// Cash
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_002.PNG" , 211 , 22 );	// Point
	pWindowStatic	=	NULL;

	// 메시지 TEXT
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_DIALOG_TEXT , 11 , 111 , 210 , 54 , pWindowStaticBG );
	pWindowStatic->SetFontColor( RGBA(195, 57, 57, 255) );
	//pWindowStatic->SetFormat(DT_TOP | DT_CENTER);
	pWindowStatic->SetFormat(DT_VCENTER );
	pWindowStatic->SetMultiLine(true, 2) ;
	pWindowStatic->SetWindowText("message012messa ge012me ssage012message012message012message012messag e012message012") ;
	pWindowStatic	=	NULL;

	// 확인 & 취소
	pWindowButton	=	new	SPWindowButton( WIID_AUCTION_DIALOG_OK , 43 , 180 , 68 , 17 , pWindowStaticBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 289 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 307 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 325 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 277 , 343 );
	pWindowButton	=	new	SPWindowButton( WIID_AUCTION_DIALOG_CANCEL , 121 , 180 , 68 , 17 , pWindowStaticBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 361 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 379 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 397 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG" , 1 , 415 );

	m_bShowFeeDialog = false ;
}



void SPWindowAuctionMsg::InitAuctionWait()
{
	SPWindowStatic*	pWindowStaticFrame;
	SPWindowStatic*	pWindowStaticBG;
	SPWindowStatic*	pWindowStatic;


	pWindowStaticFrame	=	new SPWindowStatic( WIID_AUCTION_WAIT , 0 , 0 , 800 , 600 , this );
	pWindowStaticFrame->SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , 502 , 502 );
	pWindowStaticFrame->SetSrcSize(4, 4);
	m_pWindowWait = (SPWindow*)pWindowStaticFrame ;
	pWindowStaticFrame->Hide() ;

	// Dialog BG
	pWindowStaticBG	=	new SPWindowStatic( WIID_AUCTION_WAIT+1 , 267 , 262 , 265 , 76 , m_pWindowWait );
	pWindowStaticBG->SetWindowType(WND_TYPE_NINEBASE) ;
	pWindowStaticBG->SetImageNine( "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG", 452, 452, 4, 1) ;


	// 메시지 TEXT
	pWindowStatic	=	new SPWindowStatic( WIID_AUCTION_WAIT_TEXT , 87 , 24 , 162 , 26 , pWindowStaticBG );
	pWindowStatic->SetFontColor( RGBA(255, 255, 255, 255) );
	pWindowStatic->SetFormat(DT_TOP | DT_LEFT);
	pWindowStatic->SetMultiLine(true, 2) ;
	pWindowStatic->SetWindowText("message012message012message012message012message012") ;
	pWindowStatic	=	NULL;


	pWindowStatic=	new SPWindowStatic( WIID_AUCTION_WAIT_IMG1 , 12 , 5 , 35, 35, pWindowStaticBG);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 224, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 260, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 296, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 332, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 368, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 404, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 440, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 476, 476);
	pWindowStatic = NULL ;
	pWindowStatic=	new SPWindowStatic( WIID_AUCTION_WAIT_IMG2 , 31 , 26 , 45, 45, pWindowStaticBG);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 224, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 260, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 296, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 332, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 368, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 404, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 440, 476);
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG" , 476, 476);
	pWindowStatic->SetSrcSize(35, 35);
	pWindowStatic = NULL ;

	m_bShowWait = false ;
}

/**
	AuctionWindow Pointer를 가지고 있는다.
 */
void SPWindowAuctionMsg::SetAuctionWindow(SPWindow* pWindow)
{
	m_pWindowAuction = pWindow ;
}

void SPWindowAuctionMsg::Process(float fTime)
{
	if( !m_bShow )	return;

	// Wait 상태
	if( m_bShowWait )	{
		m_fWaitImgTime += fTime ;
		if( m_fWaitImgTime > 0.03f )	{
			m_fWaitImgTime-= 0.03f ;
			m_iWaitImgIndex = (++m_iWaitImgIndex) % 8 ;
			SPWindow* pWindow = NULL ;
			pWindow = Find(WIID_AUCTION_WAIT_IMG1, true) ;
			if( pWindow	)	pWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, m_iWaitImgIndex) ;
			pWindow = Find(WIID_AUCTION_WAIT_IMG2, true) ;
			if( pWindow	)	pWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 7-m_iWaitImgIndex) ;
		}
	}

	SPWindow::Process( fTime );
}

/**
	ModalWindow로 띄워야 한다.
 */
void SPWindowAuctionMsg::ShowModalWindow(SPWindow* pWindow)
{
	m_pWindowAuction = g_pInterfaceManager->GetAuctionWindow() ;
	pWindow->SetShowEnable(true, false) ;
	g_pInterfaceManager->SetModalWindow(this) ;
	Show() ;
}


/**
	모든 메시지 창이 닫히면 Window를 닫는다.
	Progress만 닫고 다른 창을 띄우는 경우가있어서 확인후 닫느다.
 */
void SPWindowAuctionMsg::HideMsgWindow()
{
	if( m_bShowWait == false && m_bShowFeeDialog == false && m_bShowMessage == false )
	{
		Hide() ;
	}
}



void SPWindowAuctionMsg::Hide(bool bSendServer)
{
	SPWindow::Hide( bSendServer );
	g_pInterfaceManager->SetModalWindow(NULL) ;
}




SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowAuctionMsg )

SPIMESSAGE_COMMAND(	SPIM_AUCTION_SERVER_CLOSE	, OnAuctionServerClose)
SPIMESSAGE_COMMAND(	SPIM_AUCTION_ERROR_NOTICE	, OnAuctionNotice)
SPIMESSAGE_COMMAND(	SPIM_AUCTION_MSG_NOTICE		, OnAuctionMessage)
SPIMESSAGE_COMMAND(	SPIM_AUCTION_BID_MSG		, OnAuctionBidMessage)
SPIMESSAGE_COMMAND(	SPIM_AUCTION_FEE_MSG		, OnAuctionFeeMessage)
SPIMESSAGE_COMMAND(	SPIM_AUCTION_WAIT_SHOW		, OnAuctionWaitShow)
SPIMESSAGE_COMMAND(	SPIM_AUCTION_WAIT_CLOSE		, OnAuctionWaitClose)


// MessageBox
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_MESSAGE_OK				, OnMessageOK	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_MESSAGE_CANCEL			, OnMessageCancel	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_MESSAGE_BIDSPOT			, OnMessageBidSpot	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_DIALOG_OK				, OnFeeDialogOK	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			, WIID_AUCTION_DIALOG_CANCEL			, OnFeeDialogCancel	)

SPIMESSAGE_OBJ_MAP_END(	SPWindow	)




int SPWindowAuctionMsg::OnAuctionServerClose(WPARAM wParam, LPARAM lParam)
{
	OnAuctionNotice(AUCTION_MSG_SERVER_DOWN, (LPARAM)g_pResourceManager->GetGlobalString(52000075) ) ;
	return 0 ;
}


/**
	메시지 Dialog를 띄운다[확인/취소].
	@param WPARAM wParam : AUCTION_MSG_TYPE
	@param LPARAM lParam : const char* pMessage
*/
int SPWindowAuctionMsg::OnAuctionMessage(WPARAM wParam, LPARAM lParam)
{
	AUCTION_MSG_TYPE eMsgType = (AUCTION_MSG_TYPE)wParam ;
	const char* pMessage = (const char*)lParam ;
	if( pMessage == NULL)
		return 0;

	SPWindowStatic* pWindowStatic = (SPWindowStatic*)Find( WIID_AUCTION_MESSAGE_TEXT, true);
	if( pWindowStatic )
		pWindowStatic->SetWindowText(pMessage) ;

	// 선택 메시지는 [확인/취소]만 나온다.
	SPWindow* pWindow = NULL ;
	pWindow	=	Find( WIID_AUCTION_MESSAGE_OK, true );
	if( pWindow )	{		
		pWindow->AdjustCoord(59, 69) ;
		pWindow->SetShowEnable(true) ;
	}
	pWindow	=	Find( WIID_AUCTION_MESSAGE_CANCEL, true );
	if( pWindow )			pWindow->SetShowEnable(true) ;		// Show

	pWindow	=	Find( WIID_AUCTION_MESSAGE_BIDSPOT, true );
	if( pWindow )			pWindow->SetShowEnable(false) ;		// Hide

	m_pBidItemAttr = NULL ;
	m_iMessageType = eMsgType ;
	m_bShowMessage = true ;
	ShowModalWindow(m_pWindowMessage) ;
	return 0;
}



/**
	메시지 Dialog를 띄운다. [확인]
	@param WPARAM wParam : AUCTION_MSG_TYPE
	@param LPARAM lParam : const char* pMessage
*/
int SPWindowAuctionMsg::OnAuctionNotice(WPARAM wParam, LPARAM lParam)
{
	AUCTION_MSG_TYPE eMsgType = (AUCTION_MSG_TYPE)wParam ;
	const char* pMessage = (const char*)lParam ;
	if( pMessage == NULL)
		return 0 ;

	SPWindowStatic* pWindowStatic = (SPWindowStatic*)Find( WIID_AUCTION_MESSAGE_TEXT, true);
	if( pWindowStatic )
		pWindowStatic->SetWindowText(pMessage) ;

	// 알림 메시지는 [확인]만 나온다.
	SPWindow* pWindow = NULL ;
	pWindow	=	Find( WIID_AUCTION_MESSAGE_OK, true );
	if( pWindow )		{
		pWindow->AdjustCoord(95, 69) ;
		pWindow->SetShowEnable(true) ;
	}
	pWindow	=	Find( WIID_AUCTION_MESSAGE_CANCEL, true );
	if( pWindow )			pWindow->SetShowEnable(false) ;		// Hide
	pWindow	=	Find( WIID_AUCTION_MESSAGE_BIDSPOT, true );
	if( pWindow )			pWindow->SetShowEnable(false) ;		// Hide

	m_pBidItemAttr = NULL ;
	m_iMessageType = eMsgType ;
	m_bShowMessage = true ;
	ShowModalWindow(m_pWindowMessage) ;
	return 0 ;
}



/**
	입찰에 관련된 메시지를 출력한다.
	@param WPARAM wParam : AUCTION_MSG_TYPE
	@param LPARAM lParam : SPAuctionAttr*
*/
int SPWindowAuctionMsg::OnAuctionBidMessage(WPARAM wParam, LPARAM lParam)
{
	AUCTION_MSG_TYPE iMsgType	= (AUCTION_MSG_TYPE)wParam ;
	SPAuctionAttr* pItemAttr	= (SPAuctionAttr*)lParam ;
	SPWindow* pWindowOK = NULL ;
	SPWindow* pWindowCN = NULL ;
	SPWindow* pWindowBID = NULL ;
	SPWindowStatic* pWindowStatic = (SPWindowStatic*)Find( WIID_AUCTION_MESSAGE_TEXT, true);
	TCHAR	szMessage[256] ;


	if( iMsgType == AUCTION_MSG_BID )	{		// 입찰
		// 입찰가가 즉구가 보다 크거나 같으면 즉구메시지
		if( pItemAttr->GetSpotPrice() > 0 && pItemAttr->GetNextBidPrice() >= pItemAttr->GetSpotPrice() )	{
			iMsgType = AUCTION_MSG_BIDSPOT ;
			if( pItemAttr->GetType() == AUCTION_TYPE_ELY_BY_CASH )	{
				_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000105), ConvertMoneyToString(pItemAttr->GetSpotPrice())) ;
				pWindowStatic->SetWindowText(szMessage) ;
			}
			else if( pItemAttr->GetType() == AUCTION_TYPE_ELY_BY_MILEAGE )	{
				_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000106), ConvertMoneyToString(pItemAttr->GetSpotPrice())) ;
				pWindowStatic->SetWindowText(szMessage) ;
			}
			else	{
				_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000104), ConvertMoneyToString(pItemAttr->GetSpotPrice())) ;
				pWindowStatic->SetWindowText(szMessage) ;
			}
		}
		else	{
			// 입찰 메시지
			if( pItemAttr->GetType() == AUCTION_TYPE_ELY_BY_CASH )	{
				_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000088), ConvertMoneyToString(pItemAttr->GetNextBidPrice())) ;
				pWindowStatic->SetWindowText(szMessage) ;
			}
			else if( pItemAttr->GetType() == AUCTION_TYPE_ELY_BY_MILEAGE )	{
				_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000089), ConvertMoneyToString(pItemAttr->GetNextBidPrice())) ;
				pWindowStatic->SetWindowText(szMessage) ;
			}
			else	{
				_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000087), ConvertMoneyToString(pItemAttr->GetNextBidPrice())) ;
				pWindowStatic->SetWindowText(szMessage) ;
			}
		}
	}
	else if( iMsgType == AUCTION_MSG_SPOT ) {	
		// 즉시구매
		if( pItemAttr->GetType() == AUCTION_TYPE_ELY_BY_CASH )	{
			_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000102), ConvertMoneyToString(pItemAttr->GetSpotPrice())) ;
			pWindowStatic->SetWindowText(szMessage) ;
		}
		else if( pItemAttr->GetType() == AUCTION_TYPE_ELY_BY_MILEAGE )	{
			_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000103), ConvertMoneyToString(pItemAttr->GetSpotPrice())) ;
			pWindowStatic->SetWindowText(szMessage) ;
		}
		else	{
			_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000101), ConvertMoneyToString(pItemAttr->GetSpotPrice())) ;
			pWindowStatic->SetWindowText(szMessage) ;
		}
	}
	else if( iMsgType == AUCTION_MSG_CANCEL ) {	
		_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000109)) ;
		pWindowStatic->SetWindowText(szMessage) ;
	}
	else	{
		ErrorFLogs->Error_Log("Auction BidMessage Type Error[%d]", iMsgType) ;
		return 0 ;
	}

	m_pBidItemAttr = pItemAttr ;
	// 입찰 메시지는 [확인][취소]가 나온다.
	pWindowOK	=	Find( WIID_AUCTION_MESSAGE_OK, true );
	pWindowCN	=	Find( WIID_AUCTION_MESSAGE_CANCEL, true );
	pWindowBID	=	Find( WIID_AUCTION_MESSAGE_BIDSPOT, true );

	if( iMsgType == AUCTION_MSG_BIDSPOT )	{		// 즉구/취소
		if( pWindowOK )		{
			pWindowOK->AdjustCoord(56, 69) ;
			pWindowOK->SetShowEnable(false) ;
		}
		if( pWindowCN )		pWindowCN->SetShowEnable(true) ;
		if( pWindowBID )	pWindowBID->SetShowEnable(true) ;
	}
	else	{		// 확인/취소
		if( pWindowOK )		{
			pWindowOK->AdjustCoord(56, 69) ;
			pWindowOK->SetShowEnable(true) ;
		}
		if( pWindowCN )		pWindowCN->SetShowEnable(true) ;
		if( pWindowBID )	pWindowBID->SetShowEnable(false) ;
	}

	m_iMessageType = iMsgType ;
	m_bShowMessage = true ;
	ShowModalWindow(m_pWindowMessage) ;
	return 0 ;
}




/**
	타입별로 Dialog를 띄운다.
	@param WPARAM wParam : AUCTION_MSG_TYPE
	@param LPARAM lParam : SPAuctionAttr*
*/
int SPWindowAuctionMsg::OnAuctionFeeMessage(WPARAM wParam, LPARAM lParam)
{
	int iIconType = 0 ;
	AUCTION_MSG_TYPE iMsgType	= (AUCTION_MSG_TYPE)wParam ;
	SPAuctionAttr* pItemAttr	= (SPAuctionAttr*)lParam ;

	AUCTION_TYPE eType = pItemAttr->GetType() ;
	INT64 fItemMoney = pItemAttr->GetSellPoint() ;
	INT64 fFeeMoney = pItemAttr->GetFeePoint() ;
	m_pBidItemAttr = pItemAttr ;

	if( iMsgType == 0 )	{
		ErrorFLogs->Error_Log("Auction Message Type Error[%d]", iMsgType) ;
		return 0 ;
	}

	if( eType == AUCTION_TYPE_CASH_BY_ELY )
		iIconType = 1 ;		// Cash
	else if( eType == AUCTION_TYPE_MILEAGE_BY_ELY )
		iIconType = 2 ;		// Point
	else	{
		ErrorFLogs->Error_Log("FeeDialog Error AuctionType Miss-match[%d]", eType) ;
		return 0 ;
	}

	SPWindowStatic* pWindowStatic ;
	pWindowStatic	=	(SPWindowStatic*)Find( WIID_AUCTION_DIALOG+2 , true );			// 등록금액
	if( pWindowStatic )	pWindowStatic->SetWindowText(g_pResourceManager->GetGlobalString(52000057)) ;
	pWindowStatic	=	(SPWindowStatic*)Find( WIID_AUCTION_DIALOG+3 , true );			// 수수료
	if( pWindowStatic )	pWindowStatic->SetWindowText(g_pResourceManager->GetGlobalString(52000025)) ;
	pWindowStatic	=	(SPWindowStatic*)Find( WIID_AUCTION_DIALOG+4 , true );			// 최종금액
	if( pWindowStatic )	pWindowStatic->SetWindowText(g_pResourceManager->GetGlobalString(52000062)) ;


	pWindowStatic	=	(SPWindowStatic*)Find( WIID_AUCTION_DIALOG_ACCEPT_MONEY+1, true );
	if( pWindowStatic )	pWindowStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, iIconType) ;
	pWindowStatic	=	(SPWindowStatic*)Find( WIID_AUCTION_DIALOG_FEE_MONEY+1, true );
	if( pWindowStatic )	pWindowStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, iIconType) ;
	pWindowStatic	=	(SPWindowStatic*)Find( WIID_AUCTION_DIALOG_GIVEN_MONEY+1, true );
	if( pWindowStatic )	pWindowStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, iIconType) ;

	pWindowStatic	=	(SPWindowStatic*)Find( WIID_AUCTION_DIALOG_ACCEPT_MONEY, true );
	if( pWindowStatic )	pWindowStatic->SetWindowText(ConvertMoneyToString(fItemMoney)) ;
	pWindowStatic	=	(SPWindowStatic*)Find( WIID_AUCTION_DIALOG_FEE_MONEY, true );
	if( pWindowStatic )	pWindowStatic->SetWindowText(ConvertMoneyToString(fFeeMoney)) ;
	pWindowStatic	=	(SPWindowStatic*)Find( WIID_AUCTION_DIALOG_GIVEN_MONEY, true );
	if( pWindowStatic )	pWindowStatic->SetWindowText(ConvertMoneyToString(fItemMoney-fFeeMoney)) ;

	// 즉구 가격이 있고,
	// 입찰가가 즉구가 보다 크거나 같으면 즉구메시지
	if( pItemAttr->GetSpotPrice() > 0 && pItemAttr->GetNextBidPrice() >= pItemAttr->GetSpotPrice() )
		iMsgType = AUCTION_MSG_SPOT ;

	pWindowStatic	=	(SPWindowStatic*)Find( WIID_AUCTION_DIALOG_TEXT , true );			// 메시지
	if( pWindowStatic )		{
		TCHAR szMessage[256] ;
		if( iMsgType == AUCTION_MSG_BID )	{			
			// 입찰 수수료 메시지
			if( pItemAttr->GetType() == AUCTION_TYPE_ELY_BY_CASH )	{
				_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000096), ConvertMoneyToString(pItemAttr->GetNextBidPrice()), g_pkAuctionManager->GetAuctionFeeRate()) ;
				pWindowStatic->SetWindowText(szMessage) ;
			}
			else if( pItemAttr->GetType() == AUCTION_TYPE_ELY_BY_MILEAGE )	{
				_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000097), ConvertMoneyToString(pItemAttr->GetNextBidPrice()), g_pkAuctionManager->GetAuctionFeeRate()) ;
				pWindowStatic->SetWindowText(szMessage) ;
			}
			else	{
				_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000095), ConvertMoneyToString(pItemAttr->GetNextBidPrice()), g_pkAuctionManager->GetAuctionFeeRate()) ;
				pWindowStatic->SetWindowText(szMessage) ;
			}
		}
		else if( iMsgType == AUCTION_MSG_SPOT )	{	
			// 즉시구매 수수료 메시지
			if( pItemAttr->GetType() == AUCTION_TYPE_ELY_BY_CASH )	{
				_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000099), ConvertMoneyToString(pItemAttr->GetSpotPrice()), g_pkAuctionManager->GetAuctionFeeRate()) ;
				pWindowStatic->SetWindowText(szMessage) ;
			}
			else if( pItemAttr->GetType() == AUCTION_TYPE_ELY_BY_MILEAGE )	{
				_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000100), ConvertMoneyToString(pItemAttr->GetSpotPrice()), g_pkAuctionManager->GetAuctionFeeRate()) ;
				pWindowStatic->SetWindowText(szMessage) ;
			}
			else	{
				_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000098), ConvertMoneyToString(pItemAttr->GetSpotPrice()), g_pkAuctionManager->GetAuctionFeeRate()) ;
				pWindowStatic->SetWindowText(szMessage) ;
			}
		}
	}

	m_iMessageType = iMsgType ;
	m_bShowFeeDialog = true ;
	ShowModalWindow(m_pWindowFeeDialog) ;

	return 0 ;
}



/**
	메시지 Dialog를 띄운다.
	@param WPARAM wParam : Message Type
	@param LPARAM lParam : Message String
*/
int SPWindowAuctionMsg::OnAuctionWaitShow(WPARAM wParam, LPARAM lParam)
{
	const char* pMsgString = (const char*)lParam ;
	if( pMsgString == NULL)
		return 0 ;

	SPWindowStatic* pWindowStatic = (SPWindowStatic*)Find( WIID_AUCTION_WAIT_TEXT, true);
	if( pWindowStatic )
		pWindowStatic->SetWindowText(pMsgString) ;

	m_bShowWait = true ;		// for Animation
	m_fWaitImgTime = 0 ;
	ShowModalWindow(m_pWindowWait) ;
	return 0 ;
}


/**
	Wait Dialog Hide
*/
int SPWindowAuctionMsg::OnAuctionWaitClose(WPARAM wParam, LPARAM lParam)
{
	if( m_bShowWait == true )	{
		m_bShowWait = false ;
		m_fWaitImgTime = 0 ;
		m_pWindowWait->SetShowEnable(false) ;
		HideMsgWindow() ;
	}
	return 0 ;
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
int SPWindowAuctionMsg::OnMessageOK(WPARAM wParam, LPARAM lParam)
{
	//ErrorFLogs->Error_Log("SPWindowAuctionMsg::OnMessageOK") ;
	
	if( m_iMessageType == AUCTION_MSG_REFRESH )	{
		// Search GetNext
		OnAuctionWaitShow(0, (LPARAM)g_pResourceManager->GetGlobalString(52000079)) ;
		m_pWindowAuction->SPSendMessage(SPIM_AUCTION_SEND_NEXT, 0, 0) ;
	}
	else if( m_iMessageType == AUCTION_MSG_BID )	{
		// Send Bid
		OnAuctionWaitShow(0, (LPARAM)g_pResourceManager->GetGlobalString(52000078)) ;
		m_pWindowAuction->SPSendMessage(SPIM_AUCTION_SEND_BID, 0, (LPARAM)m_pBidItemAttr) ;
	}
	else if( m_iMessageType == AUCTION_MSG_SPOT )	{
		// Send Spot
		OnAuctionWaitShow(0, (LPARAM)g_pResourceManager->GetGlobalString(52000078)) ;
		m_pWindowAuction->SPSendMessage(SPIM_AUCTION_SEND_SPOT, 0, (LPARAM)m_pBidItemAttr) ;
	}
	else if( m_iMessageType == AUCTION_MSG_CANCEL )	{
		// Send Cancel
		OnAuctionWaitShow(0, (LPARAM)g_pResourceManager->GetGlobalString(52000078)) ;
		m_pWindowAuction->SPSendMessage(SPIM_AUCTION_SEND_CANCEL, 0, (LPARAM)m_pBidItemAttr) ;
	}
	else if( m_iMessageType == AUCTION_MSG_SERVER_DOWN )	{
		// Close Window
		m_pWindowAuction->SPSendMessage(SPIM_AUCTION_SERVER_CLOSE, 0, 0) ;
	}

	m_pBidItemAttr = NULL ;
	m_bShowMessage = false ;
	m_iMessageType = AUCTION_MSG_NULL ;
	m_pWindowMessage->SetShowEnable(false) ;
	HideMsgWindow() ;
	return 0 ;
}

int SPWindowAuctionMsg::OnMessageCancel(WPARAM wParam, LPARAM lParam)
{
	//ErrorFLogs->Error_Log("SPWindowAuctionMsg::OnMessageCancel") ;

	m_pBidItemAttr = NULL ;
	m_iMessageType = AUCTION_MSG_NULL ;
	m_bShowMessage = false ;
	m_pWindowMessage->SetShowEnable(false) ;
	HideMsgWindow() ;
	return 0 ;
}


int SPWindowAuctionMsg::OnMessageBidSpot(WPARAM wParam, LPARAM lParam)
{
	//ErrorFLogs->Error_Log("SPWindowAuctionMsg::OnMessageBidSpot") ;
	// Send Spot
	OnAuctionWaitShow(0, (LPARAM)g_pResourceManager->GetGlobalString(52000078)) ;
	m_pWindowAuction->SPSendMessage(SPIM_AUCTION_SEND_SPOT, 0, (LPARAM)m_pBidItemAttr) ;

	m_pBidItemAttr = NULL ;
	m_iMessageType = AUCTION_MSG_NULL ;
	m_bShowMessage = false ;
	m_pWindowMessage->SetShowEnable(false) ;
	HideMsgWindow() ;
	return 0 ;
}



//////////////////////////////////////////////////////////////////////////
int SPWindowAuctionMsg::OnFeeDialogOK(WPARAM wParam, LPARAM lParam)
{
	//ErrorFLogs->Error_Log("SPWindowAuction::OnMessageOK") ;

	if( m_iMessageType == AUCTION_MSG_REFRESH )	{
		// Search GetNext
		OnAuctionWaitShow(0, (LPARAM)g_pResourceManager->GetGlobalString(52000079)) ;
		m_pWindowAuction->SPSendMessage(SPIM_AUCTION_SEND_NEXT, 0, 0) ;
	}
	else if( m_iMessageType == AUCTION_MSG_BID )	{
		// Send Bid
		OnAuctionWaitShow(0, (LPARAM)g_pResourceManager->GetGlobalString(52000078)) ;
		m_pWindowAuction->SPSendMessage(SPIM_AUCTION_SEND_BID, 0, (LPARAM)m_pBidItemAttr) ;
	}
	else if( m_iMessageType == AUCTION_MSG_SPOT )	{
		// Send Spot
		OnAuctionWaitShow(0, (LPARAM)g_pResourceManager->GetGlobalString(52000078)) ;
		m_pWindowAuction->SPSendMessage(SPIM_AUCTION_SEND_SPOT, 0, (LPARAM)m_pBidItemAttr) ;
	}

	m_pBidItemAttr = NULL ;
	m_iMessageType = AUCTION_MSG_NULL ;
	m_bShowFeeDialog = false ;
	m_pWindowFeeDialog->SetShowEnable(false) ;
	HideMsgWindow() ;
	return 0 ;
}

int SPWindowAuctionMsg::OnFeeDialogCancel(WPARAM wParam, LPARAM lParam)
{
	//ErrorFLogs->Error_Log("SPWindowAuction::OnMessageCancel") ;

	m_pBidItemAttr = NULL ;
	m_iMessageType = AUCTION_MSG_NULL ;
	m_bShowFeeDialog = false ;
	m_pWindowFeeDialog->SetShowEnable(false) ;
	HideMsgWindow() ;
	return 0 ;
}