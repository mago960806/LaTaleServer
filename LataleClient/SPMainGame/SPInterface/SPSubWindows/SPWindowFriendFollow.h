// Copyright (C) liuyang
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : liuyang 2008-10-10
//***************************************************************************

#pragma once

#define WIID_FRIEND_FOLLOW_TITLE_IMG					( WIID_FRIEND_FOLLOW + 10 )
#define WIID_FRIEND_FOLLOW_TITLE_TEXT					( WIID_FRIEND_FOLLOW + 11 )
#define WIID_FRIEND_FOLLOW_CLOSE						( WIID_FRIEND_FOLLOW + 12 )
#define WIID_FRIEND_FOLLOW_WHITE_LIST_BUTTON			( WIID_FRIEND_FOLLOW + 13 )
#define WIID_FRIEND_FOLLOW_BLACK_LIST_BUTTON			( WIID_FRIEND_FOLLOW + 14 )
#define WIID_FRIEND_FOLLOW_RECENT_LIST_BUTTON			( WIID_FRIEND_FOLLOW + 19 )//recent
#define WIID_FRIEND_FOLLOW_SLIDER_UP					( WIID_FRIEND_FOLLOW + 15 )
#define WIID_FRIEND_FOLLOW_SLIDER_DOWN					( WIID_FRIEND_FOLLOW + 16 )
#define WIID_FRIEND_FOLLOW_SLIDER						( WIID_FRIEND_FOLLOW + 17 )
#define WIID_FRIEND_FOLLOW_EDIT							( WIID_FRIEND_FOLLOW + 18 )

//-------------------------------------------------------------------------------------------
#define WIID_FRIEND_FOLLOW_WHITE_LIST_PANEL				( WIID_FRIEND_FOLLOW + 20 )
#define WIID_FRIEND_FOLLOW_WHITE_LIST_FIND				( WIID_FRIEND_FOLLOW + 25 )
#define WIID_FRIEND_FOLLOW_WHITE_LIST_DEL				( WIID_FRIEND_FOLLOW + 26 )
#define WIID_FRIEND_FOLLOW_WHITE_LIST_INVITATION		( WIID_FRIEND_FOLLOW + 27 )
#define WIID_FRIEND_FOLLOW_WHITE_LIST_WHISPER			( WIID_FRIEND_FOLLOW + 28 )

#define WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER			6
#define WIID_FRIEND_FOLLOW_WHITE_LIST_USER_PANEL		( WIID_FRIEND_FOLLOW + 100 )
#define WIID_FRIEND_FOLLOW_WHITE_LIST_USER_WINDOW		( WIID_FRIEND_FOLLOW + 110 )
#define WIID_FRIEND_FOLLOW_WHITE_LIST_USER_STEP			10

//--------------------------------------------------------------------------------------Recent
#define WIID_FRIEND_FOLLOW_RECENT_LIST_PANEL			( WIID_FRIEND_FOLLOW + 40 )
#define WIID_FRIEND_FOLLOW_RECENT_LIST_FIND				( WIID_FRIEND_FOLLOW + 45 )
#define WIID_FRIEND_FOLLOW_RECENT_LIST_ADD				( WIID_FRIEND_FOLLOW + 46 )
#define WIID_FRIEND_FOLLOW_RECENT_LIST_INVITATION		( WIID_FRIEND_FOLLOW + 47 )
#define WIID_FRIEND_FOLLOW_RECENT_LIST_WHISPER			( WIID_FRIEND_FOLLOW + 48 )

#define WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER			6
#define WIID_FRIEND_FOLLOW_RECENT_LIST_USER_PANEL		( WIID_FRIEND_FOLLOW + 300 )	//��ʱû��
#define WIID_FRIEND_FOLLOW_RECENT_LIST_USER_WINDOW		( WIID_FRIEND_FOLLOW + 310 )
#define WIID_FRIEND_FOLLOW_RECENT_LIST_USER_STEP		10
//--------------------------------------------------------------------------------------------

#define WIID_FRIEND_FOLLOW_BLACK_LIST_PANEL				( WIID_FRIEND_FOLLOW + 30 )
#define WIID_FRIEND_FOLLOW_BLACK_LIST_FIND				( WIID_FRIEND_FOLLOW + 35 )
#define WIID_FRIEND_FOLLOW_BLACK_LIST_DEL				( WIID_FRIEND_FOLLOW + 36 )

