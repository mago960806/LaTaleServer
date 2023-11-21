// ***************************************************************
//  SPFXFrameAnimationUnit   version:  1.0   ·  date: 09/29/2006
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

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"

#include "SPFXFrameAnimationUnit.h"

const float FX_Y_ADJUST = 70.0f;	// 캐릭터 키(Height) 때문에 올림.

SPFXFrameAnimationUnit::SPFXFrameAnimationUnit()
{
	SetNull();
}

SPFXFrameAnimationUnit::~SPFXFrameAnimationUnit()
{
	Destory();
}

void SPFXFrameAnimationUnit::SetNull()
{
	SPFXUnit::SetNull();

	m_iCurFrameNum		=	0;
	m_iMaxFrameCount	=	0;
}

void SPFXFrameAnimationUnit::Init()
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

	SPFXUnit::Init();
}

bool SPFXFrameAnimationUnit::Process( float fTime )
{
	if( SPFXUnit::Process( fTime ) == false )
		return false;

	SPFXFrameData* pFrameData = (SPFXFrameData*)m_pFXModel->GetMemberValue( MV_GET_FRAMEDATA , m_iCurFrameNum );

	if(pFrameData)
	{
		m_RGBA				= pFrameData->FrameColor;
		m_fCurDelay			= pFrameData->fFrameTime;
		m_RectSrc			= pFrameData->RectSrc;
		m_fScaleX			= pFrameData->fFrameScaleX;
		m_fScaleY			= pFrameData->fFrameScaleY;
		m_fCurRotateDegree	= pFrameData->fFrameDegree;
		m_RectAdjust		= pFrameData->RectAdjust;
		m_bLRSwap			= pFrameData->bLRSwap;

		if( m_iFollowParent == 0 )
			m_FXCurUseParam	= m_FXParam;

		if( m_FXCurUseParam == FXPARAM_NULL )
			m_FXCurUseParam = m_FXParam;

		if( m_FXCurUseParam == FXPARAM_FACING_LEFT )
			m_RectAdjust.left = -m_RectAdjust.left;

	} else {
		assert(0 && "SPFXUnit - FrameData not found");
	}

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
				m_fPosX	=	pGOB->SPGOBPerformMessage( SPGM_GETPOSX );
				m_fPosY	=	pGOB->SPGOBPerformMessage( SPGM_GETPOSY );
			}
			else if( m_tGOBID <= 0 )
			{
				m_fPosX	=	m_fParentPosX;
				m_fPosY	=	m_fParentPosY;
			}

		}
		else
			m_iCurFrameNum = m_iMaxFrameCount - 1;

		if( m_iCurFrameNum < 0 )
			m_iCurFrameNum	=	0;
	}

	return true;
}

bool SPFXFrameAnimationUnit::Render( float fTime )
{
	if( SPFXUnit::Render( fTime ) == false )
		return false;

	if( m_RectSrc.left == m_RectSrc.right || m_RectSrc.top == m_RectSrc.bottom )
	{
		RenderReset( fTime );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	bool	bLRSwap	=	false;

	if( m_FXCurUseParam == FXPARAM_FACING_LEFT )
	{
		bLRSwap	=	true;
		(*m_ppCurImage)->SetRotation( -m_fCurRotateDegree );
	}
	//else if( m_FXCurUseParam == FXPARAM_FACING_RIGHT )
	//{
	//	bLRSwap	=	false;
	//	(*m_ppCurImage)->SetRotation( m_fCurRotateDegree );
	//}

	(*m_ppCurImage)->SetLRSwap( bLRSwap );

	if( m_bLRSwap == true )
		(*m_ppCurImage)->ToggleLRSwap();

	int	iPosX , iPosY;

	if( m_bScreenRender == TRUE )
	{
		iPosX	=	abs( m_RectAdjust.left );
		iPosY	=	m_RectAdjust.top;
	}
	else
	{
		iPosX	=	(int)( m_fPosX - m_fCamX + m_RectAdjust.left );
		iPosY	=	(int)( m_fPosY - m_fCamY + m_RectAdjust.top	- FX_Y_ADJUST );
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	클리핑 체크
	//

	if( IsClipping( iPosX , iPosY , m_RectSrc.right - m_RectSrc.left , m_RectSrc.bottom - m_RectSrc.top , m_fScaleX , m_fScaleY ) == true )
	{
		RenderReset( fTime );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	(*m_ppCurImage)->RenderTextureImmediateCenterBase( iPosX , iPosY );

	//////////////////////////////////////////////////////////////////////////

	RenderReset( fTime );

	return true;
}
