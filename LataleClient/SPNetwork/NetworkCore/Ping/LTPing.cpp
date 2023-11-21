#include "StdAfx.h"
#include "LTPing.h"

#ifdef _LATALE_CLIENT
#include "DebugPrint.h"
#else 
void DebugPrint(const TCHAR* lpOutputString, ...)
{
	va_list args;
	char tmp[1000];

	va_start(args, lpOutputString);
	vsprintf(tmp, lpOutputString, args);
	va_end(args);

	OutputDebugString(tmp);
}
#endif


CLTPing::CLTPing(void) : 
	m_iAddressFamily(AF_INET)		// AF_INET6
	,m_iProtocol(IPPROTO_ICMP)		// IPPROTO_ICMP6
	,m_iTTL(DEFAULT_TTL)
	,m_iDataSize(DEFAULT_DATA_SIZE)
	,m_bRecordRoute(FALSE)
	,m_s(INVALID_SOCKET)
	,m_pLocal(NULL)
	,m_pDest(NULL)
	,m_pICMPBuf(NULL)
	,m_iRecvBufLen(MAX_RECV_BUF_LEN)
	,m_iFromLen(0)
	,m_dwTime(0)
	,m_iPacketLen(0)
{
}

CLTPing::~CLTPing(void)
{
}

bool CLTPing::Init()
{
	WSADATA wsd;
	int rc = 0;

	ZeroMemory(m_szRecvBuf,sizeof(m_szRecvBuf));

	m_olRecv.hEvent = WSA_INVALID_EVENT;

	if((rc = WSAStartup(MAKEWORD(2,2), &wsd)) != 0)
	{
		return false;
	}

	// Resolve addresses
	m_pLocal = ResolveAddress(NULL,"0",m_iAddressFamily,0,0);
	if(m_pLocal == NULL)
	{
		DebugPrint("Unable to obtain the bind address!\n");
		return false;
	}

	m_s = socket(m_iAddressFamily, SOCK_RAW, m_iProtocol);
	if(m_s == INVALID_SOCKET) 
	{
		DebugPrint("socket() failed\n");
		return false;
	}

	if(!SetTTL(m_iTTL))
	{
		DebugPrint("SetTTL() failed\n");
		return false;
	}

	if(!InitICMPHeaderInfo())
	{
		DebugPrint("InitICMPHeaderInfo() failed\n");
		return false;
	}

	// Bind the socket -- need to do this since we post a receive first
	rc = bind(m_s, m_pLocal->ai_addr, (int)m_pLocal->ai_addrlen);
	if(rc == SOCKET_ERROR)
	{
		DebugPrint("bind failed: %d\n", WSAGetLastError());
		return false;
	}

	// Setup the receive operation
	memset(&m_olRecv, 0, sizeof(m_olRecv));
	m_olRecv.hEvent = WSACreateEvent();
	if(m_olRecv.hEvent == WSA_INVALID_EVENT)
	{
		DebugPrint("WSACreateEvent failed: %d\n", WSAGetLastError());
		return false;
	}

	// Post the first overlapped receive
	if(NO_ERROR != PostRecvfrom())
	{
		return false;
	}

	return true;
}

void CLTPing::Exit(void)
{

	Stop();

	WSACleanup();
}

void CLTPing::Stop()
{
	if(m_pLocal) 
	{
		freeaddrinfo(m_pLocal);
		m_pLocal = NULL;
	}
	if(m_pDest) 
	{
		freeaddrinfo(m_pDest);
		m_pDest = NULL;
	}

	if(m_s != INVALID_SOCKET) 
	{
		closesocket(m_s);
		m_s = INVALID_SOCKET;
	}

	if(m_olRecv.hEvent != WSA_INVALID_EVENT)
	{
		WSACloseEvent(m_olRecv.hEvent);
		m_olRecv.hEvent = WSA_INVALID_EVENT;
	}

	if (m_pICMPBuf)
	{
		HeapFree(GetProcessHeap(), 0, m_pICMPBuf);
		m_pICMPBuf = NULL;
	}
}

