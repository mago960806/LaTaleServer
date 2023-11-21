// LataleClient.cpp : ?? ?¯À¯…€ÈÒ’ ¬ µ ◊≥??E ?’…??ıµ.
//

#include <vector>
#include <string>
#include <process.h>

#include "SPCommon.h"
#include "SPDebug.h"
#include "_Error_log.h"
#include "SPFrameSkipper.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"

#include "LataleVersionDef.h"
#include "LataleClient.h"

#include "SPArgManager/SPArgManager.h"
#include "resource.h"
#include "SPResourceDEF.h"

#include "./NPGameGuard/SPGameGuard.h"		// AJJIYA [11/15/2005]
#include "./SPError/MiniDumper.h"			//  [2/24/2006 AJJIYA]

#include "SPLogoThread.h"

#include "SPRefOLE.h" // for IngameWB
#include "SPIngameWebbrowser.h"

#include "lgclientlog.h"

#include "AutoInterface.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "IGWInterface.h"
#include "IGWLoaderDx9.h"
#include "SPUtil.h"
#define MAX_LOADSTRING 100
#define TITLE_NAME	TEXT("LaTale Client")
#define CLASS_NAME	TEXT("LATALE_CLIENT")
#define BGTIMER		1024	

#define WM_CLOSEIGWMSG	WM_USER + 0x11
HWND		g_hWnd;
HINSTANCE	g_hInst;							// ??E?E?E??ıµ.

TCHAR szTitle[MAX_LOADSTRING];					// ’ÙÃ´ ??? ¯”????ıµ.
TCHAR szWindowClass[MAX_LOADSTRING];			// ¯∑? ÿ” “Ω?? ????ıµ.

const int g_iScrWid = 800;
const int g_iScrHei = 600;

//const int g_iScrWid = 1024;
//const int g_iScrHei = 768;

DEVMODE g_OrgDM;
DEVMODE g_InGameDM;
bool g_bClipChild = false;
bool g_bFSDA = false;		// Sanda Admin (for Windowed Mode)
typedef int ( *GmWindow )();
int GmWindowed();
// 2007/2/1  appended by hoshim : ∏Æº“Ω∫ ∆˙¥ı∏¶ Ω««‡¿Œ¿⁄∑Œ ¡ˆ¡§∞°¥…«œ∞‘ «œ±ÅE¿ß«‘.
//									Ex) /ResPath://BELOS/RESOURCE/CLIENT/
#ifdef _RES_PATH
TCHAR RES_WORK_RESOURCE_PATH[MAX_PATH] = "../../Resouce/debug/";//"//BELOS/RESOURCE/CLIENT/";
#endif


//////////////////////////////////////////////////////////////////////////
// debug for VTune
// if not running tuning job, delete this
//#define _RES_PATH

//#ifdef _RES_PATH
//	#undef _RES_PATH
//#endif
//
//#ifdef _RES_BELOS
//	#undef _RES_BELOS
//#endif
//////////////////////////////////////////////////////////////////////////


BOOL g_bActive   = FALSE; // Whether the app is active (not minimized)
BOOL g_bReady    = FALSE; // Whether the app is ready to render frames
static bool s_bNotifyActivated = false;


BOOL g_bHasFocus = FALSE;
bool g_bExternalCursor = true;

TCHAR g_szLoginID[24] = { 0, };
TCHAR g_szCharName[24] = { 0, };
bool g_LocalPlayerMoveDisable = false;
INITTHREAD_INFO g_InitialThreadInfo ; 

CRITICAL_SECTION g_CriticalSection;

#ifdef _DEBUG 
	bool g_bRunApproval = true;
#else
	bool g_bRunApproval = false;
#endif

extern bool g_bUsePackFile;
extern bool g_bWindowed = false;

//#ifdef NDEBUG
//extern bool g_bWindowed = false;
//#else
//extern bool g_bWindowed = true;
//#endif

TCHAR szLog[256];


VIDEO_QUALITY g_VideoQuality = VIDEOQUAL_HIGH;

