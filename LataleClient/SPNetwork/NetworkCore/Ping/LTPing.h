#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <strsafe.h>
#include "iphdr.h"


#define DEFAULT_DATA_SIZE      32       // default data size
#define DEFAULT_SEND_COUNT     4        // number of ICMP requests to send
#define DEFAULT_RECV_TIMEOUT   6000     // six second
#define DEFAULT_TTL            128

#define MAX_RECV_BUF_LEN       4096  



class CLTPing
{
public:
	CLTPing(void);
	virtual ~CLTPing(void);


	bool Init();
	void Exit();
	void Stop();

	// -2(FAILED),-1(TIMEOUT),0>(OK)
	int Ping(char* pszDest,DWORD dwTimeout=DEFAULT_RECV_TIMEOUT);
	DWORD GetLastPingTime(){return m_dwTime;}

protected:

	int		PostRecvfrom();

	struct addrinfo *ResolveAddress(char *addr, char *port, int af, int type, int proto);
	int		PrintAddress(SOCKADDR *sa, int salen);


	bool	SetTTL(int iTTL);
	bool	InitICMPHeaderInfo();

	void	InitIcmpHeader(char *buf, int datasize);
	int		InitIcmp6Header(char *buf, int datasize);

	USHORT	Checksum(USHORT *buffer, int size);

	void	SetIcmpSequence(char *buf);
	USHORT	ComputeIcmp6PseudoHeaderChecksum(char *icmppacket, int icmplen, struct addrinfo *dest);
	void	ComputeIcmpChecksum(char *buf, int packetlen, struct addrinfo *dest);

	void	PrintPayload(char *buf, int bytes);

public:

	// fot testing
	bool Process(char* pszDest);

private:
	int		m_iAddressFamily;
	int		m_iProtocol;
	int		m_iTTL;
	int		m_iDataSize;
	BOOL	m_bRecordRoute;
	int		m_iRecvBufLen;    // Length of received packets.
	int		m_iFromLen;
	DWORD	m_dwTime;
	int		m_iPacketLen;

	struct addrinfo*	m_pDest;
	struct addrinfo*	m_pLocal;
	WSAOVERLAPPED		m_olRecv;
	SOCKADDR_STORAGE	m_From;


	char*	m_pICMPBuf;
	char	m_szRecvBuf[MAX_RECV_BUF_LEN];

	SOCKET m_s;
};


/*
 *	

int _tmain(int argc, _TCHAR* argv[])
{

	CLTPing ping;

	if(ping.Init())
	{
		int iTime = 0;
		for(int i=0;i<4;++i)
		{
			iTime = ping.Ping("www.latale.com",1000);
			if(iTime >= 0)
			{
				printf("pingtime : %d \n",iTime);
			}
			else if(iTime == -1)
				printf("timeout\n");
			else 
				printf("system error\n");

			Sleep(1000);
		}

		ping.Exit();
	}

	return 0;
}

 */