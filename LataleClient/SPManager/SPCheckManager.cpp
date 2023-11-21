#include "SPCommon.h"
#include "SPManagerDef.h"
#include "SPManager.h"

//
#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModel.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPGOBCluster.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBModelUnit.h"			
#include "SPPlayerEquipInfo.h"
#include "SPGOBStatus.h"			
#include "SPPlayerInvenArchive.h"	
#include "SPPlayerStatusModel.h"	
#include "SPPlayerEquipInfo.h"
#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"
#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPMotionStatus.h"
#include "SPSkillManager.h"
#include "SPPlayer.h"
#include "SPPet.h"
#include "SPPetManager.h"
#include "SPGOBManager.h"
#include "SPCoolTimeManager.h"
#include "SPCommandConvert.h"
#include "SPComboManager.h"

//
#include "SPEventDEF.h"
#include "SPEvent.h"
#include "SPQuestAttr.h"
#include "SPQuestArchive.h"
#include "SPQuestManager.h"
#include "SPEventArchive.h"
#include "SPCoreMissionArchive.h"
#include "SPCoreEventManager.h"
#include "SPEventManager.h"

#include "SPEffect.h"
#include "SPEffectArchive.h"
#include "SPEffectManager.h"

#include "SPGuildArchive.h"

#include "SPTitleArchive.h"
#include "SPTitleManager.h"

#include "SPStage.h"
#include "SPStageManager.h"

#include "SPLocalizeManager.h"
#include "SPPvpManager.h"

#include "SPCheckManager.h"

#include "SPSubGameManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"


SPCheckManager*	g_pCheckManager = NULL;

SPCheckManager::SPCheckManager() : SPManager()
{
	m_bDBBlock = false;
	SetMgrID(MGR_ID_CHECK);
	g_pCheckManager = this;
	m_bPetShopOpen = false;
}


SPCheckManager::~SPCheckManager()
{
	Clean();
}


void SPCheckManager::Process(float fTime)
{
	SPManager::Process(fTime);
}


void SPCheckManager::Render(float fTime)
{
	SPManager::Render(fTime);
}
//virtual void RenderTextDebug();
//virtual void RenderText();

bool SPCheckManager::Init()
{
	SPManager::Init();
	return true;
}


void SPCheckManager::Clean()
{
	SPManager::Clean();	
}

int SPCheckManager::SPPerformMessage(UINT msg, WPARAM wparam /*= 0*/, LPARAM lParam /*= 0*/)
{
	return 1;
}


//////////////////////////////////////////////////////////////////////////	Status 체크하는...
bool SPCheckManager::CheckPCStatus(CHECK_STATUS iStatus, INT64 iValue1, INT64 iValue2)
{
	INT64 iCheckValue = 0;
	
	if(iStatus == CHECK_STATUS_NULL)				//[2005/10/5]
		return true;

	if(g_pGOBManager->GetLocalPlayer()) {
		iCheckValue = g_pGOBManager->GetLocalPlayer()->GetCheckStatusValue(iStatus);

		if(iCheckValue < 0)							//검사할값이 0보다 작은 경우 실패로 간주
			return false;

		if(iStatus == CHECK_STATUS_CLASS) {
			int iMyClass = GetClass(iCheckValue);
			int iCheckVClass = (int)iValue1;
			//if((iValue1 & iMyClass) == iMyClass) {//제한 플래그 &연산 결과가 내 클래스와 같은 경우
			if((iCheckVClass & iMyClass) == iMyClass) {
				return true;
			}
			else {
				return false;
			}
		}
		if(iStatus == CHECK_STATUS_GENDER) {
			if(iCheckValue == iValue1) {
				return true;
			}
			else {
				return false;
			}
		}
		else {										//그 외의 경우
			if(iValue2 < 1) {						//Value1만 체크하는 경우
				//if(iValue1 < 0) {
				//	return false;
				//}
				//else {
				//	if(iValue1 <= iCheckValue)		//Check할값이 iValue1보다 크거나 같은 경우에만
				//		return true;
				//}
				if(iValue1 <= iCheckValue) {		//Check할값이 iValue1보다 크거나 같은 경우에만
					return true;
				}
				else {
					return false;
				}
			}
			else{									//Value1, 2 다 체크하는 경우				
				//if(iValue1 > -1 && iValue2 > 0) {
				//	if(iValue1 <= iCheckValue && iCheckValue <= iValue2) {	//Check할값이 iValue1보다 크거나 같고 iValue2보다 작거나 같은 경우에만.
				//		return true;
				//	}
				//}

				if(iValue1 <= iCheckValue && iCheckValue <= iValue2) {	//Check할값이 iValue1보다 크거나 같고 iValue2보다 작거나 같은 경우에만.
					return true;
				}
				else {
					return false;
				}
			}
		}		
	}

	return false;								//모두 실패로 간주
}


bool SPCheckManager::CheckPCItem(int iItemID, INT64 iValue1, INT64 iValue2)
{	
	SPItemAttr* pItem;
	g_pItemCluster->GetItemInfo(iItemID, pItem);

	if(pItem == NULL) {		
		return false;
	}
	
	if(g_pGOBManager->GetLocalPlayer()) {
		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		int iCount = pInven->GetTotalStackCount((CONTAINER_TYPE)(pItem->m_eType - 1), iItemID);
		if(iValue2 <= iCount) {
			return true;
		}
	}
	
	return false;
}


int	SPCheckManager::GetTotalPCItemCount(int iItemID)
{
	int iItemTotalCount = 0;

	SPItemAttr* pItem;
	g_pItemCluster->GetItemInfo(iItemID, pItem);
	if(pItem == NULL)
		return false;

	if(g_pGOBManager->GetLocalPlayer()) {
		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		iItemTotalCount = pInven->GetTotalStackCount((CONTAINER_TYPE)(pItem->m_eType - 1), iItemID);
		return iItemTotalCount;
	}

	return iItemTotalCount;
}


