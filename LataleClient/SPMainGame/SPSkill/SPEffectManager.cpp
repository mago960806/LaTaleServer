#include <vector>
#include <string>
#include <hash_map>

#include "SPCommon.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPEffect.h"
#include "SPEffectManager.h"
#include "SPDebug.h"

using namespace std;
using namespace stdext;

SPEffectManager* SPEffectManager::m_pkInstance = NULL;
//------------------------------------------------------------------------------------
SPEffectManager* SPEffectManager::GetInstance()
{
	if( m_pkInstance == NULL )
		m_pkInstance = new SPEffectManager;

	return m_pkInstance;
}

//------------------------------------------------------------------------------------
void SPEffectManager::Release()
{
	if( m_pkInstance )
	{
		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}

//------------------------------------------------------------------------------------
SPEffectManager::SPEffectManager()
{
}

//------------------------------------------------------------------------------------
SPEffectManager::~SPEffectManager()
{
	Clear();
}

//------------------------------------------------------------------------------------
bool SPEffectManager::Init()
{
#if defined(_DEBUG)
	DXUTOutputDebugString("SPEffectManager::Init()\n");
#endif

	if( !LoadLDTFile() )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
bool SPEffectManager::LoadLDTFile()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/EFFECT.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iItemID, i, j;
	char szBuf[128];

	LDT_Field LFD_Name, LFD_Description;
	LDT_Field LFD_BuffID, LFD_BuffLv, LFD_BuffElv, LFD_BuffIcon, LFD_BuffIconIndex , LFD_BuffName , LFD_BuffDescription;
	LDT_Field LFD_Type, LFD_ApplyTime, LFD_ApplyTimeElv;
	LDT_Field LFD_MissileType, LFT_MissileFX, LFT_MissileSpeed;
	LDT_Field LFD_TargetFX, LFD_TargetSound, LFD_CasterFX, LFD_CasterSound;
	LDT_Field LFD_RequireEquip, LFD_RequireItem, LFD_RequireMotion, LFD_StandCheck;
	LDT_Field LFD_StatusType, LFD_StatusKindType, LFD_StatusParameter;
	LDT_Field LFD_StatusDelayTime, LFD_StatusDelayTimeElv;
	LDT_Field LFD_AttachFXTarget, LFD_Transparency;

	SPEffect* pkEffect;
	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);

		/*
		pkLDTFile->GetFieldFromLable(iItemID, "_Name",					LFD_Name);
		pkLDTFile->GetFieldFromLable(iItemID, "_Description",			LFD_Description);
		pkLDTFile->GetFieldFromLable(iItemID, "_BuffID",				LFD_BuffID);
		pkLDTFile->GetFieldFromLable(iItemID, "_BuffLv",				LFD_BuffLv);
		pkLDTFile->GetFieldFromLable(iItemID, "_BuffElv",				LFD_BuffElv);
		pkLDTFile->GetFieldFromLable(iItemID, "_Buff_Icon",				LFD_BuffIcon);
		pkLDTFile->GetFieldFromLable(iItemID, "_Buff_IconIndex",		LFD_BuffIconIndex);
		pkLDTFile->GetFieldFromLable(iItemID, "_Buff_Name",				LFD_BuffName);
		pkLDTFile->GetFieldFromLable(iItemID, "_Buff_Description",		LFD_BuffDescription);
		pkLDTFile->GetFieldFromLable(iItemID, "_EffectType",			LFD_Type);
		pkLDTFile->GetFieldFromLable(iItemID, "_ApplyTime",				LFD_ApplyTime);
		pkLDTFile->GetFieldFromLable(iItemID, "_ApplyTimeElv",			LFD_ApplyTimeElv);
		pkLDTFile->GetFieldFromLable(iItemID, "_MissileType",			LFD_MissileType);
		pkLDTFile->GetFieldFromLable(iItemID, "_FxMissile_Target",		LFT_MissileFX);
		pkLDTFile->GetFieldFromLable(iItemID, "_FxMissileSpeed_Target",	LFT_MissileSpeed);
		pkLDTFile->GetFieldFromLable(iItemID, "_Fx_Target",				LFD_TargetFX);
		pkLDTFile->GetFieldFromLable(iItemID, "_Sound_Target",			LFD_TargetSound);
		pkLDTFile->GetFieldFromLable(iItemID, "_Fx_Caster",				LFD_CasterFX);
		pkLDTFile->GetFieldFromLable(iItemID, "_Sound_Caster",			LFD_CasterSound);
		pkLDTFile->GetFieldFromLable(iItemID, "_EffectEquipType",		LFD_RequireEquip);
		pkLDTFile->GetFieldFromLable(iItemID, "_EffectItemKind",		LFD_RequireItem);
		pkLDTFile->GetFieldFromLable(iItemID, "_EffectAction",			LFD_RequireMotion);
		pkLDTFile->GetFieldFromLable(iItemID, "_EffectStand",			LFD_StandCheck);
		pkLDTFile->GetFieldFromLable(iItemID, "_StatusEffect",			LFD_StatusType);
		pkLDTFile->GetFieldFromLable(iItemID, "_StatusEffectType",		LFD_StatusKindType);
		pkLDTFile->GetFieldFromLable(iItemID, "_EffectDelayTime",		LFD_StatusDelayTime);
		pkLDTFile->GetFieldFromLable(iItemID, "_EffectDelayTimeElv",	LFD_StatusDelayTimeElv);
		pkLDTFile->GetFieldFromLable(iItemID, "_AttachmentFX_Target",	LFD_AttachFXTarget);
		pkLDTFile->GetFieldFromLable(iItemID, "_Translucency",			LFD_Transparency);
		*/

		pkLDTFile->GetField(iItemID, 0,		LFD_Name);				//"_Name"
		pkLDTFile->GetField(iItemID, 1,		LFD_Description);		//"_Description"
		pkLDTFile->GetField(iItemID, 3,		LFD_BuffID);			//"_BuffID"
		pkLDTFile->GetField(iItemID, 4,		LFD_BuffLv);			//"_BuffLv"
		pkLDTFile->GetField(iItemID, 5,		LFD_BuffElv);			//"_BuffElv"
		pkLDTFile->GetField(iItemID, 9,		LFD_BuffIcon);			//"_Buff_Icon"
		pkLDTFile->GetField(iItemID, 10,	LFD_BuffIconIndex);		//"_Buff_IconIndex"
		pkLDTFile->GetField(iItemID, 11,	LFD_BuffName);			//"_Buff_Name"
		pkLDTFile->GetField(iItemID, 12,	LFD_BuffDescription);	//"_Buff_Description"
		pkLDTFile->GetField(iItemID, 13,	LFD_Type);				//"_EffectType"
		pkLDTFile->GetField(iItemID, 28,	LFD_ApplyTime);			//"_ApplyTime"
		pkLDTFile->GetField(iItemID, 29,	LFD_ApplyTimeElv);		//"_ApplyTimeElv"
		pkLDTFile->GetField(iItemID, 30,	LFD_MissileType);		//"_MissileType"
		pkLDTFile->GetField(iItemID, 31,	LFT_MissileFX);			//"_FxMissile_Target"
		pkLDTFile->GetField(iItemID, 32,	LFT_MissileSpeed);		//"_FxMissileSpeed_Target"
		pkLDTFile->GetField(iItemID, 33,	LFD_TargetFX);			//"_Fx_Target"
		pkLDTFile->GetField(iItemID, 34,	LFD_TargetSound);		//"_Sound_Target"
		pkLDTFile->GetField(iItemID, 35,	LFD_CasterFX);			//"_Fx_Caster"
		pkLDTFile->GetField(iItemID, 36,	LFD_CasterSound);		//"_Sound_Caster"
		pkLDTFile->GetField(iItemID, 43,	LFD_RequireEquip);		//"_EffectEquipType"
		pkLDTFile->GetField(iItemID, 44,	LFD_RequireItem);		//"_EffectItemKind"
		pkLDTFile->GetField(iItemID, 45,	LFD_RequireMotion);		//"_EffectAction"
		pkLDTFile->GetField(iItemID, 46,	LFD_StandCheck);		//"_EffectStand"
		pkLDTFile->GetField(iItemID, 47,	LFD_StatusType);		//"_StatusEffect"
		pkLDTFile->GetField(iItemID, 48,	LFD_StatusKindType);	//"_StatusEffectType"
		pkLDTFile->GetField(iItemID, 58,	LFD_StatusDelayTime);	//"_EffectDelayTime"
		pkLDTFile->GetField(iItemID, 59,	LFD_StatusDelayTimeElv);//"_EffectDelayTimeElv"
		pkLDTFile->GetField(iItemID, 38,	LFD_AttachFXTarget);	//"_AttachmentFX_Target"
		pkLDTFile->GetField(iItemID, 63,	LFD_Transparency);		//"_Translucency"

		pkEffect = new SPEffect(iItemID, (EFFECT_TYPE)LFD_Type.uData.lValue,
			(float)LFD_ApplyTime.uData.lValue / 10.0f, (float)LFD_ApplyTimeElv.uData.lValue / 10.0f);

		pkEffect->SetName(LFD_Name.uData.pValue);
		pkEffect->SetDescription(LFD_Description.uData.pValue);

		EFFECT_BUFF BuffInfo;
		BuffInfo.iID = LFD_BuffID.uData.lValue;
		BuffInfo.iLevel = LFD_BuffLv.uData.lValue;
		BuffInfo.iLevelElv = LFD_BuffElv.uData.lValue;
		BuffInfo.iIcon = LFD_BuffIcon.uData.lValue;
		BuffInfo.iIconIndex = LFD_BuffIconIndex.uData.lValue;
		BuffInfo.strName = LFD_BuffName.uData.pValue;
		BuffInfo.strDescription = LFD_BuffDescription.uData.pValue;
		pkEffect->SetBuffInfo(BuffInfo);

		EFFECT_MISSILE MissileInfo;
		MissileInfo.eType = (EFFECT_MISSILE_TYPE)LFD_MissileType.uData.lValue;
		MissileInfo.iTargetFX = LFT_MissileFX.uData.lValue;
		MissileInfo.iSpeed = LFT_MissileSpeed.uData.lValue;
		pkEffect->SetMissileInfo(MissileInfo);

		EFFECT_FX_SOUND FXSoundInfo;
		FXSoundInfo.iTargetFX = LFD_TargetFX.uData.lValue;
		FXSoundInfo.iTargetSound = LFD_TargetSound.uData.lValue;
		FXSoundInfo.iCasterFX = LFD_CasterFX.uData.lValue;
		FXSoundInfo.iCasterSound = LFD_CasterSound.uData.lValue;
		pkEffect->SetFXSoundInfo(FXSoundInfo);

		EFFECT_REQUIRE RequireInfo;
		RequireInfo.iEquipType = LFD_RequireEquip.uData.lValue;
		RequireInfo.iItemKind = LFD_RequireItem.uData.lValue;
		RequireInfo.iStandCheck = LFD_StandCheck.uData.lValue;
		pkEffect->SetRequireInfo(RequireInfo);

		RequireInfo.iMotionType = 0;
		if( LFD_RequireMotion.uData.pValue )
			RequireInfo.iMotionType = _atoi64(LFD_RequireMotion.uData.pValue);

		EFFECT_STATUS StatusInfo;
		ZeroMemory(&StatusInfo, sizeof(EFFECT_STATUS));
		StatusInfo.eType = (EFFECT_STATUS_TYPE)LFD_StatusType.uData.lValue;
		StatusInfo.eKindType = (EFFECT_STATUS_KIND_TYPE)LFD_StatusKindType.uData.lValue;
		StatusInfo.fDelayTime = (float)LFD_StatusDelayTime.uData.lValue / 10.0f;
		StatusInfo.fDelayTimeElv = (float)LFD_StatusDelayTimeElv.uData.lValue / 10.0f;
		for( j = 0; j < 6; j++ )
		{
			sprintf(szBuf, "_EffectParameter%d", j + 1);
			pkLDTFile->GetFieldFromLable(iItemID, szBuf, LFD_StatusParameter);
			StatusInfo.iParameter[j] = LFD_StatusParameter.uData.lValue;
		}
		pkEffect->SetStatusInfo(StatusInfo);

		pkEffect->SetAttachFXGroup( LFD_AttachFXTarget.uData.lValue );
		pkEffect->SetTransparency(LFD_Transparency.uData.lValue);

		if( !AddEffect(iItemID, pkEffect) )
			delete pkEffect;
	}

	SAFE_RELEASE(pkLDTFile);
	return true;
}

//------------------------------------------------------------------------------------
void SPEffectManager::Clear()
{
	hash_map< int, SPEffect* >::iterator iter = m_hmEffectList.begin();
	while( iter != m_hmEffectList.end() )
	{
		delete iter->second;
		iter++;
	}
	m_hmEffectList.clear();
}

//------------------------------------------------------------------------------------
bool SPEffectManager::AddEffect(int iID, SPEffect* pkEffect)
{
	if( IsExistEffect(iID) )
		return false;

	m_hmEffectList.insert(hash_map< int, SPEffect* >::value_type(iID, pkEffect));
	return true;
}

//------------------------------------------------------------------------------------
bool SPEffectManager::IsExistEffect(int iID)
{
	hash_map< int, SPEffect* >::iterator iter = m_hmEffectList.find(iID);
	if( iter == m_hmEffectList.end() )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
SPEffect* SPEffectManager::GetEffect(int iID)
{
	hash_map< int, SPEffect* >::iterator iter = m_hmEffectList.find(iID);
	if( iter == m_hmEffectList.end() )
		return NULL;

	return iter->second;
}