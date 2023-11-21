
#include "SPCommon.h"

#include "SPGameObject.h"
#include "SPGOBManager.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"

#include "SPResourceDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"

#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPPlayer.h"
#include "SPPlayerInvenArchive.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowGrid.h"

#include "SPGameObjectDEF.h"

#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPSubGameManager.h"

#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"
#include "SPWindowToolTip.h"

#include "SPWindowEquip.h"

SPWindowEquip::SPWindowEquip(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{	
	Init();
}

SPWindowEquip::SPWindowEquip(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_EQUIP, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowEquip::~SPWindowEquip()
{
	Clean();
}

void SPWindowEquip::Init()
{	
	//m_eCurrentPage = EQUIP_STORAGE_BATTLE;
	m_iCurrentPage = FIGURE_EQUIP;
	
	m_iActiveWeaponSlot = 1;

	SPWindow* pStatic = NULL;
	SPWindow* pCenter = NULL;
	
	pStatic = new SPWindowStatic(WIID_EQUIP_TOP, 0, 0, 234, 4, this);
	pStatic->SetImage("Data/Interface/Concept/ui102.png", 1,	16);
	pStatic->Show();
	pStatic = NULL;

	pCenter = new SPWindowStatic(WIID_EQUIP_CENTER, 0, 4, 234, 207, this);
	pCenter->SetImage("Data/Interface/Concept/ui102.png", 1,	22);
	pCenter->SetSrcSize(234, 2);
	pCenter->Show();

	pStatic = new SPWindowStatic(WIID_EQUIP_BOTTOM, 0, 211, 234, 4, this);
	pStatic->SetImage("Data/Interface/Concept/ui102.png", 1,	26);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_EQUIP_TITLE_IMAGE, 8, 3, 26, 25, this);
	pStatic->SetImage("Data/Interface/Concept/ui102.png", 98,	341);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_EQUIP_TITLE, 42, 7, 62, 15, this);
	pStatic->SetImage("Data/Interface/Concept/ui102.png", 139,	367);
	pStatic->Show();
	pStatic = NULL;
    

	SPWindowButton* pButton;
	pButton = new SPWindowButton( WIID_EQUIP_HIDE, 213, 9 - 4, 12, 12, pCenter);
	pButton->Show();	
	pButton->SetImage		("Data/Interface/Concept/ui102.png", 247,	217);	
	pButton->SetImageHit	("Data/Interface/Concept/ui102.png", 247,	230);	
	pButton->SetImagePush	("Data/Interface/Concept/ui102.png", 247,	243);	
	pButton->SetImageDisable("Data/Interface/Concept/ui102.png", 247,	256);
	m_pHide = pButton;
	pButton = NULL;
	
	//Button Group Skin
	SPWindow* pGroup = NULL;
	pGroup = new SPWindowStatic(WIID_EQUIP_GROUP, 2, 26 - 4, 230, 19, pCenter);
	pGroup->SetImage("Data/Interface/Concept/ui102.png", 349,	312);
	pGroup->SetSrcSize(6, 6);
	pGroup->Show();

	pButton = new SPWindowButton( WIID_EQUIP_BATTLE, 10 - 2, 28 - 26, 64, 15, pGroup);
	pButton->Show();
	pButton->SetImage		("Data/Interface/Concept/ui102.png", 1,	384);
	pButton->SetImageHit	("Data/Interface/Concept/ui102.png", 1,	400);	
	pButton->SetImagePush	("Data/Interface/Concept/ui102.png", 1,	416);
	pButton->SetImageDisable("Data/Interface/Concept/ui102.png", 1,	432);
	pButton->SetImageCheck	("Data/Interface/Concept/ui102.png", 1,	416);
	pButton->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
	pButton = NULL;

	pButton = new SPWindowButton( WIID_EQUIP_FASHION, 73 - 2, 28 - 26, 64, 15, pGroup);
	pButton->Show();
	pButton->SetImage		("Data/Interface/Concept/ui102.png", 66, 384);
	pButton->SetImageHit	("Data/Interface/Concept/ui102.png", 66, 400);	
	pButton->SetImagePush	("Data/Interface/Concept/ui102.png", 66, 416);
	pButton->SetImageDisable("Data/Interface/Concept/ui102.png", 66, 432);	
	pButton->SetImageCheck	("Data/Interface/Concept/ui102.png", 66, 416);
	pButton = NULL;

	pButton = new SPWindowButton( WIID_EQUIP_EFFECT, 73+63 - 2, 28 - 26, 64, 15, pGroup);
	pButton->Show();
	pButton->SetImage		("Data/Interface/Concept/CNUI01.PNG", 1, 58);
	pButton->SetImageHit	("Data/Interface/Concept/CNUI01.PNG", 1, 74);	
	pButton->SetImagePush	("Data/Interface/Concept/CNUI01.PNG", 1, 90);
	pButton->SetImageDisable("Data/Interface/Concept/CNUI01.PNG", 1, 106);	
	pButton->SetImageCheck	("Data/Interface/Concept/CNUI01.PNG", 1, 90);
	pButton = NULL;

	////[2006/12/22] - jinssaga   
	//pButton = new SPWindowButton( WIID_EQUIP_PET, 136 - 2, 28 - 26, 64, 15, pGroup);	
	//pButton->SetImage		("Data/Interface/Concept/ui102.png", 131, 384);
	//pButton->SetImageHit	("Data/Interface/Concept/ui102.png", 131, 400);	
	//pButton->SetImagePush	("Data/Interface/Concept/ui102.png", 131, 416);
	//pButton->SetImageDisable("Data/Interface/Concept/ui102.png", 131, 432);	
	//pButton->SetImageCheck	("Data/Interface/Concept/ui102.png", 131, 416);
	//pButton->Show();	
	//pButton->SetEnable(false);
	//pButton = NULL;

	//Skin
	SPWindow* pSkin = NULL;
	pSkin = new SPWindowStatic(WIID_EQUIP_SKIN, 2, 45 - 4, 230, 160, pCenter);
	pSkin->SetImage("Data/Interface/Concept/ui102.png", 281, 1);
	pSkin->Show();	

	SPWindowGrid*	pGrid;
	pGrid = new SPWindowGrid(WNDID_CTRL_GRID, WIID_EQUIP_GRID, 10 - 2, 57 - 45, 102, 136, pSkin, 3, 4, 34, 34, 32);	
	//pGrid->SetImage("");
	pGrid->SetGridImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 358, 415);
	pGrid = NULL;

	pGrid = new SPWindowGrid(WNDID_CTRL_GRID, WIID_EQUIP_WEAPON1, 168 - 2, 54 - 45, 32, 68, pSkin, 1, 2, 34, 34, 32);
	//pGrid->SetImage("");
	pGrid->SetGridImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 358, 415);
	pGrid = NULL;

	pGrid = new SPWindowGrid(WNDID_CTRL_GRID, WIID_EQUIP_WEAPON2, 168 - 2, 130 - 45, 32, 68, pSkin, 1, 2, 34, 34, 32);
	//pGrid->SetImage("");
	pGrid->SetGridImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 358, 415);
	pGrid = NULL;
}

