
#include <string>

#include <assert.h>
#include <comutil.h>

#include <D3D9.h>
#include <d3dx9.h>

#include <MsHTML.h>

#include <OleIdl.h>

#include "SPRefOLE.h"

#include "DXUtil.h"
#include "CD3DEnumeration.h"
#include "D3DSettings.h"

#include "SPVideoCommon.h"
#include "SPTextureMgr.h"
#include "SPLine.h"
#include "SPFont.h"
#include "SPVideo.h"


#include "dxutil.h"

#include "SPIngameWebbrowser.h"

#define NOTIMPLEMENTED assert(0); return(E_NOTIMPL)

using namespace std;

// 프로젝트 속성 옵션에 따라
#if defined _NATIVE_WCHAR_T_DEFINED   
	#pragma comment(lib, "comsuppw.lib")	
#else
	#pragma comment(lib, "comsupp.lib")		
#endif




SPIngameWebbrowser* g_pIngameWebBrowser = NULL;

void ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource)
{
	int cchDestChar; 

	if( wstrDestination==NULL || strSource==NULL )
		return;

	cchDestChar = strlen(strSource)+1;

	MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
		wstrDestination, cchDestChar-1 );

	wstrDestination[cchDestChar-1] = 0;
}


SPIngameWebbrowser::SPIngameWebbrowser(HWND hwnd, HINSTANCE hInstance, LPCTSTR pszWindowClass)
: m_pWebBrowser(NULL)
, m_hMainWnd(hwnd)
, m_NavigateState(NAVIGATE_NULL)
, m_fElapsed(0.0f)
, m_iIEVer(0)
, m_bNotDisplayChange(false)
, m_hInstance(hInstance)
, m_pBrowserObject(NULL)
, m_pfCallbackWebEnable(NULL)
{
	OleInitialize(NULL);
	CheckIEVersion();

	m_ptPosition.x = 0;
	m_ptPosition.y = 0;

	RECT rcMainWindow;
	GetWindowRect(m_hMainWnd, &rcMainWindow);

	m_szeSize.cx = rcMainWindow.right - rcMainWindow.left;
	m_szeSize.cy = rcMainWindow.bottom - rcMainWindow.top;
	
	m_hWnd = CreateWindowEx(0, pszWindowClass, "", WS_CHILD | WS_VISIBLE,	
		m_ptPosition.x, m_szeSize.cx, m_ptPosition.y, m_szeSize.cy,	m_hMainWnd, 
		NULL, m_hInstance, 0);
	
	m_IOleInPlaceFrame.Init(m_hWnd);
	m_IOleInPlaceSite.Init( &m_IOleInPlaceFrame, m_hWnd);		
	m_IOleClientSite.Init( &m_IOleInPlaceSite, NULL, NULL);
	m_IOleInPlaceSite.SetClientSite(&m_IOleClientSite);

	m_bNotDisplayChange = false;
}

SPIngameWebbrowser::~SPIngameWebbrowser()
{
	m_IStorage.Release();
	m_IOleInPlaceFrame.Release();
	m_IOleInPlaceSite.Release();
	m_IOleClientSite.Release();

	UnEmbedBrowserObject(m_hWnd);
	OleUninitialize();	

	DestroyWindow(m_hWnd);
}

bool SPIngameWebbrowser::CheckIEVersion()
{
	HKEY  hkey;
	unsigned long dispos;
	unsigned long type;
	BYTE  pData[64];
	unsigned long dwDataSize;

	char subkey[256];
	strcpy( subkey, "SOFTWARE\\Microsoft\\Internet Explorer\\Version Vector" );	

	if ( RegCreateKeyEx( HKEY_LOCAL_MACHINE, subkey, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dispos ) != ERROR_SUCCESS ) return false;
	if ( RegQueryValueEx( hkey, "IE", NULL, &type, (BYTE*)pData, &dwDataSize ) != ERROR_SUCCESS ) {
		strcpy(m_szIEVer, "0.0000");
		m_iIEVer = 0;
	} else {
		strcpy(m_szIEVer, (char*)pData);	
		m_iIEVer = atoi((char*)pData);
	}
	RegCloseKey( hkey );

	return true;
}


bool SPIngameWebbrowser::CreateNewInstance ()
{
	SAFE_RELEASE(m_pWebBrowser);

	HRESULT hr;
	IWebBrowser2* pWebBrowser = NULL;

	m_bNotDisplayChange = true;

	EmbedBrowserObject(m_hWnd);

	if (m_hWnd && (m_pWebBrowser != NULL)) {
		//TODO : additional browser option
		return true;
	} else {
		assert(0);
		m_bNotDisplayChange = false;		
		SAFE_RELEASE(m_pWebBrowser);		
		OutputDebugString( "	-IGW navigate failed\n");		
	}

	return false;
}



bool SPIngameWebbrowser::IsValid ()
{
	if (m_pWebBrowser == NULL)
		return false;

	return true;
}


