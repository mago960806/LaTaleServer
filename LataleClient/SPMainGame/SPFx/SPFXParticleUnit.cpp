// ***************************************************************
//  SPFXParticleUnit   version:  1.0   ·  date: 09/29/2006
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPGameObject.h"
#include "SPGOBManager.h"

#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXUtil.h"
#include "SPFXParticleObject.h"
#include "SPFXManager.h"

#include "SPFXParticleUnit.h"

SPFXParticleUnit::SPFXParticleUnit()
{
	SetNull();
}

SPFXParticleUnit::~SPFXParticleUnit()
{
	Destory();
}

void SPFXParticleUnit::SetNull()
{
	SPFXUnit::SetNull();

	m_RGBA				=	INITCOLOR;

	m_fScaleX			=	1.0f;
	m_fScaleY			=	1.0f;

	m_RectAdjust.left	=	0;
	m_RectAdjust.top	=	0;
	m_RectAdjust.right	=	0;
	m_RectAdjust.bottom	=	0;

	m_bLRSwap			=	false;

	m_iCurFrameNum							=	0;
	m_iMaxFrameCount						=	0;

	m_fCalOut[ PARTICLE_DATA_ALPHA ]		=	1.0f;
	m_fCalOut[ PARTICLE_DATA_DIRECTION ]	=	0.0f;
	m_fCalOut[ PARTICLE_DATA_GRAVITY ]		=	0.0f;
	m_fCalOut[ PARTICLE_DATA_RANGE ]		=	0.0f;
	m_fCalOut[ PARTICLE_DATA_MAGNET_POINT ]	=	0.0f;
	m_fCalOut[ PARTICLE_DATA_RED ]			=	1.0f;
	m_fCalOut[ PARTICLE_DATA_GREEN ]		=	1.0f;
	m_fCalOut[ PARTICLE_DATA_BLUE ]			=	1.0f;

	m_fCalOutNextGravity					=	0.0f;

	float	fDefaultValue	=	0.0f;

	for( int i = 0 ; i < PARTICLE_DATA_COUNT ; ++i )
	{
		if(	i == PARTICLE_DATA_ALPHA ||	i == PARTICLE_DATA_RED || i == PARTICLE_DATA_GREEN || i == PARTICLE_DATA_BLUE )
		{
			fDefaultValue	=	1.0f;
		}
		else
		{
			fDefaultValue	=	0.0f;
		}

		m_fStartTimeOut[ i ]	=	0.0f;
		m_fEndTimeOut[ i ]		=	0.0f;

		m_fStartValueOut[ i ]	=	fDefaultValue;
		m_fEndValueOut[ i ]		=	fDefaultValue;
	}

	m_fStartTimeOutNextGravity	=	0.0f;
	m_fEndTimeOutNextGravity	=	0.0f;
	m_fStartValueOutNextGravity	=	0.0f;
	m_fEndValueOutNextGravity	=	0.0f;

	STD_VECTOR_SPFXPARTICLEOBJECT::iterator		Iter	=	m_vpParticleObject.begin();

	while( Iter != m_vpParticleObject.end() )
	{
		(*Iter)->Destory();
		++Iter;
	}

	m_vpParticleObject.clear();
}

void SPFXParticleUnit::CalFrameData()
{
	SPFXParticleFrameData*		pFrameData		=	(SPFXParticleFrameData*)m_pFXModel->GetMemberValue( MV_GET_FRAMEDATA, m_iCurFrameNum );

	if( pFrameData == NULL )
		return;

	m_fCurDelay			= pFrameData->fFrameTime;
	m_RectSrc			= pFrameData->RectArea;
	m_fCurRotateDegree	= pFrameData->fFrameDegree;
	m_fCreateTime		= pFrameData->fCreateTime;
	m_iCreateCount		= pFrameData->iCreateCount;
	m_iMagnetPosX		= pFrameData->iMagnetPosX;
	m_iMagnetPosY		= pFrameData->iMagnetPosY;

	SPFXParticleFrameData*		pFrameDataNext	=	(SPFXParticleFrameData*)m_pFXModel->GetMemberValue( MV_GET_FRAMEDATA, m_iCurFrameNum + 1 );

	if( pFrameDataNext != NULL )
	{
		float	fLerpTime	=	m_fAccumulateRender / m_fCurDelay;

		SPFX_UTIL::Lerp( m_RectSrc , pFrameData->RectArea , pFrameDataNext->RectArea , fLerpTime );
		SPFX_UTIL::Lerp( m_fCurRotateDegree , pFrameData->fFrameDegree , pFrameDataNext->fFrameDegree , fLerpTime );

		SPFX_UTIL::Lerp( m_iMagnetPosX , pFrameData->iMagnetPosX , pFrameDataNext->iMagnetPosX , fLerpTime );
		SPFX_UTIL::Lerp( m_iMagnetPosY , pFrameData->iMagnetPosY , pFrameDataNext->iMagnetPosY , fLerpTime );
	}

}

