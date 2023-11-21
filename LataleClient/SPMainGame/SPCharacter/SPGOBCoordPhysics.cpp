
#include <vector>
#include <string>

// 서버
#ifdef _LATALE_SERVER
	#include "../LataleGAmeServer/StdAfx.h"
	#include "../LataleGAmeServer/BaseAbility.h"
	#include "../LataleGAmeServer/character.h"

	#include "../LataleGAmeServer/PCMain.h"
	#include "../LataleGAmeServer/BattleSystem.h"

	#include "../LataleMapLoader/SPCommandConvert.h"
	#include "../LataleGameLogic/UDFCommand.h"
#endif

// 클라이언트
#ifdef _LATALE_CLIENT	
	#include "SPCommon.h"	
	//TODO : Client Side
#endif

#include "SPGameObject.h"
#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"
#include "SPCommandConvert.h"
#include "SPGOBCoordPhysics.h"

//------------------------------------------------------------------------------------
#ifdef _LATALE_CLIENT
SPGOBCoordPhysics::SPGOBCoordPhysics(SPGameObject* pkParent)
#else
SPGOBCoordPhysics::SPGOBCoordPhysics(CCharacter* pkParent)
#endif
: m_pkParent(pkParent)
, m_iCurStageID(0)
, m_iCurMapGroupID(0)
, m_bIgnoreStage(false)
{
	Init();
}

//------------------------------------------------------------------------------------
SPGOBCoordPhysics::~SPGOBCoordPhysics()
{
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::Init()
{
	m_bMove = true;

	m_fPosX = 0.0f;
	m_fPosY = 0.0f;

	m_fBeforePosX = 0.0f;
	m_fBeforePosY = 0.0f;

	m_fMapOffsetPosX = 0.0f;
	m_fMapOffsetPosY = 0.0f;
	
	m_fMaxVelocityX = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fMaxUpDownVelocity = 0.0f;
	m_fMaxHangingVelocity = 0.0f;
	m_fCurVelocityX = 0.0f;
	m_fCurVelocityY = 0.0f;
	m_fAccelX = 0.0f;
	m_fAccelY = 0.0f;
	m_fJumpSpeed = 0.0f;
	m_fSimulateJumpLength = 0.0f;
	m_fMaxJumpPosY = 0.0f;
	m_fMaxJumpPosTime = 0.0f;
	m_fGravity = 0.0f;
	m_fMaxDropVelocity = 0.0f;

	m_fDefGravity = 0.0f;
	m_fDefDropVelocity = 0.0f;
	m_fDefVelocityX = 0.0f;
	m_fDefJumpSpeed = 0.0f;
	m_fDefUpDownVelocity = 0.0f;
	m_fDefHangingVelocity = 0.0f;
	
	m_eFacing = FACING_LEFT;
	m_eStandLayer = BG_ID_LAYER_NULL;
	m_eCurState = GOB_STATE_NULL;
	
	m_uiTerAttribute = 0;
	m_uiBeforeTerAttribute = 0;
	m_iRevision = 0;
	m_cMoveVerType = -1;
	m_bAutoHanging = true;
	m_fAutoHangingPosY = 0.0f;

	m_uiCurAction = 0;
	m_fActionTime = 0.0f;

	m_pkCurStage = NULL;
	m_pkCurMapGroup = NULL;
	m_pkCurTerrainAttribute = NULL;

	m_iCurMapGroupID	= 0;
	m_iCurStageID		= 0;

	m_bJumpAttack = true;
	m_bTerrainAttributeChange = true;
	
	ZeroMemory(&m_KnockBackInfo, sizeof(KNOCKBACK_INFO));
	ZeroMemory(&m_ActivityVelocityInfo, sizeof(KNOCKBACK_INFO));

#if defined(_LATALE_SERVER)
	m_fProcessAccumulateTime = 0.0f;
#endif
}


//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::SetCurStageID(int iStageID)
{
	m_iCurStageID = iStageID;
	m_pkCurStage = g_StageManager.GetCurStage(iStageID);
	//m_pkCurMapGroup = NULL;
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::SetCurMapGroupID(int iMapGroupID)
{	
	//if(m_pkCurStage && m_pkCurMapGroup) {
	//	m_fTempDropVelocity = m_fMaxDropVelocity - m_pkCurMapGroup->m_fMaxDropSpeed;
	//	m_fTempVelocityX = m_fMaxVelocityX - m_pkCurMapGroup->m_fVelocityX;
	//	m_fTempJumpSpeed = m_fJumpSpeed - m_pkCurMapGroup->m_fJumpSpeed;
	//}
	
	m_iCurMapGroupID = iMapGroupID;
	SPStage* pStage = g_StageManager.GetCurStage(GetCurStageID());
	if( pStage )
	{
		m_pkCurMapGroup = pStage->GetMapGroup(iMapGroupID);
		m_pkCurTerrainAttribute = pStage->GetCurTerrainAttribute(
			m_fPosX, m_fPosY, m_fMapOffsetPosX, m_fMapOffsetPosY, m_iCurMapGroupID);
	}
	
	SetGravity(m_fDefGravity);
	SetMaxDropVelocity(m_fDefDropVelocity);
	SetMaxVelocityX(m_fDefVelocityX);
	SetJumpSpeed(m_fDefJumpSpeed);
	SetMaxUpDownVelocity(m_fDefUpDownVelocity);
	SetMaxHangingVelocity(m_fDefHangingVelocity);
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::SetFirstAction()
{
	UINT64 uiAction = ACTION_STOP;
	SendActionCommand((LPARAM)&uiAction);
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::Process(float fElapsedTime)
{
	// 2005.12.29 dhpark 로직쓰레드에서 64프레임정도로 유지되므로 주석처리합니다.
	/*	
#if defined(_LATALE_SERVER)
	m_fProcessAccumulateTime += fElapsedTime;
	if( m_fProcessAccumulateTime < 0.03f )
		return;

	fElapsedTime = m_fProcessAccumulateTime;
	m_fProcessAccumulateTime = 0.0f;
#endif
	*/

	if( !m_bIgnoreStage )
	{
		if( m_pkCurStage == NULL ||
			m_pkCurMapGroup == NULL )
			return;

		if( ReCalKineticEnergy() )	
			CalKineticEnergy(fElapsedTime);
		
		if( m_eCurState != GOB_STATE_IDLE )
		{
			if( m_pkCurTerrainAttribute == NULL ||
				m_fPosX != m_fBeforePosX || m_fPosY != m_fBeforePosY ||
				m_bTerrainAttributeChange )
			{
				m_pkCurTerrainAttribute = m_pkCurStage->GetCurTerrainAttribute(
					m_fPosX, m_fPosY, m_fMapOffsetPosX, m_fMapOffsetPosY, m_iCurMapGroupID);
				
				m_bTerrainAttributeChange = false;
				if( !m_pkCurTerrainAttribute )
					return;
			}
		}
	}

	StateProcess();

	m_fBeforePosX = m_fPosX;
	m_fBeforePosY = m_fPosY;

	ProcessTimeTick(fElapsedTime);
}


//////////////////////////////////////////////////////////////////////////
void SPGOBCoordPhysics::SetCoordPhysicsMap(int iStageID, int iMapGroupID)
{
	SetCurStageID(iStageID);
	SetCurMapGroupID(iMapGroupID);

	//Stage내부 데이터가 변경(삭제)되었는데 예전 스테이지및 그룹 포인터를 
	//참조하려고 하기때문에 CoordPhysics 에서 에러 발생
	//Stage변경시 두 값을 NULL로 참조 못하게 막음
	//m_pkCurStage = NULL;
	//m_pkCurMapGroup = NULL;
}


//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::ProcessTimeTick(float fElapsedTime)
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
				m_ActivityVelocityInfo.bDelay = false;

				if( m_ActivityVelocityInfo.fVelocityY )
				{
					m_fCurVelocityY = m_ActivityVelocityInfo.fVelocityY;
					m_fAccelY = m_fGravity;

					m_ActivityVelocityInfo.fVelocityY = 0.0f;
				}
			}
		}
		else
		{
			if( m_ActivityVelocityInfo.fAccmulateTime > m_ActivityVelocityInfo.fTime &&
				m_ActivityVelocityInfo.fVelocityY == 0.0f &&
				m_fCurVelocityY == 0.0f)
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
				m_KnockBackInfo.bDelay = false;

				if( m_KnockBackInfo.fKnockBackVelocityY )
				{
					m_fCurVelocityY = m_KnockBackInfo.fKnockBackVelocityY;
					m_fAccelY = m_fGravity;

					m_KnockBackInfo.fKnockBackVelocityY = 0.0f;
				}
			}
		}
		else
		{
			if( m_KnockBackInfo.fKnockBackAccmulateTime > m_KnockBackInfo.fKnockBackTime &&
				m_KnockBackInfo.fKnockBackVelocityY == 0.0f &&
				m_fCurVelocityY == 0.0f)
			{
				ZeroMemory(&m_KnockBackInfo, sizeof(KNOCKBACK_INFO));
			}
		}
	}
}

