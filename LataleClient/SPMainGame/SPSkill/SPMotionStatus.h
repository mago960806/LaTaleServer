#ifndef __SPMOTION_STATUS_H__
#define __SPMOTION_STATUS_H__

enum EFFECT_STATUS_KIND_TYPE;
struct SKILL_APPLY_EFFECT;
struct SKILL_ACTIVITY_VELOCITY;

enum MOTION_STATUS_TYPE
{
	MST_NULL = 0, MST_ACTIVIE, MST_TOGGLE
};

struct MOTION_STATUS_STAT
{
	EFFECT_STATUS_KIND_TYPE eType;
	int	iID;
	int iValue;
	int	iValue2;
};

class SPMotionStatus
{
public:
	SPMotionStatus(MOTION_STATUS_TYPE eType);
	~SPMotionStatus();

	void Init();
	void Clear();

	void SetActivityVelocity(const SKILL_ACTIVITY_VELOCITY& Src);
	void SetEffectValue(const SKILL_APPLY_EFFECT& Src);
	void AddRequireStat(const MOTION_STATUS_STAT& Src);
	void AddConsumptionStat(const MOTION_STATUS_STAT& Src);
	void SetConditionCheckTime(float fTime);
	void SetComboInfo(const SKILL_COMBO& Src);

	MOTION_STATUS_STAT* GetRequireStatList(int iIndex);
	MOTION_STATUS_STAT* GetConsumptionStatList(int iIndex);
	SKILL_APPLY_EFFECT* GetApplyEffect();
	SKILL_ACTIVITY_VELOCITY* GetActivityVelocity();
	float GetConditionCheckTime();
	SKILL_COMBO* GetComboInfo();

	int GetRequireStatListCount();

protected:
	MOTION_STATUS_TYPE m_eType;
	
	float m_fConditionCheckTime;
	//float m_fEffectDelayTime;
	//int m_iEffectID[4];

	SKILL_APPLY_EFFECT m_ApplyEffectInfo;
	SKILL_ACTIVITY_VELOCITY m_ActivityVelocity;
	SKILL_COMBO m_ComboInfo;

	std::vector< MOTION_STATUS_STAT > m_vRequireStatList;
	std::vector< MOTION_STATUS_STAT > m_vConsumptionStatList;
};


class SPActionInterface
{
public:
	SPActionInterface(int iActionID, int iAction, int iMotionType, SKILL_TYPE eSkillType, std::string strName);
	~SPActionInterface();
	
	void Init();

	int GetActionID();
	int GetActionCommand();
	int GetMotionType();
	
	void SetUIInfo(const SKILL_UI_INFO& Src);
	SKILL_UI_INFO* GetUIInfo();

	const char* GetName();
	SKILL_TYPE* GetType();

protected:
	
	int	m_iActionID;
	int m_iActionCommand;
	int m_iMotionType;
	
	std::string m_strName;

	SKILL_TYPE m_eSkillType;
	SKILL_UI_INFO m_UIInfo;
};

class SPAttackCombo
{
public:
	SPAttackCombo(int iNextNormalNo, int iNextHeavyNo);
	~SPAttackCombo();

	int GetNextNo(int iAttackType = 1);

protected:
	int m_iNextNormalNo;
	int m_iNextHeavyNo;
};

#endif
