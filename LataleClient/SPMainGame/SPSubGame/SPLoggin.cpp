
#include "SPCommon.h"
#include "SPUtil.h"
#include "SPTimer.h"
#include "LT_Error.h"
#include "SPResourceDEF.h"
#include "SPMainGameDEF.H"

#include "SPMouseCursor.h"
#include "SPManager.h"
#include "SPGOBManager.h"
#include "SPIMEPool.h"
#include "SPSubGameManager.h"
#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPInputManager.h"

#include "NPGameGuard/SPGameGuard.h"

#include <shellapi.h>
#include "SPLoggin.h"
#include "SPTerrainDef.h"

#include "SPResourceManager.h"
#include "SPAbuseFilter.h"

#include "LataleVersionDef.h"
#include "SPMainGameManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPWindow.h"
#include "SPWindowLogin.h"
#include "SPInterfaceManager.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPNoticeBox.h"

#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"

#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"
#include "_Error_log.h"

#include "IGWInterface.h"
#include "IGWLoaderDx9.h"

char g_szWebArg[1024] = {""};
static bool g_bIsFirstLogin = true ;
extern char g_szLoginServerIP[];
extern int g_iLoginServerPort;

SPLoggin::SPLoggin()
: SPSubGameManager(SUBGAME_LOGGIN)
, m_pInterface (NULL)
, m_pLogInImage (NULL)	
, m_pLogInImage2(NULL)
, m_bWebConnectTry(false)
, m_pkFXArchive(NULL)
, m_pkFXArchive2(NULL)
, m_pEkeyTimer(NULL)
, m_pLoginTimer(NULL)
{
}

SPLoggin::~SPLoggin() 
{	
	SAFE_RELEASE(m_pInterface);			// Age Mark
	SAFE_RELEASE(m_pLogInImage);		// Back
	SAFE_RELEASE(m_pLogInImage2);		// Char
	SAFE_DELETE(m_pkFXArchive);			// Fx
	SAFE_DELETE(m_pkFXArchive2);		// Fx

	SAFE_DELETE(m_pEkeyTimer) ;
	SAFE_DELETE(m_pLoginTimer) ;
}



bool SPLoggin::Init()
{	

	m_bReserveExit = false ;
	m_bConnectTimeOut = false ;
	m_iShowState = SHOW_LOGIN_DLG ;

	m_strWebID.clear() ;
	m_strWebKey.clear() ;
	//strcpy( m_szWebID, "");
	//strcpy( m_szWebKey, "");

	OnSetBGM( (WPARAM) "Data/BGM/TitleBGM.mp3", 0);	
	m_pEkeyTimer = new SPTimer ;
	m_pEkeyTimer->Clear() ;
	m_pLoginTimer = new SPTimer ;
	m_pLoginTimer->Clear() ;
	
	//////////////////////////////////////////////////////////////////////////
	// 신 버전 Resource
	//g_pVideo->GetTextureMgr()->LoadTexture( "DATA/LOGGIN/LOGIN001.PNG", &m_pInterface);
	g_pVideo->GetTextureMgr()->GetTexture( "DATA/LOGGIN/LOGIN001.PNG", &m_pInterface,TEX_INTERFACE,LEVEL_NOW);

	//g_pVideo->GetTextureMgr()->LoadTexture( "DATA/LOGGIN/LOGIN003.PNG", &m_pLogInImage);
	g_pVideo->GetTextureMgr()->GetTexture( "DATA/LOGGIN/LOGIN003.PNG", &m_pLogInImage,TEX_INTERFACE,LEVEL_NOW);
	
	//g_pVideo->GetTextureMgr()->LoadTexture( "DATA/LOGGIN/LOGIN002.PNG", &m_pLogInImage2);
	g_pVideo->GetTextureMgr()->GetTexture( "DATA/LOGGIN/LOGIN002.PNG", &m_pLogInImage2,TEX_INTERFACE,LEVEL_NOW);

	char StrVersion[128];
	sprintf(StrVersion, "Build Number %d%d", VER_MAJOR, VER_MINOR);
	m_strVersion = StrVersion;

	//////////////////////////////////////////////////////////////////////////
	// BackGround
	SetRect(&m_rtSrc1, 0, 0, 512, 512);
	SetRect(&m_rtSrc2, 512, 0, 800, 512);
	SetRect(&m_rtSrc3, 800, 0, 1024, 88);
	SetRect(&m_rtSrc4, 800, 88, 1024, 176);
	SetRect(&m_rtSrc5, 800, 176, 1024, 264);
	SetRect(&m_rtSrc6, 800, 264, 928, 352);

	SetRect(&m_rtDest1, 0, 0, 512, 512);
	SetRect(&m_rtDest2, 512, 0, 800, 512);
	SetRect(&m_rtDest3, 0, 512, 224, 600);
	SetRect(&m_rtDest4, 224, 512, 448, 600);
	SetRect(&m_rtDest5, 448, 512, 672, 600);
	SetRect(&m_rtDest6, 672, 512, 800, 600);
	
	// Version Text
	SetRect(&m_rtVer, 508, 83, 508+120, 83+16);

	// Age Mark
	//SetRect(&m_rtSrcAgeMark, 91, 411, 91+122, 411+53);
	//SetRect(&m_rtDestAgeMark, 657, 25, 657+112, 25+53);
	SetRect(&m_rtSrcAgeMark, 391, 8, 512, 149);
	SetRect(&m_rtDestAgeMark, 677, 2, 677+121, 2+141);
	

	m_bWebConnectTry = false ;

	m_pkFXArchive = new SPFXArchive(NULL);		///<FX
	m_pkFXArchive2 = new SPFXArchive(NULL);		///<FX
	m_pkFXArchive->InsertFXGroup((SPID_FX_GROUP)15062);		// Back
	m_pkFXArchive2->InsertFXGroup((SPID_FX_GROUP)15061);	// Frount

	m_fAccumulateWebLoginSend	=	0.0f;

	g_pInterfaceManager->SetIMEModeEnglish(true, false);
	bool bRet = SPSubGameManager::Init();

#ifndef _DEBUG
	//////////////////////////////////////////////////////////////////////////
	// BUDDYBUDDY && !WEBARG ==> Exit
	if( SPLocalizeManager::GetInstance()->OnlyWebLogin() && strlen(g_szWebArg) < 1 )
	{
		if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_BUDDYBUDDY )
			ErrorFLogs->Error_Log("BuddyBuddy Login WEBARG Fail!!!") ;
		if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_ATGAMES )
			ErrorFLogs->Error_Log("SITE_ATGAMES Login WEBARG Fail!!!") ;
		if( g_bIsFirstLogin == true)	ShowHomePage(SPLocalizeManager::GetInstance()->GetHomePage()) ;		// Show HomePage
		g_pMainGameManager->SetExit() ;		// Not Reserve, Immediately Exit
		return false ;
	}