bool SPCheckManager::CheckEnableSlot(int iItemID, int iStack /*= 1*/, int iCheckCount /*= 1*/)
{
	SPPlayerInvenArchive* pInven = NULL;
	CONTAINER_TYPE iContainer = CONTAINER_TYPE_INVALID;
	
	SPItemAttr* pItem;
	g_pItemCluster->GetItemInfo(iItemID, pItem);
	if(pItem == NULL)
		return false;

	iContainer = (CONTAINER_TYPE)(pItem->m_eType - 1);
	
	if(g_pGOBManager->GetLocalPlayer()) {
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	}
	else {
		return false;
	}
	
	if(pInven->GetEmptySlotCount(iContainer) >= iCheckCount) {		//빈 슬롯 부터 체크
		return true;
	}
	else {
		if(pInven->IsEnableSpace(iContainer, iItemID, iStack))		//여유 스택공간 체크
			return true;
	}

	return false;
}


bool SPCheckManager::CheckRequireSlot(int iContainerType, int iRequireCount)
{
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer()) {
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	}
	else {
		return false;
	}
	
	if(pInven->GetEmptySlotCount((CONTAINER_TYPE)iContainerType) >= iRequireCount) {		//빈 슬롯 부터 체크
		return true;
	}

	return false;
}


bool SPCheckManager::CheckPCSkill(int iSkillID, INT64 iValue1, INT64 iValue2)
{
	int iCurSkillLevel = 0;
	if(g_pGOBManager->GetLocalPlayer()) {
		SPSkillActivity* pActiveSkill = g_pGOBManager->GetLocalPlayer()->GetSkillArchive()->GetSkillActivity(iSkillID);
		if(pActiveSkill == NULL)
			return false;

		iCurSkillLevel = pActiveSkill->GetLevel();
		if(iValue1 == 1) {
			if(iCurSkillLevel > 0)				//0보다 크면 성공...
				return true;
		}
		else if(iValue1 == 2) {					//Value2보다 크거나 같으면 성공
			if(iValue2 <= iCurSkillLevel)
				return true;
		}
		else {
		}		
	}
	
	return false;
}


bool SPCheckManager::CheckPCQuest(int iQuestID, INT64 iValue1, INT64 iValue2)
{
	int iRewordCount = 0;
	if(g_pGOBManager->GetLocalPlayer()) {
		iRewordCount = g_pEventManager->GetQuestManager()->GetRewordCount(iQuestID);

		if(iValue1 == 1) {
			if(iValue2 <= iRewordCount) {
				return true;
			}
			else {
				return false;
			}
		}
		else if(iValue1 == 2) {
			if(iRewordCount < iValue2) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}	
	
	return false;
}


bool SPCheckManager::CheckDoingQuest(int iType, int iQuestID)
{
	bool bExist = false;
	if(g_pGOBManager->GetLocalPlayer()) {
		bExist = g_pEventManager->GetQuestManager()->IsDoingQuest(iQuestID);

		if(iType == 3)
			return bExist;
		else if(iType == 4)
			return !bExist;
	}

	return bExist;
}


bool SPCheckManager::CheckPCTitle(int iTitleID, INT64 iValue1, INT64 iValue2)
{
	if(g_pGOBManager->GetLocalPlayer()) {
		SPTitleManager*	pTitleManager	=	SPTitleManager::GetInstance();
		return pTitleManager->GetTitleArchive()->FindTitle(iTitleID);
	}
	return false;
}


bool SPCheckManager::CheckPCEffect(int iEffectID, INT64 iValue1, INT64 iValue2)
{
	if(g_pGOBManager->GetLocalPlayer()) {
		SPEffectArchive* pkEffectArchive = g_pGOBManager->GetLocalPlayer()->GetEffectArchive();
		return pkEffectArchive->IsExistEffectActivity(iEffectID);
	}

	return false;
}


bool SPCheckManager::CheckPCGuildLevel(int iGuildLevel, INT64 iValue1, INT64 iValue2)
{
	if(g_pGOBManager->GetLocalPlayer()) {
		SPGuildArchive* pkGuildArchive = g_pGOBManager->GetGuildArchive() ;
		return pkGuildArchive->CheckGuildLevel(iGuildLevel, (int)iValue2) ;
	}

	return false;
}


bool SPCheckManager::CheckPCGuildType(int iGuildType, INT64 iValue1, INT64 iValue2)
{
	if(g_pGOBManager->GetLocalPlayer()) {
		SPGuildArchive* pkGuildArchive = g_pGOBManager->GetGuildArchive() ;
		return pkGuildArchive->CheckGuildType(iGuildType) ;
	}

	return false;
}

////
//bool SPCheckManager::CheckPCCoreEvent(int iIndex, bool bCheckValue)
//{
//	bool bResult = g_pEventManager->GetCoreEventManager()->IsCompleteMission(iIndex);
//
//	if(bResult == bCheckValue)
//		return true;
//
//	return false;
//}


bool SPCheckManager::CheckPCCoreEvent(int iID, int iIndex)
{
	bool bResult = false;
	bResult = g_pEventManager->GetCoreEventManager()->IsCompleteMission(iIndex);
	
	if(iID == 1) {
		return bResult;
	}
	else if(iID == 2) {
		return !bResult;
	}
}


D3DXCOLOR SPCheckManager::GetItemColor(int iRareLevel)
{
	// AJJIYA [7/10/2006]
	//	이전엔 잘 되었는데 이번에 안되어서 변경
	//	int / int = int 가되어서 거의다 0이 나와서 검정색만 표현 1일 경우도 가끔 있음.

	D3DXCOLOR Color(0.0f, 0.0f, 0.0f, 1.0f);
	switch(iRareLevel)
	{
	case 1:
		Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 2:
//		Color = D3DXCOLOR(44 / 225,	143 / 225, 53 / 225, 1.0f);
		Color = D3DCOLOR_ARGB( 255 , 44 , 143 , 53 );
		break;
	case 3:
//		Color = D3DXCOLOR(26 / 225,	155 / 225, 209 / 225, 1.0f);
		Color = D3DCOLOR_ARGB( 255 , 26 , 155 , 209 );
		break;
	case 4:
//		Color = D3DXCOLOR(222 / 225, 146 / 225, 27 / 225, 1.0f);
		Color = D3DCOLOR_ARGB( 255 , 222 , 146 , 27 );
		break;
	case 5:
//		Color = D3DXCOLOR(255 / 225, 0.0f, 0.0f, 1.0f);
		Color = D3DCOLOR_ARGB( 255 , 255 , 0 , 0 );
		break;
	case 6:
//		Color = D3DXCOLOR(154 / 225, 65 / 225, 176 / 225, 1.0f);
		Color = D3DCOLOR_ARGB( 255 , 154 , 65 , 176 );
		break;
	case 7:
//		Color = D3DXCOLOR(255 / 225, 117 / 225, 188 / 225, 1.0f);
		Color = D3DCOLOR_ARGB( 255 , 255 , 117, 188 );
		break;
	default:
		Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	}

	return Color;
}


bool SPCheckManager::CheckItemStatus(int iItemID, bool bPvp /*= false*/)
{
	int iCheckItemID = iItemID;	

	if(iCheckItemID == 0)
		return false;

	SPItemAttr* pItemAttr;
	g_pItemCluster->GetItemInfo(iCheckItemID, pItemAttr);
	if( pItemAttr == NULL )
		return false;

	if(bPvp) {
		if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP)) {
			if(g_pkPvpManager->GetPvpStatus() > PVP_STATUS_ENTERWAIT) {
				if(pItemAttr->m_bPvp == false) {
					return false;
				}		
			}
		}
	}

	// AJJIYA [11/18/2005]
	// 필요 조건 변경

	for( int i = 0 ; i < MAX_ITEM_OPTION ; i++ )
	{
		if( CheckCondition( pItemAttr->m_RequireType[i].m_iType , pItemAttr->m_RequireType[i].m_iID , pItemAttr->m_RequireType[i].m_iValue1 , pItemAttr->m_RequireType[i].m_iValue2 ) == false )
			return false;
	}

	return true;
}


