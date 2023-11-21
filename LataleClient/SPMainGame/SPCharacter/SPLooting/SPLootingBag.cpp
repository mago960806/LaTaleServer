#include "SPCommon.h"
#include "SPDebug.h"
#include "SPUtil.h"
#include "SPMainGameDEF.H"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"

#include "SPGOBClusterDef.h"
#include "SPGOBModelUnit.h"
#include "SPMonsterModelUnit.h"

#include "SPManager.h"
//#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16]
#include "SPGOBManager.h"
#include "SPCameraManager.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

#include "SPCommandConvert.h"
#include "SPMonsterModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPMonsterCoordPhysics.h"
#include "SPGOBStatus.h"
#include "SPMonsterStatus.h"
#include "SPMonsterStatusModel.h"

#include "SPAvatarModelUnitDef.h"
#include "SPPlayerStatusModel.h"
#include "SPPlayer.h"

#include "SPBattle.h"
#include "SPBattleCluster.h"
#include "SPBattleLoader.h"

#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXManager.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPCheckManager.h"

#include "Packet.h"

#include "SPLootingBag.h"
#include "SPUtil.h"
#include "SPResourceDef.h"

//------------------------------------------------------------------------------------
SPLootingBag::SPLootingBag()
: m_eCurState(GOB_STATE_NULL)
, m_eLastState(GOB_STATE_NULL)
, m_uiCurAction(ACTION_NULL)
, m_uiLastAction(ACTION_NULL)
, m_fCamX(0.0f)
, m_fCamY(0.0f)
, m_pkModelUnit(NULL)
, m_pkCoordPhysics(NULL)
, m_OwerID(0)
{
}

//------------------------------------------------------------------------------------
SPLootingBag::SPLootingBag(INSTANCE_ID instanceID, CLASS_ID ClassID)
: SPGameObject(instanceID, ClassID)
, m_eCurState(GOB_STATE_NULL)
, m_eLastState(GOB_STATE_NULL)
, m_uiCurAction(ACTION_NULL)
, m_uiLastAction(ACTION_NULL)
, m_fCamX(0.0f)
, m_fCamY(0.0f)
, m_pkModelUnit(NULL)
, m_pkCoordPhysics(NULL)
, m_pkFXArchive(NULL)
{
}

//------------------------------------------------------------------------------------
SPLootingBag::~SPLootingBag()
{
	Destroy();
}

//------------------------------------------------------------------------------------
void SPLootingBag::Destroy()
{
	SAFE_DELETE(m_pkModelUnit);
	SAFE_DELETE(m_pkCoordPhysics);
	SAFE_DELETE(m_pkFXArchive);

	m_vItemList.clear();
}

//------------------------------------------------------------------------------------
void SPLootingBag::Init()
{
	SPGameObject::Init();
	Destroy();

	m_pkModelUnit = new SPMonsterModelUnit(this);
	m_pkModelUnit->Init();

	m_pkCoordPhysics = new SPMonsterCoordPhysics(this);
	m_pkCoordPhysics->Init();

	m_pkFXArchive = new SPFXArchive(this);

	m_pkCoordPhysics->SetDirection(FACING_LEFT);
}

//------------------------------------------------------------------------------------
void SPLootingBag::SetState(GOB_STATE_TYPE state, bool bForce)
{
	if( !bForce && m_eCurState == state )
		return;

	m_eLastState = state;
	m_eCurState = state;

	m_pkModelUnit->SPSendMessage(MV_SET_ACCUMUL_ACTION, 0);
	switch( m_eCurState )
	{
	case GOB_STATE_STAND:
		m_pkModelUnit->SPSendMessage(MV_SET_ANIMATION_INDEX, ANI_MONSTER_STAND);
		m_pkModelUnit->SPSendMessage(MV_SET_ANI_LOOP, false);
		m_pkFXArchive->InsertFXGroup(ANI_MONSTER_STAND);
		break;
	}
}

//------------------------------------------------------------------------------------
void SPLootingBag::SetAction(UINT64 uiAction, bool bForce)
{
	if( !bForce && m_uiCurAction == uiAction )
		return;

	m_uiLastAction = m_uiCurAction;
	m_uiCurAction = uiAction;

	m_pkCoordPhysics->SetAction(uiAction);
	if( m_uiCurAction == ACTION_STOP )
	{
		SetState(GOB_STATE_STAND);
	}
	
	m_bEnablePickup = false;

	float fTime;
	m_pkModelUnit->SPSendMessage(MV_GET_ANI_ACCUMUL_TIME, (WPARAM)&fTime);
	m_pkCoordPhysics->SetActionTime(fTime);
	m_fActionCompleteTime = fTime;
}

