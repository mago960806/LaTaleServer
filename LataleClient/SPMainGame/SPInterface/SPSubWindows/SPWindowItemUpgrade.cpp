/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/05/19
	created:	2006:5:19:   12:53
	filename: 	SPWindowItemUpgrade.cpp
	
	purpose:	
*********************************************************************/

#include "SPCommon.h"
#include "SPUtil.h"

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <map>

//#include "SPGOBStatus.h"
#include "SPGOBManager.h"
//
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "SPCommandConvert.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"

#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"

#include "SPCheckManager.h"

#include "SPUIUnit.h"
#include "SPUIItemUnit.h"
#include "SPUIUnitManager.h"

#include "SPPlayerInvenArchive.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include <WinSock2.h>
#include "PacketID.h"
#include "Packet.h"
#include "PacketHandler.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowToolTip.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPInterfaceManager.h"

#include "SPDebug.h"

#include "SPWindowItemUpgrade.h"

SPWindowItemUpgrade::SPWindowItemUpgrade(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowItemUpgrade::SPWindowItemUpgrade(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_ITEM_UPGRADE, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowItemUpgrade::~SPWindowItemUpgrade()
{
	Clean();
}

void SPWindowItemUpgrade::Init()
{
	SPWindowStatic* pWindowStatic = NULL;

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_BG_TOP , 0 , 0 , 192 , 4 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 1 , 76 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_BG_BOTTOM , 0 , 208 , 192 , 4 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 1 , 86 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_BG_MIDDLE , 0 , 4 , 192 , 204 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 1 , 82 );
	pWindowStatic->SetSrcSize( 192 , 2 );

	InitContentsWindow( pWindowStatic , 0 , -4 );
	InitProgressWindow( pWindowStatic , 0 , -4 );
	InitUIItemUnit();
	UpdatePosUIItemUnit();

	m_eCurState							=	ITEM_UPGRADE_STATE_NULL;

	m_fItemToolTipAccmulateTime			=	0.0f;
	m_fItemToolTipMaxTime				=	0.05f;

	m_fMaterialToolTipAccmulateTime		=	0.0f;
	m_fMaterialToolTipMaxTime			=	0.05f;

	m_fProgressAccmulateTime			=	0.0f;
	m_fProgressMaxTime					=	100.0f;

	m_fProgressBrightAccmulateTime		=	0.0f;
	m_fProgressBrightMaxTime			=	2.0f;

	m_pUpgradeItem						=	NULL;

	for( int i = 0 ; i < MAX_ITEM_UPGRADE_MATERIAL ; ++i )
	{
		m_pMaterialItem[ i ]			=	NULL;
	}

	SetSuccessProbability( -1 );

	m_bUse	=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_ITEMUPGRADE );
}

void SPWindowItemUpgrade::Clean()
{
	SPWindow::Clean();

	m_UIUnitManager.ClearAll( TRUE );
}

void SPWindowItemUpgrade::Process(float fTime)
{
	if(!m_bShow) return;

	SPWindow::Process( fTime );

	m_UIUnitManager.Process( fTime );

	if( m_eCurState == ITEM_UPGRADE_STATE_READY_ITEM )
	{
		ProcessItemToolTip( fTime );
	}
	else if( m_eCurState == ITEM_UPGRADE_STATE_READY_MATERIAL )
	{
		ProcessMaterialToolTip( fTime );
	}
	else if( m_eCurState == ITEM_UPGRADE_STATE_UPGRADING )
	{
		ProcessProgress( fTime );
	}
	else if( m_eCurState == ITEM_UPGRADE_STATE_BRIGHTTING )
	{
		ProcessBright( fTime );
	}
}

void SPWindowItemUpgrade::Render(float fTime)
{
	if(!m_bShow) return;

	SPWindow::Render( fTime );

	g_pVideo->Flush();

	m_UIUnitManager.Render();

	g_pVideo->Flush();
}

void SPWindowItemUpgrade::Show(bool bWithChild)
{
	if( m_bUse == false )
		return;

	SPWindow::Show( bWithChild );

	DefaultHideWindow();
}

void SPWindowItemUpgrade::Hide(bool bSendServer /* = true */)
{
	OnCancel( NULL , NULL );

	SetUpgradeItem( NULL );

	SPWindow::Hide( bSendServer );

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
		OnMaterialCursorOut( NULL , NULL ,NULL );
}

void SPWindowItemUpgrade::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();

	UpdatePosUIItemUnit();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowItemUpgrade )

	SPIMESSAGE_COMMAND(	SPIM_ITEM_UPGRADE_CANCEL	,											OnCancel				)
	SPIMESSAGE_COMMAND(	SPIM_ITEM_UPGRADE_RESULT	,											OnResult				)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_ITEM_UPGRADE_CLOSE			,		OnClose					)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_ITEM_UPGRADE_ITEM_BOX		,		OnItemBox				)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_ITEM_UPGRADE_ITEM_BOX		,		OnItemBoxCancel			)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_ITEM_UPGRADE_RUN			,		OnRun					)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_ITEM_UPGRADE_CANCEL		,		OnCancel				)

	SPIMESSAGE_CONTROL( SPIM_CURSOR_ENTER			,	WIID_ITEM_UPGRADE_ITEM_BOX		,		OnItemCursorEnter		)
	SPIMESSAGE_CONTROL( SPIM_CURSOR_OUT				,	WIID_ITEM_UPGRADE_ITEM_BOX		,		OnItemCursorOut			)
	SPIMESSAGE_CONTROL( SPIM_CURSOR_ENTER			,	WIID_ITEM_UPGRADE_RESULT_BOX	,		OnResultCursorEnter		)
	SPIMESSAGE_CONTROL( SPIM_CURSOR_OUT				,	WIID_ITEM_UPGRADE_RESULT_BOX	,		OnResultCursorOut		)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_ITEM_UPGRADE_MATERIAL_BOX_START	,	WIID_ITEM_UPGRADE_MATERIAL_BOX_END	,	OnMaterial				)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DOWN		,	WIID_ITEM_UPGRADE_MATERIAL_BOX_START	,	WIID_ITEM_UPGRADE_MATERIAL_BOX_END	,	OnMaterialCancel		)

	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER		,	WIID_ITEM_UPGRADE_MATERIAL_BOX_START	,	WIID_ITEM_UPGRADE_MATERIAL_BOX_END	,	OnMaterialCursorEnter	)
	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT		,	WIID_ITEM_UPGRADE_MATERIAL_BOX_START	,	WIID_ITEM_UPGRADE_MATERIAL_BOX_END	,	OnMaterialCursorOut		)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowItemUpgrade::OnClose( WPARAM wParam, LPARAM lParam )
{
	Hide();

	return 1;
}

int SPWindowItemUpgrade::OnItemBox( WPARAM wParam, LPARAM lParam )
{
	if( m_eCurState == ITEM_UPGRADE_STATE_READY_ITEM		||
		m_eCurState == ITEM_UPGRADE_STATE_READY_MATERIAL	||
		m_eCurState == ITEM_UPGRADE_STATE_READY_UPGRADE		)
	{
		SPItem* pPickUpItem = GetInvenPickUpItem();

		if( pPickUpItem == NULL )
			return 1;

		SetUpgradeItem( pPickUpItem );
		OnItemCursorEnter( wParam , lParam );
	}

	return 1;
}

int SPWindowItemUpgrade::OnItemBoxCancel( WPARAM wParam, LPARAM lParam )
{
	if( m_eCurState == ITEM_UPGRADE_STATE_READY_ITEM		||
		m_eCurState == ITEM_UPGRADE_STATE_READY_MATERIAL	||
		m_eCurState == ITEM_UPGRADE_STATE_READY_UPGRADE		)
	{
		DefaultHideWindow();
		SetUpgradeItem( NULL );
		OnItemCursorOut( wParam , lParam );
	}

	return 1;
}

