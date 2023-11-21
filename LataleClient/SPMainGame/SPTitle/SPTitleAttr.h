// ***************************************************************
//  SPTitleAttr   version:  1.0   ¡¤  date: 03/29/2006
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

class SPTitleAttr
{
public:
	SPTitleAttr				(void);
	virtual ~SPTitleAttr	(void);

	enum TITLE_TYPE
	{
		TITLE_TYPE_NULL							=	0x0000	,
		TITLE_TYPE_STRING						=	0x0001	,
		TITLE_TYPE_TEXTURE						=	0x0002	,
		TITLE_TYPE_ANIMATION					=	0x0004	,
	};

	enum TITLE_COUNT
	{
		TITLE_STATUS_COUNT						=	4	,
		TITLE_EFFECT_COUNT						=	4	,
	};


	void			Init				();

	void			SetID				( UINT32 uiID );
	void			SetName				( const char* pstrName );
	void			SetDesc				( const char* pstrDesc );
	void			SetType				( int iType );
	void			SetBGTexture		( const char* pstrBGTexture );
	void			SetSize				( int iX , int iY );
	void			SetSrcRect			( int iLeft , int iTop , int iRight , int iBottom );
	void			SetAlign			( DWORD dwAlign );
	void			SetMargin			( int iMargin );
	void			SetVMargin			( int iMargin );
	void			SetStatusType		( int iNumber , DWORD dwType , int iValue );
	void			SetEffect			( int iNumber , DWORD dwEffect , const char* pstrDesc );
	void			SetFontColor		( int iR , int iG , int iB );

	UINT32			GetID				();
	const char*		GetName				();
	const char*		GetDesc				();
	TITLE_TYPE		GetType				();
	const char*		GetBGTexture		();
	POINT			GetSize				();
	RECT			GetSrcRect			();
	DWORD			GetAlign			();
	int				GetMargin			();
	int				GetVMargin			();
	DWORD			GetStatusType		( int iNumber );
	int				GetStatusTypeValue	( int iNumber );
	DWORD			GetEffect			( int iNumber );
	const char*		GetEffectDesc		( int iNumber );
	D3DXCOLOR		GetFontColor		();

protected:


	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼ö¸í
	//

	UINT32						m_uiID;
	std::string					m_strName;
	std::string					m_strDesc;
	TITLE_TYPE					m_eType;
	std::string					m_strBGTexture;
	POINT						m_ptSize;
	RECT						m_rcSrc;
	DWORD						m_dwAlign;
	int							m_iMargin;
	int							m_iVMargin;
	DWORD						m_dwStatusType[ TITLE_STATUS_COUNT ];
	int							m_iStatusValue[ TITLE_STATUS_COUNT ];
	DWORD						m_dwEffect[ TITLE_EFFECT_COUNT ];
	std::string					m_strEffectDesc[ TITLE_EFFECT_COUNT ];
	D3DXCOLOR					m_FontColor;
};
