
#include <algorithm>

#include "SPCommon.h"
#include "SPQuestAttr.h"
#include "SPQuestArchive.h"
#include "SPQuestManager.h"
#include "SPEventDef.h"
//#include "SPEvent.h"
#include "SPEventManager.h"

#include "SPCheckManager.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPDebug.h"



/**
 * 주어진 ID의 QuestAttr 검사 <br>
*/
struct _IsEqualQuest : binary_function<SPQuestAttr*, int, bool> {
	bool operator() (SPQuestAttr* pQuestAttr, int iQuestID) const
	{
		if(pQuestAttr->m_iQuestID == iQuestID)
			return true;
		return false;
	}
} IsEqualQuest;


/**
 * 2개의 QuestAttr의 보상 NpcID를 비교 <br> 
*/
struct _SortAscendNpcID : public binary_function<SPQuestAttr*, SPQuestAttr*, bool>
{
	bool operator() ( SPQuestAttr* pLeftQuest, SPQuestAttr* pRightQuest) const
	{
		if(pLeftQuest->m_iRewordNpcID < pRightQuest->m_iRewordNpcID){
			return true;
		}
		return false;
	}
} SortAscendNpcID;



//------------------------------------------------------------------------------------
SPQuestArchive::SPQuestArchive()
{
	DeleteAllQuest();
}


//------------------------------------------------------------------------------------
SPQuestArchive::~SPQuestArchive()
{
	DeleteAllQuest();
}


//------------------------------------------------------------------------------------
void SPQuestArchive::DeleteAllQuest()
{
	std::vector<SPQuestAttr*>::iterator iter;
	for(iter = m_vpQuest.begin(); iter != m_vpQuest.end(); iter++) {
		SAFE_DELETE((*iter));
	}
	m_vpQuest.clear();

	DXUTOutputDebugString("Delete Quest size[%d]\n", static_cast<int>(m_vpQuest.size()));
}


//------------------------------------------------------------------------------------
void SPQuestArchive::ClearAllQuest()
{
	std::vector<SPQuestAttr*>::iterator iter;
	for(iter = m_vpQuest.begin(); iter != m_vpQuest.end(); iter++) {
		(*iter)->Clear();
	}
}


//------------------------------------------------------------------------------------
void SPQuestArchive::InitQuestList(int iSize /*= 0*/)
{
	SPQuestAttr* pQuestAttr = NULL;
	
	for(int i = 0; i < iSize ; i++) {
		pQuestAttr = new SPQuestAttr;
		m_vpQuest.push_back(pQuestAttr);
	}

	DXUTOutputDebugString("Init Quest size[%d]\n", static_cast<int>(m_vpQuest.size()));
}


//------------------------------------------------------------------------------------
int SPQuestArchive::AddQuest(SPQuestAttr* pQuestAttr)
{
	m_vpQuest.push_back(pQuestAttr);
	
	return GetSize();
}


//------------------------------------------------------------------------------------
int SPQuestArchive::DeleteQuest(int iQuestID)
{	
	//m_vpQuest.erase(
	//	remove_if(m_vpQuest.begin(), m_vpQuest.end(), std::bind2nd(IsEqualQuest, iQuestID)),
	//	m_vpQuest.end());	

	std::vector<SPQuestAttr*>::iterator iter;
	iter = std::find_if(m_vpQuest.begin(), m_vpQuest.end(),
		std::bind2nd(IsEqualQuest, iQuestID));	

	if(iter != m_vpQuest.end())
	{
		SAFE_DELETE( *iter );
		m_vpQuest.erase(iter);		
	}
	
	return GetSize();
}


//------------------------------------------------------------------------------------
SPQuestAttr* SPQuestArchive::GetQuestAttr(int iIndex)
{
	if(iIndex < 0 || iIndex >= static_cast<int>(m_vpQuest.size()))	
		return NULL;

	return m_vpQuest.at(iIndex);
}


//------------------------------------------------------------------------------------
SPQuestAttr* SPQuestArchive::GetQuest(int iQuestID)
{
	std::vector<SPQuestAttr*>::iterator iter;
	iter = std::find_if(m_vpQuest.begin(), m_vpQuest.end(),
				std::bind2nd(IsEqualQuest, iQuestID));	

	if(iter == m_vpQuest.end())
		return NULL;
	else 
		return (*iter);
	
	return NULL;
}


//------------------------------------------------------------------------------------
int SPQuestArchive::GetSize()
{
	int iSize = static_cast<int>(m_vpQuest.size());
	return iSize;
}


//------------------------------------------------------------------------------------
int SPQuestArchive::GetShowSize()
{
	int iCount = 0;
	
	std::vector<SPQuestAttr*>::iterator iter;
	for(iter = m_vpQuest.begin(); iter != m_vpQuest.end(); ++iter) {
		if((*iter)->m_iUiShow == QUEST_UI_SHOW_ABLE || (*iter)->m_iUiShow == QUEST_UI_SHOW_DISABLE ||
			(*iter)->m_iUiShow == QUEST_UI_SHOW_COMPLETE)
		{
			iCount++;
		}
	}

	return iCount;
}


