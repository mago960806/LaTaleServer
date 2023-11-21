
#include "SPCommon.h"
#include "SPDebug.h"
#include "SPUtil.h"
#include "SPMainGameDEF.H"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"
#include "SPGameObject.h"
#include "SPGOBManager.h"
#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPRenderModelDEF.h"
#include "SPGOBClusterDef.h"
#include "SPGOBModelUnit.h"
#include "SPMOBLayerModelUnit.h"
#include "SPMonsterModelUnit.h"

#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXFrameAnimationUnit.h"
#include "SPFXManager.h"
#include "SPFXDamageUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPManager.h"
#include "SPCameraManager.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

#include "SPRenderModel.h"
#include "SPMonsterStatusModel.h"

#include "SPCommandConvert.h"
#include "SPMonsterModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPMonsterCoordPhysics.h"
#include "SPFlyCoordPhysics.h"

#include "SPPet.h"
#include "SPResourceDef.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPSoundManager.h"
#include "SPMOBCluster.h"

#include "SPBattle.h"
#include "SPBattleCluster.h"

#include "SPSkill.h"

#include "SPZone.h"

#include "SPAvatarModelUnitDef.h"
#include "SPUtil.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayerEquipDefine.h"
#include "SPPlayer.h"
#include <WinSock2.h>
#include "PacketID.h"
#include "GlobalDefines_Share.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "SPPetLevelMgr.h"
#include "SPGOBStatus.h"
#include "SPGameObject.h"
#include "SPGOBCoordPhysics.h"
#include "SPMonster.h"
#include "SPCommandConvert.h"
#include "SPEventDEF.h"
#include "SPUserShopAttr.h"
#include "SPUserShopArchive.h"
#include "SPEventManager.h"
#include <algorithm>

//------------------------------------------------------------------------------------
SPPet::SPPet()
: m_eCurState(GOB_STATE_NULL)
, m_pkParent(NULL)
, m_eLastState(GOB_STATE_NULL)
, m_uiCurAction(ACTION_STOP)
, m_uiLastAction(ACTION_STOP)
, m_fCamX(0.0f)
, m_fCamY(0.0f)
, m_pkModelUnit(NULL)
, m_pkCoordPhysics(NULL)
, m_pkStatusModel(NULL)
, m_pkFXArchive(NULL)
, m_bJustRender(false)
, m_iPetPlace( 0 )
, m_Facing( FACING_LEFT )
, m_iItemNoID( 0 )
, m_bOpenShop( false )
, m_iShopStyle( 0 )
, m_bNewPet( false )
, m_bInitedAttr( false )
, m_bShopUnclosedError( false ) 
, m_iItemID( 0 )
{	
	memset( &m_petInfo, 0, sizeof(m_petInfo) );
	m_vecChatMsg.clear();
	m_vecSkill.clear();
}

//------------------------------------------------------------------------------------
SPPet::SPPet(INSTANCE_ID instanceID, CLASS_ID ClassID, SPGameObject* pkParent)
: SPGameObject(instanceID, ClassID)
, m_pkParent(pkParent)
, m_eCurState(GOB_STATE_NULL)
, m_eLastState(GOB_STATE_NULL)
, m_uiCurAction(ACTION_STOP)
, m_uiLastAction(ACTION_STOP)
, m_fCamX(0.0f)
, m_fCamY(0.0f)
, m_pkModelUnit(NULL)
, m_pkCoordPhysics(NULL)
, m_pkStatusModel(NULL)
, m_pkFXArchive(NULL)
, m_bJustRender(false)
, m_iPetPlace( 0 )
, m_Facing( FACING_LEFT )
, m_iItemNoID( 0 )
, m_bOpenShop( false )
, m_iShopStyle( 0 )
, m_bNewPet( false )
, m_bInitedAttr( false )
, m_bShopUnclosedError( false )
, m_iItemID( 0 )
{
	memset( &m_petInfo, 0, sizeof(m_petInfo) );
	m_vecChatMsg.clear();
	m_vecSkill.clear();
}

//------------------------------------------------------------------------------------
SPPet::~SPPet()
{
	Destroy();
}

//------------------------------------------------------------------------------------
void SPPet::Destroy()
{
	if (g_pGOBManager)
	{
		SPPlayer *pLocalPlayer = g_pGOBManager->GetLocalPlayer();
		if (pLocalPlayer && m_pkParent == pLocalPlayer)
		{
			SPPlayerInvenArchive* pInven = pLocalPlayer->GetInvenArchive();
			if (!pInven)
				return;

			SPItem *pItem = pInven->GetInventoryItem( FIGURE_PET, m_iPetPlace );
			if (!pItem)
				return;

			if (m_pkParent)
			{
				PET_SKILL_INFO info;
				info.pItem = pItem;
				info.vecSkills = m_vecSkill;
				m_pkParent->SPGOBPerformMessage( SPGM_PET_SKILL_DELETE, (LPARAM)&info );
			}
		}
	}
	SAFE_DELETE(m_pkModelUnit);
	SAFE_DELETE(m_pkCoordPhysics);
	SAFE_DELETE(m_pkStatusModel);
	SAFE_DELETE(m_pkFXArchive);
}

