// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 오전 10:26:19
//***************************************************************************

#pragma once 

enum PART_LAYER;
enum EQUIP_ID;
enum GOB_STATE_TYPE;
enum GOB_SEQ_INDEX;
enum FXUNIT_DAMAGE_TYPE;
enum PET_PLACE;

struct SPGOBModel;
struct SPTitleData;

class SPItem;
class SPItemCluster;
class SPGameObject;
class SPGOBModelUnit;
class SPPlayerEquipInfo;
class SPPlayerCoordPhysics;
class SPPlayerInvenArchive;
class SPLootingBag;

class SPBattleCluster;
class SPCharacterAttack;
class SPCharacterBeAttacked;

class SPFXArchive;
class SPSoundArchive;
class SPSkill;
class SPSkillActivity;
class SPSkillArchive;
class SPEffectArchive;

class SPEventUnit;
class SPEventArchive;

class SPGOBCoordPhysics;
class SPGOBStatus;
class SPPlayerStatusModel;
class SPTeamArchive;

class SPTitleRenderBase;
class SPPet;

class SPBeAttackedList;
class PhantomAnim;
class RTREffectManager;
class SPPetManager;

struct FIGURE_CHANGE_INFO
{
	FIGURE_ITEM Item;
	bool bOff;
};

struct PET_SKILL_INFO
{
	SPItem *pItem;
	std::vector< t_Pet_Skill_To_Client > vecSkills;
	PET_SKILL_INFO(void)
	{
		pItem = NULL;
		vecSkills.clear();
	}
};

class SPPlayer : public SPGameObject {
	friend class PhantomEffect;
	friend class CAutoInterface;
public:

	SPPlayer();
	virtual ~SPPlayer();

