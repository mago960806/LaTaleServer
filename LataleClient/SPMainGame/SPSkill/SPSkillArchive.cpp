#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "SPCommon.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBStatus.h"
#include "SPPlayer.h"
#include "SPCheckManager.h"


#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPSkillManager.h"

#include "SPMotionStatus.h"

#include "SPEffect.h"
#include "SPEffectArchive.h"
#include "SPEffectManager.h"

#include "SPBattle.h"
#include "SPBattleCluster.h"

#include "SPGOBManager.h"

using namespace std;

//------------------------------------------------------------------------------------
struct IsLessDistance
{
	static int iPosX;
	static int iPosY;

	static bool compare(SPGameObject* pkObj1, SPGameObject* pkObj2)
	{
		int iDiffX1, iDiffY1, iDiffX2, iDiffY2;

		iDiffX1 = iPosX - pkObj1->SPGOBPerformMessage(SPGM_GETPOSX);
		iDiffY1 = iPosY - pkObj1->SPGOBPerformMessage(SPGM_GETPOSY);

		iDiffX2 = iPosX - pkObj2->SPGOBPerformMessage(SPGM_GETPOSX);
		iDiffY2 = iPosY - pkObj2->SPGOBPerformMessage(SPGM_GETPOSY);

		if( (iDiffX1 * iDiffX1 + iDiffY1 * iDiffY1) < (iDiffX2 * iDiffX2 + iDiffY2 * iDiffY2) )
			return true;

		return false;
	}
};

int IsLessDistance::iPosX = 0;
int IsLessDistance::iPosY = 0;

//------------------------------------------------------------------------------------
// SPSkillActivity
SPSkillActivity::SPSkillActivity()
: m_pkSkill(NULL)
{
	Init();
}

//------------------------------------------------------------------------------------
SPSkillActivity::SPSkillActivity(SPSkill* pkSkill, int iLevel, int iCurLevel)
: m_pkSkill(pkSkill)
{
	Init();
	m_iLevel = iLevel;
	m_iCurLevel = iCurLevel;
}

//------------------------------------------------------------------------------------
SPSkillActivity::SPSkillActivity(int iID, int iLevel, int iCurLevel)
{
	Init();

	m_pkSkill = SPSkillManager::GetInstance()->GetSkill(iID);
	m_iLevel = iLevel;
	m_iCurLevel = iCurLevel;
}

//------------------------------------------------------------------------------------
SPSkillActivity::~SPSkillActivity()
{
}

//------------------------------------------------------------------------------------
void SPSkillActivity::Init()
{
	m_iLevel = 0;
	m_fAccmulateTime = 0.0f;

	m_bPossibleUsed = true;
	m_bToggleOn = false;
}

//------------------------------------------------------------------------------------
void SPSkillActivity::SetSkill(SPSkill* pkSkill)
{
	m_pkSkill = pkSkill;
}

//------------------------------------------------------------------------------------
SPSkill* SPSkillActivity::GetSkill()
{
	return m_pkSkill;
}

//------------------------------------------------------------------------------------
int SPSkillActivity::GetLevel()
{
	return m_iLevel;
}

//------------------------------------------------------------------------------------
void SPSkillActivity::SetLevel(int iLevel)
{
	m_iLevel	=	iLevel;
}

//------------------------------------------------------------------------------------
int SPSkillActivity::GetCurLevel()
{
	return m_iCurLevel;
}

//------------------------------------------------------------------------------------
void SPSkillActivity::SetCurLevel( int iCurLevel )
{
	m_iCurLevel	=	iCurLevel;
}

//------------------------------------------------------------------------------------
bool SPSkillActivity::Process(float fElapsedTime)
{
	if( m_bPossibleUsed )
		return true;

	m_fAccmulateTime += fElapsedTime;
	return true;
}

//------------------------------------------------------------------------------------
void SPSkillActivity::PossibleUsedCheck()
{
	m_bPossibleUsed = true;
	m_fAccmulateTime = 0.0f;
}

//------------------------------------------------------------------------------------
bool SPSkillActivity::GetPossibleUsed()
{
	return m_bPossibleUsed;
}

//------------------------------------------------------------------------------------
void SPSkillActivity::SetToggle(bool bOn)
{
	m_bToggleOn = bOn;
}

//------------------------------------------------------------------------------------
bool SPSkillActivity::GetToggleOn()
{
	return m_bToggleOn;
}



//------------------------------------------------------------------------------------
// SPItemSkillActivity
SPItemSkillActivity::SPItemSkillActivity()
{
	Init();
}

//------------------------------------------------------------------------------------
SPItemSkillActivity::SPItemSkillActivity(SPSkill* pkSkill, int iLevel, int iCurLevel,
	int iItemID, int iContainerType, int iSlotIndex)
: SPSkillActivity(pkSkill, iLevel, iCurLevel)
{
	Init();
	
	m_iItemID = iItemID;
	m_iSlotIndex = iSlotIndex;
	m_iContainerType = iContainerType;

	m_iLevel = iLevel;
	m_iCurLevel = iCurLevel;
}

//------------------------------------------------------------------------------------
SPItemSkillActivity::~SPItemSkillActivity()
{
}

//------------------------------------------------------------------------------------
void SPItemSkillActivity::Init()
{
	SPSkillActivity::Init();

	m_iItemID = 0;
	m_iSlotIndex = 0;
	m_iContainerType = 0;
}

//------------------------------------------------------------------------------------
int SPItemSkillActivity::GetItemID()
{
	return m_iItemID;
}

//------------------------------------------------------------------------------------
int SPItemSkillActivity::GetSlotIndex()
{
	return m_iSlotIndex;
}

//------------------------------------------------------------------------------------
int SPItemSkillActivity::GetContainerType()
{
	return m_iContainerType;
}




//------------------------------------------------------------------------------------
// SPSkillUsed
SPSkillUsed::SPSkillUsed()
{
	Init();
}

//------------------------------------------------------------------------------------
SPSkillUsed::SPSkillUsed(SPGameObject* pkParent, SPSkill* pkSkill)
: SPSkillActivity(pkSkill, 0, 0)
, m_pkParent(pkParent)
{
	Init();
}

//------------------------------------------------------------------------------------
SPSkillUsed::~SPSkillUsed()
{
}

//------------------------------------------------------------------------------------
void SPSkillUsed::Init()
{
	SPSkillActivity::Init();

	m_iCurEffectCount = 0;
	m_iMaxEffectCount = 1;

	if( m_pkSkill )
	{
		SKILL_APPLY_EFFECT* pApplyEffect = m_pkSkill->GetApplyEffect();
		if( pApplyEffect )
		{
			int i;
			for( i = 1; i < MAX_EFFECT_NUM; i++ )
			{
				if( pApplyEffect->fDelayTime[i] > 0.0f )
					m_iMaxEffectCount++;
			}
		}
	}
}

