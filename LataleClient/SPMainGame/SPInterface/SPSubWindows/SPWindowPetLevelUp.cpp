// ***************************************************************
//  SPWindowPetLevelUp   version:  1.0   ·  date: 11/22/2006
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************/

#include "SPCommon.h"
#include "SPUtil.h"

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <map>

#include "SPGOBManager.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "SPCommandConvert.h"

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
#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPMotionStatus.h"
#include "SPSkillManager.h"
#include "SPDebug.h"

#include "SPWindowPetLevelUp.h"

SPWindowPetLevelUp::SPWindowPetLevelUp(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowPetLevelUp::SPWindowPetLevelUp(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_PET_LEVELUP, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowPetLevelUp::~SPWindowPetLevelUp()
{
	Clean();
}

void SPWindowPetLevelUp::Init()
{
	SPWindowStatic* pWindowStatic = NULL;

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_BG_TOP , 0 , 0 , 240 , 4 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 1 , 31 );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_BG_BOTTOM , 0 , 121 , 240 , 4 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 1 , 41 );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_BG_MIDDLE , 0 , 4 , 240 , 117 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 1 , 37 );
	pWindowStatic->SetSrcSize( 240 , 2 );

	InitContentsWindow( pWindowStatic , 0 , -4 );
	InitUIItemUnit();
	UpdatePosUIItemUnit();

	m_eCurState							=	PET_LEVELUP_STATE_NULL;

	m_fItemToolTipAccmulateTime			=	0.0f;
	m_fItemToolTipMaxTime				=	0.05f;

	m_fMaterialToolTipAccmulateTime		=	0.0f;
	m_fMaterialToolTipMaxTime			=	0.05f;

	m_pUpgradeItem						=	NULL;
	m_pMaterialItem						=	NULL;
}

void SPWindowPetLevelUp::Clean()
{
	SPWindow::Clean();

	m_UIUnitManager.ClearAll( TRUE );
}

void SPWindowPetLevelUp::Process(float fTime)
{
	if(!m_bShow) return;

	SPWindow::Process( fTime );

	m_UIUnitManager.Process( fTime );

	if( m_eCurState == PET_LEVELUP_STATE_READY_ITEM )
	{
		ProcessItemToolTip( fTime );
	}
	else if( m_eCurState == PET_LEVELUP_STATE_READY_MATERIAL )
	{
		ProcessMaterialToolTip( fTime );
	}
}

void SPWindowPetLevelUp::Render(float fTime)
{
	if(!m_bShow) return;

	SPWindow::Render( fTime );

	g_pVideo->Flush();

	m_UIUnitManager.Render();

	g_pVideo->Flush();
}

void SPWindowPetLevelUp::Show(bool bWithChild)
{
	SPWindow::Show( bWithChild );

	DefaultHideWindow();
}

void SPWindowPetLevelUp::Hide(bool bSendServer /* = true */)
{
	SetUpgradeItem( NULL );

	SPWindow::Hide( bSendServer );

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
		OnMaterialCursorOut( NULL ,NULL );
}

void SPWindowPetLevelUp::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();

	UpdatePosUIItemUnit();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowPetLevelUp )

	SPIMESSAGE_COMMAND(	SPIM_ITEM_UPGRADE_RESULT	,											OnResult				)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_PET_LEVELUP_CLOSE			,		OnClose					)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_PET_LEVELUP_RUN			,		OnRun					)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_PET_LEVELUP_ITEM_BOX		,		OnItemBox				)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_PET_LEVELUP_ITEM_BOX		,		OnItemBoxCancel			)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_PET_LEVELUP_MATERIAL_BOX	,		OnMaterial				)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_PET_LEVELUP_MATERIAL_BOX	,		OnMaterialCancel		)

	SPIMESSAGE_CONTROL( SPIM_CURSOR_ENTER			,	WIID_PET_LEVELUP_ITEM_BOX		,		OnItemCursorEnter		)
	SPIMESSAGE_CONTROL( SPIM_CURSOR_OUT				,	WIID_PET_LEVELUP_ITEM_BOX		,		OnItemCursorOut			)
	SPIMESSAGE_CONTROL( SPIM_CURSOR_ENTER			,	WIID_PET_LEVELUP_RESULT_BOX		,		OnResultCursorEnter		)
	SPIMESSAGE_CONTROL( SPIM_CURSOR_OUT				,	WIID_PET_LEVELUP_RESULT_BOX		,		OnResultCursorOut		)
	SPIMESSAGE_CONTROL( SPIM_CURSOR_ENTER			,	WIID_PET_LEVELUP_MATERIAL_BOX	,		OnMaterialCursorEnter	)
	SPIMESSAGE_CONTROL( SPIM_CURSOR_OUT				,	WIID_PET_LEVELUP_MATERIAL_BOX	,		OnMaterialCursorOut		)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowPetLevelUp::OnResult( WPARAM wParam , LPARAM lParam )
{
	UINT8				uiError			=	(UINT8)lParam;
	GLOBAL_STRING_ID	eGlobalStringID	=	(GLOBAL_STRING_ID)wParam;

	if( eGlobalStringID > 0 )
		SetError( eGlobalStringID );

	m_eCurState	=	PET_LEVELUP_STATE_READY_ITEM;

	DefaultHideWindow();
	SetUpgradeItem( NULL );

	g_pItemCluster->SetItemUpgradeState( SPIU_STATE_NULL );

	return 1;
}

