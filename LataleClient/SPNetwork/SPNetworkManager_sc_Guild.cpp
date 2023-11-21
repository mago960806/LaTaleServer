
#include "SPCommon.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPWindow.h"
#include "SPInterfaceManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"
#include "SPEventDEF.h"
#include "SPEventManager.h"

#include "SPSubGameManager.h"
//
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayer.h"
#include "GlobalDefines_Share.h"
#include "SPGuildLDT.h"
#include "SPCropArchive.h"
#include "SPGuildArchive.h"

#include "SPAbuseFilter.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"

extern SPNetworkManager* g_pNetworkManager;


DEFINE_SC_PACKET_PROC(GUILD_SC_PCGUILD)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	PCGUILD				stPCGuild;							// 캐릭터 길드정보
	GUILD_BROADCAST		stGuildBroad ;
		
	packet->ExtractUINT64(&ObjectGUID);	
	packet->ExtractStruct(&stPCGuild, sizeof(PCGUILD));

	// 클라이언트 처리
	g_pGOBManager->GetGuildArchive()->SetGuildBasicInfo(&stPCGuild) ;
	memcpy(&stGuildBroad, &stPCGuild, sizeof(GUILD_BROADCAST)) ;
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_SET_GUILD_INFO, (LPARAM)&stGuildBroad);
}

DEFINE_SC_PACKET_PROC(GUILD_SC_GUILD_COMMON)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	GUILD_COMMON		stGuildCommon;						// 일반 길드정보
		
	packet->ExtractUINT64(&ObjectGUID);	
	packet->ExtractUINT32(&ErrorID);
	if(0 == ErrorID) // 성공이면
	{
		packet->ExtractStruct(&stGuildCommon, sizeof(GUILD_COMMON));

		// 클라이언트 처리
		g_pGOBManager->GetGuildArchive()->SetGuildCommonInfo(&stGuildCommon) ;
	}

}

DEFINE_SC_PACKET_PROC(GUILD_SC_MEMBERLIST)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	//INT32				iMemberCnt;							// 길드 멤버수
	//GUILD_MEMBER		stGuildMember;						// 길드 멤버정보
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	if(0 == ErrorID) // 성공이면
	{
		// 클라이언트 처리
		g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_MEMBER_LIST, 0, (LPARAM)packet) ;
		// g_pGOBManager->GetGuildArchive()->ReceiveGuildUserList(packet) ;
	}
}

DEFINE_SC_PACKET_PROC(GUILD_SC_CREATE)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	PCGUILD				stPCGuild;							// 캐릭터 길드정보
	GUILD_BROADCAST		stGuildBroad ;
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	if(0 == ErrorID) // 성공이면
	{
		packet->ExtractStruct(&stPCGuild, sizeof(PCGUILD));
		// 클라이언트 처리
		// 사용자 정보의 Emblem과 길드명 업데이트
		memcpy(&stGuildBroad, &stPCGuild, sizeof(GUILD_BROADCAST)) ;
		SPPlayer* pPlayer = (SPPlayer*)g_pGOBManager->Find(ObjectGUID);
		if( pPlayer == NULL )	{
			return ;
		}
		//눼쉔소痢
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->OnCommunityEventA(ceCreateGuild, stPCGuild.szGName );
		}
		pPlayer->SPGOBPerformMessage(SPGM_SET_GUILD_INFO, (LPARAM)&stGuildBroad);
		if( ObjectGUID == g_pGOBManager->GetLocalPlayer()->GetGUID() )
		{
			// Local Message
			g_pGOBManager->GetGuildArchive()->SetGuildBasicInfo(&stPCGuild) ;
			g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_CREATE, SPIM_GUILD_CREATE_RESULT, ErrorID, 0);
		}
	}
	else
	{
		// Error Message
		g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_CREATE, SPIM_GUILD_CREATE_RESULT, ErrorID, 0);
	}
}

DEFINE_SC_PACKET_PROC(GUILD_SC_DESTROY)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	if(0 == ErrorID) // 성공이면
	{
		SPPlayer* pPlayer = (SPPlayer*)g_pGOBManager->Find(ObjectGUID);
		if( pPlayer == NULL )	{
			return ;
		}
		pPlayer->SPGOBPerformMessage(SPGM_SET_GUILD_INFO, (LPARAM)0);

		if( ObjectGUID == g_pGOBManager->GetLocalPlayer()->GetGUID() )
		{
			g_pGOBManager->GetGuildArchive()->RemoveGuildInfo() ;
			if( g_pResourceManager->GetGlobalString(27000003) )
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000003) );
			else
			{
				TCHAR szError[64] ;
				wsprintf(szError, "[Guild]Destroy Success[%d]") ;
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szError );
			}
		}
	}
	else
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
		else
		{
			TCHAR szError[64] ;
			wsprintf(szError, "[Guild]Destroy Fail[%d]", ErrorID) ;
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szError );
		}
	}
}

