
#include <math.h>
#include <float.h>
#include <string>
#include <algorithm>
#include <io.h>
#include <vector>
#include <queue>


//#include "SPGameObjectDEF.h"
//#include "SPGameObject.h"
#include "SPFileDEF.h"
//#include "SPResourceDef.h"

#include "SPStream.h"
#include "SPUtil.h"


using namespace std;

int ReadLine( int iStream, char s[], int nBufSize )
{	
	if ( eof(iStream) )  return -1;

	int i = 0; 
	char ch = -1;	

	int iPos = _lseek( iStream, 0L, SEEK_CUR );

	while( ch != 10 && !eof(iStream) ) // 10(LineFeed) is string termainate for DOS FileSystem. (same as UNIX)
	{ 
		read(iStream, &ch, 1); 		
		if ( i+1 > nBufSize )
		{			
			_lseek( iStream, iPos, SEEK_SET );			
			return -1;
		}
		s[i++] = ch;
	}
	s[i-1] = 0; //stuff EOS mark
	return i-1; //return length of the string	
}

int LTrim(char s[]) 
{
	unsigned int nLen =(unsigned int)strlen(s);
	if ( nLen <= 0 ) return -1;
	for ( unsigned int i = 0; i < nLen; i++)
	{
		if( s[i] != ' '  &&
			s[i] != '\t' &&
			s[i] != '\n' &&
			s[i] != '\r' )
		{
			memmove( &s[0], &s[i], nLen-i+1);
			return i;
		}
	}
	s[0] = '\0';
	return nLen;
}

int RTrim(char s[])
{
	int nLen = (int) strlen(s);
	if ( nLen <= 0 ) return -1;
	for ( int i = nLen-1; i >= 0; i-- )
	{
		if( s[i] != ' '  &&
			s[i] != '\t' &&
			s[i] != '\n' &&
			s[i] != '\r' )
		{
			s[i+1] = 0;
			return nLen-i;
		}
	}
	s[0] = '\0';
	return nLen;
}

char* Trim(char s[])
{
	LTrim(s);
	RTrim(s);
	return s;
}

int ReadLine( int iStream, string& str)
{	
	if ( eof(iStream) )  return -1;

	int i = 0; 

	str.clear();

	char szBuf[256];
	i = ReadLine( iStream, szBuf, 255);
	str = szBuf;

	return i; //return length of the string	
}

int ReadLineFromStream( SPStream* pStream, char s[], int nBufSize)
{		
	if ( !pStream->Valid() )  return -1;

	int i = 0; 
	char ch = -1;		

	int iPos = pStream->Seek( 0L, SEEK_CUR );

	while( ch != 10 && !pStream->Eos() ) // 10(LineFeed) is string termainate for DOS FileSystem. (same as UNIX)
	{ 
		pStream->Read(&ch, 1); 				
		if ( i+1 > nBufSize )
		{			
			pStream->Seek( iPos, SEEK_SET );			
			return -1;
		}
		s[i++] = ch;
	}
	s[i-1] = 0; //stuff EOS mark
	return i-1; //return length of the string	
}

int ReadLineFromStream( SPStream* pStream, string& str)
{	
	if ( pStream->Eos() )  return -1;

	int i = 0; 

	str.clear();

	char szBuf[256];
	i = ReadLineFromStream( pStream, szBuf, 255);
	str = szBuf;

	return i; //return length of the string	
}

int LTrim(string& str) 
{
	unsigned int nLen = (unsigned int) str.length();	
	if ( nLen <= 0 ) return -1;

	char* pszStr = new char[nLen+1];
	strcpy( pszStr, str.c_str() );

	LTrim(pszStr);
	str = pszStr;

	delete[] pszStr;
	return nLen;
}

int RTrim(string& str)
{
	unsigned int nLen = (unsigned int) str.length();	
	if ( nLen <= 0 ) return -1;

	char* pszStr = new char[nLen+1];
	strcpy( pszStr, str.c_str() );

	RTrim(pszStr);
	str = pszStr;

	delete[] pszStr;
	return nLen;
}

