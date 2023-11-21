// SPNetworkManager_sc_proc.cpp

#include "SPCommon.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"

#include <WinSock2.h>
#include "LT_Error.h"
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"

//#include "SPWindow.h"
//#include "SPNoticeBox.h"

#include "SPMouseCursor.h"
#include "SPResourceDef.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"
#include "DebugPrint.h"

#include "./NPGameGuard/SPGameGuard.h"		// AJJIYA [11/15/2005]

#include "SPOptionDEF.h"
#include "SPClientOptionArchive.h"

#include "SPResourceManager.h"

#include "PacketHandler.h"

#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayer.h"

#include "GlobalDefines_Share.h"

#include "LataleCipher.h"
#include "LataleCipherDefines.h"


extern char g_szLoginServerIP[MAX_PATH];
extern int g_iLoginServerPort;

extern SPNetworkManager* g_pNetworkManager;
extern SPInterfaceManager* g_pInterfaceManager;
extern int g_iCurrentCipherMode;

#define MAX_CLIENT_DYN_CODE_LEN	10000

static BYTE s_szClientDynCode[MAX_CLIENT_DYN_CODE_LEN] = {0,};
static int s_iClientDynCodeLen = 0;

bool g_bDynCodeInstalled = false;

DEFINE_SC_PACKET_PROC(PROTOCOL_SC_START_DYNCODE_1)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	UINT32		iDynCodeLenStep1;

	g_bDynCodeInstalled = false;

	packet->ExtractUINT32(&iDynCodeLenStep1);
	if(iDynCodeLenStep1 < 1)
	{
		assert(false && "Invalid DynCodeLen.");
		DebugPrint("[FATAL ERROR] Step1 : Invalid DynCodeLen.\n");
		return;
	}

	packet->Extract(&s_szClientDynCode, iDynCodeLenStep1);

	s_iClientDynCodeLen = iDynCodeLenStep1;
}

DEFINE_SC_PACKET_PROC(PROTOCOL_SC_START_DYNCODE_2)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	UINT32		iDynCodeLenStep2;

	packet->ExtractUINT32(&iDynCodeLenStep2);
	if(s_iClientDynCodeLen + iDynCodeLenStep2 < 1)
	{
		assert(false && "Invalid DynCodeLen.");
		DebugPrint("[FATAL ERROR] Step2 : Invalid DynCodeLen.\n");
		return;
	}

	if (s_iClientDynCodeLen > MAX_CLIENT_DYN_CODE_LEN
		||
		s_iClientDynCodeLen + iDynCodeLenStep2 > MAX_CLIENT_DYN_CODE_LEN)
	{
		MessageBox( NULL, "뚤꼇폅，貢쭹젯쌈믐崍，헝路폘와빵똥。", "댄轎", MB_OK );
		return;
	}

	BYTE* pszDynCodePos = s_szClientDynCode + s_iClientDynCodeLen;

	packet->Extract(pszDynCodePos, iDynCodeLenStep2);

	s_iClientDynCodeLen += iDynCodeLenStep2;
}

DEFINE_SC_PACKET_PROC(PROTOCOL_SC_LOGINSERVERINFO)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	UINT16	uiServerID = 0;
	TCHAR	szIP[LEN_IP] = {0,};
	UINT16	usPort = 0;

	packet->ExtractUINT16(&uiServerID);
	packet->Extract(szIP, LEN_IP);
	packet->ExtractUINT16(&usPort);

	g_pNetworkManager->SetLoginServerID(uiServerID);
	g_pNetworkManager->SetLoginServerInfo(szIP,usPort);

	_tcsncpy(g_szLoginServerIP,szIP,LEN_IP);
	g_iLoginServerPort = usPort;

}

DEFINE_SC_PACKET_PROC(PROTOCOL_SC_START_REQ)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	UINT8	uiStartDataLen = 0;
	UINT8	uiCipherMode = 0;

	packet->ExtractUINT8(&uiStartDataLen);

//#ifdef _USED_GAME_GUARD
	//if(uiStartDataLen > 0 && uiStartDataLen == sizeof(GG_AUTH_DATA))
	//{
	//	GG_AUTH_DATA authData = {0,};
	//	packet->Extract(&authData, sizeof(GG_AUTH_DATA));
	//	packet->ExtractUINT8(&uiCipherMode);

	//	if(uiCipherMode != g_iCurrentCipherMode)
	//	{
	//		g_iCurrentCipherMode = uiCipherMode;

	//		Cipher_Exit();
	//		if(!Cipher_Init(g_iCurrentCipherMode))
	//		{
	//			assert(false && "PROTOCOL_SC_START_REQ :Cipher_Init() failed ");
	//		}
	//		else if(!Cipher_SetClientDynCode(s_szClientDynCode,s_iClientDynCodeLen))
	//		{
	//			assert(false && "Check the site : Cipher_SetClientDynCode() failed.");
	//			DebugPrint("[FATAL ERROR] Step2 : Cipher_SetClientDynCode() failed.\n");
	//			return;
	//		}

	//		g_bDynCodeInstalled = true;
	//		g_pNetworkManager->SetCipherInitialized(true);

	//	}

	//	SPGameGuard::GetInstance()->SetAuth( authData );
	//}
	//else
