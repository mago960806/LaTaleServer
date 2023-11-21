
// Copyright (C) jinhee
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : jinhee 2005-1-13 오후 2:56:36
//***************************************************************************


#pragma once


struct SPMapInfo;
class SPMapGroup;
class SPStage;

/**
* @class SPStageManager
* @brief 스테이지 관리
* @author Jinhee
* @version 00.00.01
* @date 2005.1.13
* @bug 없음
* @warning 리소스 관리와 LocalPlayer의 Stage, MapGroup Index를 관리한다.
*/
class SPStageManager {
public:
	SPStageManager();
	~SPStageManager();

	/**
	* 스테이지 리스트를 초기화 한다 <br> 
	@param NONE
	@return NONE
	*/
	void DeleteAllStage();

	/**
	 * 현재 스테이지의 갯수를 얻어온다 <br>  
	 @param NONE
	 @return int 스테이지 갯수
	*/
	int	GetStageCount();

	/**
	 * 현재 스테이지 인덱스를 설정 <br>
	 @param int iStage = -1
	 @return int
	*/
	int SetCurStageIndex(int iStage = -1);
	
	/**
	 * 현재 그룹 인덱스를 설정 <br>
	 @param int iGroup = -1
	 @return int
	*/
	int SetCurGroupIndex(int iGroup = -1);
	
	/**
	 * 현재 맵 인덱스를 설정 <br>
	 @param int iMap = 0
	 @param int iGroup = -1
	 @param int iStage = -1
	 @return int
	*/
	int SetCurMapIndex(int iMap = 0, int iGroup = -1, int iStage = -1);

	/**
	 * 현재 스테이지의 인덱스를 가져온다 <br>
	 @param NONE
	 @return int
	*/
	int GetCurStageIndex();
	
	/**
	 * 현재 스테이지의 그룹 인덱스를 가져온다 <br>
	 @param NONE
	 @return int
	*/
	int GetCurGroupIndex();
	
	/**
	 * 현재 스테이지 그룹 내 맵 인덱스를 가져온다 <br>
	 @param NONE
	 @return int
	*/
	int GetCurMapIndex();	

	/**
	 * 현재 스테이지를 가져온다 <br>
	 @param int iStage = -1		인덱스로 스테이지를 가져올때
	 @return SPStage*	없는경우 NULL
	*/
	SPStage* GetCurStage(int iStage = -1);
	
	/**
	 * 현재 스테이지의 그룹을 가져온다 <br>
	 @param int iGroup = -1	인덱스
	 @param int iStage = -1 인덱스
	 @return SPMapGroup*	없는경우 NULL
	*/
	SPMapGroup* GetMapGroup(int iGroup = -1, int iStage = -1);	

	/**
	 * 다음 가야할 스테이지 정보를 미리 셋팅해 기억하고 있는다 <br>
	 @param int iStage
	 @param int iMapGroup
	 @return NULL
	*/
	void SetNextStage(int iStage, int iMapGroup) {
		m_iNextStage = iStage;
		m_iNextMapGroup = iMapGroup;
	}

	/**
	 * 다음 가야할 스테이지 정보를 확인한다  <br>
	 @param NONE
	 @return int
	*/
	int GetNextStage() { return m_iNextStage; }
	
	/**
	 * 다음 가야할 맵 그룹 정보를 확인한다 <br>
	 @param NONE
	 @return int
	*/
	int GetNextMapGroup() { return m_iNextMapGroup; }


protected:
	/**
	 * 스테이지 리스트 외 메니저의 맴버를 초기화 한다 <br> 
	 @param NONE
	 @return NONE
	*/
	void SetNull();
	
	

public:
	//int m_iStageCount;
	std::vector<SPStage*>	m_vpStageList;	

protected:	
	int m_iCurStage;				/**< 현재 선택한 스테이지의 인덱스 <br> */
	int m_iCurMap;					/**< 이 ID는 스테이지에 종속적이지 않으므로... <br> */

	int m_iNextStage;				/**< 다음 가야할 스테이지 <br> */
	int m_iNextMapGroup;			/**< 다음 가야할 맵그룹 <br> */
};

extern SPStageManager g_StageManager;