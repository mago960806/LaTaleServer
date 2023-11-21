#pragma once

//#define WIID_PC_TRADE_TOP				(WIID_PC_TRADE	+	1)		//스킨 TOP
#define WIID_PC_TRADE_CENTER			(WIID_PC_TRADE	+	2)		//스킨 CENTER
//#define WIID_PC_TRADE_SUB_TOP			(WIID_PC_TRADE	+	3)		//스킨 SUB_TOP
//#define WIID_PC_TRADE_SUB_HLINE1		(WIID_PC_TRADE	+	4)		//스킨 SUB_HLINE1
//#define WIID_PC_TRADE_SUB_BOTTOM		(WIID_PC_TRADE	+	5)		//스킨 SUB_BOTTOM
//#define WIID_PC_TRADE_SUB_HLINE2		(WIID_PC_TRADE	+	6)		//스킨 SUB_HLINE2
//#define WIID_PC_TRADE_BOTTOM			(WIID_PC_TRADE	+	7)		//스킨 BOTTOM

#define WIID_PC_TRADE_TITLE				(WIID_PC_TRADE	+	10)		//제목
#define WIID_PC_TRADE_LINE1				(WIID_PC_TRADE	+	11)		//제목하단 경계1
#define WIID_PC_TRADE_HIDE				(WIID_PC_TRADE	+	20)		//종료버튼
#define WIID_PC_TRADE_ACCEPT			(WIID_PC_TRADE	+	30)		//수락버튼
#define WIID_PC_TRADE_CANCEL			(WIID_PC_TRADE	+	40)		//취소버튼
#define WIID_PC_TRADE_HELP				(WIID_PC_TRADE	+	50)		//도움말

//#define WIID_PC_TRADE_STATUS_WAIT		(WIID_PC_TRADE	+	100)	//내 거래준비상태_준비중
#define WIID_PC_TRADE_STATUS_ACCEPT		(WIID_PC_TRADE	+	110)	//내 거래준비상태_준비완료
#define WIID_PC_TRADE_NAME_WAIT			(WIID_PC_TRADE	+	120)	//내 이름 준비중 
#define WIID_PC_TRADE_NAME_ACCEPT		(WIID_PC_TRADE	+	121)	//내 이름 준비완료
#define WIID_PC_TRADE_VLINE1			(WIID_PC_TRADE	+	130)	//수직선1
//#define WIID_PC_TRADE_PC_STATUS_WAIT	(WIID_PC_TRADE	+	140)	//상대 거래준비상태_준비중
#define WIID_PC_TRADE_PC_STATUS_ACCEPT	(WIID_PC_TRADE	+	150)	//상대 거래준비상태_준비완료
#define WIID_PC_TRADE_PC_NAME_WAIT		(WIID_PC_TRADE	+	160)	//상대 이름 준비
#define WIID_PC_TRADE_PC_NAME_ACCEPT	(WIID_PC_TRADE	+	161)	//상대 이름 준비 완료

//#define WIID_PC_TRADE_PC_TEXT			(WIID_PC_TRADE	+	200)	//상대 거래텍스트
#define WIID_PC_TRADE_PC_ITEM_BTN		(WIID_PC_TRADE	+	210)	//상대 아이템의 버튼(툴팁용)
#define WIID_PC_TRADE_PC_ITEM_ICONLINE	(WIID_PC_TRADE	+	220)	//상대 아이템의 ICON배경라인(배열. 10개한계)
#define WIID_PC_TRADE_PC_ITEM_ICON		(WIID_PC_TRADE	+	230)	//상대 아이템의 ICON(배열. 10개한계)
#define WIID_PC_TRADE_PC_ITEM_TEXT		(WIID_PC_TRADE	+	240)	//상대 아이템의 Text(배열. 10개한계)

