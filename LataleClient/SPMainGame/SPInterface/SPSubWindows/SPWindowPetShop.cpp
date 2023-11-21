#include "SPCommon.h"

#include "SPGameObject.h"
#include "SPGOBManager.h"
#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPSubGameManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "Packet.h"
#include "PacketID.h"
#include "SPNetworkManager.h"

#include "SPUIUnit.h"
#include "SPUISkillUnit.h"
#include "SPUIUnitManager.h"
#include "SPDragnDropManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowSlider.h"
#include "SPWindowGrid.h"
#include "SPWindowGauge.h"
#include "SPWindowToolTip.h"
#include "SPWindowPetShop.h"

#include "SPCommandConvert.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPPlayerInvenArchive.h"

#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPPlayer.h"

#include "SPCheckManager.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPUtil.h"

#include "SPDebug.h"



SPWindowPetShop::SPWindowPetShop(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_PETSHOP, InstanceID, iX, iY, iCX, iCY, pParent)	
, m_pSell(NULL)
, m_pEly(NULL)
, m_iSkillID(0)
{
	Init();
}


SPWindowPetShop::~SPWindowPetShop()
{
	Clean();
}


void SPWindowPetShop::Init()
{
	InitSubControl();
}


void SPWindowPetShop::Clean()
{
	SPWindow::Clean();
}


void SPWindowPetShop::Process(float fTime)
{	
	if(m_pEly && g_pGOBManager->GetLocalPlayer()) {
		m_pEly->SetWindowText(ConvertMoneyToString(g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY)));
	}
	SPWindow::Process(fTime);
}


void SPWindowPetShop::Render(float fTime)
{
	if(m_bShow == false)
		return;

	SPWindow::Render(fTime);

#ifdef _DEBUG
	RECT rtRect = {0, 0, 0, 0};	
	m_pSell->GetWindowRect(rtRect);
	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	g_pVideo->GetSysLine()->DrawRect(rtRect);
	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif
}


void SPWindowPetShop::Show(bool bWithChild /*= true*/)
{
	m_iSkillID = 0;
	
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return;

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return;

	SPItem* pPetItem = pInven->GetInventoryItem(FIGURE_PET, m_iPetSlot);
	if(pPetItem == NULL || pPetItem->GetItemStatus() == NULL || pPetItem->GetItemStatus()->GetItemID() ==0) {
		return;
	}

	if(pPetItem->GetItemStatus()->IsTimeAvailable() == false)
		return;
	
	SPWindow::Show(bWithChild);

	g_pCheckManager->SetPetShopOpen(true);
}


void SPWindowPetShop::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
	g_pCheckManager->SetPetShopOpen(false);
}


void SPWindowPetShop::InitSubControl()
{
	SPWindow* pStatic;
	//SPWindow* pChild;

	SPWindow* pCenter = NULL;
	SPWindow* pSkin1 = NULL;
	SPWindow* pSkin2 = NULL;
	SPWindowButton* pButton;

	pStatic = new SPWindowStatic(WIID_PETSHOP_TOP, 0, 0, 240, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 161);
	pStatic->Show();
	pStatic = NULL;

	pCenter = new SPWindowStatic(WIID_PETSHOP_CENTER, 0, 4, 240, 97, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 167);
	pCenter->SetSrcSize(240, 2);
	pCenter->Show();

	pStatic = new SPWindowStatic(WIID_PETSHOP_BOTTOM, 0, 101, 240, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 171);
	pStatic->Show();
	pStatic = NULL;	

	//WIID_PETSHOP_TITLE
	pStatic = new SPWindowStatic(WIID_PETSHOP_TITLE, 8, 7 , 200, 15, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 376);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PETSHOP_CLOSE
	pButton = new SPWindowButton(WIID_PETSHOP_CLOSE, 219, 9 - 4, 12, 12, pCenter);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG",	301,	321);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI103.PNG",	314,	321); //on
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI103.PNG",	327,	321); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	340,	321); //disable
	pButton->Show();
	pButton = NULL;

	//WIID_PETSHOP_SELLSKIN	
	pSkin1 = new SPWindowStatic(WIID_PETSHOP_SELLSKIN, 2, 26-4, 236, 55, pCenter);
	pSkin1->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 240, 35);
	pSkin1->SetSrcSize(2, 2);
	pSkin1->Show();

	//WIID_PETSHOP_SELL
	pButton = new SPWindowButton(WIID_PETSHOP_BTN_SELL, 0, 0, 236, 55, pSkin1);
	pButton->Show();
	m_pSell = pButton;
	
	//WIID_PETSHOP_SELLHELP	
	pStatic = new SPWindowStatic(WIID_PETSHOP_SELLHELP, 35 - 2, 37 - 26, 169, 33, pSkin1);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 342);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_PETSHOP_ELY_TEXT_IMG
	pStatic = new SPWindowStatic(WIID_PETSHOP_ELY_TEXT_IMG, 52, 87 - 4, 34, 11, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 301, 378);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PETSHOP_ELY
	pStatic = new SPWindowStatic(WIID_PETSHOP_ELY, 92, 85 - 4, 121, 14, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 104, 345);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	pStatic->Show();
	m_pEly = pStatic;
	pStatic = NULL;

	//WIID_PETSHOP_ELY_IMG
	pStatic = new SPWindowStatic(WIID_PETSHOP_ELY_TEXT_IMG, 217, 92 - 4, 17, 5, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 494, 506);
	pStatic->Show();
	pStatic = NULL;
}


