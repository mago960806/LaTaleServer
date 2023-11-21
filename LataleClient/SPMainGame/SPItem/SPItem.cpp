#include "SPCommon.h"
#include "SPUtil.h"

#include "SPResourceDef.h"
#include "SPTexture.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"


//#include "SPGOBClusterDef.h"
//#include "SPAvatarModelUnitDef.h"
#include "SPWindowDEF.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPResourceDef.h"

//#include "PacketID.h"
//#include "Packet.h"
//#include "PacketID.h"
//#include "SPItem.h"
//#include "SPNetworkManager.h"
//#include "SPPlayerInvenArchive.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPItemAttr.h"
#include "SPItemCluster.h"
#include "SPItemStatus.h"
#include "SPItemStatusEquip.h"
#include "SPItemStatusConsumable.h"
#include "SPItem.h"

#include "PacketID.h"


SPItem::SPItem()
{
	//Clear();
	Init();
}


SPItem::SPItem(CONTAINER_TYPE iContainerType, int iSlotIndex)
{
	//Clear();
	Init();
	
	m_iContainerType = iContainerType;
	m_iSlotIndex = iSlotIndex;
	
	CreateItemStatus(iContainerType);
}


SPItem::~SPItem()
{
	//SAFE_DELETE(m_pItemAttr);
	SAFE_DELETE(m_pItemStatus);
	Clear();
}


void SPItem::Init()
{
	//m_pItemAttr			= NULL;

	//SAFE_DELETE(m_pItemStatus);
	m_pItemStatus		= NULL;

	m_iContainerType	= CONTAINER_TYPE_INVALID;
	m_iSlotIndex		= -1;

	m_iInstanceID		= 0;
	m_iQuickSlotIndex	= -1;
}


void SPItem::Clear() 
{	
	//m_pItemAttr			= NULL;
	
	//if(m_pItemStatus)
	//	SAFE_DELETE(m_pItemStatus);
	//if(m_pItemStatus)
	//	m_pItemStatus	= NULL;
	
	if(m_pItemStatus){		
		m_pItemStatus->SetItemID(0);
		SetItemStatusValue(MV_ITEM_SETUID, 0);
		m_pItemStatus->SetTimeAvailable(true);
	}

	m_iContainerType	= CONTAINER_TYPE_INVALID;
	m_iSlotIndex		= -1;

	m_iInstanceID		= 0;
	m_iQuickSlotIndex	= -1;	
}


bool SPItem::SetClear()
{
	//Interface Clear
	
	Clear();
	
	return true;
}


bool SPItem::SetItemAttr(CONTAINER_TYPE iContainerType, CONTAINER_ITEM* item /*= NULL*/)
{
	if(item == NULL)					return false;
	if(m_pItemStatus == NULL)			return false;
	
	//item->Item.iItemID
	
	////
	//if( g_pItemCluster->GetItemInfo(item->Item.iItemID, m_pItemAttr) ) {
	//	//m_iItemID = iItemID;
	//	m_pItemStatus->SetItemID(item->Item.iItemID);
	//	return true;
	//}
	return false;
	
	//return true;
}


bool SPItem::CreateItemStatus(CONTAINER_TYPE iContainerType, CONTAINER_ITEM* item /*= NULL*/)
{
	//if(item == NULL)				return false;
	//if(item->Item.iItemID == 0 )	return false;
	
	switch(iContainerType){
		case CONTAINER_TYPE_INVALID:
			return false;
			break;			
		case INVENTORY_EQUIP:			
			m_pItemStatus = new SPItemStatus();
			break;
		case INVENTORY_CONSUME:
			//m_pItemStatus = new SPItemStatusConsumable();
			m_pItemStatus = new SPItemStatus();
			break;
		case INVENTORY_ETC:
			m_pItemStatus = new SPItemStatus();
			break;
		case INVENTORY_EVENT:
			m_pItemStatus = new SPItemStatus();
			break;
		case INVENTORY_PET:
			m_pItemStatus = new SPItemStatus();
			break;
		case INVENTORY_HOUSING:
			m_pItemStatus = new SPItemStatus();
			break;
		case FIGURE_EQUIP:
			//m_pItemStatus = new SPItemStatusEquip();
			m_pItemStatus = new SPItemStatus();
			break;
		case FIGURE_FASHION:
			//m_pItemStatus = new SPItemStatusEquip();
			m_pItemStatus = new SPItemStatus();
			break;
		case FIGURE_BASIC:
			m_pItemStatus = new SPItemStatus();
			break;
		case STORAGE:
			m_pItemStatus = new SPItemStatus();
			break;
		case INVENTORY_CASH:
			m_pItemStatus = new SPItemStatus();
			break;
		case INVENTORY_WEAR:           //[xialin 2008/07/22] add
			m_pItemStatus = new SPItemStatus();
			break;
		case FIGURE_PET:
			m_pItemStatus = new SPItemStatus();
			break;
		case FIGURE_EFFECT:
			m_pItemStatus = new SPItemStatus();
			break;
		case CONTAINER_MAX:
			break;
		default:
			break;
	}
	
	
	return true;
}