int SPWindowItemUpgrade::OnRun( WPARAM wParam, LPARAM lParam )
{
	if( m_eCurState != ITEM_UPGRADE_STATE_READY_UPGRADE )
		return 1;

	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return 1;

	if( g_pGOBManager->GetLocalPlayer()->GetCurAction() != ACTION_STOP )
	{
		SetError( GS_ITEM_UPGRADE_ACTION_STOP );
		return 1;
	}

	m_fProgressAccmulateTime	=	0.0f;
	m_fProgressMaxTime			=	2.0f / (float)( GetSuccessProbability( m_pUpgradeItem ) ) * 100.0f;

	m_fProgressBrightAccmulateTime	=	0.0f;
	m_fProgressBrightMaxTime		=	1.0f;

	m_eCurState	=	ITEM_UPGRADE_STATE_READY_UPGRADE;

	NextState();

	SendParentAction( ACTION_ENCHANTTING , 0 , true );

	SPWindow*	pWindow	=	Find( WIID_ITEM_UPGRADE_RUN , true );

	if( pWindow != NULL )
		pWindow->SetEnable( false );

	pWindow	=	Find( WIID_ITEM_UPGRADE_PROGRESS , true );

	if( pWindow != NULL )
	{
		pWindow->Show();

		pWindow	=	pWindow->Find( WIID_ITEM_UPGRADE_PROGRESS_BRIGHT );

		if( pWindow != NULL )
			pWindow->Hide();
	}

	return 1;
}

int SPWindowItemUpgrade::OnCancel( WPARAM wParam, LPARAM lParam )
{
	if( IsShow() == false )
		return 0;

	int	iType	=	(int)wParam;

	if( iType == 1 )
	{
		SetError( GS_ITEM_UPGRADE_OBSTACLE_CANCEL );
	}

	if( m_eCurState == ITEM_UPGRADE_STATE_UPGRADING || m_eCurState == ITEM_UPGRADE_STATE_BRIGHTTING )
	{
		DefaultHideWindow();
		SendParentAction( ACTION_STOP );

		m_eCurState	=	ITEM_UPGRADE_STATE_READY_ITEM;

		if( m_pUpgradeItem != NULL )
		{
			SetSuccessProbability( m_pUpgradeItem );
			NextState();
		}

		if( CheckMaterialCondition() == true )
		{
			NextState();
		}
	}
	else
	{
		SetUpgradeItem( NULL );
		DefaultHideWindow();
	}

	return 1;
}

int SPWindowItemUpgrade::OnResult( WPARAM wParam, LPARAM lParam )
{
	UINT8				uiError			=	(UINT8)lParam;
	GLOBAL_STRING_ID	eGlobalStringID	=	(GLOBAL_STRING_ID)wParam;

	if( eGlobalStringID > 0 )
		SetError( eGlobalStringID );

	m_eCurState	=	ITEM_UPGRADE_STATE_READY_ITEM;

	DefaultHideWindow();
	SetUpgradeItem( m_pUpgradeItem );

	g_pItemCluster->SetItemUpgradeState( SPIU_STATE_NULL );

	return 1;
}

int SPWindowItemUpgrade::OnItemCursorEnter( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	SPUIUnit*	pUIUnit	=	m_UIUnitManager.GetUnit( WIID_ITEM_UPGRADE_UIUNIT_POS_ITEM );

	if( pUIUnit == NULL )
		return 1;

	if( pUIUnit->GetStatus( SPUIUnit::STATUS_UI_UNIT_RENDER ) == false )
		return 1;

	if( m_pUpgradeItem == NULL )
		return 1;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)m_pUpgradeItem->GetItemStatus() );

	return 1;
}

int SPWindowItemUpgrade::OnItemCursorOut( WPARAM wParam, LPARAM lParam )
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );
	return 1;
}

int SPWindowItemUpgrade::OnResultCursorEnter( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	if( m_pUpgradeItem == NULL )
		return 1;

	if( m_pUpgradeItem == NULL || g_pItemCluster == NULL )
		return 1;

	SPItemStatus*	pItemStatus		=	m_pUpgradeItem->GetItemStatus();

	if( pItemStatus == NULL )
		return 1;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pItemStatus, SPWindowToolTip::TOOLTIP_SHOW_TYPE_ITEMUPGRADE );

	return 1;
}

int SPWindowItemUpgrade::OnResultCursorOut( WPARAM wParam, LPARAM lParam )
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );
	return 1;
}

int SPWindowItemUpgrade::OnMaterial( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( m_eCurState == ITEM_UPGRADE_STATE_READY_ITEM		||
		m_eCurState == ITEM_UPGRADE_STATE_READY_MATERIAL	||
		m_eCurState == ITEM_UPGRADE_STATE_READY_UPGRADE		)
	{
		SPItem* pPickUpItem = GetInvenPickUpItem();

		if( pPickUpItem == NULL )
			return 1;

		SetMaterialItem( pPickUpItem );
	}

	return 1;
}

int SPWindowItemUpgrade::OnMaterialCancel( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( m_eCurState == ITEM_UPGRADE_STATE_READY_ITEM		||
		m_eCurState == ITEM_UPGRADE_STATE_READY_MATERIAL	||
		m_eCurState == ITEM_UPGRADE_STATE_READY_UPGRADE		)
	{
		ResetMaterialData( iID - WIID_ITEM_UPGRADE_MATERIAL_BOX_START );
	}

	return 1;
}

int SPWindowItemUpgrade::OnMaterialCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	SPUIUnit*	pUIUnit	=	m_UIUnitManager.GetUnit( iID - WIID_ITEM_UPGRADE_MATERIAL_BOX_START );

	if( pUIUnit == NULL )
		return 1;

	if( pUIUnit->GetStatus( SPUIUnit::STATUS_UI_UNIT_RENDER ) == false )
		return 1;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pUIUnit, -2 );

	return 1;
}

int SPWindowItemUpgrade::OnMaterialCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, -2 );
	return 1;
}

void SPWindowItemUpgrade::InitContentsWindow( SPWindow* pParent , int iAdjustX , int iAdjustY )
{
	SPWindowStatic* pWindowStatic = NULL;
	SPWindowButton* pWindowButton = NULL;

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_TITLE , 17 + iAdjustX , 7 + iAdjustY , 75 , 15 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 204 , 297 );

	pWindowButton = new SPWindowButton( WIID_ITEM_UPGRADE_CLOSE , 171 + iAdjustX , 10 + iAdjustY , 12 , 12 , pParent );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 301 , 321 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 314 , 321 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 327 , 321 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 340 , 321 );

	pWindowButton = new SPWindowButton( WIID_ITEM_UPGRADE_RUN , 46 + iAdjustX , 188 + iAdjustY , 68 , 17 , pParent );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 346 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 346 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 346 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 346 , 494 );
	pWindowButton->SetEnable( false );

	pWindowButton = new SPWindowButton( WIID_ITEM_UPGRADE_CANCEL , 118 + iAdjustX , 188 + iAdjustY , 68 , 17 , pParent );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 415 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 415 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 415 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 415 , 494 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_SUCCESS_PROBABILITY_BG , 18 + iAdjustX , 152 + iAdjustY , 95 , 15 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 416 , 257 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_SUCCESS_PROBABILITY_TEXT , 74 + iAdjustX , 153 + iAdjustY , 22 , 15 , pParent );
	pWindowStatic->SetFormat( DT_VCENTER | DT_RIGHT );
	pWindowStatic->SetFontColor( D3DXCOLOR( 143.0f / 255.0f , 99.0f / 255.0f , 99.0f / 255.0f , 1.0f ) );
	pWindowStatic->SetWindowText("100");

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_SUCCESS_PROBABILITY_TEXT_BOLD , 75 + iAdjustX , 153 + iAdjustY , 22 , 15 , pParent );
	pWindowStatic->SetFormat( DT_VCENTER | DT_RIGHT );
	pWindowStatic->SetFontColor( D3DXCOLOR( 143.0f / 255.0f , 99.0f / 255.0f , 99.0f / 255.0f , 1.0f ) );
	pWindowStatic->SetWindowText("100");


	SPWindowStatic* pBGMiddlePanel = new SPWindowStatic( WIID_ITEM_UPGRADE_PANEL_CONTENTS , 2 + iAdjustX , 26 + iAdjustY , 188 , 122 , pParent );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_BALLOON_MAIN , 0 , 0 , 128 , 122 , pBGMiddlePanel );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 493 , 477 );
	pWindowStatic->SetSrcSize( 2 , 2 );

	InitBalloonWindow( pWindowStatic , 0 , 0 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_BALLOON_POINT , 128 , 55 , 6 , 12 , pBGMiddlePanel );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 339 , 346 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_BALLOON_CORNER_RIGHT_TOP , 127 , 0 , 1 , 1 , pBGMiddlePanel );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 462 , 424 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_BALLOON_CORNER_RIGHT_BOTTOM , 127 , 121 , 1 , 1 , pBGMiddlePanel );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 462 , 424 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_RESULT_BOX , 145 , 45 , 32 , 32 , pBGMiddlePanel );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 446 , 195 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_RESULT_BOX_MARK , 155 , 52 , 12 , 18 , pBGMiddlePanel );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 360 , 321 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_RESULT_PIC , 145 , 45 , 32 , 32 , pBGMiddlePanel );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_ITEM_TOOLTIP , -40 , 45 , 162 , 15 , pBGMiddlePanel );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 349 , 305 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_ITEM_TOOLTIP_CURSOR , 20 + 40 , 33 - 45 , 10 , 10 , pWindowStatic );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 354 , 260 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 365 , 260 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 376 , 260 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 387 , 260 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 398 , 260 );

	m_iCurItemToolTipImgIndex	=	0;
	m_iMaxItemToolTipImgIndex	=	5;
	m_iStepItemToolTipImgIndex	=	1;

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_MATERIAL_TOOLTIP , 16 , 126 , 158 , 15 , pBGMiddlePanel );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 353 , 289 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_MATERIAL_TOOLTIP_CURSOR , 88 - 16 , 114 - 126 , 10 , 10 , pWindowStatic );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 354 , 260 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 365 , 260 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 376 , 260 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 387 , 260 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 398 , 260 );

	m_iCurMaterialToolTipImgIndex	=	0;
	m_iMaxMaterialToolTipImgIndex	=	5;
	m_iStepMaterialToolTipImgIndex		=	1;

}

