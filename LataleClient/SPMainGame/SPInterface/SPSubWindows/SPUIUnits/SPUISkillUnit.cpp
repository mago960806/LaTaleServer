// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-6-28    15:26 
//***************************************************************************

#include "SPCommon.h"
#include "SPUIUnit.h"

#include "SPPlayerActionManager.h"
#include "SPSkill.h"
#include "SPMotionStatus.h"
#include "SPSkillArchive.h"
#include "SPSkillManager.h"
#include "SPCommandConvert.h"
#include "SPGOBManager.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "SPEffect.h"
#include "SPGOBStatus.h"
#include "SPComboManager.h"

#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemCluster.h"
#include "SPPlayerInvenArchive.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPUISkillUnit.h"
#include "SPUIItemUnit.h"

SPUISkillUnit::SPUISkillUnit()
{
	Init();
}

SPUISkillUnit::~SPUISkillUnit()
{
	Clear();
}

//////////////////////////////////////////////////////////////////////////
//
//	기본 인터페이스
//

void SPUISkillUnit::Init()
{
	SPUIUnit::Init();

	m_iSkillID						=	0;
	m_eType							=	SIT_NULL;
	m_iItemID						=	0;
	m_stContainerSlot.ContainerType	=	CONTAINER_TYPE_INVALID;
	m_stContainerSlot.SlotIndex		=	0;

	m_pComboTexture					=	NULL;

	m_rtComboSrc.left				=	0;
	m_rtComboSrc.top				=	0;
	m_rtComboSrc.right				=	0;
	m_rtComboSrc.bottom				=	0;

	m_bCombo						=	FALSE;
	m_stComboColor					=	INITCOLOR;

	m_fComboLimitTime				=	0.2f;
	m_fComboAccmulateTime			=	0.0f;

	m_bToggle						=	FALSE;
	m_iCurToggleFrame				=	0;
	m_iMaxToggleFrame				=	4;
	m_fToggleLimitTime				=	0.1f;
	m_fToggleAccmulateTime			=	0.0f;
	m_pToggleTexture				=	NULL;
	m_rtToggleSrc.left				=	0;
	m_rtToggleSrc.top				=	0;
	m_rtToggleSrc.right				=	0;
	m_rtToggleSrc.bottom			=	0;
	m_ptToogleRenderOffset.x		=	-16;
	m_ptToogleRenderOffset.y		=	-16;
	m_ptToggleSize.x				=	64;
	m_ptToggleSize.y				=	64;
}

void SPUISkillUnit::Clear()
{
	SPUIUnit::Clear();

	SAFE_RELEASE( m_pComboTexture );
}

void SPUISkillUnit::Process( float fTime )
{
	SPUIUnit::Process( fTime );

	ProcessCombo( fTime );

	ProcessToggle( fTime );
}

void SPUISkillUnit::Render()
{
	if( IsTexture() == FALSE || IsRender() == FALSE || g_pVideo == NULL )
		return;

	g_pVideo->Flush();

	SPUIUnit::Render();

	g_pVideo->Flush();

	if( GetToggle() == TRUE && GetStatus( STATUS_UI_UNIT_DISABLE ) == FALSE && GetToggleTexture() != NULL )
	{
		g_pVideo->SetTextureEffect( TEXTURE_EFFECT_NORMAL );
		m_pToggleTexture->SetColor( INITCOLOR );
		m_pToggleTexture->RenderTexture( m_ptPos.x + m_ptToogleRenderOffset.x , m_ptPos.y + m_ptToogleRenderOffset.y , &m_rtToggleSrc );
		m_pToggleTexture->SetColor();
		g_pVideo->SetTextureEffect( TEXTURE_EFFECT_NULL );
		g_pVideo->Flush();
	}

	if( GetCombo() == TRUE && GetStatus( STATUS_UI_UNIT_DISABLE ) == FALSE && GetComboTexture() != NULL )
	{
		m_pComboTexture->SetColor( m_stComboColor );
		m_pComboTexture->RenderTexture( m_ptPos.x , m_ptPos.y , &m_rtComboSrc );
		m_pComboTexture->SetColor();
		g_pVideo->Flush();
	}
}

