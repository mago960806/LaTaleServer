// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 오전 10:26:19
//***************************************************************************

#pragma once

#define WIID_TOOLTIP_TITLE					(WIID_TOOLTIP	+	100 )
#define WIID_TOOLTIP_CONTENTS				(WIID_TOOLTIP	+	200 )
#define WIID_TOOLTIP_DESC					(WIID_TOOLTIP	+	300 )
#define WIID_TOOLTIP_TITLE2					(WIID_TOOLTIP	+	400 )
#define WIID_TOOLTIP_CONTENTS2				(WIID_TOOLTIP	+	500 )
#define WIID_TOOLTIP_ARROW					(WIID_TOOLTIP	+	600 )

#define TOOLTIP_BG_COUNT					9

#define MAX_TOOLTIP_CONTENTS_LINE			25

struct TOOLTIP_DATA
{
	std::string				m_strName;
	std::string				m_strDesc;
	bool					m_bCutInCheck;
};

typedef std::map< INSTANCE_ID , TOOLTIP_DATA >	STD_MAP_TOOLTIP;

struct TOOLTIP_NORMAL_DATA
{
	int						m_iContentsCount;		//	실제 사용할 라인수 세팅 MAX_TOOLTIP_CONTENTS_LINE보다 작아야 한다.

	std::string				m_strName;
	std::string				m_strContents[ MAX_TOOLTIP_CONTENTS_LINE ];
	std::string				m_strDesc;

	D3DXCOLOR				m_cName;
	D3DXCOLOR				m_cContents[ MAX_TOOLTIP_CONTENTS_LINE ];
	D3DXCOLOR				m_cDesc;
};

class SPUIUnit;
class SPItem;
 
class SPWindow;
class SPWindowStatic;

class SPWindowToolTip : public SPWindow {

public:

	enum TOOLTIP_SHOW_TYPE
	{
		TOOLTIP_SHOW_TYPE_NULL				=	0	,
		TOOLTIP_SHOW_TYPE_ITEM				=	-1	,
		TOOLTIP_SHOW_TYPE_UIUNIT			=	-2	,
		TOOLTIP_SHOW_TYPE_CTRL				=	-3	,
		TOOLTIP_SHOW_TYPE_TITLE				=	-4	,
		TOOLTIP_SHOW_TYPE_CASHITEM			=	-5	,
		TOOLTIP_SHOW_TYPE_ENCHANT			=	-6	,
		TOOLTIP_SHOW_TYPE_ITEMUPGRADE		=	-7	,
		TOOLTIP_SHOW_TYPE_SKILL_LVUP		=	-8	,
		TOOLTIP_SHOW_TYPE_FRIEND_WHITE		=	-9	,
		TOOLTIP_SHOW_TYPE_LOTTERY			=	-10	,
		TOOLTIP_SHOW_TYPE_ITEM_SET			=	-11	,
		TOOLTIP_SHOW_TYPE_NPC_EVENT			=	-12	,
		TOOLTIP_SHOW_TYPE_NORMAL			=	-13 ,
		TOOLTIP_SHOW_TYPE_ITEMMIX			=	-14 ,
		TOOLTIP_SHOW_TYPE_PUSHITEM			=	-15 ,	// push item consume
	};


	SPWindowToolTip(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowToolTip();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);

	virtual int  PerformMouseMove( int iX, int iY) {	return 0;	}

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnToolTipEnter	( WPARAM, LPARAM );
	virtual int OnToolTipOut	( WPARAM, LPARAM );
	virtual int OnToolTipMove	( WPARAM, LPARAM );
	virtual int OnRefresh		( WPARAM, LPARAM );
	virtual int OnPosUpdate		( WPARAM, LPARAM );
	 // }}

protected:

	enum TOOLTIP_CONTENT_TYPE
	{
		TCT_STRING					=	0	,
		TCT_SPACE							,
		TCT_LINE							,
	};

	void	OnToolTipShow			( WPARAM wParam , LPARAM lParam );
	void	UpdatePos				();