void SPWindowItemUpgrade::InitBalloonWindow( SPWindow* pParent , int iAdjustX , int iAdjustY )
{
	SPWindowStatic* pWindowStatic = NULL;

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_ITEM_BOX , 9 + iAdjustX , 4 + iAdjustY , 32 , 32 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 203 , 408 );
	pWindowStatic->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_ITEM_PIC , 9 + iAdjustX , 4 + iAdjustY , 32 , 32 , pParent );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_MATERIAL_BOX_START + 0 , 78 + iAdjustX , 4 + iAdjustY , 32 , 32 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 479 , 195 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 203 , 408 );
	pWindowStatic->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );

	pWindowStatic	= new SPWindowStatic( WIID_ITEM_UPGRADE_MATERIAL_PIC_START + 0 , 78 + iAdjustX , 4 + iAdjustY , 32 , 32 , pParent );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_MATERIAL_BOX_START + 1 , 86 + iAdjustX , 39 + iAdjustY , 32 , 32 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 479 , 195 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 203 , 408 );
	pWindowStatic->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_MATERIAL_PIC_START + 1 , 86 + iAdjustX , 39 + iAdjustY , 32 , 32 , pParent );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_MATERIAL_BOX_START + 2 , 78 + iAdjustX , 74 + iAdjustY , 32 , 32 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 479 , 195 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 203 , 408 );
	pWindowStatic->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_MATERIAL_PIC_START + 2 , 78 + iAdjustX , 74 + iAdjustY , 32 , 32 , pParent );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_MATERIAL_BOX_START + 3 , 43 + iAdjustX , 86 + iAdjustY , 32 , 32 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 479 , 195 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 203 , 408 );
	pWindowStatic->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_MATERIAL_PIC_START + 3 , 43 + iAdjustX , 86 + iAdjustY , 32 , 32 , pParent );
}

void SPWindowItemUpgrade::InitProgressWindow( SPWindow* pParent , int iAdjustX , int iAdjustY )
{
	SPWindowStatic* pWindowStatic = NULL;

	// 프로그래스바 전체 영역 [5/23/2006]
	SPWindowStatic* pProgressWindow = new SPWindowStatic( WIID_ITEM_UPGRADE_PROGRESS , -8 + iAdjustX , 151 + iAdjustY , 188 , 32 , pParent );

	iAdjustX	=	8;
	iAdjustY	=	-151;

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_PROGRESS_BG_LEFT , 12 + iAdjustX , 172 + iAdjustY , 3 , 11 , pProgressWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 236 , 390 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_PROGRESS_BG_MIDDLE , 15 + iAdjustX , 172 + iAdjustY , 162 , 11 , pProgressWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 241 , 390 );
	pWindowStatic->SetSrcSize( 2 , 11 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_PROGRESS_BG_RIGHT , 177 + iAdjustX , 172 + iAdjustY , 3 , 11 , pProgressWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 245 , 390 );

	m_iProgressGageWidth	=	163;

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_PROGRESS_GAGE , 14 + iAdjustX , 173 + iAdjustY , m_iProgressGageWidth , 9 , pProgressWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 251 , 391 );
	pWindowStatic->SetSrcSize( 1 , 9 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_PROGRESS_GAGE_RIGHT , 0 + iAdjustX , 173 + iAdjustY , 2 , 9 , pProgressWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 261 , 391 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_PROGRESS_GAGE_DECORATION , 0 + iAdjustX , 173 + iAdjustY , 4 , 9 , pProgressWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 256 , 391 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_PROGRESS_BRIGHT , 14 + iAdjustX , 173 + iAdjustY , 164 , 9 , pProgressWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 347 , 235 );

	SPWindowStatic* pNumberWindow = new SPWindowStatic( WIID_ITEM_UPGRADE_PROGRESS_NUMBER_BG , -8 + iAdjustX , 151 + iAdjustY , 31 , 20 , pProgressWindow );
	pNumberWindow->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 336 , 361 );

	pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_PROGRESS_NUMBER_PER , 22 , 6 , 6 , 5 , pNumberWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 264 , 291 );

	int iNumberXOffSet = 6;

	for( int i = 0 ; i < WIID_ITEM_UPGRADE_PROGRESS_NUMBER_COUNT ; ++i )
	{
		pWindowStatic = new SPWindowStatic( WIID_ITEM_UPGRADE_PROGRESS_NUMBER_START + i , 3 + iNumberXOffSet * i , 6 , 5 , 5 , pNumberWindow );

		for( int j = 0 ; j < 10 ; ++j )
		{
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 204 + j * 6 , 291 );
		}
	}
}

void SPWindowItemUpgrade::InitUIItemUnit()
{
	int	iUIItemUnitCount = WIID_ITEM_UPGRADE_MATERIAL_BOX_COUNT + 2;

	m_UIUnitManager.SetBufferSize( iUIItemUnitCount );

	SPUIItemUnit*	pUIItemUnit	=	NULL;
	char			szResPath[]		=	"DATA/INTERFACE/CONCEPT/UI100.PNG";

	for( int i = 0 ; i < iUIItemUnitCount ; ++i )
	{
		pUIItemUnit		=	new	SPUIItemUnit;

		pUIItemUnit->SetPos( 0 ,0 );
		pUIItemUnit->SetSize( 32 , 32 );

		pUIItemUnit->SetDisableTexture( szResPath );
		pUIItemUnit->SetDisableRectSrc( 434 , 37 , 436 , 39 );

		pUIItemUnit->SetCoolTimeTexture( szResPath );
		pUIItemUnit->SetCoolTimeRectSrc( 434 , 37 , 436 , 39 );

		if( i == WIID_ITEM_UPGRADE_UIUNIT_POS_RESULT )
            pUIItemUnit->SetInstanceID( WIID_ITEM_UPGRADE );
		else
			pUIItemUnit->SetInstanceID( WIID_ITEM );

		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );

		m_UIUnitManager.AddUnit( pUIItemUnit );
	}
}

