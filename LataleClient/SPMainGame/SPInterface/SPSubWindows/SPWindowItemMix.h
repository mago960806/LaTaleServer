// ***************************************************************
//  SPWindowItemMix   version:  1.0   ·  date: 09/03/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPWindowItemMix_h__
#define SPWindowItemMix_h__

#pragma once

#define WIID_ITEM_MIX_IMG_TITLE					( WIID_ITEM_MIX + 1 )
#define WIID_ITEM_MIX_CLOSE						( WIID_ITEM_MIX + 2 )

#define WIID_ITEM_MIX_IMG_SUBJECT				( WIID_ITEM_MIX + 3 )
#define WIID_ITEM_MIX_TEXT_SUBJECT				( WIID_ITEM_MIX + 4 )

#define WIID_ITEM_MIX_GAGE_LEVEL				( WIID_ITEM_MIX + 5 )

#define WIID_ITEM_MIX_IMG_LIST					( WIID_ITEM_MIX + 6 )

#define WIID_ITEM_MIX_LIST_BTN					( WIID_ITEM_MIX + 7 )
#define LIST_BTN_COUNT							10

#define WIID_ITEM_MIX_TEXT_RESULT				( WIID_ITEM_MIX + 21 )

#define WIID_ITEM_MIX_IMG_RESULT_PIC			( WIID_ITEM_MIX + 22 )
#define WIID_ITEM_MIX_IMG_RESULT_BOX			( WIID_ITEM_MIX + 23 )

#define WIID_ITEM_MIX_IMG_MATERIAL_PIC			( WIID_ITEM_MIX + 30 )
#define WIID_ITEM_MIX_IMG_MATERIAL_BOX			( WIID_ITEM_MIX + 40 )
#define MATERIAL_BOX_COUNT						5

#define WIID_ITEM_MIX_IMG_BRACKET				( WIID_ITEM_MIX + 50 )

#define WIID_ITEM_MIX_TEXT_SUCCESS				( WIID_ITEM_MIX + 51 )
#define WIID_ITEM_MIX_IMG_SUCCESS				( WIID_ITEM_MIX + 52 )

#define WIID_ITEM_MIX_GAGE_RUN					( WIID_ITEM_MIX + 53 )

#define WIID_ITEM_MIX_RUN						( WIID_ITEM_MIX + 54 )
#define WIID_ITEM_MIX_CANCEL					( WIID_ITEM_MIX + 55 )

#define WIID_ITEM_MIX_LIST_BTN_LEFT				( WIID_ITEM_MIX + 60 )
#define WIID_ITEM_MIX_LIST_BTN_RIGHT			( WIID_ITEM_MIX + 61 )
#define WIID_ITEM_MIX_LIST_BTN_PAGE				( WIID_ITEM_MIX + 62 )

#define WIID_ITEM_MIX_LIST_TREE					( WIID_ITEM_MIX + 100 )

#define WIID_ITEM_MIX_GAGE_BG_LEVEL_BG			( WIID_ITEM_MIX + 200 )
#define WIID_ITEM_MIX_GAGE_BG_RUN_BG			( WIID_ITEM_MIX + 201 )

#define WIID_ITEM_MIX_GAGE_LEVEL_TEXT			( WIID_ITEM_MIX + 300 )

#define WIID_ITEM_MIX_BG_TITLE_LEFT				( WIID_ITEM_MIX + 1000 )
#define WIID_ITEM_MIX_BG_TITLE_CENTER			( WIID_ITEM_MIX + 1001 )
#define WIID_ITEM_MIX_BG_TITLE_RIGHT			( WIID_ITEM_MIX + 1002 )

#define WIID_ITEM_MIX_BG_LIST_LEFT				( WIID_ITEM_MIX + 1003 )
#define WIID_ITEM_MIX_BG_LIST_CENTER			( WIID_ITEM_MIX + 1004 )
#define WIID_ITEM_MIX_BG_LIST_RIGHT				( WIID_ITEM_MIX + 1005 )

#define WIID_ITEM_MIX_BG_SLOT_LEFT				( WIID_ITEM_MIX + 1006 )
#define WIID_ITEM_MIX_BG_SLOT_CENTER			( WIID_ITEM_MIX + 1007 )
#define WIID_ITEM_MIX_BG_SLOT_RIGHT				( WIID_ITEM_MIX + 1008 )

enum ITEM_MIX_BG
{
	ITEM_MIX_BG_LEFT_TOP		=	0	,
	ITEM_MIX_BG_CENTER_TOP				,
	ITEM_MIX_BG_RIGHT_TOP				,

	ITEM_MIX_BG_LEFT_MIDDLE				,
	ITEM_MIX_BG_CENTER_MIDDLE			,
	ITEM_MIX_BG_RIGHT_MIDDLE			,

	ITEM_MIX_BG_LEFT_BOTTOM				,
	ITEM_MIX_BG_CENTER_BOTTOM			,
	ITEM_MIX_BG_RIGHT_BOTTOM			,

	ITEM_MIX_BG_SUB_WHITE				,

	ITEM_MIX_BG_COUNT					,
};

struct TREEBOX_ITEM;

struct ITEMMIX_CATEGORY_ICON 
{
	int						m_iCategory;
	std::string				m_strFileName;
	int						m_iSrcSX;
	int						m_iSrcSY;
	int						m_iSrcEX;
	int						m_iSrcEY;
};