#define WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER			6
#define WIID_FRIEND_FOLLOW_BLACK_LIST_USER_PANEL		( WIID_FRIEND_FOLLOW + 200 )
#define WIID_FRIEND_FOLLOW_BLACK_LIST_USER_WINDOW		( WIID_FRIEND_FOLLOW + 210 )
#define WIID_FRIEND_FOLLOW_BLACK_LIST_USER_STEP			3

#define FRIEND_FOLLOW_BG_COUNT							7

#define FRIEND_FOLLOW_GLOBAL_MSG_ONE_METHOD				5010000

enum FRIEND_FOLLOW_TYPE
{
	FRIEND_FOLLOW_TYPE_NULL	=	0	,
	FRIEND_FOLLOW_TYPE_WHITE			,
	FRIEND_FOLLOW_TYPE_BLACK			,
	FRIEND_FOLLOW_TYPE_RECENT
};

struct FollowSortNameCmp
{
	bool operator()( const std::string& friend1 , const std::string& friend2 ) const
	{
		return friend1 < friend2;
	}
};


typedef std::map< std::string , FRIEND_INFO* , FollowSortNameCmp >	STD_MAP_FRIEND_FOLLOW;

struct RFWName 
{

	TCHAR RecentFindName[LEN_NAME + 1];
	RFWName()
	{
		memset( RecentFindName, 0, sizeof( RecentFindName ) );
	}

};

class SPWindow;
class SPWindowStatic;
class SPWindowButton;
class SPWindowList;
class SPWindowEdit;
class SPWindowFriendFollowUser;
class SPWindowFindMessage;
class SPWindowFindMessageOne;
class SPWindowFindMessageTwo;
struct SPMapInfo;

