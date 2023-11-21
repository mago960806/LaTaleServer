// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD LIST WINDOW
// Comment     : 
// Creation    : DURAGON 2007-4-04    11:18 
//***************************************************************************

#pragma once

//------------------------------------------------------------------------------------
#define WIID_GUILD_BASE_BG					(WIID_GUILD_INFO + 1)
#define WIID_GUILD_BASE_TITLE				(WIID_GUILD_INFO + 2)
#define WIID_GUILD_BASE_TITLE_TEXT1			(WIID_GUILD_INFO + 3)
#define WIID_GUILD_BASE_TITLE_ICON1			(WIID_GUILD_INFO + 4)
#define WIID_GUILD_BASE_TITLE_ICON2			(WIID_GUILD_INFO + 5)
#define WIID_GUILD_BASE_TITLE_TEXT2			(WIID_GUILD_INFO + 6)
#define WIID_GUILD_BASE_CLOSE				(WIID_GUILD_INFO + 7)

#define WIID_GUILD_BASE_INVITE_NAME			(WIID_GUILD_INFO + 8)
#define WIID_GUILD_BASE_INVITE_BTN			(WIID_GUILD_INFO + 9)
#define WIID_GUILD_MENU_LIST				(WIID_GUILD_INFO + 11)
#define WIID_GUILD_MENU_STATUS				(WIID_GUILD_INFO + 12)
#define WIID_GUILD_MENU_CROPS				(WIID_GUILD_INFO + 13)


// List
#define WIID_GUILD_LIST_PANNEL				(WIID_GUILD_INFO + 21)
#define WIID_GUILD_LIST_SLIDER				(WIID_GUILD_INFO + 23)
#define WIID_GUILD_LIST_SLIDERUP			(WIID_GUILD_INFO + 24)
#define WIID_GUILD_LIST_SLIDERDOWN			(WIID_GUILD_INFO + 25)
#define WIID_GUILD_LIST_SHOWOFFLINE			(WIID_GUILD_INFO + 27)
#define WIID_GUILD_LIST_SLOGUN_EDIT			(WIID_GUILD_INFO + 28)
#define WIID_GUILD_LIST_SLOGUN_STC			(WIID_GUILD_INFO + 29)
#define WIID_GUILD_LIST_SLOGUNBTN			(WIID_GUILD_INFO + 30)
#define WIID_GUILD_LIST_INVITE_PARTY		(WIID_GUILD_INFO + 31)
#define WIID_GUILD_LIST_WHISPER				(WIID_GUILD_INFO + 32)
#define WIID_GUILD_LIST_LEAVE				(WIID_GUILD_INFO + 33)
#define WIID_GUILD_LIST_EXPEL				(WIID_GUILD_INFO + 34)
// Inside List -> each MAX_GUILD_LIST_VIEW
#define WIID_GUILD_USER_LIST				(WIID_GUILD_INFO + 101)		// (1+7) * 9 = 72
#define WIID_GUILD_LIST_POPUP_BG			(WIID_GUILD_INFO + 181)		// 3
#define WIID_GUILD_LIST_POPUP_DLG			(WIID_GUILD_INFO + 185)		// (1+2) * 5 = 15


// Status
#define WIID_GUILD_STATUS_PANNEL			(WIID_GUILD_INFO + 41)
#define WIID_GUILD_STUS_MASTER_JOB			(WIID_GUILD_INFO + 42)
#define WIID_GUILD_STUS_MASTER_INFO			(WIID_GUILD_INFO + 43)
#define WIID_GUILD_STUS_NAME				(WIID_GUILD_INFO + 44)
#define WIID_GUILD_STUS_LEVEL				(WIID_GUILD_INFO + 45)
#define WIID_GUILD_STUS_PROPERTY			(WIID_GUILD_INFO + 46)
#define WIID_GUILD_STUS_POINTBG				(WIID_GUILD_INFO + 47)
#define WIID_GUILD_STUS_POINTNUM			(WIID_GUILD_INFO + 48)
#define WIID_GUILD_STUS_MSTLIST_SLIDER		(WIID_GUILD_INFO + 55)
#define WIID_GUILD_STUS_MSTLIST_SLIDERUP	(WIID_GUILD_INFO + 56)
#define WIID_GUILD_STUS_MSTLIST_SLIDERDOWN	(WIID_GUILD_INFO + 57)
#define WIID_GUILD_STUS_MSTCHANGE			(WIID_GUILD_INFO + 58)
#define WIID_GUILD_STUS_WHISPER				(WIID_GUILD_INFO + 59)
#define WIID_GUILD_STUS_LEAVE				(WIID_GUILD_INFO + 60)
#define WIID_GUILD_STUS_LEVEL_TOOLTIP		(WIID_GUILD_INFO + 61)
// Inside List -> each MAX_MASTER_LIST_VIEW
#define WIID_GUILD_STUS_MSTLIST			(WIID_GUILD_INFO + 201)		// (1+2) * 4 = 12

