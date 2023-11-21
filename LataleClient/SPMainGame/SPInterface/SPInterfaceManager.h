// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPInterfaceManager
// Comment     : 
// Creation    : metalgeni 2004-08-12 오후 2:56:36
//***************************************************************************


#pragma once 

#define SP_SHOW			true
#define SP_HIDE			false
#define SP_NONE			false
#define SP_TOGGLE_ON	true
#define SP_TOGGLE_OFF	false

const int MAX_HIDE_TOP = 113;
const int MAX_HIDE_BOTTOM = 514;
const int MAX_CUTIN_ALPHA = 128;
const int MAX_MESBOX      = 8;

enum NOTICE_TYPE;
enum MOUSE_TYPE;
class SPMouseCursor;
class SPWindow;
class SPIMEPool;
enum SUBGAME_ID;
class SPSoundArchive;

class SPInterfaceManager : public SPManager {
public:
	SPInterfaceManager();
	virtual ~SPInterfaceManager();

	virtual void Process	(float fTime);
	virtual	void Render		(float fTime);
	virtual void RenderTextDebug();
	virtual void RenderText();

	virtual bool Init();
	virtual void Clean();
		
	virtual bool PurgeAll(); // Purge Instant Data for Subgame Change
	virtual int SPPerformMessage	(UINT msg, WPARAM wparam = 0, LPARAM lParam= 0);

	INSTANCE_ID    MessageBox(const char* pszMsgText, int iType = MES_OK, 
								INSTANCE_ID tMsgWnd = -1, SPIM eMsg = SPIM_NULL);

	INSTANCE_ID    MessageBox(int iMsgID, int iType = MES_OK, 
								INSTANCE_ID tMsgWnd = -1, SPIM eMsg = SPIM_NULL);

	virtual int	WndProc(UINT message, WPARAM wParam, LPARAM lParam);
	void ProcessHotKey();	
	void ProcessJoyPad(float fTime);			//[2007/5/22]-jinhee
	void ProcessJoyPadButtons(float fTime);		//[2007/6/4]-jinhee

	SPIMEPool*	GetIMEPool();
	BOOL		IsKeyInput();
	void		SetKeyInput( BOOL bKeyInput );
	bool		RegisterSoundUnit(const char* pszFileName, float fDelay=0.0f, bool bLoop=false);

	bool SetSubGame(SUBGAME_ID SubGameID);
	bool IsSubGame() { return (m_ppCurSubgame)? true : false;}
	void SetIMEModeEnglish( bool bEnglish, bool bOnlyButton );

	int PerformMouseMove		( int iX, int iY);
	int PerformMouseLDown		( int iX, int iY);
	int PerformMouseLUp			( int iX, int iY);
	int PerformMouseRDown		( int iX, int iY);
	int PerformMouseRUp			( int iX, int iY);
	int PerformMouseLDBLClick	( int iX, int iY);
	int PerformMouseRDBLClick	( int iX, int iY);
	int PerformMouseWheelUp		( int iDelta );
	int PerformMouseWheelDown	( int iDelta );

	void SetCursorWindow		( SPWindow* pWindow );
	void SetFocusWindow			( SPWindow* pWindow );
	void SetActiveWindow		( SPWindow* pWindow );
	void SetModalWindow			( SPWindow* pWindow );
	void SetMoveWindow			( SPWindow* pWindow );


	SPWindow* GetCandVertical	();
	SPWindow* GetCursorWindow	();
	SPWindow* GetFocusWindow	();
	SPWindow* GetActiveWindow	();
	SPWindow* GetModalWindow	();
	SPWindow* GetLobbyWindow	();
	SPWindow* GetSubGameZone	();					//[liuyang, 2008.9.5]add;
	SPWindow* GetToolTipWindow	();
	SPWindow* GetLBToolTipWindow();
	int	GetCursorX();
	int	GetCursorY();

	bool CreateSPWindowAll();	

