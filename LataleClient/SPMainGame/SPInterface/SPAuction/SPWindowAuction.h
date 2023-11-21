// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowAuction.h
//	created:	2008/2/26   Time:10:59
//	purpose:	
//*********************************************************************/


#pragma once

// Money Frame
#define WIID_AUCTION_MONEY_FRAME				(WIID_AUCTION + 1)
#define WIID_AUCTION_MONEY_FRAME_LEFT			(WIID_AUCTION + 2)
#define WIID_AUCTION_MONEY_FRAME_RIGHT			(WIID_AUCTION + 3)
#define WIID_AUCTION_MONEY_ELY_BG				(WIID_AUCTION + 4)
#define WIID_AUCTION_MONEY_ELY_TITLE			(WIID_AUCTION + 5)
#define WIID_AUCTION_MONEY_ELY_TEXT				(WIID_AUCTION + 6)
#define WIID_AUCTION_MONEY_ELY_ICON				(WIID_AUCTION + 7)
#define WIID_AUCTION_MONEY_POINT_BG				(WIID_AUCTION + 8)
#define WIID_AUCTION_MONEY_POINT_TITLE			(WIID_AUCTION + 9)
#define WIID_AUCTION_MONEY_POINT_TEXT			(WIID_AUCTION + 10)
#define WIID_AUCTION_MONEY_POINT_ICON			(WIID_AUCTION + 11)
#define WIID_AUCTION_MONEY_OZ_BG				(WIID_AUCTION + 12)
#define WIID_AUCTION_MONEY_OZ_TITLE				(WIID_AUCTION + 13)
#define WIID_AUCTION_MONEY_OZ_TEXT				(WIID_AUCTION + 14)
#define WIID_AUCTION_MONEY_OZ_ICON				(WIID_AUCTION + 15)

// MainFrame
#define WIID_AUCTION_MAIN_TAB				(WIID_AUCTION + 22)		// (+3)
#define WIID_AUCTION_MAIN_EQUIP				(WIID_AUCTION + 25)
#define WIID_AUCTION_MAIN_ITEM				(WIID_AUCTION + 26)
#define WIID_AUCTION_MAIN_CLOSE				(WIID_AUCTION + 27)
#define WIID_AUCTION_MAIN_TITLE_ICON		(WIID_AUCTION + 28)
#define WIID_AUCTION_MAIN_TITLE_TEXT		(WIID_AUCTION + 29)
#define WIID_AUCTION_MAIN_TITLE_LINE		(WIID_AUCTION + 30)

//////////////////////////////////////////////////////////////////////////
// Search
#define WIID_AUCTION_SEARCH_FRAME			(WIID_AUCTION + 101)
#define WIID_AUCTION_SEARCH_SEL_BG			(WIID_AUCTION + 102)		// 현재 선택된 메뉴 표시
#define WIID_AUCTION_SEARCH_SEL_ICON		(WIID_AUCTION + 103)
#define WIID_AUCTION_SEARCH_SEL_TEXT		(WIID_AUCTION + 104)
#define WIID_AUCTION_SEARCH_SEL_LINE		(WIID_AUCTION + 105)		// (+2)
#define WIID_AUCTION_SEARCH_MENU_BUTTON		(WIID_AUCTION + 111)		// AUCTION_MAX_MENU (+8)
#define WIID_AUCTION_SEARCH_MENU_ICON		(WIID_AUCTION + 121)		// AUCTION_MAX_MENU (+8)
#define WIID_AUCTION_SEARCH_MENU_TEXT		(WIID_AUCTION + 131)		// AUCTION_MAX_MENU (+8)
#define WIID_AUCTION_POPUP_FRAME			(WIID_AUCTION + 35)			// Search PopUP Menu
//#define WIID_AUCTION_POPUP_SELECT			(WIID_AUCTION + 36)			// PopUP Menu Select
#define WIID_AUCTION_POPUP_LIST				(WIID_AUCTION + 36)			// 3*20 (+60)

#define WIID_AUCTION_SEARCH_LIST_LINE		(WIID_AUCTION + 31)			// 리스트 상단 라인
#define WIID_AUCTION_SEARCH_LIST_BG			(WIID_AUCTION + 32)			// 리스트 배경