//------------------------------------------------------------------------------------
bool SPGOBCoordPhysics::ReCalKineticEnergy()
{
	switch( m_eCurState )
	{
	case GOB_STATE_IDLE:
	case GOB_STATE_STAND:
	case GOB_STATE_VERTICAL_STAND:
	case GOB_STATE_HANGING_STAND:
	case GOB_STATE_RUN_STOP:
	case GOB_STATE_LAND:
	case GOB_STATE_GUARD:
	case GOB_STATE_DEAD:
	case GOB_STATE_LOOTING:
	case GOB_STATE_LOOTING_WAIT:
	case GOB_STATE_LOOTING_STAND:
	case GOB_STATE_SIT_DOWN:
	case GOB_STATE_SIT_WAIT:
	case GOB_STATE_SIT_STAND:
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------------
// fElapsedTime타임이 클경우 x, y값이 크게 변동되어 밑으로 떨어짐
// 벽이 있고 y축 속도가 양일때 빠짐
void SPGOBCoordPhysics::CalKineticEnergy(float fElapsedTime)
{
	float fTotalVelocity = m_fCurVelocityX;
	if( !m_KnockBackInfo.bDelay )
		fTotalVelocity += m_KnockBackInfo.fKnockBackVelocityX;
	if( !m_ActivityVelocityInfo.bDelay )
		fTotalVelocity += m_ActivityVelocityInfo.fVelocityX;

	float fDistanceX = (fTotalVelocity * fElapsedTime);
	if( fDistanceX < -SPEED_LIMIT_LAND )
		fDistanceX = -SPEED_LIMIT_LAND;
	else if( fDistanceX > SPEED_LIMIT_LAND )
		fDistanceX = SPEED_LIMIT_LAND;

	m_fPosX += fDistanceX;

	//m_fAccmulateTimeY += fElapsedTime;
	//m_fCurVelocityY = m_fStrVelocityY + (m_fAccelY * m_fAccmulateTimeY);
	//float fPosY = m_fStrPosY + m_fCurVelocityY * m_fAccmulateTimeY;
	//float fDistanceY = fPosY - m_fPosY;

	if( m_fCurVelocityY || m_fAccelY )
	{
		int		m_fStarVelocityY	= m_fCurVelocityY;
		m_fCurVelocityY += (m_fAccelY * fElapsedTime);
		if( m_fCurVelocityY > 1000.0f )
			m_fCurVelocityY = 1000.0f;

		// float fDistanceY = m_fCurVelocityY * fElapsedTime;
		float fDistanceY = (m_fStarVelocityY+m_fCurVelocityY) * 0.5 * fElapsedTime;
		if( fDistanceY > m_fMaxDropVelocity )
			fDistanceY = m_fMaxDropVelocity;

		if( fDistanceY > 15.0f && m_eCurState != GOB_STATE_JUMP )
		{
			if( !CheckDrop(m_fPosX, m_fPosY + 15.0f) )
				fDistanceY = 15.0f;
		}
		
		if( (m_eCurState != GOB_STATE_JUMP) || ((m_eCurState == GOB_STATE_JUMP) && (m_fCurVelocityY < 0.0f)) )
			m_fPosY += fDistanceY;
	}
	
	if( m_pkCurMapGroup )
	{
		if( m_fPosX < TILE_WIDTH )
			m_fPosX = TILE_WIDTH;
		else if( m_fPosX > m_pkCurMapGroup->m_lGroupWidth - (TILE_WIDTH + 1) )
			m_fPosX = (float)m_pkCurMapGroup->m_lGroupWidth - (TILE_WIDTH + 1);

		if( m_fPosY < 0 )
			m_fPosY = 0;
		else if( m_fPosY > m_pkCurMapGroup->m_lGroupHeight )
		{
			char szBuf[128];

#ifdef _LATALE_CLIENT
			
			sprintf(szBuf, "RestorePos = stage:%d, mapgroup:%d, x:%.1f, y:%.1f\n",
				m_iCurStageID, m_iCurMapGroupID, m_fPosX, m_fPosY);
#else
			if( m_pkParent )
			{
				sprintf(szBuf, "RestorePos : classID=[%d],InstanceId=[%d] [%s] stage:%d, mapgroup:%d, x:%.1f, y:%.1f\n",
					m_pkParent->GetClassID(),m_pkParent->GetID(),m_pkParent->GetName(), m_iCurStageID, m_iCurMapGroupID, m_fPosX, m_fPosY);
				m_pkParent->OnRestorePos();
			}
#endif

			OutputDebugString(szBuf);

			m_fPosY = (float)m_pkCurMapGroup->m_lGroupHeight;
			RestorePos();
		}
	}

//	float fTotalVelocity = m_fCurVelocityX;
//	if( !m_KnockBackInfo.bDelay )
//		fTotalVelocity += m_KnockBackInfo.fKnockBackVelocityX;
//	if( !m_ActivityVelocityInfo.bDelay )
//		fTotalVelocity += m_ActivityVelocityInfo.fVelocityX;
//
//	float fDistanceX = (fTotalVelocity * fElapsedTime);
//	if( fDistanceX < -SPEED_LIMIT_LAND )
//		fDistanceX = -SPEED_LIMIT_LAND;
//	else if( fDistanceX > SPEED_LIMIT_LAND )
//		fDistanceX = SPEED_LIMIT_LAND;
//
//	m_fPosX += fDistanceX;
//
//	//m_fAccmulateTimeY += fElapsedTime;
//	//m_fCurVelocityY = m_fStrVelocityY + (m_fAccelY * m_fAccmulateTimeY);
//	//float fPosY = m_fStrPosY + m_fCurVelocityY * m_fAccmulateTimeY;
//	//float fDistanceY = fPosY - m_fPosY;
//
//	if( m_fCurVelocityY || m_fAccelY )
//	{
//		m_fCurVelocityY += (m_fAccelY * fElapsedTime);
//		if( m_fCurVelocityY > 1000.0f )
//			m_fCurVelocityY = 1000.0f;
//
//		float fDistanceY = m_fCurVelocityY * fElapsedTime;
//		if( fDistanceY > m_fMaxDropVelocity )
//			fDistanceY = m_fMaxDropVelocity;
//
//		if( fDistanceY > 15.0f && m_eCurState != GOB_STATE_JUMP )
//		{
//			if( !CheckDrop(m_fPosX, m_fPosY + 15.0f) )
//				fDistanceY = 15.0f;
//		}
//		
//		if( (m_eCurState != GOB_STATE_JUMP) || ((m_eCurState == GOB_STATE_JUMP) && (m_fCurVelocityY < 0.0f)) )
//			m_fPosY += fDistanceY;
//	}
//	
//	if( m_pkCurMapGroup )
//	{
//		if( m_fPosX < TILE_WIDTH )
//			m_fPosX = TILE_WIDTH;
//		else if( m_fPosX > m_pkCurMapGroup->m_lGroupWidth - (TILE_WIDTH + 1) )
//			m_fPosX = (float)m_pkCurMapGroup->m_lGroupWidth - (TILE_WIDTH + 1);
//
//		if( m_fPosY < 0 )
//			m_fPosY = 0;
//		else if( m_fPosY > m_pkCurMapGroup->m_lGroupHeight )
//		{
//			char szBuf[128];
//
//#ifdef _LATALE_CLIENT
//			
//			sprintf(szBuf, "RestorePos = stage:%d, mapgroup:%d, x:%.1f, y:%.1f\n",
//				m_iCurStageID, m_iCurMapGroupID, m_fPosX, m_fPosY);
//#else
//			if( m_pkParent )
//			{
//				sprintf(szBuf, "RestorePos : classID=[%d],InstanceId=[%d] [%s] stage:%d, mapgroup:%d, x:%.1f, y:%.1f\n",
//					m_pkParent->GetClassID(),m_pkParent->GetID(),m_pkParent->GetName(), m_iCurStageID, m_iCurMapGroupID, m_fPosX, m_fPosY);
//				m_pkParent->OnRestorePos();
//			}
//#endif
//
//			OutputDebugString(szBuf);
//
//			m_fPosY = (float)m_pkCurMapGroup->m_lGroupHeight;
//			RestorePos();
//		}
//	}
}

//------------------------------------------------------------------------------------
// 빠졌을때 복귀
void SPGOBCoordPhysics::RestorePos()
{
	if( m_pkCurStage == NULL ||
		m_pkCurMapGroup == NULL )
		return;

	/*
	unsigned int uiAttribute;
	float fPosX, fPosY, fMapOffsetPosX, fMapOffsetPosY;
	SPTerrainAttribute* pkCurTerrainAttribute;

	fPosX = m_fPosX;
	fPosY = m_fPosY;
	while( 1 )
	{
		m_fPosY -= TILE_HEIGHT;
		fPosY = m_fPosY;

		GetNextAttriState(fPosX, fPosY, uiAttribute);

		pkCurTerrainAttribute = m_pkCurStage->GetCurTerrainAttribute(
			fPosX, fPosY, fMapOffsetPosX, fMapOffsetPosY, m_iCurMapGroupID);

		if( pkCurTerrainAttribute == NULL || m_fPosY < 0.0f )
		{
			m_fPosY = 0.0f;
			break;
		}

		if( m_pkCurTerrainAttribute->IsGround(uiAttribute, fMapOffsetPosX, fMapOffsetPosY) )
		{
			m_fPosY = fPosY;
			break;
		}
	}
	*/

	m_fPosY = 0.0f;
	UINT64 uiAction = ACTION_MOVE_DROP;
	SendActionCommand((LPARAM)&uiAction);
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::CalVelocity(UINT64 uiAction)
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

		//if( m_KnockBackInfo.bKnockBack && m_KnockBackInfo.fKnockBackVelocityY )
		//{
		//	m_fCurVelocityY = m_KnockBackInfo.fKnockBackVelocityY;
		//	m_fAccelY = m_fGravity;
		//}

		if( uiAction == ACTION_STOP )
		{
			ZeroMemory(&m_KnockBackInfo, sizeof(KNOCKBACK_INFO));
			ZeroMemory(&m_ActivityVelocityInfo, sizeof(ACTIVE_VEL_INFO));
		}
		
		return;
	}

	//if( m_eCurState != GOB_STATE_JUMP && m_eCurState != GOB_STATE_DROP )
	//{
	//	m_fCurVelocityY = 0.0f;
	//	m_fAccelY = 0.0f;
	//}

	int iDistX;

	if( uiAction & ACTION_MOVE_UP )
	{
		m_fCurVelocityX = 0.0f;
		m_fAccelX = 0.0f;

		m_fCurVelocityY = -m_fMaxUpDownVelocity;
		m_fAccelY = 0.0f;

		iDistX = (int)m_fMapOffsetPosX % TILE_WIDTH;
		m_fPosX += (TILE_WIDTH / 2 - iDistX);
	}
	else if( uiAction & ACTION_MOVE_DOWN )
	{
		m_fCurVelocityX = 0.0f;
		m_fAccelX = 0.0f;

		m_fCurVelocityY = m_fMaxUpDownVelocity;
		m_fAccelY = 0.0f;

		iDistX = (int)m_fMapOffsetPosX % TILE_WIDTH;
		m_fPosX += (TILE_WIDTH / 2 - iDistX);
	}
	
	if( (uiAction & ACTION_MOVE_JUMP) &&
		m_eCurState != GOB_STATE_JUMP )//m_eCurState != GOB_STATE_JUMP && m_eCurState != GOB_STATE_DROP )
	{
		m_fCurVelocityY = m_fJumpSpeed;
		m_fAccelY = m_fGravity;

		m_fMaxJumpPosY = m_fPosY + m_fSimulateJumpLength;
	}
	else if( uiAction & ACTION_MOVE_DROP &&
		m_eCurState != GOB_STATE_DROP )
	{
		m_fCurVelocityY = 0.0f;
		m_fAccelY = m_fGravity;
	}
}

//------------------------------------------------------------------------------------
bool SPGOBCoordPhysics::CheckDrop()
{
	if( m_pkCurTerrainAttribute == NULL )
		return false;

	bool bDrop;
	bDrop = m_pkCurTerrainAttribute->MoveDrop(m_fMapOffsetPosX, m_fMapOffsetPosY, BG_ID_LAYER_NULL);
	if( bDrop )
	{
		if( m_uiTerAttribute > 0 && m_uiBeforeTerAttribute > 0 )
		{
			if( m_pkCurTerrainAttribute->IsGround((BYTE)m_uiTerAttribute,
				(int)m_fMapOffsetPosX, (int)m_fMapOffsetPosY) )
				bDrop = false;
		}
	}
	
	m_uiBeforeTerAttribute = m_uiTerAttribute;
	return bDrop;
}

//------------------------------------------------------------------------------------
bool SPGOBCoordPhysics::CheckDrop(float fPosX, float fPosY)
{
	if( m_pkCurStage == NULL )
		return false;

	float fMapOffsetPosX, fMapOffsetPosY;
	SPTerrainAttribute* pkCurTerrainAttribute;
	pkCurTerrainAttribute = m_pkCurStage->GetCurTerrainAttribute(
		fPosX, fPosY, fMapOffsetPosX, fMapOffsetPosY, m_iCurMapGroupID);

	if( pkCurTerrainAttribute == NULL )
		return false;

	BG_ID eStandLayer;
	int iRevision;
	unsigned int uiTerAttribute = pkCurTerrainAttribute->InspectStandLayer(
		(int)fMapOffsetPosX, (int)fMapOffsetPosY, 
		eStandLayer, iRevision);

	bool bMoveEnable = CheckMoveEnable(uiTerAttribute, fPosX, fPosY,
		&fMapOffsetPosX, &fMapOffsetPosY, pkCurTerrainAttribute);

	if( !bMoveEnable )
	{
		pkCurTerrainAttribute = m_pkCurStage->GetCurTerrainAttribute(
			fPosX, fPosY, fMapOffsetPosX, fMapOffsetPosY, m_iCurMapGroupID);
		if( pkCurTerrainAttribute == NULL )
			return false;

		uiTerAttribute = pkCurTerrainAttribute->InspectStandLayer(
			(int)fMapOffsetPosX, (int)fMapOffsetPosY, 
			eStandLayer, iRevision);
	}

	bool bDrop = pkCurTerrainAttribute->MoveDrop(fMapOffsetPosX, fMapOffsetPosY, BG_ID_LAYER_NULL);
	if( bDrop )
	{
		if( uiTerAttribute > 0 && m_uiTerAttribute > 0 )
		{
			if( pkCurTerrainAttribute->IsGround((BYTE)uiTerAttribute,
				(int)fMapOffsetPosX, (int)fMapOffsetPosY) )
				bDrop = false;
		}

		/*
		//		|				|
		//	----  *		      *	 ----		
		//	벽 대각선으로 빠지는 체크
		if( m_fBeforePosX != fPosX && bDrop )
		{
			bool bUpBlock = false;
			pkCurTerrainAttribute = m_pkCurStage->GetCurTerrainAttribute(
				fPosX, fPosY - TILE_HEIGHT, fMapOffsetPosX, fMapOffsetPosY, m_iCurMapGroupID);
			if( pkCurTerrainAttribute )
			{
				uiTerAttribute = pkCurTerrainAttribute->InspectStandLayer(
					fMapOffsetPosX, fMapOffsetPosY, 
					eStandLayer, iRevision);
				
				if( pkCurTerrainAttribute->IsBlock(uiTerAttribute) )
					bUpBlock = true;
			}

			if( bUpBlock )
			{
				for( int i = 0; i < 2; i++ )
				{
					pkCurTerrainAttribute = m_pkCurStage->GetCurTerrainAttribute(
						m_fBeforePosX, fPosY - i * TILE_HEIGHT, fMapOffsetPosX, fMapOffsetPosY, m_iCurMapGroupID);

					if( pkCurTerrainAttribute )
					{
						uiTerAttribute = pkCurTerrainAttribute->InspectStandLayer(
							fMapOffsetPosX, fMapOffsetPosY, 
							eStandLayer, iRevision);

						if( pkCurTerrainAttribute->IsGround((BYTE)uiTerAttribute,
							fMapOffsetPosX, fMapOffsetPosY) )
						{
							bDrop = false;
#if defined(_DEBUG)
							OutputDebugString("==========Blcok Drop\n");
#endif
							break;
						}
					}
				}
			}
		}
		*/
	}

	return bDrop;
}

bool SPGOBCoordPhysics::GetMoveEnable(float& fPosX, float& fPosY, int iDir)
{
	if ( iDir == 0 )
	{
		if( !m_pkCurStage )
			return false;

		float fMapOffsetPosX, fMapOffsetPosY;
		SPTerrainAttribute * pkCurTerrainAttribute = m_pkCurStage->GetCurTerrainAttribute(
			fPosX, fPosY, fMapOffsetPosX, fMapOffsetPosY, m_iCurMapGroupID );
		if( pkCurTerrainAttribute == NULL )
			return false;

		int iRevision;
		BG_ID eStandLayer;
		unsigned int uiTerAttribute = pkCurTerrainAttribute->InspectStandLayer(
			(int)fMapOffsetPosX, (int)fMapOffsetPosY, eStandLayer, iRevision);

		m_bMove = CheckMoveEnable(uiTerAttribute, fPosX, fPosY,
			&fMapOffsetPosX, &fMapOffsetPosY, pkCurTerrainAttribute);
	}
	else if ( iDir == 1 )
	{
		m_bMove = (EnableLadder(true) || EnableVerRope(true));
	}
	else if ( iDir == 2 )
	{
		m_bMove = (EnableLadder(false) || EnableVerRope(false));
	}

	return m_bMove;
}

//------------------------------------------------------------------------------------
bool SPGOBCoordPhysics::CheckMoveEnable(unsigned int uiAttribute,
	float& fPosX, float& fPosY, float* pfMapOffsetX, float* pfMapOffsetY,
	SPTerrainAttribute* pkTerrainAttribute)
{
	if( pkTerrainAttribute == NULL )
		pkTerrainAttribute = m_pkCurTerrainAttribute;

	if( pkTerrainAttribute == NULL )
		return false;

	int	bMove = true;
	if( pkTerrainAttribute->IsBlock((BYTE)uiAttribute) )
		bMove = !CheckBlock(uiAttribute, fPosX, fPosY);

//	if( !bMove )
//	{
//		if( pfMapOffsetX && pfMapOffsetY )
//			m_pkCurMapGroup->GetMapPos(fPosX, fPosY, *pfMapOffsetX, *pfMapOffsetY);
//	}
	
	return bMove;
}

//------------------------------------------------------------------------------------
bool SPGOBCoordPhysics::CheckBlock(unsigned int uiAttribute, float& fPosX, float& fPosY)
{
	bool bBlock = false;
	int iDistX;
	
	iDistX = (int)m_fMapOffsetPosX % TILE_WIDTH;
	
	FACING eDir;
	if( m_fBeforePosX < fPosX )
		eDir = FACING_RIGHT;
	else if( m_fBeforePosX > fPosX )
		eDir = FACING_LEFT;
	else
		return true;

	switch( uiAttribute )
	{
	case 0x04:
		{
			if( eDir == FACING_RIGHT )
			{
				fPosX += (-1 - iDistX);
				bBlock = true;
			}
			//else
			//{
			//	if( iDistX < 2 )
			//	{
			//		fPosX += (2 - iDistX);
			//		bBlock = true;
			//	}
			//}
		}
		break;

	case 0x14:
		{
			if( eDir == FACING_RIGHT )
			{
				if( iDistX >= (TILE_WIDTH / 2) )
				{
					fPosX += (TILE_WIDTH / 2 - 1 - iDistX);
					bBlock = true;
				}
			}
			else
			{
				if( iDistX < (TILE_WIDTH / 2) )
				{
					fPosX += (TILE_WIDTH / 2 - iDistX);
					bBlock = true;
				}
			}
		}
		break;

	case 0x05:
		{
			if( eDir == FACING_LEFT )
			{
				fPosX += (TILE_WIDTH - iDistX);
				bBlock = true;
			}
			//if( eDir == FACING_RIGHT )
			//{
			//	if( iDistX > TILE_WIDTH - 3 )
			//	{
			//		fPosX += (TILE_WIDTH - 3 - iDistX);
			//		bBlock = true;
			//	}
			//}
			//else
			//{
			//	fPosX += (TILE_WIDTH - iDistX);
			//	bBlock = true;
			//}
		}
		break;
	}
	
	return bBlock;
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::InspectStandLayer(bool bRevisionY)
{
	if( m_pkCurStage == NULL || m_pkCurMapGroup == NULL ||
		m_pkCurTerrainAttribute == NULL )
		return;

	m_uiTerAttribute = m_pkCurTerrainAttribute->InspectStandLayer(
		(int)m_fMapOffsetPosX, (int)m_fMapOffsetPosY, 
		m_eStandLayer, m_iRevision);

	bool bMoveEnable = CheckMoveEnable(m_uiTerAttribute, m_fPosX, m_fPosY,
		&m_fMapOffsetPosX, &m_fMapOffsetPosY);

	if( !bMoveEnable )
	{
		m_pkCurTerrainAttribute = m_pkCurStage->GetCurTerrainAttribute(
			m_fPosX, m_fPosY, m_fMapOffsetPosX, m_fMapOffsetPosY, m_iCurMapGroupID);
		if( m_pkCurTerrainAttribute == NULL )
			return;

		m_uiTerAttribute = m_pkCurTerrainAttribute->InspectStandLayer(
			(int)m_fMapOffsetPosX, (int)m_fMapOffsetPosY, 
			m_eStandLayer, m_iRevision);
	}

	if( m_fCurVelocityY < 0.0f && CheckUpBlock() )
	{
		m_fCurVelocityY = 0.5f;
		m_fMaxJumpPosY = m_fPosY;
	}

	bool bLadder = false;
	if( m_pkCurTerrainAttribute->IsLadder(m_uiTerAttribute) ||
		m_pkCurTerrainAttribute->IsVerRope(m_uiTerAttribute) )
		bLadder = true;

	if( m_pkCurTerrainAttribute->IsHozRope(m_uiTerAttribute) )
	{
		if( m_eCurState == GOB_STATE_ATTACKED ||
			m_eCurState == GOB_STATE_CRITICAL_ATTACKED ||
			m_eCurState == GOB_STATE_ATTACKED_DOWN ||
			m_eCurState == GOB_STATE_ATTACK_SPEEDY ||
			m_eCurState == GOB_STATE_ATTACK_MIGHTY ||
			m_eCurState == GOB_STATE_SKILL ||
			m_eCurState == GOB_STATE_GUARD ||
			m_eCurState == GOB_STATE_GUARD_DAMAGE )
			return;
	}
	
	if( bMoveEnable && bRevisionY && !bLadder )
	{
		if( !CheckUpBlock() )
			m_fPosY += m_iRevision;
		m_pkCurMapGroup->GetMapPos(m_fPosX, m_fPosY, m_fMapOffsetPosX, m_fMapOffsetPosY);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::SetAction(UINT64 uiAction)
{
	m_uiCurAction = uiAction;
	CalVelocity(uiAction);

	switch( uiAction )
	{
	case ACTION_STOP:
		StateChange(GOB_STATE_STAND);
		return;
	case ACTION_VERTICAL_STOP:
		StateChange(GOB_STATE_VERTICAL_STAND);
		return;
	case ACTION_HANGING_STOP:
		StateChange(GOB_STATE_HANGING_STAND);
		return;
	case ACTION_LOOTING_WAIT:
		StateChange(GOB_STATE_LOOTING_WAIT);
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

	if( uiAction & ACTION_MOVE_HANGING_LEFT ||
		uiAction & ACTION_MOVE_HANGING_RIGHT )
		StateChange(GOB_STATE_HANGING);

	if( uiAction & ACTION_MOVE_UP )
		StateChange(GOB_STATE_UP);
	else if( uiAction & ACTION_MOVE_DOWN )
		StateChange(GOB_STATE_DOWN);
	
	if( uiAction & ACTION_MOVE_JUMP )
		StateChange(GOB_STATE_JUMP);
	else if( uiAction & ACTION_MOVE_DROP )
		StateChange(GOB_STATE_DROP);

	if( uiAction & ACTION_ATTACK_SPEEDY )
		StateChange(GOB_STATE_ATTACK_SPEEDY);
	else if( uiAction & ACTION_ATTACK_MIGHTY )
		StateChange(GOB_STATE_ATTACK_MIGHTY);

	if( uiAction & ACTION_BEATTACKED )
		StateChange(GOB_STATE_ATTACKED);
	else if( uiAction & ACTION_CRITICAL_BEATTACKED )
		StateChange(GOB_STATE_CRITICAL_ATTACKED);
	
	if( uiAction & ACTION_GUARD_DAMAGE )
		StateChange(GOB_STATE_GUARD_DAMAGE);

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
bool SPGOBCoordPhysics::IsActionChange(UINT64& uiAction)
{
	bool bRet = true;
	if( m_pkCurMapGroup == NULL )
		return false;
	
	switch( m_eCurState )
	{
	case GOB_STATE_ATTACKED:
	case GOB_STATE_CRITICAL_ATTACKED:
		{
			bRet = false;
			if( uiAction & ACTION_BEATTACKED ||
				uiAction & ACTION_CRITICAL_BEATTACKED )
				bRet = true;
		}
		break;

	case GOB_STATE_DROP:
	case GOB_STATE_JUMP:
	case GOB_STATE_DEAD:
	case GOB_STATE_ATTACK_SPEEDY:
	case GOB_STATE_ATTACK_MIGHTY:
	case GOB_STATE_SIT_STAND:
	case GOB_STATE_SKILL:
		bRet = false;
		break;
	}
	
	return bRet;
}

//------------------------------------------------------------------------------------
bool SPGOBCoordPhysics::EnableLadder(bool bUp)
{
	if( m_pkCurTerrainAttribute == NULL )
		return false;

	bool bLadder;
	if( bUp )
		bLadder = m_pkCurTerrainAttribute->EnableLadderUp(m_fMapOffsetPosX, m_fMapOffsetPosY);
	else
		bLadder = m_pkCurTerrainAttribute->EnableLadderDown(m_fMapOffsetPosX, m_fMapOffsetPosY);

	return bLadder;
}

//------------------------------------------------------------------------------------
bool SPGOBCoordPhysics::EnableVerRope(bool bUp)
{
	if( m_pkCurTerrainAttribute == NULL )
		return false;
	
	bool bVerRope;
	if( bUp )
		bVerRope = m_pkCurTerrainAttribute->EnableVerRopeUp(m_fMapOffsetPosX, m_fMapOffsetPosY);
	else
		bVerRope = m_pkCurTerrainAttribute->EnableVerRopeDown(m_fMapOffsetPosX, m_fMapOffsetPosY);

	return bVerRope;
}

//------------------------------------------------------------------------------------
bool SPGOBCoordPhysics::EnableHozRope()
{
	if( m_pkCurTerrainAttribute == NULL )
		return false;
	
	bool bHozRope;
	bHozRope = m_pkCurTerrainAttribute->EnableHozRopeUp(m_fMapOffsetPosX, m_fMapOffsetPosY);

	return bHozRope;
}

//------------------------------------------------------------------------------------
bool SPGOBCoordPhysics::CheckUpBlock()
{
	if( m_pkCurTerrainAttribute == NULL )
		return false;

	return m_pkCurTerrainAttribute->IsUpBlock(m_uiTerAttribute);
}

//------------------------------------------------------------------------------------
int SPGOBCoordPhysics::SendActionCommand(LPARAM lParam)
{
	if( m_pkParent )
#ifdef _LATALE_CLIENT
		m_pkParent->SPGOBPerformMessage(SPGM_COMMAND_ACTION, lParam);
#else
		m_pkParent->ActionCommand(lParam);
#endif
	else
		DirectActionCommand(lParam);

	return 1;
}

//------------------------------------------------------------------------------------
int SPGOBCoordPhysics::SendActionCompleteCommand(LPARAM lParam)
{
	if( m_pkParent )
#ifdef _LATALE_CLIENT
		m_pkParent->SPGOBPerformMessage(SPGM_COMMAND_ACTION_COMPLETE, lParam);
#else
		m_pkParent->ActionCompleteCommand(lParam);
#endif
	else
		DirectActionCompleteCommand(lParam);

	return 1;
}

//------------------------------------------------------------------------------------
int SPGOBCoordPhysics::DirectActionCommand(LPARAM lParam)
{
	UINT64 uiTempAction = m_uiCurAction;
	UINT64 uiAction = *((UINT64*)lParam);
	uiTempAction |= uiAction;
	SetAction(uiTempAction);
	return 1;
}

//------------------------------------------------------------------------------------
int SPGOBCoordPhysics::DirectActionCompleteCommand(LPARAM lParam)
{
	UINT64 uiTempAction = m_uiCurAction;
	UINT64 uiAction = *((UINT64*)lParam);
	uiTempAction &= ~uiAction;
	SetAction(uiTempAction);
	return 1;
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::GetNextPos(float& fPosX, float& fPosY,
	float fVelocityX, float fVelocityY)
{
	if( !m_pkCurMapGroup )
		return;

	if( fVelocityX == 0.0f )
		fVelocityX = m_fCurVelocityX;
	if( fVelocityY == 0.0f )
		fVelocityY = m_fCurVelocityY;
	
	float fDistanceX, fDistanceY, fSimulateElapsedTime;

	// min 10frame/sec
	fSimulateElapsedTime = 0.1f;

	fDistanceX = (fVelocityX * fSimulateElapsedTime);
	if( fDistanceX < -SPEED_LIMIT_LAND )
		fDistanceX = -SPEED_LIMIT_LAND;
	else if( fDistanceX > SPEED_LIMIT_LAND )
		fDistanceX = SPEED_LIMIT_LAND;

	fDistanceY = fVelocityY * fSimulateElapsedTime;

	if( fDistanceY > m_fMaxDropVelocity )
		fDistanceY = m_fMaxDropVelocity;
	else if( fDistanceY < m_fJumpSpeed )
		fDistanceY = m_fJumpSpeed;

	fPosX += fDistanceX;
	fPosY += fDistanceY;

	if( fPosX < TILE_WIDTH )
		fPosX = TILE_WIDTH;
	else if( fPosX > m_pkCurMapGroup->m_lGroupWidth - (TILE_WIDTH + 1) )
		fPosX = (float)m_pkCurMapGroup->m_lGroupWidth - (TILE_WIDTH + 1);
	
	if( fPosY < 0 )
		fPosY = 0;
	else if( fPosY > m_pkCurMapGroup->m_lGroupHeight )
		fPosY = (float)m_pkCurMapGroup->m_lGroupHeight;
}

//------------------------------------------------------------------------------------
int SPGOBCoordPhysics::GetNextAttriState(float& fPosX, float& fPosY, unsigned int& uiAttribute)
{
	if( !m_pkCurStage || !m_pkCurMapGroup )
		return 1;

	float fMapOffsetPosX, fMapOffsetPosY;
	int iRevision, iRet;
	BG_ID eStandLayer;
	
	SPTerrainAttribute* pkCurTerrainAttribute = m_pkCurStage->GetCurTerrainAttribute(
		fPosX, fPosY, fMapOffsetPosX, fMapOffsetPosY, m_iCurMapGroupID);

	if( pkCurTerrainAttribute == NULL )
		return 1;

	uiAttribute = pkCurTerrainAttribute->InspectStandLayer(
		(int)fMapOffsetPosX, (int)fMapOffsetPosY, 
		eStandLayer, iRevision);

	bool bMoveEnable = CheckMoveEnable(uiAttribute, fPosX, fPosY,
		NULL, NULL, pkCurTerrainAttribute);

	if( !bMoveEnable )
	{
		pkCurTerrainAttribute = m_pkCurStage->GetCurTerrainAttribute(
			fPosX, fPosY, fMapOffsetPosX, fMapOffsetPosY, m_iCurMapGroupID);
		if( pkCurTerrainAttribute == NULL )
			return 1;

		uiAttribute = pkCurTerrainAttribute->InspectStandLayer(
			(int)fMapOffsetPosX, (int)fMapOffsetPosY, 
			eStandLayer, iRevision);
	}
	
	fPosY += iRevision;

	iRet = 0;
	if( fPosX < TILE_WIDTH || fPosX > m_pkCurMapGroup->m_lGroupWidth - (TILE_WIDTH + 1) )
		iRet = 1;
	if( fPosY > m_pkCurMapGroup->m_lGroupHeight )
		iRet = 1;

	// block
	if( !bMoveEnable )
		iRet = 1;

	// drop
	if( !uiAttribute )
		iRet = 2;

	// movable
	return iRet;
}

//------------------------------------------------------------------------------------
bool SPGOBCoordPhysics::IsJumpable(float& fPosX, float& fPosY)
{
	if( !m_pkCurStage || !m_pkCurTerrainAttribute || m_fJumpSpeed >= 0.0f )
		return false;

	int iCheckTile, i;
	unsigned int uiAttribute;
	float fMapOffsetPosX, fMapOffsetPosY;

	iCheckTile = 11;
	fPosY += m_fSimulateJumpLength;//fPosY -= (TILE_HEIGHT * iCheckTile);
	for( i = 0; i < iCheckTile - 2; i++ )
	{
		GetNextAttriState(fPosX, fPosY, uiAttribute);

		fMapOffsetPosX = m_pkCurMapGroup->GetMapPosX(fPosX, fPosY);
		fMapOffsetPosY = m_pkCurMapGroup->GetMapPosY(fPosX, fPosY);
		if( m_pkCurTerrainAttribute->IsGround(uiAttribute, (int)fMapOffsetPosX, (int)fMapOffsetPosY) )
			return true;

		fPosY += TILE_HEIGHT;
	}

	return false;
}

//------------------------------------------------------------------------------------
/*
bool SPGOBCoordPhysics::IsMoveJumpable(float fPosX, float fPosY, FACING eFacing)
{
	if( !m_pkCurStage || m_fJumpSpeed >= 0.0f )
		return false;

	float fNextPosX, fNextPosY, fVelocityX, fVelocityY, fAccelY;
	float fMapOffsetPosX, fMapOffsetPosY;
	float fSimulateElapsedTime;
	int iType;
	unsigned int uiAttribute;

	fVelocityY = m_fJumpSpeed;
	fAccelY = m_fGravity;

	if( eFacing == FACING_LEFT )
		fVelocityX = -m_fMaxVelocityX;
	else
		fVelocityX = m_fMaxVelocityX;

	fNextPosX = fPosX;
	fNextPosY = fPosY;

	// min 10frame/sec
	fSimulateElapsedTime = 0.1f;
	while( fVelocityY < 0.0f )
	{
		fVelocityY += (fAccelY * fSimulateElapsedTime);
		GetNextPos(fNextPosX, fNextPosY,
			fVelocityX, fVelocityY);
	}

	int iForceExitCount = 0;
	float fCheckPosY = 0.0f;
	fVelocityY = 0.0f;
	while( TRUE )
	{
		fVelocityY += (fAccelY * fSimulateElapsedTime);

		GetNextPos(fNextPosX, fNextPosY,
			fVelocityX, fVelocityY);
		iType = GetNextAttriState(fNextPosX, fNextPosY, uiAttribute);

		if( fNextPosY > fPosY )
			return false;

		// test block success
		if( iType == 1 )
			return true;

		if( iType == 0 )
			break;

		int iDiff = (int)fabs(fCheckPosY - fNextPosY);
		if( iDiff < 1 )
			return false;

		if( fCheckPosY != fNextPosY )
			fCheckPosY = fNextPosY;

		++iForceExitCount;
		if( iForceExitCount > 25 )
			return false;
	}

	fMapOffsetPosX = m_pkCurMapGroup->GetMapPosX(fNextPosX, fNextPosY);
	fMapOffsetPosY = m_pkCurMapGroup->GetMapPosY(fNextPosX, fNextPosY);

	if( !m_pkCurTerrainAttribute->IsGround(uiAttribute, fMapOffsetPosX, fMapOffsetPosY) )
		return false;

	return true;
}
*/
bool SPGOBCoordPhysics::IsMoveJumpable(float fPosX, float fPosY, FACING eFacing)
{
	if( !m_pkCurStage || !m_pkCurTerrainAttribute || m_fJumpSpeed >= 0.0f )
		return false;

	float fNextPosX, fNextPosY, fVelocityX, fVelocityY, fAccelY;
	float fMapOffsetPosX, fMapOffsetPosY;
	float fSimulateElapsedTime;
	int iType;
	unsigned int uiAttribute;

	fVelocityY = m_fJumpSpeed;
	fAccelY = m_fGravity;

	if( eFacing == FACING_LEFT )
		fVelocityX = -m_fMaxVelocityX;
	else
		fVelocityX = m_fMaxVelocityX;

	fNextPosX = fPosX + fVelocityX * m_fMaxJumpPosTime;
	fNextPosY = fPosY + m_fSimulateJumpLength;

	// min 10frame/sec
	fSimulateElapsedTime = 0.1f;

	int iForceExitCount = 0;
	float fCheckPosY = 0.0f;
	float fMaxDropDistance = fPosY - m_fSimulateJumpLength;
	fVelocityY = 0.0f;
	while( TRUE )
	{
		fVelocityY += (fAccelY * fSimulateElapsedTime);

		GetNextPos(fNextPosX, fNextPosY,
			fVelocityX, fVelocityY);
		iType = GetNextAttriState(fNextPosX, fNextPosY, uiAttribute);

		if( fNextPosY >= fMaxDropDistance )
			return false;

		// test block success
		if( iType == 1 )
			return true;

		if( iType == 0 )
			break;

		int iDiff = (int)fabs(fCheckPosY - fNextPosY);
		if( iDiff < 1 )
			return false;

		if( fCheckPosY != fNextPosY )
			fCheckPosY = fNextPosY;

		++iForceExitCount;
		if( iForceExitCount > 25 )
			return false;
	}

	fMapOffsetPosX = m_pkCurMapGroup->GetMapPosX(fNextPosX, fNextPosY);
	fMapOffsetPosY = m_pkCurMapGroup->GetMapPosY(fNextPosX, fNextPosY);

	if( !m_pkCurTerrainAttribute->IsGround(uiAttribute, (int)fMapOffsetPosX, (int)fMapOffsetPosY) )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
bool SPGOBCoordPhysics::IsMoveDrop(float fPosX, float fPosY, FACING eFacing)
{
	if( !m_pkCurStage || !m_pkCurTerrainAttribute )
		return false;

	float fNextPosX, fNextPosY, fVelocityX, fVelocityY, fAccelY, fDiffY;
	float fMapOffsetPosX, fMapOffsetPosY;
	float fSimulateElapsedTime;
	float fDropMaxDistance;
	
	int iType;
	unsigned int uiAttribute;

	fVelocityY = 0.0f;
	fAccelY = m_fGravity;

	if( eFacing == FACING_LEFT )
		fVelocityX = -m_fMaxVelocityX;
	else
		fVelocityX = m_fMaxVelocityX;

	fNextPosX = fPosX;
	fNextPosY = fPosY;

	fSimulateElapsedTime = 0.1f;
	fDropMaxDistance = -m_fSimulateJumpLength;

	int iForceExitCount = 0;

	while( TRUE )
	{
		fVelocityY += (fAccelY * fSimulateElapsedTime);

		GetNextPos(fNextPosX, fNextPosY,
			fVelocityX, fVelocityY);
		iType = GetNextAttriState(fNextPosX, fNextPosY, uiAttribute);

		fDiffY = fNextPosY - fPosY;
		if( fDiffY > fDropMaxDistance )
			return false;

		if( iType == 1 )
			return false;

		if( iType == 0 )
			break;

		++iForceExitCount;
		if( iForceExitCount > 20 )
			return false;
	}

	fMapOffsetPosX = m_pkCurMapGroup->GetMapPosX(fNextPosX, fNextPosY);
	fMapOffsetPosY = m_pkCurMapGroup->GetMapPosY(fNextPosX, fNextPosY);

	if( !m_pkCurTerrainAttribute->IsGround(uiAttribute, (int)fMapOffsetPosX, (int)fMapOffsetPosY) )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
float SPGOBCoordPhysics::GetHeight(float fPosX, float fPosY)
{
	if( m_pkCurStage == NULL )
		return 0.0f;

	unsigned int uiAttribute;
	float fMapOffsetPosX, fMapOffsetPosY;
	int iRevision, iRet;
	BG_ID eStandLayer;

	int iForceExitCount = 0;
	while( 1 )
	{
		iRet = GetNextAttriState(fPosX, fPosY, uiAttribute);
		if( iRet == 1 )
			return 0.0f;
		else if( iRet == 0 )
		{
			SPTerrainAttribute* pkCurTerrainAttribute = m_pkCurStage->GetCurTerrainAttribute(
				fPosX, fPosY, fMapOffsetPosX, fMapOffsetPosY, m_iCurMapGroupID);

			if( pkCurTerrainAttribute == NULL )
				return 0.0f;

			uiAttribute = pkCurTerrainAttribute->InspectStandLayer(
				(int)fMapOffsetPosX, (int)fMapOffsetPosY, 
				eStandLayer, iRevision);

			if( pkCurTerrainAttribute->IsGround(uiAttribute, (int)fMapOffsetPosX, (int)fMapOffsetPosY) )
				break;
		}

		fPosY += TILE_HEIGHT;

		++iForceExitCount;
		if( iForceExitCount > 30 )
			return 0.0f;
	}

	return fPosY;
}

//------------------------------------------------------------------------------------
unsigned char SPGOBCoordPhysics::IsFacingChange(UINT64 uiAction)
{
	if( m_eCurState == GOB_STATE_JUMP ||
		m_eCurState == GOB_STATE_DROP )
	{
		if( uiAction & ACTION_MOVE_LEFT || uiAction & ACTION_MOVE_RIGHT ||
			uiAction & ACTION_RUN_LEFT || uiAction & ACTION_RUN_RIGHT )
		{
			if( uiAction & ACTION_MOVE_LEFT || uiAction & ACTION_RUN_LEFT )
			{
				if( m_eFacing == FACING_RIGHT )
					return 1;
			}
			else
			{
				if( m_eFacing == FACING_LEFT )
					return 2;
			}
		}
	}

	return 0;
}

//------------------------------------------------------------------------------------
// State Func
void SPGOBCoordPhysics::OnStateIdle()
{
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateStop()
{
	CalVelocity(ACTION_STOP);
	
	InspectStandLayer();
	if( CheckDrop() )
	{
		UINT64 uiAction = ACTION_MOVE_DROP;
		SendActionCommand((LPARAM)&uiAction);
	}
	else
		StateChange(GOB_STATE_IDLE);

	m_bAutoHanging = true;
	m_bJumpAttack = true;
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateVerticalStand()
{

}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateHangingStand()
{

}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateWalk()
{
	InspectStandLayer();
	if( CheckDrop() )
	{
		UINT64 uiAction = ACTION_MOVE_DROP;
		SendActionCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateDrop()
{
	InspectStandLayer(false);
	if( !CheckDrop() )
	{
		CalVelocity(ACTION_STOP);
		InspectStandLayer();

		UINT64 uiAction = ACTION_MOVE_DROP | ACTION_MOVE_UP | ACTION_MOVE_DOWN
			| ACTION_MOVE_HANGING_LEFT | ACTION_MOVE_HANGING_RIGHT | ACTION_HANGING_STOP;

		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateJump()
{
	InspectStandLayer(false);
	if( m_fCurVelocityY >= 0.0f )
	{
		if( m_fPosY > m_fMaxJumpPosY )
			m_fPosY = m_fMaxJumpPosY;

		UINT64 uiAction = ACTION_MOVE_JUMP;
		SendActionCompleteCommand((LPARAM)&uiAction);

		uiAction = ACTION_MOVE_DROP;
		SendActionCommand((LPARAM)&uiAction);

		m_bAutoHanging = true;
		m_fMaxJumpPosY = 0.0f;
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateMoveUp()
{
	InspectStandLayer(false);

	bool bComplete = false;
	if( m_cMoveVerType == 0 )
	{
		if( !EnableLadder(true) )
			bComplete = true;
	}
	else
	{
		if( !EnableVerRope(true) )
			bComplete = true;
	}

	if( bComplete )
	{
		UINT64 uiAction = ACTION_MOVE_UP;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateMoveDown()
{
	InspectStandLayer(false);
	
	bool bComplete = false;
	bool bDrop = false;
	if( m_cMoveVerType == 0 )
	{
		if( !EnableLadder(false) )
			bComplete = true;
	}
	else
	{
		if( !EnableVerRope(false) )
			bComplete = true;
	}

	bDrop = CheckDrop();
	if( bComplete )
	{
		UINT64 uiAction = ACTION_MOVE_DOWN;
		SendActionCompleteCommand((LPARAM)&uiAction);
		if( bDrop )
		{
			uiAction = ACTION_MOVE_DROP;
			SendActionCommand((LPARAM)&uiAction);
		}
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateHanging()
{
	InspectStandLayer(false);
	bool bComplete = false;

	if( !EnableHozRope() )
		bComplete = true;

	m_fAutoHangingPosY = m_fPosY;
	if( bComplete )
	{
		UINT64 uiAction = ACTION_MOVE_HANGING_LEFT | ACTION_MOVE_HANGING_RIGHT;
		SendActionCompleteCommand((LPARAM)&uiAction);

		uiAction = ACTION_MOVE_DROP;
		SendActionCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateAttackSpeedY()
{
	if( m_fActionTime <= 0.0f )
	{
		UINT64 uiAction = ACTION_ATTACK_SPEEDY;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateAttackMighty()
{
	if( m_fActionTime <= 0.0f )
	{
		UINT64 uiAction = ACTION_ATTACK_MIGHTY;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateAttacked()
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

			ZeroMemory(&m_KnockBackInfo, sizeof(KNOCKBACK_INFO));
		}
	}

	if( m_fActionTime <= 0.0f && !m_KnockBackInfo.bKnockBack )
	{
		UINT64 uiAction = ACTION_BEATTACKED;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateDead()
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

			ZeroMemory(&m_KnockBackInfo, sizeof(KNOCKBACK_INFO));
		}
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateSkill()
{
	if( m_fActionTime <= 0.0f )
	{
		UINT64 uiAction = ACTION_SKILL;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateSitDown()
{
	if( m_fActionTime <= 0.0f )
	{
		UINT64 uiAction = 0xFFFFFFFFFFFFFFFF;
		SendActionCompleteCommand((LPARAM)&uiAction);

		uiAction = ACTION_SIT_WAIT;
		SendActionCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateSitDownWait()
{
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateSitDownStand()
{
	if( m_fActionTime <= 0.0f )
	{
		UINT64 uiAction = ACTION_SIT_STAND;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateRun()
{
	InspectStandLayer();
	if( CheckDrop() )
	{
		UINT64 uiAction = ACTION_MOVE_DROP;
		SendActionCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::OnStateGuardDamage()
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

			ZeroMemory(&m_KnockBackInfo, sizeof(KNOCKBACK_INFO));
		}
	}

	if( m_fActionTime <= 0.0f && !m_KnockBackInfo.bKnockBack )
	{
		UINT64 uiAction = ACTION_GUARD_DAMAGE;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPGOBCoordPhysics::StateProcess()
{
	switch( m_eCurState )
	{
	//case GOB_STATE_IDLE:			OnStateIdle();			return;
	case GOB_STATE_STAND:			OnStateStop();			return;
	case GOB_STATE_VERTICAL_STAND:	OnStateVerticalStand();	return;
	case GOB_STATE_HANGING_STAND:	OnStateHangingStand();	return;
	case GOB_STATE_WALK:			OnStateWalk();			return;
	case GOB_STATE_DROP:			OnStateDrop();			return;
	case GOB_STATE_JUMP:			OnStateJump();			return;
	case GOB_STATE_UP:				OnStateMoveUp();		return;
	case GOB_STATE_DOWN:			OnStateMoveDown();		return;
	case GOB_STATE_HANGING:			OnStateHanging();		return;
	case GOB_STATE_ATTACK_SPEEDY:	OnStateAttackSpeedY();	return;
	case GOB_STATE_ATTACK_MIGHTY:	OnStateAttackMighty();	return;
	case GOB_STATE_ATTACKED:		OnStateAttacked();		return;
	case GOB_STATE_DEAD:			OnStateDead();			return;
	case GOB_STATE_SKILL:			OnStateSkill();			return;
	case GOB_STATE_SIT_DOWN:		OnStateSitDown();		return;
	case GOB_STATE_SIT_WAIT:		OnStateSitDownWait();	return;
	case GOB_STATE_SIT_STAND:		OnStateSitDownStand();	return;
	case GOB_STATE_RUN:				OnStateRun();			return;
	case GOB_STATE_GUARD_DAMAGE:	OnStateGuardDamage();	return;
	}
}

//------------------------------------------------------------------------------------
void  SPGOBCoordPhysics::GetLTPosition(LT_POSITION * lpPos)
{
	if ( lpPos )
	{
		lpPos->fPosX = GetPosX();
		lpPos->fPosY = GetPosY();
		lpPos->fVelocityX = GetCurVelocityX();
		lpPos->fVelocityY = GetCurVelocityY();
		lpPos->fAccelX = GetAccelX();
		lpPos->fAccelY = GetAccelY();
		lpPos->eFacing = GetDirection();
		lpPos->bgStandLayer = GetStandLayer();

		lpPos->bKnockBack = m_KnockBackInfo.bKnockBack;
		lpPos->bDelay = m_KnockBackInfo.bDelay;
		lpPos->fKnockTime = m_KnockBackInfo.fKnockBackTime;
		lpPos->fDelayTime = m_KnockBackInfo.fDelayTime;
		lpPos->fKnockVelocityX = m_KnockBackInfo.fKnockBackVelocityX;
		lpPos->fKnockVelocityY = m_KnockBackInfo.fKnockBackVelocityY;
	}
}