int SPWindowPetLevelUp::OnClose( WPARAM wParam , LPARAM lParam )
{
	Hide();

	return 1;
}

int SPWindowPetLevelUp::OnRun( WPARAM wParam , LPARAM lParam )
{
	if( m_eCurState != PET_LEVELUP_STATE_READY_UPGRADE )
		return 1;

	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return 1;

	if( g_pGOBManager->GetLocalPlayer()->GetCurAction() != ACTION_STOP )
	{
		SetError( GS_PET_LEVELUP_ACTION_STOP );
		return 1;
	}

	m_eCurState	=	PET_LEVELUP_STATE_BRIGHTTING;

	NextState();

	SPWindow*	pWindow	=	Find( WIID_PET_LEVELUP_RUN , true );

	if( pWindow != NULL )
		pWindow->SetEnable( false );

	return 1;
}

int SPWindowPetLevelUp::OnItemBox( WPARAM wParam , LPARAM lParam )
{
	if( m_eCurState == PET_LEVELUP_STATE_READY_ITEM		||
		m_eCurState == PET_LEVELUP_STATE_READY_MATERIAL	||
		m_eCurState == PET_LEVELUP_STATE_READY_UPGRADE		)
	{
		SPItem* pPickUpItem = GetInvenPickUpItem();

		if( pPickUpItem == NULL )
			return 1;

		SetUpgradeItem( pPickUpItem );
	}

	return 1;
}

int SPWindowPetLevelUp::OnItemBoxCancel( WPARAM wParam , LPARAM lParam )
{
	if( m_eCurState == PET_LEVELUP_STATE_READY_ITEM		||
		m_eCurState == PET_LEVELUP_STATE_READY_MATERIAL	||
		m_eCurState == PET_LEVELUP_STATE_READY_UPGRADE		)
	{
		DefaultHideWindow();
		SetUpgradeItem( NULL );
	}

	return 1;
}

int SPWindowPetLevelUp::OnMaterial( WPARAM wParam , LPARAM lParam )
{
	if( m_eCurState == PET_LEVELUP_STATE_READY_ITEM		||
		m_eCurState == PET_LEVELUP_STATE_READY_MATERIAL	||
		m_eCurState == PET_LEVELUP_STATE_READY_UPGRADE		)
	{
		SPItem* pPickUpItem = GetInvenPickUpItem();

		if( pPickUpItem == NULL )
			return 1;

		SetMaterialItem( pPickUpItem );
	}

	return 1;
}

int SPWindowPetLevelUp::OnMaterialCancel( WPARAM wParam , LPARAM lParam )
{
	if( m_eCurState == PET_LEVELUP_STATE_READY_ITEM		||
		m_eCurState == PET_LEVELUP_STATE_READY_MATERIAL	||
		m_eCurState == PET_LEVELUP_STATE_READY_UPGRADE		)
	{
		ResetMaterialData();
	}

	return 1;
}

int SPWindowPetLevelUp::OnItemCursorEnter( WPARAM wParam , LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	SPUIUnit*	pUIUnit	=	m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_ITEM );

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

int SPWindowPetLevelUp::OnItemCursorOut( WPARAM wParam , LPARAM lParam )
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );
	return 1;
}

int SPWindowPetLevelUp::OnResultCursorEnter( WPARAM wParam , LPARAM lParam )
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

int SPWindowPetLevelUp::OnResultCursorOut( WPARAM wParam , LPARAM lParam )
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );
	return 1;
}

int SPWindowPetLevelUp::OnMaterialCursorEnter( WPARAM wParam , LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	SPUIUnit*	pUIUnit	=	m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_MATERIAL );

	if( pUIUnit == NULL )
		return 1;

	if( pUIUnit->GetStatus( SPUIUnit::STATUS_UI_UNIT_RENDER ) == false )
		return 1;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pUIUnit, SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );

	return 1;
}

int SPWindowPetLevelUp::OnMaterialCursorOut( WPARAM wParam , LPARAM lParam )
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );
	return 1;
}

//////////////////////////////////////////////////////////////////////////

void SPWindowPetLevelUp::InitContentsWindow( SPWindow* pParent , int iAdjustX , int iAdjustY )
{
	SPWindowStatic* pWindowStatic = NULL;
	SPWindowButton* pWindowButton = NULL;

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_TITLE , 10 + iAdjustX , 6 + iAdjustY , 200 , 15 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI104.PNG", 1 , 392 );

	pWindowButton = new SPWindowButton( WIID_PET_LEVELUP_CLOSE , 219 + iAdjustX , 9 + iAdjustY , 12 , 12 , pParent );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 301 , 321 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 314 , 321 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 327 , 321 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 340 , 321 );

	pWindowButton = new SPWindowButton( WIID_PET_LEVELUP_RUN , 162 + iAdjustX , 102 + iAdjustY , 68 , 17 , pParent );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 346 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 346 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 346 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 346 , 494 );
	pWindowButton->SetEnable( false );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_LINE1 , 2 + iAdjustX , 26 + iAdjustY , 236 , 1 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 493 , 494 );
	pWindowStatic->SetSrcSize( 4 , 1 );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_LINE2 , 2 + iAdjustX , 97 + iAdjustY , 236 , 1 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 493 , 494 );
	pWindowStatic->SetSrcSize( 4 , 1 );

	SPWindowStatic* pBGMiddlePanel = new SPWindowStatic( WIID_PET_LEVELUP_PANEL_CONTENTS , 2 + iAdjustX , 27 + iAdjustY , 236 , 70 , pParent );
	pBGMiddlePanel->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 493 , 477 );
	pBGMiddlePanel->SetSrcSize( 2 , 2 );

	InitWhiteBoardWindow( pBGMiddlePanel , -2 , -27 );
}

