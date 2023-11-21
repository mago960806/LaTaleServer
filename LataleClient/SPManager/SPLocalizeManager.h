#pragma once

enum COUNTRY_TYPE
{
	CT_NULL = -1,
	CT_KOREA = 0,		// SITE_ACTOZ
	CT_JAPAN,			// SITE_GAMEPOT
	CT_CHINA,			// SITE_SANDA
	CT_BUDDYBUDDY,		// SITE_BUDDYBUDDY	
	CT_OGP,				// [4/28/2008 metalgeni]
};

//enum SITE_TYPE		// GlobalDefines_share.h와 중복되어 "_"를 넣음. 작업완료시 서버에 알림필요함.
//{
//	SITE_LOCAL_ = 0,
//	SITE_ACTOZ_,
//	SITE_GAMEPOT_,
//	SITE_SANDA_,
//	SITE_BUDDYBUDDY_,
//	MAX_SITE_,
//};


enum LOCALIZE_VALUE
{
	LV_PROLOGUE,
	LV_CASHSHOP,
	LV_BEAUTYSHOP,
	LV_PET,
	LV_PHANTOM,
	LV_WAYPOINT,
	LV_COUPON,
	LV_GIFT,
	LV_GUILD,
	LV_LOTTERY,
	LV_INDUN,
	LV_JOYPAD,
	LV_CHATMACRO,
	LV_ITEMMIX,
	LV_GUIDEBOOK,
	LV_PVP,
	LV_CHATTINGROOM,
	LV_REPORTSHOT,
	LV_ITEMENCHANT,
	LV_ITEMUPGRADE,
	LV_EXTENDEDTERMITEM,
	LV_PETDISABLE,
	LV_LOVE,
	LV_LOVE_COLOR,

};


enum SITE_TYPE ;


/**
 * 게임내 포인트 거래 및 캐시아이템 즉시 구매시 <br> 
*/
enum CASH_TRADE {
	CASH_TRADE_NULL,				/**< 캐시 거래 불가 <br> */
	CASH_TRADE_ALL,					/**< 캐시및 포인트 거래 가능 <br> */
	CASH_TRADE_CASH,				/**< 캐시 거래 만 가능  <br> */
	CASH_TRADE_POINT,				/**< 포인트 거래만 가능  <br> */	
};


class SPLocalizeManager
{
public:
	static SPLocalizeManager* GetInstance();
	static void Release();

	bool Init();
	bool IsEnableValue(LOCALIZE_VALUE eValue);
	COUNTRY_TYPE GetCountryType();
	SITE_TYPE	GetSiteType() ;
	int GetMileageRatio();
	int GetMileageGlowUnit();
	int GetCashierGlowUnit();

	const char* GetHomePage();
	const char* GetNewAccount();
	const char* GetBillingPage();
	const char* GetHomeFindUserID();
	const char* GetHomeFindPasswd();
	const char* GetRegistryCompany();

	CASH_TRADE	GetCashTradeType();	
	CASH_TRADE	GetCashTradeDisplay();

	int GetAuctionNextBidRate();
	int GetAuctionTimeShort();
	int GetAuctionTimeMiddle();
	int GetAuctionTimeLong();
	int GetAuctionFeeRate();

	/**
	 * 치트용 함수 <br> 
	 @param 
	 @return  
	*/
	void		SetCashTradeType(int iType);

	int			GetCashTradeUnit();
	int			GetPointTradeUnit();
	int			GetCashFee();
	int			GetPointFee();

	bool OnlyWebLogin();			// Buddy, AtGames
private:
	SPLocalizeManager();
	~SPLocalizeManager();

	bool LoadLDTFile();

private:
	static SPLocalizeManager* m_pkInstance;

	COUNTRY_TYPE m_eCountry;	// 나라
	SITE_TYPE	m_ePublisher;	// 사이트
	bool m_bPrologue;
	bool m_bCashShop;
	bool m_bBeautyShop;
	bool m_bPet;
	bool m_bWayPoint;
	bool m_bCashShopCoupon;
	bool m_bCashShopGift;
	bool m_bGuild ;
	bool m_bIndun ;
	bool m_bJoyPad ;
	bool m_bLottery;
	bool m_bChatMacro;
	bool m_bItemMix;
	bool m_bGuideBook;
	bool m_bPVP;
	bool m_bChattingRoom;
	bool m_bItemEnchant;
	bool m_bItemUpgrade;
	int m_iMileageRatio;
	int m_iMileageGlowUnit;
	int m_iCashierGlowUnit;
	bool m_bReportShot;
	int iAuctionNextBidRate ;		// 상회입찰금 증가비율
	int	m_iAuctionShortTime ;		// 경매 시간(단기)
	int	m_iAuctionMiddleTime ;		// 경매 시간(중기)
	int	m_iAuctionLongTime ;		// 경매 시간(최장기)
	int m_iAuctioinFeeRate ;		// 경매 수수료 비율

	bool m_bExtendedTermItem;		// 아이템기간연장 Disable
	bool m_bPetDisable;				// 애완동물 Disable
	bool m_bPhantom;
	bool m_bLove;

	std::string m_strHomePage;
	std::string m_strNewAccount;
	std::string m_strBillingPage;
	std::string m_strFindUserID;
	std::string m_strFindPasswd;
	std::string m_strRegistryCompany;

	CASH_TRADE		m_iCashTrade;		/**< 유저간 캐시 거래 가능 플래그 <br> */
	CASH_TRADE		m_iCashDisplay;		/**< 게임상 캐시관련 정보 표현 플래그 <br> */
	int				m_iCashTradeUnit;	/**< 캐시 거래시 기본 단위 이하단위는 절삭 <br> */
	int				m_iPointTradeUnit;	/**< 포인트 거래시 기본 단위 이하 단위는 절삭 <br> */
	int				m_iCashFee;			/**< 캐시 거래시 강제로 제하는 값 <br> */
	int				m_iPointFee;		/**< 포인트 거래시 강제로 제하는 값<br> */
};






