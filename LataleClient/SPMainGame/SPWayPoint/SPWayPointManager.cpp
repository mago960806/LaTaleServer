#include "SPCommon.h"
#include "SPEventDef.h"
#include "SPEventManager.h"

#include "SPCheckManager.h"

#include "PacketID.h"
#include "Packet.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPSubGameManager.h"
#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPCheckManager.h"
#include "SPLocalizeManager.h"

#include "SPWayPointAttr.h"
#include "SPWayPointArchive.h"
#include "SPWayPointManager.h"

#include "SPDebug.h"


SPWayPointManager::SPWayPointManager()
{
	m_pWayPointArchive = new SPWayPointArchive;

	LoadGroupName();
}


SPWayPointManager::~SPWayPointManager()
{
	m_vGroupName.clear();
	SAFE_DELETE(m_pWayPointArchive);
}


void SPWayPointManager::ClearWayPoint()
{
	m_pWayPointArchive->ClearAllWayPoint();
}


SPWayPointArchive* SPWayPointManager::GetWayPointArchive()
{
	return m_pWayPointArchive;
}


int SPWayPointManager::LoadGroupName()
{	
	int iStartIndex = 2000001;
	int iIndex = 0;
	int i = 0;
	for(i = 0; i < 128; i++) {
		iIndex = iStartIndex + i;
		if(g_pResourceManager->GetGlobalString(iIndex) == NULL )
			break;
		
		std::string strMsg = g_pResourceManager->GetGlobalString(iIndex);

		m_vGroupName.push_back(strMsg);
	}	

	return i;
}


std::vector<std::string>* SPWayPointManager::GetGroupNameList()
{
	return &m_vGroupName;
}


int SPWayPointManager::GetGroupSize()
{
	return static_cast<int>(m_vGroupName.size());
}


SPWayPointAttr* SPWayPointManager::GetWayPoint(int iIndex)
{
	return m_pWayPointArchive->GetWayPoint(iIndex);
}


SPWayPointAttr* SPWayPointManager::FindWayPointByID(int iID)
{
	return m_pWayPointArchive->FindWayPointByID(iID);
}


SPWayPointAttr* SPWayPointManager::FindWayPointByMapEventID(int iMapEventID)
{
	return m_pWayPointArchive->FindWayPointByMapEventID(iMapEventID);
}


bool SPWayPointManager::SetSaveData(int iSaveInfo[WAYPOINT_SIZE_DATA])
{
	int iIndex = 0;
	int iShift = 0;
	
	for(int i = 0; i < WAYPOINT_SIZE_DATA; i++) {
		for(int j = 0; j < 32; j++) {
			iShift = iSaveInfo[i] >> j ;
			if((iShift & CHECK_FIELD) == CHECK_FIELD ) {
				SPWayPointAttr* pWayPointAttr = GetWayPoint(iIndex);
				if(pWayPointAttr == NULL)
					return false;

				pWayPointAttr->m_bSave = true;
			}
			iIndex++;
		}
	}

	//m_pWayPointArchive->PrintSaveInfo();
	
	return true;
}


bool SPWayPointManager::IsNewWayPoint(int iMapEventID)
{
	SPWayPointAttr* pWayPointAttr = m_pWayPointArchive->FindWayPointByMapEventID(iMapEventID);

	if(pWayPointAttr == NULL)
		return false;
	
	if(pWayPointAttr->IsSave() == false) {
		return true ;
	}

	return false;
}


bool SPWayPointManager::SaveNewWayPoint(int iMapEventID)
{
	SPWayPointAttr* pWayPointAttr = m_pWayPointArchive->FindWayPointByMapEventID(iMapEventID);

	if(pWayPointAttr == NULL)
		return false;

	if(pWayPointAttr->IsSave() == false) {
		if(pWayPointAttr->m_iMapEventID == iMapEventID) {
			//
			//WAYPOINT_CS_FIND,
			// int				iWayPointID;		// 웨이포인트아이디

			CPacket Packet;
			Packet.Add((UINT32)WAYPOINT_CS_FIND);			
			Packet.Add((UINT32)pWayPointAttr->m_iID);
			g_pNetworkManager->SPPerformMessage(WAYPOINT_CS_FIND, 0, (LPARAM)&Packet);
			return true ;
		}
	}

	return false;
}



bool SPWayPointManager::OnUpdateNewWayPoint(int iWayPointID)
{
	SPWayPointAttr* pWayPointAttr = m_pWayPointArchive->FindWayPointByID(iWayPointID);
	
	if(pWayPointAttr == NULL)
		return false;

	pWayPointAttr->m_bSave = true;

	if( pWayPointAttr->m_iStoryID )
	{
		if( g_pSubGameManager )
		{
			//g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
			g_pEventManager->SetCutInState(CUT_IN_STATE_NULL);
			g_pSubGameManager->SPPerformMessage(SPGM_SET_STORY_VIEW, 1, pWayPointAttr->m_iStoryID);
		}
	}
	
	return true;
}


bool SPWayPointManager::SendMove(int iWayPointID, int iRank)
{
	//WAYPOINT_CS_MOVE,
	// int				iWayPointID;		// 웨이포인트아이디
	// UINT8			byRank;				// 0, 1

	if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_WAYPOINT)) {	
	
		CPacket Packet;
		Packet.Add((UINT32)WAYPOINT_CS_MOVE);			
		Packet.Add((UINT32)iWayPointID);
		Packet.AddUINT8((UINT8)iRank);
		g_pNetworkManager->SPPerformMessage(WAYPOINT_CS_MOVE, 0, (LPARAM)&Packet);	
		
		return true;
	}

	return false;
}


bool SPWayPointManager::OnMove(CPacket* packet)
{
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_WAYPOINT);
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_WAYPOINT_MOVERESULT, (WPARAM)0, (LPARAM)0);
	}
	
	GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)
	int					iWayPointID;				// 웨이포인트아이디
	
	packet->ExtractUINT32(&ErrorID);
	if(ErrorID) // 실패
	{
		// ErrorID는 GLOBAL_STRING.LDT의 공통에러문구입니다.
		std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );	
		
		return false;
	}	

	packet->ExtractUINT32((UINT32*)&iWayPointID);
	
	return true;
}

