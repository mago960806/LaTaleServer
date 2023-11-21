
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
#include "SPTeamArchive.h"
#include "SPPlayer.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPCheckManager.h"

#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"

extern SPNetworkManager* g_pNetworkManager;

//////////////////////////////////////////////////////////////////////////
// Server --> Client

DEFINE_SC_PACKET_PROC(ITEM_SC_CONTAINER_INFO )
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_CONTAINER_INFO, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(ITEM_SC_CONTAINER_INFOS )
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_CONTAINER_INFOS, wParam, lParam);
}


DEFINE_SC_PACKET_PROC(ITEM_SC_LOOTING )
{
	GU_ID globalID = 0;
	GLOBAL_STRING_ID ErrorID;

	packet->ExtractUINT64(&globalID);
	packet->ExtractUINT32(&ErrorID);

	if(ErrorID)
	{
		// 에러입니다.
		std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		g_pCheckManager->SetDBBlock(false);
		return;
	}

	//g_pCheckManager->SetDBBlock(false);		//SPLootingBag에서 처리...
	//메시지 처리만...
	GU_ID ActCharGUID;
	LOOTING_ITEM LootingItemInfo;
	UINT8 byRare;
	packet->ExtractUINT64(&ActCharGUID);
	packet->Extract(&LootingItemInfo, sizeof(LOOTING_ITEM));
	packet->ExtractUINT8(&byRare);

	// ActCharGUID가 로컬플레이어와 같으면 자신이 특정아이템을 습득했다고 표시
	// 그렇지 않으면 팀에서 찾아서 누구누구님이 특정아이템을 습득했다고 표시
	//TCHAR szMsg[128];
	char szMsg[256];
	ZeroMemory(szMsg, 256);
	SPItemAttr* pkItemAttr = NULL;

	// 글로벌 스트링 습득 메세지변경 [2/9/2006 AJJIYA]
	// 5007010 -> 5017006
	// 5007011 -> 5017007

	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() == ActCharGUID) {			//로컬 플레이어			
			if( g_pItemCluster->GetItemInfo(LootingItemInfo.iItemID, pkItemAttr) ) {				
				D3DXCOLOR color;
				if(pkItemAttr->m_bNameColor) {
					color = pkItemAttr->GetNameColor();
				}
				else {
					color = g_pCheckManager->GetItemColor(byRare);
				}
				
				sprintf(szMsg, g_pResourceManager->GetGlobalString(5017006), pkItemAttr->m_strName.c_str());
				
				if(g_pClientOption->GetOptionStruct()->bItem){
//					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg, (LPARAM)&color);
					g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMsg , (LPARAM)&color );
				}				
				
				g_pCheckManager->SetDBBlock(false);								//[2005/11/22]
			}
		}
		else if(g_pGOBManager->GetLocalPlayer()->GetTeamActive()) {				//팀 동료
			if(g_pGOBManager->GetLocalPlayer()->GetTeam()->GetCharNameChannel(ActCharGUID, g_pNetworkManager->GetServerID())) {
				std::string strPlayer = g_pGOBManager->GetLocalPlayer()->GetTeam()->GetCharNameChannel(ActCharGUID, g_pNetworkManager->GetServerID());
				if( g_pItemCluster->GetItemInfo(LootingItemInfo.iItemID, pkItemAttr) ) {
					D3DXCOLOR color;
					if(pkItemAttr->m_bNameColor) {
						color = pkItemAttr->GetNameColor();
					}
					else {
						color = g_pCheckManager->GetItemColor(byRare);
					}					
					
					sprintf(szMsg, g_pResourceManager->GetGlobalString(5017007), strPlayer.c_str(), pkItemAttr->m_strName.c_str());
					if(g_pClientOption->GetOptionStruct()->bItem){
//						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg, (LPARAM)&color);
						g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMsg , (LPARAM)&color );
					}
				}
			}			
		}
	}
	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;
	//bool bLocalResult = false;
	g_pGOBManager->SPPerformMessage(SPGM_STAT_SC_LOOTING_ITEM, wParam, lParam); //로컬 플레이어 처리...	
}

