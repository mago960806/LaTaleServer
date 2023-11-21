
#include "SPCommon.h"
#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBManager.h"

#include "SPStoryActor.h"

#include "SPGOBCoordPhysics.h"
#include "SPPlayer.h"
#include "SPMonster.h"
#include "SPCommandConvert.h"
#include "SPStoryScene.h"

using namespace std;

//------------------------------------------------------------------------------------
SPStoryActor::SPStoryActor(INSTANCE_ID iInstanceID, CLASS_ID iClassID,
	const char* pszName, float fPosX, float fPosY, float fVelocityX, float fJumpSpeed,
	bool bPlayer, int iFacing, SPStoryScene* pkScene)
: m_pkObject(NULL)
, m_pCurAction(NULL)
, m_pkScene(pkScene)
, m_bPlayer(bPlayer)
, m_bPlayAction(false)
, m_iRenderOrder(0)
{
	int iObjectType;
	if( bPlayer )
	{
		m_pkObject = new SPPlayer;
		m_pkObject->SetInstanceID(iInstanceID);
		m_pkObject->Init();

		iObjectType = 1;
	}
	else
	{
		m_pkObject = new SPMonster(iInstanceID, iClassID);
		m_pkObject->Init();
		iObjectType = 2;
	}

	m_strName = pszName;
	UINT64 uiAction = ACTION_STOP;
	m_pkObject->SPGOBPerformMessage(SPGM_SETACTION, (LPARAM)&uiAction);
	m_pkObject->SPGOBPerformMessage(SPGM_SETSTATUSRENDER, 0);
	m_pkObject->SPGOBPerformMessage(SPGM_SETFACING, iFacing);
	m_pkObject->SPGOBPerformMessage(SPGM_SETIGNORESTAGE, 1);
	g_pGOBManager->AddGameObject(m_pkObject, iObjectType);
	
	if( bPlayer )
		g_pGOBManager->SetLocalPlayer((SPPlayer*)m_pkObject);

	SPGOBCoordPhysics* pkCoordPhysics = NULL;
	m_pkObject->SPGOBPerformMessage(SPGM_GETCOORDPHYSICS, (LPARAM)&pkCoordPhysics);
	if( pkCoordPhysics )
	{
		pkCoordPhysics->SetMaxVelocityX(fVelocityX);
		pkCoordPhysics->SetJumpSpeed(fJumpSpeed);
		pkCoordPhysics->SetPos(fPosX, fPosY);
		
	}
}

//------------------------------------------------------------------------------------
SPStoryActor::~SPStoryActor()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPStoryActor::Clear()
{
	//SAFE_DELETE(m_pkObject);
	m_mActionList.clear();
}

//------------------------------------------------------------------------------------
void SPStoryActor::StartScene(int iSceneNo)
{
	m_pCurAction = NULL;
	map< int, ACTOR_ACTION >::iterator iter = m_mActionList.find(iSceneNo);
	if( iter == m_mActionList.end() )
		return;

	m_pCurAction = &((*iter).second);
	if( m_pCurAction )
	{
		m_pCurAction->m_fAccmulateTime = 0.0f;
		m_pCurAction->m_fCurPosX = m_pCurAction->m_fStrPosX;
		m_pCurAction->m_fCurPosY = m_pCurAction->m_fStrPosY;

		if( m_pCurAction->iSkillID )
		{
		}
		else
		{
			m_pkObject->SPGOBPerformMessage(SPGM_SETGOBSEQINDEX, (LPARAM)m_pCurAction->iSeqIndex);
			m_pkObject->SPGOBPerformMessage(SPGM_SETACTION, (LPARAM)&m_pCurAction->uiAction);
		}

		ResumeAction();
		m_iCurSceneNo = iSceneNo;
	}
}

//------------------------------------------------------------------------------------
void SPStoryActor::WaitAction()
{
	m_bPlayAction = false;
}

//------------------------------------------------------------------------------------
void SPStoryActor::ResumeAction()
{
	m_bPlayAction = true;
}

