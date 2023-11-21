	// GlobalDefines_Share.h

/*

	Client / Server 상의 공유 Define 에 대한 모음 용도
	(Packet 에 무관한 모든 Share_Defines 모음)

*/

#pragma once


#define ITEMSKILL_SELL_PERCENT	20 // 2007.09.10 dhpark 15 -> 20 펫스킬에 의해서 아이템판매를 했을때 판매가 비율입니다. 기획팀정책변경에 의해서 변경함. 유명상팀장

// ID/PW/NAME Length definition
//{
	//extern int g_LEN_ID/* = LEN_ID*/;
	//extern int g_LEN_PW/* = LEN_PW*/;

	//extern int g_MIN_LEN_ID/* = MIN_LEN_ID*/;
	//extern int g_MIN_LEN_PW/* = MIN_LEN_PW*/;

	//extern bool SetLength_Init(TCHAR* pszIniFile);

	//extern bool SetLength_ID(int iIDLen);
	//extern bool SetLength_PW(int iPWLen);
	//extern bool SetLength_MinID(int iMinIDLen);
	//extern bool SetLength_MinPW(int iMinPWLen);
	//extern bool CheckLength_IDPW();

//}


enum SITE_TYPE {
	SITE_LOCAL,
	SITE_ACTOZ,
	SITE_GAMEPOT,
	SITE_SANDA,
	SITE_BUDDYBUDDY, 
	SITE_ATGAMES, 
	SITE_OGP,		//  [4/28/2008] append by metalgeni 
	MAX_SITES};


// 1(6,without challenge),2(not used),3(8,with challenge)
enum {EKEYTYPE_NONE,EKEYTYPE_WITHOUT_CHALLENGE_6,EKEYTYPE_WITHOUT_CHALLENGE_8,EKEYTYPE_WITH_CHALLENGE_8,MAX_EKEYTYPE};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 리소스 버전 체크 용도 (네크워크 패킷 송수신용도로는 사용하지 말것)
//{
	#define MAX_RESOURCE_FILE_COUNT		50

	#pragma pack(push,8)

	typedef struct _RES_VERSION_INFO
	{
		int iVersion;
		TCHAR szFile[MAX_PATH];

	}RES_VERSION_INFO;

	#pragma pack(pop)

//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 팩을 해서 얼라인을 맞춥니다. 팩과 언팩안에서 구조체 정의하세요.
#pragma pack(push,1)

//------------------------------------------------------------------------------
// 길드
//#define _GUILD_FUNC_								0
#define GUILD_MAX_NAME								16					// 길드명 최대길이(DB는 20으로 잡을 예정임)
#define GUILD_MAX_SLOGAN							100					// 길드슬로건 최대길이
#define GUILD_MAX_MEMBERLIST_PERPACKET				100					// 패킷당 멤버의 수(멤버구조체의 크기가 커지면 변경필요함)
#define GUILD_MIN_MEMBERCNT							1					// 길드를 구성하는 최소인원(길마만 남은상태)
#define GUILD_MAX_MEMBERCNT							200					// 시스템제한 최대길드원수
// 길드인포
#define GUILD_MAX_REQUIRE_CREATE					5					// 생성(랭크업)시 조건검사수
#define GUILD_MAX_CONSUMPTION_ITEM_CREATE			1					// 길드생성시 소모되는 아이템 최대수
#define GUILD_MAX_REQUIRE_LOAN						5					// 길드방 대여시 조건검사수
// 크롭인포
#define CROP_MAX_REQUIRE_CREATE						3					// 생성시 조건검사수
#define CROP_MAX_CONSUMPTION_ITEM_CREATE			1					// 생성시 소모되는 아이템 최대수
#define CROP_MAX_EVENTID_CREATE						2					// 생성시 생성해야하는 이벤트 최대수
// 디비젼인포
#define DIVISION_MAX_EVENTID						10					// 길드인던포탈이벤트 최대수
// 비료인포
#define NOURISH_MAX_CONSUMPTION_ITEM				1					// 소모되는 아이템 최대수
// 길드포인트최대값
#define GUILD_MAX_POINT								2000000000			// 길드포인트 최대값(개인/길드 동일)
// 길드랭크최대값
#define GUILD_MAX_RANK								10					// 길드최대랭크(2007.08.14 dhpark 랭크리콰검사를 범위로 변경하기 위해서 임의로 설정)
// 길드랭크별 최대 스탯수
#define GUILD_MAX_STATUS							4					// 길드랭크별 최대 스탯수
// 길드마크의 기본값
#define GUILD_DEFAULT_MARK1							0					// 길드마크1의 기본값(심볼)
#define GUILD_DEFAULT_MARK2							0					// 길드마크2의 기본값(배경)
#define GUILD_DEFAULT_MARK3							0					// 길드마크3의 기본값(효과). 기본효과는 이펙트가 없는것으로 한다고 함.
// 길드마크인포
#define GUILDMARK_MAX_REQUIRE						3					// 길드마크변경시 제한조건 최대수