#define WIID_PC_TRADE_PC_ELY_BACK		(WIID_PC_TRADE	+	250)	//상대 거래 Ely 배경
#define WIID_PC_TRADE_PC_ELY			(WIID_PC_TRADE	+	255)	//상대 거래 Ely
#define WIID_PC_TRADE_PC_ELYIMG			(WIID_PC_TRADE	+	260)	//상대 거래 Ely이미지

#define WIID_PC_TRADE_PC_CASH_BACK		(WIID_PC_TRADE	+	270)	//상대 거래 Cash 배경 
#define WIID_PC_TRADE_PC_CASH			(WIID_PC_TRADE	+	271)	//상대 거래 Cash
#define WIID_PC_TRADE_PC_CASHIMG		(WIID_PC_TRADE	+	272)	//상대 거래 Cash이미지
#define WIID_PC_TRADE_PC_CASHDIS		(WIID_PC_TRADE	+	273)	//상대 거래 Cash Disable 표현

#define WIID_PC_TRADE_PC_POINT_BACK		(WIID_PC_TRADE	+	280)	//상대 거래 Point 배경
#define WIID_PC_TRADE_PC_POINT			(WIID_PC_TRADE	+	281)	//상대 거래 Point
#define WIID_PC_TRADE_PC_POINTIMG		(WIID_PC_TRADE	+	282)	//상대 거래 Point이미지
#define WIID_PC_TRADE_PC_POINTDIS		(WIID_PC_TRADE	+	283)	//상대 거래 Point Disable 표현

#define WIID_PC_TRADE_PC_FEE_TEXT		(WIID_PC_TRADE	+	290)	//상대 거래 수수료 문자이미지
#define WIID_PC_TRADE_PC_FEE_BACK		(WIID_PC_TRADE	+	291)	//상대 거래 Fee 배경
#define WIID_PC_TRADE_PC_FEE			(WIID_PC_TRADE	+	292)	//상대 거래 Fee
#define WIID_PC_TRADE_PC_FEEIMG			(WIID_PC_TRADE	+	293)	//상대 거래 Fee이미지

//#define WIID_PC_TRADE_VLINE2			(WIID_PC_TRADE	+	300)	//수직선2
//#define WIID_PC_TRADE_TEXT				(WIID_PC_TRADE	+	400)	//내 거래텍스트
#define WIID_PC_TRADE_ITEM_BTN			(WIID_PC_TRADE	+	410)	//내 아이템의 버튼(툴팁용)
#define WIID_PC_TRADE_ITEM_ICONLINE		(WIID_PC_TRADE	+	420)	//내 아이템의 ICON배경라인(배열. 10개한계)
#define WIID_PC_TRADE_ITEM_ICON			(WIID_PC_TRADE	+	430)	//내 아이템의 ICON(배열. 10개한계)
#define WIID_PC_TRADE_ITEM_TEXT			(WIID_PC_TRADE	+	440)	//내 아이템의 Text(배열. 10개한계)

#define WIID_PC_TRADE_ELY_BASE			(WIID_PC_TRADE	+	450)	//내 거래 Ely Base Skin
#define WIID_PC_TRADE_ELY				(WIID_PC_TRADE	+	451)	//내 거래 Ely	//Edit Control
#define WIID_PC_TRADE_ELYIMG			(WIID_PC_TRADE	+	460)	//내 거래 Ely이미지

#define WIID_PC_TRADE_CASH_BACK			(WIID_PC_TRADE	+	470)	//내 거래 Cash 입력 바탕
#define WIID_PC_TRADE_CASH				(WIID_PC_TRADE	+	471)	//내 거래 Cash Edit
#define WIID_PC_TRADE_CASH_UNIT			(WIID_PC_TRADE	+	472)	//내 거래 Cash 단위 표현
#define WIID_PC_TRADE_CASHIMG			(WIID_PC_TRADE	+	473)	//내 거래 Cash이미지
#define WIID_PC_TRADE_CASHDIS			(WIID_PC_TRADE	+	474)	//내 거래 Cash비활성

