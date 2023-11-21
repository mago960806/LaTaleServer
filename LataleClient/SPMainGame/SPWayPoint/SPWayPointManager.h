#pragma once

class SPWayPointAttr;
class SPWayPointArchive;

//const int CHECK_FIELD		=	0x80000000;
const int CHECK_FIELD		=	0x00000001;

class SPWayPointManager {
public:
	SPWayPointManager();
	~SPWayPointManager();

	/**
	 * 캐릭터 변경시 웨이포인트 정보 초기화 <br> 
	 @param 
	 @return
	*/
	void ClearWayPoint();

	/**
	 * 
	 @param 
	 @return
	*/
	SPWayPointArchive* GetWayPointArchive();

	/**
	 * Global String에 지정된 웨이포인트 그룹별 이름을 로드한다 <br> 
	 @param 
	 @return
	*/
	int LoadGroupName();

	/**
	 * 웨이포인트 그룹별 이름 리스트를 가져온다 <br> 
	 @param 
	 @return
	*/
	std::vector<std::string>* GetGroupNameList();

	/**
	 * 웨이포인트 그룹 리스트 사이즈를 확인한다 <br>
	 @param 
	 @return
	*/
	int GetGroupSize();

	/**
	 * WayPoint Data를 가져온다 <br>
	 @param 
	 @return
	*/
	SPWayPointAttr* GetWayPoint(int iIndex);
	
	/**
	 * 해당 ID의 WayPoint Data를 가져온다 <br>
	 @param 
	 @return
	*/
	SPWayPointAttr* FindWayPointByID(int iID);
	
	/**
	 * 해당 MapEventID의 WayPoint Data를 가져온다 <br>
	 @param 
	 @return
	*/
	SPWayPointAttr* FindWayPointByMapEventID(int iMapEventID);

	/**
	 * Save정보를 업데이트 한다 <br>
	 @param 
	 @return
	*/
	bool SetSaveData(int iSaveInfo[WAYPOINT_SIZE_DATA]);

	/**
	 * 주어진 맵이벤트를 지닌 WayPoint가 저장되어 있는지 확인한다 <br>
	 @param 
	 @return bool	false 인경우 없는 WayPoint이거나 이미 저장된 WayPoint임
	*/
	bool IsNewWayPoint(int iMapEventID);

	/**
	* 주어진 맵이벤트를 지닌 WayPoint가 저장되어 있는지 확인한다 <br>
	* 새로이 저장되는 WayPoint의 경우 서버로 새로운 세이브포인트 등록을 알림 <br>
	@param 
	@return bool	false 인경우 없는 WayPoint이거나 이미 저장된 WayPoint임
	*/
	bool SaveNewWayPoint(int iMapEventID);

	/**
	 * 새로운 웨이포인트가 정장됨을 업데이트 한다 <br> 
	 @param 
	 @return
	*/
	bool OnUpdateNewWayPoint(int iWayPointID);

	/**
	 * 웨이포인트 이동 요청을 한다 <br>
	 @param 
	 @return
	*/
	bool SendMove(int iWayPointID, int iRank);

	/**
	 * 웨이포인트 이동 요청의 응답 <br> 
	 @param 
	 @return
	*/
	bool OnMove(CPacket* packet);

protected:
	

private:
	SPWayPointArchive* m_pWayPointArchive;			/**< 웨이포인트 데이터 <br> */
	std::vector<std::string> m_vGroupName;			/**< 웨이포인트 그룹 데이터 <br> */
};