//------------------------------------------------------------------------------------
void SPStoryActor::AddAction(int iSceneNo, ACTOR_ACTION& Action)
{
	m_mActionList.insert(make_pair(iSceneNo, Action));
}

//------------------------------------------------------------------------------------
void SPStoryActor::ItemSetting(ACTOR_DEFAULT_LAYER* pLayer)
{
	if( m_pkObject == NULL || m_bPlayer == false || pLayer == NULL )
		return;

	SPPlayer* pkPlayer = static_cast< SPPlayer* >(m_pkObject);
	if( pkPlayer == NULL )
		return;
	
	//[2007/11/21] - 플레이어타입 스토리 액터 클래스 지정
	pkPlayer->SetPlayerClass(pLayer->iClassType);
	
	FIGURE_ITEM figureItem;
	figureItem.iItemID = pLayer->iDefaultHair;
	figureItem.SlotInfo.ContainerType = FIGURE_BASIC;
	figureItem.SlotInfo.SlotIndex = EQ_HAIR;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iDefaultExpression;
	figureItem.SlotInfo.ContainerType = FIGURE_BASIC;
	figureItem.SlotInfo.SlotIndex = EQ_EXPRESSION;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iDefaultSkin;
	figureItem.SlotInfo.ContainerType = FIGURE_BASIC;
	figureItem.SlotInfo.SlotIndex = EQ_SKIN;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iDefaultBlouse;
	figureItem.SlotInfo.ContainerType = FIGURE_BASIC;
	figureItem.SlotInfo.SlotIndex = EQ_BLOUSE;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iDefaultPants;
	figureItem.SlotInfo.ContainerType = FIGURE_BASIC;
	figureItem.SlotInfo.SlotIndex = EQ_PANTS;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iCap;
	figureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
	figureItem.SlotInfo.SlotIndex = EQ_HAIR_DRESS;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iGlasses;
	figureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
	figureItem.SlotInfo.SlotIndex = EQ_GOGGLE;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iEarring;
	figureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
	figureItem.SlotInfo.SlotIndex = EQ_EARRING;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iBlouse;
	figureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
	figureItem.SlotInfo.SlotIndex = EQ_BLOUSE;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iPants;
	figureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
	figureItem.SlotInfo.SlotIndex = EQ_PANTS;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iCloak;
	figureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
	figureItem.SlotInfo.SlotIndex = EQ_CLOAK;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iGlove;
	figureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
	figureItem.SlotInfo.SlotIndex = EQ_GLOVES;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iFootwear;
	figureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
	figureItem.SlotInfo.SlotIndex = EQ_SHOES;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iStocking;
	figureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
	figureItem.SlotInfo.SlotIndex = EQ_STOCKINGS;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iMakeup;
	figureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
	figureItem.SlotInfo.SlotIndex = EQ_MAKEUP;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iWeapon;
	figureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
	figureItem.SlotInfo.SlotIndex = EQ_WEAPON1;
	pkPlayer->ItemSetting(figureItem);

	figureItem.iItemID = pLayer->iShield;
	figureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
	figureItem.SlotInfo.SlotIndex = EQ_BUCKLER1;
	pkPlayer->ItemSetting(figureItem);
}

//------------------------------------------------------------------------------------
void SPStoryActor::ItemSetting(FIGURE_ITEM ItemInfo)
{
	if( m_pkObject == NULL || m_bPlayer == false )
		return;

	SPPlayer* pkPlayer = static_cast< SPPlayer* >(m_pkObject);
	if( pkPlayer == NULL )
		return;

	pkPlayer->ItemSetting(ItemInfo);
}

//------------------------------------------------------------------------------------
void SPStoryActor::SetRenderOrder(int iOrder)
{
	m_iRenderOrder = iOrder;
}

//------------------------------------------------------------------------------------
int SPStoryActor::GetRenderOrder()
{
	return m_iRenderOrder;
}

