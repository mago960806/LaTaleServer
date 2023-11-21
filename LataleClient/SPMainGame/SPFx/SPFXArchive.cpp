#include <algorithm>
#include <functional>
#include <string>
#include <vector>

#include "SPCommon.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPGameObjectDef.h"
#include "SPGameObject.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXFrameAnimationUnit.h"
#include "SPFXManager.h"
#include "SPFXDamageUnit.h"
#include "SPFXGroupUnit.h"

#include "SPFXArchive.h"

#include "SPGOBClusterDEF.h"
#include "SPGOBCluster.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"

#include "SPSubGameManager.h"
#include "SPZone.h"

SPFXArchive::SPFXArchive( SPGameObject* pParent )
{
	SetNULL();

	m_pParentGOB	=	pParent;
}

SPFXArchive::~SPFXArchive()
{	
	Clean();
}

void SPFXArchive::SetNULL()
{
	ClearData();

	m_fParentPosX	=	0.0f;
	m_fParentPosY	=	0.0f;

	m_mpFXGroupUnit.clear();
}

void SPFXArchive::Init()
{
	m_vNextFXGroupIDOrder.reserve( 100 );
	m_vNextGOBID.reserve( 100 );
}

void SPFXArchive::Clean()
{
	m_IterKey	=	m_mpFXGroupUnit.begin();
	m_IterEnd	=	m_mpFXGroupUnit.end();

	while( m_IterKey != m_IterEnd )
	{
//		SAFE_DELETE( (*IterKey).second );
		(*m_IterKey).second->Destory();
		++m_IterKey;
	}

	m_mpFXGroupUnit.clear();

	m_vNextFXGroupIDOrder.clear();
	m_vNextGOBID.clear();
	m_mNextFXInfo.clear();
}

void SPFXArchive::Process( float fTime )
{
	m_fPosX		=	m_fParentPosX;
	m_fPosY		=	m_fParentPosY;
	m_eFacing	=	FACING_RIGHT;

	if( m_pParentGOB != NULL )
	{
		m_fPosX		= (float)m_pParentGOB->SPGOBPerformMessage( SPGM_GETPOSX );
		m_fPosY		= (float)m_pParentGOB->SPGOBPerformMessage( SPGM_GETPOSY );
		m_eFacing	= (FACING)m_pParentGOB->SPGOBPerformMessage( SPGM_GETFACING );
	}

	if (m_pParentGOB && m_pParentGOB->GetClassID() == CLASS_ID_AVATAR
		&& ((SPPlayer *)m_pParentGOB)->GetLobbyAvatar() != LOBBY_CASH )
	{
		SPPlayer *pPlayer = (SPPlayer*)m_pParentGOB;
		SPAvatarModelUnit	*pAvatar = (SPAvatarModelUnit*)pPlayer->GetModelUnit();
		if (pAvatar)
		{
			m_fPosX = (float)pAvatar->GetRenderPosX() + g_pCameraManagr->GetCameraStruct()->fOffsetX;
			m_fPosY = (float)pAvatar->GetRenderPosY() + g_pCameraManagr->GetCameraStruct()->fOffsetY;
		}
	}

	//[xialin 2008/08/06]add
	if ( m_pParentGOB && m_pParentGOB->GetClassID() == CLASS_ID_AVATAR
		&& ((SPPlayer *)m_pParentGOB)->GetLobbyAvatar() == LOBBY_CASH )
	{
		m_fPosX -= CASHSHOP_OFFSETCAMX;
		m_fPosY -= CASHSHOP_OFFSETCAMY;
	}

	m_IterKey	=	m_mpFXGroupUnit.begin();
	m_IterEnd	=	m_mpFXGroupUnit.end();

	while( m_IterKey != m_IterEnd )
	{
		m_pFXGroupUnit	=	(*m_IterKey).second;
//		UINT	uiFXGroupID	=	(*IterKey).first;

		if( m_pFXGroupUnit == NULL )
			continue;

		m_pFXGroupUnit->SetParentPos( m_fPosX , m_fPosY );
		m_pFXGroupUnit->SetParentFacing( m_eFacing );
		m_pFXGroupUnit->Process( fTime );

		switch( m_pFXGroupUnit->GetState() )
		{
		case FS_INIT:
		case FS_ENABLE:
			++m_IterKey;
			break;
		case FS_RELEASE:
//			SAFE_DELETE( pFXGroupUnit );
			m_IterKey	=	m_mpFXGroupUnit.erase( m_IterKey );
			m_IterEnd	=	m_mpFXGroupUnit.end();
			break;
		}
	}
}

