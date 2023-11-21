
#pragma once 

class SPPet;

class SPGroundPet : public SPPet
{
public:
	SPGroundPet();
	SPGroundPet(INSTANCE_ID instanceID, CLASS_ID ClassID, SPGameObject* pkParent);
	virtual ~SPGroundPet();

	virtual void Process(float fTime);
	virtual void Init();

	virtual void SetState(GOB_STATE_TYPE state, bool bForce = false);
	virtual void SetAction(UINT64 uiAction, bool bForce = false);
	virtual void SetLootingFX(int iID = 0);
	
protected:
	SPGMESSAGE_OBJ_MAP_DECLARE()

	virtual void OnWaitPattern(float fTime);
	virtual void OnTracePattern(float fTime);
	virtual void OnSelfPosPattern(float fTime);

	virtual void CaluSelfPos();
	virtual void CheckTraceAction(int iDirection);

	int CheckMovable(unsigned __int64 uiAction);

protected:
	int m_iMoveVerType;
};