void Trim(string& str)
{
	LTrim(str);
	RTrim(str);	
}

#pragma warning(disable : 4267)
// cszARG를 STR_SEP많큼 짜른후 cszCASE와 비교한다.
// 주의.. 버그 많음..
bool isCase(const char* cszCASE, const char* cszARG)
{
	// CASE Size로 체크하면 대략 낭패스러움.
	std::string strParam = cszARG;
	std::string strCase;
	int			iIter = -1;	
	
	iIter = strParam.find(STR_SEP);
	if(iIter == -1) {
		iIter = strParam.find(STR_EQ);
	}	

	strCase = strParam.substr( 0,  iIter );
	Trim(strCase);
	if (strCase == cszCASE) 
		return true;
	return false;
}


const char* GetCase(const char* cszARG)
{
	static std::string strCase;	
	std::string strParam = cszARG;	
	int			iIter = -1;	

	strCase.clear();

	iIter = strParam.find(STR_SEP);
	if(iIter == -1) {
		iIter = strParam.find(STR_EQ);
	}

	strCase = strParam.substr( 0,  iIter );
	Trim(strCase);
	return strCase.c_str();
}

#pragma warning(default : 4267)

bool SetGoldForm(const char* szSrc, std::string& strTar)
{		
	try {			
		GetGoldFormStr(szSrc, strTar); //','를 없애주고 시작한다.

		if(*strTar.begin() == '0' && strTar.size() == 1) {
			return true;
		}

		while(*strTar.begin() == '0') {
			strTar.erase(strTar.begin());
		}

		//strTar = szSrc;	
		int iSize = (int)strTar.size();
		for(int ic = (int)strTar.size() ; ic > 0;  ic--) {
			if( (iSize-ic) % 3 == 0 && (iSize != ic) )
				strTar.insert( ic , 1, ',');
		}
		return true;
	}
	catch(...) {
		strTar.clear();
		return false;
	}
}

bool SetGoldForm(int iSrc, std::string& strTar)
{
	try {	
		char szSrc[128];
		_itoa(iSrc, szSrc, 10);
		strTar = szSrc;	
		int iSize = (int)strTar.size();
		for(int ic = (int)strTar.size() ; ic > 0;  ic--) {
			if( (iSize-ic) % 3 == 0 && (iSize != ic) )
				strTar.insert( ic , 1, ',');
		}
		return true;
	}
	catch(...) {
		strTar.clear();
		return false;
	}
}

long GetGoldForm(const char* szGold)
{
	std::string szTemp = szGold;
	std::string::iterator new_end = std::remove(szTemp.begin(), szTemp.end(),',');
	szTemp.erase(new_end, szTemp.end());

	return atol(szTemp.c_str());
}

bool GetGoldFormStr(const char* szGold, std::string& szTemp)
{
	try {	
		szTemp = szGold;
		std::string::iterator new_end = std::remove(szTemp.begin(), szTemp.end(),',');
		szTemp.erase(new_end, szTemp.end());		
		return true;
	} catch(...) {		
		szTemp.clear();
		return false;
	}
}

char* GetFileName(const char *szSrc)
{
	if ( !szSrc ) 
		return NULL;

	const int nLen = (const int)strlen(szSrc);
	if ( nLen <= 0 || nLen >= 256 ) 
		return NULL;

	for ( int iOffset = nLen-1; iOffset >= 0; --iOffset )
	{
		if( szSrc[iOffset] == '/' ||
			szSrc[iOffset] == '\\' )
		{
			static char szTemp[256];
			strcpy(szTemp, szSrc + iOffset + 1);			
			return szTemp;
		}
	}
	
	return NULL;
}