void SPWindowItemUpgrade::UpdatePosUIItemUnit()
{
	SPUIItemUnit*	pUIItemUnit	=	NULL;
	SPWindow*		pWindow	=	NULL;
	RECT			rcPos;

	for( int i = 0 ; i < WIID_ITEM_UPGRADE_MATERIAL_BOX_COUNT ; ++i )
	{
		pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( i );

		if( pUIItemUnit == NULL )
			continue;

		pWindow	=	Find( WIID_ITEM_UPGRADE_MATERIAL_PIC_START + i , true );

		if( pWindow == NULL )
			continue;

		pWindow->GetWindowRect( rcPos );

		pUIItemUnit->SetPos( rcPos.left , rcPos.top );
		pUIItemUnit->ProcessCount();
	}

	pWindow	=	Find( WIID_ITEM_UPGRADE_ITEM_PIC , true );
	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( i );

	if( pWindow != NULL && pUIItemUnit != NULL )
	{
		pWindow->GetWindowRect( rcPos );
		pUIItemUnit->SetPos( rcPos.left , rcPos.top );
		pUIItemUnit->ProcessCount();
	}

	++i;

	pWindow	=	Find( WIID_ITEM_UPGRADE_RESULT_PIC , true );
	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( i );

	if( pWindow != NULL && pUIItemUnit != NULL )
	{
		pWindow->GetWindowRect( rcPos );
		pUIItemUnit->SetPos( rcPos.left , rcPos.top );
		pUIItemUnit->ProcessCount();
	}
}

void SPWindowItemUpgrade::SetSuccessProbability( SPItem* pItem )
{
	SetSuccessProbability( GetSuccessProbability( pItem ) );
}

void SPWindowItemUpgrade::SetSuccessProbability( int iPercentage )
{
	SPWindow* pWindow = NULL;

	pWindow = Find( WIID_ITEM_UPGRADE_SUCCESS_PROBABILITY_BG , true );

	if( pWindow != NULL )
	{
		if( iPercentage >= 0 )
			pWindow->Show();
		else
			pWindow->Hide();
	}

	char szBuf[_MAX_PATH];
	sprintf( szBuf, "%d", iPercentage);

	pWindow = Find( WIID_ITEM_UPGRADE_SUCCESS_PROBABILITY_TEXT , true );

	if( pWindow != NULL )
	{
		if( iPercentage >= 0 )
			pWindow->SetWindowText( szBuf );
		else
			pWindow->SetWindowText( NULL );
	}

	pWindow = Find( WIID_ITEM_UPGRADE_SUCCESS_PROBABILITY_TEXT_BOLD , true );

	if( pWindow != NULL )
	{
		if( iPercentage >= 0 )
			pWindow->SetWindowText( szBuf );
		else
			pWindow->SetWindowText( NULL );
	}
}

int SPWindowItemUpgrade::GetSuccessProbability( SPItem* pItem )
{
	int iPercentage	=	0;

	if( pItem == NULL || g_pItemCluster == NULL )
		return iPercentage;

	SPItemStatus*	pItemStatus		=	pItem->GetItemStatus();

	if( pItemStatus == NULL )
		return iPercentage;

	SPItemUpgrade*	pItemUpgrade	=	pItemUpgrade	=	g_pItemCluster->GetItemUpgrade( pItemStatus->GetItemID() , pItemStatus->GetRareLevel() );

	if( pItemUpgrade == NULL )
		return iPercentage;

	int iOptPoint				= (int)(pItemStatus->SPSendMessage( MV_ITEM_GET_OPTPOINT ));
	int	iEnchant_Probability	=	0;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		SPGOBStatus* pPlayerStatus	=	pLocalPlayer->GetStatus();

		if( pPlayerStatus != NULL )
			iEnchant_Probability	=	pPlayerStatus->GetStatusValue( STATUS_PROB_ENCHANT );
	}
	
	iPercentage	= iOptPoint - pItemUpgrade->m_iProbabilityModifier + iEnchant_Probability;

	if( iPercentage < pItemUpgrade->m_iProbabilityMin )
		iPercentage = pItemUpgrade->m_iProbabilityMin;
	else if( iPercentage > pItemUpgrade->m_iProbabilityMax + iEnchant_Probability )
		iPercentage = pItemUpgrade->m_iProbabilityMax + iEnchant_Probability;

	if( iPercentage > 100 )
		iPercentage	=	100;

	return iPercentage;
}

void SPWindowItemUpgrade::SetProgressMaxTime( float fMaxTime )
{
	m_fProgressMaxTime	=	fMaxTime;
}

void SPWindowItemUpgrade::ProcessItemToolTip( float fTime )
{
	m_fItemToolTipAccmulateTime	+=	fTime;

	if( m_fItemToolTipAccmulateTime >= m_fItemToolTipMaxTime )
	{
		m_fItemToolTipAccmulateTime	=	0.0f;
		m_iCurItemToolTipImgIndex	+=	m_iStepItemToolTipImgIndex;

		if( m_iCurItemToolTipImgIndex >= m_iMaxItemToolTipImgIndex - 1 )
		{
			m_iStepItemToolTipImgIndex	=	-1;
		}

		if( m_iCurItemToolTipImgIndex <= 0 )
		{
			m_iStepItemToolTipImgIndex	=	1;
		}

		SPWindow*	pWindow	=	Find( WIID_ITEM_UPGRADE_ITEM_TOOLTIP_CURSOR , true );

		if( pWindow != NULL )
		{
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , m_iCurItemToolTipImgIndex );
		}
	}
}

void SPWindowItemUpgrade::ProcessMaterialToolTip( float fTime )
{
	m_fMaterialToolTipAccmulateTime	+=	fTime;

	if( m_fMaterialToolTipAccmulateTime >= m_fMaterialToolTipMaxTime )
	{
		m_fMaterialToolTipAccmulateTime	=	0.0f;
		m_iCurMaterialToolTipImgIndex	+=	m_iStepMaterialToolTipImgIndex;

		if( m_iCurMaterialToolTipImgIndex >= m_iMaxMaterialToolTipImgIndex - 1 )
		{
			m_iStepMaterialToolTipImgIndex	=	-1;
		}

		if( m_iCurMaterialToolTipImgIndex <= 0 )
		{
			m_iStepMaterialToolTipImgIndex	=	1;
		}

		SPWindow*	pWindow	=	Find( WIID_ITEM_UPGRADE_MATERIAL_TOOLTIP_CURSOR , true );

		if( pWindow != NULL )
		{
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , m_iCurMaterialToolTipImgIndex );
		}
	}
}