COUNTRY_LOCALE	g_eCountryLocale	=	CL_CHINA;		// AJJIYA [11/1/2006 AJJIYA]
bool			g_bGameGuardPatch	=	false;			// AJJIYA [5/26/2008 AJJIYA]

STICKYKEYS g_StartupStickyKeys = {sizeof(STICKYKEYS), 0};	//	∞˙›§≈∞
TOGGLEKEYS g_StartupToggleKeys = {sizeof(TOGGLEKEYS), 0};	//	≈‰±€≈∞
FILTERKEYS g_StartupFilterKeys = {sizeof(FILTERKEYS), 0};	//	« ≈Õ≈∞

//////////////////////////////////////////////////////////////////////////
// º¢¥Ÿ ?E?
//#ifdef _INCLUDE_RSCLIENT_
	#include "SPRsManager.h"
//#endif


LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
static bool Run();
static bool NotifyActivate(bool bActivate);
//unsigned __stdcall ThreadLogo( LPVOID pParam );
unsigned __stdcall ThreadLoad( LPVOID pParam );
void AllowAccessibilityShortcutKeys( bool bAllowKeys );
//bool CreateLogoThread() ;
bool CreateLoadThread() ;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{	
	//  [2/24/2006 AJJIYA]
	MiniDumper	md( MiniDumper::DUMP_LEVEL_2 );

	WNDCLASSEX	wndClassEx;
	wndClassEx.cbSize        = sizeof(wndClassEx);
	wndClassEx.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	wndClassEx.lpfnWndProc   = WndProc;
	wndClassEx.cbClsExtra    = 0;
	wndClassEx.cbWndExtra    = 0;
	wndClassEx.hInstance     = g_hInst;
	wndClassEx.hIcon         = LoadIcon( hInstance , MAKEINTRESOURCE( IDI_LATALECLIENT ) );
	wndClassEx.hIconSm       = LoadIcon( hInstance , MAKEINTRESOURCE( IDI_SMALL ) );
	wndClassEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClassEx.lpszMenuName  = NULL;		
	wndClassEx.lpszClassName = CLASS_NAME;

	if( 0 == RegisterClassEx(&wndClassEx) )
		return -1;

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	

#ifdef _RES_BELOS
	g_bUsePackFile = false;
#elif _RES_ELIAS
	g_bUsePackFile = false;
#else 
	g_bUsePackFile = true;
#endif

	//////////////////////////////////////////////////////////////////////////
	// Excute Option Init
	SPArgManager ArgManage(lpCmdLine);
	ArgManage.ExcuteArgument();

	////////////////////////////////////////////////////////////////////////////
	// Launcher Option Notice 
	if(g_bRunApproval == false) {
		MessageBox( NULL, "This Program is used to excute programs through Latale Launcher and does not need to be run.", "LaTale Client Approval", MB_OK|MB_ICONERROR );
		return -1;
	}

	// GameGuard Init
	if( SPGameGuard::GetInstance()->Init() == FALSE )
	{
		SPGameGuard::DelInstance();
		return -1;
	}

	//InitializeCriticalSection(&g_CriticalSection);	

	// For File Logs
	// g_bFileLog = true ;
	if( g_bFileLog)	
		ErrorFLog::GetInstance()->SetFileLog(g_bFileLog) ;
	ErrorFLogs->Error_Log("File Log [%d]", g_bFileLog) ;

	//////////////////////////////////////////////////////////////////////////
	//
	// Resource Init
	SPResourceBase::Create();

	// Change Init Method..refer to LataleVersionDef
	SPResourceBase::GetInstance()->ReserveInitFile(RESOURCE_FILES2, MAX_RESOURCE_FILE2);
	SPResourceBase::GetInstance()->Init(RES_WORK_RESOURCE_PATH);

	DWORD dwStyle;
	int iWidth, iHeight;
	int iPosX	=	CW_USEDEFAULT;
	int iPosY	=	CW_USEDEFAULT;

	HKL hkl = GetKeyboardLayout( 0 );
	int iLanguageType = PRIMARYLANGID(LOWORD(hkl));

	//Debug for China
	//g_eCountryLocale = CL_CHINA;
	
	//////////////////////////////////////////////////////////////////////////
	// For Sanda Administrator
	if(g_eCountryLocale == CL_CHINA) {
	//if( iLanguageType == LANG_CHINESE )	{
		g_bClipChild = true;
		//g_bWindowed = false;								// China Keyboard Full Screen
		GmWindowed();
		if( g_bFSDA == true )		g_bWindowed = true ;	// China Administrator Windowed	
	}
	else 
		if(g_eCountryLocale == CL_JAPAN || g_eCountryLocale == CL_KOREA) {
		//if( iLanguageType == LANG_JAPANESE )
			g_bClipChild = true;
		}

	ZeroMemory(&g_OrgDM, sizeof(DEVMODE));		
	EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &g_OrgDM);
	CopyMemory(&g_InGameDM, &g_OrgDM, sizeof(DEVMODE));

	//g_bWindowed = true;

	if( g_bWindowed )
	{
		dwStyle = WS_OVERLAPPEDWINDOW  | WS_CLIPCHILDREN;

		// AJJIYA [12/27/2005 AJJIYA]
		// Ω∫≈∏¿œø° µ˚∏• Ω«¡¶ ¿©µµøÅE≈©±ÅEªÅEÅEπ◊ ¿˚øÅE
		RECT rcRealWindows	=	{	0 , 0 , g_iScrWid , g_iScrHei };
		AdjustWindowRect( &rcRealWindows , dwStyle , false );

		iWidth	=	rcRealWindows.right		-	rcRealWindows.left;
		iHeight	=	rcRealWindows.bottom	-	rcRealWindows.top;

		// AJJIYA [12/27/2005 AJJIYA]
		// ¿©µµøÅE∏µÂ¿œ∞ÊøÅE∞°ø˚—• ¿ßƒ°∑Œ ¡§∑ƒ.
		int iFullScreenX = GetSystemMetrics( SM_CXSCREEN );	// ÅE˝ß
		int iFullScreenY = GetSystemMetrics( SM_CYSCREEN );	// ?

		if( iFullScreenX > g_iScrWid )
			iPosX	=	( iFullScreenX - iWidth )	/ 2;

		if( iFullScreenY > g_iScrHei )
			iPosY	=	( iFullScreenY - iHeight )	/ 2;
	}
	else
	{
		
		dwStyle = WS_POPUP | WS_SYSMENU | WS_CLIPCHILDREN;
		iWidth = g_iScrWid;
		iHeight = g_iScrHei;

		//china or japan		
		if(g_bClipChild) {
			
			//ZeroMemory(&g_OrgDM, sizeof(DEVMODE));		
			//EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &g_OrgDM);
			//CopyMemory(&g_InGameDM, &g_OrgDM, sizeof(DEVMODE));

			g_InGameDM.dmPelsWidth = g_iScrWid;
			g_InGameDM.dmPelsHeight = g_iScrHei;

			int result = ChangeDisplaySettings(&g_InGameDM, CDS_FULLSCREEN);
		}
	}





	// Create Window
	g_hWnd = CreateWindowEx	(
		0,	// dwExStyle
		CLASS_NAME,
		TITLE_NAME,
		dwStyle,
		iPosX,
		iPosY,
		iWidth, 
		iHeight, 
		GetDesktopWindow(),
		NULL,
		g_hInst,
		NULL
		);

	if( !IsWindow(g_hWnd) )
	{
		if(iLanguageType == LANG_KOREAN) {
			MessageBox( NULL, "¿©µµøÏª˝º∫¿Ã Ω«∆–µ«æ˙Ω¿¥œ¥Ÿ.", "Initialize Fail", MB_OK|MB_ICONERROR );
		} else {
			MessageBox( NULL, "Window Create Fail", "Initialize Fail", MB_OK|MB_ICONERROR );
		}		
		return -1;
	}

	// Sound Init
	if( SPSoundBase::Create() == NULL )
	{
		if(iLanguageType == LANG_KOREAN) {
			MessageBox( NULL, "ªÁøÓµÂ¿Âƒ° ª˝º∫¿Ã Ω«∆–µ«æ˙Ω¿¥œ¥Ÿ.", "Initialize Fail", MB_OK|MB_ICONERROR );
		} else {
			MessageBox( NULL, "SoundDevice create Fail", "Initialize Fail", MB_OK|MB_ICONERROR );
		}
		return -1;
	}

