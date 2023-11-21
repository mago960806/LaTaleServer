// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once 

enum PART_LAYER;
enum FXUNIT_DAMAGE_TYPE;

struct SPGOBModel;

class SPGameObject;
class SPGOBModelUnit;
class SPMonsterCoordPhysics;
class SPSoundArchive;
class SPFXArchive;
class SPEffectArchive;
class SPSkillArchive;
class SPMonsterStatusModel;

class SPMonster : public SPGameObject 
{

public:
	SPMonster();
	SPMonster(INSTANCE_ID instanceID, CLASS_ID ClassID);
	virtual ~SPMonster();

	virtual void Process(float fTime);
	virtual	void Render(float fTime);
	virtual void Init();	
	virtual void Destroy();
	virtual void RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer );

	void ProcessImage(float fTime);

	void SetState(GOB_STATE_TYPE state, bool bForce = false);
	void SetAction(UINT64 uiAction, bool bForce = false);

	FACING	GetDirection();				
	BG_ID	GetStandLayer();				
	void	SetPos(float fX, float fY);
	
	int		GetCurStageID();
	void	SetCurStageID(int iStageID);
	int		GetCurMapGroupID();
	void	SetCurMapGroupID(int iMapGroupID);

	SPGOBModelUnit* GetModelUnit();
	SPGOBCoordPhysics* GetCoordPhysics();

	bool IsDead();
	void SetShowStatus(bool bName, bool bHP, bool bShadow);

	void SetMonsterType(int iType);
	void SetMOBName(const char* pszName);

	int  GetIconIndex();
	SPGOBStatus* GetGobStatus();

	virtual bool IsShowName();
	virtual bool IsShowHP();

protected:
	SPGMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnGetPosX( LPARAM lParam );
	virtual int OnGetPosY( LPARAM lParam );
	virtual int OnGetFacing(LPARAM lParam);
	virtual int OnGetStandlayer(LPARAM lParam);
	virtual int OnGetModelUnit	( LPARAM lParam );
	virtual int OnGetCoordPhysics( LPARAM lParam );
	virtual int OnGetEffectArchive( LPARAM lParam );
	virtual int OnGetFXArchive( LPARAM lParam );
	virtual int OnGetSoundArchive( LPARAM lParam );
	virtual int OnGetGOBStatus( LPARAM lParam );
	virtual int OnGetCurAniIndex( LPARAM lParam );
	virtual int OnGetCurAniFrame( LPARAM lParam );
	virtual int OnEffectFXSound( LPARAM lParam );
	virtual int OnCasterEffectFXSound(LPARAM lParam);
	virtual int OnTargetEffectFXSound(LPARAM lParam);
	virtual int OnIsDead(LPARAM lParam);
	virtual int OnIsPlayer(LPARAM lParam);
	virtual int OnGetType(LPARAM lParam);
	virtual int OnPlaySound(LPARAM lParam);
	virtual int OnGetAction(LPARAM lParam);

	virtual int OnSetMonsterInfo(LPARAM lParam);
	virtual int OnSetLPPosotion(LPARAM lParam);
	virtual int OnSetMoveCommand(LPARAM lParam);
	virtual int OnSetDamageFX	( LPARAM lParam );
	virtual int OnSetHealingFX	( LPARAM lParam );
	virtual int OnSetEffectActivity(LPARAM lParam);
	virtual int OnSetEffectDelete(LPARAM lParam);
	virtual int OnFXGroup(LPARAM lParam);
	virtual int OnSetKnockBack(LPARAM lParam);
	virtual int OnSetNameColor(LPARAM lParam);
	virtual int OnSetAction(LPARAM lParam);
	virtual int OnSetStatusRender(LPARAM lParam);
	virtual int OnSetGOBSeqIndex(LPARAM lParam);
	virtual int OnSetIgnoreStage(LPARAM lParam);

	virtual int	OnStatMessage( SPGM msg, LPARAM lParam );

	virtual int OnSetActionCommand(LPARAM lParam);
	virtual int OnSetActionCompleteCommand(LPARAM lParam);

	void AttackBoxRender();
	void Test();

	void SetAnimationLoop();
	void SetActionSound(ANIMATION_INDEX eIndex, bool bSkill = false);

	void InsertDamageFX( FXUNIT_DAMAGE_TYPE eDamageType , int iDamage , int iMode , int iType );
	void InsertHitComboFX( UINT8 uiHitComboCount , SPFXArchive* pLocalFXArchive );

protected:		

	// State
	GOB_STATE_TYPE m_eCurState;
	GOB_STATE_TYPE m_eLastState;
	bool m_bStatusRender;
	
	// Action
	UINT64 m_uiCurAction;
	UINT64 m_uiLastAction;
	ANIMATION_INDEX m_eCurAnimationIndex;
	ANIMATION_INDEX m_eAttackRandIndex;

	int m_iType;
	
	float m_fCamX;
	float m_fCamY;
	
	float m_fDeadTime;
	float m_fDeadAccmulateTime;

	RECT m_RectPosition;

	// Render Model
	SPGOBModelUnit*			m_pkModelUnit;
	SPGOBCoordPhysics*		m_pkCoordPhysics;
	SPGOBStatus*			m_pkStatus;
	SPMonsterStatusModel*	m_pkStatusModel;
	SPFXArchive*			m_pkFXArchive;
	SPSoundArchive*			m_pSoundArchive;
	SPEffectArchive*		m_pkEffectArchive;
	SPSkillArchive*			m_pkSkillArchive;
};



