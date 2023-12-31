// packetID.h

#pragma once

#ifdef _LATALE_CLIENT
	#include "BaseTypes.h"
#endif
#include "SPGameObjectDEF.h"
//#include "../LataleGameLogic/ItemStruct.h"
#include <time.h>
#include "SQLTypes.h"

#define __NEW_STAT 1
#define PACKET_ID_VERSTION	0.7
// history
/*

	0.7
		- 2006/6/2	: int,unsigned int 배제
	0.6
		- 8월 8일 사내 테스트 버전
	0.5
		- WORLD_ID 전송 : CHAR_CS_CREATE,CHAR_CS_CHECK_CHARNAME,CHAR_CS_DELETE
	0.4 
		- CHARACTER_INFO 삭제. 모든 소스 CHARACTER_INFO2 사용하게 수정
		- NAV_SC_INSIGHT 송신시, MOB의 경우 MONSTER_INFO 송신하게 수정
		- CharName : 12 ==> 16 으로 변경

	0.3
		.... 중략
*/


// LEN_ID / LEN_PW 대신에 g_LEN_ID / g_LEN_PW (GlobalDefines_Share.h) 를 사용할 것.   2006/11/29
//
// ID/PW/CHARNAME Length Definition 
//{
		#define	MIN_LEN_NAME			2
		#define MIN_LEN_GUILDNAME		2			// for CHINA

		#define LEN_NAME				16			// charName,Name

		#define	MAX_LEN_ID				24
		#define	MAX_LEN_PW				24
		#define	MAX_LEN_NAME			24

		// 2007.02.27	dhpark	DB에서의 실제사이즈에 대한 정의. 현재는 지엠툴에서 사용하기 위해서 정의함.
		// 2007-5-3		hoshim	LEN_ID/PW에 대한 종속성 배제 목적으로 서버 코드상에도 사용되기 시작.
		#define	MAX_LEN_ID_DB			20
		#define	MAX_LEN_PW_DB			20
		#define	MAX_LEN_NAME_DB			20
		#define LEN_BILLING_KEY			40 

	#if MAX_LEN_ID_DB >= MAX_LEN_ID
		#error "MAX_LEN_ID_DB DEFINITION [FAILED]: MAX_LEN_ID_DB >= MAX_LEN_ID !!!"
	#endif

	// MIN_LEN_NAME <= LEN_NAME <MAX_LEN_NAME
	#if LEN_NAME >= MAX_LEN_NAME
		#error "LEN_NAME DEFINITION [FAILED]: MAX_LEN_NAME > LEN_NAME !!!"
	#endif

//}

#define MAGIC_CHAR					'*'

#define LEN_IP						16			// ip address
#define LEN_SERVERNAME				20			// server name

#define MAX_CHATMSG_LEN				150		// 채팅 메세지 문자열 제한값
#define LEN_CASHITEM_NORMAL_MSG		60		// Cash Item - '월드/채널 외치기'시 문자열 제한값
//#define LEN_CASHITEM_EVENT_MSG		150		// Cash Item - '이벤트 메세지'시 문자열 제한값

#define MAX_CHAR_NUM				3			// 유저당 생성가능 캐릭터 수
#define MAX_CHAR_DEL				30			// 유저당 캐릭 최대삭제횟수
#define MAX_WORLDS					10			// 최대 월드 수
#define MAX_CHANNELS				20			// 월드내 최대 채널 수(1 ~ 20)
#define MAX_INDUNS					30			// 월드내 최대 인던서버 수(51 ~ 80)
#define MAX_PVPS					19			// 월드내 최대 PVP서버 수(81 ~ 99)
#define MAX_LOBBYS					1			// 월드내 최대 로비서버 수
#define MAX_WORLDSERVERS			MAX_CHANNELS + MAX_INDUNS + MAX_PVPS + MAX_LOBBYS
#define MAX_GAMESERVERS				MAX_CHANNELS + MAX_INDUNS + MAX_PVPS

// 기획에의해서 아이템 이름 최대크기 요구됨.(임시)
// DB 와 연관되어 있기 때문에 절대 임의 변경 불가 !!!!!
#define MAX_ITEM_NAME		30  

#define MAX_CHATROOM_TITLE_LEN		40
#define MAX_CHATROOM_ADVERT_LEN		50
#define MAX_CHATROOM_PASSWORD_LEN	4

#define MAX_FRIEND_LIST				99
#define MAX_BLOCK_LIST				99

// MailBox
#define	MAILBOX_MAX_ENTRY			40			// (수신) 메일함의 최대 메일 갯수
#define	MAILBOX_MAX_SUBJECT_LEN		50			// 메일 제목 최대 크기
#define	MAILBOX_MAX_MEMO_LEN		100			// 메일 본문 최대 크기
#define MAILBOX_SEND_FEE			50			// 메일 전송 수수료

#define KEEP_ALIVE_TIMEOUT			10000		// 10초

#define PACKET_MAGIC_NUMBER			0	

#define DECLARE_VAR(TYPE,VAR,LEN)	TYPE VAR[LEN+1]; memset(VAR,0,sizeof(VAR))
//#define ZEROVAR(VAR,SIZE)			memset(VAR,0,SIZE)
//#define ZERO_TCHAR(X,LEN)			memset(X,0,sizeof(TCHAR)*LEN)

#define DECLARE_TCHAR(VAR,LEN)		TCHAR VAR[LEN+1]; memset(VAR,0,sizeof(VAR))

#define BETWEEN(X,Y,Z)				Y = max(X,Y); Y = min(Y,Z)

#if 0
typedef unsigned short	WORLD_ID;
typedef WORLD_ID		REALM_ID;

typedef unsigned short	SERVER_ID;
typedef unsigned short	SERVER_PORT;

typedef unsigned char	RESULTCODE;

typedef unsigned long	USERID;
typedef unsigned long	PASSPORT;
typedef unsigned long	CHARID;

typedef unsigned long	ACTIONID;

typedef unsigned long	GLOBAL_STRING_ID;

#else

typedef UINT16		WORLD_ID;
typedef WORLD_ID	REALM_ID;

typedef UINT16		SERVER_ID;
typedef UINT16		SERVER_PORT;

typedef UINT8		RESULTCODE;


typedef UINT32		USERID;
typedef UINT32		PASSPORT;
typedef UINT32		CHARID;
typedef UINT32		ENTIRYID;


typedef UINT8		ACTIONID;

typedef UINT32		GLOBAL_STRING_ID;


#endif


//enum {GENDER_NONE,GENDER_MALE,GENDER_FEMALE,GENDER_BOTH};
enum GENDER
{
	GENDER_INVALID	= -1,
	GENDER_NULL		= 0,
	MALE			= 0x01,
	FEMALE			= 0x02,
	GENDER_BOTH		= MALE | FEMALE,
};

//#ifdef __CHANGE_CLASS
enum CLASS_TYPE {
	CLASS_NULL	= 0,
	CLASS_A		= 1,		// 공격형
	CLASS_B 	= 2,		// 방어형
	CLASS_C		= 3,		// 마법형
	CLASS_D		= 4,		// 정찰형
	CLASS_E		= 5,		// 워리어
	CLASS_F 	= 6,		// 광전사
	CLASS_G		= 7,		// 성기사
	CLASS_H		= 8,		// 몽크
	CLASS_I		= 9,		// 템플러
	CLASS_J 	= 10,		// 아티스트
	CLASS_K		= 11,		// 트레져헌터
	CLASS_L		= 12,		// 건너
	CLASS_M		= 13,		// 엔지니어(Base)
	CLASS_N		= 14,		// 엔지니어확장
	CLASS_MAX 
};
//#else
//enum CLASS_TYPE {
//	CLASS_NULL	= 0,
//	CLASS_A		= 1,		// 공격형
//	CLASS_B		= 2,		// 방어형
//	CLASS_C		= 3,		// 마법형
//	CLASS_D		= 4,		// 정찰형
//	CLASS_MAX 
//};
//#endif

const int CLASS_CHANGE_BASE = (CLASS_D+1);
enum VERSION_OPTION{KEEPALIVE=1,VERBOSE};

enum SAVEPOINT_TYPE // shift count
{
	SAVEPOINT_TYPE_TOWN,	// 마을.	
	SAVEPOINT_TYPE_SPOT,	// 기타포인트.
	SAVEPOINT_TYPE_MAX,
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Packet ID (UINT32)
//////////////////////////////////////////////////////////////////////////////
/*
enum PACKETID
{
	//////////////////////////////////////////////////////////////////////////////
	//
	// Subject1
	//
	
		SUBJECT1_BASE = 100,
		//---------------------------------------------------------------------------
		//	Server <-> Client
		//---------------------------------------------------------------------------

		//---------------------------------------------------------------------------
		//	Server <-> Server
		//---------------------------------------------------------------------------
}

*/

// 2006.08.07 dhpark 패킷정의 룰을 변경합니다.
#define PACKETID		UINT32
#define PI_CS_BASE		0				// 0				~	999,999,999		0부터 10억 -1 까지
#define PI_SS_BASE		1000000000		// 1,000,000,000	~	1,999,999,999	10억부터 20억 - 1 까지
#define PI_SYSTEM_BASE	0				// 0
#define PI_TOOL_BASE	100000000		// 1억
#define PI_LOGIC_BASE	200000000		// 2억
#define PI_SITE_BASE	300000000		// 3억

#define PI_CS_SYSTEM	PI_CS_BASE + PI_SYSTEM_BASE
#define PI_CS_TOOL		PI_CS_BASE + PI_TOOL_BASE
#define PI_CS_LOGIC		PI_CS_BASE + PI_LOGIC_BASE
#define PI_SS_SYSTEM	PI_SS_BASE + PI_SYSTEM_BASE
#define PI_SS_TOOL		PI_SS_BASE + PI_TOOL_BASE
#define PI_SS_LOGIC		PI_SS_BASE + PI_LOGIC_BASE
#define PI_SS_SITE		PI_SS_BASE + PI_SITE_BASE

// 내부의 Subject의 단위는 기본 100 단위 추천...
// 예)	PI_CS_SUBJECT3 = PI_CS_TOOL + 700,
//		PI_SS_SUBJECT3 = PI_SS_TOOL + 700,

/*
enum PACKETID_CS_SYSTEM
{
	//////////////////////////////////////////////////////////////////////////////
	//
	// Subject1
	//
	
		PI_CS_SUBJECT1 = PI_CS_SYSTEM + 100,

}

enum PACKETID_CS_TOOL
{
	//////////////////////////////////////////////////////////////////////////////
	//
	// Subject2
	//
	
		PI_CS_SUBJECT2 = PI_CS_TOOL + 100,

}

enum PACKETID_CS_LOGIC
{
	//////////////////////////////////////////////////////////////////////////////
	//
	// Subject3
	//
	
		PI_CS_SUBJECT3 = PI_CS_LOGIC + 100,

}


"PacketID4GM.h"
enum PACKETID_SS_SYSTEM
{
	//////////////////////////////////////////////////////////////////////////////
	//
	// Subject1
	//
	
		PI_SS_SUBJECT1 = PI_SS_SYSTEM + 100,

}

enum PACKETID_SS_TOOL
{
	//////////////////////////////////////////////////////////////////////////////
	//
	// Subject2
	//
	
		PI_SS_SUBJECT2 = PI_SS_TOOL + 100,

}

enum PACKETID_SS_LOGIC
{
	//////////////////////////////////////////////////////////////////////////////
	//
	// Subject3
	//
	
		PI_SS_SUBJECT3 = PI_SS_LOGIC + 100,

}

*/


enum PACKETID_CS_SYSTEM
{
	//////////////////////////////////////////////////////////////////////////////
	//
	// Echo (HeartBeat)
	//
		PI_CS_ECHO = PI_CS_SYSTEM + 0,
		
		ECHO_XX_PACKET = PI_CS_ECHO,
		KEEP_ALIVE_PACKET = PI_CS_ECHO, // 수신된 패킷을 가공없이 그대로 재전송한다.

	//////////////////////////////////////////////////////////////////////////////
	//
	// Protocol : Start & Version Check
	//
		PI_CS_PROTOCOL = PI_CS_SYSTEM + 100,
		
		// Step 1 : OnAccept이후 최초로 서버가 클라이언트에게 전송하는 패킷
		PROTOCOL_SC_START_REQ = PI_CS_PROTOCOL,				// nProtect (게임 가드 인증)
		// UINT8	uiStartDataLen;				// sizeof(GG_AUTH_DATA)		: 서버에서 키를 생성하여 클라이언트에 전송
		// TCHAR	szStartData[];				// GG_AUTU_DATA
		// UINT8	uiCipherMode;

		// Step 2 : 클라이언트의 응답
		PROTOCOL_CS_START_RES,				// 클라이언트에서는 게임 가드에 넘긴후, 결과값을 리턴.  서버상에서 이 값을 확인하여 게임 가드 상태를 인증한다. 
		// UINT8	uiStartDataLen;
		// TCHAR	szStartData[];

		// Step 3 : 이 시점부터 클라이언트는 패킷을 서버로 전송할 수 있다.
		PROTOCOL_SC_START,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT 상의 메세지를 itemID 값

		//PROTOCOL_CS_VERSION_CHECK,
		//// UINT16 major;
		//// UINT16 minor;
		//// UINT32 option;		// VERSION_OPTION 참고
		//		//option :

		//		//	1st bit : Keep-alive packet sending ON/OFF
		//		//	2nd bit : Verbose ON/OFF
		//		//	3rd bit : reserved
		//		//	...		: reserved

		//PROTOCOL_SC_VERSION_CHECK,
		//// RESULTCODE resultCode;			// LT_ERROR_VERSION_LOWER or LT_NO_ERROR or LT_ERROR_VERSION_UNSURPORTED_OPTION

		// 일반적인 패킷 관련 오류 발생
		SYSTEM_SC_ERROR,
		// GLOBAL_STRING_ID	itemID;	
		// UINT16	uiPacketLen;			// 오류발생 시점의 패킷 정보
		// UINT16	uiPacketKey;
		// UINT32	uiPacketID;

		PROTOCOL_SC_START_DYNCODE_1,			// for SNDC (Step 1)
		// UINT32	iDynCodeLen;
		// BYTE		szDynCode[];

		PROTOCOL_SC_START_DYNCODE_2,			// for SNDC (Step 2)
		// UINT32	iDynCodeLen;
		// BYTE		szDynCode[];


		PROTOCOL_SC_CHECK_CLIENT_VALIDATION_REQ,
		// UINT32	uiValue1
		// UINT32	uiValue2
		// UINT32	uiValue3
		// UINT32	uiValue4

		PROTOCOL_CS_CHECK_CLIENT_VALIDATION_RES,
		// UINT32	uiValue1
		// UINT32	uiValue2
		// UINT32	uiValue3
		// UINT32	uiValue4

		// DNS Server 도입 site에서의 '서버 변경' 기능 이용시 LS로의 재연결 보장 목적
		PROTOCOL_SC_LOGINSERVERINFO,
		// UINT16	uiServerID;
		// TCHAR	szIP[LEN_IP];
		// UINT16	usPort;


	//////////////////////////////////////////////////////////////////////////////
	//
	// World Environment (DATETIME/Weather,...) (GameServer)
	//
		PI_CS_WORLDENV = PI_CS_SYSTEM + 200,

		// 월드 타임 설정
		WORLDENV_SC_DATETIME = PI_CS_WORLDENV,
		// UINT8	year;
		// UINT8	month;
		// UINT8	day;
		// UINT8	hour;
		// UINT8	minute;
		// UINT8	second;

		WORLDENV_SC_WEATHER ,
		// UINT8	weatherType;		// Sunny,rainy,windy,cloudy,...

	//////////////////////////////////////////////////////////////////////////////
	//
	// 중복로그인검사
	//
		PI_CS_USER = PI_CS_SYSTEM + 300,

		// 게임서버에 유저가 로그인한다고 알립니다.(로그인, 로비, 게임)
		USER_CS_IN = PI_CS_USER,
		// UINT32			uiUserID;
		// PASSPORT			uiPassport;
		// TCHAR			szIP[LEN_IP+1];

		// 클라이언트에 중복검사 결과를 송신합니다.
		USER_SC_IN,
		// GLOBAL_STRING_ID	uiErrorID;
		// PASSPORT			uiPassport;

		// 게임서버에 유저가 이동한다고 알립니다.(로그인->로비, 로비->게임, 게임->게임, 게임->로그인, 로비->로그인)
		USER_CS_MOVE,
		// UINT16			wDstServerID;

		// 클라이언트에 이동 결과를 송신합니다.
		USER_SC_MOVE,
		// GLOBAL_STRING_ID	uiErrorID;

	//////////////////////////////////////////////////////////////////////////////
	//
	// Cheat 
	//
		PI_CS_CHEAT = PI_CS_SYSTEM + 400,

		// 현재 위치를 주어진 곳으로 지정(워프됨) for cheating
		//   서버는 NAV_SC_MOVE를 브로드캐스팅
		CHEAT_CS_SETPOSITION = PI_CS_CHEAT,
		// TCHAR			charName[LEN_NAME];
		// LT_POSITION		pos;

		// GM의 cheat mode 지원 목적
		CHEAT_CS_COMMAND ,
		// UINT8			len;
		// TCHAR			szCheatCommand[];			//<CheatCommand><whitespace><ParamList>


	//////////////////////////////////////////////////////////////////////////////
	//
	// Notification
	//
		NOTI_SC_BASE = PI_CS_SYSTEM + 500,

		// 알리미 서비스(친구,...)
		NOTI_SC_ONLINE_STATUS_FRIEND = NOTI_SC_BASE,
		// UINT8		uiNotiType;				// 1(NotiType_Friend_OnlineStatus)
		// UINT8		uiStatus;				// 1(CH_IN),2(CH_OUT) [,3(CH_MOVE),4(ETC)]
		// UINT16		uiServerID;				// 현재 위치한 서버
		// UINT32		uiActorCharID;
		// UINT8		len;
		// TCHAR		szActorCharName[];


		// 등록된 친구의 online상태 변경 사항을 알림. (ON,OFF)
		NOTI_MG_FRIEND_STATUS,
		// UINT8		uiNotiType,				// 1(NotiType_Friend_OnlineStatus)
		// UINT8		uiStatus,				// 1(CH_IN),2(CH_OUT) [,3(CH_MOVE),4(ETC)]
		// UINT32		uiCharID;				// Notification를 수신할 대상
		// UINT32		uiNotiActorCharID;		// 해당 Notification의 대상
		// UINT16		uiServerID;				// Actor가 위치한 서버
		// UINT8		len;
		// TCHAR		szActorCharName[];

		// CHANNEL_CS_IN 상태에서 친구목록을 등록함.
		// 혹시 해당 캐릭터의 친구 목록이 존재하면 기존 목록은 모두 삭제하고 새로 등록함
		NOTI_GM_FRIEND_REGISTER_LIST,
		// UINT32		uiCharID;
		// UINT8		nCount;
		// UINT32		uiFriendCharID[];

		// CHANNEL_CS_OUT 상태에서 uiCharID의 친구목록을 등록 해제함.
		NOTI_GM_FRIEND_UNREGISTER_LIST,
		// UIINT32		uiCharID;

		// 친구 목록에 추가로 인한 관리목록상에 추가
		NOTI_GM_FRIEND_ADD,
		// UINT32		uiCharID;
		// UINT32		uiFriendCharID;

		// 친구 목록에서 삭제하여 관리목록상에서 삭제
		NOTI_GM_FRIEND_DELETE,
		// UINT32		uiCharID;
		// UINT32		uiFriendCharID;

	//////////////////////////////////////////////////////////////////////////////
	//
	// SubLogic
	//
		SUBLOGIC_CS_BASE = PI_CS_SYSTEM + 600,

		SUBLOGIC_SC_COMMAND = SUBLOGIC_CS_BASE,
		// UINT8		uiLen;
		// TCHAR		szCommand[];

	//////////////////////////////////////////////////////////////////////////////
	//
	// Fatigue Degree System
	//

		FDS_CS_BASE = PI_CS_SYSTEM + 700,

		FDS_SC_NOTICE = FDS_CS_BASE,
		// UINT32		uiOnlineMinutes;				// 접속시간
		// UINT32		uiOfflineMinutes;				// 비접속시간
		// UINT16		usRate_Ely;				
		// UINT16		usRate_Exp;						// 현재 적용된 경험치 관련 factor
		// UINT16		usRate_ItemDrop;				// 현재 적용된 아이템 드랍률 관련 factor

		// 유저가 자신의 접속상태를 확인하는 용도로 사용. FDS_SC_NOTICE 리플라이됨
		FDS_CS_QUERYSTATE,
		// UINT32		uiReservedParam1;
		// UINT32		uiReservedParam2;


	//////////////////////////////////////////////////////////////////////////////
	//
	// Security System : 클라이언트 보안툴 검사
	//

		SECURITY_CS_BASE = PI_CS_SYSTEM + 800,

		// 서버에서 Query()를 통해서 생성된 검증용 데이터 송신
		SECURITY_SC_CHECK = SECURITY_CS_BASE,
		// SECURITY_PACKDATA	stPackData;				// 보안툴 검증용 데이터

		// 클라이언트에서 보안툴을 이용해서 검증한 데이터 수신
		SECURITY_CS_CHECK,
		// SECURITY_PACKDATA	stPackData;				// 보안툴 검증용 데이터

		// 보안툴 검사 결과 통보. 메세지창을 통해서 클라이언트 재시작을 유도합니다.
		SECURITY_SC_INFO,
		// GLOBAL_STRING_ID		ErrorID;				// 에러아이디

};

enum PACKETID_CS_TOOL
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	GMLOGIN
	//
		PI_CS_GMLOGIN = PI_CS_TOOL + 0,

		// 툴서버에 로그인
		TOOL_CS_LOGIN = PI_CS_GMLOGIN,
		// char 			szID[MAX_LEN_ID_DB+1];
		// char 			szPW[MAX_LEN_PW_DB+1];
		// int				iVersion;		// GM툴이 사용하는 버젼번호
		// UINT16			major;			// 서버&클라이언트가 사용하는 버젼중 메이져
		// UINT16			minor;			// 서버&클라이언트가 사용하는 버젼중 마이너
		// UINT32			option;			// VERSION_OPTION 참고

		// 로그인에 대한 응답
		TOOL_SC_LOGIN,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// UINT8 byPermit;					// 권한
		// char	szName[LEN_NAME+1];			// 이름

		TOOL_CS_LOGIN2,
		// UINT8			lenID;
		// TCHAR			ID[];
		// TCHAR			cMagicChar[1];	// must be MAGIC_CHAR
		// UINT8			lenPW;
		// TCHAR			PW[];
		// int				iVersion;		// GM툴이 사용하는 버젼번호
		// UINT16			major;			// 서버&클라이언트가 사용하는 버젼중 메이져
		// UINT16			minor;			// 서버&클라이언트가 사용하는 버젼중 마이너
		// UINT32			option;			// VERSION_OPTION 참고

	//////////////////////////////////////////////////////////////////////////
	//
	//	GMNOTICE
	//
		PI_CS_GMNOTICE = PI_CS_TOOL + 100,

		// 툴에서 공지사항을 툴서버에 송신
		TOOL_CS_NOTICE = PI_CS_GMNOTICE,
		// UINT16 worldID;					// 0(all)
		// UINT16 serverID;					// 0(all)
		// UINT8 displayTime;				// 0(forever), sec
		// UINT8 optionFlags;				// reserved. display 효과 용도(캄박임,칼라,...)
		// UINT16 uiLen;
		// TCHAR szMsg[];

	//////////////////////////////////////////////////////////////////////////
	//
	//	GMWORLDLIST
	//
		PI_CS_GMWORLDLIST = PI_CS_TOOL + 200,

		// 월드리스트를 가져옵니다.
		TOOL_CS_WORLDLIST = PI_CS_GMWORLDLIST,

		// 월드리스트를 가져옵니다.
		TOOL_SC_WORLDLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iCnt;
		// GM_WORLD			stGM_World;
		// ...

	//////////////////////////////////////////////////////////////////////////
	//
	//	USERTAB
	//
		PI_CS_USERTAB = PI_CS_TOOL + 300,
	
		// -- 사라질 패킷
		// MServer의 온라인유저를 삭제합니다.(유저관리가 제대로 안되어 남아있는 경우에 대한 처리)
		TOOL_CS_OFFLINE = PI_CS_USERTAB,
		// UINT32 uiUserID;

		// -- 사라질 패킷
		// 온라인유저 삭제 결과
		TOOL_SC_OFFLINE,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

		// 게임서버내의 유저를 추방합니다.
		TOOL_CS_KICKOUT,
		// UINT32 uiUserID;

		// 유저추방 결과
		TOOL_SC_KICKOUT,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// 계정의 블록을 수정합니다.
		TOOL_CS_UPDATE_USER_BLOCK,
		// UINT32			uiUserID;		// 유저아이디(유저의 디비키)
		// UINT8			byBlock;		// 블록여부

		// 계정의 블록을 수정합니다.
		TOOL_SC_UPDATE_USER_BLOCK,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// 창고머니를 수정합니다.
		TOOL_CS_UPDATE_STORAGE_MONEY,
		// int				iWID;			// 월드아이디
		// UINT32			uiUserID;		// 유저아이디(유저의 디비키)
		// INT64			iStorageMoney;	// 창고돈

		// 창고머니를 수정합니다.
		TOOL_SC_UPDATE_STORAGE_MONEY,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

		// 유저를 검색합니다.
		TOOL_CS_FINDUSER,
		// UINT32 uiUserID;

		// 유저검색 결과
		TOOL_SC_FINDUSER,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// UINT16 usServerID;				// 서버아이디(플레이중인 서버위치)
		// UINT8 byStatus;					// 유저상태(IN, MOVE(이동시작), OUT(이동을위해서종료))
		// UINT8 byValid;					// 게임서버에 실제로 존재여부(0:X, 1:존재)
		// char szCharName[LEN_NAME+1];		// 채널서버에서 플레이중일때 선택된 캐릭터명
	
		// 캐릭터삭제횟수를 수정합니다.
		TOOL_CS_UPDATE_CHARDELCOUNT,
		// int				iWID;			// 월드아이디
		// UINT32			uiUserID;		// 유저아이디(유저의 디비키)
		// UINT8			byCharDelCount;	// 캐릭터삭제횟수

		// 캐릭터삭제횟수를 수정합니다.
		TOOL_SC_UPDATE_CHARDELCOUNT,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

	//////////////////////////////////////////////////////////////////////////
	//
	//	PCTAB
	//
		PI_CS_PCTAB = PI_CS_TOOL + 400,
	
		// AccountName으로 PC리스트를 가져옵니다.
		TOOL_CS_PCLISTBYACCOUNT = PI_CS_PCTAB,
		// int				iWID;			// 월드아이디
		// char				szAccountName[MAX_LEN_ID_DB+1];

		// AccountName으로 PC리스트를 가져옵니다.
		TOOL_SC_PCLISTBYACCOUNT,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// UINT32			uiUserID;		// 유저아이디(유저의 디비키)
		// UINT8			byBlock;		// 블록
		// INT64			iStorageMoney;	// 창고돈
		// char				szAccountName[MAX_LEN_ID_DB+1];
		// int				iCnt;
		// GM_PC			stGM_PC;
		// ...
		// UINT8			byCharDelCount;	// 캐릭터삭제횟수
	
		// CharName으로 PC리스트를 가져옵니다.
		TOOL_CS_PCLISTBYCHAR,
		// int				iWID;			// 월드아이디
		// char				szCharName[LEN_NAME+1];

		// CharName으로 PC리스트를 가져옵니다.
		TOOL_SC_PCLISTBYCHAR,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// UINT32			uiUserID;		// 유저아이디(유저의 디비키)
		// UINT8			byBlock;		// 블록
		// INT64			iStorageMoney;	// 창고돈
		// char				szAccountName[MAX_LEN_ID_DB+1];
		// int				iCnt;
		// GM_PC			stGM_PC;
		// ...
		// UINT8			byCharDelCount;	// 캐릭터삭제횟수
	
		// PC의 권한을 수정합니다.
		TOOL_CS_UPDATE_PC_PERMISSION,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키
		// UINT8			byPermission;	// 권한

		// PC의 권한을 수정합니다.
		TOOL_SC_UPDATE_PC_PERMISSION,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// PC의 이름을 수정합니다.
		TOOL_CS_UPDATE_PC_CHARNAME,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키
		// char				szOldCharName[LEN_NAME+1];
		// char				szNewCharName[LEN_NAME+1];

		// PC의 이름을 수정합니다.
		TOOL_SC_UPDATE_PC_CHARNAME,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// PC의 정보들을 수정합니다.
		TOOL_CS_UPDATE_PC,
		// int				iWID;			// 월드아이디
		// GM_PC			stGM_PC;		// PC정보

		// PC의 정보들을 수정합니다.
		TOOL_SC_UPDATE_PC,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// PC를 삭제 또는 복구합니다.
		TOOL_CS_UPDATE_PC_DEL,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키
		// UINT8			byDel;			// 삭제여부

		// PC를 삭제 또는 복구합니다.
		TOOL_SC_UPDATE_PC_DEL,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// PC의 타이틀아이디를 수정합니다.
		TOOL_CS_UPDATE_PC_TITLEID,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키
		// int				iTitleID;		// 타이틀아이디

		// PC의 타이틀아이디를 수정합니다.
		TOOL_SC_UPDATE_PC_TITLEID,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

	//////////////////////////////////////////////////////////////////////////
	//
	//	ITEMTAB
	//
		PI_CS_ITEMTAB = PI_CS_TOOL + 500,

		// 컨테이너 타입별 아이템리스트/확장슬롯을 가져옵니다.
		TOOL_CS_ITEMLIST = PI_CS_ITEMTAB,
		// int				iWID;			// 월드아이디
		// UINT32			uiUserID;		// 유저아이디(유저의 디비키)
		// int				iCharID;		// PC의 DB키
		// UINT8			byContainerType;// 컨테이너타입

		// 컨테이너 타입별 아이템리스트/확장슬롯을 가져옵니다.
		TOOL_SC_ITEMLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// UINT8			byContainerType;// 컨테이너타입
		// UINT8			byExpandSlot;	// 확장슬롯수
		// int				iCnt;
		// GM_ITEM			stGM_Item;
		// ...
	
		// 확장슬롯을 수정합니다.
		TOOL_CS_UPDATE_EXPANDSLOT,
		// int				iWID;			// 월드아이디
		// UINT32			uiUserID;		// 유저아이디(유저의 디비키)
		// int				iCharID;		// PC의 DB키
		// UINT8			byContainerType;// 컨테이너타입
		// UINT8			byExpandSlot;	// 확장슬롯수

		// 확장슬롯을 수정합니다.
		TOOL_SC_UPDATE_EXPANDSLOT,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// 아이템의 정보들을 수정합니다.
		TOOL_CS_UPDATE_ITEM,
		// int				iWID;			// 월드아이디
		// GM_ITEM			stGM_ITEM;		// ITEM정보

		// 아이템의 정보들을 수정합니다.
		TOOL_SC_UPDATE_ITEM,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// 아이템을 생성합니다.
		TOOL_CS_CREATE_ITEM,
		// int				iWID;			// 월드아이디
		// GM_ITEM			stGM_ITEM;		// ITEM정보

		// 아이템을 생성합니다.
		TOOL_SC_CREATE_ITEM,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// INT64			iItemNo;		// 아이템DB키
	
		// 아이템을 삭제합니다.
		TOOL_CS_DELETE_ITEM,
		// int				iWID;			// 월드아이디
		// INT64			iItemNo;		// 아이템DB키

		// 아이템을 삭제합니다.
		TOOL_SC_DELETE_ITEM,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

	//////////////////////////////////////////////////////////////////////////
	//
	//	SKILLTAB
	//
		PI_CS_SKILLTAB = PI_CS_TOOL + 600,
	
		// 스킬리스트를 가져옵니다.
		TOOL_CS_SKILLLIST = PI_CS_SKILLTAB,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키

		// 스킬리스트를 가져옵니다.
		TOOL_SC_SKILLLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iCnt;
		// GM_SKILL			stGM_Skill;
		// ...
	
		// 스킬정보를 수정합니다.
		TOOL_CS_UPDATE_SKILL,
		// int				iWID;			// 월드아이디
		// GM_SKILL			stGM_Skill;

		// 스킬정보를 수정합니다.
		TOOL_SC_UPDATE_SKILL,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// 스킬을 생성합니다.
		TOOL_CS_CREATE_SKILL,
		// int				iWID;			// 월드아이디
		// GM_SKILL			stGM_Skill;

		// 스킬을 생성합니다.
		TOOL_SC_CREATE_SKILL,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// 스킬을 삭제합니다.
		TOOL_CS_DELETE_SKILL,
		// int				iWID;			// 월드아이디
		// GM_SKILL			stGM_Skill;

		// 스킬을 삭제합니다.
		TOOL_SC_DELETE_SKILL,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

	//////////////////////////////////////////////////////////////////////////
	//
	//	QUESTTAB
	//
		PI_CS_QUESTTAB = PI_CS_TOOL + 700,

		// 퀘스트리스트를 가져옵니다.
		TOOL_CS_QUESTLIST = PI_CS_QUESTTAB,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키

		// 퀘스트리스트를 가져옵니다.
		TOOL_SC_QUESTLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iCnt;
		// GM_QUEST			stGM_Quest;
		// ...
	
		// 퀘스트정보를 수정합니다.
		TOOL_CS_UPDATE_QUEST,
		// int				iWID;			// 월드아이디
		// GM_QUEST			stGM_Quest;

		// 퀘스트정보를 수정합니다.
		TOOL_SC_UPDATE_QUEST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// 퀘스트을 생성합니다.
		TOOL_CS_CREATE_QUEST,
		// int				iWID;			// 월드아이디
		// GM_QUEST			stGM_Quest;

