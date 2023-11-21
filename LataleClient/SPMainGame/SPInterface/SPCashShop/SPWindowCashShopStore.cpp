#include "SPCommon.h"
#include "SPUtil.h"
#include "SPWindowDEF.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowGrid.h"
#include "SPWindowSlider.h"
#include "SPWindowEdit.h"
#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"
#include "SPTreeBoxItemTexture.h"
#include "SPWindowTreeBox.h"
#include "SPWindowPageSelect.h"
#include "SPWindowCashShop.h"

#include "SPSubGameManager.h"

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
#include "SPIMEPool.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include "SPCashTradeManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPHotItemInfo.h"
#include "SPHotItemManager.h"
#include "SPCashTradeManager.h"

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetStoreTabType(CASHSHOP_STORE_TAB eType)
{
	if( m_eStoreTabType == eType )
		return;
	
	m_eStoreTabType = eType;
	SetStoreTabWindow(eType);

	m_iStoreCategory[0] = SetStoreCashCategory(eType, 0);
	
	int iDefaultIndex = 0;
	if( eType != CST_SEARCH && eType != CST_HELP )
		iDefaultIndex = GetDefaultPage(m_iStoreCategory[0]);
	m_iStoreCategory[1] = SetStoreCashCategory(iDefaultIndex, 1);

	m_iStoreCategory[2] = SetStoreCashCategory(0, 2);

	SetStoreCategory(GetStoreWindowID());
	
	m_bHelpTextDraw = false;
	if( eType == CST_HELP )
		m_bHelpTextDraw = true;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetStoreTabWindow(CASHSHOP_STORE_TAB eType)
{
	SPWindow* pkWindow;
	vector< unsigned int > vShowWindow, vHideWindow;
	int i;

	for( i = WIID_CASHSHOP_STORE_EVENT_NEW; i <= WIID_CASHSHOP_STORE_EVENT_EVENT; i++ )
		vHideWindow.push_back(i);
	for( i = WIID_CASHSHOP_STORE_FASSION_DRESS; i <= WIID_CASHSHOP_STORE_FASSION_ETC; i++ )
		vHideWindow.push_back(i);
	for( i = WIID_CASHSHOP_STORE_PET_PET; i <= WIID_CASHSHOP_STORE_PET_ETC; i++ )
		vHideWindow.push_back(i);
	for( i = WIID_CASHSHOP_STORE_ETC_FUNCTIONAL; i <= WIID_CASHSHOP_STORE_ETC_COMMUNITY; i++ )
		vHideWindow.push_back(i);
	for( i = WIID_CASHSHOP_STORE_PACK_FASSION; i <= WIID_CASHSHOP_STORE_PACK_ETC; i++ )
		vHideWindow.push_back(i);
	for( i = WIID_CASHSHOP_STORE_SEARCH_STATIC; i <= WIID_CASHSHOP_STORE_SEARCH_SEARCH; i++ )
		vHideWindow.push_back(i);
#ifdef __CASHSHOP_POINT_SHOP__
	for( i = WIID_CASHSHOP_STORE_POINT_FASSION; i <= WIID_CASHSHOP_STORE_POINT_FUNCTIONAL; i++ )
		vHideWindow.push_back(i);
#endif
	for( i = WIID_CASHSHOP_STORE_ITEM; i <= WIID_CASHSHOP_STORE_ITEM + 10; i++ )
		vHideWindow.push_back(i);

	vHideWindow.push_back(WIID_CASHSHOP_STORE_SORT_GENDER);
	vHideWindow.push_back(WIID_CASHSHOP_STORE_SORT_PRICE);

	switch( eType )
	{
	case CST_EVENT:
		{
			for( i = WIID_CASHSHOP_STORE_EVENT_NEW; i <= WIID_CASHSHOP_STORE_EVENT_EVENT; i++ )
				vShowWindow.push_back(i);

			for( i = WIID_CASHSHOP_STORE_ITEM; i <= WIID_CASHSHOP_STORE_ITEM + 10; i++ )
				vShowWindow.push_back(i);

			vShowWindow.push_back(WIID_CASHSHOP_STORE_SORT_GENDER);
			vShowWindow.push_back(WIID_CASHSHOP_STORE_SORT_PRICE);
		}
		break;
	case CST_FASSION:
		{
			for( i = WIID_CASHSHOP_STORE_FASSION_DRESS; i <= WIID_CASHSHOP_STORE_FASSION_ETC; i++ )
				vShowWindow.push_back(i);

			for( i = WIID_CASHSHOP_STORE_ITEM; i <= WIID_CASHSHOP_STORE_ITEM + 10; i++ )
				vShowWindow.push_back(i);

			vShowWindow.push_back(WIID_CASHSHOP_STORE_SORT_GENDER);
			vShowWindow.push_back(WIID_CASHSHOP_STORE_SORT_PRICE);
		}
		break;
	case CST_PET:
		{
			for( i = WIID_CASHSHOP_STORE_PET_PET; i <= WIID_CASHSHOP_STORE_PET_ETC; i++ )
				vShowWindow.push_back(i);

			for( i = WIID_CASHSHOP_STORE_ITEM; i <= WIID_CASHSHOP_STORE_ITEM + 10; i++ )
				vShowWindow.push_back(i);

			vShowWindow.push_back(WIID_CASHSHOP_STORE_SORT_GENDER);
			vShowWindow.push_back(WIID_CASHSHOP_STORE_SORT_PRICE);
		}
		break;
	case CST_ETC:
		{
			for( i = WIID_CASHSHOP_STORE_ETC_FUNCTIONAL; i <= WIID_CASHSHOP_STORE_ETC_COMMUNITY; i++ )
				vShowWindow.push_back(i);

			for( i = WIID_CASHSHOP_STORE_ITEM; i <= WIID_CASHSHOP_STORE_ITEM + 10; i++ )
				vShowWindow.push_back(i);

			vShowWindow.push_back(WIID_CASHSHOP_STORE_SORT_GENDER);
			vShowWindow.push_back(WIID_CASHSHOP_STORE_SORT_PRICE);
		}
		break;
	case CST_PACKAGE:
		{
			for( i = WIID_CASHSHOP_STORE_PACK_FASSION; i <= WIID_CASHSHOP_STORE_PACK_ETC; i++ )
				vShowWindow.push_back(i);

			for( i = WIID_CASHSHOP_STORE_ITEM; i <= WIID_CASHSHOP_STORE_ITEM + 10; i++ )
				vShowWindow.push_back(i);

			vShowWindow.push_back(WIID_CASHSHOP_STORE_SORT_GENDER);
			vShowWindow.push_back(WIID_CASHSHOP_STORE_SORT_PRICE);
		}
		break;
	case CST_SEARCH:
		{
			for( i = WIID_CASHSHOP_STORE_SEARCH_STATIC; i <= WIID_CASHSHOP_STORE_SEARCH_SEARCH; i++ )
				vShowWindow.push_back(i);

			for( i = WIID_CASHSHOP_STORE_ITEM; i <= WIID_CASHSHOP_STORE_ITEM + 10; i++ )
				vShowWindow.push_back(i);

			vShowWindow.push_back(WIID_CASHSHOP_STORE_SORT_GENDER);
			vShowWindow.push_back(WIID_CASHSHOP_STORE_SORT_PRICE);
		}
		break;
	case CST_HELP:
		break;
#ifdef __CASHSHOP_POINT_SHOP__
	case CST_POINT:
		{
			for( i = WIID_CASHSHOP_STORE_POINT_FASSION; i <= WIID_CASHSHOP_STORE_POINT_FUNCTIONAL; i++ )
				vShowWindow.push_back(i);

			vHideWindow.push_back(WIID_CASHSHOP_STORE_SORT_GENDER);
			vHideWindow.push_back(WIID_CASHSHOP_STORE_SORT_PRICE);
		}
		break;
#endif
	}

	for( i = 0; i < CST_MAX; i++ )
	{
		pkWindow = Find(WIID_CASHSHOP_STORE_EVENT + i, true);
		if( pkWindow == NULL )
			continue;

		if( eType == i + 1 )
			pkWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE);
		else
			pkWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);
	}
	
	vector< unsigned int >::iterator iter = vHideWindow.begin();
	while( iter != vHideWindow.end() )
	{
		pkWindow = Find(*iter, true);
		if( pkWindow )
			pkWindow->Hide();

		++iter;
	}

	iter = vShowWindow.begin();
	while( iter != vShowWindow.end() )
	{
		pkWindow = Find(*iter, true);
		if( pkWindow )
			pkWindow->Show();

		++iter;
	}

	vShowWindow.clear();
	vHideWindow.clear();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetStoreCategory(int iCategory)
{
	SPWindow* pkParentWindow, *pkChildWindow;

	pkParentWindow = Find(WIID_CASHSHOP_STORE);
	if( pkParentWindow == NULL )
		return;

	int iMinID = 0, iMaxID = 0;
	int i;
	if( iCategory >= WIID_CASHSHOP_STORE_EVENT_NEW &&
		iCategory <= WIID_CASHSHOP_STORE_EVENT_EVENT )
	{
		iMinID = WIID_CASHSHOP_STORE_EVENT_NEW;
		iMaxID = WIID_CASHSHOP_STORE_EVENT_EVENT;
	}
	else if( iCategory >= WIID_CASHSHOP_STORE_FASSION_DRESS &&
		iCategory <= WIID_CASHSHOP_STORE_FASSION_ETC )
	{
		iMinID = WIID_CASHSHOP_STORE_FASSION_DRESS;
		iMaxID = WIID_CASHSHOP_STORE_FASSION_ETC;
	}
	else if( iCategory >= WIID_CASHSHOP_STORE_PET_PET &&
		iCategory <= WIID_CASHSHOP_STORE_PET_ETC )
	{
		iMinID = WIID_CASHSHOP_STORE_PET_PET;
		iMaxID = WIID_CASHSHOP_STORE_PET_ETC;
	}
	else if( iCategory >= WIID_CASHSHOP_STORE_ETC_FUNCTIONAL &&
		iCategory <= WIID_CASHSHOP_STORE_ETC_COMMUNITY )
	{
		iMinID = WIID_CASHSHOP_STORE_ETC_FUNCTIONAL;
		iMaxID = WIID_CASHSHOP_STORE_ETC_COMMUNITY;
	}
	else if( iCategory >= WIID_CASHSHOP_STORE_PACK_FASSION &&
		iCategory <= WIID_CASHSHOP_STORE_PACK_ETC )
	{
		iMinID = WIID_CASHSHOP_STORE_PACK_FASSION;
		iMaxID = WIID_CASHSHOP_STORE_PACK_ETC;
	}
#ifdef __CASHSHOP_POINT_SHOP__
	else if( iCategory >= WIID_CASHSHOP_STORE_POINT_FASSION &&
		iCategory <= WIID_CASHSHOP_STORE_POINT_FUNCTIONAL )
	{
		iMinID = WIID_CASHSHOP_STORE_POINT_FASSION;
		iMaxID = WIID_CASHSHOP_STORE_POINT_FUNCTIONAL;
	}
#endif

	BOOL bCheck = FALSE;
	for( i = iMinID; i <= iMaxID; i++ )
	{
		pkChildWindow = pkParentWindow->Find(i, true);
		if( pkChildWindow == NULL )
			continue;

		bCheck = FALSE;
		if( iCategory == i )
			bCheck = TRUE;

		pkChildWindow->SPSendMessage(SPIM_BTN_SELECT, bCheck);
	}
	
	CalcPageNum();
	SetStorePageText();
	RefreshStore();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::CalcPageNum()
{
	if( m_eStoreTabType == CST_HELP )
		m_iStoreMaxPage = (int)m_mHelpInfoList.size();
	else
	{
		int iSize = g_pItemCluster->GetSortCashItemSize(ConvertCashCategory());
		m_iStoreMaxPage = 1;
		if( iSize )
			m_iStoreMaxPage = (iSize - 1) / 10 + 1;
	}

	m_iStoreCurPage = 0;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::SetStoreCashCategory(int iCategory, int iIndex)
{
	int iRet = 0;
	if( iIndex == 0 )
	{
		switch( (CASHSHOP_STORE_TAB)iCategory )
		{
		case CST_EVENT:
			iRet = CI_EVENT;
			break;
		case CST_FASSION:
			iRet = CI_FASSION;
			break;
		case CST_PET:
			iRet = CI_PET;
			break;
		case CST_ETC:
			iRet = CI_ETC;
			break;
		case CST_PACKAGE:
			iRet = CI_PACKAGE;
			break;
		case CST_SEARCH:
			iRet = CI_SEARCH;
			break;
		case CST_HELP:
			iRet = CI_NULL;
			break;
#ifdef __CASHSHOP_POINT_SHOP__
		case CST_POINT:
			iRet = CI_POINT;
			break;
#endif
		}
	}
	else if( iIndex == 1 )
	{
		switch( m_iStoreCategory[0] )
		{
		case CI_EVENT:
			iRet = iCategory + 1;
			break;
		case CI_FASSION:
			iRet = iCategory + CIS1_DRESS;
			break;
		case CI_PET:
			iRet = iCategory + CIS1_PET;
			break;
		case CI_ETC:
			iRet = iCategory + CIS1_FUNCTIONAL;
			break;
		case CI_PACKAGE:
			iRet = iCategory + CIS1_PACKAGE_FASSION;
			break;
#ifdef __CASHSHOP_POINT_SHOP__
		case CI_POINT:
			iRet = iCategory + CIS1_POINT_FASSION;
			break;
#endif
		}
	}
	else if( iIndex == 2 )
	{
		if( iCategory == 0 )
			return 0;

		iCategory -= 1;
		switch( m_iStoreCategory[1] )
		{
		case CIS1_DRESS:
			iRet = CIS2_ONEPEACE + iCategory;
			break;
		case CIS1_HELMET:
			iRet = CIS2_HELMET + iCategory;
			break;
		case CIS1_SHOES:
			iRet = CIS2_SHOES + iCategory;
			break;
		case CIS1_GLOVE:
			iRet = CIS2_GLOVE + iCategory;
			break;
		case CIS1_ACCESSARIES:
			iRet = CIS2_GLASS + iCategory;
			break;
		case CIS1_COMMUNITY:
			iRet = CIS2_EMOTICON + iCategory;
			break;
		}
	}

	return iRet;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::GetStoreWindowID()
{
	int iRet = 0;
	switch( m_iStoreCategory[0] )
	{
	case CI_EVENT:
		if( m_iStoreCategory[1] >= 1 && m_iStoreCategory[1] <= 3 )
			iRet = WIID_CASHSHOP_STORE_EVENT_NEW + (m_iStoreCategory[1] - 1);
		break;
	case CI_FASSION:
		if( m_iStoreCategory[1] >= CIS1_DRESS && m_iStoreCategory[1] <= CIS1_ETC )
			iRet = WIID_CASHSHOP_STORE_FASSION_DRESS + (m_iStoreCategory[1] - CIS1_DRESS);
		break;
	case CI_PET:
		if( m_iStoreCategory[1] >= CIS1_PET && m_iStoreCategory[1] <= CIS1_PET_ETC )
			iRet = WIID_CASHSHOP_STORE_PET_PET + (m_iStoreCategory[1] - CIS1_PET);
		break;
	case CI_ETC:
		if( m_iStoreCategory[1] >= CIS1_FUNCTIONAL && m_iStoreCategory[1] <= CIS1_COMMUNITY )
			iRet = WIID_CASHSHOP_STORE_ETC_FUNCTIONAL + (m_iStoreCategory[1] - CIS1_FUNCTIONAL);
		break;
	case CI_PACKAGE:
		if( m_iStoreCategory[1] >= CIS1_PACKAGE_FASSION && m_iStoreCategory[1] <= CIS1_PACKAGE_ETC )
			iRet = WIID_CASHSHOP_STORE_PACK_FASSION + (m_iStoreCategory[1] - CIS1_PACKAGE_FASSION);
		break;
#ifdef __CASHSHOP_POINT_SHOP__
	case CI_POINT:
		if( m_iStoreCategory[1] >= CIS1_POINT_FASSION && m_iStoreCategory[1] <= CIS1_POINT_FUNCTIONAL )
			iRet = WIID_CASHSHOP_STORE_POINT_FASSION + (m_iStoreCategory[1] - CIS1_POINT_FASSION);
		break;
#endif
	}

	return iRet;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::ConvertCashCategory()
{
	int iRet;

	iRet = ((m_iStoreCategory[0] & 0xFF) << 16);
	iRet |= ((m_iStoreCategory[1] & 0xFF) << 8);
	iRet |= (m_iStoreCategory[2] & 0xFF);
	
	return iRet;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::RefreshStore()
{
	SPCashItemAttr* pkCashItemAttr;
	SPWindow* pkParentWindow, *pkAttachWindow, *pkChildWindow[2];
	SPWindowButton* pkButton;
	SPTexture* pkTexture;
	POINT pt;
	RECT rcDst;
	int i;

	m_vEventAnimation.clear();
	m_vSaleArrow.clear();
	m_vSaleOrderArrow.clear();

	if( m_eStoreTabType == CST_HELP )
		return;

	pkParentWindow = Find(WIID_CASHSHOP_STORE);
	if( pkParentWindow == NULL )
		return;

	for( i = 0; i < 10; i++ )
	{
		pkAttachWindow = pkParentWindow->Find(WIID_CASHSHOP_STORE_ITEM + i, true);
		if( pkAttachWindow )
			pkAttachWindow->CleanChildAll();
	}

	for( i = 0; i < 10; i++ )
	{
		pkCashItemAttr = NULL;
		pkCashItemAttr = g_pItemCluster->GetSortCashItem(ConvertCashCategory(), i + m_iStoreCurPage * 10);
		
		if( pkCashItemAttr == NULL )
			break;
		
		pkAttachWindow = pkParentWindow->Find(WIID_CASHSHOP_STORE_ITEM + i, true);
		if( pkAttachWindow == NULL )
			break;

		pkAttachWindow->Show();
		if( pkCashItemAttr->iGender == 1 )
		{
			pkAttachWindow = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_SKIN_M + i, 0, 0, 226, 79, pkAttachWindow);
			((SPWindowStatic*)pkAttachWindow)->SetWindowType(WND_TYPE_TOPBASE);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 429, 227, 432);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 434, 227, 435);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 437, 227, 441);

			pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_GENDER_M + i, 2, 60, 7, 14, pkAttachWindow);
			pkChildWindow[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 453, 100);
		}
		else if( pkCashItemAttr->iGender == 2 )
		{
			pkAttachWindow = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_SKIN_M + i, 0, 0, 226, 79, pkAttachWindow);
			((SPWindowStatic*)pkAttachWindow)->SetWindowType(WND_TYPE_TOPBASE);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 442, 227, 445);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 447, 227, 448);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 450, 227, 454);

			pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_GENDER_W + i, 2, 60, 7, 14, pkAttachWindow);
			pkChildWindow[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 453, 122);
		}
		else
		{
			pkAttachWindow = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_SKIN_M + i, 0, 0, 226, 79, pkAttachWindow);
			((SPWindowStatic*)pkAttachWindow)->SetWindowType(WND_TYPE_TOPBASE);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 416, 227, 419);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 421, 227, 422);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 424, 227, 428);

			pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_GENDER + i, 2, 40, 7, 36, pkAttachWindow);
			pkChildWindow[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 453, 100);
		}

		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_SLOT_IMAGE + i, 10, 2, 74, 73, pkAttachWindow);
		((SPWindowStatic*)pkChildWindow[0])->SetWindowType(WND_TYPE_TOPBASE);
		((SPWindowStatic*)pkChildWindow[0])->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 455, 75, 460);
		((SPWindowStatic*)pkChildWindow[0])->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 462, 75, 465);
		((SPWindowStatic*)pkChildWindow[0])->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 467, 75, 472);

		pkChildWindow[1] = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_IMAGE_BG + i, 20, 50, 35, 11, pkChildWindow[0]);
		((SPWindowStatic*)pkChildWindow[1])->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 443, 137);

		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_IMAGE + i, 22, 22, 32, 32, pkChildWindow[0]);
		pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(
			pkCashItemAttr->iIcon, pkCashItemAttr->iIconIndex, pt);
		if( pkTexture )
		{
			SPWindowStatic * pkChildStatic = static_cast<SPWindowStatic *>(pkChildWindow[0]);
			pkChildStatic->SetWindowType(WND_TYPE_RECT);
			pkChildStatic->SetImageRect(pkTexture, pt.x, pt.y, pt.x+32, pt.y+32);
			pkTexture->IncreaseRef();
		}

		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_NAME_BG + i, 84, 2, 140, 15, pkAttachWindow);
		((SPWindowStatic*)pkChildWindow[0])->SetWindowType(WND_TYPE_LEFTBASE);
		((SPWindowStatic*)pkChildWindow[0])->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 306, 110, 309, 125);
		((SPWindowStatic*)pkChildWindow[0])->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 311, 110, 312, 125);
		((SPWindowStatic*)pkChildWindow[0])->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 314, 110, 317, 125);
			
		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_NAME + i, 2, 2, 136, 12, pkChildWindow[0]);
		pkChildWindow[0]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pkChildWindow[0]->SetFormat(DT_VCENTER | DT_RIGHT);
		pkChildWindow[0]->SetWindowText(pkCashItemAttr->strName.c_str());


		SPWindowStatic * pkChildStatic = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_PRICE_BACK + i, 158, 34, 62, 14, pkAttachWindow);
		pkChildStatic->SetWindowType(WND_TYPE_LEFTBASE);
		pkChildStatic->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 493, 497, 495, 511);
		pkChildStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 497, 497, 499, 511);
		pkChildStatic->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 501, 497, 503, 511);

		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_OZ_IMAGE + i, 204, 36, 15, 11, pkAttachWindow);
		if( m_iMileageRatio )
		{
			pkChildWindow[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);
#ifdef __CASHSHOP_POINT_SHOP__
			pkChildWindow[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 125);
#endif
		}
		else
			pkChildWindow[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

		int iPrice = pkCashItemAttr->iPrice;
		if( pkCashItemAttr->iSalePrice )
			iPrice = pkCashItemAttr->iSalePrice;
		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_PRICE + i, 160, 36, 42, 12, pkAttachWindow);
		pkChildWindow[0]->SetFont(FONT_12_NORMAL);
		//pkChildWindow[0]->SetFontColor(ARGB(255, 54, 74, 107));
		pkChildWindow[0]->SetFontColor(ARGB(255, 255, 255, 255));
		pkChildWindow[0]->SetFormat(DT_VCENTER | DT_RIGHT);
		pkChildWindow[0]->SetWindowText(ConvertMoneyToString(iPrice));

		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM_SALE_PRICE + i, 115, 36, 42, 12, pkAttachWindow);
		pkChildWindow[0]->SetFontColor(ARGB(255, 255, 0, 0));
		pkChildWindow[0]->SetFormat(DT_VCENTER | DT_CENTER);
		if( pkCashItemAttr->iSalePrice )
		{
			rcDst.left = 365 + (i % 2) * 229;
			rcDst.top = 124 + (i / 2) * 82;	// 8磊啊 救焊捞绰 何盒锭巩俊 2 侨伎 酒贰肺 [9/10/2007 AJJIYA]
			rcDst.right = rcDst.left + 40;
			rcDst.bottom = rcDst.top + 5;
			m_vSaleArrow.push_back(rcDst);

			pkChildWindow[0]->SetWindowText(ConvertMoneyToString(pkCashItemAttr->iPrice));
		}

		pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ITEM_BUY + i, 84, 59, 47, 17, pkAttachWindow);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 371, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 371, 110);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 371, 128);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 371, 146);
		pkButton->SetEnable( !pkCashItemAttr->bBuyDisable );
		
		pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ITEM_GIFT + i, 131, 59, 47, 17, pkAttachWindow);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 418, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 418, 110);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 418, 128);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 418, 146);
		pkButton->SetEnable( !pkCashItemAttr->bGiftDisable );

		if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_GIFT) )
			pkButton->SetEnable(false);

		pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ITEM_EXACT + i, 178, 59, 47, 17, pkAttachWindow);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 465, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 465, 110);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 465, 128);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 465, 146);

		pkAttachWindow->Show();

		//推荐按钮
		pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ITEM_SKIN_W + i, 86, 19, 66, 16, pkAttachWindow);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 253, 248);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 253, 265);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 253, 282);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 253, 248);
		if ( g_pItemCluster->RefreshCaseList(pkCashItemAttr) )
			pkButton->Show();
		else
			pkButton->Hide();
		
		if( pkCashItemAttr->iEventIcon )
		{
			EVENT_ANIMATION  Event;

			Event.fAccmulateTime = 0.0f;
			Event.fDelayTime = 0.5f;
			Event.iTotalFrame = 2;
			Event.iCurFrame = 0;

			Event.rcDst.left = 250 + (i % 2) * 231;
			Event.rcDst.top = 82 + (i / 2) * 82;
			Event.rcDst.right = Event.rcDst.left + 55;
			Event.rcDst.bottom = Event.rcDst.top + 29;

			switch( pkCashItemAttr->iEventIcon )
			{
			case 1:		//new
				SetRect(&Event.rcSrc[0], 456, 209, 511, 238);
				SetRect(&Event.rcSrc[1], 456, 239, 511, 268);
				break;
			case 2:		// hot
				SetRect(&Event.rcSrc[0], 456, 149, 511, 178);
				SetRect(&Event.rcSrc[1], 456, 179, 511, 208);
				break;
			case 3:		// sale
				SetRect(&Event.rcSrc[0], 456, 329, 511, 358);
				SetRect(&Event.rcSrc[1], 456, 359, 511, 388);
				break;
			case 4:		// event
				SetRect(&Event.rcSrc[0], 456, 269, 511, 298);
				SetRect(&Event.rcSrc[1], 456, 299, 511, 328);
				break;
			default:
				SetRect(&Event.rcDst, 0, 0, 0, 0);
				SetRect(&Event.rcSrc[0], 0, 0, 0, 0);
				SetRect(&Event.rcSrc[1], 0, 0, 0, 0);
				break;
			}

			m_vEventAnimation.push_back(Event);
		}

		if( m_iStoreCategory[0] == CI_EVENT && m_iStoreCategory[1] == 2 && m_iStoreCurPage == 0 )
		{
			rcDst.left = 250 + (i % 2) * 231;
			rcDst.top = 110 + (i / 2) * 80;
			rcDst.right = rcDst.left + 16;
			rcDst.bottom = rcDst.top + 16;
			m_vSaleOrderArrow.push_back(rcDst);
		}
	}
}

