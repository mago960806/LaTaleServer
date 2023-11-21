#pragma once


#define WIID_CASH_GIFT_TOP						(WIID_CASH_GIFT	+	10 )
#define WIID_CASH_GIFT_CENTER					(WIID_CASH_GIFT	+	20 )
#define WIID_CASH_GIFT_BOTTOM					(WIID_CASH_GIFT	+	30 )

#define WIID_CASH_GIFT_OK						(WIID_CASH_GIFT	+	40 )
#define WIID_CASH_GIFT_CANCEL					(WIID_CASH_GIFT	+	41 )
#define WIID_CASH_GIFT_HELP						(WIID_CASH_GIFT	+	42 )

#define WIID_CASH_GIFT_ITEMSKIN					(WIID_CASH_GIFT	+	50 )
#define WIID_CASH_GIFT_ITEM						(WIID_CASH_GIFT	+	51 )
#define WIID_CASH_GIFT_ITEMNAME					(WIID_CASH_GIFT	+	52 )

#define WIID_CASH_GIFT_SEND_SKIN				(WIID_CASH_GIFT	+	60 )
#define WIID_CASH_GIFT_SEND_RECEIVER_IMG		(WIID_CASH_GIFT	+	70 )		/**< 받는 사람 이미지 <br> */
#define WIID_CASH_GIFT_SEND_RECEIVER_SKIN		(WIID_CASH_GIFT	+	71 )		/**< 받는 사람 입력 스킨 <br> */
#define WIID_CASH_GIFT_SEND_RECEIVER			(WIID_CASH_GIFT	+	72 )		/**< 받는 사람 입력 <br> */
#define WIID_CASH_GIFT_SEND_MSG_IMG				(WIID_CASH_GIFT	+	80 )
#define WIID_CASH_GIFT_SEND_MSG_SKIN			(WIID_CASH_GIFT	+	81 )
#define WIID_CASH_GIFT_SEND_MSG					(WIID_CASH_GIFT	+	82 )
#define WIID_CASH_GIFT_SEND_FRIENDLIST_BTN		(WIID_CASH_GIFT	+	90 )		/**< 친구 목록 버튼 <br> */

#define WIID_CASH_GIFT_FRIEND					(WIID_CASH_GIFT	+	100 )		/**< 친구 목록 파업 윈도우 <br> */
#define WIID_CASH_GIFT_FRIEND_LIST				(WIID_CASH_GIFT	+	110 )		/**< 친구목록 틀 <br> */
#define WIID_CASH_GIFT_FRIEND_SCROOL_UP			(WIID_CASH_GIFT	+	115 )
#define WIID_CASH_GIFT_FRIEND_SCROOL			(WIID_CASH_GIFT	+	116 )
#define WIID_CASH_GIFT_FRIEND_SCROOL_DOWN		(WIID_CASH_GIFT	+	117 )

#define WIID_CASH_GIFT_FRIEND_LIST_SKIN			(WIID_CASH_GIFT	+	150 )
#define WIID_CASH_GIFT_FRIEND_LIST_BTN			(WIID_CASH_GIFT	+	160 )		/**< 친구목록 이벤트 처리기 6개 <br> */
#define WIID_CASH_GIFT_FRIEND_LIST_SEL			(WIID_CASH_GIFT	+	170 )		/**< 친구목록 스킨 6개 <br> */
#define WIID_CASH_GIFT_FRIEND_LIST_TEXT			(WIID_CASH_GIFT	+	180 )		/**< 친구목록 텍스트 6개 <br> */

#define WIID_CASH_GIFT_COSTSKIN					(WIID_CASH_GIFT	+	200 )
#define WIID_CASH_GIFT_COST						(WIID_CASH_GIFT	+	201 )
#define WIID_CASH_GIFT_COSTICON					(WIID_CASH_GIFT	+	202 )

#define WIID_CASH_GIFT_CUR_CASH_TEXTIMG 		(WIID_CASH_GIFT	+	210 )		/**< 보유한 오즈 텍스트 이미지 <br> */
#define WIID_CASH_GIFT_CUR_CASH					(WIID_CASH_GIFT	+	211 )
#define WIID_CASH_GIFT_CUR_CASH_ICON			(WIID_CASH_GIFT	+	212 )

#define WIID_CASH_GIFT_USE_CASH_TEXTIMG			(WIID_CASH_GIFT	+	220 )		/**< 사용할 오즈 텍스트 이미지 <br> */
#define WIID_CASH_GIFT_USE_CASH					(WIID_CASH_GIFT	+	221 )
#define WIID_CASH_GIFT_USE_CASH_ICON			(WIID_CASH_GIFT	+	222 )

