#include "SPCommon.h"
#include "SPMainGameDEF.H"

#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"

#include "Packet.h"
#include "PacketID.h"
#include "SPNetworkManager.h"

//
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"

#include "SPItemAttr.h"
#include "SPItem.h"
#include "SPItemCluster.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"
#include "SPClassChangeManager.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPCheckManager.h"

#include "SPDebug.h"



SPClassChangeManager::SPClassChangeManager()
{
	LoadClassChangeAttr();
	LoadClassChangeEffectAttr();
	LoadLevelUpEffectAttr();
}


SPClassChangeManager::~SPClassChangeManager()
{	
	DeleteAllClassChangeAttr();
	DeleteAllClassChangeEffectAttr();
	DeleteAllLevelUpEffectAttr();
}


void SPClassChangeManager::DeleteAllClassChangeAttr()
{
	std::map<unsigned int, SPClassChangeReqAttr*>::iterator Iter = m_mClassChangeReqInfo.begin();
	for(;Iter != m_mClassChangeReqInfo.end(); ++Iter) {
		SAFE_DELETE((*Iter).second);
	}
	
	m_mClassChangeReqInfo.clear();
}


bool SPClassChangeManager::LoadClassChangeAttr()
{
	SPLDTFile* pLDTFile = NULL;
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_CHANGE_CLASS , &pLDTFile);	
	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->CHANGE CLASS LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_CHANGE_CLASS );
#endif
		return false;
	}

	LDT_Field LDTField;
	int	iItemID = 0;
	int i = 0;
	int j = 0;

	int				iRecordCount	= pLDTFile->GetItemCount();
	SPClassChangeReqAttr* 	pChangeReqAttr = NULL;

	for( i = 0; i < iRecordCount; ++i )
	{
		pChangeReqAttr = new SPClassChangeReqAttr;
		
		iItemID	=	pLDTFile->GetPrimaryKey( i );

		pChangeReqAttr->m_iID	=	iItemID;		

		pLDTFile->GetField( iItemID , 32, LDTField );				//_Consumption_ItemID
		pChangeReqAttr->m_iItemID = LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 33, LDTField );				//_Consumption_Ele
		pChangeReqAttr->m_iEly = LDTField.uData.lValue;

		InsertClassChangeAttr(pChangeReqAttr);

	}

	SAFE_RELEASE(pLDTFile);
	return true;
}


bool SPClassChangeManager::InsertClassChangeAttr(SPClassChangeReqAttr* pClassChangeReqAttr)
{
	if(GetClassChangeReqAttr(pClassChangeReqAttr->m_iID))
		return false;

	m_mClassChangeReqInfo.insert(std::map<unsigned int, SPClassChangeReqAttr*>::value_type(pClassChangeReqAttr->m_iID, pClassChangeReqAttr));	
	return true;
}


SPClassChangeReqAttr*	SPClassChangeManager::GetClassChangeReqAttr(int iItemID)
{
	std::map<unsigned int, SPClassChangeReqAttr*>::iterator iter = m_mClassChangeReqInfo.find(iItemID);

	if(iter != m_mClassChangeReqInfo.end())
		return ((*iter).second);

	return NULL;
}


bool SPClassChangeManager::SendClassChangeInfo(int iClassChangeID)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	SPClassChangeReqAttr* pClassChangeReqAttr = GetClassChangeReqAttr(iClassChangeID);
	if(pClassChangeReqAttr == NULL)
		return false;

	TRASH_ITEM m_ReqItem;
	ZeroMemory(&m_ReqItem, sizeof(TRASH_ITEM));
	m_ReqItem.iContainerSlot.ContainerType = CONTAINER_TYPE_INVALID;

	if(pClassChangeReqAttr->m_iItemID) {
		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		if(pInven == NULL)
			return false;
		
		SPItemAttr* pItemAttr = NULL;
		pItemAttr = NULL;
		g_pItemCluster->GetItemInfo(pClassChangeReqAttr->m_iItemID, pItemAttr);
		if(pItemAttr == NULL)
			return false;
		
		CONTAINER_TYPE iContainer = (CONTAINER_TYPE)(pItemAttr->m_eType - 1);

		SPItem* pItem = pInven->GetSPItem(iContainer, pClassChangeReqAttr->m_iItemID);

		//아이템 확인
		if(pItem == NULL || pItem->IsEmptyItem()) {
			if(g_pResourceManager->GetGlobalString(13003001)) {
				char szTemp[256];
				ZeroMemory(szTemp, 256);
				
				std::string strMsg = g_pResourceManager->GetGlobalString(13003001);	//
				sprintf(szTemp, strMsg.c_str(), pItemAttr->m_strName.c_str());				

				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
			}
			return false;
		}
		else {
			m_ReqItem.iContainerSlot.ContainerType = iContainer;
			m_ReqItem.iContainerSlot.SlotIndex		= pItem->GetSlotIndex() + 1;
			m_ReqItem.iCount						= 1;
			//m_iReqItemNo = *(ITEMNO*)pItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
		}
	}

	if(pClassChangeReqAttr->m_iEly) {
		UINT64 iUserEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY);
		if(iUserEly < pClassChangeReqAttr->m_iEly) {
			if(g_pResourceManager->GetGlobalString(13003002)) {
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(13003002));
			}			
			return false;
		}
	}

	if(g_pCheckManager->IsDBBlock())
		return false;

	g_pCheckManager->SetDBBlock(true);

	// int				iChangeClassID ;	// CLASS_CHANGE.ldt::_Item_ID
	// TRASH_ITEM		stTrashItem;		// 소모아이템

	CPacket Packet;
	Packet.Add((UINT32)CHAR_CS_CHANGE_CLASS);			
	Packet.Add((UINT32)iClassChangeID);
	Packet.AddData(&m_ReqItem, sizeof(TRASH_ITEM));
	g_pNetworkManager->SPPerformMessage(CHAR_CS_CHANGE_CLASS, 0, (LPARAM)&Packet);

	return true;
}