		// 퀘스트을 생성합니다.
		TOOL_SC_CREATE_QUEST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// 퀘스트을 삭제합니다.
		TOOL_CS_DELETE_QUEST,
		// int				iWID;			// 월드아이디
		// GM_QUEST			stGM_Quest;

		// 퀘스트을 삭제합니다.
		TOOL_SC_DELETE_QUEST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

	//////////////////////////////////////////////////////////////////////////
	//
	//	MISSIONTAB
	//
		PI_CS_MISSIONTAB = PI_CS_TOOL + 800,
	
		// 미션리스트를 가져옵니다.
		TOOL_CS_MISSIONLIST = PI_CS_MISSIONTAB,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키

		// 미션리스트를 가져옵니다.
		TOOL_SC_MISSIONLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iCnt;
		// GM_MISSION		stGM_Mission;
		// ...
	
		// 미션정보를 수정합니다.
		TOOL_CS_UPDATE_MISSION,
		// int				iWID;			// 월드아이디
		// GM_MISSION		stGM_Mission;

		// 미션정보를 수정합니다.
		TOOL_SC_UPDATE_MISSION,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// 미션을 생성합니다.
		TOOL_CS_CREATE_MISSION,
		// int				iWID;			// 월드아이디
		// GM_MISSION		stGM_Mission;

		// 미션을 생성합니다.
		TOOL_SC_CREATE_MISSION,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
	
		// 미션을 삭제합니다.
		TOOL_CS_DELETE_MISSION,
		// int				iWID;			// 월드아이디
		// GM_MISSION		stGM_Mission;

		// 미션을 삭제합니다.
		TOOL_SC_DELETE_MISSION,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

	//////////////////////////////////////////////////////////////////////////
	//
	//	MAILTAB
	//
		PI_CS_MAILTAB = PI_CS_TOOL + 900,
	
		// 메일리스트를 가져옵니다.
		TOOL_CS_MAILLIST = PI_CS_MAILTAB,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키

		// 메일리스트를 가져옵니다.
		TOOL_SC_MAILLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iCnt;
		// GM_MAIL			stGM_Mail;
		// ...
	
		// 메일을 삭제합니다.
		TOOL_CS_DELETE_MAIL,
		// int				iWID;			// 월드아이디
		// GM_MAIL			stGM_Mail;

		// 메일을 삭제합니다.
		TOOL_SC_DELETE_MAIL,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

		// 메일을 업데이트 합니다.
		TOOL_CS_UPDATE_MAIL,
		// int				iWID;			// 월드아이디
		// GM_MAIL			stGM_Mail;

		// 메일을 업데이트 합니다.
		TOOL_SC_UPDATE_MAIL,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iCnt;
		// GM_MAIL			stGM_Mail;
		

	//////////////////////////////////////////////////////////////////////////
	//
	//	GIFT
	//
		PI_CS_GIFTTAB = PI_CS_TOOL + 1000,
	
		// 메일리스트를 가져옵니다.
		TOOL_CS_GIFTLIST = PI_CS_GIFTTAB,
		// int				iWID;			// 월드아이디
		// char				szAccountName[LEN_ID+1];

		// 메일리스트를 가져옵니다.
		TOOL_SC_GIFTLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iCnt;
		// GM_GIFT			stGM_Gift;
		// ...

	//////////////////////////////////////////////////////////////////////////
	//
	//	EFFECTTAB
	//
		PI_CS_EFFECTTAB = PI_CS_TOOL + 1100,
	
		// 이펙트리스트를 가져옵니다.
		TOOL_CS_EFFECTLIST = PI_CS_EFFECTTAB,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키

		// 이펙트리스트를 가져옵니다.
		TOOL_SC_EFFECTLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iCnt;
		// GM_EFFECT		stGM_Effect;
		// ...
	
		// 이펙트를 삭제합니다.
		TOOL_CS_DELETE_EFFECT,
		// int				iWID;			// 월드아이디
		// GM_EFFECT		stGM_Effect;

		// 이펙트를 삭제합니다.
		TOOL_SC_DELETE_EFFECT,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

	//////////////////////////////////////////////////////////////////////////
	//
	//	TITLETAB
	//
		PI_CS_TITLETAB = PI_CS_TOOL + 1200,
	
		// 타이틀리스트를 가져옵니다.
		TOOL_CS_TITLELIST = PI_CS_TITLETAB,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키

		// 타이틀리스트를 가져옵니다.
		TOOL_SC_TITLELIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iCnt;
		// GM_TITLE			stGM_Title;
		// ...
		
		// 타이틀을 생성합니다.
		TOOL_CS_CREATE_TITLE,
		// int				iWID;			// 월드아이디
		// GM_TITLE			stGM_Title;

		// 타이틀을 생성합니다.
		TOOL_SC_CREATE_TITLE,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

		// 타이틀을 삭제합니다.
		TOOL_CS_DELETE_TITLE,
		// int				iWID;			// 월드아이디
		// GM_TITLE			stGM_Title;

		// 타이틀을 삭제합니다.
		TOOL_SC_DELETE_TITLE,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

	//////////////////////////////////////////////////////////////////////////
	//
	//	WAYPOINTTAB
	//
		PI_CS_WAYPOINTTAB = PI_CS_TOOL + 1300,
	
		// 웨이포인트리스트를 가져옵니다.
		TOOL_CS_WAYPOINTLIST = PI_CS_WAYPOINTTAB,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키

		// 웨이포인트리스트를 가져옵니다.
		TOOL_SC_WAYPOINTLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// GM_WAYPOINT		stGM_WayPoint;

		//웨이포인트 저장 수정 요청합니다
		TOOL_CS_WAYPOINTSAVE,
		// int				iWID;			// 월드아이디
		// GM_EVENT			stGM_WayPoint

		//웨이포인트 저장 수정 요청합니다
		TOOL_SC_WAYPOINTSAVE,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// GM_WAYPOINT		stGM_WayPoint;

	//////////////////////////////////////////////////////////////////////////
	//
	//	ACCOUNTTAB
	//
		PI_CS_ACCOUNTTAB = PI_CS_TOOL + 1400,
	
		// GM계정리스트를 가져옵니다.
		TOOL_CS_ACCOUNTLIST = PI_CS_ACCOUNTTAB,

		// GM계정리스트를 가져옵니다.
		TOOL_SC_ACCOUNTLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iCnt;
		// GM_ACCOUNT		stGM_Account;
		// ...
		
		// GM계정을 생성합니다.
		TOOL_CS_CREATE_ACCOUNT,
		// GM_ACCOUNT		stGM_Account;

		// GM계정을 생성합니다.
		TOOL_SC_CREATE_ACCOUNT,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iGMNum;			// GM계정번호
		
		// GM계정을 수정합니다.
		TOOL_CS_UPDATE_ACCOUNT,
		// GM_ACCOUNT		stGM_Account;

		// GM계정을 수정합니다.
		TOOL_SC_UPDATE_ACCOUNT,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

	//////////////////////////////////////////////////////////////////////////
	//
	//	GMCASHSHOP
	//
		PI_CS_GMCASHSHOP = PI_CS_TOOL + 1500,

		// 툴에서 공지사항을 툴서버에 송신
		TOOL_CS_CASHSHOP = PI_CS_GMCASHSHOP,
		// UINT16 worldID;					// 0(all)
		// UINT16 serverID;					// 0(all)
		// UINT8 optionFlags;				// 0: 캐쉬샵 닫기 1: 버전 갱신 2: 캐쉬샵 열기 

		TOOL_SC_CASHSHOP	,
		// UINT16 worldID;					// 0(all)
		// UINT16 serverID;					// 0(all)
		// UINT8 optionFlags;				// 0: 캐쉬샵 닫기 1: 버전 갱신 2: 캐쉬샵 열기 
		// UINT8 uiResult;					// 에러코드

	//////////////////////////////////////////////////////////////////////////
	//
	//	GUILDTAB
	//
		PI_CS_GUILDTAB = PI_CS_TOOL + 1600,
	
		// 길드정보를 가져옵니다.
		TOOL_CS_GUILD = PI_CS_GUILDTAB,
		// UINT32				iWID;			// 월드아이디
		// TCHAR				szCharName[MAX_LEN_NAME_DB+1];	// 캐릭터명

		// 길드정보를 가져옵니다.
		TOOL_SC_GUILD,
		// GLOBAL_STRING_ID		uiErrorID;		// 에러코드
		// GM_GUILD				stGM_Guild;		// 길드정보

		// 길드를 삭제합니다.(길드멤버도 삭제합니다.)
		TOOL_CS_DELETE_GUILD,
		// UINT32				iWID;			// 월드아이디
		// UINT32				iGNum;			// 길드의 DB키

		// 길드를 삭제합니다.(길드멤버도 삭제합니다.)
		TOOL_SC_DELETE_GUILD,
		// GLOBAL_STRING_ID		uiErrorID;		// 에러코드

		// 길드정보중 길드명을 수정합니다.
		TOOL_CS_UPDATE_GUILD_GNAME,
		// UINT32				iWID;			// 월드아이디
		// UINT32				iGNum;			// 길드의 DB키
		// TCHAR				szGName[GUILD_MAX_NAME+1];	// 길드명

		// 길드정보중 길드명을 수정합니다.
		TOOL_SC_UPDATE_GUILD_GNAME,
		// GLOBAL_STRING_ID		uiErrorID;		// 에러코드

		// 길드정보중 길드슬로건을 수정합니다.
		TOOL_CS_UPDATE_GUILD_GSLOGAN,
		// UINT32				iWID;			// 월드아이디
		// UINT32				iGNum;			// 길드의 DB키
		// TCHAR				szGSlogan[GUILD_MAX_SLOGAN+1];	// 길드슬로건

		// 길드정보중 길드슬로건을 수정합니다.
		TOOL_SC_UPDATE_GUILD_GSLOGAN,
		// GLOBAL_STRING_ID		uiErrorID;		// 에러코드

		// 길드정보중 묘목아이디를 수정합니다.
		TOOL_CS_UPDATE_GUILD_CROPID,
		// UINT32				iWID;			// 월드아이디
		// UINT32				iGNum;			// 길드의 DB키
		// UINT32				iCropID;		// 묘목아이디

		// 길드정보중 묘목아이디를 수정합니다.
		TOOL_SC_UPDATE_GUILD_CROPID,
		// GLOBAL_STRING_ID		uiErrorID;		// 에러코드

		// 길드정보중 묘목성장완료까지 남은시간을 수정합니다.
		TOOL_CS_UPDATE_GUILD_REMAINTIME,
		// UINT32				iWID;			// 월드아이디
		// UINT32				iGNum;			// 길드의 DB키
		// UINT32				iRemainTime;	// 묘목성장완료까지 남은시간

		// 길드정보중 묘목성장완료까지 남은시간을 수정합니다.
		TOOL_SC_UPDATE_GUILD_REMAINTIME,
		// GLOBAL_STRING_ID		uiErrorID;		// 에러코드

		// 길드정보중 투여된 비료시간을 수정합니다.
		TOOL_CS_UPDATE_GUILD_NOURISHTIME,
		// UINT32				iWID;			// 월드아이디
		// UINT32				iGNum;			// 길드의 DB키
		// UINT32				iNourishTime;	// 투여된 비료시간

		// 길드정보중 투여된 비료시간을 수정합니다.
		TOOL_SC_UPDATE_GUILD_NOURISHTIME,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

		// 길드정보중 비료투여된 시작시간을 수정합니다.
		TOOL_CS_UPDATE_GUILD_NOURISHSTARTTIME,
		// UINT32				iWID;				// 월드아이디
		// UINT32				iGNum;				// 길드의 DB키
		// TIMESTAMP_STRUCT		stNourishStartTime;	// 비료투여된 시작시간

		// 길드정보중 비료투여된 시작시간을 수정합니다.
		TOOL_SC_UPDATE_GUILD_NOURISHSTARTTIME,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

		// 길드정보중 투여된 촉진제시간을 수정합니다.
		TOOL_CS_UPDATE_GUILD_HASTENSUMTIME,
		// UINT32				iWID;			// 월드아이디
		// UINT32				iGNum;			// 길드의 DB키
		// UINT32				iHastenSumTime;	// 투여된 촉진제시간

		// 길드정보중 투여된 촉진제시간을 수정합니다.
		TOOL_SC_UPDATE_GUILD_HASTENSUMTIME,
		// GLOBAL_STRING_ID		uiErrorID;		// 에러코드

		// 길드정보중 인던입장용 맵이벤트아이디를 수정합니다.
		TOOL_CS_UPDATE_GUILD_INDUNEVENTID,
		// UINT32				iWID;			// 월드아이디
		// UINT32				iGNum;			// 길드의 DB키
		// UINT32				iIndunEventID;	// 인던입장용 맵이벤트아이디

		// 길드정보중 인던입장용 맵이벤트아이디를 수정합니다.
		TOOL_SC_UPDATE_GUILD_INDUNEVENTID,
		// GLOBAL_STRING_ID		uiErrorID;		// 에러코드


		// 길드멤버리스트를 가져옵니다.
		TOOL_CS_GUILDMEMBERLIST,
		// UINT32				iWID;			// 월드아이디
		// UINT32				iGNum;			// 길드의 DB키

		// 길드멤버리스트를 가져옵니다.
		TOOL_SC_GUILDMEMBERLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// UINT32				iCnt;
		// GM_GUILDMEMBER		stGM_GuildMember;
		// ...

		// 길드멤버를 삭제합니다.
		TOOL_CS_DELETE_GUILDMEMBER,
		// UINT32				iWID;			// 월드아이디
		// UINT32				iCharID;		// PC의 DB키

		// 길드멤버를 삭제합니다.
		TOOL_SC_DELETE_GUILDMEMBER,
		// GLOBAL_STRING_ID		uiErrorID;		// 에러코드

		// 길드멤버정보중 직위를 수정합니다.
		TOOL_CS_UPDATE_GUILDMEMBER_GRADE,
		// UINT32				iWID;			// 월드아이디
		// UINT32				iCharID;		// PC의 DB키
		// UINT8				byGrade;		// 직위

		// 길드멤버정보중 직위를 수정합니다.
		TOOL_SC_UPDATE_GUILDMEMBER_GRADE,
		// GLOBAL_STRING_ID		uiErrorID;		// 에러코드

		// 길드타입을 변경합니다
		TOOL_CS_UPDATE_GUILD_GTYPE,
		// UINT32				iWID;			// 월드아이디
		// UINT32				iGNum;			// 길드의 DB키

		// 길드타입을 변경합니다
		TOOL_SC_UPDATE_GUILD_GTYPE,
		// GLOBAL_STRING_ID		uiErrorID;		// 에러코드

		// Guild End-Mark
		PI_CS_GUILDTAB_END,


	//////////////////////////////////////////////////////////////////////////
	//
	//	LDT2DB TAB
	//
		TOOL_CS_LDT2DB_BASE = PI_CS_TOOL + 1700,

		TOOL_CS_LDT2DB_SELECT = TOOL_CS_LDT2DB_BASE,
		// UINT32				uiTableType;				// 1(MapInfo),2(GiftInfo),3(ItemInfo)
		TOOL_SC_LDT2DB_SELECT,
		// GLOBAL_STRING_ID		uiErrorID;					// 에러코드
		// UINT32				uiTableType;			
		// UINT32				uiLen;
		// TCHAR				szData[];

		// MapInfo (DELETE)
		TOOL_CS_LDT2DB_DELETE,
		// UINT32				uiTableType;			
		TOOL_SC_LDT2DB_DELETE,
		// GLOBAL_STRING_ID		uiErrorID;		
		// UINT32				uiTableType;			

		// MapInfo (UPDATE)
		TOOL_CS_LDT2DB_UPDATE,
		// UINT32				uiTableType;			
		// UINT32				uiLen;
		// TCHAR				szData[];
		TOOL_SC_LDT2DB_UPDATE,
		// GLOBAL_STRING_ID		uiErrorID;		
		// UINT32				uiTableType;			

		// LDT2DB End-Mark
		TOOL_CS_LDT2DB_END,

	//////////////////////////////////////////////////////////////////////////
	//
	//	ABILITYTAB
	//
		PI_CS_ABILITYTAB = PI_CS_TOOL + 1800,

		//조합정보를 가져옵니다
		TOOL_CS_MIXINFO = PI_CS_ABILITYTAB,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키

		//조합정보를 가져옵니다
		TOOL_SC_MIXINFO,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iProfessionLv_1;//조합레벨
		// int				iProfessionExp_1;//조합경험치

		//조합정보를 수정합니다
		TOOL_CS_MIXINFO_UPDATE,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키	
		// int				iCurMixLv;		// 변경된 조합레벨
		// int				iCurMixExp;		// 변경된 조합경험치

		//조합정보를 수정합니다
		TOOL_SC_MIXINFO_UPDATE,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

		//PVP 정보를 가져옵니다
		TOOL_CS_PVPINFO,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키

		//PVP 정보를 가져옵니다
		TOOL_SC_PVPINFO,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// int				iPVPLv;			// PVP레벨
		// int				iPVPGlory;		// PVP경험치
		// int				iPVPPoint;		// PVP포인트

		//PVP 정보를 수정합니다
		TOOL_CS_PVPINFO_UPDATE,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키
		// int				iPVPLv;			// PVP레벨
		// int				iPVPGlory;		// PVP경험치
		// int				iPVPPoint;		// PVP포인트

		//PVP 정보를 수정합니다
		TOOL_SC_PVPINFO_UPDATE,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드

	//////////////////////////////////////////////////////////////////////////
	//
	//	EVENTTAB
	//
		PI_CS_EVENTTAB = PI_CS_TOOL + 1900,

		//강제이벤트 리스트를 가져옵니다
		TOOL_CS_EVENTLIST = PI_CS_EVENTTAB,
		// int				iWID;			// 월드아이디
		// int				iCharID;		// PC의 DB키

		//강제이벤트 리스트를 가져옵니다
		TOOL_SC_EVENTLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// GM_EVENT		stGM_Event;

		//강제이벤트 체크 여부를 수정합니다
		TOOL_CS_EVENTCHECK,
		// int				iWID;			// 월드아이디
		// GM_EVENT		stGM_Event;

		//강제이벤트 체크 여부를 수정합니다
		TOOL_SC_EVENTCHECK,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// GM_EVENT		stGM_Event;

	//////////////////////////////////////////////////////////////////////////
	//
	//	GLOBALEVENTTAB
	//
		PI_CS_GLOBALEVENTTAB = PI_CS_TOOL + 2000,

		// 예약된 전역이벤트리스트를 가져옵니다.
		TOOL_CS_GLOBALEVENT_RESERVELIST = PI_CS_GLOBALEVENTTAB,

		// 예약된 전역이벤트리스트를 가져옵니다.
		TOOL_SC_GLOBALEVENT_RESERVELIST,
		// INT32			iCnt;
		// GLOBALEVENT		stGlobalEvent;
		// ...

		// 적용된 전역이벤트리스트를 가져옵니다.
		TOOL_CS_GLOBALEVENT_APPLYLIST,

		// 적용된 전역이벤트리스트를 가져옵니다.
		TOOL_SC_GLOBALEVENT_APPLYLIST,
		// INT32			iCnt;
		// GLOBALEVENT		stGlobalEvent;
		// ...

		// 게임서버에 실제 적용된 전역이벤트리스트를 가져옵니다.
		TOOL_CS_GLOBALEVENT_REALLIST,

		// 게임서버에 실제 적용된 전역이벤트리스트를 가져옵니다.
		TOOL_SC_GLOBALEVENT_REALLIST,
		// UINT16			wWorldID;
		// UINT16			wServerID;
		// INT32			iCnt;
		// GLOBALEVENT		stGlobalEvent;
		// ...

		// 지엠툴로 전역이벤트를 예약합니다.
		TOOL_CS_GLOBALEVENT_RESERVE,
		// GLOBALEVENT		stGlobalEvent;

		// 지엠툴로 전역이벤트를 예약합니다.
		TOOL_SC_GLOBALEVENT_RESERVE,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// GLOBALEVENT		stGlobalEvent;

		// 예약된 전역이벤트를 삭제합니다.
		TOOL_CS_GLOBALEVENT_DELETE_RESERVE,
		// GLOBALEVENT		stGlobalEvent;

		// 예약된 전역이벤트를 삭제합니다.
		TOOL_SC_GLOBALEVENT_DELETE_RESERVE,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// GLOBALEVENT		stGlobalEvent;

		// 적용된 전역이벤트를 삭제합니다.
		TOOL_CS_GLOBALEVENT_DELETE_APPLY,
		// GLOBALEVENT		stGlobalEvent;

		// 적용된 전역이벤트를 삭제합니다.
		TOOL_SC_GLOBALEVENT_DELETE_APPLY,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// GLOBALEVENT		stGlobalEvent;

		// 게임서버에 실제 적용된 전역이벤트를 삭제합니다.
		TOOL_CS_GLOBALEVENT_DELETE_REAL,
		// GLOBALEVENT		stGlobalEvent;

		// 게임서버에 실제 적용된 전역이벤트를 삭제합니다.
		TOOL_SC_GLOBALEVENT_DELETE_REAL,
		// GLOBAL_STRING_ID	uiErrorID;		// 에러코드
		// GLOBALEVENT		stGlobalEvent;

};

enum PACKETID_CS_LOGIC
{
	//////////////////////////////////////////////////////////////////////////////
	//
	// Auth
	//
		PI_CS_AUTH = PI_CS_LOGIC + 0,

		AUTH_CS_LOGIN = PI_CS_AUTH,
		// NOT SUPPORTED !!! (Don't use it)

		AUTH_SC_LOGIN ,
		// RESULTCODE		resultCode;
		// USERID			uid;
		// PASSPORT			passport;
		// UINT16			uiServerID;
		// TCHAR			ID[MAX_LEN_ID];			// included NULL

		// for FDS (Shanda)
			// UINT32		uiAdditionalParam1;
			// UINT32		uiAdditionalParam2;

		AUTH_CS_LOGOUT ,
		//                        RID			uid;
		// PASSPORT			passport;

		AUTH_SC_LOGOUT ,
		// RESULTCODE		resultCode;

		AUTH_CS_LOGIN_WITH_VERSION ,
		// NOT SUPPORTED !!! (Don't use it)


		// keylogin + Version Check
		AUTH_CS_KEYLOGIN,
		// UINT8			lenKeyID;
		// TCHAR			KEYID[];				// include NULL
		// UINT8			lenStartTime;
		// TCHAR			StartTTime[];			// include NULL
		// UINT16			major;					// Build #
		// UINT16			minor;
		// UINT32			option;					// VERSION_OPTION 참고

		// UINT8			uiCount;
			// UINT32			uiVersion_1;		// uiCount 갯수만큼 반복
			// UINT8			uiFileNameLen_1;
			// TCHAR			szFileName_1[];


		AUTH_CS_LOGIN_WITH_VERSION2 ,
		// NOT SUPPORTED !!! (Don't use it)


		// 로긴(버전확인) : SITE_SANDA,SITE_BUDDYBUDDY
		AUTH_CS_LOGIN_EX,
		// UINT32			option;					// VERSION_OPTION 참고
		// UINT16			major;					// Build #
		// UINT16			minor;
		// UINT8			lenParam1;
		// TCHAR			szParam1[];
		// UINT8			lenParam2;	
		// TCHAR			szParam2[];

		/*
				SITE_ACTOZ
					param1 : ID
					param2 : PW
				SITE_GAMEPOT
					param1 : ID
					param2 : PW
				SITE_SANDA
					param1 : ID
					param2 : PW
				SITE_BUDDYBUDDY
					param1 : ID
					param2 : AUTHCODE[144]
		 */

		// UINT8			uiCount;
			// UINT32			uiVersion_1;		// uiCount 갯수만큼 반복
			// UINT8			uiFileNameLen_1;
			// TCHAR			szFileName_1[];

		 AUTH_SC_EKEY,
		 // UINT8			mode;					// 1(EKEY_REQUEST),2(EKEY_ERROR),3(EKEY_SN_REQUEST)
		 // int				errorValue;

		 //if mode == EKEY_REQUEST
			// UINT8		EKeyType;				// 1(6,old ekey with challenge),2(8,old ekey without challenge),3(8,new ekey with challenge)
			// UINT8		lenChallenge;			// include NULL
			// TCHAR		szChallenge[];			

		 AUTH_CS_EKEY,
		 // UINT8			lenChallenge;
		 // TCHAR			szChallenge[];			// include NULL
		 // UINT8			lenEKey;	
		 // TCHAR			szEKey[];				// include NULL

		 // Ekey machine's SN : 끝에서 6자리
		 AUTH_CS_EKEY_SN,
		 // TCHAR			szEKeySN[7];				// include NULL

		 // AUTH_CS_LOGIN_EX에 대한 결과 
		 AUTH_SC_LOGIN_EX ,
		 // GLOBAL_STRING_ID	itemID;	

		 // if itemID == 0
			// USERID				uid;
			// PASSPORT				passport;
			// UINT16				uiServerID;
			// UINT8				lenID;
			// TCHAR				szID[];

	//////////////////////////////////////////////////////////////////////////////
	//
	// World/Channel Selection (WorldServer)
	//
		PI_CS_WORLD = PI_CS_LOGIC + 100,

		// 월드 목록 요청
		WORLD_CS_GETLIST = PI_CS_WORLD,
		// USERID		userID;

		WORLD_SC_GETLIST ,
		//UINT8			numOfWorlds;
		//WORLD_INFO	worldInfos[];

		// 월드 선택결과를 서버로 전송, 해당 월드내의 채널 상태 수신(WORLD_SC_CHANNELSTATUS)하게 됨
		WORLD_CS_SELECT ,
		//WORLD_ID		worldID;

		WORLD_SC_SELECT ,
		// RESULTCODE		resultCode;			

		// 선택된 월드의 채널 상태 정보 요청
		WORLD_CS_CHANNELSTATUS ,
		//WORLD_ID			worldID;

		// 로긴/로비창에서 요청된 경우
		WORLD_SC_CHANNELSTATUS ,
		// UINT8			numOfChannels;
		// CHANNEL_INFO		chInfo[];

		// zone에서 채널 변경을 위한 과정중, 채널 목록 요청의 결과
		WORLD_SC_CHANNELSTATUS2 ,
		// UINT8			numOfChannels;
		// CHANNEL_INFO		chInfo[];
		// SERVER_ID		curChannelID;

		// 채널 선택 결과를 서버로 전송
		CHANNEL_CS_SELECT ,
		// WORLD_ID			worldID;
		// SERVER_ID		channelID;
		// USERID			uid;
		// PASSPORT			passport;

		CHANNEL_SC_SELECT ,
		// RESULTCODE		resultCode;
		// TCHAR			ip[LEN_IP];
		// UINT16			port;

		// 특정 서버에 입장
		WORLD_CS_ENTER ,
		// USERID			uid;
		// PASSPORT			passport;
		// WORLD_ID			worldID;
		// TCHAR			charName[LEN_NAME];
		// TCHAR			szID[LEN_ID];
		// UINT32			uiIndunID; // 인던서버인 경우는 인던인스턴스아이디, 게임서버인 경우는 인던LDT아이디

		WORLD_SC_ENTER ,
		// RESULTCODE		resultCode;

		// 게임에서 나가기
		WORLD_CS_LEAVE ,
		// USERID			uid;
		// PASSPORT			passport;

		WORLD_SC_LEAVE ,
		// RESULTCODE		resultCode;
		
		GAME_CS_START ,
		
		// 게임 시작!!!
		GAME_SC_START ,				
		// RESULTCODE		resultCode;
		
		// 현 시점에서 채널 변경 작업을 시작. 게임 서버는 이 시점에서 SaveDB 수행
		GAME_CS_CHANGE_CHANNEL ,
		// WORLD_ID			worldID;
		// SERVER_ID		channelID;

		// saveDB 결과 전송. 클라이언트는 disconnect하고 목적지 채널로 connection
		GAME_SC_CHANGE_CHANNEL ,
		// GLOBAL_STRING_ID	itemID;				// itemID = 0 or 4000006 or LT_ERROR_SYSTEMERROR(= 6)
		// if(itemID == 0)
			// TCHAR			ip[LEN_IP];
			// UINT16			port;
			// USERID			userID;
			// PASSPORT			passport2;		// DestServer에서 새로 생성된 passport


		// Channel in and out
		//
		// 채널상태(게임 플레이 상태)로의 입장/퇴장 
		//{
			// GS로의 입장 : LBS --> GS
			CHANNEL_CS_IN,
			// UINT32	uiUserID;
			// UINT32	uiReservedParam;
			CHANNEL_SC_IN,
			// GLOBAL_STRING_ID	itemID;			

			// GS에서의 퇴장 : GS --> LS [,kickout,disconnect,serverdown]
			CHANNEL_CS_OUT,
			// UINT32	uiUserID;
			// UINT32	uiReservedParam;
			CHANNEL_SC_OUT,
			// GLOBAL_STRING_ID	itemID;		

		//}

	//////////////////////////////////////////////////////////////////////////////
	//
	// Character Management (WorldServer)
	//
		PI_CS_CHAR = PI_CS_LOGIC + 200,

		// 캐릭터 생성작업중 가등록된 캐릭터명 삭제
		CHAR_CS_DELETE_CHARNAME = PI_CS_CHAR,
		// TCHAR			charName[LEN_NAME];

		CHAR_SC_DELETE_CHARNAME ,
		// RESULTCODE		resultCode;

		// 캐릭터 생성과정중, 캐릭터 이름 중복 검사
		CHAR_CS_CHECK_CHARNAME ,
		// WORLD_ID			worldID;
		// TCHAR			szID[LEN_ID];
		// TCHAR			szNewCharName[LEN_NAME];

		CHAR_SC_CHECK_CHARNAME ,
		// RESULTCODE		resultCode;					// LT_NO_ERROR(사용가능), LT_ERROR_FAILED(캐릭터명 중복)

		// 현재 해당 유저의 캐릭터 목록을 요청한다.
		CHAR_CS_GETLIST ,
		// USERID			userID;
		// WORLD_ID			worldID;
		// TCHAR			szID[LEN_ID];

		CHAR_SC_GETLIST ,
		// RESULTCODE		resultCode;
		// UINT8			numOfChars;

		// for(numOfChars)
			// CHARACTER_INFO2	charInfo[];
			// UINT16			numOfItems
			// CONTAINER_ITEM	itemInfo[];
			// UINT8			byNewbi;		// 캐릭터 생성후 최초 진입 여부

		// 해당 월드내에 새로운 캐릭터를 하나 생성한다.
		CHAR_CS_CREATE ,
		// WORLD_ID			worldID;
		// CHARACTER_INFO	charInfo;
		// CHAR_FIGURE		charFigure;

		CHAR_SC_CREATE ,
		// RESULTCODE		resultCode;
		// CHARID			charID;

		// 해당 캐릭터를 삭제한다.
		CHAR_CS_DELETE ,
		// WORLD_ID			worldID;
		// USERID			userID;
		// CHARID			charID;
		// TCHAR			szCharName[LEN_NAME+1];

		CHAR_SC_DELETE ,
		// GLOBAL_STRING_ID	iErrorMsg;		// 에러코드

		// 특정 캐릭터를 선택하고 게임 시작.
		CHAR_CS_SELECT ,
		// USERID			userID;
		// CHARID			charID;
		// SERVER_ID		channelID;		// 접속할 채널ID

		CHAR_SC_SELECT ,	// 게임 시작
		// RESULTCODE			resultCode;
		// CHARACTER_INFO		charInfo;
		// LT_POSITION			posInfo;
		// STATUS_INFO			statusInfo;
		// MAP_INFO				mapInfo;
		// ITEMMIX_INFO			ItemMixInfo;
		// PVP_STAT				PvPInfo;
		// CHARACTER_CASH_INFO charCashInfo; //mijin 20080306 캐시추가

		// 특정 캐릭터의 정보 얻기
		CHAR_CS_CHARINFO,
		// CHARID			charID;				// for PC/NPC/MOB

		CHAR_SC_CHARINFO ,
		// RESULTCODE		resultCode;
		// GU_ID			globalID;
		// CHARACTER_INFO2	charInfo2;

		// 특정 캐릭터 위치정보 얻기
		CHAR_CS_POSINFO ,
		// CHARID			charID;				// for PC/NPC/MOB

		CHAR_SC_POSINFO ,
		// RESULTCODE		resultCode;
		// MAP_INFO			mapInfo;
		// LT_POSITION		posInfo;

		// 특정 캐릭터 스탯 정보 얻기
		CHAR_CS_STATUSINFO ,       // 사용 안되고 있음 삭제예정임
		// CHARID			charID;				

		CHAR_SC_STATUSINFO ,		// 사용 안되고 잇음 삭제예정임
		// GU_ID		globalID;
		// STATUS_INFO	statusInfo;

		CHAR_CS_ACTION ,
		// ACTIONID			actionID;

		CHAR_SC_ACTION ,
		// CHARID			charID;
		// ACTIONID			actionID:

		CHAR_CS_SET_QUICK_SLOT ,
		// UINT8			iNumOfQuickSlot;
		// QUICK_SLOT*iNumOfLevelUpChange;

		CHAR_SC_QUICK_SLOT	,
		// GU_ID			globalID
		// UINT8			iNumOfQuickSlot;
		// QUICK_SLOT*iNumOfLevelUpChange;
	
		// 특정 캐릭터 이동관련 Status 정보 얻기
		CHAR_CS_MOVING_STATUS_INFO ,

		CHAR_SC_MOVING_STATUS_INFO ,
		// GU_ID			globalID
		// MOVING_STAUS		MovingStatusInfo;

		// 캐릭터의 디렉션 정보 요청.
		CHAR_CS_FACING				,
		// UINT8			facing

		// 캐릭터의 디렉션 정보 전송.
		CHAR_SC_FACING				,
		// GU_ID			globalID
		// FACING			facing

