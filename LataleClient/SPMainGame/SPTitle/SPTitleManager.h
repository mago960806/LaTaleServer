// ***************************************************************
//  SPTitleManager   version:  1.0   ¡¤  date: 03/29/2006
//  -------------------------------------------------------------
//  
//	AJJIYA
//
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class SPTitleAttr;
class SPTitleRenderBase;
class SPTitleArchive;

typedef	std::map< UINT32 , SPTitleAttr >		STD_MAP_TITLE_ATTR;

class SPTitleManager
{
private:
	SPTitleManager(void);
	static	SPTitleManager*	m_pTitleManagerInstance;

public:
	virtual ~SPTitleManager	(void);

	static	SPTitleManager*	GetInstance			(void);
	static	void			DelInstance			(void);

	SPTitleAttr*			GetTitleAttr		( UINT32 uiTitleID );
	SPTitleRenderBase*		GetCreteTitleBase	( UINT32 uiTitleID , bool bNoUse );

	SPTitleArchive*			GetTitleArchive		();

	void					SetSelectTitle		( int iSelectTitle );
	int						GetSelectTitle		();

	void					SendChangeTitle		( SPTitleData& stTitleData );

protected:

	void	Init				();

	void	LoadLDTFile			();

	void	AddTitleAttr		( UINT32 uiTitleID , SPTitleAttr& TitleAttr );

	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼ö¸í
	//

	STD_MAP_TITLE_ATTR			m_mTitleAttr;

	SPTitleArchive				m_TitleArchive;

	int							m_iSelectTitle;

};
