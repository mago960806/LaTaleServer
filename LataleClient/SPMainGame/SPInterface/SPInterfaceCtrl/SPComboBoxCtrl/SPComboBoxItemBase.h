// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-11-9    11:59 
//***************************************************************************

#pragma once

class SPComboBoxItemBase
{
public:

	SPComboBoxItemBase				();
	virtual	~SPComboBoxItemBase		();

	virtual	void	Init			();
	virtual	void	Clean			();
	virtual	void	Process			( float fTime );
	virtual	void	Render			( float fTime );
	virtual void	Show			();
	virtual void	Hide			();

	virtual	BOOL	IsProcess		();
	virtual	BOOL	IsRender		();

	virtual	void	SetStatus		( COMBOBOX_DATA_TEXTURE_LABEL eStatus );
	virtual	void	SetRenderInfo	( CBB_RENDER_INFO* pstRenderStatus );

	virtual void	SetSize			( int iX , int iY );
	virtual POINT	GetSize			();

	COMBOBOX_DATA_TEXTURE_LABEL	GetStatus();
	CBB_RENDER_INFO*			GetRenderInfo();

	//	텍스트 함수
	void			SetText			( const char* strText );
	void			SetTextColor	( D3DXCOLOR stColor );
	void			SetTextShadow	( bool bShadow );
	void			SetTextAlign	( DWORD dwAlign );
	const char*		GetText			();

protected:

	BOOL							m_bShow;					//	그리기 여부
	COMBOBOX_DATA_TEXTURE_LABEL		m_eStatus;					//	내 상태
	CBB_RENDER_INFO*				m_pstRenderInfo;			//	그려질 정보들
	POINT							m_ptSize;					//	아이템의 크기

	//	텍스트 변수
	std::string						m_strText;
	RECT							m_rcTextPos;
	D3DXCOLOR						m_stTextColor;
	bool							m_bTextShadow;
	DWORD							m_dwTextAlign;
	RECT							m_rcTextMargin;

};
