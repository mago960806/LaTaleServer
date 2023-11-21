// ***************************************************************
//  SPFXParticleObject   version:  1.0   ·  date: 10/12/2006
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXUtil.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"

#include "SPFXParticleObject.h"

//////////////////////////////////////////////////////////////////////////

SPFXParticleObject::SPFXParticleObject(void)
{
	SetNull();
}

SPFXParticleObject::~SPFXParticleObject(void)
{
	Destory();
}

void SPFXParticleObject::SetNull()
{
	m_FUState				=	FS_INIT;
	m_fAccumulateProcess	=	0.0f;

	m_pfCarmeraPosX			=	NULL;
	m_pfCarmeraPosY			=	NULL;

	m_fParentPosX			=	0.0f;
	m_fParentPosY			=	0.0f;

	m_pfParentAlpha			=	NULL;
	m_pfParentGravity		=	NULL;
	m_fDirection			=	0.0f;
	m_bPathFollow			=	false;
	m_fPathFollowAngle		=	0.0f;
	m_pfParentNextGravity	=	NULL;
	m_piMagnetPosX			=	NULL;
	m_piMagnetPosY			=	NULL;

	m_pfParentRed			=	NULL;
	m_pfParentGreen			=	NULL;
	m_pfParentBlue			=	NULL;
	m_pfParentMagnet		=	NULL;

	m_bParentDirection		=	false;

	m_fPosX					=	0.0f;
	m_fPosY					=	0.0f;
	m_fDefaultPosX			=	0.0f;
	m_fDefaultPosY			=	0.0f;


	m_pTexture				=	NULL;
	m_bRandomStart			=	false;

	m_iCurFrameNumber		=	0;
	m_iStartFrameNumber		=	0;
	m_iEndFrameNumber		=	0;

	m_fAccumulateImageFrame	=	0.0f;
	m_fCurFrameDelayTime	=	0.0f;

	m_pCurFrameData			=	NULL;
	m_vpImageFrameData.clear();
}

