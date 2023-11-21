// Util.cpp

#include "stdafx.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

// "%04d/%02d/%02d %02d:%02d:%02d"
char* FormattingDateTime(time_t tms,char* buf)
{
	struct tm * ptm;

	ptm = localtime(&tms);
	if(!ptm) return NULL;
	_stprintf(buf,"%04d/%02d/%02d %02d:%02d:%02d",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,
		ptm->tm_hour,ptm->tm_min,ptm->tm_sec);

	return buf;
}

// "%04d/%02d/%02d %02d:%02d:%02d"
char* GetCurDateTimeStr(char* buf,char* szFormat)
{
	struct tm * ptm;
	time_t tms;

	time(&tms);
	ptm = localtime(&tms);
	if(!ptm) return NULL;
	_stprintf(buf,szFormat,ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,
		ptm->tm_hour,ptm->tm_min,ptm->tm_sec);

	return buf;
}

// "%04d/%02d/%02d"
char* GetCurDateStr(char* buf,char* szFormat)
{
	struct tm * ptm;
	time_t tms;

	time(&tms);
	ptm = localtime(&tms);
	if(!ptm) return NULL;
	_stprintf(buf,szFormat,ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday);
	return buf;
}


TCHAR* GetCurDateTimeStrEx(TCHAR* pszStr,TCHAR* pszFormat)
{
	static TCHAR szFormat[100];

	if(pszFormat)
		memcpy(szFormat,pszFormat,_tcslen(pszFormat));
	return GetCurDateTimeStr((char*)pszStr,(char*)szFormat);
}

TCHAR* GetCurDateStrEx(TCHAR* pszStr,TCHAR* pszFormat)
{
	static TCHAR szFormat[100];

	if(pszFormat)
		memcpy(szFormat,pszFormat,_tcslen(pszFormat));
	return GetCurDateStr((char*)pszStr,(char*)szFormat);
}



void WriteStringToFile(char* msg)
{
	FILE* fp;
	char logfile[MAX_PATH],date[MAX_PATH];

	GetCurDateStr(date,"%04d%02d%02d");

//	_stprintf(logfile,TEXT("%s%s_%s.txt"),g_szRootPath,g_szServerName,date);
	fp = fopen(logfile,"a+");
	if(fp)
	{
		fputs(msg,fp);
		fclose(fp);
	}
}

/*

const char* pMsg = "test";
myReportEvent(&pMsg);
_exit(0);

*/

//
// MessageId: MSG_ERR_EXIST
// MessageText:
//  File %1 does not exist.
//
#define MSG_ERR_EXIST                         ((DWORD)0x00000004L)

void myReportEvent(LPCTSTR *szMsg)
{
	HANDLE h; 

	h = RegisterEventSource(NULL,  // uses local computer 
		TEXT("Application"));    // source name 
	if (h == NULL) 
	{
		DebugPrint("Could not register the event source.\n"); 
		return;
	}

	if (!ReportEvent(h,					// event log handle 
		EVENTLOG_INFORMATION_TYPE,  // event type 
		0,                    // category zero 
		MSG_ERR_EXIST,        // event identifier 
		NULL,                 // no user security identifier 
		1,                    // one substitution string 
		0,                    // no data 
		szMsg,                // pointer to string array 
		NULL))                // pointer to data 
	{
		DebugPrint(TEXT("Could not report the event.\n")); 
	}

	DeregisterEventSource(h); 
} 
#pragma warning (disable:4127)


TCHAR* GetCurrentDir(TCHAR* pFullPath)
{
	DWORD dwPathLength = 0;
	if(0 == (dwPathLength = ::GetModuleFileName(NULL, pFullPath, MAX_PATH + 1)))
	{
		SC_ERROR("GetCurrentDir\n");
		return NULL;
	}

	UINT uiIndex = dwPathLength - 1;
	while(TRUE)
	{
		if(TEXT('\\') == pFullPath[uiIndex])
		{
			pFullPath[uiIndex + 1] = TEXT('\0');
			return pFullPath;
		}

		if(uiIndex <= 0)
		{
			return NULL;
		}

		uiIndex--;
	}
}

bool IsExist(TCHAR* pszFile)
{
	FILE* pStream = _tfopen(pszFile,"r");
	if(pStream)
		fclose(pStream);
	return (pStream)?true:false;
}

