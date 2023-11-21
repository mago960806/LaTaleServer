#ifndef __SPEFFECT_ARCHIVE_H__
#define __SPEFFECT_ARCHIVE_H__

class SPEffect;
class SPGameObject;

class SPEffectActivity
{
public:
	SPEffectActivity();
	SPEffectActivity(SPEffect* pkEffect, int iLevel, float fApplyTime, bool bCaster);
	SPEffectActivity(int iID, int iLevel, float fApplyTime, bool bCaster);
	~SPEffectActivity();

	void Init();
	void EffectApplyInit();
	void Process(float fElapsedTime);

	int GetLevel();
	int GetApplyEffect();
	SPEffect* GetEffect();
	float GetApplyTime();
	float GetAccmulateTime();
	
	bool IsCaster();
	
	void SetUniqueKey( unsigned int uiUniqueKey );
	unsigned int GetUniqueKey();

protected:
	SPEffect* m_pkEffect;

	int m_iLevel;
	int m_iApplyEffect;
	bool m_bCaster;

	float m_fApplyTime;
	float m_fAccmulateTime;
	float m_fEffectAccmulateTime;

	unsigned int m_uiUniqueKey;
};

class SPEffectArchive
{
public:
	SPEffectArchive(SPGameObject* pkParent);
	~SPEffectArchive();

	void Clear();
	void Process(float fElapsedTime);

	bool AddEffect(unsigned int uiID, SPEffectActivity* pkEffectActivity, bool bTargetFX = true);
	bool RemoveEffect(SPEffectActivity* pkEffectActivity);
	bool RemoveEffect(unsigned int uiID);

	static STATUS_TYPE GetStatus(EFFECT_STATUS_KIND_TYPE eType);

	SPEffectActivity*	GetFirstEffectActivity();
	SPEffectActivity*	GetNextEffectActivity();
	SPEffectActivity*	GetUniqueKeyActivity( unsigned int uiUniqueKey );
	
	bool IsExistEffectActivity(int iEffectID);
 
protected:

	void ApplyEffect(SPEffectActivity* pkEffectActivity);

	void ApplyStatus(SPEffect* pkEffect, int iLevel);
	STATUS_TYPE ApplyEffectValue(SPGOBStatus* pkGOBStatus,
		EFFECT_STATUS* pEffectStatus, int iLevel, int& iValue, bool& bAddValue);

	void ApplyFXSound(SPEffect* pkEffect);

	SPEffectActivity*	GetCurEffectActivity();


protected:

	SPGameObject* m_pkParent;
	stdext::hash_multimap< unsigned int, SPEffectActivity* > m_hmEffectList;

	int			m_iCurEffectIndex;
};

#endif