#define WIID_AUCTION_SEARCH_LIST_QUALITY	(WIID_AUCTION + 151)		// 품질
#define WIID_AUCTION_SEARCH_LIST_LEVEL		(WIID_AUCTION + 152)		// 레벨
#define WIID_AUCTION_SEARCH_LIST_DURATION	(WIID_AUCTION + 153)		// 내구도
#define WIID_AUCTION_SEARCH_LIST_FINISHTIME	(WIID_AUCTION + 154)		// 마감
#define WIID_AUCTION_SEARCH_LIST_SELLER		(WIID_AUCTION + 155)		// 판매자
#define WIID_AUCTION_SEARCH_LIST_PRICE		(WIID_AUCTION + 156)		// 현재 입찰가
#define WIID_AUCTION_SEARCH_SORT_ARROW		(WIID_AUCTION + 161)		// Sort Arrow(+6)
#define WIID_AUCTION_SEARCH_LIST			(WIID_AUCTION + 171)		// 18*6 = (+108)
#define WIID_AUCTION_SEARCH_PAGE			(WIID_AUCTION + 821)		// 하단 페이지 (+20)

#define WIID_AUCTION_SEARCH_BT_ITEM			(WIID_AUCTION + 301)		// 아이템 검색 배경
#define WIID_AUCTION_SEARCH_BT_LEVEL_TITLE	(WIID_AUCTION + 302)		// 레벨
#define WIID_AUCTION_SEARCH_BT_LEVEL_DASH	(WIID_AUCTION + 303)
#define WIID_AUCTION_SEARCH_BT_LEVEL_MIN	(WIID_AUCTION + 304)
#define WIID_AUCTION_SEARCH_BT_LEVEL_MAX	(WIID_AUCTION + 306)
#define WIID_AUCTION_SEARCH_BT_QUAL_TITLE	(WIID_AUCTION + 308)		// 품질
#define WIID_AUCTION_SEARCH_BT_QUAL_COMBO	(WIID_AUCTION + 321)		// 품질 콤보 (+20)
#define WIID_AUCTION_SEARCH_BT_USEABLE_TITLE	(WIID_AUCTION + 309)	// 사용 가능 레벨
#define WIID_AUCTION_SEARCH_BT_USEABLE_BTN		(WIID_AUCTION + 310)	
#define WIID_AUCTION_SEARCH_BT_NAME_TITLE	(WIID_AUCTION + 311)		// 검색 내용
#define WIID_AUCTION_SEARCH_BT_NAME_BG		(WIID_AUCTION + 312)
#define WIID_AUCTION_SEARCH_BT_NAME_EDIT	(WIID_AUCTION + 313)

#define WIID_AUCTION_SEARCH_BT_MONEY		(WIID_AUCTION + 351)		// 금액검색 배경
#define WIID_AUCTION_SEARCH_BT_MONEY_MIN	(WIID_AUCTION + 352)		// 금액검색(MIN) (+2)
#define WIID_AUCTION_SEARCH_BT_MONEY_DASH	(WIID_AUCTION + 354)		// 금액검색
#define WIID_AUCTION_SEARCH_BT_MONEY_MAX	(WIID_AUCTION + 355)		// 금액검색(MAX) (+2)
#define WIID_AUCTION_SEARCH_BT_MONEY_ICON	(WIID_AUCTION + 357)		// 금액 아이콘

#define WIID_AUCTION_SEARCH_BT_SEARCH		(WIID_AUCTION + 361)		// 찾기
#define WIID_AUCTION_SEARCH_BT_CLOSE		(WIID_AUCTION + 362)		// 나가기

//////////////////////////////////////////////////////////////////////////
// Buy
#define WIID_AUCTION_BUY_FRAME				(WIID_AUCTION + 400)
#define WIID_AUCTION_BUY_LIST_LINE			(WIID_AUCTION + 401)		// 리스트 상단 라인
#define WIID_AUCTION_BUY_LIST_BG			(WIID_AUCTION + 402)		// 리스트 배경
#define WIID_AUCTION_BUY_BT_CLOSE			(WIID_AUCTION + 403)		// 나가기