void SPFXParticleObject::Create()
{
	m_fPosX		=	m_fDefaultPosX;
	m_fPosY		=	m_fDefaultPosY;

	m_fAccumulateProcess	=	0.0f;
	m_fAccumulateImageFrame	=	0.0f;

	m_fCalOut[ PARTICLE_OBJECT_DATA_VELOCITY ]			=	0.0f;
	m_fCalOut[ PARTICLE_OBJECT_DATA_X_SCALE ]			=	1.0f;
	m_fCalOut[ PARTICLE_OBJECT_DATA_Y_SCALE ]			=	1.0f;
	m_fCalOut[ PARTICLE_OBJECT_DATA_DIRECTION_MOVE ]	=	0.0f;
	m_fCalOut[ PARTICLE_OBJECT_DATA_LIFETIME ]			=	0.0f;
	m_fCalOut[ PARTICLE_OBJECT_DATA_ROTATION ]			=	0.0f;
	m_fCalOut[ PARTICLE_OBJECT_DATA_ROTATION_VELOCITY ]	=	0.0f;
	m_fCalOut[ PARTICLE_OBJECT_DATA_ALPHA ]				=	1.0f;
	m_fCalOut[ PARTICLE_OBJECT_DATA_MAGNET_POINT ]		=	0.0f;
	m_fCalOut[ PARTICLE_OBJECT_DATA_RED ]				=	1.0f;
	m_fCalOut[ PARTICLE_OBJECT_DATA_GREEN ]				=	1.0f;
	m_fCalOut[ PARTICLE_OBJECT_DATA_BLUE ]				=	1.0f;

	float	fDefaultValue		=	0.0f;

	for( int i = 0 ; i < PARTICLE_OBJECT_DATA_COUNT ; ++i )
	{
		if( i == PARTICLE_OBJECT_DATA_X_SCALE ||
			i == PARTICLE_OBJECT_DATA_Y_SCALE ||
			i == PARTICLE_OBJECT_DATA_ALPHA ||
			i == PARTICLE_OBJECT_DATA_RED ||
			i == PARTICLE_OBJECT_DATA_GREEN ||
			i == PARTICLE_OBJECT_DATA_BLUE )
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

	if( m_fDirection > 0.0f )
		m_fPathFollowAngle	=	180.0f	-	m_fDirection;
	else
		m_fPathFollowAngle	=	-( 180.0f	+	m_fDirection );

	m_iCurFrameNumber	=	0;
	m_pCurFrameData		=	NULL;

	if( m_bRandomStart == true )
		m_iCurFrameNumber	=	(int)GetRandom( m_iEndFrameNumber );

	m_iStartFrameNumber	=	m_iCurFrameNumber;

	GetCurImageFrameData();
	CalNowDirection( 0.0f );

	//////////////////////////////////////////////////////////////////////////

//	m_vLine1.push_back( D3DXVECTOR2( (float)m_fPosX , (float)m_fPosY ) );
//	m_vLine2.push_back( D3DXVECTOR2( (float)m_fPosX , (float)m_fPosY ) );

	//////////////////////////////////////////////////////////////////////////

	m_FUState	=	FS_ENABLE;

	Process( 0.0f );
}

void SPFXParticleObject::Destory()
{
	SetNull();

	m_FUState	=	FS_RELEASE;
}

void SPFXParticleObject::Process( float fTime )
{
	if(	m_FUState == FS_RELEASE || m_FUState == FS_INIT )
		return;

	m_fAccumulateProcess	+=	fTime;
	m_fAccumulateImageFrame	+=	fTime;

	CalValueData( m_fAccumulateProcess );

	if( m_fAccumulateProcess >= m_fCalOut[ PARTICLE_OBJECT_DATA_LIFETIME ] )
	{
		m_FUState	=	FS_RELEASE;
		return;
	}

	if( m_fAccumulateImageFrame >= m_fCurFrameDelayTime )
	{
		m_fAccumulateImageFrame	-=	m_fCurFrameDelayTime;

		++m_iCurFrameNumber;

		if( m_iCurFrameNumber >= m_iEndFrameNumber )
			m_iCurFrameNumber	=	0;

		if( m_bRandomStart == TRUE && m_iCurFrameNumber == m_iStartFrameNumber )
		{
			m_iStartFrameNumber	=	(int)GetRandom( m_iEndFrameNumber );
			m_iCurFrameNumber	=	m_iStartFrameNumber;
		}

		GetCurImageFrameData();
	}

	CalNowDirection( fTime );
}

void SPFXParticleObject::Render()
{
	if(	m_FUState == FS_RELEASE || m_FUState == FS_INIT || m_pCurFrameData == NULL || m_pTexture == NULL )
		return;

	//////////////////////////////////////////////////////////////////////////

	D3DXCOLOR	CurColor(	m_fCalOut[ PARTICLE_OBJECT_DATA_RED ]	,
							m_fCalOut[ PARTICLE_OBJECT_DATA_GREEN ]	,
							m_fCalOut[ PARTICLE_OBJECT_DATA_BLUE ]	,
							m_fCalOut[ PARTICLE_OBJECT_DATA_ALPHA ] );

	if( m_pfParentRed != NULL )
		CurColor.r	=	*m_pfParentRed		*	m_fCalOut[ PARTICLE_OBJECT_DATA_RED ];

	if( m_pfParentGreen != NULL )
		CurColor.g	=	*m_pfParentGreen	*	m_fCalOut[ PARTICLE_OBJECT_DATA_GREEN ];

	if( m_pfParentBlue != NULL )
		CurColor.b	=	*m_pfParentBlue		*	m_fCalOut[ PARTICLE_OBJECT_DATA_BLUE ];

	if( m_pfParentAlpha != NULL )
		CurColor.a	=	*m_pfParentAlpha	*	m_fCalOut[ PARTICLE_OBJECT_DATA_ALPHA ];

	if( CurColor.a < 0.0f )
		CurColor.a = 0.0f;

	//////////////////////////////////////////////////////////////////////////

	float	fScaleX		=	m_fCalOut[ PARTICLE_OBJECT_DATA_X_SCALE ]	* m_pCurFrameData->fFrameScaleX;
	float	fScaleY		=	m_fCalOut[ PARTICLE_OBJECT_DATA_Y_SCALE ]	* m_pCurFrameData->fFrameScaleY;
	float	fAngle		=	m_fCalOut[ PARTICLE_OBJECT_DATA_ROTATION ]	+ m_fCalOut[ PARTICLE_OBJECT_DATA_ROTATION_VELOCITY ] * m_fAccumulateProcess;
	float	fOffsetX	=	(float)m_pCurFrameData->RectAdjust.left;
	float	fOffsetY	=	(float)m_pCurFrameData->RectAdjust.top;

	RECT	rcDest;

	if( m_bPathFollow == TRUE )
		fAngle	=	m_fPathFollowAngle;

	float	fFinalPosX	=	m_fPosX	-	fOffsetX	+	m_fParentPosX;
	float	fFinalPosY	=	m_fPosY	-	fOffsetY	+	m_fParentPosY;

	if( m_pfCarmeraPosX != NULL )
		fFinalPosX	-=	*m_pfCarmeraPosX;

	if( m_pfCarmeraPosY != NULL )
		fFinalPosY	-=	*m_pfCarmeraPosY;

	POINT	ptSrcSize;

	ptSrcSize.x		=	m_pCurFrameData->RectSrc.right	-	m_pCurFrameData->RectSrc.left;
	ptSrcSize.y		=	m_pCurFrameData->RectSrc.bottom	-	m_pCurFrameData->RectSrc.top;

	rcDest.left		=	(int)(fFinalPosX);
	rcDest.top		=	(int)(fFinalPosY);
	rcDest.right	=	rcDest.left	+	ptSrcSize.x;
	rcDest.bottom	=	rcDest.top	+	ptSrcSize.y;

	//////////////////////////////////////////////////////////////////////////
	//
	//	클리핑 체크
	//

	RECT	rcClipping;

	rcClipping.left				=	rcDest.left;
	rcClipping.top				=	rcDest.top;
	rcClipping.right			=	rcClipping.left	+	(int)( (float)ptSrcSize.x * fScaleX );
	rcClipping.bottom			=	rcClipping.top	+	(int)( (float)ptSrcSize.y * fScaleY );

	int		iClippingOffsetX	=	(int)( fOffsetX * fScaleX );
	int		iClippingOffsetY	=	(int)( fOffsetY * fScaleY );

	rcClipping.left				-=	iClippingOffsetX;
	rcClipping.top				-=	iClippingOffsetY;
	rcClipping.right			-=	iClippingOffsetX;
	rcClipping.bottom			-=	iClippingOffsetY;

	int		iVideoSizeX			=	g_pVideo->GetScreenLenX();
	int		iVideoSizeY			=	g_pVideo->GetScreenLenY();

	if( ( rcClipping.left < 0 && rcClipping.right < 0 ) || ( rcClipping.left > iVideoSizeX && rcClipping.right > iVideoSizeX ) ||
		( rcClipping.top < 0 && rcClipping.bottom < 0 ) || ( rcClipping.top > iVideoSizeY && rcClipping.bottom > iVideoSizeY ) )
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////

	m_pTexture->SetColor( CurColor );
	m_pTexture->SetScale( fScaleX , fScaleY );
	m_pTexture->SetLRSwap( m_bParentDirection );

	m_pTexture->RenderTexture( &rcDest , &m_pCurFrameData->RectSrc , -fOffsetX , -fOffsetY , -fOffsetX / fScaleX , -fOffsetY / fScaleY , D3DXToRadian( fAngle ) );

	m_pTexture->SetColor( INITCOLOR );

	//////////////////////////////////////////////////////////////////////////

/*	int	iLine1Count		=	(int)m_vLine1.size();
	int	iLine2Count		=	(int)m_vLine2.size();
	int	i				=	0;

	RECT	rcLine;

	for( i = 1 ; i < iLine1Count ; ++i )
	{
		rcLine.left		=	(LONG)m_vLine1.at( i - 1 ).x;
		rcLine.top		=	(LONG)m_vLine1.at( i - 1 ).y;
		rcLine.right	=	(LONG)m_vLine1.at( i ).x;
		rcLine.bottom	=	(LONG)m_vLine1.at( i ).y;

		g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 1.0f , 0.0f , 0.0f , 1.0f ) );
		g_pVideo->GetSysLine()->DrawLine( rcLine );
		g_pVideo->GetSysLine()->SetColor( INITCOLOR );
	}

	for( i = 1 ; i < iLine2Count ; ++i )
	{
		rcLine.left		=	(LONG)m_vLine2.at( i - 1 ).x;
		rcLine.top		=	(LONG)m_vLine2.at( i - 1 ).y;
		rcLine.right	=	(LONG)m_vLine2.at( i ).x;
		rcLine.bottom	=	(LONG)m_vLine2.at( i ).y;

		g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 1.0f , 1.0f , 0.0f , 1.0f ) );
		g_pVideo->GetSysLine()->DrawLine( rcLine );
		g_pVideo->GetSysLine()->SetColor( INITCOLOR );
	}


	int	iLineMaxCount	=	min( iLine1Count , iLine2Count );

	//	for( i = 1 ; i < iLineMaxCount ; ++i )
	//	{
	////		if( iLine1Count > i )
	//		{
	//			rcLine.left		=	(LONG)m_vLine1.at( i ).x;
	//			rcLine.top		=	(LONG)m_vLine1.at( i ).y;
	//		}
	//
	////		if( iLine2Count > i )
	//		{
	//			rcLine.right	=	(LONG)m_vLine2.at( i ).x;
	//			rcLine.bottom	=	(LONG)m_vLine2.at( i ).y;
	//		}
	//
	//		g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ) );
	//		g_pVideo->GetSysLine()->DrawLine( rcLine );
	//		g_pVideo->GetSysLine()->SetColor( INITCOLOR );
	//	}


	//float	fXPos , fYPos , fXPosOld , fYPosOld;

	//GetCalPosByTime( fXPosOld , fYPosOld , 0.0f , m_pfParentGravity );

	//RECT	rcLine;

	//for( float fTime = 1.0f ; fTime < 10.0f ; fTime += 1.0f )
	//{
	//	GetCalPosByTime( fXPos , fYPos , fTime , m_pfParentGravity );

	//	rcLine.left		=	(LONG)fXPos;
	//	rcLine.top		=	(LONG)fYPos;
	//	rcLine.right	=	(LONG)fXPosOld;
	//	rcLine.bottom	=	(LONG)fYPosOld;

	//	fXPosOld		=	fXPos;
	//	fYPosOld		=	fYPos;

	//	g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 1.0f , 0.0f , 0.0f , 1.0f ) );
	//	g_pVideo->GetSysLine()->DrawLine( rcLine );
	//	g_pVideo->GetSysLine()->SetColor( INITCOLOR );
	//}

*/
}

void SPFXParticleObject::RenderOnlyLine()
{
	if(	m_FUState == FS_RELEASE || m_FUState == FS_INIT )
		return;

	float	fAlpha	=	1.0f;

	if( m_pfParentAlpha != NULL )
		fAlpha	=	*m_pfParentAlpha	*	m_fCalOut[ PARTICLE_OBJECT_DATA_ALPHA ];
	else
		fAlpha	=	m_fCalOut[ PARTICLE_OBJECT_DATA_ALPHA ];

	g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 1.0f , 1.0f , 0.0f , fAlpha ) );

	int	iScaleX	=	(int)m_fCalOut[ PARTICLE_OBJECT_DATA_X_SCALE ];
	int	iScaleY	=	(int)m_fCalOut[ PARTICLE_OBJECT_DATA_Y_SCALE ];

	if( iScaleX <= 0 )
		iScaleX	=	1;

	if( iScaleY	<= 0 )
		iScaleY	=	1;

	int	iAdjustX	=	400;
	int	iAdjustY	=	450;

	RECT	rcBox;

	rcBox.left		=	(LONG)m_fPosX	-	iScaleX	-	iAdjustX;
	rcBox.top		=	(LONG)m_fPosY	-	iScaleY	-	iAdjustY;
	rcBox.right		=	(LONG)m_fPosX	+	iScaleX	-	iAdjustX;
	rcBox.bottom	=	(LONG)m_fPosY	+	iScaleY	-	iAdjustY;

	float	fAngle;
	float	fXPos[ 5 ] , fYPos[ 5 ];

	fXPos[ 0 ]	=	(float)( rcBox.left );
	fYPos[ 0 ]	=	(float)( rcBox.top );

	fXPos[ 1 ]	=	(float)( rcBox.right );
	fYPos[ 1 ]	=	(float)( rcBox.top );

	fXPos[ 2 ]	=	(float)( rcBox.right );
	fYPos[ 2 ]	=	(float)( rcBox.bottom );

	fXPos[ 3 ]	=	(float)( rcBox.left );
	fYPos[ 3 ]	=	(float)( rcBox.bottom );

	fXPos[ 4 ]	=	(float)( rcBox.left );
	fYPos[ 4 ]	=	(float)( rcBox.top );

	int	iWidth		=	rcBox.right - rcBox.left;
	int	iHeight		=	rcBox.bottom - rcBox.top;

	float	fXCenterPos	=	(float)rcBox.left	+	(float)iWidth	/ 2;
	float	fYCenterPos	=	(float)rcBox.top	+	(float)iHeight	/ 2;

	float	fRadiusX	=	SPFX_UTIL::Distance2D( fXCenterPos , fYCenterPos , fXPos[ 0 ] , fYPos[ 0 ] );
	float	fRadiusY	=	fRadiusX;

	RECT	rcLine;

	fAngle	=	m_fCalOut[ PARTICLE_OBJECT_DATA_ROTATION ];

	for( int i = 0 ; i < 5 ; ++i )
	{
		SPFX_UTIL::CalAngle2D( fAngle , 0.0f , fRadiusY , fXPos[ i ] - fXCenterPos , fYPos[ i ] - fYCenterPos );
		SPFX_UTIL::CalEllipse( fXPos[ i ] , fYPos[ i ] , fXCenterPos , fYCenterPos , fRadiusX , fRadiusY , fAngle + m_fCalOut[ PARTICLE_OBJECT_DATA_ROTATION ] + m_fCalOut[ PARTICLE_OBJECT_DATA_ROTATION_VELOCITY ] * m_fAccumulateProcess );

		rcLine.left		=	(LONG)( fXCenterPos	+	iAdjustX );
		rcLine.top		=	(LONG)( fYCenterPos	+	iAdjustY );

		rcLine.right	=	(LONG)( fXPos[ i ]	+	iAdjustX );
		rcLine.bottom	=	(LONG)( fYPos[ i ]	+	iAdjustY );

		g_pVideo->GetSysLine()->DrawLine( rcLine );
	}

	for( int i = 0 ; i < 4 ; ++i )
	{
		rcLine.left		=	(LONG)( fXPos[ i ]	+	iAdjustX );
		rcLine.top		=	(LONG)( fYPos[ i ]	+	iAdjustY );

		rcLine.right	=	(LONG)( fXPos[ i + 1 ]	+	iAdjustX );
		rcLine.bottom	=	(LONG)( fYPos[ i + 1 ]	+	iAdjustY );

		g_pVideo->GetSysLine()->DrawLine( rcLine );
	}

	g_pVideo->GetSysLine()->SetColor( INITCOLOR );

}

