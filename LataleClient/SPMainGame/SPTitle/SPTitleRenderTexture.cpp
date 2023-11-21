// ***************************************************************
//  SPTitleRenderTexture   version:  1.0   ¡¤  date: 03/30/2006
//  -------------------------------------------------------------
//  
//	AJJIYA
//
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"

#include "SPTitleRenderDEF.h"
#include "SPTitleRenderBase.h"

#include "SPTitleRenderTexture.h"

//////////////////////////////////////////////////////////////////////////

SPTitleRenderTexture::SPTitleRenderTexture()
{
	Init();
}

SPTitleRenderTexture::~SPTitleRenderTexture()
{
	Clean();
}

void SPTitleRenderTexture::Init()
{
	m_pTexture					=	NULL;

	m_rcTextureSrc.left			=	0;
	m_rcTextureSrc.top			=	0;
	m_rcTextureSrc.right		=	0;
	m_rcTextureSrc.bottom		=	0;

	m_rcTexturePos.left			=	0;
	m_rcTexturePos.top			=	0;
	m_rcTexturePos.right		=	0;
	m_rcTexturePos.bottom		=	0;

	m_rcTextureMargin.left		=	0;
	m_rcTextureMargin.top		=	0;
	m_rcTextureMargin.right		=	0;
	m_rcTextureMargin.bottom	=	0;
}

void SPTitleRenderTexture::Clean()
{
	SAFE_RELEASE( m_pTexture );
}

void SPTitleRenderTexture::Process( float fTime )
{
	if( IsProcess() == FALSE )
		return;

	SPTitleRenderBase::Process( fTime );

	m_rcTexturePos.left		=	m_ptPos.x	+	m_rcTextureMargin.left;
	m_rcTexturePos.top		=	m_ptPos.y	+	m_rcTextureMargin.top;
	m_rcTexturePos.right	=	m_ptPos.x	+	m_ptSize.x	-	m_rcTextureMargin.right;
	m_rcTexturePos.bottom	=	m_ptPos.y	+	m_ptSize.y	-	m_rcTextureMargin.bottom;
}

void SPTitleRenderTexture::Render( float fTime )
{
	if( IsRender() == FALSE )
		return;

	if( m_pTexture != NULL )
	{
		m_pTexture->RenderTexture( &( m_rcTexturePos ) , &( m_rcTextureSrc ) );
		g_pVideo->Flush();
	}

	SPTitleRenderBase::Render( fTime );
}

BOOL SPTitleRenderTexture::SetTexture( const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY )
{
	SPTexture*		pTexture	=	NULL;

	if( g_pVideo->GetTextureMgr()->LoadTexture( strFileName , &pTexture ) == false )
		return FALSE;

	m_rcTextureSrc.left		=	iSrcSX;
	m_rcTextureSrc.top		=	iSrcSY;
	m_rcTextureSrc.right	=	iSrcEX;
	m_rcTextureSrc.bottom	=	iSrcEY;

	SAFE_RELEASE( m_pTexture );

	m_pTexture	=	pTexture;

	return TRUE;
}

void SPTitleRenderTexture::SetTextureMargin( int iLeft , int iTop , int iRight , int iBottom )
{
	m_rcTextureMargin.left		=	iLeft;
	m_rcTextureMargin.top		=	iTop;
	m_rcTextureMargin.right		=	iRight;
	m_rcTextureMargin.bottom	=	iBottom;
}