//------------------------------------------------------------------------------------
void SPPet::RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer )
{
	if( m_pkFXArchive != NULL )
		m_pkFXArchive->Render( fTime , eRenderLayer );
}

//------------------------------------------------------------------------------------
void SPPet::Init()
{
	SPGameObject::Init();
	Destroy();

	if( g_pMOBCluster == NULL )
		return;

	MONSTER_TYPE eMonsterType;
	eMonsterType = g_pMOBCluster->GetMonsterType(GetClassID());
	if( eMonsterType == MT_LAYER_PET )
		m_pkModelUnit = new SPMOBLayerModelUnit(this);
	else
		m_pkModelUnit = new SPMonsterModelUnit(this);

	m_pkModelUnit->Init();

	if( !m_bJustRender )	
	{
		m_pkStatusModel = new SPMonsterStatusModel(this);

		SPMOBUIInfo* pUIInfo = g_pMOBCluster->GetMOBUIInfo(GetClassID());
		if( pUIInfo )
			SetShowStatus(pUIInfo->m_bShowName, pUIInfo->m_bShowHP, pUIInfo->m_bShowShadow);

		m_pkFXArchive = new SPFXArchive(this);

		SPMonsterBeAttacked* pkBeAttacked = NULL;
		g_pBattleCluster->GetAttack(ANI_MONSTER_STAND, &pkBeAttacked, GetClassID());
		if( pkBeAttacked )
			m_pkStatusModel->SetMonterType(pkBeAttacked->m_nMonsterSizeIndex);
	}
	
	m_fCamX = 0.0f;
	m_fCamY = 0.0f;

	m_iJustRenderPosX = 0;
	m_iJustRenderPosY = 0;

	m_iCurXP = 0;
	m_iCurSP = 0;
	m_iMaxXP = 0;
	m_iMaxSP = 0;

	m_eCurAnimationIndex = ANI_MONSTER_STAND;

	m_iSpecialSkillID.clear();
	m_eSpecialSkillType.clear();
	ZeroMemory(m_bAutoLooting, sizeof(m_bAutoLooting));
	ZeroMemory(m_bItemSell, sizeof(m_bItemSell));
	ZeroMemory(m_bAutoItemUsed, sizeof(m_bAutoItemUsed));
	ZeroMemory(m_btagAutoChat, sizeof(m_btagAutoChat));
	ZeroMemory(m_btagOpenShop, sizeof(m_btagOpenShop));

	m_vecChatMsg.clear();
	m_bAutoChat = false;
	m_bOpenShop = false;
	memset( m_SkillStatus, 0, 64*sizeof(int) );
}

//------------------------------------------------------------------------------------
void SPPet::SetPattern(PATTERN_TYPE eType)
{
	m_ePatternType = eType;
	m_fPatternAccmulateTime = 0.0f;
	m_bSetSelfPos = false;
}

//------------------------------------------------------------------------------------
bool SPPet::IsDistanceSquare(float fCompareDist, bool bIn)
{
	if( m_pkCoordPhysics == NULL || m_pkParent == NULL )
		return false;

	float fPosX, fPosY, fParentX, fParentY, fDiffX, fDiffY, fDist;
	bool bRet;
	
	fPosX = m_pkCoordPhysics->GetPosX();
	fPosY = m_pkCoordPhysics->GetPosY();

	fParentX = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSX);
	fParentY = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSY);

	fDiffX = fParentX - fPosX;
	fDiffY = fParentY - fPosY;

	fDist = fDiffX * fDiffX + fDiffY * fDiffY;
	
	bRet = false;
	if( bIn )
	{
		if( fDist <= fCompareDist )
			bRet = true;
	}
	else
	{
		if( fDist > fCompareDist )
			bRet = true;
	}

	return bRet;
}

//------------------------------------------------------------------------------------
int SPPet::GetTargetDirection(float fDistanceX, float fDistanceY)
{
	int iDirection = TD_OUT;
	if( m_pkCoordPhysics == NULL || m_pkParent == NULL )
		return iDirection;

	float fPosX, fPosY, fParentX, fParentY;

	fPosX = m_pkCoordPhysics->GetPosX();
	fPosY = m_pkCoordPhysics->GetPosY();

	fParentX = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSX);
	fParentY = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSY);

	if( fabs(fParentX - fPosX) <= fDistanceX )
		iDirection |= TD_IN_X;
	else
	{
		if( fPosX < fParentX )
			iDirection |= TD_RIGHT;
		else
			iDirection |= TD_LEFT;
	}

	if( fabs(fParentY - fPosY) <= fDistanceY )
		iDirection |= TD_IN_Y;
	else
	{
		if( fPosY < fParentY )
			iDirection |= TD_DOWN;
		else
			iDirection |= TD_UP;
	}

	if (iDirection & TD_RIGHT)
		m_Facing = FACING_RIGHT;
	else if (iDirection & TD_LEFT)
		m_Facing = FACING_LEFT;

	return iDirection;
}