void SPFXArchive::Render( float fTime , SP_FXM_RENDER_LAYER eRenderLayer /* = FXM_RENDER_LAYER_ALL */ )
{
	if( g_pVideo != NULL )
		g_pVideo->Flush();

	m_IterKey	=	m_mpFXGroupUnit.begin();
	m_IterEnd	=	m_mpFXGroupUnit.end();

	while( m_IterKey != m_IterEnd )
	{
		m_pFXGroupUnit	=	(*m_IterKey).second;
//		UINT	uiFXGroupID	=	(*IterKey).first;

		if( m_pFXGroupUnit == NULL )
			continue;

		m_pFXGroupUnit->Render( fTime , eRenderLayer );

		++m_IterKey;
	}
}

bool SPFXArchive::IsEmpty()
{
	return m_mpFXGroupUnit.empty();
}

void SPFXArchive::SetFXGroup( UINT uiFXGroupID )
{
	m_uiFXGroupID	=	uiFXGroupID;
}

void SPFXArchive::SetLPARAM( LPARAM lParam )
{
	m_tlParam	=	lParam;
}

void SPFXArchive::SetSrcPosX( float fSrcPosX )
{
	m_fSrcPosX	=	fSrcPosX;
}

void SPFXArchive::SetSrcPosY( float fSrcPosY )
{
	m_fSrcPosY	=	fSrcPosY;
}

void SPFXArchive::SetDestPosX( float fDestPosX )
{
	m_fDestPosX	=	fDestPosX;
}

void SPFXArchive::SetDestPosY( float fDestPosY )
{
	m_fDestPosY	=	fDestPosY;
}

void SPFXArchive::SetLoop( BOOL bLoop )
{
	m_bLoop	=	bLoop;
}

void SPFXArchive::SetContinue( BOOL bContinue )
{
	m_bContinue	=	bContinue;
}

void SPFXArchive::SetVelocity( float fVelocity )
{
	m_fVelocity	=	fVelocity;
}

void SPFXArchive::SetLifeTime( float fLifeTime )
{
	m_fLifeTime	=	fLifeTime;
}

void SPFXArchive::SetTargetGOBID( GU_ID tTargetGOBID )
{
	m_tTargetGOBID	=	tTargetGOBID;
}

void SPFXArchive::SetFollowParent( int iFollowParent )
{
	m_iFollowParent	=	iFollowParent;
}

void SPFXArchive::SetDealyTime( float fDealyTime )
{
	m_fDealyTime	=	fDealyTime;
}

void SPFXArchive::SetParentPos( float fPosX , float fPosY )
{
	m_fParentPosX	=	fPosX;
	m_fParentPosY	=	fPosY;
}

void SPFXArchive::SetOffsetPos( float fPosX , float fPosY )
{
	m_fOffsetPosX	=	fPosX;
	m_fOffsetPosY	=	fPosY;
}

void SPFXArchive::AddNextFXGroupID( UINT uiNextFXGroupID )
{
	m_vNextFXGroupIDOrder.push_back( uiNextFXGroupID );

	STD_VECTOR_NEXTGOBID::iterator	Iter	=	m_vNextGOBID.begin();

	while ( Iter != m_vNextGOBID.end() )
	{
		m_mNextFXInfo.insert( STD_MULTIMAP_NEXTFXINFO::value_type( uiNextFXGroupID , (*Iter) ) );
		++Iter;
	}

	m_vNextGOBID.clear();
}

void SPFXArchive::AddNextTargetGOBID( GU_ID tTargetGOBID )
{
	m_vNextGOBID.push_back( tTargetGOBID );
}

void SPFXArchive::SetGOBBox( POINT ptGOBBox )
{
	m_ptGOBBox	=	ptGOBBox;
}

void SPFXArchive::SetDamageType( FXUNIT_DAMAGE_TYPE eDamageType )
{
	m_eDamageType	=	eDamageType;
}

