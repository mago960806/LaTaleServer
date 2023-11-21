/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/04/07
	created:	2006:4:7:   15:41
	filename: 	SPWindowWorldMap.cpp
	
	purpose:	
*********************************************************************/

#include "SPCommon.h"
#include "SPUtil.h"

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

#include "SPGOBStatus.h"
#include "SPGOBManager.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"

#include "SPTeamArchive.h"

#include "SPStage.h"

#include "SPEventDEF.h"
#include "SPEventArchive.h"
#include "SPEventManager.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include <WinSock2.h>
#include "PacketID.h"
#include "Packet.h"
#include "PacketHandler.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowWorldMapToolTip.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPInterfaceManager.h"

#include "SPWayPointAttr.h"
#include "SPPvpManager.h"

#include "SPDebug.h"

#include "SPWindowWorldMap.h"

SPWindowWorldMap::SPWindowWorldMap(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowWorldMap::SPWindowWorldMap(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_WORLDMAP, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowWorldMap::~SPWindowWorldMap()
{
	Clean();
}

void SPWindowWorldMap::Init()
{
	InitWorldTootip();

	SPWindowButton*	pWindowButton	=	NULL;

	pWindowButton =	new SPWindowButton( WIID_WORLDMAP_CTRL_CLOSE , 766 , 5 , 29 , 26 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/WORLDMAP/ICON_BUTTON.PNG" ,  0 , 78 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/WORLDMAP/ICON_BUTTON.PNG" , 29 , 78 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/WORLDMAP/ICON_BUTTON.PNG" , 58 , 78 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/WORLDMAP/ICON_BUTTON.PNG" , 87 , 78 );
	pWindowButton	=	NULL;

	pWindowButton =	new SPWindowButton( WIID_WORLDMAP_CTRL_WORLD_VIEW , 735 , 5 , 29 , 26 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/WORLDMAP/ICON_BUTTON.PNG" ,  0 , 26 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/WORLDMAP/ICON_BUTTON.PNG" , 29 , 26 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/WORLDMAP/ICON_BUTTON.PNG" , 58 , 26 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/WORLDMAP/ICON_BUTTON.PNG" , 87 , 26 );
	pWindowButton	=	NULL;

	pWindowButton =	new SPWindowButton( WIID_WORLDMAP_CTRL_STAGE_VIEW , 735 , 5 , 29 , 26 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/WORLDMAP/ICON_BUTTON.PNG" ,  0 , 52 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/WORLDMAP/ICON_BUTTON.PNG" , 29 , 52 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/WORLDMAP/ICON_BUTTON.PNG" , 58 , 52 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/WORLDMAP/ICON_BUTTON.PNG" , 87 , 52 );
	pWindowButton	=	NULL;

	SPWindowStatic*	pWindowStatic	=	NULL;

	pWindowStatic =	new SPWindowStatic( WIID_WORLDMAP_CTRL_LEGEND , 0 , 0 , 128 , 128 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/WORLDMAP/INFO01.PNG" , 0 , 0 );
	pWindowStatic->Show();
	pWindowStatic	=	NULL;

	pWindowStatic =	new SPWindowStatic( WIID_WORLDMAP_STAGE , 0 , 0 , 0 , 0 , this );
	pWindowStatic->Hide();
	pWindowStatic	=	NULL;

	pWindowStatic =	new SPWindowStatic( WIID_WORLDMAP_CTRL_BOTTOM_LINE_TOOLTIP , 0 , 568 , 800 , 32 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/WORLDMAP/INFO02.PNG" , 0 , 0 );
	pWindowStatic->Show();

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/WORLDMAP/WORLD_01_JIENDIA.PNG" , &m_pEffectWorldMap );

	pWindowStatic	=	new SPWindowStatic( WIID_WORLDMAP_WORLD , 0 , 0 , 1024 , 1024 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/WORLDMAP/WORLD_01_JIENDIA.PNG" );
	pWindowStatic->Show();
	pWindowStatic	=	NULL;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/WORLDMAP/SCREENEFFECT01.PNG" , &m_pEffectWorldMapDamage );
	
	m_rcEffectWorldMapDamageDest.left	=	0;
	m_rcEffectWorldMapDamageDest.top	=	0;
	m_rcEffectWorldMapDamageDest.right	=	g_pVideo->GetScreenLenX();
	m_rcEffectWorldMapDamageDest.bottom	=	g_pVideo->GetScreenLenY();;

	m_rcEffectWorldMapDamageSrc.left	=	0;
	m_rcEffectWorldMapDamageSrc.top		=	0;
	m_rcEffectWorldMapDamageSrc.right	=	m_rcEffectWorldMapDamageSrc.left + 128;
	m_rcEffectWorldMapDamageSrc.bottom	=	m_rcEffectWorldMapDamageSrc.top + 128;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/WORLDMAP/TIP01.PNG" , &m_pEffectWayPoint );

	m_ptEffectWayPointDest.x		=	-1;
	m_ptEffectWayPointDest.y		=	-1;

	m_rcEffectWayPointSrc.left		=	198;
	m_rcEffectWayPointSrc.top		=	69;
	m_rcEffectWayPointSrc.right		=	m_rcEffectWayPointSrc.left + 32;
	m_rcEffectWayPointSrc.bottom	=	m_rcEffectWayPointSrc.top + 32;

	m_fEffectWayPointScaleX			=	1.0f;
	m_fEffectWayPointScaleY			=	1.0f;

	m_cEffectWayPointColor			=	D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

//////////////////////////////////////////////////////////////////////////

	LoadLDTStage();
	LoadLDTGroup();

	InitKey();

	m_CurMoveID					=	WIID_WORLDMAP_WORLD;
	m_iCreateStageID			=	-1;
	m_fLimitTimeTeamInfo		=	1.0f;
	m_fAccumulateTimeTeamInfo	=	0.0f;

	m_eEffectType				=	WORLDMAP_EFFECT_NULL;
	m_cEffectColor				=	INITCOLOR;
}

void SPWindowWorldMap::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pEffectWorldMap );
	SAFE_RELEASE( m_pEffectWorldMapDamage );
	SAFE_RELEASE( m_pEffectWayPoint );
}

void SPWindowWorldMap::Process(float fTime)
{
	if(!m_bShow) return;

	CUT_IN_STATE	eCutInState	=	g_pEventManager->GetCutInState();

	if( eCutInState != CUT_IN_STATE_NULL )
	{
		Hide();
		return;
	}

	ProcessKey( fTime );

	if( !( GetAsyncKeyState(VK_LBUTTON) < 0 ) )
		CheckAutoScroll( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() );

	ProcessEffect( fTime );

	HideCtrl();

	SendRequestTeamInfo( fTime );

	UpdateMeInfo();
	UpdateTeamInfo();

	SPWindow::Process( fTime );
}

void SPWindowWorldMap::Render(float fTime)
{
	if(!m_bShow) return;

	SPWindow::RenderReverse(fTime);

	if( ( m_eEffectType == WORLDMAP_EFFECT_STAGE_CLICK || m_eEffectType == WORLDMAP_EFFECT_WORLD_CLICK ) && m_pEffectWorldMap != NULL )
	{
		m_pEffectWorldMap->SetColor( m_cEffectColor );
		m_pEffectWorldMap->SetScale( 1.0f , 1.0f );
		m_pEffectWorldMap->RenderTexture( &m_rcEffectWorldMapDest , &m_rcEffectWorldMapSrc );
		m_pEffectWorldMap->SetColor( INITCOLOR );
	}

	if( m_eEffectType == WORLDMAP_EFFECT_DAMAGE && m_pEffectWorldMapDamage != NULL )
	{
		g_pVideo->SetTextureEffect( TEXTURE_EFFECT_NORMAL );
		m_pEffectWorldMapDamage->SetColor( m_cEffectColor );
		m_pEffectWorldMapDamage->SetScale( 1.0f , 1.0f );
		m_pEffectWorldMapDamage->RenderTexture( &m_rcEffectWorldMapDamageDest , &m_rcEffectWorldMapDamageSrc );
		m_pEffectWorldMapDamage->SetColor( INITCOLOR );
		g_pVideo->SetTextureEffect( TEXTURE_EFFECT_NULL );
	}

	if( m_pEffectWayPoint != NULL && m_eEffectType != WORLDMAP_EFFECT_STAGE_CLICK && m_eEffectType != WORLDMAP_EFFECT_WORLD_CLICK )
	{
		SPWindow*	pWindow	=	Find( WIID_WORLDMAP_STAGE , true );

		if( pWindow != NULL )
		{
			if( pWindow->IsShow() == true )
			{
				SPWindow*	pWayPointWindow	=	Find( m_eWayPointInstanceID , true );

				if( pWayPointWindow != NULL )
				{
					RECT	rcMapEvent;

					pWayPointWindow->GetWindowRect( rcMapEvent );

					m_ptEffectWayPointDest.x	=	rcMapEvent.left + ( ( rcMapEvent.right - rcMapEvent.left ) / 2 );
					m_ptEffectWayPointDest.y	=	rcMapEvent.top + ( ( rcMapEvent.bottom - rcMapEvent.top ) / 2 );

					g_pVideo->SetTextureEffect( TEXTURE_EFFECT_NORMAL );
					m_pEffectWayPoint->SetColor( m_cEffectWayPointColor );
					m_pEffectWayPoint->SetScale( m_fEffectWayPointScaleX , m_fEffectWayPointScaleY );
					m_pEffectWayPoint->SetSrcRect( m_rcEffectWayPointSrc );
					m_pEffectWayPoint->RenderTextureImmediateCenterBase( m_ptEffectWayPointDest.x , m_ptEffectWayPointDest.y );
					m_pEffectWayPoint->SetColor( INITCOLOR );
					m_pEffectWayPoint->SetScale( 1.0f , 1.0f );
					g_pVideo->SetTextureEffect( TEXTURE_EFFECT_NULL );
				}
			}
		}
	}
}

void SPWindowWorldMap::Show(bool bWithChild)
{
	g_pInterfaceManager->SetFocusWindow( this );
	g_pInterfaceManager->SetActiveWindow( this->GetWrapWindow() );

	SPWindow::Show( bWithChild );

	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_WORLDMAP_CTRL_WORLD_DAMAGE , true );

	if( pWindow != NULL )
		pWindow->Hide();

	pWindow	=	Find( WIID_WORLDMAP_CTRL_STAGE_DAMAGE , true );

	if( pWindow != NULL )
		pWindow->Hide();

	pWindow							=	Find( WIID_WORLDMAP_WORLD , true );
	SPWindowStatic*	pWindowStatic	=	(SPWindowStatic*)pWindow;

	if( pWindowStatic != NULL )
		pWindowStatic->SetColor( INITCOLOR );

	pWindow							=	Find( WIID_WORLDMAP_STAGE , true );
	pWindowStatic					=	(SPWindowStatic*)pWindow;

	if( pWindowStatic != NULL )
		pWindowStatic->SetColor( INITCOLOR );

	ResetLMouseData();
}

void SPWindowWorldMap::Hide(bool bSendServer /* = true */)
{
	g_pInterfaceManager->SetActiveWindow( NULL );

	SPWindow::Hide( bSendServer );

	g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_DEFAULT );

	ResetLMouseData();

	m_fLimitTimeTeamInfo		=	1.0f;
	m_fAccumulateTimeTeamInfo	=	0.0f;

	m_eEffectType				=	WORLDMAP_EFFECT_NULL;
	m_cEffectColor				=	INITCOLOR;

	SPWindow*	pWindow;

	pWindow							=	Find( WIID_WORLDMAP_WORLD , true );
	SPWindowStatic*	pWindowStatic	=	(SPWindowStatic*)pWindow;

	if( pWindowStatic != NULL )
		pWindowStatic->SetColor( INITCOLOR );

	pWindow							=	Find( WIID_WORLDMAP_STAGE , true );
	pWindowStatic					=	(SPWindowStatic*)pWindow;

	if( pWindowStatic != NULL )
		pWindowStatic->SetColor( INITCOLOR );

	m_eWayPointInstanceID			=	-1;

	m_ptEffectWayPointDest.x		=	-1;
	m_ptEffectWayPointDest.y		=	-1;
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowWorldMap )

	SPIMESSAGE_COMMAND(	SPIM_PURGE					,		OnPurge				)
	SPIMESSAGE_COMMAND(	SPIM_REFRESH				,		OnRefresh			)
	SPIMESSAGE_COMMAND(	SPIM_KEY_RETURN				,		OnKeyReturn			)
	SPIMESSAGE_COMMAND( SPIM_WORLDMAP_WORLD			,		OnWorldMap			)
	SPIMESSAGE_COMMAND( SPIM_WORLDMAP_STAGE			,		OnStageMap			)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE			,		OnCursorMove		)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_WORLDMAP_CTRL_CLOSE			,	OnClose				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_WORLDMAP_CTRL_WORLD_VIEW		,	OnStageMap			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_WORLDMAP_CTRL_STAGE_VIEW		,	OnWorldMap			)

	SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE			,	WIID_WORLDMAP_WORLD					,	OnCursorMove		)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE			,	WIID_WORLDMAP_STAGE					,	OnCursorMove		)

	SPIMESSAGE_CONTROL_RANGE(	SPIM_CURSOR_MOVE	,	WIID_WORLDMAP_CTRL_START			,	WIID_WORLDMAP_CTRL_END			,	OnCursorMove		)
	SPIMESSAGE_CONTROL_RANGE(	SPIM_CURSOR_ENTER	,	WIID_WORLDMAP_CTRL_START			,	WIID_WORLDMAP_CTRL_END			,	OnCursorEnter		)
	SPIMESSAGE_CONTROL_RANGE(	SPIM_CURSOR_OUT		,	WIID_WORLDMAP_CTRL_START			,	WIID_WORLDMAP_CTRL_END			,	OnCursorOut			)

	SPIMESSAGE_CONTROL_RANGE(	SPIM_LBUTTON_UP		,	WIID_WORLDMAP_CTRL_WORLD_LOCATION	,	WIID_WORLDMAP_CTRL_WORLD_END	,	OnStageLButtonDown	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowWorldMap::OnWorldMap( WPARAM wParam, LPARAM lParam )
{
	bool	bShow			=	m_bShow;
	m_eWayPointInstanceID	=	-1;

	if( ( m_bShow == TRUE && m_CurMoveID == WIID_WORLDMAP_WORLD ) || m_eEffectType == WORLDMAP_EFFECT_WORLD_CLICK || g_pkPvpManager->IsPvpZone() )
	{
		Hide();
		return 1;
	}

	if( m_eEffectType == WORLDMAP_EFFECT_STAGE_CLICK )
		return 1;

	Show();

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		int	iStageID	= pLocalPlayer->GetCurStageID();

		//[xialin 2008/09/11]add ÐÂÊÖ¹Ø²»ÏÔÊ¾ >>>
		//if ( iStageID == 1 )
		//{
		//	Hide();
		//	return 1;
		//}
		//[xialin 2008/09/11]add ÐÂÊÖ¹Ø²»ÏÔÊ¾ <<<

		LoadStage( iStageID );
	}

	SPWindow*	pWindow	=	Find( WIID_WORLDMAP_CTRL_TOOLTIP , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT );

	if( m_iCreateStageID != -1 && bShow == true )
	{
		OnClickWorldMap();
		return 1;
	}

	WorldMapComplete();

	return 1;
}

