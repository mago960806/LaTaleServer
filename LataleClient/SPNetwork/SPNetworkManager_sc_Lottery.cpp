
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

extern SPNetworkManager* g_pNetworkManager;



DEFINE_SC_PACKET_PROC(LOTTERY_SC_START)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_LOTTERY , SPIM_LOTTERY_START , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(LOTTERY_SC_UPDATE)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_LOTTERY , SPIM_LOTTERY_UPDATE , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(LOTTERY_SC_CHOICE)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_LOTTERY , SPIM_LOTTERY_CHIOCE , wParam , lParam );
}

/////////////////////////////////////////////////////////////
DEFINE_SC_PACKET_PROC( GAMBLE_SC_GET_STATUS	)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_LOTTREY_STATUS, wParam, lParam);

}

DEFINE_SC_PACKET_PROC( GAMBLE_SC_UPDATE	)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_LOTTREY_UPDATE, wParam, lParam);

}

DEFINE_SC_PACKET_PROC( GAMBLE_SC_BUY	)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_LOTTREY_BUY, wParam, lParam);

}

DEFINE_SC_PACKET_PROC( GAMBLE_SC_UN_BUY	)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_LOTTREY_UNBUY, wParam, lParam);

}

DEFINE_SC_PACKET_PROC( GAMBLE_SC_ACCEPT_ITEM	)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_LOTTREY_ACCEPT, wParam, lParam);

}

DEFINE_SC_PACKET_PROC( GAMBLE_SC_ACCEPT_SECOND_ITEM	)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_LOTTREY_ACCEPT_SECOND, wParam, lParam);

}

DEFINE_SC_PACKET_PROC( GAMBLE_SC_ENTER_GAMBLE	)
{
	WPARAM wParam = 1;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_LOTTREY_IN_RECEIVE, wParam, lParam);

}

DEFINE_SC_PACKET_PROC( GAMBLE_SC_LEAVER_GAMBLE	)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_LOTTREY_IN_RECEIVE, wParam, lParam);
}

DEFINE_SC_PACKET_PROC( GAMBLE_SC_INITIAL			)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_LOTTREY_INITIAL, wParam, lParam);
}

DEFINE_SC_PACKET_PROC( GAMBLE_SC_BROADCAST_GETTED_ITEM	)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pInterfaceManager)
		g_pInterfaceManager->SPChildWndSendMessage( WIID_LOTTERY, SPIM_LOTTERY_BROADCAST, wParam, lParam);
}

DEFINE_SC_PACKET_PROC( GAMBLE_SC_GET_NAME	)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pInterfaceManager)
		g_pInterfaceManager->SPChildWndSendMessage( WIID_LOTTERY, SPIM_LOTTERY_SETNAME, wParam, lParam);
}

