
#ifndef __SPWINDOW_LOBBY_H__
#define __SPWINDOW_LOBBY_H__

#define LOBBY_SERVER_LIST_MAX		50		// 서버목록은 20개 까지
#define LOBBY_SERVER_VIEW_MAX		8		// 보여지는것은 8개
#define LOBBY_SERVER_PAGESIZE		1		// 페이지 단위 (1)

//------------------------------------------------------------------------------------
// ServerLobby
#define WIID_LOBBY_SERVER_TITLE			(WIID_LOBBY + 1)

#define WIID_LOBBY_SERVERLIST_SKIN			(WIID_LOBBY + 2)
#define WIID_LOBBY_SERVERLIST_SLIDER		(WIID_LOBBY + 3 )
#define WIID_LOBBY_SERVERLIST_SLIDER_UP		(WIID_LOBBY + 4 )
#define WIID_LOBBY_SERVERLIST_SLIDER_DOWN	(WIID_LOBBY + 5 )

#define WIID_LOBBY_SERVERLIST_ICON		(WIID_LOBBY + 41)	// MAX 50 -> But 8 View
#define WIID_LOBBY_SERVERLIST_BUTTON	(WIID_LOBBY + 51)	
#define WIID_LOBBY_SERVERLIST_CHARNUM	(WIID_LOBBY + 61)
#define WIID_LOBBY_SERVERLIST_STATE		(WIID_LOBBY + 71)

#define WIID_LOBBY_CHANNELLIST_SKINTOP	(WIID_LOBBY + 200)		// Skin TOP
#define WIID_LOBBY_CHANNELLIST_SKINMDL	(WIID_LOBBY + 201)		// Skin Middle (4)
#define WIID_LOBBY_CHANNELLIST_SKINBTM	(WIID_LOBBY + 206)		// Skin Bottom
#define WIID_LOBBY_CHANNELLIST_ICON		(WIID_LOBBY + 207)
#define WIID_LOBBY_CHANNELLIST_SERVER	(WIID_LOBBY + 208)
#define WIID_LOBBY_CHANNELLIST_BUTTON	(WIID_LOBBY + 209)		// 105 ~ 124 (20)
#define WIID_LOBBY_CHANNELLIST_STATE	(WIID_LOBBY + 229)		// 125 ~ 144 (20)
#define WIID_LOBBY_CHANNELLIST_CONNECT_BG	(WIID_LOBBY + 250)
#define WIID_LOBBY_CHANNELLIST_CONNECT		(WIID_LOBBY + 251)

#define WIID_LOBBY_SERVER_BACK_SKIN		(WIID_LOBBY + 290)
#define WIID_LOBBY_SERVER_BACK			(WIID_LOBBY + 291)


//------------------------------------------------------------------------------------
// Character Select Lobby
#define WIID_LOBBY_CHAR_TITLE			(WIID_LOBBY + 301)
#define WIID_LOBBY_CHAR_SERVER_SKIN		(WIID_LOBBY + 302)
#define WIID_LOBBY_CHAR_SERVER_ICON		(WIID_LOBBY + 303)
#define WIID_LOBBY_CHAR_SERVER_NAME		(WIID_LOBBY + 304)

#define WIID_LOBBY_CHAR_SKIN			(WIID_LOBBY + 305)
#define WIID_LOBBY_CHAR_CARD			(WIID_LOBBY + 306) // +3장

#define WIID_LOBBY_CHAR_MENU_SKIN		(WIID_LOBBY + 311)
#define WIID_LOBBY_CHAR_MENU_SELECT		(WIID_LOBBY + 312)
#define WIID_LOBBY_CHAR_MENU_CREATE		(WIID_LOBBY + 313)
#define WIID_LOBBY_CHAR_MENU_DELETE		(WIID_LOBBY + 314)

#define WIID_LOBBY_CHAR_BACK_SKIN		(WIID_LOBBY + 350)
#define WIID_LOBBY_CHAR_BACK			(WIID_LOBBY + 351)


//------------------------------------------------------------------------------------
// Character Create Lobby
#define WIID_LOBBY_CHAR_CREATE_TITLE			(WIID_LOBBY + 401)