DEFINE_SC_PACKET_PROC(ITEM_SC_CHANGE_SLOT)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_CHANGE_SLOT, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(ITEM_SC_TRASH )
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_TRASH, wParam, lParam);
}


DEFINE_SC_PACKET_PROC(ITEM_SC_DESTROY_BY_TIME )
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_DESTROY_BY_TIME, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// divide

DEFINE_SC_PACKET_PROC( ITEM_SC_DIVIDE )
{

	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_DIVIDE, wParam, lParam);

}

DEFINE_SC_PACKET_PROC( NPC_SHOP_SC_BUY )
{
	GU_ID globalID = 0;
	//GLOBAL_STRING_ID ErrorID;

	packet->ExtractUINT64(&globalID);
	
	////
	//packet->ExtractUINT32(&ErrorID);
	//if(ErrorID)
	//{
	//	// 에러입니다.
	//	return;
	//}

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_TRADE_SC_NPC, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(ITEM_SC_ALARM )
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_ALARM, wParam, lParam);
}


DEFINE_SC_PACKET_PROC(ITEM_SC_USING)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_USING, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(ITEM_SC_ENCHANT)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_ENCHANT, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(ITEM_SC_UPGRADE)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_UPGRADE, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(ITEM_SC_ADDTIME)
{
	//GU_ID globalID = 0;
	//int					iItemAddTimeID = 0;	
	//GLOBAL_STRING_ID	iResultError = 0;
	//ITEM_LIFETIME		stItemLifeTime;
	//ZeroMemory(&stItemLifeTime, sizeof(ITEM_LIFETIME));

	//packet->ExtractUINT64(&globalID);	
	//packet->ExtractUINT32((UINT32*)&iItemAddTimeID);	
	//packet->ExtractUINT32(&iResultError);	
	//if(0 == iResultError) // 성공일 경우만

	//{
	//	packet->Extract(&stItemLifeTime, sizeof(ITEM_LIFETIME));
	//}

	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_ADDTIME, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(ITEM_SC_EFFECTUPGRADE)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_EFFECTUPGRADE, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(ITEM_SC_EXPIRE)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);

	WPARAM wParam = (WPARAM)&globalID;	
	LPARAM lParam = (LPARAM)packet;	

	//CONTAINER_SLOT stItemPos;
	//packet->ExtractStruct(&stItemPos, sizeof(CONTAINER_SLOT));
	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_EXPIRE, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(ITEM_SC_ITEMEXP)
{
	GU_ID			globalID = 0;

	//ITEM_EXP		stItemExp;
	//ZeroMemory(&stItemExp, sizeof(ITEM_EXP));

	packet->ExtractUINT64(&globalID);	
	//packet->Extract(&stItemExp, sizeof(ITEM_EXP));

	WPARAM wParam = (WPARAM)&globalID;	
	LPARAM lParam = (LPARAM)packet;	
	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_ITEMEXP, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(ITEM_SC_ADDSP)
{
	GU_ID				globalID = 0;
	
	//GLOBAL_STRING_ID	ErrorID = 0;
	//ITEM_SP				stItemSP;
	//ZeroMemory(&stItemSP, sizeof(ITEM_SP));

	packet->ExtractUINT64(&globalID);
	WPARAM wParam = (WPARAM)&globalID;	
	LPARAM lParam = (LPARAM)packet;	
	
	//packet->ExtractUINT32(&ErrorID);
	//if(0 == ErrorID) // 성공이면
	//{
	//	packet->Extract(&stItemSP, sizeof(ITEM_SP));
	//}
	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_ADDSP, wParam, lParam);
}


DEFINE_SC_PACKET_PROC(ITEM_SC_CHAT)	//[2007/3/28]
{
	GU_ID				globalID = 0;

	// GU_ID				globalID	; // 사용자 아이디
	// GLOBAL_STRING_ID		iError		; // 0 : 성공
	// TRASH_ITEM			UsingItem	; // 사용 된 아이템 위치 정보

	packet->ExtractUINT64(&globalID);
	WPARAM wParam = (WPARAM)&globalID;	
	LPARAM lParam = (LPARAM)packet;	

	//packet->ExtractUINT32(&ErrorID);
	//if(0 == ErrorID) // 성공이면
	//{
	//	packet->Extract(&stItemSP, sizeof(ITEM_SP));
	//}
	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_CHAT, wParam, lParam);
}