// 기존 파일을 분석하여 순차적인 파일명을 생성한다.
bool MakeSequentialFileName(TCHAR* pszNewFileName,TCHAR* pszOldFileName)
{
	TCHAR szDrive[20],szDir[MAX_PATH],szFileName[MAX_PATH],szExt[20],szNewExt[20];
	TCHAR szNewFileName[MAX_PATH];

	_tcscpy(szNewFileName,pszOldFileName);

	int i = 0;
	int nTryCount = 0;

	// 새로운 파일명
	/*
	*	
		W002S200_20051118_Connect.log
		W002S200_20051118_Connect.log.1
		W002S200_20051118_Connect.log.2
		W002S200_20051118_Connect.log.3
		...
	*/
	while(IsExist(szNewFileName))
	{
		_tsplitpath(szNewFileName,szDrive,szDir,szFileName,szExt);

		// szExt 첫문자가 숫자가 아니면
		if(_istdigit(szExt[1]) == 0)
		{
			_stprintf(szNewExt,"%s.%d",szExt,++i);
		}
		else
		{
			i = _ttoi(&szExt[1]);
			_stprintf(szNewExt,".%d",++i);
		}

		_stprintf(szNewFileName,"%s%s%s%s",szDrive,szDir,szFileName,szNewExt);

		// 무한루프 방지
		if(++nTryCount > 100)
		{
			SC_ERROR2(szNewFileName," : ++nTryCount > 100\n");
			return false;
		}
	}

	_tcscpy(pszNewFileName,szNewFileName);

	return true;
}

TCHAR* MakeFullPath(TCHAR* pFullPath, TCHAR* pPartialPath)
{
	DWORD dwPathLength = 0;
	if(0 == (dwPathLength = ::GetModuleFileName(NULL, pFullPath, MAX_PATH + 1)))
	{
		SC_ERROR("MakeFullPath\n");
		return NULL;
	}

	UINT uiIndex = dwPathLength - 1;
	while(TRUE)
	{
		if(TEXT('\\') == pFullPath[uiIndex])
		{
			pFullPath[uiIndex + 1] = TEXT('\0');
			_tcscat(pFullPath, pPartialPath);
			return pFullPath;
		}

		if(uiIndex <= 0)
		{
			return NULL;
		}

		uiIndex--;
	}
}

#pragma warning(default:4127)

void MakeConfigureFullPathName(TCHAR* pszFullPathName)
{
	TCHAR szTemp[MAX_PATH];

	GetModuleFileName( GetModuleHandle( NULL ), szTemp, MAX_PATH );

	*(_tcsrchr( szTemp, '.') + 1) = 0;
	_tcscat(szTemp,"ini");
	_tcscpy(pszFullPathName,szTemp);
}

// 문자열 오른쪽 공백 제거
TCHAR* RTrim(TCHAR* pszStr)
{
	int nLen = (int)_tcslen(pszStr);
	if(nLen <= 0 ) return NULL;
	
	int i = nLen-1;
	while(i>=0 && IS_WHITESPACE(pszStr[i]))
	{
		pszStr[i] = '\0';
		--i;
	}
	return pszStr;
}

// 문자열 왼쪽 공백 제거
TCHAR* LTrim(TCHAR* pszStr)
{
	int nLen = (int)_tcslen(pszStr);
	if(nLen <= 0 ) return NULL;
	if(!IS_WHITESPACE(pszStr[0])) return pszStr;
	for(int i=0;i<nLen;++i)
	{
		if(!IS_WHITESPACE(pszStr[i]))
		{
			memcpy(&pszStr[0], &pszStr[i], nLen-i+1);
			return pszStr;
		}
	}
	pszStr[0] = '\0';
	return pszStr;
}

TCHAR* Trim(TCHAR* pszStr)
{
	TCHAR* pstr = RTrim(pszStr);
	return LTrim(pszStr);
}

/////////////////////////////////////////////////////////////////////////////////////////
//#if 0
//bool GetIPAddress(char* szHostname,char* ipaddress)
//{
//	SOCKADDR_IN addr;
//	LPHOSTENT   lpHostEnt;
//
//	ZeroMemory((PVOID)&addr, sizeof(addr));
//	addr.sin_family = PF_INET;
//	addr.sin_port = 0;
//	addr.sin_addr.s_addr = inet_addr(szHostname);
//	if(INADDR_NONE == addr.sin_addr.s_addr)
//	{
//		// the host name for the server is not in dot format,
//		// therefore try it just as a string
//		if (NULL == (lpHostEnt = gethostbyname(szHostname)))
//			DebugPrint("gethostbyname %d for %s", WSAGetLastError(), szHostname);
//		else
//		{
//			CopyMemory(&addr.sin_addr, lpHostEnt->h_addr_list[0],lpHostEnt->h_length);
//			_stprintf(ipaddress,TEXT("%d.%d.%d.%d"),addr.sin_addr.S_un.S_un_b.s_b1,
//				addr.sin_addr.S_un.S_un_b.s_b2,addr.sin_addr.S_un.S_un_b.s_b3,addr.sin_addr.S_un.S_un_b.s_b4);
//		}
//	}
//
//	return true;
//}
//
//void GetLocalIPAddress(char* ipaddress)
//{
//	char buf[50];
//	gethostname(buf,50);
//	GetIPAddress(buf,ipaddress);
//}
//#endif