void SPFXParticleUnit::CalValueData( float fTime )
{
	STD_MAP_PARTICLE_VALUE*	pmParticleValue	=	NULL;
	float					fNextTime;

	for( int i = 0 ; i < PARTICLE_DATA_COUNT ; ++i )
	{
		pmParticleValue	=	(STD_MAP_PARTICLE_VALUE*)m_pFXModel->GetMemberValue( MV_GET_PARTICLE_ITEM_DATA , (LPARAM)i );

		if( m_fAccumulateProcess <= 0.0f || m_fAccumulateProcess < m_fStartTimeOut[ i ] || m_fAccumulateProcess > m_fEndTimeOut[ i ] )
		{
			SPFX_UTIL::CalMapParticleValue( m_fStartValueOut[ i ] , m_fEndValueOut[ i ] , m_fStartTimeOut[ i ] , m_fEndTimeOut[ i ] , pmParticleValue , m_fAccumulateProcess );
		}

		SPFX_UTIL::CalMapParticleValueLerp( m_fCalOut[ i ] , m_fStartValueOut[ i ] , m_fEndValueOut[ i ] , m_fStartTimeOut[ i ] , m_fEndTimeOut[ i ] , m_fAccumulateProcess );

		if( (SPFX_PARTICLE_DATA_TYPE)i == PARTICLE_DATA_GRAVITY )
		{
			fNextTime	=	m_fAccumulateProcess + fTime;

			if( m_fAccumulateProcess <= 0.0f || fNextTime < m_fStartTimeOutNextGravity || fNextTime > m_fEndTimeOutNextGravity )
			{
				SPFX_UTIL::CalMapParticleValue( m_fStartValueOutNextGravity , m_fEndValueOutNextGravity , m_fStartTimeOutNextGravity , m_fEndTimeOutNextGravity , pmParticleValue , fNextTime );
			}

			SPFX_UTIL::CalMapParticleValueLerp( m_fCalOutNextGravity , m_fStartValueOutNextGravity , m_fEndValueOutNextGravity , m_fStartTimeOutNextGravity , m_fEndTimeOutNextGravity , fNextTime );
		}
	}
}

void SPFXParticleUnit::RandomCreatePos( float& fOutPosX , float& fOutPosY )
{
	GetRandom( 1000 );		//	테스트

	float	fRandomAngle	=	GetRandomAngle();

	//////////////////////////////////////////////////////////////////////////

	if( m_eAreaType == SPFX_PARTICLE_AREA_ELLIPSE )
		RandomCreatePosEllipse( fOutPosX , fOutPosY , fRandomAngle );
	else
		RandomCreatePosArea( fOutPosX , fOutPosY , fRandomAngle );
}

void SPFXParticleUnit::RandomCreatePosEllipse( float& fOutPosX , float& fOutPosY , float fRandomAngle )
{
	float	fWidth		=	(float)( m_RectSrc.right	-	m_RectSrc.left );
	float	fHeight		=	(float)( m_RectSrc.bottom	-	m_RectSrc.top );

	float	fCenterPosX	=	(float)( m_RectSrc.left )	+	fWidth	/	2;
	float	fCenterPosY	=	(float)( m_RectSrc.top )	+	fHeight	/	2;

	float	fRadiusX	=	fWidth	/	2.0f;
	float	fRadiusY	=	fHeight	/	2.0f;

	SPFX_UTIL::CalEllipse( fOutPosX , fOutPosY , fCenterPosX , fCenterPosY , fRadiusX , fRadiusY , fRandomAngle - m_fCurRotateDegree );

	float	fDistance	=	SPFX_UTIL::Distance2D( fOutPosX , fOutPosY , fCenterPosX , fCenterPosY );
	float	fCalAngle;

	SPFX_UTIL::CalAngle2D( fCalAngle , 0.0f , fDistance , fOutPosX - fCenterPosX , fOutPosY - fCenterPosY );
	SPFX_UTIL::CalEllipse( fOutPosX , fOutPosY , fCenterPosX , fCenterPosY , fDistance , fDistance , fCalAngle + m_fCurRotateDegree );
}

void SPFXParticleUnit::RandomCreatePosArea( float& fOutPosX , float& fOutPosY , float fRandomAngle )
{
	float	fMaxXPos		,	fMaxYPos;
	float	fBoxXPos[ 5 ]	,	fBoxYPos[ 5 ];

	fBoxXPos[ 0 ]	=	(float)( m_RectSrc.left );
	fBoxYPos[ 0 ]	=	(float)( m_RectSrc.top );

	fBoxXPos[ 1 ]	=	(float)( m_RectSrc.right );
	fBoxYPos[ 1 ]	=	(float)( m_RectSrc.top );

	fBoxXPos[ 2 ]	=	(float)( m_RectSrc.right );
	fBoxYPos[ 2 ]	=	(float)( m_RectSrc.bottom );

	fBoxXPos[ 3 ]	=	(float)( m_RectSrc.left );
	fBoxYPos[ 3 ]	=	(float)( m_RectSrc.bottom );

	fBoxXPos[ 4 ]	=	(float)( m_RectSrc.left );
	fBoxYPos[ 4 ]	=	(float)( m_RectSrc.top );

	float	fWidth	=	(float)( m_RectSrc.right	-	m_RectSrc.left );
	float	fHeight	=	(float)( m_RectSrc.bottom	-	m_RectSrc.top );

	float	fCenterPosX	=	(float)( m_RectSrc.left )	+	fWidth	/	2.0f;
	float	fCenterPosY	=	(float)( m_RectSrc.top )	+	fHeight	/	2.0f;

	float	fDistance	=	0.0f;
	float	fRadius		=	0.0f;

	for( int i = 0 ; i < 4 ; ++i )
	{
		fDistance	=	SPFX_UTIL::Distance2D( fCenterPosX , fCenterPosY , fBoxXPos[ i ] , fBoxYPos[ i ] );

		if( fDistance > fRadius )
			fRadius	=	fDistance;
	}

	float	fEllipseXPos , fEllipseYPos;

	SPFX_UTIL::CalEllipse( fEllipseXPos , fEllipseYPos , fCenterPosX , fCenterPosY , fRadius , fRadius , fRandomAngle );

	float	fAngle;

	for( i = 0 ; i < 5 ; ++i )
	{
		SPFX_UTIL::CalAngle2D( fAngle , 0.0f , fRadius , fBoxXPos[ i ] - fCenterPosX , fBoxYPos[ i ] - fCenterPosY );
		SPFX_UTIL::CalEllipse( fBoxXPos[ i ] , fBoxYPos[ i ] , fCenterPosX , fCenterPosY , fRadius , fRadius , fAngle + m_fCurRotateDegree );
	}

	bool	bResult	=	false;

	for( i = 0 ; i < 4 ; ++i )
	{
		bResult	=	SPFX_UTIL::GetIntersection( fMaxXPos , fMaxYPos , fCenterPosX , fCenterPosY , fEllipseXPos , fEllipseYPos , fBoxXPos[ i ] , fBoxYPos[ i ] , fBoxXPos[ i + 1 ] , fBoxYPos[ i + 1 ] );

		if( bResult == true )
			break;
	}

	fDistance	=	0.0f;

	if( bResult == true )
		fDistance	=	SPFX_UTIL::Distance2D( fCenterPosX , fCenterPosY , fMaxXPos , fMaxYPos );

	fRadius	=	(float)GetRandom( fDistance );

	SPFX_UTIL::CalEllipse( fOutPosX , fOutPosY , fCenterPosX , fCenterPosY , fRadius , fRadius , fRandomAngle );
}

