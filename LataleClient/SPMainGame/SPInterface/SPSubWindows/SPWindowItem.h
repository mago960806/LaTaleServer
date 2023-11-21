
#pragma once

//#define WIID_ITEM_BASE			(WIID_ITEM + 1)
#define WIID_ITEM_TOP					(WIID_ITEM + 1)
#define WIID_ITEM_CENTER				(WIID_ITEM + 2)
#define WIID_ITEM_BOTTOM				(WIID_ITEM + 3)

#define WIID_ITEM_TITLE_IMAGE			(WIID_ITEM + 5)
#define WIID_ITEM_TITLE					(WIID_ITEM + 6)

#define WIID_ITEM_SKIN					(WIID_ITEM + 10)
#define WIID_ITEM_SKIN_UP_LINE			(WIID_ITEM + 11)
#define WIID_ITEM_SKIN_DOWN_LINE		(WIID_ITEM + 12)

#define WIID_ITEM_HIDE					(WIID_ITEM + 15)

#define WIID_ITEM_GROUP					(WIID_ITEM + 20)		//Ä«Å×°í¸® ¹öÆ° ¹Ù´Ú

#define WIID_ITEM_EQUIP					(WIID_ITEM + 40)
#define WIID_ITEM_CONSUME				(WIID_ITEM + 50)
#define WIID_ITEM_ETC					(WIID_ITEM + 60)
#define WIID_ITEM_EVENT					(WIID_ITEM + 70)
#define WIID_ITEM_PET					(WIID_ITEM + 80)
#define WIID_ITEM_HOUSING				(WIID_ITEM + 90)
#define WIID_ITEM_GRID					(WIID_ITEM + 100)
#define WIID_ITEM_LINE_UP				(WIID_ITEM + 110)
#define WIID_ITEM_LINE_DOWN				(WIID_ITEM + 120)
#define WIID_ITEM_SLIDER				(WIID_ITEM + 130)

#define WIID_ITEM_ENCHANT				(WIID_ITEM + 140)
#define WIID_ITEM_ITEMUPGRADE			(WIID_ITEM + 150)
#define WIID_ITEM_USERSHOP				(WIID_ITEM + 160)

#define WIID_ITEM_ELY_BASE				(WIID_ITEM + 170)
#define WIID_ITEM_ELY					(WIID_ITEM + 180)
#define WIID_ITEM_TIMEADD_BUTTON		(WIID_ITEM + 190)
#define WIID_ITEM_PET_LEVELUP			(WIID_ITEM + 200)
#define WIID_ITEM_EFFECTITEM_UPGRADE	(WIID_ITEM + 210)

class SPWindow;
class SPWindowSlider;
class SPWindowGrid;
class SPItemCluster;
class SPUIItemUnit;

