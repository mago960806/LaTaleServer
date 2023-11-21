
#include "SPCommon.h"
//#include "PacketHandler.h"
#include "SPManagerDef.h"
#include "SPManager.h"
//#include "SPNetworkDEF.h"
//#include "SPNetworkManager.h"

#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPMouseCursor.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPRenderModel.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPEventDEF.h"
#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBManager.h"
#include "SPEvent.h"
#include "SPEventArchive.h"
#include "SPEventManager.h"
#include "SPEventNpcHandler.h"

////
//#include "SPRenderModelDEF.h"
//#include "SPRenderModel.h"
//#include "SPGOBClusterDef.h"
//#include "SPAvatarModelUnitDef.h"
//#include "SPGOBModel.h"
//#include "SPGOBModelUnit.h"
//#include "SPAvatarModelUnit.h"
//#include "SPGOBCluster.h"
//#include "SPGameObject.h"
//#include "SPGOBStatus.h"
//#include "SPGOBCoordPhysics.h"
//#include "SPGOBModelUnit.h"			
//#include "SPPlayerEquipInfo.h"
//#include "SPGOBStatus.h"			
//#include "SPPlayerInvenArchive.h"	
//#include "SPPlayerStatusModel.h"	
//#include "SPPlayerEquipInfo.h"
//#include "SPItemAttr.h"
//#include "SPItemStatus.h"
//#include "SPItemCluster.h"
//#include "SPPlayer.h"
//#include "SPGOBManager.h"


//#include "SPItemAttr.h"
//#include "SPItemStatus.h"

//#include "SPStage.h"
//#include "SPStageManager.h"

//#ifdef _DEBUG
//#include <iostream>			//jinhee - [2005/1/3] //테스트를 위한...
//#include <fstream>
//#endif

#include "Packet.h"

#include "SPQuestAttr.h"
#include "SPQuestArchive.h"
#include "SPQuestManager.h"

#include "SPGiftAttr.h"
#include "SPGiftArchive.h"
#include "SPGiftManager.h"

#include "SPMailAttr.h"
#include "SPMailArchive.h"
#include "SPMailManager.h"

#include "SPUserShopAttr.h"
#include "SPUserShopArchive.h"

#include "SPWayPointAttr.h"
#include "SPWayPointArchive.h"
#include "SPWayPointManager.h"

#include "SPCoreMissionAttr.h"
#include "SPCoreMissionArchive.h"
#include "SPCoreEventManager.h"
#include "SPClassChangeManager.h"

#include "SPCheckManager.h"

#include "SPDebug.h"

SPEventManager*	g_pEventManager = NULL;

SPEventManager::SPEventManager() : SPManager()
, m_bCutInRender	(false)
, m_pEventArchive(NULL)
, m_pNpcHandler(NULL)
, m_pQuestManager(NULL)
, m_pEvent(NULL)
, m_bSendPacket(false)
, m_bEventBlock(false)
, m_pGiftManager(NULL)
, m_pMailManager(NULL)
, m_pUserShopArchive(NULL)
, m_pWayPointManager(NULL)
, m_pClassChangeManager(NULL)
, m_pCoreRequire(NULL)
{
	SetMgrID(MGR_ID_EVENT);
	g_pEventManager = this;
}


SPEventManager::~SPEventManager()
{
	Clean();
}


bool SPEventManager::Init() 
{
	SPManager::Init();
	
	
	DXUTOutputDebugString("SPEventManager::Init() ...\n");

	m_pEventArchive = new SPEventArchive;		

	m_pNpcHandler = new SPEventNpcHandler;
	m_pQuestManager = SPQuestManager::GetInstance();
	m_pGiftManager = new SPGiftManager;
	m_pMailManager = new SPMailManager;	

	m_pEventArchive->LoadEventLDT();
	m_pEventArchive->LoadMapInfoLDT();
	m_pEventArchive->LoadEvevtPortalLDT();
	m_pEventArchive->LoadNpcLDT();
	m_pEventArchive->LoadQuestListLDT();
	m_pEventArchive->LoadQuestAttrLDT();
	m_pEventArchive->LoadGlobalSysLDT();
	m_pEventArchive->LoadCoreEventInfoLDT();
	m_pEventArchive->LoadCoreRequireAttrLDT();	
	m_pEventArchive->LoadCutInTalkLDT();
	
	m_pClassChangeManager = new SPClassChangeManager;

	m_pUserShopArchive = new SPUserShopArchive;
	m_pWayPointManager = new SPWayPointManager;	
	m_pCoreEventManager = new SPCoreEventManager;	
	
	m_bSendPacket = false;
	m_bEventBlock = false;
	
	return true;
}


