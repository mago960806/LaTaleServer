#include <vector>
#include <string>
#include <hash_map>

#include "SPCommon.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBStatus.h"
#include "SPPlayer.h"

#include "SPEffect.h"
#include "SPEffectArchive.h"
#include "SPEffectManager.h"

#include "SPCheckManager.h"

using namespace stdext;
//------------------------------------------------------------------------------------
// SPEffectActivity
SPEffectActivity::SPEffectActivity()
: m_pkEffect(NULL)
, m_iLevel(0)
, m_fApplyTime(0.0f)
, m_bCaster(false)
{
	Init();
}

//------------------------------------------------------------------------------------
SPEffectActivity::SPEffectActivity(SPEffect* pkEffect, int iLevel, float fApplyTime, bool bCaster)
: m_pkEffect(pkEffect)
, m_iLevel(iLevel)
, m_fApplyTime(fApplyTime)
, m_bCaster(bCaster)
{
	Init();
}

//------------------------------------------------------------------------------------
SPEffectActivity::SPEffectActivity(int iID, int iLevel, float fApplyTime, bool bCaster)
{
	Init();
	m_pkEffect = SPEffectManager::GetInstance()->GetEffect(iID);
	m_iLevel = iLevel;
	m_fApplyTime = fApplyTime;
	m_bCaster = bCaster;
}

//------------------------------------------------------------------------------------
SPEffectActivity::~SPEffectActivity()
{
}

//------------------------------------------------------------------------------------
void SPEffectActivity::Init()
{
	m_fAccmulateTime = 0.0f;
	EffectApplyInit();
}

//------------------------------------------------------------------------------------
void SPEffectActivity::EffectApplyInit()
{
	m_fEffectAccmulateTime = 0.0f;
	m_iApplyEffect = 0;
}

//------------------------------------------------------------------------------------
void SPEffectActivity::Process(float fElapsedTime)
{
	m_fAccmulateTime += fElapsedTime;
	m_fEffectAccmulateTime += fElapsedTime;

	EFFECT_TYPE eType = m_pkEffect->GetType();
	if( eType == ET_NULL || eType == ET_INSTANT || eType == ET_PASSIVE )
		return;
	
	//if( eType == ET_KEEP )
	//{
	//	if( m_fAccmulateTime > m_fApplyTime )
	//	{
	//		m_iApplyEffect = 2;
	//		return;
	//	}
	//}

	EFFECT_STATUS* pStatus = m_pkEffect->GetStatusInfo();
	float fEffectApplyTime = pStatus->fDelayTime + pStatus->fDelayTimeElv * (float)m_iLevel;
	
	if( fEffectApplyTime <= 0.0f )
		return;

	if( m_fEffectAccmulateTime > fEffectApplyTime )
		m_iApplyEffect = 1;
}

//------------------------------------------------------------------------------------
SPEffect* SPEffectActivity::GetEffect()
{
	return m_pkEffect;
}

//------------------------------------------------------------------------------------
int SPEffectActivity::GetLevel()
{
	return m_iLevel;
}

//------------------------------------------------------------------------------------
// 0: continue	1: EffectApply	2: delete
int SPEffectActivity::GetApplyEffect()
{
	return m_iApplyEffect;
}

//------------------------------------------------------------------------------------
float SPEffectActivity::GetApplyTime()
{
	return m_fApplyTime;
}

//------------------------------------------------------------------------------------
float SPEffectActivity::GetAccmulateTime()
{
	return m_fAccmulateTime;
}

//------------------------------------------------------------------------------------
bool SPEffectActivity::IsCaster()
{
	return m_bCaster;
}

//------------------------------------------------------------------------------------
void SPEffectActivity::SetUniqueKey( unsigned int uiUniqueKey )
{
	m_uiUniqueKey	=	uiUniqueKey;
}

//------------------------------------------------------------------------------------
unsigned int SPEffectActivity::GetUniqueKey()
{
	return m_uiUniqueKey;
}

