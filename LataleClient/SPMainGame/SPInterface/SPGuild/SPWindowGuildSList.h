// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD USER UNIT
// Comment     : 
// Creation    : DURAGON 2007-4-04    11:18 
//***************************************************************************

#pragma once

#define SIMPLE_SELECT_BG_COUNT	3


class SPWindowGuildSimpleList : public SPWindow
{
public:
	SPWindowGuildSimpleList( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual	~SPWindowGuildSimpleList();

	virtual	void		Init				();
	virtual	void		Clean				();
	virtual void		Show				(bool bWithChild = true);
	virtual void		Hide				(bool bSendServer = true);

	virtual	void		Process				( float fTime );
	virtual	void		Render				( float fTime );
	virtual void		RefreshRelationCoord();

	void				SetIconImage		( const char* szImage , int iSrcX , int iSrcY );
	void				SetIClassImage		( int iClassType ) ;
	void				SetIconImagePos		( int iXPos , int iYPos );
	void				SetIconImageSize	( int iXSize , int iYSize );
	void				SetTextString		( const char* strText);
	void				SetTextPos			( int iXPos , int iYPos );
	void				SetTextSize			( int iXSize , int iYSize );
	void				SetTextColor		( D3DXCOLOR cColor );
	void				SetSelectAble		( bool bSelect ) ;

	bool				GetSelectAble		() ;
	bool				IsHaveInfo			() ;
	void				ShowToolTip			( bool bShowToolTip );
	const char*			GetText				();
	void				SetWndState			( WND_STATE eWndState );
	void				SetRelatedPos		(int iX, int iY) ;

private:
	void				ReposOwnImage();

protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int					OnCursorEnter		( WPARAM wParam, LPARAM lParam );
	int					OnCursorOut			( WPARAM wParam, LPARAM lParam );
	int					OnCursorMove		( WPARAM wParam, LPARAM lParam );
	int					OnCursorEnter		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnCursorOut			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnCursorMove		( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int					OnLButtonUp			( WPARAM wParam, LPARAM lParam );
	int					OnLButtonUp			( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int					OnWheelUp			( WPARAM wParam, LPARAM lParam );
	int					OnWheelDown			( WPARAM wParam, LPARAM lParam );
	int					OnWheelUp			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnWheelDown			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	// }}


protected:
	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//
	SPTexture*			m_pBGTexture;
	RECT				m_rcBGDest[ SIMPLE_SELECT_BG_COUNT ];
	RECT				m_rcBGSrc[ SIMPLE_SELECT_BG_COUNT ];

	SPWindowStatic*		m_pIcon;
	SPWindowStatic*		m_pText;

	INSTANCE_ID			m_iStartID;
	INSTANCE_ID			m_iEndID;

	bool				m_bShowToolTip;
	bool				m_bSelectAble ;
};
