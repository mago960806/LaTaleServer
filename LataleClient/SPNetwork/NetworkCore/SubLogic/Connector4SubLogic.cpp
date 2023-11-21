// Connector4SubLogic.cpp

#include "stdafx.h"
#include "PacketMsg.h"
#include "Connector4SubLogic.h"

#include "Util.h"
#include "PacketProcDefines.h"

CConnector4SubLogic::CConnector4SubLogic(void) : m_usPort(0)
{
}

CConnector4SubLogic::~CConnector4SubLogic(void)
{
}

bool CConnector4SubLogic::Init()
{
	m_sIP.empty();
	m_usPort = 0;

	SetDispathFunc(CConnector4SubLogic::DispatchPacketChunk);

	return CConnector::Init();
}

bool CConnector4SubLogic::Exit()
{
	return CConnector::Exit();
}


void CConnector4SubLogic::SetServerInfo(TCHAR* pszIP,USHORT uiPort)
{
	m_sIP = pszIP;
	m_usPort = uiPort;
}

int CConnector4SubLogic::OnConnect(DWORD Event)
{
	if(CConnector::OnConnect(Event) <0)
		return -1;

	TCHAR szIP[20] = {0,};
	USHORT usPort;

	SOCKET sock = GetSocketHandle();
	//GetPeerInfo(sock,szIP,usPort);
	//{
		struct sockaddr_in* pLocal;
		struct sockaddr localName;
		int lenLocalName = sizeof(localName);
		getpeername(sock,&localName,&lenLocalName);
		pLocal = (struct sockaddr_in*) &localName;
		usPort = pLocal->sin_port;
	//}

	CPacketComposer packet(ECHO_XX_PACKET);
	packet << (BYTE) pLocal->sin_addr.S_un.S_un_b.s_b1;
	packet << (BYTE) pLocal->sin_addr.S_un.S_un_b.s_b2;
	packet << (BYTE) pLocal->sin_addr.S_un.S_un_b.s_b3;
	packet << (BYTE) pLocal->sin_addr.S_un.S_un_b.s_b4;
	packet << (USHORT) usPort;

	if(!SendBufferEx(packet.GetBuffer(),packet.GetLength()))
	{
		return -1;
	}

	return 0;
}

int CConnector4SubLogic::OnClose(DWORD dwEvent)
{
	return CConnector::OnClose(dwEvent);
}

void CConnector4SubLogic::DispatchPacketChunk(CPacketMsg* pPacketMsg,LPVOID pThis)
{
	if(!pPacketMsg)
	{
		assert(pPacketMsg);
		return;
	}

	CConnector4SubLogic* pConnector = (CConnector4SubLogic*) pThis;

	switch(pPacketMsg->GetPacketID())
	{
		case SYSTEM_SC_ERROR:
		{
			GLOBAL_STRING_ID	itemID;	
			UINT16	uiPacketLen;			// 오류발생 시점의 패킷 정보
			UINT16	uiPacketKey;
			UINT32	uiPacketID;

			//GETVALUE(itemID);
			//GETVALUE(uiPacketLen);
			//GETVALUE(uiPacketKey);
			//GETVALUE(uiPacketID);
		}
		break;
		case MSG_SC_NOTICE:
		{
			RESULTCODE		resultCode = 0;
			UINT8			displayTime = 0;
			UINT8			optionFlags = 0;
			UINT16			len = 0;
			TCHAR			szMsg[MAX_CHATMSG_LEN+1];

			GETVALUE(resultCode);
			GETVALUE(displayTime);
			GETVALUE(optionFlags);
			GETVALUE(len);
			if(len > 0)
				GETDATA(szMsg,len);

			DebugPrint("MSG_SC_NOTICE : [%d][%d][%d][%s] \n",displayTime,optionFlags,len,szMsg);
		}
		break;
	default:
		DebugPrint(">>>[%d][%d][%d] \n",pPacketMsg->GetLength(),pPacketMsg->GetKey(),pPacketMsg->GetPacketID());
		break;
	}
}