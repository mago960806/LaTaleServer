#ifndef __SPSKILL_H__
#define __SPSKILL_H__

#define MAX_EFFECT_NUM		4

enum SKILL_TYPE
{
	ST_NULL = 0, ST_ACTIVE, ST_TOGGLE, ST_PASSIVE
};

enum SKILL_INVENTORY_TYPE
{
	SIT_NULL = 0, SIT_ACTION, SIT_WEAPON, SIT_MAGIC, SIT_ETC, SIT_EMOTICON, SIT_PET, SIT_ITEMMIX, SIT_NEWPET, SIT_NUM
};

enum SKILL_SPECIAL_TYPE
{
	SST_NULL = 0,
	SST_AUTO_LOOTING, 
	SST_ITEM_SELL, 
	SST_AUTO_ITEM_USED, 
	SST_PET_CHAT,
	SST_PET_SHOP,
	SST_PET_QICHONG,
	SST_NUM
};

enum SKILL_TARGET_TYPE
{
	STT_NULL = 0,
	STT_ENEMY,
	STT_NEUTRALITY,
	STT_TEAM,
	STT_BOTH
};

enum SKILL_SAVE_CHECK
{
	SSC_NULL	=	0	,
	SSC_SKILL			,
	SSC_ITEMMIX			,
};

struct SKILL_MOTION
{
	int iSeqKeyIndex;
	int iSoundKeyIndex;
	int iFXKeyIndex;
};

struct SKILL_REQUIRE_STAT
{
	int iType;
	int	iID;			// AJJIYA [11/9/2005]
	int iValue;
	int iValueSlv;
	int	iValue2;		// AJJIYA [11/9/2005]
	int iValueSlv2;		// AJJIYA [11/9/2005]
};

struct SKILL_ACTIVITY_VELOCITY
{
	float fVelocityX;
	float fVelocityY;
	float fTime;
	float fDelayTime;
};

struct SKILL_COOLTIME
{
	int iApplyID;
	float fTime;
	float fTimeSlv;
};

struct SKILL_COMBO
{
	bool bComboCheck;
	
	int iSelfComboID;
	int iApplyComboID;
	
	float fComboMinTime;
	float fComboMaxTime;	
};

struct SKILL_UI_INFO
{
	int	iID;
	int iIcon;
	int iIconIndex;
	bool bQuickSlotUse;
	std::string strDescription;
	SKILL_INVENTORY_TYPE eInventoryType;

	int iDisableIcon;
	int iDisableIconIndex;
	int iDisableAlpha;
	std::string strDisableDescription;
	int	iSubID;
	int iSubIndex;
};

struct SKILL_APPLY_EFFECT
{
	SKILL_TARGET_TYPE eTargetType;
	int iTargetNum;
	int iTargetSlvNum;

	float fDelayTime[MAX_EFFECT_NUM];
	int iSelfEffect[MAX_EFFECT_NUM];
	int iTargetEffect[MAX_EFFECT_NUM];
};

class SPSkillUpRequire
{
public:
	SPSkillUpRequire();
	SPSkillUpRequire(const SPSkillUpRequire& kSrc);
	~SPSkillUpRequire();

	SPSkillUpRequire& operator = (const SPSkillUpRequire& kSrc);

	int m_iSkillPoint;
	int m_iSkillPointSlv;

	std::vector< SKILL_REQUIRE_STAT > m_vRequireStateList;
};

class SPSkillActivityRequire
{
public:
	SPSkillActivityRequire();
	SPSkillActivityRequire(const SPSkillActivityRequire& kSrc);
	~SPSkillActivityRequire();

	SPSkillActivityRequire& operator = (const SPSkillActivityRequire& kSrc);

	int m_iEquipType;
	int m_iItemKind;
	__int64 m_iMotionType;
	int m_iStandCheck;

	std::vector< SKILL_REQUIRE_STAT > m_vRequireStateList;
};


