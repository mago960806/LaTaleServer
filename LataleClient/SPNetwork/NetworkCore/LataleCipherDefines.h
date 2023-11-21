// LataleCipherDefines.h


#pragma once

// history
/*
 *	

	0.9.1 : Local site(Korea)
	0.1.0 : international version

*/

#define LATALE_CIPHER_VERSION		_T("0.9.1")

#define LATALE_CIPHER_KEY_0_1_0		_T("LATALE")
#define LATALE_CIPHER_KEY_0_9_1		_T("v0.9.1")


// 해당 버전에 대한 암호화 키 선택
#define LATALE_CIPHER_KEY			LATALE_CIPHER_KEY_0_9_1

// Cipher Libraries
enum CIPHER_LIB {CIPHER_LIB_NONE,CIPHER_LIB_SIMPLE,CIPHER_LIB_NPROTECT,CIPHER_LIB_SNDC};

#define DEFAULT_SECURITY_KEY	CIPHER_LIB_NONE
#define LATALE_SECURITY_KEY		CIPHER_LIB_SIMPLE


#if defined(LATALE_CIPHER_DLL)
	// for server
	#define VIRTUAL	
#else
	// for client
	#define VIRTUAL	virtual
#endif