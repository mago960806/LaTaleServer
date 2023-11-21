
#include "SPCommon.h"

#include "SPJoyPadManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowLooting.h"

#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
//#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee

#include "SPGameObject.h"
#include "SPGOBManager.h"	   

#include "SPGOBCoordPhysics.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatusModel.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"

#include "SPCheckManager.h"
#include "SPLocalizeManager.h"

using namespace std;

//------------------------------------------------------------------------------------
SPWindowLooting::SPWindowLooting(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowLooting::SPWindowLooting(INSTANCE_ID InstanceID,
								 int iX, int iY, int iCX, int iCY, SPWindow* pParent)
								 : SPWindow(WNDID_LOOTING, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowLooting::~SPWindowLooting()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowLooting::Clean()
{
	SPWindow::Clean();
}


//------------------------------------------------------------------------------------
void SPWindowLooting::Init()
{
	SPWindowStatic* pkSkinWindow, *pkChildWindow, *pkChildWindow2;
	SPWindowButton* pkButtonWindow;
	int i;

	// upper
	pkSkinWindow = new SPWindowStatic(WIID_LOOTING_SKIN_UPPER, 0, 0, 210, 4, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 46);

	// lower
	pkSkinWindow = new SPWindowStatic(WIID_LOOTING_SKIN_LOWER, 0, 261, 210, 4, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 56);

	// middle
	pkSkinWindow = new SPWindowStatic(WIID_LOOTING_SKIN_MIDDLE, 0, 4, 210, 257, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 52);
	pkSkinWindow->SetSrcSize(210, 2);

	pkChildWindow = new SPWindowStatic(WIID_LOOTING_SUB_BG, 2, 23, 206, 178, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 486);
	pkChildWindow->SetSrcSize(6, 6);

	pkButtonWindow = new SPWindowButton(WIID_LOOTING_PAGE_PREV, 70, 164, 7, 10, pkChildWindow);
	pkButtonWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 237, 378);
	pkButtonWindow->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 245, 378);
	pkButtonWindow->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 253, 378);
	pkButtonWindow->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 261, 378);

	pkButtonWindow = new SPWindowButton(WIID_LOOTING_PAGE_NEXT, 129, 164, 7, 10, pkChildWindow);
	pkButtonWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 269, 378);
	pkButtonWindow->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 277, 378);
	pkButtonWindow->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 286, 378);
	pkButtonWindow->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 293, 378);

	pkChildWindow = new SPWindowStatic(WIID_LOOTING_TITLE_TEXT, 10, 3, 121, 15, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 117, 360);

	pkChildWindow = new SPWindowStatic(WIID_LOOTING_SEPARATE_LINEUP, 2, 22, 206, 1, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 494);
	pkChildWindow->SetSrcSize(6, 1);

	pkChildWindow = new SPWindowStatic(WIID_LOOTING_SEPARATE_LINEDOWN, 2, 201, 206, 1, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 494);
	pkChildWindow->SetSrcSize(6, 1);

	pkChildWindow = new SPWindowStatic(WIID_LOOTING_DESCRIPTION, 10, 206, 190, 49, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 269, 390);

	pkChildWindow = new SPWindowStatic(WIID_LOOTING_PAGE_BG, 81, 185, 48, 14, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 226, 345);

	pkChildWindow2 = new SPWindowStatic(WIID_LOOTING_PAGE_TEXT, 0, 2, 48, 12, pkChildWindow);
	pkChildWindow2->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	pkButtonWindow = new SPWindowButton(WIID_LOOTING_EXIT, 189, 5, 12, 12, pkSkinWindow);
	pkButtonWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 301, 321);
	pkButtonWindow->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 314, 321);
	pkButtonWindow->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 327, 321);
	pkButtonWindow->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 340, 321);
	
	for( i = 0; i < 4; i++ )
	{
		pkChildWindow = new SPWindowStatic(WIID_LOOTING_ITEM_WHITE_BG + i, 2, 23 + i * 40, 206, 40, pkSkinWindow);
		pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 477);
		pkChildWindow->SetSrcSize(6, 6);

		pkChildWindow = new SPWindowStatic(WIID_LOOTING_ITEM_SEL_BG + i, 10, 27 + i * 40, 190, 32, pkSkinWindow);
		pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 321, 146);

		pkChildWindow = new SPWindowStatic(WIID_LOOTING_ITEM_SLOT_IMAGE + i, 12, 27 + i * 40, 32, 32, pkSkinWindow);
		pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 408);
		pkChildWindow = new SPWindowStatic(WIID_LOOTING_ITEM_SLOT + i, 12, 27 + i * 40, 32, 32, pkSkinWindow);

		pkChildWindow = new SPWindowStatic(WIID_LOOTING_ITEM_EMPTY + i, 101, 39 + i * 40, 36, 7, pkSkinWindow);
		pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 336, 382);

		pkChildWindow = new SPWindowStatic(WIID_LOOTING_ITEM_TEXT + i, 51, 37 + i * 40, 140, 12, pkSkinWindow);
		pkChildWindow->SetImage((SPTexture*)NULL, 0, 0);
		pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 374, 245);
		pkChildWindow->SetFormat(DT_VCENTER | DT_LEFT);
		pkChildWindow->SetFontColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

		SPWindow* pkParentWindow;
		if( pkParentWindow = Find(WIID_LOOTING_SUB_BG, true) )
			pkButtonWindow = new SPWindowButton(WIID_LOOTING_ITEM_BTN + i, 0, i * 40, 206, 40, pkParentWindow);
	}

	m_iCurPage = 0;
	m_iMaxPage = 0;
	m_iValidItemNum = 0;
}

