
#include "SPCommon.h"
#include "SPDebug.h"
#include "SPUtil.h"
#include "SPMainGameDEF.H"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"

#include "SPRenderModelDEF.h"
#include "SPGOBClusterDef.h"
#include "SPGOBModelUnit.h"
#include "SPMonsterModelUnit.h"
#include "SPMOBLayerModelUnit.h"

#include "SPManager.h"
//#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPGOBManager.h"
#include "SPCameraManager.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXFrameAnimationUnit.h"
#include "SPFXManager.h"
#include "SPFXDamageUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"


#include "SPSoundArchive.h"

#include "SPCommandConvert.h"
#include "SPMonsterModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPMonsterCoordPhysics.h"
#include "SPFlyCoordPhysics.h"
#include "SPGOBStatus.h"
#include "SPMonsterStatus.h"
#include "SPMonsterStatusModel.h"

#include "SPEffect.h"
#include "SPEffectArchive.h"
#include "SPEffectManager.h"

#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPMotionStatus.h"
#include "SPSkillArchive.h"
#include "SPSkillManager.h"

#include "SPBattle.h"
#include "SPBattleCluster.h"
#include "SPBattleLoader.h"

#include "Packet.h"

#include "SPMonster.h"
#include "SPUtil.h"
#include "SPResourceDef.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPSoundManager.h"
#include "SPMOBCluster.h"

#include "SPBeAttackedList.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"


//------------------------------------------------------------------------------------
SPMonster::SPMonster()
: m_eCurState(GOB_STATE_NULL)
, m_eLastState(GOB_STATE_NULL)
, m_uiCurAction(ACTION_STOP)
, m_uiLastAction(ACTION_STOP)
, m_fCamX(0.0f)
, m_fCamY(0.0f)
, m_pkModelUnit(NULL)
, m_pkCoordPhysics(NULL)
, m_pkStatus(NULL)
, m_pkStatusModel(NULL)
, m_pkFXArchive(NULL)
, m_pSoundArchive(NULL)
, m_pkEffectArchive(NULL)
, m_pkSkillArchive(NULL)
, m_bStatusRender(true)
, m_iType(0)
{	
}

//------------------------------------------------------------------------------------
SPMonster::SPMonster(INSTANCE_ID instanceID, CLASS_ID ClassID)
: SPGameObject(instanceID, ClassID)
, m_eCurState(GOB_STATE_NULL)
, m_eLastState(GOB_STATE_NULL)
, m_uiCurAction(ACTION_STOP)
, m_uiLastAction(ACTION_STOP)
, m_fCamX(0.0f)
, m_fCamY(0.0f)
, m_pkModelUnit(NULL)
, m_pkCoordPhysics(NULL)
, m_pkStatus(NULL)
, m_pkStatusModel(NULL)
, m_pkFXArchive(NULL)
, m_pSoundArchive(NULL)
, m_pkEffectArchive(NULL)
, m_pkSkillArchive(NULL)
, m_bStatusRender(true)
, m_iType(0)
{
	//Init();
}

//------------------------------------------------------------------------------------
SPMonster::~SPMonster()
{
	Destroy();
}

//------------------------------------------------------------------------------------
void SPMonster::Destroy()
{
	SAFE_DELETE(m_pkModelUnit);
	SAFE_DELETE(m_pkCoordPhysics);
	SAFE_DELETE(m_pkStatus);
	SAFE_DELETE(m_pkStatusModel);
	SAFE_DELETE(m_pkFXArchive);
	SAFE_DELETE(m_pkEffectArchive);
	SAFE_DELETE(m_pkSkillArchive);
		
	//InstanceID 초기화 하지 않은 Player가 존재하기 때문에 GU_ID만 넘겨줘서 이미 지워진건 스킵한다.
	unsigned __int64 ui64Guid = (unsigned __int64)GetGUID();
	SPSoundBase::GetInstance()->FreeArchive(ui64Guid);
	m_pSoundArchive = NULL;

}


//------------------------------------------------------------------------------------
void SPMonster::RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer )
{
	if( m_pkFXArchive != NULL )
		m_pkFXArchive->Render( fTime , eRenderLayer );
}

//------------------------------------------------------------------------------------
void SPMonster::Init()
{
	SPGameObject::Init();
	Destroy();

	MONSTER_TYPE eMonsterType;
	eMonsterType = g_pMOBCluster->GetMonsterType(GetClassID());
	if( eMonsterType == MT_LAYER_BOSS )
		m_pkModelUnit = new SPMOBLayerModelUnit(this);
	else
		m_pkModelUnit = new SPMonsterModelUnit(this);
	
	m_pkModelUnit->Init();

	if( m_iType == 11 )
		m_pkCoordPhysics = new SPFlyCoordPhysics(this);
	else
		m_pkCoordPhysics = new SPMonsterCoordPhysics(this);

	m_pkCoordPhysics->Init();

}