bool SPWindowCashShop::RefreshSultListView(SPCashItemAttr* pkItemAttr)
{
	SPCashItemAttr* pkCashItemAttr;
	SPWindow* pkParentWindow, *pkAttachWindow, *pkChildWindow[2];
	SPWindowButton* pkButton;
	SPTexture* pkTexture;
	POINT  pt;
	int i;

	pkParentWindow = Find(WIID_CASHSHOP_SULT);
	if( pkParentWindow == NULL )
		return false;

	if ( !pkItemAttr || g_pItemCluster->RefreshCaseList(pkItemAttr) == false )
		return false;

	pkParentWindow->Show();
	for( i = 0; i < 6; i++ )
	{
		pkAttachWindow = pkParentWindow->Find(WIID_CASHSHOP_SULT_ITEM + i, true);
		if( pkAttachWindow )
			pkAttachWindow->CleanChildAll();
		else
			break;

		pkCashItemAttr = NULL;
		if (i == 0)
			pkCashItemAttr = pkItemAttr;
		else
			pkCashItemAttr = g_pItemCluster->GetCaseListItem(i-1);
		
		if( pkCashItemAttr == NULL )
			break;
		
		pkAttachWindow->Show();
		if( pkCashItemAttr->iGender == 1 )
		{
			pkAttachWindow = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_SKIN + i, 0, 0, 226, 79, pkAttachWindow);
			((SPWindowStatic*)pkAttachWindow)->SetWindowType(WND_TYPE_TOPBASE);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 429, 227, 432);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 434, 227, 435);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 437, 227, 441);

			pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_GENDER + i, 2, 62, 7, 14, pkAttachWindow);
			pkChildWindow[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 453, 100);
		}
		else if( pkCashItemAttr->iGender == 2 )
		{
			pkAttachWindow = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_SKIN + i, 0, 0, 226, 79, pkAttachWindow);
			((SPWindowStatic*)pkAttachWindow)->SetWindowType(WND_TYPE_TOPBASE);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 442, 227, 445);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 447, 227, 448);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 450, 227, 454);

			pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_GENDER + i, 2, 62, 7, 14, pkAttachWindow);
			pkChildWindow[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 453, 122);
		}
		else
		{
			pkAttachWindow = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_SKIN + i, 0, 0, 226, 79, pkAttachWindow);
			((SPWindowStatic*)pkAttachWindow)->SetWindowType(WND_TYPE_TOPBASE);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 416, 227, 419);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 421, 227, 422);
			((SPWindowStatic*)pkAttachWindow)->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 424, 227, 428);

			pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_GENDER + i, 2, 40, 7, 36, pkAttachWindow);
			pkChildWindow[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 453, 100);
		}

		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_SLOT_IMAGE + i, 10, 2, 74, 73, pkAttachWindow);
		((SPWindowStatic*)pkChildWindow[0])->SetWindowType(WND_TYPE_TOPBASE);
		((SPWindowStatic*)pkChildWindow[0])->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 455, 75, 460);
		((SPWindowStatic*)pkChildWindow[0])->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 462, 75, 465);
		((SPWindowStatic*)pkChildWindow[0])->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 467, 75, 472);

		pkChildWindow[1] = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_IMAGE_BG + i, 20, 50, 35, 11, pkChildWindow[0]);
		((SPWindowStatic*)pkChildWindow[1])->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 443, 137);

		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_IMAGE + i, 22, 22, 32, 32, pkChildWindow[0]);
		pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(
			pkCashItemAttr->iIcon, pkCashItemAttr->iIconIndex, pt);
		if( pkTexture )
		{
			SPWindowStatic * pkChildStatic = static_cast<SPWindowStatic *>(pkChildWindow[0]);
			pkChildStatic->SetWindowType(WND_TYPE_RECT);
			pkChildStatic->SetImageRect(pkTexture, pt.x, pt.y, pt.x+32, pt.y+32);
			pkTexture->IncreaseRef();
		}

		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_NAME_BG + i, 84, 2, 140, 15, pkAttachWindow);
		((SPWindowStatic*)pkChildWindow[0])->SetWindowType(WND_TYPE_LEFTBASE);
		((SPWindowStatic*)pkChildWindow[0])->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 306, 110, 309, 125);
		((SPWindowStatic*)pkChildWindow[0])->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 311, 110, 312, 125);
		((SPWindowStatic*)pkChildWindow[0])->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 314, 110, 317, 125);
			
		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_NAME + i, 2, 2, 136, 12, pkChildWindow[0]);
		pkChildWindow[0]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pkChildWindow[0]->SetFormat(DT_VCENTER | DT_RIGHT);
		pkChildWindow[0]->SetWindowText(pkCashItemAttr->strName.c_str());

		if (i == 0)
		{
			pkButton = new SPWindowButton(WIID_CASHSHOP_SULT_ITEM_EXIT, 88, 20, 12, 11, pkAttachWindow);
			pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 262, 1);
			pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 262, 14);
			pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 262, 28);
			pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 262, 40);
		}

		SPWindowStatic * pkChildStatic = new SPWindowStatic(WIID_CASHSHOP_SULT_PRICE_BACK + i, 158, 34, 62, 14, pkAttachWindow);
		pkChildStatic->SetWindowType(WND_TYPE_LEFTBASE);
		pkChildStatic->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 493, 497, 495, 511);
		pkChildStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 497, 497, 499, 511);
		pkChildStatic->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 501, 497, 503, 511);

		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_OZ_IMAGE + i, 204, 36, 15, 11, pkAttachWindow);
		if( m_iMileageRatio )
		{
			pkChildWindow[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);
#ifdef __CASHSHOP_POINT_SHOP__
			pkChildWindow[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 125);
#endif
		}
		else
			pkChildWindow[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

		int iPrice = pkCashItemAttr->iPrice;
		if( pkCashItemAttr->iSalePrice )
			iPrice = pkCashItemAttr->iSalePrice;
		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_PRICE + i, 160, 36, 44, 12, pkAttachWindow);
		pkChildWindow[0]->SetFont(FONT_12_NORMAL);
		//pkChildWindow[0]->SetFontColor(ARGB(255, 54, 74, 107));
		pkChildWindow[0]->SetFontColor(ARGB(255, 255, 255, 255));
		pkChildWindow[0]->SetFormat(DT_VCENTER | DT_RIGHT);
		pkChildWindow[0]->SetWindowText(ConvertMoneyToString(iPrice));

		pkChildWindow[0] = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_SALE_PRICE + i, 115, 36, 44, 12, pkAttachWindow);
		pkChildWindow[0]->SetFontColor(ARGB(255, 255, 0, 0));
		pkChildWindow[0]->SetFormat(DT_VCENTER | DT_CENTER);
		if( pkCashItemAttr->iSalePrice )
		{
			pkChildWindow[0]->SetWindowText(ConvertMoneyToString(pkCashItemAttr->iPrice));
		}

		pkButton = new SPWindowButton(WIID_CASHSHOP_SULT_ITEM_BUY + i, 84, 59, 47, 17, pkAttachWindow);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 371, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 371, 110);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 371, 128);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 371, 146);
		pkButton->SetEnable( !pkCashItemAttr->bBuyDisable );
		
		pkButton = new SPWindowButton(WIID_CASHSHOP_SULT_ITEM_GIFT + i, 131, 59, 47, 17, pkAttachWindow);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 418, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 418, 110);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 418, 128);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 418, 146);
		pkButton->SetEnable( !pkCashItemAttr->bGiftDisable );

		if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_GIFT) )
			pkButton->SetEnable(false);

		pkButton = new SPWindowButton(WIID_CASHSHOP_SULT_ITEM_EXACT + i, 178, 59, 47, 17, pkAttachWindow);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 465, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 465, 110);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 465, 128);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 465, 146);

		pkAttachWindow->Show();

		//推荐按钮
		//pkButton = new SPWindowButton(WIID_CASHSHOP_SULT_ITEM_SKIN_W + i, 86, 19, 66, 16, pkAttachWindow);
		//pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 253, 248);
		//pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 253, 265);
		//pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 253, 282);
		//pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 253, 248);
		//if ( i == 0 )
		//	pkButton->SetEnable(false);
		//else
		//	pkButton->Hide();

	}
	return true;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ShowStorePopupMenu(int iID, bool bToggle)
{
	SPWindow* pkParentWindow, *pkChildWindow;
	
	int i;
	for( i = WIID_CASHSHOP_STORE_FAS_DRESS_POP; i <= WIID_CASHSHOP_STORE_ETC_COMMU_POP; i++ )
	{
		pkParentWindow = Find(i, true);
		if( pkParentWindow == NULL )
			continue;

		bool bShow = true;
		if( i == iID )
		{
			if( bToggle )
			{
				if( pkParentWindow->IsShow() )
					bShow = false;
			}
		}
		else
			bShow = false;

		if( bShow )
			pkParentWindow->Show();
		else
			pkParentWindow->Hide();
	}

	pkParentWindow = Find(iID, true);
	if( pkParentWindow == NULL )
		return;
	
	int iMin = 0, iMax = 0, iDiff = 0;
	switch( iID )
	{
	case WIID_CASHSHOP_STORE_FAS_DRESS_POP:
		iMin = WIID_CASHSHOP_STORE_FAS_DRESS_TOTAL;
		iMax = WIID_CASHSHOP_STORE_FAS_DRESS_COAT;
		iDiff = CIS2_ONEPEACE;
		break;
	case WIID_CASHSHOP_STORE_FAS_HELMET_POP:
		iMin = WIID_CASHSHOP_STORE_FAS_HELMET_TOTAL;
		iMax = WIID_CASHSHOP_STORE_FAS_HELMET_PIN;
		iDiff = CIS2_HELMET;
		break;
	case WIID_CASHSHOP_STORE_FAS_SHOES_POP:
		iMin = WIID_CASHSHOP_STORE_FAS_SHOES_TOTAL;
		iMax = WIID_CASHSHOP_STORE_FAS_SHOES_BAND;
		iDiff = CIS2_SHOES;
		break;
	case WIID_CASHSHOP_STORE_FAS_GLOVE_POP:
		iMin = WIID_CASHSHOP_STORE_FAS_GLOVE_TOTAL;
		iMax = WIID_CASHSHOP_STORE_FAS_GLOVE_BAND;
		iDiff = CIS2_GLOVE;
		break;
	case WIID_CASHSHOP_STORE_FAS_ACCE_POP:
		iMin = WIID_CASHSHOP_STORE_FAS_ACCE_TOTAL;
		iMax = WIID_CASHSHOP_STORE_FAS_ACCE_STOCK;
		iDiff = CIS2_GLASS;
		break;
	case WIID_CASHSHOP_STORE_ETC_COMMU_POP:
		iMin = WIID_CASHSHOP_STORE_ETC_COMMU_TOTAL;
		iMax = WIID_CASHSHOP_STORE_ETC_COMMU_CHAT;
		iDiff = CIS2_EMOTICON;
		break;
	}
	
	int iIndex;
	for( i = iMin; i <= iMax; i++ )
	{	
		pkChildWindow = pkParentWindow->Find(i, true);
		if( pkChildWindow )
		{
			BOOL bCheck = FALSE;
			iIndex = 0;

			if( m_iStoreCategory[2] )
				iIndex = m_iStoreCategory[2] - iDiff + 1;
			else
				iIndex = m_iStoreCategory[2];

			if( (i - iMin) == iIndex )
				bCheck = TRUE;

			pkChildWindow->SPSendMessage(SPIM_BTN_SELECT, bCheck);
		}
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::HideStorePopupMenu(int iID)
{
	SPWindow* pkWindow;
	if( iID == 0 )
	{
		int i;
		for( i = WIID_CASHSHOP_STORE_FAS_DRESS_POP; i <= WIID_CASHSHOP_STORE_ETC_COMMU_POP; i++ )
		{
			pkWindow = Find(i, true);
			if( pkWindow )
				pkWindow->Hide();
		}
	}
	else
	{
		pkWindow = Find(iID, true);
		if( pkWindow )
			pkWindow->Hide();
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::UpdateItemList()
{
	if( g_pItemCluster == NULL )
		return;

	g_pItemCluster->SortCashItem(m_iSortType[0], m_iSortType[1]);
	g_pItemCluster->RandCashItem();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetStorePageText()
{
	SPWindow* pkParentWindow;
	SPWindowPageSelect* pkWindow;

	pkParentWindow = Find(WIID_CASHSHOP_STORE);
	if( pkParentWindow == NULL )
		return;

	pkWindow = (SPWindowPageSelect*)pkParentWindow->Find(WIID_CASHSHOP_STORE_PAGE, true);
	if( pkWindow == NULL )
		return;

	pkWindow->SetMaxPageInfo(m_iStoreMaxPage);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ShowCashierDlg(SPCashItemAttr* pkItemAttr)
{
	m_vCashierItemAttr.clear();
	m_vCashierItemAttr.push_back(pkItemAttr);
	if( pkItemAttr == NULL )
		return;

	SPWindow* pkParentWindow, *pkChildWindow;
	pkParentWindow = Find(WIID_CASHSHOP_CASHIER);
	if( pkParentWindow == NULL )
		return;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_ITEM, true);
	if( pkChildWindow )
	{
		pkChildWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);

		SPTexture* pkTexture;
		POINT pt;
		pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(
			pkItemAttr->iIcon, pkItemAttr->iIconIndex, pt);
		if( pkTexture )
			pkChildWindow->SetImage(pkTexture, pt.x, pt.y);
	}

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_NAME, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(pkItemAttr->strName.c_str());

	int iPrice = pkItemAttr->iPrice;
	if( pkItemAttr->iSalePrice )
		iPrice = pkItemAttr->iSalePrice;
	//CaluCashierGlow(iPrice);
	//int iNum10 = (iPrice / 10) % 10;
	//if( iNum10 )
	//	iPrice += 100 - (iNum10 * 10);

	m_iCahsierPrice = iPrice;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_PRICE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_iCahsierPrice));

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_USE_PRI, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_iCahsierPrice));

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_MY_OZ, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_uiCurOZ));

	if( m_iMileageRatio )
	{
		int iMileageLimit = 0;
		if( m_iMileageGlowUnit > 0 )
			iMileageLimit = (m_uiCurMileage / m_iMileageGlowUnit) * m_iMileageGlowUnit;

		if( pkItemAttr->iSalePrice )
		{
			m_iCashierMileageRange[0] = (m_iCahsierPrice - pkItemAttr->iMaxOZ);
			m_iCashierMileageRange[1] = min(pkItemAttr->iMaxMile, iMileageLimit);
		}
		else
		{
			m_iCashierMileageRange[0] = (pkItemAttr->iPrice - pkItemAttr->iMaxOZ);
			m_iCashierMileageRange[1] = min(pkItemAttr->iMaxMile, iMileageLimit);
		}
	}
	else
	{
		m_iCashierMileageRange[0] = 0;
		m_iCashierMileageRange[1] = 0;
	}

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_USE_MILE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_iCashierMileageRange[1]));

	int iMileage = m_iCashierMileageRange[0];
	if( m_uiCurOZ < m_iCahsierPrice && m_iCashierMileageRange[1] )
	{
		if( m_iMileageGlowUnit > 0 )
		{
			int i;
			for( i = m_iCashierMileageRange[0]; i <= m_iCashierMileageRange[1]; i += m_iMileageGlowUnit )
			{
				if( m_uiCurOZ + i >= m_iCahsierPrice )
				{
					iMileage = i;
					break;
				}
			}
		}
	}
	CalcCashierMoney(m_iCahsierPrice - iMileage, iMileage);

	//////////////////////////////////////////////////////////////////////////

