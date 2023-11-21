// LataleCipherUtil.cpp

#include "StdAfx.h"
#include <tchar.h>
#include <stdio.h>
#include <Windows.h>


#include "LataleCipherDefines.h"

int g_iCipherLibMode = DEFAULT_SECURITY_KEY;
bool g_bUseEncryption = false;

#ifdef USING_LATALE_CIPHER

#include "LataleCipher.h"

#define USING_EXTERN_C		extern "C" 

#ifdef LATALE_CIPHER_DLL


HMODULE g_hModule = NULL;

CIPHER_INIT			g_pfnInit		= NULL;
CIPHER_EXIT			g_pfnExit		= NULL;
CIPHER_GETMODE		g_pfnGetMode	= NULL;
CIPHER_SETINFO		g_pfnSetInfo	= NULL;
CIPHER_SETKEY		g_pfnSetKey		= NULL;
CIPHER_ENCRYPT		g_pfnEncrypt	= NULL;
CIPHER_DECRYPT		g_pfnDecrypt	= NULL;
CIPHER_GETLASTERROR	g_pfnGetLastError = NULL;

CIPHER_LIBINIT		g_pfnLibInit	= NULL;
CIPHER_GETCLIENTDYNCODE		g_pfnGetClientDynCode = NULL;
CIPHER_SETCLIENTDYNCODE		g_pfnSetClientDynCode = NULL;


USING_EXTERN_C  bool Cipher_Init(int iMode)
{
	TCHAR szDLL[MAX_PATH];

#ifdef _DEBUG
	_stprintf(szDLL,_T("LCD_%d.DAT"),iMode);
#else
	_stprintf(szDLL,_T("LC_%d.DAT"),iMode);
#endif
	g_hModule = LoadLibrary(szDLL);
	if(!g_hModule)
	{
		DWORD dwError = GetLastError();
//#ifndef _LATALE_CLIENT
//		ErrorLog("dwError=[%d] Library=[%s]\n",dwError,szDLL);
//		DebugPrint("dwError=[%d] Library=[%s]\n",dwError,szDLL);
//#endif
		return false;
	}
	g_pfnInit		= (CIPHER_INIT)GetProcAddress(g_hModule,FN_CIPHER_INIT);
	if(!g_pfnInit) goto end_1;
	g_pfnExit		= (CIPHER_EXIT)GetProcAddress(g_hModule,FN_CIPHER_EXIT);
	if(!g_pfnExit) goto end_1;
	g_pfnGetMode	= (CIPHER_GETMODE)GetProcAddress(g_hModule,FN_CIPHER_GETMODE);
	if(!g_pfnGetMode) goto end_1;
	g_pfnSetInfo	= (CIPHER_SETINFO)GetProcAddress(g_hModule,FN_CIPHER_SETINFO);
	if(!g_pfnSetInfo) goto end_1;
	g_pfnSetKey		= (CIPHER_SETKEY)GetProcAddress(g_hModule,FN_CIPHER_SETKEY);
	if(!g_pfnSetKey) goto end_1;
	g_pfnEncrypt	= (CIPHER_ENCRYPT)GetProcAddress(g_hModule,FN_CIPHER_ENCRYPT);
	if(!g_pfnEncrypt) goto end_1;
	g_pfnDecrypt	= (CIPHER_DECRYPT)GetProcAddress(g_hModule,FN_CIPHER_DECRYPT);
	if(!g_pfnDecrypt) goto end_1;
	g_pfnGetLastError = (CIPHER_GETLASTERROR)GetProcAddress(g_hModule,FN_CIPHER_GETLASTERROR);
	if(!g_pfnGetLastError) goto end_1;

	g_pfnLibInit	= (CIPHER_LIBINIT)GetProcAddress(g_hModule,FN_CIPHER_LIBINIT);
	//if(!g_pfnLibInit) goto end_1;
	g_pfnGetClientDynCode	= (CIPHER_GETCLIENTDYNCODE)GetProcAddress(g_hModule,FN_CIPHER_GETCLIENTDYNCODE);
	//if(!g_pfnGetClientDynCode) goto end_1;
	g_pfnSetClientDynCode	= (CIPHER_SETCLIENTDYNCODE)GetProcAddress(g_hModule,FN_CIPHER_SETCLIENTDYNCODE);
	//if(!g_pfnSetClientDynCode) goto end_1;

	if(!g_pfnInit(iMode))
		goto end_1;

	return true;

end_1:

	if(g_hModule)
	{
		DWORD dwError = GetLastError();

//#ifndef _LATALE_CLIENT
//		ErrorLog("dwError=[%d] Library=[%s]\n",dwError,szDLL);
//		DebugPrint("dwError=[%d] Library=[%s]\n",dwError,szDLL);
//#endif		
		FreeLibrary(g_hModule);
		g_hModule = NULL;
	}

	return false;
}

