#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPGameObject.h"
#include "SPItemAttr.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPGOBManager.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"

#include "SPCommandConvert.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayer.h"

#include "SPItemStatus.h"
#include "SPItemStatusEquip.h"
#include "SPItemStatusConsumable.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPNoticeBox.h"

#include "Packet.h"
#include "PacketID.h"
//#include "SPItemCluster.h"				//[2005/6/16] - jinhee
#include "SPItem.h"
#include "SPMouseCursor.h"
#include "SPNetworkManager.h"
#include "SPInterfaceManager.h"
#include "SPWindowStorage.h"
//#include "SPwindowItem.h"					//[2005/12/12]
#include "SPPlayerInvenArchive.h"

#include "SPEventDEF.h"						//[2005/7/20]
#include "SPEventNpcHandler.h"
#include "SPQuestManager.h"
#include "SPEventManager.h"

#include "SPPvpManager.h"

#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"

#include "SPCheckManager.h"					//[2005/9/27]
#include "SPLocalizeManager.h"

#include "SPCoolTimeManager.h"

#include "SPCashTradeManager.h"

#include "SPEffect.h"
#include "SPEffectArchive.h"
#include "SPSubGameManager.h"

#include "SPDebug.h"
#include "SPWindowBugleSelect.h"
#include "SPWindowVisualBugle.h"
#include "SPWindowPhantomLevelUp.h"

SPPlayerInvenArchive::SPPlayerInvenArchive(SPGameObject* pParent)
:m_pParent(pParent)
, m_bPickup(false)
, m_iCurrentContainer(INVENTORY_EQUIP)
, m_iCurrentInventoryType(INVENTORY_EQUIP)
, m_iCurrentFigureType(FIGURE_EQUIP)
, m_iCurrentWeaponSlot(1)
, m_iItemLClickIndex(-1)
, m_iEquipLClickIndex(-1)
, m_iStorageLClickIndex(-1)
, m_iPetLClickIndex(-1)
, m_bClickMove(false)
, m_bSendRequest(false)
, m_bRequested(false)
, m_iNoticeUse(NOTICE_USE_NULL)
{
	Init();
}


SPPlayerInvenArchive::~SPPlayerInvenArchive()
{
	Clear();
}


void SPPlayerInvenArchive::Init()
{	
	for(int i = 0; i < CONTAINER_MAX; i++) {
		m_iInventorySlot[i] = 0;
		m_iExtendSlot[i] = 0;
		
		m_iInvenPage[i] = 0;
		if(i < FIGURE_EQUIP)
			m_iInvenPage[i] = (DEFAULT_INVENTORY_SLOT - 16) / 4;
		
		m_iCurInvenPage[i] = 0;
	}	

	
	//[8/9/2005]	ÀÌ°ÍµéÀº ·ÎÄÃ¸¸ ÀÖÀ¸¸é µÇ´Âµ¥ °ø°£³¶ºñÇßÀ½ ¤Ñ.¤Ñ;
	m_PickUpItem.Clear();
	m_TrashItem.Clear();
	m_SellItem.Clear();
	m_MicItem.Clear();
	m_FindItem.Clear();//ÕÒÈË
	//m_PetFeed.Clear();
	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		m_vecPetFeeds[idx].feedEnergy.Clear();
		m_vecPetFeeds[idx].feedNormal.Clear();
	}
	
	ClearAllUserShopItem();
	ClearAllBlockItem();
	ClearAllTradeItem();
}


bool SPPlayerInvenArchive::CreateLocalInventory()
{
	for(int i = 0; i < INVENTORY_TYPE; i++) {
		if(i == INVENTORY_PET) {
			m_iInventorySlot[i] = DEFAULT_INVENTORY_PET_SLOT;
			m_iExtendSlot[i] = 0;			
			
			m_iInvenPage[i] = 0;
			m_iCurInvenPage[i] = 0;
		}
		else {
			m_iInventorySlot[i] = DEFAULT_INVENTORY_SLOT;
			m_iExtendSlot[i] = 0;

			m_iInvenPage[i] = (DEFAULT_INVENTORY_SLOT - 16) / INVENTORY_LINE;
			m_iCurInvenPage[i] = 0;
		}		
	}

	m_iStorageSlot = DEFAULT_STORAGE_SIZE;	
	m_iStoragePage = 0;	
	if(m_iStorageSlot > VIEW_STORAGE_SLOT) {
		m_iStoragePage = (DEFAULT_STORAGE_SIZE - VIEW_STORAGE_SLOT) / STORAGE_LINE;
		if( ((DEFAULT_STORAGE_SIZE - VIEW_STORAGE_SLOT) % STORAGE_LINE) > 0 )
			m_iStoragePage++;
	}
	
	m_iCurStoragePage = 0;

	InitInventory();
	InitFigure();
	InitStorage();
	InitCashItem();
	InitWearItem();
	InitPetEquip();
	PrintInventoryInfo();

	m_PickUpItem.Clear();
	m_TrashItem.Clear();
	m_SellItem.Clear();
	m_MicItem.Clear();
	m_FindItem.Clear();
	//m_PetFeed.Clear();	
	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		m_vecPetFeeds[idx].feedEnergy.Clear();
		m_vecPetFeeds[idx].feedNormal.Clear();
	}


	ClearAllUserShopItem();
	ClearAllBlockItem();
	ClearAllTradeItem();
	
	m_iCurrentContainer = INVENTORY_EQUIP;
	m_iCurrentInventoryType = INVENTORY_EQUIP;
	m_iCurrentFigureType = FIGURE_EQUIP;
	g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SETDEFAULT, (WPARAM)0, (LPARAM)0);
	g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SETDEFAULT, (WPARAM)0, (LPARAM)0);
	ReflashInventory();
	SettingCurrentEquip();

	return true;
}

void SPPlayerInvenArchive::ClearInventory(CONTAINER_TYPE eType)
{
	if( eType >= CONTAINER_MAX )
		return;

	vector<SPItem*>::iterator iter = m_vpInventory[eType].begin();
	while( iter != m_vpInventory[eType].end() )
	{
		(*iter)->SetClear();
		++iter;
	}
}


void SPPlayerInvenArchive::SetExtendSlot(CONTAINER_TYPE iType, int iTotalCount /*= 0*/, int iExtendSize /*= 0*/)
{
	if(iType < INVENTORY_EQUIP || iType > STORAGE) //||(iType > INVENTORY_HOUSING || iType < STORAGE))
		return;

	if(iType > INVENTORY_HOUSING && iType < STORAGE)
		return;

	if(iTotalCount <= m_iInventorySlot[iType])
		return;

	int iExtendSlot = iTotalCount - m_iInventorySlot[iType];	//ÀÌ°Ô ½ÇÁ¦ ´Ã¾î¾ß ÇÒ ¼ö´Ù...	

	m_iExtendSlot[iType] += iExtendSlot;
	
	m_iInventorySlot[iType] = m_iInventorySlot[iType] + iExtendSlot;
	
	if(m_iInventorySlot[iType] > VIEW_SLOT) {
		m_iInvenPage[iType] = (m_iInventorySlot[iType] - 16) / INVENTORY_LINE;
		if(m_iExtendSlot[iType] % INVENTORY_LINE != 0) {
			m_iInvenPage[iType] = m_iInvenPage[iType] + 1;		
		}
	}	
	DXUTOutputDebugString("	- InvenPage[%d] [%d]\n", iType, m_iInvenPage[iType]);
	CreateInventory(iType, &m_vpInventory[iType], iExtendSlot);
	DXUTOutputDebugString("	* CreateInventory Con[%d] extendSize[%d]\n", iType, m_iExtendSlot[iType]);	
	
	if(iType == m_iCurrentInventoryType) {		
		if(iType == INVENTORY_PET) {
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_LINE, (WPARAM)m_iCurrentInventoryType, (LPARAM)0);
		}		

		SetItemScroll(iType);
		ReflashInventory();
	}	

	if(iType == STORAGE) {
		//ÁÖÀÇ... Ã¢°íÀÇ °æ¿ì ÆäÀÌÁö °ü¸®¸¦ ´Ù¸¥ ¹æ½ÄÀ¸·Î ÇÑ´Ù 		
		m_iStorageSlot = m_iInventorySlot[STORAGE];
		if(m_iStorageSlot > VIEW_STORAGE_SLOT) {
			m_iStoragePage = (m_iStorageSlot - VIEW_STORAGE_SLOT) / STORAGE_LINE;
			if( ((m_iStorageSlot - VIEW_STORAGE_SLOT) % STORAGE_LINE) > 0 )
				m_iStoragePage++;
		}

		g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_STORAGE_SETSLOT, (WPARAM)iExtendSlot);
	}
}


bool SPPlayerInvenArchive::IsEnableContainer(CONTAINER_TYPE iContainerType)
{
	if (iContainerType == FIGURE_EFFECT)
		return true;
	if(iContainerType < INVENTORY_EQUIP || iContainerType > INVENTORY_WEAR)
		return false;
	
	return true;
}


void SPPlayerInvenArchive::InitInventory()
{
	for(int i = 0; i < INVENTORY_TYPE ; i++) {		
		if(m_pParent->IsLocalPlayer() && g_pInterfaceManager->GetInvenWindow() && i == m_iCurrentInventoryType)
		{
			SetItemScroll((CONTAINER_TYPE)i);
		}
		
		switch(i){
			case INVENTORY_EQUIP:
				CreateInventory(INVENTORY_EQUIP, &m_vpInventory[INVENTORY_EQUIP]);
				break;
			case INVENTORY_CONSUME:
				CreateInventory(INVENTORY_CONSUME, &m_vpInventory[INVENTORY_CONSUME]);
				break;
			case INVENTORY_ETC:
				CreateInventory(INVENTORY_ETC, &m_vpInventory[INVENTORY_ETC]);
				break;
			case INVENTORY_EVENT:
				CreateInventory(INVENTORY_EVENT, &m_vpInventory[INVENTORY_EVENT]);
				break;
			case INVENTORY_PET:
				CreateInventory(INVENTORY_PET, &m_vpInventory[INVENTORY_PET], DEFAULT_INVENTORY_PET_SLOT);
				break;
			case INVENTORY_HOUSING:
				CreateInventory(INVENTORY_HOUSING, &m_vpInventory[INVENTORY_HOUSING]);
				break;
			default :
				break;
		}		
	}
}


void SPPlayerInvenArchive::InitFigure()
{
	for(int i = (int)FIGURE_EQUIP ; i <= (int)FIGURE_BASIC; i++) {
		switch(i) {
			case FIGURE_EQUIP:
				CreateInventory(FIGURE_EQUIP, & m_vpInventory[FIGURE_EQUIP]);
				break;
			case FIGURE_FASHION:
				CreateInventory(FIGURE_FASHION, & m_vpInventory[FIGURE_FASHION]);
				break;
			case FIGURE_BASIC:
				CreateInventory(FIGURE_BASIC, & m_vpInventory[FIGURE_BASIC]);
				break;
			default :
				break;
		}
	}

	// init effect figure
	CreateInventory( FIGURE_EFFECT, &m_vpInventory[FIGURE_EFFECT] );
}


void SPPlayerInvenArchive::InitStorage()
{
	m_iInventorySlot[STORAGE] = 4;
	m_iExtendSlot[STORAGE] = 0;
	m_iInvenPage[STORAGE] = 0;
	m_iCurInvenPage[STORAGE] = 0;
	
	CreateInventory(STORAGE, &m_vpInventory[STORAGE], DEFAULT_STORAGE_SIZE);
	
	g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_STORAGE_INITSLOT);
}

void SPPlayerInvenArchive::InitCashItem()
{
	CreateInventory(INVENTORY_CASH, &m_vpInventory[INVENTORY_CASH], MAX_INVENTORY_SLOT);
}

void SPPlayerInvenArchive::InitWearItem()
{
	CreateInventory(INVENTORY_WEAR, &m_vpInventory[INVENTORY_WEAR], MAX_INVENWEAR_SLOT);
}

void SPPlayerInvenArchive::InitPetEquip()
{
	m_iInventorySlot[FIGURE_PET] = DEFAULT_PET_SLOT;
	m_iExtendSlot[FIGURE_PET] = 0;
	m_iInvenPage[FIGURE_PET] = 0;
	m_iCurInvenPage[FIGURE_PET] = 0;
	CreateInventory(FIGURE_PET, &m_vpInventory[FIGURE_PET], MAX_PET_INVENTORY_SLOT);
}


bool SPPlayerInvenArchive::CreateInventory(CONTAINER_TYPE iType, 
										   std::vector<SPItem*>* vpInventory, 
										   int iSize /*= DEFAULT_INVENTORY_SLOT*/)
{	
	int i = 0;
	int iIndex = 0;												//³»ºÎ ÀÎµ¦½º º¸Á¤¿ë	
	
	if(iType < FIGURE_EQUIP) {
		iIndex = static_cast<int>(vpInventory->size());
		for(i = 0; i < iSize ; i++) {
			SPItem* pItem = new SPItem(iType, i + iIndex);
			vpInventory->push_back(pItem);
		}
	}
	else if(iType == STORAGE) {
		iIndex = static_cast<int>(vpInventory->size());
		for(i = 0; i < iSize ; i++) {
			SPItem* pItem = new SPItem(iType, i + iIndex);
			vpInventory->push_back(pItem);
		}
	}
	else if(iType == INVENTORY_CASH) {
		iIndex = static_cast<int>(vpInventory->size());
		for(i = 0; i < iSize ; i++) {
			SPItem* pItem = new SPItem(iType, i + iIndex);
			vpInventory->push_back(pItem);
		}
	}
	else if(iType == INVENTORY_WEAR) {
		iIndex = static_cast<int>(vpInventory->size());
		for(i = 0; i < iSize ; i++) {
			SPItem* pItem = new SPItem(iType, i + iIndex);
			vpInventory->push_back(pItem);
		}
	}
	else if(iType == FIGURE_PET) {
		iIndex = static_cast<int>(vpInventory->size());
		for(i = 0; i < iSize ; i++) {
			SPItem* pItem = new SPItem(iType, i + iIndex);
			vpInventory->push_back(pItem);
		}
	}
	else {
		for(i = 0; i < DEFAULT_EQUIP_SLOT; i++ ){
			SPItem* pItem = new SPItem(iType, i);
			vpInventory->push_back(pItem);
		}
	}	
	
	return true;
}


//////////////////////////////////////////////////////////////////////////
void	SPPlayerInvenArchive::Clear()
{
	//PrintInventoryInfo();
	ClearAllInventory();
	ClearAllFigure();
	ClearAllStorage();
	ClearCashItem();
	ClearWearItem();
	ClearPetEquip();
	//PrintInventoryInfo();
}


void SPPlayerInvenArchive::ClearAllInventory()
{	
	std::vector<SPItem*>::iterator iter;
	for(int i = 0 ; i < FIGURE_EQUIP; i++) {
		iter = m_vpInventory[i].begin();
		for(; iter != m_vpInventory[i].end(); ++iter) {
			SAFE_DELETE(*iter);
		}
		m_vpInventory[i].clear();
	}

	if(m_pParent->IsLocalPlayer()) {
		m_iCurrentContainer = INVENTORY_EQUIP;	
		m_iCurrentInventoryType = INVENTORY_EQUIP;
		SetItemScroll(m_iCurrentContainer);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SETDEFAULT, (WPARAM)0, (LPARAM)0);
	}
	
	//·ÎÄÃ ÇÃ·¹ÀÌ¾îÀÇ °æ¿ì ÀÎÅÍÆäÀÌ½º»óÀÇ ¾ÆÀÌÄÜ Á¤¸®µµ ÇØÁØ´Ù
	if(m_pParent->IsLocalPlayer()) {
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_ALLCLEAR, 0, 0, NULL);	

		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MICROPHONE);
		if(pWindow) {
			pWindow->Hide();
		}
		pWindow = NULL;													//[liuyang, 2008.8.29]
		pWindow = g_pInterfaceManager->FindWindow( WIID_FRIEND_FIND );
		if ( pWindow )
		{
			pWindow->Hide();
		}
		pWindow = NULL;
		pWindow = g_pInterfaceManager->FindWindow( WIID_FRIEND_FOLLOW );
		if ( pWindow )
		{
			pWindow->Hide();
		}
		pWindow = NULL;
		pWindow = g_pInterfaceManager->FindWindow( WIID_FRIEND_SNATCH );
		if ( pWindow )
		{
			pWindow->Hide();
		}
	}	
}


void SPPlayerInvenArchive::ClearAllFigure()
{
	std::vector<SPItem*>::iterator iter;

	for(int i = FIGURE_EQUIP ; i < STORAGE ; i++) {
		iter = m_vpInventory[i].begin();
		
		for(; iter != m_vpInventory[i].end(); ++iter) {
			SAFE_DELETE(*iter);
		}
		m_vpInventory[i].clear();
	}

	//·ÎÄÃ ÇÃ·¹ÀÌ¾îÀÇ °æ¿ì ÀÎÅÍÆäÀÌ½º»óÀÇ ¾ÆÀÌÄÜ Á¤¸®µµ ÇØÁØ´Ù
	if(m_pParent->IsLocalPlayer()) {
		g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_ALLCLEAR, 0,0, NULL);
		g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR1, 0, 0, NULL);
		g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR1, 0, 1, NULL);
		g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR2, 0, 0, NULL);
		g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR2, 0, 1, NULL);		
	}
}


void SPPlayerInvenArchive::ClearAllStorage()
{
	std::vector<SPItem*>::iterator iter;
	iter = m_vpInventory[STORAGE].begin();
	for(; iter != m_vpInventory[STORAGE].end(); ++iter) {
		SAFE_DELETE(*iter);
	}
	m_vpInventory[STORAGE].clear();

	//·ÎÄÃ ÇÃ·¹ÀÌ¾îÀÇ °æ¿ì ÀÎÅÍÆäÀÌ½º»óÀÇ ¾ÆÀÌÄÜ Á¤¸®µµ ÇØÁØ´Ù
	if(m_pParent->IsLocalPlayer()) {
		g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_ALLCLEAR, 0,0, NULL);
	}
}

void SPPlayerInvenArchive::ClearCashItem()
{
	std::vector<SPItem*>::iterator iter;
	iter = m_vpInventory[INVENTORY_CASH].begin();
	for(; iter != m_vpInventory[INVENTORY_CASH].end(); ++iter) {
		SAFE_DELETE(*iter);
	}
	m_vpInventory[INVENTORY_CASH].clear();
}

void SPPlayerInvenArchive::ClearWearItem()
{
	std::vector<SPItem*>::iterator iter;
	iter = m_vpInventory[INVENTORY_WEAR].begin();
	for(; iter != m_vpInventory[INVENTORY_WEAR].end(); ++iter) {
		SAFE_DELETE(*iter);
	}
	m_vpInventory[INVENTORY_WEAR].clear();
}

void SPPlayerInvenArchive::DeleteWearItem()
{
	std::vector<SPItem*>::iterator iter;
	iter = m_vpInventory[INVENTORY_WEAR].begin();
	for(; iter != m_vpInventory[INVENTORY_WEAR].end(); ++iter) {
		(*iter)->SetClear();
	}
}

void SPPlayerInvenArchive::ClearPetEquip()
{
	std::vector<SPItem*>::iterator iter;
	iter = m_vpInventory[FIGURE_PET].begin();
	for(; iter != m_vpInventory[FIGURE_PET].end(); ++iter) {
		SAFE_DELETE(*iter);
	}
	m_vpInventory[FIGURE_PET].clear();

	if(m_pParent->IsLocalPlayer()) {
		g_pInterfaceManager->GetPetWindow()->SPSendMessage(SPIM_ITEM_ALLCLEAR, 0,0, NULL);
	}
}


void SPPlayerInvenArchive::DecreaseLifeTime(int iTime /*= 60*/)
{
	std::vector<SPItem*>::iterator iter;
	for(int i = 0; i < CONTAINER_MAX; i++) {
		iter = m_vpInventory[i].begin();
		for(; iter != m_vpInventory[i].end(); ++iter) {
			if((*iter)->IsEmptyItem())
				continue;

			if((*iter)->GetItemAttr()->GetDurationType() == ITEM_DURATION_NULL) 
				continue;

			//ÀÌ°æ¿ì ¾ÆÀÌÅÛ ¶óÀÌÇÁ Å¸ÀÓÀÌ Á¸ÀçÇÑ´Ù 
			(*iter)->DecreaseLifeTime(iTime);
		}
	}
}


void SPPlayerInvenArchive::SetItemScroll(CONTAINER_TYPE iContainerType /*= CONTAINER_TYPE_INVALID*/)
{
	if(iContainerType == CONTAINER_TYPE_INVALID) {
		iContainerType == m_iCurrentInventoryType; 	
	}
	
	if(iContainerType == m_iCurrentInventoryType) {
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SET_MAXPAGE, (WPARAM)iContainerType, (LPARAM)m_iInvenPage[iContainerType] + 1, NULL);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SET_PAGE, (WPARAM)iContainerType, (LPARAM)m_iCurInvenPage[iContainerType], NULL);
	}	
}


void SPPlayerInvenArchive::PrintInventoryInfo()
{
#ifdef _DEBUG
	
	int iSize = 0;
	for(int i = 0 ; i < CONTAINER_MAX ; i++) {
		iSize = static_cast<int>(m_vpInventory[i].size());
		DXUTOutputDebugString("index[%d] size[%d]\t", i, iSize);
	}
	DXUTOutputDebugString("..\n");
#endif
}


int SPPlayerInvenArchive::GetActiveWeaponSlot() 
{	
	return m_iCurrentWeaponSlot;	
}


void SPPlayerInvenArchive::SetActiveWeaponSlot(int iCurSlot)
{
	if(iCurSlot < 1 || iCurSlot > 2) {
		return ;
	}
	m_iCurrentWeaponSlot = iCurSlot;
	DXUTOutputDebugString("Inven::SetActiveWeaponSlot [%d]\n", m_iCurrentWeaponSlot);
}


void	SPPlayerInvenArchive::SetCurrentItemPage(CONTAINER_TYPE item)
{
	m_iCurrentInventoryType = item;
	m_iCurrentContainer = item;
}


void	SPPlayerInvenArchive::SetCurrentEquipPage(CONTAINER_TYPE item)
{
	m_iCurrentInventoryType = item;
	m_iCurrentContainer = item;
}


SPSelectItem SPPlayerInvenArchive::GetPickupItem()
{	
	return m_PickUpItem;	
}


bool SPPlayerInvenArchive::IsFullInventory(CONTAINER_TYPE iContainerType)
{	
	if(IsEnableContainer(iContainerType) == false)
		return false;
	
	std::vector<SPItem*>::iterator iter;
	if(m_vpInventory[iContainerType].empty())			return false;	

	iter = m_vpInventory[iContainerType].begin();	
	for(; iter != m_vpInventory[iContainerType].end(); ++iter) {
		int iItemID = (*iter)->GetItemStatus()->SPSendMessage(MV_ITEM_GETUID);
		
		if(iItemID == 0)
			return false;
	}
	return true;
}


bool SPPlayerInvenArchive::IsEmptyInventory(CONTAINER_TYPE iContainerType)
{
	if(IsEnableContainer(iContainerType) == false)
		return false;
	
	std::vector<SPItem*>::iterator iter;
	if(m_vpInventory[iContainerType].empty())			return false;

	iter = m_vpInventory[iContainerType].begin();	
	for(; iter != m_vpInventory[iContainerType].end(); ++iter) {
		if((*iter)->IsEmptyItem() == false)
			return false;
	}

	return true;
}


bool SPPlayerInvenArchive::IsEnableSpace(CONTAINER_TYPE iContainerType, int iItemID, int iStack /*= 1*/)
{
	if(iContainerType < INVENTORY_EQUIP || iContainerType > INVENTORY_HOUSING)
		return false;

	if(iItemID == 0)
		return false;

	std::vector<SPItem*>::iterator iter = m_vpInventory[iContainerType].begin();
	for(; iter != m_vpInventory[iContainerType].end(); ++iter) {
		int iCheckItemID = (*iter)->GetItemStatus()->SPSendMessage(MV_ITEM_GETUID);
		
		//ItemID¸¦ ÀÌ¿ëÇÑ Ã¼Å©
		if(iCheckItemID == iItemID) {
			//Stack Ã¼Å©
			int iStackLimit = (*iter)->GetItemStatus()->GetItemAttr()->m_iStackLimit;
			int iCurrentStack = (*iter)->GetItemStatus()->SPSendMessage(MV_ITEM_GETSTACK_CNT);
			if(iCurrentStack + iStack <= iStackLimit)
				return true;
		}
	}

	return false;
}


int SPPlayerInvenArchive::GetEmptySlotCount(CONTAINER_TYPE iContainerType)
{
	int iEmptyCount = 0;	
	
	if( iContainerType >= FIGURE_EQUIP && iContainerType <= STORAGE )
		return iEmptyCount;

	std::vector<SPItem*>::iterator iter = m_vpInventory[iContainerType].begin();
	for(; iter != m_vpInventory[iContainerType].end(); ++iter) {
		if((*iter)->IsEmptyItem())
			iEmptyCount++;
	}

	return iEmptyCount;
}


bool SPPlayerInvenArchive::FindInvenItem(int iItemID, int iStack /*= 1*/, SPSelectItem* pSelectItem /*= NULL*/)
{
	if(iItemID == 0 || (iStack < 1 || iStack > 99) || pSelectItem == NULL)
		return false;

	SPItemAttr* pItemAttr = NULL;
	g_pItemCluster->GetItemInfo(iItemID, pItemAttr);
	if(pItemAttr == NULL)
		return false;

	CONTAINER_TYPE iContainerType = (CONTAINER_TYPE)(pItemAttr->m_eType - 1); 
	
	if(iContainerType < INVENTORY_EQUIP || iContainerType > INVENTORY_HOUSING)
		return false;

	std::vector<SPItem*>::iterator iter = m_vpInventory[iContainerType].begin();
	for(; iter != m_vpInventory[iContainerType].end(); ++iter) {
		if((*iter)->IsEmptyItem())
			continue;
		
		if((*iter)->GetItemStatus()->GetItemID() == iItemID && 
			(*iter)->GetItemStatus()->GetStackCount() >= iStack)
		{
			pSelectItem->SetSelectItem(iContainerType, (*iter)->GetSlotIndex());
			return true;
		}
	}
	
	return false;
}


SPItem* SPPlayerInvenArchive::GetFindItem(int iItemID, int iStack /*= 1*/, int iRare /*= 0*/, int iBlockIndex /*= MAX_ITEM_BLOCK*/)
{
	if(iItemID == 0 || (iStack < 1 || iStack > 99))
		return NULL;
	
	SPItemAttr* pItemAttr = NULL;
	g_pItemCluster->GetItemInfo(iItemID, pItemAttr);
	if(pItemAttr == NULL)
		return NULL;

	CONTAINER_TYPE iContainerType = (CONTAINER_TYPE)(pItemAttr->m_eType - 1); 

	if(iContainerType < INVENTORY_EQUIP || iContainerType > INVENTORY_HOUSING)
		return NULL;

	std::vector<SPItem*>::reverse_iterator rIter = m_vpInventory[iContainerType].rbegin();
	for(; rIter != m_vpInventory[iContainerType].rend(); ++rIter) {
		if((*rIter)->IsEmptyItem())
			continue;

		if((*rIter)->GetItemStatus()->GetItemID() == iItemID && 
			(*rIter)->GetItemStatus()->GetStackCount() >= iStack)
		{
			if(iRare > 0) {
				if((*rIter)->GetItemStatus()->GetRareLevel() == iRare) {
					if(iBlockIndex >= 0 && iBlockIndex < MAX_ITEM_BLOCK) {
						if(SetBlockItem(iBlockIndex, iContainerType, (*rIter)->GetSlotIndex()))
							return (*rIter);
					}
				}
			}
			else {
				if(iBlockIndex >= 0 && iBlockIndex < MAX_ITEM_BLOCK) {
					if(SetBlockItem(iBlockIndex, iContainerType, (*rIter)->GetSlotIndex()))
						return (*rIter);
				}
			}			
		}
	}

	return NULL;
}


SPItem * SPPlayerInvenArchive::GetInvenItem(int iItemID)
{
	if( iItemID == 0 )
		return NULL;

	SPItemAttr* pItemAttr = NULL;
	g_pItemCluster->GetItemInfo(iItemID, pItemAttr);
	if(pItemAttr == NULL)
		return NULL;

	CONTAINER_TYPE iContainerType = (CONTAINER_TYPE)(pItemAttr->m_eType -1); 
	
	if(iContainerType < INVENTORY_EQUIP || iContainerType > INVENTORY_HOUSING)
		return NULL;

	std::vector<SPItem*>::iterator iter = m_vpInventory[iContainerType].begin();
	for(; iter != m_vpInventory[iContainerType].end(); ++iter)
	{
		if((*iter)->IsEmptyItem())
			continue;
		
		if( (*iter)->GetItemStatus()->GetItemID() == iItemID )
		{
			return *iter;
		}
	}
	
	return NULL;
}

int SPPlayerInvenArchive::GetSetItemPieces(int iItemID)
{
	SPItemAttr* pItemAttr = NULL;
	g_pItemCluster->GetItemInfo(iItemID, pItemAttr);
	if(pItemAttr == NULL)
		return 0;

	int iSetID = pItemAttr->m_iSetID;

	return GetTotalSetItemPieces(iSetID);
}


bool SPPlayerInvenArchive::IsSetItem(int iItemID)
{
	bool bRet = false;
	
	SPItemAttr* pItemAttr = NULL;
	g_pItemCluster->GetItemInfo(iItemID, pItemAttr);
	if(pItemAttr == NULL)
		return bRet;

	if(pItemAttr->m_iSetID == 0)
		return bRet;
	
	return g_pItemCluster->IsItemSet(pItemAttr->m_iSetID);
}


void SPPlayerInvenArchive::CheckSetItem(int iItemID)
{
	if(IsSetItem(iItemID) == false)	
		return;

	int iTotalPieces = GetSetItemPieces(iItemID);
	if(iTotalPieces < 1)
		return;

	SPItemAttr* pItemAttr = NULL;
	g_pItemCluster->GetItemInfo(iItemID, pItemAttr);
	if(pItemAttr == NULL)
		return;
	
	int iSetID = pItemAttr->m_iSetID;

	SPItemSet* pItemSet = NULL;
	pItemSet = g_pItemCluster->GetItemSet(iSetID);
	if(pItemSet) {
		pItemSet->SetCurrentPieces(iTotalPieces);
	}
}


//////////////////////////////////////////////////////////////////////////
// Send
int		SPPlayerInvenArchive::SendReqLooting(LOOTING_ITEM item)
{	
	return 0;			//[2005/11/4]
	
	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_LOOTING);
	Packet.AddData(&item, sizeof(LOOTING_ITEM));
	return g_pNetworkManager->SPPerformMessage(ITEM_CS_LOOTING, 0, (LPARAM)&Packet );
}


int		SPPlayerInvenArchive::SendReqChangeSlot(CHANGE_SLOT item)
{
	item.SrcSlotInfo.SlotIndex = item.SrcSlotInfo.SlotIndex + 1 ;		//[2005/6/24]
	item.DestSlotInfo.SlotIndex = item.DestSlotInfo.SlotIndex + 1;		//[2005/6/24]

	if(g_pCheckManager->IsDBBlock()) {
		
		DXUTOutputDebugString("\tSPPlayerInvenArchive::SendReqChangeSlot DB Working Block\n");
		RecoverIcon();
		return 0;
	}
	
	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_CHANGE_SLOT);
	Packet.AddData(&item, sizeof(CHANGE_SLOT));	
	return g_pNetworkManager->SPPerformMessage(ITEM_CS_CHANGE_SLOT, 0, (LPARAM)&Packet );	
}


int SPPlayerInvenArchive::SendReqDivide(CHANGE_SLOT item)			// ¾ÆÀÌÅÛ ºÐÇÒÀÇ °æ¿ì ¿äÃ»
{
	item.SrcSlotInfo.SlotIndex = item.SrcSlotInfo.SlotIndex + 1 ;		//[2005/6/24]
	item.DestSlotInfo.SlotIndex = item.DestSlotInfo.SlotIndex + 1;		//[2005/6/24]

	if(g_pCheckManager->IsDBBlock()) {
		
		DXUTOutputDebugString("\tSPPlayerInvenArchive::SendReqDivide DB Working Block\n");
		RecoverIcon();
		return 0;
	}

	g_pCheckManager->SetDBBlock(true);

	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_DIVIDE);
	Packet.AddData(&item, sizeof(CHANGE_SLOT));	
	return g_pNetworkManager->SPPerformMessage(ITEM_CS_DIVIDE, 0, (LPARAM)&Packet );	
}


int		SPPlayerInvenArchive::SendReqTrash(TRASH_ITEM item)
{
	item.iContainerSlot.SlotIndex = item.iContainerSlot.SlotIndex + 1;	//[2005/6/24]
	
	if(g_pCheckManager->IsDBBlock()) {
		
		DXUTOutputDebugString("\tSPPlayerInvenArchive::SendReqTrash DB Working Block\n");
		RecoverIcon();
		return 0;
	}

	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_TRASH);
	Packet.AddData(&item, sizeof(TRASH_ITEM));
	return g_pNetworkManager->SPPerformMessage(ITEM_CS_TRASH, 0, (LPARAM)&Packet );
}


int		SPPlayerInvenArchive::SendReqWeaponExchange()
{
	if((((SPPlayer*)m_pParent)->IsWeaponChangeEnable()) == false)	
		return 0;
	
	if(m_bPickup) return 0;

	if( ((SPPlayer*)m_pParent)->IsLocalPlayer() )
		((SPPlayer*)m_pParent)->SetWeaponSwap(true);

	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_TOGGLE_USE_WEAPON_SET);	
	return g_pNetworkManager->SPPerformMessage(ITEM_CS_TOGGLE_USE_WEAPON_SET, 0, (LPARAM)&Packet );
}



//////////////////////////////////////////////////////////////////////////
//
int		SPPlayerInvenArchive::OnSendMessage(SPGM msg, LPARAM lParam)
{
	//
	//DXUTOutputDebugString("SPPlayerInvenArchive::OnSendMessage[%d] lParam[%d]\n", msg, (int)lParam);
	
	switch(msg)
	{
		case SPGM_ITEM_CS_TRASH:			SetTrash();					break;
		case SPGM_ITEM_SC_CONTAINER_INFO:	OnSetInventory(lParam);		break;
		case SPGM_ITEM_SC_CONTAINER_INFOS:	OnSetInventoryEx(lParam);	break;
		case SPGM_ITEM_SC_CHANGE_SLOT:		OnSetChangeSlot(lParam);	break;
		case SPGM_ITEM_SC_DIVIDE:			OnSetDivideSlot(lParam);	break;
		case SPGM_ITEM_SC_TRASH:			OnSetTrash(lParam);			break;
		case SPGM_ITEM_SC_DESTROY_BY_TIME:	OnSetTrashByTime(lParam);	break;
		case SPGM_ITEM_SC_EXPIRE:			OnSetExpire(lParam);		break;
		case SPGM_ITEM_SC_ITEMEXP:			OnSetExp(lParam);			break;
		case SPGM_ITEM_SC_ADDSP:			OnAddSp(lParam);			break;
		case SPGM_ITEM_SC_ALARM:			OnSetExceptionMsg(lParam);	break;
		case SPGM_ITEM_SC_USING:			OnSetUsing(lParam);			break;
		case SPGM_ITEM_SC_ADDTIME:			OnAddTime(lParam);			break;
		case SPGM_ITEM_SC_CHAT:				OnSetChatUsing(lParam);		break;
		case SPGM_ITEM_SC_WHERE:			OnSetFindUsing( lParam );   break;
		case SPGM_ITEM_SC_TOGGLE_USE_WEAPON_SET:	WeaponToggle();		break;
		case SPGM_ITEM_SC_USE_WEAPON_SET:	SetActiveWeaponSlot(lParam);	break;
		case SPGM_ITEM_SC_USE_WEAPON_CHANGE: WeaponChange();				break;		

		case SPGM_ITEM_CLICK_EQUIP:			OnChangeInventory(INVENTORY_EQUIP);		break;
		case SPGM_ITEM_CLICK_CONSUME:		OnChangeInventory(INVENTORY_CONSUME);	break;
		case SPGM_ITEM_CLICK_ETC:			OnChangeInventory(INVENTORY_ETC);		break;
		case SPGM_ITEM_CLICK_EVENT:			OnChangeInventory(INVENTORY_EVENT);		break;
		case SPGM_ITEM_CLICK_PET:			OnChangeInventory(INVENTORY_PET);		break;
		case SPGM_ITEM_CLICK_HOUSING:		OnChangeInventory(INVENTORY_HOUSING);	break;

		case SPGM_ITEM_REFRESH_GRID:		SettingCurrentInventory();				break;
		case SPGM_EQUIP_REFRESH_GRID:		SettingCurrentEquip();					break;		

		////////////////////////////////////////////////////////////////////////////
		case SPGM_ITEM_GRID_CLICK_DOWN:			ItemGridClickDown(int(lParam));		break;
		case SPGM_ITEM_GRID_CLICK_UP:			ItemGridClickUp(int(lParam));		break;
		case SPGM_ITEM_CURSOR_MOVE:				ItemGridCursorMove(lParam);			break;
		case SPGM_ITEM_GRID_DBK:				ItemGridDBK(int(lParam));			break;
		case SPGM_ITEM_PAGE_UP:					ItemPageUp();						break;
		case SPGM_ITEM_PAGE_DOWN:				ItemPageDown();						break;
		case SPGM_ITEM_SETPAGE:					ItemSetPage(int(lParam));			break;		
		
		case SPGM_EQUIP_GRID_CLICK_DOWN:		EquipGridClickDown(int(lParam));	break;
		case SPGM_EQUIP_GRID_CLICK_UP:			EquipGridClickUp(int(lParam));		break;
		case SPGM_EQUIP_GRID_CLICK_MOVE:		EquipGridCursorMove(lParam);		break;
		case SPGM_EQUIP_GRID_RCLICK_UP:			EquipGridRClickUp(int(lParam));		break;
		
		case SPGM_EQUIP_WEAPON1_CLICK_DOWN:		Weapon1ClickDown(int(lParam));		break;
		case SPGM_EQUIP_WEAPON1_CLICK_UP:		Weapon1ClickUp(int(lParam));		break;
		case SPGM_EQUIP_WEAPON1_CLICK_MOVE:		Weapon1CursorMove(lParam);			break;
		case SPGM_EQUIP_WEAPON1_RCLICK_UP:		Weapon1RClickUp(int(lParam));		break;
		
		case SPGM_EQUIP_WEAPON2_CLICK_DOWN:		Weapon2ClickDown(int(lParam));		break;
		case SPGM_EQUIP_WEAPON2_CLICK_UP:		Weapon2ClickUp(int(lParam));		break;
		case SPGM_EQUIP_WEAPON2_CLICK_MOVE:		Weapon2CursorMove(lParam);			break;
		case SPGM_EQUIP_WEAPON2_RCLICK_UP:		Weapon2RClickUp(int(lParam));		break;

		case SPGM_STORAGE_CURSOR_MOVE:			StorageCursorMove(lParam);			break;
		case SPGM_STORAGE_CLICK_DOWN:			StorageClickDown(int(lParam));		break;
		case SPGM_STORAGE_CLICK_UP:				StorageClickUp(int(lParam));		break;
		case SPGM_STORAGE_PAGE_UP:				StoragePageUp();					break;
		case SPGM_STORAGE_PAGE_DOWN:			StoragePageDown();					break;
		case SPGM_STORAGE_SETPAGE:				StorageSetPage(int(lParam));		break;

		case SPGM_PET_CURSOR_MOVE:				PetCursorMove(lParam);				break;
		case SPGM_PET_CLICK_DOWN:				PetClickDown(int(lParam));			break;
		case SPGM_PET_CLICK_UP:					PetClickUp(int(lParam));			break;


		case SPGM_INVEN_RECOVER_ICON:			RecoverIcon();						break;

		case SPGM_NOTICE_YES:					OnYes(lParam);						break;
		case SPGM_NOTICE_NO:					OnNo(lParam);						break;
		
		////////////////////////////////////////////////////////////////////////////		

		case SPGM_ITEM_RCLICK_GRID:				ItemGridRClick(int(lParam));		break;//[2005/5/23] - ¾ÆÀÌÅÛ »ç¿ë

		case SPGM_EQUIP_CLICK_BATTLE:		OnChangeEquip( FIGURE_EQUIP );	break;
		case SPGM_EQUIP_CLICK_FASHION:		OnChangeEquip( FIGURE_FASHION ); break;	
		case SPGM_EQUIP_CLICK_EFFECT:		OnChangeEquip( FIGURE_EFFECT ); break;	
		
		//PC TRADE
		case SPGM_TRADE_SC_REQUEST:		OnTradeRequest(lParam);		break;
		case SPGM_TRADE_SC_RESPONSE:	OnTradeResponse(lParam);	break;
		case SPGM_TRADE_SC_PUSHMONEY:	OnTradePushMoney(lParam);	break;
		case SPGM_TRADE_SC_PUSHITEM:	OnTradePushItem(lParam);	break;
		case SPGM_TRADE_SC_POPITEM:		OnTradePopItem(lParam);		break;
		case SPGM_TRADE_SC_ACCEPT:		OnTradeAccept(lParam);		break;
		case SPGM_TRADE_SC_CANCEL:		OnTradeCancel(lParam);		break;
		case SPGM_TRADE_SC_WAIT:		OnTradeWait(lParam);		break;

		case SPGM_TRADE_SC_NPC:			OnNpcTrade(lParam);			break;

		case SPGM_FIGURE_INFOS_SC_FOR_SINGLE:	OnPlayerInfoEquip(lParam); break;
			
	}
	return 0;
}

