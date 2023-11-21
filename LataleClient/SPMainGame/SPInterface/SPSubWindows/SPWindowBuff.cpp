// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-10-10    16:02 
//***************************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPGOBManager.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"

#include "SPEffect.h"
#include "SPEffectArchive.h"

#include "SPUIUnit.h"
#include "SPUIBuffUnit.h"
#include "SPUIUnitManager.h"

#include "SPWindowBuff.h"


SPWindowBuff::SPWindowBuff(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowBuff::SPWindowBuff(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_QUEST, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowBuff::~SPWindowBuff()
{
	Clean();
}

void SPWindowBuff::Init()
{
	m_strToolTip	= "[SPWindowBuff]";

	m_pUIUnit			=	NULL;
	m_fLimitTime		=	1.0f;
	m_fAccmulateTime	=	0.0f;

	InitIcon();
}

void SPWindowBuff::Clean()
{
	SPWindow::Clean();

	m_UIUnitManager.ClearAll( TRUE );
}

void SPWindowBuff::Process( float fTime )
{
//	SPWindow::Process( fTime );
	if( !m_bShow )
		return;

	ProcessBuffEffect();

	m_UIUnitManager.Process( fTime );

	ProcessToolTip( fTime );
}

void SPWindowBuff::Render( float fTime )
{
	if( m_bShow == false ) return;

	g_pVideo->Flush();

//	SPWindow::Render(fTime);
//	g_pVideo->Flush();

	m_UIUnitManager.Render();

	g_pVideo->Flush();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowBuff )
//	SPIMESSAGE_COMMAND( SPIM_LBUTTON_UP		,	OnLButtonUp		)
	SPIMESSAGE_COMMAND( SPIM_CURSOR_MOVE	,	OnCursorMove	)
	SPIMESSAGE_COMMAND( SPIM_CURSOR_OUT		,	OnCursorOut		)
	SPIMESSAGE_COMMAND( SPIM_POS_UPDATE		,	OnPosUpdate		)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

//int SPWindowBuff::OnLButtonUp( WPARAM wParam, LPARAM lParam )
//{
//	int			iXPos	=	LOWORD(lParam);
//	int			iYPos	=	HIWORD(lParam);
//	SPUIUnit*	pUIUnit	=	m_UIUnitManager.GetUnitMouseOver( iXPos , iYPos );
//
//	if( pUIUnit == NULL )
//		return 0;
//
//	if( pUIUnit->Use() == FALSE )
//		return 0;
//
//	return 1;
//}

int SPWindowBuff::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	int				iXPos			=	LOWORD(lParam);
	int				iYPos			=	HIWORD(lParam);
	SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();

	m_pUIUnit	=	m_UIUnitManager.GetUnitMouseOver( iXPos , iYPos );

	if( m_pUIUnit == NULL )
		pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, -2 );
	else
        pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)m_pUIUnit, -2 );

	return 1;
}

int SPWindowBuff::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	m_pUIUnit	=	NULL;
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, -2 );
	return 1;
}

int SPWindowBuff::OnPosUpdate( WPARAM wParam, LPARAM lParam )
{
	int				iXPos			=	LOWORD(lParam);
	int				iYPos			=	HIWORD(lParam);

	AdjustCoord( iXPos , iYPos );
	RefreshChildPos();

	SPUIUnit*	pUIUnit;
	int			iUnitNumber	=	0;

	for( int y = 0 ; y < BUFF_Y ; ++y )
	{
		for( int x = 0 ; x < BUFF_X ; ++x )
		{
			pUIUnit	=	m_UIUnitManager.GetUnit( iUnitNumber );

			++iUnitNumber;

			if( pUIUnit == NULL )
				continue;

			pUIUnit->SetPos(	m_iAX + BUFF_X_OFFSET + ( x * BUFF_X_GAP ) ,
				m_iAY + BUFF_Y_OFFSET + ( y * BUFF_Y_GAP ) );
		}
	}

	m_UIUnitManager.ReCalRect();

	return 1;
}

