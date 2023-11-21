///
/// Service.cpp
///
/// 괹괹굁궻긖깛긵깑귩긹?긚궸긇긚??귽긛
/// DDK\src\general\cancel\exe\install.c
///

#include "stdafx.h"

#ifdef _LATALE_CLIENT
#include "SPCoreDef.h"			// SPCore에 관한 헤더 묶음.#else 
#endif

#include <winsvc.h>
#include "Service.h"

namespace _Service_ns_ {


///
/// 볙븫듫릶
///

static void ClearError()
{
	Service::iLibErr = 0;
	Service::iSysErr = 0;
}

static void SetError(DWORD iLibErr, DWORD iSysErr)
{
	Service::iLibErr = iLibErr;
	Service::iSysErr = iSysErr;
}

static BOOL DoCreate(SC_HANDLE schSCManager, PSTR szDriverName, PSTR szDriverFile, PSTR szDisplayName)
{
	SC_HANDLE schService = ::CreateService(
			schSCManager,           // handle of service control manager database
			szDriverName,           // address of name of service to start
			szDisplayName,          // address of display name
			SERVICE_ALL_ACCESS,     // type of access to service
			SERVICE_KERNEL_DRIVER,  // type of service
			SERVICE_DEMAND_START,   // when to start service
			SERVICE_ERROR_NORMAL,   // severity if service fails to start
			szDriverFile,           // address of name of binary file
			NULL,                   // service does not belong to a group
			NULL,                   // no tag requested
			NULL,                   // no dependency names
			NULL,                   // use LocalSystem account
			NULL                    // no password for service account
			);
	if (!schService) {
		DWORD iSysErr = ::GetLastError();
		BOOL bRet = (iSysErr == ERROR_SERVICE_EXISTS);
		if (!bRet) SetError(2, iSysErr);
		return bRet;
	}
	::CloseServiceHandle(schService);
	return TRUE;
}

static BOOL DoStart(SC_HANDLE schSCManager, PSTR szDriverName)
{
	SC_HANDLE schService = ::OpenService(schSCManager, szDriverName, SERVICE_ALL_ACCESS);
	if (!schService) {
		SetError(3, ::GetLastError());
		return FALSE;
	}
	BOOL bRet = ::StartService(schService, 0, NULL);
	if (!bRet) {
		DWORD iSysErr = ::GetLastError();
		bRet = (iSysErr == ERROR_SERVICE_ALREADY_RUNNING);
		if (!bRet) SetError(4, iSysErr);
	}
	::CloseServiceHandle(schService);
	return bRet;
}

static BOOL DoStop(SC_HANDLE schSCManager, PSTR szDriverName)
{
	SC_HANDLE schService = ::OpenService(schSCManager, szDriverName, SERVICE_ALL_ACCESS);
	if (!schService) {
		SetError(3, ::GetLastError());
		return FALSE;
	}
	SERVICE_STATUS dServiceStatus;
	BOOL bRet = ::ControlService(schService, SERVICE_CONTROL_STOP, &dServiceStatus);
	if (!bRet) {
		DWORD iSysErr = ::GetLastError();
		bRet = (iSysErr == ERROR_SERVICE_NOT_ACTIVE);
		if (!bRet) SetError(5, iSysErr);
	}
	::CloseServiceHandle(schService);
	return bRet;
}

static BOOL DoDelete(SC_HANDLE schSCManager, PSTR szDriverName)
{
	SC_HANDLE schService = ::OpenService(schSCManager, szDriverName, SERVICE_ALL_ACCESS);
	if (!schService) {
		SetError(3, ::GetLastError());
		return FALSE;
	}
	BOOL bRet = ::DeleteService(schService);
	if (!bRet) {
		SetError(6, ::GetLastError());
	}
	::CloseServiceHandle(schService);
	return bRet;
}

static BOOL DoIsRun(SC_HANDLE schSCManager, PSTR szDriverName)
{
	SC_HANDLE schService = ::OpenService(schSCManager, szDriverName, SERVICE_ALL_ACCESS);
	if (!schService) {
		SetError(3, ::GetLastError());
		return FALSE;
	}
	SERVICE_STATUS dServiceStatus;
	BOOL bRet = ::QueryServiceStatus(schService, &dServiceStatus);
	if (!bRet) {
		SetError(7, ::GetLastError());
	}
	if (bRet) {
		bRet = (dServiceStatus.dwCurrentState == SERVICE_RUNNING);
	}
	::CloseServiceHandle(schService);
	return bRet;
}

static BOOL DoQuery(SC_HANDLE schSCManager, PSTR szDriverName)
{
	SC_HANDLE schService = ::OpenService(schSCManager, szDriverName, SERVICE_ALL_ACCESS);
	if (!schService) {
		SetError(3, ::GetLastError());
		return FALSE;
	}
	static CHAR acBuff[1024]; DWORD iNeedSize;
	LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)acBuff;
	BOOL bRet = ::QueryServiceConfig(schService, pServiceConfig, sizeof(acBuff), &iNeedSize);
	if (!bRet) {
		SetError(8, ::GetLastError());
	}
	if (bRet) {
		szDriverFileQry  = pServiceConfig->lpBinaryPathName;
		szDisplayNameQry = pServiceConfig->lpDisplayName;
	}
	::CloseServiceHandle(schService);
	return bRet;
}


///
/// 둖븫빾릶
///

DWORD iLibErr = 0;
DWORD iSysErr = 0;
PSTR  szDriverFileQry  = NULL;
PSTR  szDisplayNameQry = NULL;


///
/// 둖븫듫릶
///

BOOL DriverLoad(PSTR szDriverName, PSTR szDriverFile, PSTR szDisplayName)
{
	ClearError();
	SC_HANDLE schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!schSCManager) {
		SetError(1, ::GetLastError());
		return FALSE;
	}
	BOOL bRet = TRUE;
	if (bRet) bRet = DoCreate(schSCManager, szDriverName, szDriverFile, szDisplayName);
	if (bRet) bRet = DoStart (schSCManager, szDriverName);
	::CloseServiceHandle(schSCManager);
	return bRet;
}

BOOL DriverUnload(PSTR szDriverName)
{
	ClearError();
	SC_HANDLE schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!schSCManager) {
		SetError(1, ::GetLastError());
		return FALSE;
	}
	BOOL bRet = TRUE;
	if (bRet) bRet = DoStop  (schSCManager, szDriverName);
	if (bRet) bRet = DoDelete(schSCManager, szDriverName);
	::CloseServiceHandle(schSCManager);
	return bRet;
}

BOOL DriverIsRun(PSTR szDriverName)
{
	ClearError();
	SC_HANDLE schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!schSCManager) {
		SetError(1, ::GetLastError());
		return FALSE;
	}
	BOOL bRet = TRUE;
	if (bRet) bRet = DoIsRun(schSCManager, szDriverName);
	::CloseServiceHandle(schSCManager);
	return bRet;
}

BOOL DriverQuery(PSTR szDriverName)
{
	ClearError();
	SC_HANDLE schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!schSCManager) {
		SetError(1, ::GetLastError());
		return FALSE;
	}
	BOOL bRet = TRUE;
	if (bRet) bRet = DoQuery(schSCManager, szDriverName);
	::CloseServiceHandle(schSCManager);
	return bRet;
}


} // namespace _Service_ns_