// 연결된 peer의 정보(ip,port) 얻기
bool GetPeerInfo(SOCKET socket,TCHAR* pszIPAddress,u_short& usPort)
{
	if(!pszIPAddress)
	{
		assert(false && "pszIPAddress == NULL");
		return false;
	}

	struct sockaddr_in* pPeer;
	struct sockaddr peerName;
	int nameLen = sizeof(struct sockaddr);
	
	int iError = getpeername(socket,&peerName,&nameLen);
	if(iError == SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();
		DebugPrint("GetPeerInfo() : FAILED : Sock=[%d]\n",socket);
		return false;
	}
	pPeer = (struct sockaddr_in*)&peerName;
	_stprintf(pszIPAddress,"%d.%d.%d.%d",pPeer->sin_addr.S_un.S_un_b.s_b1
			,pPeer->sin_addr.S_un.S_un_b.s_b2,pPeer->sin_addr.S_un.S_un_b.s_b3
			,pPeer->sin_addr.S_un.S_un_b.s_b4);
	usPort = pPeer->sin_port;

	//DebugPrint("Peer=[%d.%d.%d.%d:%d]\n",pPeer->sin_addr.S_un.S_un_b.s_b1
	//	,pPeer->sin_addr.S_un.S_un_b.s_b2,pPeer->sin_addr.S_un.S_un_b.s_b3
	//	,pPeer->sin_addr.S_un.S_un_b.s_b4,pPeer->sin_port);

	return true;
}

void GetOSVersionInfo(OSVERSIONINFO& verInfo)
{
	//OSVERSIONINFO verInfo = {0};

	verInfo.dwOSVersionInfoSize = sizeof(verInfo);
	GetVersionEx(&verInfo);
	if( verInfo.dwPlatformId  != VER_PLATFORM_WIN32_NT ) 
	{
	}
}

bool GetComputerName(TCHAR* pszComputerName)
{
	TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1] ;
	DWORD computerNameLen = MAX_COMPUTERNAME_LENGTH ;

	if (::GetComputerName(computerName, &computerNameLen))
	{
		_tcscpy(pszComputerName,computerName);
		return true;
	}

	return false;
}


// pszStr내에 원하지 않는 문자가 있는지 확인
bool CheckMaskString(TCHAR* pszStr)
{
	if(!pszStr) return false;

	static const TCHAR* pszMask = TEXT(" ~!@#$%^&*()+-=|\\{}[]:;\"\'`<>.,?/");
	static const int nMask = (int)_tcslen(pszMask);

	int nID = (int)_tcslen(pszStr);

	for(int i=0;i<nMask;++i)
	{
		for(int j=0;j<nID;++j)
		{
			if(pszMask[i] == pszStr[j])
				return false;
		}
	}
	return true;
}


// MAC Address 얻기
#if 0

#include <Windows.h>
#include <Iphlpapi.h>
#include <assert.h>
#pragma comment(lib,"Iphlpapi.lib");