void SPFXArchive::SetDamage( int iDamage )
{
	m_iDamage	=	iDamage;
}

void SPFXArchive::SetCritical( bool bCritical )
{
	m_bCritical	=	bCritical;
}

void SPFXArchive::InsertFXGroup( UINT uiFXGroupID /* = 0  */ )
{
	SetFXGroup( uiFXGroupID );

	GU_ID		stGOBID		=	NULL;

	if( m_pParentGOB != NULL )
		stGOBID	=	m_pParentGOB->GetGUID();

	if( m_tTargetGOBID > 0 )
		stGOBID	=	m_tTargetGOBID;

	SPFXGroup*	pFXGroup	=	g_pFXManager->GetFXGroup( m_uiFXGroupID , stGOBID  , m_bEnumType , false );

	if( pFXGroup == NULL )
	{
		ClearData();
		return;
	}

	SPFXGroupUnit*	pFXGroupUnit	=	g_pFXManager->GetFXGroupUnitBuffer();
	
	if( pFXGroupUnit == NULL )
	{
		ClearData();
		return;
	}

	bool bIsUnitNotDel = true;

	if( GetGUID2ClassID( stGOBID ) != CLASS_ID_AVATAR || m_bEnumType == TRUE )
	{
		bIsUnitNotDel	=	false;
	}

	if( m_iDamage > -1 )
	{
		pFXGroupUnit->SetDamageFX( m_iDamage , m_ptGOBBox , m_eDamageType );
		bIsUnitNotDel = false;
	}

	if( m_bCritical == true )
	{
		pFXGroupUnit->SetCritical( m_ptGOBBox );
		bIsUnitNotDel = false;
	}

	float	fPosX	=	m_fParentPosX;
	float	fPosY	=	m_fParentPosY;
	FACING	eFacing	=	FACING_RIGHT;

	if( m_pParentGOB != NULL )
	{
		fPosX	= (float)m_pParentGOB->SPGOBPerformMessage( SPGM_GETPOSX );
		fPosY	= (float)m_pParentGOB->SPGOBPerformMessage( SPGM_GETPOSY );
		eFacing	= (FACING)m_pParentGOB->SPGOBPerformMessage( SPGM_GETFACING );
	}

	SPFXGroup*	pFXGroupNotDel	=	NULL;

	if( bIsUnitNotDel == true )
		pFXGroupNotDel	=	g_pFXManager->GetFXGroup( m_uiFXGroupID , stGOBID  , m_bEnumType , true );

	if( pFXGroupNotDel == NULL )
		bIsUnitNotDel	=	false;

	if( bIsUnitNotDel )
	{
		SPFXGroupUnit*	pFXGroupUnitNotDel	=	g_pFXManager->GetFXGroupUnitBuffer();

		if( pFXGroupUnitNotDel != NULL )
		{
			pFXGroupUnitNotDel->SetParentPos( fPosX , fPosY );
			pFXGroupUnitNotDel->SetParentFacing( eFacing );
			pFXGroupUnitNotDel->SetFXGroup( m_uiFXGroupID );
			pFXGroupUnitNotDel->SetFXGroup( pFXGroupNotDel );
			pFXGroupUnitNotDel->SetTargetGOBID( m_tTargetGOBID );
			pFXGroupUnitNotDel->SetLPARAM( m_tlParam );
			pFXGroupUnitNotDel->SetSrcPosX( m_fSrcPosX );
			pFXGroupUnitNotDel->SetSrcPosY( m_fSrcPosY );
			pFXGroupUnitNotDel->SetDestPosX( m_fDestPosX );
			pFXGroupUnitNotDel->SetDestPosY( m_fDestPosY );
			pFXGroupUnitNotDel->SetLoop( FALSE );
			pFXGroupUnitNotDel->SetContinue( m_bContinue );
			pFXGroupUnitNotDel->SetVelocity( m_fVelocity );
			pFXGroupUnitNotDel->SetLifeTime( m_fLifeTime );
			pFXGroupUnitNotDel->SetEnumType( m_bEnumType );
			pFXGroupUnitNotDel->SetNextFXGroupIDOrder( m_vNextFXGroupIDOrder );
			pFXGroupUnitNotDel->SetNextFXInfo( m_mNextFXInfo );
			pFXGroupUnitNotDel->SetFollowParent( m_iFollowParent );
			pFXGroupUnitNotDel->SetDealyTime( m_fDealyTime );
			pFXGroupUnitNotDel->SetNotDel( TRUE );
			pFXGroupUnitNotDel->SetOffsetPos( m_fOffsetPosX , m_fOffsetPosY );

			if( pFXGroupUnitNotDel->Init() == TRUE )
			{
				m_mpFXGroupUnit.insert( STD_MULTIMAP_SPFXGROUPUNIT::value_type( pFXGroupUnitNotDel->GetFXGroup() , pFXGroupUnitNotDel ) );
			}
			else
			{
				pFXGroupUnitNotDel->Destory();
//				SAFE_DELETE( pFXGroupUnitNotDel );
			}
		}
	}

	pFXGroupUnit->SetParentPos( fPosX , fPosY );
	pFXGroupUnit->SetParentFacing( eFacing );
	pFXGroupUnit->SetFXGroup( m_uiFXGroupID );
	pFXGroupUnit->SetFXGroup( pFXGroup );
	pFXGroupUnit->SetTargetGOBID( m_tTargetGOBID );
	pFXGroupUnit->SetLPARAM( m_tlParam );
	pFXGroupUnit->SetSrcPosX( m_fSrcPosX );
	pFXGroupUnit->SetSrcPosY( m_fSrcPosY );
	pFXGroupUnit->SetDestPosX( m_fDestPosX );
	pFXGroupUnit->SetDestPosY( m_fDestPosY );
	pFXGroupUnit->SetLoop( m_bLoop );
	pFXGroupUnit->SetContinue( m_bContinue );
	pFXGroupUnit->SetVelocity( m_fVelocity );
	pFXGroupUnit->SetLifeTime( m_fLifeTime );
	pFXGroupUnit->SetEnumType( m_bEnumType );
	pFXGroupUnit->SetNextFXGroupIDOrder( m_vNextFXGroupIDOrder );
	pFXGroupUnit->SetNextFXInfo( m_mNextFXInfo );
	pFXGroupUnit->SetFollowParent( m_iFollowParent );
	pFXGroupUnit->SetDealyTime( m_fDealyTime );
	pFXGroupUnit->SetOffsetPos( m_fOffsetPosX , m_fOffsetPosY );

	if( pFXGroupUnit->Init() == TRUE )
	{
		m_mpFXGroupUnit.insert( STD_MULTIMAP_SPFXGROUPUNIT::value_type( uiFXGroupID , pFXGroupUnit ) );
	}
	else
	{
		pFXGroupUnit->Destory();
//		SAFE_DELETE( pFXGroupUnit );
	}

	ClearData();
}