int SPWindowWorldMap::OnStageMap( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == TRUE && m_CurMoveID == WIID_WORLDMAP_STAGE )
	{
		Hide();
		return 1;
	}

	if( m_eEffectType == WORLDMAP_EFFECT_STAGE_CLICK || m_eEffectType == WORLDMAP_EFFECT_WORLD_CLICK || g_pkPvpManager->IsPvpZone() )
		return 1;

	Show();

	SPPlayer*		pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
	SPWayPointAttr*	pWayPointAttr	=	(SPWayPointAttr*)wParam;
	MAPEVENT_INFO*	pMapEventInfo	=	NULL;

	if( pWayPointAttr != NULL )
		pMapEventInfo	=	pWayPointAttr->m_pMapEvent;

	if( pLocalPlayer != NULL )
	{
		int	iStageID	= pLocalPlayer->GetCurStageID();
		//[xialin 2008/09/11]add ÐÂÊÖ¹Ø²»ÏÔÊ¾ >>>
		//if ( iStageID == 1 )
		//{
		//	Hide();
		//	return 1;
		//}
		//[xialin 2008/09/11]add ÐÂÊÖ¹Ø²»ÏÔÊ¾ <<<

		if( pMapEventInfo != NULL )
			iStageID	=	pMapEventInfo->mapInfo.uiStageID;

		if( LoadStage( iStageID ) == false )
		{
			Hide();
			return 1;
		}
	}

	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_WORLDMAP_CTRL_WORLD_VIEW , true );

	if( pWindow != NULL )
		pWindow->Hide();

	m_CurMoveID	=	WIID_WORLDMAP_STAGE;

	if( PerformMouseMove( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() ) == 1 )
	{
		pWindow = g_pInterfaceManager->GetCursorWindow();

		if( pWindow != NULL )
			OnCursorEnter( pWindow->GetInstanceID() , NULL , NULL );
	}

	OnStageLButtonDown( WIID_WORLDMAP_CTRL_WORLD_LOCATION + m_iCreateStageID , NULL , NULL );

	if( pMapEventInfo != NULL )
	{
		AdjustMapEvent( pMapEventInfo->mapInfo.uiStageID , pMapEventInfo->mapInfo.uiMapGroupID , pMapEventInfo->mapInfo.uiMapID , pMapEventInfo->posX , pMapEventInfo->posY );
	}

	return 1;
}

int SPWindowWorldMap::OnPurge( WPARAM wParam, LPARAM lParam )
{
	ResetLMouseData();

	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_WORLDMAP_WORLD , true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( m_iAX , m_iAY );

	return 1;
}

int SPWindowWorldMap::OnRefresh( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	if( m_eEffectType != WORLDMAP_EFFECT_NULL )
		return 1;

	int	iType		=	(int)wParam;

	// 0:ÀÏ¹Ý°ø°Ý 1:Å©¸® 2: ¹Ì½º 3: Immune
	if( iType > 1 )
		return 1;

	m_eEffectType	=	WORLDMAP_EFFECT_DAMAGE;
	m_cEffectColor	=	INITCOLOR;

	SPWindow*	pWindow			=	NULL;
	SPWindow*	pWindowDamage	=	NULL;

	if( m_CurMoveID == WIID_WORLDMAP_WORLD )
	{
		pWindow	=	Find( WIID_WORLDMAP_CTRL_WORLD_ME , true );

		if( pWindow != NULL )
		{
			if( pWindow->IsShow() == false )
				pWindow = NULL;
		}

		if( pWindow == NULL )
			pWindow	=	Find( WIID_WORLDMAP_CTRL_WORLD_PARTY , true );

		pWindowDamage	=	Find( WIID_WORLDMAP_CTRL_WORLD_DAMAGE , true );

		if( pWindowDamage == NULL && pWindow != NULL )
		{
			pWindowDamage	=	new	SPWindowStatic( WIID_WORLDMAP_CTRL_WORLD_DAMAGE , 0 , 0 , WORLDMAP_DAMAGE_ICON_X , WORLDMAP_DAMAGE_ICON_Y , pWindow->GetParent() );
			pWindowDamage->SetImage( "DATA/INTERFACE/WORLDMAP/INFO00.PNG" ,  23 , 16 );
		}
	}
	else if( m_CurMoveID == WIID_WORLDMAP_STAGE )
	{
		pWindow	=	Find( WIID_WORLDMAP_CTRL_STAGE_ME , true );

		if( pWindow != NULL )
		{
			if( pWindow->IsShow() == false )
				pWindow = NULL;
		}

		if( pWindow == NULL )
			pWindow	=	Find( WIID_WORLDMAP_CTRL_STAGE_PARTY , true );

		pWindowDamage	=	Find( WIID_WORLDMAP_CTRL_STAGE_DAMAGE , true );

		if( pWindowDamage == NULL && pWindow != NULL )
		{
			pWindowDamage	=	new	SPWindowStatic( WIID_WORLDMAP_CTRL_STAGE_DAMAGE , 0 , 0 , WORLDMAP_DAMAGE_ICON_X , WORLDMAP_DAMAGE_ICON_Y , pWindow->GetParent() );
			pWindowDamage->SetImage( "DATA/INTERFACE/WORLDMAP/INFO00.PNG" ,  23 , 16 );
		}
	}

	if( pWindow != NULL )
	{
		int	iParentPosX = 0 , iParentPosY = 0;
		int	iPosX = 0 , iPosY = 0;

		pWindow->GetAbsCoord( iPosX , iPosY );

		if( pWindowDamage != NULL )
		{
			SPWindow*	pWindowParent	=	pWindowDamage->GetParent();

			if( pWindowParent != NULL )
			{
				pWindowParent->GetAbsCoord( iParentPosX , iParentPosY );
			}

			pWindowDamage->AdjustCoord( iPosX + WORLDMAP_ICON_X + WORLDMAP_DAMAGE_ICON_X / 2 - iParentPosX , iPosY - WORLDMAP_ICON_Y / 2 - iParentPosY );

			SPWindowStatic*	pWindowStatic	=	(SPWindowStatic*)pWindowDamage;

			if( pWindowStatic != NULL )
			{
				pWindowStatic->SetImageScaleX( 1.0f );
				pWindowStatic->SetImageScaleY( 1.0f );
				pWindowStatic->SetCenterRender( true );
				pWindowStatic->SetColor( INITCOLOR );
			}

			pWindowDamage->Show();
		}
	}

	return 1;
}

int SPWindowWorldMap::OnKeyReturn( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowWorldMap::OnClose( WPARAM wParam, LPARAM lParam )
{
	Hide();

	return 1;
}

int SPWindowWorldMap::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	int	iXPos	=	LOWORD(lParam);
	int	iYPos	=	HIWORD(lParam);

	if( GetAsyncKeyState(VK_LBUTTON) < 0 && m_eEffectType != WORLDMAP_EFFECT_STAGE_CLICK && m_eEffectType != WORLDMAP_EFFECT_WORLD_CLICK )
	{
		if( m_ptLMousePos.x == 0 && m_ptLMousePos.y == 0 )
		{
			m_ptLMousePos.x	=	iXPos;
			m_ptLMousePos.y	=	iYPos;
		}

		int	iDistanceX	=	iXPos - m_ptLMousePos.x;
		int	iDistanceY	=	iYPos - m_ptLMousePos.y;

		MoveScreenDistance( iDistanceX , iDistanceY );

		g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_WORLDMAP_DRAG );
	}

	m_ptLMousePos.x	=	iXPos;
	m_ptLMousePos.y	=	iYPos;

	SPWindow*	pWindow	=	Find( WIID_WORLDMAP_CTRL_TOOLTIP , true );

	if( pWindow != NULL )
	{
		if( pWindow->IsShow() == true )
			pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE );
	}

	return 1;
}

int SPWindowWorldMap::OnCursorMove( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnCursorMove( wParam , lParam );
}

