/**
 *	@file SPWindowBugleSelect.h
 *	@brief
		choose bugle menu. Shortcut window.
 *	@author
		jmulrosbok, snda.
 *	@data
		2008-07-15
 */
//--------------------------------------------------
#pragma once
//--------------------------------------------------
// PROTECTED START: NEVER INSERT DEFINEs BETWEEN FOLLOWs
//--------------------------------------------------
#define SLOT_COUNT	(4)
#define WIID_BUGLE_MENU_ICON_START		(WIID_BUGLE_MENU + 1)
#define WIID_BUGLE_MENU_ICON_END		(WIID_BUGLE_MENU_ICON_START + SLOT_COUNT-1)
#define WIID_BUGLE_MENU_NAME_START		(WIID_BUGLE_MENU_ICON_END + 1)
#define WIID_BUGLE_MENU_NAME_END		(WIID_BUGLE_MENU_NAME_START + SLOT_COUNT-1)
#define WIID_BUGLE_MENU_SETTING_START	(WIID_BUGLE_MENU_NAME_END + 1)
#define WIID_BUGLE_MENU_SETTING_END		(WIID_BUGLE_MENU_SETTING_START + SLOT_COUNT-1)
#define WIID_BUGLE_MENU_SLOT_BG_START	(WIID_BUGLE_MENU_SETTING_END + 1)
#define WIID_BUGLE_MENU_SLOT_BG_END		(WIID_BUGLE_MENU_SLOT_BG_START + SLOT_COUNT-1)
//--------------------------------------------------
// PROTECTED END
//--------------------------------------------------
#include "SPWindowBugleSelect.h"
class SPWindow;
class SPUIUnitManager;
//--------------------------------------------------
class SPWindowBugleMenu : public SPWindow
{
public:
	SPWindowBugleMenu( WND_ID wndClassID, INSTANCE_ID instanceID );
	SPWindowBugleMenu( INSTANCE_ID instanceID, 
		int iX, int iY, int iCX, int iCY, 
		SPWindow *pParent = NULL );
	virtual ~SPWindowBugleMenu( void );

	virtual void Init( void );
	virtual void Clean( void );
	virtual void Show( bool bWithChild = true );
	virtual void Hide( bool bSendServer = true );
	virtual void Process( float fDeltaTime );
	virtual void Render( float fDeltaTime );
	virtual void RefreshRelationCoord( void );

public:
	bool SetSlotBugleInfo( const BugleInfo &info );

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE();
	// select appointed bugle slot from icon
	int _onBugleSelectIconBtn( unsigned int iID, WPARAM wParam, LPARAM lParam );
	// select appointed bugle slot from name
	int _onBugleSelectNameBtn( unsigned int iID, WPARAM wParam, LPARAM lParam );
	// select appointed bugle slot from backgroud image
	int _onBugleSelectBg( unsigned int iID, WPARAM wParam, LPARAM lParam );
	// open setting window for appointed bugle slot
	int _onBugleSettingBtn( unsigned int iID, WPARAM wParam, LPARAM lParam );
	// update slot information
	bool _updateSlot( int slot_index );

	/// on bugle name, icon, get focus
	int _onBugleNameFocus( unsigned int iID, WPARAM wParam, LPARAM lParam );
	int _onBugleIconFocus( unsigned int iID, WPARAM wParam, LPARAM lParam );

	/// on lost focus, name, icon
	int _onBugleNameLostFocus( unsigned int iID, WPARAM wParam, LPARAM lParam );
	int _onBugleIconLostFocus( unsigned int iID, WPARAM wParam, LPARAM lParam );

private:
	bool _initGUI( void );
	bool _onBugleSelect( int slot_index );
	bool _onBugleSetting( int slot_index );

private:
	bool _loadBugleMenuSetting( void );
	bool _saveBugleMenuSetting( void );

protected:
	SPWindowButton	*m_pVecBugleIcon[SLOT_COUNT];
	SPWindowButton	*m_pVecBugleName[SLOT_COUNT];
	SPWindowButton	*m_pVecBugleSetting[SLOT_COUNT];
	SPWindowButton	*m_pVecSlotBG[SLOT_COUNT];
	int			m_iCurSlot;	//!< current been setting slot

public:
	static BugleInfo m_bugleInfo[SLOT_COUNT];	//!< selected bugle info
}; // end of class SPWindowBugleMenu
//--------------------------------------------------
// end of file