//------------------------------------------------------------------------------------
void SPMonster::SetState(GOB_STATE_TYPE state, bool bForce)
{
	if( !bForce && m_eCurState == state )
		return;

	m_eLastState = state;
	m_eCurState = state;

	ANIMATION_INDEX	eAniIndex	=	ANI_NULL;

	m_pkModelUnit->SPSendMessage(MV_SET_ACCUMUL_ACTION, 0);
	switch( m_eCurState )
	{
	case GOB_STATE_STAND:
		eAniIndex	=	ANI_MONSTER_STAND;
		break;

	case GOB_STATE_WALK:
		eAniIndex	=	ANI_MONSTER_WALK;
		break;

	case GOB_STATE_RUN:
		eAniIndex	=	ANI_MONSTER_DASH;
		break;

	case GOB_STATE_SIT_DOWN:
		eAniIndex	=	ANI_MONSTER_SIT_DOWN;
		break;

	case GOB_STATE_SIT_WAIT:
		eAniIndex	=	ANI_MONSTER_SIT_WAIT;
		break;

	case GOB_STATE_SIT_STAND:
		eAniIndex	=	ANI_MONSTER_SIT_UP;
		break;

	case GOB_STATE_DROP:
		eAniIndex	=	ANI_MONSTER_FALL;
		break;

	case GOB_STATE_JUMP:
		eAniIndex	=	ANI_MONSTER_JUMP;
		break;

	case GOB_STATE_ATTACK_SPEEDY:
		eAniIndex	=	m_eAttackRandIndex;
		break;

	case GOB_STATE_ATTACK_MIGHTY:
		//eAniIndex	=	ANI_MONSTER_SKILL01;
		break;
	
	case GOB_STATE_ATTACKED:
		eAniIndex	=	ANI_MONSTER_BEATTACKED;
		break;

	case GOB_STATE_DEAD:
		eAniIndex	=	ANI_MONSTER_DEAD;
		break;

	case GOB_STATE_GUARD_DAMAGE:
		eAniIndex = ANI_MONSTER_GUARD_DAMAGE;
		break;
	}

	if( eAniIndex != ANI_NULL )
	{
		unsigned int uiAniIndex = static_cast<unsigned int>( eAniIndex );
		m_pkModelUnit->SPSendMessage(MV_SET_ANIMATION_INDEX, eAniIndex );
		m_pkFXArchive->InsertFXGroup( uiAniIndex );
		SetAnimationLoop();
		SetActionSound(eAniIndex);

		int iMotionIndex;
		SPMotionStatus* pkStatus;

		// remove
		//iMotionIndex = (GetClassID() - 810000000) * 100 + (m_eCurAnimationIndex + 1);
		//m_pkSkillArchive->RemoveUsedMotion(iMotionIndex);

		// insert
		iMotionIndex = (GetClassID() - 810000000) * 100 + (uiAniIndex + 1);
		pkStatus = SPSkillManager::GetInstance()->GetMOBMotionStatus(iMotionIndex);
		if( pkStatus )
		{
			//SKILL_APPLY_EFFECT* pApplyEffect = pkStatus->GetApplyEffect();
			//if( pApplyEffect->eTargetType == STT_ENEMY )
			//{
			//	SPMotionUsed* pkMotionUsed = new SPMotionUsed(this, pkStatus, uiAniIndex);
			//	if( !m_pkSkillArchive->UsedMotion(pkMotionUsed) )
			//		delete pkMotionUsed;
			//}

			SKILL_ACTIVITY_VELOCITY* pActivityVel = pkStatus->GetActivityVelocity();
			if( pActivityVel->fVelocityX || pActivityVel->fVelocityY )
			{
				ACTIVE_VEL_INFO Info;
				ZeroMemory(&Info, sizeof(ACTIVE_VEL_INFO));

				Info.bEnable = true;
				Info.fVelocityY = pActivityVel->fVelocityY;
				Info.fTime = pActivityVel->fTime;

				if( m_pkCoordPhysics->GetDirection() == FACING_LEFT )
					Info.fVelocityX = -pActivityVel->fVelocityX;
				else
					Info.fVelocityX = pActivityVel->fVelocityX;

				Info.bDelay = true;
				Info.fDelayTime = pActivityVel->fDelayTime;

				m_pkCoordPhysics->SetActicityVelocity(Info);
			}
		}
	}

	m_eCurAnimationIndex = eAniIndex;
}

//------------------------------------------------------------------------------------
void SPMonster::SetAnimationLoop()
{
	bool bLoop = true;
	switch( m_eCurState )
	{
	case GOB_STATE_ATTACK_SPEEDY:
	case GOB_STATE_ATTACK_MIGHTY:
	case GOB_STATE_ATTACKED:
	case GOB_STATE_DEAD:
	case GOB_STATE_LAND:
	case GOB_STATE_JUMP:
	case GOB_STATE_DROP:
	case GOB_STATE_SIT_DOWN:
	case GOB_STATE_SIT_STAND:
	case GOB_STATE_GUARD_DAMAGE:
		bLoop = false;
		break;
	}

	m_pkModelUnit->SPSendMessage(MV_SET_ANI_LOOP, bLoop);
}

//------------------------------------------------------------------------------------
void SPMonster::SetActionSound(ANIMATION_INDEX eIndex, bool bSkill)
{
	SOUND_TABLE_INFO* pInfo;
	if( bSkill )
		pInfo = SPSoundManager::GetInstance()->GetSoundTable((unsigned int)eIndex);
	else
		pInfo = SPSoundManager::GetInstance()->GetMOBSoundTable(GetClassID(), eIndex);
	
	if( pInfo == NULL )
		return;

	int i;
	string strFullName;

	for( i = 0; i < 3; i++ )
	{
		if( pInfo->strFileName[i].empty() )
			continue;

		strFullName = "DATA/SOUND/";
		strFullName += pInfo->strFileName[i];

		m_pSoundArchive->InsertItem(strFullName.c_str(), pInfo->fDelayTime[i]);
	}
}

//------------------------------------------------------------------------------------
void SPMonster::SetAction(UINT64 uiAction, bool bForce)
{
	if(!m_pkCoordPhysics)
		return;

	if( !bForce && m_uiCurAction == uiAction )
		return;

	m_uiLastAction = m_uiCurAction;
	m_uiCurAction = uiAction;

	m_pkCoordPhysics->SetAction(uiAction);
	
	if( m_uiCurAction == ACTION_STOP )
		SetState(GOB_STATE_STAND, bForce);
	else if( m_uiCurAction & ACTION_DEAD )
		SetState(GOB_STATE_DEAD, bForce);
	else
	{
		if( m_uiCurAction & ACTION_MOVE_LEFT ||
			m_uiCurAction & ACTION_MOVE_RIGHT )
			SetState(GOB_STATE_WALK, bForce);

		if( m_uiCurAction == ACTION_RUN_LEFT ||
			m_uiCurAction == ACTION_RUN_RIGHT )
			SetState(GOB_STATE_RUN, bForce);

		if( m_uiCurAction & ACTION_MOVE_JUMP )
			SetState(GOB_STATE_JUMP, bForce);
		else if( m_uiCurAction & ACTION_MOVE_DROP )
			SetState(GOB_STATE_DROP, bForce);

		if( m_uiCurAction & ACTION_SIT_DOWN )
			SetState(GOB_STATE_SIT_DOWN, bForce);
		else if( m_uiCurAction & ACTION_SIT_WAIT )
			SetState(GOB_STATE_SIT_WAIT, bForce);
		else if( m_uiCurAction & ACTION_SIT_STAND )
			SetState(GOB_STATE_SIT_STAND, bForce);
		
		if( m_uiCurAction & ACTION_ATTACK_SPEEDY )
			SetState(GOB_STATE_ATTACK_SPEEDY, bForce);
		if( m_uiCurAction & ACTION_ATTACK_MIGHTY )
			SetState(GOB_STATE_ATTACK_MIGHTY, bForce);
		else if( m_uiCurAction & ACTION_BEATTACKED )
			SetState(GOB_STATE_ATTACKED, bForce);
		else if( m_uiCurAction & ACTION_GUARD_DAMAGE )
			SetState(GOB_STATE_GUARD_DAMAGE, bForce);
	}

	float fTime;
	m_pkModelUnit->SPSendMessage(MV_GET_ANI_ACCUMUL_TIME, (WPARAM)&fTime);
	m_pkCoordPhysics->SetActionTime(fTime);

	if( uiAction & ACTION_DEAD )
	{
#ifndef _USE_OLD_HP_BAR		
		if(g_pGOBManager->GetLocalPlayer()) {
			SPBeAttackedList* pBeAttackedList = g_pGOBManager->GetLocalPlayer()->GetBeAttackedList();
			if(pBeAttackedList)	{
				g_pGOBManager->GetLocalPlayer()->GetBeAttackedList()->SetDead(this->GetGUID());
			}		
		}
#endif
		m_fDeadTime = fTime;
		m_fDeadAccmulateTime = 0.0f;
	}

}