bool SPClassChangeManager::OnClassChangeEffect(int iNewClassChangeType)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)	
		return false;
	
	SPClassChangeEffectAttr* pChangeEffect = GetClassChangeEffectAttr(iNewClassChangeType);
	
	if(pChangeEffect == NULL)
		return false;

	//
	if(pChangeEffect->m_iFx1) {
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_SET_FX_GROUP, pChangeEffect->m_iFx1);
	}

	if(pChangeEffect->m_iFx2) {
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_SET_FX_GROUP, pChangeEffect->m_iFx2);
	}

	if(pChangeEffect->m_iSoundEffect) {
		g_pGOBManager->GetLocalPlayer()->SetActionSound(pChangeEffect->m_iSoundEffect, true);
	}

	if(!pChangeEffect->m_strMsg.empty()) {
		float fRed = float(pChangeEffect->m_iRed) / 255.0f;
		float fGreen  = float(pChangeEffect->m_iGreen) / 255.0f;
		float fBlue  = float(pChangeEffect->m_iBlue) / 255.0f;
		D3DXCOLOR color(fRed, fGreen, fBlue, 1.0f);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING, (WPARAM)pChangeEffect->m_strMsg.c_str(), (LPARAM)&color);
	}

	return true;
}


SPClassChangeEffectAttr* SPClassChangeManager::GetClassChangeEffectAttr(int iItemID)
{
	std::map<unsigned int, SPClassChangeEffectAttr*>::iterator iter = m_mClassChangeEffectInfo.find(iItemID);

	if(iter != m_mClassChangeEffectInfo.end())
		return ((*iter).second);

	return NULL;
}


void SPClassChangeManager::DeleteAllClassChangeEffectAttr()
{
	std::map<unsigned int, SPClassChangeEffectAttr*>::iterator Iter = m_mClassChangeEffectInfo.begin();
	for(;Iter != m_mClassChangeEffectInfo.end(); ++Iter) {
		SAFE_DELETE((*Iter).second);
	}

	m_mClassChangeEffectInfo.clear();
}


bool SPClassChangeManager::LoadClassChangeEffectAttr()
{
	SPLDTFile* pLDTFile = NULL;
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_CHANGE_CLASS_EFFECT , &pLDTFile);	
	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->CHANGE CLASS EFFECT LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_CHANGE_CLASS_EFFECT );
#endif
		return false;
	}

	LDT_Field LDTField;
	int	iItemID = 0;
	int i = 0;
	int j = 0;

	int				iRecordCount	= pLDTFile->GetItemCount();
	SPClassChangeEffectAttr* 	pChangeEffectAttr = NULL;

	for( i = 0; i < iRecordCount; ++i )
	{
		pChangeEffectAttr = new SPClassChangeEffectAttr;

		iItemID	=	pLDTFile->GetPrimaryKey( i );

		pChangeEffectAttr->m_iID	=	iItemID;		

		pLDTFile->GetField( iItemID , 0, LDTField );				//_FX1
		pChangeEffectAttr->m_iFx1 = LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 1, LDTField );				//_FX2
		pChangeEffectAttr->m_iFx2 = LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 2, LDTField );				//_SoundEffect1
		pChangeEffectAttr->m_iSoundEffect = LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 3, LDTField );				//_Message
		if(LDTField.uData.pValue)
			pChangeEffectAttr->m_strMsg = LDTField.uData.pValue;

		pLDTFile->GetField( iItemID , 4, LDTField );				//_Red
		pChangeEffectAttr->m_iRed = LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 5, LDTField );				//_GReen
		pChangeEffectAttr->m_iGreen = LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 6, LDTField );				//_Blue
		pChangeEffectAttr->m_iBlue = LDTField.uData.lValue;

		//pLDTFile->GetField( iItemID , 7, LDTField );				//_Motion
		//if(LDTField.uData.lValue)
		//	pChangeEffectAttr->m_strMotion = LDTField.uData.lValue;
	
		InsertClassChangeEffectAttr(pChangeEffectAttr);
	}

	SAFE_RELEASE(pLDTFile);
	return true;
}