int SPWindowWorldMap::OnCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindowWorldMapToolTip::WMTOOLTIP_SHOW_TYPE	eTooltipType	=	SPWindowWorldMapToolTip::TOOLTIP_SHOW_TYPE_NULL;
	LPARAM					lDataParam	=	NULL;
	SPStageToolTipData		stStageData;

	if( iID >= WIID_WORLDMAP_CTRL_WORLD_LOCATION && iID <= WIID_WORLDMAP_CTRL_WORLD_END )
	{
		SPWorldMapStageAttr*	pStageAttr		=	GetStageAttr( iID - WIID_WORLDMAP_CTRL_WORLD_LOCATION );

		eTooltipType	=	SPWindowWorldMapToolTip::TOOLTIP_SHOW_TYPE_WORLD;
		lDataParam			=	(LPARAM)pStageAttr;
	}
	else if( iID >= WIID_WORLDMAP_CTRL_STAGE && iID <= WIID_WORLDMAP_CTRL_STAGE_END )
	{
		SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer == NULL )
			return 1;

		eTooltipType	=	SPWindowWorldMapToolTip::TOOLTIP_SHOW_TYPE_STAGE;

		stStageData.m_iResID		=	0;
		stStageData.m_iResIndex		=	0;
		stStageData.m_uiClassType	=	0;

		char	szBuf[_MAX_PATH];

		const char*	pszPosFormat	=	g_pResourceManager->GetGlobalString( 1500001 );

		SPWorldMapMapAttr*	pMapAttr;
		int iMapWidth		=	0;
		int iMapHeight		=	0;

		float fMapEventPosX	=	0.0f;
		float fMapEventPosY	=	0.0f;

		if( iID == WIID_WORLDMAP_CTRL_STAGE_ME )
		{
			char szName[64];
			ZeroMemory(szName, 64);
			pLocalPlayer->SPGOBPerformMessage( SPGM_GETGOBNAME, (LPARAM)szName );

			SPGOBStatus* pPlayerStatus	= pLocalPlayer->GetStatus();

			if( pPlayerStatus != NULL )
			{
				stStageData.m_uiClassType = pPlayerStatus->GetStatusValue(STATUS_CLASS_TYPE);
			}

			pMapAttr	=	GetMapAttr( pLocalPlayer->GetCurStageID() , pLocalPlayer->GetCurMapGroupID() );

			if( pMapAttr == NULL )
				return 1;

			iMapWidth		=	pMapAttr->m_iRealMapWidth;
			iMapHeight		=	pMapAttr->m_iRealMapHeight;

			fMapEventPosX	=	pLocalPlayer->GetPosX();
			fMapEventPosY	=	pLocalPlayer->GetPosY();

			if( pMapAttr->m_eType == MAP_LINK_HORIZONTAL )		//	¼öÆò¸Ê
			{
				iMapWidth		*=	pMapAttr->m_iRealMapCount;
			}
			else if( pMapAttr->m_eType == MAP_LINK_VERTICAL )	//	¼öÁ÷¸Ê
			{
				iMapHeight		*=	pMapAttr->m_iRealMapCount;
			}

#if defined(_DEBUG)
			char	szAddPosInfo[_MAX_PATH];
			sprintf( szAddPosInfo , " ( S:%d : M:%d )" , pLocalPlayer->GetCurStageID() , pLocalPlayer->GetCurMapGroupID() );
			std::string	strAddPosInfo	=	pszPosFormat;
			strAddPosInfo	+=	szAddPosInfo;
			pszPosFormat	=	strAddPosInfo.c_str();
#endif

			// À¯ÆÀÀå´Ô ¿äÃ»¿¡ ÀÇÇÑ Á¤Ã¥ º¯°æ [9/15/2006 AJJIYA]
//			sprintf( szBuf , pszPosFormat , (int)fMapEventPosX , iMapHeight - (int)fMapEventPosY );
			sprintf( szBuf , pszPosFormat , (int)fMapEventPosX , (int)fMapEventPosY );

			stStageData.m_strName	=	szName;
			stStageData.m_strPos	=	szBuf;
		}
		else if( iID >= WIID_WORLDMAP_CTRL_STAGE_PARTY && iID <= WIID_WORLDMAP_CTRL_STAGE_PARTY_END )
		{
			SPTeamArchive*	pTeamArchive	=	pLocalPlayer->GetTeam();

			if( pTeamArchive == NULL )
				return 1;

			if( pTeamArchive->GetActive() == false )
				return 1;

			int	iTeamCount	=	pTeamArchive->GetTeamCount();
			std::vector< TEAM_MEMBER* >*	pvTeamMemberList = pTeamArchive->GetTeamList();
			TEAM_MEMBER*					pTeamMember;

			for( int i = iID - WIID_WORLDMAP_CTRL_STAGE_PARTY ; i < iTeamCount ; ++i )
			{
				pTeamMember	=	pvTeamMemberList->at( i );

				if( pTeamMember == NULL )
					continue;

				if( pTeamMember->usStageID != m_iCreateStageID )
					continue;

				pMapAttr	=	GetMapAttr( pTeamMember->usStageID , pTeamMember->usMapGroupID );

				if( pMapAttr == NULL )
					continue;

				iMapWidth		=	pMapAttr->m_iRealMapWidth;
				iMapHeight		=	pMapAttr->m_iRealMapHeight;

				fMapEventPosX	=	pTeamMember->fPosX;
				fMapEventPosY	=	pTeamMember->fPosY;

				if( pMapAttr->m_eType == MAP_LINK_HORIZONTAL )		//	¼öÆò¸Ê
				{
					iMapWidth		*=	pMapAttr->m_iRealMapCount;
				}
				else if( pMapAttr->m_eType == MAP_LINK_VERTICAL )	//	¼öÁ÷¸Ê
				{
					iMapHeight		*=	pMapAttr->m_iRealMapCount;
				}

#if defined(_DEBUG)
				char	szAddPosInfo[_MAX_PATH];
				sprintf( szAddPosInfo , " ( S:%d : M:%d )" , pTeamMember->usStageID , pTeamMember->usMapGroupID );
				std::string	strAddPosInfo	=	pszPosFormat;
				strAddPosInfo	+=	szAddPosInfo;
				pszPosFormat	=	strAddPosInfo.c_str();
#endif
				// À¯ÆÀÀå´Ô ¿äÃ»¿¡ ÀÇÇÑ Á¤Ã¥ º¯°æ [9/15/2006 AJJIYA]
	//			sprintf( szBuf , pszPosFormat , (int)fMapEventPosX , iMapHeight - (int)fMapEventPosY );
				sprintf( szBuf , pszPosFormat , (int)fMapEventPosX , (int)fMapEventPosY );

				stStageData.m_strName		=	pTeamMember->szCharName;
				stStageData.m_uiClassType	=	pTeamMember->byClass;
				stStageData.m_strPos		=	szBuf;
				break;
			}
		}
		else
		{
			int	iNumber	=	iID - WIID_WORLDMAP_CTRL_STAGE_ETC;
			std::vector<MAPEVENT_INFO*>::iterator	Iter	=	m_vMapEventInfo.begin();
			MAPEVENT_INFO*	pMapEventInfo	=	NULL;

			while( Iter != m_vMapEventInfo.end() )
			{
				pMapEventInfo	=	(*Iter);
				++Iter;

				if( pMapEventInfo != NULL )
				{
					if( pMapEventInfo->byDynamicCreate != 0 )
						continue;

					pMapAttr	=	GetMapAttr( pMapEventInfo->mapInfo.uiStageID , pMapEventInfo->mapInfo.uiMapGroupID );
					
					if( pMapAttr != NULL )
					{
						switch( pMapEventInfo->eventType )
						{
						case EVENT_NPC:
							break;
						case EVENT_BEAUTY:
							break;
						case EVENT_SAVEPOINT:
							break;
						case EVENT_PORTAL:
							break;
						case EVENT_MAILBOX:
							break;
						default:
							continue;
						}

						if( iNumber == 0 )
							break;

						--iNumber;
					}
				}
			}

			if( pMapEventInfo == NULL || pMapAttr == NULL )
				return 1;

			stStageData.m_strName	=	pMapEventInfo->name;

			int iMapWidth		=	pMapAttr->m_iRealMapWidth;
			int iMapHeight		=	pMapAttr->m_iRealMapHeight;

			float fMapEventPosX	=	pMapEventInfo->posX;
			float fMapEventPosY	=	pMapEventInfo->posY;

			if( pMapAttr->m_eType == MAP_LINK_HORIZONTAL )		//	¼öÆò¸Ê
			{
				fMapEventPosX	+=	iMapWidth * pMapEventInfo->mapInfo.uiMapID;
				iMapWidth		*=	pMapAttr->m_iRealMapCount;
			}
			else if( pMapAttr->m_eType == MAP_LINK_VERTICAL )	//	¼öÁ÷¸Ê
			{
				fMapEventPosY	+=	iMapHeight * pMapEventInfo->mapInfo.uiMapID;
				iMapHeight		*=	pMapAttr->m_iRealMapCount;
			}

#if defined(_DEBUG)
			char	szAddPosInfo[_MAX_PATH];
			sprintf( szAddPosInfo , " ( S:%d : M:%d )" , pMapEventInfo->mapInfo.uiStageID , pMapEventInfo->mapInfo.uiMapGroupID );
			std::string	strAddPosInfo	=	pszPosFormat;
			strAddPosInfo	+=	szAddPosInfo;
			pszPosFormat	=	strAddPosInfo.c_str();
#endif

			// À¯ÆÀÀå´Ô ¿äÃ»¿¡ ÀÇÇÑ Á¤Ã¥ º¯°æ [9/15/2006 AJJIYA]
//			sprintf( szBuf , pszPosFormat , (int)fMapEventPosX , iMapHeight - (int)fMapEventPosY );
			sprintf( szBuf , pszPosFormat , (int)fMapEventPosX , (int)fMapEventPosY );
			stStageData.m_strPos	=	szBuf;

			SPEventArchive*	pEventArchive	=	g_pEventManager->GetEventArchive();

			if( pEventArchive != NULL  )
			{
				MAPEVENT_INFO_EXT*	pMapEventInfoEx = pEventArchive->GetEventExt( pMapEventInfo->id );

				if( pMapEventInfoEx != NULL )
				{
					stStageData.m_strDesc	=	pMapEventInfoEx->m_strDesc;
					stStageData.m_iResID	=	pMapEventInfoEx->m_iIcon;
					stStageData.m_iResIndex	=	pMapEventInfoEx->m_iIconIndex;
				}
			}
		}

		lDataParam	=	(LPARAM)&stStageData;
	}

	SPWindow*	pWindow	=	Find( WIID_WORLDMAP_CTRL_TOOLTIP , true );

	if( pWindow != NULL )
	{
		if( eTooltipType != SPWindowWorldMapToolTip::TOOLTIP_SHOW_TYPE_NULL )
			pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER , (WPARAM)eTooltipType , lDataParam );
		else
			pWindow->SPSendMessage( SPIM_TOOLTIP_OUT );
	}

	return 1;
}

int SPWindowWorldMap::OnCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow*	pWindow	=	Find( WIID_WORLDMAP_CTRL_TOOLTIP , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT );

	return 1;
}

int SPWindowWorldMap::OnStageLButtonDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	int	iStage						=	iID	-	WIID_WORLDMAP_CTRL_WORLD_LOCATION;;
	m_eEffectType					=	WORLDMAP_EFFECT_STAGE_CLICK;
	m_cEffectColor					=	INITCOLOR;

	SPWindow*	pWindow;

	pWindow							=	Find( WIID_WORLDMAP_WORLD , true );
	SPWindowStatic*	pWindowStatic	=	(SPWindowStatic*)pWindow;

	if( pWindowStatic != NULL )
		pWindowStatic->SetColor( INITCOLOR );

	pWindow							=	Find( WIID_WORLDMAP_STAGE , true );
	pWindowStatic					=	(SPWindowStatic*)pWindow;

	if( pWindowStatic != NULL )
		pWindowStatic->SetColor( INITCOLOR );

	m_rcEffectWorldMapDest.left		=	m_iAX;
	m_rcEffectWorldMapDest.top		=	m_iAY;
	m_rcEffectWorldMapDest.right	=	m_iAX + m_iSX;
	m_rcEffectWorldMapDest.bottom	=	m_iAY + m_iSY;

	MakeViewInfo( m_rcEffectWorldMapViewStart , m_rcEffectWorldMapViewEnd , iStage , true );

	m_rcEffectWorldMapSrc.left		=	m_rcEffectWorldMapViewStart.left;
	m_rcEffectWorldMapSrc.top		=	m_rcEffectWorldMapViewStart.top;
	m_rcEffectWorldMapSrc.right		=	m_rcEffectWorldMapViewStart.right;
	m_rcEffectWorldMapSrc.bottom	=	m_rcEffectWorldMapViewStart.bottom;

	Show();

	if( LoadStage( iStage ) == false )
	{
		Hide();
		return 1;
	}

	pWindow	=	Find( WIID_WORLDMAP_CTRL_WORLD_VIEW , true );

	if( pWindow != NULL )
		pWindow->Hide();

	m_CurMoveID	=	WIID_WORLDMAP_STAGE;

	if( PerformMouseMove( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() ) == 1 )
	{
		pWindow = g_pInterfaceManager->GetCursorWindow();

		if( pWindow != NULL )
			OnCursorEnter( pWindow->GetInstanceID() , NULL , NULL );
	}

	g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_WAIT );
	g_pMouseCursor->ResetWaitData();

	AdjustPlayer();

	return 1;
}

//////////////////////////////////////////////////////////////////////////

void SPWindowWorldMap::LoadLDTStage()
{
#ifdef _DEBUG
	
#endif

	SPLDTFile*	pLDTFile	=	NULL;

	//
	// WORLD_MAP_STAGE LDT LOAD [3/29/2006 AJJIYA]
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_WORLD_MAP_STAGE , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> WORLDMAP LDT Load Fail [%s] \n" , RES_FNAME_LDT_WORLD_MAP_STAGE );
#endif
		return;
	}

	LDT_Field ldtFieldName;
	LDT_Field ldtFieldPosX;
	LDT_Field ldtFieldPosY;
	LDT_Field ldtFieldRectWidth;
	LDT_Field ldtFieldRectHeight;
	LDT_Field ldtFieldMapImage;
	LDT_Field ldtFieldMapWidth;
	LDT_Field ldtFieldMapHeight;
	LDT_Field ldtFieldDesc;
	LDT_Field ldtFieldIcon;
	LDT_Field ldtFieldIconIndex;
	LDT_Field ldtFieldNearMapInfo;

	int		iRecordCount	= pLDTFile->GetItemCount();
	int		iItemID , iNumber;
	char	szBuf[_MAX_PATH];
	SPWorldMapStageAttr	stStageAttr;
	SPWindowButton*		pWindowButton;

	SPWindow*	pkWorldMap	=	Find( WIID_WORLDMAP_WORLD , true );

	if( pkWorldMap == NULL )
		pkWorldMap	=	this;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetFieldFromLable( iItemID, "_Name",			ldtFieldName			);
		pLDTFile->GetFieldFromLable( iItemID, "_PosX",			ldtFieldPosX			);
		pLDTFile->GetFieldFromLable( iItemID, "_PosY",			ldtFieldPosY			);
		pLDTFile->GetFieldFromLable( iItemID, "_Rect_Width",	ldtFieldRectWidth		);
		pLDTFile->GetFieldFromLable( iItemID, "_Rect_Height",	ldtFieldRectHeight		);
		pLDTFile->GetFieldFromLable( iItemID, "_Map_Image",		ldtFieldMapImage		);
		pLDTFile->GetFieldFromLable( iItemID, "_Map_Width",		ldtFieldMapWidth		);
		pLDTFile->GetFieldFromLable( iItemID, "_Map_Height",	ldtFieldMapHeight		);
		pLDTFile->GetFieldFromLable( iItemID, "_Description",	ldtFieldDesc			);
		pLDTFile->GetFieldFromLable( iItemID, "_Icon",			ldtFieldIcon			);
		pLDTFile->GetFieldFromLable( iItemID, "_Icon_Index",	ldtFieldIconIndex		);

		for( iNumber = 1 ; iNumber <= WORLDMAP_NEARMAPINFO ; ++iNumber )
		{
			sprintf( szBuf , "_Map_Info%d" , iNumber );
			pLDTFile->GetFieldFromLable( iItemID, szBuf,		ldtFieldNearMapInfo			);

			if( ldtFieldNearMapInfo.uData.pValue != NULL )
				stStageAttr.m_strNearMapInfo[ iNumber - 1 ]	=	ldtFieldNearMapInfo.uData.pValue;
		}

		pWindowButton	=	new SPWindowButton( WIID_WORLDMAP_CTRL_WORLD_LOCATION + iItemID , ldtFieldPosX.uData.lValue , ldtFieldPosY.uData.lValue , ldtFieldRectWidth.uData.lValue , ldtFieldRectHeight.uData.lValue , pkWorldMap );

		stStageAttr.m_iStage		=	iItemID;
		
		if( ldtFieldName.uData.pValue != NULL )
			stStageAttr.m_strName		=	ldtFieldName.uData.pValue;

		if( ldtFieldMapImage.uData.pValue != NULL )
			stStageAttr.m_strMapFile	=	ldtFieldMapImage.uData.pValue;

		stStageAttr.m_iMapWidth		=	ldtFieldMapWidth.uData.lValue;
		stStageAttr.m_iMapHeight	=	ldtFieldMapHeight.uData.lValue;

		if( ldtFieldDesc.uData.pValue != NULL )
			stStageAttr.m_strDesc		=	ldtFieldDesc.uData.pValue;

		stStageAttr.m_iMapIcon		=	ldtFieldIcon.uData.lValue;
		stStageAttr.m_iMapIconIndex	=	ldtFieldIconIndex.uData.lValue;

		AddStageAttr( stStageAttr );
	}

	SAFE_RELEASE(pLDTFile);

	//	WORLD_MAP_STAGE LDT LOAD END
}

