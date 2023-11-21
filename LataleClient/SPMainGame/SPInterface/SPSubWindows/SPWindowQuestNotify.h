/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/06/29
	created:	2006:6:29:   1:21
	filename: 	SPWindowQuestNotify.h
	
	purpose:	
*********************************************************************/

#pragma once

struct TREEBOX_ITEM;

class SPWindow;
class SPTreeBoxItemQuest;

class SPWindowQuestNotify : public SPWindow 
{
public:

	SPWindowQuestNotify(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowQuestNotify(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowQuestNotify();

	virtual	void			Init					();
	virtual	void			Clean					();
	virtual void			Show					( bool bWithChild = true );
	virtual void			Hide					( bool bSendServer = true );

	virtual	void			Process					( float fTime );
	virtual	void			Render					( float fTime );
	virtual void			RefreshRelationCoord	();


protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int 			OnBeginMove				( WPARAM, LPARAM );	// metalgeni [6/2/2006]
	virtual int 			OnEndMove				( WPARAM, LPARAM );	
	virtual int 			OnMove					( WPARAM, LPARAM );

	int						OnPurge					( WPARAM wParam , LPARAM lParam );
	int						OnRefreshQuest			( WPARAM wParam , LPARAM lParam );

	int						OnQuestNotify			( WPARAM wParam , LPARAM lParam );

	int						OnTreeBoxLButtonUp		( WPARAM wParam , LPARAM lParam );

	//	}}

	//////////////////////////////////////////////////////////////////////////

//	void				ReposOwnImage			();

	SPQuestAttr*			GetQuestAttr			( int iQuestID );

	void					ResetCategoryItem		();
	TREEBOX_ITEM*			CreateCategoryItem		( int iCategory );
	bool					DeleteCategoryItem		( int iCategory );

	TREEBOX_ITEM*			FindQuestItem			( int iQuestID , int iMissionID );
//	SPTreeBoxItemQuest*		FindQuestItem			( int iQuestID , int iMissionID );

	SPTreeBoxItemQuest*		CreateTreeBoxItem		( bool bMenuType );

	bool					GetMissionString		( std::string& strMission , int iQuestID , int iMissionID );
	void					UpdateQuest				( TREEBOX_ITEM* pItem );
	void					UpdateQuestColor		( SPTreeBoxItemQuest* pQuestItem , bool bComplete );


	//////////////////////////////////////////////////////////////////////////
	//
	//	선언
	//

	enum QUESTNOTIFY_INSTANCE_ID
	{
		WIID_QUESTNOTIFY_CLOSE						=	WIID_QUESTNOTIFY	+	1				,
		WIID_QUESTNOTIFY_TREE_LIST																,
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	//SPTexture*							m_pBGTexture;
	//RECT								m_rcBGDest[ QUESTVIEW_BG_COUNT ];
	//RECT								m_rcBGSrc[ QUESTVIEW_BG_COUNT ];

	typedef std::map< int , TREEBOX_ITEM* >	STD_MAP_TREEBOX_ITEM;

	STD_MAP_TREEBOX_ITEM				m_mpCategory;

	D3DXCOLOR							m_cStringColor[ 2 ];

	std::string							m_strMissionView;

};