	void	CreateCtrl				( INSTANCE_ID InstanceID );
	void	CreateItemData			( SPItemStatus* pItemStatus );
	void	CreateTitle				( UINT32 uiTitleID );
	void	CreateCashItem			( SPCashItemAttr* pCashItemAttr );
	void	CreateEnchant			( SPItemEnchantResult* pEnchantResult );
	void	CreateItemUpgrade		( SPItemStatus* pItemUpgradeStatus );
	void	CreateSkillLVUP			( SPUIUnit*	pUIUnit );
	void	CreateFriendWhite		( FRIEND_INFO* pFriendInfo );
	void	CreateLottery			( WPARAM wParam );
	void	CreateItemDataSet		( int iSetItemID , D3DXCOLOR cItemColor );
	void	CreateItemDataSet		( SPSetEffect* pSetEffect , int iSetNumber , bool bEnable , D3DXCOLOR cEnableColor , D3DXCOLOR cDisableColor , const char* pszLeftString );
	void	CreateNpcEvent			( WPARAM wParam );
	void	CreateNormal			( TOOLTIP_NORMAL_DATA* pToolTipNomal );
	void	CreateItemMix			( SPItemMix* pItemMix );
	void	CreatePushItem			( SPCashItemAttr* pCashItemAttr );

	void	ShowToolTip				( SPUIUnit*	pUIUnit );
	int		CreateSkillData			( SPUIUnit* pUIUnit );						// AJJIYA [7/8/2005]
	void	CreateSkillDataAction	( SPUIUnit* pUIUnit );						// AJJIYA [7/13/2005]
	void	CreateSkillDataEtc		( SPUIUnit* pUIUnit );						// AJJIYA [7/13/2005]
	void	CreateQuestData			( SPUIUnit* pUIUnit );						// AJJIYA [10/6/2005]
	void	CreateGOBData			( SPUIUnit* pUIUnit );						// AJJIYA [10/6/2005]
	void	CreateBuffData			( SPUIUnit* pUIUnit );						// AJJIYA [10/11/2005]

	void	SetWindowRequire		( int iType , int iID , int iValue1 , int iValueSlv1 , int iValue2 , int iValueSlv2 , int iLevel , int iGSNumber , D3DXCOLOR cDefaltColor , D3DXCOLOR cDisableColor );

	void	ClearContents			();

	void	MakeContentsOne			( const char* pstrString1 , const char* pstrString2 , D3DXCOLOR cColor );
	void	SetContents				( TOOLTIP_CONTENT_TYPE eContentType , const char* pstrString = NULL , D3DXCOLOR cColor = D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , FONT_ENUMERATE eFont = FONT_12_NORMAL );
	void	SetContents2			( TOOLTIP_CONTENT_TYPE eContentType , const char* pstrString = NULL , D3DXCOLOR cColor = D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) , FONT_ENUMERATE eFont = FONT_12_NORMAL );
	void	SetDesc					( const char* pstrString , D3DXCOLOR cColor = D3DCOLOR_ARGB( 255 , 0 , 0 , 0 ) );

	void	CalAutoWidth			();
	void	CalDescPos				();
	void	CalBGSize				();

	void	SetBGRect				( RECT* prcOutput , int iStartX , int iStartY , int iEndX , int iEndY , int iStep );

	void	LoadToolTipLDT			();