// 직책
enum { GUILD_GRADE_NONE, GUILD_GRADE_MASTER, GUILD_GRADE_NO2, GUILD_GRADE_NO3, GUILD_GRADE_NO4, GUILD_GRADE_NO5, GUILD_GRADE_NO6,  };
// 권한(0:작물관리, 1:직위변경, 2:슬로건변경, 3:강퇴, 4:가입권유, 5~9:예약, 10:권한최대수)
enum { GUILD_AUTHORITY_CROP, GUILD_AUTHORITY_GRADE, GUILD_AUTHORITY_SLOGAN, GUILD_AUTHORITY_EXPEL, GUILD_AUTHORITY_REQUEST, GUILD_AUTHORITY_RESERVE1, GUILD_AUTHORITY_RESERVE2, GUILD_AUTHORITY_RESERVE3, GUILD_AUTHORITY_RESERVE4, GUILD_AUTHORITY_RESERVE5, GUILD_MAX_AUTHORITY };
// 멤버메세지타입(0:가입, 1:탈퇴, 2:강퇴, 3:직위변동, 4:로그인, 5:로그아웃, 6:위임)
enum { GUILD_MSG_MEMBER_JOIN, GUILD_MSG_MEMBER_SECEDE, GUILD_MSG_MEMBER_EXPEL, GUILD_MSG_MEMBER_GRADE, GUILD_MSG_MEMBER_LOGIN, GUILD_MSG_MEMBER_LOGOUT, GUILD_MSG_MEMBER_CHANGEMASTER, };
// 길드메세지타입(0:슬로건변경, 1:랭크변경, 2:묘목심기, 3:묘목뽑기, 4:묘목성장완료, )
enum { GUILD_MSG_GUILD_SLOGAN, GUILD_MSG_GUILD_RANK, GUILD_MSG_GUILD_PLANT, GUILD_MSG_GUILD_GRUB, GUILD_MSG_GUILD_GROWTHCROP, };
// 비료타입(0:작물아님, 1:비료, 2:촉진제)
enum { BOOSTER_TYPE_NONE, BOOSTER_TYPE_MANURE, BOOSTER_TYPE_HASTEN, };
// 길드타입/리콰
enum { GUILD_TYPE_NONE, GUILD_TYPE_NEUTRALITY, GUILD_TYPE_ANGEL, GUILD_TYPE_DEVIL, GUILD_TYPE_NA, GUILD_TYPE_ND, GUILD_TYPE_AD, GUILD_TYPE_NAD, };
// 길드마크타입(1:아이콘, 2:배경, 3:효과)
enum { GMARK_TYPE_ICON = 1, GMARK_TYPE_BACKGROUND, GMARK_TYPE_EFFECT, };


typedef struct _GUILD_BROADCAST
{
	INT32	iGNum;
	char	szGName[GUILD_MAX_NAME+1];
#if defined(__ADD_GUILD_MARK) || defined(_LATALE_CLIENT)
	UINT8	arrGMark[3];						// 심볼, 배경, 효과
#else
	UINT8	arrGMark[2];						// 심볼, 배경
#endif
	UINT8	byGType;							// 길드유형(1:일반, 2:천사, 3:악마)
	UINT8	byGRank;							// 길드랭크(길드최대인원수에 영향을 미침)

} GUILD_BROADCAST, *PGUILD_BROADCAST;