const char*	GetUpperString(const char* szFileName)
{
	assert(0 && " 쓰지마삼  ");
	return GetUpperString2(szFileName);	
	//return _strupr( _strdup( szFileName ) );

	//return _strupr( const_cast<char*>(szFileName) );
	//char strTemp[256];
	
	//&(strTemp[0]) = _strdup(szFileName);

	//return _strupr( strTemp );

	//return szFileName;

	//static std::string strTarFileName;
	//std::string strSrcFileName;

	//strTarFileName.clear();
	//strSrcFileName = szFileName;

	//std::string::iterator Iter = strSrcFileName.begin();

	//for(; Iter != strSrcFileName.end(); ++Iter) {
	//	(*Iter) = toupper((*Iter));		
	//	strTarFileName.push_back(*Iter);
	//}	
	//return strTarFileName.c_str();
}

const char*	GetUpperString2(const char* szFileName)
{
	assert(0 && "  쓰지마삼  ");
	//return _strupr( szFileName );
	//char strTemp[256];	
	//strcpy( strTemp, szFileName);
	//return _strupr( strTemp);

	//volatile char strTemp[256];
	//char* pTemp;
	//strcpy( (char*)strTemp, szFileName);
	//pTemp = (char*)strTemp;
	//return _strupr( pTemp);

	static std::string strTarFileName;
	std::string strSrcFileName;

	strTarFileName.clear();
	strSrcFileName = szFileName;

	std::string::iterator Iter = strSrcFileName.begin();

	for(; Iter != strSrcFileName.end(); ++Iter) {
		(*Iter) = toupper((*Iter));		
		strTarFileName.push_back(*Iter);
	}	
	return strTarFileName.c_str();
}


void GetUpperString3(std::string& strResult, const char* szSrcString)
{
	char* pszTemp = _strdup(szSrcString);	
	strResult = _strupr(pszTemp);
	free(pszTemp);
}


const char* GetAddResPath(const char* szFileName, const char* szResPath)
{
	static std::string strPath;
	strPath.clear();
	strPath = szResPath;
	strPath += szFileName;
	return strPath.c_str();
}

bool	PrecisionCompareFloat ( float fData1, float fData2 )
{
	return (abs(fData1-fData2)<=FLT_EPSILON)?true:false;
}
/*
template < typename Type1, typename Type2 >
void DataConvert( const Type1& SrcData, Type2& DestData )
{
	*((Type1*)&DestData) = SrcData;
};
*/
#pragma warning(disable: 4267)

/**
	★ STR_SEP(:) 이후 인자부터 가져온다.
*/
int GetSeperateString( const std::string strParam, std::vector<std::string>& vstrArg )
{
	std::string strArgPart;
	int		iIter;
	int		iIter2;	
	int		iNOArg = 0;

	iIter2 = strParam.find(STR_SEP);

	while(iIter2 != -1) {
		iIter = iIter2;
		iIter2 = strParam.find(STR_SEP, iIter + 1);
		strArgPart = strParam.substr( iIter + 1,  iIter2 - ( iIter + 1 ) );
		Trim(strArgPart);
		vstrArg.push_back(strArgPart);		
		strArgPart.clear();
		++iNOArg;
	}
	return iNOArg;
}


/**
	String을 Delimiter를 이용해서 vstrArg로 분리해 주는 함수 [2006.12.05 Duragon]
	★ 첫번째 인자부터 가져온다.
	@param string strParam : 분리 되어야 할 문자열
	@param vector<string>vstrArg : 분리된 문자열이 들어갈 벡터
	@prarm char delimiter : 구분자
	@return int : 분리후 분리된 문자의 개수
*/
int GetSeperateString( const std::string strParam, std::vector<std::string>& vstrArg, char cDelimiter )
{
	std::string strArgPart;
	int		iIter;
	int		iIter2;	
	int		iNOArg = 0;

	iIter2 = -1 ;
	 do {
		iIter = iIter2;
		iIter2 = strParam.find(cDelimiter, iIter + 1);
		strArgPart = strParam.substr( iIter + 1,  iIter2 - ( iIter + 1 ) );
		Trim(strArgPart);
		vstrArg.push_back(strArgPart);		
		strArgPart.clear();
		++iNOArg;
	}while(iIter2 != -1) ;
	return iNOArg;
}