class SPItem;
class SPWindow;
class SPUIUnitManager;

class SPTreeBoxItemQuest;

class SPWindowItemMix : public SPWindow 
{
public:
	SPWindowItemMix(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowItemMix(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowItemMix();

	virtual	void			Init					();
	virtual	void			Clean					();
	virtual void			Show					( bool bWithChild = true );
	virtual void			Hide					( bool bSendServer = true) ;

	virtual	void			Process					( float fTime );
	virtual	void			Render					( float fTime );

	virtual void			RefreshRelationCoord	();

private:
	void					ReposOwnImage			();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int						OnWindowShow			( WPARAM wParam, LPARAM lParam );
	int						OnItemMixResult			( WPARAM wParam, LPARAM lParam );
	int						OnSkillUpdate			( WPARAM wParam, LPARAM lParam );
	int						OnItemMixSkillInit		( WPARAM wParam, LPARAM lParam );

	int						OnCloseLButtonUp		( WPARAM wParam, LPARAM lParam );
	int						OnTreeBoxLButtonUp		( WPARAM wParam, LPARAM lParam );
	int						OnRun					( WPARAM wParam, LPARAM lParam );
	int						OnCancel				( WPARAM wParam, LPARAM lParam );
	int						OnLeft					( WPARAM wParam, LPARAM lParam );
	int						OnRight					( WPARAM wParam, LPARAM lParam );

	int						OnResultCursorEnter		( WPARAM wParam, LPARAM lParam );
	int						OnResultCursorOut		( WPARAM wParam, LPARAM lParam );

	int						OnMaterial				( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int						OnMaterialCancel		( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int						OnMaterialCursorEnter	( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int						OnMaterialCursorOut		( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int						OnItemMixSkillLButtonUp	( unsigned int iID, WPARAM wParam , LPARAM lParam );

	// }}

	//////////////////////////////////////////////////////////////////////////

	void					LoadIconLDT				();

	void					AddCategoryIcon			( ITEMMIX_CATEGORY_ICON& stCategoryIcon );
	ITEMMIX_CATEGORY_ICON*	GetCategoryIcon			( int iCategory );


	TREEBOX_ITEM*			GetCategoryItem			( int iCategory );
	bool					DelCategoryItem			( int iCategory );

	SPTreeBoxItemQuest*		CreateTreeBoxItem		( bool bMenuType , int iCategory = 0 );

	void					InitUIItemUnit			();

	void					UpdatePosUIItemUnit		();

	void					UpdateTreeList			( int iSkillID );
	void					UpdateMaterial			( int iSkillID , int iMixID );
	void					UpdateItemMixInfo		();
	void					UpdateItemMixSkill		();

	void					ClearMaterial			();

	void					SetMaterialItem			( SPItem* pMaterialItem );

	void					ResetMaterialData		();
	void					ResetMaterialData		( int iMaterialPos );

	bool					CheckMaterialCondition	();
	bool					CheckMaterialCondition	( int& iOutMaterialCount , int& iOutMaterialPos , SPItemMix* pMixItem , SPItem* pMaterialItem , bool bMultiCheck );

	void					SetLevelText			( int iLevel );
	void					SetLevelGage			( float fCurValue , float fMaxValue );
	void					SetRunGage				( int iCurValue , int iMaxValue );
	void					SetSuccessProbability	( int iProbability );

	void					SendServerRun			();

	void					ProcessProgress			( float fTime );

	float					CalLevelPercent			( ITEMMIX_INFO* pItemMixInfo );
	float					CalSuccessPercent		( ITEMMIX_INFO* pItemMixInfo , SPItemMix* pCurItemMix );

	//////////////////////////////////////////////////////////////////////////

	void					SetError				( int iGloalStringID , const char* pszItemString = NULL );
	void					SetError				( const char* pszErrorString );
	SPItem*					GetInvenPickUpItem		();
	void					SetInvenItemLock		( SPItem* pkItem , int iIndex );
	void					SetInvenItemUnlock		( SPItem* pkItem , int iIndex );
	void					SetInvenItemAllUnlock	();
	void					SendParentAction		( UINT64 uiAction , unsigned int uiIndex = 0 , bool bSendInit = false );

	////////////////////////////////////////////////////////////////////////
	
	//	변수들
	

	SPTexture*										m_pBGTexture;
	RECT											m_rcBGDest[ ITEM_MIX_BG_COUNT ];
	RECT											m_rcBGSrc[ ITEM_MIX_BG_COUNT ];

	SPUIUnitManager									m_UIUnitManager;

	bool											m_bUse;

	typedef std::map< int , TREEBOX_ITEM* >			STD_MAP_TREEBOX_ITEM;

	STD_MAP_TREEBOX_ITEM							m_mpCategory;

	typedef std::map< int , ITEMMIX_CATEGORY_ICON >	STD_MAP_CATEGORY_ICON;

	STD_MAP_CATEGORY_ICON							m_mCategoryIcon;

	int												m_iCurSkillID;

	SPItemMix*										m_pCurItemMix;

	SPItem*											m_pMaterialItem[ MATERIAL_BOX_COUNT ];

	float											m_fProgressAccmulateTime;			//	프로그래스바 현재 시간
	float											m_fProgressMaxTime;					//	프로그래스바 맥스 시간

	int												m_iCurItemMixSkillPage;

};

#endif // SPWindowItemMix_h__