	virtual void Process(float fTime);
	virtual	void Render(float fTime);
	virtual void Init();	
	virtual bool IsLocalPlayer(); 
	virtual void RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer );

	void ProcessImage(float fTime);	
	void SetLocalPlayer(bool bLocalPlayer = true);
	void SetControl(float fTime);	
	void SetFirstAction();

	void TriggerEnable(bool bEnable = true);

	float	GetPosX();				
	float	GetPosY();				
	FACING	GetDirection();				
	BG_ID	GetStandLayer();				
	void	SetPos(float fX, float fY);

	void SetState(GOB_STATE_TYPE state, bool bForce = false);
	bool SetAction(UINT64 uiAction, bool bForce = false);
	void SetActionStatus(unsigned int uiBeforeAniIndex);
	
	int		SendActionPacket();
	int		SendSkillPacket();
	//int		SendEventPacket(SPEventUnit* pEvent);		//[2005/4/21] - jinhee
	void	InitSendAction();

	void	EquipChange_CHAR_EQUIPMENT(CHAR_EQUIPMENT CharEquipment, bool bDefault = false);	

	SPGOBCoordPhysics*		GetCoordPhysics();
	SPGOBStatus*			GetStatus();
	SPPlayerInvenArchive*	GetInvenArchive();
	SPGOBModelUnit*			GetModelUnit();
	SPPlayerEquipInfo*		GetEquipInfo();

	int  GetAniIndex(UINT64 uiAction);			//[xialin 2008/11/11]add

	GOB_SEQ_INDEX	GetCurSeqIndex() { return  m_eCurSeqIndex; }

	bool IsDead();

	// Inventory(Item) - Send
	//int		SendReqLooting(LOOTING_ITEM item);
	//int	SendReqChangeSlot(CHANGE_SLOT item);
	//int		SendReqTrash(TRASH_ITEM item);

	int		ItemSetting(FIGURE_ITEM item, bool bOff=false);
	void	WeaponChange();

	//[2005/4/13] - jinhee Stage Management
	int		GetCurStageID();												/**< 현재 StageID를 가져온다 <br> */
	void	SetCurStageID(int iStageID,  bool bSetStageManager = true);		/**< 현재 StageID를 설정한다 <br> */
	int		GetCurMapGroupID();												/**< 현재 MapGroupID를 가져온다 <br> */
	void	SetCurMapGroupID(int iMapGroupID, bool bSetStageManager = true); /**< 현재 MapGroupID를 설정한다 <br> */
	
	/**
	 * 현재 플레이어의 Stage Index 와 MapGroup Index를 설정한다 <br>
	 * coordPhysics와 StageManager쪽 둘다 설정한다 <br>
	 @param int iStage
	 @param int iMapGroup
	 @return NONE
	*/
	void	SetStage(int iStage, int iMapGroup, bool bSetStageManager = true);	
	void	SetLobbyAvatar(LOBBY_AVATAT  eLobby)		{ m_eLobbyAvatar = eLobby; }
	void	SetShowValue(bool bName, bool bHP, bool bShadow);
	void	SetGOBName(const char* pszName);
	void	SetGOBNameColorType(int iType);
	const char* GetGOBName();
	
	LOBBY_AVATAT	GetLobbyAvatar()	{ return m_eLobbyAvatar; }

	UINT64	GetControl()	{ return m_uiControlAction; }
	void	SetControlAction(UINT64 uiAction)	{ m_uiControlAction = uiAction; }

	void	SetSendAttackIndex(unsigned int uiIndex)	{ m_uiSendAttackIndex = uiIndex; }
	unsigned int	GetSendAttackIndex()	{ return m_uiSendAttackIndex; }

	int		GetWeaponItemSeq()			{ return m_iWeaponItemSeq; }
	int		GetWeaponItemKind()			{ return m_iWeaponitemKind; }
	int		GetEquipItem(EQUIP_ID eID);	
	bool	IsWeaponChangeEnable();
	int		GetActiveWeapon()			{ return m_iActiveWeapon; }
	void	SetWeaponSwap(bool bSwap)	{ m_bWeaponSwapping = bSwap; }
	
	//SPEventArchive* GetEventArchive() { return m_pEventArchive; }
	void SetWeaponSeqItem();

	SPSkillArchive*		GetSkillArchive()	{	return m_pkSkillArchive;	}	//	Skill UI에서 사용할 예정 AJJIYA [7/7/2005]
	SPEffectArchive*	GetEffectArchive()	{	return m_pkEffectArchive;	}	//	Buff UI에서 사용할 예정 AJJIYA  [10/10/2005]

	void ApplyCoolTime( int iID , float fCoolTime , float fAccmulateTime );

	void		SetTradeState(TRADE_STATE iTrade, UINT64 uiID = 0);
	TRADE_STATE GetTradeState();
	bool		CheckPcInterval(GU_ID uiGuid);

	void		SetPetResetPos(bool bReset)		{ m_bPetResetPos = bReset; }

	CONTAINER_SLOT GetItemSkillContainer()		{ return m_ItemSkillContainer; }

	/**
	* 해당 스테이터스별로 맞는 플레이어의 정보값을 반환한다 <br>  
	@param  CHECK_STATUS iStatus
	@return int
	*/
	INT64			GetCheckStatusValue(CHECK_STATUS iStatus);			//[2005/9/26]

	/**
	 * 전체 루팅시 해당 프로세스를 돌면서 하나씩 아이템을 루팅한다 <br>
	 @param float fTime
	 @return int
	*/
	int			ProcessLooting(float fTime);
	int			ProcessPetLooting(float fTime);
	int			ProcessPetItemUsed(float fTime);
	int			ProcessPetChat(float fTime);

	/**
	 * 60초에 한번씩 아이템 지속시간을 감소하도록 하는 메시지를 날리는 역할을 한다 <br> 
	 * LocalPlyaer에게만 해당 <br>
	 * 실제 아이템 지속시간 감소는 인벤아카이브에서 담당 <br> 
	 @param  float fTime
	 @return int
	*/
	int			ProcessItemDuration(float fTime);

	// AJJIYA [8/25/2005]
	void		SetTalkBalloon( const char* pszString , BROADCAST_SCOPE eScope , D3DXCOLOR color );	//	말풍선

	// AJJIYA [3/31/2006]
	void		SetTitle( SPTitleData& stTitleData );
	SPTitleRenderBase*	GetTitle();

	/**
	* 플레이어가 죽었을때 행동 <br>
	* 포탈을 어떻게 탈것인지 결정할 메시지 박스를 뛰운다 <br>
	@param 
	@return
	*/
	void SetDeadAction();


	// AJJIYA [11/9/2005]
	UINT64		GetCurAction();

	bool IsMaster();
	bool GetTeamActive();
	SPTeamArchive* GetTeam();

	void SetEffectActivity(LPARAM lParam, bool bTargetFX = true);

	void InitFigureInfo();

	/**
	* 자신이 타인의 개인상점에 들어가기 요청 <br>
	@param 
	@return
	*/
	bool SendUserShopIn(GU_ID iGuid);

	/**
	 * 자신이 타인의 개인상점에 나가기 요청 <br>  
	 @param 
	 @return
	*/
	bool SendUserShopOut();

	/**
	 * 자신이 들어간 타인의 개인상점 GUID 를 확인 <br>  
	 @param 
	 @return
	*/
	GU_ID GetUserShopID();

	/**
	 * 개인상점 인 경우 상점명을 기억해놓는다 <br> 
	 @param 
	 @return
	*/
	void SetUserShopName(std::string strShopName);
	
	/**
	 * 개인상점 인 경우 상점명을 확인한다 <br>
	 @param 
	 @return
	*/
	std::string GetUserShopName();

	/**
	 * 개인상점 타입을 설정한다 <br> 
	 @param 
	 @return
	*/
	void SetUserShopType(int iType);
	
	/**
	 * 개인상점 타입을 확인한다. <br> 
	 @param 
	 @return
	*/
	int GetUserShopType();

	/**
	 * 내가 남의 개인상점에서 나간다 <br> 
	 @param 
	 @return  
	*/
	void SetUserShopOut();

	bool	SendPetShopIn(GU_ID iGuid );
	bool	SendPetShopOut( void );
	GU_ID	GetPetShopID( void );
	void	SetPetShopName(std::string strShopName);
	std::string GetPetShopName( void );
	void	SetPetShopType( int iType );
	int		GetPetShopType( void );	
	void	SetPetShopOut( void );

	void RefreshEquipItem();

	// Friend List Inteface [1/26/2007 AJJIYA]
	bool AddFriendList( FRIEND_INFO* pFriendInfo);
	bool DelFriendList( const char* pszName );
	bool DelFriendListAll();

	int				GetFriendListCount();
	FRIEND_INFO*	GetFriendListMember( int iPos );
	FRIEND_INFO*	GetFriendListMember( const char* pszName );

	// Hp Gage View Interface [2/26/2007 AJJIYA]
	void	SetLocalPlayerTeam( bool bLocalPlayerTeam );
	bool	GetLocalPlayerTeam();

	// Skill Init [7/21/2008 AJJIYA]
	void	SetSkillInitData( UINT32 uiSaveCheck , int iSkillPoint , bool bUIRefresh );

	/**
	 * 팀찾기 렌더에 관련한 설정을 한다 <br>
	 @param bool 보여줄것인지
	 @param int 클래스
	 @param int 레벨
	 @return bool
	*/
	bool SetWantTeam(bool bShow, int iClass, int iLevel);

	bool IsTeamMember(GU_ID iID);

	/**
	* 피격 리스트를 가져온다 <br>
	* Local Player 한정 생성, 동작<br>
	@param 
	@return  
	*/
	SPBeAttackedList* GetBeAttackedList() { return m_pBeAttackedList; }

	/**
	 * 피격 리스트를 초기화 한다 <br>
	 @param 
	 @return  
	*/
	void ClearBeAttackedList();
	
	/**
	 * 피격 리스트의 해당 아이디를 삭제한다 <br>
	 @param 
	 @return  
	*/
	void DeleteBeAttackedObject(GU_ID iId);
	
	/**
	 * 퀵슬롯의 크기를 설정한다 <br> 
	 * 기본값 1 <br> 
	 @param 
	 @return  
	*/
	void SetQuickSlotSize(int iSize);

	/**
	 * 크리티컬 타격 효과 표현용 <br>
	 * 테스트 버전 <br> 
	 @param 
	 @return  
	*/
	void SetCriticalEffect(bool bCritical = true);

	/**
	 *
	 @param 
	 @return  
	*/
	void SetPlayerClass(unsigned char iClass);
	
	/**
	 *
	 @param 
	 @return  
	*/
	unsigned char GetPlayerClass();

	/**
	 *
	 @param 
	 @return  
	*/
	SPPet*	GetPlayerPet();

	void SetActionSound(unsigned int uiKeyValue, bool bSkill = false, bool bInsert = true);

	//////////////////////////////////////////////////////////////////////////	
	virtual int  PerformMouseMove		( int iX, int iY);
	virtual int  PerformMouseLDown		( int iX, int iY);
	virtual int  PerformMouseLUp		( int iX, int iY);
	virtual int  PerformMouseRDown		( int iX, int iY);
	virtual int  PerformMouseRUp		( int iX, int iY);
	virtual bool IsCursorIn				( int iX, int iY);