bool SPCheckManager::CheckBiggerSlot(int iItemID, int iCheckStack)
{
	SPPlayerInvenArchive* pInven = NULL;
	CONTAINER_TYPE iContainer = CONTAINER_TYPE_INVALID;
	bool bResult = false;

	SPItemAttr* pItem;
	g_pItemCluster->GetItemInfo(iItemID, pItem);
	if(pItem == NULL)
		return false;

	iContainer = (CONTAINER_TYPE)(pItem->m_eType - 1);

	if(g_pGOBManager->GetLocalPlayer()) {
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		
		bResult =  pInven->IsBiggerStack(iContainer, iItemID, iCheckStack);
		return bResult;
	}

	return bResult;
}


bool SPCheckManager::CheckBiggerSlot5(int iItemID, int iCheckStack)
{
	SPPlayerInvenArchive* pInven = NULL;
	CONTAINER_TYPE iContainer = CONTAINER_TYPE_INVALID;
	bool bResult = false;

	SPItemAttr* pItem;
	g_pItemCluster->GetItemInfo(iItemID, pItem);
	if(pItem == NULL)
		return false;

	iContainer = (CONTAINER_TYPE)(pItem->m_eType - 1);

	if(g_pGOBManager->GetLocalPlayer()) {
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();

		bResult =  pInven->IsBiggerStack5(iContainer, iItemID, iCheckStack);
		return bResult;
	}

	return bResult;
}


bool SPCheckManager::CheckEquip(int iCheckType, int iCheckSlotIndex /*= 0*/)
{
	bool bResult = false;
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return bResult;

	if(iCheckSlotIndex > 0)			//0베이스
		iCheckSlotIndex -= 1;	
	
	SPPlayerInvenArchive* pInven = NULL;
	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if(pInven == NULL)
		return bResult;

	bResult = pInven->CheckEquipInven(iCheckType, iCheckSlotIndex);

	return bResult;
}


bool SPCheckManager::CheckEquipMotion(int iCheckType, int iCheckSlotIndex /*= 0*/, int iCheckMotionID /*= 0*/)
{
	bool bResult = false;
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return bResult;

	if(iCheckSlotIndex > 0)			//0베이스
		iCheckSlotIndex -= 1;

	SPPlayerInvenArchive* pInven = NULL;
	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if(pInven == NULL)
		return bResult;

	bResult = pInven->CheckEquipMotion(iCheckType, iCheckSlotIndex, iCheckMotionID);

	return bResult;
}


NOTICE_USE SPCheckManager::GetNoticeUse()
{
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer()) {
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		if( pInven )
			return pInven->GetCurNoticeUse();
	}
	
	return NOTICE_USE_NULL;
}


NOTICE_USE SPCheckManager::SetNoticeUse(NOTICE_USE iUse /*= NOTICE_USE_NULL*/)
{
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer()) {
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		pInven->SetNoticeUse(iUse);
	}
	return iUse;
}


