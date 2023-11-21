
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

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPWindow.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPGiftAttr.h"
#include "SPGiftArchive.h"
#include "SPGiftManager.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"
#include "SPCheckManager.h"

#include "DebugPrint.h"

extern SPNetworkManager* g_pNetworkManager;



DEFINE_CS_PACKET_PROC(MAILBOX_CS_GETLIST)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


DEFINE_CS_PACKET_PROC(MAILBOX_CS_SEND)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


DEFINE_CS_PACKET_PROC(MAILBOX_CS_DELETE)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}


DEFINE_CS_PACKET_PROC(MAILBOX_CS_GETBODY)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

DEFINE_CS_PACKET_PROC(MAILBOX_CS_MOVEATTACHMENT)
{
	if((CPacket*)lParam == NULL) return -1;
	GetPacketHandler()->PostMsg((CPacket*)lParam);

	return 0;
}

