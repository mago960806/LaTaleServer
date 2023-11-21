// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD USER UNIT
// Comment     : 
// Creation    : DURAGON 2007-4-04    11:18 
//***************************************************************************

#pragma once

#define GUILD2_SELECT_BG_COUNT	3


class SPWindowGuildUser : public SPWindow
{
public:
	SPWindowGuildUser( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual	~SPWindowGuildUser();

	virtual	void		Init				();
	virtual	void		Clean				();
	virtual void		Show				(bool bWithChild = true);
	virtual void		Hide				(bool bSendServer = true);

	virtual	void		Process				( float fTime );
	virtual	void		Render				( float fTime );
	virtual void		RefreshRelationCoord();

	bool				IsGuildUserInfo		();
	void				SetGuildInfo		(int iIndex, int iGuildProperty, PGUILD_MEMBER pGuildUserInfo );
	void				ShowToolTip			( bool bShowToolTip );
	const char*			GetName				();
	int					GetGrade			() ;
	bool				IsOffLine			() ;
	void				SetSelButton		(bool bSet) ;
	bool				GetSelButton		() ;
	PGUILD_MEMBER		GetUserInfo()		{	return m_pUserInfo;		}

	void				SetWndState			( WND_STATE eWndState );
	void				ShowSelectBG		(bool bShow) ;


private:
	void				ReposOwnImage();

	void				SetIClassImage	( int iClassType );
	void				SetName			( const char* pstrName );
	void				SetLevel		( int uiLevel );
	void				SetChannel		( int uiChannel );
	void				SetRankImage	( int iProperty, int iRank ) ;
	void				SetOnline		( bool bOnline ) ;
	void				SetGPoint( int iGPoint );


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
	int					OnBGButtonUp		( WPARAM wParam, LPARAM lParam );
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
	RECT				m_rcBGDest[ GUILD2_SELECT_BG_COUNT ];
	RECT				m_rcBGSrc[ GUILD2_SELECT_BG_COUNT ];

	SPWindowStatic*		m_pClass;
	SPWindowStatic*		m_pName;
	SPWindowStatic*		m_pLevel;
	SPWindowStatic*		m_pGPoint;
	SPWindowStatic*		m_pChannel;
	SPWindowStatic*		m_pRank;
	SPWindowStatic*		m_pSelectBG;
	SPWindowButton*		m_pSelectBtn ;

	INSTANCE_ID			m_iStartID;
	INSTANCE_ID			m_iEndID;

	DWORD				m_cOnLineColor;
	DWORD				m_cOffLineColor;

	bool				m_bShowToolTip;
	bool				m_bOnline ;
	bool				m_bSelectable ;
	PGUILD_MEMBER 		m_pUserInfo;
};
