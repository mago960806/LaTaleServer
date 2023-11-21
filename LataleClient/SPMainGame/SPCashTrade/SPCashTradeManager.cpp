#include "SPCommon.h"
#include "SPUtil.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPMouseCursor.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPNoticeBox.h"
#include "SPInterfaceManager.h"

#include "Packet.h"

//
#include "SPCommandConvert.h"
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
#include "SPGOBManager.h"

#include "SPEventDEF.h"
#include "SPEventNpcHandler.h"
#include "SPEventManager.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPLocalizeManager.h"

#include "SPCheckManager.h"

#include "SPHotItemInfo.h"
#include "SPHotItemManager.h"
#include "SPCashTradeManager.h"

#include "SPDebug.h"

#include "SPWindowBeExact.h"


SPCashTradeManager* SPCashTradeManager::m_pInstance = NULL;
SPCashTradeManager* g_pCashTradeManager = NULL;

SPCashTradeManager* SPCashTradeManager::GetInstance()
{
	if(m_pInstance == NULL) {
		m_pInstance = new SPCashTradeManager;
		g_pCashTradeManager = m_pInstance;
	}

	return m_pInstance;
}


void SPCashTradeManager::Release()
{
	if(m_pInstance) {
		delete m_pInstance;
		m_pInstance = NULL;
		g_pCashTradeManager = NULL;
	}	
}



SPCashTradeManager::SPCashTradeManager() 
: m_pHotItemManager (NULL)
, m_pSelectCashItem (NULL)
, m_bSocketFail (false)
, m_bMinorLoad (false)
, m_bHotListLoad (false)
{	
	//Init();	
	m_pHotItemManager = new SPHotItemManager;
}


SPCashTradeManager::~SPCashTradeManager()
{
	SAFE_DELETE(m_pHotItemManager);
}


void SPCashTradeManager::Init()
{
	Reset();
	LoadCashShopVersionLDT();
}


void SPCashTradeManager::Reset()
{
	m_iCashTradePlace		= CASH_TRADE_PLACE_NULL;
	m_iCashTradeState		= CASH_TRADE_STATE_NULL;
	m_iCashTradeType		= CASH_TRADE_TYPE_NULL;
}


bool SPCashTradeManager::LoadCashShopVersionLDT()
{
	SPLDTFile* pLDTFile;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_GLOBAL_SYS, &pLDTFile);	

	if(pLDTFile == NULL) {
		return false;
	}

	int iRecordCount = pLDTFile->GetItemCount();
	int iItemID = 0;

	LDT_Field ldtField;

	int iCashMajor;
	int iCashMinor;
	int iCashHot;
	pLDTFile->GetFieldFromLable(21, "_Check",	ldtField);
	iCashMajor = ldtField.uData.lValue;

	pLDTFile->GetFieldFromLable(22, "_Check",	ldtField);
	iCashMinor = ldtField.uData.lValue;

	pLDTFile->GetFieldFromLable(23, "_Check",	ldtField);
	iCashHot = ldtField.uData.lValue;

	g_pItemCluster->SetCashVersion(iCashMajor, -1);

	//SPCashTradeManager::GetInstance()->GetCashVerInfo().SetVerInfo(iCashMajor, iCashMinor, iCashHot);
	
	m_CashShopVerInfo.SetVerInfo(iCashMajor, iCashMinor, iCashHot);
	
	SAFE_RELEASE(pLDTFile);
	
	return true;
}


CASH_TRADE_PLACE SPCashTradeManager::GetCashTradePlace()
{
	return m_iCashTradePlace;
}


CASH_TRADE_STATE SPCashTradeManager::GetCashTradeState()
{
	return m_iCashTradeState;
}


CASH_TRADE_TYPE	 SPCashTradeManager::GetCashTradeType()
{
	return m_iCashTradeType;
}


void SPCashTradeManager::SetVerInfo(int iMajor, int iMinor, int iHotVer) {
	m_CashShopVerInfo.SetVerInfo(iMajor, iMinor, iHotVer);	
}


void SPCashTradeManager::SetMajorVer(int iMajor) {
	m_CashShopVerInfo.SetMajorVer(iMajor);
}


void SPCashTradeManager::SetMinorVer(int iMinor) {
	m_CashShopVerInfo.SetMinorVer(iMinor);
}


void SPCashTradeManager::SetHotVer(int iHotVer) {
	m_CashShopVerInfo.SetHotVer(iHotVer);
}


void SPCashTradeManager::SetSocketFail()
{
	m_bSocketFail		= true;
	m_bMinorLoad		= true;
	m_bHotListLoad		= true;	
}


