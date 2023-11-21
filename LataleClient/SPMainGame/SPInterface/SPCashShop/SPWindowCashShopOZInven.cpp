#include "SPCommon.h"
#include "SPUtil.h"
#include "SPWindowDEF.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowGrid.h"
#include "SPWindowSlider.h"
#include "SPWindowCashShop.h"

#include "SPGOBManager.h"
#include "SPGameObject.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"

#include "SPItem.h"
#include "SPItemStatus.h"

#include "SPCheckManager.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

//------------------------------------------------------------------------------------
void SPWindowCashShop::ClearOZInven()
{
	SPWindow* pkParentWindow;
	SPWindowGrid* pkWindowGrid;

	m_iOZInvenCurPage[0] = 0;
	m_iOZInvenCurPage[1] = 0;
	//SetOZInvenPageText();

	pkParentWindow = Find(WIID_CASHSHOP_OZ_INVEN);
	if( pkParentWindow == NULL )
		return;

	pkWindowGrid = static_cast< SPWindowGrid* >(pkParentWindow->Find(WIID_CASHSHOP_OZ_INVEN_SLOT, true));
	if( pkWindowGrid == NULL )
		return;

	pkWindowGrid->ClearCellAll();
	//ClearOZSellInfo();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::UpdateOZInven()
{
	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return;

	SPWindow* pkParentWindow;
	SPWindowGrid* pkWindowGrid;

	if( g_pItemCluster == NULL )
		return;

	pkParentWindow = Find(WIID_CASHSHOP_OZ_INVEN);
	if( pkParentWindow == NULL )
		return;

	pkWindowGrid = static_cast< SPWindowGrid* >(pkParentWindow->Find(WIID_CASHSHOP_OZ_INVEN_SLOT, true));
	if( pkWindowGrid == NULL )
		return;

	pkWindowGrid->ClearCellAll();

	int iSize, iStartIndex, i;
	SPItem* pkItem;

	iStartIndex = m_iOZInvenCurPage[m_iOZInvenCategory] * 5;
	if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_BUYINFO )
	{
		iSize = pkInvenArchive->GetInventorySize(INVENTORY_CASH);
	}
	else if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_WEARINFO )
	{
		iSize = pkInvenArchive->GetInventorySize(INVENTORY_WEAR);
	}

	for( i = 0; i < 10; i++ )
	{
		if( (iStartIndex + i) >= iSize )
			break;

		if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_BUYINFO )
		{
			pkItem = pkInvenArchive->GetInventoryItem(INVENTORY_CASH, iStartIndex + i);
		}
		else if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_WEARINFO )
		{
			pkItem = pkInvenArchive->GetInventoryItem(INVENTORY_WEAR, iStartIndex + i);
		}

		if( pkItem == NULL )
			continue;

		CONTAINER_ITEM Item;
		pkItem->ConvertContainerItem(&Item);

		bool bEnable = g_pCheckManager->CheckItemStatus(Item.Item.iItemID);	
		Item.SlotInfo.SlotIndex = Item.SlotInfo.SlotIndex - iStartIndex;
		if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_WEARINFO )
		{
			Item.SlotInfo.SlotIndex = i;
		}

		POINT ptSrcPos;	
		SPTexture* pkTexture = g_pItemCluster->GetItemTexture(Item.Item.iItemID, ptSrcPos); 	

		pkWindowGrid->SettingIconImage(pkTexture, Item.Item.iItemID,
			ptSrcPos.x, ptSrcPos.y, Item.SlotInfo.SlotIndex, Item.Item.iStackCount, bEnable);
	}

	//
	SetSliderMaxPage(2, m_iOZInvenMaxPage[m_iOZInvenCategory] + 1);
	SetSliderPage(2, m_iOZInvenCurPage[m_iOZInvenCategory]);

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetOZInvenPageText()
{
	//SPWindow* pkParentWindow, *pkChildWindow;

	//pkParentWindow = Find(WIID_CASHSHOP_OZ_INVEN);
	//if( pkParentWindow == NULL )
	//	return;

	//pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_OZ_INVEN_PAGE_TEXT, true);
	//if( pkChildWindow == NULL )
	//	return;

	//char szBuf[32];
	////sprintf(szBuf, "%d / %d", m_iOZInvenCurPage + 1, m_iOZInvenMaxPage);
	//sprintf(szBuf, "%d", m_iOZInvenCurPage[m_iOZInvenCategory] + 1);
	//pkChildWindow->SetWindowText(szBuf);
}

