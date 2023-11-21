// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-8-31    14:24 
//***************************************************************************

#include "SPCommon.h"

#include "SPFXDEF.h"
#include "SPRenderModel.h"
#include "SPFXModel.h"
#include "SPGameObject.h"
#include "SPFXUnit.h"
#include "SPFXFrameAnimationUnit.h"
#include "SPFXDamageUnit.h"
#include "SPFXCriticalUnit.h"
#include "SPFXParticleUnit.h"
#include "SPFXManager.h"

#include "SPGOBManager.h"

#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

SPFXGroupUnit::SPFXGroupUnit()
{
	SetNull();
}

SPFXGroupUnit::~SPFXGroupUnit()
{
	Destory();
}

void SPFXGroupUnit::SetNull()
{
	m_eState			=	FS_INIT;
	m_bEnumType			=	FALSE;

	for( m_iIndex = 0 ; m_iIndex < FXM_RENDER_LAYER_COUNT ; ++m_iIndex )
	{
		m_vpFXUnit[ m_iIndex ].clear();
	}

	m_vNextFXGroupIDOrder.clear();
	m_mNextFXInfo.clear();

	m_fParentPosX		=	0.0f;
	m_fParentPosY		=	0.0f;
	m_eParentFacing		=	FACING_RIGHT;
	m_pFXGroup			=	NULL;
	m_tTargetGOBID		=	0;
	m_uiFXGroupID		=	0;
	m_tlParam			=	NULL;
	m_fSrcPosX			=	0.0f;
	m_fSrcPosY			=	0.0f;
	m_fDestPosX			=	0.0f;
	m_fDestPosY			=	0.0f;
	m_bLoop				=	FALSE;
	m_bContinue			=	FALSE;
	m_fVelocity			=	0.0f;
	m_fLifeTime			=	0.0f;
	m_iFollowParent		=	0;
	m_fDealyTime		=	0.0f;
	m_bDealyInit		=	FALSE;
	m_bNotDel			=	FALSE;
	m_fOffsetPosX		=	0.0f;
	m_fOffsetPosY		=	0.0f;

	m_ptGOBBox.x		=	0;
	m_ptGOBBox.y		=	0;
	m_eDamageType		=	FDT_ATTACK;
	m_iDamage			=	-1;
	m_strNumber.clear();
	m_istrLength		=	0;
	m_rcRender.left		=	LONG_MAX;
	m_rcRender.top		=	LONG_MAX;
	m_rcRender.right	=	LONG_MIN;
	m_rcRender.bottom	=	LONG_MIN;

	m_bCritical			=	false;
}

BOOL SPFXGroupUnit::Init()
{
	if( m_pFXGroup == NULL )
		return FALSE;

	for( m_iIndex = 0 ; m_iIndex < FXM_RENDER_LAYER_COUNT ; ++m_iIndex )
	{
		m_vpFXUnit[ m_iIndex ].reserve( 100 );
	}

	m_eState	=	FS_INIT;

	SPGameObject*	pGOB	=	NULL;

	if( m_tTargetGOBID > 0 )
		pGOB	=	g_pGOBManager->Find( m_tTargetGOBID );

	if( m_fSrcPosX == 0.0f )
	{
		if( pGOB != NULL )
			m_fSrcPosX =	(float)( pGOB->SPGOBPerformMessage( SPGM_GETPOSX ) );
		else if( m_tTargetGOBID <= 0 )
			m_fSrcPosX	=	m_fParentPosX;
	}

	if( m_fSrcPosY == 0.0f )
	{
		if( pGOB != NULL )
			m_fSrcPosY =	(float)( pGOB->SPGOBPerformMessage( SPGM_GETPOSY ) );
		else if( m_tTargetGOBID <= 0 )
			m_fSrcPosY	=	m_fParentPosY;
	}

	if( m_fDestPosX == 0.0f )
		m_fDestPosX =	m_fSrcPosX;

	if( m_fDestPosY == 0.0f )
		m_fDestPosY =	m_fSrcPosY;

	m_IterFXIDKey	=	m_pFXGroup->vFXID.begin();

	if( m_iDamage > -1 )
	{
		memset( m_szBuf , 0 , sizeof( m_szBuf ) );
		itoa( m_iDamage , m_szBuf , 10 );

		int istrLength	=	(int)strlen( m_szBuf );

		for( m_iIndex = 0 ; m_iIndex < istrLength ; ++m_iIndex )
		{
			SetDamageString( m_szBuf[ m_iIndex ] , istrLength );
			if( InsertFXModel( *m_IterFXIDKey ) == FALSE )
			{
				Destory();
				return FALSE;
			}
			++m_IterFXIDKey;
		}
	}
	else
	{
		m_IterFXIDEnd	=	m_pFXGroup->vFXID.end();

		for( ; m_IterFXIDKey != m_IterFXIDEnd ; ++m_IterFXIDKey )
		{
			if( InsertFXModel( *m_IterFXIDKey ) == FALSE )
			{
				Destory();
				return FALSE;
			}
		}
	}

	m_eState	=	FS_ENABLE;

	return TRUE;
}

