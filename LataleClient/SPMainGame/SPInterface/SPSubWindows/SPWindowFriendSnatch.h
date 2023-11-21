// Copyright (C) liuyang
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : liuyang 2008-10-10
//***************************************************************************

#pragma once

#define WIID_FRIEND_SNATCH_TITLE_IMG					( WIID_FRIEND_SNATCH + 10 )
#define WIID_FRIEND_SNATCH_TITLE_TEXT					( WIID_FRIEND_SNATCH + 11 )
#define WIID_FRIEND_SNATCH_CLOSE						( WIID_FRIEND_SNATCH + 12 )
#define WIID_FRIEND_SNATCH_WHITE_LIST_BUTTON			( WIID_FRIEND_SNATCH + 13 )
#define WIID_FRIEND_SNATCH_BLACK_LIST_BUTTON			( WIID_FRIEND_SNATCH + 14 )
#define WIID_FRIEND_SNATCH_RECENT_LIST_BUTTON			( WIID_FRIEND_SNATCH + 19 )//recent
#define WIID_FRIEND_SNATCH_SLIDER_UP					( WIID_FRIEND_SNATCH + 15 )
#define WIID_FRIEND_SNATCH_SLIDER_DOWN					( WIID_FRIEND_SNATCH + 16 )
#define WIID_FRIEND_SNATCH_SLIDER						( WIID_FRIEND_SNATCH + 17 )
#define WIID_FRIEND_SNATCH_EDIT							( WIID_FRIEND_SNATCH + 18 )

//-------------------------------------------------------------------------------------------
#define WIID_FRIEND_SNATCH_WHITE_LIST_PANEL				( WIID_FRIEND_SNATCH + 20 )
#define WIID_FRIEND_SNATCH_WHITE_LIST_FIND				( WIID_FRIEND_SNATCH + 25 )
#define WIID_FRIEND_SNATCH_WHITE_LIST_DEL				( WIID_FRIEND_SNATCH + 26 )
#define WIID_FRIEND_SNATCH_WHITE_LIST_INVITATION		( WIID_FRIEND_SNATCH + 27 )
#define WIID_FRIEND_SNATCH_WHITE_LIST_WHISPER			( WIID_FRIEND_SNATCH + 28 )

#define WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER			6
#define WIID_FRIEND_SNATCH_WHITE_LIST_USER_PANEL		( WIID_FRIEND_SNATCH + 100 )
#define WIID_FRIEND_SNATCH_WHITE_LIST_USER_WINDOW		( WIID_FRIEND_SNATCH + 110 )
#define WIID_FRIEND_SNATCH_WHITE_LIST_USER_STEP			10

//--------------------------------------------------------------------------------------Recent
#define WIID_FRIEND_SNATCH_RECENT_LIST_PANEL			( WIID_FRIEND_SNATCH + 40 )
#define WIID_FRIEND_SNATCH_RECENT_LIST_FIND				( WIID_FRIEND_SNATCH + 45 )
#define WIID_FRIEND_SNATCH_RECENT_LIST_ADD				( WIID_FRIEND_SNATCH + 46 )
#define WIID_FRIEND_SNATCH_RECENT_LIST_INVITATION		( WIID_FRIEND_SNATCH + 47 )
#define WIID_FRIEND_SNATCH_RECENT_LIST_WHISPER			( WIID_FRIEND_SNATCH + 48 )

#define WIID_FRIEND_SNATCH_RECENT_LIST_MAX_USER			6
#define WIID_FRIEND_SNATCH_RECENT_LIST_USER_PANEL		( WIID_FRIEND_SNATCH + 300 )	//‘› ±√ª”√
#define WIID_FRIEND_SNATCH_RECENT_LIST_USER_WINDOW		( WIID_FRIEND_SNATCH + 310 )
#define WIID_FRIEND_SNATCH_RECENT_LIST_USER_STEP		10
//--------------------------------------------------------------------------------------------