void SPWindowItemUpgrade::ProcessProgress( float fTime )
{
	if( m_fProgressMaxTime <= 0.0f || m_fProgressAccmulateTime > m_fProgressMaxTime )
	{
		NextState();
		return;
	}

	m_fProgressAccmulateTime	+=	fTime;

	SPWindow*	pWindowProgress		=	Find( WIID_ITEM_UPGRADE_PROGRESS , true );

	if( pWindowProgress == NULL )
		return;

	int	iProgressParentPosX , iProgressParentPosY;

	pWindowProgress->GetAbsCoord( iProgressParentPosX , iProgressParentPosY );

	SPWindow*	pWindowProgressGage				=	pWindowProgress->Find( WIID_ITEM_UPGRADE_PROGRESS_GAGE );
	SPWindow*	pWindowProgressGageRight		=	pWindowProgress->Find( WIID_ITEM_UPGRADE_PROGRESS_GAGE_RIGHT );
	SPWindow*	pWindowProgressGageDecoration	=	pWindowProgress->Find( WIID_ITEM_UPGRADE_PROGRESS_GAGE_DECORATION );
	SPWindow*	pWindowProgressNumber			=	pWindowProgress->Find( WIID_ITEM_UPGRADE_PROGRESS_NUMBER_BG );

	if( pWindowProgressGage == NULL || pWindowProgressGageRight == NULL || pWindowProgressGageDecoration == NULL || pWindowProgressNumber == NULL )
		return;

	// 게이지 너비 변경 [5/23/2006]

	RECT	rcProgressGage;

	int	iProgressGageCurWidth	=	(int)( ( m_fProgressAccmulateTime / m_fProgressMaxTime ) * (float)( m_iProgressGageWidth ));

	pWindowProgressGage->GetWindowRect( rcProgressGage );
	pWindowProgressGage->SetRectSize( iProgressGageCurWidth , rcProgressGage.bottom - rcProgressGage.top );
	pWindowProgressGage->GetWindowRect( rcProgressGage );

	// 게이지 오른쪽 위치 변경 [5/23/2006]

	RECT	rcProgressGageRight;

	pWindowProgressGageRight->GetWindowRect( rcProgressGageRight );
	pWindowProgressGageRight->AdjustCoord( rcProgressGage.right - iProgressParentPosX , rcProgressGageRight.top - iProgressParentPosY );
	pWindowProgressGageRight->GetWindowRect( rcProgressGageRight );

	//	게이지 오른쪽 데코레이션 위치 변경 [5/23/2006]

	RECT	rcProgressGageDecoration;

	pWindowProgressGageDecoration->GetWindowRect( rcProgressGageDecoration );
	pWindowProgressGageDecoration->AdjustCoord( rcProgressGage.right - ( rcProgressGageDecoration.right - rcProgressGageDecoration.left ) - iProgressParentPosX , rcProgressGageDecoration.top - iProgressParentPosY );

	//	게이지 퍼센트 숫자 위치 변경 [5/23/2006]
	
	RECT	rcProgressNumber;

	pWindowProgressNumber->GetWindowRect( rcProgressNumber );
	pWindowProgressNumber->AdjustCoord( iProgressGageCurWidth , rcProgressNumber.top - iProgressParentPosY );

	//	게이지 퍼센트 숫자 변경 [5/23/2006]

	SPWindow*	pWindowProgressNumberValue;

	int iProgressNumber	= (int)( ( m_fProgressAccmulateTime / m_fProgressMaxTime ) * 100.0f );

	if( iProgressNumber > 999 )
		iProgressNumber	= 999;

	int	iNumberValue[ 3 ];

	iNumberValue[ 0 ]	= iProgressNumber / 100;
	iProgressNumber		-=	iNumberValue[ 0 ] * 100;
	iNumberValue[ 1 ]	= iProgressNumber / 10;
	iProgressNumber		-=	iNumberValue[ 1 ] * 00;
	iNumberValue[ 2 ]	= iProgressNumber % 10;

	for( int i = 0 ; i < WIID_ITEM_UPGRADE_PROGRESS_NUMBER_COUNT ; ++i )
	{
		pWindowProgressNumberValue	=	pWindowProgressNumber->Find( WIID_ITEM_UPGRADE_PROGRESS_NUMBER_START + i );

		if( pWindowProgressNumberValue == NULL )
			continue;

		pWindowProgressNumberValue->Show();
		pWindowProgressNumberValue->SPSendMessage( SPIM_SET_IMAGEINDEX , iNumberValue[ i ] );
	}

	if( iNumberValue[ 0 ] == 0 )
	{
		pWindowProgressNumberValue	=	pWindowProgressNumber->Find( WIID_ITEM_UPGRADE_PROGRESS_NUMBER_START + 0 );

		if( pWindowProgressNumberValue != NULL )
		{
			pWindowProgressNumberValue->Hide();
		}

		if( iNumberValue[ 1 ] == 0 )
		{
			pWindowProgressNumberValue	=	pWindowProgressNumber->Find( WIID_ITEM_UPGRADE_PROGRESS_NUMBER_START + 1 );

			if( pWindowProgressNumberValue != NULL )
			{
				pWindowProgressNumberValue->Hide();
			}
		}
	}

	pWindowProgress->RefreshRelationCoord();
}

void SPWindowItemUpgrade::ProcessBright( float fTime )
{
	if( m_fProgressBrightMaxTime <= 0.0f || m_fProgressBrightAccmulateTime > m_fProgressBrightMaxTime )
	{
		NextState();
		return;
	}

	m_fProgressBrightAccmulateTime	+=	fTime;

	SPWindow*	pWindow	= Find( WIID_ITEM_UPGRADE_PROGRESS_BRIGHT , true );

	if( pWindow == NULL )
		return;

	pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , ( m_fProgressBrightMaxTime - m_fProgressBrightAccmulateTime ) / m_fProgressBrightMaxTime ) );
	pWindow->Show();
}

void SPWindowItemUpgrade::DefaultHideWindow()
{
	SPWindow*	pWindow	=	NULL;

	pWindow		=	Find( WIID_ITEM_UPGRADE_PROGRESS , true );

	if( pWindow != NULL )
	{
		pWindow->Hide();
	}

	pWindow	=	Find( WIID_ITEM_UPGRADE_RUN , true );

	if( pWindow != NULL )
	{
		pWindow->SetEnable( false );
	}

	pWindow	=	Find( WIID_ITEM_UPGRADE_MATERIAL_TOOLTIP , true );

	if( pWindow != NULL )
	{
		pWindow->Hide();
	}

	SetSuccessProbability( -1 );
}

void SPWindowItemUpgrade::ShowProgressWindow()
{
	SPWindow*	pWindowProgress		=	Find( WIID_ITEM_UPGRADE_PROGRESS , true );

	if( pWindowProgress == NULL )
		return;

	pWindowProgress->Show();

	SPWindow*	pWindow	=	pWindowProgress->Find( WIID_ITEM_UPGRADE_PROGRESS_BRIGHT );

	if( pWindow != NULL )
	{
		pWindow->Hide();
	}
}

void SPWindowItemUpgrade::NextState()
{
	m_eCurState	=	(ITEM_UPGRADE_STATE)( (int)m_eCurState + 1 );

	if( (int)m_eCurState >= ITEM_UPGRADE_STATE_END )
		m_eCurState	=	ITEM_UPGRADE_STATE_NULL;

	SPWindow*	pWindow	=	NULL;

	if( m_eCurState == ITEM_UPGRADE_STATE_READY_MATERIAL )
	{
		pWindow	=	Find( WIID_ITEM_UPGRADE_ITEM_TOOLTIP , true );

		if( pWindow != NULL )
			pWindow->Hide();

		pWindow	=	Find( WIID_ITEM_UPGRADE_MATERIAL_TOOLTIP , true );

		if( pWindow != NULL )
			pWindow->Show();
	}

	if( m_eCurState == ITEM_UPGRADE_STATE_READY_UPGRADE )
	{
		pWindow	=	Find( WIID_ITEM_UPGRADE_RUN , true );

		if( pWindow != NULL )
			pWindow->SetEnable( true );

		pWindow	=	Find( WIID_ITEM_UPGRADE_ITEM_TOOLTIP , true );

		if( pWindow != NULL )
			pWindow->Hide();

		pWindow	=	Find( WIID_ITEM_UPGRADE_MATERIAL_TOOLTIP , true );

		if( pWindow != NULL )
			pWindow->Hide();
	}

	if( m_eCurState == ITEM_UPGRADE_STATE_UPGRADING )
		ShowProgressWindow();

	if( m_eCurState == ITEM_UPGRADE_STATE_UPGRADING_WAIT )
		SendServerRun();
}