//------------------------------------------------------------------------------------
bool SPSkillUsed::Process(float fElapsedTime)
{
	m_fAccmulateTime += fElapsedTime;

	SKILL_APPLY_EFFECT* pApplyEffect = m_pkSkill->GetApplyEffect();
	if( pApplyEffect == NULL )
		return false;

	if( m_iMaxEffectCount == 0 || m_iCurEffectCount >= m_iMaxEffectCount )
		return false;

	if( m_fAccmulateTime > pApplyEffect->fDelayTime[m_iCurEffectCount] )
	{
		CheckApplyEffect(pApplyEffect);
		m_iCurEffectCount++;

		if( m_iCurEffectCount >= m_iMaxEffectCount )
			return false;
	}

	return true;
}

//------------------------------------------------------------------------------------
void SPSkillUsed::CheckApplyEffect(SKILL_APPLY_EFFECT* pApplyEffect)
{
	int iIndex, iGOBCount = 0;
	int iCasterPosX, iCasterPosY, iTargetPosX, iTargetPosY, iDistance, iCompareDiff;
	int iDiffX, iDiffY;
	
	SPGameObject* pkMissileTargetObject;
	FX_SOUND_INFO FXSoundInfo;
	std::vector< SPGameObject* > vGOBList, vGOBAttackedList;
	bool bPlayerList;

	if( m_pkParent == NULL )
		return;

	SPCharacterAttack* pkCasterAttack = NULL;
	SPMonsterAttack* pkMonsterAttack = NULL;
	SPMonsterBeAttacked* pkMonsterBeAttacked = NULL;
	SPCharacterBeAttacked* pkPlayerBeAttacked = NULL;
	
	int iIsPlayer = m_pkParent->SPGOBPerformMessage(SPGM_ISPLAYER);
	iCasterPosX = m_pkParent->SPGOBPerformMessage(SPGM_GETPOSX);
	iCasterPosY = m_pkParent->SPGOBPerformMessage(SPGM_GETPOSY);
	FACING eCasterFacing = (FACING)m_pkParent->SPGOBPerformMessage(SPGM_GETFACING);

	iIndex = m_pkParent->SPGOBPerformMessage(SPGM_GETCURANIINDEX);
	if( iIndex < 0 )
		return;

	if( iIsPlayer )
	{
		g_pBattleCluster->GetAttack(iIndex, &pkCasterAttack);
		if( pkCasterAttack == NULL )
			return;
	}
	else
	{
		g_pBattleCluster->GetAttack((ANIMATION_INDEX)iIndex, &pkMonsterAttack, m_pkParent->GetClassID());
		if( pkMonsterAttack == NULL )
			return;
	}

	pkMissileTargetObject = NULL;
	iCompareDiff = 100000000;

	switch( pApplyEffect->eTargetType )
	{
	case STT_ENEMY:
		{
			if( iIsPlayer )
			{
				iGOBCount = g_pGOBManager->GetAllMonster(vGOBList);
				bPlayerList = false;
			}
			else
			{
				iGOBCount = g_pGOBManager->GetAllPlayer(vGOBList);
				bPlayerList = true;
			}
		}
		break;

	case STT_NEUTRALITY:
		{
			iGOBCount = g_pGOBManager->GetAllPlayer(vGOBList);
			bPlayerList = true;
		}
		break;
	}

	std::vector< SPGameObject* >::iterator iter = vGOBList.begin();
	for( ; iter != vGOBList.end(); ++iter )
	{
		if( (*iter)->SPGOBPerformMessage(SPGM_ISDEAD) )
			continue;

		if( (*iter)->SPGOBPerformMessage(SPGM_GETTYPE) > 100 )
			continue;

		iTargetPosX = (*iter)->SPGOBPerformMessage(SPGM_GETPOSX);
		iTargetPosY = (*iter)->SPGOBPerformMessage(SPGM_GETPOSY);

		iIndex = (*iter)->SPGOBPerformMessage(SPGM_GETCURANIINDEX);
		if( iIndex < 0 )
			return;

		bool bInAttack = false;
		int iFrame = (int)(*iter)->SPGOBPerformMessage(SPGM_GETCURANIFRAME) + 1;
		if( bPlayerList )
		{
			g_pBattleCluster->GetAttack(iIndex, &pkPlayerBeAttacked);
			if( pkPlayerBeAttacked == NULL )
				continue;

			vector< ATTACKINFO >* pAttackInfo;
			if( iIsPlayer )
				pAttackInfo = &pkCasterAttack->m_AttackInfo;
			else
				pAttackInfo = &pkMonsterAttack->m_AttackInfo;


			if( InAttackRgn(*pAttackInfo, eCasterFacing,
				iCasterPosX, iCasterPosY, iTargetPosX, iTargetPosY,
				*iter, pkPlayerBeAttacked->m_BeAttackInfo, iFrame) )
				bInAttack = true;
		}
		else
		{
			g_pBattleCluster->GetAttack((ANIMATION_INDEX)iIndex, &pkMonsterBeAttacked, (*iter)->GetClassID());
			if( pkMonsterBeAttacked == NULL )
				continue;

			vector< ATTACKINFO >* pAttackInfo;
			if( iIsPlayer )
				pAttackInfo = &pkCasterAttack->m_AttackInfo;
			else
				pAttackInfo = &pkMonsterAttack->m_AttackInfo;

			if( InAttackRgn(*pAttackInfo, eCasterFacing,
				iCasterPosX, iCasterPosY, iTargetPosX, iTargetPosY,
				*iter, pkMonsterBeAttacked->m_BeAttackInfo, iFrame) )
				bInAttack = true;
		}

		if( bInAttack )
		{
			iDiffX = iCasterPosX - iTargetPosX;
			iDiffY = iCasterPosY - iTargetPosY;

			iDistance = iDiffX * iDiffX + iDiffY * iDiffY;
			if( iDistance < iCompareDiff )
			{
				pkMissileTargetObject = (*iter);
				iCompareDiff = iDistance;
			}

			vGOBAttackedList.push_back(*iter);
		}
	}

	CheckTargetCount(iCasterPosX, iCasterPosY, pApplyEffect,
		vGOBAttackedList, FXSoundInfo.vTargetList);

	vGOBList.clear();
	vGOBAttackedList.clear();

	FXSoundInfo.pkMissileTarget = pkMissileTargetObject;
	FXSoundInfo.pApplyEffect = pApplyEffect;

	m_pkParent->SPGOBPerformMessage(SPGM_SET_EFFECT_FXSOUND, (LPARAM)&FXSoundInfo);
}

