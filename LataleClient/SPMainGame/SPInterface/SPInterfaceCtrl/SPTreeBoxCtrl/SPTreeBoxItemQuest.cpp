/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/06/23
	created:	2006:6:23:   16:00
	filename: 	SPTreeBoxItemQuest.cpp
	
	purpose:	
*********************************************************************/

#include "SPCommon.h"

#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"

#include "SPTreeBoxItemQuest.h"

//////////////////////////////////////////////////////////////////////////

SPTreeBoxItemQuest::SPTreeBoxItemQuest()
{
	Init();
}

SPTreeBoxItemQuest::~SPTreeBoxItemQuest()
{
	Clear();
}

void SPTreeBoxItemQuest::Init()
{
	for( int i = 0 ; i < QUEST_TEXTURE_COUNT ; ++i )
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

		m_ptTextureSize[ i ].x			=	0;
		m_ptTextureSize[ i ].y			=	0;

		m_ptTextureOffset[ i ].x		=	0;
		m_ptTextureOffset[ i ].y		=	0;
	}

	m_ptCtrlPos.x		=	0;
	m_ptCtrlPos.y		=	0;

	m_ptCtrlSize.x		=	0;
	m_ptCtrlSize.y		=	0;

	m_ptCtrlOffset.x	=	0;
	m_ptCtrlOffset.y	=	0;

	m_bRightMouseOver	=	false;

	m_bQuestComplete	=	false;
	m_bQuestMiniWindow	=	false;

	m_pParentWindow		=	NULL;
}

void SPTreeBoxItemQuest::Clear()
{
	for( int i = 0 ; i < QUEST_TEXTURE_COUNT ; ++i )
	{
		SAFE_RELEASE( m_pTexture[ i ] );
	}
}

void SPTreeBoxItemQuest::Process( float fTime )
{
	SPTreeBoxItemBase::Process( fTime );
}

void SPTreeBoxItemQuest::Render( float fTime )
{
	g_pVideo->Flush();

	if( GetStatus( DATA_STATUS_SELECT ) == true )
	{
		int	iMouseOver	=	QUEST_TEXTURE_MOUSE_OVER * 2;

		if( m_pTexture[ iMouseOver ] != NULL )
		{
			m_pTexture[ iMouseOver ]->RenderTexture( &( m_rcTexturePos[ iMouseOver ] ) , &( m_rcTextureSrc[ iMouseOver ] ) );
			g_pVideo->Flush();
		}
	}

	int	iLeftNumber	=	QUEST_TEXTURE_LEFT_ENABLE * 2;

	if( ( GetStatus( DATA_STATUS_PULLDOWN ) == true && GetStatus( DATA_STATUS_DISABLE ) == false ) || m_bQuestComplete == true )
	{
		++iLeftNumber;
	}

	if( m_pTexture[ iLeftNumber ] != NULL )
	{
		m_pTexture[ iLeftNumber ]->RenderTexture( &( m_rcTexturePos[ iLeftNumber ] ) , &( m_rcTextureSrc[ iLeftNumber ] ) );
		g_pVideo->Flush();
	}

	int	iRightNumber	=	QUEST_TEXTURE_RIGHT_ENABLE;

	if( m_bRightMouseOver == true )
	{
		if( GetStatus( DATA_STATUS_PUSH ) == true )
		{
			iRightNumber	=	QUEST_TEXTURE_RIGHT_CLICK;
		}
		else if( GetStatus( DATA_STATUS_HIT ) == true )
		{
			iRightNumber	=	QUEST_TEXTURE_RIGHT_MOUSE_OVER;
		}
	}

	iRightNumber	*=	2;

	if( m_bQuestMiniWindow == true )
	{
		++iRightNumber;
	}
	
	if( m_pTexture[ iRightNumber ] != NULL )
	{
		m_pTexture[ iRightNumber ]->RenderTexture( &( m_rcTexturePos[ iRightNumber ] ) , &( m_rcTextureSrc[ iRightNumber ] ) );
		g_pVideo->Flush();
	}

	SPTreeBoxItemBase::Render( fTime );

	//RECT	rtTemp	=	GetPosRect();

	//g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ) );
	//g_pVideo->GetSysLine()->DrawRect( rtTemp );
	//g_pVideo->GetSysLine()->SetColor();
}

int SPTreeBoxItemQuest::GetNextPosX()
{
	return m_ptCtrlPos.x;
}

int SPTreeBoxItemQuest::GetNextPosY()
{
	return m_ptCtrlPos.y	+	m_ptCtrlSize.y;
}

bool SPTreeBoxItemQuest::IsCursorIn( int iX, int iY )
{
	m_bRightMouseOver	=	false;

	RECT	rcCtrlPos	=	GetPosRect();

	if( rcCtrlPos.left	<=	iX	&&	rcCtrlPos.right		>=	iX &&
		rcCtrlPos.top	<=	iY	&&	rcCtrlPos.bottom	>=	iY )
	{
		int	iRightEnable	=	QUEST_TEXTURE_RIGHT_ENABLE * 2;

		if( m_rcTexturePos[ iRightEnable ].left	<=	iX	&&	m_rcTexturePos[ iRightEnable ].right	>=	iX &&
			m_rcTexturePos[ iRightEnable ].top	<=	iY	&&	m_rcTexturePos[ iRightEnable ].bottom	>=	iY )
		{
			m_bRightMouseOver	=	true;
		}

		return true;
	}

	return false;
}