#define WIID_LOBBY_CHAR_CREATE_INFO_SKIN		(WIID_LOBBY + 402)
#define WIID_LOBBY_CHAR_CREATE_INFO_NAME		(WIID_LOBBY + 404)
//#define WIID_LOBBY_CHAR_CREATE_INFO_NAME_CHECK	(WIID_LOBBY + 505)
#define WIID_LOBBY_CHAR_CREATE_INFO_ZENDER_M	(WIID_LOBBY + 406)
#define WIID_LOBBY_CHAR_CREATE_INFO_ZENDER_W	(WIID_LOBBY + 407)
#define WIID_LOBBY_CHAR_CREATE_INFO_ATTACK		(WIID_LOBBY + 408)
#define WIID_LOBBY_CHAR_CREATE_INFO_DEFFEN		(WIID_LOBBY + 409)
#define WIID_LOBBY_CHAR_CREATE_INFO_MAGIC		(WIID_LOBBY + 410)
#define WIID_LOBBY_CHAR_CREATE_INFO_SCOUT		(WIID_LOBBY + 411)
#define WIID_LOBBY_CHAR_CREATE_TYPE_INFO		(WIID_LOBBY + 412)

#define WIID_LOBBY_CHAR_CREATE_ICON_KNUCKLE		(WIID_LOBBY + 413)		// Use UI_TOOLTIP.LDT	536875421
#define WIID_LOBBY_CHAR_CREATE_ICON_TWO_SD		(WIID_LOBBY + 414)
#define WIID_LOBBY_CHAR_CREATE_ICON_SPERE		(WIID_LOBBY + 415)
#define WIID_LOBBY_CHAR_CREATE_ICON_ONE_SD		(WIID_LOBBY + 416)
#define WIID_LOBBY_CHAR_CREATE_ICON_HAMMER		(WIID_LOBBY + 417)
#define WIID_LOBBY_CHAR_CREATE_ICON_DAGGER		(WIID_LOBBY + 418)
#define WIID_LOBBY_CHAR_CREATE_ICON_BOW			(WIID_LOBBY + 419)
#define WIID_LOBBY_CHAR_CREATE_ICON_CROSSBOW	(WIID_LOBBY + 420)
#define WIID_LOBBY_CHAR_CREATE_ICON_WAND		(WIID_LOBBY + 421)

#define WIID_LOBBY_CHAR_CREATE_SKIN				(WIID_LOBBY + 431)
#define WIID_LOBBY_CHAR_CREATE					(WIID_LOBBY + 432)

#define WIID_LOBBY_CHAR_CREATE_CHANGE_SKIN		(WIID_LOBBY + 442)
#define WIID_LOBBY_CHAR_CREATE_CHANGE_LEFT		(WIID_LOBBY + 443)		// +7
#define WIID_LOBBY_CHAR_CREATE_CHANGE_RIGHT		(WIID_LOBBY + 451)		// +7
#define WIID_LOBBY_CHAR_CREATE_CHANGE_CURNUM	(WIID_LOBBY + 459)		// +7
#define WIID_LOBBY_CHAR_CREATE_CHANGE_TOLNUM	(WIID_LOBBY + 467)		// +7
#define WIID_LOBBY_CHAR_CREATE_CHANGE_EXPLAIN	(WIID_LOBBY + 475)		// +7

#define WIID_LOBBY_CHAR_CREATE_BACK_SKIN		(WIID_LOBBY + 490)
#define WIID_LOBBY_CHAR_CREATE_BACK				(WIID_LOBBY + 491)

#define WIID_LOBBY_DIALOG_JUMIN					(WIID_LOBBY + 500)
#define WIID_LOBBY_DIALOG_JUMIN_OK				(WIID_LOBBY + 501)
#define WIID_LOBBY_DIALOG_JUMIN_CANCEL			(WIID_LOBBY + 502)
#define WIID_LOBBY_DIALOG_JUMIN_EDIT			(WIID_LOBBY + 503)

#define WIID_LOBBY_DIALOG_INFOMATION			(WIID_LOBBY + 510)
#define WIID_LOBBY_DIALOG_INFOMATION_OK			(WIID_LOBBY + 511)
#define WIID_LOBBY_DIALOG_INFOMATION_LIST		(WIID_LOBBY + 512)

class SPWindow;
class SPPlayer;

class SPImageProcessor;
class SPWindowSlider;
class SPWindowLobby : public SPWindow
{
public:
	SPWindowLobby(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowLobby(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);

	virtual ~SPWindowLobby();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);

	void SetLobbyType(int iType);
	void ShowLobbyType(int iType);

