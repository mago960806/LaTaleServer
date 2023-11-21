
#include "SPCommon.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"

//
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayer.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"

extern SPNetworkManager* g_pNetworkManager;


// 팀초대 결과. 에러인경우는 요청자에게만 송신, 성공인경우는 대상자에게만 송신
// TEAM_SC_REQUEST,	// 요청자와 대상자에게 송신
// GU_ID			ObjectGUID;					// 게임오브젝트아이디
// char				szCharName[LEN_NAME+1];		// 이름
// GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)
DEFINE_SC_PACKET_PROC(TEAM_SC_REQUEST)
{
	GU_ID ObjectGUID;
	packet->ExtractUINT64(&ObjectGUID);

	WPARAM wParam = (WPARAM)&ObjectGUID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_TEAM_INVITE, wParam, lParam);
}

// 팀참여응답 결과
// TEAM_SC_JOIN,		// 실패면 대상자에게 송신, 성공이면 모든 팀원에게 송신
// GU_ID			ObjectGUID;					// 게임오브젝트아이디
// char				szActCharName[LEN_NAME+1];	// 요청자이름
// GU_ID			RcvCharGUID;				// 대상자
// char				szRcvCharName[LEN_NAME+1];	// 대상자이름
// GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)
// UINT8			byLootingRule;				// 루팅룰(0:차례대로,1:랜덤,2:자유)
// int				iCnt;						// 팀원수
// TEAM_MEMBER		stTeam_Member;				// 팀원정보
// ...											// 팀원수만큼
DEFINE_SC_PACKET_PROC(TEAM_SC_JOIN)
{
	GU_ID ObjectGUID;
	packet->ExtractUINT64(&ObjectGUID);

	WPARAM wParam = (WPARAM)&ObjectGUID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_TEAM_JOIN, wParam, lParam);

}

// 팀탈퇴 결과.
// TEAM_SC_SECEDE,		// 모든 팀원에게 송신, *** 주의 : 팀원이 2명인 경우에 패킷을 받으면 파티해제해야합니다.
// GU_ID			ObjectGUID;					// 게임오브젝트아이디
// GU_ID			ActCharGUID;				// 탈퇴자
DEFINE_SC_PACKET_PROC(TEAM_SC_SECEDE)
{
	GU_ID ObjectGUID;

	packet->ExtractUINT64(&ObjectGUID);
	WPARAM wParam = (WPARAM)&ObjectGUID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_TEAM_SECEDE, wParam, lParam);

	// TODO : 성공처리
}

// 강퇴결과
// TEAM_SC_EXPEL,		// 모든 팀원에게 송신, *** 주의 : 팀원이 2명인 경우에 패킷을 받으면 파티해제해야합니다.
// GU_ID			ObjectGUID;					// 게임오브젝트아이디
// GU_ID			RcvCharGUID;				// 강퇴자
DEFINE_SC_PACKET_PROC(TEAM_SC_EXPEL)
{
	GU_ID ObjectGUID;

	packet->ExtractUINT64(&ObjectGUID);
	WPARAM wParam = (WPARAM)&ObjectGUID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_TEAM_EXPEL, wParam, lParam);
}

// 팀장위임 결과. 팀장이 탈퇴했을때도 사용됩니다.
// TEAM_SC_CHANGECAPTAIN,	// 모든 팀원에게 송신
// GU_ID			ObjectGUID;					// 게임오브젝트아이디
// GU_ID			RcvCharGUID;				// 새팀장
DEFINE_SC_PACKET_PROC(TEAM_SC_CHANGECAPTAIN)
{
	GU_ID ObjectGUID;

	packet->ExtractUINT64(&ObjectGUID);
	WPARAM wParam = (WPARAM)&ObjectGUID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_TEAM_CHANGE_MASTER, wParam, lParam);
}

// 루팅룰변경 결과
// TEAM_SC_CHANGELOOTINGRULE,	// 모든 팀원에게 송신
// GU_ID			ObjectGUID;					// 게임오브젝트아이디
// UINT8			byLottingRule;				// 새루팅룰
DEFINE_SC_PACKET_PROC(TEAM_SC_CHANGELOOTINGRULE)
{
	GU_ID ObjectGUID;

	packet->ExtractUINT64(&ObjectGUID);
	WPARAM wParam = (WPARAM)&ObjectGUID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_TEAM_CHANGE_LOOTING, wParam, lParam);
}

// 팀원정보갱신 결과(서버에서 보내기만 함) : 어떤시점에 보낼것인지 필요
// TEAM_SC_UPDATEMEMBER,	// 모든 팀원에게 송신. *** 주의 : 브로드캐스팅영역 밖의 팀원만 갱신합니다. 영역안의 팀원은 브로드캐스팅정보가 더 정확합니다.
// GU_ID			ObjectGUID;					// 게임오브젝트아이디
// TEAM_MEMBER		stTeam_Member;				// 팀원정보
DEFINE_SC_PACKET_PROC(TEAM_SC_UPDATEMEMBER)
{
	GU_ID ObjectGUID;

	packet->ExtractUINT64(&ObjectGUID);
	WPARAM wParam = (WPARAM)&ObjectGUID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_TEAM_UPDATE_MEMBER, wParam, lParam);
}

// 팀정보송신(채널이동한 유저에게 팀정보 송신시 사용)
//TEAM_SC_INFO,
// GU_ID					ObjectGUID;						// 게임오브젝트아이디
// UINT8					byLootingRule;					// 루팅룰(0:차례대로,1:랜덤,2:자유)
// int						iCnt;							// 팀원수
// TEAM_MEMBER				stTeam_Member;					// 팀원정보
// ...														// 팀원수만큼
DEFINE_SC_PACKET_PROC(TEAM_SC_INFO)
{
	GU_ID					ObjectGUID;

	packet->ExtractUINT64(&ObjectGUID);
	WPARAM wParam = (WPARAM)&ObjectGUID;
	LPARAM lParam = (LPARAM)packet;	

	g_pGOBManager->SPPerformMessage(SPGM_TEAM_CHANNEL_MOVE, wParam, lParam);

/*	GU_ID					ObjectGUID;
	UINT8					byLootingRule;					// 루팅룰(0:차례대로,1:랜덤,2:자유)
	int						iCnt;							// 팀원수
	TEAM_MEMBER				stTeam_Member;					// 팀원정보

	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT8(&byLootingRule);
	packet->ExtractUINT32((UINT32*)&iCnt);
	for(int i=0; i<iCnt; i++)
	{
		packet->ExtractStruct(&stTeam_Member, sizeof(TEAM_MEMBER));
	}

	*/
}