//#endif	
	{
		packet->UpdateRPoint(uiStartDataLen);
		packet->ExtractUINT8(&uiCipherMode);


		//{
			g_iCurrentCipherMode = uiCipherMode;

			Cipher_Exit();
			if(!Cipher_Init(g_iCurrentCipherMode))
			{
				assert(false && "PROTOCOL_SC_START_REQ :Cipher_Init() failed ");
				DebugPrint("[FATAL ERROR] Cipher_Init() failed.\n");
				
				//g_pInterfaceManager->SetNotice("[FATAL ERROR] Cipher_Init() failed.", NULL, NOTICE_TYPE_MSG) ;

				return;
			}

			if(g_iCurrentCipherMode == CIPHER_LIB_SNDC && s_iClientDynCodeLen > 0)
			{
				if(!Cipher_SetClientDynCode(s_szClientDynCode,s_iClientDynCodeLen))
				{
					assert(false && "Check the site : Cipher_SetClientDynCode() failed.");
					DebugPrint("[FATAL ERROR] Step2 : Cipher_SetClientDynCode() failed.\n");

					//g_pInterfaceManager->SetNotice("[FATAL ERROR] Cipher_SetClientDynCode() failed.", NULL, NOTICE_TYPE_MSG) ;
					return;
				}
			}

			g_bDynCodeInstalled = true;
			g_pNetworkManager->SetCipherInitialized(true);
		//}
	}

	CPacket packet2(PROTOCOL_CS_START_RES);
	packet2.AddUINT8(uiStartDataLen);
	g_pNetworkManager->GetPacketHandler()->PostMsg(&packet2);

}


DEFINE_SC_PACKET_PROC(PROTOCOL_SC_CHECK_CLIENT_VALIDATION_REQ)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	UINT32 uiValue1;
	UINT32 uiValue2;
	UINT32 uiValue3;
	UINT32 uiValue4;

	packet->ExtractUINT32(&uiValue1);
	packet->ExtractUINT32(&uiValue2);
	packet->ExtractUINT32(&uiValue3);
	packet->ExtractUINT32(&uiValue4);

	uiValue4 = uiValue3 + uiValue2 + uiValue1;
	uiValue3 = uiValue2 + uiValue1;

	CPacket packet2(PROTOCOL_CS_CHECK_CLIENT_VALIDATION_RES);
	packet2.AddUINT32(uiValue1);
	packet2.AddUINT32(uiValue2);
	packet2.AddUINT32(uiValue3);
	packet2.AddUINT32(uiValue4);
	g_pNetworkManager->GetPacketHandler()->PostMsg(&packet2);
}

DEFINE_SC_PACKET_PROC(PROTOCOL_SC_START)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	GLOBAL_STRING_ID	itemID = 0;
	packet->ExtractUINT32(&itemID);
	if(itemID == 0)	
	{
		// NO_ERROR

		// 이 시점에 일반 Game Logic 관련 패킷 송수신 가능함.
		if( g_pSubGameManager->GetSubGameID() != SUBGAME_LOGGIN )
			g_pNetworkManager->UserInRequest();
	}
	else
	{
#ifdef _USED_GAME_GUARD
		g_pSubGameManager->SPPerformMessage(SPGM_SUG_SOCKET_FAILED, itemID, 0);
#else
		if( g_pSubGameManager->GetSubGameID() != SUBGAME_LOGGIN )
			g_pNetworkManager->UserInRequest();
#endif
	}

	g_pNetworkManager->SetReceived_PROTOL_SC_START();
}

DEFINE_SC_PACKET_PROC(GAME_SC_START)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	//g_pGOBManager->SPPerformMessage(GAME_SC_START, wParam, lParam);
}


DEFINE_SC_PACKET_PROC(SYSTEM_SC_ERROR)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;


	GLOBAL_STRING_ID	ErrorID;
	packet->ExtractUINT32(&ErrorID);
	if(ErrorID == 0) 
	{
		return;
	}
	else if(ErrorID == 90001)
	{
		TCHAR szBuf[260];
		_stprintf(szBuf,_T("ERROR(%d) : Encryption Key Mismatch!!!\n"),ErrorID);
		MessageBox(NULL,szBuf,"Login",MB_OK);
	}

	//g_pGOBManager->SPPerformMessage(SYSTEM_SC_ERROR, wParam, lParam);
}


DEFINE_SC_PACKET_PROC(GAME_SC_GETOPTION)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	GLOBAL_STRING_ID	ErrorID;
	UINT16				uiGameOption;		// 귀속말,친구추가,거래요청,팀요청,상태알림			

	packet->ExtractUINT32(&ErrorID);
	if(ErrorID == 0) {
		packet->ExtractUINT16(&uiGameOption);
		//g_pClientOption->SPPerformMessage( SPOAM_RECEIVE_FROM_SERVER, uiGameOption);		
	} else {
		//g_pClientOption->SPPerformMessage( SPOAM_RECEIVE_FROM_SERVER, SSO_DEFAULT);
	}
    	
}