BOOL SPUISkillUnit::Use()
{
	if( SPUIUnit::Use() == FALSE )
		return FALSE;

	SPPlayerActionManager*	pPlayerActionManager	=	SPPlayerActionManager::GetInstance();

	if( m_eType == SIT_ACTION )
	{
		SPSkillManager*			pSkillManager		=	SPSkillManager::GetInstance();
		SPActionInterface*		pActionInterface	=	pSkillManager->GetActionInterface( m_iSkillID );

		if( pActionInterface == NULL )
			return FALSE;

		int iActionCommand	=	pActionInterface->GetActionCommand();

		if( iActionCommand == -1 )
			return FALSE;

		if( iActionCommand == ACTION_GUARD )
			pPlayerActionManager->SetProcessSkip( TRUE );

		pPlayerActionManager->AddAction( iActionCommand );
	}
	else if( m_eType == SIT_PET )
	{
		pPlayerActionManager->AddAction( ACTION_SKILL );
		pPlayerActionManager->SetItemSkillID( m_iSkillID , m_stContainerSlot );
	}
	else if (SIT_NEWPET == m_eType)
	{
		pPlayerActionManager->AddAction( ACTION_SKILL );
		pPlayerActionManager->SetItemSkillID( m_iSkillID , m_stContainerSlot );
	}
	else if( m_eType == SIT_ITEMMIX )
	{
		g_pInterfaceManager->SPChildWndSendMessage( WIID_ITEM_MIX , SPIM_ITEM_MIX_SHOW , (WPARAM)m_iSkillID , NULL );
	}
	else
	{
		pPlayerActionManager->AddAction( ACTION_SKILL );
		pPlayerActionManager->SetSkillID( m_iSkillID );
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	스킬 아이디 인터페이스
//

void SPUISkillUnit::SetSkillID( int iSkillID )
{
	m_iSkillID	=	iSkillID;
}

int SPUISkillUnit::GetSkillID()
{
	return m_iSkillID;
}

//////////////////////////////////////////////////////////////////////////
//
//	스킬 인벤토리 타입 인터페이스
//

void SPUISkillUnit::SetType( SKILL_INVENTORY_TYPE eType )
{
	m_eType	=	eType;
}

SKILL_INVENTORY_TYPE SPUISkillUnit::GetType()
{
	return m_eType;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이템 아이디 인터페이스
//

void SPUISkillUnit::SetItemID( int iItemID )
{
	m_iItemID	=	iItemID;
}

int SPUISkillUnit::GetItemID()
{
	return m_iItemID;
}

//////////////////////////////////////////////////////////////////////////
//
//	CONTAINER_SLOT 인터페이스 - 아이템 스킬일 경우 CONTAINER_SLOT를 세팅해야 한다.
//

void SPUISkillUnit::SetContainerSlot( CONTAINER_SLOT stContainerSlot )
{
	m_stContainerSlot	=	stContainerSlot;
}

CONTAINER_SLOT SPUISkillUnit::GetContainerSlot()
{
	return m_stContainerSlot;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이템 정보 로드 인터페이스
//

void SPUISkillUnit::ReItemInfoLoad()
{
	SPItemAttr*	pSkillItem	=	NULL;

	g_pItemCluster->GetItemInfo( m_iItemID , pSkillItem );

	if( pSkillItem == NULL )
		return;

	CONTAINER_TYPE	eContainerType	=	CONTAINER_TYPE_INVALID;

	if( pSkillItem->m_eType == ITEM_TYPE_FIGURE )
	{
		if( pSkillItem->m_eSubType == ITEM_SUBTYPE_EQUIP_EQUIP )
			eContainerType	=	FIGURE_EQUIP;
		else if( pSkillItem->m_eSubType == ITEM_SUBTYPE_EQUIP_FASHION )
			eContainerType	=	FIGURE_FASHION;
		else if (pSkillItem->m_eSubType == ITEM_SUBTYPE_EQUIP_EFFECT)
			eContainerType	=	FIGURE_EFFECT;
	}
	else if( pSkillItem->m_eType == ITEM_TYPE_PET )
	{
		eContainerType	=	FIGURE_PET;
	}

	//////////////////////////////////////////////////////////////////////////

	int	iSlotIndex	=	(int)pSkillItem->m_ePosID1	-	1;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPPlayerInvenArchive*	pLocalPlayerInvenArchive	=	pLocalPlayer->GetInvenArchive();

	if( pLocalPlayerInvenArchive == NULL )
		return;

	SPItem*	pNowEquipItem	=	NULL;

	pNowEquipItem	=	pLocalPlayerInvenArchive->GetInventoryItem( eContainerType , iSlotIndex );

	if( pNowEquipItem == NULL )
	{
		iSlotIndex	=	(int)pSkillItem->m_ePosID2	-	1;
		pNowEquipItem	=	pLocalPlayerInvenArchive->GetInventoryItem( eContainerType , iSlotIndex );

		if( pNowEquipItem == NULL )
			return;
	}

	m_stContainerSlot.ContainerType	=	eContainerType;
	m_stContainerSlot.SlotIndex		=	iSlotIndex;

	if( pNowEquipItem->GetItemStatusValue( MV_ITEM_GETUID ) == m_iItemID )
		return;

	//////////////////////////////////////////////////////////////////////////

	iSlotIndex	=	(int)pSkillItem->m_ePosID2	-	1;
	pNowEquipItem	=	pLocalPlayerInvenArchive->GetInventoryItem( eContainerType , iSlotIndex );

	if( pNowEquipItem == NULL )
		return;

	if( pNowEquipItem->GetItemStatusValue( MV_ITEM_GETUID ) != m_iItemID )
		return;

	m_stContainerSlot.ContainerType	=	eContainerType;
	m_stContainerSlot.SlotIndex		=	iSlotIndex;
}

//////////////////////////////////////////////////////////////////////////
//
//	자기 자신 복사 인터페이스
//

SPUIUnit* SPUISkillUnit::ClonePtr()
{
	return new SPUISkillUnit;
}

BOOL SPUISkillUnit::Copy( SPUIUnit** ppDestUIUnit )
{
	SPUISkillUnit*	pUISkillUnit	=	(SPUISkillUnit*)(*ppDestUIUnit);

	if( pUISkillUnit == NULL )
		return FALSE;

	SPTexture*		pComboTexture	=	pUISkillUnit->GetComboTexture();

	SAFE_RELEASE( pComboTexture );

	pUISkillUnit->SetComboTexture( pComboTexture );

	if( SPUIUnit::Copy( ppDestUIUnit ) == FALSE )
		return FALSE;

	pUISkillUnit->SetSkillID( GetSkillID() );
	pUISkillUnit->SetType( GetType() );
	pUISkillUnit->SetItemID( GetItemID() );
	pUISkillUnit->SetContainerSlot( GetContainerSlot() );

	pUISkillUnit->SetComboTexture( GetComboTexture() );
	pUISkillUnit->SetComboRectSrc( GetComboRectSrc() );
	pUISkillUnit->SetCombo( GetCombo() );
	pUISkillUnit->SetComboLimitTime( GetComboLimitTime() );

	pUISkillUnit->SetToggle( GetToggle() );
	pUISkillUnit->SetToggleTexture( GetToggleTexture() );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이디로 리소스 재 로드
//

BOOL SPUISkillUnit::ReResourceLoad()
{
	SKILL_UI_INFO*		pUIInfo	=	NULL;
	SPSkillManager*		pSkillManager		=	SPSkillManager::GetInstance();

	if( m_eType == SIT_ACTION || m_eType == SIT_NULL )
	{
		SPActionInterface*	pActionInterface	=	pSkillManager->GetActionInterface( m_iSkillID );

		if( pActionInterface == NULL && m_eType == SIT_ACTION )
			return FALSE;

		if( pActionInterface != NULL )
			pUIInfo	=	pActionInterface->GetUIInfo();
	}

	if( ( m_eType == SIT_NULL || m_eType != SIT_ACTION ) && pUIInfo == NULL )
	{
		SPSkill*	pSkill	=	pSkillManager->GetSkill( m_iSkillID );

		if( pSkill == NULL )
			return FALSE;

		pUIInfo	=	pSkill->GetUIInfo();
	}

	if( pUIInfo == NULL )
		return FALSE;

	SetType( pUIInfo->eInventoryType );

	return SetResource( pUIInfo->iIcon , pUIInfo->iIconIndex );
}

//////////////////////////////////////////////////////////////////////////
//
//	콤보 텍스처 인터페이스
//

void SPUISkillUnit::SetComboTexture( SPTexture* pTexture )
{
	if( pTexture != NULL )
		SetComboTexture( pTexture->GetResName() );
	else
		m_pComboTexture	=	pTexture;
}

BOOL SPUISkillUnit::SetComboTexture( const char* pstrTexture )
{
	SAFE_RELEASE( m_pComboTexture );

	return g_pVideo->GetTextureMgr()->LoadTexture( pstrTexture , &m_pComboTexture );
}

SPTexture* SPUISkillUnit::GetComboTexture()
{
	return m_pComboTexture;
}

//////////////////////////////////////////////////////////////////////////
//
//	콤보 텍스처 영역 인터페이스
//

void SPUISkillUnit::SetComboRectSrc( RECT rtSrc )
{
	m_rtComboSrc		=	rtSrc;
}

void SPUISkillUnit::SetComboRectSrc( int ixLeft , int iyTop , int ixRight , int iyBottom )
{
	SetRect( &m_rtComboSrc , ixLeft , iyTop , ixRight , iyBottom );
}

RECT SPUISkillUnit::GetComboRectSrc()
{
	return m_rtComboSrc;
}

//////////////////////////////////////////////////////////////////////////
//
//	콤보 세팅 인터페이스
//

void SPUISkillUnit::SetCombo( BOOL bCombo )
{
	if( m_bCombo == bCombo )
		return;

	m_bCombo				=	bCombo;
	m_fComboAccmulateTime	=	0.0f;
}

BOOL SPUISkillUnit::GetCombo()
{
	return m_bCombo;
}

//////////////////////////////////////////////////////////////////////////
//
//	콤보 표시 시간 인터페이스
//

void SPUISkillUnit::SetComboLimitTime( float fLimitTime )
{
	m_fComboLimitTime	=	fLimitTime;
}

float SPUISkillUnit::GetComboLimitTime()
{
	return m_fComboLimitTime;
}

//////////////////////////////////////////////////////////////////////////
//
//	토글 텍스처 인터페이스
//

void SPUISkillUnit::SetToggleTexture( SPTexture* pTexture )
{
	if( pTexture != NULL )
		SetToggleTexture( pTexture->GetResName() );
	else
		m_pToggleTexture	=	pTexture;
}

BOOL SPUISkillUnit::SetToggleTexture( const char* pstrTexture )
{
	SAFE_RELEASE( m_pToggleTexture );

	return g_pVideo->GetTextureMgr()->LoadTexture( pstrTexture , &m_pToggleTexture );
}

SPTexture* SPUISkillUnit::GetToggleTexture()
{
	return m_pToggleTexture;
}

//////////////////////////////////////////////////////////////////////////
//
//	토클 스킬 인터페이스
//

void SPUISkillUnit::SetToggle( BOOL bToggle )
{
	m_bToggle	=	bToggle;
}

BOOL SPUISkillUnit::GetToggle()
{
	return m_bToggle;
}

//////////////////////////////////////////////////////////////////////////
//
//	콤보 표시 프로세서
//

void SPUISkillUnit::ProcessCombo( float fTime )
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( GetStatus( STATUS_UI_UNIT_DISABLE ) == TRUE || GetStatus( STATUS_UI_UNIT_COOLTIME ) == TRUE )
	{
		SetCombo( FALSE );
		return;
	}

	if( pLocalPlayer != NULL )
	{
		SPSkillArchive*		pSkillArchive	=	pLocalPlayer->GetSkillArchive();
		SPSkillActivity*	pSkillActivity	=	pSkillArchive->GetSkillActivity( m_iSkillID );

		if( pSkillActivity != NULL )
		{
			SPSkill*	pSkill	=	pSkillActivity->GetSkill();

			if( pSkill != NULL )
			{
				SKILL_COMBO*	pSkillCombo	=	pSkill->GetCombo();

				if( pSkillCombo != NULL )
				{
					if( SPComboManager::GetInstance()->CanConnection( pSkillCombo->iSelfComboID ) == TRUE )
					{
						SetCombo( TRUE );
					}
					else
					{
						SetCombo( FALSE );
					}
				}
			}
		}
	}

	if( GetCombo() == FALSE )
		return;

	m_fComboAccmulateTime	+=	fTime;

	float fRatio	=	m_fComboAccmulateTime	/	m_fComboLimitTime;

	if( fRatio > 1.0f )
		fRatio	=	1.0f;

	m_stComboColor.a	=	fRatio;

}

//////////////////////////////////////////////////////////////////////////
//
//	Toggle 표시 프로세서
//

void SPUISkillUnit::ProcessToggle( float fTime )
{
	if( GetToggle() == FALSE )
		return;

	m_fToggleAccmulateTime	+=	fTime;

	if( m_fToggleAccmulateTime < m_fToggleLimitTime )
		return;

	m_fToggleAccmulateTime	-=	m_fToggleLimitTime;

	if( ++m_iCurToggleFrame >= m_iMaxToggleFrame )
		m_iCurToggleFrame	=	0;

	m_rtToggleSrc.left		=	m_iCurToggleFrame	*	m_ptToggleSize.x;
	m_rtToggleSrc.top		=	0;
	m_rtToggleSrc.right		=	m_rtToggleSrc.left	+	m_ptToggleSize.x;
	m_rtToggleSrc.bottom	=	m_rtToggleSrc.top	+	m_ptToggleSize.y;
}
