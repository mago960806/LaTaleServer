// ***************************************************************
//  SPTreeBoxItemBase.cpp   version:  1.0   date:2006/01/20
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

//////////////////////////////////////////////////////////////////////////

SPTreeBoxItemBase::SPTreeBoxItemBase()
{
	Init();
}

SPTreeBoxItemBase::~SPTreeBoxItemBase()
{
	Clear();
}

void SPTreeBoxItemBase::Init()
{
	m_dwStatus			=	DATA_STATUS_ENABLE;
	m_ptSize.x			=	0;
	m_ptSize.y			=	0;
	m_rcPos.left		=	0;
	m_rcPos.top			=	0;
	m_rcPos.right		=	0;
	m_rcPos.bottom		=	0;
	m_strText.clear();

	for( int i = 0 ; i < DATA_STRING_COUNT ; i++ )
	{
		SetTextColor( (TREEBOX_DATA_STRING_LABEL)i , INITCOLOR );
	}

	m_bBold				=	false;
	m_ptMargin.x		=	0;
	m_ptMargin.y		=	0;
	m_dwAlign			=	DT_LEFT | DT_VCENTER;
	m_eFontEnumerate	=	FONT_12_NORMAL;
	m_iReturnValue		=	0;
	m_bTextPeriod		=	false;
}

void SPTreeBoxItemBase::Clear()
{

}

void SPTreeBoxItemBase::Process( float fTime )
{
}

void SPTreeBoxItemBase::Render( float fTime )
{
	if( g_pVideo == NULL )
		return;

	SPFont*	pFont	=	g_pVideo->GetFont( m_eFontEnumerate );

	if( pFont == NULL )
		return;

	TREEBOX_DATA_STRING_LABEL	eStatus	=	DATA_STRING_DISABLE;

	if( GetStatus( DATA_STATUS_DISABLE ) == true )
	{
		eStatus	=	DATA_STRING_DISABLE;
	}
	else if( GetStatus( DATA_STATUS_PUSH ) == true )
	{
		eStatus	=	DATA_STRING_PUSH;
	}
	else if( GetStatus( DATA_STATUS_HIT ) == true )
	{
		eStatus	=	DATA_STRING_HIT;
	}
	else if( GetStatus( DATA_STATUS_SELECT ) == true )
	{
		eStatus	=	DATA_STRING_SELECT;
	}
	else if( GetStatus( DATA_STATUS_ENABLE ) == true )
	{
		eStatus	=	DATA_STRING_ENABLE;
	}

	int	iStringColorNumber	=	(int)eStatus * 2;

	if( GetStatus( DATA_STATUS_PULLDOWN ) == true )
		iStringColorNumber++;

	g_pVideo->Flush();
	pFont->SetColor( m_cTextColor[ iStringColorNumber ] );
	pFont->SetShadow( false );
	pFont->SetPeriod( m_bTextPeriod );

	if( m_bBold == true )
	{
		RECT	rcPos	=	m_rcPos;
		rcPos.left++;
		pFont->RenderText( m_strText.c_str() , &rcPos , m_dwAlign );
		g_pVideo->Flush();
	}

	pFont->RenderText( m_strText.c_str() , &m_rcPos , m_dwAlign );
	g_pVideo->Flush();

	pFont->SetPeriod( false );
	pFont->SetShadow( true );
	pFont->SetColor();

	//g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ) );
	//g_pVideo->GetSysLine()->DrawRect( m_rcPos );
	//g_pVideo->GetSysLine()->SetColor();
}

int SPTreeBoxItemBase::GetNextPosX()
{
	return	m_rcPos.left;
}

int SPTreeBoxItemBase::GetNextPosY()
{
	return	m_rcPos.bottom;
}

bool SPTreeBoxItemBase::IsCursorIn( int iX, int iY )
{
	if( m_rcPos.left	<=	iX	&&	m_rcPos.right	>=	iX &&
		m_rcPos.top		<=	iY	&&	m_rcPos.bottom	>=	iY )
	{
		return true;
	}

	return false;
}

void SPTreeBoxItemBase::SetPos( int iX , int iY )
{
	m_rcPos.left	=	iX	+	m_ptMargin.x;
	m_rcPos.top		=	iY	+	m_ptMargin.y;
	m_rcPos.right	=	m_rcPos.left	+	m_ptSize.x;
	m_rcPos.bottom	=	m_rcPos.top		+	m_ptSize.y;
}

RECT SPTreeBoxItemBase::GetPosRect()
{
	return m_rcPos;
}

void SPTreeBoxItemBase::SetSize( int iX , int iY )
{
	m_ptSize.x	=	iX;
	m_ptSize.y	=	iY;
}

POINT SPTreeBoxItemBase::GetSize()
{
	return m_ptSize;
}

void SPTreeBoxItemBase::SetStatus( TREEBOX_DATA_STATUS_LABEL eStatus )
{
	m_dwStatus	=	eStatus;
}

void SPTreeBoxItemBase::AddStatus( DWORD dwStatus )
{
	m_dwStatus	|=	dwStatus;
}

void SPTreeBoxItemBase::DelStatus( DWORD dwStatus )
{
	if( GetStatus( dwStatus ) == true )
		m_dwStatus	^=	dwStatus;
}

bool SPTreeBoxItemBase::GetStatus( DWORD dwStatus )
{
	return ( ( m_dwStatus & dwStatus ) == dwStatus );
}

DWORD SPTreeBoxItemBase::GetStatus()
{
	return m_dwStatus;
}

void SPTreeBoxItemBase::SetText( const char* pString )
{
	m_strText	=	pString;
}

const char* SPTreeBoxItemBase::GetText()
{
	return m_strText.c_str();
}

void SPTreeBoxItemBase::SetTextColor( TREEBOX_DATA_STRING_LABEL eLabel , D3DXCOLOR cColor )
{
	SetTextColor( (int)eLabel , cColor );
}

void SPTreeBoxItemBase::SetTextColor( int iLabel , D3DXCOLOR cColor )
{
	if( iLabel < 0 || iLabel >= DATA_STRING_COUNT )
		return;

	m_cTextColor[ iLabel ]	=	cColor;
}

void SPTreeBoxItemBase::SetBold( bool bBold )
{
	m_bBold	=	bBold;
}

void SPTreeBoxItemBase::SetTextMargin( int iX , int iY )
{
	SetPos( m_rcPos.left - m_ptMargin.x * 2 , m_rcPos.top - m_ptMargin.y * 2 );

	m_ptMargin.x	=	iX;
	m_ptMargin.y	=	iY;

	SetPos( m_rcPos.left , m_rcPos.top );
}

void SPTreeBoxItemBase::SetAlign( DWORD dwAlign )
{
	m_dwAlign	=	dwAlign;
}

DWORD SPTreeBoxItemBase::GetAlign()
{
	return m_dwAlign;
}

void SPTreeBoxItemBase::SetFontEnumerate( FONT_ENUMERATE eFontEnumerate )
{
	m_eFontEnumerate	=	eFontEnumerate;
}

FONT_ENUMERATE SPTreeBoxItemBase::GetFontEnumerate()
{
	return m_eFontEnumerate;
}

void SPTreeBoxItemBase::SetReturnValue( int iValue )
{
	m_iReturnValue	=	iValue;
}

int SPTreeBoxItemBase::GetReturnValue()
{
	return m_iReturnValue;
}

void SPTreeBoxItemBase::SetTextPeriod( bool bPeriod )
{
	m_bTextPeriod	=	bPeriod;
}

bool SPTreeBoxItemBase::GetTextPeriod()
{
	return m_bTextPeriod;
}