FXUNIT_STATE SPFXParticleObject::GetState()
{
	return m_FUState;
}

void SPFXParticleObject::SetCarmeraPos( float* pfCarmeraPosX , float* pfCarmeraPosY )
{
	m_pfCarmeraPosX	=	pfCarmeraPosX;
	m_pfCarmeraPosY	=	pfCarmeraPosY;
}

void SPFXParticleObject::SetParentPos( float fParentPosX , float fParentPosY )
{
	m_fParentPosX	=	fParentPosX;
	m_fParentPosY	=	fParentPosY;
}

void SPFXParticleObject::SetParentRed( float* pfParentRed )
{
	m_pfParentRed		=	pfParentRed;
}

void SPFXParticleObject::SetParentGreen( float* pfParentGreen )
{
	m_pfParentGreen		=	pfParentGreen;
}

void SPFXParticleObject::SetParentBlue( float* pfParentBlue )
{
	m_pfParentBlue		=	pfParentBlue;
}

void SPFXParticleObject::SetParentMagnet( float* pfParentMagnet )
{
	m_pfParentMagnet	=	pfParentMagnet;
}

void SPFXParticleObject::SetParentAlpha( float* pfParentAlpha )
{
	m_pfParentAlpha		=	pfParentAlpha;
}

void SPFXParticleObject::SetParentGravity(float* pfParentGravity )
{
	m_pfParentGravity	=	pfParentGravity;
}

