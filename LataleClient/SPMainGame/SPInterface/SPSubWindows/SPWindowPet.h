#pragma once


#define WIID_PET_TOP				(WIID_PET	+	1)		//스킨 TOP
#define WIID_PET_CENTER				(WIID_PET	+	2)		//스킨 CENTER
#define WIID_PET_BOTTOM				(WIID_PET	+	3)		//스킨 BOTTOM

#define WIID_PET_TITLE_ICON			(WIID_PET	+	10)
#define WIID_PET_TITLE				(WIID_PET	+	11)
#define WIID_PET_CLOSE				(WIID_PET	+	12)

#define WIID_PET_SKIN_1				(WIID_PET	+	20)		//녹색 바닥
#define WIID_PET_LINE_1				(WIID_PET	+	21)
#define WIID_PET_LINE_2				(WIID_PET	+	22)
#define WIID_PET_NAME				(WIID_PET	+	23)
#define WIID_PET_DESC				(WIID_PET	+	24)

#define WIID_PET_ICONLINE			(WIID_PET	+	30)
#define WIID_PET_GRID				(WIID_PET	+	31)

#define WIID_PET_SKIN_2				(WIID_PET	+	100)	//녹색 바닥
#define WIID_PET_SPXP				(WIID_PET	+	101)
#define WIID_PET_SP					(WIID_PET	+	102)
#define WIID_PET_SPINFO				(WIID_PET	+	103)
#define WIID_PET_XP					(WIID_PET	+	104)
#define WIID_PET_XPINFO				(WIID_PET	+	105)
#define WIID_PET_ENERGY				(WIID_PET	+	106)
#define WIID_PET_ENERGYINFO			(WIID_PET	+	107)

#define WIID_PET_SKILLINFO			(WIID_PET	+	110)

#define WIID_PET_SKILLLINE1			(WIID_PET	+	131)
#define WIID_PET_SKILLLINE2			(WIID_PET	+	132)

#define WIID_PET_ABILITYINFO		(WIID_PET	+	140)	//ability
#define WIID_PET_ABILITYLINE		(WIID_PET	+	142)
#define WIID_PET_ABILITYLINE2		(WIID_PET	+	143)

#define WIID_PET_SKILL_1			(WIID_PET	+	121)	// 스킬 작업용으로 연속으로 위치 요망 [11/27/2006 AJJIYA]
#define WIID_PET_SKILL_2			(WIID_PET	+	122)	// 스킬 작업용으로 연속으로 위치 요망 [11/27/2006 AJJIYA]
#define WIID_PET_ABILITY			(WIID_PET	+	123)	// 스킬 작업용으로 연속으로 위치 요망 [11/27/2006 AJJIYA]
#define WIID_PET_ABILITY2			(WIID_PET	+	124)

#define WIID_PET_TALK_TOP			(WIID_PET	+	150)	//말풍선
#define WIID_PET_TALK_CENTER		(WIID_PET	+	151)
#define WIID_PET_TALK_BOTTOM		(WIID_PET	+	152)
#define WIID_PET_TALK				(WIID_PET	+	153)

#define WIID_PET_EAT				(WIID_PET	+	160)	//팻 먹이주기용 영역
#define WIID_PET_IMAGE_LINE			(WIID_PET	+	161)	//팻 시퀀스 출력 백그라운드 이미지

#define WIID_PET_FEED				(WIID_PET	+	200)	//아이템 드래그해 먹이를 주는 영역
#define WIID_PET_SOULREMOVE			(WIID_PET	+	220)	//영혼이전

#define WIID_PET_FEED_ITEM			(WIID_PET	+	230)	//
#define WIID_PET_FEED_ITEM_BUTTON	(WIID_PET	+	231)	//
#define WIID_PET_FEED_ITEM_SKIN		(WIID_PET	+	232)	//
#define WIID_PET_FEED_ITEM_ICON		(WIID_PET	+	233)	//
#define WIID_PET_FEED_ITEM_BUTTON_2	(WIID_PET	+	234)	//
#define WIID_PET_FEED_ITEM_SKIN_2	(WIID_PET	+	235)	//
#define WIID_PET_FEED_ITEM_ICON_2	(WIID_PET	+	236)	//

#define WIID_PET_PRIMARY			(WIID_PET	+	240)
#define WIID_PET_SECONDARY			(WIID_PET	+	241)
#define WIID_PET_TERTIARY			(WIID_PET	+	242)

#define WIID_PET_NAME_EDITOR		(WIID_PET	+	250)
#define WIID_PET_NAME_BTN			(WIID_PET	+	251)