protected:

	SPGMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnGetPosX		( LPARAM lParam );
	virtual int OnGetPosY		( LPARAM lParam );
	virtual int OnGetGOBName	( LPARAM lParam );
	virtual int OnGetModelUnit	( LPARAM lParam );
	virtual int OnGetCoordPhysics( LPARAM lParam );
	virtual int OnGetEffectArchive( LPARAM lParam );
	virtual int OnGetFXArchive	( LPARAM lParam );
	virtual int OnGetSoundArchive( LPARAM lParam );
	virtual int OnGetGOBStatus( LPARAM lParam );
	virtual int OnGetWeaponVisible( LPARAM lParam );

	virtual int OnGetModel		( LPARAM lParam );
	virtual int OnGetFacing		( LPARAM lParam );
	virtual int OnGetStandlayer	( LPARAM lParam );
	virtual int OnGetVelosity	( LPARAM lParam );
	virtual int OnGetCurAniIndex( LPARAM lParam );
	virtual int OnGetCurAniFrame( LPARAM lParam );
	virtual int OnIsDead( LPARAM lParam );
	virtual int OnGetType(LPARAM lParam);
	virtual int OnGetAction(LPARAM lParam);
	
	virtual int OnSetLPPosotion	( LPARAM lParam );	
	virtual int OnSetDamageFX	( LPARAM lParam );
	virtual int OnSetHealingFX	( LPARAM lParam );
	virtual int OnLevelUpFX		( LPARAM lParam );
	virtual int OnFXGroup		( LPARAM lParam );
	virtual int OnFXGroupClear	( LPARAM lParam );
	virtual int OnSetFacing		( LPARAM lParam );
	virtual int OnSetKnockBack	( LPARAM lParam );

	virtual int OnSetMoveCommand		( LPARAM lParam );
	virtual int OnSetAction(LPARAM lParam);
	virtual int OnSetStatusRender(LPARAM lParam);
	virtual int OnSetGOBSeqIndex(LPARAM lParam);
	virtual int OnSetIgnoreStage(LPARAM lParam);

	virtual int OnSetEquipDefaultChange	( LPARAM lParam );
	virtual int OnSetEquipChange		( LPARAM lParam );
	virtual int OnSetEquipUnload		( LPARAM lParam );

	virtual int OnSetEquipInit_Broadcast( LPARAM lParam );
	virtual int OnSetEquipOn_Broadcast( LPARAM lParam );
	virtual int OnSetEquipOff_Broadcast( LPARAM lParam );
	virtual int OnSetWeaponChange( LPARAM lParam);
	virtual int OnSetContainerInfos(LPARAM lParam);

	virtual int OnItemMessage	( SPGM msg, /*WPARAM wParam,*/ LPARAM lParam );
	virtual int	OnStatMessage	( SPGM msg, LPARAM lParam );

	virtual int OnLootingItemGainReq(LPARAM lParam);
	virtual int OnLootingItemAllGainReq(LPARAM lParam);
	virtual int OnLootingItemGain(LPARAM lParam);
	virtual int OnLootingExit(LPARAM lParam);
	virtual int OnLootingDelete(LPARAM lParam);

	virtual int OnPetLootingItemGainReq(LPARAM lParam);

	virtual int OnSetSkillInit(LPARAM lParam);		// AJJIYA [7/28/2005]
	virtual int OnSetSkillInitSelective(LPARAM lParam);
	virtual int OnSetSkillList(LPARAM lParam);
	virtual int OnSetSkillUsing(LPARAM lParam);
	virtual int OnSetItemSkillUsing(LPARAM lParam);
	virtual int OnSetLoveSkillUsing(LPARAM lParam);
	virtual int OnSetEffectActivity(LPARAM lParam);
	virtual int OnSetEffectDelete(LPARAM lParam);
	
	virtual int OnSetActionCommand(LPARAM lParam);
	virtual int OnSetActionCompleteCommand(LPARAM lParam);

	//virtual int OnEventMessage			( LPARAM lParam ); //  [2005/4/25]
	virtual int OnYes(LPARAM lParam);
	virtual int OnNo(LPARAM lParam);

	virtual int OnSetContainerInfo(LPARAM lParam);	//[2005/8/26] - jinhee //플레이어의 인벤토리 슬롯 확장

	virtual int OnEffectFXSound(LPARAM lParam);
	virtual int OnCasterEffectFXSound(LPARAM lParam);
	virtual int OnTargetEffectFXSound(LPARAM lParam);
	virtual int OnPlaySound(LPARAM lParam);
	virtual int OnEffectAllClear(LPARAM lParam);

	virtual int OnSetTransparency(LPARAM lParam);
	virtual int OnDelTransparency(LPARAM lParam);
	
	virtual int OnIsPlayer(LPARAM lParam);

	virtual int OnTeamInviteRequest(LPARAM lParam);
	virtual int OnTeamInviteReceive(LPARAM lParam);
	virtual int OnTeamJoinReceive(LPARAM lParam);
	virtual int OnTeamSecedeRequest(LPARAM lParam);
	virtual int OnTeamSecedeReceive(LPARAM lParam);
	virtual int OnTeamExpelReceive(LPARAM lParam);
	virtual int OnTeamChangeMasterRequest(LPARAM lParam);
	virtual int OnTeamChangeMasterReceive(LPARAM lParam);
	virtual int OnTeamChangeLootingRequest(LPARAM lParam);
	virtual int OnTeamChangeLootingReceive(LPARAM lParam);
	virtual int OnTeamUpdateMemberReceive(LPARAM lParam);
	virtual int OnTeamChannelMove(LPARAM lParam);

	virtual int OnGuildInviteReceive(LPARAM lParam);
	virtual int OnGuildSecedeMe(LPARAM lParam);
	//virtual int OnGuildJoinReceive(LPARAM lParam);
	//virtual int OnGuildSecedeReceive(LPARAM lParam);
	//virtual int OnGuildExpelReceive(LPARAM lParam);

	virtual int OnTitleChange( LPARAM lParam );
	virtual int OnSetCoolTime( LPARAM lParam );

	virtual int OnItemEnchantReceive(LPARAM lParam);
	virtual int OnItemUpgradeReceive(LPARAM lParam);
	virtual int OnItemEffectUpgradeReceive(LPARAM lParam);
	
	virtual int OnUserShopOpen	(LPARAM lParam);
	virtual int OnUserShopClose	(LPARAM lParam);
	virtual int OnUserShopIn	(LPARAM lParam);
	virtual int OnUserShopOut	(LPARAM lParam);
	virtual int OnUserShopBuy	(LPARAM lParam);

	virtual int OnPetShopOpen	(LPARAM lParam);
	virtual int OnPetShopClose	(LPARAM lParam);
	virtual int OnPetShopIn		(LPARAM lParam);
	virtual int OnPetShopOut	(LPARAM lParam);
	virtual int OnPetShopBuy	(LPARAM lParam);
	virtual int OnPetShopItemUpdate (LPARAM lParam );

	virtual int OnSetPet(LPARAM lParam);
	virtual int OnPetSkillInsert( LPARAM lParam );
	virtual int OnPetSkillDelete( LPARAM lParam );

	virtual int OnItemSkillInsert(LPARAM lParam);
	virtual int OnItemSkillDelete(LPARAM lParam);

	virtual int OnItemRtrEffectInsert(LPARAM lParam);
	virtual int OnItemRtrEffectDelete(LPARAM lParam);

	virtual int OnSetGuildInfo(LPARAM lParam);

	void AttackBoxRender();
	void ParsingControlAction();
	bool CheckStatus(UINT64& uiAction);
	bool CheckSkillStatus(UINT64& uiAction);

	void SetAnimationLoop();
	bool CheckLootingBag();

	bool CheckPetLootingBag();
	CONTAINER_ITEM* GetCanLootingItem(SPLootingBag* pkBag);
	
	bool IsSkillActivity(unsigned int uiSkillID);

	void InsertTeamMember(TEAM_MEMBER& tMember);
	void DeleteTeamMember(GU_ID iID);
	void DeleteTeamMember( INT32 iCharDBKey );
	void ClearTeamMember();
	void UpdateTeamStatus();
	
	bool CheckRun(float fElapsedTime, int iDirection);

	struct RUN_CONTROL
	{
		float fKeyTermTime;
		float fKeyTermAccmulateTime;
		float fKeyPushAccmulateTime;
		
		int iDirection;
	};

	void ChangeFigureItem();	
	
	/**
	 * 되살아나기 요청을 한다 <br> 
	 * 어떤 포탈을 탈것인지를 서버에 알린다 <br>
	 @param 
	 @return
	*/
	void SelectRevivalPortal(SAVEPOINT_TYPE iType);	

	/**
	* 되살아나기시 데스 패널티를 줄여주는 경우<br>
	@param SAVEPOINT_TYPE 부활 포탈 타입
	@param SPItem*	패널티를 줄여줄수 있는 아이템을 사용하는 경우 
	@return bool
	*/
	bool SendRevivalPortal(SAVEPOINT_TYPE iType, SPItem* pItem = NULL);

	void InsertDamageFX( FXUNIT_DAMAGE_TYPE eDamageType , int iDamage , int iMode , int iType );
	void InsertHitComboFX( UINT8 uiHitComboCount , SPFXArchive* pLocalFXArchive );
	
	SPPet *SetPet(unsigned int uiClassID, PET_PLACE place = PET_PRIMARY, int iItemID = 0);
	void OnSetMultiPet( int itemID, PET_PLACE place ); 
	void ReleasePet( PET_PLACE place = PET_PRIMARY );
	void UpdatePetSpecialSkillToggle();
	
	/**
	 * 파티 찾기 렌더 영역을 설정 <br>
	 @param int 플레이어 중점 X
	 @param int 플레이어 중점 Y
	 @return
	*/
	void SetPartyFindRenderDest(int iCenterX, int iCenterY);
	
	/**
	 * 파티 찾기 렌더 <br>
	 @param
	 @return
	*/
	void RenderPartyFind(float fTime);	
	
	/**
	 * 크리티컬 타격 효과 표현용 <br>
	 * 테스트 버전 <br> 
	 @param 
	 @return  
	*/
	void ProcessCriticalEffect(float fTime);
	
	/**
	* 크리티컬 타격 효과 표현용 <br>
	* 테스트 버전 <br> 
	 @param 
	 @return  
	*/
	void RenderCriticalEffect(float fTime);
	
	//--------------------------------------------------
	// new pet method
	int SetPetInfo( LPARAM lParam );
	int UpdatePetInfo( LPARAM lParam );
	int	TakeOffPet( LPARAM lParam );
	int OnPetAddSSP( LPARAM lParam );
	int OnPetChangeName( LPARAM lParam );
	int OnPetShout( LPARAM lParam );
	int OnPetLearnSkill( LPARAM lParam );
	int OnPetShopForceClosed( LPARAM lParam );
	//--------------------------------------------------