//------------------------------------------------------------------------------------
void SPWindowLooting::Process(float fTime)
{
	SPWindow::Process(fTime);
	if( IsShow() ) {
		ProcessHotKey();		
#ifdef _JOY_PAD
		if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_JOYPAD)) {
			ProcessJoyPad(fTime);
		}
#endif		
	}
}

//------------------------------------------------------------------------------------
void SPWindowLooting::Render(float fTime)
{
	SPWindow::Render(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowLooting::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);
	for( int i = 0; i < 4; i++ )
		SetEmptySlot(i);

	SetItemWindow();
	OnItemClick(WIID_LOOTING_ITEM_BTN + (m_iSelectItem % 4), 0, 0);
	
	
//
#ifdef _JOY_PAD
	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_JOYPAD)) {
		std::vector<SPJoyPadButtonInfo*>* pvButton = g_pInputManager->GetJoyPadManager()->GetPadButton(PAD_BUTTON_KEY_TYPE_QUICK);
		std::vector<SPJoyPadButtonInfo*>::iterator iter;
		iter = pvButton->begin();
		for(; iter != pvButton->end(); ++iter) {		
			if((*iter)->m_iKey == BUTTON_KEY_QUICK_11) {									//Space
				if((*iter)->m_iInputState == INPUT_REPEAT) {					
					m_bJoyPadSpacePush = false;
				}
			}
		}
	}
#endif		
}
//------------------------------------------------------------------------------------
void SPWindowLooting::Hide(bool bSendServer)
{
	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide(bSendServer);
	OnToolTipOut(0, 0, 0);
}


//------------------------------------------------------------------------------------
void SPWindowLooting::Clear()
{
	m_iCurPage = 0;
	m_iMaxPage = 0;
	m_iSelectItem = 0;
	m_iValidItemNum = 0;

	int i;
	for( i = 0; i < ITEM_MAX_NUM; i++ )
		m_ContainerItem[i].Item.iItemID = 0;

	ZeroMemory(m_bInventoryFull, sizeof(bool) * ITEM_MAX_NUM);
}