#define WIID_FRIEND_SNATCH_BLACK_LIST_PANEL				( WIID_FRIEND_SNATCH + 30 )
#define WIID_FRIEND_SNATCH_BLACK_LIST_FIND				( WIID_FRIEND_SNATCH + 35 )
#define WIID_FRIEND_SNATCH_BLACK_LIST_DEL				( WIID_FRIEND_SNATCH + 36 )

#define WIID_FRIEND_SNATCH_BLACK_LIST_MAX_USER			6
#define WIID_FRIEND_SNATCH_BLACK_LIST_USER_PANEL		( WIID_FRIEND_SNATCH + 200 )
#define WIID_FRIEND_SNATCH_BLACK_LIST_USER_WINDOW		( WIID_FRIEND_SNATCH + 210 )
#define WIID_FRIEND_SNATCH_BLACK_LIST_USER_STEP			3

#define FRIEND_SNATCH_BG_COUNT							7

#define FRIEND_SNATCH_GLOBAL_MSG_ONE_METHOD				5010000

enum FRIEND_SNATCH_TYPE
{
	FRIEND_SNATCH_TYPE_NULL	=	0	,
	FRIEND_SNATCH_TYPE_WHITE
};

struct SnatchSortNameCmp
{
	bool operator()( const std::string& friend1 , const std::string& friend2 ) const
	{
		return friend1 < friend2;
	}
};


typedef std::map< std::string , FRIEND_INFO* , SnatchSortNameCmp >	STD_MAP_FRIEND_SNATCH;

class SPWindow;
class SPWindowStatic;
class SPWindowButton;
class SPWindowList;
class SPWindowEdit;
class SPWindowFriendSnatchUser;
class SPWindowFindMessage;
class SPWindowFindMessageOne;
class SPWindowFindMessageTwo;
struct SPMapInfo;