//------------------------------------------------------------------------------------
bool SPPet::IsPortal()
{
	UINT64 uiParentAction;
	m_pkParent->SPGOBPerformMessage(SPGM_GETACTION, (LPARAM)&uiParentAction);

	if( (uiParentAction & GOB_SEQ_JUMP) ||
		(uiParentAction & GOB_SEQ_FALL) )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
void SPPet::SetPortal()
{
	if( !IsPortal() )
		return;

	float fParentX, fParentY;
	int iFacing, iStandLayer;

	fParentX = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSX);
	fParentY = (float)m_pkParent->SPGOBPerformMessage(SPGM_GETPOSY);
	iFacing = m_pkParent->SPGOBPerformMessage(SPGM_GETFACING);
	iStandLayer = m_pkParent->SPGOBPerformMessage(SPGM_GETSTANDLAYER);

	m_pkCoordPhysics->SetPos(fParentX, fParentY);
	m_pkCoordPhysics->SetDirection((FACING)iFacing);
	m_pkCoordPhysics->SetStandLayer((BG_ID)iStandLayer);

	//SetPattern(PT_SELF_POS);
}

//------------------------------------------------------------------------------------
bool SPPet::GetExceptionAction(UINT64& uiParentAction)
{
	if( m_pkParent == NULL )
		return false;

	m_pkParent->SPGOBPerformMessage(SPGM_GETACTION, (LPARAM)&uiParentAction);
	return IsExceptionAction(uiParentAction);
}

//------------------------------------------------------------------------------------
bool SPPet::IsExceptionAction(UINT64 uiAction)
{
	if( (uiAction & ACTION_VERTICAL_STOP) ||
		(uiAction & ACTION_HANGING_STOP) ||
		(uiAction & ACTION_MOVE_UP) ||
		(uiAction & ACTION_MOVE_DOWN) ||
		(uiAction & ACTION_MOVE_HANGING_LEFT) ||
		(uiAction & ACTION_MOVE_HANGING_RIGHT) )
		return true;

	return false;
}

//------------------------------------------------------------------------------------
bool SPPet::IsPreRendering()
{
	if( (m_uiCurAction & ACTION_VERTICAL_STOP) ||
		(m_uiCurAction & ACTION_HANGING_STOP) ||
		(m_uiCurAction & ACTION_MOVE_UP) ||
		(m_uiCurAction & ACTION_MOVE_DOWN) ||
		(m_uiCurAction & ACTION_MOVE_HANGING_LEFT) ||
		(m_uiCurAction & ACTION_MOVE_HANGING_RIGHT) )
		return false;

	return true;
}

//------------------------------------------------------------------------------------
void SPPet::SetState(GOB_STATE_TYPE state, bool bForce)
{
	if( m_pkFXArchive )
		m_pkFXArchive->InsertFXGroup((int)m_eCurAnimationIndex);
}

//------------------------------------------------------------------------------------
void SPPet::SetAnimationLoop()
{
	bool bLoop = true;
	switch( m_eCurState )
	{
	case GOB_STATE_JUMP:
	case GOB_STATE_DROP:
	case GOB_STATE_SKILL:
		bLoop = false;
		break;
	}

	if( m_pkModelUnit )
		m_pkModelUnit->SPSendMessage(MV_SET_ANI_LOOP, bLoop);
}

//------------------------------------------------------------------------------------
void SPPet::SetAction(UINT64 uiAction, bool bForce)
{
	if( !m_pkCoordPhysics )
		return;

	if( !bForce && m_uiCurAction == uiAction )
		return;

	float fTime = 0.0f;
	if( m_pkModelUnit )
		m_pkModelUnit->SPSendMessage(MV_GET_ANI_ACCUMUL_TIME, (WPARAM)&fTime);
	m_pkCoordPhysics->SetActionTime(fTime);
}

//------------------------------------------------------------------------------------
void SPPet::Process(float fTime)
{
	SPGameObject::Process(fTime);

	if( !m_bJustRender )
	{
		if( m_pkCoordPhysics )
			m_pkCoordPhysics->Process(fTime);

		if( m_pkFXArchive )
			m_pkFXArchive->Process(fTime);

		ProcessImage(fTime);
		if (!m_bOpenShop)
			ProcessPattern(fTime);
	}
	else
		ProcessImageJustRender(fTime);

	if (m_pkStatusModel)
		m_pkStatusModel->Process(fTime);
}