//------------------------------------------------------------------------------------
void SPMonster::Process(float fTime)
{
	SPGameObject::Process(fTime);

	if( m_pkCoordPhysics )
		m_pkCoordPhysics->Process(fTime);
	
	if( m_pkFXArchive )
		m_pkFXArchive->Process(fTime);

	if( m_pkEffectArchive )
		m_pkEffectArchive->Process(fTime);

	if( m_pkSkillArchive )
		m_pkSkillArchive->Process(fTime);

	ProcessImage(fTime);

	if(m_pSoundArchive) {

		POINT pos;
		//float fX = ((pkMobUnit->GetRenderPosX()) - ((float)(g_pVideo->GetScreenLenX()) / 2.0f) + 10.0f) / 5.5f;
		//float fY = ((pkMobUnit->GetRenderPosY()) - ((float)(g_pVideo->GetScreenLenY()) / 2.0f) + 10.0f) / 5.5f; 

		float fX = m_pkModelUnit->GetRenderPosX();
		float fY = m_pkModelUnit->GetRenderPosY();

		// offset to center position
		fX -= (float)(g_pVideo->GetScreenLenX()) / 2.0f;
		fY -= (float)(g_pVideo->GetScreenLenY()) / 2.0f;

		// adjust center position
		fX += 10.0f; 
		fY += 10.0f;

		// adjust distance
		fX /= 5.5f; 
		fY /= 5.5f;

		pos.x = (long)fX;
		pos.y = (long)fY;
		m_pSoundArchive->Process(fTime, pos);
	}
}

//------------------------------------------------------------------------------------
void SPMonster::ProcessImage(float fTime)
{
	if( m_pkModelUnit == NULL || m_pkCoordPhysics == NULL )
		return;

	m_fCamX = g_pCameraManagr->GetCameraStruct()->fOffsetX;
	m_fCamY = g_pCameraManagr->GetCameraStruct()->fOffsetY;

	m_RectPosition.left		= (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX - 5);
	m_RectPosition.right	= (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX + 5);
	m_RectPosition.top		= (LONG)(m_pkCoordPhysics->GetPosY() - m_fCamY - 5);
	m_RectPosition.bottom	= (LONG)(m_pkCoordPhysics->GetPosY() - m_fCamY + 5);

	float	fParsingX, fParsingY;
	WPARAM	wParsingX, wParsingY; 
	fParsingX = m_pkCoordPhysics->GetPosX()-m_fCamX;
	fParsingY = m_pkCoordPhysics->GetPosY()-m_fCamY;
	DataConvert<float,WPARAM>(fParsingX, wParsingX);
	DataConvert<float,WPARAM>(fParsingY, wParsingY);
	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSX, wParsingX );
	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSY, wParsingY );
	m_pkModelUnit->Process(fTime);

}

//------------------------------------------------------------------------------------
void SPMonster::Render(float fTime)
{
	SPGameObject::Render(fTime);
	
	RenderFX( fTime , FXM_RENDER_LAYER_OBJECT_BACK );

	if( !m_bClipping )
	{
		if( m_pkStatusModel )
		{
			if( m_fDeadTime > 0.0f )
			{
				if( m_fDeadAccmulateTime < m_fDeadTime )
					m_pkStatusModel->Render(fTime);
			}
			else
			{
				if( m_bStatusRender )
					m_pkStatusModel->Render(fTime);
			}
		}

		if( m_pkModelUnit )
			m_pkModelUnit->Render(fTime);
	}

	//if( m_pkFXArchive )
	//	m_pkFXArchive->Render(fTime);

	RenderFX( fTime , FXM_RENDER_LAYER_OBJECT_FRONT );

#if defined(_DEBUG)
	if( g_pMainGameManager->GetRenderDebugInfo() )
	{
		// Position Rectangle
		g_pVideo->GetSysLine()->DrawRect(m_RectPosition);

		AttackBoxRender();
	}

#endif
}

//------------------------------------------------------------------------------------
void SPMonster::AttackBoxRender()
{
	SPMonsterAttack* pkAttack = NULL;
	SPMonsterBeAttacked* pkBeAttacked = NULL;

	g_pBattleCluster->GetAttack((ANIMATION_INDEX)m_pkModelUnit->SPSendMessage(MV_GET_ANIMATION_INDEX),
		&pkAttack, GetClassID());
	
	int i;
	RECT rcAttack, rcBeAttack;
	if( pkAttack )
	{
		for( i = 0; i < (int)pkAttack->m_AttackInfo.size(); i++ )
		{
			int iWidth = pkAttack->m_AttackInfo.at(i).m_rtRange.right - pkAttack->m_AttackInfo.at(i).m_rtRange.left;
			if( m_pkCoordPhysics->GetDirection() == FACING_RIGHT )
			{
				rcAttack.left = (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
					+ pkAttack->m_AttackInfo.at(i).m_rtRange.left);
				rcAttack.right	= (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
					+ pkAttack->m_AttackInfo.at(i).m_rtRange.right);
			}
			else
			{
				rcAttack.left = (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
					-pkAttack->m_AttackInfo.at(i).m_rtRange.left - iWidth);
				rcAttack.right = (LONG)(rcAttack.left + iWidth);
			}

			rcAttack.top = (LONG)(m_pkCoordPhysics->GetPosY() - m_fCamY
				+ pkAttack->m_AttackInfo.at(i).m_rtRange.top);
			rcAttack.bottom	= (LONG)(m_pkCoordPhysics->GetPosY() - m_fCamY
				+ pkAttack->m_AttackInfo.at(i).m_rtRange.bottom);

			g_pVideo->GetSysLine()->DrawRect(rcAttack);
		}
	}
	
	g_pBattleCluster->GetAttack((ANIMATION_INDEX)m_pkModelUnit->SPSendMessage(MV_GET_ANIMATION_INDEX),
		&pkBeAttacked, GetClassID());

	if( pkBeAttacked )
	{
		int nFrame = (int)m_pkModelUnit->SPSendMessage(MV_GET_CURR_FRAME) + 1;

		vector< APCBEATTACKEDINFO >::iterator iter = pkBeAttacked->m_BeAttackInfo.begin();
		while( iter!= pkBeAttacked->m_BeAttackInfo.end() )
		{
			if( nFrame <= (*iter).m_nIndex )
			{
				if( m_pkCoordPhysics->GetDirection()==FACING_RIGHT )
				{
					rcBeAttack.left	= (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
						+ (*iter).m_rtRange.left);
					rcBeAttack.right = (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
						+(*iter).m_rtRange.right);
				}
				else
				{
					rcBeAttack.left	= (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
						- (*iter).m_rtRange.right);
					rcBeAttack.right = (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
						- (*iter).m_rtRange.left);
				}

				rcBeAttack.top = (LONG)(m_pkCoordPhysics->GetPosY() - m_fCamY
					+ (*iter).m_rtRange.top);
				rcBeAttack.bottom = (LONG)(m_pkCoordPhysics->GetPosY() - m_fCamY
					+ (*iter).m_rtRange.bottom);

				g_pVideo->GetSysLine()->DrawRect(rcBeAttack);
				break;
			}
			
			++iter;
		}		
	}
}

