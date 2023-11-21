

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <Imm.h>

#include "SPCoreDef.h"
#include "SPDebug.h"

#include "SPMainGameDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPLocalizeManager.h"
#include "SPWindowDEF.h"
#include "SPIMEPool.h"
#include "SPInterfaceManager.h"
#include "SPAbuseFilter.h"

extern HWND g_hWnd;

//#define GETPROCADDRESS( Module, APIName, Temp ) \
//	Temp = GetProcAddress( Module, #APIName ); \
//	if( Temp ) \
//	*(FARPROC*)&_##APIName = Temp
//
//#define PLACEHOLDERPROC( APIName ) \
//	_##APIName = Dummy_##APIName
//
//#define IMM32_DLLNAME L"\\imm32.dll"
//#define VER_DLLNAME L"\\version.dll"


#define CHT_IMEFILENAME1    "TINTLGNT.IME" // New Phonetic
#define CHT_IMEFILENAME2    "CINTLGNT.IME" // New Chang Jie
#define CHT_IMEFILENAME3    "MSTCIPHA.IME" // Phonetic 5.1
#define CHS_IMEFILENAME1    "PINTLGNT.IME" // MSPY1.5/2/3
#define CHS_IMEFILENAME2    "MSSCIPYA.IME" // MSPY3 for OfficeXP

#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
#define _CHT_HKL            ( (HKL)(INT_PTR)0xE0080404 ) // New Phonetic
#define _CHT_HKL2           ( (HKL)(INT_PTR)0xE0090404 ) // New Chang Jie
#define _CHS_HKL            ( (HKL)(INT_PTR)0xE00E0804 ) // MSPY
#define MAKEIMEVERSION( major, minor ) \
( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )

#define IMEID_CHT_VER42 ( LANG_CHT | MAKEIMEVERSION( 4, 2 ) )   // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
#define IMEID_CHT_VER43 ( LANG_CHT | MAKEIMEVERSION( 4, 3 ) )   // New(Phonetic/ChanJie)IME98a : 4.3.x.x // Win2k
#define IMEID_CHT_VER44 ( LANG_CHT | MAKEIMEVERSION( 4, 4 ) )   // New ChanJie IME98b          : 4.4.x.x // WinXP
#define IMEID_CHT_VER50 ( LANG_CHT | MAKEIMEVERSION( 5, 0 ) )   // New(Phonetic/ChanJie)IME5.0 : 5.0.x.x // WinME
#define IMEID_CHT_VER51 ( LANG_CHT | MAKEIMEVERSION( 5, 1 ) )   // New(Phonetic/ChanJie)IME5.1 : 5.1.x.x // IME2002(w/OfficeXP)
#define IMEID_CHT_VER52 ( LANG_CHT | MAKEIMEVERSION( 5, 2 ) )   // New(Phonetic/ChanJie)IME5.2 : 5.2.x.x // IME2002a(w/Whistler)
#define IMEID_CHT_VER60 ( LANG_CHT | MAKEIMEVERSION( 6, 0 ) )   // New(Phonetic/ChanJie)IME6.0 : 6.0.x.x // IME XP(w/WinXP SP1)
#define IMEID_CHS_VER41 ( LANG_CHS | MAKEIMEVERSION( 4, 1 ) )   // MSPY1.5  // SCIME97 or MSPY1.5 (w/Win98, Office97)
#define IMEID_CHS_VER42 ( LANG_CHS | MAKEIMEVERSION( 4, 2 ) )   // MSPY2    // Win2k/WinME
#define IMEID_CHS_VER53 ( LANG_CHS | MAKEIMEVERSION( 5, 3 ) )   // MSPY3    // WinXP

WCHAR		SPIMEPool::s_aszIndicator[5][3] = // String to draw to indicate current input locale
{
	L"En",
	L"\x7B80",
	L"\x7E41",
	L"\xAC00",
	L"\x3042",
};
LPWSTR		SPIMEPool::s_wszCurrIndicator   // Points to an indicator string that corresponds to current input locale
= SPIMEPool::s_aszIndicator[0];

HKL			SPIMEPool::s_hklCurrent;
bool		SPIMEPool::s_bChineseIME;
bool		SPIMEPool::s_bInsertOnType;
HIMC		SPIMEPool::s_hIMC;

SPIMEPool::IMESTATE  SPIMEPool::s_ImeState = IMEUI_STATE_OFF;


SPIMEPool::SPIMEPool()
: m_iNoSkipChar(0)
, m_iNoExCarrotPos(0)
{
	ClearBuffer();

	//m_nSelStart = m_nSelStart = m_nEnd = -1;

	m_nSelStart = 0;

	m_bComp = FALSE;
	m_nCaret = 0;
	m_nCompCaret = 0;

	m_bFocus = false;
	m_bUpdate = false;
	m_bCandUpdate = false;

	m_nLength = MAX_STRING;
	ZeroMemory( m_wszString, sizeof(m_wszString) );
	ZeroMemory( m_wszComp,   sizeof(m_wszComp) );
	ZeroMemory( m_wszClip,   sizeof(m_wszClip));

	m_bIsEnglishMode = true;
	m_bNumber = false;
	m_bPassword = false;	//!< added by jmulro.

	m_bVerticalCand = false;

	// ?EE
	m_bUnknown = false;

	m_bInsertMode = true;
	m_bMultiLine = false;
	m_bHoldAddChar = false;

	ZeroMemory( &m_ptStatusWindowPos		, sizeof(POINT));
	ZeroMemory( &m_ptCandidateWindowPos		, sizeof(POINT));	
	ZeroMemory( &m_szDivide					, sizeof(m_szDivide));	 

	//m_bEngishOnly = !(SPLocalizeManager::GetInstance()->GetCountryType() == CT_OGP);
	m_bEngishOnly = false;

	// Save the default input context
	s_hIMC = ImmGetContext( g_hWnd );
	ImmReleaseContext( g_hWnd, s_hIMC );
}

SPIMEPool::~SPIMEPool()
{
}

void SPIMEPool::Refreshed()
{ 
	m_bUpdate = false; 
}

void SPIMEPool::CandRefreshed()
{
	m_bCandUpdate = false;
}

bool SPIMEPool::IsComposition() 
{
	if(s_bInsertOnType == true) // Korean IME is return always
		return false;


	if(lstrlenW(m_wszComp) != 0) {
		return true;
	}

	return false;
	//return m_bComp; // But Japanese, Chinese IME can't return when composition.
}

bool SPIMEPool::IsChina()
{
	if( GetPrimaryLanguage() == LANG_CHINESE)
		return true;

	return false;
}

void SPIMEPool::SetSel( int nStart, int nEnd )
{	
	if(nStart == -1 ) {
		m_nSelStart = m_nCaret;
		return;
	}

	// for Ansi Position
	SetCaretPos( nEnd ); 
	m_nSelStart = SPAbuseFilter::GetInstance()->GetAnsiLenToWideLen (nStart, m_wszString);	
	m_bUpdate = true;
}

void SPIMEPool::SetSelW( int nStart, int nEnd )
{	
	if(nStart == -1 ) {
		m_nSelStart = m_nCaret;
		return;
	}

	// for Ansi Position
	SetCaretPosW( nEnd ); 
	m_nSelStart = nStart;	
	m_bUpdate = true;
}


const WCHAR* SPIMEPool::GetStringW()
{
	static WCHAR wszTemp[MAX_STRING];	

	StringCchCopyW(wszTemp, MAX_STRING, m_wszString);
	if(!s_bInsertOnType) {		
		if(m_nCaret + lstrlenW(m_wszComp) < MAX_STRING) {
			CopyMemory(wszTemp+m_nCaret, m_wszComp, lstrlenW(m_wszComp)* sizeof(WCHAR));
			if( m_nCaret + lstrlenW(m_wszComp) > lstrlenW(m_wszString)) {
				wszTemp[m_nCaret + lstrlenW(m_wszComp)] = '\0';
			}
		}
	}	
	return wszTemp;
}