//------------------------------------------------------------------------------------
void SPPet::ProcessImage(float fTime)
{
	if( m_pkModelUnit == NULL || m_pkCoordPhysics == NULL )
		return;

	m_fCamX = g_pCameraManagr->GetCameraStruct()->fOffsetX;
	m_fCamY = g_pCameraManagr->GetCameraStruct()->fOffsetY;

	//[xialin 2008/08/06]add
	if ( m_pkParent && m_pkParent->GetClassID() == CLASS_ID_AVATAR
		&& ((SPPlayer *)m_pkParent)->GetLobbyAvatar() == LOBBY_CASH )
	{
		m_fCamX += CASHSHOP_OFFSETCAMX;
		m_fCamY += CASHSHOP_OFFSETCAMY;
	}

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

	bool bShadow = true;
	if( m_pkCoordPhysics->GetCurVelocityY() ||
		m_pkCoordPhysics->GetCurState() == GOB_STATE_DROP ||
		IsExceptionAction(m_uiCurAction) )
		bShadow = false;

	m_bClipping = false;
	if( m_pkStatusModel )
	{
		RECT rcRenderRgn;
		m_pkModelUnit->SPSendMessage(MV_GET_RENDER_RGN, (WPARAM)&rcRenderRgn);

		if( (rcRenderRgn.left < 0 && rcRenderRgn.right < 0) ||
			(rcRenderRgn.left > 800 && rcRenderRgn.right > 800) ||
			(rcRenderRgn.bottom < 0 || rcRenderRgn.top > 600) )
			m_bClipping = true;

		rcRenderRgn.top -= 20;
		m_pkStatusModel->SetObjectRgn(rcRenderRgn, fParsingX, fParsingY, bShadow);
		m_pkStatusModel->SetPetShopRgn(rcRenderRgn, fParsingX, fParsingY );
	}
}

//------------------------------------------------------------------------------------
void SPPet::ProcessImageJustRender(float fTime)
{
	float	fParsingX, fParsingY;
	WPARAM	wParsingX, wParsingY;
	fParsingX = (float)m_iJustRenderPosX;
	fParsingY = (float)m_iJustRenderPosY;
	DataConvert<float,WPARAM>(fParsingX, wParsingX);
	DataConvert<float,WPARAM>(fParsingY, wParsingY);
	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSX, wParsingX);
	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSY, wParsingY);

	m_pkModelUnit->Process(fTime);
}

//------------------------------------------------------------------------------------
void SPPet::ProcessPattern(float fTime)
{
	m_fPatternAccmulateTime += fTime;

	switch( m_ePatternType )
	{
	case PT_WAIT:
		OnWaitPattern(fTime);
		break;
	case PT_TRACE:
		OnTracePattern(fTime);
		break;
	case PT_SELF_POS:
		OnSelfPosPattern(fTime);
		break;
	}
}

//------------------------------------------------------------------------------------
void SPPet::Render(float fTime)
{
	SPGameObject::Render(fTime);

	RenderFX( fTime , FXM_RENDER_LAYER_OBJECT_BACK );

	if( !m_bClipping )
	{
		bool bToggleName;
		bToggleName = IsPreRendering();

		if( m_pkStatusModel )
		{
			m_pkStatusModel->SetToggleName(bToggleName);
			m_pkStatusModel->Render(fTime);
		}
		
		if( m_pkModelUnit )
			m_pkModelUnit->Render(fTime);
	}

	//if( m_pkFXArchive )
	//	m_pkFXArchive->Render(fTime);

	RenderFX( fTime , FXM_RENDER_LAYER_OBJECT_FRONT );
}

//------------------------------------------------------------------------------------
void SPPet::SetPos(float fX, float fY)	
{ 
	m_pkCoordPhysics->SetPos(fX, fY);
	SetPattern(PT_SELF_POS);
}

//------------------------------------------------------------------------------------
void SPPet::SetRePos(int iStageID, int iMapGroupID, float fPosX, float fPosY)
{
	SetPos(fPosX, fPosY);
	SetCurStageID(iStageID);
	SetCurMapGroupID(iMapGroupID);
	
	SetAction(ACTION_STOP);
	SetPattern(PT_WAIT);
}

//------------------------------------------------------------------------------------
void SPPet::SetJustRenderPos(int iX, int iY)
{
	m_iJustRenderPosX = iX;
	m_iJustRenderPosY = iY;
}

//------------------------------------------------------------------------------------
void SPPet::SetJustRenderAnimation(int iAniIndex, bool bLoop)
{
	if( m_pkModelUnit == NULL )
		return;
	m_pkModelUnit->SPSendMessage(MV_SET_ANIMATION_INDEX, iAniIndex);
	m_pkModelUnit->SPSendMessage(MV_SET_ANI_LOOP, bLoop);
}