// Only Caster fx, sound
bool SPCheckManager::SetLocalEffect(int iEffectID)
{
	if( iEffectID == 0 )
		return false;

	if( g_pGOBManager->GetLocalPlayer() )
	{
		SPEffectArchive* pkEffectArchive = g_pGOBManager->GetLocalPlayer()->GetEffectArchive();

		SPEffect* pkEffect = SPEffectManager::GetInstance()->GetEffect(iEffectID);
		if( pkEffect == NULL )
			return false;

		SPEffectActivity* pkEffectActivity;
		pkEffectActivity = new SPEffectActivity(pkEffect, 0, 0.0f, true);
		pkEffectArchive->AddEffect(0, pkEffectActivity);
	}

	return true;
}


void SPCheckManager::SetDBBlock(bool bBlock)
{
	m_bDBBlock = bBlock;
}


bool SPCheckManager::IsDBBlock()
{
	return m_bDBBlock;
}

bool SPCheckManager::CheckEquipItem( int eEquipID , INT64 iValue )
{
	SPPlayer*	pPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pPlayer == NULL )
		return false;

	INT64	iSourValue		=	(INT64)pPlayer->GetEquipItem( (EQUIP_ID)eEquipID );

	if( iValue != 0 && iValue != iSourValue )
		return false;

	return true;
}

bool SPCheckManager::CheckSkillUpRequire( int iSkillID )
{
	SPPlayer* pkPlayer = g_pGOBManager->GetLocalPlayer();
	if( pkPlayer == NULL )
		return false;

	SPSkillArchive*	pSkillArchive	=	pkPlayer->GetSkillArchive();
	if( pSkillArchive == NULL )
		return false;

	SPSkillActivity* pkSkillActivity = pSkillArchive->GetSkillActivity( iSkillID );
	if( pkSkillActivity == NULL )
		return false;

	SPSkill* pkSkill = pkSkillActivity->GetSkill();
	if( pkSkill == NULL )
		return false;

	int	iSkillLevel		= pkSkillActivity->GetLevel();
	int	iSkillMaxLevel	= pkSkill->GetMaxLevel();
	SPSkillUpRequire*	pkUpRequire = pkSkill->GetUpRequre();

	int iUpSkillPoint = pkUpRequire->m_iSkillPoint + pkUpRequire->m_iSkillPointSlv * iSkillLevel;

	if( iSkillLevel >= iSkillMaxLevel )
		return false;

	if( CheckPCStatus( CHECK_STATUS_SKILL_POINT , iUpSkillPoint , 0 ) == false )
		return false;

	vector< SKILL_REQUIRE_STAT >::iterator iter = pkUpRequire->m_vRequireStateList.begin();
	INT64	iValue1	, iValue2;

	while( iter != pkUpRequire->m_vRequireStateList.end() )
	{
		iValue1	=	(*iter).iValue	+ (*iter).iValueSlv		* iSkillLevel;
		iValue2	=	(*iter).iValue2	+ (*iter).iValueSlv2	* iSkillLevel;

		if( CheckCondition( (*iter).iType , (CHECK_STATUS)(*iter).iID , iValue1 , iValue2 ) == false )
			return false;

		++iter;
	}

	return true;
}

bool SPCheckManager::CheckSkillUseRequire( int iSkillID , bool bDisplayCheck /* = false  */ )
{
	//[xialin 2008/08/06] edit >>>
	SPPlayer* pkPlayer = NULL;
	if ( g_pSubGameManager->GetGameState() == GS_CASHSHOP )
	{
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_GETPLAYER, (WPARAM)&pkPlayer, 0);
	}
	else
	{
		pkPlayer = g_pGOBManager->GetLocalPlayer();
	}
	//[xialin 2008/08/06] edit <<<

	if( pkPlayer == NULL )
		return false;

	SPSkillArchive*	pSkillArchive	=	pkPlayer->GetSkillArchive();
	if( pSkillArchive == NULL )
		return false;

	SPSkillActivity* pkSkillActivity = pSkillArchive->GetSkillActivity( iSkillID );
	if( pkSkillActivity == NULL )
	{
		CONTAINER_SLOT Container = pkPlayer->GetItemSkillContainer();
		pkSkillActivity = pSkillArchive->GetItemSkillActivity(iSkillID,
			Container.ContainerType, Container.SlotIndex);
		
		if( pkSkillActivity == NULL )
			return false;
	}

	SPSkill* pkSkill = pkSkillActivity->GetSkill();
	if( pkSkill == NULL )
		return false;

	//	스킬 레벨 체크
	int	iSkillLevel	= pkSkillActivity->GetLevel();
	if( iSkillLevel == 0 )
		return false;

	SPSkillActivityRequire* pkActivityRequire	=	pkSkill->GetActivityRequire();
	if( pkActivityRequire == NULL )
		return false;

	//	장착 아이템 체크
	if( pkActivityRequire->m_iEquipType )
	{
		int iItemKind =	pkPlayer->GetEquipItem( (EQUIP_ID)pkActivityRequire->m_iEquipType );
		if( iItemKind )
		{
			if( pkActivityRequire->m_iItemKind != iItemKind )
				return false;
		}
		else
			return false;
	}

	//	버프 체크

	UINT64			uiCurAction		=	pkPlayer->GetCurAction();

	//	필요조건
	vector< SKILL_REQUIRE_STAT >::iterator iter = pkActivityRequire->m_vRequireStateList.begin();
	INT64	iValue1	, iValue2;

	while( iter != pkActivityRequire->m_vRequireStateList.end() )
	{
		iValue1	=	(*iter).iValue	+ (*iter).iValueSlv		* iSkillLevel;
		iValue2	=	(*iter).iValue2	+ (*iter).iValueSlv2	* iSkillLevel;

		if( CheckCondition( (*iter).iType , (CHECK_STATUS)(*iter).iID , iValue1 , iValue2 ) == false )
			return false;

		++iter;
	}

	//	여기서부터 화면용은 체크 안함.
	if( bDisplayCheck == true )
		return true;

	//	체크 순서 꼭 지킬것!!!!!!!!! AJJIYA

	//	모션 체크
	if( pkActivityRequire->m_iMotionType == 0 )
		return false;

	//	쿨타임 ID 체크
	if( SPCoolTimeManager::GetInstance()->IsExistCoolTime( pkSkill->GetCoolTimeID() ) )
		return false;

	//	콤보 체크
	SKILL_COMBO*	pSkillComboInfo	=	pkSkill->GetCombo();

	if( uiCurAction & ACTION_SKILL )
	{
		if( SPComboManager::GetInstance()->CanConnection( pSkillComboInfo->iSelfComboID ) == false )
			return false;
	}
	else
	{
		if( pSkillComboInfo->bComboCheck )
			return false;
	}

	if( uiCurAction == ACTION_STOP )
	{
		if( pkActivityRequire->m_iStandCheck == 0 )
			return false;
	}
	else
	{	
		if( (uiCurAction & ACTION_MOVE_JUMP) &&
			!(pkActivityRequire->m_iMotionType & ACTION_MOVE_JUMP) )
			return false;

		if( (uiCurAction & ACTION_MOVE_DROP) &&
			!(pkActivityRequire->m_iMotionType & ACTION_MOVE_DROP) )
			return false;

		if( uiCurAction & ACTION_ATTACK_SPEEDY ||
			uiCurAction & ACTION_ATTACK_MIGHTY )
		{
			uiCurAction &= ~ACTION_MOVE_LEFT;
			uiCurAction &= ~ACTION_MOVE_RIGHT;
			uiCurAction &= ~ACTION_RUN_LEFT;
			uiCurAction &= ~ACTION_RUN_RIGHT;
		}

		if( !(uiCurAction & pkActivityRequire->m_iMotionType) && !(uiCurAction & ACTION_SKILL) )
			return false;
	}

	return true;
}

