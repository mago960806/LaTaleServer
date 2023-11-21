// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-20    13:57 
//***************************************************************************

#include "SPCommon.h"

#include "SPFXDEF.h"
#include "SPRenderModel.h"
#include "SPFXModel.h"
#include "SPGameObject.h"
#include "SPFXUnit.h"
#include "SPFXFrameAnimationUnit.h"

#include "SPFXDamageUnit.h"

//////////////////////////////////////////////////////////////////////////
//
//	SPFXDamageUnit 시작
//

SPFXDamageUnit::SPFXDamageUnit()
{
	SetNull();
}

SPFXDamageUnit::~SPFXDamageUnit()
{
	Destory();
}

void SPFXDamageUnit::SetNull()
{
	SPFXFrameAnimationUnit::SetNull();

	m_strDamage.clear();

	for( int i = 0 ; i < SPFXDAMAGE_NUMBER_COUNT ; i++ )
	{
		m_rtNumberSrc[ i ].left		=	0;
		m_rtNumberSrc[ i ].top		=	0;
		m_rtNumberSrc[ i ].right	=	0;
		m_rtNumberSrc[ i ].bottom	=	0;
	}

	m_ptNumberSize.x	=	0;
	m_ptNumberSize.y	=	0;

	m_ptGOBBox.x		=	0;
	m_ptGOBBox.y		=	0;
	m_eDamageType		=	FDT_ATTACK;
	m_ptOffSet.x		=	0;
	m_ptOffSet.y		=	0;

	//m_rcRender.left		=	0;
	//m_rcRender.top		=	0;
	//m_rcRender.right	=	0;
	//m_rcRender.bottom	=	0;
}

void SPFXDamageUnit::Init()
{
	InitSrcRect();

	SPFXFrameAnimationUnit::Init();
}

bool SPFXDamageUnit::Process( float fTime )
{
	if( SPFXFrameAnimationUnit::Process( fTime ) == false )
		return false;

	int		iNum		=	atoi( m_strDamage.c_str() );

	m_RectSrc	=	m_rtNumberSrc[ iNum ];

	return true;
}

bool SPFXDamageUnit::Render( float fTime )
{
	if( SPFXUnit::Render( fTime ) == false )
		return false;

	int		iXPos		=	0;
	int		iYPos		=	0;

	CalRenderPos( iXPos , iYPos );

	//////////////////////////////////////////////////////////////////////////
	//
	//	클리핑 체크
	//

	if( IsClipping( iXPos , iYPos , m_RectSrc.right - m_RectSrc.left , m_RectSrc.bottom - m_RectSrc.top , m_fScaleX , m_fScaleY ) == true )
	{
		RenderReset( fTime );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	(*m_ppCurImage)->RenderTextureImmediateCenterBase( iXPos , iYPos );

	//////////////////////////////////////////////////////////////////////////

	RenderReset( fTime );

	return true;
}

void SPFXDamageUnit::InitSrcRect()
{
	if( m_pFXModel == NULL )
	{
		m_FUState = FS_RELEASE;
		return;
	}

	SPFXFrameData* pFrameData = (SPFXFrameData*)m_pFXModel->GetMemberValue( MV_GET_FRAMEDATA , 0 );

	if( pFrameData == NULL )
	{
		m_FUState = FS_RELEASE;
		return;
	}

	m_ptNumberSize.x	=	pFrameData->RectAdjust.right;
	m_ptNumberSize.y	=	pFrameData->RectAdjust.bottom;

	if( m_ptNumberSize.x <= 0 || m_ptNumberSize.y <= 0 )
	{
		m_FUState = FS_RELEASE;
		return;
	}

	int iOffSetY	=	(int)( m_eDamageType ) * m_ptNumberSize.y;

	//	숫자별 SrcRect를 지정하자.
	for( int i = 0 ; i < SPFXDAMAGE_NUMBER_COUNT ; i++ )
	{
		m_rtNumberSrc[ i ].left		=	m_ptNumberSize.x * i;
		m_rtNumberSrc[ i ].top		=	iOffSetY;

		m_rtNumberSrc[ i ].right	=	m_ptNumberSize.x * ( i + 1 );
		m_rtNumberSrc[ i ].bottom	=	m_ptNumberSize.y + iOffSetY;
	}

	int	iXPos , iYPos;

	CalRenderPos( iXPos , iYPos );

	int	iXSize	=	m_ptNumberSize.x	/	2;
	int	iYSize	=	m_ptNumberSize.y	/	2;

	//m_rcRender.left		=	iXPos	-	iXSize;
	//m_rcRender.top		=	iYPos	-	iYSize;
	//m_rcRender.right	=	iXPos	+	iXSize;
	//m_rcRender.bottom	=	iYPos	+	iYSize;
}

void SPFXDamageUnit::SetDamage( const char* pstrNumber , int iStrLength )
{
	m_strDamage		=	pstrNumber;

	CalStrPos( iStrLength );
}

void SPFXDamageUnit::CalStrPos( int iStrLength )
{
	SPFXFrameData* pFrameData = (SPFXFrameData*)m_pFXModel->GetMemberValue( MV_GET_FRAMEDATA , iStrLength );

	if( pFrameData == NULL )
	{
		m_FUState = FS_RELEASE;
		return;
	}

	m_ptOffSet.x	=	pFrameData->RectAdjust.right /	2;
	m_ptOffSet.y	=	pFrameData->RectAdjust.bottom;

	Process( 0.0f );
}

void SPFXDamageUnit::SetGOBBox( POINT ptBox )
{
	m_ptGOBBox	=	ptBox;
}

void SPFXDamageUnit::SetDamageType( FXUNIT_DAMAGE_TYPE eDamageType )
{
	m_eDamageType	=	eDamageType;
}

RECT SPFXDamageUnit::GetRenderRect()
{
	return	m_rcRender;
}

void SPFXDamageUnit::CalRenderPos( int& iXPos , int& iYPos )
{
	if( m_bScreenRender == TRUE )
	{
		iXPos	=	abs( m_RectAdjust.left );
		iYPos	=	m_RectAdjust.top;
		return;
	}

	m_fCamX = g_pCameraManagr->GetCameraStruct()->fOffsetX;
	m_fCamY = g_pCameraManagr->GetCameraStruct()->fOffsetY;

	float	fPosX , fPosY;

	fPosX	=	m_fPosX	-	m_fCamX;
	fPosY	=	m_fPosY	-	m_fCamY	-	m_ptGOBBox.y;

	fPosX	=	fPosX	+	static_cast<float>( m_RectAdjust.left	)	-	static_cast<float>( m_ptOffSet.x );
	fPosY	=	fPosY	+	static_cast<float>( m_RectAdjust.top	)	-	static_cast<float>( m_ptOffSet.y );

	iXPos	=	static_cast<int>( fPosX );
	iYPos	=	static_cast<int>( fPosY );

}
