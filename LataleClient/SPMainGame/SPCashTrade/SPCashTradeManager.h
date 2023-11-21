#pragma once


#define		__FIELD_CASH_TRADE__		279


/**
 * 캐시 구매 장소 <br> 
*/
enum CASH_TRADE_PLACE {
	CASH_TRADE_PLACE_NULL,				/**< <br> */
	CASH_TRADE_PLACE_SHOP,				/**< 캐시샵 <br> */
	CASH_TRADE_PLACE_FIELD,				/**< 필드 <br> */
};


/**
 * 캐시 구매 요청 응답 대기 <br> 
*/
enum CASH_TRADE_STATE {
	CASH_TRADE_STATE_NULL,				/**< 캐시 구매중 아님 <br> */
	CASH_TRADE_STATE_WAIT,				/**< 캐시 구매 응답 대기중 <br> */
};


/**
 * 캐시 구매 타입 <br> 
*/
enum CASH_TRADE_TYPE {
	CASH_TRADE_TYPE_NULL,				/**< <br> */
	CASH_TRADE_TYPE_BUY,				/**< 단순 구매 <br> */
	CASH_TRADE_TYPE_GIFT,				/**< 선물 구매 <br> */
};


class SPHotItemInfo;
class SPHotItemManager;
struct SPCashItemAttr;


/**
 * 클라이언트의 캐시샵 버전 정보 <br> 
*/
struct SPCashShopVerInfo {
	int		m_iShopMajor;			/**< 메지저 버전 <br> */
	int		m_iShopMinor;			/**< 마이너 버전 <br> */
	int		m_iHotListVer;			/**< 추천상품 버전 <br> */

	SPCashShopVerInfo() {
		Clear();
	}

	~SPCashShopVerInfo() {
		Clear();
	}

	void Clear() {
		m_iShopMajor		= -1;
		m_iShopMinor		= -1;
		m_iHotListVer		= -1;
	}

	void SetVerInfo(int iMajor, int iMinor, int iHotVer) {
		m_iShopMajor		= iMajor;
		m_iShopMinor		= iMinor;
		m_iHotListVer		= iHotVer;
	}

	void SetMajorVer(int iMajor) {
		m_iShopMajor		= iMajor;
	}

	void SetMinorVer(int iMinor) {
		m_iShopMinor		= iMinor;
	}

	void SetHotVer(int iHotVer) {
		m_iHotListVer		= iHotVer;
	}
};


/**
 * 캐시 거래 결과의 정보를 기억 <br> 
*/
struct SPCashTradeResultInfo {
	int				m_iCashShopID;				/**< 거래 품목 <br> */
	std::string		m_strReceiver;				/**< 선물인 경우 받은 이 <br> 없으면 자기자신 구매 <br> */

	SPCashTradeResultInfo() {
		Clear();
	}

	~SPCashTradeResultInfo() {
		Clear();
	}

	void Clear() {
		m_iCashShopID	= 0;
		m_strReceiver.clear();
	}
};


class SPCashTradeManager{
public:
	static SPCashTradeManager* GetInstance();
	static void Release();

	void Init();

	void Reset();

	bool LoadCashShopVersionLDT();
	
	CASH_TRADE_PLACE GetCashTradePlace();
	CASH_TRADE_STATE GetCashTradeState();
	CASH_TRADE_TYPE	 GetCashTradeType();

	void SetVerInfo(int iMajor, int iMinor, int iHotVer);
	void SetMajorVer(int iMajor);
	void SetMinorVer(int iMinor);
	void SetHotVer(int iHotVer);

	SPCashShopVerInfo GetCashVerInfo()			{	return m_CashShopVerInfo; 	}
	SPCashTradeResultInfo* GetCashTradeResult()	{	return &m_CashTradeResult;	}

	/**
	 * 로딩중 소켓 상태를 확인하기 위한 용도 <br> 
	 @param 
	 @return  
	*/
	void ResetSocket()							{	m_bSocketFail = false;	}
	
	/**
	 * 로딩중 소켓 패일인지 확인한다 <br> 
	 @param 
	 @return  
	*/
	bool IsSocketFail()							{	return m_bSocketFail;	}
	
	bool IsMinorLoad()							{	return m_bMinorLoad;	}
	bool IsHotListLoad()						{	return m_bHotListLoad;	}

	/**
	 * 로딩시 네트워크 이상시 기능을 초기화 한다 <br>
	 * 대기 상태를 풀도록 한다 <br>
	 @param 
	 @return  
	*/
	void SetSocketFail();
	
	/**
	 * 추천상품 매니저를 확인한다 <br>
	 @param 
	 @return  
	*/
	SPHotItemManager* GetHotItemManager()		{	return m_pHotItemManager;}

	/**
	 * 해당 ID 로 캐시샵 아이템 구매 의사를 묻는다 <br>
	 * 아이템 기억후 인터페이스 오픈 <br>
	 @param int iShopID
	 @return  
	*/
	bool SetSelectCashItem(int iShopID, bool bShowSelectWnd = true);
	
	/**
	 * 현재 구매를 위해 선택한 캐시아이템 정보를 확인한다 <br>
	 @param 
	 @return  
	*/
	SPCashItemAttr* GetSelectCashItem();
	
	/**
	 * 현재 구매를 위해 선택한 캐시아이템 정보를 초기화한다 <br>
	 @param 
	 @return  
	*/
	bool ClearSelectCashItem();	

	/**
	 * 구매, 선물 의 응답 패킷을 받음 <br> 
	 @param 
	 @return  
	*/
	void SetBuyPacketReceive();
	
	/**
	 * 대기 에니메이션이 완료됨을 받음 <br>
	 @param 
	 @return  
	*/
	void SetWaitAniComplete();
	