void SPEventManager::Clean() 
{		
	SAFE_DELETE(m_pCoreEventManager);
	SAFE_DELETE(m_pNpcHandler);
	SAFE_DELETE(m_pEventArchive);
	m_pQuestManager->Release();
	SAFE_DELETE(m_pGiftManager);
	SAFE_DELETE(m_pMailManager);
	SAFE_DELETE(m_pClassChangeManager);
	SAFE_DELETE(m_pUserShopArchive);
	SAFE_DELETE(m_pWayPointManager);
	SPManager::Clean();	
}


void SPEventManager::Process(float fTime) 
{	
	SPManager::Process(fTime);

	m_pNpcHandler->Process(fTime);
}


void SPEventManager::Render(float fTime) 
{
	SPManager::Render(fTime);

	m_pNpcHandler->Render(fTime);
}



//////////////////////////////////////////////////////////////////////////
int SPEventManager::SPPerformMessage(UINT msg, WPARAM wparam /*= 0*/, LPARAM lParam /*= 0*/)
{
	return 1;
}


void SPEventManager::SetCutIn(bool bCutIn /*= false*/)
{
	m_bCutInRender = bCutIn;

	m_pNpcHandler->SetCutIn(m_bCutInRender);
}


bool SPEventManager::IsCutIn()
{
	return m_bCutInRender;
}


void SPEventManager::SetCutInState(CUT_IN_STATE iCutInState /*= CUT_IN_STATE_NULL*/, MAPEVENT_INFO* pEvent /*= NULL*/)
{
	////
	//if(iCutInState == CUT_IN_STATE_NULL) {
	//	SetCutIn(false);
	//}
	//else if(iCutInState == CUT_IN_STATE_SCROLL) {
	//	SetCutIn(true);
	//}
	//
	//m_pNpcHandler->SetCutInState(iCutInState);

	//[2006/2/22]
	if(m_bEventBlock)
		return;

	if(iCutInState == CUT_IN_STATE_NULL) {
		//
		if(m_bSendPacket == false && m_pEvent) {
			if(m_pNpcHandler->GetCutInState() != CUT_IN_STATE_NULL) {
				m_pCoreRequire = NULL;
				
				SendCutInEnd(m_pEvent->eventTypeID);

				if( m_pQuestManager != NULL )
					m_pQuestManager->DeleteNpcQuest();
			}
		}
		
		//OnCutInEnd();
	}
	else if(iCutInState == CUT_IN_STATE_SCROLL) {
		if(m_bSendPacket == false && pEvent) {
			m_pEvent = pEvent;
			SendCutInStart(m_pEvent->eventTypeID);
		}		
	}
	else {
		if(m_pNpcHandler->GetCutInState() == CUT_IN_STATE_RELEASE)
			return;
		
		//[2007/10/17] - 컷인이 아닌데 해제 요청이 들어오는 경우
		if(iCutInState == CUT_IN_STATE_RELEASE && m_pNpcHandler->GetCutInState() == CUT_IN_STATE_NULL)
			iCutInState = CUT_IN_STATE_NULL;
		
		m_pNpcHandler->SetCutInState(iCutInState);
	}	
}


CUT_IN_STATE SPEventManager::GetCutInState()
{
	return m_pNpcHandler->GetCutInState();
}


SPEventNpcHandler* SPEventManager::GetEventHandler()
{
	return m_pNpcHandler;
}


SPQuestManager* SPEventManager::GetQuestManager()
{
	return m_pQuestManager;
}


SPEventArchive*	SPEventManager::GetEventArchive()
{
	return m_pEventArchive;
}


SPGiftManager* SPEventManager::GetGiftManager()
{
	return m_pGiftManager;
}


SPMailManager* SPEventManager::GetMailManager()
{
	return m_pMailManager;
}


SPUserShopArchive* SPEventManager::GetUserShopArchive()
{
	return m_pUserShopArchive;
}


SPWayPointManager* SPEventManager::GetWayPointManager()
{
	return m_pWayPointManager;
}


SPCoreEventManager* SPEventManager::GetCoreEventManager()
{
	return m_pCoreEventManager;
}


SPClassChangeManager* SPEventManager::GetClassChangeManager()
{
	return m_pClassChangeManager;
}


void SPEventManager::OnCutInStart()
{
	if(m_pEvent == NULL) 
		return;
	
	SetCutIn(true);
	m_pNpcHandler->SetCutInState(CUT_IN_STATE_SCROLL);	

	if(m_pEvent->eventType == EVENT_NPC){
		g_pEventManager->GetEventHandler()->LoadNpc(m_pEvent->eventTypeID);
		
		g_pGOBManager->ClearEventQuestFx();
	}

	m_bSendPacket = false;
}


