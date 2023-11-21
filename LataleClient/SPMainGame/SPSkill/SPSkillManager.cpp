#include <vector>
#include <string>
#include <hash_map>

#include "SPCommon.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPSkill.h"
#include "SPMotionStatus.h"
#include "SPSkillManager.h"
#include "SPDebug.h"

using namespace std;
using namespace stdext;

SPSkillManager* SPSkillManager::m_pkInstance = NULL;
//------------------------------------------------------------------------------------
SPSkillManager* SPSkillManager::GetInstance()
{
	if( m_pkInstance == NULL )
		m_pkInstance = new SPSkillManager;

	return m_pkInstance;
}

//------------------------------------------------------------------------------------
void SPSkillManager::Release()
{
	if( m_pkInstance )
	{
		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}

//------------------------------------------------------------------------------------
SPSkillManager::SPSkillManager()
{
}

//------------------------------------------------------------------------------------
SPSkillManager::~SPSkillManager()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPSkillManager::Clear()
{
	hash_map< int, SPSkill* >::iterator iterSkill = m_hmSkillList.begin();
	while( iterSkill != m_hmSkillList.end() )
	{
		delete iterSkill->second;
		iterSkill++;
	}
	m_hmSkillList.clear();

	m_mSkillListFind.clear();
	m_mActionInterfaceListFind.clear();

	hash_map< int, SPMotionStatus* >::iterator iterMotion = m_hmMotionStatusList.begin();
	while( iterMotion != m_hmMotionStatusList.end() )
	{
		delete iterMotion->second;
		iterMotion++;
	}
	m_hmMotionStatusList.clear();

	iterMotion = m_hmMOBMotionStatusList.begin();
	while( iterMotion != m_hmMOBMotionStatusList.end() )
	{
		delete iterMotion->second;
		iterMotion++;
	}
	m_hmMOBMotionStatusList.clear();

	hash_map< int, SPActionInterface* >::iterator iterActionInterface = m_hmActionInterfaceList.begin();
	while( iterActionInterface != m_hmActionInterfaceList.end() )
	{
		delete iterActionInterface->second;
		iterActionInterface++;
	}
	m_hmActionInterfaceList.clear();

	hash_map< int, SPActionInterface* >::iterator iterNullActionInterface = m_hmNullActionInterfaceList.begin();
	while( iterNullActionInterface != m_hmNullActionInterfaceList.end() )
	{
		delete iterNullActionInterface->second;
		iterNullActionInterface++;
	}
	m_hmNullActionInterfaceList.clear();

	hash_map< int, SPAttackCombo* >::iterator iterAttackCombo = m_hmAttackComboList.begin();
	while( iterAttackCombo != m_hmAttackComboList.end() )
	{
		delete iterAttackCombo->second;
		iterAttackCombo++;
	}
	m_hmAttackComboList.clear();
}

//------------------------------------------------------------------------------------
bool SPSkillManager::Init()
{
#if defined(_DEBUG)
	DXUTOutputDebugString("SkillManager::Init()\n");
#endif

	if( !LoadLDTFile() )
		return false;

	if( !LoadLDTMotionFile() )
		return false;

	if( !LoadMOBMotionFile() )
		return false;

	if( !LoadLDTActionInterfaceFile() )
		return false;

	if( !LoadLDTAttackComboFile() )
		return false;

	m_iCurIndex = 0;
	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::LoadLDTFile()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/SKILL.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iItemID, i, j;
	char szBuf[128];

	LDT_Field LFD_Name, LFD_Type, LFD_MotionInfo1, LFD_Sound1, LFD_FX1;
	LDT_Field LFD_MotionInfo2, LFD_Sound2, LFD_FX2;
	LDT_Field LFD_BaseLevel, LFD_MaxLevel, LFD_GetLevel, LFD_GetID, LFD_Save;
	LDT_Field LFD_SpeedX, LFD_SpeedY, LFD_SpeedTime, LFD_SpeedDelayTime;
	LDT_Field LFD_Icon, LFD_IconIndex, LFD_Description, LFD_QuickSlotUse, LFD_InventoryType;
	LDT_Field LFD_UpgradeSP, LFD_UpgradeSPSlv, LFD_UpgradeStatType, LFD_UpgradeStatValue, LFD_UpgradeStatValueSlv;
	LDT_Field LFD_RequireEquipType, LFD_RequireItemKind, LFD_RequireMotion, LFD_StandCheck;
	LDT_Field LFD_RequireStat, LFD_RequireValue, LFD_RequireValueSlv;
	LDT_Field LFD_CoolTimeID, LFD_ApplyCoolTimeID, LFD_ApplyCoolTime, LFD_ApplyCoolTimeSlv;

	LDT_Field LFD_EffectDelay, LFD_EffectID, LFD_TargetType, LFD_TargetNum, LFD_TargetSlvNum;
	LDT_Field LFD_ComboCheck, LFD_ComboSelfID, LFD_ComboApplyID, LFD_ComboMinTime, LFD_ComboMaxTime;
	LDT_Field LFD_DisableIcon , LFD_DisableIconIndex , LFD_DisableAlpha , LFD_DisableDescription , LFD_SubID , LFD_SubIndex;
	LDT_Field LFD_SpecialType, LFD_ToggleID, LFD_PetAction, LFD_LearnSkill, LFD_RequirePet;
	
	SPSkill* pkSkill;

	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);

		pkLDTFile->GetFieldFromLable(iItemID, "_Name",			LFD_Name);
		pkLDTFile->GetFieldFromLable(iItemID, "_SkillType",		LFD_Type);
		pkLDTFile->GetFieldFromLable(iItemID, "_MotionInfo1",	LFD_MotionInfo1);
		pkLDTFile->GetFieldFromLable(iItemID, "_SoundEffect1",	LFD_Sound1);
		pkLDTFile->GetFieldFromLable(iItemID, "_FX1",			LFD_FX1);
		pkLDTFile->GetFieldFromLable(iItemID, "_MotionInfo1",	LFD_MotionInfo2);
		pkLDTFile->GetFieldFromLable(iItemID, "_SoundEffect2",	LFD_Sound2);
		pkLDTFile->GetFieldFromLable(iItemID, "_FX2",			LFD_FX2);
		pkLDTFile->GetFieldFromLable(iItemID, "_BaseSlv",		LFD_BaseLevel);
		pkLDTFile->GetFieldFromLable(iItemID, "_MaxSlv",		LFD_MaxLevel);
		pkLDTFile->GetFieldFromLable(iItemID, "_GetSkillLv",	LFD_GetLevel);
		pkLDTFile->GetFieldFromLable(iItemID, "_GetSkillID",	LFD_GetID);
		pkLDTFile->GetFieldFromLable(iItemID, "_SaveCheck",		LFD_Save);
		pkLDTFile->GetFieldFromLable(iItemID, "_Speed_Y",		LFD_SpeedY);
		pkLDTFile->GetFieldFromLable(iItemID, "_Speed_X",		LFD_SpeedX);
		pkLDTFile->GetFieldFromLable(iItemID, "_Speed_Time",	LFD_SpeedTime);
		pkLDTFile->GetFieldFromLable(iItemID, "_Speed_Delay",	LFD_SpeedDelayTime);

		pkLDTFile->GetFieldFromLable(iItemID, "_Icon",			LFD_Icon);
		pkLDTFile->GetFieldFromLable(iItemID, "_IconIndex",		LFD_IconIndex);
		pkLDTFile->GetFieldFromLable(iItemID, "_Description",	LFD_Description);
		pkLDTFile->GetFieldFromLable(iItemID, "_QuickSlotUse",	LFD_QuickSlotUse);
		pkLDTFile->GetFieldFromLable(iItemID, "_InventoryType",	LFD_InventoryType);

		pkLDTFile->GetFieldFromLable(iItemID, "_DisableIcon",		LFD_DisableIcon);
		pkLDTFile->GetFieldFromLable(iItemID, "_DisableIconIndex",	LFD_DisableIconIndex);
		pkLDTFile->GetFieldFromLable(iItemID, "_DisableAlpha",		LFD_DisableAlpha);
		pkLDTFile->GetFieldFromLable(iItemID, "_DisableDescription",LFD_DisableDescription);
		pkLDTFile->GetFieldFromLable(iItemID, "_SubID",				LFD_SubID);
		pkLDTFile->GetFieldFromLable(iItemID, "_SubIndex",			LFD_SubIndex);


		pkSkill = new SPSkill(iItemID, (SKILL_TYPE)LFD_Type.uData.lValue, LFD_BaseLevel.uData.lValue,
			LFD_MaxLevel.uData.lValue, LFD_Save.uData.lValue, LFD_Name.uData.pValue);

		pkSkill->SetUpgradeSkill(LFD_GetLevel.uData.lValue, LFD_GetID.uData.lValue);
		
		SKILL_MOTION SkillMotion;
		SkillMotion.iSeqKeyIndex = LFD_MotionInfo1.uData.lValue;
		SkillMotion.iSoundKeyIndex = LFD_Sound1.uData.lValue;
		SkillMotion.iFXKeyIndex = LFD_FX1.uData.lValue;
		pkSkill->AddMotion(SkillMotion);

		SkillMotion.iSeqKeyIndex = LFD_MotionInfo2.uData.lValue;
		SkillMotion.iSoundKeyIndex = LFD_Sound2.uData.lValue;
		SkillMotion.iFXKeyIndex = LFD_FX2.uData.lValue;
		pkSkill->AddMotion(SkillMotion);

		SKILL_ACTIVITY_VELOCITY Velocity;
		Velocity.fVelocityX = (float)LFD_SpeedX.uData.lValue;
		Velocity.fVelocityY = (float)LFD_SpeedY.uData.lValue;
		Velocity.fTime = (float)LFD_SpeedTime.uData.lValue / 100.0f;
		Velocity.fDelayTime = (float)LFD_SpeedDelayTime.uData.lValue / 100.0f;
		pkSkill->SetActivityVelocity(Velocity);

		SKILL_UI_INFO UIInfo;
		UIInfo.iID = iItemID;
		UIInfo.iIcon = LFD_Icon.uData.lValue;
		UIInfo.iIconIndex = LFD_IconIndex.uData.lValue;
		UIInfo.strDescription = LFD_Description.uData.pValue;
		UIInfo.bQuickSlotUse = LFD_QuickSlotUse.uData.lValue;
		UIInfo.eInventoryType = (SKILL_INVENTORY_TYPE)LFD_InventoryType.uData.lValue;
		UIInfo.iDisableIcon = LFD_DisableIcon.uData.lValue;
		UIInfo.iDisableIconIndex = LFD_DisableIconIndex.uData.lValue;
		UIInfo.iDisableAlpha = LFD_DisableAlpha.uData.lValue;

		if( LFD_DisableDescription.uData.pValue != NULL )
			UIInfo.strDisableDescription = LFD_DisableDescription.uData.pValue;

		UIInfo.iSubID = LFD_SubID.uData.lValue;
		UIInfo.iSubIndex = LFD_SubIndex.uData.lValue;

		pkSkill->SetUIInfo(UIInfo);

		pkLDTFile->GetFieldFromLable(iItemID, "_UpRequireSkillPoint",		LFD_UpgradeSP);
		pkLDTFile->GetFieldFromLable(iItemID, "_UpRequireSkillPointSlv",	LFD_UpgradeSPSlv);

		SPSkillUpRequire kUpRequire;
		kUpRequire.m_iSkillPoint = LFD_UpgradeSP.uData.lValue;
		kUpRequire.m_iSkillPointSlv = LFD_UpgradeSPSlv.uData.lValue;

		SKILL_REQUIRE_STAT StatInfo;
		vector< SKILL_REQUIRE_STAT > vRequireStat;

		LDT_Field LFD_UpgradeStatID , LFD_UpgradeStatValue2 , LFD_UpgradeStatValueSlv2;

		for( j = 0; j < 2; j++ )
		{
			sprintf(szBuf, "_UpRequire%d_Type", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_UpgradeStatType);
			sprintf(szBuf, "_UpRequire%d_ID", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_UpgradeStatID);

			sprintf(szBuf, "_UpRequire%d_Value1", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_UpgradeStatValue);
			sprintf(szBuf, "_UpRequire%d_ValueSlv1", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_UpgradeStatValueSlv);

			sprintf(szBuf, "_UpRequire%d_Value2", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_UpgradeStatValue2);
			sprintf(szBuf, "_UpRequire%d_ValueSlv2", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_UpgradeStatValueSlv2);

			StatInfo.iType = LFD_UpgradeStatType.uData.lValue;
			StatInfo.iID = LFD_UpgradeStatID.uData.lValue;
			StatInfo.iValue = LFD_UpgradeStatValue.uData.lValue;
			StatInfo.iValueSlv = LFD_UpgradeStatValueSlv.uData.lValue;
			StatInfo.iValue2 = LFD_UpgradeStatValue2.uData.lValue;
			StatInfo.iValueSlv2 = LFD_UpgradeStatValueSlv2.uData.lValue;

			vRequireStat.push_back(StatInfo);
		}

		kUpRequire.m_vRequireStateList = vRequireStat;
		pkSkill->SetUpRequire(kUpRequire);
		vRequireStat.clear();

		SPSkillActivityRequire kActivityRequire;

		pkLDTFile->GetFieldFromLable(iItemID, "_RequireEquipType",		LFD_RequireEquipType);
		pkLDTFile->GetFieldFromLable(iItemID, "_RequireItemKind",		LFD_RequireItemKind);
		pkLDTFile->GetFieldFromLable(iItemID, "_MotionStatusCheck",		LFD_RequireMotion);
		pkLDTFile->GetFieldFromLable(iItemID, "_StandCheck",			LFD_StandCheck);
		kActivityRequire.m_iEquipType = LFD_RequireEquipType.uData.lValue;
		kActivityRequire.m_iItemKind = LFD_RequireItemKind.uData.lValue;
		kActivityRequire.m_iStandCheck = LFD_StandCheck.uData.lValue;

		kActivityRequire.m_iMotionType = 0;
		if( LFD_RequireMotion.uData.pValue )
			kActivityRequire.m_iMotionType = _atoi64(LFD_RequireMotion.uData.pValue);

		LDT_Field LFD_RequireID, LFD_RequireValue2, LFD_RequireValueSlv2;

		for( j = 0; j < 2; j++ )
		{
			sprintf(szBuf, "_Require%d_Type", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireStat);
			sprintf(szBuf, "_Require%d_ID", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireID);

			sprintf(szBuf, "_Require%d_Value1", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireValue);
			sprintf(szBuf, "_Require%d_ValueSlv1", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireValueSlv);

			sprintf(szBuf, "_Require%d_Value2", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireValue2);
			sprintf(szBuf, "_Require%d_ValueSlv2", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireValueSlv2);

			StatInfo.iType = LFD_RequireStat.uData.lValue;
			StatInfo.iID = LFD_RequireID.uData.lValue;
			StatInfo.iValue = LFD_RequireValue.uData.lValue;
			StatInfo.iValueSlv = LFD_RequireValueSlv.uData.lValue;
			StatInfo.iValue2 = LFD_RequireValue2.uData.lValue;
			StatInfo.iValueSlv2 = LFD_RequireValueSlv2.uData.lValue;

			vRequireStat.push_back(StatInfo);
		}
		kActivityRequire.m_vRequireStateList = vRequireStat;
		pkSkill->SetAcitivityRequire(kActivityRequire);
		vRequireStat.clear();



		for( j = 0; j < 2; j++ )
		{
			sprintf(szBuf, "_ConsumptionStat%d", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireStat);
			sprintf(szBuf, "_ConsumptionStatValue%d", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireValue);
			sprintf(szBuf, "_ConsumptionStatValueSlv%d", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireValueSlv);

			StatInfo.iType = LFD_RequireStat.uData.lValue;
			StatInfo.iValue = LFD_RequireValue.uData.lValue;
			StatInfo.iValueSlv = LFD_RequireValueSlv.uData.lValue;

			pkSkill->AddConsumptionStat(StatInfo);
		}

		pkLDTFile->GetFieldFromLable(iItemID, "_ComboCheck",		LFD_ComboCheck);
		pkLDTFile->GetFieldFromLable(iItemID, "_Skill_ComboID",		LFD_ComboSelfID);
		pkLDTFile->GetFieldFromLable(iItemID, "_ComboApplyID",		LFD_ComboApplyID);
		pkLDTFile->GetFieldFromLable(iItemID, "_ComboTimeDelay",	LFD_ComboMinTime);
		pkLDTFile->GetFieldFromLable(iItemID, "_ComboTime",			LFD_ComboMaxTime);

		SKILL_COMBO ComboInfo;
		ComboInfo.bComboCheck = LFD_ComboCheck.uData.lValue;
		ComboInfo.iSelfComboID = LFD_ComboSelfID.uData.lValue;
		ComboInfo.iApplyComboID = LFD_ComboApplyID.uData.lValue;
		ComboInfo.fComboMinTime = (float)LFD_ComboMinTime.uData.lValue / 10.0f;
		ComboInfo.fComboMaxTime = (float)LFD_ComboMaxTime.uData.lValue / 10.0f;

		pkSkill->SetCombo(ComboInfo);

		pkLDTFile->GetFieldFromLable(iItemID,	"_Skill_CoolTimeID",	LFD_CoolTimeID);
		pkSkill->SetCoolTimeID(LFD_CoolTimeID.uData.lValue);

		SKILL_COOLTIME CoolTimeInfo;
		for( j = 0; j < 2; j++ )
		{
			sprintf(szBuf, "_CoolTimeApplyID%d", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_ApplyCoolTimeID);
			sprintf(szBuf, "_CoolTime%d", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_ApplyCoolTime);
			sprintf(szBuf, "_CoolTimeSLv%d", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_ApplyCoolTimeSlv);

			CoolTimeInfo.iApplyID = LFD_ApplyCoolTimeID.uData.lValue;
			CoolTimeInfo.fTime = (float)LFD_ApplyCoolTime.uData.lValue / 10.0f;
			CoolTimeInfo.fTimeSlv = (float)LFD_ApplyCoolTimeSlv.uData.lValue / 10.0f;

			pkSkill->AddApplyCoolTime(CoolTimeInfo);
		}

		SKILL_APPLY_EFFECT ApplyEffect;
		pkLDTFile->GetFieldFromLable(iItemID, "_TargetType",		LFD_TargetType);
		pkLDTFile->GetFieldFromLable(iItemID, "_TargetMaxCount",	LFD_TargetNum);
		pkLDTFile->GetFieldFromLable(iItemID, "_TargetMaxCountSlv",	LFD_TargetSlvNum);

		ApplyEffect.eTargetType = (SKILL_TARGET_TYPE)LFD_TargetType.uData.lValue;
		ApplyEffect.iTargetNum = (int)LFD_TargetNum.uData.lValue;
		ApplyEffect.iTargetSlvNum = (int)LFD_TargetNum.uData.lValue;

		for( j = 0; j < 4; j++ )
		{
			//sprintf(szBuf, "_EffectDelay%d", j + 1);
			//pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_EffectDelay);
			//ApplyEffect.fDelayTime[j] =  (float)LFD_EffectDelay.uData.lValue / 10.0f;

			//sprintf(szBuf, "_EffectID%d", j + 1);
			//pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_EffectID);
			//ApplyEffect.iSelfEffect[j] = (int)LFD_EffectID.uData.lValue;

			//sprintf(szBuf, "_TargetEffectID%d", j + 1);
			//pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_EffectID);
			//ApplyEffect.iTargetEffect[j] = (int)LFD_EffectID.uData.lValue;

			ApplyEffect.fDelayTime[j] = 0.0f;
			ApplyEffect.iSelfEffect[j] = 0;
			ApplyEffect.iTargetEffect[j] = 0;
		}
		pkSkill->SetApplyEffect(ApplyEffect);

		pkLDTFile->GetFieldFromLable(iItemID, "_Special_Skill",	LFD_SpecialType);
		pkLDTFile->GetFieldFromLable(iItemID, "_Toggle_Check",	LFD_ToggleID);
		pkSkill->SetSpecialSkill((SKILL_SPECIAL_TYPE)LFD_SpecialType.uData.lValue, LFD_ToggleID.uData.lValue);

		pkLDTFile->GetFieldFromLable(iItemID, "_Pet_Action", LFD_PetAction);
		pkSkill->SetPetActionSeq(LFD_PetAction.uData.lValue);

		pkLDTFile->GetFieldFromLable(iItemID, "_Learn_Skill", LFD_LearnSkill);
		pkSkill->SetLearnSkill( (bool)LFD_LearnSkill.uData.lValue );

		pkLDTFile->GetFieldFromLable(iItemID, "_Requirepet", LFD_RequirePet);
		pkSkill->SetRequirePet( LFD_RequirePet.uData.lValue );

		if( !AddSkill(iItemID, pkSkill) )
			delete pkSkill;
	}

	SAFE_RELEASE(pkLDTFile);
	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::LoadLDTMotionFile()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/MOTION_STATUS.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	SPMotionStatus* pkMotionStatus;

	int iDataNum, iItemID, i, j;
	int iEffectID[4];
	char szBuf[32];
	LDT_Field LFD_Type, LFD_SpeedX, LFD_SpeedY, LFD_SpeedTime, LFD_SpeedDelayTime;
	LDT_Field LFD_StatType , LFD_StatValue, LFD_ConditionCheck;
	LDT_Field LFD_RequireStatType , LFD_RequireStatID, LFD_RequireStatValue1, LFD_RequireStatValue2;
	LDT_Field LFD_EffectDelay, LFD_EffectID, LFD_TargetType, LFD_TargetNum;
	LDT_Field LFD_ComboSelfID, LFD_ComboApplyID, LFD_ComboMinTime, LFD_ComboMaxTime;

	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);

		pkLDTFile->GetFieldFromLable(iItemID, "_ActiveType",			LFD_Type);
		pkLDTFile->GetFieldFromLable(iItemID, "_Speed_X",				LFD_SpeedX);
		pkLDTFile->GetFieldFromLable(iItemID, "_Speed_Y",				LFD_SpeedY);
		pkLDTFile->GetFieldFromLable(iItemID, "_Speed_Time",			LFD_SpeedTime);
		pkLDTFile->GetFieldFromLable(iItemID, "_Speed_Delay",			LFD_SpeedDelayTime);

		pkMotionStatus = new SPMotionStatus((MOTION_STATUS_TYPE)LFD_Type.uData.lValue);
		
		SKILL_ACTIVITY_VELOCITY ActivityVelocity;
		ActivityVelocity.fVelocityX = (float)LFD_SpeedX.uData.lValue;
		ActivityVelocity.fVelocityY = (float)LFD_SpeedY.uData.lValue;
		ActivityVelocity.fTime = (float)LFD_SpeedTime.uData.lValue / 100.0f;
		ActivityVelocity.fDelayTime = (float)LFD_SpeedDelayTime.uData.lValue / 100.0f;
		pkMotionStatus->SetActivityVelocity(ActivityVelocity);

		pkLDTFile->GetFieldFromLable(iItemID, "_ConditionCheckTime",	LFD_ConditionCheck);
		pkMotionStatus->SetConditionCheckTime((float)LFD_ConditionCheck.uData.lValue / 10.0f);

		MOTION_STATUS_STAT StatusStat;

		for( j = 0 ; j < 2 ; j++ )
		{
			sprintf(szBuf, "_Require%d_Type", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireStatType);
			sprintf(szBuf, "_Require%d_ID", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireStatID);
			sprintf(szBuf, "_Require%d_Value1", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireStatValue1);
			sprintf(szBuf, "_Require%d_Value2", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_RequireStatValue2);

			StatusStat.eType	=	(EFFECT_STATUS_KIND_TYPE)LFD_RequireStatType.uData.lValue;
			StatusStat.iID		=	LFD_RequireStatID.uData.lValue;
			StatusStat.iValue	=	LFD_RequireStatValue1.uData.lValue;
			StatusStat.iValue2	=	LFD_RequireStatValue2.uData.lValue;

			pkMotionStatus->AddRequireStat( StatusStat );
		}

		StatusStat.iID		=	0;
		StatusStat.iValue2	=	0;

		pkLDTFile->GetFieldFromLable(iItemID, "_ConsumptionStat1",		LFD_StatType);
		pkLDTFile->GetFieldFromLable(iItemID, "_ConsumptionStatValue1",	LFD_StatValue);
		StatusStat.eType = (EFFECT_STATUS_KIND_TYPE)LFD_StatType.uData.lValue;
		StatusStat.iValue = LFD_StatValue.uData.lValue;
		pkMotionStatus->AddConsumptionStat(StatusStat);

		pkLDTFile->GetFieldFromLable(iItemID, "_ConsumptionStat2",		LFD_StatType);
		pkLDTFile->GetFieldFromLable(iItemID, "_ConsumptionStatValue2",	LFD_StatValue);
		StatusStat.eType = (EFFECT_STATUS_KIND_TYPE)LFD_StatType.uData.lValue;
		StatusStat.iValue = LFD_StatValue.uData.lValue;
		pkMotionStatus->AddConsumptionStat(StatusStat);

		SKILL_APPLY_EFFECT ApplyEffect;
		
		pkLDTFile->GetFieldFromLable(iItemID, "_TargetType",		LFD_TargetType);
		pkLDTFile->GetFieldFromLable(iItemID, "_TargetMaxCount",	LFD_TargetNum);

		ApplyEffect.eTargetType = (SKILL_TARGET_TYPE)LFD_TargetType.uData.lValue;
		ApplyEffect.iTargetNum = (int)LFD_TargetNum.uData.lValue;

		for( j = 0; j < 4; j++ )
		{
			//sprintf(szBuf, "_EffectDelay%d", j + 1);
			//pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_EffectDelay);
			//ApplyEffect.fDelayTime[j] =  (float)LFD_EffectDelay.uData.lValue / 10.0f;

			//sprintf(szBuf, "_EffectID%d", j + 1);
			//pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_EffectID);
			//ApplyEffect.iSelfEffect[j] = (int)LFD_EffectID.uData.lValue;

			//sprintf(szBuf, "_TargetEffectID%d", j + 1);
			//pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_EffectID);
			//ApplyEffect.iTargetEffect[j] = (int)LFD_EffectID.uData.lValue;

			ApplyEffect.fDelayTime[j] = 0.0f;
			ApplyEffect.iSelfEffect[j] = 0;
			ApplyEffect.iTargetEffect[j] = 0;
		}
		
		pkMotionStatus->SetEffectValue(ApplyEffect);

		pkLDTFile->GetFieldFromLable(iItemID, "_Motion_ComboID",	LFD_ComboSelfID);
		pkLDTFile->GetFieldFromLable(iItemID, "_ComboApplyID",		LFD_ComboApplyID);
		pkLDTFile->GetFieldFromLable(iItemID, "_ComboTimeDelay",	LFD_ComboMinTime);
		pkLDTFile->GetFieldFromLable(iItemID, "_ComboTime",			LFD_ComboMaxTime);

		SKILL_COMBO ComboInfo;
		ComboInfo.bComboCheck = false;
		ComboInfo.iSelfComboID = LFD_ComboSelfID.uData.lValue;
		ComboInfo.iApplyComboID = LFD_ComboApplyID.uData.lValue;
		ComboInfo.fComboMinTime = (float)LFD_ComboMinTime.uData.lValue / 10.0f;
		ComboInfo.fComboMaxTime = (float)LFD_ComboMaxTime.uData.lValue / 10.0f;

		pkMotionStatus->SetComboInfo(ComboInfo);
		
		if( !AddMotionStatus(iItemID, pkMotionStatus) )
			delete pkMotionStatus;
	}

	SAFE_RELEASE(pkLDTFile);
	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::LoadMOBMotionFile()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/MOB_MOTIONSTATUS.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	SPMotionStatus* pkMotionStatus;

	int iDataNum, iItemID, i, j;
	int iEffectID[4];
	char szBuf[32];
	LDT_Field LFD_Type, LFD_SpeedX, LFD_SpeedY, LFD_SpeedTime, LFD_SpeedDelayTime;
	LDT_Field LFD_StatType, LFD_StatValue;
	LDT_Field LFD_EffectDelay, LFD_EffectID, LFD_TargetType, LFD_TargetNum;

	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);

		pkLDTFile->GetFieldFromLable(iItemID, "_ActiveType",			LFD_Type);
		pkLDTFile->GetFieldFromLable(iItemID, "_Speed_X",				LFD_SpeedX);
		pkLDTFile->GetFieldFromLable(iItemID, "_Speed_Y",				LFD_SpeedY);
		pkLDTFile->GetFieldFromLable(iItemID, "_Speed_Time",			LFD_SpeedTime);
		pkLDTFile->GetFieldFromLable(iItemID, "_Speed_Delay",			LFD_SpeedDelayTime);

		pkMotionStatus = new SPMotionStatus((MOTION_STATUS_TYPE)LFD_Type.uData.lValue);

		SKILL_ACTIVITY_VELOCITY ActivityVelocity;
		ActivityVelocity.fVelocityX = (float)LFD_SpeedX.uData.lValue;
		ActivityVelocity.fVelocityY = (float)LFD_SpeedY.uData.lValue;
		ActivityVelocity.fTime = (float)LFD_SpeedTime.uData.lValue / 100.0f;
		ActivityVelocity.fDelayTime = (float)LFD_SpeedDelayTime.uData.lValue / 100.0f;
		pkMotionStatus->SetActivityVelocity(ActivityVelocity);

		MOTION_STATUS_STAT StatusStat;

		pkLDTFile->GetFieldFromLable(iItemID, "_RequireStat1",			LFD_StatType);
		pkLDTFile->GetFieldFromLable(iItemID, "_RequireStatValue1",		LFD_StatValue);
		StatusStat.eType = (EFFECT_STATUS_KIND_TYPE)LFD_StatType.uData.lValue;
		StatusStat.iValue = LFD_StatValue.uData.lValue;
		pkMotionStatus->AddRequireStat(StatusStat);

		pkLDTFile->GetFieldFromLable(iItemID, "_RequireStat2",			LFD_StatType);
		pkLDTFile->GetFieldFromLable(iItemID, "_RequireStatValue2",		LFD_StatValue);
		StatusStat.eType = (EFFECT_STATUS_KIND_TYPE)LFD_StatType.uData.lValue;
		StatusStat.iValue = LFD_StatValue.uData.lValue;
		pkMotionStatus->AddRequireStat(StatusStat);

		pkLDTFile->GetFieldFromLable(iItemID, "_ConsumptionStat1",		LFD_StatType);
		pkLDTFile->GetFieldFromLable(iItemID, "_ConsumptionStatValue1",	LFD_StatValue);
		StatusStat.eType = (EFFECT_STATUS_KIND_TYPE)LFD_StatType.uData.lValue;
		StatusStat.iValue = LFD_StatValue.uData.lValue;
		pkMotionStatus->AddConsumptionStat(StatusStat);

		pkLDTFile->GetFieldFromLable(iItemID, "_ConsumptionStat2",		LFD_StatType);
		pkLDTFile->GetFieldFromLable(iItemID, "_ConsumptionStatValue2",	LFD_StatValue);
		StatusStat.eType = (EFFECT_STATUS_KIND_TYPE)LFD_StatType.uData.lValue;
		StatusStat.iValue = LFD_StatValue.uData.lValue;
		pkMotionStatus->AddConsumptionStat(StatusStat);

		SKILL_APPLY_EFFECT ApplyEffect;
		pkLDTFile->GetFieldFromLable(iItemID, "_TargetType",		LFD_TargetType);
		pkLDTFile->GetFieldFromLable(iItemID, "_TargetMaxCount",	LFD_TargetNum);

		ApplyEffect.eTargetType = (SKILL_TARGET_TYPE)LFD_TargetType.uData.lValue;
		ApplyEffect.iTargetNum = (int)LFD_TargetNum.uData.lValue;

		for( j = 0; j < 4; j++ )
		{
			//sprintf(szBuf, "_EffectDelay%d", j + 1);
			//pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_EffectDelay);
			//ApplyEffect.fDelayTime[j] =  (float)LFD_EffectDelay.uData.lValue / 10.0f;

			//sprintf(szBuf, "_EffectID%d", j + 1);
			//pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_EffectID);
			//ApplyEffect.iSelfEffect[j] = (int)LFD_EffectID.uData.lValue;

			//sprintf(szBuf, "_TargetEffectID%d", j + 1);
			//pkLDTFile->GetFieldFromLable(iItemID,	szBuf,		LFD_EffectID);
			//ApplyEffect.iTargetEffect[j] = (int)LFD_EffectID.uData.lValue;

			ApplyEffect.fDelayTime[j] = 0.0f;
			ApplyEffect.iSelfEffect[j] = 0;
			ApplyEffect.iTargetEffect[j] = 0;
		}

		pkMotionStatus->SetEffectValue(ApplyEffect);

		if( !AddMOBMotionStatus(iItemID, pkMotionStatus) )
			delete pkMotionStatus;
	}

	SAFE_RELEASE(pkLDTFile);
	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::LoadLDTActionInterfaceFile()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/ACTION_INTERFACE.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	SPActionInterface* pkActionInterface;

	int iDataNum, iItemID, iActionCommand, iShiftCount, i;
	
	LDT_Field LFD_Name, LFD_ActionCommand, LFD_SkillType, LFD_MotionType;
	LDT_Field LFD_Icon, LFD_IconIndex, LFD_Description, LFD_QuickSlotUse, LFD_InventoryType;
	LDT_Field LFD_SubID , LFD_SubIndex;

	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);

		pkLDTFile->GetFieldFromLable(iItemID, "_Name",			LFD_Name);
		pkLDTFile->GetFieldFromLable(iItemID, "_ActionType",	LFD_ActionCommand);
		pkLDTFile->GetFieldFromLable(iItemID, "_ActiveType",	LFD_SkillType);
		pkLDTFile->GetFieldFromLable(iItemID, "_MotionType",	LFD_MotionType);

		pkLDTFile->GetFieldFromLable(iItemID, "_Icon",			LFD_Icon);
		pkLDTFile->GetFieldFromLable(iItemID, "_IconIndex",		LFD_IconIndex);
		pkLDTFile->GetFieldFromLable(iItemID, "_Description",	LFD_Description);
		pkLDTFile->GetFieldFromLable(iItemID, "_QuickSlotUse",	LFD_QuickSlotUse);
		pkLDTFile->GetFieldFromLable(iItemID, "_InventoryType",	LFD_InventoryType);

		pkLDTFile->GetFieldFromLable(iItemID, "_SubID",			LFD_SubID);
		pkLDTFile->GetFieldFromLable(iItemID, "_SubIndex",		LFD_SubIndex);

		if( LFD_ActionCommand.uData.lValue == 0 )
			iActionCommand = -1;
		else if( LFD_ActionCommand.uData.lValue == 1 )
			iActionCommand = 0;
		else
		{
			iShiftCount = LFD_ActionCommand.uData.lValue - 2;
			iActionCommand = 1 << iShiftCount;
		}

		// DUMP 상에서 char 형이 NULL 이 들어와서 임시로 막어본다.
		// AJJIYA [7/26/2006]

		if( LFD_Name.uData.pValue == NULL || LFD_Description.uData.pValue == NULL )
			continue;

		pkActionInterface = new SPActionInterface(iItemID ,iActionCommand, LFD_MotionType.uData.lValue,
			(SKILL_TYPE)LFD_SkillType.uData.lValue, LFD_Name.uData.pValue);

		SKILL_UI_INFO UIInfo;
		UIInfo.iID = iItemID;
		UIInfo.iIcon = LFD_Icon.uData.lValue;
		UIInfo.iIconIndex = LFD_IconIndex.uData.lValue;
		UIInfo.strDescription = LFD_Description.uData.pValue;
		UIInfo.bQuickSlotUse = LFD_QuickSlotUse.uData.lValue;
		UIInfo.eInventoryType = (SKILL_INVENTORY_TYPE)LFD_InventoryType.uData.lValue;
		UIInfo.iSubID = LFD_SubID.uData.lValue;
		UIInfo.iSubIndex = LFD_SubIndex.uData.lValue;
		pkActionInterface->SetUIInfo(UIInfo);

		if( !AddActionInterface(iItemID, pkActionInterface) )
			delete pkActionInterface;
	}

	SAFE_RELEASE(pkLDTFile);
	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::LoadLDTAttackComboFile()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/ATTACK_TREE.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iItemID, i;

	LDT_Field LFD_NAttack, LFD_HAttack;
	SPAttackCombo* pkAttackCombo;

	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);

		pkLDTFile->GetFieldFromLable(iItemID, "_Normal_Attack",		LFD_NAttack);
		pkLDTFile->GetFieldFromLable(iItemID, "_Heavy_Attack",		LFD_HAttack);
		
		pkAttackCombo = new SPAttackCombo(LFD_NAttack.uData.lValue, LFD_HAttack.uData.lValue);
		if( !AddAttackCombo(iItemID, pkAttackCombo) )
			delete pkAttackCombo;
	}

	SAFE_RELEASE(pkLDTFile);
	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::AddSkill(int iID, SPSkill* pkSkill)
{
	if( IsExistSkill(iID) )
		return false;

	m_hmSkillList.insert(hash_map< int, SPSkill* >::value_type(iID, pkSkill));

	if( pkSkill != NULL )
		AddSkillFind( pkSkill->GetUIInfo() , m_mSkillListFind );

	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::RemoveSkill(int iID)
{
	hash_map< int, SPSkill* >::iterator iter = m_hmSkillList.find(iID);
	if( iter == m_hmSkillList.end() )
		return false;

	SPSkill*	pSkill	=	(SPSkill*)( iter->second );

	if( pSkill != NULL )
		DelSkillFind( pSkill->GetUIInfo() , m_mSkillListFind );

	delete iter->second;
	m_hmSkillList.erase(iter);

	return true;
}

//------------------------------------------------------------------------------------
SPSkill* SPSkillManager::GetSkill(int iID)
{
	hash_map< int, SPSkill* >::iterator iter = m_hmSkillList.find(iID);
	if( iter == m_hmSkillList.end() )
		return NULL;

	return iter->second;
}

//------------------------------------------------------------------------------------
SPSkill* SPSkillManager::GetSkill(const char* pszName)
{
	SPSkill* pkSkill;
	if( pszName == NULL || (int)strlen(pszName) == 0 )
		return NULL;

	hash_map< int, SPSkill* >::iterator iter = m_hmSkillList.begin();
	while( iter != m_hmSkillList.end() )
	{
		pkSkill = iter->second;
		if( strcmp(pszName, pkSkill->GetName()) == 0 )
			return pkSkill;

		++iter;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::IsExistSkill(int iID)
{
	hash_map< int, SPSkill* >::iterator iter = m_hmSkillList.find(iID);
	if( iter == m_hmSkillList.end() )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::AddMotionStatus(int iID, SPMotionStatus* pkMotionStatus)
{
	if( IsExistMotionStatus(iID) )
		return false;

	m_hmMotionStatusList.insert(hash_map< int, SPMotionStatus* >::value_type(iID, pkMotionStatus));
	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::RemoveMotionStatus(int iID)
{
	hash_map< int, SPMotionStatus* >::iterator iter = m_hmMotionStatusList.find(iID);
	if( iter == m_hmMotionStatusList.end() )
		return false;

	delete iter->second;
	m_hmMotionStatusList.erase(iter);
	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::AddMOBMotionStatus(int iID, SPMotionStatus* pkMotionStatus)
{
	if( IsExistMOBMotionStatus(iID) )
		return false;

	m_hmMOBMotionStatusList.insert(hash_map< int, SPMotionStatus* >::value_type(iID, pkMotionStatus));
	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::IsExistMotionStatus(int iID)
{
	hash_map< int, SPMotionStatus* >::iterator iter = m_hmMotionStatusList.find(iID);
	if( iter == m_hmMotionStatusList.end() )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::IsExistMOBMotionStatus(int iID)
{
	hash_map< int, SPMotionStatus* >::iterator iter = m_hmMOBMotionStatusList.find(iID);
	if( iter == m_hmMOBMotionStatusList.end() )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
SPMotionStatus* SPSkillManager::GetMotionStatus(int iID)
{
	hash_map< int, SPMotionStatus* >::iterator iter = m_hmMotionStatusList.find(iID);
	if( iter == m_hmMotionStatusList.end() )
		return NULL;

	return iter->second;
}

//------------------------------------------------------------------------------------
SPMotionStatus* SPSkillManager::GetMOBMotionStatus(int iID)
{
	hash_map< int, SPMotionStatus* >::iterator iter = m_hmMOBMotionStatusList.find(iID);
	if( iter == m_hmMOBMotionStatusList.end() )
		return NULL;

	return iter->second;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::AddActionInterface(int iID, SPActionInterface* pkActionInterface)
{
	if( IsExistActionInterface(iID) )
		return false;

	if( pkActionInterface->GetUIInfo()->eInventoryType == SIT_ACTION )
        m_hmActionInterfaceList.insert(hash_map< int, SPActionInterface* >::value_type(iID, pkActionInterface));
	else
		m_hmNullActionInterfaceList.insert(hash_map< int, SPActionInterface* >::value_type(iID, pkActionInterface));

	if( pkActionInterface != NULL )
		AddSkillFind( pkActionInterface->GetUIInfo() , m_mActionInterfaceListFind );

	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillManager::IsExistActionInterface(int iID)
{
	hash_map< int, SPActionInterface* >::iterator iter = m_hmActionInterfaceList.find(iID);
	hash_map< int, SPActionInterface* >::iterator iterNull = m_hmNullActionInterfaceList.find(iID);

	if( iter == m_hmActionInterfaceList.end() && iterNull == m_hmNullActionInterfaceList.end() )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
SPActionInterface* SPSkillManager::GetFirstActionInterface()
{
	m_iCurIndex = 0;
	if( m_iCurIndex >= (int)m_hmActionInterfaceList.size() )
		return NULL;

	hash_map< int, SPActionInterface* >::iterator iter = m_hmActionInterfaceList.begin();
	return iter->second;
}

//------------------------------------------------------------------------------------
SPActionInterface* SPSkillManager::GetNextActionInterface()
{
	m_iCurIndex++;
	if( m_iCurIndex >= (int)m_hmActionInterfaceList.size() )
		return NULL;

	hash_map< int, SPActionInterface* >::iterator iter = m_hmActionInterfaceList.begin();
	for( int i = 0; i < m_iCurIndex; i++ )
	{
		++iter;
		if( iter == m_hmActionInterfaceList.end() )
			return NULL;
	}
	return iter->second;
}

//------------------------------------------------------------------------------------
SPActionInterface* SPSkillManager::GetActionInterface(int iID)
{
	hash_map< int, SPActionInterface* >::iterator iter = m_hmActionInterfaceList.find(iID);
	hash_map< int, SPActionInterface* >::iterator iterNull = m_hmNullActionInterfaceList.find(iID);

	if( iter != m_hmActionInterfaceList.end() )
		return iter->second;

	if( iterNull != m_hmNullActionInterfaceList.end() )
		return iterNull->second;

	return NULL;
}

//------------------------------------------------------------------------------------
int SPSkillManager::GetActionInterfaceCount()
{
	return (int)m_hmActionInterfaceList.size();
}

//------------------------------------------------------------------------------------
bool SPSkillManager::AddAttackCombo(int iID, SPAttackCombo* pkAttackCombo)
{
	hash_map< int, SPAttackCombo* >::iterator iter = m_hmAttackComboList.find(iID);
	if( iter != m_hmAttackComboList.end() )
		return false;

	m_hmAttackComboList.insert(hash_map< int, SPAttackCombo* >::value_type(iID, pkAttackCombo));
	return true;
}

//------------------------------------------------------------------------------------
int SPSkillManager::GetNextAttackComboNo(int iID, int iAttackType)
{
	hash_map< int, SPAttackCombo* >::iterator iter = m_hmAttackComboList.find(iID);
	if( iter == m_hmAttackComboList.end() )
		return 0;

	return iter->second->GetNextNo(iAttackType);
}

//------------------------------------------------------------------------------------
SKILL_UI_INFO* SPSkillManager::GetSkillSubID( int iSubID , int iIndex , SKILL_INVENTORY_TYPE eType )
{
	SKILL_FIND_BUNDLE*	pSkillFindBundle	=	NULL;

	if( eType == SIT_ACTION )
		pSkillFindBundle	=	GetSkillFindBundle( iSubID , m_mActionInterfaceListFind );
	else
		pSkillFindBundle	=	GetSkillFindBundle( iSubID , m_mSkillListFind );

	SKILL_UI_INFO*		pSkillUIInfo		=	NULL;

	if( pSkillFindBundle == NULL )
		return pSkillUIInfo;

	STD_MAP_SKILL_FIND::iterator	mIter	=	pSkillFindBundle->m_mUIContent.find( iIndex );

	if( mIter != pSkillFindBundle->m_mUIContent.end() )
	{
		pSkillUIInfo	=	(*mIter).second;
	}

	return pSkillUIInfo;
}

//------------------------------------------------------------------------------------
int SPSkillManager::GetSkillSubIDPage( int iSubID , SKILL_INVENTORY_TYPE eType )
{
	SKILL_FIND_BUNDLE*	pSkillFindBundle	=	NULL;

	if( eType == SIT_ACTION )
		pSkillFindBundle	=	GetSkillFindBundle( iSubID , m_mActionInterfaceListFind );
	else
		pSkillFindBundle	=	GetSkillFindBundle( iSubID , m_mSkillListFind );

	int					iPage				=	-1;

	if( pSkillFindBundle == NULL )
		return iPage;

	iPage	=	pSkillFindBundle->m_iEndPage;

	return iPage;
}

//------------------------------------------------------------------------------------
SPSkillManager::SKILL_FIND_BUNDLE* SPSkillManager::GetSkillFindBundle( int iSubID , STD_MAP_SKILL_FIND_BUNDLE& mBundle )
{
	STD_MAP_SKILL_FIND_BUNDLE::iterator		mIter		=	mBundle.find( iSubID );
	SKILL_FIND_BUNDLE*						pSkillFind	=	NULL;

	if( mIter != mBundle.end() )
	{
		pSkillFind	=	&( (*mIter).second );
	}

	return pSkillFind;
}

//------------------------------------------------------------------------------------
void SPSkillManager::AddSkillFind( SKILL_UI_INFO* pSkillUIInfo , STD_MAP_SKILL_FIND_BUNDLE& mBundle )
{
	if( pSkillUIInfo == NULL )
		return;

	SKILL_FIND_BUNDLE*	pSkillFind	=	GetSkillFindBundle( pSkillUIInfo->iSubID , mBundle );

	int	iSkillGridXCount	=	4;
	int iSKillGridYCount	=	3;

	int	iEndPage	=	pSkillUIInfo->iSubIndex / iSkillGridXCount - iSKillGridYCount + 1;

	if( pSkillUIInfo->iSubIndex % iSkillGridXCount > 0 )
		++iEndPage;

	if( iEndPage <= 0 )
		iEndPage = 0;

	if( pSkillFind != NULL )
	{
		if( iEndPage > pSkillFind->m_iEndPage )
			pSkillFind->m_iEndPage	=	iEndPage;

		pSkillFind->m_mUIContent.insert( STD_MAP_SKILL_FIND::value_type( pSkillUIInfo->iSubIndex - 1 , pSkillUIInfo ) );
	}
	else
	{
		SKILL_FIND_BUNDLE	stSkillFindBundle;

		stSkillFindBundle.m_iEndPage	=	iEndPage;
		stSkillFindBundle.m_mUIContent.insert( STD_MAP_SKILL_FIND::value_type( pSkillUIInfo->iSubIndex - 1 , pSkillUIInfo ) );

		mBundle.insert( STD_MAP_SKILL_FIND_BUNDLE::value_type( pSkillUIInfo->iSubID , stSkillFindBundle ) );
	}
}

//------------------------------------------------------------------------------------
void SPSkillManager::DelSkillFind( SKILL_UI_INFO* pSkillUIInfo , STD_MAP_SKILL_FIND_BUNDLE& mBundle )
{
	if( pSkillUIInfo == NULL )
		return;

	SKILL_FIND_BUNDLE*	pSkillFind	=	GetSkillFindBundle( pSkillUIInfo->iSubID , mBundle );

	if( pSkillFind == NULL )
		return;

	pSkillFind->m_mUIContent.erase( pSkillUIInfo->iSubIndex - 1 );

	STD_MAP_SKILL_FIND::iterator	mIter				=	pSkillFind->m_mUIContent.begin();
	SPSkill*						pPageSkill			=	NULL;
	SKILL_UI_INFO*					pPageSkillUIInfo	=	NULL;

	int	iSkillGridXCount	=	4;
	int iSKillGridYCount	=	3;

	int	iEndPage			=	0;

	while( mIter != pSkillFind->m_mUIContent.end() )
	{
		pPageSkillUIInfo	=	(*mIter).second;

		if( pPageSkillUIInfo != NULL )
		{
			iEndPage	=	pPageSkillUIInfo->iSubIndex / iSkillGridXCount - iSKillGridYCount + 1;

			if( pPageSkillUIInfo->iSubIndex % iSkillGridXCount > 0 )
				++iEndPage;

			if( iEndPage <= 0 )
				iEndPage = 0;

			if( iEndPage > pSkillFind->m_iEndPage )
				pSkillFind->m_iEndPage	=	iEndPage;
		}

		++mIter;
	}
}
