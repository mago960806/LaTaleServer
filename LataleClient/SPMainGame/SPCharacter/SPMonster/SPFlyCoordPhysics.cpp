
#include <vector>
#include <string>

// 서버
#ifdef _LATALE_SERVER
#include <math.h>
#include <WTypes.h>	
//TODO : Server Side
#include "SPGameObjectDef.h"
#include <assert.h>
#endif

// 클라이언트
#ifdef _LATALE_CLIENT	
#include "SPCommon.h"	
//TODO : Client Side
#include "SPDebug.h"

#endif 

#include "SPGameObject.h"
#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"
#include "SPCommandConvert.h"
#include "SPGOBCoordPhysics.h"
#include "SPFlyCoordPhysics.h"

//------------------------------------------------------------------------------------
#ifdef _LATALE_CLIENT
SPFlyCoordPhysics::SPFlyCoordPhysics(SPGameObject* pkParent)
#else
SPFlyCoordPhysics::SPFlyCoordPhysics(CCharacter* pkParent)
#endif
: SPGOBCoordPhysics(pkParent)
{
	Init();
}

//------------------------------------------------------------------------------------
SPFlyCoordPhysics::~SPFlyCoordPhysics()
{	
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::Init()
{
	SPGOBCoordPhysics::Init();
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::Process(float fElapsedTime)
{	
	if( ReCalKineticEnergy() )	
		CalKineticEnergy(fElapsedTime);

	StateProcess();

	m_fBeforePosX = m_fPosX;
	m_fBeforePosY = m_fPosY;

	ProcessTimeTick(fElapsedTime);
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::ProcessTimeTick(float fElapsedTime)
{
	m_fActionTime -= fElapsedTime;

	//if( m_KnockBackInfo.bKnockBack )
	//{
	//	m_KnockBackInfo.fKnockBackAccmulateTime += fElapsedTime;
	//	if( m_KnockBackInfo.fKnockBackAccmulateTime > m_KnockBackInfo.fKnockBackTime &&
	//		m_KnockBackInfo.fKnockBackVelocityY == 0.0f &&
	//		m_fCurVelocityY == 0.0f )
	//	{
	//		m_KnockBackInfo.bKnockBack = false;
	//		m_KnockBackInfo.fKnockBackAccmulateTime = 0.0f;
	//		m_KnockBackInfo.fKnockBackVelocityX = 0.0f;
	//		m_KnockBackInfo.fKnockBackVelocityY = 0.0f;
	//	}
	//}

	if( m_ActivityVelocityInfo.bEnable )
	{
		m_ActivityVelocityInfo.fAccmulateTime += fElapsedTime;
		if( m_ActivityVelocityInfo.bDelay )
		{
			if( m_ActivityVelocityInfo.fAccmulateTime > m_ActivityVelocityInfo.fDelayTime )
			{
				m_ActivityVelocityInfo.fDelayTime = 0.0f;
				m_ActivityVelocityInfo.fAccmulateTime = 0.0f;
				m_ActivityVelocityInfo.fVelocityY = 0.0f;
				m_ActivityVelocityInfo.bDelay = false;
			}
		}
		else
		{
			if( m_ActivityVelocityInfo.fAccmulateTime > m_ActivityVelocityInfo.fTime &&
				m_ActivityVelocityInfo.fVelocityY == 0.0f )
			{
				ZeroMemory(&m_ActivityVelocityInfo, sizeof(ACTIVE_VEL_INFO));
			}
		}
	}
	
	if( m_KnockBackInfo.bKnockBack )
	{
		m_KnockBackInfo.fKnockBackAccmulateTime += fElapsedTime;
		if( m_KnockBackInfo.bDelay )
		{
			if( m_KnockBackInfo.fKnockBackAccmulateTime > m_KnockBackInfo.fDelayTime )
			{
				m_KnockBackInfo.fDelayTime = 0.0f;
				m_KnockBackInfo.fKnockBackAccmulateTime = 0.0f;
				m_KnockBackInfo.fKnockBackVelocityY = 0.0f;
				m_KnockBackInfo.bDelay = false;
			}
		}
		else
		{
			if( m_KnockBackInfo.fKnockBackAccmulateTime > m_KnockBackInfo.fKnockBackTime &&
				m_KnockBackInfo.fKnockBackVelocityY == 0.0f )
			{
				ZeroMemory(&m_KnockBackInfo, sizeof(KNOCKBACK_INFO));
			}
		}
	}
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::SetKnockBack(KNOCKBACK_INFO& Info)
{
	m_KnockBackInfo.bKnockBack = Info.bKnockBack;
	m_KnockBackInfo.bDelay = Info.bDelay;
	m_KnockBackInfo.fKnockBackTime = Info.fKnockBackTime;
	m_KnockBackInfo.fDelayTime = Info.fDelayTime;
	
	m_KnockBackInfo.fKnockBackVelocityX = Info.fKnockBackVelocityX;
	m_KnockBackInfo.fKnockBackVelocityY = 0.0f;

	m_KnockBackInfo.fKnockBackAccmulateTime = 0.0f;
	if( m_KnockBackInfo.bKnockBack )
		ZeroMemory(&m_ActivityVelocityInfo, sizeof(ACTIVE_VEL_INFO));
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::CalVelocity(UINT64 uiAction)
{
	if( uiAction == ACTION_STOP ||
		uiAction == ACTION_VERTICAL_STOP ||
		uiAction == ACTION_HANGING_STOP ||
		uiAction == ACTION_DEAD ||
		uiAction == ACTION_BEATTACKED ||
		uiAction == ACTION_CRITICAL_BEATTACKED ||
		uiAction == ACTION_GUARD_DAMAGE ||
		uiAction == ACTION_GUARD ||
		uiAction & ACTION_SKILL )
	{
		if( uiAction & ACTION_SKILL )
		{
			if( m_fCurVelocityY == 0.0f && m_fAccelY == 0.0f )
			{
				m_fCurVelocityX = 0.0f;
				m_fAccelX = 0.0f;
			}
		}
		else
		{
			m_fCurVelocityX = 0.0f;
			m_fAccelX = 0.0f;
		}

		if( (uiAction & ACTION_BEATTACKED) ||
			(uiAction & ACTION_CRITICAL_BEATTACKED) ||
			(uiAction & ACTION_GUARD_DAMAGE) ||
			(uiAction & ACTION_GUARD) ||
			(uiAction & ACTION_DEAD) ||
			(uiAction & ACTION_SKILL) )
		{
			ZeroMemory(&m_ActivityVelocityInfo, sizeof(ACTIVE_VEL_INFO));
		}
		else
		{
			m_fCurVelocityY = 0.0f;
			m_fAccelY = 0.0f;
		}

		if( m_eCurState == GOB_STATE_HANGING ||
			m_eCurState == GOB_STATE_UP ||
			m_eCurState == GOB_STATE_DOWN )
		{
			m_fCurVelocityY = 0.0f;
			m_fAccelY = 0.0f;
		}
		
		if( uiAction == ACTION_STOP )
		{
			ZeroMemory(&m_KnockBackInfo, sizeof(KNOCKBACK_INFO));
			ZeroMemory(&m_ActivityVelocityInfo, sizeof(ACTIVE_VEL_INFO));
		}
	}

	m_fCurVelocityX = 0.0f;
	m_fAccelX = 0.0f;

	m_fCurVelocityY = 0.0f;
	m_fAccelY = 0.0f;

	if( uiAction & ACTION_MOVE_LEFT )
	{
		SetDirection(FACING_LEFT);
		m_fCurVelocityX = -m_fMaxVelocityX;
	}
	else if( uiAction & ACTION_MOVE_RIGHT )
	{
		SetDirection(FACING_RIGHT);
		m_fCurVelocityX = m_fMaxVelocityX;
	}

	if( uiAction & ACTION_RUN_LEFT )
	{
		if( m_fCurVelocityX >= 0.0f )
			SetDirection(FACING_LEFT);

		m_fCurVelocityX = -m_fMaxVelocityX;
	}
	else if( uiAction & ACTION_RUN_RIGHT )
	{
		if( m_fCurVelocityX <= 0.0f )
			SetDirection(FACING_RIGHT);

		m_fCurVelocityX = m_fMaxVelocityX;
	}

	if( uiAction & ACTION_MOVE_UP )
	{
		m_fCurVelocityY = m_fJumpSpeed;
	}
	else if( uiAction & ACTION_MOVE_DOWN )
	{
		m_fCurVelocityY = -m_fJumpSpeed;
	}
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::CalKineticEnergy(float fElapsedTime)
{
	float fTotalVelocity = m_fCurVelocityX;
	if( !m_KnockBackInfo.bDelay )
		fTotalVelocity += m_KnockBackInfo.fKnockBackVelocityX;
	if( !m_ActivityVelocityInfo.bDelay )
		fTotalVelocity += m_ActivityVelocityInfo.fVelocityX;

	float fDistanceX = (fTotalVelocity * fElapsedTime);
	m_fPosX += fDistanceX;

	float fDistanceY = m_fCurVelocityY * fElapsedTime;
	m_fPosY += fDistanceY;

	if( m_pkCurMapGroup )
	{
		if( m_fPosX < TILE_WIDTH )
			m_fPosX = TILE_WIDTH;
		else if( m_fPosX > m_pkCurMapGroup->m_lGroupWidth - (TILE_WIDTH + 1) )
			m_fPosX = (float)m_pkCurMapGroup->m_lGroupWidth - (TILE_WIDTH + 1);

		if( m_fPosY < 0 )
			m_fPosY = 0;
		else if( m_fPosY > m_pkCurMapGroup->m_lGroupHeight )
			m_fPosY = (float)m_pkCurMapGroup->m_lGroupHeight;
	}
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::SetAction(UINT64 uiAction)
{
	m_uiCurAction = uiAction;
	CalVelocity(uiAction);

	switch( uiAction )
	{
	case ACTION_STOP:
		StateChange(GOB_STATE_STAND);
		return;
	case ACTION_RUN_STOP:
		StateChange(GOB_STATE_RUN_STOP);
		return;
	case ACTION_DEAD:
		StateChange(GOB_STATE_DEAD);
		return;
	}

	if( uiAction & ACTION_MOVE_LEFT ||
		uiAction & ACTION_MOVE_RIGHT )
		StateChange(GOB_STATE_WALK);

	if( uiAction & ACTION_RUN_LEFT ||
		uiAction & ACTION_RUN_RIGHT )
		StateChange(GOB_STATE_RUN);

	if( uiAction & ACTION_MOVE_DROP )
		StateChange(GOB_STATE_DROP);
	
	if( uiAction & ACTION_MOVE_UP )
		StateChange(GOB_STATE_WALK);
	else if( uiAction & ACTION_MOVE_DOWN )
		StateChange(GOB_STATE_WALK);
	
	if( uiAction & ACTION_ATTACK_SPEEDY )
		StateChange(GOB_STATE_ATTACK_SPEEDY);
	else if( uiAction & ACTION_ATTACK_MIGHTY )
		StateChange(GOB_STATE_ATTACK_MIGHTY);

	if( uiAction & ACTION_BEATTACKED )
		StateChange(GOB_STATE_ATTACKED);
	else if( uiAction & ACTION_CRITICAL_BEATTACKED )
		StateChange(GOB_STATE_CRITICAL_ATTACKED);

	if( uiAction & ACTION_SIT_DOWN )
		StateChange(GOB_STATE_SIT_DOWN);
	else if( uiAction & ACTION_SIT_WAIT )
		StateChange(GOB_STATE_SIT_WAIT);
	else if( uiAction & ACTION_SIT_STAND )
		StateChange(GOB_STATE_SIT_STAND);

	if( uiAction & ACTION_SKILL )
		StateChange(GOB_STATE_SKILL);
}

//------------------------------------------------------------------------------------
bool SPFlyCoordPhysics::IsActionChange(UINT64& uiAction)
{
	return SPGOBCoordPhysics::IsActionChange(uiAction);
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::OnStateStop()
{
	StateChange(GOB_STATE_IDLE);
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::OnStateWalk()
{
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::OnStateAttackSpeedY()
{
	if( m_fActionTime <= 0.0f && !m_ActivityVelocityInfo.bEnable )
	{
		UINT64 uiAction = ACTION_ATTACK_SPEEDY;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::OnStateAttackMighty()
{
	if( m_fActionTime <= 0.0f && !m_ActivityVelocityInfo.bEnable )
	{
		UINT64 uiAction = ACTION_ATTACK_MIGHTY;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::OnStateSkill()
{
	if( m_fActionTime <= 0.0f && !m_ActivityVelocityInfo.bEnable )
	{
		UINT64 uiAction = 0xFFFFFFFFFFFFFFFF;
		SendActionCompleteCommand((LPARAM)&uiAction);

		uiAction = ACTION_SKILL;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::OnStateAttacked()
{
	if( m_fActionTime <= 0.0f && !m_KnockBackInfo.bKnockBack )
	{
		UINT64 uiAction = ACTION_BEATTACKED;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::OnStateDead()
{
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::OnStateRun()
{
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::OnStateDrop()
{
	UINT64 uiAction = ACTION_MOVE_DROP;
	SendActionCompleteCommand((LPARAM)&uiAction);
}

//------------------------------------------------------------------------------------
void SPFlyCoordPhysics::OnStateGuardDamage()
{
	if( m_fActionTime <= 0.0f && !m_KnockBackInfo.bKnockBack )
	{
		UINT64 uiAction = ACTION_GUARD_DAMAGE;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}