//------------------------------------------------------------------------------------
void SPPet::SetSpecialSkill(SKILL_SPECIAL_TYPE eSkillType, int iSkillID, bool bEnable, bool bToggle)
{
	int iIndex = 0;
	if( bToggle )
		iIndex = 1;

	if( bEnable )
	{
		bool bFound = false;
		assert( m_iSpecialSkillID.size() == m_eSpecialSkillType.size());
		for (int idx = 0; idx < m_iSpecialSkillID.size() && idx < m_eSpecialSkillType.size(); ++idx)
		{
			if (m_iSpecialSkillID[idx] == iSkillID && m_eSpecialSkillType[idx] == eSkillType)
				bFound = true;
		}
		if (!bFound)
		{
			m_iSpecialSkillID.push_back( iSkillID );
			m_eSpecialSkillType.push_back( eSkillType );
		}
	}

	switch( eSkillType )
	{
	case SST_AUTO_LOOTING:
		m_bAutoLooting[iIndex]= bEnable;
		break;
	case SST_ITEM_SELL:
		m_bItemSell[iIndex]= bEnable;
		break;
	case SST_AUTO_ITEM_USED:
		m_bAutoItemUsed[iIndex]= bEnable;
		break;
	case SST_PET_SHOP:
		m_btagOpenShop[iIndex] = bEnable;
		break;
	case SST_PET_CHAT:
		m_btagAutoChat[iIndex] = bEnable;
		break;
	}

	if (bToggle && bEnable)
		SetSkillStatus( eSkillType, bEnable );
}

//------------------------------------------------------------------------------------
bool SPPet::IsEnableSpecialSkill(SKILL_SPECIAL_TYPE eType)
{
	if (m_bOpenShop
		&&
		(eType != SST_PET_CHAT && eType != SST_PET_SHOP))
	{
		return false;
	}

	bool* pSpecialSkill = NULL;

	switch( eType )
	{
	case SST_AUTO_LOOTING:
		pSpecialSkill = m_bAutoLooting;
		break;
	case SST_ITEM_SELL:
		pSpecialSkill = m_bItemSell;
		break;
	case SST_AUTO_ITEM_USED:
		pSpecialSkill = m_bAutoItemUsed;
		break;
	case SST_PET_SHOP:
		pSpecialSkill = m_btagOpenShop;
		break;
	case SST_PET_CHAT:
		pSpecialSkill = m_btagAutoChat;
		break;
	}

	if( pSpecialSkill == NULL )
		return false;

	return pSpecialSkill[0];
}

//------------------------------------------------------------------------------------
bool SPPet::IsToggleOnSpecialSkill(SKILL_SPECIAL_TYPE eType)
{
	if (m_bOpenShop
		&&
		(eType != SST_PET_CHAT && eType != SST_PET_SHOP))
	{
		return false;
	}

	return GetSkillStatus( eType );

	/*
	bool* pSpecialSkill = NULL;

	switch( eType )
	{
	case SST_AUTO_LOOTING:
		pSpecialSkill = m_bAutoLooting;
		break;
	case SST_ITEM_SELL:
		pSpecialSkill = m_bItemSell;
		break;
	case SST_AUTO_ITEM_USED:
		pSpecialSkill = m_bAutoItemUsed;
		break;
	case SST_PET_SHOP:
		pSpecialSkill = m_btagOpenShop;
		break;
	case SST_PET_CHAT:
		pSpecialSkill = m_btagAutoChat;
		break;
	}

	if( pSpecialSkill == NULL )
		return false;

	return pSpecialSkill[1];
	*/
}

//------------------------------------------------------------------------------------
std::vector<int> &SPPet::GetSpecialSkillID()
{
	return m_iSpecialSkillID;
}

//------------------------------------------------------------------------------------
std::vector<SKILL_SPECIAL_TYPE> &SPPet::GetSpecialSkillType()
{
	return m_eSpecialSkillType;
}

//------------------------------------------------------------------------------------
void SPPet::SetSkillAnimation(ANIMATION_INDEX eIndex)
{
	m_eSkillAnimationIndex = eIndex;
}

//------------------------------------------------------------------------------------
int SPPet::GetCurStageID()
{
	return m_pkCoordPhysics->GetCurStageID();
}

//------------------------------------------------------------------------------------
void SPPet::SetCurStageID(int iStageID)
{
	m_pkCoordPhysics->SetCurStageID(iStageID);
}

//------------------------------------------------------------------------------------
int	SPPet::GetCurMapGroupID()
{
	return m_pkCoordPhysics->GetCurMapGroupID();
}

//------------------------------------------------------------------------------------
void SPPet::SetCurMapGroupID(int iMapGroupID)
{
	m_pkCoordPhysics->SetCurMapGroupID(iMapGroupID);
}

//------------------------------------------------------------------------------------
void SPPet::SetGravity(float fGravity, float fMaxDropVelocity)
{
	m_pkCoordPhysics->SetGravity(fGravity);
	m_pkCoordPhysics->SetMaxDropVelocity(fMaxDropVelocity);
}

