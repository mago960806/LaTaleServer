
#include "SPCommon.h"
#include "SPUtil.h"
#include "SPMainGameDEF.H"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"
#include "SPCommandConvert.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"

#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXFrameAnimationUnit.h"
#include "SPFXManager.h"
#include "SPFXDamageUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPGOBClusterDef.h"
#include "SPGOBModelUnit.h"
#include "SPGOBCoordPhysics.h"
#include "SPMonsterCoordPhysics.h"

#include "SPPet.h"
#include "SPGroundPet.h"

//------------------------------------------------------------------------------------
SPGroundPet::SPGroundPet()
{
}

//------------------------------------------------------------------------------------
SPGroundPet::SPGroundPet(INSTANCE_ID instanceID, CLASS_ID ClassID, SPGameObject* pkParent)
: SPPet(instanceID, ClassID, pkParent)
{
}

//------------------------------------------------------------------------------------
SPGroundPet::~SPGroundPet()
{
}

//------------------------------------------------------------------------------------
void SPGroundPet::Init()
{
	SPPet::Init();

	m_pkCoordPhysics = new SPMonsterCoordPhysics(this);
	m_pkCoordPhysics->Init();
}

//------------------------------------------------------------------------------------
void SPGroundPet::Process(float fTime)
{
	UINT64 uiParentAction;
	float fPosX, fPosY;

	if( GetExceptionAction(uiParentAction) )
	{
		SPGOBCoordPhysics* pkParentPhysics;

		m_pkParent->SPGOBPerformMessage(SPGM_GETCOORDPHYSICS, (LPARAM)&pkParentPhysics);
		if( pkParentPhysics )
			m_iMoveVerType = pkParentPhysics->GetMoveVerType();
		
		SetAction(uiParentAction);

		fPosX = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSX);
		fPosY = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSY);

		m_pkCoordPhysics->SetPos(fPosX, fPosY);
		m_pkCoordPhysics->SetMoveVerType((char)m_iMoveVerType);
	}
	else
	{
		if( IsExceptionAction(m_uiCurAction) )
		{
			if( uiParentAction == ACTION_STOP )
			{
				fPosX = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSX);
				fPosY = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSY);

				m_pkCoordPhysics->SetPos(fPosX, fPosY);
				SetAction(ACTION_STOP);
			}
			else if( (uiParentAction & ACTION_MOVE_JUMP) ||
				(uiParentAction & ACTION_MOVE_DROP) )
			{
				m_pkCoordPhysics->SetMaxVelocityX(PET_WALK_VELOCITY);
				SetAction(uiParentAction);
			}
			else
				SetAction(ACTION_MOVE_DROP);
		}
	}

	SPPet::Process(fTime);
}

//------------------------------------------------------------------------------------
void SPGroundPet::SetAction(UINT64 uiAction, bool bForce)
{
	if( !m_pkCoordPhysics )
		return;

	if( !bForce && m_uiCurAction == uiAction )
		return;

	m_uiLastAction = m_uiCurAction;
	m_uiCurAction = uiAction;

	m_pkCoordPhysics->SetAction(uiAction);

	if( m_uiCurAction == ACTION_STOP )
		SetState(GOB_STATE_STAND, bForce);
	else if( m_uiCurAction == ACTION_VERTICAL_STOP )
		SetState(GOB_STATE_VERTICAL_STAND, bForce);
	else if( m_uiCurAction == ACTION_HANGING_STOP )
		SetState(GOB_STATE_HANGING_STAND, bForce);
	else
	{
		if( m_uiCurAction & ACTION_MOVE_LEFT ||
			m_uiCurAction & ACTION_MOVE_RIGHT )
			SetState(GOB_STATE_WALK, bForce);

		if( m_uiCurAction == ACTION_RUN_LEFT ||
			m_uiCurAction == ACTION_RUN_RIGHT )
			SetState(GOB_STATE_RUN, bForce);

		if( m_uiCurAction & ACTION_MOVE_HANGING_LEFT ||
			m_uiCurAction & ACTION_MOVE_HANGING_RIGHT )
			SetState(GOB_STATE_HANGING, bForce);

		if( m_uiCurAction & ACTION_MOVE_UP )
			SetState(GOB_STATE_UP, bForce);
		else if( m_uiCurAction & ACTION_MOVE_DOWN )
			SetState(GOB_STATE_DOWN, bForce);

		if( m_uiCurAction & ACTION_MOVE_JUMP )
			SetState(GOB_STATE_JUMP, bForce);
		else if( m_uiCurAction & ACTION_MOVE_DROP )
			SetState(GOB_STATE_DROP, bForce);

		if( m_uiCurAction & ACTION_SKILL )
			SetState(GOB_STATE_SKILL, bForce);
	}

	SPPet::SetAction(uiAction, bForce);
}

//------------------------------------------------------------------------------------
void SPGroundPet::SetLootingFX(int iID)
{
	if( m_pkFXArchive == NULL )
		return;

	if( iID == 0 )
		iID = 15038;

	m_pkFXArchive->InsertFXGroup((SPID_FX_GROUP)iID);
}