void SPFXParticleUnit::SettingObject( SPFXParticleObject& stObject )
{
	if( m_pFXModel == NULL )
		return;

	if( m_bScreenRender == FALSE )
	{
		stObject.SetCarmeraPos( &m_fCamX , &m_fCamY );
		stObject.SetParentPos( m_fPosX , m_fPosY );
	}
	else
	{
		stObject.SetParentPos( 400.0f , 450.0f );
	}

	stObject.SetParentAlpha( &m_fCalOut[ PARTICLE_DATA_ALPHA ] );
	stObject.SetParentGravity( &m_fCalOut[ PARTICLE_DATA_GRAVITY ] );
	stObject.SetPathFollow( m_bPathFollow );
	stObject.SetParentNextGravity( &m_fCalOutNextGravity );
	stObject.SetMagnetPos( &m_iMagnetPosX , &m_iMagnetPosY );
	stObject.SetParentMagnet( &m_fCalOut[ PARTICLE_DATA_MAGNET_POINT ] );
	stObject.SetParentRed( &m_fCalOut[ PARTICLE_DATA_RED ] );
	stObject.SetParentGreen( &m_fCalOut[ PARTICLE_DATA_GREEN ] );
	stObject.SetParentBlue( &m_fCalOut[ PARTICLE_DATA_BLUE ] );

	STD_MAP_PARTICLE_VALUE*	pmObjectValue;
	int	i	=	0;

	for( i = 0 ; i < PARTICLE_OBJECT_DATA_COUNT ; ++i )
	{
		pmObjectValue	=	(STD_MAP_PARTICLE_VALUE*)m_pFXModel->GetMemberValue( MV_GET_PARTICLE_OBJECT_ITEM_DATA , (LPARAM)i );
		stObject.SetObjectValue( (SPFX_PARTICLE_OBJECT_DATA_TYPE)i , pmObjectValue );
	}

	if( m_ppCurImage != NULL )
		stObject.SetTexture( *m_ppCurImage );

	if( m_pFXModel != NULL )
	{
		int				iImageFrameCount	=	(int)m_pFXModel->GetMemberValue( MV_GET_PARTICLE_IMAGE_FRAMEDATA_COUNT );
		SPFXFrameData*	pFrameData			=	NULL;

		for( i = 0 ; i < iImageFrameCount ; ++i )
		{
			pFrameData			=	(SPFXFrameData*)m_pFXModel->GetMemberValue( MV_GET_PARTICLE_IMAGE_FRAMEDATA, i );

			stObject.AddImageFrameData( pFrameData );
		}

		if( pFrameData != NULL )
		{
			if( pFrameData->RectAdjust.bottom > 0 )
				stObject.SetRandomStart( true );
			else
				stObject.SetRandomStart( false );
		}
	}
}

