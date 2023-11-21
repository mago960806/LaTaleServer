// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPPlayerCoordPhysics
// Comment     : 클라이언트/서버 모두 쓰이는 모듈이므로 특화된 모듈 추가시 주의.
//
// Creation    : metalgeni 2004-08-24 오전 10:26:19
//***************************************************************************

#pragma once

class SPGOBCoordPhysics;

class SPPlayerCoordPhysics : public SPGOBCoordPhysics
{
public:
#ifdef _LATALE_CLIENT
	SPPlayerCoordPhysics(SPGameObject* pkParent = NULL);
#else
	SPPlayerCoordPhysics(CCharacter* pkParent = NULL);
#endif
	virtual ~SPPlayerCoordPhysics();
	
	virtual void Init();
	virtual void Process(float fElapsedTime);

	virtual void SetAction(UINT64 uiAction);
	virtual bool IsActionChange(UINT64& uiAction);

	virtual void SetFirstAction();
	virtual bool CheckDrop();
	virtual bool CheckDrop(float fPosX, float fPosY);

protected:
	virtual void CalVelocity(UINT64 uiAction);
	virtual void StateProcess();

	// state function
	virtual void OnStateLooting();
	virtual void OnStateLootingWait();
	virtual void OnStateLootingStand();
	virtual void OnStateSitDown();
	virtual void OnStateSitDownWait();
	virtual void OnStateSitDownStand();
	virtual void OnStateRun();
	virtual void OnStateRunStop();
	virtual void OnStateDrop();
	virtual void OnStateLand();
	virtual void OnStateAttackSpeedY();
	virtual void OnStateAttackMighty();
	virtual void OnStateCriticalAttacked();
	virtual void OnStateGuard();
	virtual void OnStateAttackedDown();
	virtual void OnStateAttackedDownStand();
	virtual void OnStateSkill();
	virtual void OnStateBlock();
	virtual void OnStateEnchantResult();

protected:
	bool m_bReserveHangingStop;
};