void SPWindowWorldMap::LoadLDTGroup()
{
#ifdef _DEBUG
	
#endif

	SPLDTFile*	pLDTFile	=	NULL;

	//
	// WORLD_MAP_GROUP LDT LOAD [3/29/2006 AJJIYA]
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_WORLD_MAP_GROUP , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> WORLDMAP LDT Load Fail [%s] \n" , RES_FNAME_LDT_WORLD_MAP_GROUP );
#endif
		return;
	}

	LDT_Field ldtFieldName;
	LDT_Field ldtFieldStage;
	LDT_Field ldtFieldGroup;
	LDT_Field ldtFieldPosX;
	LDT_Field ldtFieldPosY;
	LDT_Field ldtFieldRectWidth;
	LDT_Field ldtFieldRectHeight;
	LDT_Field ldtFieldVisibleParty;
	LDT_Field ldtFieldVisiblePortal;
	LDT_Field ldtFieldVisibleSave;
	LDT_Field ldtFieldVisibleNPC;
	LDT_Field ldtFieldAdjustParty;
	LDT_Field ldtFieldAdjustPortal;
	LDT_Field ldtFieldAdjustSave;
	LDT_Field ldtFieldAdjustNPC;
	LDT_Field ldtFieldMapWidth;
	LDT_Field ldtFieldMapHeight;
	LDT_Field ldtFieldMapCount;
	LDT_Field ldtFieldMapLink;

	int		iRecordCount	= pLDTFile->GetItemCount();
	int		iItemID;
	SPWorldMapMapAttr	stMapAttr;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetFieldFromLable( iItemID, "_Name",				ldtFieldName			);
		pLDTFile->GetFieldFromLable( iItemID, "_Stage",				ldtFieldStage			);
		pLDTFile->GetFieldFromLable( iItemID, "_Group",				ldtFieldGroup			);
		pLDTFile->GetFieldFromLable( iItemID, "_PosX",				ldtFieldPosX			);
		pLDTFile->GetFieldFromLable( iItemID, "_PosY",				ldtFieldPosY			);
		pLDTFile->GetFieldFromLable( iItemID, "_Rect_Width",		ldtFieldRectWidth		);
		pLDTFile->GetFieldFromLable( iItemID, "_Rect_Height",		ldtFieldRectHeight		);
		pLDTFile->GetFieldFromLable( iItemID, "_Visible_Party",		ldtFieldVisibleParty	);
		pLDTFile->GetFieldFromLable( iItemID, "_Visible_Portal",	ldtFieldVisiblePortal	);
		pLDTFile->GetFieldFromLable( iItemID, "_Visible_Save",		ldtFieldVisibleSave		);
		pLDTFile->GetFieldFromLable( iItemID, "_Visible_NPC",		ldtFieldVisibleNPC		);
		pLDTFile->GetFieldFromLable( iItemID, "_Pos_Party",			ldtFieldAdjustParty		);
		pLDTFile->GetFieldFromLable( iItemID, "_Pos_Portal",		ldtFieldAdjustPortal	);
		pLDTFile->GetFieldFromLable( iItemID, "_Pos_Save",			ldtFieldAdjustSave		);
		pLDTFile->GetFieldFromLable( iItemID, "_Pos_NPC",			ldtFieldAdjustNPC		);
		pLDTFile->GetFieldFromLable( iItemID, "_Map_Width_Size",	ldtFieldMapWidth		);
		pLDTFile->GetFieldFromLable( iItemID, "_Map_Height_Size",	ldtFieldMapHeight		);
		pLDTFile->GetFieldFromLable( iItemID, "_Map_Count",			ldtFieldMapCount		);
		pLDTFile->GetFieldFromLable( iItemID, "_Map_Link",			ldtFieldMapLink			);

		if( ldtFieldName.uData.pValue != NULL )
			stMapAttr.m_strName		=	ldtFieldName.uData.pValue;


		stMapAttr.m_iStage	=	ldtFieldStage.uData.lValue;
		stMapAttr.m_iGroup	=	ldtFieldGroup.uData.lValue;

		stMapAttr.m_iPosX	=	ldtFieldPosX.uData.lValue;
		stMapAttr.m_iPosY	=	ldtFieldPosY.uData.lValue;

		stMapAttr.m_iWidth	=	ldtFieldRectWidth.uData.lValue;
		stMapAttr.m_iHeight	=	ldtFieldRectHeight.uData.lValue;

		stMapAttr.m_bVisible[ WORLDMAP_MAP_EVENT_PARTY ]	=	ldtFieldVisibleParty.uData.lValue;
		stMapAttr.m_bVisible[ WORLDMAP_MAP_EVENT_PORTAL ]	=	ldtFieldVisiblePortal.uData.lValue;
		stMapAttr.m_bVisible[ WORLDMAP_MAP_EVENT_SAVE ]		=	ldtFieldVisibleSave.uData.lValue;
		stMapAttr.m_bVisible[ WORLDMAP_MAP_EVENT_NPC ]		=	ldtFieldVisibleNPC.uData.lValue;

		stMapAttr.m_bAdjust[ WORLDMAP_MAP_EVENT_PARTY ]		=	ldtFieldAdjustParty.uData.lValue;
		stMapAttr.m_bAdjust[ WORLDMAP_MAP_EVENT_PORTAL ]	=	ldtFieldAdjustPortal.uData.lValue;
		stMapAttr.m_bAdjust[ WORLDMAP_MAP_EVENT_SAVE ]		=	ldtFieldAdjustSave.uData.lValue;
		stMapAttr.m_bAdjust[ WORLDMAP_MAP_EVENT_NPC ]		=	ldtFieldAdjustNPC.uData.lValue;

		stMapAttr.m_iRealMapWidth	=	ldtFieldMapWidth.uData.lValue;
		stMapAttr.m_iRealMapHeight	=	ldtFieldMapHeight.uData.lValue;
		stMapAttr.m_iRealMapCount	=	ldtFieldMapCount.uData.lValue;
		stMapAttr.m_eType			=	(MAP_LINK)ldtFieldMapLink.uData.lValue;

		AddMapAttr( stMapAttr );
	}

	SAFE_RELEASE(pLDTFile);

	//	WORLD_MAP_GROUP LDT LOAD END
}

void SPWindowWorldMap::InitKey()
{
	g_pInputManager->SetActionMap( KEY_AM_WORLDMAP_LEFT		, SPKL_NUMPAD4	);
	g_pInputManager->SetActionMap( KEY_AM_WORLDMAP_UP		, SPKL_NUMPAD8	);
	g_pInputManager->SetActionMap( KEY_AM_WORLDMAP_RIGHT	, SPKL_NUMPAD6	);
	g_pInputManager->SetActionMap( KEY_AM_WORLDMAP_DOWN		, SPKL_NUMPAD2	);
}

void SPWindowWorldMap::ProcessKey( float fTime )
{
	SPMInputKey*	pKeyLeft	=	g_pInputManager->GetActionMap( KEY_AM_WORLDMAP_LEFT );
	SPMInputKey*	pKeyRight	=	g_pInputManager->GetActionMap( KEY_AM_WORLDMAP_RIGHT );
	SPMInputKey*	pKeyUp		=	g_pInputManager->GetActionMap( KEY_AM_WORLDMAP_UP );
	SPMInputKey*	pKeyDown	=	g_pInputManager->GetActionMap( KEY_AM_WORLDMAP_DOWN );

	int	iLeftPos	=	m_iAX;
	int	iRightPos	=	m_iAX	+	m_iSX;
	int	iUpPos		=	m_iAY;
	int	iDownPos	=	m_iAY	+	m_iSY;

	int	iXPos	=	iRightPos	/	2;
	int	iYPos	=	iDownPos	/	2;

	if( pKeyLeft != NULL )
	{
		if( pKeyLeft->InputState == INPUT_PRESS_REPEAT )
		{
			iXPos	=	iLeftPos;
		}
	}

	if( pKeyUp != NULL )
	{
		if( pKeyUp->InputState == INPUT_PRESS_REPEAT )
		{
			iYPos	=	iUpPos;
		}
	}

	if( pKeyRight != NULL )
	{
		if( pKeyRight->InputState == INPUT_PRESS_REPEAT )
		{
			iXPos	=	iRightPos;
		}
	}

	if( pKeyDown != NULL )
	{
		if( pKeyDown->InputState == INPUT_PRESS_REPEAT )
		{
			iYPos	=	iDownPos;
		}
	}

	CheckAutoScroll( iXPos , iYPos );
}

void SPWindowWorldMap::ResetLMouseData()
{
	m_ptLMousePos.x	=	0;
	m_ptLMousePos.y	=	0;
}

void SPWindowWorldMap::MoveScreenDistance( int iXDistance , int iYDistance )
{
	RECT	rcWorldMap;

	SPWindow*	pWindow	=	Find( m_CurMoveID , true );

	if( pWindow == NULL )
		return;

	pWindow->GetWindowRect( rcWorldMap );

	int	iNewPosX	=	rcWorldMap.left;
	int	iNewPosY	=	rcWorldMap.top;

	int	iWorldMapSizeX	=	rcWorldMap.right	-	rcWorldMap.left;
	int	iWorldMapSizeY	=	rcWorldMap.bottom	-	rcWorldMap.top;

	iNewPosX	+=	iXDistance;
	iNewPosY	+=	iYDistance;

	LimitPosCheck( iNewPosX , iNewPosY , iWorldMapSizeX , iWorldMapSizeY );

	pWindow->AdjustCoord( iNewPosX - m_iAX , iNewPosY - m_iAY );
	pWindow->RefreshChildPos();
}

void SPWindowWorldMap::LimitPosCheck( int& iXPos , int& iYPos , int iXSize , int iYSize )
{
	if( iXPos > m_iAX )
		iXPos	=	m_iAX;

	if( iXPos + iXSize  < m_iAX + m_iSX )
		iXPos	=	m_iSX - iXSize;

	if( iYPos > m_iAY )
		iYPos	=	m_iAY;

	if( iYPos + iYSize  < m_iAY + m_iSY )
		iYPos	=	m_iSY - iYSize;
}


