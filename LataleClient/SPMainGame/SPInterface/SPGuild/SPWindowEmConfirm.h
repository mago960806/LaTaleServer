// Copyright (C) DURAGON
//***************************************************************************
// System Name : Buying Confirm
// Comment     : 
// Creation    : DURAGON 2007-9-8
//***************************************************************************

#pragma once

//------------------------------------------------------------------------------------
// Property Select
#define WIID_BUY_CONFIRM_CLOSE			(WIID_BUY_CONFIRM + 1)
#define WIID_BUY_CONFIRM_COMMIT			(WIID_BUY_CONFIRM + 2)

#define WIID_BUY_CONFIRM_TITLE_ICON		(WIID_BUY_CONFIRM + 4)
#define WIID_BUY_CONFIRM_TITLE			(WIID_BUY_CONFIRM + 5)
#define WIID_BUY_CONFIRM_DESC			(WIID_BUY_CONFIRM + 6)
#define WIID_BUY_CONFIRM_NEEDLIST		(WIID_BUY_CONFIRM + 10)	// 8

#define WIID_BUY_CONFIRM_END			(WIID_BUY_CONFIRM + 50)
//------------------------------------------------------------------------------------

#define CONFIRM_BG_IMAGE				11

class SPWindow;
class SPWindowStatic;
class SPWindowButton;
struct EMBLEM_INFO ;
class SPWindowBuyConfirm : public SPWindow 
{

public:
	SPWindowBuyConfirm(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowBuyConfirm(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowBuyConfirm();

	virtual	void	Init				();
	virtual	void	Clean				();
	virtual void	Show				(bool bWithChild = true);
	virtual void	Hide				(bool bSendServer = true);

	virtual	void	Process				( float fTime );
	virtual	void	Render				( float fTime );

	virtual void	RefreshRelationCoord();

	// For Title & Description
	//void			SetTitleImage(const char* pszImage, RECT rcSrc) ;
	//void			SetDescIcon(const char* pszImage, RECT rcSrc) ;
	//void			SetDescText(const char* pszText) ;

	// For List
	bool			SetNeedList(EMBLEM_INFO* pEmblem) ;
	//bool			InsertListItem( const char* pszImage, RECT rcImageSrc, const char* pszItemText , D3DXCOLOR color ) ;
	//bool			InsertListItem( int iItemID, int iItemCount, D3DXCOLOR color ) ;
	//bool			SetBGImage(const char* pszImage, RECT rcImageSrc ) ;
	//void			SetListMargin( int iWidthMargin, int iHeightMargin ) ;

private:
	void			ReposOwnImage() ;
	void			RenderBackGround(float fTime);

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int				OnClose				( WPARAM wParam, LPARAM lParam );	// 닫기
	int				OnCommit			( WPARAM wParam, LPARAM lParam );	// 확인

	int				OnListResize		( WPARAM wParam, LPARAM lParam );	// 
	// }}

	//////////////////////////////////////////////////////////////////////////
	//	변수들
	SPWindow*		m_pNeedList ;
	SPTexture*		m_pBGTexture;
	void*			m_pItemInfo ;
	int				m_iListSY ;			// 리스트의 시작위치
	int				m_iListSize ;		// 리트스의 크기
	RECT			m_rcBGDst[ CONFIRM_BG_IMAGE ];
	RECT			m_rcBGSrc[ CONFIRM_BG_IMAGE ];
};
