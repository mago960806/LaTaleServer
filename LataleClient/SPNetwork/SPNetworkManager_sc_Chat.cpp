
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



DEFINE_SC_PACKET_PROC(CHATROOM_SC_CREATE)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_CREATE , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_ROOMINFO_CHECK)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_ROOM_CHECK , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_JOIN)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_JOIN , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_MODIFY)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_TALKING_ROOM , SPIM_CHATTINGROOM_MODIFY , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_OUT)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_TALKING_ROOM , SPIM_CHATTINGROOM_OUT , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_FORCEOUT)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_TALKING_ROOM , SPIM_CHATTINGROOM_FORCEOUT , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_CHANGEMASTER)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_TALKING_ROOM , SPIM_CHATTINGROOM_CHANGEMASTER , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_BROADCAST_UNITED_MESSAGE)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_TALKING_ROOM , SPIM_CHATTINGROOM_MESSAGE , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_MEMBERLIST)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_TALKING_ROOM , SPIM_CHATTINGROOM_MEMBERLIST , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_ADVERT)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_TALKING_ROOM , SPIM_CHATTINGROOM_ADVERT , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_ROOMLIST)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_ROOMLIST , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_USERLIST)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_USERLIST , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_INVITE_REQ)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_INVITE_REQ , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_INVITE_ANS)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_INVITE_ANS , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_OTHERPC_INFO)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_OTHERPC_INFO , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(CHATROOM_SC_GET_MYSTYLE)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_GET_MYSTYLE , wParam , lParam );
}