bool SPCashTradeManager::SetSelectCashItem(int iShopID, bool bShowSelectWnd /* = true */)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL)					//컷인상태 제한
			return false;

		if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE)	//트레이드시 키 막음
			return false;
		
		if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP)		//내가 상점개설자
			return false;
		
		if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_MOVE_JUMP)		//점프중
			return false;
		
		if(g_pGOBManager->GetLocalPlayer()->IsDead())								//사망인 경우
			return false;
		
		if(g_pGOBManager->GetLocalPlayer()->GetUserShopID())						//내가 상점구매자
			return false;

		//캐시샵 진입이 안되는 맵 관련 처리
		SPStage* pStage = g_StageManager.GetCurStage(g_pGOBManager->GetLocalPlayer()->GetCurStageID());
		if(pStage) {
			SPMapGroup* pMapGroup = pStage->GetMapGroup(g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID());
			if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_CASHSHOP) &&
				pMapGroup && pMapGroup->IsMapGroupType(GROUP_TYPE_CASH)) {
					//g_pInterfaceManager->SPChildWndSendMessage(WIID_MENU, SPIM_SET_CASH_ENABLE, 1, 0);
					;
				}
			else {
				return false;
			}
		}
	}	
	
	SPWindow* pWindow = NULL;
	if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {			
		pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE_SELECT);
		if(pWindow && pWindow->IsShow()) {
			if(m_pSelectCashItem && m_pSelectCashItem->iCashID == iShopID) {
				g_pInterfaceManager->SetFocusWindow(pWindow);
				return false;
			}
		}		
	}

	if(m_iCashTradeState ==	CASH_TRADE_STATE_WAIT)
		return false;
	
	m_pSelectCashItem = g_pItemCluster->GetMinorCashItem(iShopID);
	if(m_pSelectCashItem) {
		if(m_pSelectCashItem->bView == false)
			return false;

		m_iCashTradeState = CASH_TRADE_STATE_NULL;
		
		if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
			pWindow = NULL;
			pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE);
			if(pWindow) {
				pWindow->Hide();
			}

			pWindow = NULL;
			pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_GIFT);
			if(pWindow) {
				pWindow->Hide();
			}
			
			pWindow = NULL;
			pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE_SELECT);
			if(pWindow) {
				g_pInterfaceManager->SetFocusWindow(pWindow);
				if (bShowSelectWnd)
					pWindow->Show();
			}
		}
		return true;
	}

	return false;
}


SPCashItemAttr* SPCashTradeManager::GetSelectCashItem()
{
	return m_pSelectCashItem;
}


bool SPCashTradeManager::ClearSelectCashItem()
{
	if(m_pSelectCashItem) {
		m_pSelectCashItem = NULL;
		return true;
	}

	return false;
}


void SPCashTradeManager::SetBuyPacketReceive()
{
	m_bBuyPacketReceive = true;

	ShowResultAction();
}


void SPCashTradeManager::SetWaitAniComplete()
{
	m_bWaitAniComplete = true;

	ShowResultAction();
}


bool SPCashTradeManager::PleaseBuyMoney()
{
	if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_CASHSHOP);
		if(pWindow) {
			pWindow->SPSendMessage(SPIM_CASHSHOP_WEB_PAGE, (WPARAM)0, LPARAM(0), NULL);
		}
	}
	return true;
}


bool SPCashTradeManager::IsEnableInventory()
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pInven == NULL )
		return false;
	
	if(m_pSelectCashItem == NULL || m_pSelectCashItem->iCashID == 0 || m_pSelectCashItem->eCategory == CI_NULL)
		return false;
	
	int iRequireSlot[FIGURE_EQUIP] = {0, 0, 0, 0, 0, 0};		//Inventroy별 size 6
	
	if(m_pSelectCashItem->eCategory == CI_PACKAGE || m_pSelectCashItem->eCategory == CI_POINT)
	{
		CONTAINER_TYPE iContainer = CONTAINER_TYPE_INVALID;
		SPItemAttr* pItem = NULL;
		int iItemID = 0;
		int iStack = 0;
		
		for(int i = 0; i < CASHITEMINFO_COUNT; i++) {
			iContainer = CONTAINER_TYPE_INVALID;
			pItem = NULL;
			iItemID = m_pSelectCashItem->ItemInfo[i].iID;
			iStack = m_pSelectCashItem->ItemInfo[i].iCount;
			
			if(iItemID) {
				g_pItemCluster->GetItemInfo(iItemID, pItem);
				if(pItem) {
					iContainer = (CONTAINER_TYPE)(pItem->m_eType - 1);
					iRequireSlot[iContainer]++;
				}
			}
		}

		for(i = 0; i < FIGURE_EQUIP; i++) {
			if(iRequireSlot[i] == 0) 
				continue;

			if(false == g_pCheckManager->CheckRequireSlot(i, iRequireSlot[i])) {
				//i번째 인벤토리 공간 부족
				SetInvenFullMsg(i);
				return false;
			}
		}

		// 추가적인 Stack 체크
		for(int i = 0; i < CASHITEMINFO_COUNT; i++) {
			iContainer = CONTAINER_TYPE_INVALID;
			pItem = NULL;
			iItemID = m_pSelectCashItem->ItemInfo[i].iID;
			iStack = m_pSelectCashItem->ItemInfo[i].iCount;

			if(iItemID) {
				if(false == g_pCheckManager->CheckEnableSlot(iItemID, iStack)) {
					//i번째 인벤토리 공간 부족
					CONTAINER_TYPE iContainer = CONTAINER_TYPE_INVALID;
					SPItemAttr* pItem = NULL;
					g_pItemCluster->GetItemInfo(iItemID, pItem);
					if(pItem == NULL)
						return false;

					iContainer = (CONTAINER_TYPE)(pItem->m_eType - 1);
					SetInvenFullMsg(iContainer);
					return false;
				}
			}
		}

		return true;
	}	
	else {
		int iItemID = m_pSelectCashItem->ItemInfo[0].iID;
		int iStack = m_pSelectCashItem->ItemInfo[0].iCount;
		
		SPItemAttr* pItem = NULL;
		g_pItemCluster->GetItemInfo(iItemID, pItem);
		if(pItem == NULL)
			return false;

		CONTAINER_TYPE iContainer = CONTAINER_TYPE_INVALID;
		iContainer = (CONTAINER_TYPE)(pItem->m_eType - 1);

		if(false == g_pCheckManager->CheckEnableSlot(iItemID, iStack)) {
			//iReturn = QUEST_RETURN_F_INVEN;
			//SetReturnMsg(iReturn);
			SetInvenFullMsg(iContainer);
			return false;
		}

		return true;
	}

	return true;
}


