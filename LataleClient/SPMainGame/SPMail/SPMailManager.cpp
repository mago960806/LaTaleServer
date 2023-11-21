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

//
#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModel.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPGOBCluster.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBModelUnit.h"			
#include "SPPlayerEquipInfo.h"
#include "SPGOBStatus.h"			
#include "SPPlayerInvenArchive.h"	
#include "SPPlayerStatusModel.h"	
#include "SPPlayerEquipInfo.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"
#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPMotionStatus.h"
#include "SPSkillManager.h"
#include "SPPlayer.h"
#include "SPCoolTimeManager.h"
#include "SPCommandConvert.h"
#include "SPComboManager.h"

#include "SPGameObject.h"
#include "SPGOBManager.h"

#include "Packet.h"

#include "SPCheckManager.h"

#include "SPMailAttr.h"
#include "SPMailArchive.h"
#include "SPMailManager.h"
#include "SPEventDEF.h"
#include "SPEventManager.h"

#include "SPDebug.h"

SPMailManager::SPMailManager()
{
	m_pMailArchive = new SPMailArchive;
	m_bMailLoad = false;
	m_iSelectMail = -1;
	m_pSelectMail = NULL;
	m_bNewMail = false;
}


SPMailManager::~SPMailManager()
{
	m_bMailLoad = false;
	if(m_pMailArchive)
		delete m_pMailArchive;
}


void SPMailManager::Clear()
{	
	m_pMailArchive->DeleteAllMail();	
	m_bMailLoad = false;
	m_iSelectMail = -1;
	m_pSelectMail = NULL;
	m_bNewMail = false;

	//SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_SHORTCUT);
	//if(pWindow /*&& pWindow->IsShow()*/) {
	//	pWindow->SPSendMessage(SPIM_SHOTCUT_MAILVOLUME, (WPARAM)MAIL_VOLUME_NULL, (LPARAM)0);
	//}
	
	//pWindow = g_pInterfaceManager->FindWindow(WIID_SHORTCUT);
	//if(pWindow /*&& pWindow->IsShow()*/) {
	//	pWindow->SPSendMessage(SPIM_SHOTCUT_MAILNEW, (WPARAM)m_bNewMail, (LPARAM)0);
	//}
}


int SPMailManager::AddMail(SPMailAttr* pMail)
{
	return m_pMailArchive->AddMail(pMail);
}


int SPMailManager::AddNewMail(SPMailAttr* pMail)
{
	m_bNewMail = true;

	int iResult = m_pMailArchive->AddMail(pMail);

	if(iResult > 0) {
		////리스트 갱신
		//OnUpdateMailList();

		if(m_bNewMail) {
			//SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_SHORTCUT);
			//if(pWindow /*&& pWindow->IsShow()*/) {
			//	pWindow->SPSendMessage(SPIM_SHOTCUT_MAILNEW, (WPARAM)m_bNewMail, (LPARAM)0);
			//}
			SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_STATPALLET);
			if(pWindow /*&& pWindow->IsShow()*/) {
				pWindow->SPSendMessage(SPIM_REFRESH, (WPARAM)0, (LPARAM)0);
			}
		}		
	}	

	//return m_pMailArchive->AddNewMail(pMail);	
	//return m_pMailArchive->AddMail(pMail);
	return iResult;
}


int	SPMailManager::GetMailListSize()
{
	return m_pMailArchive->GetSize();
}


void SPMailManager::SetNewMail(bool bNew /*= false*/) { 
	m_bNewMail = bNew;
	//SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_SHORTCUT);
	//if(pWindow /*&& pWindow->IsShow()*/) {
	//	pWindow->SPSendMessage(SPIM_SHOTCUT_MAILNEW, (WPARAM)m_bNewMail, (LPARAM)0);
	//}
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_STATPALLET);
	if(pWindow /*&& pWindow->IsShow()*/) {
		pWindow->SPSendMessage(SPIM_REFRESH, (WPARAM)0, (LPARAM)0);
	}
}


MAIL_VOLUME SPMailManager::GetMailVolume()
{
	int iMailSize = m_pMailArchive->GetSize();
	if(iMailSize > 37)
		return MAIL_VOLUME_LARGE;
	else if(iMailSize > 19)
		return MAIL_VOLUME_MIDDLE;
	else if(iMailSize > 0)
		return MAIL_VOLUME_SMALL;
	else 
		return MAIL_VOLUME_NULL;
}


void SPMailManager::CheckNewMail()
{
	//실제 인터페이스는 CheckReadMail과 반대로 돈다
	m_bNewMail = !(m_pMailArchive->CheckReadMail());
	
	//SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_SHORTCUT);	
	//if(pWindow /*&& pWindow->IsShow()*/) {
	//	pWindow->SPSendMessage(SPIM_SHOTCUT_MAILNEW, (WPARAM)m_bNewMail, (LPARAM)0);
	//}	
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_STATPALLET);
	if(pWindow /*&& pWindow->IsShow()*/) {
		pWindow->SPSendMessage(SPIM_REFRESH, (WPARAM)0, (LPARAM)0);
	}
}