protected:
	bool				m_bLocalPlayer;	
	bool				m_bControlEnable;	
	bool				m_bStatusRender;
	LOBBY_AVATAT		m_eLobbyAvatar;

	// State
	GOB_STATE_TYPE m_eCurState;
	GOB_STATE_TYPE m_eLastState;
	GOB_SEQ_INDEX m_eCurSeqIndex;
	GOB_SEQ_INDEX m_eAttackRandIndex;
	unsigned int m_uiSendAttackIndex;
	
	// Action
	UINT64 m_uiCurAction;
	UINT64 m_uiLastAction;
	UINT64 m_uiControlAction;
	INT64 m_iSendAction;
	float m_fSendAccmulateTime;

	float				m_fWalkStep;	

	float				m_fCamX;
	float				m_fCamY;

	bool m_bBuckler;
	int m_iWeaponItemSeq;
	int m_iWeaponitemKind;
	int m_iActiveWeapon;

	int m_iRequestSkillID;
	int m_iSendSkillID;
	int m_iBeforeSkillID;
	SPSkillActivity* m_pkRevSkillActivity;
	SPSkill* m_pkRevSkill;
	CONTAINER_SLOT m_ItemSkillContainer;

	bool m_bActiveSkill;
	float m_fSkillEffectDelay;
	float m_fSkillAccmulateTime;

	bool m_bFacingSend;
	bool m_bWeaponSwapping;

	RUN_CONTROL m_RunControl;
	

	// Render Model
	SPGOBModelUnit*			m_pGOBModelUnit;	
	SPPlayerEquipInfo*		m_pPlayerEquipInfo;
	SPGOBCoordPhysics*		m_pkCoordPhysics;
	SPGOBStatus*			m_pPlayerStatus;	
	SPPlayerInvenArchive*	m_pPlayerInventory;
	SPFXArchive*			m_pFXArchive;
	SPPlayerStatusModel*	m_pPlayerStatusModel;
	//SPEventArchive*		m_pEventArchive;		//[2005/4/21] - jinhee
	//bool					m_bSendEventPacket;		//[2005/4/21] - jinhee
	SPLootingBag*			m_pkCurLootingBag;
	SPLootingBag*			m_pkPetCurLootingBag;
	SPSoundArchive*			m_pSoundArchive;
	SPSkillArchive*			m_pkSkillArchive;
	SPEffectArchive*		m_pkEffectArchive;
	SPTeamArchive*			m_pkTeamArchive;
	//SPPet*					m_pkPet;
	RTREffectManager		*m_pkRTREffectMgr;
	SPPetManager			*m_pPetMgr;

	int						m_nBattleEquipInfo[EQ_END];
	int						m_nFashionEquipInfo[EQ_END];
	int						m_nBasicEquipInfo[EQ_END];
	int						m_nEffectEquipInfo[EQ_END];
	
	RECT					m_RectPosition;
	RECT					m_RectName;

	TRADE_STATE				m_iTrade;				//[2005/7/29] - jinhee //현재 유저의 (유저간)트레이드 상태를 지님
	GU_ID					m_uiCheckIntervalPlayer;	//[2005/7/29] - 거래시 간격 체크용 유저의 포인터
	GU_ID					m_uiUserShopPlayer;		//[2006/5/22] - jinhee //내가 연 개인상점 GU_ID
	int						m_iUserShopType;		//[2006/6/5] - jinhee //유저별 상점 클래스 

	bool					m_bLootingAll;			//[2005/11/3] - jinhee //루팅백내의 모든 아이템을 루팅하는 경우 ture
	int						m_iLootingIndex;		//[2005/11/3] - jinhee //LooingAll 이 ture이면 이 수의 순서대로 하나씩 루팅을 하도록 한다 <br> 
	float					m_fAccumulateLooting;	//[2005/11/4] - 루팅 프로세스용
	float					m_fCurLootingDelay;		//[2005/11/4] - 루팅 프로세스용

	float					m_fAccmulatePetCheckLooting;
	float					m_fAccumulatePetLooting;
	float					m_fPetLootingDelay;

	float					m_fAccumulatePetItemUsed;
	float					m_fPetItemUsedDelay;

	float					m_fAccumulatePetChat;

	float					m_fAccumulateItem;		//[2006/6/9] - 아이템 지속시간 용
	float					m_fCurItemDelay;		//[2006/6/9] - 아이템 지속시간 용

	SAVEPOINT_TYPE			m_iSavePoint;			//[2007/1/23] - 부활시 선택한 포탈 정보

	bool					m_bPetResetPos;

	SPTexture*				m_pPartyFindTexture;	//[2007/2/27] 파티 찾기 바탕용 이미지
	SPTexture*				m_pClassTexture;		//[2007/2/26] 클레스 이미지

	RECT					m_rtDestPartyFindTail;
	RECT					m_rtDestPartyFindTop;
	RECT					m_rtDestPartyFindCenter;
	RECT					m_rtDestPartyFindBottom;
	RECT					m_rtDestPartyFindLvImg;
	RECT					m_rtDestPartyFindLv;
	RECT					m_rtDestPartyFindClass;

	RECT					m_rtSrcPartyFindTail;	
	RECT					m_rtSrcPartyFindTop;
	RECT					m_rtSrcPartyFindCenter;
	RECT					m_rtSrcPartyFindBottom;
	RECT					m_rtSrcPartyFindLvImg;

	RECT					m_rtSrcClass[CLASS_MAX];//[2007/2/27] 클래스

	bool					m_bRenderPartyFind;		//[2007/2/27] 파티찾기 렌더링 플래그
	unsigned short			m_iClass;				//[2007/2/27] 플레이어 클래스
	char					m_szLevel[4];			//[2007/2/27] 플레이어 레벨

	// set figure
	float m_fFigureAccmulateTime;
	float m_fFigureDelayTime;
	bool m_bActiveWeaponChange;
	std::vector< FIGURE_CHANGE_INFO > m_vFigureItemList;

	//	Friend List
	std::map< std::string , FRIEND_INFO* >	m_mpFriendList;	// 친구 리스트 [1/26/2007 AJJIYA]

	SPBeAttackedList*		m_pBeAttackedList;				/**< 피격 리스트 <br> */

	bool					m_bCriticalEffect;				/**< 크리티컬 타격 효과 <br> 테스트 버전 <br> */
	float					m_fTotalCriticalAccumulate;		/**< 전체 효과 <br> */
	float					m_fTotalCriticalDelay;			/**< 전체 효과 <br> */
	float					m_fCriticalAccumulate;			/**< 효과중 전환 <br> */
	float					m_fCriticalDelay;				/**< 효과중 전환 <br> */
	bool					m_bCamMoveLeft;					/**< 이동 방향 <br> */
	float					m_fLastCamX;					/**< 종료시 복원할 위치 <br> */
	float					m_fLastCamY;					/**< 종료시 복원할 위치 <br> */

	float					m_fRemainProccessTime;