		// 캐릭터의 컨테이너 정보.
		CHAR_SC_CONTAINER_INFOS		,
		// GU_ID			globalID		//
		// INT8				iNumOfInfo		// 변경될 정보세트 갯수
		// CONTAINER_TYPE	ContainerType	// 컨테이너 타입.		(변경될 정보세트)
		// INT8				TotalSlot		// 총갯수 (=  기본 + OptionSlotTot )
		// INT8				OptionSlotTot	// 옵션 슬롯의 총갯수.(변경될 정보세트)

		// 캐릭터의 컨테이너 정보.
		CHAR_CS_CONTAINER_INFOS		,

		CHAR_SC_EFFECT_STATUS_INFO ,
		// GU_ID				globalID		
		// EFFECT_STATUS_INFO	StatusInfo;

		// 되살아나기 요청
		CHAR_CS_REVIVAL,
		// SAVEPOINT_TYPE			iSavePointType; 
		// ITEM_NO					iTrashItemNo;
		// TRASH_ITEM				TrashItem;

		// 아이템 조합 숙련도 변경시
		CHAR_SC_ITEMMIX_INFO,
		// GU_ID				globalID;
		// ITEMMIX_INFO			stItemMixInfo;

		// 캐릭터 클래스변경
		CHAR_CS_CHANGE_CLASS ,
		// int				iChangeClassID ;	// CLASS_CHANGE.ldt::_Item_ID
		// TRASH_ITEM		stTrashItem;		// 소모아이템

		// 캐릭터 클래스변경 결과
		CHAR_SC_CHANGE_CLASS ,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// GLOBAL_STRING_ID	iErrorID;		// 에러아이디,  0 인경우만 이하 데이터송신
		// UINT8			NewClassType;
		// __int64			iCurrEly;		// 돈 
		// short			iSkillFreePoint;// SKILL_SC_INIT와 동일한 이벤트 처리요망
		// UINT8			iNumOfLevelUpChange;
		// STATUS_CHANGE    tStatPack[];	// 바뀔 능력치 	
		// STATUS_INFO		statInfo ;
		// UINT8			iNumOfItem;		// 전직함으로 해서 생성되는 아이템
		// CONTAINER_ITEM	aryContainerItem[];
		// TRASH_ITEM		stTrashItem;

		// PVP 되살아나기 요청
		CHAR_CS_PVP_REVIVAL,

		//	 鯤소돨GM榴檄
		CHAR_SC_GM_STATUS,
		//	INT  byIsGM;			0槨렷GM ，댕黨0槨GM

	//////////////////////////////////////////////////////////////////////////////
	//
	// Status (LevelUp,Exp,hp,sp,...) 
	//
	PI_CS_STAT = PI_CS_LOGIC + 300,

		// 변경된 status 값을 전송
		STAT_SC_CHANGE = PI_CS_STAT,
		// GU_ID			globalID; 
		// UINT8			numOfStatus;
		// STATUS_CHANGE	statusChanges[];

		STAT_SC_CHANGE_EX ,
		// GU_ID			globalID;
		// UINT8			numOfStatus;
		// STATUS_CHANGE_EX	statusChanges[];

		STAT_SC_LEVEL_UP ,
#ifdef __CHANGE_CLASS
		//GU_ID		globalID			
		//UINT16	iLevel;
		//__int64   BaseExp	
		//__int64   NextExp
		//UINT8		iNumOfLevelUpChange;
		//STATUS_CHANGE tStatPack[]
#else
		//GU_ID   globalID			
		//__int64 BaseExp	
		//__int64 NextExp
		//UINT8 iNumOfLevelUpChange;
		//STATUS_CHANGE tStatPack[]
#endif

	//////////////////////////////////////////////////////////////////////////////
	//
	// Game Options : 게임 서버로의 로긴시 DBLoad 되고, 로그아웃시 DBSave됨
	//
		PI_CS_GAME_OPTION = PI_CS_LOGIC + 400,

		// PC별 게임 옵션 요청. *** 현재 사용하지 않습니다. CHARACTER_INFO2 구조체에 포함되어서 게임서버 진입시와 인사이트/아웃사이트시 클라이언트에 송신합니다.
		GAME_CS_GETOPTION = PI_CS_GAME_OPTION,
		GAME_SC_GETOPTION ,
		// GLOBAL_STRING_ID	ErrorID;
		// UINT16			uiGameOption;		// 귀속말,친구추가,거래요청,팀요청,상태알림			

		// PC별 게임 옵션 설정
		GAME_CS_SETOPTION ,
		// INT32			iGameOptions;
		GAME_SC_SETOPTION ,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;
		// INT32			iGameOptions;

	//////////////////////////////////////////////////////////////////////////////
	//
	// Action Command (MOVE/STOP/ATTACK/SKILL,...)
	//
		PI_CS_ACTION_COMMAND = PI_CS_LOGIC + 500,

		ACTION_CS_COMMAND = PI_CS_ACTION_COMMAND,
		// unsigned __int64	commandBuf;
		// unsigned int		sequenceIndex;

		ACTION_SC_COMMAND ,
		// RESULTCODE		resultCode;
		// GU_ID			globalID;			// classID(CLASS_ID_AVATAR(PC) | CLASS_ID_MONSTER(MOB)) + instanceID
		// unsigned __int64	commandBuf;
		// unsigned int		seqIndex
		// LT_POSITION		pos;

		ACTION_SC_KNOCKBACK,
		// RESULTCODE		resultCode;
		// GU_ID			globalID;
		// LT_KNOCKBACK		knockBackInfo;

	//////////////////////////////////////////////////////////////////////////////
	//
	// Battle (GameServer)
	//
		PI_CS_BATTLE = PI_CS_LOGIC + 600,

		BATTLE_SC_ATTACKED = PI_CS_BATTLE,
		// RESULTCODE		resultCode;
		// GU_ID			globalID;	// 피격자
		// int				iDamage;		// damage value
		// int				iMode		// 0:일반 1:크리 2: 미스 3: Immune
		// int				iType		// EFFECT.LDT::_StatusEffectType
		// GU_ID			AttackerGlobalID;	// 공격자
		// uint8			iHitCount			// Combo HitCount

		BATTLE_SC_HEALING,
		// RESULTCODE		resultCode;
		// GU_ID			globalID;
		// int				iHeal;		// damage value
		// int				iMode		// 0:일반 1:크리 2: 미스 3: Immune
		// int				iType		// EFFECT.LDT::_StatusEffectType

	//////////////////////////////////////////////////////////////////////////////
	//
	// Navigation	(GameServer)
	//
		PI_CS_NAV = PI_CS_LOGIC + 700,

		// 특정 캐릭터가 시야에 들어왔다.
		NAV_SC_INSIGHT = PI_CS_NAV,

		//기본정보
		//packet << pChar->GetGUID();
		//packet.PushBack(&charInfo2,sizeof(CHARACTER_INFO2));

		//Action
		//packet << pChar->GetCurAction();
		//packet << uiSeqIndex;
		//packet.PushBack(&posInfo,sizeof(LT_POSITION));

		//Item
		//packet.PushBack((UINT16)iNumOfFigureItem);
		//sizeof(FIGURE_ITEM) * iNumOfFigureItem;

		//Effect
		//packet.PushBack((UINT16)iNumOfEffect);
		//sizeof(EFFECT_DATA) * iNumOfEffect;

		// 개인상점오픈중인 상태일때만 상점명을 보냅니다.
		// char				szFMName[FM_MAX_NAME+1]; // 상점명

		// 길드정보
		// packet.PushBack(&stGuildBroadCast, sizeof(GUILD_BROADCAST));

		// PC 인경우
		//UINT8		iGloryLv;

		// 대화방 오브젝트 인 경우.. 2008.04.14 Hoony
		// GU_ID					ObjectGUID;					// 게임오브젝트아이디
		// BG_ID					Layer;
		// UINT						iPosX;						//
		// UINT						iPosY;						//
		// CHATROOM_OBJECT_INFO		stChatRoomObject_Info		//

		// 특정 캐릭터가 시야에서 벗어났다.
		NAV_SC_OUTOFSIGHT ,
		// GU_ID			globalID;			// classID + instanceID


		//	륩蛟포蕨와빵똥랙箇쏵흙柬柰돨훙膠돨guid
		NAV_SC_INSIGHT_GUID,
		//  UINT8 uCount;
		//  UINT32 insight_ID[];

		//	와빵똥몽앴륩蛟포똥랙箇돨쏵흙柬柰돨훙膠guid윱롸툽혤훙膠斤口
		NAV_CS_INSIGHT_GUID,
		//	UINT8 uCount;
		//	UINT32 insight_ID[];

	//////////////////////////////////////////////////////////////////////////////
	//
	// Event : NPC, SavePoint, Portal	(GameServer)
	//
		PI_CS_EVENT = PI_CS_LOGIC + 800,

		EVENT_CS_RAISE = PI_CS_EVENT,
		//UINT8		eventType;					// NPC(1),SavePoint(2),Portal(3)
		//UINT32	uiEventID;					// NPCID,SavePointID,PortalID
		
		EVENT_SC_RAISE ,
		// RESULTCODE		resultCode;
		// UINT8			EventType;			// NPC(1),SavePoint(2),Portal(3)
		// UINT32			uiEventID;
		// MAP_INFO			mapInfo;			// for Portal		 (현재 채널내로 국한됨)
		// float			posX;				// for Portal	
		// float			posY;				// for Portal	

		// EVENT_SC_RAISE -> Client -> Client:;LoadStage() 완료후, 서버로 전송됨.
		NAV_CS_ENTERSTAGE ,

	//////////////////////////////////////////////////////////////////////////
	//
	// Item 
	//
		PI_CS_ITEM = PI_CS_LOGIC + 900,

		// 컨테이너 내에서 단일 슬롯의 아이템 정보 갱신
		ITEM_SC_CONTAINER_INFO  = PI_CS_ITEM,	
		// GU_ID				globalID;
		// CONTAINER_ITEM		containerItem;

		// 컨테이너 내에서 복수 슬롯의 아이템 정보 갱신
		ITEM_SC_CONTAINER_INFOS  ,
		// GU_ID				globalID;
		// UINT16				nItems;
		// CONTAINER_ITEM		containerItem;

		// 아이템 줍기 이동 요청
		ITEM_CS_LOOTING ,
		// GU_ID				iLootingBagID;
		// LOOTING_ITEM			stLooting_Item;

		// 아이템 줍기 성공. (해당 몹 인벤의 슬롯을 클리어 해준다.)
		ITEM_SC_LOOTING , // 브로드 캐스팅도 해야함.			
		// GU_ID				globalID;
		// GLOBAL_STRING_ID		ErrorID;		// 에러아이디
		// GU_ID				ActCharGUID;	// 아이템을 습득한 PC의 GUID
		// LOOTING_ITEM			stLooting_Item;	// 클라이언트 구조의 편의성을 위하여 아이템정보를 한번 더 넣어줍니다. <-- 메세지용
		// UINT8				byRare;			// 메세지에 색상표시를 위해서 레어도를 추가합니다.
		// LOOTING_ITEM			stLooting_Item;

		// 컨테이너내에서의 아이템 위치 이동 요청.
		ITEM_CS_CHANGE_SLOT ,	
		// CHANGE_SLOT			changeSlot;

		// 컨테이너내에서의 아이템 위치 이동 요청.
		ITEM_SC_CHANGE_SLOT ,	
		// GU_ID				globalID;
		// CHANGE_SLOT			changeSlot;

		// 아이템 버리기 요청.
		ITEM_CS_TRASH ,
		//TRASH_ITEM			trashItem;

		// 아이템 파괴 성공 ( 해당 인벤의 슬롯을 클리어 해줌)
		ITEM_SC_TRASH ,			
		// GU_ID				globalID;
		// UINT8				iNumOfTrashItem;
		// TRASH_ITEM			trashItem*iNumOfTrashItem;

		// 아이템 행위에 대한 메세지. 
		ITEM_SC_ALARM , // 사용 안함
		//// GU_ID				globalID;
		//// EXCEPTION_CODE		exceptionCode;

		// 아이템 사용
		ITEM_CS_USING ,
		// GU_ID				타겟 엔티티 아이디
		// CONTAINER_SLOT		위치정보
	
		// 사용 성공. 클라연트에서 해당 아이템 가감.
		ITEM_SC_USING ,
		// GU_ID				globalID	; // 타겟 엔티티 아이디
		// GLOBAL_STRING_ID		iError		; // 0 : 성공
		// TRASH_ITEM			UsingItem	; // 사용 된 아이템 위치 정보

		// 외부에 보여야 하는 아이템들의 브로드 캐스팅 정보
		ITEM_SC_FIGURE_INFOS  ,	// 외형정보
		ITEM_SC_FIGURE_ON,				// 외형 
		ITEM_SC_FIGURE_OFF,				// 외형 해제(장비시 필요)
		
		ITEM_CS_TOGGLE_USE_WEAPON_SET,  // 무기 swap 요청.
		ITEM_SC_TOGGLE_USE_WEAPON_SET,	// 무기 swap 성공.
		//GU_ID				globalID	;
		//RESULT_CODE iError 
		//UINT8 uiActiveSet);

		ITEM_SC_LOOTING_LIST,           // 루팅 아이템 정보를 보냄. 
		// GU_ID					// Looting bag GlobalID
		// GU_ID					// Looting bag Owner
		// POSITION					// Looting bag Position
		// iNumOfItem(UINT8)				// Items In Bag
		//    iSlot	 (UINT8)					 
		//    ITEM	 			
		
		ITEM_CS_CONTAINER_INFOS ,
		// GU_ID				globalID;
		// UINT16				nItems;
		// CONTAINER_ITEM		containerItem;

		// 상태보기용.
		ITEM_CS_FIGURE_INFOS_FOR_SINGLE,
		// GU_ID			iGlobalID				// Target유저의 GU_ID 
		// INT8				iNumOfCONTAINER_TYPE	// 보고싶은 컨테이너번호의 갯수.						//
		// CONTAINER_TYPE	iContainerType[] 		// 컨테이너 타입.
			
		// 다른 유저의 아이템 정보보기
		ITEM_SC_FIGURE_INFOS_FOR_SINGLE,
		// GU_ID			iGUID					// 유저의 GU_ID
		// GU_ID			iTargetGUID				// 타겟유저의 GU_ID
		// UINT16			iNumOfContainerItem		// 컨테이너 아이템 갯수		
		// CONTAINER_ITEM	iContainerItems[]		// 타겟유저의 컨테이너 아이템.

		// 신규 새로운 인스턴스 아이템을 만들어 낸다.
		ITEM_CS_DIVIDE,
		// CHANGE_SLOT		changeSlot				// 해당 아이템의 위치 및 갯수정보/

		ITEM_SC_DIVIDE,
		// GU_ID			iGUID					// 유저의 GU_ID
		// GLOBAL_STRING_ID	iResultError			// 0: 성공 0보다 크면 에러.
		// TRASH_ITEM		TrashITem				// 소스의 카운팅 삭감 정보.
		// CONTAINER_ITEM	NewItem					// 새로 생성된 인스턴스 컨테이너 아이템.

		ITEM_CS_ENCHANT,
		// int				iEnchantID;				// ITEM_ENCHANT.LDT::PrimaryKey
		//----------------------------------------------------------
		// CONTAINER_TYPE	iMainContainerType;			// 메인
		// UINT8			iMainContainerSlot;			// 메인
		// ITEMNO			iMainItemNo;				// 메인

		// CONTAINER_TYPE	iMaterialContainerType;		// 재료 아이템 위치  
		// UINT8			iMaterialContainerSlot;		// 재료 아이템 위치
		// ITEMNO			iMaterialItemNo;			// 재료 아이템 번호
		// UINT8			iMaterialItemCount;			// 재료 아이템 요구갯수.  -- 참조용
		//-------------------------------------------------------------

		ITEM_SC_ENCHANT,
		// GU_ID			iGUID;					// 유저의 GU_ID
		// UINT8			iEnchanted;				// 0: 인첸트 조건 실퍠 1: 인첸트
		// GLOBAL_STRING_ID	iResultError;			// iEnchanted가 1 인경우 0: 성공 0보다 크면 꽝.  iEnchanted가 0인 경우 이값이 무조건 0보다 큰값으로 와야함.
													// 아이템 정보갱신용은 따로 (ITEM_SC_CONTAINER_INFOS/ITEM_SC_TRASH) 등의 패킷으로 전달.
		// int				iEnchantID;	


		ITEM_CS_UPGRADE,
		// int				iUpgradeID;				// ITEM_ENCHANT.LDT::PrimaryKey
		//----------------------------------------------------------
		// CONTAINER_TYPE	iMainContainerType;			// 메인
		// UINT8			iMainContainerSlot;			// 메인
		// ITEMNO			iMainItemNo;				// 메인
		//----------------------------------------------------------------
		// unsigned INT8	iNumOfMaterial; // 최대 4
		// MATERIAL_DATA	MaterialData[iNumOfMaterial]; // iNumOfMaterial 만큼 붙여주세요	
		//-------------------------------------------------------------
		// ITEMNO			iResultItemID;				// 결과 아이템			-- 참조용
		// 

		ITEM_SC_UPGRADE,
		// GU_ID			iGUID;					/// 유저의 GU_ID
		// UIN8				iUpgraded;				/// 0: 업그레이드 조건 실퍠 1: 업그레이드
		// GLOBAL_STRING_ID	iResultError;			/// iUpgraded가 1 인경우 0: 성공 0보다 크면 꽝.  iUpgraded가 0인 경우 이값이 무조건 0보다 큰값으로 와야함./ 0: 성공 0보다 크면 에러.  // 아이템 정보갱신용은 따로 (ITEM_SC_CONTAINER_INFOS/ITEM_SC_TRASH) 등의 패킷으로 전달.
		// int				iUpgradeID;						// 아이템 정보갱신용은 따로 (ITEM_SC_CONTAINER_INFOS/ITEM_SC_TRASH) 등의 패킷으로 전달.

		ITEM_CS_DESTROY_BY_TIME ,
		// ITEMNO			iItemNo;				
		// CONTAINER_SLOT	ContainerInfo;

		ITEM_SC_DESTROY_BY_TIME ,
		//GU_ID					iGUID;	
		//TRASH_ITEM			trashItem;

		ITEM_CS_BASIC_FIGURE_CHANGE,
		// int					iBeautyShopID;		// BEAUTY_SHOP.LDT::ItemID #
		// UINT8				iBasicFigureSlot;	//FIGURE_BASIC_TYPE 중 하나 
		// CONTAINER_SLOT		UsingItemPos1;  // 사용된쿠폰아이템위치
		// ITEMNO				iUsingItemNo1;  // 사용된쿠폰아이템키
		// CONTAINER_SLOT		UsingItemPos2;  // 사용된쿠폰아이템위치
		// ITEMNO				iUsingItemNo2;  // 사용된쿠폰아이템키

		ITEM_SC_BASIC_FIGURE_CHANGE,  // 주변인들에게는 ITEM_SC_FIGURE_ON 
		//GU_ID					iGUID;
		//GLOBAL_STRING_ID		iResultError;	
		//int					iBeautyShopID;	// 변형
		//TRASH_ITEM			UsingItemPos1;	// 사용된쿠폰아이템위치
		//ITEMNO				iUsingItemNo1;	// 사용된쿠폰아이템키
		//TRASH_ITEM			UsingItemPos2;	// 사용된쿠폰아이템위치
		//ITEMNO				iUsingItemNo2;	// 사용된쿠폰아이템키
		//CONTAINER_ITEM		NewCharItem;	// 수정후 정보

		ITEM_SC_EXPIRE,
		//GU_ID					iGUID;	
		//CONTAINER_SLOT		stItemPos;

		ITEM_CS_ADDTIME,
		// int				iItemAddTimeID;				// ITEM_ADD_TIME.LDT::PrimaryKey
		//----------------------------------------------------------
		// CONTAINER_TYPE	iMainContainerType;			// 메인
		// UINT8			iMainContainerSlot;			// 메인
		// ITEMNO			iMainItemNo;				// 메인

		// CONTAINER_TYPE	iMaterialContainerType;		// 재료 아이템 위치  
		// UINT8			iMaterialContainerSlot;		// 재료 아이템 위치
		// ITEMNO			iMaterialItemNo;			// 재료 아이템 번호
		// UINT8			iMaterialItemCount;			// 재료 아이템 요구갯수.  -- 참조용
		//-------------------------------------------------------------

		ITEM_SC_ADDTIME,
		// GU_ID			iGUID;					// 유저의 GU_ID
		// int				iItemAddTimeID;	
		// GLOBAL_STRING_ID	iResultError;			// 0: 성공 0보다 크면 꽝. 아이템 정보갱신용은 따로 (ITEM_SC_TRASH) 등의 패킷으로 전달.
		// ITEM_LIFETIME	stItemLifeTime;			// 성공시에만 유효한 변경된 라이프타임정보

		ITEM_SC_ITEMEXP,
		//GU_ID					iGUID;	
		//ITEM_EXP				stItemExp;

		ITEM_CS_ADDSP,
		// CONTAINER_SLOT		stMainItemPos;
		// ITEMNO				iMainItemNo;
		// CONTAINER_SLOT		stMaterialItemPos;
		// ITEMNO				iMaterialItemNo;

		ITEM_SC_ADDSP,
		//GU_ID					iGUID;	
		//GLOBAL_STRING_ID		ErrorID;
		//ITEM_SP				stItemSP;

		ITEM_CS_CHAT,
		// CONTAINER_SLOT		UsingItemPos;		// 사용할 아이템 위치정보
		// ITEMNO				iUsingItemNo;		// 사용할 아이템 키번호
		// BROADCAST_SCOPE		scope;				// 브로드캐스팅 범위
		// UINT8				len;				// 메세지 길이
		// TCHAR				szMsg[];	

		// 사용 성공. 클라연트에서 해당 아이템 가감. // 실제채팅은 CHAT_CS_MSG 로 날라감
		ITEM_SC_CHAT,
		// GU_ID				globalID	; // 사용자 아이디
		// GLOBAL_STRING_ID		iError		; // 0 : 성공
		// TRASH_ITEM			UsingItem	; // 사용 된 아이템 위치 정보
		
		// 조합
		ITEM_CS_MIX,
		// int				iMixID;				// ITEM_CREATE_RECIPE.LDT::PrimaryKey
		//----------------------------------------------------------------
		// UINT8			iNumOfMaterial; // 최대 5
		// MATERIAL_DATA	MaterialData[iNumOfMaterial]; // iNumOfMaterial 만큼 붙여주세요	

		// 조합결과
		ITEM_SC_MIX,
		// GU_ID			iGUID;					/// 유저의 GU_ID
		// GLOBAL_STRING_ID	iResultError;			/// 0: 성공 0보다 크면 에러. 
		// UIN8				iMixed;					/// 0: 확률실패 1: 확률성공
		// int				iMixID;
		// ITEMMIX_INFO		stItemMixInfo;			// 캐릭터의 조합정보
		// UINT8			iNumOfMeterial;
		// TRASH_ITEM		stMeterial[];
		// CONTAINER_ITEM	stResult;				// 결과아이템

		//	賈痰듐얩좔莖 [2008-07-17]
		MONEY_CS_CHAT,
		//	CONTAINER_SLOT		UsingItemPos;		// 사용할 아이템 위치정보
		//	ITEMNO				iUsingItemNo;		// 사용할 아이템 키번호
		//	BROADCAST_SCOPE		scope;				//	밤꺄돨렀鍋
		//	ITEMID				iItemID;			//	뚤壇돨膠틔돨ID
		//  UINT8				len;				//  句口낀똑
		//  TCHAR				szMsg[];			//	句口돨코휭

		//	角뤠냥묘	[2008-07-17]
		MONEY_SC_CHAT,	
		// GU_ID				globalID	; 鯤소돨GUID
		// GLOBAL_STRING_ID		iError		; 뚤壇돨句口돨ID
		// TRASH_ITEM			UsingItem	; // 사용 된 아이템 위치 정보
		
		ITEM_CS_EFFECTUPGRADE,
		// int				iUpgradeID;					// 
		// CONTAINER_TYPE	iMainContainerType;			// 
		// UINT8			iMainContainerSlot;			// 
		// ITEMNO			iMainItemNo;				// 
		// MATERIAL_DATA	MaterialData;				//
		// ITEMNO			iResultItemID;				//

		ITEM_SC_EFFECTUPGRADE,
		// GU_ID			iGUID;					//
		// UIN8				iUpgraded;				//
		// GLOBAL_STRING_ID	iResultError;			//
		// int				iUpgradeID;				//

		///add by chenhao 08.8.29
		ITEM_CS_WHERE,
		// GU_ID				타겟 엔티티 아이디
		// CONTAINER_SLOT		위치정보
		// INT32	SvrSeq
		// UINT32	Seq
		// TCHAR	szTargetCharName[LEN_NAME+1];

		ITEM_SC_WHERE,
		// GLOBAL_STRING_ID		iError;					// 댄轎ID
		// INT32			SvrSeq
		// UINT32			Seq
		// UINT8			bFlag;						// 鯤소角뤠瞳窟，1槨瞳窟，0槨꼇瞳窟
		// TCHAR			szCharName[LEN_NAME+1];
		// TCHAR			szTargetCharName[LEN_NAME+1];
		// UINT16			ServerID;
		// UINT				uiStageID;					// 스테이지아이디
		// UINT				uiMapGroupID;				// 맵그룹아이디
		// UINT				uiPosX;						// X좌표
		// UINT				uiPosY;						// Y좌표

		ITEM_SC_WHERE_COMMAMD_INFOS,
		// int nWhereItemCount;
		// WHERE_ITEMINFO stInfo[11];
		///add by chenhao 08.9.2

		ITEM_CS_MSGBOARD,
		// GU_ID
		// CONTAINER_SLOT
		// INT				iPosX;
		// INT				iPosY;
		// UINT				uiType;						//  句口경잚謹
		// UINT8			len;						//  句口낀똑
		// TCHAR			szMsg[];					//	句口돨코휭

		ITEM_SC_MSGBOARD,
		// GLOBAL_STRING_ID		iError;					// 댄轎ID

		ITEM_CS_DEL_MSGBOARD,
		// GU_ID										//돛야id
		// CONTAINER_SLOT
		// GU_ID										//句口경id

		ITEM_SC_DEL_MSGBOARD,
		// GLOBAL_STRING_ID		iError;					// 댄轎ID
		
		ITEM_CS_TRACK,									//뫄踞，婁훙돛야
		// GU_ID
		// CONTAINER_SLOT
		// TCHAR			szTargetCharName[LEN_NAME+1];

		ITEM_SC_TRACK,
		// GU_ID
		// GLOBAL_STRING_ID		iError;					// 댄轎ID


	//////////////////////////////////////////////////////////////////////////
	//
	//	NPC
	// 
		PI_CS_NPC = PI_CS_LOGIC + 1000,

		// NPC에게 팔기. PC -> NPC
		NPC_SHOP_CS_SELL = PI_CS_NPC, 
		// int				iNpcID 
		// int				iShopID
		// CONTAINTER_ITEM tContainerItem;

		// NPC에게 사기.	NPC -> PC
		NPC_SHOP_CS_BUY,  
		// int			iNpcID;			 // 누구에게서 산것인가
		// int			iShopID;		 // Shop ID 
		// UINT8		iSlotIndex;      // 어느칸에 있는 아이템을 살것인가.
		// ITEM			tItem;			 // ITEMID / COUNT  어떤 아이템을 몇개 살것인가.
	
		// 2005.11.03 dhpark NPC에게 사기 결과. 상태제어 때문에 추가합니다.
		NPC_SHOP_SC_BUY,  
		// GU_ID			ObjectGUID;	// 게임오브젝트아이디
		// GLOBAL_STRING_ID	iErrorID;	// 에러아이디(0이면 에러없음)

		// 컷인 열기
		CUTIN_CS_OPEN,
		// int				iNpcID;		// NPC

		// 컷인 열기
		CUTIN_SC_OPEN,
		// GU_ID			ObjectGUID;	// 게임오브젝트아이디

		// 컷인 닫기
		CUTIN_CS_CLOSE,
		// int				iNpcID;		// NPC

		// 컷인 닫기
		CUTIN_SC_CLOSE,
		// GU_ID			ObjectGUID;	// 게임오브젝트아이디

		// Npc PORTAL
		NPC_CS_PORTAL,
		// int            iNpcID;			// 엔피시
		// int			  iPortalGroupID;	// 포탈 그룹의 Key ID NPCPORTAL.LDT::_ItemID
		// int			  iPotalIndex;		// 위치정보
		// int			  iPortalInfoID;    // PORTAL_DETAIL.LDT::_ItemID
		// CONTAINER_SLOT stUsingItemPos;	// 소모아이템
		// ITEMNO		  iUsingItemNo;		// 소모아이템 No
		// int			  iUsingEly;		// 소모돈

		NPC_SC_PORTAL,
		// GU_ID			iPCGUID;		// 게임오브젝트아이디
		// GLOBAL_STRING_ID iErrorID;		// 성공 0 
		// UINT64			iCurrEly;       // 실제 포탈은 EVENT_SC_RAISE
		// TRASH_ITEM		stUsingItem;

		// NPC 보상 시스템 요청
		NPC_CS_REWARD,
		// int            iNpcID;			// 엔피시
		// int			  iRewardGroupID;		  
		// UINT8		  iRewardIndex;		// 랜덤 및 모두 보상일경우는의미없음
		// UINT8		  iNumOfUsingItem;
		// TRASH_ITEM	  stReqItem;		// 소모아이템
		// ITEMNO	      iReqItemNo;		// 소모아이템 No
		// int			  iUsingEly;		// 소모돈
		// int			  iUsingPVPPoint;	// 소모	PVP Point mijin 20071129

		// NPC 보상 시스템 결과
		NPC_SC_REWARD,
		// GU_ID			iPCGUID;		// 게임오브젝트아이디
		// GLOBAL_STRING_ID iErrorID;		// 성공 0 
		// UINT64			iCurrEly;       //
		// UINT8			iNumOfReqItem;
		// TRASH_ITEM		stReqItem;
		// UINT8			iNumOfRewardItem;
		// CONTAINER_ITEM	stItem;
		// int				iCurrPVPPoint;	// Current PVP Point mijin 20071129

	//////////////////////////////////////////////////////////////////////////
	//
	//	STORAGE
	// 
		PI_CS_STORAGE = PI_CS_LOGIC + 1100,

		// 창고에 게임머니 보관하기
		STORAGE_CS_INMONEY = PI_CS_STORAGE,
		// UINT64			uiMoney;		// 보관하려는 금액

		// 창고에 게임머니 보관하기
		STORAGE_SC_INMONEY,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// UINT64			uiMoney;		// 변경후 아바타가 가지고 있는 금액
		// UINT64			uiStorageMoney;	// 변경후 창고에 저장된 금액

		// 창고에서 게임머니 가져오기
		STORAGE_CS_OUTMONEY,
		// UINT64			uiMoney;		// 꺼내려고하는 금액

		// 창고에서 게임머니 가져오기
		STORAGE_SC_OUTMONEY,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// UINT64			uiMoney;		// 변경후 아바타가 가지고 있는 금액
		// UINT64			uiStorageMoney;	// 변경후 창고에 저장된 금액

	//////////////////////////////////////////////////////////////////////////////
	//
	// Chatting : 최대 MAX_CHATMSG_LEN 길이의 메세지를 전송
	//
		PI_CS_CHAT = PI_CS_LOGIC + 1200,

		// 채팅메세지
		CHAT_CS_MSG	= PI_CS_CHAT,
		// BROADCAST_SCOPE	scope;					// 브로드캐스팅 범위
		// UINT32			destination;			// 대상(partyID,channelID,worldID). if 서버간 전송시, HIWORD(worldID),LOWORD(serverID)
		// UINT8			len;					// 메세지 길이
		// TCHAR			szMsg[];				// 메세지
		// UINT8			nReceiverName;				// SCOPE_WHISPER일때만 사용됨
		// TCHAR			szReceiverName[LEN_NAME];	// SCOPE_WHISPER일때만 사용됨, 상대방 캐릭터정보

		//	GLOBAL_STRING_ID 예: 귓속말 전송 오류시 별도의 system Message로 반송 : 000님을 찾을 수 없습니다.
		CHAT_SC_MSG ,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT 상의 메세지를 itemID 값
		// BROADCAST_SCOPE	scope;				// 브로드캐스팅 범위
		// UINT8			uiFontColor;		// 폰트색	: 0(default),1(Red),2(Orange),3(Yellow),4(white) : CHAT_COLOR.LDT::ItemID
		// UINT8			uiOptions;			// reserved for FontSize,FontType
		// UINT8			len;
		// TCHAR			szMsg[];		
		// GU_ID			guidSender;			// 송신자 정보
		// UINT8			nSenderName;
		// TCHAR			szSender[];			

		// 수신자 정보(SCOPE_WHISPER only)
		//{
			// GU_ID			guidReceiver;
			// UINT8			nReceiverName;			
			// TCHAR			szReceiver[];
		//}

		// (SCOPE_WORLD ONLY)
		// UINT16			ChannelID;			// 채널 ID	(Cash-Item에 의한 월드 외치기시 채널 정보 표시용)

		// (SCOPE_TEAM ONLY)
		// INT32			iSenderCharID;		// 팀채팅을 보낸 캐릭터DB키(말풍선을 보여주기 위해서 추가)

		CHAT_SC_MSG_MONEY,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT 상의 메세지를 itemID 값
		// BROADCAST_SCOPE	scope;				// 브로드캐스팅 범위
		// UINT8			uiFontColor;		// 폰트색	: 0(default),1(Red),2(Orange),3(Yellow),4(white) : CHAT_COLOR.LDT::ItemID
		// UINT8			uiOptions;			// reserved for FontSize,FontType
		// ITEMID			useitemID
		// UINT8			len;
		// TCHAR			szMsg[];		
		// GU_ID			guidSender;			// 송신자 정보
		// UINT8			nSenderName;
		// TCHAR			szSender[];
		// UINT16			ChannelID;			// 채널 ID	(Cash-Item에 의한 월드 외치기시 채널 정보 표시용)

