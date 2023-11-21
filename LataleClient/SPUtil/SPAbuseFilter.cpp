
#ifdef _LATALE_SERVER
	#include "GlobalDefines.h"
#else 
	#include "SPCommon.h"
	#include "dxutil.h"
#endif

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPLocalizeManager.h"

#include "SPAbuseFilter.h"


#define MAX_STRING	256

//////////////////////////////////////////////////////////////////////////

SPAbuseFilter::SPAbuseTag::~SPAbuseTag() 
{ 
	delete[] pStrFrom; 
	delete[] pStrTo; 
}

//////////////////////////////////////////////////////////////////////////


SPAbuseFilter* SPAbuseFilter::s_pAbuseFilter = 0;

SPAbuseFilter::SPAbuseFilter()
{
}

SPAbuseFilter::~SPAbuseFilter()
{
	for( unsigned cnt = 0; cnt < m_vpAbuseWordList.size(); cnt++ ) {
		delete m_vpAbuseWordList[cnt];
	}
	m_vpAbuseWordList.clear();

	for( cnt = 0; cnt < m_vpszBanWordList.size(); cnt++ ) {
		delete [] (m_vpszBanWordList[cnt]);
	}
	m_vpszBanWordList.clear();
}

SPAbuseFilter* SPAbuseFilter::GetInstance()
{
	return s_pAbuseFilter;
}

SPAbuseFilter* SPAbuseFilter::Create()
{
	if( s_pAbuseFilter == 0 ) {
		s_pAbuseFilter = new SPAbuseFilter;		
	}
	return s_pAbuseFilter;
}

void SPAbuseFilter::Release()
{
	SAFE_DELETE(s_pAbuseFilter);
}

bool SPAbuseFilter::Init()
{
//////////////////////////////////////////////////////////////////////////
	
#if !defined(_JUST_RUN)
	SPLDTFile* pLDTFile;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_ABUSE_LIST, &pLDTFile);	

	if(pLDTFile == NULL) return false;

	int recordCount = pLDTFile->GetItemCount();
	int fieldCount = pLDTFile->GetFieldCount();
	LDT_Field ldtField1;
	LDT_Field ldtField2;

	int		iItemID;
//	char	szTemp[128];
	for( int record = 0; record < recordCount; record++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( record );

		//_Abuse			ｾｲﾁｻｾﾆｾﾟﾇﾒｴﾜｾ・txt
		//_Replace			ﾄ｡ﾈｯｵﾉｴﾜｾ・		txt

		pLDTFile->GetField(iItemID, 0, ldtField1);
		pLDTFile->GetField(iItemID, 1, ldtField2);
		//pLDTFile->GetFieldFromLable( iItemID, "_Abuse"	,		ldtField1);
		//pLDTFile->GetFieldFromLable( iItemID, "_Replace",		ldtField2); 

		AddRule(ldtField1.uData.pValue, ldtField2.uData.pValue );

	}
	SAFE_RELEASE(pLDTFile);

//////////////////////////////////////////////////////////////////////////
	
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_BAN_LIST, &pLDTFile);	

	if(pLDTFile == NULL) return false;

	recordCount = pLDTFile->GetItemCount();
	fieldCount = pLDTFile->GetFieldCount();		

	for( int record = 0; record < recordCount; record++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( record );

		//_BanName			ｾｲﾁｻｾﾆｾﾟﾇﾒｴﾜｾ・txt
		pLDTFile->GetFieldFromLable( iItemID, "_BanName",	ldtField1);
		AddBenWord( ldtField1.uData.pValue );

	}
	SAFE_RELEASE(pLDTFile);
//////////////////////////////////////////////////////////////////////////
#endif

	return true;
}


inline bool SPAbuseFilter::IsSpecialChar( const char cChar )
{
	if( !( cChar & (1<<7) ) && 
		!( cChar >='0' && cChar <='9' ) && 
		!( cChar >='a' && cChar <='z' ) && 
		!( cChar >='A' && cChar <='Z' ) &&
		!( cChar == 0 )) return true;
	return false;
}