//------------------------------------------------------------------------------------
SPGameObject* SPSkillUsed::CheckAttackRgn(SPSkill * pSkill)
{
	int iIndex, iGOBCount = 0;
	int iCasterPosX, iCasterPosY, iTargetPosX, iTargetPosY;
	std::vector< SPGameObject* > vGOBList, vGOBAttackedList;
	bool bPlayerList;

	if( m_pkParent == NULL )
		return NULL;

	if ( !pSkill )
		return NULL;

	SKILL_APPLY_EFFECT * pApplyEffect = pSkill->GetApplyEffect();
	if ( !pApplyEffect )
		return NULL;
	
	SKILL_MOTION* pMotion = pSkill->GetMotion(false);
	if( !pMotion )
		return NULL;

	SPCharacterAttack* pkCasterAttack = NULL;
	SPMonsterAttack* pkMonsterAttack = NULL;
	SPMonsterBeAttacked* pkMonsterBeAttacked = NULL;
	SPCharacterBeAttacked* pkPlayerBeAttacked = NULL;
	
	int iIsPlayer = m_pkParent->SPGOBPerformMessage(SPGM_ISPLAYER);
	iCasterPosX = m_pkParent->SPGOBPerformMessage(SPGM_GETPOSX);
	iCasterPosY = m_pkParent->SPGOBPerformMessage(SPGM_GETPOSY);
	FACING eCasterFacing = (FACING)m_pkParent->SPGOBPerformMessage(SPGM_GETFACING);

	iIndex = pMotion->iSeqKeyIndex;
	if( iIndex < 0 )
		return NULL;

	if( iIsPlayer )
	{
		g_pBattleCluster->GetAttack(iIndex, &pkCasterAttack);
		if( pkCasterAttack == NULL )
			return NULL;
	}
	else
	{
		g_pBattleCluster->GetAttack((ANIMATION_INDEX)iIndex, &pkMonsterAttack, m_pkParent->GetClassID());
		if( pkMonsterAttack == NULL )
			return NULL;
	}

	if( iIsPlayer )
	{
		iGOBCount = g_pGOBManager->GetAllMonster(vGOBList);
		bPlayerList = false;
	}
	else
	{
		iGOBCount = g_pGOBManager->GetAllPlayer(vGOBList);
		bPlayerList = true;
	}

	std::vector< SPGameObject* >::iterator iter = vGOBList.begin();
	for( ; iter != vGOBList.end(); ++iter )
	{
		if( (*iter)->SPGOBPerformMessage(SPGM_ISDEAD) )
			continue;

		if( (*iter)->SPGOBPerformMessage(SPGM_GETTYPE) > 100 )
			continue;

		if( (*iter)->SPGOBPerformMessage(SPGM_GETTYPE) == 21 )
			continue;

		if( (*iter)->IsShowName() == false )
			continue;

		if( (*iter)->IsShowHP() == false )
			continue;

		iTargetPosX = (*iter)->SPGOBPerformMessage(SPGM_GETPOSX);
		iTargetPosY = (*iter)->SPGOBPerformMessage(SPGM_GETPOSY);

		iIndex = (*iter)->SPGOBPerformMessage(SPGM_GETCURANIINDEX);
		if( iIndex < 0 )
			return NULL;

		bool bInAttack = false;
		int iFrame = (int)(*iter)->SPGOBPerformMessage(SPGM_GETCURANIFRAME) + 1;
		if( bPlayerList )
		{
			g_pBattleCluster->GetAttack(iIndex, &pkPlayerBeAttacked);
			if( pkPlayerBeAttacked == NULL )
				continue;

			vector< ATTACKINFO >* pAttackInfo;
			if( iIsPlayer )
				pAttackInfo = &pkCasterAttack->m_AttackInfo;
			else
				pAttackInfo = &pkMonsterAttack->m_AttackInfo;


			if( InAttackRect(*pAttackInfo, eCasterFacing,
				iCasterPosX, iCasterPosY, iTargetPosX, iTargetPosY,
				*iter, pkPlayerBeAttacked->m_BeAttackInfo, iFrame) )
				bInAttack = true;
		}
		else
		{
			g_pBattleCluster->GetAttack((ANIMATION_INDEX)iIndex, &pkMonsterBeAttacked, (*iter)->GetClassID());
			if( pkMonsterBeAttacked == NULL )
				continue;

			vector< ATTACKINFO >* pAttackInfo;
			if( iIsPlayer )
				pAttackInfo = &pkCasterAttack->m_AttackInfo;
			else
				pAttackInfo = &pkMonsterAttack->m_AttackInfo;

			if( InAttackRect(*pAttackInfo, eCasterFacing,
				iCasterPosX, iCasterPosY, iTargetPosX, iTargetPosY,
				*iter, pkMonsterBeAttacked->m_BeAttackInfo, iFrame) )
				bInAttack = true;
		}

		if( bInAttack )
			return (*iter);
	}
	return NULL;
}

