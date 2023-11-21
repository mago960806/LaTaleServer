
//------------------------------------------------------------------------------------
inline void SPSkill::SetID(int iID)
{
	m_iID = iID;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetType(SKILL_TYPE eType)
{
	m_eType = eType;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetLevel(int iBaseLevel, int iMaxLevel)
{
	m_iBaseLevel = iBaseLevel;
	m_iMaxLevel = iMaxLevel;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetUpgradeSkill(int iLevel, int iID)
{
	m_iUpgradeSkillLevel = iLevel;
	m_iUpgradeSkillID = iID;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetSaveCheck(int iSaveCheck)
{
	m_iSaveCheck = iSaveCheck;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetName(std::string strName)
{
	m_strName = strName;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetActivityVelocity(const SKILL_ACTIVITY_VELOCITY& Src)
{
	m_ActivityVelocity = Src;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetUpRequire(const SPSkillUpRequire& kSrc)
{
	m_kUpRequire = kSrc;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetAcitivityRequire(const SPSkillActivityRequire& kSrc)
{
	m_kActivityRequire = kSrc;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetUIInfo(const SKILL_UI_INFO& kSrc)
{
	m_UIInfo = kSrc;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetApplyEffect(const SKILL_APPLY_EFFECT& kSrc)
{
	m_ApplyEffect = kSrc;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetCombo(const SKILL_COMBO& kSrc)
{
	m_Combo = kSrc;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetCoolTimeID(int iCheckID)
{
	m_iCoolTimeID = iCheckID;
}

//------------------------------------------------------------------------------------
inline void SPSkill::AddApplyCoolTime(const SKILL_COOLTIME& Src)
{
	m_vCoolTimeList.push_back(Src);
}

//------------------------------------------------------------------------------------
inline void SPSkill::AddConsumptionStat(const SKILL_REQUIRE_STAT& Src)
{
	m_vConsumptionStatList.push_back(Src);
}

//------------------------------------------------------------------------------------
inline void SPSkill::AddMotion(const SKILL_MOTION& Src)
{
	m_vMotionList.push_back(Src);
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetSpecialSkill(SKILL_SPECIAL_TYPE eType, int iToggleID)
{
	m_eSpecialType = eType;
	m_iToggleEffectID = iToggleID;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetPetActionSeq(int iActionSeq)
{
	m_iPetActionSeq = iActionSeq;
}

//------------------------------------------------------------------------------------
inline void SPSkill::SetLearnSkill( bool bLearnSkill )
{
	m_bLearnSkill = bLearnSkill;
}

//------------------------------------------------------------------------------------
inline int SPSkill::GetID()
{
	return m_iID;
}

//------------------------------------------------------------------------------------
inline const char* SPSkill::GetName()
{
	return m_strName.c_str();
}

//------------------------------------------------------------------------------------
inline int SPSkill::GetUpgradeSkillLevel()
{
	return m_iUpgradeSkillLevel;
}

//------------------------------------------------------------------------------------
inline int SPSkill::GetUpgradeSkillID()
{
	return m_iUpgradeSkillID;
}

//------------------------------------------------------------------------------------
inline int SPSkill::GetCoolTimeID()
{
	return m_iCoolTimeID;
}

//------------------------------------------------------------------------------------
inline SKILL_ACTIVITY_VELOCITY* SPSkill::GetActivityVelocity()
{
	return &m_ActivityVelocity;
}

//------------------------------------------------------------------------------------
inline SPSkillActivityRequire* SPSkill::GetActivityRequire()
{
	return &m_kActivityRequire;
}

//------------------------------------------------------------------------------------
inline SKILL_UI_INFO* SPSkill::GetUIInfo()
{
	return &m_UIInfo;
}

//------------------------------------------------------------------------------------
inline SKILL_APPLY_EFFECT* SPSkill::GetApplyEffect()
{
	return &m_ApplyEffect;
}

//------------------------------------------------------------------------------------
inline SKILL_COMBO* SPSkill::GetCombo()
{
	return &m_Combo;
}

//------------------------------------------------------------------------------------
inline SKILL_TYPE SPSkill::GetType()
{
	return m_eType;
}

//------------------------------------------------------------------------------------
inline int SPSkill::GetCoolTimeSize()
{
	return (int)m_vCoolTimeList.size();
}

//------------------------------------------------------------------------------------
inline SKILL_COOLTIME* SPSkill::GetCoolTime(int iIndex)
{
	if( iIndex < 0 || iIndex >= GetCoolTimeSize() )
		return NULL;

	std::vector< SKILL_COOLTIME >::iterator iter = m_vCoolTimeList.begin() + iIndex;
	return &(*iter);
}

//------------------------------------------------------------------------------------
inline int SPSkill::GetConsumptionStatSize()
{
	return (int)m_vConsumptionStatList.size();
}

//------------------------------------------------------------------------------------
inline SKILL_REQUIRE_STAT* SPSkill::GetConsumptionState(int iIndex)
{
	if( iIndex < 0 || iIndex >= GetConsumptionStatSize() )
		return NULL;

	std::vector< SKILL_REQUIRE_STAT >::iterator iter = m_vConsumptionStatList.begin() + iIndex;
	return &(*iter);
}

//------------------------------------------------------------------------------------
inline int SPSkill::GetBaseLevel()
{
	return m_iBaseLevel;
}

//------------------------------------------------------------------------------------
inline int SPSkill::GetMaxLevel()
{
	return m_iMaxLevel;
}

//------------------------------------------------------------------------------------
inline int SPSkill::GetSaveCheck()
{
	return m_iSaveCheck;
}

//------------------------------------------------------------------------------------
inline SPSkillUpRequire* SPSkill::GetUpRequre()
{
	return &m_kUpRequire;
}

//------------------------------------------------------------------------------------
inline SKILL_SPECIAL_TYPE SPSkill::GetSpecialType()
{
	return m_eSpecialType;
}

//------------------------------------------------------------------------------------
inline int SPSkill::GetToggleEffectID()
{
	return m_iToggleEffectID;
}

//------------------------------------------------------------------------------------
inline int SPSkill::GetPetActionSeq()
{
	return m_iPetActionSeq;
}

//------------------------------------------------------------------------------------
inline bool SPSkill::GetLearnSkill()
{
	return m_bLearnSkill;
}
