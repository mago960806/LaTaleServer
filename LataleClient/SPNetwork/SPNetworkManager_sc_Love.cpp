// ***************************************************************
//  SPNetworkManager_sc_Love		Version:  1.0		Date: 2008/11/03
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

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
#include "SPSubGameManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "LT_Error.h"

#include "SPLoveManager.h"

#include "SPNetworkManager.h"

extern SPNetworkManager* g_pNetworkManager;

DEFINE_SC_PACKET_PROC(WEDDING_SC_INFO)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_LOVE , SPIM_LOVE_INFO , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(WEDDING_SC_REQUEST)
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
		pLoveManager->OnWeddingRequest( packet );
}

DEFINE_SC_PACKET_PROC(WEDDING_SC_REQUEST_RECV)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_LOVE , SPIM_LOVE_REQUEST_RECV , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(WEDDING_SC_RESPONSE)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_LOVE , SPIM_LOVE_RESPONSE , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(WEDDING_SC_WEDDING_CANCEL)
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
		pLoveManager->OnWeddingCancel( packet );
}

DEFINE_SC_PACKET_PROC(WEDDING_SC_MOTION_STARTEND)
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
		pLoveManager->OnMotionStartEnd( packet );
}

DEFINE_SC_PACKET_PROC(WEDDING_SC_UPDATEMEMBER)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_LOVE , SPIM_LOVE_UPDATEMEMBER , wParam , lParam );
}

DEFINE_SC_PACKET_PROC(WEDDING_SC_PARTING)
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
		pLoveManager->OnWeddingDivorce(packet);
}

DEFINE_SC_PACKET_PROC(WEDDING_SC_SKILL_USING)
{
	RESULTCODE retCode;
	packet->Extract(&retCode);

	if( retCode != LT_NO_ERROR )
	{
		if( g_pGOBManager->GetLocalPlayer() )
			g_pGOBManager->GetLocalPlayer()->InitSendAction();

		return;
	}

	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_SET_LOVE_SKILL_USING, wParam, lParam);
}


DEFINE_SC_PACKET_PROC( WEDDING_SC_SUMMON_REQUEST	)
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
		pLoveManager->OnRequestRecall(packet);	
}


DEFINE_SC_PACKET_PROC( WEDDING_SC_SUMMON_REQUEST_ERR)
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
		pLoveManager->OnRequestRecallErr(packet);	
}


DEFINE_SC_PACKET_PROC( WEDDING_SC_SUMMON_RESPONSE	)
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
		pLoveManager->OnAcceptRecall(packet);
}


DEFINE_SC_PACKET_PROC( WEDDING_SC_SUMMON_RESPONSE_RECV	)
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
		pLoveManager->OnResultRecall(packet);
}