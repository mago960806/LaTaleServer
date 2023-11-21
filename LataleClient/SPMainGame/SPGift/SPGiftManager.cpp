#include "SPCommon.h"
#include "SPUtil.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "Packet.h"

#include "SPCheckManager.h"

#include "SPGiftAttr.h"
#include "SPGiftArchive.h"
#include "SPGiftManager.h"

#include "SPDebug.h"


SPGiftManager::SPGiftManager()
{	
	m_pGiftArchive = NULL;
	m_pGiftArchive = new SPGiftArchive;

	m_pLDTGiftData = NULL;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_GIFT, &m_pLDTGiftData);

	Clear();
}


SPGiftManager::~SPGiftManager()
{
	if(m_pGiftArchive)
		delete m_pGiftArchive;	

	m_bGiftListLoad = false;

	SAFE_RELEASE(m_pLDTGiftData);
}


void SPGiftManager::Clear()
{
	m_pGiftArchive->DeleteAllGift();
	
	m_bGiftListLoad = false;
}


bool SPGiftManager::InsertGift(int iItemID, INT64 iKeyID)
{
	if(iItemID == 0 || iKeyID == 0)
		return false;

	SPGiftAttr* pGiftAttr = new SPGiftAttr;

	pGiftAttr->Init(iKeyID);

	if(LoadGiftLDT(iItemID, pGiftAttr)) {
		m_pGiftArchive->AddGift(pGiftAttr);
	}
	else {
		if(pGiftAttr)
			delete pGiftAttr;
		
		return false;
	}

	return true;
}


bool SPGiftManager::LoadGiftLDT(int iItemID, SPGiftAttr* pGiftAttr)
{	
#ifdef _DEBUG
	assert(m_pLDTGiftData != NULL && "m_pLDTGiftData is NULL..");
#else
	if(m_pLDTGiftData == NULL)
		return false;
#endif

	if(pGiftAttr == NULL || pGiftAttr->m_iKey == 0)
		return false;

	int recordCount = m_pLDTGiftData->GetItemCount();
	int i = 0;

	LDT_Field ldtFieldEventGroup;
	LDT_Field ldtFieldSubject;
	LDT_Field ldtFieldDesc;		
	LDT_Field ldtFieldItemID;
	LDT_Field ldtFieldItemRare;
	LDT_Field ldtFieldItemCount;
	LDT_Field ldtFieldEly;

	m_pLDTGiftData->GetFieldFromLable(iItemID, "_EventGroup",	ldtFieldEventGroup);
	int iEventGroup = ldtFieldEventGroup.uData.lValue;

	m_pLDTGiftData->GetFieldFromLable(iItemID, "_Subject",	ldtFieldSubject);
    if(ldtFieldSubject.uData.pValue)
		pGiftAttr->m_strName = ldtFieldSubject.uData.pValue;

	m_pLDTGiftData->GetFieldFromLable(iItemID, "_Description",	ldtFieldDesc);
	if(ldtFieldDesc.uData.pValue)
		pGiftAttr->m_strDesc = ldtFieldDesc.uData.pValue;

	m_pLDTGiftData->GetFieldFromLable(iItemID, "_ItemID",	ldtFieldItemID);
	pGiftAttr->m_iItemID = ldtFieldItemID.uData.lValue;

	m_pLDTGiftData->GetFieldFromLable(iItemID, "_ItemRare",	ldtFieldItemRare);
	pGiftAttr->m_iItemRare = ldtFieldItemRare.uData.lValue;

	m_pLDTGiftData->GetFieldFromLable(iItemID, "_ItemCount",	ldtFieldItemCount);
	pGiftAttr->m_iItemStack = ldtFieldItemCount.uData.lValue;

	m_pLDTGiftData->GetFieldFromLable(iItemID, "_Ely",	ldtFieldEly);
	pGiftAttr->m_iEly = ldtFieldEly.uData.lValue;

#ifdef _DEBUG
	
	DXUTOutputDebugString("GiftItem[Key:%d] Name[%s] Desc[%s] Item[%d-%d-%d] Ely[%d]\n", 
		pGiftAttr->m_iKey, pGiftAttr->m_strName.c_str(), pGiftAttr->m_strDesc.c_str(), pGiftAttr->m_iItemID,
		pGiftAttr->m_iItemRare, pGiftAttr->m_iItemStack, pGiftAttr->m_iEly);
#endif
    	
	return true;
}


int	SPGiftManager::GetGiftListSize()
{
	return m_pGiftArchive->GetSize();
}


SPGiftAttr* SPGiftManager::GetGift(int iIndex)
{
	return m_pGiftArchive->GetGiftAttr(iIndex);
}


bool SPGiftManager::DeleteGiftItem(UINT64 iKey)
{	
	return m_pGiftArchive->DeleteGift(iKey);
}


void SPGiftManager::SendGiftList()
{
	//
	CPacket Packet;
	Packet.Add((UINT32)GIFT_CS_LIST);

	//이부분 리턴값 문제 있을수 있음...
	g_pNetworkManager->SPPerformMessage(GIFT_CS_LIST, 0, (LPARAM)&Packet);
	
	Clear();
}


void SPGiftManager::SendGetGiftItem(int iIndex)
{
	if(iIndex < 0 || iIndex >= GetGiftListSize())
		return;

	SPGiftAttr* pGiftAttr = GetGift(iIndex);
	if(pGiftAttr == NULL  || pGiftAttr->m_iKey == 0)
		return;

	if(g_pCheckManager->IsDBBlock()) {
		return;
	}

	if(pGiftAttr->m_iItemID != 0) {
		if(false == g_pCheckManager->CheckEnableSlot(pGiftAttr->m_iItemID, pGiftAttr->m_iItemStack)) {
			std::string strMsg;
			if(g_pResourceManager->GetGlobalString(6001004) != NULL)
				strMsg = g_pResourceManager->GetGlobalString(6001004);

			if(strMsg != "") {
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			
			return;
		}
	}

	// __int64				iGiftNO;
	CPacket Packet;
	Packet.Add((UINT32)GIFT_CS_GET);
	Packet.AddUINT64(pGiftAttr->m_iKey);

	//이부분 리턴값 문제 있을수 있음...
	g_pNetworkManager->SPPerformMessage(GIFT_CS_GET, 0, (LPARAM)&Packet);	

	g_pCheckManager->SetDBBlock(true);
}