typedef struct _GUILD_COMMON
{
	char	szGSlogan[GUILD_MAX_SLOGAN+1];		// 길드슬로건
	UINT32	uiCropID;							// 묘목아이디
	UINT32	uiRemainTime;						// 현재시간과 비교하여 계산된 성장완료까지 남은시간(초단위)
	UINT32	uiNourishTime;						// 현재시간과 비교하여 계산된 비료를 먹을 시간(초단위)
	UINT32	uiHastenSumTime;					// 촉진제 누적시간

} GUILD_COMMON, *PGUILD_COMMON;

typedef struct _GUILD_SERVER
{
	INT32				iOriginRemainTime;					// DB에 저장된 성장완료까지 남은시간(초단위)
	INT32				iOriginNourishTime;					// DB에 저장된 비료를 먹을 시간(초단위)
	time_t				tmMemLoadTime;						// 게임서버에서 처음 로드된 시간. 시간 재계산시 사용함.
	TIMESTAMP_STRUCT	stDBLoadTime;						// 로드시 DB의 GETDATE()를 가져옴.
	TIMESTAMP_STRUCT	stNourishStartTime;					// 비료를 먹기 시작한 시각
	INT32				iEventID;							// 묘목인던포탈이벤트아이디
	UINT8				byIndunFinish;						// 인던점령여부(0:점령전, 1:점령완료)

} GUILD_SERVER, *PGUILD_SERVER;

typedef struct _GUILD : public GUILD_BROADCAST, public GUILD_COMMON
{

} GUILD, *PGUILD;

typedef struct _DBGUILD : public GUILD_BROADCAST, public GUILD_COMMON, public GUILD_SERVER
{

} DBGUILD, *PDBGUILD;

typedef struct _PCGUILD : public GUILD_BROADCAST, public GUILD_COMMON
{
	UINT8	byGrade;
#if defined(__ADD_GUILD_RANKUP) || defined(_LATALE_CLIENT)
	INT32	iPoint;				// 길드포인트
#endif

} PCGUILD, *PPCGUILD;

typedef struct _DBPCGUILD : public GUILD_BROADCAST, public GUILD_COMMON, public GUILD_SERVER
{
	UINT8	byGrade;
#if defined(__ADD_GUILD_RANKUP) || defined(_LATALE_CLIENT)
	INT32	iPoint;				// 길드포인트
#endif

} DBPCGUILD, *PDBPCGUILD;

typedef struct _GUILD_MEMBER
{
	INT32	iCharID;			// 캐릭터의 DB키
	INT32	iUserID;			// Account의 DB키(유저정보를 찾기위한값)
	char	szName[LEN_NAME+1];	// 캐릭명
	UINT8	byClass;			// 직업
	UINT16	wLevel;				// 레벨
	UINT16	wServerID;			// 서버아이디(채널정보표시를 위해서)
	UINT8	byGrade;			// 길드내 직책(1:길마, 2:부길마, 3~6:회원)
#if defined(__ADD_GUILD_RANKUP) || defined(_LATALE_CLIENT)
	INT32	iPoint;				// 길드포인트
#endif

} GUILD_MEMBER, *PGUILD_MEMBER;


// 길드
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// 인던

#define INDUN_MAX_PERSERVER						50					// 인던서버당 인던의 최대수
#define INDUN_MAX_REQUIRE_IN					5					// 인던입장조건검사 최대수
#define INDUN_DESTROYTIME_WIHTOUTPC				60*10				// PC가 인던에 아무도 없을때 파괴되는 시간의 최대값(초단위)
#define INDUN_MIN_DESTROYTIME_WIHTOUTPC			1					// PC가 인던에 아무도 없을때 파괴되는 시간의 최소값(초단위)
#define INDUNMISSION_MAX_REQUIRE				2					// 인던미션조건수
#define INDUNMISSION_MAX_MISSION				2					// 인던미션수
#define INDUNMISSION_MAX_REWARD					3					// 인던미션보상수
#define INDUNIN_MAX_PERPACKET					100					// MgmtServer에 접속시 팀정보등록을 위해서 보내는 패킷당 인던입장정보의 최대수
#define INDUN_CHANNEL_MAX						20					// 50 보다 크면 인던 서버
#define INDUN_PVP_CHANNEL_MAX					80					// 80 보다 크면 PVP인던