//------------------------------------------------------------------------------------
int SPQuestArchive::SetMissionInfo(MISSION* pMission)
{
	std::vector<SPQuestAttr*>::iterator iter;

	for(iter = m_vpQuest.begin(); iter != m_vpQuest.end(); ++iter) {
		//(*iter)->m_iUiShow = QUEST_UI_SHOW_COMPLETE;
		
		for(int i = 0; i < QUEST_MAX_MISSION; i++) 
		{
			if((*iter)->m_Mission[i].m_iMissionType == MISSION_TYPE_NULL)
				continue;

			if((*iter)->m_Mission[i].m_iMissionType == MISSION_TYPE_ACTION && 
				(*iter)->m_Mission[i].m_iMissionType == pMission->uiMissionType)
			{
				//[2005/12/28]	클라이언트는 현재 완료하지 못한 미션이더라도 서버에서 완료한 미션의 경우
				//if((*iter)->m_Mission[i].m_bClear == false && pMission->iCurValue >= pMission->iFinishValue)
				//	pMission->uiValueID = pMission->uiValueID + 1;

				//MissionType이 ACTION인 경우에만....
				if((*iter)->m_Mission[i].m_iMissionTypeID == pMission->uiValueID + 1) {
					if((*iter)->m_Mission[i].m_bClear) {
						continue;
					}
					
					if((*iter)->m_Mission[i].m_iMissionType != MISSION_TYPE_ITEM) {
						(*iter)->m_Mission[i].m_iCurCount = pMission->iCurValue;
					}				
					//(*iter)->m_Mission[i].m_bClear = pMission->iFinishValue;
					if((*iter)->m_Mission[i].m_iCurCount >= pMission->iFinishValue) {
						(*iter)->m_Mission[i].m_bClear = true;
						break;
					}

					continue;
				}
			}
			else if((*iter)->m_Mission[i].m_iMissionType == pMission->uiMissionType &&
				(*iter)->m_Mission[i].m_iMissionTypeID == pMission->uiValueID )
			{
				if((*iter)->m_Mission[i].m_bClear) {					//[2005/12/28] 이미 클리어된 미션인 경우 다음 미션으로
					continue;
				}
				
				if((*iter)->m_Mission[i].m_iMissionType != MISSION_TYPE_ITEM) {
					(*iter)->m_Mission[i].m_iCurCount = pMission->iCurValue;
				}				
				//(*iter)->m_Mission[i].m_bClear = pMission->iFinishValue;
				if((*iter)->m_Mission[i].m_iCurCount >= pMission->iFinishValue) {
					(*iter)->m_Mission[i].m_bClear = true;
					break;
				}				
				
				continue;
			}			
		}

		CheckMissionAllClear((*iter), false);
	}	

	return 0;
}


//------------------------------------------------------------------------------------
void SPQuestArchive::CheckRequireStat()
{
	std::vector<SPQuestAttr*>::iterator iter;	

	for(iter = m_vpQuest.begin(); iter != m_vpQuest.end(); ++iter) {
		(*iter)->IsRequire();
	}
}


//------------------------------------------------------------------------------------
int	SPQuestArchive::CheckMissionItem(int iItemID)
{
	std::vector<SPQuestAttr*>::iterator iter;
	int i = 0;
	int iResult = 0;
	for(iter = m_vpQuest.begin(); iter != m_vpQuest.end(); ++iter) {
		for(i = 0; i < QUEST_MAX_MISSION; i++) {
			//if((*iter)->m_Mission[i].m_bClear)
			//	continue;

			if((*iter)->m_Mission[i].m_iMissionType == MISSION_TYPE_ITEM) {
				if((*iter)->m_Mission[i].m_iMissionTypeID == iItemID) {
					//(*iter)->m_Mission[i].m_iCurCount++;
					(*iter)->m_Mission[i].m_iCurCount = g_pCheckManager->GetTotalPCItemCount(iItemID);
					
					if((*iter)->m_Mission[i].m_iCurCount >= (*iter)->m_Mission[i].m_iMissionCount) {
						(*iter)->m_Mission[i].m_iCurCount = (*iter)->m_Mission[i].m_iMissionCount;
						(*iter)->m_Mission[i].m_bClear = true;
						iResult++;						
					}

					CheckMissionAllClear((*iter));
					
					return iResult;				//[2005/10/4]	한번에 하나의 미션만 가능
				}
			}
		}
	}
	return iResult;
}


