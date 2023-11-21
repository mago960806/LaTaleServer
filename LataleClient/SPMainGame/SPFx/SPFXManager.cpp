
#include <algorithm>
#include <functional>

#include "SPCommon.h"
#include "SPUtil.h"
#include "SPResourceDEF.h"
#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPGOBManager.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXModelFrameAnimation.h"
#include "SPFXModelParticle.h"
#include "SPFXFrameAnimationUnit.h"
#include "SPFXDamageUnit.h"
#include "SPFXCriticalUnit.h"
#include "SPFXParticleUnit.h"
#include "SPFXParticleObject.h"
#include "SPManager.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPFXModelBaseLoader.h"
#include "SPFXGroupLoader.h"
#include "SPFXFactory.h"
#include "SPFXModelLoader.h"
#include "SPFXLoader.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include <time.h>

#include "SPFXManager.h"


using namespace std;
using namespace stdext;

SPFXManager* g_pFXManager = NULL;


SPFXManager::SPFXManager()
: SPManager()
{
	SetMgrID(MGR_ID_FX);
	g_pFXManager = this;

	time_t	tms;
	time(&tms);
	srand( (unsigned int)tms );
}

SPFXManager::~SPFXManager()
{
	Clean();
}

bool SPFXManager::Init()
{
	SPManager::Init();
	
	m_FXUnitFactory.RegisterClass<SPFXFrameAnimationUnit>( FXUNIT_FRAMEANIMATION );
	m_FXUnitFactory.RegisterClass<SPFXDamageUnit>( FXUNIT_DAMAGE );
	m_FXUnitFactory.RegisterClass<SPFXCriticalUnit>( FXUNIT_CRITICAL );
	m_FXUnitFactory.RegisterClass<SPFXParticleUnit>( FXUNIT_PARTICLE );
	
	m_iFXGroupUnitCountMax							=	0;

	m_iFXUnitCountMax[ FXUNIT_FRAMEANIMATION	]	=	0;
	m_iFXUnitCountMax[ FXUNIT_DAMAGE			]	=	0;
	m_iFXUnitCountMax[ FXUNIT_CRITICAL			]	=	0;
	m_iFXUnitCountMax[ FXUNIT_PARTICLE			]	=	0;

	m_iFXParticleObjectCountMax						=	0;

#if defined(_DEBUG)
	DXUTOutputDebugString("SPFXManager::Init()\n");
#endif

#if !defined(_JUST_RUN)
	//TODO: FXModel List Create
	if( LoadFXList( RES_FNAME_LDT_FXLIST ) == false )
	{
		LoadFXModel( RES_FNAME_LDT_FXUNIT	, m_mpFXLocalModel	, RES_FNAME_FX );
		LoadFXGroup( RES_FNAME_LDT_FXGROUP	, m_hmFXGroup		, RES_FNAME_FX );
	}

	LoadMotionFX("DATA/LDT/MOTIONFX_TYPE.LDT"	, m_hmMotionFX_PC			, MOTION_FX_PC );
	LoadMotionFX("DATA/LDT/MOTIONFX_TYPE2.LDT"	, m_hmMotionFX_PC_NotDel	, MOTION_FX_PC );
	LoadMotionFX("DATA/LDT/MOB_MOTIONFXTYPE.LDT", m_hmMotionFX_MOB			, MOTION_FX_MOB );
	LoadMotionFX("DATA/LDT/NPC_MOTIONFXTYPE.LDT", m_hmMotionFX_NPC			, MOTION_FX_NPC );

	CreateFXGroupUnitBuffer( 100 );

	CreateFXUnitBuffer( FXUNIT_FRAMEANIMATION	, 100 );
	CreateFXUnitBuffer( FXUNIT_DAMAGE			, 100 );
	CreateFXUnitBuffer( FXUNIT_CRITICAL			, 100 );
	CreateFXUnitBuffer( FXUNIT_PARTICLE			, 100 );

	CreateFXParticleObjectBuffer( 1000 );
#endif

	return true;
}

void SPFXManager::Clean()
{
	SPManager::Clean();

	std::map< UINT , SPFXModel*>::iterator	mIter	=	m_mpFXLocalModel.begin();
	for(; mIter != m_mpFXLocalModel.end(); ++mIter) {
		(*mIter).second->Destroy();
		SAFE_DELETE( (*mIter).second );
	}
	m_mpFXLocalModel.clear();

	stdext::hash_map<SPID_FX_GROUP, SPFXGroup*>::iterator hmIter = m_hmFXGroup.begin();
	for(; hmIter != m_hmFXGroup.end(); ++hmIter) {
		SAFE_DELETE((*hmIter).second);
	}
	m_hmFXGroup.clear();

	m_hmMotionFX_PC.clear();
	m_hmMotionFX_PC_NotDel.clear();
	m_hmMotionFX_MOB.clear();
	m_hmMotionFX_NPC.clear();

	ClearExtraFX();

	DeleteFXGroupUnitBuffer( 0 );

	DeleteFXUnitBuffer( FXUNIT_FRAMEANIMATION	, 0 );
	DeleteFXUnitBuffer( FXUNIT_DAMAGE			, 0 );
	DeleteFXUnitBuffer( FXUNIT_CRITICAL			, 0 );
	DeleteFXUnitBuffer( FXUNIT_PARTICLE			, 0 );

	DeleteFXParticleObjectBuffer( 0 );
}