#ifdef __CASHSHOP_POINT_SHOP__
	if( m_iMileageRatio )
	{
		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_PLUS, true);
		if( pkChildWindow )
			pkChildWindow->SetShowEnable( true );

		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_MINUS, true);
		if( pkChildWindow )
			pkChildWindow->SetShowEnable( true );

		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_USE_MILE_BG, true);
		if( pkChildWindow )
			pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );

		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_OZ_ICON, true);
		if( pkChildWindow )
			pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );

		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_USE_PRI_ICON, true);
		if( pkChildWindow )
			pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );

		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_AF_MY_OZ_ICON, true);
		if( pkChildWindow )
			pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );

		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_USE_MILE, true);
		if( pkChildWindow )
			pkChildWindow->SetEnable( true );

		if( /*m_pkCashierItemAttr*/pkItemAttr->iMaxOZ <= 0 )
		{
			if( m_iCahsierPrice > m_uiCurMileage )
			{
				SetErrorDlg( 9001011 );
				return;
			}

			pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_PLUS, true);
			if( pkChildWindow )
				pkChildWindow->SetShowEnable( false );

			pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_MINUS, true);
			if( pkChildWindow )
				pkChildWindow->SetShowEnable( false );

			pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_USE_MILE_BG, true);
			if( pkChildWindow )
				pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );

			pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_OZ_ICON, true);
			if( pkChildWindow )
				pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );

			pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_USE_PRI_ICON, true);
			if( pkChildWindow )
				pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );

			pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_AF_MY_OZ_ICON, true);
			if( pkChildWindow )
				pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );

			pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_USE_MILE, true);
			if( pkChildWindow )
				pkChildWindow->SetEnable( false );
		}
	}
