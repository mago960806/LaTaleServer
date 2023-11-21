// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD CREATE WINDOW
// Comment     : 
// Creation    : DURAGON 2007-4-04    11:18 
//***************************************************************************

#pragma once

//------------------------------------------------------------------------------------
// Property Select
#define WIID_CREATEGUILD_CLOSE				(WIID_GUILD_CREATE + 1)

#define WIID_CREATEGUILD_SELECT_BG			(WIID_GUILD_CREATE + 11)
#define WIID_CREATEGUILD_SELECT_DESC_BG		(WIID_GUILD_CREATE + 12)
#define WIID_CREATEGUILD_SELECT_DESCRIPT	(WIID_GUILD_CREATE + 13)
#define WIID_CREATEGUILD_SELECT_NEXT		(WIID_GUILD_CREATE + 14)
#define WIID_CREATEGUILD_SELECT_CANCEL		(WIID_GUILD_CREATE + 15)
#define WIID_CREATEGUILD_SELECT_01			(WIID_GUILD_CREATE + 16)
#define WIID_CREATEGUILD_SELECT_02			(WIID_GUILD_CREATE + 17)
#define WIID_CREATEGUILD_SELECT_03			(WIID_GUILD_CREATE + 18)

// Create
#define WIID_CREATEGUILD_NAME_BG			(WIID_GUILD_CREATE + 21)
#define WIID_CREATEGUILD_NAME_TAG_L			(WIID_GUILD_CREATE + 22)
#define WIID_CREATEGUILD_NAME_TAG_R			(WIID_GUILD_CREATE + 23)
#define WIID_CREATEGUILD_NAME_SELECT_ICON	(WIID_GUILD_CREATE + 24)
//#define WIID_CREATEGUILD_NAME_SELECT_IMG	(WIID_GUILD_CREATE + 25)
#define WIID_CREATEGUILD_NAME_SELECT_TYPE	(WIID_GUILD_CREATE + 26)
#define WIID_CREATEGUILD_NAME_EDIT_BG		(WIID_GUILD_CREATE + 27)
#define WIID_CREATEGUILD_NAME_EDIT			(WIID_GUILD_CREATE + 28)
#define WIID_CREATEGUILD_OK					(WIID_GUILD_CREATE + 29)
#define WIID_CREATEGUILD_CANCEL				(WIID_GUILD_CREATE + 30)

#define WIID_CREATEGUILD_END				(WIID_GUILD_CREATE + 50)

//------------------------------------------------------------------------------------

#define GUILD_CREATE_BG_IMG			12
#define GUILD_SELECT_TYPE_IMG		7
#define GUILD_MAKE_NAME_IMG			3

#define	GUILD_CREATE_ITEM_ICON01	190170201
#define	GUILD_CREATE_ITEM_ICON02	190170202
#define	GUILD_CREATE_ITEM_ICON03	190170203


enum GUILD_CREATE_SHOW_PAGE
{
	SHOW_GUILD_SELECT = 0,
	SHOW_GUILD_MAKE,
} ;

class SPWindow;
class SPWindowStatic;
class SPWindowButton;
class SPWindowEdit;

class SPWindowCreateGuild : public SPWindow 
{

public:
	SPWindowCreateGuild(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowCreateGuild(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowCreateGuild();

	virtual	void	Init				();
	virtual	void	Clean				();
	virtual void	Show				(bool bWithChild = true);
	virtual void	Hide				(bool bSendServer = true);

	virtual	void	Process				( float fTime );
	virtual	void	Render				( float fTime );

	virtual void	RefreshRelationCoord();

	void			ChangeShowWnd(int iShowPage) ;

private:
	void			Init_SelectType() ;
	void			Init_MakeName() ;

	void			ReposOwnImage() ;
	void			RenderBackGround(float fTime);
	void			ShowGuildCreateNotice(int iRet) ;
	void			ShowGuildTypeIcon() ;
	void			SetSelectedType(int iGType) ;

	bool			CheckAbuseGuildName(const char* strGName) ;

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
		int				OnClose				( WPARAM wParam, LPARAM lParam );	// 닫기

	int				OnEditSetFocus		( WPARAM wParam, LPARAM lParam );
	int				OnEditKillFocus		( WPARAM wParam, LPARAM lParam );

	int				OnSelectProperty	( unsigned int iID, WPARAM wParam , LPARAM lParam );	// 성향선택
	int				OnPropertyCommit	( WPARAM wParam , LPARAM lParam );	// Next

	int				OnCommit			( WPARAM wParam, LPARAM lParam );	// 확인
	int				OnCreateResult		( WPARAM wParam, LPARAM lParam );	// 생성 결과
	int 			OnNameKeyReturn( WPARAM wParam, LPARAM lParam );

	int				OnTest			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	// }}

	//////////////////////////////////////////////////////////////////////////
	//	변수들
	int				m_iShowPage ;			// 보고 있는 페이지
	int				m_iSelectedProperty ;	// 성향

	SPTexture*		m_pBGTexture;
	RECT			m_rcBGDest[ GUILD_CREATE_BG_IMG ];
	RECT			m_rcBGSrc[ GUILD_CREATE_BG_IMG ];
	RECT			m_rcTypeDest[ GUILD_SELECT_TYPE_IMG ];
	RECT			m_rcTypeSrc[ GUILD_SELECT_TYPE_IMG ];
	RECT			m_rcNameDest[ GUILD_MAKE_NAME_IMG ];
	RECT			m_rcNameSrc[ GUILD_MAKE_NAME_IMG ];

	SPWindowEdit*	m_pEditName ;			// 길드 이름
	SPWindowStatic*	m_pTypeWnd ;
	SPWindowStatic*	m_pMakeWnd ;

};