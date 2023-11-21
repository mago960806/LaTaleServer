// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-11-9    12:01 
//***************************************************************************

#include "SPCommon.h"

#include "SPComboBoxDEF.h"
#include "SPComboBoxItemBase.h"

#include "SPComboBoxItemTexture.h"

//////////////////////////////////////////////////////////////////////////

SPComboBoxItemTexture::SPComboBoxItemTexture()
{
	Init();
}

SPComboBoxItemTexture::~SPComboBoxItemTexture()
{
	Clean();
}

void SPComboBoxItemTexture::Init()
{
	for( int i = 0 ; i < COMBOBOX_DATA_TEXTURE_COUNT ; i++ )
	{
		m_pTexture[ i ]					=	NULL;

		m_rcTextureSrc[ i ].left		=	0;
		m_rcTextureSrc[ i ].top			=	0;
		m_rcTextureSrc[ i ].right		=	0;
		m_rcTextureSrc[ i ].bottom		=	0;

		m_rcTexturePos[ i ].left		=	0;
		m_rcTexturePos[ i ].top			=	0;
		m_rcTexturePos[ i ].right		=	0;
		m_rcTexturePos[ i ].bottom		=	0;

		m_rcTextureMargin[ i ].left		=	0;
		m_rcTextureMargin[ i ].top		=	0;
		m_rcTextureMargin[ i ].right	=	0;
		m_rcTextureMargin[ i ].bottom	=	0;
	}
}

void SPComboBoxItemTexture::Clean()
{
	for( int i = 0 ; i < COMBOBOX_DATA_TEXTURE_COUNT ; i++ )
	{
		SAFE_RELEASE( m_pTexture[ i ] );
	}
}

void SPComboBoxItemTexture::Process( float fTime )
{
	if( IsProcess() == FALSE )
		return;

	SPComboBoxItemBase::Process( fTime );

	for( int i = 0 ; i < COMBOBOX_DATA_TEXTURE_COUNT ; i++ )
	{
		m_rcTexturePos[ i ].left		=	m_pstRenderInfo->m_rcRenderPos.left		+ m_rcTextureMargin[ i ].left;
		m_rcTexturePos[ i ].top			=	m_pstRenderInfo->m_rcRenderPos.top		+ m_rcTextureMargin[ i ].top;
		m_rcTexturePos[ i ].right		=	m_pstRenderInfo->m_rcRenderPos.right	- m_rcTextureMargin[ i ].right;
		m_rcTexturePos[ i ].bottom		=	m_pstRenderInfo->m_rcRenderPos.bottom	- m_rcTextureMargin[ i ].bottom;
	}
}

void SPComboBoxItemTexture::Render( float fTime )
{
	if( IsRender() == FALSE )
		return;

	if( m_pTexture[ COMBOBOX_DATA_TEXTURE_ENABLE ] != NULL )
	{
		m_pTexture[ COMBOBOX_DATA_TEXTURE_ENABLE ]->RenderTexture( &( m_rcTexturePos[ COMBOBOX_DATA_TEXTURE_ENABLE ] ) , &( m_rcTextureSrc[ COMBOBOX_DATA_TEXTURE_ENABLE ] ) );
		g_pVideo->Flush();
	}

	if( m_eStatus != COMBOBOX_DATA_TEXTURE_ENABLE )
	{
		if( m_pTexture[ m_eStatus ] != NULL )
		{
			m_pTexture[ m_eStatus ]->RenderTexture( &( m_rcTexturePos[ m_eStatus ] ) , &( m_rcTextureSrc[ m_eStatus ] ) );
			g_pVideo->Flush();
		}
	} else if( m_pstRenderInfo->m_eStatus != COMBOBOX_DATA_TEXTURE_ENABLE )
	{
		if( m_pTexture[ m_pstRenderInfo->m_eStatus ] != NULL )
		{
			m_pTexture[ m_pstRenderInfo->m_eStatus ]->RenderTexture( &( m_rcTexturePos[ m_pstRenderInfo->m_eStatus ] ) , &( m_rcTextureSrc[ m_pstRenderInfo->m_eStatus ] ) );
			g_pVideo->Flush();
		}
	}

	SPComboBoxItemBase::Render( fTime );
}

BOOL SPComboBoxItemTexture::SetTexture( COMBOBOX_DATA_TEXTURE_LABEL eLabel , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY )
{
	int	iTextureType	=	(int)eLabel;

	if( iTextureType >= COMBOBOX_DATA_TEXTURE_COUNT )
		return FALSE;

	SPTexture*		pTexture	=	NULL;

	if( g_pVideo->GetTextureMgr()->LoadTexture( strFileName , &pTexture ) == false )
		return FALSE;

	m_rcTextureSrc[ iTextureType ].left		=	iSrcSX;
	m_rcTextureSrc[ iTextureType ].top		=	iSrcSY;
	m_rcTextureSrc[ iTextureType ].right	=	iSrcEX;
	m_rcTextureSrc[ iTextureType ].bottom	=	iSrcEY;

	SAFE_RELEASE( m_pTexture[ iTextureType ] );

	m_pTexture[ iTextureType ]	=	pTexture;

	return TRUE;
}

void SPComboBoxItemTexture::SetTextureMargin( COMBOBOX_DATA_TEXTURE_LABEL eLabel , int iLeft , int iTop , int iRight , int iBottom )
{
	int	iTextureType	=	(int)eLabel;

	if( iTextureType >= COMBOBOX_DATA_TEXTURE_COUNT )
		return;

	m_rcTextureMargin[ iTextureType ].left		=	iLeft;
	m_rcTextureMargin[ iTextureType ].top		=	iTop;
	m_rcTextureMargin[ iTextureType ].right		=	iRight;
	m_rcTextureMargin[ iTextureType ].bottom	=	iBottom;
}
