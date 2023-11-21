// ***************************************************************
//  SPWindowPetLevelUp   version:  1.0   ·  date: 11/22/2006
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************/

#pragma once

enum PET_LEVELUP_STATE
{
	PET_LEVELUP_STATE_NULL					=	0	,
	PET_LEVELUP_STATE_CANCEL						,
	PET_LEVELUP_STATE_READY_ITEM					,
	PET_LEVELUP_STATE_READY_MATERIAL				,
	PET_LEVELUP_STATE_READY_UPGRADE					,
	PET_LEVELUP_STATE_UPGRADING						,
	PET_LEVELUP_STATE_BRIGHTTING					,
	PET_LEVELUP_STATE_UPGRADING_WAIT				,
	PET_LEVELUP_STATE_END							,
};

class SPItem;
class SPUIUnit;
class SPUIUnitManager;
class SPWindow;

class SPWindowPetLevelUp : public SPWindow
{

public:
	SPWindowPetLevelUp(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowPetLevelUp(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowPetLevelUp();

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

	int			OnResult				( WPARAM wParam , LPARAM lParam );

	int			OnClose					( WPARAM wParam , LPARAM lParam );
	int			OnRun					( WPARAM wParam , LPARAM lParam );

	int			OnItemBox				( WPARAM wParam , LPARAM lParam );
	int			OnItemBoxCancel			( WPARAM wParam , LPARAM lParam );
	int			OnMaterial				( WPARAM wParam , LPARAM lParam );
	int			OnMaterialCancel		( WPARAM wParam , LPARAM lParam );

	int			OnItemCursorEnter		( WPARAM wParam , LPARAM lParam );
	int			OnItemCursorOut			( WPARAM wParam , LPARAM lParam );
	int			OnResultCursorEnter		( WPARAM wParam , LPARAM lParam );
	int			OnResultCursorOut		( WPARAM wParam , LPARAM lParam );
	int			OnMaterialCursorEnter	( WPARAM wParam , LPARAM lParam );
	int			OnMaterialCursorOut		( WPARAM wParam , LPARAM lParam );

	// }}

	void		InitContentsWindow		( SPWindow* pParent , int iAdjustX , int iAdjustY );
	void		InitWhiteBoardWindow	( SPWindow* pParent , int iAdjustX , int iAdjustY );
	void		InitUIItemUnit			();

	void		UpdatePosUIItemUnit		();

	void		ProcessItemToolTip		( float fTime );
	void		ProcessMaterialToolTip	( float fTime );

	void		DefaultHideWindow		();

	void		NextState				();

	void		SetUpgradeItem			( SPItem* pUpgradeItem );
	void		SetMaterialItem			( SPItem* pMaterialItem );


	void		SendServerRun			();

	void		ResetMaterialData		();

	bool		CheckMaterialCondition	();
	bool		CheckMaterialCondition	( int& iOutMaterialCount , int& iOutMaterialPos , SPItem* pUpgradeItem , SPItem* pMaterialItem , bool bMultiCheck );


	//////////////////////////////////////////////////////////////////////////
	
	void		SetError				( int iGloalStringID , const char* pszItemString = NULL );
	void		SetError				( const char* pszErrorString );
	SPItem*		GetInvenPickUpItem		();
	void		SetInvenItemLock		( SPItem* pkItem , int iIndex );
	void		SetInvenItemUnlock		( SPItem* pkItem , int iIndex );
	void		SetInvenItemAllUnlock	();

	//////////////////////////////////////////////////////////////////////////

	enum GLOBAL_STRING_PET_LEVELUP
	{
		GS_PET_LEVELUP_NULL								=	0												,
		GS_PET_NEED_EXP									=	11001001										,
		GS_PET_LEVELUP_ITEM_TYPE_DIFFERENT				=	11001002										,
		GS_PET_LEVELUP_ITEM_DURATIONTYPE_DIFFERENT		=	11001003										,
		GS_PET_LEVELUP_ONLY_ITEM_WINDOW					=	11001004										,
		GS_PET_LEVELUP_NOT_LEVELUP						=	11001005										,

		GS_PET_LEVELUP_NOT_UPGRADE						=	11000011										,
		GS_PET_LEVELUP_ACTION_STOP						=	11000015										,
		GS_PET_LEVELUP_MATERIAL_DIFFERENT				=	11000012										,
		GS_PET_LEVELUP_MATERIAL_COUNT					=	11000013										,
		GS_PET_LEVELUP_MATERIAL_RARE					=	11000014										,
		GS_PET_LEVELUP_IS_MATERIAL						=	11000017										,

		GS_PET_LEVELUP_ITEM_EXPIRE						=	10000040										,
	};

	enum PET_LEVELUP_INSTANCE_ID
	{
		WIID_PET_LEVELUP_UIUNIT_POS_ITEM				=	0												,
		WIID_PET_LEVELUP_UIUNIT_POS_MATERIAL			=	1												,
		WIID_PET_LEVELUP_UIUNIT_POS_RESULT				=	2												,
		WIID_PET_LEVELUP_UIUNIT_COUNT					=	3												,

		WIID_PET_LEVELUP_BG_TOP							=	WIID_PET_LEVELUP					+	1		,
		WIID_PET_LEVELUP_BG_MIDDLE																			,
		WIID_PET_LEVELUP_BG_BOTTOM																			,
		WIID_PET_LEVELUP_TITLE																				,
		WIID_PET_LEVELUP_CLOSE																				,
		WIID_PET_LEVELUP_RUN																				,
		WIID_PET_LEVELUP_LINE1																				,
		WIID_PET_LEVELUP_LINE2																				,

		WIID_PET_LEVELUP_PANEL_CONTENTS																		,
		WIID_PET_LEVELUP_ITEM_BOX																			,
		WIID_PET_LEVELUP_ITEM_PIC																			,
		WIID_PET_LEVELUP_MATERIAL_BOX																		,
		WIID_PET_LEVELUP_MATERIAL_PIC																		,
		WIID_PET_LEVELUP_RESULT_BOX																			,
		WIID_PET_LEVELUP_RESULT_BOX_MARK																	,
		WIID_PET_LEVELUP_RESULT_PIC																			,
		WIID_PET_LEVELUP_PLUS																				,
		WIID_PET_LEVELUP_EQUAL																				,
		WIID_PET_LEVELUP_ITEM_TOOLTIP																		,
		WIID_PET_LEVELUP_ITEM_TOOLTIP_CURSOR																,
		WIID_PET_LEVELUP_MATERIAL_TOOLTIP																	,
		WIID_PET_LEVELUP_MATERIAL_TOOLTIP_CURSOR															,
		WIID_PET_LEVELUP_RUN_TOOLTIP																		,

		WIID_PET_LEVELUP_END							=	WIID_PET_LEVELUP						+	0x999											,
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	PET_LEVELUP_STATE				m_eCurState;						//	현재 작업 상태

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

	SPItem*							m_pUpgradeItem;						//	업그레이드 할 아이템 ( 레벨 업 할 펫 )
	SPItem*							m_pMaterialItem;	//	재료 아이템

	SPUIUnitManager					m_UIUnitManager;

};