void SPWindowEquip::Clean()
{
	SPWindow::Clean();
}

void SPWindowEquip::Process(float fTime)
{
	SPWindow::Process(fTime);
}

void SPWindowEquip::Show(bool bWithChild)
{
#ifdef _PHANTOM_EDITOR_ENABLE
	SPWindow *wnd = g_pInterfaceManager->FindWindow( WIID_PHANTOM_EDITOR );
	if (wnd)
		wnd->Show();
#endif
	if(m_bShow == false) {
		if(g_pGOBManager->GetLocalPlayer()) // metalgeni
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_REFRESH_GRID , 0);
	}	
	
	SPWindow::Show(bWithChild);	

	//	AJJIYA [9/8/2005]
	//	창 정렬을 위해서!!
	g_pInterfaceManager->OrderWindowPosMove();
}

void SPWindowEquip::Hide(bool bSendServer)
{
	if(m_bShow) {
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
		//g_pInterfaceManager->ForceReply();					//[2005/10/7]
	}
	
	SPWindow::Hide(bSendServer);
	
	OnToolTipOut(0, 0);

	//	AJJIYA [9/8/2005]
	//	창 정렬을 위해서!!
	g_pInterfaceManager->OrderWindowPosMove();
}

void SPWindowEquip::Render(float fTime)
{
	SPWindow::Render(fTime);
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowEquip )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,			OnCursorEnter	)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,			OnCursorOut		)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,			OnLButtonUp		)	
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,			OnLButtonDown	)

	SPIMESSAGE_COMMAND(	SPIM_EQUIP_CHANGE_WEAPON	, OnChangeWeapon )
	SPIMESSAGE_COMMAND( SPIM_WEAPON_CLEAR1			, OnClearWeapon1 )
	SPIMESSAGE_COMMAND( SPIM_WEAPON_CLEAR2			, OnClearWeapon2 )
	SPIMESSAGE_COMMAND( SPIM_WEAPON_SELECT1,		OnWeapon1Select )	//[2005/7/8]
	SPIMESSAGE_COMMAND( SPIM_WEAPON_DISABLE1,		OnWeapon1Disable )

	SPIMESSAGE_COMMAND( SPIM_WEAPON_SETTING1		, OnSettingWeapon1 )
	SPIMESSAGE_COMMAND( SPIM_WEAPON_SETTING2		, OnSettingWeapon2 )
	SPIMESSAGE_COMMAND( SPIM_WEAPON_ACTIVE_SLOT		, OnActiceSlot	)
	SPIMESSAGE_COMMAND( SPIM_WEAPON_SELECT2,		OnWeapon2Select)	//[2005/7/8]
	SPIMESSAGE_COMMAND( SPIM_WEAPON_DISABLE2,		OnWeapon2Disable)

	SPIMESSAGE_COMMAND(	SPIM_EQUIP_SETTING,			OnEquipSetting	)
	SPIMESSAGE_COMMAND(	SPIM_EQUIP_ALLCLEAR ,		OnEquipAllClear	)
	SPIMESSAGE_COMMAND(	SPIM_EQUIP_CLEAR ,			OnEquipClear	)	
	SPIMESSAGE_COMMAND( SPIM_EQUIP_SELECT,			OnEquipSelect	)	//[2005/7/8]
	SPIMESSAGE_COMMAND( SPIM_EQUIP_DISABLE,			OnEquipDisable	)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_TOP,			OnLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_CENTER,		OnLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_BOTTOM,		OnLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_SKIN,		OnLButtonUp	)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_HIDE,		OnHide	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_HIDE,		OnHide	)
	
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_MARK1,		OnMark1	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_MARK2,		OnMark2	)
	
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN,		WIID_EQUIP_GRID,		OnGridDown		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_GRID,		OnGridUp		)	
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE,		WIID_EQUIP_GRID, 		OnCursorEquipMove	)
	SPIMESSAGE_CONTROL(	SPIM_RBUTTON_UP,		WIID_EQUIP_GRID,		OnGridRUp		)	
	
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN,		WIID_EQUIP_WEAPON1,		OnWeapon1Down	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_WEAPON1,		OnWeapon1Up		)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE,		WIID_EQUIP_WEAPON1, 	OnCursorWeapon1Move	)
	SPIMESSAGE_CONTROL(	SPIM_RBUTTON_UP,		WIID_EQUIP_WEAPON1,		OnWeapon1RUp	)
	
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN,		WIID_EQUIP_WEAPON2,		OnWeapon2Down	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_WEAPON2,		OnWeapon2Up		)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE,		WIID_EQUIP_WEAPON2, 	OnCursorWeapon2Move	)
	SPIMESSAGE_CONTROL(	SPIM_RBUTTON_UP,		WIID_EQUIP_WEAPON2,		OnWeapon2RUp	)

	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_EQUIP_GRID,		OnToolTipEnter	)
	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_EQUIP_GRID,		OnToolTipOut	)
	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,		WIID_EQUIP_GRID,		OnToolTipMove	)

	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_EQUIP_WEAPON1,		OnToolTipEnterWeaponSlot1	)
	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,		WIID_EQUIP_WEAPON1,		OnToolTipEnterWeaponSlot1	)
	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_EQUIP_WEAPON1,		OnToolTipOut	)	

	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_EQUIP_WEAPON2,		OnToolTipEnterWeaponSlot2	)
	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,		WIID_EQUIP_WEAPON2,		OnToolTipEnterWeaponSlot2	)
	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_EQUIP_WEAPON2,		OnToolTipOut	)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_BATTLE,		OnEquipBattle	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_FASHION,		OnEquipFashion	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_EFFECT,		OnEquipMedal	)
	//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_EQUIP_PET,			OnEquipPet		)

	SPIMESSAGE_COMMAND( SPIM_SET_EXIT_SHOW,		OnExitHide		)
	SPIMESSAGE_COMMAND( SPIM_EQUIP_SETDEFAULT,	OnSetDefault	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowEquip::OnCursorEnter( WPARAM, LPARAM )
{
	return 1;
}

int SPWindowEquip::OnCursorOut( WPARAM, LPARAM )
{
	return 1;
}

int SPWindowEquip::OnLButtonDown( WPARAM, LPARAM )
{
	return 1;
}

int SPWindowEquip::OnLButtonUp( WPARAM, LPARAM )
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	return 1;
}

