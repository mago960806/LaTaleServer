// ***************************************************************
//  SPWindowWorldMapShortcut.cpp   version:  1.0   date:2006/01/24
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 AJJIYA - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPWindowWorldMapShortcut.h"

SPWindowWorldMapShortcut::SPWindowWorldMapShortcut(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowWorldMapShortcut::SPWindowWorldMapShortcut(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_MENU, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowWorldMapShortcut::~SPWindowWorldMapShortcut()
{
	Clean();
}

void SPWindowWorldMapShortcut::Init()
{
	SPWindowButton*	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_WORLDMAP_SHORTCUT_STAGE , 2 , 0 , 15 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,   1 , 338 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,  17 , 338 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,  33 , 338 );

	pWindowButton	=	new SPWindowButton( WIID_WORLDMAP_SHORTCUT_MAP , 18 , 0 , 15 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,   1 , 354 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,  17 , 354 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI100.PNG" ,  33 , 354 );
}

void SPWindowWorldMapShortcut::UpdatePos()
{
	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_WORLDMAP_SHORTCUT_STAGE );

	if( pWindow != NULL )
	{
		pWindow->AdjustCoord( 2 , 0 );
	}

	pWindow	=	Find( WIID_WORLDMAP_SHORTCUT_MAP );

	if( pWindow != NULL )
	{
		pWindow->AdjustCoord( 18 , 0 );
	}
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowWorldMapShortcut )

	SPIMESSAGE_COMMAND( SPIM_WORLDMAP_WORLD			,	OnWorldMap	)
	SPIMESSAGE_COMMAND( SPIM_WORLDMAP_STAGE			,	OnStageMap	)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_WORLDMAP_SHORTCUT_STAGE	,	OnWorldMap			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_WORLDMAP_SHORTCUT_MAP		,	OnStageMap			)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowWorldMapShortcut::OnWorldMap( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_WORLDMAP , SPIM_WORLDMAP_WORLD , NULL , NULL );
	return 1;
}

int SPWindowWorldMapShortcut::OnStageMap( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_WORLDMAP , SPIM_WORLDMAP_STAGE , NULL , NULL ); 
	return 1;
}
