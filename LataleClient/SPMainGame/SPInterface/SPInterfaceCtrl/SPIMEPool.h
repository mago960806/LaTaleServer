// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2005-01-07 ??E10:26:19
//***************************************************************************

#pragma once

#define MAX_STRING	255
#define MAX_CANDLIST 10



class SPIMEPool
{
public :
	SPIMEPool();
	~SPIMEPool();

	// update
	bool IsUpdate() { return m_bUpdate; }
	void Refreshed();

	bool IsInsertOnType() { return s_bInsertOnType; }
	bool IsVerticalCand() { return m_bVerticalCand; }
	bool IsCandUpdate() { return m_bCandUpdate; }
	void CandRefreshed();

	void EnableIME();
	void DisableIME();
	bool IsNative() { return (s_ImeState == IMEUI_STATE_ON); }

	bool IsWideCaret(); // For Japanese Composition Caret
	int  GetCompLenW();
	int  GetCompLen();
	const WCHAR* GetCompStringW();
	const char* GetCompString();
	const BYTE* GetCompositionAttr() { return m_abCompStringAttr;	}

	// selection
	void SetSel( int nStart = -1, int nEnd = -1);
	void SetSelW( int nStart = -1, int nEnd = -1);
	void GetSel( int& nStart, int& nEnd );
	void GetSelW( int& nStart, int& nEnd );

	void SetSelectPos(int iPos);
	void Deselect();

	// caret pos
	int GetCaretPos(); 
	int GetCaretPosW(); 

	void SetCaretPos(int iPos);
	void SetCaretPosW(int iPos);

	void SetMultiLine(bool bMultiLine = true);
	void SetHoldAddChar(bool bHold);

	// forcus
	void SetFocus( bool bFocus );
	bool IsFocus() { return m_bFocus; }
	void KillFocus();
	bool IsComposition();	
	void PreKillFocus();

	// number
	void SetNumber( bool bNumber ) { m_bNumber = bNumber; }
	/// password
	void SetPassword( bool bPassword ) { m_bPassword = bPassword; }

	// string
	void  SetLimitText( int nLength );
	int   GetStringLenW();
	int   GetStringLen();

	void  SetString( const char* szStr );
	void  SetStringW( const WCHAR* wszStr );

	const char* GetString();
	const WCHAR* GetStringW();

	void  ClearBuffer();
	void Purge();
	int	GetTextSize();


	void SetEnglishOnly(bool bVal);
	bool IsEnglishMode();
	void EnableHagulMode();
	void EnableEnglishMode();

	// Utility for SPWindowEdit Family
	//TCodeType  IsHangul(const char* OutText, int Len);
	//void ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource);
	//void ConvertWideToAnsiString( CHAR* strDestination, const WCHAR* wstrSource);
	//int GetAnsiLenToWideLen(int iPos, const char* szAnsi);
	//int GetAnsiLenToWideLen(int iPos, const WCHAR* wszWide);
	//int GetWideLenToAnsiLen(int iPosW, const WCHAR* wszWide);


	// Use for only chinese
	//DWORD GetImeId( UINT uIndex = 0);
	void SetStatusWindowPos(int iX, int iY);
	void SetCandidateWindowPos(int iX, int iY);

	struct SPCandList {
		WCHAR awszCandidate	[MAX_CANDLIST][256];
		wstring wstrHoriCand;		// Candidate list string (for horizontal candidate window)
		int   nFirstSelected;	// First character position of the selected string in HoriCand
		int   nHoriSelectedLen; // Length of the selected string in HoriCand
		DWORD dwCount;			// Number of valid entries in the candidate list
		DWORD dwSelection;		// Currently selected candidate entry relative to page top
		DWORD dwPageSize;
		int   nReadingError;	// Index of the error character
		bool  bShowWindow;		// Whether the candidate list window is visible
		RECT  rcCandidate;		// Candidate rectangle computed and filled each time before rendered
	};

	const SPCandList* GetCandList() { return &m_CandList; }

	void SetClipString(const char* IN pszStr);
	void SetClipStringW(const WCHAR* IN pwszStr);

public :
	bool WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam );	