void SPWindowItemUpgrade::SetUpgradeItem( SPItem* pUpgradeItem )
{
	//	새로온 아이템의 업그레이드 가능 유무를 확인한다.
	SPItemAttr*		pUpgradeItemAttr	=	NULL;
	SPItemStatus*	pUpgradeItemStatus	=	NULL;
	SPItemUpgrade*	pItemUpgrade		=	NULL;

	if( pUpgradeItem != NULL )
	{
		pUpgradeItemAttr	=	pUpgradeItem->GetItemAttr();
		pUpgradeItemStatus	=	pUpgradeItem->GetItemStatus();

		if( pUpgradeItemAttr != NULL && pUpgradeItemStatus != NULL && g_pItemCluster != NULL )
		{
			pItemUpgrade	=	g_pItemCluster->GetItemUpgrade( pUpgradeItemStatus->GetItemID() , pUpgradeItemStatus->GetRareLevel() );

			if( pItemUpgrade == NULL )
			{
				if( m_pUpgradeItem == NULL )
					SetError( GS_ITEM_UPGRADE_NOT_UPGRADE );

				pUpgradeItem	=	NULL;
			}

			// 아이템 유효성 체크 [10/31/2006 AJJIYA]
			if( pUpgradeItemStatus->IsTimeAvailable() == false )
			{
				SetError( GS_ITEM_UPGRADE_ITEM_EXPIRE , pUpgradeItemAttr->m_strName.c_str() );
				return;
			}
		}
	}

	SetSuccessProbability( -1 );

	if( m_pUpgradeItem != NULL )
	{
		SetInvenItemAllUnlock();
		ResetMaterialData();
	}

	m_pUpgradeItem	=	pUpgradeItem;
	m_eCurState		=	ITEM_UPGRADE_STATE_READY_ITEM;

	//////////////////////////////////////////////////////////////////////////
	
	SPUIUnit*	pUIUnit	=	NULL;

	for( int i = 0 ; i < m_UIUnitManager.GetUnitCount() ; ++i )
	{
		pUIUnit	=	m_UIUnitManager.GetUnit( i );

		if( pUIUnit != NULL )
		{
			pUIUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	
	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_ITEM_UPGRADE_ITEM_BOX , true );

	if( pWindow == NULL )
		return;

	pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );

	//////////////////////////////////////////////////////////////////////////
	
	pWindow	=	Find( WIID_ITEM_UPGRADE_ITEM_TOOLTIP , true );

	if( pWindow == NULL )
		return;

	pWindow->Show();

	pWindow	=	Find( WIID_ITEM_UPGRADE_MATERIAL_TOOLTIP , true );

	if( pWindow == NULL )
		return;

	pWindow->Hide();

	//////////////////////////////////////////////////////////////////////////

	SPWindow*		pMaterialPanel		=	Find( WIID_ITEM_UPGRADE_BALLOON_MAIN , true );

	if( pMaterialPanel == NULL )
		return;

	//////////////////////////////////////////////////////////////////////////
	
	for( int i = 0 ; i < MAX_ITEM_UPGRADE_MATERIAL ; ++i )
	{
		pWindow	=	pMaterialPanel->Find( WIID_ITEM_UPGRADE_MATERIAL_BOX_START + i );

		if( pWindow != NULL )
		{
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );
			pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );
			pWindow->Show();
		}
	}

	//////////////////////////////////////////////////////////////////////////

	SPWindow* pBGMiddlePanel = Find( WIID_ITEM_UPGRADE_PANEL_CONTENTS , true );

	if( pBGMiddlePanel == NULL )
		return;

	pWindow	=	pBGMiddlePanel->Find( WIID_ITEM_UPGRADE_RESULT_BOX_MARK );

	if( pWindow != NULL )
		pWindow->Show();

	//////////////////////////////////////////////////////////////////////////
	
	if( g_pItemCluster == NULL )
		return;

	if( pUpgradeItem == NULL )
		return;

	if( pUpgradeItemAttr == NULL || pUpgradeItemStatus == NULL )
		return;

	if( pItemUpgrade == NULL )
	{
		SetError( GS_ITEM_UPGRADE_NOT_UPGRADE );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	
	if( pUpgradeItem->GetContainerType() != INVENTORY_EQUIP &&
		pUpgradeItem->GetContainerType() != INVENTORY_CONSUME &&
		pUpgradeItem->GetContainerType() != INVENTORY_ETC &&
		pUpgradeItem->GetContainerType() != INVENTORY_EVENT )
	{
		SetError( GS_ITEM_UPGRADE_ONLY_ITEM_WINDOW );
		return;
	}

	bool		bItemIDDifferent	=	true;
	SPItemAttr* pResultItemAttr		=	NULL;

	if( g_pItemCluster->GetItemInfo( pItemUpgrade->m_iResult , pResultItemAttr ) == true )
	{
		if( pResultItemAttr != NULL )
		{
			if( pResultItemAttr->m_eType == pUpgradeItemAttr->m_eType )
			{
				bItemIDDifferent	=	false;
			}
		}
	}

	if( bItemIDDifferent == true )
	{
		SetError( GS_ITEM_UPGRADE_ITEM_TYPE_DIFFERENT );
		return;
	}
    
	//////////////////////////////////////////////////////////////////////////
	
	pWindow	=	Find( WIID_ITEM_UPGRADE_ITEM_BOX , true );

	if( pWindow == NULL )
		return;

	pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );

	SPUIItemUnit*	pUIItemUnit	=	NULL;

	pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( WIID_ITEM_UPGRADE_UIUNIT_POS_ITEM ) );

	if( pUIItemUnit != NULL )
	{
		pUIItemUnit->SetItemID( pUpgradeItemStatus->GetItemID() );
		pUIItemUnit->ReResourceLoad();
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
	}

	SetSuccessProbability( pUpgradeItem );

	//////////////////////////////////////////////////////////////////////////
	//
	//	재료 표시
	//

	SPItemAttr*		pMaterialItemAttr	=	NULL;
	SPWindow*		pWindowMaterialBox	=	NULL;

	for( i = 0 ; i < MAX_ITEM_UPGRADE_MATERIAL ; ++i )
	{
		pWindowMaterialBox	=	pMaterialPanel->Find( WIID_ITEM_UPGRADE_MATERIAL_BOX_START + i );

		if( pWindowMaterialBox == NULL )
			continue;

		pWindowMaterialBox->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );

		if( pItemUpgrade->m_stMaterial[ i ].m_iID == 0 || pItemUpgrade->m_stMaterial[ i ].m_iCount == 0 )
		{
			pWindowMaterialBox->Hide();
			continue;
		}

		pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( i ) );

		if( pUIItemUnit != NULL )
		{
			if( pItemUpgrade->m_stMaterial[ i ].m_iCount > 1)
				pUIItemUnit->SetCount( pItemUpgrade->m_stMaterial[ i ].m_iCount );
			else
				pUIItemUnit->SetCount( 0 );

			pUIItemUnit->SetItemID( pItemUpgrade->m_stMaterial[ i ].m_iID );
			pUIItemUnit->ReResourceLoad();
			pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
		}

	}
	
	//////////////////////////////////////////////////////////////////////////
	//
	//	결과물 표시
	//

	pWindow	=	pBGMiddlePanel->Find( WIID_ITEM_UPGRADE_RESULT_BOX_MARK );

	if( pWindow != NULL )
		pWindow->Hide();

	pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( WIID_ITEM_UPGRADE_UIUNIT_POS_RESULT ) );

	if( pUIItemUnit != NULL )
	{
		pUIItemUnit->SetItemID( pItemUpgrade->m_iResult );
		pUIItemUnit->ReResourceLoad();
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
	}

	OnItemCursorEnter( NULL , NULL );

	NextState();
	SetInvenItemLock( m_pUpgradeItem , 0 );
}

