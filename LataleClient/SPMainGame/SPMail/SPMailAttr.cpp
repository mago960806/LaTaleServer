#include "SPCommon.h"
#include "SPUtil.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"

#include "SPCheckManager.h"

#include "SPMailAttr.h"

SPMailAttr::SPMailAttr()
{
	m_pHeader = new MAIL_HEADER_ATTACHMENT;
	m_bSendBody = false;
	m_pItem = new SPItem(STORAGE, 0);
	Clear();
}


SPMailAttr::~SPMailAttr()
{
	if(m_pHeader)
		delete m_pHeader;

	if(m_pItem)
		delete m_pItem;
}


void SPMailAttr::Init(MAIL_HEADER_ATTACHMENT* pHeaderAttach)
{
	if(m_pHeader) {
		m_pHeader->uiMailID			= pHeaderAttach->uiMailID;
		m_pHeader->tmSendTime		= pHeaderAttach->tmSendTime;
		ZeroMemory(m_pHeader->szSender, LEN_NAME+1);
		wsprintf(m_pHeader->szSender, "%s", pHeaderAttach->szSender);

		m_pHeader->uiEly			= pHeaderAttach->uiEly		;
		m_pHeader->uiItemID			= pHeaderAttach->uiItemID	;
		m_pHeader->uiItemNo			= pHeaderAttach->uiItemNo	;
		m_pHeader->uiItemCount		= pHeaderAttach->uiItemCount;
		m_pHeader->uiReadFlag		= pHeaderAttach->uiReadFlag	;

		m_pHeader->iType			= pHeaderAttach->iType	;
		
		//for(int i = 0; i < 3; i++) {
		//	m_pHeader->uiDummy[i]	= pHeaderAttach->uiDummy[i];
		//}
	}

	m_iMailType = (int)pHeaderAttach->iType ;
}


void SPMailAttr::SetSubject(char* pSubject, int iLen)
{
	if( m_iMailType == MAIL_TYPE_NULL ) {
		if(m_pHeader && m_pHeader->uiMailID > 0) {
			m_strSubject = pSubject;
			m_iSubjectLen = iLen;
		}
	}
	else if( m_iMailType == MAIL_TYPE_AUCTION_SELL )	{
			m_strSubject = g_pResourceManager->GetGlobalString(52000067);
			m_iSubjectLen = (int)m_strSubject.size();
	}
	else if( m_iMailType == MAIL_TYPE_AUCTION_BUY )	{
			m_strSubject = g_pResourceManager->GetGlobalString(52000064);
			m_iSubjectLen = (int)m_strSubject.size();
	}
	else if( m_iMailType == MAIL_TYPE_AUCTION_OUTBID )	{
			m_strSubject = g_pResourceManager->GetGlobalString(52000066);
			m_iSubjectLen = (int)m_strSubject.size();
	}
	else if( m_iMailType == MAIL_TYPE_AUCTION_CANCEL_BIDDER )	{
			m_strSubject = g_pResourceManager->GetGlobalString(52000065);
			m_iSubjectLen = (int)m_strSubject.size();
	}
	else if( m_iMailType == MAIL_TYPE_AUCTION_CANCEL_SELLER)	{
			m_strSubject = g_pResourceManager->GetGlobalString(52000065);
			m_iSubjectLen = (int)m_strSubject.size();
	}
	else if( m_iMailType == MAIL_TYPE_AUCTION_EXPIRED )	{
			m_strSubject = g_pResourceManager->GetGlobalString(52000063);
			m_iSubjectLen = (int)m_strSubject.size();
	}
}


void SPMailAttr::SetBody(char* pMemo, int iLen)
{
	if(m_pHeader && m_pHeader->uiMailID > 0) {
		m_strMemo = pMemo;
		m_iMemoLen = iLen;
	}

	m_bSendBody = true;
}


bool SPMailAttr::SetItem(/*CONTAINER_ITEM* pItem*/)
{
	if(m_pHeader && m_pHeader->uiItemID) {
		if(m_pItem /*&& m_ContainerItem*/) {			
			return m_pItem->SetItemStatus(&m_ContainerItem);
		}
	}

	return false;
}


bool SPMailAttr::IsRead()
{
	if(m_pHeader) {
		return m_pHeader->uiReadFlag;
	}
	else 
		return false;
}


void SPMailAttr::Clear()
{	
	if(m_pHeader) {
		m_pHeader->uiMailID			= 0;				// Mail index
		ZeroMemory(&m_pHeader->tmSendTime, sizeof(TIMESTAMP_STRUCT));// 전송 시각
		ZeroMemory(m_pHeader->szSender, LEN_NAME+1);	// 송신자

		m_pHeader->uiEly			= 0;				// 첨부할 ely amount
		m_pHeader->uiItemID			= 0;				// 첨부된 Item Type (tbluItem2::TypeID)
		m_pHeader->uiItemNo			= 0;				// 첨부된 Item NO (tbluItem2::ItemNo)
		m_pHeader->uiItemCount		= 0;				// 첨부된 Item 갯수 (tbluItem2::cnt)
		m_pHeader->uiContainerType	= CONTAINER_TYPE_INVALID;	//Item 첨부시 사용
		m_pHeader->uiContainerSlot	= -1;						//Item 첨부시 사용
		//m_pHeader->uiDummy[0]		= 0;				// reserved
		m_pHeader->uiReadFlag		= false;
	}

	m_strSubject.clear();		// 제목
	m_iSubjectLen = 0;			// 제목 길이
	m_strReceiver.clear();		// 수신자, null included
	m_strMemo.clear();			// 본문 (MAX_MAIl_BODY_LEN == 100)
	m_iMemoLen	= 0;			// 본문 길이	
	m_iMailType = MAIL_TYPE_NULL ;
}


