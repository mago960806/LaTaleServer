
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



DEFINE_SC_PACKET_PROC(GAMBLE_SC_REQ)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_GAMBLE , SPIM_GAMBLE_RESULT , wParam , lParam );
}
