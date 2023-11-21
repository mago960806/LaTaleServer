// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-10    18:12 
//***************************************************************************

#include "SPCommon.h"
#include "SPUtil.h"
#include "SPMouseCursor.h"

#include "SPResourceDef.h"

#include "SPWindowMiniMapToolTip.h"

//////////////////////////////////////////////////////////////////////////
//
//	SPWindowMiniMapToolTip
//

SPWindowMiniMapToolTip::SPWindowMiniMapToolTip()
{
	m_iPosX			=	0;							//	월드 좌표 X
	m_iPosY			=	0;							//	월드 좌표 Y
	m_strToolTip	=	"";							//	툴팁 문자
	m_FontColor		=	D3DXCOLOR( 0 , 0 , 0 , 1 );	//	폰트 색깔
	m_bFontShadow	=	false;						//	폰트 그림자
	m_RGBA			=	D3DXCOLOR( 1 , 1 , 1 , 1 );
	m_pToolTipImage	=	NULL;

	Init( "DATA/INTERFACE/CONCEPT/UI100.PNG" );
}

SPWindowMiniMapToolTip::~SPWindowMiniMapToolTip()
{
	Destory();
}

void SPWindowMiniMapToolTip::Init( const char* pStrResource /* = NULL  */ )
{
	if( pStrResource == NULL )
		return;

	//
	//	리소스를 로딩하자
	SetImage( pStrResource );

	SetRect( &m_rtImageSrcLeft		, 424 , 53 , 426 , 69 );
	SetRect( &m_rtImageSrcMiddle	, 428 , 53 , 429 , 69 );
	SetRect( &m_rtImageSrcRight		, 431 , 53 , 433 , 69 );
}

void SPWindowMiniMapToolTip::Destory()
{
	SAFE_RELEASE( m_pToolTipImage );
}

bool SPWindowMiniMapToolTip::SetImage( const char* szImagePath )
{
	std::string strResource;

	//strResource = RES_WORK_RESOURCE_PATH;
	strResource = szImagePath;

	g_pVideo->GetTextureMgr()->LoadTexture( strResource.c_str() , &m_pToolTipImage );

	if( m_pToolTipImage == NULL )
		return false;

	return true;
}

void SPWindowMiniMapToolTip::Render()
{
	if( m_strToolTip.empty() || m_bShow == false || m_pToolTipImage == NULL || g_pMouseCursor->IsShowCursor() == false )
		return;

	if( g_pVideo == NULL )
		return;

	g_pVideo->Flush();

	m_pToolTipImage->SetColor( m_RGBA );

	GetToolTipRect( m_rtImageDestMiddle );

	int	iLeftSizeX	=	m_rtImageSrcLeft.right - m_rtImageSrcLeft.left;
	int	iRightSizeX	=	m_rtImageSrcRight.right - m_rtImageSrcRight.left;

	m_rtImageDestLeft.left		=	m_rtImageDestMiddle.left - iLeftSizeX;
	m_rtImageDestLeft.right		=	m_rtImageDestMiddle.left;

	m_rtImageDestLeft.top		=	m_rtImageDestMiddle.top;
	m_rtImageDestLeft.bottom	=	m_rtImageDestMiddle.bottom;

	m_rtImageDestRight.left		=	m_rtImageDestMiddle.right;
	m_rtImageDestRight.right	=	m_rtImageDestRight.left + iRightSizeX;

	m_rtImageDestRight.top		=	m_rtImageDestMiddle.top;
	m_rtImageDestRight.bottom	=	m_rtImageDestMiddle.bottom;

	m_pToolTipImage->RenderTexture( &m_rtImageDestLeft	, &m_rtImageSrcLeft );
	m_pToolTipImage->RenderTexture( &m_rtImageDestMiddle, &m_rtImageSrcMiddle );
	m_pToolTipImage->RenderTexture( &m_rtImageDestRight	, &m_rtImageSrcRight );

	if( g_pVideo->GetFont() == NULL )
		return;

	bool bShadow = g_pVideo->GetFont()->IsShadow();
	g_pVideo->GetFont()->SetColor(	m_FontColor );
	g_pVideo->GetFont()->SetShadow(	m_bFontShadow );
	g_pVideo->GetFont()->RenderText( m_strToolTip.c_str() , &m_rtImageDestMiddle, DT_CENTER | DT_VCENTER |DT_NOCLIP);

	g_pVideo->GetFont()->SetColor();
	g_pVideo->GetFont()->SetShadow(	bShadow );
}

void SPWindowMiniMapToolTip::SetToolTipText( const char* pStrToolTip )
{
	m_strToolTip	=	pStrToolTip;
}

void SPWindowMiniMapToolTip::GetToolTipRect( RECT& rtTooTip )
{
	int iStrHeight		=	12;

	if( g_pVideo != NULL )
	{
		if( g_pVideo->GetFont() != NULL )
		{
			iStrHeight		= static_cast<int>( g_pVideo->GetFont()->GetHeight() );
		}
	}

	int iStrWidth		= static_cast<int>( m_strToolTip.size() ) * 4;
	int iHeightOffSet	= 8;			//	표시되는 이미지와 너무 붙어서 보정
	int iHeightSize		= 3;

	rtTooTip.left	= m_iPosX - iStrWidth;
	rtTooTip.top	= m_iPosY - iStrHeight - iHeightOffSet - iHeightSize;
	rtTooTip.right  = m_iPosX + iStrWidth;
	rtTooTip.bottom	= m_iPosY - iHeightOffSet + iHeightSize;

	//	오른쪽 영역 계산
	int	iRightSizeX	=	m_rtImageSrcRight.right - m_rtImageSrcRight.left;
	int iScreenX	=	static_cast<int>( g_pVideo->GetScreenLenX() ) - iRightSizeX;

	//	오른쪽 화면 밖으로 나가는거 체크해줘야 한다.
	if( rtTooTip.right > iScreenX )
	{
		rtTooTip.left	=	iScreenX - iStrWidth * 2;
		rtTooTip.right	=	iScreenX;
	}
}
