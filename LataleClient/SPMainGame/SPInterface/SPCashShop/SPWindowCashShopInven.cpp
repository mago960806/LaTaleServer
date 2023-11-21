#include "SPCommon.h"
#include "SPUtil.h"
#include "SPWindowDEF.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowGrid.h"
#include "SPWindowSlider.h"
#include "SPWindowCashShop.h"

#include "SPSubGameManager.h"

#include "SPGOBManager.h"
#include "SPGameObject.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"

#include "SPItem.h"
#include "SPCheckManager.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetItemInvenCategory(int iType)
{
	SPWindow* pkWindow, *pkChildWindow;

	pkWindow = Find(WIID_CASHSHOP_ITEM_INVEN);
	if( pkWindow == NULL )
		return;

	int i = WIID_CASHSHOP_ITEM_INVEN_EQUIP;
	for( ; i <= WIID_CASHSHOP_ITEM_INVEN_HOUSING; i++ )
	{
		BOOL bCheck = FALSE;
		if( iType == (i - WIID_CASHSHOP_ITEM_INVEN_EQUIP) )
			bCheck = TRUE;

		pkChildWindow = pkWindow->Find(i, true);
		if( pkChildWindow )
			pkChildWindow->SPSendMessage(SPIM_BTN_SELECT, bCheck);
	}

	if( m_iItemInvenCategory != iType )
	{
		m_iItemInvenCategory = iType;
		RefreshItemInven();
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetOZInvenCategory(int iType)
{
	SPWindow* pkWindow, *pkChildWindow;

	pkWindow = Find(WIID_CASHSHOP_OZ_INVEN);
	if( pkWindow == NULL )
		return;

	int i = WIID_CASHSHOP_OZ_INVEN_BUYINFO;
	for( ; i <= WIID_CASHSHOP_OZ_INVEN_WEARINFO; i++ )
	{
		BOOL bCheck = FALSE;
		if( iType == (i - WIID_CASHSHOP_OZ_INVEN_BUYINFO) )
			bCheck = TRUE;

		pkChildWindow = pkWindow->Find(i, true);
		if( pkChildWindow )
			pkChildWindow->SPSendMessage(SPIM_BTN_SELECT, bCheck);
	}

	if( m_iOZInvenCategory != iType )
	{
		m_iOZInvenCategory = iType;
		m_iOZInvenCurPage[0] = 0;
		m_iOZInvenCurPage[1] = 0;
		UpdateOZInven();
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::RefreshItemInven()
{
	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return;
	
	SPWindowGrid* pkWindowGrid = static_cast<SPWindowGrid*>(Find(WIID_CASHSHOP_ITEM_INVEN_SLOT, true));
	if( pkWindowGrid == NULL )
		return;

	pkWindowGrid->ClearCellAll();

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return;

	int iLineSize;
	if( m_iItemInvenCategory == 4 )
	{
		int iSize = pkInvenArchive->GetInventorySize(INVENTORY_PET);
		iLineSize = iSize / INVENTORY_LINE;
		if( iLineSize > 4 )
			iLineSize = 4;	
	}
	else
		iLineSize = 4;

	pkWindowGrid->SetShowLine(iLineSize);
	
	int iSize, iStartIndex, i;
	SPItem* pkItem;

	iStartIndex = m_iItemInvenCurPage[m_iItemInvenCategory] * INVENTORY_LINE;
	iSize = pkInvenArchive->GetInventorySize((CONTAINER_TYPE)m_iItemInvenCategory);
	for( i = 0; i < VIEW_SLOT; i++ )
	{
		if( (iStartIndex + i) >= iSize )
			break;

		pkItem = pkInvenArchive->GetInventoryItem((CONTAINER_TYPE)m_iItemInvenCategory, iStartIndex + i);
		if( pkItem == NULL )
			continue;

		CONTAINER_ITEM Item;
		pkItem->ConvertContainerItem(&Item);

		bool bEnable = g_pCheckManager->CheckItemStatus(Item.Item.iItemID);	
		Item.SlotInfo.SlotIndex = Item.SlotInfo.SlotIndex - (m_iItemInvenCurPage[m_iItemInvenCategory] * INVENTORY_LINE);
		
		POINT ptSrcPos;	
		SPTexture* pkTexture = g_pItemCluster->GetItemTexture(Item.Item.iItemID, ptSrcPos); 	

		pkWindowGrid->SettingIconImage(pkTexture, Item.Item.iItemID,
			ptSrcPos.x, ptSrcPos.y, Item.SlotInfo.SlotIndex, Item.Item.iStackCount, bEnable);
	}

	int iMaxInvenPage = pkInvenArchive->GetMaxInvenPage(m_iItemInvenCategory);
	if( m_iItemInvenMaxPage[m_iItemInvenCategory] != iMaxInvenPage )
		m_iItemInvenMaxPage[m_iItemInvenCategory] = iMaxInvenPage;

	SetSliderMaxPage(1, iMaxInvenPage + 1);
	SetSliderPage(1, m_iItemInvenCurPage[m_iItemInvenCategory]);
}

//------------------------------------------------------------------------------------
SPItem* SPWindowCashShop::GetItemInvenItem(int iSlotIndex)
{
	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return NULL;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return NULL;

	int iSize, iStartIndex;
	iStartIndex = m_iItemInvenCurPage[m_iItemInvenCategory] * INVENTORY_LINE;
	iSize = pkInvenArchive->GetInventorySize((CONTAINER_TYPE)m_iItemInvenCategory);

	if( (iStartIndex + iSlotIndex) >= iSize )
		return NULL;

	 return pkInvenArchive->GetInventoryItem((CONTAINER_TYPE)m_iItemInvenCategory, iStartIndex + iSlotIndex);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::EmptyItemInvenSlot()
{
	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return;

	if( pkInvenArchive->IsFullInventory((CONTAINER_TYPE)m_iItemInvenCategory) )
		return;

	SPItem* pkItem;
	int iSize, i;
	iSize = pkInvenArchive->GetInventorySize((CONTAINER_TYPE)m_iItemInvenCategory);
	for( i = 0; i < iSize; i++ )
	{
		if( i >= iSize )
			break;

		pkItem = pkInvenArchive->GetInventoryItem((CONTAINER_TYPE)m_iItemInvenCategory, i);
		if( pkItem == NULL )
		{
			int iIndex = (i / INVENTORY_LINE) - 3;
			if( iIndex > 0 )
				m_iItemInvenCurPage[m_iItemInvenCategory] = iIndex;
				
			return;
		}
	}
}

//------------------------------------------------------------------------------------
// Message
int SPWindowCashShop::OnItemInvenUp(WPARAM wParam, LPARAM lParam)
{
	m_iItemInvenCurPage[m_iItemInvenCategory]--;
	if( m_iItemInvenCurPage[m_iItemInvenCategory] < 0 )
		 m_iItemInvenCurPage[m_iItemInvenCategory] = 0;
	else
		RefreshItemInven();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnItemInvenDown(WPARAM wParam, LPARAM lParam)
{
	m_iItemInvenCurPage[m_iItemInvenCategory]++;
	if( m_iItemInvenCurPage[m_iItemInvenCategory] > m_iItemInvenMaxPage[m_iItemInvenCategory] )
		m_iItemInvenCurPage[m_iItemInvenCategory] = m_iItemInvenMaxPage[m_iItemInvenCategory];
	else
		RefreshItemInven();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetItemInvenUp(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_ITEM_INVEN_THUMB, true);
	if( pkWindow )
		pkWindow->SPSendMessage(pkWindow, SPIM_WHEEL_UP, wParam, lParam);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetItemInvenDown(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_ITEM_INVEN_THUMB, true);
	if( pkWindow )
		pkWindow->SPSendMessage(pkWindow, SPIM_WHEEL_DOWN, wParam, lParam);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetItemInvenCurPage(WPARAM wParam, LPARAM lParam)
{
	if( m_iItemInvenCurPage[m_iItemInvenCategory] != wParam )
	{
		m_iItemInvenCurPage[m_iItemInvenCategory] = (int)wParam;
		RefreshItemInven();
	}
	
	return 1;
}

//------------------------------------------------------------------------------------
// Message
int SPWindowCashShop::OnOZInvenUp(WPARAM wParam, LPARAM lParam)
{
	m_iOZInvenCurPage[m_iOZInvenCategory]--;
	if( m_iOZInvenCurPage[m_iOZInvenCategory] < 0 )
		 m_iOZInvenCurPage[m_iOZInvenCategory] = 0;
	else
		UpdateOZInven();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZInvenDown(WPARAM wParam, LPARAM lParam)
{
	m_iOZInvenCurPage[m_iOZInvenCategory]++;
	if( m_iOZInvenCurPage[m_iOZInvenCategory] > m_iOZInvenMaxPage[m_iOZInvenCategory] )
		m_iOZInvenCurPage[m_iOZInvenCategory] = m_iOZInvenMaxPage[m_iOZInvenCategory];
	else
		UpdateOZInven();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetOZInvenUp(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_OZ_INVEN_THUMB, true);
	if( pkWindow )
		pkWindow->SPSendMessage(pkWindow, SPIM_WHEEL_UP, wParam, lParam);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetOZInvenDown(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_OZ_INVEN_THUMB, true);
	if( pkWindow )
		pkWindow->SPSendMessage(pkWindow, SPIM_WHEEL_DOWN, wParam, lParam);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetOZInvenCurPage(WPARAM wParam, LPARAM lParam)
{
	if( m_iOZInvenCurPage[m_iOZInvenCategory] != wParam )
	{
		m_iOZInvenCurPage[m_iOZInvenCategory] = (int)wParam;
		UpdateOZInven();
	}
	
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnInvenCategory(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	SetItemInvenCategory(iID - WIID_CASHSHOP_ITEM_INVEN_EQUIP);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZInvenCategory(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	SetOZInvenCategory(iID - WIID_CASHSHOP_OZ_INVEN_BUYINFO);
	return 1;
}


//------------------------------------------------------------------------------------
int SPWindowCashShop::OnItemInvenCursorMove(WPARAM wParam, LPARAM lParam)
{
	if( m_pkSelectItem == NULL && m_iSelectSlot >= 0 && m_iSelectInven == 0 )
	{
		m_pkSelectItem = GetItemInvenItem(m_iSelectSlot);
		if( m_pkSelectItem )
		{
			POINT ptSrcPos;
			SPTexture* pkTexture = g_pItemCluster->GetItemTexture( m_pkSelectItem->GetItemStatusValue(MV_ITEM_GETUID), ptSrcPos);
			g_pInterfaceManager->SetMousePickup(pkTexture, ptSrcPos.x, ptSrcPos.y);

			SPWindowGrid* pkGridWindow =
				static_cast< SPWindowGrid* >(Find(WIID_CASHSHOP_ITEM_INVEN_SLOT, true));
			if( pkGridWindow )
				pkGridWindow->CellSelectSell(m_iSelectSlot, true);
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnItemInvenLButtonDown(WPARAM wParam, LPARAM lParam)
{
	m_iSelectSlot = (int)lParam;
	m_iSelectInven = 0;
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnItemInvenLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if( m_iSelectSlot < 0 || m_iSelectInven < 0 || g_pItemCluster == NULL )
		return 1;

	int iDstSlotIndex = (int)lParam;
	if( iDstSlotIndex < 0 )
		return 1;

	CHANGE_SLOT ChangeSlot;
	if( m_pkSelectItem )
	{
		if( g_pCheckManager->IsDBBlock() )
			return 1;
		
		int iStartIndex = m_iItemInvenCurPage[m_iItemInvenCategory] * INVENTORY_LINE;
		if( (m_pkSelectItem->GetSlotIndex() != iStartIndex + iDstSlotIndex) || 
			(m_pkSelectItem->GetContainerType() != m_iItemInvenCategory) )
		{
			ChangeSlot.SrcSlotInfo.ContainerType = m_pkSelectItem->GetContainerType();
			ChangeSlot.SrcSlotInfo.SlotIndex = m_pkSelectItem->GetSlotIndex() + 1;
			ChangeSlot.iCount = m_pkSelectItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
			
			ChangeSlot.DestSlotInfo.ContainerType = (CONTAINER_TYPE)m_iItemInvenCategory;
			ChangeSlot.DestSlotInfo.SlotIndex = iStartIndex + iDstSlotIndex + 1;

			CPacket kPacket;
			kPacket.Add((UINT32)ITEM_CS_CHANGE_SLOT);
			kPacket.AddData(&ChangeSlot, sizeof(CHANGE_SLOT));	
			(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);

			//if( m_pkSelectItem == m_pSellItem )
			//ClearOZSellInfo();
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnItemInvenToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	if( m_pkSelectItem )
	{
		SPItemAttr* pItemAttr = m_pkSelectItem->GetItemAttr();
		if( pItemAttr )
		{
			int iType = pItemAttr->m_eType;
			if( iType != 0 && (iType - 1) != m_iItemInvenCategory )
			{
				SetItemInvenCategory(iType - 1);
				EmptyItemInvenSlot();
				RefreshItemInven();
			}
		}
	}

	int iSlotIndex = (int)lParam;
	SPItem* pkItem = GetItemInvenItem(iSlotIndex);
	if( pkItem == NULL )
		return 1;

	SPItemStatus* pkItemStatus = pkItem->GetItemStatus();
	if( pkItemStatus == NULL )
		return 1;
	
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)pkItemStatus, lParam);
	pkWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnItemInvenToolTipOut(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, lParam);
	pkWindow->Show();

	return 1;
}
//------------------------------------------------------------------------------------
int SPWindowCashShop::OnItemInvenToolTipMove(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)lParam;
	SPItem* pkItem = GetItemInvenItem(iSlotIndex);
	if( pkItem == NULL )
		return 1;

	SPItemStatus* pkItemStatus = pkItem->GetItemStatus();
	if( pkItemStatus == NULL )
		return 1;

	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)pkItemStatus, lParam);
	pkWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnAddParcel(WPARAM wParam, LPARAM lParam)
{
	SPCashEffectItem* pkCashItem = g_pItemCluster->GetEffectByID(EFT_ADDPARCEL);
	if ( pkCashItem )
	{
		char  szMsg[256];
		sprintf( szMsg, "%s(价格:%d点券)", pkCashItem->strName.c_str(), pkCashItem->iCashNum );
		g_pInterfaceManager->MessageBox(szMsg, MES_OKCANCEL, this->GetInstanceID(), SPIM_CASHSHOP_MSG_ADDPARCEL);
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnAddConsume(WPARAM wParam, LPARAM lParam)
{
	SPCashEffectItem* pkCashItem = g_pItemCluster->GetEffectByID(EFT_ADDCONSUME);
	if ( pkCashItem )
	{
		char  szMsg[256];
		sprintf( szMsg, "%s(价格:%d点券)", pkCashItem->strName.c_str(), pkCashItem->iCashNum );
		g_pInterfaceManager->MessageBox(szMsg, MES_OKCANCEL, this->GetInstanceID(), SPIM_CASHSHOP_MSG_ADDCONSUME);
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnAddETC(WPARAM wParam, LPARAM lParam)
{
	SPCashEffectItem* pkCashItem = g_pItemCluster->GetEffectByID(EFT_ADDETC);
	if ( pkCashItem )
	{
		char  szMsg[256];
		sprintf( szMsg, "%s(价格:%d点券)", pkCashItem->strName.c_str(), pkCashItem->iCashNum );
		g_pInterfaceManager->MessageBox(szMsg, MES_OKCANCEL, this->GetInstanceID(), SPIM_CASHSHOP_MSG_ADDETC);
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnAddEvent(WPARAM wParam, LPARAM lParam)
{
	SPCashEffectItem* pkCashItem = g_pItemCluster->GetEffectByID(EFT_ADDEVENT);
	if ( pkCashItem )
	{
		char  szMsg[256];
		sprintf( szMsg, "%s(价格:%d点券)", pkCashItem->strName.c_str(), pkCashItem->iCashNum );
		g_pInterfaceManager->MessageBox(szMsg, MES_OKCANCEL, this->GetInstanceID(), SPIM_CASHSHOP_MSG_ADDEVENT);
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnAddPET(WPARAM wParam, LPARAM lParam)
{
	SPCashEffectItem* pkCashItem = g_pItemCluster->GetEffectByID(EFT_ADDPET);
	if ( pkCashItem )
	{
		char  szMsg[256];
		sprintf( szMsg, "%s(价格:%d点券)", pkCashItem->strName.c_str(), pkCashItem->iCashNum );
		g_pInterfaceManager->MessageBox(szMsg, MES_OKCANCEL, this->GetInstanceID(), SPIM_CASHSHOP_MSG_ADDPET);
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnAddWH(WPARAM wParam, LPARAM lParam)
{
	SPCashEffectItem* pkCashItem = g_pItemCluster->GetEffectByID(EFT_ADDWH);
	if ( pkCashItem )
	{
		char  szMsg[256];
		sprintf( szMsg, "%s(价格:%d点券)", pkCashItem->strName.c_str(), pkCashItem->iCashNum );
		g_pInterfaceManager->MessageBox(szMsg, MES_OKCANCEL, this->GetInstanceID(), SPIM_CASHSHOP_MSG_ADDWH);
	}
	return 1;
}

//------------------------------------------------------------------------------------
//Message
int SPWindowCashShop::OnAddParcel2(WPARAM wParam, LPARAM lParam)
{
	if ( (MesBox_Type)wParam == MES_OK )
	{	
		SPCashEffectItem* pkCashItem = g_pItemCluster->GetEffectByID(EFT_ADDPARCEL);
		if ( pkCashItem )
		{
			if ( pkCashItem->iCashNum > m_uiCurOZ )
			{
				OnMyInfoFilling(0, 0);
			}
			else
			{
				CPacket kPacket(CASHSHOP_CS_EFFECT);
				kPacket.AddUINT32(EFT_ADDPARCEL);
				kPacket.AddUINT32(EFT_ADDPARCEL);
				kPacket.AddUINT8(0);
				(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
			}
		}
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnAddConsume2(WPARAM wParam, LPARAM lParam)
{
	if ( (MesBox_Type)wParam == MES_OK )
	{
		SPCashEffectItem* pkCashItem = g_pItemCluster->GetEffectByID(EFT_ADDPARCEL);
		if ( pkCashItem )
		{
			if ( pkCashItem->iCashNum > m_uiCurOZ )
			{
				OnMyInfoFilling(0, 0);
			}
			else
			{
				CPacket kPacket(CASHSHOP_CS_EFFECT);
				kPacket.AddUINT32(EFT_ADDCONSUME);
				kPacket.AddUINT32(EFT_ADDCONSUME);
				kPacket.AddUINT8(0);
				(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
			}
		}
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnAddETC2(WPARAM wParam, LPARAM lParam)
{
	if ( (MesBox_Type)wParam == MES_OK )
	{
		SPCashEffectItem* pkCashItem = g_pItemCluster->GetEffectByID(EFT_ADDPARCEL);
		if ( pkCashItem )
		{
			if ( pkCashItem->iCashNum > m_uiCurOZ )
			{
				OnMyInfoFilling(0, 0);
			}
			else
			{
				CPacket kPacket(CASHSHOP_CS_EFFECT);
				kPacket.AddUINT32(EFT_ADDETC);
				kPacket.AddUINT32(EFT_ADDETC);
				kPacket.AddUINT8(0);
				(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
			}
		}
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnAddEvent2(WPARAM wParam, LPARAM lParam)
{
	if ( (MesBox_Type)wParam == MES_OK )
	{
		SPCashEffectItem* pkCashItem = g_pItemCluster->GetEffectByID(EFT_ADDPARCEL);
		if ( pkCashItem )
		{
			if ( pkCashItem->iCashNum > m_uiCurOZ )
			{
				OnMyInfoFilling(0, 0);
			}
			else
			{
				CPacket kPacket(CASHSHOP_CS_EFFECT);
				kPacket.AddUINT32(EFT_ADDEVENT);
				kPacket.AddUINT32(EFT_ADDEVENT);
				kPacket.AddUINT8(0);
				(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
			}
		}
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnAddPET2(WPARAM wParam, LPARAM lParam)
{
	if ( (MesBox_Type)wParam == MES_OK )
	{
		SPCashEffectItem* pkCashItem = g_pItemCluster->GetEffectByID(EFT_ADDPARCEL);
		if ( pkCashItem )
		{
			if ( pkCashItem->iCashNum > m_uiCurOZ )
			{
				OnMyInfoFilling(0, 0);
			}
			else
			{
				CPacket kPacket(CASHSHOP_CS_EFFECT);
				kPacket.AddUINT32(EFT_ADDPET);
				kPacket.AddUINT32(EFT_ADDPET);
				kPacket.AddUINT8(0);
				(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
			}
		}
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnAddWH2(WPARAM wParam, LPARAM lParam)
{
	if ( (MesBox_Type)wParam == MES_OK )
	{
		SPCashEffectItem* pkCashItem = g_pItemCluster->GetEffectByID(EFT_ADDPARCEL);
		if ( pkCashItem )
		{
			if ( pkCashItem->iCashNum > m_uiCurOZ )
			{
				OnMyInfoFilling(0, 0);
			}
			else
			{
				CPacket kPacket(CASHSHOP_CS_EFFECT);
				kPacket.AddUINT32(EFT_ADDWH);
				kPacket.AddUINT32(EFT_ADDWH);
				kPacket.AddUINT8(0);
				(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
			}
		}
	}
	return 1;
}