	bool	ToggleChat();
	bool	ShowWindow(INSTANCE_ID, bool bShow, bool bToggle=false, bool bSound = false);
	void	CheckShowWindow(INSTANCE_ID, bool bShow);
	void	SetToggleWindow();
	void	InsertToggleWindow(unsigned int uiKey, unsigned int uiValue);
	bool	IsShowWindow(INSTANCE_ID);
	void	OrderWindowPosMove();	// AJJIYA [9/8/2005]

	void	ClearMouse();
	void	SetMousePickup(SPTexture *icon, int nXSrcPos, int nYSrcPos);

	// NoticeBox
	SPWindow*		GetNoticeBox();
	bool			SetNotice(const char* pszMsg, SPWindow* pSender);
	bool			SetNotice(const char* pszMsg, SPGameObject* pSender);
	bool			SetNumNotice(const char* pszMsg, SPWindow* pSender);
	bool			SetNumNotice(const char* pszMsg, SPGameObject* pSender);
	bool			SetNoticeMsg(const char* pszMsg, SPWindow* pSender);
	bool			SetNoticeMsg(const char* pszMsg, SPGameObject* pSender);
	bool			SetNotice(const char* pszMsg, SPWindow* pSender, NOTICE_TYPE iType, int iShowTime = 0, SPIM IMDefaultAnswer = SPIM_NOTICE_NO );
	bool			SetNotice(const char* pszMsg, SPGameObject* pSender, NOTICE_TYPE iType, int iShowTime = 0, SPGM GMDefaultAnswer = SPGM_NOTICE_NO);
	bool			ForceReply();
	bool			SetNoticeHide();		//[2005/7/25] - jinhee

	// CutIn
	void CutInInit();
	void CutInClear();
	void CutInProcess (float fTime);			/**< CutIn시 Porcess <br> */
	void CutInRender (float fTime);				/**< CutIn시 Render <br> */
	void ResetRect();							/**< CutIn시 Window Hide Area Reset <br> */
	void SetCutIn(bool bCutIn = false);			/**< CutIn 설정 <br> */
	bool IsCutIn();								/**< CutIn 상태인햨E<br> */

	//[2005/7/8]	Inventory
	SPWindow*		GetStatPalletWindow();		/**< StatPallet Window <br> */
	SPWindow*		GetInvenWindow();			/**< Inventory Window <br> */
	SPWindow*		GetEquipWindow();			/**< Equip Windwo <br> */
	SPWindow*		GetStorageWindow();			/**< Local User Storage <br> */
	SPWindow*		GetPCInfoWindow();			/**< Another PC Infomation Window <br> */
	SPWindow*		GetPcTradeWindow();			/**< Pc to Pc Trade Window <br> */
	SPWindow*		GetNpcTalkWindow();			/**< Npc Talk Window <br> */
	SPWindow*		GetPetWindow();				/**< Pet Equip Window <br> */
	SPWindow*		GetSoftKeyWindow() ;		/**< Login Softkeyboard Window <br> */
	SPWindow*		GetGuildInfoWindow();
	SPWindow*		GetCreateGuildWindow();
	SPWindow*		GetGuildEmblemWindow();
	SPWindow*		GetDestroyGuildWindow();
	SPWindow*		GetGuildLevelupWindow();
	SPWindow*		GetAuctionWindow();
	SPWindow*		GetAuctionMsgWindow();

	bool IsItemEnableWindow(SPWindow *pWindow); /**< 아이템이 올라갈펯E있는 윈도퓖E适갋확인한다 <br> */

	BOOL			IsShowLooting();			// 루팅백이 표시되엉復는햨E확인한다. AJJIYA [8/10/2005]

	void PreDeleteWindow(SPWindow* pkWindow);

	//	AJJIYA [10/25/2005]
	//	메세지를 하위 차일탛E윈도퓖E?픸E貧?하는경퓖E사퓖E磯?
	//	특별한 동작이 필요할경퓖E?기존의 SPPerformMessage를 사퓖E臼?처리하자.
	int SPChildWndSendMessage( INSTANCE_ID tChildWnd , SPIM eMsg , WPARAM wParam, LPARAM lParam, bool bFindChild = false );

	SPWindow*	FindWindow(INSTANCE_ID); // metalgeni [11/9/2005]