void SPWindowPetLevelUp::InitWhiteBoardWindow( SPWindow* pParent , int iAdjustX , int iAdjustY )
{
	SPWindowStatic* pWindowStatic = NULL;

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_ITEM_BOX , 36 + iAdjustX , 37 + iAdjustY , 32 , 32 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 203 , 408 );
	pWindowStatic->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_ITEM_PIC , 36 + iAdjustX , 37 + iAdjustY , 32 , 32 , pParent );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_MATERIAL_BOX , 104 + iAdjustX , 37 + iAdjustY , 32 , 32 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 203 , 408 );
	pWindowStatic->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_MATERIAL_PIC , 104 + iAdjustX , 37 + iAdjustY , 32 , 32 , pParent );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_RESULT_BOX , 172 + iAdjustX , 37 + iAdjustY , 32 , 32 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 203 , 408 );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_RESULT_BOX_MARK , 182 + iAdjustX , 44 + iAdjustY , 12 , 18 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 360 , 321 );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_RESULT_PIC , 172 + iAdjustX , 37 + iAdjustY , 32 , 32 , pParent );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_PLUS , 78 + iAdjustX , 44 + iAdjustY , 16 , 16 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 239 , 423 );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_EQUAL , 146 + iAdjustX , 45 + iAdjustY , 16 , 14 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 239 , 408 );

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_ITEM_TOOLTIP , 16 + iAdjustX , 79 + iAdjustY , 208 , 15 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI104.PNG", 1 , 109 );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_MATERIAL_TOOLTIP , 16 + iAdjustX , 79 + iAdjustY , 208 , 15 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI104.PNG", 1 , 125 );

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_RUN_TOOLTIP , 16 + iAdjustX , 79 + iAdjustY , 208 , 15 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI104.PNG", 1 , 141 );

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_ITEM_TOOLTIP_CURSOR , 47 + iAdjustX , 67 + iAdjustY , 10 , 10 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 354 , 261 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 365 , 261 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 376 , 261 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 387 , 261 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 398 , 261 );

	m_iCurItemToolTipImgIndex		=	0;
	m_iMaxItemToolTipImgIndex		=	5;
	m_iStepItemToolTipImgIndex		=	1;

	pWindowStatic = new SPWindowStatic( WIID_PET_LEVELUP_MATERIAL_TOOLTIP_CURSOR , 115 + iAdjustX , 67 + iAdjustY , 10 , 10 , pParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 354 , 261 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 365 , 261 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 376 , 261 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 387 , 261 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG", 398 , 261 );

	m_iCurMaterialToolTipImgIndex	=	0;
	m_iMaxMaterialToolTipImgIndex	=	5;
	m_iStepMaterialToolTipImgIndex	=	1;
}

void SPWindowPetLevelUp::InitUIItemUnit()
{
	int	iUIItemUnitCount = WIID_PET_LEVELUP_UIUNIT_COUNT;

	m_UIUnitManager.SetBufferSize( iUIItemUnitCount );

	SPUIItemUnit*	pUIItemUnit		=	NULL;
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

		if( i == WIID_PET_LEVELUP_UIUNIT_POS_RESULT )
            pUIItemUnit->SetInstanceID( WIID_ITEM_UPGRADE );
		else
			pUIItemUnit->SetInstanceID( WIID_ITEM );

		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );

		m_UIUnitManager.AddUnit( pUIItemUnit );
	}
}

void SPWindowPetLevelUp::UpdatePosUIItemUnit()
{
	SPUIItemUnit*	pUIItemUnit	=	NULL;
	SPWindow*		pWindow		=	NULL;
	RECT			rcPos;

	pWindow	=	Find( WIID_PET_LEVELUP_ITEM_PIC , true );
	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_ITEM );

	if( pWindow != NULL && pUIItemUnit != NULL )
	{
		pWindow->GetWindowRect( rcPos );
		pUIItemUnit->SetPos( rcPos.left , rcPos.top );
		pUIItemUnit->ProcessCount();
	}

	pWindow		=	Find( WIID_PET_LEVELUP_MATERIAL_PIC , true );
	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_MATERIAL );

	if( pUIItemUnit != NULL && pWindow != NULL )
	{
		pWindow->GetWindowRect( rcPos );

		pUIItemUnit->SetPos( rcPos.left , rcPos.top );
		pUIItemUnit->ProcessCount();
	}

	pWindow	=	Find( WIID_PET_LEVELUP_RESULT_PIC , true );
	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_RESULT );

	if( pWindow != NULL && pUIItemUnit != NULL )
	{
		pWindow->GetWindowRect( rcPos );
		pUIItemUnit->SetPos( rcPos.left , rcPos.top );
		pUIItemUnit->ProcessCount();
	}
}