#define WIID_PC_TRADE_POINT_BACK		(WIID_PC_TRADE	+	480)	//내 거래 Point 입력 바탕
#define WIID_PC_TRADE_POINT				(WIID_PC_TRADE	+	481)	//내 거래 Point
#define WIID_PC_TRADE_POINT_UNIT		(WIID_PC_TRADE	+	482)	//내 거래 Point 단위 표현
#define WIID_PC_TRADE_POINTIMG			(WIID_PC_TRADE	+	483)	//내 거래 Point이미지
#define WIID_PC_TRADE_POINTDIS			(WIID_PC_TRADE	+	484)	//내 거래 Point비활성

#define WIID_PC_TRADE_FEE_TEXT			(WIID_PC_TRADE	+	490)	//내 거래 수수료 문자 이미지
#define WIID_PC_TRADE_FEE_BACK			(WIID_PC_TRADE	+	491)	//내 거래 Fee 바탕
#define WIID_PC_TRADE_FEE				(WIID_PC_TRADE	+	492)	//내 거래 Fee
#define WIID_PC_TRADE_FEEIMG			(WIID_PC_TRADE	+	493)	//내 거래 Fee이미지


/*
#define WIID_PC_TRADE_ITEM_ICON			(WIID_PC_TRADE	+	40)		//내 아이템의 ICON
#define WIID_PC_TRADE_ITEM_TEXT			(WIID_PC_TRADE	+	50)		//내 아이템의 Text
#define WIID_PC_TRADE_ITEM_BTN			(WIID_PC_TRADE	+	60)		//내 아이템의 버튼

#define WIID_PC_TRADE_PC_NAME			(WIID_PC_TRADE	+	65)		//상대 이름
#define WIID_PC_TRADE_PC_ITEM_ICON		(WIID_PC_TRADE	+	70)		//상대 아이템의 ICON
#define WIID_PC_TRADE_PC_ITEM_TEXT		(WIID_PC_TRADE	+	80)		//상대 아이템의 Text
#define WIID_PC_TRADE_PC_ITEM_BTN		(WIID_PC_TRADE	+	90)		//상대 아이템의 버튼

#define WIID_PC_TRADE_HIDE				(WIID_PC_TRADE	+	100)	//종료
#define WIID_PC_TRADE_ACCEPT			(WIID_PC_TRADE	+	110)	//Local User 수락
#define WIID_PC_TRADE_PC_ACCEPT			(WIID_PC_TRADE	+	120)	//상대 수락

#define WIID_PC_TRADE_ELY				(WIID_PC_TRADE	+	130)	//내 거래 Ely	//Edit Control
#define WIID_PC_TRADE_PC_ELY			(WIID_PC_TRADE	+	140)	//상대 거래 Ely	//Static Control
*/

const int MAX_PC_TRADE_COUNT = 4;	// 최대 트레이드 아이템 수
//const float ENABLE_X = 400.0f;
//const float ENABLE_Y = 300.0f;

//
//enum PC_TRADE_ACCEPT {
//	PC_TRADE_ACCEPT_DISABLE = 0,
//	PC_TRADE_ACCEPT_ENABLE = 1,
//	PC_TRADE_ACCEPT_ACCEPT = 2,
//};

enum ICON_MARK;
class SPItem;
class SPWindow;
class SPWindowButton;
class SPWindowEdit;


enum TRADE_MODE {
	TRADE_MODE_NULL = 0,
	TRADE_MODE_WAIT = 1,
};


enum TRADE_USER {
	TRADE_USER_LOCAL	= 0,
	TRADE_USER_PC		= 1,
};


/**
 * 캐시 트레이드시 수수료형식
*/
enum TRADE_FEE {
	TRADE_FEE_NULL,
	TRADE_FEE_CASH,
	TRADE_FEE_POINT,
};

/**
* @class SPWindowPcTrade
* @brief 유저 트레이드 윈도우
* @author Jinhee
* @version 00.00.01
* @date 2005.7.18
* @bug 없음
* @warning 
*/
class SPWindowPcTrade : public SPWindow {

public:
	SPWindowPcTrade( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowPcTrade();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);