//------------------------------------------------------------------------------------
const char* SPStoryActor::GetName()
{
	return m_strName.c_str();
}

//------------------------------------------------------------------------------------
int SPStoryActor::SendGOBMessage(SPGM uiMsg, LPARAM lParam)
{
	if( m_pkObject == NULL )
		return 0;
	
	return m_pkObject->SPGOBPerformMessage(uiMsg, lParam);
}

//------------------------------------------------------------------------------------
void SPStoryActor::SendActionComplete()
{
	if( m_pkScene == NULL )
		return;

	m_pkScene->CompleteActorAction(m_strName.c_str(), m_iCurSceneNo);
	m_pCurAction = NULL;
	m_iCurSceneNo = 0;
}

//------------------------------------------------------------------------------------
bool SPStoryActor::ProcessbyTime(float fTime)
{
	if( m_pCurAction == NULL || m_pkObject == NULL )
	{
		SendActionComplete();
		return false;
	}

	m_pCurAction->m_fAccmulateTime += fTime;
	if( m_pCurAction->m_fAccmulateTime >= m_pCurAction->m_fCompleteTime )
		SendActionComplete();

	return true;
}

//------------------------------------------------------------------------------------
bool SPStoryActor::ProcessbyPos(float fTime)
{
	if( m_pCurAction == NULL || m_pkObject == NULL )
	{
		SendActionComplete();
		return false;
	}

	SPGOBCoordPhysics* pkCoordPhysics = NULL;
	m_pkObject->SPGOBPerformMessage(SPGM_GETCOORDPHYSICS, (LPARAM)&pkCoordPhysics);
	if( pkCoordPhysics )
	{
		float fDirection;
		float fVelocityX = pkCoordPhysics->GetMaxVelocityX();
		if( m_pCurAction->m_fCurPosX < m_pCurAction->m_fDstPosX )
			fDirection = 1.0f;
		else
			fDirection = -1.0f;

		m_pCurAction->m_fCurPosX += (fVelocityX * fTime * fDirection);
		if( fDirection > 0.0f )
		{
			if( m_pCurAction->m_fCurPosX >= m_pCurAction->m_fDstPosX )
				m_pCurAction->m_fCurPosX = m_pCurAction->m_fDstPosX;
		}
		else
		{
			if( m_pCurAction->m_fCurPosX <= m_pCurAction->m_fDstPosX )
				m_pCurAction->m_fCurPosX = m_pCurAction->m_fDstPosX;
		}
		pkCoordPhysics->SetPos(m_pCurAction->m_fCurPosX, m_pCurAction->m_fCurPosY);
	}

	if( m_pCurAction->m_fCurPosX == m_pCurAction->m_fDstPosX && m_pCurAction->m_fCurPosY == m_pCurAction->m_fDstPosY )
		SendActionComplete();

	return true;
}

//------------------------------------------------------------------------------------
bool SPStoryActor::ProcessbySeq(float fTime)
{
	if( m_pCurAction == NULL || m_pkObject == NULL )
	{
		SendActionComplete();
		return false;
	}

	UINT64 uiAction;
	m_pkObject->SPGOBPerformMessage(SPGM_GETACTION, (LPARAM)&uiAction);
	if( uiAction == ACTION_STOP )
		SendActionComplete();
	
	return true;
}

//------------------------------------------------------------------------------------
void SPStoryActor::Process(float fTime)
{
	if( m_pkObject == NULL )
		return;

	m_pkObject->Process(fTime);
	if( m_pCurAction && m_bPlayAction )
	{
		switch( m_pCurAction->eCompleteType )
		{
		case ACT_TIME:
			ProcessbyTime(fTime);
			break;
		case ACT_POS:
			ProcessbyPos(fTime);
			break;
		case ACT_SEQ:
			ProcessbySeq(fTime);
			break;
		}
	}
}

//------------------------------------------------------------------------------------
void SPStoryActor::Render(float fTime)
{
	if( m_pkObject == NULL )
		return;

	m_pkObject->Render(fTime);
}
