// ***************************************************************
//  SPWindowTalkingRoom   version:  1.0   ¡¤  date: 03/06/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPWindowTalkingRoom_h__
#define SPWindowTalkingRoom_h__

#pragma once

#define WIID_TALKING_ROOM_TITLE_IMG									(WIID_TALKING_ROOM + 1)
#define WIID_TALKING_ROOM_CLOSE										(WIID_TALKING_ROOM + 2)
#define WIID_TALKING_ROOM_TYPE_ICON									(WIID_TALKING_ROOM + 3)
#define WIID_TALKING_ROOM_NUMBER_100								(WIID_TALKING_ROOM + 4)
#define WIID_TALKING_ROOM_NUMBER_010								(WIID_TALKING_ROOM + 5)
#define WIID_TALKING_ROOM_NUMBER_001								(WIID_TALKING_ROOM + 6)
#define WIID_TALKING_ROOM_TYPE_TEXT									(WIID_TALKING_ROOM + 7)
#define WIID_TALKING_ROOM_SUBJECT_TEXT								(WIID_TALKING_ROOM + 8)

#define WIID_TALKING_ROOM_SEND_ALL									(WIID_TALKING_ROOM + 14)
#define WIID_TALKING_ROOM_SEND_WHISPER								(WIID_TALKING_ROOM + 15)

#define WIID_TALKING_ROOM_EDIT_ALL									(WIID_TALKING_ROOM + 16)
#define WIID_TALKING_ROOM_EDIT_WHISPER								(WIID_TALKING_ROOM + 17)

#define WIID_TALKING_ROOM_WHISPER_IMG								(WIID_TALKING_ROOM + 18)

#define WIID_TALKING_ROOM_ADVERTISING_TEXT_IMG						(WIID_TALKING_ROOM + 19)
#define WIID_TALKING_ROOM_ADVERTISING_LIST							(WIID_TALKING_ROOM + 20)
#define WIID_TALKING_ROOM_ADVERTISING_UP							(WIID_TALKING_ROOM + 21)
#define WIID_TALKING_ROOM_ADVERTISING_DOWN							(WIID_TALKING_ROOM + 22)
#define WIID_TALKING_ROOM_ADVERTISING_SLIDER						(WIID_TALKING_ROOM + 23)

#define WIID_TALKING_ROOM_CHAT_LIST									(WIID_TALKING_ROOM + 24)
#define WIID_TALKING_ROOM_CHAT_UP									(WIID_TALKING_ROOM + 25)
#define WIID_TALKING_ROOM_CHAT_DOWN									(WIID_TALKING_ROOM + 26)
#define WIID_TALKING_ROOM_CHAT_SLIDER								(WIID_TALKING_ROOM + 27)

#define WIID_TALKING_ROOM_MIN										(WIID_TALKING_ROOM + 30)
#define WIID_TALKING_ROOM_MAX										(WIID_TALKING_ROOM + 31)

#define WIID_TALKING_ROOM_MAX_PANEL									(WIID_TALKING_ROOM + 32)

#define WIID_TALKING_ROOM_MAX_BLIND_LEFT_TOP						(WIID_TALKING_ROOM + 33)
#define WIID_TALKING_ROOM_MAX_BLIND_CENTER_TOP						(WIID_TALKING_ROOM + 34)
#define WIID_TALKING_ROOM_MAX_BLIND_LEFT_BOTTOM						(WIID_TALKING_ROOM + 35)

#define WIID_TALKING_ROOM_MAX_CLOSE									(WIID_TALKING_ROOM + 36)

#define WIID_TALKING_ROOM_MAX_WHITE_BG								(WIID_TALKING_ROOM + 37)
#define WIID_TALKING_ROOM_MAX_LINE_1								(WIID_TALKING_ROOM + 38)
#define WIID_TALKING_ROOM_MAX_JOIN_PERSON_COUNT						(WIID_TALKING_ROOM + 39)
#define WIID_TALKING_ROOM_MAX_LINE_2								(WIID_TALKING_ROOM + 40)
#define WIID_TALKING_ROOM_MAX_JOIN_PERSON_TEXT						(WIID_TALKING_ROOM + 41)

