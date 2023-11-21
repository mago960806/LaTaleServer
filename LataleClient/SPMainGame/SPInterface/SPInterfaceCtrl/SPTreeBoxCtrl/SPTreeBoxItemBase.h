// ***************************************************************
//  SPTreeBoxItemBase.h   version:  1.0   date:2006/01/20
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 AJJIYA - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class SPTreeBoxItemBase
{
public:
	SPTreeBoxItemBase				();
	virtual	~SPTreeBoxItemBase		();

	virtual	void	Init			();
	virtual	void	Clear			();

	virtual	void	Process			( float fTime );
	virtual	void	Render			( float fTime );

	virtual	int		GetNextPosX		();
	virtual	int		GetNextPosY		();

	virtual	bool	IsCursorIn		( int iX, int iY );

	virtual	void	SetPos			( int iX , int iY );
	virtual	RECT	GetPosRect		();

	virtual	void	SetSize			( int iX , int iY );
	virtual	POINT	GetSize			();

	virtual	void	SetStatus		( TREEBOX_DATA_STATUS_LABEL eStatus );
	virtual	void	AddStatus		( DWORD dwStatus );
	virtual	void	DelStatus		( DWORD dwStatus );
	virtual	bool	GetStatus		( DWORD dwStatus );
	virtual	DWORD	GetStatus		();

	void			SetText			( const char* pString );
	const char*		GetText			();

	void			SetTextColor	( TREEBOX_DATA_STRING_LABEL eLabel , D3DXCOLOR cColor );
	void			SetTextColor	( int iLabel , D3DXCOLOR cColor );
	void			SetBold			( bool bBold );

	void			SetTextMargin	( int iX , int iY );
	POINT			GetTextMargin	();

	void			SetAlign		( DWORD dwAlign );
	DWORD			GetAlign		();

	void			SetFontEnumerate( FONT_ENUMERATE eFontEnumerate );
	FONT_ENUMERATE	GetFontEnumerate();

	void			SetReturnValue	( int iValue );
	int				GetReturnValue	();

	void			SetTextPeriod	( bool bPeriod );
	bool			GetTextPeriod	();

protected:
private:

	DWORD			m_dwStatus;							//	내 상태
	POINT			m_ptSize;							//	크기
	RECT			m_rcPos;							//	렌더되는 위치
	std::string		m_strText;							//	글자
	D3DXCOLOR		m_cTextColor[ DATA_STRING_COUNT ];	//	글자 색깔
	bool			m_bBold;							//	글자 두번 찍기 ( 진하게 )
	POINT			m_ptMargin;							//	글자 오프셋
	DWORD			m_dwAlign;							//	글자 정렬값
	FONT_ENUMERATE  m_eFontEnumerate;					//	글자 크기 및 종류

	int				m_iReturnValue;						//	저장해논 값 리턴
	bool			m_bTextPeriod;						//	텍스트 ..으로 표시여부
};