void SPFXManager::Process(float fTime)
{
//	SPManager::Process(fTime);
}
void SPFXManager::Render(float fTime)
{
//	SPManager::Render(fTime);
}

int SPFXManager::SPPerformMessage(UINT msg, WPARAM wparam , LPARAM lParam )
{
	return -1;
}

SPFXModel* SPFXManager::GetFXModel( UINT uiFXID )
{
	SPFXModel*	pFXModel	=	NULL;
	std::map<UINT,SPFXModel*>::iterator	mIter	=	m_mpFXLocalModel.find( uiFXID );

	if( mIter != m_mpFXLocalModel.end() && ( uiFXID == (*mIter).first ) )
	{
		pFXModel	=	(*mIter).second;
	}

	if( pFXModel == NULL )
	{
		mIter	=	m_mpExtraFXLocalModel.find( uiFXID );

		if( mIter != m_mpExtraFXLocalModel.end() && ( uiFXID == (*mIter).first ) )
		{
			pFXModel	=	(*mIter).second;
		}
	}

	return pFXModel;
}

SPFXGroup* SPFXManager::GetFXGroup( SPID_FX_GROUP eFXGroupID )
{	
	SPFXGroup* pFXGroup = NULL;
	stdext::hash_map<SPID_FX_GROUP, SPFXGroup*>::iterator Iter = m_hmFXGroup.find( eFXGroupID );
	if(Iter != m_hmFXGroup.end() && (eFXGroupID == (*Iter).first) )
	{
		pFXGroup = (*Iter).second;
	}

	if( pFXGroup == NULL )
	{
		Iter = m_hmExtraFXGroup.find( eFXGroupID );

		if(Iter != m_hmExtraFXGroup.end() && (eFXGroupID == (*Iter).first) )
		{
			pFXGroup = (*Iter).second;
		}
	}

	return pFXGroup;
}

SPFXGroup* SPFXManager::GetFXGroup( unsigned int uiAniIndex , CLASS_ID eClassID )
{
	SPFXGroup* pFXGroup = NULL;

	if( eClassID  < CLASS_ID_GOB_FIRST || eClassID > CLASS_ID_GOB_LAST )
		return NULL;

	unsigned int	uiClassID	=	static_cast<unsigned int>( eClassID );

	if( eClassID == CLASS_ID_AVATAR )
	{
		pFXGroup	=	GetPCFXGroup( uiAniIndex );
	}
	else if( eClassID >= CLASS_ID_MOB_FIRST && eClassID <= CLASS_ID_MOB_LAST )
	{
		pFXGroup	=	GetMOBFXGroup( uiAniIndex , uiClassID );
	}
	else if( eClassID >= CLASS_ID_MAPEVENT_FIRST && eClassID <= CLASS_ID_MAPEVENT_LAST )
	{
		pFXGroup	=	GetNPCFXGroup( uiAniIndex , uiClassID );
	}
	else if( eClassID == CLASS_ID_LOOTING_BAG )
	{
		pFXGroup	=	GetMOBFXGroup( uiAniIndex , uiClassID );
	}

	return pFXGroup;
}

SPFXGroup* SPFXManager::GetFXGroupNotDel( unsigned int uiAniIndex , CLASS_ID eClassID )
{
	SPFXGroup* pFXGroup = NULL;

	if( eClassID  < CLASS_ID_GOB_FIRST || eClassID > CLASS_ID_GOB_LAST )
		return NULL;

	unsigned int	uiClassID	=	static_cast<unsigned int>( eClassID );

	if( eClassID == CLASS_ID_AVATAR )
	{
		pFXGroup	=	GetPCFXGroupNotDel( uiAniIndex );
	}
	else if( eClassID >= CLASS_ID_MOB_FIRST && eClassID <= CLASS_ID_MOB_LAST )
	{
//		pFXGroup	=	GetMOBFXGroup( uiAniIndex , uiClassID );
	}
	else if( eClassID >= CLASS_ID_MAPEVENT_FIRST && eClassID <= CLASS_ID_MAPEVENT_LAST )
	{
//		pFXGroup	=	GetNPCFXGroup( uiAniIndex , uiClassID );
	}
	else if( eClassID == CLASS_ID_LOOTING_BAG )
	{
//		pFXGroup	=	GetMOBFXGroup( uiAniIndex , uiClassID );
	}

	return pFXGroup;
}

bool SPFXManager::LoadExtraFXLDT( const char* pszFXModel , const char* pszFXGroup , const char* pszFXPath )
{
	bool	bResult	=	true;

	bResult	&=	LoadFXGroup( pszFXGroup , m_hmExtraFXGroup , pszFXPath );
	bResult	&=	LoadFXModel( pszFXModel , m_mpExtraFXLocalModel , pszFXPath );

	return bResult;
}

