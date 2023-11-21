// ***************************************************************
//  SPTitleAttr   version:  1.0   ¡¤  date: 03/29/2006
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

#include "SPTitleAttr.h"

SPTitleAttr::SPTitleAttr(void)
{
	Init();
}

SPTitleAttr::~SPTitleAttr(void)
{
}

//////////////////////////////////////////////////////////////////////////
//
//

void SPTitleAttr::Init()
{
	m_uiID	=	0;
	m_strName.clear();
	m_strDesc.clear();
	m_eType	=	TITLE_TYPE_NULL;
	m_strBGTexture.clear();
	SetRect( &m_rcSrc , 0 , 0 , 0 , 0 );

	m_dwAlign	=	DT_LEFT	| DT_VCENTER | DT_NOCLIP;
	m_iMargin	=	0;

	for( int i = 0 ; i < TITLE_STATUS_COUNT ; ++i )
	{
		m_dwStatusType[ i ]	=	0;
		m_iStatusValue[ i ]	=	0;
	}

	for( i = 0 ; i < TITLE_EFFECT_COUNT ; ++i )
	{
		m_dwEffect[ i ]	=	0;
		m_strEffectDesc[ i ].clear();
	}

	m_FontColor	=	D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
}

void SPTitleAttr::SetID( UINT32 uiID )
{
	m_uiID	=	uiID;
}

void SPTitleAttr::SetName( const char* pstrName )
{
	if( pstrName == NULL )
		return;

	m_strName	=	pstrName;
}

void SPTitleAttr::SetDesc( const char* pstrDesc )
{
	if( pstrDesc == NULL )
		return;

	m_strDesc	=	pstrDesc;
}

void SPTitleAttr::SetType( int iType )
{
	m_eType	=	(TITLE_TYPE)iType;
}

void SPTitleAttr::SetBGTexture( const char* pstrBGTexture )
{
	if( pstrBGTexture == NULL )
		return;

	m_strBGTexture	=	pstrBGTexture;
}

void SPTitleAttr::SetSize( int iX , int iY )
{
	m_ptSize.x	=	iX;
	m_ptSize.y	=	iY;
}

void SPTitleAttr::SetSrcRect( int iLeft , int iTop , int iRight , int iBottom )
{
	SetRect( &m_rcSrc , iLeft , iTop , iRight , iBottom );
}

void SPTitleAttr::SetAlign( DWORD dwAlign )
{
	m_dwAlign	=	dwAlign;
}

void SPTitleAttr::SetMargin( int iMargin )
{
	m_iMargin	=	iMargin;
}

void SPTitleAttr::SetVMargin( int iMargin )
{
	m_iVMargin	=	iMargin;
}

void SPTitleAttr::SetStatusType( int iNumber , DWORD dwType , int iValue )
{
	if( iNumber < 0 || iNumber >= TITLE_STATUS_COUNT )
		return;

	m_dwStatusType[ iNumber ]	=	dwType;
	m_iStatusValue[ iNumber ]	=	iValue;
}

void SPTitleAttr::SetEffect( int iNumber , DWORD dwEffect , const char* pstrDesc )
{
	if( iNumber < 0 || iNumber >= TITLE_EFFECT_COUNT )
		return;

	m_dwEffect[ iNumber ]		=	dwEffect;
	m_strEffectDesc[ iNumber ]	=	pstrDesc;
}

void SPTitleAttr::SetFontColor( int iR , int iG , int iB )
{
	m_FontColor	=	D3DXCOLOR((iR / 255.0f), (iG / 255.0f), (iB / 255.0f), 1.0f);
}

UINT32 SPTitleAttr::GetID()
{
	return m_uiID;
}

const char* SPTitleAttr::GetName()
{
	return m_strName.c_str();
}

const char* SPTitleAttr::GetDesc()
{
	return m_strDesc.c_str();
}

SPTitleAttr::TITLE_TYPE SPTitleAttr::GetType()
{
	return m_eType;
}

const char* SPTitleAttr::GetBGTexture()
{
	return m_strBGTexture.c_str();
}

POINT SPTitleAttr::GetSize()
{
	return m_ptSize;
}

RECT SPTitleAttr::GetSrcRect()
{
	return m_rcSrc;
}

DWORD SPTitleAttr::GetAlign()
{
	return m_dwAlign;
}

int SPTitleAttr::GetMargin()
{
	return m_iMargin;
}

int SPTitleAttr::GetVMargin()
{
	return m_iVMargin;
}

DWORD SPTitleAttr::GetStatusType( int iNumber )
{
	if( iNumber < 0 || iNumber >= TITLE_STATUS_COUNT )
		return NULL;

	return	m_dwStatusType[ iNumber ];
}

int SPTitleAttr::GetStatusTypeValue( int iNumber )
{
	if( iNumber < 0 || iNumber >= TITLE_STATUS_COUNT )
		return NULL;

	return	m_iStatusValue[ iNumber ];
}

DWORD SPTitleAttr::GetEffect( int iNumber )
{
	if( iNumber < 0 || iNumber >= TITLE_EFFECT_COUNT )
		return NULL;

	return	m_dwEffect[ iNumber ];
}

const char* SPTitleAttr::GetEffectDesc( int iNumber )
{
	if( iNumber < 0 || iNumber >= TITLE_EFFECT_COUNT )
		return NULL;

	return m_strEffectDesc[ iNumber ].c_str();
}

D3DXCOLOR SPTitleAttr::GetFontColor()
{
	return	m_FontColor;
}