//------------------------------------------------------------------------------------
SPGameObject* SPSkillUsed::CheckAttackRgn(int iActionCommand)
{
	int iIndex, iGOBCount = 0;
	int iCasterPosX, iCasterPosY, iTargetPosX, iTargetPosY;
	std::vector< SPGameObject* > vGOBList, vGOBAttackedList;
	bool bPlayerList;

	if( m_pkParent == NULL )
		return NULL;

	if ( iActionCommand < 0 )
		return NULL;

	SPCharacterAttack* pkCasterAttack = NULL;
	SPMonsterAttack* pkMonsterAttack = NULL;
	SPMonsterBeAttacked* pkMonsterBeAttacked = NULL;
	SPCharacterBeAttacked* pkPlayerBeAttacked = NULL;
	
	int iIsPlayer = m_pkParent->SPGOBPerformMessage(SPGM_ISPLAYER);
	iCasterPosX = m_pkParent->SPGOBPerformMessage(SPGM_GETPOSX);
	iCasterPosY = m_pkParent->SPGOBPerformMessage(SPGM_GETPOSY);
	FACING eCasterFacing = (FACING)m_pkParent->SPGOBPerformMessage(SPGM_GETFACING);

	iIndex = iActionCommand;

	if( iIsPlayer )
	{
		g_pBattleCluster->GetAttack(iIndex, &pkCasterAttack);
		if( pkCasterAttack == NULL )
			return NULL;
	}
	else
	{
		g_pBattleCluster->GetAttack((ANIMATION_INDEX)iIndex, &pkMonsterAttack, m_pkParent->GetClassID());
		if( pkMonsterAttack == NULL )
			return NULL;
	}

	if( iIsPlayer )
	{
		iGOBCount = g_pGOBManager->GetAllMonster(vGOBList);
		bPlayerList = false;
	}
	else
	{
		iGOBCount = g_pGOBManager->GetAllPlayer(vGOBList);
		bPlayerList = true;
	}

	std::vector< SPGameObject* >::iterator iter = vGOBList.begin();
	for( ; iter != vGOBList.end(); ++iter )
	{
		if( (*iter)->SPGOBPerformMessage(SPGM_ISDEAD) )
			continue;

		if( (*iter)->SPGOBPerformMessage(SPGM_GETTYPE) > 100 )
			continue;

		if( (*iter)->SPGOBPerformMessage(SPGM_GETTYPE) == 21 )
			continue;

		if( (*iter)->IsShowName() == false )
			continue;

		if( (*iter)->IsShowHP() == false )
			continue;

		iTargetPosX = (*iter)->SPGOBPerformMessage(SPGM_GETPOSX);
		iTargetPosY = (*iter)->SPGOBPerformMessage(SPGM_GETPOSY);

		iIndex = (*iter)->SPGOBPerformMessage(SPGM_GETCURANIINDEX);
		if( iIndex < 0 )
			return NULL;

		bool bInAttack = false;
		int iFrame = (int)(*iter)->SPGOBPerformMessage(SPGM_GETCURANIFRAME) + 1;
		if( bPlayerList )
		{
			g_pBattleCluster->GetAttack(iIndex, &pkPlayerBeAttacked);
			if( pkPlayerBeAttacked == NULL )
				continue;

			vector< ATTACKINFO >* pAttackInfo;
			if( iIsPlayer )
				pAttackInfo = &pkCasterAttack->m_AttackInfo;
			else
				pAttackInfo = &pkMonsterAttack->m_AttackInfo;


			if( InAttackRect(*pAttackInfo, eCasterFacing,
				iCasterPosX, iCasterPosY, iTargetPosX, iTargetPosY,
				*iter, pkPlayerBeAttacked->m_BeAttackInfo, iFrame) )
				bInAttack = true;
		}
		else
		{
			g_pBattleCluster->GetAttack((ANIMATION_INDEX)iIndex, &pkMonsterBeAttacked, (*iter)->GetClassID());
			if( pkMonsterBeAttacked == NULL )
				continue;

			vector< ATTACKINFO >* pAttackInfo;
			if( iIsPlayer )
				pAttackInfo = &pkCasterAttack->m_AttackInfo;
			else
				pAttackInfo = &pkMonsterAttack->m_AttackInfo;

			if( InAttackRect(*pAttackInfo, eCasterFacing,
				iCasterPosX, iCasterPosY, iTargetPosX, iTargetPosY,
				*iter, pkMonsterBeAttacked->m_BeAttackInfo, iFrame) )
				bInAttack = true;
		}

		if( bInAttack )
			return (*iter);
	}
	return NULL;
}

//------------------------------------------------------------------------------------
void SPSkillUsed::SetApplyEffect(SPEffectArchive* pkEffectArchive,
	SKILL_APPLY_EFFECT* pApplyEffect, bool bSelf)
{
	if( pkEffectArchive == NULL || pApplyEffect == NULL )
		return;

	SPEffect* pkEffect;
	int i;
	int* pEffectID;
	
	if( bSelf )
		pEffectID = pApplyEffect->iSelfEffect;
	else
		pEffectID = pApplyEffect->iTargetEffect;

	for( i = 0; i < 4; i++ )
	{
		if( pEffectID[i] )
		{
			pkEffect = SPEffectManager::GetInstance()->GetEffect(pEffectID[i]);
			if( pkEffect == NULL )
				continue;

			SPEffectActivity* pkEffectActivity = new SPEffectActivity(pkEffect, 0, 0.0f, bSelf);
			pkEffectArchive->AddEffect(0, pkEffectActivity);
		}
	}
}

//------------------------------------------------------------------------------------
bool SPSkillUsed::InAttackRgn(std::vector< ATTACKINFO > & vAttackInfo, FACING eCasterFacing,
	int iCasterPosX, int iCasterPosY, int iTargetPosX, int iTargetPosY,
	SPGameObject* pkTargetObject, std::vector< BEATTACKEDINFO > vBeAttackedInfo, int iFrame)
{
	RECT rcBeAttackedRgn;

	vector< BEATTACKEDINFO >::iterator iterBeAttacked = vBeAttackedInfo.begin();
	while( iterBeAttacked != vBeAttackedInfo.end() )
	{
		if( iFrame <= (*iterBeAttacked).m_nIndex )
		{
			FACING eTargetFacing = (FACING)pkTargetObject->SPGOBPerformMessage(SPGM_GETFACING);
			if( eTargetFacing == FACING_RIGHT )
			{
				rcBeAttackedRgn.left = (LONG)(iTargetPosX
					+ (*iterBeAttacked).m_rtRange.left);
				rcBeAttackedRgn.right = (LONG)(iTargetPosX
					+(*iterBeAttacked).m_rtRange.right);
			}
			else
			{
				rcBeAttackedRgn.left = (LONG)(iTargetPosX
					- (*iterBeAttacked).m_rtRange.right);
				rcBeAttackedRgn.right = (LONG)(iTargetPosX
					- (*iterBeAttacked).m_rtRange.left);
			}

			rcBeAttackedRgn.top = (LONG)(iTargetPosY
				+ (*iterBeAttacked).m_rtRange.top);
			rcBeAttackedRgn.bottom = (LONG)(iTargetPosY
				+ (*iterBeAttacked).m_rtRange.bottom);

			rcBeAttackedRgn.left -= iCasterPosX;
			rcBeAttackedRgn.right -= iCasterPosX;
			rcBeAttackedRgn.top -= iCasterPosY;
			rcBeAttackedRgn.bottom -= iCasterPosY;

			if( g_pBattleCluster->IsCollision(rcBeAttackedRgn, vAttackInfo, eCasterFacing) )
				return true;

			break;
		}

		++iterBeAttacked;
	}

	return false;
}