void SPFXManager::ClearExtraFX()
{
	std::map< UINT , SPFXModel*>::iterator	mIter	=	m_mpExtraFXLocalModel.begin();
	for(; mIter != m_mpExtraFXLocalModel.end(); ++mIter) {
		(*mIter).second->Destroy();
		SAFE_DELETE( (*mIter).second );
	}
	m_mpExtraFXLocalModel.clear();

	stdext::hash_map<SPID_FX_GROUP, SPFXGroup*>::iterator hmIter = m_hmExtraFXGroup.begin();
	for(; hmIter != m_hmExtraFXGroup.end(); ++hmIter) {
		SAFE_DELETE((*hmIter).second);
	}
	m_hmExtraFXGroup.clear();
}

SPFXUnit* SPFXManager::CreateFXUnit( SP_FX_UNIT eFXUnitType )
{
	return m_FXUnitFactory.CreateClass( eFXUnitType );
}

SPFXGroup* SPFXManager::GetFXGroup( UINT uiFXGroupID , GU_ID tID , bool bEnumType , bool bNotDel )
{
	SPFXGroup*		pFXGroup	=	NULL;

	if( bEnumType == true )
		pFXGroup	=	GetFXGroup( (SPID_FX_GROUP)uiFXGroupID );
	else
	{
		if( tID > 0 )
		{
			if( bNotDel == FALSE )
				pFXGroup	=	GetFXGroup( uiFXGroupID , GetGUID2ClassID( tID ) );
			else
				pFXGroup	=	GetFXGroupNotDel( uiFXGroupID , GetGUID2ClassID( tID ) );
		}
	}

	return pFXGroup;
}

//////////////////////////////////////////////////////////////////////////
//
//	FXGroupUnitBuffer 인터페이스
//

bool SPFXManager::CreateFXGroupUnitBuffer( int iCount )
{
	bool			bResult			=	true;
	SPFXGroupUnit*	pFXGroupUnit	=	NULL;

	if( m_iFXGroupUnitCountMax > 0 )
	{
		int	iFXGroupUnitCount	=	(int)m_vpFXGroupUnit_Pool.size();

		if( iFXGroupUnitCount + iCount > m_iFXGroupUnitCountMax )
			iCount	=	m_iFXGroupUnitCountMax - iFXGroupUnitCount;

		if( iCount <= 0 )
		{
			iCount	=	0;
			bResult	=	false;
		}
	}

	for( int i = 0 ; i < iCount ; ++i )
	{
		pFXGroupUnit	=	new SPFXGroupUnit;

		if( pFXGroupUnit == NULL )
		{
			bResult	=	false;
			continue;
		}

		pFXGroupUnit->Destory();

		m_vpFXGroupUnit_Pool.push_back( pFXGroupUnit );
	}

	return bResult;
}

bool SPFXManager::DeleteFXGroupUnitBuffer( int iCount )
{
	bool	bResult			=	true;
	bool	bRealDelete		=	false;

	if( iCount <= 0 )
	{
		iCount		=	(int)m_vpFXGroupUnit_Pool.size();
		bRealDelete	=	true;
	}

	int	iDeleteCount	=	0;

	SPFXGroupUnit*						pFXGroupUnit	=	NULL;
	STD_VECTOR_FX_GROUP_UNIT::iterator	Iter			=	m_vpFXGroupUnit_Pool.begin();
	bool								bDelete			=	false;

	while( Iter != m_vpFXGroupUnit_Pool.end() )
	{
		if( iDeleteCount >= iCount )
			break;

		bDelete			=	false;
		pFXGroupUnit	=	(SPFXGroupUnit*)(*Iter);

		if( pFXGroupUnit != NULL )
		{
			if( pFXGroupUnit->GetState() == FS_RELEASE || m_iFXGroupUnitCountMax == 0 || bRealDelete == true )
			{
				if( iDeleteCount < iCount || bRealDelete == true )
				{
					Iter	=	m_vpFXGroupUnit_Pool.erase( Iter );
					SAFE_DELETE( pFXGroupUnit );
					++iDeleteCount;
					bDelete	=	true;
				}
			}
		}

		if( bDelete == false )
			++Iter;
	}

	if( iDeleteCount < iCount )
		bResult	=	false;

	return bResult;
}

SPFXGroupUnit* SPFXManager::GetFXGroupUnitBuffer( bool bCreate /* = true  */ )
{
	SPFXGroupUnit*						pFXGroupUnit	=	NULL;
	STD_VECTOR_FX_GROUP_UNIT::iterator	Iter			=	m_vpFXGroupUnit_Pool.begin();

	while( Iter != m_vpFXGroupUnit_Pool.end() )
	{
		pFXGroupUnit	=	(SPFXGroupUnit*)(*Iter);

		if( pFXGroupUnit != NULL )
		{
			if( pFXGroupUnit->GetState() != FS_RELEASE )
				pFXGroupUnit	=	NULL;
			else
				break;
		}

		++Iter;
	}

	if( pFXGroupUnit == NULL && bCreate == true )
	{
		int	iFXGroupUnitCount	=	(int)m_vpFXGroupUnit_Pool.size();

		if( iFXGroupUnitCount < m_iFXGroupUnitCountMax || m_iFXGroupUnitCountMax == 0 )
		{
			CreateFXGroupUnitBuffer( 10 );
			pFXGroupUnit	=	GetFXGroupUnitBuffer( false );
		}
	}

	if( pFXGroupUnit != NULL )
		pFXGroupUnit->Destory();

	return pFXGroupUnit;
}

