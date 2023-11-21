

//***************************************************************************
// System Name : SPNetworkManager_System.cpp
// Comment     : 
//  [3/20/2008 metalgeni]
//  ReportShot 등 시스템관련 사항 추가로 인한 모듈추가
//***************************************************************************



#include "SPCommon.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"

// 2008.05.26 dhpark 보안툴 검증
#include "../NPGameGuard/SPGameGuard.h"

#include "SPNetworkManager.h"

extern SPNetworkManager* g_pNetworkManager;


DEFINE_CS_PACKET_PROC(SERVERTIME_CS_INFO)
{
	CPacket Packet;
	Packet.Add((UINT32)SERVERTIME_CS_INFO);

	GetPacketHandler()->PostMsg(&Packet);

	return 0;
}



// 서버시간/정보 송신(MAX_LEN_ID, LEN_IP는 널포함 길이입니다.)
//SERVERTIME_SC_INFO,
// GU_ID			ObjectGUID;					// 게임오브젝트아이디
// SYSTEMTIME		stServerTime;				// 서버시간
// WORLD_ID			wWorldID;					// 월드아이디
// SERVER_ID		wServerID;					// 서버아이디
// TCHAR			szAccountID[MAX_LEN_ID];	// 계정아이디
// TCHAR			szClientIP[LEN_IP];			// 클라이언트아이피
// TCHAR			szName[LEN_NAME+1];			// 캐릭터명
// UINT				uiStageID;					// 스테이지아이디
// UINT				uiMapGroupID;				// 맵그룹아이디
// UINT				uiPosX;						// X좌표
// UINT				uiPosY;						// Y좌표
DEFINE_SC_PACKET_PROC(SERVERTIME_SC_INFO)
{
	GU_ID			ObjectGUID;					// 게임오브젝트아이디
	SYSTEMTIME		stServerTime;				// 서버시간
	WORLD_ID		wWorldID;					// 월드아이디
	SERVER_ID		wServerID;					// 서버아이디
	TCHAR			szAccountID[MAX_LEN_ID];	// 계정아이디
	TCHAR			szClientIP[LEN_IP];			// 클라이언트아이피
	TCHAR			szName[LEN_NAME+1];			// 캐릭터명
	UINT			uiStageID;					// 스테이지아이디
	UINT			uiMapGroupID;				// 맵그룹아이디
	UINT			uiPosX;						// X좌표
	UINT			uiPosY;						// Y좌표

	ReportStuff rsStuffInstance;

	packet->ExtractUINT64(&rsStuffInstance.ObjectGUID); 
	packet->ExtractStruct(&rsStuffInstance.stServerTime, sizeof(SYSTEMTIME));
	packet->ExtractUINT16(&rsStuffInstance.wWorldID);
	packet->ExtractUINT16(&rsStuffInstance.wServerID);
	packet->ExtractData(rsStuffInstance.szAccountID, MAX_LEN_ID);
	packet->ExtractData(rsStuffInstance.szClientIP, LEN_IP);
	packet->ExtractData(rsStuffInstance.szName, LEN_NAME+1);
	packet->ExtractUINT32(&rsStuffInstance.uiStageID);
	packet->ExtractUINT32(&rsStuffInstance.uiMapGroupID);
	packet->ExtractUINT32(&rsStuffInstance.uiPosX);
	packet->ExtractUINT32(&rsStuffInstance.uiPosY);

	if(g_pMainGameManager)
		g_pMainGameManager->ReportShot(&rsStuffInstance);


}

// 서버에서 Query()를 통해서 생성된 검증용 데이터 송신
//SECURITY_SC_CHECK = SECURITY_CS_BASE,
// SECURITY_PACKDATA	stPackData;				// 보안툴 검증용 데이터
DEFINE_SC_PACKET_PROC(SECURITY_SC_CHECK)
{
	SECURITY_PACKDATA	stPackData = {0,};				// 보안툴 검증용 데이터

	packet->ExtractStruct(&stPackData, sizeof(SECURITY_PACKDATA));

	// 현재 클라이언트의 보안모듈로 데이터 생성 / 서버에 송신
	// nProtect는 콜백함수로 내부에서 패킷송신하고 XTrap은 바로 송신합니다. 모두 내부에서 패킷송신하도록 처리.
	SPGameGuard::GetInstance()->Auth(&stPackData);
}

// 보안툴 검사 결과 통보. 메세지창을 통해서 클라이언트 재시작을 유도합니다.
//SECURITY_SC_INFO,
// GLOBAL_STRING_ID		ErrorID;				// 에러아이디
DEFINE_SC_PACKET_PROC(SECURITY_SC_INFO)
{
	GLOBAL_STRING_ID		ErrorID = 0;				// 에러아이디

	packet->ExtractUINT32(&ErrorID);

	if(0 != ErrorID)
	{
		g_pSubGameManager->SPPerformMessage(SPGM_SUG_SOCKET_FAILED, ErrorID, 0);
		g_pNetworkManager->SetExceptSocketFailed(true);
	}
}














