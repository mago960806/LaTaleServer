
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
#include "SPPlayerCoordPhysics.h"

#include "SPGOBClusterDEF.h"
#include "SPAvatarModelUnitDef.h"

#include "SPUtil.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayerEquipDefine.h" // TEST Equip
#include "SPPlayer.h"

//------------------------------------------------------------------------------------
#ifdef _LATALE_CLIENT
SPPlayerCoordPhysics::SPPlayerCoordPhysics(SPGameObject* pkParent)
#else
SPPlayerCoordPhysics::SPPlayerCoordPhysics(CCharacter* pkParent)
#endif
: SPGOBCoordPhysics(pkParent)
{
	Init();
}

//------------------------------------------------------------------------------------
SPPlayerCoordPhysics::~SPPlayerCoordPhysics()
{	
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::Init()
{
	SPGOBCoordPhysics::Init();
	m_bReserveHangingStop = false;
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::Process(float fElapsedTime)
{	
	SPGOBCoordPhysics::Process(fElapsedTime);
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::CalVelocity(UINT64 uiAction)
{
	SPGOBCoordPhysics::CalVelocity(uiAction);
	
	//m_fAccelX = 0.0f;

	if( (uiAction & ACTION_MOVE_LEFT) &&
		!((uiAction & ACTION_MOVE_UP) || (uiAction & ACTION_MOVE_DOWN)) )
	{
		if( m_fCurVelocityX >= 0.0f )
			SetDirection(FACING_LEFT);
		
		m_fCurVelocityX = -m_fMaxVelocityX;
	}
	else if( (uiAction & ACTION_MOVE_RIGHT) &&
		!((uiAction & ACTION_MOVE_UP) || (uiAction & ACTION_MOVE_DOWN)) )
	{
		if( m_fCurVelocityX <= 0.0f )
			SetDirection(FACING_RIGHT);
		
		m_fCurVelocityX = m_fMaxVelocityX;
		
	}
	
	if( (uiAction & ACTION_MOVE_HANGING_LEFT) &&
		!((uiAction & ACTION_MOVE_UP) || (uiAction & ACTION_MOVE_DOWN)) )
	{
		if( m_fCurVelocityX >= 0.0f )
			SetDirection(FACING_LEFT);

		m_fCurVelocityX = -m_fMaxHangingVelocity;
		
		if( !(uiAction & ACTION_MOVE_JUMP) &&
			!(uiAction & ACTION_MOVE_DROP) )
		{
			m_fCurVelocityY = 0.0f;
			m_fAccelY = 0.0f;
		}
	}
	else if( (uiAction & ACTION_MOVE_HANGING_RIGHT) &&
		!((uiAction & ACTION_MOVE_UP) || (uiAction & ACTION_MOVE_DOWN)) )
	{
		if( m_fCurVelocityX <= 0.0f )
			SetDirection(FACING_RIGHT);

		m_fCurVelocityX = m_fMaxHangingVelocity;

		if( !(uiAction & ACTION_MOVE_JUMP) &&
			!(uiAction & ACTION_MOVE_DROP) )
		{
			m_fCurVelocityY = 0.0f;
			m_fAccelY = 0.0f;
		}
	}

	if( (uiAction & ACTION_RUN_LEFT) &&
		!((uiAction & ACTION_MOVE_UP) || (uiAction & ACTION_MOVE_DOWN)) )
	{
		if( m_fCurVelocityX >= 0.0f )
			SetDirection(FACING_LEFT);

		m_fCurVelocityX = -m_fMaxVelocityX;
		
		//[xialin 2008/09/01] add
		//SPPlayer * pkPlayer = dynamic_cast<SPPlayer *>(m_pkParent);
		//if ( pkPlayer && pkPlayer->GetLobbyAvatar() == LOBBY_CASH )
		//	m_fCurVelocityX *= 2;
		if ( m_pkParent && m_pkParent->GetClassID() == CLASS_ID_AVATAR
			&& ((SPPlayer*)m_pkParent)->GetLobbyAvatar() == LOBBY_CASH )
			m_fCurVelocityX *= 2;
		
	}
	else if( (uiAction & ACTION_RUN_RIGHT) &&
		!((uiAction & ACTION_MOVE_UP) || (uiAction & ACTION_MOVE_DOWN)) )
	{
		if( m_fCurVelocityX <= 0.0f )
			SetDirection(FACING_RIGHT);

		m_fCurVelocityX = m_fMaxVelocityX;

		//[xialin 2008/09/01] add
		//SPPlayer * pkPlayer = dynamic_cast<SPPlayer *>(m_pkParent);
		//if ( pkPlayer && pkPlayer->GetLobbyAvatar() == LOBBY_CASH )
		//	m_fCurVelocityX *= 2;
		if ( m_pkParent && m_pkParent->GetClassID() == CLASS_ID_AVATAR
			&& ((SPPlayer*)m_pkParent)->GetLobbyAvatar() == LOBBY_CASH )
			m_fCurVelocityX *= 2;
	}

	if( (uiAction & ACTION_LOOTING) ||
		(uiAction & ACTION_SIT_DOWN) ||
		(uiAction & ACTION_RUN_STOP) ||
		(uiAction & ACTION_MOVE_LAND) ||
		(uiAction & ACTION_BEATTACKED_DOWN) ||
		(uiAction & ACTION_BEATTACKED_DOWN_STAND) ||
		(uiAction & ACTION_GUARD_DAMAGE) ||
		(uiAction & ACTION_HANGING_STOP) ||
		(uiAction & ACTION_ENCHANTTING) ||
		(uiAction & ACTION_ENCHANT_RESULT) )
	{
		m_fCurVelocityX = 0.0f;
		m_fAccelX = 0.0f;

		if( uiAction & ACTION_BEATTACKED_DOWN_STAND ||
			uiAction & ACTION_ENCHANT_RESULT )
		{
			m_fCurVelocityY = 0.0f;
			m_fAccelY = 0.0f;
			ZeroMemory(&m_KnockBackInfo, sizeof(KNOCKBACK_INFO));
			ZeroMemory(&m_ActivityVelocityInfo, sizeof(ACTIVE_VEL_INFO));
		}
	}

	if( (uiAction & ACTION_ATTACK_SPEEDY) ||
		(uiAction & ACTION_ATTACK_MIGHTY) )
	{
		if( m_eCurState != GOB_STATE_JUMP &&
			m_eCurState != GOB_STATE_DROP )
		{
			m_fCurVelocityX = 0.0f;
			m_fAccelX = 0.0f;
		}
	}

	if( (uiAction & ACTION_HANGING_STOP) ||
		(uiAction & ACTION_MOVE_HANGING_LEFT) ||
		(uiAction & ACTION_MOVE_HANGING_RIGHT) )
	{
		// Y값 보정
		float fMapOffsetPosY;
		int iDistY;
		
		if( m_pkCurMapGroup )
		{
			fMapOffsetPosY = m_pkCurMapGroup->GetMapPosY(m_fPosX, m_fPosY);
			iDistY = (int)fMapOffsetPosY % TILE_HEIGHT;

			m_fPosY += -iDistY;
		}
	}
}

//------------------------------------------------------------------------------------
bool SPPlayerCoordPhysics::IsActionChange(UINT64& uiAction)
{
	bool bRet;
	bRet = SPGOBCoordPhysics::IsActionChange(uiAction);

	if( uiAction & ACTION_SKILL )
		return true;
	
	bool bMoveVer, bEnableMove;

	bMoveVer = false;
	bEnableMove = false;
	bool bUp = true;

	if( uiAction & ACTION_MOVE_UP )
	{
		bMoveVer = true;
		bUp = true;
	}
	else if( uiAction & ACTION_MOVE_DOWN )
	{
		bMoveVer = true;
		bUp = false;
	}

	if( bMoveVer || m_cMoveVerType < 0 )
	{
		bEnableMove = EnableLadder(bUp);
		m_cMoveVerType = 0;
		
		if( !bEnableMove )
		{
			bEnableMove = EnableVerRope(bUp);
			if( bEnableMove )
				m_cMoveVerType = 1;
		}
	}

	//
	if( uiAction & ACTION_MOVE_JUMP )
		SubAction(uiAction, ACTION_SIT_DOWN | ACTION_LOOTING);

	if( (uiAction & ACTION_ATTACK_SPEEDY) || (uiAction & ACTION_ATTACK_MIGHTY) )
	{
		SubAction(uiAction, ACTION_MOVE_UP | ACTION_MOVE_DOWN | ACTION_MOVE_JUMP |
			ACTION_SIT_DOWN | ACTION_LOOTING);
	}

	if( bMoveVer && bEnableMove )
		SubAction(uiAction, ACTION_SIT_DOWN | ACTION_LOOTING);

	if( uiAction & ACTION_USERSHOP )
		bRet = false;

	switch( m_eCurState )
	{
	case GOB_STATE_NULL:
	case GOB_STATE_IDLE:
	case GOB_STATE_STAND:
		{
			if( bMoveVer && !bEnableMove )
			{
				bRet = false;
				
				if( uiAction & ACTION_MOVE_DOWN )
				{
					uiAction = ACTION_GUARD;
					bRet = true;
				}
			}

			if( uiAction & ACTION_MOVE_JUMP )
				SubAction(uiAction, ACTION_ATTACK_SPEEDY | ACTION_ATTACK_MIGHTY);

			if( uiAction & ACTION_USERSHOP )
				bRet = true;
		}
		break;

	case GOB_STATE_LOOTING:
		{
#if defined(_LATALE_SERVER)
			bRet = true;
#else
			bRet = false;
			if( uiAction & ACTION_LOOTING_STAND )
				bRet = true;
#endif
		}
		break;

	case GOB_STATE_WALK:
		{
			if( bMoveVer && !bEnableMove )
			{
				if( uiAction & ACTION_MOVE_DOWN )
					uiAction = ACTION_GUARD;
				else
					SubAction(uiAction, ACTION_MOVE_UP | ACTION_MOVE_DOWN);
			}

			if( (uiAction & ACTION_ATTACK_SPEEDY) ||
				(uiAction & ACTION_ATTACK_MIGHTY) )
				SubAction(uiAction, ACTION_MOVE_LEFT | ACTION_MOVE_RIGHT);
			
			if( uiAction & ACTION_MOVE_JUMP )
				SubAction(uiAction, ACTION_ATTACK_SPEEDY | ACTION_ATTACK_MIGHTY);
		}
		break;

	case GOB_STATE_RUN:
		{
			if( bMoveVer && !bEnableMove )
				SubAction(uiAction, ACTION_MOVE_UP | ACTION_MOVE_DOWN);
			
			if( (uiAction & ACTION_ATTACK_SPEEDY) ||
				(uiAction & ACTION_ATTACK_MIGHTY) )
				SubAction(uiAction, ACTION_RUN_LEFT | ACTION_RUN_RIGHT);
			
			//if( uiAction == ACTION_STOP )
			//	uiAction = ACTION_RUN_STOP;

			if( uiAction & ACTION_MOVE_JUMP )
				SubAction(uiAction, ACTION_ATTACK_SPEEDY | ACTION_ATTACK_MIGHTY);
		}
		break;

	case GOB_STATE_DROP:
		{
			bRet = false;
			if( bMoveVer && bEnableMove )
			{
				SubAction(uiAction, ACTION_MOVE_JUMP);
				bRet = true;
			}
			else
			{
				if( m_bJumpAttack )
				{
					if( uiAction & ACTION_ATTACK_SPEEDY ||
						uiAction & ACTION_ATTACK_MIGHTY )
					{
						bRet = true;
						m_bJumpAttack = false;

						SubAction(uiAction, ACTION_MOVE_UP | ACTION_MOVE_DOWN |
							ACTION_MOVE_LEFT | ACTION_MOVE_RIGHT |
							ACTION_RUN_LEFT | ACTION_RUN_RIGHT);
					}
				}
			}

			if( uiAction & ACTION_MOVE_HANGING_LEFT ||
				uiAction & ACTION_MOVE_HANGING_RIGHT )
			{
				if( EnableHozRope() )
					bRet = true;
			}

#if defined(_LATALE_SERVER)
			if( uiAction & ACTION_HANGING_STOP )
				m_bReserveHangingStop = true;
#else
			if( uiAction & ACTION_HANGING_STOP )
				bRet = true;
#endif
		}
		break;

	case GOB_STATE_JUMP:
		{
			bRet = false;
			if( bMoveVer && bEnableMove )
			{
				SubAction(uiAction, ACTION_MOVE_JUMP);
				bRet = true;
			}
			else
			{
				if( m_bJumpAttack )
				{
					if( uiAction & ACTION_ATTACK_SPEEDY ||
						uiAction & ACTION_ATTACK_MIGHTY )
					{
						bRet = true;
						m_bJumpAttack = false;

						SubAction(uiAction, ACTION_MOVE_UP | ACTION_MOVE_DOWN |
							ACTION_MOVE_LEFT | ACTION_MOVE_RIGHT |
							ACTION_RUN_LEFT | ACTION_RUN_RIGHT);
					}
				}
			}
		}
		break;

	case GOB_STATE_VERTICAL_STAND:
	case GOB_STATE_UP:
	case GOB_STATE_DOWN:
		{
			bRet = false;
			SubAction(uiAction, ACTION_ATTACK_SPEEDY | ACTION_ATTACK_MIGHTY |
				ACTION_SIT_DOWN | ACTION_LOOTING);
			
			if( uiAction & ACTION_MOVE_JUMP )
			{
				if( (uiAction & ACTION_MOVE_LEFT) || (uiAction & ACTION_MOVE_RIGHT) ||
					(uiAction & ACTION_RUN_LEFT) || (uiAction & ACTION_RUN_RIGHT) )
				{
					if( !(uiAction & ACTION_MOVE_UP) &&
						!(uiAction & ACTION_MOVE_DOWN) )
					{
						if( uiAction & ACTION_RUN_LEFT )
						{
							SubAction(uiAction, ACTION_RUN_LEFT);
							AddAction(uiAction, ACTION_MOVE_LEFT);
						}
						else if( uiAction & ACTION_RUN_RIGHT )
						{
							SubAction(uiAction, ACTION_RUN_RIGHT);
							AddAction(uiAction, ACTION_MOVE_RIGHT);
						}

						bRet = true;
					}
					else
					{
						SubAction(uiAction, ACTION_MOVE_JUMP |
							ACTION_MOVE_LEFT | ACTION_MOVE_RIGHT |
							ACTION_RUN_LEFT | ACTION_RUN_RIGHT );
					}
				}
				else
					SubAction(uiAction, ACTION_MOVE_JUMP);
			}
			else
			{
				SubAction(uiAction, ACTION_MOVE_LEFT | ACTION_MOVE_RIGHT |
					ACTION_RUN_LEFT | ACTION_RUN_RIGHT );
			}

			if( (uiAction & ACTION_MOVE_UP) ||
				(uiAction & ACTION_MOVE_DOWN) )
				bRet = true;

			if( uiAction == ACTION_STOP )
				uiAction = ACTION_VERTICAL_STOP;
			if( uiAction == ACTION_VERTICAL_STOP )
				bRet = true;
		}
		break;

	case GOB_STATE_HANGING_STAND:
	case GOB_STATE_HANGING:
		{
			bRet = false;
			SubAction(uiAction, ACTION_MOVE_UP | ACTION_SIT_DOWN |
				ACTION_ATTACK_SPEEDY | ACTION_ATTACK_MIGHTY);

			if( (uiAction & ACTION_MOVE_HANGING_LEFT) ||
				(uiAction & ACTION_MOVE_HANGING_RIGHT) ||
				(uiAction & ACTION_MOVE_JUMP) )
				bRet = true;

			if( uiAction & ACTION_MOVE_DOWN )
			{
				if( uiAction & ACTION_MOVE_HANGING_LEFT )
				{
					SubAction(uiAction, ACTION_MOVE_HANGING_LEFT);
					AddAction(uiAction, ACTION_MOVE_LEFT);
				}

				if( uiAction & ACTION_MOVE_HANGING_RIGHT )
				{
					SubAction(uiAction, ACTION_MOVE_HANGING_RIGHT);
					AddAction(uiAction, ACTION_MOVE_RIGHT);
				}

				AddAction(uiAction, ACTION_MOVE_DROP);

				m_bAutoHanging = false;
				bRet = true;
			}

			if( uiAction == ACTION_STOP )
				uiAction = ACTION_HANGING_STOP;
			if( uiAction == ACTION_HANGING_STOP )
				bRet = true;
		}
		break;

	case GOB_STATE_LOOTING_WAIT:
		{
			bRet = false;
			
			SubAction(uiAction, ACTION_MOVE_UP | ACTION_MOVE_DOWN);
			if( uiAction & ACTION_MOVE_LEFT ||
				uiAction & ACTION_MOVE_RIGHT ||
				uiAction & ACTION_LOOTING_STAND )
			{
				bRet = true;
			}
		}
		break;

	case GOB_STATE_SIT_DOWN:
		{
			bRet = false;
			if( uiAction & ACTION_SIT_WAIT )
				bRet = true;
		}
		break;
	
	case GOB_STATE_SIT_WAIT:
		{
			bRet = false;
			if( uiAction & ACTION_SIT_WAIT || 
				uiAction & ACTION_SIT_STAND)
				bRet = true;
		}
		break;

	case GOB_STATE_GUARD:
		{
			uiAction &= ~ACTION_MOVE_UP;
			uiAction &= ~ACTION_MOVE_JUMP;
			uiAction &= ~ACTION_LOOTING;
			//if( uiAction & ACTION_BEATTACKED ||
			//	uiAction & ACTION_CRITICAL_BEATTACKED ||
			//	uiAction & ACTION_MOVE_JUMP )
			if( uiAction & ACTION_BEATTACKED ||
				uiAction & ACTION_CRITICAL_BEATTACKED )
				bRet = false;
		}
		break;
	
	case GOB_STATE_LOOTING_STAND:
	case GOB_STATE_SIT_STAND:
	case GOB_STATE_RUN_STOP:
	case GOB_STATE_LAND:
	case GOB_STATE_ATTACKED_DOWN:
	case GOB_STATE_ATTACKED_DOWN_STAND:
	case GOB_STATE_GUARD_DAMAGE:
	case GOB_STATE_SKILL:
	case GOB_STATE_BLOCK:
	case GOB_STATE_ENCHANT_RESULT:
	case GOB_STATE_USERSHOP:
		bRet = false;
		break;

	case GOB_STATE_ATTACK_SPEEDY:
	case GOB_STATE_ATTACK_MIGHTY:
		{
			if( m_bJumpAttack &&
				(uiAction & ACTION_ATTACK_SPEEDY || uiAction & ACTION_ATTACK_MIGHTY) )
				bRet = true;
			//bRet = false;
		}
		break;

	case GOB_STATE_ENCHANTTING:
		{
#if defined(_LATALE_CLIENT)
			if( uiAction == ACTION_STOP )
				bRet = false;
#endif
		}
		break;
	}

	return bRet;
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::SetAction(UINT64 uiAction)
{
	SPGOBCoordPhysics::SetAction(uiAction);

	if( uiAction & ACTION_LOOTING )
		StateChange(GOB_STATE_LOOTING);
	else if( uiAction & ACTION_LOOTING_STAND )
		StateChange(GOB_STATE_LOOTING_STAND);

	if( uiAction & ACTION_MOVE_LAND )
		StateChange(GOB_STATE_LAND);

	if( uiAction & ACTION_GUARD )
		StateChange(GOB_STATE_GUARD);

	if( uiAction & ACTION_BEATTACKED_DOWN )
		StateChange(GOB_STATE_ATTACKED_DOWN);
	else if( uiAction & ACTION_BEATTACKED_DOWN_STAND )
		StateChange(GOB_STATE_ATTACKED_DOWN_STAND);
	
	if( uiAction & ACTION_BLOCK )
		StateChange(GOB_STATE_BLOCK);
	
	if( uiAction & ACTION_ENCHANTTING )
		StateChange(GOB_STATE_ENCHANTTING);
	else if( uiAction & ACTION_ENCHANT_RESULT )
		StateChange(GOB_STATE_ENCHANT_RESULT);

	if( uiAction & ACTION_USERSHOP )
		StateChange(GOB_STATE_USERSHOP);
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::SetFirstAction()
{
	if( m_pkCurStage == NULL || m_pkCurMapGroup == NULL )
		return;

	//if( m_pkCurTerrainAttribute == NULL )
	//{
		m_pkCurTerrainAttribute = m_pkCurStage->GetCurTerrainAttribute(
			m_fPosX, m_fPosY, m_fMapOffsetPosX, m_fMapOffsetPosY, m_iCurMapGroupID);
		
		if( !m_pkCurTerrainAttribute )
			return;
	//}

	UINT64 uiAction;
	bool bEnableMove = EnableLadder(true);
	if( bEnableMove )
	{
		m_cMoveVerType = 0;
		uiAction = ACTION_VERTICAL_STOP;
		SendActionCommand((LPARAM)&uiAction);
		return;
	}
	else
	{
		bEnableMove = EnableVerRope(true);
		if( bEnableMove )
		{
			m_cMoveVerType = 1;
			uiAction = ACTION_VERTICAL_STOP;
			SendActionCommand((LPARAM)&uiAction);
			return;
		}
	}

	if( EnableHozRope() )
	{
		uiAction = ACTION_HANGING_STOP;
		SendActionCommand((LPARAM)&uiAction);
	}
	else if( CheckDrop() )
	{
		uiAction = ACTION_MOVE_DROP;
		SendActionCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
bool SPPlayerCoordPhysics::CheckDrop()
{
	return SPGOBCoordPhysics::CheckDrop();
}

//------------------------------------------------------------------------------------
bool SPPlayerCoordPhysics::CheckDrop(float fPosX, float fPosY)
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
		fMapOffsetPosX, fMapOffsetPosY, 
		eStandLayer, iRevision);

	bool bDrop = pkCurTerrainAttribute->MoveDrop(fMapOffsetPosX, fMapOffsetPosY, BG_ID_LAYER_NULL);
	if( bDrop )
	{
		if( uiTerAttribute > 0 && m_uiTerAttribute > 0 )
		{
			if( pkCurTerrainAttribute->IsGround((BYTE)uiTerAttribute,
				fMapOffsetPosX, fMapOffsetPosY) )
				bDrop = false;
		}
	}
	
	if( m_bAutoHanging )
	{
		bool bHozRope;
		bHozRope = pkCurTerrainAttribute->EnableHozRopeUp(fMapOffsetPosX, fMapOffsetPosY);
		if( bHozRope )
			bDrop = false;
	}

	return bDrop;
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateLooting()
{
	if( m_fActionTime <= 0.0f )
		StateChange(GOB_STATE_LOOTING_WAIT);
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateLootingWait()
{
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateLootingStand()
{
	if( m_fActionTime < 0.0f )
	{
		UINT64 uiAction = ACTION_LOOTING_STAND;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateSitDown()
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
void SPPlayerCoordPhysics::OnStateSitDownWait()
{
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateSitDownStand()
{
	if( m_fActionTime <= 0.0f )
	{
		UINT64 uiAction = ACTION_SIT_STAND;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateRun()
{
	InspectStandLayer();
	if( CheckDrop() )
	{
		UINT64 uiAction = ACTION_MOVE_DROP;
		SendActionCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateRunStop()
{
	if( m_fActionTime <= 0.0f )
	{
		UINT64 uiAction = ACTION_RUN_STOP;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateDrop()
{
	InspectStandLayer(false);

	UINT64 uiAction;
	if( !CheckDrop() )
	{
		CalVelocity(ACTION_STOP);
		InspectStandLayer();
		
		uiAction = 0xFFFFFFFFFFFFFFFF;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}

	if( m_bAutoHanging )
	{
		if( EnableHozRope() )
		{
			uiAction = 0xFFFFFFFFFFFFFFFF;
			SendActionCompleteCommand((LPARAM)&uiAction);
			
			m_fAutoHangingPosY = m_fPosY;

			if( m_bReserveHangingStop )
			{
				uiAction = ACTION_HANGING_STOP;
				m_bReserveHangingStop = false;
			}
			else
			{
				
				if( m_fCurVelocityX < 0 )
					uiAction = ACTION_MOVE_HANGING_LEFT;
				else if( m_fCurVelocityX > 0 )
					uiAction = ACTION_MOVE_HANGING_RIGHT;
				else
					uiAction = ACTION_HANGING_STOP;
			}

			SendActionCommand((LPARAM)&uiAction);
		}
	}
	else
	{
		//if( m_fPosY < m_fAutoHangingPosY ||
		//	m_fPosY > m_fAutoHangingPosY + TILE_HEIGHT )
		if( m_fPosY > m_fAutoHangingPosY + TILE_HEIGHT )
			m_bAutoHanging = true;
	}
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateAttackSpeedY()
{
	bool bRevisionY = true;
	if( m_fCurVelocityY < 0.0f )
		bRevisionY = false;

	UINT64 uiAction;
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

			uiAction = 0xFFFFFFFFFFFFFFFF;
			SendActionCompleteCommand((LPARAM)&uiAction);

			uiAction = ACTION_MOVE_LAND;
			SendActionCommand((LPARAM)&uiAction);
			return;
		}
	}

	if( m_fActionTime <= 0.0f && !m_ActivityVelocityInfo.bEnable )
	{
		uiAction = 0xFFFFFFFFFFFFFFFF;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateAttackMighty()
{
	bool bRevisionY = true;
	if( m_fCurVelocityY < 0.0f )
		bRevisionY = false;

	UINT64 uiAction;
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

			uiAction = 0xFFFFFFFFFFFFFFFF;
			SendActionCompleteCommand((LPARAM)&uiAction);

			uiAction = ACTION_MOVE_LAND;
			SendActionCommand((LPARAM)&uiAction);
			return;
		}
	}

	if( m_fActionTime <= 0.0f && !m_ActivityVelocityInfo.bEnable )
	{
		uiAction = 0xFFFFFFFFFFFFFFFF;
		SendActionCompleteCommand((LPARAM)&uiAction);
		
	}
}


//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateLand()
{
	if( m_fActionTime <= 0.0f )
	{
		UINT64 uiAction = ACTION_MOVE_LAND;
		SendActionCompleteCommand((LPARAM)&uiAction);
		m_bJumpAttack = true;
	}
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateCriticalAttacked()
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
		UINT64 uiAction = ACTION_CRITICAL_BEATTACKED;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateGuard()
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
void SPPlayerCoordPhysics::OnStateAttackedDown()
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
		UINT64 uiAction = ACTION_BEATTACKED_DOWN;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateAttackedDownStand()
{
	if( m_fActionTime <= 0.0f )
	{
		UINT64 uiAction = ACTION_BEATTACKED_DOWN_STAND;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateSkill()
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

			m_fCurVelocityX = 0.0f;
			m_fAccelX = 0.0f;

			ZeroMemory(&m_ActivityVelocityInfo, sizeof(ACTIVE_VEL_INFO));
		}

		//if( m_pkCurTerrainAttribute && m_pkCurTerrainAttribute->IsHozRope(m_uiTerAttribute) )
		//{
		//	m_fPosY += TILE_HEIGHT;

		//	SendActionCompleteCommand(0xFFFFFFFF);
		//	m_fAutoHangingPosY = m_fPosY;
			
		//	SendActionCommand(ACTION_HANGING_STOP);
		//	m_bReserveHangingStop = false;

		//	ZeroMemory(&m_ActivityVelocityInfo, sizeof(ACTIVE_VEL_INFO));
		//}
	}
	else if( m_fActionTime <= 0.0f && !m_ActivityVelocityInfo.bEnable )
	{
		UINT64 uiAction = 0xFFFFFFFFFFFFFFFF;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateBlock()
{
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::OnStateEnchantResult()
{
	if( m_fActionTime <= 0.0f )
	{
		UINT64 uiAction = ACTION_ENCHANT_RESULT;
		SendActionCompleteCommand((LPARAM)&uiAction);
	}
}

//------------------------------------------------------------------------------------
void SPPlayerCoordPhysics::StateProcess()
{
	switch( m_eCurState )
	{
	case GOB_STATE_LOOTING:				OnStateLooting();			return;
	case GOB_STATE_LOOTING_WAIT:		OnStateLootingWait();		return;
	case GOB_STATE_LOOTING_STAND:		OnStateLootingStand();		return;
	case GOB_STATE_RUN:					OnStateRun();				return;
	case GOB_STATE_RUN_STOP:			OnStateRunStop();			return;
	case GOB_STATE_LAND:				OnStateLand();				return;
	case GOB_STATE_GUARD:				OnStateGuard();				return;
	case GOB_STATE_GUARD_DAMAGE:		OnStateGuardDamage();		return;
	case GOB_STATE_SKILL:				OnStateSkill();				return;
	case GOB_STATE_BLOCK:				OnStateBlock();				return;
	case GOB_STATE_CRITICAL_ATTACKED:	OnStateCriticalAttacked();	return;
	case GOB_STATE_ATTACKED_DOWN:		OnStateAttackedDown();		return;
	case GOB_STATE_ATTACKED_DOWN_STAND:	OnStateAttackedDownStand();	return;
	case GOB_STATE_ENCHANT_RESULT:		OnStateEnchantResult();		return;
	}

	SPGOBCoordPhysics::StateProcess();
}















