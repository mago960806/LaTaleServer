#pragma once 


enum CHECK_STATUS_RESULT {
	CHECK_STATUS_RESULT_FAIL = 0,
	CHECK_STATUS_RESULT_V1 = 1,
	CHECK_STATUS_RESULT_V2 = 2,
	CHECK_STATUS_RESULT_OK = 3,
	CHECK_STATUS_RESULT_MAX = 4,
};


enum REQUIRE_TYPE {
	REQUIRE_TYPE_NULL			= 0,	/**< 비교타입 없음 <br> */
	REQUIRE_TYPE_STAT			= 1,	/**< 비교타입 스탯 <br> */
	REQUIRE_TYPE_ITEM			= 2,	/**< 비교타입 아이템 <br> */	
	REQUIRE_TYPE_SKILL			= 3,	/**< 비교타입 스틸 <br> */
	REQUIRE_TYPE_QUEST			= 4,	/**< 비교타입 퀘스트 <br> */
	REQUIRE_TYPE_TITLE			= 5,	/**< 비교타입 타이틀 <br> */
	REQUIRE_TYPE_MAP			= 6,	/**< 비교타입 맵 <br> */
	REQUIRE_TYPE_EFFECT			= 7,	/**< 비교타입 이팩트 <br> */
	REQUIRE_TYPE_GUILD			= 8,	/**< 비교타입 길드 <br> */
	REQUIRE_TYPE_COREEVENT		= 9,	/**< 비교타입 강제 이벤트<br> */
	REQUIRE_TYPE_EQUIP			= 10,	/**< 비교타입 장비 여부 <br> */
	REQUIRE_TYPE_EQUIP_MOTION	= 11,	/**< 비교타입 장비 MotionID 타입 <br> */
	REQUIRE_TYPE_MAX,					/**< 최대 <br> */
};


enum PLAYER_STATE {
	PLAYER_STATE_NOT			= -1,	/**< 플레이어 객체 없음 <br> */
	PLAYER_STATE_NULL			= 0,	/**< 무엇이든 할수 있음 <br> */
	PLAYER_STATE_DEAD			= 1,	/**< 사망 <br> */
	PLAYER_STATE_TRADE			= 2,	/**< 트레이드 중 <br> */
	PLAYER_STATE_SHOPOPEN		= 3,	/**< 개인상점 개설중 <br> */
	PLAYER_STATE_SHOPIN			= 4,	/**< 개인상점 들어간 경우 <br> */
	PLAYER_STATE_CUTIN			= 5,	/**< 컷인 상황인 경우 <br> */
	PLAYER_STATE_MAX			= 6,
};


enum NOTICE_USE;


/**
* @class SPCheckManager
* @brief PC의 스테이터스, ITEM, SKILL, QUEST등을 체크하기 위한 용도의 메니져
* @author Jinhee
* @version 00.00.01
* @date 2005.9.27
* @bug 없음
* @warning 
*/
class SPCheckManager : public SPManager {
public:
	SPCheckManager();
	virtual ~SPCheckManager();

	virtual void Process	(float fTime);
	virtual	void Render		(float fTime);
	//virtual void RenderTextDebug();
	//virtual void RenderText();

	virtual bool Init();
	virtual void Clean();

	virtual int SPPerformMessage	(UINT msg, WPARAM wparam = 0, LPARAM lParam= 0);	


	/**
	* PC STATUS 를 체크해 가능한지 불가능한지를 반환한다 <br> 
	@param CHECK_STATUS iStatus
	@param INT64 iValue1
	@param INT64 iValue2
	@return bool
	*/
	bool	CheckPCStatus(CHECK_STATUS iStatus, INT64 iValue1, INT64 iValue2);

	/**
	 * 해당 ID의 아이템의 스택수를 체크한다 <br>
	 @param int iItemID
	 @param INT64 iValue1
	 @param INT64 iValue2
	 @return bool
	*/
	bool	CheckPCItem(int iItemID, INT64 iValue1, INT64 iValue2);