// Plant
#define WIID_GUILD_CROPS_PANNEL				(WIID_GUILD_INFO + 71)
#define WIID_GUILD_NO_CROPS					(WIID_GUILD_INFO + 72)
#define WIID_GUILD_CROP_REGISTRY_BG			(WIID_GUILD_INFO + 73)
#define WIID_GUILD_CROP_REGISTRY_AREA		(WIID_GUILD_INFO + 74)
#define WIID_GUILD_CROP_REGISTRY_ARROW		(WIID_GUILD_INFO + 75)
#define WIID_GUILD_CROP_REGISTRY_TEXT		(WIID_GUILD_INFO + 76)
#define WIID_GUILD_CROPS_ICON				(WIID_GUILD_INFO + 77)
#define WIID_GUILD_CROPS_NAME				(WIID_GUILD_INFO + 78)
#define WIID_GUILD_CROPS_IMAGE				(WIID_GUILD_INFO + 79)
#define WIID_GUILD_CROPS_TIME_BAR			(WIID_GUILD_INFO + 80)
#define WIID_GUILD_CROPS_GROWTH_TIME		(WIID_GUILD_INFO + 81)
#define WIID_GUILD_CROPS_MANURE_TIME		(WIID_GUILD_INFO + 82)
#define WIID_GUILD_CROPS_HASTEN_TIME		(WIID_GUILD_INFO + 83)
#define WIID_GUILD_CROPS_LEAVE				(WIID_GUILD_INFO + 84)

//------------------------------------------------------------------------------------
#define MAX_GUILD_LIST_VIEW				7
#define MAX_GUILD_USERLIST_STEP			9
#define MAX_GUILD_GRADE_VIEW			5
#define MAX_MASTER_LIST_VIEW			4
#define MAX_GUILD_SIMPLIST_STEP			3

#define GUILD_BG_IMG					15
#define GUILD_LISTBG_IMG				12
#define GUILD_STUSBG_IMG				12
#define GUILD_CROPSBG_IMG				4


#define GUILD_LEVEL_ABILITY_COUNT		5
struct GUILD_LEVEL_ABILITY
{
	int iCurAbilCount ;
	int iNextAbilCount ;
	std::string strCurAbility[GUILD_LEVEL_ABILITY_COUNT] ;
	std::string strNextAbility[GUILD_LEVEL_ABILITY_COUNT] ;
	void Clear()
	{
		iCurAbilCount = 0 ;
		iNextAbilCount = 0 ;
		for( int i = 0; i < GUILD_LEVEL_ABILITY_COUNT; i++ )
		{
			strCurAbility[i].clear() ;
			strNextAbility[i].clear() ;
		}
	}
};
typedef std::vector< PGUILD_MEMBER >			STD_VEC_GUILD;		// Guild Memver List

// Asending Sort
bool GuildSortByGrade( PGUILD_MEMBER lv, PGUILD_MEMBER rv ) ;