DEFINE_SC_PACKET_PROC(GAME_SC_SETOPTION)
{
	//WPARAM wParam = 0;
	//LPARAM lParam = (LPARAM)packet;
	// 저장의 결과값...

	// GLOBAL_STRING_ID	ErrorID;
	// UINT8			uiGameOption;		// 무시 가능(단지 재전송된 값)

	//g_pGOBManager->SPPerformMessage(GAME_SC_SETOPTION, wParam, lParam);

	// 변경된 옵션을 브로드캐스팅함.
	GU_ID				guid;
	GLOBAL_STRING_ID	ErrorID;
	UINT32				iGameOptions;		// 귀속말,친구추가,거래요청,팀요청,상태알림			


	packet->Extract(&guid);
	packet->ExtractUINT32(&ErrorID);

	if(ErrorID == 0) {		
		packet->ExtractUINT32(&iGameOptions);

		WPARAM wParam = (WPARAM)&guid;
		LPARAM lParam = (UINT32)iGameOptions;
		g_pGOBManager->SPPerformMessage(SPGM_STAT_SC_SETOPTION, wParam, lParam);
	}	
}


DEFINE_SC_PACKET_PROC(STAT_SC_CHANGE)
{
	GU_ID guid;
	packet->Extract(&guid);

	WPARAM wParam = (WPARAM)&guid;
	LPARAM lParam = (LPARAM)packet;

	g_pGOBManager->SPPerformMessage(SPGM_STAT_SC_CHANGE, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(STAT_SC_CHANGE_EX)
{
	GU_ID guid;
	packet->Extract(&guid);

	WPARAM wParam = (WPARAM)&guid;
	LPARAM lParam = (LPARAM)packet;

	g_pGOBManager->SPPerformMessage(SPGM_STAT_SC_CHANGE_EX, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(STAT_SC_LEVEL_UP)
{
	GU_ID guid;
	packet->Extract(&guid);

	WPARAM wParam = (WPARAM)&guid;
	LPARAM lParam = (LPARAM)packet;

	// Perform Msg
	g_pGOBManager->SPPerformMessage(SPGM_STAT_SC_LEVEL_UP, wParam, lParam);

}
DEFINE_SC_PACKET_PROC(WORLDENV_SC_DATETIME)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	//g_pGOBManager->SPPerformMessage(WORLDENV_SC_DATETIME, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(WORLDENV_SC_WEATHER)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	//g_pGOBManager->SPPerformMessage(WORLDENV_SC_WEATHER, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(AUTH_SC_LOGIN)
{
	RESULTCODE	retCode = 0;
	USERID		UserID = 0;
	PASSPORT	Passport = 0;	
	SERVER_ID	ServerID = 0;
	TCHAR		szID[MAX_LEN_ID+1] = {0,};

	packet->Extract(&retCode);
	if(retCode == LT_NO_ERROR) 
	{
		packet->Extract(&UserID);
		packet->Extract(&Passport);
		packet->Extract(&ServerID);
		packet->ExtractData(szID,MAX_LEN_ID);

		g_pNetworkManager->SetUserID(UserID);
		g_pNetworkManager->SetPassport(Passport);
		g_pNetworkManager->SetLoginServerID(ServerID);
		g_pNetworkManager->SetLoginID(szID);

		if(g_pNetworkManager->GetSite() == SITE_SANDA)
		{
			UINT32 uiFDS_Param1 = 0;
			UINT32 uiFDS_Param2 = 0;
			packet->ExtractUINT32(&uiFDS_Param1);
			packet->ExtractUINT32(&uiFDS_Param2);
			g_pNetworkManager->FDS_SetParam(uiFDS_Param1,uiFDS_Param2);
		}
	}
	
	g_pSubGameManager->SPPerformMessage(SPGM_AUTH_SC_LOGIN, (WPARAM)retCode);
}


DEFINE_SC_PACKET_PROC(AUTH_SC_LOGIN_EX)
{
	GLOBAL_STRING_ID globalStringID = 0;
	USERID		UserID = 0;
	PASSPORT	Passport = 0;	
	SERVER_ID	ServerID = 0;
	UINT8		uiLenID = 0;
	TCHAR		szID[MAX_LEN_ID+1] = {0,};

	packet->Extract(&globalStringID);
	if(globalStringID == 0) 
	{
		packet->Extract(&UserID);
		packet->Extract(&Passport);
		packet->Extract(&ServerID);
		packet->Extract(&uiLenID);
		uiLenID = min(uiLenID,MAX_LEN_ID);
		packet->ExtractData(szID,uiLenID);

		g_pNetworkManager->SetUserID(UserID);
		g_pNetworkManager->SetPassport(Passport);
		g_pNetworkManager->SetLoginServerID(ServerID);
		g_pNetworkManager->SetLoginID(szID);
	}

	g_pSubGameManager->SPPerformMessage(SPGM_AUTH_SC_LOGIN_EX, (WPARAM)globalStringID);
}

DEFINE_SC_PACKET_PROC(AUTH_SC_EKEY)
{
	UINT8	mode = 0;											// 1(EKEY_REQUEST),2(EKEY_ERROR),3(EKEY_SN_REQUEST)
	int		errorValue = 0;

	packet->Extract(&mode);
	packet->ExtractUINT32((UINT32*)&errorValue);

	switch(mode)
	{
	// E-key 입력 요구
	case 1:
		{
			UINT8 EKeyType = 0;								// 1(without challenge), 3(with challenge)	
			UINT8 lenChallenge = 0;					
			TCHAR szChallenge[MAX_PATH] = {0,};			

			packet->Extract(&EKeyType);
			packet->Extract(&lenChallenge);
			if(lenChallenge > 0)
				packet->ExtractData(szChallenge,lenChallenge);	// include NULL

			// E-Key 입력 다이얼로그창을 띄우고, Challenge와 e-key 를 입력받고,
			//    AuthUser_EKeyInfo()를 호출하여 로긴 서버로 전송
			g_pSubGameManager->SPPerformMessage(SPGM_AUTH_SC_EKEY, (WPARAM)EKeyType, (LPARAM)szChallenge);

#ifdef _DEBUG
			DebugPrint("AUTH_SC_EKEY : Mode=[RequestEKey] Type=[%d] Challenge[%s]\n",EKeyType,szChallenge);
#endif
		}
		break;

	// E-key 에 대한 인증 결과
	case 2:
		{
			GLOBAL_STRING_ID	itemID = 0;	

			switch(errorValue) 
			{
				case 1001:	itemID = 20020001; break;		// The net connection is not quite well.
				case 1002:	itemID = 20020002; break;		// The Ekey Serial Number you entered doesn't exist.
				case 1003:	itemID = 20020003; break;		// The Ekey Dynamic Password you entered is wrong.
				case 1004:	itemID = 20020004; break;		// The User Service Number you entered is incorrect.
				case 1006:	itemID = 20020005; break;		// Your EKey hasn't been bound with any account yet.
				case 1007:	itemID = 20020006; break;		// No related Ekey binding information.
				case 1008:	itemID = 20020007; break;		// Binding Failed.
				case 1009:	itemID = 20020008; break;		// This Ekey has already bound with an account.
				case 1011:	itemID = 20020009; break;		// Please check if there's any typing error and try again.
				case 1012:	itemID = 20020010; break;		// The net connection is not quite well. .... 
				case 1013:	itemID = 20020011; break;		// The net connection is not quite well. .... 
				case 1014:	itemID = 20020012; break;		// This account has already been bound with an Ekey.
				case 1015:	itemID = 20020013; break;		// The User Service Number is wrong.
				case 1019:	itemID = 20020014; break;		// Game Type number error.
				case 1020:	itemID = 20020015; break;		// Game distriction error.
				case 1021:	itemID = 20020016; break;		// Game Account error.
				case 1023:	itemID = 20020017; break;		// Identification card ID error.
				case 1024:	itemID = 20020018; break;		// The synchronous time of the Ekey fluctuates too much.
				case 1025:	itemID = 20020019; break;		// reserved password error.
				case 1027:	itemID = 20020020; break;		// You've changed your service number too many times to modify again because of the related regulation.
				case 1028:	itemID = 20020021; break;		// Waiting for working.
				case 10:	itemID = 20020022; break;		// EKey Status Error : Your Ekey can not be use.
				case 20:	itemID = 20020023; break;		// EKey Status Error : You must startup your Ekey before you bind it.
				case 40:	itemID = 20020024; break;		// EKey Status Error : The Ekey yor operated has been reporteed as lost,...
				case 50:	itemID = 20020025; break;		// EKey Status Error : The Ekey yor operated has been reporteed as lost,...
				case 60:	itemID = 20020026; break;		// EKey Status Error : This Ekey is invalid.
				case 70:	itemID = 20020027; break;		// EKey Status Error : This Ekey is invalid.
				case 80:	itemID = 20020028; break;		// EKey Status Error : As a result of entering incorrect dynamic password too many times in a little period of time, your Ekey has been locked.
				case 90:	itemID = 20020029; break;		// EKey Status Error : Your Ekey has been destroyed.
				case 100:	itemID = 20020030; break;		// EKey Status Error : Your Ekey is out of service time and cannot be used again.
				case 110:	itemID = 20020031; break;		// EKey Status Error : Your Ekey has been replaced by a new Ekey.
				default:	itemID = errorValue; break;
			}
			g_pSubGameManager->SPPerformMessage(SPGM_AUTH_SC_EKEY_RES, (WPARAM)itemID);

#ifdef _DEBUG
			DebugPrint("AUTH_SC_EKEY : Mode=[EKeyError] errValue=[%d] GlobalStringID=[%d] \n",errorValue,itemID);
#endif
		}
		break;
	case 3:
		{
			// EKeySN 입력 다이얼로그창을 띄우고, EKey machine's SN 끝에서 6자리를 입력받는다.
			//    AuthUser_EKeySN()를 호출하여 로긴 서버로 전송
			g_pSubGameManager->SPPerformMessage(SPGM_AUTH_SC_EKEY_SN);

#ifdef _DEBUG
			DebugPrint("AUTH_SC_EKEY : Mode=[RequestEKeySN]\n");
#endif
		}
		break;
	default:
		break;
	}
}


DEFINE_SC_PACKET_PROC(USER_SC_IN)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	g_pSubGameManager->SPPerformMessage(SPGM_SUG_SC_USER_IN, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(USER_SC_MOVE)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	g_pSubGameManager->SPPerformMessage(SPGM_SUG_SC_USER_MOVE, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(WORLD_SC_GETLIST)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	g_pSubGameManager->SPPerformMessage(SPGM_WORLD_SC_GETLIST, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(WORLD_SC_SELECT)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	g_pSubGameManager->SPPerformMessage(SPGM_WORLD_SC_SELECT, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(WORLD_SC_CHANNELSTATUS)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	g_pSubGameManager->SPPerformMessage(SPGM_WORLD_SC_CHANNELSTATUS, wParam, lParam);
}

// 채널 변경 과정중 채널 목록 요청의 결과
DEFINE_SC_PACKET_PROC(WORLD_SC_CHANNELSTATUS2)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	g_pInterfaceManager->SPPerformMessage(SPIM_CHANNEL_SET, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CHANNEL_SC_SELECT)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	g_pSubGameManager->SPPerformMessage(SPGM_CHANNEL_SC_SELECT, wParam, lParam);
}

// 채널 변경 과정중 채널 선택의 결과
DEFINE_SC_PACKET_PROC(GAME_SC_CHANGE_CHANNEL)
{
	WPARAM wParam = 0;

	GLOBAL_STRING_ID	itemID = 0;			// GLOBAL_STRING.LDT 상의 메세지를 itemID 값

	packet->ExtractUINT32(&itemID);

	LPARAM lParam = (LPARAM)packet;

	if(itemID != 0)
	{
		if( itemID == LT_ERROR_SYSTEMERROR )
		{
			if( g_pSubGameManager != NULL )
				g_pSubGameManager->SPPerformMessage(SPGM_EXIT_ZONE, 0);

			return;
		}

		D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
		g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(itemID), (LPARAM)&color);
		g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_BTN_ENABLE, 1, 0);
		
		if( g_pGOBManager->GetLocalPlayer() )
			g_pGOBManager->GetLocalPlayer()->TriggerEnable(true);

		return;
	}

	TCHAR ip[LEN_IP] = {0,};
	SERVER_PORT port = 0;
	USERID userID = 0;
	PASSPORT passport2 = 0;

	packet->ExtractStruct(ip,LEN_IP);		// Destination Channel IP address
	packet->ExtractUINT16(&port);			//		"				port
	packet->ExtractUINT32(&userID);
	packet->ExtractUINT32(&passport2);		// new passport to be created by destination Channel

	g_pNetworkManager->SetGameServerInfo(ip,port);
	//g_pNetworkManager->SetUserID(userID);
	g_pNetworkManager->SetPassport(passport2);

#if 1
	g_pSubGameManager->SPPerformMessage(SPGM_CHANNEL_CHANGE_READY, wParam, lParam);
	
	
	//g_pSubGameManager->SPPerformMessage(SPGM_EXIT_ZONE, 2);	
#endif

}

DEFINE_SC_PACKET_PROC(WORLD_SC_ENTER)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	g_pSubGameManager->SPPerformMessage(SPGM_WORLD_SC_ENTER, wParam, lParam);

	// 캐릭터 처음엔 SPLobby , 채널 이동시엔 SPZone으로 메세지가 날아간다. [4/5/2006]
	// 타이틀 데이터를 UI상으로 넣어준다. [4/5/2006]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_STATUS		, SPIM_TITLE_GET_LIST , NULL , NULL );
}

DEFINE_SC_PACKET_PROC(WORLD_SC_LEAVE)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;


	g_pSubGameManager->SPPerformMessage(SPGM_WORLD_SC_LEAVE, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CHAR_SC_STATUSINFO)
{
	GU_ID guid;
	packet->Extract(&guid);

	STATUS_INFO tStatInfo = {0,};
	// for test
	{
		
		packet->Extract(&tStatInfo, sizeof(STATUS_INFO));
	}

	WPARAM wParam = (WPARAM)&guid;
	LPARAM lParam = (LPARAM)&tStatInfo;

	g_pGOBManager->SPPerformMessage(SPGM_SET_SC_STATUSINFO, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CHAR_SC_EFFECT_STATUS_INFO)
{
	GU_ID guid;
	packet->Extract(&guid);

	EFFECT_STATUS_INFO tStatInfo= {0,};
	{
		packet->Extract(&tStatInfo, sizeof(EFFECT_STATUS_INFO));
	}

	WPARAM wParam = (WPARAM)&guid;
	LPARAM lParam = (LPARAM)&tStatInfo;

	g_pGOBManager->SPPerformMessage(SPGM_SET_SC_EFFECT_STATUS, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CHAR_SC_QUICK_SLOT)
{
	GU_ID guid;
	packet->Extract( &guid );

	UINT8		iNumOfSlot = 0;
	packet->ExtractUINT8( &iNumOfSlot );

	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_GET_QUICK_SLOT	, (WPARAM)iNumOfSlot	, (LPARAM)packet );	// AJJIYA [7/6/2007 AJJIYA]
}

DEFINE_SC_PACKET_PROC(MSG_SC_NOTICE)
{
	RESULTCODE		resultCode;
	UINT8			displayTime;		// 0(forever), sec
	UINT8			optionFlags;		// reserved. display 효과 용도(캄박임,칼라,...)
	UINT16			len;
	TCHAR			szMsg[MAX_CHATMSG_LEN+1] ={0,};

	packet->Extract(&resultCode);
	if(resultCode == LT_NO_ERROR) 
	{
		packet->Extract(&displayTime);
		packet->Extract(&optionFlags);
		packet->Extract(&len);
		len = min(len,MAX_CHATMSG_LEN);
		packet->ExtractData(szMsg,len);		

		if(optionFlags > 0) {
			//g_pInterfaceManager->SPPerformMessage(SPIM_SET_MESSAGE_TEXT, (WPARAM)szMsg, (LPARAM)MAX_EVENT);
			//g_pInterfaceManager->SPPerformMessage(SPIM_SET_MESSAGE_SHOW, (WPARAM)true);

			//g_pSubGameManager->SPPerformMessage(SPGM_GOB_ENTER,		(WPARAM), lParam);
			g_pMainGameManager->OnSetNotice(szMsg, displayTime, optionFlags);
		}
		else {
			D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg, (LPARAM)&color);
			g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMsg , (LPARAM)&color );
		}
	}

}

DEFINE_SC_PACKET_PROC(CHAT_SC_MSG)
{
	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_GET_MSG , (WPARAM)packet );
	return;
}

DEFINE_SC_PACKET_PROC(CHAT_SC_MSG_MONEY)
{
	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_GET_BUGLE, (WPARAM)packet );
	return;
}