class SPWindowItem : public SPWindow 
{

public:
	SPWindowItem(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowItem(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowItem();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

	virtual void RefreshRelationCoord( void );

	SPWindow* GetTimeAddWindow() {
		return m_pTimeAdd;
	}
	
public:
	//SPWindow* GetGridWindow() {
	//	return m_pGrid;
	//}

protected:
	/**
	 *
	 @param 
	 @return  
	*/
	void	UpdateSubWindowPos(bool bShowHotList = false);

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()	
	int OnCursorEnter	( WPARAM, LPARAM );
	int OnCursorOut		( WPARAM, LPARAM );
	int OnLButtonDown	( WPARAM, LPARAM );
	int OnLButtonUp		( WPARAM, LPARAM );	
	int OnRButtonDown	( WPARAM, LPARAM );		//[2005/5/23]
	int OnRButtonUP		( WPARAM, LPARAM );		//[2005/5/23]	

	int	OnButtonPageUp		( WPARAM wParam, LPARAM lParam );
	int	OnButtonPageDown	( WPARAM wParam, LPARAM lParam );
	int	OnSliderPageUp		( WPARAM wParam, LPARAM lParam );
	int	OnSliderPageDown	( WPARAM wParam, LPARAM lParam );
	int	OnSliderPageNum		( WPARAM wParam, LPARAM lParam );
	int OnSliderButtonUp	( WPARAM wParam, LPARAM lParam );		//[2005/7/8]

	virtual int OnToolTipEnter	( WPARAM, LPARAM );
	virtual int OnToolTipOut	( WPARAM, LPARAM );
	virtual int OnToolTipMove	( WPARAM, LPARAM );

	// Button Event
	int OnHide			( WPARAM wParam, LPARAM lParam );		//[2005/7/8]
	int OnEquip			( WPARAM, LPARAM ) ;
	int OnConsumable	( WPARAM, LPARAM ) ;
	int OnEtc			( WPARAM, LPARAM ) ;
	int OnEvent			( WPARAM, LPARAM ) ;
	int OnPet			( WPARAM, LPARAM ) ;
	int OnHousing		( WPARAM, LPARAM ) ;

	int OnSetDefault	( WPARAM wParam, LPARAM lParam );		//[2005/11/27]
	
	int OnSetLine		( WPARAM wParam, LPARAM lParam );		//[2006/12/21] - jinssaga   

	int OnCursorMove	( WPARAM wParam, LPARAM lParam );		//[2005/5/23]
	
	//int OnGrid			( WPARAM wParam, LPARAM lParam);
	int OnGridDown		( WPARAM wParam, LPARAM lParam);
	int OnGridUp		( WPARAM wParam, LPARAM lParam);
	int OnGridDBK 		( WPARAM wParam, LPARAM lParam);		//[2005/10/27]
	int OnItemClearAllUserShopItem	( WPARAM wParam, LPARAM lParam);	//  [9/5/2007 AJJIYA]
	int OnItemSetting	( WPARAM wParam, LPARAM lParam);
	int OnItemDisable	( WPARAM wParam, LPARAM lParam);
	int OnItemClear		( WPARAM wParam, LPARAM lParam);
	int OnItemAllClear	( WPARAM wParam, LPARAM lParam);	
	
	int OnItemSetStack	( WPARAM wParam, LPARAM lParam);			//[2005/5/23] ½ºÅÃ Àç¼³Á¤

	int OnExitHide		( WPARAM wParam, LPARAM lParam);			//[2005/5/21] ´ÝÀ½ ¹öÆ° ¼û±è	
	int OnSubMenu		( WPARAM wParam, LPARAM lParam);			//[2008/5/26] ÇÏ´Ü ¸Þ´ºÀÇ Show/Hide

	int OnItemSellSelect( WPARAM wParam, LPARAM lParam);			//[2005/5/20]

	int OnSetMaxPage	( WPARAM wParam, LPARAM lParam);			//[2005/7/6]
	int OnSetPage		( WPARAM wParam, LPARAM lParam);

	int OnUse			( WPARAM wParam, LPARAM lParam);			//[2005/5/23] ¾ÆÀÌÅÛ »ç¿ë	

	int OnDragnDropEnd	( WPARAM wParam, LPARAM lParam );			// AJJIYA [7/18/2005]

	int OnItemEnchantKeyUp	( WPARAM wParam, LPARAM lParam );			// AJJIYA [5/22/2006]
	int OnItemUpgradeKeyUp	( WPARAM wParam, LPARAM lParam );			// AJJIYA [5/22/2006]

	int OnOpenUserShop	( WPARAM wParam, LPARAM lParam );
	int OnItemAddTime	( WPARAM wParam, LPARAM lParam );
	int OnPetLevelUP	( WPARAM wParam, LPARAM lParam );
	int OnPhantomLevelUp( WPARAM wParam, LPARAM lParam );//»ÃÓ°Éý¼¶

	int OnSetHotStatus	( WPARAM wParam, LPARAM lParam );
	// }}

	void	CheckDragNDrop();

protected:	
	SPWindow*		m_pHide;			/**< Á¾·á ¹öÆ° <br> */
	SPWindowSlider*	m_pSlider;			/**< Scroll Bar <br> */
	SPWindowGrid*	m_pGrid;			/**< Grid Control <br> */

	int				m_iCurrentLine;		/**< ÇöÀç ¶óÀÎ Á¤º¸ <br> */
	int				m_iSelectIndex;		/**< ÇöÀç ¼±ÅÃÇÑ ±×¸®µå ÀÎµ¦½º <br> */

	int				m_iLine;			/**< ÇöÀç ¶óÀÎ <br> */

	SPUIItemUnit*	m_pSelectItem;		//	ÇöÀç ¼±ÅÃµÈ CONSUME ¾ÆÀÌÅÛ [7/18/2005]
	SPWindow*		m_pUserShop;
	SPWindow*		m_pUpGrade;
	SPWindow*		m_pEnchant;
	SPWindow*		m_pEly;
	SPWindow*		m_pTimeAdd;			/**< ½Ã°£ ¿¬Àå À©µµ¿ì <br> */
	SPWindow*		m_pPetLevelUP;
	SPWindow*       m_pPhantomLevelUp;  //»ÃÓ°Éý¼¶

	bool			m_bUseItemEnchant;
	bool			m_bUseItemUpgrade;
	bool			m_bUesItemTimeAdd;
	bool			m_bUseItemPetLevelUp;
	bool            m_bUseItemPhantomLevelUp; //»ÃÓ°Éý¼¶

	int				m_iPrevSlotIndex;

};