//------------------------------------------------------------------------------------
SPItem* SPWindowCashShop::GetOZInvenItem(int iSlotIndex)
{
	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return NULL;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return NULL;

	int iSize, iStartIndex;
	iStartIndex = m_iOZInvenCurPage[m_iOZInvenCategory] * 5;

	if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_BUYINFO )
	{
		iSize = pkInvenArchive->GetInventorySize(INVENTORY_CASH);
	}
	else if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_WEARINFO )
	{
		iSize = pkInvenArchive->GetInventorySize(INVENTORY_WEAR);
	}

	if( (iStartIndex + iSlotIndex) >= iSize )
		return NULL;

	SPItem * pkItem = NULL;
	if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_BUYINFO )
	{
		pkItem = pkInvenArchive->GetInventoryItem(INVENTORY_CASH, iStartIndex + iSlotIndex);
	}
	else if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_WEARINFO )
	{
		pkItem = pkInvenArchive->GetInventoryItem(INVENTORY_WEAR, iStartIndex + iSlotIndex);
	}

	return pkItem;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetOZSellInfo(SPItem* pItem)
{
	if( pItem == NULL || g_pItemCluster == NULL )
		return;

	SPItemStatus* pkItemStatus = pItem->GetItemStatus();
	SPItemAttr* pkItemAttr = pItem->GetItemAttr();
	if( pkItemStatus == NULL || pkItemAttr == NULL )
		return;

	if( !pkItemAttr->m_bCashCheck )
		return;

	m_pSellItem = pItem;
	ShowSellDlg();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ShowSellDlg()
{	
	SPWindow* pkParentWindow, *pkChildWindow;

	SPItemStatus* pkItemStatus = m_pSellItem->GetItemStatus();
	SPItemAttr* pkItemAttr = m_pSellItem->GetItemAttr();
	if( pkItemStatus == NULL || pkItemAttr == NULL )
		return;

	pkParentWindow = Find(WIID_CASHSHOP_SELL, true);
	if( pkParentWindow == NULL )
		return;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_SELL_ITEM, true);
	if( pkChildWindow )
	{
		pkChildWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);

		POINT ptSrcPos;
		SPTexture* pkTexture = g_pItemCluster->GetItemTexture(pkItemStatus->GetItemID(), ptSrcPos);
		if( pkTexture )
			pkChildWindow->SetImage(pkTexture, ptSrcPos.x, ptSrcPos.y);
	}

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_SELL_ITEM_NAME, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(pkItemAttr->m_strName.c_str());

	int	iShowPrice	=	pkItemAttr->m_iCashPoint;
	int	iShowCount	=	pkItemStatus->GetStackCount();

	if( iShowPrice <= 0 )
		iShowPrice = pkItemStatus->SPSendMessage( MV_ITEM_GET_BACKPRICE );

	iShowPrice *= iShowCount;

	const char*		pstrShowPrice	=	ConvertMoneyToString( iShowPrice );

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_SELL_MILEAGE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText( pstrShowPrice );

	char szTemp[256];
	wsprintf(szTemp, g_pResourceManager->GetGlobalString(9001004), pstrShowPrice );
	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_SELL_INFO_TEXT, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(szTemp);

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_SELL_COUNT, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText( ConvertMoneyToString( iShowCount ) );
	
	pkParentWindow->Show();
	g_pInterfaceManager->SetModalWindow(pkParentWindow);
}



