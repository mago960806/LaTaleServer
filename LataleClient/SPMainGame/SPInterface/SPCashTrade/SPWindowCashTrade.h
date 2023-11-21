#pragma once


#define WIID_CASH_TRADE_TOP						(WIID_CASH_TRADE	+	10 )
#define WIID_CASH_TRADE_CENTER					(WIID_CASH_TRADE	+	20 )
#define WIID_CASH_TRADE_BOTTOM					(WIID_CASH_TRADE	+	30 )

#define WIID_CASH_TRADE_OK						(WIID_CASH_TRADE	+	40 )
#define WIID_CASH_TRADE_CANCEL					(WIID_CASH_TRADE	+	41 )
#define WIID_CASH_TRADE_HELP					(WIID_CASH_TRADE	+	42 )

#define WIID_CASH_TRADE_ITEMSKIN				(WIID_CASH_TRADE	+	50 )
#define WIID_CASH_TRADE_ITEM					(WIID_CASH_TRADE	+	51 )
#define WIID_CASH_TRADE_ITEMNAME				(WIID_CASH_TRADE	+	52 )

#define WIID_CASH_TRADE_COSTSKIN				(WIID_CASH_TRADE	+	60 )
#define WIID_CASH_TRADE_COST					(WIID_CASH_TRADE	+	61 )
#define WIID_CASH_TRADE_COSTICON				(WIID_CASH_TRADE	+	62 )

#define WIID_CASH_TRADE_CUR_CASH_TEXTIMG 		(WIID_CASH_TRADE	+	70 )		/**< 보유한 오즈 텍스트 이미지 <br> */
#define WIID_CASH_TRADE_CUR_CASH				(WIID_CASH_TRADE	+	71 )
#define WIID_CASH_TRADE_CUR_CASH_ICON			(WIID_CASH_TRADE	+	72 )

#define WIID_CASH_TRADE_USE_CASH_TEXTIMG		(WIID_CASH_TRADE	+	80 )		/**< 사용할 오즈 텍스트 이미지 <br> */
#define WIID_CASH_TRADE_USE_CASH				(WIID_CASH_TRADE	+	81 )
#define WIID_CASH_TRADE_USE_CASH_ICON			(WIID_CASH_TRADE	+	82 )

#define WIID_CASH_TRADE_USE_POINT_TEXTIMG		(WIID_CASH_TRADE	+	90 )		/**< 사용할 포인트 텍스트 이미지 <br> */
#define WIID_CASH_TRADE_USE_POINT				(WIID_CASH_TRADE	+	91 )
#define WIID_CASH_TRADE_USE_POINT_ICON			(WIID_CASH_TRADE	+	92 )
#define WIID_CASH_TRADE_USE_POINT_INC			(WIID_CASH_TRADE	+	93 )		/**< 포인트 증가 <br> */
#define WIID_CASH_TRADE_USE_POINT_DEC			(WIID_CASH_TRADE	+	94 )		/**< 포인트 감소 <br> */

#define WIID_CASH_TRADE_PAY_TEXTIMG				(WIID_CASH_TRADE	+	100 )		/**< 결제할 금액 텍스트 이미지 <br> */
#define WIID_CASH_TRADE_PAY						(WIID_CASH_TRADE	+	101 )
#define WIID_CASH_TRADE_PAY_ICON				(WIID_CASH_TRADE	+	102 )

#define WIID_CASH_TRADE_CHANGE_TEXTIMG			(WIID_CASH_TRADE	+	110 )		/**< 결제후 금액 텍스트 이미지 <br> */
#define WIID_CASH_TRADE_CHANGE					(WIID_CASH_TRADE	+	111 )
#define WIID_CASH_TRADE_CHANGE_ICON				(WIID_CASH_TRADE	+	112 )

struct SPCashItemAttr;

class SPWindowCashTrade : public SPWindow {

public:	
	SPWindowCashTrade(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowCashTrade();

	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);


	virtual void Process(float fTime);
	virtual void Render(float fTime);

protected:
	void InitSubControl();
	
	void UpdatePointControl(SPCashItemAttr* pCashItem);	

	bool UpdateControl();

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnOk					(WPARAM wParam, LPARAM lParam);
	int OnCancel				(WPARAM wParam, LPARAM lParam);
	int OnPointInc				(WPARAM wParam, LPARAM lParam);
	int OnPointDec				(WPARAM wParam, LPARAM lParam);
	// }}

private:
	int			m_iPointTradeUnit;

	SPWindow*	m_pItemName;						/**< 아이템 이름 <br> */
	SPWindow*	m_pCost;							/**< 아이템 가격 <br> */
	SPWindow*	m_pCostIcon;						/**< 아이템 가격 종류 캐시인지 포인트인지 <br> */
	SPWindow*	m_pItemIcon;						/**< 아이템 아이콘 <br> */

	SPWindow*	m_pCurCash;							/**< 보유한 오즈 <br> */
	SPWindow*	m_pUseCash;							/**< 사용할 오즈 <br> */
	int			m_iUseCash;							/**< <br> */
	
	SPWindow*	m_pUsePoint;						/**< 사용할 포인트 <br> */
	int			m_iUsePoint;						/**< 사용할 포인트 <br> */	

	SPWindow*	m_pPayCash;							/**< 결제할 금액 <br> */
	SPWindow*	m_pPayIcon;							/**< 결제할 금액 아이콘 <br> */
	
	SPWindow*	m_pChangeCash;						/**< 결제후 잔액 <br> */
	int			m_iChangeCash;						/**< <br> */
	SPWindow*	m_pChangeIcon;						/**< 결제후 잔액 아이콘 <br> */

	SPWindow*	m_pHelp;
};