void SPFXArchive::InsertFXGroup( SPID_FX_GROUP eFXGroupID /* = IDFXGR_NULL  */ )
{
	m_bEnumType	=	TRUE;
	InsertFXGroup( (UINT)eFXGroupID );
}

void SPFXArchive::InsertFXGroup( UINT uiFXGroupID, SPFXGroup *pFXGroup )
{
	SetFXGroup( uiFXGroupID );

	if( pFXGroup == NULL )
	{
		ClearData();
		return;
	}

	SPFXGroupUnit*	pFXGroupUnit	=	g_pFXManager->GetFXGroupUnitBuffer();

	if( pFXGroupUnit == NULL )
	{
		ClearData();
		return;
	}

	float	fPosX	=	m_fParentPosX;
	float	fPosY	=	m_fParentPosY;
	FACING	eFacing	=	FACING_RIGHT;

	if( m_pParentGOB != NULL )
	{
		fPosX	= (float)m_pParentGOB->SPGOBPerformMessage( SPGM_GETPOSX );
		fPosY	= (float)m_pParentGOB->SPGOBPerformMessage( SPGM_GETPOSY );
		eFacing	= (FACING)m_pParentGOB->SPGOBPerformMessage( SPGM_GETFACING );
	}

	pFXGroupUnit->SetParentPos( fPosX , fPosY );
	pFXGroupUnit->SetParentFacing( eFacing );
	pFXGroupUnit->SetFXGroup( m_uiFXGroupID );
	pFXGroupUnit->SetFXGroup( pFXGroup );
	pFXGroupUnit->SetTargetGOBID( m_tTargetGOBID );
	pFXGroupUnit->SetLPARAM( m_tlParam );
	pFXGroupUnit->SetSrcPosX( m_fSrcPosX );
	pFXGroupUnit->SetSrcPosY( m_fSrcPosY );
	pFXGroupUnit->SetDestPosX( m_fDestPosX );
	pFXGroupUnit->SetDestPosY( m_fDestPosY );
	pFXGroupUnit->SetLoop( m_bLoop );
	pFXGroupUnit->SetContinue( m_bContinue );
	pFXGroupUnit->SetVelocity( m_fVelocity );
	pFXGroupUnit->SetLifeTime( m_fLifeTime );
	pFXGroupUnit->SetEnumType( m_bEnumType );
	pFXGroupUnit->SetNextFXGroupIDOrder( m_vNextFXGroupIDOrder );
	pFXGroupUnit->SetNextFXInfo( m_mNextFXInfo );
	pFXGroupUnit->SetFollowParent( m_iFollowParent );
	pFXGroupUnit->SetDealyTime( m_fDealyTime );
	pFXGroupUnit->SetOffsetPos( m_fOffsetPosX , m_fOffsetPosY );

	if( pFXGroupUnit->Init() == TRUE )
	{
		m_mpFXGroupUnit.insert( STD_MULTIMAP_SPFXGROUPUNIT::value_type( uiFXGroupID , pFXGroupUnit ) );
	}
	else
	{
		pFXGroupUnit->Destory();
	}

	ClearData();
}