void SPWindowWorldMap::CheckAutoScroll( int iXPos , int iYPos )
{
	//	ÀÌµ¿ Á¦¿Ü
	SPWindow*	pWindow	=	g_pInterfaceManager->GetCursorWindow();

	if( pWindow != NULL )
	{
		int	iCount	=	WIID_WORLDMAP_CTRL_NO_MOVE_END	-	WIID_WORLDMAP_CTRL_NO_MOVE_START;

		for ( int i = 0 ; i < iCount ; ++i )
		{
			if( pWindow->GetInstanceID() == WIID_WORLDMAP_CTRL_NO_MOVE_START + i )
			{
				g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_DEFAULT );
				return;
			}
		}
	}

	int	iMargin			=	16;	//	ÇÈ¼¿´ÜÀ§

	int	iLeftAreaStart	=	m_iAX;
	int	iLeftAreaEnd	=	m_iAX	+	iMargin;

	int	iTopAreaStart	=	m_iAY;
	int	iTopAreaEnd		=	m_iAY	+	iMargin;

	int	iRightAreaStart	=	m_iAX	+	m_iSX	-	iMargin;
	int	iRightAreaEnd	=	m_iAX	+	m_iSX;

	int	iBottomAreaStart=	m_iAY	+	m_iSY	-	iMargin;
	int	iBottomAreaEnd	=	m_iAY	+	m_iSY;

	int	iXStep	=	0;
	int	iYStep	=	0;
	int	iStep	=	2;

	if( iXPos >= iLeftAreaStart	&& iXPos <= iLeftAreaEnd )
		iXStep	= iStep;

	if( iXPos >= iRightAreaStart && iXPos <= iRightAreaEnd )
		iXStep	= - iStep;

	if( iYPos >= iTopAreaStart && iYPos <= iTopAreaEnd )
		iYStep	= iStep;

	if( iYPos >= iBottomAreaStart && iYPos <= iBottomAreaEnd )
		iYStep	= - iStep;

	if( iXStep == 0 && iYStep == 0 )
	{
		if( g_pMouseCursor != NULL )
		{
			SPMouseCursor::MOUSE_TYPE	eCurMouseType	=	g_pMouseCursor->GetMouseType();
			SPMouseCursor::MOUSE_TYPE	eMouseType		=	SPMouseCursor::MOUSE_TYPE_DEFAULT;

			SPWindow*	pCursorIn	=	g_pInterfaceManager->GetCursorWindow();
			SPWindow*	pCurWindow	=	Find( m_CurMoveID , true );

			if( pCursorIn != NULL && pCurWindow != NULL )
			{
				if( pCurWindow->GetInstanceID() != pCursorIn->GetInstanceID() && ( pCursorIn->GetInstanceID() & WIID_WORLDMAP_CTRL_NO_MOUSE ) != WIID_WORLDMAP_CTRL_NO_MOUSE && m_CurMoveID == WIID_WORLDMAP_WORLD )
					eMouseType	=	SPMouseCursor::MOUSE_TYPE_WORLDMAP_DETAIL;
			}

			if( ( eCurMouseType & SPMouseCursor::MOUSE_TYPE_WORLDMAP_SCROLL ) == SPMouseCursor::MOUSE_TYPE_WORLDMAP_SCROLL )
			{
				if( GetAsyncKeyState(VK_LBUTTON) < 0 )
					eMouseType	=	SPMouseCursor::MOUSE_TYPE_WORLDMAP_DRAG;
			}

			g_pMouseCursor->SetType( eMouseType );

		}
		return;
	}

	MoveScreenDistance( iXStep , iYStep );

	if( g_pMouseCursor != NULL )
	{
		SPMouseCursor::MOUSE_TYPE	eMouseType = SPMouseCursor::MOUSE_TYPE_DEFAULT;

		if( iXStep > 0 )
		{
			eMouseType	=	SPMouseCursor::MOUSE_TYPE_WORLDMAP_LEFT;
		}
		else if( iXStep < 0 )
		{
			eMouseType	=	SPMouseCursor::MOUSE_TYPE_WORLDMAP_RIGHT;
		}

		if( iYStep > 0 )
		{
			eMouseType	=	SPMouseCursor::MOUSE_TYPE_WORLDMAP_TOP;
		}
		else if( iYStep < 0 )
		{
			eMouseType	=	SPMouseCursor::MOUSE_TYPE_WORLDMAP_BOTTOM;
		}

		if( iXStep > 0 && iYStep > 0 )
		{
			eMouseType	=	SPMouseCursor::MOUSE_TYPE_WORLDMAP_LEFT_TOP;
		}
		else if( iXStep > 0 && iYStep < 0 )
		{
			eMouseType	=	SPMouseCursor::MOUSE_TYPE_WORLDMAP_LEFT_BOTTOM;
		}
		else if( iXStep < 0 && iYStep > 0 )
		{
			eMouseType	=	SPMouseCursor::MOUSE_TYPE_WORLDMAP_RIGHT_TOP;
		}
		else if( iXStep < 0 && iYStep < 0 )
		{
			eMouseType	=	SPMouseCursor::MOUSE_TYPE_WORLDMAP_RIGHT_BOTTOM;
		}

		g_pMouseCursor->SetType( eMouseType );
	}
}

void SPWindowWorldMap::AddStageAttr( SPWorldMapStageAttr& stStageAttr )
{
	m_mStageAttr.insert( STD_MAP_WORLDMAP_STAGE_ATTR::value_type( stStageAttr.m_iStage , stStageAttr ) );
}

SPWorldMapStageAttr* SPWindowWorldMap::GetStageAttr( int iStage )
{
	STD_MAP_WORLDMAP_STAGE_ATTR::iterator	mIter	=	m_mStageAttr.find( iStage );

	if( mIter == m_mStageAttr.end() )
		return NULL;

	return &((*mIter).second);
}

void SPWindowWorldMap::AddMapAttr( SPWorldMapMapAttr& stMapAttr )
{
	m_mMapAttr.insert( STD_MAP_WORLDMAP_MAP_ATTR::value_type( MAKELONG( stMapAttr.m_iStage , stMapAttr.m_iGroup ) , stMapAttr ) );
}

SPWorldMapMapAttr* SPWindowWorldMap::GetMapAttr( int iStage , int iGroup )
{
	STD_MAP_WORLDMAP_MAP_ATTR::iterator	mIter	=	m_mMapAttr.find( MAKELONG( iStage , iGroup ) );

	if( mIter == m_mMapAttr.end() )
		return NULL;

	return &((*mIter).second);
}

bool SPWindowWorldMap::LoadStage( int iStage )
{
	if( m_iCreateStageID == iStage )
	{
		g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_DEFAULT );
		CreateEventInfo( false );
		return true;
	}
	else if( m_iCreateStageID != iStage )
		CleanStage();

	SPWorldMapStageAttr*	pStageAttr	=	GetStageAttr( iStage );

	if( pStageAttr == NULL )
	{
		g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_DEFAULT );
		return false;
	}

	std::string	strMapImage	=	"DATA/INTERFACE/WORLDMAP/";
	strMapImage	+=	pStageAttr->m_strMapFile;

	SPWindow*	pWindow	=	Find( WIID_WORLDMAP_STAGE , true );

	if( pWindow != NULL )
	{
		pWindow->AdjustCoord( m_iAX , m_iAY , pStageAttr->m_iMapWidth , pStageAttr->m_iMapHeight );

		if( pWindow->SetImage( strMapImage.c_str() , 0 , 0 ) == false )
		{
			g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_DEFAULT );
			return false;
		}
	}

	m_iCreateStageID	=	iStage;

	g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_DEFAULT );

	CreateEventInfo( true );

	return true;
}

void SPWindowWorldMap::CleanStage()
{
	SPWindow*	pWindow	=	Find( WIID_WORLDMAP_STAGE , true );

	if( pWindow != NULL )
	{
		pWindow->Clean();
	}

	m_iCreateStageID	=	-1;
}

void SPWindowWorldMap::HideCtrl()
{
	SPWindow*		pWindow;
	INSTANCE_ID		StartInstanceID;
	INSTANCE_ID		EndInstanceID;

	if( m_CurMoveID == WIID_WORLDMAP_WORLD )
	{
		StartInstanceID	=	WIID_WORLDMAP_CTRL_WORLD_ME;
		EndInstanceID	=	WIID_WORLDMAP_CTRL_WORLD_PARTY_END;
	}
	else if( m_CurMoveID == WIID_WORLDMAP_STAGE )
	{
		StartInstanceID	=	WIID_WORLDMAP_CTRL_STAGE_ME;
		EndInstanceID	=	WIID_WORLDMAP_CTRL_STAGE_PARTY_END;
	}

	int	iCount	=	EndInstanceID - StartInstanceID;

	for( int i = 0 ; i < iCount ; ++i )
	{
		pWindow	=	Find( StartInstanceID + i , true );

		if( pWindow == NULL )
			continue;

		pWindow->Hide();
	}
}

void SPWindowWorldMap::UpdateMeInfo()
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPTeamArchive*	pTeamArchive	=	pLocalPlayer->GetTeam();

	if( pTeamArchive == NULL )
		return;

	if( pTeamArchive->GetActive() == true )
		return;

	INSTANCE_ID		MeInstanceID;
	SPWindow*		pParentWindow;
	SPWindow*		pWindow;

	if( m_CurMoveID == WIID_WORLDMAP_WORLD )
	{
		MeInstanceID	=	WIID_WORLDMAP_CTRL_WORLD_ME;
		pParentWindow	=	Find( WIID_WORLDMAP_WORLD , true );
	}
	else if( m_CurMoveID == WIID_WORLDMAP_STAGE )
	{
		MeInstanceID	=	WIID_WORLDMAP_CTRL_STAGE_ME;
		pParentWindow	=	Find( WIID_WORLDMAP_STAGE , true );
	}

	pWindow	=	Find( MeInstanceID , true );

	if( pWindow == NULL )
	{
		pWindow	=	new	SPWindowStatic( MeInstanceID , 0 , 0 , WORLDMAP_ICON_X , WORLDMAP_ICON_Y , pParentWindow );
		pWindow->SetImage( "DATA/INTERFACE/WORLDMAP/INFO00.PNG" ,  0 , 0 );
	}

	int		iParentPosX , iParentPosY;

	if( pParentWindow == NULL )
		pParentWindow	=	this;

	pParentWindow->GetAbsCoord( iParentPosX , iParentPosY );

	if( m_CurMoveID == WIID_WORLDMAP_WORLD )
	{
		SPWindow*		pStageWindow	=	Find( WIID_WORLDMAP_CTRL_WORLD_LOCATION + pLocalPlayer->GetCurStageID() , true );

		if( pStageWindow != NULL )
		{
			RECT	rcStageWindow;

			pStageWindow->GetWindowRect( rcStageWindow );

			pWindow->AdjustCoord( ( rcStageWindow.left + rcStageWindow.right ) / 2 - WORLDMAP_ICON_X / 2 - iParentPosX , ( rcStageWindow.top + rcStageWindow.bottom ) / 2 - ( WORLDMAP_ICON_Y / 2 ) - iParentPosY );
			pWindow->Show();
		}
	}
	else if( m_CurMoveID == WIID_WORLDMAP_STAGE )
	{
		float fAPosX = pLocalPlayer->GetPosX();
		float fAPosY = pLocalPlayer->GetPosY();

		int	iCurStage		=	pLocalPlayer->GetCurStageID();
		int	iCurMapGroudID	=	pLocalPlayer->GetCurMapGroupID();

		if( iCurStage != m_iCreateStageID )
		{
			pWindow->Hide();
			return;
		}

		SPWorldMapMapAttr*	pMapAttr	=	GetMapAttr( iCurStage , iCurMapGroudID );

		if( pMapAttr == NULL )
		{
			pWindow->Hide();
			return;
		}

		int	iMapWidth	=	pMapAttr->m_iRealMapWidth;
		int	iMapHeight	=	pMapAttr->m_iRealMapHeight;

		if( pMapAttr->m_eType == MAP_LINK_HORIZONTAL )		//	¼öÆò¸Ê
			iMapWidth	*=	pMapAttr->m_iRealMapCount;
		else if( pMapAttr->m_eType == MAP_LINK_VERTICAL )	//	¼öÁ÷¸Ê
			iMapHeight	*=	pMapAttr->m_iRealMapCount;

		float	fRatioWidth		=	(float)pMapAttr->m_iWidth	/	(float)iMapWidth;
		float	fRatioHeight	=	(float)pMapAttr->m_iHeight	/	(float)iMapHeight;

		float	fRPosX	=	fAPosX	*	fRatioWidth;
		float	fRPosY	=	fAPosY	*	fRatioHeight;

		int	iRPosX = pMapAttr->m_iPosX	+	(int)fRPosX	-	WORLDMAP_ICON_X / 2;
		int iRPosY = pMapAttr->m_iPosY	+	(int)fRPosY	-	WORLDMAP_ICON_Y;

		pWindow->AdjustCoord( iParentPosX + iRPosX - iParentPosX , iParentPosY + iRPosY - iParentPosY );
		pWindow->Show();
	}
}