void SPEventManager::OnCutInEnd()
{
	g_pGOBManager->CheckEventQuestFx();
	
	SetCutIn(false);
	m_pNpcHandler->SetCutInState(CUT_IN_STATE_NULL);
	m_pEvent = NULL;

	m_bSendPacket = false;

	//OnCutInEnd();									//메시지 보낸뒤 클라이언트 단에서 그냥 닫아버림...
}


void SPEventManager::SetCoreRequireAttr(SPCoreRequireAttr* pAttr)
{
	m_pCoreRequire = pAttr;
}


SPCoreRequireAttr* SPEventManager::GetCoreRequireAttr()
{
	if(m_pCoreRequire)
		return m_pCoreRequire;

	return NULL;
}


CUT_IN_STATE SPEventManager::GetNextCutInStateByCoreEvent()
{
	CUT_IN_STATE iReturn = CUT_IN_STATE_NULL;

	if(m_pNpcHandler->GetCutInState() < CUT_IN_STATE_SCROLL_END || 
		m_pNpcHandler->GetCutInState() == CUT_IN_STATE_RELEASE)
		return CUT_IN_STATE_RELEASE;
	
	if(m_pCoreRequire && m_pCoreRequire->m_iID)	{
		if(m_pCoreRequire->m_iEventType && m_pCoreRequire->m_iEventID){
			switch(m_pCoreRequire->m_iEventType)
			{
			case NPC_EVENT_TYPE_TALK:
				return CUT_IN_STATE_TALK;
			case NPC_EVENT_TYPE_TRADE:
				return CUT_IN_STATE_TRADE;
				break;
			case NPC_EVENT_TYPE_STORAGE:
				return CUT_IN_STATE_STORAGE;
				break;
			case NPC_EVENT_TYPE_QUEST:
				return CUT_IN_STATE_QUEST;
				break;
			case NPC_EVENT_TYPE_GIFT:
				return CUT_IN_STATE_GIFT;
				break;
			case NPC_EVENT_TYPE_GUILD_CREATE:
				return CUT_IN_STATE_GUILD_CREATE;
				break;
			case NPC_EVENT_TYPE_GUILD_DISSOLVE:
				return CUT_IN_STATE_GUILD_DISSOLVE;
				break;
			case NPC_EVENT_TYPE_GUILD_ENTER:
				return CUT_IN_STATE_GUILD_CROPS;
				break;
			case NPC_EVENT_TYPE_PORTAL:
				return CUT_IN_STATE_NPC_PORTAL;
				break;
			case NPC_EVENT_TYPE_CHANGE:
				return CUT_IN_STATE_NPC_CHANGE;
				break;
			case NPC_EVENT_TYPE_GUILD_LEVELUP:
				return CUT_IN_STATE_GUILD_LEVEL_UP;
				break;
			case NPC_EVENT_TYPE_GUILD_EMBLEM:
				return CUT_IN_STATE_GUILD_EMBLEM;
				break;
			default:
				return CUT_IN_STATE_RELEASE;
				break;
			}
		}	
	}
}


void SPEventManager::SendCutInStart(UINT32 iNpcID /*= 0*/)
{
	if(g_pCheckManager->IsDBBlock()) {
		
		DXUTOutputDebugString("\tSPEventManager::SendCutInStart DB Working Block\n");
		return;
	}
	
	//		
	CPacket Packet;
	Packet.Add((UINT32)CUTIN_CS_OPEN);	
	Packet.Add((UINT32)iNpcID);

	//이부분 리턴값 문제 있을수 있음...
	g_pNetworkManager->SPPerformMessage(CUTIN_CS_OPEN, 0, (LPARAM)&Packet);

	//DXUTOutputDebugString("Send CutInStart..\n");
	
	m_bSendPacket = true;
}


void SPEventManager::SendCutInEnd(UINT32 iNpcID /*= 0*/)
{
	//		
	CPacket Packet;
	Packet.Add((UINT32)CUTIN_CS_CLOSE);	
	Packet.Add((UINT32)iNpcID);

	//이부분 리턴값 문제 있을수 있음...
	g_pNetworkManager->SPPerformMessage(CUTIN_CS_CLOSE, 0, (LPARAM)&Packet);

	m_bSendPacket = true;

	//DXUTOutputDebugString("Send CutInEnd..\n");

	OnCutInEnd();
}



