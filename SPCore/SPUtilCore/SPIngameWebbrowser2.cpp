
#include <string>

#include <assert.h>
#include <comutil.h>

#include <D3D9.h>
#include <d3dx9.h>

#include <MsHTML.h>


#include "DXUtil.h"
#include "CD3DEnumeration.h"
#include "D3DSettings.h"

#include "SPVideoCommon.h"
#include "SPTextureMgr.h"
#include "SPLine.h"
#include "SPFont.h"
#include "SPVideo.h"


#include "dxutil.h"

#include "SPIngameWebbrowser2.h"



using namespace std;

#pragma comment(lib, "comsupp.lib")


SPIngameWebbrowser2* g_pIngameWebBrowser = NULL;

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


SPIngameWebbrowser2::SPIngameWebbrowser2()
: m_pWebBrowser(NULL)
, m_hMainWnd(NULL)
, m_NavigateState(NAVIGATE_NULL)
, m_fElapsed(0.0f)
, m_bPreload(false)
, m_bPreloaded(false)
, m_iIEVer(0)
, m_bNotDisplayChange(false)
{
	CoInitialize(NULL);
	m_strLastUrl = "NON";

	CheckIEVersion();

	CreateNewInstance();
	SAFE_RELEASE(m_pWebBrowser);

	m_bNotDisplayChange = false;
}

SPIngameWebbrowser2::~SPIngameWebbrowser2()
{	
	Quit();
	SAFE_RELEASE(m_pWebBrowser);	
	CoUninitialize();

	strcpy(m_szIEVer, "6.0");
}

void SPIngameWebbrowser2::SetMainWidow(HWND hWnd)
{
	m_hMainWnd = hWnd;
}

bool SPIngameWebbrowser2::CheckIEVersion()
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


bool SPIngameWebbrowser2::CreateNewInstance ()
{
	if (m_pWebBrowser != NULL)
	{
		SAFE_RELEASE(m_pWebBrowser);		
		//return false;
	}

	HRESULT hr;
	IWebBrowser2* pWebBrowser = NULL;

	m_bNotDisplayChange = true;

	hr = CoCreateInstance (CLSID_InternetExplorer, NULL, CLSCTX_SERVER, IID_IWebBrowser2, (LPVOID*)&pWebBrowser);
	//hr = CoCreateInstance (CLSID_InternetExplorer, NULL, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, IID_IWebBrowser2, (LPVOID*)&pWebBrowser);

	//hr = CoCreateInstance (CLSID_InternetExplorer, NULL, 
	//	CLSCTX_SERVER , IID_IWebBrowser2, (LPVOID*)&pWebBrowser);

	//CLSID_WebBrowser
	//CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER
	//IID_IOleObject
	
	//CO_E_NOTINITIALIZED

	if (SUCCEEDED (hr) && (pWebBrowser != NULL))
	{
		m_pWebBrowser = pWebBrowser;
		m_pWebBrowser->get_HWND((long*)&m_hWnd);

		//SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD);
		//SetParent(m_hWnd, m_hMainWnd);		

		m_pWebBrowser->put_ToolBar(FALSE);
		m_pWebBrowser->put_AddressBar(VARIANT_FALSE);
		m_pWebBrowser->put_MenuBar(VARIANT_FALSE);
		m_pWebBrowser->put_StatusBar(VARIANT_FALSE);
		m_pWebBrowser->put_Resizable(VARIANT_FALSE);
		m_pWebBrowser->put_RegisterAsDropTarget(VARIANT_FALSE);
		m_pWebBrowser->put_Width(800);
		m_pWebBrowser->put_Height(600);
		m_pWebBrowser->put_Left(0);
		m_pWebBrowser->put_Top(0);			

		//CCODE
		//m_pWebBrowser->put_FullScreen(VARIANT_TRUE );
		//::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);	
		m_pWebBrowser->put_Visible (VARIANT_FALSE);

		return true;
	}
	else
	{
		m_bNotDisplayChange = false;
		if (pWebBrowser) {
			SAFE_RELEASE(m_pWebBrowser);
		}
		OutputDebugString( "	-IGW navigate failed\n");
		return false;
	}

	return false;
}



bool SPIngameWebbrowser2::IsValid ()
{
	if (m_pWebBrowser == NULL)
		return false;

	return true;
}


