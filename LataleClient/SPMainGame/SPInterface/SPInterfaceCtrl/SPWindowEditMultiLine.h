// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPWindowEditMultiLine
// Comment     : 
// Creation    : metalgeni 4/6/2006 ø¿¿ÅE10:26:19
//***************************************************************************

#pragma once

class SPWindow;
class SPWindowEditMultiLine : public SPWindow
{
public :	
	SPWindowEditMultiLine( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	~SPWindowEditMultiLine();

	// default
	virtual void Process( float fTime );
	virtual void Render	( float fTime );
	virtual void Hide(bool bSendServer = true);

	// focus
	void SetFocus();
	void KillFocus();

	void SetEnableEnter(bool bEnable);
	bool IsEnableEnter();

	// text
	virtual void SetWindowText(const char* pStr);
	void SetLimitText( int nLength );
	void SetNULL();		
	void SetWindowFormatText( const WCHAR* wszText, ... ); 
	const char* GetWindowText();

protected :

	bool ProcessString();
	void ProcessCaret();
	void ProcessSelect(int iBeginLen, int iEndLen);
	void ProcessComposition();
	void ProcessCandidate();
	

	void CalcMaxLine();

	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnSelFocus		( WPARAM wParam, LPARAM lParam );
	int OnKillFocus		( WPARAM wParam, LPARAM lParam );
	int OnCursorLDown	( WPARAM wParam, LPARAM lParam );
	int OnSetCaretPos	( WPARAM wParam, LPARAM lParam );

	int OnWheelUp		( WPARAM wParam, LPARAM lParam);		//[2005/12/20]
	int OnWheelDown		( WPARAM wParam, LPARAM lParam);		//[2005/12/20]
	int OnKeyDown		( WPARAM wParam, LPARAM lParam);		//[12/27/2005]
	int OnSetMargin		( WPARAM wParam, LPARAM lParam);
	int OnGetEnableEnter( WPARAM wParam, LPARAM lParam);

protected:

	std::wstring	m_wstrWindowText;
	std::wstring	m_wstrWindowTextBuf;
	CHAR			m_szWindowText[256];

	int				m_iLimitText;
	
	bool			m_bTwinkleCursor;
	bool			m_bSelect;
	float 			m_fPrevTime;

	SIZE			m_sizeCaretLen;
	RECT			m_rcWindowRect;
	RECT			m_rcCaretRect;

	int				m_iMargin;
	int				m_iCurLine;
	int				m_iCurCaretInLine;	
	int				m_iMaxLine;
	bool			m_bEnableEnter;

	std::vector<std::wstring>	m_vwstrWindowText;
	std::vector<bool>			m_vbWithNewLine;
	std::vector<RECT>			m_vrcCaretRect;
	std::vector<RECT>			m_vrcCompositionCaret;	

};







