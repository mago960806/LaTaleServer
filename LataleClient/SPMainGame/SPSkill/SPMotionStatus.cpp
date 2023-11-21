#include <vector>
#include "SPCommon.h"
#include "SPSkill.h"
#include "SPEffect.h"
#include "SPMotionStatus.h"

using namespace std;
//------------------------------------------------------------------------------------
// SPMotionStatus
SPMotionStatus::SPMotionStatus(MOTION_STATUS_TYPE eType)
: m_eType(eType)
{
	Init();
}

//------------------------------------------------------------------------------------
SPMotionStatus::~SPMotionStatus()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPMotionStatus::Clear()
{
	m_vRequireStatList.clear();
	m_vConsumptionStatList.clear();
}

//------------------------------------------------------------------------------------
void SPMotionStatus::Init()
{
	ZeroMemory(&m_ApplyEffectInfo, sizeof(SKILL_APPLY_EFFECT));
	ZeroMemory(&m_ActivityVelocity, sizeof(SKILL_ACTIVITY_VELOCITY));
}

//------------------------------------------------------------------------------------
void SPMotionStatus::SetActivityVelocity(const SKILL_ACTIVITY_VELOCITY& Src)
{
	m_ActivityVelocity = Src;
}

//------------------------------------------------------------------------------------
void SPMotionStatus::SetEffectValue(const SKILL_APPLY_EFFECT& Src)
{
	m_ApplyEffectInfo = Src;
}

//------------------------------------------------------------------------------------
void SPMotionStatus::AddRequireStat(const MOTION_STATUS_STAT& Src)
{
	m_vRequireStatList.push_back(Src);
}

//------------------------------------------------------------------------------------
void SPMotionStatus::AddConsumptionStat(const MOTION_STATUS_STAT& Src)
{
	m_vConsumptionStatList.push_back(Src);
}

//------------------------------------------------------------------------------------
void SPMotionStatus::SetConditionCheckTime(float fTime)
{
	m_fConditionCheckTime = fTime;
}

//------------------------------------------------------------------------------------
void SPMotionStatus::SetComboInfo(const SKILL_COMBO& Src)
{
	m_ComboInfo = Src;
}

//------------------------------------------------------------------------------------
MOTION_STATUS_STAT* SPMotionStatus::GetRequireStatList(int iIndex)
{
	if( iIndex < 0 || iIndex >= (int)m_vRequireStatList.size() )
		return NULL;

	vector< MOTION_STATUS_STAT >::iterator iter = m_vRequireStatList.begin() + iIndex;
	return &(*iter);
}

//------------------------------------------------------------------------------------
MOTION_STATUS_STAT* SPMotionStatus::GetConsumptionStatList(int iIndex)
{
	if( iIndex < 0 || iIndex >= (int)m_vConsumptionStatList.size() )
		return NULL;

	vector< MOTION_STATUS_STAT >::iterator iter = m_vConsumptionStatList.begin() + iIndex;
	return &(*iter);
}

//------------------------------------------------------------------------------------
SKILL_APPLY_EFFECT* SPMotionStatus::GetApplyEffect()
{
	return &m_ApplyEffectInfo;
}

//------------------------------------------------------------------------------------
SKILL_ACTIVITY_VELOCITY* SPMotionStatus::GetActivityVelocity()
{
	return &m_ActivityVelocity;
}

//------------------------------------------------------------------------------------
float SPMotionStatus::GetConditionCheckTime()
{
	return m_fConditionCheckTime;
}

//------------------------------------------------------------------------------------
SKILL_COMBO* SPMotionStatus::GetComboInfo()
{
	return &m_ComboInfo;
}

//------------------------------------------------------------------------------------
int SPMotionStatus::GetRequireStatListCount()
{
	return (int)m_vRequireStatList.size();
}


//------------------------------------------------------------------------------------
// SPActionInterface
SPActionInterface::SPActionInterface(int iActionID, int iAction, int iMotionType,
	SKILL_TYPE eSkillType, std::string strName)
: m_iActionID(iActionID)
, m_iActionCommand(iAction)
, m_iMotionType(iMotionType)
, m_eSkillType(eSkillType)
, m_strName(strName)
{
}

//------------------------------------------------------------------------------------
SPActionInterface::~SPActionInterface()
{
}

//------------------------------------------------------------------------------------
void SPActionInterface::Init()
{
	m_iMotionType = 0;
	m_iActionCommand = 0;
	m_iMotionType = 0;

	m_strName = "";
}

//------------------------------------------------------------------------------------
int SPActionInterface::GetActionID()
{
	return m_iActionID;
}

//------------------------------------------------------------------------------------
int SPActionInterface::GetActionCommand()
{
	return m_iActionCommand;
}

//------------------------------------------------------------------------------------
int SPActionInterface::GetMotionType()
{
	return m_iMotionType;
}

//------------------------------------------------------------------------------------
void SPActionInterface::SetUIInfo(const SKILL_UI_INFO& Src)
{
	m_UIInfo = Src;
}

//------------------------------------------------------------------------------------
SKILL_UI_INFO* SPActionInterface::GetUIInfo()
{
	return &m_UIInfo;
}

//------------------------------------------------------------------------------------
const char* SPActionInterface::GetName()
{
	return m_strName.c_str();
}

//------------------------------------------------------------------------------------
SKILL_TYPE* SPActionInterface::GetType()
{
	return &m_eSkillType;
}


//------------------------------------------------------------------------------------
SPAttackCombo::SPAttackCombo(int iNextNormalNo, int iNextHeavyNo)
: m_iNextNormalNo(iNextNormalNo)
, m_iNextHeavyNo(iNextHeavyNo)
{
}

//------------------------------------------------------------------------------------
SPAttackCombo::~SPAttackCombo()
{
}

//------------------------------------------------------------------------------------
int SPAttackCombo::GetNextNo(int iAttackType)
{
	int iRet = 0;
	switch( iAttackType )
	{
	case 1:
		iRet = m_iNextNormalNo;
		break;
	case 2:
		iRet = m_iNextHeavyNo;
		break;
	}
	
	return iRet;
}