bool SPIngameWebbrowser::IsNavigate()
{
	if(m_pWebBrowser == NULL)
		return false;

	if(m_NavigateState == NAVIGATE_NULL ) {
		return  false;
	}

	if(m_NavigateState == NAVIGATE_OK_READY) {
		return true;
	}

	return true;;
}

void SPIngameWebbrowser::SetRelease()
{
	SAFE_RELEASE(m_pWebBrowser);	
	Active(false);
}


void SPIngameWebbrowser::Active(bool bActive)
{
	if( m_NavigateState == NAVIGATE_NULL)
		return;

	//DEVMODE dm;
	//if(bActive) {
	//	if(g_pVideo->IsWindowed() == false) {			
	//		g_pVideo->GetIngameDM(&dm);
	//		dm.dmPelsWidth = g_pVideo->GetScreenLenX();
	//		dm.dmPelsHeight = g_pVideo->GetScreenLenY();
	//		int result = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
	//	}
	//} else {
	//	if(g_pVideo->IsWindowed() == false) {			
	//		g_pVideo->GetIngameDM(&dm);
	//		int result = ChangeDisplaySettings(&dm, 0);
	//	}
	//}
}

void SPIngameWebbrowser::SetPos(int x, int y)
{
	m_ptPosition.x = x;
	m_ptPosition.y = y;
}

void SPIngameWebbrowser::SetSize(int cx, int cy)
{
	m_szeSize.cx = cx;
	m_szeSize.cy = cy;
}

bool SPIngameWebbrowser::IsWait()
{
	if(m_NavigateState == NAVIGATE_WAIT)
		return true;

	return false;
}

bool SPIngameWebbrowser::IsLoadOK()
{
	return (m_NavigateState == NAVIGATE_OK_READY);
}

//bool SPIngameWebbrowser::IsPreloaded()
//{
//	return m_bPreloaded;
//}

bool SPIngameWebbrowser::IsNotDisplayChange()
{
	return m_bNotDisplayChange;
}

bool SPIngameWebbrowser::Navigate(LPCTSTR lpszURL, DWORD dwFlags /* = 0 */,
								   LPCTSTR lpszTargetFrameName /* = NULL */ ,
								   LPCTSTR lpszHeaders /* = NULL */, LPVOID lpvPostData /* = NULL */,
								   DWORD dwPostDataLen /* = 0 */,
								   bool bPreload)
{

	//BSTR bstrURL = _com_util::ConvertStringToBSTR("http://ruliweb.empas.com"); // 
	BSTR bstrURL = _com_util::ConvertStringToBSTR(lpszURL); // 

	VARIANT varURL;
	VARIANT vars[4];
	//memset(vars,0,sizeof(vars));	
	VariantInit(&vars[0]);
	VariantInit(&vars[1]);
	VariantInit(&vars[2]);
	VariantInit(&vars[3]);

	HRESULT hrie = m_pWebBrowser->Navigate(bstrURL, vars,vars+1,vars+2,vars+3);

	SysFreeString(bstrURL);

	if (SUCCEEDED(hrie)) {

		if(m_pfCallbackWebEnable)
			m_pfCallbackWebEnable(true);	

		m_NavigateState = NAVIGATE_WAIT;
		m_bNotDisplayChange = true;

		SetWindowPos(m_hWnd, HWND_TOP, m_ptPosition.x, m_ptPosition.y, m_szeSize.cx, m_szeSize.cy, SWP_SHOWWINDOW);		
		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);

		return true;
	}

	return false;
}


void SPIngameWebbrowser::Process(float fTime)
{
	HRESULT hr;

	if(m_pWebBrowser == NULL) {
		return;
	}

	m_fElapsed += fTime;
	if(m_fElapsed < WAITTICKTIME) {
		return;
	}
	m_fElapsed = 0.0f;

	READYSTATE result;
	VARIANT_BOOL bBusy = VARIANT_TRUE;	
	if(m_pWebBrowser) {		
		m_pWebBrowser->get_ReadyState (&result);
	}	

	// finished page load  and click another web link.
	if(result == READYSTATE_COMPLETE && m_NavigateState == NAVIGATE_OK_READY) {
		BSTR BStrUrl;
		char* pszUrl;
		m_pWebBrowser->get_LocationURL(&BStrUrl);
		pszUrl = _com_util::ConvertBSTRToString(BStrUrl);
		SysFreeString(BStrUrl);

		

		if(strcmp(pszUrl , "") == 0) {
			UnEmbedBrowserObject(m_hWnd);

			ShowWindow(m_hWnd, SW_HIDE);
			UpdateWindow(m_hWnd);

			OutputDebugString( "	-IGW quit\n");
			SendMessage(m_hMainWnd, WM_ACTIVATE, TRUE, 0);
		}	
		delete [] pszUrl;
		return;
	}

	// finish load top billing page.
	if(result == READYSTATE_COMPLETE && m_NavigateState == NAVIGATE_WAIT) {

		m_NavigateState = NAVIGATE_OK_READY;		
		SetWindowPos(m_hWnd, HWND_TOP, m_ptPosition.x, m_ptPosition.y, m_szeSize.cx, m_szeSize.cy, SWP_SHOWWINDOW);		
		hr = m_pWebBrowser->put_Visible (VARIANT_TRUE);		
		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);

		SetFocus(m_hMainWnd);

		//m_pWebBrowser->put_ToolBar(FALSE);
		//m_pWebBrowser->put_AddressBar(VARIANT_FALSE);
		//m_pWebBrowser->put_MenuBar(VARIANT_FALSE);
		//m_pWebBrowser->put_StatusBar(VARIANT_FALSE);
		//m_pWebBrowser->put_Resizable(VARIANT_FALSE);
		//m_pWebBrowser->put_RegisterAsDropTarget(VARIANT_FALSE);

		//if(m_iIEVer == 7) {
		//	m_pWebBrowser->put_FullScreen(VARIANT_TRUE );
		//}
		//::SendMessage(m_hMainWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);

		////Sleep(200);
		//::SetForegroundWindow( m_hMainWnd );
		//::SetFocus(m_hWnd);
		OutputDebugString( "	-IGW Navigate OK\n");

		//::SendMessage(m_hMainWnd, WM_ACTIVATE, true, 0);
		//::SendMessage(m_hMainWnd, WM_NCACTIVATE, true, 0);
		::SendMessage(m_hMainWnd, WM_ACTIVATEAPP, true, 0);
		
		SetFocus(m_hMainWnd);
	}

}

