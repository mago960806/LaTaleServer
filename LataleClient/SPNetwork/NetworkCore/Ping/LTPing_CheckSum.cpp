#include "StdAfx.h"
#include "LTPing.h"

//    This function calculates the 16-bit one's complement sum
//    of the supplied buffer (ICMP) header.
//
USHORT CLTPing::Checksum(USHORT *buffer, int size) 
{
	unsigned long cksum=0;

	while (size > 1) 
	{
		cksum += *buffer++;
		size -= sizeof(USHORT);
	}
	if (size) 
	{
		cksum += *(UCHAR*)buffer;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >>16);
	return (USHORT)(~cksum);
}

//    This routine sets the sequence number of the ICMP request packet.
//
void CLTPing::SetIcmpSequence(char *buf)
{
	ULONG    sequence=0;

	sequence = GetTickCount();

	if (m_iAddressFamily == AF_INET)
	{
		ICMP_HDR    *icmpv4=NULL;

		icmpv4 = (ICMP_HDR *)buf;

		icmpv4->icmp_sequence = (USHORT)sequence;
	}
	else if (m_iAddressFamily == AF_INET6)
	{
		ICMPV6_HDR          *icmpv6=NULL;
		ICMPV6_ECHO_REQUEST *req6=NULL;

		icmpv6 = (ICMPV6_HDR *)buf;
		req6   = (ICMPV6_ECHO_REQUEST *)(buf + sizeof(ICMPV6_HDR));

		req6->icmp6_echo_sequence = (USHORT)sequence;
	}
}

//    This routine computes the ICMP6 checksum which includes the pseudo
//    header of the IPv6 header (see RFC2460 and RFC2463). The one difficulty
//    here is we have to know the source and destination IPv6 addresses which
//    will be contained in the IPv6 header in order to compute the checksum.
//    To do this we call the SIO_ROUTING_INTERFACE_QUERY ioctl to find which
//    local interface for the outgoing packet.
//
USHORT CLTPing::ComputeIcmp6PseudoHeaderChecksum(char *icmppacket, int icmplen, struct addrinfo *dest)
{
	SOCKADDR_STORAGE localif;
	DWORD            bytes;
	char             tmp[MAX_RECV_BUF_LEN] = {'\0'},*ptr=NULL,proto=0;
	int              rc, total, length, i;

	// Find out which local interface for the destination
	rc = WSAIoctl(
		m_s,
		SIO_ROUTING_INTERFACE_QUERY,
		dest->ai_addr,
		(DWORD) dest->ai_addrlen,
		(SOCKADDR *) &localif,
		(DWORD) sizeof(localif),
		&bytes,
		NULL,
		NULL
		);
	if (rc == SOCKET_ERROR)
	{
		//fprintf(stderr, "WSAIoctl failed: %d\n", WSAGetLastError());
		return 0xFFFF;
	}

	// We use a temporary buffer to calculate the pseudo header. 
	ptr = tmp;
	total = 0;

	// Copy source address
	memcpy(ptr, &((SOCKADDR_IN6 *)&localif)->sin6_addr, sizeof(struct in6_addr));
	ptr   += sizeof(struct in6_addr);
	total += sizeof(struct in6_addr);

	// Copy destination address
	memcpy(ptr, &((SOCKADDR_IN6 *)dest->ai_addr)->sin6_addr, sizeof(struct in6_addr));
	ptr   += sizeof(struct in6_addr);
	total += sizeof(struct in6_addr);

	// Copy ICMP packet length
	length = htonl(icmplen);

	memcpy(ptr, &length, sizeof(length));
	ptr   += sizeof(length);
	total += sizeof(length);

	// Zero the 3 bytes
	memset(ptr, 0, 3);
	ptr   += 3;
	total += 3;

	// Copy next hop header
	proto = IPPROTO_ICMP6;

	memcpy(ptr, &proto, sizeof(proto));
	ptr   += sizeof(proto);
	total += sizeof(proto);

	// Copy the ICMP header and payload
	memcpy(ptr, icmppacket, icmplen);
	ptr   += icmplen;
	total += icmplen;

	for(i=0; i < icmplen%2 ;i++)
	{
		*ptr = 0;
		ptr++;
		total++;
	}

	return Checksum((USHORT *)tmp, total);
}

//    This routine computes the checksum for the ICMP request. For IPv4 its
//    easy, just compute the checksum for the ICMP packet and data. For IPv6,
//    its more complicated. The pseudo checksum has to be computed for IPv6
//    which includes the ICMP6 packet and data plus portions of the IPv6
//    header which is difficult since we aren't building our own IPv6
//    header.
//
void CLTPing::ComputeIcmpChecksum(char *buf, int packetlen, struct addrinfo *dest)
{
	if (m_iAddressFamily == AF_INET)
	{
		ICMP_HDR    *icmpv4=NULL;

		icmpv4 = (ICMP_HDR *)buf;
		icmpv4->icmp_checksum = 0;
		icmpv4->icmp_checksum = Checksum((USHORT *)buf, packetlen);
	}
	else if (m_iAddressFamily == AF_INET6)
	{
		ICMPV6_HDR  *icmpv6=NULL;

		icmpv6 = (ICMPV6_HDR *)buf;
		icmpv6->icmp6_checksum = 0;
		icmpv6->icmp6_checksum = ComputeIcmp6PseudoHeaderChecksum(buf,packetlen,dest);
	}
}