DEFINE_SC_PACKET_PROC(GUILD_SC_UPDATE_SLOGAN)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	if(0 == ErrorID) // 성공이면
	{
		g_pGOBManager->GetGuildArchive()->ChangeSlogun(1, NULL) ;
	}
	else
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
		else
		{
			TCHAR szError[64] ;
			wsprintf(szError, "[Guild]Update Slogun Fail[%d]", ErrorID) ;
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szError );
		}
	}
}

DEFINE_SC_PACKET_PROC(GUILD_SC_REQUEST)
{
	//GU_ID				ObjectGUID;							// 게임오브젝트아이디
	//char				szGName[GUILD_MAX_NAME+1];			// 길드명
	//char				szCharName[LEN_NAME+1];				// 성공인 경우는 요청자명, 실패인 경우는 대상자명
	//GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)

	// 초대는 NoticeBox 때문에 Player에서 처리
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GUILD_INVITE, (LPARAM)packet);
}

DEFINE_SC_PACKET_PROC(GUILD_SC_JOIN)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)
	PCGUILD				stPCGuild;							// 캐릭터 길드정보
	GUILD_BROADCAST		stGuildBroad ;
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	
	if(0 == ErrorID) // 성공이면
	{
		packet->ExtractStruct(&stPCGuild, sizeof(PCGUILD));
		memcpy(&stGuildBroad, &stPCGuild, sizeof(GUILD_BROADCAST)) ;
		SPPlayer* pPlayer = (SPPlayer*)g_pGOBManager->Find(ObjectGUID);
		if( pPlayer == NULL )	{
			return ;
		}
		//속흙소痢
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->OnCommunityEventA(ceJoinGuild, stPCGuild.szGName );
		}
		pPlayer->SPGOBPerformMessage(SPGM_SET_GUILD_INFO, (LPARAM)&stGuildBroad);
		if( ObjectGUID == g_pGOBManager->GetLocalPlayer()->GetGUID() )
		{
			g_pGOBManager->GetGuildArchive()->SetGuildBasicInfo(&stPCGuild) ;
		}
	}
	else
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
		else
		{
			TCHAR szError[64] ;
			wsprintf(szError, "[Guild] Join Fail[%d]", ErrorID) ;
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szError );
		}
	}
	g_pGOBManager->GetGuildArchive()->ClearInviteName() ;
}

DEFINE_SC_PACKET_PROC(GUILD_SC_SECEDE)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);

	if(0 == ErrorID) // 성공이면
	{
		SPPlayer* pPlayer = (SPPlayer*)g_pGOBManager->Find(ObjectGUID);
		if( pPlayer == NULL )	{
			return ;
		}
		//藁놔소痢
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->OnCommunityEventA(ceQuitGuild, g_pGOBManager->GetGuildArchive()->GetGuildName() );
		}
		pPlayer->SPGOBPerformMessage(SPGM_SET_GUILD_INFO, (LPARAM)0);
		
		if( ObjectGUID == g_pGOBManager->GetLocalPlayer()->GetGUID() )
			g_pGOBManager->GetGuildArchive()->RemoveGuildInfo() ;

		g_pSubGameManager->SPPerformMessage(SPGM_INDUN_FORCE_EXIT, 0, 0);		// 길드방 나가기
	}
	else
	{
		// Guild Secede Fail
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
		else
		{
			TCHAR szError[64] ;
			wsprintf(szError, "[Guild] [%s]Secede Error[%d]", ErrorID) ;
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szError );
		}
	}
}

DEFINE_SC_PACKET_PROC(GUILD_SC_EXPEL)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	if(0 != ErrorID)	// 실패면
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
		else
		{
			TCHAR szError[64] ;
			wsprintf(szError, "[Guild] [%s]Expel Error[%d]", ErrorID) ;
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szError );
		}
	}
}