//------------------------------------------------------------------------------------
void SPWindowLooting::ProcessHotKey()
{
	SPInputStruct* pkInputStruct = g_pInputManager->GetInputStruct();

	if( pkInputStruct->ikArrowUp.InputState == INPUT_PRESS )
	{
		if( m_iSelectItem == 0 )
			return;

		if( m_iSelectItem == 4 )
			OnPagePrev(0, 0);
		else
			OnItemClick(WIID_LOOTING_ITEM_BTN + ((m_iSelectItem - 1) % 4), 0, 0);
	}
	else if( pkInputStruct->ikArrowDown.InputState == INPUT_PRESS )
	{
		if( m_iSelectItem == ITEM_MAX_NUM - 1 )
			return;

		if( m_iSelectItem == 3 )
			OnPageNext(0, 0);
		else
			OnItemClick(WIID_LOOTING_ITEM_BTN + ((m_iSelectItem + 1) % 4), 0, 0);
	}

	if( pkInputStruct->ikSpaceBar.InputState == INPUT_PRESS )
	{
		if( pkInputStruct->ikLShift.InputState == INPUT_PRESS_REPEAT )
			ItemAllGain();
		else
			ItemGain();
	}	
}


//------------------------------------------------------------------------------------
void SPWindowLooting::ProcessJoyPad(float fTime)
{
	if(m_bShow == false)
		return;

	if(!g_pInput->IsEquipJoystick()) 
		return;

	SPInputStructJoyStick* pStick = g_pInputManager->GetInputStructJoyStick();
	if(pStick == NULL)	return;

	if(g_pInputManager->GetJoyPadManager()->GetJoyStickMode() == JOYSTICK_MODE_ANALOG) {		
		if(m_bJoyPadUpPush == false && pStick->ibRgdwPOV[0].iValue == 0)			//Up
		{
			if( m_iSelectItem == 0 )
				return;

			if( m_iSelectItem == 4 )
				OnPagePrev(0, 0);
			else
				OnItemClick(WIID_LOOTING_ITEM_BTN + ((m_iSelectItem - 1) % 4), 0, 0);	

			m_bJoyPadUpPush = true;
		}		
		else if(m_bJoyPadDownPush == false && pStick->ibRgdwPOV[0].iValue == 18000)	//Down
		{
			if( m_iSelectItem == ITEM_MAX_NUM - 1 )
				return;

			if( m_iSelectItem == 3 )
				OnPageNext(0, 0);
			else
				OnItemClick(WIID_LOOTING_ITEM_BTN + ((m_iSelectItem + 1) % 4), 0, 0);

			m_bJoyPadDownPush = true;
		}
		else if((m_bJoyPadUpPush || m_bJoyPadDownPush) && (pStick->ibRgdwPOV[0].iValue != 0 && pStick->ibRgdwPOV[0].iValue != 18000)){
			m_bJoyPadUpPush = false;
			m_bJoyPadDownPush = false;
		}
		
	}	
	else if(g_pInputManager->GetJoyPadManager()->GetJoyStickMode() == JOYSTICK_MODE_DIGITAL){
		if(m_bJoyPadUpPush == false && pStick->ibLY.iValue == -1000)			//Up
		{
			if( m_iSelectItem == 0 )
				return;

			if( m_iSelectItem == 4 )
				OnPagePrev(0, 0);
			else
				OnItemClick(WIID_LOOTING_ITEM_BTN + ((m_iSelectItem - 1) % 4), 0, 0);	

			m_bJoyPadUpPush = true;
		}
		else if(m_bJoyPadDownPush == false && pStick->ibLY.iValue == 1000)		//Down
		{
			if( m_iSelectItem == ITEM_MAX_NUM - 1 )
				return;

			if( m_iSelectItem == 3 )
				OnPageNext(0, 0);
			else
				OnItemClick(WIID_LOOTING_ITEM_BTN + ((m_iSelectItem + 1) % 4), 0, 0);

			m_bJoyPadDownPush = true;
		}
		else if((m_bJoyPadUpPush || m_bJoyPadDownPush) && ((pStick->ibLY.iValue != -1000 && pStick->ibLY.iValue != 1000))){
			m_bJoyPadUpPush = false;
			m_bJoyPadDownPush = false;
		}
	}

	std::vector<SPJoyPadButtonInfo*>* pvButton = g_pInputManager->GetJoyPadManager()->GetPadButton(PAD_BUTTON_KEY_TYPE_QUICK);
	std::vector<SPJoyPadButtonInfo*>::iterator iter;
	iter = pvButton->begin();
	DWORD dwInputType =	0;
	m_bJoyPadSpacePush = false;
	for(; iter != pvButton->end(); ++iter) {		
		if((*iter)->m_iKey == BUTTON_KEY_QUICK_11) {									//Space
			////
			//if(m_bJoyPadSpacePush == false && (*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
			//	m_bJoyPadSpacePush = true;
			//	(*iter)->m_bPush == true;
			//}
			//else if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush == true/*&& m_bJoyPadSpacePush*/){
			//	m_bJoyPadSpacePush = false;
			//	(*iter)->m_bPush == false;
			//}

			if((*iter)->m_iInputState == INPUT_REPEAT) {
				dwInputType = INPUT_REPEAT;
			}
			else if((*iter)->m_iInputState ==  INPUT_UNREPEAT )
			{				
				(*iter)->m_bPush = false;
			}
			
			if((*iter)->m_iInputState == dwInputType) {
				if((*iter)->m_bPush == false)
					m_bJoyPadSpacePush = true;

				(*iter)->m_bPush == true;
			}
		}

		if((*iter)->m_iKey == BUTTON_KEY_QUICK_6) {										//Shift
			if(m_bJoyPadShiftPush == false && (*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
				m_bJoyPadShiftPush = true;
				(*iter)->m_bPush == true;
			}
			else if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush == true/*&& m_bJoyPadShiftPush*/){
				m_bJoyPadShiftPush = false;
				(*iter)->m_bPush == false;
			}
		}
	}

	if(m_bJoyPadShiftPush && m_bJoyPadSpacePush) {
		ItemAllGain();
	}
	else if(m_bJoyPadSpacePush) {
		ItemGain();
	}
}