void	SPPlayerInvenArchive::DeletePickupItem()
{
	m_bPickup = false;	
	g_pInterfaceManager->ClearMouse();

	//[2005/6/24]
	m_PickUpItem.Clear();
	m_iItemLClickIndex = -1;
	m_iEquipLClickIndex = -1;
	m_iStorageLClickIndex = -1;
	m_iPetLClickIndex = -1;
}



//////////////////////////////////////////////////////////////////////////
int 	SPPlayerInvenArchive::SetTrash()
{	
	if(m_bPickup) {
		if(m_PickUpItem.m_iContainer == FIGURE_EQUIP || m_PickUpItem.m_iContainer == FIGURE_FASHION
			|| m_PickUpItem.m_iContainer == FIGURE_PET || FIGURE_EFFECT == m_PickUpItem.m_iContainer) {
			RecoverIcon();
			return 1;
		}

		if((((SPPlayer*)m_pParent)->IsDead())){
			RecoverIcon();
			return 1;
		}

		//[8/1/2005] - Æ®·¹ÀÌµå ¿äÃ»À» ¹ÞÀº °æ¿ì¿¡´Â ¹«½ÃÇÔ.
		if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_RESPONSE) {
			g_pInterfaceManager->ForceReply();
		}

		//[2006/4/27] - ¸ÞÀÏ º¸³»´ÂÁß ¾ÆÀÌÅÛ ÆÄ±« ¾ÈµÊ.
		if(g_pEventManager->GetCutInState() == CUT_IN_STATE_MAIL) {
			RecoverIcon();
			return 1;
		}
		
		//[2006/2/17]
		SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
		if(pPickUpItem == NULL)	 {			
			RecoverIcon();
			return 1;
		}

		int iItemID = pPickUpItem->GetItemStatusValue(MV_ITEM_GETUID);
		if(iItemID == 0) {
			RecoverIcon();
			return 1;
		}

		//[2006/2/17]
		if(!pPickUpItem->GetItemAttr()->m_bDestroy) {
			RecoverIcon();
			return 1;
		}
		
		char szMsg[512];
		ZeroMemory(szMsg, 512);
		std::string strMsg = "";
		std::string strItemName = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex)->GetItemAttr()->m_strName;
		
		if(pPickUpItem->GetItemAttr()->m_bCashCheck) {
			if( pPickUpItem->GetItemAttr()->m_iCashPoint > 0 )
			{
				m_iNoticeUse = NOTICE_USE_ITEM_TRASH_FAIL;
				if(g_pResourceManager->GetGlobalString(4003006))
					strMsg = g_pResourceManager->GetGlobalString(4003006);
			}
			else
			{
				if(SPLocalizeManager::GetInstance()->GetMileageRatio() == 0)	{				//Æ÷ÀÎÆ®¸¦ »ç¿ëÇÏÁö ¾Ê´Â °æ¿ì
					m_iNoticeUse = NOTICE_USE_ITEM_TRASH;
					if(g_pResourceManager->GetGlobalString(4003007))
						strMsg = g_pResourceManager->GetGlobalString(4003007);
				}
				else {																			//Æ÷ÀÎÆ®¸¦ »ç¿ë
					if(pPickUpItem->GetItemStatus()->GetItemBackPrice() == 0) {					//Æ÷ÀÎÆ®°¡ 0ÀÎ°æ¿ì
						m_iNoticeUse = NOTICE_USE_ITEM_TRASH;
						if(g_pResourceManager->GetGlobalString(4003005))
							strMsg = g_pResourceManager->GetGlobalString(4003005);
					}
					else if(pPickUpItem->GetItemStatus()->GetItemBackPrice() > 0){				//Æ÷ÀÎÆ®°¡ ³²Àº °æ¿ì
						m_iNoticeUse = NOTICE_USE_ITEM_TRASH_FAIL;
						if(g_pResourceManager->GetGlobalString(4003006))
							strMsg = g_pResourceManager->GetGlobalString(4003006);
					}
				}			
			}
		}
		else {																				//º¸Åë ¾ÆÀÌÅÛÀÎ °æ¿ì
			m_iNoticeUse = NOTICE_USE_ITEM_TRASH;
			if(g_pResourceManager->GetGlobalString(4003000))
				strMsg = g_pResourceManager->GetGlobalString(4003000);
		}		
		
		//ÆÇ¸Å ·Î ¼³Á¤µÈ ¾ÆÀÌÅÛ ¸µÅ©¸¦ ±â¾ï
		m_TrashItem.SetSelectItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
		if(m_iNoticeUse == NOTICE_USE_ITEM_TRASH_FAIL) {
			wsprintf(szMsg, strMsg.c_str());
			g_pInterfaceManager->SetNotice(szMsg, this->m_pParent, NOTICE_TYPE_YES);
		}
		else {
			wsprintf(szMsg, strMsg.c_str(), strItemName.c_str());
			g_pInterfaceManager->SetNotice(szMsg, this->m_pParent);
		}				

		//DeletePickupItem();
		g_pInterfaceManager->ClearMouse();		//¸¶¿ì½º À§¿¡ ¿Ã¶ó°£ ¾ÆÀÌÄÜ¸¸ »èÁ¦
	}
	
	return 1;
}


int SPPlayerInvenArchive::OnYes(LPARAM lParam)
{
	//
	int iPartition = (int)lParam;
	//DXUTOutputDebugString("SPPlayerInvenArchive::OnYes [%d]\n", iPartition);
	
	if(/*m_bPickup &&*/m_iNoticeUse == NOTICE_USE_ITEM_TRASH && iPartition == 0 && m_bRequested == false)	//Item ÆÄ±«
	{
		//
		if(m_TrashItem.m_iContainer == CONTAINER_TYPE_INVALID || m_TrashItem.m_iSlotIndex == -1) {
			RecoverIcon();
			return false;
		}

		if(m_TrashItem.m_iContainer == FIGURE_EQUIP || m_TrashItem.m_iContainer == FIGURE_FASHION
			|| FIGURE_EFFECT == m_TrashItem.m_iContainer) {
			RecoverIcon();
			return false;
		}

		SPItem* pPickUpItem = GetInventoryItem(m_TrashItem.m_iContainer, m_TrashItem.m_iSlotIndex);
		if(pPickUpItem == NULL)	 {			
			RecoverIcon();
			return false;
		}
		
		int iItemID = pPickUpItem->GetItemStatusValue(MV_ITEM_GETUID);
		if(iItemID == 0) {
			RecoverIcon();
			return false;
		}

		//[2006/2/17]
		if(!pPickUpItem->GetItemAttr()->m_bDestroy) {
			RecoverIcon();
			return false;
		}

		TRASH_ITEM item;
		item.iContainerSlot.ContainerType = pPickUpItem->GetContainerType();
		item.iContainerSlot.SlotIndex = pPickUpItem->GetSlotIndex();
		item.iCount = pPickUpItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
		SendReqTrash( item );
		return true;
	}
	
	//Æ®·¹ÀÌµå ¿äÃ»ÀÀ´ä
	if(m_bRequested && g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_RESPONSE) {
		//SendÀÀ´ä ¸Þ½ÃÁö
		SendTradeResponse(true);
		m_bRequested = false;
		return true;
	}

	if(iPartition > 0) {							//Item ºÐÇÒ
		//³ª´­°¹¼ö°¡ ÀÚ½ÅÀÌ µé°í ÀÖ´Â °¹¼ö¿Í °°Àº °æ¿ì¿¡´Â ½½·Ô Ã¼ÀÎÁö·Î º¸³»¾ß ÇÑ´Ù 
		SPItem* pPickUpItem = GetInventoryItem(m_ChangeSlot.SrcSlotInfo.ContainerType, m_ChangeSlot.SrcSlotInfo.SlotIndex);
		if( pPickUpItem )
		{
			int iPickUpItemStack = pPickUpItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
			if(iPartition == iPickUpItemStack) {
				m_ChangeSlot.iCount = iPickUpItemStack;
				SendReqChangeSlot(m_ChangeSlot);
			}
			else {
				m_ChangeSlot.iCount = iPartition;
				SendReqDivide(m_ChangeSlot);
			}		
		}
	}

	if(m_iNoticeUse == NOTICE_USE_ITEM_TRASH_FAIL) {
		OnNo((LPARAM)0);
	}

	m_iNoticeUse = NOTICE_USE_NULL;

	return 1;
}


int SPPlayerInvenArchive::OnNo(LPARAM lParam)
{
	if(m_TrashItem.IsNull() == false){
		if(m_TrashItem.m_iContainer == m_iCurrentInventoryType) {
			int iTrashIndex = m_TrashItem.m_iSlotIndex - (m_iCurInvenPage[m_TrashItem.m_iContainer] * INVENTORY_LINE);			
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTrashIndex, (LPARAM)false, NULL);
		}
		
		m_TrashItem.Clear();
		RecoverIcon();
	}
	
	if(m_bRequested) {
		//SendÀÀ´ä ¸Þ½ÃÁö
		SendTradeResponse(false);
	}

	m_bRequested = false;
	m_iNoticeUse = NOTICE_USE_NULL;
	
	return 1;
}


//////////////////////////////////////////////////////////////////////////
// Receive
int		SPPlayerInvenArchive::OnSetInventory( LPARAM lParam )
{
	CPacket* pPacket = (CPacket*)lParam;
	CONTAINER_ITEM item;
	pPacket->ExtractStruct(&item, sizeof(CONTAINER_ITEM));

	assert(item.Item.iItemID != 0);

	item.SlotInfo.SlotIndex = item.SlotInfo.SlotIndex - 1;	//[2005/6/24]		
	ItemSetting(item, true);

	if(item.SlotInfo.ContainerType == INVENTORY_CONSUME)
		SendQuickSlotUpdate(item.Item.iItemID);

	//[2005/9/29]	Quest Mission Check...
	g_pEventManager->GetQuestManager()->CheckMissionItem(item.Item.iItemID);

	if( g_pSubGameManager && g_pSubGameManager->GetGameState() == GS_LOTTERY )
	{
		if( g_pInterfaceManager )
			g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_REFRESH_ITEMINVEN, 0, 0);
	}

	return 1;
}


int		SPPlayerInvenArchive::OnSetInventoryEx( LPARAM lParam )
{
	CPacket* pPacket = (CPacket*)lParam;

	UINT16	nNum;
	pPacket->ExtractUINT16(&nNum);

	for(int i=0; i<nNum; i++)
	{
		CONTAINER_ITEM item;
		pPacket->ExtractStruct(&item, sizeof(CONTAINER_ITEM));

		assert(item.Item.iItemID!=0);
		
		item.SlotInfo.SlotIndex = item.SlotInfo.SlotIndex - 1;	//[2005/6/24]		
		ItemSetting(item, true);

		if(item.SlotInfo.ContainerType == INVENTORY_CONSUME)
			SendQuickSlotUpdate(item.Item.iItemID);

		//[2005/9/29]	Quest Mission Check...
		g_pEventManager->GetQuestManager()->CheckMissionItem(item.Item.iItemID);		
	}

	ReflashInventory();	

	return 1;
}


int		SPPlayerInvenArchive::OnSetChangeSlot( LPARAM lParam )
{
	CPacket* pPacket = (CPacket*)lParam;

	CHANGE_SLOT item;
	pPacket->ExtractStruct(&item, sizeof(CHANGE_SLOT));

	item.DestSlotInfo.SlotIndex = item.DestSlotInfo.SlotIndex - 1;	//[2005/6/24]
	item.SrcSlotInfo.SlotIndex = item.SrcSlotInfo.SlotIndex - 1;	//[2005/6/24]

	//////////////////////////////////////////////////////////////////////////	
	bool bMergeEnabel = false;				//º´ÇÕ °¡´ÉÇÑÁö
	bool bDestItemExist = false;			//´ë»ó ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÏ´ÂÁö

	int iDestItemID = 0;
	int iDestStackCount = 0;
	int iDestMaxStack = 0;
	int iDestSlotIndex = item.DestSlotInfo.SlotIndex ;
	CONTAINER_TYPE iDestContainerType = item.DestSlotInfo.ContainerType;
	SPItem* pDestItem = GetInventoryItem(iDestContainerType, iDestSlotIndex);

	if(IsItemExist(iDestSlotIndex, iDestContainerType)) {
		iDestItemID = pDestItem->GetItemStatusValue(MV_ITEM_GETUID);	
		iDestStackCount = pDestItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
		iDestMaxStack = pDestItem->GetItemAttr()->m_iStackLimit;
		bDestItemExist = true;
	}

	int iSrcItemID = 0;
	int iSrcStackCount = 0;
	int iSrcMaxStack = 0;
	int iSrcSlotIndex = item.SrcSlotInfo.SlotIndex ;
	CONTAINER_TYPE iSrcContainerType = item.SrcSlotInfo.ContainerType;
	SPItem* pSrcItem = GetInventoryItem(iSrcContainerType, iSrcSlotIndex);

	iSrcItemID = pSrcItem->GetItemStatusValue(MV_ITEM_GETUID);
	iSrcStackCount = pSrcItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
	iSrcMaxStack = pSrcItem->GetItemAttr()->m_iStackLimit;	
	
	//[2005/10/20] ¸Â´Â ¹öÁ¯ÀÓ
	if(iDestStackCount < iDestMaxStack && iSrcStackCount < iSrcMaxStack) {
		bMergeEnabel = true;
	}
	
	//´ë»ó¾ÆÀÌÅÛÀÌ Á¸ÀçÇÏ°í ¼Ò½º¿Í ´ë»ó¾ÆÀÌÅÛÀÌ °°°í º´ÇÕÀÌ °¡´ÉÇÑ °æ¿ì¸¸ º´ÇÕÇÑ´Ù 
	if(/*(item.SrcSlotInfo.ContainerType == item.DestSlotInfo.ContainerType) &&*/ 
		bDestItemExist && iSrcItemID == iDestItemID && bMergeEnabel)
	{	//º´ÇÕ		
		SlotMerge(item);
	}
	else {																	//½½·Ô°£ ±³È¯
		SlotChange( item );		
	}

	if(iSrcContainerType == INVENTORY_CONSUME)					//Quick Slot Update
		SendQuickSlotUpdate(iSrcItemID);

	if( g_pSubGameManager && g_pSubGameManager->GetGameState() == GS_CASHSHOP )
	{
		if( g_pInterfaceManager )
			g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_REFRESH_ITEMINVEN, 0, 0);
	}
	else if( g_pSubGameManager && g_pSubGameManager->GetGameState() == GS_LOTTERY )
	{
		if( g_pInterfaceManager )
			g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_REFRESH_ITEMINVEN, 0, 0);
	}

	return 1;
}


int SPPlayerInvenArchive::OnSetDivideSlot(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	GLOBAL_STRING_ID	iResultError;		// 0: ¼º°ø 0º¸´Ù Å©¸é ¿¡·¯.
	TRASH_ITEM		TrashITem;				// ¼Ò½ºÀÇ Ä«¿îÆÃ »è°¨ Á¤º¸.
	CONTAINER_ITEM	NewItem;				// »õ·Î »ý¼ºµÈ ÀÎ½ºÅÏ½º ÄÁÅ×ÀÌ³Ê ¾ÆÀÌÅÛ.
	
	pPacket->ExtractUINT32(&iResultError);	

	if(iResultError == 0) {
		pPacket->ExtractStruct(&TrashITem, sizeof(TRASH_ITEM));
		pPacket->ExtractStruct(&NewItem, sizeof(CONTAINER_ITEM));

		TrashITem.iContainerSlot.SlotIndex = TrashITem.iContainerSlot.SlotIndex - 1;
		NewItem.SlotInfo.SlotIndex = NewItem.SlotInfo.SlotIndex - 1;		
		ItemSetting(NewItem, true);
		TrashItem(TrashITem);		
		
		//[2005/10/18]	Quest Mission Check...
		g_pEventManager->GetQuestManager()->CheckMissionItem(NewItem.Item.iItemID);
	}
	else {
		//¿¡·¯ Ã³¸®...
		if(g_pResourceManager->GetGlobalString(iResultError)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iResultError);	
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
	}

	g_pCheckManager->SetDBBlock(false);
	
	return 1;
}


int		SPPlayerInvenArchive::OnSetTrash( LPARAM lParam )
{	
	CPacket* pPacket = (CPacket*)lParam;
	TRASH_ITEM item;
	
	UINT8 iTrashCount = 0;

	pPacket->ExtractUINT8(&iTrashCount);
	
	for(int i = 0; i < iTrashCount; i++) {
		pPacket->ExtractStruct(&item, sizeof(TRASH_ITEM));
		item.iContainerSlot.SlotIndex = item.iContainerSlot.SlotIndex - 1;	
		
		if(TrashItem(item) == false) {
			DXUTOutputDebugString("TrashItem Error Count[%d] TrashItemCount[%d]\n", i, iTrashCount);
		}
	}	
	
	DeletePickupItem();

	//ÆÇ¸ÅÃ¢ »óÅÂ¸¦ ÃÊ±âÈ­ ÇÑ´Ù 
	ClearOutSideControl();
	
	m_SellItem.Clear();
	SettingCurrentInventory();

	g_pInterfaceManager->SetNoticeHide();
	m_iNoticeUse = NOTICE_USE_NULL;

	g_pInterfaceManager->SPPerformMessage(SPIM_LOOTING_ITEM_REDRAW, 0, (LPARAM)this);

	if( g_pSubGameManager && g_pSubGameManager->GetGameState() == GS_LOTTERY )
	{
		if( g_pInterfaceManager )
			g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_REFRESH_ITEMINVEN, 0, 0);
	}

	return 1;
}


int SPPlayerInvenArchive::OnSetTrashByTime( LPARAM lParam )
{
	CPacket* pPacket = (CPacket*)lParam;
	TRASH_ITEM item;
	pPacket->ExtractStruct(&item, sizeof(TRASH_ITEM));
	item.iContainerSlot.SlotIndex = item.iContainerSlot.SlotIndex - 1;	

	if(TrashItem(item) == false) {
		DXUTOutputDebugString("OnSetTrashByTime Error\n");
	}

	DeletePickupItem();

	//ÆÇ¸ÅÃ¢ »óÅÂ¸¦ ÃÊ±âÈ­ ÇÑ´Ù 
	ClearOutSideControl();
	
	m_SellItem.Clear();
	SettingCurrentInventory();

	g_pInterfaceManager->SetNoticeHide();
	m_iNoticeUse = NOTICE_USE_NULL;

	g_pInterfaceManager->SPPerformMessage(SPIM_LOOTING_ITEM_REDRAW, 0, (LPARAM)this);

	return 1;
}


int SPPlayerInvenArchive::OnSetExpire(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	CONTAINER_SLOT stItemPos;
	pPacket->ExtractStruct(&stItemPos, sizeof(CONTAINER_SLOT));

	stItemPos.SlotIndex--;				//Server 1º£ÀÌ½º Client 0º£ÀÌ½º
	
	SPItem* pItem = GetInventoryItem(stItemPos.ContainerType, stItemPos.SlotIndex);
	if(pItem && pItem->GetItemStatus() && pItem->GetItemStatus()->GetItemID() > 0) {
		int iDisAbleIndex = 0;
		bool bTimeAvaliable = true;
		bool bEnable = true;
		if(pItem->GetItemAttr()->GetDurationType() == ITEM_DURATION_NULL) {
			return 1;
		}		
		
		pItem->GetItemStatus()->SetTimeAvailable();
		bEnable = CheckPCStatus(pItem->GetItemStatus()->GetItemID());
		bTimeAvaliable = pItem->GetItemStatus()->IsTimeAvailable();

		// ÇÑ¹ø´õ ¾ÆÀÌÅÛ ¼ÂÆÃÀ» ÇØÁØ´Ù (´ëºÎºÐÀÇ ÀÛ¾÷Àº ÀÌ°÷¿¡¼­ ÀÌ·ç¾îÁø´Ù)
		CONTAINER_ITEM item;
		pItem->ConvertContainerItem((CONTAINER_ITEM*)&item);		
		ItemSetting(item);
		int iSlotCount = GetSlotCount(stItemPos.ContainerType, stItemPos.SlotIndex);		
		
		// ¸Þ½ÃÁö Ãâ·Â
		D3DXCOLOR color;
		if(pItem->GetItemAttr()->m_bNameColor) {
			color = pItem->GetItemAttr()->GetNameColor();
		}
		else {
			color = g_pCheckManager->GetItemColor(pItem->GetItemStatus()->GetRareLevel());
		}

		if(g_pResourceManager->GetGlobalString(4007001)) {
			char szMsg[256];
			ZeroMemory(szMsg, 256);
			sprintf(szMsg, g_pResourceManager->GetGlobalString(4007001), pItem->GetItemAttr()->m_strName.c_str());
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg, (LPARAM)&color);
		}		

		// ÀåºñÂÊÀÇ Ç¥Çö ÇØÁ¦ ¿ë
		if(stItemPos.ContainerType == FIGURE_EQUIP || stItemPos.ContainerType == FIGURE_FASHION
			|| FIGURE_EFFECT == stItemPos.ContainerType) {						
			if(iSlotCount == 3) {
				m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_ONE_PIECE);
				bool bEquipExist = IsItemExist(3, FIGURE_EQUIP);
				if(bEquipExist) {
					SPItem* pEquipItem = GetInventoryItem(FIGURE_EQUIP, 3);
					int iEquipItemID = pEquipItem->GetItemStatusValue(MV_ITEM_GETUID);
					bool bTimeAvaliable = pEquipItem->GetItemStatus()->IsTimeAvailable();
					if(iEquipItemID != 0 && bTimeAvaliable) {
						//Item ¸¸µé±â
						CONTAINER_ITEM item;
						item.Item.iItemID = iEquipItemID;
						item.Item.iStackCount = 1;
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
					}
				}
				else {
					m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(3 + 1));
				}

				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)4, NULL);	
				bEquipExist = IsItemExist(4, FIGURE_EQUIP);		//ÇÑ¹ú¿ÊÀ» ¹þÀº °æ¿ì Àåºñ·Î ·£´õ
				if(bEquipExist) {
					SPItem* pEquipItem = GetInventoryItem(FIGURE_EQUIP, 4);
					int iEquipItemID = pEquipItem->GetItemStatusValue(MV_ITEM_GETUID);
					bool bTimeAvaliable = pEquipItem->GetItemStatus()->IsTimeAvailable();
					if(iEquipItemID != 0 && bTimeAvaliable) {
						//Item ¸¸µé±â
						CONTAINER_ITEM item;
						item.Item.iItemID = iEquipItemID;
						item.Item.iStackCount = 1;
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
					}
				}
				else {
					m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(4 + 1));			
				}

				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)5, NULL);
				bEquipExist = IsItemExist(5, FIGURE_EQUIP);		//ÇÑ¹ú¿ÊÀ» ¹þÀº °æ¿ì Àåºñ·Î ·£´õ
				if(bEquipExist) {
					SPItem* pEquipItem = GetInventoryItem(FIGURE_EQUIP, 5);
					int iEquipItemID = pEquipItem->GetItemStatusValue(MV_ITEM_GETUID);
					bool bTimeAvaliable = pEquipItem->GetItemStatus()->IsTimeAvailable();
					if(iEquipItemID != 0 && bTimeAvaliable) {
						//Item ¸¸µé±â
						CONTAINER_ITEM item;
						item.Item.iItemID = iEquipItemID;
						item.Item.iStackCount = 1;
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
					}
				}
				else {				
					m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(5 + 1));	
				}
			}
			else {													//ÇÑ¹ú¿ÊÀÌ ¾Æ´Ñ °æ¿ì
				if(stItemPos.ContainerType == FIGURE_EQUIP) {
					if(stItemPos.SlotIndex < EQ_WEAPON1 - 1) {
						bool bFashionExist = false;						
						bFashionExist = IsItemExist(stItemPos.SlotIndex, FIGURE_FASHION);
						if(bFashionExist) {
							SPItem* pFashionItem = GetInventoryItem(FIGURE_FASHION, stItemPos.SlotIndex);
							int iFashionItemID = pFashionItem->GetItemStatusValue(MV_ITEM_GETUID);
							bool bTimeAvaliable = pFashionItem->GetItemStatus()->IsTimeAvailable();
							if(iFashionItemID != 0 && bTimeAvaliable) {
								//Item ¸¸µé±â
								CONTAINER_ITEM item;
								item.Item.iItemID = iFashionItemID;
								item.Item.iStackCount = 1;
								m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
							}
						}
						else {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(stItemPos.SlotIndex + 1));
						}
					}
					else {		//¹«±âÀÇ °æ¿ì
						if(stItemPos.SlotIndex == EQ_WEAPON1 - 1 || stItemPos.SlotIndex == EQ_BUCKLER1 - 1) {
							if(m_iCurrentWeaponSlot == 1)
								m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(stItemPos.SlotIndex + 1));
						}
						else if(stItemPos.SlotIndex == EQ_WEAPON2 - 1 || stItemPos.SlotIndex == EQ_BUCKLER2 - 1) {
							if(m_iCurrentWeaponSlot != 1) {
								if(stItemPos.SlotIndex == EQ_WEAPON2 - 1) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_WEAPON1);							
								}
								else if(stItemPos.SlotIndex == EQ_BUCKLER2 - 1)
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_BUCKLER1);
							}
						}
					}
				}
				else if(stItemPos.ContainerType == FIGURE_FASHION) {
					if(stItemPos.SlotIndex < EQ_WEAPON1 - 1) {
						bool bEquipExist = false;
						bEquipExist = IsItemExist(stItemPos.SlotIndex, FIGURE_EQUIP);
						if(bEquipExist) {
							SPItem* pEquipItem = GetInventoryItem(FIGURE_EQUIP, stItemPos.SlotIndex);
							int iEquipItemID = pEquipItem->GetItemStatusValue(MV_ITEM_GETUID);
							bool bTimeAvaliable = pEquipItem->GetItemStatus()->IsTimeAvailable();
							if(iEquipItemID != 0 && bTimeAvaliable) {
								//Item ¸¸µé±â
								CONTAINER_ITEM item;
								item.Item.iItemID = iEquipItemID;
								item.Item.iStackCount = 1;
								m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
							}
						}
						else {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(stItemPos.SlotIndex + 1));
						}
					}
				}
				else if (stItemPos.ContainerType == FIGURE_EFFECT)
				{
					if(stItemPos.SlotIndex < EQ_WEAPON1 - 1)
					{
						if (pItem && pItem->GetItemStatus())
							m_pParent->SPGOBPerformMessage( SPGM_ITEM_RTREFFECT_DELETE, (LPARAM)(pItem->GetItemStatus()->GetItemID()));
					}
					else
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(stItemPos.SlotIndex + 1));
				}
			}			
		}		
	}

	// ¾ÆÀÌÅÛ À¯È¿°¡ ¸¸·á µÇ¾úÀ»¶§ °ü·Ã Ã¢ ¾÷µ¥ÀÌÆ® [10/31/2006 AJJIYA]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_REFRESH , NULL , NULL );
	g_pInterfaceManager->SPChildWndSendMessage( WIID_SKILL , SPIM_REFRESH , NULL , NULL );

	return 1;
}


int SPPlayerInvenArchive::OnAddTime(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	
	int					iItemAddTimeID = 0;	
	GLOBAL_STRING_ID	iResultError = 0;
	ITEM_LIFETIME		stItemLifeTime;
	ZeroMemory(&stItemLifeTime, sizeof(ITEM_LIFETIME));
	
	pPacket->ExtractUINT32((UINT32*)&iItemAddTimeID);	
	pPacket->ExtractUINT32(&iResultError);	
	if(0 == iResultError) // ¼º°øÀÏ °æ¿ì¸¸

	{
		pPacket->Extract(&stItemLifeTime, sizeof(ITEM_LIFETIME));

		stItemLifeTime.stSlotInfo.SlotIndex--;						//Server 1º£ÀÌ½º Client 0º£ÀÌ½º
		
		SPItem* pItem = GetInventoryItem(stItemLifeTime.stSlotInfo.ContainerType, stItemLifeTime.stSlotInfo.SlotIndex);
		if(pItem && pItem->GetItemStatus() && pItem->GetItemStatus()->GetItemID() != 0) {            
			pItem->GetItemStatus()->SetLifeTime(stItemLifeTime.iLifeTime);		//±âÁ¸ÀÇ °Í¿¡ ´õÇØÁØ´Ù
		}

		CONTAINER_ITEM item;
		pItem->ConvertContainerItem(&item);

		ItemSetting(item);

		//Item TimeADD ÄÁÆ®·Ñ ½½·Ô ¾øµ¥ÀÌÆ® ÀÛ¾÷
		g_pInterfaceManager->SPChildWndSendMessage(WIID_ITEM_TIMEADD, SPIM_ITEM_TIMEADD_RESULT, (WPARAM)true, (LPARAM)0, true);		
		//g_pInterfaceManager->SPChildWndSendMessage(WIID_EFFECTITEM_UPGRADE, SPIM_EFFECTITEM_UPGRADE_RESULT, (WPARAM)true, (LPARAM)0, true);		

	}
	else {
		if(g_pResourceManager->GetGlobalString(iResultError)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iResultError);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );//»ÃÓ°
		}

		//Item TimeADD ÄÁÆ®·Ñ ½½·Ô ¾øµ¥ÀÌÆ® ÀÛ¾÷
		g_pInterfaceManager->SPChildWndSendMessage(WIID_ITEM_TIMEADD, SPIM_ITEM_TIMEADD_RESULT, (WPARAM)false, (LPARAM)0, true);
		//g_pInterfaceManager->SPChildWndSendMessage(WIID_EFFECTITEM_UPGRADE, SPIM_EFFECTITEM_UPGRADE_RESULT, (WPARAM)false, (LPARAM)0, true);//»ÃÓ°
	}

	return 1;
}


int SPPlayerInvenArchive::OnSetExp(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;

	ITEM_EXP		stItemExp;
	ZeroMemory(&stItemExp, sizeof(ITEM_EXP));
	pPacket->Extract(&stItemExp, sizeof(ITEM_EXP));
	
	stItemExp.stSlotInfo.SlotIndex-- ;					//¼­¹ö 1 º£ÀÌ½º, Å¬¶óÀÌ¾ðÆ® 0º£ÀÌ½º
	if(stItemExp.stSlotInfo.SlotIndex < 0){
		return 1;
	}

	if(IsItemExist(stItemExp.stSlotInfo.SlotIndex, stItemExp.stSlotInfo.ContainerType) == false) {
		return 1;
	}

	SPItem* pItem = GetInventoryItem(stItemExp.stSlotInfo.ContainerType, stItemExp.stSlotInfo.SlotIndex);
	if(pItem == NULL || pItem->GetItemStatus() == NULL || pItem->GetItemStatus()->GetItemID() == 0) {
		return 1;
	}

	pItem->GetItemStatus()->SetExp(stItemExp.iItemExp);	

	if(stItemExp.stSlotInfo.ContainerType == FIGURE_PET)
		g_pInterfaceManager->GetPetWindow()->SPSendMessage(SPIM_PET_STAT_UPDATE, (WPARAM)2, (LPARAM)stItemExp.stSlotInfo.SlotIndex);

	return 1;
}


int SPPlayerInvenArchive::OnAddSp( LPARAM lParam )
{
	CPacket* pPacket = (CPacket*)lParam;

	GLOBAL_STRING_ID	ErrorID = 0;
	ITEM_SP				stItemSP;
	ZeroMemory(&stItemSP, sizeof(ITEM_SP));

	pPacket->ExtractUINT32(&ErrorID);
	if(0 == ErrorID) // ¼º°øÀÌ¸é
	{
		pPacket->Extract(&stItemSP, sizeof(ITEM_SP));
		
		stItemSP.stSlotInfo.SlotIndex-- ;					//¼­¹ö 1 º£ÀÌ½º, Å¬¶óÀÌ¾ðÆ® 0º£ÀÌ½º
		if(stItemSP.stSlotInfo.SlotIndex < 0){
			return 1;
		}

		if(IsItemExist(stItemSP.stSlotInfo.SlotIndex, stItemSP.stSlotInfo.ContainerType) == false) {
			return 1;
		}

		SPItem* pItem = GetInventoryItem(stItemSP.stSlotInfo.ContainerType, stItemSP.stSlotInfo.SlotIndex);
		if(pItem == NULL || pItem->GetItemStatus() == NULL || pItem->GetItemStatus()->GetItemID() == 0) {
			return 1;
		}

		pItem->GetItemStatus()->SetSp(stItemSP.sItemSP);
		
		if(stItemSP.stSlotInfo.ContainerType == FIGURE_PET)
			g_pInterfaceManager->GetPetWindow()->SPSendMessage(SPIM_PET_STAT_UPDATE, (WPARAM)1, (LPARAM)stItemSP.stSlotInfo.SlotIndex);
	}
	else {
		if(g_pResourceManager->GetGlobalString(ErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
	}

	return 1;
}

//-------------------------------------------------------------------------------------------------------------
int SPPlayerInvenArchive::OnSetFindUsing( LPARAM lParam )
{
	return 1;
}
//-------------------------------------------------------------------------------------------------------------
int SPPlayerInvenArchive::OnSetChatUsing(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	GLOBAL_STRING_ID		iError		; // 0 : ¼º°ø
	// TRASH_ITEM			UsingItem	; // »ç¿ë µÈ ¾ÆÀÌÅÛ À§Ä¡ Á¤º¸
	TRASH_ITEM item;

	bool bResult = false;

	pPacket->ExtractUINT32(&iError);
	if(0 == iError) // ¼º°øÀÌ¸é
	{
		pPacket->ExtractStruct(&item, sizeof(TRASH_ITEM));
		item.iContainerSlot.SlotIndex = item.iContainerSlot.SlotIndex - 1;	

		if(TrashItem(item) == false) {
			DXUTOutputDebugString("TrashItem Error OnSetChatUsing\n");
		}

		bResult = true;
	}
	else {
		if(g_pResourceManager->GetGlobalString(iError)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iError);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
	}

	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MICROPHONE);
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_MICROPHONE_RESULT, (WPARAM)bResult, (LPARAM)0);
	}

	m_MicItem.Clear();

	return 1;
}