#define WIID_TALKING_ROOM_MAX_PLAYER_PAGE_LEFT						(WIID_TALKING_ROOM + 42)
#define WIID_TALKING_ROOM_MAX_PLAYER_PAGE_RIGHT						(WIID_TALKING_ROOM + 43)
#define WIID_TALKING_ROOM_MAX_PLAYER_PAGE_TEXT						(WIID_TALKING_ROOM + 44)

#define WIID_TALKING_ROOM_MAX_PLAYER_ACTION_PANEL					(WIID_TALKING_ROOM + 45)
#define WIID_TALKING_ROOM_MAX_PLAYER_ACTION_WHISPER					(WIID_TALKING_ROOM + 46)
#define WIID_TALKING_ROOM_MAX_PLAYER_ACTION_PARTY_INVITE			(WIID_TALKING_ROOM + 47)
#define WIID_TALKING_ROOM_MAX_PLAYER_ACTION_GUILD_INVITE			(WIID_TALKING_ROOM + 48)
#define WIID_TALKING_ROOM_MAX_PLAYER_ACTION_FRIEND_ADD				(WIID_TALKING_ROOM + 49)
#define WIID_TALKING_ROOM_MAX_PLAYER_ACTION_MASTER_CHANGE			(WIID_TALKING_ROOM + 50)
#define WIID_TALKING_ROOM_MAX_PLAYER_ACTION_FORCE_LEAVE				(WIID_TALKING_ROOM + 51)
#define WIID_TALKING_ROOM_MAX_PLAYER_ACTION_LINE_1					(WIID_TALKING_ROOM + 52)
#define WIID_TALKING_ROOM_MAX_PLAYER_ACTION_LINE_2					(WIID_TALKING_ROOM + 53)
#define WIID_TALKING_ROOM_MAX_PLAYER_ACTION_LINE_3					(WIID_TALKING_ROOM + 54)
#define WIID_TALKING_ROOM_MAX_PLAYER_ACTION_LINE_4					(WIID_TALKING_ROOM + 55)
#define WIID_TALKING_ROOM_MAX_PLAYER_ACTION_LINE_5					(WIID_TALKING_ROOM + 56)

#define WIID_TALKING_ROOM_MAX_SETUP_IMG								(WIID_TALKING_ROOM + 57)
#define WIID_TALKING_ROOM_MAX_SETUP_LINE							(WIID_TALKING_ROOM + 58)
#define WIID_TALKING_ROOM_MAX_ADVERTISING							(WIID_TALKING_ROOM + 59)
#define WIID_TALKING_ROOM_MAX_SEARCH								(WIID_TALKING_ROOM + 60)
#define WIID_TALKING_ROOM_MAX_MY_INFO_SETUP							(WIID_TALKING_ROOM + 61)
#define WIID_TALKING_ROOM_MAX_ROOM_INFO_SETUP						(WIID_TALKING_ROOM + 62)

//////////////////////////////////////////////////////////////////////////

#define WIID_TALKING_ROOM_ADVERTISING_PANEL							(WIID_TALKING_ROOM + 70)
#define WIID_TALKING_ROOM_ADVERTISING_BG							(WIID_TALKING_ROOM + 71)
#define WIID_TALKING_ROOM_ADVERTISING_TITLE_IMG						(WIID_TALKING_ROOM + 72)
#define WIID_TALKING_ROOM_ADVERTISING_CLOSE							(WIID_TALKING_ROOM + 73)
#define WIID_TALKING_ROOM_ADVERTISING_EDIT_BG_LEFT					(WIID_TALKING_ROOM + 74)
#define WIID_TALKING_ROOM_ADVERTISING_EDIT_BG_CENTER				(WIID_TALKING_ROOM + 75)
#define WIID_TALKING_ROOM_ADVERTISING_EDIT_BG_RIGHT					(WIID_TALKING_ROOM + 76)
#define WIID_TALKING_ROOM_ADVERTISING_EDIT							(WIID_TALKING_ROOM + 77)
#define WIID_TALKING_ROOM_ADVERTISING_HELP_IMG						(WIID_TALKING_ROOM + 78)
#define WIID_TALKING_ROOM_ADVERTISING_OK							(WIID_TALKING_ROOM + 79)
#define WIID_TALKING_ROOM_ADVERTISING_CANCEL						(WIID_TALKING_ROOM + 80)