bool SPIngameWebbrowser2::IsNavigate()
{
	if(m_pWebBrowser == NULL)
		return false;

	if(m_NavigateState == NAVIGATE_NULL ) {
		return  false;
	}

	// Too Slow..
	//HWND hwnd = NULL;
	//m_pWebBrowser->get_HWND((long*)&hwnd);
	//if(hwnd == NULL) {
	//	m_NavigateState = NAVIGATE_NULL;
	//	return false;
	//}

	return true;;
}

void SPIngameWebbrowser2::SetRelease()
{
	SAFE_RELEASE(m_pWebBrowser);	
	Active(false);
}


void SPIngameWebbrowser2::Active(bool bActive)
{
	if( m_NavigateState == NAVIGATE_NULL)
		return;

	DEVMODE dm;
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


bool SPIngameWebbrowser2::IsWait()
{
	if(m_NavigateState == NAVIGATE_WAIT)
		return true;

	if(m_bPreloaded == true)
		return true;

	return false;
}

bool SPIngameWebbrowser2::IsLoadOK()
{
	if(m_NavigateState == NAVIGATE_OK) {
		//m_NavigateState = NAVIGATE_NULL;
		return true;
	}
	return false;
	//return (m_NavigateState == NAVIGATE_OK);
}

bool SPIngameWebbrowser2::IsPreloaded()
{
	return m_bPreloaded;
}

bool SPIngameWebbrowser2::IsNotDisplayChange()
{
	return m_bNotDisplayChange;
}

//bool SPIngameWebbrowser2::ShowPreload()
//{
//	if(IsPreloaded() == false) 
//		return false;
//
//	m_NavigateState = NAVIGATE_OK;
//
//	m_pWebBrowser->get_HWND((long*)&m_hWnd);
//	SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD);
//	SetParent(m_hWnd, m_hMainWnd);
//
//	if(m_iIEVer == 7) {
//		m_pWebBrowser->put_FullScreen(VARIANT_TRUE );
//	}
//	::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
//
//	m_pWebBrowser->put_Visible (VARIANT_TRUE);
//
//	//CCODE
//	//SAFE_RELEASE(m_pWebBrowser);
//	::SetForegroundWindow( m_hWnd );
//	m_bPreloaded = false;
//}

bool SPIngameWebbrowser2::Navigate(LPCTSTR lpszURL, DWORD dwFlags /* = 0 */,
								  LPCTSTR lpszTargetFrameName /* = NULL */ ,
								  LPCTSTR lpszHeaders /* = NULL */, LPVOID lpvPostData /* = NULL */,
								  DWORD dwPostDataLen /* = 0 */,
								  bool bPreload)
{

	if(m_pWebBrowser == NULL) {
		assert( m_pWebBrowser == NULL );			
		return false;
	}

	if(bPreload == true)
		return false;

	//test string
	//BSTR bstrURL = _com_util::ConvertStringToBSTR("http://ruliweb.empas.com/ruliboard/read.htm?main=xbox&table=game_xbox02&page=6&num=17964&find=&ftext=&left=b&time=");
	//BSTR bstrURL = _com_util::ConvertStringToBSTR("http://bill.latale.jp/sample.aspx"); // 
	//http://test.lazeska.com/Special/Default.asp?charactor=undefined

	m_strLastUrl = lpszURL;


	//BSTR bstrURL = _com_util::ConvertStringToBSTR("http://ruliweb.empas.com"); // 
	BSTR bstrURL = _com_util::ConvertStringToBSTR(lpszURL); // 

	VARIANT varURL;
	VARIANT vars[4];
	//memset(vars,0,sizeof(vars));	
	VariantInit(&vars[0]);
	VariantInit(&vars[1]);
	VariantInit(&vars[2]);
	VariantInit(&vars[3]);


	VariantInit(&varURL);
	varURL.vt = VT_BSTR;
	varURL.bstrVal = bstrURL;

	m_pWebBrowser->Refresh();


	BSTR bstrPostURL = _com_util::ConvertStringToBSTR("main/user_news.htm"); //

	//http://ruliweb.empas.com/main/user_news.htm
	//http://bill.latale.jp/login.ashx?userid=actoz0001&token=azt7gciy

	BSTR bstrFrame = _com_util::ConvertStringToBSTR("_SELF"); // 
	//BSTR bstrFrame = _com_util::ConvertStringToBSTR("_PARENT"); // 
	//BSTR bstrFrame = _com_util::ConvertStringToBSTR(TEXT("SPLibary Test")); // 
	//vars[0].vt = VT_BSTR;
	//vars[0].bstrVal = bstrPostURL;	

	vars[1].vt = VT_BSTR;
	vars[1].bstrVal = bstrFrame;

	//vars[2].vt = VT_BSTR;
	//vars[2].bstrVal = bstrPostURL;

	//vars[3].vt = VT_BSTR;
	//vars[3].bstrVal = bstrPostURL;


	HRESULT hrie = m_pWebBrowser->Navigate(bstrURL, vars,vars+1,vars+2,vars+3);	
	//HRESULT hrie = m_pWebBrowser->Navigate(bstrURL, vars,vars+1,vars+2,vars+3);	
	
	if (SUCCEEDED(hrie)) {
		m_NavigateState = NAVIGATE_WAIT;
		m_bNotDisplayChange = true;
		m_bPreload = bPreload;
		return true;
	}

	return false;
}


void SPIngameWebbrowser2::Process(float fTime)
{
	HRESULT hr;

	if(m_pWebBrowser == NULL) {
		return;
	}

	if( m_pWebBrowser && m_NavigateState == NAVIGATE_OK_READY )
	{
		VARIANT_BOOL bVisible = VARIANT_TRUE;
		hr = m_pWebBrowser->get_Visible(&bVisible);
		if( bVisible == VARIANT_FALSE )
		{
			//m_pWebBrowser->put_Visible (VARIANT_TRUE);
			m_bNotDisplayChange = false;
			SAFE_RELEASE(m_pWebBrowser);
			m_NavigateState = NAVIGATE_OK;

			SetFocus(m_hMainWnd);

			//::SetForegroundWindow( m_hMainWnd );
			//::SetFocus(m_hWnd);


			OutputDebugString( "	-IGW delete, send WM_NCACTIVATE, true\n");
			SendMessage(m_hMainWnd, WM_NCACTIVATE, TRUE, 0);
			
		} else {

			//SetFocus(m_hMainWnd);
			//m_bNotDisplayChange = false;
			//SetFocus(m_hMainWnd);

			//::SetForegroundWindow( m_hMainWnd );
			//::SetFocus(m_hWnd);

			//SAFE_RELEASE(m_pWebBrowser);
			//m_NavigateState = NAVIGATE_OK;
			//SendMessage(m_hMainWnd, WM_NCACTIVATE, TRUE, 0);

			//OutputDebugString( "	-IGW delete, send WM_NCACTIVATE, true\n");

		}
	}



	//if(m_NavigateState != NAVIGATE_WAIT) {
	//	m_fElapsed = 0.0f;
	//	//SetFocus(m_hMainWnd);
	//	//::SetForegroundWindow( m_hMainWnd );
	//	//::SetFocus(m_hWnd);
	//	return;
	//}

	m_fElapsed += fTime;
	if(m_fElapsed < WAITTICKTIME) {
		return;
	}

	m_fElapsed = 0.0f;

	READYSTATE result;
	VARIANT_BOOL bBusy = VARIANT_TRUE;	
	if(m_pWebBrowser) {
		//m_pWebBrowser->get_Busy(&bBusy);
		m_pWebBrowser->get_ReadyState (&result);
	}



	//if(bBusy == VARIANT_FALSE && m_NavigateState == NAVIGATE_WAIT) {
	if(result == READYSTATE_COMPLETE && m_NavigateState == NAVIGATE_WAIT) {

		//m_NavigateState = NAVIGATE_OK;

		if(m_bPreload == false) {
			m_NavigateState = NAVIGATE_OK_READY;

			//CCODE
			hr = m_pWebBrowser->get_HWND((long*)&m_hWnd);

			SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD);
			SetParent(m_hWnd, m_hMainWnd);

			if(m_iIEVer == 7) {
				m_pWebBrowser->put_FullScreen(VARIANT_TRUE );
			} 	

			::SendMessage(m_hMainWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);	
			//::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);	

			hr = m_pWebBrowser->put_Visible (VARIANT_TRUE);			

			Sleep(100);
			::SetForegroundWindow( m_hMainWnd );
			::SetFocus(m_hWnd);
			OutputDebugString( "	-IGW Busy false, Maximize\n");

		} else {
			//Quit();
			//m_pWebBrowser->put_Visible (VARIANT_FALSE);
			m_bNotDisplayChange = false;
			SetFocus(m_hMainWnd);
			SendMessage(m_hMainWnd, WM_NCACTIVATE, TRUE, 0);
			m_NavigateState = NAVIGATE_PRELOADED;
			m_bPreloaded = true;
		}
	}	
}

