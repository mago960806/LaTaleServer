// ***************************************************************
//  SPWindowChatMacro   version:  1.0   ¡¤  date: 08/07/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPWindowChatMacro_h__
#define SPWindowChatMacro_h__

#pragma once

#define WIID_CHAT_MACRO_IMG_MICRO					( WIID_CHAT_MACRO + 1 )
#define WIID_CHAT_MACRO_IMG_MAIN_TITLE				( WIID_CHAT_MACRO + 2 )

#define WIID_CHAT_MACRO_CLOSE_ICON					( WIID_CHAT_MACRO + 3 )
#define WIID_CHAT_MACRO_NORMAL						( WIID_CHAT_MACRO + 4 )
#define WIID_CHAT_MACRO_SPECIAL						( WIID_CHAT_MACRO + 5 )

#define WIID_CHAT_MACRO_SLIDER						( WIID_CHAT_MACRO + 6 )
#define WIID_CHAT_MACRO_SLIDER_UP					( WIID_CHAT_MACRO + 7 )
#define WIID_CHAT_MACRO_SLIDER_DOWN					( WIID_CHAT_MACRO + 8 )

#define WIID_CHAT_MACRO_IMG_TITLE					( WIID_CHAT_MACRO + 9 )
#define WIID_CHAT_MACRO_TITLE_EDIT					( WIID_CHAT_MACRO + 10 )
#define WIID_CHAT_MACRO_TITLE_EDIT_COMMENT			( WIID_CHAT_MACRO + 11 )

#define WIID_CHAT_MACRO_IMG_CONTENTS				( WIID_CHAT_MACRO + 12 )
#define WIID_CHAT_MACRO_CONTENTS_EDIT				( WIID_CHAT_MACRO + 13 )
#define WIID_CHAT_MACRO_TIP							( WIID_CHAT_MACRO + 14 )

#define WIID_CHAT_MACRO_MENU_BG_LEFT				( WIID_CHAT_MACRO + 15 )
#define WIID_CHAT_MACRO_MENU_BG_MIDDLE				( WIID_CHAT_MACRO + 16 )
#define WIID_CHAT_MACRO_MENU_BG_RIGHT				( WIID_CHAT_MACRO + 17 )

#define WIID_CHAT_MACRO_TITLE_EDIT_BG_LEFT			( WIID_CHAT_MACRO + 18 )
#define WIID_CHAT_MACRO_TITLE_EDIT_BG_MIDDLE		( WIID_CHAT_MACRO + 19 )
#define WIID_CHAT_MACRO_TITLE_EDIT_BG_RIGHT			( WIID_CHAT_MACRO + 20 )

#define WIID_CHAT_MACRO_CONTENTS_EDIT_BG_LEFT		( WIID_CHAT_MACRO + 21 )
#define WIID_CHAT_MACRO_CONTENTS_EDIT_BG_MIDDLE		( WIID_CHAT_MACRO + 22 )
#define WIID_CHAT_MACRO_CONTENTS_EDIT_BG_RIGHT		( WIID_CHAT_MACRO + 23 )

#define WIID_CHAT_MACRO_TIP_BG_LEFT					( WIID_CHAT_MACRO + 24 )
#define WIID_CHAT_MACRO_TIP_BG_MIDDLE				( WIID_CHAT_MACRO + 25 )
#define WIID_CHAT_MACRO_TIP_BG_RIGHT				( WIID_CHAT_MACRO + 26 )

#define WIID_CHAT_MACRO_LINE						( WIID_CHAT_MACRO + 30 )

#define WIID_CHAT_MACRO_CLOSE						( WIID_CHAT_MACRO + 31 )
#define WIID_CHAT_MACRO_CONTENTS_COUNT				( WIID_CHAT_MACRO + 32 )

#define WIID_CHAT_MACRO_UNIT_OUTLINE				( WIID_CHAT_MACRO + 100 )

enum CHAT_MACRO_BG
{
	CHAT_MACRO_BG_LEFT_TOP		=	0	,
	CHAT_MACRO_BG_CENTER_TOP			,
	CHAT_MACRO_BG_RIGHT_TOP				,

	CHAT_MACRO_BG_LEFT_MIDDLE			,
	CHAT_MACRO_BG_CENTER_MIDDLE			,
	CHAT_MACRO_BG_RIGHT_MIDDLE			,

	CHAT_MACRO_BG_LEFT_BOTTOM			,
	CHAT_MACRO_BG_CENTER_BOTTOM			,
	CHAT_MACRO_BG_RIGHT_BOTTOM			,

	CHAT_MACRO_BG_SUB_WHITE				,

	CHAT_MACRO_BG_COUNT					,
};

enum CHAT_MACRO_UNIT
{
	CHAT_MACRO_UNIT_X_COUNT		=	5	,
	CHAT_MACRO_UNIT_Y_COUNT		=	2	,

	CHAT_MACRO_UNIT_X_OFFSET	=	20	,
	CHAT_MACRO_UNIT_Y_OFFSET	=	67	,

	CHAT_MACRO_UNIT_X_GAP		=	41	,
	CHAT_MACRO_UNIT_Y_GAP		=	44	,

	CHAT_MACRO_UNIT_X_SIZE		=	32	,
	CHAT_MACRO_UNIT_Y_SIZE		=	32	,