void SPWindowWorldMap::UpdateTeamInfo()
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPTeamArchive*	pTeamArchive	=	pLocalPlayer->GetTeam();

	if( pTeamArchive == NULL )
		return;

	if( pTeamArchive->GetActive() == false )
		return;

	int i;

	int	iTeamCount	=	pTeamArchive->GetTeamCount();
	std::vector< TEAM_MEMBER* >*	pvTeamMemberList = pTeamArchive->GetTeamList();
	TEAM_MEMBER*					pTeamMember;

	if( pvTeamMemberList == NULL )
		return;

	std::multimap< int , TEAM_MEMBER* >	mmTeamStage;
	UINT16	uiServerID	=	(UINT16)g_pNetworkManager->GetServerID();
	
	//	°°Àº À§Ä¡ÀÇ ¸â¹ö¸¦ ¹­¾î³õ´Â´Ù.
	for( i = 0 ; i < iTeamCount ; ++i )
	{
		pTeamMember	=	pvTeamMemberList->at( i );

		if( pTeamMember == NULL )
			continue;

#if !defined(_CHANNEL_TEAM_VERSION_)
		if( pTeamMember->wServerID != uiServerID )
			continue;
#endif

		mmTeamStage.insert( std::multimap< int , TEAM_MEMBER* >::value_type( pTeamMember->usStageID , pTeamMember ) );
	}

	INSTANCE_ID		TeamInstanceID;
	SPWindow*		pParentWindow;
	SPWindow*		pWindow;

	//	À§Ä¡¸¦ ÁöÁ¤ÇÑ´Ù.
	std::multimap< int , TEAM_MEMBER* >::iterator	mmIter	=	mmTeamStage.begin();

	int							iOldStage	=	-1;
	int							iCurStage;
	int							iCurNumber	=	0;
	int							iStageCount;
	POINT						ptPos;
	std::map< GU_ID , POINT >	mTeamPos;

	ptPos.x			=	0;
	ptPos.y			=	0;

	while ( mmIter != mmTeamStage.end() )
	{
		iCurStage	=	(*mmIter).first;
		if( iOldStage !=  iCurStage )
		{
			ptPos.x			=	0;
			ptPos.y			=	0;
			iCurNumber		=	0;
		}

		pTeamMember	=	((*mmIter).second);
		iStageCount	=	(int)mmTeamStage.count( iCurStage );
		ptPos.x		=	iCurNumber * WORLDMAP_ICON_X - ( iStageCount * WORLDMAP_ICON_X / 2 );
		ptPos.y		=	-( WORLDMAP_ICON_Y / 2 );

		if( iCurNumber != 0 )
			--ptPos.x;

		mTeamPos.insert( std::map< GU_ID , POINT >::value_type( pTeamMember->CharGUID , ptPos ) );

		iOldStage	=	iCurStage;
		++iCurNumber;
		++mmIter;
	}

	std::map< GU_ID , POINT >::iterator	mIter;
	SPWindow*		pStageWindow;
	RECT			rcStageWindow;
	int				iParentPosX , iParentPosY;

	SPGameObject*		pGOB;
	SPWorldMapMapAttr*	pMapAttr;
	float				fAPosX , fAPosY;
	float				fRPosX , fRPosY;
	float				fRatioWidth , fRatioHeight;
	int					iMapWidth , iMapHeight;
	int					iRPosX , iRPosY;
	int					iSrcX , iSrcY;

	for( i = 0 ; i < iTeamCount ; ++i )
	{
		if( m_CurMoveID == WIID_WORLDMAP_WORLD )
		{
			TeamInstanceID	=	WIID_WORLDMAP_CTRL_WORLD_PARTY;
			pParentWindow	=	Find( WIID_WORLDMAP_WORLD , true );
		}
		else if( m_CurMoveID == WIID_WORLDMAP_STAGE )
		{
			TeamInstanceID	=	WIID_WORLDMAP_CTRL_STAGE_PARTY;
			pParentWindow	=	Find( WIID_WORLDMAP_STAGE , true );
		}

		TeamInstanceID	+=	i;

		pWindow	=	Find( TeamInstanceID , true );

		if( pParentWindow == NULL )
			pParentWindow	=	this;

		pParentWindow->GetAbsCoord( iParentPosX , iParentPosY );

		if( pWindow == NULL )
		{
			pWindow	=	new	SPWindowStatic( TeamInstanceID , 0 , 0 , WORLDMAP_ICON_X , WORLDMAP_ICON_Y , pParentWindow );
			pWindow->SetImage( "DATA/INTERFACE/WORLDMAP/INFO00.PNG" ,  0 , 0 );
		}

		pTeamMember	=	pvTeamMemberList->at( i );

		if( pTeamMember == NULL )
		{
			pWindow->Hide();
			continue;
		}

		if( m_CurMoveID == WIID_WORLDMAP_WORLD )
		{
			pStageWindow	=	Find( WIID_WORLDMAP_CTRL_WORLD_LOCATION + pTeamMember->usStageID , true );

			if( pStageWindow != NULL )
			{
				pStageWindow->GetWindowRect( rcStageWindow );

				mIter	=	mTeamPos.find( pTeamMember->CharGUID );

				if( mIter != mTeamPos.end() )
				{
					ptPos	=	((*mIter).second);

					if( pLocalPlayer->GetGUID() == pTeamMember->CharGUID )
					{
						if( pTeamMember->byGrade == 0 )
						{
							iSrcX	=	0;
							iSrcY	=	0;
						}
						else
						{
							iSrcX	=	11;
							iSrcY	=	0;
						}
					}
					else
					{
						if( pTeamMember->byGrade == 0 )
						{
							iSrcX	=	33;
							iSrcY	=	0;
						}
						else
						{
							iSrcX	=	22;
							iSrcY	=	0;
						}
					}

					pWindow->SetImagePos( iSrcX , iSrcY );
					pWindow->AdjustCoord( ( rcStageWindow.left + rcStageWindow.right ) / 2 + ptPos.x - iParentPosX , ( rcStageWindow.top + rcStageWindow.bottom ) / 2 + ptPos.y - iParentPosY );
					pWindow->Show();
				}
			}
		}
		else if( m_CurMoveID == WIID_WORLDMAP_STAGE )
		{
#if !defined(_CHANNEL_TEAM_VERSION_)
			if( pTeamMember->wServerID != uiServerID )
			{
				pWindow->Hide();
				continue;
			}
#endif

			if( pTeamMember->usStageID != m_iCreateStageID )
			{
				pWindow->Hide();
				continue;
			}

			pMapAttr	=	GetMapAttr( pTeamMember->usStageID , pTeamMember->usMapGroupID );

			if( pMapAttr == NULL )
			{
				pWindow->Hide();
				continue;
			}

			pGOB	=	g_pGOBManager->Find( pTeamMember->CharGUID );

			if( pGOB == NULL )
			{
				fAPosX	=	pTeamMember->fPosX;
				fAPosY	=	pTeamMember->fPosY;
			}
			else
			{
				fAPosX = (float)pGOB->SPGOBPerformMessage( SPGM_GETPOSX );
				fAPosY = (float)pGOB->SPGOBPerformMessage( SPGM_GETPOSY );
			}

			iMapWidth	=	pMapAttr->m_iRealMapWidth;
			iMapHeight	=	pMapAttr->m_iRealMapHeight;

			if( pMapAttr->m_eType == MAP_LINK_HORIZONTAL )		//	¼öÆò¸Ê
				iMapWidth	*=	pMapAttr->m_iRealMapCount;
			else if( pMapAttr->m_eType == MAP_LINK_VERTICAL )	//	¼öÁ÷¸Ê
				iMapHeight	*=	pMapAttr->m_iRealMapCount;

			fRatioWidth		=	(float)pMapAttr->m_iWidth	/	(float)iMapWidth;
			fRatioHeight	=	(float)pMapAttr->m_iHeight	/	(float)iMapHeight;

			fRPosX	=	fAPosX	*	fRatioWidth;
			fRPosY	=	fAPosY	*	fRatioHeight;

			iRPosX = pMapAttr->m_iPosX	+	(int)fRPosX	-	WORLDMAP_ICON_X / 2;
			iRPosY = pMapAttr->m_iPosY	+	(int)fRPosY	-	WORLDMAP_ICON_Y;

			if( pLocalPlayer->GetGUID() == pTeamMember->CharGUID )
			{
				if( pTeamMember->byGrade == 0 )
				{
					iSrcX	=	0;
					iSrcY	=	0;
				}
				else
				{
					iSrcX	=	11;
					iSrcY	=	0;
				}
			}
			else
			{
				if( pTeamMember->byGrade == 0 )
				{
					iSrcX	=	33;
					iSrcY	=	0;
				}
				else
				{
					iSrcX	=	22;
					iSrcY	=	0;
				}
			}

			if( pMapAttr->m_bVisible[ WORLDMAP_MAP_EVENT_PARTY ] == false )
			{
				pWindow->Hide();
			}
			else
			{
				pWindow->Show();
			}

			pWindow->SetImagePos( iSrcX , iSrcY );
			pWindow->AdjustCoord( iParentPosX + iRPosX - iParentPosX , iParentPosY + iRPosY - iParentPosY );
		}
	}
}

void SPWindowWorldMap::CreateEventInfo( bool bDel )
{
	SPEventArchive*	pEventArchive	=	g_pEventManager->GetEventArchive();

	if( pEventArchive == NULL )
		return;

	if( bDel == true )
	{
		m_vMapEventInfo.clear();

		if( pEventArchive->FindEvent( &m_vMapEventInfo , m_iCreateStageID ) == false )
			return;
	}

	std::vector<MAPEVENT_INFO*>::iterator	Iter	=	m_vMapEventInfo.begin();
	int	iInstanceID	=	0;
	MAPEVENT_INFO*	pMapEventInfo;
	SPWindowStatic*	pWindowStatic;
	SPWindow*	pWindow;
	SPWindow*	pParentWindow	=	Find( WIID_WORLDMAP_STAGE , true );

	if( pParentWindow == NULL )
		pParentWindow = this;

	int	iWindowPosX , iWindowPosY;
	int	iWindowSizeX , iWindowSizeY;
	int	iWindowSrcX , iWindowSrcY;

	SPWorldMapMapAttr*	pMapAttr;

	int		iStageID , iMapGroupID;
	int		iMapWidth , iMapHeight;
	float	fRatioWidth , fRatioHeight;
	float	fMapEventPosX , fMapEventPosY;
	WORLDMAP_MAP_EVENT_TYPE	eMapEventType;

	while( Iter != m_vMapEventInfo.end() )
	{
		pMapEventInfo	=	(*Iter);
		++Iter;

		if( pMapEventInfo != NULL )
		{
			if( pMapEventInfo->byDynamicCreate != 0 )
				continue;

			iStageID	=	pMapEventInfo->mapInfo.uiStageID;
			iMapGroupID	=	pMapEventInfo->mapInfo.uiMapGroupID;

			pMapAttr	=	GetMapAttr( iStageID , iMapGroupID );

			if( pMapAttr != NULL )
			{
				switch( pMapEventInfo->eventType )
				{
				case EVENT_NPC:
				case EVENT_BEAUTY:
					eMapEventType	=	WORLDMAP_MAP_EVENT_NPC;
					iWindowSrcX		=	44;
					iWindowSrcY		=	0;
					iWindowSizeX	=	WORLDMAP_ICON_X;
					iWindowSizeY	=	WORLDMAP_ICON_Y;
					break;
				case EVENT_SAVEPOINT:
					eMapEventType	=	WORLDMAP_MAP_EVENT_SAVE;
					iWindowSrcX		=	11;
					iWindowSrcY		=	16;
					iWindowSizeX	=	WORLDMAP_ICON_X;
					iWindowSizeY	=	WORLDMAP_ICON_Y;
					break;
				case EVENT_PORTAL:
					eMapEventType	=	WORLDMAP_MAP_EVENT_PORTAL;
					iWindowSrcX		=	0;
					iWindowSrcY		=	16;
					iWindowSizeX	=	WORLDMAP_ICON_X;
					iWindowSizeY	=	WORLDMAP_ICON_Y;
					break;
				case EVENT_MAILBOX:
					eMapEventType	=	WORLDMAP_MAP_EVENT_MAILBOX;
					iWindowSrcX		=	44;
					iWindowSrcY		=	16;
					iWindowSizeX	=	WORLDMAP_ICON_X;
					iWindowSizeY	=	WORLDMAP_ICON_Y;
					break;
				default:
					continue;
				}

				pWindow	=	Find( WIID_WORLDMAP_CTRL_STAGE_ETC + iInstanceID , true );

				if( pWindow == NULL )
				{
					iMapWidth		=	pMapAttr->m_iRealMapWidth;
					iMapHeight		=	pMapAttr->m_iRealMapHeight;

					fMapEventPosX	=	pMapEventInfo->posX;
					fMapEventPosY	=	pMapEventInfo->posY;

					if( pMapAttr->m_eType == MAP_LINK_HORIZONTAL )		//	¼öÆò¸Ê
					{
						fMapEventPosX	+=	iMapWidth * pMapEventInfo->mapInfo.uiMapID;
						iMapWidth		*=	pMapAttr->m_iRealMapCount;
					}
					else if( pMapAttr->m_eType == MAP_LINK_VERTICAL )	//	¼öÁ÷¸Ê
					{
						fMapEventPosY	+=	iMapHeight * pMapEventInfo->mapInfo.uiMapID;
						iMapHeight		*=	pMapAttr->m_iRealMapCount;
					}

					fRatioWidth		=	(float)pMapAttr->m_iWidth	/	(float)iMapWidth;
					fRatioHeight	=	(float)pMapAttr->m_iHeight	/	(float)iMapHeight;

					fMapEventPosX	=	fMapEventPosX	*	fRatioWidth;
					fMapEventPosY	=	fMapEventPosY	*	fRatioHeight;

					iWindowPosX		= pMapAttr->m_iPosX	+	(int)fMapEventPosX	-	iWindowSizeX / 2;
					iWindowPosY		= pMapAttr->m_iPosY	+	(int)fMapEventPosY	-	iWindowSizeY;

					pWindowStatic	=	new	SPWindowStatic( WIID_WORLDMAP_CTRL_STAGE_ETC + iInstanceID , iWindowPosX , iWindowPosY , iWindowSizeX , iWindowSizeY , pParentWindow );
					pWindowStatic->SetImage( "DATA/INTERFACE/WORLDMAP/INFO00.PNG" ,  iWindowSrcX , iWindowSrcY );
					pWindow	=	(SPWindow*)pWindowStatic;
				}

				if( pWindow != NULL )
				{
					if( pMapAttr->m_bVisible[ eMapEventType ] == false )
						pWindow->Hide();
					else
						pWindow->Show();
				}
				
				++iInstanceID;
			}
		}
	}
}

void SPWindowWorldMap::SendRequestTeamInfo( float fTime )
{
	if( m_CurMoveID != WIID_WORLDMAP_STAGE )
	{
		m_fAccumulateTimeTeamInfo	=	0.0f;
		return;
	}

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
	{
		m_fAccumulateTimeTeamInfo	=	0.0f;
		return;
	}

	SPTeamArchive*	pTeamArchive	=	pLocalPlayer->GetTeam();

	if( pTeamArchive == NULL )
	{
		m_fAccumulateTimeTeamInfo	=	0.0f;
		return;
	}

	if( pTeamArchive->GetActive() == false )
	{
		m_fAccumulateTimeTeamInfo	=	0.0f;
		return;
	}

	m_fAccumulateTimeTeamInfo	+=	fTime;

	if( m_fAccumulateTimeTeamInfo < m_fLimitTimeTeamInfo )
		return;

	m_fAccumulateTimeTeamInfo	=	0.0f;

	int	iTeamCount	=	pTeamArchive->GetTeamCount();
	std::vector< TEAM_MEMBER* >* pvTeamMemberList = pTeamArchive->GetTeamList();
	TEAM_MEMBER*	pTeamMember;
	SPGameObject*	pGOB	=	NULL;

	for( int i = 0 ; i < iTeamCount ; ++i )
	{
		pTeamMember	=	pvTeamMemberList->at( i );

		if( pTeamMember->usStageID != m_iCreateStageID )
			continue;

		pGOB	=	g_pGOBManager->Find( pTeamMember->CharGUID );

		if( pGOB != NULL )
		{
			if( pGOB->IsLocalPlayer() == true )
				continue;
		}

#if !defined(_CHANNEL_TEAM_VERSION_)
		if( pTeamMember->wServerID != (UINT16)g_pNetworkManager->GetServerID() )
			continue;
#endif

		CPacket kPacket;
		kPacket.AddUINT32( TEAM_CS_UPDATEMEMBER );
		kPacket.AddUINT64( pTeamMember->CharGUID );
		g_pNetworkManager->GetPacketHandler()->PostMsg( &kPacket );
	}
}