int SPWindowEquip::OnToolTipEnter( WPARAM wParam, LPARAM lParam) 
{
	// by metalgeni
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 0;

	//int iXPos = LOWORD(lParam);
	//int iYPos = HIWORD(lParam);
	//CONTAINER_TYPE iContainerType = (CONTAINER_TYPE)wParam;
	CONTAINER_TYPE iContainerType = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetCurrentEquipPage();
	int iSlotIndex = (int)lParam;


	//Pick Up Item과 같은 아이템인 경우
	if(iContainerType == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() &&
		iSlotIndex == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetSlotIndex()) 
	{
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	//SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetGridItemStatusInven(iXPos, iYPos);
	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex) == NULL)
		return 0;
	
	SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex)->GetItemStatus();
	//if(pItemStatus == NULL)
	//	return 0;

	//if(pItemStatus->GetItemID() == 0 || pItemStatus->GetItemAttr() == NULL)
	//	return 0;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pItemStatus, SPWindowToolTip::TOOLTIP_SHOW_TYPE_ITEM_SET );
	pWindow->Show();

	return 1;
}

int SPWindowEquip::OnToolTipOut	( WPARAM wParam, LPARAM lParam) 
{

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();

	return 1;
}

int SPWindowEquip::OnToolTipMove( WPARAM wParam, LPARAM lParam) 
{
	//int iXPos = LOWORD(lParam);
	//int iYPos = HIWORD(lParam);
	//CONTAINER_TYPE iContainerType = (CONTAINER_TYPE)wParam;
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 0;

	CONTAINER_TYPE iContainerType = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetCurrentEquipPage();
	int iSlotIndex = (int)lParam;

	//Pick Up Item과 같은 아이템인 경우
	if(iContainerType == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() &&
		iSlotIndex == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetSlotIndex()) 
	{
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	//SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetGridItemStatusEquip(iXPos, iYPos);
	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex) == NULL)
		return 0;
	
	SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex)->GetItemStatus();
	//if(pItemStatus == NULL)
	//	return 0;

	//if(pItemStatus->GetItemID() == 0 || pItemStatus->GetItemAttr() == NULL)
	//	return 0;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pItemStatus, SPWindowToolTip::TOOLTIP_SHOW_TYPE_ITEM_SET );
	pWindow->Show();

	return 1;
}