void SPFXParticleUnit::CreateObject()
{
	if( m_iCreateCount == 0 )
		return;

	SPFXParticleObject*		pParticleObject	=	NULL;
	float					fXPos , fYPos;

	int		iCreateCount		=	m_iCreateCount;
	float	fRandomAngle		=	(float)GetRandom( 1000 );
	bool	bParentDirection	=	false;

	if( m_FXCurUseParam == FXPARAM_FACING_LEFT )
		bParentDirection	=	true;

	//////////////////////////////////////////////////////////////////////////

	int	iMinCreateCount	=	1;

	if( iCreateCount > iMinCreateCount )
	{
		if( g_pClientOption != NULL )
		{
			SPOptionStruct*	pOptionStruct	=	g_pClientOption->GetOptionStruct();

			if( pOptionStruct != NULL )
			{
				switch( pOptionStruct->iEffect )
				{
				case 0:				iCreateCount	=	iMinCreateCount;							break;
				case 1:				iCreateCount	=	(int)( (float)iCreateCount	*	0.25f );	break;
				case 2:				iCreateCount	=	(int)( (float)iCreateCount	*	0.50f );	break;
				case 3:				iCreateCount	=	(int)( (float)iCreateCount	*	0.75f );	break;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	for( int i = 0 ; i < iCreateCount ; ++i )
	{
		pParticleObject	=	g_pFXManager->GetFXParticleObjectBuffer();

		if( pParticleObject == NULL )
			return;

		SettingObject( *pParticleObject );

		RandomCreatePos( fXPos, fYPos );

		fRandomAngle	=	GetRandomAngle();

		pParticleObject->SetDefaultPos( fXPos , fYPos );
		pParticleObject->SetDirection( fRandomAngle );
		pParticleObject->SetParentDirection( bParentDirection );
		pParticleObject->Create();

		m_vpParticleObject.push_back( pParticleObject );
	}
}

void SPFXParticleUnit::ProcessObject( float fTime )
{
	if(	m_FUState == FS_RELEASE || m_FUState == FS_INIT )
		return;

	STD_VECTOR_SPFXPARTICLEOBJECT::iterator		Iter			=	m_vpParticleObject.begin();
	SPFXParticleObject*							pParticleObject	=	NULL;
	bool										bDelete			=	false;

	//////////////////////////////////////////////////////////////////////////

	int											iRenderCurCount	=	0;
	int											iRenderMaxCount	=	(int)m_vpParticleObject.size();

	int	iMinProcessCount	=	1;

	if( iRenderMaxCount > iMinProcessCount )
	{
		if( g_pClientOption != NULL )
		{
			SPOptionStruct*	pOptionStruct	=	g_pClientOption->GetOptionStruct();

			if( pOptionStruct != NULL )
			{
				switch( pOptionStruct->iEffect )
				{
				case 0:		iRenderMaxCount	=	iMinProcessCount;								break;
				case 1:		iRenderMaxCount	=	(int)( (float)iRenderMaxCount	*	0.25f );	break;
				case 2:		iRenderMaxCount	=	(int)( (float)iRenderMaxCount	*	0.50f );	break;
				case 3:		iRenderMaxCount	=	(int)( (float)iRenderMaxCount	*	0.75f );	break;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	while( Iter != m_vpParticleObject.end() )
	{
		pParticleObject	=	(*Iter);

		if( pParticleObject != NULL )
		{
			if( iRenderCurCount >= iRenderMaxCount )
				pParticleObject->Destory();
			else
				pParticleObject->Process( fTime );

			++iRenderCurCount;

			if( pParticleObject->GetState() == FS_RELEASE )
			{
				Iter	=	m_vpParticleObject.erase( Iter );
				bDelete	=	true;
			}
		}

		if( bDelete == true )
			bDelete			=	false;
		else
			++Iter;
	}
}

void SPFXParticleUnit::RenderObject()
{
	if(	m_FUState == FS_RELEASE || m_FUState == FS_INIT )
		return;

	STD_VECTOR_SPFXPARTICLEOBJECT::iterator	Iter	=	m_vpParticleObject.begin();
	SPFXParticleObject*						pParticleObject	=	NULL;

	while( Iter != m_vpParticleObject.end() )
	{
		pParticleObject	=	(*Iter);

		if( pParticleObject != NULL )
			pParticleObject->Render();

		++Iter;
	}
}

float SPFXParticleUnit::GetRandomAngle()
{
	float	fRandomAngle	=	(float)GetRandom( m_fCalOut[ PARTICLE_DATA_RANGE ] );

	if( (int)GetRandom( 1000 ) > 500 )
		fRandomAngle	=	m_fCalOut[ PARTICLE_DATA_DIRECTION ] + m_fCurRotateDegree + fRandomAngle;
	else
		fRandomAngle	=	m_fCalOut[ PARTICLE_DATA_DIRECTION ] + m_fCurRotateDegree - fRandomAngle;

	return fRandomAngle;
}

void SPFXParticleUnit::Init()
{
	m_FUState			=	FS_INIT;

	if( m_pFXModel == NULL )
	{
		m_FUState		=	FS_RELEASE;
		return;
	}

	m_iCurFrameNum		=	0;
	m_iMaxFrameCount	=	(int)m_pFXModel->GetMemberValue( MV_GET_MAXFRAME );
	m_fModelDelay		=	0.0f;

	m_eAreaType			=	(SPFX_PARTICLE_AREA_TYPE)m_pFXModel->GetMemberValue( MV_GET_PARTICLE_AREA_TYPE );
	m_bPathFollow		=	(BOOL)m_pFXModel->GetMemberValue( MV_GET_PARTICLE_PATH_FOLLOW );

	m_fAccumulateCreateTime	=	0.0f;

	SPFXUnit::Init();

	Process( 0.0f );

	int	iMaxCount	=	100;

	if( m_fCreateTime > 0.0f &&  m_iCreateCount > 0 )
	{
		iMaxCount	=	static_cast<int>( m_fFXMLifeTime / m_fCreateTime * (float)m_iCreateCount );
	}

	m_vpParticleObject.reserve( iMaxCount );

	CreateObject();
}

bool SPFXParticleUnit::Process( float fTime )
{
	if(	m_FUState == FS_RELEASE || m_FUState == FS_INIT )
		return false;

	//////////////////////////////////////////////////////////////////////////

	m_fAccumulateCreateTime	+=	fTime;

	if( m_fAccumulateCreateTime >= m_fCreateTime && m_fCreateTime > 0.0f )
	{
		m_fAccumulateCreateTime	-=	m_fCreateTime;
		CreateObject();
	}

	CalFrameData();
	CalValueData( fTime );

	ProcessObject( fTime );

	//////////////////////////////////////////////////////////////////////////

	if( SPFXUnit::Process( fTime ) == false )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	if( ++m_iCurFrameNum >= m_iMaxFrameCount )
	{
		if( m_bLoop == TRUE )
		{
			m_iCurFrameNum = 0;

			SPGameObject*	pGOB	=	NULL;

			if( m_tGOBID > 0 )
				pGOB	=	g_pGOBManager->Find( m_tGOBID );

			if( pGOB != NULL )
			{
				m_fPosX	=	(float)pGOB->SPGOBPerformMessage( SPGM_GETPOSX );
				m_fPosY	=	(float)pGOB->SPGOBPerformMessage( SPGM_GETPOSY );
			}
			else if( m_tGOBID <= 0 )
			{
				m_fPosX	=	m_fParentPosX;
				m_fPosY	=	m_fParentPosY;
			}
		}
		else
			m_iCurFrameNum = m_iMaxFrameCount - 1;
	}

	m_fAccumulateCreateTime	=	0.0f;

	CalFrameData();
	CreateObject();

	return true;
}

bool SPFXParticleUnit::Render( float fTime )
{
	if( SPFXUnit::Render( fTime ) == false )
		return false;

	//////////////////////////////////////////////////////////////////////////

	RenderObject();

	//////////////////////////////////////////////////////////////////////////

	RenderReset( fTime );

//	RenderOnlyLine();

	return true;
}

void SPFXParticleUnit::RenderOnlyLine()
{
	float	fAdjustX	=	0.0f;
	float	fAdjustY	=	0.0f;

	fAdjustX	=	m_fPosX	-	m_fCamX;
	fAdjustY	=	m_fPosY	-	m_fCamY;

	float	fAngle;
	float	fXPosBox[ 5 ] , fYPosBox[ 5 ];

	fXPosBox[ 0 ]	=	(float)( m_RectSrc.left );
	fYPosBox[ 0 ]	=	(float)( m_RectSrc.top );

	fXPosBox[ 1 ]	=	(float)( m_RectSrc.right );
	fYPosBox[ 1 ]	=	(float)( m_RectSrc.top );

	fXPosBox[ 2 ]	=	(float)( m_RectSrc.right );
	fYPosBox[ 2 ]	=	(float)( m_RectSrc.bottom );

	fXPosBox[ 3 ]	=	(float)( m_RectSrc.left );
	fYPosBox[ 3 ]	=	(float)( m_RectSrc.bottom );

	fXPosBox[ 4 ]	=	(float)( m_RectSrc.left );
	fYPosBox[ 4 ]	=	(float)( m_RectSrc.top );

	float	fWidth		=	(float)( m_RectSrc.right	- m_RectSrc.left );
	float	fHeight		=	(float)( m_RectSrc.bottom	- m_RectSrc.top );

	float	fHalfWidth	=	fWidth	/ 2.0f;
	float	fHalfHeight	=	fHeight	/ 2.0f;

	float	fXCenterPos	=	(float)( m_RectSrc.left )	+	fHalfWidth;
	float	fYCenterPos	=	(float)( m_RectSrc.top )	+	fHalfHeight;

	float	fRadiusX	=	SPFX_UTIL::Distance2D( fXCenterPos , fYCenterPos , fXPosBox[ 0 ] , fYPosBox[ 0 ] );
	float	fRadiusY	=	fRadiusX;

	float	fXPos , fYPos , fXPosOld , fYPosOld;
	RECT	rcLine;

	//////////////////////////////////////////////////////////////////////////

	// 활성 상태 [9/11/2006 AJJIYA]
	m_RGBA.a		=	m_fCalOut[ PARTICLE_DATA_ALPHA ];

	//////////////////////////////////////////////////////////////////////////

	// 범위 [9/11/2006 AJJIYA]
	float	fStartRange	=	m_fCalOut[ PARTICLE_DATA_DIRECTION ] + m_fCurRotateDegree - m_fCalOut[ PARTICLE_DATA_RANGE ];
	float	fEndRange	=	m_fCalOut[ PARTICLE_DATA_DIRECTION ] + m_fCurRotateDegree + m_fCalOut[ PARTICLE_DATA_RANGE ];

	SPFX_UTIL::CalEllipse( fXPos , fYPos , fXCenterPos , fYCenterPos , fRadiusX , fRadiusY , fStartRange );

	rcLine.left		=	(LONG)( fXCenterPos	+	fAdjustX );
	rcLine.top		=	(LONG)( fYCenterPos	+	fAdjustY );
	rcLine.right	=	(LONG)( fXPos		+	fAdjustX );
	rcLine.bottom	=	(LONG)( fYPos		+	fAdjustY );

	g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 0.0f , 0.0f , 1.0f , m_RGBA.a ) );
	g_pVideo->GetSysLine()->DrawLine( rcLine );

	SPFX_UTIL::CalEllipse( fXPos , fYPos , fXCenterPos , fYCenterPos , fRadiusX , fRadiusY , fEndRange );

	rcLine.left		=	(LONG)( fXCenterPos	+	fAdjustX );
	rcLine.top		=	(LONG)( fYCenterPos	+	fAdjustY );
	rcLine.right	=	(LONG)( fXPos		+	fAdjustX );
	rcLine.bottom	=	(LONG)( fYPos		+	fAdjustY );

	g_pVideo->GetSysLine()->DrawLine( rcLine );
	g_pVideo->GetSysLine()->SetColor( INITCOLOR );

	//////////////////////////////////////////////////////////////////////////

	// 진행 방향 [9/11/2006 AJJIYA]

	SPFX_UTIL::CalEllipse( fXPos , fYPos , fXCenterPos , fYCenterPos , fRadiusX , fRadiusY , m_fCalOut[ PARTICLE_DATA_DIRECTION ] + m_fCurRotateDegree );

	rcLine.left		=	(LONG)( fXCenterPos	+	fAdjustX );
	rcLine.top		=	(LONG)( fYCenterPos	+	fAdjustY );
	rcLine.right	=	(LONG)( fXPos		+	fAdjustX );
	rcLine.bottom	=	(LONG)( fYPos		+	fAdjustY );

	g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 0.0f , 1.0f , 0.0f , m_RGBA.a ) );
	g_pVideo->GetSysLine()->DrawLine( rcLine );
	g_pVideo->GetSysLine()->SetColor( INITCOLOR );

	//////////////////////////////////////////////////////////////////////////

	switch( m_eAreaType )
	{
	case SPFX_PARTICLE_AREA_ELLIPSE:
		{
			g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 0.0f , 1.0f , 0.0f , m_RGBA.a ) );

			for( float fAngle = 0.0f ; fAngle < 370.0f ; fAngle += 10.0f )
			{
				SPFX_UTIL::CalEllipse( fXPos , fYPos , fXCenterPos , fYCenterPos , fHalfWidth , fHalfHeight , fAngle );

				if( fAngle != 0.0f )
				{
					rcLine.left		=	(LONG)( fXPos		+	fAdjustX );
					rcLine.top		=	(LONG)( fYPos		+	fAdjustY );
					rcLine.right	=	(LONG)( fXPosOld	+	fAdjustX );
					rcLine.bottom	=	(LONG)( fYPosOld	+	fAdjustY );

					g_pVideo->GetSysLine()->DrawLine( rcLine );
				}

				fXPosOld	=	fXPos;
				fYPosOld	=	fYPos;
			}

			g_pVideo->GetSysLine()->SetColor( INITCOLOR );
		}
		break;
	}

	for( int i = 0 ; i < 5 ; ++i )
	{
		SPFX_UTIL::CalAngle2D( fAngle , 0.0f , fRadiusY , fXPosBox[ i ] - fXCenterPos , fYPosBox[ i ] - fYCenterPos );
		SPFX_UTIL::CalEllipse( fXPosBox[ i ] , fYPosBox[ i ] , fXCenterPos , fYCenterPos , fRadiusX , fRadiusY , fAngle + m_fCurRotateDegree );
	}

	g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , m_RGBA.a ) );

	for( i = 0 ; i < 4 ; ++i )
	{
		rcLine.left		=	(LONG)( fXPosBox[ i ]		+	fAdjustX );
		rcLine.top		=	(LONG)( fYPosBox[ i ]		+	fAdjustY );
		rcLine.right	=	(LONG)( fXPosBox[ i + 1 ]	+	fAdjustX );
		rcLine.bottom	=	(LONG)( fYPosBox[ i + 1 ]	+	fAdjustY );

		g_pVideo->GetSysLine()->DrawLine( rcLine );
	}

	//////////////////////////////////////////////////////////////////////////

	rcLine.left		=	m_iMagnetPosX	-	10	+	(int)fAdjustX;
	rcLine.right	=	m_iMagnetPosX	+	10	+	(int)fAdjustX;
	rcLine.top		=	m_iMagnetPosY	+	(int)fAdjustY;
	rcLine.bottom	=	m_iMagnetPosY	+	(int)fAdjustY;

	g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 0.0f , 0.0f ,1.0f , 1.0f ) );
	g_pVideo->GetSysLine()->DrawLine( rcLine );

	rcLine.top		=	m_iMagnetPosY	-	10	+	(int)fAdjustY;
	rcLine.bottom	=	m_iMagnetPosY	+	10	+	(int)fAdjustY;
	rcLine.left		=	m_iMagnetPosX	+	(int)fAdjustX;
	rcLine.right	=	m_iMagnetPosX	+	(int)fAdjustX;

	g_pVideo->GetSysLine()->DrawLine( rcLine );

	//////////////////////////////////////////////////////////////////////////

	g_pVideo->GetSysLine()->SetColor( INITCOLOR );

	//////////////////////////////////////////////////////////////////////////

	//SPFont*	pFont	=	g_pVideo->GetFont( FONT_12_NORMAL );

	//char	szBuf[_MAX_PATH];

	//if( pFont != NULL )
	//{
	//	int	iFontXPos	=	300;
	//	int	iFontYPos	=	10;
	//	int	iFontStep	=	20;

	//	sprintf( szBuf , " All Alpha : %f" , m_fCalOut[ PARTICLE_DATA_ALPHA ] );
	//	pFont->RenderTextPos( iFontXPos, iFontYPos , szBuf );
	//	iFontYPos	+=	iFontStep;

	//	sprintf( szBuf , " Direction : %f" , m_fCalOut[ PARTICLE_DATA_DIRECTION ] );
	//	pFont->RenderTextPos( iFontXPos, iFontYPos , szBuf );
	//	iFontYPos	+=	iFontStep;


		/*
		m_RGBA.r	=	1.0f;
		m_RGBA.b	=	0.0f;
		m_RGBA.g	=	0.0f;

		g_pVideo->GetSysLine()->SetColor( m_RGBA );

		int	iCollisionXPos	=	0;
		int	iCollisionYPos	=	0;

		bool bCollision = Collision_Rectangle_Point_2D( fXPos[ 0 ] , fYPos[ 0 ] , fXPos[ 1 ] , fYPos[ 1 ] , fXPos[ 2 ] , fYPos[ 2 ] , fXPos[ 3 ] , fYPos[ 3 ] , iCollisionXPos , iCollisionYPos );

		sprintf( szBuf , "Collision : %d , %d = %d " , iCollisionXPos , iCollisionYPos , bCollision );
		pFont->RenderTextPos( iFontXPos, iFontYPos , szBuf );
		iFontYPos	+=	iFontStep;

		rcLine.left		=	iCollisionXPos	-	iFontStep	+	iAdjustX;
		rcLine.top		=	iCollisionYPos					+	iAdjustY;
		rcLine.right	=	iCollisionXPos	+	iFontStep	+	iAdjustX;
		rcLine.bottom	=	iCollisionYPos					+	iAdjustY;

		g_pVideo->GetSysLine()->DrawLine( rcLine );

		rcLine.left		=	iCollisionXPos					+	iAdjustX;
		rcLine.top		=	iCollisionYPos	-	iFontStep	+	iAdjustY;	
		rcLine.right	=	iCollisionXPos					+	iAdjustX;
		rcLine.bottom	=	iCollisionYPos	+	iFontStep	+	iAdjustY;

		g_pVideo->GetSysLine()->DrawLine( rcLine );

		iCollisionXPos	=	-100;
		iCollisionYPos	=	-100;

		bCollision = Collision_Rectangle_Point_2D( fXPos[ 0 ] , fYPos[ 0 ] , fXPos[ 1 ] , fYPos[ 1 ] , fXPos[ 2 ] , fYPos[ 2 ] , fXPos[ 3 ] , fYPos[ 3 ] , iCollisionXPos , iCollisionYPos );

		sprintf( szBuf , "Collision : %d , %d = %d " , iCollisionXPos , iCollisionYPos , bCollision );
		pFont->RenderTextPos( iFontXPos, iFontYPos , szBuf );
		iFontYPos	+=	iFontStep;

		rcLine.left		=	iCollisionXPos	-	iFontStep	+	iAdjustX;
		rcLine.top		=	iCollisionYPos					+	iAdjustY;
		rcLine.right	=	iCollisionXPos	+	iFontStep	+	iAdjustX;
		rcLine.bottom	=	iCollisionYPos					+	iAdjustY;

		g_pVideo->GetSysLine()->DrawLine( rcLine );

		rcLine.left		=	iCollisionXPos					+	iAdjustX;
		rcLine.top		=	iCollisionYPos	-	iFontStep	+	iAdjustY;	
		rcLine.right	=	iCollisionXPos					+	iAdjustX;
		rcLine.bottom	=	iCollisionYPos	+	iFontStep	+	iAdjustY;

		g_pVideo->GetSysLine()->DrawLine( rcLine );

		iCollisionXPos	=	100;
		iCollisionYPos	=	-100;

		bCollision = Collision_Rectangle_Point_2D( fXPos[ 0 ] , fYPos[ 0 ] , fXPos[ 1 ] , fYPos[ 1 ] , fXPos[ 2 ] , fYPos[ 2 ] , fXPos[ 3 ] , fYPos[ 3 ] , iCollisionXPos , iCollisionYPos );

		sprintf( szBuf , "Collision : %d , %d = %d " , iCollisionXPos , iCollisionYPos , bCollision );
		pFont->RenderTextPos( iFontXPos, iFontYPos , szBuf );
		iFontYPos	+=	iFontStep;

		rcLine.left		=	iCollisionXPos	-	iFontStep	+	iAdjustX;
		rcLine.top		=	iCollisionYPos					+	iAdjustY;
		rcLine.right	=	iCollisionXPos	+	iFontStep	+	iAdjustX;
		rcLine.bottom	=	iCollisionYPos					+	iAdjustY;

		g_pVideo->GetSysLine()->DrawLine( rcLine );

		rcLine.left		=	iCollisionXPos					+	iAdjustX;
		rcLine.top		=	iCollisionYPos	-	iFontStep	+	iAdjustY;	
		rcLine.right	=	iCollisionXPos					+	iAdjustX;
		rcLine.bottom	=	iCollisionYPos	+	iFontStep	+	iAdjustY;

		g_pVideo->GetSysLine()->DrawLine( rcLine );

		*/

	//	g_pVideo->GetSysLine()->SetColor( INITCOLOR );
	//}
}

