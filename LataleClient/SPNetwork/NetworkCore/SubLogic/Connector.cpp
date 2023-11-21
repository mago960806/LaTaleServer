#include "StdAfx.h"
#include "Connector.h"

#include "PacketMsg.h"

CConnector::CConnector(void) : 
	m_pfnDispatchPacket(NULL)
	,m_bEncyptMode(false)
{
}

CConnector::~CConnector(void)
{
}

bool CConnector::Init()
{
	if(!CmyAsynchSocket4Client::Init()) return false;

	SetCallbackFunc(CConnector::OnReceivedPacketEx);
	SetEncryptMode(false);

	return true;
}

bool CConnector::Init(TCHAR* pszIPAddr,USHORT usPort)
{
	if(!CmyAsynchSocket4Client::Init()) return false;

	SetCallbackFunc(CConnector::OnReceivedPacketEx);
	SetEncryptMode(false);

	return Connect(pszIPAddr,usPort);
}

bool CConnector::Exit()
{
	Close();
	CmyAsynchSocket4Client::Exit();

	return true;
}

// return : Ã³¸®µÈ ÃÑ bytes ¼ö
int CConnector::OnReceivedPacketEx(BYTE* pBuffer,int nReceived,LPVOID pThis)
{
	CConnector* pSelf = (CConnector*)pThis;

	if(nReceived < 8)
	{
		return 0;
	}

	///////////////////////////////////////////
	unsigned __int16 len = 0;
	memcpy(&len,pBuffer,sizeof(len));
	CPacketMsg  packetMsg;

	if(pSelf)
		packetMsg.SetEncryptMode(pSelf->GetEncryptMode());
	else
		packetMsg.SetEncryptMode(false);

	USHORT usOriginLen = packetMsg.Set(0,(BYTE*)pBuffer,len,-1);	pBuffer += len;
	if(usOriginLen == 0)
	{
		DWORD dwError = packetMsg.GetLastError();
		ErrorLog("packetMsg.Set() failed. dwError=[%d]\n",dwError);
		return len;
	}
	///////////////////////////////////////////

	int nRemainedLen = nReceived;
	len = min(len,(UINT16)nRemainedLen);

	while(len >= 8 && nRemainedLen >= len)
	{
		if(pSelf && pSelf->m_pfnDispatchPacket)
			pSelf->m_pfnDispatchPacket(&packetMsg,pThis);

		nRemainedLen -= len;
		if(nRemainedLen <= 0) 
			break;

		// for the next PacketChunk
		//{
		memcpy(&len,pBuffer,sizeof(len));
		if(len > (UINT16)nRemainedLen) break;
		usOriginLen = packetMsg.Set(0,(BYTE*)pBuffer,len,-1);	pBuffer += len;
		if(usOriginLen == 0)
		{
			DWORD dwError = packetMsg.GetLastError();
			ErrorLog("packetMsg.Set() failed. dwError=[%d]\n",dwError);
			break;
		}
		//}
	}

	return nReceived - nRemainedLen;
}