		CHAT_SC_MSG_PCINFO,		
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT 상의 메세지를 itemID 값
		// BROADCAST_SCOPE	scope;				// 브로드캐스팅 범위
		// UINT8			uiFontColor;		// 폰트색	: 0(default),1(Red),2(Orange),3(Yellow),4(white) : CHAT_COLOR.LDT::ItemID
		// UINT8			uiOptions;			// reserved for FontSize,FontType
		// ITEMID			useitemID			// 鯤소賈痰돨膠틔
		// CHARACTER_INFO2  info;				//	鯤소돨AVATA斤口
		// UINT				uItemCount;			//	苟충죗놔돨膠틔돨悧鑒
		// FIGURE_ITEM      tFigureItemList[MAX_FIGURE_SIZE + MAX_EQUIP_SIZE + MAX_FASHION_SIZE] ; //鯤소돨Avatar斤口
		// UINT8			len;
		// TCHAR			szMsg[];

	//////////////////////////////////////////////////////////////////////////////
	// Notice
	//
		PI_CS_NOTICE = PI_CS_LOGIC + 1300,

		// 공지 메세지 출력. 서버다운,전체 공지....
		MSG_CS_NOTICE = PI_CS_NOTICE,
		// UINT8			displayTime;		// 0(forever), sec
		// UINT8			optionFlags;		// reserved. display 효과 용도(캄박임,칼라,...)
		// UINT16			len;
		// TCHAR			szMsg[];

		MSG_SC_NOTICE ,
		// RESULTCODE		resultCode;
		// UINT8			displayTime;		// 0(forever), sec
		// UINT8			optionFlags;		// reserved. display 효과 용도(캄박임,칼라,...)
		// UINT16			len;
		// TCHAR			szMsg[];

	//////////////////////////////////////////////////////////////////////////
	//
	// Trade
	//
		PI_CS_TRADE = PI_CS_LOGIC + 1400,

		// 거래요청
		TRADE_CS_REQUEST = PI_CS_TRADE,
		// GU_ID			DstCharGUID;				// 거래대상자

		// 거래요청 결과
		TRADE_SC_REQUEST,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GU_ID			SrcCharGUID;				// 거래요청자
		// char				szSrcCharName[LEN_NAME+1];	// 거래요청자이름
		// GU_ID			DstCharGUID;				// 거래대상자
		// char				szDstCharName[LEN_NAME+1];	// 거래대상자이름
		// GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)

		// 거래대상자의 수락여부
		TRADE_CS_RESPONSE,
		// UINT8			bAccept;					// 수락/거절

		TRADE_SC_RESPONSE,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GU_ID			SrcCharGUID;				// 거래요청자
		// char				szSrcCharName[LEN_NAME+1];	// 거래요청자이름
		// GU_ID			DstCharGUID;				// 거래대상자
		// char				szDstCharName[LEN_NAME+1];	// 거래대상자이름
		// UINT8			byAccept;					// 수락/거절
		// GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)

		// 돈을 거래창에 올린다.
		TRADE_CS_PUSHMONEY,
		// UINT64	uiMoney;							// 올린 게임머니
		// int		iCash;								// 20080312 mijin 올린 캐쉬
		// int		iMile;								// 20080312 mijin 올린 마일리지
		// int		iCashComm;							// 20080312 mijin 캐쉬 수수료
		// int		iMileComm;							// 20080312 mijin 마일리지 수수료


		// 돈을 올린 PC의 보유금액과 비교하여 성공이면 두 아바타에게 모두 송신한다.
		TRADE_SC_PUSHMONEY,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GU_ID			ActCharGUID;				// 돈을 올린 아바타의 키
		// UINT64			uiMoney;					// 올린 돈
		// int				iCash;						// 20080312 mijin 올린 캐쉬
		// int				iMile;						// 20080312 mijin 올린 마일리지
		// int				iCashComm;					// 20080312 mijin 캐쉬 수수료
		// int				iMileComm;					// 20080312 mijin 마일리지 수수료


		// 아이템을 거래창에 올린다.
		TRADE_CS_PUSHITEM,
		// TRADE_ITEM		stTradeItem;				// 아이템을 올린 유저의 컨테이너,슬롯,트레이드슬롯 정보가 들어있는 구조체

		// 거래중인지, 아이템이 거래가능한건지, 해당 슬롯이 아이템이 이미 있는지 등등을 검사한후 성공이면 보내고 실패면 오류로그
		TRADE_SC_PUSHITEM,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GU_ID			ActCharGUID;				// 아이템을 올린 아바타키
		// TRADE_ITEM		stTradeItem;				// 아이템을 올린 유저의 컨테이너,슬롯,트레이드슬롯 정보가 들어있는 구조체
		// ITEM				stItem;						// 아이템에 대한 상세정보를 보내줘서 마우스를 아이템에 위치시키면 정보를 보여줄수있도록함

		// 아이템을 거래창에서 내린다. Accept상태를 해제함. 
		TRADE_CS_POPITEM,
		// SLOT_INDEX		TradeSlotIndex;				// 거래슬롯의 인덱스

		// 조건을 검사한후 성공이면 보내고 실패면 오류로그
		TRADE_SC_POPITEM,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GU_ID			ActCharGUID;				// 아이템을 내린 아바타키
		// SLOT_INDEX		TradeSlotIndex;				// 거래슬롯의 인덱스

		// 상대방이 이미 accept한 상태면 거래완료, 아니면 accept상태
		TRADE_CS_ACCEPT,

		// 상대방의 상태를 검사하여 accept상태로 변경하거나 거래완료를 합니다.
		TRADE_SC_ACCEPT,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GU_ID			ActCharGUID;				// accept를 누른 아바타키
		// UINT8			byFinish;					// 거래완료여부
		// GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)
		// GU_ID			ErrCharGUID;				// 오류제공자 아바타키

		// 거래를 취소합니다.
		TRADE_CS_CANCEL,

		// 거래취소를 두PC에게 송신합니다. 거래관리자에서 거래오브젝트 삭제
		TRADE_SC_CANCEL,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GU_ID			ActCharGUID;				// 거래를 취소한 아바타키

		// 상대방이 이미 WAIT한 상태면 거래대기완료모드로, 아니면 WAIT상태
		TRADE_CS_WAIT,

		// 상대방의 상태를 검사하여 WAIT상태로 변경하거나 거래대기완료모드로 전환합니다.
		TRADE_SC_WAIT,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GU_ID			ActCharGUID;				// WAIT를 누른 아바타키
		// UINT8			byAllWait;					// 거래대기완료여부

	//////////////////////////////////////////////////////////////////////////
	//
	// Mission 
	//
		PI_CS_MISSION = PI_CS_LOGIC + 1500,

		// 미션 리스트
		MISSION_SC_LIST = PI_CS_MISSION,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// int				iCnt;			// 미션 개수
		// MISSION			stMission;		// 미션 내용
		// MISSION			stMission;		// 미션 내용
		// MISSION			stMission;		// 미션 내용
		// ...								// 미션 개수만큼

		// 미션 변경
		MISSION_SC_UPDATE,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// MISSION			stMission;		// 미션 정보

		// 액션형미션 성공
		MISSION_CS_ACTION,
		// UINT32			uiActionID;		// 액션아이디

		// 액션형미션 성공결과
		MISSION_SC_ACTION,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// UINT32			uiActionID;		// 액션아이디

	//////////////////////////////////////////////////////////////////////////
	//
	// Quest 
	//
		PI_CS_QUEST = PI_CS_LOGIC + 1600,

		// 퀘스트 리스트
		QUEST_SC_LIST =  PI_CS_QUEST,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// int				iCnt;			// 퀘스트 개수
		// QUEST			stQuest;		// 퀘스트 내용
		// QUEST			stQuest;		// 퀘스트 내용
		// QUEST			stQuest;		// 퀘스트 내용
		// ...								// 퀘스트 개수만큼
			
		// 퀘스트 수락. 2007.09.19 dhpark 퀘스트수락종류증가에 따른 패킷구조변경.
		QUEST_CS_ACCEPT,
		// UINT8			byAcceptType;	// 퀘스트 수락 타입(1:NPC, 2:공유, 4:아이템, 8:이벤트)
		// INT32			iValue;			// 타입에 따른 검사용아이디(NPC아이디, 공유한 캐릭터DB키, 등등)
		// UINT32			uiQuestID;		// 퀘스트아이디

		// 퀘스트 수락. 2007.10.01 dhpark 클라이언트의 수락결과 구분을 위하여 수락타입송신
		QUEST_SC_ACCEPT,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;		// 에러아이디
		// QUEST			stQuest;		// 퀘스트 정보
		// UINT8			byAcceptType;	// 퀘스트 수락 타입(1:NPC, 2:공유, 4:아이템, 8:이벤트)

		// 퀘스트 포기
		QUEST_CS_CANCEL,
		// UINT32			uiQuestID;		// 퀘스트아이디
			
		// 퀘스트 포기
		QUEST_SC_CANCEL,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// UINT32			uiQuestID;		// 퀘스트아이디

		// 퀘스트 완료
		QUEST_CS_FINISH,
		// int				iNpcID;			// NPC아이디
		// UINT32			uiQuestID;		// 퀘스트아이디
		// UINT8			byRank;			// 0, 1, 2
		// UINT8			byReward;		// 선택형일때 유저가 선택한 보상인덱스(0, 1, 2)
			
		// 퀘스트 수락
		QUEST_SC_FINISH,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;		// 에러아이디
		// QUEST			stQuest;		// 퀘스트 정보

		// 퀘스트 공유
		QUEST_CS_SHARE,
		// UINT32			uiQuestID;		// 퀘스트아이디
			
		// 퀘스트 공유(공유한 캐릭터용)
		QUEST_SC_SHARE,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;					// 에러아이디
		// char				szRcvCharName[LEN_NAME+1];	// 공유받는 캐릭터명
			
		// 퀘스트 공유(공유받는 캐릭터용)
		QUEST_SC_SHARE_RECV,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// UINT32			uiQuestID;		// 퀘스트아이디
		// INT32			iActCharID;		// 공유한 캐릭터DB키(QUEST_CS_ACCEPT를 보낼때 사용해야합니다.)

	//////////////////////////////////////////////////////////////////////////
	//
	//	커뮤니티 : 친구 목록, 차단 목록, ...
	//
		PI_CS_COMMUNITY = PI_CS_LOGIC + 1700,

		// 친구 목록
		FRIEND_CS_GETLIST = PI_CS_COMMUNITY,
		FRIEND_SC_GETLIST,
		// UINT8			nCount;
		// FRIEND_INFO		friendInfo[];
		// UINT8			uiRefreshPeriodSec;		// 0(disable)

		// 친구 추가 요청
		FRIEND_CS_ADD,
		// UINT8			len;
		// TCHAR			szCharName[];

		// 친구 추가 요청 결과
		FRIEND_SC_ADD, 
		// GLOBAL_STRING_ID	itemID;					// GLOBAL_STRING.LDT 상의 메세지를 itemID 값
		// UINT8			len;
		// TCHAR			szAccepter[];

		// 친구 삭제
		FRIEND_CS_DELETE,
		// UINT8			len;
		// TCHAR			szCharName[];
		FRIEND_SC_DELETE,
		// GLOBAL_STRING_ID	itemID;					// GLOBAL_STRING.LDT 상의 메세지를 itemID 값

		// 해당 캐릭터의 상태(ON/OFF LINE,Level) 갱신 요청
		FRIEND_CS_REFRESH_STATUS,
		// UINT8			nCount;
		// TCHAR			charNames[][LEN_NAME];
		FRIEND_SC_REFRESH_STATUS,
		// UINT8			uiRefreshPeriodSec;		// 1<= uiNextRequestSec<=255.  // 0(disable)
		// UINT8			nCount;
		// FRIEND_INFO		friendInfo[];

		// 차단 목록
		BLOCK_CS_GETLIST,
		BLOCK_SC_GETLIST,
		// UINT8			nCount;
		// TCHAR			szCharName[][LEN_NAME];

		//차단 목록에 추가
		BLOCK_CS_ADD,
		// UINT8			len;
		// TCHAR			szCharName[];
		BLOCK_SC_ADD,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT 상의 메세지를 itemID 값
		// UINT8			len;
		// TCHAR			szCharName[];

		// 차단 목록에서 삭제
		BLOCK_CS_DELETE,
		// UINT8			len;
		// TCHAR			szCharName[];
		BLOCK_SC_DELETE,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT 상의 메세지를 itemID 값

		// 상대(szTargetName)의 차단 목록상에 내가 등록되었는가?
		BLOCK_CS_ISBLOCKED,
		// UINT8			len;
		// TCHAR			szTargetName[];
		BLOCK_SC_ISBLOCKED,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT 상의 메세지를 itemID 값
		// GU_ID			guid;				// 요청한 당사자의 GU_ID
		// bool				bWedding;			// 결혼정보가 있을때 밑에 정보를 같이 보낸다
		// UINT8			iWeddingLv;			// 결혼레벨
		// int				iWeddingDay;		// 교제일수 또는 결혼일수
		// TCHAR			szWeddingPartName[LEN_NAME+1]; //결혼상대방이름

		// 수신자에게 친구추가 요청이 들어왓을때
		FRIEND_SC_ADD_RECV,
		// GLOBAL_STRING_ID iErrorMsg;
		// USERID			iRequesterUserID;
		// CHARID			iRequesterCharID;
		// UINT8			iLen;
		// TCHAR			szRequesterName[];
	
		// 
		FRIEND_CS_ACCEPT,
		// UINT8			iFlag;				// 0: 취소 1: 거부 2: 동의
		// USERID			iRequesterUserID;
		// CHARID			iRequesterCharID;
		// UINT8			iLen;
		// TCHAR			szRequesterName[];

		FRIEND_SC_ACCEPT,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT 상의 메세지를 itemID 값
		// FRIEND_INFO		friendInfo;			// 

		// 해당 캐릭터의 상태(ON/OFF LINE,Level) 갱신 요청
		FRIEND_CS_REFRESH_STATUS2,
		// UINT8			nCount;
		// UINT32			uiFriendCharIDs[];

	//////////////////////////////////////////////////////////////////////////
	//
	//	SKILL
	//
		PI_CS_SKILL = PI_CS_LOGIC + 1800,

		// 유저가 소유한 스킬 정보.
		SKILL_SC_INFOS = PI_CS_SKILL, 
		// GU_ID			iInstanceID
		// unsigned short	iNumOfSkill
		// sizeof(CHAR_SKILL)*iNumOfSkill

		// 스킬포인트를 환원하고 초기화.
		SKILL_SC_INIT, 
		// GU_ID			iInstanceID
		// short			iSkillFreePoint;	

		// 스킬 사용.
		SKILL_CS_USING, 
		// int iSkillID // 스킬 아이디.
		// UINT8 facing

		// 스킬 사용 결과.
		SKILL_SC_USING,
		// RESULTCODE		resultCode;
		// GU_ID			iInstanceID;
		// UINT32			iSkillID;
		// LT_POSITION		pos;

		// 아이템스킬 사용.
		ITEMSKILL_CS_USING, 
		// CONTAINER_SLOT	stItemPos;	// 아이템정보
		// int				iSkillID;	// 스킬 아이디.
		// UINT8			facing;

		// 아이템스킬 사용 결과.
		ITEMSKILL_SC_USING,
		// GU_ID			iInstanceID;
		// GLOBAL_STRING_ID	ErrorID;		// 에러아이디(0이면 에러없음)
		// ITEM_SP			stItemSP;		// 아이템정보 / 갱신용 SP정보
		// int				iSkillID;
		// LT_POSITION		stPCPos;

		// 스킬에 프리포인트를 배분하여 레벨업을 한다.
		SKILL_CS_LVUP,
		// int iSkillID // 스킬 아이디.

		SKILL_SC_LVUP,
		// RESULTCODE		resultCode;
		// GU_ID			iInstanceID;
		// short			iSkillFreePoint;
		// if(0 == resultCode)
			// CHAR_SKILL		SkillData;

		// 토글스킬오프, 결과는 EFFECT_SC_DEL로 보냅니다.
		SKILL_CS_OFFTOGGLE, 
		// int iSkillID // 스킬 아이디.

		// 아이템스킬을 이용한 아이템판매. 결과는 STAT_SC_CHANGE_EX, ITEM_SC_TRASH 패킷으로 클라이언트에 송신합니다.(현재 아이템상점판매 구조와 동일)
		ITEMSKILL_CS_SELL, 
		// CONTAINER_SLOT	stSkillItemPos;	// 상점판매특수스킬을 가지고 있는 장착한아이템정보
		// int				iSkillID;		// 상점판매 속성을 가지고 있는 스킬번호
		// CONTAINER_ITEM	stSellItem;		// 판매할 아이템정보

		SKILL_CS_LEARN,
		// int iSkillID;
		// int			iUseEly;
		// TRASH_ITEM	stUseItem;


		SKILL_SC_LEARN,
		// GLOBAL_STRING_ID	iErrorMsg;
		// GU_ID			iInstanceID;
		// CHAR_SKILL       stSkillInfo;
		// __int64			iCurEly;
		// TRASH_ITEM		stUseItem;
	
		SKILL_SC_INIT_ITEMMIX,
		// GU_ID			iInstanceID
		// ITEMMIX_INFO     stItemMixInfo;	

		SKILL_SC_INIT_SELECTIVE,
		// GU_ID			iInstanceID
		// UINT32			iMaskingFlag;

	//////////////////////////////////////////////////////////////////////////
	//
	//	EFFECT
	//
		PI_CS_EFFECT = PI_CS_LOGIC + 1900,

		// 현재 Char에걸린 이팩트정보.
		EFFECT_SC_INFOS = PI_CS_EFFECT,
		// GU_ID			iInstanceID; // pc
		// GU_ID			iInstanceID; // Caster
		// UINT16			iNumOfEffect;
		// sizeof(EFFECT_DATA)*iNumOfEffect;

		// 현재 Char에 걸린 이팩트 정보를 지우기.
		EFFECT_SC_DEL,  
		// GU_ID			iInstanceID; // pc
		// GU_ID			iInstanceID; // Caster
		// UINT16			iNumOfEffect;
		// sizeof(int)*iNumOfEffect;	// iEffectInstanceID;

	//////////////////////////////////////////////////////////////////////////
	//
	// Team
	//
		PI_CS_TEAM = PI_CS_LOGIC + 2000,

		// 팀초대
		TEAM_CS_REQUEST = PI_CS_TEAM,
		// char				szRcvCharName[LEN_NAME+1];	// 대상자이름

		// 팀초대 결과. 에러인경우는 요청자에게만 송신, 성공인경우는 대상자에게만 송신
		TEAM_SC_REQUEST,	// 요청자와 대상자에게 송신
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// char				szCharName[LEN_NAME+1];		// 이름
		// GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)

		// 팀참여응답
		TEAM_CS_JOIN,
		// char				szActCharName[LEN_NAME+1];	// 요청자이름

		// 팀참여응답 결과
		TEAM_SC_JOIN,		// 실패면 대상자에게 송신, 성공이면 모든 팀원에게 송신
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// char				szActCharName[LEN_NAME+1];	// 요청자이름
		// GU_ID			RcvCharGUID;				// 대상자
		// char				szRcvCharName[LEN_NAME+1];	// 대상자이름
		// GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)
		// UINT8			byLootingRule;				// 루팅룰(0:차례대로,1:랜덤,2:자유)
		// int				iCnt;						// 팀원수
		// TEAM_MEMBER		stTeam_Member;				// 팀원정보
		// ...											// 팀원수만큼

		// 팀탈퇴
		TEAM_CS_SECEDE,

		// 팀탈퇴 결과.(팀월드화)
		TEAM_SC_SECEDE,		// 모든 팀원에게 송신, *** 주의 : 팀원이 2명인 경우에 패킷을 받으면 파티해제해야합니다.
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// INT32			iActCharID;					// 탈퇴자캐릭터DB키

		// 강퇴(팀월드화)
		TEAM_CS_EXPEL,
		// INT32			iRcvCharID;					// 대상자캐릭터DB키

		// 강퇴결과(팀월드화)
		TEAM_SC_EXPEL,		// 모든 팀원에게 송신, *** 주의 : 팀원이 2명인 경우에 패킷을 받으면 파티해제해야합니다.
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// INT32			iRcvCharID;					// 강퇴자캐릭터DB키

		// 팀장위임(팀월드화)
		TEAM_CS_CHANGECAPTAIN,
		// INT32			iRcvCharID;					// 대상자캐릭터DB키

		// 팀장위임 결과(팀월드화)
		TEAM_SC_CHANGECAPTAIN,	// 모든 팀원에게 송신
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// INT32			iRcvCharID;					// 새팀장캐릭터DB키

		// 루팅룰변경
		TEAM_CS_CHANGELOOTINGRULE,
		// UINT8			byLootingRule;				// 새루팅룰

		// 루팅룰변경 결과
		TEAM_SC_CHANGELOOTINGRULE,	// 모든 팀원에게 송신
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// UINT8			byLootingRule;				// 새루팅룰

		// 팀원정보갱신 결과(서버에서 보내기만 함) : 어떤시점에 보낼것인지 필요
		// 2006.04.13 dhpark 월드맵의 위치표시를 위해서 위치정보가 추가되고 클라이언트가 요청할수있도록 수정합니다.
		TEAM_SC_UPDATEMEMBER,	// 모든 팀원에게 송신. *** 주의 : 브로드캐스팅영역 밖의 팀원만 갱신합니다. 영역안의 팀원은 브로드캐스팅정보가 더 정확합니다.
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// TEAM_MEMBER		stTeam_Member;				// 팀원정보

		// 팀원정보갱신 요청. 월드맵을 열고 있는 팀원은 일정주기마다 다른팀원의 정보를 요청합니다.
		TEAM_CS_UPDATEMEMBER,
		// GU_ID			RcvCharGUID;				// 정보가 갱신될 팀원

		// 팀정보송신(채널이동한 유저에게 팀정보 송신시 사용)
		TEAM_SC_INFO,
		// GU_ID					ObjectGUID;						// 게임오브젝트아이디
		// UINT8					byLootingRule;					// 루팅룰(0:차례대로,1:랜덤,2:자유)
		// int						iCnt;							// 팀원수
		// TEAM_MEMBER				stTeam_Member;					// 팀원정보
		// ...														// 팀원수만큼

	//////////////////////////////////////////////////////////////////////////
	//
	//	Event Box 
	//
		PI_CS_GIFT = PI_CS_LOGIC + 2100,

		// gift 보관함에 존재하는 gift리스트를 요청.
		GIFT_CS_LIST = PI_CS_GIFT,

		// gift 보관함에 존재하는 gift요청 결과.
		GIFT_SC_LIST,
		// GU_ID				CharGUID;		// 주체자
		// GLOBAL_STRING_ID		iError ;		// 0: 성공
		// int					iNumOfGiftID;	// 받아야 할 gift 갯수
		// __int64				iGiftNO;		// gift key ID					
		// int					iGiftID;		// GIFT.LDT 참조	
		
		// gift 보관함에서 1개의 gift 받기 요청.
		GIFT_CS_GET,
		// __int64				iGiftNO;

		// gift 보관함에서 1개의 gift 받기 요청 결과
		GIFT_SC_GET,
		// GU_ID				CharGUID;		// 주체자
		// GLOBAL_STRING_ID		iError ;		// 0: 성공
		// __int64				iGiftNO;
		// __int64				iCurrMoney;		// 돈갱신
		// UINT8				iItemCount;		// 아이템 생성 갯수
		// CONTAINER_ITEM		tContainerItem;	// 생성 아이템 정보

	//////////////////////////////////////////////////////////////////////////
	//
	//	MailBox
	//
		PI_CS_MAILBOX = PI_CS_LOGIC + 2200,

		// 메일 목록 요청
		MAILBOX_CS_GETLIST	= PI_CS_MAILBOX,
		// TCHAR					szCharName[LEN_NAME+1];					// null included
		// UINT32					uiMailBoxID;
		MAILBOX_SC_GETLIST,
		// GLOBAL_STRING_ID			iError;
		// UINT8					nMailCount;				// 수신 메일 갯수
			// nMailCount 반복
			//{
				// MAIL_HEADER_ATTACHMENT	mailHeaderInfo[];		// 메일헤더 정보
				// UINT8			uiSubjectLen;
				// TCHAR			szSubect[];
				// CONTAINER_ITEM	containerItem;				
			//}
		
		// 메일 전송 (클라이언트에서의 전송은 한번에 하나씩만 가능하게 블럭됨. (전송 - 결과 수신 - 다음 전송)
		MAILBOX_CS_SEND,
		// MAIL_HEADER_ATTACHMENT	mailHeaderInfo[];		// 메일헤더 정보
		// UINT8					uiSubjectLen;			// 제목
		// TCHAR					szSubect[];
		// UINT8					uiMemoLen;				// 본문
		// TCHAR					szMemo[];
		// TCHAR					szReceiver[LEN_NAME+1];	// 수신자
		// UINT32					uiMailBoxID;
		// CONTAINER_SLOT			UsingItemPos;		// 사용할 아이템 위치정보
		// ITEMNO					iUsingItemNo;		// 사용할 아이템 키번호

		MAILBOX_SC_SEND,
		// GLOBAL_STRING_ID			iError;

		// if iError == 0
			// UINT8				flagTrashItem;			// 0(NONE),1(Item를 인벤에서 삭제할 것)
			// TRASH_ITEM			AttachItem;				// 첨부아이템
			// UINT64				uiRemainEly;			// 메일 전송후 잔여 Ely량
			// TRASH_ITEM			UsingItem;				// 우표아이템
		// else
			// TCHAR				szReceiver[LEN_NAME+1];	// if iError != 0(오류시)

		// 메일 삭제
		MAILBOX_CS_DELETE,
		// UINT64					uiMailID;
		// UINT32					uiMailBoxID;
		MAILBOX_SC_DELETE,
		// GLOBAL_STRING_ID			iError;
		// UINT64					uiMailID;

		// 메일 (본문) 수신
		MAILBOX_CS_GETBODY,
		// UINT64					uiMailID;
		// UINT32					uiMailBoxID;
		MAILBOX_SC_GETBODY,
		// GLOBAL_STRING_ID			iError;
		// UINT64					uiMailID;
		// UINT8					uiLen;
		// TCHAR					szMemo[];

		// 첨부된 정보 처리 
		// 첨부된 Ely,Item를 나의 인벤과 잔고에 추가한다.(Attachment 정보는 소거,Memo는 유지. -->  차후 클라이언트의 명시적인 삭제 요청에 의해 메일 삭제됨)
		MAILBOX_CS_MOVEATTACHMENT,
		// UINT64					uiMailID;
		// CONTAINER_ITEM			containerItem;				
		// UINT32					uiMailBoxID;
		MAILBOX_SC_MOVEATTACHMENT,						// Item 이동 정보(InsertITem), Ely변화량은 별도의 패킷으로 처리됨
		// GLOBAL_STRING_ID			iError;
		// UINT64					uiMailID;
		// UINT64					uiRemainEly;
		// UINT8					uiItemCount;		// 아이템 생성 갯수
		// CONTAINER_ITEM			containerItem;		// 생성 아이템 정보
		// UINT8					uiMailDeleted;		// Memo 내용이 없는 경우 자동 삭제됨

		// New Mail arrived
		MAILBOX_SC_NEWMAIL,
		// GLOBAL_STRING_ID			iError;
		// UINT8					nMailCount;			// 새로 수신한 메일 갯수 (현재는 항상 1)
		// MAIL_HEADER_ATTACHMENT	mailHeaderInfo[];	// 메일헤더 정보
		// UINT8					uiLen;
		// TCHAR					szSubject[];
		// CONTAINER_ITEM			containerItem;		// 추가 아이템 정보

	//////////////////////////////////////////////////////////////////////////
	//
	//	Cash Shop 
	//
		PI_CS_CASHSHOP = PI_CS_LOGIC + 2300,

		// 캐쉬샵 진입
		CASHSHOP_CS_IN = PI_CS_CASHSHOP,
		// int					iMajor;  // 추가 : GLOBAL_SYS : 21번 값. 
		// int					iMinor;  // 추가 : 클라이언트에서 최종 받은 마이너 버전. 채널 이동시에도 유지되었으면 좋겠으나... 
		// int					iHotVer; // 추가

		CASHSHOP_SC_IN,
 		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;
		// int					iMinor;  // 추가 
		// int					iHotVer; // 추가

			// for GamePot
				// UINT8		uiTokenLen;
				// if uiTokenLen > 0
					// TCHAR	szToken[];

			// other
				// UINT64		uiDID;
				// int    nCount;             //[xialin 2008/07/30] add桿누션쩌
				// ITEMID nItems[nCount];


		// 캐쉬샵 이탈
		CASHSHOP_CS_OUT,
		// int nCount;		//[xialin 2008/07/30] add桿누션쩌
		// ITEMID nItems[nCount];

		CASHSHOP_SC_OUT,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;

		// 캐쉬정보/인벤아이템정보요청.
		CASHSHOP_CS_INFO,

		// 캐쉬정보/인벤아이템 정보 요청 결과
		CASHSHOP_SC_INFO,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;
		// int					iCurrOz;
		// int					iCurrMileage;
		// UINT16				nItems;
		// CONTAINER_ITEM		containerItem;

		// 캐쉬정보 갱신 요청.
		CASHSHOP_CS_CASH,
		
		// 캐쉬정보 갱신 요청 결과.
		CASHSHOP_SC_CASH,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;
		// int					iCurrOz;
		// int					iCurrMileage;

		// CASHSHOP 에서 구입.
		CASHSHOP_CS_BUY,
//new
		// int					iPackageID;				//CASH_SHOP.LDT::ItemID
		//-------------------------------------------   // 유효성 검사를 위해
		// int					iShopPrice;				//CASH_SHOP.LDT::_CashPrice
		// int					iSalePrice;				//CASH_SHOP.LDT::_SalePrice
		// int					iMaxOz;					//CASH_SHOP.LDT::_MaxOz
		// int					iMaxMileage;			//CASH_SHOP.LDT::_MaxMileage
		//--------------------------------------------  // 유효성 검사를 위해
		// int					iBuyOz;					// 현금투자가.
		// int					iBuyMileage;			// 마일리지 투자가.

		// CASHSHOP 에서 구입결과.
		CASHSHOP_SC_BUY,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;
		// int					iPackageID;				//CASH_SHOP.LDT::ItemID 
		// int					iCurrOz;				// 현재 오즈
		// int					iCurrMileage;			// 현재 마일리지.
		// UINT16				nItems;					// 신규 생성 아이템 갯수
		// CONTAINER_ITEM		containerItem;			// 신규 생성 아이템 정보.

		// CASHSHOP 에 팔기. // stack 이 있는 건 팔수 없음. 기본 1개짜리만 되팔기가 됨. 
		CASHSHOP_CS_SELL,
		// CONTAINER_ITEM		tContainerItem;         // 어느 위치에 있는 놈을 // 무조건 슬롯 단위이며 // MaxStack 이 1인것만 가능

		// CASHSHOP 에 팔기 결과.
		CASHSHOP_SC_SELL,
		// GLOBAL_STRING_ID		iError ;		// 0: 성공
		// GU_ID				globalID;
		// int					iCurrMileage;	// 마일리지로 환산 되므로 거의 변화 되어야 함.
		// TRASH_ITEM			tTrashItem;		// 삭제되어야 할 아이템 정보
		// UINT8				iSkipUpdate;	// 잔고를업데이트 할지여부 0: 업데이트 1: 업데이트안함


		// 쿠폰 등록
		CASHSHOP_CS_COUPON,
		//UINT8	len;
		//TCHAR szCouponCode[];

		CASHSHOP_SC_COUPON,
		// GLOBAL_STRING_ID		iError ;		// 0: 성공
			//iError == 9002003
				// int iMilage;					// iMilage 마일리지를 획득하셨습니다.		
				// int iRemainMilage;			// 총 마일리지 잔고
			//iError == 9002004
				// UINT32	uiGiftID;			// uiGiftID을 획득하셨습니다. Gift 를 확인하세요....
			//iError == 9002010
				// int iOz;						// iOz를 획득하셨습니다.		
				// int iRemainOz;				// 총 잔고

		// CASHSHOP 에서 구입.
		CASHSHOP_CS_BUY_PRESENT,
//new
		// TCHAR				TargetCharName[LEN_NAME];
		// int					iPackageID;				//CASH_SHOP.LDT::ItemID
		//-------------------------------------------   // 유효성 검사를 위해
		// int					iShopPrice;				//CASH_SHOP.LDT::_CashPrice
		// int					iSalePrice;				//CASH_SHOP.LDT::_SalePrice
		// int					iMaxOz;					//CASH_SHOP.LDT::_MaxOz
		// int					iMaxMileage;			//CASH_SHOP.LDT::_MaxMileage
		//--------------------------------------------  // 유효성 검사를 위해
		// int					iBuyOz;					// 현금투자가.
		// int					iBuyMileage;			// 마일리지 투자가.
		// TCHAR				Comment[MAX_MAIl_BODY_LEN] // 선물 내용.					

		// CASHSHOP 에서 구입결과.
		CASHSHOP_SC_BUY_PRESENT,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;
		// TCHAR				TargetCharName[LEN_NAME];
		// int					iPackageID;				//CASH_SHOP.LDT::ItemID 
		// int					iCurrOz;				// 현재 오즈
		// int					iCurrMileage;			// 현재 마일리지.

		// 찜리스트 송신(CASHSHOP_CS_INFO에 의해서 자동으로 클라이언트에 송신)
		CASHSHOP_SC_WISH_LIST,
		// int					iCnt;			// 찜개수
		// int					iPackageID;		// 패키지아이디
		// ...									// 찜개수만큼

