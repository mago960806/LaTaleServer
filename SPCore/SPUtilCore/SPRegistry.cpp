
#include <string>
#include <Windows.h>

#include "SPRegistry.h"

using namespace std;

static string m_strProject = "LaTale";

bool SetMachineRegistryKey( char* pValueName, void* pData, unsigned long dwDataSize , const char* pszCompanyName )
{
	HKEY  hkey;
	unsigned long dispos;

	char subkey[256];
	strcpy( subkey, "SOFTWARE\\" );
	strcat( subkey, pszCompanyName );	// AJJIYA [2/8/2007]
	strcat( subkey, "\\" );
	strcat( subkey, m_strProject.c_str() );

	if ( RegCreateKeyEx( HKEY_LOCAL_MACHINE, subkey, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dispos ) != ERROR_SUCCESS ) return false;
	if ( RegSetValueEx( hkey, pValueName, 0, REG_BINARY, (const BYTE*) pData, dwDataSize ) != ERROR_SUCCESS ) return false;
	RegCloseKey( hkey );

	return true;
}

bool GetMachineRegistryKey( char* szSubDir, char* pValueName, void* pData, unsigned long dwDataSize, void* pDefData , const char* pszCompanyName )
{
	HKEY  hkey;
	unsigned long dispos;
	unsigned long type;

	char subkey[256];
	strcpy( subkey, "SOFTWARE\\" );
	strcat( subkey, pszCompanyName );	// AJJIYA [2/8/2007]
	strcat( subkey, "\\" );
	strcat( subkey, m_strProject.c_str() );
	if ( szSubDir != NULL && strlen(szSubDir) > 0 )
	{
		strcat( subkey, "\\" );
		strcat( subkey, szSubDir );
	}

	if ( RegCreateKeyEx( HKEY_LOCAL_MACHINE, subkey, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dispos ) != ERROR_SUCCESS ) return false;
	if ( RegQueryValueEx( hkey, pValueName, NULL, &type, (BYTE*) pData, &dwDataSize ) != ERROR_SUCCESS )
	{
		if ( pDefData != NULL )
		{
			if ( RegSetValueEx( hkey, pValueName, 0, REG_BINARY, (const BYTE*) pDefData, dwDataSize ) != ERROR_SUCCESS ) return false;
			memcpy( pData, pDefData, dwDataSize );
		}
		else 
		{
			memset( pData, 0, dwDataSize );
			return false;
		}
	}
	RegCloseKey( hkey );

	return true;
}


bool SetMachineRegistryKeyString( char* pValueName, void* pData, unsigned long dwDataSize , const char* pszCompanyName )
{
	HKEY  hkey;
	unsigned long dispos;

	char subkey[256];
	strcpy( subkey, "SOFTWARE\\" );
	strcat( subkey, pszCompanyName );	// AJJIYA [2/8/2007]
	strcat( subkey, "\\" );
	strcat( subkey, m_strProject.c_str() );

	if ( RegCreateKeyEx( HKEY_LOCAL_MACHINE, subkey, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dispos ) != ERROR_SUCCESS ) return false;
	if ( RegSetValueEx( hkey, pValueName, 0, REG_SZ, (const BYTE*) pData, dwDataSize ) != ERROR_SUCCESS )
	{
		RegCloseKey( hkey );
		return false;
	}
	RegCloseKey( hkey );

	return true;
}

bool GetMachineRegistryKeyString( char* szSubDir, char* pValueName, void* pData, unsigned long dwDataSize, void* pDefData , const char* pszCompanyName )
{
	HKEY  hkey;
	unsigned long dispos;
	unsigned long type;

	char subkey[256];
	strcpy( subkey, "SOFTWARE\\" );
	strcat( subkey, pszCompanyName );	// AJJIYA [2/8/2007]
	strcat( subkey, "\\" );
	strcat( subkey, m_strProject.c_str() );
	if ( szSubDir != NULL && strlen(szSubDir) > 0 )
	{
		strcat( subkey, "\\" );
		strcat( subkey, szSubDir );
	}

	if ( RegCreateKeyEx( HKEY_LOCAL_MACHINE, subkey, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dispos ) != ERROR_SUCCESS ) return false;
	if ( RegQueryValueEx( hkey, pValueName, NULL, &type, (BYTE*) pData, &dwDataSize ) != ERROR_SUCCESS )
	{
		if ( pDefData != NULL )
		{
			if ( RegSetValueEx( hkey, pValueName, 0, REG_SZ, (const BYTE*) pDefData, dwDataSize ) != ERROR_SUCCESS ) return false;
			memcpy( pData, pDefData, dwDataSize );
		}
		else 
		{
			memset( pData, 0, dwDataSize );
			RegCloseKey( hkey );
			return false;
		}
	}
	RegCloseKey( hkey );

	return true;
}