	void SetServerList(std::vector< WORLD_INFO >& vWorldList);
	int	 UpdateServerList( int iPage, vector< WORLD_INFO >& vWorldList) ;
	void SetChannelList(WORLD_INFO* pServer, std::vector< CHANNEL_INFO >& vChannelList);

	void SetCharacterList(std::vector< CHARACTER_INFO2 >& vCharInfo,
		std::map< unsigned int, std::vector< CONTAINER_ITEM > >& mItem);
	void SetCharacterNameCheck(bool bTrue);

	bool SetInfomationDialog(bool bShow, int iNoticeType = 0, const TCHAR* szMsg = NULL) ;

	void EnableChildWindow(int iLobbyType, bool bEnable);
	void InitLobbyType();
	void ClearImageProcessor();

protected:

	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnCursorEnter	(WPARAM, LPARAM);
	virtual int OnCursorOut		(WPARAM, LPARAM);
	virtual int OnConnect		(WPARAM, LPARAM);
	virtual int OnServerBack	(WPARAM, LPARAM);
	virtual int OnWeaponTooltipEnter(unsigned int, WPARAM, LPARAM);
	virtual int OnWeaponTooltipOut	(unsigned int, WPARAM, LPARAM);
	virtual int OnWeaponTooltipMove	(unsigned int, WPARAM, LPARAM);

	virtual int OnCharMenuSelect(WPARAM, LPARAM);
	virtual int OnCharMenuCreate(WPARAM, LPARAM);
	virtual int OnCharMenuDelete(WPARAM, LPARAM);
	virtual int OnJuminOk		(WPARAM, LPARAM);
	virtual int OnJuminCancel	(WPARAM, LPARAM);
	virtual int OnCharBack		(WPARAM, LPARAM);

	virtual int OnCharCrateNameFocus(WPARAM, LPARAM);
	virtual int OnCharCreateNameCheck(WPARAM, LPARAM);
	virtual int OnCharCreateRun	(WPARAM, LPARAM);
	virtual int OnCharCreateBack(WPARAM, LPARAM);

	virtual int OnInfomationOk	(WPARAM, LPARAM);
	virtual int OnKeyReturn		(WPARAM, LPARAM);

	virtual int OnServerClick	(unsigned int, WPARAM, LPARAM);
	virtual int OnChannelClick	(unsigned int, WPARAM, LPARAM);
	virtual int OnChannelDBLClick(unsigned int, WPARAM, LPARAM);
	virtual int OnCardClick		(unsigned int, WPARAM, LPARAM);
	virtual int OnCardDBLClick	(unsigned int, WPARAM, LPARAM);

	virtual int OnZenderClick		(unsigned int, WPARAM, LPARAM);
	virtual int OnClassClick		(unsigned int, WPARAM, LPARAM);
	virtual int OnChangeLeftClick	(unsigned int, WPARAM, LPARAM);
	virtual int OnChangeRightClick	(unsigned int, WPARAM, LPARAM);

	virtual int	OnSliderUp	( WPARAM wParam, LPARAM lParam );
	virtual int	OnSliderDown( WPARAM wParam, LPARAM lParam );
	virtual int	OnSlider	( WPARAM wParam, LPARAM lParam );
	virtual int	OnWheelSliderUp	( unsigned int, WPARAM wParam, LPARAM lParam );
	virtual int	OnWheelSliderDown( unsigned int, WPARAM wParam, LPARAM lParam );

	virtual int	OnClickTest( unsigned int, WPARAM wParam, LPARAM lParam );

	void InitServerListWindow();
	void InitChannelListWindow();

	void ClearServerList();
	void ClearChannelList();
	void ClearChracterList();

	void InitCharWindow();
	void InitCharCreateWindow();
	void InitCharCreateItem();
	void InitDefaultCharCreateImage();

	void CreateDialogBox();
	void DeleteDialogBox();

	void RenderCard(float fTime);
	void ForceRenderCard(int iNum);

	void SetDefaultCharacetLayer();
	void SetChangeCharacterLayer();
	void SetCurSelectLayer();
	void SetMyCharacterItem(int iIndex, std::map< unsigned int, std::vector< CONTAINER_ITEM > >& mItem);
	void SetClassWeapon(int iClassType) ;