void GetSeperateString(const std::string strFullPath,
	std::string& strPath, std::string& strName)
{
	int iSize, iIter1, iIter2;

	strPath.clear();
	strName.clear();

	iSize = (int)strFullPath.size();
	if( iSize == 0 )
		return;

	iIter1 = 0;
	do
	{
		iIter2 = strFullPath.find('\\', iIter1);
		if( iIter2 == -1 )
			iIter2 = strFullPath.find('/', iIter1);

		if( iIter2 == -1 )
			break;

		iIter1 = iIter2 + 1;
	}while( 1 );
	
	strPath = strFullPath.substr(0, iIter1);
	strName = strFullPath.substr(iIter1, iSize - iIter1);
}

void GetSeperateStringPeriod(const std::string strIn, std::string& strOut)
{
	int iIter1;
	strOut.clear();

	iIter1 = strIn.find('.');
	if( iIter1 == -1 )
		return;

	strOut = strIn.substr(0, iIter1);
}

void GetSeperateStringDir( const std::string strFullPath , std::string& strPath, std::string& strName )
{
	int iSize, iIter1, iIter2;

	strPath.clear();
	strName.clear();

	iSize = (int)strFullPath.size();
	if( iSize == 0 )
		return;

	iIter1 = 0;
	do
	{
		iIter2 = (int)strFullPath.find('\\', iIter1);
		if( iIter2 == -1 )
			iIter2 = (int)strFullPath.find('/', iIter1);

		if( iIter2 == -1 )
			break;

		iIter1 = iIter2 + 1;
	}while( 1 );

	strPath = strFullPath.substr(0, iIter1);
	strName = strFullPath.substr(iIter1, iSize - iIter1);
}

bool Convert255To1(float& fTar, unsigned int iSrc)
{
	fTar = (float)iSrc / 255.f;

	if(fTar > 1.0) { 
		fTar = 1.0f;
		return false;
	}
	return true;
}

bool Convert1To255(unsigned char& iTar, float fSrc)
{
	iTar = (int)(fSrc * 255);

	if(iTar > 255) {
		iTar = 255;
		return false;
	}
	return true;

}

const char* ConvertMoneyToString(__int64 iMoney)
{
	static std::string s_strBuf;

	s_strBuf.clear();
	char szTemp[256];
	sprintf(szTemp, "%I64d", iMoney);
	int iLength = (int)strlen(szTemp);
	int i, iCount = 0;
	
	for( i = iLength - 1; i >= 0; i-- )
	{
		if( iCount && ((iCount % 3) == 0) )
			s_strBuf.insert((std::string::size_type)0, 1, ',');

		s_strBuf.insert((std::string::size_type)0, 1, szTemp[i]);
		iCount++;
	}

	return s_strBuf.c_str();
}

/**
	String의 특정 문자(Char)를 지정한 문자로 변환	[2006.12.11 Duragon]
	(MapEvent의 Space가 들어가 있는 문자열을 @이 들어간 문자열로 변환한다.)
	@param string strSrc	: 원본 문자열
	@param string strDest	: Replace된 문자열이 들어갈 문자열
	@param char chSrc		: 찾아야 할 문자 (변경되어야 할)
	@param char chDest		: 바뀌어 들어갈 문자
	@return int : Replace Count
*/
int StringReplaceChar(std::string& strParam, char chSrc, char chDest)
{
	int		iIter;
	int		iIter2;	
	int		iNOArg = 0;

	iIter2 = -1 ;
	while(1) {
		iIter = iIter2;
		iIter2 = strParam.find(chSrc, iIter + 1);		// chSrc를 찾아서
		if(iIter2 == -1)	break ;					
		strParam.replace(iIter2, 1, 1, chDest) ;			// chDest로 변경
		++iNOArg;
	}
	return iNOArg;
}

#pragma warning(default : 4267)











