// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-11-9    12:00 
//***************************************************************************

#include "SPCommon.h"

#include "SPComboBoxDEF.h"
#include "SPComboBoxItemBase.h"

//////////////////////////////////////////////////////////////////////////

SPComboBoxItemBase::SPComboBoxItemBase()
{
	Init();
}

SPComboBoxItemBase::~SPComboBoxItemBase()
{
	Clean();
}

void SPComboBoxItemBase::Init()
{
	m_bShow					=	TRUE;
	m_eStatus				=	COMBOBOX_DATA_TEXTURE_ENABLE;
	m_pstRenderInfo			=	NULL;
	m_ptSize.x				=	0;
	m_ptSize.y				=	0;

	m_strText.clear();
	m_rcTextPos.left		=	0;
	m_rcTextPos.top			=	0;
	m_rcTextPos.right		=	0;
	m_rcTextPos.bottom		=	0;
	m_stTextColor			=	D3DXCOLOR( 0.0f , 0.0f , 0.0f , 1.0f );
	m_bTextShadow			=	false;
	m_dwTextAlign			=	DT_CENTER | DT_VCENTER;
	m_rcTextMargin.left		=	0;
	m_rcTextMargin.top		=	0;
	m_rcTextMargin.right	=	0;
	m_rcTextMargin.bottom	=	0;
}

void SPComboBoxItemBase::Clean()
{
}

void SPComboBoxItemBase::Process( float fTime )
{
	if( IsProcess() == FALSE )
		return;

	m_rcTextPos.left	=	m_pstRenderInfo->m_rcRenderPos.left		+	m_rcTextMargin.left;
	m_rcTextPos.top		=	m_pstRenderInfo->m_rcRenderPos.top		+	m_rcTextMargin.top;
	m_rcTextPos.right	=	m_pstRenderInfo->m_rcRenderPos.right	-	m_rcTextMargin.right;
	m_rcTextPos.bottom	=	m_pstRenderInfo->m_rcRenderPos.bottom	-	m_rcTextMargin.bottom;
}

void SPComboBoxItemBase::Render( float fTime )
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

	//switch( m_pstRenderInfo->m_eStatus )
	//{
	//	case COMBOBOX_DATA_TEXTURE_HIT:
	//	case COMBOBOX_DATA_TEXTURE_PUSH:
	//		pFont->SetShadow( true );
	//		break;
	//	case COMBOBOX_DATA_TEXTURE_DISABLE:
	//		pFont->SetShadow( false );
	//		break;
	//	case COMBOBOX_DATA_TEXTURE_ENABLE:
	//		pFont->SetShadow( m_bTextShadow );
	//		break;
	//}

	pFont->SetShadow( false );

	pFont->RenderText( m_strText.c_str() , &m_rcTextPos , m_dwTextAlign );

	pFont->SetColor( INITCOLOR );
	pFont->SetShadow( bFontShadow );

	g_pVideo->Flush();
}

void SPComboBoxItemBase::Show()
{
	m_bShow	=	TRUE;
}

void SPComboBoxItemBase::Hide()
{
	m_bShow	=	FALSE;
}

BOOL SPComboBoxItemBase::IsProcess()
{
	if( m_pstRenderInfo == NULL || m_bShow == FALSE )
		return FALSE;

	return TRUE;
}

BOOL SPComboBoxItemBase::IsRender()
{
	if( m_strText.empty() == true || m_pstRenderInfo == NULL || m_bShow == FALSE )
		return FALSE;

	return TRUE;
}

void SPComboBoxItemBase::SetStatus( COMBOBOX_DATA_TEXTURE_LABEL eStatus )
{
	m_eStatus	=	eStatus;
}

void SPComboBoxItemBase::SetRenderInfo( CBB_RENDER_INFO* pstRenderStatus )
{
	m_pstRenderInfo	=	pstRenderStatus;
}

void SPComboBoxItemBase::SetSize( int iX , int iY )
{
	m_ptSize.x	=	iX;
	m_ptSize.y	=	iY;
}

POINT SPComboBoxItemBase::GetSize()
{
	return m_ptSize;
}

COMBOBOX_DATA_TEXTURE_LABEL SPComboBoxItemBase::GetStatus()
{
	return m_eStatus;
}

CBB_RENDER_INFO* SPComboBoxItemBase::GetRenderInfo()
{
	return m_pstRenderInfo;
}

void SPComboBoxItemBase::SetText( const char* strText )
{
	if( strText == NULL )
	{
		m_strText.clear();
		return;
	}

	m_strText	=	strText;
}

void SPComboBoxItemBase::SetTextColor( D3DXCOLOR stColor )
{
	m_stTextColor	=	stColor;
}

void SPComboBoxItemBase::SetTextShadow( bool bShadow )
{
	m_bTextShadow	=	bShadow;
}

void SPComboBoxItemBase::SetTextAlign( DWORD dwAlign )
{
	m_dwTextAlign	=	dwAlign;
}

const char* SPComboBoxItemBase::GetText()
{
	return m_strText.c_str();
}