// 인던타입
enum { INDUN_TYPE_GUILD = 1, INDUN_TYPE_TEAM, INDUN_TYPE_SOLO, INDUN_TYPE_EVENT, INDUN_TYPE_GUILDINDUN, INDUN_TYPE_CHATROOM };
// 인던상태
enum { INDUNIN_STATUS_RESERVE = 0, INDUNIN_STATUS_CREATE, };
// 인던미션타입
enum { INDUNMISSION_TYPE_NONE = 0, INDUNMISSION_TYPE_MOBCNT, INDUNMISSION_TYPE_MAPEVENT, };
// 인던미션보상타입
enum { INDUNMISSION_REWARD_TYPE_NONE = 0, INDUNMISSION_REWARD_TYPE_MAPEVENT, INDUNMISSION_REWARD_TYPE_EFFECT, };

typedef struct _INDUNIN
{
	UINT32	uiID;				// 인던 인스턴스 아이디
	UINT32	uiLDTID;			// 인던 LDT 아이디
	UINT8	byType;				// 인던타입(1:길드인던, 2:솔로인던, 3:팀인던, 4:이벤트인던)
	INT32	iGuildNum;			// 길드 DB키
	UINT32	uiTeamID;			// 팀 아이디
	INT32	iCharID;			// 캐릭터 DB키
	UINT16	wServerID;			// 서버아이디(어떤인던서버인지 구분을 위해서)

} INDUNIN, *PINDUNIN;

typedef struct _tagINDUNMISSION
{
	UINT8	byType;	// 0:NONE, 1:몹카운트, 2:맵이벤트
	UINT32	uiID;
	INT32	iFinishCnt;	// 미션완료카운트
	INT32	iCurCnt;	// 현재카운트(LDT로드/인던미션관리에서 함께 사용)

} INDUNMISSION, *PINDUNMISSION;

// 인던
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// PVP

#define PVP_MAX_PERSERVER						500					// PVP서버당 PVP의 최대수
#define PVP_MAX_REQUIRE_IN						5					// PVP입장조건검사 최대수
#define PVP_DESTROYTIME_WIHTOUTPC				60*3				// PC가 PVP에 아무도 없을때 파괴되는 시간(초단위)
#define PVPIN_MAX_PERPACKET						200					// MgmtServer에 접속시 PVP정보등록을 위해서 보내는 패킷당 PVP입장정보의 최대수
#define PVP_MAX_ROOMUSER						8					// 현재 한곳에 PVP 사용자 수
#define PVP_MAX_STARTPOS						8					// 시작위치의 최대수
#define PVPDIVISION_MAX_PVPINDUN				20					// PVP인던을 랜덤하게 선택하기 위한 데이터의 최대수
#define PVP_TIME_ALARM							30					// 중간점수를 클라이언트에 보내주는 시간간격(초단위)
#define PVP_TIME_PLAY							60*5				// 플레이시간(초단위)
#define PVP_MAX_RANK							8					// 순위의 수(8등까지 있음. PVP_POINT.LDT와 연관있음)
#define	PVP_REVIVAL_DELAY						5					// 죽음 부활대기시간(초단위)
#define PVP_MAX_POINT							2000000000			// PVP포인트의 최대값
#define PVP_MAX_GLORY							999999999999999		// PVP명예의 최대값

// PVP타입
enum { PVP_TYPE_NONE = 0, PVP_TYPE_FDEATH = 1, PVP_TYPE_FTIME, PVP_TYPE_PDEATH, PVP_TYPE_PTIME, PVP_TYPE_GDEATH, PVP_TYPE_GTIME, PVP_TYPE_MAX, };
// PVP규모(중복설정가능하도록 2의배수로 설정)
enum { PVP_SCALE_NONE = 0, PVP_SCALE_2 = 1, PVP_SCALE_4 = 2, PVP_SCALE_6 = 4, PVP_SCALE_8 = 8, PVP_SCALE_MAX, };
// PVP규모타입(LDT에서 입력되는 값)
enum { PVP_SCALETYPE_NONE = 0, PVP_SCALETYPE_2, PVP_SCALETYPE_4, PVP_SCALETYPE_6, PVP_SCALETYPE_8, PVP_SCALETYPE_MAX, };
// 매치최소인원
enum { PVP_MIN_MEMBERCNT_FTIME_2 = 2, PVP_MIN_MEMBERCNT_FTIME_4 = 4, PVP_MIN_MEMBERCNT_FTIME_6 = 6, PVP_MIN_MEMBERCNT_FTIME_8 = 8, };
// PVPIN상태
enum { PVPIN_STATUS_RESERVE = 0, PVPIN_STATUS_CREATE, };
// PVPLOADSTATUS
enum { PVPLOADSTATUS_STATUS_NOTLOAD = 0, PVPLOADSTATUS_STATUS_LOAD, PVPLOADSTATUS_STATUS_DISCONNECT, };
// PVPSTATUS(PVP인던의 상태)
enum { PVP_STATUS_NONE = 0, PVP_STATUS_PLAY, PVP_STATUS_FINISH, };