void SPFXParticleObject::SetDirection(float fDirection )
{
	m_fDirection	=	fDirection;
}

void SPFXParticleObject::SetPathFollow( BOOL bPathFollow )
{
	m_bPathFollow	=	bPathFollow;
}

void SPFXParticleObject::SetMagnetPos( int* piMagnetPosX , int* piMagnetPosY )
{
	m_piMagnetPosX	=	piMagnetPosX;
	m_piMagnetPosY	=	piMagnetPosY;
}

void SPFXParticleObject::SetParentNextGravity( float* pfParentNextGravity )
{
	m_pfParentNextGravity	=	pfParentNextGravity;
}

void SPFXParticleObject::SetParentDirection( bool bParentDirection )
{
	m_bParentDirection	=	bParentDirection;
}

void SPFXParticleObject::SetDefaultPos( float fDefaultPosX , float fDefaultPosY )
{
	m_fDefaultPosX	=	fDefaultPosX;
	m_fDefaultPosY	=	fDefaultPosY;
}

void SPFXParticleObject::SetObjectValue( SPFX_PARTICLE_OBJECT_DATA_TYPE eObjectDataType , STD_MAP_PARTICLE_VALUE* pmParticleObjectValue )
{
	m_pmParticleObjectValue[ eObjectDataType ]	=	pmParticleObjectValue;
}