#endif
	g_bIsFirstLogin = false ;

	return bRet ;
}




bool SPLoggin::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//return SPSubGameManager::WndProc(message, wParam, lParam);
	return (g_pInterfaceManager->WndProc(message, wParam, lParam) == -1) ? true : false;  
}



void SPLoggin::Process(float fTime)
{
	switch(m_iGameState) {
	case GS_GAME:
		{	
			g_pInputManager->Process(fTime);
			g_pInterfaceManager->Process(fTime);
			g_pNetworkManager->Process(fTime);

			// Login Request TimeOut
			if( m_pLoginTimer->IsEnable() )
			{
				if( m_pLoginTimer->CheckTimer(fTime) == true )
				{
					g_pNetworkManager->Disconnect2();
					OnSocketFailed(0, 0);
					m_pLoginTimer->Clear() ;
					m_bConnectTimeOut = true ;
					if( g_pResourceManager->GetGlobalString(80014) == NULL )
						ShowNoticeBox(NOTICE_TYPE_YES, "Connect TimeOut!") ;	//> connect Fail
					else
						ShowNoticeBox(NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(80014)) ;	//> connect Fail
					// if Buddy Login TimeOut -> Exit Program
					if( SPLocalizeManager::GetInstance()->OnlyWebLogin() ) {
						ReserveExit() ;
					}
				}
			}
			if( m_pEkeyTimer->IsEnable() )
			{
				if( m_pEkeyTimer->CheckTimer(fTime) == true ) {
					m_pEkeyTimer->Clear() ;
					m_bConnectTimeOut = true ;
					g_pNetworkManager->Disconnect2();
					if( g_pResourceManager->GetGlobalString(80014) == NULL )
						ShowNoticeBox(NOTICE_TYPE_YES, "Connect TimeOut!") ;	//> connect Fail
					else
						ShowNoticeBox(NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(80014)) ;	//> connect Fail
				}
			}
		}
		break;
	case GS_RELEASE_READY:
		{
			m_fWaitTime -= fTime;
			if( m_fWaitTime <= 0 )
			{
				ForceCloseNotice() ;
				m_iGameState = GS_RELEASE;
				m_NextSubGameID	= SUBGAME_LOBBY;
				IGWLogin();
			} else {
				g_pInputManager->Process(fTime);
				g_pInterfaceManager->Process(fTime);
				g_pNetworkManager->Process(fTime);
			}
		}
		break;
	}

	if( m_pkFXArchive != NULL )
		m_pkFXArchive->Process( fTime );
	if( m_pkFXArchive2 != NULL )
		m_pkFXArchive2->Process( fTime );

	SPSubGameManager::Process(fTime);

	//////////////////////////////////////////////////////////////////////////
	//
	//	WebLogin
	//
	m_fAccumulateWebLoginSend	+=	fTime;
	if( m_fAccumulateWebLoginSend > 1.0f && m_pLoginTimer->IsEnable() == false && IsWebLogin() == true && m_bWebConnectTry == false )
	{
		// 여기서 WebLogin을 위한 Parsing을 한다.
		if( ParseWebArgs_New() == true )
		{
			// Actoz ( Key, Time )
			// BuddyBuddy ( ID, Key )
			// ATGames ( ID, Password )
			m_iShowState = SHOW_NON_DLG ;
			OnReqConnectTry( (WPARAM)m_strWebID.c_str(), (LPARAM)m_strWebKey.c_str() );
		}
		else
		{
			// 전달받은 인자가 잘못되었다면 종료
			PrintLogs("[SPLoggin::Process] WEB Login Parse Fail=[%s]\n", g_szWebArg) ;
			ShowNoticeBox(NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(90001)) ;		// SecureKey Error
			ReserveExit();
		}
		
		m_fAccumulateWebLoginSend	=	0.0f;
	}
}