void SPFXGroupUnit::Destory()
{
	for( m_iIndex = 0 ; m_iIndex < FXM_RENDER_LAYER_COUNT ; ++m_iIndex )
	{
		m_IterKey	=	m_vpFXUnit[ m_iIndex ].begin();
		m_IterEnd	=	m_vpFXUnit[ m_iIndex ].end();

		for( ; m_IterKey != m_IterEnd ; ++m_IterKey )
			(*m_IterKey)->Destory();
//			SAFE_DELETE(*Iter);
	}

	SetNull();

	m_eState	=	FS_RELEASE;
}

void SPFXGroupUnit::Process( float fTime )
{
	if( m_eState != FS_ENABLE )
		return;

	//if( ProcessDealy( fTime ) == TRUE )
	//	return;

	m_iCount	=	0;

	for( m_iIndex = 0 ; m_iIndex < FXM_RENDER_LAYER_COUNT ; ++m_iIndex )
	{
		m_IterKey	=	m_vpFXUnit[ m_iIndex ].begin();
		m_IterEnd	=	m_vpFXUnit[ m_iIndex ].end();

		for( ;m_IterKey != m_IterEnd ; )
		{
			m_pFXUnit	=	(SPFXUnit*)(*m_IterKey);

			if( m_pFXUnit == NULL )
				continue;

			m_pFXUnit->SetParentPos( m_fParentPosX + m_fOffsetPosX , m_fParentPosY + m_fOffsetPosY );
			m_pFXUnit->SetParentFacing( m_eParentFacing );
			m_pFXUnit->Process( fTime );

			switch( m_pFXUnit->GetState() )
			{
			case FS_INIT:
			case FS_ENABLE:
				++m_IterKey;
				break;
			case FS_RELEASE:
//				SAFE_DELETE( (*Iter) );
				m_IterKey	=	m_vpFXUnit[ m_iIndex ].erase( m_IterKey );
				m_IterEnd	=	m_vpFXUnit[ m_iIndex ].end();
				break;
			}		
		}

		m_IterKey	=	m_vpFXUnit[ m_iIndex ].begin();
		m_IterEnd	=	m_vpFXUnit[ m_iIndex ].end();

		if( m_IterKey != m_IterEnd )
		{
			m_fDestPosX	=	(*m_IterKey)->GetDestPosX();
			m_fDestPosY	=	(*m_IterKey)->GetDestPosY();
		}

		m_iCount	+=	(int)m_vpFXUnit[ m_iIndex ].size();
	}

	if( m_iCount <= 0 )
	{
		if( m_vNextFXGroupIDOrder.empty() == TRUE )
		{
			m_eState	=	FS_RELEASE;
			return;
		}

		STD_VECTOR_NEXTFXGROUPID::iterator	IterNextIDOrder	=	m_vNextFXGroupIDOrder.begin();

		if( IterNextIDOrder != m_vNextFXGroupIDOrder.end() )
		{
			m_uiFXGroupID	=	(*IterNextIDOrder);

			m_vNextFXGroupIDOrder.erase( IterNextIDOrder );

			int	iGOBIDCount	=	(int)m_mNextFXInfo.count( m_uiFXGroupID );
			STD_MULTIMAP_NEXTFXINFO::iterator	mIterNextFXInfo	=	m_mNextFXInfo.find( m_uiFXGroupID );

			if( iGOBIDCount == 0 )
			{
				float	fOldPosX	=	m_fSrcPosX;
				float	fOldPosY	=	m_fSrcPosY;
				GU_ID	tOldGOBID	=	m_tTargetGOBID;
				float	fVelocity	=	m_fVelocity;

				m_fSrcPosX		=	m_fDestPosX;
				m_fSrcPosY		=	m_fDestPosY;
				m_tTargetGOBID	=	0;
				m_fVelocity		=	0.0f;

				m_pFXGroup	=	g_pFXManager->GetFXGroup( m_uiFXGroupID , m_tTargetGOBID  , m_bEnumType , false );

				Init();

				m_fSrcPosX		=	fOldPosX;
				m_fSrcPosY		=	fOldPosY;
				m_tTargetGOBID	=	tOldGOBID;
				m_fVelocity	=	fVelocity;

				return;
			}

			SPGameObject*	pGOB;
			SPFXArchive*	pkFXArchive;
			GU_ID			tGOBID;

			while( mIterNextFXInfo != m_mNextFXInfo.end() )
			{
				if( ((*mIterNextFXInfo).first) == m_uiFXGroupID )
				{
					tGOBID	=	(*mIterNextFXInfo).second;

					if( tGOBID == m_tTargetGOBID )
					{
						float	fOldPosX	=	m_fSrcPosX;
						float	fOldPosY	=	m_fSrcPosY;
						float	fVelocity	=	m_fVelocity;

						m_fSrcPosX	=	m_fDestPosX;
						m_fSrcPosY	=	m_fDestPosY;
						m_fVelocity	=	0.0f;

						m_pFXGroup	=	g_pFXManager->GetFXGroup( m_uiFXGroupID , m_tTargetGOBID  , m_bEnumType , false );

						Init();

						m_fSrcPosX	=	fOldPosX;
						m_fSrcPosY	=	fOldPosY;
						m_fVelocity	=	fVelocity;
					}
					else
					{
						pGOB = g_pGOBManager->Find( tGOBID );

						if( pGOB != NULL )
						{
							pGOB->SPGOBPerformMessage( SPGM_GETFXARCHIVE , (LPARAM)&pkFXArchive );
							//							pkFXArchive->SetTargetGOBID( tGOBID );
							pkFXArchive->InsertFXGroup( (SPID_FX_GROUP)m_uiFXGroupID );
						}
					}

					mIterNextFXInfo = m_mNextFXInfo.erase( mIterNextFXInfo );
				}
				else
					break;
			}
		}
	}
}