bool SPItem::SetItemStatus(int iItemID, int iRear /*= 1*/)
{
	if(iItemID == 0)
		return false;

	if( m_pItemStatus->SetItemID(iItemID) == true ) {		
		m_pItemStatus->SPSendMessage( MV_ITEM_SETINDEX,			1);
		m_pItemStatus->SPSendMessage( MV_ITEM_SETSTATUSID,		m_iContainerType );
		m_pItemStatus->SPSendMessage( MV_ITEM_SETSTACK_CNT,		1 );
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_RARELEVEL,	iRear);
		//m_pItemStatus->SPSendMessage( MV_ITEM_SET_OPTION,		(LPARAM)item->Item.Opt );		
		
		return true;
	}	

	return false;
}


bool SPItem::SetItemStatus(CONTAINER_ITEM* item)
{
	if(item->Item.iItemID==0)
		return false;

	////
	//if(m_pStorageConsumable[item.SlotInfo.SlotIndex-1]->SetItemID( item.Item.iItemID )) {
	//	m_pStorageConsumable[item.SlotInfo.SlotIndex-1]->SPSendMessage( MV_ITEM_SETINDEX, item.SlotInfo.SlotIndex );
	//	m_pStorageConsumable[item.SlotInfo.SlotIndex-1]->SPSendMessage( MV_ITEM_SETSTATUSID, IS_CONSUMABLE );
	//	m_pStorageConsumable[item.SlotInfo.SlotIndex-1]->SPSendMessage( MV_ITEM_SETSTACK_CNT, item.Item.iStackCount );
	//	m_pStorageConsumable[item.SlotInfo.SlotIndex-1]->SPSendMessage( MV_ITEM_SET_RARELEVEL, item.Item.iRare);
	//	m_pStorageConsumable[item.SlotInfo.SlotIndex-1]->SPSendMessage( MV_ITEM_SET_OPTION, (LPARAM)item.Item.Opt );
	//}	
	
	if( m_pItemStatus->SetItemID(item->Item.iItemID) == true ) {		
		m_pItemStatus->SPSendMessage( MV_ITEM_SETINDEX,			item->SlotInfo.SlotIndex );
		m_pItemStatus->SPSendMessage( MV_ITEM_SETSTATUSID,		m_iContainerType );
		m_pItemStatus->SPSendMessage( MV_ITEM_SETSTACK_CNT,		item->Item.iStackCount );
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_RARELEVEL,	item->Item.iRare);
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_OPTION,		(LPARAM)item->Item.Opt );
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_BACKPRICE,	(LPARAM)item->Item.iBackPrice );
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_ITEMNO,		(LPARAM)&item->Item.iItemNo );
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_OPTPOINT,		item->Item.iOptPoint );
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_LIFETIME,		item->Item.iLifeTime );

		//[2006/10/27]
		m_pItemStatus->SetTimeAvailable(true);
		if(m_pItemStatus->GetItemAttr()->m_eDuration != ITEM_DURATION_NULL) {
			if(item->Item.iLifeTime < 1) {
				m_pItemStatus->SetTimeAvailable(false);
			}
		}

		//[2006/11/17]
		m_pItemStatus->SetExp(item->Item.iItemExp);
		m_pItemStatus->SetSp(item->Item.iItemSP);

		//ItemAttr도 설정
		//SetItemAttr(m_iContainerType, item);
		
		//인터페이스에도 연결하면 좋을듯...
		return true;
	}	
	
	return false;
}