void SPWindowWorldMap::InitWorldTootip()
{
	SPWindowWorldMapToolTip*	pWindowToolTip;

	pWindowToolTip	= new SPWindowWorldMapToolTip( WIID_WORLDMAP_CTRL_TOOLTIP , 0 , 0 , 216 , 288 , this );
	pWindowToolTip->Show();
}

void SPWindowWorldMap::ProcessEffect( float fTime )
{
	if( m_ptEffectWayPointDest.x	>= 0 && m_ptEffectWayPointDest.y	>= 0 )
	{
		if( m_cEffectWayPointColor.a > 0.0f )
		{
			m_cEffectWayPointColor.a	-=	fTime;
			m_fEffectWayPointScaleX		+=	fTime;
			m_fEffectWayPointScaleY		+=	fTime;
		}
		else
		{
			m_cEffectWayPointColor.a	=	1.0f;
			m_fEffectWayPointScaleX		=	1.0f;
			m_fEffectWayPointScaleY		=	1.0f;
		}
	}

	if( m_eEffectType == WORLDMAP_EFFECT_NULL )
		return;

	switch( m_eEffectType )
	{
	case WORLDMAP_EFFECT_DAMAGE:
		{
			SPWindow*		pWindowDamage		=	NULL;
			SPWindowStatic*	pWindowStaticDamage	=	NULL;

			if( m_CurMoveID == WIID_WORLDMAP_WORLD )
			{
				pWindowDamage	=	Find( WIID_WORLDMAP_CTRL_WORLD_DAMAGE , true );
			}
			else if( m_CurMoveID == WIID_WORLDMAP_STAGE )
			{
				pWindowDamage	=	Find( WIID_WORLDMAP_CTRL_STAGE_DAMAGE , true );
			}

			pWindowStaticDamage	=	(SPWindowStatic*)pWindowDamage;

			float	fEffectScaleStep	=	2.0f;
			float	fEffectScaleX		=	fEffectScaleStep;
			float	fEffectScaleY		=	fEffectScaleStep;

			bool	bComplete	=	FALSE;
			D3DXCOLOR	cDamageColor;	

			if( pWindowStaticDamage != NULL )
			{
				fEffectScaleX	=	pWindowStaticDamage->GetImageScaleX();
				fEffectScaleY	=	pWindowStaticDamage->GetIamgeScaleY();

				RECT	rtPos;

				pWindowStaticDamage->GetWindowRect( rtPos );

				if( pWindowStaticDamage->IsShow() == true &&
					rtPos.left	>=	0	&& rtPos.top	>=	0	&&
					rtPos.right	<=	800 && rtPos.bottom <=	600 )
				{
					bComplete		=	TRUE;
				}

				cDamageColor = pWindowStaticDamage->GetColor();
			}

			cDamageColor.a	-=	fTime;

			if( cDamageColor.a < 0.0f )
				cDamageColor.a	=	0.0f;

			if( bComplete == FALSE )
			{
				m_cEffectColor.a	-=	fTime;

				if( m_cEffectColor.a < 0.0f )
					m_cEffectColor.a	=	0.0f;
			}
			else
				m_cEffectColor.a	=	0.0f;

			fEffectScaleX		+=	fTime * 2.0f;
			fEffectScaleY		+=	fTime * 2.0f;

			if( fEffectScaleX > fEffectScaleStep )
				fEffectScaleX	=	fEffectScaleStep;

			if( fEffectScaleY > fEffectScaleStep )
				fEffectScaleY	=	fEffectScaleStep;

			if( pWindowStaticDamage != NULL )
			{
				pWindowStaticDamage->SetImageScaleX( fEffectScaleX );
				pWindowStaticDamage->SetImageScaleY( fEffectScaleY );
				pWindowStaticDamage->SetColor( cDamageColor );
			}

			if( ( bComplete == FALSE && m_cEffectColor.a == 0.0f ) ||
				( bComplete == TRUE && fEffectScaleX == fEffectScaleStep && fEffectScaleY == fEffectScaleStep ) )
			{
				m_eEffectType	=	WORLDMAP_EFFECT_NULL;
				m_cEffectColor	=	INITCOLOR;

				if( pWindowStaticDamage != NULL )
				{
					pWindowStaticDamage->Hide();
				}
			}
			
			break;
		}
	case WORLDMAP_EFFECT_STAGE_CLICK:
		{
			g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_WAIT );

			int	iSizeX	=	m_rcEffectWorldMapViewEnd.right		-	m_rcEffectWorldMapViewEnd.left;
			int	iSizeY	=	m_rcEffectWorldMapViewEnd.bottom	-	m_rcEffectWorldMapViewEnd.top;

			int	iCurSizeX	=	m_rcEffectWorldMapSrc.right		-	m_rcEffectWorldMapSrc.left;
			int	iCurSizeY	=	m_rcEffectWorldMapSrc.bottom	-	m_rcEffectWorldMapSrc.top;

			if( fTime < 1.0f )
				fTime	=	1.0f;

			int	iScaleX	=	(int)(8.0f	*	fTime);
			int	iScaleY	=	(int)(6.0f	*	fTime);

			if( iSizeX < iCurSizeX || iSizeY < iCurSizeY )
			{
				m_rcEffectWorldMapSrc.left		+=	iScaleX;
				m_rcEffectWorldMapSrc.right		-=	iScaleX;
				m_rcEffectWorldMapSrc.top		+=	iScaleY;
				m_rcEffectWorldMapSrc.bottom	-=	iScaleY;
			}

			int	iCurPosX	=	m_rcEffectWorldMapSrc.left + (int)( ( m_rcEffectWorldMapSrc.right	-	m_rcEffectWorldMapSrc.left	)	*	0.5f );
			int	iCurPosY	=	m_rcEffectWorldMapSrc.top + (int)( ( m_rcEffectWorldMapSrc.bottom	-	m_rcEffectWorldMapSrc.top	)	*	0.5f );

			int	iEndPosX	=	m_rcEffectWorldMapViewEnd.left + (int)( ( m_rcEffectWorldMapViewEnd.right	-	m_rcEffectWorldMapViewEnd.left	)	*	0.5f );
			int	iEndPosY	=	m_rcEffectWorldMapViewEnd.top + (int)( ( m_rcEffectWorldMapViewEnd.bottom	-	m_rcEffectWorldMapViewEnd.top	)	*	0.5f );

			if( iEndPosX - iCurPosX > iScaleX )
			{
				m_rcEffectWorldMapSrc.left	+=	iScaleX;
				m_rcEffectWorldMapSrc.right	+=	iScaleX;
			}
			else if( iEndPosX - iCurPosX < -iScaleX )
			{
				m_rcEffectWorldMapSrc.left	-=	iScaleX;
				m_rcEffectWorldMapSrc.right	-=	iScaleX;
			}

			if( iEndPosY - iCurPosY > iScaleY )
			{
				m_rcEffectWorldMapSrc.top	+=	iScaleY;
				m_rcEffectWorldMapSrc.bottom+=	iScaleY;
			}
			else if( iEndPosY - iCurPosY < -iScaleY )
			{
				m_rcEffectWorldMapSrc.top	-=	iScaleY;
				m_rcEffectWorldMapSrc.bottom-=	iScaleY;
			}

			if( iSizeX == iCurSizeX && iSizeY == iCurSizeY &&
				abs( m_rcEffectWorldMapSrc.left - m_rcEffectWorldMapViewEnd.left ) <= iScaleX &&
				abs( m_rcEffectWorldMapSrc.top	== m_rcEffectWorldMapViewEnd.top ) <= iScaleY &&
				abs( m_rcEffectWorldMapSrc.right	== m_rcEffectWorldMapViewEnd.right) <= iScaleX &&
				abs( m_rcEffectWorldMapSrc.bottom== m_rcEffectWorldMapViewEnd.bottom ) <= iScaleY )
			{
				m_cEffectColor.a	-=	( fTime * 0.1f );

				if( m_cEffectColor.a <= 0.0f )
				{
					m_eEffectType	=	WORLDMAP_EFFECT_NULL;
					m_cEffectColor	=	INITCOLOR;
					g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_DEFAULT );
					g_pMouseCursor->ResetWaitData();
				}
			}
			break;
		}
	case WORLDMAP_EFFECT_WORLD_CLICK:
		{
			g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_WAIT );

			if( fTime < 1.0f )
				fTime	=	1.0f;

			m_cEffectColor.a	+=	fTime;

			if( m_cEffectColor.a >= 1.0f )
			{
				int	iSizeX	=	m_rcEffectWorldMapViewEnd.right		-	m_rcEffectWorldMapViewEnd.left;
				int	iSizeY	=	m_rcEffectWorldMapViewEnd.bottom	-	m_rcEffectWorldMapViewEnd.top;

				int	iCurSizeX	=	m_rcEffectWorldMapSrc.right		-	m_rcEffectWorldMapSrc.left;
				int	iCurSizeY	=	m_rcEffectWorldMapSrc.bottom	-	m_rcEffectWorldMapSrc.top;

				if( fTime < 1.0f )
					fTime	=	1.0f;

				int	iScaleX	=	(int)(8.0f	*	fTime);
				int	iScaleY	=	(int)(6.0f	*	fTime);

				if( iSizeX > iCurSizeX || iSizeY > iCurSizeY )
				{
					m_rcEffectWorldMapSrc.left		-=	iScaleX;
					m_rcEffectWorldMapSrc.right		+=	iScaleX;
					m_rcEffectWorldMapSrc.top		-=	iScaleY;
					m_rcEffectWorldMapSrc.bottom	+=	iScaleY;
				}

				int	iCurPosX	=	m_rcEffectWorldMapSrc.left + (int)( ( m_rcEffectWorldMapSrc.right	-	m_rcEffectWorldMapSrc.left	)	*	0.5f );
				int	iCurPosY	=	m_rcEffectWorldMapSrc.top + (int)( ( m_rcEffectWorldMapSrc.bottom	-	m_rcEffectWorldMapSrc.top	)	*	0.5f );

				int	iEndPosX	=	m_rcEffectWorldMapViewEnd.left + (int)( ( m_rcEffectWorldMapViewEnd.right	-	m_rcEffectWorldMapViewEnd.left	)	*	0.5f );
				int	iEndPosY	=	m_rcEffectWorldMapViewEnd.top + (int)( ( m_rcEffectWorldMapViewEnd.bottom	-	m_rcEffectWorldMapViewEnd.top	)	*	0.5f );

				if( iEndPosX - iCurPosX > iScaleX )
				{
					m_rcEffectWorldMapSrc.left	+=	iScaleX;
					m_rcEffectWorldMapSrc.right	+=	iScaleX;
				}
				else if( iEndPosX - iCurPosX < -iScaleX )
				{
					m_rcEffectWorldMapSrc.left	-=	iScaleX;
					m_rcEffectWorldMapSrc.right	-=	iScaleX;
				}

				if( iEndPosY - iCurPosY > iScaleY )
				{
					m_rcEffectWorldMapSrc.top	+=	iScaleY;
					m_rcEffectWorldMapSrc.bottom+=	iScaleY;
				}
				else if( iEndPosY - iCurPosY < -iScaleY )
				{
					m_rcEffectWorldMapSrc.top	-=	iScaleY;
					m_rcEffectWorldMapSrc.bottom-=	iScaleY;
				}

				if( iSizeX == iCurSizeX && iSizeY == iCurSizeY &&
					abs( m_rcEffectWorldMapSrc.left - m_rcEffectWorldMapViewEnd.left ) <= iScaleX &&
					abs( m_rcEffectWorldMapSrc.top	== m_rcEffectWorldMapViewEnd.top ) <= iScaleY &&
					abs( m_rcEffectWorldMapSrc.right	== m_rcEffectWorldMapViewEnd.right) <= iScaleX &&
					abs( m_rcEffectWorldMapSrc.bottom== m_rcEffectWorldMapViewEnd.bottom ) <= iScaleY )
				{
					m_eEffectType	=	WORLDMAP_EFFECT_NULL;
					m_cEffectColor	=	INITCOLOR;
					g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_DEFAULT );
					g_pMouseCursor->ResetWaitData();

					WorldMapComplete();
				}

			}
		}
		break;
//	default:
	}

}

void SPWindowWorldMap::OnClickWorldMap()
{
	m_eEffectType		=	WORLDMAP_EFFECT_WORLD_CLICK;
	m_cEffectColor		=	INITCOLOR;
	m_cEffectColor.a	=	0.0f;

	MakeViewInfo( m_rcEffectWorldMapViewStart , m_rcEffectWorldMapViewEnd , m_iCreateStageID , false );

	m_rcEffectWorldMapDest.left		=	m_iAX;
	m_rcEffectWorldMapDest.top		=	m_iAY;
	m_rcEffectWorldMapDest.right	=	m_iAX + m_iSX;
	m_rcEffectWorldMapDest.bottom	=	m_iAY + m_iSY;

	m_rcEffectWorldMapSrc.left		=	m_rcEffectWorldMapViewStart.left;
	m_rcEffectWorldMapSrc.top		=	m_rcEffectWorldMapViewStart.top;
	m_rcEffectWorldMapSrc.right		=	m_rcEffectWorldMapViewStart.right;
	m_rcEffectWorldMapSrc.bottom	=	m_rcEffectWorldMapViewStart.bottom;
}

