#ifndef __SPGOB_COORDPHYSICS_H__
#define __SPGOB_COORDPHYSICS_H__

#define SPEED_LIMIT_LAND	(15.0f)

// 0 : -800.0f, 1: -100.0f
const float g_fSimulateJumpLength[2] = {-193.33130f, -2.3040004f};
const float g_fMaxJumpPosTime[2] = {0.51200002f, 0.064000003f};

enum GOB_STATE_TYPE
{
	GOB_STATE_NULL = 0,
	GOB_STATE_IDLE,
	GOB_STATE_STAND,
	GOB_STATE_VERTICAL_STAND,
	GOB_STATE_HANGING_STAND,
	GOB_STATE_WALK,
	GOB_STATE_RUN,
	GOB_STATE_RUN_STOP,
	GOB_STATE_JUMP,
	GOB_STATE_DROP,
	GOB_STATE_LAND,
	GOB_STATE_ATTACK_SPEEDY,
	GOB_STATE_ATTACK_MIGHTY,
	GOB_STATE_UP,
	GOB_STATE_DOWN,
	GOB_STATE_HANGING,
	GOB_STATE_ATTACKED,
	GOB_STATE_CRITICAL_ATTACKED,
	GOB_STATE_ATTACKED_DOWN,
	GOB_STATE_ATTACKED_DOWN_STAND,
	GOB_STATE_GUARD,
	GOB_STATE_GUARD_DAMAGE,
	GOB_STATE_DEAD,
	GOB_STATE_LOOTING,
	GOB_STATE_LOOTING_WAIT,
	GOB_STATE_LOOTING_STAND,
	GOB_STATE_SIT_DOWN,
	GOB_STATE_SIT_WAIT,
	GOB_STATE_SIT_STAND,
	GOB_STATE_SKILL,
	GOB_STATE_BLOCK,
	GOB_STATE_ENCHANTTING,
	GOB_STATE_ENCHANT_RESULT,
	GOB_STATE_USERSHOP,
};

struct KNOCKBACK_INFO
{
	bool bKnockBack;
	bool bDelay;

	float fKnockBackTime;
	float fDelayTime;
	float fKnockBackAccmulateTime;

	float fKnockBackVelocityX;
	float fKnockBackVelocityY;
};

struct ACTIVE_VEL_INFO
{
	bool bEnable;
	bool bDelay;

	float fTime;
	float fDelayTime;
	float fAccmulateTime;

	float fVelocityX;
	float fVelocityY;
};

class SPGameObject;
class SPStage;
class SPMapGroup;
class SPTerrainAttribute;

#ifdef _LATALE_SERVER
class CCharacter;
#endif

class SPGOBCoordPhysics
{
public:
#ifdef _LATALE_CLIENT
	SPGOBCoordPhysics(SPGameObject* pkParent = NULL);
#else
	SPGOBCoordPhysics(CCharacter* pkParent = NULL);
#endif
	virtual ~SPGOBCoordPhysics();

	virtual void Init();
	virtual void Process(float fElapsedTime);
	virtual void ProcessTimeTick(float fElapsedTime);

	virtual UINT64 GetCurAction();
	virtual void SetAction(UINT64 uiAction);
	virtual bool IsActionChange(UINT64& uiAction);
	virtual void SetFirstAction();

	// inline Func
	float GetPosX();
	float GetPosY();
	void SetPosX(float fPosX);
	void SetPosY(float fPosY);
	void SetPos(float fPosX, float fPosY);
	

	float GetMaxVelocityX();
	float GetMaxVelocityY();
	float GetCurVelocityX();
	float GetCurVelocityY();
	float GetAccelX();
	float GetAccelY();
	float GetJumpSpeed();
	float GetSimulateJumpLength();
	float GetGravity();
	float GetMaxDropVelocity();

	char GetMoveVerType();
	void SetMoveVerType(char cType);
	GOB_STATE_TYPE GetCurState();
	
	void SetMaxVelocityX(float fVelocityX);
	void SetMaxVelocityY(float fVelocityY);
	void SetMaxVelocity(float fVelocityX, float fVelocityY);
	void SetMaxUpDownVelocity(float fVelocity);
	void SetMaxHangingVelocity(float fVelocity);
	float GetMaxUpDownVelocity(void);    //[xialin 2008/08/06]add
	float GetMaxHangingVelocity(void);   //[xialin 2008/08/06]add
	
	void SetVelocityRatioX(float fRatio);
	void SetJumpRatio(float fRatio);

	void SetCurVelocityX(float fVelocityX);
	void SetCurVelocityY(float fVelocityY);
	void SetCurVelocity(float fVelocityX, float fVelocityY);

	void SetAccelX(float fAccelX);
	void SetAccelY(float fAccelY);
	void SetAccel(float fAccelX, float fAccelY);
	void SetJumpSpeed(float fJumpSpeed);
	void SetGravity(float fGravity);
	void SetMaxDropVelocity(float fVelocity);

	void SetDirection(const FACING eFace);
	FACING GetDirection();
	void SetStandLayer(const BG_ID eLayer = BG_ID_LAYER_TERRAIN0);
	BG_ID GetStandLayer();

	void SetActionTime(float fTime);
	float GetActionTime();

	// 현재 위치 기존, 다음 위치값 계산
	void GetNextPos(float& fPosX,float& fPosY, float fVelocityX = 0.0f, float fVelocityY = 0.0f);
	int GetNextAttriState(float& fPosX, float& fPosY, unsigned int& uiAttrbute);
	bool IsJumpable(float& fPosX, float& fPosY);
	bool IsMoveJumpable(float fPosX, float fPosY, FACING eFacing);
	bool IsMoveDrop(float fPosX, float fPosY, FACING eFacing);

