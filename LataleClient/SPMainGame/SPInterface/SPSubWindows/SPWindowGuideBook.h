// ***************************************************************
//  SPWindowGuideBook   version:  1.0   ¡¤  date: 11/19/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPWindowGuideBook_h__
#define SPWindowGuideBook_h__

#pragma once

#define WIID_GUIDE_BOOK_CLOSE_ICON						( WIID_GUIDE_BOOK + 1 )
#define WIID_GUIDE_BOOK_TITLE_ICON						( WIID_GUIDE_BOOK + 2 )
#define WIID_GUIDE_BOOK_INDEX_ICON						( WIID_GUIDE_BOOK + 3 )
#define WIID_GUIDE_BOOK_SUBJECT_TREE					( WIID_GUIDE_BOOK + 100 )
#define WIID_GUIDE_BOOK_TYPE							( WIID_GUIDE_BOOK + 200 )
#define WIID_GUIDE_BOOK_SUBJECT							( WIID_GUIDE_BOOK + 300 )
#define WIID_GUIDE_BOOK_CONTENTS						( WIID_GUIDE_BOOK + 400 )


//////////////////////////////////////////////////////////////////////////

enum GUIDE_BOOK_TYPE
{
	GUIDE_BOOK_TYPE_NULL		=	0	,
	GUIDE_BOOK_TYPE_GENERAL				,
	GUIDE_BOOK_TYPE_QUEST				,
	GUIDE_BOOK_TYPE_SCENARIO			,
	GUIDE_BOOK_TYPE_SKILL				,
	GUIDE_BOOK_TYPE_COUNT				,
};

typedef std::vector< SPRequire >						STD_VECTOR_GUIDE_BOOK_REQUIRE;

struct GUIDE_BOOK_LDT 
{
	int													m_iItemNo;

	D3DXCOLOR											m_cSubjectColor;
	bool												m_bBold;

	GUIDE_BOOK_TYPE										m_eGroup;
	int													m_iSectionLevel;

	std::string											m_strSubject;
	std::string											m_strContent;

	STD_VECTOR_GUIDE_BOOK_REQUIRE						m_vRequireBuffer;
};

typedef std::map< int , GUIDE_BOOK_LDT >				STD_MAP_GUIDE_BOOK_LDT;
typedef std::map< int , GUIDE_BOOK_LDT* >				STD_MAP_GUIDE_BOOK_LDT_P;
typedef std::map< int , STD_MAP_GUIDE_BOOK_LDT_P >		STD_MAP_GUIDE_BOOK;

//////////////////////////////////////////////////////////////////////////

class SPTreeBoxItemTexture;
class SPWindow;

class SPWindowGuideBook : public SPWindow 
{

public:
	SPWindowGuideBook									( WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowGuideBook									( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowGuideBook							();

	virtual	void				Init					();
	virtual	void				Clean					();
	virtual void				Show					( bool bWithChild = true );
	virtual void				Hide					( bool bSendServer = true);

	virtual	void				Process					( float fTime );
	virtual	void				Render					( float fTime );

//	virtual void				RefreshRelationCoord	();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int							OnView					( WPARAM wParam, LPARAM lParam );
	int							OnClose					( WPARAM wParam, LPARAM lParam );
	int							OnTreeBoxLButtonUp		( WPARAM wParam, LPARAM lParam );

	int							OnType					( unsigned int iID, WPARAM wParam , LPARAM lParam );

	// }}

	//////////////////////////////////////////////////////////////////////////

	void						LoadGlobalSysLDT		();
	void						LoadGuideBookLDT		();

	int							ConvertLevelToKey		( int iLevel );

	void						AddGuideBookItemNo		( GUIDE_BOOK_LDT& stGuideBook );
	void						AddGuideBookLevel		( GUIDE_BOOK_LDT* pstGuideBook );

	GUIDE_BOOK_LDT*				GetGuideBookItemNo		( int iItemNo );
	STD_MAP_GUIDE_BOOK_LDT_P*	GetGuideBookLevel		( int iLevel );

	SPTreeBoxItemTexture*		CreateTreeBoxItem		();
	void						SetTreeBoxItemData		( SPTreeBoxItemTexture* pItem , GUIDE_BOOK_LDT* pstGuideBook );
	void						SetTreeBoxItem			( int iPos , GUIDE_BOOK_LDT* pstGuideBook );

	void						UpdateItemNo			( int iItemNo );
	void						UpdateLevel				( int iLevel );
	int							UpdateLevel				( int iStartItemPos , STD_MAP_GUIDE_BOOK_LDT_P* pmGuideBookLDT , SPWindow* pTreeBoxWindow );

	bool						CheckRequire			( SPRequire& stRequire );

	void						SetType					( GUIDE_BOOK_TYPE eType );
	GUIDE_BOOK_TYPE				GetType					();
	void						SetTypeButton			( GUIDE_BOOK_TYPE eType );
	void						SetSubject				( const char* pszSubject );
	void						SetContents				( const char* pszContents );

	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//

	bool												m_bUse;
	bool												m_bView;
	GUIDE_BOOK_TYPE										m_eType;

	int													m_iLevelRange;

	STD_MAP_GUIDE_BOOK_LDT								m_mGuideBookItemNo;
	STD_MAP_GUIDE_BOOK									m_mGuideBookLevel;

	SPTreeBoxItemTexture*								m_pCurSelectItem;

	//////////////////////////////////////////////////////////////////////////
};

#endif // SPWindowGuideBook_h__