enum GUILD_MENU_TYPE;
class SPItem ;
class SPWindow;
class SPWindowStatic;
class SPWindowButton;
class SPWindowSlider;
class SPWindowList;
class SPWindowEdit;
class SPWindowGuildUser;
class SPWindowGuildSimpleList;
class SPGuildArchive ;
class SPGOBModelUnit ;
class SPWindowGuildInfo : public SPWindow 
{

public:
	SPWindowGuildInfo(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowGuildInfo(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowGuildInfo();

	virtual	void	Init				();
	virtual	void	Clean				();
	virtual void	Show				(bool bWithChild = true);
	virtual void	Hide				(bool bSendServer = true);

	virtual	void	Process				( float fTime );
	virtual	void	Render				( float fTime );

	virtual void	RefreshRelationCoord();

	void			RenderGageNumber(float fTime) ;
	void			RenderLevelTooltip(float fTime) ;

private:
	void			InitGuildList() ;
	void			InitGuildStatus() ;
	void			InitGuildCorps() ;
	//	void			InitGuildLevelTooltip() ;		// 길드 능력치 툴팁
	void			SetGuildLevelTooltip() ;

	void			ReposOwnImage() ;
	void			ShowGuildName() ;
	void			ShowGuildEmblem() ;
	void			ShowRankPopup(bool bShow) ;
	void			ShowSlider(bool bShow) ;
	void			ShowNoCrops(bool bShow) ;

	void			UpdateAthority() ;
	void			UpdateRankControl() ;
	void			UpdatePopupWidthProperty(int iProperty) ;
	void			UpdateStatusInfo() ;
	void			UpdateCropInfo(bool bSet) ;
	void			UpdateCropImage(int iCropLevel);
	void			UpdateCropTime() ;
	void			UpdateSlogun() ;

	void			UpdateMasterInfo() ;
	void			UpdateGuildPoint(int iProtege) ;
	void			SetShowMasterList(int iShowCount, std::vector<PGUILD_MEMBER>* vpShowMemver) ;
	void			SetShowGuildUserList(int iCurList, std::vector<PGUILD_MEMBER>* vpShowMemver) ;
	void			UpdateListScroll(int iTotalCount ,int iShowPage) ;

	bool			ComputeTimeString(UINT32 iTime, char* &pstrTime) ;
	SPItem*			GetDragItem() ;			// Create Crop Drag

	//////////////////////////////////////////////////////////////////////////
	// For Packet	
	bool			SendReqGuildMemberList() ;
	bool			SendChangeGrade(const char* szSelName, int iGrade) ;
	bool			SendGuildExpel(const char* szCharName) ;
	bool			SendChangeManster(const char* szCharName) ;

	//////////////////////////////////////////////////////////////////////////
	// Guild Member List
	void			SetEnableUserList()		{	m_bListInfo = true ;	}
	bool			GetListAvailable()		{	return m_bListInfo ;	}
	bool			GetShowOffline()		{	return m_bShowOffline ;	}
	bool			SetShowOffline(bool bShow) ;

	bool			AddGuildUser(PGUILD_MEMBER guildUser) ;
	void			DeleteMemberList() ;
	bool			DeleteGuildUser(const char* strUserName) ;
	void			RemakeSubMasterList() ;
	PGUILD_MEMBER	FindGuildUser(const char* strUserName) ;
	bool			FindMemberIndex(const char* strUserName, int& iSlot, int& isOn);
	int				CheckCurrentPage(int iPage) ;
	bool			PageToIndex(int iPage, bool& bOnline, int& iSlot) ;

	PGUILD_MEMBER	GetMasterInfo() ;
	int				GetGuildUserCount(int iMode) ;
	int				GetSubMasterCount(int iMode = 0) ;
	bool			UpdateGuildMasterList(int iNewPage, int iUpdateType) ;
	bool			UpdateGuildUserList(int iNewPage, int iUpdateType) ;
	bool			IsOnline(const char* strUserName ) ;
	//bool			AddCropMonster() ;
	//void			CleanCropMonster() ;
	//void			ChangeAnimation(UINT32 iCropImageID) ;

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
		int				OnClose				( WPARAM wParam, LPARAM lParam );	// 닫기
	int				OnGetCurrentMenu	( WPARAM wParam, LPARAM lParam );	// 현재 메뉴 얻기

	int				OnInviteButton		( WPARAM wParam, LPARAM lParam );	// 길드 초대
	int				OnInviteKeyReturn	( WPARAM wParam, LPARAM lParam );
	int				OnGuildListButton	( WPARAM wParam, LPARAM lParam );	// 목록
	int				OnGuildStatusButton	( WPARAM wParam, LPARAM lParam );	// 상태
	int				OnGuildCorpsButton	( WPARAM wParam, LPARAM lParam );	// 작물

	int				OnSliderUp			( WPARAM wParam, LPARAM lParam );
	int				OnSliderDown		( WPARAM wParam, LPARAM lParam );
	int				OnSlider			( WPARAM wParam, LPARAM lParam );

	int				OnEditSetFocus		( WPARAM wParam, LPARAM lParam );
	int				OnEditKillFocus		( WPARAM wParam, LPARAM lParam );
	int				OnMEditSetFocus		( WPARAM wParam, LPARAM lParam );
	int				OnMEditKillFocus		( WPARAM wParam, LPARAM lParam );

	int				OnYes		( WPARAM wParam, LPARAM lParam );
	int				OnNo		( WPARAM wParam, LPARAM lParam );
	int				OnToolTipMove	( WPARAM wParam, LPARAM lParam );
	int				OnToolTipOut	( WPARAM wParam, LPARAM lParam );
	int				OnToolTipMove	( unsigned int iID, WPARAM wParam, LPARAM lParam );
	int				OnToolTipOut	( unsigned int iID, WPARAM wParam, LPARAM lParam );
	int				OnClickTest	( unsigned int iID, WPARAM wParam, LPARAM lParam  ) ;

	//////////////////////////////////////////////////////////////////////////
	// List	
	int				OnGuildListClick	( unsigned int iID, WPARAM wParam, LPARAM lParam  ) ;
	int				OnPopupClick		( unsigned int iID, WPARAM wParam, LPARAM lParam  ) ;
	int				OnShowRankChange	( unsigned int iID, WPARAM wParam, LPARAM lParam );	// 작위수정
	int				OnSliderUp			( unsigned int iID, WPARAM wParam, LPARAM lParam );
	int				OnSliderDown		( unsigned int iID, WPARAM wParam, LPARAM lParam );
	int				OnShowOffline		( WPARAM wParam, LPARAM lParam );	// OffLine 표시
	int				OnSlogunChgBtn		( WPARAM wParam, LPARAM lParam );	// 슬로건 변경
	int				OnInvitePartyButton	( WPARAM wParam, LPARAM lParam );	// 파티초대
	int				OnWhisperButton		( WPARAM wParam, LPARAM lParam );	// 귓속말
	int				OnLeaveGuildButton	( WPARAM wParam, LPARAM lParam );	// 길드탈퇴
	int				OnExpelGuildButton	( WPARAM wParam, LPARAM lParam );	// 강제퇴장

	int				OnGetGuildList		( WPARAM wParam, LPARAM lParam );
	int				OnAddGuildUser		( WPARAM wParam, LPARAM lParam );
	int				OnDelGuildUser		( WPARAM wParam, LPARAM lParam );
	int				OnChangeUserList	( WPARAM wParam, LPARAM lParam );

	//////////////////////////////////////////////////////////////////////////
	// Status
	int				OnMasterListClick	( unsigned int iID, WPARAM wParam, LPARAM lParam  ) ;
	int				OnChangeMasterButton	( WPARAM wParam , LPARAM lParam );
	int				OnLevelCursorEnter	( WPARAM wParam, LPARAM lParam );		// 길드레벨
	int				OnLevelCursorOut		( WPARAM wParam, LPARAM lParam );	//

	int				OnCropCursorEnter	( WPARAM wParam, LPARAM lParam );	// 작물
	int				OnCropCursorOut		( WPARAM wParam, LPARAM lParam );	//
	int				OnCropLButtonUp		( WPARAM wParam, LPARAM lParam );	//

	//////////////////////////////////////////////////////////////////////////
	// Command
	int				OnRecvGuildInfoEnable		( WPARAM wParam , LPARAM lParam );
	int				OnReceiveGuildUserList( WPARAM wParam , LPARAM lParam ) ;
	int				OnReceiveGuildUserJoin( WPARAM wParam , LPARAM lParam )  ;
	int				OnReceiveGuildUserSecede( WPARAM wParam , LPARAM lParam ) ;
	int				OnReceiveChangeGuildUser( WPARAM wParam , LPARAM lParam ) ;
	int				OnRecvGuildCommonInfo		( WPARAM wParam , LPARAM lParam );
	int				OnRecvGuildCropSetting		( WPARAM wParam , LPARAM lParam );
	int				OnGuildCropUpdate			( WPARAM wParam , LPARAM lParam ) ;


	// }}


private:

	SPGuildArchive*				m_pGuildArchive ;

	bool						m_bListInfo ;			// 정보설정 여부
	bool						m_bShowOffline ;		// Offline Show
	int							m_iCurPage ;
	STD_VEC_GUILD				m_vpOnUsers ;		// 랭크별 사용자
	STD_VEC_GUILD				m_vpOffUsers ;		// 랭크별 사용자
	STD_VEC_GUILD				m_vpSubMaster ;		// 부길마 목록

	//////////////////////////////////////////////////////////////////////////
	//	변수들
	SPWindowStatic*				m_pGuildListWnd;
	SPWindowStatic*				m_pGuildStatusWnd;
	SPWindowStatic*				m_pGuildCorpsWnd;
	GUILD_MENU_TYPE				m_iCurMenu ;
	int							m_iCurSelected ;
	int							m_iPreSelected ;
	int							m_iCurSelecteIndex ;
	int							m_iCurPopupSelect ;
	int							m_iGNoticeState ;

	SPTexture*					m_pBGTexture;
	RECT						m_rcBGDest[ GUILD_BG_IMG ];
	RECT						m_rcBGSrc[ GUILD_BG_IMG ];

	bool						m_bShowSlide ;				// 슬라이드 Show
	bool						m_bShowMSTSlide ;			// 마스터 슬라이드
	int							m_iGuildProperty ;			// 길드 유형

	//////////////////////////////////////////////////////////////////////////
	// For List
	SPWindowSlider*				m_pUserListSlider;
	SPWindowEdit*				m_pEditInviteName;
	int							m_iPageCount ;
	int							m_iListSelectNumber;
	RECT						m_rcListBGDst[ GUILD_LISTBG_IMG ];
	RECT						m_rcListBGSrc[ GUILD_LISTBG_IMG ];
	SPWindowGuildUser*			m_pGuildMember[ MAX_GUILD_LIST_VIEW ];
	SPWindowStatic*				m_pGuildGradePopupFront;
	SPWindowStatic*				m_pGuildGradePopup;
	SPWindowGuildSimpleList*	m_pGuildPopupList[MAX_GUILD_GRADE_VIEW] ;
	std::string					m_strExpelName ;
	std::string					m_strChangeMasterName ;

	//////////////////////////////////////////////////////////////////////////
	// for Status
	bool						m_bShowAbilToolTip ;
	RECT						m_rcStatusBGDst[ GUILD_STUSBG_IMG ];
	RECT						m_rcStatusBGSrc[ GUILD_STUSBG_IMG ];
	RECT						m_rcPointGageSrc[2] ;			// 길드경험치 게이지
	RECT						m_rcPointGageDst[2] ;
	SPWindowStatic*				m_pPointNum ;
	SPWindowStatic*				m_pLevelTooltip ;
	SPWindowSlider*				m_pMasterListSlider;
	SPWindowGuildSimpleList*	m_pGuildSubMaster[MAX_GUILD_LIST_VIEW] ;
	GUILD_LEVEL_ABILITY			m_stLevelAbility ;

	//////////////////////////////////////////////////////////////////////////
	// for Corps
	//	SPGOBModelUnit*				m_pkModelUnit;				// 작물 NPC
	//	UINT32						m_iCropImageID;				// 작물 Object Image ID
	//	float						m_fAnimationTime ;			// 작물 Animation
	RECT						m_rcCorpsBGDst[ GUILD_CROPSBG_IMG ];
	RECT						m_rcCorpsBGSrc[ GUILD_CROPSBG_IMG ];
	SPItem*						m_pCropItem;				// 작물 아이템

	enum GUILD_WND_NOTICE {
		GUILD_NOTICE_EMPTY		= 0 ,
		GUILD_EXPEL_NOTICE		= 1 ,
		GUILD_MSTCHANGE_NOTICE	= 2 ,
	} ;
	GUILD_WND_NOTICE			m_iNoticeState ;		// Notice Box State
};