		// 찜추가
		CASHSHOP_CS_WISH_ADD,
		// int					iPackageID;		// 패키지아이디

		// 찜추가
		CASHSHOP_SC_WISH_ADD,
		// GLOBAL_STRING_ID		iErrorMsg;
		// int					iPackageID;		// 패키지아이디(성공일때만)

		// 찜삭제
		CASHSHOP_CS_WISH_DEL,
		// int					iPackageID;		// 패키지아이디

		// 찜삭제
		CASHSHOP_SC_WISH_DEL,
		// GLOBAL_STRING_ID		iErrorMsg;
		// int					iPackageID;		// 패키지아이디(성공일때만)

		// 추가
		CASHSHOP_SC_MINOR_INIT,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;

		// 추가
		CASHSHOP_SC_MINOR_INFO,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;
		// UINT16				nShopInfo;
		// CASHSHOP_INFO		ShopInfos[];		

		CASHSHOP_SC_OUT_READY,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;



		//20080407 mijin 로딩시 캐쉬샵,추천상품 버전 체크
		CASHSHOP_CS_VERSION,
		// int					iMajor;  				//캐쉬샵 Major버전
		// int					iMinor;  				//캐쉬샵 Minor버전
		// int					iHotVer;				//추천상품 버전

		//20080407 mijin 캐쉬샵,추천상품 버전이 틀리면 데이터를 보낸다
		CASHSHOP_SC_VERSION,
		// GLOBAL_STRING_ID			iErrorMsg;
		// GU_ID				globalID;
		// int					iMinor;  				//캐쉬샵 Minor버전
		// int					iHotVer;				//추천상품 버전



		//20080407 mijin 필드에서 물품 가격 체크
		CASHSHOP_CS_PRICE,
		// int					iHotVer;				//추천상품 버전
		// int					iPackageID;				//CASH_SHOP.LDT::ItemID
		// int					iShopPrice;				//CASH_SHOP.LDT::_CashPrice
		// int					iSalePrice;				//CASH_SHOP.LDT::_SalePrice
		// int					iMaxOz;					//CASH_SHOP.LDT::_MaxOz
		// int					iMaxMileage;				//CASH_SHOP.LDT::_MaxMileage

		//20080407 mijin 필드에서 물품 가격 체크 결과
		CASHSHOP_SC_PRICE,
		// GLOBAL_STRING_ID			iErrorMsg;
		// GU_ID				globalID;
		// int					iHotVer;				//추천상품 버전
		// int					iPackageID;				//CASH_SHOP.LDT::ItemID 
		// int					iShopPrice;				//CASH_SHOP.LDT::_CashPrice
		// int					iSalePrice;				//CASH_SHOP.LDT::_SalePrice
		// int					iMaxOz;					//CASH_SHOP.LDT::_MaxOz
		// int					iMaxMileage;				//CASH_SHOP.LDT::_MaxMileage



		//추천상품 버전이 틀릴경우 데이터를 보낸다
		CASHSHOP_SC_HOT_INFO,
		// GLOBAL_STRING_ID			iErrorMsg;
		// GU_ID					globalID;
		// UINT16					nHotInfo;
		// CASHSHOP_HOT_INFO		HotInfos[];


		// 지엠툴에 의한 추천상품 변경상태를 모든 pc들에게 전송
		CASHSHOP_SC_HOT_STATUS,
		// UINT8					iHotCashShopMode;

		CASHSHOP_CS_EFFECT,
		//int					iSeqID;						//와빵똥SeqID
		//int					iID;						//CASH_EFFECT.LDT::nID
		//UINT8					iOptLen;					//맒숭斤口낀똑
		//TCHAR					szOpt[];					//맒숭斤口

		CASHSHOP_SC_EFFECT,
		//int					iSeqID;						//와빵똥SeqID
		//GLOBAL_STRING_ID		iErrorMsg;

		//	jmulro 우醵뭔찜，2008－8－26  ---start
		CASHSHOP_CS_GET_QUICK_BUY_INFO,	
		CASHSHOP_SC_GET_QUICK_BUY_INFO,
		//	GLOBAL_STRING_ID			iErrorMsg;
		//	UINT uInfoCount;
		//	t_Quick_Buy_Info info[];
		CASHSHOP_CS_QUICK_BUY_BUY_ITEM,
		//	int					nPackageID;
		//-------------------------------------------   // 유효성 검사를 위해
		// int					iShopPrice;				//CASH_SHOP.LDT::_CashPrice
		// int					iSalePrice;				//CASH_SHOP.LDT::_SalePrice
		// int					iMaxOz;					//CASH_SHOP.LDT::_MaxOz
		// int					iMaxMileage;			//CASH_SHOP.LDT::_MaxMileage
		//--------------------------------------------  // 유효성 검사를 위해
		// int					iBuyOz;					// 현금투자가.
		// int					iBuyMileage;			// 마일리지 투자가.		
		//	jmulro 우醵뭔찜，2008－8－26 ---end

		CASHSHOP_CS_ASK_BUY,
		// UINT8				nPackageID;					// iPackageID돨鑒좆
		// int					iPackageID[];				// CASH_SHOP.LDT::ItemID
		// UINT8				len;						// szMsg낀똑
		// TCHAR				szMsg[];					// 句口
		// UINT8				nReceiverName;				// szReceiverName돨낀똑
		// TCHAR				szReceiverName[LEN_NAME];	// 굳헝헹鯤소츰俚

		CASHSHOP_SC_ASK_BUY,
		// GLOBAL_STRING_ID		iErrorMsg;
		// UINT8				nPackageID;					// iPackageID돨鑒좆
		// int					iPackageID[];				// CASH_SHOP.LDT::ItemID
		// UINT8				nSenderName;				// szSender돨낀똑
		// TCHAR				szSender[];					// 랙箇諒츰俚

		CASHSHOP_SC_ASK_BUY_RECV,
		// UINT8				nPackageID;					// iPackageID돨鑒좆
		// int					iPackageID;					// CASH_SHOP.LDT::ItemID
		// UINT8				iLen;						// szReceiverName돨낀똑
		// TCHAR				szRequesterName[LEN_NAME];	// 굳헝헹鯤소츰俚
		// UINT8				len;						// szMsg낀똑
		// TCHAR				szMsg[];					// 句口

	//////////////////////////////////////////////////////////////////////////
	//
	//	Title
	//
		PI_CS_TITLE = PI_CS_LOGIC + 2400,

		// 타이틀 리스트. 타이틀의 수가 많으면 패킷당 일정수만큼만 여러번 송신합니다. 
		TITLE_SC_LIST = PI_CS_TITLE,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// int				iCnt;			// 타이틀 개수
		// UINT32			uiTitleID;		// 타이틀 아이디
		// ...								// 타이틀 개수만큼

		// 타이틀 추가
		TITLE_SC_INSERT,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// UINT32			uiTitleID;		// 습득한 타이틀아이디

		// 타이틀 변경
		TITLE_CS_CHANGE,
		// UINT32			uiTitleID;		// 장착할 타이틀아이디

		// 타이틀 변경
		TITLE_SC_CHANGE,	// 주변에 브로드캐스트해야함
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// UINT32			uiTitleID;		// 장착할 타이틀아이디

	//////////////////////////////////////////////////////////////////////////
	//
	//	FreeMarket
	//
		PI_CS_FREEMARKET = PI_CS_LOGIC + 2500,

		// 개인상점 열기
		FREEMARKET_CS_OPEN = PI_CS_FREEMARKET,
		// UINT8			byType;			// 상점타입(이값에 해당하는 조건검사/페이지수등을 검사합니다.)
		// char				szFMName[FM_MAX_NAME+1]; // 상점명
		// int				iCnt;			// 판매아이템 개수
		// FM_SELLITEM		stFM_SellItem;	// 판매아이템 정보
		// ...								// 판매아이템 개수만큼

		// 개인상점 열기
		FREEMARKET_SC_OPEN,		// 주변에 브로드캐스트해야함
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디(상점주인)
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디(상점주인)
		// GLOBAL_STRING_ID	ErrorID;		// 에러아이디(0이면 에러없음)
		// UINT8			byType;			// 상점타입
		// char				szFMName[FM_MAX_NAME+1]; // 상점명

		// 개인상점 닫기
		FREEMARKET_CS_CLOSE,

		// 개인상점 닫기
		FREEMARKET_SC_CLOSE,	// 주변에 브로드캐스트해야함
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디(상점주인)
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디(상점주인)
		// GLOBAL_STRING_ID	ErrorID;		// 에러아이디(0이면 에러없음)

		// 개인상점에 들어가기
		FREEMARKET_CS_IN,
		// GU_ID			FMGUID;			// 상점주인의 게임오브젝트아이디

		// 개인상점에 들어가기
		FREEMARKET_SC_IN,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;		// 에러아이디(0이면 에러없음)
		// int				iCnt;			// 판매아이템 개수
		// FM_VIEWITEM		stFM_ViewItem;	// 판매아이템 정보
		// ...								// 판매아이템 개수만큼

		// 개인상점에서 나가기
		FREEMARKET_CS_OUT,
		// GU_ID			FMGUID;			// 상점주인의 게임오브젝트아이디

		// 개인상점에서 나가기
		FREEMARKET_SC_OUT,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디

		// 개인상점에서 물건 사기
		FREEMARKET_CS_BUY,
		// GU_ID			FMGUID;			// 상점주인의 게임오브젝트아이디
		// FM_ITEM			stFM_Item;		// 사고싶은 아이템 정보

		// 개인상점에서 물건 사기
		FREEMARKET_SC_BUY,	// 에러일때는 당사자에게만 보내고 성공일때는 손님들 모두에게 보내서 판매아이템목록을 갱신하게 합니다.
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디(물건을산손님)
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디(물건을산손님)
		// GLOBAL_STRING_ID	ErrorID;		// 에러아이디(0이면 에러없음)
		// FM_ITEM			stFM_Item;		// 판매된 아이템 정보

	//////////////////////////////////////////////////////////////////////////
	//
	// Festival ( 게임내에서 진행되는 각종 이벤트)
	//
		PI_CS_FESTIVAL = PI_CS_LOGIC + 2600,

	//////////////////////////////////////////////////////////////////////////
	//
	//	WayPoint
	//
		PI_CS_WAYPOINT = PI_CS_LOGIC + 2700,

		// 웨이포인트 리스트
		WAYPOINT_SC_LIST = PI_CS_WAYPOINT,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// int				arrWayPoint[];	// 웨이포인트데이터

		// 새 웨이포인트 발견
		WAYPOINT_CS_FIND,
		// int				iWayPointID;		// 웨이포인트아이디

		// 새 웨이포인트 발견
		WAYPOINT_SC_FIND,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;		// 에러아이디
		// int				iWayPointID;	// 웨이포인트아이디

		// 선택한 웨이포인트로 이동
		WAYPOINT_CS_MOVE,
		// int				iWayPointID;		// 웨이포인트아이디
		// UINT8			byRank;				// 0, 1

		// 선택한 웨이포인트로 이동
		WAYPOINT_SC_MOVE,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;		// 에러아이디
		// int				iWayPointID;	// 웨이포인트아이디

	//////////////////////////////////////////////////////////////////////////
	//
	//	Guild
	//
		PI_CS_GUILD = PI_CS_LOGIC + 2800,

		// PC로그인시 서버가 보내주는 PC가 소속된 길드정보
		GUILD_SC_PCGUILD = PI_CS_GUILD,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// PCGUILD			stPCGuild;							// 캐릭터 길드정보

		// 일반 길드정보
		GUILD_CS_GUILD_COMMON,
			
		// 일반 길드정보
		GUILD_SC_GUILD_COMMON,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		// GUILD_COMMON		stGuildCommon;						// 일반 길드정보

		// 길드 멤버 리스트
		GUILD_CS_MEMBERLIST,
			
		// 길드 멤버 리스트
		GUILD_SC_MEMBERLIST,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		// INT32			iMemberCnt;							// 길드 멤버수
		// GUILD_MEMBER		stGuildMember;						// 길드 멤버정보
		// ...
	
		// 길드 생성
		GUILD_CS_CREATE,
		// char				szGName[GUILD_MAX_NAME+1];			// 길드명
		// UINT8			byGType;							// 길드타입(1:일반, 2:천사, 3:악마)
		// UINT8			arrGMark[3];						// 길드마크
		// UINT8			byGRank;							// 길드랭크(타입과 랭크를 묶어서 LDT키로 사용합니다.)
			
		// 길드 생성(성공시 브로드캐스트되어야함)
		GUILD_SC_CREATE,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		// PCGUILD			stPCGuild;							// 캐릭터 길드정보

		// 길드 파괴
		GUILD_CS_DESTROY,
			
		// 길드 파괴(성공시 브로드캐스트되어야함(주변사람들이 길드가 사라졌다는것을 알아야함). 길드원은 이미 길마만 남은 상황이므로 고려하지 않음)
		GUILD_SC_DESTROY,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디

		// 길드 슬로건 수정
		GUILD_CS_UPDATE_SLOGAN,
		// char				szGSlogan[GUILD_MAX_SLOGAN+1];		// 길드슬로건

		// 길드 슬로건 수정
		GUILD_SC_UPDATE_SLOGAN,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디

		// 길드초대
		GUILD_CS_REQUEST,
		// char				szRcvCharName[LEN_NAME+1];			// 대상자이름

