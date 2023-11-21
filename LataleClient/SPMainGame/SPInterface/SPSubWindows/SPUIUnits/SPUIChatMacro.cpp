// ***************************************************************
//  SPUIChatMacro   version:  1.0   ·  date: 08/09/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"
#include "SPUIUnit.h"

#include "SPItemCluster.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBManager.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "SPItemAttr.h"
#include "SPItem.h"

#include "SPWindowDEF.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPUIChatMacro.h"

SPUIChatMacro::SPUIChatMacro()
{
	Init();
}

SPUIChatMacro::~SPUIChatMacro()
{
	Clear();
}

//////////////////////////////////////////////////////////////////////////
//
//	기본 인터페이스
//

void SPUIChatMacro::Init()
{
	SPUIUnit::Init();

	m_pChatMacro				=	NULL;
	m_pChatMacroLDT				=	NULL;

	m_rtTextDest.left			=	0;
	m_rtTextDest.top			=	0;
	m_rtTextDest.right			=	0;
	m_rtTextDest.bottom			=	0;

	m_cTextRGBA.r				=	1.0f;
	m_cTextRGBA.g				=	1.0f;
	m_cTextRGBA.b				=	1.0f;
	m_cTextRGBA.a				=	1.0f;

	m_dwTextAlign				=	0;
	m_bTextFontShadow			=	FALSE;
	m_bTextPeriod				=	FALSE;

	SetFont( FONT_12_NORMAL );
}

void SPUIChatMacro::Clear()
{
	SPUIUnit::Clear();
}

void SPUIChatMacro::Process( float fTime )
{
	SPUIUnit::Process( fTime );
}

void SPUIChatMacro::Render()
{
	if( IsTexture() == FALSE || IsRender() == FALSE || g_pVideo == NULL )
		return;

	g_pVideo->Flush();

	SPUIUnit::Render();

	if( IsText() == TRUE )
	{
		bool	bIsShadow	=	m_pTextFont->IsShadow();

		m_pTextFont->SetColor( m_cTextRGBA );
		m_pTextFont->SetShadow(	(bool)m_bTextFontShadow );	
		m_pTextFont->SetPeriod( (bool)m_bTextPeriod );

		m_pTextFont->RenderText( m_strText.c_str() , &m_rtTextDest , m_dwTextAlign );
		g_pVideo->Flush();

		m_pTextFont->SetColor();
		m_pTextFont->SetShadow(	bIsShadow );
		m_pTextFont->SetPeriod( false );
	}
}

BOOL SPUIChatMacro::Use()
{
	if( SPUIUnit::Use() == FALSE )
		return FALSE;

	if( m_pChatMacro == NULL || m_pChatMacroLDT == NULL )
		return FALSE;

	if( m_pChatMacroLDT->m_eType == CHAT_MACRO_TYPE_NORMAL )
	{
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHAT , SPIM_WNDCHAT_SEND_MESSAGE , (WPARAM)m_pChatMacro->m_strContents.c_str() , NULL );
		return TRUE;
	}
	else if( m_pChatMacroLDT->m_eType == CHAT_MACRO_TYPE_SPECIAL )
	{
		if( g_pGOBManager == NULL || g_pItemCluster == NULL )
			return FALSE;

		SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer == NULL )
			return FALSE;

		SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();

		if( pInvenArchive == NULL )
			return FALSE;

		SPItemAttr*	pItemAttr	=	NULL;

		if( g_pItemCluster->GetItemInfo( m_pChatMacroLDT->m_iRequireItem , pItemAttr ) == false )
			return FALSE;

		CONTAINER_TYPE iContainer = (CONTAINER_TYPE)( pItemAttr->m_eType - 1 );

		SPItem*	pRequireItem	=	pInvenArchive->GetSPItem( iContainer , m_pChatMacroLDT->m_iRequireItem );

		if( pRequireItem == NULL )
			return FALSE;

		if( pInvenArchive->SetMicItem( pRequireItem->GetContainerType() , pRequireItem->GetSlotIndex() ) == false )
			return FALSE;

		if( pInvenArchive->SendMicItemUseMsg( m_pChatMacro->m_strContents.c_str() ) == true )
			return TRUE;

		pInvenArchive->ClearMicItem();
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//
//	자기 자신 복사 인터페이스
//

SPUIUnit* SPUIChatMacro::ClonePtr()
{
	return new SPUIChatMacro;
}

