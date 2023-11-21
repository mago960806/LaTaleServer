// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 2006-06-19 리뉴얼 AJJIYA
// Creation    : AJJIYA 2005-6-30    17:18 
//***************************************************************************

#pragma once

#define SKILL_BG_COUNT							9

#define SKILL_MAIN_MENU_MAX						10
#define SKILL_SUB_MENU_MAX						10

#define SKILL_CLASS_SEPARATION					100
#define SKILL_SUB_SEPARATION					100

#define SKILL_GRID_X_COUNT						4
#define SKILL_GRID_Y_COUNT						3

#define SKILL_GRID_X_SIZE						56
#define SKILL_GRID_Y_SIZE						68

#define SKILL_GRID_X_OFFSET						10
#define SKILL_GRID_Y_OFFSET						69

#define SKILL_UNIT_X_OFFSET						12
#define SKILL_UNIT_Y_OFFSET						14

#define LS_REQUIRE_COUNT						8

class SPWindow;
class SPWindowButton;
class SPUIUnitManager;

class SPWindowSkill : public SPWindow 
{
public:
	SPWindowSkill								( WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowSkill								( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowSkill						();

	virtual	void		Init					();
	virtual	void		Clean					();
	virtual void		Show					( bool bWithChild = true );
	virtual void		Hide					( bool bSendServer = true );

	virtual	void		Process					( float fTime );
	virtual	void		Render					( float fTime );
	virtual void		RefreshRelationCoord	();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int 		OnBeginMove				( WPARAM, LPARAM );	// metalgeni [6/2/2006]
	virtual int 		OnEndMove				( WPARAM, LPARAM );	
	virtual int 		OnMove					( WPARAM, LPARAM );

	int					OnDragnDropEnd			( WPARAM wParam, LPARAM lParam );

	int					OnWheelUp				( WPARAM wParam, LPARAM lParam );
	int					OnWheelDown				( WPARAM wParam, LPARAM lParam );

	int					OnPurge					( WPARAM wParam, LPARAM lParam );
	int					OnWndPosUpdate			( WPARAM wParam, LPARAM lParam );

	int					OnStatusRefresh			( WPARAM wParam, LPARAM lParam );
	int					OnSkillUpdate			( WPARAM wParam, LPARAM lParam );

	int					OnGetSkillLVUP			( WPARAM wParam, LPARAM lParam );
	int					OnGetSkillLearn			( WPARAM wParam, LPARAM lParam );

	int					OnLearnSkillYes			( WPARAM wParam, LPARAM lParam );

	int					OnClose					( WPARAM wParam, LPARAM lParam );

	int					OnSliderUp				( WPARAM wParam, LPARAM lParam );
	int					OnSliderDown			( WPARAM wParam, LPARAM lParam );

	int					OnSliderBar				( WPARAM wParam, LPARAM lParam );

	int					OnGetUIUnitManager		( WPARAM wParam, LPARAM lParam );

	int					OnMainMenuLButtonUp		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnSubMenuLButtonUp		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnContentLButtonUp		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnContentLButtonDown	( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnContentCursorMove		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnContentCursorEnter	( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnContentCursorOut		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnContentWheelUp		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnContentWheelDown		( unsigned int iID, WPARAM wParam , LPARAM lParam );


	// }}

	//////////////////////////////////////////////////////////////////////////
	
	enum SKILL_INSTANCE_ID
	{
		WIID_SKILL_CLOSE						=	WIID_SKILL	+	1						,
		WIID_SKILL_ICON_IMAGE																,
		WIID_SKILL_TEXT_IMAGE																,
		WIID_SKILL_POINT_LABEL																,
		WIID_SKILL_POINT_TEXT																,
		WIID_SKILL_SCROLL_UP																,
		WIID_SKILL_SCROLL_DOWN																,
		WIID_SKILL_SCROLL_BAR																,

		WIID_SKILL_MAIN_MENU_START				=	WIID_SKILL	+	5000					,
		WIID_SKILL_MAIN_MENU_END				=	WIID_SKILL_MAIN_MENU_START	+	9999	,
		WIID_SKILL_SUB_MENU_START															,
		WIID_SKILL_SUB_MENU_END					=	WIID_SKILL_SUB_MENU_START	+	9999	,
		WIID_SKILL_CONTENT_START															,
		WIID_SKILL_CONTENT_END					=	WIID_SKILL_CONTENT_START	+	9999	,
	};

	enum SKILL_BUTTON
	{
		SKILL_BUTTON_ENABLE						=	0										,
		SKILL_BUTTON_DISABLE																,
		SKILL_BUTTON_HIT																	,	//	MouseOver
		SKILL_BUTTON_CLICK																	,
		SKILL_BUTTON_CHECK																	,	//	Select
		SKILL_BUTTON_COUNT																	,
	};

	struct UI_IMAGE_DATA
	{
		std::string				m_strImage;		//	이미지 경로 ( DATA/INTERFACE/CONCEPT/UI102.PNG )
		RECT					m_rcSrc;		//	이미지 소스 영역
	};

	struct UI_SUBID
	{
		int						m_iSubID;							//	서브 아이디
		int						m_iPosX;							//	출력 X 위치 ( 상대 좌표 )
		int						m_iPosY;							//	출력 Y 위치 ( 상대 좌표 )
	};

	typedef std::vector< UI_SUBID >									STD_VECTOR_UI_SUBID;

	struct UI_MAIN_MENU
	{
		int						m_iItemID;							//	아이템 아이디
		int						m_iClassType;						//	캐릭터 유형
		int						m_iDestXPos;						//	버튼 출력 X 위치
		int						m_iDestYPos;						//	버튼 출력 Y 위치
		int						m_iXSize;							//	버튼 X 사이즈
		int						m_iYSize;							//	버튼 Y 사이즈
		UI_IMAGE_DATA			m_stButton[ SKILL_BUTTON_COUNT ];	//	버튼 이미지 정보
		STD_VECTOR_UI_SUBID		m_vUISubID;							//	Sub UI
	};

	typedef	std::multimap< int , UI_MAIN_MENU >						STD_MULTIMAP_UI_MAIN_MENU;

	enum SKILL_CONTENT_VIEW
	{
		SCV_NULL							=	0				,	//	표시 없음
		SCV_TREE												,	//	Tree 형태로 표시
		SCV_LIST												,	//	스킬 배운 순서대로 표시
		SCV_CARD												,	//	스킬 모두 Diable로 표시 배운것은 Enable
		SCV_ITEM												,	//	아이템 스킬
		SCV_COUNT												,
	};

	struct UI_SUB_MENU
	{
		SKILL_CONTENT_VIEW		m_eMenuType;						//	컨텐츠 표시 방법
		int						m_iXSize;							//	버튼 X 사이즈
		int						m_iYSize;							//	버튼 Y 사이즈
		UI_IMAGE_DATA			m_stButton[ SKILL_BUTTON_COUNT ];	//	버튼 이미지 정보
	};

	typedef	std::map< int , UI_SUB_MENU >							STD_MAP_UI_SUB_MENU;

	struct UI_CONTENT
	{
		int						m_iSubID;
		int						m_iGridIndex;
		DWORD					m_dwImageSetting;
		int						m_iSkillID;
	};

	typedef	std::map< int , UI_CONTENT >							STD_MAP_UI_CONTENT;

	struct UI_CONTENT_BUNDLE
	{
		int						m_iEndPage;
		STD_MAP_UI_CONTENT		m_mUIContent;
	};

	typedef	std::map< int , UI_CONTENT_BUNDLE >						STD_MAP_UI_CONTENT_BUNDLE;

	struct UI_TREE
	{
		int						m_iItemID;							//	아이템 아이디
		int						m_iDestXPos;						//	출력 X 위치
		int						m_iDestYPos;						//	출력 Y 위치
		int						m_iXSize;							//	X 사이즈
		int						m_iYSize;							//	Y 사이즈
		UI_IMAGE_DATA			m_stImage;							//	이미지 정보
	};

	typedef std::vector< UI_TREE >									STD_VECTOR_UI_TREE;

	enum SKILL_CONTENT_LABEL
	{
		SCL_BG									=	0					,	//	스킬 슬롯 Index ( 0 : 일반 ) ( 1 : 스킬레벨0 ) ( 2 : 비어있을때 )
		SCL_CUR_LEVEL10													,	//	현재 레벨 십단위
		SCL_CUR_LEVEL1													,	//	현재 레벨 일단위
		SCL_LEVEL_SLASH													,	//	레벨 구분자 ( / )
		SCL_MAX_LEVEL10													,	//	최대 레벨 십단위
		SCL_MAX_LEVEL1													,	//	최대 레벨 일단위
		SCL_LEVEL_UP													,	//	레벨 업 버튼
		SCL_NOT_LEARN													,	//	스킬 안 배웠을때 ? / ?
		SCL_COUNT														,	//	이미지들
	};

	enum SKILL_CONTENT_GRID_VIEW											//	SKILL_TREE_SETTING.LDT 와 동일
	{
		SCGV_NULL								=	0					,
		SCGV_SLOT_NOT_LEARN						=	28					,	//	LDT No. 29	아이콘 슬롯 이미지 ? / ?
		SCGV_SLOT_GENERAL						=	29					,	//	LDT No. 30	아이콘 슬롯 이미지 일반
		SCGV_SLOT_LEVEL0						=	30					,	//	LDT No. 31	아이콘 슬롯 이미지 스킬레벨0
		SCGV_SLOT_EMPTY							=	31					,	//	LDT No. 32	아이콘 슬롯 이미지 비어있을때
		SCGV_COUNT														,

	};

	struct UI_CONTENT_WINDOW
	{
		SPWindow*				m_pContent[ SCL_COUNT ];
		SPWindow*				m_pImages[ SCGV_COUNT ];					//	라인 이미지들
	};

	typedef std::vector< UI_CONTENT_WINDOW >						STD_VECTOR_UI_CONTENT_WINDOW;
	typedef	std::map< int , int >									STD_MAP_UI_CONTENT_PAGE;

	struct SKILL_LEARN_REQUIRE
	{
		int						m_iRequireType;								//	제한 타입
		int						m_iRequireID;								//	제한 아이디
		int						m_iRequireValue1;							//	제한 값
		int						m_iRequireValue2;							//	제한 값
		std::string				m_strErrorMsg;								//	에러 메세지 제한
	};

	struct SKILL_LEARN_LDT_DATA 
	{
		int						m_iKeyID;									//	스킬 아이디
		std::string				m_strName;									//	이름
		UINT64					m_uiConsumptionEly;							//	소비 엘리
		int						m_iConsumptionItemID;						//	소비 아이템
//		int						m_iConsumptionItemCount;					//	소비 아이템 갯수
		SKILL_LEARN_REQUIRE		m_stRequireCondition[ LS_REQUIRE_COUNT ];	//	제한 조건
		std::string				m_strDesc;									//	설명
		std::string				m_strErrorEly;								//	에러 메세지 엘리
		std::string				m_strErrorItem;								//	에러 메세지 아이템
		int						m_iFXGroupID;								//	획득시 출력해줄 FXGroupID
		std::string				m_strSuccess;								//	획득시 출력해줄 채팅 문자열
	};

	typedef	std::map< int , SKILL_LEARN_LDT_DATA >						STD_MAP_LEARN_LDT_DATA;


	//////////////////////////////////////////////////////////////////////////
	//
	//	함수들
	//

	void						LoadLDTMainMenu			();
	void						LoadLDTSubMenu			();
	void						LoadLDTContent			();
	void						LoadLDTTreeSetting		( STD_VECTOR_UI_TREE& vTreeSetting );
	void						LoadLDTLearnSkill		();

	void						CreateWindowMainMenu	();
	void						CreateWindowSubMenu		();
	void						CreateWindowContent		();

	void						UpdateAll				();
	void						UpdateMainMenu			( int iClassType );
	void						UpdateSubMenu			( UI_MAIN_MENU* pUIMainMenu );
	void						UpdateContent			( int iCurSubMenu , int iCurContentPage );
	void						UpdateContentTree		( int iCurSubMenu , int iCurContentPage );
	void						UpdateContentList		( int iCurSubMenu , int iCurContentPage );
	void						UpdateContentCard		( int iCurSubMenu , int iCurContentPage );
	void						UpdateContentItem		( int iCurSubMenu , int iCurContentPage );

	//////////////////////////////////////////////////////////////////////////
	//
	//	MainMenu	Interface
	//

	void						AddMainMenu				( int iKey , UI_MAIN_MENU& stMainMenuData );

	int							GetMainMenuCount		();
	int							GetMainMenuCount		( int iKey );
	int							GetMainMenuSubCount		( int iKey , int iNumber );
	UI_MAIN_MENU*				GetMainMenu				( int iKey , int iNumber );

	//////////////////////////////////////////////////////////////////////////
	//
	//	SubMenu	Interface
	//

	void						AddSubMenu				( int iKey , UI_SUB_MENU& stSubMenuData );

	int							GetSubMenuCount			();
	UI_SUB_MENU*				GetSubMenu				( int iKey );

	//////////////////////////////////////////////////////////////////////////
	//
	//	SubMenu Page Interface
	//

	void						SetSubMenuPage			( int iKey , int iPage );
	int*						GetSubMenuPage			( int iKey );
	void						DelSubMenuPage			();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Content	Interface
	//

	int							GetContentKey			( int iSubID , int iGridIndex );
	int							GetContentKeyToSub		( int iKey );

	void						AddContent				( int iKey , UI_CONTENT& stContent );

	int							GetContentBundleCount	();
	int							GetContentEndPage		( int iSubID );
	int							GetContentCount			( int iSubID );

	UI_CONTENT_BUNDLE*			GetContentBundle		( int iSubID );
	UI_CONTENT*					GetContent				( int iKey );

	//////////////////////////////////////////////////////////////////////////
	//
	//	Content Page Interface
	//

	int							GetContentPageKey		( int iMainID , int iSubID );
	void						SetContentPage			( int iKey , int iPage );
	int*						GetContentPage			( int iKey );
	void						DelContentPage			();

	//////////////////////////////////////////////////////////////////////////
	
	void						DataReset				();
	void						ReposOwnImage			();

	void						HideContentWindow		( int iPos );
	void						SetContentWindow		( int iPos , int iSkillID , bool bShow , bool bLearnShow );
	void						SetContentImageWindow	( int iPos , DWORD dwImageSetting );
	void						SetSkillPoints			();
	void						SetSkillUIInfo			( int iID , int iUIUnitPos , int iCoolTimeID , bool bQuickSlotUse , int iDisableIcon , int iDisableIconIndex , int iDisableAlpha , int iItemID , int iContainerType , int iSlotIndex );

	//////////////////////////////////////////////////////////////////////////

	void						AddLearnSkillLdt		( SKILL_LEARN_LDT_DATA& stSkillLearnLDtData );
	void						ClearLearnSkillLdt		( SKILL_LEARN_LDT_DATA& stSkillLearnLDtData );
	SKILL_LEARN_LDT_DATA*		GetLearnSKillLdt		( int iSkillID );
	void						SetLearnSkillMsg		( int iSkillID );
	void						SetLearnSkillRequire	( std::string& strMessage , int iType , int iID , int iValue1 , int iValueSlv1 , int iValue2 , int iValueSlv2 , int iLevel , int iGSNumber );



	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	STD_MULTIMAP_UI_MAIN_MENU		m_mmUIMainMenu;
	STD_MAP_UI_SUB_MENU				m_mUISubMenu;
	STD_MAP_UI_CONTENT_BUNDLE		m_mUIContent;

	SPTexture*						m_pBGTexture;
	RECT							m_rcBGDest[ SKILL_BG_COUNT ];
	RECT							m_rcBGSrc[ SKILL_BG_COUNT ];

	SPWindowButton*					m_pMainMenuButton[ SKILL_MAIN_MENU_MAX ];
	SPWindowButton*					m_pSubMenuButton[ SKILL_SUB_MENU_MAX ];

	int								m_iCurClassType;
	UI_MAIN_MENU*					m_pCurMainMenu;
	int								m_iCurSubMenu;
	STD_MAP_UI_CONTENT_PAGE			m_mCurSubMenuPage;
	STD_MAP_UI_CONTENT_PAGE			m_mCurContentPage;

	SPUIUnitManager					m_UIUnitManager;
	STD_VECTOR_UI_CONTENT_WINDOW	m_vUIContentWindow;

	BOOL							m_bLMouseDown;
	LPARAM							m_lParam;

	STD_MAP_LEARN_LDT_DATA			m_mLearnSkill;
	int								m_iLearnSkillID;




};
