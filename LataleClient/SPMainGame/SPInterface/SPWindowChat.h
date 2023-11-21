// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 오전 10:26:19
//***************************************************************************

#pragma once

#ifdef _DEFINE_DEPRECATED_HASH_CLASSES
#undef _DEFINE_DEPRECATED_HASH_CLASSES
#define _DEFINE_DEPRECATED_HASH_CLASSES 0	
#endif

#define WIID_CHAT_BTN_UP				(WIID_CHAT + 1)
#define WIID_CHAT_BTN_DOWN				(WIID_CHAT + 2)
#define WIID_CHAT_BTN_LATEST			(WIID_CHAT + 3)

#define WIID_CHAT_EDIT_PANEL			(WIID_CHAT + 10)
#define WIID_CHAT_EDIT_WHISPER			(WIID_CHAT + 11)
#define WIID_CHAT_EDIT					(WIID_CHAT + 12)
#define WIID_CHAT_BUTTON_IME_ENGLISH	(WIID_CHAT + 13)
#define WIID_CHAT_BUTTON_IME_HANGUL		(WIID_CHAT + 14)
#define WIID_CHAT_SLIDER_SIZE			(WIID_CHAT + 15)
#define WIID_CHAT_HISTORY				(WIID_CHAT + 16)

#define WIID_CHAT_BUGLE_ICON			(WIID_CHAT + 17)
#define WIID_CHAT_BUGLE_NAME			(WIID_CHAT + 18)
#define WIID_CHAT_BUGLE_BG				(WIID_CHAT + 19)
#define WIID_CHAT_BUGLECHANNEL_HL		(WIID_CHAT + 20)
#define WIID_CHAT_BUGLESELECT_HL		(WIID_CHAT + 21)

#define WIID_CHAT_TYPE_LIST				(WIID_CHAT + 100)

#define WIID_CHAT_NULL					(WIID_CHAT + 888)

#define CHAT_MENU_COUNT					(5+1)
#define CHAT_BG_COUNT					2

#define CHAT_GLOBAL_MSG_ONE_METHOD		5010000
#define CHAT_GLOBAL_MSG_TWO_METHOD		5020000
#define CHAT_GLOBAL_MSG_END_METHOD		5030000

#define CHAT_HISTORY_COUNT				28
#define CHAT_HISTORY_VIEW_COUNT			4
#define CHAT_HISTORY_LINE_HEIGHT		14
typedef int ( *fnOutPut )( const int, char[], char[] );



enum CHAT_TYPE
{
	CHAT_TYPE_NULL	=	0	,
	CHAT_TYPE_CHAT			,
	CHAT_TYPE_EMOTICON		,
};

enum CHAT_SLIDER
{
	SLIDER_TOP		=	0	,
	SLIDER_TOP_HIT			,
	SLIDER_BOTTOM			,
	SLIDER_BOTTOM_HIT		,
	SLIDER_MAX				,
};

enum CHAT_STRING_ID
{
	CHAT_STRING_NULL				=	0		,
	CHAT_STRING_AROUNDREGION					,
	CHAT_STRING_FRIENDLIST						,
	CHAT_STRING_GUILD							,
	CHAT_STRING_TEAM							,
	CHAT_STRING_WHISPER							,
	CHAT_STRING_BUGLE							,

	CHAT_STRING_HELP				=	2003	,

	CHAT_STRING_TEAM_ADD			=	201		,
	CHAT_STRING_TEAM_LEAVE			=	202		,
	CHAT_STRING_WHITE_FRIEND_ADD	=	301		,
	CHAT_STRING_WHITE_FRIEND_DEL	=	302		,
	CHAT_STRING_BLACK_FRIEND_ADD	=	303		,
	CHAT_STRING_BLACK_FRIEND_DEL	=	304		,
	CHAT_STRING_TRADE				=	401		,
	CHAT_STRING_GUILD_ADD			=	501		,
	CHAT_STRING_GUILD_LEAVE			=	502		,
	CHAT_STRING_FATIGUE				=	601		,
	CHAT_STRING_CHAT_MACRO			=	602		,
};

struct CHAT_SEND_APPEND_DATA
{
	BROADCAST_SCOPE		m_eScope;
	std::string			m_strWhisper;
};

struct CHAT_TOSS_DATA
{
	BROADCAST_SCOPE		m_eScope;
	std::string			m_strMsg;
	D3DXCOLOR			m_cColor;
	std::string			m_strID;
	bool				m_bWhisper;
	int					m_iChannelNumber;
	int					m_iCategory;
};

