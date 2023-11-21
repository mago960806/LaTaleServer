// Connector4SubLogic.h

#pragma once

#include "Connector.h"

class CConnector4SubLogic : public CConnector
{
public:
	CConnector4SubLogic(void);
	virtual ~CConnector4SubLogic(void);

	bool Init();
	bool Exit();

	virtual int OnConnect(DWORD Event);
	virtual int OnClose(DWORD Event);

	static void DispatchPacketChunk(CPacketMsg* pPacketMsg,LPVOID pThis);

	void SetServerInfo(TCHAR* pszIP,USHORT uiPort);


private:
	string m_sIP;
	USHORT m_usPort;
};