DEFINE_SC_PACKET_PROC(GUILD_SC_EXPEL_ME)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	packet->ExtractUINT64(&ObjectGUID);

	SPPlayer* pPlayer = (SPPlayer*)g_pGOBManager->Find(ObjectGUID);
	if( pPlayer == NULL )	{
		return ;
	}
	//굳소痢역뇜,藁놔소痢
	if ( g_pCIGWLoaderDx9 )
	{
		g_pCIGWLoaderDx9->OnCommunityEventA( ceQuitGuild, g_pGOBManager->GetGuildArchive()->GetGuildName() );
	}
	pPlayer->SPGOBPerformMessage(SPGM_SET_GUILD_INFO, (LPARAM)0);

	// 클라이언트 처리
	if( ObjectGUID == g_pGOBManager->GetLocalPlayer()->GetGUID() )
		g_pGOBManager->GetGuildArchive()->RemoveGuildInfo() ;

	g_pSubGameManager->SPPerformMessage(SPGM_INDUN_FORCE_EXIT, 0, 0);		// 길드방 나가기
}

DEFINE_SC_PACKET_PROC(GUILD_SC_CHANGEGRADE)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	if(0 != ErrorID)	// 실패면 신청한사람이 받는다.
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
}

DEFINE_SC_PACKET_PROC(GUILD_SC_CHANGEGRADE_ME)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	UINT8				byGrade;							// 변경 직책
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT8(&byGrade);

	// 클라이언트 처리
	// 실제 처리는 GUILD_SC_MSG_MEMBER 에서 진행
	// ReceiveChangeGuildUser -> SetGuildAuthority -> UpdateGuildUserList -> UpdateRankControl
	g_pGOBManager->GetGuildArchive()->UpdateMyGrade((int)byGrade) ;
}

DEFINE_SC_PACKET_PROC(GUILD_SC_MSG_MEMBER)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	UINT8				byMemberMsgType;					// 멤버메세지타입(0:가입, 1:탈퇴, 2:강퇴, 3:직위변동, 4:로그인, 5:로그아웃)
	GUILD_MEMBER		stGuildMember;						// 멤버정보
	char szMsg[256];
	TCHAR szPlayerName[MAX_LEN_NAME] ;

	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT8(&byMemberMsgType);
	packet->ExtractStruct(&stGuildMember, sizeof(GUILD_MEMBER));

	memset(szPlayerName, 0, MAX_LEN_NAME) ;
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szPlayerName);

	// 클라이언트 처리
	int iMsgType = (int)byMemberMsgType ;
	switch( iMsgType )
	{
	case 0:		// 가입
		//g_pGOBManager->GetGuildArchive()->ReceiveGuildUserJoin(&stGuildMember) ;
		g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_MEMBER_JOIN, 0, (LPARAM)&stGuildMember) ;
		if( g_pResourceManager->GetGlobalString(27000021) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(27000021), stGuildMember.szName);
		break ;
	case 1:		// 탈퇴
		//g_pGOBManager->GetGuildArchive()->ReceiveGuildUserSecede(&stGuildMember) ;
		g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_MEMBER_SECEDE, 0, (LPARAM)&stGuildMember) ;
		if( g_pResourceManager->GetGlobalString(27000022) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(27000022), stGuildMember.szName);
		break ;
	case 2:		// 강퇴
		//g_pGOBManager->GetGuildArchive()->ReceiveGuildUserSecede(&stGuildMember) ;
		g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_MEMBER_SECEDE, 0, (LPARAM)&stGuildMember) ;
		if( g_pResourceManager->GetGlobalString(27000023) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(27000023), stGuildMember.szName);
		break ;
	case 3:		// 직위변동
		//g_pGOBManager->GetGuildArchive()->ReceiveChangeGuildUser(&stGuildMember) ;
		g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_MEMBER_INFOCHANGE, 0, (LPARAM)&stGuildMember) ;
		if( g_pResourceManager->GetGlobalString(27000025) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(27000025), stGuildMember.szName);
		break ;
	case 4:		// 로그인
		if( g_pResourceManager->GetGlobalString(27000026) && strcmp(stGuildMember.szName, szPlayerName) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(27000026), stGuildMember.szName);
		else
			return ;
		break ;
	case 5:		// 로그아웃
		if( g_pResourceManager->GetGlobalString(27000027) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(27000027), stGuildMember.szName);
		break ;
	case 6:		// 마스터위임
		g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_MEMBER_INFOCHANGE, 0, (LPARAM)&stGuildMember) ;
		if( g_pResourceManager->GetGlobalString(27000073) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(27000073), stGuildMember.szName);
		break ;
	default:
		wsprintf(szMsg, "[Guild] MEMBER Information Change[%d]", iMsgType);
		break ;
	}
	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg );
}