#define WIID_TALKING_ROOM_MY_INFO_SETUP_PANEL						(WIID_TALKING_ROOM + 81)
#define WIID_TALKING_ROOM_MY_INFO_SETUP_BG							(WIID_TALKING_ROOM + 82)
#define WIID_TALKING_ROOM_MY_INFO_SETUP_TITLE_IMG					(WIID_TALKING_ROOM + 83)
#define WIID_TALKING_ROOM_MY_INFO_SETUP_CLOSE						(WIID_TALKING_ROOM + 84)
#define WIID_TALKING_ROOM_MY_INFO_SETUP_MY_INFO_SEARCH				(WIID_TALKING_ROOM + 85)
#define WIID_TALKING_ROOM_MY_INFO_SETUP_MY_INFO_SEARCH_IMG			(WIID_TALKING_ROOM + 86)
#define WIID_TALKING_ROOM_MY_INFO_SETUP_CHAT_REQUEST				(WIID_TALKING_ROOM + 87)
#define WIID_TALKING_ROOM_MY_INFO_SETUP_CHAT_REQUEST_IMG			(WIID_TALKING_ROOM + 88)
#define WIID_TALKING_ROOM_MY_INFO_SETUP_MY_STYLE_OPEN				(WIID_TALKING_ROOM + 89)
#define WIID_TALKING_ROOM_MY_INFO_SETUP_MY_STYLE_OPEN_IMG			(WIID_TALKING_ROOM + 90)
#define WIID_TALKING_ROOM_MY_INFO_SETUP_OK							(WIID_TALKING_ROOM + 91)
#define WIID_TALKING_ROOM_MY_INFO_SETUP_CANCEL						(WIID_TALKING_ROOM + 92)

#define WIID_TALKING_ROOM_INFO_SETUP_PANEL							(WIID_TALKING_ROOM + 93)
#define WIID_TALKING_ROOM_INFO_SETUP_BG								(WIID_TALKING_ROOM + 94)
#define WIID_TALKING_ROOM_INFO_SETUP_TITLE_IMG						(WIID_TALKING_ROOM + 95)
#define WIID_TALKING_ROOM_INFO_SETUP_CLOSE							(WIID_TALKING_ROOM + 96)
#define WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_IMG					(WIID_TALKING_ROOM + 97)
#define WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_EDIT					(WIID_TALKING_ROOM + 98)
#define WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_EDIT_BG_LEFT			(WIID_TALKING_ROOM + 99)
#define WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_EDIT_BG_CENTER			(WIID_TALKING_ROOM + 100)
#define WIID_TALKING_ROOM_INFO_SETUP_SUBJECT_EDIT_BG_RIGHT			(WIID_TALKING_ROOM + 101)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_IMG						(WIID_TALKING_ROOM + 102)

#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TITLE_LEFT		(WIID_TALKING_ROOM + 103)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TITLE_CENTER		(WIID_TALKING_ROOM + 104)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TITLE_RIGHT		(WIID_TALKING_ROOM + 105)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_ARROW_LEFT		(WIID_TALKING_ROOM + 106)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_ARROW_RIGHT		(WIID_TALKING_ROOM + 107)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TEXT_BG_LEFT		(WIID_TALKING_ROOM + 108)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TEXT_BG_CENTER	(WIID_TALKING_ROOM + 109)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TEXT_BG_RIGHT	(WIID_TALKING_ROOM + 110)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_GENDER_TEXT				(WIID_TALKING_ROOM + 111)

#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TITLE_LEFT			(WIID_TALKING_ROOM + 112)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TITLE_CENTER		(WIID_TALKING_ROOM + 113)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TITLE_RIGHT			(WIID_TALKING_ROOM + 114)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_ARROW_LEFT			(WIID_TALKING_ROOM + 115)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_ARROW_RIGHT			(WIID_TALKING_ROOM + 116)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TEXT_BG_LEFT		(WIID_TALKING_ROOM + 117)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TEXT_BG_CENTER		(WIID_TALKING_ROOM + 118)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TEXT_BG_RIGHT		(WIID_TALKING_ROOM + 119)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_AGE_TEXT				(WIID_TALKING_ROOM + 120)