void SPWindowPetLevelUp::ProcessItemToolTip( float fTime )
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

		SPWindow*	pWindow	=	Find( WIID_PET_LEVELUP_ITEM_TOOLTIP_CURSOR , true );

		if( pWindow != NULL )
		{
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , m_iCurItemToolTipImgIndex );
		}
	}
}

void SPWindowPetLevelUp::ProcessMaterialToolTip( float fTime )
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

		SPWindow*	pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP_CURSOR , true );

		if( pWindow != NULL )
		{
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , m_iCurMaterialToolTipImgIndex );
		}
	}
}

void SPWindowPetLevelUp::DefaultHideWindow()
{
	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_PET_LEVELUP_RUN , true );

	if( pWindow != NULL )
	{
		pWindow->SetEnable( false );
	}

	pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP_CURSOR , true );

	if( pWindow != NULL )
	{
		pWindow->Hide();
	}

	pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP , true );

	if( pWindow != NULL )
	{
		pWindow->Hide();
	}

	pWindow	=	Find( WIID_PET_LEVELUP_RUN_TOOLTIP , true );

	if( pWindow != NULL )
	{
		pWindow->Hide();
	}

}

void SPWindowPetLevelUp::NextState()
{
	m_eCurState	=	(PET_LEVELUP_STATE)( (int)m_eCurState + 1 );

	if( (int)m_eCurState >= PET_LEVELUP_STATE_END )
		m_eCurState	=	PET_LEVELUP_STATE_NULL;

	SPWindow*	pWindow	=	NULL;

	if( m_eCurState == PET_LEVELUP_STATE_READY_MATERIAL )
	{
		pWindow	=	Find( WIID_PET_LEVELUP_ITEM_TOOLTIP , true );

		if( pWindow != NULL )
			pWindow->Hide();

		pWindow	=	Find( WIID_PET_LEVELUP_ITEM_TOOLTIP_CURSOR , true );

		if( pWindow != NULL )
			pWindow->Hide();

		pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP , true );

		if( pWindow != NULL )
			pWindow->Show();

		pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP_CURSOR , true );

		if( pWindow != NULL )
			pWindow->Show();

		pWindow	=	Find( WIID_PET_LEVELUP_RUN_TOOLTIP , true );

		if( pWindow != NULL )
			pWindow->Hide();
	}

	if( m_eCurState == PET_LEVELUP_STATE_READY_UPGRADE )
	{
		pWindow	=	Find( WIID_PET_LEVELUP_RUN , true );

		if( pWindow != NULL )
			pWindow->SetEnable( true );

		pWindow	=	Find( WIID_PET_LEVELUP_ITEM_TOOLTIP , true );

		if( pWindow != NULL )
			pWindow->Hide();

		pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP , true );

		if( pWindow != NULL )
			pWindow->Hide();

		pWindow	=	Find( WIID_PET_LEVELUP_RUN_TOOLTIP , true );

		if( pWindow != NULL )
			pWindow->Show();
	}

	if( m_eCurState == PET_LEVELUP_STATE_UPGRADING_WAIT )
		SendServerRun();
}

