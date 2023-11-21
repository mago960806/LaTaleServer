// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-8-30    11:18 
//***************************************************************************

#pragma once

#define WIID_FRIEND_TITLE_IMG					( WIID_FRIEND + 10 )
#define WIID_FRIEND_TITLE_TEXT					( WIID_FRIEND + 11 )
#define WIID_FRIEND_CLOSE						( WIID_FRIEND + 12 )
#define WIID_FRIEND_WHITE_LIST_BUTTON			( WIID_FRIEND + 13 )
#define WIID_FRIEND_BLACK_LIST_BUTTON			( WIID_FRIEND + 14 )
#define WIID_FRIEND_SLIDER_UP					( WIID_FRIEND + 15 )
#define WIID_FRIEND_SLIDER_DOWN					( WIID_FRIEND + 16 )
#define WIID_FRIEND_SLIDER						( WIID_FRIEND + 17 )
#define WIID_FRIEND_EDIT						( WIID_FRIEND + 18 )

#define WIID_FRIEND_WHITE_LIST_PANEL			( WIID_FRIEND + 20 )
#define WIID_FRIEND_WHITE_LIST_ADD				( WIID_FRIEND + 25 )
#define WIID_FRIEND_WHITE_LIST_DEL				( WIID_FRIEND + 26 )
#define WIID_FRIEND_WHITE_LIST_INVITATION		( WIID_FRIEND + 27 )
#define WIID_FRIEND_WHITE_LIST_WHISPER			( WIID_FRIEND + 28 )

#define WIID_FRIEND_WHITE_LIST_MAX_USER			6
#define WIID_FRIEND_WHITE_LIST_USER_PANEL		( WIID_FRIEND + 100 )
#define WIID_FRIEND_WHITE_LIST_USER_WINDOW		( WIID_FRIEND + 110 )
#define WIID_FRIEND_WHITE_LIST_USER_STEP		10

#define WIID_FRIEND_BLACK_LIST_PANEL			( WIID_FRIEND + 30 )
#define WIID_FRIEND_BLACK_LIST_ADD				( WIID_FRIEND + 35 )
#define WIID_FRIEND_BLACK_LIST_DEL				( WIID_FRIEND + 36 )

#define WIID_FRIEND_BLACK_LIST_MAX_USER			6
#define WIID_FRIEND_BLACK_LIST_USER_PANEL		( WIID_FRIEND + 200 )
#define WIID_FRIEND_BLACK_LIST_USER_WINDOW		( WIID_FRIEND + 210 )
#define WIID_FRIEND_BLACK_LIST_USER_STEP		3

#define FRIEND_BG_COUNT							7

#define FRIEND_GLOBAL_MSG_ONE_METHOD			5010000

enum FRIEND_TYPE
{
	FRIEND_TYPE_NULL	=	0	,
	FRIEND_TYPE_WHITE			,
	FRIEND_TYPE_BLACK			,
};

struct SortNameCmp
{
	bool operator()( const std::string& friend1 , const std::string& friend2 ) const
	{
		return friend1 < friend2;
	}
};


typedef std::map< std::string , FRIEND_INFO* , SortNameCmp >	STD_MAP_FRIEND;

class SPWindow;
class SPWindowStatic;
class SPWindowButton;
class SPWindowList;
class SPWindowEdit;
class SPWindowFriendUser;

