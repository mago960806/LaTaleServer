// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 오전 10:26:19
//***************************************************************************

#pragma once

#include <string>
#include <vector>

class SPStream;

#define RGBA(r,g,b,a)	    ((DWORD)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16))|(((DWORD)(BYTE)(a))<<24))
#define SAFE_DESTROY(p)       { if(p) { (p)->Destroy();     (p)=NULL; } }

int		ReadLine	( int iStream, char s[], int nBufSize = 255);
int		LTrim		( char s[] );
int		RTrim		( char s[] );
char*	Trim		( char s[] );

int		ReadLine	( int iStream, std::string& str);
int		LTrim		( std::string& str );
int		RTrim		( std::string& str );
void	Trim		( std::string& str );

int		ReadLineFromStream( SPStream* pStream, char s[], int nBufSize = 255);
int		ReadLineFromStream( SPStream* pStream, std::string& str);

bool	isCase(const char* cszCASE, const char* cszARG);
const char* GetCase(const char* cszARG);

char*	GetFileName(const char *szSrc);

const char*	GetUpperString(const char* szFileName);
const char*	GetUpperString2(const char* szFileName);
void		GetUpperString3(std::string& strResult, const char* szSrcString);

const char* GetAddResPath(const char* szFileName, const char* szResPath);
int			GetSeperateString( const std::string strParam, std::vector<std::string>& vstrArg );
int			GetSeperateString( const std::string strParam, std::vector<std::string>& vstrArg, char cDelimiter );
void		GetSeperateString(const std::string strFullPath,
					   std::string& strPath, std::string& strName);
int			StringReplaceChar(std::string& strParam, char chSrc, char chDest) ;
void		GetSeperateStringPeriod(const std::string strIn, std::string& strOut);
void		GetSeperateStringDir( const std::string strFullPath , std::string& strPath, std::string& strName );

bool	SetGoldForm		(const char* szSrc, std::string& strTar); // ',' 를 붙여준다.
bool	SetGoldForm		(int iSrc, std::string& strTar);
bool	GetGoldFormStr	(const char* szGold, std::string& szTemp);	// ','를 없애준다.
long	GetGoldForm		(const char* szGold);

bool	PrecisionCompareFloat ( float fData1, float fData2 );

template < typename Type1, typename Type2 >
__inline void DataConvert( const Type1& SrcData, Type2& DestData )
{
	*((Type1*)&DestData) = SrcData;
};

bool Convert255To1(float& fTar, unsigned int iSrc);
bool Convert1To255(unsigned char& iTar, float fSrc);

const char* ConvertMoneyToString(__int64 iMoney);