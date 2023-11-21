// ***************************************************************
//  SPTitleRenderBase   version:  1.0   ¡¤  date: 03/30/2006
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

//////////////////////////////////////////////////////////////////////////

SPTitleRenderBase::SPTitleRenderBase()
{
	Init();
}

SPTitleRenderBase::~SPTitleRenderBase()
{
	Clean();
}

void SPTitleRenderBase::Init()
{
	m_bShow					=	TRUE;
//	m_eStatus				=	TITLE_STATUS_ENABLE;
	m_ptPos.x				=	0;
	m_ptPos.y				=	0;
	m_ptSize.x				=	0;
	m_ptSize.y				=	0;

	m_strText.clear();
	m_rcTextPos.left		=	0;
	m_rcTextPos.top			=	0;
	m_rcTextPos.right		=	0;
	m_rcTextPos.bottom		=	0;
	m_stTextColor			=	D3DXCOLOR( 0.0f , 0.0f , 0.0f , 1.0f );
	m_bTextShadow			=	false;
	m_dwTextAlign			=	DT_CENTER | DT_VCENTER | DT_NOCLIP;
	m_rcTextMargin.left		=	0;
	m_rcTextMargin.top		=	0;
	m_rcTextMargin.right	=	0;
	m_rcTextMargin.bottom	=	0;
}

void SPTitleRenderBase::Clean()
{
}

void SPTitleRenderBase::Process( float fTime )
{
	if( IsProcess() == FALSE )
		return;

	m_rcTextPos.left	=	m_ptPos.x	+	m_rcTextMargin.left;
	m_rcTextPos.top		=	m_ptPos.y	+	m_rcTextMargin.top;
	m_rcTextPos.right	=	m_ptPos.x	+	m_ptSize.x	-	m_rcTextMargin.right;
	m_rcTextPos.bottom	=	m_ptPos.y	+	m_ptSize.y	-	m_rcTextMargin.bottom;
}

void SPTitleRenderBase::Render( float fTime )
{
	if( IsRender() == FALSE )
		return;

	if( g_pVideo == NULL )
		return;

	SPFont*	pFont		=	g_pVideo->GetFont(FONT_12_NORMAL);
	if( pFont == NULL )
		return;

	bool	bFontShadow	=	pFont->IsShadow();

	pFont->SetColor( m_stTextColor );

	//switch( GetStatus() )
	//{
	//	case TITLE_STATUS_SHADOW:
	//		pFont->SetShadow( true );
	//		break;
	//	case DATA_TEXTURE_DISABLE:
	//		pFont->SetShadow( false );
	//		break;
	//	case DATA_TEXTURE_ENABLE:
	//		pFont->SetShadow( m_bTextShadow );
	//		break;
	//}

	pFont->RenderText( m_strText.c_str() , &m_rcTextPos , m_dwTextAlign );

	pFont->SetColor( INITCOLOR );
	pFont->SetShadow( bFontShadow );

	g_pVideo->Flush();
}

void SPTitleRenderBase::Show()
{
	m_bShow	=	TRUE;
}

void SPTitleRenderBase::Hide()
{
	m_bShow	=	FALSE;
}

BOOL SPTitleRenderBase::IsProcess()
{
	if( m_bShow == FALSE )
		return FALSE;

	return TRUE;
}

BOOL SPTitleRenderBase::IsRender()
{
	if( m_strText.empty() == true || m_bShow == FALSE )
		return FALSE;

	return TRUE;
}

//void SPTitleRenderBase::SetStatus( TITLE_STATUS eStatus )
//{
//	m_eStatus	=	eStatus;
//}

void SPTitleRenderBase::SetPos( int iX , int iY )
{
	m_ptPos.x	=	iX;
	m_ptPos.y	=	iY;
}

void SPTitleRenderBase::SetSize( int iX , int iY )
{
	m_ptSize.x	=	iX;
	m_ptSize.y	=	iY;
}

POINT SPTitleRenderBase::GetSize()
{
	return m_ptSize;
}

void SPTitleRenderBase::SetID( UINT32 uiID )
{
	m_uiID	=	uiID;
}

UINT32 SPTitleRenderBase::GetID()
{
	return m_uiID;
}

//TITLE_STATUS SPTitleRenderBase::GetStatus()
//{
//	return m_eStatus;
//}

void SPTitleRenderBase::SetText( const char* strText )
{
	m_strText	=	strText;
}

void SPTitleRenderBase::SetTextColor( D3DXCOLOR stColor )
{
	m_stTextColor	=	stColor;
}

void SPTitleRenderBase::SetTextShadow( bool bShadow )
{
	m_bTextShadow	=	bShadow;
}

void SPTitleRenderBase::SetTextAlign( DWORD dwAlign )
{
	m_dwTextAlign	=	dwAlign;
}

void SPTitleRenderBase::SetTextMargin( int iLeft , int iTop , int iRight , int iBottom )
{
	SetRect( &m_rcTextMargin , iLeft , iTop , iRight , iBottom );
}

const char* SPTitleRenderBase::GetText()
{
	return m_strText.c_str();
}
