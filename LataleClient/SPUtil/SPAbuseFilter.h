
// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPAbuseFilter
// Comment     : 
// Creation    : metalgeni 2005-09-02 ¿ÀÈÄ 2:56:36
//***************************************************************************

#pragma once 

#include <vector>

using namespace std;


enum TCodeType { 
	ctError = -1, 
	ctEng, 
	ctFirst, 
	ctLast 
};


class SPAbuseFilter {
public:
	~SPAbuseFilter();
	static SPAbuseFilter* GetInstance();
	static SPAbuseFilter* Create();
	static void Release();
	bool Init();

	bool Filter( const char* pStr, char* pBuffer, unsigned iLen, int* pIsReplaced = NULL);	
	bool ReplaceLineFeed( const char* pStr, char* pBuffer, unsigned iLen);
	bool IsBanName( const char* pStr);

	bool IsStringWithSpecialChar(const char* pStr);
	bool IsStringWithSpecialCharJapan(const char* pStr);

	TCodeType  IsHangul(const char* OutText, int Len);
	void ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource);
	void ConvertWideToAnsiString( CHAR* strDestination, const WCHAR* wstrSource);
	int GetAnsiLenToWideLen(int iPos, const char* szAnsi);
	int GetAnsiLenToWideLen(int iPos, const WCHAR* wszWide);
	int GetWideLenToAnsiLen(int iPosW, const WCHAR* wszWide);


private:
	SPAbuseFilter();

	class SPAbuseTag {
	public:
		char* pStrFrom;
		char* pStrTo; 
		~SPAbuseTag();
	};

	bool	IsSpecialChar( const char p );
	bool	IsEqual( const char p1, const char p2 );


	bool	IsSpecialCharW	( const WCHAR cChar );
	bool	IsEqualW		( const WCHAR cChar1, const WCHAR cChar2 );

	void	Arrange( char* str );
	char*	FindWord( const char* pStr, const char* pStrWord, unsigned& iReplaceLen );
	WCHAR*	FindWordW( const WCHAR* pwStr, const WCHAR* pwStrWord, unsigned& iReplaceLen );
	bool	AddRule( const char* pStrFrom, const char* pStrTo );
	bool	AddBenWord( const char* pStrBan );

private:

	static SPAbuseFilter* s_pAbuseFilter;

	std::vector< SPAbuseTag* >	m_vpAbuseWordList;	
	std::vector< char* >		m_vpszBanWordList;



};