//------------------------------------------------------------------------------------
int SPQuestArchive::CheckMission(MISSION* pMission)
{
	std::vector<SPQuestAttr*>::iterator iter;
	int iResult = 0;

	for(iter = m_vpQuest.begin(); iter != m_vpQuest.end(); ++iter) {
		for(int i = 0; i < QUEST_MAX_MISSION; i++) 
		{
			if((*iter)->m_Mission[i].m_iMissionType == pMission->uiMissionType &&
				(*iter)->m_Mission[i].m_iMissionTypeID == pMission->uiValueID)
			{
				if((*iter)->m_Mission[i].m_iCurCount < pMission->iCurValue) {
					(*iter)->m_Mission[i].m_iCurCount = pMission->iCurValue;
					//카운터 증가 메시지...
				}
				
				if((*iter)->m_Mission[i].m_iCurCount >= pMission->iFinishValue) {//iFinishValue가 해당 미션에서의 충족요구조건이다
					(*iter)->m_Mission[i].m_bClear = true;
				}

				////
				//if((*iter)->m_Mission[i].m_iCurCount >= ) {	
				//	(*iter)->m_Mission[i].m_bClear = pMission->iFinishValue; // = true;
				//	//(*iter)->m_iUiShow = QUEST_UI_SHOW_COMPLETE;					
				//	//완료 메시지...
				//}

				CheckMissionAllClear((*iter));

				iResult++;

				return iResult;				//[2005/10/4]	한번에 하나의 미션만 가능
			}
		}
	}
	
	return iResult;
}


//------------------------------------------------------------------------------------
int SPQuestArchive::CheckAction(int iAction)
{
	std::vector<SPQuestAttr*>::iterator iter;
	int iResult = 0;

	for(iter = m_vpQuest.begin(); iter != m_vpQuest.end(); ++iter) {
		if((*iter)->m_iUiShow == QUEST_UI_SHOW_COMPLETE)
			continue;
		
		for(int i = 0; i < QUEST_MAX_MISSION; i++) {
			if((*iter)->m_Mission[i].m_bClear)
				continue;

			if((*iter)->m_Mission[i].m_iMissionType == MISSION_TYPE_ACTION) {

				int iCheckAction = 1<<((*iter)->m_Mission[i].m_iMissionTypeID - 2);	//LDT로딩시 이미 +1했음 //성훈씨한테 물어볼것
				
				//현재의 액션을 &로 비교하여 검사하는 액션과 같은 상황이 나오면 행동으로 처리
				if((iAction & iCheckAction) == iCheckAction) {
					return ((*iter)->m_Mission[i].m_iMissionTypeID - 1);	//LDT의 ID와 동일하게 
				}
			}
		}
	}

	return 0;
}	


//------------------------------------------------------------------------------------
bool SPQuestArchive::CheckMapEvent(int iEventID)
{	
	std::vector<SPQuestAttr*>::iterator iter;
	int iResult = 0;

	for(iter = m_vpQuest.begin(); iter != m_vpQuest.end(); ++iter) {
		for(int i = 0; i < QUEST_MAX_MISSION; i++) {
			if((*iter)->m_Mission[i].m_bClear)
				continue;

			if((*iter)->m_Mission[i].m_iMissionType == MISSION_TYPE_EVENT) {

				//주어진 맵이벤트 ID에 맞는것이 있는지 확인
				if(iEventID == (*iter)->m_Mission[i].m_iMissionTypeID) {
					return true;
				}
			}
		}
	}

	return false;
}



//------------------------------------------------------------------------------------
bool SPQuestArchive::CheckMissionAllClear(SPQuestAttr* pQuestAttr, bool bMsg /*= true*/)
{
	bool bResult = true;	
	
	//미션 클리어 상황 확인
	for(int i = 0; i < QUEST_MAX_MISSION; i++) {
		if(pQuestAttr->m_Mission[i].m_iMissionType == MISSION_TYPE_NULL)
			continue;
		
		if(pQuestAttr->m_Mission[i].m_bClear == false)
			bResult = false;

		if(pQuestAttr->m_Mission[i].m_iCurCount < pQuestAttr->m_Mission[i].m_iMissionCount)
			bResult = false;
	}

	//주어진 퀘스트의 모든 미션이 클리어된 상태.
	//여기는 플레이어 진행 퀘스트 목록의 아이템만이 들어올수 있다.
	if(bResult) {		
		if(bMsg && pQuestAttr->m_iUiShow != QUEST_UI_SHOW_COMPLETE) {
			if(g_pResourceManager->GetGlobalString(6001002) != NULL) {
				std::string strMsg = g_pResourceManager->GetGlobalString(6001002);
				char szMsg[512];
				wsprintf(szMsg, strMsg.c_str(), pQuestAttr->m_strQuestName.c_str());
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
			}			
		}

		g_pGOBManager->CheckEventQuestFx();
		pQuestAttr->m_iUiShow = QUEST_UI_SHOW_COMPLETE;
	}
	else {
		if(pQuestAttr->m_iUiShow == QUEST_UI_SHOW_COMPLETE) {
			pQuestAttr->m_iUiShow = QUEST_UI_SHOW_ABLE;
		}
	}

	if( pQuestAttr != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUEST , SPIM_QUEST_REFRESH, (WPARAM)pQuestAttr->m_iQuestID , NULL );	// AJJIYA [10/25/2005]

	return bResult;
}


//------------------------------------------------------------------------------------
bool SPQuestArchive::SetSort()
{
	std::sort(m_vpQuest.begin(), m_vpQuest.end(), SortAscendNpcID);	
	return true;
}

