#pragma once


#define WIID_CASH_TRADE_RESULT_TOP						(WIID_CASH_TRADE_RESULT	+	10 )
#define WIID_CASH_TRADE_RESULT_CENTER					(WIID_CASH_TRADE_RESULT	+	20 )
#define WIID_CASH_TRADE_RESULT_BOTTOM					(WIID_CASH_TRADE_RESULT	+	30 )

#define WIID_CASH_TRADE_RESULT_OK						(WIID_CASH_TRADE_RESULT	+	40 )
//#define WIID_CASH_TRADE_RESULT_CANCEL					(WIID_CASH_TRADE_RESULT	+	41 )
#define WIID_CASH_TRADE_RESULT_HELP						(WIID_CASH_TRADE_RESULT	+	42 )

#define WIID_CASH_TRADE_RESULT_ITEMSKIN					(WIID_CASH_TRADE_RESULT	+	50 )
#define WIID_CASH_TRADE_RESULT_ITEM						(WIID_CASH_TRADE_RESULT	+	51 )
#define WIID_CASH_TRADE_RESULT_ITEMNAME					(WIID_CASH_TRADE_RESULT	+	52 )

#define WIID_CASH_TRADE_RESULT_COSTSKIN					(WIID_CASH_TRADE_RESULT	+	60 )
#define WIID_CASH_TRADE_RESULT_COST						(WIID_CASH_TRADE_RESULT	+	61 )
#define WIID_CASH_TRADE_RESULT_COSTICON					(WIID_CASH_TRADE_RESULT	+	62 )

////
//#define WIID_CASH_TRADE_RESULT_CUR_CASH_TEXTIMG 		(WIID_CASH_TRADE_RESULT	+	70 )		/**< 보유한 오즈 텍스트 이미지 <br> */
//#define WIID_CASH_TRADE_RESULT_CUR_CASH					(WIID_CASH_TRADE_RESULT	+	71 )
//#define WIID_CASH_TRADE_RESULT_CUR_CASH_ICON			(WIID_CASH_TRADE_RESULT	+	72 )
//
//#define WIID_CASH_TRADE_RESULT_USE_CASH_TEXTIMG			(WIID_CASH_TRADE_RESULT	+	80 )		/**< 사용할 오즈 텍스트 이미지 <br> */
//#define WIID_CASH_TRADE_RESULT_USE_CASH					(WIID_CASH_TRADE_RESULT	+	81 )
//#define WIID_CASH_TRADE_RESULT_USE_CASH_ICON			(WIID_CASH_TRADE_RESULT	+	82 )
//
//#define WIID_CASH_TRADE_RESULT_USE_POINT_TEXTIMG		(WIID_CASH_TRADE_RESULT	+	90 )		/**< 사용할 포인트 텍스트 이미지 <br> */
//#define WIID_CASH_TRADE_RESULT_USE_POINT				(WIID_CASH_TRADE_RESULT	+	91 )
//#define WIID_CASH_TRADE_RESULT_USE_POINT_ICON			(WIID_CASH_TRADE_RESULT	+	92 )
//#define WIID_CASH_TRADE_RESULT_USE_POINT_INC			(WIID_CASH_TRADE_RESULT	+	93 )		/**< 포인트 증가 <br> */
//#define WIID_CASH_TRADE_RESULT_USE_POINT_DEC			(WIID_CASH_TRADE_RESULT	+	94 )		/**< 포인트 감소 <br> */

#define WIID_CASH_TRADE_RESULT_PAY_TEXTIMG				(WIID_CASH_TRADE_RESULT	+	100 )		/**< 결제할 금액 텍스트 이미지 <br> */
#define WIID_CASH_TRADE_RESULT_PAY						(WIID_CASH_TRADE_RESULT	+	101 )
#define WIID_CASH_TRADE_RESULT_PAY_ICON					(WIID_CASH_TRADE_RESULT	+	102 )

#define WIID_CASH_TRADE_RESULT_CHANGE_TEXTIMG			(WIID_CASH_TRADE_RESULT	+	110 )		/**< 결제후 금액 텍스트 이미지 <br> */
#define WIID_CASH_TRADE_RESULT_CHANGE					(WIID_CASH_TRADE_RESULT	+	111 )
#define WIID_CASH_TRADE_RESULT_CHANGE_ICON				(WIID_CASH_TRADE_RESULT	+	112 )


class SPWindowCashTradeResult : public SPWindow {

public:	
	SPWindowCashTradeResult(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowCashTradeResult();

	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);


	virtual void Process(float fTime);
	virtual void Render(float fTime);

protected:
	void InitSubControl();
	bool UpdateControl();

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnOk					(WPARAM wParam, LPARAM lParam);
	// }}

private:
	SPWindow*	m_pItemName;						/**< 아이템 이름 <br> */
	SPWindow*	m_pCost;							/**< 아이템 가격 <br> */
	SPWindow*	m_pCostIcon;						/**< 아이템 가격 종류 캐시인지 포인트인지 <br> */
	SPWindow*	m_pItemIcon;						/**< 아이템 아이콘 <br> */	

	SPWindow*	m_pPayCash;							/**< 결제할 금액 <br> */
	SPWindow*	m_pPayIcon;							/**< 결제할 금액 아이콘 <br> */

	SPWindow*	m_pChangeCash;						/**< 결제후 잔액 <br> */
	int			m_iChangeCash;						/**< <br> */
	SPWindow*	m_pChangeIcon;						/**< 결제후 잔액 아이콘 <br> */

	SPWindow*	m_pHelp;
};