DEFINE_SC_PACKET_PROC(GUILD_SC_MSG_GUILD)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	UINT8				byGuildMsgType;						// 길드메세지타입(0:슬로건변경, 1:랭크변경)
	GUILD_COMMON		stGuildCommon;						// 일반적인 길드정보
	char szMsg[256];

	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT8(&byGuildMsgType);
	packet->ExtractStruct(&stGuildCommon, sizeof(GUILD_COMMON));

	// 클라이언트 처리
	int iMsgType = (int)byGuildMsgType ;
	switch( iMsgType )
	{
	case 0:		// 슬로건 변경
		g_pGOBManager->GetGuildArchive()->SetGuildCommonInfo(&stGuildCommon) ;
		if( g_pResourceManager->GetGlobalString(27000024) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000024) );
		break ;
	case 1:		// 레벨 변경
		g_pGOBManager->GetGuildArchive()->SetGuildCommonInfo(&stGuildCommon) ;
		if( g_pResourceManager->GetGlobalString(27000059) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000059) );
		break ;
	case 2:		// 묘목 생성
		break ;
	case 3:		// 묘목 파괴
		if( g_pResourceManager->GetGlobalString(27000077) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000077) );
		break ;
	case 4:		// 묘목 성장완료
		if( g_pResourceManager->GetGlobalString(27000014) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000014) );
		break ;
	default:
		wsprintf(szMsg, "[Guild] Guild Information Change[%d]", iMsgType);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg );
		break ;
	}
}

// 요청자에게 보내는 길드권한위임 결과. 에러인경우는 요청자에게만 송신, 성공인경우는 요청자주변에 브로드캐스팅(예전길마의 길드탈퇴를 주변사람들에게 알리는 목적)
//GUILD_SC_CHANGEMASTER,
// GU_ID			ObjectGUID;							// 게임오브젝트아이디(요청자)
// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)
DEFINE_SC_PACKET_PROC(GUILD_SC_CHANGEMASTER)
{
	GU_ID				ObjectGUID;						// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;						// 에러아이디

	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	if(0 == ErrorID) // 성공이면
	{
		SPPlayer* pPlayer = (SPPlayer*)g_pGOBManager->Find(ObjectGUID);
		if( pPlayer == NULL )	{
			return ;
		}
		//겉痢낀못깎훙,菱성藁놔소痢
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->OnCommunityEventA( ceQuitGuild, g_pGOBManager->GetGuildArchive()->GetGuildName() );
		}
		pPlayer->SPGOBPerformMessage(SPGM_SET_GUILD_INFO, (LPARAM)0);

		if( ObjectGUID == g_pGOBManager->GetLocalPlayer()->GetGUID() )
			g_pGOBManager->GetGuildArchive()->RemoveGuildInfo() ;
	}
	else			// 실패면
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
}

// 대상자에게 보내는 길드권한위임 결과. 길마직급으로 변경이 필요하게되면 여기서 처리해야합니다. 현재는 길드정보창에만 길마직급액션이 몰려있어서 필요없음.
//GUILD_SC_CHANGEMASTER_ME,
// GU_ID			ObjectGUID;							// 게임오브젝트아이디(새마스터)
DEFINE_SC_PACKET_PROC(GUILD_SC_CHANGEMASTER_ME)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디

	packet->ExtractUINT64(&ObjectGUID);
	if( ObjectGUID == g_pGOBManager->GetLocalPlayer()->GetGUID() )
	{
		// 위임 받는 사람은 창을 다시 띄우면 Refresh
		g_pGOBManager->GetGuildArchive()->UpdateMyGrade(GUILD_GRADE_01) ;
	}
}

// 묘목심기 결과. 주변에 브로드캐스트합니다.(단, 길드방에서만 가능한 액션이고 길드방은 전체가 브로드캐스트영역으로 설정한다고 약속됨.)
//GUILD_SC_PLANT,
// GU_ID			ObjectGUID;							// 게임오브젝트아이디
// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)
// UINT32			uiCropID;							// 묘목아이디
DEFINE_SC_PACKET_PROC(GUILD_SC_PLANT)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	UINT32				uiCropID;							// 묘목아이디
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	if(0 == ErrorID) // 성공이면
	{
		packet->ExtractUINT32(&uiCropID);
		g_pGOBManager->GetGuildArchive()->SetCropInformation(CROP_SET_INITIALIZE, uiCropID) ;	// Set
		// Show Chat Message
		if( g_pResourceManager->GetGlobalString(27000013) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000013) );

	}
	else
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
}

// 묘목뽑기 결과. 주변에 브로드캐스트합니다.(단, 길드방에서만 가능한 액션이고 길드방은 전체가 브로드캐스트영역으로 설정한다고 약속됨.)
//GUILD_SC_GRUB,
// GU_ID			ObjectGUID;							// 게임오브젝트아이디
// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)
DEFINE_SC_PACKET_PROC(GUILD_SC_GRUB)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	if(0 == ErrorID) // 성공이면
	{
		g_pGOBManager->GetGuildArchive()->SetCropInformation(CROP_SET_CLEAR) ;		// Clear
		
		g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	}
	else
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
}