int SPWindowEquip::OnToolTipEnterWeaponSlot1	( WPARAM wParam, LPARAM lParam) 
{
	//int iXPos = LOWORD(lParam);
	//int iYPos = HIWORD(lParam);
	//CONTAINER_TYPE iContainerType = (CONTAINER_TYPE)wParam;
	//CONTAINER_TYPE iContainerType = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetCurrentEquipPage();
	CONTAINER_TYPE iContainerType = FIGURE_EQUIP;
	
	int iSlotIndex = (int)lParam;
	if(iSlotIndex == 0)
		iSlotIndex = EQ_WEAPON1 - 1;		//12
	else 
		iSlotIndex = EQ_BUCKLER1 - 1;		//13

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 0;

	//Pick Up Item과 같은 아이템인 경우
	if(iContainerType == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() &&
		iSlotIndex == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetSlotIndex()) 
	{
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex) == NULL)
		return 0;
	//SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetGridItemStatusWeapon1(iXPos, iYPos);
	SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex)->GetItemStatus();
	//if(pItemStatus == NULL)
	//	return 0;

	//if(pItemStatus->GetItemID() == 0 || pItemStatus->GetItemAttr() == NULL)
	//	return 0;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pItemStatus, SPWindowToolTip::TOOLTIP_SHOW_TYPE_ITEM_SET );
	pWindow->Show();

	return 1;	
}

int SPWindowEquip::OnToolTipEnterWeaponSlot2	( WPARAM wParam, LPARAM lParam) 
{
	//int iXPos = LOWORD(lParam);
	//int iYPos = HIWORD(lParam);
	//CONTAINER_TYPE iContainerType = (CONTAINER_TYPE)wParam;
	//CONTAINER_TYPE iContainerType = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetCurrentEquipPage();
	CONTAINER_TYPE iContainerType = FIGURE_EQUIP;
	
	int iSlotIndex = (int)lParam;
	if(iSlotIndex == 0)
		iSlotIndex = EQ_WEAPON2 - 1;		//14
	else 
		iSlotIndex = EQ_BUCKLER2 - 1;		//15

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 0;

	//Pick Up Item과 같은 아이템인 경우
	if(iContainerType == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() &&
		iSlotIndex == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetSlotIndex()) 
	{
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex) == NULL)
		return 0;
	//SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetGridItemStatusWeapon2(iXPos, iYPos);
	SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex)->GetItemStatus();
	//if(pItemStatus == NULL)
	//	return 0;

	//if(pItemStatus->GetItemID() == 0 || pItemStatus->GetItemAttr() == NULL)
	//	return 0;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pItemStatus, SPWindowToolTip::TOOLTIP_SHOW_TYPE_ITEM_SET );
	pWindow->Show();
	return 1;
}