	// CashShop, Storybook, TextBook, PVPLoadWait, PVPResult 에 사용
	// Main Interface 지우고 wndID 만 띄움
	void SetOnlyWindowView(INSTANCE_ID wndID, bool bIn, bool bHideChat = true) ;  
	void SetHotKeyBlock(bool bBlock) ;		// Set HotKey Block
	bool GetHotKeyBlock() ;

	void	SetShowDefaultWindow( bool bShow );

	void		InitClassTextureInfo();
	const char*	GetClassTextureName		( CLASS_TYPE eClassType );
	const char*	GetClassTextureNameEx	( CLASS_TYPE eClassType );
	RECT*		GetClassTextureSrc		( CLASS_TYPE eClassType );
	RECT*		GetClassTextureSrcEx	( CLASS_TYPE eClassType );

	const char* GetCalcLumpStringChat();

	void SetEnglishOnly(bool bVal);


//protected:
private:

	SPIMESSAGE_MAP_DECLARE()	
	int OnChatGetMsg			(WPARAM wparam, LPARAM lparam);
	int OnChatGetBugle			(WPARAM wparam, LPARAM lparam);
	int OnChatGetVisualBugle	(WPARAM wParam, LPARAM lParam);
	int OnChatMessage			(WPARAM wparam, LPARAM lparam);
	int OnChatClear				(WPARAM wparam, LPARAM lparam); 
	int OnChatInsertMsgEdit		(WPARAM wparam, LPARAM lparam);		// AJJIYA [9/7/2005]
	int OnRefreshPlayerStat		(WPARAM wParam, LPARAM lParam);	
	int OnTeamIconUpdate		(WPARAM wParam, LPARAM lParam);	
	
	//[2005/4/14] for MiniMap 스테이햨E또는 그톩E?바끔劤 경퓖E호출해준다 
	int OnSetStage				(WPARAM wParam, LPARAM lParam);

	//[2005/5/3] for MessageWindow 
	int OnMessageShow			(WPARAM wParam, LPARAM lParam);
	int OnMessageTextSet		(WPARAM wParam, LPARAM lParam);

	// looting
	int OnLootingItemSet		(WPARAM wParam, LPARAM lParam);
	int OnLootingItemDelete		(WPARAM wParam, LPARAM lParam);
	int OnLootingItemReDraw		(WPARAM wParam, LPARAM lParam);	

	//[2005/5/17] for Npc Shop(Quest) Window
	int OnShowNpcShopWindow		(WPARAM wParam, LPARAM lParam);

	//[2005/5/18] 컷인시 보여주엉雹 할 윈도퓖E?컖E쳬?설정을 해준다.
	int OnCutInInterfaceShow	(WPARAM wParam, LPARAM lParam);

	//[2005/5/20] 판매창에 올라가는 경퓖E(또는 그 반큱E 아이템 아이콘의 알파적퓖E갋
	int OnItemSellSelect		(WPARAM wParam, LPARAM lParam);	

	//[2005/5/21]
	int OnExitButtonShow		(WPARAM wParam, LPARAM lParam);

	int OnChannelListSet		(WPARAM wParam, LPARAM lParam);

	int OnSetPlayerInfo			(WPARAM wParam, LPARAM lParam);

	int OnSetPCInfoStatus		(WPARAM wParam, LPARAM lParam);	

	// Team
	int OnTeamJoin				(WPARAM wParam, LPARAM lParam);
	int OnTeamSecede			(WPARAM wParam, LPARAM lParam);
	int OnTeamUpdate			(WPARAM wParam, LPARAM lParam);
	int OnTeamUpdateStatus		(WPARAM wParam, LPARAM lParam);
	int OnTeamUpdateLootingRule (WPARAM wParam, LPARAM lParam);

	int OnItemIDInfo			(WPARAM wParam, LPARAM lParam);

	int OnSetErrorMsg			(WPARAM wParam, LPARAM lParam);	

	int WndEscapeProc			(WPARAM wParam, LPARAM lParam);
	int WndReturnProc			(WPARAM wParam, LPARAM lParam);

	void CreatePrologueWindow();
	void ReleasePrologueWindow();

private:

	SPMouseCursor		m_MouseCursor;
	SPIMEPool*			m_pIMEPool;
	SPSoundArchive*		m_pSoundArchive;
	POINT				m_PtSoundPos;

	SPWindow*			m_pWindowCursorIn;
	SPWindow*			m_pWindowFocusIn;
	SPWindow*			m_pWindowActive;		// Focus먹은 윈도퓖E?포함한 WrapWindow
	SPWindow*			m_pWindowModal;
	SPWindow*			m_pWindowMove;


	SPWindow**			m_ppCurSubgame;
	SPWindow*			m_pRoot;
	SPWindow*			m_pSubGameZone;
	SPWindow*			m_pSubGameLogin;	
	SPWindow*			m_pSubGameLobby;
	SPWindow*			m_pSubGamePrologue;

	SPWindow*			m_pCandVertical;

	// 따로 보컖E.(자주 검색해야 하는것탛E.)
	SPWindow*			m_pWindowStat;				//[8/2/2005] - jinhee StatPallet
	SPWindow*			m_pWindowToolTip;
	SPWindow*			m_pWindowLBToolTip;
	SPWindow*			m_pWindowChat;
	SPWindow*			m_pWindowTalkingRoom;		//  [4/29/2008 AJJIYA]
	SPWindow*			m_pWindowNpcTalk;			//[2005/5/10] - jinhee
	SPWindow*			m_pWindowPcTalk;			//[2007/9/13] - jinhee
	SPWindow*			m_pWindowShopTalk;			//[2005/5/16] - jinhee
	SPWindow*			m_pWindowNpcTrade;			//[2005/5/17] - jinhee
	SPWindow*			m_pWindowStorage;			//[2005/7/15] - jinhee
	SPWindow*			m_pWindowGift;				//[2006/3/20] - jinhee
	SPWindow*			m_pWindowMailList;			//[2006/4/11] - jinhee
	SPWindow*			m_pWindowMailMsg;			//[2006/4/11] - jinhee
	SPWindow*			m_pWindowInven;
	SPWindow*			m_pWindowEquip;
	SPWindow*			m_pWindowStatus;	
	SPWindow*			m_pWindowPcInfo;			//[2005/7/18] - jinhee
	SPWindow*			m_pWindowPcTrade;			//[2005/7/18] - jinhee
	SPWindow*			m_pWindowLooting;			// AJJIYA [8/10/2005]
	SPWindow*			m_pWindowNpcQuest;			//[2005/9/22] - jinhee
	SPWindow*			m_pWindowQuest;				// AJJIYA [9/23/2005]
	SPWindow*			m_pWindowQuestView;
	SPWindow*			m_pWindowBeauty;			//[2006/10/10]
	SPWindow*			m_pWindowPet;				//[2006/10/20]
	SPWindow*			m_pWindowSoftKB;			//[2007/03/14]
	SPWindow*			m_pWindowGuildInfo;			//[2007/04/11]
	SPWindow*			m_pWindowCreateGuild;		//[2007/04/11]
	SPWindow*			m_pWindowDestroyGuild;		//[2007/04/25]
	SPWindow*			m_pWindowGuildLevelup;		//[2007/09/03]
	SPWindow*			m_pwindowGuildEmblem;		//[2007.09.06]
	SPWindow*			m_pWindowGuildCrop;			//[2007/6/15]-jinhee
	SPWindow*			m_pWindowNpcPortal;			//[2007/8/2]-jinhee
	SPWindow*			m_pWindowNpcEventList;		//[2008/6/12]-jinhee
	SPWindow*			m_pWindowNpcEvent;			//[2007/8/7]-jinhee
	SPWindow*			m_pWindowSystemMessage;		//[2007/10/1]-jinhee
	SPWindow*			m_pWindowStoryBook;			//[2007/9/17]-Duragon
	SPWindow*			m_pWindowPvpReserve;		//[2007/11/7]-jinhee
	SPWindow*			m_pWindowPvpEnterWait;		//[2007/11/7]-jinhee
	SPWindow*			m_pWindowPvpResult;			//[2007/12/13]-Duragon
	SPWindow*			m_pWindowAuction;			//[2008/05/19]-Duragon
	SPWindow*			m_pWindowAuctionMSG;		//[2008/05/19]-Duragon
	SPWindow*			m_pWindowLoveMsgInput;		//  [12/5/2008 AJJIYA]
	SPWindow*			m_pWindowLoveMsgView;		//  [12/5/2008 AJJIYA]