bool SPItem::SetItemStatus(ITEM* Item, int iIndex /*= 0*/)
{
	//ITEMID		iItemID;			// 아이템 아이디
	//UINT8		iStackCount;		// 아이템 갯수
	//UINT32		iDurability;		// 내구도 
	//UINT32		iDuration;
	//UINT8		iRare;				// 레어도
	//PACK_OPT	Opt[MAX_ITEM_OPT];	// 옵션	
	
	if(Item->iItemID == 0)
		return false;

	if( m_pItemStatus->SetItemID(Item->iItemID) == true ) {		
		//m_pItemStatus->SPSendMessage( MV_ITEM_SETINDEX,			item->SlotInfo.SlotIndex );
		//m_pItemStatus->SPSendMessage( MV_ITEM_SETINDEX,			iIndex);
		m_pItemStatus->SPSendMessage( MV_ITEM_SETSTATUSID,		m_iContainerType );
		m_pItemStatus->SPSendMessage( MV_ITEM_SETSTACK_CNT,		Item->iStackCount );
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_RARELEVEL,	Item->iRare);
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_OPTION,		(LPARAM)Item->Opt );
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_BACKPRICE,	Item->iBackPrice );
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_ITEMNO,		(LPARAM)&Item->iItemNo );
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_OPTPOINT,		Item->iOptPoint );
		m_pItemStatus->SPSendMessage( MV_ITEM_SET_LIFETIME,		Item->iLifeTime );
		
		//[2006/10/27]
		m_pItemStatus->SetTimeAvailable(true);
		if(m_pItemStatus->GetItemAttr()->m_eDuration != ITEM_DURATION_NULL) {
			if(Item->iLifeTime < 1) {
				m_pItemStatus->SetTimeAvailable(false);
			}
		}
		
		//[2006/11/17]
		m_pItemStatus->SetExp(Item->iItemExp);
		m_pItemStatus->SetSp(Item->iItemSP);

		//인터페이스에도 연결하면 좋을듯...
		return true;
	}

	return false;
}


bool SPItem::SetItemStatus(FIGURE_ITEM* Item, int iIndex /*= 0*/)
{
	if(Item->iItemID == 0)
		return false;

	if( m_pItemStatus->SetItemID(Item->iItemID) == true ) {
		m_pItemStatus->SPSendMessage( MV_ITEM_SETSTATUSID,		m_iContainerType );
		//m_pItemStatus->SPSendMessage( MV_ITEM_SETSTACK_CNT,	Item->iStackCount );
		//m_pItemStatus->SPSendMessage( MV_ITEM_SET_RARELEVEL,	Item->iRare);
		//m_pItemStatus->SPSendMessage( MV_ITEM_SET_OPTION,		(LPARAM)Item->Opt );		
		return true;
	}

	return false;
}


bool SPItem::SetItem(CONTAINER_TYPE iType, int iSlotIndex, SPItem* pItem /*= NULL*/)
{
	//if(pItem == NULL)					return false;	
	//this->Clear();			//현재의 데이터를 비워주고

	this->m_iContainerType = iType;
	//if(m_iSlotIndex	!= iSlotIndex)		return false;
	this->m_iSlotIndex = iSlotIndex;	

	this->SetItemStatusValue(MV_ITEM_SETINDEX, this->m_iSlotIndex);

	////
	//this->m_pItemAttr = pItem->GetItemAttr();
	//this->m_pItemStatus = pItem->GetItemStatus();
	//this->m_pItemStatus->Copy(pItem->GetItemStatus());
	//this->m_pItemStatus->SetItemID(pItem->GetItemStatusValue(MV_ITEM_GETUID));
	//this->m_iQuickSlotIndex = pItem->GetQuickSlotIndex();

	return true;
	
}



//////////////////////////////////////////////////////////////////////////
SPItemAttr*	SPItem::GetItemAttr()
{
	//return m_pItemAttr;
	if(m_pItemStatus)
		return m_pItemStatus->GetItemAttr();
	return NULL;
}


SPItemStatus* SPItem::GetItemStatus()
{
	return m_pItemStatus;
}