	float GetHeight(float fPosX, float fPosY);

	int		GetCurStageID();
	int		GetCurMapGroupID();
	void	SetCurStageID(int iStageID);
	void	SetCurMapGroupID(int iMapGroupID);	
	void	SetCoordPhysicsMap(int iStageID, int iMapGroupID); //내부 스테이지 맵그룹 포인터를 NULL로 셋팅한다

	virtual void SetKnockBack(KNOCKBACK_INFO& Info);
	KNOCKBACK_INFO* GetKnockBack();

	void SetActicityVelocity(ACTIVE_VEL_INFO& Info);
	bool ReCalKineticEnergy();

	unsigned char IsFacingChange(UINT64 uiAction);
	void SetIgnoreStage(bool bIgnore);

	//[xialin 2008/07/29] add
	void	GetLTPosition(LT_POSITION * pos);

	bool	GetMoveEnable(float& fPosX, float& fPosY, int iDir = 0);		//[xialin 2008/11/07]add

protected:
	// state function
	virtual void OnStateIdle();
	virtual void OnStateStop();
	virtual void OnStateVerticalStand();
	virtual void OnStateHangingStand();
	virtual void OnStateWalk();
	virtual void OnStateDrop();
	virtual void OnStateJump();
	virtual void OnStateMoveUp();
	virtual void OnStateMoveDown();
	virtual void OnStateHanging();
	virtual void OnStateAttackSpeedY();
	virtual void OnStateAttackMighty();
	virtual void OnStateSkill();
	virtual void OnStateAttacked();
	virtual void OnStateDead();
	virtual void OnStateSitDown();
	virtual void OnStateSitDownWait();
	virtual void OnStateSitDownStand();
	virtual void OnStateRun();
	virtual void OnStateGuardDamage();

	virtual void StateProcess();
	void StateChange(GOB_STATE_TYPE eType);

	virtual bool CheckDrop();
	virtual bool CheckDrop(float fPosX, float fPosY);
	bool CheckMoveEnable(unsigned int uiAttribute, float& fPosX, float& fPosY,
		float* pfMapOffsetX, float* pfMapOffsetY, SPTerrainAttribute* pkTerrainAttribute = NULL);
	bool CheckBlock(unsigned int uiAttribute, float& fPosX, float& fPosY);
	void InspectStandLayer(bool bRevisionY = true);

	bool EnableLadder(bool bUp);
	bool EnableVerRope(bool bUp);
	bool EnableHozRope();
	bool CheckUpBlock();

	virtual void CalKineticEnergy(float fElapsedTime);
	virtual void CalVelocity(UINT64 uiAction);

	int SendActionCommand(LPARAM lParam);
	int SendActionCompleteCommand(LPARAM lParam);
	int DirectActionCommand(LPARAM lParam);
	int DirectActionCompleteCommand(LPARAM lParam);
	
	void RestorePos();
	
	void AddAction(UINT64& uiAction, UINT64 uiAddAction);
	void SubAction(UINT64& uiAction, UINT64 uiSubAction);

protected:
	
	bool  m_bMove;

	// Pos
	float m_fPosX;
	float m_fPosY;

	float m_fBeforePosX;
	float m_fBeforePosY;

	float m_fMapOffsetPosX;
	float m_fMapOffsetPosY;
	
	// Velocity
	float m_fMaxVelocityX;
	float m_fMaxVelocityY;
	float m_fMaxUpDownVelocity;
	float m_fMaxHangingVelocity;
	float m_fCurVelocityX;
	float m_fCurVelocityY;
	float m_fAccelX;
	float m_fAccelY;
	float m_fJumpSpeed;
	float m_fSimulateJumpLength;
	float m_fMaxJumpPosY;
	float m_fMaxJumpPosTime;
	float m_fGravity;
	float m_fMaxDropVelocity;

	float m_fDefGravity;
	float m_fDefDropVelocity;
	float m_fDefVelocityX;
	float m_fDefJumpSpeed;
	float m_fDefUpDownVelocity;
	float m_fDefHangingVelocity;

	// knockback
	KNOCKBACK_INFO m_KnockBackInfo;
	ACTIVE_VEL_INFO m_ActivityVelocityInfo;

	bool m_bJumpAttack;
	bool m_bTerrainAttributeChange;

	// state info
	FACING m_eFacing;
	BG_ID m_eStandLayer;
	GOB_STATE_TYPE m_eCurState;
	
	unsigned int m_uiTerAttribute;
	unsigned int m_uiBeforeTerAttribute;
	int m_iRevision;

	char m_cMoveVerType;
	bool m_bAutoHanging;
	float m_fAutoHangingPosY;
	
	UINT64 m_uiCurAction;
	float m_fActionTime;
	bool m_bIgnoreStage;

#ifdef _LATALE_CLIENT
	SPGameObject* m_pkParent;
#elif _LATALE_SERVER
	CCharacter* m_pkParent;
	float m_fProcessAccumulateTime;
#endif

	int	m_iCurStageID;
	int	m_iCurMapGroupID;

	SPStage* m_pkCurStage;
	SPMapGroup* m_pkCurMapGroup;
	SPTerrainAttribute* m_pkCurTerrainAttribute;
};

#include "SPGOBCoordPhysics.inl"

#endif