bool SPClassChangeManager::InsertClassChangeEffectAttr(SPClassChangeEffectAttr* pClassChangeEffectAttr)
{
	if(GetClassChangeEffectAttr(pClassChangeEffectAttr->m_iID))
		return false;

	m_mClassChangeEffectInfo.insert(std::map<unsigned int, SPClassChangeEffectAttr*>::value_type(pClassChangeEffectAttr->m_iID, pClassChangeEffectAttr));	
	return true;
}


SPLevelUpEffectAttr* SPClassChangeManager::GetLevelUpEffectAttr(int iItemID)
{
	std::map<unsigned int, SPLevelUpEffectAttr*>::iterator iter = m_mLevelUpEffectInfo.find(iItemID);

	if(iter != m_mLevelUpEffectInfo.end())
		return ((*iter).second);

	return NULL;
}


SPLevelUpEffectAttr* SPClassChangeManager::GetLevelUpEffectAttr(int iLevel, int iClass)
{
	int iItemID = GetLevelUpEffectID(iLevel, iClass);
	
	return GetLevelUpEffectAttr(iItemID);
}


int SPClassChangeManager::GetLevelUpEffectID(int iLevel, int iClass)
{
	int iID = iClass * 1000 + iLevel;
	return iID;
}


void SPClassChangeManager::DeleteAllLevelUpEffectAttr()
{
	std::map<unsigned int, SPLevelUpEffectAttr*>::iterator Iter = m_mLevelUpEffectInfo.begin();
	for(;Iter != m_mLevelUpEffectInfo.end(); ++Iter) {
		SAFE_DELETE((*Iter).second);
	}

	m_mLevelUpEffectInfo.clear();
}


bool SPClassChangeManager::LoadLevelUpEffectAttr()
{
	SPLDTFile* pLDTFile = NULL;
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_LEVEL_UP_EFFECT , &pLDTFile);	
	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->LEVEL UP EFFECT LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_LEVEL_UP_EFFECT );
#endif
		return false;
	}

	LDT_Field LDTField;
	int	iItemID = 0;
	int i = 0;
	int j = 0;

	int				iRecordCount	= pLDTFile->GetItemCount();
	SPLevelUpEffectAttr* 	pLevelUpEffectAttr = NULL;

	int iClass = 0;
	int iLevel = 0;
	for( i = 0; i < iRecordCount; ++i )
	{
		iClass = 0;
		iLevel = 0;		

		iItemID	=	pLDTFile->GetPrimaryKey( i );

		pLDTFile->GetField( iItemID , 0, LDTField );				//_Lv
		iLevel = LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 1, LDTField );				//_Class
		iClass = LDTField.uData.lValue;

		if(iLevel == 0 || iClass == 0)
			continue;
		
		pLevelUpEffectAttr = new SPLevelUpEffectAttr;

		pLevelUpEffectAttr->SetID(iLevel, iClass);					//pLevelUpEffectAttr->m_iID	=	iItemID;

		pLDTFile->GetField( iItemID , 2, LDTField );				//_FX2
		pLevelUpEffectAttr->m_iFx = LDTField.uData.lValue;
		
		pLDTFile->GetField( iItemID , 3, LDTField );				//_SoundEffect1
		pLevelUpEffectAttr->m_iSoundEffect = LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 4, LDTField );				//_Message
		if(LDTField.uData.pValue)
			pLevelUpEffectAttr->m_strMsg = LDTField.uData.pValue;

		pLDTFile->GetField( iItemID , 5, LDTField );				//_Red
		pLevelUpEffectAttr->m_iRed = LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 6, LDTField );				//_GReen
		pLevelUpEffectAttr->m_iGreen = LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 7, LDTField );				//_Blue
		pLevelUpEffectAttr->m_iBlue = LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 8, LDTField );				//_Guide_Book
		pLevelUpEffectAttr->m_iGuideBook = LDTField.uData.lValue;

		InsertLevelUpEffectAttr(pLevelUpEffectAttr);
	}

	SAFE_RELEASE(pLDTFile);
	return true;
}


bool SPClassChangeManager::InsertLevelUpEffectAttr(SPLevelUpEffectAttr* pLevelUpEffectAttr)
{
	if(GetLevelUpEffectAttr(pLevelUpEffectAttr->m_iID))
		return false;

	m_mLevelUpEffectInfo.insert(std::map<unsigned int, SPLevelUpEffectAttr*>::value_type(pLevelUpEffectAttr->m_iID, pLevelUpEffectAttr));	
	return true;	
}




