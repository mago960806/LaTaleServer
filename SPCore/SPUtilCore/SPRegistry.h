// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPRegistry
// Comment     : 클래스에서 글로벌함수로 바꿨다.
// Creation    : metalgeni 2005-10-7 오전 10:26:19
//***************************************************************************

#pragma once


bool SetMachineRegistryKey( char* pValueName, void* pData, unsigned long dwDataSize , const char* pszCompanyName );
bool GetMachineRegistryKey( char* szSubDir, char* pValueName, void* pData, unsigned long dwDataSize, void* pDefData , const char* pszCompanyName );

bool SetMachineRegistryKeyString( char* pValueName, void* pData, unsigned long dwDataSize , const char* pszCompanyName );
bool GetMachineRegistryKeyString( char* szSubDir, char* pValueName, void* pData, unsigned long dwDataSize, void* pDefData , const char* pszCompanyName );

bool SetUserRegistryKey( char* pValueName, void* pData, unsigned long dwDataSize , const char* pszCompanyName );
bool GetUserRegistryKey( char* szSubDir, char* pValueName, void* pData, unsigned long dwDataSize, void* pDefData , const char* pszCompanyName );

bool SetUserRegistryKeyString( char* pValueName, void* pData, unsigned long dwDataSize , const char* pszCompanyName );
bool GetUserRegistryKeyString( char* szSubDir, char* pValueName, void* pData, unsigned long dwDataSize, void* pDefData , const char* pszCompanyName );









