
#include "SPCommon.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPWindow.h"
#include "SPInterfaceManager.h"

#include "SPItemAttr.h"
#include "SPItemCluster.h"

//
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"

//
#include "SPEventDEF.h"
#include "SPQuestAttr.h"
#include "SPQuestArchive.h"
#include "SPQuestManager.h"
#include "SPEvent.h"
#include "SPEventManager.h"

#include "SPCheckManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"

extern SPNetworkManager* g_pNetworkManager;


DEFINE_SC_PACKET_PROC(CUTIN_SC_OPEN)
{
	GU_ID			ObjectGUID	= 0;	// 게임오브젝트아이디

	packet->ExtractUINT64(&ObjectGUID);

	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
			return ;
		}
	}

	g_pEventManager->OnCutInStart();
}


DEFINE_SC_PACKET_PROC(CUTIN_SC_CLOSE)
{
	GU_ID			ObjectGUID	= 0;	// 게임오브젝트아이디

	packet->ExtractUINT64(&ObjectGUID);

	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
			return ;
		}
	}

	//g_pEventManager->OnCutInEnd();
}


DEFINE_SC_PACKET_PROC(NPC_SC_PORTAL)
{
	// 실제 포탈은 EVENT_SC_RAISE
	GU_ID				iPCGUID		= 0;		// 게임오브젝트아이디
	GLOBAL_STRING_ID	iErrorID	= 0;		// 성공 0 
	UINT64				iCurrEly	= 0;		// 현재 갱신된 돈 
	TRASH_ITEM			stUsingItem;			// 아이템은 삭제해주어야 한다.
	ZeroMemory(&stUsingItem, sizeof(TRASH_ITEM));		

	packet->ExtractUINT64(&iPCGUID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != iPCGUID) {
			return ;
		}
	}
	
	packet->ExtractUINT32(&iErrorID);
	if(iErrorID) {			//실패
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(iErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str());
		}
		return;
	}

    packet->ExtractUINT64(&iCurrEly);	
	if(g_pGOBManager->GetLocalPlayer()) {		
		g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_ELY, iCurrEly);
	}	

	packet->ExtractStruct(&stUsingItem, sizeof(TRASH_ITEM));	//삭제할 아이템이 있는 경우
	stUsingItem.iContainerSlot.SlotIndex = stUsingItem.iContainerSlot.SlotIndex - 1;
	g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->TrashItem(stUsingItem);
}


DEFINE_SC_PACKET_PROC(NPC_SC_REWARD)
{	
	GU_ID			iPCGUID;		// 게임오브젝트아이디
	GLOBAL_STRING_ID iErrorID;		// 성공 0 
	UINT64			iCurrEly;       //
	UINT8			iNumOfReqItem;
	TRASH_ITEM		stReqItem;
	UINT8			iNumOfRewardItem;
	CONTAINER_ITEM	stItem;
	UINT32			iCurrPVPPoint;	// Current PVP Point mijin 20071129

	packet->ExtractUINT64(&iPCGUID);
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return;

	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != iPCGUID) {
			return ;
		}
	}

	g_pCheckManager->SetDBBlock(false);

	packet->ExtractUINT32(&iErrorID);
	if(iErrorID) {			//실패
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(iErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str());
		}
		return;
	}

	packet->ExtractUINT64(&iCurrEly);
	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_ELY, iCurrEly);
	
	int i = 0;
	packet->ExtractUINT8(&iNumOfReqItem);
	for(i = 0; i < iNumOfReqItem; i++) {
		ZeroMemory(&stReqItem, sizeof(TRASH_ITEM));
		packet->ExtractStruct(&stReqItem, sizeof(TRASH_ITEM));	//삭제할 아이템이 있는 경우
		stReqItem.iContainerSlot.SlotIndex = stReqItem.iContainerSlot.SlotIndex - 1;
		g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->TrashItem(stReqItem);
	}
	
	SPItemAttr* pItemAttr = NULL;
	char szMsg[256];

	packet->ExtractUINT8(&iNumOfRewardItem);
	for(i = 0; i < iNumOfRewardItem; i++) {
		ZeroMemory(&stItem, sizeof(CONTAINER_ITEM));
		packet->ExtractStruct(&stItem, sizeof(CONTAINER_ITEM));
		stItem.SlotInfo.SlotIndex = stItem.SlotInfo.SlotIndex - 1;
		g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->ItemSetting(stItem, false);		
		
		pItemAttr = NULL;
		ZeroMemory(szMsg, 256);
		
		if( g_pItemCluster->GetItemInfo(stItem.Item.iItemID, pItemAttr) ) {				
			D3DXCOLOR color;
			if(pItemAttr->m_bNameColor) {
				color = pItemAttr->GetNameColor();
			}
			else {
				color = g_pCheckManager->GetItemColor(stItem.Item.iRare);
			}
			
			sprintf(szMsg, g_pResourceManager->GetGlobalString(5017006), pItemAttr->m_strName.c_str());

			//if(g_pClientOption->GetOptionStruct()->bItem) {
			g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMsg , (LPARAM)&color );
			//}

			//[7/17/2008 jinssaga] Quest체크
			g_pEventManager->GetQuestManager()->CheckMissionItem(stItem.Item.iItemID);
		}
	}
	
	packet->ExtractUINT32(&iCurrPVPPoint);
	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValue(STATUS_PVP_POINT, iCurrPVPPoint);
}