//------------------------------------------------------------------------------------
bool SPSkillUsed::InAttackRgn(std::vector< ATTACKINFO > & vAttackInfo, FACING eCasterFacing,
	int iCasterPosX, int iCasterPosY, int iTargetPosX, int iTargetPosY,
	SPGameObject* pkTargetObject, std::vector< APCBEATTACKEDINFO > vBeAttackedInfo, int iFrame)
{
	RECT rcBeAttackedRgn;

	vector< APCBEATTACKEDINFO >::iterator iterBeAttacked = vBeAttackedInfo.begin();
	while( iterBeAttacked != vBeAttackedInfo.end() )
	{
		if( iFrame <= (*iterBeAttacked).m_nIndex )
		{
			FACING eTargetFacing = (FACING)pkTargetObject->SPGOBPerformMessage(SPGM_GETFACING);
			if( eTargetFacing == FACING_RIGHT )
			{
				rcBeAttackedRgn.left = (LONG)(iTargetPosX
					+ (*iterBeAttacked).m_rtRange.left);
				rcBeAttackedRgn.right = (LONG)(iTargetPosX
					+(*iterBeAttacked).m_rtRange.right);
			}
			else
			{
				rcBeAttackedRgn.left = (LONG)(iTargetPosX
					- (*iterBeAttacked).m_rtRange.right);
				rcBeAttackedRgn.right = (LONG)(iTargetPosX
					- (*iterBeAttacked).m_rtRange.left);
			}

			rcBeAttackedRgn.top = (LONG)(iTargetPosY
				+ (*iterBeAttacked).m_rtRange.top);
			rcBeAttackedRgn.bottom = (LONG)(iTargetPosY
				+ (*iterBeAttacked).m_rtRange.bottom);

			rcBeAttackedRgn.left -= iCasterPosX;
			rcBeAttackedRgn.right -= iCasterPosX;
			rcBeAttackedRgn.top -= iCasterPosY;
			rcBeAttackedRgn.bottom -= iCasterPosY;

			if( g_pBattleCluster->IsCollision(rcBeAttackedRgn, vAttackInfo, eCasterFacing) )
				return true;

			break;
		}

		++iterBeAttacked;
	}

	return false;
}

//------------------------------------------------------------------------------------
bool SPSkillUsed::InAttackRect(std::vector< ATTACKINFO > & vAttackInfo, FACING eCasterFacing,
	int iCasterPosX, int iCasterPosY, int iTargetPosX, int iTargetPosY,
	SPGameObject* pkTargetObject, std::vector< APCBEATTACKEDINFO > vBeAttackedInfo, int iFrame)
{
	RECT rcBeAttackedRgn;

	vector< APCBEATTACKEDINFO >::iterator iterBeAttacked = vBeAttackedInfo.begin();
	while( iterBeAttacked != vBeAttackedInfo.end() )
	{
		if( iFrame <= (*iterBeAttacked).m_nIndex )
		{
			FACING eTargetFacing = (FACING)pkTargetObject->SPGOBPerformMessage(SPGM_GETFACING);
			if( eTargetFacing == FACING_RIGHT )
			{
				rcBeAttackedRgn.left = (LONG)(iTargetPosX
					+ (*iterBeAttacked).m_rtRange.left);
				rcBeAttackedRgn.right = (LONG)(iTargetPosX
					+(*iterBeAttacked).m_rtRange.right);
			}
			else
			{
				rcBeAttackedRgn.left = (LONG)(iTargetPosX
					- (*iterBeAttacked).m_rtRange.right);
				rcBeAttackedRgn.right = (LONG)(iTargetPosX
					- (*iterBeAttacked).m_rtRange.left);
			}

			rcBeAttackedRgn.top = (LONG)(iTargetPosY
				+ (*iterBeAttacked).m_rtRange.top);
			rcBeAttackedRgn.bottom = (LONG)(iTargetPosY
				+ (*iterBeAttacked).m_rtRange.bottom);

			//
			RECT rcAttacker;
			ATTACKINFO* pAttackInfo;
			vector< ATTACKINFO >::const_iterator iter = vAttackInfo.begin();
			while( iter != vAttackInfo.end() )
			{
				pAttackInfo = (ATTACKINFO*)&(*iter);
				if( pAttackInfo )
				{
					int iWidth = pAttackInfo->m_rtRange.right - pAttackInfo->m_rtRange.left;
					if( eCasterFacing == FACING_RIGHT )
						rcAttacker.left = iCasterPosX + pAttackInfo->m_rtRange.left;
					else
						rcAttacker.left = iCasterPosX - pAttackInfo->m_rtRange.left - iWidth;

					rcAttacker.right  = rcAttacker.left + iWidth;
					rcAttacker.top    = iCasterPosY + pAttackInfo->m_rtRange.top;
					rcAttacker.bottom = iCasterPosY + pAttackInfo->m_rtRange.bottom;

					if( !((rcBeAttackedRgn.left > rcAttacker.right) || (rcBeAttackedRgn.right < rcAttacker.left))
						&& !((rcBeAttackedRgn.top > rcAttacker.bottom) || (rcBeAttackedRgn.bottom < rcAttacker.top)) )
						return true;
				}
				++iter;
			}

			break;
		}

		++iterBeAttacked;
	}

	return false;
}

//------------------------------------------------------------------------------------
void SPSkillUsed::CheckTargetCount(int iCasterPosX, int iCasterPosY,
	SKILL_APPLY_EFFECT* pApplyEffect,
	vector< SPGameObject* >& vGameObject, vector< SPGameObject* >& vTargetObject)
{
	vTargetObject.clear();
	if( vGameObject.empty() )
		return;

	int iObjCount = (int)vGameObject.size();
	if( iObjCount <= pApplyEffect->iTargetNum )
		vTargetObject = vGameObject;
	else
	{
		IsLessDistance::iPosX = iCasterPosX;
		IsLessDistance::iPosY = iCasterPosY;

		sort(vGameObject.begin(), vGameObject.end(), IsLessDistance::compare);

		int iCount = 0;
		vector< SPGameObject* >::iterator iter = vGameObject.begin();
		while( iter != vGameObject.end() )
		{
			vTargetObject.push_back(*iter);
			
			++iCount;
			++iter;

			if( iCount >= pApplyEffect->iTargetNum )
				break;
		}
	}
}





//------------------------------------------------------------------------------------
// SPMotionUsed
SPMotionUsed::SPMotionUsed()
{
	Init();
}

//------------------------------------------------------------------------------------
SPMotionUsed::SPMotionUsed(SPGameObject* pkParent, SPMotionStatus* pkMotionUsed, unsigned int uiAniIndex)
: SPSkillUsed(pkParent, 0)
, m_pkMotionStatus(pkMotionUsed)
, m_uiAniIndex(uiAniIndex)
{
	Init();
}

//------------------------------------------------------------------------------------
SPMotionUsed::~SPMotionUsed()
{
}

