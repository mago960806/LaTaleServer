// SPNetworkManager_SC_MailBox.cpp

#include "SPCommon.h"
#include "SPManagerDef.h"
#include "SPSubGameManager.h"
#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"
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

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPWindow.h"
#include "SPInterfaceManager.h"

#include "SPEventDEF.h"
#include "SPMailAttr.h"
#include "SPMailArchive.h"
#include "SPMailManager.h"
#include "SPEventManager.h"
#include "SPCheckManager.h"

#include "SPNetworkManager.h"

DEFINE_SC_PACKET_PROC(MAILBOX_SC_GETLIST)
{
	//WPARAM wParam = 0;
	//LPARAM lParam = (LPARAM)packet;	
	//packet->ExtractUINT64(&ObjectGUID);	
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
	//		return ;
	//	}
	//}

	if(g_pGOBManager->GetLocalPlayer() == NULL) {
		return;
	}

	GLOBAL_STRING_ID			iError;
	UINT8						nMailCount;			// 수신 메일 갯수
	MAIL_HEADER_ATTACHMENT		mailHeaderInfo;		// 메일헤더 정보
	UINT8						uiSubjectLen;
	//CONTAINER_ITEM				containerItem;

	char szTemp[MAILBOX_MAX_SUBJECT_LEN + 1];
	
	packet->ExtractUINT32(&iError);
	if(iError) {			//실패
		// 에러입니다.
		std::string strMsg = g_pResourceManager->GetGlobalString(iError);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		return;
	}

	////[2006/5/8] - 리스트 받기전 초기화
	//g_pEventManager->GetMailManager()->Clear();

    packet->ExtractUINT8(&nMailCount);

	for(int i = 0; i < nMailCount; i++) {
		SPMailAttr* pNewAttr = new SPMailAttr;
		packet->ExtractStruct(&mailHeaderInfo, sizeof(MAIL_HEADER_ATTACHMENT));
		pNewAttr->Init(&mailHeaderInfo);
		
		ZeroMemory(szTemp, MAILBOX_MAX_SUBJECT_LEN + 1);
		packet->ExtractUINT8(&uiSubjectLen);
		packet->ExtractData(szTemp, uiSubjectLen);
		pNewAttr->SetSubject(szTemp, uiSubjectLen);

		//packet->ExtractStruct(&containerItem, sizeof(CONTAINER_ITEM));
		packet->ExtractStruct(&pNewAttr->m_ContainerItem, sizeof(CONTAINER_ITEM));
		pNewAttr->SetItem();

		if(g_pEventManager->GetMailManager()->AddMail(pNewAttr) < 0) {
			delete pNewAttr;
		}
	}	

	g_pEventManager->GetMailManager()->OnUpdateMailList(true);	
	
	//if(g_pSubGameManager)
	//	g_pSubGameManager->SPPerformMessage(SPGM_MAILBOX_GETLIST, wParam, lParam);	
}


DEFINE_SC_PACKET_PROC(MAILBOX_SC_SEND)
{
	//WPARAM wParam = 0;
	//LPARAM lParam = (LPARAM)packet;
	//packet->ExtractUINT64(&ObjectGUID);	
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
	//		return ;
	//	}
	//}	
	
	GLOBAL_STRING_ID			iError;
	UINT8						flagTrashItem;			// 0(NONE),1(Item를 인벤에서 삭제할 것)
	TRASH_ITEM					trashItem;	
	UINT64						uiRemainEly;			// 메일 전송후 잔여 Ely량
	TRASH_ITEM					UsingItem;				// 우표아이템

	if(g_pGOBManager->GetLocalPlayer() == NULL) {
		return;
	}

	g_pCheckManager->SetDBBlock(false);
	
	packet->ExtractUINT32(&iError);
	if(iError) {			//실패
		// 에러입니다.
		std::string strMsg = g_pResourceManager->GetGlobalString(iError);
		if(iError == 5007004) {
			char szMsg[256];
			ZeroMemory(szMsg, 256);
			
			char szName[LEN_NAME+1];
			ZeroMemory(szName, LEN_NAME+1);
			packet->ExtractData(szName, LEN_NAME+1);			
			
			wsprintf(szMsg, strMsg.c_str(), szName);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
		}
		else {
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
		return;
	}

	packet->ExtractUINT8(&flagTrashItem);
	if(flagTrashItem) {
		packet->ExtractStruct(&trashItem, sizeof(TRASH_ITEM));	//삭제할 아이템이 있는 경우
		trashItem.iContainerSlot.SlotIndex = trashItem.iContainerSlot.SlotIndex - 1;
		g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->TrashItem(trashItem);
	}

	//돈 갱신
	packet->ExtractUINT64(&uiRemainEly);
	if(g_pGOBManager->GetLocalPlayer()) {		
		g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_ELY, uiRemainEly);
	}	

	//우표 아이템
	packet->ExtractStruct(&UsingItem, sizeof(TRASH_ITEM));
	if(g_pGOBManager->GetLocalPlayer()) {
		UsingItem.iContainerSlot.SlotIndex = UsingItem.iContainerSlot.SlotIndex - 1;
		g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->TrashItem(UsingItem);
	}

	//Mail List Send Interface 클리어
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MAILLIST);
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_MAIL_LIST_SENDCLEAR, (WPARAM)0, (LPARAM)0);
	}	
	//if(g_pSubGameManager)
	//	g_pSubGameManager->SPPerformMessage(SPGM_MAILBOX_SEND, wParam, lParam);
}



