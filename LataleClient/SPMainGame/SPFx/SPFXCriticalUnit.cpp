// ***************************************************************
//  SPFXCriticalUnit.cpp   version:  1.0   date:2006/01/24
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 AJJIYA - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"

#include "SPFXDEF.h"
#include "SPRenderModel.h"
#include "SPFXModel.h"
#include "SPGameObject.h"
#include "SPFXUnit.h"
#include "SPFXFrameAnimationUnit.h"

#include "SPFXCriticalUnit.h"

//////////////////////////////////////////////////////////////////////////
//
//	SPFXCriticalUnit 시작
//

SPFXCriticalUnit::SPFXCriticalUnit()
{
	SetNull();
}

SPFXCriticalUnit::~SPFXCriticalUnit()
{
	Destory();
}

void SPFXCriticalUnit::SetNull()
{
	SPFXFrameAnimationUnit::SetNull();

	m_ptGOBBox.x		=	0;
	m_ptGOBBox.y		=	0;
}

void SPFXCriticalUnit::Init()
{
	SPFXFrameAnimationUnit::Init();
}

bool SPFXCriticalUnit::Render( float fTime )
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

	RenderReset( fTime );

	return true;
}

void SPFXCriticalUnit::SetGOBBox( POINT ptBox )
{
	m_ptGOBBox	=	ptBox;
}

void SPFXCriticalUnit::CalRenderPos( int& iXPos , int& iYPos )
{
	m_fCamX = g_pCameraManagr->GetCameraStruct()->fOffsetX;
	m_fCamY = g_pCameraManagr->GetCameraStruct()->fOffsetY;

	float	fPosX , fPosY;

	fPosX	=	m_fPosX	-	m_fCamX;
	fPosY	=	m_fPosY	-	m_fCamY	-	m_ptGOBBox.y;

	fPosX	=	fPosX	+	static_cast<float>( m_RectAdjust.left	);
	fPosY	=	fPosY	+	static_cast<float>( m_RectAdjust.top	);

	iXPos	=	static_cast<int>( fPosX );
	iYPos	=	static_cast<int>( fPosY );
}
