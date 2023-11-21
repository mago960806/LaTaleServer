// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-9-23    14:56 
//***************************************************************************

#pragma once

#define QUEST_BG_COUNT						3
#define QUEST_SUB_BG_COUNT					3

struct TREEBOX_ITEM;

class SPWindow;
class SPTreeBoxItemQuest;

class SPWindowQuest : public SPWindow 
{
public:

	SPWindowQuest(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowQuest(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowQuest();

	virtual	void		Init					();
	virtual	void		Clean					();
	virtual void		Show					( bool bWithChild = true );
	virtual void		Hide					( bool bSendServer = true );

	virtual	void		Process					( float fTime );
	virtual	void		Render					( float fTime );
	virtual void		RefreshRelationCoord	();

//	void				SetCutInPos				();


protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int 		OnBeginMove				( WPARAM, LPARAM );	// metalgeni [6/2/2006]
	virtual int 		OnEndMove				( WPARAM, LPARAM );	
	virtual int 		OnMove					( WPARAM, LPARAM );

	int					OnPurge					( WPARAM wParam, LPARAM lParam );
	int					OnRefreshQuest			( WPARAM wParam, LPARAM lParam );

	int					OnQuestAdd				( WPARAM wParam, LPARAM lParam );
	int					OnQuestDel				( WPARAM wParam, LPARAM lParam );

	int					OnQuestNotify			( WPARAM wParam, LPARAM lParam );

	int					OnCloseLButtonUp		( WPARAM wParam, LPARAM lParam );
	int					OnTreeBoxLButtonUp		( WPARAM wParam, LPARAM lParam );
	int					OnAcceptLButtonUp		( WPARAM wParam, LPARAM lParam );
	int					OnCompleteLButtonUp		( WPARAM wParam, LPARAM lParam );
	int					OnGiveUpLButtonUp		( WPARAM wParam, LPARAM lParam );
	int					OnShareLButtonUp		( WPARAM wParam, LPARAM lParam );

	//	}}

	//////////////////////////////////////////////////////////////////////////

	void				ReposOwnImage			();

	void				ResetCategoryItem		();
	TREEBOX_ITEM*		CreateCategoryItem		( int iCategory );
	bool				DeleteCategoryItem		( int iCategory );

	TREEBOX_ITEM*		FindQuestItem			( int iQuestID , int iCategory );

	void				SetActionButton			( int iAction , int iShare );
	int					GetSelectItem			();

	SPTreeBoxItemQuest*	CreateTreeBoxItem		( bool bMenuType );
	SPTreeBoxItemQuest*	CreateTreeBoxItem		( int iQuestID );

	void				ShowFirstQuest			( bool bOverWrite );
	int					FindFirstQuestID		( SPWindow* pTreeWindow , TREEBOX_ITEM* pStartItem );



	//////////////////////////////////////////////////////////////////////////
	//
	//	선언
	//

	enum QUEST_INSTANCE_ID
	{
		WIID_QUEST_CLOSE						=	WIID_QUEST	+	1						,
		WIID_QUEST_TITLE_IMAGE																,
		WIID_QUEST_TITLE_TEXT																,
		WIID_QUEST_SUB_CATEGORY																,
		WIID_QUEST_ACCEPT																	,
		WIID_QUEST_COMPLETE																	,
		WIID_QUEST_GIVEUP																	,
		WIID_QUEST_SHARE																	,

		WIID_QUEST_TREE_LIST																,		//	내부적으로 Up , Down , Slider 버튼 내장 번호 여유 줄것
	};

	enum GLOBAL_STRING_QUEST
	{
		GS_CATEGORY_CITYNAME_START				=	6100000									,		//	도시 이름 정보
		GC_CATEGORY_QUESTTYPE_START				=	6200000									,		//	퀘스트 진행 타입 정보
		GC_SUB_CATEGORY_PC						=	6300001									,		//	퀘스트창 이름 ( PC )
		GC_SUB_CATEGORY_NPC						=	6300002									,		//	퀘스트창 이름 ( NPC )
		GC_SUB_CATEGORY_SHARE					=	6400001									,		//	췌스트창 이름 ( 공유 )
	};


	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	SPTexture*							m_pBGTexture;
	RECT								m_rcBGDest[ QUEST_BG_COUNT ];
	RECT								m_rcBGSrc[ QUEST_BG_COUNT ];

	SPTexture*							m_pSubBGTexture;
	RECT								m_rcSubBGDest[ QUEST_SUB_BG_COUNT ];
	RECT								m_rcSubBGSrc[ QUEST_SUB_BG_COUNT ];

	typedef std::map< int , TREEBOX_ITEM* >	STD_MAP_TREEBOX_ITEM;

	STD_MAP_TREEBOX_ITEM				m_mpCategory;

	D3DXCOLOR							m_cStringColor;

	bool								m_bClickNotify;

	int									m_iOldAX;
	int									m_iOldAY;

};
