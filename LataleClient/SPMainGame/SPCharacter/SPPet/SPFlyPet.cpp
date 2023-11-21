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
#include "SPFlyCoordPhysics.h"

#include "SPPet.h"
#include "SPFlyPet.h"

//------------------------------------------------------------------------------------
SPFlyPet::SPFlyPet()
{
}

//------------------------------------------------------------------------------------
SPFlyPet::SPFlyPet(INSTANCE_ID instanceID, CLASS_ID ClassID, SPGameObject* pkParent)
: SPPet(instanceID, ClassID, pkParent)
{
}

//------------------------------------------------------------------------------------
SPFlyPet::~SPFlyPet()
{
}

//------------------------------------------------------------------------------------
void SPFlyPet::Init()
{
	SPPet::Init();

	m_pkCoordPhysics = new SPFlyCoordPhysics(this);
	m_pkCoordPhysics->Init();
}

//------------------------------------------------------------------------------------
void SPFlyPet::SetAction(UINT64 uiAction, bool bForce)
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
	else
	{
		if( m_uiCurAction & ACTION_MOVE_LEFT ||
			m_uiCurAction & ACTION_MOVE_RIGHT )
			SetState(GOB_STATE_WALK, bForce);

		if( m_uiCurAction == ACTION_RUN_LEFT ||
			m_uiCurAction == ACTION_RUN_RIGHT )
			SetState(GOB_STATE_RUN, bForce);

		if( m_uiCurAction & ACTION_SKILL )
			SetState(GOB_STATE_SKILL, bForce);
	}

	SPPet::SetAction(uiAction, bForce);
}

//------------------------------------------------------------------------------------
void SPFlyPet::SetLootingFX(int iID)
{
	if( m_pkFXArchive == NULL )
		return;

	if( iID == 0 )
		iID = 15057;

	m_pkFXArchive->InsertFXGroup((SPID_FX_GROUP)iID);
}

//------------------------------------------------------------------------------------
void SPFlyPet::SetState(GOB_STATE_TYPE state, bool bForce)
{
	if( !bForce && m_eCurState == state )
		return;

	m_eLastState = m_eCurState;
	m_eCurState = state;
	
	ANIMATION_INDEX	eAniIndex	=	ANI_NULL;
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

	case GOB_STATE_SKILL:
		eAniIndex = m_eSkillAnimationIndex;
		break;
	}

	if( eAniIndex != ANI_NULL )
	{
		m_pkModelUnit->SPSendMessage(MV_SET_ANIMATION_INDEX, eAniIndex );
		SetAnimationLoop();
	}

	m_eCurAnimationIndex = eAniIndex;
	SPPet::SetState(state, bForce);
}

//------------------------------------------------------------------------------------
bool SPFlyPet::IsPreRendering()
{
	return true;
}

//------------------------------------------------------------------------------------
void SPFlyPet::CaluSelfPos()
{
	unsigned __int64 uiAction = ACTION_MOVE_LEFT;
	if( (rand() % 2) == 0 )
		uiAction = ACTION_MOVE_RIGHT;

	m_pkCoordPhysics->SetMaxVelocityX(PET_WALK_VELOCITY * 0.5f);
	SetAction(uiAction, true);

	m_bSetSelfPos = true;
}

