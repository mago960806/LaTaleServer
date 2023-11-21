// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPOptionDEF.h
// Comment : 
// Creation : metalgeni 2005-04-7 오후 1:30:38
//***************************************************************************


#pragma once

#define CUR_CLIENT_OPTION_VER  0x00000012

//typedef unsigned short SERVER_SIDE_OPTION;
typedef INT32 SERVER_SIDE_OPTION;

#define LOGIN_ID_LEN 32

enum SPServerSideOption {
	SSO_NULL				= 0,	
	SSO_DEFAULT				= 0xFFFFFFFF,	// 모든 옵션 허용 상태 의미
	SSO_REQ_TRADE			= 1 << 0,		// 거래 요청 허용
	SSO_REQ_TEAM			= 1 << 1,		// 팀 초대 허용
	SSO_REQ_ALERT			= 1 << 2,		// 온라인 상태 전송 허용	
	SSO_REQ_WANTTEAM		= 1 << 3,		// 파티구함 숨김
	SSO_REQ_WHISPER			= 1 << 4,		// 귓속말 허용
	SSO_REQ_CHATTEAM		= 1 << 5,		// 파티채팅 보기
	SSO_REQ_CHATFRIEND		= 1 << 6,		// 친구채팅 보기
	SSO_REQ_CHATGUILD		= 1 << 7,		// 길드채팅 보기
	SSO_REQ_FRIEND			= 1 << 8,		// 친구 초대 허용
	SSO_REQ_CHATROOM_UNUSED = 1 << 9,		// 내정보 검색 허용
	SSO_REQ_CHATROOM_INVITE_DENIAL = 1 << 10,	// 대화 신청 허용
	SSO_REQ_CHATROOM_STYLE_SECRET = 1 << 11,	// 스타일 공개

	//- 
	SSO_REQ_PVP						= 1 << 13,	
	SSO_REQ_LEVEL					= 1 << 14,	
	SSO_REQ_COMBAT					= 1 << 15,	
	SSO_REQ_LOVE					= 1 << 16,		//	LOVE 신청 허용
	SSO_REQ_LOVE_INFO				= 1 << 17,		//	LOVE 연예정보 허용
};

/*
enum SCREEN_SHOT {
	SCREEN_SHOT_PNG			= 0,			// 스크린샷 포멧 PNG
	SCREEN_SHOT_BMP			= 1,			// 스크린샷 포멧 BMP
	SCREEN_SHOT_JPG			= 2,			// 스크린샷 포멧 JPG
	SCREEN_SHOT_MAX			= 3,
};
*/

enum SCREEN_SHOT {
	SCREEN_SHOT_JPG			= 0,			// 스크린샷 포멧 JPG
	SCREEN_SHOT_PNG			= 1,			// 스크린샷 포멧 PNG
	SCREEN_SHOT_BMP			= 2,			// 스크린샷 포멧 BMP	
	SCREEN_SHOT_MAX			= 3,
};


enum EFFECT_STEP {
	EFFECT_STEP_0		= 0,			// 이펙트 하
	EFFECT_SEEP_1		= 1,
	EFFECT_STEP_2		= 2,			// 이펙트 중
	EFFECT_STEP_3		= 3,
	EFFECT_STEP_4		= 4,			// 이펙트 상
	EFFECT_SETP_MAX		= 5,			// 이펙트 맥스
};


struct SPOptionStruct {	
	//System
	bool bExp;							/**< 습득 경험치 <br> */
	bool bItem;							/**< 습득 아이템 <br> Ely 포함 <br> */
	bool bUseItem;						/**< 사용 아이템 <br> */
	bool bHelp;							/**< 채팅창 도움말 <br> */

	bool bEnableBGM;					/**< BGM <br> */
	bool bEnableSE;						/**< 효과음 <br> */	

	int	iVolumeBGM;						/**< BGM Volume <br> */
	int	iVolumeSE;						/**< 효과음 Volume <br> */
	int	iGammaRamp;						/**< 화면밝기 Volume <br> */
	int	iEffect;						/**< Effect Volume <br> */	

	//Community
	//bool bChatParty;					/**< 파티채팅 <br> */
	//bool bChatFriend;					/**< 친구채팅 <br> */
	//bool bChatGuild;					/**< 길드채팅 <br> */

