
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

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPWindow.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

//
//#include "SPEventDEF.h"
//#include "SPQuestAttr.h"
//#include "SPQuestArchive.h"
//#include "SPQuestManager.h"
//#include "SPEvent.h"
//#include "SPEventManager.h"

#include "SPCheckManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPTitleAttr.h"
#include "SPTitleArchive.h"
#include "SPTitleRenderDEF.h"
#include "SPTitleRenderBase.h"
#include "SPTitleManager.h"

#include "SPNetworkManager.h"

extern SPNetworkManager* g_pNetworkManager;


DEFINE_SC_PACKET_PROC(TITLE_SC_LIST)
{
	GU_ID			ObjectGUID	= 0;	// 게임오브젝트아이디
	int				iCnt = 0;			// 타이틀 개수
	UINT32			uiTitleID = 0;		// 타이틀 아이디

	packet->ExtractUINT64(&ObjectGUID);	
	packet->ExtractUINT32((UINT32*)&iCnt);

	SPTitleManager*	pTitleManager	=	SPTitleManager::GetInstance();

	if( pTitleManager == NULL )
		return;

	SPTitleData		stTitleData;
	SPTitleArchive*	pTitleArchive	=	pTitleManager->GetTitleArchive();

	if( pTitleArchive == NULL )
		return;

	for(int i=0; i<iCnt; i++)
	{
		packet->ExtractUINT32(&uiTitleID);

		stTitleData.m_uiTitleID	=	uiTitleID;
		pTitleArchive->AddTitleData( stTitleData );
	}

	// 리스트에 데이터를 넣어두고 WORLD_SC_ENTER에서 UI상으로 값을 갱신한다.
}

DEFINE_SC_PACKET_PROC(TITLE_SC_INSERT)
{
	GU_ID			ObjectGUID	= 0;	// 게임오브젝트아이디
	UINT32			uiTitleID = 0;		// 타이틀 아이디

	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&uiTitleID);

	SPTitleManager*	pTitleManager	=	SPTitleManager::GetInstance();

	if( pTitleManager == NULL )
		return;

	SPTitleData		stTitleData;
	SPTitleArchive*	pTitleArchive	=	pTitleManager->GetTitleArchive();

	if( pTitleArchive == NULL )
		return;

	stTitleData.m_uiTitleID	=	uiTitleID;
	pTitleArchive->AddTitleData( stTitleData );

	WPARAM	wParam	=	(WPARAM)&stTitleData;

	g_pInterfaceManager->SPChildWndSendMessage( WIID_STATUS		, SPIM_TITLE_ADD , wParam , NULL );
}

DEFINE_SC_PACKET_PROC(TITLE_SC_CHANGE)
{
	GU_ID			ObjectGUID	= 0;	// 게임오브젝트아이디
	UINT32			uiTitleID = 0;		// 타이틀 아이디

	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&uiTitleID);

	SPTitleData		stTitleData;

	stTitleData.m_uiTitleID	=	uiTitleID;

	WPARAM wParam = (WPARAM)&ObjectGUID;
	LPARAM lParam = (LPARAM)&stTitleData;

	g_pGOBManager->SPPerformMessage( SPGM_TITLE_CHANGE , wParam , lParam );

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( ObjectGUID == pLocalPlayer->GetGUID() )
	{
		g_pInterfaceManager->SPChildWndSendMessage( WIID_STATUS		, SPIM_TITLE_CHANGE , wParam , lParam );
	}
	else
	{
		g_pInterfaceManager->SPChildWndSendMessage( WIID_PC_STATUS	, SPIM_REFRESH , wParam , lParam );
	}
}
