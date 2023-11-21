
#include <string>

#include <assert.h>
#include <comutil.h>

#include <D3D9.h>
#include <d3dx9.h>

#include <MsHTML.h>

#include <Shlguid.h>
#include <atlcomcli.h>

#include <Oleacc.h>


#include "DXUtil.h"
#include "CD3DEnumeration.h"
#include "D3DSettings.h"

#include "SPVideoCommon.h"
#include "SPTextureMgr.h"
#include "SPLine.h"
#include "SPFont.h"
#include "SPVideo.h"


#include "dxutil.h"

#include "SPIngameWebbrowser3.h"



using namespace std;

#pragma comment(lib, "comsupp.lib")


typedef struct _IELAUNCHURLINFO {

	DWORD cbSize;

	DWORD dwCreationFlags;

} IELAUNCHURLINFO, *LPIELAUNCHURLINFO;
typedef BOOL (WINAPI *PFN_IELaunchURL)(LPCWSTR szUrl, LPPROCESS_INFORMATION pProcInfo, LPIELAUNCHURLINFO lpInfo);


SPIngameWebbrowser3* g_pIngameWebBrowser = NULL;

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


BOOL CALLBACK EnumWinProc(HWND hWnd, LPARAM lParam)
{

	TCHAR  szClassName[256];
	GetClassName(hWnd, (LPTSTR)&szClassName, 256);
	if (_tcscmp(szClassName, _T("IEFrame")) == 0)
	{
		*(HWND*)lParam = hWnd;
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK EnumChildWinProc(HWND hWnd, LPARAM lParam)
{
	TCHAR  szClassName[256];
	GetClassName(hWnd, (LPTSTR)&szClassName, 256);
	if (_tcscmp(szClassName, _T("Internet Explorer_Server")) == 0)
	{
		// 진짜 IE7의 Internet Explorer_Server가 맞나?
		HWND hParent = GetParent(hWnd);
		if (hParent)
		{
			hParent = GetParent(hParent);
			if (hParent)
			{
				GetClassName(hParent, (LPTSTR)&szClassName, 256);
				if (_tcscmp(szClassName, _T("TabWindowClass")) == 0)
				{
					*(HWND*)lParam = hWnd;
					return FALSE;
				}

			}
		}
	}
	return TRUE;
}




SPIngameWebbrowser3::SPIngameWebbrowser3(HWND hWnd)
: m_pWebBrowser(NULL)
, m_hMainWnd(hWnd)
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

	//CreateNewInstance();
	//SAFE_RELEASE(m_pWebBrowser);

	m_bNotDisplayChange = false;
}

SPIngameWebbrowser3::~SPIngameWebbrowser3()
{	
	Quit();
	//SAFE_RELEASE(m_pWebBrowser);	
	CoUninitialize();

	strcpy(m_szIEVer, "6.0");
}

void SPIngameWebbrowser3::SetMainWidow(HWND hWnd)
{
	m_hMainWnd = hWnd;
}

bool SPIngameWebbrowser3::CheckIEVersion()
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


bool SPIngameWebbrowser3::CreateNewInstance ()
{
	if (m_pWebBrowser != NULL)
	{
		//SAFE_RELEASE(m_pWebBrowser);		
		//return false;
	}

	HRESULT hr;
	IWebBrowser2* pWebBrowser = NULL;

	m_bNotDisplayChange = true;


	//////////////////////////////////////////////////////////////////////////

	//HMODULE hModule = LoadLibrary(_T("ieframe.dll"));
	//if (!hModule)
	//	return false;

	//PFN_IELaunchURL pfnIELaunchURL = (PFN_IELaunchURL)GetProcAddress(hModule, _T("IELaunchURL"));
	//if (!pfnIELaunchURL)
	//{
	//	FreeLibrary(hModule);
	//	return false;
	//}


	//PROCESS_INFORMATION pProcInfo;
	////hr = (pfnIELaunchURL)(L"http://bill.latale.jp/login.ashx?userid=actoz0001&token=azt7gciy", &pProcInfo, NULL);
	//hr = (pfnIELaunchURL)(L"", &pProcInfo, NULL);
	//if (FAILED(hr))
	//{
	//	FreeLibrary(hModule);
	//	return false;
	//}


	//// 프로세스가 초기화되고 IE창이 뜰 때까지 기다림

	//HWND hWndIEFrame = NULL;
	//HWND hWnd = NULL;
	//int nWait = 100;
	//while ((hWnd == NULL) && (nWait > 0))
	//{
	//	nWait--;	
	//	WaitForInputIdle(pProcInfo.hProcess, 30000);
	//	if (hWndIEFrame == NULL)
	//		EnumThreadWindows(pProcInfo.dwThreadId, &EnumWinProc, (LPARAM)&hWndIEFrame);		

	//	//SetWindowLong(hWndIEFrame, GWL_STYLE, WS_CHILD);
	//	//SetParent(hWndIEFrame, m_hMainWnd);
	//	if (hWndIEFrame)
	//		EnumChildWindows(hWndIEFrame, &EnumChildWinProc, (LPARAM)&m_hWnd);


	//	ShowWindow(m_hWnd, SW_HIDE);
	//	SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD );

	//	SetParent(m_hWnd, m_hMainWnd);	


	//	Sleep(100);
	//}

	//WaitForInputIdle(pProcInfo.hProcess, 30000);
	//CloseHandle(pProcInfo.hProcess);
	//CloseHandle(pProcInfo.hThread);

	//if (hWnd == NULL)
	//{
	//	FreeLibrary(hModule);
	//	return false;
	//}


	//CComPtr<IHTMLDocument2> spDoc;
	//LRESULT lRes;

	//UINT nMsg = ::RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));
	//::SendMessageTimeout(hWnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 10000, (DWORD*)&lRes);

	//if (FAILED(ObjectFromLresult(lRes, __uuidof(IHTMLDocument2), 0, (void**)&spDoc)))
	//{
	//	FreeLibrary(hModule);
	//	return false;
	//}    

	//CComQIPtr<IServiceProvider> spsp1;
	//CComQIPtr<IServiceProvider> spsp2;
	//CComPtr<IWebBrowser2> spwb;

	//spsp1 = spDoc;

	//spsp1->QueryService(SID_STopLevelBrowser, IID_IServiceProvider, (void**)&spsp2);
	//spsp2->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (void**)&spwb);



	////m_ctrlNotice.CreateControl( CLSID_WebBrowser , NULL , WS_VISIBLE | WS_CHILD , m_rcBrowser , this , IDC_ACTIVEX_WEB );
	////m_spBrowser	=	m_ctrlNotice.GetControlUnknown();


	////CreateWindowExA(WS_VISIBLE | WS_CHILD, "","",)
	//

	//

	//m_pWebBrowser = spwb;

	//m_pWebBrowser->put_Resizable(FALSE);

	//m_pWebBrowser->put_AddressBar(FALSE);

	//m_pWebBrowser->put_MenuBar(FALSE);

	//m_pWebBrowser->put_ToolBar(FALSE);

	//m_pWebBrowser->put_StatusBar(FALSE);

	////m_pWebBrowser->get_HWND((long*)&m_hWnd);
	////SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD);
	////SetParent(m_hWnd, m_hMainWnd);		





	//////////////////////////////////////////////////////////////////////////
	//m_ctrlNotice.CreateControl( CLSID_WebBrowser , NULL , WS_VISIBLE | WS_CHILD , m_rcBrowser , this , IDC_ACTIVEX_WEB );

	//hr = CoCreateInstance (CLSID_InternetExplorer, NULL, CLSCTX_SERVER, IID_IWebBrowser2, (LPVOID*)&pWebBrowser);
	hr = CoCreateInstance (CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (LPVOID*)&pWebBrowser);
	

	if (SUCCEEDED (hr) && (pWebBrowser != NULL))
	{
		m_pWebBrowser = pWebBrowser;
		m_pWebBrowser->get_HWND((long*)&m_hWnd);

		SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD);
		SetParent(m_hWnd, m_hMainWnd);		

		// Capture Explorer HWND 
		//if((m_hWnd = FindWindow("IEFrame", "Microsoft Internet Explorer")) == NULL) 
		//	return FALSE; 

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

		m_pWebBrowser->put_Visible (VARIANT_FALSE);

		return true;
	}
	else
	{
		m_bNotDisplayChange = false;
		if (pWebBrowser) {
			//SAFE_RELEASE(m_pWebBrowser);
		}
		OutputDebugString( "	-IGW navigate failed\n");
		return false;
	}
	return false;
}