inline bool SPAbuseFilter::IsSpecialCharW( const WCHAR cChar )
{
	bool bRetVal = false;
	static COUNTRY_TYPE ct = SPLocalizeManager::GetInstance()->GetCountryType();
	
	// ﾇﾑﾀﾚ ｫn ｺﾒｰ｡
	if( cChar == 0x20 || cChar == 0x8140 || cChar == 0x3000 ) { // white space half and full
		bRetVal = true;
	}

	if(ct != CT_KOREA) {
		return bRetVal;
	}

	if( ct == CT_KOREA) {
		//if( cChar <= 0xC8FE && cChar >= 0xB0A1 ) { 
		//if( cChar <= 0xC8FE && cChar >= 0xAC00 ) { 
		if( cChar <= 0xD7A3 && cChar >= 0xAC00 ) { 
			
			return false;
		}
	}


	if(!( cChar >='0' && cChar <='9' ) && 
		!( cChar >='a' && cChar <='z' ) && 
		!( cChar >='A' && cChar <='Z' ) &&
		!( cChar == 0 )) {
		bRetVal = true;
	}


	return bRetVal;
}


inline bool SPAbuseFilter::IsEqual( const char cChar1, const char cChar2 )
{
	if( cChar1 == cChar2 ) return true;
	if( cChar1 >= 'A' && cChar1 <= 'Z' && ( cChar1 - 'A' + 'a' ) == cChar2 ) return true;
	if( cChar2 >= 'A' && cChar2 <= 'Z' && ( cChar2 - 'A' + 'a' ) == cChar1 ) return true;
	return false;
}

inline bool SPAbuseFilter::IsEqualW( const WCHAR cChar1, const WCHAR cChar2 )
{
	if( cChar1 == cChar2 ) return true;
	if( cChar1 >= 'A' && cChar1 <= 'Z' && ( cChar1 - 'A' + 'a' ) == cChar2 ) return true;
	if( cChar2 >= 'A' && cChar2 <= 'Z' && ( cChar2 - 'A' + 'a' ) == cChar1 ) return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool SPAbuseFilter::IsStringWithSpecialCharJapan(const char* pStr)
{
	if(pStr && strlen(pStr) > LEN_NAME)
		return true;


	WCHAR wstrTar[256];
	ConvertAnsiStringToWide( wstrTar, pStr);

	int iLenW = (int)lstrlenW(wstrTar);

	for(int i = 0; i <= iLenW; ++i) {
		if( IsSpecialCharW( wstrTar[i] ) ) 
			return true;
	}
	return false;
}

bool SPAbuseFilter::IsStringWithSpecialChar(const char* pStr)
{	
	if(pStr && strlen(pStr) > LEN_NAME)
		return true;


	char strTar[256] = { 0, };
	strcpy( strTar, pStr);
	int iLen = strlen(strTar);


	// ﾇﾑｱﾛﾀﾌ ｼｯｿｩ ﾀﾖｴﾂﾁ・
	bool bHan = false;

	unsigned short iTemp;
	//for(int i = 0; i < iLen - 1; ++i) {
	for(int i = 0; i <= iLen; ++i) {	// <- ﾆｯｼｮﾀﾚ ﾃ｣ｱ・ﾀｧﾇﾘ

		if(strTar[i] & 0x80 ) { 
			iTemp = (((BYTE)strTar[i]) << 8) | ((BYTE)strTar[i+1]);
			if( !(iTemp <= 0xC8FE && iTemp >= 0xB0A1) ) { // ﾇﾑｱﾛ ﾀｯｴﾏﾄﾚｵ蟆｡ ｾﾆｴﾏｸ・
				return true;
			} else {
				bHan = true;
				++i;
				continue;
			}
		}

		if( IsSpecialChar( strTar[i] ) ) 
			return true;
	}

	// ﾇﾑｱﾛ ﾇﾑｱﾛﾀﾚﾀﾏ ｰ豼・ｾﾈｵﾈｴﾙ.
	if(bHan && iLen <= 2)
		return true;

	// ｿｵｹｮ ﾇﾑｱﾛﾀﾚﾀﾏ ｰ豼・ｾﾈｵﾈｴﾙ.
	if(iLen <= 1)
		return true;

	return false;
}


// a Kind of Common Utility

TCodeType  SPAbuseFilter::IsHangul(const char* OutText, int Len)
{
	if( (int)strlen(OutText) < Len )
		return ctError;

	int i;
	TCodeType type = ctEng;

	for(i=0; OutText[i] != '\0'; i++)
	{
		if(type == ctFirst)
			type = ctLast;
		else
		{
			if(OutText[i] & 0x80)
				type = ctFirst;
			else type = ctEng;
		}
		if(i == Len) 
			return type;
	}

	if(OutText[i] != '\0') {
		return ctError;	
	}
	return ctEng;
	
}

void SPAbuseFilter::ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource)
{
	int cchDestChar; 

	if( wstrDestination==NULL || strSource==NULL )
		return;

	cchDestChar = strlen(strSource)+1;

	MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
		wstrDestination, cchDestChar-1 );

	wstrDestination[cchDestChar-1] = 0;
}