bool SPPlayerInvenArchive::TrashItem(TRASH_ITEM item)
{
	////
	int iStackCount = 1;
	int iMaxStackCount = 1;
	int iTrashStackCount = item.iCount;
	
	CONTAINER_TYPE iContainer = item.iContainerSlot.ContainerType;
	int iSlotIndex = item.iContainerSlot.SlotIndex;
	
	if(iContainer < INVENTORY_EQUIP || iContainer >= CONTAINER_MAX)
		return false;

	if(iSlotIndex < 0 || iSlotIndex >= GetInventorySize(iContainer))
		return false;

	SPItem* pTrashItem = GetInventoryItem(iContainer, iSlotIndex);
	if(pTrashItem == NULL)
		return false;

	int iItemID = pTrashItem->GetItemStatusValue(MV_ITEM_GETUID);
	if(iItemID == 0)
		return false;

	iStackCount = pTrashItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
	
	if(iContainer < FIGURE_EQUIP) {							//ÀÎº¥Åä¸®ÀÇ °æ¿ì
		if(iStackCount > iTrashStackCount){					//¾ÆÀÌÅÛÀÌ ³²´Â °æ¿ì
			iStackCount -= iTrashStackCount;
			pTrashItem->SetItemStatusValue(MV_ITEM_SETSTACK_CNT, (LPARAM)iStackCount);

			CONTAINER_ITEM _item;
			pTrashItem->ConvertContainerItem((CONTAINER_ITEM*)&_item);		

			bool bEnable = CheckPCStatus(_item.Item.iItemID);
			if(pTrashItem->GetContainerType() == m_iCurrentInventoryType) {
				_item.SlotInfo.SlotIndex = _item.SlotInfo.SlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
				
				g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SETTING, (WPARAM)&_item, (LPARAM)bEnable, NULL);
			}			
		}
		else {												//¾ÆÀÌÅÛÀÌ ¿ÏÀüÈ÷ »ç¶óÁö´Â °æ¿ì
			if(pTrashItem->GetContainerType() == m_iCurrentInventoryType || 
				pTrashItem->GetContainerType() == m_iCurrentFigureType)
			{
				int iTrashIndex = pTrashItem->GetSlotIndex() - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
				pTrashItem->SetItemStatusValue(MV_ITEM_SETSTACK_CNT, (LPARAM)0);
				g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_CLEAR, 0, (LPARAM)iTrashIndex, NULL);	
			}		

			pTrashItem->SetClear();		
		}	

		//[2005/10/17]	Quest Mission Check...
		g_pEventManager->GetQuestManager()->CheckMissionItem(iItemID);		
	}
	else if(INVENTORY_HOUSING < iContainer && iContainer < STORAGE) {		//ÀåºñÀÎ °æ¿ì
		CONTAINER_ITEM _item;
		pTrashItem->ConvertContainerItem((CONTAINER_ITEM*)&_item);
		
		if(SetEquipTakeOff( &_item )) {	
			m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&_item);
			pTrashItem->SetClear();
		}
	}
	else if(iContainer == STORAGE) {						//Ã¢°íÀÇ °æ¿ì
		if(iStackCount > iTrashStackCount){					//¾ÆÀÌÅÛÀÌ ³²´Â °æ¿ì
			iStackCount -= iTrashStackCount;
			pTrashItem->SetItemStatusValue(MV_ITEM_SETSTACK_CNT, (LPARAM)iStackCount);

			CONTAINER_ITEM _item;
			pTrashItem->ConvertContainerItem((CONTAINER_ITEM*)&_item);		

			bool bEnable = CheckPCStatus(_item.Item.iItemID);
			_item.SlotInfo.SlotIndex = _item.SlotInfo.SlotIndex - (m_iCurStoragePage * STORAGE_LINE);
			g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_SETTING, (WPARAM)&_item, (LPARAM)bEnable, NULL);
		}
		else {												//¾ÆÀÌÅÛÀÌ ¿ÏÀüÈ÷ »ç¶óÁö´Â °æ¿ì
			if(pTrashItem->GetContainerType() == STORAGE) {
				int iTrashIndex = pTrashItem->GetSlotIndex() - (m_iCurStoragePage * STORAGE_LINE);
				g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_CLEAR, 0, (LPARAM)iTrashIndex, NULL);	
			}
			pTrashItem->SetClear();		
		}
	}
	else if(iContainer == INVENTORY_CASH) {
		pTrashItem->SetClear();
	}

	m_TrashItem.Clear();

	if(iContainer == INVENTORY_CONSUME)				//Quick Slot Update
		SendQuickSlotUpdate(iItemID);
	
	return true;
}


int	SPPlayerInvenArchive::OnSetExceptionMsg( LPARAM lParam )
{
	CPacket* pPacket = (CPacket*)lParam;

	//EXCEPTION_CODE item;
	//pPacket->ExtractStruct(&item, sizeof(EXCEPTION_CODE));
	return 1;
}


int SPPlayerInvenArchive::OnSetUsing(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	GLOBAL_STRING_ID		iErrorID;
	TRASH_ITEM item;

	UINT8 iTrashCount = 1;
	
	pPacket->ExtractUINT32(&iErrorID);

	if(iErrorID == 0) {
		
	}

	char szMsg[256];
	for(int i = 0; i < iTrashCount; i++) {
		pPacket->ExtractStruct(&item, sizeof(TRASH_ITEM));
		item.iContainerSlot.SlotIndex = item.iContainerSlot.SlotIndex - 1;	

		RunCoolTime(item);		
		
		//[2007/2/26] - ¾ÆÀÌÅÛ »ç¿ë ¸Þ½ÃÁö
		if(g_pClientOption && g_pClientOption->GetOptionStruct()->bUseItem) {
			SPItem* pTrashItem = GetInventoryItem(item.iContainerSlot.ContainerType, item.iContainerSlot.SlotIndex);			
			if(pTrashItem && pTrashItem->GetItemStatus() && pTrashItem->GetItemStatus()->GetItemID() && pTrashItem->GetItemAttr()){
				ZeroMemory(szMsg, 256);

				D3DXCOLOR color;
				if(pTrashItem->GetItemAttr()->m_bNameColor) {
					color = pTrashItem->GetItemAttr()->GetNameColor();
				}
				else {
					color = g_pCheckManager->GetItemColor(pTrashItem->GetItemStatus()->GetRareLevel());
				}

				if(g_pResourceManager->GetGlobalString(5017009)) {
					sprintf(szMsg, g_pResourceManager->GetGlobalString(5017009), pTrashItem->GetItemAttr()->m_strName.c_str());			
//					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg, (LPARAM)&color);
					g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMsg , (LPARAM)&color );
				}				
			}			
		}

		if(TrashItem(item) == false) {
			DXUTOutputDebugString("TrashItem Error Count[%d] TrashItemCount[%d]\n", i, iTrashCount);
		}		
	}
	
	DeletePickupItem();

	//ÆÇ¸ÅÃ¢ »óÅÂ¸¦ ÃÊ±âÈ­ ÇÑ´Ù 
	ClearOutSideControl();
	
	m_SellItem.Clear();
	SettingCurrentInventory();

	g_pInterfaceManager->SetNoticeHide();
	m_iNoticeUse = NOTICE_USE_NULL;

	g_pInterfaceManager->SPPerformMessage(SPIM_LOOTING_ITEM_REDRAW, 0, (LPARAM)this);
	return 1;
}


//////////////////////////////////////////////////////////////////////////
int		SPPlayerInvenArchive::OnChangeInventory( LPARAM lParam )
{	
	RecoverIcon();	
	
	CONTAINER_TYPE iContainerType = (CONTAINER_TYPE)lParam;		

	if(m_iCurrentInventoryType == iContainerType)
		return 0;
	
	m_iCurrentInventoryType = iContainerType;	
	m_iCurrentContainer = iContainerType;
	
	SetItemScroll(m_iCurrentInventoryType);
	ReflashInventory();

	return 1;
}


int SPPlayerInvenArchive::ReflashInventory()
{
	g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_ALLCLEAR, 0, 0, NULL);	

	//
	g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SET_PAGE, (WPARAM)m_iCurrentInventoryType, (LPARAM)m_iCurInvenPage[m_iCurrentInventoryType], NULL);
	int iStartIndex = m_iCurInvenPage[m_iCurrentInventoryType] * 4 ;	

	int iCount = 0;
	if(m_vpInventory[m_iCurrentInventoryType].empty())			return 0;
	
	for(iCount = 0; iCount < VIEW_SLOT; iCount++) {		
		if( (iStartIndex + iCount) >= m_vpInventory[m_iCurrentInventoryType].size()){
			break;
		}		
		
		CONTAINER_ITEM item;
		m_vpInventory[m_iCurrentInventoryType].at(iStartIndex + iCount)->ConvertContainerItem((CONTAINER_ITEM*)&item);		

		ItemSetting( item );
	}

	SetDisableIcon();
}


void SPPlayerInvenArchive::SetDisableIcon()
{	
	int iDisableIndex = -1;
	
	if(m_SellItem.IsNull() == false) {
		if(m_SellItem.m_iContainer == m_iCurrentInventoryType) {
			iDisableIndex = m_SellItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
		}
	}

	if(m_MicItem.IsNull() == false) {
		if(m_MicItem.m_iContainer == m_iCurrentInventoryType) {
			iDisableIndex = m_MicItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
		}
	}
	//ÕÒÈË
	if(m_FindItem.IsNull() == false) 
	{
		if(m_FindItem.m_iContainer == m_iCurrentInventoryType) 
		{
			iDisableIndex = m_FindItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
		}
	}
	if(m_FollowItem.IsNull() == false) 
	{
		if(m_FollowItem.m_iContainer == m_iCurrentInventoryType) 
		{
			iDisableIndex = m_FollowItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
		}
	}
	if(m_SnatchItem.IsNull() == false) 
	{
		if(m_SnatchItem.m_iContainer == m_iCurrentInventoryType) 
		{
			iDisableIndex = m_SnatchItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
		}
	}

	if(m_bPickup && m_PickUpItem.IsNull() == false) {
		if(m_PickUpItem.m_iContainer > CONTAINER_TYPE_INVALID && m_PickUpItem.m_iContainer < FIGURE_EQUIP) {
			if(m_PickUpItem.m_iContainer == m_iCurrentInventoryType) {				
				int iStartX = m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE;
				if(m_PickUpItem.m_iSlotIndex >= iStartX && m_PickUpItem.m_iSlotIndex < iStartX + VIEW_SLOT) {
					iDisableIndex = m_PickUpItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
					g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
				}
			}
		}
	}

	if(m_TrashItem.IsNull() == false) {
		if(m_TrashItem.m_iContainer == m_iCurrentInventoryType) {
			iDisableIndex = m_TrashItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
		}
		else if(m_TrashItem.m_iContainer == STORAGE) {
			iDisableIndex = m_TrashItem.m_iSlotIndex - (m_iCurStoragePage * STORAGE_LINE);
			g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
		}
	}

	for(int i = 0; i < MAX_PC_TRADE_SLOT ; i++) {
		if(m_TradeItem[i].IsNull())
			continue;
		if(m_TradeItem[i].m_iContainer < INVENTORY_EQUIP || m_TradeItem[i].m_iContainer > INVENTORY_HOUSING)
			continue;
		if(m_TradeItem[i].m_iSlotIndex < 0)
			continue;		
		
		if(m_iCurrentInventoryType == m_TradeItem[i].m_iContainer) {
			iDisableIndex = m_TradeItem[i].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
		}
	}

	for(int i = 0; i < FM_MAX_SELLITEM; i++) {
		if(m_UserShopItem[i].IsNull())
			continue;
		if(m_UserShopItem[i].m_iContainer < INVENTORY_EQUIP || m_UserShopItem[i].m_iContainer > INVENTORY_HOUSING)
			continue;
		if(m_UserShopItem[i].m_iSlotIndex < 0)
			continue;

		if(m_iCurrentInventoryType == m_UserShopItem[i].m_iContainer) {
			iDisableIndex = m_UserShopItem[i].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
		}
	}

	for(int i = 0; i < MAX_ITEM_BLOCK; i++) {
		if(m_BlockItem[i].IsNull())
			continue;
		if(m_BlockItem[i].m_iContainer < INVENTORY_EQUIP || m_BlockItem[i].m_iContainer > INVENTORY_HOUSING)
			continue;
		if(m_BlockItem[i].m_iSlotIndex < 0)
			continue;

		if(m_iCurrentInventoryType == m_BlockItem[i].m_iContainer) {
			iDisableIndex = m_BlockItem[i].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
		}
	}

	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		SPSelectItem &feedNormal = m_vecPetFeeds[idx].feedNormal;
		SPSelectItem &feedEnergy = m_vecPetFeeds[idx].feedEnergy;
		if(feedNormal.IsNull() == false) {
			if(feedNormal.m_iContainer == m_iCurrentInventoryType) {
				iDisableIndex = feedNormal.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
				g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
			}
		}
		if(feedEnergy.IsNull() == false) {
			if(feedEnergy.m_iContainer == m_iCurrentInventoryType) {
				iDisableIndex = feedEnergy.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
				g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
			}
		}
	}
}


int SPPlayerInvenArchive::ItemPageUp()
{
	if(m_iInvenPage[m_iCurrentInventoryType] < 1)
		return 1;
	
	m_iCurInvenPage[m_iCurrentInventoryType]--;

	if(m_iCurInvenPage[m_iCurrentInventoryType] < 0) {
		m_iCurInvenPage[m_iCurrentInventoryType] = 0;
		return 1;
	}

	//DXUTOutputDebugString("ItemPageUp Con[%d] Page[%d]\n", m_iCurrentInventoryType, m_iCurInvenPage[m_iCurrentInventoryType]);

	ReflashInventory();

	return 1;
}


int SPPlayerInvenArchive::ItemPageDown()
{
	if(m_iInvenPage[m_iCurrentInventoryType] < 1)
		return 1;
	
	m_iCurInvenPage[m_iCurrentInventoryType]++;

	if(m_iInvenPage[m_iCurrentInventoryType] < m_iCurInvenPage[m_iCurrentInventoryType]) {
		m_iCurInvenPage[m_iCurrentInventoryType] = m_iInvenPage[m_iCurrentInventoryType];
		return 1;
	}

	//DXUTOutputDebugString("ItemPageDown Con[%d] Page[%d]\n", m_iCurrentInventoryType, m_iCurInvenPage[m_iCurrentInventoryType]);

	ReflashInventory();

	return 1;
}


int SPPlayerInvenArchive::ItemSetPage(int iPage)
{
	if(iPage < 0)
		iPage = 0;

	if(iPage > m_iInvenPage[m_iCurrentInventoryType])
		iPage = m_iInvenPage[m_iCurrentInventoryType];

	m_iCurInvenPage[m_iCurrentInventoryType] = iPage;
	ReflashInventory();
	
	return 1;
}


int	SPPlayerInvenArchive::ItemGridCursorMove(LPARAM lParam)
{
	m_bClickMove = (bool)lParam;
	//DXUTOutputDebugString("Click ÈÄ À§Ä¡ ÀÌµ¿...[%d]\n", m_bClickMove);	

	if(m_bPickup == false && m_bClickMove) {		
		ItemPickUp(m_iItemLClickIndex);				//¿©±â¼­ ¾ÆÀÌÅÛ PickUp
	}
	
	return 0;
}


int	SPPlayerInvenArchive::ItemGridClickDown(int iSlotIndex)
{
	//¸¸ÀÏ ÀÌÀüÀÇ PickUp¾ÆÀÌÅÛ°ú °°Àº °æ¿ì
	if(m_TrashItem.IsNull() == false) {
		g_pInterfaceManager->ForceReply();
		RecoverIcon();
	}
	
	if(m_iNoticeUse == NOTICE_USE_ITEM_TRASH || m_iNoticeUse == NOTICE_USE_ITEM_PARTITION) {
		g_pInterfaceManager->ForceReply();
		RecoverIcon();
	}

	if(m_bPickup)					return 0;
	
	iSlotIndex = iSlotIndex + (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
	m_iItemLClickIndex = iSlotIndex;	

	//Æ®·¹ÀÌµå ¾ÆÀÌÅÛÀ¸·Î Á¸ÀçÇÏ´Â °æ¿ì Ã³¸®
	int iTradeIndex = IsTradeItem(m_iCurrentInventoryType, iSlotIndex);
	if(iTradeIndex > -1) { 		
		m_bPickup = false;
		return 0;
	}
	
	//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛÀ¸·Î Á¸ÀçÇÏ´Â °æ¿ì Ã³¸®
	int iUserShopIndex = IsUserShopItem(m_iCurrentInventoryType, iSlotIndex);
	if(iUserShopIndex > -1) {
		m_bPickup = false;
		return 0;
	}

	//ºí·° ¾ÆÀÌÅÛÀ¸·Î Á¸ÀçÇÏ´Â °æ¿ì Ã³¸®
	int iItemBlockIndex = IsBlockItem(m_iCurrentInventoryType, iSlotIndex);
	if(iItemBlockIndex > -1) {
		m_bPickup = false;
		return 0;
	}
	
	//Æê ¸ÔÀÌ·Î µî·ÏµÇ¾î ÀÖ´Â °æ¿ì
	if(IsPetFeedItem(m_iCurrentInventoryType, iSlotIndex)){
		m_bPickup = false;
		return 0;
	}

	//¿ÜÄ¡±â ¾ÆÀÌÅÛÀÎ °æ¿ì
	if(IsMicItem(m_iCurrentInventoryType, iSlotIndex)){
		m_bPickup = false;
		return 0;
	}	
	
	if ( IsFindItem( m_iCurrentInventoryType, iSlotIndex ) )
	{
		m_bPickup = false;
		return 0;
	}

	if ( IsFollowItem( m_iCurrentInventoryType, iSlotIndex ) )
	{
		m_bPickup = false;
		return 0;
	}

	if ( IsSnatchItem( m_iCurrentInventoryType, iSlotIndex ) )
	{
		m_bPickup = false;
		return 0;
	}
	ClearOutSideControl();

	//DXUTOutputDebugString("ItemGridClick Slot[%d]\n", iSlotIndex);
	return 0;
}


int	SPPlayerInvenArchive::ItemPickUp(int iSlotIndex)
{
	if(m_bPickup)				return 0;		
	
	m_bPickup = true;
	int iItemID = 0;
	int iSrcSlotIndex = 0;

	SPTexture* icon = NULL;

	if(IsItemExist(iSlotIndex, m_iCurrentInventoryType) == false) {
		m_bPickup = false;
		return 0;
	}		

	iItemID = m_vpInventory[m_iCurrentInventoryType].at(iSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);				
	if(iItemID == 0) {
		m_bPickup = false;
		return 0;
	}

	//Æ®·¹ÀÌµå ¾ÆÀÌÅÛÀ¸·Î Á¸ÀçÇÏ´Â °æ¿ì Ã³¸®
	int iTradeIndex = IsTradeItem(m_iCurrentInventoryType, iSlotIndex);
	if(iTradeIndex > -1) {
		m_bPickup = false;
		return 0;
	}

	//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛÀ¸·Î Á¸ÀçÇÏ´Â °æ¿ì Ã³¸®
	int iUserShopIndex = IsUserShopItem(m_iCurrentInventoryType, iSlotIndex);
	if(iUserShopIndex > -1) {
		m_bPickup = false;
		return 0;
	}

	int iPetShopIndex = IsPetShopItem(m_iCurrentInventoryType, iSlotIndex);
	if (iPetShopIndex > -1)
	{
		m_bPickup = false;
		return 0;
	}

	//ºí·° ¾ÆÀÌÅÛÀ¸·Î Á¸ÀçÇÏ´Â °æ¿ì Ã³¸®
	int iItemBlockIndex = IsBlockItem(m_iCurrentInventoryType, iSlotIndex);
	if(iItemBlockIndex > -1) {
		m_bPickup = false;
		return 0;
	}

	//Æê ¸ÔÀÌ·Î µî·ÏµÇ¾î ÀÖ´Â °æ¿ì
	if(IsPetFeedItem(m_iCurrentInventoryType, iSlotIndex)){
		m_bPickup = false;
		return 0;
	}

	//¿ÜÄ¡±â ¾ÆÀÌÅÛÀÎ °æ¿ì
	if(IsMicItem(m_iCurrentInventoryType, iSlotIndex)){
		m_bPickup = false;
		return 0;
	}
	if( IsFindItem( m_iCurrentInventoryType, iSlotIndex ) )
	{
		m_bPickup = false;
		return 0;
	}

	if( IsFollowItem( m_iCurrentInventoryType, iSlotIndex ) )
	{
		m_bPickup = false;
		return 0;
	}

	if( IsSnatchItem( m_iCurrentInventoryType, iSlotIndex ) )
	{
		m_bPickup = false;
		return 0;
	}

	m_PickUpItem.SetSelectItem(m_iCurrentInventoryType, iSlotIndex);
	SetMouseIcon();		

	SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);		
}


int	SPPlayerInvenArchive::ItemGridClickUp(int iSlotIndex)
{	
	iSlotIndex = iSlotIndex + (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);	
	
	//Æ®·¹ÀÌµå ¾ÆÀÌÅÛÀ¸·Î Á¸ÀçÇÏ´Â °æ¿ì Ã³¸®
	int iTradeIndex = IsTradeItem(m_iCurrentInventoryType, iSlotIndex);
	if(iTradeIndex > -1) {
		if(m_bPickup) {			//´Ù¸¥ ¾ÆÀÌÅÛÀ» µé°í ÀÖ´Â °æ¿ì
			if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
				RecoverIcon();
		}		
		m_bPickup = false;
		return 0;
	}

	//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛÀ¸·Î Á¸ÀçÇÏ´Â °æ¿ì Ã³¸®
	int iUserShopIndex = IsUserShopItem(m_iCurrentInventoryType, iSlotIndex);
	if(iUserShopIndex > -1) {
		if(m_bPickup) {			//´Ù¸¥ ¾ÆÀÌÅÛÀ» µé°í ÀÖ´Â °æ¿ì
			if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
				RecoverIcon();
		}		
		m_bPickup = false;
		return 0;
	}

	//ºí·° ¾ÆÀÌÅÛÀ¸·Î Á¸ÀçÇÏ´Â °æ¿ì Ã³¸®
	int iItemBlockIndex = IsBlockItem(m_iCurrentInventoryType, iSlotIndex);
	if(iItemBlockIndex > -1) {
		if(m_bPickup) {			//´Ù¸¥ ¾ÆÀÌÅÛÀ» µé°í ÀÖ´Â °æ¿ì
			if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
				RecoverIcon();
		}		
		m_bPickup = false;
		return 0;
	}

	//Æê ¸ÔÀÌ·Î µî·ÏµÇ¾î ÀÖ´Â °æ¿ì
	if(IsPetFeedItem(m_iCurrentInventoryType, iSlotIndex)){
		if(m_bPickup) {
			if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
				RecoverIcon();
		}
		m_bPickup = false;
		return 0;
	}

	//¿ÜÄ¡±â ¾ÆÀÌÅÛÀÎ °æ¿ì
	if(IsMicItem(m_iCurrentInventoryType, iSlotIndex)){
		if(m_bPickup) {
			if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
				RecoverIcon();
		}
		m_bPickup = false;
		return 0;
	}

	if( IsFindItem( m_iCurrentInventoryType, iSlotIndex ) )
	{
		if(m_bPickup) 
		{
			if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
				RecoverIcon();
		}
		m_bPickup = false;
		return 0;
	}

	if( IsFollowItem( m_iCurrentInventoryType, iSlotIndex ) )
	{
		if(m_bPickup) 
		{
			if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
				RecoverIcon();
		}
		m_bPickup = false;
		return 0;
	}

	if( IsSnatchItem( m_iCurrentInventoryType, iSlotIndex ) )
	{
		if(m_bPickup) 
		{
			if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
				RecoverIcon();
		}
		m_bPickup = false;
		return 0;
	}
	
	if(m_bPickup)
	{
		if(iSlotIndex < 0 || iSlotIndex >= static_cast<int>(m_vpInventory[m_iCurrentInventoryType].size())) {
			RecoverIcon();
			return 0;
		}
		
		if((((SPPlayer*)m_pParent)->IsDead())){
			RecoverIcon();
			return 0;
		}
		
		CHANGE_SLOT item;
		item.DestSlotInfo.ContainerType = (CONTAINER_TYPE)m_iCurrentInventoryType;
		item.DestSlotInfo.SlotIndex = iSlotIndex;		
		item.SrcSlotInfo.ContainerType = m_PickUpItem.m_iContainer;
		item.SrcSlotInfo.SlotIndex = m_PickUpItem.m_iSlotIndex;		

		int iSlotPosID;
		iSlotPosID = m_PickUpItem.m_iSlotIndex;		

		// °°Àº °÷ ÀÌµ¿ÀÌ¶ó¸é ±×´ë·Î~
		if(item.DestSlotInfo.SlotIndex == item.SrcSlotInfo.SlotIndex && 
			item.DestSlotInfo.ContainerType == item.SrcSlotInfo.ContainerType)
		{			
			CONTAINER_ITEM _item;			
			SPItem* pItem = GetInventoryItem(item.DestSlotInfo.ContainerType, item.DestSlotInfo.SlotIndex);
			pItem->ConvertContainerItem((CONTAINER_ITEM*)&_item);
			DeletePickupItem();
			ItemSetting( _item );			
			return 0;
		}

		switch( m_iCurrentInventoryType )
		{
		case INVENTORY_EQUIP:
			{
				if((item.SrcSlotInfo.ContainerType < FIGURE_EQUIP) && (item.SrcSlotInfo.ContainerType != INVENTORY_EQUIP)) 
					return 0;

				// Equip->Inventory ¿¡¼­ PosID°¡ ´Ù¸¥°æ¿ì return
				int iItemID = m_vpInventory[INVENTORY_EQUIP].at(item.DestSlotInfo.SlotIndex)->GetItemStatus()->SPSendMessage(MV_ITEM_GETUID);				

				SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
				int iSubType = pPickUpItem->GetItemAttr()->m_eSubType;
				int iEquipPos = pPickUpItem->GetItemAttr()->m_ePosID1;								
				
				//Inventory Equip Dest¿¡ Àåºñ°ü·Ã ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÏ´Â °æ¿ì ¿¹¿Ü Ã³¸®
				if(IsItemExist(iSlotIndex, INVENTORY_EQUIP)) {
					SPItem* pDestItem = GetInventoryItem(INVENTORY_EQUIP, iSlotIndex);
					
					//µÎ ¾ÆÀÌÅÛÀÌ °°Àº ÄÁÅ×ÀÌ³Ê°¡ ¾Æ´Ñ °æ¿ì
					if(item.DestSlotInfo.ContainerType != item.SrcSlotInfo.ContainerType) {
						bool bItemAvailable = pDestItem->GetItemStatus()->IsTimeAvailable();		//À¯È¿ÇÑ ¾ÆÀÌÅÛÀÎÁö
						if(bItemAvailable == false) {
							RecoverIcon();
							return 0;
						}
						int iDestSubType = pDestItem->GetItemAttr()->m_eSubType;
						int iDestEquipPos =  pDestItem->GetItemAttr()->m_ePosID1;					

						//¸ÞÀÎ½½·Ô¸¸ Ã¼Å©ÇÏ±â ¶§¹®¿¡ ¹®Á¦ÀÇ ¼ÒÁö°¡ ÀÖÀ½...
						if(m_PickUpItem.m_iContainer == FIGURE_EQUIP || m_PickUpItem.m_iContainer == FIGURE_FASHION
							|| FIGURE_EFFECT == m_PickUpItem.m_iContainer) {
							if(iDestSubType != iSubType || iDestEquipPos != iEquipPos ) {
								RecoverIcon();
								return 0;
							}
						}

						//[2006/9/15] DestÀ§Ä¡ÀÇ ¾ÆÀÌÅÛÀÌ Àåºñ³ª ÆÐ¼ÇÀ¸·Î ¿Ã¶ó°¥¼ö ÀÖ´ÂÁö È®ÀÎ
						if(CheckPCStatus(pDestItem->GetItemStatus()->GetItemID()) == false) {
							RecoverIcon();
							return 0;
						}
					}
					
					//[2005/7/13]
					//Dest¿¡ Àåºñ ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÏ´Â °æ¿ì ½ÇÁ¦ Src¿Í Dest¸¦ ¹Ù²ã¼­ Àü¼ÛÇÏµµ·Ï ÇÑ´Ù..
					//´Ü¹æÇâ¼ºÀ» °¡Áö±â À§ÇØ¼­ ¼Õ´ë¸®´ÔÀÌ °­Á¦·Î ½ÃÅ´
					if(m_PickUpItem.m_iContainer == FIGURE_EQUIP || m_PickUpItem.m_iContainer == FIGURE_FASHION
						|| FIGURE_EFFECT == m_PickUpItem.m_iContainer) {
						item.SrcSlotInfo.ContainerType = (CONTAINER_TYPE)m_iCurrentInventoryType;
						item.SrcSlotInfo.SlotIndex = iSlotIndex;		
						item.DestSlotInfo.ContainerType = m_PickUpItem.m_iContainer;
						item.DestSlotInfo.SlotIndex = m_PickUpItem.m_iSlotIndex;
					}					
				}
				
				ITEMSTATUS_ID iItemstatusID = (ITEMSTATUS_ID)pPickUpItem->GetItemStatusValue(MV_ITEM_GETSTATUSID);

				if(iItemID != 0 && (iItemstatusID == IS_EQUIP || iItemstatusID >= IS_EQUIP_BATTLE))
				{
					SPItemAttr* pItemAttr;
					g_pItemCluster->GetItemInfo(iItemID, pItemAttr);
					int iDestEquipPosID = pItemAttr->m_ePosID1; 
					int iEquipPosID = m_PickUpItem.m_iSlotIndex + 1;

					// ÇÈ¾÷ÇÑ ¾ÆÀÌÅÛÀÌ ÀåºñÀÌ¸é¼­ ¹«±âÀÎ°æ¿ì ¿¹¿Ü Ã³¸®
					if(INVENTORY_HOUSING < m_PickUpItem.m_iContainer && m_PickUpItem.m_iContainer < STORAGE) {
						if(iEquipPosID >= EQ_WEAPON1) {
							if(iEquipPosID >= EQ_WEAPON2) {					//2¹ø ¹«±â½½·ÔÀÎ°æ¿ì 
								iEquipPosID = iEquipPosID - 2;
							}

							if(iEquipPosID != iDestEquipPosID){
								RecoverIcon();
								return 0;
							}
						}
					}
				}				
				break;
			}
		default:
			//if(item.SrcSlotInfo.ContainerType != STORAGE && m_iCurrentInventoryType != item.SrcSlotInfo.ContainerType) {
			if(item.SrcSlotInfo.ContainerType == STORAGE) {
			}
			else if(item.SrcSlotInfo.ContainerType == FIGURE_PET) {
				if(IsItemExist(iSlotIndex, INVENTORY_PET)) {
					SPItem* pDestItem = GetInventoryItem(INVENTORY_PET, iSlotIndex);
					bool bItemAvailable = pDestItem->GetItemStatus()->IsTimeAvailable();		//À¯È¿ÇÑ ¾ÆÀÌÅÛÀÎÁö
					if(bItemAvailable == false) {
						RecoverIcon();
						return 0;
					}
				}
			}
			else if(m_iCurrentInventoryType != item.SrcSlotInfo.ContainerType) {
				RecoverIcon();
				return 0;
			}

			//[2005/7/25] ¾ÆÀÌÅÛ ³ª´©±â Å×½ºÆ® ÄÚµåÀÓ!
			if(item.DestSlotInfo.ContainerType == item.SrcSlotInfo.ContainerType) {
				if(IsItemExist(item.DestSlotInfo.SlotIndex, item.DestSlotInfo.ContainerType) == false) {
					SPItem* pPickUpItem = GetInventoryItem(item.SrcSlotInfo.ContainerType, item.SrcSlotInfo.SlotIndex);
					int iPickUpItemStack = pPickUpItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
					if(iPickUpItemStack > 1) {
						g_pInterfaceManager->GetNoticeBox()->SPSendMessage(SPIM_SET_MAXVALUE, (WPARAM)iPickUpItemStack, 0);
						g_pInterfaceManager->GetNoticeBox()->SPSendMessage(SPIM_SET_MINVALUE, (WPARAM)1, 0);
						
						char szMsg[512];
						ZeroMemory(szMsg, 512);
						if(g_pResourceManager->GetGlobalString(4003004)) {
							std::string strMsg = g_pResourceManager->GetGlobalString(4003004);
							std::string strItemName = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex)->GetItemAttr()->m_strName;
							wsprintf(szMsg, strMsg.c_str(), strItemName.c_str());
						}
						g_pInterfaceManager->SetNumNotice(szMsg, this->m_pParent);

						m_iNoticeUse = NOTICE_USE_ITEM_PARTITION;
						m_ChangeSlot = item;
						RecoverIcon();
						return 0;
					}
				}
			}

			break;
		}

		if(item.SrcSlotInfo.ContainerType == STORAGE) {				//¼Ò½º°¡ Ã¢°í¿¡¼­ ¿À´Â °æ¿ì...		
			SPItem* pPickUpItem = GetInventoryItem(item.SrcSlotInfo.ContainerType, item.SrcSlotInfo.SlotIndex);			
			CONTAINER_TYPE iPickUpContainerType = (CONTAINER_TYPE)(pPickUpItem->GetItemAttr()->m_eType - 1); 
			
			if(iPickUpContainerType != m_iCurrentInventoryType){	//¾ÆÀÌÅÛ Å¸ÀÔÀÌ ÀÎº¥Åä¸® Å¸ÀÔ°ú ´Ù¸£¸é ½ÇÆÐ...
				RecoverIcon();
				return 0;
			}
		}
		
		ClearOutSideControl();

		SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
		item.iCount = pPickUpItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
		SendReqChangeSlot( item );
	}
	else {							//Item Use 
		if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL) {
			//[2006/1/2] - Äü½½·Ô(¶Ç´Â ´Ù¸¥ À©µµ¿ì)¿¡¼­ ¾ÆÀÌÄÜÀÌ ¿À´Â °æ¿ì...
			if(m_iItemLClickIndex < 0) {
				if(m_bPickup) {
					if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
						RecoverIcon();
				}

				return 0;
			}
			
			ItemUse(m_iCurrentInventoryType, iSlotIndex);
		}
		
		//[2008/5/16] - Shift click ½Ã 
		SPInputStruct* pInputStruct = g_pInputManager->GetInputStruct();
		if(pInputStruct->ikLShift.InputState >= INPUT_PRESS_REPEAT) { 
			if(IsItemExist(iSlotIndex, m_iCurrentInventoryType)) {
				SPItem* pItem = GetInventoryItem(m_iCurrentInventoryType, iSlotIndex);

				if(pItem && pItem->IsEmptyItem() == false) {
					g_pInterfaceManager->SPPerformMessage(SPIM_ITEM_ID_INFO, (WPARAM)pItem->GetItemStatus()->GetItemID(), (LPARAM)pItem);
				}
			}
		}
	}
}


int SPPlayerInvenArchive::ItemGridDBK(int iSlotIndex)
{
	iSlotIndex = iSlotIndex + (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
	
	//Æ®·¹ÀÌµå ¾ÆÀÌÅÛÀ¸·Î Á¸ÀçÇÏ´Â °æ¿ì Ã³¸®
	int iTradeIndex = IsTradeItem(m_iCurrentInventoryType, iSlotIndex);
	if(iTradeIndex > -1) { 
		if(m_bPickup) {			//´Ù¸¥ ¾ÆÀÌÅÛÀ» µé°í ÀÖ´Â °æ¿ì
			if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
				RecoverIcon();
		}		
		m_bPickup = false;
		return 0;
	}

	//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛÀ¸·Î Á¸ÀçÇÏ´Â °æ¿ì Ã³¸®
	int iUserShopIndex = IsUserShopItem(m_iCurrentInventoryType, iSlotIndex);
	if(iUserShopIndex > -1) {
		if(m_bPickup) {			//´Ù¸¥ ¾ÆÀÌÅÛÀ» µé°í ÀÖ´Â °æ¿ì
			if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
				RecoverIcon();
		}		
		m_bPickup = false;
		return 0;
	}

	//ºí·° ¾ÆÀÌÅÛÀ¸·Î Á¸ÀçÇÏ´Â °æ¿ì Ã³¸®
	int iItemBlockIndex = IsBlockItem(m_iCurrentInventoryType, iSlotIndex);
	if(iItemBlockIndex > -1) {
		if(m_bPickup) {			//´Ù¸¥ ¾ÆÀÌÅÛÀ» µé°í ÀÖ´Â °æ¿ì
			if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
				RecoverIcon();
		}		
		m_bPickup = false;
		return 0;
	}

	//NPC »óÁ¡À» ÀÌ¿ëÇÏ´Â °æ¿ì 
	if(g_pEventManager->GetCutInState() == CUT_IN_STATE_TRADE) {
		if(m_bPickup) {			//´Ù¸¥ ¾ÆÀÌÅÛÀ» µé°í ÀÖ´Â °æ¿ì
			if(iSlotIndex != m_PickUpItem.m_iSlotIndex)
				RecoverIcon();
		}		
		m_bPickup = false;
		return 0;
	}

	if((((SPPlayer*)m_pParent)->IsDead())){
		return 0;
	}	
	
	if(m_iCurrentInventoryType == INVENTORY_EQUIP || m_iCurrentInventoryType == INVENTORY_PET) {
	}
	else {
		return 0;
	}
	
	SPItem* pItem = NULL;
	int iSubType = 0;
	int iEquipPos = 0;
	CHANGE_SLOT item;	
	
	if(m_iCurrentInventoryType == INVENTORY_EQUIP) {
		if(IsItemExist(iSlotIndex, INVENTORY_EQUIP) == false)
			return 0;

		pItem = GetInventoryItem(INVENTORY_EQUIP, iSlotIndex);
		bool bItemAvailable = pItem->GetItemStatus()->IsTimeAvailable();
		if(bItemAvailable == false)
			return 0;

		iSubType = pItem->GetItemAttr()->m_eSubType;
		iEquipPos = pItem->GetItemAttr()->m_ePosID1 - 1;
		
		if(iSubType == ITEM_SUBTYPE_EQUIP_EQUIP) {
			item.DestSlotInfo.ContainerType = FIGURE_EQUIP;
		}
		else if(iSubType == ITEM_SUBTYPE_EQUIP_FASHION) {
			item.DestSlotInfo.ContainerType = FIGURE_FASHION;
		}
		else if(iSubType == ITEM_SUBTYPE_EQUIP_EFFECT) {
			item.DestSlotInfo.ContainerType = FIGURE_EFFECT;
		}
		else {
			return 0;
		}

		if(iEquipPos >= EQ_WEAPON1 - 1 ) {
			if(m_iCurrentWeaponSlot != 1) {
				iEquipPos += 2;
			}
		}
	}
	else if(m_iCurrentInventoryType == INVENTORY_PET) {
		if(IsItemExist(iSlotIndex, INVENTORY_PET) == false)
			return 0;

		pItem = GetInventoryItem(INVENTORY_PET, iSlotIndex);
		bool bItemAvailable = pItem->GetItemStatus()->IsTimeAvailable();
		if(bItemAvailable == false)
			return 0;

		if(pItem->GetItemStatus()->GetItemAttr()->m_ePosID1 != 1) {
			RecoverIcon();
			return 0;
		}
		
		item.DestSlotInfo.ContainerType = FIGURE_PET;
		iEquipPos = 0;		
	}			

	//[2006/9/15] ÀÎº¥Åä¸®ÀÇ ´õºíÅ¬¸¯ÇÑ ¾ÆÀÌÅÛÀÌ Àåºñ³ª ÆÐ¼Ç¿¡ ¿Ã¶ó°¥¼ö ÀÖ´ÂÁö È®ÀÎ
	if(CheckPCStatus(pItem->GetItemStatus()->GetItemID()) == false) {
		RecoverIcon();
		return 0;
	}
	
	item.DestSlotInfo.SlotIndex = iEquipPos;
	item.SrcSlotInfo.ContainerType = m_iCurrentInventoryType;
	item.SrcSlotInfo.SlotIndex = iSlotIndex;
	item.iCount = pItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
	
	SendReqChangeSlot( item );

	return 1;
}


bool SPPlayerInvenArchive::ItemQuickUse(CONTAINER_TYPE iContainerType, int iItemID)
{
	//ÇØ´çÇÔ¼ö´Â QuickSlot¿¡¼­¸¸ »ç¿ë...	
	std::vector<SPItem*>::iterator iter = m_vpInventory[iContainerType].begin();
	int iSlotIndex = 0;
	int iCheckItemID = 0;
	for(; iter != m_vpInventory[iContainerType].end(); ++iter, iSlotIndex++) {
		iCheckItemID = (*iter)->GetItemStatusValue(MV_ITEM_GETUID);
		if(iItemID == iCheckItemID){
			if(ItemUse(iContainerType, iSlotIndex) == true) {
				return true;
			}
		}
	}

	return false;
}


bool SPPlayerInvenArchive::ItemUse(CONTAINER_TYPE iContainerType, int iSlotIndex)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	//»ç¸Á½Ã Á¦ÇÑ
	if(g_pGOBManager->GetLocalPlayer()->IsDead())
		return false;
	
	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL)					//ÄÆÀÎ»óÅÂ Á¦ÇÑ
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE)	//Æ®·¹ÀÌµå Áß Á¦ÇÑ
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP)		//³»°¡ °³ÀÎ »óÁ¡°³¼³ÇÑ °æ¿ì
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetUserShopID())						//³»°¡ Å¸ÀÎÀÇ °³ÀÎ»óÁ¡¿¡ µé¾î°£ °æ¿ì
		return false;

	if(SPCashTradeManager::GetInstance()->GetCashTradeState() == CASH_TRADE_STATE_WAIT) {	//[2008/4/25] - cash °Å·¡ ´ë±âÁß
		return false;
	}
	
	if(iContainerType < INVENTORY_EQUIP || iContainerType > INVENTORY_HOUSING)
		return false;

	if(iSlotIndex < 0 || iSlotIndex >= GetInventorySize(iContainerType))
		return false;

	if(IsItemExist(iSlotIndex, iContainerType) == false)
		return false;

	SPItem* pUseItem = GetInventoryItem(iContainerType, iSlotIndex);//
	bool bItemAvailable = pUseItem->GetItemStatus()->IsTimeAvailable();
	if(bItemAvailable == false)
		return false;

	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP)) {
		if(g_pkPvpManager->GetPvpStatus() > PVP_STATUS_ENTERWAIT) {
			if(pUseItem->GetItemAttr()->m_bPvp == false) {
				return false;
			}		
		}
	}	

	ITEM_TYPE		iType		= pUseItem->GetItemAttr()->m_eType;
	ITEM_SUBTYPE	iSubType	= pUseItem->GetItemAttr()->m_eSubType;
	int iCoolTimeID = pUseItem->GetItemAttr()->m_iItemCoolTimeID;

	//[2005/10/11] ÇöÀç »ç¿ë°¡´ÉÇÑ ¾ÆÀÌÅÛÀ» »ç¿ëÇß´ÂÁö È®ÀÎÇÑ´Ù
	if(g_pCheckManager->CheckItemStatus(pUseItem->GetItemStatus()->GetItemID()) == false)
		return false;

	//cool time Check
	if(SPCoolTimeManager::GetInstance()->IsExistCoolTime(iCoolTimeID)) {
		return false;
	}	
	
	if(iType == ITEM_TYPE_CONSUME) {
		//I need;
		if(iSubType == ITEM_SUBTYPE_WASTE_NORMAL)
			SendItemUseMsg(iContainerType, iSlotIndex);
		else if(iSubType == ITEM_SUBTYPE_WASTE_ENCHANT)
			ItemPreUse(iContainerType, iSlotIndex);

		else if(iSubType == ITEM_SUBTYPE_WASTE_MSGBOARD)
		{
			g_pInterfaceManager->SPChildWndSendMessage(WIID_CREATEMSG, SPIM_MSGBOARD_CREATE_OPEN,(WPARAM)iContainerType, (LPARAM)iSlotIndex);
		}	

		else if( iSubType == ITEM_SUBTYPE_WASTE_SEARCH )
		{
			ItemPreUseEx( iContainerType, iSlotIndex );
		}
		else if( iSubType == ITEM_SUBTYPE_WASTE_FOLLOW )
		{
			FWItemPreUseEx( iContainerType, iSlotIndex );
		}
		else if( iSubType == ITEM_SUBTYPE_WASTE_SNATCH )
		{
			SItemPreUseEx( iContainerType, iSlotIndex );
		}

		return true;
	}
	// ÀÌº¥Æ® ¾ÆÀÌÅÛÀº Äù½ºÆ® ½ÇÇà [10/2/2007 AJJIYA]
	else if( iType == ITEM_TYPE_EVENT )
	{
		if( pUseItem == NULL )
			return false;

		int	iItemID		=	0;

		SPItemStatus*	pUseItemStatus	=	pUseItem->GetItemStatus();

		if( pUseItemStatus != NULL )
			iItemID	=	pUseItemStatus->GetItemID();

		if( iSubType == ITEM_SUBTYPE_EVENT_QUEST )
		{
			SPItemAttr*		pUseItemAttr	=	pUseItem->GetItemAttr();

			int	iQuestID	=	0;

			if( pUseItemAttr != NULL )
				iQuestID	=	pUseItemAttr->m_iQuestID;

			return SPQuestManager::GetInstance()->AddItemQuest( pUseItemAttr->m_iQuestID , iItemID );
		}
		else if( iSubType == ITEM_SUBTYPE_EVENT_GAMBLE )
		{
			return g_pInterfaceManager->SPChildWndSendMessage( WIID_GAMBLE , SPIM_GAMBLE_USE , (WPARAM)pUseItem , iItemID );
		}
	}

	return false;
}


