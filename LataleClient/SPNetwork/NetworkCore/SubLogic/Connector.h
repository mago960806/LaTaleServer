// Connector.h

#pragma once

#include "myAsynchSocket4Client.h"

struct CPacketMsg;

typedef 	void (*CALLBACK_DISPATCHPACKET_FN)(CPacketMsg* pPacketMsg,LPVOID pThis);

class CConnector : public CmyAsynchSocket4Client
{
public:
	CConnector(void);
	virtual ~CConnector(void);

	bool Init();
	bool Init(TCHAR* pszIPAddr,USHORT usPort);
	bool Exit();

	void SetDispathFunc(CALLBACK_DISPATCHPACKET_FN pfnCallback){m_pfnDispatchPacket = pfnCallback;}
	virtual LPVOID GetThis(){return this;}

	static int OnReceivedPacketEx(BYTE* pBuffer,int nReceived,LPVOID pThis);

	void SetEncryptMode(bool bUseEncrypt = false){m_bEncyptMode = bUseEncrypt;}
	bool GetEncryptMode(){return m_bEncyptMode;}

public:
	CALLBACK_DISPATCHPACKET_FN m_pfnDispatchPacket;

private:
	bool m_bEncyptMode;
};


/*
	CConnector client;
	client.Init();
	client.SetDispathFunc(...);

	client.Connect((char*)(LPCTSTR)addr,(USHORT)port);

	client.Close();

*/