void SPAbuseFilter::ConvertWideToAnsiString( CHAR* strDestination, const WCHAR* wstrSource)
{
	if( strDestination==NULL || wstrSource==NULL )
		return;

	int iLen = WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, 
		strDestination, 256, NULL, NULL );	

	strDestination[iLen] = 0;
}

int SPAbuseFilter::GetAnsiLenToWideLen(int iPos, const char* szAnsi)
{
	if(iPos == 0)
		return 0;

	if(szAnsi == NULL) 
		return 0;

	if(iPos > strlen(szAnsi))
		return 0;

	TCodeType tct;
	int iLen = 0;
	for(int i = 0; i <= iPos; ++i) {
		tct = IsHangul(szAnsi, i);	
		switch(tct) {
		case ctError: 
			return 0;
		case ctEng	: 
		case ctLast	: 
			++iLen;
			break;
		case ctFirst: 
			if(i == iPos)
				++iLen;
			break;		
		}
	}
	return iLen;
}


int SPAbuseFilter::GetAnsiLenToWideLen(int iPos, const WCHAR* wszWide)
{
	char szConvertedAnsi[MAX_STRING];
	if(wszWide == NULL) 
		return 0;

	if(iPos == 0)
		return 0;

	ConvertWideToAnsiString(szConvertedAnsi, wszWide);
	//if(iPos > strlen(szConvertedAnsi))
	//	return 0;

	TCodeType tct;
	int iLen = 0;
	for(int i = 0; i <= iPos; ++i) {
		tct = IsHangul(szConvertedAnsi, i);	
		switch(tct) {
		case ctError: 
			return iLen;
		case ctEng	: 
		case ctLast	: 
			++iLen;
			break;
		case ctFirst: 
			if(i == iPos)
				++iLen;
			break;		
		}
	}
	return iLen;
}



int SPAbuseFilter::GetWideLenToAnsiLen(int iPosW, const WCHAR* wszWide)
{
	char szConvertedAnsi[MAX_STRING];
	int iLenW = lstrlenW(wszWide);

	if(wszWide == NULL) 
		return 0;	

	if(iPosW > iLenW)
		return 0;

	if(iPosW == 0)
		return 0;

	ConvertWideToAnsiString(szConvertedAnsi, wszWide);

	iLenW = strlen(szConvertedAnsi);

	TCodeType tct;
	int iCalcAnsiLen = 0;
	int iCalcSrcPos = 0;
	for(int i = 0; i <= iLenW; ++i) {
		tct = IsHangul(szConvertedAnsi, i);	
		switch(tct) {
		case ctError: 
			return 0;
		case ctEng	: 
		case ctLast	: 
			++iCalcSrcPos;
			break;
		}
		++iCalcAnsiLen;
		if(iCalcSrcPos >= iPosW)
			break;
	}

	return iCalcAnsiLen;
}


void SPAbuseFilter::Arrange( char* pStr )
{
	WCHAR* ptr = new WCHAR[ strlen(pStr)+1 ];
	memset( ptr, 0, (strlen(pStr)+1) * sizeof(WCHAR) );

	WCHAR* pComp1; //= pStr;
	WCHAR* pComp2 = ptr;
	WCHAR* pwszSrc = new WCHAR[ strlen(pStr)+1 ];

	ConvertAnsiStringToWide( pwszSrc, pStr);
	pComp1 = pwszSrc;

	for( pComp1 = pwszSrc, pComp2 = ptr; *pComp1 ; pComp1++ ) {
		if( IsSpecialCharW( *pComp1 ) ) 
			continue;

		*pComp2 = *pComp1;
		pComp2++;
	}

	if( pComp2 != ptr ) {
		lstrcpyW( pwszSrc, ptr );
	}

	delete[] ptr;
	delete [] pwszSrc;
}