//////////////////////////////////////////////////////////////////////////

bool SPFXManager::CreateFXUnitBuffer( SP_FX_UNIT eFXUnitType , int iCount )
{
	bool			bResult			=	true;
	SPFXUnit*		pFXUnit			=	NULL;

	if( m_iFXUnitCountMax[ eFXUnitType ] > 0 )
	{
		int	iFXUnitCount	=	(int)m_vpFXUnit_Pool[ eFXUnitType ].size();

		if( iFXUnitCount + iCount > m_iFXUnitCountMax[ eFXUnitType ] )
			iCount	=	m_iFXUnitCountMax[ eFXUnitType ] - iFXUnitCount;

		if( iCount <= 0 )
		{
			iCount	=	0;
			bResult	=	false;
		}
	}

	for( int i = 0 ; i < iCount ; ++i )
	{
		pFXUnit	=	CreateFXUnit( eFXUnitType );

		if( pFXUnit == NULL )
		{
			bResult	=	false;
			continue;
		}

		pFXUnit->Destory();

		m_vpFXUnit_Pool[ eFXUnitType ].push_back( pFXUnit );
	}

	return bResult;
}

bool SPFXManager::DeleteFXUnitBuffer( SP_FX_UNIT eFXUnitType , int iCount )
{
	bool	bResult			=	true;
	bool	bRealDelete		=	false;

	if( iCount <= 0 )
	{
		iCount	=	(int)m_vpFXUnit_Pool[ eFXUnitType ].size();
		bRealDelete	=	true;
	}

	int	iDeleteCount	=	0;

	SPFXUnit*						pFXUnit			=	NULL;
	STD_VECTOR_SPFXUNIT::iterator	Iter			=	m_vpFXUnit_Pool[ eFXUnitType ].begin();
	bool							bDelete			=	false;

	while( Iter != m_vpFXUnit_Pool[ eFXUnitType ].end() )
	{
		if( iDeleteCount >= iCount )
			break;

		bDelete			=	false;
		pFXUnit	=	(SPFXUnit*)(*Iter);

		if( pFXUnit != NULL )
		{
			if( pFXUnit->GetState() == FS_RELEASE || m_iFXUnitCountMax[ eFXUnitType ] == 0 || bRealDelete == true )
			{
				if( iDeleteCount < iCount || bRealDelete == true )
				{
					Iter	=	m_vpFXUnit_Pool[ eFXUnitType ].erase( Iter );
					SAFE_DELETE( pFXUnit );
					++iDeleteCount;
					bDelete	=	true;
				}
			}
		}

		if( bDelete == false )
			++Iter;
	}

	if( iDeleteCount < iCount )
		bResult	=	false;

	return bResult;
}

SPFXUnit* SPFXManager::GetFXUnitBuffer( SP_FX_UNIT eFXUnitType , bool bCreate /* = true  */ )
{
	SPFXUnit*						pFXUnit	=	NULL;
	STD_VECTOR_SPFXUNIT::iterator	Iter	=	m_vpFXUnit_Pool[ eFXUnitType ].begin();

	while( Iter != m_vpFXUnit_Pool[ eFXUnitType ].end() )
	{
		pFXUnit	=	(SPFXUnit*)(*Iter);

		if( pFXUnit != NULL )
		{
			if( pFXUnit->GetState() != FS_RELEASE )
				pFXUnit	=	NULL;
			else
				break;
		}

		++Iter;
	}

	if( pFXUnit == NULL && bCreate == true )
	{
		int	iFXUnitCount	=	(int)m_vpFXUnit_Pool[ eFXUnitType ].size();

		if( iFXUnitCount < m_iFXUnitCountMax[ eFXUnitType ] || m_iFXUnitCountMax[ eFXUnitType ] == 0 )
		{
			CreateFXUnitBuffer( eFXUnitType , 10 );
			pFXUnit	=	GetFXUnitBuffer( eFXUnitType , false );
		}
	}

	if( pFXUnit != NULL )
		pFXUnit->Destory();

	return pFXUnit;
}

//////////////////////////////////////////////////////////////////////////