typedef struct _PVPIN
{
	UINT32	uiID;				// PVP 인스턴스 아이디
	UINT32	uiLDTID;			// PVP LDT 아이디
	UINT8	byType;				// PVP타입(1:개별킬수, 2:개별시간제, 3:팀킬수, 4:팀시간제, 5:길드킬수, 6:길드시간제)
	UINT32	uiScale;			// 몇인용인지(1:2인용, 2:4인용, 4:6인용, 8:8인용)
	UINT16	wServerID;			// 서버아이디(어떤PVP서버인지 구분을 위해서)

} PVPIN, *PPVPIN;

typedef struct _PVPWAIT
{
	INT32			iCharID;				// 캐릭터의 DB키
	INT32			iUserID;				// Account의 DB키(유저정보를 찾기위한값)
	UINT16			wWorldID;				// 월드아이디(월드통합PVP로의 변신을 대비함)
	UINT16			wServerID;				// 서버아이디
	INSTANCE_ID		PCIID;					// InstanceID
	char			szCharName[LEN_NAME+1];	// 캐릭명
	UINT16			wLv;					// 레벨

} PVPWAIT, *PPVPWAIT;

typedef struct _PVPREGIST
{
	PVPWAIT			stPVPWait;				// 대기자정보
	UINT8			byType;					// PVP타입(1:개별킬수, 2:개별시간제, 3:팀킬수, 4:팀시간제, 5:길드킬수, 6:길드시간제)
	UINT32			uiScale;				// 몇인용인지(1:2인용, 2:4인용, 4:6인용, 8:8인용)

} PVPREGIST, *PPVPREGIST;

typedef struct _PVPLOADSTATUS
{
	UINT32			uiTeamID;				// 팀아이디(팀전인경우 팀아이디, 길드전인 경우 길드DB키, 0인 경우는 개인전)
	char			szCharName[LEN_NAME+1];	// 캐릭명
	UINT8			byStatus;				// (0:로드전, 1:로드완료)

} PVPLOADSTATUS, *PPVPLOADSTATUS;

typedef struct _PVPSTATUS
{
	INT32			iCharID;				// 캐릭터 DB키
	UINT32			uiTeamID;				// 팀아이디(팀전인경우 팀아이디, 길드전인 경우 길드DB키, 0인 경우는 개인전)
	char			szCharName[LEN_NAME+1];	// 캐릭명
	UINT8			byStatus;				// (0:로드전, 1:로드완료)
	INT32			iPosIndex;				// 위치정보
	INT32			iDamage;				// 데미지
	INT32			iKillCnt;				// 죽인수
	INT32			iDeathCnt;				// 죽은수

} PVPSTATUS, *PPVPSTATUS;

typedef struct _PVPRANK
{
	INT32			iCharID;				// 캐릭터 DB키
	INT32			iRank;					// 순위
	char			szCharName[LEN_NAME+1];	// 캐릭명
	INT32			iKillCnt;				// 죽인수
	INT32			iDeathCnt;				// 죽은수
	INT32			iScore;					// 순위를 결정하기위한 값(데미지값 * ((죽인수*2)-죽은수)) 2007.11.25 dhpark 현재의 공식
	INT32			iPVPPoint;				// 이번경기에서 획득한 PVP포인트

} PVPRANK, *PPVPRANK;

