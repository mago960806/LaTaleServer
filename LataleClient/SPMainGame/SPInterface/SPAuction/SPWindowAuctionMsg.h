// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowAuctionMsg.h
//	created:	2008/2/26   Time:10:59
//	purpose:	
//*********************************************************************/


#pragma once


#define WIID_AUCTION_DIALOG					(WIID_AUCTION_MSG + 1)		// 경매 수수료 다이얼로그
#define WIID_AUCTION_DIALOG_ACCEPT_MONEY	(WIID_AUCTION_MSG + 11)		// 낙찰금액 (+2)
#define WIID_AUCTION_DIALOG_FEE_MONEY		(WIID_AUCTION_MSG + 13)		// 수수료 (+3)
#define WIID_AUCTION_DIALOG_GIVEN_MONEY		(WIID_AUCTION_MSG + 16)		// 최종금액 (+2)
#define WIID_AUCTION_DIALOG_TEXT			(WIID_AUCTION_MSG + 18)		// 수수료 메시지
#define WIID_AUCTION_DIALOG_OK				(WIID_AUCTION_MSG + 21)		// 확인
#define WIID_AUCTION_DIALOG_CANCEL			(WIID_AUCTION_MSG + 22)		// 취소

#define WIID_AUCTION_MESSAGE				(WIID_AUCTION_MSG + 51)		// 경고 메시지 창
#define WIID_AUCTION_MESSAGE_TEXT			(WIID_AUCTION_MSG + 53)		// 경고 메시지
#define WIID_AUCTION_MESSAGE_OK				(WIID_AUCTION_MSG + 54)		// 확인
#define WIID_AUCTION_MESSAGE_CANCEL			(WIID_AUCTION_MSG + 55)		// 취소
#define WIID_AUCTION_MESSAGE_BIDSPOT		(WIID_AUCTION_MSG + 56)		// 즉시구매(입찰가==즉구가)

#define WIID_AUCTION_WAIT					(WIID_AUCTION_MSG + 71)		// Wait 메시지 창 (+2)
#define WIID_AUCTION_WAIT_IMG1				(WIID_AUCTION_MSG + 73)		// Wait Image
#define WIID_AUCTION_WAIT_IMG2				(WIID_AUCTION_MSG + 74)		// Wait Image
#define WIID_AUCTION_WAIT_TEXT				(WIID_AUCTION_MSG + 75)		// Wait 메시지


enum AUCTION_MSG_TYPE	{
	AUCTION_MSG_NULL = 0,
	AUCTION_MSG_NOTICE,			// 일반 메시지
	AUCTION_MSG_REFRESH,		// 없는 아이템
	AUCTION_MSG_BID,			// 입찰 수수료 표시
	AUCTION_MSG_SPOT,			// 즉구 수수료 표시
	AUCTION_MSG_BIDSPOT,		// 입찰가 = 즉구 인경우
	AUCTION_MSG_CANCEL,			// 등록 취소
	AUCTION_MSG_SERVER_DOWN,	// Server Down
} ;




//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class SPWindow;
class SPAuctionAttr ;
struct AUCTION_TYPE_INFO;
class SPWindowAuctionMsg : public SPWindow
{
public:
	SPWindowAuctionMsg(WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowAuctionMsg( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowAuctionMsg();

	virtual void	Init();
	virtual void	Clean();

	virtual void	Process( float fTime );
	//virtual void	Render( float fTime );

	//virtual void	Show( bool bWithChild = true );
	virtual void	Hide(bool bSendServer = true);

	void ShowModalWindow(SPWindow* pWindow) ;
	void HideMsgWindow() ;
	void SetAuctionWindow(SPWindow* pWindow) ;

private:
	void InitAuctionFeeDialog();					// 수수료 Dialog
	void InitAuctionMessage();						// Message Dialog
	void InitAuctionWait() ;						// Wait Dialog


private:
	SPWindow*	m_pWindowFeeDialog;
	SPWindow*	m_pWindowMessage;
	SPWindow*	m_pWindowWait;
	SPWindow*	m_pWindowAuction;

	bool		m_bShowFeeDialog ;		// 수수료
	bool		m_bShowMessage ;		// 알림 창
	bool		m_bShowWait ;			// 데이터 대기창
	int			m_iWaitImgIndex ;		// Wait Image Animation Index
	float		m_fWaitImgTime ;		// Wait Image Animation Time
	AUCTION_MSG_TYPE	m_iMessageType ;		// 메시지창 타입
	SPAuctionAttr*		m_pBidItemAttr ;		// 구입하는 아이템 속성

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	virtual int OnAuctionServerClose(WPARAM wParam, LPARAM lParam) ;		// Server Down
	virtual int OnAuctionWaitShow(WPARAM wParam, LPARAM lParam) ;			// Show Wait Dialog
	virtual int OnAuctionWaitClose(WPARAM wParam, LPARAM lParam) ;			// Hide Wait Dialog
	
	virtual int OnAuctionNotice(WPARAM wParam, LPARAM lParam) ;			// MSG_TYPE, MSG
	virtual int OnAuctionMessage(WPARAM wParam, LPARAM lParam) ;		// MSG_TYPE, MSG
	virtual int OnAuctionBidMessage(WPARAM wParam, LPARAM lParam) ;		// MSG_TYPE, SPAuctionAttr*
	virtual int OnAuctionFeeMessage(WPARAM wParam, LPARAM lParam) ;		// MSG_TYPE, SPAuctionAttr*

	// Dialog & Message
	int OnFeeDialogOK(WPARAM wParam, LPARAM lParam) ;			// 수수료 ok
	int OnFeeDialogCancel(WPARAM wParam, LPARAM lParam);		// 수수료 Cancel
	int OnMessageOK(WPARAM wParam, LPARAM lParam) ;				// Message ok
	int OnMessageBidSpot(WPARAM wParam, LPARAM lParam);			// Message BidSpot
	int OnMessageCancel(WPARAM wParam, LPARAM lParam);			// Message Cancel

	// }}


};



