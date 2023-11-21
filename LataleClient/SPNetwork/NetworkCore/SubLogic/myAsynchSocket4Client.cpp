#include "StdAfx.h"
#include "myAsynchSocket4client.h"

#ifdef SC_THREAD_SAFE
	#include "GuardLock.h"
#endif

CmyAsynchSocket4Client::CmyAsynchSocket4Client(void) : 
	m_Socket(INVALID_SOCKET)
	,m_pfnOnReceive(NULL)
	,m_bConnected(false)
	,m_bWritable(false)
	,m_wsaEvent(WSA_INVALID_EVENT)
{
}

CmyAsynchSocket4Client::~CmyAsynchSocket4Client(void)
{
	SEND_BUFFER* pSendData = NULL;
	if(!m_SendQueue.empty())
	{
		pSendData = m_SendQueue.front();

		while(!m_SendQueue.empty())
		{
			SAFE_DELETE(pSendData);
			m_SendQueue.pop();
			pSendData = m_SendQueue.front();
		}
	}
}

void CmyAsynchSocket4Client::Close(void)
{
	m_bWritable = false;
	m_bConnected = false;

	if(m_Socket != INVALID_SOCKET)
	{
		////if(!bGraceful)
		//{
		//	LINGER linger;
		//	linger.l_onoff = 1;
		//	linger.l_linger = 0;
		//	::setsockopt(m_Socket,SOL_SOCKET,SO_LINGER,(char*)&linger,sizeof(linger));
		//}

		shutdown(m_Socket,SD_SEND);

		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}

	if(m_wsaEvent != WSA_INVALID_EVENT)
	{
		WSACloseEvent(m_wsaEvent);
		m_wsaEvent = WSA_INVALID_EVENT;
	}
}

bool CmyAsynchSocket4Client::Init()
{
	int nReturn = WSAStartup(0x0202, &m_wsadata);		// socket library loading
	if (nReturn != 0)
	{
		return false;
	}

	memset(m_szDestIP,0,sizeof(m_szDestIP));
	m_usDestPort = 0;
	
	m_dwConnectRetry_LastTime = 0;
	m_dwConnectRetry_Period = 10000;

	return true;
}

void CmyAsynchSocket4Client::Exit()
{
	if(m_Socket != INVALID_SOCKET)
		Close();

	int nReturn = WSACleanup();
	if (nReturn != 0)
	{
		return;
	}
}

bool CmyAsynchSocket4Client::Connect(char* ipaddr,unsigned short port)
{
	if(ipaddr[0] == '\0' || port == 0)
	{
#ifdef _DEBUG
		_ASSERTE(ipaddr[0] != '\0' || port != 0);
#endif
		return false;
	}

	DebugPrint("[%d] CmyAsynchSocket4Client::Connect(%s:%d)\n",GetTickCount(), ipaddr,port);

	m_bConnected = false;
	m_bWritable = false;

	m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(m_Socket == INVALID_SOCKET)
		return false;

	int nodelay = 1;
	int len = sizeof(int);

	if(SOCKET_ERROR ==(::setsockopt(m_Socket,IPPROTO_TCP,TCP_NODELAY,(char*)&nodelay,sizeof(nodelay))) )
	{
		SC_WSAERROR(setsocket_nodelay);
		return false;
	}
	if(SOCKET_ERROR ==(::getsockopt(m_Socket,IPPROTO_TCP,TCP_NODELAY,(char*)&nodelay,&len)) )
	{
		SC_WSAERROR(getsocket_nodelay);
		return false;
	}

	InitSockInfo(m_Socket);

	if (WSAEventSelect(m_Socket, m_wsaEvent, FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE) == SOCKET_ERROR)
	{
		SC_WSAERROR("WSAEventSelect");
		return false;
	}

	m_addr_in.sin_family = AF_INET;
	m_addr_in.sin_port = htons(port);
	m_addr_in.sin_addr.S_un.S_addr = inet_addr(ipaddr);

	if(m_addr_in.sin_addr.S_un.S_addr == INADDR_NONE)
	{
		struct hostent *h = gethostbyname(ipaddr);
		if(!h)	return false;
		memcpy(&(m_addr_in.sin_addr.S_un.S_addr), h->h_addr, sizeof(int));
	}

	if(connect(m_Socket, (struct sockaddr *)&m_addr_in, sizeof(m_addr_in)) != 0) 
	{
		DWORD dwError = WSAGetLastError();
		if(dwError != WSAEWOULDBLOCK)
		{
			SC_WSAERROR("connect");
			return false;
		}
	}

	_tcscpy((TCHAR*)m_szDestIP,ipaddr);
	m_usDestPort = port;

	m_dwConnectRetry_LastTime = GetTickCount();

	return true;
}

