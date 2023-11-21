
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

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPWindow.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

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


DEFINE_SC_PACKET_PROC(MISSION_SC_LIST)
{
	GU_ID			ObjectGUID	= 0;	// 게임오브젝트아이디
	int				iCnt = 0;			// 미션 개수
	MISSION			stMission = {0,};	// 미션 내용

	packet->ExtractUINT64(&ObjectGUID);	
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
			return ;
		}
	}
	
	packet->ExtractUINT32((UINT32*)&iCnt);
	for(int i=0; i<iCnt; i++)
	{
		packet->ExtractStruct(&stMission, sizeof(MISSION));
		// 미션 매니저에 추가해야합니다.
		
		//[2005/9/27] 진행중 퀘스트의 현재 상황을 설정
		g_pEventManager->GetQuestManager()->SetPlayerMissionInfo(&stMission);
	}
}


DEFINE_SC_PACKET_PROC(MISSION_SC_UPDATE)
{
	GU_ID			ObjectGUID	= 0;	// 게임오브젝트아이디
	MISSION			stMission = {0,};	// 미션 내용

	packet->ExtractUINT64(&ObjectGUID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
			return ;
		}
	}
	
	packet->ExtractStruct(&stMission, sizeof(MISSION));
	
	//[2005/9/29] 진행중 퀘스트의 미션 업데이트
	g_pEventManager->GetQuestManager()->UpdateMission(&stMission);

}


DEFINE_SC_PACKET_PROC(MISSION_SC_ACTION)
{
	GU_ID			ObjectGUID	= 0;	// 게임오브젝트아이디
	UINT32			uiActionID	= 0;

	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&uiActionID);
}


DEFINE_SC_PACKET_PROC(QUEST_SC_LIST)
{
	GU_ID			ObjectGUID	= 0;	// 게임오브젝트아이디
	int				iCnt = 0;			// 퀘스트 개수
	QUEST			stQuest = {0,};		// 퀘스트 내용

	packet->ExtractUINT64(&ObjectGUID);	
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
			return ;
		}
	}

	////[2005/10/6] 리스트를 한번이라도 받아서 소트한 상황이라면...
	//if(g_pEventManager->GetQuestManager()->IsListSort()) {
	//	return;
	//}

	packet->ExtractUINT32((UINT32*)&iCnt);
	for(int i=0; i<iCnt; i++)
	{
		packet->ExtractStruct(&stQuest, sizeof(QUEST));
		// 퀘스트 매니저에 추가해야합니다.
		
		//[2005/9/27] 퀘스트를 플레이어쪽에 생성하도록 함
		g_pEventManager->GetQuestManager()->SetPlayerQuest(&stQuest, false);
	}	

	//[2005/10/6] 리스트를 처음 받은 경우 리스트를 소트하도록 한다 
	if(g_pEventManager->GetQuestManager()->IsListSort() == false) {
		g_pEventManager->GetQuestManager()->SetListSort();
	}
}


DEFINE_SC_PACKET_PROC(QUEST_SC_ACCEPT)
{
	GU_ID				ObjectGUID = 0;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID = 0;							// 에러아이디
	QUEST				stQuest = {0,};							// 퀘스트 정보

	packet->ExtractUINT64(&ObjectGUID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
			return ;
		}
	}

	g_pEventManager->GetQuestManager()->ResetSendBlock();
	
	packet->ExtractUINT32(&ErrorID);
	if(ErrorID) // 실패
	{
		// 에러시 처리
		// 락상태를 해제해야합니다.
		g_pCheckManager->SetDBBlock(false);
		
		// ErrorID는 GLOBAL_STRING.LDT의 공통에러문구입니다. 4000002
		std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );		
		
		return;
	}

	packet->ExtractStruct(&stQuest, sizeof(QUEST));
	//[2005/9/29] 퀘스트를 플레이어쪽에 생성하도록 함
	g_pEventManager->GetQuestManager()->SetPlayerQuest(&stQuest);
	
	////
	//packet->ExtractUINT32((UINT32*)&iItemCnt);
	//if(QUEST_MAX_ACQUISITION_ITEM < iItemCnt)
	//{
	//	// *** 미결 *** 에러로그
	//	return;
	//}
	//for(int i=0; i<iItemCnt; i++)
	//{
	//	packet->ExtractStruct(&arrItem[i], sizeof(CONTAINER_ITEM));		
	//}
/* 2005.10.04 dhpark 패킷에서 제외하고 ITEM_SC_CONTAINER_INFO로 송신합니다.
	//[2005/9/29] 아이템 처리는 아이템 처리 루틴으로 가야...
	// SPGM_ITEM_SC_CONTAINER_INFOS 로 들어가서 처리됨...
	// 이쪽에 문제 없도록 주의할것!
	WPARAM wParam = (WPARAM)&ObjectGUID;
	LPARAM lParam = (LPARAM)packet;	
	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_CONTAINER_INFOS, wParam, lParam);	
*/
	g_pCheckManager->SetDBBlock(false);				//[2005/11/4] - 예외적으로 여기서 블럭을 해제 해준다...

	////CutIn 해제...
	//g_pEventManager->SetCutInState(CUT_IN_STATE_NULL);

	UINT8			byAcceptType;	// 퀘스트 수락 타입(1:NPC, 2:공유, 4:아이템, 8:이벤트)
	packet->ExtractUINT8( &byAcceptType );

	if( byAcceptType & QUEST_ACCEPTTYPE_NPC == QUEST_ACCEPTTYPE_NPC )
	{
		g_pEventManager->GetQuestManager()->LoadNpcQuest();
	}
	else
	{
		g_pEventManager->GetQuestManager()->DeleteNpcQuest( stQuest.uiQuestID );
	}

	//SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_NPC_QUEST);
	//pWindow->Show();
	//pWindow = g_pInterfaceManager->FindWindow(WIID_QUEST);
	//pWindow->Show();
}