#endif

	//////////////////////////////////////////////////////////////////////////

	pkParentWindow->Show();
	g_pInterfaceManager->SetModalWindow(pkParentWindow);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ShowGiftDlg(SPCashItemAttr* pkItemAttr)
{
	m_vCashierItemAttr.clear();
	m_vCashierItemAttr.push_back(pkItemAttr);
	if( pkItemAttr == NULL || g_pItemCluster == NULL )
		return;

	SPWindow* pkParentWindow, *pkChildWindow;
	pkParentWindow = Find(WIID_CASHSHOP_GIFT);
	if( pkParentWindow == NULL )
		return;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_RECEIVE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(NULL);

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_MESSAGE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(NULL);

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_ITEM, true);
	if( pkChildWindow )
	{
		pkChildWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);

		SPTexture* pkTexture;
		POINT pt;
		pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(
			pkItemAttr->iIcon, pkItemAttr->iIconIndex, pt);
		if( pkTexture )
			pkChildWindow->SetImage(pkTexture, pt.x, pt.y);
	}

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_NAME, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(pkItemAttr->strName.c_str());

	int iPrice = pkItemAttr->iPrice;
	if( pkItemAttr->iSalePrice )
		iPrice = pkItemAttr->iSalePrice;
	//CaluCashierGlow(iPrice);
	//int iNum10 = (iPrice / 10) % 10;
	//if( iNum10 )
	//	iPrice += 100 - (iNum10 * 10);

	m_iCahsierPrice = iPrice;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_PRICE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_iCahsierPrice));

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_USE_PRI, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_iCahsierPrice));

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_MY_OZ, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_uiCurOZ));
	
	if( m_iMileageRatio )
	{
		int iMileageLimit = 0;
		if( m_iMileageGlowUnit > 0 )
			iMileageLimit = (m_uiCurMileage / m_iMileageGlowUnit) * m_iMileageGlowUnit;

		if( pkItemAttr->iSalePrice )
		{
			m_iCashierMileageRange[0] = (m_iCahsierPrice - pkItemAttr->iMaxOZ);
			m_iCashierMileageRange[1] = min(pkItemAttr->iMaxMile, iMileageLimit);
		}
		else
		{
			m_iCashierMileageRange[0] = (pkItemAttr->iPrice - pkItemAttr->iMaxOZ);
			m_iCashierMileageRange[1] = min(pkItemAttr->iMaxMile, iMileageLimit);
		}
	}
	else
	{
		m_iCashierMileageRange[0] = 0;
		m_iCashierMileageRange[1] = 0;
	}

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_USE_MILE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_iCashierMileageRange[1]));

	int iMileage = m_iCashierMileageRange[0];
	if( m_uiCurOZ < m_iCahsierPrice && m_iCashierMileageRange[1] )
	{
		if( m_iMileageGlowUnit > 0 )
		{
			int i;
			for( i = m_iCashierMileageRange[0]; i <= m_iCashierMileageRange[1]; i += m_iMileageGlowUnit )
			{
				if( m_uiCurOZ + i >= m_iCahsierPrice )
				{
					iMileage = i;
					break;
				}
			}
		}
	}
	CalcGiftMoney(m_iCahsierPrice - iMileage, iMileage);

	pkParentWindow->Show();
	g_pInterfaceManager->SetModalWindow(pkParentWindow);
	UpdateFriendList();

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_FRIEND, true);
	if( pkChildWindow )
		pkChildWindow->Hide();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ShowExactDlg(SPCashItemAttr* pkItemAttr)
{
	m_vCashierItemAttr.clear();
	m_vCashierItemAttr.push_back(pkItemAttr);
	if( pkItemAttr == NULL || g_pItemCluster == NULL )
		return;

	SPWindow* pkParentWindow, *pkChildWindow;
	pkParentWindow = Find(WIID_CASHSHOP_EXACT);
	if( pkParentWindow == NULL )
		return;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_EXACT_RECEIVE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(NULL);

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_EXACT_MESSAGE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(NULL);

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_EXACT_ITEM, true);
	if( pkChildWindow )
	{
		pkChildWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);

		SPTexture* pkTexture;
		POINT pt;
		pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(
			pkItemAttr->iIcon, pkItemAttr->iIconIndex, pt);
		if( pkTexture )
			pkChildWindow->SetImage(pkTexture, pt.x, pt.y);
	}

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_EXACT_NAME, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(pkItemAttr->strName.c_str());

	int iPrice = pkItemAttr->iPrice;
	if( pkItemAttr->iSalePrice )
		iPrice = pkItemAttr->iSalePrice;

	m_iCahsierPrice = iPrice;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_EXACT_PRICE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_iCahsierPrice));


	pkParentWindow->Show();
	g_pInterfaceManager->SetModalWindow(pkParentWindow);
	UpdateFriendList();

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_EXACT_FRIEND, true);
	if( pkChildWindow )
		pkChildWindow->Hide();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::UpdateFriendList()
{
	SPWindowTreeBox* pkTreeBox;
	pkTreeBox = (SPWindowTreeBox*)Find(WIID_CASHSHOP_GIFT_FRIEND_LIST, true);
	if( pkTreeBox == NULL )
		return;

	pkTreeBox->DelItemAll();
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	int iSize, i, j;
	FRIEND_INFO* pFriendInfo;
	SPPlayer* pkPlayer = g_pGOBManager->GetLocalPlayer();
	iSize = pkPlayer->GetFriendListCount();
	
	SPTreeBoxItemTexture* pItem;
	for( i = 0; i < iSize; i++ )
	{
		pFriendInfo = pkPlayer->GetFriendListMember(i);
		if( pFriendInfo == NULL )
			continue;

		pItem = new SPTreeBoxItemTexture;
		pItem->SetReturnValue(i);
		pItem->SetAlign(DT_VCENTER | DT_CENTER);
		pItem->SetSize(127, 12);
		pItem->SetTextMargin(0, 0);
		pItem->SetText(pFriendInfo->szCharName);

		for( j = 0 ; j < DATA_STRING_COUNT ; ++j )
		{
			if( j == (DATA_STRING_DISABLE * 2) || j == (DATA_STRING_DISABLE * 2) + 1 )
				pItem->SetTextColor(j, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			else
				pItem->SetTextColor(j, D3DXCOLOR(0.5607f, 0.3882f, 0.3882f, 1.0f));
		}

		pItem->SetTexture( DATA_TEXTURE_HIT * 2, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_HIT * 2 + 1, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_PUSH * 2, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_PUSH * 2 + 1, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_SELECT * 2, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_SELECT * 2 + 1, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pkTreeBox->AddItem(NULL, pItem);
	}
	pkTreeBox->Refresh();

	//[xialin 2008/07/10] 索取功能 >>>
	pkTreeBox = (SPWindowTreeBox*)Find(WIID_CASHSHOP_EXACT_FRIEND_LIST, true);
	if( pkTreeBox == NULL )
		return;

	pkTreeBox->DelItemAll();
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	pkPlayer = g_pGOBManager->GetLocalPlayer();
	iSize = pkPlayer->GetFriendListCount();
	
	for( i = 0; i < iSize; i++ )
	{
		pFriendInfo = pkPlayer->GetFriendListMember(i);
		if( pFriendInfo == NULL )
			continue;

		pItem = new SPTreeBoxItemTexture;
		pItem->SetReturnValue(i);
		pItem->SetAlign(DT_VCENTER | DT_CENTER);
		pItem->SetSize(127, 12);
		pItem->SetTextMargin(0, 0);
		pItem->SetText(pFriendInfo->szCharName);

		for( j = 0 ; j < DATA_STRING_COUNT ; ++j )
		{
			if( j == (DATA_STRING_DISABLE * 2) || j == (DATA_STRING_DISABLE * 2) + 1 )
				pItem->SetTextColor(j, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			else
				pItem->SetTextColor(j, D3DXCOLOR(0.5607f, 0.3882f, 0.3882f, 1.0f));
		}

		pItem->SetTexture( DATA_TEXTURE_HIT * 2, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_HIT * 2 + 1, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_PUSH * 2, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_PUSH * 2 + 1, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_SELECT * 2, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_SELECT * 2 + 1, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pkTreeBox->AddItem(NULL, pItem);
	}
	pkTreeBox->Refresh();
	//[xialin 2008/07/10] 索取功能 <<<

	//[xialin 2008/07/10] 批量索取功能 >>>
	pkTreeBox = (SPWindowTreeBox*)Find(WIID_CASHSHOP_EXACTLIST_FRIEND_LIST, true);
	if( pkTreeBox == NULL )
		return;

	pkTreeBox->DelItemAll();
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	pkPlayer = g_pGOBManager->GetLocalPlayer();
	iSize = pkPlayer->GetFriendListCount();
	
	for( i = 0; i < iSize; i++ )
	{
		pFriendInfo = pkPlayer->GetFriendListMember(i);
		if( pFriendInfo == NULL )
			continue;

		pItem = new SPTreeBoxItemTexture;
		pItem->SetReturnValue(i);
		pItem->SetAlign(DT_VCENTER | DT_CENTER);
		pItem->SetSize(127, 12);
		pItem->SetTextMargin(0, 0);
		pItem->SetText(pFriendInfo->szCharName);

		for( j = 0 ; j < DATA_STRING_COUNT ; ++j )
		{
			if( j == (DATA_STRING_DISABLE * 2) || j == (DATA_STRING_DISABLE * 2) + 1 )
				pItem->SetTextColor(j, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			else
				pItem->SetTextColor(j, D3DXCOLOR(0.5607f, 0.3882f, 0.3882f, 1.0f));
		}

		pItem->SetTexture( DATA_TEXTURE_HIT * 2, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_HIT * 2 + 1, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_PUSH * 2, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_PUSH * 2 + 1, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_SELECT * 2, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pItem->SetTexture( DATA_TEXTURE_SELECT * 2 + 1, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 107 , 65 , 113 , 71 );
		pkTreeBox->AddItem(NULL, pItem);
	}
	pkTreeBox->Refresh();
	//[xialin 2008/07/10] 批量索取功能 <<<
}

//------------------------------------------------------------------------------------
SPCashItemAttr* SPWindowCashShop::GetStoreCashItemAttr(int iIndex)
{
	SPCashItemAttr* pkCashItemAttr = NULL;
	
	//[xialin 2008/07/17] eidt >>>
	//if( m_eStoreTabType != CST_SEARCH )
	//	pkCashItemAttr = g_pItemCluster->GetSortCashItem(ConvertCashCategory(), iIndex + m_iStoreCurPage * 10);
	//else
	//{
	//	if( iIndex < (int)m_vCartList.size() )
	//		pkCashItemAttr = g_pItemCluster->GetMinorCashItem(m_vCartList.at(iIndex));
	//}
	pkCashItemAttr = g_pItemCluster->GetSortCashItem(ConvertCashCategory(), iIndex + m_iStoreCurPage * 10);
	//[xialin 2008/07/17] eidt <<<
	
	return pkCashItemAttr;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::CalcCashierMoney(int iOZ, int iMileage)
{
	m_iCashierOZ = iOZ;
	m_iCashierMileage = iMileage;
	
	if( m_iCashierOZ > m_uiCurOZ )
		m_iCashierOZ = m_uiCurOZ;
	if( m_iCashierMileage > m_uiCurMileage )
	{
		int iMileageLimit = 0;
		if( m_iMileageGlowUnit > 0 )
			iMileageLimit = (m_uiCurMileage / m_iMileageGlowUnit) * m_iMileageGlowUnit;

		m_iCashierMileage = iMileageLimit;
	}

	SPWindow* pkParentWindow, *pkChildWindow;

	pkParentWindow = Find(WIID_CASHSHOP_CASHIER);
	if( pkParentWindow == NULL )
		return;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_USE_OZ, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_iCashierOZ));

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_USE_MILE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_iCashierMileage));

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_AF_MY_OZ, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_uiCurOZ - m_iCashierOZ));

#ifdef __CASHSHOP_POINT_SHOP__
	if( m_vCashierItemAttr.size() > 0 && m_vCashierItemAttr.back() != NULL )
	{
		if( m_vCashierItemAttr.back()->iMaxOZ <= 0 && m_iMileageRatio )
			if( pkChildWindow )
				pkChildWindow->SetWindowText(ConvertMoneyToString(m_uiCurMileage - m_iCashierMileage));
	}
#endif

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_TYPE, true);
	if( pkChildWindow )
	{
		if( (m_iCashierOZ + m_iCashierMileage) >= m_iCahsierPrice )
			pkChildWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);
		else
			pkChildWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 1);

