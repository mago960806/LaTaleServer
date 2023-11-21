// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPIngameWebbrowser2
// Comment     : 
// Creation    : metalgeni 2007-2-28 ø¿¿ÅE10:26:19
//***************************************************************************

#pragma once



#include <Exdisp.h>

enum SPNavigateState {
	NAVIGATE_NULL = 0,
	NAVIGATE_WAIT,
	NAVIGATE_OK,
	NAVIGATE_OK_READY,
	NAVIGATE_PRELOADED,
};

#define WAITTICKTIME 0.2f

class SPIngameWebbrowser2 {

public:

	SPIngameWebbrowser2();
	~SPIngameWebbrowser2();

	bool CreateNewInstance ();
	bool IsValid ();
	bool IsNavigate();
	bool IsWait();
	bool IsLoadOK();
	bool IsPreloaded();
	//bool ShowPreload();
	bool CheckIEVersion();
	bool IsNotDisplayChange();

	void SetRelease();	
	void Process(float fTime);

	HWND GetHWND();

	const WCHAR* GetFullName() const;
	const WCHAR* GetType() const;
	const WCHAR* GetLocationName() const;
	const WCHAR* GetLocationURL() const;
	bool Navigate(LPCTSTR lpszURL, DWORD dwFlags = 0, LPCTSTR lpszTargetFrameName  = NULL ,
		LPCTSTR lpszHeaders = NULL , LPVOID lpvPostData = NULL ,
		DWORD dwPostDataLen = 0, bool bPreload = false );
	bool WaitTillLoaded (int nTimeout);
	void Refresh ();
	void Stop ();

	void Active(bool bActive);

	void GoBack ();
	void GoForward ();
	void Quit ();
	bool GetBusy() const;

	READYSTATE GetReadyState() const;
	LPDISPATCH GetHtmlDocument() const;

	void SetMainWidow(HWND hWnd); 
	const char* GetLastURL();	

protected:

private:
	IWebBrowser2	*m_pWebBrowser;
	HWND			m_hMainWnd;
	HWND			m_hWnd;

	int				m_iCX;
	int				m_iCY;

	float			m_fElapsed;
	SPNavigateState m_NavigateState;	

	bool			m_bPreload;
	bool			m_bPreloaded;
	bool			m_bNotDisplayChange;

	std::string		m_strLastUrl;
	char			m_szIEVer[32];
	unsigned int    m_iIEVer;

};

extern SPIngameWebbrowser2* g_pIngameWebBrowser;