void SPWindowPetLevelUp::SetUpgradeItem( SPItem* pUpgradeItem )
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
			if (pUpgradeItemAttr->m_iNewPet == 0)
			{
				pItemUpgrade	=	g_pItemCluster->GetItemUpgrade( pUpgradeItemStatus->GetItemID() , pUpgradeItemStatus->GetRareLevel() );

				if( pItemUpgrade == NULL )
				{
					SetError( GS_PET_LEVELUP_NOT_UPGRADE );
					return;
				}

				// 재료 1개가 있나 없나 데이터 체크 [11/24/2006 AJJIYA]
				if( pItemUpgrade->m_stMaterial[0].m_eRare == 0 || pItemUpgrade->m_stMaterial[0].m_iCount <= 0 || pItemUpgrade->m_stMaterial[0].m_iID <= 0 )
				{
					SetError( GS_PET_LEVELUP_NOT_LEVELUP );
					return;
				}

				// 아이템 유효성 체크 [10/31/2006 AJJIYA]
				if( pUpgradeItemStatus->IsTimeAvailable() == false )
				{
					SetError( GS_PET_LEVELUP_ITEM_EXPIRE , pUpgradeItemAttr->m_strName.c_str() );
					return;
				}

				// 경험치 가능 유무 체크 [11/24/2006 AJJIYA]
				if( pUpgradeItemStatus->GetExp() < pUpgradeItemAttr->m_iPetMaxExp )
				{
					SetError( GS_PET_NEED_EXP );
					return;
				}
			}
		}
	}

	if( m_pUpgradeItem != NULL )
	{
		SetInvenItemAllUnlock();
		ResetMaterialData();
	}

	m_pUpgradeItem	=	pUpgradeItem;
	m_eCurState		=	PET_LEVELUP_STATE_READY_ITEM;

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

	pWindow	=	Find( WIID_PET_LEVELUP_ITEM_BOX , true );

	if( pWindow != NULL )
		pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );

	//////////////////////////////////////////////////////////////////////////
	
	pWindow	=	Find( WIID_PET_LEVELUP_ITEM_TOOLTIP , true );

	if( pWindow != NULL )
		pWindow->Show();

	pWindow	=	Find( WIID_PET_LEVELUP_ITEM_TOOLTIP_CURSOR , true );

	if( pWindow != NULL )
		pWindow->Show();

	pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP , true );

	if( pWindow != NULL )
		pWindow->Hide();

	pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP_CURSOR , true );

	if( pWindow != NULL )
		pWindow->Hide();

	pWindow	=	Find( WIID_PET_LEVELUP_RUN_TOOLTIP , true );

	if( pWindow != NULL )
		pWindow->Hide();

	//////////////////////////////////////////////////////////////////////////

	SPWindow* pBGMiddlePanel = Find( WIID_PET_LEVELUP_PANEL_CONTENTS , true );

	if( pBGMiddlePanel == NULL )
		return;

	//////////////////////////////////////////////////////////////////////////
	
	pWindow	=	pBGMiddlePanel->Find( WIID_PET_LEVELUP_ITEM_BOX );

	if( pWindow != NULL )
	{
		pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );
		pWindow->Show();
	}

	pWindow	=	pBGMiddlePanel->Find( WIID_PET_LEVELUP_MATERIAL_BOX );

	if( pWindow != NULL )
	{
		pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );
		pWindow->Show();
	}

	//////////////////////////////////////////////////////////////////////////

	pWindow	=	pBGMiddlePanel->Find( WIID_PET_LEVELUP_RESULT_BOX_MARK );

	if( pWindow != NULL )
		pWindow->Show();

	//////////////////////////////////////////////////////////////////////////
	
	if( g_pItemCluster == NULL )
		return;

	if( pUpgradeItem == NULL )
		return;

	if( pUpgradeItemAttr == NULL || pUpgradeItemStatus == NULL )
		return;

	if( pUpgradeItemAttr->m_iNewPet == 0 && pItemUpgrade == NULL )
	{
		SetError( GS_PET_LEVELUP_NOT_UPGRADE );
		return;
	}

	pWindow	=	pBGMiddlePanel->Find( WIID_PET_LEVELUP_ITEM_BOX );

	if( pWindow != NULL )
	{
		pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );
		pWindow->Show();
	}

	pWindow	=	pBGMiddlePanel->Find( WIID_PET_LEVELUP_MATERIAL_BOX );

	if( pWindow != NULL )
	{
		pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );
		pWindow->Show();
	}

	//////////////////////////////////////////////////////////////////////////
	
	if( pUpgradeItem->GetContainerType() != INVENTORY_PET && pUpgradeItem->GetContainerType() != FIGURE_PET)
	{
		SetError( GS_PET_LEVELUP_ONLY_ITEM_WINDOW );
		return;
	}

	SPItemAttr* pResultItemAttr		=	NULL;

	if( pUpgradeItemAttr->m_iNewPet == 0
		&&
		g_pItemCluster->GetItemInfo( pItemUpgrade->m_iResult , pResultItemAttr ) == true )
	{
		if( pResultItemAttr != NULL )
		{
			if( pResultItemAttr->m_eType != pUpgradeItemAttr->m_eType )
			{
				SetError( GS_PET_LEVELUP_ITEM_TYPE_DIFFERENT );
				return;
			}

			if( pResultItemAttr->GetDurationType() != pUpgradeItemAttr->GetDurationType() )
			{
				SetError( GS_PET_LEVELUP_ITEM_TYPE_DIFFERENT );
				return;
			}
		}
	}
    
	//////////////////////////////////////////////////////////////////////////
	
	pWindow	=	Find( WIID_PET_LEVELUP_ITEM_BOX , true );

	if( pWindow == NULL )
		return;

	pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );

	SPUIItemUnit*	pUIItemUnit	=	NULL;

	pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_ITEM ) );

	if( pUIItemUnit != NULL )
	{
		pUIItemUnit->SetItemID( pUpgradeItemStatus->GetItemID() );
		pUIItemUnit->ReResourceLoad();
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	재료 표시
	//

	pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_MATERIAL ) );

	if( pUIItemUnit != NULL && pItemUpgrade != NULL )
	{
		if( pItemUpgrade->m_stMaterial[ 0 ].m_iID != 0 && pItemUpgrade->m_stMaterial[ 0 ].m_iCount != 0 && pItemUpgrade->m_stMaterial[ 0 ].m_eRare != 0 )
		{
			if( pItemUpgrade->m_stMaterial[ 0 ].m_iCount > 1)
				pUIItemUnit->SetCount( pItemUpgrade->m_stMaterial[ 0 ].m_iCount );
			else
				pUIItemUnit->SetCount( 0 );

			pUIItemUnit->SetItemID( pItemUpgrade->m_stMaterial[ 0 ].m_iID );
			pUIItemUnit->ReResourceLoad();
			pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	결과물 표시
	//

	pWindow	=	pBGMiddlePanel->Find( WIID_PET_LEVELUP_RESULT_BOX_MARK );

	if( pWindow != NULL )
		pWindow->Hide();

	pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_RESULT ) );

	if( pUIItemUnit != NULL && pItemUpgrade != NULL)
	{
		pUIItemUnit->SetItemID( pItemUpgrade->m_iResult );
		pUIItemUnit->ReResourceLoad();
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
	}

	OnItemCursorEnter( NULL , NULL );

	NextState();
	SetInvenItemLock( m_pUpgradeItem , WIID_PET_LEVELUP_UIUNIT_POS_ITEM );
}