void SPLoggin::Render(float fTime)
{
	if( g_pVideo == NULL ) 
		return;

	g_pVideo->Flush();
	
	////////////////////////////////////////////////////////////////////////
	//신 버전
	// 1. Layer Image
	if(m_pLogInImage) {
		m_pLogInImage->RenderTexture(&m_rtDest1, &m_rtSrc1);
		m_pLogInImage->RenderTexture(&m_rtDest2, &m_rtSrc2);
		m_pLogInImage->RenderTexture(&m_rtDest3, &m_rtSrc3);
		m_pLogInImage->RenderTexture(&m_rtDest4, &m_rtSrc4);
		m_pLogInImage->RenderTexture(&m_rtDest5, &m_rtSrc5);
		m_pLogInImage->RenderTexture(&m_rtDest6, &m_rtSrc6);
	}
	g_pVideo->Flush();

	// 2. Layer FX
	if( m_pkFXArchive != NULL )
		m_pkFXArchive->Render( fTime );

	// 3. Layer Image
	if(m_pLogInImage2) {
		m_pLogInImage2->RenderTexture(&m_rtDest1, &m_rtSrc1);
		m_pLogInImage2->RenderTexture(&m_rtDest2, &m_rtSrc2);
		m_pLogInImage2->RenderTexture(&m_rtDest3, &m_rtSrc3);
		m_pLogInImage2->RenderTexture(&m_rtDest4, &m_rtSrc4);
		m_pLogInImage2->RenderTexture(&m_rtDest5, &m_rtSrc5);
		m_pLogInImage2->RenderTexture(&m_rtDest6, &m_rtSrc6);
	}
	g_pVideo->Flush();

	// 4. Layer FX
	if( m_pkFXArchive2 != NULL )
		m_pkFXArchive2->Render( fTime );

	// AgeMark
	if(m_pInterface) {
		m_pInterface->RenderTexture(&m_rtDestAgeMark, &m_rtSrcAgeMark);
	}
	// Version
	if( g_pVideo->GetFont(FONT_12_NORMAL) != NULL )
		g_pVideo->GetFont(FONT_12_NORMAL)->RenderText( m_strVersion.c_str(), &m_rtVer, DT_TOP | DT_LEFT | DT_NOCLIP);	
	

	g_pInterfaceManager->Render(fTime);
	
	SPSubGameManager::Render(fTime);
}




void SPLoggin::RenderText()
{
}



int SPLoggin::SPPerformMessage	(UINT msg, WPARAM wparam, LPARAM lParam)
{
	if(SPGM_SUG_LOGGIN_BEGIN <= msg && SPGM_SUG_LOGGIN_END > msg )
		return SPGMESSAGE_FUNC((SPGM)msg, wparam, lParam);

	return SPSubGameManager::SPPerformMessage(msg, wparam, lParam);
}


/**
	Web-Login인지 여부 파악
*/
bool SPLoggin::IsWebLogin()
{
	bool	bResult	=	false;

	// Actoz WebLogin의 경우 두개의 인자를 받는다.
	if( g_szWebArg != NULL )
	{
		if( strlen( g_szWebArg ) > 0 )
			bResult = true;
	}
	// BuddyBuddy은 WebLogin만 가능하다.
	if( SPLocalizeManager::GetInstance()->OnlyWebLogin() )
	{
		bResult = true ;
	}

	return bResult;
}


/**
	Web-Login 시 인자값의 Parsing & Login 호출
*/
//bool SPLoggin::ParseWebArgs()
//{
//	if( g_szWebArg == NULL || strlen( g_szWebArg ) < 1 )
//		return false ;
//
//	int iNOArg;
//	std::vector<std::string> vstrArg;
//	std::vector<std::string> vstrSubArg;
//	std::string strWebID ;
//	std::string strIDParts ;
//
//	if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_ACTOZ )
//	{
//		// [Time]`[Key]
//		if( (iNOArg = GetSeperateString(g_szWebArg, vstrArg, '`') ) < 2 )
//			return false ;
//		m_strWebID	= vstrArg[1] ;		// Key
//		m_strWebKey = vstrArg[0] ;		// time
//		PrintLogs("===> Actoz WebARG[%d]:[%s][%s]", iNOArg, m_strWebID.c_str(), m_strWebKey.c_str());
//	}
//	else if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_BUDDYBUDDY )
//	{
//		// [0~0~8][ID]`[AuthData]
//		if( (iNOArg = GetSeperateString(g_szWebArg, vstrArg, '`') ) < 2 )
//			return false ;
//
//		m_strWebKey	= vstrArg[1] ;		// Key
//		
//		strIDParts = vstrArg[0].c_str() ;
//		
//		int		iIter;
//		int		iIter2 = -1;	
//		int		iNOArg = 0;
//		do {
//			iIter = iIter2;
//			iIter2 = strIDParts.find('~', iIter + 1);
//			++iNOArg;
//		}while(iIter2 != -1 && iNOArg < 3) ;
//
//		if( iNOArg < 3 )		return false ;
//
//		m_strWebID = strIDParts.substr( iIter2 + 1, std::string.npos );
//		PrintLogs("===> BUDDYBUDDY WebARG[%d]:[%s][%s]", iNOArg, m_strWebID.c_str(), m_strWebKey.c_str());
//	}
//	else if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_ATGAMES )
//	{
//		// [Time]`[Key]
//		if( (iNOArg = GetSeperateString(g_szWebArg, vstrArg, '`') ) < 2 )
//			return false ;
//		m_strWebID	= vstrArg[0] ;		// ID
//		m_strWebKey = vstrArg[1] ;		// PASSWORD
//		PrintLogs("===> ATGames WebARG[%d]:[%s][%s]", iNOArg, m_strWebID.c_str(), m_strWebKey.c_str());
//	}
//	return true ;
//}


