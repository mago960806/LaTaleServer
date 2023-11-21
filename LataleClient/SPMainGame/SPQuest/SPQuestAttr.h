#pragma once


///**
// * 스탯 비교시 사용되는 비교 연산자 <br>
//*/
//enum STAT_OPERATOR {
//	STAT_OPERATOR_NULL = 0,			/**< 사용안함 <br> */
//	STAT_OPERATOR_SMALL = 1,		/**< 작음 <br> */
//	STAT_OPERATOR_LARGE = 2,		/**< 큼 <br> */
//	STAT_OPERATOR_LESS = 3,			/**< 이하 <br> */
//	STAT_OPERATOR_MORE = 4,			/**< 이상 <br> */
//	STAT_OPERATOR_EQUAL = 5,		/**< 같은 <br> */
//	STAT_OPERATOR_WRONG = 6,		/**< 다른 <br> */
//};

/**
 * Quest를 UI에 보여줄지를 결정하는 플래그 <br>
*/
enum QUEST_UI_SHOW {
	QUEST_UI_SHOW_NULL = 0,				/**< 숨김 <br> */
	QUEST_UI_SHOW_DISABLE = 1,			/**< 비활성화 <br> */
	QUEST_UI_SHOW_ABLE = 2,				/**< 정상 <br> 생성시 기본값 <br> */
	QUEST_UI_SHOW_COMPLETE = 3,			/**< 모든 미션을 만족한 경우 <br> 완료 <br> */
	QUEST_UI_SHOW_REWORD = 4,			/**< 보상 받은 경우<br> 완료 리스트의 경우 모두 이값임 <br> NPC의 퀘스트 리스트에도 이상태인 것이 있을수 있음 <br> */
};


/**
 * 퀘스트 NPC <br> 
*/
enum QUEST_NPC {
	QUEST_NPC_REWORD	= 0,		/**< 보상을 주는 NPC <br> */
	QUEST_NPC_GIVE		= 1,		/**< 퀘스트를 주는 NPC <br> */
	QUEST_NPC_MAX		= 2,		/**< 퀘스트 NPC 최대 <br> */
};


/**
 * 퀘스트 보상 랭크 <br>
*/
enum REWARD_RANK {
	REWARD_RANK_0		= 0,		/**< 보상랭크 1등급 <br> */
	REWARD_RANK_1		= 1,		/**< 보상랭크 2등급 <br> */
	REWARD_RANK_2		= 2,		/**< 보상랭크 3등급 <br> */
	REWARD_RANK_NULL	= 3,		/**< 보상 실패 <br> */
};


/**
 * 퀘스트 지역별 소트용 <br>
 * 엘리아스(Elias)아오이치(Aoichi)용경(龍京-RonJing)엘파(Elfa) <br> 
*/
enum CITY_NAME {
	CITY_NAME_NULL		= 0,		/**< 사용안함 <br> */
	CITY_NAME_BELOS		= 1,		/**< 벨로스 <br> */
	CITY_NAME_ELIAS		= 2,		/**< 엘리아스 <br> */
	CITY_NAME_AOICHI	= 3,		/**< 아이이치 <br> */
	CITY_NAME_RONJING	= 4,		/**< 용경 <br> */
	CITY_NAME_ELFA		= 5,		/**< 엘파 <br> */
	CITY_NAME_COUNT		= 6,		// 도시 종류 갯수 AJJIYA [6/23/2006]
};

class SPRewardAttr;

/**
 * 퀘스트 습득 요구 조건 <br>
*/
struct SPRequire {
	int m_iRequireStat;				/**< 습득에 필요한 스탯 종류 <br> */
	int m_iRequireStatValue;		/**< 습득에 필요한 스탯의 값 <br> */
	int m_iValue1;					/**< 비교값 1 <br> */
	int m_iValue2;					/**< 비교값 2 <br> */	

	bool m_bOK;						/**< 체크가 성공했는지 <br> */
	
	SPRequire() {
		Clear();
	}

	~SPRequire() {
		Clear();
	}

	void Clear(){
		m_iRequireStat = 0;
		m_iRequireStatValue = 0;
		m_iValue1 = 0;
		m_iValue2 = 0;
		m_bOK = true;
	}

	void operator=(const SPRequire Src) {
		m_iRequireStat = Src.m_iRequireStat;
		m_iRequireStatValue = Src.m_iRequireStatValue;
		m_iValue1 = Src.m_iValue1;
		m_iValue2 = Src.m_iValue2;
		m_bOK = Src.m_bOK;
	}

