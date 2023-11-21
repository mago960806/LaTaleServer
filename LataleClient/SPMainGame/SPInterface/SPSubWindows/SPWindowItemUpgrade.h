/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/05/19
	created:	2006:5:19:   12:46
	filename: 	SPWindowItemUpgrade.h
	
	purpose:	
*********************************************************************/

#pragma once

enum ITEM_UPGRADE_STATE
{
	ITEM_UPGRADE_STATE_NULL					=	0	,
	ITEM_UPGRADE_STATE_CANCEL						,
	ITEM_UPGRADE_STATE_READY_ITEM					,
	ITEM_UPGRADE_STATE_READY_MATERIAL				,
	ITEM_UPGRADE_STATE_READY_UPGRADE				,
	ITEM_UPGRADE_STATE_UPGRADING					,
	ITEM_UPGRADE_STATE_BRIGHTTING					,
	ITEM_UPGRADE_STATE_UPGRADING_WAIT				,
	ITEM_UPGRADE_STATE_END							,
};

class SPItem;
class SPUIUnit;
class SPUIUnitManager;
class SPWindow;

class SPWindowItemUpgrade : public SPWindow
{

public:
	SPWindowItemUpgrade(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowItemUpgrade(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowItemUpgrade();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

	virtual void RefreshRelationCoord();

protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()


	int			OnClose					( WPARAM wParam, LPARAM lParam );
	int			OnItemBox				( WPARAM wParam, LPARAM lParam );
	int			OnItemBoxCancel			( WPARAM wParam, LPARAM lParam );
	int			OnRun					( WPARAM wParam, LPARAM lParam );
	int			OnCancel				( WPARAM wParam, LPARAM lParam );
	int			OnResult				( WPARAM wParam, LPARAM lParam );

	int			OnItemCursorEnter		( WPARAM wParam, LPARAM lParam );
	int			OnItemCursorOut			( WPARAM wParam, LPARAM lParam );
	int			OnResultCursorEnter		( WPARAM wParam, LPARAM lParam );
	int			OnResultCursorOut		( WPARAM wParam, LPARAM lParam );

	int			OnMaterial				( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int			OnMaterialCancel		( unsigned int iID, WPARAM wParam , LPARAM lParam );

	int			OnMaterialCursorEnter	( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int			OnMaterialCursorOut		( unsigned int iID, WPARAM wParam , LPARAM lParam );

	// }}

	void		InitContentsWindow		( SPWindow* pParent , int iAdjustX , int iAdjustY );
	void		InitBalloonWindow		( SPWindow* pParent , int iAdjustX , int iAdjustY );
	void		InitProgressWindow		( SPWindow* pParent , int iAdjustX , int iAdjustY );
	void		InitUIItemUnit			();

	void		UpdatePosUIItemUnit		();

	void		SetSuccessProbability	( SPItem* pItem );
	void		SetSuccessProbability	( int iPercentage );
	int			GetSuccessProbability	( SPItem* pItem );

	void		SetProgressMaxTime		( float fMaxTime );

	void		ProcessItemToolTip		( float fTime );
	void		ProcessMaterialToolTip	( float fTime );
	void		ProcessProgress			( float fTime );
	void		ProcessBright			( float fTime );

	void		DefaultHideWindow		();

	void		ShowProgressWindow		();
	void		NextState				();

	void		SetUpgradeItem			( SPItem* pUpgradeItem );
	void		SetMaterialItem			( SPItem* pMaterialItem );


	void		SendServerRun			();

	void		ResetMaterialData		();
	void		ResetMaterialData		( int iMaterialPos );

	bool		CheckMaterialCondition	();
	bool		CheckMaterialCondition	( int& iOutMaterialCount , int& iOutMaterialPos , SPItem* pUpgradeItem , SPItem* pMaterialItem , bool bMultiCheck );


	//////////////////////////////////////////////////////////////////////////
	
	void		SetError				( int iGloalStringID , const char* pszItemString = NULL );
	void		SetError				( const char* pszErrorString );
	SPItem*		GetInvenPickUpItem		();
	void		SetInvenItemLock		( SPItem* pkItem , int iIndex );
	void		SetInvenItemUnlock		( SPItem* pkItem , int iIndex );
	void		SetInvenItemAllUnlock	();
	void		SendParentAction		( UINT64 uiAction , unsigned int uiIndex = 0 , bool bSendInit = false );

	//////////////////////////////////////////////////////////////////////////

	enum GLOBAL_STRING_ITEM_UPGRADE
	{
		GS_ITEM_UPGRADE_NULL							=	0												,
		GS_ITEM_UPGRADE_NOT_UPGRADE						=	11000011										,
		GS_ITEM_UPGRADE_ACTION_STOP						=	11000015										,
		GS_ITEM_UPGRADE_OBSTACLE_CANCEL					=	11000016										,
		GS_ITEM_UPGRADE_ONLY_ITEM_WINDOW				=	10000031										,
		GS_ITEM_UPGRADE_MATERIAL_DIFFERENT				=	11000012										,
		GS_ITEM_UPGRADE_MATERIAL_COUNT					=	11000013										,
		GS_ITEM_UPGRADE_MATERIAL_RARE					=	11000014										,
		GS_ITEM_UPGRADE_IS_MATERIAL						=	11000017										,
		GS_ITEM_UPGRADE_ITEM_TYPE_DIFFERENT				=	11000010										,
		GS_ITEM_UPGRADE_ITEM_EXPIRE						=	10000040										,


	};


	enum ITEM_UPGRADE_INSTANCE_ID
	{
		WIID_ITEM_UPGRADE_MATERIAL_BOX_COUNT			=	4												,
		WIID_ITEM_UPGRADE_PROGRESS_NUMBER_COUNT			=	3												,

		WIID_ITEM_UPGRADE_UIUNIT_POS_ITEM				=	4												,
		WIID_ITEM_UPGRADE_UIUNIT_POS_RESULT				=	5												,

		WIID_ITEM_UPGRADE_BG_TOP						=	WIID_ITEM_UPGRADE					+	1		,
		WIID_ITEM_UPGRADE_BG_MIDDLE																			,
		WIID_ITEM_UPGRADE_BG_BOTTOM																			,
		WIID_ITEM_UPGRADE_TITLE																				,
		WIID_ITEM_UPGRADE_CLOSE																				,
		WIID_ITEM_UPGRADE_RUN																				,
		WIID_ITEM_UPGRADE_CANCEL																			,
		WIID_ITEM_UPGRADE_SUCCESS_PROBABILITY_BG															,
		WIID_ITEM_UPGRADE_SUCCESS_PROBABILITY_TEXT															,
		WIID_ITEM_UPGRADE_SUCCESS_PROBABILITY_TEXT_BOLD														,
		WIID_ITEM_UPGRADE_PANEL_CONTENTS																	,
		WIID_ITEM_UPGRADE_BALLOON_MAIN																		,
		WIID_ITEM_UPGRADE_BALLOON_POINT																		,
		WIID_ITEM_UPGRADE_BALLOON_CORNER_RIGHT_TOP															,
		WIID_ITEM_UPGRADE_BALLOON_CORNER_RIGHT_BOTTOM														,
		WIID_ITEM_UPGRADE_ITEM_BOX																			,
		WIID_ITEM_UPGRADE_ITEM_PIC																			,
		WIID_ITEM_UPGRADE_RESULT_BOX																		,
		WIID_ITEM_UPGRADE_RESULT_BOX_MARK																	,
		WIID_ITEM_UPGRADE_RESULT_PIC																		,
		WIID_ITEM_UPGRADE_ITEM_TOOLTIP																		,
		WIID_ITEM_UPGRADE_ITEM_TOOLTIP_CURSOR																,
		WIID_ITEM_UPGRADE_MATERIAL_TOOLTIP																	,
		WIID_ITEM_UPGRADE_MATERIAL_TOOLTIP_CURSOR															,

		WIID_ITEM_UPGRADE_PROGRESS																			,
		WIID_ITEM_UPGRADE_PROGRESS_BG_LEFT																	,
		WIID_ITEM_UPGRADE_PROGRESS_BG_MIDDLE																,
		WIID_ITEM_UPGRADE_PROGRESS_BG_RIGHT																	,
		WIID_ITEM_UPGRADE_PROGRESS_GAGE																		,
		WIID_ITEM_UPGRADE_PROGRESS_GAGE_RIGHT																,
		WIID_ITEM_UPGRADE_PROGRESS_GAGE_DECORATION															,
		WIID_ITEM_UPGRADE_PROGRESS_BRIGHT																	,
		WIID_ITEM_UPGRADE_PROGRESS_NUMBER_BG																,
		WIID_ITEM_UPGRADE_PROGRESS_NUMBER_PER																,
		WIID_ITEM_UPGRADE_PROGRESS_NUMBER_START																,
		WIID_ITEM_UPGRADE_PROGRESS_NUMBER_END			=	WIID_ITEM_UPGRADE_PROGRESS_NUMBER_START	+	WIID_ITEM_UPGRADE_PROGRESS_NUMBER_COUNT			,

		WIID_ITEM_UPGRADE_MATERIAL_BOX_START																,
		WIID_ITEM_UPGRADE_MATERIAL_BOX_END				=	WIID_ITEM_UPGRADE_MATERIAL_BOX_START	+	WIID_ITEM_UPGRADE_MATERIAL_BOX_COUNT			,
		WIID_ITEM_UPGRADE_MATERIAL_PIC_START																,
		WIID_ITEM_UPGRADE_MATERIAL_PIC_END				=	WIID_ITEM_UPGRADE_MATERIAL_PIC_START	+	WIID_ITEM_UPGRADE_MATERIAL_BOX_COUNT			,

		WIID_ITEM_UPGRADE_END							=	WIID_ITEM_UPGRADE						+	0x999											,
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	ITEM_UPGRADE_STATE				m_eCurState;						//	현재 작업 상태;

	float							m_fItemToolTipAccmulateTime;		//	아이템 툴팁 현재 시간
	float							m_fItemToolTipMaxTime;				//	아이템 툴팁 맥스 시간
	int								m_iCurItemToolTipImgIndex;			//	아이템 이미지 인덱스
	int								m_iMaxItemToolTipImgIndex;			//	아이템 이미지 최대 인덱스
	int								m_iStepItemToolTipImgIndex;			//	아이템 이미지 인덱스 변경 값

	float							m_fMaterialToolTipAccmulateTime;	//	재료 툴팁 현재 시간
	float							m_fMaterialToolTipMaxTime;			//	재료 툴팁 맥스 시간
	int								m_iCurMaterialToolTipImgIndex;		//	재료 이미지 인덱스
	int								m_iMaxMaterialToolTipImgIndex;		//	재료 이미지 최대 인덱스
	int								m_iStepMaterialToolTipImgIndex;		//	재료 이미지 인덱스 변경 값

	int								m_iProgressGageWidth;				//	프로그래스바 게이지 너비

	float							m_fProgressAccmulateTime;			//	프로그래스바 현재 시간
	float							m_fProgressMaxTime;					//	프로그래스바 맥스 시간

	float							m_fProgressBrightAccmulateTime;		//	프로그래스바 밝기 이펙스 현재 시간
	float							m_fProgressBrightMaxTime;			//	프로그래스바 밝기 이펙트 맥스 시간

	SPItem*							m_pUpgradeItem;												//	업그레이드 할 아이템
	SPItem*							m_pMaterialItem[ WIID_ITEM_UPGRADE_MATERIAL_BOX_COUNT ];	//	재료 아이템

	SPUIUnitManager					m_UIUnitManager;

	bool							m_bUse;
};