// PVP
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 대화방
#ifndef MAX_BUF_LEN
#define MAX_BUF_LEN		4096
#endif
#define MIN_LEN_ROOOMTITLE	4
#define MAX_ROOM_NUMBER 999
// 대화방 타입 분류
// enum { CHATROOM_TYPE_GENERAL = 0, CHATROOM_TYPE_MARKET, CHATROOM_TYPE_OFFERJOB, CHATROOM_TYPE_MEETING, MAX_CHATROOM_TYPE, };
#define MAX_CHATROOM_TYPE 4
// 대화방 성별 분류
// enum { CHATROOM_SEX_ALL = 0, CHATROOM_SEX_MAN, CHATROOM_SEX_WOMAN, MAX_CHATROOM_SEX, };
#define MAX_CHATROOM_SEX 3
// 대화방 연령대 분류
//enum { CHATROOM_AGE_ALL = 0, CHATROOM_AGE_ELEMENTARY, CHATROOM_AGE_MIDDLE, CHATROOM_AGE_HIGH, MAX_CHATROOM_AGE, };
#define MAX_CHATROOM_AGE 4
// 대화방 카테고리 분류
enum { /*CHATROOM_CATEGORY_NONE = 0, */CHATROOM_CATEGORY_1 = 0, CHATROOM_CATEGORY_2, CHATROOM_CATEGORY_3, CHATROOM_CATEGORY_4, MAX_CHATROOM_CATEGORY, };
// Indun LDT 에 저장된 대화방 타입
enum { CHATROOM_CATEGORY_NORMAL = 1000, CHATROOM_CATEGORY_MARKET, CHATROOM_CATEGORY_TAKE, CHATROOM_CATEGORY_MEET, };
// CHATROOM_SC_BROADCAST_UNITED_MESSAGE 에서 사용될 Flag 의 열거형
enum { CHATROOM_UNITED_MSG_JOIN = 0, CHATROOM_UNITED_MSG_OUT, CHATROOM_UNITED_MSG_FORCEOUT, CHATROOM_UNITED_MSG_CHANGEMASTER, MAX_CHATROOM_UNITED_MSG, };
// 대화방 오브젝트 이미지 사이즈
#define CHATROOM_LEFT_SIZE		-128
#define CHATROOM_TOP_SIZE		-227
#define CHATROOM_RIGHT_SIZE		128
#define CHATROOM_BOTTOM_SIZE	0

typedef struct _CHATROOM_INFO
{
	TCHAR			szTitle[MAX_CHATROOM_TITLE_LEN+1];			// 대화방 제목
	UINT8			iCategory;									// 대화방 타입(0:일반, 1:장터, 2:구인, 3:미팅)
	UINT8			iSex;										// 성별
	UINT8			iAge;										// 연령 대
	UINT8			iLimitNum;									// 최대 인원 수
	TCHAR			szPassword[MAX_CHATROOM_PASSWORD_LEN+1];	// 비밀번호

	UINT32			uiID;										// 대화방 인스턴스 아이디 (Unique Key)
	UINT32			uiRoomNo;									// 대화방 번호
	UINT32			iMasterCharID;								// 방장 캐릭터 DB키
	TCHAR			szMasterName[LEN_NAME+1];					// 대화방 방장의 캐릭터 이름.
	UINT8			iCurrentNum;								// 현재 인원 수
} CHATROOM_INFO, *PCHATROOM_INFO;

typedef struct _ROOM_MEMBER 
{
	//UINT8		uiMaster;
	CLASS_TYPE	eClassType;
	UINT32		uiCharID;
	TCHAR		szCharName[LEN_NAME+1];
}ROOM_MEMBER, *PROOM_MEMBER;

typedef struct _BAN_MEMBER 
{
	UINT8		uiCount;
	UINT32		uiCharID;
	//TCHAR		szCharName[LEN_NAME + 1];
}BAN_MEMBER, *PBAN_MEMBER;

typedef struct _CHATROOM_OBJECT
{
	UINT32		uiIndunID;
	UINT8		iCategory;
	SERVER_ID	ServerID;
	SERVER_ID	IndunServerID;
	GU_ID		uiObjectID;
} CHATROOM_OBJECT, *PCHATROOM_OBJECT;

typedef struct _CHATROOM_OBJECT_INFO
{
	UINT32			uiID;										// 대화방 인스턴스 아이디 (Unique Key)
	UINT32			uiRoomNo;									// 대화방 번호
	UINT8			iPassword;									// 공개/비공개 (패스워드 사용여부)
	UINT8			iCategory;									// 대화방 타입(0:일반, 2:장터, 3:구인, 4:미팅)
	TCHAR			szTitle[MAX_CHATROOM_TITLE_LEN+1];			// 대화방 제목
	UINT8			iNowPCNum;									// 현재 인원수
	UINT8			iLimitNum;									// 최대 인원 수
} CHATROOM_OBJECT_INFO, *PCHATROOM_OBJECT_INFO;