//////////////////////////////////////////////////////////////////////////

/*

const float FX_Y_ADJUST = 70.0f;	// 캐릭터 키(Height) 때문에 올림.

SPFXParticleUnit21::SPFXParticleUnit21()
{
	SetNull();
}

SPFXParticleUnit21::~SPFXParticleUnit21()
{
}

void SPFXParticleUnit21::SetNull()
{
	m_iMaxRenderParticle	=	0;
	m_iMaxParticle			=	0;
	m_iCurNOParticle		=	0;
	m_vParticleInstance.clear();
	m_ParticleMove			=	PARTICLE_NULL;
}

void SPFXParticleUnit21::Init()
{
	m_FUState			=	FS_INIT;

	if( m_pFXModel == NULL )
	{
		m_FUState		=	FS_RELEASE;
		return;
	}

	m_fModelDelay		= *(float*)		m_pFXModel->GetMemberValue( MV_NEXTANIMATION );
	m_fCurRotateDegree	= *(float*)		m_pFXModel->GetMemberValue( MV_GET_RADIANDEGREE );
	m_RGBA				= *(D3DXCOLOR*)	m_pFXModel->GetMemberValue( MV_GET_COLOR );
	m_fScaleX			= *(float*)		m_pFXModel->GetMemberValue( MV_GET_SCALE );
	m_RectSrc			= *(RECT*)		m_pFXModel->GetMemberValue( MV_GET_SRC_RECT );
	m_RectAdjust		= *(RECT*)		m_pFXModel->GetMemberValue( MV_GET_ADJUST );
	m_ParticleMove		= (SPFX_PARTICLE_MOVE)m_pFXModel->GetMemberValue( MV_GET_PARTICLE_MOVE );

	m_fScaleY			=	m_fScaleX;

	// 최대 그릴수 있는 수
	m_iMaxRenderParticle 	= (UINT)m_pFXModel->GetMemberValue(MV_GET_MAXRENDERPARTICLE);	

	// 몇개 까지 찍구 끝낼거냐 0이면 항상 m_iMaxRenderParticle로 찍는다.
	m_iMaxParticle			= (UINT)m_pFXModel->GetMemberValue(MV_GET_MAXPARTICLE);
	m_iCurNOParticle		= 0;

	//(*m_ppCurImage)->SetSrcRect( m_RectSrc );

	SPFXParticleInstance tempInstance;
	for( UINT i = 0 ; i < m_iMaxRenderParticle ; ++i )
	{
		InitParticleInstance(tempInstance);
		m_vParticleInstance.push_back(tempInstance);
	}

	SPFXUnit::Init();
}

bool SPFXParticleUnit21::Process( float fTime )
{
	if( SPFXUnit::Process( fTime ) == false )
		return false;

	std::vector<SPFXParticleInstance>::iterator	Iter;

	m_fCurRotateDegree	+=	*(float*)m_pFXModel->GetMemberValue( MV_GET_RADIANDEGREE );

	switch(m_ParticleMove)
	{
		case PARTICLE_CHARGE:
			Iter = m_vParticleInstance.begin();
			for(;Iter != m_vParticleInstance.end(); ++Iter)
			{
				(*Iter).vec2Pos += (*Iter).vec2Velocity;

				if((*Iter).vec2Velocity.x < 0.0f && (*Iter).vec2Pos.x < 0.0f)
					InitParticleInstance(*Iter);

				if((*Iter).vec2Velocity.y < 0.0f && (*Iter).vec2Pos.y < 0.0f)
					InitParticleInstance(*Iter);

				if((*Iter).vec2Velocity.x > 0.0f && (*Iter).vec2Pos.x > 0.0f)
					InitParticleInstance(*Iter);

				if((*Iter).vec2Velocity.y > 0.0f && (*Iter).vec2Pos.y > 0.0f)
					InitParticleInstance(*Iter);
			}
			break;

		case PARTICLE_SPREAD_BACK:
		case PARTICLE_SPREAD:
			Iter = m_vParticleInstance.begin();
			for(;Iter != m_vParticleInstance.end(); ++Iter) {
				(*Iter).vec2Pos += (*Iter).vec2Velocity;
				if((*Iter).vec2Pos.x < m_RectAdjust.left || (*Iter).vec2Pos.x > m_RectAdjust.right) {
					InitParticleInstance(*Iter);
				}
				if((*Iter).vec2Pos.y < m_RectAdjust.top || (*Iter).vec2Pos.y > m_RectAdjust.bottom) {
					InitParticleInstance(*Iter);
				}
			}
			break;

		case PARTICLE_DROP:
			{
			}
			break;
	}

	return true;
}

bool SPFXParticleUnit21::Render( float fTime )
{
	if( SPFXUnit::Render( fTime ) == false )
		return false;

	//////////////////////////////////////////////////////////////////////////

	int	iPosX , iPosY;

	std::vector<SPFXParticleInstance>::iterator Iter = m_vParticleInstance.begin();

	for(;Iter != m_vParticleInstance.end(); ++Iter)
	{
		iPosX	=	(int)( m_fPosX - m_fCamX + (*Iter).vec2Pos.x );
		iPosY	=	(int)( m_fPosY - m_fCamY + (*Iter).vec2Pos.y - FX_Y_ADJUST );

		(*m_ppCurImage)->RenderTextureImmediateCenterBase( iPosX , iPosY );
	}

	//////////////////////////////////////////////////////////////////////////

	RenderReset( fTime );

	return true;
}

void SPFXParticleUnit21::InitParticleInstance( SPFXParticleInstance& ParticleInstance )
{
	SPGameObject*	pGOB	=	g_pGOBManager->Find( m_tGOBID );

	if( m_iFollowParent & FX_FOLLOW_FACINGPARENT && pGOB != NULL )
	{
		FACING Dir = (FACING)pGOB->SPGOBPerformMessage( SPGM_GETFACING );
		if(Dir == FACING_LEFT)
		{
			m_FXCurUseParam = FXPARAM_FACING_LEFT;
		} else if( Dir == FACING_RIGHT )
		{
			m_FXCurUseParam = FXPARAM_FACING_RIGHT;
		}
	} else {
		switch(m_FXParam)
		{
		case FACING_LEFT:
			m_FXCurUseParam = FXPARAM_FACING_LEFT;
			break;
		case FACING_RIGHT:
			m_FXCurUseParam = FXPARAM_FACING_RIGHT;
			break;		
		}
	}

	switch(m_ParticleMove)
	{
	case PARTICLE_SPREAD_BACK:
		{
			ParticleInstance.vec2Pos.x = (float)( (rand() % (m_RectAdjust.right/4)*2) - m_RectAdjust.right / 4 );
			ParticleInstance.vec2Pos.y = (float)( (rand() % (m_RectAdjust.bottom/4)*2) - m_RectAdjust.bottom / 4 );

			switch(m_FXCurUseParam)
			{
			case FXPARAM_FACING_LEFT:
				{
					ParticleInstance.vec2Pos.x = -abs(ParticleInstance.vec2Pos.x);
				}
				break;
			case FXPARAM_FACING_RIGHT:
				{
					ParticleInstance.vec2Pos.x = +abs(ParticleInstance.vec2Pos.x);
				}
				break;
			}
			D3DXVec2Normalize( &ParticleInstance.vec2Velocity, &ParticleInstance.vec2Pos);
			ParticleInstance.vec2Velocity *= 2.5f;
		}
		break;
	case PARTICLE_SPREAD:
		{
			ParticleInstance.vec2Pos.x = (float)( (rand() % (m_RectAdjust.right/4)*2) - m_RectAdjust.right/4 );
			ParticleInstance.vec2Pos.y = (float)( (rand() % (m_RectAdjust.bottom/4)*2) - m_RectAdjust.bottom/4 );

			D3DXVec2Normalize( &ParticleInstance.vec2Velocity, &ParticleInstance.vec2Pos);
			ParticleInstance.vec2Velocity *= 2.5f;
		}
		break;

	case PARTICLE_CHARGE:
		{
			ParticleInstance.vec2Pos.x = (float)( (rand() % (m_RectAdjust.right*2)) - m_RectAdjust.right );
			ParticleInstance.vec2Pos.y = (float)( (rand() % (m_RectAdjust.bottom*2)) - m_RectAdjust.bottom );

			D3DXVec2Normalize( &ParticleInstance.vec2Velocity, &ParticleInstance.vec2Pos);
			ParticleInstance.vec2Velocity *= -2.5f;
		}
		break;

	case PARTICLE_DROP:
		{
		}
		break;
	}

	// TODO: Initialize tempInstance.vec2Accel
	// 가속도
}
*/