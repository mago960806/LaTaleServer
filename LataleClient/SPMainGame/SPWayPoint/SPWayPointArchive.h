#pragma once


class SPWayPointAttr;

const int	MAX_WAY_POINT		= 128;			/**< 최대 Waypoint 의 갯수 <br> */

class SPWayPointArchive {
public:
	SPWayPointArchive();
	~SPWayPointArchive();
	
	/**
	 * 모든 WayPoint 정보를 Reset한다 <br> 
	 @param 
	 @return
	*/
	void ClearAllWayPoint();
	
	/**
	 * LDT에서 기본 WayPoint정보를 Load한다 <br>
	 @param 
	 @return
	*/
	bool LoadLDTData();

	/**
	 * WayPoint 리스트를 가져온다 <br>
	 @param 
	 @return
	*/
	std::vector<SPWayPointAttr>*	GetWayPointList();
	
	/**
	 * WayPoint Data를 인덱스 순서에 의해 가져온다 <br>
	 @param 
	 @return	 
	*/
	SPWayPointAttr* GetWayPoint(int iIndex);
	
	/**
	 * WayPoint Data를 주어진 ID에서 찾아 가져온다 <br>
	 * 없으면 NULL <br>
	 @param 
	 @return
	*/
	SPWayPointAttr* FindWayPointByID(int iID);
	
	/**
	 * WayPoint Data를 주어진 MapEventID에서 찾아 가져온다 <br>
	 * 없으면 NULL <br>
	 @param 
	 @return
	*/
	SPWayPointAttr* FindWayPointByMapEventID(int iMapEventID);

	/**
	 * 디버깅용 <br>
	 @param 
	 @return
	*/
	void PrintSaveInfo();

protected:	

private:
	std::vector<SPWayPointAttr>		m_vpWayPoint;		/**< 웨이포인트 리스트 <br> */
};