//------------------------------------------------------------------------------------
void SPLootingBag::Process(float fTime)
{
	SPGameObject::Process(fTime);
	m_pkCoordPhysics->Process(fTime);
	ProcessImage(fTime);

	if( !m_bEnablePickup )
	{
		m_fActionCompleteTime -= fTime;
		if( m_fActionCompleteTime < 0 )
		{
			m_bEnablePickup = true;
			//m_pkCoordPhysics->SetAction(ACTION_MOVE_DROP);
		}
	}

	if( m_pkFXArchive != NULL )
		m_pkFXArchive->Process( fTime );
}

//------------------------------------------------------------------------------------
void SPLootingBag::ProcessImage(float fTime)
{
	m_fCamX = g_pCameraManagr->GetCameraStruct()->fOffsetX;
	m_fCamY = g_pCameraManagr->GetCameraStruct()->fOffsetY;

	float	fParsingX, fParsingY;
	WPARAM	wParsingX, wParsingY; 
	fParsingX = m_pkCoordPhysics->GetPosX()-m_fCamX;
	fParsingY = m_pkCoordPhysics->GetPosY()-m_fCamY;
	DataConvert<float,WPARAM>(fParsingX, wParsingX);
	DataConvert<float,WPARAM>(fParsingY, wParsingY);
	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSX, wParsingX );
	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSY, wParsingY );

	m_pkModelUnit->Process(fTime);

	RECT rcRenderRgn;
	m_pkModelUnit->SPSendMessage(MV_GET_RENDER_RGN, (WPARAM)&rcRenderRgn);

	m_bClipping = false;
	if( (rcRenderRgn.left < 0 && rcRenderRgn.right < 0) ||
		(rcRenderRgn.left > 800 && rcRenderRgn.right > 800) ||
		(rcRenderRgn.bottom < 0 || rcRenderRgn.top > 600) )
		m_bClipping = true;
}


//------------------------------------------------------------------------------------
void SPLootingBag::Render(float fTime)
{
	SPGameObject::Render(fTime);

	RenderFX( fTime , FXM_RENDER_LAYER_OBJECT_BACK );

	if( !m_bClipping )
	{
		m_pkModelUnit->Render(fTime);
	
#if defined(_DEBUG)
	char szBuf[128];
	sprintf(szBuf, "%d", m_InstanceID);

	int	iParsingX, iParsingY;
	iParsingX = (int)(m_pkCoordPhysics->GetPosX() - m_fCamX);
	iParsingY = (int)(m_pkCoordPhysics->GetPosY() - m_fCamY);

	RECT rcRgn = {iParsingX - 50, iParsingY - 50, iParsingX + 50, iParsingY - 30};
	g_pVideo->GetFont()->RenderText(szBuf, &rcRgn, DT_VCENTER | DT_CENTER );
#endif
	}

	//if( m_pkFXArchive != NULL )
	//	m_pkFXArchive->Render( fTime );

	RenderFX( fTime , FXM_RENDER_LAYER_OBJECT_FRONT );
}

//------------------------------------------------------------------------------------
void SPLootingBag::RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer )
{
	if( m_pkFXArchive != NULL )
		m_pkFXArchive->Render( fTime , eRenderLayer );
}

//------------------------------------------------------------------------------------
bool SPLootingBag::GetEnablePickup()
{
	return m_bEnablePickup;
}

//------------------------------------------------------------------------------------
void SPLootingBag::SetOwerID(GU_ID id)
{
	m_OwerID = id;
}

//------------------------------------------------------------------------------------
void SPLootingBag::AddItem(CONTAINER_ITEM item)
{
	m_vItemList.push_back(item);
}

//------------------------------------------------------------------------------------
void SPLootingBag::DeleteItem(CONTAINER_ITEM item)
{
	vector< CONTAINER_ITEM >::iterator iter = m_vItemList.begin();
	while( iter != m_vItemList.end() )
	{
		if( (item.Item.iItemID == (*iter).Item.iItemID) &&
			(item.SlotInfo.SlotIndex == (*iter).SlotInfo.SlotIndex) )
		{
			m_vItemList.erase(iter);
			return;
		}

		iter++;
	}
}

//------------------------------------------------------------------------------------
int SPLootingBag::OnGetPosX(LPARAM lParam)
{
	return (int)m_pkCoordPhysics->GetPosX();
}

//------------------------------------------------------------------------------------
int SPLootingBag::OnGetPosY(LPARAM lParam)
{
	return (int)m_pkCoordPhysics->GetPosY();
}

//------------------------------------------------------------------------------------
int SPLootingBag::OnGetFacing(LPARAM lParam)
{
	return m_pkCoordPhysics->GetDirection();
}

//------------------------------------------------------------------------------------
std::vector< CONTAINER_ITEM >* SPLootingBag::GetItemList()
{
	return &m_vItemList;
}