		// 길드초대 결과. 에러인경우는 요청자에게만 송신, 성공인경우는 대상자에게만 송신. szCharName(에러인경우는 대상자명, 성공인경우는 요청자명)
		GUILD_SC_REQUEST,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// char				szGName[GUILD_MAX_NAME+1];			// 길드명
		// char				szCharName[LEN_NAME+1];				// 성공인 경우는 요청자명, 실패인 경우는 대상자명
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)

		// 길드참여응답
		GUILD_CS_JOIN,
		// char				szActCharName[LEN_NAME+1];			// 요청자이름

		// 길드참여응답
		GUILD_SC_JOIN,		// 실패면 대상자에게 송신, 성공이면 주변에 브로드캐스팅
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)
		// PCGUILD			stPCGuild;							// 캐릭터 길드정보

		// 길드탈퇴
		GUILD_CS_SECEDE,

		// 길드탈퇴 결과.
		GUILD_SC_SECEDE,	// 주위에 브로드캐스팅되어야함. 
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)

		// 길드강퇴
		GUILD_CS_EXPEL,
		// INT32			iRcvCharID;							// 강퇴대상자의 DBNum

		// 길드강퇴결과
		GUILD_SC_EXPEL,		// 요청자에게 보내는 요청에 대한 결과
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)

		// 길드강퇴결과
		GUILD_SC_EXPEL_ME,	// 강퇴대상자 주위에게 송신(강퇴대상자의 길드가 사라져야하므로).
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디

		// 직책변경
		GUILD_CS_CHANGEGRADE,
		// INT32			iRcvCharID;							// 직책변경대상자의 DBNum
		// UINT8			byGrade;							// 변경 직책

		// 직책변경
		GUILD_SC_CHANGEGRADE, // 요청자에게 보내는 요청에 대한 결과
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)

		// 직책변경
		GUILD_SC_CHANGEGRADE_ME, // 대상자에게 보내는 통보패킷. 직책은 보여지는 정보가 아니므로 브로드캐스팅하지 않음
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// UINT8			byGrade;							// 변경 직책

		// 멤버정보변경 메세지
		GUILD_SC_MSG_MEMBER,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// UINT8			byMemberMsgType;					// 멤버메세지타입(0:가입, 1:탈퇴, 2:강퇴, 3:직위변동, 4:로그인, 5:로그아웃)
		// GUILD_MEMBER		stGuildMember;						// 멤버정보

		// 길드정보변경 메세지
		GUILD_SC_MSG_GUILD,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// UINT8			byGuildMsgType;						// 길드메세지타입(0:슬로건변경, 1:랭크변경)
		// GUILD_COMMON		stGuildCommon;						// 일반적인 길드정보

		// 길드권한위임
		GUILD_CS_CHANGEMASTER,
		// INT32			iRcvCharID;							// 대상자캐릭터DB키

		// 요청자에게 보내는 길드권한위임 결과. 에러인경우는 요청자에게만 송신, 성공인경우는 요청자주변에 브로드캐스팅(예전길마의 길드탈퇴를 주변사람들에게 알리는 목적)
		GUILD_SC_CHANGEMASTER,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)

		// 대상자에게 보내는 길드권한위임 결과. 길마직급으로 변경이 필요하게되면 여기서 처리해야합니다. 현재는 길드정보창에만 길마직급액션이 몰려있어서 필요없음.
		GUILD_SC_CHANGEMASTER_ME,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디

		// 묘목심기
		GUILD_CS_PLANT,
		// UINT32			uiCropID;							// 묘목아이디
		// TRASH_ITEM		stConsumeItem;						// 소모되는아이템

		// 묘목심기 결과. 주변에 브로드캐스트합니다.(단, 길드방에서만 가능한 액션이고 길드방은 전체가 브로드캐스트영역으로 설정한다고 약속됨.)
		GUILD_SC_PLANT,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)
		// UINT32			uiCropID;							// 묘목아이디

		// 묘목뽑기
		GUILD_CS_GRUB,

		// 묘목뽑기 결과. 주변에 브로드캐스트합니다.(단, 길드방에서만 가능한 액션이고 길드방은 전체가 브로드캐스트영역으로 설정한다고 약속됨.)
		GUILD_SC_GRUB,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)

		// 비료/촉진제 주기
		GUILD_CS_NOURISH,
		// UINT32			uiBoosterID;						// 비료/촉진제아이디
		// TRASH_ITEM		stConsumeItem;						// 소모되는아이템

		// 비료/촉진제 주기 결과. 주변에 브로드캐스트합니다.(단, 길드방에서만 가능한 액션이고 길드방은 전체가 브로드캐스트영역으로 설정한다고 약속됨.)
		GUILD_SC_NOURISH,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)
		// UINT32			uiRemainTime;						// 현재시간과 비교하여 계산된 성장완료까지 남은시간(초단위)
		// UINT32			uiNourishTime;						// 현재시간과 비교하여 계산된 비료를 먹을 시간(초단위)
		// UINT32			uiHastenSumTime;					// 촉진제 누적시간

		// 묘목성장완료. 길드방내의 모든 PC에게 브로드캐스트합니다.
		GUILD_SC_GROWTHCROP,

		// 랭크업
		GUILD_CS_RANKUP,
		// UINT8			byNewGRank;							// New길드랭크
		// TRASH_ITEM		stConsumeItem;						// 소모되는아이템

		// 랭크업. 길드원주변에 브로드캐스팅하지 않습니다.(2007.08.13 dhpark 유명상팀장이 결정)
		GUILD_SC_RANKUP,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)
		// INT32			iGNum;								// 길드DBNum
		// UINT8			byNewGRank;							// New길드랭크

		// 길드마크변경
		GUILD_CS_CHANGEMARK,
		// UINT32			uiGMarkID;							// 길드마크LDT아이디
		// TRASH_ITEM		stConsumeItem;						// 소모되는아이템

		// 길드마크변경. 길드원주변에 브로드캐스팅하지 않습니다.(2007.08.13 dhpark 유명상팀장이 결정)
		GUILD_SC_CHANGEMARK,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디(0이면 에러없음)
		// INT32			iGNum;								// 길드DBNum
		// UINT8			arrGMark[3];						// 길드마크

	//////////////////////////////////////////////////////////////////////////
	//
	//	Indun
	//
		PI_CS_INDUN = PI_CS_LOGIC + 2900,

		// 인던 입장 요청
		INDUN_CS_IN = PI_CS_INDUN,
		// UINT32			uiIndunLDTID;						// 입장하려고하는 인던의 LDTID
			
		// 인던 입장 요청 결과
		INDUN_SC_IN,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		// UINT32			uiIndunID;							// 인던 인스턴트 아이디
		// UINT16			wServerID;							// 인던 서버아이디

		// 인던 퇴장 요청 결과. OnPortalEvent()내에서 분기하여 처리됩니다.(인던서버이면서 목표지가 인던형맵그룹타입이 아닌경우에 채널이동으로 나가는것으로 판단)
		INDUN_SC_OUT,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
			
		// 인던 초기화 요청
		INDUN_CS_INIT,
			
		// 인던 초기화 요청 결과. 소유하고 있는 인던이 없으면 uiIndunLDTID이 0입니다.
		INDUN_SC_INIT,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// UINT32			uiIndunLDTID;						// 인던 LDT 아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디

	//////////////////////////////////////////////////////////////////////////
	//
	//	Lottery
	//
		PI_CS_LOTTERY = PI_CS_LOGIC + 3000,

		//뽑기 판 만들고 시작.
		LOTTERY_SC_START = PI_CS_LOTTERY, 

		// 뽑기 업데이트 뽑기 진입시나 업데이트버튼 누를때
		LOTTERY_CS_UPDATE ,
		// UINT32			iVer;

		// 뽑기 업데이트 결과
		LOTTERY_SC_UPDATE,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		// UINT32			iVer;								// ErrorID가 0 경우만 전송
		// UINT16			iLotteryCount;						// ErrorID가 0 경우만 전송
		// UINT8			iNumOfLotteryBoard;					// ErrorID가 0 경우만 전송
		// UINT32			iLotteryBoard[iNumOfLotteryBoard];					// ErrorID가 0 경우만 전송
		// UINT8			iNumOfRewardInfo;
		// LOTTERY_REWARD	RewardInfo[iNumOfRewardInfo];

		// 뽑기
		LOTTERY_CS_CHOICE,
		// UINT32			iVer;								// 판버전
		// UINT16			iIndex;								// bit의배열 첨자
		// CONTAINER_SLOT	UsingItemPos;				// 사용할 아이템 위치정보
		// ITEMNO			iUsingItemNo;				// 사용할 아이템 키번호
		// int				iUsingEly;					// 사용할 게임 머니	

		// 

		// 뽑기 결과
		LOTTERY_SC_CHOICE,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		// UINT16			iRewardID;							// ErrorID가 0경우만 전송
		// CONTAINER_ITEM   tItem;								// ErrorID가 0경우만 전송 
		// UINT64			iPCCurrEly;							// ErrorID가 0경우만 전송
		// TRASH_ITEM		TrashItem;							// ErrorID가 0경우만 전송
		// UINT16			iLotteryCount;						// ErrorID가 0, 13001004 경우만 전송.
		// UINT8			iNumOfLotteryBoard;					// ErrorID가 0, 13001004 경우만 전송.
		// UINT32			iLotteryBoard[iNumOfLotteryBoard];	// ErrorID가 0, 13001004 경우만 전송. iLotteryCount를 포함할수 이는 배열 갯수만큼만

	//////////////////////////////////////////////////////////////////////////
	//
	//	Event Mission
	//
		PI_CS_EVENTMISSION = PI_CS_LOGIC + 3100,

		// 웨이포인트 리스트
		EVENTMISSION_SC_LIST = PI_CS_EVENTMISSION, // 블럭요망
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// int				arrMissionFlag[MAX_EVENT_MISSION_DATA];	// CP_EVENT_MISSION.ldt::_Save 마스킹값 

		// 이벤트 발생
		EVENTMISSION_CS_RAISE ,
		// int				iMissionID ;	// CP_EVENT_MISSION.ldt::_Item_ID

		// 이벤트 발생 결과. 
		// iMissionType 이 퀘스트 이고 iErrorMsg = 0 이면 블럭 해제 하지 말아주세요. 
		EVENTMISSION_SC_RAISE,
		// GU_ID			ObjectGUID;		// 게임오브젝트아이디
		// GLOBAL_STRING_ID	iErrorID;		// 에러아이디,  0 인경우만 이하 데이터송신
		// int				iMissionID;		// CP_EVENT_MISSION.ldt::_Item_ID
		// UINT8			iMissionType;	// CP_EVENT_MISSION.ldt::_MissionType
		// int				iMissionFlag;	// 저장할 이벤트미션플래그값(0이면 저장안함)
		// UINT8			iNumOfItem;		// 받은 아이템수
		// CONTAINER_ITEM   stItem[];		// 받은 아이템


	//////////////////////////////////////////////////////////////////////////
	//
	//	GAMBLE
	//
		PI_CS_GAMBLE = PI_CS_LOGIC + 3200,

		// 뽑기
		GAMBLE_CS_REQ	= PI_CS_GAMBLE,
		// int				iGambleID;					// GAMBLE.LDT::_ITEM_ID
		// UINT8			iBettingCount;				// 
		// UINT8			iNumOfUseItem;				// 소모할 아이템
		// TRASH_ITEM		TrashItem[];				// 사용할 아이템 위치정보
		// UINT8			iNumOfAutoSell;				// 자동아이템 팔기
		// int				iAutoSellRewardID[];

		// 뽑기 결과
		GAMBLE_SC_REQ,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		// int				iGambleID;							// ErrorID가 0경우만 전송
		// UINT8			iBettingCount;
		// int				iGambleRewardID[];					// ErrorID가 0경우만 전송
		// UINT8			iAutoSell[];						// 자동판매
		// UINT8			iNumOfItem;							// ErrorID가 0경우만 전송
		// CONTAINER_ITEM   tItem;								// ErrorID가 0경우만 전송 
		// UINT64			iPCCurrEly;							// ErrorID가 0경우만 전송
		// UINT8			iNumOfTrashItem;					// ErrorID가 0경우만 전송
		// TRASH_ITEM		TrashItem;							// ErrorID가 0경우만 전송


	//////////////////////////////////////////////////////////////////////////
	//
	//	PVP
	//
		PI_CS_PVP = PI_CS_LOGIC + 3400,

		// PVP 등록
		PVP_CS_REGIST = PI_CS_PVP,
		// UINT8			byPVPType;							// (1:FDEATH, 2:FTIME, 3:PDEATH, 4:PTIME, 5:GDEATH, 6:GTIME)
		// UINT32			uiPVPScale;							// 몇인용인지. (1:2인용, 2:4인용, 4:6인용, 8:8인용)
			
		// PVP 등록 결과
		PVP_SC_REGIST,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디

		// PVP 등록 취소
		PVP_CS_CANCEL,
			
		// PVP 등록 취소 결과
		PVP_SC_CANCEL,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
			
		// PVP 입장 결과(서버에서 매칭이 되면 보내줌)
		PVP_SC_IN,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		// UINT32			uiIndunID;							// PVP방 인스턴트 아이디
		// UINT16			wServerID;							// 인던 서버아이디
			
		// 입장시 대기자들의 로딩상태를 알려줌(1명 로딩완료시마다 모든 대기자에게 송신)
		PVP_SC_LOADSTATUS,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// INT32			iMemberCnt;							// 대기자수
		// PVPLOADSTATUS	stPVPLoadStatus;					// 대기자의 PVP로드상태
		// ...
			
		// 입장시 대기자들에게 PVP가 시작되었다고 알려줌
		PVP_SC_START,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// UINT32			uiLDTID;							// PVP LDT 아이디
			
		// PVP 퇴장요청(PVP인던에는 퇴장용 포탈이 없어서 종료후 나갈때 사용함)
		PVP_CS_OUT,
			
		// PVP 퇴장요청 결과. OnPortalEvent()내에서 분기하여 처리됩니다.(인던서버이면서 목표지가 인던형맵그룹타입이 아닌경우에 채널이동으로 나가는것으로 판단)
		// 클라이언트가 PVP종료후 나갈때 PVP_CS_OUT을 보내고 서버에서는 위치저장만하고 보내기도 함.
		PVP_SC_OUT,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
			
		// PVP성적을 송신합니다.
		PVP_SC_RANK,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// INT32			iMemberCnt;							// 대기자수
		// PVPRANK			stPVPRank;							// 대기자의 PVP순위정보
		// ...
			
		// PVP종료를 송신합니다.
		PVP_SC_FINISH,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// INT32			iMemberCnt;							// 대기자수
		// PVPRANK			stPVPRank;							// 대기자의 PVP순위정보
		// ...


	//////////////////////////////////////////////////////////////////////////
	//
	//	SERVERINFO : 클라이언트의 요청에 의해 서버정보를 송신합니다.
	//
		PI_CS_SERVERINFO = PI_CS_LOGIC + 3500,

		// 서버시간/정보 요청
		SERVERTIME_CS_INFO = PI_CS_SERVERINFO,

		// 서버시간/정보 송신
		SERVERTIME_SC_INFO,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// tm				stServerTime;				// 서버시간
		// WORLD_ID			wWorldID;					// 월드아이디
		// SERVER_ID		wServerID;					// 서버아이디
		// char				szAccountID[MAX_LEN_ID+1];	// 계정아이디
		// char				szClientIP[LEN_IP+1];		// 클라이언트아이피
		// TCHAR			szName[LEN_NAME+1];			// 캐릭터명
		// UINT				uiStageID;					// 스테이지아이디
		// UINT				uiMapGroupID;				// 맵그룹아이디
		// UINT				uiPosX;						// X좌표
		// UINT				uiPosY;						// Y좌표


	//////////////////////////////////////////////////////////////////////////
	//
	//	Chat Room												// 2008/03/07 by Hoony0109
	//
		PI_CS_CHATROOM = PI_CS_LOGIC + 3600,

		// 대화방 생성 요청
		CHATROOM_CS_CREATE = PI_CS_CHATROOM, // (캐시템 소모로 방제목스킨과 오브젝트의 변경 기능 확장성 고려.)
		// CHATROOM_INFO	stChatRoomInfo;						// 생성하려고하는 대화방의 정보
		// UINT8			iAdvert;							// 광고 여부 (0 , 1)
		// if(iAdvert == 1)
			// TRASH_ITEM		stConsumeItem;						// 사용할 아이템
			// UINT8			iChatRoomAdvertLen;					// 광고 메세지 길이
			// TCHAR			szAdvert[iChatRoomAdvertLen];		// 광고 메세지 (최대: MAX_CHATROOM_ADVERT_LEN)

		// 대화방 생성 결과.	+ INDUN_SC_IN 전송 (광고 사용시 + ITEM_SC_TRASH 전송)
		CHATROOM_SC_CREATE,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// UINT32			uiMyCharID;
		// UINT8			iAdvert;							// 광고 여부 (0, 1)
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		// if(ErrorID == 0)
			// CHATROOM_INFO	stCRInfo;							// 생성하려고하는 대화방의 정보

		// 대화방 오브젝트 생성.
		// CHATROOM_SC_MAKE_OBJECT, => NAV_SC_INSIGHT 로 변경
			// GU_ID					ObjectGUID;					// 게임오브젝트아이디
			// BG_ID					Layer;
			// UINT						iPosX;						//
			// UINT						iPosY;						//
			// CHATROOM_OBJECT_INFO		stChatRoomObject_Info		//

		// 대화방 오브젝트 파괴.
		// CHATROOM_SC_DESTROY_OBJECT, => NAV_SC_OUTOFSIGHT 로 변경.
			// GU_ID						ObjectGUID;					// 게임오브젝트아이디

		// 대화방 참여 전 방 정보 확인.
		CHATROOM_CS_ROOMINFO_CHECK,
		// UINT32			uiRoomNo;

		// 요청한 방정보 결과.
		CHATROOM_SC_ROOMINFO_CHECK,
		// GU_ID			globalID;							// 사용자 아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		// if(0 == _ErrorID)
			// CHATROOM_INFO	stChatRoomInfo;						// 생성하려고하는 대화방의 정보

		// 대화방 참여 요청. (이미 ActPC 가 2번 강퇴된 방인지 체크.) (인던, 대화방에서 또 이동하는 경우 처리.)
		CHATROOM_CS_JOIN,
		// CHATROOM_INFO	stChatRoomInfo;						// 입장하려고하는 대화방의 정보

		// 대화방 참여 결과.	+ INDUN_SC_IN 전송
		CHATROOM_SC_JOIN,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// UINT32			uiMyCharID;
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		// if(ErrorID == 0)
			// CHATROOM_INFO	stCRInfo;							// 입장하려고하는 대화방의 정보

		// 대화방 재수정 요청.
		CHATROOM_CS_MODIFY,
		// UINT8			iPassWord;							// 0: 패스워드 미변경, 1: 패스워드 변경 포함시.
		// CHATROOM_INFO	stCRInfo;							// 수정하려고하는 대화방의 정보

		// 대화방 재수정 결과. (강퇴자 목록 전부 삭제. - 패스워드만 변경시는 제외.)
		CHATROOM_SC_MODIFY,
		// GU_ID				globalID;						// 사용자 아이디
		// GLOBAL_STRING_ID		iError;							// 0 : 성공
		// CHATROOM_INFO		stCRInfo;						// 수정하려고하는 대화방의 정보

		// 대화방 퇴장 요청.
		CHATROOM_CS_OUT,
		// UINT32			uiID;								// 대화방 인스턴스 아이디 (Unique Key)
		// UINT32			uiRoomNo;							// 퇴장하려고 하는 대화방의 번호.
		// UINT8			iBanOut;							// 0: 일반퇴장 1: 강제퇴장

		// 대화방 퇴장 결과.	+ INDUN_SC_OUT 전송
		CHATROOM_SC_OUT,	
		// GU_ID			globalID;							// 사용자 아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디

		// 방장의 강제 퇴장 요청. (최대 2번의 강퇴 횟수 및 방번호 저장.) 방정보에 참여자 목록과 강퇴자 목록 및 횟수 저장.
		CHATROOM_CS_FORCEOUT,
		// UINT32			uiID;								// 대화방 인스턴스 아이디 (Unique Key)
		// UINT32			uiRoomNo;							// 퇴장시키려고 하는 대화방의 번호.
		// UINT32			uiCharID;							// 강제 퇴장 대상.

		// 방장의 강제 퇴장 요청 실패. (성공시 CHATROOM_SC_BROADCAST_UNITED_MESSAGE 메세지 송신.)
		CHATROOM_SC_FORCEOUT,
		// GU_ID				globalID;						// 사용자 아이디
		// GLOBAL_STRING_ID		iError;							// 0 : 성공
		// if(0 == iError)
			// UINT8				iFlag;						// 0: 방장의 강퇴 요청의 응답. 1:강제 퇴장 대상자.

		// 방장 양도 요청.
		CHATROOM_CS_CHANGEMASTER,
		// UINT32			uiTargetCharID;
		// UINT32			uiRoomID;
		// UINT32			uiRoomNo;

		// 방장 양도 요청 결과.
		CHATROOM_SC_CHANGEMASTER,
		// GU_ID			globalID;							// 사용자 아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디

		CHATROOM_SC_BROADCAST_UNITED_MESSAGE, // (0:Join, 1:Out, 2:Forceout, 3:ChangeMaster)
		// UINT8			iFlag;
		// ROOM_MEMBER		stRoomMember;

		// 대화방 참여자 목록 전송.
		CHATROOM_SC_MEMBERLIST,
		// UINT32			uiSize;
		// ROOM_MEMBER		stRoomMember[uiSize];

		// 대화방 광고 요청. (방 생성과 입장 이후 이후 ...) -> SCOPE_CHATROOM_ADVERT 추가하는 방식으로...
		CHATROOM_CS_ADVERT,
		// UINT32			uiRoomID;
		// UINT32			uiRoomNo;
		// TRASH_ITEM		stConsumeItem;								// 사용할 아이템
		// UINT16			iAdvertSize;
		// TCHAR			szAdvertMessage[iAdvertSize]				// iAdvertSize <= MAX_CHATROOM_ADVERT_LEN

		// 대화방 광고 결과. (아이템 소모 => ITEM_SC_TRASH)
		CHATROOM_SC_ADVERT,
		// GU_ID			globalID;							// 사용자 아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디

		// 대화방 목록 요청. (요청시 초기화)
		CHATROOM_CS_ROOMLIST,
		// UINT8			iRoomType;

		// 대화방 목록 결과. (복수개의 패킷으로 나누어 전송.)
		CHATROOM_SC_ROOMLIST,
		// GU_ID			globalID;						// 사용자 아이디
		// GLOBAL_STRING_ID ErrorID;
		// if(ErrorID == 0)
			// UINT8			iResetFlag;						// 1 = 리스트 초기화, 0 = 리스트 전송중, 2 = 리스트 마지막, 3 = 리스트 시작이자 마지막(방의 총 갯수가 1개의 패킷에 다 담겨짐을 의미함)
			// UINT8			iRoomType;
			// UINT32			uiCount;
			// CHATROOM_INFO	stChatRoomInfo[uiCount];

		// 대화 상대 목록 요청.
		CHATROOM_CS_USERLIST,
		// UINT8			iSex;
		// UINT16			iMinLevel;
		// UINT16			iMaxLevel;
		// PCSTATUS_STYLE	stStyle;

		// 대화 상대 목록 결과.
		CHATROOM_SC_USERLIST,
		// GU_ID				globalID;						// 사용자 아이디
		// GLOBAL_STRING_ID		ErrorID;
		// if(ErrorID == 0)
			// UINT32				uiCount;
			// CHARACTER_BASICINFO	stCharInfo[uiCount];

		// 초대의 정상 Flow (A-S-B)
		// CHATROOM_CS_INVITE_REQ (A -> S) - CHATROOM_SC_INVITE_REQ (S -> B) - CHATROOM_CS_INVITE_ANS (B -> S) - CHATROOM_SC_INVITE_ANS (S -> A)
		// 초대 자체의 실패의 경우
		// CHATROOM_CS_INVITE_REQ (A -> S) - CHATROOM_SC_INVITE_REQ (S -> A)
		// 초대 후 초대자에 의한 실패의 경우
		// CHATROOM_CS_INVITE_REQ (A -> S) - CHATROOM_SC_INVITE_REQ (S -> B) - CHATROOM_CS_INVITE_ANS (B -> S) - CHATROOM_SC_INVITE_ANS (S -> B)

		// 대화 상대 초대 요청 (신청자). // 1:1 대화 초대 포함 (Client A -> Sv)
		CHATROOM_CS_INVITE_REQ,
		// TCHAR			szRcvCharName[LEN_NAME+1];				// 대상자이름
		// UINT8			uiInviteInRoom;							// 초대자 상태.(이미 생성된 방에서 초대하는 것인지(1), 1:1대화 초대인지(0))
		// CHATROOM_INFO	stChatRoomInfo;

		// 대화 상대 초대 (신청 대상). // 1:1 대화 초대 포함 (Sv -> Client B)
		CHATROOM_SC_INVITE_REQ,
		// GU_ID				globalID;						// 사용자 아이디
		// TCHAR				szTargetName[LEN_NAME+1];
		// GLOBAL_STRING_ID		ErrorID;
		// if(0 == ErrorID)
			// TCHAR				szActorName[LEN_NAME+1];
			// UINT8				uiInviteInRoom;
			// CHATROOM_INFO		stChatRoomInfo;

		// 대화 상대 초대 응답 (신청 대상). // 1:1 대화 초대 포함 (Client B -> Sv)
		CHATROOM_CS_INVITE_ANS,
		// UINT8				iAllowed;						// 0: 초대 거부, 1: 초대 수락.
		// TCHAR				szActorName[LEN_NAME+1];
		// UINT8				uiInviteInRoom;
		// CHATROOM_INFO		stChatRoomInfo;

		// 대화 상대 초대 결과 (신청자). // 1:1 대화 초대 포함 (Sv -> Client A)
		CHATROOM_SC_INVITE_ANS,
		// GU_ID				globalID;						// 사용자 아이디
		// TCHAR				szTargetName[LEN_NAME+1];
		// GLOBAL_STRING_ID		ErrorID;

		// 대화상대 특정 PC 정보 요청.
		CHATROOM_CS_OTHERPC_INFO,
		// TCHAR			szCharName[LEN_NAME+1];

		// 대화상대 내 특정 PC 정보 결과.
		CHATROOM_SC_OTHERPC_INFO,
		// GU_ID			globalID;							// 사용자 아이디
		// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
		// UINT8			iFlag;								// 0: 정상, 1: 자신이 스타일 비공개, 2: 대상이 스타일 비공개
		// if(0 == ErrorID)
			// CHARACTER_INFO2	stCharInfo2;
			// UINT16			iTotalSize;
			// FIGURE_ITEM		stFigureItemList[iTotalSize];
			// PCSTATUS_STYLE	stStyle;

		// 스타일 설정 저장 요청. (별도 Tabel에 저장)
		CHATROOM_CS_SET_MYSTYLE,
		// PCSTATUS_STYLE	stStyle;

		// 스타일 정보 결과.
		CHATROOM_SC_GET_MYSTYLE,
		// GU_ID			ObjectGUID;							// 게임오브젝트아이디
		// PCSTATUS_STYLE	stStyle;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Auction												// 2008/03/07 by Hoony0109
	//
		PI_CS_AUCTION = PI_CS_LOGIC + 3700,		

		// 경매장오픈( 게임진행중 경매장이 뜨게 된경우)
		AUCTION_SC_OPEN,
		// GLOBAL_STRING_ID	iErrorID;							// 에러아이디

					
		// 물건 등록
		AUCTION_CS_CREATE,
		// UINT8	iType;				//거래타입(AUCTION_TYPE)
		// UINT8	iDuration;			//기간타입(AUCTION_DURATION )
		// __int64	iStartingPrice;		//시작가
		// __int64	iBuyOutPrice;		//즉구가
		// __int64	iEnrolPrice;		//등록수수료
		// TRASH_ITEM stUseItem;		//필요아이템위치및 갯수(1)
		// ITEM_NO	iUseItemNo;			//필요아이템키
		//-------------------------------------------------
		//	iType == AUCTION_TYPE_ITEM_BY_ELY
		//		CONTAINER_SLOT		stUpItemPos;
		//		ITEM_NO				iUpItemNo;
		//	else
		//		AUCTION_GOODS_POINT stUpPoint;

		// 물건등록 결과
		// 판매리스트 한번도 받지 않은경우는 무시
		AUCTION_SC_CREATE,
		// GU_ID			iObjectGUID;						// 게임오브젝트아이디
		// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
		// iErrorID == 0
			// AUCTION_HEADER	stAuctionHeader;					// 기본정보
			// AUCTION_GOODS_XX stAuctionGoods;						// 올린 물건 정보( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)
			// __int64			iCurrEly;							// 현재남은돈 (등록수수료 or 올리 돈을 제하고 현재 가지고있는돈)
			// TRASH_ITEM		stUseItem;							// 필요아이템
			//-------------------------------------------------
			//	stAuctionHeader.iType == AUCTION_TYPE_ITEM_BY_ELY
			//		TRASH_ITEM			stUpItem;					// 올린Z아이템삭제정보
			//	else
			//		CHARACTER_CASH_INFO stCash;						// 현재캐쉬정보
		// iErrorID != 0
			// CHARACTER_CASH_INFO stCash;	


		// 취소 요청
		AUCTION_CS_CANCEL,
		//__int64	iAuctionID;									// 경매키
		// int		iCategory;									// Item.ldt::AuctionCategory or 8000 이상
	
		// 취소 요청 결과(등록자)
   		AUCTION_SC_CANCEL,
		// GU_ID			iObjectGUID;						// 게임오브젝트아이디
		// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
		// iErrorID == 0
			// __int64			iAuctionID;							// 경매키
			// CHARACTER_CASH_INFO stCash;							// 
		// iErrorID != 0
			// CHARACTER_CASH_INFO stCash;							// 


		// 물건종료
		//		- 유찰시 판매자/입찰자에게 
		//		- 취소시 입찰자에게
		AUCTION_SC_FINISH,
		// GU_ID			iObjectGUID;						// 게임오브젝트아이디
		// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
		// iErrorID == 0
			// __int64			iAuctionID;							// 경매키
			// CHARACTER_CASH_INFO stCash;							// 
		// iErrorID != 0
			// CHARACTER_CASH_INFO stCash;							// 

		// 낙찰완료
		//		- 낙찰시 판매자/낙찰자에게
		//		- 즉구시 판매자에게
		AUCTION_SC_FINISH_SELL,
		// GU_ID			iObjectID;
		// __int64			iAuctionID;							// 경매키
		// USERID			iBuyerID;							// 구매자
		// CHARACTER_CASH_INFO stCash;							// 

		// 입찰하기요청
		AUCTION_CS_BID,
		// __int64			iAuctionID;							// 경매키
		// int				iCategory;							// Item.ldt::AuctionCategory or 8000 이상
		// __int64			iBidPrice;							// 입찰가

		// 입찰정보 : 판매자 입찰자에게 갱신
		// 판매리스트/입찰리스트를 한번도 받지 않은경우는 무시
		AUCTION_SC_BID,
		// GU_ID			iObjectGUID;						// 게임오브젝트아이디
		// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
		// iErrorID == 0
			// AUCTION_HEADER	stAuctionHeader;					// 기본정보
			// AUCTION_GOODS_XX stAuctionGoods;						// 올린 물건 정보( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)
			// __int64			iCurrEly;							// 현재남은돈 (등록수수료 or 올리 돈을 제하고 현재 가지고있는돈)
			// CHARACTER_CASH_INFO stCash;							// 현재캐쉬정보							
		// iErrorID != 0
			// CHARACTER_CASH_INFO stCash;							// 


		// 즉구요청
		AUCTION_CS_BUYOUT,
		// __int64			iAuctionID;							// 경매키
		// int				iCategory;							// Item.ldt::AuctionCategory or 8000 이상
		// __int64			iBidPrice;							// 즉구가
		// UINT8			iRefresh;							// 1: 갱신 0: 갱신안함
		// int				iStartNumber;						// 몇번째것부터 더 받고 싶은지
		// int				iCategory;							// 
		// UINT8			iSortKey;							// AUCTION_SORT_ITEMKEY(유저가 지정하지 않은경우이면 기획서에 정한값)
		// UINT8			IsAsc;								// 1: 오름차순 0: 내림차순
		// iCategory < 8000
			// TCHAR			szItemName[MAX_ITEM_NAME];		// 아이템명
			// short			iItemLvMin;							// 아이템레벨이상: 기본 1
			// short			iItemLvMax;							// 아이템레벨이하: 기본 999 (검색가능 범위가 체크시 본인레벨까지)
			// UINT8			iItemQuility;						// 아이템품질
		// iCategory >= 8000
			// int				iMinPoint;							// 전체검색일경우 iMinPoint iMaxPoint 0
			// int				iMaxPoint;

		// 즉구요청결과
		AUCTION_SC_BUYOUT,
		// GU_ID			iObjectGUID;						// 게임오브젝트아이디
		// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
		// iErrorID == 0
			// __int64			iAuctionID;							// 경매키
			// __int64			iCurrMoney;							// 경매키
			// CHARACTER_CASH_INFO stCash;							// 
		// iErrorID != 0
			// CHARACTER_CASH_INFO stCash;							// 



		//-----------------------------------------------------------------
		// 물건검색(
		AUCTION_CS_SEARCH,
		// int				iCategory;							// 
		// UINT8			iSortKey;							// AUCTION_SORT_ITEMKEY(유저가 지정하지 않은경우이면 기획서에 정한값)
		// UINT8			IsAsc;								// 1: 오름차순 0: 내림차순
		// iCategory < 8000
			// TCHAR			szItemName[MAX_ITEM_NAME];		// 아이템명 : 전체(NULL)
			// short			iItemLvMin;							// 아이템레벨이상: 기본 1
			// short			iItemLvMax;							// 아이템레벨이하: 기본 999 (검색가능 범위가 체크시 본인레벨까지)
			// UINT8			iItemQuility;						// 아이템품질 : 전체품질(0)
		// iCategory >= 8000
			// int				iMinPoint;							// 전체검색일경우 iMinPoint iMaxPoint 0
			// int				iMaxPoint;


		// 물건검색이이미 이루어진 상태에서 소트만 해서 내려갈경우
		// Item Auction/ 아비오베이 공통
		AUCTION_CS_SORTING, // 무조건 첫페이지로 간다
		// int				iCategory;							// 
		// UINT8			iSortKey;							// AUCTION_SORT_ITEMKEY(유저가 지정하지 않은경우이면 기획서에 정한값)
		// UINT8			IsAsc;								// 1: 오름차순 0: 내림차순
		// iCategory < 8000
			// TCHAR			szItemName[MAX_ITEM_NAME];		// 아이템명
			// short			iItemLvMin;							// 아이템레벨이상: 기본 1
			// short			iItemLvMax;							// 아이템레벨이하: 기본 999 (검색가능 범위가 체크시 본인레벨까지)
			// UINT8			iItemQuility;						// 아이템품질								// 1: 오름차순 0: 내림차순
		// iCategory >= 8000
			// int				iMinPoint;							// 전체검색일경우 iMinPoint iMaxPoint 0
			// int				iMaxPoint;

		// 검색과소트가 이루어진상태에서 갯수를 더 받아내고 싶을때
		AUCTION_CS_GETNEXT,
		// int				iStartNumber;						// 몇번째것부터 더 받고 싶은지
		// int				iCategory;							// 
		// UINT8			iSortKey;							// AUCTION_SORT_ITEMKEY(유저가 지정하지 않은경우이면 기획서에 정한값)
		// UINT8			IsAsc;								// 1: 오름차순 0: 내림차순
		// iCategory < 8000
			// TCHAR			szItemName[MAX_ITEM_NAME];		// 아이템명
			// short			iItemLvMin;							// 아이템레벨이상: 기본 1
			// short			iItemLvMax;							// 아이템레벨이하: 기본 999 (검색가능 범위가 체크시 본인레벨까지)
			// UINT8			iItemQuility;						// 아이템품질		
		// iCategory >= 8000
			// int				iMinPoint;							// 전체검색일경우 iMinPoint iMaxPoint 0
			// int				iMaxPoint;

		// 물건검색/소트/더받아오기 결과
		AUCTION_SC_SEARCH_INFO,
		// GU_ID			iObjectGUID;						// 게임오브젝트아이디
		// GLOBAL_STRING_ID iErrorMsg ;							// iErrorMsg != 0 면 에러 에러면 이후 무시
		// int				iNumOfTotalCount;					// 총갯수
		// int				iStartNumber;						// 현재전송처음값이 총갯수중 몇번째값에 해당하는지
		// UINT8			iNumOfAuction;						// 전송될옥션정보갯수
			// AUCTION_HEADER	stAuctionHeader;					// 기본정보
			// AUCTION_GOODS_XX stAuctionGoods;						// 올린 물건 정보( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)


		//--------------------------------------------------------------------

		// 자기입찰정보 얻어오기 
		//		- 게임진입이후 최초 입찰탭을 눌렀을때 경우이거나 
		//		- AUCTION_SC_OPEN 이후 처음 입찰탭을 눌렀을때 발생.
		AUCTION_CS_INFO_4BIDDER,			

		AUCTION_SC_INFO_4BIDDER,
		// GU_ID			iObjectGUID;						// 게임오브젝트아이디
		// GLOBAL_STRING_ID	iErrorID;							// 에러아이디

		AUCTION_SC_BIDLIST,
		// GU_ID			iObjectGUID;						// 게임오브젝트아이디
		// UINT8			iNumOfAuction;						// 전송될옥션정보갯수
			// AUCTION_HEADER	stAuctionHeader;					// 기본정보
			// AUCTION_GOODS_XX stAuctionGoods;						// 올린 물건 정보( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)

		//	상회입찰시 이전입찰자에게 입찰정보삭제
		AUCTION_SC_BIDDEL,
		// GU_ID			iObjectGUID;						// 게임오브젝트아이디
		// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
		// __int64			iAuctionID;							// 경매키

		// 자기 경매정보 얻어오기
		//		- 게임진입이후 최초 입찰탭을 눌렀을때발생 경우이거나
		//		- AUCTION_SC_OPEN 이후 처음 입찰탭을 눌렀을때 발생.
		AUCTION_CS_INFO_4SELLER,

		// 자기 경매정보
		AUCTION_SC_INFO_4SELLER,
		// GU_ID			iObjectGUID;						// 게임오브젝트아이디
		// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
	
		AUCTION_SC_SELLLIST,
		// GU_ID			iObjectGUID;						// 게임오브젝트아이디
		// UINT8			iNumOfAuction;						// 전송될옥션정보갯수
			// AUCTION_HEADER	stAuctionHeader;					// 기본정보
			// AUCTION_GOODS_XX stAuctionGoods;						// 올린 물건 정보( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)

		//--------------------------------------------------------------------
		// Greener Guide
		
		GREENERGUIDE_SC_EVENT,
		// UINT32	iStartLevel

		GREENERGUIDE_CS_FETCHAWARD,
		// UINT32	iFetchLevel

		GREENERGUIDE_SC_FETECHAWARD,
		// UINT32	iFetchLevel
		// GLOBAL_STRING_ID	bResult
		// UINT32	iNotifyCode	(0: 퀭돨�薨뗌週節トゲ옆▦孤식骸켤幹瀋�헝혼몹냘코錟뒵逃뇹쥴혤。
		//						(1: 퀭돨관범綠찮，�近늑餌÷週節シ↗좔骸켤幹瀋�헝혼몹냘코錟뒵逃뇹쥴혤。
		//						(2: 냥묘쥴혤


	//////////////////////////////////////////////////////////////////////////
	//
	//	COOLTIME
	//
		PI_CS_COOLTIME = PI_CS_LOGIC + 3800,

		COOLTIME_SC_INFOS = PI_CS_COOLTIME,
		// GU_ID			iInstanceID
		// unsigned short	iNumOfCoolTime
		// COOLTIME_DATA	stCoolTime[]


	//////////////////////////////////////////////////////////////////////////
	//
	//	LOTTERY
	//

		//	겟괜芎돨葵累관
		//	鯤소뭔찜겟괜芎   2008－9－12 侊價츰
		PI_GAMBLE_LOGIC		= PI_CS_LOGIC + 3900,

		GAMBLE_SC_INITIAL	= PI_GAMBLE_LOGIC,
		//  UINT64 gambleVersion;
		//	UNSIGNED SHORT      usServerPoit;		//	角뤠角륩蛟포쌘듐뭔찜

		GAMBLE_CS_GET_STATUS,	
		//	
		GAMBLE_SC_GET_STATUS,
		//	GLOBAL_STRING_ID	ErrID;
		//	unsigned short		usFinished;    // 角뤠랙箇供귄
		//	UINT64				GambleVersion;
		//	UNSIGNED SHORT      usServerPoit;		//	角뤠角륩蛟포쌘듐뭔찜
		//	UINT				count;
		//  t_Gamble_Status		status[];
		
		GAMBLE_SC_UPDATE,					//	뫘劤겟괜芎뚤壇돨榴檄斤口
		//	UINT64 gambleVersion;			//	겟괜芎돨경굶뵀
		//	t_Gamble_Status status;			//	겟괜芎돨榴檄	
		//	char	szName[LEN_NAME+1];		//	鯤소츰냔
			
		GAMBLE_CS_BUY,
		//	UINT64  gambleVersion;		//	경굶뵀
		//	UINT32	uIndex;				//	貫零
		//	CONTAINER_SLOT ContainerSlot;	//	膠틔貫零,ContainerSlot.ContainerType槨CONTAINER_TYPE_INVALID깊刻痰듐얩뭔찜
		//	ITEMNO	ItemNO;				//	膠틔돨긍뵀

		GAMBLE_SC_BUY,
		//	GLOBAL_STRING_ID	iErrorID;			//	0槨청唐댄轎，1槨경굶놔댄，와빵똥矜狼뫘劤겟괜芎,2槨겟괜芎董珂轟랬賈痰
													//	3槨듐얩꼇璃，4槨膠틔댄轎，5槨뭔찜돨겟괜芎貫零꼇뚤，6槨綠쒔굳儉훙뭔찜
													//	7槨鯤소攣瞳�求菅봔紙켰늬ВЪ늴늰怨��求菅봔�
		//	CHARID				charID;				//	鯤소돨ID뵀
		//	UINT64				gambleVersion;		//	경굶뵀
		//	t_Gamble_Getted_Item	itemID;				//	청唐놔댄돨헙워苟，뭔찜돕돨膠틔
		//	UNSIGNED SHORT      usServerPoit;		//	角뤠角륩蛟포쌘듐뭔찜
		//	UINT8				uCount;				//	뒤랗늴膠틔돨悧鑒
		//	t_Gamble_Getted_Item	itemID[];			//	뒤랗늴돨膠틔	

		GAMBLE_SC_UN_BUY,				//	鯤소혤句뭔찜캐몸겟괜芎
		//	GLOBAL_STRING_ID	iErrorID;			//	errorID == 0 깊刻청唐랙�駭鵲徵е뽈핥謹轢蘆≠薑봔�
		//	UINT64				gambleVersion;		//	경굶뵀
		//	UINT				uIndex;				//	겟괜芎돨캐몸貫零

		GAMBLE_CS_ACCEPT_ITEM,
		//	UINT64  gambleVersion;		//	경굶뵀
		//	UINT8	accept;				//	角뤠쌈肝뒤寧늴돨�鉗�	0槨쌈肝，1槨꼇쌈肝
		//	t_Gamble_Getted_Item  itemID;				//	뒤寧늴�鉗럿횵D
		//	CONTAINER_SLOT ContainerSlot;	//	膠틔貫零,ContainerSlot.ContainerType槨CONTAINER_TYPE_INVALID깊刻痰듐얩뭔찜
		//	ITEMNO	ItemNO;				//	膠틔돨긍뵀
		//	UNSIGNED SHORT      usServerPoit;		//	角뤠角륩蛟포쌘듐뭔찜


		GAMBLE_SC_ACCEPT_ITEM,
		//	GLOBAL_STRING_ID	iErrorID;	//	댄轎斤口
		//	UINT8				accept;		//	角뤠쌈肝뒤寧늴돨�鉗�	1槨뒤寧늴，2槨뒤랗늴膠틔
		//	t_Gamble_Getted_Item	itemID;		//	膠틔돨斤口

		GAMBLE_CS_ACCEPT_SECOND_ITEM,		//	쌈肝뒤랗늴돨膠틔
		//	ITEMID  itemIDGetted;			//	뒤랗늴쌈肝돨膠틔

		GAMBLE_SC_ACCEPT_SECOND_ITEM,
		//	GLOBAL_STRING_ID		iErrorID;	//	댄轎斤口
		//	t_Gamble_Getted_Item	itemID;		//	膠틔돨斤口

		GAMBLE_CS_ENTER_GAMBLE,				//	쏵흙겟괜芎뭔찜榴檄
		GAMBLE_SC_ENTER_GAMBLE,				//	륩蛟포럿쀼쏵흙뭔찜榴檄角뤠냥묘
		//	GLOBAL_STRING_ID	iErrorID;
		GAMBLE_CS_LEAVER_GAMBLE,			//	와빵똥잼역뭔찜榴檄	
		GAMBLE_SC_LEAVER_GAMBLE,			//	잼역겟괜芎
		//	GLOBAL_STRING_ID	iErrorID;
		GAMBLE_SC_BROADCAST_GETTED_ITEM,	//	밤꺄캐몸鯤소삿돤돨膠틔
		//	char   szName[LEN_NAME + 1];	//	삿돤膠틔돨鯤소돨츰俚
		//	t_Gamble_Getted_Item itemID;					//	삿돤돨膠틔돨ID

		GAMBLE_CS_GET_NAME,				//	삿혤겟괜芎櫓삿돤쉽틔돨鯤소돨츰俚
		//	UINT64  gambleVersion;		//	경굶뵀
		//	UINT32	uIndex;				//	貫零
		GAMBLE_SC_GET_NAME,
		//	UINT32	uIndex;					//	貫零
		//	char    szName[LEN_NAME + 1];	//	츰俚


	//////////////////////////////////////////////////////////////////////////
	//
	//	NEW PET	
	//
		//	劤녘膠葵累관  2008－11－5
		PI_PET_LOGIC	= PI_CS_LOGIC + 4000,

		PET_SC_GET_INFO = PI_PET_LOGIC,
		//	UINT8 slotIndex;			//	녘膠瞳녘膠陋구으櫓돨貫零
		//	ITEMNO itemNo;				//	녘膠돨顆寧긍뵀
		//	t_PetAttr_Info Info;		//	녘膠돨샘굶橄昑
		//	int	   nSkillCount;			//	녘膠세콘悧鑒
		//	t_Pet_Skill_To_Client petSkill[nSkillCount];	//	녘膠세콘
		
		PET_CS_OPEN_PET_SHOP,
		//	UINT8	slotIndex;				//	겠覡녘膠瞳陋구으돨貫零
		//	ITEMNO	itemNo;					//	겠覡녘膠돨膠틔긍뵀
		//	char	szShopName[LEN_NAME];	//	�絹援컬惠�
		//	UINT8	uShopStyle;				//	�絹援켰걺誇Т煇렀�??????
		//	UINT8	uItemCount;				//	�鉗럿켁卉�
		//	FM_SELLITEM items[];			//	膠틔돨貫零

		PET_SC_OPEN_PET_SHOP,
		//	GLOBAL_STRING_ID	iErrorID;		//	겠覡角뤠냥묘

		PET_CS_GET_PET_SHOP_INFO,
		//	GU_ID shopGuid;					//	�絹援켑ⓕ뺑胃씌�
		PET_SC_GET_PET_SHOP_INFO,
		//	GLOBAL_STRING_ID	iErrorID;	
		//	char szShopName[LEN_NAME];		//	�絹願胡�
		//	UINT32 itemCount;				//	�鉗럿켁卉�
		//	FM_VIEWITEM items[];			//	�鉗럿켕턴�

		PET_CS_ENTER_PET_SHOP,		//	鯤소쏵흙녘膠�絹�  ，槨죄밤꺄뭔찜句口못瞳侶몸�絹援켐轢�
		//	GU_ID shopguid;
		PET_SC_ENTER_PET_SHOP,
		//	GLOBAL_STRING_ID	iErrorID;		
		PET_CS_LEAVE_PET_SHOP,
		//	GU_ID shopguid;
		PET_SC_LEAVE_PET_SHOP,
		//	GLOBAL_STRING_ID	iErrorID;		
		PET_SC_UPDATE_PET_SHOP_ITEM,
		//	GU_ID shopguid;					// �犬풩횵D
		//	UINT8 uSlotIndex;				// 맡貫零돨膠틔綠찡놔	

		PET_CS_BUY_ITEM,					//	찜膠틔
		//	GU_ID	shopID;					//	�絹嫄僊�
		//	UINT8	shopSlotIndex;			//	뭔찜돨貫零
		//	FM_VIEWITEM viewItem;			//	뭔찜�鉗럿켕턴∀в쳄뉼人右�

		PET_SC_BUY_ITEM,					
		//	GLOBAL_STRING_ID	iErrorID;	//	角뤠뭔찜냥묘	

		PET_CS_CLOSE_PET_SHOP,
		//	USERID  uID;		
		//	CHARID  charID;		
		//	ITEMNO  itemNo;			//	녘膠돨膠틔긍뵀

		PET_SC_CLOSE_PET_SHOP,
		//	GLOBAL_STRING_ID	iErrorID;
		//	ITEMNO  itemNo;			//	녘膠돨膠틔긍뵀

		PET_CS_LEARN_SKILL,		//	欺構세콘
		//	CONTAINER_SLOT itemSlot;	//	녘膠세콘蝎瞳꾑욋櫓돨貫零
		//	ITEMNO itemNo;
		//	int nSkillID;
		//	int nBaseLevel;				//	세콘된섬

		PET_SC_LEARN_SKILL,
		//	GLOBAL_STRING_ID			//	iErrorID;		
		//	ITEMNO	itemNo;				//	컴몸녘膠欺돨
		//	int		nSkillID;			//	欺構돨세콘
		//	int		nBaseLevel;			//	欺構세콘돨된섬
		
		PET_CS_DISCARD_SKILL,
		//	ITEMNO	itemNo;
		//	int		nSkillID;			//	혼딜세콘돨ID
		//	int		nBaseLevel;			//	혼딜세콘돨된섬

		PET_SC_DISCARD_SKILL,
		//	GLOBAL_STRING_ID	iErrorID;	//	角뤠냥묘
		//	ITEMNO itemNo;		//	녘膠돨긍뵀
		//	int nSkillID;		//	녘膠瑙戈돨세콘돨ID
		//	int		nBaseLevel;			//	혼딜세콘돨된섬

		PET_SC_PET_ATTR_CHANGE,			//	녘膠돨橄昑맣긴，矜狼밤꺄못맒쐤돨鯤소
		//	CHARID  charID;				//	鯤소돨ID
		//	UINT8 slotIndex;			//	녘膠瞳녘膠陋구으櫓돨貫零
		//	ITEMNO itemNo;				//	녘膠돨顆寧긍뵀
		//	t_PetAttr_Info Info;		//	녘膠돨샘굶橄昑

		PET_SC_PET_TAKE_OFF,			//	菌潼녘膠珂밤꺄
		//	CHARID	charID;
		//	UINT8	slotIndex;
		//	ITEMNO	itemNo;

		PET_CS_ADD_SSP,				//	못녘膠警속君쏜句봬돛야
		//	CONTAINER_SLOT slot;	//	君쏜돛야돨貫零
		//	ITEMNO	itemNo;			//	녘膠돨긍뵀
		PET_SC_ADD_SSP,				//	
		//	GLOBAL_STRING_ID	iErrorID;	//	角뤠냥묘

		PET_CS_CHANGE_NAME,			//	못녘膠錦맣츰俚
		//	CONTAINER_SLOT slot;	//	君쏜돛야돨貫零
		//	ITEMNO itemNo;			//	녘膠돨긍뵀
		//	char   szPetName[LEN_NAME];	//	녘膠돨劤츰냔
		PET_SC_CHANGE_NAME,
		//	GLOBAL_STRING_ID	iErrorID;

        PET_CS_SHOUT,
		//	ITEMNO itemNo;		//	뚤壇돨녘膠
		//	char   szContent[50];	//	별뺐코휭

		PET_SC_SHOUT,
		//	GLOBAL_STRING_ID	iErrorID;
		//	GU_ID  guid;			//	흔벎녘膠瞳겠覡，컸척랙�犬풩횳UID，꼇瞳겠覡，컸척랙箇鯤소돨GUID
		//	char   szContent[50];	//	별뺐코휭

		PET_SC_PET_SHOP_CLOSED,		//	녘膠�絹嫄뻘邈蘭켐ⓙ�
		//	ITEMNO itemNo;


	//////////////////////////////////////////////////////////////////////////
	//
	//	WEDDING
	//

		PI_CS_WEDDING = PI_CS_LOGIC + 4200,

		WEDDING_SC_INFO = PI_CS_WEDDING,
		// GU_ID				ObjectGUID;						// 게임오브젝트아이디
		// WEDDING_INFO			WeddingInfo;					// 웨딩정보
		
		// 신청
		WEDDING_CS_REQUEST,
		// UINT8				iWeddingType;					// 1:교제, 2:프로포즈, 3:결혼
		// char					szRcvCharName[LEN_NAME+1];		// 신청대상자이름
		// int					iEly;							// 소비되야하는 엘리
		// CONTAINER_SLOT		tItemSlot;					// 소비되야하는 아이템

		// 신청
		WEDDING_SC_REQUEST,
		// GU_ID				ObjectGUID;						// 게임오브젝트아이디
		// UINT8				iWeddingType;					// 1:교제, 2:프로포즈, 3:결혼
		// GLOBAL_STRING_ID		ErrorID;						// 에러아이디(0이면 에러없음)
		// char					szRcvCharName[LEN_NAME+1];		// 신청대상자이름


		// 수신자에게 신청 요청이 들어왔을때
		WEDDING_SC_REQUEST_RECV,
		// GU_ID				ObjectGUID;						// 게임오브젝트아이디
		// UINT8				iWeddingType;					// 1:교제, 2:프로포즈, 3:결혼
		// char					szCharName[LEN_NAME+1];			// 이름 

		// 신청 응답
		WEDDING_CS_RESPONSE,
		// UINT8				iWeddingType;					// 1:교제, 2:프로포즈, 3:결혼
		// char					szActCharName[LEN_NAME+1];		// 요청자이름
		// UINT8				byAccept;						// 0:거절, 1:수락


		// 신청 응답
		WEDDING_SC_RESPONSE,
		// GU_ID				ObjectGUID;						// 게임오브젝트아이디
		// UINT8				iWeddingType;					// 1:교제, 2:프로포즈, 3:결혼
		// char					szCharName[LEN_NAME+1];
		// GLOBAL_STRING_ID		ErrorID;						// 에러아이디(0이면 에러없음)

		// errorID가 성공일때만 accept,weddingInfo를 보낸다
		// UINT8				byAccept;						// 0:거절, 1:수락
		// weddingInfo는 accept가 수락일경우만 보낸다
		// WEDDING_INFO			WeddingInfo;					// 웨딩정보
		// TRASH_ITEM			TrashItem;						// 사용할 아이템


		// 결혼 외치기
		WEDDING_CS_CHAT,
		//UINT8					iMsgLen;							// 메세지 길이
		//TCHAR					szMsg[LEN_CASHITEM_NORMAL_MSG+1] = {0,};	

		//기존외치기에 타입추가로 결정
		//CHAT_SC_MSG ,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT 상의 메세지를 itemID 값
		// BROADCAST_SCOPE	scope;				// 브로드캐스팅 범위
		// UINT8			uiFontColor;		// 폰트색	: 0(default),1(Red),2(Orange),3(Yellow),4(white) : CHAT_COLOR.LDT::ItemID
		// UINT8			uiOptions;			// reserved for FontSize,FontType
		// UINT8			len;
		// TCHAR			szMsg[];		
		// GU_ID			guidSender;			// 송신자 정보
		// UINT8			nSenderName;
		// TCHAR			szSender[];
		// UINT16			ChannelID;			// 채널 ID	


		// 결혼식 취소
		WEDDING_SC_WEDDING_CANCEL,
		// GU_ID				ObjectGUID;						// 게임오브젝트아이디


		// 결혼식 시작/끝 알림
		WEDDING_SC_MOTION_STARTEND,
		// GU_ID				globalID; 						// 사용자 아이디
		// UINT8				iStartEnd; 						// 0:Start(외치기창부터시작) 1:Start(외치기창없이시작) 2:End


		//미니맵, 월드맵에서 배우자 위치 보이기
		WEDDING_CS_UPDATEMEMBER,


		//로그오프, 채널이 다르면 안보낸다
		WEDDING_SC_UPDATEMEMBER,
		// GU_ID			globalID;							// 사용자 아이디
		// GU_ID			PartnerID;							// 배우자 GUID, 오프라인일경우 0으로
		// UINT8			iServerID;							// 서버아이디, 오프라인일경우 0으로 
		// int				usStageID;							// stageID, 오프라인일경우나 다른채널일때 -1으로
		// int				usMapGroupID;						// MapGroupID, 오프라인일경우나 다른채널일때 -1으로


		//이별
		WEDDING_CS_PARTING,
		// int			iEly;									// 소모되는엘리


		WEDDING_SC_PARTING,
		// GU_ID				globalID;						// 사용자 아이디
		// GLOBAL_STRING_ID		iError;							// 0 : 성공

		//WEDDING_CS_CHAR_INFO,
		//// char					szCharName[LEN_NAME+1];		// 정보보기하기위한 캐릭터이름


		//WEDDING_SC_CHAR_INFO,
		//// GU_ID				globalID;					// 사용자 아이디
		//// GLOBAL_STRING_ID		iError;						// 결혼 정보가 없는사람은 에러로 표시
		//// int					iLv;
		//// int					iDay;
		//// char					szCharName[LEN_NAME+1];		// 교제자 캐릭터이름

		// 결혼 모션 스킬
		WEDDING_SC_SKILL_USING,
		// RESULTCODE		resultCode;
		// GU_ID			iInstanceID;
		// UINT32			iSkillID;
		// LT_POSITION		pos;

		WEDDING_SC_SUMMON_REQUEST,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디

		// 소환 시전자에게 에러일때만 보낸다
		WEDDING_SC_SUMMON_REQUEST_ERR,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)



		// 연인소환요청응답
		WEDDING_CS_SUMMON_RESPONSE,
		// UINT8			byAccept;					// 0:거절, 1:수락

		// 연인소환요청응답 결과
		// 에러일때는 대상자에게만 보낸다
		WEDDING_SC_SUMMON_RESPONSE,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)

		// 연인소환요청응답
		// 요청자에게 에러가 아닐때 보낸다
		WEDDING_SC_SUMMON_RESPONSE_RECV,
		// GU_ID			ObjectGUID;					// 게임오브젝트아이디
		// GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)
		// UINT8			byAccept;

};