void SPWindowWorldMap::WorldMapComplete()
{
	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_WORLDMAP_CTRL_STAGE_VIEW , true );

	if( pWindow != NULL )
		pWindow->Hide();

	pWindow	=	Find( WIID_WORLDMAP_STAGE , true );

	if( pWindow != NULL )
		pWindow->Hide();

	m_CurMoveID	=	WIID_WORLDMAP_WORLD;

	if( PerformMouseMove( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() ) == 1 )
	{
		pWindow = g_pInterfaceManager->GetCursorWindow();

		if( pWindow != NULL )
			OnCursorEnter( pWindow->GetInstanceID() , NULL , NULL );
	}

	g_pMouseCursor->SetType( SPMouseCursor::MOUSE_TYPE_WAIT );
	g_pMouseCursor->ResetWaitData();

	AdjustPlayer();
}

bool SPWindowWorldMap::MakeViewInfo( RECT& rcOutStartPos , RECT& rcOutEndPos , int iStage , bool bZoom )
{
	SPWindow*	pWorldWindow	=	Find( WIID_WORLDMAP_WORLD , true );
	SPWindow*	pStageWindow	=	Find( iStage + WIID_WORLDMAP_CTRL_WORLD_LOCATION , true );

	if( pWorldWindow == NULL || pStageWindow == NULL )
		return false;

	int	iPosX , iPosY;
	pWorldWindow->GetAbsCoord( iPosX , iPosY );

	iPosX = abs( iPosX );
	iPosY = abs( iPosY );

	RECT	rcStartPos , rcEndPos;

	rcStartPos.left		=	iPosX;
	rcStartPos.top		=	iPosY;
	rcStartPos.right	=	rcStartPos.left	+	m_iSX;
	rcStartPos.bottom	=	rcStartPos.top	+	m_iSY;

	RECT	rcWorldPos , rcStagePos;

	pWorldWindow->GetWindowRect( rcWorldPos );
	pStageWindow->GetWindowRect( rcStagePos );

	if( abs( rcStagePos.right - rcStagePos.left ) == 0 || abs( rcStagePos.bottom - rcStagePos.top ) == 0 )
	{
		if( m_eEffectType == WORLDMAP_EFFECT_STAGE_CLICK || m_eEffectType == WORLDMAP_EFFECT_WORLD_CLICK )
		{
			SetRect( &m_rcEffectWorldMapViewStart , 0 , 0 , 0 , 0 );
			SetRect( &m_rcEffectWorldMapViewEnd , 0 , 0 , 0 , 0 );
			ProcessEffect( 100000.0f );
		}

		return true;
	}

	int	iCenterPosX	=	rcStagePos.left	+ ( (rcStagePos.right - rcStagePos.left) / 2 )	-	rcWorldPos.left;
	int	iCenterPosY	=	rcStagePos.top	+ ( (rcStagePos.bottom - rcStagePos.top) / 2 )	-	rcWorldPos.top;

	int	iStep		=	12;

	rcEndPos.left		=	iCenterPosX	-	8 * iStep;
	rcEndPos.right		=	iCenterPosX	+	8 * iStep;
	rcEndPos.top		=	iCenterPosY	-	6 * iStep;
	rcEndPos.bottom		=	iCenterPosY	+	6 * iStep;

	int	iDistance;

	int	iWorldWindowSizeX	=	rcWorldPos.right	-	rcWorldPos.left;
	int	iWorldWindowSizeY	=	rcWorldPos.bottom	-	rcWorldPos.top;

	if( rcEndPos.left < 0 )
	{
		rcEndPos.right	+=	-rcEndPos.left;
		rcEndPos.left	+=	-rcEndPos.left;
	}

	if( rcEndPos.top < 0 )
	{
		rcEndPos.bottom	+=	-rcEndPos.top;
		rcEndPos.top	+=	-rcEndPos.top;
	}

	if( rcEndPos.right > iWorldWindowSizeX )
	{
		iDistance	=	iWorldWindowSizeX - rcEndPos.right;

		rcEndPos.right	+=	iDistance;
		rcEndPos.left	+=	iDistance;
	}

	if( rcEndPos.bottom > iWorldWindowSizeY )
	{
		iDistance	=	iWorldWindowSizeY - rcEndPos.bottom;

		rcEndPos.bottom	+=	iDistance;
		rcEndPos.top	+=	iDistance;
	}

	if( rcStartPos.left	>	rcEndPos.left	||	rcStartPos.right	<	rcEndPos.right	||
		rcStartPos.top	>	rcEndPos.top	||	rcStartPos.bottom	<	rcEndPos.bottom  )
	{
		AdjustStage( iStage );

		MakeViewInfo( rcOutStartPos , rcOutEndPos , iStage , bZoom );

		return true;
	}

	if( rcEndPos.left < rcStartPos.left )
	{
		iDistance		=	rcStartPos.left - rcEndPos.left;
		rcEndPos.right	+=	iDistance;
		rcEndPos.left	+=	iDistance;
	}

	if( rcEndPos.top < rcStartPos.top )
	{
		iDistance		=	rcStartPos.top - rcEndPos.top;
		rcEndPos.bottom	+=	iDistance;
		rcEndPos.top	+=	iDistance;
	}

	if( rcEndPos.right > rcStartPos.right )
	{
		iDistance		=	rcEndPos.right - rcStartPos.right;
		rcEndPos.left	-=	iDistance;
		rcEndPos.right	-=	iDistance;
	}

	if( rcEndPos.bottom > rcStartPos.bottom )
	{
		iDistance		=	rcStartPos.bottom - rcEndPos.bottom;
		rcEndPos.top	-=	iDistance;
		rcEndPos.bottom	-=	iDistance;
	}

	if( bZoom == true )
	{
		rcOutStartPos.left		=	rcStartPos.left;
		rcOutStartPos.top		=	rcStartPos.top;
		rcOutStartPos.right		=	rcStartPos.right;
		rcOutStartPos.bottom	=	rcStartPos.bottom;

		rcOutEndPos.left		=	rcEndPos.left;
		rcOutEndPos.top			=	rcEndPos.top;
		rcOutEndPos.right		=	rcEndPos.right;
		rcOutEndPos.bottom		=	rcEndPos.bottom;
	}
	else
	{
		rcOutEndPos.left		=	rcStartPos.left;
		rcOutEndPos.top			=	rcStartPos.top;
		rcOutEndPos.right		=	rcStartPos.right;
		rcOutEndPos.bottom		=	rcStartPos.bottom;

		rcOutStartPos.left		=	rcEndPos.left;
		rcOutStartPos.top		=	rcEndPos.top;
		rcOutStartPos.right		=	rcEndPos.right;
		rcOutStartPos.bottom	=	rcEndPos.bottom;
	}

	return true;
}

void SPWindowWorldMap::AdjustPlayer()
{
	UpdateMeInfo();
	UpdateTeamInfo();

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPTeamArchive*	pTeamArchive	=	pLocalPlayer->GetTeam();

	if( pTeamArchive == NULL )
		return;

	INSTANCE_ID	InstanceMe;

	if( m_CurMoveID == WIID_WORLDMAP_WORLD )
	{
		if( pTeamArchive->GetActive() == true )
			InstanceMe	=	WIID_WORLDMAP_CTRL_WORLD_PARTY;
		else
			InstanceMe	=	WIID_WORLDMAP_CTRL_WORLD_ME;
	}
	else if( m_CurMoveID == WIID_WORLDMAP_STAGE )
	{
		if( pTeamArchive->GetActive() == true )
			InstanceMe	=	WIID_WORLDMAP_CTRL_STAGE_PARTY;
		else
			InstanceMe	=	WIID_WORLDMAP_CTRL_STAGE_ME;
	}
	else
		return;

	SPWindow*	pWindow	=	Find( InstanceMe , true );

	if( pWindow == NULL )
		return;

	RECT	rcMe;

	pWindow->GetWindowRect( rcMe );

	int		iSourPosX	=	rcMe.left + ( ( rcMe.right - rcMe.left ) / 2 );
	int		iSourPosY	=	rcMe.top + ( ( rcMe.bottom - rcMe.top ) / 2 );

	int		iCenterPosX	=	g_pVideo->GetScreenLenX() / 2;
	int		iCenterPosY	=	g_pVideo->GetScreenLenY() / 2;

	int		iAdjustPosX	=	iCenterPosX	-	iSourPosX;
	int		iAdjustPosY	=	iCenterPosY	-	iSourPosY;

	MoveScreenDistance( iAdjustPosX , iAdjustPosY );
}

void SPWindowWorldMap::AdjustStage( int iStage )
{
	SPWindow*	pStageWindow	=	Find( WIID_WORLDMAP_CTRL_WORLD_LOCATION + iStage , true );

	if( pStageWindow == NULL )
		return;

	RECT	rcMe;

	pStageWindow->GetWindowRect( rcMe );

	int		iSourPosX	=	rcMe.left + ( ( rcMe.right - rcMe.left ) / 2 );
	int		iSourPosY	=	rcMe.top + ( ( rcMe.bottom - rcMe.top ) / 2 );

	int		iCenterPosX	=	g_pVideo->GetScreenLenX() / 2;
	int		iCenterPosY	=	g_pVideo->GetScreenLenY() / 2;

	int		iAdjustPosX	=	iCenterPosX	-	iSourPosX;
	int		iAdjustPosY	=	iCenterPosY	-	iSourPosY;

	INSTANCE_ID	eOldCurMoveID	=	m_CurMoveID;

	m_CurMoveID	=	WIID_WORLDMAP_WORLD;

	MoveScreenDistance( iAdjustPosX , iAdjustPosY );

	m_CurMoveID	=	eOldCurMoveID;
}

void SPWindowWorldMap::AdjustMapEvent( int iDestStage , int iDestMapGroup , int iDestMapID , float fDestPosX , float fDestPosY )
{
	int		iFindMapEvent		=	FindMapEventInfo( iDestStage , iDestMapGroup , iDestMapID , fDestPosX , fDestPosY );

	if( iFindMapEvent == -1 )
		return;

	m_eWayPointInstanceID	=	(INSTANCE_ID)( WIID_WORLDMAP_CTRL_STAGE_ETC + iFindMapEvent );

	SPWindow*	pWindow	=	Find( m_eWayPointInstanceID , true );

	if( pWindow == NULL )
		return;

	RECT	rcMapEvent;

	pWindow->GetWindowRect( rcMapEvent );

	int		iSourPosX	=	rcMapEvent.left + ( ( rcMapEvent.right - rcMapEvent.left ) / 2 );
	int		iSourPosY	=	rcMapEvent.top + ( ( rcMapEvent.bottom - rcMapEvent.top ) / 2 );

	int		iCenterPosX	=	g_pVideo->GetScreenLenX() / 2;
	int		iCenterPosY	=	g_pVideo->GetScreenLenY() / 2;

	int		iAdjustPosX	=	iCenterPosX	-	iSourPosX;
	int		iAdjustPosY	=	iCenterPosY	-	iSourPosY;

	MoveScreenDistance( iAdjustPosX , iAdjustPosY );

	m_ptEffectWayPointDest.x	=	iSourPosX;
	m_ptEffectWayPointDest.y	=	iSourPosY;
}

int SPWindowWorldMap::FindMapEventInfo( int iDestStage , int iDestMapGroup , int iDestMapID , float fDestPosX , float fDestPosY )
{
	std::vector<MAPEVENT_INFO*>::iterator	Iter		=	m_vMapEventInfo.begin();

	MAPEVENT_INFO*			pMapEventInfo;

	int		iInstanceID		=	0;
	int		iSrcStage		=	-1;
	int		iSrcMapGroup	=	-1;
	int		iSrcMapID		=	-1;
	float	fDistancePosX	=	1.0f;
	float	fDistancePosY	=	1.0f;
	float	fDistanceStep	=	0.001f;

	while( Iter != m_vMapEventInfo.end() )
	{
		pMapEventInfo	=	(*Iter);

		if( pMapEventInfo != NULL )
		{
			if( pMapEventInfo->eventType == EVENT_NPC ||
				pMapEventInfo->eventType == EVENT_BEAUTY ||
				pMapEventInfo->eventType == EVENT_SAVEPOINT ||
				pMapEventInfo->eventType == EVENT_PORTAL ||
				pMapEventInfo->eventType == EVENT_MAILBOX
				)
			{
				iSrcStage		=	pMapEventInfo->mapInfo.uiStageID;
				iSrcMapGroup	=	pMapEventInfo->mapInfo.uiMapGroupID;
				iSrcMapID		=	pMapEventInfo->mapInfo.uiMapID;
				fDistancePosX	=	abs( fDestPosX - pMapEventInfo->posX );
				fDistancePosY	=	abs( fDestPosY - pMapEventInfo->posY );

				if( iSrcStage == iDestStage			&& iSrcMapGroup == iDestMapGroup &&	iSrcMapID	==	iDestMapID &&
					fDistancePosX <= fDistanceStep	&& fDistancePosY <= fDistanceStep )
				{
					return iInstanceID;
				}
			}
			else
			{
				--iInstanceID;
			}
		}

		++Iter;
		++iInstanceID;
	}

	return -1;
}
