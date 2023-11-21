/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/04/07
	created:	2006:4:7:   15:40
	filename: 	SPWindowWorldMap.h
	
	purpose:	
*********************************************************************/

#pragma once

#define WORLDMAP_NEARMAPINFO	5

struct	SPWorldMapStageAttr
{
	int				m_iStage;
	std::string		m_strName;
	std::string		m_strMapFile;
	int				m_iMapWidth;
	int				m_iMapHeight;
	std::string		m_strDesc;
	int				m_iMapIcon;
	int				m_iMapIconIndex;
	std::string		m_strNearMapInfo[ WORLDMAP_NEARMAPINFO ];
};

enum WORLDMAP_MAP_EVENT_TYPE
{
	WORLDMAP_MAP_EVENT_PARTY		=	0	,
	WORLDMAP_MAP_EVENT_PORTAL				,
	WORLDMAP_MAP_EVENT_SAVE					,
	WORLDMAP_MAP_EVENT_NPC					,
	WORLDMAP_MAP_EVENT_MAILBOX				,
	WORLDMAP_MAP_EVENT_MAX					,
};

enum MAP_LINK;

struct	SPWorldMapMapAttr
{
	std::string		m_strName;
	int				m_iStage;
	int				m_iGroup;
	int				m_iPosX;
	int				m_iPosY;
	int				m_iWidth;
	int				m_iHeight;
	BOOL			m_bVisible[ WORLDMAP_MAP_EVENT_MAX ];
	BOOL			m_bAdjust[ WORLDMAP_MAP_EVENT_MAX ];
	MAP_LINK		m_eType;
	int				m_iRealMapWidth;
	int				m_iRealMapHeight;
	int				m_iRealMapCount;
};



typedef	std::map< int	, SPWorldMapStageAttr >		STD_MAP_WORLDMAP_STAGE_ATTR;
typedef	std::map< DWORD , SPWorldMapMapAttr >		STD_MAP_WORLDMAP_MAP_ATTR;

class SPWindow;

