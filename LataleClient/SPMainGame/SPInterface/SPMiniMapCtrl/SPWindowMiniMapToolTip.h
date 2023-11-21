// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-10    18:08 
//***************************************************************************

#pragma once

#include <string>

class SPWindowMiniMapToolTip {
public:
	SPWindowMiniMapToolTip();
	~SPWindowMiniMapToolTip();

	//	좌표는 월드 좌표
	void	Init( const char* pStrResource = NULL );

	void	Render();

	void	SetToolTipText( const char* pStrToolTip );
	void	SetPosX( int iPosX )	{	m_iPosX = iPosX;	}
	void	SetPosY( int iPosY )	{	m_iPosY = iPosY;	}
	void	Show()					{	m_bShow = true;		}
	void	Hide()					{	m_bShow = false;	}

protected:

	void	Destory			();

	bool	SetImage		( const char* szImagePath );

	void	GetToolTipRect	( RECT& rtTooTip );

	SPTexture*		m_pToolTipImage;

	RECT			m_rtImageSrcLeft;
	RECT			m_rtImageSrcMiddle;
	RECT			m_rtImageSrcRight;
	
	RECT			m_rtImageDestLeft;
	RECT			m_rtImageDestMiddle;
	RECT			m_rtImageDestRight;

	D3DXCOLOR		m_RGBA;
	bool			m_bShow;

	int				m_iPosX;		//	월드 좌표 X
	int				m_iPosY;		//	월드 좌표 Y
	std::string		m_strToolTip;	//	툴팁 문자
	D3DXCOLOR		m_FontColor;	//	폰트 색깔
	bool			m_bFontShadow;	//	폰트 그림자

};