enum PET_SKILL_POS
{
	PET_SKILL_ONE			=	0	,
	PET_SKILL_TWO					,
	PET_SPECIAL_SKILL_ONE			,
	PET_SPECIAL_SKILL_TWO			,
};


enum PET_STAT_UPDATE {
	PET_STAT_UPDATE_NULL	= 0,
	PET_STAT_UPDATE_SP		= 1,
	PET_STAT_UPDATE_EXP		= 2,
	PET_STAT_UPDATE_ENERGY	= 3,
};


#include "SPUIUnitManager.h"


const int MAX_PET_TALK		= 3;							// 랜덤용 펫 대화 리스트

struct SPPetTalk {
	int			m_iPetID;
	std::string m_strName;
	std::string m_strSP;
	std::string m_strEXP;
	int			m_iTalkIndex;
	std::string m_strTalk[MAX_PET_TALK];

	SPPetTalk() {
		Clear();
	}

	~SPPetTalk() {
		Clear();
	}

	void Clear() {
		m_iPetID = 0;
		m_strName.clear();
		m_strSP.clear();
		m_strEXP.clear();
		m_iTalkIndex = 0;
		for(int i = 0; i < MAX_PET_TALK; i++) {
			m_strTalk[i].clear();
		}
	}
};


class SPWindow;
class SPWindowStatic;
class SPWindowButton;
class SPWindowGauge;
class SPWindowGrid;
class SPUIUnitManager;
class SPWindowPetShop;

class SPItem;
class SPPet;
enum PET_PLACE;

const int PET_SP_REPLETION		= 90;						//포만감 체크
const int PET_SP_HUNGRY			= 50;						//배고픔 체크
const int PET_ENERGY_HUNGRY		= 500;

struct PetInfo
{
	PetInfo( void )
	{
		reset();
	}
	void reset( void )
	{
		m_pPetItem = NULL;
		m_pPetFeed = NULL;
		m_pPetFeedEnergy = NULL;
		m_iPetClassID = 0;
		item.SlotInfo.ContainerType = CONTAINER_TYPE_INVALID;
		item.SlotInfo.SlotIndex = 0;
		item.Item.iItemID = 0;
	}
	SPItem	*m_pPetItem;
	SPItem	*m_pPetFeed;
	SPItem	*m_pPetFeedEnergy;
	int		m_iPetClassID;
	CONTAINER_ITEM	item;
};