	void SetRequire(int iStat, int iStatValue, int iValue1, int iValue2) {
		Clear();
		m_iRequireStat = iStat;
		m_iRequireStatValue = iStatValue;
		m_iValue1 = iValue1;
		m_iValue2 = iValue2;
	}
};


/**
 * 퀘스트에 사용되는 아이템 정보 <br> 
*/
struct SPQuestItem {
	int m_iItemID;					/**< 아이템 ID <br> */
	int m_iStack;					/**< 갯수 <br> */
	int m_iRear;					/**< 레어도 <br> */

	SPQuestItem(){
		Clear();
	}
	
	~SPQuestItem(){
		Clear();
	}

	void Clear() {
		m_iItemID = 0;
		m_iStack = 0;
		m_iRear = 0;
	}

	void operator=(const SPQuestItem Src) {
		m_iItemID = Src.m_iItemID;
		m_iStack = Src.m_iStack;
		m_iRear = Src.m_iRear;
	}

	void SetQuestItem(int iItemID, int iStack, int iRear) {
		Clear();
		m_iItemID = iItemID;
		m_iStack = iStack;
		m_iRear = iRear;
	}
};


/**
 * 퀘스트 수행 임무 <br>
*/
struct SPMission {
	MISSION_TYPE m_iMissionType;			/**< 임무 종류 <br> */
	int m_iMissionTypeID;					/**< 임무에 대한 ID <br> 임무 종류가 아이템이면 아이템 ID 가 됨 <br> */
	int m_iMissionCount;					/**< 충족시켜야 할 횟수 <br> */
	bool m_bShow;							/**< 미션 수행목적을 보여줄것인지 <br> */

	bool m_bClear;							/**< 플레이어만 사용 <br> 임무가 클리어 되어 있는지 <br> */
	int m_iCurCount;						/**< 현재 플레이어가 충족시킨 횟수 <br> */
	
	SPMission(){
		Clear();
	}
	
	~SPMission(){
		Clear();
	}
	
	void Clear() {
		m_iMissionType = MISSION_TYPE_NULL;
		m_iMissionTypeID = 0;
		m_iMissionCount = 0;
		m_bShow = true;
		m_bClear = false;
		m_iCurCount = 0;
	}

	void operator=(const SPMission Src) {
		m_iMissionType = Src.m_iMissionType;
		m_iMissionTypeID = Src.m_iMissionTypeID;
		m_iMissionCount = Src.m_iMissionCount;
		m_bShow = Src.m_bShow;
		m_bClear = Src.m_bClear;
		m_iCurCount = Src.m_iCurCount;
	}

	void SetMission(MISSION_TYPE iMission, int iTypeID, int iCount, bool bShow) {
		Clear();
		m_iMissionType = iMission;
		m_iMissionTypeID = iTypeID;
		m_bShow = bShow;
		if(m_iMissionType == MISSION_TYPE_ACTION) {
			m_iMissionTypeID++;
		}		
		m_iMissionCount = iCount;
	}
};


/**
 * 퀘스트 완료(보상)시 소모되는 아이템 <br>
*/
struct SPConsumeItem {
	int m_iItemID;							/**< 아이템 ID <br> */
	int m_iStack;							/**< 갯수 <br> */

	SPConsumeItem(){
		Clear();
	}

	~SPConsumeItem(){
		Clear();
	}

	void Clear() {
		m_iItemID = 0;
		m_iStack = 0;		
	}

	void operator=(const SPConsumeItem Src) {
		m_iItemID = Src.m_iItemID;
		m_iStack = Src.m_iStack;
	}

	void SetConsumeItem(int iItemID, int iStack) {
		Clear();
		m_iItemID = iItemID;
		m_iStack = iStack;
	}
};


/**
 * Quest에 사용되는 NPC(퀘스트를 준 NPC, 보상을 주는 NPC)의 정보를 보관 <br> 
*/
struct SPQuestNpc{
	int m_iIcon;							/**< 아이콘 <br> */
	int m_iIconIndex;						/**< 아이콘 인덱스 <br> */
	std::string m_strPlace;					/**< NPC 장소 문자열 <br> */
	std::string m_strName;					/**< NPC 이름 문자열 <br> */

	SPQuestNpc(){
	}

	~SPQuestNpc(){
	}

	void Clear() {
		m_iIcon = 0;
		m_iIconIndex = 0;
		m_strPlace = "";
		m_strPlace.clear();
		m_strName = "";
		m_strName.clear();
	}

