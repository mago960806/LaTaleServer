// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPMonsterCoordPhysics
// Comment     : 클라이언트/서버 모두 쓰이는 모듈이므로 특화된 모듈 추가시 주의.
//
// Creation    : metalgeni 2004-08-24 오전 10:26:19
//***************************************************************************

#pragma once

class SPMonsterCoordPhysics : public SPGOBCoordPhysics
{
public:
#ifdef _LATALE_CLIENT
	SPMonsterCoordPhysics(SPGameObject* pkParent = NULL);
#else
	SPMonsterCoordPhysics(CCharacter* pkParent = NULL);
#endif
	virtual ~SPMonsterCoordPhysics();
	
	virtual void Init();
	virtual void Process(float fElapsedTime);

	virtual bool IsActionChange(UINT64& uiAction);

protected:
	virtual void CalVelocity(UINT64 uiAction);

	virtual void OnStateAttackSpeedY();
	virtual void OnStateAttackMighty();
	virtual void OnStateSkill();
};

#include "SPMonsterCoordPhysics.inl"