void SPIngameWebbrowser::Quit ()
{	

	UnEmbedBrowserObject(m_hWnd);

	ShowWindow(m_hWnd, SW_HIDE);
	UpdateWindow(m_hWnd);

	OutputDebugString( "	-IGW quit\n");
	SendMessage(m_hMainWnd, WM_ACTIVATE, TRUE, 0);
	return;

//////////////////////////////////////////////////////////////////////////
	if (m_pWebBrowser == NULL)
		return;

	if(m_NavigateState == NAVIGATE_NULL) 
		return;

	m_pWebBrowser->Quit ();
	m_NavigateState = NAVIGATE_NULL;
}

void SPIngameWebbrowser::SetReadyCallBack(pfCallbackWebEnable apfCallbackWebEnable)
{
	m_pfCallbackWebEnable = apfCallbackWebEnable;
}



void SPIngameWebbrowser::UnEmbedBrowserObject(HWND hwnd)
{
	//Quit();

	if(m_pWebBrowser)
		m_pWebBrowser->Quit();

	SAFE_RELEASE(m_pWebBrowser);

	if(m_pBrowserObject) {
		m_pBrowserObject->Close(OLECLOSE_NOSAVE);
		SAFE_RELEASE(m_pBrowserObject);
	}

	m_NavigateState = NAVIGATE_NULL;
	if(m_pfCallbackWebEnable)
		m_pfCallbackWebEnable(false);

	OutputDebugString( "	-IGW Unembeded browser\n");

	return;
}


long SPIngameWebbrowser::EmbedBrowserObject(HWND hwnd)
{
	IOleObject*			browserObject;
	IWebBrowser2		*webBrowser2;
	RECT				rect;
	char				*ptr;
	
	if (!OleCreate( CLSID_WebBrowser, IID_IOleObject, OLERENDER_DRAW, 0, &m_IOleClientSite, &m_IStorage, (void**)&browserObject)) {				
		m_IOleInPlaceSite.SetBrowserObject(browserObject);
		m_pBrowserObject = browserObject;
		browserObject->SetHostNames( L"metalgeni_test", 0);
		GetClientRect(hwnd, &rect);
		
		if (!OleSetContainedObject((struct IUnknown *)browserObject, TRUE) &&			
			!browserObject->DoVerb( OLEIVERB_SHOW, NULL, (IOleClientSite *)&m_IOleClientSite, -1, hwnd, &rect) &&
			!browserObject->QueryInterface(IID_IWebBrowser2, (void**)&webBrowser2))
		{
			m_pWebBrowser = webBrowser2;
			//m_pWebBrowser->put_ToolBar(FALSE);
			//m_pWebBrowser->put_AddressBar(VARIANT_FALSE);
			//m_pWebBrowser->put_MenuBar(VARIANT_FALSE);
			//m_pWebBrowser->put_StatusBar(VARIANT_FALSE);
			//m_pWebBrowser->put_Resizable(VARIANT_FALSE);
			//m_pWebBrowser->put_RegisterAsDropTarget(VARIANT_FALSE);

			m_pWebBrowser->put_Width(m_szeSize.cx);
			m_pWebBrowser->put_Height(m_szeSize.cy);
			//m_pWebBrowser->put_Left(m_ptPosition.x);
			//m_pWebBrowser->put_Top(m_ptPosition.y);

			m_pWebBrowser->put_Left(0);
			m_pWebBrowser->put_Top(0);

			//m_pWebBrowser->put_Visible (VARIANT_FALSE);
			return 0;
		}
	}

	assert(0);
	UnEmbedBrowserObject(hwnd);	
	return -2;
}