#ifdef _RES_PATH
	TCHAR szMilesPath[MAX_PATH] = {0,};
	_stprintf(szMilesPath,"%s%s",RES_WORK_RESOURCE_PATH,RES_MILES_REDIST_PATH);
	SPSoundBase::GetInstance()->Init(g_hWnd, szMilesPath);
#else
	SPSoundBase::GetInstance()->Init(g_hWnd, RES_MILES_REDIST_PATH);
#endif
	SPSoundBase::GetInstance()->SetSoundFocus( g_hWnd );

	// GameGuard CreateWindow [5/26/2008 AJJIYA]
	SPGameGuard::GetInstance()->AfterCreateWindow();
	
	g_pIngameWebBrowser = new SPIngameWebbrowser(g_hWnd, hInstance, CLASS_NAME);
	if(g_pIngameWebBrowser == NULL) {
		assert(0 && "IWB FAIL");
	}
	////‘ÿ»ÎIGW
	//if ( NULL == g_pCIGWLoaderDx9 )
	//{	
	//	char StrVersion[128];
	//	sprintf(StrVersion, "%d%d", VER_MAJOR, VER_MINOR);
	//	GameBasicInfoExA	GameInfo = { sizeof(GameBasicInfoExA), 39, "≤ ∫Áµ∫", StrVersion, 4, 1, NULL, NULL, -1, -1} ;
	//	g_pCIGWLoaderDx9 = new CIGWLoaderDx9( &GameInfo );
	//}

	ShowWindow( g_hWnd, SW_HIDE );
	UpdateWindow( g_hWnd );