USING_EXTERN_C  LPVOID Cipher_LibInit(LPVOID pParam1,LPVOID pParam2)
{
	if(!g_pfnLibInit) return 0;
	return g_pfnLibInit(pParam1,pParam2);
}

USING_EXTERN_C  bool Cipher_GetClientDynCode(int& nCodeIndex,BYTE** pClientDynCode,int& nCodeLen)
{
	if(!g_pfnGetClientDynCode) return 0;
	return g_pfnGetClientDynCode(nCodeIndex,(BYTE**)pClientDynCode,nCodeLen);
}

USING_EXTERN_C  bool Cipher_SetClientDynCode(BYTE* pClientDynCode,int& nCodeLen)
{
	if(!g_pfnSetClientDynCode) return 0;
	return g_pfnSetClientDynCode(pClientDynCode,nCodeLen);
}

USING_EXTERN_C  bool Cipher_Exit()
{
	if(g_pfnExit)
		g_pfnExit();

	if(g_hModule)
	{
		FreeLibrary(g_hModule);
		g_hModule = NULL;
	}
	return true;
}

USING_EXTERN_C  unsigned short Cipher_GetMode()
{
	if(!g_pfnGetMode) return 0;
	return g_pfnGetMode();
}

USING_EXTERN_C  bool Cipher_SetInfo(LPVOID lpData,DWORD dwLen)
{
	if(!g_pfnSetInfo) return false;
	return g_pfnSetInfo(lpData,dwLen);
}

USING_EXTERN_C  bool Cipher_SetKey(BYTE* pszKey,DWORD dwLen)
{
	if(!g_pfnSetKey) return false;
	return g_pfnSetKey(pszKey,dwLen);
}

USING_EXTERN_C  DWORD Cipher_Encrypt(BYTE* pszData,DWORD dwLen,int iKeyValue)
{
	if(!g_pfnEncrypt) return 0;
	return g_pfnEncrypt(pszData,dwLen,iKeyValue);
}

USING_EXTERN_C  DWORD Cipher_Decrypt(BYTE* pszData,DWORD dwLen,int iKeyValue)
{
	if(!g_pfnDecrypt) return 0;
	return g_pfnDecrypt(pszData,dwLen,iKeyValue);
}

USING_EXTERN_C  DWORD Cipher_GetLastError()
{
	if(!g_pfnGetLastError) return 0 ;
	return g_pfnGetLastError();
}

#endif // LATALE_CIPHER_DLL

#endif // USING_LATALE_CIPHER

#ifndef _LATALE_CLIENT
void Cipher_LoadConfig(TCHAR* pszIniFile,TCHAR* pszSection)
{
	TCHAR* pszEntry_Mode = _T("Cipher_Mode");
	TCHAR* pszEntry_UseEncryption = _T("Cipher_UseEncryption");

	TCHAR szBuf[260];

	g_iCipherLibMode = GetPrivateProfileInt(pszSection,pszEntry_Mode,g_iCipherLibMode,pszIniFile);
	g_bUseEncryption = (GetPrivateProfileInt(pszSection,pszEntry_UseEncryption,g_bUseEncryption,pszIniFile) > 0)?true:false;

	_stprintf(szBuf,_T("%d		;0(None),Simple(1),nProtect(2)"),g_iCipherLibMode);
	WritePrivateProfileString(pszSection,pszEntry_Mode,szBuf,pszIniFile);
	_stprintf(szBuf,_T("%d		;0(TRUE),1(TRUE)"),g_bUseEncryption?1:0);
	WritePrivateProfileString(pszSection,pszEntry_UseEncryption,szBuf,pszIniFile);

}
#endif