void SPFXParticleObject::SetTexture( SPTexture* pTexture )
{
	m_pTexture	=	pTexture;
}

void SPFXParticleObject::SetRandomStart( bool bRandom )
{
	m_bRandomStart	=	bRandom;
}

void SPFXParticleObject::AddImageFrameData( SPFXFrameData* pFrameDaa )
{
	m_vpImageFrameData.push_back( pFrameDaa );

	m_iEndFrameNumber	=	(int)m_vpImageFrameData.size();
}

void SPFXParticleObject::CalValueData( float fTime )
{
	STD_MAP_PARTICLE_VALUE*	pmParticleValue	=	NULL;

	for( int i = 0 ; i < PARTICLE_OBJECT_DATA_COUNT ; ++i )
	{
		pmParticleValue	=	m_pmParticleObjectValue[ i ];

		if( fTime <= 0.0f || fTime < m_fStartTimeOut[ i ] || fTime > m_fEndTimeOut[ i ] )
		{
			SPFX_UTIL::CalMapParticleValue( m_fStartValueOut[ i ] , m_fEndValueOut[ i ] , m_fStartTimeOut[ i ] , m_fEndTimeOut[ i ] , pmParticleValue , fTime );
		}

		SPFX_UTIL::CalMapParticleValueLerp( m_fCalOut[ i ] , m_fStartValueOut[ i ] , m_fEndValueOut[ i ] , m_fStartTimeOut[ i ] , m_fEndTimeOut[ i ] , fTime );
	}
}