void SPWindowPetLevelUp::SetMaterialItem( SPItem* pMaterialItem )
{
	if( m_pUpgradeItem == NULL || pMaterialItem == NULL )
		return;

	if (!m_pUpgradeItem->GetItemAttr())
		return;
	bool bNewPet = (m_pUpgradeItem->GetItemAttr()->m_iNewPet == 0 ? false : true);

	if( pMaterialItem->GetContainerType() > INVENTORY_HOUSING )
	{
		SetError( GS_PET_LEVELUP_ONLY_ITEM_WINDOW );
		return;
	}

	SPItemStatus*	pUpgradeItemStatus	=	m_pUpgradeItem->GetItemStatus();

	if( pUpgradeItemStatus == NULL || g_pItemCluster == NULL )
		return;

	SPItemUpgrade* pItemUpgrade = NULL;
	if (!bNewPet)
	{
		pItemUpgrade		=	g_pItemCluster->GetItemUpgrade( pUpgradeItemStatus->GetItemID() , pUpgradeItemStatus->GetRareLevel() ); 
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
				SetError( GS_PET_LEVELUP_ITEM_EXPIRE , pMaterialItemAttr->m_strName.c_str() );

			return;
		}

		int	iMaterialCount	=	0;
		int	iPos			=	0;

		if( CheckMaterialCondition( iMaterialCount , iPos , m_pUpgradeItem , pMaterialItem , true ) == false )
			return;

		if( m_pMaterialItem != NULL )
		{
			SetInvenItemUnlock( m_pMaterialItem , WIID_PET_LEVELUP_UIUNIT_POS_MATERIAL );
		}

		SPWindow*		pMaterialPanel		=	Find( WIID_PET_LEVELUP_PANEL_CONTENTS , true );

		if( pMaterialPanel == NULL )
			return;

		//////////////////////////////////////////////////////////////////////////

		SPWindow*	pWindow	=	NULL;

		pWindow	=	pMaterialPanel->Find( WIID_PET_LEVELUP_MATERIAL_BOX );

		if( pWindow != NULL )
			pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );

		SPUIItemUnit*	pUIItemUnit	=	NULL;

		pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_MATERIAL );

		if( pUIItemUnit != NULL )
			pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );

		m_pMaterialItem	=	pMaterialItem;
		m_eCurState		=	PET_LEVELUP_STATE_READY_MATERIAL;

		//////////////////////////////////////////////////////////////////////////

		int i = 0;
		int iMaterialDataCount	=	1;

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

		pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_RESULT );

		if( pUIItemUnit != NULL )
		{
			if( bResultBox == true )
				pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
			else
				pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
		}


		pWindow	=	Find( WIID_PET_LEVELUP_RUN , true );

		if( pWindow != NULL )
		{
			pWindow->SetEnable( bResultBox );

			pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP , true );

			if( bResultBox == true )
			{
				m_eCurState = PET_LEVELUP_STATE_READY_UPGRADE;

				if( pWindow != NULL )
					pWindow->Hide();

				pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP_CURSOR , true );

				if( pWindow != NULL )
					pWindow->Hide();

				pWindow	=	Find( WIID_PET_LEVELUP_RUN_TOOLTIP , true );

				if( pWindow != NULL )
					pWindow->Show();
			}
			else
			{
				m_eCurState = PET_LEVELUP_STATE_READY_MATERIAL;

				if( pWindow != NULL )
					pWindow->Show();

				pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP_CURSOR , true );

				if( pWindow != NULL )
					pWindow->Show();

				pWindow	=	Find( WIID_PET_LEVELUP_RUN_TOOLTIP , true );

				if( pWindow != NULL )
					pWindow->Hide();
			}
		}
	}
	else
	{
		// check is this a pet upgrade book
		SPSkill *theSkill = SPSkillManager::GetInstance()->GetSkill( pMaterialItem->GetItemAttr()->m_iSkillID[0] );
		if (!theSkill)
			return;

		SPItemStatus*	pMaterialItemStatus	=	pMaterialItem->GetItemStatus();

		if( pMaterialItemStatus == NULL )
			return;

		// 아이템 유효성 체크 [10/31/2006 AJJIYA]
		if( pMaterialItemStatus->IsTimeAvailable() == false )
		{
			SPItemAttr*	pMaterialItemAttr	=	pMaterialItem->GetItemAttr();

			if( pMaterialItemAttr != NULL )
				SetError( GS_PET_LEVELUP_ITEM_EXPIRE , pMaterialItemAttr->m_strName.c_str() );

			return;
		}

		if( m_pMaterialItem != NULL )
		{
			SetInvenItemUnlock( m_pMaterialItem , WIID_PET_LEVELUP_UIUNIT_POS_MATERIAL );
		}

		SPWindow*		pMaterialPanel		=	Find( WIID_PET_LEVELUP_PANEL_CONTENTS , true );

		if( pMaterialPanel == NULL )
			return;

		//////////////////////////////////////////////////////////////////////////

		SPWindow*	pWindow	=	NULL;

		pWindow	=	pMaterialPanel->Find( WIID_PET_LEVELUP_MATERIAL_BOX );

		if( pWindow != NULL )
			pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );

		SPUIItemUnit*	pUIItemUnit	=	NULL;

		pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_MATERIAL );
		if( pUIItemUnit != NULL)
		{
			pUIItemUnit->SetItemID( pMaterialItem->GetItemStatus()->GetItemID() );
			pUIItemUnit->ReResourceLoad();
			pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
		}
		//--------------------------------------------------

		m_pMaterialItem	=	pMaterialItem;
		m_eCurState		=	PET_LEVELUP_STATE_READY_MATERIAL;

		//////////////////////////////////////////////////////////////////////////

		int i = 0;
		int iMaterialDataCount	=	1;

		bool	bResultBox	=	true;

		pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_RESULT );

		if( pUIItemUnit != NULL )
		{
			if( bResultBox == true )
				pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
			else
				pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
		}


		pWindow	=	Find( WIID_PET_LEVELUP_RUN , true );

		if( pWindow != NULL )
		{
			pWindow->SetEnable( bResultBox );

			pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP , true );

			if( bResultBox == true )
			{
				m_eCurState = PET_LEVELUP_STATE_READY_UPGRADE;

				if( pWindow != NULL )
					pWindow->Hide();

				pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP_CURSOR , true );

				if( pWindow != NULL )
					pWindow->Hide();

				pWindow	=	Find( WIID_PET_LEVELUP_RUN_TOOLTIP , true );

				if( pWindow != NULL )
					pWindow->Show();
			}
			else
			{
				m_eCurState = PET_LEVELUP_STATE_READY_MATERIAL;

				if( pWindow != NULL )
					pWindow->Show();

				pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP_CURSOR , true );

				if( pWindow != NULL )
					pWindow->Show();

				pWindow	=	Find( WIID_PET_LEVELUP_RUN_TOOLTIP , true );

				if( pWindow != NULL )
					pWindow->Hide();
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	SetInvenItemLock( m_pMaterialItem , WIID_PET_LEVELUP_UIUNIT_POS_MATERIAL );
}