	/**
	 * 현재 인벤토리내 해당아이템의 총스택수를 가져온다 <br>
	 @param int iItemID
	 @return int
	*/
	int		GetTotalPCItemCount(int iItemID);

	/**
	 * 해당 ID의 아이템이 들어갈수 있는 공간 여유가 있는지 확인한다 <br>
	 @param int iItemID
	 @param int iStack = 1			아이템의 확인할 스택수
	 @param int iCheckCount = 1		아이템의 확인할 슬롯수
	 @return bool
	*/
	bool	CheckEnableSlot(int iItemID, int iStack = 1, int iCheckCount = 1);

	/**
	 * 
	 @param 
	 @return
	*/
	bool	CheckRequireSlot(int iContainerType, int iRequireCount);
	
	/**
	 * 해당 ID의 스킬을 체크한다 <br>  
	 @param int iSkillID
	 @param INT64 iValue1
	 @param INT64 iValue2
	 @return bool
	*/
	bool	CheckPCSkill(int iSkillID, INT64 iValue1, INT64 iValue2);
	
	/**
	 * 해당 ID의 퀘스트를 체크한다 <br>  
	 * 퀘스트의 반복 조건을 체크한다 <br>
	 @param int iQuestID
	 @param INT64 iValue1
	 @param INT64 iValue2
	 @return bool
	*/
	bool	CheckPCQuest(int iQuestID, INT64 iValue1, INT64 iValue2);

	/**
	 * 주어진 ID의 퀘스트가 진행 중인지 체크한다 <br>
	 * Type이 3인경우 진행중인 퀘스트로 존재하는지를 반환 <br>
	 * Type이 4인경우 3인경우의 반대값으로 반환 <br> 진행중이여야 하지 않는경우 파악 <br>
	 @param int iType
	 @param int iQuestID
	 @return bool  
	*/
	bool	CheckDoingQuest(int iType, int iQuestID);

	/**
	 * 해당 ID의 Title를 체크한다 <br> 
	 @param int iTitleID
	 @param INT64 iValue1	사용안함
	 @param INT64 iValue2   사용안함
	 @return bool (있는경우  true)
	*/
	bool	CheckPCTitle(int iTitleID, INT64 iValue1, INT64 iValue2);

	/**
	* 해당 ID의 Effect를 체크한다 <br> 
	@param int iTitleID
	@param INT64 iValue1	사용안함
	@param INT64 iValue2   사용안함
	@return bool (있는경우  true)
	*/
	bool	CheckPCEffect(int iEffectID, INT64 iValue1, INT64 iValue2);

	/**
	* 길드가 해당 Level인제 체크한다. <br> 
	@param int iGuildLevel
	@param INT64 iValue1	사용안함
	@param INT64 iValue2   사용안함
	@return bool (있는경우  true)
	*/
	bool CheckPCGuildLevel(int iGuildLevel, INT64 iValue1, INT64 iValue2) ;
	bool CheckPCGuildType(int iGuildType, INT64 iValue1, INT64 iValue2) ;

	/**
	 * 주어진 인덱스의 강제 이벤트의 결과를 CheckValue와 비교하여 <br> 
	 * 결과가 동일하면 true 그렇지 않으면 false를 리턴한다 <br>
	 @param int iIndex			확인할 강제 이벤트 인덱스
	 @param bool bCheckValue	비교할 강제 이벤트 실행 여부 
	 @return  bool 동일한지 (CheckValue와 동일한 경우 true)
	*/
	//bool CheckPCCoreEvent(int iIndex, bool bCheckValue);
	
	/**
	* 주어진 인덱스의 강제 이벤트의 결과를 CheckValue와 비교하여 <br> 
	* 결과가 동일하면 true 그렇지 않으면 false를 리턴한다 <br>
	@param int iID			체크할 강제 이벤트 타입 1인경우 저장된것 2인경우 저장되지 않음
	@param int iIndex		확인할 강제 이벤트 인덱스
	@return  bool			
	*/
	bool CheckPCCoreEvent(int iID, int iIndex);

	/**
	* 해당 레어도에 맞는 레어컬러를 얻어온다 <br> 
	@param int iRareLevel
	@return D3DXCOLOR
	*/
	D3DXCOLOR GetItemColor(int iRareLevel);
	