char* SPAbuseFilter::FindWord( const char* pStr, const char* pStrWord, unsigned& iReplaceLen )
{
	char *pCompPosition = NULL;
	char *pComp1, *pComp2;	

	int iIndicater1 = 0;
	int iIndicater2 = 0;
//	bool bEqual;
	
	iReplaceLen = 0;

	pComp1 = const_cast< char* >( pStr );
	pComp2 = const_cast< char* >( pStrWord );

	while( *pComp1 ) {
		if( IsSpecialChar( *pComp1 ) ) {
			if( pCompPosition != NULL) { 				
				iReplaceLen++;
			}
			pComp1++;
			++iIndicater1;
			continue;
		}

		//if(ctFirst == g_pInterfaceManager->GetIMEPool()->IsHangul(pStr, iIndicater1 ) &&
		//	ctFirst == g_pInterfaceManager->GetIMEPool()->IsHangul(pStrWord, iIndicater2 )) {

		//	
		//}

		if( IsEqual( *pComp1, *pComp2 ) ) {
			if( pCompPosition == NULL ) {
				pCompPosition = pComp1;
				iReplaceLen = 0;
			}						
			iReplaceLen++;
			pComp2++;
			++iIndicater2;
			
			if( *pComp2 == '\0' ) 
				break;

		} else {
			if( pCompPosition != NULL ) {
				pComp2 = const_cast< char* >( pStrWord );
				pCompPosition = NULL;

				if(iReplaceLen == 1) { // ﾂﾁ ﾀﾌｷｳ ｾﾈｵﾇｴﾂｵ･..
					pComp1++;
					++iIndicater1;
				}
				continue;
			}

			if( (*pComp1) & 128 ) {
				pComp1 = CharNext( pComp1 );
				pComp1--;
				--iIndicater1;
			}

			if( !(*pComp1) ) break;
		}
		pComp1++;
		++iIndicater1;
	}

	if( *pComp2 == '\0' )
	{
		return pCompPosition;
	}

	return NULL;
}



WCHAR* SPAbuseFilter::FindWordW( const WCHAR* pwStr, const WCHAR* pwStrWord, unsigned& iReplaceLen )
{
	WCHAR *pCompPosition = NULL;
	WCHAR *pComp1, *pComp2;	

	int iIndicater1 = 0;
	int iIndicater2 = 0;
//	bool bEqual;

	iReplaceLen = 0;

	pComp1 = const_cast< WCHAR* >( pwStr );
	pComp2 = const_cast< WCHAR* >( pwStrWord );

	while( *pComp1 ) {
		if( IsSpecialCharW( *pComp1 ) ) {
			if( pCompPosition != NULL) { 				
				iReplaceLen++;
			}
			pComp1++;
			++iIndicater1;
			continue;
		}

		if( IsEqualW( *pComp1, *pComp2 ) ) {
			if( pCompPosition == NULL ) {
				pCompPosition = pComp1;
				iReplaceLen = 0;
			}						
			iReplaceLen++;
			pComp2++;
			++iIndicater2;

			if( *pComp2 == '\0' ) 
				break;

		} else {
			if( pCompPosition != NULL ) {
				pComp2 = const_cast< WCHAR* >( pwStrWord );
				pCompPosition = NULL;

				//if(iReplaceLen == 1) { // ﾂﾁ ﾀﾌｷｳ ｾﾈｵﾇｴﾂｵ･..
				//	pComp1++;
				//	++iIndicater1;
				//}
				continue;
			}

			//if( (*pComp1) & 128 ) {
			//	pComp1 = CharNextW( pComp1 );
			//	pComp1--;
			//	--iIndicater1;
			//}

			if( !(*pComp1) ) break;
		}
		pComp1++;
		++iIndicater1;
	}

	if( *pComp2 == '\0' )
	{
		return pCompPosition;
	}

	return NULL;
}