//------------------------------------------------------------------------------------
void SPMotionUsed::Init()
{
	SPSkillUsed::Init();

	m_iMaxEffectCount = 1;
	if( m_pkMotionStatus )
	{
		SKILL_APPLY_EFFECT* pApplyEffect = m_pkMotionStatus->GetApplyEffect();
		if( pApplyEffect )
		{
			int i;
			for( i = 1; i < MAX_EFFECT_NUM; i++ )
			{
				if( pApplyEffect->fDelayTime[i] > 0.0f )
					m_iMaxEffectCount++;
			}
		}
	}
}

//------------------------------------------------------------------------------------
bool SPMotionUsed::Process(float fElapsedTime)
{
	m_fAccmulateTime += fElapsedTime;

	SKILL_APPLY_EFFECT* pApplyEffect = m_pkMotionStatus->GetApplyEffect();
	if( pApplyEffect == NULL )
		return false;

	if( m_iMaxEffectCount == 0 || m_iCurEffectCount >= m_iMaxEffectCount )
		return false;

	if( m_fAccmulateTime > pApplyEffect->fDelayTime[m_iCurEffectCount] )
	{
		CheckApplyEffect(pApplyEffect);
		m_iCurEffectCount++;

		if( m_iCurEffectCount >= m_iMaxEffectCount )
			return false;
	}

	return true;
}

//------------------------------------------------------------------------------------
SPMotionStatus* SPMotionUsed::GetMotionStatus()
{
	return m_pkMotionStatus;
}

//------------------------------------------------------------------------------------
unsigned int SPMotionUsed::GetAniIndex()
{
	return m_uiAniIndex;
}





//------------------------------------------------------------------------------------
// SPSkillArchive
SPSkillArchive::SPSkillArchive(SPGameObject* pkParent)
: m_pkParent(pkParent)
, m_iCurSkillIndex(0)
{
	for( int i = 0 ; i < SIT_NUM ; i++ )
		m_iTotalSkillCount[i]	=	0;
}