void SPWindowBuff::InitIcon()
{
	char				szResPath[]		=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
	SPUIBuffUnit*		pUIBuffUnit		=	NULL;

	m_UIUnitManager.SetBufferSize( BUFF_X * BUFF_Y );

	for( int y = 0 ; y < BUFF_Y ; y++ )
	{
		for( int x = 0 ; x < BUFF_X ; x++ )
		{
			pUIBuffUnit	=	new	SPUIBuffUnit;

			pUIBuffUnit->SetPos(	m_iAX + BUFF_X_OFFSET + ( x * BUFF_X_GAP ) ,
									m_iAY + BUFF_Y_OFFSET + ( y * BUFF_Y_GAP ) );

			pUIBuffUnit->SetTexture( szResPath );
			pUIBuffUnit->SetRectSrc( 400 , 20 , 432 , 52 );

			pUIBuffUnit->SetDisableTexture( szResPath );
			pUIBuffUnit->SetDisableRectSrc( 434 , 37 , 436 , 39 );

			pUIBuffUnit->SetCoolTimeTexture( szResPath );
			pUIBuffUnit->SetCoolTimeRectSrc( 434 , 37 , 436 , 39 );

			pUIBuffUnit->SetInstanceID( WIID_BUFF );
			pUIBuffUnit->SetTimeCheck( TRUE );
			pUIBuffUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIBuffUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );

			m_UIUnitManager.AddUnit( pUIBuffUnit );
		}
	}
}

void SPWindowBuff::ProcessBuffEffect()
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPEffectArchive*	pEffectArchive	=	pLocalPlayer->GetEffectArchive();

	if( pEffectArchive == NULL )
		return;

	SPEffectActivity*	pEffectActivity	=	pEffectArchive->GetFirstEffectActivity();
	int					iCurIndex		=	0;
	SPUIBuffUnit*		pUIBufUnit		=	NULL;
	SPEffect*			pEffect			=	NULL;
	int					iUIBuffCount	=	m_UIUnitManager.GetUnitCount();

	while ( pEffectActivity != NULL )
	{
		if( iCurIndex >= iUIBuffCount )
			break;

		pEffect		=	pEffectActivity->GetEffect();
		pUIBufUnit	=	(SPUIBuffUnit*)m_UIUnitManager.GetUnit( iCurIndex );

		if( pUIBufUnit == NULL )
			continue;

		if( pUIBufUnit->SetEffect( pEffect ) == TRUE )
		{
			pUIBufUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIBufUnit->SetBuffID( pEffectActivity->GetUniqueKey() );
			pUIBufUnit->SetTimeData( (int)( pEffectActivity->GetApplyTime() / 10.0f - pEffectActivity->GetAccmulateTime() ) );
			iCurIndex++;
		}

		pEffectActivity	=	pEffectArchive->GetNextEffectActivity();
	}

	for( int i = iCurIndex ; i < iUIBuffCount ; i++ )
	{
		pUIBufUnit	=	(SPUIBuffUnit*)m_UIUnitManager.GetUnit( i );

		if( pUIBufUnit == NULL )
			continue;

		pUIBufUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		pUIBufUnit->SetBuffID( 0 );
	}

	if( IsCursorIn( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() ) == false )
		return;

	pUIBufUnit	=	(SPUIBuffUnit*)m_UIUnitManager.GetUnitMouseOver( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() );
	SPUIBuffUnit*		pUIBufUnitToolTip	=	(SPUIBuffUnit*)m_pUIUnit;

	if( pUIBufUnit != NULL && pUIBufUnitToolTip != NULL )
	{
		if( pUIBufUnitToolTip->GetBuffID() == pUIBufUnit->GetBuffID() )
			return;
	}

	if( pUIBufUnit == NULL )
	{
		OnCursorOut( NULL , NULL );
		return;
	}

	m_pUIUnit	=	(SPUIUnit*)pUIBufUnit;
	SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();
	pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)m_pUIUnit, -2 );
}

void SPWindowBuff::ProcessToolTip( float fTime )
{
	m_fAccmulateTime	+=	fTime;

	if( m_fAccmulateTime < m_fLimitTime )
		return;

	m_fAccmulateTime	=	0.0f;

	if( IsCursorIn( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() ) == false )
		return;

	if( m_pUIUnit == NULL )
		return;

	SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();
	pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)m_pUIUnit, -2 );
}