////
//int SPWindowEquip::OnGrid(WPARAM wParam, LPARAM lParam)
//{
//	if(g_pGOBManager->GetLocalPlayer()) // metalgeni
//		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_CLICK_GRID , lParam);
//	return 1;
//}


int SPWindowEquip::OnGridDown(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer()) // metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_GRID_CLICK_DOWN , lParam);
	
	return 1;
}


int SPWindowEquip::OnGridUp(WPARAM wParam, LPARAM lParam)
{
	if((int)lParam < 0) {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	}
	
	if(g_pGOBManager->GetLocalPlayer()) // metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_GRID_CLICK_UP , lParam);

	return 1;
}


int SPWindowEquip::OnCursorEquipMove(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_GRID_CLICK_MOVE , (LPARAM)lParam);
	
	return 1;
}


int SPWindowEquip::OnGridRUp(WPARAM wParam, LPARAM lParam)
{
	if((int)lParam < 0) {
		return 1;
	}

	if(g_pGOBManager->GetLocalPlayer()) // metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_GRID_RCLICK_UP , lParam);

	return 1;
}


//////////////////////////////////////////////////////////////////////////

int SPWindowEquip::OnEquipBattle	( WPARAM, LPARAM lParam)
{
	//// PickUp Item 처리
	//if(g_pGOBManager->GetLocalPlayer()){
	//	if( g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() > INVENTORY_HOUSING
	//		&& g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() < STORAGE) 
	//	{
	//		if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() != FIGURE_EQUIP)
	//			return 0;
	//	}
	//}
	
	SPWindow* pWindow;
	pWindow = Find(WIID_EQUIP_BATTLE, true);
	if(pWindow) 
		pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE);

	pWindow = Find(WIID_EQUIP_FASHION, true);
	if(pWindow) 
		pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

	pWindow = Find( WIID_EQUIP_EFFECT, true);
	if (pWindow)
		pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);
	
	//pWindow = Find(WIID_EQUIP_PET, true);
	//if(pWindow) 
	//	pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

	//m_eCurrentPage = EQUIP_STORAGE_BATTLE;
	m_iCurrentPage = FIGURE_EQUIP;

	SPWindowStatic *skin = (SPWindowStatic*)Find( WIID_EQUIP_SKIN, true );
	skin->SPSendMessage( SPIM_SET_IMAGECLEAR );
	skin->SetImage("Data/Interface/Concept/ui102.png", 281, 1);
	
	if(g_pGOBManager->GetLocalPlayer()) // metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_CLICK_BATTLE , lParam);	
	return 1;
}

int SPWindowEquip::OnEquipFashion	( WPARAM, LPARAM lParam)
{
	//// PickUp Item 처리
	//if(g_pGOBManager->GetLocalPlayer()){
	//	if( g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() > INVENTORY_HOUSING
	//		&& g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() < STORAGE) 
	//	{
	//		if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() != FIGURE_FASHION)
	//			return 0;
	//	}
	//}
	
	SPWindow* pWindow;
	pWindow = Find(WIID_EQUIP_BATTLE, true);
	if(pWindow) 
		pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

	pWindow = Find(WIID_EQUIP_FASHION, true);
	if(pWindow) 
		pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE);

	pWindow = Find( WIID_EQUIP_EFFECT, true);
	if (pWindow)
		pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

	//pWindow = Find(WIID_EQUIP_PET, true);
	//if(pWindow) 
	//	pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);


	//m_eCurrentPage = EQUIP_STORAGE_FASHION;
	m_iCurrentPage = FIGURE_FASHION;

	SPWindowStatic *skin = (SPWindowStatic*)Find( WIID_EQUIP_SKIN, true );
	skin->SPSendMessage( SPIM_SET_IMAGECLEAR );
	skin->SetImage("Data/Interface/Concept/ui102.png", 281, 1);

	
	if(g_pGOBManager->GetLocalPlayer()) // metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_CLICK_FASHION , lParam);		
	return 1;
}

int SPWindowEquip::OnEquipMedal( WPARAM, LPARAM lParam)
{
	SPWindow* pWindow;
	pWindow = Find(WIID_EQUIP_BATTLE, true);
	if(pWindow) 
		pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

	pWindow = Find(WIID_EQUIP_FASHION, true);
	if(pWindow) 
		pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

	pWindow = Find( WIID_EQUIP_EFFECT, true);
	if (pWindow)
		pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE);

	m_iCurrentPage = FIGURE_EFFECT;

	SPWindowStatic *skin = (SPWindowStatic*)Find( WIID_EQUIP_SKIN, true );
	skin->SPSendMessage( SPIM_SET_IMAGECLEAR );
	skin->SetImage( "DATA/INTERFACE/CONCEPT/CNUI01.PNG", 66, 74 );

	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_CLICK_EFFECT, lParam);		
	return 1;
}

