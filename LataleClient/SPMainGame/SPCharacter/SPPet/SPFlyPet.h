
#pragma once 

class SPPet;

class SPFlyPet : public SPPet
{
public:
	SPFlyPet();
	SPFlyPet(INSTANCE_ID instanceID, CLASS_ID ClassID, SPGameObject* pkParent);
	virtual ~SPFlyPet();

	virtual void Init();

	virtual void SetState(GOB_STATE_TYPE state, bool bForce = false);
	virtual void SetAction(UINT64 uiAction, bool bForce = false);
	virtual void SetLootingFX(int iID = 0);

	virtual bool IsPreRendering();

protected:
	SPGMESSAGE_OBJ_MAP_DECLARE()

	virtual void OnWaitPattern(float fTime);
	virtual void OnTracePattern(float fTime);
	virtual void OnSelfPosPattern(float fTime);

	virtual void CaluSelfPos();
	virtual void CheckTraceAction(int iDirection);
};