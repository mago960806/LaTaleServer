
#include "SPCommon.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPWindow.h"
#include "SPInterfaceManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

//
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayer.h"
#include "GlobalDefines_Share.h"
#include "SPGuildArchive.h"
#include "SPSubGameManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"
#include "SPPvpManager.h"

extern SPNetworkManager* g_pNetworkManager;



DEFINE_SC_PACKET_PROC(PVP_SC_REGIST)
{
	//GU_ID				ObjectGUID;							// 게임오브젝트아이디
	//GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	//	
	//packet->ExtractUINT64(&ObjectGUID);	
	//packet->ExtractUINT32(&ErrorID);	

	if( g_pkPvpManager )
		g_pkPvpManager->OnPVPScEnterRegist(packet) ;
}

DEFINE_SC_PACKET_PROC(PVP_SC_CANCEL)
{
	//GU_ID				ObjectGUID;							// 게임오브젝트아이디
	//GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	//	
	//packet->ExtractUINT64(&ObjectGUID);	
	//packet->ExtractUINT32(&ErrorID);	

	if( g_pkPvpManager )
		g_pkPvpManager->OnPVPScEnterCancel(packet) ;
}

DEFINE_SC_PACKET_PROC(PVP_SC_IN)
{
	//GU_ID				ObjectGUID;							// 게임오브젝트아이디
	//GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	//UINT32				uiIndunID;							// PVP방 인스턴트 아이디
	//UINT16				wServerID;							// 인던 서버아이디
	//	
	//// OnPvpMatchOk
	//packet->ExtractUINT64(&ObjectGUID);	
	//packet->ExtractUINT32(&ErrorID);
	//if(0 == ErrorID) // 성공이면
	//{
	//	packet->ExtractUINT32(&uiIndunID);	
	//	packet->ExtractUINT16(&wServerID);	
	//}

	if( g_pkPvpManager )
		g_pkPvpManager->OnPVPScMatchOK(packet) ;
}

DEFINE_SC_PACKET_PROC(PVP_SC_LOADSTATUS)
{
	//GU_ID				ObjectGUID;							// 게임오브젝트아이디
	//GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	//UINT32				iMemberCnt;							// 대기자수
	//PVPLOADSTATUS		stPVPLoadStatus;					// 대기자의 PVP로드상태

	//memset(&stPVPLoadStatus, 0, sizeof(PVPLOADSTATUS));
	//	
	//packet->ExtractUINT64(&ObjectGUID);	
	//packet->ExtractUINT32(&iMemberCnt);
	//for(int i=0; i<iMemberCnt; i++)
	//{
	//	packet->ExtractStruct(&stPVPLoadStatus, sizeof(PVPLOADSTATUS));
	//}

	g_pkPvpManager->OnPVPScLoadStatus(packet) ;
}

DEFINE_SC_PACKET_PROC(PVP_SC_START)
{
	//GU_ID				ObjectGUID;							// 게임오브젝트아이디
	//UINT32				uiLDTID;							// PVP LDT 아이디
	//	
	//packet->ExtractUINT64(&ObjectGUID);	
	//packet->ExtractUINT32(&uiLDTID);	

	if( g_pkPvpManager )
		g_pkPvpManager->OnPVPScStart(packet) ;
}

DEFINE_SC_PACKET_PROC(PVP_SC_OUT)
{
	//GU_ID				ObjectGUID;							// 게임오브젝트아이디
	//	
	//packet->ExtractUINT64(&ObjectGUID);	

	if( g_pkPvpManager )
		g_pkPvpManager->OnPVPScExit(packet) ;
}

DEFINE_SC_PACKET_PROC(PVP_SC_RANK)
{
	//GU_ID			ObjectGUID;							// 게임오브젝트아이디
	//UINT32			iMemberCnt;							// 대기자수
	//PVPRANK			stPVPRank;							// 대기자의 PVP순위정보

	//memset(&stPVPRank, 0, sizeof(PVPRANK));

	//packet->ExtractUINT64(&ObjectGUID);
	//packet->ExtractUINT32(&iMemberCnt);
	//for(int i=0; i<iMemberCnt; i++) // 멤버수만큼 순위정보를 받아서 처리해야합니다.
	//{
	//	packet->ExtractStruct(&stPVPRank, sizeof(PVPRANK));
	//}

	if( g_pkPvpManager )
		g_pkPvpManager->OnPVPScRank(packet) ;
}

DEFINE_SC_PACKET_PROC(PVP_SC_FINISH)
{
	//GU_ID				ObjectGUID;							// 게임오브젝트아이디
	//UINT32			iMemberCnt;							// 대기자수
	//PVPRANK			stPVPRank;							// 대기자의 PVP순위정보

	//memset(&stPVPRank, 0, sizeof(PVPRANK));

	//packet->ExtractUINT64(&ObjectGUID);
	//packet->ExtractUINT32(&iMemberCnt);
	//for(int i=0; i<iMemberCnt; i++) // 멤버수만큼 순위정보를 받아서 처리해야합니다.
	//{
	//	packet->ExtractStruct(&stPVPRank, sizeof(PVPRANK));
	//}
	g_pkPvpManager->OnPVPScResult(packet) ;
}
