#include "SPCommon.h"
#include "SPUtil.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPEventDEF.h"
#include "SPEventArchive.h"
#include "SPEventManager.h"

#include "SPCheckManager.h"

#include "SPWayPointAttr.h"

#include "SPDebug.h"


SPWayPointAttr::SPWayPointAttr()
{
	Clear();
}


SPWayPointAttr::~SPWayPointAttr()
{
	Clear();
}


void SPWayPointAttr::Clear()
{
	m_iID				= 0;
	m_iGroup			= 0;
	m_strName.clear();
	m_strDesc.clear();
	m_iType				= WAY_POINT_TYPE_NULL;
	m_iMapEventID		= 0;
	m_pMapEvent			= NULL;	
	m_strErr.clear();

	for(int i = 0; i < MAX_WAY_POINT_RANK; i++) {
		for(int j = 0; j < MAX_WAY_POINT_REQUIRE; j++) {
			m_Require[i][j].Clear();
		}

		m_iUseItemID[i] = 0;
		m_iUseItemCount[i] = 0;
	}

	for(int i = 0; i < MAX_WAY_POINT_EFFECT; i++) {
		m_iEffect[i] = 0;
	}

	m_iStoryID			= 0;
	m_bOpen				= true;

	m_bSave				= false;
	m_iRank				= WAY_POINT_RANK_NULL;
}


void SPWayPointAttr::Reset()
{
	m_bSave				= false;
	m_iRank				= WAY_POINT_RANK_NULL;
}


bool SPWayPointAttr::IsSave()
{
	return m_bSave;
}


bool SPWayPointAttr::SetMapEventID(int iID)
{
	m_iMapEventID = iID;

	m_pMapEvent = g_pEventManager->GetEventArchive()->GetEvent(m_iMapEventID);
	
	if(m_pMapEvent == NULL)								//찾은 이벤트가 없는 경우
		return false;

	if(m_pMapEvent->eventType != EVENT_SAVEPOINT) {		//찾은 이벤트가 SavePoint가 아닌경우
		m_pMapEvent = NULL;
		return false;
	}

	return true;
}


WAY_POINT_RANK	SPWayPointAttr::CheckRequire()
{
	REQUIRE_TYPE iRequireType = REQUIRE_TYPE_NULL;
	int iID = 0;
	int iValue1 = 0;
	int iValue2 = 0;

	bool bCheck[MAX_WAY_POINT_REQUIRE];
	m_iRank = WAY_POINT_RANK_NULL;					//완료 조건 검사시 랭크 초기화... 

	int i = 0;

	for(i = 0; i < MAX_WAY_POINT_RANK; i++) {
		for(int j = 0; j < MAX_WAY_POINT_REQUIRE; j++) {
			bCheck[j] = false;

			if( g_pCheckManager->CheckCondition(m_Require[i][j].m_iRequireStat, 
				m_Require[i][j].m_iRequireStatValue, 
				m_Require[i][j].m_iValue1,
				m_Require[i][j].m_iValue2) == false)
			{				
				continue;
			}
			else {
				bCheck[j] = true;
			}
		}

		bool bOk = true;
		for(int k = 0; k < MAX_WAY_POINT_REQUIRE; k++) {
			bOk &= bCheck[k];
		}

		if(bOk) {
			m_iRank = (WAY_POINT_RANK)i;
			DXUTOutputDebugString("WayPoint[%d] CheckRequire [%d]\n", m_iID, m_iRank);			
			return m_iRank;
		}
	}

	DXUTOutputDebugString("WayPoint[%d] CheckRequire Find Fail [%d]\n", m_iID, m_iRank);
	return m_iRank;
}

