// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : LataleServerTest랑 연동 안시킴..... AJJIYA
// Creation    : AJJIYA 2005-11-15    14:38 
//***************************************************************************

#include <Windows.h>

#ifdef _USED_GAME_GUARD
	#define _USING_NPROTECT		1

	#pragma comment( lib , "./NPGameGuard/NPGameLib" )
	
	#pragma comment( lib , "./XTrapLib/XTrap4Client_mt" )
	#pragma comment( lib , "./XTrapLib/XTrap4Client_ClientPart_mt" )
	#pragma comment( lib , "./XTrapLib/XTrap4Client_ServerPart_mt" )
	#pragma comment( lib , "./XTrapLib/XTrap_Unipher_mt" )
	#pragma comment( lib , "./XTrapLib/XTrap4Launcher_mt" )

	#pragma comment( lib , "./CrackProof/HtGEPD" )
	#pragma comment( lib , "./CrackProof/HtGECACln" )
#endif

#include "SPCommon.h"

#include "PacketID.h"
#include "SPNetworkDEF.h"
#include "SPNetworkManager.h"

#include "Packet.h"
#include "PacketHandler.h"

#include "./NPGameGuard/SDCltDynCode.h"		// SNDC Update [5/15/2007 AJJIYA]

#include "./XTrapLib/XTrap4Client.h"
#include "./XTrapLib/XTrap4Launcher.h"

#include "./CrackProof/Service.h"
#include "./CrackProof/HtGEPD.h"

#include "SPGameGuard.h"

static void WINAPI CPFCallBackFunc()
{
	LPCSTR	lpszMsg;
	BOOL	bAppExit = TRUE;

	//	저희 라테일에서 허용되지 않는 프로그램이 발견되었습니다.
	lpszMsg	=	"LaTale will be shot off for (using) unacceptable program.\r\nPlease check programs (running) on your PC\r\n";

	if (lpszMsg)
	{
		MessageBox( g_hWnd , lpszMsg , "CrackProof", MB_OK ); // 또는 게임 UI 사용
	}

	// 게임 종료시에만 false를 리턴
	if ( bAppExit == TRUE )
		SendMessage( g_hWnd , WM_CLOSE , bAppExit , 1 );

	::ExitProcess(0);
}

SPGameGuard::SPGameGuard()
{
	m_pNPGameLib	=	NULL;

#ifdef _USED_GAME_GUARD
	switch( g_eCountryLocale )
	{
	case CL_KOREA:
		m_pNPGameLib	=	new	CNPGameLib( "LaTale" );
		break;
	case CL_JAPAN:
		m_pNPGameLib	=	new	CNPGameLib( "LaTaleJP" );
		HtGEPDSetCallback( CPFCallBackFunc );
	    break;
	case CL_CHINA:
		SGDP::SDDynCodeCheckUpdate( "http://autopatch.chd.sdo.com/latale/latale/LaTale-MainServer" , SDUPDATE_SYNC );
		break;
	case CL_USA:
		if( g_bGameGuardPatch == true )
			XTrap_L_Patch( X_TRAP_ARG , NULL , 60 );

		XTrap_C_Start( X_TRAP_ARG , NULL );
		break;
	}
#endif
}

SPGameGuard::~SPGameGuard()
{
#ifdef _USED_GAME_GUARD
	delete m_pNPGameLib;

	if( Service::DriverIsRun( CPF_DRIVE_NAME ) == TRUE )
		Service::DriverUnload( CPF_DRIVE_NAME );
#endif
}

//////////////////////////////////////////////////////////////////////////
//
//	싱글톤 인터페이스
//

SPGameGuard* SPGameGuard::m_pGameGuardInstance	=	NULL;

SPGameGuard* SPGameGuard::GetInstance()
{
	if( m_pGameGuardInstance	==	NULL )
		m_pGameGuardInstance	=	new	SPGameGuard;

	return	m_pGameGuardInstance;
}

void SPGameGuard::DelInstance()
{
	delete m_pGameGuardInstance;
}

