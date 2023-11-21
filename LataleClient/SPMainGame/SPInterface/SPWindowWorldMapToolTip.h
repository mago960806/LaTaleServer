/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/04/13
	created:	2006:4:13:   17:07
	filename: 	SPWindowWorldMapToolTip.h
	
	purpose:	
*********************************************************************/

#pragma once

#define TOOLTIP_BG_COUNT	9

struct	SPWorldMapStageAttr;

struct	SPStageToolTipData
{
	UINT			m_uiClassType;
	int				m_iResID;
	int				m_iResIndex;
	std::string		m_strName;
	std::string		m_strPos;
	std::string		m_strDesc;
};

class SPWindow;

class SPWindowWorldMapToolTip : public SPWindow
{

public:
	enum WMTOOLTIP_SHOW_TYPE
	{
		TOOLTIP_SHOW_TYPE_NULL		=	0	,
		TOOLTIP_SHOW_TYPE_WORLD				,
		TOOLTIP_SHOW_TYPE_STAGE				,
	};

	enum WMTOOLTIP_TEXTURE
	{
		WMTOOLTIP_TEXTURE_LEFT_TOP				=	0			,
		WMTOOLTIP_TEXTURE_CENTER_TOP							,
		WMTOOLTIP_TEXTURE_RIGHT_TOP								,
		WMTOOLTIP_TEXTURE_LEFT_MIDDLE							,
		WMTOOLTIP_TEXTURE_CENTER_MIDDLE							,
		WMTOOLTIP_TEXTURE_RIGHT_MIDDLE							,
		WMTOOLTIP_TEXTURE_LEFT_BOTTOM							,
		WMTOOLTIP_TEXTURE_CENTER_BOTTOM							,
		WMTOOLTIP_TEXTURE_RIGHT_BOTTOM							,
		WMTOOLTIP_TEXTURE_COUNT									,
	};


	SPWindowWorldMapToolTip(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowWorldMapToolTip();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);
    
	virtual int  PerformMouseMove( int iX, int iY )	{	return 0;	}

	SPWindow*	Find( INSTANCE_ID iID, bool bChild = false );

	void		SetBGTexture( const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY , int iStep );
	void		SetBGTexture( WMTOOLTIP_TEXTURE eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY );


protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnToolTipEnter	( WPARAM, LPARAM );
	virtual int OnToolTipOut	( WPARAM, LPARAM );
	virtual int OnToolTipMove	( WPARAM, LPARAM );
	 // }}

protected:

	void	UpdatePos				();
	void	UpdateBGPos				();
	void	CalEndPos				();


	void	OnToolTipShow			( WPARAM wParam , LPARAM lParam );

	void	OnWorldShow				( SPWorldMapStageAttr* pStageAttr );
	void	CheckQuestNPC			( int iStage );

	void	OnStageShow				( SPStageToolTipData* pStageData );


	void	SetIcon					( int iResID , int iResIndex );
	void	SetWindowMultiText		( INSTANCE_ID WindowID , const char* pstrString );




	enum WMTOOLTIP_INSTANCE_ID
	{
		WIID_WORLDMAPTOOLTIP_ICON_TITLE_OUTLINE			=		1	,
		WIID_WORLDMAPTOOLTIP_ICON									,
		WIID_WORLDMAPTOOLTIP_NAME_LINE								,
		WIID_WORLDMAPTOOLTIP_NAME									,
		WIID_WORLDMAPTOOLTIP_TITLE									,
		WIID_WORLDMAPTOOLTIP_DESC									,
		WIID_WORLDMAPTOOLTIP_CAL_START								,
		WIID_WORLDMAPTOOLTIP_QUEST_NPC_TITLE						,
		WIID_WORLDMAPTOOLTIP_QUEST_NPC_VALUE						,
		WIID_WORLDMAPTOOLTIP_NEAR_PLACE_TITLE						,
		WIID_WORLDMAPTOOLTIP_NEAR_PLACE_VALUE						,
		WIID_WORLDMAPTOOLTIP_CAL_END								,
	};

	struct WM_TEXTURE_INFO
	{
		SPTexture*							m_pTexture;
		RECT								m_rcPos;
		RECT								m_rcSrc;
		POINT								m_ptSize;
	};

	typedef	std::vector< WM_TEXTURE_INFO >				STD_VECTOR_WM_TEXTURE;

	STD_VECTOR_WM_TEXTURE					m_vBGTexture;
	
	std::string								m_strName14;
	D3DXCOLOR								m_color14;

};