//------------------------------------------------------------------------------------
SPSkillArchive::~SPSkillArchive()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPSkillArchive::Clear()
{
	//[xialin 2008/08/06]add
	if ( m_pkParent && m_pkParent->GetClassID() == CLASS_ID_AVATAR
		&& ((SPPlayer *)m_pkParent)->GetLobbyAvatar() == LOBBY_CASH )
	{
		m_mSkillList.clear();
		return;
	}


	map< int, SPSkillActivity* >::iterator iterKey = m_mSkillList.begin();
	while( iterKey != m_mSkillList.end() )
	{
		delete (*iterKey).second;
		iterKey++;
	}
	m_mSkillList.clear();

	m_iCurSkillIndex = 0;
	for( int i = 0 ; i < SIT_NUM ; i++ )
		m_iTotalSkillCount[i]	=	0;

	iterKey = m_mUsedSkillList.begin();
	while( iterKey != m_mUsedSkillList.end() )
	{
		delete (*iterKey).second;
		iterKey++;
	}
	m_mUsedSkillList.clear();

	map< int, SPMotionUsed* >::iterator iterMotion = m_mUsedMotionList.begin();
	while( iterMotion != m_mUsedMotionList.end() )
	{
		delete (*iterMotion).second;
		iterMotion++;
	}
	m_mUsedMotionList.clear();

	vector< SPItemSkillActivity* >::iterator iterItemSkill = m_vItemSkillList.begin();
	while( iterItemSkill != m_vItemSkillList.end() )
	{
		delete (*iterItemSkill);
		iterItemSkill++;
	}
	m_vItemSkillList.clear();

	iterKey = m_mLoveSkillList.begin();
	while( iterKey != m_mLoveSkillList.end() )
	{
		delete (*iterKey).second;
		iterKey++;
	}
	m_mLoveSkillList.clear();
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::AddSkill(int iOrder, SPSkillActivity* pkSkillActivity)
{
	SPSkill* pkSkill = pkSkillActivity->GetSkill();
	if( pkSkill == NULL || IsExistSkill(pkSkill) )
		return false;

	map< int, SPSkillActivity* >::iterator iterKey;
	iterKey = m_mSkillList.find(iOrder);
	if( iterKey != m_mSkillList.end() )
		return false;

	m_mSkillList.insert(make_pair(iOrder, pkSkillActivity));

	m_iTotalSkillCount[SIT_NULL]++;
	
	SKILL_UI_INFO* pkSkillUIInfo = pkSkill->GetUIInfo();
	if( pkSkillUIInfo != NULL )
		m_iTotalSkillCount[pkSkillUIInfo->eInventoryType]++;

	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::AddSkill(int iOrder, SPSkill* pkSkill, unsigned int uiLevel, unsigned int uiCurLevel)
{
	SPSkillActivity* pkSkillActivity = GetSkillActivity(pkSkill);
	if( pkSkillActivity )
	{
		pkSkillActivity->SetLevel(uiLevel);
		pkSkillActivity->SetCurLevel(uiCurLevel);
	}
	else
	{
		pkSkillActivity = new SPSkillActivity(pkSkill, uiLevel, uiCurLevel);
		if( !AddSkill(iOrder, pkSkillActivity) )
		{
			delete pkSkillActivity;
			return false;
		}
	}

	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::RemoveSkill(SPSkillActivity* pkSkillActivity)
{
	if( pkSkillActivity == NULL )
		return false;

	map< int, SPSkillActivity* >::iterator iterKey;
	iterKey = m_mSkillList.begin();
	while( iterKey != m_mSkillList.end() )
	{
		if( (*iterKey).second == pkSkillActivity )
		{
			SPSkill* pkSkill = pkSkillActivity->GetSkill();
			if( pkSkill )
			{
				SKILL_UI_INFO* pUIInfo = pkSkill->GetUIInfo();
				if( pUIInfo )
					m_iTotalSkillCount[pUIInfo->eInventoryType]--;
			}
			
			delete (*iterKey).second;
			m_mSkillList.erase(iterKey);
			m_iTotalSkillCount[SIT_NULL]--;

			return true;
		}

		++iterKey;
	}
	
	return false;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::RemoveSkill(int iID)
{
	SPSkill* pkSkill;

	map< int, SPSkillActivity* >::iterator iterKey;
	iterKey = m_mSkillList.begin();
	while( iterKey != m_mSkillList.end() )
	{
		pkSkill = (*iterKey).second->GetSkill();
		if( pkSkill && pkSkill->GetID() == iID )
		{
			SKILL_UI_INFO* pUIInfo = pkSkill->GetUIInfo();
			if( pUIInfo )
				m_iTotalSkillCount[pUIInfo->eInventoryType]--;

			delete (*iterKey).second;
			m_mSkillList.erase(iterKey);
			m_iTotalSkillCount[SIT_NULL]--;

			return true;
		}

		++iterKey;
	}

	return false;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::AddSkill(SPItemSkillActivity* pkItemSkillActivity)
{
	if( pkItemSkillActivity == NULL )
		return false;

	m_vItemSkillList.push_back(pkItemSkillActivity);
	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::RemoveSkill(int iSkillID, int iItemID, int iContainerType, int iSlotIndex)
{
	SPSkill* pkSkill;

	vector< SPItemSkillActivity* >::iterator iter = m_vItemSkillList.begin();
	while( iter != m_vItemSkillList.end() )
	{
		pkSkill = (*iter)->GetSkill();
		if( pkSkill )
		{
			if( (pkSkill->GetID() == iSkillID) && ((*iter)->GetItemID() == iItemID) &&
				 ((*iter)->GetContainerType() == iContainerType) && ((*iter)->GetSlotIndex() == iSlotIndex) )
			{
				delete (*iter);
				m_vItemSkillList.erase(iter);
				return true;
			}
		}

		++iter;
	}

	return false;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::UsedSkill(SPSkillActivity* pkSkillActivity)
{
	SPSkill* pkSkill = pkSkillActivity->GetSkill();
	if( pkSkill == NULL || IsExistUsedSkill(pkSkill) )
		return false;

	m_mUsedSkillList.insert(make_pair(pkSkill->GetID(), pkSkillActivity));
	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::RemoveUsedSkill(SPSkill* pkSkill)
{
	if( pkSkill == NULL )
		return false;

	std::map< int, SPSkillActivity* >::iterator iterKey;
	iterKey = m_mUsedSkillList.find(pkSkill->GetID());
	if( iterKey != m_mUsedSkillList.end() )
	{
		delete (*iterKey).second;
		m_mUsedSkillList.erase(iterKey);
		return true;
	}
	
	return false;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::UsedMotion(SPMotionUsed* pkMotionUsed)
{
	SPMotionStatus* pkMotionStatus = pkMotionUsed->GetMotionStatus();
	if( pkMotionStatus == NULL || IsExistUsedMotion(pkMotionStatus) )
		return false;

	m_mUsedMotionList.insert(make_pair(pkMotionUsed->GetAniIndex(), pkMotionUsed));
	return true;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::RemoveUsedMotion(unsigned int uiAniIndex)
{
	std::map< int, SPMotionUsed* >::iterator iterKey;
	iterKey = m_mUsedMotionList.find(uiAniIndex);
	if( iterKey != m_mUsedMotionList.end() )
	{
		delete (*iterKey).second;
		m_mUsedMotionList.erase(iterKey);
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------------
SPSkillActivity* SPSkillArchive::GetSkillActivity(int iID)
{
	SPSkill* pkSkill;

	map< int, SPSkillActivity* >::iterator iterKey;
	iterKey = m_mSkillList.begin();
	while( iterKey != m_mSkillList.end() )
	{
		pkSkill = (*iterKey).second->GetSkill();
		if( pkSkill && pkSkill->GetID() == iID )
			return (*iterKey).second;

		++iterKey;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
SPSkillActivity* SPSkillArchive::GetSkillActivity(SPSkill* pkSkill)
{
	map< int, SPSkillActivity* >::iterator iterKey;
	iterKey = m_mSkillList.begin();
	while( iterKey != m_mSkillList.end() )
	{
		if( (*iterKey).second->GetSkill() == pkSkill ) 
			return (*iterKey).second;

		++iterKey;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
int SPSkillArchive::GetSkillCount(SKILL_INVENTORY_TYPE eType)
{
	return m_iTotalSkillCount[eType];
}

//------------------------------------------------------------------------------------
SPSkillActivity* SPSkillArchive::GetFirstSkillActivity(SKILL_INVENTORY_TYPE eType)
{
	m_iCurSkillIndex = 0;
	SPSkillActivity* pkSkillActivity = GetCurSkillActivity(eType);
	return pkSkillActivity;

}

//------------------------------------------------------------------------------------
SPSkillActivity* SPSkillArchive::GetNextSkillActivity(SKILL_INVENTORY_TYPE eType)
{
	m_iCurSkillIndex++;
	SPSkillActivity* pkSkillActivity = GetCurSkillActivity(eType);
	return pkSkillActivity;
}

//------------------------------------------------------------------------------------
SPSkillActivity* SPSkillArchive::GetCurSkillActivity(SKILL_INVENTORY_TYPE eType)
{
	if( m_iCurSkillIndex >= m_iTotalSkillCount[SIT_NULL] )
		return NULL;

	int iCount = 0;
	SPSkill* pkSkill;
	SKILL_UI_INFO* pUIInfo;

	map< int, SPSkillActivity* >::iterator iterKey;
	iterKey = m_mSkillList.begin();
	while( iterKey != m_mSkillList.end() )
	{
		pkSkill = (*iterKey).second->GetSkill();
		pUIInfo = pkSkill->GetUIInfo();
		
		if( eType == SIT_NULL )
		{
			if( iCount == m_iCurSkillIndex )
				return (*iterKey).second;

			++iCount;
		}
		else
		{
			if( pUIInfo && pUIInfo->eInventoryType == eType )
			{
				if( iCount == m_iCurSkillIndex )
					return (*iterKey).second;

				++iCount;
			}
		}

		++iterKey;
	}
	
	return NULL;
}

//------------------------------------------------------------------------------------
SPItemSkillActivity* SPSkillArchive::GetItemSkillActivity(int iSkillID, 
	int iContainerType, int iSlotIndex)
{
	SPSkill* pkSkill;

	vector< SPItemSkillActivity* >::iterator iter = m_vItemSkillList.begin();
	while( iter != m_vItemSkillList.end() )
	{
		pkSkill = (*iter)->GetSkill();
		if( pkSkill )
		{
			if( (pkSkill->GetID() == iSkillID) &&
				((*iter)->GetContainerType() == iContainerType) && ((*iter)->GetSlotIndex() == iSlotIndex) )
			{
				return (*iter);
			}
		}

		++iter;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
SPItemSkillActivity* SPSkillArchive::GetItemSkillActivity(int iIndex)
{
	int iSize = (int)m_vItemSkillList.size();
	
	if( iIndex < 0 || iIndex >= iSize )
		return NULL;

	vector< SPItemSkillActivity* >::iterator iter = m_vItemSkillList.begin() + iIndex;
	return (*iter);
}

//------------------------------------------------------------------------------------
int SPSkillArchive::GetItemSkillCount()
{
	return (int)m_vItemSkillList.size();
}

//------------------------------------------------------------------------------------
SPSkillActivity* SPSkillArchive::GetLoveSkillActivity( int iSkillID )
{
	SPSkill*	pLoveSkill	=	SPSkillManager::GetInstance()->GetSkill( iSkillID );

	if( pLoveSkill == NULL )
		return NULL;

	std::map< int, SPSkillActivity* >::iterator	IterFind	=	m_mLoveSkillList.find( iSkillID );

	if( IterFind != m_mLoveSkillList.end() )
		return (*IterFind).second;

	SPSkillActivity*	pLoveSkillActivity	=	new SPSkillActivity( iSkillID , pLoveSkill->GetBaseLevel() , pLoveSkill->GetBaseLevel() );

	if( pLoveSkillActivity == NULL )
		return NULL;

	m_mLoveSkillList.insert( make_pair( iSkillID , pLoveSkillActivity ) );

	return pLoveSkillActivity;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::IsExistSkill(SPSkill* pkSkill)
{
	map< int, SPSkillActivity* >::iterator iterKey;
	iterKey = m_mSkillList.begin();
	while( iterKey != m_mSkillList.end() )
	{
		if( (*iterKey).second->GetSkill() == pkSkill )
			return true;

		++iterKey;
	}
	
	return false;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::IsExistSkill(int iID)
{
	SPSkill* pkSkill;

	map< int, SPSkillActivity* >::iterator iterKey;
	iterKey = m_mSkillList.begin();
	while( iterKey != m_mSkillList.end() )
	{
		pkSkill = (*iterKey).second->GetSkill();
		if( pkSkill && pkSkill->GetID() == iID )
			return true;

		++iterKey;
	}

	return false;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::IsExistUsedSkill(SPSkill* pkSkill)
{
	map< int, SPSkillActivity* >::iterator iterKey;
	iterKey = m_mUsedSkillList.begin();
	while( iterKey != m_mUsedSkillList.end() )
	{
		if( (*iterKey).second->GetSkill() == pkSkill )
			return true;

		++iterKey;
	}

	return false;
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::IsExistUsedMotion(SPMotionStatus* pkMotion)
{
	map< int, SPMotionUsed* >::iterator iterKey;
	iterKey = m_mUsedMotionList.begin();
	while( iterKey != m_mUsedMotionList.end() )
	{
		if( (*iterKey).second->GetMotionStatus() == pkMotion )
			return true;

		++iterKey;
	}

	return false;
}

//------------------------------------------------------------------------------------
void SPSkillArchive::Process(float fElapsedTime)
{
	map< int, SPSkillActivity* >::iterator iterKey;
	iterKey = m_mSkillList.begin();
	while( iterKey != m_mSkillList.end() )
	{
		(*iterKey).second->Process(fElapsedTime);
		++iterKey;
	}

	iterKey = m_mUsedSkillList.begin();
	while( iterKey != m_mUsedSkillList.end() )
	{
		if( !(*iterKey).second->Process(fElapsedTime) )
		{
			delete (*iterKey).second;
			iterKey = m_mUsedSkillList.erase(iterKey);
			continue;
		}

		++iterKey;
	}

	map< int, SPMotionUsed* >::iterator iterMotion = m_mUsedMotionList.begin();
	while( iterMotion != m_mUsedMotionList.end() )
	{
		if( !(*iterMotion).second->Process(fElapsedTime) )
		{
			delete (*iterMotion).second;
			iterMotion = m_mUsedMotionList.erase(iterMotion);
			continue;
		}

		++iterMotion;
	}
}

//------------------------------------------------------------------------------------
bool SPSkillArchive::IsPossibleUsed(int iID)
{
	SPSkillActivity* pkSkillActivity = GetSkillActivity(iID);
	if( pkSkillActivity == NULL )
		return false;

	return pkSkillActivity->GetPossibleUsed();
}

//------------------------------------------------------------------------------------
void SPSkillArchive::ReInitSkill( UINT32 uiSaveCheck )
{
	if( uiSaveCheck <= 0 )
		return;

	SPSkillActivity*	pSkillActivity	=	GetFirstSkillActivity();
	SPSkill*			pSkill			=	NULL;
	std::vector<int>	vRemoveSkillID;
	UINT32				uiGetSaveCheck	=	0;

	if( ( uiSaveCheck & SSC_SKILL ) == SSC_SKILL )
		uiSaveCheck = SSC_SKILL;

	if( ( uiSaveCheck & SSC_ITEMMIX ) == SSC_ITEMMIX )
		uiSaveCheck = SSC_ITEMMIX;

	while( pSkillActivity != NULL )
	{
		pSkill	=	pSkillActivity->GetSkill();

		if( pSkill != NULL )
		{
			//	스킬이면 모든 스킬에 대해 초기화를 한다.
			if( uiSaveCheck == SSC_SKILL )
			{
				if( pSkill->GetSaveCheck() == 0 )
					vRemoveSkillID.push_back( pSkill->GetID() );
				else if( pSkill->GetSaveCheck() == uiSaveCheck )
					pSkillActivity->SetLevel( pSkill->GetBaseLevel() );
			}
			else
			{
				uiGetSaveCheck	=	(UINT32)pSkill->GetSaveCheck();

				if( uiGetSaveCheck != 0 && ( ( uiGetSaveCheck & uiSaveCheck ) == uiGetSaveCheck ) )
					vRemoveSkillID.push_back( pSkill->GetID() );
			}
		}

		pSkillActivity	=	GetNextSkillActivity();
	}

	int	iTotalRemoveCount	=	(int)vRemoveSkillID.size();
	int	iRemoveSkillID;

	for( int i = 0 ; i < iTotalRemoveCount ; i++ )
	{
		iRemoveSkillID	=	vRemoveSkillID.at( i );

		RemoveSkill( iRemoveSkillID );
	}

	return;
}

//------------------------------------------------------------------------------------
void SPSkillArchive::SetToggleSkill(int iEffectID, bool bOn)
{
	SPSkill* pkSkill;

	map< int, SPSkillActivity* >::iterator iterSkill = m_mSkillList.begin();
	while( iterSkill != m_mSkillList.end() )
	{
		pkSkill = ((*iterSkill).second)->GetSkill();
		if( pkSkill && pkSkill->GetType() == ST_TOGGLE && pkSkill->GetToggleEffectID() == iEffectID )
			((*iterSkill).second)->SetToggle(bOn);

		++iterSkill;
	}

	vector< SPItemSkillActivity* >::iterator iterItem = m_vItemSkillList.begin();
	while( iterItem != m_vItemSkillList.end() )
	{
		pkSkill = (*iterItem)->GetSkill();
		if( pkSkill && pkSkill->GetType() == ST_TOGGLE && pkSkill->GetToggleEffectID() == iEffectID )
			(*iterItem)->SetToggle(bOn);

		++iterItem;
	}
}