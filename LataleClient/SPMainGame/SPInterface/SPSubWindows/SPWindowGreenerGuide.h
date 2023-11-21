#pragma once
#include <vector>
//--------------------------------------------------
#ifndef MAX_STRING_SIZE
#define  MAX_STRING_SIZE (128)
#endif
//--------------------------------------------------
#define GG_MONSTER_COUNT (4)
#define GG_QUEST_COUNT (2)
#define GG_GIFT_COUNT (4)
//--------------------------------------------------
enum WIID_GREENERGUIDE_COMPONENTS
{
	WIID_GREENERGUIDE______ = WIID_GREENERGUIDE,
	WIID_GREENERGUIDE_FETCHAWARD,
	WIID_GREENERGUIDE_OK,
	WIID_GREENERGUIDE_NOTIFICATION,
	WIID_GREENERGUIDE_CLOSE,
	WIID_GREENERGUIDE_TXFINFO,
	WIID_GREENERGUIDE_TXTMAP_BEGIN,
	WIID_GREENERGUIDE_TXTMAP_END = WIID_GREENERGUIDE_TXTMAP_BEGIN + GG_MONSTER_COUNT,
	WIID_GREENERGUIDE_TXTMONSTER_BEGIN,
	WIID_GREENERGUIDE_TXTMONSTER_END = WIID_GREENERGUIDE_TXTMONSTER_BEGIN + GG_MONSTER_COUNT,
	WIID_GREENERGUIDE_TXTLEVEL_BEGIN,
	WIID_GREENERGUIDE_TXTLEVEL_END = WIID_GREENERGUIDE_TXTLEVEL_BEGIN + GG_MONSTER_COUNT,
	WIID_GREENERGUIDE_TXTQUEST_BEGIN,
	WIID_GREENERGUIDE_TXTQUEST_END = WIID_GREENERGUIDE_TXTQUEST_BEGIN + GG_QUEST_COUNT,
	WIID_GREENERGUIDE_TXTGIFT,
	WIID_GREENERGUIDE_IMGGIFT_BEGIN,
	WIID_GREENERGUIDE_IMGGIFT_END = WIID_GREENERGUIDE_IMGGIFT_BEGIN + GG_GIFT_COUNT,
	WIID_GREENERGUIDE_IMGGIFTBG_BEGIN,
	WIID_GREENERGUIDE_IMGGIFTBG_END = WIID_GREENERGUIDE_IMGGIFTBG_BEGIN + GG_GIFT_COUNT,
	WIID_GREENERGUIDE_PIC_STATUS,
	WIID_GREENERGUIDE_NONAME = WIID_GREENERGUIDE + 0x900,
	WIID_GREENERGUIDE_FORCEENUMMAX = WIID_GREENERGUIDE+0x999
};
//--------------------------------------------------
struct GreenerGuideData
{
	int iPlayerLevel;
	int iQuestID[GG_QUEST_COUNT];
	int iGiftBoy[GG_GIFT_COUNT];
	int iGiftBoyNum[GG_GIFT_COUNT];
	int iGiftGirl[GG_GIFT_COUNT];
	int iGiftGirlNum[GG_GIFT_COUNT];
	int iEly;

	char strMonster[GG_MONSTER_COUNT][MAX_STRING_SIZE];
	char strMonsterLevel[GG_MONSTER_COUNT][MAX_STRING_SIZE];
	char strMonsterMap[GG_MONSTER_COUNT][MAX_STRING_SIZE];

	char strHelp[MAX_STRING_SIZE];
	char strSkill[MAX_STRING_SIZE];
	char strCommend[MAX_STRING_SIZE];