BOOL SPGameGuard::Init()
{
	if( m_pNPGameLib == NULL )
		return TRUE;

#ifdef _USED_GAME_GUARD
	DWORD	dwResult	=	m_pNPGameLib->Init();

	if( dwResult != NPGAMEMON_SUCCESS )
	{
		TCHAR	msg[256];
		LPCSTR	lpszMsg;

		DWORD	dwLocale	=	PRIMARYLANGID( ::GetUserDefaultLangID() );

		if( dwLocale == LANG_JAPANESE )
		{
#include "SPGameGuard-Init-Japan.cpp"
		}
		else
		{
			// ‘6. 주요에러코드’를 참조하여 상황에 맞는 메시지를 출력해줍니다.
			switch (dwResult)
			{
			case NPGAMEMON_ERROR_EXIST:
				lpszMsg = "게임가드가 실행 중 입니다. 잠시 후나 재부팅 후에 다시 실행해보시기 바랍니다.";
				break;
			case NPGAMEMON_ERROR_GAME_EXIST:
				lpszMsg = "게임이 중복 실행되었거나 게임가드가 이미 실행 중 입니다. 게임 종료 후 다시 실행해보시기 바랍니다.";
				break;
			case NPGAMEMON_ERROR_INIT:
				lpszMsg = "게임가드 초기화 에러입니다. 재부팅 후 다시 실행해보거나 충돌할 수 있는 다른 프로그램들을 종료한 후 실행해 보시기 바랍니다.";
				break;
			case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
			case NPGAMEMON_ERROR_NFOUND_GG:
			case NPGAMEMON_ERROR_AUTH_INI:
			case NPGAMEMON_ERROR_NFOUND_INI:
				lpszMsg = "게임가드 파일이 없거나 변조되었습니다. 게임가드 셋업 파일을 설치해보시기 바랍니다.";
				break;
			case NPGAMEMON_ERROR_CRYPTOAPI:
				lpszMsg = "윈도우의 일부 시스템 파일이 손상되었습니다. 인터넷 익스플로러(IE)를 다시 설치해보시기 바랍니다.";
				break;
			case NPGAMEMON_ERROR_EXECUTE:
				lpszMsg = "게임가드 실행에 실패했습니다. 게임가드 셋업 파일을 다시 설치해보시기 바랍니다.";
				break;
			case NPGAMEMON_ERROR_ILLEGAL_PRG:
				lpszMsg = "불법 프로그램이 발견되었습니다. 불필요한 프로그램을 종료한 후 다시 실행해보시기 바랍니다.";
				break;
			case NPGMUP_ERROR_ABORT:
				lpszMsg = "게임가드 업데이트를 취소하셨습니다. 접속이 계속 되지 않을 경우 인터넷 및 개인 방화벽 설정을 조정해 보시기 바랍니다.";
				break;
			case NPGMUP_ERROR_CONNECT:
			case HOOK_TIMEOUT:
				lpszMsg = "바이러스나 스파이웨어로 인해 후킹이 실패하였습니다. 최신백신을 받으신 후 컴퓨터 전체검사를 해봅니다.";
				break;
			case NPGAMEMON_ERROR_GAMEGUARD:
				lpszMsg = "게임가드 초기화 에러 또는 구버젼의 게임가드 파일입니다. 게임가드 셋업파일을 다시 설치하고 게임을 실행해봅니다.";
				break;
			case NPGMUP_ERROR_PARAM:
				lpszMsg = "ini 파일이 없거나 변조되었습니다. 게임가드 셋업 파일을 설치하면 해결할 수 있습니다.";
				break;
			case NPGMUP_ERROR_INIT:
				lpszMsg = "npgmup.des 초기화 에러입니다. 게임가드폴더를 삭제후 다시 게임실행을 해봅니다.";
				break;
			//case ???:
			//	lpszMsg = "검증되지 않은 Windows XP x64 버전을 실행한 경우입니다. 최신으로 버전으로 설치후 게임실행을 해봅니다.";
			//	break;
			case NPGMUP_ERROR_DOWNCFG:
				lpszMsg = "게임가드 업데이트 서버 접속에 실패하였습니다. 잠시 후 재시도 해보거나, 개인 방화벽이 있다면 설정을 조정해 보시기 바랍니다.";
				break;
			case NPGMUP_ERROR_AUTH:
				lpszMsg = "게임가드 업데이트를 완료하지 못 했습니다. 바이러스 백신을 일시 중시 시킨 후 재시도 해보시거나, PC 관리 프로그램을 사용하시면 설정을 조정해 보시기 바랍니다.";
				break;
			case NPGAMEMON_ERROR_NPSCAN:
				lpszMsg = "바이러스 및 해킹툴 검사 모듈 로딩에 실패 했습니다. 메모리 부족이거나 바이러스에 의한 감염일 수 있습니다.";
				break;
			default:
				// 적절한 종료 메시지 출력
				lpszMsg = "게임가드 실행 중 에러가 발생하였습니다. 게임 폴더 안의 GameGuard 폴더에 있는 *.erl 파일들을 Game1@inca.co.kr로 첨부하여 메일 보내주시기 바랍니다.";
				break;
			}
			wsprintf( msg, "게임가드 에러 : %lu" , dwResult );
		}

		MessageBox( NULL , lpszMsg , msg , MB_OK );

		return FALSE;
	}

#endif

	return TRUE;
}