/**
	Web-Login 시 인자값의 Parsing & Login 호출
*/
bool SPLoggin::ParseWebArgs_New()
{
	if( g_szWebArg == NULL || strlen( g_szWebArg ) < 1 )
		return false ;

	int iNOArg = 0 ;
	std::vector<std::string> vstrArg;
	std::vector<std::string> vstrSubArg;

	if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_ACTOZ )
	{
		// [Time]`[Key]
		if( (iNOArg = GetSeperateString(g_szWebArg, vstrArg, '`') ) < 2 )
			return false ;
		m_strWebID	= vstrArg[1] ;		// Key
		m_strWebKey = vstrArg[0] ;		// time
		PrintLogs("===> Actoz WebARG[%d]:[%s][%s]", iNOArg, m_strWebID.c_str(), m_strWebKey.c_str());
	}
	else if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_BUDDYBUDDY )
	{
		int iIter = -1 ;
		std::string strWebArgs ;

		// [Key]`[ID]
		strWebArgs = g_szWebArg ;
		iIter = strWebArgs.find("`", 0);
		if( iIter == -1 )
		{
			PrintLogs("BUDDYBUDDY WebARG ERROR [%s]", g_szWebArg) ;
			return false ;
		}
		else
		{
			m_strWebKey = strWebArgs.substr( 0, iIter ) ;
			m_strWebID = strWebArgs.substr( iIter + 1, std::string.npos );
			PrintLogs("===> BUDDYBUDDY WebARG:[%s][%s]", m_strWebID.c_str(), m_strWebKey.c_str());
			PrintLogs("===> BUDDYBUDDY IP[%s], Port[%d]", g_szLoginServerIP, g_iLoginServerPort);
		}
	}
	else if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_ATGAMES )
	{
		// [ID] [Password] > Seperater is SPACE
		if( (iNOArg = GetSeperateString(g_szWebArg, vstrArg, '`') ) < 2 )
			return false ;
		m_strWebID	= vstrArg[0] ;		// ID
		m_strWebKey = vstrArg[1] ;		// PASSWORD
		PrintLogs("===> ATGames WebARG[%d]:[%s][%s]", iNOArg, m_strWebID.c_str(), m_strWebKey.c_str());
	}
	return true ;
}


/**
	Notice Box를 출력한다.
*/
bool SPLoggin::ShowNoticeBox(NOTICE_TYPE iType, const TCHAR* szMsg)
{
	g_pInterfaceManager->ForceReply() ;

	if( szMsg == NULL )	{
		PrintLogs("===> ShowNoticeBox ERROR!!![Message is NULL]") ;
		return false ;
	}
	//PrintLogs("ShowNoticeBox [%d][%s]", iType, szMsg) ;
	switch(iType)
	{
	case NOTICE_TYPE_MSG:
		return g_pInterfaceManager->SetNotice(szMsg, g_pInterfaceManager->FindWindow( WIID_LOGIN ), NOTICE_TYPE_MSG) ;
		break ;
	case NOTICE_TYPE_YES:
		return g_pInterfaceManager->SetNotice(szMsg, g_pInterfaceManager->FindWindow( WIID_LOGIN ), NOTICE_TYPE_YES) ;
		break ;
	}
	return false ;
}

void SPLoggin::ForceCloseNotice()
{
	PrintLogs("---> Force Close Notice") ;
	g_pInterfaceManager->ForceReply() ;
}


/**
	해당 URL의 Explore를 호출한다.
*/
void SPLoggin::ShowHomePage(const TCHAR* szURL) 
{
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
	sei.cbSize = sizeof(SHELLEXECUTEINFO);

	sei.lpFile = __TEXT("explorer");
	sei.nShow = SW_SHOWMAXIMIZED;

	if( strlen(szURL) < 1 )
		sei.lpParameters = SPLocalizeManager::GetInstance()->GetHomePage();
	else
		sei.lpParameters = szURL ;

	if( strlen(sei.lpParameters) == 0 )
		return ;	

	sei.lpVerb = __TEXT("open");
	ShellExecuteEx(&sei);
}



void SPLoggin::ReserveExit() 
{
	m_bReserveExit = true ;
}