// 비료/촉진제 주기 결과. 주변에 브로드캐스트합니다.(단, 길드방에서만 가능한 액션이고 길드방은 전체가 브로드캐스트영역으로 설정한다고 약속됨.)
//GUILD_SC_NOURISH,
// GU_ID			ObjectGUID;							// 게임오브젝트아이디
// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)
// UINT32			uiRemainTime;						// 현재시간과 비교하여 계산된 성장완료까지 남은시간(초단위)
// UINT32			uiNourishTime;						// 현재시간과 비교하여 계산된 비료를 먹을 시간(초단위)
// UINT32			uiHastenSumTime;					// 촉진제 누적시간
DEFINE_SC_PACKET_PROC(GUILD_SC_NOURISH)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	UINT32				uiRemainTime;						// 현재시간과 비교하여 계산된 성장완료까지 남은시간(초단위)
	UINT32				uiNourishTime;						// 현재시간과 비교하여 계산된 비료를 먹을 시간(초단위)
	UINT32				uiHastenSumTime;					// 촉진제 누적시간
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	if(0 == ErrorID) // 성공이면
	{
		packet->ExtractUINT32(&uiRemainTime);
		packet->ExtractUINT32(&uiNourishTime);
		packet->ExtractUINT32(&uiHastenSumTime);
		g_pGOBManager->GetGuildArchive()->SetCropInformation(CROP_SET_NOURISH, 0, uiRemainTime, uiNourishTime, uiHastenSumTime) ;		// TimeUpdate
	}
	else
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
}

// 묘목성장완료. 길드방내의 모든 PC에게 브로드캐스트합니다.
//GUILD_SC_GROWTHCROP,
DEFINE_SC_PACKET_PROC(GUILD_SC_GROWTHCROP)
{
	// 묘목 성장 완료 동기화
	g_pGOBManager->GetGuildArchive()->SetCropGrowthComplete() ;
}

// 랭크업. 길드원주변에 브로드캐스팅하지 않습니다.(2007.08.13 dhpark 유명상팀장이 결정)
//GUILD_SC_RANKUP,
// GU_ID			ObjectGUID;							// 게임오브젝트아이디
// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)
// INT32			iGNum;								// 길드DBNum
// UINT8			byNewGRank;							// New길드랭크
DEFINE_SC_PACKET_PROC(GUILD_SC_RANKUP)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	INT32				iGNum;								// 길드DBNum
	UINT8				byNewGRank;							// New길드랭크
	char				szMsg[256];
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	if(0 == ErrorID) // 성공이면
	{
		packet->ExtractUINT32((UINT32*)&iGNum);
		packet->ExtractUINT8(&byNewGRank);
		g_pGOBManager->GetGuildArchive()->UpdateGuildLevel((int)byNewGRank) ;
		g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_INFO_ENABLE, 0, 0, true);		// 길드 기본정보 Update (for Tooltip)
		if( g_pResourceManager->GetGlobalString(27000059) )	{

			wsprintf(szMsg, g_pResourceManager->GetGlobalString(27000059), byNewGRank);
			char szLineFeedBuf[MAX_STR_LENGTH+1];
			SPAbuseFilter::GetInstance()->ReplaceLineFeed( szMsg , szLineFeedBuf , MAX_STR_LENGTH );
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szLineFeedBuf );
		}
	}
	else
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
}

// 길드마크변경. 길드원주변에 브로드캐스팅하지 않습니다.(2007.08.13 dhpark 유명상팀장이 결정)
//GUILD_SC_CHANGEMARK,
// GU_ID			ObjectGUID;							// 게임오브젝트아이디
// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)
// INT32			iGNum;								// 길드DBNum
// UINT8			arrGMark[3];						// 길드마크
DEFINE_SC_PACKET_PROC(GUILD_SC_CHANGEMARK)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	INT32				iGNum;								// 길드DBNum
	UINT8				arrGMark[3];						// 길드마크
	GUILD_BROADCAST		stGuildBroad ;
		
	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);
	if(0 == ErrorID) // 성공이면
	{
		packet->ExtractUINT32((UINT32*)&iGNum);
		packet->ExtractStruct(arrGMark, sizeof(UINT8)*3);
		g_pGOBManager->GetGuildArchive()->UpdateMyEmblem(arrGMark[0], arrGMark[1], arrGMark[2]) ;
	}
	else
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
		g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_EMBLEM, SPIM_GUILD_EM_CHANGE_RESULT, -1, 0);
	}
}