void SPFXArchive::DeleteFXGroup( UINT uiFXGroupID )
{
	m_IterKey	=	m_mpFXGroupUnit.find( uiFXGroupID );
	m_IterEnd	=	m_mpFXGroupUnit.end();

	while( m_IterKey != m_IterEnd )
	{
		if( ((*m_IterKey).first) == uiFXGroupID )
		{
			m_pFXGroupUnit	=	(SPFXGroupUnit*)((*m_IterKey).second);
			if( m_pFXGroupUnit != NULL )
				m_pFXGroupUnit->Destory();

//			SAFE_DELETE( (*m_IterKey).second );
			m_IterKey	=	m_mpFXGroupUnit.erase( m_IterKey );
			m_IterEnd	=	m_mpFXGroupUnit.end();
		}
		else
			break;
	}
}

bool SPFXArchive::IsFXGroup( UINT uiOriginalFXGroupID , UINT uiChangeFXGroupID /* = 0  */ )
{
	m_IterKey	=	m_mpFXGroupUnit.find( uiOriginalFXGroupID );
	m_IterEnd	=	m_mpFXGroupUnit.end();

	while( m_IterKey != m_IterEnd )
	{
		if( ((*m_IterKey).first) == uiOriginalFXGroupID )
		{
			m_pFXGroupUnit	=	(SPFXGroupUnit*)((*m_IterKey).second);

			if( m_pFXGroupUnit != NULL )
			{
				if( uiChangeFXGroupID <= 0 )
					return true;

				if( m_pFXGroupUnit->GetFXGroup() == uiChangeFXGroupID )
					return true;
			}
		}

		return false;
	}

	return false;
}

void SPFXArchive::ClearData()
{
	m_tTargetGOBID	=	0;
	m_uiFXGroupID	=	0;
	m_tlParam		=	NULL;
	m_fSrcPosX		=	0.0f;
	m_fSrcPosY		=	0.0f;
	m_fDestPosX		=	0.0f;
	m_fDestPosY		=	0.0f;
	m_bLoop			=	FALSE;
	m_bContinue		=	FALSE;
	m_fVelocity		=	0.0f;
	m_fLifeTime		=	0.0f;
	m_bEnumType		=	FALSE;
	m_iFollowParent	=	0;
	m_fDealyTime	=	0.0f;
	m_fOffsetPosX	=	0.0f;
	m_fOffsetPosY	=	0.0f;

	m_vNextFXGroupIDOrder.clear();
	m_vNextGOBID.clear();
	m_mNextFXInfo.clear();

	m_ptGOBBox.x	=	0;
	m_ptGOBBox.y	=	0;
	m_eDamageType	=	FDT_ATTACK;
	m_iDamage		=	-1;

	m_bCritical		=	false;
}