	/**
	 * 캐시 충전 페이지로 이동 <br> 
	 @param 
	 @return  
	*/
	bool PleaseBuyMoney();

	/**
	 * 현 클라이언트의 캐시샵 버전을 알림 <br> 
	 @param 
	 @return  
	*/
	bool SendCashShopVersion();
	
	/**
	 * 캐시샵 클라이언트 버전 확인 응답 <br> 
	 @param 
	 @return  
	*/
	bool OnCashShopVersion(CPacket* pPacket);
	
	/**
	 * 필드 구매시 해당 캐시 아이템의 가격 갱신을 요청한다 <br> 
	 @param CASH_TRADE_TYPE iTradeType
	 @return  bool
	*/
	bool SendCashShopPrice(CASH_TRADE_TYPE iTradeType);
	
	/**
	 * 필드 구매시 캐시 아이템 가격 갱신 응답 <br> 
	 @param 
	 @return  
	*/
	bool OnCashShopPrice(CPacket* pPacket);
	
	/**
	 * 추천 상품 리스트를 업데이트 한다 <br>
	 @param 
	 @return  
	*/
	bool OnCashShopHotItemUpdate(CPacket* pPacket);

	/**
	* 추천 상품 리스트 Show 상태를 On/Off 한다 <br> 
	@param 
	@return  
	*/
	bool OnSetHotItemStatus(CPacket* pPacket);
	
	/**
	 * 캐시샵 아이템 업데이트 <br> 
	 @param 
	 @return  
	*/
	bool OnCashShopItemUpdate(CPacket* pPacket);
	

	/**
	 * 구매 요청 <br>
	 @param CASH_TRADE_PLACE iPlace
	 @param int iPackageID
	 @param int iShopPrice
	 @param int iSalePrice
	 @param int iMaxOz
	 @param int iMaxMileage
	 @param int iBuyOz = 0
	 @param int iBuyMileage = 0
	 @return  
	*/
	bool SendCashShopBuy(CASH_TRADE_PLACE iPlace, int iPackageID, 
							int iShopPrice,	int iSalePrice, int iMaxOz, int iMaxMileage, 
							int iBuyOz = 0, int iBuyMileage = 0);
	
	/**
	 * 구매 요청 응답 <br> 
	 @param 
	 @return  
	*/
	bool OnCashShopBuy(CPacket* pPacket);
	
	/**
	 * 선물 구매 요청 <br> 
	 * 캐시샵에서도 사용 <br> 
	 @param CASH_TRADE_PLACE iPlace
	 @param int iPackageID
	 @param int iShopPrice
	 @param int iSalePrice
	 @param int iMaxOz
	 @param	int iMaxMileage
	 @param int iBuyOz = 0
	 @param int iBuyMileage = 0
	 @return bool  
	*/
	bool SendCashShopPresent(CASH_TRADE_PLACE iPlace, int iPackageID,
								int iShopPrice,	int iSalePrice, int iMaxOz, int iMaxMileage,
								std::string strName, std::string strMsg,
								int iBuyOz = 0, int iBuyMileage = 0);
	
	/**
	 * 선물 구매 요청 응답 <br> 
	 @param 
	 @return  
	*/
	bool OnCashShopGift(CPacket* pPacket);

	/**
	 * 찜 목록 추가 요청 <br> 
	 * 캐시샵에서도 사용 <br> 
	 @param CASH_TRADE_PLACE iPlace
	 @param int iPackageID
	 @return  bool
	*/
	bool SendWishAdd(CASH_TRADE_PLACE iPlace, int iPackageID);	
	
	/**
	 * 찜 목록 추가 요청 응답 <br> 
	 @param 
	 @return  
	*/
	bool OnCashWishAdd(CPacket* pPacket);

private:
	SPCashTradeManager();
	~SPCashTradeManager();
	

protected:
	/**
	 * 인벤토리 공간 체크 <br> 
	 @param 
	 @return  
	*/
	bool IsEnableInventory();

	/**
	* 인벤토리 공간이 모자른 경우 에러 메시지 <br> 
	@param 
	@return  
	*/
	void SetInvenFullMsg(int i);

	/**
	 *
	 @param 
	 @return  
	*/
	bool IsSendCheck();

	/**
	 * 구매, 선물 패킷후 결과 상태로 처리 <br> 
	 @param 
	 @return  
	*/
	void ShowResultAction();

	/**
	 * 구매, 선물 패킷이 실패한 경우 처리 <br> 
	 @param 
	 @return  
	*/
	void ShowResultFail();

private:
	CASH_TRADE_PLACE		m_iCashTradePlace;
	CASH_TRADE_STATE		m_iCashTradeState;
	CASH_TRADE_TYPE			m_iCashTradeType;

	static SPCashTradeManager*	m_pInstance;

	SPCashShopVerInfo		m_CashShopVerInfo;

	bool					m_bSocketFail;				/**< 로딩시 소켓 패일인 경우 true <br> */
	bool					m_bMinorLoad;
	bool					m_bHotListLoad;

	SPHotItemManager*		m_pHotItemManager;

	SPCashItemAttr*			m_pSelectCashItem;			/**< 구매 하려는 상품의 정보를 기억 <br> */	

	SPCashTradeResultInfo	m_CashTradeResult;			/**< 결과 정보를 기억 <br> */

	bool					m_bBuyPacketReceive;		/**< 구매 관련 패킷의 응답 을 받았는지 <br> */
	bool					m_bWaitAniComplete;			/**< 구매 대기 에니메이션을 완료 했는지 <br> */
};

extern SPCashTradeManager* g_pCashTradeManager;