class SPWindowWorldMap : public SPWindow
{

public:
	SPWindowWorldMap(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowWorldMap(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowWorldMap();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);


protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int						OnWorldMap			( WPARAM wParam, LPARAM lParam );
	int						OnStageMap			( WPARAM wParam, LPARAM lParam );

	int						OnPurge				( WPARAM wParam, LPARAM lParam );
	int						OnRefresh			( WPARAM wParam, LPARAM lParam );
	int						OnKeyReturn			( WPARAM wParam, LPARAM lParam );

	int						OnClose				( WPARAM wParam, LPARAM lParam );

	int						OnCursorMove		( WPARAM wParam, LPARAM lParam );

	int						OnCursorMove		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int						OnCursorEnter		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int						OnCursorOut			( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int						OnStageLButtonDown	( unsigned int iID, WPARAM wParam , LPARAM lParam );

	// }}

	void					LoadLDTStage		();
	void					LoadLDTGroup		();

	void					InitKey				();
	void					ProcessKey			( float fTime );

	void					ResetLMouseData		();
	void					MoveScreenDistance	( int iXDistance , int iYDistance );
	void					LimitPosCheck		( int& iXPos , int& iYPos , int iXSize , int iYSize );

	void					CheckAutoScroll		( int iXPos , int iYPos );

	void					AddStageAttr		( SPWorldMapStageAttr&	stStageAttr );
	SPWorldMapStageAttr*	GetStageAttr		( int iStage );

	void					AddMapAttr			( SPWorldMapMapAttr&	stMapAttr );
	SPWorldMapMapAttr*		GetMapAttr			( int iStage , int iGroup );

	bool					LoadStage			( int iStage );
	void					CleanStage			();

	void					HideCtrl			();
	void					UpdateMeInfo		();
	void					UpdateTeamInfo		();

	void					CreateEventInfo		( bool bDel );

	void					SendRequestTeamInfo	( float fTime );

	void					InitWorldTootip		();

	void					ProcessEffect		( float fTime );

	void					OnClickWorldMap		();
	void					WorldMapComplete	();

	bool					MakeViewInfo		( RECT& rcOutStartPos , RECT& rcOutEndPos , int iStage , bool bZoom );

	void					AdjustPlayer		();
	void					AdjustStage			( int iStage );
	void					AdjustMapEvent		( int iDestStage , int iDestMapGroup , int iDestMapID , float fDestPosX , float fDestPosY );

	int						FindMapEventInfo	( int iDestStage , int iDestMapGroup , int iDestMapID , float fDestPosX , float fDestPosY );


	//////////////////////////////////////////////////////////////////////////

	enum WORLDMAP_ENUM
	{
		WORLDMAP_PARTY			=	4	,
		WORLDMAP_ICON_X			=	11	,
		WORLDMAP_ICON_Y			=	16	,
		WORLDMAP_DAMAGE_ICON_X	=	21	,
		WORLDMAP_DAMAGE_ICON_Y	=	16	,
	};

	enum WORLDMAP_EFFECT
	{
		WORLDMAP_EFFECT_NULL			=		0		,
		WORLDMAP_EFFECT_DAMAGE							,
		WORLDMAP_EFFECT_STAGE_CLICK						,
		WORLDMAP_EFFECT_WORLD_CLICK						,
	};

	enum WORLDMAP_INSTANCE_ID
	{
		WIID_WORLDMAP_WORLD							=	WIID_WORLDMAP					+	1				,
		WIID_WORLDMAP_STAGE																					,
		WIID_WORLDMAP_CTRL_START					=	WIID_WORLDMAP					+	0x100			,
			WIID_WORLDMAP_CTRL_MOUSE				=	WIID_WORLDMAP_CTRL_START							,
			WIID_WORLDMAP_CTRL_WORLD				=	WIID_WORLDMAP_CTRL_MOUSE							,
				WIID_WORLDMAP_CTRL_WORLD_DAMAGE																,
				WIID_WORLDMAP_CTRL_WORLD_ME																	,
				WIID_WORLDMAP_CTRL_WORLD_PARTY																,
				WIID_WORLDMAP_CTRL_WORLD_PARTY_END	=	WIID_WORLDMAP_CTRL_WORLD_PARTY	+	WORLDMAP_PARTY	,
				WIID_WORLDMAP_CTRL_WORLD_LOCATION															,
			WIID_WORLDMAP_CTRL_WORLD_END			=	WIID_WORLDMAP					+	0x199			,

			WIID_WORLDMAP_CTRL_STAGE				=	WIID_WORLDMAP					+	0x200			,
				WIID_WORLDMAP_CTRL_STAGE_DAMAGE																,
				WIID_WORLDMAP_CTRL_STAGE_ME																	,
				WIID_WORLDMAP_CTRL_STAGE_PARTY																,
				WIID_WORLDMAP_CTRL_STAGE_PARTY_END	=	WIID_WORLDMAP_CTRL_STAGE_PARTY	+	WORLDMAP_PARTY	,
				WIID_WORLDMAP_CTRL_STAGE_ETC																,
			WIID_WORLDMAP_CTRL_STAGE_END			=	WIID_WORLDMAP					+	0x799			,

			WIID_WORLDMAP_CTRL_NO_MOUSE				=	WIID_WORLDMAP					+	0x800			,
			WIID_WORLDMAP_CTRL_LEGEND																		,
			WIID_WORLDMAP_CTRL_BOTTOM_LINE_TOOLTIP															,

			WIID_WORLDMAP_CTRL_TOOLTIP																		,
			WIID_WORLDMAP_CTRL_TOOLTIP_END			=	WIID_WORLDMAP_CTRL_TOOLTIP		+	0x10			,

			WIID_WORLDMAP_CTRL_NO_MOVE_START																,
				WIID_WORLDMAP_CTRL_CLOSE																	,
				WIID_WORLDMAP_CTRL_WORLD_VIEW																,
				WIID_WORLDMAP_CTRL_STAGE_VIEW																,
			WIID_WORLDMAP_CTRL_NO_MOVE_END																	,

		WIID_WORLDMAP_CTRL_END						=	WIID_WORLDMAP					+	0x999			,

	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//

	POINT							m_ptLMousePos;

	int								m_iCreateStageID;

	INSTANCE_ID						m_CurMoveID;

	STD_MAP_WORLDMAP_STAGE_ATTR		m_mStageAttr;
	STD_MAP_WORLDMAP_MAP_ATTR		m_mMapAttr;

	float							m_fLimitTimeTeamInfo;
	float							m_fAccumulateTimeTeamInfo;

	std::vector<MAPEVENT_INFO*>		m_vMapEventInfo;

	WORLDMAP_EFFECT					m_eEffectType;
	D3DXCOLOR						m_cEffectColor;

	SPTexture*						m_pEffectWorldMap;
	RECT							m_rcEffectWorldMapDest;
	RECT							m_rcEffectWorldMapSrc;

	RECT							m_rcEffectWorldMapViewStart;
	RECT							m_rcEffectWorldMapViewEnd;

	SPTexture*						m_pEffectWorldMapDamage;
	RECT							m_rcEffectWorldMapDamageDest;
	RECT							m_rcEffectWorldMapDamageSrc;

	SPTexture*						m_pEffectWayPoint;
	POINT							m_ptEffectWayPointDest;
	RECT							m_rcEffectWayPointSrc;
	float							m_fEffectWayPointScaleX;
	float							m_fEffectWayPointScaleY;
	D3DXCOLOR						m_cEffectWayPointColor;
	INSTANCE_ID						m_eWayPointInstanceID;


};