//------------------------------------------------------------------------------------
void SPGroundPet::SetState(GOB_STATE_TYPE state, bool bForce)
{
	if( !bForce && m_eCurState == state )
		return;

	m_eLastState = m_eCurState;
	m_eCurState = state;

	//if( (m_eLastState == GOB_STATE_WALK || m_eLastState == GOB_STATE_RUN) &&
	//	(m_eCurState == GOB_STATE_WALK || m_eCurState == GOB_STATE_RUN) )
	//	return;

	ANIMATION_INDEX	eAniIndex	=	ANI_NULL;
	if( m_pkModelUnit )
		m_pkModelUnit->SPSendMessage(MV_SET_ACCUMUL_ACTION, 0);

	switch( m_eCurState )
	{
	case GOB_STATE_STAND:
		eAniIndex	=	ANI_MONSTER_STAND;
		break;

	case GOB_STATE_WALK:
	case GOB_STATE_RUN:
		eAniIndex	=	ANI_MONSTER_WALK;
		break;
	
	case GOB_STATE_DROP:
		eAniIndex	=	ANI_MONSTER_FALL;
		break;

	case GOB_STATE_JUMP:
		eAniIndex	=	ANI_MONSTER_JUMP;
		break;

	case GOB_STATE_VERTICAL_STAND:
		if( m_iMoveVerType == 0 )
			eAniIndex = ANI_MONSTER_RESERVE01;
		else
			eAniIndex = ANI_MONSTER_RESERVE02;
		break;

	case GOB_STATE_HANGING_STAND:
		eAniIndex = ANI_MONSTER_RESERVE03;
		break;

	case GOB_STATE_UP:
		if( m_iMoveVerType == 0 )
			eAniIndex = ANI_MONSTER_RESERVE01;
		else
			eAniIndex = ANI_MONSTER_RESERVE02;
		break;

	case GOB_STATE_DOWN:
		if( m_iMoveVerType == 0 )
			eAniIndex = ANI_MONSTER_RESERVE01;
		else
			eAniIndex = ANI_MONSTER_RESERVE02;
		break;

	case GOB_STATE_HANGING:
		eAniIndex = ANI_MONSTER_RESERVE03;
		break;

	case GOB_STATE_SKILL:
		eAniIndex = m_eSkillAnimationIndex;
		break;
	}

	if( eAniIndex != ANI_NULL )
	{
		if( m_pkModelUnit )
			m_pkModelUnit->SPSendMessage(MV_SET_ANIMATION_INDEX, eAniIndex );
		SetAnimationLoop();
	}

	m_eCurAnimationIndex = eAniIndex;
	SPPet::SetState(state, bForce);
}

//------------------------------------------------------------------------------------
int SPGroundPet::CheckMovable(unsigned __int64 uiAction)
{
	if( m_pkCoordPhysics == NULL )
		return 0;

	float fPosX, fPosY, fCurVelocityX;
	unsigned int uiAttribute;

	// 맥스치로 이동하기위해 충분히 큰값설정(10.0f)
	fCurVelocityX = 0.0f;
	if( uiAction & ACTION_MOVE_LEFT )
		fCurVelocityX = m_pkCoordPhysics->GetMaxVelocityX() * -10.0f;
	else if( uiAction & ACTION_MOVE_RIGHT )
		fCurVelocityX = m_pkCoordPhysics->GetMaxVelocityX() * 10.0f;

	fPosX = m_pkCoordPhysics->GetPosX();
	fPosY = m_pkCoordPhysics->GetPosY();

	m_pkCoordPhysics->GetNextPos(fPosX, fPosY, fCurVelocityX);
	return m_pkCoordPhysics->GetNextAttriState(fPosX, fPosY, uiAttribute);
}

//------------------------------------------------------------------------------------
void SPGroundPet::CaluSelfPos()
{
	unsigned __int64 uiAction = ACTION_MOVE_LEFT;
	if( (rand() % 2) == 0 )
		uiAction = ACTION_MOVE_RIGHT;

	m_pkCoordPhysics->SetMaxVelocityX(PET_WALK_VELOCITY * 0.5f);
	SetAction(uiAction, true);

	m_bSetSelfPos = true;
}