bool SPCheckManager::CheckItemSkillUseRequire( int iSkillID , int iItemID , CONTAINER_TYPE eContainerType , int iSlotIndex , bool bDisplayCheck /* = false  */ )
{
	SPPlayer* pkPlayer = g_pGOBManager->GetLocalPlayer();
	if( pkPlayer == NULL )
		return false;

	SPSkillArchive*	pSkillArchive	=	pkPlayer->GetSkillArchive();
	if( pSkillArchive == NULL )
		return false;

	SPItemSkillActivity*	pkItemSkillActivity	=	pSkillArchive->GetItemSkillActivity( iSkillID , eContainerType , iSlotIndex );
	if( pkItemSkillActivity == NULL )
		return false;

	SPSkill* pkSkill = pkItemSkillActivity->GetSkill();
	if( pkSkill == NULL )
		return false;

	SPPlayerInvenArchive*	pLocalPlayerInvenArchive	=	pkPlayer->GetInvenArchive();
	if( pLocalPlayerInvenArchive == NULL )
		return false;

	SPItem*	pNowEquip	=	pLocalPlayerInvenArchive->GetInventoryItem( eContainerType , iSlotIndex );
	if( pNowEquip == NULL )
		return false;

	//	아이템 장착 유무 체크
	if( pNowEquip->GetItemStatusValue( MV_ITEM_GETUID ) != iItemID )
		return false;

	//	스킬 필요 SP 체크
	SPItemAttr*		pNowAttr	=	pNowEquip->GetItemAttr();
	SPItemStatus*	pNowStatus	=	pNowEquip->GetItemStatus();
	if( pNowAttr == NULL || pNowStatus == NULL )
		return false;

	int	iConsumeSP	=	-1;

	for( int i = 0 ; i < MAX_ITEM_SKILL ; ++i )
	{
		if( pNowAttr->m_iSkillID[ i ] == iSkillID )
		{
			iConsumeSP	=	pNowAttr->m_iConsumeSP[ i ];
			break;
		}
	}

	if( iConsumeSP == -1 )
		return false;

	if( pNowStatus->GetSp() < iConsumeSP )
		return false;

	//	스킬 레벨 체크
	int	iSkillLevel	= pkItemSkillActivity->GetLevel();
	if( iSkillLevel == 0 )
		return false;

	SPSkillActivityRequire* pkActivityRequire	=	pkSkill->GetActivityRequire();
	if( pkActivityRequire == NULL )
		return false;

	//	장착 아이템 체크
	if( pkActivityRequire->m_iEquipType )
	{
		int iItemKind =	pkPlayer->GetEquipItem( (EQUIP_ID)pkActivityRequire->m_iEquipType );
		if( iItemKind )
		{
			if( pkActivityRequire->m_iItemKind != iItemKind )
				return false;
		}
		else
			return false;
	}

	//	버프 체크

	UINT64			uiCurAction		=	pkPlayer->GetCurAction();

	//	필요조건
	vector< SKILL_REQUIRE_STAT >::iterator iter = pkActivityRequire->m_vRequireStateList.begin();
	INT64	iValue1	, iValue2;

	while( iter != pkActivityRequire->m_vRequireStateList.end() )
	{
		iValue1	=	(*iter).iValue	+ (*iter).iValueSlv		* iSkillLevel;
		iValue2	=	(*iter).iValue2	+ (*iter).iValueSlv2	* iSkillLevel;

		if( CheckCondition( (*iter).iType , (CHECK_STATUS)(*iter).iID , iValue1 , iValue2 ) == false )
			return false;

		++iter;
	}

	//	여기서부터 화면용은 체크 안함.
	if( bDisplayCheck == true )
		return true;

	//	체크 순서 꼭 지킬것!!!!!!!!! AJJIYA

	//	모션 체크
	if( pkActivityRequire->m_iMotionType == 0 )
		return false;

	//	쿨타임 ID 체크
	if( SPCoolTimeManager::GetInstance()->IsExistCoolTime( pkSkill->GetCoolTimeID() ) )
		return false;

	//	콤보 체크
	SKILL_COMBO*	pSkillComboInfo	=	pkSkill->GetCombo();

	if( uiCurAction & ACTION_SKILL )
	{
		if( SPComboManager::GetInstance()->CanConnection( pSkillComboInfo->iSelfComboID ) == false )
			return false;
	}
	else
	{
		if( pSkillComboInfo->bComboCheck )
			return false;
	}

	if( uiCurAction == ACTION_STOP )
	{
		if( pkActivityRequire->m_iStandCheck == 0 )
			return false;
	}
	else
	{	
		if( (uiCurAction & ACTION_MOVE_JUMP) &&
			!(pkActivityRequire->m_iMotionType & ACTION_MOVE_JUMP) )
			return false;

		if( (uiCurAction & ACTION_MOVE_DROP) &&
			!(pkActivityRequire->m_iMotionType & ACTION_MOVE_DROP) )
			return false;

		if( uiCurAction & ACTION_ATTACK_SPEEDY ||
			uiCurAction & ACTION_ATTACK_MIGHTY )
		{
			uiCurAction &= ~ACTION_MOVE_LEFT;
			uiCurAction &= ~ACTION_MOVE_RIGHT;
			uiCurAction &= ~ACTION_RUN_LEFT;
			uiCurAction &= ~ACTION_RUN_RIGHT;
		}

		if( !(uiCurAction & pkActivityRequire->m_iMotionType) && !(uiCurAction & ACTION_SKILL) )
			return false;
	}

	return true;
}