	void SetImageProcessor(int iType, bool bShow);
	void SetServerImageProcessor(bool bShow);
	void SetCharSelectImageProcessor(bool bShow);
	void SetCharCreateImageProcessor(bool bShow);
	void SetCardImageProcessor(float fElapsedTime);
	void ProcessImageProcessor(float fElapsedTime);
	void RenderImageProcessor();

	void StatusProcess(float fElapsedTime);
	void SetStatusGage(int iType);
	void RenderStatus();

	struct CHAR_ANI_TIME
	{
		bool bInit;
		float fTime;
		float fActionTime;
	};

	void SetAniSeq(int iIndex, int iClassType, int iSex, bool bInit, bool bSelect);


protected:

	// 0: server select 1: char select 2: char create
	int m_iType;
	int m_iNextType;

	// server value
	RECT m_rcServerIconBase;
	RECT m_rcServerButtonBase;
	RECT m_rcCharNumBase;
	RECT m_rcServerStateBase;

	int m_iServerIconInterval;
	int m_iServerButtonInterval;
	int m_iCharNumInterval;
	int m_iServerStateInterval;
//	int m_iPageServerNum;

	WORLD_INFO* m_pCurServerInfo;
	SPWindowSlider*	m_pSlider;

	// channel value
	RECT m_rcChannelButtonBase;
	RECT m_rcChannelStateBase;

	int m_iCHANNEL_BTN_VERTICAL_INTERVAL;
	int m_iCHANNEL_BTN_HORIZEN_INTERVAL;
	int m_CHANNEL_MAXX;
	int m_CHANNEL_MAXY;

	int m_iSelectServer;
	int m_iSelectChannel;
	int m_iServerListSize ;
	bool m_bIsSlide ;
	int m_iServerSliderTop ;
	int m_iSliderMoveRange ;		// ListSize - ViewSize = MoveRange

	RECT m_rcSrcState[4];

	// char select value
	RECT m_rcDstCardBase;
	RECT m_rcDstCardNumBase;
	RECT m_rcDstCardSelectBase;

	RECT m_rcSrcCardFront;
	RECT m_rcSrcCardBack;
	RECT m_rcSrcCardSelect;
	RECT m_rcSrcCardNum[3];

	int m_iCardInterval;
	int m_iCardSelectInterval;
	int m_iSelectCard;
	bool m_bCardRender;
	int m_iCardProcessing;
	float m_fCardProcessingTime;
	int m_iCardProcessingCount;
	int m_iCardProcessingCompleteCount;

	SPTexture* m_pkCard;

	int m_iCharacterNum;
	CHAR_ANI_TIME m_MyCharacterAniTime[3];
	SPPlayer* m_pkMyCharacter[3];
	std::vector< CHARACTER_INFO2 > m_vMyCharacterInfo;
	SPPlayer* m_pkCreateCharacter;

	map< unsigned int, std::vector< CONTAINER_ITEM > > m_mTempCharacterItem;
	
	bool m_bCharacterNameSuccess;
	std::string m_strCharacterName;

	int m_iZender;
	int m_iBeforeZender;

	int m_iClassType;
	int m_iCurLayerIndex[7];
	int m_iBeforeLayerIndex[7];

	int m_iImageProcessorType;
	bool m_bWaitChageSubGame;
	std::vector< SPImageProcessor* > m_vImageProcessor;

	SPTexture* m_pkStatusSkin;
	RECT m_rcStatusSkinDst;
	RECT m_rcStatusSkinSrc;
	RECT m_rcStatusDst[4];
	RECT m_rcStatusSrc[4];
	int m_iMaxStatusDst[4];
	float m_fCurStatusWidth[4];
	int m_iMaxStatusWidth;

	std::vector< unsigned int > m_vHairID[2];
	std::vector< unsigned int > m_vHairColorID[2];
	std::vector< unsigned int > m_vExpressionID[2];
	std::vector< unsigned int > m_vExpressionColorID[2];
	std::vector< unsigned int > m_vSkinID[2];
	std::vector< unsigned int > m_vBlouseID[2];
	std::vector< unsigned int > m_vPantsID[2];
	std::vector< std::string > m_vstrHair[2];
	std::vector< std::string > m_vstrHairColor[2];
	std::vector< std::string > m_vstrExpression[2];
	std::vector< std::string > m_vstrExpressionColor[2];
	std::vector< std::string > m_vstrSkin[2];
	std::vector< std::string > m_vstrBlouse[2];
	std::vector< std::string > m_vstrPants[2];
};

#endif