DEFINE_SC_PACKET_PROC(QUEST_SC_CANCEL)
{
	GU_ID			ObjectGUID	= 0;	// 게임오브젝트아이디
	UINT32			uiQuestID = 0;		// 퀘스트아이디

	packet->ExtractUINT64(&ObjectGUID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
			return ;
		}
	}

	g_pEventManager->GetQuestManager()->ResetSendBlock();
	
	packet->ExtractUINT32(&uiQuestID);	
	
	//[2005/9/29]	진행중 퀘스트를 포기하게 함
	g_pEventManager->GetQuestManager()->OnGiveUpQuest(uiQuestID);

	////CutIn 해제...
	//g_pEventManager->SetCutInState(CUT_IN_STATE_NULL);
	
	//SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_QUEST);
	//pWindow->Show();
	//pWindow = g_pInterfaceManager->FindWindow(WIID_NPC_QUEST);
	//pWindow->Show();
}


DEFINE_SC_PACKET_PROC(QUEST_SC_FINISH)
{
	GU_ID				ObjectGUID = 0;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID = 0;							// 에러아이디
	QUEST				stQuest = {0,};							// 퀘스트 정보

	packet->ExtractUINT64(&ObjectGUID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
			return ;
		}
	}

	g_pEventManager->GetQuestManager()->ResetSendBlock();
	
	packet->ExtractUINT32(&ErrorID);
	if(ErrorID) // 실패
	{
		// 에러시 처리
		// 락상태를 해제해야합니다.
		g_pCheckManager->SetDBBlock(false);
		
		// ErrorID는 GLOBAL_STRING.LDT의 공통에러문구입니다. 4000002
		std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );		
		
		return;
	}

	packet->ExtractStruct(&stQuest, sizeof(QUEST));
	g_pEventManager->GetQuestManager()->OnFinishQuest(&stQuest);

	////
	//packet->ExtractUINT32((UINT32*)&iItemCnt);
	//if(QUEST_MAX_REWARD_ITEM < iItemCnt)
	//{
	//	// *** 미결 *** 에러로그
	//	return;
	//}
	//for(int i=0; i<iItemCnt; i++)
	//{
	//	packet->ExtractStruct(&arrItem[i], sizeof(CONTAINER_ITEM));
	//}
/* 2005.10.04 dhpark 패킷에서 제외하고 ITEM_SC_CONTAINER_INFO로 송신합니다.
	//[2005/9/29] 아이템 처리는 아이템 처리 루틴으로 가야...
	// SPGM_ITEM_SC_CONTAINER_INFOS 로 들어가서 처리됨...
	// 이쪽에 문제 없도록 주의할것!
	WPARAM wParam = (WPARAM)&ObjectGUID;
	LPARAM lParam = (LPARAM)packet;	
	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_CONTAINER_INFOS, wParam, lParam);	
*/
	
	////CutIn 해제...
	//g_pEventManager->SetCutInState(CUT_IN_STATE_NULL);

	g_pEventManager->GetQuestManager()->LoadNpcQuest();

	//SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_NPC_QUEST);
	//pWindow->Show();
	//pWindow = g_pInterfaceManager->FindWindow(WIID_QUEST);
	//pWindow->Show();
}

// 퀘스트 공유(공유한 캐릭터용)
//QUEST_SC_SHARE,
// GU_ID			ObjectGUID;					// 게임오브젝트아이디
// GLOBAL_STRING_ID	ErrorID;					// 에러아이디
// char				szRcvCharName[LEN_NAME+1];	// 공유받는 캐릭터명
DEFINE_SC_PACKET_PROC(QUEST_SC_SHARE)
{
	GU_ID				ObjectGUID	= 0;			// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID = 0;				// 에러아이디
	char				szRcvCharName[LEN_NAME+1];	// 공유받는 캐릭터명

	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	packet->ExtractData(szRcvCharName, LEN_NAME+1);	

	if( g_pGOBManager == NULL )
		return;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return;

	char			szBuf[ _MAX_PATH ];
	const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( ErrorID );

	if( pszGlobalString == NULL )
		return;

	if( strlen( pszGlobalString ) <= 0 )
		return;

	sprintf( szBuf , pszGlobalString , szRcvCharName );
	D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szBuf , (LPARAM)&color );
}

// 퀘스트 공유(공유받는 캐릭터용)
//QUEST_SC_SHARE_RECV,
// GU_ID			ObjectGUID;		// 게임오브젝트아이디
// UINT32			uiQuestID;		// 퀘스트아이디
// INT32			iActCharID;		// 공유한 캐릭터DB키(QUEST_CS_ACCEPT를 보낼때 사용해야합니다.)
DEFINE_SC_PACKET_PROC(QUEST_SC_SHARE_RECV)
{
	GU_ID				ObjectGUID	= 0;			// 게임오브젝트아이디
	UINT32				uiQuestID = 0;				// 퀘스트아이디
	INT32				iActCharID = 0;				// 공유한 캐릭터DB키(QUEST_CS_ACCEPT를 보낼때 사용해야합니다.)

	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&uiQuestID);
	packet->ExtractUINT32((UINT32*)&iActCharID);

	if( g_pGOBManager == NULL )
		return;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return;

	if( g_pEventManager != NULL )
	{
		SPQuestManager*	pQuestManager	=	g_pEventManager->GetQuestManager();

		if( pQuestManager != NULL )
		{
			pQuestManager->RecvQuestSharePacket( uiQuestID ,iActCharID );
		}
	}
}