bool SPCheckManager::CheckPetSkillUseRequire(int iSkillID, int iItemID, CONTAINER_TYPE eContainerType, int iSlotIndex, bool bDisplayCheck /* = false  */)
{
	SPPlayer	*pPlayer = g_pGOBManager->GetLocalPlayer();
	if (!pPlayer) return false;

	SPSkillArchive	*pSkillArchive = pPlayer->GetSkillArchive();
	if (!pSkillArchive) return false;

	SPItemSkillActivity	*pItemSkillActivity = pSkillArchive->GetItemSkillActivity( iSkillID, eContainerType, iSlotIndex );
	if (!pItemSkillActivity) return false;

	SPSkill	*pSkill = pItemSkillActivity->GetSkill();
	if (!pSkill) return false;

	int iConsumeSP = -1;
	int iConsumeSSP = -1;

	SPPetManager *pPetMgr = pPlayer->GetPetMgr();
	if (!pPetMgr) return false;

	SPPet	*pet = pPetMgr->GetPet( (PET_PLACE)iSlotIndex );
	if (!pet) return false;
	if (!pet->IsNewGenerationPet()) return false;
	t_PetAttr_Info *petInfo = pet->GetPetAttr();
	if (!petInfo) return false;

	// if SST_PET_CHAT, SST_PET_SHOP, return true
	if (pSkill->GetSpecialType() == SST_PET_SHOP
		||
		pSkill->GetSpecialType() == SST_PET_CHAT)
		return true;

	if (pSkill->GetConsumptionState(0)->iType == 10)
	{/* use ssp */
		iConsumeSSP = pSkill->GetConsumptionState(0)->iValue;
	}
	else if (pSkill->GetConsumptionState(0)->iType == 7)
	{/* use sp */
		iConsumeSP = pSkill->GetConsumptionState(0)->iValue;
	}

	if (petInfo->sp < iConsumeSP || petInfo->ssp < iConsumeSSP)
		return false;

	return true;
}

bool SPCheckManager::CheckActionSkillUseRequire( int iSkillID )
{
	SPSkillManager*		pSkillManager		=	SPSkillManager::GetInstance();
	SPActionInterface*	pActionInterface	=	pSkillManager->GetActionInterface( iSkillID );
	if( pActionInterface == NULL )
		return false;

	SPPlayer* pkPlayer = g_pGOBManager->GetLocalPlayer();
	if( pkPlayer == NULL )
		return false;

	int	iEquipWeapon					=	pkPlayer->GetWeaponItemSeq();
	int	iMotionType						=	pActionInterface->GetMotionType();
	int	iConsumptionStatTable			=	( iEquipWeapon	*	100 )	+	iMotionType;
	SPMotionStatus*		pMotionStatus	=	pSkillManager->GetMotionStatus( iConsumptionStatTable );

	if( pMotionStatus == NULL )
		return true;

	MOTION_STATUS_STAT*	pMotionStatusType;
	int					iMotionStatusType;

	int	iRequireStatListCount			=	pMotionStatus->GetRequireStatListCount();

	for( int i = 0 ; i < iRequireStatListCount ; i++ )
	{
		pMotionStatusType	=	pMotionStatus->GetRequireStatList( i );

		if( pMotionStatusType == NULL )
			continue;

		iMotionStatusType	=	(int)pMotionStatusType->eType;

		if( iMotionStatusType == 0 )
			continue;

		if( CheckCondition( iMotionStatusType , pMotionStatusType->iID , pMotionStatusType->iValue , pMotionStatusType->iValue2 ) == false )
			return false;
	}

	return true;
}