void SPGameGuard::AfterCreateWindow()
{
	switch( g_eCountryLocale )
	{
		case CL_USA:
#ifdef _USED_GAME_GUARD
			XTrap_C_KeepAlive();
#endif
			break;
	}
}

void SPGameGuard::SetHwnd( HWND hWnd )
{
	if( m_pNPGameLib == NULL )
		return;

#ifdef _USED_GAME_GUARD
	m_pNPGameLib->SetHwnd( hWnd );
#endif
}

void SPGameGuard::SetUserID( const char* szUserID )
{
	if( m_pNPGameLib == NULL )
		return;

#ifdef _USED_GAME_GUARD
	m_pNPGameLib->Send( szUserID );
#endif
}

//void SPGameGuard::SetAuth( GG_AUTH_DATA& stAuthData )
//{
//	if( m_pNPGameLib == NULL )
//		return;
//
//#ifdef _USED_GAME_GUARD
//	m_pNPGameLib->Auth2( &stAuthData );
//#endif
//}

// 2008.05.26 dhpark 보안툴 검증
void SPGameGuard::Auth(SECURITY_PACKDATA* pPackData)
{
#ifdef _USED_GAME_GUARD
	BOOL bResult = 0;
	DWORD dwError = 0;
	SECURITY_PACKDATA stOutPackData = {0,};

	switch(g_eCountryLocale)
	{
	case CL_KOREA:
		if(m_pNPGameLib == NULL)
			return;
		dwError = m_pNPGameLib->Auth2(&pPackData->stAuthData);
		// *** 미결 : dwError처리. 현재는 서버에서 검출하여 접속종료처리함.
		// NPGameMonCallback()함수에서 결과 송신
		break;
	case CL_JAPAN:
		memcpy(&stOutPackData, pPackData, sizeof(SECURITY_PACKDATA));
		bResult = HtGECAAuthorize((PHtGECAData)stOutPackData.arrCPFData, (PHtGECAData)pPackData->arrCPFData);
		// *** 미결 : bResult처리. 현재는 서버에서 검출하여 접속종료처리함.
		// 바로 송신
		{
			CPacket packet(SECURITY_CS_CHECK);
			packet.AddData(&stOutPackData, sizeof(SECURITY_PACKDATA));
			if(g_pNetworkManager != NULL)
				g_pNetworkManager->GetPacketHandler()->PostMsg(&packet);
		}
		break;
	case CL_USA:
		memcpy(&stOutPackData, pPackData, sizeof(SECURITY_PACKDATA));
		dwError = XTrap_CS_Step2((LPCVOID)pPackData->arrBufPackData, (LPVOID)stOutPackData.arrBufPackData, XTRAP_PROTECT_PE | XTRAP_PROTECT_TEXT | XTRAP_PROTECT_EXCEPT_VIRUS);
		// *** 미결 : dwError처리. 현재는 서버에서 검출하여 접속종료처리함.
		// 바로 송신
		{
			CPacket packet(SECURITY_CS_CHECK);
			packet.AddData(&stOutPackData, sizeof(SECURITY_PACKDATA));
			if(g_pNetworkManager != NULL)
				g_pNetworkManager->GetPacketHandler()->PostMsg(&packet);
		}
		break;
	default:
		break;
	}
#endif
}