const char* SPIMEPool::GetString() 
{ 
	WCHAR wszTemp[MAX_STRING];	

	StringCchCopyW(wszTemp, MAX_STRING, m_wszString);
	if(!s_bInsertOnType) {		
		if(m_nCaret + lstrlenW(m_wszComp) < MAX_STRING) {
			CopyMemory(wszTemp+m_nCaret, m_wszComp, lstrlenW(m_wszComp)* sizeof(WCHAR));
			if( m_nCaret + lstrlenW(m_wszComp) > lstrlenW(m_wszString)) {
				wszTemp[m_nCaret + lstrlenW(m_wszComp)] = '\0';
			}
		}
	}	
	static char szTemp[MAX_STRING];
	SPAbuseFilter::GetInstance()->ConvertWideToAnsiString(szTemp, wszTemp);
	return szTemp; 

}

void SPIMEPool::SetSelectPos(int iPos)
{
	m_nSelStart = SPAbuseFilter::GetInstance()->GetAnsiLenToWideLen(iPos, m_wszString);
}

// for external use
void SPIMEPool::SetCaretPos(int iPos)
{
	char szTempStr[MAX_STRING];
	int iLen;

	if(iPos == -1) {
		iLen = lstrlenW(m_wszString);
		m_nCaret = iLen;
	} else {
		SPAbuseFilter::GetInstance()->ConvertWideToAnsiString(szTempStr, m_wszString);	
		iLen = lstrlenA(szTempStr);
		if(iPos >= iLen + 1)
			iPos = iLen + 1;

		iPos = SPAbuseFilter::GetInstance()->GetAnsiLenToWideLen(iPos, szTempStr);
		m_nCaret = iPos;
	}	

	//m_nSelStart = m_nCaret;	
	m_bUpdate = true;
}

bool SPIMEPool::IsWideCaret()
{
	if(lstrlenW(m_wszComp) > 1) {
		return true;
	}
	return false;


	if(m_abCompStringAttr[0] == ATTR_INPUT) { // Check only first char
		return false;
	}
	return true;
}

int SPIMEPool::GetCompLen()
{
	int iLen = lstrlenW(m_wszComp);
	return SPAbuseFilter::GetInstance()->GetWideLenToAnsiLen( iLen, m_wszComp );
}

int SPIMEPool::GetCompLenW()
{
	return lstrlenW(m_wszComp);
}

const WCHAR* SPIMEPool::GetCompStringW()
{
	return m_wszComp;
}

const char* SPIMEPool::GetCompString()
{
	static char szTemp[MAX_STRING];
	SPAbuseFilter::GetInstance()->ConvertWideToAnsiString(szTemp, m_wszComp);
	return szTemp;
}

void SPIMEPool::Deselect()
{
	m_nSelStart = m_nCaret;
}


//private
void SPIMEPool::SetCaretPosW(int iPos)
{	
	int iLen = lstrlenW(m_wszString);
	if(iPos == -1) {
		m_nCaret = iLen;
	} else {
		if(iPos >= iLen + 1)
			iPos = iLen + 1;

		m_nCaret = iPos;
	}	
	//m_nSelStart = m_nCaret;	

	m_bUpdate = true;
}

void SPIMEPool::GetSel( int& nStart, int& nEnd )
{
	nStart = SPAbuseFilter::GetInstance()->GetWideLenToAnsiLen( m_nSelStart, m_wszString);
	nEnd = SPAbuseFilter::GetInstance()->GetWideLenToAnsiLen( m_nCaret, m_wszString );
}

void SPIMEPool::GetSelW( int& nStart, int& nEnd )
{
	nStart = m_nSelStart;
	nEnd = m_nCaret;
}


int SPIMEPool::GetCaretPos() 
{ 
	return SPAbuseFilter::GetInstance()->GetWideLenToAnsiLen( m_nCaret, m_wszString );	
}

int SPIMEPool::GetCaretPosW() 
{ 
	return m_nCaret;	
}

int SPIMEPool::GetStringLen() 
{ 	
	return SPAbuseFilter::GetInstance()->GetWideLenToAnsiLen( m_nStringLen, m_wszString ); 
}

int SPIMEPool::GetStringLenW() 
{
	return lstrlenW(m_wszString);
	//return m_nStringLen;	
}


void SPIMEPool::SetLimitText( int nLength )
{ 
	m_nLength = nLength; 
	if(m_nLength > MAX_STRING)
		m_nLength = MAX_STRING;
}

void SPIMEPool::SetString( const char* szStr )
{ 	
	ZeroMemory( m_wszString, MAX_STRING );	

	SPAbuseFilter::GetInstance()->ConvertAnsiStringToWide(m_wszString, szStr);	

	m_nStringLen = lstrlenW(m_wszString); 

	m_nSelStart = 0; //  Do Select

	m_nCaret = m_nStringLen;
	
	m_bUpdate = true; 
}


void SPIMEPool::SetStringW( const WCHAR* wszStr )
{ 	
	ZeroMemory( m_wszString, MAX_STRING );	
	
	wcscpy( m_wszString, wszStr);

	//int iWideLen =SPAbuseFilter::GetInstance()->GetAnsiLenToWideLen( m_nLength, m_wszString);
	m_wszString[m_nLength] = '\0';

	m_nStringLen = lstrlenW(m_wszString); 

	m_nSelStart = 0; //  Do Select

	m_nCaret = m_nStringLen;

	m_bUpdate = true; 
}




void SPIMEPool::ClearBuffer() 
{ 
	ZeroMemory(m_wszString, MAX_STRING); 
	m_nStringLen = 0; 
}


void SPIMEPool::Purge() 
{
	LONG len;
	if (Enter()) 
	{
		ImmSetCompositionString(m_hIMC, SCS_SETSTR, NULL, 0, 0, 0 );
		//if ((len = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, NULL, 0)) > 0) 
		//{
		//	ImmGetCompositionString(m_hIMC, GCS_COMPSTR, m_wszComp, len);
		//}
		Leave();
	}
}


void SPIMEPool::EnableIME()
{	
	ImmAssociateContext(g_hWnd, s_hIMC);
	m_hIMC = ImmGetContext(g_hWnd);
}

void SPIMEPool::DisableIME()
{
	ImmReleaseContext( g_hWnd, m_hIMC );
	ImmAssociateContext(g_hWnd, NULL);
}

void SPIMEPool::SetFocus( bool bFocus )
{ 
	m_bUpdate = true;
	m_bFocus = bFocus; 
	m_nSelStart = 0;
	//m_nSelStart = -1;
	//m_nEnd = -1;
	m_bComp	=	FALSE;	// AJJIYA [8/9/2005]

	//Leave();
	if( bFocus )
	{
		//Enter();
		
		if(m_bEngishOnly == false)
			EnableIME();

		//SendMessage(g_hWnd, WM_IME_NOTIFY, IMN_CLOSECANDIDATE, 0);
		ImmNotifyIME( m_hIMC, NI_CLOSECANDIDATE, 0, 0);

		// ﾀﾌﾀ・ｸ蟾ﾎ ｹﾙｲﾞ
		//if( m_bIsEnglishMode )
		//	EnableEnglishMode();
		//else
		//	EnableHagulMode();

	} else {
		KillFocus();
	}
}


void SPIMEPool::SetMultiLine(bool bMultiLine)
{
	m_bMultiLine = bMultiLine;
}

void SPIMEPool::SetHoldAddChar(bool bHold)
{
	m_bHoldAddChar = bHold;
}


void SPIMEPool::PreKillFocus()
{
	if(!s_bInsertOnType) {
		ResetCompositionString();
	}	
	m_bUpdate = true;
}