bool SPPlayerInvenArchive::ItemPreUse(CONTAINER_TYPE iContainerType, int iSlotIndex)					//ÅÐ¶ÏÊ¹ÓÃµÄÊÇÄÇÖÖÀ®°È
{
	if(!m_MicItem.IsNull())
		return false;
	
	int iItemID = GetInventoryItem(iContainerType, iSlotIndex)->GetItemStatus()->GetItemID();
	
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MICROPHONE);
	
	//SPWindow* pWindowTest = g_pInterfaceManager->FindWindow( WIID_FRIEND_FIND );
	
	//int Level = 0;
	//SPItemAttr* pItemAttr = NULL;
	//g_pItemCluster->GetItemInfo( iItemID, pItemAttr );
	//if( pItemAttr != NULL )
	//{
	//	Level = pItemAttr->m_iItemLevel;
	//	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND, SPIM_ITEM_LEVEL, 0, (LPARAM)&Level );
	//}
	
	//if( NULL == pWindowTest )
	//{
	//	return false;
	//}
	
	if(pWindow == NULL)
		return false;		
	
	if(iItemID == g_pCheckManager->GetMicServerItemID()) {
		pWindow->SPSendMessage(SPIM_MICROPHONE_MODE, (WPARAM)0, (LPARAM)iItemID);		//MIC_MODE_SERVER
	}
	else if(iItemID == g_pCheckManager->GetMicChannelItemID()) {
		pWindow->SPSendMessage(SPIM_MICROPHONE_MODE, (WPARAM)1, (LPARAM)iItemID);			//MIC_MODE_CHANNEL
	}
	else if(SPWindowBugleSelect::GetBugleInfo( iItemID ).m_iItemShopID > 0)
	{
		pWindow->SPSendMessage(SPIM_MICROPHONE_MODE, (WPARAM)2, (LPARAM)iItemID);			//MIC_CHAT_MONEY
	}
	else
	{
		return false;
	}
	
	SetMicItem(iContainerType, iSlotIndex);
	
	//pWindowTest->Show();
	pWindow->Show();
	
	return true;
}

bool SPPlayerInvenArchive::ItemPreUseEx( CONTAINER_TYPE iContainerType, int iSlotIndex )
{
	if(!m_FindItem.IsNull())
		return false;

	int iItemID = GetInventoryItem(iContainerType, iSlotIndex)->GetItemStatus()->GetItemID();

	SPWindow* pWindow = g_pInterfaceManager->FindWindow( WIID_FRIEND_FIND );

	int Level = 0;
	SPItemAttr* pItemAttr = NULL;
	g_pItemCluster->GetItemInfo( iItemID, pItemAttr );
	if( pItemAttr != NULL )
	{
		Level = pItemAttr->m_iItemLv;
		g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND, SPIM_ITEM_LEVEL, 0, (LPARAM)&Level );
	}

	if( NULL == pWindow )
	{
		return false;
	}

	SetFindItem( iContainerType, iSlotIndex );

	pWindow->Show();

	return true;
}

bool SPPlayerInvenArchive::FWItemPreUseEx( CONTAINER_TYPE iContainerType, int iSlotIndex )
{
	if(!m_FollowItem.IsNull())
		return false;

	int iItemID = GetInventoryItem(iContainerType, iSlotIndex)->GetItemStatus()->GetItemID();

	SPWindow* pWindow = g_pInterfaceManager->FindWindow( WIID_FRIEND_FOLLOW );

	if( NULL == pWindow )
	{
		return false;
	}

	SetFollowItem( iContainerType, iSlotIndex );

	pWindow->Show();

	return true;
}

bool SPPlayerInvenArchive::SItemPreUseEx( CONTAINER_TYPE iContainerType, int iSlotIndex )
{
	if(!m_SnatchItem.IsNull())
		return false;

	int iItemID = GetInventoryItem(iContainerType, iSlotIndex)->GetItemStatus()->GetItemID();

	SPWindow* pWindow = g_pInterfaceManager->FindWindow( WIID_FRIEND_SNATCH );

	if( NULL == pWindow )
	{
		return false;
	}

	SetSnatchItem( iContainerType, iSlotIndex );

	pWindow->Show();

	return true;
}

bool SPPlayerInvenArchive::SendItemUseMsg(CONTAINER_TYPE iContainerType, int iSlotIndex)
{
	
	CONTAINER_SLOT Slot;
	Slot.ContainerType = iContainerType;
	Slot.SlotIndex = iSlotIndex + 1;

	if(g_pCheckManager->IsDBBlock()) {
		
		DXUTOutputDebugString("\tSPPlayerInvenArchive::SendItemUseMsg DB Working Block\n");
		RecoverIcon();
		return false;
	}
	
	CPacket Packet(ITEM_CS_USING);
	Packet.Add((UINT64)g_pGOBManager->GetLocalPlayer()->GetGUID());
	Packet.AddData(&Slot, sizeof(CONTAINER_SLOT));		

	g_pNetworkManager->SPPerformMessage(ITEM_CS_USING, 0, (LPARAM)&Packet);	
	
	return true;
}


bool SPPlayerInvenArchive::SendBugle(std::string Msg /* =  */)
{
	if(Msg.empty())
		return false;

	int iMsgLen = static_cast<UINT8>(Msg.size());

	if(iMsgLen <= 0 || iMsgLen > LEN_CASHITEM_NORMAL_MSG )
	{
		Msg.resize( LEN_CASHITEM_NORMAL_MSG );
		iMsgLen = static_cast<UINT8>(Msg.size());
	}

	if(m_MicItem.IsNull())
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	//»ç¸Á½Ã Á¦ÇÑ
	if(g_pGOBManager->GetLocalPlayer()->IsDead())
		return false;

	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL)					//ÄÆÀÎ»óÅÂ Á¦ÇÑ
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE)	//Æ®·¹ÀÌµå Áß Á¦ÇÑ
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP)		//³»°¡ °³ÀÎ »óÁ¡°³¼³ÇÑ °æ¿ì
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetUserShopID())						//³»°¡ Å¸ÀÎÀÇ °³ÀÎ»óÁ¡¿¡ µé¾î°£ °æ¿ì
		return false;

	if(m_MicItem.m_iContainer < INVENTORY_EQUIP || m_MicItem.m_iContainer > INVENTORY_HOUSING)
		return false;

	if(!IsItemExist(m_MicItem.m_iSlotIndex, m_MicItem.m_iContainer))
		return false;

	SPItem* pUseItem = GetInventoryItem(m_MicItem.m_iContainer, m_MicItem.m_iSlotIndex);
	bool bItemAvailable = pUseItem->GetItemStatus()->IsTimeAvailable();
	if(bItemAvailable == false)
		return false;

	//ITEM_CS_CHAT,
	// CONTAINER_SLOT		UsingItemPos;		// »ç¿ëÇÒ ¾ÆÀÌÅÛ À§Ä¡Á¤º¸
	// ITEMNO				iUsingItemNo;		// »ç¿ëÇÒ ¾ÆÀÌÅÛ Å°¹øÈ£
	// BROADCAST_SCOPE		scope;				// ºê·ÎµåÄ³½ºÆÃ ¹üÀ§
	// UINT8				len;				// ¸Þ¼¼Áö ±æÀÌ
	// TCHAR				szMsg[];

	CONTAINER_SLOT Slot;
	Slot.ContainerType = m_MicItem.m_iContainer;
	Slot.SlotIndex = m_MicItem.m_iSlotIndex + 1;

	INT64 iItemNo = *(ITEMNO*)pUseItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
	int iItemID = pUseItem->GetItemStatus()->GetItemID();

	BugleInfo &info = SPWindowBugleSelect::GetBugleInfo( iItemID );
	/* Èç¹ûÓÐÇé¾°À®°ÈÕýÔÚ·¢ËÍ,Ôò¾Ü¾øÐÂµÄÇé¾°À®°È */
	if (BUGLE_CHANNELBG_VISIBLE == info.m_channelBgType)
	{
		SPWindowVisualBugle	*pWnd = (SPWindowVisualBugle *)g_pInterfaceManager->FindWindow( WIID_VISUAL_BUGLE );
		if (!pWnd || !pWnd->IsQueueReadyForNew())
		{
			DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );
			//PrintErrorMsg( szErrorMsg , 4000005 , NULL , NULL );
			_snprintf( szErrorMsg, MAX_CHATMSG_LEN-1, "%s", "Çé¾°À®°ÈÆµµÀÊ¹ÓÃÖÐ,ÇëÉÔºòÔÙÊÔ." );
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szErrorMsg );
			return false;
		}
	}

	BROADCAST_SCOPE scope;
	scope = SCOPE_WORLD;

	if(g_pCheckManager->IsDBBlock()) {
		DXUTOutputDebugString("\tSPPlayerInvenArchive::SendMicItemUseMsg DB Working Block\n");
		RecoverIcon();
		return false;
	}

	//
	CPacket Packet;
	Packet.Add((UINT32)MONEY_CS_CHAT);
	Packet.AddData(&Slot, sizeof(CONTAINER_SLOT));
	Packet.AddUINT64(iItemNo);
	Packet.AddUINT32(scope);
	Packet.AddUINT32(0);	// itemid
	Packet.AddUINT8(iMsgLen);
	Packet.AddData(Msg.c_str(), iMsgLen);
	g_pNetworkManager->SPPerformMessage(MONEY_CS_CHAT, 0, (LPARAM)&Packet);

	return true;
}
bool SPPlayerInvenArchive::FollowPeople( std::string Msg /* = ""  */)
{
	if(Msg.empty())
		return false;

	int iMsgLen = static_cast<UINT8>(Msg.size());

	if(iMsgLen <= 0 || iMsgLen > LEN_NAME )
		return false;

	if(m_FollowItem.IsNull())
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	//»ç¸Á½Ã Á¦ÇÑ
	if(g_pGOBManager->GetLocalPlayer()->IsDead())
		return false;

	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL)					//ÄÆÀÎ»óÅÂ Á¦ÇÑ
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE)	//Æ®·¹ÀÌµå Áß Á¦ÇÑ
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP)		//³»°¡ °³ÀÎ »óÁ¡°³¼³ÇÑ °æ¿ì
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetUserShopID())						//³»°¡ Å¸ÀÎÀÇ °³ÀÎ»óÁ¡¿¡ µé¾î°£ °æ¿ì
		return false;

	if(m_FollowItem.m_iContainer < INVENTORY_EQUIP || m_FollowItem.m_iContainer > INVENTORY_HOUSING)
		return false;

	if(!IsItemExist(m_FollowItem.m_iSlotIndex, m_FollowItem.m_iContainer))
		return false;

	SPItem* pUseItem = GetInventoryItem(m_FollowItem.m_iContainer, m_FollowItem.m_iSlotIndex);
	bool bItemAvailable = pUseItem->GetItemStatus()->IsTimeAvailable();
	if(bItemAvailable == false)
		return false;

	CONTAINER_SLOT Slot;
	Slot.ContainerType = m_FollowItem.m_iContainer;
	Slot.SlotIndex = m_FollowItem.m_iSlotIndex + 1;

	INT64 iItemNo = *(ITEMNO*)pUseItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
	int iItemID = pUseItem->GetItemStatus()->GetItemID();
	GU_ID iItemGUID = iItemID;

	if(g_pCheckManager->IsDBBlock()) 
	{
		DXUTOutputDebugString("\tSPPlayerInvenArchive::SendWhere DB Working Block\n");
		RecoverIcon();
		return false;
	}

	DECLARE_VAR( TCHAR , szTargetCharName , LEN_NAME + 1 );
	strcpy( szTargetCharName, Msg.c_str() );

	//ITEM_CS_TRACK,									//¸úËæ£¬×¥ÈËµÀ¾ß
		// GU_ID
		// CONTAINER_SLOT
		// TCHAR			szTargetCharName[LEN_NAME+1];


	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_TRACK);
	Packet.AddUINT64( iItemGUID );
	Packet.AddData(&Slot, sizeof(CONTAINER_SLOT));
	Packet.AddData( szTargetCharName, LEN_NAME + 1 );
	g_pNetworkManager->SPPerformMessage(ITEM_CS_TRACK, 0, (LPARAM)&Packet);

	return true;
}

bool SPPlayerInvenArchive::SnatchFriend( std::string Msg /* = ""  */)
{
	if(Msg.empty())
		return false;

	int iMsgLen = static_cast<UINT8>(Msg.size());

	if(iMsgLen <= 0 || iMsgLen > LEN_NAME )
		return false;

	if(m_SnatchItem.IsNull())
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	//»ç¸Á½Ã Á¦ÇÑ
	if(g_pGOBManager->GetLocalPlayer()->IsDead())
		return false;

	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL)					//ÄÆÀÎ»óÅÂ Á¦ÇÑ
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE)	//Æ®·¹ÀÌµå Áß Á¦ÇÑ
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP)		//³»°¡ °³ÀÎ »óÁ¡°³¼³ÇÑ °æ¿ì
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetUserShopID())						//³»°¡ Å¸ÀÎÀÇ °³ÀÎ»óÁ¡¿¡ µé¾î°£ °æ¿ì
		return false;

	if(m_SnatchItem.m_iContainer < INVENTORY_EQUIP || m_SnatchItem.m_iContainer > INVENTORY_HOUSING)
		return false;

	if(!IsItemExist(m_SnatchItem.m_iSlotIndex, m_SnatchItem.m_iContainer))
		return false;

	SPItem* pUseItem = GetInventoryItem(m_SnatchItem.m_iContainer, m_SnatchItem.m_iSlotIndex);
	bool bItemAvailable = pUseItem->GetItemStatus()->IsTimeAvailable();
	if(bItemAvailable == false)
		return false;

	CONTAINER_SLOT Slot;
	Slot.ContainerType = m_SnatchItem.m_iContainer;
	Slot.SlotIndex = m_SnatchItem.m_iSlotIndex + 1;

	INT64 iItemNo = *(ITEMNO*)pUseItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
	int iItemID = pUseItem->GetItemStatus()->GetItemID();
	GU_ID iItemGUID = iItemID;

	if(g_pCheckManager->IsDBBlock()) 
	{
		DXUTOutputDebugString("\tSPPlayerInvenArchive::SendWhere DB Working Block\n");
		RecoverIcon();
		return false;
	}

	DECLARE_VAR( TCHAR , szTargetCharName , LEN_NAME + 1 );
	strcpy( szTargetCharName, Msg.c_str() );

	//ITEM_CS_TRACK,									//¸úËæ£¬×¥ÈËµÀ¾ß
	// GU_ID
	// CONTAINER_SLOT
	// TCHAR			szTargetCharName[LEN_NAME+1];

	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_TRACK);
	Packet.AddUINT64( iItemGUID );
	Packet.AddData(&Slot, sizeof(CONTAINER_SLOT));
	Packet.AddData( szTargetCharName, LEN_NAME + 1 );
	g_pNetworkManager->SPPerformMessage(ITEM_CS_TRACK, 0, (LPARAM)&Packet);

	return true;
}
bool SPPlayerInvenArchive::SendWhereEx( std::string Msg /* = "" */, int iID /* = 0 */, WINDOW_STATE WinState /* = WINDOW_NULL  */)
{
	DECLARE_VAR( TCHAR , szTargetCharName , LEN_NAME + 1 );
	strcpy( szTargetCharName, Msg.c_str() );
	
	CONTAINER_SLOT Slot;
	Slot.ContainerType = CONTAINER_TYPE_INVALID;
	Slot.SlotIndex = -1;
	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_WHERE);
	Packet.AddUINT64( 0 );
	Packet.AddData(&Slot, sizeof(CONTAINER_SLOT));
	Packet.AddUINT32( iID );
	Packet.AddUINT32( WinState );
	Packet.AddData( szTargetCharName, LEN_NAME + 1 );
	g_pNetworkManager->SPPerformMessage(ITEM_CS_WHERE, 0, (LPARAM)&Packet);

	return true;
}
bool SPPlayerInvenArchive::SendWhere( std::string Msg /* = ""  */, int iID, WINDOW_STATE WinState )
{
	if ( iID != 0 )
	{
		SendWhereEx( Msg, iID, WinState );
		return true;
	}
	if(Msg.empty())
		return false;

	int iMsgLen = static_cast<UINT8>(Msg.size());

	if(iMsgLen <= 0 || iMsgLen > LEN_NAME )
		return false;

	if(m_FindItem.IsNull())
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	//»ç¸Á½Ã Á¦ÇÑ
	if(g_pGOBManager->GetLocalPlayer()->IsDead())
		return false;

	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL)					//ÄÆÀÎ»óÅÂ Á¦ÇÑ
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE)	//Æ®·¹ÀÌµå Áß Á¦ÇÑ
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP)		//³»°¡ °³ÀÎ »óÁ¡°³¼³ÇÑ °æ¿ì
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetUserShopID())						//³»°¡ Å¸ÀÎÀÇ °³ÀÎ»óÁ¡¿¡ µé¾î°£ °æ¿ì
		return false;

	if(m_FindItem.m_iContainer < INVENTORY_EQUIP || m_FindItem.m_iContainer > INVENTORY_HOUSING)
		return false;

	if(!IsItemExist(m_FindItem.m_iSlotIndex, m_FindItem.m_iContainer))
		return false;

	SPItem* pUseItem = GetInventoryItem(m_FindItem.m_iContainer, m_FindItem.m_iSlotIndex);
	bool bItemAvailable = pUseItem->GetItemStatus()->IsTimeAvailable();
	if(bItemAvailable == false)
		return false;

			

	CONTAINER_SLOT Slot;
	Slot.ContainerType = m_FindItem.m_iContainer;
	Slot.SlotIndex = m_FindItem.m_iSlotIndex + 1;

	INT64 iItemNo = *(ITEMNO*)pUseItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
	int iItemID = pUseItem->GetItemStatus()->GetItemID();
	GU_ID iItemGUID = iItemID;

	if(g_pCheckManager->IsDBBlock()) 
	{
		DXUTOutputDebugString("\tSPPlayerInvenArchive::SendWhere DB Working Block\n");
		RecoverIcon();
		return false;
	}

	DECLARE_VAR( TCHAR , szTargetCharName , LEN_NAME + 1 );
	strcpy( szTargetCharName, Msg.c_str() );

//	ITEM_CS_WHERE,
		// GU_ID				Å¸°Ù ¿£Æ¼Æ¼ ¾ÆÀÌµð
		// CONTAINER_SLOT		À§Ä¡Á¤º¸
		// UINT32	Seq
		// TCHAR	szTargetCharName[LEN_NAME+1];
	
	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_WHERE);
	Packet.AddUINT64( iItemGUID );
	Packet.AddData(&Slot, sizeof(CONTAINER_SLOT));
	Packet.AddUINT32( iID );
	Packet.AddUINT32( WinState );
	Packet.AddData( szTargetCharName, LEN_NAME + 1 );
	g_pNetworkManager->SPPerformMessage(ITEM_CS_WHERE, 0, (LPARAM)&Packet);

	return true;
}
bool SPPlayerInvenArchive::SendMicItemUseMsg(std::string Msg /*= ""*/)
{
	if(Msg.empty())
		return false;

	int iMsgLen = static_cast<UINT8>(Msg.size());

	if(iMsgLen <= 0 || iMsgLen > LEN_CASHITEM_NORMAL_MSG )
		return false;
	
	if(m_MicItem.IsNull())
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	//»ç¸Á½Ã Á¦ÇÑ
	if(g_pGOBManager->GetLocalPlayer()->IsDead())
		return false;

	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL)					//ÄÆÀÎ»óÅÂ Á¦ÇÑ
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE)	//Æ®·¹ÀÌµå Áß Á¦ÇÑ
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP)		//³»°¡ °³ÀÎ »óÁ¡°³¼³ÇÑ °æ¿ì
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetUserShopID())						//³»°¡ Å¸ÀÎÀÇ °³ÀÎ»óÁ¡¿¡ µé¾î°£ °æ¿ì
		return false;

	if(m_MicItem.m_iContainer < INVENTORY_EQUIP || m_MicItem.m_iContainer > INVENTORY_HOUSING)
		return false;

	if(!IsItemExist(m_MicItem.m_iSlotIndex, m_MicItem.m_iContainer))
		return false;

	SPItem* pUseItem = GetInventoryItem(m_MicItem.m_iContainer, m_MicItem.m_iSlotIndex);
	bool bItemAvailable = pUseItem->GetItemStatus()->IsTimeAvailable();
	if(bItemAvailable == false)
		return false;

	//ITEM_CS_CHAT,
	// CONTAINER_SLOT		UsingItemPos;		// »ç¿ëÇÒ ¾ÆÀÌÅÛ À§Ä¡Á¤º¸
	// ITEMNO				iUsingItemNo;		// »ç¿ëÇÒ ¾ÆÀÌÅÛ Å°¹øÈ£
	// BROADCAST_SCOPE		scope;				// ºê·ÎµåÄ³½ºÆÃ ¹üÀ§
	// UINT8				len;				// ¸Þ¼¼Áö ±æÀÌ
	// TCHAR				szMsg[];

	CONTAINER_SLOT Slot;
	Slot.ContainerType = m_MicItem.m_iContainer;
	Slot.SlotIndex = m_MicItem.m_iSlotIndex + 1;

	INT64 iItemNo = *(ITEMNO*)pUseItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
	int iItemID = pUseItem->GetItemStatus()->GetItemID();
	
	BROADCAST_SCOPE scope;
	if(iItemID == g_pCheckManager->GetMicServerItemID()) {
		scope = SCOPE_WORLD;
	}
	else if(iItemID == g_pCheckManager->GetMicChannelItemID()) {
		scope = SCOPE_CHANNEL;
	}
	else {
		return false;
	}	

	if(g_pCheckManager->IsDBBlock()) {
		DXUTOutputDebugString("\tSPPlayerInvenArchive::SendMicItemUseMsg DB Working Block\n");
		RecoverIcon();
		return false;
	}

	//
	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_CHAT);
	Packet.AddData(&Slot, sizeof(CONTAINER_SLOT));
	Packet.AddUINT64(iItemNo);
	Packet.AddUINT32(scope);
	Packet.AddUINT8(iMsgLen);
	Packet.AddData(Msg.c_str(), iMsgLen);
	g_pNetworkManager->SPPerformMessage(ITEM_CS_CHAT, 0, (LPARAM)&Packet);
	
	return true;
}


int SPPlayerInvenArchive::ItemGridRClick(int iSlotIndex)		//ÀÌÁ¦ ÀÌ°ÍÀ» »ç¿ëÇÏÁö ¾ÊÀ½...
{	
	iSlotIndex = iSlotIndex + (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);

	//if(g_pEventManager->GetCutInState() != CUT_IN_STATE_TRADE)
	//	return 1;
	
	if(m_iCurrentInventoryType < CONTAINER_TYPE_INVALID || m_iCurrentInventoryType > INVENTORY_HOUSING)	
		return 1;
	
	if(IsItemExist(iSlotIndex, m_iCurrentInventoryType) == false)
		return 1;

	SPItem* pItem = GetInventoryItem(m_iCurrentInventoryType, iSlotIndex);
	if(pItem->GetItemAttr()->m_bCashCheck && pItem->GetItemAttr()->m_iShopID) {
		SPCashTradeManager::GetInstance()->SetSelectCashItem(pItem->GetItemAttr()->m_iShopID);
	}
	
	//ClearOutSideControl();
		
	return 1;	
}


void	SPPlayerInvenArchive::SettingCurrentInventory()
{	
	ReflashInventory();					//½ºÅÈ º¯°æ½Ã¸¶´Ù È£ÃâÇØÁà¾ß ÇÒµí...
}


void	SPPlayerInvenArchive::SettingCurrentEquip()
{	
	OnChangeEquip((LPARAM)m_iCurrentFigureType);
}


void SPPlayerInvenArchive::SlotMerge(CHANGE_SLOT item)
{
	int iDestItemID = 0;
	int iDestStackCount = 0;
	int iDestSlotIndex = item.DestSlotInfo.SlotIndex ;
	CONTAINER_TYPE iDestContainerType = item.DestSlotInfo.ContainerType;
	SPItem* pDestItem = NULL;

	int iSrcItemID = 0;	
	int iSrcStackCount = 0;
	int iSrcSlotIndex = item.SrcSlotInfo.SlotIndex ;
	CONTAINER_TYPE iSrcContainerType = item.SrcSlotInfo.ContainerType;
	SPItem* pSrcItem = NULL;

	iSrcItemID = m_vpInventory[iSrcContainerType].at(iSrcSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
	iSrcStackCount = m_vpInventory[iSrcContainerType].at(iSrcSlotIndex)->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
	pSrcItem = GetInventoryItem(iSrcContainerType, iSrcSlotIndex);
	
	iDestItemID = m_vpInventory[iDestContainerType].at(iDestSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);	
	iDestStackCount = m_vpInventory[iDestContainerType].at(iDestSlotIndex)->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
	pDestItem = GetInventoryItem(iDestContainerType, iDestSlotIndex);
	
	//if(iDestContainerType != iSrcContainerType)
	//	return;

	if(iSrcItemID != iDestItemID)
		return;

	iSrcStackCount = iSrcStackCount - item.iCount;
	iDestStackCount = iDestStackCount + item.iCount;	
	
	pDestItem->SetItemStatusValue(MV_ITEM_SETSTACK_CNT, (LPARAM)iDestStackCount);

	if(iSrcStackCount > 0)
		pSrcItem->SetItemStatusValue(MV_ITEM_SETSTACK_CNT, (LPARAM)iSrcStackCount);	
	
	if(iDestContainerType == STORAGE) {
		iDestSlotIndex = iDestSlotIndex - (m_iCurStoragePage * STORAGE_LINE);
		g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_SETSTACK, (WPARAM)iDestSlotIndex, (LPARAM)iDestStackCount, NULL);
	}
	else if(iDestContainerType > CONTAINER_TYPE_INVALID && iDestContainerType < FIGURE_EQUIP) {
		iDestSlotIndex = iDestSlotIndex - (m_iCurInvenPage[iDestContainerType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SETSTACK, (WPARAM)iDestSlotIndex, (LPARAM)iDestStackCount, NULL);		
	}

	if(iSrcStackCount < 1) {
		pSrcItem->SetClear();
		if(iSrcContainerType == STORAGE) {
			iSrcSlotIndex = iSrcSlotIndex - (m_iCurStoragePage * STORAGE_LINE);			
			g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_CLEAR, 0, (LPARAM)iSrcSlotIndex, NULL);
		}
		else if(iSrcContainerType > CONTAINER_TYPE_INVALID && iSrcContainerType < FIGURE_EQUIP) {
			iSrcSlotIndex = iSrcSlotIndex - (m_iCurInvenPage[iSrcContainerType] * INVENTORY_LINE);
			if(iSrcSlotIndex >= 0 && iSrcSlotIndex < VIEW_SLOT)
				g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_CLEAR, 0, (LPARAM)iSrcSlotIndex, NULL);
		}
	}
	else {
		if(iSrcContainerType == STORAGE) {
			iSrcSlotIndex = iSrcSlotIndex - (m_iCurStoragePage * STORAGE_LINE);			
			g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_SETSTACK, (WPARAM)iSrcSlotIndex, (LPARAM)iSrcStackCount, NULL);
		}
		else if(iSrcContainerType > CONTAINER_TYPE_INVALID && iSrcContainerType < FIGURE_EQUIP) {
			iSrcSlotIndex = iSrcSlotIndex - (m_iCurInvenPage[iSrcContainerType] * INVENTORY_LINE);
			if(iSrcSlotIndex >= 0 && iSrcSlotIndex < VIEW_SLOT)
				g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SETSTACK, (WPARAM)iSrcSlotIndex, (LPARAM)iSrcStackCount, NULL);
		}
	}
	
	if(iDestContainerType > CONTAINER_TYPE_INVALID && iDestContainerType < FIGURE_EQUIP) {
		if(iDestItemID > 0) {
			g_pEventManager->GetQuestManager()->CheckMissionItem(iDestItemID);
		}
	}

	RecoverIcon();
	return;
}


