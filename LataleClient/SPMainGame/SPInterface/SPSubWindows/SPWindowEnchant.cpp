#include "SPCommon.h"
#include "SPUtil.h"
#include "SPWindowDEF.h"
#include "SPCommandConvert.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include "SPWindowEnchant.h"

#include "SPGOBManager.h"
#include "SPGameObject.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPGOBStatus.h"
#include "SPPlayer.h"

#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

//------------------------------------------------------------------------------------
SPWindowEnchant::SPWindowEnchant(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_pkCursorTexture(NULL)
, m_pkEnchantEquip(NULL)
, m_pkEnchantMaterial(NULL)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowEnchant::SPWindowEnchant(INSTANCE_ID InstanceID,
	int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_ENCHANT, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pkCursorTexture(NULL)
, m_pkEnchantEquip(NULL)
, m_pkEnchantMaterial(NULL)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowEnchant::~SPWindowEnchant()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE(m_pkCursorTexture);
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::Init()
{
	SPWindowStatic* pkWindow, *pkChildWindow;
	SPWindowButton* pkButton;

	pkWindow = new SPWindowStatic(WIID_ENCHANT_SKIN_UPPER, 0, 0, 240, 4, this);
	pkWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 31);

	pkWindow = new SPWindowStatic(WIID_ENCHANT_SKIN_LOWER, 0, 140, 240, 4, this);
	pkWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 41);

	pkWindow = new SPWindowStatic(WIID_ENCHANT_SKIN_MIDDLE, 0, 4, 240, 136, this);
	pkWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 37);
	pkWindow->SetSrcSize(240, 2);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_TITLE, 17, 3, 87, 15, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 204, 329);

	pkButton = new SPWindowButton(WIID_ENCHANT_CLOSE, 219, 7, 12, 12, pkWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 301, 321);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 314, 321);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 327, 321);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 340, 321);

	pkButton = new SPWindowButton(WIID_ENCHANT_OK, 93, 116, 68, 17, pkWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 346, 440);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 346, 458);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 346, 476);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 346, 494);

	pkButton = new SPWindowButton(WIID_ENCHANT_CANCEL, 165, 116, 68, 17, pkWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 415, 440);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 415, 458);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 415, 476);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 415, 494);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_PERCENTAGE_SKIN, 10, 98, 95, 15, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 416, 257);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_PERCENTAGE, 66, 98, 22, 15, pkWindow);
	pkChildWindow->SetFormat(DT_VCENTER | DT_RIGHT);

	InitMiddleWindow(pkWindow);
	InitEnchantValue();

	m_bEnchantBlock = false;

	m_bUse	=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_ITEMENCHANT );
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::InitMiddleWindow(SPWindow* pkParent)
{
	SPWindowStatic* pkWindow, *pkChildWindow;

	pkWindow = new SPWindowStatic(WIID_ENCHANT_MIDDLE_BGK, 2, 22, 236, 72, pkParent);
	pkWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 486);
	pkWindow->SetSrcSize(6, 6);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_RESULT_QUESTION, 180, 18, 12, 18, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 360, 321);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_EQUIP_SKIN, 34, 11, 32, 32, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 408);
	new SPWindowStatic(WIID_ENCHANT_EQUIP, 0, 0, 32, 32, pkChildWindow);
	
	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_MATERIAL_SKIN, 102, 11, 32, 32, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 408);
	new SPWindowStatic(WIID_ENCHANT_MATERIAL, 0, 0, 32, 32, pkChildWindow);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_RESULT_SKIN, 170, 11, 32, 32, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 408);
	new SPWindowStatic(WIID_ENCHANT_RESULT, 0, 0, 32, 32, pkChildWindow);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_PLUS, 76, 18, 16, 16, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 239, 423);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_EQUAL, 144, 19, 16, 14, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 239, 408);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_EQUIP_HELP, 12, 53, 158, 15, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 353, 273);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_MATERIAL_HELP, 72, 53, 158, 15, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 353, 289);	

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_RESULT_HELP, 30, 53, 177, 15, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 106);	

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_CURSOR, 45, 40, 10, 10, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 354, 261);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 365, 261);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 376, 261);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 387, 261);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 398, 261);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_MATERAIL_NUM1, 102, 11, 9, 10, pkWindow);
	pkChildWindow->SetImage(g_pItemCluster->GetNumTexture(), 0, 0);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_MATERAIL_NUM10, 102, 11, 9, 10, pkWindow);
	pkChildWindow->SetImage(g_pItemCluster->GetNumTexture(), 0, 0);

	InitPrograssBarWindow(pkWindow);
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::InitPrograssBarWindow(SPWindow* pkParent)
{
	SPWindowStatic* pkWindow, *pkChildWindow, *pkChildWindow2;

	pkWindow = new SPWindowStatic(WIID_ENCHANT_PROGRASS, 10, 52, 217, 11, pkParent);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_PROGRASS_BG_MIDDLE, 2, 0, 211, 11, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 241, 390);
	pkChildWindow->SetSrcSize(2, 11);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_PROGRASS_BG_RIGHT, 213, 0, 3, 11, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 245, 390);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_PROGRASS_GAGE, 2, 1, 0, 9, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 250, 391);
	pkChildWindow->SetSrcSize(4, 9);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_PROGRASS_GAGE_RIGHT, 2, 1, 0, 9, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 261, 391);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_PROGRASS_GAGE_DECORATION, 2, 1, 0, 9, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 256, 391);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_PROGRASS_BRIGHT, 2, 1, 213, 9, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 203);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_PROGRASS_BG_LEFT, 0, 0, 3, 11, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 236, 390);

	pkChildWindow = new SPWindowStatic(WIID_ENCHANT_PROGRASS_NUMBER_BG, -22, -21, 31, 20, pkWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 336, 361);

	pkChildWindow2 = new SPWindowStatic(WIID_ENCHANT_PROGRASS_NUMBER_PER, 22, 6, 6, 5, pkChildWindow);
	pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 264, 291);

	int i;
	for( i = 0; i < 3; i++ )
	{
		pkChildWindow2 = new SPWindowStatic(WIID_ENCHANT_PROGRASS_NUMBER + i, 3 + 6 * i, 6, 5, 5, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 204, 291);
	}

	for( i = 0; i < 10; i++ )
	{
		m_ptPercentageNum[i].x = 204 + i * 6;
		m_ptPercentageNum[i].y = 291;
	}

	m_iPrograssWidth = 213;
	m_iPrograssRightWidth = 2;
	m_iPrograssDecorationWidth = 4;

	m_iPrograssStartPosX = 2;
	m_iPrograssStartPosY = 1;
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::InitEnchantValue()
{
	SetEnchantStep(ECS_READY_EQUIP);
	SetPercentage(-1);

	SetEnchantItem(NULL);
	SetMaterialItem(NULL);
	SetResultItem(false);
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::SetEnchantStep(ENCHANT_STEP eStep)
{
	SPWindow* pkWindow;

	m_eEnchantStep = eStep;

	pkWindow = Find(WIID_ENCHANT_CURSOR, true);
	if( pkWindow )
	{
		int iPosX = 0, iPosY = 0, iPrograssType = 0;
		unsigned int uiHelpID = 0;
		bool bOkButton = false, bCancelButton = false;
		
		switch( eStep )
		{
		case ECS_READY_EQUIP:
			iPosX = 45;
			iPosY = 40;
			uiHelpID = WIID_ENCHANT_EQUIP_HELP;
			break;
		case ECS_READY_MATERIAL:
			iPosX = 113;
			iPosY = 40;
			uiHelpID = WIID_ENCHANT_MATERIAL_HELP;
			break;
		case ECS_READY_ENCHANT:
			bOkButton = true;
			bCancelButton = true;
			if( m_pItemEnchant && m_pItemEnchant->iWhenFailed )
				uiHelpID = WIID_ENCHANT_RESULT_HELP;
			break;
		case ECS_ENCHANTTING:
			bCancelButton = true;
			iPrograssType = 1;
			break;
		case ECS_BRIGHTTING:
			iPrograssType = 2;
			break;
		}
		if( eStep < ECS_READY_ENCHANT )
		{
			pkWindow->AdjustCoord(iPosX , iPosY);
			pkWindow->SPSendMessage(SPIM_POS_UPDATE);
			pkWindow->Show();
		}
		else
			pkWindow->Hide();

		pkWindow = Find(WIID_ENCHANT_EQUIP_HELP, true);
		if( pkWindow )
			pkWindow->Hide();
		
		pkWindow = Find(WIID_ENCHANT_MATERIAL_HELP, true);
		if( pkWindow )
			pkWindow->Hide();

		pkWindow = Find(WIID_ENCHANT_RESULT_HELP, true);
		if( pkWindow )
			pkWindow->Hide();

		pkWindow = Find(uiHelpID, true);
		if( pkWindow )
			pkWindow->Show();

		pkWindow = Find(WIID_ENCHANT_OK, true);
		if( pkWindow )
			pkWindow->SPSendMessage(SPIM_BTN_ENABLE, (WPARAM)bOkButton);
		pkWindow = Find(WIID_ENCHANT_CANCEL, true);
		if( pkWindow )
			pkWindow->SPSendMessage(SPIM_BTN_ENABLE, (WPARAM)bCancelButton);

		pkWindow = Find(WIID_ENCHANT_MIDDLE_BGK, true);
		if( pkWindow )
		{
			if( eStep < ECS_ENCHANTTING )
				pkWindow->SetImagePos(341, 2);
			else
				pkWindow->SetImagePos(336, 2);
		}

		SetPrograss(iPrograssType);
		
		m_iCursorCurFrame = 0;
		m_fCursorAccmulateTime = 0.0f;
		m_iCursorDirection = 0;
	}
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::SetEnchantItem(SPItem* pkItem)
{
	if( m_pkEnchantEquip )
		SetInvenItemUnlock(m_pkEnchantEquip, 0);

	m_pkEnchantEquip = pkItem;
	m_eEnchantEquipType = CONTAINER_TYPE_INVALID;
	m_iEnchantEquipSlotIndex = -1;

	SPWindow* pkWindow;
	pkWindow = Find(WIID_ENCHANT_EQUIP, true);
	if( pkWindow == NULL )
		return;

	pkWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);
	if( pkItem == NULL )
		return;

	SPItemAttr* pItemAttr = pkItem->GetItemAttr();
	if( pItemAttr == NULL )
		return;

	POINT pt;
	SPTexture* pkTexture;
	pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(pItemAttr->m_iIcon, pItemAttr->m_iIconIndex, pt);
	if( pkTexture )
		pkWindow->SetImage(pkTexture, pt.x, pt.y);

	if( m_pkEnchantEquip )
	{
		SetInvenItemLock(m_pkEnchantEquip, 0);
		m_eEnchantEquipType = m_pkEnchantEquip->GetContainerType();
		m_iEnchantEquipSlotIndex = m_pkEnchantEquip->GetSlotIndex();
	}
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::SetMaterialItem(SPItem* pkItem)
{
	if( m_pkEnchantMaterial )
		SetInvenItemUnlock(m_pkEnchantMaterial, 1);

	m_pkEnchantMaterial = pkItem;

	SPWindow* pkWindow, *pkNumWindow[2];
	pkWindow = Find(WIID_ENCHANT_MATERIAL, true);
	if( pkWindow == NULL )
		return;

	pkNumWindow[0] = Find(WIID_ENCHANT_MATERAIL_NUM1, true);
	pkNumWindow[1] = Find(WIID_ENCHANT_MATERAIL_NUM10, true);
	if( pkNumWindow[0] == NULL || pkNumWindow[1] == NULL )
		return;

	pkWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);
	pkNumWindow[0]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	pkNumWindow[1]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	if( pkItem == NULL )
		return;

	SPItemAttr* pItemAttr = pkItem->GetItemAttr();

	POINT pt;
	SPTexture* pkTexture;
	pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(pItemAttr->m_iIcon, pItemAttr->m_iIconIndex, pt);
	if( pkTexture )
		pkWindow->SetImage(pkTexture, pt.x, pt.y);

	SPItemStatus* pkItemStatus = pkItem->GetItemStatus();
	if( pkItemStatus == NULL )
		return;
	
	if( m_pkEnchantMaterial )
		SetInvenItemLock(m_pkEnchantMaterial, 1);
	  
	if( m_pItemEnchant == NULL )
		return;

	int iStackCount = m_pItemEnchant->iMaterialNum;
	if( iStackCount < 2 )
		return;

	if( iStackCount < 10 )
	{
		POINT ptDst, ptSrc;
		ptDst.x = 102;
		ptDst.y = 33;

		ptSrc.x = iStackCount * 9;
		ptSrc.y = 0;

		pkNumWindow[0]->AdjustCoord(ptDst.x, ptDst.y);
		pkNumWindow[0]->SetImage(g_pItemCluster->GetNumTexture(), ptSrc.x, ptSrc.y);
		pkNumWindow[0]->SPSendMessage(SPIM_POS_UPDATE);
	}
	else if( iStackCount < 100 )
	{
		POINT ptDst, ptSrc;
		ptDst.x = 102;
		ptDst.y = 33;

		ptSrc.x = (iStackCount / 10) * 9;
		ptSrc.y = 0;

		pkNumWindow[1]->AdjustCoord(ptDst.x, ptDst.y);
		pkNumWindow[1]->SetImage(g_pItemCluster->GetNumTexture(), ptSrc.x, ptSrc.y);
		pkNumWindow[1]->SPSendMessage(SPIM_POS_UPDATE);

		ptDst.x = 109;
		ptDst.y = 33;

		ptSrc.x = (iStackCount % 10) * 9;
		ptSrc.y = 0;

		pkNumWindow[0]->AdjustCoord(ptDst.x, ptDst.y);
		pkNumWindow[0]->SetImage(g_pItemCluster->GetNumTexture(), ptSrc.x, ptSrc.y);
		pkNumWindow[0]->SPSendMessage(SPIM_POS_UPDATE);
	}
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::SetResultItem(bool bFlag)
{
	m_bResultItem = bFlag;

	SPWindow* pkWindow, *pkQuestionWindow;
	pkWindow = Find(WIID_ENCHANT_RESULT, true);
	if( pkWindow == NULL )
		return;

	pkWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);
	pkQuestionWindow = Find(WIID_ENCHANT_RESULT_QUESTION, true);
	if( pkQuestionWindow )
		pkQuestionWindow->Show();

	SetPercentage(-1);
	if( !bFlag )
		m_pItemEnchant = NULL;

	if( m_pkEnchantEquip == NULL || g_pItemCluster == NULL )
		return;

	if( !bFlag )
		return;

	SPItemAttr* pItemAttr = m_pkEnchantEquip->GetItemAttr();

	POINT pt;
	SPTexture* pkTexture;
	pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(pItemAttr->m_iIcon, pItemAttr->m_iIconIndex, pt);
	if( pkTexture )
		pkWindow->SetImage(pkTexture, pt.x, pt.y);

	if( pkQuestionWindow )
		pkQuestionWindow->Hide();

	if( m_pItemEnchant )
	{
		int iProvEnchant = 0;
		if( g_pGOBManager->GetLocalPlayer() && g_pGOBManager->GetLocalPlayer()->GetStatus() )
			iProvEnchant = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_PROB_ENCHANT);

		SPItemStatus* pkItemStatus = m_pkEnchantEquip->GetItemStatus();
		int iOptPoint = pkItemStatus->SPSendMessage(MV_ITEM_GET_OPTPOINT);
		int iPercentage = iOptPoint - m_pItemEnchant->iProbabilityModifier;

		iPercentage += iProvEnchant;
		if( iPercentage < m_pItemEnchant->iProbabilityMin )
			iPercentage = m_pItemEnchant->iProbabilityMin;
		else if( iPercentage > m_pItemEnchant->iProbabilityMax + iProvEnchant )
			iPercentage = m_pItemEnchant->iProbabilityMax + iProvEnchant;

		if( iPercentage > 100 )
			iPercentage = 100;

		SetPercentage(iPercentage);
	}
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::SetPercentage(int iPercentage)
{
	SPWindow* pkWindow;

	m_iSuccessedPercentage = iPercentage;
	pkWindow = Find(WIID_ENCHANT_PERCENTAGE, true);
	if( pkWindow == NULL )
		return;

	pkWindow->SetWindowText(NULL);
	if( iPercentage >= 0 )
	{
		char szBuf[12];
		sprintf(szBuf, "%d", iPercentage);
		pkWindow->SetWindowText(szBuf);
	}
}

//------------------------------------------------------------------------------------
SPItem* SPWindowEnchant::GetInvenPickUpItem()
{
	SPPlayer* pkPlayer = g_pGOBManager->GetLocalPlayer();
	if( pkPlayer == NULL )
		return NULL;

	SPPlayerInvenArchive* pkInvenArchive = pkPlayer->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return NULL;

	SPSelectItem SelItemInfo = pkInvenArchive->GetPickupItem();
	return pkInvenArchive->GetInventoryItem(SelItemInfo.m_iContainer, SelItemInfo.m_iSlotIndex);
}

//------------------------------------------------------------------------------------
SPItem* SPWindowEnchant::GetInvenItem(CONTAINER_TYPE iType, int iSlotIndex)
{
	SPPlayer* pkPlayer = g_pGOBManager->GetLocalPlayer();
	if( pkPlayer == NULL )
		return NULL;

	SPPlayerInvenArchive* pkInvenArchive = pkPlayer->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return NULL;
	
	return pkInvenArchive->GetInventoryItem(iType, iSlotIndex);
}

//------------------------------------------------------------------------------------
bool SPWindowEnchant::EnableEnchantEquip(SPItem* pkItem)
{
	if( pkItem == NULL || g_pItemCluster == NULL )
		return false;

	if( pkItem->GetContainerType() > INVENTORY_HOUSING )
		return false;

	SPItemAttr* pkItemAttr = pkItem->GetItemAttr();
	if( pkItemAttr == NULL )
		return false;

	// 시간제 아이템 유효성 검사 [10/31/2006 AJJIYA]
	SPItemStatus* pItemStatus = pkItem->GetItemStatus();

	if( pItemStatus == NULL )
		return false;

	if( pItemStatus->IsTimeAvailable() == false )
	{
		char szTemp[256];
		ZeroMemory(szTemp, 256);
		sprintf(szTemp, g_pResourceManager->GetGlobalString(10000040), pItemStatus->GetItemAttr()->m_strName.c_str());
		SetError(szTemp);
		//SetError(g_pResourceManager->GetGlobalString(10000040));
		return false;
	}

	return g_pItemCluster->IsItemEnchant(pkItemAttr->m_iOptionType);
}

//------------------------------------------------------------------------------------
SPItemEnchant* SPWindowEnchant::EnableEnchantMaterial(SPItem* pkItem)
{
	if( pkItem == NULL || g_pItemCluster == NULL || m_pkEnchantEquip == NULL )
		return NULL;

	SPItemAttr* pkItemAttr = m_pkEnchantEquip->GetItemAttr();
	if( pkItemAttr == NULL )
		return NULL;

	SPItemStatus* pkEquipItemStatus = m_pkEnchantEquip->GetItemStatus();
	if( pkEquipItemStatus == NULL )
		return NULL;

	SPItemStatus* pkMaterialItemStatus = pkItem->GetItemStatus();
	if( pkMaterialItemStatus == NULL )
		return NULL;

	// 시간제 아이템 유효성 검사 [10/31/2006 AJJIYA]
	if( pkEquipItemStatus->IsTimeAvailable() == false )
	{
		char szTemp[256];
		ZeroMemory(szTemp, 256);
		sprintf(szTemp, g_pResourceManager->GetGlobalString(10000040), pkEquipItemStatus->GetItemAttr()->m_strName.c_str());
		SetError(szTemp);
		//SetError(g_pResourceManager->GetGlobalString(10000040));
		return NULL;
	}

	if( pkMaterialItemStatus->IsTimeAvailable() == false )
	{
		char szTemp[256];
		ZeroMemory(szTemp, 256);
		sprintf(szTemp, g_pResourceManager->GetGlobalString(10000040), pkMaterialItemStatus->GetItemAttr()->m_strName.c_str());
		SetError(szTemp);
		//SetError(g_pResourceManager->GetGlobalString(10000040));
		return NULL;
	}

	if( !g_pItemCluster->IsItemEnchant(pkItemAttr->m_iOptionType, pkMaterialItemStatus->GetItemID()) )
	{
		SetError(g_pResourceManager->GetGlobalString(10000011));
		return NULL;
	}

	SPItemEnchant* pItemEnchant = NULL;

	int i;
	pItemEnchant = g_pItemCluster->GetItemEnchant(pkItemAttr->m_iOptionType,
		pkMaterialItemStatus->GetItemID(), pkMaterialItemStatus->GetStackCount());
	if( pItemEnchant == NULL )
	{
		SetError(g_pResourceManager->GetGlobalString(10000012));
		return NULL;
	}

	if( pkItemAttr->m_iOptionClass < pItemEnchant->iItemOptionClass )
	{
		SetError(g_pResourceManager->GetGlobalString(10000013));
		return NULL;
	}

	int iOptionCount = pkEquipItemStatus->GetOptionCount();
	
	SPItemOption* pItemOption;
	bool bOptionEqual = false;
	int iOptionEqualValue = 0;

	for( i = 0; i < iOptionCount; i++ )
	{
		PACK_OPT* pPackOpt = pkEquipItemStatus->GetOption(i);
		if( pPackOpt == NULL || pPackOpt->iID == 0)
			continue;

		pItemOption = g_pItemCluster->GetItemOption(pPackOpt->iID);
		if( pItemOption == NULL )
			continue;

		if( pItemEnchant->iItemOptionGroup == pItemOption->m_iOptionGroup )
		{
			bOptionEqual = true;
			iOptionEqualValue = pItemOption->m_iOptionLevel;
			break;
		}
	}

	if( pItemEnchant->bItemOptionCheck )
	{
		if( !bOptionEqual || iOptionEqualValue != pItemEnchant->iItemOptionLevel )
		{
			SetError(pItemEnchant->strErrorMsg.c_str());
			return NULL;
		}
	}
	else
	{
		if( bOptionEqual )
		{
			SetError(g_pResourceManager->GetGlobalString(10000014));
			return NULL;
		}

		if( iOptionCount >= pkEquipItemStatus->SPSendMessage(MV_ITEM_GET_RARELEVEL) )
		{			
			SetError(g_pResourceManager->GetGlobalString(10000015));
			return NULL;
		}
	}

	return pItemEnchant;
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::SetPrograss(int iType)
{
	bool bShow = false;
	if( iType == 1 )
	{
		m_fPrograssAccmulateTime = 0.0f;
		m_fPrograssMaxTime = 10.0f;
		if( m_iSuccessedPercentage )
			m_fPrograssMaxTime = 2.0f / (float)m_iSuccessedPercentage * 100.0f;

		bShow = true;
		SendParentAction(ACTION_ENCHANTTING, 0, true);
	}
	else if( iType == 2 )
	{
		m_fPrograssBrightAlpha = 1.0f;
		bShow = true;
		SendItemEnchant();
	}

	SPWindow* pkWindow, *pkChildWindow;
	pkWindow = Find(WIID_ENCHANT_PROGRASS, true);
	if( pkWindow )
	{
		pkChildWindow = pkWindow->Find(WIID_ENCHANT_PROGRASS_BRIGHT);

		if( bShow )
		{
			pkWindow->Show();
			if( pkChildWindow )
			{
				if( iType == 2 )
				{
					pkChildWindow->Show();
					pkChildWindow->SetColor(RGBA(0xFF,0xFF,0xFF,0xFF));
				}
				else
					pkChildWindow->Hide();
			}
		}
		else
			pkWindow->Hide();

		if( bShow && iType == 1 )
		{
			for( int i = 0; i < 3; i++ )
			{
				pkChildWindow = pkWindow->Find(WIID_ENCHANT_PROGRASS_GAGE + i);
				if( pkChildWindow == NULL )
					continue;

				pkChildWindow->AdjustCoord(m_iPrograssStartPosX, m_iPrograssStartPosY, 0, 9);
				pkChildWindow->SPSendMessage(SPIM_POS_UPDATE);
			}

			pkChildWindow = pkWindow->Find(WIID_ENCHANT_PROGRASS_NUMBER_BG);
			if( pkChildWindow )
			{
				pkChildWindow->AdjustCoord(-22, -21);
				pkChildWindow->SPSendMessage(SPIM_POS_UPDATE);

				SPWindow* pkChildWindow2;
				pkChildWindow2 = pkChildWindow->Find(WIID_ENCHANT_PROGRASS_NUMBER + 2);
				pkChildWindow2->SetImagePos(m_ptPercentageNum[0].x, m_ptPercentageNum[0].y);
				pkChildWindow2->RefreshRelationCoord();

				pkChildWindow2 = pkChildWindow->Find(WIID_ENCHANT_PROGRASS_NUMBER + 1);
				pkChildWindow2->Hide();
				pkChildWindow2 = pkChildWindow->Find(WIID_ENCHANT_PROGRASS_NUMBER);
				pkChildWindow2->Hide();
			}
		}
	}
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::SendParentAction(UINT64 uiAction, unsigned int uiIndex, bool bSendInit)
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	if( bSendInit )
		g_pGOBManager->GetLocalPlayer()->InitSendAction();

	if( uiIndex )
		g_pGOBManager->GetLocalPlayer()->SetSendAttackIndex(uiIndex);

	g_pGOBManager->GetLocalPlayer()->SetControlAction(uiAction);
	g_pGOBManager->GetLocalPlayer()->SendActionPacket();
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::SendItemEnchant()
{
	if( m_pkEnchantEquip && m_pkEnchantMaterial && m_pItemEnchant )
	{
		CPacket kPacket(ITEM_CS_ENCHANT);
		kPacket.AddUINT32(m_pItemEnchant->iEnchantKey);

		CONTAINER_ITEM item;
		m_pkEnchantEquip->ConvertContainerItem(&item);
		kPacket.AddUINT32(m_pkEnchantEquip->GetContainerType());
		kPacket.AddUINT8(m_pkEnchantEquip->GetSlotIndex() + 1);
		kPacket.AddUINT64(item.Item.iItemNo);

		m_pkEnchantMaterial->ConvertContainerItem(&item);
		kPacket.AddUINT32(m_pkEnchantMaterial->GetContainerType());
		kPacket.AddUINT8(m_pkEnchantMaterial->GetSlotIndex() + 1);
		kPacket.AddUINT64(item.Item.iItemNo);
		kPacket.AddUINT8(m_pItemEnchant->iMaterialNum);

		g_pNetworkManager->GetPacketHandler()->PostMsg(&kPacket);
		m_bEnchantBlock = true;
	}
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::SetError(const char* pszErrorMsg)
{
	if( g_pInterfaceManager == NULL || pszErrorMsg == NULL )
		return;

	D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
	g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING,
		(WPARAM)pszErrorMsg, (LPARAM)&color);
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::SetInvenItemLock(SPItem* pkItem, int iIndex)
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	if( pkItem == NULL )
		return;
	
	SPPlayerInvenArchive* pkInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInven ) {
		//pkInven->SetUserShopItemFake(iIndex, pkItem->GetContainerType(), pkItem->GetSlotIndex());
		pkInven->SetBlockItem(iIndex, pkItem->GetContainerType(), pkItem->GetSlotIndex());
	}
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::SetInvenItemUnlock(SPItem* pkItem, int iIndex)
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	if( pkItem == NULL )
		return;

	SPPlayerInvenArchive* pkInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInven ) {
		//pkInven->ClearUserShopItem(iIndex);
		pkInven->ClearBlockItem(iIndex);
	}
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::SetInvenItemAllUnlock()
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	SPPlayerInvenArchive* pkInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInven ) {
		//pkInven->ClearAllUserShopItem();
		pkInven->ClearAllBlockItem();
	}
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::ToolTipEnter(SPItem* pkItem, SPItemEnchant* pEnchant)
{
	if( pkItem == NULL )
	{
		ToolTipOut();
		return;
	}

	SPItemStatus* pkItemStatus = pkItem->GetItemStatus();
	if( pkItemStatus == NULL )
		return;

	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return;

	if( pEnchant )
	{
		SPItemEnchantResult ResultItem;
		ResultItem.pItem = static_cast< void* >(pkItem);
		ResultItem.pItemEnchant = pEnchant;
		pkWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&ResultItem, -6);
	}
	else
		pkWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)pkItemStatus, 0);

	pkWindow->Show();
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::ToolTipOut()
{
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return;

	pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, 0);
	pkWindow->Show();
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::Show(bool bWithChild)
{
	if( m_bUse == false )
		return;

	SPWindow::Show(bWithChild);
	
	InitEnchantValue();
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::Hide(bool bSendServer)
{
	if( m_eEnchantStep == ECS_ENCHANTTING )
		OnCancel(0, 0);

	SPWindow::Hide(bSendServer);
	SetInvenItemAllUnlock();
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::Process(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Process(fTime);
	ProcessCursor(fTime);
	ProcessPrograss(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::ProcessCursor(float fTime)
{
	m_fCursorAccmulateTime += fTime;
	if( m_fCursorAccmulateTime > 0.05f )
	{
		if( m_iCursorDirection )
		{
			m_iCursorCurFrame--;
			if( m_iCursorCurFrame < 0 )
			{
				m_iCursorCurFrame = 1;
				m_iCursorDirection = 0;
			}
		}
		else
		{
			m_iCursorCurFrame++;
			if( m_iCursorCurFrame > 4 )
			{
				m_iCursorCurFrame = 3;
				m_iCursorDirection = 1;
			}
		}

		SPWindow* pkWindow;
		pkWindow = Find(WIID_ENCHANT_CURSOR, true);
		if( pkWindow )
			pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, m_iCursorCurFrame);

		m_fCursorAccmulateTime = 0.0f;
	}
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::ProcessPrograss(float fTime)
{
	if( m_eEnchantStep == ECS_ENCHANTTING )
	{
		if( m_fPrograssMaxTime <= 0.0f )
			return;

		m_fPrograssAccmulateTime += fTime;
		if( m_fPrograssAccmulateTime > m_fPrograssMaxTime )
			m_fPrograssAccmulateTime = m_fPrograssMaxTime;

		int iCurWidth = (int)((m_fPrograssAccmulateTime / m_fPrograssMaxTime) * (float)m_iPrograssWidth);
		if( iCurWidth == 0 )
			return;

		SPWindow* pkParentWindow, *pkGageWindow[3];
		pkParentWindow = Find(WIID_ENCHANT_PROGRASS, true);
		if( pkParentWindow == NULL )
			return;

		int i;
		for( i = 0; i < 3; i++ )
		{
			pkGageWindow[i] = pkParentWindow->Find(WIID_ENCHANT_PROGRASS_GAGE + i);
			if( pkGageWindow[i] == NULL )
				return;
		}

		if( iCurWidth < m_iPrograssRightWidth )
			return;

		pkGageWindow[1]->AdjustCoord(m_iPrograssStartPosX + (iCurWidth - m_iPrograssRightWidth), m_iPrograssStartPosY,
			m_iPrograssRightWidth, 9);
		pkGageWindow[1]->SPSendMessage(SPIM_POS_UPDATE);

		int iMidGageWidth = iCurWidth - m_iPrograssRightWidth;
		if( iMidGageWidth < 0 )
			return;

		pkGageWindow[0]->AdjustCoord(m_iPrograssStartPosX, m_iPrograssStartPosY, iMidGageWidth, 9);
		pkGageWindow[0]->SPSendMessage(SPIM_POS_UPDATE);

		if( iMidGageWidth >= m_iPrograssDecorationWidth )
		{
			pkGageWindow[2]->AdjustCoord(
				m_iPrograssStartPosX + iMidGageWidth - m_iPrograssDecorationWidth, m_iPrograssStartPosY,
				m_iPrograssDecorationWidth, 9);
			pkGageWindow[2]->SPSendMessage(SPIM_POS_UPDATE);
		}

		pkGageWindow[0] = pkParentWindow->Find(WIID_ENCHANT_PROGRASS_NUMBER_BG);
		if( pkGageWindow[0] )
		{
			pkGageWindow[0]->AdjustCoord(m_iPrograssStartPosX + iCurWidth - 22 - m_iPrograssRightWidth, -21);
			pkGageWindow[0]->SPSendMessage(SPIM_POS_UPDATE);

			int iRatio = (int)((m_fPrograssAccmulateTime / m_fPrograssMaxTime) * 100.0f);
			int iNum1 = iRatio % 10;
			int iNum10 = iRatio / 10;
			int iNum100 = iRatio / 100;

			if( iNum10 == 10 )
				iNum10 = 0;

			SPWindow* pkChildWindow2;
			pkChildWindow2 = pkGageWindow[0]->Find(WIID_ENCHANT_PROGRASS_NUMBER_PER);
			if( pkChildWindow2 )
				pkChildWindow2->RefreshRelationCoord();

			pkChildWindow2 = pkGageWindow[0]->Find(WIID_ENCHANT_PROGRASS_NUMBER + 2);
			if( pkChildWindow2 )
			{
				pkChildWindow2->Show();
				pkChildWindow2->SetImagePos(m_ptPercentageNum[iNum1].x, m_ptPercentageNum[iNum1].y);
				pkChildWindow2->RefreshRelationCoord();
			}
		
			pkChildWindow2 = pkGageWindow[0]->Find(WIID_ENCHANT_PROGRASS_NUMBER + 1);
			if( pkChildWindow2 )
			{
				if( iNum10 || iNum100 )
				{
					pkChildWindow2->Show();
					pkChildWindow2->SetImagePos(m_ptPercentageNum[iNum10].x, m_ptPercentageNum[iNum10].y);
					pkChildWindow2->RefreshRelationCoord();
				}
				else
					pkChildWindow2->Hide();
			}

			pkChildWindow2 = pkGageWindow[0]->Find(WIID_ENCHANT_PROGRASS_NUMBER);
			if( pkChildWindow2 )
			{
				if( iNum100 )
				{
					pkChildWindow2->Show();
					pkChildWindow2->SetImagePos(m_ptPercentageNum[iNum100].x, m_ptPercentageNum[iNum100].y);
					pkChildWindow2->RefreshRelationCoord();
				}
				else
					pkChildWindow2->Hide();
			}
		}

		if( m_fPrograssAccmulateTime >= m_fPrograssMaxTime )
			SetEnchantStep(ECS_BRIGHTTING);
	}
	else if( m_eEnchantStep == ECS_BRIGHTTING )
	{
		float fMaxBrightTime = 2.0f;
		float fTempAlpha = m_fPrograssBrightAlpha;
		m_fPrograssBrightAlpha -= (fTime / fMaxBrightTime);

		bool bSetColor = true;
		if( m_fPrograssBrightAlpha < 0.0f )
		{
			SetEnchantStep(ECS_ENCHANT_WAIT);
			//SendParentAction(ACTION_STOP);

			bSetColor = false;
			if( fTempAlpha > 0.0f )
			{
				m_fPrograssBrightAlpha = 0.0f;
				bSetColor = true;
			}
		}
		
		if( bSetColor )
		{
			SPWindow* pkWindow, *pkChildWindow;
			pkWindow = Find(WIID_ENCHANT_PROGRASS, true);
			if( pkWindow )
			{
				pkChildWindow = pkWindow->Find(WIID_ENCHANT_PROGRASS_BRIGHT);
				if( pkChildWindow )
					pkChildWindow->SetColor(RGBA(0xFF,0xFF,0xFF,(BYTE)(m_fPrograssBrightAlpha * 255.0f)));
			}
		}
	}
	else if( m_eEnchantStep == ECS_ENCHANT_WAIT )
	{
		if( !m_bEnchantBlock )
		{
			if( m_pkEnchantEquip && m_pkEnchantEquip->GetItemAttr() )
				SetEnchantStep(ECS_READY_MATERIAL);
			else
				SetEnchantStep(ECS_READY_EQUIP);
		}
	}
}

//------------------------------------------------------------------------------------
void SPWindowEnchant::Render(float fTime)
{
	if( !m_bShow )
		return;
	
	SPWindow::Render(fTime);
}

//------------------------------------------------------------------------------------
// Message
int SPWindowEnchant::OnExit(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowEnchant::OnOK(WPARAM wParam, LPARAM lParam)
{
	if( m_bEnchantBlock )
		return 1;

	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return 1;

	if( g_pGOBManager->GetLocalPlayer()->GetCurAction() != ACTION_STOP )
	{
		SetError(g_pResourceManager->GetGlobalString(10000016));
		return 1;
	}

	SetEnchantStep(ECS_ENCHANTTING);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowEnchant::OnCancel(WPARAM wParam, LPARAM lParam)
{
	if( m_eEnchantStep == ECS_ENCHANTTING )
	{
		SetEnchantStep(ECS_READY_ENCHANT);
		SendParentAction(ACTION_STOP);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowEnchant::OnEnchantLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if( m_eEnchantStep >= ECS_ENCHANTTING )
		return 1;

	if( m_pkEnchantEquip == NULL )
		return 1;

	SetEnchantItem(NULL);
	SetMaterialItem(NULL);
	SetResultItem(false);
	SetEnchantStep(ECS_READY_EQUIP);

	OnEquipToolTipOut( wParam , lParam );
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowEnchant::OnMaterailLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if( m_eEnchantStep >= ECS_ENCHANTTING )
		return 1;

	if( m_pkEnchantMaterial == NULL )
		return 1;

	SetMaterialItem(NULL);
	SetEnchantStep(ECS_READY_MATERIAL);
	SetResultItem(false);

	OnMaterialToolTipOut( wParam , lParam );
	return 1;
}


//------------------------------------------------------------------------------------
int SPWindowEnchant::OnEnchantLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if( m_eEnchantStep >= ECS_ENCHANTTING )
		return 1;

	SPItem* pkItem = GetInvenPickUpItem();
	if( pkItem == NULL )
		return 1;

	if( !EnableEnchantEquip(pkItem) )
		return 1;

	SetEnchantItem(pkItem);
	SetMaterialItem(NULL);
	SetResultItem(false);
	SetEnchantStep(ECS_READY_MATERIAL);

	OnEquipToolTipEnter( wParam , lParam );

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowEnchant::OnMaterailLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if( m_eEnchantStep >= ECS_ENCHANTTING )
		return 1;

	SPItem* pkItem = GetInvenPickUpItem();
	if( pkItem == NULL )
		return 1;

	SPItemEnchant* pkItemEnchant;
	if( (pkItemEnchant = EnableEnchantMaterial(pkItem)) == NULL )
		return 1;

	m_pItemEnchant = pkItemEnchant;
	SetMaterialItem(pkItem);
	SetEnchantStep(ECS_READY_ENCHANT);
	SetResultItem(true);

	OnMaterialToolTipEnter( wParam , lParam );

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowEnchant::OnEnchantReceive(WPARAM wParam, LPARAM lParam)
{
	m_bEnchantBlock = false;
	if( wParam )
		SetError(g_pResourceManager->GetGlobalString(wParam));

	SPItem* pkChangeItem = NULL;
	if( m_eEnchantEquipType != CONTAINER_TYPE_INVALID && m_iEnchantEquipSlotIndex >= 0 )
		pkChangeItem = GetInvenItem(m_eEnchantEquipType, m_iEnchantEquipSlotIndex);

	SetEnchantItem(pkChangeItem);
	SetMaterialItem(NULL);
	SetResultItem(false);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowEnchant::OnEquipToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	if( m_pkEnchantEquip == NULL )
		return 1;

	ToolTipEnter(m_pkEnchantEquip);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowEnchant::OnEquipToolTipOut(WPARAM wParam, LPARAM lParam)
{
	ToolTipOut();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowEnchant::OnMaterialToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	if( m_pkEnchantMaterial == NULL )
		return 1;

	ToolTipEnter(m_pkEnchantMaterial);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowEnchant::OnMaterialToolTipOut(WPARAM wParam, LPARAM lParam)
{
	ToolTipOut();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowEnchant::OnResultToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	if( !m_bResultItem || m_pkEnchantEquip == NULL || m_pkEnchantMaterial == NULL )
		return 1;

	if( m_pItemEnchant == NULL )
		return 1;

	ToolTipEnter(m_pkEnchantEquip, m_pItemEnchant);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowEnchant::OnResultToolTipOut(WPARAM wParam, LPARAM lParam)
{
	ToolTipOut();
	return 1;
}


//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowEnchant)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_ENCHANT_CLOSE,			OnExit )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_ENCHANT_OK,			OnOK )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_ENCHANT_CANCEL,		OnCancel )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN,		WIID_ENCHANT_EQUIP,			OnEnchantLButtonDown )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN,		WIID_ENCHANT_MATERIAL,		OnMaterailLButtonDown )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_ENCHANT_EQUIP,			OnEnchantLButtonUp )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_ENCHANT_MATERIAL,		OnMaterailLButtonUp )

SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_ENCHANT_EQUIP,			OnEquipToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_ENCHANT_EQUIP,			OnEquipToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_ENCHANT_MATERIAL,		OnMaterialToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_ENCHANT_MATERIAL,		OnMaterialToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_ENCHANT_RESULT,		OnResultToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_ENCHANT_RESULT,		OnResultToolTipOut	)

SPIMESSAGE_COMMAND(	SPIM_ITEM_ENCHANT_CANCEL,	OnCancel)
SPIMESSAGE_COMMAND(	SPIM_ITEM_ENCHANT_RESULT,	OnEnchantReceive)

SPIMESSAGE_OBJ_MAP_END(SPWindow)