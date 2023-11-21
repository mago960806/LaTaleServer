#pragma once

#define WIID_PCTALK_BASE		(WIID_PCTALK	+	1 )
#define WIID_PCTALK_PCNAME		(WIID_PCTALK	+	10 )
#define WIID_PCTALK_LINE		(WIID_PCTALK	+	20 )
#define WIID_PCTALK_MESSAGE		(WIID_PCTALK	+	30 )
#define WIID_PCTALK_MESSAGE_L	(WIID_PCTALK	+	31 )
#define WIID_PCTALK_MENU		(WIID_PCTALK	+	50 )
#define WIID_PCTALK_BUTTON		(WIID_PCTALK	+	100 )


enum PC_MENU {
	PC_MENU_0,
	PC_MENU_1,
	PC_MENU_2,
	PC_MENU_3,
	PC_MENU_MAX,
};


enum TALK_FADE;
enum TALK_STATE;

class SPWindowPcTalk : public SPWindow {

public:	
	SPWindowPcTalk(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowPcTalk();

	virtual void Init();
	virtual void Clean();
	//virtual void SetFocus();

	virtual void Show(bool bWithChild = true);
	void ShowComplete();

	virtual void Hide(bool bSendServer = true);


	virtual void Process(float fTime);
	virtual void Render(float fTime);

protected:
	/**
	 * 내부 컨트롤을 초기화한다 <br> 
	 @param 
	 @return  
	*/
	void	InitSubControl();
	
	/**
	 * 문자열 출력 관련 컨트롤을 초기화 한다 <br> 
	 @param 
	 @return  
	*/
	void	ClearMenuText(bool bMenu);
	
	/**
	 * 메뉴 선택시 관련한 동작을 한다 <br> 
	 @param 
	 @return  
	*/
	int		SetSelectItem(int iItem);	

	/**
	* 해당 index의 NpcTalkData를 Hander에서 로드해 설정하도록 한다 <br>
	* 내부에서 SetNpcTalk를 호출한다 <br> 
	@param 
	@return NONE
	*/
	void SetNpcTalkData(int iTalkIndex = 0);

	/**
	* 로드한 NpcTalkData의 BoxPos에 의한 다음 NpcTalk동작을 지시 <br>
	@param 
	@return NONE
	*/
	void SetNpcTalk();	

	/**
	* SavePoint에 위치 저장 메시지를 보낸다 <br> 
	@param NONE
	@return bool
	*/
	bool SendEventPacket();

	/**
	* 상점을 선택한 경우 상점 데이터를 Event Hander가 로드 할수 있도록 하고 <br> 
	* NPC Event Shop 상태로 이전한다 
	@param int iTradeIndex = 0			로드할 상점 데이터 인덱스
	@return NONE
	*/
	void SetNpcShopData(int iTradeIndex = 0);

	/**
	* 창고 상태로 바꿔준다 <br>
	@param NONE 
	@return NONE
	*/
	void SetNpcStorage(int iStorageIndex = 0);

	/**
	* Quest 상태로 바꿔준다 <br>
	@param 
	@return
	*/
	void SetNpcQuest(int iQuestIndex = 0);

	/**
	* Gift 상태로 바꿔준다 <br>
	@param 
	@return
	*/
	void SetNpcGift(int iGiftIndex = 0);

	/**
	* Guild 생성 관리 상태로 바꿔준다 <br>
	@param
	@return
	*/
	void SetNpcGuildCreate(int iGuildIndex = 0);
	
	/**
	 *
	 @param 
	 @return  
	*/
	void SetNpcGuildDissolve(int iGuildIndex = 0);
	
	/**
	 *
	 @param 
	 @return  
	*/
	void SetNpcGuildEnter();
	
	/**
	 *
	 @param 
	 @return  
	*/
	void SetNpcGuildLevelUp(int iGuildIndex = 0);
	
	/**
	 *
	 @param 
	 @return  
	*/
	void SetNpcGuildEmblemChange(int iGuildIndex = 0);	

	/**
	* Npc Portal 상태로 바꿔준다 <br> 
	@param 
	@return  
	*/
	void SetNpcPortal(int iPortalIndex = 0);

	/**
	* Npc Event 상태로 바꿔준다 <br> 
	@param 
	@return  
	*/
	void SetNpcEvent(int iEventIndex = 0);

	/**
	*
	@param 
	@return  
	*/
	void SetNpcPvpReserve(int iPvpIndex = 0, bool bParty = false);

	/**
	 *
	 @param 
	 @return  
	*/
	void SetNpcAuction(int iAuctionIndex = 0);

	/**
	 * PC Evnet 윈도우가 포커스를 잃은것저럼 설정 <br> 
	 * 이 루프를 타지 않는 코드가 있으니 주의 <br> 
	 @param 
	 @return  
	*/
	void SetFocusLose(bool bLose = false);

	/**  
	 * NPC Item Option 재분배 상태로 바꿔준다 <br>
	 @param  
	 @return 
	*/  
	void SetNpcItemReOption(int iReOptionIndex = 0);


	/** W.Fairy : 2008.10.07
	* 랭킹 상태로 바꿔준다 <br>
	@param NONE 
	@return NONE
	*/
	void SetNpcRanking(int rec_Index = 0);


protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnHideStart			(WPARAM wParam, LPARAM lParam);				/**< Faid in 작업을 시작 <br> */
	int OnSetNpcData		(WPARAM wParam, LPARAM lParam);				//처음 SPNpc를 이용하여 UI를 설정한다 
	int OnSetSavePoint		( WPARAM wParam, LPARAM lParam );			//Save Point 이용하여 UI를 설정한 경우
	int OnSetNpcTalk		( WPARAM wParam, LPARAM lParam );			/**< 외부에서 NPCTALK를 로드 하도록 요청한 경우 <br> */
	int OnNpcTalkItemSelect	( WPARAM wParam, LPARAM lParam );			/**< 메뉴관련 컨트롤에 의해 선택 동작이 발생하는 시점 <br> */
	int OnItemSelect		(unsigned int uiID, WPARAM wParam, LPARAM lParam);	/**< OnNpcTalkItemSelect 호출 <br> */
	int OnClick				(WPARAM wParam, LPARAM lParam);				/**< 해당 윈도우가 클릭된 경우 <br> */
	int OnSetFocusLose		(WPARAM wParam, LPARAM lParam);				/**< SetFocusLose 호출 <br> */
	// }}
	
private:
	SPWindow*	m_pParent;
	SPTexture*	m_pWindowSkin;					/**< 윈도우 스킨 텍스쳐 <br> */
	
	RECT		m_rtSkinSrc;					/**< 윈도우 스킨 소스 영역 <br> */
	RECT		m_rtSkinDest;					/**< 윈도우 스킨 출력 영역 <br> */
	
	RECT		m_rtSkinBaseDest;				/**< 실제 윈도우의 영역 기억 <br> */

	TALK_FADE	m_iTalkFadeMode;				/**< 스킨 패이드 타입 <br> */

	int			m_iEndHeight;
	int			m_iEndWidth;
	int			m_iMoveSkinHeight;
	int			m_iMoveSkinWidth;	

	int			m_iSkinMoveX;
	int			m_iSkinMoveY;

	int			m_iSkinMoveUnitX;
	int			m_iSkinMoveUnitY;

	float		m_fFadeAlpha;
	float		m_fFadeAlphaUnit;

	float		m_fAccumulateRender;			/**< 렌더링 시간 계산 <br> */
	float		m_fCurDelay;					/**< 프레임 간격 시간 <br> */
	
	SPWindow*	m_pName;						/**< 유저 이름 표시 <br> */
	SPWindow*	m_pMessage;						/**< 작은 메시지 <br> */
	SPWindow*	m_pLMessage;					/**< 큰 메시지 <br> */
	
	SPWindow*	m_pMenu[PC_MENU_MAX];
	SPWindow*	m_pButton[PC_MENU_MAX];			/**< 4개의 선택문 버튼 <br> */
	int			m_iSelectItem;					/**< 4개의 선택문 버튼 중 선택된 번호 <br> */

	bool		m_bFocusLose;					/**< 포커스를 읽은 경우 표현 <br> */
	SPTexture*	m_pFocusLoseSkin;				/**< 포커스를 잃은 경우 추가 스킨 <br> */
	RECT		m_rtFocusLoseSkinSrc;			/**< 포커스를 잃은 경우 추가 스킨 소스 <br> */

	TALK_STATE	m_iTalkState;					/**< 현재 윈도우가 표현하는 대화 상황 <br> */
	SPNpcTalk*	m_pNpcTalk;						/**< NPcTalkData <br> */

	SPSavePoint* m_pSavePoint;					/**< Save Point <br> */

};