typedef struct _CHARACTER_BASICINFO				// 대화방 내에서 사용될 캐릭터 기본 정보
{
	UINT16			wWorldID;
	UINT16			wServerID;
	UINT32			uiCharID;					// 캐릭터 ID
	TCHAR			szName[LEN_NAME+1];
	UINT16			uiLevel;					// 레벨
	UINT8			uiClassType;				// Class Type
	UINT8			uiSex;						// 1: 남, 2: 여
	INT32			iGameOptions;				// 게임옵션들
	PCSTATUS_STYLE	stStyle;
} CHARACTER_BASICINFO, *PCHARACTER_BASICINFO;

#define CHATROOM_MAX_PERPACKET	40
#define CHATROOM_MIN_MEMBER_IN_ROOM		2
#define CHATROOM_MAX_MEMBER_IN_ROOM		50
#define CHATROOM_MAX_USER_TYPESEARCH	50

// 대화방
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// GlobalEvent

#define GLOBALEVENT_MAX_RATE		1000*100		// 기획적인 제한은 100배고 시스템적인 제한은 1000배로 결정합니다. 2008.03.19 dhpark 기획팀담당자와 협의
#define GLOBALEVENT_MIN_DATEDIFF	5*60*1000		// 시작시간과 종료시간의 차는 최소 5분
#define GLOBALEVENT_MAX_DATEDIFF	24*60*60*1000	// 시작시간과 종료시간의 차는 최대 24시간
#define GLOBALEVENT_MAX_COMMANDERID	20				// 명령자아이디길이
#define MAX_GLOBALEVENT_PERPACKET	40				// 패킷당 최대 이벤트수

// 이벤트행위자타입 분류
enum { GLOBALEVENT_COMMANDTYPE_SYSTEM = 0, GLOBALEVENT_COMMANDTYPE_GM, };

typedef struct _tagGLOBALEVENT
{
	UINT32 uiID;
	UINT32 uiLDTID;				// byCommanderType이 0인경우 LDTID
	UINT16 wWorldID;
	UINT16 wServerID;
	UINT8 byCommanderType;		// 0:시스템, 1:지엠
	char szCommander[GLOBALEVENT_MAX_COMMANDERID+1];
	SYSTEMTIME stStartTime;
	SYSTEMTIME stEndTime;
	UINT32 uiExpRate;
	UINT32 uiItemDropRate;
	UINT32 uiElyRate;
	UINT32 uiItemRareRate;
	UINT32 uiPortalID;			// 포탈타입 MapEventID
	UINT32 uiMaxUseCnt;			// 포탈사용회수
	UINT32 uiNPCID;				// NPC의 MapEventID

} GLOBALEVENT, *PGLOBALEVENT;

// GlobalEvent
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Auction
#define AUCTION_FEE_MIN			1			// ELY입찰시/cash.mile 등록시 수수료 최소값
#define	SET_AUCTION_FEE(up, ratio, fee) {fee = up*ratio/100; if(ratio && !fee) fee = AUCTION_FEE_MIN;}

// ely,cash,mile 등록시
#define	AUCTION_UPPOINT_MIN		10			
//#define	AUCTION_UPPOINT_MAX		999999999	// 999,999,999 

// 입찰가/시작가/즉구가
#define AUCTION_PRICE_MIN		10			
//#define AUCTION_PRICE_MAX		999999999999 // 999,999,999,999 

#define	AUCTION_ELY_MAX			999999999	//999,999,999
#define AUCTION_MILE_MAX		9999999		//9,999,999
#define AUCTION_CASH_MAX		9999999		//9,999,999


// 대카테고리 아이디
enum AUCTION_CATEGORY
{
	AUCTION_CATEGORY_NULL, 
	AUCTION_CATEGORY_ITEM, 
	AUCTION_CATEGORY_ELY = 8, 
	AUCTION_CATEGORY_CASH, 
	AUCTION_CATEGORY_MILEAGE, 
	MAX_AUCTION_CATEGORY 
};

#define CATEGORY_UNIT 1000			// 대카테고리 단위
#define GET_CATEGORY(x) (x * CATEGORY_UNIT) // 대카테고리