bool CmyAsynchSocket4Client::InitSockInfo(SOCKET s)
{
	if(m_wsaEvent != WSA_INVALID_EVENT)
	{
		WSACloseEvent(m_wsaEvent);
		m_wsaEvent = WSA_INVALID_EVENT;
	}
	else if ((m_wsaEvent = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
		return false;
	}

	m_SockInfo.Socket = s;
	ResetBufferLength();

	return true;
}

void CmyAsynchSocket4Client::ResetBufferLength()
{
	m_SockInfo.dwSent = 0;
	m_SockInfo.dwRecv = 0;
	m_SockInfo.dwTotalToSend = 0;
	m_SockInfo.dwRemained = 0;
}

bool CmyAsynchSocket4Client::CheckEvent(const DWORD& dwTimeoout /* = WSA_INFINITE */)
{
	DWORD Event;
	WSANETWORKEVENTS NetworkEvents;
	DWORD dwEventTotal = 1;
	bool bOK = false;

	// Wait for one of the sockets to receive I/O notification and 
	if((Event = WSAWaitForMultipleEvents(dwEventTotal, &m_wsaEvent, FALSE,dwTimeoout,FALSE)) == WSA_WAIT_FAILED
		|| Event == WSA_WAIT_TIMEOUT)
	{
		if(Event == WSA_WAIT_TIMEOUT)
		{
			return true;
		}
		
		//DebugPrint("WSA_WAIT_TIMEOUT XXX : ErrorCode=[%d]\n", WSAGetLastError());
		return false;
	}

	// m_wsaEvent가 수동 리셋 모드이므로, WSAResetEvent()를 불러주어야 하는데, WSAEnumNetworkEvents()에서
	//		자동 리셋되므로 WSAResetEvent()를 호출할 필요 없어짐.
	if (WSAEnumNetworkEvents(m_SockInfo.Socket, m_wsaEvent, &NetworkEvents) == SOCKET_ERROR)
	{
		return false;
	}

	if (NetworkEvents.lNetworkEvents & FD_CONNECT)
	{
		if (NetworkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
		{
			TCHAR szError[512] = {0,};
			sprintf(szError, "FD_CONNECT (%s,%d) EC=[%ld, %d] : Dest=[%s:%d]\n",__FUNCTION__,__LINE__,::WSAGetLastError(),  NetworkEvents.iErrorCode[FD_CONNECT_BIT], m_szDestIP,m_usDestPort);
			SC_ERROR(szError);
			//DebugPrint("FD_CONNECT XXX : ErrorCode=[%d]\n", NetworkEvents.iErrorCode[FD_CONNECT_BIT]);

			return (0 == OnClose(Event))?true:false;
		}

		bOK = (0 == OnConnect(Event))?true:false;
	}


	// FD_READ|FD_WRITE 이벤트에 대한 처리		
	if (NetworkEvents.lNetworkEvents & FD_READ)
	{
		if(NetworkEvents.iErrorCode[FD_READ_BIT] != 0)
		{
			//ErrorLog("FD_READ (%s,%d) EC=[%ld, %d] : Dest=[%s:%d]\n",__FUNCTION__,__LINE__,::WSAGetLastError(),m_szDestIP,m_usDestPort,  NetworkEvents.iErrorCode[FD_READ_BIT]);
			DebugPrint("FD_READ XXX : ErrorCode=[%d]\n", NetworkEvents.iErrorCode[FD_READ_BIT]);
			return false;
		}

		if(OnReceive(Event) == 0)
		{
			if(m_pfnOnReceive)
			{
				int nProcessed = m_pfnOnReceive(m_SockInfo.szBuf,m_SockInfo.dwRecv,this);
				int nRemained = m_SockInfo.dwRecv - nProcessed;
				m_SockInfo.dwRemained = max(0,nRemained);

				bOK = true;
			}
		}

		// 바쁜 경우에는 FD_READ 와 FD_WRITE가 동시에 전달될 수 있음.
	}

	if (NetworkEvents.lNetworkEvents & FD_WRITE)
	{
		if(NetworkEvents.iErrorCode[FD_WRITE_BIT] != 0)
		{
			//ErrorLog("FD_WRITE (%s,%d) EC=[%ld, %d] : Dest=[%s:%d]\n",__FUNCTION__,__LINE__,::WSAGetLastError(),m_szDestIP,m_usDestPort,  NetworkEvents.iErrorCode[FD_WRITE_BIT]);
			DebugPrint("FD_WRITE XXX : ErrorCode=[%d]\n", NetworkEvents.iErrorCode[FD_WRITE_BIT]);
			return false;
		}

		bOK = (0 == OnWritable(Event))?true:false;
	}

	// FD_CLOSE 이벤트에 대한 처리		
	if (NetworkEvents.lNetworkEvents & FD_CLOSE)
	{
		TCHAR szError[512] = {0,};
		sprintf(szError, "FD_LOSE (%s,%d) EC=[%ld, %d] : Dest=[%s:%d]\n",__FUNCTION__,__LINE__,::WSAGetLastError(),  NetworkEvents.iErrorCode[FD_CLOSE_BIT],m_szDestIP,m_usDestPort);
		SC_ERROR(szError);

		//DebugPrint("FD_CLOSE XXX : ErrorCode=[%d]\n", NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
		OnClose(Event);

		return false;
	}

	return bOK;
}

int CmyAsynchSocket4Client::OnConnect(DWORD /*Event*/)
{
	if (WSAEventSelect( m_SockInfo.Socket, m_wsaEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		SC_WSAERROR("CmyAsynchSocket4Client::WSAEventSelect() was failed\n");
		closesocket(m_SockInfo.Socket);
		return -1;
	}

	DebugPrint("CmyAsynchSocket4Client::OnConnect(%s:%d) OK\n",m_szDestIP,m_usDestPort);

	m_bConnected = true;
	m_bWritable = true;

	return 0;
}

int CmyAsynchSocket4Client::OnReceive(DWORD /*Event*/)
{
	DWORD dwFlags = 0;
	DWORD dwRecv;

	// 버퍼에 남아 있음. 이어받기 시도
	if(m_SockInfo.dwRemained > 0)
	{
		// 2008.05.08 dhpark 잔여량 이전에 버그가 있어서 수정합니다. 남은양과 받을양에 대한 개념정리
		//// 잔여량 앞으로 이전
		////{
		//	BYTE szTemp[MAX_PACKET_SIZE];
		//	int nProcessed = max(0,m_SockInfo.dwRecv-m_SockInfo.dwRemained);
		//	nProcessed = min(nProcessed,MAX_PACKET_SIZE);
		//	memcpy(szTemp,m_SockInfo.szBuf+nProcessed,m_SockInfo.dwRemained);
		//	memcpy(m_SockInfo.szBuf,szTemp,nProcessed);
		////}

		//m_SockInfo.wsaBuf.buf = (char*)(m_SockInfo.szBuf + nProcessed);
		//m_SockInfo.wsaBuf.len = MAX_PACKET_SIZE - nProcessed;
		//m_SockInfo.dwRecv = nProcessed;

		// 잔여량 앞으로 이전
		//{
			BYTE szTemp[MAX_PACKET_SIZE] = {0,};
			int nProcessed = max(0,m_SockInfo.dwRecv-m_SockInfo.dwRemained);
			nProcessed = min(nProcessed,MAX_PACKET_SIZE);
			memcpy(szTemp,m_SockInfo.szBuf+nProcessed,m_SockInfo.dwRemained);
			memcpy(m_SockInfo.szBuf,szTemp,m_SockInfo.dwRemained);
		//}

		m_SockInfo.wsaBuf.buf = (char*)(m_SockInfo.szBuf + m_SockInfo.dwRemained);
		m_SockInfo.wsaBuf.len = MAX_PACKET_SIZE - m_SockInfo.dwRemained;
		m_SockInfo.dwRecv = m_SockInfo.dwRemained;
	}
	else 
	{
		m_SockInfo.wsaBuf.buf = (char*)m_SockInfo.szBuf;
		m_SockInfo.wsaBuf.len = MAX_PACKET_SIZE;
		m_SockInfo.dwRecv = 0;
	}

	//{
		dwFlags = 0;
		if (WSARecv(m_SockInfo.Socket, &(m_SockInfo.wsaBuf), 1, &dwRecv,&dwFlags, NULL, NULL) == SOCKET_ERROR)
		{
			DWORD dwError = WSAGetLastError();
			if (dwError != WSAEWOULDBLOCK)
			{
				SC_WSAERROR("WSARecv() was failed\n");
				Close();
				return -1;
			}
		} 
		else
		{
			// 2008.05.08 dhpark 버퍼에 남아있는것이 있는경우를 위해서 더해줘야 맞다고 생각하여 수정합니다.
			//m_SockInfo.dwRecv = dwRecv;
			m_SockInfo.dwRecv += dwRecv;
		}
	//}


	return 0;
}

int CmyAsynchSocket4Client::RecvData()
{
	DWORD dwFlags = 0;
	DWORD dwRecv;

	// 버퍼에 남아 있음. 이어받기 시도
	if(m_SockInfo.dwRemained > 0)
	{
		// 잔여량 앞으로 이전
		//{
			BYTE szTemp[MAX_PACKET_SIZE];
			int nProcessed = max(0,m_SockInfo.dwRecv-m_SockInfo.dwRemained);
			nProcessed = min(nProcessed,MAX_PACKET_SIZE);
			memcpy(szTemp,m_SockInfo.szBuf+nProcessed,m_SockInfo.dwRemained);
			memcpy(m_SockInfo.szBuf,szTemp,nProcessed);
		//}

		m_SockInfo.wsaBuf.buf = (char*)(m_SockInfo.szBuf + nProcessed);
		m_SockInfo.wsaBuf.len = MAX_PACKET_SIZE - nProcessed;
		m_SockInfo.dwRecv = nProcessed;
	}
	else 
	{
		m_SockInfo.wsaBuf.buf = (char*)m_SockInfo.szBuf;
		m_SockInfo.wsaBuf.len = MAX_PACKET_SIZE;
		m_SockInfo.dwRecv = 0;
	}

	//{
		dwFlags = 0;
		if (WSARecv(m_SockInfo.Socket, &(m_SockInfo.wsaBuf), 1, &dwRecv,&dwFlags, NULL, NULL) == SOCKET_ERROR)
		{
			DWORD dwError = WSAGetLastError();
			if (dwError != WSAEWOULDBLOCK)
			{
				SC_WSAERROR("WSARecv() was failed\n");
				Close();
				return -1;
			}
		} 
		else
		{
			m_SockInfo.dwRecv = dwRecv;
		}
	//}


	return 0;
}

bool CmyAsynchSocket4Client::SendBuffer(const TCHAR* pszBuffer,const DWORD& dwLen)
{
	return (0==SendData((DWORD)m_wsaEvent,(LPSTR)pszBuffer,(INT)dwLen));
}

int CmyAsynchSocket4Client::SendData(DWORD /*dwEvent*/,char* pszData, int nLen)
{
	DWORD dwSent;
	nLen = min(nLen,MAX_PACKET_SIZE);

	CopyMemory(m_SockInfo.szBuf,pszData,nLen);
	m_SockInfo.wsaBuf.buf = (char*)m_SockInfo.szBuf;
	m_SockInfo.wsaBuf.len = nLen;

	m_SockInfo.dwSent = 0;
	m_SockInfo.dwTotalToSend = nLen;

	if (WSASend(m_SockInfo.Socket, &(m_SockInfo.wsaBuf), 1, &dwSent, 0,NULL, NULL) == SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();
		if (dwError != WSAEWOULDBLOCK)
		{
			//SC_WSAERROR("WSASend() was failed\n");
			ErrorLog("(%s,%d) EC=[%ld] : Dest=[%s:%d]\n",__FUNCTION__,__LINE__,::WSAGetLastError(),m_szDestIP,m_usDestPort);
			Close();
			return -1;
		}
	}
	else
	{
		m_SockInfo.dwSent += dwSent;
	}

	if (m_SockInfo.dwSent >= m_SockInfo.dwTotalToSend)
	{
		m_SockInfo.dwSent = 0;
		m_SockInfo.dwTotalToSend = 0;
	}

	return 0;
}


int CmyAsynchSocket4Client::OnWritable(DWORD /*dwEvent*/)
{
	DWORD dwSent;

	m_bWritable = true;

	// 출력 버퍼가 사용가능할 때 송신한다.			
	if(m_SockInfo.dwSent < m_SockInfo.dwTotalToSend)
	{
		m_SockInfo.wsaBuf.buf = (char*)(m_SockInfo.szBuf + m_SockInfo.dwSent);
		m_SockInfo.wsaBuf.len = m_SockInfo.dwTotalToSend - m_SockInfo.dwSent;

		if (WSASend(m_SockInfo.Socket, &(m_SockInfo.wsaBuf), 1, &dwSent, 0,NULL, NULL) == SOCKET_ERROR)
		{
			DWORD dwError = WSAGetLastError();
			if (dwError != WSAEWOULDBLOCK)
			{
				SC_WSAERROR("WSASend() was failed\n");
				Close();
				return -1;
			}
			else if(dwError == WSAEWOULDBLOCK)
			{
				m_bWritable = false;
			}
		}
		else
		{
			m_SockInfo.dwSent += dwSent;
		}
	}

	if(m_SockInfo.dwSent >= m_SockInfo.dwTotalToSend)
	{
		m_SockInfo.dwSent = 0;
		m_SockInfo.dwTotalToSend = 0;
	}

	return 0;
}


int CmyAsynchSocket4Client::OnClose(DWORD /*Event*/)
{
	DebugPrint("[%d] Disconnected : Socket#=[%d]\n", GetTickCount(),m_SockInfo.Socket);

	Close();
	
	return 0;
}

///////////////////////////////////////////////////////////
// Send Queue 구현

bool CmyAsynchSocket4Client::SendBufferEx(const TCHAR* pszBuffer,const DWORD& dwLen)
{
	SEND_BUFFER* pSendData = new SEND_BUFFER;
	if(!pSendData)
	{
		return false;
	}

	int nLen = min(dwLen,MAX_PACKET_SIZE);
	memcpy(pSendData->Buffer,pszBuffer,nLen);
	pSendData->dwSent = 0;
	pSendData->dwTotalToSend = nLen;
	pSendData->wsaBuf.buf = pSendData->Buffer;
	pSendData->wsaBuf.len = 0;

	AddToSendQueue(pSendData);

	return true;
}

// Send Queue에 추가
void CmyAsynchSocket4Client::AddToSendQueue(SEND_BUFFER* pSendData)
{
#ifdef SC_THREAD_SAFE
	CGuardLock guard(&m_LockObject);
#endif
	m_SendQueue.push(pSendData);
}

// Loop에서 현재 쌓여있는 송신 패킷 전송
bool CmyAsynchSocket4Client::SendFromQueue()
{
	if(!m_bConnected)
		return false;

	//if(!m_bWritable)
	//	return;

	if(m_SendQueue.empty())
		return true;

#ifdef SC_THREAD_SAFE
	CGuardLock guard(&m_LockObject);
#endif
	SEND_BUFFER* pSendData = NULL;
	//if(!m_SendQueue.empty())
	{
		pSendData = m_SendQueue.front();

		int nMaxLoopCount = 10;		// 한번에 10개만 처리, 나머지는 다음 틱에서 처리

		while(pSendData)
		{
			DWORD dwSent = 0;
			pSendData->wsaBuf.buf = pSendData->Buffer + pSendData->dwSent;
			pSendData->wsaBuf.len = max(0,pSendData->dwTotalToSend - pSendData->dwSent);

			if(WSASend(m_SockInfo.Socket, &(pSendData->wsaBuf), 1, &dwSent, 0,NULL, NULL) == SOCKET_ERROR)
			{
				DWORD dwError = WSAGetLastError();
				if(dwError == WSAEWOULDBLOCK)
				{
					//WSASend returns SOCKET_ERROR and indicates error code WSA_IO_PENDING. In this case, lpNumberOfBytesSent is not updated.
					//pSendData->dwSent += dwSent;

					// 다음 틱에서 계속 전송
					m_bWritable = false;

					return true;
				}
				else
				{
					goto error;
				}
			}
			else
			{
				pSendData->dwSent += dwSent;

				if(pSendData->dwSent >= pSendData->dwTotalToSend)
				{
					m_SendQueue.pop();
					SAFE_DELETE(pSendData);
				}
				else
				{
					SC_WSAERROR("Warning!!! SendFromQueue() -- 이어보내기\n");
					continue;		//이어보내기
				}
			}

			if(--nMaxLoopCount < 0 || m_SendQueue.empty())
				break;

			pSendData = m_SendQueue.front();
		}
	}

	return true;
error:
	SC_WSAERROR("WSASend() was failed\n");
	Close();
	return false;
}


bool CmyAsynchSocket4Client::Process(DWORD dwCheckTimeout)
{
	if(!IsConnected() && m_dwConnectRetry_Period > 0)
	{
		if(m_dwConnectRetry_Period < GetTickCount() - m_dwConnectRetry_LastTime)
		{
			if(m_szDestIP[0] == NULL || m_usDestPort == 0)
			{
				m_dwConnectRetry_Period = 0;
				return false;
			}

			if(!Connect(m_szDestIP,m_usDestPort))
			{
				SC_WSAERROR("Connect() failed.\n");
			}
			m_dwConnectRetry_LastTime = GetTickCount();
		}
	}

	if(!CheckEvent(dwCheckTimeout))
		return false;
	if(!SendFromQueue())
		return false;

	return true;
}