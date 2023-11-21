#pragma once

enum WAY_POINT_TYPE {
	WAY_POINT_TYPE_NULL			= 0,		// NULL
	WAY_POINT_TYPE_STONE		= 1,		// 석탑
	WAY_POINT_TYPE_TOMB			= 2,		// 비석
};

enum WAY_POINT_RANK {
	WAY_POINT_RANK_0			= 0,		// Way Point Require Rank 0
	WAY_POINT_RANK_1			= 1,		// Way Point Require Rank 1
	WAY_POINT_RANK_NULL			= 3,		// Way Point Require Rank NULL
};

const int MAX_WAY_POINT_RANK	= 2;		// 웨이포인트 랭크 
const int MAX_WAY_POINT_REQUIRE	= 2;		// 각랭크별 리콰이어의 갯수
const int MAX_WAY_POINT_EFFECT	= 2;		// 최대 저장 이펙트 갯수, 클라이언트에서 사용안함 

#include "SPQuestAttr.h"


class SPWayPointAttr {
public:
	SPWayPointAttr();
	~SPWayPointAttr();
	
	/**
	 * 모든 내부 데이터 초기화 <br> 
	 @param 
	 @return
	*/
	void Clear();
	
	/**
	 * 웨이포인트 저장정보만 초기화 <br> 
	 @param 
	 @return
	*/
	void Reset();

	/**
	 * 저장 되어 있는지 확인한다 <br> 
	 @param 
	 @return bool	ture인 경우 저장되어 있음
	*/
	bool IsSave();

	/**
	 * 맵이벤트 ID를 설정하면서 맵이벤트 포인터를 설정한다 <br>
	 @param 
	 @return
	*/
	bool SetMapEventID(int iID);

	/**
	 * 포탈전 조건 체크 <br>
	 @param 
	 @return
	*/
	WAY_POINT_RANK	CheckRequire();
	

protected:
	

public:
	int				m_iID;								/**< 웨이포인트 ID <br> MapEvent ID와 동일 <br> */
	int				m_iGroup;							/**< 웨이포인트가 속한 Group ID <br> */
	std::string		m_strName;							/**< 웨이포인트 이름 <br> */
	std::string		m_strDesc;							/**< 웨이포인트 설명 <br> */
	WAY_POINT_TYPE	m_iType;							/**< 웨이포인트 타입 <br> */
	int				m_iMapEventID;						/**< 웨이포인트 ID와 동일 <br> */
	MAPEVENT_INFO*	m_pMapEvent;						/**< 해당 ID의 MapEvent 정보 <br> 로딩시 셋팅됨 <br> */
	SPRequire		m_Require[MAX_WAY_POINT_RANK][MAX_WAY_POINT_REQUIRE];	/**< 웨이포인트 체크용 요구사항 <br> */
	int				m_iUseItemID[MAX_WAY_POINT_RANK];						/**< 사용될 아이템 ID <br> */
	int				m_iUseItemCount[MAX_WAY_POINT_RANK];					/**< 사용될 아이템 갯수 <br> */
	int				m_iEffect[MAX_WAY_POINT_EFFECT];						/**< 저장시 사용되는 이펙트 <br> */
	std::string		m_strErr;							/**< Require Rank NULL 이거나 Item 체크가 실패했을시 출력 메시지 <br> */
	int				m_iStoryID;							/**< 웨이포인트 에 설정된 스토리 ID <br> */
	bool			m_bOpen;							/**< 웨이포인트가 개방이 되어 있는지의 플래그 <br> */

	bool			m_bSave;							/**< 웨이포인트의 저장여부 <br> */
	WAY_POINT_RANK	m_iRank;							/**< 웨이포인트 체크용 리콰에 의해 결정된 랭크 <br> */
};