class SPWindowPet : public SPWindow 
{
public:	
	SPWindowPet(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowPet();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

	virtual void RefreshRelationCoord();

protected:	
	void	InitSubControl();
	bool	LoadPetTalkLDT();
	void	ClearAllPetTalk();
	SPPetTalk* FindPetTalk(int iID);
	void	UpdatePetTalk();
	void	ClearPetFeed();						/**< 등록된 펫 자동 먹이를 정리한다 <br> */
	void	ClearPetFeedEnergy( void );
	bool	UpdatePetFeed();					/**< 등록된 펫 자동 먹이정보를 갱신한다 <br> */
	bool 	CheckPetFeed(PET_PLACE iPlace);						/**< 펫이 자동으로 먹이를 먹을수 있는지 확인한다 <br> */
	void	AutoPetEat(PET_PLACE iPlace);						/**< 자동 먹기 <br> */
	bool	CheckPetFeedEnergy(PET_PLACE iPlace);
	void	AutoPetEatEnergy(PET_PLACE iPlace);
	
	void	InitSkillUI();
	void	UpdatePosSkillUI();
	void	UpdateSkillUI();
	void	UpdateSkillUIContents( std::vector< int >& vSkillList , int iItemID , CONTAINER_SLOT stContainerSlot , int iStartPos );

	void SetPet(unsigned int uiClassID);
	void ReleasePet();
	
	/**
	 *	@brief reload pet's info from m_iCurPetSelect
	 */
	void ReloadPetInfo( void );

	/// load cash pet food configuration
	bool LoadCashFoodCfg( void );
	bool IsItemCashFood( int itemID );
	void reloadTabImg(void);
	

protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()	
	int OnStatUpdate	(WPARAM wParam, LPARAM lParam);
	int OnFeedUpdate	(WPARAM wParam, LPARAM lParam);					///< 외부에서 등록된 자동 먹이 정보를 갱신
	int OnEat			(WPARAM wParam, LPARAM lParam);					///< 마우스 드래그로 먹이는 경우
	int OnPetShop		(WPARAM wParam, LPARAM lParam);
	int OnNewPetShop	(WPARAM wParam, LPARAM lParam);
	int OnItemSetting	(WPARAM wParam, LPARAM lParam);
	int OnItemClear		(WPARAM wParam, LPARAM lParam);
	int OnItemAllClear	(WPARAM wParam, LPARAM lParam);
	int OnItemSetStack	(WPARAM wParam, LPARAM lParam);
	int OnItemSetIconSelect(WPARAM wParam, LPARAM lParam);
	int OnItemDisable	(WPARAM wParam, LPARAM lParam);
	int OnClose 		(WPARAM wParam, LPARAM lParam);	
	int OnCursorMove	(WPARAM wParam, LPARAM lParam);
	int OnGridDown		(WPARAM wParam, LPARAM lParam);
	int OnGridUp		(WPARAM wParam, LPARAM lParam);

	int OnSkill1			(WPARAM wParam, LPARAM lParam);
	int OnSkill2			(WPARAM wParam, LPARAM lParam);
	int OnAbility			(WPARAM wParam, LPARAM lParam);
	int OnAbility2			(WPARAM wParam, LPARAM lParam);
	int OnFeed				(WPARAM wParam, LPARAM lParam);				///< 자동 먹이 먹기용 아이템 등록
	int OnFeedEnergy		(WPARAM wParam, LPARAM lParam);				///< 자동 먹이 먹기용 아이템 등록

	int	OnDragnDropEnd		(WPARAM wParam, LPARAM lParam);
	int OnRefresh			(WPARAM wParam, LPARAM lParam);
	int OnSkillLButtonUp	( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int OnSkillLButtonDown	( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int OnSkillCursorMove	( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int OnSkillCursorEnter	( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int OnSKillCursorOut	( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int OnPetSelectClick	( unsigned int iID, WPARAM wParam , LPARAM lParam );

	virtual int	OnBeginMove		( WPARAM, LPARAM );

	virtual int OnToolTipEnter	( WPARAM, LPARAM );
	virtual int OnToolTipOut	( WPARAM, LPARAM );
	virtual int OnToolTipMove	( WPARAM, LPARAM );

	int		onEditSetFocus( WPARAM wParam, LPARAM lParam );
	int		onEditKillFocus( WPARAM wParam, LPARAM lParam );
	int		onEditNameClick(WPARAM wParam, LPARAM lParam );
	int		onPetInfoUpdate( WPARAM wParam, LPARAM lParam );

private:
	SPTexture* m_pDisable;		
	RECT	m_rtSrcDisable;						/**< 비활성 표현용 소스 <br> */
	
	//SPItem*			m_pPetItem;	
	//SPItem*			m_pPetFeed;					/**< 자동 먹이주기용 아이템 <br> */	

	PetInfo			m_petInfo[3];
	
	SPWindowGrid*	m_pGrid;
	SPWindow*		m_pName;	

	SPWindowGauge*	m_pXP;
	SPWindowGauge*	m_pSP;
	SPWindowGauge*	m_pEnergy;
    SPWindow*		m_pXPInfo;
	SPWindow*		m_pSPInfo;
	SPWindow*		m_pEnergyInfo;
	SPWindow*		m_pPetMsg;
	SPWindowPetShop* m_pPetShop;
	SPWindow*		m_pPetFeedIcon;				/**< 자동 먹이주기 등록 Icon <br> */
	SPWindow*		m_pPetFeedIcon2;			//!< icon for energy food

	char			m_szXPInfo[16];
	char			m_szSPInfo[16];
	char			m_szEnergyInfo[16];

	SPUIUnitManager					m_UIUnitManager;

	// 스킬 드래그를 위한 변수 [11/27/2006 AJJIYA]
	BOOL							m_bLMouseDown;
	LPARAM							m_lParam;
	
	std::vector<SPPetTalk*>			m_vpPetTalk;
	SPPetTalk*						m_pCurPetTalk;

	RECT	m_rtMarkSrc[4];						/**< ICON MARK Src <br> */	
	RECT	m_rtStack;							/**< Item Stack Count <br> */
	bool	m_bItemAble;						/**< 내가 올려논 아이템이 내가 착용가능한 아이템인지 <br> */
	int		m_iStack;							/**< 받은 Item의 갯수 <br> */
	ICON_MARK	m_iItemMark;					/**< ICON MARK <br> */
	RECT	m_rtStack_energy;
	bool	m_bItemAble_energy;
	int		m_iStack_energy;
	ICON_MARK	m_iItemMark_energy;

	int m_iModelPosX;
	int m_iModelPosY;
	SPPet* m_pkPet;

	PET_PLACE	m_iCurPetSelect;
	SPWindowButton	*m_pBtnPrimary;
	SPWindowButton	*m_pBtnSecondary;
	SPWindowButton	*m_pBtnTertiary;
	SPWindowButton	*m_pBtnNameEditor;
	SPWindowEdit	*m_pNameEditor;
	std::vector< int >	m_vecCashFood;

};
