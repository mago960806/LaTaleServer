#pragma once


#define WIID_CASH_TRADE_SELECT_BASE					(WIID_CASH_TRADE_SELECT	+	1 )
#define WIID_CASH_TRADE_SELECT_ITEM_NAME			(WIID_CASH_TRADE_SELECT	+	10 )

#define WIID_CASH_TRADE_SELECT_CLOSE				(WIID_CASH_TRADE_SELECT	+	20 )
#define WIID_CASH_TRADE_SELECT_WISH					(WIID_CASH_TRADE_SELECT	+	30 )		/**< 찜 <br> */
#define WIID_CASH_TRADE_SELECT_GIFT					(WIID_CASH_TRADE_SELECT	+	40 )
#define WIID_CASH_TRADE_SELECT_BUY					(WIID_CASH_TRADE_SELECT	+	50 )

#define WIID_CASH_TRADE_SELECT_MALE					(WIID_CASH_TRADE_SELECT	+	60 )		/**< 성별 <br> */
#define WIID_CASH_TRADE_SELECT_FEMALE				(WIID_CASH_TRADE_SELECT	+	61 )

#define WIID_CASH_TRADE_SELECT_ITEMBUTTON			(WIID_CASH_TRADE_SELECT	+	70 )
#define WIID_CASH_TRADE_SELECT_ITEMSKIN				(WIID_CASH_TRADE_SELECT	+	71 )
#define WIID_CASH_TRADE_SELECT_ITEMICON				(WIID_CASH_TRADE_SELECT	+	72 )
#define WIID_CASH_TRADE_SELECT_ITEMEVENT			(WIID_CASH_TRADE_SELECT	+	73 )		/**< Event Icon <br> */

#define WIID_CASH_TRADE_SELECT_COSTSKIN				(WIID_CASH_TRADE_SELECT	+	80 )
#define WIID_CASH_TRADE_SELECT_COST					(WIID_CASH_TRADE_SELECT	+	81 )
#define WIID_CASH_TRADE_SELECT_COSTICON				(WIID_CASH_TRADE_SELECT	+	82 )


class SPWindowCashTradeSelect : public SPWindow {

public:	
	SPWindowCashTradeSelect(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowCashTradeSelect();

	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);


	virtual void Process(float fTime);
	virtual void Render(float fTime);

protected:
	void SetWindowBase();
	void InitSubControl();
	bool UpdateControl();

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnClose					(WPARAM wParam, LPARAM lParam);
	int OnWish					(WPARAM wParam, LPARAM lParam);
	int OnGift					(WPARAM wParam, LPARAM lParam);
	int OnBuy					(WPARAM wParam, LPARAM lParam);
	// }}

private:
	SPTexture* m_pBaseSkin;
	RECT m_rtBaseSrc[WINDOW_BASE_MAX];
	RECT m_rtBaseDest[WINDOW_BASE_MAX];

	SPTexture* m_pDisable;							/**< 비활성 표현용 <br> */
	RECT m_rtSrcDisable;							/**< 비활성 표현용 소스 <br> */
	RECT m_rtMarkSrc[4];							/**< ICON MARK Src <br> */

	SPWindow*	m_pItemName;						/**< 아이템 이름 <br> */
	SPWindow*	m_pCost;							/**< 아이템 가격 <br> */
	SPWindow*	m_pCostIcon;						/**< 아이템 가격 종류 캐시인지 포인트인지 <br> */
	SPWindow*	m_pMaleIcon;						/**< 아이템 남성 <br> */
	SPWindow*	m_pFeMaleIcon;						/**< 아이템 여성 <br> */
	SPWindow*	m_pItemIcon;						/**< 아이템 아이콘 <br> */
	SPWindow*	m_pItemEvent;						/**< 아이템 이벤트 아이콘 <br> */
};


