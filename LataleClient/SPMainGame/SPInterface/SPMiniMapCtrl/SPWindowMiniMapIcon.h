// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-12    11:40 
//***************************************************************************

#pragma once

enum MINIMAP_ICON_TYPE
{
	MINIMAP_ICON_NULL				=	0	,
	MINIMAP_ICON_NPC						,
	MINIMAP_ICON_SAVEPOINT					,
	MINIMAP_ICON_PORTAL						,
	MINIMAP_ICON_MAILBOX					,
	MINIMAP_ICON_MYPLAYER					,
	MINIMAP_ICON_OTHERPLAYER				,
	MINIMAP_ICON_DEATHTEAMPLAYER			,
	MINIMAP_ICON_TEAMPLAYER					,

	MINIMAP_ICON_DEL_AVATAR			=	-1	,
	MINIMAP_ICON_DEL_EVENT			=	-2	,
	MINIMAP_ICON_DEL_ALL			=	-3	,
};

class SPGameObject;
class SPWindowMiniMapView;

class SPWindowMiniMapIcon {
public:
	SPWindowMiniMapIcon();
	~SPWindowMiniMapIcon();

	void	Create			( MINIMAP_ICON_TYPE eIconType , SPGameObject* pGOB , SPWindowMiniMapView* pParent );

	void	Process			( float fTime );
	void	Render			();

	bool	CheckInMouse	( int iPosX , int iPosY );

	void	SetMiniMapRect	( RECT rtRect )					{	m_rtMiniMap	=	rtRect;		}
	
	int		GetPosX			()	{	return	static_cast<int>( m_rtImageDest.left );	}
	int		GetPosY			()	{	return	static_cast<int>( m_rtImageDest.top );	}

	void	GetGOBName		( char* pStr );

	void	SetIconType		( MINIMAP_ICON_TYPE eIconType );

protected:
	void	Destory			();

	void	SetGOB			( SPGameObject* pGOB )			{	m_pGOB			=	pGOB;		}
	void	SetParent		( SPWindowMiniMapView* pParent)	{	m_pParent		=	pParent;	}
	void	SetColor		( D3DXCOLOR color )				{	m_RGBA			=	color;		}
	bool	SetImage		( const char* szImagePath );
	void	SetPos			( int iPosX , int iPosY );

	void	ChangeIcon		( MINIMAP_ICON_TYPE eIconType );

	//	월드좌표를 미니맵 좌표로 변환하자.
	POINT	GetMiniMapPos	( int iWorldPosX , int iWorldPosY , RECT rtMiniMap );

	//	위치가 바뀌어서 영역을 업데이트 하자.
	void	UpdateDestRect	();

	//	미니맵 뷰 영역을 벗어나는지 확인하자.
	void	MiniMapViewValidRect();

	SPWindowMiniMapView*	m_pParent;

	SPGameObject*			m_pGOB;

	SPTexture*				m_pIconImage;

	D3DXCOLOR				m_RGBA;

	POINT					m_ptImagePos;
	POINT					m_ptImageSize;

	RECT					m_rtMiniMapView;

	RECT					m_rtImageSrc;
	RECT					m_rtCurImageSrc;

	RECT					m_rtImageDest;
	RECT					m_rtMiniMap;

	bool					m_bShow;
	MINIMAP_ICON_TYPE		m_eIconType;
};
