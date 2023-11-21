// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-12    11:39 
//***************************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPResourceDef.h"
#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPWindowDEF.h"
#include "SPWIndow.h"
#include "SPWindowMiniMapIcon.h"
#include "SPWindowMiniMapToolTip.h"
#include "SPWindowMiniMapView.h"

#include "SPWindowMiniMapIcon.h"

//////////////////////////////////////////////////////////////////////////
//
//	SPWindowMiniMapIcon
//

SPWindowMiniMapIcon::SPWindowMiniMapIcon()
{
	m_pIconImage	=	NULL;
	m_pParent		=	NULL;
	m_pGOB			=	NULL;
	m_eIconType		=	MINIMAP_ICON_NULL;
}

SPWindowMiniMapIcon::~SPWindowMiniMapIcon()
{
	Destory();
}

void SPWindowMiniMapIcon::Create( MINIMAP_ICON_TYPE eIconType , SPGameObject* pGOB , SPWindowMiniMapView* pParent )
{
	SetGOB( pGOB );
	SetIconType( eIconType );
	SetColor( D3DXCOLOR( 1 , 1 , 1 , 1 ) );
	SetParent( pParent );

	m_pParent->GetWindowRect( m_rtMiniMapView );

	m_rtMiniMapView.left++;
	m_rtMiniMapView.top++;
	m_rtMiniMapView.right--;
	m_rtMiniMapView.bottom--;
}

void SPWindowMiniMapIcon::Destory()
{
	SAFE_RELEASE( m_pIconImage );
	m_pParent		=	NULL;
	m_pGOB			=	NULL;
}

void SPWindowMiniMapIcon::Process( float fTime )
{
	m_bShow	=	false;

	if( m_pGOB == NULL || m_pParent == NULL )
		return;

	int iPosX = m_pGOB->SPGOBPerformMessage( SPGM_GETPOSX );
	int iPosY = m_pGOB->SPGOBPerformMessage( SPGM_GETPOSY );

	m_rtMiniMap	=	m_pParent->GetMiniMapRect();

	//	현재 위치가 미니맵 그려주는 영역안인지 판단
	if( iPosX < m_rtMiniMap.left	|| iPosX > m_rtMiniMap.right ||
		iPosY < m_rtMiniMap.top		|| iPosY > m_rtMiniMap.bottom )
	{
		m_bShow	=	false;
		return;
	}

	m_bShow	=	true;

	//	현재 위치를 미니맵의 상대 좌표로 세팅한다.
	m_ptImagePos	=	GetMiniMapPos( iPosX , iPosY , m_rtMiniMap );

	UpdateDestRect();
}

void SPWindowMiniMapIcon::Render()
{
	if( m_bShow == false || m_pIconImage == NULL )
		return;

	if( m_pGOB == NULL || m_pParent == NULL )
		return;

	g_pVideo->Flush();

	m_pIconImage->SetColor( m_RGBA );
	m_pIconImage->RenderTexture( &m_rtImageDest , &m_rtCurImageSrc );
}

bool SPWindowMiniMapIcon::CheckInMouse( int iPosX , int iPosY )
{
	//	마우스 좌표로 이미지 영역에 들어온건지를 판단한다.
	if( iPosX < m_rtImageDest.left	|| iPosX > m_rtImageDest.right ||
		iPosY < m_rtImageDest.top	|| iPosY > m_rtImageDest.bottom )
	{
		return false;
	}

	return true;
}

void SPWindowMiniMapIcon::GetGOBName( char* pStr )
{
	m_pGOB->SPGOBPerformMessage( SPGM_GETGOBNAME , (LPARAM)pStr );
}

void SPWindowMiniMapIcon::SetIconType( MINIMAP_ICON_TYPE eIconType )
{
	ChangeIcon( eIconType );
}

bool SPWindowMiniMapIcon::SetImage( const char* szImagePath )
{
	std::string strResource;

	//strResource = RES_WORK_RESOURCE_PATH;
	strResource = szImagePath;

	g_pVideo->GetTextureMgr()->LoadTexture( strResource.c_str() , &m_pIconImage );

	if( m_pIconImage == NULL )
		return false;

	return true;
}