DEFINE_SC_PACKET_PROC(MAILBOX_SC_DELETE)
{
	//WPARAM wParam = 0;
	//LPARAM lParam = (LPARAM)packet;
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
	//		return ;
	//	}
	//}	
	
	GLOBAL_STRING_ID			iError;
	UINT64					uiMailID;
	if(g_pGOBManager->GetLocalPlayer() == NULL) {
		return;
	}

	packet->ExtractUINT32(&iError);
	if(iError) {			//실패
		// 에러입니다.
		std::string strMsg = g_pResourceManager->GetGlobalString(iError);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		return;
	}

	packet->ExtractUINT64(&uiMailID);

	g_pEventManager->GetMailManager()->OnDeleteMail(uiMailID);

	//if(g_pSubGameManager)
	//	g_pSubGameManager->SPPerformMessage(SPGM_MAILBOX_DELETE, wParam, lParam);
}



DEFINE_SC_PACKET_PROC(MAILBOX_SC_GETBODY)
{
	//WPARAM wParam = 0;
	//LPARAM lParam = (LPARAM)packet;
	//packet->ExtractUINT64(&ObjectGUID);	
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
	//		return ;
	//	}
	//}	

	GLOBAL_STRING_ID		iError = 0;
	UINT8					uiLen = 0;
	TCHAR					szMemo[MAILBOX_MAX_MEMO_LEN + 1] = {0,};

	if(g_pGOBManager->GetLocalPlayer() == NULL) {
		return;
	}
	
	packet->ExtractUINT32(&iError);
	if(iError) {			//실패
		// 에러입니다.
		std::string strMsg = g_pResourceManager->GetGlobalString(iError);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		return;
	}

	UINT64 uiMailID = 0;
	packet->ExtractUINT64(&uiMailID);
	g_pEventManager->GetMailManager()->OnReadMail(uiMailID);

	//SPMailAttr* pSelectMail = g_pEventManager->GetMailManager()->GetSelectMail();	
	int iSelectIndex = g_pEventManager->GetMailManager()->GetSelectMailIndex();
	SPMailAttr* pSelectMail = g_pEventManager->GetMailManager()->GetMail(iSelectIndex);	
	if(pSelectMail == NULL || pSelectMail->m_pHeader->uiMailID == 0) {
		return;
	}

	packet->ExtractUINT8(&uiLen);
	packet->ExtractData(szMemo, uiLen);

	pSelectMail->SetBody(szMemo, uiLen);

	//리스트 업데이트
	//g_pEventManager->GetMailManager()->OnUpdateMailList();
	
	// Msg Window Update
	g_pInterfaceManager->ShowWindow(WIID_MAILMSG, SP_SHOW);	
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MAILMSG);
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_MAIL_MSG_UPDATE, (WPARAM)iSelectIndex, (LPARAM)0);
	}

	//if(g_pSubGameManager)
	//	g_pSubGameManager->SPPerformMessage(SPGM_MAILBOX_GETBODY, wParam, lParam);
}