void	SPPlayerInvenArchive::SlotChange( CHANGE_SLOT item )
{	
	int iDestItemID = 0;
	int iDestSlotIndex = item.DestSlotInfo.SlotIndex ;
	CONTAINER_TYPE iDestContainerType = item.DestSlotInfo.ContainerType;	
	
	int iSrcItemID = 0;	
	int iSrcSlotIndex = item.SrcSlotInfo.SlotIndex ;
	CONTAINER_TYPE iSrcContainerType = item.SrcSlotInfo.ContainerType;
	
	iSrcItemID = m_vpInventory[iSrcContainerType].at(iSrcSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);	
	iDestItemID = m_vpInventory[iDestContainerType].at(iDestSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);		

	DXUTOutputDebugString("Swap Àü SrcContainer[%d] SrcSlot[%d] SrcItemID[%d] - DestContainer[%d] DestSlot[%d] DestItemID[%d]\n", iSrcContainerType, iSrcSlotIndex, iSrcItemID, iDestContainerType, iDestSlotIndex, iDestItemID);

	if(iSrcItemID && (iSrcContainerType == FIGURE_EQUIP || iSrcContainerType == FIGURE_FASHION 
		|| iSrcContainerType == FIGURE_PET || iSrcContainerType == FIGURE_EFFECT)) {
		SPItem* pItem = NULL;
		CONTAINER_ITEM tempItem;
		pItem = GetInventoryItem(iSrcContainerType, iSrcSlotIndex);
		if(pItem) {
			pItem->ConvertContainerItem(&tempItem);
			tempItem.SlotInfo.ContainerType = iSrcContainerType;
			tempItem.SlotInfo.SlotIndex = iSrcSlotIndex;
			m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&tempItem);
		}		
	}

	if(iDestItemID && (iDestContainerType == FIGURE_EQUIP || iDestContainerType == FIGURE_FASHION 
		|| iDestContainerType == FIGURE_PET || iDestContainerType == FIGURE_EFFECT)) {
		SPItem* pItem = NULL;
		CONTAINER_ITEM tempItem;
		pItem = GetInventoryItem(iDestContainerType, iDestSlotIndex);
		if(pItem) {
			pItem->ConvertContainerItem(&tempItem);
			tempItem.SlotInfo.ContainerType = iDestContainerType;
			tempItem.SlotInfo.SlotIndex = iDestSlotIndex;
			m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&tempItem);
		}		
	}
	
	//Swap ¿©±â¼­ Src ¿Í Dest°¡ ¹Ù²ï´Ù
	swap(m_vpInventory[iDestContainerType].at(iDestSlotIndex), m_vpInventory[iSrcContainerType].at(iSrcSlotIndex));	
	m_vpInventory[iDestContainerType].at(iDestSlotIndex)->SetItem(iDestContainerType, iDestSlotIndex);
	m_vpInventory[iSrcContainerType].at(iSrcSlotIndex)->SetItem(iSrcContainerType, iSrcSlotIndex);	
		
	iSrcItemID = m_vpInventory[iSrcContainerType].at(iSrcSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
	iDestItemID = m_vpInventory[iDestContainerType].at(iDestSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
	int iSrcSlotCount = GetSlotCount(iSrcContainerType, iSrcSlotIndex);
	int iDestSlotCount = GetSlotCount(iDestContainerType, iDestSlotIndex);	//ÀÌ°Ç ÇÊ¼öÀÓ...
	//Test Print
	DXUTOutputDebugString("Swap ÈÄ SrcContainer[%d] SrcSlot[%d] SrcItemID[%d] SrcSlotCount[%d] - DestContainer[%d] DestSlot[%d] DestItemID[%d] DestSlotCount[%d]\n", iSrcContainerType, iSrcSlotIndex, iSrcItemID, iSrcSlotCount, iDestContainerType, iDestSlotIndex, iDestItemID, iDestSlotCount);
	
	//[2005/10/17] ´Ù¸¥ Å¸ÀÔÀÇ ÄÁÅ×ÀÌ³Ê·Î ¹Ù²î´Â °æ¿ì
	if(iDestContainerType != iSrcContainerType) {		
		if(iDestItemID > 0) {
			//Quest Mission Check...
			g_pEventManager->GetQuestManager()->CheckMissionItem(iDestItemID);
		}
		
		if(iSrcItemID > 0) {
			g_pEventManager->GetQuestManager()->CheckMissionItem(iSrcItemID);
		}
	}

	CONTAINER_ITEM DestItem;
	CONTAINER_ITEM SrcItem;
	m_vpInventory[iDestContainerType].at(iDestSlotIndex)->ConvertContainerItem((CONTAINER_ITEM*)&DestItem);
	m_vpInventory[iSrcContainerType].at(iSrcSlotIndex)->ConvertContainerItem((CONTAINER_ITEM*)&SrcItem);

	//[2005/7/8]	Delete PickUp Icon
	if(SrcItem.SlotInfo.ContainerType < INVENTORY_EQUIP || SrcItem.SlotInfo.ContainerType >= CONTAINER_MAX) {
		if(m_PickUpItem.m_iContainer > CONTAINER_TYPE_INVALID && m_PickUpItem.m_iContainer < FIGURE_EQUIP) {
			int iClearIndex = m_PickUpItem.m_iSlotIndex - (m_iCurInvenPage[m_PickUpItem.m_iContainer] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_CLEAR, 0, iClearIndex, NULL);
		}
		else if((m_PickUpItem.m_iContainer > INVENTORY_HOUSING && m_PickUpItem.m_iContainer < FIGURE_BASIC)
			|| m_PickUpItem.m_iContainer == FIGURE_EFFECT)
		{
			if(m_PickUpItem.m_iSlotIndex < EQ_WEAPON1 - 1) {
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, m_PickUpItem.m_iSlotIndex, NULL);				
			}			
			else if(m_PickUpItem.m_iSlotIndex == EQ_WEAPON1 - 1 || m_PickUpItem.m_iSlotIndex == EQ_BUCKLER1 - 1) {	//¹«±â 1
				int iWeapon1 = m_PickUpItem.m_iSlotIndex - (EQ_WEAPON1 - 1);
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR1, 0, iWeapon1, NULL);
				
				int iSlotCount = GetSlotCount(iSrcContainerType, iSrcSlotIndex);
				if(iSlotCount == 1) {
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR1, 0, iWeapon1 + 1, NULL);
				}
			}
			else if(m_PickUpItem.m_iSlotIndex == EQ_WEAPON2 - 1 || m_PickUpItem.m_iSlotIndex == EQ_BUCKLER2 - 1) {	//¹«±â 2
				int iWeapon2 = m_PickUpItem.m_iSlotIndex - (EQ_WEAPON2 - 1);
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR2, 0, iWeapon2, NULL);

				int iSlotCount = GetSlotCount(iSrcContainerType, iSrcSlotIndex);
				if(iSlotCount == 1) {
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR2, 0, iWeapon2 + 1, NULL);
				}
			}
		}
	}
	
	//RecoverIcon();
	m_PickUpItem.Clear();
	m_bPickup = false;
	DeletePickupItem();	

	//Dest Icon Setting	
	if(DestItem.Item.iItemID != 0) {
		SetInventoryIcon(&DestItem);		
	}	
	
	//Src Icon Setting
	if(SrcItem.Item.iItemID != 0) {		
		SetInventoryIcon(&SrcItem);
	}	

	//¿ÊÀÌ ¹þ°ÜÁö´Â °æ¿ìÀÇ ¿¹¿Ü Ã³¸®...
	if(iSrcItemID == 0 && (iSrcContainerType == FIGURE_EQUIP || iSrcContainerType == FIGURE_FASHION
		|| iSrcContainerType == FIGURE_EFFECT)) {
		//ÇÑ¹ú¿ÊÀÌ ÀÎº¥Åä¸®·Î ¿Ã¶ó°£ °æ¿ì
		if( iDestSlotCount == 3 && iSrcSlotIndex == 3 ) {
			bool bEquipExist = false;
			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)iSrcSlotIndex, NULL);
			m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_ONE_PIECE);

			bEquipExist = IsItemExist(3, FIGURE_EQUIP);
			if(bEquipExist) {
				int iEquipItemID = m_vpInventory[FIGURE_EQUIP].at(3)->GetItemStatusValue(MV_ITEM_GETUID);
				if(iEquipItemID!=0)
				{
					//Item ¸¸µé±â
					CONTAINER_ITEM item;
					item.Item.iItemID = iEquipItemID;
					item.Item.iStackCount = 1;
					m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
				}
			}
			else {
				m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(3 + 1));			
			}			
			
			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)4, NULL);	
			bEquipExist = IsItemExist(4, FIGURE_EQUIP);		//ÇÑ¹ú¿ÊÀ» ¹þÀº °æ¿ì Àåºñ·Î ·£´õ
			if(bEquipExist) {
				int iEquipItemID = m_vpInventory[FIGURE_EQUIP].at(4)->GetItemStatusValue(MV_ITEM_GETUID);
				if(iEquipItemID!=0)
				{
					//Item ¸¸µé±â
					CONTAINER_ITEM item;
					item.Item.iItemID = iEquipItemID;
					item.Item.iStackCount = 1;
					m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
				}
			}
			else {
				m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(4 + 1));			
			}			
			
			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)5, NULL);
			bEquipExist = IsItemExist(5, FIGURE_EQUIP);		//ÇÑ¹ú¿ÊÀ» ¹þÀº °æ¿ì Àåºñ·Î ·£´õ
			if(bEquipExist) {
				int iEquipItemID = m_vpInventory[FIGURE_EQUIP].at(5)->GetItemStatusValue(MV_ITEM_GETUID);
				if(iEquipItemID!=0)
				{
					//Item ¸¸µé±â
					CONTAINER_ITEM item;
					item.Item.iItemID = iEquipItemID;
					item.Item.iStackCount = 1;
					m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
				}
			}
			else {				
				m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(5 + 1));	
			}
		}
		else {
			if(iSrcContainerType == FIGURE_EQUIP) {
				int iSrcSlotCount = GetSlotCount(iSrcContainerType, iSrcSlotIndex);
				if(iSrcSlotIndex < EQ_WEAPON1 - 1) {
					bool bFashionExist = false;

					//ÇØ´ç ¾ÆÀÌÄÜÀ» Áö¿ì°í Àåºñ¸¦ ¾ð·Îµå ÇÑ´Ù
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)iSrcSlotIndex, NULL);					

					bFashionExist = IsItemExist(iSrcSlotIndex, FIGURE_FASHION);
					if(bFashionExist) {
						int iFashionItemID = m_vpInventory[FIGURE_FASHION].at(iSrcSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
						if(iFashionItemID!=0) {
							//Item ¸¸µé±â
							CONTAINER_ITEM item;
							item.Item.iItemID = iFashionItemID;
							item.Item.iStackCount = 1;
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
						}
					}
					else {
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(iSrcSlotIndex + 1));
						
						////[2006/11/23] Àåºñ Å»Âø
						//CONTAINER_ITEM tempItem;
						//CopyMemory(&tempItem, &DestItem, sizeof(CONTAINER_ITEM));
						//tempItem.SlotInfo.ContainerType = SrcItem.SlotInfo.ContainerType;
						//tempItem.SlotInfo.SlotIndex = SrcItem.SlotInfo.SlotIndex;
						//CheckSetItem(tempItem.Item.iItemID);
						//m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&tempItem);
					}
				}
				else if(iSrcSlotIndex == EQ_WEAPON1 - 1 || iSrcSlotIndex == EQ_BUCKLER1 - 1) {	//
					int iClearSlot = iSrcSlotIndex - (EQ_WEAPON1 - 1);
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR1, 0, (LPARAM)iClearSlot, NULL);

					int iSlotCount = GetSlotCount(iDestContainerType, iDestSlotIndex);
					if(iSrcSlotIndex == EQ_WEAPON1 - 1 && iSlotCount == 2) {
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR1, 0, iClearSlot + 1, NULL);
					}
					
					if(m_iCurrentWeaponSlot == 1) {
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(iSrcSlotIndex + 1));
						
						////[2006/11/23] Àåºñ Å»Âø
						//CONTAINER_ITEM tempItem;
						//CopyMemory(&tempItem, &DestItem, sizeof(CONTAINER_ITEM));
						//tempItem.SlotInfo.ContainerType = SrcItem.SlotInfo.ContainerType;
						//tempItem.SlotInfo.SlotIndex = SrcItem.SlotInfo.SlotIndex;
						//CheckSetItem(tempItem.Item.iItemID);
						//m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&tempItem);
					}
				}
				else if(iSrcSlotIndex == EQ_WEAPON2 - 1 || iSrcSlotIndex == EQ_BUCKLER2 - 1) {	//
					int iClearSlot = iSrcSlotIndex - (EQ_WEAPON2 - 1);
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR2, 0, (LPARAM)iClearSlot, NULL);
					

					int iSlotCount = GetSlotCount(iDestContainerType, iDestSlotIndex);
					if(iSrcSlotIndex == EQ_WEAPON2 - 1 && iSlotCount == 2) {
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR2, 0, iClearSlot + 1, NULL);
					}					
					
					if(m_iCurrentWeaponSlot != 1) {
						if(iSrcSlotIndex == EQ_WEAPON2 - 1) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_WEAPON1);							
						}
						else if(iSrcSlotIndex == EQ_BUCKLER2 - 1) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_BUCKLER1);
						}

						////[2006/11/23] Àåºñ Å»Âø
						//CONTAINER_ITEM tempItem;
						//CopyMemory(&tempItem, &DestItem, sizeof(CONTAINER_ITEM));
						//tempItem.SlotInfo.ContainerType = SrcItem.SlotInfo.ContainerType;
						//tempItem.SlotInfo.SlotIndex = SrcItem.SlotInfo.SlotIndex;
						//CheckSetItem(tempItem.Item.iItemID);
						//m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&tempItem);
					}
				}
			}
			else if(iSrcContainerType == FIGURE_FASHION) {
				if(iSrcSlotIndex < EQ_WEAPON1 - 1) {
					bool bEquipExist = false;

					//ÇØ´ç ¾ÆÀÌÄÜÀ» Áö¿ì°í Àåºñ¸¦ ¾ð·Îµå ÇÑ´Ù
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)iSrcSlotIndex, NULL);

					bEquipExist = IsItemExist(iSrcSlotIndex, FIGURE_EQUIP);
					if(bEquipExist) {
						int iEquipItemID = m_vpInventory[FIGURE_EQUIP].at(iSrcSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
						if(iEquipItemID!=0)
						{
							//Item ¸¸µé±â
							CONTAINER_ITEM item;
							item.Item.iItemID = iEquipItemID;
							item.Item.iStackCount = 1;
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
						}
					}
					else {
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(iSrcSlotIndex + 1));
						
						////[2006/11/23] Àåºñ Å»Âø
						//CONTAINER_ITEM tempItem;
						//CopyMemory(&tempItem, &DestItem, sizeof(CONTAINER_ITEM));
						//tempItem.SlotInfo.ContainerType = SrcItem.SlotInfo.ContainerType;
						//tempItem.SlotInfo.SlotIndex = SrcItem.SlotInfo.SlotIndex;
						//CheckSetItem(tempItem.Item.iItemID);
						//m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&tempItem);
					}
				}
			}
			else if (iSrcContainerType == FIGURE_EFFECT)
			{
				if(iSrcSlotIndex < EQ_WEAPON1 - 1)
				{
					bool bEquipExist = false;
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)iSrcSlotIndex, NULL);
				}
			}
		}		
		
		////[2006/11/23] Àåºñ Å»Âø
		//CONTAINER_ITEM tempItem;
		//CopyMemory(&tempItem, &DestItem, sizeof(CONTAINER_ITEM));
		//tempItem.SlotInfo.ContainerType = SrcItem.SlotInfo.ContainerType;
		//tempItem.SlotInfo.SlotIndex = SrcItem.SlotInfo.SlotIndex;
		//CheckSetItem(tempItem.Item.iItemID);
		//m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&tempItem);
	}
	else if(iSrcItemID == 0 && (iSrcContainerType > CONTAINER_TYPE_INVALID && iSrcContainerType < FIGURE_EQUIP)) {
		int iClearIndex =  iSrcSlotIndex - (m_iCurInvenPage[iSrcContainerType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_CLEAR, 0, (LPARAM)iClearIndex, NULL);
	}
	else if(iSrcItemID == 0 && iSrcContainerType == STORAGE) {
		int iClearIndex =  iSrcSlotIndex - (m_iCurStoragePage * STORAGE_LINE);
		g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_CLEAR, 0, (LPARAM)iClearIndex, NULL);
	}
	else if(iSrcItemID == 0 && iSrcContainerType == FIGURE_PET) {
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PET);
		if(pWindow) {
			pWindow->SPSendMessage(SPIM_ITEM_CLEAR, 0, (LPARAM)iSrcSlotIndex, NULL);
		}		

		////[2006/11/23] Àåºñ Å»Âø
		//CONTAINER_ITEM tempItem;
		//CopyMemory(&tempItem, &DestItem, sizeof(CONTAINER_ITEM));
		//tempItem.SlotInfo.ContainerType = SrcItem.SlotInfo.ContainerType;
		//tempItem.SlotInfo.SlotIndex = SrcItem.SlotInfo.SlotIndex;
		//m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&tempItem);
	}	

	//[2005/9/7] CHANGE_SLOTÀÇ Á¤º¸¸¦ºñ¿ö ³õ´Â´Ù.
	m_ChangeSlot.DestSlotInfo.ContainerType = CONTAINER_TYPE_INVALID;
	m_ChangeSlot.iCount = 0;
	m_ChangeSlot.SrcSlotInfo.ContainerType = CONTAINER_TYPE_INVALID;

	//[2005/5/23] ¾ÆÀÌÅÛÀ» ±³È¯ÇÏ´Â(¹Ù²Ù´Â) °æ¿ì ÆÇ¸ÅÃ¢¿¡ ¿Ã¸° ItemÀ» NULL·Î ¹Ù²Û´Ù
	ClearOutSideControl();
}


bool SPPlayerInvenArchive::ItemSetting( CONTAINER_ITEM item, bool bSetEquip /*= true*/)
{	
	if(IsEnableContainer(item.SlotInfo.ContainerType) == false)
		return false;
	
	//if(item.Item.iItemID==0)
	//	return false;

	bool bItemExist = false;

	if(bSetEquip && item.SlotInfo.ContainerType == FIGURE_BASIC) {
		m_pParent->SPGOBPerformMessage( SPGM_EQUIP_DEFAULT_CHANGE , (LPARAM)&item);
		m_vpInventory[item.SlotInfo.ContainerType].at(item.SlotInfo.SlotIndex)->SetItemStatus(&item);
		m_vpInventory[item.SlotInfo.ContainerType].at(item.SlotInfo.SlotIndex)->SetItem(item.SlotInfo.ContainerType, item.SlotInfo.SlotIndex);	
		if(item.Item.iItemID != 0)
			ReflashFigure();
		return true;
	}

	//»çÀÌÁî Ã¼Å© ÈÄ
	//¾ÆÀÌÅÛ¼ÂÆÃÇÒ°Í
	if(item.SlotInfo.SlotIndex < 0) {
		return false;
	}
	else if(item.SlotInfo.SlotIndex >= GetInventorySize(item.SlotInfo.ContainerType)) {
		return false;
	}

	//[xialin 2008/07/22] add >>>
	if ( item.SlotInfo.ContainerType == INVENTORY_WEAR )
	{
		int iWearSize = GetInventorySize(item.SlotInfo.ContainerType);
		SPItem * pkItem = m_vpInventory[item.SlotInfo.ContainerType].at(iWearSize-1);
		for (int i = iWearSize-1; i > item.SlotInfo.SlotIndex; i--)
		{
			m_vpInventory[item.SlotInfo.ContainerType].at(i) = m_vpInventory[item.SlotInfo.ContainerType].at(i-1);
		}
		m_vpInventory[item.SlotInfo.ContainerType].at(item.SlotInfo.SlotIndex) = pkItem;
	}
	//[xialin 2008/07/22] add <<<
	
	//ÀÎº¥Åä¸®¿¡ ¾ÆÀÌÅÛ ¼ÂÆÃ
	m_vpInventory[item.SlotInfo.ContainerType].at(item.SlotInfo.SlotIndex)->SetItemStatus(&item);
	m_vpInventory[item.SlotInfo.ContainerType].at(item.SlotInfo.SlotIndex)->SetItem(item.SlotInfo.ContainerType, item.SlotInfo.SlotIndex);	
	
	//¼³Á¤µÇ´Â ¾ÆÀÌÅÛÀÌ Æê ÀÚµ¿ ¸ÔÀÌ·Î µî·ÏµÇ¾î ÀÖ´Â °æ¿ì
	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		//--------------------------------------------------
		SPSelectItem &petFeed = m_vecPetFeeds[idx].feedNormal;
		if(petFeed.IsNull() == false && IsPetFeedItem(item.SlotInfo.ContainerType, item.SlotInfo.SlotIndex)) {
			SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PET);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_PET_FEED_UPDATE, (WPARAM)0, (LPARAM)0);
			}

			if(m_iCurrentInventoryType == petFeed.m_iContainer) {
				int iPetFeedIndex = petFeed.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
				g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iPetFeedIndex, (LPARAM)true, NULL);
			}
		}
		//--------------------------------------------------
		SPSelectItem &petFeedEnergy = m_vecPetFeeds[idx].feedEnergy;
		if(petFeedEnergy.IsNull() == false && IsPetFeedEnergyItem(item.SlotInfo.ContainerType, item.SlotInfo.SlotIndex)) {
			SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PET);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_PET_FEED_UPDATE, (WPARAM)0, (LPARAM)0);
			}

			if(m_iCurrentInventoryType == petFeedEnergy.m_iContainer) {
				int iPetFeedIndex = petFeedEnergy.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
				g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iPetFeedIndex, (LPARAM)true, NULL);
			}
		}
		//--------------------------------------------------
	}

	//ÀÎÅÍÆäÀÌ½º ¾ÆÀÌÄÜ ¼ÂÆÃ	
	if( item.SlotInfo.ContainerType != INVENTORY_CASH )
		SetInventoryIcon(&item, bSetEquip);

	return true;
}


bool SPPlayerInvenArchive::IsItemExist(int iIndex, CONTAINER_TYPE iType)
{
	if(IsEnableContainer(iType) == false)
		return false;

	if(iIndex < 0 || iIndex >= GetInventorySize(iType))
		return false;

	if(m_vpInventory[iType].at(iIndex)->GetItemAttr() == 0)
		return false;
	
	if(m_vpInventory[iType].at(iIndex)->GetItemStatusValue(MV_ITEM_GETUID) != 0)
		return true;
	
	return false;
}


//////////////////////////////////////////////////////////////////////////
// Equip
int	SPPlayerInvenArchive::EquipGridCursorMove(LPARAM lParam)
{
	m_bClickMove = (bool)lParam;
	//DXUTOutputDebugString("Click ÈÄ À§Ä¡ ÀÌµ¿...[%d]\n", m_bClickMove);	

	if(m_bPickup == false && m_bClickMove) {
		//¿©±â¼­ Àåºñ PickUp
		EquipPickUp(m_iEquipLClickIndex);
	}
	
	return 0;
}


int	SPPlayerInvenArchive::EquipPickUp(int iSlotIndex)
{
	m_bPickup = true;
	// µé¾î¿Ã¸°´Ù. (pick) 
	int iItemID;
	SPTexture* icon = NULL;	

	bool bItemExist = false;
	bool bFashionExist = false;
	bool bEquipExist = false;

	//DXUTOutputDebugString("*EquipPickUp Slot[%d]\n", iSlotIndex);

	//ÇØ´ç ÀÎº¥Åä¸® ¾È¿¡ ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÏÁö ¾Ê´Â °æ¿ì 
	if(IsItemExist(iSlotIndex, m_iCurrentFigureType) == false) {
		m_bPickup = false;
		return 0;
	}

	switch(m_iCurrentFigureType)
	{
	case FIGURE_EQUIP:
		{
			iItemID = m_vpInventory[FIGURE_EQUIP].at(iSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
			if(iItemID == 0) {
				m_bPickup = false;
				return 0;
			}
			m_PickUpItem.Clear();
			m_PickUpItem.SetSelectItem(FIGURE_EQUIP, iSlotIndex);
			break;
		}			
	case FIGURE_FASHION://EquipGridClick
		{
			iItemID = m_vpInventory[FIGURE_FASHION].at(iSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
			if(iItemID == 0) {
				m_bPickup = false;
				return 0;
			}				

			SPItem* pPickUpItem = m_vpInventory[FIGURE_FASHION].at(iSlotIndex);

			m_PickUpItem.Clear();
			m_PickUpItem.SetSelectItem(FIGURE_FASHION, iSlotIndex);
			break;
		}			
	case FIGURE_EFFECT://EquipGridClick
		{
			iItemID = m_vpInventory[FIGURE_EFFECT].at(iSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
			if(iItemID == 0) {
				m_bPickup = false;
				return 0;
			}				

			SPItem* pPickUpItem = m_vpInventory[FIGURE_EFFECT].at(iSlotIndex);

			m_PickUpItem.Clear();
			m_PickUpItem.SetSelectItem(FIGURE_EFFECT, iSlotIndex);
			break;
		}	
	}

	//[2005/12/29] - ÀÚµ¿À¸·Î ÀÎº¥ À©µµ¿ìÀÇ ÅÇÀ» Àåºñ·Î º¯È¯
	g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SETDEFAULT, (WPARAM)INVENTORY_EQUIP, (LPARAM)0);
	m_iCurrentInventoryType = INVENTORY_EQUIP;	
	m_iCurrentContainer = INVENTORY_EQUIP;
	SetItemScroll(m_iCurrentInventoryType);
	ReflashInventory();
	SetMouseIcon();		
	ClearOutSideControl();
	
	return 0;
}


int	SPPlayerInvenArchive::EquipGridClickDown(int iSlotIndex)
{	
	//¸¸ÀÏ ÀÌÀüÀÇ PickUp¾ÆÀÌÅÛ°ú °°Àº °æ¿ì
	if(m_TrashItem.IsNull() == false) {
		g_pInterfaceManager->ForceReply();
		RecoverIcon();
	}

	if(m_iNoticeUse == NOTICE_USE_ITEM_TRASH || m_iNoticeUse == NOTICE_USE_ITEM_PARTITION) {
		g_pInterfaceManager->ForceReply();
		RecoverIcon();
	}
	
	if(m_bPickup)				return 0;

	m_iEquipLClickIndex = iSlotIndex;	
	//DXUTOutputDebugString("EquipGridClick Slot[%d]\n", iSlotIndex);
	
	return 0;
}


int	SPPlayerInvenArchive::EquipGridClickUp(int iSlotIndex)
{
	if(m_bPickup)
	{
		if((((SPPlayer*)m_pParent)->IsWeaponChangeEnable()) == false){
			RecoverIcon();
			return 0;
		}

		if((((SPPlayer*)m_pParent)->IsDead())){
			RecoverIcon();
			return 0;
		}
		
		//// °°Àº °÷ ÀÌµ¿
		if(iSlotIndex == m_PickUpItem.m_iSlotIndex &&
			m_PickUpItem.m_iContainer == m_iCurrentFigureType)
		{			
			CONTAINER_ITEM _item;
			GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex)->ConvertContainerItem((CONTAINER_ITEM*)&_item);
			DeletePickupItem();
			ItemSetting( _item , true);			
			return 0;
		}

		//[2005/7/8] ÀåºñÂÊ¿¡¼­ Àåºñ·Î ÀÌµ¿ÇÒ¶§
		if(m_PickUpItem.m_iContainer == FIGURE_EQUIP || m_PickUpItem.m_iContainer == FIGURE_FASHION
			|| m_PickUpItem.m_iContainer == FIGURE_EFFECT) {
			RecoverIcon();
			return 0;
		}		

		// Equip ÀåºñÀÏ°æ¿ì¿¡´Â Pos¸¦ °­Á¦·Î ÇØ¼­ °¥¾ÆÀÔÈ÷´Â ½ÄÀ¸·Î ÇÑ´Ù.
		SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
		bool bItemAvailable = pPickUpItem->GetItemStatus()->IsTimeAvailable();
		if(bItemAvailable == false){
			RecoverIcon();
			return 0;
		}

		int iPickUpItemID = pPickUpItem->GetItemStatusValue(MV_ITEM_GETUID);			//Å×½ºÆ®¿ë
		//int iPickUpItemStatusID = pPickUpItem->GetItemStatusValue(MV_ITEM_GETSTATUSID);
		ITEM_TYPE iItemType = pPickUpItem->GetItemAttr()->m_eType;
		ITEM_SUBTYPE iSubType = pPickUpItem->GetItemAttr()->m_eSubType;
		int iEquipPos = pPickUpItem->GetItemAttr()->m_ePosID1;
		
		if(iItemType == ITEM_TYPE_FIGURE) {
			if(m_iCurrentFigureType == FIGURE_FASHION) {
				if(iSubType != ITEM_SUBTYPE_EQUIP_FASHION){
					RecoverIcon();
					return 0;
				}
			}
			else if(m_iCurrentFigureType == FIGURE_EQUIP){
				if(iSubType != ITEM_SUBTYPE_EQUIP_EQUIP){
					RecoverIcon();
					return 0;
				}
			}
			else if (m_iCurrentFigureType == FIGURE_EFFECT)
			{
				if (iSubType != ITEM_SUBTYPE_EQUIP_EFFECT)
				{
					RecoverIcon();
					return 0;
				}
			}
			
			int iSubType;
			iSubType = (int)pPickUpItem->GetItemAttr()->m_eSubType;

			if(iSubType < 1 || iSubType > 3)		return 0;		//À¯È¿ÇÑ ¼­ºêÅ¸ÀÔÀÎÁö¸¸ °Ë»ç

			CHANGE_SLOT item;
			int iSlotIndex;
			item.DestSlotInfo.ContainerType = m_iCurrentFigureType;

			iSlotIndex = (int)pPickUpItem->GetItemAttr()->m_ePosID1;

			// ¹«±â Ã³¸®
			if(iSlotIndex >= EQ_WEAPON1 && iSlotIndex <= EQ_BUCKLER2) {
				RecoverIcon();
				return 0;
			}

			// PlayerÀÇ »óÅÂ È®ÀÎ
			if(CheckPCStatus() == false) {
				RecoverIcon();
				return 0;
			}

			// ½½·Ô ±³È¯ ÆÖÅ¶ ¿äÃ»
			item.DestSlotInfo.SlotIndex = iSlotIndex - 1;		//ÆÖÅ¶ º¸³»´ÂÂÊ¿¡¼­ ´õÇØÁÖ±â ¶§¹®¿¡ ¿©±â¼­ ÇÑ¹ø »©Áà¾ß ÇÑ´Ù...
			item.SrcSlotInfo.ContainerType = m_PickUpItem.GetContainerType();
			item.SrcSlotInfo.SlotIndex = m_PickUpItem.GetSlotIndex();
			item.iCount = pPickUpItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
			SendReqChangeSlot( item );
		}
		else {
			RecoverIcon();
			return 0;
		}
	}
	else {
	}
	return 1;
}


int SPPlayerInvenArchive::EquipGridRClickUp(int iSlotIndex)
{	
	if(IsItemExist(iSlotIndex, m_iCurrentFigureType) == false)
		return 1;	

	SPItem* pItem = GetInventoryItem(m_iCurrentFigureType, iSlotIndex);
	if(pItem->GetItemAttr()->m_bCashCheck && pItem->GetItemAttr()->m_iShopID) {
		SPCashTradeManager::GetInstance()->SetSelectCashItem(pItem->GetItemAttr()->m_iShopID);
	}
	return 1;
}


int		SPPlayerInvenArchive::OnChangeEquip( LPARAM lParam )
{
	RecoverIcon();
	
	CONTAINER_TYPE iContainerType = (CONTAINER_TYPE)lParam;	
	
	m_iCurrentFigureType = iContainerType ;
	m_iCurrentContainer = m_iCurrentFigureType;
	g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_ALLCLEAR, 0,0, NULL);

	std::vector<SPItem*>::iterator iter;
	for(iter = m_vpInventory[iContainerType].begin(); iter != m_vpInventory[iContainerType].end(); ++iter)
	{
		CONTAINER_ITEM item;
		item.Item.iItemID = (*iter)->GetItemStatusValue(MV_ITEM_GETUID);
		if(item.Item.iItemID==0)
			continue;		
		
		(*iter)->ConvertContainerItem((CONTAINER_ITEM*)&item);
		ItemSetting( item, false );
	}

	SetDisableIcon();

	return 1;
}


//////////////////////////////////////////////////////////////////////////
//// Weapon1
int	SPPlayerInvenArchive::Weapon1CursorMove(LPARAM lParam)
{
	m_bClickMove = (bool)lParam;
	//DXUTOutputDebugString("Click ÈÄ À§Ä¡ ÀÌµ¿...[%d]\n", m_bClickMove);	

	if(m_bPickup == false && m_bClickMove) {		
		Weapon1PickUp(m_iEquipLClickIndex);				//¿©±â¼­ Àåºñ PickUp
	}

	return 0;
}


int	SPPlayerInvenArchive::Weapon1PickUp(int iSlotIndex)
{
	if(m_bPickup)		return 0;
	
	//DXUTOutputDebugString("Weapon1PickUp Slot[%d]\n", iSlotIndex);	
	
	m_bPickup = true;
	int nUID = 0;
	SPTexture* icon = NULL;

	if(!IsItemExist(iSlotIndex, FIGURE_EQUIP)){
		m_bPickup = false;
		return 0;
	}

	SPItem* pClickItem;
	pClickItem = GetInventoryItem(FIGURE_EQUIP, iSlotIndex);
	

	m_PickUpItem.Clear();
	m_PickUpItem.SetSelectItem(FIGURE_EQUIP, iSlotIndex);	
	
	//[2005/12/29] - ÀÚµ¿À¸·Î ÀÎº¥ À©µµ¿ìÀÇ ÅÇÀ» Àåºñ·Î º¯È¯
	g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SETDEFAULT, (WPARAM)INVENTORY_EQUIP, (LPARAM)0);
	m_iCurrentInventoryType = INVENTORY_EQUIP;	
	m_iCurrentContainer = INVENTORY_EQUIP;
	SetItemScroll(m_iCurrentInventoryType);
	ReflashInventory();	
	SetMouseIcon();		
	ClearOutSideControl();
	
	return 0;
}


int	SPPlayerInvenArchive::Weapon1ClickDown(int iSlotIndex)
{	
	//¸¸ÀÏ ÀÌÀüÀÇ PickUp¾ÆÀÌÅÛ°ú °°Àº °æ¿ì
	if(m_TrashItem.IsNull() == false) {
		g_pInterfaceManager->ForceReply();
		RecoverIcon();
	}

	if(m_iNoticeUse == NOTICE_USE_ITEM_TRASH || m_iNoticeUse == NOTICE_USE_ITEM_PARTITION) {
		g_pInterfaceManager->ForceReply();
		RecoverIcon();
	}
	
	if(m_bPickup)				return 0;
	
	int iRealSlot = (EQ_WEAPON1 - 1) + iSlotIndex;
	m_iEquipLClickIndex = iRealSlot;
	//DXUTOutputDebugString("Weapon1ClickDown Slot[%d]\n", iRealSlot);

	return 0;	
}


int	SPPlayerInvenArchive::Weapon1ClickUp(int iSlotIndex)
{
	int iRealSlot = (EQ_WEAPON1 - 1) + iSlotIndex;

	if(m_bPickup)
	{
		if((((SPPlayer*)m_pParent)->IsWeaponChangeEnable()) == false){	
			RecoverIcon();
			return 0;
		}

		if((((SPPlayer*)m_pParent)->IsDead())){
			RecoverIcon();
			return 0;
		}

		int iEquipSlotIndex;
		
		if(iRealSlot == m_PickUpItem.m_iSlotIndex && m_PickUpItem.m_iContainer > INVENTORY_HOUSING
			/*(m_PickUpItem.m_iContainer == FIGURE_EQUIP || m_PickUpItem.m_iContainer == FIGURE_FASHION)*/)
		{			
			CONTAINER_ITEM _item;
			GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex)->ConvertContainerItem((CONTAINER_ITEM*)&_item);
			DeletePickupItem();
			ItemSetting( _item , true);			
			return 0;
		}

		//[2005/7/8] ÀåºñÂÊ¿¡¼­ Àåºñ·Î ÀÌµ¿ÇÒ¶§
		if(m_PickUpItem.m_iContainer == FIGURE_EQUIP || m_PickUpItem.m_iContainer == FIGURE_FASHION
			|| m_PickUpItem.m_iContainer == FIGURE_EFFECT) {
			RecoverIcon();
			return 0;
		}

		SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
		iEquipSlotIndex = pPickUpItem->GetItemAttr()->m_ePosID1;
		int iSubType = pPickUpItem->GetItemAttr()->m_eSubType;
		
		if(iSubType != ITEM_SUBTYPE_EQUIP_EQUIP){
			RecoverIcon();
			return 0;
		}

		if(iRealSlot != iEquipSlotIndex -1){
			RecoverIcon();
			return 0;
		}

		if(iEquipSlotIndex == EQ_WEAPON1 /*- 1*/ || iEquipSlotIndex == EQ_BUCKLER1 /*- 1*/)
		{	
			CHANGE_SLOT item;
			item.DestSlotInfo.ContainerType = FIGURE_EQUIP;
			item.DestSlotInfo.SlotIndex = iEquipSlotIndex - 1;
			item.SrcSlotInfo.ContainerType = m_PickUpItem.m_iContainer;
			item.SrcSlotInfo.SlotIndex = m_PickUpItem.m_iSlotIndex;
			item.iCount = pPickUpItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);			
			
			// PlayerÀÇ »óÅÂ È®ÀÎ
			if(CheckPCStatus() == false) {
				RecoverIcon();
				return 0;
			}

			SendReqChangeSlot( item );
		}
	}
	else {
	}
}


int	SPPlayerInvenArchive::Weapon1RClickUp(int iSlotIndex)
{
	int iRealSlot = (EQ_WEAPON1 - 1) + iSlotIndex;
	
	if(IsItemExist(iRealSlot, FIGURE_EQUIP) == false)
		return 1;
	
	SPItem* pItem = GetInventoryItem(FIGURE_EQUIP, iRealSlot);
	if(pItem->GetItemAttr()->m_bCashCheck && pItem->GetItemAttr()->m_iShopID) {
		SPCashTradeManager::GetInstance()->SetSelectCashItem(pItem->GetItemAttr()->m_iShopID);
	}
	return 1;
}


//////////////////////////////////////////////////////////////////////////
// Weapon2 
int	SPPlayerInvenArchive::Weapon2CursorMove(LPARAM lParam)
{
	m_bClickMove = (bool)lParam;
	//DXUTOutputDebugString("Click ÈÄ À§Ä¡ ÀÌµ¿...[%d]\n", m_bClickMove);	

	if(m_bPickup == false && m_bClickMove) {		
		Weapon2PickUp(m_iEquipLClickIndex);				//¿©±â¼­ Àåºñ PickUp
	}

	return 0;
}


int	SPPlayerInvenArchive::Weapon2PickUp(int iSlotIndex)
{
	if(m_bPickup)			return 0;
	
	m_bPickup = true;
	int nUID = 0;
	SPTexture* icon = NULL;		
	
	if(!IsItemExist(iSlotIndex, FIGURE_EQUIP)){
		m_bPickup = false;
		return 0;
	}
	
	SPItem* pClickItem;
	pClickItem = GetInventoryItem(FIGURE_EQUIP, iSlotIndex);	

	m_PickUpItem.Clear();
	m_PickUpItem.SetSelectItem(FIGURE_EQUIP, iSlotIndex);	

	//[2005/12/29] - ÀÚµ¿À¸·Î ÀÎº¥ À©µµ¿ìÀÇ ÅÇÀ» Àåºñ·Î º¯È¯
	g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SETDEFAULT, (WPARAM)INVENTORY_EQUIP, (LPARAM)0);
	m_iCurrentInventoryType = INVENTORY_EQUIP;	
	m_iCurrentContainer = INVENTORY_EQUIP;
	SetItemScroll(m_iCurrentInventoryType);
	ReflashInventory();
	SetMouseIcon();		
	ClearOutSideControl();

	return 0;
}


int	SPPlayerInvenArchive::Weapon2ClickDown(int iSlotIndex)
{
	//¸¸ÀÏ ÀÌÀüÀÇ PickUp¾ÆÀÌÅÛ°ú °°Àº °æ¿ì
	if(m_TrashItem.IsNull() == false) {
		g_pInterfaceManager->ForceReply();
		RecoverIcon();
	}

	if(m_iNoticeUse == NOTICE_USE_ITEM_TRASH || m_iNoticeUse == NOTICE_USE_ITEM_PARTITION) {
		g_pInterfaceManager->ForceReply();
		RecoverIcon();
	}
	
	if(m_bPickup)				return 0;

	int iRealSlot = (EQ_WEAPON2 - 1) + iSlotIndex;
	m_iEquipLClickIndex = iRealSlot;	
	//DXUTOutputDebugString("Weapon2ClickDown Slot[%d]\n", iRealSlot);

	return 0;
}


int	SPPlayerInvenArchive::Weapon2ClickUp(int iSlotIndex)
{
	int iRealSlot = (EQ_WEAPON2 - 1) + iSlotIndex;

	if(m_bPickup)
	{
		if((((SPPlayer*)m_pParent)->IsWeaponChangeEnable()) == false){	
			RecoverIcon();
			return 0;
		}

		if((((SPPlayer*)m_pParent)->IsDead())){
			RecoverIcon();
			return 0;
		}
		
		int iEquipSlotIndex;
		
		if(iRealSlot == m_PickUpItem.m_iSlotIndex && m_PickUpItem.m_iContainer > INVENTORY_HOUSING
			/*(m_PickUpItem.m_iContainer == FIGURE_EQUIP || m_PickUpItem.m_iContainer == FIGURE_FASHION)*/)
		{			
			CONTAINER_ITEM _item;
			GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex)->ConvertContainerItem((CONTAINER_ITEM*)&_item);
			DeletePickupItem();
			ItemSetting( _item , true);			
			return 0;
		}

		//[2005/7/8] ÀåºñÂÊ¿¡¼­ Àåºñ·Î ÀÌµ¿ÇÒ¶§
		if(m_PickUpItem.m_iContainer == FIGURE_EQUIP || m_PickUpItem.m_iContainer == FIGURE_FASHION
			|| m_PickUpItem.m_iContainer == FIGURE_EFFECT) {
			RecoverIcon();
			return 0;
		}

		SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);		
		iEquipSlotIndex = pPickUpItem->GetItemAttr()->m_ePosID2;
		int iSubType = pPickUpItem->GetItemAttr()->m_eSubType;
		
		if(iSubType != ITEM_SUBTYPE_EQUIP_EQUIP){
			RecoverIcon();
			return 0;
		}

		if(iRealSlot != iEquipSlotIndex -1){
			RecoverIcon();
			return 0;
		}

		if(iEquipSlotIndex == EQ_WEAPON2 || iEquipSlotIndex == EQ_BUCKLER2)
		{
			CHANGE_SLOT item;			
			item.DestSlotInfo.ContainerType = FIGURE_EQUIP;
			item.DestSlotInfo.SlotIndex = iEquipSlotIndex - 1;
			item.SrcSlotInfo.ContainerType = m_PickUpItem.m_iContainer;
			item.SrcSlotInfo.SlotIndex = m_PickUpItem.m_iSlotIndex;
			item.iCount = 1;			

			// PlayerÀÇ »óÅÂ È®ÀÎ
			if(CheckPCStatus() == false) {
				RecoverIcon();
				return 0;
			}

			SendReqChangeSlot( item );			
		}
	}
	else {

	}
}


int	SPPlayerInvenArchive::Weapon2RClickUp(int iSlotIndex)
{
	int iRealSlot = (EQ_WEAPON2 - 1) + iSlotIndex;

	if(IsItemExist(iRealSlot, FIGURE_EQUIP) == false)
		return 1;

	SPItem* pItem = GetInventoryItem(FIGURE_EQUIP, iRealSlot);
	if(pItem->GetItemAttr()->m_bCashCheck && pItem->GetItemAttr()->m_iShopID) {
		SPCashTradeManager::GetInstance()->SetSelectCashItem(pItem->GetItemAttr()->m_iShopID);
	}
	return 1;
}


int SPPlayerInvenArchive::StoragePageUp()
{
	m_iCurStoragePage--;
	if(m_iCurStoragePage < 0)
		m_iCurStoragePage = 0;
	

	ReflashStorage();
	return 1;
}


int SPPlayerInvenArchive::StoragePageDown()
{
	m_iCurStoragePage++;
	if(m_iCurStoragePage > m_iStoragePage)
		m_iCurStoragePage = m_iStoragePage;
	
	ReflashStorage();
	return 1;
}


int SPPlayerInvenArchive::StorageSetPage(int iPage)
{
	if(iPage < 0)
		iPage = 0;

	if(iPage > m_iStoragePage)
		iPage = m_iStoragePage;

	m_iCurStoragePage = iPage;
	
	ReflashStorage();
	return 1;
}