DEFINE_SC_PACKET_PROC(CHAT_SC_MSG_PCINFO)
{
	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_GET_VISUALBUGLE, (WPARAM)packet );
	return;
}

DEFINE_SC_PACKET_PROC(MONEY_SC_CHAT)
{
	GU_ID				globalID = 0;

	packet->ExtractUINT64(&globalID);
	WPARAM wParam = (WPARAM)&globalID;	
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_ITEM_SC_CHAT, wParam, lParam);
	return;
}

DEFINE_SC_PACKET_PROC(CHAR_SC_GM_STATUS)
{
	GU_ID				globalID = 0;

	packet->ExtractUINT64(&globalID);
	WPARAM wParam = (WPARAM)&globalID;	
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_STAT_SC_ISGM, wParam, lParam);
	return;
}
// Friend/Block list
DEFINE_SC_PACKET_PROC(FRIEND_SC_GETLIST)
{	
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND , SPIM_GET_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.8.29]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FOLLOW , SPIM_GET_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.10.10]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_SNATCH , SPIM_GET_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.10.11]


	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_GET_FRIEND , 1 , (LPARAM)packet );	// AJJIYA [10/25/2005]
	return;
}

DEFINE_SC_PACKET_PROC( FRIEND_SC_ADD)
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND , SPIM_ADD_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.8.29]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FOLLOW , SPIM_ADD_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.10.10]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_SNATCH , SPIM_ADD_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.10.10]


	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_ADD_FRIEND , 1 , (LPARAM)packet );	// AJJIYA [10/25/2005]
	return;
}