#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TITLE_LEFT		(WIID_TALKING_ROOM + 121)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TITLE_CENTER		(WIID_TALKING_ROOM + 122)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TITLE_RIGHT		(WIID_TALKING_ROOM + 123)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT_BG_LEFT		(WIID_TALKING_ROOM + 124)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT_BG_CENTER	(WIID_TALKING_ROOM + 125)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT_BG_RIGHT	(WIID_TALKING_ROOM + 126)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT				(WIID_TALKING_ROOM + 127)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_NUMBER_TEXT_LIMIT		(WIID_TALKING_ROOM + 128)

#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TITLE_LEFT		(WIID_TALKING_ROOM + 129)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TITLE_CENTER	(WIID_TALKING_ROOM + 130)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TITLE_RIGHT	(WIID_TALKING_ROOM + 131)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_TEXT			(WIID_TALKING_ROOM + 132)

#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_CHANGE			(WIID_TALKING_ROOM + 133)
#define WIID_TALKING_ROOM_INFO_SETUP_DETAIL_PASSWORD_CHANGE_IMG		(WIID_TALKING_ROOM + 134)

#define WIID_TALKING_ROOM_INFO_SETUP_OK								(WIID_TALKING_ROOM + 135)
#define WIID_TALKING_ROOM_INFO_SETUP_CANCEL							(WIID_TALKING_ROOM + 136)

//////////////////////////////////////////////////////////////////////////

#define TALKING_ROOM_PLAYER_COUNT									(10)

#define WIID_TALKING_ROOM_PLAYER_SELECT_ICON						(WIID_TALKING_ROOM						+	300)
#define WIID_TALKING_ROOM_PLAYER_CLASS_ICON							(WIID_TALKING_ROOM_PLAYER_SELECT_ICON	+	TALKING_ROOM_PLAYER_COUNT + 1)
#define WIID_TALKING_ROOM_PLAYER_NAME_TEXT							(WIID_TALKING_ROOM_PLAYER_CLASS_ICON	+	TALKING_ROOM_PLAYER_COUNT + 1)

//////////////////////////////////////////////////////////////////////////

enum ROOM_STYLE_ID;
enum MY_STYLE_ID;
enum ROOM_TYPE_ID;
enum CHAT_LDT_TYPE;

enum TALKING_ROOM_BG
{
	TALKING_ROOM_BG_LEFT_TOP			=	0	,
	TALKING_ROOM_BG_CENTER_TOP					,
	TALKING_ROOM_BG_RIGHT_TOP					,

	TALKING_ROOM_BG_LEFT_MIDDLE					,
	TALKING_ROOM_BG_CENTER_MIDDLE				,
	TALKING_ROOM_BG_RIGHT_MIDDLE				,

	TALKING_ROOM_BG_LEFT_BOTTOM					,
	TALKING_ROOM_BG_CENTER_BOTTOM				,
	TALKING_ROOM_BG_RIGHT_BOTTOM				,

	TALKING_ROOM_BG_LINE_1						,
	TALKING_ROOM_BG_SUB							,

	TALKING_ROOM_BG_ADVERTISING_LEFT_TOP		,
	TALKING_ROOM_BG_ADVERTISING_CENTER_TOP		,
	TALKING_ROOM_BG_ADVERTISING_RIGHT_TOP		,

	TALKING_ROOM_BG_ADVERTISING_LEFT_MIDDLE		,
	TALKING_ROOM_BG_ADVERTISING_CENTER_MIDDLE	,
	TALKING_ROOM_BG_ADVERTISING_RIGHT_MIDDLE	,

	TALKING_ROOM_BG_ADVERTISING_LEFT_BOTTOM		,
	TALKING_ROOM_BG_ADVERTISING_CENTER_BOTTOM	,
	TALKING_ROOM_BG_ADVERTISING_RIGHT_BOTTOM	,