void SPFXGroupUnit::Render( float fTime , SP_FXM_RENDER_LAYER eRenderLayer )
{
	if( m_eState != FS_ENABLE )
		return;

	if( g_pVideo != NULL )
		g_pVideo->Flush();

	if( eRenderLayer == FXM_RENDER_LAYER_ALL )
	{
		m_eRenderLayerStart	=	FXM_RENDER_LAYER_OBJECT_FRONT;
		m_eRenderLayerEnd	=	(SP_FXM_RENDER_LAYER)(FXM_RENDER_LAYER_COUNT - 1);
	}
	else
	{
		m_eRenderLayerStart	=	eRenderLayer;
		m_eRenderLayerEnd	=	eRenderLayer;
	}

	if( m_eRenderLayerStart < FXM_RENDER_LAYER_OBJECT_FRONT )
		m_eRenderLayerStart	=	FXM_RENDER_LAYER_OBJECT_FRONT;

	if( m_eRenderLayerEnd >= FXM_RENDER_LAYER_COUNT )
		m_eRenderLayerEnd	=	(SP_FXM_RENDER_LAYER)(FXM_RENDER_LAYER_COUNT - 1);

	for( m_iIndex = (int)m_eRenderLayerStart ; m_iIndex <= (int)m_eRenderLayerEnd ; ++m_iIndex )
	{
		m_IterKey	=	m_vpFXUnit[ m_iIndex ].begin();
		m_IterEnd	=	m_vpFXUnit[ m_iIndex ].end();

		for( ; m_IterKey != m_IterEnd ; ++m_IterKey )
		{
			(*m_IterKey)->Render( fTime );
		}
	}
}