	void operator=(const SPQuestNpc Src) {
		Clear();
		m_iIcon = Src.m_iIcon;
		m_iIconIndex = Src.m_iIconIndex;
		m_strPlace = Src.m_strPlace;
		m_strName = Src.m_strName;
	}

	void SetQuestNpc(int iIcon, int iIconIndex, std::string strPlace, std::string strName)
	{
		m_iIcon = iIcon;
		m_iIconIndex = iIconIndex;
		m_strPlace = strPlace;
		m_strName = strName;
	}

};


/**
 * 완료시 해당 랭크에서 선택가능한 보상 정보 <br> 
 * 랜덤 보상인 경우 Probability 에 의해 결정 <br>
 * ID에 의해서 보상 정보 참조 <br>
*/
struct SPRewordInfo {
	int m_iID;						/**< 랭크 ID <br> */
	int m_iProbability;				/**< 랭크 확률? <br> */
	SPRewardAttr* m_pRewardInfo;

	SPRewordInfo() {
	}

	~SPRewordInfo() {
	}

	void Clear() {
		m_iID = 0;
		m_iProbability = 0;
		m_pRewardInfo = NULL;
	}

	void operator=(const SPRewordInfo Src) {
		Clear();
		m_iID = Src.m_iID;
		m_iProbability = Src.m_iProbability;
		m_pRewardInfo = Src.m_pRewardInfo;
	}

	void SetRewordInfo(int iID, int iProbability) {
		m_iID = iID;
		m_iProbability = iProbability;
	}
};


/**
 * 랭크에 따른 보상 형태에 대한 정보및 실제 보상 데이터의 링크 정보를 지님 <br>
*/
struct SPRewordRank {
	REWARD_TYPE	m_iType;					/**< 랭크 타입 <br> */
	SPRewordInfo m_Info[QUEST_MAX_REWARD];	/**< 완료시 해당 랭크에서 선택가능한 보상 정보 <br> */	

	SPRewordRank(){
		Clear();
	}
	
	~SPRewordRank(){
		Clear();
	}

	void Clear() {
		m_iType = REWARD_TYPE_FIX;		
		for(int i = 0; i < QUEST_MAX_REWARD; i++) {
			m_Info[i].Clear();
		}
	}

	void operator=(const SPRewordRank Src) {
		Clear();
		m_iType = Src.m_iType;		
		
		for(int i = 0; i < QUEST_MAX_REWARD; i++) {
			m_Info[i] = Src.m_Info[i];
		}
	}

	void SetRankType(REWARD_TYPE iType) {
		m_iType = iType;
	}
};


/**
* @class SPQuestAttr
* @brief 퀘스트 로드 단위
* @author Jinhee
* @version 00.00.01
* @date 2005.09.21
* @bug 없음
* @warning 
*/
class SPQuestAttr {
public:
	SPQuestAttr();
	~SPQuestAttr();

	void Clear();

	void operator=(const SPQuestAttr Src);
	void Copy(SPQuestAttr* pSrc);

	bool SetQuestAttr(int iQuestID, std::string strName, int iVisible);	

	bool SetQuestAttrInfo(std::string strTalk1, std::string strTalk2,
		std::string strPurPose, int iRewordNpcID);

	bool SetQuestRequireStat(int iIndex, int iStat, int iStatValue, int iValue1, int iValue2);

	bool SetAcquisitionEffect(int iIndex, int iEffect);

	bool SetAcquisitionItem(int iIndex, int iItemID, int iStack, int iRear);

	bool SetMission(int iIndex, MISSION_TYPE iMission, int iTypeID, int iCount, bool bShow);

	bool SetCunsumeItem(int iIndex, int iItemID, int iStack);

	//bool SetRewordEffect(int iIndex, int iEffect);

	//bool SetRewordItem(int iIndex, int iItemID, int iStack, int iRear);

	bool SetQuestRewordStat(int i, int j, int iStat, int iStatValue, int iValue1, int iValue2);

	bool SetNpc(int iIndex, int iIcon, int iIconIndex, std::string strPlace, std::string strName);

	bool SetNpcMessage(std::string strAccept, int iAccept, 
		std::string strReword, int iReword);

	/**
	 * 기본적인 사항의 요구 조건을 체크한다 <br>
	 * 조건 체크후 UiShow 상태를 설정한다 <br>
	 @param NONE
	 @return QUEST_UI_SHOW
	*/
	QUEST_UI_SHOW IsRequire();

