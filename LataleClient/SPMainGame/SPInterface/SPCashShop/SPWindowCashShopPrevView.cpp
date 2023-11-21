#include "SPCommon.h"
#include "SPUtil.h"
#include "SPWindowDEF.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowGrid.h"
#include "SPWindowSlider.h"
#include "SPWindowEdit.h"
#include "SPWindowCashShop.h"

#include "SPSubGameManager.h"
#include "SPGOBManager.h"
#include "SPGameObject.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayerEquipInfo.h"
#include "SPPlayer.h"
#include "SPPet.h"

#include "SPItemStatus.h"
#include "SPItem.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include <WinSock2.h>
#include "PacketID.h"
#include "Packet.h"
#include "PacketHandler.h"

//------------------------------------------------------------------------------------
void SPWindowCashShop::ClearPreViewInven()
{
	m_iPreViewInvenCurPage = 0;
	m_iPreViewInvenMaxPage = 1;
	m_mPreViewInvenItem.clear();

	m_vEquipFigureItem.clear();
	m_vEquipCashItem.clear();
	ClearEffectList();
	RefreshPreViewInven();
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::InsertPreViewInven(int iIndex, SPCashItemAttr* pkItemAttr)
{
	if( (int)m_mPreViewInvenItem.size() >= OZ_INVENSLOT_MAX )
		return false;

	if( IsExistPrevViewInvenItem(pkItemAttr) )
		return false;

	map< int, SPCashItemAttr* >::iterator iter;
	iter = m_mPreViewInvenItem.find(iIndex);
	if( iter != m_mPreViewInvenItem.end() )
	{
		if( pkItemAttr == NULL )
		{
			m_mPreViewInvenItem.erase(iter);
			RefreshPreViewInven();
			return true;
		}

		(*iter).second = pkItemAttr;
	}
	else
	{
		if( pkItemAttr )
		{
			m_mPreViewInvenItem.insert(make_pair(iIndex, pkItemAttr));
			if( !m_mPreViewInvenItem.empty() )
				m_iPreViewInvenMaxPage = ((int)m_mPreViewInvenItem.size() - 1) / 8 + 1;
		}
	}
	
	RefreshPreViewInven();
	return true;
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::InsertPreViewInven(SPCashItemAttr* pkItemAttr)
{
	if( (int)m_mPreViewInvenItem.size() >= OZ_INVENSLOT_MAX )
		return false;

	if( IsExistPrevViewInvenItem(pkItemAttr) )
		return false;

	map< int, SPCashItemAttr* >::iterator iter;
	for( int i = 0; i < 8; i++ )
	{
		iter = m_mPreViewInvenItem.find(i);
		if( iter == m_mPreViewInvenItem.end() )
		{
			InsertPreViewInven(i, pkItemAttr);
			return true;
		}
	}
	
	return false;
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::IsExistPrevViewInvenItem(SPCashItemAttr* pkItemAttr)
{
	map< int, SPCashItemAttr* >::iterator iter;
	iter = m_mPreViewInvenItem.begin();

	while( iter != m_mPreViewInvenItem.end() )
	{
		if( (*iter).second == pkItemAttr )
			return true;

		++iter;
	}

	return false;
}

//------------------------------------------------------------------------------------
SPCashItemAttr* SPWindowCashShop::GetPreViewInvenItem(int iSlotIndex)
{
	map< int, SPCashItemAttr* >::iterator iter;
	iter = m_mPreViewInvenItem.find(iSlotIndex + m_iPreViewInvenCurPage * 8);
	if( iter != m_mPreViewInvenItem.end() )
		return (*iter).second;

	return NULL;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::RefreshPreViewInven()
{
	SPWindowGrid* pkWindowGrid = static_cast<SPWindowGrid*>(Find(WIID_CASHSHOP_PREVIEW_SLOT, true));
	if( pkWindowGrid == NULL )
		return;

	pkWindowGrid->ClearCellAll();

	map< int, SPCashItemAttr* >::iterator iter;
	SPCashItemAttr* pkCashItemAttr;
	int iIndex, i;

	iIndex = m_iPreViewInvenCurPage * 8;
	for( i = 0; i < 8; i++ )
	{
		iter = m_mPreViewInvenItem.find(iIndex + i);
		if( iter == m_mPreViewInvenItem.end() )
			continue;

		pkCashItemAttr = (*iter).second;
		if( pkCashItemAttr == NULL )
			continue;

		POINT ptSrcPos;
		SPTexture* pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(
			pkCashItemAttr->iIcon, pkCashItemAttr->iIconIndex, ptSrcPos);	

		pkWindowGrid->SettingIconImage(pkTexture, pkCashItemAttr->ItemInfo[0].iID,
			ptSrcPos.x, ptSrcPos.y, (SLOT_INDEX)i, 1);
	}

	SetPrevViewInvenPageText();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetPrevViewInvenPageText()
{
	SPWindow* pkParentWindow, *pkChildWindow;

	pkParentWindow = Find(WIID_CASHSHOP_PREVIEW);
	if( pkParentWindow == NULL )
		return;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_PREVIEW_PAGE_TEXT, true);
	if( pkChildWindow == NULL )
		return;

	char szBuf[32];
	sprintf(szBuf, "%d", m_iPreViewInvenCurPage + 1);
	pkChildWindow->SetWindowText(szBuf);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ClearPreViewSelect()
{
	SetSelectSlot(-1, false);
	SPWindow* pkWindow = Find(WIID_CASHSHOP_PREVIEW_PRICE, true);
	if( pkWindow )
		pkWindow->SetWindowText(NULL);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ResetPlayerLayer()
{
	SPPlayer* pkLocalPlayer;
	if( m_pkPlayer == NULL || (pkLocalPlayer = g_pGOBManager->GetLocalPlayer()) == NULL )
		return;

	SPPlayerInvenArchive* pkInvenArchive = pkLocalPlayer->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return;

	m_vDefaultFigureItem.clear();
	m_vEquipFigureItem.clear();
	m_vEquipCashItem.clear();
	m_pkPlayer->InitFigureInfo();
	m_pkPlayer->SetPlayerClass(pkLocalPlayer->GetPlayerClass());		// Add


	SPItem* pkItem;
	SPItemStatus* pkItemStatus;
	FIGURE_ITEM FigureItem;
	int iSize, i, j;
	CONTAINER_TYPE eFigureIndex[3+1] = {FIGURE_BASIC, FIGURE_EQUIP, FIGURE_FASHION, FIGURE_EFFECT};
	for( i = 0; i < 3+1; i++ )
	{
		iSize = pkInvenArchive->GetInventorySize(eFigureIndex[i]);
		for( j = 0; j < iSize; j++ )
		{
			pkItem = pkInvenArchive->GetInventoryItem(eFigureIndex[i], j);
			if( pkItem == NULL )
				continue;

			FigureItem.iItemID = pkItem->GetItemStatusValue(MV_ITEM_GETUID);
			if( FigureItem.iItemID == 0 )
				continue;
			
			pkItemStatus = pkItem->GetItemStatus();
			if( pkItemStatus == NULL || !pkItemStatus->IsTimeAvailable() )
				continue;

			FigureItem.SlotInfo.ContainerType = pkItem->GetContainerType();
			FigureItem.SlotInfo.SlotIndex = pkItem->GetSlotIndex() + 1;

			m_pkPlayer->ItemSetting(FigureItem);
			m_vDefaultFigureItem.push_back(FigureItem);
		}
	}
	
	m_pkPlayer->SPGOBPerformMessage( SPGM_ITEM_SC_TOGGLE_USE_WEAPON_SET, pkLocalPlayer->GetActiveWeapon() );
	
	ReleasePet();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitPlayerAnimation()
{
	if( m_pkPlayer == NULL )
		return;

	SPGOBModelUnit* pkModelUnit = m_pkPlayer->GetModelUnit();
	if( pkModelUnit == NULL )
		return;

	m_uiAniIndex = -1;
	pkModelUnit->SPSendMessage(MV_FRAMEINIT);
	SetAniSeq(10);
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::IsEquipPlayerLayer(int iItemID)
{
	if( g_pItemCluster == NULL )
		return false;

	SPItemAttr* pkItemAttr = NULL;
	g_pItemCluster->GetItemInfo(iItemID, pkItemAttr);
	if( pkItemAttr == NULL )
		return false;

	if( pkItemAttr->m_eType == ITEM_TYPE_FIGURE )
		return true;

	if( pkItemAttr->m_eType == ITEM_TYPE_CONSUME )
	{
		if( pkItemAttr->m_eSubType == ITEM_SUBTYPE_WASTE_DEFAULT )
			return true;
	}

	return false;
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::IsPetLayer(int iItemID)
{
	if( g_pItemCluster == NULL )
		return false;

	SPItemAttr* pkItemAttr = NULL;
	g_pItemCluster->GetItemInfo(iItemID, pkItemAttr);
	if( pkItemAttr == NULL )
		return false;

	if( pkItemAttr->m_eType == ITEM_TYPE_PET )
		return true;

	return false;
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::IsBugle(int iItemID)
{
	if( g_pItemCluster == NULL )
		return false;

	SPItemAttr* pkItemAttr = NULL;
	g_pItemCluster->GetItemInfo(iItemID, pkItemAttr);
	if( pkItemAttr == NULL )
		return false;

	if( pkItemAttr->m_iCategory == 1702 )
		return true;

	return false;
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::SetEquipPlayerLayer(int iItemID, bool bOff)
{
	if( g_pItemCluster == NULL || m_pkPlayer == NULL )
		return false;

	SPItemAttr* pkItemAttr = NULL;
	g_pItemCluster->GetItemInfo(iItemID, pkItemAttr);
	if( pkItemAttr == NULL )
		return false;

	int i;
	FIGURE_ITEM item;
	pkItemAttr->GetConvertFigureItem(&item);
	item.iItemID = iItemID;

	FIGURE_ITEM* pExistItem = GetDefaultFigureItem(item.SlotInfo.ContainerType, item.SlotInfo.SlotIndex);
	FIGURE_ITEM* pEquipItem = GetEquipFigureItem(item.SlotInfo.ContainerType, item.SlotInfo.SlotIndex);
	FIGURE_ITEM* pReserveEquipItem;

	FIGURE_ITEM	stEquipItem;

	if( pEquipItem != NULL )
	{
		stEquipItem.iItemID					=	pEquipItem->iItemID;
		stEquipItem.SlotInfo.ContainerType	=	pEquipItem->SlotInfo.ContainerType;
		stEquipItem.SlotInfo.SlotIndex		=	pEquipItem->SlotInfo.SlotIndex;

		pEquipItem	=	&stEquipItem;
	}

	// ÇÑ¹ú¿Ê
	if( (pkItemAttr->m_iCategory / 100) == 5 )
	{
		pReserveEquipItem = GetEquipFigureItem(item.SlotInfo.ContainerType, EQ_PANTS);
		if( pReserveEquipItem && !bOff )
		{
			m_pkPlayer->ItemSetting(*pReserveEquipItem, true);
			DeleteEquipFigureItem(pReserveEquipItem->SlotInfo.ContainerType,
				pReserveEquipItem->SlotInfo.SlotIndex, pReserveEquipItem->iItemID);
		}
		pReserveEquipItem = GetEquipFigureItem(item.SlotInfo.ContainerType, EQ_BLOUSE);
		if( pReserveEquipItem && !bOff )
		{
			m_pkPlayer->ItemSetting(*pReserveEquipItem, true);
			DeleteEquipFigureItem(pReserveEquipItem->SlotInfo.ContainerType,
				pReserveEquipItem->SlotInfo.SlotIndex, pReserveEquipItem->iItemID);
		}
	}
	else if( pkItemAttr->m_ePosID1 == EQ_PANTS )
	{
		pReserveEquipItem = GetEquipFigureItem(item.SlotInfo.ContainerType, EQ_BLOUSE);
		if( pReserveEquipItem && !bOff )
		{
			SPItemAttr* pkReserveItemAttr = NULL;
			g_pItemCluster->GetItemInfo(pReserveEquipItem->iItemID, pkReserveItemAttr);
			if( pkReserveItemAttr && ((pkReserveItemAttr->m_iCategory / 100) == 5) )
			{
				m_pkPlayer->ItemSetting(*pReserveEquipItem, true);
				DeleteEquipFigureItem(pReserveEquipItem->SlotInfo.ContainerType,
					pReserveEquipItem->SlotInfo.SlotIndex, pReserveEquipItem->iItemID);
			}
		}
	}
	
	if( pExistItem && !bOff )
		m_pkPlayer->ItemSetting(*pExistItem, true);

	if( bOff )
	{
		item.iItemID = 0;
		if( pEquipItem )
		{
			m_pkPlayer->ItemSetting(*pEquipItem, true);
			DeleteEquipFigureItem(pEquipItem->SlotInfo.ContainerType, pEquipItem->SlotInfo.SlotIndex, pEquipItem->iItemID);
			DeleteEffectList(pEquipItem->iItemID);

			if( pExistItem )
				item.iItemID = pExistItem->iItemID;
		}
	}
	else
	{
		if( pEquipItem )
		{
			DeleteEffectList(pEquipItem->iItemID);
			m_pkPlayer->ItemSetting(*pEquipItem, true);  
			if( pEquipItem->iItemID == iItemID )
			{
				DeleteEquipFigureItem(item.SlotInfo.ContainerType, item.SlotInfo.SlotIndex, iItemID);
				if( pExistItem )
					item.iItemID = pExistItem->iItemID;
				else
					item.iItemID = 0;
			}
			else
				pEquipItem->iItemID = iItemID;
		}
		else
			InsertEquipFigureItem(item);
	}
	
	if( item.iItemID )
	{
		//[xialin 2008/07/22] add >>>
		CONTAINER_ITEM	stWearItem;
		memset(&stWearItem, 0x00, sizeof(stWearItem));
		stWearItem.SlotInfo.ContainerType = INVENTORY_WEAR;
		stWearItem.SlotInfo.SlotIndex = 0;
		stWearItem.Item.iBackPrice = pkItemAttr->m_iCashPrice;
		stWearItem.Item.iItemExp = pkItemAttr->m_iPetMaxExp;
		stWearItem.Item.iItemID = item.iItemID;
		stWearItem.Item.iLifeTime = pkItemAttr->m_iLifeTime;
		stWearItem.Item.iOptPoint = pkItemAttr->m_iCashPoint;
		stWearItem.Item.iRare = pkItemAttr->m_eRareLimit;
		stWearItem.Item.iStackCount = pkItemAttr->m_iStackLimit;
		g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->ItemSetting(stWearItem, false);
		UpdateOZInven();
		//[xialin 2008/07/22] add <<<

		m_pkPlayer->ItemSetting(item);
		for( i = 0; i < MAX_ITEM_EFFECT; i++ )
		{
			if( pkItemAttr->m_iEffectID[i] == 0 )
				continue;

			AddEffectList(item.iItemID, GetEffectUniqueNo(), pkItemAttr->m_iEffectID[i]);
		}
	}	

	SetPlayerDefaultFigureItem();
	return true;
}

bool SPWindowCashShop::SetEquipPlayerLayer(SPCashItemAttr* pkCashItemAttr, bool bOff)
{
	if( g_pItemCluster == NULL || m_pkPlayer == NULL || pkCashItemAttr == NULL )
		return false;
			
	SPCashItemAttr* pkEquipItem = GetEquipCashItem(pkCashItemAttr);
	
	if ( pkEquipItem )
	{
		DeleteEquipCashItemByItem(pkEquipItem);
	}
	else
	{
		DeleteSameEquipCashItem(pkCashItemAttr);

		static  SPCashItemAttr*  pkLastCashItemAttr = NULL;
		if( pkCashItemAttr->eCategory == CI_PACKAGE )
			ResetPlayerLayer();
		else if ( pkLastCashItemAttr && pkLastCashItemAttr->eCategory == CI_PACKAGE 
				&& ( IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[0].iID) 
					|| IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[1].iID) 
					|| IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[2].iID) 
					|| IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[3].iID) 
					|| IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[4].iID) 
					|| IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[5].iID) 
					|| IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[6].iID) 
					|| IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[7].iID) ) )
			ResetPlayerLayer();

		pkLastCashItemAttr = pkCashItemAttr;

		InsertEquipCashItem(pkCashItemAttr);
		return true;
	}

	return false;

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetAniSeq(unsigned int uiAniIndex, bool bLoop)
{
	if( m_pkPlayer == NULL )
		return;

	if( m_uiAniIndex == uiAniIndex )
		return;

	if( m_uiAniIndex == 10 || m_uiAniIndex == 20 || m_uiAniIndex == 30 )
		m_uiBeforeAniIndex = m_uiAniIndex;

	m_uiAniIndex = uiAniIndex;
	SPGOBModelUnit*	pGOBModel = m_pkPlayer->GetModelUnit();
	pGOBModel->SPSendMessage(MV_SET_ANIMATION_INDEX, uiAniIndex);
	pGOBModel->SPSendMessage(MV_SET_ANI_LOOP, bLoop);

	if( bLoop )
		m_fAniMaxTime = 0.0f;
	else
	{
		pGOBModel->SPSendMessage(MV_GET_ANI_ACCUMUL_TIME, (WPARAM)&m_fAniMaxTime);
		m_fAniAccmulateTime = 0.0f;
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetPlayerDefaultFigureItem()
{
	SPPlayer* pkLocalPlayer;
	if( m_pkPlayer == NULL || (pkLocalPlayer = g_pGOBManager->GetLocalPlayer()) == NULL )
		return;

	SPPlayerInvenArchive* pkInvenArchive = pkLocalPlayer->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return;

	m_vDefaultFigureItem.clear();

	SPItem* pkItem;
	SPItemStatus* pkItemStatus;
	FIGURE_ITEM FigureItem;
	int iSize, i, j;
	CONTAINER_TYPE eFigureIndex[3+1] = {FIGURE_BASIC, FIGURE_EQUIP, FIGURE_FASHION, FIGURE_EFFECT};
	for( i = 0; i < 3+1; i++ )
	{
		iSize = pkInvenArchive->GetInventorySize(eFigureIndex[i]);
		for( j = 0; j < iSize; j++ )
		{
			pkItem = pkInvenArchive->GetInventoryItem(eFigureIndex[i], j);
			if( pkItem == NULL )
				continue;

			FigureItem.iItemID = pkItem->GetItemStatusValue(MV_ITEM_GETUID);
			if( FigureItem.iItemID == 0 )
				continue;

			pkItemStatus = pkItem->GetItemStatus();
			if( pkItemStatus == NULL || !pkItemStatus->IsTimeAvailable() )
				continue;

			FigureItem.SlotInfo.ContainerType = pkItem->GetContainerType();
			FigureItem.SlotInfo.SlotIndex = pkItem->GetSlotIndex() + 1;
			
			m_vDefaultFigureItem.push_back(FigureItem);
		}
	}
}

//------------------------------------------------------------------------------------
FIGURE_ITEM* SPWindowCashShop::GetDefaultFigureItem(CONTAINER_TYPE eType, int iSlotIndex)
{
	vector< FIGURE_ITEM >::iterator iter = m_vDefaultFigureItem.begin();
	while( iter != m_vDefaultFigureItem.end() )
	{
		if( (*iter).SlotInfo.ContainerType == eType &&
			(*iter).SlotInfo.SlotIndex == iSlotIndex )
			return &(*iter);

		++iter;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
FIGURE_ITEM* SPWindowCashShop::GetEquipFigureItem(CONTAINER_TYPE eType, int iSlotIndex)
{
	vector< FIGURE_ITEM >::iterator iter = m_vEquipFigureItem.begin();
	while( iter != m_vEquipFigureItem.end() )
	{
		if( (*iter).SlotInfo.ContainerType == eType &&
			(*iter).SlotInfo.SlotIndex == iSlotIndex )
			return &(*iter);

		++iter;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InsertEquipFigureItem(FIGURE_ITEM& item)
{
	m_vEquipFigureItem.push_back(item);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::DeleteEquipFigureItem(CONTAINER_TYPE eType, int iSlotIndex, int iItemID)
{
	vector< FIGURE_ITEM >::iterator iter = m_vEquipFigureItem.begin();
	while( iter != m_vEquipFigureItem.end() )
	{
		if( (*iter).SlotInfo.ContainerType == eType &&
			(*iter).SlotInfo.SlotIndex == iSlotIndex &&
			(*iter).iItemID == iItemID )
		{
			m_vEquipFigureItem.erase(iter);
			break;
		}

		++iter;
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InsertEquipCashItem(SPCashItemAttr* pkCashItemAttr)
{
	if ( !pkCashItemAttr || !m_pkPlayer )
		return;

	vector< SPCashItemAttr* >::iterator iter = find(m_vEquipCashItem.begin(), m_vEquipCashItem.end(), pkCashItemAttr);
	if ( iter == m_vEquipCashItem.end() )
	{
		bool   bEquipItem = false;
		for( int i = 0; i < 8; i++ )
		{
			if( pkCashItemAttr->ItemInfo[i].iID == 0 )
				continue;

			if( IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[i].iID) )
			{
				bEquipItem = true;

				SPItemAttr* pkItemAttr = NULL;
				g_pItemCluster->GetItemInfo(pkCashItemAttr->ItemInfo[i].iID, pkItemAttr);
				if( pkItemAttr == NULL )
					continue;

				FIGURE_ITEM item;
				pkItemAttr->GetConvertFigureItem(&item);
				item.iItemID = pkCashItemAttr->ItemInfo[i].iID;
				
				m_pkPlayer->ItemSetting(item);
				for( int j = 0; j < MAX_ITEM_EFFECT; j++ )
				{
					if( pkItemAttr->m_iEffectID[j] == 0 )
						continue;

					AddEffectList(item.iItemID, GetEffectUniqueNo(), pkItemAttr->m_iEffectID[j]);
				}
				SetPlayerDefaultFigureItem();

				CONTAINER_ITEM	stWearItem;
				memset(&stWearItem, 0x00, sizeof(stWearItem));
				stWearItem.SlotInfo.ContainerType = INVENTORY_WEAR;
				stWearItem.SlotInfo.SlotIndex = 0;
				stWearItem.Item.iBackPrice = pkItemAttr->m_iCashPrice;
				stWearItem.Item.iItemExp = pkItemAttr->m_iPetMaxExp;
				stWearItem.Item.iItemID = item.iItemID;
				stWearItem.Item.iLifeTime = pkItemAttr->m_iLifeTime;
				stWearItem.Item.iOptPoint = pkItemAttr->m_iCashPoint;
				stWearItem.Item.iRare = pkItemAttr->m_eRareLimit;
				stWearItem.Item.iStackCount = pkItemAttr->m_iStackLimit;
				g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->ItemSetting(stWearItem, false);
				UpdateOZInven();

			}	
			else if( IsPetLayer(pkCashItemAttr->ItemInfo[i].iID) )
			{
				bEquipItem = true;
				if( pkCashItemAttr->iClassID )
					SetPet(pkCashItemAttr->iClassID, pkCashItemAttr->ItemInfo[i].iID);
			}
			else if ( IsBugle(pkCashItemAttr->ItemInfo[i].iID) )
			{
			}

			if( pkCashItemAttr->iMotionID )
				SetAniSeq(pkCashItemAttr->iMotionID, false);
		}

		if ( bEquipItem )
			m_vEquipCashItem.push_back(pkCashItemAttr);

	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::DeleteEquipCashItemByItem(SPCashItemAttr* pkCashItemAttr)
{
	if ( !pkCashItemAttr || !m_pkPlayer )
		return;

	vector< SPCashItemAttr* >::iterator iter = find(m_vEquipCashItem.begin(), m_vEquipCashItem.end(), pkCashItemAttr);
	if ( iter != m_vEquipCashItem.end() )
	{
		m_vEquipCashItem.erase(iter);
	}

	for( int i = 0; i < 8; i++ )
	{
		if( pkCashItemAttr->ItemInfo[i].iID == 0 )
			continue;

		if( IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[i].iID) )
		{
			SPItemAttr* pkItemAttr = NULL;
			g_pItemCluster->GetItemInfo(pkCashItemAttr->ItemInfo[i].iID, pkItemAttr);
			if( pkItemAttr == NULL )
				continue;

			FIGURE_ITEM item;
			pkItemAttr->GetConvertFigureItem(&item);
			item.iItemID = pkCashItemAttr->ItemInfo[i].iID;
			
			m_pkPlayer->ItemSetting(item, true);
			DeleteEffectList(item.iItemID);
		}
		else if( IsPetLayer(pkCashItemAttr->ItemInfo[i].iID) )
		{
			if( m_pkPlayer && pkCashItemAttr->iClassID )
				m_pkPlayer->SPGOBPerformMessage(SPGM_SET_PET, -1);	
		}
	}

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::DeleteSameEquipCashItem(SPCashItemAttr* pkCashItemAttr)
{
	if ( !pkCashItemAttr || !m_pkPlayer )
		return;

	vector< SPCashItemAttr* >::iterator iter = m_vEquipCashItem.begin();
	while( iter != m_vEquipCashItem.end() )
	{
		bool   bIterAdd = true;
		for( int i = 0; i < 8; i++ )
		{
			for( int j = 0; j < 8; j++ )
			{
				if( pkCashItemAttr->ItemInfo[i].iID == 0 )
					break;
				if ( (*iter)->ItemInfo[j].iID == 0 )
					continue;

				if( IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[i].iID)
					&& IsEquipPlayerLayer((*iter)->ItemInfo[j].iID) )
				{
					SPItemAttr* pkItemAttr1 = NULL, * pkItemAttr2 = NULL;
					g_pItemCluster->GetItemInfo(pkCashItemAttr->ItemInfo[i].iID, pkItemAttr1);
					g_pItemCluster->GetItemInfo((*iter)->ItemInfo[j].iID, pkItemAttr2);
					if( pkItemAttr1 == NULL ) 
						break;
					if ( pkItemAttr2 == NULL )
						continue;

					FIGURE_ITEM item1, item2;
					pkItemAttr1->GetConvertFigureItem(&item1);
					pkItemAttr2->GetConvertFigureItem(&item2);
					item1.iItemID = pkCashItemAttr->ItemInfo[i].iID;
					item2.iItemID = (*iter)->ItemInfo[j].iID;

					if( (pkItemAttr1->m_iCategory / 100) == 5 )
					{
						if ( item1.SlotInfo.ContainerType == item2.SlotInfo.ContainerType
							&& (item2.SlotInfo.SlotIndex == EQ_PANTS || item2.SlotInfo.SlotIndex == EQ_BLOUSE) )
						{
							iter = m_vEquipCashItem.erase(iter);
							m_pkPlayer->ItemSetting(item2, true);
							DeleteEffectList(item2.iItemID);
							bIterAdd = false;
							break;
						}
					}
					else if( pkItemAttr1->m_ePosID1 == EQ_PANTS )
					{
						if ( item1.SlotInfo.ContainerType == item2.SlotInfo.ContainerType
							&& item2.SlotInfo.SlotIndex == EQ_BLOUSE )
						{
							SPItemAttr* pkReserveItemAttr = NULL;
							g_pItemCluster->GetItemInfo(item2.iItemID, pkReserveItemAttr);
							if( pkReserveItemAttr && ((pkReserveItemAttr->m_iCategory / 100) == 5) )
							{
								iter = m_vEquipCashItem.erase(iter);
								m_pkPlayer->ItemSetting(item2, true);
								DeleteEffectList(item2.iItemID);
								bIterAdd = false;
								break;
							}
						}
					}

					if ( item1.SlotInfo.ContainerType == item2.SlotInfo.ContainerType
						&& item1.SlotInfo.SlotIndex == item2.SlotInfo.SlotIndex )
					{
						iter = m_vEquipCashItem.erase(iter);
						m_pkPlayer->ItemSetting(item2, true);
						DeleteEffectList(item2.iItemID);
						bIterAdd = false;
						break;
					}
				}
				else if( IsPetLayer(pkCashItemAttr->ItemInfo[i].iID)
						&& IsPetLayer((*iter)->ItemInfo[j].iID) )
				{
					if( m_pkPlayer )
						m_pkPlayer->SPGOBPerformMessage(SPGM_SET_PET, -1);	

					iter = m_vEquipCashItem.erase(iter);
					bIterAdd = false;
					break;
				}
			}
			if ( bIterAdd == false )
				break;
		}
		if ( bIterAdd )
			++iter;
	}

}

//------------------------------------------------------------------------------------
SPCashItemAttr* SPWindowCashShop::GetEquipCashItem(SPCashItemAttr* pkCashItemAttr)
{
	vector< SPCashItemAttr* >::iterator iter = find(m_vEquipCashItem.begin(), m_vEquipCashItem.end(), pkCashItemAttr);
	if ( iter != m_vEquipCashItem.end() )
	{
		return (*iter);
	}

	return NULL;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetPet(unsigned int uiClassID, int iItemID)
{
	ReleasePet();

	m_pkPet = new SPPet(1, (CLASS_ID)uiClassID, NULL);
	m_pkPet->SetJustRender(true);
	m_pkPet->Init();
	m_pkPet->SetJustRenderPos(61, 195);

	//[xialin 2008/07/29]add
	m_pkPlayer->SPGOBPerformMessage(SPGM_SET_PET, iItemID);

	m_pkPet->SetJustRenderAnimation(0, true);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ReleasePet()
{
	SAFE_DELETE(m_pkPet);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ClearEffectList()
{
	map< int, vector< int > >::iterator iter = m_mEffectList.begin();
	while( iter != m_mEffectList.end() )
	{
		(*iter).second.clear();
		++iter;
	}
	m_mEffectList.clear();

	m_iEffectUniqueNo = 0;
	if( m_pkPlayer )
		m_pkPlayer->SPGOBPerformMessage(SPGM_SET_EFFECT_ALL_CLEAR);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::AddEffectList(int iItemID, int iUniqueNo, int iEffectID)
{
	map< int, vector< int > >::iterator iterKey = m_mEffectList.find(iItemID);
	if( iterKey == m_mEffectList.end() )
	{
		vector< int > vEffect;
		vEffect.push_back(iUniqueNo);

		m_mEffectList.insert(make_pair(iItemID, vEffect));
		vEffect.clear();
	}
	else
	{
		vector< int >::iterator iterEffect = (*iterKey).second.begin();
		while( iterEffect != (*iterKey).second.end() )
		{
			if( (*iterEffect) == iUniqueNo )
				return;

			++iterEffect;
		}

		(*iterKey).second.push_back(iUniqueNo);
	}

	if( m_pkPlayer )
	{
		unsigned short iNumofEffect = 1;
		
		CPacket kPacket;
		kPacket.AddUINT16(iNumofEffect);
		
		EFFECT_DATA  EffectData;
		EffectData.iCasterID = m_pkPlayer->GetGUID();
		EffectData.iEffectID = iEffectID;
		EffectData.iEffectLv = 1;
		EffectData.iRemainTime = 0;
		EffectData.iInstanceID = iUniqueNo;

		kPacket.Add(&EffectData, sizeof(EFFECT_DATA));
		m_pkPlayer->SPGOBPerformMessage(SPGM_SET_EFFECT_ACTIVITY, (LPARAM)&kPacket);
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::DeleteEffectList(int iItemID)
{
	map< int, vector< int > >::iterator iterKey = m_mEffectList.find(iItemID);
	if( iterKey == m_mEffectList.end() )
		return;

	unsigned short iNumofEffect = (unsigned short)(*iterKey).second.size();
	
	CPacket kPacket;
	kPacket.AddUINT16(iNumofEffect);

	vector< int >::iterator iterEffect = (*iterKey).second.begin();
	while( iterEffect != (*iterKey).second.end() )
	{
		kPacket.AddUINT32(*iterEffect);
		++iterEffect;
	}
	(*iterKey).second.clear();
	m_mEffectList.erase(iterKey);

	if( m_pkPlayer )
		m_pkPlayer->SPGOBPerformMessage(SPGM_SET_EFFECT_DELETE, (LPARAM)&kPacket);
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::GetEffectUniqueNo()
{
	return m_iEffectUniqueNo++;
}

//------------------------------------------------------------------------------------
// Message
int SPWindowCashShop::OnSetPreViewInvenNormal(WPARAM wParam, LPARAM lParam)
{
	SetAniSeq(10);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetPreViewInvenRun(WPARAM wParam, LPARAM lParam)
{
	SetAniSeq(20);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetPreViewInvenLadder(WPARAM wParam, LPARAM lParam)
{
	SetAniSeq(30);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetPreViewInvenCharInit(WPARAM wParam, LPARAM lParam)
{
	ResetPlayerLayer();
	ClearPreViewSelect();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnPreViewInvenLeftPage(WPARAM wParam, LPARAM lParam)
{
	m_iPreViewInvenCurPage--;
	if( m_iPreViewInvenCurPage < 0 )
		m_iPreViewInvenCurPage = 0;
	else
	{
		RefreshPreViewInven();
		SetSelectSlot(-1, false);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnPreViewInvenRightPage(WPARAM wParam, LPARAM lParam)
{
	m_iPreViewInvenCurPage++;
	if( m_iPreViewInvenCurPage >= m_iPreViewInvenMaxPage )
		m_iPreViewInvenCurPage = m_iPreViewInvenMaxPage - 1;
	else
	{
		RefreshPreViewInven();
		SetSelectSlot(-1, false);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnPreViewInvenLButtonDown(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)lParam;
	SPCashItemAttr* pkCashItemAttr = GetPreViewInvenItem(iSlotIndex);
	if( pkCashItemAttr == NULL )
	{
		ClearPreViewSelect();
		return 1;
	}

	SetSelectSlot(iSlotIndex, true);

	SPWindow* pkWindow = Find(WIID_CASHSHOP_PREVIEW_PRICE, true);
	if( pkWindow )
	{
		int iPrice = pkCashItemAttr->iPrice;
		if( pkCashItemAttr->iSalePrice )
			iPrice = pkCashItemAttr->iSalePrice;
		//CaluCashierGlow(iPrice);
		//int iNum10 = (iPrice / 10) % 10;
		//if( iNum10 )
		//	iPrice += 100 - (iNum10 * 10);

		pkWindow->SetWindowText(ConvertMoneyToString(iPrice));
	}

	if( pkCashItemAttr->iGender )
	{
		if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
			return 1;

		int iCheckValue = (int)g_pGOBManager->GetLocalPlayer()->GetCheckStatusValue(CHECK_STATUS_GENDER);
		if( pkCashItemAttr->iGender != iCheckValue )
			return 1;
	}
	
	if( pkCashItemAttr->eCategory == CI_PACKAGE )
		ResetPlayerLayer();

	for( int i = 0; i < 8; i++ )
	{
		if( pkCashItemAttr->ItemInfo[i].iID == 0 )
			break;

		if( IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[i].iID) )
			SetEquipPlayerLayer(pkCashItemAttr->ItemInfo[i].iID);
		else if( IsPetLayer(pkCashItemAttr->ItemInfo[i].iID) )
		{
			if( pkCashItemAttr->iClassID )
				SetPet(pkCashItemAttr->iClassID, pkCashItemAttr->ItemInfo[i].iID);
		}

		if( pkCashItemAttr->iMotionID )
			SetAniSeq(pkCashItemAttr->iMotionID, false);
	}

	return 1;
}


//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetPreViewInvenDel(WPARAM wParam, LPARAM lParam)
{
	if( m_iPreSelectSlot < 0 || !m_bPreViewSelect )
		return 1;

	SPCashItemAttr* pkCashItemAttr = GetPreViewInvenItem(m_iPreSelectSlot);
	if( pkCashItemAttr )
	{
		int i;
		for( i = 0; i < CASHITEMINFO_COUNT; i++ )
		{
			if( pkCashItemAttr->ItemInfo[i].iID == 0 )
				continue;

			SetEquipPlayerLayer(pkCashItemAttr->ItemInfo[i].iID, true);
		}
	}

	InsertPreViewInven(m_iPreSelectSlot + m_iPreViewInvenCurPage * 8, NULL);
	//ResetPlayerLayer();
	ClearPreViewSelect();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetPreViewInvenInit(WPARAM wParam, LPARAM lParam)
{
	ClearPreViewInven();
	//ResetPlayerLayer();
	ClearPreViewSelect();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetPreViewInvenBuy(WPARAM wParam, LPARAM lParam)
{
	if( m_iPreSelectSlot < 0 || !m_bPreViewSelect )
		return 1;

	SPCashItemAttr* pkCashItemAttr = GetPreViewInvenItem(m_iPreSelectSlot);
	ShowCashierDlg(pkCashItemAttr);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnPreViewToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)lParam;
	SPCashItemAttr* pkCashItemAttr = GetPreViewInvenItem(iSlotIndex);
	SetStoreToolTipEnter(pkCashItemAttr, lParam);
	
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnPreViewToolTipOut(WPARAM wParam, LPARAM lParam)
{
	SetStoreToolTipOut(lParam);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnPreViewToolTipMove(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)lParam;
	SPCashItemAttr* pkCashItemAttr = GetPreViewInvenItem(iSlotIndex);
	if( pkCashItemAttr )
		SetStoreToolTipMove(pkCashItemAttr, lParam);
	else
		SetStoreToolTipOut(lParam);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetPlayerViewBuy(WPARAM wParam, LPARAM lParam)
{
	m_iCurStartItem = 0;
	m_vecBuyCheck.resize(m_vEquipCashItem.size());
	for ( int i=0; i < static_cast<int>(m_vecBuyCheck.size()); i++ )
	{
		if ( IsBuyCashItemList(m_vEquipCashItem[i]) )
		{
			m_vecBuyCheck[i] = false;
		}
		else
		{
			m_vecBuyCheck[i] = true;
		}
	}

	RefreshBuyListDlg();

	SPWindow * pkParentWindow = NULL;
	pkParentWindow = Find(WIID_CASHSHOP_BUYLIST);
	if ( !pkParentWindow )
		return 1;

	pkParentWindow->Show();
	g_pInterfaceManager->SetModalWindow(pkParentWindow);

	SPWindow * pkChild = Find(WIID_CASHSHOP_BUYLIST_TIP, true);
	if ( pkChild )
	{
		if (m_vEquipCashItem.size() > 0)
			pkChild->Hide();
		else
			pkChild->Show();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetPlayerViewWearInfo(WPARAM wParam, LPARAM lParam)
{
	m_iCurStartItem = 0;
	RefreshWearListDlg();
	
	SPWindow * pkParentWindow = NULL;
	pkParentWindow = Find(WIID_CASHSHOP_WEARLIST);
	if ( !pkParentWindow )
		return 1;

	pkParentWindow->Show();
	g_pInterfaceManager->SetModalWindow(pkParentWindow);
	
	SPWindow * pkChild = Find(WIID_CASHSHOP_WEARLIST_TIP, true);
	if ( pkChild )
	{
		if (m_vEquipCashItem.size() > 0)
			pkChild->Hide();
		else
			pkChild->Show();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetPlayerViewExact(WPARAM wParam, LPARAM lParam)
{
	m_iCurStartItem = 0;
	m_vecBuyCheck.resize(m_vEquipCashItem.size());
	for ( int i=0; i < static_cast<int>(m_vecBuyCheck.size()); i++ )
	{
		m_vecBuyCheck[i] = true;
	}

	RefreshExactListDlg();
	

	SPWindow * pkParentWindow = NULL;
	pkParentWindow = Find(WIID_CASHSHOP_EXACTLIST);
	if ( !pkParentWindow )
		return 1;

	SPWindow * pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_EXACTLIST_RECEIVE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(NULL);

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_EXACTLIST_MESSAGE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(NULL);
	
	pkParentWindow->Show();
	g_pInterfaceManager->SetModalWindow(pkParentWindow);
	
	UpdateFriendList();
	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_EXACTLIST_FRIEND, true);
	if( pkChildWindow )
		pkChildWindow->Hide();
	
	SPWindow * pkChild = Find(WIID_CASHSHOP_EXACTLIST_TIP, true);
	if ( pkChild )
	{
		if (m_vEquipCashItem.size() > 0)
			pkChild->Hide();
		else
			pkChild->Show();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactListFriend(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_EXACTLIST_FRIEND, true);
	if( pkWindow )
	{
		if( pkWindow->IsShow() )
			pkWindow->Hide();
		else
			pkWindow->Show();
	}

	return 1;
}