//------------------------------------------------------------------------------------
void SPWindowLooting::SetEmptySlot(int iIndex)
{
	SPWindow* pkWindow;

	pkWindow = Find(WIID_LOOTING_ITEM_SLOT + iIndex, true);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);

	pkWindow = Find(WIID_LOOTING_ITEM_EMPTY + iIndex, true);
	if( pkWindow )
		pkWindow->Show();

	pkWindow = Find(WIID_LOOTING_ITEM_WHITE_BG + iIndex, true);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_LOOTING_ITEM_TEXT + iIndex, true);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_LOOTING_ITEM_SEL_BG + iIndex, true);
	if( pkWindow )
		pkWindow->Hide();
}

//------------------------------------------------------------------------------------
void SPWindowLooting::SetItemWindow()
{
	SPWindow* pkWindow;
	CONTAINER_ITEM ItemInfo;
	int i;

	for( i = 0; i < 4; i++ )
	{
		ItemInfo = m_ContainerItem[m_iCurPage * 4 + i];
		if( ItemInfo.Item.iItemID == 0 )
		{
			SetEmptySlot(i);
			continue;
		}
		
		pkWindow = Find(WIID_LOOTING_ITEM_WHITE_BG + i, true);
		if( pkWindow )
			pkWindow->Show();

		pkWindow = Find(WIID_LOOTING_ITEM_EMPTY + i, true);
		if( pkWindow )
			pkWindow->Hide();

		pkWindow = Find(WIID_LOOTING_ITEM_SLOT + i, true);
		if( pkWindow )
		{
			POINT ptSrcPos;
			SPTexture* pkTexture = g_pItemCluster->GetItemTexture(ItemInfo.Item.iItemID, ptSrcPos);

			pkWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);
			pkWindow->SetImage(pkTexture, ptSrcPos.x, ptSrcPos.y);
		}

		pkWindow = Find(WIID_LOOTING_ITEM_TEXT + i, true);
		if( pkWindow )
		{
			pkWindow->Show();

			SPItemAttr* pkItemAttr = NULL;
			if( g_pItemCluster->GetItemInfo(ItemInfo.Item.iItemID, pkItemAttr) )
			{
				if( m_bInventoryFull[m_iCurPage * 4 + i] )
				{
					pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 1);
					pkWindow->SetWindowText(NULL);
				}
				else
				{
					pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);

					// 아이템 컬러 우선순위 에 따른 출력상태 변경 [2/13/2006 AJJIYA]
					if( g_pItemCluster->IsItemNameColor( ItemInfo.Item.iItemID ) == true )
						pkWindow->SetFontColor( g_pItemCluster->GetItemNameColor( ItemInfo.Item.iItemID ) );
					else
						pkWindow->SetFontColor(g_pCheckManager->GetItemColor(ItemInfo.Item.iRare));

					pkWindow->SetWindowText(pkItemAttr->m_strName.c_str());
				}
			}
		}
	}

	SetPageText();
}