#ifdef __CASHSHOP_POINT_SHOP__
		if( m_vCashierItemAttr.size() > 0 && m_vCashierItemAttr.back() != NULL )
		{
			if( m_vCashierItemAttr.back()->iMaxOZ <= 0 && m_iMileageRatio )
				pkChildWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 2);
		}
#endif
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::CalcGiftMoney(int iOZ, int iMileage)
{
	m_iCashierOZ = iOZ;
	m_iCashierMileage = iMileage;

	if( m_iCashierOZ > m_uiCurOZ )
		m_iCashierOZ = m_uiCurOZ;
	if( m_iCashierMileage > m_uiCurMileage )
	{
		int iMileageLimit = 0;
		if( m_iMileageGlowUnit > 0 )
			iMileageLimit = (m_uiCurMileage / m_iMileageGlowUnit) * m_iMileageGlowUnit;

		m_iCashierMileage = iMileageLimit;
	}

	SPWindow* pkParentWindow, *pkChildWindow;

	pkParentWindow = Find(WIID_CASHSHOP_GIFT);
	if( pkParentWindow == NULL )
		return;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_USE_OZ, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_iCashierOZ));

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_USE_MILE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_iCashierMileage));

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_AF_MY_OZ, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_uiCurOZ - m_iCashierOZ));

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_TYPE, true);
	if( pkChildWindow )
	{
		if( (m_iCashierOZ + m_iCashierMileage) >= m_iCahsierPrice )
			pkChildWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);
		else
			pkChildWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 1);
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetStoreToolTipEnter(SPCashItemAttr* pkCashItemAttr, LPARAM lParam)
{
	if( pkCashItemAttr == NULL )
		return;

	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return;

	if( pkCashItemAttr->eCategory == CI_PACKAGE )
		pkWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)pkCashItemAttr, -5);
	else
	{

		SPItemAttr* pkItemAttr = NULL;
		g_pItemCluster->GetItemInfo(pkCashItemAttr->ItemInfo[0].iID, pkItemAttr);
		if( pkItemAttr == NULL )
			return;

		SPItemStatus kItemStatus;
		kItemStatus.SetItemID(pkCashItemAttr->ItemInfo[0].iID);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pkItemAttr->m_ePosID1);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, pkItemAttr->m_iCategory);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, pkItemAttr->m_iNum);
		kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	pkItemAttr->m_iColorID);
		kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, pkItemAttr->m_eRareLimit);

		if(pkItemAttr->m_eDuration != ITEM_DURATION_NULL) {
			kItemStatus.SetLifeTime(pkItemAttr->m_iLifeTime * 3600);
		}

		pkWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&kItemStatus, lParam);
	}

	pkWindow->Show();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetStoreToolTipMove(SPCashItemAttr* pkCashItemAttr, LPARAM lParam)
{
	if( pkCashItemAttr == NULL )
		return;

	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return;

	if( pkCashItemAttr->eCategory == CI_PACKAGE )
		pkWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)pkCashItemAttr, -5);
	else
	{

		SPItemAttr* pkItemAttr = NULL;
		g_pItemCluster->GetItemInfo(pkCashItemAttr->ItemInfo[0].iID, pkItemAttr);
		if( pkItemAttr == NULL )
			return;

		SPItemStatus kItemStatus;
		kItemStatus.SetItemID(pkCashItemAttr->ItemInfo[0].iID);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pkItemAttr->m_ePosID1);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, pkItemAttr->m_iCategory);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, pkItemAttr->m_iNum);
		kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	pkItemAttr->m_iColorID);
		kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, pkItemAttr->m_eRareLimit);

		if(pkItemAttr->m_eDuration != ITEM_DURATION_NULL) {
			kItemStatus.SetLifeTime(pkItemAttr->m_iLifeTime * 3600);
		}

		pkWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)&kItemStatus, lParam);
	}

	pkWindow->Show();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetStoreToolTipOut(LPARAM lParam)
{
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow )
	{
		pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, lParam);
		pkWindow->Show();
	}
}