//------------------------------------------------------------------------------------
void SPPet::SetJumpSpeed(float fSpeed)
{
	m_pkCoordPhysics->SetJumpSpeed(fSpeed);
}

//------------------------------------------------------------------------------------
void SPPet::SetJustRender(bool bRender)
{
	m_bJustRender = bRender;
}

//------------------------------------------------------------------------------------
void SPPet::SetName(const char* pszName, D3DXCOLOR* pColor)
{
	static D3DXCOLOR color(0.5f, 1.0f, 0.5f, 1.0f); 
	if (!pColor)
		pColor = &color;
	if( m_pkStatusModel )
		m_pkStatusModel->SetObjectName(pszName, pColor);
	_snprintf( m_petInfo.szPetName, 16, "%s", pszName );
}

//------------------------------------------------------------------------------------
void SPPet::SetLocal(bool bLocal)
{
	if( bLocal && m_pkStatusModel )
		m_pkStatusModel->SetPet(true);
}

//------------------------------------------------------------------------------------
void SPPet::SetPetGageRgn(int iCurSP, int iMaxSP, INT64 iCurXP, INT64 iMaxXP)
{
	if (m_bNewPet)
	{
		iCurSP = m_petInfo.sp;
		iMaxSP = 100;
		iCurXP = m_petInfo.exp;
		iMaxXP = SPPetLevelMgr::getInstance().GetXPForPet( m_petInfo.lv );
		if (iMaxXP == 0)
		{
			iMaxXP = 100;
			iCurXP = 100;
		}
	}

	if (m_pkStatusModel)
		m_pkStatusModel->SetPetGageRgn(iCurSP, iMaxSP, iCurXP, iMaxXP);
}


BG_ID SPPet::GetStandLayer()
{
	if(m_pkCoordPhysics)
		return m_pkCoordPhysics->GetStandLayer();

	return BG_ID_LAYER_NULL;
}


//------------------------------------------------------------------------------------
void SPPet::SetShowStatus(bool bName, bool bHP, bool bShadow)
{
	if( m_pkStatusModel )
		m_pkStatusModel->SetShowValue(bName, bHP, bShadow);
}

//------------------------------------------------------------------------------------
int SPPet::OnGetPosX(LPARAM lParam)
{
	return static_cast<int>( m_pkCoordPhysics->GetPosX() );
}

//------------------------------------------------------------------------------------
int SPPet::OnGetPosY(LPARAM lParam)
{
	return static_cast<int>( m_pkCoordPhysics->GetPosY() );
}

//------------------------------------------------------------------------------------
int SPPet::OnGetFacing(LPARAM lParam)
{
	if( m_pkCoordPhysics == NULL )
		return 0;

	return m_pkCoordPhysics->GetDirection();
}

//------------------------------------------------------------------------------------
int SPPet::OnSetLPPosotion( LPARAM lParam )
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
	Info.fKnockBackTime = pPosInfo->fKnockTime;
	Info.fDelayTime = pPosInfo->fDelayTime;
	Info.fKnockBackVelocityX = pPosInfo->fKnockVelocityX;
	Info.fKnockBackVelocityY = pPosInfo->fKnockVelocityY;

	m_pkCoordPhysics->SetKnockBack(Info);

	SetPattern(PT_SELF_POS);
	return 0;
}

//------------------------------------------------------------------------------------
int SPPet::OnSetActionCommand(LPARAM lParam)
{
	UINT64 uiTempAction = m_uiCurAction;
	UINT64 uiAction = *((UINT64*)lParam);
	uiTempAction |= uiAction;
	SetAction(uiTempAction);
	return 1;
}

//------------------------------------------------------------------------------------
int SPPet::OnSetActionCompleteCommand(LPARAM lParam)
{
	UINT64 uiTempAction = m_uiCurAction;
	UINT64 uiAction = *((UINT64*)lParam);
	uiTempAction &= ~uiAction;
	SetAction(uiTempAction);
	return 1;
}

//--------------------------------------------------
int SPPet::OnPetShout(LPARAM lParam)
{
	CPacket	*packet = (CPacket*)lParam;
	if (!packet)
		return 1;

	GLOBAL_STRING_ID	errorID;
	packet->ExtractUINT32( &errorID );
	if (errorID)
	{
		return 1;
	}

	// msg
	char strMsg[51] = {0,};
	UINT8 slotIndex = 0;
	packet->ExtractUINT8( &slotIndex );
	packet->ExtractData( (TCHAR*)(&strMsg), 50 );
	SetTalkBalloon( strMsg );
	return 0;
}

//--------------------------------------------------
void SPPet::SetTalkBalloon( const char *pszString )
{
	if (m_pkStatusModel)
		m_pkStatusModel->SetTalkBalloon( pszString );
}