SPMailAttr* SPMailManager::GetMail(int iIndex)
{
	return m_pMailArchive->GetMailAttr(iIndex);
}


void SPMailManager::SetSelectMail(int iIndex)
{
	if(GetMail(iIndex)) {
		m_iSelectMail = iIndex;
		m_pSelectMail = GetMail(m_iSelectMail);
	}	
}


void SPMailManager::OnUpdateMailList(bool bMsg /*= false*/)
{
	//m_pSelectMail = NULL;
	//m_iSelectMail = -1;
	//TCHAR				szMessage[256] ;
	//TCHAR				szItemName[MAX_ITEM_NAME] ;
	//D3DXCOLOR color( 0.62745f , 1.0f, 0.0f, 1.0f);

	m_pMailArchive->SetSort();
	
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MAILLIST);
	if(pWindow && pWindow->IsShow()) {
		pWindow->SPSendMessage(SPIM_MAIL_LIST_UPDATE, (WPARAM)0, (LPARAM)0);
	}

	//pWindow = g_pInterfaceManager->FindWindow(WIID_SHORTCUT);
	//if(pWindow /*&& pWindow->IsShow()*/) {
	//	pWindow->SPSendMessage(SPIM_SHOTCUT_MAILVOLUME, (WPARAM)GetMailVolume(), (LPARAM)0);
	//}
	pWindow = g_pInterfaceManager->FindWindow(WIID_STATPALLET);
	if(pWindow /*&& pWindow->IsShow()*/) {
		pWindow->SPSendMessage(SPIM_REFRESH, (WPARAM)0, (LPARAM)0);
	}

	CheckNewMail();

	//사이즈 가 40과 같거나 큰경우
	if(bMsg) {
		if(GetMailListSize() >= MAILBOX_MAX_ENTRY) {
			if(g_pResourceManager->GetGlobalString(5007101)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(5007101);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}		
		}
		else {
			if(m_bNewMail) {
				if(g_pResourceManager->GetGlobalString(5007100)) {
					std::string strMsg = g_pResourceManager->GetGlobalString(5007100);
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
				}
			}					
		}
	}	

	//m_pMailArchive->CheckReadMail();
	
}


void SPMailManager::SendGetMailList()
{
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MAILLIST);
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_MAIL_LIST_UPDATE, (WPARAM)0, (LPARAM)0);
	}
	
	//return;
	
	//MAILBOX_CS_GETLIST,
	// TCHAR					szCharName[LEN_NAME+1];					// null included
	std::string strUserName = "";
	if(g_pGOBManager->GetLocalPlayer()) {
		//wsprintf(stMailHeader.szSender, "%s", g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
		strUserName = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName();
	}
	
	//
	CPacket Packet;
	Packet.Add((UINT32)MAILBOX_CS_GETLIST);
	Packet.AddData(strUserName.c_str(), LEN_NAME+1);
	if(g_pEventManager->GetCurEvent()) {
		Packet.AddUINT32(g_pEventManager->GetCurEvent()->eventTypeID);
	}
	else {
		//return;
		Packet.AddUINT32(0);
	}

	//이부분 리턴값 문제 있을수 있음...
	g_pNetworkManager->SPPerformMessage(MAILBOX_CS_GETLIST, 0, (LPARAM)&Packet);	
}


void SPMailManager::SendGetMailBody(int iSelectIndex)
{
	m_iSelectMail = iSelectIndex;
	m_pSelectMail = GetMail(m_iSelectMail);
	
	if(m_pSelectMail == NULL || m_pSelectMail->m_pHeader->uiMailID == 0) {
		return;
	}

	if(m_pSelectMail->IsSendBody()) {			//이미 몸을 받은 경우
		return;
	}

	if(g_pEventManager->GetCurEvent() == NULL)
		return;
	
	//MAILBOX_CS_GETBODY,
	// UINT64					uiMailID;
	CPacket Packet;
	Packet.Add((UINT32)MAILBOX_CS_GETBODY);
	Packet.AddUINT64(m_pSelectMail->m_pHeader->uiMailID);
	Packet.AddUINT32(g_pEventManager->GetCurEvent()->eventTypeID);
	//이부분 리턴값 문제 있을수 있음...
	g_pNetworkManager->SPPerformMessage(MAILBOX_CS_GETBODY, 0, (LPARAM)&Packet);
}