//Search
DEFINE_SC_PACKET_PROC( ITEM_SC_WHERE )
{	
	LPARAM lParam = (LPARAM)packet;
	WPARAM wParam = 0;
	
	if ( g_pInterfaceManager != NULL )
	{
		g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND, SPIM_FRIEND_WHERE, wParam, lParam );
	}
}

//Follow and Snatch
DEFINE_SC_PACKET_PROC( ITEM_SC_TRACK )
{
	LPARAM lParam = (LPARAM)packet;
	WPARAM wParam = 0;
	
	if ( g_pInterfaceManager != NULL )
	{
		g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FOLLOW, SPIM_FOLLOW_FAIL, wParam, lParam );
		g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_SNATCH, SPIM_SNATCH_FAIL, wParam, lParam );
	}
}

DEFINE_SC_PACKET_PROC( ITEM_SC_WHERE_COMMAMD_INFOS )
{
	LPARAM lParam = (LPARAM)packet;
	WPARAM wParam = 0;
	
	if ( g_pInterfaceManager != NULL )
	{
		g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND, SPIM_LEAVING_TIMES, wParam, lParam );
	}
	return;
}

DEFINE_SC_PACKET_PROC(ITEM_SC_MIX)	// 아이템조합 [9/12/2007 AJJIYA]
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_ITEM_MIX , SPIM_ITEM_MIX_RESULT , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHAR_SC_ITEMMIX_INFO)	// 아이템조합 정보 [9/12/2007 AJJIYA]
{
	GU_ID				globalID		=	0;
	ITEMMIX_INFO		stItemMixInfo	=	{0,};

	packet->ExtractUINT64(&globalID);
	packet->Extract( &stItemMixInfo , sizeof( ITEMMIX_INFO ) );

	WPARAM wParam = (WPARAM)&globalID;	
	LPARAM lParam = (LPARAM)&stItemMixInfo;

	g_pGOBManager->SPPerformMessage( SPGM_SET_SC_ITEMMIX_INFO, wParam, lParam);
}

// Another PC Equip
DEFINE_SC_PACKET_PROC( ITEM_SC_FIGURE_INFOS_FOR_SINGLE )
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_FIGURE_INFOS_SC_FOR_SINGLE, wParam, lParam);
}




// Storage
DEFINE_SC_PACKET_PROC(STORAGE_SC_INMONEY)
{
	GU_ID ObjectGUID = 0;
	
	//UINT64 uiMoney = 0, uiStorageMoney = 0;
	packet->ExtractUINT64(&ObjectGUID);
	//packet->ExtractUINT64(&uiMoney);
	//packet->ExtractUINT64(&uiStorageMoney);

	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_STORAGE_SC_IN_MONENY, wParam, lParam);

	////
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_ELY, uiMoney);
	//	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_STORAGE_ELY, uiStorageMoney);
	//}	
}

DEFINE_SC_PACKET_PROC(STORAGE_SC_OUTMONEY)
{
	GU_ID ObjectGUID = 0;
	//UINT64 uiMoney = 0, uiStorageMoney = 0;
	packet->ExtractUINT64(&ObjectGUID);
	//packet->ExtractUINT64(&uiMoney);
	//packet->ExtractUINT64(&uiStorageMoney);

	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;

	g_pGOBManager->SPPerformMessage(SPGM_STORAGE_SC_OUT_MONENY, wParam, lParam);

	//if(g_pGOBManager->GetLocalPlayer()) {
	//	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_ELY, uiMoney);
	//	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_STORAGE_ELY, uiStorageMoney);
	//}
}