int SPPlayerInvenArchive::ReflashStorage()
{
	g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_ALLCLEAR, 0, 0, NULL);	
	
	int iStartIndex = m_iCurStoragePage * STORAGE_LINE;
	
	for(int iCount = 0; iCount < VIEW_STORAGE_SLOT; iCount++) {
		CONTAINER_ITEM item;
		
		if( (iStartIndex + iCount) >= m_vpInventory[STORAGE].size()){
			break;
		}
		
		if(IsItemExist(iStartIndex + iCount, STORAGE)) {
			m_vpInventory[STORAGE].at(iStartIndex + iCount)->ConvertContainerItem((CONTAINER_ITEM*)&item);
			ItemSetting( item );
		}		
	}
	
	SetDisableIcon();

	return 1;
}


int	SPPlayerInvenArchive::StorageCursorMove(LPARAM lParam)
{
	m_bClickMove = (bool)lParam;
	//DXUTOutputDebugString("Click ÈÄ À§Ä¡ ÀÌµ¿...[%d]\n", m_bClickMove);	

	if(m_bPickup == false && m_bClickMove) {		
		StoragePickUp(m_iStorageLClickIndex);				//¿©±â¼­ Àåºñ PickUp
	}

	return 0;
}


int	SPPlayerInvenArchive::StoragePickUp(int iSlotIndex)
{
	if(m_bPickup)				return 0;

	//DXUTOutputDebugString("*StoragePickUp Slot[%d]\n", iSlotIndex);

	m_bPickup = true;
	int iItemID = 0;
	int iSrcSlotIndex = 0;		

	SPTexture* icon = NULL;

	if(IsItemExist(iSlotIndex, STORAGE) == false) {
		m_bPickup = false;
		return 0;
	}			
	
	iItemID = m_vpInventory[STORAGE].at(iSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);				
	if(iItemID == 0) {
		m_bPickup = false;
		return 0;
	}

	m_PickUpItem.SetSelectItem(STORAGE, iSlotIndex);
	SetMouseIcon();

	int iDisabelIndex = m_PickUpItem.m_iSlotIndex - (m_iCurStoragePage * STORAGE_LINE);	
	g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisabelIndex, (LPARAM)true, NULL);

	SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
	
	CONTAINER_TYPE iContainerType = (CONTAINER_TYPE)(pPickUpItem->GetItemAttr()->m_eType - 1);

	//[2005/12/29] - ÀÚµ¿À¸·Î ÀÎº¥ À©µµ¿ìÀÇ ÅÇÀ» Àåºñ·Î º¯È¯
	g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SETDEFAULT, (WPARAM)iContainerType, (LPARAM)0);
	m_iCurrentInventoryType = iContainerType;	
	m_iCurrentContainer = iContainerType;
	SetItemScroll(m_iCurrentInventoryType);
	ReflashInventory();	
	ClearOutSideControl();
	
	return 0;
}


int SPPlayerInvenArchive::StorageClickDown(int iSlotIndex)
{
	if(m_TrashItem.IsNull() == false) {
		g_pInterfaceManager->ForceReply();
		RecoverIcon();
	}

	if(m_iNoticeUse == NOTICE_USE_ITEM_TRASH || m_iNoticeUse == NOTICE_USE_ITEM_PARTITION) {
		g_pInterfaceManager->ForceReply();
		RecoverIcon();
	}
	
	if(m_bPickup)				return 0;
	
	iSlotIndex = iSlotIndex + (m_iCurStoragePage * STORAGE_LINE);

	m_iStorageLClickIndex = iSlotIndex;	
	//DXUTOutputDebugString("StorageClickDown Slot[%d]\n", iSlotIndex);

	return 0;
}


int	SPPlayerInvenArchive::StorageClickUp(int iSlotIndex)
{
  	iSlotIndex = iSlotIndex + (m_iCurStoragePage * STORAGE_LINE);
	
	if(m_bPickup)
	{
		if(iSlotIndex >= GetInventorySize(STORAGE)) {
			RecoverIcon();
			return 0;
		}
		
		CHANGE_SLOT item;
		item.DestSlotInfo.ContainerType = (CONTAINER_TYPE)STORAGE;
		item.DestSlotInfo.SlotIndex = iSlotIndex;
		item.SrcSlotInfo.ContainerType = m_PickUpItem.m_iContainer;
		item.SrcSlotInfo.SlotIndex = m_PickUpItem.m_iSlotIndex;		

		int iSlotPosID;
		iSlotPosID = m_PickUpItem.m_iSlotIndex;		

		// °°Àº °÷ ÀÌµ¿ÀÌ¶ó¸é ±×´ë·Î~
		if(item.DestSlotInfo.SlotIndex == item.SrcSlotInfo.SlotIndex && 
			item.DestSlotInfo.ContainerType == item.SrcSlotInfo.ContainerType)
		{			
			CONTAINER_ITEM _item;			
			SPItem* pItem = GetInventoryItem(item.DestSlotInfo.ContainerType, item.DestSlotInfo.SlotIndex);
			pItem->ConvertContainerItem((CONTAINER_ITEM*)&_item);
			DeletePickupItem();
			ItemSetting( _item );			
			return 0;
		}

		if(item.SrcSlotInfo.ContainerType == FIGURE_EQUIP || item.SrcSlotInfo.ContainerType == FIGURE_FASHION
			|| item.SrcSlotInfo.ContainerType == FIGURE_EFFECT) {			
			int iItemID = m_vpInventory[STORAGE].at(item.DestSlotInfo.SlotIndex)->GetItemStatus()->SPSendMessage(MV_ITEM_GETUID);			

			SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
			int iSubType = pPickUpItem->GetItemAttr()->m_eSubType;
			int iEquipPos = pPickUpItem->GetItemAttr()->m_ePosID1;								

			//Inventory Equip Dest¿¡ Àåºñ°ü·Ã ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÏ´Â °æ¿ì ¿¹¿Ü Ã³¸®
			if(IsItemExist(iSlotIndex, STORAGE)) {
				SPItem* pDestItem = GetInventoryItem(STORAGE, iSlotIndex);
				int iDestSubType = pDestItem->GetItemAttr()->m_eSubType;
				int iDestEquipPos =  pDestItem->GetItemAttr()->m_ePosID1;					

				//¸ÞÀÎ½½·Ô¸¸ Ã¼Å©ÇÏ±â ¶§¹®¿¡ ¹®Á¦ÀÇ ¼ÒÁö°¡ ÀÖÀ½...
				if(m_PickUpItem.m_iContainer == FIGURE_EQUIP || m_PickUpItem.m_iContainer == FIGURE_FASHION
					|| m_PickUpItem.m_iContainer == FIGURE_EFFECT) {
					if(iDestSubType != iSubType || iDestEquipPos != iEquipPos ) {
						RecoverIcon();
						return 0;
					}
				}					

				//[2005/7/13]
				//Dest¿¡ Àåºñ ¾ÆÀÌÅÛÀÌ Á¸ÀçÇÏ´Â °æ¿ì ½ÇÁ¦ Src¿Í Dest¸¦ ¹Ù²ã¼­ Àü¼ÛÇÏµµ·Ï ÇÑ´Ù..
				//´Ü¹æÇâ¼ºÀ» °¡Áö±â À§ÇØ¼­ ¼Õ´ë¸®´ÔÀÌ °­Á¦·Î ½ÃÅ´
				item.SrcSlotInfo.ContainerType = (CONTAINER_TYPE)STORAGE;
				item.SrcSlotInfo.SlotIndex = iSlotIndex;		
				item.DestSlotInfo.ContainerType = m_PickUpItem.m_iContainer;
				item.DestSlotInfo.SlotIndex = m_PickUpItem.m_iSlotIndex;
			}

			ITEMSTATUS_ID iItemstatusID = (ITEMSTATUS_ID)pPickUpItem->GetItemStatusValue(MV_ITEM_GETSTATUSID);

			if(iItemID != 0 && (iItemstatusID == IS_EQUIP || iItemstatusID >= IS_EQUIP_BATTLE))
			{
				SPItemAttr* pItemAttr;
				g_pItemCluster->GetItemInfo(iItemID, pItemAttr);
				int iDestEquipPosID = pItemAttr->m_ePosID1; 
				int iEquipPosID = m_PickUpItem.m_iSlotIndex + 1;

				// ÇÈ¾÷ÇÑ ¾ÆÀÌÅÛÀÌ ÀåºñÀÌ¸é¼­ ¹«±âÀÎ°æ¿ì ¿¹¿Ü Ã³¸®
				if(m_PickUpItem.m_iContainer > INVENTORY_HOUSING) {
					if(iEquipPosID >= EQ_WEAPON1) {
						if(iEquipPosID >= EQ_WEAPON2) {					//2¹ø ¹«±â½½·ÔÀÎ°æ¿ì 
							iEquipPosID = iEquipPosID - 2;
						}

						if(iEquipPosID != iDestEquipPosID){
							RecoverIcon();
							return 0;
						}
					}
				}
			}			
		}
		
		ClearOutSideControl();

		SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);

		//Ã¢°í¿¡ ÀúÀå°¡´ÉÇÑ ¾ÆÀÌÅÛÀÎÁö È®ÀÎ
		if(pPickUpItem->GetItemAttr()->m_bStorage == false) {
			RecoverIcon();
			return 0;
		}

		//[2007/6/27]- ÇýÁø¾¾ ¿äÃ» Á¦ÇÑ		½Ã°£ Á¦ÇÑ ¾ÆÀÌÅÛÀÎ °æ¿ì
		//if(pPickUpItem->GetItemAttr()->GetDurationType() > ITEM_DURATION_NULL) {
		//	RecoverIcon();
		//	return 0;
		//}

		//[2005/7/25] ¾ÆÀÌÅÛ ³ª´©±â Å×½ºÆ® ÄÚµåÀÓ!
		if(item.DestSlotInfo.ContainerType == item.SrcSlotInfo.ContainerType) {
			if(IsItemExist(item.DestSlotInfo.SlotIndex, item.DestSlotInfo.ContainerType) == false) {
				SPItem* pPickUpItem = GetInventoryItem(item.SrcSlotInfo.ContainerType, item.SrcSlotInfo.SlotIndex);
				int iPickUpItemStack = pPickUpItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
				if(iPickUpItemStack > 1) {
					g_pInterfaceManager->GetNoticeBox()->SPSendMessage(SPIM_SET_MAXVALUE, (WPARAM)iPickUpItemStack, 0);
					g_pInterfaceManager->GetNoticeBox()->SPSendMessage(SPIM_SET_MINVALUE, (WPARAM)1, 0);
					
					char szMsg[512];
					ZeroMemory(szMsg, 512);		
					if(g_pResourceManager->GetGlobalString(4003004)) {
						std::string strMsg = g_pResourceManager->GetGlobalString(4003004);
						std::string strItemName = pPickUpItem->GetItemAttr()->m_strName;
						wsprintf(szMsg, strMsg.c_str(), strItemName.c_str());
					}
					g_pInterfaceManager->SetNumNotice(szMsg, this->m_pParent);
					m_iNoticeUse = NOTICE_USE_ITEM_PARTITION;
					m_ChangeSlot = item;
					RecoverIcon();
					return 0;
				}
			}
		}		
		else {
			//[2005/10/11] - ÇöÀç Dest¿¡ ¾ÆÀÌÅÛÀÌ ÀÖ°í ±× ¾ÆÀÌÅÛÀÇ ItemTypeÀÌ ¿Ã¸®·Á´Â ¾ÆÀÌÅÛ°ú Æ²¸°°æ¿ì
			if(IsItemExist(item.DestSlotInfo.SlotIndex, item.DestSlotInfo.ContainerType)) {
				SPItem* pDestItem = GetInventoryItem(item.DestSlotInfo.ContainerType, item.DestSlotInfo.SlotIndex);
				SPItem* pPickUpItem = GetInventoryItem(item.SrcSlotInfo.ContainerType, item.SrcSlotInfo.SlotIndex);

				if(pDestItem->GetItemAttr()->m_eType != pPickUpItem->GetItemAttr()->m_eType){
					RecoverIcon();
					return 0;
				}
			}
		}
		

		item.iCount = pPickUpItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
		SendReqChangeSlot( item );
	}
	return 0;
}


int SPPlayerInvenArchive::PetClickDown(int iSlotIndex)
{
	if(m_TrashItem.IsNull() == false) {
		g_pInterfaceManager->ForceReply();
		RecoverIcon();
	}

	if(m_iNoticeUse == NOTICE_USE_ITEM_TRASH || m_iNoticeUse == NOTICE_USE_ITEM_PARTITION) {
		g_pInterfaceManager->ForceReply();
		RecoverIcon();
	}

	if(m_bPickup)				return 0;

	m_iPetLClickIndex = iSlotIndex;
	//DXUTOutputDebugString("PetClickDown Slot[%d]\n", iSlotIndex);
	
	return 0;
}


int SPPlayerInvenArchive::PetClickUp(int iSlotIndex)
{
	if(m_bPickup)
	{
		if(iSlotIndex >= GetInventorySize(FIGURE_PET)) {
			RecoverIcon();
			return 0;
		}

		// PlayerÀÇ »óÅÂ È®ÀÎ
		if((((SPPlayer*)m_pParent)->IsDead())){
			RecoverIcon();
			return 0;
		}		
		
		if(CheckPCStatus() == false) {
			RecoverIcon();
			return 0;
		}

		CHANGE_SLOT item;
		item.DestSlotInfo.ContainerType = (CONTAINER_TYPE)FIGURE_PET;
		item.DestSlotInfo.SlotIndex = iSlotIndex;
		item.SrcSlotInfo.ContainerType = m_PickUpItem.m_iContainer;
		item.SrcSlotInfo.SlotIndex = m_PickUpItem.m_iSlotIndex;		

		int iSlotPosID;
		iSlotPosID = m_PickUpItem.m_iSlotIndex;		

		// °°Àº °÷ ÀÌµ¿ÀÌ¶ó¸é ±×´ë·Î~
		if(item.DestSlotInfo.SlotIndex == item.SrcSlotInfo.SlotIndex && 
			item.DestSlotInfo.ContainerType == item.SrcSlotInfo.ContainerType)
		{			
			CONTAINER_ITEM _item;			
			SPItem* pItem = GetInventoryItem(item.DestSlotInfo.ContainerType, item.DestSlotInfo.SlotIndex);
			pItem->ConvertContainerItem((CONTAINER_ITEM*)&_item);
			DeletePickupItem();
			ItemSetting( _item );			
			return 0;
		}

		if(item.SrcSlotInfo.ContainerType != INVENTORY_PET) {
			RecoverIcon();
			return 0;
		}
	
		ClearOutSideControl();

		SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);		
		bool bItemAvailable = pPickUpItem->GetItemStatus()->IsTimeAvailable();
		if(bItemAvailable == false) {
			RecoverIcon();
			return 0;
		}

		if(pPickUpItem->GetItemAttr()->m_ePosID1 != 1) {
			RecoverIcon();
			return 0;
		}		

		item.iCount = pPickUpItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
		SendReqChangeSlot( item );
	}
	return 0;
}


int SPPlayerInvenArchive::PetCursorMove(LPARAM lParam)
{
	m_bClickMove = (bool)lParam;
	//DXUTOutputDebugString("Click ÈÄ À§Ä¡ ÀÌµ¿...[%d]\n", m_bClickMove);	

	if(m_bPickup == false && m_bClickMove) {		
		PetPickUp(m_iPetLClickIndex);				//¿©±â¼­ Àåºñ PickUp
	}

	return 0;
}


int	SPPlayerInvenArchive::PetPickUp(int iSlotIndex)
{
	if(m_bPickup)				return 0;

	//DXUTOutputDebugString("*StoragePickUp Slot[%d]\n", iSlotIndex);

	m_bPickup = true;
	int iItemID = 0;
	SPTexture* icon = NULL;

	if(IsItemExist(iSlotIndex, FIGURE_PET) == false) {
		m_bPickup = false;
		return 0;
	}		

	iItemID = m_vpInventory[FIGURE_PET].at(iSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);				
	if(iItemID == 0) {
		m_bPickup = false;
		return 0;
	}

	m_PickUpItem.SetSelectItem(FIGURE_PET, iSlotIndex);		
	SetMouseIcon();
	
	g_pInterfaceManager->GetPetWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)m_PickUpItem.m_iSlotIndex, (LPARAM)true, NULL);
	SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);

	CONTAINER_TYPE iContainerType = (CONTAINER_TYPE)(pPickUpItem->GetItemAttr()->m_eType - 1);

	//[2005/12/29] - ÀÚµ¿À¸·Î ÀÎº¥ À©µµ¿ìÀÇ ÅÇÀ» Àåºñ·Î º¯È¯
	g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SETDEFAULT, (WPARAM)iContainerType, (LPARAM)0);
	m_iCurrentInventoryType = iContainerType;	
	m_iCurrentContainer = iContainerType;
	SetItemScroll(m_iCurrentInventoryType);
	ReflashInventory();
	ClearOutSideControl();
	return 0;
}


void SPPlayerInvenArchive::WeaponChange()
{	
	WeaponChangeInterface();
}


void	SPPlayerInvenArchive::WeaponToggle()
{	
	if(m_iCurrentWeaponSlot == 1) {
		m_iCurrentWeaponSlot = 2;
	} else if(m_iCurrentWeaponSlot == 2) {
		m_iCurrentWeaponSlot = 1;
	} else {
		assert(0);
	}
	
	WeaponChangeInterface();
}


void SPPlayerInvenArchive::WeaponChangeInterface()
{
	g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_ACTIVE_SLOT, m_iCurrentWeaponSlot, 0, NULL);

	SPItem* pItem = NULL;
	bool bItemAvailable = false;				//ItemÀÇ ½Ã°£Á¦°¡ À¯È¿ÇÑÁö È®ÀÎÇÑ´Ù 
	CONTAINER_ITEM item;
	item.SlotInfo.ContainerType = FIGURE_EQUIP;
	item.Item.iStackCount = 1;
	switch(m_iCurrentWeaponSlot) {
	case 1:
		{
			// 1¹ø ¿þÆù ½½·Ô
			pItem = GetInventoryItem(FIGURE_EQUIP, EQ_WEAPON1-1);			
			if(pItem == NULL)	
				return;
			
			bItemAvailable = pItem->GetItemStatus()->IsTimeAvailable();
			item.Item.iItemID = pItem->GetItemStatusValue(MV_ITEM_GETUID);
			item.SlotInfo.SlotIndex = EQ_WEAPON1;			
			if(item.Item.iItemID != 0 && bItemAvailable) {
				m_pParent->SPGOBPerformMessage( SPGM_EQUIP_CHANGE, (LPARAM)&item);
				
				item.SlotInfo.SlotIndex--;	
				m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_INSERT, (LPARAM)&item);

				SPItem* pItem = GetInventoryItem(FIGURE_EQUIP, EQ_WEAPON2 - 1);
				if(pItem && !pItem->IsEmptyItem()) {
					CONTAINER_ITEM TakeOffItem;
					pItem->ConvertContainerItem(&TakeOffItem);
					m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&TakeOffItem);
				}
			}
			else {
				m_pParent->SPGOBPerformMessage( SPGM_EQUIP_UNLOAD, (LPARAM)EQ_WEAPON1);

				SPItem* pItem = GetInventoryItem(FIGURE_EQUIP, EQ_WEAPON2 - 1);
				if(pItem && !pItem->IsEmptyItem()) {
					CONTAINER_ITEM TakeOffItem;
					pItem->ConvertContainerItem(&TakeOffItem);
					m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&TakeOffItem);
				}
			}

			// 1¹ø ¹öÅ¬·¯ ½½·Ô
			pItem = GetInventoryItem(FIGURE_EQUIP, EQ_BUCKLER1-1);
			bItemAvailable = pItem->GetItemStatus()->IsTimeAvailable();
			item.Item.iItemID = pItem->GetItemStatusValue(MV_ITEM_GETUID);
			item.SlotInfo.SlotIndex = EQ_BUCKLER1;
			if(item.Item.iItemID != 0 && bItemAvailable) {
				m_pParent->SPGOBPerformMessage( SPGM_EQUIP_CHANGE, (LPARAM)&item);
				
				item.SlotInfo.SlotIndex--;
				m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_INSERT, (LPARAM)&item);

				SPItem* pItem = GetInventoryItem(FIGURE_EQUIP, EQ_BUCKLER2 - 1);
				if(pItem && !pItem->IsEmptyItem()) {
					CONTAINER_ITEM TakeOffItem;
					pItem->ConvertContainerItem(&TakeOffItem);
					m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&TakeOffItem);
				}
			}
			else {
				m_pParent->SPGOBPerformMessage( SPGM_EQUIP_UNLOAD, (LPARAM)EQ_BUCKLER1);

				SPItem* pItem = GetInventoryItem(FIGURE_EQUIP, EQ_BUCKLER2 - 1);
				if(pItem && !pItem->IsEmptyItem()) {
					CONTAINER_ITEM TakeOffItem;
					pItem->ConvertContainerItem(&TakeOffItem);
					m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&TakeOffItem);
				}
			}
		}
		break;

	case 2:
		{
			// 2¹ø ¿þÆù ½½·Ô
			pItem = GetInventoryItem(FIGURE_EQUIP, EQ_WEAPON2-1);			
			if(pItem == NULL)
				return;
			
			bItemAvailable = pItem->GetItemStatus()->IsTimeAvailable();
			item.Item.iItemID = pItem->GetItemStatusValue(MV_ITEM_GETUID);
			item.SlotInfo.SlotIndex = EQ_WEAPON1;
			if(item.Item.iItemID != 0 && bItemAvailable) {
				m_pParent->SPGOBPerformMessage( SPGM_EQUIP_CHANGE, (LPARAM)&item);
				
				item.SlotInfo.SlotIndex = EQ_WEAPON2 - 1;	
				m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_INSERT, (LPARAM)&item);

				SPItem* pItem = GetInventoryItem(FIGURE_EQUIP, EQ_WEAPON1- 1);
				if(pItem && !pItem->IsEmptyItem()) {
					CONTAINER_ITEM TakeOffItem;
					pItem->ConvertContainerItem(&TakeOffItem);
					m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&TakeOffItem);
				}
			}
			else {	//ÀÎµ¦½º·Î´Â ¾ð·Îµå°¡ ¾ÈµÈ´Ù ¾Æ¹ÙÅ¸´Â ¹«±â½½·ÔÀÌ ÇÑ°³ÀÎ°Å·Î ÀÎ½ÄÇÏ´Âµí...
				m_pParent->SPGOBPerformMessage( SPGM_EQUIP_UNLOAD, (LPARAM)EQ_WEAPON1);
				
				SPItem* pItem = GetInventoryItem(FIGURE_EQUIP, EQ_WEAPON1- 1);
				if(pItem && !pItem->IsEmptyItem()) {
					CONTAINER_ITEM TakeOffItem;
					pItem->ConvertContainerItem(&TakeOffItem);
					m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&TakeOffItem);
				}
			}

			// 2¹ø ¹öÅ¬·¯ ½½·Ô
			pItem = GetInventoryItem(FIGURE_EQUIP, EQ_BUCKLER2-1);
			bItemAvailable = pItem->GetItemStatus()->IsTimeAvailable();
			item.Item.iItemID = pItem->GetItemStatusValue(MV_ITEM_GETUID);
			item.SlotInfo.SlotIndex = EQ_BUCKLER1;
			if(item.Item.iItemID != 0 && bItemAvailable) {
				m_pParent->SPGOBPerformMessage( SPGM_EQUIP_CHANGE, (LPARAM)&item);
				
				item.SlotInfo.SlotIndex = EQ_BUCKLER2 - 1;
				m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_INSERT, (LPARAM)&item);

				SPItem* pItem = GetInventoryItem(FIGURE_EQUIP, EQ_BUCKLER1 - 1);
				if(pItem && !pItem->IsEmptyItem()) {
					CONTAINER_ITEM TakeOffItem;
					pItem->ConvertContainerItem(&TakeOffItem);
					m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&TakeOffItem);
				}
			}
			else {
				m_pParent->SPGOBPerformMessage( SPGM_EQUIP_UNLOAD, (LPARAM)EQ_BUCKLER1);

				SPItem* pItem = GetInventoryItem(FIGURE_EQUIP, EQ_BUCKLER1 - 1);
				if(pItem && !pItem->IsEmptyItem()) {
					CONTAINER_ITEM TakeOffItem;
					pItem->ConvertContainerItem(&TakeOffItem);
					m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)&TakeOffItem);
				}
			}
		}
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
SPItem* SPPlayerInvenArchive::GetInventoryItem(CONTAINER_TYPE iType, int iSlotIndex)
{	
	if(IsEnableContainer(iType) == false)
		return NULL;
	
	int iInvenSize = GetInventorySize(iType);
	
	if(iInvenSize < 0 || iSlotIndex < 0 || iInvenSize <= iSlotIndex)	return NULL;

	return m_vpInventory[iType].at(iSlotIndex);
}


int	SPPlayerInvenArchive::GetInventorySize(CONTAINER_TYPE iType)
{
	int iSize = static_cast<int>(m_vpInventory[iType].size());
	
	return iSize;
}


int SPPlayerInvenArchive::GetInventoryCurPage(CONTAINER_TYPE iType)
{	
	int iPage = m_iCurInvenPage[iType];
	return iPage;
}


bool SPPlayerInvenArchive::SetMouseIcon(/*SPItem* pItem*/)
{
	if(m_PickUpItem.m_iContainer < INVENTORY_EQUIP ||  m_PickUpItem.m_iContainer >= CONTAINER_MAX )
		return false;
	
	int iInvenSize = GetInventorySize(m_PickUpItem.m_iContainer);
	if(iInvenSize < 0 || iInvenSize <= m_PickUpItem.m_iSlotIndex)		return false;

	POINT ptSrcPos;
	SPTexture* icon = NULL;
	
	SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
	icon = g_pItemCluster->GetItemTexture( pPickUpItem->GetItemStatusValue(MV_ITEM_GETUID), ptSrcPos);
	g_pInterfaceManager->SetMousePickup(icon, ptSrcPos.x, ptSrcPos.y);

	if(m_PickUpItem.m_iContainer > CONTAINER_TYPE_INVALID && m_PickUpItem.m_iContainer < FIGURE_EQUIP) {
		int iDisableIndex = m_PickUpItem.m_iSlotIndex - (m_iCurInvenPage[m_PickUpItem.m_iContainer] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iDisableIndex, (LPARAM)true, NULL);
	}
	else if(m_PickUpItem.m_iContainer > INVENTORY_HOUSING && m_PickUpItem.m_iContainer < FIGURE_BASIC) {
		if(m_PickUpItem.m_iSlotIndex < EQ_WEAPON1 - 1) {
			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SELECT, (WPARAM)m_PickUpItem.m_iSlotIndex, (LPARAM)true, NULL);
			
			//ÇÑ¹ú¿ÊÀÎ °æ¿ì Ã³¸®
			int iItemID = pPickUpItem->GetItemStatusValue(MV_ITEM_GETUID);
			int iSlotCount = GetSlotCount(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
			if(m_PickUpItem.m_iSlotIndex == 3 && iSlotCount == 3) {
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SELECT, (WPARAM)4, (LPARAM)true, NULL);
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SELECT, (WPARAM)5, (LPARAM)true, NULL);
			}
		}		
		else if(m_PickUpItem.m_iSlotIndex == EQ_WEAPON1 - 1 || m_PickUpItem.m_iSlotIndex == EQ_BUCKLER1 - 1) {	//¹«±â 1
			//¾ç¼Õ¹«±â Ã¼Å©ÇÒ°Í!
			int iWeapon1 = m_PickUpItem.m_iSlotIndex - (EQ_WEAPON1 - 1);
			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SELECT1, (WPARAM)iWeapon1, (LPARAM)true, NULL);
		}
		else if(m_PickUpItem.m_iSlotIndex == EQ_WEAPON2 - 1 || m_PickUpItem.m_iSlotIndex == EQ_BUCKLER2 - 1) {	//¹«±â 2
			int iWeapon1 = m_PickUpItem.m_iSlotIndex - (EQ_WEAPON2 - 1);
			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SELECT2, (WPARAM)iWeapon1, (LPARAM)true, NULL);
		}
	}
	
	
	return true;
}


bool SPPlayerInvenArchive::RecoverIcon()
{
	bool bIsPlayerPvp = false;//ÇÃ·¹ÀÌ¾î°¡ PVP¸ðµå¿¡ µé¾î¿Ô´ÂÁö È®ÀÎ
	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP)) {
		if(g_pkPvpManager->GetPvpStatus() > PVP_STATUS_ENTERWAIT) {
			bIsPlayerPvp = true;
		}
	}
	
	if(m_bPickup == false /*|| m_bClickMove == false*/)			return false;
	if(m_PickUpItem.m_iContainer < INVENTORY_EQUIP || m_PickUpItem.m_iContainer >= CONTAINER_MAX) return false;	
	if(m_PickUpItem.m_iSlotIndex < 0 || m_PickUpItem.m_iSlotIndex >= GetInventorySize(m_PickUpItem.m_iContainer)) return false;	
	if(IsItemExist(m_PickUpItem.m_iSlotIndex, m_PickUpItem.m_iContainer) == false){
		m_bPickup = false;
		m_PickUpItem.Clear();
		DeletePickupItem();
		return false;
	}

	SPItem* pItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
	bool bItemAvailable = pItem->GetItemStatus()->IsTimeAvailable();
	bool bIsPvpItem = pItem->GetItemAttr()->m_bPvp;										//PVP¿¡¼­ »ç¿ëÇÒ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÎÁö È®ÀÎ
	
	if(m_PickUpItem.m_iContainer > CONTAINER_TYPE_INVALID && m_PickUpItem.m_iContainer < FIGURE_EQUIP) {
		int iRecoverIndex = m_PickUpItem.m_iSlotIndex - (m_iCurInvenPage[m_PickUpItem.m_iContainer] * INVENTORY_LINE);
		
		CONTAINER_ITEM item;
		GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex)->ConvertContainerItem((CONTAINER_ITEM*)&item);
		bool bEnable = CheckPCStatus(item.Item.iItemID);
		bEnable &= bItemAvailable;
		if(bIsPlayerPvp) {
			bEnable &= bIsPvpItem;
		}

		item.SlotInfo.SlotIndex = iRecoverIndex;	
		
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SETTING, (WPARAM)&item, (LPARAM)bEnable, NULL);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iRecoverIndex, (LPARAM)false, NULL);
		m_iItemLClickIndex = -1;
	}
	else if(m_PickUpItem.m_iContainer > INVENTORY_HOUSING && m_PickUpItem.m_iContainer < FIGURE_BASIC) {
		CONTAINER_ITEM item;
		GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex)->ConvertContainerItem((CONTAINER_ITEM*)&item);
		bool bEnable = CheckPCStatus(item.Item.iItemID);
		bEnable &= bItemAvailable;
		if(bIsPlayerPvp) {
			bEnable &= bIsPvpItem;
		}
		
		if(m_PickUpItem.m_iSlotIndex == EQ_WEAPON1 - 1 || m_PickUpItem.m_iSlotIndex == EQ_BUCKLER1 - 1) {	//¹«±â 1
			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SETTING1, (WPARAM)&item, (LPARAM)bEnable, NULL);
			
			int iSlotCount = GetSlotCount(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
			if(iSlotCount == 2) {
				item.SlotInfo.SlotIndex++;
				int iWeapon1 = item.SlotInfo.SlotIndex - (EQ_WEAPON1 - 1);
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SETTING1, (WPARAM)&item, (LPARAM)bEnable, NULL);
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SELECT1, (WPARAM)iWeapon1, (LPARAM)true, NULL);
			}
		}
		else if(m_PickUpItem.m_iSlotIndex == EQ_WEAPON2 - 1 || m_PickUpItem.m_iSlotIndex == EQ_BUCKLER2 - 1) {	//¹«±â 2			
			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SETTING2, (WPARAM)&item, (LPARAM)bEnable, NULL);
			
			int iSlotCount = GetSlotCount(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);			
			if(iSlotCount == 2) {
				item.SlotInfo.SlotIndex++;
				int iWeapon1 = item.SlotInfo.SlotIndex - (EQ_WEAPON2 - 1);
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SETTING2, (WPARAM)&item, (LPARAM)bEnable, NULL);
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SELECT2, (WPARAM)iWeapon1, (LPARAM)true, NULL);
			}
		}
		else if(m_PickUpItem.m_iSlotIndex < EQ_WEAPON1 - 1) {
			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SETTING, (WPARAM)&item, (LPARAM)bEnable, NULL);			

			//ÇÑ¹ú¿Ê Ã³¸®
			SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
			int iSlotCount = GetSlotCount(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
			int iItemID = pPickUpItem->GetItemStatusValue(MV_ITEM_GETUID);
			if(m_PickUpItem.m_iSlotIndex == 3 && iSlotCount == 3) {
				item.SlotInfo.SlotIndex = 4;
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SETTING, (WPARAM)&item, (LPARAM)bEnable, NULL);
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SELECT, (WPARAM)item.SlotInfo.SlotIndex, (LPARAM)true); //[2005/9/12]
				item.SlotInfo.SlotIndex = 5;
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SETTING, (WPARAM)&item, (LPARAM)bEnable, NULL);
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SELECT, (WPARAM)item.SlotInfo.SlotIndex, (LPARAM)true); //[2005/9/12]
			}
		}
		
		m_iEquipLClickIndex = -1;
	}
	else if(m_PickUpItem.m_iContainer == STORAGE) {
		int iRecoverIndex = m_PickUpItem.m_iSlotIndex - (m_iCurStoragePage * STORAGE_LINE);
		
		CONTAINER_ITEM item;
		GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex)->ConvertContainerItem((CONTAINER_ITEM*)&item);
		bool bEnable = CheckPCStatus(item.Item.iItemID);
		bEnable &= bItemAvailable;
		if(bIsPlayerPvp) {
			bEnable &= bIsPvpItem;
		}
		
		item.SlotInfo.SlotIndex = iRecoverIndex;
		g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_SETTING, (WPARAM)&item, (LPARAM)bEnable, NULL);		
		
		g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iRecoverIndex, (LPARAM)false, NULL);
		m_iStorageLClickIndex = -1;
	}
	else if(m_PickUpItem.m_iContainer == FIGURE_PET) {
		int iRecoverIndex = m_PickUpItem.m_iSlotIndex - (m_iCurStoragePage * STORAGE_LINE);

		CONTAINER_ITEM item;
		GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex)->ConvertContainerItem((CONTAINER_ITEM*)&item);
		bool bEnable = CheckPCStatus(item.Item.iItemID);
		bEnable &= bItemAvailable;
		if(bIsPlayerPvp) {
			bEnable &= bIsPvpItem;
		}

		item.SlotInfo.SlotIndex = iRecoverIndex;
		m_PickUpItem.Clear();
		m_bPickup = false;
		DeletePickupItem();	
		m_bClickMove = false;	

		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PET);
		if(pWindow) {
			pWindow->SPSendMessage(SPIM_ITEM_SETTING, (WPARAM)&item, (LPARAM)bEnable, NULL);
			pWindow->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iRecoverIndex, (LPARAM)false, NULL);
		}		
		
		m_iPetLClickIndex = -1;
	}
	else {
		DXUTOutputDebugString("RecoverIcon Not Defined Container Type\n");
	}
	
	m_PickUpItem.Clear();
	m_bPickup = false;
	DeletePickupItem();	
	
	m_bClickMove = false;	

	DXUTOutputDebugString("** RecoverIcon PickUpItem Container[%d] SlotIndex[%d]\n", m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);

	g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_ICON_PUT_DOWN.WAV");	
	
	return true;
}