//------------------------------------------------------------------------------------
// Message
int SPWindowCashShop::OnStorePage(WPARAM wParam, LPARAM lParam)
{
	m_iStoreCurPage = wParam;
	if( m_iStoreCurPage < 0 )
	{
		m_iStoreCurPage = 0;
		return 1;
	}

	//SetStorePageText();
	RefreshStore();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnChangeSortGender(WPARAM wParam, LPARAM lParam)
{
	if( m_iSortType[0] == wParam )
		return 1;

	m_iSortType[0] = wParam;
	UpdateItemList();
	CalcPageNum();
	SetStorePageText();
	RefreshStore();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnChangeSortPrice(WPARAM wParam, LPARAM lParam)
{
	if( m_iSortType[1] == wParam )
		return 1;

	m_iSortType[1] = wParam;
	UpdateItemList();
	CalcPageNum();
	SetStorePageText();
	RefreshStore();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreTabType(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	SetStoreTabType((CASHSHOP_STORE_TAB)(iID - WIID_CASHSHOP_STORE_EVENT + 1));
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreCategory1(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iRet, iPopupID = 0;
	if( iID >= WIID_CASHSHOP_STORE_EVENT_NEW &&
		iID <= WIID_CASHSHOP_STORE_EVENT_EVENT )
		iRet = SetStoreCashCategory(iID - WIID_CASHSHOP_STORE_EVENT_NEW, 1);
	else if( iID >= WIID_CASHSHOP_STORE_FASSION_DRESS &&
		iID <= WIID_CASHSHOP_STORE_FASSION_ETC )
	{
		iRet = SetStoreCashCategory(iID - WIID_CASHSHOP_STORE_FASSION_DRESS, 1);
		switch( iID )
		{
		case WIID_CASHSHOP_STORE_FASSION_DRESS:
			iPopupID = WIID_CASHSHOP_STORE_FAS_DRESS_POP;
			break;
		case WIID_CASHSHOP_STORE_FASSION_HELMET:
			iPopupID = WIID_CASHSHOP_STORE_FAS_HELMET_POP;
			break;
		case WIID_CASHSHOP_STORE_FASSION_SHOES:
			iPopupID = WIID_CASHSHOP_STORE_FAS_SHOES_POP;
			break;
		case WIID_CASHSHOP_STORE_FASSION_GLOVE:
			iPopupID = WIID_CASHSHOP_STORE_FAS_GLOVE_POP;
			break;
		case WIID_CASHSHOP_STORE_FASSION_ACCE:
			iPopupID = WIID_CASHSHOP_STORE_FAS_ACCE_POP;
			break;
		}
	}
	else if( iID >= WIID_CASHSHOP_STORE_PET_PET &&
		iID <= WIID_CASHSHOP_STORE_PET_ETC )
		iRet = SetStoreCashCategory(iID - WIID_CASHSHOP_STORE_PET_PET, 1);
	else if( iID >= WIID_CASHSHOP_STORE_ETC_FUNCTIONAL &&
		iID <= WIID_CASHSHOP_STORE_ETC_COMMUNITY )
	{
		iRet = SetStoreCashCategory(iID - WIID_CASHSHOP_STORE_ETC_FUNCTIONAL, 1);
		switch( iID )
		{
		case WIID_CASHSHOP_STORE_ETC_COMMUNITY:
			iPopupID = WIID_CASHSHOP_STORE_ETC_COMMU_POP;
			break;
		}
	}
	else if( iID >= WIID_CASHSHOP_STORE_PACK_FASSION &&
		iID <= WIID_CASHSHOP_STORE_PACK_ETC )
	{
		iRet = SetStoreCashCategory(iID - WIID_CASHSHOP_STORE_PACK_FASSION, 1);
	}
#ifdef __CASHSHOP_POINT_SHOP__
	else if( iID >= WIID_CASHSHOP_STORE_POINT_FASSION &&
		iID <= WIID_CASHSHOP_STORE_POINT_FUNCTIONAL )
	{
		iRet = SetStoreCashCategory(iID - WIID_CASHSHOP_STORE_POINT_FASSION, 1);
	}
#endif

	if( m_iStoreCategory[1] != iRet )
	{
		m_iStoreCategory[1] = iRet;
		m_iStoreCategory[2] = SetStoreCashCategory(0, 2);
		SetStoreCategory(iID);
	}
	
	ShowStorePopupMenu(iPopupID, true);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreCategory2(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iRet = 0;
	int iParentID;
	if( iID >= WIID_CASHSHOP_STORE_FAS_DRESS_TOTAL &&
		iID <= WIID_CASHSHOP_STORE_FAS_DRESS_COAT )
	{
		iRet = SetStoreCashCategory(iID - WIID_CASHSHOP_STORE_FAS_DRESS_TOTAL, 2);
		iParentID = WIID_CASHSHOP_STORE_FAS_DRESS_POP;
	}
	else if( iID >= WIID_CASHSHOP_STORE_FAS_HELMET_TOTAL &&
		iID <= WIID_CASHSHOP_STORE_FAS_HELMET_PIN )
	{
		iRet = SetStoreCashCategory(iID - WIID_CASHSHOP_STORE_FAS_HELMET_TOTAL, 2);
		iParentID = WIID_CASHSHOP_STORE_FAS_HELMET_POP;
	}
	else if( iID >= WIID_CASHSHOP_STORE_FAS_SHOES_TOTAL &&
		iID <= WIID_CASHSHOP_STORE_FAS_SHOES_BAND )
	{
		iRet = SetStoreCashCategory(iID - WIID_CASHSHOP_STORE_FAS_SHOES_TOTAL, 2);
		iParentID = WIID_CASHSHOP_STORE_FAS_SHOES_POP;
	}
	else if( iID >= WIID_CASHSHOP_STORE_FAS_GLOVE_TOTAL &&
		iID <= WIID_CASHSHOP_STORE_FAS_GLOVE_BAND )
	{
		iRet = SetStoreCashCategory(iID - WIID_CASHSHOP_STORE_FAS_GLOVE_TOTAL, 2);
		iParentID = WIID_CASHSHOP_STORE_FAS_GLOVE_POP;
	}
	else if( iID >= WIID_CASHSHOP_STORE_FAS_ACCE_TOTAL &&
		iID <= WIID_CASHSHOP_STORE_FAS_ACCE_STOCK )
	{
		iRet = SetStoreCashCategory(iID - WIID_CASHSHOP_STORE_FAS_ACCE_TOTAL, 2);
		iParentID = WIID_CASHSHOP_STORE_FAS_ACCE_POP;
	}
	else if( iID >= WIID_CASHSHOP_STORE_ETC_COMMU_TOTAL &&
		iID <= WIID_CASHSHOP_STORE_ETC_COMMU_CHAT )
	{
		iRet = SetStoreCashCategory(iID - WIID_CASHSHOP_STORE_ETC_COMMU_TOTAL, 2);
		iParentID = WIID_CASHSHOP_STORE_ETC_COMMU_POP;
	}

	HideStorePopupMenu(iParentID);
	if( m_iStoreCategory[2] != iRet )
	{
		m_iStoreCategory[2] = iRet;
		SetStoreCategory(0);
	}
	
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreLButtonDown(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	if ( g_pItemCluster == NULL )
		return 1;

	int iIndex = 0;
	SPCashItemAttr* pkCashItemAttr = NULL;

	//[xialin 2008/07/28] add
	if ( iID == WIID_CASHSHOP_SULT_ITEM_SLOT_IMAGE || iID == WIID_CASHSHOP_SULT_ITEM_IMAGE_BG 
		 || iID == WIID_CASHSHOP_SULT_ITEM_IMAGE )
	{
		pkCashItemAttr = m_pCurSultItemAttr;
		if( pkCashItemAttr == NULL )
			return 1;
	}
	else if ( iID >= WIID_CASHSHOP_SULT_ITEM_SLOT_IMAGE && iID <= WIID_CASHSHOP_SULT_ITEM_IMAGE+5 )
	{
		if( iID < WIID_CASHSHOP_SULT_ITEM_IMAGE_BG )
			iIndex = iID - (WIID_CASHSHOP_SULT_ITEM_SLOT_IMAGE+1);
		else if( iID < WIID_CASHSHOP_SULT_ITEM_IMAGE )
			iIndex = iID - (WIID_CASHSHOP_SULT_ITEM_IMAGE_BG+1);
		else
			iIndex = iID - (WIID_CASHSHOP_SULT_ITEM_IMAGE+1);

		pkCashItemAttr = g_pItemCluster->GetCaseListItem(iIndex);
		if( pkCashItemAttr == NULL )
			return 1;
	}
	else
	{
		if( iID < WIID_CASHSHOP_STORE_ITEM_IMAGE_BG )
			iIndex = iID - WIID_CASHSHOP_STORE_ITEM_SLOT_IMAGE;
		else if( iID < WIID_CASHSHOP_STORE_ITEM_IMAGE )
			iIndex = iID -WIID_CASHSHOP_STORE_ITEM_IMAGE_BG;
		else
			iIndex = iID - WIID_CASHSHOP_STORE_ITEM_IMAGE;

		pkCashItemAttr = GetStoreCashItemAttr(iIndex);
		if( pkCashItemAttr == NULL )
			return 1;
	}

	if( pkCashItemAttr->iGender )
	{
		if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
			return 1;

		int iCheckValue = (int)g_pGOBManager->GetLocalPlayer()->GetCheckStatusValue(CHECK_STATUS_GENDER);
		if( pkCashItemAttr->iGender != iCheckValue )
			return 1;
	}

	//if( pkCashItemAttr->eCategory == CI_PACKAGE )
	//	ResetPlayerLayer();

	//[xialin 2008/08/04] edit >>>
	//for( int i = 0; i < 8; i++ )
	//{
	//	if( pkCashItemAttr->ItemInfo[i].iID == 0 )
	//		break;

	//	if( IsEquipPlayerLayer(pkCashItemAttr->ItemInfo[i].iID) )
	//		SetEquipPlayerLayer(pkCashItemAttr->ItemInfo[i].iID);
	//	else if( IsPetLayer(pkCashItemAttr->ItemInfo[i].iID) )
	//	{
	//		if( pkCashItemAttr->iClassID )
	//			SetPet(pkCashItemAttr->iClassID, pkCashItemAttr->ItemInfo[i].iID);
	//	}

	//	if( pkCashItemAttr->iMotionID )
	//		SetAniSeq(pkCashItemAttr->iMotionID, false);
	//}

	SetEquipPlayerLayer(pkCashItemAttr);
	//[xialin 2008/08/04] edit <<<

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreLButtonDBL(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iIndex = 0;
	if( iID < WIID_CASHSHOP_STORE_ITEM_IMAGE )
		iIndex = iID - WIID_CASHSHOP_STORE_ITEM_SLOT_IMAGE;
	else if( iID < WIID_CASHSHOP_STORE_ITEM_IMAGE_BG )
		iIndex = iID - WIID_CASHSHOP_STORE_ITEM_IMAGE;
	else
		iIndex = iID - WIID_CASHSHOP_STORE_ITEM_IMAGE_BG;

	SPCashItemAttr* pkCashItemAttr = GetStoreCashItemAttr(iIndex);
	if( pkCashItemAttr == NULL )
		return 1;
	
	//InsertPreViewInven(pkCashItemAttr);    //[xialin 2008/07/10] 去掉购物车
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreBuy(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	if ( iID == WIID_CASHSHOP_SULT_ITEM_BUY )
	{
		ShowCashierDlg(m_pCurSultItemAttr);
	}
	else if ( iID >= WIID_CASHSHOP_SULT_ITEM_BUY+1 && iID <= WIID_CASHSHOP_SULT_ITEM_BUY+5 )
	{
		int iIndex = iID - (WIID_CASHSHOP_SULT_ITEM_BUY+1);
		ShowCashierDlg(g_pItemCluster->GetCaseListItem(iIndex));
	}
	else
	{
		int iIndex = iID - WIID_CASHSHOP_STORE_ITEM_BUY;
		ShowCashierDlg(GetStoreCashItemAttr(iIndex));
	}
	return 1;
}
//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCashSortBuy( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	int iIndex = iID - WIID_CASHSHOP_SORT_ICON;

	SPHotItemManager* pHotManager = SPCashTradeManager::GetInstance()->GetHotItemManager();
	if(pHotManager == NULL)
		return 0;

	if ( iIndex >= pHotManager->GetHotItemSize() )
		return 0;

	SPCashItemAttr* pkCashItemAttr = pHotManager->GetHotItem( iIndex )->GetCashItemAttr();
	if( NULL == pkCashItemAttr )
	{
		return 0;
	}
	ShowCashierDlg( pkCashItemAttr );
	return 1;
}
//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreGift(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	if ( iID == WIID_CASHSHOP_SULT_ITEM_GIFT )
	{
		ShowGiftDlg(m_pCurSultItemAttr);
	}
	else if ( iID >= WIID_CASHSHOP_SULT_ITEM_GIFT+1 && iID <= WIID_CASHSHOP_SULT_ITEM_GIFT+5 )
	{
		int iIndex = iID - (WIID_CASHSHOP_SULT_ITEM_GIFT+1);
		ShowGiftDlg(g_pItemCluster->GetCaseListItem(iIndex));
	}
	else
	{
		int iIndex = iID - WIID_CASHSHOP_STORE_ITEM_GIFT;
		ShowGiftDlg(GetStoreCashItemAttr(iIndex));
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreExact(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	if ( iID == WIID_CASHSHOP_SULT_ITEM_EXACT )
	{
		ShowExactDlg(m_pCurSultItemAttr);
	}
	else if ( iID >= WIID_CASHSHOP_SULT_ITEM_EXACT+1 && iID <= WIID_CASHSHOP_SULT_ITEM_EXACT+5 )
	{
		int iIndex = iID - (WIID_CASHSHOP_SULT_ITEM_EXACT+1);
		ShowExactDlg(g_pItemCluster->GetCaseListItem(iIndex));
	}
	else
	{
		int iIndex = iID - WIID_CASHSHOP_STORE_ITEM_EXACT;
		ShowExactDlg(GetStoreCashItemAttr(iIndex));
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreCart(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	//if( (int)m_vCartList.size() >= CASHSHOP_MAX_WISH )
	//{
	//	SetErrorDlg(9005003);
	//	return 1;
	//}

	//if( g_pCheckManager->IsDBBlock() )
	//	return 1;

	//int iIndex = iID - WIID_CASHSHOP_STORE_ITEM_CART;
	//SPCashItemAttr* pkCashItemAttr = GetStoreCashItemAttr(iIndex);
	//if( pkCashItemAttr == NULL )
	//	return 1;

	//vector< unsigned int >::iterator iter = m_vCartList.begin();
	//while( iter != m_vCartList.end() )
	//{
	//	if( (*iter) == pkCashItemAttr->iCashID )
	//	{
	//		SetErrorDlg(9005001);
	//		return 1;
	//	}

	//	++iter;
	//}

	//CPacket kPacket(CASHSHOP_CS_WISH_ADD);
	//kPacket.AddUINT32((unsigned int)pkCashItemAttr->iCashID);
	//(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
	//g_pCheckManager->SetDBBlock(true);

	//SPCashTradeManager::GetInstance()->SendWishAdd(CASH_TRADE_PLACE_SHOP, pkCashItemAttr->iCashID);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreCartDel(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	//int iIndex = iID - WIID_CASHSHOP_STORE_ITEM_CART_DEL;
	//SPCashItemAttr* pkCashItemAttr = GetStoreCashItemAttr(iIndex);
	//if( pkCashItemAttr == NULL )
	//	return 1;

	//if( g_pCheckManager->IsDBBlock() )
	//	return 1;

	//CPacket kPacket(CASHSHOP_CS_WISH_DEL);
	//kPacket.AddUINT32((unsigned int)pkCashItemAttr->iCashID);
	//(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
	//g_pCheckManager->SetDBBlock(true);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCashierPlus(WPARAM wParam, LPARAM lParam)
{
	if( m_vCashierItemAttr.size() <= 0 || m_vCashierItemAttr.back() == NULL )
		return 1;

	if( m_iMileageGlowUnit <= 0 )
		return 1;

	int iMileage = m_iCashierMileage + m_iMileageGlowUnit;
	int iOZ = m_iCashierOZ - m_iMileageGlowUnit;

	if( iMileage > m_iCashierMileageRange[1] )
		return 1;

	if( iMileage > (int)m_uiCurMileage )
		return 1;

	if( iOZ + iMileage < m_iCahsierPrice )
		iOZ = m_iCahsierPrice - iMileage;

	CalcCashierMoney(iOZ, iMileage);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCashierMinus(WPARAM wParam, LPARAM lParam)
{
	if( m_vCashierItemAttr.size() <= 0 || m_vCashierItemAttr.back() == NULL )
		return 1;
	
	if( m_iMileageGlowUnit <= 0 )
		return 1;

	int iMileage = m_iCashierMileage - m_iMileageGlowUnit;
	int iOZ = m_iCashierOZ + m_iMileageGlowUnit;

	if( iMileage < m_iCashierMileageRange[0] )
		return 1;

	CalcCashierMoney(iOZ, iMileage);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCashierOK(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_CASHIER);
	if( pkWindow )
		pkWindow->Hide();

	g_pInterfaceManager->SetModalWindow(NULL);

	Buy();
		
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCashierCancel(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_CASHIER);
	if( pkWindow )
		pkWindow->Hide();

	g_pInterfaceManager->SetModalWindow(NULL);
	m_vCashierItemAttr.clear();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCashierMileageChange(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkParentWindow = Find(WIID_CASHSHOP_CASHIER);
	if( pkParentWindow == NULL )
		return 1;

	SPWindow* pkWindow = pkParentWindow->Find(WIID_CASHSHOP_CASHIER_USE_MILE, true);
	if( pkWindow == NULL )
		return 1;

	int iMileage = 0;
	const char* pszText = pkWindow->GetWindowText();
	if( pszText != NULL && strlen(pszText) )
		iMileage = atoi(pszText);

	if( m_iMileageGlowUnit > 0 )
		iMileage = (iMileage / m_iMileageGlowUnit) * m_iMileageGlowUnit;

	if( iMileage < m_iCashierMileageRange[0] )
		iMileage = m_iCashierMileageRange[0];
	else if( iMileage > m_iCashierMileageRange[1] )
		iMileage = m_iCashierMileageRange[1];

	CalcCashierMoney(m_iCahsierPrice - iMileage, iMileage);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnGiftPlus(WPARAM wParam, LPARAM lParam)
{
	if( m_vCashierItemAttr.size() <= 0 || m_vCashierItemAttr.back() == NULL )
		return 1;

	if( m_iMileageGlowUnit <= 0 )
		return 1;

	int iMileage = m_iCashierMileage + m_iMileageGlowUnit;
	int iOZ = m_iCashierOZ - m_iMileageGlowUnit;

	if( iMileage > m_iCashierMileageRange[1] )
		return 1;

	if( iMileage > (int)m_uiCurMileage )
		return 1;

	CalcGiftMoney(iOZ, iMileage);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnGiftMinus(WPARAM wParam, LPARAM lParam)
{
	if( m_vCashierItemAttr.size() <= 0 || m_vCashierItemAttr.back() == NULL )
		return 1;

	if( m_iMileageGlowUnit <= 0 )
		return 1;

	int iMileage = m_iCashierMileage - m_iMileageGlowUnit;
	int iOZ = m_iCashierOZ + m_iMileageGlowUnit;

	if( iMileage < m_iCashierMileageRange[0] )
		return 1;

	CalcGiftMoney(iOZ, iMileage);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnGiftFriendListLButton(WPARAM wParam, LPARAM lParam)
{
	TREEBOX_ITEM* pSelectItem =	(TREEBOX_ITEM*)wParam;
	if( pSelectItem == NULL )
		return 1;

	if( pSelectItem->m_pItemBase )
	{
		SPWindow* pkWindow = Find(WIID_CASHSHOP_GIFT_RECEIVE, true);
		if( pkWindow )
			pkWindow->SetWindowText(pSelectItem->m_pItemBase->GetText());

		pkWindow = Find(WIID_CASHSHOP_GIFT_FRIEND, true);
		if( pkWindow )
			pkWindow->Hide();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnGiftFriendListDBLButton(WPARAM wParam, LPARAM lParam)
{
	TREEBOX_ITEM* pSelectItem =	(TREEBOX_ITEM*)wParam;
	if( pSelectItem == NULL )
		return 1;

	if( pSelectItem->m_pItemBase )
	{
		SPWindow* pkWindow = Find(WIID_CASHSHOP_GIFT_RECEIVE, true);
		if( pkWindow )
			pkWindow->SetWindowText(pSelectItem->m_pItemBase->GetText());

		pkWindow = Find(WIID_CASHSHOP_GIFT_FRIEND, true);
		if( pkWindow )
			pkWindow->Hide();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactFriendListLButton(WPARAM wParam, LPARAM lParam)
{
	TREEBOX_ITEM* pSelectItem =	(TREEBOX_ITEM*)wParam;
	if( pSelectItem == NULL )
		return 1;

	if( pSelectItem->m_pItemBase )
	{
		SPWindow* pkWindow = Find(WIID_CASHSHOP_EXACT_RECEIVE, true);
		if( pkWindow )
			pkWindow->SetWindowText(pSelectItem->m_pItemBase->GetText());

		pkWindow = Find(WIID_CASHSHOP_EXACT_FRIEND, true);
		if( pkWindow )
			pkWindow->Hide();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactFriendListDBLButton(WPARAM wParam, LPARAM lParam)
{
	TREEBOX_ITEM* pSelectItem =	(TREEBOX_ITEM*)wParam;
	if( pSelectItem == NULL )
		return 1;

	if( pSelectItem->m_pItemBase )
	{
		SPWindow* pkWindow = Find(WIID_CASHSHOP_EXACT_RECEIVE, true);
		if( pkWindow )
			pkWindow->SetWindowText(pSelectItem->m_pItemBase->GetText());

		pkWindow = Find(WIID_CASHSHOP_EXACT_FRIEND, true);
		if( pkWindow )
			pkWindow->Hide();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactListFriendListLButton(WPARAM wParam, LPARAM lParam)
{
	TREEBOX_ITEM* pSelectItem =	(TREEBOX_ITEM*)wParam;
	if( pSelectItem == NULL )
		return 1;

	if( pSelectItem->m_pItemBase )
	{
		SPWindow* pkWindow = Find(WIID_CASHSHOP_EXACTLIST_RECEIVE, true);
		if( pkWindow )
			pkWindow->SetWindowText(pSelectItem->m_pItemBase->GetText());

		pkWindow = Find(WIID_CASHSHOP_EXACTLIST_FRIEND, true);
		if( pkWindow )
			pkWindow->Hide();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactListFriendListDBLButton(WPARAM wParam, LPARAM lParam)
{
	TREEBOX_ITEM* pSelectItem =	(TREEBOX_ITEM*)wParam;
	if( pSelectItem == NULL )
		return 1;

	if( pSelectItem->m_pItemBase )
	{
		SPWindow* pkWindow = Find(WIID_CASHSHOP_EXACTLIST_RECEIVE, true);
		if( pkWindow )
			pkWindow->SetWindowText(pSelectItem->m_pItemBase->GetText());

		pkWindow = Find(WIID_CASHSHOP_EXACTLIST_FRIEND, true);
		if( pkWindow )
			pkWindow->Hide();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnGiftOK(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_GIFT);
	if( pkWindow == NULL )
		return 1;

	m_strGiftFriendName.clear();
	m_strGiftFriendMsg.clear();

	const char* pszString = NULL;
	SPWindow* pkChildWindow;
	pkChildWindow = pkWindow->Find(WIID_CASHSHOP_GIFT_RECEIVE, true);
	if( pkChildWindow )
		pszString = pkChildWindow->GetWindowText();

	if( pszString )
	{
		m_strGiftFriendName = pszString;

		pkChildWindow = pkWindow->Find(WIID_CASHSHOP_GIFT_MESSAGE, true);
		if( pkChildWindow )
			pszString = pkChildWindow->GetWindowText();

		if( pszString )
			m_strGiftFriendMsg = pszString;
	}

	pkWindow->Hide();
	g_pInterfaceManager->SetModalWindow(NULL);

	Gift();
	return 1;
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::Gift()
{
	if( g_pCheckManager->IsDBBlock() )
		return false;

	bool bSelfGiftCheck = false, bSendPacket = false;
	if( m_vCashierItemAttr.size() > 0 && m_vCashierItemAttr.back() != NULL )
	{
		char szBuf[64];
		if( g_pGOBManager->GetLocalPlayer() )
		{
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szBuf);
			if( strcmp(m_strGiftFriendName.c_str(), szBuf) == 0 )
				bSelfGiftCheck = true;
		}

		if( !bSelfGiftCheck )
		{
			int iPrice = m_vCashierItemAttr.back()->iSalePrice ? m_vCashierItemAttr.back()->iSalePrice : m_vCashierItemAttr.back()->iPrice;

			int iOZandMileage = 0;
			if ( m_iCashierOZ == 0 && m_iCashierMileage == 0 )
			{
				iOZandMileage = m_uiCurOZ;
			}
			else
			{
				iOZandMileage = m_uiCurOZ + m_uiCurMileage;
			}

			if( iOZandMileage < iPrice )
				OnMyInfoFilling(0, 0);
			else
			{
				int iCashierPrice = 0, iCashierMileage = 0;
				if ( m_iCashierOZ == 0 && m_iCashierMileage == 0 )
				{
					iCashierPrice = iPrice;
					iCashierMileage = 0;
				}
				else
				{
					iCashierPrice = m_iCashierOZ;
					iCashierMileage = m_iCashierMileage;
				}

				if( SPCashTradeManager::GetInstance()->SendCashShopPresent(CASH_TRADE_PLACE_SHOP, 
					m_vCashierItemAttr.back()->iCashID, m_vCashierItemAttr.back()->iPrice, 
					m_vCashierItemAttr.back()->iSalePrice, m_vCashierItemAttr.back()->iMaxOZ, m_vCashierItemAttr.back()->iMaxMile, 
					m_strGiftFriendName, m_strGiftFriendMsg, 
					iCashierPrice, iCashierMileage) == true )
				{
					m_vCashierItemAttr.pop_back();
					m_bBuyRecv = false;
					bSendPacket = true;
				}

				//m_strGiftFriendName.resize(LEN_NAME);
				//m_strGiftFriendMsg.resize(MAX_MAIl_BODY_LEN);
				//CPacket kPacket(CASHSHOP_CS_BUY_PRESENT);
				//
				//kPacket.Add(const_cast<char *>(m_strGiftFriendName.c_str()), m_strGiftFriendName.size());
				//kPacket.AddUINT32(m_vCashierItemAttr.back()->iCashID);
				//kPacket.AddUINT32(m_vCashierItemAttr.back()->iPrice);
				//kPacket.AddUINT32(m_vCashierItemAttr.back()->iSalePrice);
				//kPacket.AddUINT32(m_vCashierItemAttr.back()->iMaxOZ);
				//kPacket.AddUINT32(m_vCashierItemAttr.back()->iMaxMile);

				//if ( m_iCashierOZ == 0 && m_iCashierMileage == 0 )
				//{
				//	kPacket.AddUINT32(iPrice);
				//	kPacket.AddUINT32(0);
				//}
				//else
				//{
				//	kPacket.AddUINT32(m_iCashierOZ);
				//	kPacket.AddUINT32(m_iCashierMileage);
				//}
				//kPacket.Add(const_cast<char *>(m_strGiftFriendMsg.c_str()), m_strGiftFriendMsg.size());

				//(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
				//g_pCheckManager->SetDBBlock(true);
				//m_vCashierItemAttr.pop_back();
				//m_bBuyRecv = false;
				//bSendPacket = true;
			}
		}
	}
	
	if( bSelfGiftCheck )
	{
		SetErrorDlg(9004004);
		return false;
	}
	else if( bSendPacket )
	{
		ShowBuyWaitDlg();
		return true;
	}

	m_vCashierItemAttr.clear();
	return false;

}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnGiftCancel(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_GIFT);
	if( pkWindow )
		pkWindow->Hide();

	g_pInterfaceManager->SetModalWindow(NULL);
	m_vCashierItemAttr.clear();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnGiftFriend(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_GIFT_FRIEND, true);
	if( pkWindow )
	{
		if( pkWindow->IsShow() )
			pkWindow->Hide();
		else
			pkWindow->Show();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnGiftMileageChange(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkParentWindow = Find(WIID_CASHSHOP_GIFT);
	if( pkParentWindow == NULL )
		return 1;

	SPWindow* pkWindow = pkParentWindow->Find(WIID_CASHSHOP_GIFT_USE_MILE, true);
	if( pkWindow == NULL )
		return 1;

	int iMileage = 0;
	const char* pszText = pkWindow->GetWindowText();
	if( pszText != NULL && strlen(pszText) )
		iMileage = atoi(pszText);

	if( m_iMileageGlowUnit > 0 )
		iMileage = (iMileage / m_iMileageGlowUnit) * m_iMileageGlowUnit;

	if( iMileage < m_iCashierMileageRange[0] )
		iMileage = m_iCashierMileageRange[0];
	else if( iMileage > m_iCashierMileageRange[1] )
		iMileage = m_iCashierMileageRange[1];

	CalcGiftMoney(m_iCahsierPrice - iMileage, iMileage);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactOK(WPARAM wParam, LPARAM lParam)
{
	if( g_pCheckManager->IsDBBlock() )
		return 1;

	SPWindow* pkWindow = Find(WIID_CASHSHOP_EXACT);
	if( pkWindow == NULL )
		return 1;

	bool bSelfExactCheck = false, bSendPacket =false;
	if( m_vCashierItemAttr.size() > 0 && m_vCashierItemAttr.back() != NULL )
	{
		const char* pszString = NULL;
		SPWindow* pkChildWindow;
		pkChildWindow = pkWindow->Find(WIID_CASHSHOP_EXACT_RECEIVE, true);
		if( pkChildWindow )
			pszString = pkChildWindow->GetWindowText();

		if( pszString )
		{
			char szTargetName[LEN_NAME], szComment[MAX_MAIl_BODY_LEN];
			memset(szTargetName, 0, sizeof(szTargetName));
			memset(szComment, 0, sizeof(szComment));
			strncpy(szTargetName, pszString, LEN_NAME);
			
			pkChildWindow = pkWindow->Find(WIID_CASHSHOP_EXACT_MESSAGE, true);
			if( pkChildWindow )
				pszString = pkChildWindow->GetWindowText();

			if( pszString )
				strncpy(szComment, pszString, MAX_MAIl_BODY_LEN);

			char szBuf[64];
			if( g_pGOBManager->GetLocalPlayer() )
			{
				g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szBuf);
				if( strcmp(szTargetName, szBuf) == 0 )      //如果是自己不处理
					bSelfExactCheck = true;
			}

			pkWindow->Hide();
			g_pInterfaceManager->SetModalWindow(NULL);

			if( !bSelfExactCheck )
			{
				CPacket kPacket(CASHSHOP_CS_ASK_BUY);
				
				kPacket.AddUINT8(1);
				kPacket.AddUINT32(m_vCashierItemAttr.back()->iCashID);
				kPacket.AddUINT8(sizeof(szComment));
				kPacket.Add(szComment, sizeof(szComment));
				kPacket.AddUINT8(sizeof(szTargetName));
				kPacket.Add(szTargetName, sizeof(szTargetName));
				
				(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
				//g_pCheckManager->SetDBBlock(true);
				//m_bBuyRecv = false;
				//bSendPacket = true;

				//CPacket pkPacket;
				//pkPacket.AddUINT8(1);
				//pkPacket.AddUINT32(m_vCashierItemAttr.back()->iCashID);
				//pkPacket.AddUINT8(sizeof(szTargetName));
				//pkPacket.Add(szTargetName, sizeof(szTargetName));
				//pkPacket.AddUINT8(sizeof(szComment));
				//pkPacket.Add(szComment, sizeof(szComment));
				//g_pInterfaceManager->SPChildWndSendMessage(WIID_BEEXACT, SPIM_BEEXACT_MESSAGE, (WPARAM)&pkPacket, 0);
			}
		}
	}

	m_vCashierItemAttr.clear();
	
	if( bSelfExactCheck )
	{
		SetErrorDlg(9004004);
	}
	else if( bSendPacket )
	{
		//ShowBuyWaitDlg();
	}
		
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactCancel(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_EXACT);
	if( pkWindow )
		pkWindow->Hide();

	g_pInterfaceManager->SetModalWindow(NULL);
	m_vCashierItemAttr.clear();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactFriend(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_EXACT_FRIEND, true);
	if( pkWindow )
	{
		if( pkWindow->IsShow() )
			pkWindow->Hide();
		else
			pkWindow->Show();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSearch(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_STORE_SEARCH_EDIT, true);
	if( pkWindow && g_pItemCluster )
	{
		g_pItemCluster->SetSearchKey(pkWindow->GetWindowText());
		SetStoreCategory(0);
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreSultItem(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = uiID - WIID_CASHSHOP_STORE_ITEM_SKIN_W;
	SPCashItemAttr* pkCashItemAttr = GetStoreCashItemAttr(iSlotIndex);
	if ( RefreshSultListView(pkCashItemAttr) )
	{
		m_pCurSultItemAttr = pkCashItemAttr;
		int iItemIdxX = (iSlotIndex+1)%2;
		SPWindow * pkWindowItem = Find(WIID_CASHSHOP_SULT_ITEM, true);
		SPWindow * pkWindowSrcItem = Find(WIID_CASHSHOP_STORE_ITEM+iSlotIndex, true);
		SPWindow * pkWindowBack = Find(WIID_CASHSHOP_SULT_ITEM_BACK, true);
		if ( pkWindowItem && pkWindowSrcItem && pkWindowBack )
		{
			int ix, iy;
			pkWindowBack->GetRelCoord(ix, iy);
			pkWindowBack->SetWindowRelPos(iItemIdxX*238, iy);
			pkWindowBack->RefreshChildPos();

			pkWindowSrcItem->GetRelCoord(ix, iy);
			pkWindowItem->SetWindowRelPos(ix-2, iy);
			pkWindowItem->RefreshChildPos();
		}
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExitSultView(WPARAM wParam, LPARAM lParam)
{
	SPWindow * pkParentWindow = Find(WIID_CASHSHOP_SULT);
	if( pkParentWindow )
	{
		m_pCurSultItemAttr = NULL;
		pkParentWindow->Hide();
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreToolTipEnter(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	if ( uiID == WIID_CASHSHOP_SULT_ITEM_IMAGE )
	{
		SetStoreToolTipEnter(m_pCurSultItemAttr, lParam);
	}
	else if ( uiID >= WIID_CASHSHOP_SULT_ITEM_IMAGE+1 && uiID <= WIID_CASHSHOP_SULT_ITEM_IMAGE + 5 )
	{
		int iSlotIndex = uiID - (WIID_CASHSHOP_SULT_ITEM_IMAGE+1);
		SPCashItemAttr* pkCashItemAttr = g_pItemCluster->GetCaseListItem(iSlotIndex);
		SetStoreToolTipEnter(pkCashItemAttr, lParam);
	}
	else
	{
		int iSlotIndex = uiID - WIID_CASHSHOP_STORE_ITEM_IMAGE;
		SPCashItemAttr* pkCashItemAttr = GetStoreCashItemAttr(iSlotIndex);
		SetStoreToolTipEnter(pkCashItemAttr, lParam);
	}
	
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreToolTipOut(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, lParam);
	pkWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnStoreToolTipMove(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	if ( uiID == WIID_CASHSHOP_SULT_ITEM_IMAGE )
	{
		SetStoreToolTipMove(m_pCurSultItemAttr, lParam);
	}
	else if ( uiID >= WIID_CASHSHOP_SULT_ITEM_IMAGE+1 && uiID <= WIID_CASHSHOP_SULT_ITEM_IMAGE + 5 )
	{
		int iSlotIndex = uiID - (WIID_CASHSHOP_SULT_ITEM_IMAGE+1);
		SPCashItemAttr* pkCashItemAttr = g_pItemCluster->GetCaseListItem(iSlotIndex);
		SetStoreToolTipMove(pkCashItemAttr, lParam);
	}
	else
	{
		int iSlotIndex = uiID - WIID_CASHSHOP_STORE_ITEM_IMAGE;
		SPCashItemAttr* pkCashItemAttr = GetStoreCashItemAttr(iSlotIndex);
		SetStoreToolTipMove(pkCashItemAttr, lParam);
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCashSortToolTipEnter( unsigned int uiID, WPARAM wParam, LPARAM lParam )
{
	int iIndex = uiID - WIID_CASHSHOP_SORT_ICON;
	
	SPHotItemManager* pHotManager = SPCashTradeManager::GetInstance()->GetHotItemManager();
	if(pHotManager == NULL)
		return 0;

	if ( iIndex >= pHotManager->GetHotItemSize() )
		return 0;

	SPCashItemAttr* pkCashItemAttr = pHotManager->GetHotItem( iIndex )->GetCashItemAttr();

    if( NULL == pkCashItemAttr )
	{
		return 0;
	}
	SetStoreToolTipEnter( pkCashItemAttr, lParam );
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCashSortToolTipOut( unsigned int uiID, WPARAM wParam, LPARAM lParam )
{
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, lParam);
	pkWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCashSortToolTipMove( unsigned int uiID, WPARAM wParam, LPARAM lParam )
{
	int iIndex = uiID - WIID_CASHSHOP_SORT_ICON;
	
	SPHotItemManager* pHotManager = SPCashTradeManager::GetInstance()->GetHotItemManager();
	if(pHotManager == NULL)
		return 0;

	if ( iIndex >= pHotManager->GetHotItemSize() )
		return 0;

	SPCashItemAttr* pkCashItemAttr = pHotManager->GetHotItem( iIndex )->GetCashItemAttr();

	if( NULL == pkCashItemAttr )
	{
		return 0;
	}
	SetStoreToolTipMove( pkCashItemAttr, lParam );
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCashierToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	if( m_vCashierItemAttr.size() > 0 && m_vCashierItemAttr.back() != NULL )
		SetStoreToolTipEnter(m_vCashierItemAttr.back(), lParam);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCashierToolTipOut(WPARAM wParam, LPARAM lParam)
{
	SetStoreToolTipOut(lParam);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCashierToolTipMove(WPARAM wParam, LPARAM lParam)
{
	if( m_vCashierItemAttr.size() > 0 && m_vCashierItemAttr.back() != NULL )
		SetStoreToolTipMove(m_vCashierItemAttr.back(), lParam);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnResultToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	if( m_pkResultItemAttr )
		SetStoreToolTipEnter(m_pkResultItemAttr, lParam);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnResultToolTipOut(WPARAM wParam, LPARAM lParam)
{
	SetStoreToolTipOut(lParam);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnResultToolTipMove(WPARAM wParam, LPARAM lParam)
{
	if( m_pkResultItemAttr )
		SetStoreToolTipMove(m_pkResultItemAttr, lParam);

	return 1;
}