FXUNIT_STATE SPFXGroupUnit::GetState()
{
	return m_eState;
}

void SPFXGroupUnit::SetNextFXGroupIDOrder( STD_VECTOR_NEXTFXGROUPID& vNextFXGroupID )
{
	m_vNextFXGroupIDOrder	=	vNextFXGroupID;
}

STD_VECTOR_NEXTFXGROUPID& SPFXGroupUnit::GetNextFXGroupIDOrder()
{
	return	m_vNextFXGroupIDOrder;
}

void SPFXGroupUnit::SetNextFXInfo( STD_MULTIMAP_NEXTFXINFO& vNextFXInfo )
{
	m_mNextFXInfo	=	vNextFXInfo;
}

STD_MULTIMAP_NEXTFXINFO& SPFXGroupUnit::GetNextFXInfo()
{
	return	m_mNextFXInfo;
}

void SPFXGroupUnit::SetParentPos( float fPosX , float fPosY )
{
	m_fParentPosX	=	fPosX;
	m_fParentPosY	=	fPosY;
}

void SPFXGroupUnit::SetParentFacing( FACING eFacing )
{
	m_eParentFacing	=	eFacing;
}

void SPFXGroupUnit::SetFXGroup( UINT uiFXGroupID )
{
	m_uiFXGroupID	=	uiFXGroupID;
}

void SPFXGroupUnit::SetFXGroup( SPFXGroup* pFXGroup )
{
	m_pFXGroup	=	pFXGroup;
}

UINT SPFXGroupUnit::GetFXGroup()
{
	return m_uiFXGroupID;
}

void SPFXGroupUnit::SetLPARAM( LPARAM lParam )
{
	m_tlParam	=	lParam;
}

void SPFXGroupUnit::SetSrcPosX( float fSrcPosX )
{
	m_fSrcPosX	=	fSrcPosX;
}

void SPFXGroupUnit::SetSrcPosY( float fSrcPosY )
{
	m_fSrcPosY	=	fSrcPosY;
}

void SPFXGroupUnit::SetDestPosX( float fDestPosX )
{
	m_fDestPosX	=	fDestPosX;
}

void SPFXGroupUnit::SetDestPosY( float fDestPosY )
{
	m_fDestPosY	=	fDestPosY;
}

void SPFXGroupUnit::SetLoop( BOOL bLoop )
{
	m_bLoop	=	bLoop;
}

void SPFXGroupUnit::SetContinue( BOOL bContinue )
{
	m_bContinue	=	bContinue;
}

void SPFXGroupUnit::SetVelocity( float fVelocity )
{
	m_fVelocity	=	fVelocity;
}

void SPFXGroupUnit::SetLifeTime( float fLifeTime )
{
	m_fLifeTime	=	fLifeTime;
}

void SPFXGroupUnit::SetTargetGOBID( GU_ID tTargetGOBID )
{
	m_tTargetGOBID	=	tTargetGOBID;
}

