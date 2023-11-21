// ***************************************************************
//  SPTitleRenderBase   version:  1.0   ·  date: 03/30/2006
//  -------------------------------------------------------------
//  
//	AJJIYA
//
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class SPTitleRenderBase
{
public:

	SPTitleRenderBase				();
	virtual	~SPTitleRenderBase		();

	virtual	void	Init			();
	virtual	void	Clean			();
	virtual	void	Process			( float fTime );
	virtual	void	Render			( float fTime );
	virtual void	Show			();
	virtual void	Hide			();

	virtual	BOOL	IsProcess		();
	virtual	BOOL	IsRender		();

//	virtual	void	SetStatus		( TITLE_STATUS eStatus );
	virtual	void	SetPos			( int iX , int iY );

	virtual void	SetSize			( int iX , int iY );
	virtual POINT	GetSize			();

	virtual	void	SetID			( UINT32 uiID );
	virtual UINT32	GetID			();

//	TITLE_STATUS	GetStatus();

	//	텍스트 함수
	void			SetText			( const char* strText );
	void			SetTextColor	( D3DXCOLOR stColor );
	void			SetTextShadow	( bool bShadow );
	void			SetTextAlign	( DWORD dwAlign );
	void			SetTextMargin	( int iLeft , int iTop , int iRight , int iBottom );
	const char*		GetText			();

protected:

	BOOL							m_bShow;					//	그리기 여부
//	TITLE_STATUS					m_eStatus;					//	내 상태
	POINT							m_ptPos;					//	그려질 위치
	POINT							m_ptSize;					//	아이템의 크기
	UINT32							m_uiID;						//	타이틀 아이디

	//	텍스트 변수
	std::string						m_strText;
	RECT							m_rcTextPos;
	D3DXCOLOR						m_stTextColor;
	bool							m_bTextShadow;
	DWORD							m_dwTextAlign;
	RECT							m_rcTextMargin;

};