bool SPIngameWebbrowser3::IsValid ()
{
	if (m_pWebBrowser == NULL)
		return false;

	return true;
}


bool SPIngameWebbrowser3::IsNavigate()
{
	if(m_pWebBrowser == NULL)
		return false;

	if(m_NavigateState == NAVIGATE_NULL ) {
		return  false;
	}

	return true;;
}

void SPIngameWebbrowser3::SetRelease()
{
	//SAFE_RELEASE(m_pWebBrowser);	
	Active(false);
}


void SPIngameWebbrowser3::Active(bool bActive)
{
	if( m_NavigateState == NAVIGATE_NULL)
		return;
}


bool SPIngameWebbrowser3::IsWait()
{
	if(m_NavigateState == NAVIGATE_WAIT)
		return true;

	if(m_bPreloaded == true)
		return true;

	return false;
}

bool SPIngameWebbrowser3::IsLoadOK()
{
	if(m_NavigateState == NAVIGATE_OK) {
		//m_NavigateState = NAVIGATE_NULL;
		return true;
	}
	return false;
	//return (m_NavigateState == NAVIGATE_OK);
}

bool SPIngameWebbrowser3::IsPreloaded()
{
	return m_bPreloaded;
}

bool SPIngameWebbrowser3::IsNotDisplayChange()
{
	return m_bNotDisplayChange;
}


