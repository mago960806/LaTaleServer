#pragma once

#define WIID_MENU_CACHESHOP		(WIID_MENU+1)
#define WIID_MENU_POPUP			(WIID_MENU+2)
#define WIID_MENU_LOTTERY		(WIID_MENU+3)

//#define WIID_MENU_ITEM		(WIID_MENU+1)
//#define WIID_MENU_EQUIP		(WIID_MENU+2)
//#define WIID_MENU_SKILL		(WIID_MENU+3)
//#define WIID_MENU_STATUS	(WIID_MENU+4)
//#define WIID_MENU_QUEST		(WIID_MENU+5)
//#define WIID_MENU_TEAM		(WIID_MENU+6)
//#define WIID_MENU_FRIEND	(WIID_MENU+7)
//#define WIID_MENU_OPTION	(WIID_MENU+8)
//#define WIID_MENU_EXIT		(WIID_MENU+9)
//#define WIID_MENU_CACHESHOP	(WIID_MENU+10)
//#define WIID_MENU_HELP		(WIID_MENU+11)

class SPWindow;

class SPWindowMenu : public SPWindow
{
public:
	SPWindowMenu(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowMenu();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide();

	virtual void RefreshRelationCoord();
	void ReposOwnImage();


protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int OnCursorEnter	( WPARAM, LPARAM );
	int OnCursorOut		( WPARAM, LPARAM );
	int OnLButtonDown	( WPARAM, LPARAM );
	int OnLButtonUp		( WPARAM, LPARAM );
	int OnCashEnable	( WPARAM, LPARAM ); //[2006/11/2]
	int OnShowPopup		( WPARAM wParam, LPARAM lParam);
	int OnCashShopEnter	( WPARAM wParam , LPARAM lParam );

	////
	//int OnItem 			( WPARAM, LPARAM );
	//int OnEquip 		( WPARAM, LPARAM );
	//int OnSkill 		( WPARAM, LPARAM );
	//int OnStatus 		( WPARAM, LPARAM );
	//int OnQuest 		( WPARAM, LPARAM );
	//int OnTeam 			( WPARAM, LPARAM );
	//int OnFriend		( WPARAM, LPARAM );
	//int OnOption 		( WPARAM, LPARAM );		
	int OnCacheShop		( WPARAM, LPARAM );
	int OnMenuPopUp		( WPARAM, LPARAM );
	int OnLottery		( WPARAM, LPARAM );
	//int OnExit 			( WPARAM, LPARAM );
	//int OnHelp			( WPARAM, LPARAM );
	// }}
	

	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//

	float			m_fLimitTime;
	float			m_fAccmulateTime;

	RECT			m_rcHeadSrc;
	RECT			m_rcBodySrc;
	RECT			m_rcTailSrc;

	RECT			m_rcCalcHead;
	RECT			m_rcCalcBody;
	RECT			m_rcCalcTail;

	SPWindow*		m_pPopup;	
};