	//bool bChatPermitFriend;				/**< 친구채팅 허용 <br> */
	bool bChatPermitWhisper;			/**< 귓속말 허용 <br> */
	
	//User Benefit
	bool bIndicatePlayer;				/**< 플레이어 이름 표시 <br> */
	bool bIndicateMonster;				/**< 몬스터 이름 표시 <br> */
	bool bIndicateNPC;					/**< NPC 이름 표시 <br> */
	bool bIndicatePet;					/**< Pet 이름 표시 <br> */

	bool bSpeechPlayer;					/**< 말풍선 전체 <br> */
	bool bSpeechTeam;					/**< 말풍선 파티 <br> */	
	bool bSpeechNPC;					/**< 말풍선 NPC <br> */
	bool bSpeechGuild;					/**< 말풍선 Guild <br> */	

	bool bHpBarSelf;					/**< 로컬 유저의 HP Bar <br> */
	bool bHpBarParty;					/**< 파티원의 HP Bar <br> */
	bool bHpBarPlayer;					/**< 타 유저의 HP Bar <br> */	

	bool bAutoHideMouse;				/**< 커서 자동 숨김 <br> */

	int	iScreenShot;					/**< 스크린샷 포멧 <br> */

	//std::string strID;					//[2005/11/11] 유저 ID
};



#define REFRESH_NULL			0
#define REFRESH_SERVERSIDE		(1 << 0)
#define REFRESH_CLIENTSIDE		(1 << 2)
#define REFRESH_ALL				(REFRESH_SERVERSIDE | REFRESH_CLIENTSIDE)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Registry Key Value
#define OPTION_VER			"OptionVer"		
#define OPTION_BIN			"Option"
#define LAST_ID				"ID"
#define LAST_LOGINSERVER	"LoginServer"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Option Archive Message 
#define SPOAM_NULL				0x00000000
#define SPOAM_BASE				0x00010000

#define SPOAM_COMMIT_TO_SERVER		(SPOAM_BASE + 10)
//#define SPOAM_REQUIRE_TO_SERVER		(SPOAM_BASE + 20)
//#define SPOAM_RECEIVE_FROM_SERVER	(SPOAM_BASE + 30)

//#define SPOAM_NAME_PLAYER		(SPOAM_BASE + 60)
//#define SPOAM_NAME_MONSTER		(SPOAM_BASE + 70)
//#define SPOAM_NAME_NPC			(SPOAM_BASE + 80)

//#define SPOAM_SPEECH_PLAYER		(SPOAM_BASE + 130)
//#define SPOAM_SPEECH_TEAM		(SPOAM_BASE + 140)
//#define SPOAM_SPEECH_NPC		(SPOAM_BASE + 150)

//#define SPOAM_COMM_TRADE		(SPOAM_BASE + 200)
//#define SPOAM_COMM_INVITE		(SPOAM_BASE + 210)
//#define SPOAM_COMM_ALERT		(SPOAM_BASE + 220)

#define SPOAM_HIDE_MOUSE		(SPOAM_BASE + 240)

#define SPOAM_BTN_BGM			(SPOAM_BASE + 260)
#define SPOAM_SLD_BGM			(SPOAM_BASE + 270)

#define SPOAM_BTN_SOUNDEFFECT	(SPOAM_BASE + 290)
#define SPOAM_SLD_SOUNDEFFECT	(SPOAM_BASE + 300)
#define SPOAM_SLD_GAMMARAMP		(SPOAM_BASE + 320)

//#define SPOAM_USER_ID			(SPOAM_BASE + 400)		//[2005/11/11]


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Option Archive Message Function Macro

#define SPOAMESSAGE_FUNC SPOptionArchiveMessageMapFunc
#define SPOAMESSAGE_MAP_DECLARE() \
	int SPOAMESSAGE_FUNC( unsigned int spoam, WPARAM wParam, LPARAM lparam);

#define SPOAMESSAGE_MAP_BEGIN( __class ) \
	int __class::SPOAMESSAGE_FUNC( unsigned int spoam, WPARAM wParam, LPARAM lparam) {

#define SPOAMESSAGE_MAP_END_BASE() \
	return -1;\
	}

#define SPOAMESSAGE_COMMAND( message, msg_func )		\
	if ( spoam == message  )	{ return msg_func( wParam, lparam ); }






