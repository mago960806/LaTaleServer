
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
#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"
//
//extern SPNetworkManager* g_pNetworkManager;


DEFINE_SC_PACKET_PROC(ITEM_SC_BASIC_FIGURE_CHANGE)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	g_pInterfaceManager->SPChildWndSendMessage(WIID_BEAUTYSHOP, SPIM_BEAUTYSHOP_FIGURECHANGE, wParam, lParam);
		
}