	TALKING_ROOM_BG_COUNT						,
};

enum USER_LIST_COLOR
{
	USER_LIST_COLOR_DEFAULT				=	0	,
	USER_LIST_COLOR_ME							,
	USER_LIST_COLOR_COUNT						,
};

struct ROOM_MEMBER_UI
{
	ROOM_MEMBER		m_stRoomMember;
	USER_LIST_COLOR	m_eUserListColor;
};

typedef std::vector< ROOM_MEMBER_UI >			STD_VECTOR_USER;

struct CHAT_TOSS_DATA;

class SPWindow;

class SPWindowTalkingRoom : public SPWindow
{
public:
	SPWindowTalkingRoom										( WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowTalkingRoom										( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowTalkingRoom							();

	virtual void			Init							();
	virtual void			Clean							();

	virtual void			Process							( float fTime );
	virtual void			Render							( float fTime );

	virtual void			Show							( bool bWithChild = true );
	virtual void			Hide							( bool bSendServer = true );
	virtual void			Close							();

	virtual void			RefreshRelationCoord			();

private:
	void					ForceShow						( bool bShow );
	void					ReposOwnImage					();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	//	Common
	virtual		int			OnPurge							( WPARAM wParam, LPARAM lParam );
	virtual		int			OnForceClose					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnTab							( WPARAM wParam, LPARAM lParam );
	virtual		int			OnClose							( WPARAM wParam, LPARAM lParam );
	virtual		int			OnMin							( WPARAM wParam, LPARAM lParam );
	virtual		int			OnMax							( WPARAM wParam, LPARAM lParam );
	virtual		int			OnKeyReturn						( WPARAM wParam, LPARAM lParam );
	virtual		int			OnKeyEscape						( WPARAM wParam, LPARAM lParam );

	virtual		int			OnPacketModify					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketOut						( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketForceOut				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketChangeMaster			( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketMessage					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketMemberList				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketAdvert					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRefreshOption					( WPARAM wParam, LPARAM lParam );

	virtual		int			OnListNextLine					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnGetMessage					( WPARAM wParam, LPARAM lParam );

	virtual		int			OnNoticeYes						( WPARAM wParam, LPARAM lParam );
	virtual		int			OnNoticeNo						( WPARAM wParam, LPARAM lParam );

	virtual		int			OnKeyDown						( WPARAM wParam, LPARAM lParam );	

	virtual		int			OnEditSetFocus					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnEditKillFocus					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomSetupPWEditSetFocus		( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomSetupNumberEditKillFocus	( WPARAM wParam, LPARAM lParam );

	virtual		int			OnAdvertListLButtonDblUp		( WPARAM wParam, LPARAM lParam );
	virtual		int			OnAdvertisingListUp				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnAdvertisingListDown			( WPARAM wParam, LPARAM lParam );

	virtual		int			OnChatListUp					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnChatListDown					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnChatListLButtonUp				( WPARAM wParam, LPARAM lParam );

	virtual		int			OnUserListLeft					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnUserListRight					( WPARAM wParam, LPARAM lParam );

	virtual		int			OnBtnAdvertising				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnSearch						( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnMyInfoSetup				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnRoomInfoSetup				( WPARAM wParam, LPARAM lParam );

	virtual		int			OnBtnAdvertisingOK				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnAdvertisingCancel			( WPARAM wParam, LPARAM lParam );

	virtual		int			OnBtnMyInfoSearch				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnChatRequest				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnMyStyleOpen				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnMyInfoSetupOK				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnMyInfoSetupCancel			( WPARAM wParam, LPARAM lParam );

	virtual		int			OnBtnInfoSetupGenderLeft		( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnInfoSetupGenderRight		( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnInfoSetupAgeLeft			( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnInfoSetupAgeRight			( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnInfoSetupPasswordChange	( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnInfoSetupOK				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnBtnInfoSetupCancel			( WPARAM wParam, LPARAM lParam );

	virtual		int			OnAdvertisingSlider				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnChatSlider					( WPARAM wParam, LPARAM lParam );

	virtual		int			OnSend							( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnPlayer						( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnPlayerClear					( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnPlayerAction					( unsigned int iID, WPARAM wParam , LPARAM lParam );

	// }}

protected:

	void					InitWindowMax					( SPWindow* pParentWindow );
	void					InitWindowPlayerList			( SPWindow* pParentWindow );
	void					InitWindowPlayerAction			( SPWindow* pParentWindow );
	void					InitWindowAdvertising			( SPWindow* pParentWindow );
	void					InitWindowMyInfoSetup			( SPWindow* pParentWindow );
	void					InitWindowRoomSetup				( SPWindow* pParentWindow );

	INSTANCE_ID				GetActiveMenuWindow				( INSTANCE_ID eInstanceStart , INSTANCE_ID eInstanceEnd , int iStep );

	void					SetEditWindow					( INSTANCE_ID eInstance );

	void					AddChatString					( CHAT_TOSS_DATA& stChatData , D3DXCOLOR cColor , SPWindow* pWindowList , SPWindow* pWindowSlider , SPWindow* pWindowUp , SPWindow* pWindowDown );

	void					ClearPlayerAction				();

	void					SetModalWindowShow				( SPWindow* pWindow , bool bShow );
	void					SetDefaultWindow				( INSTANCE_ID eInstanceID );
	void					SetDefaultWindowAdvertising		();
	void					SetDefaultWindowMyInfoSetup		();
	void					SetDefaultWindowRoomInfoSetup	();

	void					SetButtonCheck					( SPWindow* pWindow , bool bCheck );
	void					SetButtonCheckInv				( SPWindow* pWindow );

	void					SetViewTitle					( CHAT_LDT_TYPE eChatLDTType , int iStyleID , int& iIndex , SPWindow* pWindow );
	void					SetAutoRoomSubject				( SPWindow* pWindow , int iIndex );

	void					PrintErrorMsg					( GLOBAL_STRING_ID uiGlobalStringID );

	void					SendPacketOut					( UINT32 uiID , UINT32 uiRoomNo , UINT32 uiCharID , UINT8 iBanOut );
	void					SendChangeMaster				( UINT32 uiRoomID , UINT32 uiRoomNo , UINT32 uiTargetCharID );

	void					UpdateUserList					();
	void					SetUserList						( int iIndex , ROOM_MEMBER_UI* pRoomUser , FONT_ENUMERATE eFontType );
	void					UpdateUserListPage				( int iCurPage );
	void					UpdateUserCount					();

	void					UpdateSubject					();

	void					AddRommUser						( ROOM_MEMBER_UI& stRoomUser , bool bInsertFirst );
	bool					DeleteRoomUser					( ROOM_MEMBER_UI& stRoomUser );
	void					ChangeLeaderRoomUser			( ROOM_MEMBER_UI& stRoomUser );

	bool					SetWhisperMode					( const char* pstrID );

	void					ProcessBan						( float fTime );
	void					ResetBan						();

	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//

	SPTexture*								m_pBGTexture;
	RECT									m_rcBGDest[ TALKING_ROOM_BG_COUNT ];
	RECT									m_rcBGSrc[ TALKING_ROOM_BG_COUNT ];

	bool									m_bUse;

	bool									m_bNextLine;

	int										m_iCurIndexRoomModify[ ROOM_STYLE_MAX ];

	STD_VECTOR_USER							m_vRoomUserList;

	int										m_iRoomUserCurPage;

	bool									m_bMasterChange;
	bool									m_bForceOut;

	std::string								m_strCommand;

	CHATROOM_INFO							m_stAdvertChatRoomInfo;
	bool									m_bRoomOut;
	bool									m_bRoomOutBan;

	std::string								m_strLastMsg;
	int										m_iEqualMsgCount;

	std::vector<std::string>				m_vstrHistoryBuffer;
	char									m_cInxHistory;

	D3DXCOLOR								m_cUserListTextColor[ USER_LIST_COLOR_COUNT ];

	float									m_fBanLimitTime;
	float									m_fBanAccmulateTime;

};

#endif // SPWindowTalkingRoom_h__