int SPWindowEquip::OnEquipPet		( WPARAM, LPARAM lParam)
{
	////[2005/7/4] 이쪽 예외 처리는 나중에... 
	//if(g_pGOBManager->GetLocalPlayer()){
	//	if( g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() > INVENTORY_HOUSING
	//		&& g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() < STORAGE) 
	//	{
	//		if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() != FIGURE_FASHION)
	//			return 0;
	//	}
	//}

	SPWindow* pWindow;
	pWindow = Find(WIID_EQUIP_BATTLE, true);
	if(pWindow) 
		pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

	pWindow = Find(WIID_EQUIP_FASHION, true);
	if(pWindow) 
		pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

	//pWindow = Find(WIID_EQUIP_PET, true);
	//if(pWindow) 
	//	pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE);

	//m_eCurrentPage = EQUIP_STORAGE_PET;
	m_iCurrentPage = FIGURE_EQUIP;
	
	if(g_pGOBManager->GetLocalPlayer()) // metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_CLICK_PET , lParam);		
	return 1;
}


int SPWindowEquip::OnSetDefault(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow;
	pWindow = Find(WIID_EQUIP_BATTLE, true);
	if(pWindow) 
		pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE);

	pWindow = Find(WIID_EQUIP_FASHION, true);
	if(pWindow) 
		pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

	pWindow = Find( WIID_EQUIP_EFFECT, true);
	if (pWindow)
		pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

	//pWindow = Find(WIID_EQUIP_PET, true);
	//if(pWindow) 
	//	pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

	//m_eCurrentPage = EQUIP_STORAGE_BATTLE;
	m_iCurrentPage = FIGURE_EQUIP;

	SPWindowStatic *skin = (SPWindowStatic*)Find( WIID_EQUIP_SKIN, true );
	skin->SPSendMessage( SPIM_SET_IMAGECLEAR );
	skin->SetImage("Data/Interface/Concept/ui102.png", 281, 1);

	//if(g_pGOBManager->GetLocalPlayer()) // metalgeni
	//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_CLICK_BATTLE , lParam);	
	return 1;
}


//////////////////////////////////////////////////////////////////////////

int SPWindowEquip::OnEquipSetting  ( WPARAM wParam , LPARAM lParam )
{
	CONTAINER_ITEM *item; // = new CONTAINER_ITEM;
	item = (CONTAINER_ITEM*)wParam;


	POINT ptSrcPos;	
	SPTexture* texture = g_pItemCluster->GetItemTexture(item->Item.iItemID, ptSrcPos); 	

	//Grid 에 대한 세팅
	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_GRID, true));
	if(pWindow)
		pWindow->SettingIconImage( texture, item->Item.iItemID, ptSrcPos.x, ptSrcPos.y, item->SlotInfo.SlotIndex, item->Item.iStackCount, (bool)lParam);
	//pWindow->CellSelectSell(item->SlotInfo.SlotIndex-1, false);	//[2005/6/7] - jinhee

	// 한벌옷 세팅 ㅠ.ㅠ
	/*if(item->Item.iItemID/10000000==5)
	{
		pWindow->SettingIconImage( texture, item->Item.iItemID, ptSrcPos.x, ptSrcPos.y, item->SlotInfo.SlotIndex, item->Item.iStackCount );
		pWindow->SettingIconImage( texture, item->Item.iItemID, ptSrcPos.x, ptSrcPos.y, item->SlotInfo.SlotIndex+1, item->Item.iStackCount );
	}*/	
	//////////////////////////////////////////////////////////////////////////
	
	return 1;
}

int SPWindowEquip::OnEquipClear( WPARAM wParam, LPARAM lParam)
{	
	//int xPos = LOWORD(lParam);
	//int yPos = HIWORD(lParam);
	int iSlotIndex = (int)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_GRID, true));
	//pWindow->ClearCell( xPos, yPos ) ;
	if(pWindow)
		pWindow->ClearCell(iSlotIndex) ;
	
	return 1;
}