void SPTreeBoxItemQuest::SetPos( int iX , int iY )
{
	m_ptCtrlPos.x	=	iX;
	m_ptCtrlPos.y	=	iY;

	iX	+=	m_ptCtrlOffset.x;
	iY	+=	m_ptCtrlOffset.y;

	SPTreeBoxItemBase::SetPos( iX , iY );

	for( int i = 0 ; i < QUEST_TEXTURE_COUNT ; ++i )
	{
		m_rcTexturePos[ i ].left		=	iX	+	m_ptTextureOffset[ i ].x;
		m_rcTexturePos[ i ].top			=	iY	+	m_ptTextureOffset[ i ].y;
		m_rcTexturePos[ i ].right		=	m_rcTexturePos[ i ].left	+	m_ptTextureSize[ i ].x;
		m_rcTexturePos[ i ].bottom		=	m_rcTexturePos[ i ].top		+	m_ptTextureSize[ i ].y;
	}
}

RECT SPTreeBoxItemQuest::GetPosRect()
{
	RECT	rcCtrlPos;

	rcCtrlPos.left		=	m_ptCtrlPos.x	+	m_ptCtrlOffset.x;
	rcCtrlPos.top		=	m_ptCtrlPos.y	+	m_ptCtrlOffset.y;
	rcCtrlPos.right		=	rcCtrlPos.left	+	m_ptCtrlSize.x;
	rcCtrlPos.bottom	=	rcCtrlPos.top	+	m_ptCtrlSize.y;

	return rcCtrlPos;
}

void SPTreeBoxItemQuest::SetStatus( TREEBOX_DATA_STATUS_LABEL eStatus )
{
	SPTreeBoxItemBase::SetStatus( eStatus );

	if( m_bRightMouseOver == true )
	{
		if( GetStatus( DATA_STATUS_SELECT ) == true && m_pParentWindow != NULL )
		{
			m_pParentWindow->SPSendMessage( SPIM_QUEST_NOTIFY , (WPARAM)this , (LPARAM)GetReturnValue() );
		}
	}
}

void SPTreeBoxItemQuest::AddStatus( DWORD dwStatus )
{
	SPTreeBoxItemBase::AddStatus( dwStatus );
}

void SPTreeBoxItemQuest::DelStatus( DWORD dwStatus )
{
	SPTreeBoxItemBase::DelStatus( dwStatus );
}

BOOL SPTreeBoxItemQuest::SetTexture( TREEBOX_QUEST_TEXTURE_LABEL eLabel , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY )
{
	return SetTexture( (int)eLabel , strFileName , iSrcSX , iSrcSY , iSrcEX , iSrcEY );
}

BOOL SPTreeBoxItemQuest::SetTexture( int iLabel , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY )
{
	if( iLabel >= QUEST_TEXTURE_COUNT )
		return FALSE;

	SPTexture*		pTexture	=	NULL;

	if( g_pVideo->GetTextureMgr()->LoadTexture( strFileName , &pTexture ) == false )
		return FALSE;

	m_rcTextureSrc[ iLabel ].left	=	iSrcSX;
	m_rcTextureSrc[ iLabel ].top	=	iSrcSY;
	m_rcTextureSrc[ iLabel ].right	=	iSrcEX;
	m_rcTextureSrc[ iLabel ].bottom	=	iSrcEY;

	m_ptTextureSize[ iLabel ].x		=	iSrcEX - iSrcSX;
	m_ptTextureSize[ iLabel ].y		=	iSrcEY - iSrcSY;

	SAFE_RELEASE( m_pTexture[ iLabel ] );

	m_pTexture[ iLabel ]	=	pTexture;

	return TRUE;
}

void SPTreeBoxItemQuest::SetTextureOffset( TREEBOX_QUEST_TEXTURE_LABEL eLabel , int iX , int iY )
{
	SetTextureOffset( (int)eLabel , iX ,iY );
}

void SPTreeBoxItemQuest::SetTextureOffset( int iLabel , int iX , int iY )
{
	if( iLabel >= QUEST_TEXTURE_COUNT )
		return;

	m_ptTextureOffset[ iLabel ].x	=	iX;
	m_ptTextureOffset[ iLabel ].y	=	iY;
}

void SPTreeBoxItemQuest::SetQuestSize( int iXSize , int iYSize )
{
	m_ptCtrlSize.x	=	iXSize;
	m_ptCtrlSize.y	=	iYSize;
}

void SPTreeBoxItemQuest::SetQuestOffset( int iX , int iY )
{
	m_ptCtrlOffset.x	=	iX;
	m_ptCtrlOffset.y	=	iY;
}

void SPTreeBoxItemQuest::SetQuestComplete( bool bQuestComplete )
{
	m_bQuestComplete	=	bQuestComplete;
}

void SPTreeBoxItemQuest::SetQuestMiniWindow( bool bQuestMiniWindow )
{
	m_bQuestMiniWindow	=	bQuestMiniWindow;
}

bool SPTreeBoxItemQuest::GetQuestComplete()
{
	return m_bQuestComplete;
}

bool SPTreeBoxItemQuest::GetQuestMiniWindow()
{
	return m_bQuestMiniWindow;
}

void SPTreeBoxItemQuest::SetWindow( SPWindow* pWindow )
{
	m_pParentWindow	=	pWindow;
}