bool SPFXManager::CreateFXParticleObjectBuffer( int iCount )
{
	bool				bResult				=	true;
	SPFXParticleObject*	pFXParticleObject	=	NULL;

	if( m_iFXParticleObjectCountMax > 0 )
	{
		int	iFXParticleObjectCount	=	(int)m_vpFXParticleObject_Pool.size();

		if( iFXParticleObjectCount + iCount > m_iFXParticleObjectCountMax )
			iCount	=	m_iFXParticleObjectCountMax - iFXParticleObjectCount;

		if( iCount <= 0 )
		{
			iCount	=	0;
			bResult	=	false;
		}
	}

	for( int i = 0 ; i < iCount ; ++i )
	{
		pFXParticleObject	=	new SPFXParticleObject;

		if( pFXParticleObject == NULL )
		{
			bResult	=	false;
			continue;
		}

		pFXParticleObject->Destory();

		m_vpFXParticleObject_Pool.push_back( pFXParticleObject );
	}

	return bResult;
}

bool SPFXManager::DeleteFXParticleObjectBuffer( int iCount )
{
	bool	bResult			=	true;
	bool	bRealDelete		=	false;

	if( iCount <= 0 )
	{
		iCount		=	(int)m_vpFXParticleObject_Pool.size();
		bRealDelete	=	true;
	}

	int	iDeleteCount	=	0;

	SPFXParticleObject*						pFXParticleObject	=	NULL;
	STD_VECTOR_SPFXPARTICLEOBJECT::iterator	Iter				=	m_vpFXParticleObject_Pool.begin();
	bool									bDelete			=	false;

	while( Iter != m_vpFXParticleObject_Pool.end() )
	{
		if( iDeleteCount >= iCount )
			break;

		bDelete				=	false;
		pFXParticleObject	=	(SPFXParticleObject*)(*Iter);

		if( pFXParticleObject != NULL )
		{
			if( pFXParticleObject->GetState() == FS_RELEASE || m_iFXParticleObjectCountMax == 0 || bRealDelete == true )
			{
				if( iDeleteCount < iCount || bRealDelete == true )
				{
					Iter	=	m_vpFXParticleObject_Pool.erase( Iter );
					SAFE_DELETE( pFXParticleObject );
					++iDeleteCount;
					bDelete	=	true;
				}
			}
		}

		if( bDelete == false )
			++Iter;
	}

	if( iDeleteCount < iCount )
		bResult	=	false;

	return bResult;
}

SPFXParticleObject* SPFXManager::GetFXParticleObjectBuffer( bool bCreate /* = true  */ )
{
	SPFXParticleObject*						pFXParticleObject	=	NULL;
	STD_VECTOR_SPFXPARTICLEOBJECT::iterator	Iter				=	m_vpFXParticleObject_Pool.begin();

	while( Iter != m_vpFXParticleObject_Pool.end() )
	{
		pFXParticleObject	=	(SPFXParticleObject*)(*Iter);

		if( pFXParticleObject != NULL )
		{
			if( pFXParticleObject->GetState() != FS_RELEASE )
				pFXParticleObject	=	NULL;
			else
				break;
		}

		++Iter;
	}

	if( pFXParticleObject == NULL && bCreate == true )
	{
		int	iFXParticleObjectCount	=	(int)m_vpFXParticleObject_Pool.size();

		if( iFXParticleObjectCount < m_iFXParticleObjectCountMax || m_iFXGroupUnitCountMax == 0 )
		{
			CreateFXParticleObjectBuffer( 10 );
			pFXParticleObject	=	GetFXParticleObjectBuffer( false );
		}
	}

	if( pFXParticleObject != NULL )
		pFXParticleObject->Destory();

	return pFXParticleObject;
}

//////////////////////////////////////////////////////////////////////////

bool SPFXManager::LoadFXList( const char* pszFXList )
{
	if( pszFXList == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->FX LDT List File Name String NULL \n" );
#endif
		return false;
	}

	if( strlen( pszFXList ) <= 0 )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->FX LDT List File String Zero \n" );
#endif
		return false;
	}

	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( pszFXList , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->FX LDT List Load Fail [%s] \n" , pszFXList );
#endif
		return false;
	}

	LDT_Field	ldtFieldFXFileName , ldtFieldFXType;
	long		lFieldNumFXFileName	=	pLDTFile->GetFieldNum( "_FileName" );
	long		lFieldNumFXType		=	pLDTFile->GetFieldNum( "_Type" );

	int			iRecordCount		=	pLDTFile->GetItemCount();
	int			iItemID;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetField( iItemID , lFieldNumFXFileName	,	ldtFieldFXFileName	);
		pLDTFile->GetField( iItemID , lFieldNumFXType		,	ldtFieldFXType		);

		if ( ldtFieldFXFileName.uData.pValue == NULL || (int)strlen( ldtFieldFXFileName.uData.pValue ) <= 0 )
			continue;

		if( ldtFieldFXType.uData.lValue == 0 )
		{
			LoadFXModel( ldtFieldFXFileName.uData.pValue , m_mpFXLocalModel , RES_FNAME_FX );
		}
		else if( ldtFieldFXType.uData.lValue == 1 )
		{
			LoadFXGroup( ldtFieldFXFileName.uData.pValue , m_hmFXGroup , RES_FNAME_FX );
		}
		else
		{
#ifdef _DEBUG
			DXUTOutputDebugString( "----------------------------------->FX List Type Error ItemID [%d] FileName [%s] Type [%d] \n" , iItemID , ldtFieldFXFileName.uData.pValue , ldtFieldFXType.uData.lValue );
#endif
			continue;
		}
	}

	SAFE_RELEASE(pLDTFile);

	return true;
}