#define WIID_AUCTION_BUY_LIST_QUALITY		(WIID_AUCTION + 411)		// 품질
#define WIID_AUCTION_BUY_LIST_LEVEL			(WIID_AUCTION + 412)		// 레벨
#define WIID_AUCTION_BUY_LIST_DURATION		(WIID_AUCTION + 413)		// 내구도
#define WIID_AUCTION_BUY_LIST_FINISHTIME	(WIID_AUCTION + 414)		// 마감
#define WIID_AUCTION_BUY_LIST_SELLER		(WIID_AUCTION + 415)		// 판매자
#define WIID_AUCTION_BUY_LIST_PRICE			(WIID_AUCTION + 416)		// 현재 입찰가
#define WIID_AUCTION_BUY_SORT_ARROW			(WIID_AUCTION + 421)		// 현재 입찰가 (+6)
#define WIID_AUCTION_BUY_LIST				(WIID_AUCTION + 441)		// 18*7 = (+126)
#define WIID_AUCTION_BUY_PAGE				(WIID_AUCTION + 851)		// 하단 페이지 (+20)


//////////////////////////////////////////////////////////////////////////
// Sale
#define WIID_AUCTION_SALE_FRAME				(WIID_AUCTION + 600)

#define WIID_AUCTION_REGI_TITLE				(WIID_AUCTION + 601)		//  등록창
#define WIID_AUCTION_REGI_FRAME				(WIID_AUCTION + 602)
#define WIID_AUCTION_REGI_SALE_TITLE		(WIID_AUCTION + 603)
#define WIID_AUCTION_REGI_SALE_TEXT			(WIID_AUCTION + 604)
#define WIID_AUCTION_REGI_SALE_LEFT			(WIID_AUCTION + 605)
#define WIID_AUCTION_REGI_SALE_RIGHT		(WIID_AUCTION + 606)

#define WIID_AUCTION_REGI_ITEM_BG			(WIID_AUCTION + 608)		// 아이템 등록
#define WIID_AUCTION_REGI_ITEM				(WIID_AUCTION + 609)
#define WIID_AUCTION_REGI_ITEM_COUNT10		(WIID_AUCTION + 610)
#define WIID_AUCTION_REGI_ITEM_COUNT01		(WIID_AUCTION + 611)
#define WIID_AUCTION_REGI_ITEM_ARROW		(WIID_AUCTION + 612)
#define WIID_AUCTION_REGI_ITEM_HELP			(WIID_AUCTION + 613)
#define WIID_AUCTION_REGI_ITEM_HELP_MSG		(WIID_AUCTION + 614)
#define WIID_AUCTION_REGI_MONEY				(WIID_AUCTION + 615)		// 큰 머니 아이콘
#define WIID_AUCTION_REGI_MONEY_BG			(WIID_AUCTION + 616)		// 캐쉬, 포인트, 엘리 등록
#define WIID_AUCTION_REGI_MONEY_EDIT		(WIID_AUCTION + 617)
#define WIID_AUCTION_REGI_MONEY_ICON		(WIID_AUCTION + 618)
#define WIID_AUCTION_REGI_MONEY_HELP		(WIID_AUCTION + 619)

#define WIID_AUCTION_REGI_PRICE_FRAME		(WIID_AUCTION + 621)		// 경매 가격 배경
#define WIID_AUCTION_REGI_PRICE_ST_TEXT		(WIID_AUCTION + 622)		// 경매 시작가
#define WIID_AUCTION_REGI_PRICE_ST_BG		(WIID_AUCTION + 623)
#define WIID_AUCTION_REGI_PRICE_ST_EDIT		(WIID_AUCTION + 624)
#define WIID_AUCTION_REGI_PRICE_ST_ICON		(WIID_AUCTION + 625)
#define WIID_AUCTION_REGI_SPOT_ST_TEXT		(WIID_AUCTION + 626)		// 즉시 구매가
#define WIID_AUCTION_REGI_SPOT_ST_BG		(WIID_AUCTION + 627)
#define WIID_AUCTION_REGI_SPOT_ST_EDIT		(WIID_AUCTION + 628)
#define WIID_AUCTION_REGI_SPOT_ST_ICON		(WIID_AUCTION + 629)
#define WIID_AUCTION_REGI_DURATION_BG		(WIID_AUCTION + 631)		// 경매 기간
#define WIID_AUCTION_REGI_DURATION_TITLE	(WIID_AUCTION + 632)
#define WIID_AUCTION_REGI_DURATION_TEXT		(WIID_AUCTION + 633)		// (+3)
#define WIID_AUCTION_REGI_DURATION_BTN		(WIID_AUCTION + 636)		// (+3)
#define WIID_AUCTION_REGI_DEPOSIT_BG		(WIID_AUCTION + 641)		// 보증금
#define WIID_AUCTION_REGI_DEPOSIT_TITLE		(WIID_AUCTION + 642)
#define WIID_AUCTION_REGI_DEPOSIT_TEXT		(WIID_AUCTION + 643)
#define WIID_AUCTION_REGI_DEPOSIT_ICON		(WIID_AUCTION + 644)