#define WIID_CASH_GIFT_USE_POINT_TEXTIMG		(WIID_CASH_GIFT	+	230 )		/**< 사용할 포인트 텍스트 이미지 <br> */
#define WIID_CASH_GIFT_USE_POINT				(WIID_CASH_GIFT	+	231 )
#define WIID_CASH_GIFT_USE_POINT_ICON			(WIID_CASH_GIFT	+	232 )
#define WIID_CASH_GIFT_USE_POINT_INC			(WIID_CASH_GIFT	+	233 )		/**< 포인트 증가 <br> */
#define WIID_CASH_GIFT_USE_POINT_DEC			(WIID_CASH_GIFT	+	234 )		/**< 포인트 감소 <br> */

#define WIID_CASH_GIFT_PAY_TEXTIMG				(WIID_CASH_GIFT	+	240 )		/**< 결제할 금액 텍스트 이미지 <br> */
#define WIID_CASH_GIFT_PAY						(WIID_CASH_GIFT	+	241 )
#define WIID_CASH_GIFT_PAY_ICON					(WIID_CASH_GIFT	+	242 )

#define WIID_CASH_GIFT_CHANGE_TEXTIMG			(WIID_CASH_GIFT	+	250 )		/**< 결제후 금액 텍스트 이미지 <br> */
#define WIID_CASH_GIFT_CHANGE					(WIID_CASH_GIFT	+	251 )
#define WIID_CASH_GIFT_CHANGE_ICON				(WIID_CASH_GIFT	+	252 )


const int MAX_FRIEND_VIEW			= 6;


class SPWindow;
class SPWindowSlider;
class SPWindowEdit;
class SPWindowEditMultiLine;

struct SPCashItemAttr;

class SPWindowCashGift : public SPWindow {

public:	
	SPWindowCashGift(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowCashGift();

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
	void UpdateFriendList();

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnOk							(WPARAM wParam, LPARAM lParam);
	int OnCancel						(WPARAM wParam, LPARAM lParam);
	int OnPointInc						(WPARAM wParam, LPARAM lParam);
	int OnPointDec						(WPARAM wParam, LPARAM lParam);
	
	int	OnEditSetFocus					(WPARAM wParam, LPARAM lParam);
	int	OnEditKillFocus					(WPARAM wParam, LPARAM lParam);
	
	int OnToggleFriendList				(WPARAM wParam, LPARAM lParam);
	int OnMouseFriendList				(unsigned int uiID, WPARAM wParam, LPARAM lParam );
	int OnSelectFriendList				(unsigned int uiID, WPARAM wParam, LPARAM lParam );	
	
	int	OnButtonPageUp					(WPARAM wParam, LPARAM lParam);
	int	OnButtonPageDown				(WPARAM wParam, LPARAM lParam);
	int	OnSliderPageUp					(WPARAM wParam, LPARAM lParam);
	int	OnSliderPageDown				(WPARAM wParam, LPARAM lParam);
	int	OnSliderPageNum					(WPARAM wParam, LPARAM lParam);
	int OnSliderButtonUp				(WPARAM wParam, LPARAM lParam);
	// }}

private:
	int						m_iPointTradeUnit;

	SPWindow*				m_pItemName;						/**< 아이템 이름 <br> */
	SPWindow*				m_pCost;							/**< 아이템 가격 <br> */
	SPWindow*				m_pCostIcon;						/**< 아이템 가격 종류 캐시인지 포인트인지 <br> */
	SPWindow*				m_pItemIcon;						/**< 아이템 아이콘 <br> */

	SPWindow*				m_pFriendList;
	SPWindowSlider*			m_pSlider;							/**< Scroll Bar <br> */
	int						m_iCurPage;
	int						m_iMaxPage;

	SPWindowEdit*			m_pReceiver;						/**< 받는사람 <br> */
	SPWindowEditMultiLine*	m_pMsg;								/**< 내용 <br> */

	SPWindow*				m_pFriendSkin[MAX_FRIEND_VIEW];		/**< <br> */
	SPWindow*				m_pFriendName[MAX_FRIEND_VIEW];		/**< <br> */	

	SPWindow*				m_pCurCash;							/**< 보유한 오즈 <br> */
	SPWindow*				m_pUseCash;							/**< 사용할 오즈 <br> */	
	int						m_iUseCash;							/**< <br> */

	SPWindow*				m_pUsePoint;						/**< 사용할 포인트 <br> */
	int						m_iUsePoint;						/**< 사용할 포인트 <br> */

	SPWindow*				m_pPayCash;							/**< 결제할 금액 <br> */
	SPWindow*				m_pPayIcon;							/**< 결제할 금액 아이콘 <br> */

	SPWindow*				m_pChangeCash;						/**< 결제후 잔액 <br> */
	int						m_iChangeCash;						/**< <br> */
	SPWindow*				m_pChangeIcon;						/**< 결제후 잔액 아이콘 <br> */

	SPWindow*				m_pHelp;
};