bool SPFXManager::LoadFXGroup( const char* pszFXGroup , stdext::hash_map<SPID_FX_GROUP, SPFXGroup*>& hmFXGroup , const char* pszFXPath )
{
	if( pszFXGroup == NULL || pszFXPath == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->FX LDT Group File String NULL \n" );
#endif
		return false;
	}

	if( strlen( pszFXGroup ) <= 0 || strlen( pszFXPath ) <= 0 )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->FX LDT Group File String Zero \n" );
#endif
		return false;
	}

	SPLDTFile*	pLDTFile	=	NULL;
	SPFXLoader	FXLoader;

	//
	// AJJIYA [5/4/2005]
	// FX Group LDT LOAD
	g_pResourceManager->GetLDTFromFileName( pszFXGroup , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->FX LDT Group Load Fail [%s] \n" , pszFXGroup );
#endif
		return false;
	}

	SPFXGroup*	pFXGroup		=	NULL;
	SPFXGroup*	pSameFXGroup	=	NULL;

	std::string		strDefaultPath; // = RES_WORK_RESOURCE_PATH;
	strDefaultPath	=	pszFXPath;

	LDT_Field	ldtFieldFXGFileName;
	long		lFieldNumFXG	=	pLDTFile->GetFieldNum( "_FxGroup" );

	int		iRecordCount	= pLDTFile->GetItemCount();
	int		iItemID;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetField( iItemID , lFieldNumFXG ,	ldtFieldFXGFileName );

		if ( ldtFieldFXGFileName.uData.pValue == NULL || (int)strlen( ldtFieldFXGFileName.uData.pValue ) <= 0 )
			continue;

		pFXGroup = FXLoader.LoadGroupFile( strDefaultPath + ldtFieldFXGFileName.uData.pValue );

		if( pFXGroup == NULL )
		{
#ifdef _DEBUG
			DXUTOutputDebugString( "----------------------------------->FX Group Load Fail [%s] \n" , ldtFieldFXGFileName.uData.pValue );
#endif
			continue;
		}

		//
		// AJJIYA [5/20/2005]
		// 혹시 중복으로 로딩되었는가를 확인한다.
		// LDT에서 같은 itemID가 하위에 공백으로 있어도 상위의 값으로 대체된다.
		// 그래서 중복 로딩이 발생할수 있어 메모리 릭이 발생된다.
		pSameFXGroup	=	NULL;
		pSameFXGroup	=	GetFXGroup( pFXGroup->FXGroupID );

		if( pSameFXGroup != NULL )
		{
#ifdef _DEBUG
			DXUTOutputDebugString( "----------------------------------->FX Group ID Overlapping [%s] \n" , ldtFieldFXGFileName.uData.pValue );
#endif
			SAFE_DELETE( pFXGroup );
			continue;
		}

		hmFXGroup.insert ( stdext::hash_map<SPID_FX_GROUP, SPFXGroup*>::value_type ( pFXGroup->FXGroupID , pFXGroup ) );

#ifdef _DEBUG
		if( iItemID != pFXGroup->FXGroupID )
		{
			DXUTOutputDebugString( "----------------------------------->FX Group ID Mismatch [%s] \n" , ldtFieldFXGFileName.uData.pValue );
		}
#endif

		pFXGroup = NULL;
	}

	SAFE_RELEASE(pLDTFile);

	//	FX Group LDT LOAD END
	
	return true;
}

bool SPFXManager::LoadFXModel( const char* pszFXModel , std::map<UINT,SPFXModel*>& mpFXLocalModel , const char* pszFXPath )
{
	if( pszFXModel == NULL || pszFXPath == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->FX LDT Model File String NULL \n" );
#endif
		return false;
	}

	if( strlen( pszFXModel ) <= 0 || strlen( pszFXPath ) <= 0 )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->FX LDT Model File String Zero \n" );
#endif
		return false;
	}

	SPLDTFile*	pLDTFile	=	NULL;
	SPFXLoader	FXLoader;

	//
	// AJJIYA [5/4/2005]
	// FX Model LDT LOAD
	g_pResourceManager->GetLDTFromFileName( pszFXModel , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->FX LDT Model Load Fail [%s] \n" , pszFXModel );
#endif
		return false;
	}

	SPFXModel*	pFXModel		=	NULL;
	SPFXModel*	pSameFXModel	=	NULL;

	std::string		strDefaultPath; // = RES_WORK_RESOURCE_PATH;
	strDefaultPath	=	pszFXPath;

	LDT_Field ldtFieldFXMFileName;
	long		lFieldNumFXM	=	pLDTFile->GetFieldNum( "_FxUnit" );

	int		iRecordCount	= pLDTFile->GetItemCount();
	int		iItemID;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetField( iItemID , lFieldNumFXM , ldtFieldFXMFileName );

		if ( ldtFieldFXMFileName.uData.pValue == NULL || (int)strlen( ldtFieldFXMFileName.uData.pValue ) <= 0 )
			continue;

		pFXModel = FXLoader.LoadModelFile( strDefaultPath + ldtFieldFXMFileName.uData.pValue );

		if( pFXModel == NULL )
		{
#ifdef _DEBUG
			DXUTOutputDebugString( "----------------------------------->FX Model Load Fail [%s] \n" , ldtFieldFXMFileName.uData.pValue );
#endif
			continue;
		}

		//
		// AJJIYA [5/20/2005]
		// 혹시 중복으로 로딩되었는가를 확인한다.
		// LDT에서 같은 itemID가 하위에 공백으로 있어도 상위의 값으로 대체된다.
		// 그래서 중복 로딩이 발생할수 있어 메모리 릭이 발생된다.
		pSameFXModel	=	NULL;
		pSameFXModel	=	GetFXModel( pFXModel->GetFXModelID() );

		if( pSameFXModel != NULL )
		{
#ifdef _DEBUG
			DXUTOutputDebugString( "----------------------------------->FX Model ID Overlapping [%s] \n" , ldtFieldFXMFileName.uData.pValue );
#endif
			SAFE_DELETE( pFXModel );
			continue;
		}


		// AJJIYA [8/29/2005]
		// LifeTime Check