	/**
	 * 주어진 아이템이 현재 플레이어가 사용할수 있는지를 확인한다 <br> 
	 @param  int iItemID		체크할 아이템의 ID
	 @param bool bPvp = false	PVP 상황의 체크여부 (true인경우 Pvp 조건 체크)
	 @return bool				결과 false의 경우 사용할수 없는 
	*/
	bool CheckItemStatus(int iItemID, bool bPvp = false);

	/**
	 * 주어진 아이템이 인벤토리의 한슬롯에서 갯수를 충족하는지 확인한다 <br>
	 @param int iItemID
	 @param int iCheckStack
	 @return bool 조건 충족시 true
	*/
	bool	CheckBiggerSlot(int iItemID, int iCheckStack);

	/**
	* 주어진 아이템이 인벤토리의 순서대로 5개의 슬롯에서 갯수를 충족하는지 확인한다 <br>
	@param int iItemID
	@param int iCheckStack
	@return bool 조건 충족시 true
	*/
	bool	CheckBiggerSlot5(int iItemID, int iCheckStack);

	/**
	 * 주어진 타입에 의거 장비(전투,패션)의 착용 현황을 확인한다 <br>	
	 * SPPlayerInvenArchive::CheckEquipInven 참고 <br> 
	 @param  int iCheckType			1, 3인 경우 전투 2,4인경우 패션
	 @param  int iCheckSlotIndex = 0  슬롯값은 내부에서 0베이스로 조정한다 17인 경우 모두 체크
	 @return bool 조건 충족시 true  
	*/
	bool	CheckEquip(int iCheckType, int iCheckSlotIndex = 0);
	
	/**
	 * 주어진 타입에 의거 장비의 내부 모션 ID값을 확인한다 <br>
	 @param  int iCheckType				1, 3인 경우 전투 2,4인경우 패션
	 @param  int iCheckSlotIndex = 0	슬롯값은 내부에서 0베이스로 조정한다 
	 @param	 int iCheckMotionID = 0		확인할 모션 값
	 @return bool 조건 충족시 true   
	*/
	bool	CheckEquipMotion(int iCheckType, int iCheckSlotIndex = 0, int iCheckMotionID = 0);

	/**
	 * Notice Box의 사용 용도를 확인 <br>
	 @param  NONE
	 @return NOTICE_USE
	*/
	NOTICE_USE GetNoticeUse();

	/**
	* Notice Box의 사용 용도를 설정 <br>
	* 실제 체크하는 맴버 변수는 인벤토리의 맴버이다 <br>
	@param  NOTICE_USE iUse
	@return NOTICE_USE
	*/
	NOTICE_USE SetNoticeUse(NOTICE_USE iUse = (NOTICE_USE)0);

	/**
	 * 로컬 플레이어에게 이펙트를 적용하는 경우 사용 <br> 
	 @param int iEffectID
	 @return bool	 
	*/
	bool SetLocalEffect(int iEffectID);

	/**
	 * DB의 블럭상태를 설정한다 <br> 
	 @param bool bBlock
	 @return NONE
	*/
	void SetDBBlock(bool bBlock);
	
	/**
	 * DB의 블럭 상태를 확인한다 <br>
	 @param NONE
	 @return bool	true의 경우 블럭상태
	*/
	bool IsDBBlock();

	/**
	* 현재 eEquipID의 아이템의 장착 여부를 확인한다 <br>
	@param EQUIP_ID eEquipID
	@param INT64 iValue
	@return bool	true의 경우 해당 eEquipID를 착용하고 있다.
	*/
	bool	CheckEquipItem( int eEquipID , INT64 iValue );

	/**
	* iSkillID의 스킬업 조건을 체크한다 <br>  
	@param int iSkillID
	@return bool	true의 경우 스킬업 가능
	*/
	bool	CheckSkillUpRequire( int iSkillID );