int CLTPing::Ping(char* pszDest,DWORD dwTimeout /*=DEFAULT_RECV_TIMEOUT*/)
{
	int rc;
	DWORD   flags,bytes;


	if(m_s == INVALID_SOCKET) 
	{
		DebugPrint("INVALID_SOCKET\n");
		return -2;
	}

	if(m_pDest) 
		freeaddrinfo(m_pDest);

	// Resolve the destination address
	m_pDest = ResolveAddress(pszDest,"0",m_iAddressFamily,0,0);
	if(m_pDest == NULL)
	{
		DebugPrint("bad name %s\n", pszDest);
		return -2;
	}
	m_iAddressFamily = m_pDest->ai_family;

	if (m_iAddressFamily == AF_INET)
		m_iProtocol = IPPROTO_ICMP;
	else if (m_iAddressFamily == AF_INET6)
		m_iProtocol = IPPROTO_ICMP6;

	// Start sending the ICMP requests

	if(!m_pICMPBuf)
	{
		DebugPrint("ICMP Buff Null\n");
		return -2;
	}
	
	SetIcmpSequence(m_pICMPBuf);		// Set the sequence number and compute the checksum
	ComputeIcmpChecksum(m_pICMPBuf, m_iPacketLen, m_pDest);

	m_dwTime = GetTickCount();

	rc = sendto(m_s,m_pICMPBuf,m_iPacketLen,0,m_pDest->ai_addr,(int)m_pDest->ai_addrlen);
	if (rc == SOCKET_ERROR)
	{
		DebugPrint("sendto failed: %d\n", WSAGetLastError());
		return -2;
	}

	// Waite for a response
	rc = WaitForSingleObject((HANDLE)m_olRecv.hEvent, dwTimeout);
	if (rc == WAIT_FAILED)
	{
		DebugPrint("WaitForSingleObject failed: %d\n", GetLastError());
		return -2;
	}
	else if(rc == WAIT_TIMEOUT)
	{
		DebugPrint("Request timed out.\n");
		return -1;
	}
	else
	{
		rc = WSAGetOverlappedResult(m_s,&m_olRecv,&bytes,FALSE,&flags);
		if (rc == FALSE)
		{
			DebugPrint("WSAGetOverlappedResult failed: %d\n", WSAGetLastError());
			return -2;
		}
		m_dwTime = GetTickCount() - m_dwTime;

		WSAResetEvent(m_olRecv.hEvent);

		PostRecvfrom();
	}

	return m_dwTime;
}