class SPSkill
{
public:
	SPSkill();
	SPSkill(int iID, SKILL_TYPE eType, int iBaseLevel, int iMaxLevel,int iSaveCheck, std::string strName);
	virtual ~SPSkill();

	void Init();
	void Clear();

	void SetID(int iID);
	void SetType(SKILL_TYPE eType);
	void SetLevel(int iBaseLevel, int iMaxLevel);
	void SetUpgradeSkill(int iLevel, int iID);
	void SetSaveCheck(int iSaveCheck);
	void SetName(std::string strName);
	void SetActivityVelocity(const SKILL_ACTIVITY_VELOCITY& Src);
	void SetUpRequire(const SPSkillUpRequire& kSrc);
	void SetAcitivityRequire(const SPSkillActivityRequire& kSrc);
	void SetUIInfo(const SKILL_UI_INFO& kSrc);
	void SetApplyEffect(const SKILL_APPLY_EFFECT& kSrc);
	void SetCombo(const SKILL_COMBO& kSrc);
	
	void SetCoolTimeID(int iCheckID);
	void AddApplyCoolTime(const SKILL_COOLTIME& Src);
	void AddConsumptionStat(const SKILL_REQUIRE_STAT& Src);

	void AddMotion(const SKILL_MOTION& Src);

	void SetSpecialSkill(SKILL_SPECIAL_TYPE eType, int iToggleID);
	void SetPetActionSeq(int iActionSeq);
	void SetLearnSkill( bool bLearnSkill );

	int GetID();
	const char* GetName();
	SKILL_MOTION* GetMotion(bool bBuckler);
	int GetUpgradeSkillLevel();
	int GetUpgradeSkillID();
	int GetCoolTimeID();

	SKILL_ACTIVITY_VELOCITY* GetActivityVelocity();
	SPSkillActivityRequire* GetActivityRequire();
	SPSkillUpRequire* GetUpRequre();
	SKILL_UI_INFO* GetUIInfo();
	SKILL_APPLY_EFFECT* GetApplyEffect();
	SKILL_COMBO* GetCombo();
	SKILL_TYPE GetType();

	int GetCoolTimeSize();
	SKILL_COOLTIME* GetCoolTime(int iIndex);

	int GetConsumptionStatSize();
	SKILL_REQUIRE_STAT* GetConsumptionState(int iIndex);

	int GetBaseLevel();
	int GetMaxLevel();

	int GetSaveCheck();

	SKILL_SPECIAL_TYPE GetSpecialType();
	int GetToggleEffectID();
	int GetPetActionSeq();
	bool GetLearnSkill();

	void SetRequirePet( int iRequirePet )
	{
		m_iRequirePet = iRequirePet;
	}
	int GetRequirePet( void )
	{
		return m_iRequirePet;
	}

protected:

protected:
	int m_iID;
	int m_iBaseLevel;
	int m_iMaxLevel;
	int m_iUpgradeSkillLevel;
	int m_iUpgradeSkillID;
	int m_iSaveCheck;

	SKILL_SPECIAL_TYPE m_eSpecialType;
	int m_iToggleEffectID;
	int m_iPetActionSeq;

	SKILL_TYPE m_eType;
	float m_fDelayTime;
	std::string m_strName;

	int m_iCoolTimeID;
	std::vector< SKILL_COOLTIME > m_vCoolTimeList;
	std::vector< SKILL_REQUIRE_STAT > m_vConsumptionStatList;

	SKILL_ACTIVITY_VELOCITY m_ActivityVelocity;
	SPSkillUpRequire m_kUpRequire;
	SPSkillActivityRequire m_kActivityRequire;
	SKILL_UI_INFO m_UIInfo;
	SKILL_APPLY_EFFECT m_ApplyEffect;
	SKILL_COMBO m_Combo;

	std::vector< SKILL_MOTION > m_vMotionList;

	bool m_bLearnSkill;
	int	m_iRequirePet;
};

#include "SPSkill.inl"

#endif