//////////////////////////////////////////////////////////////////////////
//
SPGMESSAGE_MAP_BEGIN(SPLoggin)
	// NetworkManager	
	SPGMESSAGE_COMMAND( SPGM_AUTH_SC_LOGIN			, OnResultAuthLogin		)		// Auth Result
	SPGMESSAGE_COMMAND( SPGM_AUTH_SC_EKEY			, OnResultAuthEkey		)		// Auth Result Request E-Key
	SPGMESSAGE_COMMAND( SPGM_AUTH_SC_EKEY_SN		, OnResultAuthSerial	)		// Auth Result Request Serial Key
	SPGMESSAGE_COMMAND( SPGM_AUTH_SC_EKEY_RES		, OnResultEkeyResult	)		// E-key Result 
	
	SPGMESSAGE_COMMAND( SPGM_REQ_CONNCET_TRY		, OnReqConnectTry		)		// ID,Pass Auth
	SPGMESSAGE_COMMAND( SPGM_REQ_EKEY_CONNCET_TRY	, OnReqEkeyConnectTry	)		// Ekey Auth
	SPGMESSAGE_COMMAND( SPGM_REQ_SERIAL_CONNCET_TRY	, OnReqSerialConnectTry	)		// Serial Auth
	SPGMESSAGE_COMMAND( SPGM_SUG_SOCKET_FAILED		, OnSocketFailed		)

	// Other
	SPGMESSAGE_COMMAND( SPGM_CHECK_WEBLOGIN			, OnCheckWebLogin		)
	SPGMESSAGE_COMMAND( SPGM_CHECK_RESERVEEXIT		, OnCheckReserveExit	)
	SPGMESSAGE_COMMAND( SPGM_CHECK_TIMEOUT			, OnCheckTimeOut		)

	SPGMESSAGE_COMMAND( SPGM_SOFTKEY_PRESS			, OnSoftKeyPress		)		// SoftKey Event
	SPGMESSAGE_COMMAND( SPGM_SOFTKEY_PRESSBACK		, OnSoftKeyPressBack	)
	SPGMESSAGE_COMMAND( SPGM_SOFTKEY_PRESSENTER		, OnSoftKeyPressEnter	)
	SPGMESSAGE_COMMAND( SPGM_SOFTKEY_PRESSCLOSE		, OnSoftKeyPressClose	)
	

SPGMESSAGE_MAP_END(SPSubGameManager)

/**
	Receive Login Result
	@param WPARAM : Result Code
*/
int SPLoggin::OnResultAuthLogin(WPARAM wparam, LPARAM lparam)
{
	RESULTCODE retCode = (RESULTCODE)wparam;
	m_pEkeyTimer->Clear() ;
	m_bConnectTimeOut = false ;

	//////////////////////////////////////////////////////////////////////////
	//
	//	웹로그인시 에러코드와 상관없이 두번 이상의 접속을 못하게 하고
	//	기존의 아이디 패스워드 창을 보여주게 한다.
	//
	//SPWindow*	pWindow	=	g_pInterfaceManager->FindWindow( WIID_LOGIN );
	SPWindowLogin*	pLWindow	=	(SPWindowLogin*)g_pInterfaceManager->FindWindow( WIID_LOGIN );

	if( pLWindow != NULL && retCode != LT_NO_ERROR )
	{
		if( SPLocalizeManager::GetInstance()->OnlyWebLogin() )	{
			ReserveExit() ;
		}
		else	{
			pLWindow->Show();		// For Reconnect
		}
	}
	m_iShowState = SHOW_LOGIN_DLG ;

	m_LogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	PrintLogs("OnResultAuthLogin: RetCode[%d]", retCode) ;

	const char* pszMsg = NULL;

	switch(retCode)
	{
	case LT_NO_ERROR:
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHAT_MACRO , SPIM_REFRESH , NULL , NULL );
		g_pNetworkManager->UserInRequest();
		return 0;

	case LT_ERROR_DUPLICATED:
		pszMsg = g_pResourceManager->GetGlobalString(30003);
		break;

	//case LT_ERROR_LEN_PW	:
	case LT_ERROR_PWMISMATCH:
		pszMsg = g_pResourceManager->GetGlobalString(30002);		///> Incorrect Password
		break;

	case LT_ERROR_OSP_CANNOT_CONNECT:
		pszMsg = g_pResourceManager->GetGlobalString(30022);		// 고객지원팀 요청 ( 버디 ) [11/6/2007 AJJIYA]
		break;

	case LT_ERROR_DISABLEDID:
		pszMsg = g_pResourceManager->GetGlobalString(30004);		///>  Illegal User
		break;

	case LT_ERROR_UNAVAILABLEID:
	case LT_ERROR_INVALID_CHARACTER:
	//case LT_ERROR_LEN_ID:					// ID 길이 오류 (ID:2~12)
		pszMsg = g_pResourceManager->GetGlobalString(30001);		///>  Unknown User
		break;

	case LT_ERROR_VERSION_LOWER	:			// 버전 mismatch
		pszMsg = g_pResourceManager->GetGlobalString(30006);		///>  Version Mismatch
		break;

	case LT_ERROR_RESOURCE_VERSION_LOWER :	// 리소스(SPF) 버전 mismatch
		pszMsg = g_pResourceManager->GetGlobalString(30012);		///> Resource(SPF) Version Mismatch
		break;

	case LT_ERROR_REQUIREAGREEMENT	:
		pszMsg = g_pResourceManager->GetGlobalString(30021);		///> Only Actoz User (Not Latale)
		break;

	case LT_ERROR_BLOCKED:
		pszMsg = g_pResourceManager->GetGlobalString(30011);		///> Restrict User
		break;

	case LT_ERROR_INVALID_ID_PW:			// 잘못된ID나 PW입니다.
		pszMsg = g_pResourceManager->GetGlobalString(30009);		///> InCorrect UserID
		break;

	case LT_ERROR_SERVICE_NOT_READY:
		if( g_pResourceManager->GetGlobalString(30005) != NULL)
			ShowNoticeBox(NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(30005)) ;		///> Server Checking Time
		ReserveExit() ;
		goto endProc;
		break ;

	case LT_ERROR_TIMEOUT:
		pszMsg = g_pResourceManager->GetGlobalString(80014);		///> Connect Fail
		break;

	//case LT_ERROR_EKEY_USERLOCKED :			// Ekey 5 Times Error
	//	ShowNoticeBox(NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString()) ;
	//	break ;

	default:
		{
			PrintLogs("[SPLoggin::OnResultAuthLogin] default Auth Denied. ErrCode=[%d]",retCode)  ;	
			TCHAR szBuf[260];
			_stprintf(szBuf,"Access Authentication Denied. ErrCode=[%d]\n",retCode);
			ShowNoticeBox(NOTICE_TYPE_YES, szBuf) ;		///> Access Deny
		}
		break;
	}

	if(pszMsg)
		ShowNoticeBox(NOTICE_TYPE_YES, pszMsg) ;
	else
		g_pInterfaceManager->SetNoticeHide();