	void Refresh();

protected:
	/**
	 * 거래 상대 PC와 나와의 간격을 체크한다 <br> 
	 @param NONE
	 @return bool
	*/
	//bool CheckInterval();

	/**
	* CashTrade 인지 체크한다 <br>
	@param 
	@return  
	*/
	bool CheckCashTrade();

	/**
	* 두 사용자의 거래 목록이 모두 비어있는지 확인한다 <br>
	@param NONE
	@return bool
	*/
	bool IsEmptyItem();

	/**
	 * 트레이드 모드별로 윈도우 사이즈를 재 설정한다 <br>
	 @param 
	 @return  
	*/
	void SetWindowBase();

	/**
	 * 하위 윈도우 컨트롤을 생성한다 <br>
	 @param NONE
	 @return NONE
	*/
	void CreateSubWindow();

	/**
	 * 캐시 거래 수수료 단위를 가지고 거래 단위 스트링을 만들어 낸다 <br>
	 @param 
	 @return  
	*/
	std::string GetUnitString(int iUnit = 0, TRADE_FEE iMode = TRADE_FEE_NULL);

	/**
	 * 트레이드 모드별로 윈도우 내부 컨트롤을 배치한다 <br>
	 @param 
	 @return  
	*/
	void ArrangeControl();

	/**
	 * 거래 관련 창 닫힘 작동 <br> 데이터 초기화 <br>
	 @param NONE
	 @return NONE
	*/
	void CloseTrade();
	
	/**
	 * 거래 상대의 아이템공간(보여주기 위한)을 만듬 <br>
	 @param NONE
	 @return NONE
	*/
	void InitPcItem();
	
	/**
	 * 거래 상대의 아이템 공간을 모두 삭제 <br>
	 @param NONE
	 @return NONE
	*/
	void DeleteAllPcItem();
	
	/**
	 * 거래 상대의 아이템 데이터를 모두 초기화 한다 <br> 
	 @param NONE
	 @return NONE
	*/
	void ClearAllPcItem();

	/**
	 * 해당 인덱스의 상대방 아이템을 초기화 한다 <br> 
	 @param int iIndex
	 @return bool
	*/
	bool ClearPcItem(int iIndex);

	/**
	 * 상대방의 아이템을 설정하고 인터페이스를 설정한다 <br>
	 @param int iIndex
	 @param ITEM* Item
	 @return bool
	*/
	bool SetPcItem(int iIndex, ITEM* Item);	
	
	/**
	 * 내 트레이드 아이템을 설정 <br>
	 * 아이콘, 텍스트 등의 보여주기위한 셋팅 <br> 
	 * 지울때는 메시지(SPIM_TRADE_ITEM_CLEAR) 나 함수(ItemClear)를 이용한다 <br> 
	 @param int iIndex
	 @return NONE
	*/
	void SetItemIcon(int iIndex);

	/**
	 * 내 Cash, Point에 대한 컨트롤 활성을 조정한다 <br>
	 @param 
	 @return  
	*/
	void UpdateCashControl();

	/**
	 * 인던에 의해 캐시 입력을 제한하도록 해줌 <br> 
	 @param 
	 @return  
	*/
	void SetEnableIndun();

	/**
	 * 상대방의 Cash, Point 에 대한 수수료를 설정한다 <br>
	 @param 
	 @return  
	*/
	void UpdatePcFee();

	/**
	 * 내 Cash, Point 에 대한 수수료를 설정한다 <br>
	 @param 
	 @return  
	*/
	void UpdateLocalFee();

	/**
	 * 내 Cash, Point 입력 컨트롤 활성을 조정 한다 <br> 
	 @param 
	 @return  
	*/
	void UpdateEditControl();
	