void SPFXGroupUnit::SetEnumType( BOOL bEnumType )
{
	m_bEnumType	=	bEnumType;
}

void SPFXGroupUnit::SetFollowParent( int iFollowParent )
{
	m_iFollowParent	=	iFollowParent;
}

void SPFXGroupUnit::SetDealyTime( float fDealyTime )
{
	m_fDealyTime	=	fDealyTime;
}

float SPFXGroupUnit::GetDealyTime()
{
	return m_fDealyTime;
}

void SPFXGroupUnit::SetNotDel( BOOL bNotDel )
{
	m_bNotDel	=	bNotDel;
}

void SPFXGroupUnit::SetOffsetPos( float fPosX , float fPosY )
{
	m_fOffsetPosX	=	fPosX;
	m_fOffsetPosY	=	fPosY;
}

//////////////////////////////////////////////////////////////////////////
//
//	데미지에서 사용할 인자들
//

void SPFXGroupUnit::SetDamageFX( int iDamage , POINT ptGOBBox , FXUNIT_DAMAGE_TYPE eDamageType )
{
	m_iDamage		=	iDamage;
	m_ptGOBBox		=	ptGOBBox;
	m_eDamageType	=	eDamageType;
}

RECT SPFXGroupUnit::GetRenderRect()
{
	return	m_rcRender;
}

//////////////////////////////////////////////////////////////////////////
//
//	크리티컬에서 사용할 인자들
//

void SPFXGroupUnit::SetCritical( POINT ptGOBBox )
{
	m_ptGOBBox	=	ptGOBBox;
	m_bCritical	=	true;
}

BOOL SPFXGroupUnit::InsertFXModel( UINT uiFXModelID )
{
	SPFXModel* pModel = g_pFXManager->GetFXModel( uiFXModelID );

	if( pModel == NULL )
		return FALSE;

	SP_FX_UNIT			eFXUnitType		=	FXUNIT_NULL;
	SPID_FX_MODEL		eFXModelType	=	(SPID_FX_MODEL)pModel->GetMemberValue( MV_GET_FXMODEL_TYPE );
	float				fFXMVersion		=	*(float*)pModel->GetMemberValue( MV_GET_FXMODEL_VERSION );
	SP_FXM_RENDER_LAYER	eFXMRenderLayer	=	(SP_FXM_RENDER_LAYER)pModel->GetMemberValue( MV_GET_FX_RENDER_LAYER );

	if( eFXModelType == IDFXM_FRAMEANIMATION )
		eFXUnitType	=	FXUNIT_FRAMEANIMATION;
	else if( eFXModelType == IDFXM_PARTICLE )
	{
		if( fFXMVersion < 3.0f )
			return FALSE;

		eFXUnitType	=	FXUNIT_PARTICLE;
	}

	if( m_iDamage > -1 )
		eFXUnitType	=	FXUNIT_DAMAGE;

	if( m_bCritical == true )
		eFXUnitType	=	FXUNIT_CRITICAL;

	if( eFXMRenderLayer < FXM_RENDER_LAYER_OBJECT_FRONT )
		eFXMRenderLayer	=	FXM_RENDER_LAYER_OBJECT_FRONT;

	if( eFXMRenderLayer >= FXM_RENDER_LAYER_COUNT )
		eFXMRenderLayer	=	(SP_FXM_RENDER_LAYER)( FXM_RENDER_LAYER_COUNT - 1 );

//	SPFXUnit*	pUnit	=	g_pFXManager->CreateFXUnit( eFXUnitType );
	SPFXUnit*	pUnit	=	g_pFXManager->GetFXUnitBuffer( eFXUnitType );

	if( pUnit == NULL )
		return FALSE;

	pUnit->SetParentPos( m_fParentPosX , m_fParentPosY );
	pUnit->SetParentFacing( m_eParentFacing );
	pUnit->SetFXModel( pModel );
	pUnit->SetPosX( m_fSrcPosX );
	pUnit->SetPosY( m_fSrcPosY );
	pUnit->SetLParam( m_tlParam );
	pUnit->SetGOBID( m_tTargetGOBID );

	if( m_iDamage > -1 )
	{
		SPFXDamageUnit*	pDamageUnit	=	(SPFXDamageUnit*)pUnit;

		if( pDamageUnit != NULL )
		{
			pDamageUnit->SetDamageType( m_eDamageType );
			pDamageUnit->SetGOBBox( m_ptGOBBox );
			pDamageUnit->SetDamage( m_strNumber.c_str() , m_istrLength );
		}
	}

	if( m_bCritical == true )
	{
		SPFXCriticalUnit*	pCriticalUnit =	(SPFXCriticalUnit*)pUnit;

		if( pCriticalUnit != NULL )
			pCriticalUnit->SetGOBBox( m_ptGOBBox );
	}

	pUnit->Init();

	//if( m_iDamage > -1 )
	//{
	//	SPFXDamageUnit*	pDamageUnit	=	(SPFXDamageUnit*)pUnit;
	//	
	//	if( pDamageUnit != NULL )
	//		AddDamageRenderRect( pDamageUnit->GetRenderRect() );
	//}

	if( m_bLoop == TRUE )
		pUnit->SetLoop( m_bLoop );

	if( m_iFollowParent != 0 )
		pUnit->SetFollow( m_iFollowParent );

	if( m_bContinue == TRUE )
	{
		pUnit->SetContinue( m_bContinue );
		pUnit->SetFollowParent( TRUE );
	}

	if( m_fDestPosX != 0.0f )		pUnit->SetDestPosX( m_fDestPosX );
	if( m_fDestPosY != 0.0f )		pUnit->SetDestPosY( m_fDestPosY );
	if( m_fVelocity != 0.0f )		pUnit->SetVelocity( m_fVelocity );
	if( m_fLifeTime != 0.0f )		pUnit->SetLifeTime( m_fLifeTime );

	m_vpFXUnit[ eFXMRenderLayer ].push_back( pUnit );

	return TRUE;
}

