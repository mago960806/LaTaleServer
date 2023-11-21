#ifndef __SPSKILL_ARCHIVE_H__
#define __SPSKILL_ARCHIVE_H__

class SPSkill;
class SPGameObject;
class SPEffectArchive;
class SPMotionStatus;

struct tagATTACKINFO;
struct tagBEATTACKEDINFO;
struct tagAPCBEATTACKEDINFO;
typedef tagATTACKINFO ATTACKINFO;
typedef tagBEATTACKEDINFO BEATTACKEDINFO;
typedef tagAPCBEATTACKEDINFO APCBEATTACKEDINFO;


struct FX_SOUND_INFO
{
	SPGameObject* pkMissileTarget;
	SKILL_APPLY_EFFECT* pApplyEffect;
	std::vector< SPGameObject* > vTargetList;

	~FX_SOUND_INFO()
	{
		vTargetList.clear();
	}
};

class SPSkillActivity
{
public:
	SPSkillActivity();
	SPSkillActivity(SPSkill* pkSkill, int iLevel, int iCurLevel);
	SPSkillActivity(int iID, int iLevel, int iCurLevel);
	virtual ~SPSkillActivity();

	virtual void Init();
	virtual bool Process(float fElapsedTime);

	void SetSkill(SPSkill* pkSkill);
	SPSkill* GetSkill();
	int GetLevel();
	void SetLevel(int iLevel);

	int GetCurLevel();
	void SetCurLevel( int iCurLevel );

	bool GetPossibleUsed();

	void SetToggle(bool bOn);
	bool GetToggleOn();

protected:
	void PossibleUsedCheck();

protected:
	SPSkill* m_pkSkill;

	int m_iLevel;			//	베이스 레벨 [6/22/2006]
	int m_iCurLevel;		//	버프나 기타 적용으로 인해 적용되는 최종값  [6/22/2006]
	bool m_bPossibleUsed;
	bool m_bToggleOn;
	float m_fAccmulateTime;
};

class SPItemSkillActivity : public SPSkillActivity
{
public:
	SPItemSkillActivity();
	SPItemSkillActivity(SPSkill* pkSkill, int iLevel, int iCurLevel,
		int iItemID, int iContainerType, int iSlotIndex);
	virtual ~SPItemSkillActivity();

	virtual void Init();

	int GetItemID();
	int GetSlotIndex();
	int GetContainerType();

protected:
	int m_iItemID;
	int m_iSlotIndex;
	int m_iContainerType;
};

class SPSkillUsed : public SPSkillActivity
{
public:
	SPSkillUsed();
	SPSkillUsed(SPGameObject* pkParent, SPSkill* pkSkill);
	virtual ~SPSkillUsed();

	virtual void Init();
	virtual bool Process(float fElapsedTime);

	void SetParent(SPGameObject* pkParent) { m_pkParent = pkParent; }
	SPGameObject* CheckAttackRgn(SPSkill * pSkill);		//[xialin 2008/11/10]add
	SPGameObject* CheckAttackRgn(int iActionCommand);	//[xialin 2008/11/10]add

protected:
	void CheckApplyEffect(SKILL_APPLY_EFFECT* pApplyEffect);
	
	bool InAttackRgn(std::vector< ATTACKINFO > & vAttackInfo, FACING eCasterFacing,
		int iCasterPosX, int iCasterPosY, int iTargetPosX, int iTargetPosY,
		SPGameObject* pkTargetObject, std::vector< BEATTACKEDINFO > vBeAttackedInfo, int iFrame);
	bool InAttackRgn(std::vector< ATTACKINFO > & vAttackInfo, FACING eCasterFacing,
		int iCasterPosX, int iCasterPosY, int iTargetPosX, int iTargetPosY,
		SPGameObject* pkTargetObject, std::vector< APCBEATTACKEDINFO > vBeAttackedInfo, int iFrame);
	
	//[xialin 2008/11/11]add
	bool InAttackRect(std::vector< ATTACKINFO > & vAttackInfo, FACING eCasterFacing,
		int iCasterPosX, int iCasterPosY, int iTargetPosX, int iTargetPosY,
		SPGameObject* pkTargetObject, std::vector< APCBEATTACKEDINFO > vBeAttackedInfo, int iFrame);
	
