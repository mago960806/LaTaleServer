// SPNetworkManager_sc_battle.cpp

#include "SPCommon.h"
#include "DebugPrint.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBClusterDef.h"
#include "SPGOBManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPMouseCursor.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPNetworkManager.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "LT_Error.h"

#include "lgclientlog.h"

extern SPNetworkManager* g_pNetworkManager;


DEFINE_SC_PACKET_PROC(BATTLE_SC_ATTACKED)
{
	RESULTCODE retCode;
	packet->Extract(&retCode);

	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	// wParam에 게임오브젝트 인스턴스 아이디가 필요합니다.
	g_pGOBManager->SPPerformMessage(SPGM_SET_SC_DAMAGE_FX, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(BATTLE_SC_HEALING)
{
	RESULTCODE retCode;
	packet->Extract(&retCode);

	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	// wParam에 게임오브젝트 인스턴스 아이디가 필요합니다.
	g_pGOBManager->SPPerformMessage(SPGM_SET_SC_HEALING_FX, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(ACTION_SC_KNOCKBACK)
{
	RESULTCODE retCode;
	packet->Extract(&retCode);

	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	
	
	g_pGOBManager->SPPerformMessage(SPGM_SET_KNOCKBACK, wParam, lParam);
}


DEFINE_SC_PACKET_PROC( SKILL_SC_INFOS		)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_SET_SKILL_LIST, wParam, lParam);
}

DEFINE_SC_PACKET_PROC( SKILL_SC_INIT		)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage( SPGM_SET_SKILL_INIT, wParam, lParam );
}

DEFINE_SC_PACKET_PROC( SKILL_SC_USING		)
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

	g_pGOBManager->SPPerformMessage(SPGM_SET_SKILL_USING, wParam, lParam);
}

DEFINE_SC_PACKET_PROC( ITEMSKILL_SC_USING		)
{
	GLOBAL_STRING_ID	ErrorID = 0;
	GU_ID				globalID = 0;
	
	packet->ExtractUINT64(&globalID);
	packet->ExtractUINT32(&ErrorID);

	if( ErrorID != 0 )
	{
		if( g_pGOBManager->GetLocalPlayer() )
			g_pGOBManager->GetLocalPlayer()->InitSendAction();

		return;
	}

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_SET_ITEM_SKILL_USING, wParam, lParam);
}

DEFINE_SC_PACKET_PROC( SKILL_SC_LVUP		)
{
	// RESULTCODE		resultCode;
	RESULTCODE retCode;
	packet->Extract(&retCode);

	// GU_ID			iInstanceID;
//	GU_ID globalID = 0;
//	packet->ExtractUINT64(&globalID);	

	// short			iSkillFreePoint;
	// CHAR_SKILL		SkillData;

	WPARAM wParam = (WPARAM)&retCode;
	LPARAM lParam = (LPARAM)packet;	
	
	g_pInterfaceManager->SPChildWndSendMessage( WIID_SKILL , SPIM_GET_SKILL_LVUP , wParam , lParam );	// AJJIYA [10/25/2005]

	//[2005/9/21] 스탯 변경시마다 사용가능 아이템 설정 변경을 위함...
	if( g_pGOBManager->GetLocalPlayer()) {	
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_ITEM_REFRESH_GRID);
		g_pGOBManager->SPPerformMessage(SPGM_REFRESH_NAME_COLOR);
	}
}

DEFINE_SC_PACKET_PROC( SKILL_SC_LEARN		)
{
	WPARAM wParam = (WPARAM)packet;
	LPARAM lParam = (LPARAM)packet;	

	g_pInterfaceManager->SPChildWndSendMessage( WIID_SKILL , SPIM_GET_SKILL_LEARN , wParam , lParam );	//  [9/18/2007 AJJIYA]
}

DEFINE_SC_PACKET_PROC( SKILL_SC_INIT_ITEMMIX		)
{
	WPARAM wParam = (WPARAM)packet;
	LPARAM lParam = (LPARAM)packet;	

	g_pInterfaceManager->SPChildWndSendMessage( WIID_ITEM_MIX , SPIM_ITEM_MIX_SKILL_INIT , wParam , lParam );	//  [10/1/2007 AJJIYA]
}

DEFINE_SC_PACKET_PROC( SKILL_SC_INIT_SELECTIVE		)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage( SPGM_SET_SKILL_INIT_SELECTIVE, wParam, lParam );
}

DEFINE_SC_PACKET_PROC( EFFECT_SC_INFOS	)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);	

	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_SET_EFFECT_ACTIVITY, wParam, lParam);
}

DEFINE_SC_PACKET_PROC( EFFECT_SC_DEL	)
{
	GU_ID globalID = 0;
	packet->ExtractUINT64(&globalID);
	
	WPARAM wParam = (WPARAM)&globalID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_SET_EFFECT_DELETE, wParam, lParam);


	
	//WPARAM wParam = (WPARAM)&globalID;
	//LPARAM lParam = (LPARAM)packet;	

	//g_pGOBManager->SPPerformMessage(SPGM_SET_EFFECT_ACTIVITY, wParam, lParam);
}


DEFINE_SC_PACKET_PROC( ITEM_SC_MSGBOARD	)
{
	WPARAM wParam = (WPARAM)packet;

	g_pInterfaceManager->SPChildWndSendMessage( WIID_CREATEMSG , SPIM_MSGBOARD_CREATE , wParam , 0 );
}


DEFINE_SC_PACKET_PROC( ITEM_SC_DEL_MSGBOARD	)
{
	WPARAM wParam = (WPARAM)packet;

	g_pInterfaceManager->SPChildWndSendMessage( WIID_DESTORYMSG , SPIM_MSGBOARD_DESTORY , wParam , 0 );
}
