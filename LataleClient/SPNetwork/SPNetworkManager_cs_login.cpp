// SPNetworkManager_cs_login.cpp

#include "SPCommon.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"

extern SPNetworkManager* g_pNetworkManager;


//DEFINE_CS_PACKET_PROC(AUTH_CS_LOGIN)
//{
//	if((CPacket*)lParam == NULL) return -1;
//	GetPacketHandler()->PostMsg((CPacket*)lParam);
//
//	return 0;
//}

DEFINE_CS_PACKET_PROC(USER_CS_IN)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(USER_CS_MOVE)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

//DEFINE_CS_PACKET_PROC(WORLD_CS_SELECT)
//{
//	if((CPacket*)lParam == NULL) return -1;
//	GetPacketHandler()->PostMsg((CPacket*)lParam);
//
//	return 0;
//}

DEFINE_CS_PACKET_PROC(CHANNEL_CS_SELECT)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(WORLD_CS_ENTER)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(WORLD_CS_LEAVE)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}
