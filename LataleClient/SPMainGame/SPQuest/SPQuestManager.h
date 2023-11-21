#ifndef __SPQUEST_MANAGER_H__
#define __SPQUEST_MANAGER_H__


/**
 *  퀘스트 리턴값 모음 <br> 
*/
enum QUEST_RETURN {
	QUEST_RETURN_A_INVAILD			= -102,			/**< 기타 에러 <br> */
	QUEST_RETURN_A_FULL				= -101,			/**< 퀘스트 리스트 풀 <br> */
	QUEST_RETURN_A_INVEN			= -100,			/**< 인벤토리 공간 부족 <br> */
	
	QUEST_RETURN_F_SLOT				= -4,			/**< 아이템이 너무 분산되어 있음 <br> */
	QUEST_RETURN_F_INVAILD			= -3,			/**< 기타 에러 <br> */
	QUEST_RETURN_F_NOTCOMPLETE		= -2,			/**< 완료하지 않음 <br> */
	QUEST_RETURN_F_NPC				= -1,			/**< 보상받을 NPC Error <br> */
	QUEST_RETURN_F_INVEN			= 0,			/**< 인벤토리 공간 부족 <br> */
	QUEST_RETURN_OK					= 1,			/**< OK <br> */
};

class SPLDTManager;
class SPLDTFile;

class SPRewardArchive;
class SPQuestAttr;
class SPQuestArchive;

class SPQuestManager
{
public:
	static SPQuestManager* GetInstance();
	static void Release();

	bool Init();

	/**
	 * 현존하는 Quest아카이브내 QuestAttr들을 모두 지운다 <br> 
	 @param NONE
	 @return NONE
	*/
	void PurgeAll();
	
	/**
	 * NPC의 퀘스트 리스트를 가져온다 <br>
	 @param NONE
	 @return SPQuestArchive*
	*/
	SPQuestArchive* GetNpcArchive();
	
	/**
	 * PC의 퀘스트 리스트를 가져온다 <br>
	 @param NONE
	 @return SPQuestArchive*
	*/
	SPQuestArchive* GetPlayerArchive();
	
	/**
	 * PC의 보상 퀘스트 리스트를 가져온다 <br>
	 @param NONE
	 @return SPQuestArchive*
	*/
	SPQuestArchive*	GetRewordArchive();	

	/**
	 * 보상 정보에 관한 리스트를 가져온다 <br>
	 * GetRewordArchive와는 다르다 <br>
	 @param 
	 @return
	*/
	SPRewardArchive* GetRewardInfo();

	/**
	* 해당 ID의 퀘스트 기본 정보를 LDT에서 읽어 퀘스트 단위에 설정한다 <br>
	@param int iItemID
	@param SPQuestAttr* pQuestAttr
	@return bool
	*/
	bool LoadLDTFile(int iItemID, SPQuestAttr* pQuestAttr);

	/**
	* NPC가 지닌 Quest의 갯수만큼 QuestList를 만들고 LDT데이터를 로드한다 <br>
	@param NONE
	@return bool
	*/
	bool LoadNpcQuest();

	/**
	* NPC 퀘스트 아카이브에 퀘스트 별로 추가 <br>
	@param NONE
	@return bool
	*/
	bool LoadQuest(int iQuestID, QUEST_ACCEPTTYPE iType = QUEST_ACCEPTTYPE_NPC , int iTypeID = 0 , bool bRequireCheck = false );

	/**
	* NPC가 지닌 퀘스트를 지운다. UI쪽도 포함 <br>
	@param NONE
	@return void
	*/
	void DeleteNpcQuest();

	/**
	* NPC가 지닌 iQuestID 퀘스트 하나만 지운다. UI쪽도 포함 <br>
	@param int iQuestID
	@return void
	*/
	void DeleteNpcQuest( int iQuestID );
	
	/**
	 * Player가 지닐 Quest를 생성한다 <br>
	 @param QUEST* pQuest
	 @param bool bMsg = true			메시지 출력 여부
	 @return int 생성한뒤 리스트의 사이즈
	*/
	int SetPlayerQuest(QUEST* pQuest, bool bMsg = true);

	/**
	 * Player가 지닌 현재 진행 퀘스트 중 미션 정보를 설정한다 <br>
	 * 내부에서 아카이브로 넘긴다... <br>
	 @param MISSION* pMission
	 @return int 미션이 설정된 퀘스트 인덱스 (선형 인덱스)
	*/
	int SetPlayerMissionInfo(MISSION* pMission);
	
	/**
	 * 완료 퀘스트 목록에서 현재 보상받은 횟수를 얻어온다 <br>
	 * 또는 현재 완료 퀘스트 목록에 있는지 확인한다 <br>
	 @param  int iQuestID
	 @return int
	*/
	int GetRewordCount(int iQuestID);

	/**
	 * 현재 진행목록에 있는 퀘스트 인지 확인한다 <br> 
	 @param int iQuestID
	 @return bool
	*/
	bool IsDoingQuest(int iQuestID);

	/**
	 * 
	 @param 
	 @return
	*/
	bool IsRewordQuest(int iQuestID);
	
	/**
	 * 플레이어 진행의 미션을 업데이트 한다 <br>
	 * 서버로 부터 전송에 의해 이루어진다 <br>
	 @param MISSION* pMission
	 @return bool
	*/
	bool UpdateMission(MISSION* pMission);
	
	/**
	 * 플레이어 진행의 미션중 아이템 관련만 업데이트 한다 <br> 
	 * 인벤 아카이브에 의해 이루어진다 <br>
	 @param int iItemID
	 @return int
	*/
	int CheckMissionItem(int iItemID);