	SPWindow*			m_pNoticeBox;
	SPWindow*			m_pMessageBox[MAX_MESBOX];  //[xialin 2008/07/23]add

	int					m_iCursorX;
	int					m_iCursorY;

	SUBGAME_ID			m_SubGameID;

	// Key List
	int 				m_iKeyTab;
	int 				m_iKeyReturn;
	int					m_iKeyShift;
	int					m_iKeyEscape;
	// Login Key List


	// Zone Key List 
	int					m_iKeyInventory;
	int 				m_iKeyStatus;
	int					m_iKeyItem;	
	int					m_iKeyEquip;	
	int					m_iKeySkill;
	int					m_iKeyFriend;
	int					m_iKeyQuest;
	int					m_iKeyOption;
	int					m_iKeyTeam;
	int					m_iKeyHelp;
	int					m_iKeyWorldMap;
	int					m_iKeyWorldMapStage;
	int					m_iKeyEnchant;
	int					m_iKeyItemUpgrade;
	int					m_iKeyWayPoint;
	int					m_iKeyPet;
	int					m_iKeyUserGuildInfo;
	int					m_iKeyGuideBook;
	int					m_iKeyChattingRoom;
	int					m_iKeyLove;
	
//	int					m_iKeyUserShopSell;
//#ifdef _DEBUG	
//	int					m_iKeyUserShopBuy;
//#endif


	// position of SubWindows
	POINT				m_ptItemPos, m_ptItemSize;
	POINT				m_ptEquipPos, m_ptEquipSize;
	POINT				m_ptSkillPos, m_ptSkillSize;
	POINT				m_ptStatusPos, m_ptStatusSize;
	POINT				m_ptLootingPos, m_ptLootingSize;
	POINT				m_ptQuickSlotPos, m_ptQuickSlotSize;

	BOOL				m_bKeyInput;			//	하위 윈도퓖E【?Edit를 사퓖E?키입력시 메뉴 단축키를 막자!

	// CutIn
	bool				m_bCutIn;				/**< CutIn Status <br> */
	float				m_fAccumulateRender;	/**< 렌더링 시간 계퍊E<br> */
	float				m_fCurDelay;			/**< 프레임 간격 시간 <br> */
	SPTexture*			m_pTexture;				/**< CutIn Hide Texture <br> */
	RECT				m_rtSrcHide;			/**< CutIn Hide Source Area <br> */
	RECT				m_rtDestTop;			/**< Cut In Top hide Area <br> */
	RECT				m_rtDestBottom;			/**< Cut In Bottom hide Area <br> */
	RECT				m_rtDestNPC;			/**< Cut In Npc Area <br> */
	bool				m_bTopStop;
	bool				m_bBottomStop;
	int					m_iCutInAlpha;			/**< <br> */
	int					m_iAlpha;				/**< <br> */
	float				m_fCutInAlpha;

	bool				m_bActiveErrorMsg;
	
	bool				m_bCashShop;
	bool				m_bBlockHotKey;			/**< HotKey Block <br> */
	
	POINT				m_ptInven;
	POINT				m_ptEquip;

	bool				m_bJoyPadLPush;			/**< <br> */
	bool				m_bJoyPadRPush;			/**< <br> */

	std::map< unsigned int, std::vector< SPWindow* > > m_mWindowToggleList;

	POINT				m_ptOldSystemMessage;
	POINT				m_ptCutInSystemMessage;
	bool				m_bMinimizedSystemMessage;

	std::string			m_strClassTextureName	[CLASS_MAX];
	std::string			m_strClassTextureNameEx	[CLASS_MAX];
	RECT				m_rcClassTextureSrc		[CLASS_MAX];
	RECT				m_rcClassTextureSrcEx	[CLASS_MAX];

};

extern SPInterfaceManager* g_pInterfaceManager;