bool CLTPing::SetTTL(int iTTL)
{
	int optlevel = 0,option = 0,rc;

	rc = NO_ERROR;
	if(m_iAddressFamily == AF_INET)
	{
		optlevel = IPPROTO_IP;
		option   = IP_TTL;
	}
	else if(m_iAddressFamily == AF_INET6)
	{
		optlevel = IPPROTO_IPV6;
		option   = IPV6_UNICAST_HOPS;
	}
	else
	{
		rc = SOCKET_ERROR;
	}

	if(rc == NO_ERROR)
	{
		rc = setsockopt(m_s,optlevel,option,(char *)&m_iTTL,sizeof(m_iTTL));
	}

	if(rc == SOCKET_ERROR)
	{
		DebugPrint("SetTtl: setsockopt failed: %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

bool CLTPing::InitICMPHeaderInfo()
{
	m_iPacketLen = 0;

	// Figure out the size of the ICMP header and payload
	if(m_iAddressFamily == AF_INET)
		m_iPacketLen += sizeof(ICMP_HDR);
	else if(m_iAddressFamily == AF_INET6)
		m_iPacketLen += sizeof(ICMPV6_HDR) + sizeof(ICMPV6_ECHO_REQUEST);

	// Add in the data size
	m_iPacketLen += m_iDataSize;

	// Allocate the buffer that will contain the ICMP request
	m_pICMPBuf = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, m_iPacketLen);
	if(m_pICMPBuf == NULL)
	{
		DebugPrint("HeapAlloc failed: %d\n", GetLastError());
		return false;
	}

	// Initialize the ICMP headers
	if (m_iAddressFamily == AF_INET)
	{
		if (m_bRecordRoute)
		{
			int rc = 0;
			IPV4_OPTION_HDR ipopt;

			// Setup the IP option header to go out on every ICMP packet
			ZeroMemory(&ipopt, sizeof(ipopt));
			ipopt.opt_code = IP_RECORD_ROUTE; // record route option
			ipopt.opt_ptr  = 4;               // point to the first addr offset
			ipopt.opt_len  = 39;              // length of option header

			rc = setsockopt(m_s, IPPROTO_IP, IP_OPTIONS,(char *)&ipopt, sizeof(ipopt));
			if(rc == SOCKET_ERROR)
			{
				DebugPrint("setsockopt(IP_OPTIONS) failed: %d\n", WSAGetLastError());
				return false;
			}
		}

		InitIcmpHeader(m_pICMPBuf, m_iDataSize);
	}
	else if (m_iAddressFamily == AF_INET6)
	{
		InitIcmp6Header(m_pICMPBuf, m_iDataSize);
	}

	return true;
}

// Helper function to fill in various stuff in our ICMP request.
void CLTPing::InitIcmpHeader(char *buf, int datasize)
{
	ICMP_HDR   *icmp_hdr=NULL;
	char       *datapart=NULL;

	icmp_hdr = (ICMP_HDR *)buf;
	icmp_hdr->icmp_type     = ICMPV4_ECHO_REQUEST_TYPE;        // request an ICMP echo
	icmp_hdr->icmp_code     = ICMPV4_ECHO_REQUEST_CODE;
	icmp_hdr->icmp_id       = (USHORT)GetCurrentProcessId();
	icmp_hdr->icmp_checksum = 0;
	icmp_hdr->icmp_sequence = 0;

	datapart = buf + sizeof(ICMP_HDR);

	// Place some data in the buffer.
	memset(datapart, 'E', datasize);
}

//    Initialize the ICMP6 header as well as the echo request header.
int CLTPing::InitIcmp6Header(char *buf, int datasize)
{
	ICMPV6_HDR          *icmp6_hdr=NULL;
	ICMPV6_ECHO_REQUEST *icmp6_req=NULL;
	char                *datapart=NULL;

	// Initialize the ICMP6 headerf ields
	icmp6_hdr = (ICMPV6_HDR *)buf;
	icmp6_hdr->icmp6_type     = ICMPV6_ECHO_REQUEST_TYPE;
	icmp6_hdr->icmp6_code     = ICMPV6_ECHO_REQUEST_CODE;
	icmp6_hdr->icmp6_checksum = 0;

	// Initialize the echo request fields
	icmp6_req = (ICMPV6_ECHO_REQUEST *)(buf + sizeof(ICMPV6_HDR));
	icmp6_req->icmp6_echo_id       = (USHORT)GetCurrentProcessId();
	icmp6_req->icmp6_echo_sequence = 0;

	datapart = (char *)buf + sizeof(ICMPV6_HDR) + sizeof(ICMPV6_ECHO_REQUEST);

	memset(datapart, '#', datasize);

	return (sizeof(ICMPV6_HDR) + sizeof(ICMPV6_ECHO_REQUEST));
}


// This routine posts an overlapped WSARecvFrom on the raw socket.
int CLTPing::PostRecvfrom()
{
	WSABUF  wbuf;
	DWORD   flags,bytes;
	int     rc;

	wbuf.buf = m_szRecvBuf;
	wbuf.len = m_iRecvBufLen;

	flags = 0;
	m_iFromLen = sizeof(m_From);

	rc = WSARecvFrom(m_s,&wbuf,1,&bytes,&flags, (SOCKADDR *)&m_From,&m_iFromLen,&m_olRecv,NULL);
	if(rc == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			DebugPrint("WSARecvFrom failed: %d\n", WSAGetLastError());
			return SOCKET_ERROR;
		}
	}
	return NO_ERROR;
}

bool CLTPing::Process(char* pszDest)
{
	int rc;
	int i;
	DWORD   flags,bytes;

	// Resolve the destination address
	m_pDest = ResolveAddress(pszDest,"0",m_iAddressFamily,0,0);
	if(m_pDest == NULL)
	{
		DebugPrint("bad name %s\n", pszDest);
		return false;
	}
	m_iAddressFamily = m_pDest->ai_family;

	if (m_iAddressFamily == AF_INET)
		m_iProtocol = IPPROTO_ICMP;
	else if (m_iAddressFamily == AF_INET6)
		m_iProtocol = IPPROTO_ICMP6;


	// Start sending the ICMP requests
	for(i=0; i < DEFAULT_SEND_COUNT ;i++)
	{
		// Set the sequence number and compute the checksum
		SetIcmpSequence(m_pICMPBuf);
		ComputeIcmpChecksum(m_pICMPBuf, m_iPacketLen, m_pDest);

		m_dwTime = GetTickCount();

		rc = sendto(m_s,m_pICMPBuf,m_iPacketLen,0,m_pDest->ai_addr,(int)m_pDest->ai_addrlen);
		if (rc == SOCKET_ERROR)
		{
			DebugPrint("sendto failed: %d\n", WSAGetLastError());
			return false;
		}

		// Waite for a response
		rc = WaitForSingleObject((HANDLE)m_olRecv.hEvent, DEFAULT_RECV_TIMEOUT);
		if (rc == WAIT_FAILED)
		{
			DebugPrint("WaitForSingleObject failed: %d\n", GetLastError());
			return false;
		}
		else if(rc == WAIT_TIMEOUT)
		{
			DebugPrint("Request timed out.\n");
			return false;
		}
		else
		{
			rc = WSAGetOverlappedResult(m_s,&m_olRecv,&bytes,FALSE,&flags);
			if (rc == FALSE)
			{
				DebugPrint("WSAGetOverlappedResult failed: %d\n", WSAGetLastError());
			}
			m_dwTime = GetTickCount() - m_dwTime;

			WSAResetEvent(m_olRecv.hEvent);

			DebugPrint("Reply from ");
			
			PrintAddress((SOCKADDR *)&m_From, m_iFromLen);

			if(m_dwTime == 0)
				DebugPrint(": bytes=%d time<1ms TTL=%d\n", m_iDataSize, m_iTTL);
			else
				DebugPrint(": bytes=%d time=%dms TTL=%d\n", m_iDataSize, m_dwTime, m_iTTL);

			PrintPayload(m_szRecvBuf, bytes);

			if(i < DEFAULT_SEND_COUNT - 1)
			{
				PostRecvfrom();
			}
		}
		Sleep(1);
	}

	return true;
}




//
// Function: PrintPayload
// 
// Description:
//    This routine is for IPv4 only. It determines if there are any IP options
//    present (by seeing if the IP header length is greater than 20 bytes) and
//    if so it prints the IP record route options.
//
void CLTPing::PrintPayload(char *buf, int bytes)
{
	int hdrlen=0,routes=0,i;

	UNREFERENCED_PARAMETER(bytes);

	if(m_iAddressFamily == AF_INET)
	{
		SOCKADDR_IN      hop;
		IPV4_OPTION_HDR *v4opt=NULL;
		IPV4_HDR        *v4hdr=NULL;

		hop.sin_family = (USHORT)m_iAddressFamily;
		hop.sin_port   = 0;

		v4hdr = (IPV4_HDR *)buf;
		hdrlen = (v4hdr->ip_verlen & 0x0F) * 4;

		// If the header length is greater than the size of the basic IPv4
		//    header then there are options present. Find them and print them.
		if(hdrlen > sizeof(IPV4_HDR))
		{
			v4opt = (IPV4_OPTION_HDR *)(buf + sizeof(IPV4_HDR));
			routes = (v4opt->opt_ptr / sizeof(ULONG)) - 1;
			for(i=0; i < routes ;i++)
			{
				hop.sin_addr.s_addr = v4opt->opt_addr[i];

				// Print the route
				if (i == 0)
					DebugPrint("    Route: ");
				else
					DebugPrint("           ");

				PrintAddress((SOCKADDR *)&hop, sizeof(hop));

				if (i < routes-1)
					DebugPrint(" ->\n");
				else
					DebugPrint("\n");
			}
		}
	}
	return;
}