bool SPAbuseFilter::AddRule( const char* pStrFrom, const char* pStrTo )
{
	if( !pStrFrom || !pStrTo ) return false;

	SPAbuseTag *tag = new SPAbuseTag;
	tag->pStrFrom = new char[ strlen(pStrFrom) +1 ];
	tag->pStrTo = new char[ strlen(pStrTo) +1 ];

	strcpy( tag->pStrFrom, pStrFrom );
	strcpy( tag->pStrTo, pStrTo );

	static COUNTRY_TYPE ct = SPLocalizeManager::GetInstance()->GetCountryType();
	if(ct == CT_KOREA) {
		_strlwr( tag->pStrFrom );
	}
	//_strlwr( tag->pStrFrom );

	Arrange( tag->pStrFrom );

	// ﾅｫｰﾍｿ・ｱﾀｸｷ?ﾄ｡ﾈｯｵﾇｾ﨨ﾟ ﾇﾏｱ篩｡..
	if( m_vpAbuseWordList.size() ) {
		std::vector< SPAbuseTag* >::iterator it;
		//for( it = m_vpAbuseWordList.begin(); it < m_vpAbuseWordList.end(); it++ ) {
		bool bInsert = false;
		for( it = m_vpAbuseWordList.begin(); it != m_vpAbuseWordList.end(); ++it ) {
			if( strlen( (*it)->pStrFrom ) < strlen( tag->pStrFrom ) ) {
				m_vpAbuseWordList.insert( it, tag );
				bInsert = true;
				break;
			}
		}
		if(bInsert == false) {
		//if( it == m_vpAbuseWordList.end() ) {
			m_vpAbuseWordList.push_back( tag );
		}

	} else {
		m_vpAbuseWordList.push_back( tag );
	}
	return true;
}

bool SPAbuseFilter::AddBenWord( const char* pStrBan )
{
	if( !pStrBan ) return false;	

	char* pszTmpBan;
	pszTmpBan = new char[ strlen(pStrBan) +1 ];

	strcpy( pszTmpBan, pStrBan );

	static COUNTRY_TYPE ct = SPLocalizeManager::GetInstance()->GetCountryType();
	if(ct == CT_KOREA) {
		_strlwr( pszTmpBan );
	}	
	Arrange( pszTmpBan );

	// ﾅｫｰﾍｿ・ｱﾀｸｷ?ﾄ｡ﾈｯｵﾇｾ﨨ﾟ ﾇﾏｱ篩｡..
	if( m_vpszBanWordList.size() ) {
		std::vector< char* >::iterator it;
		bool bInsert = false;
		for( it = m_vpszBanWordList.begin(); it != m_vpszBanWordList.end(); ++it ) {
			if( strlen( (*it) ) < strlen( pszTmpBan ) ) {
				m_vpszBanWordList.insert( it, pszTmpBan );
				bInsert = true;
				break;
			}
		}
		if(bInsert ==  false) {
		//if( it == m_vpszBanWordList.end() ) {
			m_vpszBanWordList.push_back( pszTmpBan );
		}

	} else {
		m_vpszBanWordList.push_back( pszTmpBan );
	}

	return true;
}


bool SPAbuseFilter::Filter( const char* pStr, char* pBuffer, unsigned iLen, int* pIsReplaced )
{
	bool bFlag = false;

	if( !pBuffer || !iLen ) return false;

	if( pIsReplaced ) {
		*pIsReplaced = 0;
	}

	WCHAR* pwszStr = new WCHAR[1024];
	WCHAR* pwszBuffer = new WCHAR[iLen];
	WCHAR* pwszCompareSrc; 
	WCHAR* pwszChange;

	SPAbuseFilter::GetInstance()->ConvertAnsiStringToWide(pwszStr , pStr);
	

	ZeroMemory(pwszBuffer, iLen* sizeof(WCHAR));
	wcsncpy(pwszBuffer, pwszStr, iLen-1 );

	WCHAR *p;
	WCHAR *pLower = new WCHAR[iLen];
	

	bool bIsReplaced = true;
	for( unsigned cnt = 0; cnt < m_vpAbuseWordList.size(); cnt++ ) {

		if( bIsReplaced ) {
			lstrcpyW( pLower , pwszBuffer );			
			_wcslwr( pLower );
		}
		
		unsigned iReplaceLen;
		pwszCompareSrc = new WCHAR[ strlen( m_vpAbuseWordList[cnt]->pStrFrom) + 1 ];
		ConvertAnsiStringToWide( pwszCompareSrc, m_vpAbuseWordList[cnt]->pStrFrom);		

		pwszChange = new WCHAR[ strlen( m_vpAbuseWordList[cnt]->pStrTo) + 1 ];		
		ConvertAnsiStringToWide( pwszChange, m_vpAbuseWordList[cnt]->pStrTo);		

		p = FindWordW( pLower, pwszCompareSrc, iReplaceLen);
		if( !p ) {
			bIsReplaced = true;
			delete [] pwszCompareSrc;
			delete [] pwszChange;
			continue;
		}

		if( pIsReplaced != NULL ) {
			*pIsReplaced = 1;
		}

		WCHAR u[1024];
		lstrcpyW( u, pwszCompareSrc );

		bIsReplaced = true;

		WCHAR* tmp = new WCHAR[ lstrlenW( pLower ) + lstrlenW( pwszChange ) + 1 ];
		memset( tmp, 0, (lstrlenW( pLower ) + lstrlenW( pwszChange ) + 1) * sizeof(WCHAR) );

		if( p - pLower ) memcpy( tmp, pLower, (p - pLower) * sizeof(WCHAR)  );
		if( lstrlenW( pwszChange) ) wcscat( tmp, pwszChange );

		// ﾄ｡ﾈｯｸｻ ｴﾃｿｩｾｲｱ・ｾﾈｵﾊ..		
		p += iReplaceLen;
		wcscat( tmp, p );
		lstrcpyW( pwszBuffer, tmp );

		bIsReplaced = true;

		delete [] tmp;
		delete [] pwszCompareSrc;
		delete [] pwszChange;
		cnt--;	
	}

	SPAbuseFilter::GetInstance()->ConvertWideToAnsiString( pBuffer, pwszBuffer );

	delete [] pLower;
	delete [] pwszStr;
	delete [] pwszBuffer;


	return true;
}