DEFINE_SC_PACKET_PROC( FRIEND_SC_ADD_RECV)
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_ADD_RECV_FRIEND , 1 , (LPARAM)packet );	// AJJIYA [10/25/2005]
	return;
}

DEFINE_SC_PACKET_PROC( FRIEND_SC_ACCEPT)
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND , SPIM_ACCEPT_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.8.29]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FOLLOW , SPIM_ACCEPT_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.10.10]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_SNATCH , SPIM_ACCEPT_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.10.10]

	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_ACCEPT_FRIEND , 1 , (LPARAM)packet );	// AJJIYA [10/25/2005]
	return;
}

DEFINE_SC_PACKET_PROC( FRIEND_SC_DELETE)
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND , SPIM_DEL_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.8.29]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FOLLOW , SPIM_DEL_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.10.10]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_SNATCH , SPIM_DEL_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.10.10]

	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_DEL_FRIEND , 1 , (LPARAM)packet );	// AJJIYA [10/25/2005]
	return;
}

DEFINE_SC_PACKET_PROC( FRIEND_SC_REFRESH_STATUS)
{
	// AJJIYA [11/29/2005]
	// 현재 쓰지 않고 있음
	// FRIEND_SC_REFRESH_STATUS 로 대체됨....
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND , SPIM_REFRESH_STATUS_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.8.29]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FOLLOW , SPIM_REFRESH_STATUS_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.10.10]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_SNATCH , SPIM_REFRESH_STATUS_FRIEND , 1 , (LPARAM)packet ); //[liuyang, 2008.10.10]

	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_REFRESH_STATUS_FRIEND , 1 , (LPARAM)packet );	// AJJIYA [10/25/2005]
	return;
}