struct CHAT_COMMAND
{
	CHAT_TYPE		m_eChatType;
	int				m_iChatTypeID;
};

struct CHAT_TYPE_ID
{
	std::string			m_strType;
	BROADCAST_SCOPE		m_eScope;
	D3DXCOLOR			m_Color;
	std::string			m_strInputFormat;
	std::string			m_strOutputFormat;
};
	
typedef	stdext::hash_map< std::string , CHAT_COMMAND >	STDEXT_HASH_MAP_CHAT_COMMAND;
typedef std::map< CHAT_STRING_ID , CHAT_TYPE_ID >		STD_MAP_CHAT_TYPE_ID;
typedef std::map< CHAT_STRING_ID , std::string >		STD_MAP_CHAT_COMMAND_STRING;

#ifndef GetRandom
#define GetRandom(X)						((int)( (float)(X) * rand() / ( RAND_MAX ) ) )
#endif

#define CHAT_ADVERTISEMENT_SEPARATOR_KEY	1000

enum CHAT_ADVERTISEMENT_ID
{
	CHAT_ADVERTISEMENT_NULL		=	0		,
	CHAT_ADVERTISEMENT_STAGE				,
	CHAT_ADVERTISEMENT_MAPGROUP				,
	CHAT_ADVERTISEMENT_END		=	4		,
};

struct CHAT_ADVERTISEMENT
{
	int				m_iStageID;
	int				m_iMapGroupID;
	bool			m_bShow;

	float			m_fLimitTime;
	int				m_iColor;
	D3DXCOLOR		m_cColor;
	std::string		m_strMessage;
};

enum COUNTRY_TYPE;

typedef std::vector<CHAT_ADVERTISEMENT>						STD_VECTOR_CHAT_ADVERTISEMENT;
typedef std::map< int , STD_VECTOR_CHAT_ADVERTISEMENT >		STD_MAP_CHAT_ADVERTISEMENT;

class SPWindow;
class SPPlayer;
class SPWindowSlider;
class SPWindowButton;
class SPWindowList;
struct LIST_ITEM;
struct LIST_ICON;
class VisualBuglePlayerInfo;
#include <SPWindowBugleSelect.h>	/* for class BugleInfo */

