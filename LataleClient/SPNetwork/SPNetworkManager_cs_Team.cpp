
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

extern SPNetworkManager* g_pNetworkManager;


// ÆÀÃÊ´ë
// TEAM_CS_REQUEST = 550,
// char				szRcvCharName[LEN_NAME+1];	// ´ë»óÀÚÀÌ¸§
DEFINE_CS_PACKET_PROC(TEAM_CS_REQUEST)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

// ÆÀÂü¿©ÀÀ´ä
// TEAM_CS_JOIN,
// char				szActCharName[LEN_NAME+1];	// ¿äÃ»ÀÚÀÌ¸§
DEFINE_CS_PACKET_PROC(TEAM_CS_JOIN)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

// ÆÀÅ»Åð
// TEAM_CS_SECEDE,
DEFINE_CS_PACKET_PROC(TEAM_CS_SECEDE)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

// °­Åð
// TEAM_CS_EXPEL,
// GU_ID			RcvCharGUID;				// °­ÅðÀÚ
DEFINE_CS_PACKET_PROC(TEAM_CS_EXPEL)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

// ÆÀÀåÀ§ÀÓ
// TEAM_CS_CHANGECAPTAIN,
// GU_ID			RcvCharGUID;				// »õÆÀÀå
DEFINE_CS_PACKET_PROC(TEAM_CS_CHANGECAPTAIN)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

// ·çÆÃ·êº¯°æ
// TEAM_CS_CHANGELOOTINGRULE,
// UINT8			byLottingRule;				// »õ·çÆÃ·ê
DEFINE_CS_PACKET_PROC(TEAM_CS_CHANGELOOTINGRULE)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}