bool SPPlayerInvenArchive::SetInventoryIcon(CONTAINER_ITEM *pContainerItem, bool bSetEquip /*= true*/)
{
	SPPlayer* pPlayer = (SPPlayer*)m_pParent;
	
	bool bIsPlayerPvp = false;//ÇÃ·¹ÀÌ¾î°¡ PVP¸ðµå¿¡ µé¾î¿Ô´ÂÁö È®ÀÎ
	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP)) {
		if(g_pkPvpManager->GetPvpStatus() > PVP_STATUS_ENTERWAIT) {
			bIsPlayerPvp = true;
		}
	}
	
	CONTAINER_TYPE ContainerType = pContainerItem->SlotInfo.ContainerType;
	
	if(ContainerType <  INVENTORY_EQUIP || ContainerType >= CONTAINER_MAX) 
		return false;

	SPItem* pItem = GetInventoryItem(pContainerItem->SlotInfo.ContainerType, pContainerItem->SlotInfo.SlotIndex);
	if(pItem == NULL || pItem->GetItemStatus() == NULL || pItem->GetItemStatus()->GetItemID() == 0)
		return false;
	
	bool bItemAvailable = pItem->GetItemStatus()->IsTimeAvailable();					//½Ã°£À¯È¿ ¾ÆÀÌÅÛÀÎÁö È®ÀÎ
	bool bIsPvpItem = pItem->GetItemAttr()->m_bPvp;										//PVP¿¡¼­ »ç¿ëÇÒ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÎÁö È®ÀÎ
	
	if(ContainerType > CONTAINER_TYPE_INVALID && ContainerType < FIGURE_EQUIP) {		//¾ÆÀÌÅÛÃ¢
		if(pContainerItem->SlotInfo.ContainerType == m_iCurrentInventoryType) {
			bool bEnable = CheckPCStatus(pContainerItem->Item.iItemID);
			bEnable &= bItemAvailable;
			if(bIsPlayerPvp) {
				bEnable &= bIsPvpItem;
			}
			pContainerItem->SlotInfo.SlotIndex = pContainerItem->SlotInfo.SlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SETTING, (WPARAM)pContainerItem, (LPARAM)bEnable, NULL);			

			if(ContainerType == INVENTORY_EQUIP)
				CheckSetItem(pContainerItem->Item.iItemID);
		}		
	}
	else if(ContainerType == STORAGE) {													//Ã¢°í
		bool bEnable = CheckPCStatus(pContainerItem->Item.iItemID);
		bEnable &= bItemAvailable;
		if(bIsPlayerPvp) {
			bEnable &= bIsPvpItem;
		}
		pContainerItem->SlotInfo.SlotIndex = pContainerItem->SlotInfo.SlotIndex - (m_iCurStoragePage * STORAGE_LINE);
		g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_ITEM_SETTING, (WPARAM)pContainerItem, (LPARAM)bEnable, NULL);
	}
	else if(ContainerType == FIGURE_PET) {
		bool bEnable = CheckPCStatus(pContainerItem->Item.iItemID);

		if (pItem->GetItemAttr()->m_iNewPet == 0)
		{/* old pet */
			bEnable &= bItemAvailable;
			if(bIsPlayerPvp) {
				bEnable &= bIsPvpItem;
			}
			
			//[2006/11/23] Àåºñ Âø¿ë
			if(bSetEquip && bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
				m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_INSERT, (LPARAM)pContainerItem);
			}
			else {
				m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)pContainerItem);
			}
		}
		else
		{/* new pet */
			if (bIsPlayerPvp) {
				bEnable &= bIsPvpItem;
			}

			if (bSetEquip && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
				m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_INSERT, (LPARAM)pContainerItem);
			}
			else {
				m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)pContainerItem);
			}
		}

		g_pInterfaceManager->GetPetWindow()->SPSendMessage(SPIM_ITEM_SETTING, (WPARAM)pContainerItem, (LPARAM)bEnable, NULL);					
	}
	else if(ContainerType > INVENTORY_HOUSING && ContainerType < STORAGE) {				//ÀåºñÃ¢		
		bool bEnable = CheckPCStatus(pContainerItem->Item.iItemID);
		bEnable &= bItemAvailable;		
		if(bIsPlayerPvp) {
			bEnable &= bIsPvpItem;
		}

		if(ContainerType == FIGURE_EQUIP) {
			if(m_iCurrentFigureType != ContainerType){
				//DXUTOutputDebugString("SetInventoryIcon Disable ContainerType Equip\n");
			}

			int iSlotIndex = pContainerItem->SlotInfo.SlotIndex;
			
			if(iSlotIndex == EQ_WEAPON1 - 1 || iSlotIndex == EQ_BUCKLER1 - 1)
			{
				//[2005/10/20] ±âÁ¸ÀÇ Àåºñ¸¦ ¸ÕÀú »«´Ù
				int iSlotCount = GetSlotCount(pContainerItem->SlotInfo.ContainerType, pContainerItem->SlotInfo.SlotIndex);
				if(m_iCurrentWeaponSlot == 1) {
					if(iSlotIndex == EQ_WEAPON1 - 1) {
						if(bSetEquip && bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_WEAPON1);
						}
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR1, 0, 0, NULL);
						if(iSlotCount == 2) {
							g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR1, 0, 1, NULL);
						}
					}
					else if(iSlotIndex == EQ_BUCKLER1 - 1) {
						if(bSetEquip && bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_BUCKLER1);
						}
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR1, 0, 1, NULL);
					}
				}				
				
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SETTING1, (WPARAM)pContainerItem, (LPARAM)bEnable, NULL);
				CheckSetItem(pContainerItem->Item.iItemID);

				if(iSlotIndex == EQ_WEAPON1 - 1 && iSlotCount == 1) {
					if(IsItemExist(EQ_BUCKLER1 - 1, FIGURE_EQUIP) ==  false)
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR1, 0, 1, NULL);
				}
				else if(iSlotCount == 2) {
					pContainerItem->SlotInfo.SlotIndex++;
					int iWeapon1 = pContainerItem->SlotInfo.SlotIndex - (EQ_WEAPON1 - 1);
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SETTING1, (WPARAM)pContainerItem, (LPARAM)bEnable, NULL);
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SELECT1, (WPARAM)iWeapon1, (LPARAM)true, NULL);
				}
				
				if(m_iCurrentWeaponSlot == 1) {
					if(bSetEquip && bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {						
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)pContainerItem);

						if(iSlotCount == 2)
							pContainerItem->SlotInfo.SlotIndex--;
						
						m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_INSERT, (LPARAM)pContainerItem);
						CheckSetItem(pContainerItem->Item.iItemID);
					}
					else {
						//m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)pContainerItem);
						CheckSetItem(pContainerItem->Item.iItemID);
					}
				}
			}
			else if(iSlotIndex == EQ_WEAPON2-1 || iSlotIndex == EQ_BUCKLER2-1)
			{
				//[2005/10/20] ±âÁ¸ÀÇ Àåºñ¸¦ ¸ÕÀú »«´Ù
				int iSlotCount = GetSlotCount(pContainerItem->SlotInfo.ContainerType, pContainerItem->SlotInfo.SlotIndex);
				if(m_iCurrentWeaponSlot != 1) {
					if(iSlotIndex == EQ_WEAPON2 - 1) {					
						if(bSetEquip && bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_WEAPON1);
						}
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR2, 0, 0, NULL);
						if(iSlotCount == 2) {
							g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR2, 0, 1, NULL);
						}
					}
					else if(iSlotIndex == EQ_BUCKLER2 - 1) {
						if(bSetEquip && bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_BUCKLER1);
						}
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR2, 0, 1, NULL);
					}
				}				
				
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SETTING2, (WPARAM)pContainerItem, (LPARAM)bEnable, NULL);
				CheckSetItem(pContainerItem->Item.iItemID);
				
				if(iSlotIndex == EQ_WEAPON2 - 1 && iSlotCount == 1) {
					if(IsItemExist(EQ_BUCKLER2 - 1, FIGURE_EQUIP) ==  false)
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR2, 0, 1, NULL);
				}
				else if(iSlotCount == 2) {
					pContainerItem->SlotInfo.SlotIndex++;
					int iWeapon2 = pContainerItem->SlotInfo.SlotIndex - (EQ_WEAPON2 - 1);
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SETTING2, (WPARAM)pContainerItem, (LPARAM)bEnable, NULL);
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_SELECT2, (WPARAM)iWeapon2, (LPARAM)true, NULL);
				}
				
				if(m_iCurrentWeaponSlot == 2) {
					if(bSetEquip && bItemAvailable &&  pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {						
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)pContainerItem);
						
						if(iSlotCount == 2)
							pContainerItem->SlotInfo.SlotIndex--;
						
						m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_INSERT, (LPARAM)pContainerItem);
						CheckSetItem(pContainerItem->Item.iItemID);
					}
					else {
						//m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)pContainerItem);
						CheckSetItem(pContainerItem->Item.iItemID);
					}
				}
			}			
			else
			{
				bool bFashionExist = IsItemExist(iSlotIndex, FIGURE_FASHION);
				bool bOnePieceExist = false;
				bool bFashionAvailable = true;
				
				//ÆÐ¼Ç ÇÑ¹ú¿Ê°ú ½½·ÔÀÌ °ãÄ¡´Â °æ¿ì
				if(!bFashionExist && iSlotIndex == 4 || iSlotIndex == 5){
					if(IsItemExist(3, FIGURE_FASHION)) {
						SPItem* pOnePiece = GetInventoryItem(FIGURE_FASHION, 3);
						bFashionAvailable = pOnePiece->GetItemStatus()->IsTimeAvailable();
						int iItemID = pOnePiece->GetItemStatusValue(MV_ITEM_GETUID);
						int iSlotCount = GetSlotCount(FIGURE_FASHION, 3);
						if(iSlotCount == 3)
							bOnePieceExist = true;
					}					
				}
				
				if((bFashionExist && bFashionAvailable) || bOnePieceExist){
				}				
				else {
					if(bSetEquip && bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)pContainerItem);
						m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_INSERT, (LPARAM)pContainerItem);
						CheckSetItem(pContainerItem->Item.iItemID);
					}
					else {
						//m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)pContainerItem);
						CheckSetItem(pContainerItem->Item.iItemID);
					}
				}
				
				if(ContainerType == m_iCurrentFigureType) {										
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SETTING, (WPARAM)pContainerItem, (LPARAM)bEnable, NULL);
					CheckSetItem(pContainerItem->Item.iItemID);
				}
			}
		}
		else if(ContainerType == FIGURE_FASHION) {
			if(m_iCurrentFigureType != ContainerType){
				//DXUTOutputDebugString("SetInventoryIcon Disable ContainerType Fashion\n");
			}
			
			int iCurIndex = pContainerItem->SlotInfo.SlotIndex;			
						
			//ÇÑ¹ú¿ÊÀÎ °æ¿ì ¾ÆÀÌÄÜ ¼ÂÆÃ...
			if(pContainerItem->SlotInfo.SlotIndex == 3) {
				int iSlotCount = GetSlotCount(pContainerItem->SlotInfo.ContainerType, pContainerItem->SlotInfo.SlotIndex);				
				
				if(iSlotCount == 3){			//itemSetting
					pContainerItem->SlotInfo.SlotIndex++;
					if(ContainerType == m_iCurrentFigureType){
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)pContainerItem->SlotInfo.SlotIndex, NULL);
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SETTING, (WPARAM)pContainerItem, (LPARAM)bEnable, NULL);
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SELECT, (WPARAM)pContainerItem->SlotInfo.SlotIndex, (LPARAM)true, NULL);
					}

					if(IsItemExist(pContainerItem->SlotInfo.SlotIndex, FIGURE_EQUIP)) {
						if(bSetEquip && bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)pContainerItem->SlotInfo.SlotIndex + 1);
						}
					}
					
					pContainerItem->SlotInfo.SlotIndex++;
					if(ContainerType == m_iCurrentFigureType){
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)pContainerItem->SlotInfo.SlotIndex, NULL);
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SETTING, (WPARAM)pContainerItem, (LPARAM)bEnable, NULL);
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SELECT, (WPARAM)pContainerItem->SlotInfo.SlotIndex, (LPARAM)true, NULL);
					}

					if(IsItemExist(pContainerItem->SlotInfo.SlotIndex, FIGURE_EQUIP)) {
						if(bSetEquip && bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)pContainerItem->SlotInfo.SlotIndex + 1);
						}
					}
				}
				else {				//[2005/7/6] ÇÑ¹ú¿ÊÀÌ ¾Æ´Ñ°æ¿ì
					bool bFashionExist = false;
					bool bEquipExist = false;
					bool bTimeAvailable = true;
					
					pContainerItem->SlotInfo.SlotIndex++;
					bFashionExist = IsItemExist(pContainerItem->SlotInfo.SlotIndex, FIGURE_FASHION);
					bEquipExist = IsItemExist(pContainerItem->SlotInfo.SlotIndex, FIGURE_EQUIP);					
					
					if(bFashionExist)
						bTimeAvailable = GetInventoryItem(FIGURE_FASHION, pContainerItem->SlotInfo.SlotIndex)->GetItemStatus()->IsTimeAvailable();
					
					if(bFashionExist == false || bTimeAvailable == false) {
						if(ContainerType == m_iCurrentFigureType)
							g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)pContainerItem->SlotInfo.SlotIndex, NULL);
						if(bEquipExist) {
							int iEquipItemID = m_vpInventory[FIGURE_EQUIP].at(pContainerItem->SlotInfo.SlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
							if(iEquipItemID!=0)
							{
								//Item ¸¸µé±â
								CONTAINER_ITEM item;
								item.Item.iItemID = iEquipItemID;
								item.Item.iStackCount = 1;
								if(bSetEquip && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
								}
							}
						}
					}

					pContainerItem->SlotInfo.SlotIndex++;
					bFashionExist = IsItemExist(pContainerItem->SlotInfo.SlotIndex, FIGURE_FASHION);
					bEquipExist = IsItemExist(pContainerItem->SlotInfo.SlotIndex, FIGURE_EQUIP);
					if(bFashionExist)
						bTimeAvailable = GetInventoryItem(FIGURE_FASHION, pContainerItem->SlotInfo.SlotIndex)->GetItemStatus()->IsTimeAvailable();

					if(bFashionExist == false || bTimeAvailable == false) {
						if(ContainerType == m_iCurrentFigureType)
							g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)pContainerItem->SlotInfo.SlotIndex, NULL);
						if(bEquipExist) {
							int iEquipItemID = m_vpInventory[FIGURE_EQUIP].at(pContainerItem->SlotInfo.SlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
							if(iEquipItemID!=0)
							{
								//Item ¸¸µé±â
								CONTAINER_ITEM item;
								item.Item.iItemID = iEquipItemID;
								item.Item.iStackCount = 1;
								if(bSetEquip && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
									m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_INSERT, (LPARAM)pContainerItem);
									CheckSetItem(pContainerItem->Item.iItemID);
								}
								else {
									//m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)pContainerItem);
									CheckSetItem(pContainerItem->Item.iItemID);
								}
							}
						}
					}
				}
			}
			else if(pContainerItem->SlotInfo.SlotIndex == 4 || pContainerItem->SlotInfo.SlotIndex == 5) {
				SPItem* pOnePiece = GetInventoryItem(FIGURE_FASHION, 3);
				bool bTimeAvailable = pOnePiece->GetItemStatus()->IsTimeAvailable();
				
				int iItemID = pOnePiece->GetItemStatusValue(MV_ITEM_GETUID);
				int iSlotCount = GetSlotCount(FIGURE_FASHION, 3);
				bool bOnePieceExist = false;
				if(iItemID != 0) {
					if(iSlotCount == 3)
						bOnePieceExist = true;
				}
				
				if(bOnePieceExist && bTimeAvailable) {
					int iPrevSlotIndex = 0;
					if(bOnePieceExist && pContainerItem->SlotInfo.SlotIndex == 4) {
						iPrevSlotIndex = pContainerItem->SlotInfo.SlotIndex - 1;
						if(ContainerType == m_iCurrentFigureType){
							g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)iPrevSlotIndex, NULL);
						}

						if(bSetEquip && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)iPrevSlotIndex + 1);
						}

						iPrevSlotIndex = pContainerItem->SlotInfo.SlotIndex + 1;
						if(ContainerType == m_iCurrentFigureType){
							g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)iPrevSlotIndex, NULL);
						}
						if(bSetEquip && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)iPrevSlotIndex + 1);
						}
					}
					if(bOnePieceExist && pContainerItem->SlotInfo.SlotIndex == 5) {
						iPrevSlotIndex = 3;
						if(ContainerType == m_iCurrentFigureType){
							g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)iPrevSlotIndex, NULL);
						}
						
						if(bSetEquip && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)iPrevSlotIndex + 1);
						}

						iPrevSlotIndex = 4;
						if(ContainerType == m_iCurrentFigureType){
							g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)iPrevSlotIndex, NULL);
						}
						
						if(bSetEquip && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)iPrevSlotIndex + 1);
						}
					}
				}				
			}
 			
			pContainerItem->SlotInfo.SlotIndex = iCurIndex;
			if(ContainerType == m_iCurrentFigureType) {				
				g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SETTING, (WPARAM)pContainerItem, (LPARAM)bEnable, NULL);			
			}
			if(bSetEquip && bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
				m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)pContainerItem);				
				m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_INSERT, (LPARAM)pContainerItem);
				CheckSetItem(pContainerItem->Item.iItemID);
			}
			else {
				//m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)pContainerItem);
				CheckSetItem(pContainerItem->Item.iItemID);
			}
		}
	}			
	else if (ContainerType == FIGURE_EFFECT)
	{
		bool bEnable = CheckPCStatus( pContainerItem->Item.iItemID);
		bEnable &= bItemAvailable;

		if(bIsPlayerPvp) {
			bEnable &= bIsPvpItem;
		}	
		if(m_iCurrentFigureType != ContainerType){
			//DXUTOutputDebugString("SetInventoryIcon Disable ContainerType Fashion\n");
		}

		if(ContainerType == m_iCurrentFigureType) {				
			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_SETTING, (WPARAM)pContainerItem, (LPARAM)bEnable, NULL);			
		}
		if(bSetEquip && bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
			m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)pContainerItem);				
			m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_INSERT, (LPARAM)pContainerItem);
			CheckSetItem(pContainerItem->Item.iItemID);
		}
		else {
			//m_pParent->SPGOBPerformMessage(SPGM_ITEM_SKILL_DELETE, (LPARAM)pContainerItem);
			CheckSetItem(pContainerItem->Item.iItemID);
		}
	}
	
	return true;
}


bool SPPlayerInvenArchive::SetEquipTakeOff( CONTAINER_ITEM *pContainerItem )
{
	CONTAINER_TYPE iContainer = pContainerItem->SlotInfo.ContainerType;
	int iSlotIndex = pContainerItem->SlotInfo.SlotIndex;
	SPPlayer* pPlayer = (SPPlayer*)m_pParent;
	
	if( iContainer < FIGURE_EQUIP || iContainer > FIGURE_BASIC)
		return false;

	if(iSlotIndex >= GetInventorySize(iContainer))
		return false;

	if(IsItemExist(iSlotIndex, iContainer) == false)
		return false;	

	CheckSetItem(pContainerItem->Item.iItemID);

	int iSlotCount = GetSlotCount(iContainer, iSlotIndex);

	//////////////////////////////////////////////////////////////////////////	
	if((iContainer == FIGURE_EQUIP || iContainer == FIGURE_FASHION || iContainer == FIGURE_EFFECT)) {
		//ÇÑ¹ú¿ÊÀÌ ÀÎº¥Åä¸®·Î ¿Ã¶ó°£ °æ¿ì
		//if( ((iDestItemID / 10000000) == 5) && iSrcSlotIndex == 3 ) {
		if(iSlotIndex == 3) {
			bool bEquipExist = false;
			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)iSlotIndex, NULL);
			m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_ONE_PIECE);

			bEquipExist = IsItemExist(3, FIGURE_EQUIP);
			if(bEquipExist) {
				int iEquipItemID = m_vpInventory[FIGURE_EQUIP].at(3)->GetItemStatusValue(MV_ITEM_GETUID);
				if(iEquipItemID!=0)
				{
					//Item ¸¸µé±â
					CONTAINER_ITEM item;
					item.Item.iItemID = iEquipItemID;
					item.Item.iStackCount = 1;
					m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
				}
			}
			else {
				m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(3 + 1));			
			}			

			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)4, NULL);	
			bEquipExist = IsItemExist(4, FIGURE_EQUIP);		//ÇÑ¹ú¿ÊÀ» ¹þÀº °æ¿ì Àåºñ·Î ·£´õ
			if(bEquipExist) {
				int iEquipItemID = m_vpInventory[FIGURE_EQUIP].at(4)->GetItemStatusValue(MV_ITEM_GETUID);
				if(iEquipItemID!=0)
				{
					//Item ¸¸µé±â
					CONTAINER_ITEM item;
					item.Item.iItemID = iEquipItemID;
					item.Item.iStackCount = 1;
					m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
				}
			}
			else {
				m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(4 + 1));			
			}			

			g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)5, NULL);
			bEquipExist = IsItemExist(5, FIGURE_EQUIP);		//ÇÑ¹ú¿ÊÀ» ¹þÀº °æ¿ì Àåºñ·Î ·£´õ
			if(bEquipExist) {
				int iEquipItemID = m_vpInventory[FIGURE_EQUIP].at(5)->GetItemStatusValue(MV_ITEM_GETUID);
				if(iEquipItemID!=0)
				{
					//Item ¸¸µé±â
					CONTAINER_ITEM item;
					item.Item.iItemID = iEquipItemID;
					item.Item.iStackCount = 1;
					m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
				}
			}
			else {				
				m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(5 + 1));	
			}
		}
		else {
			if(iContainer == FIGURE_EQUIP) {				
				if(iSlotIndex < EQ_WEAPON1 - 1) {
					bool bFashionExist = false;

					//ÇØ´ç ¾ÆÀÌÄÜÀ» Áö¿ì°í Àåºñ¸¦ ¾ð·Îµå ÇÑ´Ù
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)iSlotIndex, NULL);					

					bFashionExist = IsItemExist(iSlotIndex, FIGURE_FASHION);
					if(bFashionExist) {
						int iFashionItemID = m_vpInventory[FIGURE_FASHION].at(iSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
						if(iFashionItemID!=0) {
							//Item ¸¸µé±â
							CONTAINER_ITEM item;
							item.Item.iItemID = iFashionItemID;
							item.Item.iStackCount = 1;
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
						}
					}
					else {
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(iSlotIndex + 1));
					}
				}
				else if(iSlotIndex == EQ_WEAPON1 - 1 || iSlotIndex == EQ_BUCKLER1 - 1) {	//
					int iClearSlot = iSlotIndex - (EQ_WEAPON1 - 1);
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR1, 0, (LPARAM)iClearSlot, NULL);
					
					if(iSlotIndex == EQ_WEAPON1 - 1 && iSlotCount == 2) {
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR1, 0, iClearSlot + 1, NULL);
					}

					if(m_iCurrentWeaponSlot == 1)
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(iSlotIndex + 1));
				}
				else if(iSlotIndex == EQ_WEAPON2 - 1 || iSlotIndex == EQ_BUCKLER2 - 1) {	//
					int iClearSlot = iSlotIndex - (EQ_WEAPON2 - 1);
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR2, 0, (LPARAM)iClearSlot, NULL);
					
					if(iSlotIndex == EQ_WEAPON2 - 1 && iSlotCount == 2) {
						g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_WEAPON_CLEAR2, 0, iClearSlot + 1, NULL);
					}					

					if(m_iCurrentWeaponSlot != 1) {
						if(iSlotIndex == EQ_WEAPON2 - 1) {
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_WEAPON1);
						}
						else if(iSlotIndex == EQ_BUCKLER2 - 1)
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_BUCKLER1);
					}
				}
			}
			else if(iContainer == FIGURE_FASHION || iContainer == FIGURE_EFFECT) {
				if(iSlotIndex < EQ_WEAPON1 - 1) {
					bool bEquipExist = false;

					//ÇØ´ç ¾ÆÀÌÄÜÀ» Áö¿ì°í Àåºñ¸¦ ¾ð·Îµå ÇÑ´Ù
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_EQUIP_CLEAR, 0, (LPARAM)iSlotIndex, NULL);

					bEquipExist = IsItemExist(iSlotIndex, FIGURE_EQUIP);
					if(bEquipExist) {
						int iEquipItemID = m_vpInventory[FIGURE_EQUIP].at(iSlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
						if(iEquipItemID!=0)
						{
							//Item ¸¸µé±â
							CONTAINER_ITEM item;
							item.Item.iItemID = iEquipItemID;
							item.Item.iStackCount = 1;
							m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
						}
					}
					else {
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)(iSlotIndex + 1));
					}
				}
			}
		}		
	}

	return true;
}


int SPPlayerInvenArchive::GetSlotCount(CONTAINER_TYPE iType, int iSlotIndex)
{
	if(iType < CONTAINER_TYPE_INVALID || iType > STORAGE)		return -1;

	if(iSlotIndex < 0 || iSlotIndex >= GetInventorySize(iType))	return -1;

	int iCount = 0;
	if(IsItemExist(iSlotIndex, iType) == false)					return -1;
	
	SPItem* pItem = GetInventoryItem(iType, iSlotIndex);
	if(pItem->GetItemAttr()->m_ePosID1 > 0)
		iCount++;
	
	if(pItem->GetItemAttr()->m_eSubPosID11 > 0)
		iCount++;

	if(pItem->GetItemAttr()->m_eSubPosID12 > 0)
		iCount++;
	
	//DXUTOutputDebugString("GetSlotCount [%d] \n", iCount);

	return iCount;
}


int SPPlayerInvenArchive::GetTotalSetItemPieces(int iSetID)
{
	int iSetPieces = 0;
	if(iSetID == 0)	
		return iSetPieces;	
	
	std::vector<SPItem*>::iterator iter;
	for(int iType = FIGURE_EQUIP; iType < FIGURE_BASIC; iType++) {
		iter = m_vpInventory[iType].begin();
		for(; iter != m_vpInventory[iType].end(); ++iter) {
			if((*iter)->IsEmptyItem() == false) {
				if((*iter)->GetItemAttr()->m_iSetID == iSetID) {
					if((*iter)->GetItemStatus()->IsTimeAvailable())
						iSetPieces++;
				}
			}
		}
	}

	return iSetPieces;
}


void SPPlayerInvenArchive::SetSellSelectItem(SPSelectItem SellItem, bool bSell /*= true*/)
{	
	if(bSell) {
		m_SellItem = SellItem;
		int iSellIndex = m_SellItem.m_iSlotIndex - (m_iCurInvenPage[m_SellItem.m_iContainer] * INVENTORY_LINE);		
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iSellIndex, (LPARAM)true, NULL);
	}
	else {
		m_SellItem = SellItem;
		int iSellIndex = m_SellItem.m_iSlotIndex - (m_iCurInvenPage[m_SellItem.m_iContainer] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iSellIndex, (LPARAM)false, NULL);
		m_SellItem.Clear();
	}
}


SPItem* SPPlayerInvenArchive::GetSPItem(CONTAINER_TYPE iType, int iItemID, int iStack/* = 1*/)
{	
	SPItem* pItem = NULL;
	int iCheckItemID = 0;

	if(iType < INVENTORY_EQUIP || iType > INVENTORY_HOUSING)
		return NULL;

	if(iItemID == 0)
		return NULL;

	std::vector<SPItem*>::iterator iter = m_vpInventory[iType].begin();
	for(; iter != m_vpInventory[iType].end(); ++iter) {
		iCheckItemID = (*iter)->GetItemStatusValue(MV_ITEM_GETUID);
		if(iCheckItemID == iItemID) {
			pItem = (*iter);			
			if(pItem->GetItemStatus()->GetStackCount() >= iStack)
				return pItem;
		}
	}

	return pItem;
}


int	SPPlayerInvenArchive::GetTotalStackCount(CONTAINER_TYPE iType, int iItemID, bool bCheckRequire /*= true*/)
{
	int iTotalStackCount = 0;
	int iCheckItemID = 0;
	bool bItemAvailable = false;				//[2006/10/31] À¯È¿±â°£ Ã¼Å©

	if(iType < INVENTORY_EQUIP || iType > INVENTORY_HOUSING) 
		return iTotalStackCount;
	
	std::vector<SPItem*>::iterator iter = m_vpInventory[iType].begin();
	for(; iter != m_vpInventory[iType].end(); ++iter) {
		iCheckItemID = (*iter)->GetItemStatusValue(MV_ITEM_GETUID);		
		
		if(bCheckRequire)
			bItemAvailable = (*iter)->GetItemStatus()->IsTimeAvailable();
		else 
			bItemAvailable = true;
		
		if(iCheckItemID == iItemID && bItemAvailable) {
			iTotalStackCount += (*iter)->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
		}
	}
	
	//DXUTOutputDebugString("GetTotalStackCount container[%d] itemID[%d] totalStack[%d]\n", iType, iItemID, iTotalStackCount);
	return iTotalStackCount;
}


bool SPPlayerInvenArchive::IsBiggerStack(CONTAINER_TYPE iType, int iItemID, int iCheckStack)
{
	int iTotalStackCount = 0;
	int iCheckItemID = 0;
	bool bItemAvailable = false;				//[2006/10/31] À¯È¿±â°£ Ã¼Å©
	
	std::vector<SPItem*>::iterator iter = m_vpInventory[iType].begin();
	for(; iter != m_vpInventory[iType].end(); ++iter) {
		iCheckItemID = (*iter)->GetItemStatusValue(MV_ITEM_GETUID);
		bItemAvailable = (*iter)->GetItemStatus()->IsTimeAvailable();
		if(iCheckItemID == iItemID && bItemAvailable) {
			iTotalStackCount = (*iter)->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
			if(iTotalStackCount >= iCheckStack) {
				return true;
			}
		}
	}
	
	return false;
}


bool SPPlayerInvenArchive::IsBiggerStack5(CONTAINER_TYPE iType, int iItemID, int iCheckStack)
{
	int iTotalStackCount = 0;
	int iCheckItemID = 0;
	int iCheckSlotCount = 0;
	bool bItemAvailable = false;				//[2006/10/31] À¯È¿±â°£ Ã¼Å©

	std::vector<SPItem*>::iterator iter = m_vpInventory[iType].begin();
	for(; iter != m_vpInventory[iType].end(); ++iter) {
		iCheckItemID = (*iter)->GetItemStatusValue(MV_ITEM_GETUID);
		bItemAvailable = (*iter)->GetItemStatus()->IsTimeAvailable();
		if(iCheckItemID == iItemID && bItemAvailable) {
			if(iCheckSlotCount > 4) {
				return false;
			}
			
			iTotalStackCount += (*iter)->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
			iCheckSlotCount++;
			if(iTotalStackCount >= iCheckStack) {
				return true;
			}			
		}
	}
	
	return false;
}


bool SPPlayerInvenArchive::CheckEquipInven(int iCheckType, int iCheckSlotIndex /*= 0*/)
{
	CONTAINER_TYPE iContainer = CONTAINER_TYPE_INVALID;

	if(iCheckType == 1 || iCheckType == 3) {
		iContainer = FIGURE_EQUIP;		
	}
	else if(iCheckType == 2 || iCheckType == 4) {
		iContainer = FIGURE_FASHION;
	}

	bool bResult = false;

	if(iCheckSlotIndex == EQUIP_SLOT_MAX - 1) {			//¸ðµç ½½·Ô Ã¼Å©
		bResult = !IsEmptyInventory(iContainer);			//¸ðµÎ ºñ¾î ÀÖÀ¸¸é true
	}
	else {												//ÁöÁ¤µÈ ½½·ÔÀ» Ã¼Å©
		bResult = IsItemExist(iCheckSlotIndex, iContainer);	//¾ÆÀÌÅÛÀÌ Á¸ÀçÇÏ¸é true
	}

	if(iCheckType == 1 || iCheckType == 2)				//ÀåÂøµÇ¾î ÀÖ´Â °æ¿ì ¸¸Á· 
		return bResult;
	else if(iCheckType == 3 || iCheckType == 4)			//ÀåÂøµÇ¾î ÀÖÁö ¾ÊÀº °æ¿ì ¸¸Á·
		return !bResult;

	else
		return false;	
}


bool SPPlayerInvenArchive::CheckEquipMotion(int iCheckType, int iCheckSlotIndex /*= 0*/, int iCheckMotionID /*= 0*/)
{
	bool bResult = false;

	CONTAINER_TYPE iContainer = CONTAINER_TYPE_INVALID;

	if(iCheckType == 1 || iCheckType == 3) {
		iContainer = FIGURE_EQUIP;		
	}
	else if(iCheckType == 2 || iCheckType == 4) {
		iContainer = FIGURE_FASHION;
	}

	if(IsItemExist(iCheckSlotIndex, iContainer)) {
		SPItem* pItem = GetInventoryItem(iContainer, iCheckSlotIndex);
		if(pItem->GetItemAttr()->m_iMotionTypeID1) {
			if(pItem->GetItemAttr()->m_iMotionTypeID1 == iCheckMotionID) {
				bResult = true;
				
				if(iCheckType == 1 || iCheckType == 2)			//¸Â´Â°æ¿ì ¸¸Á·
					return bResult;
				else if(iCheckType == 3 || iCheckType == 4)		//´Ù¸¥°æ¿ì ¸¸Á·
					return !bResult;
			}
		}
	}

	return bResult;
}


int SPPlayerInvenArchive::GetPickUpItemID()
{
	if(m_PickUpItem.m_iContainer < INVENTORY_EQUIP || m_PickUpItem.m_iContainer > STORAGE)
		return 0;

	if(m_PickUpItem.m_iSlotIndex < 0 || m_PickUpItem.m_iSlotIndex >= GetInventorySize(m_PickUpItem.m_iContainer))
		return 0;

	if(IsItemExist(m_PickUpItem.m_iSlotIndex, m_PickUpItem.m_iContainer) == false)
		return 0;

	SPItem* pPickUpItem = GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
	int iPickUpItemID = pPickUpItem->GetItemStatusValue(MV_ITEM_GETUID);
	
	return iPickUpItemID;
}


bool SPPlayerInvenArchive::CheckPCStatus(int iItemID /*= 0*/, bool bPvp /*= false*/)
{
	int iCheckItemID = iItemID;
	if(iCheckItemID == 0) {
		iCheckItemID = GetPickUpItemID();
	}

	return g_pCheckManager->CheckItemStatus( iCheckItemID, bPvp );
}

bool SPPlayerInvenArchive::CheckEquipWeddingItem()
{
	int iContainer = FIGURE_EQUIP;
	std::vector<SPItem*>::iterator	iter;
	for(; iContainer < FIGURE_BASIC; iContainer++) {
		iter = m_vpInventory[iContainer].begin();
		for(; iter != m_vpInventory[iContainer].end(); ++iter) {
			if(!(*iter)->IsEmptyItem()) {
				if((*iter)->GetItemAttr()->m_bWedding)
					return true;
			}
		}
	}

	return false;
}

bool SPPlayerInvenArchive::SendQuickSlotUpdate(int iItemID)
{
	if(iItemID == 0)
		return false;

	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_QUICK_SLOT_UPDATE , (WPARAM)0 , (LPARAM)iItemID );	// AJJIYA [10/25/2005]

	return true;
}


bool SPPlayerInvenArchive::RunCoolTime(TRASH_ITEM item)
{
	CONTAINER_TYPE iContainer = item.iContainerSlot.ContainerType;
	int iSlotIndex = item.iContainerSlot.SlotIndex;

	if(iContainer < INVENTORY_EQUIP || iContainer >= CONTAINER_MAX)
		return false;

	if(iSlotIndex < 0 || iSlotIndex >= GetInventorySize(iContainer))
		return false;

	SPItem* pUseItem = GetInventoryItem(iContainer, iSlotIndex);
	if(pUseItem == NULL)
		return false;

	int iItemID = pUseItem->GetItemStatusValue(MV_ITEM_GETUID);
	if(iItemID == 0)
		return false;
	
	
	//ÄðÅ¸ÀÓ ·±
	//[2005/10/12] ÆÐÅ¶ ¹ÞÀºµÚ Ã³¸®ÇÒ°Í Running Cool Tiem Process
	{
		COOLTIME_INFO CoolTimeInfo;
		if(pUseItem->GetItemAttr()->m_iCoolTimeApplyID1 > 0) {			
			CoolTimeInfo.fAccmulateTime = 0.0f;
			CoolTimeInfo.fCoolTime = (pUseItem->GetItemAttr()->m_iCoolTime1 * 0.1f);
			SPCoolTimeManager::GetInstance()->AddCoolTime(pUseItem->GetItemAttr()->m_iCoolTimeApplyID1, CoolTimeInfo);
		}

		if(pUseItem->GetItemAttr()->m_iCoolTimeApplyID2 > 0) {
			CoolTimeInfo.fAccmulateTime = 0.0f;
			CoolTimeInfo.fCoolTime = (pUseItem->GetItemAttr()->m_iCoolTime2 * 0.1f);
			SPCoolTimeManager::GetInstance()->AddCoolTime(pUseItem->GetItemAttr()->m_iCoolTimeApplyID2, CoolTimeInfo);
		}
	}

	return true;
}


void SPPlayerInvenArchive::ClearOutSideControl()
{
	//¾ÆÀÌÅÛ ÆÇ¸ÅÃ¢ ÃÊ±âÈ­
	//g_pInterfaceManager->GetItemSellWindow()->SPSendMessage(SPIM_SET_ITEMSELL_RESET);
	
	//¸ÞÀÏ¸®½ºÆ® ¾ÆÀÌÅÛ º¸³»±â Ã¢ ÃÊ±âÈ­
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MAILLIST);
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_MAIL_LIST_SENDITEM_RESET, (WPARAM)0, (LPARAM)0);
	}
}