//////////////////////////////////////////////////////////////////////////////
// typedef

#pragma pack(push,1)

typedef UINT8	SLOT_INDEX;	// Slot Index (1 Base)
typedef UINT32	ITEMID;		// Item ID
typedef __int64 ITEMNO;

typedef struct _tagPCSTATUS_STYLE
{
	UINT8			uiHair;			// 머리
	UINT8			uiFashion;		// 패션
	UINT8			uiPersonality;	// 성격
	UINT8			uiCondition;	// 상태
} PCSTATUS_STYLE, *PPCSTATUS_STYLE;

typedef struct _tagPCSTATUS_POS
{
	int				iRevivalMap;	// 부활위치
	int				iTownMap;	// 부활위치
	UINT16			uiStageID;		// StageID
	UINT16			uiMapGroupID;	// MapGroupID
	UINT16			uiLayer;		// 레이어
	int				iPosX;			// x좌표
	int				iPosY;			// y좌표
	UINT8			uiDir;			// PC가 바라보는 방뱡

} PCSTATUS_POS, *PPCSTATUS_POS;

//  [11/18/2004] 장비 아이템 by metalgeni
typedef struct _CHAR_EQUIPMENT
{
	INT16	iEquipID;			// enum EQUIP_ID		 0 = NULL, -1 = INVALID
	INT16	iEquipClass;		// enum EQUIP_CLASS		 0 = NULL, -1 = INVALID
	INT16	iiItemID;			// 0~999
	INT8	iiItemColor;		// 0~99
	UINT32  uiItemUniqueID;		
} CHAR_EQUIPMENT;


typedef struct _Gamble_Status
{
	UINT    uGambleIndex;	//	겟괜芎돨긍뵀
	int		nBuyStep;	//	뭔찜돨쌓뙈，0灌굳뭔찜，1槨綠쒔뭔찜，뎃角청唐뫘劤膠틔，2槨綠쒔뭔찜깻할綠쒔뫘劤膠틔斤口
	ITEMID	itemID;		//	綠쒔뭔찜裂빈돨itemID
	UINT32	uSetID;		//	쉽틔돨잚깎ID

	_Gamble_Status()
	{
		uGambleIndex = 0;
		nBuyStep = 0;
		itemID = 0;
		uSetID = 0;
	}

}t_Gamble_Status;


typedef struct _Gamble_Getted_Item
{
	ITEMID	itemID;		//	삿돤돨쉽틔
	UINT8   usCount;	//	쉽틔돨鑒좆
	UINT32	uSetID;		//	쉽틔돨잚깎ID

	_Gamble_Getted_Item()
	{
		itemID	= 0;
		usCount = 0;
		uSetID  = 0;
	}

}t_Gamble_Getted_Item;


//[xialin 2008/07/25]add
enum CASH_EFFECT
{
	EFT_ADDPARCEL = 1,			//속댕관범으
	EFT_ADDCONSUME = 2,			//속댕句봬으
	EFT_ADDETC = 3,				//속댕페劍으
	EFT_ADDEVENT = 4,			//속댕훨蛟으
	EFT_ADDPET = 5,				//속댕녘膠으
	EFT_ADDWH = 6,				//속댕꾑욋으
	
};


enum VALUE_TYPE
{
	VALUE_BASE, // 고유값
	VALUE_CURR, // 최종값
	VALUE_MAX,
};


enum STAT
{
	STAT_LUK,	// luck
	STAT_ATK,	// attack
	STAT_DEF,	// defense
	STAT_ELE,	// element
	STAT_TOTAL,
};

enum ELEMENT
{
	WATER,
	EARTH,
	FIRE,
	WIND,
	ELEMENT_TOTAL,
};
enum ATK_TYPE
{
	ATK_NORMAL,
	ATK_HEAVY,
	ATK_JUMP,
	ATK_MAX,
};

typedef struct _MOVING_STATUS
{
	float			fMaxVelocityX;
	float			fJumpSpeed;
	float			fUpDownVelocity;
	float			fHangingVelocity;
	float			fDropSpeedNow;
	float			fDropSpeedMax;

}MOVING_STATUS;

// 크리쳐 기본 정보
typedef struct _CREATURE_INFO
{
	GU_ID	uiGlobalID;							// ClassID + InstanceID
	TCHAR	szName[LEN_NAME];
	UINT16	uiLevel;
	UINT32	uiMHP[VALUE_MAX];					// Base MaxHp, Curr MaxHP
	UINT32	uiMSP[VALUE_MAX];					// Base MaxSp, Curr MaxSP
	UINT32	uiHP;								// NowHP
	UINT32	uiSP;								// NowSP
	UINT32	uiRHP;								// Recv HP
	UINT32	uiRSP;								// Recv SP
} CREATURE_INFO,*PCREATURE_INFO;

// 스탯 정보(LUK,ATK,DEF,ELE,WATER,EARTH,FIRE,WIND)
typedef struct _CHAR_STS
{
	UINT16		Sts[STAT_TOTAL][VALUE_MAX]; 
	UINT16		Ele[ELEMENT_TOTAL];				// 최종값만 가짐.
	UINT16		Atk[ATK_MAX];					// 최종값만 가짐.

} CHAR_STS, *PCHAR_STS;

// 경험치 정보
typedef struct _CHAR_EXP
{
	UINT64  uiCurExp;
	UINT64  uiMinExp;
	UINT64  uiMaxExp;

} CHAR_EXP, *PCHAR_EXP;

// 몬스터 정보
typedef struct _MONSTER_INFO
{
	CREATURE_INFO	BaseInfo;
	MOVING_STATUS   MovingStatus;
	int iType;


} MONSTER_INFO,*PMONSTER_INFO;

// NPC 정보
typedef struct _NPC_INFO
{
	CREATURE_INFO	BaseInfo;

} NPC_INFO,*PNPC_INFO;

// 캐릭터 기본 정보
typedef struct _CHARACTER_INFO2
{
	CREATURE_INFO	BaseInfo;
	USERID			uiUserID;					// 계정에 대한 ID
	UINT32			uiTitleID;					// 타이틀
	UINT64			uiMoney;					// 게임머니
	UINT8			uiClassType;				// Class Type
	UINT8			uiSex;						// 1: 남, 2: 여
	UINT8			uiWeaponSet;				// 0: 13,14  1:15,16 
	UINT64			uiStorageMoney;				// 창고머니
	MOVING_STATUS	MovingStatus;
	UINT32			Fame[VALUE_MAX];			// 명성치
	INT32			iGameOptions;				// 2007.02.21 dhpark 서버에서 저장해야할 게임옵션들

	// 이동
} CHARACTER_INFO2, *PCHARACTER_INFO2;


// 캐릭터 캐쉬정보 20080306 mijin
typedef struct _CHARACTER_CASH_INFO
{
	int			iCash;						// 20080306 mijin 캐시
	int			iMile;						// 20080306 mijin 마일리지
	UINT8		uiHotCashShopMode;			// 20080415 mijin 추천상품 블러킹 여부 (0: close, 1:open)
} CHARACTER_CASH_INFO, *PCHARACTER_CASH_INFO;



// 친구 목록상의 캐릭터 정보
typedef struct _FRIEND_INFO
{
	TCHAR			szCharName[LEN_NAME+1];
	UINT8			uiClassType;				// 공격형,...
	UINT8			uiLevel;					// 해당 캐릭터의 현재 레벨
	UINT16			uiServerID;					// 0(off-line), >1(on-line)
	CHARID			friendCharID;

}FRIEND_INFO,*PFRIEND_INFO;

// 팀원 정보 전송
typedef struct _TEAMMEMBER_INFO : public CREATURE_INFO
{

}TEAMMEMBER_INFO,*PTEAMMEMBER_INFO;


// 스탯 정보
typedef struct _STATUS_INFO
{
	CHAR_EXP		ExpInfo;
	CHAR_STS		StsInfo;
	short			iSkillFreePoint;
	short iAmor;					// 방어력
	short iResEarth;				// 저항력
	short iResWind;				// 저항력
	short iResWater;				// 저항력
	short iResFire;				// 저항력
	short iReqLv;					// 제한레벨
	short iProbEnchat;			// 인첸트 성공률.
} STATUS_INFO,*PSTATUS_INFO;

// 이팩트에 의하여 변경될수 있는 스탯값들
typedef struct _EFFECT_STATUS_INFO
{
	UINT16	uiLevel;
	UINT32	uiMHP[VALUE_MAX];					// Base MaxHp, Curr MaxHP
	UINT32	uiMSP[VALUE_MAX];					// Base MaxSp, Curr MaxSP
	UINT32	uiHP;								// NowHP
	UINT32	uiSP;		
	UINT32  uiRHP;
	UINT32  uiRSP;
	UINT32	Fame[VALUE_MAX];
	STATUS_INFO		Status;
	MOVING_STATUS	MovingStatus;
} EFFECT_STATUS_INFO, *PEFFECT_STATUS_INFO;

// 위치 정보
typedef struct _LT_POSITION
{
	float	fPosX;								// 현재 위치
	float	fPosY;			
	float	fVelocityX;							// 걷기 속도		ACCUMULATE_PROCESS_TICK기준
	float	fVelocityY;							// 떨어지는 속도	ACCUMULATE_PROCESS_TICK기준
	float	fAccelX;							// 가속도			ACCUMULATE_PROCESS_TICK기준
	float	fAccelY;			
	int		eFacing;							// 방향 (FACING)
	int		bgStandLayer;						// 현재 서있는 터레인 레이어 (BG_ID)

	bool	bKnockBack;
	bool	bDelay;
	float	fKnockTime;
	float	fDelayTime;
	float	fKnockVelocityX;
	float	fKnockVelocityY;

}LT_POSITION,*PLT_POSITION;

typedef struct _LT_KNOCKBACK
{
	bool	bKnockBack;
	bool	bDelay;
	float	fKnockTime;
	float	fDelayTime;
	float	fKnockVelocityX;
	float	fKnockVelocityY;

}LT_KNOCKBACK,*PLT_KNOCKBACK;

typedef struct _MAP_INFO
{
	 UINT16	uiStageID;			// 스테이지 ID
	 UINT16	uiMapGroupID;		// 맵그룹 ID
	 UINT16	uiMapID;			// 맵 ID
}MAP_INFO,*PMAP_INFO;

enum EVENT_TYPE{EVENT_NULL,EVENT_NPC,EVENT_SAVEPOINT,EVENT_PORTAL,EVENT_QUEST,EVENT_MAILBOX,EVENT_BEAUTY,EVENT_CROPS,EVENT_CORE,EVENT_CHATROOMOBJECT,MAX_EVENT};

/**
* 현재 NPC 의 EVENT 세부 사항 <br> 
*/
enum NPC_EVENT_TYPE {
	NPC_EVENT_TYPE_NULL				= 0,	/**< 아무것도 아님 <br> */
	NPC_EVENT_TYPE_TALK				= 1,	/**< 대화 <br> */
	NPC_EVENT_TYPE_TRADE			= 2,	/**< 상점 <br> */
	NPC_EVENT_TYPE_STORAGE			= 3,	/**< 창고 <br> */
	NPC_EVENT_TYPE_QUEST			= 4,	/**< Quest <br> */
	NPC_EVENT_TYPE_GIFT				= 5,	/**< Gift <br> */
	NPC_EVENT_TYPE_GUILD_CREATE		= 6,	/**< 길드 생성<br> */
	NPC_EVENT_TYPE_GUILD_DISSOLVE	= 7,	/**< 길드 해체 <br> */
	NPC_EVENT_TYPE_GUILD_ENTER		= 8,	/**< 길드 입장 <br> */
	NPC_EVENT_TYPE_PORTAL			= 9, 	/**< NPC를 통한 포탈 <br> */
	NPC_EVENT_TYPE_CHANGE			= 10,	/**< NPC를 통한 이벤트 아이템 교환 <br> */
	NPC_EVENT_TYPE_GUILD_LEVELUP	= 11,	/**< 길드 레벨 업 <br> */
	NPC_EVENT_TYPE_GUILD_EMBLEM		= 12,	/**< 길드 엠블렘 변경 <br> */	
	NPC_EVENT_TYPE_PVP_RESERVE		= 13,	/**< PVP 예약 <br> */
	NPC_EVENT_TYPE_AUCTION			= 14,	/**< 경매 <br> */
	NPC_EVENT_TYPE_REOPTION				= 15,	/**< 옵션 재분배 <br> */ 
	NPC_EVENT_TYPE_PARTY_PVP_RESERVE	= 16,	/**< 파티 PVP 예약 <br> */  
	NPC_EVENT_TYPE_RANKING				= 17,
	NPC_EVENT_TYPE_WEDDING_RESERVE		= 18,	/**< 결혼 NPC를 통한 식 예약 <br> */  
	NPC_EVENT_TYPE_WEDDING_DIVORCE		= 19,	/**< 결혼 NPC를 통한 이혼 <br> */  
};


// 재구성할 MapEventInfo에 들어갈 요구 조건
typedef struct _REQUIRE_INFO {

	UINT32	requireType;
	UINT32	requireID;
	UINT32	requireValue1;
	UINT32	requireValue2;

	_REQUIRE_INFO(int iType = 0, int iID = 0, int iValue1 = 0, int iValue2 = 0) {		
		SetRequireInfo(iType, iID, iValue1, iValue2);
	}

	void Clear() {
		requireType			= 0;
		requireID			= 0;
		requireValue1		= 0;
		requireValue2		= 0;		
	}

	void SetRequireInfo(int iType, int iID, int iValue1, int iValue2) {
		requireType			= iType;
		requireID			= iID;
		requireValue1		= iValue1;
		requireValue2		= iValue2;
	}

}REQUIRE_INFO;

typedef struct _EVENT_INFO
{
	UINT32		id; // EventID

	// position
	MAP_INFO	mapInfo	;
	float		posX;
	float		posY;

	_EVENT_INFO()
	{
		id						= 0;

		ZeroMemory(&mapInfo,sizeof(MAP_INFO));
		posX					= 0.0;
		posY					= 0.0;
	}

} EVENT_INFO, *PEVENT_INFO;


// MapEvent Info의 재구성
typedef struct _MAPEVENT_INFO 
{
	UINT32	classID;				// AVATAR_MAPEVENT_*
	UINT32	instanceID;
	UINT32	id;

	MAP_INFO mapInfo;
	float	posX;
	float	posY;
	float	rectTop;
	float	rectLeft;
	float	rectRight;
	float	rectBottom;

	UINT8	eventType;
	UINT32	eventSubType;	
	UINT32	eventTypeID;

	UINT32	obejctImage;
	UINT8	iTerrainLayer;
	UINT8	iFacing;	

	REQUIRE_INFO	requireInfo[2];	

	// 아이템 소모 관련( 포탈이벤트시만 이용:기획팀의도 ) 2007/01/19 
	ITEMID			iConsumeItemID;

	TCHAR name[LEN_NAME+1];	
	TCHAR tipMessage[512];
	TCHAR errorMessage[512];
	TCHAR tipMessageOff[512];
	TCHAR dummy[3];	

	UINT32	iInsDungeunID;
	UINT8	byDynamicCreate;
	UINT	iDynamicFX ;

	_MAPEVENT_INFO() {
		Clear();
	}

	void Clear() {
		classID					= 0;
		instanceID				= 0;
		id						= 0;

		//stageID				= 0;
		//mapGroupID			= 0;
		//mapID					= 0;
		//mapInfo.Clear();
		ZeroMemory(&mapInfo, sizeof(MAP_INFO));

		posX					= 0;
		posY					= 0;
		rectTop					= 0;
		rectLeft				= 0;
		rectRight				= 0;
		rectBottom				= 0;

		eventType				= 0;
		eventSubType			= 0;
		eventTypeID				= 0;

		obejctImage				= 0;
		iTerrainLayer			= 0;
		iFacing					= 0;

		iConsumeItemID			= 0;

		requireInfo[0].Clear();
		requireInfo[1].Clear();

		ZeroMemory(name, LEN_NAME + 1);
		ZeroMemory(tipMessage, 512);
		ZeroMemory(errorMessage, 512);
		ZeroMemory(tipMessageOff, 512);

		iInsDungeunID			= 0;
		byDynamicCreate			= 0;
		iDynamicFX				= 0;
	}

	void SetEventInfo(EVENT_INFO* srcEventInfo) {
		id = srcEventInfo->id;
		mapInfo = srcEventInfo->mapInfo;
		posX = srcEventInfo->posX;
		posY = srcEventInfo->posY;
	}

}MAPEVENT_INFO;

// 4byte 스탯
enum STATUS_TYPE 
{
	STATUS_NONE=0,
	STATUS_LV,
	STATUS_LUK_BASE,					// 레벨업시 채팅창에 표시되는문제로 순서 고정!!!! [2/6/2006 AJJIYA]
	STATUS_ATK_BASE,					// 레벨업시 채팅창에 표시되는문제로 순서 고정!!!! [2/6/2006 AJJIYA]
	STATUS_DEF_BASE,					// 레벨업시 채팅창에 표시되는문제로 순서 고정!!!! [2/6/2006 AJJIYA]
	STATUS_ELE_BASE,					// 레벨업시 채팅창에 표시되는문제로 순서 고정!!!! [2/6/2006 AJJIYA]
	STATUS_MHP_BASE,
	STATUS_MSP_BASE,
	STATUS_FAME_BASE,
	STATUS_LUK, 						// 최종값
	STATUS_ATK,  						// 최종값
	STATUS_DEF,	 						// 최종값
	STATUS_ELE,  						// 최종값
	STATUS_MHP,  						// 피통
	STATUS_MSP,							// 마나통
	STATUS_HP,  						// 현재피
	STATUS_SP,							// 현재마나
	STATUS_FAME,
	STATUS_CLASS_TYPE,
	STATUS_PVP_LV,						// PVP Rank Level
	STATUS_PVP_POINT,					// PVP 거래 포인트
	STATUS_EARTH,
	STATUS_WIND,
	STATUS_WATER,
	STATUS_FIRE,
	STATUS_NORMAL_ATK, 
	STATUS_HEAVY_ATK,
	STATUS_RECOVERY_HP,
	STATUS_RECOVERY_SP,
	STATUS_SKILL_POINT,
	STATUS_GENDER,
	STATUS_AMOR,						// 방어력
	STATUS_RES_EARTH,					// 저항력
	STATUS_RES_WIND,					// 저항력
	STATUS_RES_WATER,					// 저항력
	STATUS_RES_FIRE,					// 저항력
	STATUS_REQ_LV,						// 제한레벨스텟
	STATUS_PROB_ENCHANT,				// 인첸트 성공확률보정.		
	STATUS_CASH,						//20080310 mijin 캐쉬(오즈)
	STATUS_MILE,						//20080310 mijin 마일리지
	STATUS_PHYCRI,						// 물리크리
	STATUS_ELECRI,						// 마법크리
	STATUS_PHYDODGE,					// 물리회피
	STATUS_ELEDODGE,					// 마법회피
	STATUS_PHYHITRATE,					// 물리명중
	STATUS_ELEHITRATE,
	STATUS_LOVE_LV,						// 연애 Level
	STATUS_LOVE_POINT,					// 연애 포인트 현재값 [10/2/2008 AJJIYA]
	STATUS_LOVE_POINT_MAX,				// 연애 포인트 최대값 [10/2/2008 AJJIYA]
	STATUS_LOVE_DAY,					// 연애 교제 일 수 [10/2/2008 AJJIYA]
	STATUS_LOVE_STATE,					// 연애 상태 ( 0 : None , 1 : 교제상태 , 2 : 프로포즈 완료 상태 , 4 : 결혼상태 ) [10/2/2008 AJJIYA]
	STATUS_MOB_COUNT,					// 소환한 몹갯수
	STATUS_EXTENDED_CHARSLOT,			// 확장된 캐릭터 슬롯수
	STATUS_RANKING_LEVEL,				// 랭킹시스템 : 레벨랭킹
	STATUS_RANKING_PVP,					// 랭킹시스템 : PVP
	STATUS_RANKING_LOVE,				// 랭킹시스템 : 애정도
	STATUS_RANKING_GUILD,				// 랭킹시스템 : 길드
	MAX_STATUS
};

// 기본 8BYTE 스탯.
enum STATUS_EX_TYPE		
{
	STATUS_EX_NONE = 0,
	STATUS_EX_EXP,
	STATUS_EX_EXP_MAX,
	STATUS_EX_EXP_MIN,
	STATUS_EX_ELY,
	STATUS_EX_STORAGE_ELY,				//[2005/7/20]
	STATUS_EX_PVP_GLORY,				//[2007/11/27]	PVP 명예 포인트
	MAX_STATUS_EX,		
};

typedef struct _STATUS_CHANGE
{
	UINT16				eType;			// enum STATUS_TYPE 
	UINT32				uiValue;		// value;
} STATUS_CHANGE, *PSTATUS_CHANGE;

typedef struct _STATUS_CHANGE_EX
{
	UINT8				eType;			// enum STATUS_EX_TYPE
	UINT64				uiValue;		// value;
}STATUS_CHANGE_EX, *PSTATUS_CHANGE_EX;



// PC's mode
enum PC_MODE {PM_NONE=0,
		PM_INITIALIZING,				// 초기화중.
		PM_CHAR_MANAGING,				// 캐릭터 생성/삭제/선택 중
		PM_PALYING,						// 게임중
		PM_EXITING,						// 게임 종료중
		MAX_PM};

// 월드별 캐릭터수 정보
//typedef struct _WORLD_INFO
//{
//	WORLD_ID	wid;
//	TCHAR		name[LEN_NAME];
//	TCHAR		ip[LEN_IP];
//	SERVER_PORT	port;
//	int			nCharCount;
//	float		fStatusValue;			// 월드내 총동접수/최대접속가능수
//}WORLD_INFO;
//
//typedef struct _CHANNEL_INFO
//{
//	SERVER_ID	uiChannelID;
//	UINT16		uiMaxUsers;
//	UINT16		uiCurUsers;
//	//float		fStatusValue;			// 채널내 유저수/최대가능수
//}CHANNEL_INFO,*PCHANNEL_INFO;

// 월드별 캐릭터수 정보
typedef struct _WORLD_INFO
{
	UINT16		wid;
	TCHAR		name[LEN_NAME];
	SERVER_ID	usLobbySID;
	TCHAR		ip[LEN_IP];
	UINT16		port;
	UINT8		nCharCount;
	UINT8		status;					// 쾌적(0),보통(1),혼잡(2)
}WORLD_INFO;

typedef struct _CHANNEL_INFO
{ 
	UINT16		channelID;
	TCHAR		name[LEN_NAME];
	UINT8		status;					// 쾌적(0),보통(1),혼잡(2),접속불가(3)
}CHANNEL_INFO;


// 채팅 메세지를 전송할 범위를 지정
// 채팅쪽 추가시 LDT에서 해당부분 지정되어 있으므로 필히 사전 상의 부탁드립니다. [4/23/2007 AJJIYA]
enum BROADCAST_SCOPE 
{
	SCOPE_BROADCAST			=	0		,
	SCOPE_AROUNDREGION					,			// 근처(브로드캐스팅 영역내) PC			: 채널내
	SCOPE_FRIENDLIST					,			// 친구 목록상의 모든 유저				: 동일 채널내로 한정
	SCOPE_GUILD							,			// 길드채팅								: 월드내 길드원들
	SCOPE_TEAM							,			// 팀									: 동일 채널내로 한정
	SCOPE_WHISPER						,			// 특정 유저(귓속말)					: 월드내로 한정
	SCOPE_BUGLE							,
	
	SCOPE_CHANNEL			=	1001	,			// 특정 채널내의 모든 유저				: 채널내
	SCOPE_WORLD				=	1002	,			// 특정 월드내의 모든 유저				: 월드내
	SCOPE_CHATROOM_ADVERT	=	1003	,			// 대화방 생성시 월드내의 모든 유저를 대상으로 하는 광고 메세지
	SCOPE_WEDDING			=	1004	,			// 결혼시 월드내의 모든유저에게 광고

	SCOPE_NOTICE			=	2001	,			// 공지사항. 특정 월드내의 모든 유저(GM Only)	: 월드내
	SCOPE_SYSTEMMSG			=	2002	,			// 시스템 메세지 전송용(cheat 결과,오류/경고 메세지,...)
};

//////////////////////////////////////////////////////////////////////////
// Item Structure
enum CONTAINER_TYPE
{
	CONTAINER_TYPE_INVALID = -1,
	INVENTORY_EQUIP=0,
	INVENTORY_CONSUME,
	INVENTORY_ETC,
	INVENTORY_EVENT,
	INVENTORY_PET,
	INVENTORY_HOUSING,
	FIGURE_EQUIP,
	FIGURE_FASHION,
	FIGURE_BASIC,
	STORAGE,
	INVENTORY_CASH,
	FIGURE_PET,
	INVENTORY_WEAR,        //[xialin 2008/07/21] add 桿누션쩌
	FIGURE_EFFECT,
	CONTAINER_MAX,
	CONTAINER_AUCTION = 88,
	CONTAINER_MAILBOX = 99,
};

// Packet용 

/*-------------------------------------------------*/

// 캐릭터의 기본 외형 정보

/*-------------------------------------------------*/

enum FIGURE_BASIC_TYPE
{
	FIGURE_BASIC_NULL,
	FIGURE_BASIC_UNDER_SHIRT, // 속옷 상의
	FIGURE_BASIC_UNDER_PANTS, // 속옷 하의
	FIGURE_BASIC_HAIR,
	FIGURE_BASIC_FACE,
	FIGURE_BASIC_SKIN,
	FIGURE_BASIC_MAX,
};

/*-------------------------------------------------*/

// 패션창 슬롯번호

/*-------------------------------------------------*/

enum EQUIP_SLOT_NUMBER
{
	EQUIP_SLOT_NULL,
	EQUIP_SLOT_HEAD,		// 머리
	EQUIP_SLOT_FACE1,		// 안경
	EQUIP_SLOT_EARRING,		// 귀걸이
	EQUIP_SLOT_SHIRT,		// 셔츠
	EQUIP_SLOT_PANTS,		// 바지
	EQUIP_SLOT_MANTLE,		// 망토
	EQUIP_SLOT_GLOVE,		// 장갑
	EQUIP_SLOT_SHOES,		// 신발
	EQUIP_SLOT_LEG,			// 다리
	EQUIP_SLOT_FACE2,		// 얼굴에 차는악세서리
	EQUIP_SLOT_STONE,		// 정령석		
	EQUIP_SLOT_RING,		// 반지	
	EQUIP_SLOT_WEAPON1,
	EQUIP_SLOT_BUCKLER1,
	EQUIP_SLOT_WEAPON2,
	EQUIP_SLOT_BUCKLER2,
	EQUIP_SLOT_MAX,
};

/*-------------------------------------------------*/

// 패션창 슬롯번호

/*-------------------------------------------------*/
enum FASHION_SLOT_NUMBER
{
	FASHION_SLOT_NULL,
	FASHION_SLOT_HEAD,			// 머리
	FASHION_SLOT_FACE1,			// 얼굴에 하는 아이템
	FASHION_SLOT_EARRING,		// 귀걸이
	FASHION_SLOT_SHIRT,			// 셔츠
	FASHION_SLOT_PANTS,			// 바지
	FASHION_SLOT_MANTLE,		// 망토
	FASHION_SLOT_GLOVE,			// 장갑
	FASHION_SLOT_SHOES,			// 신발
	FASHION_SLOT_LEG,			// 다리
	FASHION_SLOT_FACE2,			// 얼굴에 하는 아이템
	FASHION_SLOT_STONE,			// 정령석
	FASHION_SLOT_RING,			// 반지
	FASHION_SLOT_MAX
};

//--------------------------------------------------
// effect slot number
enum EFFECT_SLOT_NUMBER
{
	EFFECT_SLOT_NULL,

	EFFECT_SLOT_HEAD,		// 머리
	EFFECT_SLOT_FACE1,		// 안경
	EFFECT_SLOT_EARRING,	// 귀걸이
	EFFECT_SLOT_SHIRT,		// 셔츠
	EFFECT_SLOT_PANTS,		// 바지
	EFFECT_SLOT_MANTLE,		// 망토
	EFFECT_SLOT_GLOVE,		// 장갑
	EFFECT_SLOT_SHOES,		// 신발
	EFFECT_SLOT_LEG,		// 다리
	EFFECT_SLOT_FACE2,		// 얼굴에 차는악세서리
	EFFECT_SLOT_STONE,		// 정령석		
	EFFECT_SLOT_RING,		// 반지	

	EFFECT_SLOT_FORCEENUMMAX
};
//--------------------------------------------------

/*-------------------------------------------------*/

// 펫창 슬롯번호

/*-------------------------------------------------*/
enum PET_SLOT_NUMBER
{
	PET_SLOT_NULL,
	PET_SLOT_PRIMARY,				// 펫장착
	PET_SLOT_SECONDARY,
	PET_SLOT_TERTIARY,
	PET_SLOT_MAX
};

enum PET_PLACE
{
	PET_PLACE_NULL = -1,
	PET_PRIMARY,
	PET_SECONDARY,
	PET_TERTIARY,

	PET_PLACE_TYPE_NUMBER
};

