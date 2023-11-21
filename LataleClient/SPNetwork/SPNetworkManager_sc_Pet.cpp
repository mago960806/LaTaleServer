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

DEFINE_SC_PACKET_PROC(PET_SC_GET_INFO)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;	

	//g_pGOBManager->SPPerformMessage(SPGM_PET_SETINFO,	wParam, lParam);
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PET_SETINFO, lParam );
}

DEFINE_SC_PACKET_PROC(PET_SC_OPEN_PET_SHOP)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PETSHOP_SC_OPEN, lParam);
}

DEFINE_SC_PACKET_PROC(PET_SC_GET_PET_SHOP_INFO)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PETSHOP_SC_IN, lParam);
}

DEFINE_SC_PACKET_PROC(PET_SC_ENTER_PET_SHOP)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;	

	;//g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PETSHOP_SC_IN, lParam);
}

DEFINE_SC_PACKET_PROC(PET_SC_LEAVE_PET_SHOP)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PETSHOP_SC_OUT, lParam);
}

DEFINE_SC_PACKET_PROC(PET_SC_UPDATE_PET_SHOP_ITEM)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PETSHOP_SC_UPDATEITEM, lParam);
}

DEFINE_SC_PACKET_PROC(PET_SC_BUY_ITEM)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PETSHOP_SC_BUY, lParam);
}

DEFINE_SC_PACKET_PROC(PET_SC_CLOSE_PET_SHOP)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PETSHOP_SC_CLOSE, lParam);
}

DEFINE_SC_PACKET_PROC(PET_SC_LEARN_SKILL)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PET_LEARN_SKILL, lParam);
}

DEFINE_SC_PACKET_PROC(PET_SC_DISCARD_SKILL)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PET_DISCARD_SKILL, lParam);
}

DEFINE_SC_PACKET_PROC(PET_SC_PET_ATTR_CHANGE)
{
	GU_ID				ObjectGUID;
	packet->ExtractUINT64(&ObjectGUID);	

	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;
	g_pGOBManager->SPPerformMessage(SPGM_PET_UPDATEINFO, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(PET_SC_PET_TAKE_OFF)
{
	GU_ID				ObjectGUID;
	packet->ExtractUINT64(&ObjectGUID);	

	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;
	g_pGOBManager->SPPerformMessage(SPGM_PET_TAKEOFF, wParam, lParam);
}
DEFINE_SC_PACKET_PROC(PET_SC_ADD_SSP)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PET_ADD_SSP, lParam);
}
DEFINE_SC_PACKET_PROC(PET_SC_CHANGE_NAME)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PET_CHANGE_NAME, lParam);
}
DEFINE_SC_PACKET_PROC(PET_SC_SHOUT)
{
	GU_ID				ObjectGUID;
	packet->ExtractUINT64(&ObjectGUID);	

	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;
	g_pGOBManager->SPPerformMessage(SPGM_PET_SHOUT, wParam, lParam);
}
DEFINE_SC_PACKET_PROC(PET_SC_PET_SHOP_CLOSED)
{
	WPARAM wParam = (WPARAM)0;
	LPARAM lParam = (LPARAM)packet;
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_PET_SHOP_FORCE_CLOSED, lParam);
}