void SPWindowPetLevelUp::SendServerRun()
{
	if( m_pUpgradeItem == NULL )
		return;

	SPItemStatus* pUpgradeItemStatus	=	m_pUpgradeItem->GetItemStatus();

	if( pUpgradeItemStatus == NULL || g_pItemCluster == NULL )
		return;

	if (!m_pUpgradeItem->GetItemAttr())
		return;
	bool bNewPet = (m_pUpgradeItem->GetItemAttr()->m_iNewPet == 0 ? false : true);

	if (!bNewPet)
	{
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

		MATERIAL_DATA	stMaterialData;
		UINT8			uiMaterialCount		=	0;
		SPItemStatus*	pMaterialItemStatus	=	NULL;
		int				iMaterialCount		=	0;
		int				iMaterialPos		=	0;
		int				i					=	0;

		if( m_pMaterialItem == NULL )
			return;

		if( CheckMaterialCondition( iMaterialCount , iMaterialPos , m_pUpgradeItem , m_pMaterialItem , false ) == false )
		{
			return;
		}

		m_pMaterialItem->ConvertContainerItem( &Item );

		stMaterialData.iContainerType	=	m_pMaterialItem->GetContainerType();
		stMaterialData.iContainerSlot	=	m_pMaterialItem->GetSlotIndex() + 1;
		stMaterialData.iItemCount		=	pItemUpgrade->m_stMaterial[ 0 ].m_iCount;
		stMaterialData.iItemNo			=	Item.Item.iItemNo;

		++uiMaterialCount;

		if( uiMaterialCount == 0 )
			return;

		kPacket.AddUINT8( uiMaterialCount );
		kPacket.AddData( &stMaterialData , sizeof( MATERIAL_DATA ) * uiMaterialCount );

		//////////////////////////////////////////////////////////////////////////

		kPacket.AddUINT64( pItemUpgrade->m_iResult );

		//////////////////////////////////////////////////////////////////////////

		for( i = 0 ; i < MAX_ITEM_UPGRADE_REQUIRE ; ++i )
		{
			if( g_pCheckManager->CheckCondition( pItemUpgrade->m_stRequireType[ i ].m_iType , pItemUpgrade->m_stRequireType[ i ].m_iID , pItemUpgrade->m_stRequireType[ i ].m_iValue1 , pItemUpgrade->m_stRequireType[ i ].m_iValue2 ) == false )
				return;
		}

		//////////////////////////////////////////////////////////////////////////
		
		g_pNetworkManager->GetPacketHandler()->PostMsg( &kPacket );
		g_pItemCluster->SetItemUpgradeState( SPIU_STATE_PET_LEVELUP );
	}
	else
	{
		CPacket packet( PET_CS_LEARN_SKILL );
		CONTAINER_SLOT slot;
		slot.ContainerType = m_pMaterialItem->GetContainerType();
		slot.SlotIndex = m_pMaterialItem->GetSlotIndex()+1;
		packet.AddData( &slot, sizeof(slot) );
		packet.AddUINT64( *(ITEMNO*)m_pUpgradeItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO ));
		packet.AddUINT32( m_pMaterialItem->GetItemAttr()->m_iSkillID[0] );
		SPSkill *theSkill = SPSkillManager::GetInstance()->GetSkill( m_pMaterialItem->GetItemAttr()->m_iSkillID[0] );
		if (theSkill)
			packet.AddUINT32( theSkill->GetBaseLevel() );
		else
			return;
		g_pNetworkManager->SPPerformMessage( PET_CS_LEARN_SKILL, (WPARAM)0, (LPARAM)&packet );
	}
}

