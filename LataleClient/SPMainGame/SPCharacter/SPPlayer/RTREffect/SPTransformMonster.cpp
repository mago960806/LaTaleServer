//--------------------------------------------------
// so many includes...
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
#include "SPTransformMonster.h"
//--------------------------------------------------
SPTransformMonster::SPTransformMonster( void )
{
	m_pkModelUnit = NULL;
	Init();
}
SPTransformMonster::SPTransformMonster(INSTANCE_ID instanceID, CLASS_ID ClassID)
: SPGameObject(instanceID, ClassID)
{
	m_pkModelUnit = NULL;
	Init();
}
SPTransformMonster::~SPTransformMonster( void )
{
	if (m_pkModelUnit)
		SAFE_DELETE( m_pkModelUnit );	
}
//--------------------------------------------------
void SPTransformMonster::Process( float fTime )
{
	if (m_pkModelUnit)
		m_pkModelUnit->Process( fTime );
}
void SPTransformMonster::Render( float fTime )
{
	if (m_pkModelUnit)
		m_pkModelUnit->Render( fTime );
}
void SPTransformMonster::Init( void )
{
	m_bFly = false;
	m_pkModelUnit = new SPMonsterModelUnit( this );
}
void SPTransformMonster::Destroy( void )
{
	if (m_pkModelUnit)
		SAFE_DELETE( m_pkModelUnit );
}
//--------------------------------------------------
void SPTransformMonster::SetState(GOB_STATE_TYPE state)
{
	m_eCurState = state;
	ANIMATION_INDEX	eAniIndex	=	ANI_NULL;
	switch( state )
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
		eAniIndex	=	ANI_MONSTER_ATTACK;
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
		SetAnimationLoop();
		m_pkModelUnit->SPSendMessage(MV_SET_ANIMATION_INDEX, eAniIndex );
	}
}
void SPTransformMonster::SetAnimationLoop( void )
{
	bool bLoop = true;
	switch( m_eCurState )
	{
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
void SPTransformMonster::SetAction(UINT64 uiAction)
{
	if( uiAction == ACTION_STOP )
		SetState(GOB_STATE_STAND);
	else if( uiAction & ACTION_DEAD )
		SetState(GOB_STATE_DEAD);
	else
	{
		if( uiAction & ACTION_MOVE_LEFT ||
			uiAction & ACTION_MOVE_RIGHT )
			if (!(m_uiCurAction & ACTION_MOVE_LEFT || m_uiCurAction & ACTION_MOVE_RIGHT))
				SetState(GOB_STATE_WALK);

		if( uiAction == ACTION_RUN_LEFT ||
			uiAction == ACTION_RUN_RIGHT )
			if (!(m_uiCurAction & ACTION_RUN_LEFT || m_uiCurAction & ACTION_RUN_RIGHT))
				SetState(GOB_STATE_RUN);

		if( uiAction & ACTION_MOVE_JUMP )
			SetState(GOB_STATE_JUMP);
		else if( uiAction & ACTION_MOVE_DROP )
			SetState(GOB_STATE_DROP);

		if( uiAction & ACTION_SIT_DOWN )
			SetState(GOB_STATE_SIT_DOWN);
		else if( uiAction & ACTION_SIT_WAIT )
			SetState(GOB_STATE_SIT_WAIT);
		else if( uiAction & ACTION_SIT_STAND )
			SetState(GOB_STATE_SIT_STAND);

		if( uiAction & ACTION_ATTACK_SPEEDY )
			SetState(GOB_STATE_ATTACK_SPEEDY);
		if( uiAction & ACTION_ATTACK_MIGHTY )
			SetState(GOB_STATE_ATTACK_MIGHTY);
		else if( uiAction & ACTION_BEATTACKED )
			SetState(GOB_STATE_ATTACKED);
		else if( uiAction & ACTION_GUARD_DAMAGE )
			SetState(GOB_STATE_GUARD_DAMAGE);
	}

	if( uiAction & ACTION_MOVE_LEFT || uiAction & ACTION_RUN_LEFT)
	{
		m_eFacing = FACING_LEFT;
	}
	else if (uiAction & ACTION_MOVE_RIGHT || uiAction & ACTION_RUN_RIGHT)
	{
		m_eFacing = FACING_RIGHT;
	}

	m_uiCurAction = uiAction;
}
void SPTransformMonster::Reload( void )
{
	if (m_pkModelUnit)
		m_pkModelUnit->Init();
}
//--------------------------------------------------
SPGMESSAGE_OBJ_MAP_BEGIN( SPTransformMonster )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETFACING				,	OnGetFacing				)
SPGMESSAGE_OBJ_MAP_END( SPGameObject )
//--------------------------------------------------
int SPTransformMonster::OnGetFacing(LPARAM lParam)
{
	return m_eFacing;
}
//--------------------------------------------------