class SPWindowFriendSnatch : public SPWindow 
{

public:
	SPWindowFriendSnatch(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowFriendSnatch(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowFriendSnatch();

	virtual	void		Init				();
	virtual	void		Clean				();
	virtual void		Show				(bool bWithChild = true);
	virtual void		Hide				(bool bSendServer = true);

	virtual	void		Process				( float fTime );
	virtual	void		Render				( float fTime );

	virtual void RefreshRelationCoord();

private:
	void ReposOwnImage(); // metalgeni [5/25/2006]

protected:

	// Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int				    OnCursorEnter		( WPARAM wParam, LPARAM lParam );
	int					OnCursorOut			( WPARAM wParam, LPARAM lParam );
	int					OnCursorMove		( WPARAM wParam, LPARAM lParam );

	int					OnLButtonUp				( WPARAM wParam, LPARAM lParam );
	int					OnLButtonDown			( WPARAM wParam, LPARAM lParam );
	int					OnRButtonUp			( WPARAM wParam, LPARAM lParam );

	int					OnKeyReturn			( WPARAM wParam, LPARAM lParam );
	int					OnPurge				( WPARAM wParam, LPARAM lParam );

	int					OnGetFriend			( WPARAM wParam, LPARAM lParam );
	int					OnAddFriend			( WPARAM wParam, LPARAM lParam );
	int					OnAddRecvFriend		( WPARAM wParam, LPARAM lParam );
	int					OnAcceptFriend		( WPARAM wParam, LPARAM lParam );
	int					OnDelFriend			( WPARAM wParam, LPARAM lParam );
	int					OnStatusFriend		( WPARAM wParam, LPARAM lParam );
	int					OnFindInfoShow		( WPARAM wParam, LPARAM lParam );

	int					OnNoticeYes			( WPARAM wParam, LPARAM lParam );
	int					OnNoticeNo			( WPARAM wParam, LPARAM lParam );

	int					OnClose				( WPARAM wParam, LPARAM lParam );
	int					OnWhiteList			( WPARAM wParam, LPARAM lParam );

	int					OnSliderUp			( WPARAM wParam, LPARAM lParam );
	int					OnSliderDown		( WPARAM wParam, LPARAM lParam );

	int					OnSlider			( WPARAM wParam, LPARAM lParam );

	int					OnEditSetFocus		( WPARAM wParam, LPARAM lParam );
	int					OnEditKillFocus		( WPARAM wParam, LPARAM lParam );
	//----------------------------------------------------------------------------------------------------------
	int					OnWhiteLButtonUp		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnWhiteLButtonDoubleUp	( unsigned int iID, WPARAM wParam, LPARAM lParam );

	int					OnWhiteSliderUp			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnWhiteSliderDown		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	//---------------------------------------------------------------------------------------------------------
	int					OnWhitePeopleFind		( WPARAM wParam, LPARAM lParam );
	//---------------------------------------------------------------------------------------------------------

	int					OnUserFindFriend	( WPARAM wParam, LPARAM lParam );

	int					OnDelFriendName		( WPARAM wParam, LPARAM lParam );

	int					OnSnatchResult		( WPARAM wParam, LPARAM lParam );

	void				InitWhite			();
	void				StartToSnatch		();

	//------------------------------------------------------------------------------------------------------------------
	void				AddWhiteList		( FRIEND_INFO* pFriendInfo );
	void				DelWhiteList		( const char* pFriendName );
	void				DelAllWhiteList		();

	void				UpdateWhiteList		( int iPage );
	void				HideWhiteList		( int iStartNum );

	void				PrintErrorMsg		( GLOBAL_STRING_ID uiGlobalStringID , const char* pszName = NULL );

	void				RequestStatusFriend	();

	const char*			GetSelectFriendUserName	( FRIEND_SNATCH_TYPE eType );

	void				SendAcceptPacket	( UINT8 uiFlag );

	//////////////////////////////////////////////////////////////////////////
	//
	enum GLOBAL_STRING_FRIEND
	{
		GS_FRIEND					=	0			,
		GS_FRIEND_WHITE_ALREADY		=	5006001		,
		GS_FRIEND_BLACK_ALREADY		=	5006003		,
		GS_FRIEND_WHITE_MYSELF		=	5006005		,
		GS_FRIEND_BLACK_MYSELF		=	5006006		,

		GS_FRIEND_WHITE_ADD			=	5016011		,
		GS_FRIEND_BLACK_ADD			=	5016022		,
		GS_FRIEND_WHITE_DEL			=	5016015		,
		GS_FRIEND_BLACK_DEL			=	5016023		,

		GS_FRIEND_WHITE_ACCEPT		=	5016012		,
		GS_FRIEND_WHITE_ACCEPT_YES	=	5016013		,
		GS_FRIEND_WHITE_ACCEPT_NO	=	5016014		,

	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	
	//

	FRIEND_SNATCH_TYPE			m_eFriendType;

	SPTexture*					m_pBGTexture;
	RECT						m_rcBGDest[ FRIEND_SNATCH_BG_COUNT ];
	RECT						m_rcBGSrc[ FRIEND_SNATCH_BG_COUNT ];

	SPWindowButton*				m_pWhiteButton;
	SPWindowSlider*				m_pSlider;
	SPWindowEdit*				m_pEdit;

	std::string					m_strLastAddName;
	bool						m_bDeletePacketSend;
	std::string					m_strLastDelName;
	std::string					m_SnatchName;

	//--------------------------------------------------------------------------------------------------------
	SPWindow*					m_pWhiteListPanel;
	SPWindowFriendSnatchUser*	m_pWhiteUser[ WIID_FRIEND_SNATCH_WHITE_LIST_MAX_USER ];

	int							m_iWhitePage;
	STD_MAP_FRIEND_SNATCH		m_mpWhiteListOnline;
	STD_MAP_FRIEND_SNATCH		m_mpWhiteListOffline;

	float						m_fRefreshLimit;
	float						m_fRefreshAccmulateTime;//need

	int							m_iWhiteListSelectNumber;
	//--------------------------------------------------------------------------------------------------------
	int							m_iRecentListSelectNumber;
	//--------------------------------------------------------------------------------------------------------

	USERID						m_iRequesterUserID;
	CHARID						m_iRequesterCharID;
	TCHAR						m_szRequester[ LEN_NAME + 1 ];

	int							m_iSendFriendPos;

};