#ifndef _DEBUG
	// force Focus
	DWORD dwFromId = GetCurrentThreadId(); 
	DWORD dwToId = GetWindowThreadProcessId(GetForegroundWindow(), NULL); 
	BOOL succeeded; 

	AttachThreadInput(dwFromId, dwToId, TRUE); 
	succeeded = SetForegroundWindow(g_hWnd); 
	AttachThreadInput(dwFromId, dwToId, FALSE);
#endif

	ShowWindow(g_hWnd, SW_RESTORE);

	//////////////////////////////////////////////////////////////////////////	
	// Device Initialize
	g_pVideo = new SPVideo;
	if( g_pVideo == NULL )
	{
		if(iLanguageType == LANG_KOREAN) {
			MessageBox( NULL, "∫Òµø¿¿Âƒ° ª˝º∫¿Ã Ω«∆–µ«æ˙Ω¿¥œ¥Ÿ. ∂Û≈◊¿œ¿∫ DirectX 9.0cøÕ ±◊øÕ »£»Øµ«¥¬ ±◊∑°«» ƒ´µÂ∞° « ø‰«’¥œ¥Ÿ.", "Initialize Fail", MB_OK|MB_ICONERROR );
		} else {
			MessageBox( NULL, "SPVideo Create Fail", "Initialize Fail", MB_OK|MB_ICONERROR );
		}
		return -1;
	}

	if(FAILED(g_pVideo->Init(g_hWnd, g_iScrWid, g_iScrHei, g_bWindowed, g_VideoQuality, false))) 
	//if(FAILED(g_pVideo->Init(g_hWnd, g_iScrWid, g_iScrHei, g_bWindowed, g_VideoQuality, g_bClipChild))) 
	{		
		delete g_pVideo;
		return 0;	
	}
	g_pVideo->SetClipChildMode(true);

	
	g_pVideo->SetIngameDM(&g_InGameDM);

	// Input, Audio Initialize
	g_pInput = new SPInput;
	if( g_pInput == NULL )
	{
		if(iLanguageType == LANG_KOREAN) {
			MessageBox( NULL, "¿‘∑¬ª˝º∫¿Ã Ω«∆–µ«æ˙Ω¿¥œ¥Ÿ. ∂Û≈◊¿œ¿∫ DirectX 9.0c∞° « ø‰«’¥œ¥Ÿ.", "Initialize Fail", MB_OK|MB_ICONERROR );
		} else {
			MessageBox( NULL, "SPInput Create Fail", "Initialize Fail", MB_OK|MB_ICONERROR );
		}
		return -1;
	}
	g_pInput->Init(g_hWnd);	


	// SDRS - Use for shanda