void SPWindowMiniMapIcon::SetPos( int iPosX , int iPosY )
{
	m_ptImagePos.x	=	iPosX;
	m_ptImagePos.y	=	iPosY;
}

void SPWindowMiniMapIcon::ChangeIcon( MINIMAP_ICON_TYPE eIconType )
{
	switch( eIconType )
	{
	case MINIMAP_ICON_MYPLAYER:
		SetRect( &m_rtImageSrc , 377 , 1 , 383 , 12 );
		break;
	case MINIMAP_ICON_OTHERPLAYER:
		SetRect( &m_rtImageSrc , 384 , 1 , 390 , 12 );
		break;
	case MINIMAP_ICON_MAILBOX:
		SetRect( &m_rtImageSrc , 444 , 1 , 451 , 12 );
		break;
	case MINIMAP_ICON_PORTAL:
		SetRect( &m_rtImageSrc , 369 , 1 , 376 , 12 );
		break;
	case MINIMAP_ICON_SAVEPOINT:
		SetRect( &m_rtImageSrc , 405 , 1 , 418 , 12 );
		break;
	case MINIMAP_ICON_NPC:
		SetRect( &m_rtImageSrc , 398 , 1 , 404 , 12 );
		break;
	case MINIMAP_ICON_DEATHTEAMPLAYER:
		SetRect( &m_rtImageSrc , 419 , 1 , 428 , 12 );
		break;
	case MINIMAP_ICON_TEAMPLAYER:
		SetRect( &m_rtImageSrc , 391 , 1 , 397 , 12 );
		break;
	}

	SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" );

	m_ptImageSize.x	=	m_rtImageSrc.right	- m_rtImageSrc.left;
	m_ptImageSize.y =	m_rtImageSrc.bottom	- m_rtImageSrc.top;

	UpdateDestRect();

	m_eIconType	=	eIconType;
}

POINT SPWindowMiniMapIcon::GetMiniMapPos( int iWorldPosX , int iWorldPosY , RECT rtMiniMap )
{
	POINT rtLocalPos;
	rtLocalPos.x	=	( iWorldPosX	-	rtMiniMap.left	) / MINIMAP_SCALE_X;
	rtLocalPos.y	=	( iWorldPosY	-	rtMiniMap.top	) / MINIMAP_SCALE_Y;

	return rtLocalPos;
}

void SPWindowMiniMapIcon::UpdateDestRect()
{
	int	iStep	=	m_ptImageSize.x	/	2	+	m_ptImageSize.x % 2;

	m_rtImageDest.left		=	m_rtMiniMapView.left	+	m_ptImagePos.x	-	iStep - 1;
	m_rtImageDest.top		=	m_rtMiniMapView.top		+	m_ptImagePos.y	-	m_ptImageSize.y;
	m_rtImageDest.right		=	m_rtImageDest.left		+	m_ptImageSize.x;
	m_rtImageDest.bottom	=	m_rtImageDest.top		+	m_ptImageSize.y;

	MiniMapViewValidRect();
}

void SPWindowMiniMapIcon::MiniMapViewValidRect()
{
	int	iSize;
	m_rtCurImageSrc	=	m_rtImageSrc;

	if( m_rtImageDest.left < m_rtMiniMapView.left )
	{
		iSize	=	m_rtMiniMapView.left - m_rtImageDest.left;
		m_rtCurImageSrc.left	+=	iSize;
		m_rtImageDest.left		+=	iSize;;
	}

	if( m_rtImageDest.top < m_rtMiniMapView.top )
	{
		iSize	=	m_rtMiniMapView.top - m_rtImageDest.top;
		m_rtCurImageSrc.top		+=	iSize;
		m_rtImageDest.top		+=	iSize;;
	}

	if( m_rtImageDest.right > m_rtMiniMapView.right )
	{
		iSize	=	m_rtMiniMapView.right - m_rtImageDest.right;
		m_rtCurImageSrc.right	+=	iSize;
		m_rtImageDest.right		+=	iSize;;
	}

	if( m_rtImageDest.bottom > m_rtMiniMapView.bottom )
	{
		iSize	=	m_rtMiniMapView.bottom - m_rtImageDest.bottom;
		m_rtCurImageSrc.bottom	+=	iSize;
		m_rtImageDest.bottom	+=	iSize;;
	}
}