// LataleCipher.h

#pragma once

#ifdef LATALE_CIPHER_DLL

	#ifdef LATALECIPHER_EXPORTS
		#define LATALECIPHER_API __declspec(dllexport)
	#else
		#define LATALECIPHER_API //__declspec(dllimport)
	#endif

	extern "C"
	{
		LATALECIPHER_API unsigned short Cipher_GetMode();
		LATALECIPHER_API bool Cipher_SetInfo(LPVOID lpData,DWORD dwLen);
		LATALECIPHER_API bool Cipher_SetKey(BYTE* pszKey,DWORD dwKeyLen);

		LATALECIPHER_API bool Cipher_Init(int iMode);
		LATALECIPHER_API bool Cipher_Exit();

		LATALECIPHER_API LPVOID Cipher_LibInit(LPVOID pParam1,LPVOID pParam2);
		LATALECIPHER_API bool Cipher_GetClientDynCode(int& nCodeIndex,BYTE** pClientDynCode,int& nCodeLen);
		LATALECIPHER_API bool Cipher_SetClientDynCode(BYTE* pClientDynCode,int& nCodeLen);

		LATALECIPHER_API DWORD Cipher_Encrypt(BYTE* pszData,DWORD dwLen,int iKeyValue);
		LATALECIPHER_API DWORD Cipher_Decrypt(BYTE* pszData,DWORD dwLen,int iKeyValue);

		LATALECIPHER_API DWORD Cipher_GetLastError();
	}

	typedef bool (*CIPHER_INIT)(int iMode);
	typedef bool (*CIPHER_EXIT)();

	typedef LPVOID (*CIPHER_LIBINIT)(LPVOID pParam1,LPVOID pParam2);
	typedef bool (*CIPHER_GETCLIENTDYNCODE)(int& nCodeIndex,BYTE** pClientDynCode,int& nCodeLen);
	typedef bool (*CIPHER_SETCLIENTDYNCODE)(BYTE* pClientDynCode,int& nCodeLen);

	typedef unsigned short (*CIPHER_GETMODE)();
	typedef bool  (*CIPHER_SETINFO)(LPVOID lpData,DWORD dwLen);
	typedef DWORD (*CIPHER_SETKEY)(BYTE* pszKey,DWORD dwKeyLen);

	typedef DWORD (*CIPHER_ENCRYPT)(BYTE* pszData,DWORD dwLent,int iKeyValue);
	typedef DWORD (*CIPHER_DECRYPT)(BYTE* pszData,DWORD dwLen,int iKeyValue);

	typedef DWORD (*CIPHER_GETLASTERROR)();

	#define FN_CIPHER_INIT			"Cipher_Init"
	#define FN_CIPHER_EXIT			"Cipher_Exit"

	#define FN_CIPHER_LIBINIT				"Cipher_LibInit"
	#define FN_CIPHER_GETCLIENTDYNCODE		"Cipher_GetClientDynCode"
	#define FN_CIPHER_SETCLIENTDYNCODE		"Cipher_SetClientDynCode"

	#define FN_CIPHER_GETMODE		"Cipher_GetMode"
	#define FN_CIPHER_SETINFO		"Cipher_SetInfo"
	#define FN_CIPHER_SETKEY		"Cipher_SetKey"

	#define FN_CIPHER_ENCRYPT		"Cipher_Encrypt"
	#define FN_CIPHER_DECRYPT		"Cipher_Decrypt"

	#define FN_CIPHER_GETLASTERROR	"Cipher_GetLastError"

#else

#define LATALECIPHER_API 

extern "C"
{
	unsigned short Cipher_GetMode();
	bool Cipher_SetInfo(void* lpData,unsigned long dwLen);

	bool Cipher_Init(int iMode);
	bool Cipher_Exit();
	bool Cipher_SetKey(unsigned char* pszKey,unsigned long dwKeyLen);
	unsigned long Cipher_Encrypt(unsigned char* pszData,unsigned long dwLen,int iKeyValue);
	unsigned long Cipher_Decrypt(unsigned char* pszData,unsigned long dwLen,int iKeyValue);

	unsigned long Cipher_GetLastError();

	bool Cipher_SetClientDynCode(BYTE* pClientDynCode,int& nCodeLen);
}

#endif

extern bool g_bUseEncryption;
extern int g_iCipherLibMode;

extern void Cipher_LoadConfig(TCHAR* pszIniFile,TCHAR* pszSection = _T("Cipher"));