class SPWindowFriendFollow : public SPWindow 
{

public:
	SPWindowFriendFollow(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowFriendFollow(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowFriendFollow();

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

	// {{ Message Map	
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
	int					OnBlackList			( WPARAM wParam, LPARAM lParam );
	int					OnRecentList		( WPARAM wParam, LPARAM lParam );//Recent

	int					OnSliderUp			( WPARAM wParam, LPARAM lParam );
	int					OnSliderDown		( WPARAM wParam, LPARAM lParam );

	int					OnSlider			( WPARAM wParam, LPARAM lParam );

	int					OnEditSetFocus		( WPARAM wParam, LPARAM lParam );
	int					OnEditKillFocus		( WPARAM wParam, LPARAM lParam );
	//----------------------------------------------------------------------------------------------------------
	int					OnWhiteListAdd		( WPARAM wParam, LPARAM lParam );
	int					OnWhiteListDel		( WPARAM wParam, LPARAM lParam );
	int					OnWhiteListInvitation( WPARAM wParam, LPARAM lParam );
	int					OnWhiteListWhisper	( WPARAM wParam, LPARAM lParam );

	int					OnWhiteLButtonUp		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnWhiteLButtonDoubleUp	( unsigned int iID, WPARAM wParam, LPARAM lParam );

	int					OnWhiteSliderUp			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnWhiteSliderDown		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	//---------------------------------------------------------------------------------------------------------
	int					OnRecentListAdd			( WPARAM wParam, LPARAM lParam );
	int					OnRecentListInvitation	( WPARAM wParam, LPARAM lParam );
	int					OnRecentListWhisper		( WPARAM wParam, LPARAM lParam );

	int					OnRecentLButtonUp		( unsigned int iID, WPARAM wParam, LPARAM lParam );
	int					OnRecentLButtonDoubleUp ( unsigned int iID, WPARAM wParam, LPARAM lParam );

	int					OnRecentSliderUp		( unsigned int iID, WPARAM wParam, LPARAM lParam );
	int					OnRecentSliderDown		( unsigned int iID, WPARAM wParam, LPARAM lParam );
	//---------------------------------------------------------------------------------------------------------
	int					OnWhitePeopleFind		( WPARAM wParam, LPARAM lParam );
	int					OnBlackPeopleFind		( WPARAM wParam, LPARAM lParam );
	int					OnRecentPeopleFind		( WPARAM wParam, LPARAM lParam );
	//---------------------------------------------------------------------------------------------------------
	int					OnBlackListAdd		( WPARAM wParam, LPARAM lParam );
	int					OnBlackListDel		( WPARAM wParam, LPARAM lParam );

	int					OnBlackLButtonUp		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnBlackLButtonDoubleUp	( unsigned int iID, WPARAM wParam, LPARAM lParam );

	int					OnBlackSliderUp		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnBlackSliderDown	( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int					OnUserAddFriend		( WPARAM wParam, LPARAM lParam );
	int					OnUserDelFriend		( WPARAM wParam, LPARAM lParam );
	int					OnUserCheckFriend	( WPARAM wParam, LPARAM lParam );
	int					OnUserFindFriend	( WPARAM wParam, LPARAM lParam );

	int					OnDelFriendName		( WPARAM wParam, LPARAM lParam );

	int					OnFollowResult		( WPARAM wParam, LPARAM lParam );

	void				InitWhite			();
	void				InitBlack			();
	void				InitRecent			();
	void				InitFile			();
	void				StartToFollow		();

	//------------------------------------------------------------------------------------------------------------------
	void				AddWhiteList		( FRIEND_INFO* pFriendInfo );
	void				DelWhiteList		( const char* pFriendName );
	void				DelAllWhiteList		();

	void				UpdateWhiteList		( int iPage );
	void				HideWhiteList		( int iStartNum );
	//------------------------------------------------------------------------------------------------------------------
	void				AddRecentList		( const char* pFriendName );
	void				DelRecentList		( const char* pFriendName );	
	void				DelAllRecentList	();

	void				UpdateRecentList	( int iPage );
	void				HideRecentList		( int iStartNum );
	//------------------------------------------------------------------------------------------------------------------
	void				AddBlackList		( const char* pstrFriendName );
	void				DelBlackList		( const char* pFriendName );
	void				DelAllBlackList		();

	void				UpdateBlackList		( int iPage );
	void				HideBlackList		( int iStartNum );
	//------------------------------------------------------------------------------------------------------------------
	void				PrintErrorMsg		( GLOBAL_STRING_ID uiGlobalStringID , const char* pszName = NULL );

	void				RequestStatusFriend	();

	const char*			GetSelectFriendUserName	( FRIEND_FOLLOW_TYPE eType );

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
	//	������
	//

	FRIEND_FOLLOW_TYPE			m_eFriendType;

	SPTexture*					m_pBGTexture;
	RECT						m_rcBGDest[ FRIEND_FOLLOW_BG_COUNT ];
	RECT						m_rcBGSrc[ FRIEND_FOLLOW_BG_COUNT ];

	SPWindowButton*				m_pWhiteButton;
	SPWindowButton*				m_pBlackButton;
	SPWindowButton*				m_pRecentButton;
	SPWindowSlider*				m_pSlider;
	SPWindowEdit*				m_pEdit;

	std::string					m_strLastAddName;
	bool						m_bDeletePacketSend;
	std::string					m_strLastDelName;
	std::string					m_FollowName;

	//--------------------------------------------------------------------------------------------------------
	SPWindow*					m_pWhiteListPanel;
	SPWindowFriendFollowUser*	m_pWhiteUser[ WIID_FRIEND_FOLLOW_WHITE_LIST_MAX_USER ];

	int							m_iWhitePage;
	STD_MAP_FRIEND_FOLLOW		m_mpWhiteListOnline;
	STD_MAP_FRIEND_FOLLOW		m_mpWhiteListOffline;
	//--------------------------------------------------------------------------------------------------------
	SPWindow*					m_pRecentListPanel;
	SPWindowFriendFollowUser*	m_pRecentUser[ WIID_FRIEND_FOLLOW_RECENT_LIST_MAX_USER ];

	int							m_iRecentPage;
	STD_MAP_FRIEND_FOLLOW		m_mpRecentList;
	//--------------------------------------------------------------------------------------------------------
	SPWindow*					m_pBlackListPanel;
	SPWindowFriendFollowUser*	m_pBlackUser[ WIID_FRIEND_FOLLOW_BLACK_LIST_MAX_USER ];

	int							m_iBlackPage;
	STD_MAP_FRIEND_FOLLOW		m_mpBlackList;

	float						m_fRefreshLimit;
	float						m_fRefreshAccmulateTime;//need

	int							m_iWhiteListSelectNumber;
	int							m_iBlackListSelectNumber;
	//--------------------------------------------------------------------------------------------------------
	int							m_iRecentListSelectNumber;
	//--------------------------------------------------------------------------------------------------------

	USERID						m_iRequesterUserID;
	CHARID						m_iRequesterCharID;
	TCHAR						m_szRequester[ LEN_NAME + 1 ];

	int							m_iSendFriendPos;

	std::deque<RFWName>			m_DeqRecentName;
};