endProc:

	g_pNetworkManager->Disconnect2();
	if( retCode != LT_NO_ERROR )	
	{
		m_pLoginTimer->Clear() ;
	}

	return 0;
}

/**
	E-key Authentication Result
	Enter > Ekey Dialog & Serial Dialog
	@param WPARAM : Result Global String ;
*/
int SPLoggin::OnResultEkeyResult(WPARAM wparam, LPARAM lparam)
{
	int iResultCode ;
	iResultCode = (int)wparam ;

	ForceCloseNotice() ;		//> Close 30000
	m_pEkeyTimer->Clear() ;
	m_bConnectTimeOut = false ;
	
	if( g_pResourceManager->GetGlobalString(iResultCode) != NULL)
			ShowNoticeBox(NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(iResultCode)) ;	
	else {
		char szMsg[256] ;
		sprintf(szMsg, "E-key Auth Result ErrorNo[%d]", iResultCode) ;
		ShowNoticeBox(NOTICE_TYPE_YES, szMsg) ;	
	}

	ErrorFLogs->Error_Log("OnResultEkeyResult ===> [%d]", iResultCode) ;
	return 0 ;
}


/**
	If Server need E-KeyCode 
	then close Login Dialog and show EkeyDialog
	-> LoginServer upkeep Connection
	// E-Key 입력 다이얼로그창을 띄우고, Challenge와 e-key 를 입력받고,
	//    AuthUser_EKeyInfo()를 호출하여 로긴 서버로 전송
	@param WPARAM : (UINT8)Ekey Type
	@param LPARAM : (TCHAR)szChallengeCode
	
*/
int SPLoggin::OnResultAuthEkey(WPARAM wparam, LPARAM lparam)
{
	std::string strChallengeCode ;
	
	ForceCloseNotice() ;		//> Close 30000
	m_pLoginTimer->Clear() ;
	m_pEkeyTimer->Clear() ;
	m_bConnectTimeOut = false ;

	ErrorFLogs->Error_Log("OnResultAuthEkey ===> Show Ekey Dialog [type:%d][challange:%s]", (int)wparam, (char*)lparam) ;

	SPWindowLogin*	pLWindow	=	(SPWindowLogin*)g_pInterfaceManager->FindWindow( WIID_LOGIN );
	if(pLWindow == NULL)	return 0 ;

	if( lparam == 0 )
		strChallengeCode = "" ;
	else
		strChallengeCode = (char*)lparam ;

	pLWindow->CloseLoginDialog() ;
	pLWindow->SetEnableEKeyDialog((int)wparam, strChallengeCode);
	m_iShowState = SHOW_EKEY_DLG ;
	return 1 ;
}


/**
	If E-Key Server Fail and Server need Serial-Key
	then close E-Key Dialog and show Serial-Key Dialog
	-> LoginServer upkeep Connection
	// EKeySN 입력 다이얼로그창을 띄우고, EKey machine's SN 끝에서 6자리를 입력받는다.
	//    AuthUser_EKeySN()를 호출하여 로긴 서버로 전송
	@param WPARAM : Result Code
*/
int SPLoggin::OnResultAuthSerial(WPARAM wparam, LPARAM lparam)
{
	ForceCloseNotice() ;		//> Close 30000
	m_pEkeyTimer->Clear() ;
	m_pLoginTimer->Clear() ;
	m_bConnectTimeOut = false ;

	ErrorFLogs->Error_Log("OnResultAuthSerial ===> Show Serial Dialog ") ;

	SPWindowLogin*	pLWindow	=	(SPWindowLogin*)g_pInterfaceManager->FindWindow( WIID_LOGIN );
	if(pLWindow == NULL)	return 0 ;

	pLWindow->CloseLoginDialog() ;
	pLWindow->CloseEkeyDialog() ;
	pLWindow->SetEnableSerialDialog() ;
	m_iShowState = SHOW_SERIAL_DLG ;
	return 1 ;
}