void SPCashTradeManager::SetInvenFullMsg(int i)
{
	CONTAINER_TYPE iContainer = (CONTAINER_TYPE)i;	
	int iStringID = 0;
	switch(iContainer) {
		case INVENTORY_EQUIP:
			iStringID = 13001011;
			break;
		case INVENTORY_CONSUME:
			iStringID = 13001012;
			break;
		case INVENTORY_ETC:
			iStringID = 13001013;
			break;
		case INVENTORY_EVENT:
			iStringID = 13001014;
			break;
		case INVENTORY_PET:
			iStringID = 13001015;
			break;
		case INVENTORY_HOUSING:
			iStringID = 13001016;
			break;
		default:
			return;
			break;
	}

	if(g_pResourceManager->GetGlobalString(iStringID) != NULL) {
		std::string strMsg = g_pResourceManager->GetGlobalString(iStringID);

		if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str());
		}
	}

	return;
}


bool SPCashTradeManager::IsSendCheck()
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	//사망시 제한
	if(g_pGOBManager->GetLocalPlayer()->IsDead())
		return false;

	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL){					//컷인상태 제한
		if(g_pEventManager->GetCutInState() != CUT_IN_STATE_BEAUTY)
			return false;
	}

	if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE)	//트레이드 중 제한
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP)		//내가 개인 상점개설한 경우
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetUserShopID())						//내가 타인의 개인상점에 들어간 경우
		return false;
	
	return true;
}


bool SPCashTradeManager::SendCashShopVersion()
{
	//CASHSHOP_CS_VERSION
	//// int					iMajor;  				//캐쉬샵 Major버전
	//// int					iMinor;  				//캐쉬샵 Minor버전
	//// int					iHotVer;				//추천상품 버전

	if(m_bSocketFail) {
		m_bMinorLoad = true;
		m_bHotListLoad = true;
		return false;
	}
	
	CPacket Packet;
	Packet.Add((UINT32)CASHSHOP_CS_VERSION);
	Packet.AddUINT32(m_CashShopVerInfo.m_iShopMajor);
	Packet.AddUINT32(m_CashShopVerInfo.m_iShopMinor);
	Packet.AddUINT32(m_CashShopVerInfo.m_iHotListVer);

//#ifdef __FIELD_CASH_TRADE__
	//이부분 리턴값 문제 있을수 있음...
//	g_pNetworkManager->SPPerformMessage(CASHSHOP_CS_VERSION, 0, (LPARAM)&Packet);		//[2008/4/16] - 임시 제한
//	m_bMinorLoad = false;
//	m_bHotListLoad = false;
//#else	
	//[2008/4/16] - 이하 테스트용
	m_bMinorLoad = true;
	m_bHotListLoad = true;
	
	////
	//CASHSHOP_HOT_INFO			stHotInfos;
	//ZeroMemory(&stHotInfos, sizeof(CASHSHOP_HOT_INFO));
	//stHotInfos.iShopID = 201;
	//stHotInfos.iDescLength = 10;
	//sprintf(stHotInfos.strDesc, "11111111111");
	//m_pHotItemManager->AddHotItem(&stHotInfos);

	//ZeroMemory(&stHotInfos, sizeof(CASHSHOP_HOT_INFO));
	//stHotInfos.iShopID = 20092;
	//stHotInfos.iDescLength = 10;
	//sprintf(stHotInfos.strDesc, "2222222222");
	//m_pHotItemManager->AddHotItem(&stHotInfos);

	//ZeroMemory(&stHotInfos, sizeof(CASHSHOP_HOT_INFO));
	//stHotInfos.iShopID = 310016;
	//stHotInfos.iDescLength = 10;
	//sprintf(stHotInfos.strDesc, "3333333333");
	//m_pHotItemManager->AddHotItem(&stHotInfos);
//#endif	

	return true;
}