int SPWindowEquip::OnEquipAllClear( WPARAM wParam, LPARAM lParam)
{
	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_GRID, true));
	if(pWindow)
		pWindow->ClearCellAll() ;
	pWindow = NULL;

	////[2005/6/14] 패션창에서도 무기 유지 하려면 막아 두워야 한다
	//pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_WEAPON1, true));
	//pWindow->ClearCellAll() ;
	//pWindow = NULL;
	//
	//pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_WEAPON2, true));
	//pWindow->ClearCellAll() ;

	return 1;
}



//////////////////////////////////////////////////////////////////////////
////
//int SPWindowEquip::OnWeapon1(WPARAM wParam, LPARAM lParam)
//{
//	if(g_pGOBManager->GetLocalPlayer()) 
//		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_CLICK_WEAPON1 , lParam);
//	return 1;
//}


int SPWindowEquip::OnWeapon1Down(WPARAM wParam, LPARAM lParam )
{
	if(g_pGOBManager->GetLocalPlayer()) 
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_WEAPON1_CLICK_DOWN , lParam);
	return 1;
}


int SPWindowEquip::OnWeapon1Up(WPARAM wParam, LPARAM lParam )
{
	if((int)lParam < 0) {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	}

	if(g_pGOBManager->GetLocalPlayer()) 
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_WEAPON1_CLICK_UP , lParam);
	return 1;
}


int SPWindowEquip::OnCursorWeapon1Move(WPARAM wParam, LPARAM lParam )
{
	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_WEAPON1_CLICK_MOVE , (LPARAM)lParam);
	
	return 1;
}


int SPWindowEquip::OnWeapon1RUp(WPARAM wParam, LPARAM lParam)
{
	if((int)lParam < 0) {
		//if(g_pGOBManager->GetLocalPlayer())
		//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		return 1;
	}

	if(g_pGOBManager->GetLocalPlayer()) 
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_WEAPON1_RCLICK_UP , lParam);
	
	return 1;
}


////
//int SPWindowEquip::OnWeapon2(WPARAM wParam, LPARAM lParam)
//{
//	if(g_pGOBManager->GetLocalPlayer()) 
//		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_CLICK_WEAPON2 , lParam);
//	return 1;
//}


int SPWindowEquip::OnWeapon2Down(WPARAM wParam, LPARAM lParam )
{
	if(g_pGOBManager->GetLocalPlayer()) 
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_WEAPON2_CLICK_DOWN , lParam);
	return 1;
}


int SPWindowEquip::OnWeapon2Up(WPARAM wParam, LPARAM lParam )
{
	if((int)lParam < 0) {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	}

	if(g_pGOBManager->GetLocalPlayer()) 
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_WEAPON2_CLICK_UP , lParam);
	return 1;
}


int SPWindowEquip::OnCursorWeapon2Move(WPARAM wParam, LPARAM lParam )
{
	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_WEAPON2_CLICK_MOVE , (LPARAM)lParam);
	return 1;
}


int SPWindowEquip::OnWeapon2RUp(WPARAM wParam, LPARAM lParam)
{
	if((int)lParam < 0) {
		//if(g_pGOBManager->GetLocalPlayer())
		//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		return 1;
	}

	if(g_pGOBManager->GetLocalPlayer()) 
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_EQUIP_WEAPON2_RCLICK_UP , lParam);
	
	return 1;
}


//////////////////////////////////////////////////////////////////////////

int SPWindowEquip::OnClearWeapon1( WPARAM wParam, LPARAM lParam )
{
	//int xPos = LOWORD(lParam);
	//int yPos = HIWORD(lParam);
	int iSlotIndex = (int)lParam;		//0, 1

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_WEAPON1, true));
	//pWindow->ClearCell( xPos, yPos ) ;
	if(pWindow)
		pWindow->ClearCell(iSlotIndex) ;
	
	return 1;
}

int SPWindowEquip::OnClearWeapon2( WPARAM wParam, LPARAM lParam )
{
	//int xPos = LOWORD(lParam);
	//int yPos = HIWORD(lParam);
	int iSlotIndex = (int)lParam;		//0, 1

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_WEAPON2, true));
	//pWindow->ClearCell( xPos, yPos ) ;
	if(pWindow)
		pWindow->ClearCell(iSlotIndex) ;
	return 1;
}


int SPWindowEquip::OnActiceSlot( WPARAM wParam, LPARAM lParam )
{
	m_iActiveWeaponSlot = wParam;

	SPWindow* pWindow;	
	//
	if(m_iActiveWeaponSlot == 1) {	
		pWindow = Find( WIID_EQUIP_MARK1 );
		if(pWindow) pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX, 1);
		pWindow = Find( WIID_EQUIP_MARK2 );
		if(pWindow) pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX, 0);
	} else {
		pWindow = Find( WIID_EQUIP_MARK1 );
		if(pWindow) pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX, 0);
		pWindow = Find( WIID_EQUIP_MARK2 );
		if(pWindow) pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX, 1);
	}

	return 1;
}