//------------------------------------------------------------------------------------
void SPFlyPet::CheckTraceAction(int iDirection)
{
	unsigned __int64 uiAction = 0;
	FACING eFacing = FACING_LEFT;

	float fPosX, fPosY, fParentX, fParentY, fDiffX, fDiffY;
	float fBeforeJump;
	bool bRunX = false, bRunY = false;

	fPosX = m_pkCoordPhysics->GetPosX();
	fPosY = m_pkCoordPhysics->GetPosY();

	fParentX = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSX);
	fParentY = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSY);

	fDiffX = fabs(fPosX - fParentX);
	fDiffY = fabs(fPosY - fParentY);
	fBeforeJump = m_pkCoordPhysics->GetJumpSpeed();

	int offsetX = BETWEEN_PET * m_iPetPlace;
	if( fDiffX > (PET_DISTANCE_X * 2.0f+offsetX) )
		bRunX = true;
	else
	{
		if( (m_uiCurAction & ACTION_RUN_LEFT) || (m_uiCurAction & ACTION_RUN_RIGHT) )
		{
			if( fDiffX > (PET_DISTANCE_X * 1.3f) )
				bRunX = true;
		}
	}

	if( fDiffY > (PET_DISTANCE_Y * 2.0f) )
		bRunY = true;
	else
	{
		if( fBeforeJump < -PET_WALK_VELOCITY )
		{
			if( fDiffY > (PET_DISTANCE_Y * 1.3f) )
				bRunY = true;
		}
	}

	if( iDirection & TD_LEFT )
	{
		if( bRunX )
			uiAction = ACTION_RUN_LEFT;
		else
			uiAction = ACTION_MOVE_LEFT;
	}
	else if( iDirection & TD_RIGHT )
	{
		if( bRunX )
			uiAction = ACTION_RUN_RIGHT;
		else
			uiAction = ACTION_MOVE_RIGHT;
	}

	if( iDirection & TD_UP )
		uiAction |= ACTION_MOVE_UP;
	else if( iDirection & TD_DOWN )
		uiAction |= ACTION_MOVE_DOWN;

	float fVelocityX = 0.0f;
	float fVelocityY = 0.0f;
	
	if( (uiAction & ACTION_MOVE_LEFT) || (uiAction & ACTION_MOVE_RIGHT) )
		fVelocityX = PET_WALK_VELOCITY;
	else if( (uiAction & ACTION_RUN_LEFT) || (uiAction & ACTION_RUN_RIGHT) )
		fVelocityX = PET_RUN_VELOCITY;

	if( (uiAction & ACTION_MOVE_UP) || (uiAction & ACTION_MOVE_DOWN) )
	{
		if( bRunY )
			fVelocityY = -(PET_WALK_VELOCITY * 1.5f);
		else
			fVelocityY = -PET_WALK_VELOCITY;
	}

	if( uiAction == m_uiCurAction )
	{
		if( fBeforeJump != fVelocityY )
		{
			if( uiAction & ACTION_MOVE_UP )
				m_pkCoordPhysics->SetCurVelocityY(fVelocityY);
			else if( uiAction & ACTION_MOVE_DOWN )
				m_pkCoordPhysics->SetCurVelocityY(-fVelocityY);
		}

		return;
	}
		
	m_pkCoordPhysics->SetMaxVelocityX(fVelocityX);
	m_pkCoordPhysics->SetJumpSpeed(fVelocityY);
	
	SetAction(uiAction);
}

//------------------------------------------------------------------------------------
void SPFlyPet::OnWaitPattern(float fTime)
{
	if( m_fPatternAccmulateTime < 0.2f )
		return;

	m_fPatternAccmulateTime = 0.0f;
	int iDirection;
	int offsetX = BETWEEN_PET * m_iPetPlace;
	iDirection = GetTargetDirection(PET_DISTANCE_X+offsetX, PET_DISTANCE_Y * 0.1f);
	if( (iDirection & TD_IN_X) && (iDirection & TD_IN_Y) )
		return;

	SetPattern(PT_TRACE);
}


//------------------------------------------------------------------------------------
void SPFlyPet::OnTracePattern(float fTime)
{
	unsigned __int64 uiAction = ACTION_STOP;
	if( !m_pkCoordPhysics->IsActionChange(uiAction) )
		return;

	int iDirection;
	int offsetX = BETWEEN_PET * m_iPetPlace;
	iDirection = GetTargetDirection(PET_DISTANCE_X+offsetX, PET_DISTANCE_Y * 0.1f);

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
void SPFlyPet::OnSelfPosPattern(float fTime)
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

	if( !(iDirection & TD_IN_X) )
	{
		SetAction(ACTION_STOP);
		SetPattern(PT_WAIT);
	}
}

//------------------------------------------------------------------------------------
SPGMESSAGE_OBJ_MAP_BEGIN( SPFlyPet )

SPGMESSAGE_OBJ_MAP_END( SPPet )