DEFINE_SC_PACKET_PROC(MAILBOX_SC_MOVEATTACHMENT)
{
	//WPARAM wParam = 0;
	//LPARAM lParam = (LPARAM)packet;
	//packet->ExtractUINT64(&ObjectGUID);	
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
	//		return ;
	//	}
	//}	

	if(g_pGOBManager->GetLocalPlayer() == NULL) {
		return;
	}
	
	GLOBAL_STRING_ID		iError;
	UINT64					uiMailID;
	UINT64					uiRemainEly;	
	UINT8					uiItemCount;		// 아이템 생성 갯수
	//CONTAINER_ITEM			containerItem;		// 생성 아이템 정보

	g_pCheckManager->SetDBBlock(false);

	packet->ExtractUINT32(&iError);
	if(iError) {			//실패
		// 에러입니다.
		std::string strMsg = g_pResourceManager->GetGlobalString(iError);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		return;
	}	

	//첨부내용 삭제
	packet->ExtractUINT64(&uiMailID);
	g_pEventManager->GetMailManager()->OnClearMailHeader(uiMailID);

	//돈 갱신
	packet->ExtractUINT64(&uiRemainEly);
	if(g_pGOBManager->GetLocalPlayer()) {
		g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_ELY, uiRemainEly);
	}

	//실제 아이템을 받아서 처리
	packet->ExtractUINT8(&uiItemCount);	
	if(uiItemCount > 0) {
		GU_ID	CharGUID = g_pGOBManager->GetLocalPlayer()->GetGUID();
		WPARAM wParam = (WPARAM)&CharGUID;
		LPARAM lParam = (LPARAM)packet;
		g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_CONTAINER_INFO, wParam, lParam);
	}	

	//if(g_pSubGameManager)
	//	g_pSubGameManager->SPPerformMessage(SPGM_MAILBOX_MOVEATTACHMENT, wParam, lParam);
}


DEFINE_SC_PACKET_PROC(MAILBOX_SC_NEWMAIL)
{
	//WPARAM wParam = 0;
	//LPARAM lParam = (LPARAM)packet;
	//packet->ExtractUINT64(&ObjectGUID);	
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
	//		return ;
	//	}
	//}	

	if(g_pGOBManager->GetLocalPlayer() == NULL) {
		return;
	}

	GLOBAL_STRING_ID		iError;
	UINT8					nMailCount;				// 새로 수신한 메일 갯수 (현재는 항상 1)
	MAIL_HEADER_ATTACHMENT	mailHeaderInfo;			// 메일헤더 정보
	UINT8					uiSubjectLen;
	//TCHAR					szSubject[];

	packet->ExtractUINT32(&iError);
	if(iError) {			//실패
		// 에러입니다.
		std::string strMsg = g_pResourceManager->GetGlobalString(iError);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		return;
	}

	packet->ExtractUINT8(&nMailCount);

	for(int i = 0; i < nMailCount; i++) {
		SPMailAttr* pNewAttr = new SPMailAttr;
		packet->ExtractStruct(&mailHeaderInfo, sizeof(MAIL_HEADER_ATTACHMENT));
		pNewAttr->Init(&mailHeaderInfo);

		packet->ExtractUINT8(&uiSubjectLen);
		char szTemp[MAILBOX_MAX_SUBJECT_LEN + 1];
		packet->ExtractData(szTemp, uiSubjectLen);
		pNewAttr->SetSubject(szTemp, uiSubjectLen);
		
		packet->ExtractStruct(&pNewAttr->m_ContainerItem, sizeof(CONTAINER_ITEM));
		pNewAttr->SetItem();
		
		if(g_pEventManager->GetMailManager()->AddNewMail(pNewAttr) < 0) {
			delete pNewAttr;
		}

		//// 경매메일의 경우 시스템 메시지 추가
		if( pNewAttr->m_iMailType == MAIL_TYPE_AUCTION_EXPIRED )	{		// 만료
			if( g_pResourceManager->GetGlobalString(52000020) )
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(52000020));
		}
	}

	g_pEventManager->GetMailManager()->OnUpdateMailList(true);

	//if(g_pSubGameManager)
	//	g_pSubGameManager->SPPerformMessage(SPGM_MAILBOX_NEWMAIL, wParam, lParam);
}