void SPWindowItemUpgrade::SetMaterialItem( SPItem* pMaterialItem )
{
	if( m_pUpgradeItem == NULL || pMaterialItem == NULL )
		return;

	if( pMaterialItem->GetContainerType() > INVENTORY_HOUSING )
	{
		SetError( GS_ITEM_UPGRADE_ONLY_ITEM_WINDOW );
		return;
	}

	SPItemStatus*	pUpgradeItemStatus	=	m_pUpgradeItem->GetItemStatus();

	if( pUpgradeItemStatus == NULL || g_pItemCluster == NULL )
		return;

	SPItemUpgrade*	pItemUpgrade		=	g_pItemCluster->GetItemUpgrade( pUpgradeItemStatus->GetItemID() , pUpgradeItemStatus->GetRareLevel() );

	if( pItemUpgrade == NULL )
		return;

	SPItemStatus*	pMaterialItemStatus	=	pMaterialItem->GetItemStatus();

	if( pMaterialItemStatus == NULL )
		return;

	// 아이템 유효성 체크 [10/31/2006 AJJIYA]
	if( pMaterialItemStatus->IsTimeAvailable() == false )
	{
		SPItemAttr*	pMaterialItemAttr	=	pMaterialItem->GetItemAttr();

		if( pMaterialItemAttr != NULL )
			SetError( GS_ITEM_UPGRADE_ITEM_EXPIRE , pMaterialItemAttr->m_strName.c_str() );

		return;
	}

	int	iMaterialCount	=	0;
	int	iPos			=	0;

	if( CheckMaterialCondition( iMaterialCount , iPos , m_pUpgradeItem , pMaterialItem , true ) == false )
		return;

	if( m_pMaterialItem[ iPos ] != NULL )
	{
		SetInvenItemUnlock( m_pMaterialItem[ iPos ] , iPos + 1 );
	}

	SPWindow*		pMaterialPanel		=	Find( WIID_ITEM_UPGRADE_BALLOON_MAIN , true );

	if( pMaterialPanel == NULL )
		return;

	//////////////////////////////////////////////////////////////////////////

	SPWindow*	pWindow	=	NULL;

	pWindow	=	pMaterialPanel->Find( WIID_ITEM_UPGRADE_MATERIAL_BOX_START + iPos );

	if( pWindow != NULL )
		pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );

	SPUIItemUnit*	pUIItemUnit	=	NULL;

	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( iPos );

	if( pUIItemUnit != NULL )
		pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );

	m_pMaterialItem[ iPos ]	=	pMaterialItem;
	m_eCurState				=	ITEM_UPGRADE_STATE_READY_MATERIAL;

	//////////////////////////////////////////////////////////////////////////

	int iMaterialDataCount	=	0;

	for( int i = 0 ; i < WIID_ITEM_UPGRADE_MATERIAL_BOX_COUNT ; ++i )
	{
		if( m_pMaterialItem[ i ] != NULL )
			++iMaterialDataCount;
	}

	bool	bResultBox	=	false;

	if( iMaterialCount == iMaterialDataCount )
	{
		bool	bRequire	=	true;

		for( i = 0 ; i < MAX_ITEM_UPGRADE_REQUIRE ; ++i )
		{
			if( g_pCheckManager->CheckCondition( pItemUpgrade->m_stRequireType[ i ].m_iType , pItemUpgrade->m_stRequireType[ i ].m_iID , pItemUpgrade->m_stRequireType[ i ].m_iValue1 , pItemUpgrade->m_stRequireType[ i ].m_iValue2 ) == false )
			{
				bRequire	=	false;
			}
		}

		bResultBox	=	bRequire;
	}

	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( WIID_ITEM_UPGRADE_UIUNIT_POS_RESULT );

	if( pUIItemUnit != NULL )
	{
		if( bResultBox == true )
			pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
		else
			pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
	}


	pWindow	=	Find( WIID_ITEM_UPGRADE_RUN , true );

	if( pWindow != NULL )
	{
		pWindow->SetEnable( bResultBox );

		pWindow	=	Find( WIID_ITEM_UPGRADE_MATERIAL_TOOLTIP , true );

		if( bResultBox == true )
		{
			m_eCurState = ITEM_UPGRADE_STATE_READY_UPGRADE;

			if( pWindow != NULL )
				pWindow->Hide();
		}
		else
		{
			m_eCurState = ITEM_UPGRADE_STATE_READY_MATERIAL;

			if( pWindow != NULL )
				pWindow->Show();
		}
	}

	//////////////////////////////////////////////////////////////////////////

	SetInvenItemLock( m_pMaterialItem[ iPos ] , iPos + 1 );
}

void SPWindowItemUpgrade::SendServerRun()
{
	if( m_pUpgradeItem == NULL )
		return;

	SPItemStatus* pUpgradeItemStatus	=	m_pUpgradeItem->GetItemStatus();

	if( pUpgradeItemStatus == NULL || g_pItemCluster == NULL )
		return;

	SPItemUpgrade*	pItemUpgrade	=	g_pItemCluster->GetItemUpgrade( pUpgradeItemStatus->GetItemID() , pUpgradeItemStatus->GetRareLevel() );

	if( pItemUpgrade == NULL )
		return;

	if( g_pItemCluster->GetItemUpgradeState() != SPIU_STATE_NULL ) 
		return;

	CPacket kPacket(ITEM_CS_UPGRADE);
	kPacket.AddUINT32( pItemUpgrade->m_iLDTItemID );

	CONTAINER_ITEM	Item;
	m_pUpgradeItem->ConvertContainerItem( &Item );

	kPacket.AddUINT32( m_pUpgradeItem->GetContainerType() );
	kPacket.AddUINT8( m_pUpgradeItem->GetSlotIndex() + 1);
	kPacket.AddUINT64( Item.Item.iItemNo );

	//////////////////////////////////////////////////////////////////////////

	MATERIAL_DATA	stMaterialData[ MAX_ITEM_UPGRADE_MATERIAL ];
	UINT8			uiMaterialCount		=	0;
	SPItemStatus*	pMaterialItemStatus	=	NULL;
	int				iMaterialCount		=	0;
	int				iMaterialPos		=	0;

	ZeroMemory( stMaterialData , sizeof( MATERIAL_DATA ) * MAX_ITEM_UPGRADE_MATERIAL );

	for( int i = 0 ; i < MAX_ITEM_UPGRADE_MATERIAL ; ++i )
	{
		if( m_pMaterialItem[ i ] == NULL )
			continue;

		if( CheckMaterialCondition( iMaterialCount , iMaterialPos , m_pUpgradeItem , m_pMaterialItem[ i ] , false ) == false )
		{
			return;
		}

		m_pMaterialItem[ i ]->ConvertContainerItem( &Item );

		stMaterialData[ i ].iContainerType	=	m_pMaterialItem[ i ]->GetContainerType();
		stMaterialData[ i ].iContainerSlot	=	m_pMaterialItem[ i ]->GetSlotIndex() + 1;
		stMaterialData[ i ].iItemCount		=	pItemUpgrade->m_stMaterial[ i ].m_iCount;
		stMaterialData[ i ].iItemNo			=	Item.Item.iItemNo;

		++uiMaterialCount;

	}

	if( uiMaterialCount == 0 )
		return;

	kPacket.AddUINT8( uiMaterialCount );
	kPacket.AddData( stMaterialData , sizeof( MATERIAL_DATA ) * uiMaterialCount );

	//////////////////////////////////////////////////////////////////////////

	kPacket.AddUINT64( pItemUpgrade->m_iResult );

	//////////////////////////////////////////////////////////////////////////

	for( i = 0 ; i < MAX_ITEM_UPGRADE_REQUIRE ; ++i )
	{
		if( g_pCheckManager->CheckCondition( pItemUpgrade->m_stRequireType[ i ].m_iType , pItemUpgrade->m_stRequireType[ i ].m_iID , pItemUpgrade->m_stRequireType[ i ].m_iValue1 , pItemUpgrade->m_stRequireType[ i ].m_iValue2 ) == false )
			return;
	}

	//////////////////////////////////////////////////////////////////////////
	
	g_pNetworkManager->GetPacketHandler()->PostMsg(&kPacket);
	g_pItemCluster->SetItemUpgradeState( SPIU_STATE_ITEM_UPGRADE );
}

void SPWindowItemUpgrade::ResetMaterialData()
{
	for( int i = 0 ; i < WIID_ITEM_UPGRADE_MATERIAL_BOX_COUNT ; ++i )
	{
		ResetMaterialData( i );
	}
}

