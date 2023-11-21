#include "SPCommon.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"

////
//#include "SPGameObject.h"
//#include "SPGOBStatus.h"
//#include "SPGOBClusterDef.h"
//#include "SPGOBCluster.h"
//#include "SPGobModelUnit.h"
//#include "SPAvatarModelUnitDef.h"
//#include "SPAvatarModelUnit.h"
//#include "SPPlayer.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"

#include "SPCashTradeManager.h"

extern SPNetworkManager* g_pNetworkManager;


DEFINE_CS_PACKET_PROC(CASHSHOP_CS_VERSION)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


DEFINE_CS_PACKET_PROC(CASHSHOP_CS_PRICE)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(CASHSHOP_CS_BUY)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


DEFINE_CS_PACKET_PROC(CASHSHOP_CS_BUY_PRESENT)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(CASHSHOP_CS_WISH_ADD)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