protected:

	enum GLOBAL_STRING_TOOLTIP
	{
		GS_TOOLTIP								=	0			,

		GS_TOOLTIP_COMMON_REQUIRE				=	1220000		,
		GS_TOOLTIP_COMMON_CLASS					=	6000019		,
		GS_TOOLTIP_COMMON_GENDER				=	1200000		,
		GS_TOOLTIP_COMMON_UP_CHAR				=	1200010		,
		GS_TOOLTIP_COMMON_DOWN_CHAR				=	1200011		,
		GS_TOOLTIP_COMMON_RANGE_CHAR			=	1200012		,
		GS_TOOLTIP_COMMON_FIRST_CONTENT_1		=	1900001		,
		GS_TOOLTIP_COMMON_FIRST_CONTENT_2		=	1900002		,


		GS_TOOLTIP_ITEM_TYPE					=	1000000		,
		GS_TOOLTIP_ITEM_SUBTYPE					=	1206000		,
		//GS_TOOLTIP_ITEM_REQUIRE_TITLE			=	1200022		,
		//GS_TOOLTIP_ITEM_EFFECT_TITLE			=	1200023		,
		//GS_TOOLTIP_ITEM_OPTION_TITLE			=	1200024		,
		GS_TOOLTIP_ITEM_EFFECT					=	1020000		,
		GS_TOOLTIP_ITEM_OPTION_POINT			=	1030001		,
		GS_TOOLTIP_ITEM_OPTION_RARE				=	1030002		,
		GS_TOOLTIP_ITEM_DURATION_AMOUNT_DAY		=	1040001		,
		GS_TOOLTIP_ITEM_DURATION_AMOUNT_HOUR	=	1040002		,
		GS_TOOLTIP_ITEM_DURATION_AMOUNT_ETC		=	1040003		,
		GS_TOOLTIP_ITEM_DURATION_QUANTITY_DAY	=	1040011		,
		GS_TOOLTIP_ITEM_DURATION_QUANTITY_HOUR	=	1040012		,
		GS_TOOLTIP_ITEM_DURATION_QUANTITY_ETC	=	1040013		,
		GS_TOOLTIP_ITEM_COOLTIME_DAY			=	1041001		,
		GS_TOOLTIP_ITEM_COOLTIME_HOUR			=	1041002		,
		GS_TOOLTIP_ITEM_COOLTIME_ETC			=	1041003		,
		GS_TOOLTIP_ITEM_EXPIRE					=	1600011		,
		GS_TOOLTIP_ITEM_SELL_PRICE				=	1050001		,

		GS_TOOLTIP_ITEM_UNIT_OZ					=	9001020		,
		GS_TOOLTIP_ITEM_UNIT_PT					=	9001021		,

		GS_TOOLTIP_CASHITEM_LINEUP				=	9006001		,
		GS_TOOLTIP_CASHITEM_COUNT				=	9006002 	,


		//GS_TOOLTIP_SKILL_NAME_TITLE				=	20000		,
		//GS_TOOLTIP_SKILL_ACTION_TITLE			=	1200022		,
		GS_TOOLTIP_SKILL_INVEN_TYPE				=	1203000		,
		GS_TOOLTIP_SKILL_REQUIRE_ITEM_KIND		=	1206000		,
		GS_TOOLTIP_SKILL_POINT_TITLE			=	1209000		,
		//GS_TOOLTIP_SKILL_LEVEL_UP_TITLE			=	1200021		,
		GS_TOOLTIP_SKILL_UP_REQUIRE				=	1210000		,
		//GS_TOOLTIP_SKILL_REQUIRE_TITLE			=	1200022		,
		GS_TOOLTIP_SKILL_ADD_LEVEL_0			=	1230001		,
		GS_TOOLTIP_SKILL_ADD_LEVEL_0_NOT		=	1230002		,

		GS_TOOLTIP_QUEST_CONDITION				=	6000006		,
		GS_TOOLTIP_QUEST_ING					=	6000013		,
		GS_TOOLTIP_QUEST_COMPLETE								,
		GS_TOOLTIP_QUEST_CLASSTYPE								,
		//GS_TOOLTIP_QUEST_LEVEL									,
		//GS_TOOLTIP_QUEST_FAME									,
		//GS_TOOLTIP_QUEST_CONDITION_COMPLETE						,
		GS_TOOLTIP_BUFF_REMAINTIME				=	1300001		,
		GS_TOOLTIP_BUFF_DAY										,
		GS_TOOLTIP_BUFF_HOUR									,
		GS_TOOLTIP_BUFF_MIN										,
		GS_TOOLTIP_BUFF_SEC										,

		GS_TOOLTIP_TITLE_TYPE					=	1400001		,
		//GS_TOOLTIP_TITLE_EFFECT									,

		GS_TOOLTIP_ENCHANT_OPTION				=	1200025		,
		GS_TOOLTIP_ENCHANT_CONDITION			=	1200026		,

		GS_TOOLTIP_ITEMUPGRADE_REQUIRE			=	1200027		,

		GS_TOOLTIP_FRIEND_WHITE_LV				=	20000		,
		GS_TOOLTIP_FRIEND_WHITE_CHANNEL			=	5001001		,
		GS_TOOLTIP_FRIEND_WHITE_INDUN			=	5006007		,
		GS_TOOLTIP_FRIEND_WHITE_PVP				=	5006008		,

		GS_TOOLTIP_ITEM_SET_OPTION_LEFT_STRING	=	1700002		,
		GS_TOOLTIP_ITEM_SET_FULL_TITLE			=	1700001		,
	};

	enum TOOLTIP_CONTENTS_COLOR
	{
		TCC_DISABLE								=		0		,
		TCC_BLACK												,
		TCC_WHITE												,
		TCC_RED													,
		TCC_BLUE												,

		TCC_REQUIRE												,

		TCC_ITEM_OPT											,
		TCC_ITEM_OPTION_LIMIT									,
		TCC_ITEM_EFFECT											,
		TCC_ITEM_OPTION											,
		TCC_ITEM_OPTION_ENCHANT									,
		TCC_ITEM_TIME											,
		TCC_ITEM_COOLTIME										,

		TCC_ITEM_UPGRADE_REQUIRE								,
		TCC_ITEM_UPGRADE_EFFECT									,

		TCC_ITEM_ENCHANT_REQUIRE								,
		TCC_ITEM_ENCHANT_OPTION									,

		TCC_SKILL_NOT_LEARN_TITLE								,
		TCC_SKILL_NOT_LEARN_CONTENTS							,

		TCC_SKILL_LEARN_REQUIRE									,

		TCC_ITEM_SET_OPTION_DISABLE								,
		TCC_ITEM_SET_OPTION_ENABLE								,
		TCC_ITEM_SET_FULL_OPTION_DISABLE						,
		TCC_ITEM_SET_FULL_OPTION_ENABLE							,

		TCC_ITEM_MIX_OPT										,

		TCC_COUNT												,

	};

	std::string			m_strName;
	SPItem*				m_pItem;			//	아이템 정보를 가져오기 위해 임시로 쓰는 아이템
	TOOLTIP_SHOW_TYPE	m_eType;

	//////////////////////////////////////////////////////////////////////////

	SPTexture*			m_pBGTexture;

	RECT				m_rcBGDest[ TOOLTIP_BG_COUNT ];
	RECT				m_rcBGSrc[ TOOLTIP_BG_COUNT ];
	int					m_iStep;

	RECT				m_rcMargin;

	SPWindowStatic*		m_pTitle;
	SPWindowStatic*		m_pContents[ MAX_TOOLTIP_CONTENTS_LINE ];
	SPWindowStatic*		m_pDesc;

	D3DXCOLOR			m_cColors[ TCC_COUNT ];

	int					m_iWriteContentPos;

	STD_MAP_TOOLTIP		m_mToolTip;
	INSTANCE_ID			m_eOldToolTipID;
	int					m_iOldToolTipID;

	bool				m_bShowToolTipID;

	//////////////////////////////////////////////////////////////////////////

	SPTexture*			m_pBGTexture2;

	RECT				m_rcBGDest2[ TOOLTIP_BG_COUNT ];
	RECT				m_rcBGSrc2[ TOOLTIP_BG_COUNT ];
	int					m_iStep2;

	int					m_iSX2;
	int					m_iSY2;
	RECT				m_rcMargin2;

	SPWindowStatic*		m_pTitle2;
	SPWindowStatic*		m_pContents2[ MAX_TOOLTIP_CONTENTS_LINE ];

	int					m_iWriteContentPos2;

	//////////////////////////////////////////////////////////////////////////

	std::string			m_strDesc;

};
