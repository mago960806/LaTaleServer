// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-7-1    15:54 
//***************************************************************************

#pragma once

class SPWindow;
class SPUIUnit;
class SPUIUnitManager;

#define WIID_QUICK_SLOT_1LINE_SHOW			( WIID_QUICK_SLOT + 1 )
#define WIID_QUICK_SLOT_1LINE_HOTKEY_1		( WIID_QUICK_SLOT + 2 )
#define WIID_QUICK_SLOT_1LINE_HOTKEY_2		( WIID_QUICK_SLOT + 3 )

#define WIID_QUICK_SLOT_2LINE_HIDE			( WIID_QUICK_SLOT + 4 )
#define WIID_QUICK_SLOT_2LINE_HOTKEY_1		( WIID_QUICK_SLOT + 5 )
#define WIID_QUICK_SLOT_2LINE_HOTKEY_2		( WIID_QUICK_SLOT + 6 )

#define WIID_QUICK_SLOT_1LINE_START			WIID_QUICK_SLOT_1LINE_SHOW
#define WIID_QUICK_SLOT_1LINE_END			WIID_QUICK_SLOT_1LINE_HOTKEY_2
#define WIID_QUICK_SLOT_1LINE_COUNT			( WIID_QUICK_SLOT_1LINE_END - WIID_QUICK_SLOT_1LINE_START )

#define WIID_QUICK_SLOT_2LINE_START			WIID_QUICK_SLOT_2LINE_HIDE
#define WIID_QUICK_SLOT_2LINE_END			WIID_QUICK_SLOT_2LINE_HOTKEY_2
#define WIID_QUICK_SLOT_2LINE_COUNT			( WIID_QUICK_SLOT_2LINE_END - WIID_QUICK_SLOT_2LINE_START )

#define WIID_QUICK_SLOT_HOTKEY_DIFF			( WIID_QUICK_SLOT_2LINE_HOTKEY_1 - WIID_QUICK_SLOT_1LINE_HOTKEY_1 )


#define QUICKSLOT_BG_COUNT				4

#define	QUICK_X							12
#define QUICK_Y							2
#define QUICK_X_GAP						34
#define QUICK_Y_GAP						34
#define QUICK_X_OFFSET					4
#define QUICK_Y_OFFSET					2

#define QUICK_HOTKEY_COUNT				2

#define QUICK_HOTKEY_IMG_COUNT			2

class SPWindowQuickSlot : public SPWindow 
{

public:
	SPWindowQuickSlot(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowQuickSlot(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowQuickSlot();

	virtual	void		Init				();
	virtual	void		Clean				();
	virtual void		Show				(bool bWithChild = true);
	virtual void		Hide				(bool bSendServer = true);

	virtual	void		Process				( float fTime );
	virtual	void		Render				( float fTime );

	virtual void		RefreshRelationCoord();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int					OnPurge				( WPARAM wParam, LPARAM lParam );
	int					OnCursorEnter		( WPARAM wParam, LPARAM lParam );
	int					OnCursorOut			( WPARAM wParam, LPARAM lParam );
	int					OnCursorMove		( WPARAM wParam, LPARAM lParam );

	int					OnLButtonUp			( WPARAM wParam, LPARAM lParam );
	int					OnLButtonDown		( WPARAM wParam, LPARAM lParam );

	int					OnDragnDropEnd		( WPARAM wParam, LPARAM lParam );

	int					OnGetQuickSlot		( WPARAM wParam, LPARAM lParam );
	int					OnQuickSlotUpdate	( WPARAM wParam, LPARAM lParam );
	int					OnStatusRefresh		( WPARAM wParam, LPARAM lParam );

	int					On1LineShow			( WPARAM wParam, LPARAM lParam );
	int					On1LineHotKey1		( WPARAM wParam, LPARAM lParam );
	int					On1LineHotKey2		( WPARAM wParam, LPARAM lParam );
	int					On2LineShow			( WPARAM wParam, LPARAM lParam );
	int					On2LineHotKey1		( WPARAM wParam, LPARAM lParam );
	int					On2LineHotKey2		( WPARAM wParam, LPARAM lParam );

	//[xialin 2008/11/06]add
	int					OnGetQuickSlotUnit  ( WPARAM wParam, LPARAM lParam );


	// }}

	void				InitIcon			();
	void				InitIconData		();
	void				CleanIcon			();

	void				UpdateIconPage		( int iPage );
	void				UpdateIconSlot		( int iSlotNum );
	void				UpdateIconID		( int iID );
	void				UpdateIconChatMacro	( int iID );
	void				UpdateIconChatMacro	( LPARAM lParam );

	void				BtnCheck			( int iNum );
	void				LineCheck			( int iNum );

	SPUIUnitManager*	GetUIUnitManager	( INSTANCE_ID iID );

	void				InitActionKey		();
	void				ProcessActionKey	( float fTime );
	void				ProcessJoyPad		( float fTime );		//[2007/5/29]-jinhee

	void				InitHotKey			();
	void				ProcessHotKey		();

	void				SwapQuickSlot		( int iSourNum , int iDestNum );

	void				DeleteItemSlot		( int iID );
	void				DeleteSkillSlot		( int iSaveCheck );

	void				ClearQuickSlots		( std::vector<int>* pClearSlots );

	void				CheckItemDrag		();

	//	퀵슬롯에서 사용될 이미지 미리 로딩한다.
	void				PreTextureLoad		();
	void				PreTextureClean		();

	//	퀵슬록 업데이트를 계속하지 않는다.지연!!!
	void				ProcessUpdate		( float fTime );

	void				ReposOwnImage		();


	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	BOOL									m_bLMouseDown;
	LPARAM									m_lParam;

	SPUIUnitManager							m_UIQuickSlotUnitManager;
	SPUIUnitManager							m_UINoneUnitManager;
	SPUIUnitManager							m_UIItemUnitManager;
	SPUIUnitManager							m_UISkillUnitManager;
	SPUIUnitManager							m_UIChatMacroUnitManager;

	SPTexture*								m_pBGTexture;
	RECT									m_rcBGDest[ QUICKSLOT_BG_COUNT * QUICK_HOTKEY_COUNT ];
	RECT									m_rcBGSrc[ QUICKSLOT_BG_COUNT * QUICK_HOTKEY_COUNT ];

	SPTexture*								m_pHotKeyImage;
	RECT									m_rtHotKeyDest[ QUICK_HOTKEY_IMG_COUNT ];
	RECT									m_rtHotKeySrc[ QUICK_HOTKEY_IMG_COUNT ];

	int										m_iDragDropUnitNumber;

	QUICK_SLOT								m_QuickSlot[ QUICK_X * QUICK_Y * QUICK_HOTKEY_COUNT ];

	int										m_iCurPage;
	BOOL									m_bHotKey;
	int										m_iCurLine;

	typedef	std::vector< SPTexture* >		STD_VECTOR_SPTEXTURE;
	STD_VECTOR_SPTEXTURE					m_vpPreTexture;

	float									m_fLimitTime;
	float									m_fAccumulateTime;
	bool									m_bUpdate;

	float									m_fInputPressRepeatTime[ QUICK_X * QUICK_Y ];
	bool									m_bChatMessage[ QUICK_X * QUICK_Y ];

	float									m_fPadPressRepeatTime[ QUICK_X * QUICK_Y ];
	bool									m_bJoyPadZPress;	
};
