// myAsynchSocket4Client.h

// Copyright (C) hoshim
//***************************************************************************
// System Name : CmyAsynchSocket4Client
// Comment     : Asynchronous Socket for Client connector
//
// Creation    : hoshim 2005-04-29 오전 11:00:00
//***************************************************************************

#pragma once

#include <queue>
using namespace std;

#ifdef SC_THREAD_SAFE
	#include "LockObject.h"
#endif

typedef 	int (*CALLBACK_RECV_FN)(BYTE* pszBuf,int nReceived,LPVOID pThis);

// 클라이언트 정보 구조체 (소켓정보, 입출력 정보)
typedef struct 
{
	BYTE	szBuf[MAX_PACKET_SIZE];
	SOCKET	Socket;
	DWORD	dwSent;				// 현재까지 전송된 사이즈
	DWORD	dwRecv;
	DWORD	dwTotalToSend;		// 전송될 총 사이즈
	WSABUF	wsaBuf;
	DWORD	dwRemained;			// 읽기 모드에서 잔여량

} SOCKINFO, *PSOCKINFO;

typedef struct 
{
	WSABUF	wsaBuf;
	CHAR	Buffer[MAX_PACKET_SIZE];
	DWORD	dwTotalToSend;		//전송할 총량
	DWORD	dwSent;				//현재까지 전송한 량
}SEND_BUFFER,*PSEND_BUFFER;

class CmyAsynchSocket4Client
{
public:
	CmyAsynchSocket4Client(void);
	virtual ~CmyAsynchSocket4Client(void);

private:
	SOCKET m_Socket;
	WSADATA m_wsadata;
	sockaddr_in m_addr_in;

	WSAEVENT m_wsaEvent;
	SOCKINFO m_SockInfo;

	CALLBACK_RECV_FN m_pfnOnReceive;
	
	bool m_bConnected;

	TCHAR m_szDestIP[20];
	USHORT m_usDestPort;

	DWORD m_dwConnectRetry_Period;
	DWORD m_dwConnectRetry_LastTime;

public:

	bool Init();
	void Exit();

	bool Process(DWORD dwCheckTimeout);

	void SetCallbackFunc(CALLBACK_RECV_FN pfnCallback){m_pfnOnReceive = pfnCallback;}

	bool Connect(char* ipaddr,unsigned short port);
	void Close(void);

	bool CheckEvent(const DWORD& dwTimeoout = WSA_INFINITE);	// check event once

	bool IsConnected(){return m_bConnected;	}

	bool InitSockInfo(SOCKET s);
	void ResetBufferLength();

	virtual int OnConnect(DWORD Event);
	virtual int OnReceive(DWORD Event);
	virtual int OnWritable(DWORD Event);
	virtual int OnClose(DWORD Event);

	int OnReceivedPacketEx(BYTE* pszBuf,int nReceived);

	int SendData(DWORD Event,char* pszData, int nLen);
	int RecvData();

	bool SendBuffer(const TCHAR* pszBuffer,const DWORD& dwLen);

	SOCKET GetSocketHandle(){return m_Socket;};

// Send Queue 구현

public:
	bool SendBufferEx(const TCHAR* pszBuffer,const DWORD& dwLen);
	void AddToSendQueue(SEND_BUFFER* pSendQueueEntry);		// Send Queue에 추가
	bool SendFromQueue();									// Loop에서 현재 쌓여있는 송신 패킷 전송

private:

#ifdef SC_THREAD_SAFE
	CLockObject			m_LockObject;
#endif

	bool	m_bWritable;
	typedef queue<SEND_BUFFER*>	queueForSendQueueEntry;
	queueForSendQueueEntry m_SendQueue;
};