void SPIMEPool::KillFocus()
{
	ResetCompositionString();

#ifdef _DEBUG
	const WCHAR* pwszString = GetStringW();
#endif

	m_bIsEnglishMode = IsEnglishMode();

	ZeroMemory(m_wszString, sizeof(m_wszString));

	m_CandList.bShowWindow = false;
	//if( !s_bShowReadingWindow )
	{
		m_CandList.dwCount = 0;
		ZeroMemory( m_CandList.awszCandidate, sizeof(m_CandList.awszCandidate) );
	}
	m_bCandUpdate = true;

	//SendMessage(g_hWnd, WM_IME_NOTIFY, IMN_CLOSECANDIDATE, 0);

	ImmNotifyIME( m_hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
	ImmNotifyIME( m_hIMC, NI_CLOSECANDIDATE, 0, 0);

	m_bUpdate = true;

	DisableIME();

	m_bMultiLine = false;
}


HIMC SPIMEPool::Enter() 
{
	return (m_hIMC = ImmGetContext( g_hWnd ));
}

void SPIMEPool::Leave()
{ 
	ImmReleaseContext( g_hWnd, m_hIMC ); 
}

// UTIL
bool SPIMEPool::IsEnglishMode()
{
	HIMC hIMC = ImmGetContext(g_hWnd);
	DWORD dwConv, dwSent;

	ImmGetConversionStatus(hIMC, &dwConv, &dwSent);

	bool bEnglish;
	switch( dwConv & IME_CMODE_LANGUAGE )
	{
	default :
	case IME_CMODE_ALPHANUMERIC:
		bEnglish = true;
		break;
	case IME_CMODE_NATIVE:
		bEnglish = false;
		break;
	}
	ImmReleaseContext(g_hWnd, hIMC);

	return bEnglish;
}

void SPIMEPool::EnableHagulMode()
{
	DXUTOutputDebugString("Don't call this function..!! plz~\n");
	return;

	HIMC hIMC = ImmGetContext(g_hWnd);
	ImmSetConversionStatus(hIMC, IME_CMODE_NATIVE, IME_SMODE_NONE);	
	ImmReleaseContext(g_hWnd, hIMC);
}

void SPIMEPool::EnableEnglishMode()
{
	DXUTOutputDebugString("Don't call this function..!! plz~\n");
	return;

	HIMC hIMC = ImmGetContext(g_hWnd);

	DWORD dwConversion, dwSentence;
	ImmGetConversionStatus(hIMC, &dwConversion, &dwSentence);

	// ｹｫﾁｶｰﾇ ﾈ｣ﾃ簓ﾏｸ・ｹｫﾇﾑ ｷ酩ﾁ ｵｷｴﾙ. 
	if(dwConversion != IME_CMODE_ALPHANUMERIC || dwSentence != IME_SMODE_NONE) {
		ImmSetConversionStatus(hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
	}

	ImmReleaseContext(g_hWnd, hIMC);
	SendMessage(g_hWnd, WM_IME_SETCONTEXT, TRUE, 0);
}


//--------------------------------------------------------------------------------------
// Truncate composition string by sending keystrokes to the window.
void SPIMEPool::TruncateCompString( bool bUseBackSpace, int iNewStrLen )
{
	if( !s_bInsertOnType )
		return;	

	int cc = (int) wcslen( m_wszComp );
	assert( iNewStrLen == 0 || iNewStrLen >= cc );

	// Send right arrow keystrokes to move the caret
	//   to the end of the composition string.
	for (int i = 0; i < cc - m_nCompCaret; ++i )
		SendMessage( g_hWnd, WM_KEYDOWN, VK_RIGHT, 0 );
	SendMessage( g_hWnd, WM_KEYUP, VK_RIGHT, 0 );

	if( bUseBackSpace || m_bInsertMode )
		iNewStrLen = 0;

	// The caller sets bUseBackSpace to false if there's possibility of sending
	// new composition string to the app right after this function call.
	// 
	// If the app is in overwriting mode and new comp string is 
	// shorter than current one, delete previous comp string 
	// till it's same long as the new one. Then move caret to the beginning of comp string.
	// New comp string will overwrite old one.
	if( iNewStrLen < cc )
	{
		for( int i = 0; i < cc - iNewStrLen; ++i )
		{
			SendMessage( g_hWnd, WM_KEYDOWN, VK_BACK, 0 );  // Backspace character
			SendMessageW( g_hWnd, WM_CHAR, VK_BACK, 0 );
		}
		SendMessage( g_hWnd, WM_KEYUP, VK_BACK, 0 );

		DXUTOutputDebugString("           Truncate VK_BACK..\n");
	}
	else
		iNewStrLen = cc;

	// Move the caret to the beginning by sending left keystrokes
	for (int i = 0; i < iNewStrLen; ++i )
		SendMessage( g_hWnd, WM_KEYDOWN, VK_LEFT, 0 );
	SendMessage( g_hWnd, WM_KEYUP, VK_LEFT, 0 );
}



void SPIMEPool::CheckInputLocale()
{
	static HKL hklPrev = 0;
	s_hklCurrent = GetKeyboardLayout( 0 );
	if ( hklPrev == s_hklCurrent )
		return;

	s_bInsertOnType = false;
	hklPrev = s_hklCurrent;
	switch ( GetPrimaryLanguage() )
	{
		// Simplified Chinese
	case LANG_CHINESE:
		m_bVerticalCand = true;
		switch ( GetSubLanguage() )
		{
		case SUBLANG_CHINESE_SIMPLIFIED:
			s_wszCurrIndicator = s_aszIndicator[INDICATOR_CHS];
			//m_bVerticalCand = GetImeId() == 0;
			m_bVerticalCand = false;
			break;
		case SUBLANG_CHINESE_TRADITIONAL:
			s_wszCurrIndicator = s_aszIndicator[INDICATOR_CHT];
			break;
		default:    // unsupported sub-language
			s_wszCurrIndicator = s_aszIndicator[INDICATOR_NON_IME];
			break;
		}
		break;
		// Korean
	case LANG_KOREAN:
		s_wszCurrIndicator = s_aszIndicator[INDICATOR_KOREAN];
		m_bVerticalCand = false;
		s_bInsertOnType = true;
		break;
		// Japanese
	case LANG_JAPANESE:
		s_wszCurrIndicator = s_aszIndicator[INDICATOR_JAPANESE];
		m_bVerticalCand = true;
		break;
	default:
		// A non-IME language.  Obtain the language abbreviation
		// and store it for rendering the indicator later.
		s_wszCurrIndicator = s_aszIndicator[INDICATOR_NON_IME];
	}

	// If non-IME, use the language abbreviation.
	if( s_wszCurrIndicator == s_aszIndicator[INDICATOR_NON_IME] )
	{
		WCHAR wszLang[5];
		GetLocaleInfoW( MAKELCID( LOWORD( s_hklCurrent ), SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, wszLang, 5 );
		s_wszCurrIndicator[0] = wszLang[0];
		s_wszCurrIndicator[1] = towlower( wszLang[1] );
	}
}


void SPIMEPool::CheckToggleState()
{
	CheckInputLocale();
	bool bIme = ImmIsIME( s_hklCurrent ) != 0;
	s_bChineseIME = ( GetPrimaryLanguage() == LANG_CHINESE ) && bIme;

	HIMC m_hIMC;
	if( NULL != ( m_hIMC = ImmGetContext( g_hWnd ) ) )
	{
		if( s_bChineseIME )
		{
			DWORD dwConvMode, dwSentMode;
			ImmGetConversionStatus( m_hIMC, &dwConvMode, &dwSentMode );
			s_ImeState = ( dwConvMode & IME_CMODE_NATIVE ) ? IMEUI_STATE_ON : IMEUI_STATE_ENGLISH;
		}
		else
		{
			s_ImeState = ( bIme && ImmGetOpenStatus( m_hIMC ) != 0 ) ? IMEUI_STATE_ON : IMEUI_STATE_OFF;
		}
		ImmReleaseContext( g_hWnd, m_hIMC );
	}
	else
		s_ImeState = IMEUI_STATE_OFF;


	//m_bComp = false;	
}


void SPIMEPool::DeleteSelectionText()
{
	int nFirst = __min( m_nCaret, m_nSelStart );
	int nLast = __max( m_nCaret, m_nSelStart );
	// Update caret and selection
	SetCaretPosW( nFirst );
	m_nSelStart = m_nCaret;
	// Remove the characters
	for( int i = nFirst; i < nLast; ++i )
		RemoveChar( nFirst );
}


void SPIMEPool::SendCompString()
{
	for( int i = 0; i < lstrlenW( m_wszComp ); ++i )
		WndProc( WM_CHAR, (WPARAM)m_wszComp[i], 0 );
}

void SPIMEPool::ResetCompositionString()
{
	m_nCompCaret = 0;	
	m_wszComp[0] = '\0';
	
	//m_bComp = FALSE; // ｱ簔ｸｰ・ﾈ｣ﾈｯｼｺﾀｯﾁ~
	//DXUTOutputDebugString("m_bComp = %d\n", m_bComp);	

	ZeroMemory( m_abCompStringAttr, sizeof(m_abCompStringAttr) );
}



bool SPIMEPool::WndProc(UINT Msg, WPARAM wParam,LPARAM lParam)
{
	if( !m_bFocus ) {
		return false;
	}

	switch (Msg) 
	{
	case WM_IME_CHAR:
		DXUTOutputDebugString("WM_IME_CHAR\n");
		return true;

	case WM_IME_NOTIFY :
		DXUTOutputDebugString("WM_IME_NOTIFY %x %x \n", wParam, lParam );
		//china
		if(IsChina()) {
			OnIMENotify(wParam, lParam);
			DefWindowProc(g_hWnd, Msg, wParam, lParam);
			return true;
		} else {
			return OnIMENotify(wParam, lParam);
		}


	case WM_IME_STARTCOMPOSITION:     // ｱﾛﾀﾚ ﾁｶﾇﾕﾀﾇ ｽﾃﾀﾛ. WM_IME_COMPOSITION ｸﾞｽﾃﾁｦ ｹﾞﾀｻ ﾁﾘｺｦ ﾇﾑｴﾙ. 
		DXUTOutputDebugString("WM_IME_STARTCOMPOSITION %x %x \n", wParam, lParam );
		OnIMEStartComposition(wParam,lParam);

		//china
		if(IsChina())		
			DefWindowProc(g_hWnd, Msg, wParam, lParam);

		return true;
	case WM_IME_ENDCOMPOSITION:         // ｱﾛﾀﾚ ﾁｶﾇﾕﾀﾇ ｳ｡. ﾁｶﾇﾕｵﾈ ｹｮﾀﾚｿｭ ﾃｳｸｮｸｦ ｳ｡ｳｽｴﾙ          break; 
		DXUTOutputDebugString("WM_IME_ENDCOMPOSITION %x %x \n", wParam, lParam );
		OnIMEEndComposition(wParam,lParam);

		//china
		if(IsChina())		
			DefWindowProc(g_hWnd, Msg, wParam, lParam);

		return true;
	case WM_IME_COMPOSITION:          // ﾇ・ﾁｶﾇﾕﾁﾟﾀﾎ ｹｮﾀﾚｿｭﾀﾌｳｪ ｿﾏｼｺｵﾈ ｹｮﾀﾚｿｭﾀｻ ｾ錡ｭ ﾈｭｸ・ﾃ箙ﾂ
		DXUTOutputDebugString("WM_IME_COMPOSITION %x %x \n", wParam, lParam );
		OnIMEComposition(wParam,lParam);

		//china
		if(IsChina())		
			DefWindowProc(g_hWnd, Msg, wParam, lParam);

		return true;
	case WM_IME_SETCONTEXT:			 // ｱﾛﾀﾚ ﾁｶﾇﾕ ﾀｩｵｵｿ・?ﾇﾑﾀﾚ ｺｯﾈｯ ﾀｩｵｵｿ・?ﾇ･ｽﾃﾇﾏﾁ・ｾﾊｰﾔ ｹﾙｲﾞ
		//DXUTOutputDebugString("WM_IME_SETCONTEXT %x %x \n", wParam, lParam );
		//lParam &= ~(ISC_SHOWUICOMPOSITIONWINDOW | ISC_SHOWUIALLCANDIDATEWINDOW);
		lParam = 0;
		//lParam &= ~ISC_SHOWUIALL;	// ｸ・ﾀｩｵｵｿ・?ﾇ･ｽﾃ ﾇﾏﾁ・ｾﾊﾀｽ.
		//DefWindowProc(g_hWnd, WM_IME_SETCONTEXT, wParam, lParam);
		//ImmIsUIMessage(g_hWnd, WM_IME_SETCONTEXT, wParam, 0);

		if(IsChina()) {		
			DefWindowProc(g_hWnd, Msg, wParam, lParam);
			return true;
		}

		return false;

	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		//DXUTOutputDebugString("WM_KEYDOWN %x %x \n", wParam, lParam );
		//ﾃｪﾈｺﾅ荳ｮ ｱ箒ﾉｶｧｹｮｿ｡..InterfaceManagerｿ｡ｼｭ ﾃｳｸｮﾇﾑｴﾙ.
		if(wParam == VK_DOWN || wParam == VK_UP) {
			return false;
		}


		if(IsChina()) {		
			OnIMEKeyDown(wParam, lParam);
			DefWindowProc(g_hWnd, Msg, wParam, lParam);
			return true;
		}

		return OnIMEKeyDown(wParam, lParam);

	case WM_SYSKEYUP:
	case WM_KEYUP:
		//DXUTOutputDebugString("WM_KEYUP %x %x \n", wParam, lParam );
		return OnIMEKeyUp(wParam, lParam);
		//return true;
	case WM_CHAR:		
		//DXUTOutputDebugString("WM_CHAR %x %x \n", wParam, lParam );
		OnIMEChar(wParam, lParam);
		if(IsChina())		
			DefWindowProc(g_hWnd, Msg, wParam, lParam);

		return true;	
	case WM_INPUTLANGCHANGE:
		//DXUTOutputDebugString("WM_INPUTLANGCHANGE %x %x \n", wParam, lParam );
		{		
			UINT uLang = GetPrimaryLanguage();
			CheckToggleState();
			if ( uLang != GetPrimaryLanguage() )
			{
				// Korean IME always inserts on keystroke.  Other IMEs do not.
				s_bInsertOnType = ( GetPrimaryLanguage() == LANG_KOREAN );
			}
			//if ( NULL != ( m_hIMC = ImmGetContext( g_hWnd ) ) )
			//{
			//	ShowReadingWindow( m_hIMC, false );
			//	ImmReleaseContext( g_hWnd, m_hIMC );
			//}
		}

		//china
		if(IsChina())		
			DefWindowProc(g_hWnd, Msg, wParam, lParam);
		return true;
	}

	return false;
}

bool SPIMEPool::OnIMENotify(WPARAM wParam, LPARAM lParam)
{

	switch( wParam )
	{
	case IMN_SETOPENSTATUS :
	case IMN_SETCONVERSIONMODE :
		{
			DXUTOutputDebugString(wParam == IMN_SETOPENSTATUS ? "	IMN_SETOPENSTATUS..\n" : " IMN_SETCONVERSIONMODE..\n" );					
			CheckToggleState();
		}
		break;	

	case IMN_OPENCANDIDATE:
	case IMN_CHANGECANDIDATE:
		{	

			DXUTOutputDebugString( wParam == IMN_CHANGECANDIDATE ? "  IMN_CHANGECANDIDATE\n" : "  IMN_OPENCANDIDATE\n" );

			//CFS_CANDIDATEPOS
			//break;

			//china
			if(IsChina())
				break;

			
			m_CandList.bShowWindow = true;

			if( NULL == ( m_hIMC = ImmGetContext( g_hWnd ) ) )
				break;

			LPCANDIDATELIST lpCandList = NULL;
			DWORD dwLenRequired;
			DWORD dwListCount;
			DWORD dwListRet;

			//s_bShowReadingWindow = false;
			// Retrieve the candidate list
			dwLenRequired = ImmGetCandidateListW( m_hIMC, 0, NULL, 0 );
			DXUTOutputDebugString( " Required Length == %d \n", dwLenRequired );
			if( dwLenRequired )
			{
				dwListRet = ImmGetCandidateListCountW(m_hIMC, &dwListCount);

				lpCandList = (LPCANDIDATELIST)HeapAlloc( GetProcessHeap(), 0, dwLenRequired );
				dwLenRequired = ImmGetCandidateListW( m_hIMC, 0, lpCandList, dwLenRequired );
			}		



			if( lpCandList )
			{
				// Update candidate list data
				m_CandList.dwSelection = lpCandList->dwSelection;
				m_CandList.dwCount = lpCandList->dwCount;

				int nPageTopIndex = 0;
				m_CandList.dwPageSize = __min( lpCandList->dwPageSize, MAX_CANDLIST );
				if( GetPrimaryLanguage() == LANG_JAPANESE )
				{
					// Japanese IME organizes its candidate list a little
					// differently from the other IMEs.
					nPageTopIndex = ( m_CandList.dwSelection / m_CandList.dwPageSize ) * m_CandList.dwPageSize;
				}
				else
					nPageTopIndex = lpCandList->dwPageStart;

				// Make selection index relative to first entry of page
				//m_CandList.dwSelection = ( GetLanguage() == LANG_CHS && !GetImeId() ) ? (DWORD)-1
				//	: m_CandList.dwSelection - nPageTopIndex;

				m_CandList.dwSelection = ( GetLanguage() == LANG_CHS ) ? (DWORD)-1
					: m_CandList.dwSelection - nPageTopIndex;


				ZeroMemory( m_CandList.awszCandidate, sizeof(m_CandList.awszCandidate) );
				for( UINT i = nPageTopIndex, j = 0;
					(DWORD)i < lpCandList->dwCount && j < m_CandList.dwPageSize;
					i++, j++ )
				{
					// Initialize the candidate list strings
					LPWSTR pwsz = m_CandList.awszCandidate[j];
					// For every candidate string entry,
					// write [index] + Space + [String] if vertical,
					// write [index] + [String] + Space if horizontal.

					//*pwsz++ = (WCHAR)( L'0' + ( (j + 1) % 10 ) );  // Index displayed is 1 based
					//if( m_bVerticalCand )
					//	*pwsz++ = L' ';

					WCHAR *pwszNewCand = (LPWSTR)( (LPBYTE)lpCandList + lpCandList->dwOffset[i] );
					while ( *pwszNewCand )
						*pwsz++ = *pwszNewCand++;
					if( !m_bVerticalCand )
						*pwsz++ = L' ';
					*pwsz = 0;  // Terminate
				}

				// Make dwCount in m_CandList be number of valid entries in the page.
				//m_CandList.dwCount = lpCandList->dwCount - lpCandList->dwPageStart;
				//if( m_CandList.dwCount > lpCandList->dwPageSize )
				//	m_CandList.dwCount = lpCandList->dwPageSize;

				HeapFree( GetProcessHeap(), 0, lpCandList );
				ImmReleaseContext( g_hWnd, m_hIMC );

				// Korean and old Chinese IME can't have selection.
				// User must use the number hotkey or Enter to select
				// a candidate.
				if( GetPrimaryLanguage() == LANG_KOREAN ||
					//GetLanguage() == LANG_CHT && !GetImeId() )
					GetLanguage() == LANG_CHT )
				{
					m_CandList.dwSelection = (DWORD)-1;
				}

				// Initialize m_CandList.wstrHoriCand if we have a
				// horizontal candidate window.
				if( !m_bVerticalCand )
				{
					WCHAR wszCand[256] = L"";

					m_CandList.nFirstSelected = 0;
					m_CandList.nHoriSelectedLen = 0;
					for( UINT i = 0; i < MAX_CANDLIST; ++i )
					{
						if( m_CandList.awszCandidate[i][0] == L'\0' )
							break;

						WCHAR wszEntry[64];
						StringCchPrintfW( wszEntry, 32, L"%s ", m_CandList.awszCandidate[i] );
						// If this is the selected entry, mark its char position.
						if( m_CandList.dwSelection == i )
						{
							m_CandList.nFirstSelected = lstrlenW( wszCand );
							m_CandList.nHoriSelectedLen = lstrlenW( wszEntry ) - 1;  // Minus space
						}
						StringCchCatW( wszCand, 255, wszEntry );
					}
					wszCand[lstrlenW(wszCand) - 1] = L'\0';  // Remove the last space
					m_CandList.wstrHoriCand =  wszCand ;
				}				
			}	
			m_bCandUpdate = true;
		}		
		break;

	case IMN_CLOSECANDIDATE:
		{
			if(IsChina())
				break;

			DXUTOutputDebugString("  IMN_CLOSECANDIDATE\n" );
			//break;
			m_CandList.bShowWindow = false;
			//if( !s_bShowReadingWindow )
			{
				m_CandList.dwCount = 0;
				ZeroMemory( m_CandList.awszCandidate, sizeof(m_CandList.awszCandidate) );
			}
			m_bCandUpdate = true;
		}
		break;

	case IMN_OPENSTATUSWINDOW:
		{
			//china
			DXUTOutputDebugString("		IMN_OPENSTATUSWINDOW\n");
			m_hIMC = ImmGetContext(g_hWnd);


			ImmSetStatusWindowPos(	m_hIMC, &m_ptStatusWindowPos);
			ImmReleaseContext(g_hWnd, m_hIMC);					

		}
		break;

	case IMN_PRIVATE: // Chinese only
		{
			DXUTOutputDebugString("  IMN_PRIVATE\n" );
		}
		break;
	}
	return false;

}


void SPIMEPool::OnIMEStartComposition(WPARAM wParam, LPARAM lParam)
{
	ResetCompositionString();	

	m_bComp = TRUE;	
	
}

void SPIMEPool::OnIMEComposition(WPARAM wParam, LPARAM lParam)
{

	LONG lRet;  // Returned count in CHARACTERS
	WCHAR wszCompStr[MAX_STRING ];	
	
	if( NULL == ( m_hIMC = ImmGetContext( g_hWnd ) ) )
	{
		return;
	}

	// Get the caret position in composition string
	if ( lParam & GCS_CURSORPOS )
	{
		DXUTOutputDebugString("GCS_CURSORPOS\n");
		m_nCompCaret = ImmGetCompositionStringW( m_hIMC, GCS_CURSORPOS, NULL, 0 );
		if( m_nCompCaret < 0 )
			m_nCompCaret = 0; // On error, set caret to pos 0.
	}


	m_bUpdate = true;


	if ( lParam & GCS_RESULTSTR )
	{	
		DXUTOutputDebugString("GCS_RESULTSTR\n");
		lRet = ImmGetCompositionStringW( m_hIMC, GCS_RESULTSTR, wszCompStr, sizeof( wszCompStr ) );
		if( lRet > 0 )
		{
			lRet /= sizeof(WCHAR);
			wszCompStr[lRet] = 0;  // Force terminate
			TruncateCompString( false, (int)wcslen( wszCompStr ) );

			StringCchCopyW(m_wszComp, MAX_STRING, wszCompStr);

			SendCompString();

			ResetCompositionString();
		}
	}

	//
	// Reads in the composition string.
	//
	if ( lParam & GCS_COMPSTR )
	{
		DXUTOutputDebugString("GCS_COMPSTR\n");
		//////////////////////////////////////////////////////
		// Retrieve the latest user-selected IME candidates
		lRet = ImmGetCompositionStringW( m_hIMC, GCS_COMPSTR, wszCompStr, sizeof( wszCompStr ) );
		if( lRet > 0 )
		{
			lRet /= sizeof(WCHAR);  // Convert size in byte to size in char
			wszCompStr[lRet] = 0;  // Force terminate
			//
			// Remove the whole of the string
			//
			TruncateCompString( false, (int)wcslen( wszCompStr ) );

			//s_CompString.SetText( wszCompStr );
			StringCchCopyW(m_wszComp, MAX_STRING, wszCompStr);

			// Older CHT IME uses composition string for reading string
			//if ( GetLanguage() == LANG_CHT && !GetImeId() )
			//{
			//if( lstrlenW( m_wszComp ) ) {
			//	m_CandList.dwCount = 4;             // Maximum possible length for reading string is 4
			//	m_CandList.dwSelection = (DWORD)-1; // don't select any candidate

			//	// Copy the reading string to the candidate list
			//	for( int i = 3; i >= 0; --i )
			//	{
			//		if( i > lstrlen( s_CompString.GetBuffer() ) - 1 )
			//			m_CandList.awszCandidate[i][0] = 0;  // Doesn't exist
			//		else
			//		{
			//			m_CandList.awszCandidate[i][0] = s_CompString[i];
			//			m_CandList.awszCandidate[i][1] = 0;
			//		}
			//	}
			//	m_CandList.dwPageSize = MAX_CANDLIST;
			//	// Clear comp string after we are done copying
			//	ZeroMemory( (LPVOID)s_CompString.GetBuffer(), 4 * sizeof(WCHAR) );
			//	s_bShowReadingWindow = true;
			//	GetReadingWindowOrientation( 0 );
			//	if( s_bHorizontalReading )
			//	{
			//		m_CandList.nReadingError = -1;  // Clear error
			//		// Create a string that consists of the current
			//		// reading string.  Since horizontal reading window
			//		// is used, we take advantage of this by rendering
			//		// one string instead of several.
			//		//
			//		// Copy the reading string from the candidate list
			//		// to the reading string buffer.
			//		s_wszReadingString[0] = 0;
			//		for( UINT i = 0; i < m_CandList.dwCount; ++i )
			//		{
			//			if( m_CandList.dwSelection == i )
			//				m_CandList.nReadingError = lstrlen( s_wszReadingString );
			//			StringCchCat( s_wszReadingString, 32, m_CandList.awszCandidate[i] );
			//		}
			//	}
			//}
			//else
			//{
			//	m_CandList.dwCount = 0;
			//	s_bShowReadingWindow = false;
			//}
			//}

			if( s_bInsertOnType )
			{
				// Send composition string to the edit control
				SendCompString();
				// Restore the caret to the correct location.
				// It's at the end right now, so compute the number
				// of times left arrow should be pressed to
				// send it to the original position.
				int nCount = lstrlenW( m_wszComp + m_nCompCaret );
				// Send left keystrokes
				for( int i = 0; i < nCount; ++i )
					SendMessage( g_hWnd, WM_KEYDOWN, VK_LEFT, 0 );
				SendMessage( g_hWnd, WM_KEYUP, VK_LEFT, 0 );
			} 
			Deselect();

		}		
	}

	// Retrieve comp string attributes
 	if( lParam & GCS_COMPATTR )
	{
		//DXUTOutputDebugString("GCS_COMPATTR\n");
		lRet = ImmGetCompositionStringW( m_hIMC, GCS_COMPATTR, m_abCompStringAttr, sizeof( m_abCompStringAttr ) );
		if( lRet > 0 )
			m_abCompStringAttr[lRet] = 0;  // ??? Is this needed for attributes?

		DXUTOutputDebugStringW(L"  GCS_COMPATTR CompAttr = %s , CompStr = %s\n", m_abCompStringAttr, m_wszComp);
	}


	//WILL
	// Retrieve clause information
	if( lParam & GCS_COMPCLAUSE )
	{		
		//lRet = ImmGetCompositionStringW(m_hIMC, GCS_COMPCLAUSE, m_wstrCompStringClause, sizeof( m_wstrCompStringClause ) );
		//m_wstrCompStringClause[lRet / sizeof(DWORD)] = 0;  // Terminate

		DXUTOutputDebugStringW(L"  GCS_COMPCLAUSE = %s\n", m_wstrCompStringClause);
	}

	ImmReleaseContext( g_hWnd, m_hIMC );

	return;

}

void SPIMEPool::OnIMEEndComposition(WPARAM wParam, LPARAM lParam)
{
	//m_bUpdate = true;
	//m_bComp = FALSE;	

	TruncateCompString();
	ResetCompositionString();

}


bool SPIMEPool::OnIMEKeyUp(WPARAM wParam, LPARAM lParam)
{
	switch(wParam) {
	case VK_RETURN:
	case VK_ESCAPE:
		{
			if(s_bInsertOnType == true) // Korean IME is return always
				return false;

			//return true;

			//m_bComp = true;
			//DXUTOutputDebugString("  m_bComp = %d\n", m_bComp);
			if(!IsComposition() && m_bComp == true) {
				m_bComp = false;
				return true;
			}
			return false;


		}
		break;
	case VK_TAB:
		{
			if(m_bComp == true)
				m_bComp = false;
		}
		break;
	}
	return false;
}

bool SPIMEPool::OnIMEKeyDown(WPARAM wParam, LPARAM lParam)
{	
	m_bUpdate = false;

	//DXUTOutputDebugString("  KeyDown = %x\n", wParam);
	switch( wParam )
	{
	case VK_HOME:
		SetCaretPosW( 0 );
		if( GetKeyState( VK_SHIFT ) >= 0 )
			m_nSelStart = m_nCaret;

		m_bUpdate = true;
		break;

	case VK_END:
		SetCaretPosW( lstrlenW(m_wszString) );
		if( GetKeyState( VK_SHIFT ) >= 0 )
			m_nSelStart = m_nCaret;

		m_bUpdate = true;
		break;

	case VK_INSERT:
		if( GetKeyState( VK_CONTROL ) < 0 )
		{
			OnCopy();
		} else if( GetKeyState( VK_SHIFT ) < 0 ) {				
			OnPaste();
		} else {			
			m_bInsertMode = !m_bInsertMode;
		}
		break;

	case VK_DELETE:		
		if( m_nCaret != m_nSelStart ) {
			DeleteSelectionText();
			
		} else {
			RemoveChar( m_nCaret );
		}

		m_bUpdate = true;
		break;

	case VK_LEFT:
		if( m_nCaret > 0 )
			SetCaretPosW( m_nCaret - 1 );

		// for MultiLine
		if( m_nCaret > 0 && m_wszString[m_nCaret] == '\n' )
			SetCaretPosW( m_nCaret - 1 );

		if( GetKeyState( VK_SHIFT ) >= 0 )
			m_nSelStart = m_nCaret;

		m_bUpdate = true;
		break;

	case VK_RIGHT:
		if( m_nCaret < lstrlenW(m_wszString) )
			SetCaretPosW( m_nCaret + 1 );

		// for MultiLine
		if( m_nCaret < lstrlenW(m_wszString) && m_wszString[m_nCaret] == '\n' )
			SetCaretPosW( m_nCaret + 1 );


		if( GetKeyState( VK_SHIFT ) >= 0 )
			// Shift is not down. Update selection
			// start along with the caret.
			m_nSelStart = m_nCaret;

		m_bUpdate = true;
		break;

	case VK_UP:
	case VK_DOWN:
		// Trap up and down arrows so that the dialog
		// does not switch focus to another control.
		m_bUpdate = true;
		break;

		//default:
		//	m_bUpdate = wParam != VK_ESCAPE;  // Let the application handle Esc.
	case VK_RETURN:
		{
			if(m_bComp == true)
				m_bComp = false;
			//m_bComp = !m_bComp;
		}
		break;

	case VK_ESCAPE:
		{
			//if(m_bComp == true)
			//	m_bComp = false;
			return false;
		}
		break;
	case VK_TAB:
		{
			if(m_bComp == true)
				m_bComp = false;
		}
		break;

	}
	return true;

	//////////////////////////////////////////////////////////////////////////

}

bool SPIMEPool::RemoveChar( int nIndex )
{
	if( !lstrlenW( m_wszString ) || nIndex < 0 || nIndex >= lstrlenW( m_wszString ) )
		return false;  // Invalid index

	MoveMemory( m_wszString + nIndex, m_wszString + nIndex + 1, sizeof(WCHAR) * ( lstrlenW( m_wszString ) - nIndex ) );	
	return true;
}

bool SPIMEPool::InsertChar( int nIndex, WCHAR wChar )
{
	assert( nIndex >= 0 );

	if( nIndex < 0 || nIndex > lstrlenW( m_wszString ) )
		return false;  // invalid index

	if( m_bHoldAddChar == true)
		return false;	

	int iAnsiLen =SPAbuseFilter::GetInstance()->GetWideLenToAnsiLen(lstrlenW(m_wszString), m_wszString);
	if( iAnsiLen >= m_nLength)
		return false;

	// Check for maximum length allowed
	if( GetTextSize() + 1 >= MAX_STRING )
		return false;

	//if( lstrlenW( m_wszString ) + 1 >= m_nBufferSize )
	if( lstrlenW( m_wszString ) + 1 >= MAX_STRING )
	{
		//if( !Grow() )
		return false;  // out of memory
	}


	if(IsChina()) {
		//// current caret pos is not 0
		//if(	nIndex != 0 &&  
		//	// if previous caret pos and current wChar are guess double byte character then..
		//	m_wszString[ nIndex - 1] < 0xFF && m_wszString[ nIndex - 1] & 0x80 && 
		//	wChar < 0xFF && wChar & 0x80 ) {

		//		m_wszString[ nIndex - 1] = (m_wszString[ nIndex - 1] << 8) | wChar;
		//		return true;

		//}
		if(wChar < 0xFF && wChar & 0x80) {
			if(m_szDivide[0] == 0) {
				m_szDivide[0] = wChar;
				return false;
			} else {
				WCHAR wszTemp[5];
				m_szDivide[1] = wChar;
				SPAbuseFilter::GetInstance()->ConvertAnsiStringToWide(wszTemp, m_szDivide );
				MoveMemory( m_wszString + nIndex + 1, m_wszString + nIndex, sizeof(WCHAR) * ( lstrlenW( m_wszString ) - nIndex + 1 ) );
				m_wszString[ nIndex ] = wszTemp[0];

				ZeroMemory(&m_szDivide, sizeof(m_szDivide));
				return true;
			}
		}
		MoveMemory( m_wszString + nIndex + 1, m_wszString + nIndex, sizeof(WCHAR) * ( lstrlenW( m_wszString ) - nIndex + 1 ) );
		m_wszString[ nIndex ] = wChar;

	} else {
		MoveMemory( m_wszString + nIndex + 1, m_wszString + nIndex, sizeof(WCHAR) * ( lstrlenW( m_wszString ) - nIndex + 1 ) );
		m_wszString[ nIndex ] = wChar;
	}
	
	//m_bAnalyseRequired = true;

	return true;
}




int	SPIMEPool::GetTextSize()
{
	return lstrlenW( m_wszString );
}

void SPIMEPool::OnIMEChar(WPARAM wParam, LPARAM lParam)
{
	switch( (WCHAR)wParam )
	{
		// Backspace
	case VK_BACK:
		{
			// If there's a selection, treat this
			// like a delete key.
			if( m_nCaret != m_nSelStart )
			{
				DeleteSelectionText();			
			}
			else
				if( m_nCaret > 0 )
				{
					// Move the caret, then delete the WCHAR.
					SetCaretPosW( m_nCaret - 1 );
					m_nSelStart = m_nCaret;
					RemoveChar( m_nCaret );					
				}
				break;
		}

	case 24:        // Ctrl-X Cut
		OnCut();
		break;
	case VK_CANCEL: // Ctrl-C Copy
		OnCopy();
		break;
		// Ctrl-V Paste
	case 22:
		OnPaste();			
		break;		
		// Ctrl-A Select All
	case 1:
		if( m_nSelStart == m_nCaret )
		{
			m_nSelStart = 0;
			SetCaretPosW( GetTextSize() );
		}
		break;

		//case VK_RETURN:
		//	// Invoke the callback when the user presses Enter.		
		//	break;

		// Junk characters we don't want in the string
	case 26:  // Ctrl Z
	case 2:   // Ctrl B
	case 14:  // Ctrl N
	case 19:  // Ctrl S
	case 4:   // Ctrl D
	case 6:   // Ctrl F
	case 7:   // Ctrl G
	case 10:  // Ctrl J
	case 11:  // Ctrl K
	case 12:  // Ctrl L
	case 17:  // Ctrl Q
	case 23:  // Ctrl W
	case 5:   // Ctrl E
	case 18:  // Ctrl R
	case 20:  // Ctrl T
	case 25:  // Ctrl Y
	case 21:  // Ctrl U
	case 9:   // Ctrl I
	case 15:  // Ctrl O
	case 16:  // Ctrl P
	case 27:  // Ctrl [
	case 29:  // Ctrl ]
	case 28:  // Ctrl \ 
		break;

	case 13:  // ENTER Key
		{
			if(m_bMultiLine ==  true) {
				UpdateString(L'\n');
			}
		}
		break;

	default:
		{
			DXUTOutputDebugStringW( L" WM_CHAR wParam = %c wParam = %x lParam = %d\n", (WCHAR)wParam, wParam, lParam);

			UpdateString((WCHAR)wParam);
		}
	}
	return;
}



void SPIMEPool::UpdateString( WCHAR wcUpdate)
{
	if(m_bNumber) {
		if( !(wcUpdate >= '0' && wcUpdate <= '9'))
			return;				
	}

	const WCHAR* pwszString = GetStringW();	

	//int iAnsiLen =SPAbuseFilter::GetInstance()->GetWideLenToAnsiLen(lstrlenW(pwszString), pwszString);
	//if( iAnsiLen > m_nLength)
	//	return;

	//if( m_nLength < lstrlenW( GetStringW()))
	//	return;

	if(m_bHoldAddChar == true)
		return;

	if( m_nCaret != m_nSelStart )
		DeleteSelectionText();


	if( !m_bInsertMode && m_nCaret < lstrlenW(m_wszString) ) {
		m_wszString[m_nCaret] = wcUpdate;
		SetCaretPosW( m_nCaret + 1 );
		m_nSelStart = m_nCaret;
	} else {
		// Insert the WCHAR
		if( InsertChar( m_nCaret, wcUpdate ) )
		{
			SetCaretPosW( m_nCaret + 1 );
			m_nSelStart = m_nCaret;
		}
	}

}




void SPIMEPool::SetClipString(const char* IN pszStr)
{	
	int iLen;
	ZeroMemory(m_wszClip, MAX_STRING);

	if(pszStr == NULL)
		return;	

	WCHAR wstrIn[MAX_STRING];
	SPAbuseFilter::GetInstance()->ConvertAnsiStringToWide( wstrIn, pszStr);

	iLen = lstrlenW(wstrIn);
	if(iLen == 0)
		return;

	++iLen;
	if( iLen > MAX_STRING ) {
		iLen = MAX_STRING;		
	}

	lstrcpynW(m_wszClip, wstrIn, iLen);	

	if(OpenClipboard(NULL)!=0)
	{
		EmptyClipboard();
		int iSize = iLen*2;
		WCHAR *wszCopy = new WCHAR[iLen];
		wcsncpy( wszCopy, m_wszClip, iLen );
		
		HGLOBAL hglbCpy=GlobalAlloc(GMEM_MOVEABLE,iSize);
		if(hglbCpy!=NULL) 
		{ 
			HANDLE hClipData=NULL;
			char* pc=(char*)GlobalLock(hglbCpy);
			if(pc!=NULL)
			{
				WideCharToMultiByte(CP_ACP, 0, wszCopy,
					iLen + 1,pc,iSize,NULL,NULL);
				GlobalUnlock(hglbCpy); 
				hClipData=SetClipboardData( CF_TEXT, hglbCpy );
			}
			if(hClipData==NULL)
				GlobalFree( hglbCpy );
		}

		delete [] wszCopy;
		CloseClipboard(); 
	}
}

void SPIMEPool::SetClipStringW(const WCHAR* IN pwszStr)
{	
	int iLen;
	ZeroMemory(m_wszClip, MAX_STRING);

	if(pwszStr == NULL)
		return;		

	iLen = lstrlenW(pwszStr);
	if(iLen == 0)
		return;

	if( iLen > MAX_STRING ) {
		iLen = MAX_STRING;
		return;
	}

	lstrcpynW(m_wszClip, pwszStr, iLen);		
}


void SPIMEPool::OnCopy()
{
	if (m_bPassword)
		return;

	int iBegin, iEnd;
	ZeroMemory(m_wszClip, MAX_STRING);

	if(lstrlenW(m_wszString) == 0)
		return;

	if( m_nSelStart == m_nCaret )
		return;

	if(m_nSelStart < m_nCaret) {
		iBegin = m_nSelStart;
		iEnd = m_nCaret;
	} else {
		iBegin = m_nCaret;
		iEnd = m_nSelStart;
	}

	if( iEnd > lstrlenW(m_wszString)) {
		assert(0 && "Invalid selection");
		return;
	}

	//--------------------------------------------------
	if(OpenClipboard(NULL)!=0)
	{
		EmptyClipboard();
	
		int len = iEnd - iBegin;
		int iSize=len*2+2;
		WCHAR *wszCopy = new WCHAR[len + 1];
		wcsncpy( wszCopy, m_wszString+iBegin, len );
		wszCopy[ len ] = L'\0';

		HGLOBAL hglbCpy=GlobalAlloc(GMEM_MOVEABLE,iSize);
		if(hglbCpy!=NULL) 
		{ 
			HANDLE hClipData=NULL;
			char* pc=(char*)GlobalLock(hglbCpy);
			if(pc!=NULL)
			{
				WideCharToMultiByte(CP_ACP,0,wszCopy,
					len+1,pc,iSize,NULL,NULL);
				GlobalUnlock(hglbCpy); 
				hClipData=SetClipboardData(CF_TEXT,hglbCpy);
			}
			if(hClipData==NULL)
				GlobalFree(hglbCpy);
		}

		delete [] wszCopy;
		CloseClipboard(); 
	}
	//--------------------------------------------------
}


void SPIMEPool::OnCut()
{
	if (m_bPassword)
		return;

	int iBegin, iEnd;
	int iLen;	

	ZeroMemory(m_wszClip, MAX_STRING);
	iLen = lstrlenW(m_wszString);
	if(iLen == 0)
		return;

	if( m_nSelStart == m_nCaret )
		return;	

	if(m_nSelStart < m_nCaret) {
		iBegin = m_nSelStart;
		iEnd = m_nCaret;
	} else {
		iBegin = m_nCaret;
		iEnd = m_nSelStart;
	}
	if( iEnd > lstrlenW(m_wszString)) {
		assert(0 && "Invalid selection");
		return;
	}

	//--------------------------------------------------
	if(OpenClipboard(NULL)!=0)
	{
		EmptyClipboard();

		int len = iEnd - iBegin;
		int iSize=len*2+2;
		WCHAR *wszCopy = new WCHAR[len + 1];
		wcsncpy( wszCopy, m_wszString+iBegin, len );
		wszCopy[ len ] = L'\0';

		HGLOBAL hglbCpy=GlobalAlloc(GMEM_MOVEABLE,iSize);
		if(hglbCpy!=NULL) 
		{ 
			HANDLE hClipData=NULL;
			char* pc=(char*)GlobalLock(hglbCpy);
			if(pc!=NULL)
			{
				WideCharToMultiByte(CP_ACP,0,wszCopy,
					len+1,pc,iSize,NULL,NULL);
				GlobalUnlock(hglbCpy); 
				hClipData=SetClipboardData(CF_TEXT,hglbCpy);
			}
			if(hClipData==NULL)
				GlobalFree(hglbCpy);
		}

		delete [] wszCopy;
		CloseClipboard(); 
	}
	//--------------------------------------------------
	DeleteSelectionText();
	m_bUpdate = true;
}


void SPIMEPool::OnPaste()
{
	if (m_bNumber)
		return;

	//--------------------------------------------------
	// added by jmulro, 2008-07-09
	if (m_nSelStart != m_nCaret)
	{
		DeleteSelectionText();
	}

	int iBufferLen = wcslen(m_wszString);
	if (OpenClipboard(NULL) != 0)
	{
		HANDLE hClipData = GetClipboardData( CF_UNICODETEXT );
		if (NULL != hClipData)
		{
			WCHAR *pwcClipData = (WCHAR*)GlobalLock( hClipData );
			if (NULL != pwcClipData)
			{
				unsigned int nDataLen = wcslen( pwcClipData );
				if (iBufferLen + nDataLen > m_nLength)
				{
					nDataLen = m_nLength - iBufferLen;
				}

				WCHAR *pwc = new WCHAR[nDataLen+1];
				wcsncpy( pwc, pwcClipData, nDataLen );
				pwc[nDataLen] = L'\0';

				WCHAR *pend = wcschr( pwc, L'\r' );
				if (0 != pend)
				{
					*pend = L'\0';
				}

				pend = wcschr( pwc, L'\n' );
				if (0 != pend)
				{
					*pend = L'\0';
				}

				nDataLen = wcslen( pwc );
				WCHAR oldwszString[MAX_STRING + 1] = {L'\0',}; 
				CopyMemory( oldwszString, m_wszString, sizeof(WCHAR)*iBufferLen ); 
				CopyMemory( m_wszString + m_nCaret + nDataLen,
					m_wszString + m_nCaret, 
					sizeof( WCHAR ) * (iBufferLen - m_nCaret));

				CopyMemory( m_wszString + m_nCaret, pwc, sizeof(WCHAR) * nDataLen );
				int iAnsiLen =SPAbuseFilter::GetInstance()->GetWideLenToAnsiLen(lstrlenW(m_wszString), m_wszString);
				if( iAnsiLen > m_nLength)
				{
					CopyMemory( m_wszString, oldwszString, sizeof(WCHAR)*iBufferLen );
					m_wszString[iBufferLen] = L'\0';
				}
				else
				{
					m_nCaret += nDataLen;
					m_nStringLen = lstrlenW( m_wszString );
					m_nSelStart = m_nCaret;
				}

				delete [] pwc;
				GlobalUnlock( hClipData );
			}
		}
		else if (NULL != (hClipData = GetClipboardData(CF_TEXT)))
		{
			char *pcClipData = (char *)GlobalLock( hClipData );
			if (NULL != pcClipData)
			{
				char *pc = new char[ strlen(pcClipData) + 1 ];
				strcpy( pc, pcClipData );

				char *pend = strchr( pc, '\r' );
				if (0 != pend)
				{
					*pend = '\0';
				}

				pend = strchr( pc, '\n' );
				if (0 != pend)
				{
					*pend = '\0';
				}

				int iSize = strlen( pc ) + 1;
				WCHAR *pwc = new WCHAR[ iSize ];
				MultiByteToWideChar( CP_ACP, 0, pc, -1, pwc, iSize );

				unsigned int nDataLen = wcslen( pwc );
				if (iBufferLen + nDataLen > m_nLength)
				{
					nDataLen = m_nLength - iBufferLen;
					pwc[ nDataLen ] = L'\0';
				}
				
				WCHAR oldwszString[MAX_STRING + 1] = {L'\0',}; 
				CopyMemory( oldwszString, m_wszString, sizeof(WCHAR)*iBufferLen ); 
				CopyMemory( m_wszString + m_nCaret, pwc, sizeof(WCHAR) * nDataLen );
				int iAnsiLen =SPAbuseFilter::GetInstance()->GetWideLenToAnsiLen(lstrlenW(m_wszString), m_wszString);
				if( iAnsiLen > m_nLength)
				{
					CopyMemory( m_wszString, oldwszString, sizeof(WCHAR)*iBufferLen );
					m_wszString[iBufferLen] = L'\0';
				}
				else
				{
					m_nCaret += nDataLen;
					m_nStringLen = lstrlenW( m_wszString );
					m_nSelStart = m_nCaret;
				}

				delete [] pc;
				delete [] pwc;
				GlobalUnlock( hClipData );
			}
		}

		m_bUpdate = true;
	}
	CloseClipboard();
	//--------------------------------------------------
}

//appended for chinese//////////////////////////////////////////////////////////////////////////

void SPIMEPool::SetStatusWindowPos(int iX, int iY)
{
	m_ptStatusWindowPos.x = iX;
	m_ptStatusWindowPos.y = iY;
}
void SPIMEPool::SetCandidateWindowPos(int iX, int iY)
{
	m_ptCandidateWindowPos.x = iX;
	m_ptCandidateWindowPos.y = iY;
}