void SPMailManager::SendReceiveBody(int iSelectIndex /*= -1*/)
{
	
	DXUTOutputDebugString("SendReceiveBody\n");
	
	if(iSelectIndex > -1) {					//외부에서 선택된 경우
		m_iSelectMail = iSelectIndex;
		m_pSelectMail = GetMail(m_iSelectMail);
	}	

	if(m_pSelectMail == NULL || m_pSelectMail->m_pHeader->uiMailID == 0) {
		return;
	}

	if(g_pCheckManager->IsDBBlock()) {
		DXUTOutputDebugString("\tSPWindowMailList::OnSend DB Working Block\n");
		return;
	}

	if(g_pEventManager->GetCurEvent() == NULL)
		return;

	//MAILBOX_CS_MOVEATTACHMENT,
	// UINT64					uiMailID;
	// CONTAINER_ITEM			containerItem;
	CPacket Packet;
	Packet.Add((UINT32)MAILBOX_CS_MOVEATTACHMENT);
	Packet.AddUINT64(m_pSelectMail->m_pHeader->uiMailID);
	Packet.AddData(&m_pSelectMail->m_ContainerItem, sizeof(CONTAINER_ITEM));
	Packet.AddUINT32(g_pEventManager->GetCurEvent()->eventTypeID);
	//이부분 리턴값 문제 있을수 있음...
	g_pNetworkManager->SPPerformMessage(MAILBOX_CS_MOVEATTACHMENT, 0, (LPARAM)&Packet);

	g_pCheckManager->SetDBBlock(true);
	DXUTOutputDebugString(" - MAILBOX_CS_MOVEATTACHMENT Send\n");
}


void SPMailManager::SendDeleteMail(int iSelectIndex /*= -1*/)
{	
	if(iSelectIndex > -1) {					//외부에서 선택된 경우
		m_iSelectMail = iSelectIndex;
		m_pSelectMail = GetMail(m_iSelectMail);
	}	

	if(m_pSelectMail == NULL || m_pSelectMail->m_pHeader->uiMailID == 0) {
		return;
	}

	if(g_pEventManager->GetCurEvent() == NULL)
		return;

	//MAILBOX_CS_DELETE,
	//UINT64					uiMailID;
	CPacket Packet;
	Packet.Add((UINT32)MAILBOX_CS_DELETE);
	Packet.AddUINT64(m_pSelectMail->m_pHeader->uiMailID);
	Packet.AddUINT32(g_pEventManager->GetCurEvent()->eventTypeID);
	//이부분 리턴값 문제 있을수 있음...
	g_pNetworkManager->SPPerformMessage(MAILBOX_CS_DELETE, 0, (LPARAM)&Packet);
}


bool SPMailManager::OnDeleteMail(UINT64 iMailID)
{
	if(m_pMailArchive->DeleteMail(iMailID)) {		
		//메시지 윈도우 닫음
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MAILMSG);
		if(pWindow) {
			pWindow->Hide();
		}
		
		//리스트 갱신
		OnUpdateMailList();

		m_iSelectMail = -1;
		m_pSelectMail = NULL;

		return true;
	}

	return false;
}


void SPMailManager::OnReadMail(UINT64 iMailID)
{
	SPMailAttr* pMail = m_pMailArchive->FindMail(iMailID);
	if(pMail && pMail->m_pHeader->uiReadFlag == false) {
		pMail->m_pHeader->uiReadFlag = true;

		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MAILLIST);
		if(pWindow) {
			pWindow->SPSendMessage(SPIM_MAIL_LIST_CHECK_UPDATE, (WPARAM)0, (LPARAM)0);
		}
	}	

	CheckNewMail();
}


bool SPMailManager::OnClearMailHeader(UINT64 iMailID)
{
	if(m_pMailArchive->ClearMailHeader(iMailID) == false) {
		return false;
	}

	//Interface clear 또는 Mail Delete
	SPMailAttr* pMailAttr = m_pMailArchive->FindMail(iMailID);

	if(pMailAttr->m_pHeader->uiItemID == 0 && pMailAttr->m_pHeader->uiEly == 0 &&
		pMailAttr->m_bSendBody && pMailAttr->m_iMemoLen == 0)
	{
		if(g_pEventManager->GetCurEvent() == NULL)
			return false;
		
		//메일 삭제 요청
		
		//MAILBOX_CS_DELETE,
		//UINT64					uiMailID;
		CPacket Packet;
		Packet.Add((UINT32)MAILBOX_CS_DELETE);
		Packet.AddUINT64(pMailAttr->m_pHeader->uiMailID);
		Packet.AddUINT32(g_pEventManager->GetCurEvent()->eventTypeID);		
		//이부분 리턴값 문제 있을수 있음...
		g_pNetworkManager->SPPerformMessage(MAILBOX_CS_DELETE, 0, (LPARAM)&Packet);
	}
	else {		
		//내용 갱신
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MAILMSG);
		if(pWindow) {
			pWindow->SPSendMessage(SPIM_MAIL_MSG_UPDATE, (WPARAM)m_iSelectMail, (LPARAM)0);
		}

		//리스트 갱신
		OnUpdateMailList();
	}

	return true;
}