//------------------------------------------------------------------------------------
// SPEffectArchive
SPEffectArchive::SPEffectArchive(SPGameObject* pkParent)
: m_pkParent(pkParent)
, m_iCurEffectIndex(0)
{
}

//------------------------------------------------------------------------------------
SPEffectArchive::~SPEffectArchive()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPEffectArchive::Clear()
{
	hash_multimap< unsigned int, SPEffectActivity* >::iterator iter;
	iter = m_hmEffectList.begin();
	while( iter != m_hmEffectList.end() )
	{
		delete iter->second;
		iter++;
	}
	m_hmEffectList.clear();
}

//------------------------------------------------------------------------------------
void SPEffectArchive::Process(float fElapsedTime)
{
	hash_multimap< unsigned int, SPEffectActivity* >::iterator iter;
	SPEffectActivity* pkEffectActivity;
	int iApplyEffect;

	iter = m_hmEffectList.begin();
	while( iter != m_hmEffectList.end() )
	{
		pkEffectActivity = (*iter).second;
		pkEffectActivity->Process(fElapsedTime);
		iApplyEffect = pkEffectActivity->GetApplyEffect();

		if( iApplyEffect == 2 )
		{
			delete iter->second;
			iter = m_hmEffectList.erase(iter);
			continue;
		}
		else if( iApplyEffect == 1 )
			ApplyEffect(pkEffectActivity);
		
		++iter;
	}
}

//------------------------------------------------------------------------------------
bool SPEffectArchive::AddEffect(unsigned int uiID, SPEffectActivity* pkEffectActivity, bool bTargetFX)
{
	SPEffect* pkEffect = pkEffectActivity->GetEffect();
	
	if( bTargetFX )
		m_pkParent->SPGOBPerformMessage(SPGM_SET_TARGET_EFFECT_FXSOUND, (LPARAM)pkEffect->GetFXSoundInfo());

	if( pkEffect->GetTransparency() )
		m_pkParent->SPGOBPerformMessage(SPGM_SET_TRANSPARENCY);

	if( pkEffect->GetType() == ET_INSTANT )
	{
		ApplyEffect(pkEffectActivity);
		delete pkEffectActivity;
		return true;
	}

	pkEffectActivity->SetUniqueKey( uiID );

	m_hmEffectList.insert(
		hash_multimap<unsigned int , SPEffectActivity*>::value_type(uiID, pkEffectActivity));
	return true;
}

//------------------------------------------------------------------------------------
bool SPEffectArchive::RemoveEffect(SPEffectActivity* pkEffectActivity)
{
	if( pkEffectActivity == NULL )
		return false;

	SPEffect* pkEffect;
	hash_multimap< unsigned int, SPEffectActivity* >::iterator iter;
	iter = m_hmEffectList.begin();
	while( iter != m_hmEffectList.end() )
	{
		if( iter->second == pkEffectActivity )
		{
			if( pkEffect = pkEffectActivity->GetEffect() )
			{
				if( pkEffect->GetTransparency() )
					m_pkParent->SPGOBPerformMessage(SPGM_DEL_TRANSPARENCY);
			}

			delete iter->second;
			m_hmEffectList.erase(iter);
			return true;
		}

		++iter;
	}

	return false;
}

//------------------------------------------------------------------------------------
bool SPEffectArchive::RemoveEffect(unsigned int uiID)
{
	SPEffect* pkEffect;
	hash_multimap< unsigned int, SPEffectActivity* >::iterator iter;
	iter = m_hmEffectList.begin();
	while( iter != m_hmEffectList.end() )
	{
		if( (*iter).first == uiID )
		{
			if( pkEffect = (*iter).second->GetEffect() )
			{
				if( pkEffect->GetTransparency() )
					m_pkParent->SPGOBPerformMessage(SPGM_DEL_TRANSPARENCY);
			}

			delete (*iter).second;
			iter = m_hmEffectList.erase(iter);
			continue;
		}

		++iter;
	}
	
	return true;
}