#ifdef _DEBUG
		float	fLifeTime		= *(float*)pFXModel->GetMemberValue( MV_GET_FXLIFETIME );
		int		iFollowParent	= (int)pFXModel->GetMemberValue( MV_GET_FXFOLLOW_PARENT );

		if( fLifeTime >= 25.0f && ( iFollowParent & FX_FOLLOW_LOOP ) != FX_FOLLOW_LOOP )
			DXUTOutputDebugString( "----------------------------------->FX Model [%s] : LifeTime : [%f] \n" , ldtFieldFXMFileName.uData.pValue , fLifeTime );

		SPTexture**	ppTexture	=	(SPTexture**)pFXModel->GetMemberValue( MV_GETINITIMAGE );

		if( *ppTexture == NULL )
			DXUTOutputDebugString( "----------------------------------->FX Model [%s] : Texture Load Fail : [%s%s] \n" , ldtFieldFXMFileName.uData.pValue , FXLoader.GetFXMImgPath() , FXLoader.GetFXMImgName() );

		//SPID_FX_MODEL	eFXModelType	=	(SPID_FX_MODEL)pFXModel->GetMemberValue( MV_GET_FXMODEL_TYPE );

		//if( eFXModelType == IDFXM_PARTICLE )
		//{
		//	int	iMaxParticleFrameCount	=	(int)pFXModel->GetMemberValue( MV_GET_MAXFRAME );
		//	int	iParicleCount			=	0;

		//	SPFXParticleFrameData*		pFrameData	=	NULL;

		//	for( int iParicleFrame = 0 ; iParicleFrame < iMaxParticleFrameCount ; ++iParicleFrame )
		//	{
		//		pFrameData		=	(SPFXParticleFrameData*)pFXModel->GetMemberValue( MV_GET_FRAMEDATA, iParicleFrame );
		//		iParicleCount	+=	pFrameData->fFrameTime / pFrameData->fCreateTime * pFrameData->iCreateCount;
		//	}

		//	if( iParicleCount > 50 )
		//		DXUTOutputDebugString( "----------------------------------->FX Model [%s] : Particle Count : [%d] \n" , ldtFieldFXMFileName.uData.pValue , iParicleCount );
		//}

		if( iItemID != (int)pFXModel->GetFXModelID() )
		{
			DXUTOutputDebugString( "----------------------------------->FX Model ID Mismatch [%s] \n" , ldtFieldFXMFileName.uData.pValue );
		}
#endif

		mpFXLocalModel.insert( std::map<UINT, SPFXModel*>::value_type( pFXModel->GetFXModelID() , pFXModel ) );

		pFXModel = NULL;
	}

	SAFE_RELEASE(pLDTFile);

	// FX Model LDT LOAD END

	return true;
}

bool SPFXManager::LoadMotionFX( const char* pStrFileName , stdext::hash_map<unsigned int, unsigned int>& hmMotionFX , MOTION_FX_TYPE eMotionType )
{
	//
	// AJJIYA [5/16/2005]
	// PC용
	// MotionFX LDT LOAD START

	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( pStrFileName , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		OutputDebugStr("----------------------------------->PC Motion FX Load Fail\n");
#endif
		return false;
	}

	hmMotionFX.clear();

	int				iItemID;
	LDT_Field		ldtFieldFXGroup;
	unsigned int	uiAniIndex;

	int iRecordCount	= pLDTFile->GetItemCount();
	int iFieldCount		= pLDTFile->GetFieldCount();

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		for( int iField = 0 ; iField < iFieldCount ; iField++ )
		{
			pLDTFile->GetField( iItemID , iField , ldtFieldFXGroup );

			if( ldtFieldFXGroup.uData.lValue > 0 )
			{
				switch( eMotionType )
				{
				case MOTION_FX_PC:
					uiAniIndex = GetGOBSeqKey( iItemID , iField + 1 );
					break;
				case MOTION_FX_MOB:
					uiAniIndex = GetLDTMOBID( iItemID , iField );
					break;
				case MOTION_FX_NPC:
					uiAniIndex = GetLDTNPCID( iItemID , iField );
					break;
				}

				hmMotionFX.insert ( stdext::hash_map<unsigned int , unsigned int>::value_type( uiAniIndex , ldtFieldFXGroup.uData.lValue ) );
			}
		}
	}

	SAFE_RELEASE(pLDTFile);

	// MotionFX LDT LOAD END

	return true;
}