//------------------------------------------------------------------------------------
void SPMonster::SetShowStatus(bool bName, bool bHP, bool bShadow)
{
	if( m_pkStatusModel )
		m_pkStatusModel->SetShowValue(bName, bHP, bShadow);
}


//------------------------------------------------------------------------------------
bool SPMonster::IsShowName()
{
	if( m_pkStatusModel )
		return m_pkStatusModel->IsShowName();

	return true;
}

bool SPMonster::IsShowHP()
{
	if( m_pkStatusModel )
		return m_pkStatusModel->IsShowHP();

	return true;
}


//------------------------------------------------------------------------------------
FACING	SPMonster::GetDirection()
{ 
	return m_pkCoordPhysics->GetDirection(); 
}

//------------------------------------------------------------------------------------
BG_ID	SPMonster::GetStandLayer()
{ 
	return m_pkCoordPhysics->GetStandLayer(); 
}

//------------------------------------------------------------------------------------
void SPMonster::SetPos(float fX, float fY)	
{ 
	m_pkCoordPhysics->SetPos (fX, fY); 
}

//------------------------------------------------------------------------------------
int SPMonster::GetCurStageID()
{
	return m_pkCoordPhysics->GetCurStageID();
}

//------------------------------------------------------------------------------------
void SPMonster::SetCurStageID(int iStageID)
{
	m_pkCoordPhysics->SetCurStageID(iStageID);
}

//------------------------------------------------------------------------------------
int	SPMonster::GetCurMapGroupID()
{
	return m_pkCoordPhysics->GetCurMapGroupID();
}

//------------------------------------------------------------------------------------
void SPMonster::SetCurMapGroupID(int iMapGroupID)
{
	m_pkCoordPhysics->SetCurMapGroupID(iMapGroupID);
}

//------------------------------------------------------------------------------------
SPGOBModelUnit* SPMonster::GetModelUnit()
{
	return m_pkModelUnit;
}

//------------------------------------------------------------------------------------
SPGOBCoordPhysics* SPMonster::GetCoordPhysics()
{
	return m_pkCoordPhysics;
}

//------------------------------------------------------------------------------------
bool SPMonster::IsDead()
{
	int iHP = m_pkStatus->GetStatusValue(STATUS_HP);
	if( iHP < 1 || m_pkCoordPhysics->GetCurState() == GOB_STATE_DEAD )
		return true;

	return false;
}

//------------------------------------------------------------------------------------
void SPMonster::SetMonsterType(int iType)
{
	m_iType = iType;
}

//------------------------------------------------------------------------------------
void SPMonster::SetMOBName(const char* pszName)
{
	if( pszName == NULL || m_pkStatusModel == NULL )
		return;

	m_pkStatusModel->SetObjectName(pszName);
}

//------------------------------------------------------------------------------------
int  SPMonster::GetIconIndex()
{
	if(m_pkStatusModel) {
		return m_pkStatusModel->GetIconIndex();
	}

	return 0;
}

//------------------------------------------------------------------------------------
SPGOBStatus* SPMonster::GetGobStatus()
{
	if(m_pkStatus)
		return m_pkStatus;

	return NULL;
}