bool SPAbuseFilter::IsBanName( const char* pStr)
{
	bool bFlag = false;

	if(pStr == NULL) return false;

	WCHAR *p;
	WCHAR *pLower = new WCHAR[strlen(pStr) + 1];
	WCHAR *pwszSrc = new WCHAR[strlen(pStr) + 1];
	WCHAR *pwszFrom;
	
	ConvertAnsiStringToWide(pwszSrc, pStr);

	bool bIsReplaced = true;
	for( unsigned cnt = 0; cnt < m_vpszBanWordList.size(); cnt++ ) {

		if( bIsReplaced ) {

			lstrcpyW( pLower , pwszSrc );			
			_wcslwr( pLower );

			//strcpy( pLower , pStr );
			//_strlwr( pLower );			
		}

		unsigned iReplaceLen;
		pwszFrom = new WCHAR[strlen(m_vpszBanWordList[cnt])+1];
		ConvertAnsiStringToWide(pwszFrom, m_vpszBanWordList[cnt]);

		p = FindWordW( pLower, pwszFrom, iReplaceLen);
		delete [] pwszFrom;

		if( p ) {
			bFlag = true;
			break;
		}		
	}

	delete [] pwszSrc;
	delete [] pLower;
	return bFlag;
}



bool SPAbuseFilter::ReplaceLineFeed( const char* pStr, char* pBuffer, unsigned iLen)
{
	bool bFlag = false;
	if( !pBuffer || !iLen ) return false;

	if( strlen(pStr) > iLen)
		return false;

	memset( pBuffer, 0, iLen );
	//strncpy( pBuffer, pStr, iLen-1 );
	strcpy( pBuffer, pStr );

//	char *p;
	char *pLower = new char[iLen];
	bool bIsReplaced = true;
	

	if( bIsReplaced ) {
		strcpy( pLower , pBuffer );
		//_strlwr( pLower );			
	}

	string strText = pLower;
	const char szTemp3[3] = { 92, 110, '\0' }; // '\','n' ﾀｻ '\n'ﾀｸｷﾎ ｹﾙｲ・Eﾀｧﾇﾘ~~
	const char szTemp4[3] = { 92, 78, '\0' }; // '\','N' ﾀｻ '\n'ﾀｸｷﾎ ｹﾙｲ・Eﾀｧﾇﾘ~~
	int iLine = 1;
	int iFind = 0;
	int iCurPos = 0;

	while ( iCurPos != std::string::npos ) {				
		iCurPos = strText.find(szTemp3, iFind);
		if(iCurPos != std::string::npos ) {
			strText[iCurPos] = ' ';
			strText[iCurPos+1] = '\n';
			iFind = iCurPos + 2;
			++iLine;
			continue;
		}

		iCurPos = strText.find(szTemp4, iFind);
		if(iCurPos != std::string::npos ) {
			strText[iCurPos] = ' ';
			strText[iCurPos+1] = '\n';
			iFind = iCurPos + 2;
			++iLine;
			continue;
		}
	}

	strcpy(pBuffer, strText.c_str());
	
	delete [] pLower;

	return true;
}