bool SPCashTradeManager::OnCashShopVersion(CPacket* pPacket)
{
	if( pPacket == NULL )
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	//CASHSHOP_SC_VERSION,
	GLOBAL_STRING_ID			iErrorMsg;
	GU_ID						globalID;
	UINT32						iMinor;  				//캐쉬샵 Minor버전
	UINT32						iHotVer;				//추천상품 버전

	pPacket->ExtractUINT32(&iErrorMsg);
	if(iErrorMsg) // 실패
	{
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(iErrorMsg)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iErrorMsg);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
		g_pCheckManager->SetDBBlock(false);				

		m_bMinorLoad = true;
		m_bHotListLoad = true;
		return false;
	}	

	pPacket->ExtractUINT64(&globalID);
	if(g_pGOBManager->GetLocalPlayer() && iErrorMsg == 0) {			//[2008/6/4] - 에러가 아닌 경우에만 체크
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != globalID) {
			m_bMinorLoad = true;
			m_bHotListLoad = true;
			return false;
		}
	}

	pPacket->ExtractUINT32(&iMinor);
	if(m_CashShopVerInfo.m_iShopMinor != iMinor) {
		;	
	}
	m_bMinorLoad = true;
	m_CashShopVerInfo.SetMinorVer(iMinor);

	pPacket->ExtractUINT32(&iHotVer);
	if(m_CashShopVerInfo.m_iHotListVer != iHotVer) {
		;
	}
	m_bHotListLoad = true;
	m_CashShopVerInfo.SetHotVer(iHotVer);
	
	return true;
}


bool SPCashTradeManager::SendCashShopPrice(CASH_TRADE_TYPE iTradeType)
{
	if(m_pSelectCashItem == NULL || m_pSelectCashItem->iCashID == 0)
		return false;

	if(iTradeType == CASH_TRADE_TYPE_NULL)
		return false;

	m_iCashTradeType = iTradeType;

	//CASHSHOP_CS_PRICE,
	//// int					iHotVer;				//추천상품 버전
	//// int					iPackageID;				//CASH_SHOP.LDT::ItemID
	//// int					iShopPrice;				//CASH_SHOP.LDT::_CashPrice
	//// int					iSalePrice;				//CASH_SHOP.LDT::_SalePrice
	//// int					iMaxOz;					//CASH_SHOP.LDT::_MaxOz
	//// int					iMaxMileage;			//CASH_SHOP.LDT::_MaxMileage
	
	if(m_pSelectCashItem) {
		CPacket Packet;
		Packet.Add((UINT32)CASHSHOP_CS_PRICE);
		Packet.AddUINT32(m_CashShopVerInfo.m_iHotListVer);
		Packet.AddUINT32(m_pSelectCashItem->iCashID);
		Packet.AddUINT32(m_pSelectCashItem->iPrice);
		Packet.AddUINT32(m_pSelectCashItem->iSalePrice);
		Packet.AddUINT32(m_pSelectCashItem->iMaxOZ);
		Packet.AddUINT32(m_pSelectCashItem->iMaxMile);

		g_pNetworkManager->SPPerformMessage(CASHSHOP_CS_PRICE, 0, (LPARAM)&Packet);
	}	
	
	return true;
}


bool SPCashTradeManager::OnCashShopPrice(CPacket* pPacket)
{
	if( pPacket == NULL )
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	//CASHSHOP_SC_PRICE,
	GLOBAL_STRING_ID	iErrorMsg;
	GU_ID				globalID;
	UINT32				iHotVer;				//추천상품 버전
	UINT32				iPackageID;				//CASH_SHOP.LDT::ItemID 
	UINT32				iShopPrice;				//CASH_SHOP.LDT::_CashPrice
	UINT32				iSalePrice;				//CASH_SHOP.LDT::_SalePrice
	UINT32				iMaxOz;					//CASH_SHOP.LDT::_MaxOz
	UINT32				iMaxMileage;			//CASH_SHOP.LDT::_MaxMileage

	pPacket->ExtractUINT32(&iErrorMsg);
	if(iErrorMsg) // 실패
	{
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(iErrorMsg)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iErrorMsg);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
		g_pCheckManager->SetDBBlock(false);
		return false;
	}

	pPacket->ExtractUINT64(&globalID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != globalID) {
			return false;
		}
	}

	pPacket->ExtractUINT32(&iHotVer);
	if(m_CashShopVerInfo.m_iHotListVer != iHotVer) {
		;
	}
	//m_bHotListLoad = true;
	m_CashShopVerInfo.SetHotVer(iHotVer);

	pPacket->ExtractUINT32(&iPackageID);
	if(m_pSelectCashItem && m_pSelectCashItem->iCashID == iPackageID) {
		bool bShowNotice = false;		
		pPacket->ExtractUINT32(&iShopPrice);
		pPacket->ExtractUINT32(&iSalePrice);
		pPacket->ExtractUINT32(&iMaxOz);
		pPacket->ExtractUINT32(&iMaxMileage);
		
		if(m_pSelectCashItem->iPrice != iShopPrice || m_pSelectCashItem->iSalePrice != iSalePrice ||
			m_pSelectCashItem->iMaxOZ != iMaxOz || m_pSelectCashItem->iMaxMile != iMaxMileage) 
		{
			bShowNotice = true;
		}
		
		m_pSelectCashItem->iPrice = iShopPrice;
		m_pSelectCashItem->iSalePrice = iSalePrice;
		m_pSelectCashItem->iMaxOZ = iMaxOz;
		m_pSelectCashItem->iMaxMile = iMaxMileage;

		if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
			SPWindow* pWindow = NULL;
			pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE_SELECT);
			if(pWindow) {
				pWindow->Hide();
			}

			if(m_iCashTradeType == CASH_TRADE_TYPE_BUY) {
				pWindow = NULL;
				pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE);
				if(pWindow) {
					g_pInterfaceManager->SetFocusWindow(pWindow);
					pWindow->Show();
				}
			}
			else if(m_iCashTradeType == CASH_TRADE_TYPE_GIFT) {
				pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_GIFT);
				if(pWindow) {
					g_pInterfaceManager->SetFocusWindow(pWindow);
					pWindow->Show();
				}
			}
		}

		// 결재 금액 정보가 갱신 된 경우
		if(bShowNotice) {
			if(g_pInterfaceManager) {
				if(g_pResourceManager->GetGlobalString(9001012) != NULL) {
					//g_pInterfaceManager->SetNoticeMsg(g_pResourceManager->GetGlobalString(9001012), (SPWindow*)NULL);
					g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(9001012), (SPWindow*)NULL, NOTICE_TYPE_YES);
				}
			}
		}

		return true;
	}
	else {
		// 해당 아이템은 구매할수 없습니다. 
		if(g_pResourceManager->GetGlobalString(9001014) != NULL) {
			std::string strMsg = g_pResourceManager->GetGlobalString(9001014);
			if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str());
			}
		}
	}

	return false;
}