void SPWindowPetLevelUp::ResetMaterialData()
{
	bool bNewPet = false;
	if( m_pUpgradeItem && m_pUpgradeItem->GetItemAttr() )
		bNewPet = (m_pUpgradeItem->GetItemAttr()->m_iNewPet == 0 ? false : true);

	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_PET_LEVELUP_RUN , true );

	if( pWindow != NULL )
	{
		pWindow->SetEnable( false );
	}

	pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_BOX , true );

	if( pWindow != NULL )
	{
		pWindow->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f ) );
		pWindow->Show();
	}

	SPUIItemUnit*	pUIItemUnit	=	NULL;

	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_MATERIAL );

	if( pUIItemUnit != NULL )
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );

	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_RESULT );

	if( pUIItemUnit != NULL )
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );

	if( m_pMaterialItem == NULL )
		return;

	pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP , true );

	if( pWindow != NULL )
	{
		pWindow->Show();
	}

	pWindow	=	Find( WIID_PET_LEVELUP_MATERIAL_TOOLTIP_CURSOR , true );

	if( pWindow != NULL )
	{
		pWindow->Show();
	}

	m_eCurState	=	PET_LEVELUP_STATE_READY_MATERIAL;

	SetInvenItemUnlock( m_pMaterialItem , WIID_PET_LEVELUP_UIUNIT_POS_MATERIAL );

	if (bNewPet)
	{
		pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( WIID_PET_LEVELUP_UIUNIT_POS_MATERIAL );
		if (pUIItemUnit)
		{
			pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
		}
	}

	m_pMaterialItem	=	NULL;
}

bool SPWindowPetLevelUp::CheckMaterialCondition()
{
	int		iMaterialPos		=	0;
	int		iMaterialCount		=	0;
	int		iCurMaterialCount	=	0;

	if( m_pMaterialItem != NULL )
		++iCurMaterialCount;

	if( CheckMaterialCondition( iMaterialCount , iMaterialPos , m_pUpgradeItem , m_pMaterialItem , false ) == false )
	{
		return false;
	}

	if( iCurMaterialCount != 0 && iCurMaterialCount == iMaterialCount )
		return true;

	return false;
}

bool SPWindowPetLevelUp::CheckMaterialCondition( int& iOutMaterialCount , int& iOutMaterialPos , SPItem* pUpgradeItem , SPItem* pMaterialItem , bool bMultiCheck )
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

		if( m_pMaterialItem != NULL && bMultiCheck == true )
		{
			bErrorIsMaterial	=	true;
			continue;
		}

		bFind			=	true;
		iOutMaterialPos	=	i;
	}

	if( bFind == false )
	{
		GLOBAL_STRING_PET_LEVELUP	eErrorID	=	GS_PET_LEVELUP_NULL;

		if( bErrorMaterialRare == true )
			eErrorID	=	GS_PET_LEVELUP_MATERIAL_RARE;

		if( bErrorMaterialCount == true )
			eErrorID	=	GS_PET_LEVELUP_MATERIAL_COUNT;

		if( bErrorMaterialDifferent == true )
			eErrorID	=	GS_PET_LEVELUP_MATERIAL_DIFFERENT;

		if( bErrorIsMaterial == true )
			eErrorID	=	GS_PET_LEVELUP_IS_MATERIAL;

		SetError( eErrorID );
	}

	return bFind;
}

//////////////////////////////////////////////////////////////////////////

void SPWindowPetLevelUp::SetError( int iGloalStringID , const char* pszItemString /* = NULL  */ )
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

void SPWindowPetLevelUp::SetError( const char* pszErrorString )
{
	if( g_pInterfaceManager == NULL || pszErrorString == NULL )
		return;

	D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pszErrorString , (LPARAM)&color );
}

SPItem* SPWindowPetLevelUp::GetInvenPickUpItem()
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

void SPWindowPetLevelUp::SetInvenItemLock( SPItem* pkItem , int iIndex )
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	if( pkItem == NULL )
		return;
	
	SPPlayerInvenArchive* pkInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInven ) {
		pkInven->SetBlockItem(iIndex, pkItem->GetContainerType(), pkItem->GetSlotIndex());
	}
}

void SPWindowPetLevelUp::SetInvenItemUnlock( SPItem* pkItem , int iIndex )
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

void SPWindowPetLevelUp::SetInvenItemAllUnlock()
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	SPPlayerInvenArchive* pkInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInven ) {
		//pkInven->ClearAllUserShopItem();
		pkInven->ClearAllBlockItem();
	}
}