#define WIID_AUCTION_REGI_OK_BUTTON			(WIID_AUCTION + 646)		// 경매 등록
#define WIID_AUCTION_REGI_CANCEL_BUTTON		(WIID_AUCTION + 647)		// 경매 등록 초기화

#define WIID_AUCTION_SALE_LIST_LINE1		(WIID_AUCTION + 651)		// 리스트 세로 라인
#define WIID_AUCTION_SALE_LIST_LINE2		(WIID_AUCTION + 652)		// 리스트 상단 라인
#define WIID_AUCTION_SALE_LIST_BG			(WIID_AUCTION + 653)		// 리스트 배경
#define WIID_AUCTION_SALE_BT_CLOSE			(WIID_AUCTION + 655)		// 나가기
#define WIID_AUCTION_SALE_TIP_BG			(WIID_AUCTION + 656)		// 툴팁배경
#define WIID_AUCTION_SALE_TIP_MSG			(WIID_AUCTION + 657)		// 툴팁메시지

#define WIID_AUCTION_SALE_LIST_QUALITY		(WIID_AUCTION + 661)		// 품질
#define WIID_AUCTION_SALE_LIST_FINISHTIME	(WIID_AUCTION + 662)		// 마감
#define WIID_AUCTION_SALE_LIST_SELLER		(WIID_AUCTION + 663)		// 판매자
#define WIID_AUCTION_SALE_LIST_PRICE		(WIID_AUCTION + 664)		// 현재 입찰가
#define WIID_AUCTION_SALE_SORT_ARROW		(WIID_AUCTION + 671)		// 현재 입찰가 (+6)
#define WIID_AUCTION_SALE_LIST				(WIID_AUCTION + 681)		// 18*7 = (+126)
#define WIID_AUCTION_SALE_PAGE				(WIID_AUCTION + 881)		// 하단 페이지 (+20)


#define AUCTION_BG_IMG				10			// 배경 이미지 갯수 (9+1)
#define AUCTION_MAX_MENU			8			// 카테고리 메뉴의 갯수는 최대 8
#define AUCTION_SEARCH_LIST			6			// 검색하기 목록 갯수
#define AUCTION_BUY_LIST			7			// 입찰현황 목록 갯수
#define AUCTION_SALE_LIST			7			// 경매하기 목록 갯수
#define AUCTION_MAX_LIST_ITEM		18			// 한목록당 18개의 컨텐츠
#define AUCTION_BUNDLE_PAGE			10			// 10페이지 단위로 보여짐
#define AUCTION_SEARCH_TIMER		3.0f		// SearchTimer



enum AUCTION_TAB
{
	AUCTION_SEARCH_TAB	= 0,
	AUCTION_BUY_TAB		= 1,
	AUCTION_SALE_TAB	= 2,
} ;


//////////////////////////////////////////////////////////////////////////
//	
//////////////////////////////////////////////////////////////////////////
// 메뉴의 Detail
struct AUCTION_MENU_DETAIL
{
	int			iDetail_ID ;		// Detail ID (소분류 ID)
	std::string strDetail ;			// 소분류 이름
	RECT		rcIconSrc ;			// 작은 아이콘 좌표
	RECT		rcBigIconSrc ;		// 큰 아이콘 좌표
};