private :
	// ime enter/leave
	HIMC Enter();
	void Leave();


	// for IME Message 	
	void OnIMEStartComposition( WPARAM wParam, LPARAM lParam );
	void OnIMEComposition( WPARAM wParam, LPARAM lParam );
	void OnIMEEndComposition( WPARAM wParam, LPARAM lParam );
	void OnIMEChar( WPARAM wParam, LPARAM lParam );
	bool OnIMEKeyDown( WPARAM wParam, LPARAM lParam );
	bool OnIMEKeyUp( WPARAM wParam, LPARAM lParam );

	void OnCopy();
	void OnPaste();
	void OnCut();

	bool OnIMENotify(WPARAM wParam, LPARAM lParam);

	void CheckInputLocale();

	static WORD GetLanguage() { return LOWORD( s_hklCurrent ); }
	static WORD GetPrimaryLanguage() { return PRIMARYLANGID( LOWORD( s_hklCurrent ) ); }
	static WORD GetSubLanguage() { return SUBLANGID( LOWORD( s_hklCurrent ) ); }

	void DeleteSelectionText();
	void TruncateCompString( bool bUseBackSpace = true, int iNewStrLen = 0 );
	void CheckToggleState();	

	bool RemoveChar( int nIndex );
	bool InsertChar( int nIndex, WCHAR wChar );
	void SendCompString();
	void ResetCompositionString();

	void UpdateString( WCHAR wcUpdate);


	bool IsChina();

private :

	enum { INDICATOR_NON_IME, INDICATOR_CHS, INDICATOR_CHT, INDICATOR_KOREAN, INDICATOR_JAPANESE };
	enum IMESTATE { IMEUI_STATE_OFF, IMEUI_STATE_ON, IMEUI_STATE_ENGLISH };

	// Function pointers: Traditional Chinese IME
	static UINT (WINAPI * _GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
	static BOOL (WINAPI * _ShowReadingWindow)( HIMC, BOOL );

	SPCandList m_CandList;

	static HKL		s_hklCurrent;
	static WCHAR    s_aszIndicator[5][3];
	static LPWSTR   s_wszCurrIndicator;   
	static bool		s_bChineseIME;
	static IMESTATE	s_ImeState;				// IME global state
	static bool		s_bInsertOnType;		// Insert the character as soon as a key is pressed (Korean behavior)
	bool			m_bVerticalCand;
	bool			m_bInsertMode;			// If true, control is in insert mode. Else, overwrite mode.
	WCHAR			m_wstrCompStringClause[MAX_STRING];


	bool			m_bCandUpdate;

	bool			m_bFocus;
	bool			m_bUpdate;


	int     m_nSelStart;		// Select
	int		m_nCaret;			// Ù»?EÍÝÙ¹
	int		m_nCompCaret;		// Caret position of the composition string


	BOOL	m_bComp;					// ÊµÞ¢ ??×ð?E?	
	WCHAR	m_wszComp[MAX_STRING + 1];	//?EñÕ ???? ×ð?EÞ¢?E4->256 
	BYTE    m_abCompStringAttr[MAX_STRING];
	int     m_nLength;					// string ùÞ?E
	int		m_nStringLen;
	WCHAR	m_wszString[MAX_STRING + 1];	// ÊÇ?E??Êµ Þ¢?E(?EñÕ ??? Þ¢?E	
	WCHAR	m_wszClip[MAX_STRING + 1];	// internal clipboard	

	char	m_szDivide[3];



	// mode
	bool	m_bIsEnglishMode;		// ??E???ûã?ÜË ??E
	bool	m_bMultiLine;
	bool	m_bHoldAddChar;

	// mode number
	bool	m_bNumber;				// disable paste with number input
	bool	m_bPassword;			// disable copy&cut with password input.
	int		m_iNoSkipChar;
	int		m_iNoExCarrotPos;

	// etc
	bool	m_bUnknown;
	HIMC	m_hIMC;
	static HIMC	s_hIMC;

	// Application-wide data
	POINT	m_ptStatusWindowPos;
	POINT	m_ptCandidateWindowPos;

	
	bool	m_bEngishOnly;


};


inline void SPIMEPool::SetEnglishOnly(bool bVal)
{
	m_bEngishOnly = bVal;
}