	/**
	 * 수수료 인터페이스를 보여줄지 설정한다 <br>
	 @param 
	 @return  
	*/
	void SetFeeShow(TRADE_USER iUser, bool bShow);

public:
	/**
	* 로컬 유저가 해당 인텍스의 아이템을 올렸놨다고 알린다 <br>
	@param int iIndex
	@return bool
	*/
	bool SendPushItem(int iIndex);
	
	/**
	* 로컬 유저가 해당 인텍스의 아이템을 제거 했다고 알린다 <br>
	@param int iIndex
	@return bool
	*/
	bool SendPopItem(int iIndex);

	/**
	* 로컬 유저가 돈을 올려놨다고 알린다  <br>
	@param INT64 uiMoney
	@param int iCash = 0
	@param int iPoint = 0
	@return bool
	*/
	bool SendPushMoney(INT64 uiMoney, int iCash = 0, int iPoint = 0);

	/**
	 * <br>
	 @param
	 @return
	*/
	bool SendTradeWait();
	
	/**
	* 로컬 유저가 거래를 취소한다고 알린다  <br>
	@param NONE
	@return bool
	*/
	bool SendTradeCancel();
	
	/**
	* 로컬 유저가 거래를 승락한다고 알린다  <br>
	@param NONE
	@return bool
	*/
	bool SendTradeAccept();

	/**
	* 두 사용자가 거래가 가능한 상태인지를 체크한다 <br>
	@param NONE
	@return bool
	*/
	bool IsTradeEnable();

	/**
	* 상대 유저의 거래 아이템을 확인한다 <br>
	@param int iIndex 
	@return SPItem*
	*/
	SPItem* GetPcItem(int iIndex);

	/**
	 * 상대 유저의 캐릭명을 가져온다 <br>
	 @param
	 @return char*
	*/
	const char*	GetPcName();

	/**
	 * 상대 유저의 거래 금액을 가져온다 <br>
	 @param
	 @return INT64
	*/
	INT64	GetPcEly();

	/**
	 * 내 거래 금액을 가져온다 <br>
	 @param
	 @return INT64
	*/
	INT64	GetEly();

	int		GetCash();
	int		GetPcCash();
	int		GetPoint();
	int		GetPcPoint();
	int		GetFee();
	int		GetPcFee();
    

protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnCursorEnter	( WPARAM, LPARAM );
	virtual int OnCursorOut		( WPARAM, LPARAM );
	virtual int OnCursorLDown	( WPARAM, LPARAM );
	virtual int OnCursorLUp		( WPARAM, LPARAM );	
	virtual int OnRefresh		( WPARAM, LPARAM );		

	int ItemClear				(WPARAM wParam, LPARAM lParam);			/**< 로컬 플레이어의 아이템 제거 <br> */

	virtual int OnHide			(WPARAM wParam, LPARAM lParam);
	virtual int OnSetPlayerID	(WPARAM wParam, LPARAM lParam);			/**< 거래 상대의 GUID 설정 <br> */

	int OnAccept				(WPARAM wParam, LPARAM lParam);			/**< 거래 승락 <br> */

	int OnUpdateEly				(WPARAM wParam, LPARAM lParam);			/**< Ely Update <br> */
	int OnUpdateCash			(WPARAM wParam, LPARAM lParam);			/**< <br> */	
	int OnUpdatePoint			(WPARAM wParam, LPARAM lParam);			/**< <br> */
	int OnClickCash				(WPARAM wParam, LPARAM lParam);			/**< <br> */
	int OnClickPoint			(WPARAM wParam, LPARAM lParam);			/**< <br> */