void SPWindowPetShop::SetSkillID(int iId /*= 0*/)
{
	m_iSkillID = iId;
}

void SPWindowPetShop::SetPetSlot(int iSlot /* = 0 */)
{
	m_iPetSlot = iSlot;
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowPetShop )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_PETSHOP_CLOSE,			OnClose		)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_PETSHOP_BTN_SELL,			OnItemSell	)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowPetShop::OnClose(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}


int SPWindowPetShop::OnItemSell(WPARAM wParam, LPARAM lParam)
{	
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 1;
	
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return 1;

	//사망시 제한
	if(g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurState() == GOB_STATE_DEAD) {
		return 1;
	}

	//픽업 상태가 아닌경우 넘어간다
	if(pInven->m_bPickup == false) {
		return 1;
	}

	//Pet Item 을 장비중인지
	SPItem* pPetItem = pInven->GetInventoryItem(FIGURE_PET, m_iPetSlot);
	if(pPetItem == NULL || pPetItem->GetItemStatus() == NULL || pPetItem->GetItemStatus()->GetItemID() ==0) {
		return 1;
	}
	
	//Pet Item이 시간이 만료되었는지
	if(pPetItem->GetItemStatus()->IsTimeAvailable() == false)
		return 1;

	SPSelectItem SelectItem = pInven->GetPickupItem();
	SPItem* pItem = pInven->GetInventoryItem(SelectItem.m_iContainer, SelectItem.m_iSlotIndex);	

	// 판매할 아이템이 있는지
	if(pItem == NULL || pItem->GetItemStatus() == NULL || pItem->GetItemStatus()->GetItemID() == 0) {
		return 1;
	}

	//장비탭에서 오는 경우
	if(SelectItem.GetContainerType() >= INVENTORY_HOUSING) {
		return 1;
	}

	//판매 불가능한 속성의 아이템의 경우 리턴
	SPItemAttr* pItemAttr = pItem->GetItemStatus()->GetItemAttr();
	if(pItemAttr == NULL || pItemAttr->m_bSell == false) {
		return 1 ;
	}

	//스킬이 없는경우
	if(m_iSkillID == 0) 
		return 1;
	
	CONTAINER_ITEM item;
	if(SelectItem.m_iContainer < INVENTORY_EQUIP || SelectItem.m_iContainer > INVENTORY_HOUSING)
		return 1;
	if(SelectItem.m_iSlotIndex < 0 || SelectItem.m_iSlotIndex >= pInven->GetInventorySize(SelectItem.m_iContainer))
		return 1;
	
	item.SlotInfo.ContainerType = SelectItem.m_iContainer;
	item.SlotInfo.SlotIndex = SelectItem.m_iSlotIndex + 1;
	item.Item.iItemID = pItem->GetItemStatus()->GetItemID();
	item.Item.iStackCount = pItem->GetItemStatus()->SPSendMessage(MV_ITEM_GETSTACK_CNT);
	item.Item.iBackPrice = pItem->GetItemStatus()->SPSendMessage(MV_ITEM_GET_BACKPRICE);
	item.Item.iItemNo = *(ITEMNO*)pItem->GetItemStatus()->SPSendMessage(MV_ITEM_GET_ITEMNO);		
	
	CONTAINER_SLOT PetSlot;
	PetSlot.ContainerType = FIGURE_PET;
	PetSlot.SlotIndex = m_iPetSlot+1;

	if(g_pCheckManager->IsDBBlock()) {

		DXUTOutputDebugString("\tSPWindowItemSell::OnSell DB Working Block\n");
		return false;
	}

	////ITEMSKILL_CS_SELL, 
	// CONTAINER_SLOT	stSkillItemPos;	// 상점판매특수스킬을 가지고 있는 장착한아이템정보
	// int				iSkillID;		// 상점판매 속성을 가지고 있는 스킬번호
	// CONTAINER_ITEM	stSellItem;		// 판매할 아이템정보

	//팻킷 만들기
	CPacket Packet(ITEMSKILL_CS_SELL);
	Packet.AddData(&PetSlot, sizeof(CONTAINER_SLOT));
	Packet.Add((UINT32)m_iSkillID);
	Packet.AddData(&item, sizeof(CONTAINER_ITEM));
	g_pNetworkManager->SPPerformMessage(ITEMSKILL_CS_SELL, 0, (LPARAM)&Packet);
	
	g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CASH.WAV");	

	return 1;
}