SPFXGroup* SPFXManager::GetPCFXGroup( unsigned int uiAniIndex )
{	
	SPFXGroup* pFXGroup = NULL;
	stdext::hash_map<unsigned int, unsigned int>::iterator hmIter = m_hmMotionFX_PC.find( uiAniIndex );
	if( hmIter != m_hmMotionFX_PC.end() && ( uiAniIndex == (*hmIter).first ) ) 
	{
		SPID_FX_GROUP	eFXGroup	=	static_cast<SPID_FX_GROUP>( (*hmIter).second );
		pFXGroup = GetFXGroup( eFXGroup );
	}
	return pFXGroup;
}

SPFXGroup* SPFXManager::GetPCFXGroupNotDel( unsigned int uiAniIndex )
{	
	SPFXGroup* pFXGroup = NULL;
	stdext::hash_map<unsigned int, unsigned int>::iterator hmIter = m_hmMotionFX_PC_NotDel.find( uiAniIndex );
	if( hmIter != m_hmMotionFX_PC_NotDel.end() && ( uiAniIndex == (*hmIter).first ) ) 
	{
		SPID_FX_GROUP	eFXGroup	=	static_cast<SPID_FX_GROUP>( (*hmIter).second );
		pFXGroup = GetFXGroup( eFXGroup );
	}
	return pFXGroup;
}

SPFXGroup* SPFXManager::GetMOBFXGroup( unsigned int uiAniIndex , unsigned int uiClassID )
{	
	SPFXGroup* pFXGroup = NULL;
	uiAniIndex = GetHashMapMOBID( uiClassID , uiAniIndex );
	stdext::hash_map<unsigned int, unsigned int>::iterator hmIter = m_hmMotionFX_MOB.find( uiAniIndex );
	if( hmIter != m_hmMotionFX_MOB.end() && ( uiAniIndex == (*hmIter).first ) ) 
	{
		SPID_FX_GROUP	eFXGroup	=	static_cast<SPID_FX_GROUP>( (*hmIter).second );
		pFXGroup = GetFXGroup( eFXGroup );
	}
	return pFXGroup;
}

SPFXGroup* SPFXManager::GetNPCFXGroup( unsigned int uiAniIndex , unsigned int uiClassID )
{	
	SPFXGroup* pFXGroup = NULL;
	uiAniIndex = GetHashMapNPCID( uiClassID , uiAniIndex );
	stdext::hash_map<unsigned int, unsigned int>::iterator hmIter = m_hmMotionFX_NPC.find( uiAniIndex );
	if( hmIter != m_hmMotionFX_NPC.end() && ( uiAniIndex == (*hmIter).first ) ) 
	{
		SPID_FX_GROUP	eFXGroup	=	static_cast<SPID_FX_GROUP>( (*hmIter).second );
		pFXGroup = GetFXGroup( eFXGroup );
	}
	return pFXGroup;
}

unsigned int SPFXManager::GetLDTMOBID( unsigned int uiRecord , unsigned int uiField )
{
	uiRecord	=	uiRecord	-	static_cast<unsigned int>(CLASS_ID_MOB_FIRST);
	return	uiRecord * 100 + uiField;
}

unsigned int SPFXManager::GetLDTNPCID( unsigned int uiRecord , unsigned int uiField )
{
	uiRecord	=	uiRecord	-	static_cast<unsigned int>(CLASS_ID_MAPEVENT_FIRST);
	return	uiRecord * 100 + uiField;
}

unsigned int SPFXManager::GetHashMapMOBID( unsigned int uiClassID , unsigned int uiAniIndex )
{
	uiClassID	=	uiClassID	-	static_cast<unsigned int>(CLASS_ID_MOB_FIRST);
	uiAniIndex	=	uiAniIndex	-	static_cast<unsigned int>(ANI_MONSTER_BEGIN);
	return	uiClassID * 100 + uiAniIndex;
}

unsigned int SPFXManager::GetHashMapNPCID( unsigned int uiClassID , unsigned int uiAniIndex )
{
	uiClassID	=	uiClassID	-	static_cast<unsigned int>(CLASS_ID_MAPEVENT_FIRST);
	uiAniIndex	=	uiAniIndex	-	static_cast<unsigned int>(ANI_NPC_BEGIN);
	return	uiClassID * 100 + uiAniIndex;
}