DEFINE_SC_PACKET_PROC( BLOCK_SC_GETLIST)
{	
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND , SPIM_GET_FRIEND , 2 , (LPARAM)packet ); //[liuyang, 2008.8.29]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FOLLOW , SPIM_GET_FRIEND , 2 , (LPARAM)packet ); //[liuyang, 2008.10.10]

	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_GET_FRIEND , 2 , (LPARAM)packet );	// AJJIYA [10/25/2005]
	return;
}

DEFINE_SC_PACKET_PROC( BLOCK_SC_ADD)
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND , SPIM_ADD_FRIEND , 2 , (LPARAM)packet ); //[liuyang, 2008.8.29]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FOLLOW , SPIM_ADD_FRIEND , 2 , (LPARAM)packet ); //[liuyang, 2008.10.10]

	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_ADD_FRIEND , 2 , (LPARAM)packet );	// AJJIYA [10/25/2005]
	return;
}

DEFINE_SC_PACKET_PROC( BLOCK_SC_DELETE)
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FIND , SPIM_DEL_FRIEND , 2 , (LPARAM)packet ); //[liuyang, 2008.8.29]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND_FOLLOW , SPIM_DEL_FRIEND , 2 , (LPARAM)packet ); //[liuyang, 2008.10.10]

	g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_DEL_FRIEND , 2 , (LPARAM)packet );	// AJJIYA [10/25/2005]
	return;
}