	void reset( void )
	{
		iPlayerLevel = 0;
	}
};
//--------------------------------------------------
class SPWindow;
class SPWindowStatic;
class SPWindowButton;
class SPUIItemUnit;
//--------------------------------------------------
enum GREENERGUIDESTATUS
{
	GREENERGUIDE_NULL,
	GREENERGUIDE_ICON,		//!< icon notification
	GREENERGUIDE_CONTENT,	//!< content show
	GREENERGUIDE_FETCHING	//!< fetching
};
//--------------------------------------------------
class SPWindowGreenerGuide : public SPWindow
{
public:
	SPWindowGreenerGuide( WND_ID wndClassID, INSTANCE_ID instanceID );
	SPWindowGreenerGuide( INSTANCE_ID instanceID, 
		int iX, int iY, int iCX, int iCY, 
		SPWindow *pParent = NULL );
	~SPWindowGreenerGuide( void );

	virtual void Init( void );
	virtual void Clean( void );
	virtual void Show( bool bWithChild = true );
	virtual void Hide( bool bSendServer = true );
	virtual void Process( float fDeltaTime );
	virtual void Render( float fDeltaTime );
	virtual void RefreshRelationCoord( void );
protected:
	SPIMESSAGE_OBJ_MAP_DECLARE();
	int _onBtnFetchAward	( WPARAM wParam, LPARAM lParam );
	int _onBtnOK			( WPARAM wParam, LPARAM lParam );
	int	_onBtnNotification	( WPARAM wParam, LPARAM lParam );
	int _onShowGuide		( WPARAM wParam, LPARAM lParam );
	int _onFetchAwardAck	( WPARAM wParam, LPARAM lParam );
	int _onLevelUp			( WPARAM wParam, LPARAM lParam );
protected:
	bool _loadLdtCfg( void );
	bool _initGUI( void );

	void changeStatus( GREENERGUIDESTATUS status );
	void setIconVisible( bool bVisible );
	void setContentVisible( bool bVisible );
	void setWaitingVisible( bool bVisible );

	bool isLevelExist( int iLevel, GreenerGuideData *data = NULL );
	bool makeContent( int iGuideLevel );
protected:
	std::vector< GreenerGuideData > m_vecGreenerGuideData;
	GREENERGUIDESTATUS m_currStatus;
	//--------------------------------------------------
	SPWindowStatic	*m_pWndWaiting;
	SPWindowStatic	*m_pImgStatus;
	SPWindowStatic	*m_pImgWaitingBG;
	SPWindowStatic	*m_pImgWaitingMask;
	//--------------------------------------------------
	// icon
	SPWindowStatic	*m_pWndIcon;
	SPWindowStatic	*m_pI_Nofitication;
	//--------------------------------------------------
	// content
	SPWindowStatic	*m_pWndContent;
	SPWindowStatic	*m_pC_BgHeader;
	SPWindowStatic	*m_pC_BgBody;
	SPWindowStatic	*m_pC_BgBottom;
	SPWindowStatic	*m_pC_picInfo;
	SPWindowStatic	*m_pC_picMapMonster;
	SPWindowStatic	*m_pC_picQuest;
	SPWindowStatic	*m_pC_picGift;
	SPWindowButton	*m_pC_btnFetchAward;
	SPWindowButton	*m_pC_btnOK;
	SPWindowStatic	*m_pC_txtInfo;

	SPWindowStatic	*m_pC_txtMap[GG_MONSTER_COUNT];
	SPWindowStatic	*m_pC_txtMonster[GG_MONSTER_COUNT];
	SPWindowStatic	*m_pC_txtLevel[GG_MONSTER_COUNT];

	SPWindowStatic	*m_pC_txtQuest[GG_QUEST_COUNT];
	SPUIItemUnit	*m_pC_itemGift[GG_GIFT_COUNT];
	SPWindowStatic	*m_pC_txtSkill;
	SPWindowStatic	*m_pC_txtCommend;

	SPWindowStatic	*m_pC_Gift;
	SPWindowButton	*m_pC_btnClose;
	SPWindowStatic	*m_pC_picELY;
	SPWindowStatic	*m_pC_txtELY;
	//--------------------------------------------------
	int m_iCurrLevel;
	int m_iToLevel;
	float m_fAccuTimeNofity;
	//--------------------------------------------------
};
//--------------------------------------------------
// end of file
