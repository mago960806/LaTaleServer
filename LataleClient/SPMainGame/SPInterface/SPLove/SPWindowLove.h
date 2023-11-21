// ***************************************************************
//  SPWindowLove		Version:  1.0		Date: 2008/09/25
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPWindowLove_h__
#define SPWindowLove_h__

#pragma once

#define	WIID_LOVE_BG_LT1							0x31003500
#define	WIID_LOVE_BG_MT1							0x31003501
#define	WIID_LOVE_BG_RT1							0x31003502
#define	WIID_LOVE_BG_LM1							0x31003503
#define	WIID_LOVE_BG_MM1							0x31003504
#define	WIID_LOVE_EXPANSION_BG1						0x31003400
#define	WIID_LOVE_EXPANSION_BTN1					0x31003401
#define	WIID_LOVE_BTN_EXIT1							0x31003402
#define	WIID_LOVE_BG_3RD_LAYER_BOARD_TOP1			0x31003403
#define	WIID_LOVE_BG_3RD_LAYER_BOARD_BOTTOM1		0x31003404
#define	WIID_LOVE_BG_RM1							0x31003505
#define	WIID_LOVE_BG_LB1							0x31003506
#define	WIID_LOVE_BG_MB1							0x31003507
#define	WIID_LOVE_BG_RB1							0x31003508
#define	WIID_LOVE_BG_LT2							0x31003509
#define	WIID_LOVE_BG_MT2							0x3100350a
#define	WIID_LOVE_BG_RT2							0x3100350b
#define	WIID_LOVE_BG_LM2							0x3100350c
#define	WIID_LOVE_BG_MM2							0x3100350d
#define	WIID_LOVE_CAPTION_ICON						0x31003405
#define	WIID_LOVE_CAPTION_INFO						0x31003406
#define	WIID_LOVE_CLOSE								0x31003407
#define	WIID_LOVE_BG_3RD_LAYER_BOARD_TOP2			0x31003408
#define	WIID_LOVE_BG_3RD_LAYER						0x31003409
#define	WIID_LOVE_NAME_BG_L							0x31003300
#define	WIID_LOVE_NAME_BG_M							0x31003301
#define	WIID_LOVE_CAPTION_LOVER						0x31003200
#define	WIID_LOVE_NAME_BG_3RD_LAYER_L				0x31003201
#define	WIID_LOVE_NAME_BG_3RD_LAYER_M				0x31003202
#define	WIID_LOVE_NAME								0x31003100
#define	WIID_LOVE_NAME_BG_3RD_LAYER_R				0x31003203
#define	WIID_LOVE_NAME_BG_R							0x31003302
#define	WIID_LOVE_CAPTION_LOVEINFO2					0x31003303
#define	WIID_LOVE_LOVEINFO_BG_L						0x31003304
#define	WIID_LOVE_LOVEINFO_BG_M						0x31003305
#define	WIID_LOVE_CAPTION_ID						0x31003204
#define	WIID_LOVE_GAUGE								0x31003205
#define	WIID_LOVE_ITEMSLOT							0x31003206
#define	WIID_LOVE_STRING_BG_L						0x31003207
#define	WIID_LOVE_STRING_BG_M						0x31003208
#define	WIID_LOVE_STRING							0x31003101
#define	WIID_LOVE_STRING_BG_R						0x31003209
#define	WIID_LOVE_ELY_BG							0x3100320a
#define	WIID_LOVE_ELY								0x31003102
#define	WIID_LOVE_BTN_PROPOSE1						0x3100320b
#define	WIID_LOVE_BTN_PROPOSE2						0x3100320c
#define	WIID_LOVE_MAP_BG							0x3100320d
#define	WIID_LOVE_MAP								0x31003103
#define	WIID_LOVE_MAP_QUESTIONMARK					0x31003104
#define	WIID_LOVE_MAP_ARROW							0x3100320e
#define	WIID_LOVE_MAPSTRING_BG_LT					0x3100320f
#define	WIID_LOVE_MAPSTRING_BG_MT					0x31003210
#define	WIID_LOVE_MAPSTRING_BG_RT					0x31003211
#define	WIID_LOVE_MAPSTRING_BG_LM					0x31003212
#define	WIID_LOVE_MAPSTRING_BG_MM					0x31003213
#define	WIID_LOVE_MAPSTRING							0x31003105
#define	WIID_LOVE_CAPTION_CONNECTION				0x3100310a
#define	WIID_LOVE_CAPTION_LOCALITY					0x3100310b
#define	WIID_LOVE_MAPSTRING2						0x3100310c
#define	WIID_LOVE_MAPSTRING_BG_RM					0x31003214
#define	WIID_LOVE_MAPSTRING_BG_LB					0x31003215
#define	WIID_LOVE_MAPSTRING_BG_MB					0x31003216
#define	WIID_LOVE_MAPSTRING_BG_RB					0x31003217
#define	WIID_LOVE_LOVEINFO_BG_R						0x31003306
#define	WIID_LOVE_CAPTION_LOVEINFO1					0x31003307
#define	WIID_LOVE_EXPANSION_BG2						0x3100340a
#define	WIID_LOVE_EXPANSION_BTN2					0x3100340b
#define	WIID_LOVE_LOVEINFO_BG						0x3100340c
#define	WIID_LOVE_LOVEINFO_BG_LT					0x31003308
#define	WIID_LOVE_LOVEINFO_BG_MT					0x31003309
#define	WIID_LOVE_LOVEINFO_BG_RT					0x3100330a
#define	WIID_LOVE_LOVEINFO_BG_LM					0x3100330b
#define	WIID_LOVE_LOVEINFO_BG_MM					0x3100330c
#define	WIID_LOVE_CAPTION_LOVEINFO3					0x31003218
#define	WIID_LOVE_DAY_BG_L							0x31003219
#define	WIID_LOVE_DAY_BG_M							0x3100321a
#define	WIID_LOVE_DAY_BG_R							0x3100321b
#define	WIID_LOVE_CAPTION_DAY1						0x3100321c
#define	WIID_LOVE_CAPTION_DAY2						0x3100321d
#define	WIID_LOVE_DAY_BG_3RD_LAYER_L				0x3100321e
#define	WIID_LOVE_DAY_BG_3RD_LAYER_M				0x3100321f
#define	WIID_LOVE_DAY								0x31003106
#define	WIID_LOVE_DAY_BG_3RD_LAYER_R				0x31003220
#define	WIID_LOVE_STATUS_BG_L						0x31003221
#define	WIID_LOVE_STATUS_BG_M						0x31003222
#define	WIID_LOVE_STATUS_BG_R						0x31003223
#define	WIID_LOVE_CAPTION_STATUS					0x31003224
#define	WIID_LOVE_STATUS_BG_3RD_LAYER_L				0x31003225
#define	WIID_LOVE_STATUS_BG_3RD_LAYER_M				0x31003226
#define	WIID_LOVE_STATUS							0x31003107
#define	WIID_LOVE_STATUS_BG_3RD_LAYER_R				0x31003227
#define	WIID_LOVE_POINT_BG_L						0x31003228
#define	WIID_LOVE_POINT_BG_M						0x31003229
#define	WIID_LOVE_POINT_BG_R						0x3100322a
#define	WIID_LOVE_CATION_POINT						0x3100322b
#define	WIID_LOVE_POINT_BG_3RD_LAYER_L				0x3100322c
#define	WIID_LOVE_POINT_BG_3RD_LAYER_M				0x3100322d
#define	WIID_LOVE_POINT1							0x31003108
#define	WIID_LOVE_POINT_BG_3RD_LAYER_R				0x3100322e
#define	WIID_LOVE_CATION_SLASH						0x3100322f
#define	WIID_LOVE_POINT_BG_3RD_LAYER_L2				0x31003230
#define	WIID_LOVE_POINT_BG_3RD_LAYER_M2				0x31003231
#define	WIID_LOVE_POINT2							0x31003109
#define	WIID_LOVE_POINT_BG_3RD_LAYER_R2				0x31003232
#define	WIID_LOVE_LOVEINFO_BG_RM					0x3100330d
#define	WIID_LOVE_LOVEINFO_BG_LB					0x3100330e
#define	WIID_LOVE_LOVEINFO_BG_MB					0x3100330f
#define	WIID_LOVE_LOVEINFO_BG_RB					0x31003310
#define	WIID_LOVE_BTN_EXIT2							0x3100340d
#define	WIID_LOVE_BG_3RD_LAYER_BOARD_BOTTOM2		0x3100340e
#define	WIID_LOVE_BG_RM2							0x3100350e
#define	WIID_LOVE_BG_LB2							0x3100350f
#define	WIID_LOVE_BG_MB2							0x31003510
#define	WIID_LOVE_BG_RB2							0x31003511
#define WIID_LOVE_ITEM								0x3100310d