	/**
	* 보상시 요구사항을 확인해 Rank 정보를 반환한다 <br> 
	@param 
	@return
	*/
	REWARD_RANK IsRewardRequire();

	/**
	 * 현재 진행중인 퀘스트의 미션의 경우 <br>
	 * 퀘스트가 Add될때 Item관련 미션에 사용되는 아이템의 갯수를 설정하도록 한다 <br> 
	 @param bool bMsg = true		메시지 출력여부
	 @return bool
	*/
	bool SetCheckMissionItem(bool bMsg = true);
	
	/**
	 * Quest의 UIShow Status를 설정 <br> 
	 @param QUEST_UI_SHOW iUiShow = QUEST_UI_SHOW_NULL
	 @return void
	*/
	void SetQuestUIShow(QUEST_UI_SHOW iUiShow = QUEST_UI_SHOW_NULL);
	
	/**
	 * Quest의 UIShow Status를 확인 <br> 
	 @param NONE
	 @return QUEST_UI_SHOW
	*/
	QUEST_UI_SHOW GetQuestUIShow();		
	

public:
	int m_iQuestID;												/**< Quest ID <br> */
	std::string m_strQuestName;									/**< Quest Name <br> */
	SPQuestNpc m_QuestNpc[QUEST_NPC_MAX];						/**< 퀘스트에 관련한 NPC 정보 <br> */
	
	std::string m_strTalk1;										/**< 대화 1 <br> */
	std::string m_strTalk2;										/**< 대화 2 <br> */
	std::string m_strPurPose;									/**< 목적 <br> */

	int m_iRewordNpcID;											/**< 보상을 주는 NPC ID <br> */

	int m_iVisible;												/**< 강제로 보여주거나 숨기는 옵션 <br> */	
	
	SPRequire m_Require[QUEST_MAX_REQUIRE];						/**< 습득에 필요한 스탯 정보 <br> */	

	int m_iAcquisitionEffect[QUEST_MAX_ACQUISITION_EFFECTID];	/**< 퀘스트 습득시 발동효과 <br> */
	SPQuestItem m_AcquisitionItem[QUEST_MAX_ACQUISITION_ITEM];	/**< 퀘스트 습득시 함게 들어올 아이템 <br> */
	
	SPMission m_Mission[QUEST_MAX_MISSION];						/**< 미션 <br> */	
	
	SPConsumeItem m_ConsumeItem[QUEST_MAX_CONSUMPTION_ITEM];	/**< 퀘스트 완료(보상)시 소비되는 아이템 <br> */

	//int m_iRewordEffect[QUEST_MAX_REWARD_EFFECTID];				/**< 퀘스트 완료(보상)시 받는 효과 <br> */	
	//SPQuestItem m_RewordItem[QUEST_MAX_REWARD_ITEM];			/**< 퀘스트 완료시 보상으로 받게 되는 아이템 <br> */

	SPRequire	m_RewordRequire[QUEST_MAX_RANK][QUEST_MAX_RANK_REQUIRE];  /**< 퀘스트 보상시 보상 레벨 확인용 <br> */
	SPRewordRank m_RewordRank[QUEST_MAX_RANK];					/**< 보상 레벨에 의한 보상 정보 <br> */	

	//플레이어만 사용
	QUEST_UI_SHOW m_iUiShow;									/**< 퀘스트의 UI 표시 설정 <br> */	
	int		m_iRewordCount;										/**< 퀘스트의 보상 횟수 (클리어 반복횟수) <br> */	
	REWARD_RANK	 m_iRank;										/**< 퀘스트 보상시(또는 윈도우가 열린 시점) 결정된 랭크 <br> */

	//퀘스트 수락이나 보상시 Npc Message 처리용 
	std::string m_strMsgAccept;									/**< 퀘스트 수락시 Npc 메시지 <br> */
	int			m_iImageAccept;									/**< 퀘스트 수락시 Npc Image <br> */
	std::string m_strMsgReword;									/**< 퀘스트 수락시 Npc 메시지 <br> */
	int			m_iImageReword;									/**< 퀘스트 수락시 Npc Image <br> */	

	CITY_NAME	m_iCityName;									/**< 퀘스트 지역 정보? <br> */

	QUEST_ACCEPTTYPE	m_eAcceptType;							/**< 수락가능성 체크 <br> */
	INT32				m_iActCharID;							/**< 공유시 사용하는 공유한 캐릭터ID( DBKey ) <br> */

	QUEST_ACCEPTTYPE	m_iCurrentAccept;						/**< 내가 어떻게 수락되었는지의 타입을 기억 <br> */
};