bool SPItem::IsEmptyItem()
{
	if(m_pItemStatus == NULL)
		return true;	
	
	if(m_pItemStatus->GetItemAttr() == 0)
		return true;

	if(m_pItemStatus->GetItemID() == 0)
		return true;

	if(GetItemStatusValue(MV_ITEM_GETUID) == 0)
		return true;
	
	return false;
}


CONTAINER_TYPE SPItem::GetContainerType()
{
	return m_iContainerType;
}


int	SPItem::GetSlotIndex()
{
	return m_iSlotIndex;
}


int SPItem::GetQuickSlotIndex()
{
	return m_iQuickSlotIndex;
}


bool SPItem::ConvertContainerItem(CONTAINER_ITEM* item)
{
	//if(m_pItemAttr == NULL || m_pItemStatus == NULL )		return false;
	item->SlotInfo.ContainerType = m_iContainerType;
	item->SlotInfo.SlotIndex = m_iSlotIndex;

	if(m_pItemStatus == NULL) {								
		item->Item.iItemID = 0;
		item->Item.iStackCount = 0;
		return false;
	}
	
	if(m_pItemStatus->GetItemID() == 0 ) {
		item->Item.iItemID = 0;
		item->Item.iStackCount = 0;
		return false;
	}
	
	//item.Item.iItemID = nID;
	item->Item.iItemID = m_pItemStatus->GetItemID();	

	//item.SlotInfo.ContainerType = (CONTAINER_TYPE)itemStatus->SPSendMessage( MV_ITEM_GETSTATUSID );	
	//item->SlotInfo.ContainerType = (CONTAINER_TYPE)m_pItemStatus->GetItemStatusID();	

	item->Item.iRare = m_pItemStatus->GetRareLevel();
	item->Item.iLifeTime = m_pItemStatus->GetLifeTime();
	PACK_OPT* pOpt = (PACK_OPT*)m_pItemStatus->SPSendMessage(MV_ITEM_GET_OPTION);
	CopyMemory( &(item->Item.Opt), pOpt, sizeof(item->Item.Opt) );
	item->Item.iStackCount = m_pItemStatus->SPSendMessage( MV_ITEM_GETSTACK_CNT );
	item->Item.iBackPrice = m_pItemStatus->SPSendMessage( MV_ITEM_GET_BACKPRICE );
	item->Item.iItemNo = *((ITEMNO*)m_pItemStatus->SPSendMessage( MV_ITEM_GET_ITEMNO ));
	item->Item.iOptPoint = m_pItemStatus->SPSendMessage( MV_ITEM_GET_OPTPOINT );
	item->Item.iLifeTime = m_pItemStatus->SPSendMessage( MV_ITEM_GET_LIFETIME );
	item->Item.iItemExp = m_pItemStatus->GetExp();
	item->Item.iItemSP = m_pItemStatus->GetSp();
	////
	//switch( item.SlotInfo.ContainerType )
	//{
	//case INVENTORY_CONSUME:		
	//	item.Item.iStackCount = ((SPItemStatusConsumable*)itemStatus)->SPSendMessage( MV_ITEM_GETSTACK_CNT );		
	//	break;
	//default:		
	//	item.Item.iStackCount = 1;
	//	break;
	//}
	
	return true;
}


int SPItem::GetItemStatusValue(MODEL_VALUE_TYPE iMsgID)
{	
	if(m_pItemStatus == NULL)				return -1;
	
	if(m_pItemStatus->GetItemID() == 0)		return 0;

	int iReturnVal = 0;
	iReturnVal = m_pItemStatus->SPSendMessage(iMsgID);
	
	return iReturnVal;
}


int SPItem::SetItemStatusValue(MODEL_VALUE_TYPE iMsgID, int iValue /*= 0*/)
{
	if(m_pItemStatus == NULL)				return -1;

	if(m_pItemStatus->GetItemID() == 0)		return 0;

	m_pItemStatus->SPSendMessage(iMsgID, (LPARAM)iValue);

	return 1;
}


bool SPItem::DecreaseLifeTime(int iTime)
{
	if(m_pItemStatus == NULL)				return false;

	if(m_pItemStatus->GetItemID() == 0)		return false;

	if(m_pItemStatus->GetItemAttr()->GetDurationType() == ITEM_DURATION_NULL)
		return false;

	m_pItemStatus->DecreaseLifeTime(iTime);
	return true;
}
