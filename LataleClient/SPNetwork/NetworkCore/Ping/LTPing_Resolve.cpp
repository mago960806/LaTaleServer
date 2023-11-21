#include "StdAfx.h"
#include "LTPing.h"

extern void DebugPrint(const TCHAR* lpOutputString, ...);

// Description:
//    This routine resolves the specified address and returns a list of addrinfo
//    structure containing SOCKADDR structures representing the resolved addresses.
//    Note that if 'addr' is non-NULL, then getaddrinfo will resolve it whether
//    it is a string listeral address or a hostname.
//
struct addrinfo* CLTPing::ResolveAddress(char *addr, char *port, int af, int type, int proto)
{
	struct addrinfo hints,*res = NULL;
	int             rc;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags  = ((addr) ? 0 : AI_PASSIVE);
	hints.ai_family = af;
	hints.ai_socktype = type;
	hints.ai_protocol = proto;

	rc = getaddrinfo(addr,port,&hints,&res);
	if (rc != 0)
	{
		DebugPrint("Invalid address %s, getaddrinfo failed: %d\n", addr, rc);
		return NULL;
	}
	return res;
}


//    This routine takes a SOCKADDR structure and its length and prints
//    converts it to a string representation. This string is printed
//    to the console via stdout.
//
int CLTPing::PrintAddress(SOCKADDR *sa, int salen)
{
	char    host[NI_MAXHOST],serv[NI_MAXSERV];
	int     hostlen = NI_MAXHOST,servlen = NI_MAXSERV,rc;

	rc = getnameinfo(sa,salen,host,hostlen,serv,servlen,NI_NUMERICHOST | NI_NUMERICSERV);
	if (rc != 0)
	{
		DebugPrint("%s: getnameinfo failed: %d\n", __FILE__, rc);
		return rc;
	}

	// If the port is zero then don't print it
	if (strcmp(serv, "0") != 0)
	{
		if (sa->sa_family == AF_INET)
			DebugPrint("[%s]:%s", host, serv);
		else
			DebugPrint("%s:%s", host, serv);
	}
	else
		DebugPrint("%s", host);

	return NO_ERROR;
}