//------------------------------------------------------------------------------------
// Message
int SPWindowCashShop::OnOZInvenLeftPage(WPARAM wParam, LPARAM lParam)
{
	m_iOZInvenCurPage[m_iOZInvenCategory]--;
	if( m_iOZInvenCurPage[m_iOZInvenCategory] < 0 )
	{
		m_iOZInvenCurPage[m_iOZInvenCategory] = 0;
		return 1;
	}

	SetSelectSlot(-1, false);
	SetOZInvenPageText();
	UpdateOZInven();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZInvenRightPage(WPARAM wParam, LPARAM lParam)
{
	m_iOZInvenCurPage[m_iOZInvenCategory]++;
	if( m_iOZInvenCurPage[m_iOZInvenCategory] >= m_iOZInvenMaxPage[m_iOZInvenCategory] )
	{
		m_iOZInvenCurPage[m_iOZInvenCategory] = m_iOZInvenMaxPage[m_iOZInvenCategory] - 1;
		return 1;
	}

	SetSelectSlot(-1, false);
	SetOZInvenPageText();
	UpdateOZInven();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZInvenCursorMove(WPARAM wParam, LPARAM lParam)
{
	// ‘¥©º«¬º≤ª÷ß≥÷Õœ∂Ø
	if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_WEARINFO )
		return 1;

	if( g_pItemCluster == NULL )
		return 1;

	if( m_pkSelectItem == NULL && m_iSelectSlot >= 0 && m_iSelectInven == 1 )
	{
		m_pkSelectItem = GetOZInvenItem(m_iSelectSlot);
		if( m_pkSelectItem )
		{
			POINT ptSrcPos;
			SPTexture* pkTexture = g_pItemCluster->GetItemTexture( m_pkSelectItem->GetItemStatusValue(MV_ITEM_GETUID), ptSrcPos);
			g_pInterfaceManager->SetMousePickup(pkTexture, ptSrcPos.x, ptSrcPos.y);

			SPWindowGrid* pkGridWindow =
				static_cast< SPWindowGrid* >(Find(WIID_CASHSHOP_OZ_INVEN_SLOT, true));
			if( pkGridWindow )
				pkGridWindow->CellSelectSell(m_iSelectSlot, true);
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZInvenLButtonDown(WPARAM wParam, LPARAM lParam)
{
	m_iSelectSlot = (int)lParam;
	m_iSelectInven = 1;
	
	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return 1;

	SPItem* pkItem;
	if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_BUYINFO )
	{
		pkItem = pkInvenArchive->GetInventoryItem(INVENTORY_CASH, m_iSelectSlot + m_iOZInvenCurPage[m_iOZInvenCategory] * 5);
	}
	else if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_WEARINFO )
	{
		pkItem = pkInvenArchive->GetInventoryItem(INVENTORY_WEAR, m_iSelectSlot + m_iOZInvenCurPage[m_iOZInvenCategory] * 5);
	}

	if( pkItem == NULL || pkItem->GetContainerType() == CONTAINER_TYPE_INVALID || pkItem->IsEmptyItem() == true )
		SetSelectSlot(-1, false);
	else
		SetSelectSlot(m_iSelectSlot, false);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZInvenLButtonUp(WPARAM wParam, LPARAM lParam)
{
	// ‘¥©º«¬º≤ª÷ß≥÷∑≈»Î
	if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_WEARINFO )
		return 1;

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

		if( m_pkSelectItem->GetItemAttr() == NULL )
			return 1;

		//ƒ≥Ω√ æ∆¿Ã≈€¿Œ¡ˆ »Æ¿Œ
		if(m_pkSelectItem->GetItemAttr()->m_bCashCheck == false) {			
			return 1;
		}

		//√¢∞Ìø° ¿˙¿Â∞°¥…«— æ∆¿Ã≈€¿Œ¡ˆ »Æ¿Œ
		if(m_pkSelectItem->GetItemAttr()->m_bStorage == false) {			
			return 1;
		}

		int iStartIndex = m_iOZInvenCurPage[m_iOZInvenCategory] * 5;
		if( (m_pkSelectItem->GetSlotIndex() != iStartIndex + iDstSlotIndex) || 
			(m_pkSelectItem->GetContainerType() != INVENTORY_CASH) )
		{
			ChangeSlot.SrcSlotInfo.ContainerType = m_pkSelectItem->GetContainerType();
			ChangeSlot.SrcSlotInfo.SlotIndex = m_pkSelectItem->GetSlotIndex() + 1;
			ChangeSlot.iCount = m_pkSelectItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
			
			ChangeSlot.DestSlotInfo.ContainerType = INVENTORY_CASH;
			ChangeSlot.DestSlotInfo.SlotIndex = iStartIndex + iDstSlotIndex + 1;

			CPacket kPacket;
			kPacket.Add((UINT32)ITEM_CS_CHANGE_SLOT);
			kPacket.AddData(&ChangeSlot, sizeof(CHANGE_SLOT));	
			(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZSellCursorEnter(WPARAM wParam, LPARAM lParam)
{
	if( m_pkSelectItem == NULL )
		return 1;

	SPItemStatus* pkItemStatus = m_pkSelectItem->GetItemStatus();
	SPItemAttr* pkItemAttr = m_pkSelectItem->GetItemAttr();
	if( pkItemStatus == NULL || pkItemAttr == NULL )
		return 1;

	if( !pkItemAttr->m_bCashCheck )
		return 1;

	SPWindow* pkWindow = Find(WIID_CASHSHOP_OZ_SELL, true);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 1);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZSellCursorOut(WPARAM wParam, LPARAM lParam)
{
	if( m_pkSelectItem == NULL )
		return 1;

	SPWindow* pkWindow = Find(WIID_CASHSHOP_OZ_SELL, true);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZSellLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if( m_pkSelectItem == NULL )
		return 1;

	SPWindow* pkWindow = Find(WIID_CASHSHOP_OZ_SELL, true);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);

	SetOZSellInfo(m_pkSelectItem);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZSellHelpLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if( m_fSellHelpAccumulate < 5.0f )
		m_fSellHelpAccumulate = 5.0f;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZInvenSell(WPARAM wParam, LPARAM lParam)
{
	if( m_pSellItem == NULL )
		return 1;

	if( g_pCheckManager->IsDBBlock() )
		return 1;

	CONTAINER_ITEM Item;
	m_pSellItem->ConvertContainerItem(&Item);
	Item.SlotInfo.SlotIndex += 1;

	CPacket kPacket(CASHSHOP_CS_SELL);
	kPacket.AddData(&Item, sizeof(CONTAINER_ITEM));
	(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
	g_pCheckManager->SetDBBlock(true);

	//ClearOZSellInfo();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSellOK(WPARAM wParam, LPARAM lParam)
{
	if( g_pCheckManager->IsDBBlock() || m_pSellItem == NULL )
		return 1;

	CONTAINER_ITEM Item;
	m_pSellItem->ConvertContainerItem(&Item);
	Item.SlotInfo.SlotIndex += 1;

	CPacket kPacket(CASHSHOP_CS_SELL);
	kPacket.AddData(&Item, sizeof(CONTAINER_ITEM));
	(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
	g_pCheckManager->SetDBBlock(true);

	SPWindow* pkWindow = Find(WIID_CASHSHOP_SELL, true);
	if( pkWindow )
		pkWindow->Hide();

	g_pInterfaceManager->SetModalWindow(NULL);
	m_pSellItem = NULL;
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSellCancel(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_SELL);
	if( pkWindow )
		pkWindow->Hide();

	g_pInterfaceManager->SetModalWindow(NULL);
	m_pSellItem = NULL;
	return 1;
}


//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZInvenToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	if( m_pkSelectItem )
	{
		SPItemAttr* pItemAttr = m_pkSelectItem->GetItemAttr();
		if( pItemAttr )
		{
			// ‘¥©º«¬º≤ª÷ß≥÷∑≈»Î
			if ( m_iOZInvenCategory + WIID_CASHSHOP_OZ_INVEN_BUYINFO == WIID_CASHSHOP_OZ_INVEN_WEARINFO )
			{
				SetOZInvenCategory(0);
			}
		}
	}

	int iSlotIndex = (int)lParam;
	SPItem* pkItem = GetOZInvenItem(iSlotIndex);
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
int SPWindowCashShop::OnOZInvenToolTipOut(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, lParam);
	pkWindow->Show();

	return 1;
}
//------------------------------------------------------------------------------------
int SPWindowCashShop::OnOZInvenToolTipMove(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)lParam;
	SPItem* pkItem = GetOZInvenItem(iSlotIndex);
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
int SPWindowCashShop::OnSellToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	if( m_pSellItem == NULL )
		return 1;

	SPItemStatus* pkItemStatus = m_pSellItem->GetItemStatus();
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
int SPWindowCashShop::OnSellToolTipOut(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, lParam);
	pkWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSellToolTipMove(WPARAM wParam, LPARAM lParam)
{
	if( m_pSellItem == NULL )
		return 1;

	SPItemStatus* pkItemStatus = m_pSellItem->GetItemStatus();
	if( pkItemStatus == NULL )
		return 1;

	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)pkItemStatus, lParam);
	pkWindow->Show();
	return 1;
}