//--------------------------------------------------
void SPPet::SetChatMsg(const std::vector< std::string > &vecChatMsg)
{
	m_vecChatMsg.clear();
	for (int idx = 0; idx < (int)vecChatMsg.size(); ++idx)
	{
		if (!vecChatMsg[idx].empty())
			m_vecChatMsg.push_back( vecChatMsg[idx] );
	}
}

void SPPet::SendChat( void )
{
	static int count = 0;
	if (m_vecChatMsg.empty())
		return;
	count ++;
	if (count >= (int)m_vecChatMsg.size())
		count = 0;
	char buf[128] = {0,};
	_snprintf( buf, 127, "%s", m_vecChatMsg[count].c_str() );

	CPacket packet;
	packet.AddUINT32((UINT32)PET_CS_SHOUT);
	packet.AddUINT64(m_iItemNoID);
	packet.AddData( buf, 50 );
	g_pNetworkManager->SPPerformMessage( PET_CS_SHOUT, (WPARAM)0, (LPARAM)&packet );
}

//--------------------------------------------------
bool SPPet::SetPetAttr(t_PetAttr_Info *info)
{
	if (!info)
		return false;

	if (m_bInitedAttr && info->lv != m_petInfo.lv)
	{
		unsigned int uiRealClassID = SPPetLevelMgr::getInstance().GetRealPetClass( m_iItemID, info->lv );
		SetClassID( (CLASS_ID)uiRealClassID );
		ReloadPetClass();
	}
	
	//--------------------------------------------------
	if (info->uPetStatus == 2 
		||	
		info->uPetStatus == 3)
	{
		SetSkillStatus( SST_PET_SHOP, true );
	}
	else
	{
		SetSkillStatus( SST_PET_SHOP, false );
	}
	//--------------------------------------------------

	m_petInfo = *info;
	char bufName[LEN_NAME+1 ] = {0,};
	memcpy( bufName, m_petInfo.szPetName, LEN_NAME );
	if (bufName[0] != '\0')
		SetName( bufName, NULL ); 
	if (m_petInfo.uPetStatus == 1)
	{
		m_bOpenShop = false;
	}
	else
	{
		m_bOpenShop = true;
	}

	if (m_bNewPet && !m_bInitedAttr)
	{
		unsigned int uiRealClassID = SPPetLevelMgr::getInstance().GetRealPetClass( m_iItemID, m_petInfo.lv );
		SetClassID( (CLASS_ID)uiRealClassID );
		ReloadPetClass();
		m_bInitedAttr = true;
	}
}

//--------------------------------------------------
void SPPet::SetShopName(std::string strShopName)
{
	if (m_pkStatusModel)
		m_pkStatusModel->SetShopName( strShopName );
}
void SPPet::SetOpenShopStatus(bool bOpenShop)
{
	m_bOpenShop = bOpenShop;

	if (m_bOpenShop)
	{
		m_pkFXArchive->InsertFXGroup( IDFXGR_PET_SHOP );
		unsigned int iSeqIndex = 0;
		SPUserShopAttr	*pShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData( m_iShopStyle );
		if (pShopData)
			iSeqIndex = pShopData->m_iMotion;
		unsigned int FxGroupIndex = GetGOBSeqKey( 1001, iSeqIndex, CLASS_A ); 
		SPFXGroup	*pfxGroup = g_pFXManager->GetFXGroup( FxGroupIndex, CLASS_ID_AVATAR );
		if (pfxGroup)
			m_pkFXArchive->InsertFXGroup( FxGroupIndex, pfxGroup );
	}
	else
	{
		m_pkFXArchive->DeleteFXGroup( IDFXGR_PET_SHOP );
		unsigned int iSeqIndex = 0;
		SPUserShopAttr	*pShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData( m_iShopStyle );
		if (pShopData)
			iSeqIndex = pShopData->m_iMotion;
		unsigned int FxGroupIndex = GetGOBSeqKey( 1001, iSeqIndex, CLASS_A ); 
		m_pkFXArchive->DeleteFXGroup( FxGroupIndex );
	}
}
void SPPet::SetShopStyle(int iShopStyle)
{
	/*
	if (m_iShopStyle != 0 && m_pkFXArchive)
	{
		m_pkFXArchive->DeleteFXGroup( m_iShopStyle );
		m_iShopStyle = 0;
	}

	m_iShopStyle = iShopStyle;
	if (m_pkFXArchive)
		m_pkFXArchive->InsertFXGroup( iShopStyle );
	*/

	m_iShopStyle = iShopStyle;
	if (m_pkStatusModel)
		m_pkStatusModel->SetShopType( m_iShopStyle );

}
void SPPet::SetShopGUID(GU_ID petShopGUID)
{
	m_uiPetShopGUID = petShopGUID;
}
GU_ID SPPet::GetShopGUID( void )
{
	return m_uiPetShopGUID;
}
bool SPPet::IsCursorIn(int iX, int iY)
{
	if (m_bOpenShop)
	{
		RECT *pRECT = m_pkStatusModel->GetPetShopBoardRect();
		int iMouseX = g_pInput->GetMousePosX();
		int iMouseY = g_pInput->GetMousePosY();
		if(pRECT && ((pRECT->left < iMouseX && pRECT->right > iMouseX) && 
			(pRECT->top < iMouseY && pRECT->bottom > iMouseY)))
		{
			return true;
		}
	}

	return false;
}
bool SPPet::IsCursorIn( RECT &rect )
{
	if (m_bOpenShop)
	{
		RECT *pRECT = m_pkStatusModel->GetPetShopBoardRect();
		RECT rtPetShop = *pRECT;
		rtPetShop.bottom += 150;
		bool	bLeft			=	( rtPetShop.left	<= rect.left	&& rtPetShop.right		>= rect.left	);
		bool	bTop			=	( rtPetShop.top		<= rect.top		&& rtPetShop.bottom		>= rect.top		);
		bool	bRight			=	( rtPetShop.left	<= rect.right	&& rtPetShop.right		>= rect.right	);
		bool	bBottom			=	( rtPetShop.top		<= rect.bottom	&& rtPetShop.bottom		>= rect.bottom	);

		bool	bLeftTop		=	bLeft	& bTop;
		bool	bLeftBottom		=	bLeft	& bBottom;
		bool	bRightTop		=	bRight	& bTop;
		bool	bRightBottom	=	bRight	& bBottom;

		return bLeftTop | bLeftBottom | bRightTop | bRightBottom;
	}
	else
		return false;
}
int SPPet::PerformMouseMove(int iX, int iY)
{
	if (IsCursorIn(iX,iY))
		return 1;
	else
		return 0;
}
int SPPet::PerformMouseLDown(int iX, int iY)
{
	return 0;
}
int SPPet::PerformMouseLUp(int iX, int iY)
{
	if (IsCursorIn(iX,iY))
		return 1;
	else
		return 0;
}