LPCSTR SPGameGuard::GetInfo()
{
	LPCSTR	lpszMsg	=	NULL;

	if( m_pNPGameLib == NULL )
		return lpszMsg;

#ifdef _USED_GAME_GUARD
	lpszMsg	=	m_pNPGameLib->GetInfo();
#endif

	return lpszMsg;
}

//////////////////////////////////////////////////////////////////////////
//
//	게임 가드 콜백 함수
//

BOOL CALLBACK NPGameMonCallback( DWORD dwMsg , DWORD dwArg )
{
	TCHAR msg[128];
	LPCTSTR lpszMsg = NULL;
	BOOL bAppExit;

#ifdef _USED_GAME_GUARD
	// AJJIYA [1/18/2006 AJJIYA]
	// Auth2 호출의 결과 메세지
	// 2008.05.26 dhpark 보안툴 검증
	if(dwMsg == NPGAMEMON_CHECK_CSAUTH2)
	{
		GG_AUTH_DATA stAuthData = {0,};
		memcpy(&stAuthData, (PVOID)dwArg, sizeof(GG_AUTH_DATA));
		
		SECURITY_PACKDATA stOutPackData = {0,};
		memcpy(&stOutPackData.stAuthData, &stAuthData, sizeof(GG_AUTH_DATA));

		CPacket packet(SECURITY_CS_CHECK);
		packet.AddData(&stOutPackData, sizeof(SECURITY_PACKDATA));
		if(g_pNetworkManager != NULL)
			g_pNetworkManager->GetPacketHandler()->PostMsg(&packet);

		return TRUE;
	}
#endif

	if( g_eCountryLocale == CL_JAPAN )
	{
#include "SPGameGuard-CallBack-Japan.cpp"
	}
	else
	{
		switch (dwMsg)
		{
		case NPGAMEMON_COMM_ERROR:
		case NPGAMEMON_COMM_CLOSE:
			bAppExit = TRUE; // 종료 코드
			return FALSE;
		case NPGAMEMON_INIT_ERROR:
			wsprintf( msg , "게임가드 초기화 에러 : %lu", dwArg);
			bAppExit = TRUE; // 종료 코드
			lpszMsg = msg;
			break;
		case NPGAMEMON_SPEEDHACK:
			wsprintf( msg , "스피드핵이 감지되었습니다.");
			bAppExit = TRUE; // 종료 코드
			lpszMsg = msg;
			break;
		case NPGAMEMON_GAMEHACK_KILLED:
			wsprintf( msg , "게임핵이 발견되었습니다.\r\n%s", SPGameGuard::GetInstance()->GetInfo() );
			bAppExit = TRUE; // 종료 코드
			lpszMsg = msg;
			break;
		case NPGAMEMON_GAMEHACK_DETECT:
			wsprintf( msg , "게임핵이 발견되었습니다.\r\n%s", SPGameGuard::GetInstance()->GetInfo() );
			bAppExit = TRUE; // 종료 코드
			lpszMsg = msg;
			break;
		case NPGAMEMON_GAMEHACK_DOUBT:
			wsprintf( msg , "게임이나 게임가드가 변조되었습니다.");
			bAppExit = TRUE; // 종료 코드
			lpszMsg = msg;
			break;
		default:
			// 적절한 종료 메시지 출력
			lpszMsg = "게임가드 실행 중 에러가 발생하였습니다. 게임 폴더 안의 GameGuard 폴더에 있는 *.erl 파일들을 Game1@inca.co.kr로 첨부하여 메일 보내주시기 바랍니다.";
			bAppExit = TRUE;
			break;
		}
	}

	if (lpszMsg)
	{
		MessageBox( g_hWnd , lpszMsg , "nProtect GameGuard", MB_OK ); // 또는 게임 UI 사용
	}

	// 게임 종료시에만 false를 리턴
	if ( bAppExit == TRUE )
	{
		SendMessage( g_hWnd , WM_CLOSE , bAppExit , 1 );
		return false;
	}

	return TRUE;
}
