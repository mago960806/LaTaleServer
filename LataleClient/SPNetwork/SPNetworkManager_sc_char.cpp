// SPNetworkManager_sc_char.cpp

#include <WinSock2.h>
#include "LT_Error.h"
#include "Packet.h"
#include "PacketHandler.h"

#include "SPCommon.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"

#include "SPNetworkManager.h"

extern SPNetworkManager* g_pNetworkManager;

DEFINE_SC_PACKET_PROC(CHAR_SC_GETLIST)
{
	//RESULTCODE		resultCode;
	//packet->Extract(&resultCode);

	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CHAR_SC_GETLIST, wParam, lParam);
}
DEFINE_SC_PACKET_PROC(CHAR_SC_CHECK_CHARNAME)
{
	//RESULTCODE		resultCode;
	//packet->Extract(&resultCode);

	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CHAR_SC_CHECK_CHARNAME, wParam, lParam);
}
DEFINE_SC_PACKET_PROC(CHAR_SC_CREATE)
{
	//RESULTCODE		resultCode;
	//packet->Extract(&resultCode);

	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CHAR_SC_CREATE, wParam, lParam);
}
DEFINE_SC_PACKET_PROC(CHAR_SC_DELETE)
{
	//RESULTCODE		resultCode;
	//packet->Extract(&resultCode);

	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CHAR_SC_DELETE, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CHAR_SC_SELECT)
{
	RESULTCODE		resultcode;	
	packet->Extract(&resultcode);

	WPARAM wparam = resultcode;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CHAR_SC_SELECT, wparam, lParam);
}

DEFINE_SC_PACKET_PROC(CHAR_SC_CHARINFO)
{
	RESULTCODE		resultCode;
	packet->Extract(&resultCode);

	if(resultCode != LT_NO_ERROR)
		return;

	GU_ID guid;
	packet->Extract(&guid);

	CLASS_ID ClassID;
	INSTANCE_ID InstanceID;
	GetLocalID(guid, ClassID, InstanceID);

	unsigned int uiMessage;
	WPARAM wParam;
	LPARAM lParam;

	CHARACTER_INFO2 CharInfo;
	MONSTER_INFO MonsterInfo;
	if( InstanceID == CLASS_ID_AVATAR )
	{
		packet->ExtractStruct(&CharInfo, sizeof(CHARACTER_INFO2));
		lParam = (LPARAM)&CharInfo;
		uiMessage = SPGM_SET_SC_CHARINFO;
	}
	else
	{
		packet->ExtractStruct(&MonsterInfo, sizeof(MONSTER_INFO));
		lParam = (LPARAM)&MonsterInfo;
		uiMessage = SPGM_SET_SC_MONSTERINFO;
	}

	wParam = (WPARAM)&guid;

	if(g_pSubGameManager)
		g_pGOBManager->SPPerformMessage(uiMessage, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CHAR_SC_ACTION)
{
	//RESULTCODE retCode;
	//packet->Extract(&retCode);

	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	//g_pGOBManager->SPPerformMessage(CHAR_SC_ACTION, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CHAR_SC_MOVING_STATUS_INFO)
{
	//RESULTCODE retCode;
	//packet->Extract(&retCode);

	GU_ID guid;
	MOVING_STATUS MovingStatus;
	packet->Extract(&guid);
	packet->ExtractStruct(&MovingStatus, sizeof(MOVING_STATUS));

	WPARAM wParam = (WPARAM)&guid;
	LPARAM lParam = (LPARAM)&MovingStatus;

	g_pGOBManager->SPPerformMessage(SPGM_SET_SC_MOVING_STATUS, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CHAR_SC_FACING)
{
	GU_ID guid;
	unsigned char ucFacing;

	packet->Extract(&guid);
	packet->Extract(&ucFacing);
	
	WPARAM wParam = (WPARAM)&guid;
	LPARAM lParam = ucFacing;

	g_pGOBManager->SPPerformMessage(SPGM_SETFACING, wParam, lParam);
}


// Inventory Container Slot Extend 
DEFINE_SC_PACKET_PROC(CHAR_SC_CONTAINER_INFOS)
{
	GU_ID globalID = 0;

	packet->Extract(&globalID);

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;

	g_pGOBManager->SPPerformMessage(SPGM_SET_CONTAINER_INFO, wParam, lParam);
}