bool SetUserRegistryKey( char* pValueName, void* pData, unsigned long dwDataSize , const char* pszCompanyName )
{
	HKEY  hkey;
	unsigned long dispos;

	char subkey[256];
	strcpy( subkey, "SOFTWARE\\" );
	strcat( subkey, pszCompanyName );	// AJJIYA [2/8/2007]
	strcat( subkey, "\\" );
	strcat( subkey, m_strProject.c_str() );

	if ( RegCreateKeyEx( HKEY_CURRENT_USER, subkey, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dispos ) != ERROR_SUCCESS ) return false;
	if ( RegSetValueEx( hkey, pValueName, 0, REG_BINARY, (const BYTE*) pData, dwDataSize ) != ERROR_SUCCESS )
	{
		RegCloseKey( hkey );
		return false;
	}
	RegCloseKey( hkey );

	return true;
}

bool GetUserRegistryKey( char* szSubDir, char* pValueName, void* pData, unsigned long dwDataSize, void* pDefData , const char* pszCompanyName )
{
	HKEY  hkey;
	unsigned long dispos;
	unsigned long type;

	char subkey[256];
	strcpy( subkey, "SOFTWARE\\" );
	strcat( subkey, pszCompanyName );	// AJJIYA [2/8/2007]
	strcat( subkey, "\\" );
	strcat( subkey, m_strProject.c_str() );
	if ( szSubDir != NULL && strlen(szSubDir) > 0 )
	{
		strcat( subkey, "\\" );
		strcat( subkey, szSubDir );
	}

	if ( RegCreateKeyEx( HKEY_CURRENT_USER, subkey, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dispos ) != ERROR_SUCCESS ) return false;
	if ( RegQueryValueEx( hkey, pValueName, NULL, &type, (BYTE*) pData, &dwDataSize ) != ERROR_SUCCESS )
	{
		if ( pDefData != NULL )
		{
			if ( RegSetValueEx( hkey, pValueName, 0, REG_BINARY, (const BYTE*) pDefData, dwDataSize ) != ERROR_SUCCESS ) return false;
			memcpy( pData, pDefData, dwDataSize );
		}
		else 
		{
			memset( pData, 0, dwDataSize );
			RegCloseKey( hkey );
			return false;
		}
	}
	RegCloseKey( hkey );

	return true;
}

bool SetUserRegistryKeyString( char* pValueName, void* pData, unsigned long dwDataSize , const char* pszCompanyName )
{
	HKEY  hkey;
	unsigned long dispos;

	char subkey[256];
	strcpy( subkey, "SOFTWARE\\" );
	strcat( subkey, pszCompanyName );	// AJJIYA [2/8/2007]
	strcat( subkey, "\\" );
	strcat( subkey, m_strProject.c_str() );

	if ( RegCreateKeyEx( HKEY_CURRENT_USER, subkey, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dispos ) != ERROR_SUCCESS ) return false;
	if ( RegSetValueEx( hkey, pValueName, 0, REG_SZ, (const BYTE*) pData, dwDataSize ) != ERROR_SUCCESS )
	{
		RegCloseKey( hkey );
		return false;
	}
	RegCloseKey( hkey );

	return true;
}

bool GetUserRegistryKeyString( char* szSubDir, char* pValueName, void* pData, unsigned long dwDataSize, void* pDefData , const char* pszCompanyName )
{
	HKEY  hkey;
	unsigned long dispos;
	unsigned long type;

	char subkey[256];
	strcpy( subkey, "SOFTWARE\\" );
	strcat( subkey, pszCompanyName );	// AJJIYA [2/8/2007]
	strcat( subkey, "\\" );
	strcat( subkey, m_strProject.c_str() );
	if ( szSubDir != NULL && strlen(szSubDir) > 0 )
	{
		strcat( subkey, "\\" );
		strcat( subkey, szSubDir );
	}

	if ( RegCreateKeyEx( HKEY_CURRENT_USER, subkey, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dispos ) != ERROR_SUCCESS ) return false;
	if ( RegQueryValueEx( hkey, pValueName, NULL, &type, (BYTE*) pData, &dwDataSize ) != ERROR_SUCCESS )
	{
		if ( pDefData != NULL )
		{
			if ( RegSetValueEx( hkey, pValueName, 0, REG_SZ, (const BYTE*) pDefData, dwDataSize ) != ERROR_SUCCESS ) return false;
			memcpy( pData, pDefData, dwDataSize );
		}
		else 
		{
			memset( pData, 0, dwDataSize );
			RegCloseKey( hkey );
			return false;
		}
	}
	RegCloseKey( hkey );

	return true;
}