	/**
	* iSkillID의 스킬 발동 조건을 체크한다 <br>  
	@param int iSkillID
	@param bool bDisplayCheck ( 화면 표시용으로 조건 몇개를 건너뛴다 )
	@return bool	true의 경우 스킬 발동 가능
	*/
	bool	CheckSkillUseRequire( int iSkillID , bool bDisplayCheck = false );

	/**
	* iSkillID의 스킬 발동 조건을 체크한다 <br>  
	@param int iSkillID
	@param CONTAINER_TYPE eContainerType
	@param int iSlotIndex
	@param bool bDisplayCheck ( 화면 표시용으로 조건 몇개를 건너뛴다 )
	@return bool	true의 경우 스킬 발동 가능
	*/
	bool	CheckItemSkillUseRequire( int iSkillID , int iItemID , CONTAINER_TYPE eContainerType , int iSlotIndex , bool bDisplayCheck = false );
	
	bool	CheckPetSkillUseRequire( int iSkillID, int iItemID, CONTAINER_TYPE eContainerType, int iSlotIndex, bool bDisplayCheck = false );

	/**
	* iSkillID의 액션 스킬 발동 조건을 체크한다 <br>  
	@param int iSkillID
	@return bool	true의 경우 스킬 발동 가능
	*/
	bool	CheckActionSkillUseRequire( int iSkillID );

	/**
	* iType과 iID로 조건을 체크한다. <br>
	@param int iType ( 1 스탯 , 2 아이템 , 3 스킬 , 4 퀘스트 ,8 길드)
	@param int iID ( 타입이 1일경우 각종 스탯 , 타입 2일경우 1이면 갯수 체크 타입 3일경우 1이면 스킬 존재 체크 , 2이면 스킬 레벨 체크 , 타입 4일 경우 1이면 반복횟수 이하 체크 2이면 반복횟수 초과 체크, 타입 8 길드 레벨업체크 )
	@return bool	true의 경우 조건이 만족한다.
	*/
	bool	CheckCondition( int iType , int iID , int iValue1 , int iValue2 );

	bool	CheckApplyEffectRequire(int iEffectID, SPGameObject* pkGameObject);

	void	SetPetShopOpen(bool bOpen);
	bool	IsPetShopOpen();

	/**
	 * 로컬 플레이어의 플레이중 현재 하고 있는 일에 관한 중요한 체크 정보를 확인한다 <br>
	 @param
	 @return
	*/
	PLAYER_STATE	CheckPlayerState();
	
	/**
	 * 사망패널티 감소 아이템 아이디를 확인 <br>
	 @param
	 @return
	*/
	int		GetDeathItemID();
	
	/**
	 * 사망패널티 감소 아이템이 존재하는지 확인 <br>
	 @param
	 @return
	*/
	bool	CheckDeathItem();

	/**
	 * 서버 외치기 아이템 ID <br>
	 @param
	 @return
	*/
	int		GetMicServerItemID();
	
	/**
	 * 채널 외치기 아이템 ID <br>
	 @param
	 @return
	*/
	int		GetMicChannelItemID();

	int		GetMailFee();
	int		GetMailStampItemID();


	/**
		물품에대한 경매 가능 여부 체크
	 */
	bool	CheckBidEnable(USERID iBidUser) ;				// 본인입찰 체크
	bool	CheckBidEnable(const char* pSellerName);		// 본인물품 체크

	/**
		숫자에 대한 자릿수 체크
	 */
	int		CheckDigitSize(INT64 iNumber) ;

	/**
		캐릭터 이름 비교
	*/
	bool	IsCharNameCompare( const char* pSrcCharName , const char* pDestCharName , bool bLocalName , bool bNoCase );

protected:
	//SPIMESSAGE_MAP_DECLARE()

	/**
	 * 정수형 성별값을 16진수값으로 변환해 받음 <br> 
	 @param int iStatValue
	 @return int
	*/
	int		GetClass(INT64 iStatValue);

protected:
	bool	m_bDBBlock;				/**< DB Working 으로 인한 블럭 플래그 <br> true 인경우 블럭되어 있음 <br> */
	bool	m_bPetShopOpen;			/**< Pet Shop 이 열려 있는지 <br> */
};

extern SPCheckManager*	g_pCheckManager;