	int OnToolTipMove			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipEnter			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipOut			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);

	int OnItemClickUp			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnItemClickDown			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);

	int OnPCToolTipMove			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnPCToolTipEnter		(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnPCToolTipOut			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);

	int OnPCItemClickUp			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	
	int OnPushItem				(WPARAM wParam, LPARAM lParam);			/**< 아이템 올림의 응답 <br> */
	int OnPopItem				(WPARAM wParam, LPARAM lParam);			/**< 아이템 제거의 응답 <br> */
	int OnPushMoney				(WPARAM wParam, LPARAM lParam);			/**< 돈 올림의 응답 <br> */
	int OnTradeCancel			(WPARAM wParam, LPARAM lParam);			/**< 거래 취소의 응답 <br> */
	int OnTradeWait				(WPARAM wParam, LPARAM lParam);			/**< 거래 대기 <br> */
	int OnTradeAccept			(WPARAM wParam, LPARAM lParam);			/**< 거래 승락의 응답 <br> */

	int OnCloseTrade			(WPARAM wParam, LPARAM lParam);			/**< 외부(포탈)에서의 CloseTrade 호출용 <br> */

	// }}

private:
	GU_ID m_uiPlayerGUID;							/**< 거래 대상의 GUID <br> */
	char m_szBuf[128];
	SPPlayer* m_pTargetPlayer;						/**< 거래 대상의 Player정보 <br> */

	int	m_iSrcPcAcceptX[3];
	int m_iSrcPcAcceptY;

	SPTexture* m_pBaseSkin;
	RECT m_rtBaseSrc[WINDOW_BASE_MAX];
	RECT m_rtBaseDest[WINDOW_BASE_MAX];
	
	SPWindowStatic* m_pHelp;						/**< 도움말 <br> */
	SPWindowButton* m_pAccept;						/**< 내 거래 승락 버튼 <br> */
	//SPWindowStatic* m_pPcStatus_Wait;				// 상대 수락상태(대기중)
	SPWindowStatic* m_pPcStatus_Accept;				// 상대 수락상태(수락)
	//SPWindowStatic* m_pStatus_Wait;					// 내 수락상태(대기중)
	SPWindowStatic* m_pStatus_Accept;				// 내 수락상태(수락)

	float m_fStartPosX;								/**< 거래를 시작할때 내 위치 X <br> */
	float m_fStartPosY;								/**< 거래를 시작할때 내 위치 Y <br> */	
	bool m_bSendCancel;								/**< 내가 캔슬팻킷을 한번 보냈는지? <br> */

	SPWindow* m_pItemIcon[MAX_PC_TRADE_COUNT];		/**< 내 거래 아이템 아이콘 <br> */
	SPWindow* m_pItemText[MAX_PC_TRADE_COUNT];		/**< 내 거래 아이템 이름 <br> */
	SPWindow* m_pPCItemIcon[MAX_PC_TRADE_COUNT];	/**< 상대 거래 아이템 아이콘 <br> */
	SPWindow* m_pPCItemText[MAX_PC_TRADE_COUNT];	/**< 상대 거래 아이템 이름 <br> */

	SPWindowEdit*	m_pEditEly;						/**< 내 거래 Ely <br> */
	SPWindowEdit*	m_pEditCash;					/**< 내 거래 Cash <br> */
	SPWindowEdit*	m_pEditPoint;					/**< 내 거래 Point <br> */	
	SPWindowStatic* m_pEditCashDis;					/**< 내 거래 Cash Disable <br> */
	SPWindowStatic* m_pEditPointDis;				/**< 내 거래 Point Disable <br> */
	SPWindowStatic* m_pFeeText;						/**< 내 거래 수수료 문자열 이미지 <br> */
	SPWindowStatic* m_pFeeBack;						/**< 내 거래 Fee 바탕 <br> */
	SPWindowStatic* m_pFee;							/**< 내 거래 Fee <br> */
	SPWindowStatic* m_pFeeImg;						/**< 내 거래 Fee Img <br> */
	
	SPWindowStatic* m_pPcEly;						/**< 상대 거래 Ely <br> */
	SPWindowStatic* m_pPcCash;						/**< 상대 거래 Cash <br> */
	SPWindowStatic* m_pPcCashDis;					/**< 상대 거래 Cash Disable <br> */
	SPWindowStatic* m_pPcPoint;						/**< 상대 거래 Point <br> */
	SPWindowStatic* m_pPcPointDis;					/**< 상대 거래 Point Disable <br> */
	SPWindowStatic* m_pPcFeeText;					/**< 상대 거래 수수료 문자열 이미지 <br> */
	SPWindowStatic* m_pPcFeeBack;					/**< 상대 거래 Fee 바탕 <br> */
	SPWindowStatic* m_pPcFee;						/**< 상대 거래 Fee <br> */
	SPWindowStatic* m_pPcFeeImg;					/**< 상대 거래 Fee Img <br> */

	UINT64	m_iTradeEly;							/**< 내 거래 Ely <br> */
	int		m_iTradeCash;							/**< 내 거래 Cash <br> */	
	int 	m_iTradePoint;							/**< 내 거래 Point <br> */
	int 	m_iTradeFee;							/**< 내 거래 Fee <br> */

	int			m_iCashUnitCount;					/**< 캐시 거래 단위 자리수 표시 계산용 <br> */
	std::string m_strCashUnit;						/**< 캐시 거래 단위 출력 스트링 <br> */
	int			m_iPointUnitCount;					/**< 포인트 거래 단위 자리수 표시 계산용 <br> */
	std::string m_strPointUnit;						/**< 포인트 거래 단위 출력 스트링 <br> */

	SPWindowStatic* m_pNameWait;					/**< 내 이름 대기 <br> */
	SPWindowStatic* m_pNameAccept;					/**< 내 이름 수락 <br> */
	SPWindowStatic* m_pPcNameWait;					/**< 상대 이름 대기 <br> */
	SPWindowStatic* m_pPcNameAccept;				/**< 상대 이름 수락 <br> */

	std::vector<SPItem*> m_vpPCItem;				/**< 상대 거래 아이템 리스트 <br> */
	
	INT64	m_iPcEly;								/**< 상대방의 돈 <br> */
	int 	m_iPcCash;								/**< 상대방 Cash <br> */
	int 	m_iPcPoint;								/**< 상대방 Point <br> */
	int 	m_iPcFee;								/**< 상대방 Fee <br> */
	
	bool m_bItemAble[MAX_PC_TRADE_COUNT];			/**< 내가 올려논 아이템이 내가 착용가능한 아이템인지 <br> */
	bool m_bPcItemAble[MAX_PC_TRADE_COUNT];			/**< 상대방이 올려논 아이템이 내가 착용가능한 아이템인지 <br> */	

	SPTexture* m_pDisable;							/**< 비활성 표현용 <br> */
	RECT m_rtSrcDisable;							/**< 비활성 표현용 소스 <br> */

	ICON_MARK m_iItemMark[MAX_PC_TRADE_COUNT];		/**< ICON MARK <br> */
	ICON_MARK m_iPcItemMark[MAX_PC_TRADE_COUNT];	/**< 상대방 ICON MARK <br> */

	RECT m_rtMarkSrc[4];							/**< ICON MARK Src <br> */
	
	int m_iStack[MAX_PC_TRADE_COUNT];				/**< 로컬 플레이어용 스택 <br> */
	RECT m_rtStack[MAX_PC_TRADE_COUNT];				/**< 스택표현시 참고 영역 Icon 영역과 동일 <br> */
	
	int m_iPcStack[MAX_PC_TRADE_COUNT];				/**< 상대방 플레이어용 스택 <br> */
	RECT m_rtPcStack[MAX_PC_TRADE_COUNT];			/**< 스택표현시 참고 영역 Icon 영역과 동일 <br> */	

	bool	m_bChanged;								/**< 현재 내 트레이드 상태에 변화가 생겼는지 <br> */
	float	m_fChangedDeley;						/**< 트레이드 상태 변경시 일정시간 수락 버튼을 비활성 <br> */
	float	m_fChangedAccumulate;					/**< 수락버튼 비활성을 활성으로 풀어주는 타이머 계산 <br> */	

	TRADE_MODE m_iTradeMode;						/**< 트레이드 모드 <br> */
};