void SPFXParticleObject::GetCalPosByTime( float& fOutPosX , float& fOutPosY , float fTime , float* pfGravity )
{
	float	fXPos , fYPos;

	//	진행 방향 벡터 만들기
	float	fRadius	=	m_fCalOut[ PARTICLE_OBJECT_DATA_VELOCITY ] * fTime;

	SPFX_UTIL::CalEllipse( fXPos , fYPos , 0.0f , 0.0f , fRadius , fRadius , m_fDirection );

	D3DXVECTOR2		vDirection( fXPos , fYPos );

	//	중력 벡터 만들기
	D3DXVECTOR2		vGravity( 0.0f , 1.0f );

	if( pfGravity == NULL )
		vGravity	*=	0.0f;
	else
		vGravity	*=	( *pfGravity	*	fTime );

	//	상대 Y 이동 벡터 만들기
	fRadius	=	m_fCalOut[ PARTICLE_OBJECT_DATA_DIRECTION_MOVE ] * fTime;

	SPFX_UTIL::CalEllipse( fXPos , fYPos , 0.0f , 0.0f , fRadius , fRadius , m_fDirection + 90.0f );

	D3DXVECTOR2		vNormalDirection( fXPos , fYPos );

	//////////////////////////////////////////////////////////////////////////

	vDirection	+=	vNormalDirection;
	vDirection	+=	vGravity;

	if( m_bParentDirection == true )
	{
		fOutPosX	=	-m_fDefaultPosX	-	vDirection.x;
	}
	else
	{
		fOutPosX	=	m_fDefaultPosX	+	vDirection.x;
	}

	fOutPosY	=	m_fDefaultPosY	+	vDirection.y;

	//////////////////////////////////////////////////////////////////////////

	//	자석 포인터 벡터 만들기

	if( m_piMagnetPosX != NULL && m_piMagnetPosY != NULL )
	{
		D3DXVECTOR2		vStart( fOutPosX , fOutPosY );
		D3DXVECTOR2		vEnd( (FLOAT)*m_piMagnetPosX , (FLOAT)*m_piMagnetPosY );
		D3DXVECTOR2		vDistance( 0.0f , 0.0f );
		D3DXVECTOR2		vMagnetDir( 0.0f , 0.0f );

		float	fFinalMagnetData	=	m_fCalOut[ PARTICLE_OBJECT_DATA_MAGNET_POINT ];

		if( m_bParentDirection == true )
		{
			vEnd.x	=	-vEnd.x;
		}

		if( m_pfParentMagnet != NULL )
			fFinalMagnetData	*=	*m_pfParentMagnet;

		if( fabs( fFinalMagnetData ) > 0.0f )
		{
			vDistance	=	vEnd	-	vStart;
			D3DXVec2Normalize( &vMagnetDir , &vDistance );

			vMagnetDir	=	vMagnetDir	*	fFinalMagnetData	*	fTime;

			fOutPosX	+=	vMagnetDir.x;
			fOutPosY	+=	vMagnetDir.y;

			if( ( vMagnetDir.x > 0.0f && fOutPosX > vEnd.x ) ||
				( vMagnetDir.x < 0.0f && fOutPosX < vEnd.x ) )
			{
				fOutPosX	=	vEnd.x;
			}

			if( ( vMagnetDir.y > 0.0f && fOutPosY > vEnd.y ) ||
				( vMagnetDir.y < 0.0f && fOutPosY < vEnd.y ) )
			{
				fOutPosY	=	vEnd.y;
			}
		}
	}
}