//#ifdef _INCLUDE_RSCLIENT_
	//g_eCountryLocale = CL_CHINA ;
	if( g_eCountryLocale == CL_CHINA )
	{
		SPRsManager::GetInstance();
		g_pkRsManager->Init() ;
		g_pkRsManager->SetNetParam(RS_DEF_SERVER_IP, RS_DEF_SERVER_PORT) ;
		g_pkRsManager->SetGameType(RS_DEF_GAMETYPE, RS_DEF_AREA) ;
		g_pkRsManager->SetIDType(RS_DEF_IDTYPE) ;
		g_pkRsManager->SetAccount(RS_DEF_USERID) ;
	}
//#endif
	

	// GameManager Initialize
	g_pMainGameManager = new SPMainGameManager;
	if(g_pMainGameManager == NULL) {
		if(iLanguageType == LANG_KOREAN) {
			MessageBox( NULL, "SPMainGameManager ª˝º∫ Ω«∆–µ«æ˙Ω¿¥œ¥Ÿ. √Ê∫–«— ∏ﬁ∏∏Æ∏¶ »Æ∫∏πŸ∂¯¥œ¥Ÿ.", "Initialize Fail", MB_OK|MB_ICONERROR );
		} else {
			MessageBox( NULL, "SPMainGameManager Create Fail", "Initialize Fail", MB_OK|MB_ICONERROR );
		}
		return -1;
	}
	g_pMainGameManager->InitAllocate() ;

	//////////////////////////////////////////////////////////////////////////
	// For Logo Thread
	// If don't want to use Initial-Logo, then set false "g_InitialThreadInfo.m_bDoingThread"
	g_InitialThreadInfo.m_bDoingThread = true ;		// Thread or Serial Load
	if( g_InitialThreadInfo.m_bDoingThread == true )
	{
		if( CreateLoadThread() == false )		{
			MessageBox( NULL, "Thread Load Create Fail", "Initialize Fail", MB_OK|MB_ICONERROR );
			return -1 ;
		}
	}
	else
	{
		g_pMainGameManager->Init();
		g_pMainGameManager->PostInit();
	}

	//[xialin 2008/11/05]≥ı ºªØΩ≈±æΩ”ø⁄
	CAutoInterface::GetInstance().Initial();


	//SetTimer(g_hWnd, BGTIMER, 10, NULL);

	ShowWindow( g_hWnd, SW_SHOW );
	UpdateWindow( g_hWnd );

	// GameGuard Setting
	SPGameGuard::GetInstance()->SetHwnd( g_hWnd );

	if(g_bClipChild && !g_bWindowed) {
		int result = ChangeDisplaySettings(&g_InGameDM, CDS_FULLSCREEN);
	}

	// ∞˙›§≈∞ , « ≈Õ≈∞ , ≈‰±€≈∞ ºº∆√
	// Save the current sticky/toggle/filter key settings so they can be restored them later
	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
	SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
	SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);

	// Disable when full screen
	AllowAccessibilityShortcutKeys( false );

	// End of Initialize
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	// Main Loop Poll
	BOOL bGotMsg;
	MSG  msg;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while( WM_QUIT != msg.message )
	{		
		bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

		if( bGotMsg )
		{	
			if( IsWindow(g_hWnd) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			if(Run() == false) //œ˚œ¢µƒ∑÷∑¢”Î¥¶¿Ì
				SendMessage( g_hWnd, WM_CLOSE, 0, 0 );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	
	// Restore back when going to windowed or shutting down
	AllowAccessibilityShortcutKeys( true );

	//SAFE_DELETE( g_pSDRSClient );	

	
	SAFE_DELETE( g_pIngameWebBrowser );	

	SAFE_DELETE( g_pMainGameManager );
	SAFE_DELETE( g_pInput );
	SAFE_DELETE( g_pVideo );

//#ifdef _INCLUDE_RSCLIENT_
	if( g_eCountryLocale == CL_CHINA )
		SPRsManager::GetInstance()->Release() ;
//#endif
	SPSoundBase::GetInstance()->Release();
	SPResourceBase::GetInstance()->Release();
	SPGameGuard::DelInstance();
	ErrorFLog::Release() ;	
	
	//»∑±£IGW–∂‘ÿ
	if ( g_pCIGWLoaderDx9 )
	{ 		
		delete g_pCIGWLoaderDx9;
		g_pCIGWLoaderDx9 = NULL;
	}
	//DeleteCriticalSection(&g_CriticalSection);

	return (int)msg.wParam;
	
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//‘⁄IGW…œ ±,»√IGWœ»¥¶¿Ìœ˚œ¢
	LRESULT lResult;
	if( g_pCIGWLoaderDx9 )
	{
		if ( g_pCIGWLoaderDx9->OnWindowProcEx( hWnd, message, wParam, lParam, &lResult ) == S_OK )
		{
			return lResult;
		}
	}
	// Main manager
	if(g_pMainGameManager)
		if(g_pMainGameManager->WndProc(message, wParam, lParam)) return 0;

	if ( WM_CLOSEIGWMSG == message )
	{
		if ( g_pCIGWLoaderDx9 )
		{
			delete g_pCIGWLoaderDx9;
			g_pCIGWLoaderDx9 = NULL;
			if ( ( 1 == wParam ) && ( 1 == lParam ) )
			{
				PostMessage( hWnd, WM_CLOSE, 0, 0 );
			}
			return 0L;
		}
	}

	if ( WM_CLOSE == message ) // ¿πœ¬∏√œ˚œ¢ πœ»’˝»∑ Õ∑≈IGW∫Û≤≈»√÷˜¥∞ÃÂ Õ∑≈
	{
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->FinalizeGraphic();
			g_pCIGWLoaderDx9->Terminal();// πÿ±’IE◊Èº˛ ±ª·≤˙…˙–Ì∂‡–Ë“™¥¶¿Ìµƒœ˚œ¢£¨“Ú¥À‘⁄œ¬“ª∏ˆœ˚œ¢—≠ª∑‘Ÿ Õ∑≈GameWidget.dll‘Ú√ª”–Œ Ã‚
			// œ¬“ª∏ˆœ˚œ¢—≠ª∑‘Ÿ Õ∑≈æÕ√ªŒ Ã‚¡À
			PostMessage( hWnd, WM_CLOSEIGWMSG, 1, 1 );
			return 0L;
		}		
	}

	switch (message) 
	{
	case WM_ACTIVATEAPP:
		g_bActive   = (BOOL)wParam;
		g_bHasFocus = (BOOL)wParam;
		break;

	case WM_CLOSE:		
		DestroyWindow( hWnd );
		WinExec( "LataleAdv.exe", SW_SHOWNORMAL );
		return 0L;

	case WM_DESTROY:
		{
			if(g_bClipChild && !g_bWindowed)
			{
				//HWND hTBWnd = ::FindWindow("Shell_TrayWnd",NULL);
				//::ShowWindow(hTBWnd, SW_SHOW); 
			}
			PostQuitMessage(0);
		}
		return 0L;

	case WM_KEYDOWN:
		//if( wParam == VK_ESCAPE)
		//	DestroyWindow( hWnd );
		//return 0L;
		break;

	case WM_TIMER:
		if(wParam == BGTIMER) {					
			if(Run() == false) {
				SendMessage( g_hWnd, WM_CLOSE, 0, 0 );
			} else {
				Sleep(5);
			}			
		}
		break;

	case WM_ENTERIDLE: //reduce delay from child window
		{			
			SendMessage( g_hWnd, WM_CANCELMODE, 0, 0 );
			return 0;
		}
		break;



	case WM_ACTIVATE:
		{
			if( wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE ) {
				NotifyActivate( true );
				DXUTOutputDebugString( "WM_ACTIVATE true\n");
			} else if( wParam == WA_INACTIVE ) {
				NotifyActivate( false );
				DXUTOutputDebugString( "WM_ACTIVATE false\n");
			}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;

	default:		
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


bool NotifyActivate(bool bActivate)
{
	s_bNotifyActivated = bActivate;
	if(s_bNotifyActivated) {		
		SPSoundBase::GetInstance()->EnableSystem(true);		

		///test///////////////////////////////////////////////////////////////////////
		if(g_bClipChild && !g_bWindowed) {
			// Taskbar Hide
			//HWND hTBWnd = ::FindWindow("Shell_TrayWnd",NULL);
			//::ShowWindow(hTBWnd, SW_HIDE); 

			ChangeDisplaySettings(&g_InGameDM, CDS_FULLSCREEN);
			ShowWindow(g_hWnd, SW_RESTORE);
		}		

		if(g_pVideo) 
			g_pVideo->Active();

		if(g_pInputManager)
			g_pInputManager->Active();

	} else {
		
		if( !g_InitialThreadInfo.m_bDoingThread) { //  Thread ªÁøÎæÀãö
			if(g_pInputManager)
				g_pInputManager->Deactive();
		}

		if(g_pVideo) 
			g_pVideo->Deactive();

		if( !g_InitialThreadInfo.m_bDoingThread) { //  Thread ªÁøÎæÀãö

			if(g_bClipChild && !g_bWindowed) {
				// Taskbar Hide
				//HWND hTBWnd = ::FindWindow("Shell_TrayWnd",NULL);
				//::ShowWindow(hTBWnd, SW_SHOW); 
				

				//test////////////////////////////////////////////////////////////////////////
				if(g_eCountryLocale == CL_JAPAN) {
					if( g_pIngameWebBrowser && !g_pIngameWebBrowser->IsNotDisplayChange() )
					{
						ShowWindow(g_hWnd, SW_MINIMIZE);						
						ChangeDisplaySettings(&g_OrgDM, 0);
					
					}
				} else if(g_eCountryLocale == CL_CHINA) {
					// china
					ShowWindow(g_hWnd, SW_MINIMIZE);
					ChangeDisplaySettings(&g_OrgDM, 0);
				} else if(g_eCountryLocale == CL_KOREA) {
					if(!(g_pIngameWebBrowser && g_pIngameWebBrowser->IsNavigate())) {
						ShowWindow(g_hWnd, SW_MINIMIZE);
						ChangeDisplaySettings(&g_OrgDM, 0);
					}
				}					
			}
		}

		SPSoundBase::GetInstance()->EnableSystem(false);
	}
	return s_bNotifyActivated;
}

bool Run()
{
	//if(s_bNotifyActivated == false) {
	//	Sleep(50);
	//}
	if( g_InitialThreadInfo.m_bDoingThread )
	{
		if( g_InitialThreadInfo.m_bFinish )
		{
			g_InitialThreadInfo.m_bDoingThread = false ;
			g_pMainGameManager->PostInit() ;
			Sleep(100) ;
			g_pMainGameManager->DestroyLogoDisplay() ;
		}
		else
		{
			g_pMainGameManager->ProcessLogoDisplay() ;
		}
	}
	else
	{
		if(g_pMainGameManager)
			return g_pMainGameManager->Process();
	}

	return true;
}


//bool CreateLogoThread()
//{
//	g_InitialThreadInfo.m_bExit = false ;
//
//	SPLogoThread* pLogoThread = new SPLogoThread ;
//	pLogoThread->InitLogo() ;
//	unsigned threadID;
//	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadLogo, pLogoThread, 0, &threadID);
//
//	try 
//	{
//		g_pMainGameManager->Init();
//		g_InitialThreadInfo.m_bExit = true ;
//		Sleep(100) ;
//		g_pMainGameManager->PostInit() ;
//	}
//	catch (...) 
//	{
//		DWORD dwErrCode = GetLastError() ;
//		ErrorFLogs->Error_Log("°⁄°⁄ Catch MainGameManager Init ERROR!!![%d]", dwErrCode);
//
//		DWORD dwExitCode;
//		if (GetExitCodeThread(hThread, &dwExitCode) == FALSE) 
//			ErrorFLogs->Error_Log("GetExitCodeThread Error Code: %d\n", GetLastError());
//		if( dwExitCode == STILL_ACTIVE)
//		{
//			// Thread force terminate
//			if (TerminateThread(hThread, dwExitCode) == FALSE)
//				ErrorFLogs->Error_Log("TerminateThread Error Code: %d\n", GetLastError());
//			// Check Thread Terminate
//			if (WaitForSingleObject(hThread, INFINITE) == WAIT_FAILED)
//				ErrorFLogs->Error_Log("WaitForSingleObject Error Code: %d\n", GetLastError());
//			Sleep(5) ;
//		}
//		return false ;
//	}
//	g_InitialThreadInfo.m_bUsingThread = false ;
//	pLogoThread->ClearLogo() ;
//	SAFE_DELETE(pLogoThread) ;
//	return true ;
//}


bool CreateLoadThread()
{
	g_InitialThreadInfo.m_bExit = false ;
	g_pMainGameManager->InitLogoDisplay() ;
	unsigned threadID;
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadLoad, g_pMainGameManager, 0, &threadID);

	if( hThread == NULL )	return false ;
	else					return true ;
}

unsigned __stdcall ThreadLoad( LPVOID pParam )
{
	SPMainGameManager* pLoadThread = NULL ;
	pLoadThread = (SPMainGameManager*) pParam ;

//////////////////////////////////////////////////////////////////////////
	// Exception Handling « ø‰«‘.
	//pLoadThread->Init() ;
	g_pMainGameManager->Init() ;
//////////////////////////////////////////////////////////////////////////

	ErrorFLogs->Error_Log("##### LOAD Thread End") ;

	_endthreadex( 0 );
	return 0 ;
}
//////////////////////////////////////////////////////////////////////////
//
//	∞˙›§≈∞ , ≈‰±€≈∞ , « ≈Õ≈∞ ºº∆√
//

void AllowAccessibilityShortcutKeys( bool bAllowKeys )
{
	if( bAllowKeys )
	{
		// Restore StickyKeys/etc to original state and enable Windows key      
		STICKYKEYS sk = g_StartupStickyKeys;
		TOGGLEKEYS tk = g_StartupToggleKeys;
		FILTERKEYS fk = g_StartupFilterKeys;

		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
		SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
		SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);
	}
	else
	{
		// Disable StickyKeys/etc shortcuts but if the accessibility feature is on, 
		// then leave the settings alone as its probably being usefully used

		STICKYKEYS skOff = g_StartupStickyKeys;
		if( (skOff.dwFlags & SKF_STICKYKEYSON) == 0 )
		{
			// Disable the hotkey and the confirmation
			skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
			skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
		}

		TOGGLEKEYS tkOff = g_StartupToggleKeys;
		if( (tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0 )
		{
			// Disable the hotkey and the confirmation
			tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
			tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
		}

		FILTERKEYS fkOff = g_StartupFilterKeys;
		if( (fkOff.dwFlags & FKF_FILTERKEYSON) == 0 )
		{
			// Disable the hotkey and the confirmation
			fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
			fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
		}
	}
}

int GmWindowed()
{
	HINSTANCE hinst;
	hinst = ::LoadLibrary("GmOutPut.dll");

	if( NULL == hinst )
	{
		return 1;
	}
	GmWindow beWindow = (GmWindow)::GetProcAddress( hinst, "ToBeWindows" );	
	if( NULL == beWindow )
	{	
		::FreeLibrary( hinst );
		return 1;
	}
	if ( 1986515 == beWindow() )
	{
		g_bFSDA = true;
	}
	::FreeLibrary( hinst );
	return 1;
}