bool SPIngameWebbrowser3::Navigate(LPCTSTR lpszURL, DWORD dwFlags /* = 0 */,
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

	BSTR bstrFrame = _com_util::ConvertStringToBSTR(""); // 
	//BSTR bstrFrame = _com_util::ConvertStringToBSTR("_PARENT"); // 
	//BSTR bstrFrame = _com_util::ConvertStringToBSTR(TEXT("SPLibary Test")); // 
	//vars[0].vt = VT_BSTR;
	//vars[0].bstrVal = bstrPostURL;	

	//vars[1].vt = VT_BSTR;
	//vars[1].bstrVal = bstrFrame;

	//vars[2].vt = VT_BSTR;
	//vars[2].bstrVal = bstrPostURL;

	//vars[3].vt = VT_BSTR;
	//vars[3].bstrVal = bstrPostURL;


	HRESULT hrie = m_pWebBrowser->Navigate(bstrURL, vars,vars+1,vars+2,vars+3);	
	//HRESULT hrie = m_pWebBrowser->Navigate(bstrURL, vars,vars+1,vars+2,vars+3);
	m_pWebBrowser->put_Visible(VARIANT_TRUE);
	
	if (SUCCEEDED(hrie)) {

		// Capture Explorer HWND 
		//if((m_hWnd = FindWindow("IEFrame", "Microsoft Internet Explorer")) == NULL) 
		//	return FALSE; 

		//m_pWebBrowser->get_HWND((long*)&m_hWnd);
		//SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD);
		//SetParent(m_hWnd, m_hMainWnd);		



		m_NavigateState = NAVIGATE_WAIT;
		m_bNotDisplayChange = true;
		m_bPreload = bPreload;
		return true;
	}

	return false;
}


void SPIngameWebbrowser3::Process(float fTime)
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
			//SAFE_RELEASE(m_pWebBrowser);
			m_NavigateState = NAVIGATE_OK;

			//SetFocus(m_hMainWnd);

			::SetForegroundWindow( m_hMainWnd );
			::SetFocus(m_hWnd);


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
			//::SetFocus(m_hWnd);
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


void SPIngameWebbrowser3::Quit ()
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

READYSTATE SPIngameWebbrowser3::GetReadyState() const
{
	assert (m_pWebBrowser != NULL);
	if (m_pWebBrowser == NULL)
		return READYSTATE_UNINITIALIZED;

	READYSTATE result;
	m_pWebBrowser->get_ReadyState (&result);
	return result;
}


const WCHAR*  SPIngameWebbrowser3::GetType() const
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