//typedef UINT32	DUABILITY;	// 내구도
typedef UINT32	DURATION;	// 지속시간 
typedef UINT32	MOBID;

// 컨테이너 슬롯 정보
typedef struct _CONTAINER_SLOT
{
	CONTAINER_TYPE	ContainerType;
	SLOT_INDEX		SlotIndex;

} CONTAINER_SLOT, *PCONTAINER_SLOT;

typedef struct _CHAR_FIGURE
{
	ITEMID FIGURE[FIGURE_BASIC_MAX];
}CHAR_FIGURE,*PCHAR_FIGURE;

// pc Container 정보
typedef struct _CONTAINER_SLOT_INFO
{
	CONTAINER_TYPE ContainerType;
	UINT8		   iContainerSlot;
	UINT8		   iOptionalSlot;
} CONTAINER_SLOT_INFO;

#define MAX_ITEM_OPT 5

typedef struct _PACK_OPT
{
	short iID;
	short iValue;
} PACK_OPT, *PPACK_OPT;

typedef struct _ITEM
{
	ITEMID		iItemID;			// 아이템 아이디
	UINT8		iStackCount;		// 아이템 갯수
	int			iLifeTime;			// 남은 타임이므로 음수 값이 올수 있음. DB 와 연관있음.
	UINT8		iRare;				// 레어도
	PACK_OPT	Opt[MAX_ITEM_OPT];	// 옵션
	ITEMNO		iItemNo;
	int			iBackPrice;		// 
	short		iOptPoint;
	INT64		iItemExp;			// 아이템경험치
	INT16		iItemSP;			// 아이템SP
} ITEM, *PITEM;

// 슬롯의 아이템 sizeof(CONTAINER_ITEM) => 56
typedef struct _CONTAINER_ITEM
{
	CONTAINER_SLOT	SlotInfo;
	ITEM			Item;		// Item Object
} CONTAINER_ITEM, *PCONTAINER_ITEM;

// 외형적으로 뿌려주는 아이템 (주로 브로드케스트용...)
typedef struct _FIGURE_ITEM
{
	CONTAINER_SLOT  SlotInfo;
	ITEMID			iItemID;	// Item ID --> 가져와서 해당 ID를 찾아서 뿌려준다.

}FIGURE_ITEM, *PFIGURE_ITEM;


typedef struct _LOOTING_ITEM
{
	SLOT_INDEX iSlotIndex;
	ITEMID	   iItemID;    // 검증용.
} LOOTING_ITEM, *PLOOTING_ITEM;

typedef struct _TRASH_ITEM
{
	CONTAINER_SLOT	iContainerSlot;
	UINT8			iCount;
} TRASH_ITEM, *PTRASH_ITEM;

typedef struct _CHANGE_SLOT
{
	CONTAINER_SLOT	SrcSlotInfo;
	UINT8			iCount;
	CONTAINER_SLOT	DestSlotInfo;
} CHANGE_SLOT, *PCHANGE_SLOT;

// 트레이드 아이템창에 아이템을 올릴때 사용하는 구조체
typedef struct _TRADE_ITEM
{
	CONTAINER_SLOT	stActSlotInfo;
	SLOT_INDEX		TradeSlotIndex;

} TRADE_ITEM, *PTRADE_ITEM;

// 아이템 라이프타임 구조체
typedef struct _ITEM_LIFETIME
{
	CONTAINER_SLOT	stSlotInfo;
	int				iLifeTime;

} ITEM_LIFETIME, *PITEM_LIFETIME;

// 아이템 경험치 구조체
typedef struct _ITEM_EXP
{
	CONTAINER_SLOT	stSlotInfo;
	INT64			iItemExp;

} ITEM_EXP, *PITEM_EXP;

// 아이템 SP 구조체
typedef struct _ITEM_SP
{
	CONTAINER_SLOT	stSlotInfo;
	INT16			sItemSP;

} ITEM_SP, *PITEM_SP;

// Upgrade에서 이용되는

typedef struct _MATERIAL_DATA
{
	CONTAINER_TYPE	iContainerType;		// 재료 아이템 위치  
	UINT8			iContainerSlot;		// 재료 아이템 위치
	ITEMNO			iItemNo;			// 재료 아이템 번호
	UINT8			iItemCount;			// -- 참조용
} MATERIAL_DATA, *PMATERIAL_DATA;

typedef struct _EFFECT_DATA
{
	unsigned int iInstanceID;
	int iEffectID;
	UINT8 iEffectLv;
	int iRemainTime; // 단위 :0.1초
	GU_ID iCasterID;
} EFFECT_DATA, *PEFFECT_DATA;

// mijin 쿨타임 구조체
typedef struct _COOLTIME_DATA
{
	int iCoolTimeID;
	int	iMaxCoolTime;	// 처음 적용될 쿨타임 ( 단위 : 초 )
	int iApplyCoolTime; // 남은 쿨타임 ( 단위 : 초 )

} COOLTIME_DATA, *PCOOLTIME_DATA;


// QUICK SLOT
enum QUICK_LINK_FLAG
{
	QUIKC_LINK_FLAG_INVALID = 0,
	QUICK_LINK_FLAG_ITEM,
	QUICK_LINK_FLAG_ACTION,
	QUICK_LINK_FLAG_SKILL,
	QUICK_LINK_FLAG_ITEMSKILL,
	QUICK_LINK_FLAG_CHAT,
	QUICK_LINK_FLAG_PETSKILL,
};

typedef struct _LINKED_DATA
{
	UINT8	iFlag;			/* QUICK_SLOT_FLAG */
	int		iObjTypeID;		/* ItemID, SkilllID, ActionID */
	int		iSubTypeID;		/* ItemID */ // 아이템스킬저장시 사용

}LINKED_DATA, *PLINKED_DATA;

typedef struct _QUICK_SLOT 
{
	LINKED_DATA LinkedData;
	UINT8		iSlotIndex; /* Quick slot position */ // Valid Range : 0~MAX_QUICK_SLOT -1 
} QUICK_SLOT, *PQUICK_SLOT;

#define START_EMOTICON_SLOT	36	// 2005.08.30 dhpark 이모티콘 시작 인덱스 정의

#define MAX_QUICKSLOT_DATAPERTAB	12
#define MAX_QUICKSLOT_TAB			4
#define MAX_QUICK_SLOT				MAX_QUICKSLOT_DATAPERTAB * MAX_QUICKSLOT_TAB


//////////////////////////////////////////////////////////////////////////
//
//	Npc section
//

#define MAX_NPC_TRADE_ITEM			50
#define MAX_LOOT_ITEM				3
#define MAX_LOOT_ITEM_SET			7
#define MAX_MOB_LOOT_ITEM			(MAX_LOOT_ITEM + MAX_LOOT_ITEM_SET)





//  위치가 애매해서 임시로 ..

//////////////////////////////////////////////////////////////////////////
//
// npc로부터 살때 이용
//
//////////////////////////////////////////////////////////////////////////
//
// 파라미터 설명
// ItemPrice : 사고자 하는 아이템의 "ITEM.LDT"의 "_GamePrice".
// ItemCount : NPC 에게 살 아이템 갯수.
// ShopSellPercent : "NPC_TRADE.LDT"의 해당 아이템 "_SellFactor". 
// PCBuyPercent : PC에게 적용된 구입배율. 현재는 기본 100 % 로 입력요망. 

#define GET_BUY_PRICE(ItemPrice,ItemCnt,ShopSellPercent,PCBuyPercent) \
	(ItemPrice*ShopSellPercent/100*PCBuyPercent/100*ItemCnt)

//////////////////////////////////////////////////////////////////////////
//
// NPC에게 팔때 이용
//
// ItemPrice : 팔고자하는 아이템의 "ITEM.LDT"의 "_GamePrice".
// ItemCount : NPC 에게 팔 아이템 갯수.
// ShopBuyPercent : "NPC_TRADE.LDT"의 "_BuyFactor". 
// PCSellPercent : PC에게 적용된 판매배율. 현재는 기본 100 % 로 입력요망. 
// 					

#define GET_SELL_PRICE(ItemPrice,ItemCnt,ShopBuyPercent,PCSellPercent) \
	(ItemPrice*ItemCnt*ShopBuyPercent/100*PCSellPercent/100)


//////////////////////////////////////////////////////////////////////////
//
// 유저간 캐쉬 거래시 수수료 계산
// 20080313 mijin
//	- 거래비용의 소수점 이하는 버림
//
// TradeCash : 거래되는 캐쉬
// CommPercent : 수수료 퍼센트(LOCALIZE.LDT 에 표기)
//////////////////////////////////////////////////////////////////////////					
#define GET_TRADE_CASH_COMM(TradeCash, CommPercent) (TradeCash*CommPercent/100)




// 2005.07.28 dhpark Trade시 거리계산용 Define
const float CHECK_INTERVAL_X = 400.0f;			/**< 상대 PC와 의 거리 체크용 <br> */
const float CHECK_INTERVAL_Y = 300.0f;			/**< 상대 PC와 의 거리 체크용 <br> */

//------------------------------------------------------------------------------
// QUEST

#define QUEST_MAX_ACTIVE				15	// 진행중인 퀘스트의 최대 개수
#define QUEST_MAX_NAME					64	// LDT에 설정된 txt의 크기기준임
#define QUEST_MAX_REQUIRE				5	/**< 습득에 필요한 스탯 종류의 최대 갯수 <br> */
#define QUEST_MAX_ACQUISITION_EFFECTID	5	/**< 퀘스트 습득시 함께 발생하는 효과의 최대 갯수 <br> */
#define QUEST_MAX_ACQUISITION_ITEM		2	/**< 퀘스트 습득시 함께 습득하는 아이템 최대 갯수 <br> */
#define QUEST_MAX_MISSION				4	/**< 완료에 필요한 미션의 갯수 <br> */
#define QUEST_MAX_CONSUMPTION_ITEM		5	/**< 퀘스트 완료(보상)시 소비되는 아이템 종류의 최대 갯수 <br> */
#define QUEST_MAX_RANK					3	// 랭크의 수
#define QUEST_MAX_RANK_REQUIRE			2	// 랭크를 결정하는 조건의 수
#define QUEST_MAX_REWARD				3	// 랭크에서 선택할수있는 보상의 수
#define QUEST_MAX_DIVIDE_ITEM			5	// 완료시 삭제될 아이템 한종류가 분할될수있는 최대수 -- 검사규칙:1.슬롯별 스택수가 삭제되어야할 스택수를 넘는것이 있는지 검사. 2.앞에서 다섯개의 슬롯의 아이템의 스택수의 합이 삭제되어야할 스택수를 넘는지 검사. 나머지 경우는 Mission failed...
#define MAX_ITEM_UPGRADE_MATERIALS		4

#define QUESTREWARD_MAX_EFFECTID		4	// 보상 이펙트의 수
#define QUESTREWARD_MAX_ITEM			6	// 보상 아이템의 수

typedef enum _enumQUEST_ACCEPTTYPE	// 수락타입
{
	QUEST_ACCEPTTYPE_NPC = 1,
	QUEST_ACCEPTTYPE_SHARE = 2,
	QUEST_ACCEPTTYPE_ITEM = 4,
	QUEST_ACCEPTTYPE_EVENT = 8,

} QUEST_ACCEPTTYPE;

typedef enum _enumREWARD_TYPE	// 보상타입
{
	REWARD_TYPE_FIX = 1,
	REWARD_TYPE_RANDOM,
	REWARD_TYPE_SELECT,

} REWARD_TYPE;

typedef enum _enumQUEST_STATUS
{
	QUEST_STATUS_ACTIVE = 0,
	QUEST_STATUS_FINISH = 1,
	QUEST_STATUS_MAX	= 2,		//[2006/4/3] - jinhee

} QUEST_STATUS;

typedef enum _enumMISSION_TYPE
{
	MISSION_TYPE_NULL = 0,			//[2005/9/21] - jinhee
	MISSION_TYPE_MONSTER = 1,
	MISSION_TYPE_EVENT,
	MISSION_TYPE_ITEM,
	MISSION_TYPE_ACTION,

} MISSION_TYPE;

typedef struct _tagQUEST
{
	UINT32		uiQuestID;		// 퀘스트아이디
	int			iFinishCnt;		// 완료한 횟수
	UINT8		uiStatus;		// 0:완료, 1:수행중

} QUEST, *PQUEST;

typedef struct _FESTIVAL
{
	int		iAppliedLoginEvent;
	time_t	tmLastLoginTime;
} FESTIVAL, *PFESTIVAL;

typedef struct _tagMISSION
{
	UINT8		uiMissionType;	// 미션타입(0:NULL, 1:Monster, 2:Event, 3:Item, 4:Action)
	UINT32		uiValueID;		// 미션의 대상의 아이디(미션타입이 Monster일때는 MonsterID)
	int			iFinishValue;	// 완료값
	int			iCurValue;		// 현재값

} MISSION, *PMISSION;

#define CASHSHOP_ERROR_NOT_OPENED 9009006 //9000004 // 캐쉬샵을 임시로 닫은 경우
#define CASHSHOP_ERROR_FAIL 4000020   // osp 에서 돈까지는 뺏는데 그이후 행위가 실패인 경우.
#define DEFAULT_ERROR_ID		4000002		// 에러문구를 보여줄 필요가 없거나 포괄적으로 보여주고 싶을때 사용하는 글로벌스트링의 에러아이디입니다. 내용은 지금은 이 작업을 할수없다는 내용정도일것으로 예상됩니다.(유명상 팀장님이 정할것임)
#define DEFAULT_ERROR_ID_TARGET	4000003		// 에러문구를 보여줄 필요가 없거나 포괄적으로 보여주고 싶을때 사용하는 글로벌스트링의 에러아이디입니다. 내용은 지금은 상대방이 이 작업을 할수없다는 내용정도일것으로 예상됩니다.(유명상 팀장님이 정할것임)
#define DEFAULT_ERROR_USERIN	30003		// 중복로그인검사시 에러.
#define DEFAULT_ERROR_FAIL_BY_BATTLE	4008001 // 배틀일때 못함.// 캐쉬샵 채널이동

// Auction 서버용 에러
//
//물품이 존재하지 않을 때 	확인	52000009
//페이지가 존재하지 않을 때 	확인	52000074
//상회 입찰가로 인해서 입찰 가격이 맞지 않을 때 	확인	52000073
//입찰 중 다른 유저에 의해서 입찰에 방해되는 행위가 벌어질 때(등록 취소, 상회 입찰, 즉시 구매)	확인	52000080

#define AUCTION_ERROR_NOT_OPENED 52000075  // 경매서버가 오픈되지 않은 경우
#define AUCTION_ERROR_BLOCKED	52000080	// 경매물건이 블럭된경우
#define AUCTION_ERROR_BIDPRICE  52000073 // 입찰가가 맞지 않을때
#define AUCTION_ERROR_NOT_EXIST 52000009 // 경매물건 팔렸거나 취소되서 없어진 경우
#define AUCTION_ERROR_EXPIRED   52000108 // 경매물건이 마감된경우(입찰/즉구/취소) 불가능
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Team

#define TEAM_MAX_MEMBER		4
#define TEAM_MIN_MEMBER		2

enum
{
	TEAM_GRADE_NONE = 0,	// 팀원
	TEAM_GRADE_CAPTAIN,		// 팀장
	TEAM_GRADE_MAX,			// MAX
};

enum
{
	TEAM_LOOT_ORDER = 0,	// 차례대로
	TEAM_LOOT_RANDOM,		// 랜덤
	TEAM_LOOT_FREE,			// 자유
	TEAM_LOOT_MAX,			// MAX
};

typedef struct _tagTEAM_MEMBER
{
	GU_ID	CharGUID;
	UINT8	byGrade;	// 등급(0:팀원, 1:팀장)
	short	sLv;
	UINT8	byClass;
	char	szCharName[LEN_NAME+1];
	UINT32	uiMaxHP;
	UINT32	uiHP;
	UINT32	uiMaxSP;
	UINT32	uiSP;
	UINT16	usStageID;
	UINT16	usMapGroupID;
	float	fPosX;
	float	fPosY;
	UINT8	byWhere;	// 0:일반적인장소, 1:캐쉬샵
#if defined(__ADD_WORLDTEAM) || ( !defined(_CHANNEL_TEAM_VERSION_) && defined(_LATALE_CLIENT) )
	INT32	iCharID;	// 캐릭터 DB키
	UINT16	wServerID;	// 서버아이디
#endif

} TEAM_MEMBER, *PTEAM_MEMBER;

#define TEAM_DIST_X		800.0f
#define TEAM_DIST_Y		600.0f

// Team
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Title

#define TITLE_ID_NOTUSE					1	// 사용안함을 대신하는 키값
#define TITLE_MAX_NAME					64	// LDT에 설정된 txt의 크기기준임
#define TITLE_MAX_STATUS				4
#define TITLE_MAX_EFFECTID				4

// Title
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// FreeMarket

#define FM_MAX_NAME			24
#define FM_MIN_NAME			2
#define FM_MIN_PRICE		1
#define FM_MAX_SELLITEM		16
#define FM_BASE_SELLITEM	4
#define FM_OTHER_DISTANCE_X	100.0f
#define FM_OTHER_DISTANCE_Y	100.0f

typedef struct _tagFM_ITEM
{
	UINT8			bySlot;
	ITEMID			uiItemID;
	UINT8			byStackCount;
	UINT64			uiPrice;

} FM_ITEM, *PFM_ITEM;

typedef struct _tagFM_SELLITEM
{
	FM_ITEM			stFM_Item;
	CONTAINER_SLOT	stInvenSlotInfo;

} FM_SELLITEM, *PFM_SELLITEM;

typedef struct _tagFM_VIEWITEM
{
	FM_ITEM			stFM_Item;
	ITEM			stItem;						// 아이템에 대한 상세정보를 보내줘서 마우스를 아이템에 위치시키면 정보를 보여줄수있도록함

} FM_VIEWITEM, *PFM_VIEWITEM;

// FreeMarket
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// CashShop

#define CASHSHOP_MAX_WISH				6

// CashShop
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// WayPoint

#define WAYPOINT_SIZE_BIT					128	// 비트로 구분할 데이터의 수 *** PacketID.h로 이사예정
#define WAYPOINT_SIZE_DATA					4	// 비트를 저장하기 위한 long형 데이터의 수 *** PacketID.h로 이사예정
#define WAYPOINT_MAX_NAME					64	// LDT에 설정된 txt의 크기기준임
#define WAYPOINT_MAX_RANK					2	// 랭크의 수
#define WAYPOINT_MAX_RANK_REQUIRE			2	// 랭크를 결정하는 조건의 수
#define WAYPOINT_MAX_DIVIDE_ITEM			5	// 완료시 삭제될 아이템 한종류가 분할될수있는 최대수 -- 검사규칙:1.슬롯별 스택수가 삭제되어야할 스택수를 넘는것이 있는지 검사. 2.앞에서 다섯개의 슬롯의 아이템의 스택수의 합이 삭제되어야할 스택수를 넘는지 검사. 나머지 경우는 Mission failed...
#define WAYPOINT_MAX_FIND_EFFECTID			2	// 첫발견시 이펙트 수
#define WAYPOINT_MAX_CONSUMPTION_ITEM		1	// 삭제아이템의 최대수

#define DISTANCE_DEFAULT_X				800.0f	// 일반적인 거리검사시 사용할것임
#define DISTANCE_DEFAULT_Y				600.0f	// 일반적인 거리검사시 사용할것임

// WayPoint
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Event Mission

#define MAX_EVENT_MISSION_BIT			1024	// 비트로 구분할 데이터의 수 *** PacketID.h로 이사예정
#define MAX_EVENT_MISSION_REQUIRES		8
#define MAX_EVENT_MISSION_EFFECT		4
#define	MAX_EVENT_MISSION_ITEM			2

const int MAX_EVENT_MISSION_DATA		= 32;// ( MAX_EVENT_MISSION_BIT / 32);	// 비트를 저장하기 위한 long형 데이터의 수 *** PacketID.h로 이사예정
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 클래스 변경

#define MAX_CLASS_CHANGE_REQUIRES		8
#define	MAX_CLASS_CHANGE_ITEM			4

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ItemAddTime

#define ITEMADDTIME_MAX_REQUIRE				4			// 조건의 수
#define ITEMADDTIME_MAX_ADDTIME				2100000000	// 시간제아이템이 가질수있는 시스템적 최대값 약 66년

// ItemAddTime
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 먹이주기

#define ITEM_MAX_ITEMSP						100			// 아이템 최대 SP

// ItemAddTime
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Gameble

#define MAX_GAMBLE_REWARD			30
#define MAX_GAMBLE_BETTING		3   
#define MAX_GAMBLE_MATERIALS		2
#define MAX_GAMBLE_REWARD_BIT		MAX_GAMBLE_REWARD	// 비트로 구분할 데이터의 수 
//const int MAX_GAMBLE_REWARD_DATA	= 1;//( MAX_GAMBLE_REWARD_BIT / 32);	// 비트를 저장하기 위한 long형 데이터의 수 *** PacketID.h로 이사예정
//------------------------------------------------------------------------------

// Auction
// 경매거래타입
enum AUCTION_TYPE
{
	AUCTION_TYPE_NULL,
	AUCTION_TYPE_ITEM_BY_ELY,	// 판매 : 아이템,	구매수단 : Ely
	AUCTION_TYPE_CASH_BY_ELY,	// 판매 : Cash,		구매수단 : Ely
	AUCTION_TYPE_MILEAGE_BY_ELY,// 판매 : Mileage,	구매수단 : Ely
	AUCTION_TYPE_ELY_BY_CASH,	// 판매 : Ely,		구매수단 : Cash
	AUCTION_TYPE_ELY_BY_MILEAGE,// 판매 : Ely,		구매수단 : Mileage
	AUCTION_TYPE_MAX,
};

//------------------------------------------------------------------------------
#define MAX_MAIl_BODY_LEN MAILBOX_MAX_MEMO_LEN 

// SYSTEM MAIL TYPE
// 시스템메일인경우 타이틀이 달라진다면
enum MAIL_TYPE
{
	MAIL_TYPE_NULL	,									// 사용자 메일
	MAIL_TYPE_AUCTION_BASE		= 50,					// 경매
	MAIL_TYPE_AUCTION_SELL		= MAIL_TYPE_AUCTION_BASE +1,	// 판매완료 : 등록자가 받을때
	MAIL_TYPE_AUCTION_BUY,								// 낙찰(즉구포함): 입찰자가받을때
	MAIL_TYPE_AUCTION_OUTBID,							// 상회입찰 : 기입찰자가의 입찰금을 돌려받을때
	MAIL_TYPE_AUCTION_CANCEL_BIDDER,							// 경매취소 : 등록자가 경매물건을 취소시(등록자/입찰자)
	MAIL_TYPE_AUCTION_CANCEL_SELLER,
	MAIL_TYPE_AUCTION_EXPIRED,							// 유찰 : 등록자가 올린 물건의 기간이 지났을때
};			   

#define AUCTION_MAIL_DELIMETER "_"						// 경매메일에서 내용 구분자
//////////////////////////////////////////////////////////////////////
// MailBox

// Client <-> Server
typedef struct _MAIL_HEADER
{
	UINT64				uiMailID;				// Mail index
	TIMESTAMP_STRUCT	tmSendTime;				// 전송 시각
	TCHAR				szSender[LEN_NAME+1];	// 송신자
	UINT8				iType;					// MAIL_TYPE 
}MAIL_HEADER,*PMAIL_HEADER;

typedef struct _MAIL_ATTACHMENT
{
	UINT64	uiEly;					// 첨부할 ely amount
	UINT32	uiItemID;				// 첨부된 Item Type (tbluItem2::TypeID)
	UINT64	uiItemNo;				// 첨부된 Item NO (tbluItem2::ItemNo)
	UINT8	uiItemCount;			// 첨부된 Item 갯수 (tbluItem2::cnt)
	UINT8	uiContainerType;		// 첨부된 Item의 인벤토리 타입
	UINT8	uiContainerSlot;		// 첨부된 Item의 인벤토리내의 위치
	UINT8	uiReadFlag;				// IsNewMail?  읽지 않은 새로운 메일인지를 표현함 (tbluMailBox::ReadFlag)
}MAIL_ATTACHMENT,*PMAIL_ATTACHMENT;

// 메일 송신 정보 + 제목 + 첨부 정보
typedef struct _MAIL_HEADER_ATTACHMENT: public MAIL_ATTACHMENT, public MAIL_HEADER
{
}MAIL_HEADER_ATTACHMENT,*PMAIL_HEADER_ATTACHMENT;


//////////////////////////////////////////////////////////////////////
// Server <-> DB : 서버 내부용 구조체
	typedef struct _MAIL_HEADER_ATTACHMENT_SS: public MAIL_ATTACHMENT, public MAIL_HEADER
	{
		TCHAR	szSubject[MAILBOX_MAX_SUBJECT_LEN+1];				// 제목 : MAIL_HEADER::iType > MAIL_TYPE_AUCTION_BASE 인경우는 "\0"  
	}MAIL_HEADER_ATTACHMENT_SS,*PMAIL_HEADER_ATTACHMENT_SS;

	typedef struct _MAIL_INFO_SS: public MAIL_HEADER_ATTACHMENT_SS
	{
		TCHAR	szReceiver[LEN_NAME+1];
		USERID	iReceiverUserID;
		CHARID	iReceiverCharID;
		USERID	iSenderUserID;
		CHARID	iSenderCharID;

		// MAIL_HEADER::iType > MAIL_TYPE_AUCTION_BASE 인경우 
		// "AUCTION-TYPE_원금_수수료_최종가_ITEMID_VALUE" 를 "%d_%d_%d_%d_%d_%d" 포맷로 전달하며 금액과 관련이 없는 아이템 관련물건이어도 포맷은 맞추어 보낸다
		// ITEMID 는 판매완료시 판매한 물건이 아이템인 경우 
		// VALUE는 AUCTION-TYPE 따라 ely / mile / cash 일수 있음
		TCHAR	szMemo[MAILBOX_MAX_MEMO_LEN+1];						
		CONTAINER_SLOT StampItemPos;	// 추가
		ITEMNO		   iStampItemNo;	// 추가
	}MAIL_INFO_SS,*PMAIL_INFO_SS;


//////////////////////////////////////////////////////////////////////

// 추가
typedef struct _CASHSHOP_INFO
{
	int  iPackageID;
	bool bEnable;
	UINT16 iPositionIndex;
	UINT8 iEventPage;
	UINT8 iEventIcon;
	int	 iCashPrice; // 
	int	 iSalePrice;
	int  iMaxOz; // 
	int  iMaxMileage;
} CASHSHOP_INFO, *PCASHSHOP_INFO;	


// 20080411 mijin 추천상품 
typedef struct _CASHSHOP_HOT_INFO
{
	int  	iShopID;
	UINT8 	iDescLength;
	TCHAR	strDesc[50];

} CASHSHOP_HOT_INFO, *PCASHSHOP_HOT_INFO;

//	jmulro 우醵뭔찜，2008－8－26
typedef struct _Quick_Buy_Info
{
	int  m_iPackageID;			//	�犬풩횵D
	int  m_nLevel;				//	鯤소돨된섬
	int  m_nOrder;				//	겠렴돨貫零
}t_Quick_Buy_Info;

//////////////////////////////////////////////////////////////////////
// Lottery

#define MAX_LOTTERY_INT	  50
#define	MAX_LOTTERY_COUNT (50*32)
#define MAX_LOTTERY_RANK 10

typedef struct _LOTTERY_REWARD
{
	UINT16 iRewardID;
	UINT16 iRewardCount;

} LOTTERY_REWARD, *PLOTTERY_REWARD;

//////////////////////////////////////////////////////////////////////
// PC의 조합정보
typedef struct _ITEMMIX_INFO
{
	UINT8	iLv;		//  조합레벨
	int		iCurExp;	//  조합경험치 현재량
	int		iMinExp;	//  조합경험치 시작
	int		iMaxExp;	//  조합경험치 끝
	UINT8	iRareProb;  //  조합시 레어도 결정확률팩터
	UINT8   iSuccessProb;  // 조합성공확률팩터
	UINT8	iOptProb;	// 조합시 옵션포인트 결정확률 팩터
}ITEMMIX_INFO;

//////////////////////////////////////////////////////////////////////
// PC의 pvp 정보
typedef struct _PVP_STATUS
{
	UINT8		iLv;		//  PVP 명예레벨
	__int64		iGlory;		//  명예 현재량
	int			iPoint;		//	PVP 거래용 포인트
}PVP_STATUS;

//////////////////////////////////////////////////////////////////////
// 스킬관련
typedef struct _CHAR_SKILL
{
	int				iSkillID;	// SKILL.LDT::ItemID
	short			iSkillLv;	// PC가 포인트를 찍어서 올린 고유값.
	short			iSkillCurrLv;// PC가 아이템 장착/스킬/버프 등을 통해서 올린 임시적인 값까지 계산된 최종값.
	UINT16			iSkillNo;	// 스킬 오더링용
} CHAR_SKILL, *PCHAR_SKILL;

enum SKILL_SAVE_TYPE
{
	SKILL_SAVE_NULL,	// 일반스킬 초기화시 삭제
	SKILL_SAVE_GENERAL, // 일반스킬 초기화시 남겨둠
	SKILL_SAVE_ITEMMIX,	// 조합스킬 초기화시 삭제
};

//////////////////////////////////////////////////////////////////////
// 경매

// OSP 거래타입 20080325 mijin
enum OSP_TRADE
{
	TRADE_TYPE_USER = 1,		// 유저간 직거래
	TRADE_AUCTION_REGISTER,			// 경매등록
	TRADE_AUCTION_CASH_BUY,			// 캐쉬(마일리지)로 즉시구매
	TRADE_AUCTION_ELY_BUY,			// 아이템(게임머니)로 즉시구매
	TRADE_AUCTION_TENDER,			// 경매입찰
	TRADE_AUCTION_ACCEPT,			// 경매낙찰
	TRADE_AUCTION_CANCEL = 9,			// 경매취소
};

// 경매기간 타입
enum AUCTION_DURATION
{
	PERIOD_SHORT,		// 단기		(6)
	PERIOD_LONG,		// 장기		(12)
	PERIOD_EXTREME,		// 최장기	(24)
};

// 아이템경매소트키
// 내림차순:0, 오름차순:1
enum AUCTION_SORT_KEY
{
	AUCTION_SORT_NULL,
	AUCTION_SORT_QUALITY,	// 품질
	AUCTION_SORT_ITEM_LV,		// 레벨
	AUCTION_SORT_ITEM_OPTPOINT,	// 내구도
	AUCTION_SORT_DURATION,	// 마감시간
	AUCTION_SORT_PRICE,			// 입찰가(ely,cash,mile)
	AUCTION_SORT_MAX,
};

// 경매기본정보
typedef struct _AUCTION_HEADER
{
	__int64 iAuctionID;				// 경매키
	UINT8	iType;					// AUCTION_TYPE (경매타입)
	UINT8	iDuration;				// AUCTION_DURATION (판매자가 세팅한)
	USERID	iSellerID;				// 판매가 계정키
	TCHAR	szSeller[LEN_NAME+1];	// 판매자 캐릭터
	UINT32	iRemainTime;			// 현재남은시간

	__int64 iBidPrice;				// 최초입찰가 혹은 상회입찰가
	__int64 iBuyOutPrice;			// 즉구가
	__int64 iCurrPrice;				// 현재가
	USERID  iCurrUserID;			// 현재입찰자계정키
	TCHAR	szBidder[LEN_NAME+1];	// 최고입찰자
} AUCTION_HEADER, *PAUCTION_HEADER;

// 경매거래타입별추가데이타
typedef struct _AUCTION_GOODS_ITEM 
{
	ITEM stAuctionItem;
} AUCTION_GOODS_ITEM, *PAUCTION_GOODS_ITEM;

typedef struct _AUCTION_GOODS_POINT 
{
	int SellPoint; // Ely, Cash, Mileage
	int FeePoint;  // 수수료
} AUCTION_GOODS_POINT, *PAUCTION_GOODS_POINT;

typedef struct _PetAttr_Info
{
	UINT8	uPetStatus;
	UINT16	uStayServerID;
	char	szPetName[LEN_NAME];
	UINT16	lv;
	UINT16	hp;
	UINT16	sp;
	UINT32	exp;
	int  	ssp;
} t_PetAttr_Info;

typedef struct _Pet_Skill_To_Client
{
	int nSkillID;
	int nLevel;
}t_Pet_Skill_To_Client;


//////////////////////////////////////////////////////////////////////
// PC의 결혼정보

enum WEDDING_PC_STATUS
{
	WEDDING_STATUS_NONE =		0,			// 0
	WEDDING_STATUS_LOVE =		1 << 0,		// 1 교제상태
	WEDDING_STATUS_PROPOSE =	1 << 1,		// 2 프러포즈상태
	WEDDING_STATUS_WEDDING =	1 << 2,		// 4 결혼상태
	//WEDDING_STATUS_PARTING =	1 << 3,		// 8 이별상태 이별을 했을때 WEDDING_STATUS_NONE으로 셋팅하기로함
};

enum WEDDING_TYPE
{
	WEDDING_TYPE_NONE =		0,			// 0
	WEDDING_TYPE_LOVE,				//교제
	WEDDING_TYPE_PROPOSE,			//프로포즈
	WEDDING_TYPE_WEDDING			//결혼
};

typedef struct _WEDDING_INFO
{
	UINT8				iLv;							//  Wedding Lv
	int					iLP;							//  Love Point
	int					iLPM;							//	Love Point Max
	int					iDay;							//	교제일수 또는 결혼일수
	UINT8				iStatus;						//	현재상태
	int					iPartingNum;					//	이별지난시간(초단위)
	TCHAR				szPartName[LEN_NAME+1];			//	상대방 이름

}WEDDING_INFO;


#pragma pack(pop)