//------------------------------------------------------------------------------------
void SPEffectArchive::ApplyEffect(SPEffectActivity* pkEffectActivity)
{
	SPEffect* pkEffect = pkEffectActivity->GetEffect();
	if( pkEffect == NULL || m_pkParent == NULL )
		return;

	if( !g_pCheckManager->CheckApplyEffectRequire(pkEffect->GetID(), m_pkParent) )
	{
		pkEffectActivity->EffectApplyInit();
		return;
	}

	EFFECT_TYPE eType = pkEffect->GetType();
	if( eType == ET_KEEP || eType == ET_TOGGLE )
		ApplyStatus(pkEffect, pkEffectActivity->GetLevel());

	ApplyFXSound(pkEffect);
	pkEffectActivity->EffectApplyInit();
}

//------------------------------------------------------------------------------------
void SPEffectArchive::ApplyStatus(SPEffect* pkEffect, int iLevel)
{
	if( m_pkParent == NULL )
		return;

	SPGOBStatus* pkGOBStatus = NULL;
	m_pkParent->SPGOBPerformMessage(SPGM_GETGOBSTATUS, (LPARAM)&pkGOBStatus);
	if( pkGOBStatus == NULL )
		return;

	if( m_pkParent->SPGOBPerformMessage(SPGM_ISDEAD) )
		return;

	STATUS_TYPE ePlayerStatusType = STATUS_NONE;
	EFFECT_STATUS* pStatusInfo = pkEffect->GetStatusInfo();
	int iEffectValue, iStatusValue;
	bool bAddValue;

	ePlayerStatusType = ApplyEffectValue(pkGOBStatus, pStatusInfo, iLevel, iEffectValue, bAddValue);
	if( ePlayerStatusType == STATUS_NONE )
		return;
	
	iStatusValue = pkGOBStatus->GetStatusValue(ePlayerStatusType);
	if( bAddValue )
	{
		iStatusValue += iEffectValue;
		if( iStatusValue < 0 )
			iStatusValue = 0;
	}
	else
		iStatusValue = iEffectValue;

	pkGOBStatus->SetStatusValue(ePlayerStatusType, iStatusValue);
}

//------------------------------------------------------------------------------------
STATUS_TYPE SPEffectArchive::ApplyEffectValue(SPGOBStatus* pkGOBStatus,
	EFFECT_STATUS* pEffectStatus, int iLevel, int& iValue, bool& bAddValue)
{
	STATUS_TYPE eRetType = STATUS_NONE;
	
	iValue = 0;
	bAddValue = false;

	switch( pEffectStatus->eKindType )
	{
	case DSKT_RECOVERY_HP:
		eRetType = STATUS_HP;
		iValue = pkGOBStatus->GetStatusValue(STATUS_RECOVERY_HP);
		bAddValue = true;
		break;
	case DSKT_RECOVERY_SP:
		eRetType = STATUS_SP;
		iValue = pkGOBStatus->GetStatusValue(STATUS_RECOVERY_SP);
		bAddValue = true;
		break;
	case DSKT_HP:
		eRetType = STATUS_HP;
		iValue = pEffectStatus->iParameter[0] + pEffectStatus->iParameter[1] * iLevel;
		bAddValue = true;
		break;
	case DSKT_SP:
		eRetType = STATUS_SP;
		iValue = pEffectStatus->iParameter[0] + pEffectStatus->iParameter[1] * iLevel;
		bAddValue = true;
		break;
	case DSKT_RECOVERY_RATIO_HP:
	case DSKT_RECOVERY_RATIO_SP:
		if( pEffectStatus->eType == EST_EVENT )
		{
			iValue = pEffectStatus->iParameter[0] + pEffectStatus->iParameter[1] * iLevel;
			int iMaxValue = 0;
			if( pEffectStatus->eKindType == DSKT_RECOVERY_RATIO_HP )
			{
				eRetType = STATUS_HP;
				iMaxValue = pkGOBStatus->GetStatusValue(STATUS_MHP);
			}
			else
			{
				eRetType = STATUS_SP;
				iMaxValue = pkGOBStatus->GetStatusValue(STATUS_MSP);
			}

			iValue = (int)((float)iMaxValue * (float)iValue * 0.01f);
			bAddValue = true;
		}
		break;
	}
	
	return eRetType;
}