int SPLoggin::OnReqConnectTry(WPARAM wparam, LPARAM lparam)
{
	/*
	* wparam = (char*)ID;	
	* lParam = (char*)Passwd;
	*/	

	//return 0; // test

	if( m_pLoginTimer->IsEnable() == false ) 
	{	
		bool	bResult	=	false;
		std::string strLoginID = (char*)wparam ;
		std::string strLoginPW = (char*)lparam ;

		Trim( strLoginID ) ;
		Trim( strLoginPW ) ;
		m_LogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if( IsWebLogin() == false )
		{
			//// Launcher.exe --> Client.exe --> LoginServer
			//if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_ACTOZ )
			//{
			//	bResult = g_pNetworkManager->AuthUser( (char*)wparam , (char*)lparam);				// 2007.01.17 Version
			//	//bResult = g_pNetworkManager->AuthUserEx( (char*)wparam , (char*)lparam);			// 2007.01.23 New Version
			//	PrintLogs("[SPLoggin::OnReqConnectTry] Normal Actoz Login Send [ID:%s]", (char*)wparam)  ;	
			//}
			//else
			//{
				// Launcher.exe --> Client.exe --> LoginServer
				bResult = g_pNetworkManager->AuthUserEx( strLoginID.c_str() , strLoginPW.c_str());		
				PrintLogs("[SPLoggin::OnReqConnectTry] Normal Login Send [ID:%s]", strLoginID.c_str())  ;	
			//}
		}
		else
		{
			if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_BUDDYBUDDY )
			{
				bResult = g_pNetworkManager->AuthUserEx( strLoginID.c_str() , strLoginPW.c_str());
				PrintLogs("[SPLoggin::OnReqConnectTry] Buddy Login Send [%s , %s]", strLoginID.c_str(), strLoginPW.c_str())  ;	
			}
			else if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_ATGAMES )
			{
				bResult = g_pNetworkManager->AuthUserEx( strLoginID.c_str() , strLoginPW.c_str());
				PrintLogs("[SPLoggin::OnReqConnectTry] ATGames Login Send [%s , %s]", strLoginID.c_str(), strLoginPW.c_str())  ;	
			}
			else
			{
				// for ACTOZ
				// WEB --> Launcher.exe --> Client.exe --> LoginServer
				bResult	= g_pNetworkManager->AuthUser_KeyLogin( strLoginID.c_str() , strLoginPW.c_str() );
				PrintLogs("[SPLoggin::OnReqConnectTry] Web Login Send [%s , %s]", strLoginID.c_str(), strLoginPW.c_str())  ;	
			}
			m_bWebConnectTry = true ;
		}

		m_strWebID.clear() ;
		m_strWebKey.clear() ;
		strcpy( g_szWebArg , "" );

		strLoginPW.assign( strLoginPW.size() , ' ' );
		strLoginPW.clear();

		if( bResult == true )//흔벎콘攣횅젯쌈꼽삔鞫刻"젯쌈櫓"
		{
			ShowNoticeBox(NOTICE_TYPE_MSG, g_pResourceManager->GetGlobalString(30000)) ;		///> Connecting...
			
			if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_SANDA )
				m_pLoginTimer->Start(0, 180.0f) ;
			else if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_BUDDYBUDDY )
				m_pLoginTimer->Start(0, 80.0f) ;
			else if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_ATGAMES )
				m_pLoginTimer->Start(0, 80.0f) ;
			else
				m_pLoginTimer->Start(0, 30.0f) ;
		}
		else
		{
			ShowNoticeBox(NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(30007)) ;		///> Connect Fail
			PrintLogs("[SPLoggin::OnReqConnectTry] Network connect Failed")  ;	

			//////////////////////////////////////////////////////////////////////////
			//
			//	웹로그인시 에러코드와 상관없이 두번 이상의 접속을 못하게 하고
			//	기존의 아이디 패스워드 창을 보여주게 한다.
			//
			if( SPLocalizeManager::GetInstance()->OnlyWebLogin() )
			{
				ReserveExit() ;
			}
			else
			{
				SPWindow*	pWindow	=	g_pInterfaceManager->FindWindow( WIID_LOGIN );
				if( pWindow != NULL )
					pWindow->Show();
			}
		}
	}
	return 0;
}


/**
	Ekey를 이용한 로그인 
*/
int SPLoggin::OnReqEkeyConnectTry(WPARAM wparam, LPARAM lparam)
{
	m_pEkeyTimer->Start(1, 180.0f) ;		// Timer 180 Sec

	ShowNoticeBox(NOTICE_TYPE_MSG, g_pResourceManager->GetGlobalString(30000)) ;		///> Connecting...
	ErrorFLogs->Error_Log("OnReqEkeyConnectTry [%s][%s]", (char*)wparam, (char*)lparam) ;
	g_pNetworkManager->AuthUser_EKeyInfo((char*)wparam, (char*)lparam) ;
	
	return 0;
}

/**
	Serial 이용한 로그인 
*/
int SPLoggin::OnReqSerialConnectTry(WPARAM wparam, LPARAM lparam)
{
	m_pEkeyTimer->Start(2, 180.0f) ;		// Timer 180 Sec

	ShowNoticeBox(NOTICE_TYPE_MSG, g_pResourceManager->GetGlobalString(30000)) ;		///> Connecting...
	ErrorFLogs->Error_Log("OnReqSerialConnectTry [%s]", (char*)wparam) ;
	g_pNetworkManager->AuthUser_EKeySN((char*)wparam) ;

	return 0;
}


int SPLoggin::OnCheckWebLogin(WPARAM wparam, LPARAM lparam)
{
	return (int)IsWebLogin();
}


int SPLoggin::OnCheckTimeOut(WPARAM wparam, LPARAM lparam)
{
	if( (int)wparam == 1 )
		m_bConnectTimeOut = false ;
	return (m_bConnectTimeOut) ? 1 : 0 ;
}


int SPLoggin::OnCheckReserveExit(WPARAM wparam, LPARAM lparam)
{
	return (m_bReserveExit) ? 1 : 0 ;
}


int SPLoggin::OnSoftKeyPress(WPARAM wparam, LPARAM lparam)
{
	// 현재 IME의 위치에 해당 문자를 추가한다.
	SPWindowLogin*	pLWindow	=	(SPWindowLogin*)g_pInterfaceManager->FindWindow( WIID_LOGIN );
	g_pInterfaceManager->SetFocusWindow((SPWindow*)pLWindow->GetLastFocusEdit()) ;
	g_pInterfaceManager->GetIMEPool()->SetSelW(pLWindow->GetCaretStart(), pLWindow->GetCaretEnd()) ;
	if( wparam ) {
		g_pInterfaceManager->GetIMEPool()->WndProc(WM_CHAR, wparam, 0 ) ;
		//ErrorFLogs->Error_Log("Key[%c] Press", wparam) ;
	}
	return 0 ;
}