bool SPCashTradeManager::OnCashShopHotItemUpdate(CPacket* pPacket)
{
	if( pPacket == NULL )
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	GLOBAL_STRING_ID			iErrorMsg;
	GU_ID						globalID;
	UINT16						nHotInfo = 0;
	CASHSHOP_HOT_INFO			stHotInfos;
	
	pPacket->ExtractUINT32(&iErrorMsg);
	if(iErrorMsg) // 실패
	{
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(iErrorMsg)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iErrorMsg);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
		g_pCheckManager->SetDBBlock(false);
		return false;
	}

	pPacket->ExtractUINT64(&globalID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != globalID) {
			return false;
		}
	}

	m_pHotItemManager->DeleteAllHotItem();

	pPacket->ExtractUINT16(&nHotInfo);
	for(int i = 0; i < nHotInfo; i++) {
		ZeroMemory(&stHotInfos, sizeof(CASHSHOP_HOT_INFO));
		pPacket->ExtractStruct(&stHotInfos, sizeof(CASHSHOP_HOT_INFO));	
		m_pHotItemManager->AddHotItem(&stHotInfos);
	}

	g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_CASHSORT, 0, 0);

	return true;
}


bool SPCashTradeManager::OnSetHotItemStatus(CPacket* pPacket)
{
	UINT8					iHotCashShopMode;
	pPacket->ExtractUINT8(&iHotCashShopMode);
	
	if(m_pHotItemManager) {
		if(iHotCashShopMode) {
			m_pHotItemManager->SetShowFlag(true);
		}
		else {
			m_pHotItemManager->SetShowFlag(false);			
		}

		if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
			SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_ITEM);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_ITEM_HOT_STATUS, (WPARAM)0, LPARAM(0), NULL);
			}
		}

		return true;
	}

	return false;
}


bool SPCashTradeManager::OnCashShopItemUpdate(CPacket* pPacket)
{
	//CPacket* pkPacket = reinterpret_cast< CPacket* >(lparam);
	if( pPacket == NULL )
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	GLOBAL_STRING_ID iError;
	pPacket->ExtractUINT32(&iError);
	if( iError )
	{
		g_pInterfaceManager->SPPerformMessage(
			SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(iError));
		return false;
	}

	GU_ID iGUID;
	unsigned short usNum, i;
	CASHSHOP_INFO tItemInfo;

	pPacket->ExtractUINT64(&iGUID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != iGUID) {
			return false;
		}
	}

	pPacket->ExtractUINT16(&usNum);
	for( i = 0; i < usNum; i++ )
	{
		pPacket->Extract(&tItemInfo, sizeof(CASHSHOP_INFO));
		g_pItemCluster->UpdateMinorCashItem(&tItemInfo);
	}

	g_pItemCluster->UpdateItemCashPrize();

	return true;
}