bool SPCheckManager::CheckCondition( int iType , int iID , int iValue1 , int iValue2 )
{	
	if( iType == REQUIRE_TYPE_NULL )
		return true;

	//	Stat일경우
	if( iType == REQUIRE_TYPE_STAT )
	{
		return CheckPCStatus( (CHECK_STATUS)iID , iValue1 , iValue2 );
	}
	//	아이템일경우
	else if( iType == REQUIRE_TYPE_ITEM )
	{
		int	iItemTotalCount	=	g_pCheckManager->GetTotalPCItemCount( iValue1 );

		//	갯수체크
		if( iID == 1 )
		{
			if(iValue2 > 0 && (iValue2 <= iItemTotalCount))
				return true;	//	갯수 충족
			else
				return false;	//	갯수 불충족
		}
		// 갯수 이하로만 체크 [10/15/2007 AJJIYA]
		else if( iID == 2 )
		{
			if(iValue2 > 0 && (iValue2 > iItemTotalCount))
				return true;	//	원하는 갯수보다 작을때만 충족
			else
				return false;	//	원하는 갯수와 같거나 클때는 불충족
		}
	}
	//	스킬일경우
	else if( iType == REQUIRE_TYPE_SKILL )
	{
		//	스킬 존재 유무( 스킬이 존재할때 TRUE )
		if( iID == 1 )
		{
			return CheckPCSkill( iValue1 , 2 , 0 );
		}
		//	스킬 레벨 체크
		else if( iID == 2 )
		{
			return CheckPCSkill( iValue1 , 2 , iValue2 );
		}
		//	스킬 존재 유무( 스킬이 존재하면 FALSE )
		else if( iID == 3 )
		{
			return !CheckPCSkill( iValue1 , 2 , 0 );
		}

	}
	//	퀘스트일경우
	else if( iType == REQUIRE_TYPE_QUEST )
	{
		//	반복 횟수 이하
		if( iID == 1)
		{
			return CheckPCQuest( iValue1 , 1 , iValue2 );
		}
		//	반복 횟수 초과
		else if( iID == 2)
		{
			return CheckPCQuest( iValue1 , 2 , iValue2 );
		}
		else if( iID == 3 || iID == 4)		//퀘스트 중인지에 관련한 확인
		{
			return CheckDoingQuest(iID, iValue1);
		}
	}
	// Title
	else if( iType == REQUIRE_TYPE_TITLE )
	{
		// 타이틀이 있는지 확인
		if( iID == 1)
		{
			return CheckPCTitle( iValue1, 0, 0 );
		}
		// 타일틀이 없는지 확인
		else if( iID == 2)
		{
			return !CheckPCTitle( iValue1, 0, 0 );
		}
	}
	// Map
	else if( iType == REQUIRE_TYPE_MAP )
	{		
		if(g_pGOBManager->GetLocalPlayer() == NULL)
			return false;
		
		int iStageID = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
		int iMapGroupID = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();
		SPMapGroup* pMapGroup = g_StageManager.GetMapGroup(iMapGroupID, iStageID);
		
		// 맵에 있어야 하는경우
		if( iID == 1)
		{
			if(pMapGroup && pMapGroup->IsMapGroupType(iValue1)){
				return true;
			}
			return false;
		}
		// 맵에 없어야 하는경우
		if( iID ==  2)
		{
			if(pMapGroup && pMapGroup->IsMapGroupType(iValue1) == false){
				return true;
			}
			return false;
		}
	}
	//Effect
	else if( iType == REQUIRE_TYPE_EFFECT )
	{		
		if(g_pGOBManager->GetLocalPlayer() == NULL)
			return false;

		// 이펙트가 있는지 확인
		if( iID == 1)
		{
			return CheckPCEffect( iValue1, 0, 0 );
		}
		// 이펙트가 없는지 확인
		else if( iID == 2)
		{
			return !CheckPCEffect( iValue1, 0, 0 );
		}
	}
	// Guild
	else if( iType == REQUIRE_TYPE_GUILD )
	{
		if(g_pGOBManager->GetLocalPlayer() == NULL)
			return false;

		if( iID == 1 )			// 길드 레벨
		{
			return CheckPCGuildLevel(iValue1, iValue1, iValue2) ;
		}
		else if ( iID == 2 )	// 길드 타입
		{
			return CheckPCGuildType(iValue1, 0, 0) ;
		}
	}
	// 강제 이벤트
	else if( iType == REQUIRE_TYPE_COREEVENT )
	{
		if(g_pGOBManager->GetLocalPlayer() == NULL)
			return false;

		iValue1 -= 1;		//내부 데이터가 0베이스이기때문에 보정해줘야 한다. 
		
		if(iID == 1) {		// true인지 체크 
			return CheckPCCoreEvent(iID, iValue1);
		}
		else if(iID == 2) {		// false인지 체크
			return CheckPCCoreEvent(iID, iValue1);
		}
	}
	// 장비 여부 체크 
	else if( iType == REQUIRE_TYPE_EQUIP )
	{
		if(g_pGOBManager->GetLocalPlayer() == NULL)
			return false;

		return CheckEquip(iID, iValue1);
	}
	//
	else if( iType == REQUIRE_TYPE_EQUIP_MOTION )
	{
		if(g_pGOBManager->GetLocalPlayer() == NULL)
			return false;

		return CheckEquipMotion(iID, iValue1, iValue2);
	}

	return false;
}

bool SPCheckManager::CheckApplyEffectRequire(int iEffectID, SPGameObject* pkGameObject)
{
	SPEffect* pkEffect = SPEffectManager::GetInstance()->GetEffect(iEffectID);
	if( pkEffect == NULL || pkGameObject == NULL )
		return false;

	if( pkGameObject->GetClassID() != CLASS_ID_AVATAR )
		return true;

	SPPlayer* pkPlayer = (SPPlayer*)pkGameObject;
	if( pkPlayer == NULL )
		return false;

	EFFECT_REQUIRE* pEffectRequire = pkEffect->GetRequireInfo();
	
	if( pEffectRequire->iEquipType )
	{
		int iItemKind =	pkPlayer->GetEquipItem( (EQUIP_ID)pEffectRequire->iEquipType );
		if( iItemKind )
		{
			if( pEffectRequire->iItemKind != iItemKind )
				return false;
		}
		else
			return false;
	}

	UINT64 uiCurAction = pkPlayer->GetCurAction();
	if( uiCurAction == ACTION_STOP )
	{
		if( pEffectRequire->iStandCheck == 0 )
			return false;
	}
	else if( pEffectRequire->iMotionType )
	{
		if( !(uiCurAction & pEffectRequire->iMotionType) )
			return false;
	}

	return true;
}