int SPWindowEquip::OnSettingWeapon1( WPARAM wParam, LPARAM lParam )
{
	CONTAINER_ITEM *item; // = new CONTAINER_ITEM;
	item = (CONTAINER_ITEM*)wParam;

	POINT ptSrcPos;	
	SPTexture* texture = g_pItemCluster->GetItemTexture(item->Item.iItemID, ptSrcPos); 

	SLOT_INDEX iSlotIndex = (item->SlotInfo.SlotIndex==EQ_WEAPON1 - 1)?0:1;
	bool bEnable = (bool)lParam;
	
	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_WEAPON1, true));
	if(pWindow)
		pWindow->SettingIconImage( texture, item->Item.iItemID, ptSrcPos.x, ptSrcPos.y, iSlotIndex, item->Item.iStackCount, bEnable);
		//pWindow->SettingIconImage( texture, item->Item.iItemID, ptSrcPos.x, ptSrcPos.y, (item->SlotInfo.SlotIndex==EQ_WEAPON1 - 1)?0:1, item->Item.iStackCount, (bool)lParam);
	
	
	return 1;
}

int SPWindowEquip::OnSettingWeapon2( WPARAM wParam, LPARAM lParam )
{
	CONTAINER_ITEM *item;  //= new CONTAINER_ITEM;
	item = (CONTAINER_ITEM*)wParam;

	POINT ptSrcPos;	
	SPTexture* texture = g_pItemCluster->GetItemTexture(item->Item.iItemID, ptSrcPos); 

	SLOT_INDEX iSlotIndex = (item->SlotInfo.SlotIndex==EQ_WEAPON2 - 1)?0:1;
	bool bEnable = (bool)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_WEAPON2, true));
	if(pWindow)
		pWindow->SettingIconImage( texture, item->Item.iItemID, ptSrcPos.x, ptSrcPos.y, iSlotIndex, item->Item.iStackCount, bEnable);
		//pWindow->SettingIconImage( texture, item->Item.iItemID, ptSrcPos.x, ptSrcPos.y, (item->SlotInfo.SlotIndex==EQ_WEAPON2 - 1)?0:1, item->Item.iStackCount, (bool)lParam );
	
	return 1;
}


//////////////////////////////////////////////////////////////////////////
int	SPWindowEquip::OnEquipSelect(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)wParam;
	bool bSelect = (bool)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_GRID, true));
	if(pWindow)
		pWindow->CellSelectSell(iSlotIndex, bSelect);	
	return 1;
}


int	SPWindowEquip::OnWeapon1Select(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)wParam;
	bool bSelect = (bool)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_WEAPON1, true));
	if(pWindow)
		pWindow->CellSelectSell(iSlotIndex, bSelect);	
	return 1;
}


int	SPWindowEquip::OnWeapon2Select(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)wParam;
	bool bSelect = (bool)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_WEAPON2, true));
	if(pWindow)
		pWindow->CellSelectSell(iSlotIndex, bSelect);	
	
	return 1;
}



//////////////////////////////////////////////////////////////////////////
int	SPWindowEquip::OnEquipDisable(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)wParam;
	bool bAble = (bool)lParam;
	
	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_GRID, true));
	if(pWindow)
		pWindow->CellEnable(iSlotIndex, bAble);

	return 1;	
}


int	SPWindowEquip::OnWeapon1Disable(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)wParam;
	bool bAble = (bool)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_WEAPON1, true));
	if(pWindow)
		pWindow->CellEnable(iSlotIndex, bAble);
	
	return 1;
}


int	SPWindowEquip::OnWeapon2Disable(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)wParam;
	bool bAble = (bool)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_EQUIP_WEAPON2, true));
	if(pWindow)
		pWindow->CellEnable(iSlotIndex, bAble);
	
	return 1;
}



//////////////////////////////////////////////////////////////////////////
int SPWindowEquip::OnChangeWeapon( WPARAM wParam, LPARAM lParam)
{
	return 1;
}


int	SPWindowEquip::OnHide(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	return SPWindow::OnHide(wParam, lParam);
}


int	SPWindowEquip::OnMark1(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	return 1;
}


int	SPWindowEquip::OnMark2(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	return 1;
}


int SPWindowEquip::OnExitHide( WPARAM wParam, LPARAM lParam)
{
	bool bShow = (bool)lParam;

	if(bShow) {
		m_pHide->Show();
	}
	else {
		m_pHide->Hide();
	}

	return 1;
}