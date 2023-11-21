#pragma once

class SPWindow;
class SPWindowStatic;
class SPWindowEdit;
class SPWindowEditMultiLine;
class SPWindowButton;

#define WIID_MAILMSG_BASE			(WIID_MAILMSG	+	10)		//베이스
#define WIID_MAILMSG_TOP			(WIID_MAILMSG	+	15)
#define WIID_MAILMSG_CENTER			(WIID_MAILMSG	+	20)
#define WIID_MAILMSG_BOTTOM			(WIID_MAILMSG	+	25)
#define WIID_MAILMSG_BASELINE		(WIID_MAILMSG	+	26)		//베이스와 경계

#define WIID_MAILMSG_ICON			(WIID_MAILMSG	+	27)		//

#define WIID_MAILMSG_CLOSE			(WIID_MAILMSG	+	28)		//Close Button

#define WIID_MAILMSG_TITLEBASE		(WIID_MAILMSG	+	30)		//제목 바닥
#define WIID_MAILMSG_TITLE			(WIID_MAILMSG	+	35)		//제목
#define WIID_MAILMSG_DATE 			(WIID_MAILMSG	+	36)		//제목

#define WIID_MAILMSG_SENDERBASE		(WIID_MAILMSG	+	40)		//보낸사람 바닥
#define WIID_MAILMSG_APPENDBASE		(WIID_MAILMSG	+	41)		//보낸사람 바닥
#define WIID_MAILMSG_SENDER			(WIID_MAILMSG	+	45)		//보낸사람 바닥

#define WIID_MAILMSG_MSG			(WIID_MAILMSG	+	50)		//내용
#define	WIID_MAILMSG_AUCTION_MSG	(WIID_MAILMSG	+	101)	//Auction 내용
#define	WIID_MAILMSG_AUCTION_TITLE			(WIID_MAILMSG	+	102)	//Auction 내용
#define	WIID_MAILMSG_AUCTION_TITLE_ITEM		(WIID_MAILMSG	+	103)	// 아이템 이름 출력의 경우
#define	WIID_MAILMSG_AUCTION_TITLE_MONEY	(WIID_MAILMSG	+	104)	// 머니 출력의 경우
#define	WIID_MAILMSG_AUCTION_TITLE_ICON		(WIID_MAILMSG	+	105)	//
#define	WIID_MAILMSG_AUCTION_TITLE_STRING	(WIID_MAILMSG	+	106)	// 메시지 출력의 경우
#define	WIID_MAILMSG_AUCTION_SELL		(WIID_MAILMSG	+	111)	//
#define	WIID_MAILMSG_AUCTION_SELL_TEXT	(WIID_MAILMSG	+	112)	//
#define	WIID_MAILMSG_AUCTION_SELL_ICON	(WIID_MAILMSG	+	113)	//
#define	WIID_MAILMSG_AUCTION_FEE		(WIID_MAILMSG	+	114)	//
#define	WIID_MAILMSG_AUCTION_FEE_TEXT	(WIID_MAILMSG	+	115)	//
#define	WIID_MAILMSG_AUCTION_FEE_ICON	(WIID_MAILMSG	+	116)	//
#define	WIID_MAILMSG_AUCTION_TOTAL		(WIID_MAILMSG	+	117)	//
#define	WIID_MAILMSG_AUCTION_TOTAL_TEXT	(WIID_MAILMSG	+	118)	//
#define	WIID_MAILMSG_AUCTION_TOTAL_ICON	(WIID_MAILMSG	+	119)	//
#define	WIID_MAILMSG_AUCTION_LINE		(WIID_MAILMSG	+	120)	//

#define WIID_MAILMSG_ITEM			(WIID_MAILMSG	+	60)		//Item Button
#define WIID_MAILMSG_ITEMICON_LINE	(WIID_MAILMSG	+	61)		//Item Icon LINE
#define WIID_MAILMSG_ITEMICON		(WIID_MAILMSG	+	65)		//Item Icon
#define WIID_MAILMSG_ITEMNAME		(WIID_MAILMSG	+	70)		//Item Name

#define WIID_MAILMSG_ELY			(WIID_MAILMSG	+	75)		//Ely
#define WIID_MAILMSG_ELY_IMG		(WIID_MAILMSG	+	76)		//Ely

#define WIID_MAILMSG_RECEIVE		(WIID_MAILMSG	+	80)		//받기
#define WIID_MAILMSG_DELETE			(WIID_MAILMSG	+	90)		//삭제

enum ICON_MARK;

class SPMailAttr ;
class SPWindowMailMsg : public SPWindow 
{
public:
	SPWindowMailMsg( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	virtual ~SPWindowMailMsg();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	
	void SetAuctionMsg(SPMailAttr* pMailAttr) ;

protected:
	void InitSubWindow();
	void SetAuctionMsgMoney(INT64 iValue1, INT64 iValue2, INT64 iValue3, int iIconType, bool bComplete = false) ;
	void SetAuctionMsgTitle(bool bSell, int iItemID, SPMailAttr* pMailAttr=NULL) ;
	void SetAuctionMsgTitle(bool bSell, INT64 iMoney, int iIconType) ;
	void SetAuctionMsgTitle(GLOBAL_STRING_ID iMsgID) ;

	virtual void Show(bool bWithChild = true);

	SPIMESSAGE_OBJ_MAP_DECLARE()	
	int OnUpdateMailMsg(WPARAM wParam, LPARAM lParam);
	int OnClose(WPARAM wParam, LPARAM lParam);
	int OnReceiveMail(WPARAM wParam, LPARAM lParam);
	int OnDeleteMail(WPARAM wParam, LPARAM lParam);	
	int OnToolTipEnter	( WPARAM, LPARAM );
	int OnToolTipOut	( WPARAM, LPARAM );
	int OnToolTipMove	( WPARAM, LPARAM );

private:	
	SPTexture*		m_pDisable;
	SPTexture*		m_pCheck;

	RECT			m_rtSrcCenter;
	RECT			m_rtDestCenter;	

	SPWindow*		m_pCheckWnd;
	SPWindow*		m_pTitle;						/**< 편지 제목 <br> */
	SPWindow*		m_pDate;
	SPWindow*		m_pSender;						/**< 보낸이 <br> */
	SPWindow*		m_pMsg;							/**< 내용 <br> */
	SPWindow*		m_pAuctionMsg;					/**< 내용 <br> */
	SPWindowButton*	m_pItemButton;
	SPWindow*		m_pItemIcon;
	SPWindow*		m_pItemName;
	SPWindow*		m_pEly;

	SPWindowButton* m_pReceive;
	SPWindowButton*	m_pDelete;		
	
	RECT m_rtSrcCheck;								/**< Check 표시 소스 <br> */
	RECT m_rtSrcNCheck;

	RECT m_rtItemStack;								//Send Item Stack Count
	int m_iItemCount;								//Send Item Stack Count

	RECT m_rtMarkSrc[4];							/**< ICON MARK Src <br> */
	ICON_MARK m_iItemMark;							//Send Item Mark
};