void SPFXGroupUnit::SetDamageString( const char strNumber , int istrLength )
{
	m_strNumber		=	strNumber;
	m_istrLength	=	istrLength;
}

//#include "SPDebug.h"
/*
BOOL SPFXGroupUnit::ProcessDealy( float fTime )
{
	//if( m_iDamage > -1 && m_fDealyTime != 10.0f )
	//	DXUTOutputDebugString( "-------------------> m_fDealyTime : %f , fTime : %f" , m_fDealyTime , fTime );

	if( m_bDealyInit == FALSE && m_iDamage > -1 && m_fDealyTime != 10.0f )
	{
		m_bDealyInit	=	TRUE;
		m_fDealyTime	+=	fTime;
	}

	if( m_fDealyTime <= 0.0f )
	{
		m_fDealyTime	=	0.0f;

		//if( m_iDamage > -1 )
		//	DXUTOutputDebugString( "-------------------> FALSE \n" );

		return FALSE;
	}

	m_fDealyTime	-=	fTime;

	//if( m_iDamage > -1 )
	//	DXUTOutputDebugString( "-------------------> TRUE \n" );

	return TRUE;
}

void SPFXGroupUnit::AddDamageRenderRect( RECT rcRender )
{
	if( m_rcRender.left		>	rcRender.left )
		m_rcRender.left		=	rcRender.left;

	if( m_rcRender.top		>	rcRender.top )
		m_rcRender.top		=	rcRender.top;

	if( m_rcRender.right	<	rcRender.right )
		m_rcRender.right	=	rcRender.right;

	if( m_rcRender.bottom	<	rcRender.bottom )
		m_rcRender.bottom	=	rcRender.bottom;
}
*/