bool SPCashTradeManager::SendCashShopBuy(CASH_TRADE_PLACE iPlace, int iPackageID, 
										int iShopPrice,	int iSalePrice, int iMaxOz, int iMaxMileage, 
										int iBuyOz /*= 0*/, int iBuyMileage /*= 0*/)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	m_iCashTradePlace = iPlace;
	
	if(iPlace == CASH_TRADE_PLACE_FIELD) {
		if(m_pSelectCashItem == NULL || m_pSelectCashItem->iCashID != iPackageID)
			return false;

		if(m_pSelectCashItem->iPrice != iShopPrice || m_pSelectCashItem->iSalePrice != iSalePrice ||
			m_pSelectCashItem->iMaxOZ != iMaxOz || m_pSelectCashItem->iMaxMile != iMaxMileage)
		{
			return false;
		}

		if(IsEnableInventory() == false)
			return false;

		if(IsSendCheck() == false)
			return false;

		SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
		if( pLocalPlayer == NULL )
			return 0;

		if( g_pInterfaceManager->IsCutIn() )
			return 0;

		if( g_pGOBManager == NULL )
			return 0;

		if( pLocalPlayer->IsDead() )
			return 0;

		if( pLocalPlayer->GetCurAction() == ACTION_ENCHANTTING )
		{
			D3DXCOLOR	cColor	=	D3DXCOLOR((160.0f / 255.0f), (255.0f / 255.0f), (0.0f / 255.0f), 1.0f);
			g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)g_pResourceManager->GetGlobalString( 4000002 ) , (LPARAM)&cColor );
			return 0;
		}
	}
	
	if(g_pCheckManager->IsDBBlock())
		return false;

	if(m_iCashTradeState == CASH_TRADE_STATE_WAIT)
		return false;

	CPacket Packet;
	Packet.Add((UINT32)CASHSHOP_CS_BUY);

	Packet.AddUINT32(iPackageID);
	Packet.AddUINT32(iShopPrice);
	Packet.AddUINT32(iSalePrice);
	Packet.AddUINT32(iMaxOz);
	Packet.AddUINT32(iMaxMileage);

	Packet.AddUINT32(iBuyOz);
	Packet.AddUINT32(iBuyMileage);

	g_pNetworkManager->SPPerformMessage(CASHSHOP_CS_BUY, 0, (LPARAM)&Packet);

	g_pCheckManager->SetDBBlock(true);

	m_iCashTradeState = CASH_TRADE_STATE_WAIT;

	m_CashTradeResult.Clear();

	m_bBuyPacketReceive	= false;
	m_bWaitAniComplete = false;
	
	if(iPlace == CASH_TRADE_PLACE_FIELD) {
		if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
			SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE_WAIT);
			if(pWindow) {
				g_pInterfaceManager->SetFocusWindow(pWindow);
				pWindow->Show();
				g_pInterfaceManager->SetModalWindow(pWindow);
			}

			pWindow = NULL;
			pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE);
			if(pWindow) {
				pWindow->Hide();
			}

			pWindow = NULL;
			pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_GIFT);
			if(pWindow) {
				pWindow->Hide();
			}			
		}
	}

	return true;
}


bool SPCashTradeManager::OnCashShopBuy(CPacket* pPacket)
{
	if( pPacket == NULL )
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	g_pCheckManager->SetDBBlock(false);	
	
	//여기서 분기해서 
	if(m_iCashTradePlace == CASH_TRADE_PLACE_SHOP) { //캐시샵 인터페이스로...		
		WPARAM wParam = 0;
		LPARAM lParam = (LPARAM)pPacket;		
		if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
			if( g_pInterfaceManager )
				g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_BUY, wParam, lParam);
		}

		m_iCashTradeState = CASH_TRADE_STATE_NULL;
		return true;
	}

	SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pInven == NULL )
		return false;	
	
	//CASHSHOP_SC_BUY,
	GLOBAL_STRING_ID		iErrorMsg;
	GU_ID					globalID;
	UINT32					iPackageID;				//CASH_SHOP.LDT::ItemID 
	UINT32					iCurrOz;				// 현재 오즈
	UINT32					iCurrMileage;			// 현재 마일리지.
	UINT16					nItems;					// 신규 생성 아이템 갯수
	CONTAINER_ITEM			containerItem;			// 신규 생성 아이템 정보.
	
	pPacket->ExtractUINT32(&iErrorMsg);
	if(iErrorMsg) // 실패
	{
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(iErrorMsg)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iErrorMsg);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		ShowResultFail();
		return false;
	}

	pPacket->ExtractUINT64(&globalID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != globalID) {
			return false;
		}
	}	
	
	pPacket->ExtractUINT32(&iPackageID);
	pPacket->ExtractUINT32(&iCurrOz);	
	pPacket->ExtractUINT32(&iCurrMileage);
	
	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValue(STATUS_CASH,	iCurrOz);
	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValue(STATUS_MILE,	iCurrMileage);

	int iSlotIndex = -1;
	pPacket->ExtractUINT16(&nItems);

	SPItemAttr* pItemAttr = NULL;
	char szMsg[256];
	for(int i = 0; i < nItems; i++ )
	{
		pPacket->Extract(&containerItem, sizeof(CONTAINER_ITEM));
		containerItem.SlotInfo.SlotIndex -= 1;
		
		//pInven->ItemSetting(containerItem, false);
		//iSlotIndex = containerItem.SlotInfo.SlotIndex;
		
		g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->ItemSetting(containerItem, false);

		pItemAttr = NULL;
		ZeroMemory(szMsg, 256);

		if( g_pItemCluster->GetItemInfo(containerItem.Item.iItemID, pItemAttr) ) {				
			D3DXCOLOR color;
			if(pItemAttr->m_bNameColor) {
				color = pItemAttr->GetNameColor();
			}
			else {
				color = g_pCheckManager->GetItemColor(containerItem.Item.iRare);
			}

			sprintf(szMsg, g_pResourceManager->GetGlobalString(5017006), pItemAttr->m_strName.c_str());

			if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
				g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMsg , (LPARAM)&color );
			}			
		}
	}
	
	m_CashTradeResult.Clear();
	m_CashTradeResult.m_iCashShopID = iPackageID;	

	SetBuyPacketReceive();
	
	return 1;
}