void SPCheckManager::SetPetShopOpen(bool bOpen)
{
	m_bPetShopOpen = bOpen;
}


bool SPCheckManager::IsPetShopOpen()
{
	return m_bPetShopOpen;
}


PLAYER_STATE SPCheckManager::CheckPlayerState()
{	
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return PLAYER_STATE_NOT;	

	if(g_pGOBManager->GetLocalPlayer()->IsDead())
		return PLAYER_STATE_DEAD;

	if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE)
		return PLAYER_STATE_TRADE;

	if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP) 	//개설자
		return PLAYER_STATE_SHOPOPEN;

	if(g_pGOBManager->GetLocalPlayer()->GetUserShopID() != 0)
		return PLAYER_STATE_SHOPIN;

	if(g_pEventManager->GetCutInState() > CUT_IN_STATE_NULL)
		return PLAYER_STATE_CUTIN;

	
	return PLAYER_STATE_NULL;
}


int	SPCheckManager::GetDeathItemID()
{
	return g_pEventManager->GetEventArchive()->GetDeathItem();
}


bool SPCheckManager::CheckDeathItem()
{
	int iDeathItem = GetDeathItemID();
	if(iDeathItem) {
		if(GetTotalPCItemCount(iDeathItem) > 0)
			return true;
	}

	return false;
}


int	SPCheckManager::GetMicServerItemID()
{
	return g_pEventManager->GetEventArchive()->GetMicServerItem();
}


int	SPCheckManager::GetMicChannelItemID()
{
	return g_pEventManager->GetEventArchive()->GetMicChannelItem();
}


int	SPCheckManager::GetMailFee()
{
	return g_pEventManager->GetEventArchive()->GetMailFee();
}


int SPCheckManager::GetMailStampItemID()
{
	return g_pEventManager->GetEventArchive()->GetMailStampItem();
}


/**
	입찰 가능여부 체크
*/
bool SPCheckManager::CheckBidEnable(USERID iBidUser)
{
	SPPlayer* pPlayer = NULL ;
	pPlayer = g_pGOBManager->GetLocalPlayer() ;

	if( iBidUser == g_pNetworkManager->GetUserID() )		// 본인입찰
	{
		return false ;
	}
	return true ;
}

bool SPCheckManager::CheckBidEnable(const char* pSellerName)
{
	TCHAR szName[LEN_NAME+1] ;
	SPPlayer* pPlayer = NULL ;

	pPlayer = g_pGOBManager->GetLocalPlayer() ;
	if( pPlayer == NULL )	
		return false ;
	pPlayer->SPGOBPerformMessage( SPGM_GETGOBNAME, (LPARAM)szName );
	if( strcmp(pSellerName, szName) == 0 )		// 본인 물품
	{
		return false ;
	}
	return true ;
}

/**
	숫자에 대한 자릿수 체크
 */
int	SPCheckManager::CheckDigitSize(INT64 iNumber)
{
	INT64 iDenominator = 1 ;
	int iSize = 0 ;
	if( iNumber < 1 )
		return 0 ;

	while( iNumber/iDenominator )	{
		iDenominator *= 10 ;
		++iSize ;
	}
	return iSize ;
}


/**
캐릭터 이름 비교
@param const char* pSrcCharName : 비교할 대상 원본 ( 로컬 캐릭터 이름 )
@param const char* pDestCharName : 비교할 대상 타겟 ( 입력으로 입력받은 문자열 )
@param bool bLocalName : true로 세팅하면 pSrcCharName 의 입력을 무시하고 로컬 플레이어의 이름으로 세팅한다.
@param bool bNoCase : 대소문자 구분 비교 체크 ture일때 작동
*/
bool SPCheckManager::IsCharNameCompare( const char* pSrcCharName , const char* pDestCharName , bool bLocalName , bool bNoCase )
{
	if( bLocalName == true )
	{
		if( g_pGOBManager == NULL )
			return false;

		SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer == NULL )
			return false;

		SPGOBStatus*	pLocalPlayerStatus	=	pLocalPlayer->GetStatus();

		if( pLocalPlayerStatus == NULL )
			return false;

		pSrcCharName	=	pLocalPlayerStatus->GetGOBName();
	}

	if( pSrcCharName == NULL || pDestCharName == NULL )
		return false;

	if( strlen( pSrcCharName ) <= 0 || strlen( pDestCharName ) <= 0 )
		return false;

	if( bNoCase == true )
	{
		if( strcmpi( pSrcCharName , pDestCharName ) == 0 )
			return  true;
	}

	if( strcmp( pSrcCharName , pDestCharName ) == 0 )
		return true;

	return false;
}


int	SPCheckManager::GetClass(INT64 iStatValue)
{
	if(iStatValue) {
		int iResult = 1;		
		iResult = iResult << (iStatValue - 1);
		return iResult;
	}
	
	
	/*
	switch(iStatValue) {
		case 1:								//공격형
			iResult = 1;
			break;
		case 2:								//방어형
			iResult = 2;
			break;
		case 3:								//마법형
			iResult = 4;
			break;
		case 4:								//탐색형
			iResult = 8;
			break;
		default :
			return 0;
	}
	*/

	return 0;
}



