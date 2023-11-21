
#pragma once

class SPFlyCoordPhysics : public SPGOBCoordPhysics
{
public:
#ifdef _LATALE_CLIENT
	SPFlyCoordPhysics(SPGameObject* pkParent = NULL);
#else
	SPFlyCoordPhysics(CCharacter* pkParent = NULL);
#endif
	virtual ~SPFlyCoordPhysics();

	virtual void Init();
	virtual void Process(float fElapsedTime);
	virtual void ProcessTimeTick(float fElapsedTime);

	virtual void SetAction(UINT64 uiAction);
	virtual bool IsActionChange(UINT64& uiAction);

	virtual void SetKnockBack(KNOCKBACK_INFO& Info);

protected:
	virtual void CalVelocity(UINT64 uiAction);
	virtual void CalKineticEnergy(float fElapsedTime);

	virtual void OnStateStop();
	virtual void OnStateWalk();
	virtual void OnStateAttackSpeedY();
	virtual void OnStateAttackMighty();
	virtual void OnStateSkill();
	virtual void OnStateAttacked();
	virtual void OnStateDead();
	virtual void OnStateRun();
	virtual void OnStateDrop();
	virtual void OnStateGuardDamage();
};