// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPIngameWebbrowser
// Comment     : refer to "Embed an HTML control in your own window using plain C" by Jeff Glatt 
//					URL - http://www.codeproject.com/com/cwebpage.asp
// Creation    : metalgeni 2007-2-28 10:26:19
//***************************************************************************

#pragma once


#include <OleIdl.h>
#include <Exdisp.h>
#include <mshtml.h>
#include <mshtmhst.h>
#include <oaidl.h>


enum SPNavigateState {
	NAVIGATE_NULL = 0,
	NAVIGATE_WAIT,
	NAVIGATE_OK,
	NAVIGATE_OK_READY,
	NAVIGATE_PRELOADED,
};


#define WAITTICKTIME 0.2f


typedef void (CALLBACK FAR* pfCallbackWebEnable) (bool bEnable);

class SPIStorage;

class SPIngameWebbrowser {

public:

	SPIngameWebbrowser(HWND hwnd, HINSTANCE hInstance, LPCTSTR pszWindowClass);	
	~SPIngameWebbrowser();

	bool CreateNewInstance ();
	bool IsValid ();
	bool IsNavigate();
	bool IsWait();
	bool IsLoadOK();	
	bool IsNotDisplayChange();

	void SetPos(int x, int y);
	void SetSize(int cx, int cy);

	void SetRelease();	
	void Process(float fTime);	

	bool Navigate(LPCTSTR lpszURL, DWORD dwFlags = 0, LPCTSTR lpszTargetFrameName  = NULL ,
		LPCTSTR lpszHeaders = NULL , LPVOID lpvPostData = NULL ,
		DWORD dwPostDataLen = 0, bool bPreload = false );

	void Active(bool bActive);
	void Quit();	
	
	void SetReadyCallBack(pfCallbackWebEnable apfCallbackWebEnable);

protected:
	bool CheckIEVersion();
	long EmbedBrowserObject(HWND hwnd);
	void UnEmbedBrowserObject(HWND hwnd);	

	READYSTATE GetReadyState() const;
	LPDISPATCH GetHtmlDocument() const;



private:
	IWebBrowser2	*m_pWebBrowser;
	HWND			m_hMainWnd;
	HWND			m_hWnd;
	IOleObject*		m_pBrowserObject;
	

	int				m_iCX;
	int				m_iCY;

	float			m_fElapsed;
	SPNavigateState m_NavigateState;
	bool			m_bNotDisplayChange;

	
	char			m_szIEVer[32];
	unsigned int    m_iIEVer;
	HINSTANCE		m_hInstance;

	pfCallbackWebEnable		m_pfCallbackWebEnable;

	SPIStorage			m_IStorage;
	SPIOleInPlaceFrame	m_IOleInPlaceFrame;
	SPIOleInPlaceSite	m_IOleInPlaceSite;
	SPIOleClientSite	m_IOleClientSite;

	POINT				m_ptPosition;
	SIZE				m_szeSize;

};

extern SPIngameWebbrowser* g_pIngameWebBrowser;