// 경매 메뉴 LDT
struct AUCTION_SEARCH_MENU
{
	int			iItemID ;			// ItemID
	int			iCategory_ID ;		// Category ID
	std::string	strCategory ;		// 대분류 이름
	RECT		rcIconSrc ;			// 아이콘 좌표
	std::vector<AUCTION_MENU_DETAIL*>	vpDetail ; 

	void Init() {
		iItemID			= 0 ;
		iCategory_ID	= 0 ;
		vpDetail.clear();
	}
	void Clear()
	{
		std::vector<AUCTION_MENU_DETAIL*>::iterator Iter = vpDetail.begin() ;
		for( ; Iter != vpDetail.end(); ++Iter )
		{
			SAFE_DELETE( *Iter ) ;
		}
		vpDetail.clear() ;
		iItemID			= 0 ;
		iCategory_ID	= 0 ;
	}
};




//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
enum CASH_TRADE ;
class SPWindow;
class SPWindowEdit;
class SPWindowAuctionListUNIT;
class SPWindowAuctionSaleUNIT;
class SPWindowRowUNIT;
class SPAuctionAttr ;
class SPItem;
class SPTimer;
struct AUCTION_TYPE_INFO;
struct AUCTION_SEARCH_KEY ;
enum AUCTION_SORT_KEY ;
class SPWindowAuction : public SPWindow
{
public:
	SPWindowAuction(WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowAuction( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowAuction();

	virtual void	Init();
	virtual void	Clean();

	virtual void	Process( float fTime );
	virtual void	Render( float fTime );

	virtual void	Show( bool bWithChild = true );
	virtual void	Hide(bool bSendServer = true);

private:
	void InitMoneyFrame() ;
	void InitAllMoneyFrame() ;		// Ely & Point & Cash (ALL)
	void InitElyPointFrame() ;		// Ely & Point
	void InitElyCashFrame() ;		// Ely & Cash
	void InitElyFrame() ;			// Ely만

	void InitAuctionFrame() ;
	void InitBackground() ;

	void InitAuctionSearch() ;
	void InitAuctionSearchMenu() ;
	void InitAuctionPopMenu() ;

	void InitAuctionBuy() ;
	void InitAuctionSale() ;

	void InitShowAuctionSearch() ;
	void ShowAuctionSearch() ;
	void ShowAuctionBuy() ;
	void ShowAuctionSale() ;

	void UpdateSortArrow(AUCTION_TAB eTab) ;			// 탭별 Sort Arrow 설정
	void UpdateMoneyFrame(CASH_TRADE eTrade) ;			// Localize에 따른 MoneyFrame Update

	//////////////////////////////////////////////////////////////////////////
	// Search
	void LoadMenuLDT() ;
	void SetPopMenu(int iSelCategory) ;						// Set & Show PopUp Menu (Animation-Set)
	void ShowPopMenu(int iSelCategory) ;					// Show Menu
	void HidePopMenu() ;									// Hide Menu
	void SetSelectedTitle(bool bClear=false) ;				// 선택된 메뉴 표시
	void ShowSearchList(int iPage) ;						// 해당 페이지 보이기
	bool UpdateSearchKey() ;								// SearchKey 정보를 가져온다.
	void SetSearchBottomMenu(int iCategory, int iDetail, bool bInit=true) ;	// 하단의 세부검색 표시
	void SetSearchSortButton(int iCategory) ;				// 카테고리별로 Sort버튼을 구분 표시
	bool CheckAuctionBidable(SPAuctionAttr* pItemAttr, bool isBid) ;	// 입찰/즉구 가능여부 체크
	void SetShowSearchGuide(bool bShowSearchGuide)			{	m_bShowSearchGuide = bShowSearchGuide ;		}

	//////////////////////////////////////////////////////////////////////////
	// Buy
	void ShowBuyList(int iPage) ;

	//////////////////////////////////////////////////////////////////////////
	// Sale
	void SetAuctionSaleType(bool bDefault = false) ;		// 판매 타입 셋팅
	void SetAuctionSaleTypeIcon(AUCTION_TYPE eType) ;
	void SetSaleDuration(int iDuration = 0) ;				// 경매기간 설정
	void SetSaleArrowIcon(int iIndex) ;						// Arrow Icon
	void ShowSaleList(int iPage) ;							// 해당 페이지 보이기
	void InitBidMoney() ;									// 경매가격 초기화
	void UpdateEnrolFee(bool bInit=false) ;					// 등록보증금 업데이트
	bool CheckCreateItem(AUCTION_TYPE_INFO* pAuctionTypeInfo) ;	// 아이템 등록 유효체크
	bool CheckSaleItem(bool bDrag=true , SPItem* pClickItem=NULL ) ;	// Drag or Shift+Click 아이템 체크
	bool SetSaleItemInfo( SPItem* pkItem = NULL ) ;			// 판매 아이템 설정
	SPItem* GetDragItem();									// Drag Item Check
	void SetSaleItemCount(int iCount) ;						// 판매 아이템 수량 표시
	void SetAuctionItemLock( SPItem* pkItem , int iIndex ) ;// 등록 아이템의 Blocking
	void SetAcutionItemAllUnlock() ;




private:
	SPWindow*	m_pWindowSearch;
	SPWindow*	m_pWindowBuy;
	SPWindow*	m_pWindowSale;
	SPWindow*	m_pWindowSearchPopup;
	SPWindow*	m_pWindowSaleTip;
	SPWindow*	m_pAuctionMsg ;
	SPWindow*	m_pSearchSort[AUCTION_SORT_MAX] ;
	SPWindow*	m_pBuySort[AUCTION_SORT_MAX] ;
	SPWindow*	m_pSaleSort[AUCTION_SORT_MAX] ;

	SPWindowAuctionListUNIT*	m_pWindowSearchList[AUCTION_SEARCH_LIST];
	SPWindowAuctionListUNIT*	m_pWindowBuyList[AUCTION_BUY_LIST];
	SPWindowAuctionSaleUNIT*	m_pWindowSaleList[AUCTION_SALE_LIST];

	SPWindowEdit*	m_pWindowSearchEdit;

	SPTexture*	m_pTextureBG;
	SPTexture*	m_pTextureGuide ;
	RECT		m_rcBGDest[ AUCTION_BG_IMG ];
	RECT		m_rcBGSrc[ AUCTION_BG_IMG ];
	RECT		m_rcGuideDest ;
	RECT		m_rcGuideSrc ;

	float		m_fMoneyUpdateTime ;		// 경매 머니정보 업데이트(0.1초마다)

	int			m_iSearchMenuMax ;			// 메인메뉴의 MAX (Set in LoadLDT)
	int			m_iSearchPopMax ;			// 팝업메뉴의 MAX (Set in LoadLDT)

	int			m_iSelectedTab ;			// 현재 보여지는 탭(1:Search, 2:Buy, 3:Sale)
	int			m_iPreShowPopup ;			// 이전 보여준 Popup Category (팝업 메뉴의 생성 효율을 위해)
	int			m_iSelectedCategory ;		// Category
	int			m_iSelectedDetail ;			// Popup Menu

	std::string				m_strMenuIconPath ;				// 메뉴 아이콘 소스파일
	AUCTION_SEARCH_MENU		m_stMenu[AUCTION_MAX_MENU] ;
	std::vector<SPWindowRowUNIT*>	m_vpPopMenu ;
	int			m_iPopMenuMode ;		// 메뉴 상태
	int			m_iPopMenuSize ;		// 메뉴 사이즈
	int			m_iPopMenuDestSize ;	// 메뉴 셋팅 사이즈

	int			m_iSearchQuality ;		// 검색 품질
	int			m_bSearchUsable ;		// 사용가능 레벨 체크
	AUCTION_SEARCH_KEY*	m_pSearchKey ;		// 검색 Key
	AUCTION_SEARCH_KEY*	m_pSuccessKey ;	// 마지막 성공한 SearchKey

	SPTimer*	m_pSearchTimer ;		// Search Timer (5Sed)
	SPWindow*	m_pSearchButton ;		// Search Button

	AUCTION_TYPE		m_iSaleType ;		// 판매유형( 아이템/캐쉬/포인트/엘리)
	int					m_iSaleIndex;		// 판매유형 Index 
	AUCTION_DURATION	m_iSaleDuration ;	// 경매기간 선택( 단기/중기/장기)
	AUCTION_TYPE_INFO*	m_pSaleTypeInfo ;	// 판매 유형별 정보
	int					m_iSaleArrowIndex ;	// 판매 화살표 Index
	SPTimer*			m_pSaleTimer ;		// 판매 화살표 Timer
	SPTimer*			m_pSaleTipTimer ;	// 수수료 툴팁타이머
	SPItem*				m_pSaleItem ;		// 판매 아이템
	int					m_iSaleMoney ;		// 판매 금액(최대 9억)
	INT64				m_iSaleStartMoney ;	// 경매 시작가
	INT64				m_iSaleSpotMoney ;	// 경매 즉구가
	INT64				m_iEnrolFee ;		// 경매 보증금

	int			m_iSearchCurPage ;		// 검색에서 현재 보고있는 페이지
	int			m_iBuyCurPage ;			// 입찰에서 현재 보고있는 페이지
	int			m_iSaleCurPage ;		// 경매하기에서 현재 보고있는 페이지
	bool		m_bShowSearchGuide ;	// 검색 도움말 보여주기
	bool		m_bTest ;				// 테스트

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	virtual int OnAuctionServerClose(WPARAM wParam, LPARAM lParam) ;		// Server Down
	virtual int OnAuctionMoneyUpdate(WPARAM wParam, LPARAM lParam) ;		// Money Update
	virtual int OnAuctionSetSearchName(WPARAM wParam, LPARAM lParam) ;		// Shift+Click -> Search Name
	//virtual int OnAuctionErrorNotice(WPARAM wParam, LPARAM lParam) ;		// MSG_TYPE, MSG

	virtual int OnSendAuctionSpotBuy(WPARAM wParam, LPARAM lParam) ;
	virtual int OnSendAuctionBidBuy(WPARAM wParam, LPARAM lParam) ;
	virtual int OnSendAuctionGetNext(WPARAM wParam, LPARAM lParam) ;
	virtual int OnSendAuctionSaleCancel(WPARAM wParam, LPARAM lParam) ;

	virtual int OnMainSearch(WPARAM wParam, LPARAM lParam);
	virtual int OnMainBuy(WPARAM wParam, LPARAM lParam);
	virtual int OnMainSale(WPARAM wParam, LPARAM lParam);
	virtual int OnMyEquip(WPARAM wParam, LPARAM lParam);
	virtual int OnMyInven(WPARAM wParam, LPARAM lParam);
	virtual int OnClose(WPARAM wParam, LPARAM lParam);
	virtual	int OnActivate(WPARAM wParam, LPARAM lParam) ;
	virtual	int OnFocus(WPARAM wParam, LPARAM lParam) ;

	// Search
	virtual int OnReceiveAuctionSearchList(WPARAM wParam, LPARAM lParam);		// 리스트
	virtual int OnCategorySelect(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int OnDetailSelect(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual	int OnCategoryIconCursorON(unsigned int iID, WPARAM wParam, LPARAM lParam) ;
	virtual int OnCategoryIconCursorOUT(unsigned int iID, WPARAM wParam, LPARAM lParam) ;
	virtual	int OnCategoryTextCursorON(unsigned int iID, WPARAM wParam, LPARAM lParam) ;
	virtual int OnCategoryTextCursorOUT(unsigned int iID, WPARAM wParam, LPARAM lParam) ;
	virtual int OnSearchListBidClick(unsigned int iID, WPARAM wParam, LPARAM lParam);	// 입찰
	virtual int OnSearchListSpotClick(unsigned int iID, WPARAM wParam, LPARAM lParam);	// 즉시구매
	virtual int OnSearchListClick(unsigned int iID, WPARAM wParam, LPARAM lParam);		// 리스트 내용
	virtual int OnSearchListPage(WPARAM wParam, LPARAM lParam);
	virtual int OnChangeSearchCombo(WPARAM wParam, LPARAM lParam);

	virtual int OnSortSearchQuality(WPARAM wParam, LPARAM lParam);		// 정렬 (품질)
	virtual int OnSortSearchLevel(WPARAM wParam, LPARAM lParam);		// 정렬 (레벨)
	virtual int OnSortSearchDuration(WPARAM wParam, LPARAM lParam);		// 정렬 (내구도)
	virtual int OnSortSearchFinish(WPARAM wParam, LPARAM lParam);		// 정렬 (마감)
	virtual int OnSortSearchPrice(WPARAM wParam, LPARAM lParam);		// 정렬 (입찰가)

	virtual int OnSearchUsableCheck(WPARAM wParam, LPARAM lParam);		// 사용가능한 레벨 체크
	virtual int OnSearchNameSetFocus(WPARAM wParam, LPARAM lParam);		// 검색 이름 SetFocus
	virtual int OnSearchNameKillFocus(WPARAM wParam, LPARAM lParam);	// 검색 KillFocus
	virtual int OnSearchItem(WPARAM wParam, LPARAM lParam);				// 찾기 버튼


	// Buy
	virtual int OnReceiveAuctionBuyList(WPARAM wParam, LPARAM lParam);			// 리스트
	//virtual int OnReceiveAuctionBuyUpdate(WPARAM wParam, LPARAM lParam);		// 리스트 업데이트
	virtual int OnSortBuyQuality(WPARAM wParam, LPARAM lParam);		// 정렬 (품질)
	virtual int OnSortBuyLevel(WPARAM wParam, LPARAM lParam);		// 정렬 (레벨)
	virtual int OnSortBuyDuration(WPARAM wParam, LPARAM lParam);	// 정렬 (내구도)
	virtual int OnSortBuyFinish(WPARAM wParam, LPARAM lParam);		// 정렬 (마감)
	virtual int OnSortBuyPrice(WPARAM wParam, LPARAM lParam);		// 정렬 (입찰가)
	virtual int OnBuyListBidClick(unsigned int iID, WPARAM wParam, LPARAM lParam);		// 입찰
	virtual int OnBuyListSpotClick(unsigned int iID, WPARAM wParam, LPARAM lParam);		// 즉시 구매
	virtual int OnBuyListPage(WPARAM wParam, LPARAM lParam);

	// Sale
	virtual int OnReceiveAuctionSaleList(WPARAM wParam, LPARAM lParam);			// 리스트
	//virtual int OnReceiveAuctionSaleUpdate(WPARAM wParam, LPARAM lParam);		// 리스트 업데이트
	virtual int OnSaleRegistryLeft(WPARAM wParam, LPARAM lParam);		// 등록타입 선택(왼쪽)
	virtual int OnSaleRegistryRight(WPARAM wParam, LPARAM lParam);		// 등록타입 선택(오른쪽)
	virtual int OnSaleItemButtonUp(WPARAM wParam, LPARAM lParam);		// 아이템 드래그
	virtual int	OnSaleItemToolTipEnter(WPARAM wParam, LPARAM lParam);	// 판매 등록 아이템 Tooltip
	virtual int	OnSaleItemToolTipOut(WPARAM wParam, LPARAM lParam);		// 판매 등록 아이템 Tooltip
	virtual int	OnSaleItemToolTipMove(WPARAM wParam, LPARAM lParam);	// 판매 등록 아이템 Tooltip
	virtual int OnSaleMoneyKeyDown(WPARAM wParam, LPARAM lParam);		// 경매 시작금액입력
	virtual int OnSaleDuration(unsigned int iID, WPARAM wParam, LPARAM lParam);		// 경매 기간선택
	virtual int OnSaleRegistItem(WPARAM wParam, LPARAM lParam);			// 아이템 등록하기
	virtual int OnSaleClearItem(WPARAM wParam, LPARAM lParam);			// 아이템 등록취소
	virtual int OnSortSaleQuality(WPARAM wParam, LPARAM lParam);		// 정렬 (품질)
	virtual int OnSortSaleFinish(WPARAM wParam, LPARAM lParam);			// 정렬 (마감)
	virtual int OnSortSalePrice(WPARAM wParam, LPARAM lParam);			// 정렬 (입찰가)
	virtual int OnSaleListCancelClick(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int OnSaleListPage(WPARAM wParam, LPARAM lParam);

	// }}


};