int SPLoggin::OnSoftKeyPressBack(WPARAM wparam, LPARAM lparam)
{
	// 현재 IME의 위치에 해당 문자 추가
	SPWindowLogin*	pLWindow	=	(SPWindowLogin*)g_pInterfaceManager->FindWindow( WIID_LOGIN );
	g_pInterfaceManager->SetFocusWindow((SPWindow*)pLWindow->GetLastFocusEdit()) ;
	g_pInterfaceManager->GetIMEPool()->SetSelW(pLWindow->GetCaretStart(), pLWindow->GetCaretEnd()) ;
	
	g_pInterfaceManager->GetIMEPool()->WndProc(WM_CHAR, VK_BACK, 0 ) ;
	return 0 ;
}


int SPLoggin::OnSoftKeyPressEnter(WPARAM wparam, LPARAM lparam)
{
	// 현재 IME의 위치에 해당 문자 추가
	SPWindowLogin*	pLWindow	=	(SPWindowLogin*)g_pInterfaceManager->FindWindow( WIID_LOGIN );
	pLWindow->PressEnterKey() ;
	return 0 ;
}

int SPLoggin::OnSoftKeyPressClose(WPARAM wparam, LPARAM lparam)
{
	// 현재 IME의 위치에 해당 문자 추가
	SPWindowLogin*	pLWindow	=	(SPWindowLogin*)g_pInterfaceManager->FindWindow( WIID_LOGIN );
	pLWindow->CloseSoftKeyboard() ;
	return 0 ;
}


int SPLoggin::OnSocketFailed(WPARAM wparam, LPARAM lparam)
{
	//m_strLog = g_pResourceManager->GetGlobalString(30008);
	return 0;
}

int SPLoggin::OnUserIn(WPARAM wParam, LPARAM lParam)//쌈돕륩蛟포句口되쩌냥묘
{
	CPacket* packet = (CPacket*)lParam;

	GLOBAL_STRING_ID	uiErrorID;
	PASSPORT			uiPassport;

	m_LogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	packet->Extract(&uiErrorID);

	if( uiErrorID == 0 )
	{
		packet->Extract(&uiPassport);
		g_pNetworkManager->SetInPassport(uiPassport);
		
		ShowNoticeBox(NOTICE_TYPE_MSG, g_pResourceManager->GetGlobalString(30000));		///> Connecting...
		m_iGameState	= GS_RELEASE_READY;	//학뻣돕lobby
		m_fWaitTime		= 2.0f;
		SPGameGuard::GetInstance()->SetUserID( g_pNetworkManager->GetLoginID() );
		PrintLogs("[SPLoggin::OnUserIn] User In OK") ;
	}
	else
	{
		g_pNetworkManager->Disconnect2();
		ShowNoticeBox(NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(uiErrorID)) ;
		PrintLogs("[SPLoggin::OnUserIn] UserIn Fail[%d-%s]", uiErrorID, g_pResourceManager->GetGlobalString(uiErrorID)) ;
		m_pLoginTimer->Clear() ;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	웹로그인시 에러코드와 상관없이 두번 이상의 접속을 못하게 하고
	//	기존의 아이디 패스워드 창을 보여주게 한다.
	//
	SPWindow*	pWindow	=	g_pInterfaceManager->FindWindow( WIID_LOGIN );

	if( pWindow != NULL && uiErrorID != 0 )
		pWindow->Show();

	return 0;
}

int SPLoggin::OnUserMove(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


void SPLoggin::PrintLogs(const TCHAR* fmt, ...)
{
	try
	{
		char msg[512];
		va_list va_ptr;

		va_start(va_ptr, fmt);
		vsprintf(msg, fmt, va_ptr);
		va_end(va_ptr);

		ErrorFLogs->Error_Log(msg) ;
	}catch (...) 
	{
	}
}
//痰빵되쩌,눈宮밑斤口못IGW,IGW되쩌
int SPLoggin::IGWLogin()
{
	if ( g_pCIGWLoaderDx9 )
	{
		BYTE PW[64];
		memset( PW, 0, sizeof(PW) );
		const char* IGWPW = g_pNetworkManager->GetIGWPW();
		CopyMemory( PW, IGWPW, strlen(IGWPW) + 1 );
		UserBasicInfoA UserInfo;
		UserInfo.cbSize = sizeof( UserBasicInfoA );
		UserInfo.EKey = g_pNetworkManager->GetIGWEkey().c_str();
		CopyMemory( UserInfo.Password, PW, 64 );
		UserInfo.PTAccount = g_pNetworkManager->GetIGWLoginID();
		memset( &UserInfo.PTNumID, 0, sizeof( UserInfo.PTNumID ) );
		memset( UserInfo.SSOAccessKey, 0, sizeof( UserInfo.SSOAccessKey ) );
		int LoginType = 1;
		if ( g_pNetworkManager->GetIGWEkey().empty() )
		{
			LoginType = 0;
		}
		g_pCIGWLoaderDx9->LoginA( &UserInfo, 0 );
	}
	g_pNetworkManager->IGWReset();
	return 1;
}

