#ifndef __SPQUEST_ARCHIVE_H__
#define __SPQUEST_ARCHIVE_H__

class SPQuestAttr;

class SPQuestArchive
{
public:
	SPQuestArchive();
	~SPQuestArchive();
	
	/**
	 * 모든 QuestList의 내부 데이터를 삭제 <br>  
	 @param NONE
	 @return NONE
	*/
	void DeleteAllQuest();

	/**
	 * 모든 QuestList의 내부 데이터를 초기화 <br>
	 @param  NONE
	 @return NONE
	*/
	void ClearAllQuest();

	/**
	 * 주어진 사이즈 만큼의 비어있는 QuestList 생성 <br> 
	 @param  int iSize = 0
	 @return NONE
	*/
	void InitQuestList(int iSize = 0);
	
	/**
	 * 주어진 QuestAttr을 리스트 마지막에 추가한다 <br> 
	 @param SPQuestAttr* pQuestAttr
	 @return int 추가한뒤 리스트의 사이즈
	*/
	int AddQuest(SPQuestAttr* pQuestAttr);

	/**
	 * 주어진 ID의 퀘스트를 삭제한다 <br>
	 @param int iQuestID
	 @return int
	*/
	int DeleteQuest(int iQuestID);
	
	/**
	 * 주어진 인덱스의 Quest 단위를 가져온다 <br> 
	 @param int iIndex
	 @return SPQuestAttr*
	*/
	SPQuestAttr* GetQuestAttr(int iIndex);

	/**
	 * 주어진 QuestID의 Quest 단위를 가져온다 <br>
	 @param int iQuestID
	 @return SPQuestAttr*
	*/
	SPQuestAttr* GetQuest(int iQuestID);

	/**
	 * 현재 리스트의 사이즈를 가져온다 <br> 
	 @param 
	 @return
	*/
	int GetSize();

	/**
	* 현재 리스트의 사이즈를 가져온다 <br> 
	@param 
	@return
	*/
	int GetShowSize();
	
	/**
	 * 현재 진행중인 PC 퀘스트의 경우 처음 설정시 <br>
	 * 현재까지의 진행한 미션정보를 설정한다 <br>
	 @param MISSION* pMission
	 @return int
	*/
	int SetMissionInfo(MISSION* pMission);

	/**
	* 현재 리스트내 모든 퀘스트의 요구조건을 비교해 현재의 상태를 설정하도록 한다 <br> 
	* 값의 설정은 각 퀘스트 단위의 m_iUIShow로 설정된다 <br>
	@param  NONE
	@return NONE
	*/
	void CheckRequireStat();
	
	/**
	 * 아이템 획득시 퀘스트 리스트에서 해당 아이템을 필요로하는 미션을 찾아 갱신 <br>
	 @param int iItemID
	 @return int (업데이트 된 미션의 수)
	*/
	int	CheckMissionItem(int iItemID);

	/**
	 * 몬스터, 액션, 맵이벤트의 경우 퀘스트 리스트에서 해당의 것을 필요로 하는 미션을 찾아 갱신 <br>
	 @param MISSION* pMission
	 @return int (업데이트 된 미션의 수)
	*/
	int CheckMission(MISSION* pMission);

	/**
	 * 퀘스트 중 액션미션이 있는경우 주어진 Action을 검사하여 <br>
	 * 진행한 상황이라면 미션 TypeID (검사할 퀘스트)를 리턴한다 <br>
	 @param int iAction
	 @return int QuestAttr.m_Missin[i].m_iMissionTypeID
	*/
	int CheckAction(int iAction);

	/**
	 * 퀘스트 중 액션 맵이벤트가 있는 경우 주어진 MapEvent를 검사하여 <br>
	 * 진행한 상황이라면 가불 여부를 리턴한다 <br>
	 * 리턴 받은 쪽에서 상황에 따라 맵이벤트 메시지를 서버로 보낸다 <br> 
	 @param int iEventID
	 @return bool
	*/
	bool CheckMapEvent(int iEventID);


	/**
	 * 주어진 퀘스트 단위가 모든 미션을 완료한 상태인지 확인한다 <br>  
	 @param SPQuestAttr* pQuestAttr
	 @param bool bMsg = true		true인 경우 채팅창에 퀘스트 완료 메시지 출력
	 @return bool
	*/
	bool CheckMissionAllClear(SPQuestAttr* pQuestAttr, bool bMsg = true);

	/**
	 * 퀘스트 리스트를 RewordNpcID의 오름차순으로 정렬한다 <br>
	 * 실제로 Player 진행 퀘스트 쪽만 정렬한다 <br>
	 @param NONE
	 @return bool
	*/
	bool SetSort();
	

protected:
	std::vector<SPQuestAttr*> m_vpQuest;			/**< 아카이브가 지니는 퀘스트의 리스트 <br> */
};

#endif