class SPWindowFriend : public SPWindow 
{

public:
	SPWindowFriend(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowFriend(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowFriend();

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

	int					OnCursorEnter		( WPARAM wParam, LPARAM lParam );
	int					OnCursorOut			( WPARAM wParam, LPARAM lParam );
	int					OnCursorMove		( WPARAM wParam, LPARAM lParam );

	int					OnLButtonUp			( WPARAM wParam, LPARAM lParam );
	int					OnLButtonDown		( WPARAM wParam, LPARAM lParam );
	int					OnRButtonUp			( WPARAM wParam, LPARAM lParam );

	int					OnKeyReturn			( WPARAM wParam, LPARAM lParam );
	int					OnPurge				( WPARAM wParam, LPARAM lParam );

	int					OnGetFriend			( WPARAM wParam, LPARAM lParam );
	int					OnAddFriend			( WPARAM wParam, LPARAM lParam );
	int					OnAddRecvFriend		( WPARAM wParam, LPARAM lParam );
	int					OnAcceptFriend		( WPARAM wParam, LPARAM lParam );
	int					OnDelFriend			( WPARAM wParam, LPARAM lParam );
	int					OnStatusFriend		( WPARAM wParam, LPARAM lParam );

	int					OnNoticeYes			( WPARAM wParam, LPARAM lParam );
	int					OnNoticeNo			( WPARAM wParam, LPARAM lParam );

	int					OnClose				( WPARAM wParam, LPARAM lParam );
	int					OnWhiteList			( WPARAM wParam, LPARAM lParam );
	int					OnBlackList			( WPARAM wParam, LPARAM lParam );

	int					OnSliderUp			( WPARAM wParam, LPARAM lParam );
	int					OnSliderDown		( WPARAM wParam, LPARAM lParam );

	int					OnSlider			( WPARAM wParam, LPARAM lParam );

	int					OnEditSetFocus		( WPARAM wParam, LPARAM lParam );
	int					OnEditKillFocus		( WPARAM wParam, LPARAM lParam );
	
	int					OnWhiteListAdd		( WPARAM wParam, LPARAM lParam );
	int					OnWhiteListDel		( WPARAM wParam, LPARAM lParam );
	int					OnWhiteListInvitation( WPARAM wParam, LPARAM lParam );
	int					OnWhiteListWhisper	( WPARAM wParam, LPARAM lParam );

	int					OnWhiteLButtonUp	( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int					OnWhiteSliderUp		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnWhiteSliderDown	( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int					OnBlackListAdd		( WPARAM wParam, LPARAM lParam );
	int					OnBlackListDel		( WPARAM wParam, LPARAM lParam );

	int					OnBlackLButtonUp	( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int					OnBlackSliderUp		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnBlackSliderDown	( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int					OnUserAddFriend		( WPARAM wParam, LPARAM lParam );
	int					OnUserDelFriend		( WPARAM wParam, LPARAM lParam );
	int					OnUserCheckFriend	( WPARAM wParam, LPARAM lParam );

	int					OnDelFriendName		( WPARAM wParam, LPARAM lParam );//[liuyang, 2008.9.1]add
	// }}

	void				InitWhite			();
	void				InitBlack			();

	void				AddWhiteList		( FRIEND_INFO* pFriendInfo );
	void				DelWhiteList		( const char* pFriendName );
	void				DelAllWhiteList		();

	void				UpdateWhiteList		( int iPage );
	void				HideWhiteList		( int iStartNum );

	void				AddBlackList		( const char* pstrFriendName );
	void				DelBlackList		( const char* pFriendName );
	void				DelAllBlackList		();

	void				UpdateBlackList		( int iPage );
	void				HideBlackList		( int iStartNum );

	void				PrintErrorMsg		( GLOBAL_STRING_ID uiGlobalStringID , const char* pszName = NULL );

	void				RequestStatusFriend	();

	const char*			GetSelectFriendUserName	( FRIEND_TYPE eType );

	void				SendAcceptPacket	( UINT8 uiFlag );

	//////////////////////////////////////////////////////////////////////////
	
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
	//	º¯¼öµé
	//

	FRIEND_TYPE					m_eFriendType;

	SPTexture*					m_pBGTexture;
	RECT						m_rcBGDest[ FRIEND_BG_COUNT ];
	RECT						m_rcBGSrc[ FRIEND_BG_COUNT ];

	SPWindowButton*				m_pWhiteButton;
	SPWindowButton*				m_pBlackButton;
	SPWindowSlider*				m_pSlider;
	SPWindowEdit*				m_pEdit;

	std::string					m_strLastAddName;

	bool						m_bDeletePacketSend;
	std::string					m_strLastDelName;

	SPWindow*					m_pWhiteListPanel;
	SPWindowFriendUser*			m_pWhiteUser[ WIID_FRIEND_WHITE_LIST_MAX_USER ];

	int							m_iWhitePage;
	STD_MAP_FRIEND				m_mpWhiteListOnline;
	STD_MAP_FRIEND				m_mpWhiteListOffline;

	SPWindow*					m_pBlackListPanel;
	SPWindowFriendUser*			m_pBlackUser[ WIID_FRIEND_BLACK_LIST_MAX_USER ];

	int							m_iBlackPage;
	STD_MAP_FRIEND				m_mpBlackList;

	float						m_fRefreshLimit;
	float						m_fRefreshAccmulateTime;

	int							m_iWhiteListSelectNumber;
	int							m_iBlackListSelectNumber;

	USERID						m_iRequesterUserID;
	CHARID						m_iRequesterCharID;
	TCHAR						m_szRequester[ LEN_NAME + 1 ];

	int							m_iSendFriendPos;

};