BOOL SPUIChatMacro::Copy( SPUIUnit** ppDestUIUnit )
{
	SPUIChatMacro*	pUIChatMacoUnit	=	(SPUIChatMacro*)(*ppDestUIUnit);

	if( pUIChatMacoUnit == NULL )
		return FALSE;

	if( SPUIUnit::Copy( ppDestUIUnit ) == FALSE )
		return FALSE;

	pUIChatMacoUnit->SetChatMacro( GetChatMacro() );
	pUIChatMacoUnit->SetChatMacroLDT( GetChatMacroLDT() );

	pUIChatMacoUnit->SetTextColor( GetTextColor() );
	pUIChatMacoUnit->SetTextAlign( GetTextAlign() );
	pUIChatMacoUnit->SetFontShadow( GetFontShadow() );
	pUIChatMacoUnit->SetPeriod( GetPeriod() );
	pUIChatMacoUnit->SetText( GetText() );
	pUIChatMacoUnit->SetFont( GetFont() );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	리소스 재 로드
//

BOOL SPUIChatMacro::ReResourceLoad()
{
	if( m_pChatMacroLDT == NULL )
		return FALSE;

	SetText( m_pChatMacro->m_strName.c_str() );

	return SetResource( m_pChatMacroLDT->m_iIconFile , m_pChatMacroLDT->m_iIconIndex );
}

//////////////////////////////////////////////////////////////////////////
//
//	채팅 매크로 데이터
//

void SPUIChatMacro::SetChatMacro( CHAT_MACRO* pChatMacro )
{
	m_pChatMacro	=	pChatMacro;
}

CHAT_MACRO* SPUIChatMacro::GetChatMacro()
{
	return m_pChatMacro;
}

//////////////////////////////////////////////////////////////////////////
//
//	채팅 매크로 LDT 데이터
//

void SPUIChatMacro::SetChatMacroLDT( CHAT_MACRO_LDT* pChatMacroLDT )
{
	m_pChatMacroLDT	=	pChatMacroLDT;
}

CHAT_MACRO_LDT* SPUIChatMacro::GetChatMacroLDT()
{
	return m_pChatMacroLDT;
}

//////////////////////////////////////////////////////////////////////////
//
//	글자 렌더 위치 인터페이스
//

void SPUIChatMacro::SetTextRect( RECT rtSrc )
{
	m_rtTextDest		=	rtSrc;
}

void SPUIChatMacro::SetTextRect( int ixLeft , int iyTop , int ixRight , int iyBottom )
{
	SetRect( &m_rtTextDest , ixLeft , iyTop , ixRight , iyBottom );
}

RECT SPUIChatMacro::GetTextRect()
{
	return m_rtTextDest;
}

//////////////////////////////////////////////////////////////////////////
//
//	글자 색깔 인터페이스
//

void SPUIChatMacro::SetTextColor( D3DXCOLOR cColor )
{
	m_cTextRGBA	=	cColor;
}

D3DXCOLOR SPUIChatMacro::GetTextColor()
{
	return m_cTextRGBA;
}

//////////////////////////////////////////////////////////////////////////
//
//	글자 정렬 인터페이스
//

void SPUIChatMacro::SetTextAlign( DWORD dwAlign )
{
	m_dwTextAlign	=	dwAlign;
}

DWORD SPUIChatMacro::GetTextAlign()
{
	return m_dwTextAlign;
}

//////////////////////////////////////////////////////////////////////////
//
//	글자 폰트 인터페이스
//

void SPUIChatMacro::SetFontShadow( BOOL bShow )
{
	m_bTextFontShadow	=	bShow;
}

BOOL SPUIChatMacro::GetFontShadow()
{
	return m_bTextFontShadow;
}

//////////////////////////////////////////////////////////////////////////
//
//	글자 Period 인터페이스
//

void SPUIChatMacro::SetPeriod( BOOL bPeriod )
{
	m_bTextPeriod	=	bPeriod;
}

BOOL SPUIChatMacro::GetPeriod()
{
	return m_bTextPeriod;
}

//////////////////////////////////////////////////////////////////////////
//
//	글자 내용 인터페이스
//

void SPUIChatMacro::SetText( const char* pszText )
{
	if( pszText == NULL )
	{
		m_strText.clear();
		return;
	}

	m_strText	=	pszText;
}

const char* SPUIChatMacro::GetText()
{
	return m_strText.c_str();
}

//////////////////////////////////////////////////////////////////////////
//
//	글자 폰트 인터페이스
//

void SPUIChatMacro::SetFont( FONT_ENUMERATE eFont )
{
	m_eTextFont	=	eFont;

	m_pTextFont	=	g_pVideo->GetFont( m_eTextFont );
}

FONT_ENUMERATE SPUIChatMacro::GetFont()
{
	return m_eTextFont;
}

//////////////////////////////////////////////////////////////////////////
//
//	기타 인터페이스
//

BOOL SPUIChatMacro::IsText()
{
	if( m_strText.empty() == true || m_pTextFont == NULL )
		return FALSE;

	return TRUE;
}
