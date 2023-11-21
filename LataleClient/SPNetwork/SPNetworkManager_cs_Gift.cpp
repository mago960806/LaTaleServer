
#include "SPCommon.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"

//
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayer.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPWindow.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPGiftAttr.h"
#include "SPGiftArchive.h"
#include "SPGiftManager.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"
#include "SPCheckManager.h"

#include "DebugPrint.h"

extern SPNetworkManager* g_pNetworkManager;


DEFINE_CS_PACKET_PROC(GIFT_CS_LIST)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


DEFINE_CS_PACKET_PROC(GIFT_CS_GET)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


DEFINE_SC_PACKET_PROC(GIFT_SC_LIST)
{	
	GU_ID				CharGUID;		// 주체자
	GLOBAL_STRING_ID	iError ;		// 0: 성공
	unsigned int		iNumOfGiftID;	// 받아야 할 gift 갯수
	unsigned __int64	iGiftNO;		// gift key ID					
	unsigned int		iGiftID;		// GIFT.LDT 참조	
	
	packet->ExtractUINT64(&CharGUID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != CharGUID) {
			return ;
		}
	}

	packet->ExtractUINT32(&iError);
	if(iError) // 실패
	{
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(iError)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iError);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );					
		}
		return;
	}

	packet->ExtractUINT32(&iNumOfGiftID);

	if(iNumOfGiftID == 0)
		return;	

	for(unsigned int i = 0; i < iNumOfGiftID; i++) {
		packet->ExtractUINT64(&iGiftNO);			//Key ID		
		packet->ExtractUINT32(&iGiftID);			//Gift Item ID

		g_pEventManager->GetGiftManager()->InsertGift(iGiftID, iGiftNO);
	}

	g_pEventManager->GetGiftManager()->SetGiftLoad(true);

	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_GIFT);
	if( pWindow ) {
		pWindow->SPSendMessage(SPIM_GIFT_LIST_UPDATE, (WPARAM)0, (LPARAM)0);
	}
}


DEFINE_SC_PACKET_PROC(GIFT_SC_GET)
{
	GU_ID					CharGUID;		// 주체자
	GLOBAL_STRING_ID		iError ;		// 0: 성공
	UINT64				iGiftNO;
	UINT64				iCurrMoney;		// 돈갱신
	UINT8				iItemCount;		// 아이템 생성 갯수
	//CONTAINER_ITEM		tContainerItem;	// 생성 아이템 정보	

	 //
	packet->ExtractUINT64(&CharGUID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != CharGUID) {
			 return ;
		}
	}	

	packet->ExtractUINT32(&iError);
	if(iError) // 실패
	{
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(iError)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iError);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
		g_pCheckManager->SetDBBlock(false);
		return;
	}

	packet->ExtractUINT64(&iGiftNO);
	if(g_pEventManager->GetGiftManager()->DeleteGiftItem(iGiftNO) == false) {
		SetTraceLevel(1);
		DebugPrint("Gift Key[%I64d] Delete fail..\n", iGiftNO);
	}
	

	packet->ExtractUINT64(&iCurrMoney);
	if(g_pGOBManager->GetLocalPlayer()) {
		g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_ELY, iCurrMoney);
	}
	
	packet->ExtractUINT8(&iItemCount);	
	if(iItemCount > 0) {
		WPARAM wParam = (WPARAM)&CharGUID;
		LPARAM lParam = (LPARAM)packet;
		g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_CONTAINER_INFO, wParam, lParam);
	}

	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_GIFT);
	if( pWindow ) {
		pWindow->SPSendMessage(SPIM_GIFT_LIST_UPDATE, (WPARAM)0, (LPARAM)0);
	}

	g_pCheckManager->SetDBBlock(false);
}

