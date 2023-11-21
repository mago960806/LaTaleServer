
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
#include "SPMonsterCoordPhysics.h"

//------------------------------------------------------------------------------------
#ifdef _LATALE_CLIENT
SPMonsterCoordPhysics::SPMonsterCoordPhysics(SPGameObject* pkParent)
#else
SPMonsterCoordPhysics::SPMonsterCoordPhysics(CCharacter* pkParent)
#endif
: SPGOBCoordPhysics(pkParent)
{
	Init();
}

//------------------------------------------------------------------------------------
SPMonsterCoordPhysics::~SPMonsterCoordPhysics()
{	
}

//------------------------------------------------------------------------------------
void SPMonsterCoordPhysics::Init()
{
	SPGOBCoordPhysics::Init();
}

//------------------------------------------------------------------------------------
void SPMonsterCoordPhysics::Process(float fElapsedTime)
{	
	SPGOBCoordPhysics::Process(fElapsedTime);
}

//------------------------------------------------------------------------------------
void SPMonsterCoordPhysics::CalVelocity(UINT64 uiAction)
{
	SPGOBCoordPhysics::CalVelocity(uiAction);

	m_fCurVelocityX = 0.0f;
	m_fAccelX = 0.0f;

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
}

//------------------------------------------------------------------------------------
bool SPMonsterCoordPhysics::IsActionChange(UINT64& uiAction)
{
	return SPGOBCoordPhysics::IsActionChange(uiAction);
}

//------------------------------------------------------------------------------------
void SPMonsterCoordPhysics::OnStateAttackSpeedY()
{
	bool bRevisionY = true;
	if( m_fCurVelocityY < 0.0f )
		bRevisionY = false;

	InspectStandLayer(bRevisionY);
	if( m_fCurVelocityY == 0.0f )
	{
		if( CheckDrop() )
			m_fAccelY = m_fGravity;
	}

	if( m_fCurVelocityY > 0.0f )
	{
		if( !CheckDrop() )
		{
			m_fCurVelocityY = 0.0f;
			m_fAccelY = 0.0f;

			ZeroMemory(&m_ActivityVelocityInfo, sizeof(ACTIVE_VEL_INFO));
		}
	}

	if( m_fActionTime <= 0.0f && !m_ActivityVelocityInfo.bEnable )
	{
		UINT64 uiAction = ACTION_ATTACK_SPEEDY;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPMonsterCoordPhysics::OnStateAttackMighty()
{
	bool bRevisionY = true;
	if( m_fCurVelocityY < 0.0f )
		bRevisionY = false;

	InspectStandLayer(bRevisionY);
	if( m_fCurVelocityY == 0.0f )
	{
		if( CheckDrop() )
			m_fAccelY = m_fGravity;
	}

	if( m_fCurVelocityY > 0.0f )
	{
		if( !CheckDrop() )
		{
			m_fCurVelocityY = 0.0f;
			m_fAccelY = 0.0f;

			ZeroMemory(&m_ActivityVelocityInfo, sizeof(ACTIVE_VEL_INFO));
		}
	}

	if( m_fActionTime <= 0.0f && !m_ActivityVelocityInfo.bEnable )
	{
		UINT64 uiAction = ACTION_ATTACK_MIGHTY;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPMonsterCoordPhysics::OnStateSkill()
{
	bool bRevisionY = true;
	if( m_fCurVelocityY < 0.0f )
		bRevisionY = false;

	InspectStandLayer(bRevisionY);
	if( m_fCurVelocityY == 0.0f )
	{
		if( CheckDrop() )
			m_fAccelY = m_fGravity;
	}

	if( m_fCurVelocityY > 0.0f )
	{
		if( !CheckDrop() )
		{
			m_fCurVelocityY = 0.0f;
			m_fAccelY = 0.0f;

			ZeroMemory(&m_ActivityVelocityInfo, sizeof(ACTIVE_VEL_INFO));
		}
	}

	if( m_fActionTime <= 0.0f && !m_ActivityVelocityInfo.bEnable )
	{
		UINT64 uiAction = 0xFFFFFFFFFFFFFFFF;
		SendActionCompleteCommand((LPARAM)&uiAction);

		uiAction = ACTION_SKILL;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}