public:
#if defined(_LAYER_THREAD_LOAD)
	int m_iEquipInitNum;
	FIGURE_ITEM m_FigureItem[64];

	bool m_bUseThread[4];
	bool AllUnUsedThread();
#endif
	bool				m_bIsGM;
public:
	bool getIsPlayerGM( void ) const
	{
		return m_bIsGM;
	}

protected:
	PhantomAnim			*m_ptrPhantomAnim;
	D3DXCOLOR			m_colorPhantom;
public:
	D3DXCOLOR getPhantomColor( void ) const;
	bool setPhantomColor( D3DXCOLOR color );
	RTREffectManager *getEffecgMgr( void )
	{
		return m_pkRTREffectMgr;
	}
	SPPlayerStatusModel	*GetStatusModel( void ) { return m_pPlayerStatusModel; }
	SPPetManager	*GetPetMgr( void ) { return m_pPetMgr; }
public:
	bool m_bTransformMode;

//--------------------------------------------------
public:
	void ChangeAvatarOffset( int offsetX, int offsetY )
	{
		m_iAvatarOffsetX += offsetX;
		m_iAvatarOffsetY += offsetY;
	}
	void SetAvatarOffset( int offsetX, int offsetY )
	{
		m_iAvatarOffsetX = offsetX;
		m_iAvatarOffsetY = offsetY;
	}
	void SetWeaponVisible( bool bVisible )
	{
		m_bWeaponVisible = bVisible;
	}
	bool m_bTumbleCloudMode;
protected:
	int		m_iAvatarOffsetX;
	int		m_iAvatarOffsetY;
	bool	m_bWeaponVisible;
//--------------------------------------------------


protected:
	GU_ID	m_uiPetShopPlayer;
	int		m_iPetShopType;
};

extern bool g_bWarningBugleChannel;	// 쏵흙윙걷팁돛
extern hash_map<int,int> g_bWarningBugleCoinBegin;	// 역迦듐얩윙걷
extern bool g_bWarningBugleLastOne;	// 랙供離빈寧몸윙걷
extern bool g_bNeedBugleHighlight;	// 角뤠矜狼쏵契윙걷틉돛句롤多돔
extern vector<int> g_vecNotifiers;				// 綠쒔多돔법돨鯤소죗깊
extern bool g_bPushPropInited;
bool loadBugleNotifiers( void );
bool isPlayerNotified( const char *username );
bool addNotifier( const char *username );
void resetPetSkillStatus( void );