// Looting
DEFINE_SC_PACKET_PROC(ITEM_SC_LOOTING_LIST )
{
	GU_ID		globalID = 0;
	CLASS_ID	classID = CLASS_ID_NULL;
	INSTANCE_ID instanceID = 0;

	packet->ExtractUINT64(&globalID);
	//GetLocalID(globalID,classID,instanceID);
	//globalID = GetGlobalID(CLASS_ID_LOOTING_BAG, instanceID);

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_GOB_ENTER,	wParam, lParam);


	/*
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	// local player

	//for test
	{

		GU_ID iMobID = 0;
		UINT8 iNumOfItems = 0;
		ITEM items[MAX_MOB_LOOT_ITEM] = {0,};
		LT_POSITION tPos={0,};

		packet->ExtractUINT64(&iMobID);	// local player
		packet->ExtractStruct(&tPos, sizeof(tPos));	// �ġ



		packet->ExtractUINT8(&iNumOfItems);	// local player
		for(int i = 0; i < iNumOfItems ; ++i )
		{
			UINT8 iSlot = 0;
			packet->ExtractUINT8(&iSlot);
			packet->ExtractStruct(&items[i], sizeof(ITEM));	// local player
		}


	}

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	//g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_ALARM, wParam, lParam);
	*/
}

DEFINE_SC_PACKET_PROC(ITEM_SC_FIGURE_INFOS)
{
	assert(0 && "Do not support figure infos");
	return;

	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	
	
	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_FIGURE_INFOS, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(ITEM_SC_FIGURE_ON)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_FIGURE_ON, wParam, lParam);
}
DEFINE_SC_PACKET_PROC(ITEM_SC_FIGURE_OFF)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_FIGURE_OFF, wParam, lParam);
}
DEFINE_SC_PACKET_PROC(ITEM_SC_TOGGLE_USE_WEAPON_SET)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	
	RESULTCODE iError;
	unsigned char ucActiveSlot = 0;
	packet->ExtractUINT8(&iError);
	if( iError != 0 )
	{
		if( g_pGOBManager->GetLocalPlayer() )
			g_pGOBManager->GetLocalPlayer()->SetWeaponSwap(false);

		return;
	}

	packet->ExtractUINT8(&ucActiveSlot);
	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_TOGGLE_USE_WEAPON_SET, wParam, ucActiveSlot + 1);
}


//////////////////////////////////////////////////////////////////////////
// Client --> Server 
DEFINE_CS_PACKET_PROC(ITEM_CS_LOOTING )
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(ITEM_CS_CHANGE_SLOT )
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(ITEM_CS_TRASH )
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(ITEM_CS_USING )		//[2005/5/23] - jinhee
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(ITEM_CS_DIVIDE)		//[2005/9/7]
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(ITEM_CS_CHAT)			//[2007/3/28]
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

//Search	[2008.9.31]
DEFINE_CS_PACKET_PROC( ITEM_CS_WHERE )
{
	if( NULL == (CPacket*)lParam )
	{
		return -1;
	}
	GetPacketHandler()->PostMsg( (CPacket*)lParam );
	
	return 0;
}

//Follow and Snatch [2008.10.10]
DEFINE_CS_PACKET_PROC( ITEM_CS_TRACK )
{
	if ( NULL == (CPacket*)lParam )
	{
		return -1;
	}
	GetPacketHandler()->PostMsg( (CPacket*)lParam );
	
	return 0;
}

DEFINE_CS_PACKET_PROC(ITEM_CS_TOGGLE_USE_WEAPON_SET)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

//NPC TRADE
DEFINE_CS_PACKET_PROC(NPC_SHOP_CS_SELL)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(NPC_SHOP_CS_BUY)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


//STORAGE
DEFINE_CS_PACKET_PROC(STORAGE_CS_INMONEY)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(STORAGE_CS_OUTMONEY)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
//PC TRADE
DEFINE_CS_PACKET_PROC(TRADE_CS_REQUEST)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(TRADE_CS_RESPONSE)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(TRADE_CS_PUSHMONEY)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(TRADE_CS_PUSHITEM)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(TRADE_CS_POPITEM)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


DEFINE_CS_PACKET_PROC(TRADE_CS_WAIT)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


DEFINE_CS_PACKET_PROC(TRADE_CS_ACCEPT)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(TRADE_CS_CANCEL)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Another PC Equip Item
DEFINE_CS_PACKET_PROC(ITEM_CS_FIGURE_INFOS_FOR_SINGLE)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


DEFINE_CS_PACKET_PROC(ITEM_CS_ADDTIME)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


DEFINE_CS_PACKET_PROC(ITEM_CS_ADDSP)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(ITEM_CS_EFFECTUPGRADE)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}