static void GetMACAddress()
{
	IP_ADAPTER_INFO adapterInfos[10];
	DWORD dwSize = sizeof(adapterInfos);
	DWORD dwStatus = ::GetAdaptersInfo(adapterInfos,&dwSize);

	assert(dwStatus == ERROR_SUCCESS);

	PIP_ADAPTER_INFO pAdapterInfo = adapterInfos;

	do {
		pAdapterInfo->Address;
		pAdapterInfo = pAdapterInfo->Next;

	}while(pAdapterInfo);
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 해당 드라이브의 사용가능 용량 얻기
//
//	LPCTSTR lpszPath = TEXT("c:\\");
//	DWORDLONG nFree = 0;
//
//	BOOL bOK = GetDiscFreeSpace(lpszPath,&nFree);
//
BOOL GetDiscFreeSpace(LPCTSTR lpszDrive, DWORDLONG* pnFree)
{
	ULARGE_INTEGER nTotalBytes, nTotalFreeBytes, nTotalAvailable;
	*pnFree = 0;

	if(GetDiskFreeSpaceEx(lpszDrive, &nTotalAvailable, &nTotalBytes, &nTotalFreeBytes))
	{
		*pnFree = nTotalAvailable.QuadPart;

		int megaBytes = nTotalAvailable.QuadPart / 1024 / 1024;

		return TRUE;
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Local/Public IP 얻기
//

#define MAX_IP_ADDRESSES	4

#include "iphlpapi.h"

//#pragma comment(lib,"IPhlpapi.lib")
//#pragma comment(lib,"ws2_32.lib")

/*
 *	
	#if 0
	//TCHAR pszIPs[4][20] = {0,};
	//int  nMaxEntries = 4;

	//GetIPs(pszIPs,nMaxEntries);

	//if(pszIPs)
	//	free(pszIPs);
	#endif

	#if 0
	TCHAR szLocalIP[20] = {0,};
	TCHAR szPublicIP[20] = {0,};

	GetIPs(szLocalIP,szPublicIP);

	printf("localIP=[%s] publicIP=[%s]\n",szLocalIP,szPublicIP);
	#endif
 */

// return value : IP Count
int GetIPs(TCHAR pszIPs[MAX_IP_ADDRESSES][20],int& nMaxEntries)
{
	MIB_IPADDRTABLE* pIPAddrTable = NULL;
	DWORD dwSize = 0;
	DWORD dwError = 0;

	GetIpAddrTable(NULL,&dwSize,0);
	pIPAddrTable = (MIB_IPADDRTABLE*)malloc(dwSize);
	if(!pIPAddrTable) return -1;

	dwError = GetIpAddrTable(pIPAddrTable,&dwSize,TRUE);
	if(NO_ERROR != dwError)
	{
		return -2;
	}

	TCHAR szIP[20] = {0,};
	in_addr ipAddr;
	int nEntries = max(0,pIPAddrTable->dwNumEntries);
	for(int i=0;i<nEntries;++i)
	{
		ipAddr.S_un.S_addr = pIPAddrTable->table[i].dwAddr;

		if(i < nMaxEntries)
			_tcscpy(szIP,inet_ntoa(ipAddr));
		//printf("%s , %d\n",szIP,pIPAddrTable->table[i].wType);
	}
	if(pIPAddrTable) free(pIPAddrTable);

	nMaxEntries = nEntries;

	return nEntries;
}

#pragma comment(lib,"Iphlpapi.lib")

// return value : IP Count
int GetIPs(in_addr ipAddr[MAX_IP_ADDRESSES])
{
	MIB_IPADDRTABLE* pIPAddrTable = NULL;
	DWORD dwSize = 0;
	DWORD dwError = 0;

	GetIpAddrTable(NULL,&dwSize,0);
	pIPAddrTable = (MIB_IPADDRTABLE*)malloc(dwSize);
	if(!pIPAddrTable) return -1;

	dwError = GetIpAddrTable(pIPAddrTable,&dwSize,TRUE);
	if(NO_ERROR != dwError)
	{
		return -2;
	}

	int nEntries = max(0,pIPAddrTable->dwNumEntries);
	for(int i=0;i<nEntries;++i)
	{
		ipAddr[i].S_un.S_addr = pIPAddrTable->table[i].dwAddr;
	}
	if(pIPAddrTable) free(pIPAddrTable);

	return nEntries;
}

void GetIPs(TCHAR* pszLocalIP,TCHAR* pszPublicIP)
{
	// Private Address
	// 10.0.0.0 ~ 10.255.255.255
	// 172.16.0.0 ~ 172.31.255.255
	// 192.168.0.0 ~ 192.168.255.255

	in_addr ipAddr[MAX_IP_ADDRESSES] = {0,};			// 0.0.0.0, 127.0.0.0, 10... , 61,...

	int nIPs = GetIPs(ipAddr);
	for(int i=0;i<nIPs;++i)
	{
		switch(ipAddr[i].S_un.S_un_b.s_b1) 
		{
		case 10:
			_tcscpy(pszLocalIP,inet_ntoa(ipAddr[i]));
			break;
		case 172:
			if(ipAddr[i].S_un.S_un_b.s_b2 >= 16 && ipAddr[i].S_un.S_un_b.s_b2 <=31)
			{
				_tcscpy(pszLocalIP,inet_ntoa(ipAddr[i]));
			}
			break;
		case 192:
			if(ipAddr[i].S_un.S_un_b.s_b2 == 168)
			{
				_tcscpy(pszLocalIP,inet_ntoa(ipAddr[i]));
			}
			break;
		default:
			{
				if(!(ipAddr[i].S_un.S_un_w.s_w1 == 127 && ipAddr[i].S_un.S_un_w.s_w2 == 256) && !(ipAddr[i].S_un.S_addr == 0))		// 127.0.0.1와 0.0.0.0 제외
					_tcscpy(pszPublicIP,inet_ntoa(ipAddr[i]));
			}
			break;
		}
	}
}


// Example usage:
//SetThreadName(-1, "Main thread");
//
void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName )
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = szThreadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try
	{
		RaiseException( 0x406D1388, 0,
			sizeof(info) / sizeof(DWORD),
			(DWORD*)&info );
	}
	__except( EXCEPTION_CONTINUE_EXECUTION ) {
	}
}