// 아이템 경매 카테고리 체크
// Usage 
// short iCategory = GET_CATEGORY(AUCTION_CATEGORY_ELY);
#define CHECK_AUCTION_ITEM_CATEGORY(x) ( x < GET_CATEGORY(AUCTION_CATEGORY_ELY))

//
// 경매 등록비(보증금) Auction(20080404).doc
// Usage 
//		__int64 iEnrolFee = GET_AUCTION_ENROLFEE_ITEMELY(iStartPrice, ItemLv, ItemQuality, ItemDurability); 
//		iEnrolFee = GET_AUCTION_ENROLFEE(iEnrolfee, pTypeInfo->GetPeriodRate(iDuration))
//

// 아이템등록비

#define MIN_AUCTION_STARTPRICE_FACTOR 1
#define MAX_AUCTION_STARTPRICE_FACTOR 9999
#define MIN_AUCTION_GAMEPRICE_FACTOR 1
#define MAX_AUCTION_GAMEPRICE_FACTOR 9999
#define MIN_AUCTION_STANDARDPRICE_FACTOR 1
#define MAX_AUCTION_STANDARDPRICE_FACTOR 9999

//__int64 iEnrolFee = GET_AUCTION_ENROLFEE_ITEM(pAuction->iStartPrice, iStartPriceFactor, pUpItemInfo->m_iPrice,iGamePriceFactor, tUpDBItem.ItemData.iStackCount);
//__int64 iStandardPrice = GET_AUCTION_ENROLFEE_ITEM_STANDARD(pUpItemInfo->m_iItemLv, iStartPriceFactor);
//if(iEnrolFee < iStandardPrice) iEnrolFee = iStandardPrice;

#define GET_AUCTION_ENROLFEE_ITEM(StartPrice, StartPriceFactor, GamePrice, GamePriceFactor, ItemCount) ((__int64)(StartPrice * StartPriceFactor) / 1000 + (__int64)(GamePrice * GamePriceFactor) / 1000 * ItemCount)
#define GET_AUCTION_ENROLFEE_ITEM_STANDARD(ItemLv, StandardFactor) ((__int64)(ItemLv * StandardFactor) / 10)

//  엘리 등록비
//		Auction_Enrol ={(Game_Money*0.05)+(Auction_Money*2)}/2
#define	GET_AUCTION_ENROLFEE_ELY(StartPrice,  UpPoint) ((UpPoint*5/100 + (__int64)StartPrice*2) /2 + 0.5f)

//	캐쉬 등록시 등록 비용 계산식
// 		Auction_Enrol ={(Cash_money*30)+( Auction_Money*0.2)}/2
#define	GET_AUCTION_ENROLFEE_CASH(StartPrice,  UpPoint) ((UpPoint*30 + (__int64)StartPrice*2/10) /2 + 0.5f)

//	마일리지 등록시 등록 비용 계산식
//		Auction_Enrol = {(Mile_Money*25)+(Auction_Money0.2)}/2
#define	GET_AUCTION_ENROLFEE_MILE(StartPrice,  UpPoint) ((UpPoint*25 + (__int64)StartPrice*2/10) /2 + 0.5f)

//	실제 등록 비용 =  Auction_Enrol +(Auction_Enrol*Auction_Time*0.01)
#define GET_AUCTION_ENROLFEE(Fee, FeeAddRate) (Fee + (Fee * FeeAddRate/100) + 0.5f)

//	경매 시작가 출력
#define GET_AUCTION_STARTMONEY(GamePrice, SysStartMoney, iItemCount) (((GamePrice * SysStartMoney / 1000) + 0.5f) * iItemCount)

// Auction
//------------------------------------------------------------------------------

#define MAX_MONEY			999999999999	// 999,999,999,999 pc 의 돈보유량
#define MAX_CASH			2100000000		// 유저의 캐쉬 보유량
#define MAX_MILE			2100000000		// 유저의 마일리지 보유량

#include <typeinfo.h>

#define DYNAMIC_CAST(T,X) X ? dynamic_cast<T>(X) : (T)X
#define VALID_DYNAMIC_CAST(T,X) X ? (typeid(X) == typeid(T) ? dynamic_cast<T>(X) : 0/*(T)X*/) : 0

#pragma pack(pop)


