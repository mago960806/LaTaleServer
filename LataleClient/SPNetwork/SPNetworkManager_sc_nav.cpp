// SPNetworkManager_sc_nav.cpp

#include "SPCommon.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"

#include <WinSock2.h>
#include "LT_Error.h"
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "DebugPrint.h"

extern SPNetworkManager* g_pNetworkManager;

// 人物进入场景 ID
DEFINE_SC_PACKET_PROC(NAV_SC_INSIGHT_GUID)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;	

	g_pSubGameManager->SPPerformMessage(SPGM_GOB_ENTER_ID,	wParam, lParam);
	
}

//  努贰胶..
DEFINE_SC_PACKET_PROC(NAV_SC_INSIGHT)
{
	GU_ID		globalID = 0;
	CLASS_ID	classID = CLASS_ID_NULL;
	INSTANCE_ID instanceID = 0;

	packet->ExtractUINT64(&globalID);
	GetLocalID(globalID,classID,instanceID);

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pSubGameManager->SPPerformMessage(SPGM_GOB_ENTER,		wParam, lParam);
	
}


//DEFINE_SC_PACKET_PROC(NAV_SC_INSIGHT_EX)
//{
//	assert(0);
//}


DEFINE_SC_PACKET_PROC(NAV_SC_OUTOFSIGHT)
{
	GU_ID globalID = 0;
	//CLASS_ID classID = CLASS_ID_NULL;
	//INSTANCE_ID instanceID = 0;

	packet->ExtractUINT64(&globalID);
	//GetLocalID(globalID,classID,instanceID);

	g_pSubGameManager->SPPerformMessage(SPGM_GOB_LEAVE,	(WPARAM)&globalID);

}

DEFINE_SC_PACKET_PROC(ACTION_SC_COMMAND)		// MOVE/STOP/JUMP/ATTACK
{
	RESULTCODE retCode;
	packet->Extract(&retCode);

	if(retCode != LT_NO_ERROR)
	{
		if( g_pGOBManager->GetLocalPlayer() )
			g_pGOBManager->GetLocalPlayer()->InitSendAction();

#if defined(_DEBUG)
		DebugPrint("ActionCommand Failed.\n");
#endif
		return;
	}

	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	g_pSubGameManager->SPPerformMessage(SPGM_GOB_MOVE, wParam, lParam);
}