void SPFXParticleObject::CalNowDirection( float fTime )
{
	GetCalPosByTime( m_fPosX , m_fPosY , m_fAccumulateProcess , m_pfParentGravity );

	//	PathFollow를 위한 각도 산출
	if( m_bPathFollow == TRUE )
	{
		float	fAngle , fNextPosX , fNextPosY;
		float	fNextAccumulateProcess	=	m_fAccumulateProcess	+	fTime;

		CalValueData( fNextAccumulateProcess );
		GetCalPosByTime( fNextPosX , fNextPosY , fNextAccumulateProcess , m_pfParentNextGravity );

		SPFX_UTIL::CalAngle2D( fAngle , 0.0f , -1.0f , fNextPosX - m_fPosX , fNextPosY - m_fPosY );

		m_fPathFollowAngle	=	-fAngle;

		//////////////////////////////////////////////////////////////////////////

//		m_vLine2.push_back( D3DXVECTOR2( (float)fNextPosX , (float)fNextPosY ) );
	}

	//////////////////////////////////////////////////////////////////////////

//	m_vLine1.push_back( D3DXVECTOR2( (float)m_fPosX , (float)m_fPosY ) );
}

void SPFXParticleObject::GetCurImageFrameData()
{
	int	iCount	=	(int)m_vpImageFrameData.size();

	if( iCount <= 0 )
		return;

	if( m_iCurFrameNumber >= iCount )
		m_iCurFrameNumber	=	iCount	-	1;

	m_pCurFrameData	=	m_vpImageFrameData.at( m_iCurFrameNumber );

	if( m_pCurFrameData == NULL )
		return;

	m_fCurFrameDelayTime	=	m_pCurFrameData->fFrameTime;
}
