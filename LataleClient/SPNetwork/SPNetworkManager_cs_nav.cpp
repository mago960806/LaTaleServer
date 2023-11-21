// SPNetworkManager_cs_nav.cpp

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


DEFINE_CS_PACKET_PROC(GAME_CS_START)
{
	//if((CPacket*)lParam == NULL) return -1;

	CPacket Packet;
	Packet.Add((UINT32)GAME_CS_START);
	
	GetPacketHandler()->PostMsg(&Packet);

	return 0;
}


DEFINE_CS_PACKET_PROC(ACTION_CS_COMMAND)		// MOVE/STOP/JUMP/ATTACK
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(SKILL_CS_USING)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(ITEMSKILL_CS_SELL)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(SKILL_CS_LVUP)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(CHAR_CS_REVIVAL)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC (CHAR_CS_PVP_REVIVAL)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(CHAR_CS_SET_QUICK_SLOT)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(CHAT_CS_MSG)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(MONEY_CS_CHAT)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(CASHSHOP_CS_GET_QUICK_BUY_INFO)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(CASHSHOP_CS_QUICK_BUY_BUY_ITEM)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(NAV_CS_INSIGHT_GUID)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

// Another PC Info Request
DEFINE_CS_PACKET_PROC(BLOCK_CS_ISBLOCKED)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

// greener guide fetch award req
DEFINE_CS_PACKET_PROC(GREENERGUIDE_CS_FETCHAWARD)
{
	if ((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);
	return 0;
}