//------------------------------------------------------------------------------------
void SPWindowLooting::SetPageText()
{
	SPWindow* pkWindow;
	pkWindow = Find(WIID_LOOTING_PAGE_TEXT, true);

	if( pkWindow )
	{
		if( m_iMaxPage > 0 )
		{
			char szBuf[12];
			sprintf(szBuf, "%d/%d", m_iCurPage + 1, m_iMaxPage + 1);
			pkWindow->SetWindowText(szBuf);
		}
		else
			pkWindow->SetWindowText(NULL);

		pkWindow->Show();
	}
	
	bool bShow = true;
	if( m_iMaxPage == 0 )
		bShow = false;

	pkWindow = Find(WIID_LOOTING_PAGE_BG, true);
	if( pkWindow )
	{
		if( bShow )
			pkWindow->Show();
		else
			pkWindow->Hide();
	}

	pkWindow = Find(WIID_LOOTING_PAGE_PREV, true);
	if( pkWindow )
	{
		if( bShow )
			pkWindow->Show();
		else
			pkWindow->Hide();
	}

	pkWindow = Find(WIID_LOOTING_PAGE_NEXT, true);
	if( pkWindow )
	{
		if( bShow )
			pkWindow->Show();
		else
			pkWindow->Hide();
	}
}

//------------------------------------------------------------------------------------
void SPWindowLooting::ItemGain()
{
	if( m_ContainerItem[m_iSelectItem].Item.iItemID == 0 )
		return;

	if( g_pGOBManager->GetLocalPlayer() )
	{
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(
			SPGM_STAT_CS_LOOTING_ITEM_GAIN, (LPARAM)&m_ContainerItem[m_iSelectItem]);
	}
}

//------------------------------------------------------------------------------------
void SPWindowLooting::ItemAllGain()
{
	if( m_iValidItemNum == 0 )
		return;

	if( g_pGOBManager->GetLocalPlayer() )
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_STAT_CS_LOOTING_ITEM_ALL_GAIN, 0);
}

