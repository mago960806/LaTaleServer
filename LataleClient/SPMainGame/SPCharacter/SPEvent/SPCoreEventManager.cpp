
#include "SPCommon.h"
#include "SPUtil.h"

#include <bitset>

#include "PacketID.h"
#include "Packet.h"

#include "SPEventDEF.h"

#include "SPWayPointManager.h"
#include "SPCheckManager.h"

#include "SPCoreMissionAttr.h"
#include "SPCoreMissionArchive.h"
#include "SPCoreEventManager.h"

#include "SPDebug.h"



SPCoreEventManager::SPCoreEventManager()
{
	m_pMissionArchive = new SPCoreMissionArchive;
	m_pMissionArchive->LoadCoreMissionLDT();
	Clear();
}


SPCoreEventManager::~SPCoreEventManager()
{
	SAFE_DELETE(m_pMissionArchive);
}


void SPCoreEventManager::Clear()
{
	RestPlayerMission();
}


void SPCoreEventManager::RestPlayerMission()
{
	for(int i = 0; i < MAX_EVENT_MISSION_BIT; i++) {
		m_bPlayerMission[i] = false;
	}
}


void SPCoreEventManager::SetPlayerMissionData(int iMissionInfo[MAX_EVENT_MISSION_DATA])
{
	int iIndex = 0;
	int iShift = 0;

	for(int i = 0; i < MAX_EVENT_MISSION_DATA; i++) {
		for(int j = 0; j < 32; j++) {
			iShift = iMissionInfo[i] >> j ;
			if((iShift & CHECK_FIELD) == CHECK_FIELD ) {
				m_bPlayerMission[iIndex] = true;
			}
			iIndex++;
		}
	}
}


void SPCoreEventManager::SetPlayerMission(int iIndex)
{
	if(iIndex < 0 || iIndex >= MAX_EVENT_MISSION_BIT)
		return;

	if(m_bPlayerMission[iIndex] == false)
		m_bPlayerMission[iIndex] = true;
}


bool SPCoreEventManager::IsCompleteMission(int iIndex)
{
	if(iIndex < 0 || iIndex >= MAX_EVENT_MISSION_BIT)
		return false;

	return m_bPlayerMission[iIndex];
}


SPCoreMissionArchive* SPCoreEventManager::GetMissionArchive()
{
	return m_pMissionArchive;
}


bool SPCoreEventManager::SendCoreEventRaise(int iMissionID)
{
	if(iMissionID < 0)
		return false;
	
	SPCoreMissionAttr* pMissionAttr = m_pMissionArchive->GetCoreMissionAttr(iMissionID);	
	int iSaveIndex = pMissionAttr->m_iSavePos;
	
	//저장을 하는 이벤트의 경우
	if(iSaveIndex >= 0) {
		if(IsCompleteMission(iSaveIndex)){
			return false;
		}
	}
	
	

	if(g_pCheckManager->IsDBBlock())
		return false;

	g_pCheckManager->SetDBBlock(true);

	CPacket Packet;
	Packet.Add((UINT32)EVENTMISSION_CS_RAISE);			
	Packet.Add((UINT32)iMissionID);
	g_pNetworkManager->SPPerformMessage(EVENTMISSION_CS_RAISE, 0, (LPARAM)&Packet);
	return true ;
}