//------------------------------------------------------------------------------------
void SPGroundPet::CheckTraceAction(int iDirection)
{
	unsigned __int64 uiAction = 0;
	FACING eFacing = FACING_LEFT;

	float fPosX, fPosY, fParentX, fParentY;
	bool bRun = false;

	fPosX = m_pkCoordPhysics->GetPosX();
	fPosY = m_pkCoordPhysics->GetPosY();

	fParentX = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSX);
	fParentY = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSY);

	int offsetX = BETWEEN_PET * m_iPetPlace;
	if( fabs(fPosX - fParentX) > (PET_DISTANCE_X * 2.0f + offsetX) )
		bRun = true;
	else
	{
		if( (m_uiCurAction & ACTION_RUN_LEFT) || (m_uiCurAction & ACTION_RUN_RIGHT) )
		{
			if( fabs(fPosX - fParentX) > (PET_DISTANCE_X * 1.3f) )
				bRun = true;
		}
	}

	if( iDirection & TD_LEFT )
	{
		if( bRun )
			uiAction = ACTION_RUN_LEFT;
		else
			uiAction = ACTION_MOVE_LEFT;
	}
	else if( iDirection & TD_RIGHT )
	{
		if( bRun )
			uiAction = ACTION_RUN_RIGHT;
		else
			uiAction = ACTION_MOVE_RIGHT;
	}
	else if( iDirection & TD_UP )
	{
		if( m_pkCoordPhysics->IsJumpable(fPosX, fPosY) )
			uiAction = ACTION_MOVE_JUMP;
	}

	if( !(iDirection & TD_IN_X) )
	{
		int iAttributeType = CheckMovable(uiAction);
		if( iAttributeType > 0 )
		{
			if( uiAction )
			{
				if( fPosY >= (fParentY - 5.0f) )
				{
					if( m_pkCoordPhysics->IsMoveJumpable(fPosX, fPosY, eFacing) )
					{
						uiAction |= ACTION_MOVE_JUMP;

						UINT64 uiParentAction;
						m_pkParent->SPGOBPerformMessage(SPGM_GETACTION, (LPARAM)&uiParentAction);

						if( (uiParentAction & ACTION_RUN_LEFT) ||
							(uiParentAction & ACTION_RUN_RIGHT) )
						{
							if( uiAction & ACTION_MOVE_LEFT )
								uiAction |= ACTION_RUN_LEFT;
							else if( uiAction & ACTION_MOVE_RIGHT )
								uiAction |= ACTION_RUN_RIGHT;
						}
					}
					else
						uiAction = ACTION_STOP;
				}
			}
		}
	}
	
	float fVelocity = PET_WALK_VELOCITY;
	if( (uiAction & ACTION_RUN_LEFT) || (uiAction & ACTION_RUN_RIGHT) )
		fVelocity = PET_RUN_VELOCITY;

	if( uiAction == m_uiCurAction )
		return;

	m_pkCoordPhysics->SetMaxVelocityX(fVelocity);
	SetAction(uiAction);
}

//------------------------------------------------------------------------------------
void SPGroundPet::OnWaitPattern(float fTime)
{
	if( m_fPatternAccmulateTime < 0.2f )
		return;

	m_fPatternAccmulateTime = 0.0f;
	int iDirection;

	int offsetX = BETWEEN_PET * m_iPetPlace;
	iDirection = GetTargetDirection(PET_DISTANCE_X+offsetX, PET_DISTANCE_Y);
	if( (iDirection & TD_IN_X) && (iDirection & TD_IN_Y) )
		return;

	if( (iDirection & TD_IN_X) && (iDirection & TD_DOWN) )
		return;

	SetPattern(PT_TRACE);
}

//------------------------------------------------------------------------------------
void SPGroundPet::OnTracePattern(float fTime)
{
	unsigned __int64 uiAction = ACTION_STOP;
	if( !m_pkCoordPhysics->IsActionChange(uiAction) )
		return;

	int iDirection;
	int offsetX = BETWEEN_PET * m_iPetPlace;
	iDirection = GetTargetDirection(PET_DISTANCE_X+offsetX, PET_DISTANCE_Y);

	if( (iDirection & TD_IN_X) && (iDirection & TD_IN_Y) )
	{
		SetAction(ACTION_STOP);
		SetPattern(PT_WAIT);
		return;
	}
	
	if( IsDistanceSquare(PET_DISTANCE_PORTAL * PET_DISTANCE_PORTAL, false) )
	{
		SetPortal();
		return;
	}

	//if( m_fPatternAccmulateTime > 0.5f )
	{
		CheckTraceAction(iDirection);
	//	m_fPatternAccmulateTime = 0.0f;
	}
}

//------------------------------------------------------------------------------------
void SPGroundPet::OnSelfPosPattern(float fTime)
{
	if( !m_bSetSelfPos )
	{
		if( m_fPatternAccmulateTime > 0.3f )
			CaluSelfPos();

		return;
	}

	int iDirection;
	float fSelfDist;

	fSelfDist = PET_DISTANCE_X * 0.5f;
	int offsetX = BETWEEN_PET * m_iPetPlace;
	iDirection = GetTargetDirection(fSelfDist+offsetX, 0.0f);
	if( iDirection & TD_IN_X )
	{
		int iAttribute = CheckMovable(0);
		if( iAttribute != 0 )
			iDirection &= ~TD_IN_X;
	}

	if( !(iDirection & TD_IN_X) )
	{
		SetAction(ACTION_STOP);
		SetPattern(PT_WAIT);
	}
}

//------------------------------------------------------------------------------------
SPGMESSAGE_OBJ_MAP_BEGIN( SPGroundPet )

SPGMESSAGE_OBJ_MAP_END( SPPet )