bool SPCashTradeManager::SendCashShopPresent(CASH_TRADE_PLACE iPlace, int iPackageID,
						 int iShopPrice,	int iSalePrice, int iMaxOz, int iMaxMileage,
						 std::string strName, std::string strMsg,
						 int iBuyOz /*= 0*/, int iBuyMileage /*= 0*/)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	m_iCashTradePlace = iPlace;

	if(iPlace == CASH_TRADE_PLACE_FIELD) {
		if(m_pSelectCashItem == NULL || m_pSelectCashItem->iCashID != iPackageID)
			return false;

		if(m_pSelectCashItem->iPrice != iShopPrice || m_pSelectCashItem->iSalePrice != iSalePrice ||
			m_pSelectCashItem->iMaxOZ != iMaxOz || m_pSelectCashItem->iMaxMile != iMaxMileage)
		{
			return false;
		}

		if(IsEnableInventory() == false)
			return false;

		if(IsSendCheck() == false)
			return false;
	}

	if(strName.empty())
		return false;

	//if(strMsg.empty())
	//	return false;

	if(strName.length() > LEN_NAME)
		return false;

	if(strMsg.length() > MAX_MAIl_BODY_LEN)
		return false;

	if(g_pCheckManager->IsDBBlock())
		return false;

	if(m_iCashTradeState == CASH_TRADE_STATE_WAIT)
		return false;

	char szReceiver[LEN_NAME];
	ZeroMemory(&szReceiver, LEN_NAME);
	char szMemo[MAX_MAIl_BODY_LEN];	
	ZeroMemory(&szMemo, MAX_MAIl_BODY_LEN);
	
	_tcsncpy(szReceiver, strName.c_str(), LEN_NAME);
	_tcsncpy(szMemo, strMsg.c_str(), MAX_MAIl_BODY_LEN);

	CPacket Packet;
	Packet.Add((UINT32)CASHSHOP_CS_BUY_PRESENT);	
	
	Packet.Add(szReceiver, sizeof(szReceiver));
	
	Packet.AddUINT32(iPackageID);
	Packet.AddUINT32(iShopPrice);
	Packet.AddUINT32(iSalePrice);
	Packet.AddUINT32(iMaxOz);
	Packet.AddUINT32(iMaxMileage);

	Packet.AddUINT32(iBuyOz);
	Packet.AddUINT32(iBuyMileage);

	Packet.Add(szMemo, sizeof(szMemo));

	g_pNetworkManager->SPPerformMessage(CASHSHOP_CS_BUY_PRESENT, 0, (LPARAM)&Packet);


	g_pCheckManager->SetDBBlock(true);

	m_iCashTradeState = CASH_TRADE_STATE_WAIT;
	
	m_CashTradeResult.Clear();

	m_bBuyPacketReceive	= false;
	m_bWaitAniComplete = false;

	if(iPlace == CASH_TRADE_PLACE_FIELD) {
		if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
			SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE_WAIT);
			if(pWindow) {
				g_pInterfaceManager->SetFocusWindow(pWindow);
				pWindow->Show();
				//g_pInterfaceManager->SetModalWindow(pWindow);
			}

			pWindow = NULL;
			pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE);
			if(pWindow) {
				pWindow->Hide();
			}

			pWindow = NULL;
			pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_GIFT);
			if(pWindow) {
				pWindow->Hide();
			}			
		}
	}
	
	return true;
}


bool SPCashTradeManager::OnCashShopGift(CPacket* pPacket)
{
	if( pPacket == NULL )
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	g_pCheckManager->SetDBBlock(false);	

	//[xialin 2008/09/16] add
	SPWindowBeExact * pkWindowBeExact = (SPWindowBeExact *)g_pInterfaceManager->FindWindow(WIID_BEEXACT);
	if ( pkWindowBeExact && pkWindowBeExact->IsGifting() )
	{
		WPARAM wParam = 0;
		LPARAM lParam = (LPARAM)pPacket;
		g_pInterfaceManager->SPChildWndSendMessage(WIID_BEEXACT, SPIM_BEEXACT_GIFT, wParam, lParam);
		return true;
	}

	//여기서 분기해서 
	if(m_iCashTradePlace == CASH_TRADE_PLACE_SHOP) { //캐시샵 인터페이스로...		
		WPARAM wParam = 0;
		LPARAM lParam = (LPARAM)pPacket;
		if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
			if( g_pInterfaceManager ) {
				g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_GIFT, wParam, lParam);
			}
		}
		m_iCashTradeState = CASH_TRADE_STATE_NULL;
		return true;
	}		
	
	//CASHSHOP_SC_BUY_PRESENT
	GLOBAL_STRING_ID		iErrorMsg;
	GU_ID					globalID;
	TCHAR					TargetCharName[LEN_NAME + 1];
	UINT32					iPackageID;				//CASH_SHOP.LDT::ItemID 
	UINT32					iCurrOz;				// 현재 오즈
	UINT32					iCurrMileage;			// 현재 마일리지.	

	pPacket->ExtractUINT32(&iErrorMsg);
	if(iErrorMsg) // 실패
	{
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(iErrorMsg)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iErrorMsg);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
		ShowResultFail();
		return false;
	}

	pPacket->ExtractUINT64(&globalID);
	if(g_pGOBManager->GetLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() != globalID) {
			return false;
		}
	}	

	ZeroMemory(TargetCharName, sizeof(LEN_NAME + 1));	
	pPacket->Extract(TargetCharName, sizeof(char)*LEN_NAME);

	pPacket->ExtractUINT32(&iPackageID);
	pPacket->ExtractUINT32(&iCurrOz);	
	pPacket->ExtractUINT32(&iCurrMileage);

	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValue(STATUS_CASH,	iCurrOz);
	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValue(STATUS_MILE,	iCurrMileage);	
	

	m_CashTradeResult.Clear();
	m_CashTradeResult.m_iCashShopID = iPackageID;
	m_CashTradeResult.m_strReceiver = TargetCharName;

	SetBuyPacketReceive();

	return true;
}