	/**
	 * 플레이어 진행의 미션중 맵이벤트에 관련한 사항이 있을때 <br>
	 * 서버로 업데이트 요청을 하게 한다 <br>
	 @param int iEventID
	 @return bool
	*/
	bool CheckMissionEvent(int iEventID);

	/**
	 * 플레이어 진행의 미션중 액션에 관련한 사항이 있을때 <br>
	 * 서버로 업데이트 요청을 하게 한다 <br>
	 @param int iAction
	 @return int
	*/
	int CheckMissionAction(int iAction);

	/**
	 * 해당 퀘스트가 수락할수 있는 상황인지 검사한다 <br>
	 @param int iQuestID
	 @return QUEST_RETURN
	*/
	QUEST_RETURN IsAcceptQuest(int iQuestID);
	
	/**
	 * 해당 퀘스트 수락 요청 <br>  
	 @param int iQuestID
	 @return ACCEPT_RETURN 
	*/
	QUEST_RETURN SetAcceptQuest(int iQuestID);

	/**
	 * 해당 퀘스트 포기 요청 <br>
	 @param int iQuestID
	 @return int (현재 무조건 가능으로 1처리)
	*/
	int SetGiveUpQuest(int iQuestID);
	
	/**
	 * 퀘스트 포기 요청에 의한 응답 <br> 
	 @param int iQuestID
	 @return NONE
	*/
	bool OnGiveUpQuest(int iQuestID);

	/**
	 * 해당 퀘스트가 보상받을수 있는 상황인지 검사한다 <br> 
	 @param int iQuestID
	 @return QUEST_RETURN
	*/
	QUEST_RETURN IsFinishQuest(int iQuestID);

	/**
	 * 퀘스트 완료 요청 <br> 
	 @param int iQuestID
	 @param int iReward			선택형일때 유저가 선택한 보상인덱스(0, 1, 2)
	 @return int 
	*/
	QUEST_RETURN SetFinishQuest(int iQuestID, int iReward = 0);	

	/**
	 * 퀘스트 완료 요청에 의한 응답 <br>  
	 * 실제 퀘스트 완료 처리를 한다 <br>
	 @param 
	 @return
	*/
	bool OnFinishQuest(QUEST* pQuest);

	/**
	 * 플레이어 리스트가 초기화 되면서 한번 소드 되었는지? <br> 
	 @param NONE
	 @return bool (true인 경우 소트 되어있음)
	*/
	bool IsListSort();
	
	/**
	 * 소트 상황을 설정 <br>
	 * 내부에서 소트 <br>
	 @param bool bSort = true
	 @return NONE
	*/
	void SetListSort(bool bSort = true);

	/**
	 * Accept, Give Up, Finish 패킷을 보낸뒤 해당 패킷응답전 다시 못보내도록 Block <br> 
	 @param 
	 @return
	*/
	void ResetSendBlock();

	/**
	* 공유 퀘스트 패킷을 보낸다  <br> 
	@param int iQuestID
	@return
	*/
	void SendQuestSharePacket( int iQuestID );

	/**
	* 공유 퀘스트 패킷을 받는다  <br> 
	@param int iQuestID
	@param INT32 iActCharID
	@return
	*/
	void RecvQuestSharePacket( int iQuestID , INT32 iActCharID );

	/**
	* 아이템 사용시 퀘스트를 추가한다.	<br> 
	@param int iQuestID
	@param int iItemID
	@return bool
	*/
	bool AddItemQuest( int iQuestID , int iItemID );

protected:
	SPQuestManager();
	virtual ~SPQuestManager();	

	/**
	 * 퀘스트 수락요청 팻킷을 보냄 <br> 
	 @param int iType				수락 종류
	 @param int iTypeID				타입에 의한 값
	 @param int iQuestID			수락 받으려는 퀘스트아디
	 @return NONE
	*/
	void SendAcceptQuest(int iType, int iTypeID, int iQuestID);

	/**
	 * 퀘스트 포기요청 팻킷을 보냄 <br>
	 @param int iQuestID
	 @return void
	*/
	void SendGiveUpQuest(int iQuestID);

	/**
	 * 퀘스트 완료 요청 팻킷을 보냄 <br> 
	 @param int iNpcID
	 @param int iQuestID
	 @param int iRank				완료시 퀘스트 클리어 랭크 정보
	 @param int iReward				선택형일때 유저가 선택한 보상인덱스(0, 1, 2)
	 @return NULL
	*/
	void SendFinishQuest(int iNpcID, int iQuestID, int iRank, int iReward);

	/**
	 * 액션 실행 팻킷을 보냄 <br> 
	 @param 
	 @return
	*/
	void SendMissionAction(int iActionID);

	/**
	 * 채팅창에 메시지 보냄 <br> 
	 @param 
	 @return
	*/
	void SetReturnMsg(QUEST_RETURN iReturn);

protected:
	static SPQuestManager* ms_pkInstance;
	
	SPRewardArchive* m_pRewardArchive;
	SPQuestArchive* m_pNpcArchive;				/**< NPC용 퀘스트 아카이브 <br> */
	SPQuestArchive* m_pPlayerArchive;			/**< PC용 퀘스트 아카이브 <br> */
	SPQuestArchive*	m_pPlayerReword;			/**< PC용 완료 퀘스트 아카이브 <br> */
	
	SPLDTFile*		m_pLDTQuestData;			/**< NPC Data LDT <br> */
	bool			m_bSort;					/**< Player Quest List Sort <br> */
	
	bool			m_bSendPacket;				/**< Accept, Give Up, Finish Send시 활성되 이후 패킷 전송을 block <br> */	
};

#endif