//------------------------------------------------------------------------------------
int SPMonster::OnGetPosX(LPARAM lParam)
{
	//
	// AJJIYA [5/16/2005]
	// DataConvert가 값이 이상하게 된다...
/*
	float fPosX;
	fPosX = m_pkCoordPhysics->GetPosX();
	int iRetVal;
	DataConvert<float,int>(fPosX, iRetVal);
	return iRetVal;
*/
	return static_cast<int>( m_pkCoordPhysics->GetPosX() );
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetPosY(LPARAM lParam)
{
	//
	// AJJIYA [5/16/2005]
	// DataConvert가 값이 이상하게 된다...
/*
	float fPosY;
	fPosY = m_pkCoordPhysics->GetPosY();

	int iRetVal;
	DataConvert<float,int>(fPosY, iRetVal);
	return iRetVal;
*/
	return static_cast<int>( m_pkCoordPhysics->GetPosY() );
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetFacing(LPARAM lParam)
{
	return m_pkCoordPhysics->GetDirection();
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetStandlayer(LPARAM lParam)
{	
	return m_pkCoordPhysics->GetStandLayer();
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetModelUnit(LPARAM lParam)
{
	SPGOBModelUnit** ppModelUnit = (SPGOBModelUnit**)lParam;

	*ppModelUnit = m_pkModelUnit;
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetCoordPhysics(LPARAM lParam)
{
	SPGOBCoordPhysics** ppCoordPhysics = (SPGOBCoordPhysics**)lParam;

	*ppCoordPhysics = m_pkCoordPhysics;
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetEffectArchive(LPARAM lParam)
{
	SPEffectArchive** ppEffectArchive = (SPEffectArchive**)lParam;

	*ppEffectArchive = m_pkEffectArchive;
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetFXArchive(LPARAM lParam)
{
	SPFXArchive** ppFXArchive = (SPFXArchive**)lParam;

	*ppFXArchive = m_pkFXArchive;
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetSoundArchive(LPARAM lParam)
{
	SPSoundArchive** ppSoundArchive = (SPSoundArchive**)lParam;

	*ppSoundArchive = m_pSoundArchive;
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetGOBStatus(LPARAM lParam)
{
	SPGOBStatus** ppGOBStatus = (SPGOBStatus**)lParam;

	*ppGOBStatus = m_pkStatus;
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetCurAniIndex( LPARAM lParam )
{
	if( m_pkModelUnit == NULL )
		return -1;

	return (int)m_pkModelUnit->SPSendMessage(MV_GET_ANIMATION_INDEX);
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetCurAniFrame( LPARAM lParam )
{
	if( m_pkModelUnit == NULL )
		return -1;

	return (int)m_pkModelUnit->SPSendMessage(MV_GET_CURR_FRAME);
}

//------------------------------------------------------------------------------------
int SPMonster::OnEffectFXSound(LPARAM lParam)
{
	FX_SOUND_INFO* pFXSoundInfo = (FX_SOUND_INFO*)lParam;
	SPEffect* pkEffect;
	int i, iEffectID;
	float fPosX, fPosY;
	FACING eFacing;

	fPosX = m_pkCoordPhysics->GetPosX();
	fPosY = m_pkCoordPhysics->GetPosY();
	eFacing = m_pkCoordPhysics->GetDirection();

	SPFXArchive* pkFXArchive;
	EFFECT_MISSILE* pMissileInfo;
	EFFECT_FX_SOUND* pEffectFXSound;
	for( i = 0; i < 4; i++ )
	{
		if( iEffectID = pFXSoundInfo->pApplyEffect->iTargetEffect[i] )
		{
			pkEffect = SPEffectManager::GetInstance()->GetEffect(iEffectID);
			if( pkEffect == NULL )
				continue;

			pMissileInfo = pkEffect->GetMissileInfo();
			pEffectFXSound = pkEffect->GetFXSoundInfo();

			vector< SPGameObject* >::iterator iter = pFXSoundInfo->vTargetList.begin();
			while( iter != pFXSoundInfo->vTargetList.end() )
			{
				if( pMissileInfo->eType == EMT_MISSILE )
					m_pkFXArchive->AddNextTargetGOBID((*iter)->GetGUID());
				else
				{
					(*iter)->SPGOBPerformMessage(SPGM_GETFXARCHIVE, (LPARAM)&pkFXArchive);
					pkFXArchive->SetTargetGOBID( (*iter)->GetGUID() );
					pkFXArchive->InsertFXGroup((SPID_FX_GROUP)pEffectFXSound->iTargetFX);

					if( pEffectFXSound->iTargetSound )
						(*iter)->SPGOBPerformMessage(SPGM_SET_PLAY_SOUND, (LPARAM)pEffectFXSound->iTargetSound);
				}

				++iter;
			}

			if( pMissileInfo->eType == EMT_MISSILE )
			{
				if( pFXSoundInfo->vTargetList.size() )
					m_pkFXArchive->AddNextFXGroupID((SPID_FX_GROUP)pEffectFXSound->iTargetFX);


				//				m_pFXArchive->SetLPARAM((LPARAM)eFacing);

				if( pFXSoundInfo->pkMissileTarget )
					m_pkFXArchive->SetTargetGOBID(pFXSoundInfo->pkMissileTarget->GetGUID());
				else
				{
					if( eFacing == FACING_LEFT )
						m_pkFXArchive->SetDestPosX(fPosX - 500.0f);
					else
						m_pkFXArchive->SetDestPosX(fPosX + 500.0f);

					m_pkFXArchive->SetDestPosY(fPosY);
				}

				m_pkFXArchive->InsertFXGroup((SPID_FX_GROUP)pMissileInfo->iTargetFX);
			}
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPMonster::OnCasterEffectFXSound(LPARAM lParam)
{
	EFFECT_FX_SOUND* pFXSound = (EFFECT_FX_SOUND*)lParam;
	if( pFXSound == NULL )
		return 0;

	if( pFXSound->iCasterFX )
	{
		if( m_pkFXArchive )
		{
			m_pkFXArchive->InsertFXGroup((SPID_FX_GROUP)pFXSound->iCasterFX);
		}
	}

	if( pFXSound->iCasterSound )
		SetActionSound((ANIMATION_INDEX)pFXSound->iCasterSound, true);

	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnTargetEffectFXSound(LPARAM lParam)
{
	EFFECT_FX_SOUND* pFXSound = (EFFECT_FX_SOUND*)lParam;
	if( pFXSound == NULL )
		return 0;

	if( pFXSound->iTargetFX )
	{
		if( m_pkFXArchive )
		{
			m_pkFXArchive->InsertFXGroup((SPID_FX_GROUP)pFXSound->iTargetFX);
		}
	}

	if( pFXSound->iTargetSound )
		SetActionSound((ANIMATION_INDEX)pFXSound->iTargetSound, true);

	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnIsDead(LPARAM lParam)
{
	if( IsDead() )
		return 1;
	
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnPlaySound(LPARAM lParam)
{
	SetActionSound((ANIMATION_INDEX)lParam, true);
	return 0;
}


//------------------------------------------------------------------------------------
int SPMonster::OnIsPlayer(LPARAM lParam)
{
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetType(LPARAM lParam)
{
	return m_iType;
}

//------------------------------------------------------------------------------------
int SPMonster::OnGetAction(LPARAM lParam)
{
	*((UINT64*)lParam) = m_uiCurAction;
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnSetActionCommand(LPARAM lParam)
{
	UINT64 uiTempAction = m_uiCurAction;
	UINT64 uiAction = *((UINT64*)lParam);
	uiTempAction |= uiAction;
	SetAction(uiTempAction);
	return 1;
}

//------------------------------------------------------------------------------------
int SPMonster::OnSetActionCompleteCommand(LPARAM lParam)
{
	UINT64 uiTempAction = m_uiCurAction;
	UINT64 uiAction = *((UINT64*)lParam);
	uiTempAction &= ~uiAction;
	SetAction(uiTempAction);
	return 1;
}

//------------------------------------------------------------------------------------
int SPMonster::OnSetMonsterInfo(LPARAM lParam)
{
	MONSTER_INFO* pInfo = (MONSTER_INFO*)lParam;
	m_pkStatus->SPPerformMessage(SPGM_SET_SC_MONSTERINFO,  lParam);

	m_pkStatusModel->SetHPValue(m_pkStatus->GetMaxHP(), m_pkStatus->GetCurHP());
	//m_pkStatusModel->SetObjectName(m_pkStatus->GetGOBName());

	if( m_pkCoordPhysics )
	{
		m_pkCoordPhysics->SetGravity(pInfo->MovingStatus.fDropSpeedNow);
		m_pkCoordPhysics->SetMaxDropVelocity(pInfo->MovingStatus.fDropSpeedMax);
		m_pkCoordPhysics->SetMaxVelocityX(pInfo->MovingStatus.fMaxVelocityX);
		m_pkCoordPhysics->SetJumpSpeed(pInfo->MovingStatus.fJumpSpeed);
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPMonster::OnSetLPPosotion(LPARAM lParam)
{
	LT_POSITION* pPosInfo = (LT_POSITION*)lParam;

	m_pkCoordPhysics->SetPos(pPosInfo->fPosX, pPosInfo->fPosY);
	m_pkCoordPhysics->SetDirection((FACING)pPosInfo->eFacing);
	m_pkCoordPhysics->SetStandLayer((BG_ID)pPosInfo->bgStandLayer);

	m_pkCoordPhysics->SetAccel(pPosInfo->fAccelX, pPosInfo->fAccelY);
	m_pkCoordPhysics->SetCurVelocity(pPosInfo->fVelocityX, pPosInfo->fVelocityY);

	KNOCKBACK_INFO Info;
	Info.bKnockBack = pPosInfo->bKnockBack;
	Info.bDelay = pPosInfo->bDelay;
	return 1;
}


//------------------------------------------------------------------------------------
int SPMonster::OnSetMoveCommand(LPARAM lParam)
{	
	CPacket* pPacket = (CPacket*)lParam;

	// Broadcast command 
	unsigned __int64 uiCommandBuf;
	unsigned int uiRandSeqIndex;
	pPacket->Extract(&uiCommandBuf);
	pPacket->Extract(&uiRandSeqIndex);
	
	m_eAttackRandIndex = (ANIMATION_INDEX)uiRandSeqIndex;
	SetAction(uiCommandBuf, true);

	// Broadcast position
	LT_POSITION		PosInfo;		
	ZeroMemory(&PosInfo,	sizeof(LT_POSITION)		);		
	pPacket->ExtractStruct((LPVOID)&PosInfo,		sizeof(LT_POSITION));		
	this->SPGOBPerformMessage( SPGM_SET_SC_LPPOSITION	,	(WPARAM)&PosInfo	);

	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnSetAction(LPARAM lParam)
{
	UINT64 uiAction = *((UINT64*)lParam);
	SetAction(uiAction, true);
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnSetStatusRender(LPARAM lParam)
{
	m_bStatusRender = lParam ? true:false;
	return 1;
}

//------------------------------------------------------------------------------------
int SPMonster::OnSetKnockBack(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;

	LT_KNOCKBACK KnockBackInfo;		
	pPacket->ExtractStruct((LPVOID)&KnockBackInfo, sizeof(LT_KNOCKBACK));

	KNOCKBACK_INFO Info;
	Info.bKnockBack = KnockBackInfo.bKnockBack;
	Info.fKnockBackTime = KnockBackInfo.fKnockTime;
	Info.fKnockBackVelocityX = KnockBackInfo.fKnockVelocityX;
	Info.fKnockBackVelocityY = KnockBackInfo.fKnockVelocityY;

	m_pkCoordPhysics->SetKnockBack(Info);
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnSetNameColor(LPARAM lParam)
{
	m_pkStatusModel->SetNameColor(lParam);
	return 0;
}
//------------------------------------------------------------------------------------
int SPMonster::OnSetGOBSeqIndex(LPARAM lParam)
{
	m_eAttackRandIndex = (ANIMATION_INDEX)lParam;
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnSetIgnoreStage(LPARAM lParam)
{
	bool bIgnore = lParam ? true : false;
	if( m_pkCoordPhysics )
		m_pkCoordPhysics->SetIgnoreStage(bIgnore);

	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnSetDamageFX( LPARAM lParam )
{
	// attackType, FACING, point
	CPacket* pPacket = (CPacket* )lParam;
	int	iDamage, iMode, iType;

	pPacket->ExtractUINT32((UINT*)&iDamage);
	pPacket->ExtractUINT32((UINT*)&iMode);
	pPacket->ExtractUINT32((UINT*)&iType);

	InsertDamageFX( FDT_ATTACK , iDamage , iMode , iType );

	GU_ID	iAttackGuid	=	0;
	pPacket->ExtractUINT64( &iAttackGuid );

#ifndef _USE_OLD_HP_BAR
	if(g_pGOBManager->GetLocalPlayer()) {
		SPBeAttackedList* pBeAttackedList = g_pGOBManager->GetLocalPlayer()->GetBeAttackedList();
		if(pBeAttackedList)	{
			if(g_pGOBManager->GetLocalPlayer()->GetGUID() == iAttackGuid) {
				g_pGOBManager->GetLocalPlayer()->GetBeAttackedList()->AddObject(this, TARGET_OBJECT_TYPE_MONSTER);				
				
				//if(iMode == 1)		//[2007/11/5]- 크리티컬 타격시 표현 테스트용
				//	g_pGOBManager->GetLocalPlayer()->SetCriticalEffect();
			}
		}		
	}
#endif

	UINT8	uiHitComboCount	=	0;
	pPacket->ExtractUINT8( &uiHitComboCount );

	if( g_pGOBManager != NULL )
	{
		SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer != NULL )
		{
			if( pLocalPlayer->GetGUID() == iAttackGuid )
			{
				SPFXArchive* pLocalFXArchive	=	NULL;

				pLocalPlayer->SPGOBPerformMessage( SPGM_GETFXARCHIVE , (LPARAM)&pLocalFXArchive );

				if( pLocalFXArchive != NULL )
				{
					if( iDamage > 0 && ( iMode == 0 || iMode == 1 ) &&
						( iType == 10001 || iType == 10002 || iType == 20001 || iType == 20002 || iType == 20003 || iType == 20004 || iType == 20005 ) &&
						uiHitComboCount > 1 )
					{
						InsertHitComboFX( uiHitComboCount , pLocalFXArchive );
					}
				}
			}
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//	Healing FX

int SPMonster::OnSetHealingFX( LPARAM lParam )
{
	CPacket* pPacket = (CPacket* )lParam;
	int	iDamage, iMode, iType;

	pPacket->ExtractUINT32((UINT*)&iDamage);
	pPacket->ExtractUINT32((UINT*)&iMode);
	pPacket->ExtractUINT32((UINT*)&iType);

	InsertDamageFX( FDT_HEALING , iDamage , iMode , iType );

	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnFXGroup(LPARAM lParam)
{
	m_pkFXArchive->InsertFXGroup((SPID_FX_GROUP)lParam);
	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnSetEffectActivity(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	
	unsigned short iNumofEffect ;
	pPacket->ExtractUINT16(&iNumofEffect);

	SPEffectManager* pkEffectManager = SPEffectManager::GetInstance();
	SPEffectActivity* pkEffectActivity;
	SPEffect* pkEffect;

	bool bCaster = false;
	for( int i = 0; i < iNumofEffect ; i++ )
	{
		EFFECT_DATA  EffectData;
		pPacket->ExtractStruct(&EffectData, sizeof(EFFECT_DATA));

		pkEffect = SPEffectManager::GetInstance()->GetEffect(EffectData.iEffectID);
		if( pkEffect == NULL )
			continue;

		bCaster = false;
		if( GetGUID() == EffectData.iCasterID )
			bCaster = true;

		pkEffectActivity = new SPEffectActivity(pkEffect, EffectData.iEffectLv, (float)EffectData.iRemainTime, bCaster);
		m_pkEffectArchive->AddEffect(EffectData.iInstanceID, pkEffectActivity);

		//////////////////////////////////////////////////////////////////////////
		//	AJJIYA [9/18/2006 AJJIYA]
		//	이펙트 부착 FX
		m_pkFXArchive->InsertFXGroup( (SPID_FX_GROUP)pkEffect->GetAttachFXGroup() );
		//////////////////////////////////////////////////////////////////////////

//#if defined(_DEBUG)
//		DXUTOutputDebugString("MOBSetEffectID:%d\n", EffectData.iEffectID);
//#endif
	}

	return 0;
}

//------------------------------------------------------------------------------------
int SPMonster::OnSetEffectDelete(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;

	unsigned short usNum, i;
	unsigned int uiInstanceID;
	pPacket->ExtractUINT16(&usNum);	

	SPEffectActivity* pkEffectActivity;
	SPEffect* pkEffect;

	for( i = 0; i < usNum; i++ )
	{
		pPacket->ExtractUINT32(&uiInstanceID);

		//////////////////////////////////////////////////////////////////////////
		//	AJJIYA [9/18/2006 AJJIYA]
		//	이펙트 부착 FX 삭제
		pkEffectActivity	=	m_pkEffectArchive->GetUniqueKeyActivity( uiInstanceID );
		if( pkEffectActivity != NULL )
		{
			pkEffect	=	pkEffectActivity->GetEffect();
			m_pkFXArchive->DeleteFXGroup( pkEffect->GetAttachFXGroup() );
		}
		//////////////////////////////////////////////////////////////////////////

		m_pkEffectArchive->RemoveEffect(uiInstanceID);
	}

	return 0;
}

//------------------------------------------------------------------------------------
int	SPMonster::OnStatMessage(SPGM msg, LPARAM lParam)
{
	m_pkStatus->SPPerformMessage(msg, lParam);
	return 0;
}

//------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
// SPGOBManager, SPGameObject끼리만 불려야 한다.
SPGMESSAGE_OBJ_MAP_BEGIN( SPMonster )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETFACING				,	OnGetFacing				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETSTANDLAYER			,	OnGetStandlayer			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETPOSX				,	OnGetPosX				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETPOSY				,	OnGetPosY				)

	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETMODELUNIT			,	OnGetModelUnit			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETCOORDPHYSICS		,	OnGetCoordPhysics		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETEFFECTARCHIVE		,	OnGetEffectArchive		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETFXARCHIVE			,	OnGetFXArchive			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETSOUNDARCHIVE		,	OnGetSoundArchive		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETGOBSTATUS			,	OnGetGOBStatus			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETCURANIINDEX		,	OnGetCurAniIndex		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETCURANIFRAME		,	OnGetCurAniFrame		)
	SPGMESSAGE_OBJ_COMMAND(	 SPGM_GETACTION				,	OnGetAction				)

	SPGMESSAGE_OBJ_COMMAND(	 SPGM_ISDEAD				,	OnIsDead				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_ISPLAYER				,	OnIsPlayer				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETTYPE				,	OnGetType				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_PLAY_SOUND		,	OnPlaySound				)

	SPGMESSAGE_OBJ_COMMAND(	 SPGM_SET_EFFECT_FXSOUND		,	OnEffectFXSound		)
	SPGMESSAGE_OBJ_COMMAND(	 SPGM_SET_CASTER_EFFECT_FXSOUND	,	OnCasterEffectFXSound)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_TARGET_EFFECT_FXSOUND	,	OnTargetEffectFXSound)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_FX_GROUP				,	OnFXGroup			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_KNOCKBACK				,	OnSetKnockBack		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_NAME_COLOR			,	OnSetNameColor		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SETGOBSEQINDEX			,	OnSetGOBSeqIndex	)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SETIGNORESTAGE			,	OnSetIgnoreStage	)
	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_COMMAND_ACTION		,	OnSetActionCommand		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_COMMAND_ACTION_COMPLETE,	OnSetActionCompleteCommand )

	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_MONSTERINFO	,	OnSetMonsterInfo		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_LPPOSITION		,	OnSetLPPosotion			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GOB_MOVE				,	OnSetMoveCommand		)	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_DAMAGE_FX		,	OnSetDamageFX			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_HEALING_FX		,	OnSetHealingFX			)	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SETACTION				,	OnSetAction				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SETSTATUSRENDER		,	OnSetStatusRender		)	


	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_EFFECT_ACTIVITY,		OnSetEffectActivity)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_EFFECT_DELETE,			OnSetEffectDelete)

	SPGMESSAGE_OBJ_COMMAND_RANGE ( SPGM_STAT_MSG_BEGIN, SPGM_STAT_MSG_END, OnStatMessage )

SPGMESSAGE_OBJ_MAP_END( SPGameObject )

//------------------------------------------------------------------------------------
void SPMonster::Test()
{
	SPInputStruct* pInputStructInstant = g_pInputManager->GetInputStruct();
	
	unsigned int uiAction = 0;
	bool bAction = false;
	if(pInputStructInstant->ikJ.InputState == INPUT_PRESS_REPEAT)
	{
		uiAction = ACTION_MOVE_LEFT;
		bAction = true;
	}
	else if(pInputStructInstant->ikL.InputState == INPUT_PRESS_REPEAT)
	{
		uiAction = ACTION_MOVE_RIGHT;
		bAction = true;
	}
	if(pInputStructInstant->ikI.InputState == INPUT_PRESS_REPEAT)
	{
		uiAction = ACTION_MOVE_JUMP;
		bAction = true;
	}
	if(pInputStructInstant->ikH.InputState == INPUT_PRESS_REPEAT)
	{
		uiAction = ACTION_STOP;
		bAction = true;
	}
	if(pInputStructInstant->ikO.InputState == INPUT_PRESS_REPEAT)
	{
		uiAction = ACTION_MOVE_RIGHT | ACTION_MOVE_JUMP;
		bAction = true;
	}
	if(pInputStructInstant->ikU.InputState == INPUT_PRESS_REPEAT)
	{
		uiAction = ACTION_MOVE_LEFT | ACTION_MOVE_JUMP;
		bAction = true;
	}

	if(pInputStructInstant->ikR.InputState == INPUT_PRESS_REPEAT)
	{
		uiAction = ACTION_ATTACK_SPEEDY;
		bAction = true;
	}
	if(pInputStructInstant->ikT.InputState == INPUT_PRESS_REPEAT)
	{
		uiAction = ACTION_BEATTACKED;
		bAction = true;
	}

	if(pInputStructInstant->ikF.InputState == INPUT_PRESS_REPEAT)
	{
// AJJIYA [5/25/2005]
// SPID_FX_GROUP 열거형은 거의 사용중지!!
// 사용시 알려주시기 바랍니다.
//		m_pkFXArchive->InsertFXGroup(IDFXGR_ASSULTED_SPEEDY, OnGetFacing(0));
	}

	if(pInputStructInstant->ikD.InputState == INPUT_PRESS_REPEAT)
	{
		uiAction = ACTION_DEAD;
		bAction = true;
	}

	if( bAction )
		SetAction(uiAction);
}

//////////////////////////////////////////////////////////////////////////
//
//	InsertDamageFX
//

void SPMonster::InsertDamageFX( FXUNIT_DAMAGE_TYPE eDamageType , int iDamage , int iMode , int iType )
{
	//
	// AJJIYA [5/23/2005]
	// 데미지 숫자 뜨는 FX
	POINT	ptGOBBox;
	SPMonsterBeAttacked* pkBeAttacked = NULL;

	g_pBattleCluster->GetAttack((ANIMATION_INDEX)m_pkModelUnit->SPSendMessage(MV_GET_ANIMATION_INDEX),
		&pkBeAttacked, GetClassID());

	ptGOBBox.x	=	0;
	ptGOBBox.y	=	0;		//	기본 캐릭터 높이

	//	캐릭터간 높이 보정용!!
	if( pkBeAttacked )
	{
		int nFrame		= (int)m_pkModelUnit->SPSendMessage(MV_GET_CURR_FRAME);
		int nMaxFrame	= (int)pkBeAttacked->m_BeAttackInfo.size();

		if( nFrame >= nMaxFrame )
			nFrame	=	nMaxFrame - 1;

		BEATTACKEDINFO	beAttackedInfo	=	pkBeAttacked->m_BeAttackInfo[ nFrame ];

		if( nFrame <= beAttackedInfo.m_nIndex )
			ptGOBBox.y	=	abs( beAttackedInfo.m_rtRange.bottom	-	beAttackedInfo.m_rtRange.top );
	}

	if( iMode == 1 )	// 크리티컬 공격 [6/27/2006]
	{
		if( iDamage > 0 )
		{
			m_pkFXArchive->SetGOBBox( ptGOBBox );
			m_pkFXArchive->SetCritical( true );
			m_pkFXArchive->InsertFXGroup( IDFXGR_CRITICAL );
		}
	}
	else if( iMode == 2 )	// 미스 공격 [6/27/2006]
	{
		m_pkFXArchive->SetFXGroup( IDFXGR_DAMAGE_MISS );
		m_pkFXArchive->InsertFXGroup( IDFXGR_DAMAGE_MISS );
	}
	else if( iMode == 3 )	// Immune 공격 [6/27/2006]
	{
		m_pkFXArchive->SetFXGroup( IDFXGR_DAMAGE_IMMUNE );
		m_pkFXArchive->InsertFXGroup( IDFXGR_DAMAGE_IMMUNE );
	}

	if( iType == FDET_POSION || iType == FDET_BLEEDING || iType == FDET_CURSE )
	{
		eDamageType	=	FDT_POSION;
	}
}

void SPMonster::InsertHitComboFX( UINT8 uiHitComboCount , SPFXArchive* pLocalFXArchive )
{
	if( pLocalFXArchive == NULL )
		return;

	//////////////////////////////////////////////////////////////////////////
	//	숫자 관련
	//////////////////////////////////////////////////////////////////////////

	UINT	uiFXGHitNumber		=	5007;

	if( pLocalFXArchive->IsFXGroup( uiFXGHitNumber ) == true )
		pLocalFXArchive->DeleteFXGroup( uiFXGHitNumber );

	pLocalFXArchive->SetDamage( (int)uiHitComboCount );
	pLocalFXArchive->InsertFXGroup( (SPID_FX_GROUP)uiFXGHitNumber );

	//////////////////////////////////////////////////////////////////////////
	//	콤보 시작 관련
	//////////////////////////////////////////////////////////////////////////

	UINT	uiFXGHitComboStart	=	5005;
	UINT	uiFXGHitComboIng	=	5006;
	UINT	uiFXGHitComboEnd	=	5014;

	bool	bIsFXGHitComboStart	=	pLocalFXArchive->IsFXGroup( uiFXGHitComboStart , uiFXGHitComboStart );
	bool	bIsFXGHitComboIng	=	pLocalFXArchive->IsFXGroup( uiFXGHitComboStart , uiFXGHitComboIng );

	if( bIsFXGHitComboStart == false && bIsFXGHitComboIng == false )
	{
		pLocalFXArchive->AddNextFXGroupID( uiFXGHitComboIng );
		pLocalFXArchive->AddNextFXGroupID( uiFXGHitComboEnd );

		pLocalFXArchive->InsertFXGroup( (SPID_FX_GROUP)uiFXGHitComboStart );
	}
	else if( bIsFXGHitComboStart == false && bIsFXGHitComboIng == true )
	{
		pLocalFXArchive->DeleteFXGroup( uiFXGHitComboIng );
		pLocalFXArchive->AddNextFXGroupID( uiFXGHitComboEnd );

		pLocalFXArchive->InsertFXGroup( (SPID_FX_GROUP)uiFXGHitComboIng );
	}

	//////////////////////////////////////////////////////////////////////////
	//	부가 글자 관련
	//////////////////////////////////////////////////////////////////////////

	UINT	uiFXGHitComboAdd	=	0;
	UINT	uiSoundEffect		=	0;

	switch( uiHitComboCount )
	{
		case 10:	uiFXGHitComboAdd	=	5008;	uiSoundEffect	=	783;	break;
		case 30:	uiFXGHitComboAdd	=	5009;	uiSoundEffect	=	784;	break;
		case 50:	uiFXGHitComboAdd	=	5010;	uiSoundEffect	=	785;	break;
		case 70:	uiFXGHitComboAdd	=	5011;	uiSoundEffect	=	786;	break;
		case 90:	uiFXGHitComboAdd	=	5012;	uiSoundEffect	=	787;	break;
		case 99:	uiFXGHitComboAdd	=	5013;	uiSoundEffect	=	0;		break;
	}

	if( uiFXGHitComboAdd > 0 )
	{
		pLocalFXArchive->InsertFXGroup( (SPID_FX_GROUP)uiFXGHitComboAdd );

		if( uiSoundEffect > 0 )
			SetActionSound( (ANIMATION_INDEX)uiSoundEffect , true );
	}
}