DEFINE_SC_PACKET_PROC( BLOCK_SC_ISBLOCKED)
{
	GLOBAL_STRING_ID	itemID;			// GLOBAL_STRING.LDT 상의 메세지를 itemID 값
	GU_ID			guid;				// 요청한 당사자의 GU_ID
	
	packet->ExtractUINT32(&itemID);
	packet->Extract(&guid);

	WPARAM wParam = (WPARAM)&guid;
	LPARAM lParam = (LPARAM)&itemID;

	g_pGOBManager->SPPerformMessage(SPGM_FIGURE_INFOS_BLOCK_SC_ISBLOCKED, wParam, lParam);

	wParam = 0;
	lParam = (LPARAM)packet;

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_PC_INFO , SPIM_LOVE_CHAR_INFO , wParam , lParam );

	//g_pInterfaceManager->SPPerformMessage( BLOCK_SC_ISBLOCKED, NULL , (LPARAM)packet );
	return;
}

// 등록된 친구의 online 상태 알리미
DEFINE_SC_PACKET_PROC( NOTI_SC_ONLINE_STATUS_FRIEND)
{
	UINT8		uiNotiType=0;				// 1(NotiType_Friend_OnlineStatus)
	UINT8		uiStatus=0;					// 1(CH_IN),2(CH_OUT) [,3(CH_MOVE),4(ETC)]
	UINT16		uiServerID=0;				// 현재 위치한 서버
	UINT32		uiActorCharID=0;
	UINT8		len=0;
	TCHAR		szActorCharName[MAX_LEN_NAME] = {0,};

	packet->ExtractUINT8(&uiNotiType);
	packet->ExtractUINT8(&uiStatus);
	packet->ExtractUINT16(&uiServerID);
	packet->ExtractUINT32(&uiActorCharID);
	packet->ExtractUINT8(&len);
	len = min(len,MAX_LEN_NAME-1);
	if(len > 0)
	{
		packet->ExtractData(szActorCharName,len);

		TCHAR szMsg[MAX_CHATMSG_LEN] = {0,};

		//_stprintf(szMsg,"NOTI_SC_ONLINE_STATUS_FRIEND(%d) : Status=[%d] ServerID=[%d] ActorCharID=[%u] [%d][%s]",
		//	uiNotiType,uiStatus,uiServerID,uiActorCharID,len,szActorCharName);

		D3DXCOLOR color( 0.52156f , 0.99215f , 0.83137f , 1.0f );		//	133,253,212 RGB

		int	iGlobalStringID	=	5016019;

		if( uiStatus == 1 )					//	Channel In
			iGlobalStringID	=	5016019;
		else if( uiStatus == 2 )			//	Channel Out
			iGlobalStringID	=	5016020;

		const char*	pszGlobalString	=	g_pResourceManager->GetGlobalString( iGlobalStringID );

		if( pszGlobalString != NULL )
		{
			_stprintf( szMsg , g_pResourceManager->GetGlobalString( iGlobalStringID ) , szActorCharName );

			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg, (LPARAM)&color);
			g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/[System]_Friend_Join.wav");
		}
	}
	else
	{
#ifdef _DEBUG
		DebugPrint("[NOTI_SC_ONLINE_STATUS_FRIEND] ERROR \n");
#endif
	}
}