void SPPet::SetSkill(std::vector< t_Pet_Skill_To_Client > &vecSkill)
{
	m_vecSkill.clear();
	m_vecSkill = vecSkill;
	SPPlayer *pLocalPlayer = g_pGOBManager->GetLocalPlayer();
	if (pLocalPlayer && m_pkParent == pLocalPlayer)
	{
		SPPlayerInvenArchive* pInven = pLocalPlayer->GetInvenArchive();
		if (!pInven)
			return;

		SPItem *pItem = pInven->GetInventoryItem( FIGURE_PET, m_iPetPlace );
		if (!pItem)
			return;

		if (m_pkParent)
		{
			PET_SKILL_INFO info;
			info.pItem = pItem;
			info.vecSkills = m_vecSkill;
			m_pkParent->SPGOBPerformMessage( SPGM_PET_SKILL_INSERT, (LPARAM)&info );
		}
	}
}

void SPPet::AddSkill(t_Pet_Skill_To_Client &skill)
{
	m_vecSkill.push_back( skill );
	SPPlayer *pLocalPlayer = g_pGOBManager->GetLocalPlayer();
	if (pLocalPlayer && m_pkParent == pLocalPlayer)
	{
		SPPlayerInvenArchive* pInven = pLocalPlayer->GetInvenArchive();
		if (!pInven)
			return;

		SPItem *pItem = pInven->GetInventoryItem( FIGURE_PET, m_iPetPlace );
		if (!pItem)
			return;

		if (m_pkParent)
		{
			PET_SKILL_INFO info;
			info.pItem = pItem;
			info.vecSkills = m_vecSkill;
			m_pkParent->SPGOBPerformMessage( SPGM_PET_SKILL_INSERT, (LPARAM)&info );
		}
	}
}

//------------------------------------------------------------------------------------
bool SPPet::ReloadPetClass(void)
{
	m_pkModelUnit->Init();
	return true;
}

//------------------------------------------------------------------------------------

SPGMESSAGE_OBJ_MAP_BEGIN( SPPet )

SPGMESSAGE_OBJ_COMMAND(  SPGM_GETPOSX				,	OnGetPosX				)
SPGMESSAGE_OBJ_COMMAND(  SPGM_GETPOSY				,	OnGetPosY				)
SPGMESSAGE_OBJ_COMMAND(  SPGM_GETFACING				,	OnGetFacing				)
SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_LPPOSITION		,	OnSetLPPosotion			)

SPGMESSAGE_OBJ_COMMAND(  SPGM_COMMAND_ACTION		,	OnSetActionCommand		)
SPGMESSAGE_OBJ_COMMAND(  SPGM_COMMAND_ACTION_COMPLETE,	OnSetActionCompleteCommand )
SPGMESSAGE_OBJ_COMMAND(  SPGM_PET_SHOUT, OnPetShout )

SPGMESSAGE_OBJ_MAP_END( SPGameObject )