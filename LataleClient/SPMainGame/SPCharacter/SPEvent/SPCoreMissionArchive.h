#pragma once 

class SPCoreMissionAttr;

class SPCoreMissionArchive {
public:
	SPCoreMissionArchive();
	~SPCoreMissionArchive();
	
	bool LoadCoreMissionLDT();

	/**
	 * 주어진 ID의 MissionAttr을 가져온다 <br>
	 @param 
	 @return  
	*/
	SPCoreMissionAttr*	GetCoreMissionAttr(int iItemID);
	
	/**
	 * 주어진 SavePosition을 지니는 MissionAttr을 찾아낸다 <br>
	 @param 
	 @return  
	*/
	SPCoreMissionAttr*	FindMissionAttr(int iSavePos);

private:
	/**
	 * 모든 MissionAttr을 삭제한다 <br>
	 @param 
	 @return  
	*/
	void DeleteAllCoreMission();
	
	/**
	 *
	 @param 
	 @return  
	*/
	void Clear();

private:
	std::map<int, SPCoreMissionAttr*>		m_mCoreMission;
};