void SPPlayerInvenArchive::ReflashFigure()
{
	SPPlayer* pPlayer = (SPPlayer*)m_pParent;

	int iContainer = FIGURE_EQUIP;
	int iSlotIndex = 0;
	std::vector<SPItem*>::iterator iter;		
	CONTAINER_ITEM Item;
	bool bItemAvailable = true;

	for(; iContainer < FIGURE_BASIC; iContainer++) {
		iter = m_vpInventory[iContainer].begin();
		for(; iter != m_vpInventory[iContainer].end(); ++iter) {
			if((*iter) == NULL || (*iter)->GetItemStatus() == NULL || (*iter)->GetItemStatus()->GetItemID() == 0)
				continue;

			bItemAvailable = (*iter)->GetItemStatus()->IsTimeAvailable();
			(*iter)->ConvertContainerItem(&Item);
			CONTAINER_TYPE ContainerType = Item.SlotInfo.ContainerType;
			
			if(ContainerType > INVENTORY_HOUSING && ContainerType < STORAGE) {				//ÀåºñÃ¢
				if(ContainerType == FIGURE_EQUIP) {
					int iSlotIndex = Item.SlotInfo.SlotIndex;

					if(iSlotIndex == EQ_WEAPON1 - 1 || iSlotIndex == EQ_BUCKLER1 - 1)
					{
						//[2005/10/20] ±âÁ¸ÀÇ Àåºñ¸¦ ¸ÕÀú »«´Ù
						int iSlotCount = GetSlotCount(Item.SlotInfo.ContainerType, Item.SlotInfo.SlotIndex);
						if(m_iCurrentWeaponSlot == 1) {
							if(iSlotIndex == EQ_WEAPON1 - 1) {
								if(/*bSetEquip &&*/ bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_WEAPON1);
								}
							}
							else if(iSlotIndex == EQ_BUCKLER1 - 1) {
								if(/*bSetEquip && */bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_BUCKLER1);
								}
							}
						}						

						if(m_iCurrentWeaponSlot == 1) {
							if(/*bSetEquip && */bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {						
								m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&Item);
							}
						}
					}
					else if(iSlotIndex == EQ_WEAPON2-1 || iSlotIndex == EQ_BUCKLER2-1)
					{
						//[2005/10/20] ±âÁ¸ÀÇ Àåºñ¸¦ ¸ÕÀú »«´Ù
						int iSlotCount = GetSlotCount(Item.SlotInfo.ContainerType, Item.SlotInfo.SlotIndex);
						if(m_iCurrentWeaponSlot != 1) {
							if(iSlotIndex == EQ_WEAPON2 - 1) {					
								if(/*bSetEquip && */bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_WEAPON1);
								}								
							}
							else if(iSlotIndex == EQ_BUCKLER2 - 1) {
								if(/*bSetEquip && */bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)EQ_BUCKLER1);
								}
							}
						}						

						if(m_iCurrentWeaponSlot == 2) {
							if(/*bSetEquip && */bItemAvailable &&  pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {						
								m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&Item);
							}
						}
					}			
					else
					{
						bool bFashionExist = IsItemExist(iSlotIndex, FIGURE_FASHION);
						bool bOnePieceExist = false;
						bool bFashionAvailable = true;

						//ÆÐ¼Ç ÇÑ¹ú¿Ê°ú ½½·ÔÀÌ °ãÄ¡´Â °æ¿ì
						if(!bFashionExist && iSlotIndex == 4 || iSlotIndex == 5){
							if(IsItemExist(3, FIGURE_FASHION)) {
								SPItem* pOnePiece = GetInventoryItem(FIGURE_FASHION, 3);
								bFashionAvailable = pOnePiece->GetItemStatus()->IsTimeAvailable();
								int iItemID = pOnePiece->GetItemStatusValue(MV_ITEM_GETUID);
								int iSlotCount = GetSlotCount(FIGURE_FASHION, 3);
								if(iSlotCount == 3)
									bOnePieceExist = true;
							}					
						}

						if((bFashionExist && bFashionAvailable) || bOnePieceExist){
						}				
						else {
							if(/*bSetEquip && */bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
								m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&Item);
							}
						}
					}
				}
				else if(ContainerType == FIGURE_FASHION) {					
					int iCurIndex = Item.SlotInfo.SlotIndex;
					
					//ÇÑ¹ú¿ÊÀÎ °æ¿ì ¾ÆÀÌÄÜ ¼ÂÆÃ...
					if(Item.SlotInfo.SlotIndex == 3) {
						int iSlotCount = GetSlotCount(Item.SlotInfo.ContainerType, Item.SlotInfo.SlotIndex);						
						if(iSlotCount == 3){
							Item.SlotInfo.SlotIndex++;							

							if(IsItemExist(Item.SlotInfo.SlotIndex, FIGURE_EQUIP)) {
								if(/*bSetEquip &&*/bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)Item.SlotInfo.SlotIndex + 1);
								}
							}

							Item.SlotInfo.SlotIndex++;
							if(IsItemExist(Item.SlotInfo.SlotIndex, FIGURE_EQUIP)) {
								if(/*bSetEquip &&*/ bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)Item.SlotInfo.SlotIndex + 1);
								}
							}
						}
						else {				//[2005/7/6] ÇÑ¹ú¿ÊÀÌ ¾Æ´Ñ°æ¿ì
							bool bFashionExist = false;
							bool bEquipExist = false;
							bool bTimeAvailable = true;

							Item.SlotInfo.SlotIndex++;
							bFashionExist = IsItemExist(Item.SlotInfo.SlotIndex, FIGURE_FASHION);
							bEquipExist = IsItemExist(Item.SlotInfo.SlotIndex, FIGURE_EQUIP);					

							if(bFashionExist)
								bTimeAvailable = GetInventoryItem(FIGURE_FASHION, Item.SlotInfo.SlotIndex)->GetItemStatus()->IsTimeAvailable();

							if(bFashionExist == false || bTimeAvailable == false) {
								if(bEquipExist) {
									int iEquipItemID = m_vpInventory[FIGURE_EQUIP].at(Item.SlotInfo.SlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
									if(iEquipItemID!=0)
									{
										//Item ¸¸µé±â
										CONTAINER_ITEM item;
										item.Item.iItemID = iEquipItemID;
										item.Item.iStackCount = 1;
										if(/*bSetEquip &&*/ pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
											m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
										}
									}
								}
							}

							Item.SlotInfo.SlotIndex++;
							bFashionExist = IsItemExist(Item.SlotInfo.SlotIndex, FIGURE_FASHION);
							bEquipExist = IsItemExist(Item.SlotInfo.SlotIndex, FIGURE_EQUIP);
							if(bFashionExist)
								bTimeAvailable = GetInventoryItem(FIGURE_FASHION, Item.SlotInfo.SlotIndex)->GetItemStatus()->IsTimeAvailable();

							if(bFashionExist == false || bTimeAvailable == false) {
								if(bEquipExist) {
									int iEquipItemID = m_vpInventory[FIGURE_EQUIP].at(Item.SlotInfo.SlotIndex)->GetItemStatusValue(MV_ITEM_GETUID);
									if(iEquipItemID!=0)
									{
										//Item ¸¸µé±â
										CONTAINER_ITEM item;
										item.Item.iItemID = iEquipItemID;
										item.Item.iStackCount = 1;
										if(/*bSetEquip &&*/ pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
											m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&item);
										}
									}
								}
							}
						}
					}
					else if(Item.SlotInfo.SlotIndex == 4 || Item.SlotInfo.SlotIndex == 5) {
						SPItem* pOnePiece = GetInventoryItem(FIGURE_FASHION, 3);
						
						bool bTimeAvailable = pOnePiece->GetItemStatus()->IsTimeAvailable();

						int iItemID = pOnePiece->GetItemStatusValue(MV_ITEM_GETUID);
						int iSlotCount = GetSlotCount(FIGURE_FASHION, 3);
						bool bOnePieceExist = false;
						if(iItemID != 0) {
							if(iSlotCount == 3)
								bOnePieceExist = true;
						}
						
						if(bOnePieceExist && bTimeAvailable) {
							int iPrevSlotIndex = 0;
							if(bOnePieceExist && Item.SlotInfo.SlotIndex == 4) {
								iPrevSlotIndex = Item.SlotInfo.SlotIndex - 1;								

								if(/*bSetEquip &&*/ pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)iPrevSlotIndex + 1);
								}

								iPrevSlotIndex = Item.SlotInfo.SlotIndex + 1;
								if(/*bSetEquip &&*/ pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)iPrevSlotIndex + 1);
								}
							}
							if(bOnePieceExist && Item.SlotInfo.SlotIndex == 5) {
								iPrevSlotIndex = 3;
								if(/*bSetEquip &&*/ pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)iPrevSlotIndex + 1);
								}

								iPrevSlotIndex = 4;
								if(/*bSetEquip &&*/ pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
									m_pParent->SPGOBPerformMessage(SPGM_EQUIP_UNLOAD, (LPARAM)iPrevSlotIndex + 1);
								}
							}
						}				
					}

					Item.SlotInfo.SlotIndex = iCurIndex;
					if(/*bSetEquip &&*/ bItemAvailable && pPlayer->IsLocalPlayer() && !pPlayer->IsDead()) {
						m_pParent->SPGOBPerformMessage(SPGM_EQUIP_CHANGE, (LPARAM)&Item);
					}
				}
			}
			
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void SPPlayerInvenArchive::ClearAllTradeItem()
{
	for(int i = 0; i < MAX_PC_TRADE_SLOT; i++) {		
		if(m_TradeItem[i].m_iContainer == m_iCurrentInventoryType) {	//¹ÝÅõ¸í »óÅÂ È¸º¹
			int iTradeIndex = m_TradeItem[i].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)false, NULL);
		}
		m_TradeItem[i].Clear();
	}
}


bool SPPlayerInvenArchive::ClearTradeItem(int iIndex /*= -1*/)
{
	if(iIndex < 0 || iIndex >= MAX_PC_TRADE_SLOT) 
		return false;

	if(m_iCurrentInventoryType == m_TradeItem[iIndex].m_iContainer) {
		int iTradeIndex = m_TradeItem[iIndex].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)false, NULL);
	}
	
	m_TradeItem[iIndex].Clear();	

	return true;
}


bool SPPlayerInvenArchive::IsTradeEnable(CONTAINER_TYPE iType /*= CONTAINER_TYPE_INVALID*/, int iSlotIndex /*= -1*/)
{	
	if(iType == CONTAINER_TYPE_INVALID && iSlotIndex == -1) {	//ÀÎµ¦½º¸¸ ³Ñ¾î¿Â °æ¿ì ÇÈ¾÷ ¾ÆÀÌÅÛÀ¸·Î ¼³Á¤
		iType = m_PickUpItem.m_iContainer;
		iSlotIndex = m_PickUpItem.m_iSlotIndex;
	}

	if(iType < INVENTORY_EQUIP || iType > INVENTORY_HOUSING) {
		return false;
	}

	if(IsItemExist(iSlotIndex, iType) == false) {
		return false;
	}

	//Æ®·¹ÀÌµå °¡´ÉÇÑ ¾ÆÀÌÅÛÀÎÁö?
	SPItem* pTradeItem = GetInventoryItem(iType, iSlotIndex);
	if(pTradeItem->GetItemAttr()->m_bTrade == false) {
		return false;
	}
	
	//½Ã°£Á¦ ¾ÆÀÌÅÛÀÎ °æ¿ì
	if(pTradeItem->GetItemAttr()->GetDurationType() > ITEM_DURATION_NULL) {
		return false;
	}

	return true;
}


bool SPPlayerInvenArchive::SetTradeItem(int iIndex, CONTAINER_TYPE iType /*= CONTAINER_TYPE_INVALID*/, int iSlotIndex /*= -1*/)
{	
	if(iIndex < 0 || iIndex >= MAX_PC_TRADE_SLOT) 
		return false;	
	
	if(IsTradeEnable(iType, iSlotIndex) == false)
		return false;	

	m_TradeItem[iIndex].SetSelectItem(iType, iSlotIndex);

	RecoverIcon();
	
	if(m_iCurrentInventoryType == m_TradeItem[iIndex].m_iContainer) {
		int iTradeIndex = m_TradeItem[iIndex].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)true, NULL);	
	}
	
	return true;
}


SPSelectItem* SPPlayerInvenArchive::GetTradeItem(int iIndex)
{
	if(iIndex < 0 || iIndex >= MAX_PC_TRADE_SLOT) 
		return NULL;

	return &m_TradeItem[iIndex];
}


int SPPlayerInvenArchive::IsTradeItem(CONTAINER_TYPE iType, int iSlotIndex)
{
	for(int i = 0 ; i < MAX_PC_TRADE_SLOT ; i++) {
		if(iType == m_TradeItem[i].m_iContainer && iSlotIndex == m_TradeItem[i].m_iSlotIndex)
			return i;
	}

	return -1;
}


//////////////////////////////////////////////////////////////////////////
void SPPlayerInvenArchive::ClearAllUserShopItem()
{
	for(int i = 0; i < FM_MAX_SELLITEM; i++) {		
		if(m_UserShopItem[i].m_iContainer == m_iCurrentInventoryType) {	//¹ÝÅõ¸í »óÅÂ È¸º¹
			int iTradeIndex = m_UserShopItem[i].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)false, NULL);
		}
		m_UserShopItem[i].Clear();
	}
}


bool SPPlayerInvenArchive::ClearUserShopItem(int iIndex /*= -1*/)
{
	if(iIndex < 0 || iIndex >= FM_MAX_SELLITEM) 
		return false;

	if(m_iCurrentInventoryType == m_UserShopItem[iIndex].m_iContainer) {
		int iTradeIndex = m_UserShopItem[iIndex].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)false, NULL);
	}

	m_UserShopItem[iIndex].Clear();	

	return true;
}


bool SPPlayerInvenArchive::SetUserShopItem(int iIndex, CONTAINER_TYPE iType /*= CONTAINER_TYPE_INVALID*/, int iSlotIndex /*= -1*/)
{
	if(iIndex < 0 || iIndex >= FM_MAX_SELLITEM) 
		return false;	

	if(IsTradeEnable(iType, iSlotIndex) == false)
		return false;	

	m_UserShopItem[iIndex].SetSelectItem(iType, iSlotIndex);

	RecoverIcon();

	if(m_iCurrentInventoryType == m_UserShopItem[iIndex].m_iContainer) {
		int iTradeIndex = m_UserShopItem[iIndex].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)true, NULL);	
	}

	return true;
}


bool SPPlayerInvenArchive::SetUserShopItemFake(int iIndex, CONTAINER_TYPE iType /*= CONTAINER_TYPE_INVALID*/, int iSlotIndex /*= -1*/)
{
	return false;			//Use SetBlockItem
	
	if(iIndex < 0 || iIndex >= FM_MAX_SELLITEM) 
		return false;		
	
	m_UserShopItem[iIndex].SetSelectItem(iType, iSlotIndex);

	RecoverIcon();

	if(m_iCurrentInventoryType == m_UserShopItem[iIndex].m_iContainer) {
		int iTradeIndex = m_UserShopItem[iIndex].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)true, NULL);	
	}

	return true;
}


SPSelectItem* SPPlayerInvenArchive::GetUserShopItem(int iIndex)
{
	if(iIndex < 0 || iIndex >= FM_MAX_SELLITEM) 
		return NULL;

	return &m_UserShopItem[iIndex];
}


int	SPPlayerInvenArchive::IsUserShopItem(CONTAINER_TYPE iType, int iSlotIndex)
{
	for(int i = 0 ; i < FM_MAX_SELLITEM ; i++) {
		if(iType == m_UserShopItem[i].m_iContainer && iSlotIndex == m_UserShopItem[i].m_iSlotIndex)
			return i;
	}

	return -1;
}
//--------------------------------------------------
// pet shop codes
//--------------------------------------------------
void SPPlayerInvenArchive::ClearAllPetShopItem( PET_PLACE iPlace )
{
	SPSelectItem	*pPetShopItems = &m_PetShopItem[iPlace][0];
	for(int i = 0; i < FM_MAX_SELLITEM; i++) {		
		if(pPetShopItems[i].m_iContainer == m_iCurrentInventoryType) {	//¹ÝÅõ¸í »óÅÂ È¸º¹
			int iTradeIndex = pPetShopItems[i].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)false, NULL);
		}
		pPetShopItems[i].Clear();
	}
}
bool SPPlayerInvenArchive::ClearPetShopItem(PET_PLACE iPlace, int iIndex /* = -1  */)
{
	if (iIndex < 0 || iIndex >= FM_MAX_SELLITEM)
		return false;
	SPSelectItem	*pPetShopItems = &m_PetShopItem[iPlace][0];

	if (m_iCurrentInventoryType == pPetShopItems[iIndex].m_iContainer)
	{
		int iTradeIndex = pPetShopItems[iIndex].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE );
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage( SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)false, NULL );
	}
	pPetShopItems[iIndex].Clear();
	return true;
}
bool SPPlayerInvenArchive::SetPetShopItem(PET_PLACE iPlace, int iIndex, 
										  CONTAINER_TYPE iType, int iSlotIndex /* = -1 */)
{
	if (iIndex < 0 || iIndex >= FM_MAX_SELLITEM)
		return false;

	if (IsTradeEnable(iType, iSlotIndex) == false)
		return false;

	SPSelectItem	*pPetShopItems = &m_PetShopItem[iPlace][0];
	pPetShopItems[iIndex].SetSelectItem( iType, iSlotIndex );

	RecoverIcon();

	if(m_iCurrentInventoryType == pPetShopItems[iIndex].m_iContainer) {
		int iTradeIndex = pPetShopItems[iIndex].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)true, NULL);	
	}

	return true;
}
bool SPPlayerInvenArchive::SetPetShopItemFake(PET_PLACE iPlace, int iIndex, 
											  CONTAINER_TYPE iType, int iSlotIndex /* = -1  */)
{
	return false;
}
SPSelectItem *SPPlayerInvenArchive::GetPetShopItem(PET_PLACE iPlace, int iIndex)
{
	if (iIndex < 0 || iIndex >= FM_MAX_SELLITEM)
		return NULL;

	return &m_PetShopItem[iPlace][iIndex];
}
int SPPlayerInvenArchive::IsPetShopItem(CONTAINER_TYPE iType, int iSlotIndex)
{
	for (int iPlace = 0; iPlace < PET_PLACE_TYPE_NUMBER; ++iPlace)
	{
		SPSelectItem	*pPetShopItems = &m_PetShopItem[iPlace][0];
		for (int i = 0; i < FM_MAX_SELLITEM; ++i)
		{
			if (iType == pPetShopItems[i].m_iContainer
				&&
				iSlotIndex == pPetShopItems[i].m_iSlotIndex)
				return i;
		}
	}

	return -1;
}
//--------------------------------------------------

bool SPPlayerInvenArchive::SetBlockItem(int iIndex, CONTAINER_TYPE iType /*= CONTAINER_TYPE_INVALID*/, int iSlotIndex /*= -1*/)
{
	if(iIndex < 0 || iIndex >= MAX_ITEM_BLOCK) 
		return false;		

	m_BlockItem[iIndex].SetSelectItem(iType, iSlotIndex);

	RecoverIcon();

	if(m_iCurrentInventoryType == m_BlockItem[iIndex].m_iContainer) {
		int iTradeIndex = m_BlockItem[iIndex].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)true, NULL);	
	}

	return true;
}


void SPPlayerInvenArchive::ClearAllBlockItem()
{
	for(int i = 0; i < MAX_ITEM_BLOCK; i++) {		
		if(m_BlockItem[i].m_iContainer == m_iCurrentInventoryType) {	//¹ÝÅõ¸í »óÅÂ È¸º¹
			int iTradeIndex = m_BlockItem[i].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
			g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)false, NULL);
		}
		m_BlockItem[i].Clear();
	}
}


bool SPPlayerInvenArchive::ClearBlockItem(int iIndex /*= -1*/)
{
	if(iIndex < 0 || iIndex >= MAX_ITEM_BLOCK) 
		return false;

	if(m_iCurrentInventoryType == m_BlockItem[iIndex].m_iContainer) {
		int iTradeIndex = m_BlockItem[iIndex].m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iTradeIndex, (LPARAM)false, NULL);
	}

	m_BlockItem[iIndex].Clear();	

	return true;
}


SPSelectItem* SPPlayerInvenArchive::GetBlockItem(int iIndex)
{
	if(iIndex < 0 || iIndex >= MAX_ITEM_BLOCK) 
		return NULL;

	return &m_BlockItem[iIndex];
}


int	SPPlayerInvenArchive::IsBlockItem(CONTAINER_TYPE iType, int iSlotIndex)
{
	for(int i = 0 ; i < MAX_ITEM_BLOCK ; i++) {
		if(iType == m_BlockItem[i].m_iContainer && iSlotIndex == m_BlockItem[i].m_iSlotIndex)
			return i;
	}

	return -1;
}


bool SPPlayerInvenArchive::SetPetFeedItem(PET_PLACE iPlace, CONTAINER_TYPE iType, int iSlotIndex)
{
	if(iType != INVENTORY_ETC)
		return false;

	SPSelectItem &petFeed = m_vecPetFeeds[iPlace].feedNormal;

	petFeed.SetSelectItem(iType, iSlotIndex);

	if(m_PickUpItem.m_iContainer != STORAGE)
		RecoverIcon();

	if(m_iCurrentInventoryType == petFeed.m_iContainer) {
		int iPetFeedIndex = petFeed.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iPetFeedIndex, (LPARAM)true, NULL);
	}

	return true;
}


void SPPlayerInvenArchive::ClearPetFeedItem( PET_PLACE iPlace )
{
	SPSelectItem &petFeed = m_vecPetFeeds[iPlace].feedNormal;
	if(m_iCurrentInventoryType == petFeed.m_iContainer) {
		int iPetFeedIndex = petFeed.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iPetFeedIndex, (LPARAM)false, NULL);
	}

	petFeed.Clear();	
}

void SPPlayerInvenArchive::ClearPetFeedAll( void )
{
	;
}

bool SPPlayerInvenArchive::SetPetFeedEnergyItem(PET_PLACE iPlace, CONTAINER_TYPE iType, int iSlotIndex )
{
	SPSelectItem &petFeed = m_vecPetFeeds[iPlace].feedEnergy;

	petFeed.SetSelectItem(iType, iSlotIndex);

	if(m_PickUpItem.m_iContainer != STORAGE)
		RecoverIcon();

	if(m_iCurrentInventoryType == petFeed.m_iContainer) {
		int iPetFeedIndex = petFeed.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iPetFeedIndex, (LPARAM)true, NULL);
	}

	return true;
}
void SPPlayerInvenArchive::ClearPetFeedEnergyItem(PET_PLACE iPlace)
{
	SPSelectItem &petFeed = m_vecPetFeeds[iPlace].feedEnergy;
	if(m_iCurrentInventoryType == petFeed.m_iContainer) {
		int iPetFeedIndex = petFeed.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iPetFeedIndex, (LPARAM)false, NULL);
	}

	petFeed.Clear();	
}
void SPPlayerInvenArchive::ClearPetFeedEnergyAll( void )
{
	;
}

bool SPPlayerInvenArchive::IsPetFeedItem(CONTAINER_TYPE iType, int iSlotIndex)
{
	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		SPSelectItem &petFeed = m_vecPetFeeds[idx].feedNormal;
		if(petFeed.IsNull() == false) {
			if(iType == petFeed.m_iContainer && iSlotIndex == petFeed.m_iSlotIndex)
				return true;
		}
	}

	return false;
}
bool SPPlayerInvenArchive::IsPetFeedEnergyItem(CONTAINER_TYPE iType, int iSlotIndex)
{
	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		SPSelectItem &petFeed = m_vecPetFeeds[idx].feedEnergy;
		if(petFeed.IsNull() == false) {
			if(iType == petFeed.m_iContainer && iSlotIndex == petFeed.m_iSlotIndex)
				return true;
		}
	}

	return false;
}


bool SPPlayerInvenArchive::SetMicItem(CONTAINER_TYPE iType, int iSlotIndex)					//ÉèÖÃItem;
{
	if(iType != INVENTORY_CONSUME)
		return false;

	if(!m_MicItem.IsNull())
		return false;

	m_MicItem.SetSelectItem(iType, iSlotIndex);

	//RecoverIcon();

	if(m_iCurrentInventoryType == m_MicItem.m_iContainer) {
		int iMicIndex = m_MicItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iMicIndex, (LPARAM)true, NULL);
	}

	return true;
}

bool SPPlayerInvenArchive::SetFindItem( CONTAINER_TYPE iType, int iSlotIndex )
{
	if(iType != INVENTORY_CONSUME)
		return false;

	if(!m_FindItem.IsNull())
		return false;

	m_FindItem.SetSelectItem(iType, iSlotIndex);

	//RecoverIcon();

	if(m_iCurrentInventoryType == m_FindItem.m_iContainer)
	{
		int iFindIndex = m_FindItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iFindIndex, (LPARAM)true, NULL);
	}

	return true;
}

bool SPPlayerInvenArchive::SetFollowItem( CONTAINER_TYPE iType, int iSlotIndex )
{
	if(iType != INVENTORY_CONSUME)
		return false;

	if(!m_FollowItem.IsNull())
		return false;

	m_FollowItem.SetSelectItem(iType, iSlotIndex);

	//RecoverIcon();

	if(m_iCurrentInventoryType == m_FollowItem.m_iContainer)
	{
		int iFindIndex = m_FollowItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iFindIndex, (LPARAM)true, NULL);
	}

	return true;
}

bool SPPlayerInvenArchive::SetSnatchItem( CONTAINER_TYPE iType, int iSlotIndex )
{
	if(iType != INVENTORY_CONSUME)
		return false;

	if(!m_SnatchItem.IsNull())
		return false;

	m_SnatchItem.SetSelectItem(iType, iSlotIndex);

	//RecoverIcon();

	if(m_iCurrentInventoryType == m_SnatchItem.m_iContainer)
	{
		int iFindIndex = m_SnatchItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iFindIndex, (LPARAM)true, NULL);
	}

	return true;
}

void SPPlayerInvenArchive::ClearMicItem()
{
	if(m_iCurrentInventoryType == m_MicItem.m_iContainer) {
		int iMicIndex = m_MicItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iMicIndex, (LPARAM)false, NULL);
	}

	m_MicItem.Clear();	
}

void SPPlayerInvenArchive::ClearFindItem()
{
	if(m_iCurrentInventoryType == m_FindItem.m_iContainer)
	{
		int iFindIndex = m_FindItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iFindIndex, (LPARAM)false, NULL);
	}

	m_FindItem.Clear();	
}

void SPPlayerInvenArchive::ClearFollowItem()
{
	if(m_iCurrentInventoryType == m_FollowItem.m_iContainer)
	{
		int iFindIndex = m_FollowItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iFindIndex, (LPARAM)false, NULL);
	}

	m_FollowItem.Clear();	
}

void SPPlayerInvenArchive::ClearSnatchItem()
{
	if(m_iCurrentInventoryType == m_SnatchItem.m_iContainer)
	{
		int iFindIndex = m_SnatchItem.m_iSlotIndex - (m_iCurInvenPage[m_iCurrentInventoryType] * INVENTORY_LINE);
		g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_ITEM_SELECT, (WPARAM)iFindIndex, (LPARAM)false, NULL);
	}

	m_SnatchItem.Clear();	
}
bool SPPlayerInvenArchive::IsMicItem(CONTAINER_TYPE iType, int iSlotIndex)
{
	if(m_MicItem.IsNull() == false) {
		if(iType == m_MicItem.m_iContainer && iSlotIndex == m_MicItem.m_iSlotIndex)
			return true;
	}

	return false;
}

bool SPPlayerInvenArchive::IsFindItem( CONTAINER_TYPE iType, int iSlotIndex )
{
	if(m_FindItem.IsNull() == false)
	{
		if(iType == m_FindItem.m_iContainer && iSlotIndex == m_FindItem.m_iSlotIndex)
			return true;
	}

	return false;
}

bool SPPlayerInvenArchive::IsFollowItem( CONTAINER_TYPE iType, int iSlotIndex )
{
	if(m_FollowItem.IsNull() == false)
	{
		if(iType == m_FollowItem.m_iContainer && iSlotIndex == m_FollowItem.m_iSlotIndex)
			return true;
	}

	return false;
}

bool SPPlayerInvenArchive::IsSnatchItem( CONTAINER_TYPE iType, int iSlotIndex )
{
	if(m_SnatchItem.IsNull() == false)
	{
		if(iType == m_SnatchItem.m_iContainer && iSlotIndex == m_SnatchItem.m_iSlotIndex)
			return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////
NOTICE_USE SPPlayerInvenArchive::GetCurNoticeUse()
{
	return m_iNoticeUse;
}


void SPPlayerInvenArchive::SetNoticeUse(NOTICE_USE iNoticeUse)
{
	m_iNoticeUse = iNoticeUse;
}

int SPPlayerInvenArchive::GetMaxInvenPage(int iType)
{
	return m_iInvenPage[iType];
}


////////////////////////////////////////////////////////////////////////// PC TRADE Message
bool SPPlayerInvenArchive::SendTradeResponse(bool bAccept /*= false*/)
{
	if(g_pCheckManager->IsDBBlock()) {
		
		DXUTOutputDebugString("\tSPPlayerInvenArchive::SendTradeResponse DB Working Block\n");
		return false;
	}
	
	CPacket Packet(TRADE_CS_RESPONSE);
	Packet.Add((UINT8)bAccept);	
	g_pNetworkManager->SPPerformMessage(TRADE_CS_RESPONSE, 0, (LPARAM)&Packet);	
	return true;
}


bool SPPlayerInvenArchive::IsSendTradeRequest()
{
	return m_bSendRequest;
}


void SPPlayerInvenArchive::SetSendTradeRequest(bool bSend)
{
	m_bSendRequest = bSend;
}


int	SPPlayerInvenArchive::OnTradeRequest(LPARAM lParam)
{	
	CPacket* pPacket = (CPacket*)lParam;
	
	GU_ID			SrcCharGUID;				// °Å·¡¿äÃ»ÀÚ
	char			szSrcCharName[LEN_NAME+1];	// °Å·¡¿äÃ»ÀÚÀÌ¸§
	GU_ID			DstCharGUID;				// °Å·¡´ë»óÀÚ
	char			szDstCharName[LEN_NAME+1];	// °Å·¡´ë»óÀÚÀÌ¸§
	GLOBAL_STRING_ID	iErrorID;				// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
	ZeroMemory(szSrcCharName, LEN_NAME+1);
	ZeroMemory(szDstCharName, LEN_NAME+1);	
	
	pPacket->ExtractUINT64(&SrcCharGUID);
	pPacket->ExtractData((TCHAR*)&szSrcCharName , sizeof(TCHAR) * LEN_NAME+1);
	pPacket->ExtractUINT64(&DstCharGUID);
	pPacket->ExtractData((TCHAR*)&szDstCharName , sizeof(TCHAR) * LEN_NAME+1);
	pPacket->ExtractUINT32(&iErrorID);

	char szBuf[32];
	char szMsg[512];
	ZeroMemory(szBuf, 32);
	ZeroMemory(szMsg, 512);
	
	//
	if(m_pParent->IsLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() == SrcCharGUID) {			//¿äÃ»ÀÚ ÀÚ½ÅÀÎ °æ¿ì
			if(iErrorID == 0) {							// Request OK
				if(g_pResourceManager->GetGlobalString(4001010)) {
					std::string strMsg = g_pResourceManager->GetGlobalString(4001010);	//Msg %s´Ô¿¡°Ô °Å·¡¸¦ ½ÅÃ» Çß½À´Ï´Ù.					
					wsprintf(szMsg, strMsg.c_str(), szDstCharName);
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
				}
				g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_REQUEST);
			}
			else {
				if(g_pResourceManager->GetGlobalString(iErrorID)) {
					std::string strMsg = g_pResourceManager->GetGlobalString(iErrorID);	//Msg 4001030 »ó´ë¹æ°ú Áö±Ý °Å·¡ÇÒ¼ö ¾ø½À´Ï´Ù.
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
				}				
			}
		}
		else if(g_pGOBManager->GetLocalPlayer()->GetGUID() == DstCharGUID) {	//¿äÃ» ¹ÞÀº °æ¿ì
			if(iErrorID == 0) {							// Request OK
				//[2006/2/20]	³»°¡ ÄÆÀÎÁßÀÎ°æ¿ì
				if(g_pEventManager->GetCutInState() > CUT_IN_STATE_NULL) {
					SendTradeResponse(false);
					return 1;
				}

				//[2008/4/29] - ³»°¡ Ä³½Ã °Å·¡ ÀÀ´ä ´ë±âÁßÀÎ °æ¿ì
				if(SPCashTradeManager::GetInstance()->GetCashTradeState() == CASH_TRADE_STATE_WAIT) {
					SendTradeResponse(false);
					return 1;
				}
				
				if(g_pResourceManager->GetGlobalString(4001000)) {
					std::string strMsg = g_pResourceManager->GetGlobalString(4001000);	//Msg %s´ÔÀÇ °Å·¡¸¦ ½Â³« ÇÏ½Ã°Ú½À´Ï±î?
					wsprintf(szMsg, strMsg.c_str(), szSrcCharName);				
				}
				g_pInterfaceManager->SetNotice(szMsg, this->m_pParent);
				m_bRequested = true;
				m_iNoticeUse = NOTICE_USE_TRADE_REQUEST;
				g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_RESPONSE, SrcCharGUID);
			}
			else {
				if(g_pResourceManager->GetGlobalString(iErrorID)) {
					std::string strMsg = g_pResourceManager->GetGlobalString(iErrorID);	//Msg 4001030 »ó´ë¹æ°ú Áö±Ý °Å·¡ÇÒ¼ö ¾ø½À´Ï´Ù.
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
				}				
			}
		}
		else {																	//°Å·¡¿Í »ó°ü¾ø´Â ID°¡ ³Ñ¾î¿È...		
			//std::string strMsg = g_pResourceManager->GetGlobalString(4000001);	//Msg »ó´ë¹æÀ» Ã£À»¼ö ¾ø½À´Ï´Ù.
			//g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			//g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
		}
	}

	m_bSendRequest = false;			//[2006/3/6] Æ®·¹ÀÌµå ¿äÃ»À» ´Ù½Ã ÇÒ¼ö ÀÖµµ·Ï Ç®¾îÁØ´Ù
	
	return 1;
}


int	SPPlayerInvenArchive::OnTradeResponse(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	
	GU_ID			SrcCharGUID;				// °Å·¡¿äÃ»ÀÚ
	char			szSrcCharName[LEN_NAME+1];	// °Å·¡¿äÃ»ÀÚÀÌ¸§
	GU_ID			DstCharGUID;				// °Å·¡´ë»óÀÚ
	char			szDstCharName[LEN_NAME+1];	// °Å·¡´ë»óÀÚÀÌ¸§
	UINT8			byAccept;					// ¼ö¶ô/°ÅÀý
	GLOBAL_STRING_ID	iErrorID;				// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
	ZeroMemory(szSrcCharName, LEN_NAME+1);
	ZeroMemory(szDstCharName, LEN_NAME+1);
	
	pPacket->ExtractUINT64(&SrcCharGUID);
	pPacket->ExtractData((TCHAR*)&szSrcCharName , sizeof(TCHAR) * LEN_NAME+1);
	pPacket->ExtractUINT64(&DstCharGUID);
	pPacket->ExtractData((TCHAR*)&szDstCharName , sizeof(TCHAR) * LEN_NAME+1);
	pPacket->ExtractUINT8(&byAccept);
	pPacket->ExtractUINT32(&iErrorID);

	char szName[32];
	char szMsg[512];
	ZeroMemory(szName, 32);
	ZeroMemory(szMsg, 512);
	
	//
	if(m_pParent->IsLocalPlayer()) {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() == SrcCharGUID) {	//¿äÃ»ÀÚ ÀÚ½ÅÀÎ °æ¿ì
			if(byAccept) {				//¼ö¶ôÀÇ °æ¿ì
				if(iErrorID == 0) {		//Response OK...
					if(g_pResourceManager->GetGlobalString(4001011)) {
						std::string strMsg = g_pResourceManager->GetGlobalString(4001011);	//Msg %s´ÔÀÌ °Å·¡¸¦ ½Â¶ôÇß½À´Ï´Ù.
						wsprintf(szMsg, strMsg.c_str(), szDstCharName);
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
					}

					//°Å·¡ ½ÃÀÛ				
					g_pInterfaceManager->GetPcTradeWindow()->SPSendMessage(SPIM_SET_PLAYER_INFO, (WPARAM)&DstCharGUID);					
					
					g_pInterfaceManager->ShowWindow(WIID_CHANNEL_CHANGE, SP_HIDE);
					g_pInterfaceManager->ShowWindow(WIID_EXIT, SP_HIDE);
					g_pInterfaceManager->SetShowDefaultWindow(true);
					
					g_pInterfaceManager->ShowWindow(WIID_PC_TRADE, SP_SHOW);
					g_pInterfaceManager->GetInvenWindow()->Show();				
					g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_SET_EXIT_SHOW, 0, (LPARAM)false);	
					g_pInterfaceManager->GetEquipWindow()->Show();
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_SET_EXIT_SHOW, 0, (LPARAM)false);				

					g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_TRADE, DstCharGUID);
				}
				else {
					if(g_pResourceManager->GetGlobalString(iErrorID)) {
						std::string strMsg = g_pResourceManager->GetGlobalString(iErrorID);	//Msg 4001030 »ó´ë¹æ°ú Áö±Ý °Å·¡ÇÒ¼ö ¾ø½À´Ï´Ù.
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
					}					
					g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
				}
			}
			else {						//°ÅºÎÀÇ °æ¿ì
				if(iErrorID == 0) {
					if(g_pResourceManager->GetGlobalString(4001012)) {
						std::string strMsg = g_pResourceManager->GetGlobalString(4001012);	//Msg %s´ÔÀÌ °Å·¡¸¦ Ãë¼ÒÇß½À´Ï´Ù.
						wsprintf(szMsg, strMsg.c_str(), szDstCharName);
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
					}					
				}
				else {
					if(g_pResourceManager->GetGlobalString(iErrorID)) {
						std::string strMsg = g_pResourceManager->GetGlobalString(iErrorID);	//Msg 4001030 »ó´ë¹æ°ú Áö±Ý °Å·¡ÇÒ¼ö ¾ø½À´Ï´Ù.
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
					}					
					g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
				}

				g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
			}
		}
		else if(g_pGOBManager->GetLocalPlayer()->GetGUID() == DstCharGUID) { //¿äÃ» ¹ÞÀº °æ¿ì
			if(byAccept) {				//¼ö¶ôÀÇ °æ¿ì
				if(iErrorID == 0) {		//Response OK...
					if(g_pResourceManager->GetGlobalString(4001001)) {
						std::string strMsg = g_pResourceManager->GetGlobalString(4001001);	//Msg %s´ÔÀÇ °Å·¡¸¦ ½Â³«Çß½À´Ï´Ù.
						wsprintf(szMsg, strMsg.c_str(), szSrcCharName);
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
					}					

					//°Å·¡ ½ÃÀÛ
					g_pInterfaceManager->GetPcTradeWindow()->SPSendMessage(SPIM_SET_PLAYER_INFO, (WPARAM)&SrcCharGUID);					
					
					g_pInterfaceManager->ShowWindow(WIID_CHANNEL_CHANGE, SP_HIDE);
					g_pInterfaceManager->ShowWindow(WIID_EXIT, SP_HIDE);			

					g_pInterfaceManager->SetShowDefaultWindow(true);
					
					g_pInterfaceManager->ShowWindow(WIID_PC_TRADE, SP_SHOW);
					g_pInterfaceManager->GetInvenWindow()->Show();
					g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_SET_EXIT_SHOW, 0, (LPARAM)false);	
					g_pInterfaceManager->GetEquipWindow()->Show();
					g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_SET_EXIT_SHOW, 0, (LPARAM)false);	

					g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_TRADE, SrcCharGUID);
				}
				else {
					std::string strMsg = g_pResourceManager->GetGlobalString(iErrorID);	//Msg 4001030 »ó´ë¹æ°ú Áö±Ý °Å·¡ÇÒ¼ö ¾ø½À´Ï´Ù.
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
					g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
				}
			}
			else {						//°ÅºÎÀÇ °æ¿ì
				if(iErrorID == 0) {
					if(g_pResourceManager->GetGlobalString(4001002)) {
						std::string strMsg = g_pResourceManager->GetGlobalString(4001002);	//Msg %s´Ô°ú °Å·¡°¡ Ãë¼Ò µÇ¾ú½À´Ï´Ù.
						wsprintf(szMsg, strMsg.c_str(), szSrcCharName);
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
					}					
				}
				else {
					if(g_pResourceManager->GetGlobalString(iErrorID)) {
						std::string strMsg = g_pResourceManager->GetGlobalString(iErrorID);	//Msg 4001030 »ó´ë¹æ°ú Áö±Ý °Å·¡ÇÒ¼ö ¾ø½À´Ï´Ù.
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
					}					
					g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
				}
				g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
			}
		}
	}	

	return 1;
}


int SPPlayerInvenArchive::OnTradePushMoney(LPARAM lParam)
{
	g_pInterfaceManager->GetPcTradeWindow()->SPSendMessage(SPIM_TRADE_PUSHMONEY, 0, lParam);
	return 1;
}


int SPPlayerInvenArchive::OnTradePushItem(LPARAM lParam)
{
	g_pInterfaceManager->GetPcTradeWindow()->SPSendMessage(SPIM_TRADE_PUSHITEM, 0, lParam);
	return 1;
}


int SPPlayerInvenArchive::OnTradePopItem(LPARAM lParam)
{	
	g_pInterfaceManager->GetPcTradeWindow()->SPSendMessage(SPIM_TRADE_POPITEM, 0, lParam);
	return 1;
}


int SPPlayerInvenArchive::OnTradeAccept(LPARAM lParam)
{
	g_pInterfaceManager->GetPcTradeWindow()->SPSendMessage(SPIM_TRADE_ACCEPT, 0, lParam);
	return 1;
}


int SPPlayerInvenArchive::OnTradeCancel(LPARAM lParam)
{
	g_pInterfaceManager->GetPcTradeWindow()->SPSendMessage(SPIM_TRADE_CANCEL, 0, lParam);
	return 1;
}


int SPPlayerInvenArchive::OnTradeWait(LPARAM lParam)
{
	g_pInterfaceManager->GetPcTradeWindow()->SPSendMessage(SPIM_TRADE_WAIT, 0, lParam);
	return 1;
}


int SPPlayerInvenArchive::OnNpcTrade(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;	
	GLOBAL_STRING_ID ErrorID;
	//
	pPacket->ExtractUINT32(&ErrorID);

	g_pCheckManager->SetDBBlock(false);

	if(ErrorID)
	{
		// ¿¡·¯ÀÔ´Ï´Ù.
		std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		return 1;
	}

	return 1;
}


int SPPlayerInvenArchive::OnPlayerInfoEquip(LPARAM lParam)
{
	g_pInterfaceManager->SPChildWndSendMessage(WIID_PC_INFO, SPIM_PC_INFO_SET_EQUIP, 0, lParam);
	return 1;
}


