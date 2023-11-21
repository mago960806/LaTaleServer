
#include <vector>
#include <string>
#include "SPSkill.h"

using namespace std;
//------------------------------------------------------------------------------------
// SPSkillUpRequire
SPSkillUpRequire::SPSkillUpRequire()
{
}

//------------------------------------------------------------------------------------
SPSkillUpRequire::SPSkillUpRequire(const SPSkillUpRequire& kSrc)
{
	m_iSkillPoint = kSrc.m_iSkillPoint;
	m_iSkillPointSlv = kSrc.m_iSkillPointSlv;

	m_vRequireStateList = kSrc.m_vRequireStateList;
}

//------------------------------------------------------------------------------------
SPSkillUpRequire::~SPSkillUpRequire()
{
	m_vRequireStateList.clear();
}

//------------------------------------------------------------------------------------
SPSkillUpRequire& SPSkillUpRequire::operator = (const SPSkillUpRequire& kSrc)
{
	m_iSkillPoint = kSrc.m_iSkillPoint;
	m_iSkillPointSlv = kSrc.m_iSkillPointSlv;

	m_vRequireStateList = kSrc.m_vRequireStateList;
	return *this;
}




//------------------------------------------------------------------------------------
// SPSkillActivityRequire
SPSkillActivityRequire::SPSkillActivityRequire()
{
}

//------------------------------------------------------------------------------------
SPSkillActivityRequire::SPSkillActivityRequire(const SPSkillActivityRequire& kSrc)
{
	m_iEquipType = kSrc.m_iEquipType;
	m_iItemKind = kSrc.m_iItemKind;
	m_iMotionType = kSrc.m_iMotionType;
	m_iStandCheck = kSrc.m_iStandCheck;

	m_vRequireStateList = kSrc.m_vRequireStateList;
}

//------------------------------------------------------------------------------------
SPSkillActivityRequire::~SPSkillActivityRequire()
{
	m_vRequireStateList.clear();
}

//------------------------------------------------------------------------------------
SPSkillActivityRequire& SPSkillActivityRequire::operator = (const SPSkillActivityRequire& kSrc)
{
	m_iEquipType = kSrc.m_iEquipType;
	m_iItemKind = kSrc.m_iItemKind;
	m_iMotionType = kSrc.m_iMotionType;
	m_iStandCheck = kSrc.m_iStandCheck;

	m_vRequireStateList = kSrc.m_vRequireStateList;
	return *this;
}




//------------------------------------------------------------------------------------
// SPSkill
SPSkill::SPSkill()
{
	Init();
}

//------------------------------------------------------------------------------------
SPSkill::SPSkill( int iID, SKILL_TYPE eType, int iBaseLevel, int iMaxLevel,int iSaveCheck, std::string strName )
{
	Init();

	m_iID = iID;
	m_eType = eType;
	m_iBaseLevel = iBaseLevel;
	m_iMaxLevel = iMaxLevel;
	m_iSaveCheck = iSaveCheck;
	m_strName = strName;
}

//------------------------------------------------------------------------------------
SPSkill::~SPSkill()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPSkill::Init()
{
	m_iID = 0;
	m_eType = ST_NULL;
	m_iBaseLevel = 0;
	m_iMaxLevel = 0;
	m_iUpgradeSkillLevel = 0;
	m_iUpgradeSkillID = 0;
	m_iSaveCheck = 0;

	m_fDelayTime = 0.0f;

	m_strName = "";

	m_iCoolTimeID = 0;
	
	m_eSpecialType = SST_NULL;
	m_iToggleEffectID = 0;
	m_iPetActionSeq = 0;
	m_bLearnSkill = false;
	m_iRequirePet = 0;
}

//------------------------------------------------------------------------------------
void SPSkill::Clear()
{
	m_vMotionList.clear();
	m_vCoolTimeList.clear();
	m_vConsumptionStatList.clear();
}

//------------------------------------------------------------------------------------
SKILL_MOTION* SPSkill::GetMotion(bool bBuckler)
{
	int iSize = (int)m_vMotionList.size();
	if( iSize == 0 )
		return NULL;

	int iIndex = 0;
	if( bBuckler && iSize > 1 )
		iIndex = 1;

	vector< SKILL_MOTION >::iterator iter = m_vMotionList.begin();
	iter += iIndex;

	return &(*iter);
}