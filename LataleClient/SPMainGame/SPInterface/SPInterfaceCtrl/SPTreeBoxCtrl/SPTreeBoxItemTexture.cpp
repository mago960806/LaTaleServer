// ***************************************************************
//  SPTreeBoxItemTexture.cpp   version:  1.0   date:2006/01/20
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 AJJIYA - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"

#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"

#include "SPTreeBoxItemTexture.h"

//////////////////////////////////////////////////////////////////////////

SPTreeBoxItemTexture::SPTreeBoxItemTexture()
{
	Init();
}

SPTreeBoxItemTexture::~SPTreeBoxItemTexture()
{
	Clear();
}

void SPTreeBoxItemTexture::Init()
{
	for( int i = 0 ; i < DATA_TEXTURE_COUNT ; ++i )
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

	GetTextureLabel();
}

void SPTreeBoxItemTexture::Clear()
{
	for( int i = 0 ; i < DATA_TEXTURE_COUNT ; ++i )
	{
		SAFE_RELEASE( m_pTexture[ i ] );
	}
}

void SPTreeBoxItemTexture::Process( float fTime )
{
	SPTreeBoxItemBase::Process( fTime );
}

void SPTreeBoxItemTexture::Render( float fTime )
{
	if( m_pTexture[ m_eTextureLabel ] != NULL )
	{
		m_pTexture[ m_eTextureLabel ]->RenderTexture( &( m_rcTexturePos[ m_eTextureLabel ] ) , &( m_rcTextureSrc[ m_eTextureLabel ] ) );
		g_pVideo->Flush();
	}

	SPTreeBoxItemBase::Render( fTime );
}

int SPTreeBoxItemTexture::GetNextPosX()
{
	return m_rcTexturePos[ m_eTextureLabel ].left	-	m_rcTextureMargin[ m_eTextureLabel ].left;
}

int SPTreeBoxItemTexture::GetNextPosY()
{
	return m_rcTexturePos[ m_eTextureLabel ].bottom	-	m_rcTextureMargin[ m_eTextureLabel ].bottom;
}

bool SPTreeBoxItemTexture::IsCursorIn( int iX, int iY )
{
	if( m_rcTexturePos[ m_eTextureLabel ].left	<=	iX	&&	m_rcTexturePos[ m_eTextureLabel ].right		>=	iX &&
		m_rcTexturePos[ m_eTextureLabel ].top	<=	iY	&&	m_rcTexturePos[ m_eTextureLabel ].bottom	>=	iY )
	{
		return true;
	}

	return false;
}

void SPTreeBoxItemTexture::SetPos( int iX , int iY )
{
	SPTreeBoxItemBase::SetPos( iX , iY );

	for( int i = 0 ; i < DATA_TEXTURE_COUNT ; i++ )
	{
		m_rcTexturePos[ i ].left		=	iX	+	m_rcTextureMargin[ i ].left;
		m_rcTexturePos[ i ].top			=	iY	+	m_rcTextureMargin[ i ].top;
		m_rcTexturePos[ i ].right		=	m_rcTexturePos[ i ].left	+	GetSize().x;
		m_rcTexturePos[ i ].bottom		=	m_rcTexturePos[ i ].top		+	GetSize().y;
	}
}

RECT SPTreeBoxItemTexture::GetPosRect()
{
	return m_rcTexturePos[ m_eTextureLabel ];
}

void SPTreeBoxItemTexture::SetStatus( TREEBOX_DATA_STATUS_LABEL eStatus )
{
	SPTreeBoxItemBase::SetStatus( eStatus );
	GetTextureLabel();
}

void SPTreeBoxItemTexture::AddStatus( DWORD dwStatus )
{
	SPTreeBoxItemBase::AddStatus( dwStatus );
	GetTextureLabel();
}

void SPTreeBoxItemTexture::DelStatus( DWORD dwStatus )
{
	SPTreeBoxItemBase::DelStatus( dwStatus );
	GetTextureLabel();
}

TREEBOX_DATA_TEXTURE_LABEL SPTreeBoxItemTexture::GetTextureLabel()
{
	TREEBOX_DATA_TEXTURE_LABEL	eTextureLabel	=	DATA_TEXTURE_DISABLE;

	if( GetStatus( DATA_STATUS_DISABLE ) == true )
	{
		eTextureLabel	=	DATA_TEXTURE_DISABLE;
	}
	else if( GetStatus( DATA_STATUS_PUSH ) == true )
	{
		eTextureLabel	=	DATA_TEXTURE_PUSH;
	}
	else if( GetStatus( DATA_STATUS_HIT ) == true )
	{
		eTextureLabel	=	DATA_TEXTURE_HIT;
	}
	else if( GetStatus( DATA_STATUS_SELECT ) == true )
	{
		eTextureLabel	=	DATA_TEXTURE_SELECT;
	}
	else if( GetStatus( DATA_STATUS_ENABLE ) == true )
	{
		eTextureLabel	=	DATA_TEXTURE_ENABLE;
	}

	int	iTextureLabel	=	(int)eTextureLabel	*	2;

	if( GetStatus( DATA_STATUS_PULLDOWN ) == true )
		iTextureLabel++;

	m_eTextureLabel	=	(TREEBOX_DATA_TEXTURE_LABEL)iTextureLabel;
	return	m_eTextureLabel;
}

BOOL SPTreeBoxItemTexture::SetTexture( TREEBOX_DATA_TEXTURE_LABEL eLabel , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY )
{
	return SetTexture( (int)eLabel , strFileName , iSrcSX , iSrcSY , iSrcEX , iSrcEY );
}

BOOL SPTreeBoxItemTexture::SetTexture( int iLabel , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY )
{
	if( iLabel >= DATA_TEXTURE_COUNT )
		return FALSE;

	SPTexture*		pTexture	=	NULL;

	if( g_pVideo->GetTextureMgr()->LoadTexture( strFileName , &pTexture ) == false )
		return FALSE;

	m_rcTextureSrc[ iLabel ].left	=	iSrcSX;
	m_rcTextureSrc[ iLabel ].top	=	iSrcSY;
	m_rcTextureSrc[ iLabel ].right	=	iSrcEX;
	m_rcTextureSrc[ iLabel ].bottom	=	iSrcEY;

	SAFE_RELEASE( m_pTexture[ iLabel ] );

	m_pTexture[ iLabel ]	=	pTexture;

	return TRUE;
}

void SPTreeBoxItemTexture::SetTextureMargin( TREEBOX_DATA_TEXTURE_LABEL eLabel , int iLeft , int iTop , int iRight , int iBottom )
{
	SetTextureMargin( (int)eLabel , iLeft , iTop , iRight , iBottom );
}

void SPTreeBoxItemTexture::SetTextureMargin( int iLabel , int iLeft , int iTop , int iRight , int iBottom )
{
	if( iLabel >= DATA_TEXTURE_COUNT )
		return;

	m_rcTextureMargin[ iLabel ].left	=	iLeft;
	m_rcTextureMargin[ iLabel ].top		=	iTop;
	m_rcTextureMargin[ iLabel ].right	=	iRight;
	m_rcTextureMargin[ iLabel ].bottom	=	iBottom;
}