const char* SPIngameWebbrowser2::GetLastURL()
{
	return m_strLastUrl.c_str();
}



HWND SPIngameWebbrowser2::GetHWND()
{
	if(m_pWebBrowser == NULL)
		return NULL;

	m_pWebBrowser->get_HWND((long*)&m_hWnd);

	return m_hWnd;
}

bool SPIngameWebbrowser2::WaitTillLoaded (int nTimeout)
{
	READYSTATE result;
	DWORD nFirstTick = GetTickCount ();

	do
	{
		m_pWebBrowser->get_ReadyState (&result);

		if (result != READYSTATE_COMPLETE)
			Sleep (250);

		if (nTimeout > 0)
		{
			if ((GetTickCount () - nFirstTick) > nTimeout)
				break;
		}
	} while (result != READYSTATE_COMPLETE);

	if (result == READYSTATE_COMPLETE)
		return true;
	else
		return false;
}

void SPIngameWebbrowser2::Refresh ()
{
	assert (m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return;

	m_pWebBrowser->Refresh ();
}

void SPIngameWebbrowser2::Stop ()
{
	assert (m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return;

	m_pWebBrowser->Stop ();
}

void SPIngameWebbrowser2::GoBack ()
{
	assert (m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return;

	m_pWebBrowser->GoBack ();
}

void SPIngameWebbrowser2::GoForward ()
{
	assert (m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return;

	m_pWebBrowser->GoForward ();
}

void SPIngameWebbrowser2::Quit ()
{
	//assert (m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return;

	if(m_NavigateState == NAVIGATE_NULL) 
		return;

	m_pWebBrowser->Quit ();
	//SetRelease();

	m_NavigateState = NAVIGATE_NULL;

}

bool SPIngameWebbrowser2::GetBusy() const
{
	assert (m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return false;

	VARIANT_BOOL result;
	m_pWebBrowser->get_Busy (&result);
	return result ? true : false;
}

READYSTATE SPIngameWebbrowser2::GetReadyState() const
{
	assert (m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return READYSTATE_UNINITIALIZED;

	READYSTATE result;
	m_pWebBrowser->get_ReadyState (&result);
	return result;
}

LPDISPATCH SPIngameWebbrowser2::GetHtmlDocument() const
{
	assert (m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return NULL;

	LPDISPATCH result;
	m_pWebBrowser->get_Document (&result);
	return result;
}


const WCHAR*  SPIngameWebbrowser2::GetFullName() const
{
	assert (m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return NULL;

	BSTR bstr;
	m_pWebBrowser->get_FullName (&bstr);
	wstring retVal(bstr);

	SysFreeString (bstr);
	return retVal.c_str();
}

const WCHAR*  SPIngameWebbrowser2::GetType() const
{
	assert (m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return NULL;

	BSTR bstr;
	m_pWebBrowser->get_Type (&bstr);
	wstring retVal(bstr);

	SysFreeString(bstr);
	return retVal.c_str();
}

const WCHAR* SPIngameWebbrowser2::GetLocationName() const
{
	assert(m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return NULL;

	BSTR bstr;
	m_pWebBrowser->get_LocationName (&bstr);
	wstring retVal(bstr);

	SysFreeString (bstr); // Added this line to prevent leak.
	return retVal.c_str();
}

const WCHAR*  SPIngameWebbrowser2::GetLocationURL() const
{
	assert (m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return NULL;

	BSTR bstr;
	m_pWebBrowser->get_LocationURL (&bstr);
	wstring retVal(bstr);

	SysFreeString (bstr); // Added this line to prevent leak.
	return retVal.c_str();
}