//------------------------------------------------------------------------------------
// Message
int SPWindowLooting::OnCursorEnter( WPARAM, LPARAM )
{
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnCursorOut( WPARAM, LPARAM )
{
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnItemSetting(WPARAM wParam, LPARAM lParam)
{
	Clear();

	vector< CONTAINER_ITEM >* pvItemList = (vector< CONTAINER_ITEM >*)wParam;
	SPPlayerInvenArchive* pkInvenArchive = (SPPlayerInvenArchive*)lParam;
	if( pvItemList == NULL || pkInvenArchive == NULL )
		return 1;

	int iCount = 0;
	vector< CONTAINER_ITEM >::iterator iterItem = (*pvItemList).begin();
	while( iterItem != (*pvItemList).end() )
	{
		m_ContainerItem[iCount] = (*iterItem);

		++iCount;
		++iterItem;
	}

	m_iCurPage = 0;
	m_iMaxPage = (iCount - 1) / 4;
	m_iValidItemNum = iCount;

	OnInventoryCheck(wParam, lParam);			//[2005/7/1] - jinhee
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnItemDelete(WPARAM wParam, LPARAM lParam)
{
	vector< CONTAINER_ITEM >* pvItemList = (vector< CONTAINER_ITEM >*)wParam;
	SPPlayerInvenArchive* pkInvenArchive = (SPPlayerInvenArchive*)lParam;
	if( pvItemList == NULL || pkInvenArchive == NULL )
		return 1;

	OnInventoryCheck(wParam, lParam);

	vector< CONTAINER_ITEM >::iterator iterItem;
	int i;
	bool bDelete;

	m_iValidItemNum = 0;
	for( i = 0; i < ITEM_MAX_NUM; i++ )
	{
		if( m_ContainerItem[i].Item.iItemID == 0 )
			continue;

		bDelete = true;
		iterItem = (*pvItemList).begin();
		while( iterItem != (*pvItemList).end() )
		{
			if( m_ContainerItem[i].Item.iItemID == (*iterItem).Item.iItemID &&
				m_ContainerItem[i].SlotInfo.SlotIndex == (*iterItem).SlotInfo.SlotIndex )
			{
				bDelete = false;
				break;
			}

			++iterItem;
		}

		if( bDelete )
		{
			m_ContainerItem[i].Item.iItemID = 0;
			m_ContainerItem[i].SlotInfo.SlotIndex = 0;
		}
		else
			m_iValidItemNum++;
	}

	SetItemWindow();

	if( m_iValidItemNum == 0 )
		OnExit(0, 0);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnItemReDraw(WPARAM wParam, LPARAM lParam)
{
	SPPlayerInvenArchive* pkInvenArchive = (SPPlayerInvenArchive*)lParam;
	if( pkInvenArchive == NULL )
		return 1;

	OnInventoryCheck(wParam, lParam);
	SetItemWindow();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnInventoryCheck(WPARAM wParam, LPARAM lParam)
{
	SPPlayerInvenArchive* pkInvenArchive = (SPPlayerInvenArchive*)lParam;
	if( pkInvenArchive == NULL )
		return 1;

	int i, iItemID;
	CONTAINER_TYPE eType;
	for( i = 0; i < ITEM_MAX_NUM; i++ )
	{
		iItemID = m_ContainerItem[i].Item.iItemID;
		if( iItemID == 0 )
			continue;

		eType = m_ContainerItem[i].SlotInfo.ContainerType;
		m_bInventoryFull[i] = pkInvenArchive->IsFullInventory(eType);
		if( m_bInventoryFull[i] )
		{
			if( pkInvenArchive->IsEnableSpace(eType, iItemID) )
				m_bInventoryFull[i] = false;
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnPagePrev(WPARAM, LPARAM)
{
	m_iCurPage--;
	if( m_iCurPage < 0 )
		m_iCurPage = 0;

	OnItemClick(WIID_LOOTING_ITEM_BTN, 0, 0);
	SetItemWindow();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnPageNext(WPARAM, LPARAM)
{
	m_iCurPage++;
	if( m_iCurPage > m_iMaxPage )
		m_iCurPage = m_iMaxPage;

	OnItemClick(WIID_LOOTING_ITEM_BTN, 0, 0);
	SetItemWindow();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnItemClick(unsigned int uiID, WPARAM, LPARAM)
{
	int iSelNum = uiID - WIID_LOOTING_ITEM_BTN;
	SPWindow* pkWindow;
	
	//pkWindow = Find(WIID_LOOTING_ITEM_EMPTY + iSelNum, true);
	//if( pkWindow == NULL )
	//	return 1;
	
	//if( pkWindow->IsShow() )
	//	return 1;

	for( int i = 0; i < 4; i++ )
	{
		pkWindow = Find(WIID_LOOTING_ITEM_SEL_BG + i, true);
		if( pkWindow == NULL )
			continue;

		if( i == iSelNum )
			pkWindow->Show();
		else
			pkWindow->Hide();
	}

	m_iSelectItem = iSelNum + m_iCurPage * 4;
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnItemLootingClick(unsigned int uiID, WPARAM, LPARAM)
{
	OnItemClick(uiID, 0, 0);
	ItemGain();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnExit(WPARAM, LPARAM)
{
	if( g_pGOBManager->GetLocalPlayer() )
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_STAT_LOOTING_EXIT, 0);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnToolTipMove(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	int iCurIndex = uiID - WIID_LOOTING_ITEM_BTN;
	iCurIndex += (m_iCurPage * 4);

	unsigned int uiUniqueID;
	int iEquipClass, iItemID, iItemColor;

	uiUniqueID = m_ContainerItem[iCurIndex].Item.iItemID;
	iItemColor = (m_ContainerItem[iCurIndex].Item.iItemID) % 100;
	iEquipClass = (m_ContainerItem[iCurIndex].Item.iItemID) / 100000;
	iItemID = ((m_ContainerItem[iCurIndex].Item.iItemID) % 100000 - iItemColor) / 100;

	SPItemStatus kItemStatus;
	kItemStatus.SetItemID(uiUniqueID);
	kItemStatus.SPSendMessage( MV_ITEM_SET_OPTION, (LPARAM)m_ContainerItem[iCurIndex].Item.Opt );	// AJJIYA [12/5/2005]

	SPItemAttr* pItemAttr;
	g_pItemCluster->GetItemInfo(uiUniqueID, pItemAttr);
	if( pItemAttr )
	{
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pItemAttr->m_ePosID1);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, iEquipClass);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, iItemID);
		kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	iItemColor);
		kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, m_ContainerItem[iCurIndex].Item.iRare);	//[2005/6/3] - jinhee

		if(pItemAttr->m_eDuration != ITEM_DURATION_NULL) {											//[2007/1/30] - jinssaga   
			kItemStatus.SetLifeTime(pItemAttr->m_iLifeTime * 3600);
		}

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)&kItemStatus, lParam);
		pWindow->Show();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnToolTipEnter(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	int iCurIndex = uiID - WIID_LOOTING_ITEM_BTN;
	iCurIndex += (m_iCurPage * 4);

	unsigned int uiUniqueID;
	int iEquipClass, iItemID, iItemColor;

	uiUniqueID = m_ContainerItem[iCurIndex].Item.iItemID;
	iItemColor = (m_ContainerItem[iCurIndex].Item.iItemID) % 100;
	iEquipClass = (m_ContainerItem[iCurIndex].Item.iItemID) / 100000;
	iItemID = ((m_ContainerItem[iCurIndex].Item.iItemID) % 100000 - iItemColor) / 100;

	SPItemStatus kItemStatus;
	kItemStatus.SetItemID(uiUniqueID);
	kItemStatus.SPSendMessage( MV_ITEM_SET_OPTION, (LPARAM)m_ContainerItem[iCurIndex].Item.Opt );	// AJJIYA [12/5/2005]

	SPItemAttr* pItemAttr;
	g_pItemCluster->GetItemInfo(uiUniqueID, pItemAttr);
	if( pItemAttr )
	{
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pItemAttr->m_ePosID1);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, iEquipClass);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, iItemID);
		kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	iItemColor);
		kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, m_ContainerItem[iCurIndex].Item.iRare);	//[2005/6/3] - jinhee

		if(pItemAttr->m_eDuration != ITEM_DURATION_NULL) {											//[2007/1/30] - jinssaga   
			kItemStatus.SetLifeTime(pItemAttr->m_iLifeTime * 3600);
		}

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&kItemStatus, lParam);
		pWindow->Show();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLooting::OnToolTipOut(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowLooting )

SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)

SPIMESSAGE_COMMAND(	SPIM_LOOTING_ITEM_SET,		OnItemSetting	)
SPIMESSAGE_COMMAND(	SPIM_LOOTING_ITEM_DELETE,	OnItemDelete	)
SPIMESSAGE_COMMAND(	SPIM_LOOTING_ITEM_REDRAW,	OnItemReDraw	)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_LOOTING_PAGE_PREV,			OnPagePrev	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_LOOTING_PAGE_NEXT,			OnPageNext	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_LOOTING_EXIT,				OnExit	)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_LOOTING_ITEM_BTN,		WIID_LOOTING_ITEM_BTN + 4,	OnItemClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_RBUTTON_UP, WIID_LOOTING_ITEM_BTN,		WIID_LOOTING_ITEM_BTN + 4,	OnItemLootingClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER, WIID_LOOTING_ITEM_BTN,	WIID_LOOTING_ITEM_BTN + 4,	OnToolTipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT, WIID_LOOTING_ITEM_BTN,		WIID_LOOTING_ITEM_BTN + 4,	OnToolTipOut)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE, WIID_LOOTING_ITEM_BTN,		WIID_LOOTING_ITEM_BTN + 4,	OnToolTipMove)

SPIMESSAGE_OBJ_MAP_END(SPWindow)