	void SetApplyEffect(SPEffectArchive* pkEffectArchive,
		SKILL_APPLY_EFFECT* pApplyEffect, bool bSelf);

	virtual void CheckTargetCount(int iCasterPosX, int iCasterPosY,
		SKILL_APPLY_EFFECT* pApplyEffect,std::vector< SPGameObject* >& vGameObject,
		std::vector< SPGameObject* >& vTargetObject);

protected:
	SPGameObject* m_pkParent;

	int m_iCurEffectCount;
	int m_iMaxEffectCount;
};

class SPMotionUsed : public SPSkillUsed
{
public:
	SPMotionUsed();
	SPMotionUsed(SPGameObject* pkParent, SPMotionStatus* pkMotionStatus, unsigned int uiAniIndex);
	virtual ~SPMotionUsed();

	SPMotionStatus* GetMotionStatus();
	unsigned int GetAniIndex();

	virtual void Init();
	virtual bool Process(float fElapsedTime);
	
protected:
	SPMotionStatus* m_pkMotionStatus;
	unsigned int m_uiAniIndex;
};

class SPSkillArchive
{
public:
	SPSkillArchive(SPGameObject* pkParent);
	virtual ~SPSkillArchive();

	void Clear();
	void Process(float fElapsedTime);

	bool AddSkill(int iOrder, SPSkillActivity* pkSkillActivity);
	bool AddSkill(int iOrder, SPSkill* pkSkill, unsigned int uiLevel, unsigned int uiCurLevel);
	bool RemoveSkill(SPSkillActivity* pkSkillActivity);
	bool RemoveSkill(int iID);

	bool AddSkill(SPItemSkillActivity* pkItemSkillActivity);
	bool RemoveSkill(int iSkillID, int iItemID, int iContainerType, int iSlotIndex);

	bool UsedSkill(SPSkillActivity* pkSkillActivity);
	bool RemoveUsedSkill(SPSkill* pkSkill);

	bool UsedMotion(SPMotionUsed* pkMotionUsed);
	bool RemoveUsedMotion(unsigned int uiAniIndex);

	SPSkillActivity* GetSkillActivity(int iID);
	SPSkillActivity* GetSkillActivity(SPSkill* pkSkill);
	
	SPSkillActivity* GetFirstSkillActivity(SKILL_INVENTORY_TYPE eType = SIT_NULL);
	SPSkillActivity* GetNextSkillActivity(SKILL_INVENTORY_TYPE eType = SIT_NULL);
	int GetSkillCount(SKILL_INVENTORY_TYPE eType = SIT_NULL);

	SPItemSkillActivity* GetItemSkillActivity(int iSkillID, int iContainerType, int iSlotIndex);
	SPItemSkillActivity* GetItemSkillActivity(int iIndex);
	int GetItemSkillCount();
	
	SPSkillActivity* GetLoveSkillActivity( int iSkillID );

	bool IsPossibleUsed(int iID);

	void ReInitSkill( UINT32 uiSaveCheck );
	void SetToggleSkill(int iEffectID, bool bOn);

protected:
	bool IsExistSkill(SPSkill* pkSkill);
	bool IsExistSkill(int iID);

	bool IsExistUsedSkill(SPSkill* pkSkill);
	bool IsExistUsedMotion(SPMotionStatus* pkMotion);

	SPSkillActivity* GetCurSkillActivity(SKILL_INVENTORY_TYPE eType);

protected:
	int m_iCurSkillIndex;
	int m_iTotalSkillCount[SIT_NUM];

	SPGameObject* m_pkParent;
	std::map< int, SPSkillActivity* > m_mSkillList;

	std::map< int, SPSkillActivity* > m_mUsedSkillList;
	std::map< int, SPMotionUsed* > m_mUsedMotionList;

	std::vector< SPItemSkillActivity* > m_vItemSkillList;

	std::map< int, SPSkillActivity* > m_mLoveSkillList;
};

#endif