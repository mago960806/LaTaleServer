/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/06/23
	created:	2006:6:23:   16:07
	filename: 	SPWindowQuestView.h
	
	purpose:	
*********************************************************************/

#pragma once

#define QUESTVIEW_BG_COUNT						3
#define QUESTVIEW_SUB_BG_COUNT					5

class SPWindow;
class SPUIUnitManager;

class SPWindowQuestView : public SPWindow 
{
public:

	SPWindowQuestView(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowQuestView(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowQuestView();

	virtual	void		Init					();
	virtual	void		Clean					();
	virtual void		Show					( bool bWithChild = true );
	virtual void		Hide					( bool bSendServer = true );

	virtual	void		Process					( float fTime );
	virtual	void		Render					( float fTime );
	virtual void		RefreshRelationCoord	();


protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int 		OnBeginMove				( WPARAM, LPARAM );	// metalgeni [6/2/2006]
	virtual int 		OnEndMove				( WPARAM, LPARAM );	
	virtual int 		OnMove					( WPARAM, LPARAM );

	int					OnPurge					( WPARAM wParam, LPARAM lParam );
	int					OnRefreshQuest			( WPARAM wParam, LPARAM lParam );

	int					OnQuestDetail			( WPARAM wParam, LPARAM lParam );
	int					OnQuestGetReward		( WPARAM wParam, LPARAM lParam );

	int					OnCloseLButtonUp		( WPARAM wParam, LPARAM lParam );
	int					OnTalkLButtonUp			( WPARAM wParam, LPARAM lParam );
	int					OnSummaryLButtonUp		( WPARAM wParam, LPARAM lParam );
	
	int					OnRewardLeftLButtonUp	( WPARAM wParam, LPARAM lParam );
	int					OnRewardRightLButtonUp	( WPARAM wParam, LPARAM lParam );

	int					OnRankLButtonUp			( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int					OnCursorEnter			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int					OnCursorOut				( unsigned int iID, WPARAM wParam , LPARAM lParam );

	//	}}

	//////////////////////////////////////////////////////////////////////////

	void				ReposOwnImage			();

	void				InitItemUnit			();
	void				InitBrace				( SPWindow* pWindow );
	void				InitPurpose				( SPWindow* pWindow );
	void				InitReward				( SPWindow* pWindow );

	SPQuestAttr*		GetQuestAttr			( int iQuestID , INSTANCE_ID eInstanceID );

	void				UpdatePosWindow			();
	void				UpdatePosItemUnit		( int iXPos , int iYPos , bool bShow );

	void				UpdateMission			();
	void				UpdateReward			();
	void				UpdateRewardPage		();

	void				UpdateTalk				();
	void				UpdateSummary			();
	void				UpdateRank				();
	void				UpdateRankCount			();

	void				DefaultRewardRank		();


	//////////////////////////////////////////////////////////////////////////
	//
	//	선언
	//

	enum QUESTVIEW_INSTANCE_ID
	{
		WIID_QUESTVIEW_CLOSE						=	WIID_QUESTVIEW	+	1					,
		WIID_QUESTVIEW_TITLE_TEXT																,
		WIID_QUESTVIEW_NPC_TEXT																	,
		WIID_QUESTVIEW_NPC_ICON																	,
		WIID_QUESTVIEW_TAB_TALK																	,
		WIID_QUESTVIEW_TAB_SUMMARY																,

		WIID_QUESTVIEW_TALK																		,

		WIID_QUESTVIEW_BRACE																	,
		WIID_QUESTVIEW_BRACE_LEFT_TOP															,
		WIID_QUESTVIEW_BRACE_LEFT_MIDDLE														,
		WIID_QUESTVIEW_BRACE_LEFT_BOTTOM														,
		WIID_QUESTVIEW_BRACE_RIGHT_TOP															,
		WIID_QUESTVIEW_BRACE_RIGHT_MIDDLE														,
		WIID_QUESTVIEW_BRACE_RIGHT_BOTTOM														,

		WIID_QUESTVIEW_PURPOSE_TITLE_IMG														,
		WIID_QUESTVIEW_PURPOSE_TITLE_TEXT														,

		WIID_QUESTVIEW_PURPOSE_LINE_1_IMG														,
		WIID_QUESTVIEW_PURPOSE_LINE_2_IMG														,
		WIID_QUESTVIEW_PURPOSE_LINE_3_IMG														,
		WIID_QUESTVIEW_PURPOSE_LINE_4_IMG														,
		WIID_QUESTVIEW_PURPOSE_LINE_1_TEXT														,
		WIID_QUESTVIEW_PURPOSE_LINE_2_TEXT														,
		WIID_QUESTVIEW_PURPOSE_LINE_3_TEXT														,
		WIID_QUESTVIEW_PURPOSE_LINE_4_TEXT														,

		WIID_QUESTVIEW_REWARD																	,

		WIID_QUESTVIEW_REWARD_TITLE_IMG															,
		WIID_QUESTVIEW_REWARD_TITLE_TEXT														,
		WIID_QUESTVIEW_REWARD_PAGE_IMG															,
		WIID_QUESTVIEW_REWARD_PAGE_TEXT															,

		WIID_QUESTVIEW_REWARD_RANK_A															,
		WIID_QUESTVIEW_REWARD_RANK_B															,
		WIID_QUESTVIEW_REWARD_RANK_C															,

		WIID_QUESTVIEW_REWARD_LEFT																,
		WIID_QUESTVIEW_REWARD_RIGHT																,

		WIID_QUESTVIEW_REWARD_XP_ELY_FAME_IMG													,
		WIID_QUESTVIEW_REWARD_XP_TEXT															,
		WIID_QUESTVIEW_REWARD_ELY_TEXT															,
		WIID_QUESTVIEW_REWARD_FAME_TEXT															,

		WIID_QUESTVIEW_REWARD_GUIDL_POINT_IMG													,
		WIID_QUESTVIEW_REWARD_GUIDL_POINT_TEXT													,

		WIID_QUESTVIEW_REWARD_ITEM_ICON															,
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	SPTexture*							m_pBGTexture;
	RECT								m_rcBGDest[ QUESTVIEW_BG_COUNT ];
	RECT								m_rcBGSrc[ QUESTVIEW_BG_COUNT ];

	SPTexture*							m_pSubBGTexture;
	RECT								m_rcSubBGDest[ QUESTVIEW_SUB_BG_COUNT ];
	RECT								m_rcSubBGSrc[ QUESTVIEW_SUB_BG_COUNT ];

	std::string							m_strQuestName;
	D3DXCOLOR							m_cQuestNameColor;
	D3DXCOLOR							m_cQuestNameShadowColor;

	int									m_iPlayerPosX;
	int									m_iPlayerPosY;

	SPUIUnitManager						m_UIUnitManager;

	D3DXCOLOR							m_cPurposeColor[2];

	INSTANCE_ID							m_eQuestInstanceID;
	int									m_iQuestID;
	int									m_iRewardRank;
	int									m_iRewardInfo;
	int									m_iRewardInfoMax;
	int									m_iRankCount;

	std::string							m_strMissionView;

};