//------------------------------------------------------------------------------------
void SPEffectArchive::ApplyFXSound(SPEffect* pkEffect)
{
	if( m_pkParent == NULL )
		return;

	//unsigned int uiMsg = SPGM_SET_CASTER_EFFECT_FXSOUND;
	//if( !bCaster )
	//	uiMsg = SPGM_SET_TARGET_EFFECT_FXSOUND;

	//m_pkParent->SPGOBPerformMessage((SPGM)uiMsg, (LPARAM)pkEffect->GetFXSoundInfo());

	m_pkParent->SPGOBPerformMessage(SPGM_SET_CASTER_EFFECT_FXSOUND, (LPARAM)pkEffect->GetFXSoundInfo());
}

//------------------------------------------------------------------------------------
STATUS_TYPE SPEffectArchive::GetStatus(EFFECT_STATUS_KIND_TYPE eType)
{
	STATUS_TYPE eRetType = STATUS_NONE;
	switch( eType )
	{
	case DSKT_GENDER:
		eRetType = STATUS_GENDER;
		break;
	case DSKT_LV:
		eRetType = STATUS_LV;
		break;
	case DSKT_HP:
		eRetType = STATUS_HP;
		break;
	case DSKT_SP:
		eRetType = STATUS_SP;
		break;
	}

	return eRetType;
}

//------------------------------------------------------------------------------------
SPEffectActivity* SPEffectArchive::GetFirstEffectActivity()
{
	m_iCurEffectIndex	=	0;
	return GetCurEffectActivity();
}

//------------------------------------------------------------------------------------
SPEffectActivity* SPEffectArchive::GetNextEffectActivity()
{
	m_iCurEffectIndex++;
	return GetCurEffectActivity();
}

//------------------------------------------------------------------------------------
bool SPEffectArchive::IsExistEffectActivity(int iEffectID)
{
	hash_multimap< unsigned int, SPEffectActivity* >::iterator iter = m_hmEffectList.begin();
	while( iter != m_hmEffectList.end() )
	{
		SPEffect* pkEffect = ((*iter).second)->GetEffect();
		if( pkEffect && pkEffect->GetID() == iEffectID )
			return true;
			
		++iter;
	}

	return false;
}

//------------------------------------------------------------------------------------
SPEffectActivity* SPEffectArchive::GetUniqueKeyActivity( unsigned int uiUniqueKey )
{
	stdext::hash_multimap< unsigned int, SPEffectActivity* >::iterator	hmIter	=	m_hmEffectList.find( uiUniqueKey );

	if( hmIter == m_hmEffectList.end() )
		return NULL;

	if( ( (*hmIter).first ) != uiUniqueKey )
		return NULL;

	return (*hmIter).second;
}

//------------------------------------------------------------------------------------
SPEffectActivity* SPEffectArchive::GetCurEffectActivity()
{
	int	iTotalCount	=	(int)m_hmEffectList.size();

	if( m_iCurEffectIndex >= iTotalCount )
		return NULL;

	int	iCount = 0;

	stdext::hash_multimap< unsigned int, SPEffectActivity* >::iterator	hmIter	=	m_hmEffectList.begin();

	while ( hmIter != m_hmEffectList.end() )
	{
		if( m_iCurEffectIndex == iCount )
			return (*hmIter).second;

		iCount++;
		hmIter++;
	}

	return NULL;
}