#define WIID_LOVE_TARGET_EDIT						0x31003F00
#define WIID_LOVE_GAGE_PERCENTAGE					0x31003F01

enum LOVE_NOTICE_TYPE
{
	LOVE_NOTICE_NULL			=	0	,
	LOVE_NOTICE_FRIENDSHIP				,
	LOVE_NOTICE_PROPOSE					,
	LOVE_NOTICE_WEDDING					,
};

enum LOVE_WINDOW_STATE
{
	LOVE_WINDOW_NULL				=	0x0000	,
	LOVE_WINDOW_FRIENDSHIP			=	0x0001	,
	LOVE_WINDOW_PROPOSE_BEFORE		=	0x0002	,
	LOVE_WINDOW_PROPOSE_AFTER		=	0x0004	,
	LOVE_WINDOW_WEDDING				=	0x0008	,
	LOVE_WINDOW_LOVE_INFO			=	0x1000	,
};

#define LOVE_ITEM_UNIT_COUNT								1
#define LOVE_ITEM_UNIT_POS									0

class SPUIUnit;
class SPWindow;

class SPWindowLove : public SPWindow
{
public:
	SPWindowLove											( WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowLove											( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowLove									();

	virtual void			Init							();
	virtual void			Clean							();

	virtual void			Process							( float fTime );
	virtual void			Render							( float fTime );

	virtual void			Show							( bool bWithChild = true );
	virtual void			Hide							( bool bSendServer = true );

	virtual void			RefreshRelationCoord			();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	//	Common
	virtual		int			OnKeyReturn						( WPARAM wParam, LPARAM lParam );

	virtual		int			OnNoticeYes						( WPARAM wParam, LPARAM lParam );
	virtual		int			OnNoticeNo						( WPARAM wParam, LPARAM lParam );

	virtual		int			OnRefresh						( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPurge							( WPARAM wParam, LPARAM lParam );

	virtual		int			OnLoveInfo						( WPARAM wParam, LPARAM lParam );
//	virtual		int			OnLoveRequest					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnLoveRequestRecv				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnLoveResponse					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnLoveUpdateMember				( WPARAM wParam, LPARAM lParam );
//	virtual		int			OnLoveParing					( WPARAM wParam, LPARAM lParam );
//	virtual		int			OnLoveCharInfo					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnLoveRefreshButton				( WPARAM wParam, LPARAM lParam );

	virtual		int			OnEditSetFocus					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnEditKillFocus					( WPARAM wParam, LPARAM lParam );

	virtual		int			OnClose							( WPARAM wParam, LPARAM lParam );
	virtual		int			OnExpansion1					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnExpansion2					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPropose1						( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPropose2						( WPARAM wParam, LPARAM lParam );

	// }}

	void					InitUIItemUnit					();
	void					UpdatePosUIItemUnit				();

	void					SetCtrlWindowShow				( SPWindow* pWindow , bool bShow );
	bool					GetCtrlWindowShow				( SPWindow* pWindow );
	void					SetCtrlWindowEnable				( SPWindow* pWindow , bool bEnable );
	bool					GetCtrlWindowEnable				( SPWindow* pWindow );
	void					SetCtrlWindowText				( SPWindow* pWindow , int iValue );
	void					SetCtrlWindowText				( SPWindow* pWindow , const char* pstrText );
	void					SetCtrlWindowText				( SPWindow* pWindow , int iGlobalStringID , const char* pstrAddSting );
	void					SetCtrlWindowText				( SPWindow* pWindow , int iGlobalStringID , int iAddNumber );
	void					SetCtrlGageValue				( SPWindow* pWindowGage , SPWindow* pWindowNumber , int iMin , int iMax );
	void					SetCtrlNotice					( LOVE_NOTICE_TYPE eType , int iDealyTime , int iGlobalStringID , const char* pstrAddSting );
	void					SetCtrlItemBox					( int iItemID );
	void					SetCtrlItemBoxEnable			( bool bEnable );
	bool					GetCtrlItemBoxEnable			();

	void					SetError						( int iGloalStringID , const char* pszAddString1 = NULL , const char* pszAddString2 = NULL );
	void					SetError						( int iGloalStringID , int iValue );
	void					SetError						( const char* pszErrorString );

	void					ProcessMemberUpdate				( float fTime );
	void					ProcessStatusUpdate				( float fTime );
	void					ProcessButtonUpdate				( float fTime );

	void					SendResponse					( UINT8 uiWeddingType , UINT8 uiAccept );
	void					ClearResponse					();

	void					SetWindowState					( LOVE_WINDOW_STATE eLoveWindowState , bool bShowAll );

	void					SetPartnerInfo					( int iStageID , int iMapGroupID , int iChannel );
	void					SetMinimapIcon					( SPWindow* pWindow , int iResID , int iResIndex );

	void					FindConsumeItem					( int iItemID );
	bool					IsConsumeItem					();

	void					SetInvenItemLock				( SPItem* pkItem , int iIndex );
	void					SetInvenItemUnlock				( int iIndex );
	void					SetInvenItemAllUnlock			();

	void					SetPlayerStatus					();
	void					UpdateWIndowAll					( bool bWindowShow );
	void					UpdateRequireCondition			( bool bWindowShow );


	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	bool													m_bUse;

	SPUIUnitManager											m_UIUnitManager;

	LOVE_NOTICE_TYPE										m_eNoticeType;
	char													m_szActCharName[ LEN_NAME + 1 ];

	float													m_fAccumulateMemberUpdate;
	float													m_fLimitMemberUpdate;

	LOVE_WINDOW_STATE										m_eLoveWindowState;

	SPItem*													m_pConsumeItem;

	float													m_fAccumulateStatusUpdate;
	float													m_fLimitStatusUpdate;
	bool													m_bStatusUpdate;

	float													m_fAccumulateButtonUpdate;
	float													m_fLimitButtonUpdate;
	bool													m_bButtonUpdate;

	int														m_iStageID;
	int														m_iMapGroupID;
	int														m_iChannelID;

	//	임시변수

	char													m_szBuf[ _MAX_PATH ];

};

#endif // SPWindowLove_h__