DEFINE_SC_PACKET_PROC(SUBLOGIC_SC_COMMAND)
{
	UINT8 uiLen = 0;
	DECLARE_VAR(TCHAR,szCommand,MAX_PATH);

	packet->ExtractUINT8(&uiLen);

	uiLen = min(uiLen,MAX_PATH);
	if(uiLen > 0)
	{
		packet->ExtractData(szCommand,uiLen);

		if(g_pNetworkManager)
			g_pNetworkManager->SubLogic_PushCommand(szCommand);
	}

#ifdef _DEBUG
	DebugPrint("[SUBLOGIC_SC_COMMAND] : cmd=[%s][%d]\n",szCommand,uiLen);
#endif

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Fatigue Degree System

DEFINE_SC_PACKET_PROC(FDS_SC_NOTICE)
{
	UINT32		uiOnlineMinutes = 0;			// 접속시간
	UINT32		uiOfflineMinutes = 0;			// 비접속시간
	UINT16		usRate_Ely = 0;				
	UINT16		usRate_Exp = 0;					// 현재 적용된 경험치 관련 factor
	UINT16		usRate_ItemDrop = 0;			// 현재 적용된 아이템 드랍률 관련 factor
	int			iMsgNum = 0 ;					// GlobalString Number
	TCHAR szMessage[256] ;

	packet->ExtractUINT32(&uiOnlineMinutes);
	packet->ExtractUINT32(&uiOfflineMinutes);
	packet->ExtractUINT16(&usRate_Ely);
	packet->ExtractUINT16(&usRate_Exp);
	packet->ExtractUINT16(&usRate_ItemDrop);

	if( uiOnlineMinutes == 60 )			iMsgNum = 15000006 ;
	else if( uiOnlineMinutes == 120 )	iMsgNum = 15000007 ;
	else if( uiOnlineMinutes == 180 )	iMsgNum = 15000003 ;
	else if( uiOnlineMinutes > 180 && uiOnlineMinutes < 300  )	iMsgNum = 15000004 ;
	else if( uiOnlineMinutes >= 300  )	iMsgNum = 15000005 ;
	
	// Notice Message for CHATTING
	if( !g_pNetworkManager->FDS_IsShowChat() )		// Initial Login & Channel Change
	{
		if( uiOnlineMinutes != 0 ) {				// if it hasn't onlineTime then, not display
			int iHour, iMinute ;
			iHour = uiOnlineMinutes / 60 ;
			iMinute = uiOnlineMinutes % 60 ;
			sprintf( szMessage, g_pResourceManager->GetGlobalString(15000002), iHour, iMinute);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMessage );
		}
		g_pNetworkManager->FDS_SetChat(true) ;
	}
	else if( g_pResourceManager->GetGlobalString(iMsgNum) ) {
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(iMsgNum) );
	}
	else	{
		sprintf( szMessage, "[NOTICE]Game Play Time[%d Minutes], EXP&ITEM Drop Rate[%d]", uiOnlineMinutes, usRate_Exp);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMessage );
	}
#ifdef _DEBUG
	DebugPrint("[FDS_SC_NOTICE] : [%d][%d] [%d][%d][%d]\n",uiOnlineMinutes,uiOfflineMinutes,usRate_Ely,usRate_Exp,usRate_ItemDrop);
#endif

}

void SPNetworkManager::FDS_QueryState()
{
	UINT32		uiReservedParam1 = 0;
	UINT32		uiReservedParam2 = 0;

	CPacket packet;

	packet.AddUINT32(FDS_CS_QUERYSTATE);
	packet.AddUINT32(uiReservedParam1);
	packet.AddUINT32(uiReservedParam2);

	GetPacketHandler()->PostMsg(&packet);
}

//////////////////////////////////////////////////////////////////////////
//
//	CoolTime
//

DEFINE_SC_PACKET_PROC(COOLTIME_SC_INFOS)
{
	GU_ID guid;
	packet->Extract(&guid);

	WPARAM wParam = (WPARAM)&guid;
	LPARAM lParam = (LPARAM)packet;

	if( g_pGOBManager != NULL )
		g_pGOBManager->SPPerformMessage( SPGM_SET_COOLTIME , wParam , lParam );
}

//--------------------------------------------------
// greener guide
DEFINE_SC_PACKET_PROC(GREENERGUIDE_SC_EVENT)
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_GREENERGUIDE, SPIM_GREENERGUIDE_EVENT, 0, (LPARAM)packet );
	return;
}
DEFINE_SC_PACKET_PROC(GREENERGUIDE_SC_FETECHAWARD)
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_GREENERGUIDE, SPIM_GREENERGUIDE_FETCHAWARD, 0, (LPARAM)packet );
	return;
}
//--------------------------------------------------