//------------------------------------------------------------------------------------
bool SPLootingBag::IsValidItem(CONTAINER_ITEM* pItemInfo)
{
	vector< CONTAINER_ITEM >::iterator iter = m_vItemList.begin();
	while( iter != m_vItemList.end() )
	{
		if( (pItemInfo->Item.iItemID == (*iter).Item.iItemID) &&
			(pItemInfo->SlotInfo.SlotIndex == (*iter).SlotInfo.SlotIndex) )
			return true;

		++iter;
	}

	return false;
}


//------------------------------------------------------------------------------------
int SPLootingBag::GetCurStageID()
{
	return m_pkCoordPhysics->GetCurStageID();
}

//------------------------------------------------------------------------------------
void SPLootingBag::SetCurStageID(int iStageID)
{
	m_pkCoordPhysics->SetCurStageID(iStageID);
}

//------------------------------------------------------------------------------------
int	SPLootingBag::GetCurMapGroupID()
{
	return m_pkCoordPhysics->GetCurMapGroupID();
}

//------------------------------------------------------------------------------------
void SPLootingBag::SetCurMapGroupID(int iMapGroupID)
{
	m_pkCoordPhysics->SetCurMapGroupID(iMapGroupID);
}

//------------------------------------------------------------------------------------
int SPLootingBag::OnSetLPPosotion(LPARAM lParam)
{
	LT_POSITION* pPosInfo = (LT_POSITION*)lParam;

	m_pkCoordPhysics->SetGravity(pPosInfo->fAccelX);
	m_pkCoordPhysics->SetMaxDropVelocity(pPosInfo->fAccelY);
	m_pkCoordPhysics->SetPos(pPosInfo->fPosX, pPosInfo->fPosY);
	m_pkCoordPhysics->SetStandLayer((BG_ID)pPosInfo->bgStandLayer);

	//m_pkCoordPhysics->SetDirection((FACING)pPosInfo->eFacing);
	//m_pkCoordPhysics->SetAccel(pPosInfo->fAccelX, pPosInfo->fAccelY);
	//m_pkCoordPhysics->SetCurVelocity(pPosInfo->fVelocityX, pPosInfo->fVelocityY);

	return 1;
}

//------------------------------------------------------------------------------------
int SPLootingBag::OnGetStandlayer(LPARAM lParam)
{	
	return m_pkCoordPhysics->GetStandLayer();
}

//------------------------------------------------------------------------------------
int SPLootingBag::OnLootingItem(LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;	

	//unsigned char ucNum, i;
	LOOTING_ITEM LootingItemInfo;
	CONTAINER_ITEM ContainerItemInfo;

	//pkPacket->ExtractUINT8(&ucNum);
	//for( i = 0; i < ucNum; i++ )
	//{
		pkPacket->Extract(&LootingItemInfo, sizeof(LOOTING_ITEM));
		ContainerItemInfo.Item.iItemID = LootingItemInfo.iItemID;
		ContainerItemInfo.SlotInfo.SlotIndex = LootingItemInfo.iSlotIndex;

		DeleteItem(ContainerItemInfo);
		
		DXUTOutputDebugString("Remove RootBag [%d]\n", ContainerItemInfo.SlotInfo.SlotIndex);
	//}

	if( g_pGOBManager->GetLocalPlayer() ) {
		//g_pCheckManager->SetDBBlock(false);			//[2005/11/22] 팻킷단에서 블럭해제
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_STAT_SC_LOOTING_ITEM, (LPARAM)this);
	}

	return 0;
}

//------------------------------------------------------------------------------------
int SPLootingBag::OnSetActionCommand(LPARAM lParam)
{
	UINT64 uiTempAction = m_uiCurAction;
	UINT64 uiAction = *((UINT64*)lParam);
	uiTempAction |= uiAction;
	SetAction(uiTempAction);
	return 1;
}

//------------------------------------------------------------------------------------
int SPLootingBag::OnSetActionCompleteCommand(LPARAM lParam)
{
	//UINT64 uiTempAction = m_uiCurAction;
	//UINT64 uiAction = *((UINT64*)lParam);
	//uiTempAction &= ~uiAction;
	//SetAction(uiTempAction);
	return 1;
}

//------------------------------------------------------------------------------------
// Message
SPGMESSAGE_OBJ_MAP_BEGIN(SPLootingBag)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETFACING				,	OnGetFacing				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETPOSX				,	OnGetPosX				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETPOSY				,	OnGetPosY				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETSTANDLAYER			,	OnGetStandlayer			)

	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_LPPOSITION		,	OnSetLPPosotion			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_STAT_SC_LOOTING_ITEM	,	OnLootingItem			)

	SPGMESSAGE_OBJ_COMMAND(  SPGM_COMMAND_ACTION		,	OnSetActionCommand		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_COMMAND_ACTION_COMPLETE,	OnSetActionCompleteCommand )

SPGMESSAGE_OBJ_MAP_END(SPGameObject)