class SPWindowChat : public SPWindow
{
public:
	SPWindowChat(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowChat(WND_ID WndClassID, INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowChat();

	virtual void InitBugleGUI( void );
	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);	

	virtual int  PerformMouseMove		( int iX, int iY);

    bool SetBugleInfo( const BugleInfo &info );

	bool SetVisualBugleInfo( std::vector< CONTAINER_ITEM >& Items, const CHARACTER_INFO2 &info );

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	virtual int		OnListCursorEnter		( WPARAM wParam, LPARAM lParam );
	virtual int		OnListCursorOut			( WPARAM wParam, LPARAM lParam );
	virtual int		OnListLButtonUp			( WPARAM wParam, LPARAM lParam );
	virtual int		OnListRButtonUp			( WPARAM wParam, LPARAM lParam );
	virtual int		OnChatGetBugle			( WPARAM wParam, LPARAM lParam );
	virtual int		OnChatGetVisualBugle	( WPARAM wParam, LPARAM lParam );
	virtual int		OnChatGetMsg			( WPARAM wParam, LPARAM lParam );
	virtual int		OnChatMsg				( WPARAM wParam, LPARAM lParam );
	virtual int		OnChatToggle			( WPARAM wParam, LPARAM lParam );
	virtual int		OnChatInsertMsgEdit		( WPARAM wParam, LPARAM lParam );
	virtual int		OnChatSetWhisper		( WPARAM wParam, LPARAM lParam );
	virtual int		OnChatGetNotice			( WPARAM wParam, LPARAM lParam );
	virtual int		OnChatSendMessage		( WPARAM wParam, LPARAM lParam );
	virtual int		OnChatGetCommand		( WPARAM wParam, LPARAM lParam );
	virtual int		OnSetFocusMsg			( WPARAM wParam, LPARAM lParam );
	virtual int		OnKillFocusMsg			( WPARAM wParam, LPARAM lParam );
	int				OnKeyDown				( WPARAM wParam, LPARAM lParam );	

	virtual int		OnSetFocusWhisper		( WPARAM wParam, LPARAM lParam );
	virtual int		OnKillFocusWhisper		( WPARAM wParam, LPARAM lParam );
	virtual int		OnIMEUpEnglish			( WPARAM wParam, LPARAM lParam );
	virtual int		OnIMEUpHangul			( WPARAM wParam, LPARAM lParam );

	virtual int		OnScrollUp				( WPARAM wParam, LPARAM lParam );
	virtual int		OnScrollDown			( WPARAM wParam, LPARAM lParam );
	virtual int		OnScrollLatest			( WPARAM wParam, LPARAM lParam );

	virtual int		OnScrollUpLBDown		( WPARAM wParam, LPARAM lParam );
	virtual int		OnScrollDownLBDown		( WPARAM wParam, LPARAM lParam );

	virtual int		OnScrollUpCursorOut		( WPARAM wParam, LPARAM lParam );
	virtual int		OnScrollDownCursorOut	( WPARAM wParam, LPARAM lParam );
	
	virtual int		OnSliderPageNum			( WPARAM wParam, LPARAM lParam );
	virtual int		OnSliderSetFocus		( WPARAM wParam, LPARAM lParam );
	virtual int		OnSliderKillFocus		( WPARAM wParam, LPARAM lParam );
	virtual int		OnSliderLButtonDown		( WPARAM wParam, LPARAM lParam );
	virtual int		OnSliderLButtonUp		( WPARAM wParam, LPARAM lParam );
	virtual int		OnSliderRButtonUp		( WPARAM wParam, LPARAM lParam );
	virtual int		OnSliderCursorMove		( WPARAM wParam, LPARAM lParam );
	virtual int		OnSliderCursorOut		( WPARAM wParam, LPARAM lParam );

	virtual int		OnPurge					( WPARAM wParam, LPARAM lParam );
	virtual int		OnTab					( WPARAM wParam, LPARAM lParam );
	virtual int		OnEscape				( WPARAM wParam, LPARAM lParam );

	virtual int		OnWheelUp				( WPARAM wParam, LPARAM lParam );
	virtual int		OnWheelDown				( WPARAM wParam, LPARAM lParam );

	virtual int		OnCursorEnter			( WPARAM wParam, LPARAM lParam );
	virtual int		OnCursorOut				( WPARAM wParam, LPARAM lParam );

	virtual int		OnKillFocus				( WPARAM wParam, LPARAM lParam );
	virtual int		OnGetCalcLumpString		( WPARAM wParam, LPARAM lParam );

	virtual int		OnTypeList				( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual int		OnBugleChooserFocus		( unsigned int iID, WPARAM wParam, LPARAM lParam );
	virtual int		OnBugleChooserLostFocus	( unsigned int iID, WPARAM wParam, LPARAM lParam );
	
	/// callback for bugle type choose event
	virtual int		OnBugleChooser( WPARAM wParam, LPARAM lParam );

	// }}

protected:
	BOOL			LoadChatCommandLDT();
	BOOL			LoadChatTypeLDT();
	bool			LoadChatAdvertisement();

	CHAT_TYPE		ParserChat			( BROADCAST_SCOPE& eScope , const char* pstrChatMsg , const char* pszWhisperID , std::string& strMessage , std::string& strWhisperName );
	void			SendChatMsg			( const char* pstrChatMsg );
	void			SendPacketMsg		( BROADCAST_SCOPE& eScope , const char* pstrChatMsg , const char* pszWhisperID , std::string& strMessage , std::string& strWhisperName );
	CHAT_TYPE_ID*	GetChatType			( int iTypeNumber );
	void			AddWhisperID		( const char* pstrID );
	CHAT_TYPE		CheckChatCommand	( CHAT_COMMAND& tChatCommand );
	CHAT_TYPE		ChangeChatType		( BROADCAST_SCOPE& eScope , const char* pstrCommand , BOOL bSendMsg );

	void			InitActionKey		();
	void			ProcessActionKey	();

	void			PrintErrorMsg		( TCHAR* pstrOutMsg , GLOBAL_STRING_ID uiGlobalStringID , const char* pstrSender , const char* pstrReceiver );

	void			CheckLatestMsg		();

	void			BtnPush				( int iBtnDown );

	CHAT_COMMAND*	GetChatCommand		( const char* pstrCommand );
	void			MakeHelpMessage		();

	const char*		GetChatString		( CHAT_STRING_ID eStringID );

	BOOL			IsChatCommand		( BROADCAST_SCOPE& eScope , const char* pstrCommand , const char* pszWhisperID , std::string& strMessage , std::string& strWhisperName );

	void			ProcessAdvertisement( float fTime );
	void			RunAdvertisement	();

	STD_VECTOR_CHAT_ADVERTISEMENT*		GetAdvertisement( int iStageID , int iMapGroupID );

	void			ProcessRepeat		( float fTime );
	void			SetRepeat			();
	bool			GetRepeat			();

	void			ReposOwnImage		();
	bool			IsCursorInCheck		();

	void			AddChatString		( CHAT_TOSS_DATA& stChatData );
	void			AddChatString( BROADCAST_SCOPE eScope , const char* pstrChat, 
		D3DXCOLOR cColor , const char* pstrID, 
		bool bWhisper , int iChannelNumber, UINT32 uiChatItemID = 170200101UL );

    bool _sendBugleMsg( const char *pstrChatMsg );
	bool addVisualBugle(  VisualBuglePlayerInfo &playerInfo, const char *strMsg,
		UINT32 uiItemID, bool bPreview);

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	SPWindow*						m_pMsgEditPanel;
	SPWindowEdit*					m_pWhisperEdit;
	SPWindowEdit*					m_pMsgEdit;
	SPWindowButton*					m_pIMEModeEnglish;
	SPWindowButton*					m_pIMEModeHangul;
	SPWindowList*					m_pHistory;

	RECT							m_ptMsgEditPosSize[3];
	RECT							m_ptMsgEditNormalSrc[3];
	RECT							m_ptMsgEditFocusSrc[3];
	POINT							m_rcMsgEditMargin[3];

	BOOL							m_bBGRender;
	SPTexture*						m_pBGTexture;
	RECT							m_rcBGDest[ CHAT_BG_COUNT ];
	RECT							m_rcBGSrc[ CHAT_BG_COUNT ];

	BROADCAST_SCOPE					m_eScope;
//	BROADCAST_SCOPE					m_eOldScope;
	std::string						m_strWhisperName;
	std::string						m_strMessage;
	std::string						m_strRelpyName;

	STDEXT_HASH_MAP_CHAT_COMMAND	m_hmChatCommand;
	STD_MAP_CHAT_TYPE_ID			m_mChatTypeID;
	STD_MAP_CHAT_COMMAND_STRING		m_mChatString;

	SPWindowSlider*					m_pSizeSlider;
	BOOL							m_bLButtonDown;

	RECT							m_rcSizeSliderSrc[ SLIDER_MAX ];
	BOOL							m_bSizeSliderMax;

	BOOL							m_bInsertChatMsg;
	float							m_fBlinkLimitTime;
	float							m_fBlinkAccmulateTime;
	SPWindowButton*					m_pBtnLatest;
	bool							m_bBtnLatestStatus;

	int								m_iBtnDown;
	float							m_fDownLimitTime;
	float							m_fDownAccmulateTime;

	std::string						m_strHelpMessage;
	D3DXCOLOR						m_cHelpMessage;
	std::string						m_strHelpCommand;

	int								m_iChatAdvertismentType;
	STD_MAP_CHAT_ADVERTISEMENT		m_mChatAdvertisment;

	float							m_fChatAdvertismentLimitTime;
	float							m_fChatAdvertismentAccmulateTime;

	std::vector<std::string>		m_vstrHistoryBuffer;
	char							m_cInxHistory;

	bool							m_bRepeat;
	float							m_fRepeatLimitTime;
	float							m_fRepeatAccmulateTime;

	COUNTRY_TYPE					m_eCountryType;

	SPWindow*						m_pWindowTalkingRoom;


	//--------------------------------------------------
	SPWindowStatic	*m_ptrImageBugleIcon;
	SPWindowStatic	*m_ptrImageBugleName;
	SPWindowStatic	*m_ptrImageBugleBg;
	SPWindowStatic	*m_ptrBugleChannelHL;	// HL is the ab. for HightLight
	SPWindowStatic	*m_ptrBugleSelectHL;
	BugleInfo		m_bugleInfo;
	SPPlayer		*m_pPlayer;
	//--------------------------------------------------


private:
	void _addBugleString(UINT32 uiChatItemID, int iChannelNumber, LIST_ITEM& stAddListItem, LIST_ICON& stAddListIcon);
	int  GmOutPutNeed( BROADCAST_SCOPE scope, char msg[], char SenderName[] );
};