void SPWindowItemUpgrade::ResetMaterialData( int iMaterialPos )
{
	if( iMaterialPos < 0 || iMaterialPos > WIID_ITEM_UPGRADE_MATERIAL_BOX_COUNT )
		return;

	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_ITEM_UPGRADE_RUN , true );

	if( pWindow != NULL )
	{
		pWindow->SetEnable( false );
	}

	pWindow	=	Find( WIID_ITEM_UPGRADE_MATERIAL_BOX_START + iMaterialPos , true );

	if( pWindow != NULL )
	{
		pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );
		pWindow->Show();
	}

	SPUIItemUnit*	pUIItemUnit	=	NULL;

	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( iMaterialPos );

	if( pUIItemUnit != NULL )
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );

	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( WIID_ITEM_UPGRADE_UIUNIT_POS_RESULT );

	if( pUIItemUnit != NULL )
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );

	if( m_pMaterialItem[ iMaterialPos ] == NULL )
		return;

	pWindow	=	Find( WIID_ITEM_UPGRADE_MATERIAL_TOOLTIP , true );

	if( pWindow != NULL )
	{
		pWindow->Show();
	}

	m_eCurState	=	ITEM_UPGRADE_STATE_READY_MATERIAL;

	SetInvenItemUnlock( m_pMaterialItem[ iMaterialPos ] , iMaterialPos + 1 );

	m_pMaterialItem[ iMaterialPos ]	=	NULL;
}

bool SPWindowItemUpgrade::CheckMaterialCondition()
{
	int		iMaterialPos		=	0;
	int		iMaterialCount		=	0;
	int		iCurMaterialCount	=	0;

	for( int i = 0 ; i < WIID_ITEM_UPGRADE_MATERIAL_BOX_COUNT ; ++i )
	{
		if( m_pMaterialItem[ i ] != NULL )
			++iCurMaterialCount;
		else
			continue;

		if( CheckMaterialCondition( iMaterialCount , iMaterialPos , m_pUpgradeItem , m_pMaterialItem[ i ] , false ) == false )
		{
			return false;
		}
	}

	if( iCurMaterialCount != 0 && iCurMaterialCount == iMaterialCount )
		return true;

	return false;
}

bool SPWindowItemUpgrade::CheckMaterialCondition( int& iOutMaterialCount , int& iOutMaterialPos , SPItem* pUpgradeItem , SPItem* pMaterialItem , bool bMultiCheck )
{
	iOutMaterialCount	=	0;
	iOutMaterialPos		=	0;

	if( pUpgradeItem == NULL || pMaterialItem == NULL || g_pItemCluster == NULL )
		return false;

	SPItemStatus*	pUpgradeItemStatus	=	pUpgradeItem->GetItemStatus();
	SPItemStatus*	pMaterialItemStatus	=	pMaterialItem->GetItemStatus();

	if( pUpgradeItemStatus == NULL || pMaterialItemStatus == NULL )
		return false;

	SPItemUpgrade*	pItemUpgrade		=	g_pItemCluster->GetItemUpgrade( pUpgradeItemStatus->GetItemID() , pUpgradeItemStatus->GetRareLevel() );

	if( pItemUpgrade == NULL )
		return false;

	if( pMaterialItemStatus->GetItemID()		<= 0 ||
		pMaterialItemStatus->GetRareLevel()		<= 0 || 
		pMaterialItemStatus->GetStackCount()	<=	0 )
	{
		return false;
	}

	bool	bFind					=	false;
	bool	bErrorMaterialDifferent	=	false;
	bool	bErrorMaterialCount		=	false;
	bool	bErrorMaterialRare		=	false;
	bool	bErrorIsMaterial		=	false;

	for( int i = 0 ; i < MAX_ITEM_UPGRADE_MATERIAL ; ++i )
	{
		if( pItemUpgrade->m_stMaterial[ i ].m_iID			<= 0 ||
			pItemUpgrade->m_stMaterial[ i ].m_iCount		<= 0 )
		{
			continue;
		}

		++iOutMaterialCount;

		if( pMaterialItemStatus->GetItemID() != pItemUpgrade->m_stMaterial[ i ].m_iID )
		{
			bErrorMaterialDifferent	=	true;
			continue;
		}

		if( pMaterialItemStatus->GetStackCount() < pItemUpgrade->m_stMaterial[ i ].m_iCount )
		{
			bErrorMaterialCount	=	true;
			continue;
		}

		if( pMaterialItemStatus->GetRareLevel() != (int)(pItemUpgrade->m_stMaterial[ i ].m_eRare) && (int)(pItemUpgrade->m_stMaterial[ i ].m_eRare) != 0 )
		{
			bErrorMaterialRare	=	true;
			continue;
		}

		if( m_pMaterialItem[ i ] != NULL && bMultiCheck == true )
		{
			bErrorIsMaterial	=	true;
			continue;
		}

		bFind			=	true;
		iOutMaterialPos	=	i;
	}

	if( bFind == false )
	{
		GLOBAL_STRING_ITEM_UPGRADE	eErrorID	=	GS_ITEM_UPGRADE_NULL;

		if( bErrorMaterialRare == true )
			eErrorID	=	GS_ITEM_UPGRADE_MATERIAL_RARE;

		if( bErrorMaterialCount == true )
			eErrorID	=	GS_ITEM_UPGRADE_MATERIAL_COUNT;

		if( bErrorMaterialDifferent == true )
			eErrorID	=	GS_ITEM_UPGRADE_MATERIAL_DIFFERENT;

		if( bErrorIsMaterial == true )
			eErrorID	=	GS_ITEM_UPGRADE_IS_MATERIAL;

		SetError( eErrorID );
	}

	return bFind;
}

//////////////////////////////////////////////////////////////////////////

void SPWindowItemUpgrade::SetError( int iGloalStringID , const char* pszItemString /* = NULL  */ )
{
	DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

	const char*	pszGlobalString	=	g_pResourceManager->GetGlobalString( iGloalStringID );

	if( pszGlobalString == NULL )
		return;

	if( pszItemString != NULL )
	{
		sprintf( szErrorMsg , pszGlobalString , pszItemString );
	}
	else
	{
		strcpy( szErrorMsg , pszGlobalString );
	}

	SetError( szErrorMsg );
}

void SPWindowItemUpgrade::SetError( const char* pszErrorString )
{
	if( g_pInterfaceManager == NULL || pszErrorString == NULL )
		return;

	D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pszErrorString , (LPARAM)&color );
}

SPItem* SPWindowItemUpgrade::GetInvenPickUpItem()
{
	SPPlayer* pkPlayer = g_pGOBManager->GetLocalPlayer();
	if( pkPlayer == NULL )
		return NULL;

	SPPlayerInvenArchive* pkInvenArchive = pkPlayer->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return NULL;

	SPSelectItem SelItemInfo = pkInvenArchive->GetPickupItem();
	return pkInvenArchive->GetInventoryItem(SelItemInfo.m_iContainer, SelItemInfo.m_iSlotIndex);
}

void SPWindowItemUpgrade::SetInvenItemLock( SPItem* pkItem , int iIndex )
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	if( pkItem == NULL )
		return;
	
	SPPlayerInvenArchive* pkInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInven ) {
		//pkInven->SetUserShopItemFake(iIndex, pkItem->GetContainerType(), pkItem->GetSlotIndex());
		pkInven->SetBlockItem(iIndex, pkItem->GetContainerType(), pkItem->GetSlotIndex());
	}
}

void SPWindowItemUpgrade::SetInvenItemUnlock( SPItem* pkItem , int iIndex )
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	if( pkItem == NULL )
		return;

	SPPlayerInvenArchive* pkInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInven ) {
		//pkInven->ClearUserShopItem(iIndex);
		pkInven->ClearBlockItem(iIndex);
	}
}

void SPWindowItemUpgrade::SetInvenItemAllUnlock()
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	SPPlayerInvenArchive* pkInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInven ) {
		//pkInven->ClearAllUserShopItem();
		pkInven->ClearAllBlockItem();
	}
}

void SPWindowItemUpgrade::SendParentAction( UINT64 uiAction , unsigned int uiIndex /* = 0  */, bool bSendInit /* = false  */ )
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	if( bSendInit )
		g_pGOBManager->GetLocalPlayer()->InitSendAction();

	if( uiIndex )
		g_pGOBManager->GetLocalPlayer()->SetSendAttackIndex(uiIndex);

	g_pGOBManager->GetLocalPlayer()->SetControlAction(uiAction);
	g_pGOBManager->GetLocalPlayer()->SendActionPacket();
}