	CHAT_MACRO_UNIT_COUNT		=	CHAT_MACRO_UNIT_X_COUNT	*	CHAT_MACRO_UNIT_Y_COUNT	,

	//////////////////////////////////////////////////////////////////////////

	CHAT_MACRO_UNIT_X_OUTLINE	=	2	,
	CHAT_MACRO_UNIT_Y_OUTLINE	=	2	,

	CHAT_MACRO_UNIT_X_OUTLINE_SIZE		=	CHAT_MACRO_UNIT_X_SIZE + CHAT_MACRO_UNIT_X_OUTLINE * 2	,
	CHAT_MACRO_UNIT_Y_OUTLINE_SIZE		=	CHAT_MACRO_UNIT_Y_SIZE + CHAT_MACRO_UNIT_Y_OUTLINE * 2	,

	//////////////////////////////////////////////////////////////////////////
};

enum CHAT_MACRO_TYPE;
struct CHAT_MACRO;
struct CHAT_MACRO_LDT;

typedef std::vector< CHAT_MACRO >							STD_VECTOR_CHAT_MACRO;
typedef std::vector< CHAT_MACRO_LDT >						STD_VECTOR_CHAT_MACRO_LDT;

class SPWindow;
class SPUIUnitManager;

class SPWindowChatMacro : public SPWindow 
{
public:
	SPWindowChatMacro(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowChatMacro(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowChatMacro();

	virtual	void		Init					();
	virtual	void		Clean					();
	virtual void		Show					( bool bWithChild = true );
	virtual void		Hide					( bool bSendServer = true) ;

	virtual	void		Process					( float fTime );
	virtual	void		Render					( float fTime );

	virtual void		RefreshRelationCoord	();

private:
	void				ReposOwnImage			();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int					OnBeginMove				( WPARAM wParam, LPARAM lParam );

	int					OnDragnDropEnd			( WPARAM wParam, LPARAM lParam );

	int					OnGetData				( WPARAM wParam, LPARAM lParam );
	int					OnReFresh				( WPARAM wParam, LPARAM lParam );
	int					OnTab					( WPARAM wParam, LPARAM lParam );
	int					OnSliderUp				( WPARAM wParam, LPARAM lParam );
	int					OnSliderDown			( WPARAM wParam, LPARAM lParam );

	int					OnSlider				( WPARAM wParam, LPARAM lParam );

	int					OnEditSetFocus			( WPARAM wParam, LPARAM lParam );
	int					OnEditKillFocus			( WPARAM wParam, LPARAM lParam );

	int					OnCloseLButtonUp		( WPARAM wParam, LPARAM lParam );
	int					OnNormalLButtonUp		( WPARAM wParam, LPARAM lParam );
	int					OnSpecialLButtonUp		( WPARAM wParam, LPARAM lParam );

	int					OnUnitLButtonUp			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnUnitLButtonDown		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnUnitCursorMove		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnUnitSliderUp			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnUnitSliderDown		( unsigned int iID, WPARAM wParam , LPARAM lParam );



	// }}

	//////////////////////////////////////////////////////////////////////////

	void				LoadChatMacroLDT		();

	void				InitChatMacroUnit		();
	void				UpdatePosChatMacroUnit	();

	void				Update					( CHAT_MACRO_TYPE eType );
	void				UpdateUnit				( CHAT_MACRO_TYPE eType );
	void				UpdateSelectIcon		( int iSelectNumber );

	void				SliderShow				( bool bShow );
	void				ClearPage				();

	CHAT_MACRO_TYPE		GetMacroType			();

	void				AddMacroData			( CHAT_MACRO_TYPE eType , CHAT_MACRO& stChatMacro );
	CHAT_MACRO*			GetMacroData			( int iIndex , CHAT_MACRO_TYPE eType );
	CHAT_MACRO_LDT*		GetMacroLdtData			( int iIndex , CHAT_MACRO_TYPE eType );

	void				SaveMacroDataMem		( int iIndex , CHAT_MACRO_TYPE eType , const char* pszName , const char* pszContents );
	void				MacroDataMemToFile		();
	void				MacroDataFileToMem		();

	void				LoadMacroFile			();
	void				SaveMacroFile			();

	void				FilteringMacroData		( CHAT_MACRO& stChatMacro );

	void				ProcessEditCtrl			( float fTime );

	void				ReFreshQuickSlot		();

	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//

	SPTexture*								m_pBGTexture;
	RECT									m_rcBGDest[ CHAT_MACRO_BG_COUNT ];
	RECT									m_rcBGSrc[ CHAT_MACRO_BG_COUNT ];

	SPUIUnitManager							m_UIUnitManager;

	SPWindowSlider*							m_pSlider;

	STD_VECTOR_CHAT_MACRO_LDT				m_vLDTBuffer[ CHAT_MACRO_TYPE_COUNT ];

	STD_VECTOR_CHAT_MACRO					m_vFileBuffer[ CHAT_MACRO_TYPE_COUNT ];
	STD_VECTOR_CHAT_MACRO					m_vMemBuffer[ CHAT_MACRO_TYPE_COUNT ];

	int										m_iSelectIndex;

	bool									m_bUse;

	BOOL									m_bLMouseDown;
	LPARAM									m_lParam;

};

#endif // SPWindowChatMacro_h__