void SPCashTradeManager::ShowResultAction()
{
	if(m_bBuyPacketReceive == false || m_bWaitAniComplete == false)
		return;

	// 대기 윈도우 해제
	if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
		SPWindow* pWindow = NULL;
		pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE_WAIT);
		if(pWindow) {
			pWindow->Hide();
			g_pInterfaceManager->SetModalWindow(NULL);
		}

		//Result 윈도우		
		pWindow = NULL;
		pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE_RESULT);
		if(pWindow) {
			g_pInterfaceManager->SetFocusWindow(pWindow);
			pWindow->Show();
		}
	}

	m_iCashTradeState = CASH_TRADE_STATE_NULL;
}


void SPCashTradeManager::ShowResultFail()
{
	// 대기 윈도우 해제
	if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
		SPWindow* pWindow = NULL;
		pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE_WAIT);
		if(pWindow) {
			pWindow->Hide();
			g_pInterfaceManager->SetModalWindow(NULL);
		}
	}
	
	g_pCheckManager->SetDBBlock(false);
	m_iCashTradeState = CASH_TRADE_STATE_NULL;
	
	// 플레이어 사망시 처리
	if(g_pGOBManager->GetLocalPlayer()){
		if(g_pGOBManager->GetLocalPlayer()->IsDead())
			g_pGOBManager->GetLocalPlayer()->SetDeadAction();
	}
}


bool SPCashTradeManager::SendWishAdd(CASH_TRADE_PLACE iPlace, int iPackageID)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	m_iCashTradePlace = iPlace;

	if(iPlace == CASH_TRADE_PLACE_FIELD) {
		if(m_pSelectCashItem == NULL || m_pSelectCashItem->iCashID != iPackageID)
			return false;
		
		//[2008/4/28] - 찜이 가능한지 확인 (단 캐시샵에 들어갔다 나와야 함)
		if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
			if(g_pInterfaceManager){
				int iReturn = 0;
				g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_CART_CHECK, (WPARAM)iPackageID, (LPARAM)&iReturn);
				
				if(iReturn) {
					if(g_pResourceManager->GetGlobalString(iReturn)) {
						std::string strMsg = g_pResourceManager->GetGlobalString(iReturn);
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
					}
					return false;
				}
			}
		}
	}

	CPacket Packet;
	Packet.Add((UINT32)CASHSHOP_CS_WISH_ADD);
	Packet.AddUINT32(iPackageID);
	g_pNetworkManager->SPPerformMessage(CASHSHOP_CS_WISH_ADD, 0, (LPARAM)&Packet);

	g_pCheckManager->SetDBBlock(true);
	m_iCashTradeState = CASH_TRADE_STATE_WAIT;

	if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
		SPWindow* pWindow = NULL;
		pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE_SELECT);
		if(pWindow) {
			pWindow->Hide();
		}
	}

	return true;
}


bool SPCashTradeManager::OnCashWishAdd(CPacket* pPacket)
{
	if( pPacket == NULL )
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;	

	g_pCheckManager->SetDBBlock(false);
	m_iCashTradeState = CASH_TRADE_STATE_NULL;

	//여기서 분기해서 
	if(m_iCashTradePlace == CASH_TRADE_PLACE_SHOP) { //캐시샵 인터페이스로...
		WPARAM wParam = 0;
		LPARAM lParam = (LPARAM)pPacket;
		if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
			if(g_pInterfaceManager)
				g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_CARTADD, wParam, lParam);
		}

		return true;
	}	
	
	//CASHSHOP_SC_WISH_ADD,
	GLOBAL_STRING_ID		iErrorMsg;
	UINT32					iPackageID;		// 패키지아이디(성공일때만)
	
	pPacket->ExtractUINT32(&iErrorMsg);
	if(iErrorMsg) // 실패
	{
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(iErrorMsg)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iErrorMsg);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
		g_pCheckManager->SetDBBlock(false);
		return false;
	}

	pPacket->ExtractUINT32(&iPackageID);

	//// - CASHSHOP_SC_INFO 서 받기때문에 필드에서는 저장하지 않아도 됨
	//if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
	//	if(g_pInterfaceManager)
	//		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_CARTADD, wParam, lParam);
	//}
	
	// 성공적으로 리스트에 등록
	if(g_pResourceManager->GetGlobalString(9005002) != NULL) {
		std::string strMsg = g_pResourceManager->GetGlobalString(9005002);
		if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) {
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str());
		}
	}
	
	return true;
}
