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
		- 2006/6/2	: int,unsigned int ¹èÁ¦
	0.6
		- 8¿ù 8ÀÏ »ç³» Å×½ºÆ® ¹öÀü
	0.5
		- WORLD_ID Àü¼Û : CHAR_CS_CREATE,CHAR_CS_CHECK_CHARNAME,CHAR_CS_DELETE
	0.4 
		- CHARACTER_INFO »èÁ¦. ¸ðµç ¼Ò½º CHARACTER_INFO2 »ç¿ëÇÏ°Ô ¼öÁ¤
		- NAV_SC_INSIGHT ¼Û½Å½Ã, MOBÀÇ °æ¿ì MONSTER_INFO ¼Û½ÅÇÏ°Ô ¼öÁ¤
		- CharName : 12 ==> 16 À¸·Î º¯°æ

	0.3
		.... Áß·«
*/


// LEN_ID / LEN_PW ´ë½Å¿¡ g_LEN_ID / g_LEN_PW (GlobalDefines_Share.h) ¸¦ »ç¿ëÇÒ °Í.   2006/11/29
//
// ID/PW/CHARNAME Length Definition 
//{
		#define	MIN_LEN_NAME			2
		#define MIN_LEN_GUILDNAME		2			// for CHINA

		#define LEN_NAME				16			// charName,Name

		#define	MAX_LEN_ID				24
		#define	MAX_LEN_PW				24
		#define	MAX_LEN_NAME			24

		// 2007.02.27	dhpark	DB¿¡¼­ÀÇ ½ÇÁ¦»çÀÌÁî¿¡ ´ëÇÑ Á¤ÀÇ. ÇöÀç´Â Áö¿¥Åø¿¡¼­ »ç¿ëÇÏ±â À§ÇØ¼­ Á¤ÀÇÇÔ.
		// 2007-5-3		hoshim	LEN_ID/PW¿¡ ´ëÇÑ Á¾¼Ó¼º ¹èÁ¦ ¸ñÀûÀ¸·Î ¼­¹ö ÄÚµå»ó¿¡µµ »ç¿ëµÇ±â ½ÃÀÛ.
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

#define MAX_CHATMSG_LEN				150		// Ã¤ÆÃ ¸Þ¼¼Áö ¹®ÀÚ¿­ Á¦ÇÑ°ª
#define LEN_CASHITEM_NORMAL_MSG		60		// Cash Item - '¿ùµå/Ã¤³Î ¿ÜÄ¡±â'½Ã ¹®ÀÚ¿­ Á¦ÇÑ°ª
//#define LEN_CASHITEM_EVENT_MSG		150		// Cash Item - 'ÀÌº¥Æ® ¸Þ¼¼Áö'½Ã ¹®ÀÚ¿­ Á¦ÇÑ°ª

#define MAX_CHAR_NUM				3			// À¯Àú´ç »ý¼º°¡´É Ä³¸¯ÅÍ ¼ö
#define MAX_CHAR_DEL				30			// À¯Àú´ç Ä³¸¯ ÃÖ´ë»èÁ¦È½¼ö
#define MAX_WORLDS					10			// ÃÖ´ë ¿ùµå ¼ö
#define MAX_CHANNELS				20			// ¿ùµå³» ÃÖ´ë Ã¤³Î ¼ö(1 ~ 20)
#define MAX_INDUNS					30			// ¿ùµå³» ÃÖ´ë ÀÎ´ø¼­¹ö ¼ö(51 ~ 80)
#define MAX_PVPS					19			// ¿ùµå³» ÃÖ´ë PVP¼­¹ö ¼ö(81 ~ 99)
#define MAX_LOBBYS					1			// ¿ùµå³» ÃÖ´ë ·Îºñ¼­¹ö ¼ö
#define MAX_WORLDSERVERS			MAX_CHANNELS + MAX_INDUNS + MAX_PVPS + MAX_LOBBYS
#define MAX_GAMESERVERS				MAX_CHANNELS + MAX_INDUNS + MAX_PVPS

// ±âÈ¹¿¡ÀÇÇØ¼­ ¾ÆÀÌÅÛ ÀÌ¸§ ÃÖ´ëÅ©±â ¿ä±¸µÊ.(ÀÓ½Ã)
// DB ¿Í ¿¬°üµÇ¾î ÀÖ±â ¶§¹®¿¡ Àý´ë ÀÓÀÇ º¯°æ ºÒ°¡ !!!!!
#define MAX_ITEM_NAME		30  

#define MAX_CHATROOM_TITLE_LEN		40
#define MAX_CHATROOM_ADVERT_LEN		50
#define MAX_CHATROOM_PASSWORD_LEN	4

#define MAX_FRIEND_LIST				99
#define MAX_BLOCK_LIST				99

// MailBox
#define	MAILBOX_MAX_ENTRY			40			// (¼ö½Å) ¸ÞÀÏÇÔÀÇ ÃÖ´ë ¸ÞÀÏ °¹¼ö
#define	MAILBOX_MAX_SUBJECT_LEN		50			// ¸ÞÀÏ Á¦¸ñ ÃÖ´ë Å©±â
#define	MAILBOX_MAX_MEMO_LEN		100			// ¸ÞÀÏ º»¹® ÃÖ´ë Å©±â
#define MAILBOX_SEND_FEE			50			// ¸ÞÀÏ Àü¼Û ¼ö¼ö·á

#define KEEP_ALIVE_TIMEOUT			10000		// 10ÃÊ

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
	CLASS_A		= 1,		// °ø°ÝÇü
	CLASS_B 	= 2,		// ¹æ¾îÇü
	CLASS_C		= 3,		// ¸¶¹ýÇü
	CLASS_D		= 4,		// Á¤ÂûÇü
	CLASS_E		= 5,		// ¿ö¸®¾î
	CLASS_F 	= 6,		// ±¤Àü»ç
	CLASS_G		= 7,		// ¼º±â»ç
	CLASS_H		= 8,		// ¸ùÅ©
	CLASS_I		= 9,		// ÅÛÇÃ·¯
	CLASS_J 	= 10,		// ¾ÆÆ¼½ºÆ®
	CLASS_K		= 11,		// Æ®·¹Á®ÇåÅÍ
	CLASS_L		= 12,		// °Ç³Ê
	CLASS_M		= 13,		// ¿£Áö´Ï¾î(Base)
	CLASS_N		= 14,		// ¿£Áö´Ï¾îÈ®Àå
	CLASS_MAX 
};
//#else
//enum CLASS_TYPE {
//	CLASS_NULL	= 0,
//	CLASS_A		= 1,		// °ø°ÝÇü
//	CLASS_B		= 2,		// ¹æ¾îÇü
//	CLASS_C		= 3,		// ¸¶¹ýÇü
//	CLASS_D		= 4,		// Á¤ÂûÇü
//	CLASS_MAX 
//};
//#endif

const int CLASS_CHANGE_BASE = (CLASS_D+1);
enum VERSION_OPTION{KEEPALIVE=1,VERBOSE};

enum SAVEPOINT_TYPE // shift count
{
	SAVEPOINT_TYPE_TOWN,	// ¸¶À».	
	SAVEPOINT_TYPE_SPOT,	// ±âÅ¸Æ÷ÀÎÆ®.
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

// 2006.08.07 dhpark ÆÐÅ¶Á¤ÀÇ ·êÀ» º¯°æÇÕ´Ï´Ù.
#define PACKETID		UINT32
#define PI_CS_BASE		0				// 0				~	999,999,999		0ºÎÅÍ 10¾ï -1 ±îÁö
#define PI_SS_BASE		1000000000		// 1,000,000,000	~	1,999,999,999	10¾ïºÎÅÍ 20¾ï - 1 ±îÁö
#define PI_SYSTEM_BASE	0				// 0
#define PI_TOOL_BASE	100000000		// 1¾ï
#define PI_LOGIC_BASE	200000000		// 2¾ï
#define PI_SITE_BASE	300000000		// 3¾ï

#define PI_CS_SYSTEM	PI_CS_BASE + PI_SYSTEM_BASE
#define PI_CS_TOOL		PI_CS_BASE + PI_TOOL_BASE
#define PI_CS_LOGIC		PI_CS_BASE + PI_LOGIC_BASE
#define PI_SS_SYSTEM	PI_SS_BASE + PI_SYSTEM_BASE
#define PI_SS_TOOL		PI_SS_BASE + PI_TOOL_BASE
#define PI_SS_LOGIC		PI_SS_BASE + PI_LOGIC_BASE
#define PI_SS_SITE		PI_SS_BASE + PI_SITE_BASE

// ³»ºÎÀÇ SubjectÀÇ ´ÜÀ§´Â ±âº» 100 ´ÜÀ§ ÃßÃµ...
// ¿¹)	PI_CS_SUBJECT3 = PI_CS_TOOL + 700,
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
		KEEP_ALIVE_PACKET = PI_CS_ECHO, // ¼ö½ÅµÈ ÆÐÅ¶À» °¡°ø¾øÀÌ ±×´ë·Î ÀçÀü¼ÛÇÑ´Ù.

	//////////////////////////////////////////////////////////////////////////////
	//
	// Protocol : Start & Version Check
	//
		PI_CS_PROTOCOL = PI_CS_SYSTEM + 100,
		
		// Step 1 : OnAcceptÀÌÈÄ ÃÖÃÊ·Î ¼­¹ö°¡ Å¬¶óÀÌ¾ðÆ®¿¡°Ô Àü¼ÛÇÏ´Â ÆÐÅ¶
		PROTOCOL_SC_START_REQ = PI_CS_PROTOCOL,				// nProtect (°ÔÀÓ °¡µå ÀÎÁõ)
		// UINT8	uiStartDataLen;				// sizeof(GG_AUTH_DATA)		: ¼­¹ö¿¡¼­ Å°¸¦ »ý¼ºÇÏ¿© Å¬¶óÀÌ¾ðÆ®¿¡ Àü¼Û
		// TCHAR	szStartData[];				// GG_AUTU_DATA
		// UINT8	uiCipherMode;

		// Step 2 : Å¬¶óÀÌ¾ðÆ®ÀÇ ÀÀ´ä
		PROTOCOL_CS_START_RES,				// Å¬¶óÀÌ¾ðÆ®¿¡¼­´Â °ÔÀÓ °¡µå¿¡ ³Ñ±äÈÄ, °á°ú°ªÀ» ¸®ÅÏ.  ¼­¹ö»ó¿¡¼­ ÀÌ °ªÀ» È®ÀÎÇÏ¿© °ÔÀÓ °¡µå »óÅÂ¸¦ ÀÎÁõÇÑ´Ù. 
		// UINT8	uiStartDataLen;
		// TCHAR	szStartData[];

		// Step 3 : ÀÌ ½ÃÁ¡ºÎÅÍ Å¬¶óÀÌ¾ðÆ®´Â ÆÐÅ¶À» ¼­¹ö·Î Àü¼ÛÇÒ ¼ö ÀÖ´Ù.
		PROTOCOL_SC_START,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT »óÀÇ ¸Þ¼¼Áö¸¦ itemID °ª

		//PROTOCOL_CS_VERSION_CHECK,
		//// UINT16 major;
		//// UINT16 minor;
		//// UINT32 option;		// VERSION_OPTION Âü°í
		//		//option :

		//		//	1st bit : Keep-alive packet sending ON/OFF
		//		//	2nd bit : Verbose ON/OFF
		//		//	3rd bit : reserved
		//		//	...		: reserved

		//PROTOCOL_SC_VERSION_CHECK,
		//// RESULTCODE resultCode;			// LT_ERROR_VERSION_LOWER or LT_NO_ERROR or LT_ERROR_VERSION_UNSURPORTED_OPTION

		// ÀÏ¹ÝÀûÀÎ ÆÐÅ¶ °ü·Ã ¿À·ù ¹ß»ý
		SYSTEM_SC_ERROR,
		// GLOBAL_STRING_ID	itemID;	
		// UINT16	uiPacketLen;			// ¿À·ù¹ß»ý ½ÃÁ¡ÀÇ ÆÐÅ¶ Á¤º¸
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

		// DNS Server µµÀÔ site¿¡¼­ÀÇ '¼­¹ö º¯°æ' ±â´É ÀÌ¿ë½Ã LS·ÎÀÇ Àç¿¬°á º¸Àå ¸ñÀû
		PROTOCOL_SC_LOGINSERVERINFO,
		// UINT16	uiServerID;
		// TCHAR	szIP[LEN_IP];
		// UINT16	usPort;


	//////////////////////////////////////////////////////////////////////////////
	//
	// World Environment (DATETIME/Weather,...) (GameServer)
	//
		PI_CS_WORLDENV = PI_CS_SYSTEM + 200,

		// ¿ùµå Å¸ÀÓ ¼³Á¤
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
	// Áßº¹·Î±×ÀÎ°Ë»ç
	//
		PI_CS_USER = PI_CS_SYSTEM + 300,

		// °ÔÀÓ¼­¹ö¿¡ À¯Àú°¡ ·Î±×ÀÎÇÑ´Ù°í ¾Ë¸³´Ï´Ù.(·Î±×ÀÎ, ·Îºñ, °ÔÀÓ)
		USER_CS_IN = PI_CS_USER,
		// UINT32			uiUserID;
		// PASSPORT			uiPassport;
		// TCHAR			szIP[LEN_IP+1];

		// Å¬¶óÀÌ¾ðÆ®¿¡ Áßº¹°Ë»ç °á°ú¸¦ ¼Û½ÅÇÕ´Ï´Ù.
		USER_SC_IN,
		// GLOBAL_STRING_ID	uiErrorID;
		// PASSPORT			uiPassport;

		// °ÔÀÓ¼­¹ö¿¡ À¯Àú°¡ ÀÌµ¿ÇÑ´Ù°í ¾Ë¸³´Ï´Ù.(·Î±×ÀÎ->·Îºñ, ·Îºñ->°ÔÀÓ, °ÔÀÓ->°ÔÀÓ, °ÔÀÓ->·Î±×ÀÎ, ·Îºñ->·Î±×ÀÎ)
		USER_CS_MOVE,
		// UINT16			wDstServerID;

		// Å¬¶óÀÌ¾ðÆ®¿¡ ÀÌµ¿ °á°ú¸¦ ¼Û½ÅÇÕ´Ï´Ù.
		USER_SC_MOVE,
		// GLOBAL_STRING_ID	uiErrorID;

	//////////////////////////////////////////////////////////////////////////////
	//
	// Cheat 
	//
		PI_CS_CHEAT = PI_CS_SYSTEM + 400,

		// ÇöÀç À§Ä¡¸¦ ÁÖ¾îÁø °÷À¸·Î ÁöÁ¤(¿öÇÁµÊ) for cheating
		//   ¼­¹ö´Â NAV_SC_MOVE¸¦ ºê·ÎµåÄ³½ºÆÃ
		CHEAT_CS_SETPOSITION = PI_CS_CHEAT,
		// TCHAR			charName[LEN_NAME];
		// LT_POSITION		pos;

		// GMÀÇ cheat mode Áö¿ø ¸ñÀû
		CHEAT_CS_COMMAND ,
		// UINT8			len;
		// TCHAR			szCheatCommand[];			//<CheatCommand><whitespace><ParamList>


	//////////////////////////////////////////////////////////////////////////////
	//
	// Notification
	//
		NOTI_SC_BASE = PI_CS_SYSTEM + 500,

		// ¾Ë¸®¹Ì ¼­ºñ½º(Ä£±¸,...)
		NOTI_SC_ONLINE_STATUS_FRIEND = NOTI_SC_BASE,
		// UINT8		uiNotiType;				// 1(NotiType_Friend_OnlineStatus)
		// UINT8		uiStatus;				// 1(CH_IN),2(CH_OUT) [,3(CH_MOVE),4(ETC)]
		// UINT16		uiServerID;				// ÇöÀç À§Ä¡ÇÑ ¼­¹ö
		// UINT32		uiActorCharID;
		// UINT8		len;
		// TCHAR		szActorCharName[];


		// µî·ÏµÈ Ä£±¸ÀÇ online»óÅÂ º¯°æ »çÇ×À» ¾Ë¸². (ON,OFF)
		NOTI_MG_FRIEND_STATUS,
		// UINT8		uiNotiType,				// 1(NotiType_Friend_OnlineStatus)
		// UINT8		uiStatus,				// 1(CH_IN),2(CH_OUT) [,3(CH_MOVE),4(ETC)]
		// UINT32		uiCharID;				// Notification¸¦ ¼ö½ÅÇÒ ´ë»ó
		// UINT32		uiNotiActorCharID;		// ÇØ´ç NotificationÀÇ ´ë»ó
		// UINT16		uiServerID;				// Actor°¡ À§Ä¡ÇÑ ¼­¹ö
		// UINT8		len;
		// TCHAR		szActorCharName[];

		// CHANNEL_CS_IN »óÅÂ¿¡¼­ Ä£±¸¸ñ·ÏÀ» µî·ÏÇÔ.
		// È¤½Ã ÇØ´ç Ä³¸¯ÅÍÀÇ Ä£±¸ ¸ñ·ÏÀÌ Á¸ÀçÇÏ¸é ±âÁ¸ ¸ñ·ÏÀº ¸ðµÎ »èÁ¦ÇÏ°í »õ·Î µî·ÏÇÔ
		NOTI_GM_FRIEND_REGISTER_LIST,
		// UINT32		uiCharID;
		// UINT8		nCount;
		// UINT32		uiFriendCharID[];

		// CHANNEL_CS_OUT »óÅÂ¿¡¼­ uiCharIDÀÇ Ä£±¸¸ñ·ÏÀ» µî·Ï ÇØÁ¦ÇÔ.
		NOTI_GM_FRIEND_UNREGISTER_LIST,
		// UIINT32		uiCharID;

		// Ä£±¸ ¸ñ·Ï¿¡ Ãß°¡·Î ÀÎÇÑ °ü¸®¸ñ·Ï»ó¿¡ Ãß°¡
		NOTI_GM_FRIEND_ADD,
		// UINT32		uiCharID;
		// UINT32		uiFriendCharID;

		// Ä£±¸ ¸ñ·Ï¿¡¼­ »èÁ¦ÇÏ¿© °ü¸®¸ñ·Ï»ó¿¡¼­ »èÁ¦
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
		// UINT32		uiOnlineMinutes;				// Á¢¼Ó½Ã°£
		// UINT32		uiOfflineMinutes;				// ºñÁ¢¼Ó½Ã°£
		// UINT16		usRate_Ely;				
		// UINT16		usRate_Exp;						// ÇöÀç Àû¿ëµÈ °æÇèÄ¡ °ü·Ã factor
		// UINT16		usRate_ItemDrop;				// ÇöÀç Àû¿ëµÈ ¾ÆÀÌÅÛ µå¶ø·ü °ü·Ã factor

		// À¯Àú°¡ ÀÚ½ÅÀÇ Á¢¼Ó»óÅÂ¸¦ È®ÀÎÇÏ´Â ¿ëµµ·Î »ç¿ë. FDS_SC_NOTICE ¸®ÇÃ¶óÀÌµÊ
		FDS_CS_QUERYSTATE,
		// UINT32		uiReservedParam1;
		// UINT32		uiReservedParam2;


	//////////////////////////////////////////////////////////////////////////////
	//
	// Security System : Å¬¶óÀÌ¾ðÆ® º¸¾ÈÅø °Ë»ç
	//

		SECURITY_CS_BASE = PI_CS_SYSTEM + 800,

		// ¼­¹ö¿¡¼­ Query()¸¦ ÅëÇØ¼­ »ý¼ºµÈ °ËÁõ¿ë µ¥ÀÌÅÍ ¼Û½Å
		SECURITY_SC_CHECK = SECURITY_CS_BASE,
		// SECURITY_PACKDATA	stPackData;				// º¸¾ÈÅø °ËÁõ¿ë µ¥ÀÌÅÍ

		// Å¬¶óÀÌ¾ðÆ®¿¡¼­ º¸¾ÈÅøÀ» ÀÌ¿ëÇØ¼­ °ËÁõÇÑ µ¥ÀÌÅÍ ¼ö½Å
		SECURITY_CS_CHECK,
		// SECURITY_PACKDATA	stPackData;				// º¸¾ÈÅø °ËÁõ¿ë µ¥ÀÌÅÍ

		// º¸¾ÈÅø °Ë»ç °á°ú Åëº¸. ¸Þ¼¼ÁöÃ¢À» ÅëÇØ¼­ Å¬¶óÀÌ¾ðÆ® Àç½ÃÀÛÀ» À¯µµÇÕ´Ï´Ù.
		SECURITY_SC_INFO,
		// GLOBAL_STRING_ID		ErrorID;				// ¿¡·¯¾ÆÀÌµð

};

enum PACKETID_CS_TOOL
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	GMLOGIN
	//
		PI_CS_GMLOGIN = PI_CS_TOOL + 0,

		// Åø¼­¹ö¿¡ ·Î±×ÀÎ
		TOOL_CS_LOGIN = PI_CS_GMLOGIN,
		// char 			szID[MAX_LEN_ID_DB+1];
		// char 			szPW[MAX_LEN_PW_DB+1];
		// int				iVersion;		// GMÅøÀÌ »ç¿ëÇÏ´Â ¹öÁ¯¹øÈ£
		// UINT16			major;			// ¼­¹ö&Å¬¶óÀÌ¾ðÆ®°¡ »ç¿ëÇÏ´Â ¹öÁ¯Áß ¸ÞÀÌÁ®
		// UINT16			minor;			// ¼­¹ö&Å¬¶óÀÌ¾ðÆ®°¡ »ç¿ëÇÏ´Â ¹öÁ¯Áß ¸¶ÀÌ³Ê
		// UINT32			option;			// VERSION_OPTION Âü°í

		// ·Î±×ÀÎ¿¡ ´ëÇÑ ÀÀ´ä
		TOOL_SC_LOGIN,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// UINT8 byPermit;					// ±ÇÇÑ
		// char	szName[LEN_NAME+1];			// ÀÌ¸§

		TOOL_CS_LOGIN2,
		// UINT8			lenID;
		// TCHAR			ID[];
		// TCHAR			cMagicChar[1];	// must be MAGIC_CHAR
		// UINT8			lenPW;
		// TCHAR			PW[];
		// int				iVersion;		// GMÅøÀÌ »ç¿ëÇÏ´Â ¹öÁ¯¹øÈ£
		// UINT16			major;			// ¼­¹ö&Å¬¶óÀÌ¾ðÆ®°¡ »ç¿ëÇÏ´Â ¹öÁ¯Áß ¸ÞÀÌÁ®
		// UINT16			minor;			// ¼­¹ö&Å¬¶óÀÌ¾ðÆ®°¡ »ç¿ëÇÏ´Â ¹öÁ¯Áß ¸¶ÀÌ³Ê
		// UINT32			option;			// VERSION_OPTION Âü°í

	//////////////////////////////////////////////////////////////////////////
	//
	//	GMNOTICE
	//
		PI_CS_GMNOTICE = PI_CS_TOOL + 100,

		// Åø¿¡¼­ °øÁö»çÇ×À» Åø¼­¹ö¿¡ ¼Û½Å
		TOOL_CS_NOTICE = PI_CS_GMNOTICE,
		// UINT16 worldID;					// 0(all)
		// UINT16 serverID;					// 0(all)
		// UINT8 displayTime;				// 0(forever), sec
		// UINT8 optionFlags;				// reserved. display È¿°ú ¿ëµµ(Ä¯¹ÚÀÓ,Ä®¶ó,...)
		// UINT16 uiLen;
		// TCHAR szMsg[];

	//////////////////////////////////////////////////////////////////////////
	//
	//	GMWORLDLIST
	//
		PI_CS_GMWORLDLIST = PI_CS_TOOL + 200,

		// ¿ùµå¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_WORLDLIST = PI_CS_GMWORLDLIST,

		// ¿ùµå¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_WORLDLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iCnt;
		// GM_WORLD			stGM_World;
		// ...

	//////////////////////////////////////////////////////////////////////////
	//
	//	USERTAB
	//
		PI_CS_USERTAB = PI_CS_TOOL + 300,
	
		// -- »ç¶óÁú ÆÐÅ¶
		// MServerÀÇ ¿Â¶óÀÎÀ¯Àú¸¦ »èÁ¦ÇÕ´Ï´Ù.(À¯Àú°ü¸®°¡ Á¦´ë·Î ¾ÈµÇ¾î ³²¾ÆÀÖ´Â °æ¿ì¿¡ ´ëÇÑ Ã³¸®)
		TOOL_CS_OFFLINE = PI_CS_USERTAB,
		// UINT32 uiUserID;

		// -- »ç¶óÁú ÆÐÅ¶
		// ¿Â¶óÀÎÀ¯Àú »èÁ¦ °á°ú
		TOOL_SC_OFFLINE,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

		// °ÔÀÓ¼­¹ö³»ÀÇ À¯Àú¸¦ Ãß¹æÇÕ´Ï´Ù.
		TOOL_CS_KICKOUT,
		// UINT32 uiUserID;

		// À¯ÀúÃß¹æ °á°ú
		TOOL_SC_KICKOUT,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// °èÁ¤ÀÇ ºí·ÏÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_USER_BLOCK,
		// UINT32			uiUserID;		// À¯Àú¾ÆÀÌµð(À¯ÀúÀÇ µðºñÅ°)
		// UINT8			byBlock;		// ºí·Ï¿©ºÎ

		// °èÁ¤ÀÇ ºí·ÏÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_USER_BLOCK,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// Ã¢°í¸Ó´Ï¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_STORAGE_MONEY,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32			uiUserID;		// À¯Àú¾ÆÀÌµð(À¯ÀúÀÇ µðºñÅ°)
		// INT64			iStorageMoney;	// Ã¢°íµ·

		// Ã¢°í¸Ó´Ï¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_STORAGE_MONEY,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

		// À¯Àú¸¦ °Ë»öÇÕ´Ï´Ù.
		TOOL_CS_FINDUSER,
		// UINT32 uiUserID;

		// À¯Àú°Ë»ö °á°ú
		TOOL_SC_FINDUSER,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// UINT16 usServerID;				// ¼­¹ö¾ÆÀÌµð(ÇÃ·¹ÀÌÁßÀÎ ¼­¹öÀ§Ä¡)
		// UINT8 byStatus;					// À¯Àú»óÅÂ(IN, MOVE(ÀÌµ¿½ÃÀÛ), OUT(ÀÌµ¿À»À§ÇØ¼­Á¾·á))
		// UINT8 byValid;					// °ÔÀÓ¼­¹ö¿¡ ½ÇÁ¦·Î Á¸Àç¿©ºÎ(0:X, 1:Á¸Àç)
		// char szCharName[LEN_NAME+1];		// Ã¤³Î¼­¹ö¿¡¼­ ÇÃ·¹ÀÌÁßÀÏ¶§ ¼±ÅÃµÈ Ä³¸¯ÅÍ¸í
	
		// Ä³¸¯ÅÍ»èÁ¦È½¼ö¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_CHARDELCOUNT,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32			uiUserID;		// À¯Àú¾ÆÀÌµð(À¯ÀúÀÇ µðºñÅ°)
		// UINT8			byCharDelCount;	// Ä³¸¯ÅÍ»èÁ¦È½¼ö

		// Ä³¸¯ÅÍ»èÁ¦È½¼ö¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_CHARDELCOUNT,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

	//////////////////////////////////////////////////////////////////////////
	//
	//	PCTAB
	//
		PI_CS_PCTAB = PI_CS_TOOL + 400,
	
		// AccountNameÀ¸·Î PC¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_PCLISTBYACCOUNT = PI_CS_PCTAB,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// char				szAccountName[MAX_LEN_ID_DB+1];

		// AccountNameÀ¸·Î PC¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_PCLISTBYACCOUNT,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// UINT32			uiUserID;		// À¯Àú¾ÆÀÌµð(À¯ÀúÀÇ µðºñÅ°)
		// UINT8			byBlock;		// ºí·Ï
		// INT64			iStorageMoney;	// Ã¢°íµ·
		// char				szAccountName[MAX_LEN_ID_DB+1];
		// int				iCnt;
		// GM_PC			stGM_PC;
		// ...
		// UINT8			byCharDelCount;	// Ä³¸¯ÅÍ»èÁ¦È½¼ö
	
		// CharNameÀ¸·Î PC¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_PCLISTBYCHAR,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// char				szCharName[LEN_NAME+1];

		// CharNameÀ¸·Î PC¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_PCLISTBYCHAR,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// UINT32			uiUserID;		// À¯Àú¾ÆÀÌµð(À¯ÀúÀÇ µðºñÅ°)
		// UINT8			byBlock;		// ºí·Ï
		// INT64			iStorageMoney;	// Ã¢°íµ·
		// char				szAccountName[MAX_LEN_ID_DB+1];
		// int				iCnt;
		// GM_PC			stGM_PC;
		// ...
		// UINT8			byCharDelCount;	// Ä³¸¯ÅÍ»èÁ¦È½¼ö
	
		// PCÀÇ ±ÇÇÑÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_PC_PERMISSION,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°
		// UINT8			byPermission;	// ±ÇÇÑ

		// PCÀÇ ±ÇÇÑÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_PC_PERMISSION,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// PCÀÇ ÀÌ¸§À» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_PC_CHARNAME,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°
		// char				szOldCharName[LEN_NAME+1];
		// char				szNewCharName[LEN_NAME+1];

		// PCÀÇ ÀÌ¸§À» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_PC_CHARNAME,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// PCÀÇ Á¤º¸µéÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_PC,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_PC			stGM_PC;		// PCÁ¤º¸

		// PCÀÇ Á¤º¸µéÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_PC,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// PC¸¦ »èÁ¦ ¶Ç´Â º¹±¸ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_PC_DEL,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°
		// UINT8			byDel;			// »èÁ¦¿©ºÎ

		// PC¸¦ »èÁ¦ ¶Ç´Â º¹±¸ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_PC_DEL,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// PCÀÇ Å¸ÀÌÆ²¾ÆÀÌµð¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_PC_TITLEID,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°
		// int				iTitleID;		// Å¸ÀÌÆ²¾ÆÀÌµð

		// PCÀÇ Å¸ÀÌÆ²¾ÆÀÌµð¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_PC_TITLEID,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

	//////////////////////////////////////////////////////////////////////////
	//
	//	ITEMTAB
	//
		PI_CS_ITEMTAB = PI_CS_TOOL + 500,

		// ÄÁÅ×ÀÌ³Ê Å¸ÀÔº° ¾ÆÀÌÅÛ¸®½ºÆ®/È®Àå½½·ÔÀ» °¡Á®¿É´Ï´Ù.
		TOOL_CS_ITEMLIST = PI_CS_ITEMTAB,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32			uiUserID;		// À¯Àú¾ÆÀÌµð(À¯ÀúÀÇ µðºñÅ°)
		// int				iCharID;		// PCÀÇ DBÅ°
		// UINT8			byContainerType;// ÄÁÅ×ÀÌ³ÊÅ¸ÀÔ

		// ÄÁÅ×ÀÌ³Ê Å¸ÀÔº° ¾ÆÀÌÅÛ¸®½ºÆ®/È®Àå½½·ÔÀ» °¡Á®¿É´Ï´Ù.
		TOOL_SC_ITEMLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// UINT8			byContainerType;// ÄÁÅ×ÀÌ³ÊÅ¸ÀÔ
		// UINT8			byExpandSlot;	// È®Àå½½·Ô¼ö
		// int				iCnt;
		// GM_ITEM			stGM_Item;
		// ...
	
		// È®Àå½½·ÔÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_EXPANDSLOT,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32			uiUserID;		// À¯Àú¾ÆÀÌµð(À¯ÀúÀÇ µðºñÅ°)
		// int				iCharID;		// PCÀÇ DBÅ°
		// UINT8			byContainerType;// ÄÁÅ×ÀÌ³ÊÅ¸ÀÔ
		// UINT8			byExpandSlot;	// È®Àå½½·Ô¼ö

		// È®Àå½½·ÔÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_EXPANDSLOT,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// ¾ÆÀÌÅÛÀÇ Á¤º¸µéÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_ITEM,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_ITEM			stGM_ITEM;		// ITEMÁ¤º¸

		// ¾ÆÀÌÅÛÀÇ Á¤º¸µéÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_ITEM,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// ¾ÆÀÌÅÛÀ» »ý¼ºÇÕ´Ï´Ù.
		TOOL_CS_CREATE_ITEM,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_ITEM			stGM_ITEM;		// ITEMÁ¤º¸

		// ¾ÆÀÌÅÛÀ» »ý¼ºÇÕ´Ï´Ù.
		TOOL_SC_CREATE_ITEM,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// INT64			iItemNo;		// ¾ÆÀÌÅÛDBÅ°
	
		// ¾ÆÀÌÅÛÀ» »èÁ¦ÇÕ´Ï´Ù.
		TOOL_CS_DELETE_ITEM,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// INT64			iItemNo;		// ¾ÆÀÌÅÛDBÅ°

		// ¾ÆÀÌÅÛÀ» »èÁ¦ÇÕ´Ï´Ù.
		TOOL_SC_DELETE_ITEM,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

	//////////////////////////////////////////////////////////////////////////
	//
	//	SKILLTAB
	//
		PI_CS_SKILLTAB = PI_CS_TOOL + 600,
	
		// ½ºÅ³¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_SKILLLIST = PI_CS_SKILLTAB,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°

		// ½ºÅ³¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_SKILLLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iCnt;
		// GM_SKILL			stGM_Skill;
		// ...
	
		// ½ºÅ³Á¤º¸¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_SKILL,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_SKILL			stGM_Skill;

		// ½ºÅ³Á¤º¸¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_SKILL,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// ½ºÅ³À» »ý¼ºÇÕ´Ï´Ù.
		TOOL_CS_CREATE_SKILL,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_SKILL			stGM_Skill;

		// ½ºÅ³À» »ý¼ºÇÕ´Ï´Ù.
		TOOL_SC_CREATE_SKILL,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// ½ºÅ³À» »èÁ¦ÇÕ´Ï´Ù.
		TOOL_CS_DELETE_SKILL,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_SKILL			stGM_Skill;

		// ½ºÅ³À» »èÁ¦ÇÕ´Ï´Ù.
		TOOL_SC_DELETE_SKILL,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

	//////////////////////////////////////////////////////////////////////////
	//
	//	QUESTTAB
	//
		PI_CS_QUESTTAB = PI_CS_TOOL + 700,

		// Äù½ºÆ®¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_QUESTLIST = PI_CS_QUESTTAB,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°

		// Äù½ºÆ®¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_QUESTLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iCnt;
		// GM_QUEST			stGM_Quest;
		// ...
	
		// Äù½ºÆ®Á¤º¸¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_QUEST,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_QUEST			stGM_Quest;

		// Äù½ºÆ®Á¤º¸¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_QUEST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// Äù½ºÆ®À» »ý¼ºÇÕ´Ï´Ù.
		TOOL_CS_CREATE_QUEST,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_QUEST			stGM_Quest;

		// Äù½ºÆ®À» »ý¼ºÇÕ´Ï´Ù.
		TOOL_SC_CREATE_QUEST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// Äù½ºÆ®À» »èÁ¦ÇÕ´Ï´Ù.
		TOOL_CS_DELETE_QUEST,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_QUEST			stGM_Quest;

		// Äù½ºÆ®À» »èÁ¦ÇÕ´Ï´Ù.
		TOOL_SC_DELETE_QUEST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

	//////////////////////////////////////////////////////////////////////////
	//
	//	MISSIONTAB
	//
		PI_CS_MISSIONTAB = PI_CS_TOOL + 800,
	
		// ¹Ì¼Ç¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_MISSIONLIST = PI_CS_MISSIONTAB,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°

		// ¹Ì¼Ç¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_MISSIONLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iCnt;
		// GM_MISSION		stGM_Mission;
		// ...
	
		// ¹Ì¼ÇÁ¤º¸¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_MISSION,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_MISSION		stGM_Mission;

		// ¹Ì¼ÇÁ¤º¸¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_MISSION,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// ¹Ì¼ÇÀ» »ý¼ºÇÕ´Ï´Ù.
		TOOL_CS_CREATE_MISSION,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_MISSION		stGM_Mission;

		// ¹Ì¼ÇÀ» »ý¼ºÇÕ´Ï´Ù.
		TOOL_SC_CREATE_MISSION,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
	
		// ¹Ì¼ÇÀ» »èÁ¦ÇÕ´Ï´Ù.
		TOOL_CS_DELETE_MISSION,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_MISSION		stGM_Mission;

		// ¹Ì¼ÇÀ» »èÁ¦ÇÕ´Ï´Ù.
		TOOL_SC_DELETE_MISSION,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

	//////////////////////////////////////////////////////////////////////////
	//
	//	MAILTAB
	//
		PI_CS_MAILTAB = PI_CS_TOOL + 900,
	
		// ¸ÞÀÏ¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_MAILLIST = PI_CS_MAILTAB,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°

		// ¸ÞÀÏ¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_MAILLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iCnt;
		// GM_MAIL			stGM_Mail;
		// ...
	
		// ¸ÞÀÏÀ» »èÁ¦ÇÕ´Ï´Ù.
		TOOL_CS_DELETE_MAIL,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_MAIL			stGM_Mail;

		// ¸ÞÀÏÀ» »èÁ¦ÇÕ´Ï´Ù.
		TOOL_SC_DELETE_MAIL,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

		// ¸ÞÀÏÀ» ¾÷µ¥ÀÌÆ® ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_MAIL,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_MAIL			stGM_Mail;

		// ¸ÞÀÏÀ» ¾÷µ¥ÀÌÆ® ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_MAIL,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iCnt;
		// GM_MAIL			stGM_Mail;
		

	//////////////////////////////////////////////////////////////////////////
	//
	//	GIFT
	//
		PI_CS_GIFTTAB = PI_CS_TOOL + 1000,
	
		// ¸ÞÀÏ¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_GIFTLIST = PI_CS_GIFTTAB,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// char				szAccountName[LEN_ID+1];

		// ¸ÞÀÏ¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_GIFTLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iCnt;
		// GM_GIFT			stGM_Gift;
		// ...

	//////////////////////////////////////////////////////////////////////////
	//
	//	EFFECTTAB
	//
		PI_CS_EFFECTTAB = PI_CS_TOOL + 1100,
	
		// ÀÌÆåÆ®¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_EFFECTLIST = PI_CS_EFFECTTAB,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°

		// ÀÌÆåÆ®¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_EFFECTLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iCnt;
		// GM_EFFECT		stGM_Effect;
		// ...
	
		// ÀÌÆåÆ®¸¦ »èÁ¦ÇÕ´Ï´Ù.
		TOOL_CS_DELETE_EFFECT,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_EFFECT		stGM_Effect;

		// ÀÌÆåÆ®¸¦ »èÁ¦ÇÕ´Ï´Ù.
		TOOL_SC_DELETE_EFFECT,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

	//////////////////////////////////////////////////////////////////////////
	//
	//	TITLETAB
	//
		PI_CS_TITLETAB = PI_CS_TOOL + 1200,
	
		// Å¸ÀÌÆ²¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_TITLELIST = PI_CS_TITLETAB,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°

		// Å¸ÀÌÆ²¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_TITLELIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iCnt;
		// GM_TITLE			stGM_Title;
		// ...
		
		// Å¸ÀÌÆ²À» »ý¼ºÇÕ´Ï´Ù.
		TOOL_CS_CREATE_TITLE,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_TITLE			stGM_Title;

		// Å¸ÀÌÆ²À» »ý¼ºÇÕ´Ï´Ù.
		TOOL_SC_CREATE_TITLE,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

		// Å¸ÀÌÆ²À» »èÁ¦ÇÕ´Ï´Ù.
		TOOL_CS_DELETE_TITLE,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_TITLE			stGM_Title;

		// Å¸ÀÌÆ²À» »èÁ¦ÇÕ´Ï´Ù.
		TOOL_SC_DELETE_TITLE,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

	//////////////////////////////////////////////////////////////////////////
	//
	//	WAYPOINTTAB
	//
		PI_CS_WAYPOINTTAB = PI_CS_TOOL + 1300,
	
		// ¿þÀÌÆ÷ÀÎÆ®¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_WAYPOINTLIST = PI_CS_WAYPOINTTAB,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°

		// ¿þÀÌÆ÷ÀÎÆ®¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_WAYPOINTLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// GM_WAYPOINT		stGM_WayPoint;

		//¿þÀÌÆ÷ÀÎÆ® ÀúÀå ¼öÁ¤ ¿äÃ»ÇÕ´Ï´Ù
		TOOL_CS_WAYPOINTSAVE,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_EVENT			stGM_WayPoint

		//¿þÀÌÆ÷ÀÎÆ® ÀúÀå ¼öÁ¤ ¿äÃ»ÇÕ´Ï´Ù
		TOOL_SC_WAYPOINTSAVE,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// GM_WAYPOINT		stGM_WayPoint;

	//////////////////////////////////////////////////////////////////////////
	//
	//	ACCOUNTTAB
	//
		PI_CS_ACCOUNTTAB = PI_CS_TOOL + 1400,
	
		// GM°èÁ¤¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_ACCOUNTLIST = PI_CS_ACCOUNTTAB,

		// GM°èÁ¤¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_ACCOUNTLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iCnt;
		// GM_ACCOUNT		stGM_Account;
		// ...
		
		// GM°èÁ¤À» »ý¼ºÇÕ´Ï´Ù.
		TOOL_CS_CREATE_ACCOUNT,
		// GM_ACCOUNT		stGM_Account;

		// GM°èÁ¤À» »ý¼ºÇÕ´Ï´Ù.
		TOOL_SC_CREATE_ACCOUNT,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iGMNum;			// GM°èÁ¤¹øÈ£
		
		// GM°èÁ¤À» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_ACCOUNT,
		// GM_ACCOUNT		stGM_Account;

		// GM°èÁ¤À» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_ACCOUNT,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

	//////////////////////////////////////////////////////////////////////////
	//
	//	GMCASHSHOP
	//
		PI_CS_GMCASHSHOP = PI_CS_TOOL + 1500,

		// Åø¿¡¼­ °øÁö»çÇ×À» Åø¼­¹ö¿¡ ¼Û½Å
		TOOL_CS_CASHSHOP = PI_CS_GMCASHSHOP,
		// UINT16 worldID;					// 0(all)
		// UINT16 serverID;					// 0(all)
		// UINT8 optionFlags;				// 0: Ä³½¬¼¥ ´Ý±â 1: ¹öÀü °»½Å 2: Ä³½¬¼¥ ¿­±â 

		TOOL_SC_CASHSHOP	,
		// UINT16 worldID;					// 0(all)
		// UINT16 serverID;					// 0(all)
		// UINT8 optionFlags;				// 0: Ä³½¬¼¥ ´Ý±â 1: ¹öÀü °»½Å 2: Ä³½¬¼¥ ¿­±â 
		// UINT8 uiResult;					// ¿¡·¯ÄÚµå

	//////////////////////////////////////////////////////////////////////////
	//
	//	GUILDTAB
	//
		PI_CS_GUILDTAB = PI_CS_TOOL + 1600,
	
		// ±æµåÁ¤º¸¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_GUILD = PI_CS_GUILDTAB,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// TCHAR				szCharName[MAX_LEN_NAME_DB+1];	// Ä³¸¯ÅÍ¸í

		// ±æµåÁ¤º¸¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_GUILD,
		// GLOBAL_STRING_ID		uiErrorID;		// ¿¡·¯ÄÚµå
		// GM_GUILD				stGM_Guild;		// ±æµåÁ¤º¸

		// ±æµå¸¦ »èÁ¦ÇÕ´Ï´Ù.(±æµå¸â¹öµµ »èÁ¦ÇÕ´Ï´Ù.)
		TOOL_CS_DELETE_GUILD,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32				iGNum;			// ±æµåÀÇ DBÅ°

		// ±æµå¸¦ »èÁ¦ÇÕ´Ï´Ù.(±æµå¸â¹öµµ »èÁ¦ÇÕ´Ï´Ù.)
		TOOL_SC_DELETE_GUILD,
		// GLOBAL_STRING_ID		uiErrorID;		// ¿¡·¯ÄÚµå

		// ±æµåÁ¤º¸Áß ±æµå¸íÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_GUILD_GNAME,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32				iGNum;			// ±æµåÀÇ DBÅ°
		// TCHAR				szGName[GUILD_MAX_NAME+1];	// ±æµå¸í

		// ±æµåÁ¤º¸Áß ±æµå¸íÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_GUILD_GNAME,
		// GLOBAL_STRING_ID		uiErrorID;		// ¿¡·¯ÄÚµå

		// ±æµåÁ¤º¸Áß ±æµå½½·Î°ÇÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_GUILD_GSLOGAN,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32				iGNum;			// ±æµåÀÇ DBÅ°
		// TCHAR				szGSlogan[GUILD_MAX_SLOGAN+1];	// ±æµå½½·Î°Ç

		// ±æµåÁ¤º¸Áß ±æµå½½·Î°ÇÀ» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_GUILD_GSLOGAN,
		// GLOBAL_STRING_ID		uiErrorID;		// ¿¡·¯ÄÚµå

		// ±æµåÁ¤º¸Áß ¹¦¸ñ¾ÆÀÌµð¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_GUILD_CROPID,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32				iGNum;			// ±æµåÀÇ DBÅ°
		// UINT32				iCropID;		// ¹¦¸ñ¾ÆÀÌµð

		// ±æµåÁ¤º¸Áß ¹¦¸ñ¾ÆÀÌµð¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_GUILD_CROPID,
		// GLOBAL_STRING_ID		uiErrorID;		// ¿¡·¯ÄÚµå

		// ±æµåÁ¤º¸Áß ¹¦¸ñ¼ºÀå¿Ï·á±îÁö ³²Àº½Ã°£À» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_GUILD_REMAINTIME,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32				iGNum;			// ±æµåÀÇ DBÅ°
		// UINT32				iRemainTime;	// ¹¦¸ñ¼ºÀå¿Ï·á±îÁö ³²Àº½Ã°£

		// ±æµåÁ¤º¸Áß ¹¦¸ñ¼ºÀå¿Ï·á±îÁö ³²Àº½Ã°£À» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_GUILD_REMAINTIME,
		// GLOBAL_STRING_ID		uiErrorID;		// ¿¡·¯ÄÚµå

		// ±æµåÁ¤º¸Áß Åõ¿©µÈ ºñ·á½Ã°£À» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_GUILD_NOURISHTIME,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32				iGNum;			// ±æµåÀÇ DBÅ°
		// UINT32				iNourishTime;	// Åõ¿©µÈ ºñ·á½Ã°£

		// ±æµåÁ¤º¸Áß Åõ¿©µÈ ºñ·á½Ã°£À» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_GUILD_NOURISHTIME,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

		// ±æµåÁ¤º¸Áß ºñ·áÅõ¿©µÈ ½ÃÀÛ½Ã°£À» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_GUILD_NOURISHSTARTTIME,
		// UINT32				iWID;				// ¿ùµå¾ÆÀÌµð
		// UINT32				iGNum;				// ±æµåÀÇ DBÅ°
		// TIMESTAMP_STRUCT		stNourishStartTime;	// ºñ·áÅõ¿©µÈ ½ÃÀÛ½Ã°£

		// ±æµåÁ¤º¸Áß ºñ·áÅõ¿©µÈ ½ÃÀÛ½Ã°£À» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_GUILD_NOURISHSTARTTIME,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

		// ±æµåÁ¤º¸Áß Åõ¿©µÈ ÃËÁøÁ¦½Ã°£À» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_GUILD_HASTENSUMTIME,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32				iGNum;			// ±æµåÀÇ DBÅ°
		// UINT32				iHastenSumTime;	// Åõ¿©µÈ ÃËÁøÁ¦½Ã°£

		// ±æµåÁ¤º¸Áß Åõ¿©µÈ ÃËÁøÁ¦½Ã°£À» ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_GUILD_HASTENSUMTIME,
		// GLOBAL_STRING_ID		uiErrorID;		// ¿¡·¯ÄÚµå

		// ±æµåÁ¤º¸Áß ÀÎ´øÀÔÀå¿ë ¸ÊÀÌº¥Æ®¾ÆÀÌµð¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_GUILD_INDUNEVENTID,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32				iGNum;			// ±æµåÀÇ DBÅ°
		// UINT32				iIndunEventID;	// ÀÎ´øÀÔÀå¿ë ¸ÊÀÌº¥Æ®¾ÆÀÌµð

		// ±æµåÁ¤º¸Áß ÀÎ´øÀÔÀå¿ë ¸ÊÀÌº¥Æ®¾ÆÀÌµð¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_GUILD_INDUNEVENTID,
		// GLOBAL_STRING_ID		uiErrorID;		// ¿¡·¯ÄÚµå


		// ±æµå¸â¹ö¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_GUILDMEMBERLIST,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32				iGNum;			// ±æµåÀÇ DBÅ°

		// ±æµå¸â¹ö¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_GUILDMEMBERLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// UINT32				iCnt;
		// GM_GUILDMEMBER		stGM_GuildMember;
		// ...

		// ±æµå¸â¹ö¸¦ »èÁ¦ÇÕ´Ï´Ù.
		TOOL_CS_DELETE_GUILDMEMBER,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32				iCharID;		// PCÀÇ DBÅ°

		// ±æµå¸â¹ö¸¦ »èÁ¦ÇÕ´Ï´Ù.
		TOOL_SC_DELETE_GUILDMEMBER,
		// GLOBAL_STRING_ID		uiErrorID;		// ¿¡·¯ÄÚµå

		// ±æµå¸â¹öÁ¤º¸Áß Á÷À§¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_CS_UPDATE_GUILDMEMBER_GRADE,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32				iCharID;		// PCÀÇ DBÅ°
		// UINT8				byGrade;		// Á÷À§

		// ±æµå¸â¹öÁ¤º¸Áß Á÷À§¸¦ ¼öÁ¤ÇÕ´Ï´Ù.
		TOOL_SC_UPDATE_GUILDMEMBER_GRADE,
		// GLOBAL_STRING_ID		uiErrorID;		// ¿¡·¯ÄÚµå

		// ±æµåÅ¸ÀÔÀ» º¯°æÇÕ´Ï´Ù
		TOOL_CS_UPDATE_GUILD_GTYPE,
		// UINT32				iWID;			// ¿ùµå¾ÆÀÌµð
		// UINT32				iGNum;			// ±æµåÀÇ DBÅ°

		// ±æµåÅ¸ÀÔÀ» º¯°æÇÕ´Ï´Ù
		TOOL_SC_UPDATE_GUILD_GTYPE,
		// GLOBAL_STRING_ID		uiErrorID;		// ¿¡·¯ÄÚµå

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
		// GLOBAL_STRING_ID		uiErrorID;					// ¿¡·¯ÄÚµå
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

		//Á¶ÇÕÁ¤º¸¸¦ °¡Á®¿É´Ï´Ù
		TOOL_CS_MIXINFO = PI_CS_ABILITYTAB,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°

		//Á¶ÇÕÁ¤º¸¸¦ °¡Á®¿É´Ï´Ù
		TOOL_SC_MIXINFO,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iProfessionLv_1;//Á¶ÇÕ·¹º§
		// int				iProfessionExp_1;//Á¶ÇÕ°æÇèÄ¡

		//Á¶ÇÕÁ¤º¸¸¦ ¼öÁ¤ÇÕ´Ï´Ù
		TOOL_CS_MIXINFO_UPDATE,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°	
		// int				iCurMixLv;		// º¯°æµÈ Á¶ÇÕ·¹º§
		// int				iCurMixExp;		// º¯°æµÈ Á¶ÇÕ°æÇèÄ¡

		//Á¶ÇÕÁ¤º¸¸¦ ¼öÁ¤ÇÕ´Ï´Ù
		TOOL_SC_MIXINFO_UPDATE,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

		//PVP Á¤º¸¸¦ °¡Á®¿É´Ï´Ù
		TOOL_CS_PVPINFO,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°

		//PVP Á¤º¸¸¦ °¡Á®¿É´Ï´Ù
		TOOL_SC_PVPINFO,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// int				iPVPLv;			// PVP·¹º§
		// int				iPVPGlory;		// PVP°æÇèÄ¡
		// int				iPVPPoint;		// PVPÆ÷ÀÎÆ®

		//PVP Á¤º¸¸¦ ¼öÁ¤ÇÕ´Ï´Ù
		TOOL_CS_PVPINFO_UPDATE,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°
		// int				iPVPLv;			// PVP·¹º§
		// int				iPVPGlory;		// PVP°æÇèÄ¡
		// int				iPVPPoint;		// PVPÆ÷ÀÎÆ®

		//PVP Á¤º¸¸¦ ¼öÁ¤ÇÕ´Ï´Ù
		TOOL_SC_PVPINFO_UPDATE,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå

	//////////////////////////////////////////////////////////////////////////
	//
	//	EVENTTAB
	//
		PI_CS_EVENTTAB = PI_CS_TOOL + 1900,

		//°­Á¦ÀÌº¥Æ® ¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù
		TOOL_CS_EVENTLIST = PI_CS_EVENTTAB,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// int				iCharID;		// PCÀÇ DBÅ°

		//°­Á¦ÀÌº¥Æ® ¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù
		TOOL_SC_EVENTLIST,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// GM_EVENT		stGM_Event;

		//°­Á¦ÀÌº¥Æ® Ã¼Å© ¿©ºÎ¸¦ ¼öÁ¤ÇÕ´Ï´Ù
		TOOL_CS_EVENTCHECK,
		// int				iWID;			// ¿ùµå¾ÆÀÌµð
		// GM_EVENT		stGM_Event;

		//°­Á¦ÀÌº¥Æ® Ã¼Å© ¿©ºÎ¸¦ ¼öÁ¤ÇÕ´Ï´Ù
		TOOL_SC_EVENTCHECK,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// GM_EVENT		stGM_Event;

	//////////////////////////////////////////////////////////////////////////
	//
	//	GLOBALEVENTTAB
	//
		PI_CS_GLOBALEVENTTAB = PI_CS_TOOL + 2000,

		// ¿¹¾àµÈ Àü¿ªÀÌº¥Æ®¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_GLOBALEVENT_RESERVELIST = PI_CS_GLOBALEVENTTAB,

		// ¿¹¾àµÈ Àü¿ªÀÌº¥Æ®¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_GLOBALEVENT_RESERVELIST,
		// INT32			iCnt;
		// GLOBALEVENT		stGlobalEvent;
		// ...

		// Àû¿ëµÈ Àü¿ªÀÌº¥Æ®¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_GLOBALEVENT_APPLYLIST,

		// Àû¿ëµÈ Àü¿ªÀÌº¥Æ®¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_GLOBALEVENT_APPLYLIST,
		// INT32			iCnt;
		// GLOBALEVENT		stGlobalEvent;
		// ...

		// °ÔÀÓ¼­¹ö¿¡ ½ÇÁ¦ Àû¿ëµÈ Àü¿ªÀÌº¥Æ®¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_CS_GLOBALEVENT_REALLIST,

		// °ÔÀÓ¼­¹ö¿¡ ½ÇÁ¦ Àû¿ëµÈ Àü¿ªÀÌº¥Æ®¸®½ºÆ®¸¦ °¡Á®¿É´Ï´Ù.
		TOOL_SC_GLOBALEVENT_REALLIST,
		// UINT16			wWorldID;
		// UINT16			wServerID;
		// INT32			iCnt;
		// GLOBALEVENT		stGlobalEvent;
		// ...

		// Áö¿¥Åø·Î Àü¿ªÀÌº¥Æ®¸¦ ¿¹¾àÇÕ´Ï´Ù.
		TOOL_CS_GLOBALEVENT_RESERVE,
		// GLOBALEVENT		stGlobalEvent;

		// Áö¿¥Åø·Î Àü¿ªÀÌº¥Æ®¸¦ ¿¹¾àÇÕ´Ï´Ù.
		TOOL_SC_GLOBALEVENT_RESERVE,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// GLOBALEVENT		stGlobalEvent;

		// ¿¹¾àµÈ Àü¿ªÀÌº¥Æ®¸¦ »èÁ¦ÇÕ´Ï´Ù.
		TOOL_CS_GLOBALEVENT_DELETE_RESERVE,
		// GLOBALEVENT		stGlobalEvent;

		// ¿¹¾àµÈ Àü¿ªÀÌº¥Æ®¸¦ »èÁ¦ÇÕ´Ï´Ù.
		TOOL_SC_GLOBALEVENT_DELETE_RESERVE,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// GLOBALEVENT		stGlobalEvent;

		// Àû¿ëµÈ Àü¿ªÀÌº¥Æ®¸¦ »èÁ¦ÇÕ´Ï´Ù.
		TOOL_CS_GLOBALEVENT_DELETE_APPLY,
		// GLOBALEVENT		stGlobalEvent;

		// Àû¿ëµÈ Àü¿ªÀÌº¥Æ®¸¦ »èÁ¦ÇÕ´Ï´Ù.
		TOOL_SC_GLOBALEVENT_DELETE_APPLY,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
		// GLOBALEVENT		stGlobalEvent;

		// °ÔÀÓ¼­¹ö¿¡ ½ÇÁ¦ Àû¿ëµÈ Àü¿ªÀÌº¥Æ®¸¦ »èÁ¦ÇÕ´Ï´Ù.
		TOOL_CS_GLOBALEVENT_DELETE_REAL,
		// GLOBALEVENT		stGlobalEvent;

		// °ÔÀÓ¼­¹ö¿¡ ½ÇÁ¦ Àû¿ëµÈ Àü¿ªÀÌº¥Æ®¸¦ »èÁ¦ÇÕ´Ï´Ù.
		TOOL_SC_GLOBALEVENT_DELETE_REAL,
		// GLOBAL_STRING_ID	uiErrorID;		// ¿¡·¯ÄÚµå
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
		// UINT32			option;					// VERSION_OPTION Âü°í

		// UINT8			uiCount;
			// UINT32			uiVersion_1;		// uiCount °¹¼ö¸¸Å­ ¹Ýº¹
			// UINT8			uiFileNameLen_1;
			// TCHAR			szFileName_1[];


		AUTH_CS_LOGIN_WITH_VERSION2 ,
		// NOT SUPPORTED !!! (Don't use it)


		// ·Î±ä(¹öÀüÈ®ÀÎ) : SITE_SANDA,SITE_BUDDYBUDDY
		AUTH_CS_LOGIN_EX,
		// UINT32			option;					// VERSION_OPTION Âü°í
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
			// UINT32			uiVersion_1;		// uiCount °¹¼ö¸¸Å­ ¹Ýº¹
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

		 // Ekey machine's SN : ³¡¿¡¼­ 6ÀÚ¸®
		 AUTH_CS_EKEY_SN,
		 // TCHAR			szEKeySN[7];				// include NULL

		 // AUTH_CS_LOGIN_EX¿¡ ´ëÇÑ °á°ú 
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

		// ¿ùµå ¸ñ·Ï ¿äÃ»
		WORLD_CS_GETLIST = PI_CS_WORLD,
		// USERID		userID;

		WORLD_SC_GETLIST ,
		//UINT8			numOfWorlds;
		//WORLD_INFO	worldInfos[];

		// ¿ùµå ¼±ÅÃ°á°ú¸¦ ¼­¹ö·Î Àü¼Û, ÇØ´ç ¿ùµå³»ÀÇ Ã¤³Î »óÅÂ ¼ö½Å(WORLD_SC_CHANNELSTATUS)ÇÏ°Ô µÊ
		WORLD_CS_SELECT ,
		//WORLD_ID		worldID;

		WORLD_SC_SELECT ,
		// RESULTCODE		resultCode;			

		// ¼±ÅÃµÈ ¿ùµåÀÇ Ã¤³Î »óÅÂ Á¤º¸ ¿äÃ»
		WORLD_CS_CHANNELSTATUS ,
		//WORLD_ID			worldID;

		// ·Î±ä/·ÎºñÃ¢¿¡¼­ ¿äÃ»µÈ °æ¿ì
		WORLD_SC_CHANNELSTATUS ,
		// UINT8			numOfChannels;
		// CHANNEL_INFO		chInfo[];

		// zone¿¡¼­ Ã¤³Î º¯°æÀ» À§ÇÑ °úÁ¤Áß, Ã¤³Î ¸ñ·Ï ¿äÃ»ÀÇ °á°ú
		WORLD_SC_CHANNELSTATUS2 ,
		// UINT8			numOfChannels;
		// CHANNEL_INFO		chInfo[];
		// SERVER_ID		curChannelID;

		// Ã¤³Î ¼±ÅÃ °á°ú¸¦ ¼­¹ö·Î Àü¼Û
		CHANNEL_CS_SELECT ,
		// WORLD_ID			worldID;
		// SERVER_ID		channelID;
		// USERID			uid;
		// PASSPORT			passport;

		CHANNEL_SC_SELECT ,
		// RESULTCODE		resultCode;
		// TCHAR			ip[LEN_IP];
		// UINT16			port;

		// Æ¯Á¤ ¼­¹ö¿¡ ÀÔÀå
		WORLD_CS_ENTER ,
		// USERID			uid;
		// PASSPORT			passport;
		// WORLD_ID			worldID;
		// TCHAR			charName[LEN_NAME];
		// TCHAR			szID[LEN_ID];
		// UINT32			uiIndunID; // ÀÎ´ø¼­¹öÀÎ °æ¿ì´Â ÀÎ´øÀÎ½ºÅÏ½º¾ÆÀÌµð, °ÔÀÓ¼­¹öÀÎ °æ¿ì´Â ÀÎ´øLDT¾ÆÀÌµð

		WORLD_SC_ENTER ,
		// RESULTCODE		resultCode;

		// °ÔÀÓ¿¡¼­ ³ª°¡±â
		WORLD_CS_LEAVE ,
		// USERID			uid;
		// PASSPORT			passport;

		WORLD_SC_LEAVE ,
		// RESULTCODE		resultCode;
		
		GAME_CS_START ,
		
		// °ÔÀÓ ½ÃÀÛ!!!
		GAME_SC_START ,				
		// RESULTCODE		resultCode;
		
		// Çö ½ÃÁ¡¿¡¼­ Ã¤³Î º¯°æ ÀÛ¾÷À» ½ÃÀÛ. °ÔÀÓ ¼­¹ö´Â ÀÌ ½ÃÁ¡¿¡¼­ SaveDB ¼öÇà
		GAME_CS_CHANGE_CHANNEL ,
		// WORLD_ID			worldID;
		// SERVER_ID		channelID;

		// saveDB °á°ú Àü¼Û. Å¬¶óÀÌ¾ðÆ®´Â disconnectÇÏ°í ¸ñÀûÁö Ã¤³Î·Î connection
		GAME_SC_CHANGE_CHANNEL ,
		// GLOBAL_STRING_ID	itemID;				// itemID = 0 or 4000006 or LT_ERROR_SYSTEMERROR(= 6)
		// if(itemID == 0)
			// TCHAR			ip[LEN_IP];
			// UINT16			port;
			// USERID			userID;
			// PASSPORT			passport2;		// DestServer¿¡¼­ »õ·Î »ý¼ºµÈ passport


		// Channel in and out
		//
		// Ã¤³Î»óÅÂ(°ÔÀÓ ÇÃ·¹ÀÌ »óÅÂ)·ÎÀÇ ÀÔÀå/ÅðÀå 
		//{
			// GS·ÎÀÇ ÀÔÀå : LBS --> GS
			CHANNEL_CS_IN,
			// UINT32	uiUserID;
			// UINT32	uiReservedParam;
			CHANNEL_SC_IN,
			// GLOBAL_STRING_ID	itemID;			

			// GS¿¡¼­ÀÇ ÅðÀå : GS --> LS [,kickout,disconnect,serverdown]
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

		// Ä³¸¯ÅÍ »ý¼ºÀÛ¾÷Áß °¡µî·ÏµÈ Ä³¸¯ÅÍ¸í »èÁ¦
		CHAR_CS_DELETE_CHARNAME = PI_CS_CHAR,
		// TCHAR			charName[LEN_NAME];

		CHAR_SC_DELETE_CHARNAME ,
		// RESULTCODE		resultCode;

		// Ä³¸¯ÅÍ »ý¼º°úÁ¤Áß, Ä³¸¯ÅÍ ÀÌ¸§ Áßº¹ °Ë»ç
		CHAR_CS_CHECK_CHARNAME ,
		// WORLD_ID			worldID;
		// TCHAR			szID[LEN_ID];
		// TCHAR			szNewCharName[LEN_NAME];

		CHAR_SC_CHECK_CHARNAME ,
		// RESULTCODE		resultCode;					// LT_NO_ERROR(»ç¿ë°¡´É), LT_ERROR_FAILED(Ä³¸¯ÅÍ¸í Áßº¹)

		// ÇöÀç ÇØ´ç À¯ÀúÀÇ Ä³¸¯ÅÍ ¸ñ·ÏÀ» ¿äÃ»ÇÑ´Ù.
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
			// UINT8			byNewbi;		// Ä³¸¯ÅÍ »ý¼ºÈÄ ÃÖÃÊ ÁøÀÔ ¿©ºÎ

		// ÇØ´ç ¿ùµå³»¿¡ »õ·Î¿î Ä³¸¯ÅÍ¸¦ ÇÏ³ª »ý¼ºÇÑ´Ù.
		CHAR_CS_CREATE ,
		// WORLD_ID			worldID;
		// CHARACTER_INFO	charInfo;
		// CHAR_FIGURE		charFigure;

		CHAR_SC_CREATE ,
		// RESULTCODE		resultCode;
		// CHARID			charID;

		// ÇØ´ç Ä³¸¯ÅÍ¸¦ »èÁ¦ÇÑ´Ù.
		CHAR_CS_DELETE ,
		// WORLD_ID			worldID;
		// USERID			userID;
		// CHARID			charID;
		// TCHAR			szCharName[LEN_NAME+1];

		CHAR_SC_DELETE ,
		// GLOBAL_STRING_ID	iErrorMsg;		// ¿¡·¯ÄÚµå

		// Æ¯Á¤ Ä³¸¯ÅÍ¸¦ ¼±ÅÃÇÏ°í °ÔÀÓ ½ÃÀÛ.
		CHAR_CS_SELECT ,
		// USERID			userID;
		// CHARID			charID;
		// SERVER_ID		channelID;		// Á¢¼ÓÇÒ Ã¤³ÎID

		CHAR_SC_SELECT ,	// °ÔÀÓ ½ÃÀÛ
		// RESULTCODE			resultCode;
		// CHARACTER_INFO		charInfo;
		// LT_POSITION			posInfo;
		// STATUS_INFO			statusInfo;
		// MAP_INFO				mapInfo;
		// ITEMMIX_INFO			ItemMixInfo;
		// PVP_STAT				PvPInfo;
		// CHARACTER_CASH_INFO charCashInfo; //mijin 20080306 Ä³½ÃÃß°¡

		// Æ¯Á¤ Ä³¸¯ÅÍÀÇ Á¤º¸ ¾ò±â
		CHAR_CS_CHARINFO,
		// CHARID			charID;				// for PC/NPC/MOB

		CHAR_SC_CHARINFO ,
		// RESULTCODE		resultCode;
		// GU_ID			globalID;
		// CHARACTER_INFO2	charInfo2;

		// Æ¯Á¤ Ä³¸¯ÅÍ À§Ä¡Á¤º¸ ¾ò±â
		CHAR_CS_POSINFO ,
		// CHARID			charID;				// for PC/NPC/MOB

		CHAR_SC_POSINFO ,
		// RESULTCODE		resultCode;
		// MAP_INFO			mapInfo;
		// LT_POSITION		posInfo;

		// Æ¯Á¤ Ä³¸¯ÅÍ ½ºÅÈ Á¤º¸ ¾ò±â
		CHAR_CS_STATUSINFO ,       // »ç¿ë ¾ÈµÇ°í ÀÖÀ½ »èÁ¦¿¹Á¤ÀÓ
		// CHARID			charID;				

		CHAR_SC_STATUSINFO ,		// »ç¿ë ¾ÈµÇ°í ÀÕÀ½ »èÁ¦¿¹Á¤ÀÓ
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
	
		// Æ¯Á¤ Ä³¸¯ÅÍ ÀÌµ¿°ü·Ã Status Á¤º¸ ¾ò±â
		CHAR_CS_MOVING_STATUS_INFO ,

		CHAR_SC_MOVING_STATUS_INFO ,
		// GU_ID			globalID
		// MOVING_STAUS		MovingStatusInfo;

		// Ä³¸¯ÅÍÀÇ µð·º¼Ç Á¤º¸ ¿äÃ».
		CHAR_CS_FACING				,
		// UINT8			facing

		// Ä³¸¯ÅÍÀÇ µð·º¼Ç Á¤º¸ Àü¼Û.
		CHAR_SC_FACING				,
		// GU_ID			globalID
		// FACING			facing

		// Ä³¸¯ÅÍÀÇ ÄÁÅ×ÀÌ³Ê Á¤º¸.
		CHAR_SC_CONTAINER_INFOS		,
		// GU_ID			globalID		//
		// INT8				iNumOfInfo		// º¯°æµÉ Á¤º¸¼¼Æ® °¹¼ö
		// CONTAINER_TYPE	ContainerType	// ÄÁÅ×ÀÌ³Ê Å¸ÀÔ.		(º¯°æµÉ Á¤º¸¼¼Æ®)
		// INT8				TotalSlot		// ÃÑ°¹¼ö (=  ±âº» + OptionSlotTot )
		// INT8				OptionSlotTot	// ¿É¼Ç ½½·ÔÀÇ ÃÑ°¹¼ö.(º¯°æµÉ Á¤º¸¼¼Æ®)

		// Ä³¸¯ÅÍÀÇ ÄÁÅ×ÀÌ³Ê Á¤º¸.
		CHAR_CS_CONTAINER_INFOS		,

		CHAR_SC_EFFECT_STATUS_INFO ,
		// GU_ID				globalID		
		// EFFECT_STATUS_INFO	StatusInfo;

		// µÇ»ì¾Æ³ª±â ¿äÃ»
		CHAR_CS_REVIVAL,
		// SAVEPOINT_TYPE			iSavePointType; 
		// ITEM_NO					iTrashItemNo;
		// TRASH_ITEM				TrashItem;

		// ¾ÆÀÌÅÛ Á¶ÇÕ ¼÷·Ãµµ º¯°æ½Ã
		CHAR_SC_ITEMMIX_INFO,
		// GU_ID				globalID;
		// ITEMMIX_INFO			stItemMixInfo;

		// Ä³¸¯ÅÍ Å¬·¡½ºº¯°æ
		CHAR_CS_CHANGE_CLASS ,
		// int				iChangeClassID ;	// CLASS_CHANGE.ldt::_Item_ID
		// TRASH_ITEM		stTrashItem;		// ¼Ò¸ð¾ÆÀÌÅÛ

		// Ä³¸¯ÅÍ Å¬·¡½ºº¯°æ °á°ú
		CHAR_SC_CHANGE_CLASS ,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	iErrorID;		// ¿¡·¯¾ÆÀÌµð,  0 ÀÎ°æ¿ì¸¸ ÀÌÇÏ µ¥ÀÌÅÍ¼Û½Å
		// UINT8			NewClassType;
		// __int64			iCurrEly;		// µ· 
		// short			iSkillFreePoint;// SKILL_SC_INIT¿Í µ¿ÀÏÇÑ ÀÌº¥Æ® Ã³¸®¿ä¸Á
		// UINT8			iNumOfLevelUpChange;
		// STATUS_CHANGE    tStatPack[];	// ¹Ù²ð ´É·ÂÄ¡ 	
		// STATUS_INFO		statInfo ;
		// UINT8			iNumOfItem;		// ÀüÁ÷ÇÔÀ¸·Î ÇØ¼­ »ý¼ºµÇ´Â ¾ÆÀÌÅÛ
		// CONTAINER_ITEM	aryContainerItem[];
		// TRASH_ITEM		stTrashItem;

		// PVP µÇ»ì¾Æ³ª±â ¿äÃ»
		CHAR_CS_PVP_REVIVAL,

		//	 Íæ¼ÒµÄGM×´Ì¬
		CHAR_SC_GM_STATUS,
		//	INT  byIsGM;			0Îª·ÇGM £¬´óÓÚ0ÎªGM

	//////////////////////////////////////////////////////////////////////////////
	//
	// Status (LevelUp,Exp,hp,sp,...) 
	//
	PI_CS_STAT = PI_CS_LOGIC + 300,

		// º¯°æµÈ status °ªÀ» Àü¼Û
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
	// Game Options : °ÔÀÓ ¼­¹ö·ÎÀÇ ·Î±ä½Ã DBLoad µÇ°í, ·Î±×¾Æ¿ô½Ã DBSaveµÊ
	//
		PI_CS_GAME_OPTION = PI_CS_LOGIC + 400,

		// PCº° °ÔÀÓ ¿É¼Ç ¿äÃ». *** ÇöÀç »ç¿ëÇÏÁö ¾Ê½À´Ï´Ù. CHARACTER_INFO2 ±¸Á¶Ã¼¿¡ Æ÷ÇÔµÇ¾î¼­ °ÔÀÓ¼­¹ö ÁøÀÔ½Ã¿Í ÀÎ»çÀÌÆ®/¾Æ¿ô»çÀÌÆ®½Ã Å¬¶óÀÌ¾ðÆ®¿¡ ¼Û½ÅÇÕ´Ï´Ù.
		GAME_CS_GETOPTION = PI_CS_GAME_OPTION,
		GAME_SC_GETOPTION ,
		// GLOBAL_STRING_ID	ErrorID;
		// UINT16			uiGameOption;		// ±Í¼Ó¸»,Ä£±¸Ãß°¡,°Å·¡¿äÃ»,ÆÀ¿äÃ»,»óÅÂ¾Ë¸²			

		// PCº° °ÔÀÓ ¿É¼Ç ¼³Á¤
		GAME_CS_SETOPTION ,
		// INT32			iGameOptions;
		GAME_SC_SETOPTION ,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
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
		// GU_ID			globalID;	// ÇÇ°ÝÀÚ
		// int				iDamage;		// damage value
		// int				iMode		// 0:ÀÏ¹Ý 1:Å©¸® 2: ¹Ì½º 3: Immune
		// int				iType		// EFFECT.LDT::_StatusEffectType
		// GU_ID			AttackerGlobalID;	// °ø°ÝÀÚ
		// uint8			iHitCount			// Combo HitCount

		BATTLE_SC_HEALING,
		// RESULTCODE		resultCode;
		// GU_ID			globalID;
		// int				iHeal;		// damage value
		// int				iMode		// 0:ÀÏ¹Ý 1:Å©¸® 2: ¹Ì½º 3: Immune
		// int				iType		// EFFECT.LDT::_StatusEffectType

	//////////////////////////////////////////////////////////////////////////////
	//
	// Navigation	(GameServer)
	//
		PI_CS_NAV = PI_CS_LOGIC + 700,

		// Æ¯Á¤ Ä³¸¯ÅÍ°¡ ½Ã¾ß¿¡ µé¾î¿Ô´Ù.
		NAV_SC_INSIGHT = PI_CS_NAV,

		//±âº»Á¤º¸
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

		// °³ÀÎ»óÁ¡¿ÀÇÂÁßÀÎ »óÅÂÀÏ¶§¸¸ »óÁ¡¸íÀ» º¸³À´Ï´Ù.
		// char				szFMName[FM_MAX_NAME+1]; // »óÁ¡¸í

		// ±æµåÁ¤º¸
		// packet.PushBack(&stGuildBroadCast, sizeof(GUILD_BROADCAST));

		// PC ÀÎ°æ¿ì
		//UINT8		iGloryLv;

		// ´ëÈ­¹æ ¿ÀºêÁ§Æ® ÀÎ °æ¿ì.. 2008.04.14 Hoony
		// GU_ID					ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// BG_ID					Layer;
		// UINT						iPosX;						//
		// UINT						iPosY;						//
		// CHATROOM_OBJECT_INFO		stChatRoomObject_Info		//

		// Æ¯Á¤ Ä³¸¯ÅÍ°¡ ½Ã¾ß¿¡¼­ ¹þ¾î³µ´Ù.
		NAV_SC_OUTOFSIGHT ,
		// GU_ID			globalID;			// classID + instanceID


		//	·þÎñÆ÷Ïò¿Í»§¶Ë·¢ËÍ½øÈëÊÓÒ°µÄÈËÎïµÄguid
		NAV_SC_INSIGHT_GUID,
		//  UINT8 uCount;
		//  UINT32 insight_ID[];

		//	¿Í»§¶Ë¸ù¾Ý·þÎñÆ÷¶Ë·¢ËÍµÄ½øÈëÊÓÒ°µÄÈËÎïguidÀ´·ÖÅúÈ¡ÈËÎïÐÅÏ¢
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
		// MAP_INFO			mapInfo;			// for Portal		 (ÇöÀç Ã¤³Î³»·Î ±¹ÇÑµÊ)
		// float			posX;				// for Portal	
		// float			posY;				// for Portal	

		// EVENT_SC_RAISE -> Client -> Client:;LoadStage() ¿Ï·áÈÄ, ¼­¹ö·Î Àü¼ÛµÊ.
		NAV_CS_ENTERSTAGE ,

	//////////////////////////////////////////////////////////////////////////
	//
	// Item 
	//
		PI_CS_ITEM = PI_CS_LOGIC + 900,

		// ÄÁÅ×ÀÌ³Ê ³»¿¡¼­ ´ÜÀÏ ½½·ÔÀÇ ¾ÆÀÌÅÛ Á¤º¸ °»½Å
		ITEM_SC_CONTAINER_INFO  = PI_CS_ITEM,	
		// GU_ID				globalID;
		// CONTAINER_ITEM		containerItem;

		// ÄÁÅ×ÀÌ³Ê ³»¿¡¼­ º¹¼ö ½½·ÔÀÇ ¾ÆÀÌÅÛ Á¤º¸ °»½Å
		ITEM_SC_CONTAINER_INFOS  ,
		// GU_ID				globalID;
		// UINT16				nItems;
		// CONTAINER_ITEM		containerItem;

		// ¾ÆÀÌÅÛ ÁÝ±â ÀÌµ¿ ¿äÃ»
		ITEM_CS_LOOTING ,
		// GU_ID				iLootingBagID;
		// LOOTING_ITEM			stLooting_Item;

		// ¾ÆÀÌÅÛ ÁÝ±â ¼º°ø. (ÇØ´ç ¸÷ ÀÎº¥ÀÇ ½½·ÔÀ» Å¬¸®¾î ÇØÁØ´Ù.)
		ITEM_SC_LOOTING , // ºê·Îµå Ä³½ºÆÃµµ ÇØ¾ßÇÔ.			
		// GU_ID				globalID;
		// GLOBAL_STRING_ID		ErrorID;		// ¿¡·¯¾ÆÀÌµð
		// GU_ID				ActCharGUID;	// ¾ÆÀÌÅÛÀ» ½ÀµæÇÑ PCÀÇ GUID
		// LOOTING_ITEM			stLooting_Item;	// Å¬¶óÀÌ¾ðÆ® ±¸Á¶ÀÇ ÆíÀÇ¼ºÀ» À§ÇÏ¿© ¾ÆÀÌÅÛÁ¤º¸¸¦ ÇÑ¹ø ´õ ³Ö¾îÁÝ´Ï´Ù. <-- ¸Þ¼¼Áö¿ë
		// UINT8				byRare;			// ¸Þ¼¼Áö¿¡ »ö»óÇ¥½Ã¸¦ À§ÇØ¼­ ·¹¾îµµ¸¦ Ãß°¡ÇÕ´Ï´Ù.
		// LOOTING_ITEM			stLooting_Item;

		// ÄÁÅ×ÀÌ³Ê³»¿¡¼­ÀÇ ¾ÆÀÌÅÛ À§Ä¡ ÀÌµ¿ ¿äÃ».
		ITEM_CS_CHANGE_SLOT ,	
		// CHANGE_SLOT			changeSlot;

		// ÄÁÅ×ÀÌ³Ê³»¿¡¼­ÀÇ ¾ÆÀÌÅÛ À§Ä¡ ÀÌµ¿ ¿äÃ».
		ITEM_SC_CHANGE_SLOT ,	
		// GU_ID				globalID;
		// CHANGE_SLOT			changeSlot;

		// ¾ÆÀÌÅÛ ¹ö¸®±â ¿äÃ».
		ITEM_CS_TRASH ,
		//TRASH_ITEM			trashItem;

		// ¾ÆÀÌÅÛ ÆÄ±« ¼º°ø ( ÇØ´ç ÀÎº¥ÀÇ ½½·ÔÀ» Å¬¸®¾î ÇØÁÜ)
		ITEM_SC_TRASH ,			
		// GU_ID				globalID;
		// UINT8				iNumOfTrashItem;
		// TRASH_ITEM			trashItem*iNumOfTrashItem;

		// ¾ÆÀÌÅÛ ÇàÀ§¿¡ ´ëÇÑ ¸Þ¼¼Áö. 
		ITEM_SC_ALARM , // »ç¿ë ¾ÈÇÔ
		//// GU_ID				globalID;
		//// EXCEPTION_CODE		exceptionCode;

		// ¾ÆÀÌÅÛ »ç¿ë
		ITEM_CS_USING ,
		// GU_ID				Å¸°Ù ¿£Æ¼Æ¼ ¾ÆÀÌµð
		// CONTAINER_SLOT		À§Ä¡Á¤º¸
	
		// »ç¿ë ¼º°ø. Å¬¶ó¿¬Æ®¿¡¼­ ÇØ´ç ¾ÆÀÌÅÛ °¡°¨.
		ITEM_SC_USING ,
		// GU_ID				globalID	; // Å¸°Ù ¿£Æ¼Æ¼ ¾ÆÀÌµð
		// GLOBAL_STRING_ID		iError		; // 0 : ¼º°ø
		// TRASH_ITEM			UsingItem	; // »ç¿ë µÈ ¾ÆÀÌÅÛ À§Ä¡ Á¤º¸

		// ¿ÜºÎ¿¡ º¸¿©¾ß ÇÏ´Â ¾ÆÀÌÅÛµéÀÇ ºê·Îµå Ä³½ºÆÃ Á¤º¸
		ITEM_SC_FIGURE_INFOS  ,	// ¿ÜÇüÁ¤º¸
		ITEM_SC_FIGURE_ON,				// ¿ÜÇü 
		ITEM_SC_FIGURE_OFF,				// ¿ÜÇü ÇØÁ¦(Àåºñ½Ã ÇÊ¿ä)
		
		ITEM_CS_TOGGLE_USE_WEAPON_SET,  // ¹«±â swap ¿äÃ».
		ITEM_SC_TOGGLE_USE_WEAPON_SET,	// ¹«±â swap ¼º°ø.
		//GU_ID				globalID	;
		//RESULT_CODE iError 
		//UINT8 uiActiveSet);

		ITEM_SC_LOOTING_LIST,           // ·çÆÃ ¾ÆÀÌÅÛ Á¤º¸¸¦ º¸³¿. 
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

		// »óÅÂº¸±â¿ë.
		ITEM_CS_FIGURE_INFOS_FOR_SINGLE,
		// GU_ID			iGlobalID				// TargetÀ¯ÀúÀÇ GU_ID 
		// INT8				iNumOfCONTAINER_TYPE	// º¸°í½ÍÀº ÄÁÅ×ÀÌ³Ê¹øÈ£ÀÇ °¹¼ö.						//
		// CONTAINER_TYPE	iContainerType[] 		// ÄÁÅ×ÀÌ³Ê Å¸ÀÔ.
			
		// ´Ù¸¥ À¯ÀúÀÇ ¾ÆÀÌÅÛ Á¤º¸º¸±â
		ITEM_SC_FIGURE_INFOS_FOR_SINGLE,
		// GU_ID			iGUID					// À¯ÀúÀÇ GU_ID
		// GU_ID			iTargetGUID				// Å¸°ÙÀ¯ÀúÀÇ GU_ID
		// UINT16			iNumOfContainerItem		// ÄÁÅ×ÀÌ³Ê ¾ÆÀÌÅÛ °¹¼ö		
		// CONTAINER_ITEM	iContainerItems[]		// Å¸°ÙÀ¯ÀúÀÇ ÄÁÅ×ÀÌ³Ê ¾ÆÀÌÅÛ.

		// ½Å±Ô »õ·Î¿î ÀÎ½ºÅÏ½º ¾ÆÀÌÅÛÀ» ¸¸µé¾î ³½´Ù.
		ITEM_CS_DIVIDE,
		// CHANGE_SLOT		changeSlot				// ÇØ´ç ¾ÆÀÌÅÛÀÇ À§Ä¡ ¹× °¹¼öÁ¤º¸/

		ITEM_SC_DIVIDE,
		// GU_ID			iGUID					// À¯ÀúÀÇ GU_ID
		// GLOBAL_STRING_ID	iResultError			// 0: ¼º°ø 0º¸´Ù Å©¸é ¿¡·¯.
		// TRASH_ITEM		TrashITem				// ¼Ò½ºÀÇ Ä«¿îÆÃ »è°¨ Á¤º¸.
		// CONTAINER_ITEM	NewItem					// »õ·Î »ý¼ºµÈ ÀÎ½ºÅÏ½º ÄÁÅ×ÀÌ³Ê ¾ÆÀÌÅÛ.

		ITEM_CS_ENCHANT,
		// int				iEnchantID;				// ITEM_ENCHANT.LDT::PrimaryKey
		//----------------------------------------------------------
		// CONTAINER_TYPE	iMainContainerType;			// ¸ÞÀÎ
		// UINT8			iMainContainerSlot;			// ¸ÞÀÎ
		// ITEMNO			iMainItemNo;				// ¸ÞÀÎ

		// CONTAINER_TYPE	iMaterialContainerType;		// Àç·á ¾ÆÀÌÅÛ À§Ä¡  
		// UINT8			iMaterialContainerSlot;		// Àç·á ¾ÆÀÌÅÛ À§Ä¡
		// ITEMNO			iMaterialItemNo;			// Àç·á ¾ÆÀÌÅÛ ¹øÈ£
		// UINT8			iMaterialItemCount;			// Àç·á ¾ÆÀÌÅÛ ¿ä±¸°¹¼ö.  -- ÂüÁ¶¿ë
		//-------------------------------------------------------------

		ITEM_SC_ENCHANT,
		// GU_ID			iGUID;					// À¯ÀúÀÇ GU_ID
		// UINT8			iEnchanted;				// 0: ÀÎÃ¾Æ® Á¶°Ç ½Ç»— 1: ÀÎÃ¾Æ®
		// GLOBAL_STRING_ID	iResultError;			// iEnchanted°¡ 1 ÀÎ°æ¿ì 0: ¼º°ø 0º¸´Ù Å©¸é ²Î.  iEnchanted°¡ 0ÀÎ °æ¿ì ÀÌ°ªÀÌ ¹«Á¶°Ç 0º¸´Ù Å«°ªÀ¸·Î ¿Í¾ßÇÔ.
													// ¾ÆÀÌÅÛ Á¤º¸°»½Å¿ëÀº µû·Î (ITEM_SC_CONTAINER_INFOS/ITEM_SC_TRASH) µîÀÇ ÆÐÅ¶À¸·Î Àü´Þ.
		// int				iEnchantID;	


		ITEM_CS_UPGRADE,
		// int				iUpgradeID;				// ITEM_ENCHANT.LDT::PrimaryKey
		//----------------------------------------------------------
		// CONTAINER_TYPE	iMainContainerType;			// ¸ÞÀÎ
		// UINT8			iMainContainerSlot;			// ¸ÞÀÎ
		// ITEMNO			iMainItemNo;				// ¸ÞÀÎ
		//----------------------------------------------------------------
		// unsigned INT8	iNumOfMaterial; // ÃÖ´ë 4
		// MATERIAL_DATA	MaterialData[iNumOfMaterial]; // iNumOfMaterial ¸¸Å­ ºÙ¿©ÁÖ¼¼¿ä	
		//-------------------------------------------------------------
		// ITEMNO			iResultItemID;				// °á°ú ¾ÆÀÌÅÛ			-- ÂüÁ¶¿ë
		// 

		ITEM_SC_UPGRADE,
		// GU_ID			iGUID;					/// À¯ÀúÀÇ GU_ID
		// UIN8				iUpgraded;				/// 0: ¾÷±×·¹ÀÌµå Á¶°Ç ½Ç»— 1: ¾÷±×·¹ÀÌµå
		// GLOBAL_STRING_ID	iResultError;			/// iUpgraded°¡ 1 ÀÎ°æ¿ì 0: ¼º°ø 0º¸´Ù Å©¸é ²Î.  iUpgraded°¡ 0ÀÎ °æ¿ì ÀÌ°ªÀÌ ¹«Á¶°Ç 0º¸´Ù Å«°ªÀ¸·Î ¿Í¾ßÇÔ./ 0: ¼º°ø 0º¸´Ù Å©¸é ¿¡·¯.  // ¾ÆÀÌÅÛ Á¤º¸°»½Å¿ëÀº µû·Î (ITEM_SC_CONTAINER_INFOS/ITEM_SC_TRASH) µîÀÇ ÆÐÅ¶À¸·Î Àü´Þ.
		// int				iUpgradeID;						// ¾ÆÀÌÅÛ Á¤º¸°»½Å¿ëÀº µû·Î (ITEM_SC_CONTAINER_INFOS/ITEM_SC_TRASH) µîÀÇ ÆÐÅ¶À¸·Î Àü´Þ.

		ITEM_CS_DESTROY_BY_TIME ,
		// ITEMNO			iItemNo;				
		// CONTAINER_SLOT	ContainerInfo;

		ITEM_SC_DESTROY_BY_TIME ,
		//GU_ID					iGUID;	
		//TRASH_ITEM			trashItem;

		ITEM_CS_BASIC_FIGURE_CHANGE,
		// int					iBeautyShopID;		// BEAUTY_SHOP.LDT::ItemID #
		// UINT8				iBasicFigureSlot;	//FIGURE_BASIC_TYPE Áß ÇÏ³ª 
		// CONTAINER_SLOT		UsingItemPos1;  // »ç¿ëµÈÄíÆù¾ÆÀÌÅÛÀ§Ä¡
		// ITEMNO				iUsingItemNo1;  // »ç¿ëµÈÄíÆù¾ÆÀÌÅÛÅ°
		// CONTAINER_SLOT		UsingItemPos2;  // »ç¿ëµÈÄíÆù¾ÆÀÌÅÛÀ§Ä¡
		// ITEMNO				iUsingItemNo2;  // »ç¿ëµÈÄíÆù¾ÆÀÌÅÛÅ°

		ITEM_SC_BASIC_FIGURE_CHANGE,  // ÁÖº¯ÀÎµé¿¡°Ô´Â ITEM_SC_FIGURE_ON 
		//GU_ID					iGUID;
		//GLOBAL_STRING_ID		iResultError;	
		//int					iBeautyShopID;	// º¯Çü
		//TRASH_ITEM			UsingItemPos1;	// »ç¿ëµÈÄíÆù¾ÆÀÌÅÛÀ§Ä¡
		//ITEMNO				iUsingItemNo1;	// »ç¿ëµÈÄíÆù¾ÆÀÌÅÛÅ°
		//TRASH_ITEM			UsingItemPos2;	// »ç¿ëµÈÄíÆù¾ÆÀÌÅÛÀ§Ä¡
		//ITEMNO				iUsingItemNo2;	// »ç¿ëµÈÄíÆù¾ÆÀÌÅÛÅ°
		//CONTAINER_ITEM		NewCharItem;	// ¼öÁ¤ÈÄ Á¤º¸

		ITEM_SC_EXPIRE,
		//GU_ID					iGUID;	
		//CONTAINER_SLOT		stItemPos;

		ITEM_CS_ADDTIME,
		// int				iItemAddTimeID;				// ITEM_ADD_TIME.LDT::PrimaryKey
		//----------------------------------------------------------
		// CONTAINER_TYPE	iMainContainerType;			// ¸ÞÀÎ
		// UINT8			iMainContainerSlot;			// ¸ÞÀÎ
		// ITEMNO			iMainItemNo;				// ¸ÞÀÎ

		// CONTAINER_TYPE	iMaterialContainerType;		// Àç·á ¾ÆÀÌÅÛ À§Ä¡  
		// UINT8			iMaterialContainerSlot;		// Àç·á ¾ÆÀÌÅÛ À§Ä¡
		// ITEMNO			iMaterialItemNo;			// Àç·á ¾ÆÀÌÅÛ ¹øÈ£
		// UINT8			iMaterialItemCount;			// Àç·á ¾ÆÀÌÅÛ ¿ä±¸°¹¼ö.  -- ÂüÁ¶¿ë
		//-------------------------------------------------------------

		ITEM_SC_ADDTIME,
		// GU_ID			iGUID;					// À¯ÀúÀÇ GU_ID
		// int				iItemAddTimeID;	
		// GLOBAL_STRING_ID	iResultError;			// 0: ¼º°ø 0º¸´Ù Å©¸é ²Î. ¾ÆÀÌÅÛ Á¤º¸°»½Å¿ëÀº µû·Î (ITEM_SC_TRASH) µîÀÇ ÆÐÅ¶À¸·Î Àü´Þ.
		// ITEM_LIFETIME	stItemLifeTime;			// ¼º°ø½Ã¿¡¸¸ À¯È¿ÇÑ º¯°æµÈ ¶óÀÌÇÁÅ¸ÀÓÁ¤º¸

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
		// CONTAINER_SLOT		UsingItemPos;		// »ç¿ëÇÒ ¾ÆÀÌÅÛ À§Ä¡Á¤º¸
		// ITEMNO				iUsingItemNo;		// »ç¿ëÇÒ ¾ÆÀÌÅÛ Å°¹øÈ£
		// BROADCAST_SCOPE		scope;				// ºê·ÎµåÄ³½ºÆÃ ¹üÀ§
		// UINT8				len;				// ¸Þ¼¼Áö ±æÀÌ
		// TCHAR				szMsg[];	

		// »ç¿ë ¼º°ø. Å¬¶ó¿¬Æ®¿¡¼­ ÇØ´ç ¾ÆÀÌÅÛ °¡°¨. // ½ÇÁ¦Ã¤ÆÃÀº CHAT_CS_MSG ·Î ³¯¶ó°¨
		ITEM_SC_CHAT,
		// GU_ID				globalID	; // »ç¿ëÀÚ ¾ÆÀÌµð
		// GLOBAL_STRING_ID		iError		; // 0 : ¼º°ø
		// TRASH_ITEM			UsingItem	; // »ç¿ë µÈ ¾ÆÀÌÅÛ À§Ä¡ Á¤º¸
		
		// Á¶ÇÕ
		ITEM_CS_MIX,
		// int				iMixID;				// ITEM_CREATE_RECIPE.LDT::PrimaryKey
		//----------------------------------------------------------------
		// UINT8			iNumOfMaterial; // ÃÖ´ë 5
		// MATERIAL_DATA	MaterialData[iNumOfMaterial]; // iNumOfMaterial ¸¸Å­ ºÙ¿©ÁÖ¼¼¿ä	

		// Á¶ÇÕ°á°ú
		ITEM_SC_MIX,
		// GU_ID			iGUID;					/// À¯ÀúÀÇ GU_ID
		// GLOBAL_STRING_ID	iResultError;			/// 0: ¼º°ø 0º¸´Ù Å©¸é ¿¡·¯. 
		// UIN8				iMixed;					/// 0: È®·ü½ÇÆÐ 1: È®·ü¼º°ø
		// int				iMixID;
		// ITEMMIX_INFO		stItemMixInfo;			// Ä³¸¯ÅÍÀÇ Á¶ÇÕÁ¤º¸
		// UINT8			iNumOfMeterial;
		// TRASH_ITEM		stMeterial[];
		// CONTAINER_ITEM	stResult;				// °á°ú¾ÆÀÌÅÛ

		//	Ê¹ÓÃµã¾íÁÄÌì [2008-07-17]
		MONEY_CS_CHAT,
		//	CONTAINER_SLOT		UsingItemPos;		// »ç¿ëÇÒ ¾ÆÀÌÅÛ À§Ä¡Á¤º¸
		//	ITEMNO				iUsingItemNo;		// »ç¿ëÇÒ ¾ÆÀÌÅÛ Å°¹øÈ£
		//	BROADCAST_SCOPE		scope;				//	¹ã²¥µÄ·¶Î§
		//	ITEMID				iItemID;			//	¶ÔÓ¦µÄÎïÆ·µÄID
		//  UINT8				len;				//  ÏûÏ¢³¤¶È
		//  TCHAR				szMsg[];			//	ÏûÏ¢µÄÄÚÈÝ

		//	ÊÇ·ñ³É¹¦	[2008-07-17]
		MONEY_SC_CHAT,	
		// GU_ID				globalID	; Íæ¼ÒµÄGUID
		// GLOBAL_STRING_ID		iError		; ¶ÔÓ¦µÄÏûÏ¢µÄID
		// TRASH_ITEM			UsingItem	; // »ç¿ë µÈ ¾ÆÀÌÅÛ À§Ä¡ Á¤º¸
		
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
		// GU_ID				Å¸°Ù ¿£Æ¼Æ¼ ¾ÆÀÌµð
		// CONTAINER_SLOT		À§Ä¡Á¤º¸
		// INT32	SvrSeq
		// UINT32	Seq
		// TCHAR	szTargetCharName[LEN_NAME+1];

		ITEM_SC_WHERE,
		// GLOBAL_STRING_ID		iError;					// ´íÎóID
		// INT32			SvrSeq
		// UINT32			Seq
		// UINT8			bFlag;						// Íæ¼ÒÊÇ·ñÔÚÏß£¬1ÎªÔÚÏß£¬0Îª²»ÔÚÏß
		// TCHAR			szCharName[LEN_NAME+1];
		// TCHAR			szTargetCharName[LEN_NAME+1];
		// UINT16			ServerID;
		// UINT				uiStageID;					// ½ºÅ×ÀÌÁö¾ÆÀÌµð
		// UINT				uiMapGroupID;				// ¸Ê±×·ì¾ÆÀÌµð
		// UINT				uiPosX;						// XÁÂÇ¥
		// UINT				uiPosY;						// YÁÂÇ¥

		ITEM_SC_WHERE_COMMAMD_INFOS,
		// int nWhereItemCount;
		// WHERE_ITEMINFO stInfo[11];
		///add by chenhao 08.9.2

		ITEM_CS_MSGBOARD,
		// GU_ID
		// CONTAINER_SLOT
		// INT				iPosX;
		// INT				iPosY;
		// UINT				uiType;						//  ÏûÏ¢°æÀàÐÍ
		// UINT8			len;						//  ÏûÏ¢³¤¶È
		// TCHAR			szMsg[];					//	ÏûÏ¢µÄÄÚÈÝ

		ITEM_SC_MSGBOARD,
		// GLOBAL_STRING_ID		iError;					// ´íÎóID

		ITEM_CS_DEL_MSGBOARD,
		// GU_ID										//µÀ¾ßid
		// CONTAINER_SLOT
		// GU_ID										//ÏûÏ¢°æid

		ITEM_SC_DEL_MSGBOARD,
		// GLOBAL_STRING_ID		iError;					// ´íÎóID
		
		ITEM_CS_TRACK,									//¸úËæ£¬×¥ÈËµÀ¾ß
		// GU_ID
		// CONTAINER_SLOT
		// TCHAR			szTargetCharName[LEN_NAME+1];

		ITEM_SC_TRACK,
		// GU_ID
		// GLOBAL_STRING_ID		iError;					// ´íÎóID


	//////////////////////////////////////////////////////////////////////////
	//
	//	NPC
	// 
		PI_CS_NPC = PI_CS_LOGIC + 1000,

		// NPC¿¡°Ô ÆÈ±â. PC -> NPC
		NPC_SHOP_CS_SELL = PI_CS_NPC, 
		// int				iNpcID 
		// int				iShopID
		// CONTAINTER_ITEM tContainerItem;

		// NPC¿¡°Ô »ç±â.	NPC -> PC
		NPC_SHOP_CS_BUY,  
		// int			iNpcID;			 // ´©±¸¿¡°Ô¼­ »ê°ÍÀÎ°¡
		// int			iShopID;		 // Shop ID 
		// UINT8		iSlotIndex;      // ¾î´ÀÄ­¿¡ ÀÖ´Â ¾ÆÀÌÅÛÀ» »ì°ÍÀÎ°¡.
		// ITEM			tItem;			 // ITEMID / COUNT  ¾î¶² ¾ÆÀÌÅÛÀ» ¸î°³ »ì°ÍÀÎ°¡.
	
		// 2005.11.03 dhpark NPC¿¡°Ô »ç±â °á°ú. »óÅÂÁ¦¾î ¶§¹®¿¡ Ãß°¡ÇÕ´Ï´Ù.
		NPC_SHOP_SC_BUY,  
		// GU_ID			ObjectGUID;	// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	iErrorID;	// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// ÄÆÀÎ ¿­±â
		CUTIN_CS_OPEN,
		// int				iNpcID;		// NPC

		// ÄÆÀÎ ¿­±â
		CUTIN_SC_OPEN,
		// GU_ID			ObjectGUID;	// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð

		// ÄÆÀÎ ´Ý±â
		CUTIN_CS_CLOSE,
		// int				iNpcID;		// NPC

		// ÄÆÀÎ ´Ý±â
		CUTIN_SC_CLOSE,
		// GU_ID			ObjectGUID;	// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð

		// Npc PORTAL
		NPC_CS_PORTAL,
		// int            iNpcID;			// ¿£ÇÇ½Ã
		// int			  iPortalGroupID;	// Æ÷Å» ±×·ìÀÇ Key ID NPCPORTAL.LDT::_ItemID
		// int			  iPotalIndex;		// À§Ä¡Á¤º¸
		// int			  iPortalInfoID;    // PORTAL_DETAIL.LDT::_ItemID
		// CONTAINER_SLOT stUsingItemPos;	// ¼Ò¸ð¾ÆÀÌÅÛ
		// ITEMNO		  iUsingItemNo;		// ¼Ò¸ð¾ÆÀÌÅÛ No
		// int			  iUsingEly;		// ¼Ò¸ðµ·

		NPC_SC_PORTAL,
		// GU_ID			iPCGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID iErrorID;		// ¼º°ø 0 
		// UINT64			iCurrEly;       // ½ÇÁ¦ Æ÷Å»Àº EVENT_SC_RAISE
		// TRASH_ITEM		stUsingItem;

		// NPC º¸»ó ½Ã½ºÅÛ ¿äÃ»
		NPC_CS_REWARD,
		// int            iNpcID;			// ¿£ÇÇ½Ã
		// int			  iRewardGroupID;		  
		// UINT8		  iRewardIndex;		// ·£´ý ¹× ¸ðµÎ º¸»óÀÏ°æ¿ì´ÂÀÇ¹Ì¾øÀ½
		// UINT8		  iNumOfUsingItem;
		// TRASH_ITEM	  stReqItem;		// ¼Ò¸ð¾ÆÀÌÅÛ
		// ITEMNO	      iReqItemNo;		// ¼Ò¸ð¾ÆÀÌÅÛ No
		// int			  iUsingEly;		// ¼Ò¸ðµ·
		// int			  iUsingPVPPoint;	// ¼Ò¸ð	PVP Point mijin 20071129

		// NPC º¸»ó ½Ã½ºÅÛ °á°ú
		NPC_SC_REWARD,
		// GU_ID			iPCGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID iErrorID;		// ¼º°ø 0 
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

		// Ã¢°í¿¡ °ÔÀÓ¸Ó´Ï º¸°üÇÏ±â
		STORAGE_CS_INMONEY = PI_CS_STORAGE,
		// UINT64			uiMoney;		// º¸°üÇÏ·Á´Â ±Ý¾×

		// Ã¢°í¿¡ °ÔÀÓ¸Ó´Ï º¸°üÇÏ±â
		STORAGE_SC_INMONEY,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT64			uiMoney;		// º¯°æÈÄ ¾Æ¹ÙÅ¸°¡ °¡Áö°í ÀÖ´Â ±Ý¾×
		// UINT64			uiStorageMoney;	// º¯°æÈÄ Ã¢°í¿¡ ÀúÀåµÈ ±Ý¾×

		// Ã¢°í¿¡¼­ °ÔÀÓ¸Ó´Ï °¡Á®¿À±â
		STORAGE_CS_OUTMONEY,
		// UINT64			uiMoney;		// ²¨³»·Á°íÇÏ´Â ±Ý¾×

		// Ã¢°í¿¡¼­ °ÔÀÓ¸Ó´Ï °¡Á®¿À±â
		STORAGE_SC_OUTMONEY,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT64			uiMoney;		// º¯°æÈÄ ¾Æ¹ÙÅ¸°¡ °¡Áö°í ÀÖ´Â ±Ý¾×
		// UINT64			uiStorageMoney;	// º¯°æÈÄ Ã¢°í¿¡ ÀúÀåµÈ ±Ý¾×

	//////////////////////////////////////////////////////////////////////////////
	//
	// Chatting : ÃÖ´ë MAX_CHATMSG_LEN ±æÀÌÀÇ ¸Þ¼¼Áö¸¦ Àü¼Û
	//
		PI_CS_CHAT = PI_CS_LOGIC + 1200,

		// Ã¤ÆÃ¸Þ¼¼Áö
		CHAT_CS_MSG	= PI_CS_CHAT,
		// BROADCAST_SCOPE	scope;					// ºê·ÎµåÄ³½ºÆÃ ¹üÀ§
		// UINT32			destination;			// ´ë»ó(partyID,channelID,worldID). if ¼­¹ö°£ Àü¼Û½Ã, HIWORD(worldID),LOWORD(serverID)
		// UINT8			len;					// ¸Þ¼¼Áö ±æÀÌ
		// TCHAR			szMsg[];				// ¸Þ¼¼Áö
		// UINT8			nReceiverName;				// SCOPE_WHISPERÀÏ¶§¸¸ »ç¿ëµÊ
		// TCHAR			szReceiverName[LEN_NAME];	// SCOPE_WHISPERÀÏ¶§¸¸ »ç¿ëµÊ, »ó´ë¹æ Ä³¸¯ÅÍÁ¤º¸

		//	GLOBAL_STRING_ID ¿¹: ±Ó¼Ó¸» Àü¼Û ¿À·ù½Ã º°µµÀÇ system Message·Î ¹Ý¼Û : 000´ÔÀ» Ã£À» ¼ö ¾ø½À´Ï´Ù.
		CHAT_SC_MSG ,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT »óÀÇ ¸Þ¼¼Áö¸¦ itemID °ª
		// BROADCAST_SCOPE	scope;				// ºê·ÎµåÄ³½ºÆÃ ¹üÀ§
		// UINT8			uiFontColor;		// ÆùÆ®»ö	: 0(default),1(Red),2(Orange),3(Yellow),4(white) : CHAT_COLOR.LDT::ItemID
		// UINT8			uiOptions;			// reserved for FontSize,FontType
		// UINT8			len;
		// TCHAR			szMsg[];		
		// GU_ID			guidSender;			// ¼Û½ÅÀÚ Á¤º¸
		// UINT8			nSenderName;
		// TCHAR			szSender[];			

		// ¼ö½ÅÀÚ Á¤º¸(SCOPE_WHISPER only)
		//{
			// GU_ID			guidReceiver;
			// UINT8			nReceiverName;			
			// TCHAR			szReceiver[];
		//}

		// (SCOPE_WORLD ONLY)
		// UINT16			ChannelID;			// Ã¤³Î ID	(Cash-Item¿¡ ÀÇÇÑ ¿ùµå ¿ÜÄ¡±â½Ã Ã¤³Î Á¤º¸ Ç¥½Ã¿ë)

		// (SCOPE_TEAM ONLY)
		// INT32			iSenderCharID;		// ÆÀÃ¤ÆÃÀ» º¸³½ Ä³¸¯ÅÍDBÅ°(¸»Ç³¼±À» º¸¿©ÁÖ±â À§ÇØ¼­ Ãß°¡)

		CHAT_SC_MSG_MONEY,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT »óÀÇ ¸Þ¼¼Áö¸¦ itemID °ª
		// BROADCAST_SCOPE	scope;				// ºê·ÎµåÄ³½ºÆÃ ¹üÀ§
		// UINT8			uiFontColor;		// ÆùÆ®»ö	: 0(default),1(Red),2(Orange),3(Yellow),4(white) : CHAT_COLOR.LDT::ItemID
		// UINT8			uiOptions;			// reserved for FontSize,FontType
		// ITEMID			useitemID
		// UINT8			len;
		// TCHAR			szMsg[];		
		// GU_ID			guidSender;			// ¼Û½ÅÀÚ Á¤º¸
		// UINT8			nSenderName;
		// TCHAR			szSender[];
		// UINT16			ChannelID;			// Ã¤³Î ID	(Cash-Item¿¡ ÀÇÇÑ ¿ùµå ¿ÜÄ¡±â½Ã Ã¤³Î Á¤º¸ Ç¥½Ã¿ë)

		CHAT_SC_MSG_PCINFO,		
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT »óÀÇ ¸Þ¼¼Áö¸¦ itemID °ª
		// BROADCAST_SCOPE	scope;				// ºê·ÎµåÄ³½ºÆÃ ¹üÀ§
		// UINT8			uiFontColor;		// ÆùÆ®»ö	: 0(default),1(Red),2(Orange),3(Yellow),4(white) : CHAT_COLOR.LDT::ItemID
		// UINT8			uiOptions;			// reserved for FontSize,FontType
		// ITEMID			useitemID			// Íæ¼ÒÊ¹ÓÃµÄÎïÆ·
		// CHARACTER_INFO2  info;				//	Íæ¼ÒµÄAVATAÐÅÏ¢
		// UINT				uItemCount;			//	ÏÂÃæÁÐ³öµÄÎïÆ·µÄ×ÜÊý
		// FIGURE_ITEM      tFigureItemList[MAX_FIGURE_SIZE + MAX_EQUIP_SIZE + MAX_FASHION_SIZE] ; //Íæ¼ÒµÄAvatarÐÅÏ¢
		// UINT8			len;
		// TCHAR			szMsg[];

	//////////////////////////////////////////////////////////////////////////////
	// Notice
	//
		PI_CS_NOTICE = PI_CS_LOGIC + 1300,

		// °øÁö ¸Þ¼¼Áö Ãâ·Â. ¼­¹ö´Ù¿î,ÀüÃ¼ °øÁö....
		MSG_CS_NOTICE = PI_CS_NOTICE,
		// UINT8			displayTime;		// 0(forever), sec
		// UINT8			optionFlags;		// reserved. display È¿°ú ¿ëµµ(Ä¯¹ÚÀÓ,Ä®¶ó,...)
		// UINT16			len;
		// TCHAR			szMsg[];

		MSG_SC_NOTICE ,
		// RESULTCODE		resultCode;
		// UINT8			displayTime;		// 0(forever), sec
		// UINT8			optionFlags;		// reserved. display È¿°ú ¿ëµµ(Ä¯¹ÚÀÓ,Ä®¶ó,...)
		// UINT16			len;
		// TCHAR			szMsg[];

	//////////////////////////////////////////////////////////////////////////
	//
	// Trade
	//
		PI_CS_TRADE = PI_CS_LOGIC + 1400,

		// °Å·¡¿äÃ»
		TRADE_CS_REQUEST = PI_CS_TRADE,
		// GU_ID			DstCharGUID;				// °Å·¡´ë»óÀÚ

		// °Å·¡¿äÃ» °á°ú
		TRADE_SC_REQUEST,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GU_ID			SrcCharGUID;				// °Å·¡¿äÃ»ÀÚ
		// char				szSrcCharName[LEN_NAME+1];	// °Å·¡¿äÃ»ÀÚÀÌ¸§
		// GU_ID			DstCharGUID;				// °Å·¡´ë»óÀÚ
		// char				szDstCharName[LEN_NAME+1];	// °Å·¡´ë»óÀÚÀÌ¸§
		// GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// °Å·¡´ë»óÀÚÀÇ ¼ö¶ô¿©ºÎ
		TRADE_CS_RESPONSE,
		// UINT8			bAccept;					// ¼ö¶ô/°ÅÀý

		TRADE_SC_RESPONSE,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GU_ID			SrcCharGUID;				// °Å·¡¿äÃ»ÀÚ
		// char				szSrcCharName[LEN_NAME+1];	// °Å·¡¿äÃ»ÀÚÀÌ¸§
		// GU_ID			DstCharGUID;				// °Å·¡´ë»óÀÚ
		// char				szDstCharName[LEN_NAME+1];	// °Å·¡´ë»óÀÚÀÌ¸§
		// UINT8			byAccept;					// ¼ö¶ô/°ÅÀý
		// GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// µ·À» °Å·¡Ã¢¿¡ ¿Ã¸°´Ù.
		TRADE_CS_PUSHMONEY,
		// UINT64	uiMoney;							// ¿Ã¸° °ÔÀÓ¸Ó´Ï
		// int		iCash;								// 20080312 mijin ¿Ã¸° Ä³½¬
		// int		iMile;								// 20080312 mijin ¿Ã¸° ¸¶ÀÏ¸®Áö
		// int		iCashComm;							// 20080312 mijin Ä³½¬ ¼ö¼ö·á
		// int		iMileComm;							// 20080312 mijin ¸¶ÀÏ¸®Áö ¼ö¼ö·á


		// µ·À» ¿Ã¸° PCÀÇ º¸À¯±Ý¾×°ú ºñ±³ÇÏ¿© ¼º°øÀÌ¸é µÎ ¾Æ¹ÙÅ¸¿¡°Ô ¸ðµÎ ¼Û½ÅÇÑ´Ù.
		TRADE_SC_PUSHMONEY,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GU_ID			ActCharGUID;				// µ·À» ¿Ã¸° ¾Æ¹ÙÅ¸ÀÇ Å°
		// UINT64			uiMoney;					// ¿Ã¸° µ·
		// int				iCash;						// 20080312 mijin ¿Ã¸° Ä³½¬
		// int				iMile;						// 20080312 mijin ¿Ã¸° ¸¶ÀÏ¸®Áö
		// int				iCashComm;					// 20080312 mijin Ä³½¬ ¼ö¼ö·á
		// int				iMileComm;					// 20080312 mijin ¸¶ÀÏ¸®Áö ¼ö¼ö·á


		// ¾ÆÀÌÅÛÀ» °Å·¡Ã¢¿¡ ¿Ã¸°´Ù.
		TRADE_CS_PUSHITEM,
		// TRADE_ITEM		stTradeItem;				// ¾ÆÀÌÅÛÀ» ¿Ã¸° À¯ÀúÀÇ ÄÁÅ×ÀÌ³Ê,½½·Ô,Æ®·¹ÀÌµå½½·Ô Á¤º¸°¡ µé¾îÀÖ´Â ±¸Á¶Ã¼

		// °Å·¡ÁßÀÎÁö, ¾ÆÀÌÅÛÀÌ °Å·¡°¡´ÉÇÑ°ÇÁö, ÇØ´ç ½½·ÔÀÌ ¾ÆÀÌÅÛÀÌ ÀÌ¹Ì ÀÖ´ÂÁö µîµîÀ» °Ë»çÇÑÈÄ ¼º°øÀÌ¸é º¸³»°í ½ÇÆÐ¸é ¿À·ù·Î±×
		TRADE_SC_PUSHITEM,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GU_ID			ActCharGUID;				// ¾ÆÀÌÅÛÀ» ¿Ã¸° ¾Æ¹ÙÅ¸Å°
		// TRADE_ITEM		stTradeItem;				// ¾ÆÀÌÅÛÀ» ¿Ã¸° À¯ÀúÀÇ ÄÁÅ×ÀÌ³Ê,½½·Ô,Æ®·¹ÀÌµå½½·Ô Á¤º¸°¡ µé¾îÀÖ´Â ±¸Á¶Ã¼
		// ITEM				stItem;						// ¾ÆÀÌÅÛ¿¡ ´ëÇÑ »ó¼¼Á¤º¸¸¦ º¸³»Áà¼­ ¸¶¿ì½º¸¦ ¾ÆÀÌÅÛ¿¡ À§Ä¡½ÃÅ°¸é Á¤º¸¸¦ º¸¿©ÁÙ¼öÀÖµµ·ÏÇÔ

		// ¾ÆÀÌÅÛÀ» °Å·¡Ã¢¿¡¼­ ³»¸°´Ù. Accept»óÅÂ¸¦ ÇØÁ¦ÇÔ. 
		TRADE_CS_POPITEM,
		// SLOT_INDEX		TradeSlotIndex;				// °Å·¡½½·ÔÀÇ ÀÎµ¦½º

		// Á¶°ÇÀ» °Ë»çÇÑÈÄ ¼º°øÀÌ¸é º¸³»°í ½ÇÆÐ¸é ¿À·ù·Î±×
		TRADE_SC_POPITEM,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GU_ID			ActCharGUID;				// ¾ÆÀÌÅÛÀ» ³»¸° ¾Æ¹ÙÅ¸Å°
		// SLOT_INDEX		TradeSlotIndex;				// °Å·¡½½·ÔÀÇ ÀÎµ¦½º

		// »ó´ë¹æÀÌ ÀÌ¹Ì acceptÇÑ »óÅÂ¸é °Å·¡¿Ï·á, ¾Æ´Ï¸é accept»óÅÂ
		TRADE_CS_ACCEPT,

		// »ó´ë¹æÀÇ »óÅÂ¸¦ °Ë»çÇÏ¿© accept»óÅÂ·Î º¯°æÇÏ°Å³ª °Å·¡¿Ï·á¸¦ ÇÕ´Ï´Ù.
		TRADE_SC_ACCEPT,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GU_ID			ActCharGUID;				// accept¸¦ ´©¸¥ ¾Æ¹ÙÅ¸Å°
		// UINT8			byFinish;					// °Å·¡¿Ï·á¿©ºÎ
		// GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
		// GU_ID			ErrCharGUID;				// ¿À·ùÁ¦°øÀÚ ¾Æ¹ÙÅ¸Å°

		// °Å·¡¸¦ Ãë¼ÒÇÕ´Ï´Ù.
		TRADE_CS_CANCEL,

		// °Å·¡Ãë¼Ò¸¦ µÎPC¿¡°Ô ¼Û½ÅÇÕ´Ï´Ù. °Å·¡°ü¸®ÀÚ¿¡¼­ °Å·¡¿ÀºêÁ§Æ® »èÁ¦
		TRADE_SC_CANCEL,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GU_ID			ActCharGUID;				// °Å·¡¸¦ Ãë¼ÒÇÑ ¾Æ¹ÙÅ¸Å°

		// »ó´ë¹æÀÌ ÀÌ¹Ì WAITÇÑ »óÅÂ¸é °Å·¡´ë±â¿Ï·á¸ðµå·Î, ¾Æ´Ï¸é WAIT»óÅÂ
		TRADE_CS_WAIT,

		// »ó´ë¹æÀÇ »óÅÂ¸¦ °Ë»çÇÏ¿© WAIT»óÅÂ·Î º¯°æÇÏ°Å³ª °Å·¡´ë±â¿Ï·á¸ðµå·Î ÀüÈ¯ÇÕ´Ï´Ù.
		TRADE_SC_WAIT,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GU_ID			ActCharGUID;				// WAIT¸¦ ´©¸¥ ¾Æ¹ÙÅ¸Å°
		// UINT8			byAllWait;					// °Å·¡´ë±â¿Ï·á¿©ºÎ

	//////////////////////////////////////////////////////////////////////////
	//
	// Mission 
	//
		PI_CS_MISSION = PI_CS_LOGIC + 1500,

		// ¹Ì¼Ç ¸®½ºÆ®
		MISSION_SC_LIST = PI_CS_MISSION,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// int				iCnt;			// ¹Ì¼Ç °³¼ö
		// MISSION			stMission;		// ¹Ì¼Ç ³»¿ë
		// MISSION			stMission;		// ¹Ì¼Ç ³»¿ë
		// MISSION			stMission;		// ¹Ì¼Ç ³»¿ë
		// ...								// ¹Ì¼Ç °³¼ö¸¸Å­

		// ¹Ì¼Ç º¯°æ
		MISSION_SC_UPDATE,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// MISSION			stMission;		// ¹Ì¼Ç Á¤º¸

		// ¾×¼ÇÇü¹Ì¼Ç ¼º°ø
		MISSION_CS_ACTION,
		// UINT32			uiActionID;		// ¾×¼Ç¾ÆÀÌµð

		// ¾×¼ÇÇü¹Ì¼Ç ¼º°ø°á°ú
		MISSION_SC_ACTION,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT32			uiActionID;		// ¾×¼Ç¾ÆÀÌµð

	//////////////////////////////////////////////////////////////////////////
	//
	// Quest 
	//
		PI_CS_QUEST = PI_CS_LOGIC + 1600,

		// Äù½ºÆ® ¸®½ºÆ®
		QUEST_SC_LIST =  PI_CS_QUEST,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// int				iCnt;			// Äù½ºÆ® °³¼ö
		// QUEST			stQuest;		// Äù½ºÆ® ³»¿ë
		// QUEST			stQuest;		// Äù½ºÆ® ³»¿ë
		// QUEST			stQuest;		// Äù½ºÆ® ³»¿ë
		// ...								// Äù½ºÆ® °³¼ö¸¸Å­
			
		// Äù½ºÆ® ¼ö¶ô. 2007.09.19 dhpark Äù½ºÆ®¼ö¶ôÁ¾·ùÁõ°¡¿¡ µû¸¥ ÆÐÅ¶±¸Á¶º¯°æ.
		QUEST_CS_ACCEPT,
		// UINT8			byAcceptType;	// Äù½ºÆ® ¼ö¶ô Å¸ÀÔ(1:NPC, 2:°øÀ¯, 4:¾ÆÀÌÅÛ, 8:ÀÌº¥Æ®)
		// INT32			iValue;			// Å¸ÀÔ¿¡ µû¸¥ °Ë»ç¿ë¾ÆÀÌµð(NPC¾ÆÀÌµð, °øÀ¯ÇÑ Ä³¸¯ÅÍDBÅ°, µîµî)
		// UINT32			uiQuestID;		// Äù½ºÆ®¾ÆÀÌµð

		// Äù½ºÆ® ¼ö¶ô. 2007.10.01 dhpark Å¬¶óÀÌ¾ðÆ®ÀÇ ¼ö¶ô°á°ú ±¸ºÐÀ» À§ÇÏ¿© ¼ö¶ôÅ¸ÀÔ¼Û½Å
		QUEST_SC_ACCEPT,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;		// ¿¡·¯¾ÆÀÌµð
		// QUEST			stQuest;		// Äù½ºÆ® Á¤º¸
		// UINT8			byAcceptType;	// Äù½ºÆ® ¼ö¶ô Å¸ÀÔ(1:NPC, 2:°øÀ¯, 4:¾ÆÀÌÅÛ, 8:ÀÌº¥Æ®)

		// Äù½ºÆ® Æ÷±â
		QUEST_CS_CANCEL,
		// UINT32			uiQuestID;		// Äù½ºÆ®¾ÆÀÌµð
			
		// Äù½ºÆ® Æ÷±â
		QUEST_SC_CANCEL,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT32			uiQuestID;		// Äù½ºÆ®¾ÆÀÌµð

		// Äù½ºÆ® ¿Ï·á
		QUEST_CS_FINISH,
		// int				iNpcID;			// NPC¾ÆÀÌµð
		// UINT32			uiQuestID;		// Äù½ºÆ®¾ÆÀÌµð
		// UINT8			byRank;			// 0, 1, 2
		// UINT8			byReward;		// ¼±ÅÃÇüÀÏ¶§ À¯Àú°¡ ¼±ÅÃÇÑ º¸»óÀÎµ¦½º(0, 1, 2)
			
		// Äù½ºÆ® ¼ö¶ô
		QUEST_SC_FINISH,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;		// ¿¡·¯¾ÆÀÌµð
		// QUEST			stQuest;		// Äù½ºÆ® Á¤º¸

		// Äù½ºÆ® °øÀ¯
		QUEST_CS_SHARE,
		// UINT32			uiQuestID;		// Äù½ºÆ®¾ÆÀÌµð
			
		// Äù½ºÆ® °øÀ¯(°øÀ¯ÇÑ Ä³¸¯ÅÍ¿ë)
		QUEST_SC_SHARE,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð
		// char				szRcvCharName[LEN_NAME+1];	// °øÀ¯¹Þ´Â Ä³¸¯ÅÍ¸í
			
		// Äù½ºÆ® °øÀ¯(°øÀ¯¹Þ´Â Ä³¸¯ÅÍ¿ë)
		QUEST_SC_SHARE_RECV,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT32			uiQuestID;		// Äù½ºÆ®¾ÆÀÌµð
		// INT32			iActCharID;		// °øÀ¯ÇÑ Ä³¸¯ÅÍDBÅ°(QUEST_CS_ACCEPT¸¦ º¸³¾¶§ »ç¿ëÇØ¾ßÇÕ´Ï´Ù.)

	//////////////////////////////////////////////////////////////////////////
	//
	//	Ä¿¹Â´ÏÆ¼ : Ä£±¸ ¸ñ·Ï, Â÷´Ü ¸ñ·Ï, ...
	//
		PI_CS_COMMUNITY = PI_CS_LOGIC + 1700,

		// Ä£±¸ ¸ñ·Ï
		FRIEND_CS_GETLIST = PI_CS_COMMUNITY,
		FRIEND_SC_GETLIST,
		// UINT8			nCount;
		// FRIEND_INFO		friendInfo[];
		// UINT8			uiRefreshPeriodSec;		// 0(disable)

		// Ä£±¸ Ãß°¡ ¿äÃ»
		FRIEND_CS_ADD,
		// UINT8			len;
		// TCHAR			szCharName[];

		// Ä£±¸ Ãß°¡ ¿äÃ» °á°ú
		FRIEND_SC_ADD, 
		// GLOBAL_STRING_ID	itemID;					// GLOBAL_STRING.LDT »óÀÇ ¸Þ¼¼Áö¸¦ itemID °ª
		// UINT8			len;
		// TCHAR			szAccepter[];

		// Ä£±¸ »èÁ¦
		FRIEND_CS_DELETE,
		// UINT8			len;
		// TCHAR			szCharName[];
		FRIEND_SC_DELETE,
		// GLOBAL_STRING_ID	itemID;					// GLOBAL_STRING.LDT »óÀÇ ¸Þ¼¼Áö¸¦ itemID °ª

		// ÇØ´ç Ä³¸¯ÅÍÀÇ »óÅÂ(ON/OFF LINE,Level) °»½Å ¿äÃ»
		FRIEND_CS_REFRESH_STATUS,
		// UINT8			nCount;
		// TCHAR			charNames[][LEN_NAME];
		FRIEND_SC_REFRESH_STATUS,
		// UINT8			uiRefreshPeriodSec;		// 1<= uiNextRequestSec<=255.  // 0(disable)
		// UINT8			nCount;
		// FRIEND_INFO		friendInfo[];

		// Â÷´Ü ¸ñ·Ï
		BLOCK_CS_GETLIST,
		BLOCK_SC_GETLIST,
		// UINT8			nCount;
		// TCHAR			szCharName[][LEN_NAME];

		//Â÷´Ü ¸ñ·Ï¿¡ Ãß°¡
		BLOCK_CS_ADD,
		// UINT8			len;
		// TCHAR			szCharName[];
		BLOCK_SC_ADD,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT »óÀÇ ¸Þ¼¼Áö¸¦ itemID °ª
		// UINT8			len;
		// TCHAR			szCharName[];

		// Â÷´Ü ¸ñ·Ï¿¡¼­ »èÁ¦
		BLOCK_CS_DELETE,
		// UINT8			len;
		// TCHAR			szCharName[];
		BLOCK_SC_DELETE,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT »óÀÇ ¸Þ¼¼Áö¸¦ itemID °ª

		// »ó´ë(szTargetName)ÀÇ Â÷´Ü ¸ñ·Ï»ó¿¡ ³»°¡ µî·ÏµÇ¾ú´Â°¡?
		BLOCK_CS_ISBLOCKED,
		// UINT8			len;
		// TCHAR			szTargetName[];
		BLOCK_SC_ISBLOCKED,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT »óÀÇ ¸Þ¼¼Áö¸¦ itemID °ª
		// GU_ID			guid;				// ¿äÃ»ÇÑ ´ç»çÀÚÀÇ GU_ID
		// bool				bWedding;			// °áÈ¥Á¤º¸°¡ ÀÖÀ»¶§ ¹Ø¿¡ Á¤º¸¸¦ °°ÀÌ º¸³½´Ù
		// UINT8			iWeddingLv;			// °áÈ¥·¹º§
		// int				iWeddingDay;		// ±³Á¦ÀÏ¼ö ¶Ç´Â °áÈ¥ÀÏ¼ö
		// TCHAR			szWeddingPartName[LEN_NAME+1]; //°áÈ¥»ó´ë¹æÀÌ¸§

		// ¼ö½ÅÀÚ¿¡°Ô Ä£±¸Ãß°¡ ¿äÃ»ÀÌ µé¾î¿ÓÀ»¶§
		FRIEND_SC_ADD_RECV,
		// GLOBAL_STRING_ID iErrorMsg;
		// USERID			iRequesterUserID;
		// CHARID			iRequesterCharID;
		// UINT8			iLen;
		// TCHAR			szRequesterName[];
	
		// 
		FRIEND_CS_ACCEPT,
		// UINT8			iFlag;				// 0: Ãë¼Ò 1: °ÅºÎ 2: µ¿ÀÇ
		// USERID			iRequesterUserID;
		// CHARID			iRequesterCharID;
		// UINT8			iLen;
		// TCHAR			szRequesterName[];

		FRIEND_SC_ACCEPT,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT »óÀÇ ¸Þ¼¼Áö¸¦ itemID °ª
		// FRIEND_INFO		friendInfo;			// 

		// ÇØ´ç Ä³¸¯ÅÍÀÇ »óÅÂ(ON/OFF LINE,Level) °»½Å ¿äÃ»
		FRIEND_CS_REFRESH_STATUS2,
		// UINT8			nCount;
		// UINT32			uiFriendCharIDs[];

	//////////////////////////////////////////////////////////////////////////
	//
	//	SKILL
	//
		PI_CS_SKILL = PI_CS_LOGIC + 1800,

		// À¯Àú°¡ ¼ÒÀ¯ÇÑ ½ºÅ³ Á¤º¸.
		SKILL_SC_INFOS = PI_CS_SKILL, 
		// GU_ID			iInstanceID
		// unsigned short	iNumOfSkill
		// sizeof(CHAR_SKILL)*iNumOfSkill

		// ½ºÅ³Æ÷ÀÎÆ®¸¦ È¯¿øÇÏ°í ÃÊ±âÈ­.
		SKILL_SC_INIT, 
		// GU_ID			iInstanceID
		// short			iSkillFreePoint;	

		// ½ºÅ³ »ç¿ë.
		SKILL_CS_USING, 
		// int iSkillID // ½ºÅ³ ¾ÆÀÌµð.
		// UINT8 facing

		// ½ºÅ³ »ç¿ë °á°ú.
		SKILL_SC_USING,
		// RESULTCODE		resultCode;
		// GU_ID			iInstanceID;
		// UINT32			iSkillID;
		// LT_POSITION		pos;

		// ¾ÆÀÌÅÛ½ºÅ³ »ç¿ë.
		ITEMSKILL_CS_USING, 
		// CONTAINER_SLOT	stItemPos;	// ¾ÆÀÌÅÛÁ¤º¸
		// int				iSkillID;	// ½ºÅ³ ¾ÆÀÌµð.
		// UINT8			facing;

		// ¾ÆÀÌÅÛ½ºÅ³ »ç¿ë °á°ú.
		ITEMSKILL_SC_USING,
		// GU_ID			iInstanceID;
		// GLOBAL_STRING_ID	ErrorID;		// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
		// ITEM_SP			stItemSP;		// ¾ÆÀÌÅÛÁ¤º¸ / °»½Å¿ë SPÁ¤º¸
		// int				iSkillID;
		// LT_POSITION		stPCPos;

		// ½ºÅ³¿¡ ÇÁ¸®Æ÷ÀÎÆ®¸¦ ¹èºÐÇÏ¿© ·¹º§¾÷À» ÇÑ´Ù.
		SKILL_CS_LVUP,
		// int iSkillID // ½ºÅ³ ¾ÆÀÌµð.

		SKILL_SC_LVUP,
		// RESULTCODE		resultCode;
		// GU_ID			iInstanceID;
		// short			iSkillFreePoint;
		// if(0 == resultCode)
			// CHAR_SKILL		SkillData;

		// Åä±Û½ºÅ³¿ÀÇÁ, °á°ú´Â EFFECT_SC_DEL·Î º¸³À´Ï´Ù.
		SKILL_CS_OFFTOGGLE, 
		// int iSkillID // ½ºÅ³ ¾ÆÀÌµð.

		// ¾ÆÀÌÅÛ½ºÅ³À» ÀÌ¿ëÇÑ ¾ÆÀÌÅÛÆÇ¸Å. °á°ú´Â STAT_SC_CHANGE_EX, ITEM_SC_TRASH ÆÐÅ¶À¸·Î Å¬¶óÀÌ¾ðÆ®¿¡ ¼Û½ÅÇÕ´Ï´Ù.(ÇöÀç ¾ÆÀÌÅÛ»óÁ¡ÆÇ¸Å ±¸Á¶¿Í µ¿ÀÏ)
		ITEMSKILL_CS_SELL, 
		// CONTAINER_SLOT	stSkillItemPos;	// »óÁ¡ÆÇ¸ÅÆ¯¼ö½ºÅ³À» °¡Áö°í ÀÖ´Â ÀåÂøÇÑ¾ÆÀÌÅÛÁ¤º¸
		// int				iSkillID;		// »óÁ¡ÆÇ¸Å ¼Ó¼ºÀ» °¡Áö°í ÀÖ´Â ½ºÅ³¹øÈ£
		// CONTAINER_ITEM	stSellItem;		// ÆÇ¸ÅÇÒ ¾ÆÀÌÅÛÁ¤º¸

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

		// ÇöÀç Char¿¡°É¸° ÀÌÆÑÆ®Á¤º¸.
		EFFECT_SC_INFOS = PI_CS_EFFECT,
		// GU_ID			iInstanceID; // pc
		// GU_ID			iInstanceID; // Caster
		// UINT16			iNumOfEffect;
		// sizeof(EFFECT_DATA)*iNumOfEffect;

		// ÇöÀç Char¿¡ °É¸° ÀÌÆÑÆ® Á¤º¸¸¦ Áö¿ì±â.
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

		// ÆÀÃÊ´ë
		TEAM_CS_REQUEST = PI_CS_TEAM,
		// char				szRcvCharName[LEN_NAME+1];	// ´ë»óÀÚÀÌ¸§

		// ÆÀÃÊ´ë °á°ú. ¿¡·¯ÀÎ°æ¿ì´Â ¿äÃ»ÀÚ¿¡°Ô¸¸ ¼Û½Å, ¼º°øÀÎ°æ¿ì´Â ´ë»óÀÚ¿¡°Ô¸¸ ¼Û½Å
		TEAM_SC_REQUEST,	// ¿äÃ»ÀÚ¿Í ´ë»óÀÚ¿¡°Ô ¼Û½Å
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// char				szCharName[LEN_NAME+1];		// ÀÌ¸§
		// GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// ÆÀÂü¿©ÀÀ´ä
		TEAM_CS_JOIN,
		// char				szActCharName[LEN_NAME+1];	// ¿äÃ»ÀÚÀÌ¸§

		// ÆÀÂü¿©ÀÀ´ä °á°ú
		TEAM_SC_JOIN,		// ½ÇÆÐ¸é ´ë»óÀÚ¿¡°Ô ¼Û½Å, ¼º°øÀÌ¸é ¸ðµç ÆÀ¿ø¿¡°Ô ¼Û½Å
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// char				szActCharName[LEN_NAME+1];	// ¿äÃ»ÀÚÀÌ¸§
		// GU_ID			RcvCharGUID;				// ´ë»óÀÚ
		// char				szRcvCharName[LEN_NAME+1];	// ´ë»óÀÚÀÌ¸§
		// GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
		// UINT8			byLootingRule;				// ·çÆÃ·ê(0:Â÷·Ê´ë·Î,1:·£´ý,2:ÀÚÀ¯)
		// int				iCnt;						// ÆÀ¿ø¼ö
		// TEAM_MEMBER		stTeam_Member;				// ÆÀ¿øÁ¤º¸
		// ...											// ÆÀ¿ø¼ö¸¸Å­

		// ÆÀÅ»Åð
		TEAM_CS_SECEDE,

		// ÆÀÅ»Åð °á°ú.(ÆÀ¿ùµåÈ­)
		TEAM_SC_SECEDE,		// ¸ðµç ÆÀ¿ø¿¡°Ô ¼Û½Å, *** ÁÖÀÇ : ÆÀ¿øÀÌ 2¸íÀÎ °æ¿ì¿¡ ÆÐÅ¶À» ¹ÞÀ¸¸é ÆÄÆ¼ÇØÁ¦ÇØ¾ßÇÕ´Ï´Ù.
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// INT32			iActCharID;					// Å»ÅðÀÚÄ³¸¯ÅÍDBÅ°

		// °­Åð(ÆÀ¿ùµåÈ­)
		TEAM_CS_EXPEL,
		// INT32			iRcvCharID;					// ´ë»óÀÚÄ³¸¯ÅÍDBÅ°

		// °­Åð°á°ú(ÆÀ¿ùµåÈ­)
		TEAM_SC_EXPEL,		// ¸ðµç ÆÀ¿ø¿¡°Ô ¼Û½Å, *** ÁÖÀÇ : ÆÀ¿øÀÌ 2¸íÀÎ °æ¿ì¿¡ ÆÐÅ¶À» ¹ÞÀ¸¸é ÆÄÆ¼ÇØÁ¦ÇØ¾ßÇÕ´Ï´Ù.
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// INT32			iRcvCharID;					// °­ÅðÀÚÄ³¸¯ÅÍDBÅ°

		// ÆÀÀåÀ§ÀÓ(ÆÀ¿ùµåÈ­)
		TEAM_CS_CHANGECAPTAIN,
		// INT32			iRcvCharID;					// ´ë»óÀÚÄ³¸¯ÅÍDBÅ°

		// ÆÀÀåÀ§ÀÓ °á°ú(ÆÀ¿ùµåÈ­)
		TEAM_SC_CHANGECAPTAIN,	// ¸ðµç ÆÀ¿ø¿¡°Ô ¼Û½Å
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// INT32			iRcvCharID;					// »õÆÀÀåÄ³¸¯ÅÍDBÅ°

		// ·çÆÃ·êº¯°æ
		TEAM_CS_CHANGELOOTINGRULE,
		// UINT8			byLootingRule;				// »õ·çÆÃ·ê

		// ·çÆÃ·êº¯°æ °á°ú
		TEAM_SC_CHANGELOOTINGRULE,	// ¸ðµç ÆÀ¿ø¿¡°Ô ¼Û½Å
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT8			byLootingRule;				// »õ·çÆÃ·ê

		// ÆÀ¿øÁ¤º¸°»½Å °á°ú(¼­¹ö¿¡¼­ º¸³»±â¸¸ ÇÔ) : ¾î¶²½ÃÁ¡¿¡ º¸³¾°ÍÀÎÁö ÇÊ¿ä
		// 2006.04.13 dhpark ¿ùµå¸ÊÀÇ À§Ä¡Ç¥½Ã¸¦ À§ÇØ¼­ À§Ä¡Á¤º¸°¡ Ãß°¡µÇ°í Å¬¶óÀÌ¾ðÆ®°¡ ¿äÃ»ÇÒ¼öÀÖµµ·Ï ¼öÁ¤ÇÕ´Ï´Ù.
		TEAM_SC_UPDATEMEMBER,	// ¸ðµç ÆÀ¿ø¿¡°Ô ¼Û½Å. *** ÁÖÀÇ : ºê·ÎµåÄ³½ºÆÃ¿µ¿ª ¹ÛÀÇ ÆÀ¿ø¸¸ °»½ÅÇÕ´Ï´Ù. ¿µ¿ª¾ÈÀÇ ÆÀ¿øÀº ºê·ÎµåÄ³½ºÆÃÁ¤º¸°¡ ´õ Á¤È®ÇÕ´Ï´Ù.
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// TEAM_MEMBER		stTeam_Member;				// ÆÀ¿øÁ¤º¸

		// ÆÀ¿øÁ¤º¸°»½Å ¿äÃ». ¿ùµå¸ÊÀ» ¿­°í ÀÖ´Â ÆÀ¿øÀº ÀÏÁ¤ÁÖ±â¸¶´Ù ´Ù¸¥ÆÀ¿øÀÇ Á¤º¸¸¦ ¿äÃ»ÇÕ´Ï´Ù.
		TEAM_CS_UPDATEMEMBER,
		// GU_ID			RcvCharGUID;				// Á¤º¸°¡ °»½ÅµÉ ÆÀ¿ø

		// ÆÀÁ¤º¸¼Û½Å(Ã¤³ÎÀÌµ¿ÇÑ À¯Àú¿¡°Ô ÆÀÁ¤º¸ ¼Û½Å½Ã »ç¿ë)
		TEAM_SC_INFO,
		// GU_ID					ObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT8					byLootingRule;					// ·çÆÃ·ê(0:Â÷·Ê´ë·Î,1:·£´ý,2:ÀÚÀ¯)
		// int						iCnt;							// ÆÀ¿ø¼ö
		// TEAM_MEMBER				stTeam_Member;					// ÆÀ¿øÁ¤º¸
		// ...														// ÆÀ¿ø¼ö¸¸Å­

	//////////////////////////////////////////////////////////////////////////
	//
	//	Event Box 
	//
		PI_CS_GIFT = PI_CS_LOGIC + 2100,

		// gift º¸°üÇÔ¿¡ Á¸ÀçÇÏ´Â gift¸®½ºÆ®¸¦ ¿äÃ».
		GIFT_CS_LIST = PI_CS_GIFT,

		// gift º¸°üÇÔ¿¡ Á¸ÀçÇÏ´Â gift¿äÃ» °á°ú.
		GIFT_SC_LIST,
		// GU_ID				CharGUID;		// ÁÖÃ¼ÀÚ
		// GLOBAL_STRING_ID		iError ;		// 0: ¼º°ø
		// int					iNumOfGiftID;	// ¹Þ¾Æ¾ß ÇÒ gift °¹¼ö
		// __int64				iGiftNO;		// gift key ID					
		// int					iGiftID;		// GIFT.LDT ÂüÁ¶	
		
		// gift º¸°üÇÔ¿¡¼­ 1°³ÀÇ gift ¹Þ±â ¿äÃ».
		GIFT_CS_GET,
		// __int64				iGiftNO;

		// gift º¸°üÇÔ¿¡¼­ 1°³ÀÇ gift ¹Þ±â ¿äÃ» °á°ú
		GIFT_SC_GET,
		// GU_ID				CharGUID;		// ÁÖÃ¼ÀÚ
		// GLOBAL_STRING_ID		iError ;		// 0: ¼º°ø
		// __int64				iGiftNO;
		// __int64				iCurrMoney;		// µ·°»½Å
		// UINT8				iItemCount;		// ¾ÆÀÌÅÛ »ý¼º °¹¼ö
		// CONTAINER_ITEM		tContainerItem;	// »ý¼º ¾ÆÀÌÅÛ Á¤º¸

	//////////////////////////////////////////////////////////////////////////
	//
	//	MailBox
	//
		PI_CS_MAILBOX = PI_CS_LOGIC + 2200,

		// ¸ÞÀÏ ¸ñ·Ï ¿äÃ»
		MAILBOX_CS_GETLIST	= PI_CS_MAILBOX,
		// TCHAR					szCharName[LEN_NAME+1];					// null included
		// UINT32					uiMailBoxID;
		MAILBOX_SC_GETLIST,
		// GLOBAL_STRING_ID			iError;
		// UINT8					nMailCount;				// ¼ö½Å ¸ÞÀÏ °¹¼ö
			// nMailCount ¹Ýº¹
			//{
				// MAIL_HEADER_ATTACHMENT	mailHeaderInfo[];		// ¸ÞÀÏÇì´õ Á¤º¸
				// UINT8			uiSubjectLen;
				// TCHAR			szSubect[];
				// CONTAINER_ITEM	containerItem;				
			//}
		
		// ¸ÞÀÏ Àü¼Û (Å¬¶óÀÌ¾ðÆ®¿¡¼­ÀÇ Àü¼ÛÀº ÇÑ¹ø¿¡ ÇÏ³ª¾¿¸¸ °¡´ÉÇÏ°Ô ºí·°µÊ. (Àü¼Û - °á°ú ¼ö½Å - ´ÙÀ½ Àü¼Û)
		MAILBOX_CS_SEND,
		// MAIL_HEADER_ATTACHMENT	mailHeaderInfo[];		// ¸ÞÀÏÇì´õ Á¤º¸
		// UINT8					uiSubjectLen;			// Á¦¸ñ
		// TCHAR					szSubect[];
		// UINT8					uiMemoLen;				// º»¹®
		// TCHAR					szMemo[];
		// TCHAR					szReceiver[LEN_NAME+1];	// ¼ö½ÅÀÚ
		// UINT32					uiMailBoxID;
		// CONTAINER_SLOT			UsingItemPos;		// »ç¿ëÇÒ ¾ÆÀÌÅÛ À§Ä¡Á¤º¸
		// ITEMNO					iUsingItemNo;		// »ç¿ëÇÒ ¾ÆÀÌÅÛ Å°¹øÈ£

		MAILBOX_SC_SEND,
		// GLOBAL_STRING_ID			iError;

		// if iError == 0
			// UINT8				flagTrashItem;			// 0(NONE),1(Item¸¦ ÀÎº¥¿¡¼­ »èÁ¦ÇÒ °Í)
			// TRASH_ITEM			AttachItem;				// Ã·ºÎ¾ÆÀÌÅÛ
			// UINT64				uiRemainEly;			// ¸ÞÀÏ Àü¼ÛÈÄ ÀÜ¿© Ely·®
			// TRASH_ITEM			UsingItem;				// ¿ìÇ¥¾ÆÀÌÅÛ
		// else
			// TCHAR				szReceiver[LEN_NAME+1];	// if iError != 0(¿À·ù½Ã)

		// ¸ÞÀÏ »èÁ¦
		MAILBOX_CS_DELETE,
		// UINT64					uiMailID;
		// UINT32					uiMailBoxID;
		MAILBOX_SC_DELETE,
		// GLOBAL_STRING_ID			iError;
		// UINT64					uiMailID;

		// ¸ÞÀÏ (º»¹®) ¼ö½Å
		MAILBOX_CS_GETBODY,
		// UINT64					uiMailID;
		// UINT32					uiMailBoxID;
		MAILBOX_SC_GETBODY,
		// GLOBAL_STRING_ID			iError;
		// UINT64					uiMailID;
		// UINT8					uiLen;
		// TCHAR					szMemo[];

		// Ã·ºÎµÈ Á¤º¸ Ã³¸® 
		// Ã·ºÎµÈ Ely,Item¸¦ ³ªÀÇ ÀÎº¥°ú ÀÜ°í¿¡ Ãß°¡ÇÑ´Ù.(Attachment Á¤º¸´Â ¼Ò°Å,Memo´Â À¯Áö. -->  Â÷ÈÄ Å¬¶óÀÌ¾ðÆ®ÀÇ ¸í½ÃÀûÀÎ »èÁ¦ ¿äÃ»¿¡ ÀÇÇØ ¸ÞÀÏ »èÁ¦µÊ)
		MAILBOX_CS_MOVEATTACHMENT,
		// UINT64					uiMailID;
		// CONTAINER_ITEM			containerItem;				
		// UINT32					uiMailBoxID;
		MAILBOX_SC_MOVEATTACHMENT,						// Item ÀÌµ¿ Á¤º¸(InsertITem), Elyº¯È­·®Àº º°µµÀÇ ÆÐÅ¶À¸·Î Ã³¸®µÊ
		// GLOBAL_STRING_ID			iError;
		// UINT64					uiMailID;
		// UINT64					uiRemainEly;
		// UINT8					uiItemCount;		// ¾ÆÀÌÅÛ »ý¼º °¹¼ö
		// CONTAINER_ITEM			containerItem;		// »ý¼º ¾ÆÀÌÅÛ Á¤º¸
		// UINT8					uiMailDeleted;		// Memo ³»¿ëÀÌ ¾ø´Â °æ¿ì ÀÚµ¿ »èÁ¦µÊ

		// New Mail arrived
		MAILBOX_SC_NEWMAIL,
		// GLOBAL_STRING_ID			iError;
		// UINT8					nMailCount;			// »õ·Î ¼ö½ÅÇÑ ¸ÞÀÏ °¹¼ö (ÇöÀç´Â Ç×»ó 1)
		// MAIL_HEADER_ATTACHMENT	mailHeaderInfo[];	// ¸ÞÀÏÇì´õ Á¤º¸
		// UINT8					uiLen;
		// TCHAR					szSubject[];
		// CONTAINER_ITEM			containerItem;		// Ãß°¡ ¾ÆÀÌÅÛ Á¤º¸

	//////////////////////////////////////////////////////////////////////////
	//
	//	Cash Shop 
	//
		PI_CS_CASHSHOP = PI_CS_LOGIC + 2300,

		// Ä³½¬¼¥ ÁøÀÔ
		CASHSHOP_CS_IN = PI_CS_CASHSHOP,
		// int					iMajor;  // Ãß°¡ : GLOBAL_SYS : 21¹ø °ª. 
		// int					iMinor;  // Ãß°¡ : Å¬¶óÀÌ¾ðÆ®¿¡¼­ ÃÖÁ¾ ¹ÞÀº ¸¶ÀÌ³Ê ¹öÀü. Ã¤³Î ÀÌµ¿½Ã¿¡µµ À¯ÁöµÇ¾úÀ¸¸é ÁÁ°ÚÀ¸³ª... 
		// int					iHotVer; // Ãß°¡

		CASHSHOP_SC_IN,
 		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;
		// int					iMinor;  // Ãß°¡ 
		// int					iHotVer; // Ãß°¡

			// for GamePot
				// UINT8		uiTokenLen;
				// if uiTokenLen > 0
					// TCHAR	szToken[];

			// other
				// UINT64		uiDID;
				// int    nCount;             //[xialin 2008/07/30] addÊÔ´©¼ÇÂ¼
				// ITEMID nItems[nCount];


		// Ä³½¬¼¥ ÀÌÅ»
		CASHSHOP_CS_OUT,
		// int nCount;		//[xialin 2008/07/30] addÊÔ´©¼ÇÂ¼
		// ITEMID nItems[nCount];

		CASHSHOP_SC_OUT,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;

		// Ä³½¬Á¤º¸/ÀÎº¥¾ÆÀÌÅÛÁ¤º¸¿äÃ».
		CASHSHOP_CS_INFO,

		// Ä³½¬Á¤º¸/ÀÎº¥¾ÆÀÌÅÛ Á¤º¸ ¿äÃ» °á°ú
		CASHSHOP_SC_INFO,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;
		// int					iCurrOz;
		// int					iCurrMileage;
		// UINT16				nItems;
		// CONTAINER_ITEM		containerItem;

		// Ä³½¬Á¤º¸ °»½Å ¿äÃ».
		CASHSHOP_CS_CASH,
		
		// Ä³½¬Á¤º¸ °»½Å ¿äÃ» °á°ú.
		CASHSHOP_SC_CASH,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;
		// int					iCurrOz;
		// int					iCurrMileage;

		// CASHSHOP ¿¡¼­ ±¸ÀÔ.
		CASHSHOP_CS_BUY,
//new
		// int					iPackageID;				//CASH_SHOP.LDT::ItemID
		//-------------------------------------------   // À¯È¿¼º °Ë»ç¸¦ À§ÇØ
		// int					iShopPrice;				//CASH_SHOP.LDT::_CashPrice
		// int					iSalePrice;				//CASH_SHOP.LDT::_SalePrice
		// int					iMaxOz;					//CASH_SHOP.LDT::_MaxOz
		// int					iMaxMileage;			//CASH_SHOP.LDT::_MaxMileage
		//--------------------------------------------  // À¯È¿¼º °Ë»ç¸¦ À§ÇØ
		// int					iBuyOz;					// Çö±ÝÅõÀÚ°¡.
		// int					iBuyMileage;			// ¸¶ÀÏ¸®Áö ÅõÀÚ°¡.

		// CASHSHOP ¿¡¼­ ±¸ÀÔ°á°ú.
		CASHSHOP_SC_BUY,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;
		// int					iPackageID;				//CASH_SHOP.LDT::ItemID 
		// int					iCurrOz;				// ÇöÀç ¿ÀÁî
		// int					iCurrMileage;			// ÇöÀç ¸¶ÀÏ¸®Áö.
		// UINT16				nItems;					// ½Å±Ô »ý¼º ¾ÆÀÌÅÛ °¹¼ö
		// CONTAINER_ITEM		containerItem;			// ½Å±Ô »ý¼º ¾ÆÀÌÅÛ Á¤º¸.

		// CASHSHOP ¿¡ ÆÈ±â. // stack ÀÌ ÀÖ´Â °Ç ÆÈ¼ö ¾øÀ½. ±âº» 1°³Â¥¸®¸¸ µÇÆÈ±â°¡ µÊ. 
		CASHSHOP_CS_SELL,
		// CONTAINER_ITEM		tContainerItem;         // ¾î´À À§Ä¡¿¡ ÀÖ´Â ³ðÀ» // ¹«Á¶°Ç ½½·Ô ´ÜÀ§ÀÌ¸ç // MaxStack ÀÌ 1ÀÎ°Í¸¸ °¡´É

		// CASHSHOP ¿¡ ÆÈ±â °á°ú.
		CASHSHOP_SC_SELL,
		// GLOBAL_STRING_ID		iError ;		// 0: ¼º°ø
		// GU_ID				globalID;
		// int					iCurrMileage;	// ¸¶ÀÏ¸®Áö·Î È¯»ê µÇ¹Ç·Î °ÅÀÇ º¯È­ µÇ¾î¾ß ÇÔ.
		// TRASH_ITEM			tTrashItem;		// »èÁ¦µÇ¾î¾ß ÇÒ ¾ÆÀÌÅÛ Á¤º¸
		// UINT8				iSkipUpdate;	// ÀÜ°í¸¦¾÷µ¥ÀÌÆ® ÇÒÁö¿©ºÎ 0: ¾÷µ¥ÀÌÆ® 1: ¾÷µ¥ÀÌÆ®¾ÈÇÔ


		// ÄíÆù µî·Ï
		CASHSHOP_CS_COUPON,
		//UINT8	len;
		//TCHAR szCouponCode[];

		CASHSHOP_SC_COUPON,
		// GLOBAL_STRING_ID		iError ;		// 0: ¼º°ø
			//iError == 9002003
				// int iMilage;					// iMilage ¸¶ÀÏ¸®Áö¸¦ È¹µæÇÏ¼Ì½À´Ï´Ù.		
				// int iRemainMilage;			// ÃÑ ¸¶ÀÏ¸®Áö ÀÜ°í
			//iError == 9002004
				// UINT32	uiGiftID;			// uiGiftIDÀ» È¹µæÇÏ¼Ì½À´Ï´Ù. Gift ¸¦ È®ÀÎÇÏ¼¼¿ä....
			//iError == 9002010
				// int iOz;						// iOz¸¦ È¹µæÇÏ¼Ì½À´Ï´Ù.		
				// int iRemainOz;				// ÃÑ ÀÜ°í

		// CASHSHOP ¿¡¼­ ±¸ÀÔ.
		CASHSHOP_CS_BUY_PRESENT,
//new
		// TCHAR				TargetCharName[LEN_NAME];
		// int					iPackageID;				//CASH_SHOP.LDT::ItemID
		//-------------------------------------------   // À¯È¿¼º °Ë»ç¸¦ À§ÇØ
		// int					iShopPrice;				//CASH_SHOP.LDT::_CashPrice
		// int					iSalePrice;				//CASH_SHOP.LDT::_SalePrice
		// int					iMaxOz;					//CASH_SHOP.LDT::_MaxOz
		// int					iMaxMileage;			//CASH_SHOP.LDT::_MaxMileage
		//--------------------------------------------  // À¯È¿¼º °Ë»ç¸¦ À§ÇØ
		// int					iBuyOz;					// Çö±ÝÅõÀÚ°¡.
		// int					iBuyMileage;			// ¸¶ÀÏ¸®Áö ÅõÀÚ°¡.
		// TCHAR				Comment[MAX_MAIl_BODY_LEN] // ¼±¹° ³»¿ë.					

		// CASHSHOP ¿¡¼­ ±¸ÀÔ°á°ú.
		CASHSHOP_SC_BUY_PRESENT,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;
		// TCHAR				TargetCharName[LEN_NAME];
		// int					iPackageID;				//CASH_SHOP.LDT::ItemID 
		// int					iCurrOz;				// ÇöÀç ¿ÀÁî
		// int					iCurrMileage;			// ÇöÀç ¸¶ÀÏ¸®Áö.

		// Âò¸®½ºÆ® ¼Û½Å(CASHSHOP_CS_INFO¿¡ ÀÇÇØ¼­ ÀÚµ¿À¸·Î Å¬¶óÀÌ¾ðÆ®¿¡ ¼Û½Å)
		CASHSHOP_SC_WISH_LIST,
		// int					iCnt;			// Âò°³¼ö
		// int					iPackageID;		// ÆÐÅ°Áö¾ÆÀÌµð
		// ...									// Âò°³¼ö¸¸Å­

		// ÂòÃß°¡
		CASHSHOP_CS_WISH_ADD,
		// int					iPackageID;		// ÆÐÅ°Áö¾ÆÀÌµð

		// ÂòÃß°¡
		CASHSHOP_SC_WISH_ADD,
		// GLOBAL_STRING_ID		iErrorMsg;
		// int					iPackageID;		// ÆÐÅ°Áö¾ÆÀÌµð(¼º°øÀÏ¶§¸¸)

		// Âò»èÁ¦
		CASHSHOP_CS_WISH_DEL,
		// int					iPackageID;		// ÆÐÅ°Áö¾ÆÀÌµð

		// Âò»èÁ¦
		CASHSHOP_SC_WISH_DEL,
		// GLOBAL_STRING_ID		iErrorMsg;
		// int					iPackageID;		// ÆÐÅ°Áö¾ÆÀÌµð(¼º°øÀÏ¶§¸¸)

		// Ãß°¡
		CASHSHOP_SC_MINOR_INIT,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;

		// Ãß°¡
		CASHSHOP_SC_MINOR_INFO,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;
		// UINT16				nShopInfo;
		// CASHSHOP_INFO		ShopInfos[];		

		CASHSHOP_SC_OUT_READY,
		// GLOBAL_STRING_ID		iErrorMsg;
		// GU_ID				globalID;



		//20080407 mijin ·Îµù½Ã Ä³½¬¼¥,ÃßÃµ»óÇ° ¹öÀü Ã¼Å©
		CASHSHOP_CS_VERSION,
		// int					iMajor;  				//Ä³½¬¼¥ Major¹öÀü
		// int					iMinor;  				//Ä³½¬¼¥ Minor¹öÀü
		// int					iHotVer;				//ÃßÃµ»óÇ° ¹öÀü

		//20080407 mijin Ä³½¬¼¥,ÃßÃµ»óÇ° ¹öÀüÀÌ Æ²¸®¸é µ¥ÀÌÅÍ¸¦ º¸³½´Ù
		CASHSHOP_SC_VERSION,
		// GLOBAL_STRING_ID			iErrorMsg;
		// GU_ID				globalID;
		// int					iMinor;  				//Ä³½¬¼¥ Minor¹öÀü
		// int					iHotVer;				//ÃßÃµ»óÇ° ¹öÀü



		//20080407 mijin ÇÊµå¿¡¼­ ¹°Ç° °¡°Ý Ã¼Å©
		CASHSHOP_CS_PRICE,
		// int					iHotVer;				//ÃßÃµ»óÇ° ¹öÀü
		// int					iPackageID;				//CASH_SHOP.LDT::ItemID
		// int					iShopPrice;				//CASH_SHOP.LDT::_CashPrice
		// int					iSalePrice;				//CASH_SHOP.LDT::_SalePrice
		// int					iMaxOz;					//CASH_SHOP.LDT::_MaxOz
		// int					iMaxMileage;				//CASH_SHOP.LDT::_MaxMileage

		//20080407 mijin ÇÊµå¿¡¼­ ¹°Ç° °¡°Ý Ã¼Å© °á°ú
		CASHSHOP_SC_PRICE,
		// GLOBAL_STRING_ID			iErrorMsg;
		// GU_ID				globalID;
		// int					iHotVer;				//ÃßÃµ»óÇ° ¹öÀü
		// int					iPackageID;				//CASH_SHOP.LDT::ItemID 
		// int					iShopPrice;				//CASH_SHOP.LDT::_CashPrice
		// int					iSalePrice;				//CASH_SHOP.LDT::_SalePrice
		// int					iMaxOz;					//CASH_SHOP.LDT::_MaxOz
		// int					iMaxMileage;				//CASH_SHOP.LDT::_MaxMileage



		//ÃßÃµ»óÇ° ¹öÀüÀÌ Æ²¸±°æ¿ì µ¥ÀÌÅÍ¸¦ º¸³½´Ù
		CASHSHOP_SC_HOT_INFO,
		// GLOBAL_STRING_ID			iErrorMsg;
		// GU_ID					globalID;
		// UINT16					nHotInfo;
		// CASHSHOP_HOT_INFO		HotInfos[];


		// Áö¿¥Åø¿¡ ÀÇÇÑ ÃßÃµ»óÇ° º¯°æ»óÅÂ¸¦ ¸ðµç pcµé¿¡°Ô Àü¼Û
		CASHSHOP_SC_HOT_STATUS,
		// UINT8					iHotCashShopMode;

		CASHSHOP_CS_EFFECT,
		//int					iSeqID;						//¿Í»§¶ËSeqID
		//int					iID;						//CASH_EFFECT.LDT::nID
		//UINT8					iOptLen;					//¸½¼þÐÅÏ¢³¤¶È
		//TCHAR					szOpt[];					//¸½¼þÐÅÏ¢

		CASHSHOP_SC_EFFECT,
		//int					iSeqID;						//¿Í»§¶ËSeqID
		//GLOBAL_STRING_ID		iErrorMsg;

		//	jmulro ¿ìËÙ¹ºÂò£¬2008£­8£­26  ---start
		CASHSHOP_CS_GET_QUICK_BUY_INFO,	
		CASHSHOP_SC_GET_QUICK_BUY_INFO,
		//	GLOBAL_STRING_ID			iErrorMsg;
		//	UINT uInfoCount;
		//	t_Quick_Buy_Info info[];
		CASHSHOP_CS_QUICK_BUY_BUY_ITEM,
		//	int					nPackageID;
		//-------------------------------------------   // À¯È¿¼º °Ë»ç¸¦ À§ÇØ
		// int					iShopPrice;				//CASH_SHOP.LDT::_CashPrice
		// int					iSalePrice;				//CASH_SHOP.LDT::_SalePrice
		// int					iMaxOz;					//CASH_SHOP.LDT::_MaxOz
		// int					iMaxMileage;			//CASH_SHOP.LDT::_MaxMileage
		//--------------------------------------------  // À¯È¿¼º °Ë»ç¸¦ À§ÇØ
		// int					iBuyOz;					// Çö±ÝÅõÀÚ°¡.
		// int					iBuyMileage;			// ¸¶ÀÏ¸®Áö ÅõÀÚ°¡.		
		//	jmulro ¿ìËÙ¹ºÂò£¬2008£­8£­26 ---end

		CASHSHOP_CS_ASK_BUY,
		// UINT8				nPackageID;					// iPackageIDµÄÊýÁ¿
		// int					iPackageID[];				// CASH_SHOP.LDT::ItemID
		// UINT8				len;						// szMsg³¤¶È
		// TCHAR				szMsg[];					// ÏûÏ¢
		// UINT8				nReceiverName;				// szReceiverNameµÄ³¤¶È
		// TCHAR				szReceiverName[LEN_NAME];	// ±»ÇëÇóÍæ¼ÒÃû×Ö

		CASHSHOP_SC_ASK_BUY,
		// GLOBAL_STRING_ID		iErrorMsg;
		// UINT8				nPackageID;					// iPackageIDµÄÊýÁ¿
		// int					iPackageID[];				// CASH_SHOP.LDT::ItemID
		// UINT8				nSenderName;				// szSenderµÄ³¤¶È
		// TCHAR				szSender[];					// ·¢ËÍÕßÃû×Ö

		CASHSHOP_SC_ASK_BUY_RECV,
		// UINT8				nPackageID;					// iPackageIDµÄÊýÁ¿
		// int					iPackageID;					// CASH_SHOP.LDT::ItemID
		// UINT8				iLen;						// szReceiverNameµÄ³¤¶È
		// TCHAR				szRequesterName[LEN_NAME];	// ±»ÇëÇóÍæ¼ÒÃû×Ö
		// UINT8				len;						// szMsg³¤¶È
		// TCHAR				szMsg[];					// ÏûÏ¢

	//////////////////////////////////////////////////////////////////////////
	//
	//	Title
	//
		PI_CS_TITLE = PI_CS_LOGIC + 2400,

		// Å¸ÀÌÆ² ¸®½ºÆ®. Å¸ÀÌÆ²ÀÇ ¼ö°¡ ¸¹À¸¸é ÆÐÅ¶´ç ÀÏÁ¤¼ö¸¸Å­¸¸ ¿©·¯¹ø ¼Û½ÅÇÕ´Ï´Ù. 
		TITLE_SC_LIST = PI_CS_TITLE,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// int				iCnt;			// Å¸ÀÌÆ² °³¼ö
		// UINT32			uiTitleID;		// Å¸ÀÌÆ² ¾ÆÀÌµð
		// ...								// Å¸ÀÌÆ² °³¼ö¸¸Å­

		// Å¸ÀÌÆ² Ãß°¡
		TITLE_SC_INSERT,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT32			uiTitleID;		// ½ÀµæÇÑ Å¸ÀÌÆ²¾ÆÀÌµð

		// Å¸ÀÌÆ² º¯°æ
		TITLE_CS_CHANGE,
		// UINT32			uiTitleID;		// ÀåÂøÇÒ Å¸ÀÌÆ²¾ÆÀÌµð

		// Å¸ÀÌÆ² º¯°æ
		TITLE_SC_CHANGE,	// ÁÖº¯¿¡ ºê·ÎµåÄ³½ºÆ®ÇØ¾ßÇÔ
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT32			uiTitleID;		// ÀåÂøÇÒ Å¸ÀÌÆ²¾ÆÀÌµð

	//////////////////////////////////////////////////////////////////////////
	//
	//	FreeMarket
	//
		PI_CS_FREEMARKET = PI_CS_LOGIC + 2500,

		// °³ÀÎ»óÁ¡ ¿­±â
		FREEMARKET_CS_OPEN = PI_CS_FREEMARKET,
		// UINT8			byType;			// »óÁ¡Å¸ÀÔ(ÀÌ°ª¿¡ ÇØ´çÇÏ´Â Á¶°Ç°Ë»ç/ÆäÀÌÁö¼öµîÀ» °Ë»çÇÕ´Ï´Ù.)
		// char				szFMName[FM_MAX_NAME+1]; // »óÁ¡¸í
		// int				iCnt;			// ÆÇ¸Å¾ÆÀÌÅÛ °³¼ö
		// FM_SELLITEM		stFM_SellItem;	// ÆÇ¸Å¾ÆÀÌÅÛ Á¤º¸
		// ...								// ÆÇ¸Å¾ÆÀÌÅÛ °³¼ö¸¸Å­

		// °³ÀÎ»óÁ¡ ¿­±â
		FREEMARKET_SC_OPEN,		// ÁÖº¯¿¡ ºê·ÎµåÄ³½ºÆ®ÇØ¾ßÇÔ
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð(»óÁ¡ÁÖÀÎ)
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð(»óÁ¡ÁÖÀÎ)
		// GLOBAL_STRING_ID	ErrorID;		// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
		// UINT8			byType;			// »óÁ¡Å¸ÀÔ
		// char				szFMName[FM_MAX_NAME+1]; // »óÁ¡¸í

		// °³ÀÎ»óÁ¡ ´Ý±â
		FREEMARKET_CS_CLOSE,

		// °³ÀÎ»óÁ¡ ´Ý±â
		FREEMARKET_SC_CLOSE,	// ÁÖº¯¿¡ ºê·ÎµåÄ³½ºÆ®ÇØ¾ßÇÔ
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð(»óÁ¡ÁÖÀÎ)
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð(»óÁ¡ÁÖÀÎ)
		// GLOBAL_STRING_ID	ErrorID;		// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// °³ÀÎ»óÁ¡¿¡ µé¾î°¡±â
		FREEMARKET_CS_IN,
		// GU_ID			FMGUID;			// »óÁ¡ÁÖÀÎÀÇ °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð

		// °³ÀÎ»óÁ¡¿¡ µé¾î°¡±â
		FREEMARKET_SC_IN,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;		// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
		// int				iCnt;			// ÆÇ¸Å¾ÆÀÌÅÛ °³¼ö
		// FM_VIEWITEM		stFM_ViewItem;	// ÆÇ¸Å¾ÆÀÌÅÛ Á¤º¸
		// ...								// ÆÇ¸Å¾ÆÀÌÅÛ °³¼ö¸¸Å­

		// °³ÀÎ»óÁ¡¿¡¼­ ³ª°¡±â
		FREEMARKET_CS_OUT,
		// GU_ID			FMGUID;			// »óÁ¡ÁÖÀÎÀÇ °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð

		// °³ÀÎ»óÁ¡¿¡¼­ ³ª°¡±â
		FREEMARKET_SC_OUT,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð

		// °³ÀÎ»óÁ¡¿¡¼­ ¹°°Ç »ç±â
		FREEMARKET_CS_BUY,
		// GU_ID			FMGUID;			// »óÁ¡ÁÖÀÎÀÇ °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// FM_ITEM			stFM_Item;		// »ç°í½ÍÀº ¾ÆÀÌÅÛ Á¤º¸

		// °³ÀÎ»óÁ¡¿¡¼­ ¹°°Ç »ç±â
		FREEMARKET_SC_BUY,	// ¿¡·¯ÀÏ¶§´Â ´ç»çÀÚ¿¡°Ô¸¸ º¸³»°í ¼º°øÀÏ¶§´Â ¼Õ´Ôµé ¸ðµÎ¿¡°Ô º¸³»¼­ ÆÇ¸Å¾ÆÀÌÅÛ¸ñ·ÏÀ» °»½ÅÇÏ°Ô ÇÕ´Ï´Ù.
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð(¹°°ÇÀ»»ê¼Õ´Ô)
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð(¹°°ÇÀ»»ê¼Õ´Ô)
		// GLOBAL_STRING_ID	ErrorID;		// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
		// FM_ITEM			stFM_Item;		// ÆÇ¸ÅµÈ ¾ÆÀÌÅÛ Á¤º¸

	//////////////////////////////////////////////////////////////////////////
	//
	// Festival ( °ÔÀÓ³»¿¡¼­ ÁøÇàµÇ´Â °¢Á¾ ÀÌº¥Æ®)
	//
		PI_CS_FESTIVAL = PI_CS_LOGIC + 2600,

	//////////////////////////////////////////////////////////////////////////
	//
	//	WayPoint
	//
		PI_CS_WAYPOINT = PI_CS_LOGIC + 2700,

		// ¿þÀÌÆ÷ÀÎÆ® ¸®½ºÆ®
		WAYPOINT_SC_LIST = PI_CS_WAYPOINT,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// int				arrWayPoint[];	// ¿þÀÌÆ÷ÀÎÆ®µ¥ÀÌÅÍ

		// »õ ¿þÀÌÆ÷ÀÎÆ® ¹ß°ß
		WAYPOINT_CS_FIND,
		// int				iWayPointID;		// ¿þÀÌÆ÷ÀÎÆ®¾ÆÀÌµð

		// »õ ¿þÀÌÆ÷ÀÎÆ® ¹ß°ß
		WAYPOINT_SC_FIND,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;		// ¿¡·¯¾ÆÀÌµð
		// int				iWayPointID;	// ¿þÀÌÆ÷ÀÎÆ®¾ÆÀÌµð

		// ¼±ÅÃÇÑ ¿þÀÌÆ÷ÀÎÆ®·Î ÀÌµ¿
		WAYPOINT_CS_MOVE,
		// int				iWayPointID;		// ¿þÀÌÆ÷ÀÎÆ®¾ÆÀÌµð
		// UINT8			byRank;				// 0, 1

		// ¼±ÅÃÇÑ ¿þÀÌÆ÷ÀÎÆ®·Î ÀÌµ¿
		WAYPOINT_SC_MOVE,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;		// ¿¡·¯¾ÆÀÌµð
		// int				iWayPointID;	// ¿þÀÌÆ÷ÀÎÆ®¾ÆÀÌµð

	//////////////////////////////////////////////////////////////////////////
	//
	//	Guild
	//
		PI_CS_GUILD = PI_CS_LOGIC + 2800,

		// PC·Î±×ÀÎ½Ã ¼­¹ö°¡ º¸³»ÁÖ´Â PC°¡ ¼Ò¼ÓµÈ ±æµåÁ¤º¸
		GUILD_SC_PCGUILD = PI_CS_GUILD,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// PCGUILD			stPCGuild;							// Ä³¸¯ÅÍ ±æµåÁ¤º¸

		// ÀÏ¹Ý ±æµåÁ¤º¸
		GUILD_CS_GUILD_COMMON,
			
		// ÀÏ¹Ý ±æµåÁ¤º¸
		GUILD_SC_GUILD_COMMON,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
		// GUILD_COMMON		stGuildCommon;						// ÀÏ¹Ý ±æµåÁ¤º¸

		// ±æµå ¸â¹ö ¸®½ºÆ®
		GUILD_CS_MEMBERLIST,
			
		// ±æµå ¸â¹ö ¸®½ºÆ®
		GUILD_SC_MEMBERLIST,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
		// INT32			iMemberCnt;							// ±æµå ¸â¹ö¼ö
		// GUILD_MEMBER		stGuildMember;						// ±æµå ¸â¹öÁ¤º¸
		// ...
	
		// ±æµå »ý¼º
		GUILD_CS_CREATE,
		// char				szGName[GUILD_MAX_NAME+1];			// ±æµå¸í
		// UINT8			byGType;							// ±æµåÅ¸ÀÔ(1:ÀÏ¹Ý, 2:Ãµ»ç, 3:¾Ç¸¶)
		// UINT8			arrGMark[3];						// ±æµå¸¶Å©
		// UINT8			byGRank;							// ±æµå·©Å©(Å¸ÀÔ°ú ·©Å©¸¦ ¹­¾î¼­ LDTÅ°·Î »ç¿ëÇÕ´Ï´Ù.)
			
		// ±æµå »ý¼º(¼º°ø½Ã ºê·ÎµåÄ³½ºÆ®µÇ¾î¾ßÇÔ)
		GUILD_SC_CREATE,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
		// PCGUILD			stPCGuild;							// Ä³¸¯ÅÍ ±æµåÁ¤º¸

		// ±æµå ÆÄ±«
		GUILD_CS_DESTROY,
			
		// ±æµå ÆÄ±«(¼º°ø½Ã ºê·ÎµåÄ³½ºÆ®µÇ¾î¾ßÇÔ(ÁÖº¯»ç¶÷µéÀÌ ±æµå°¡ »ç¶óÁ³´Ù´Â°ÍÀ» ¾Ë¾Æ¾ßÇÔ). ±æµå¿øÀº ÀÌ¹Ì ±æ¸¶¸¸ ³²Àº »óÈ²ÀÌ¹Ç·Î °í·ÁÇÏÁö ¾ÊÀ½)
		GUILD_SC_DESTROY,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð

		// ±æµå ½½·Î°Ç ¼öÁ¤
		GUILD_CS_UPDATE_SLOGAN,
		// char				szGSlogan[GUILD_MAX_SLOGAN+1];		// ±æµå½½·Î°Ç

		// ±æµå ½½·Î°Ç ¼öÁ¤
		GUILD_SC_UPDATE_SLOGAN,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð

		// ±æµåÃÊ´ë
		GUILD_CS_REQUEST,
		// char				szRcvCharName[LEN_NAME+1];			// ´ë»óÀÚÀÌ¸§

		// ±æµåÃÊ´ë °á°ú. ¿¡·¯ÀÎ°æ¿ì´Â ¿äÃ»ÀÚ¿¡°Ô¸¸ ¼Û½Å, ¼º°øÀÎ°æ¿ì´Â ´ë»óÀÚ¿¡°Ô¸¸ ¼Û½Å. szCharName(¿¡·¯ÀÎ°æ¿ì´Â ´ë»óÀÚ¸í, ¼º°øÀÎ°æ¿ì´Â ¿äÃ»ÀÚ¸í)
		GUILD_SC_REQUEST,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// char				szGName[GUILD_MAX_NAME+1];			// ±æµå¸í
		// char				szCharName[LEN_NAME+1];				// ¼º°øÀÎ °æ¿ì´Â ¿äÃ»ÀÚ¸í, ½ÇÆÐÀÎ °æ¿ì´Â ´ë»óÀÚ¸í
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// ±æµåÂü¿©ÀÀ´ä
		GUILD_CS_JOIN,
		// char				szActCharName[LEN_NAME+1];			// ¿äÃ»ÀÚÀÌ¸§

		// ±æµåÂü¿©ÀÀ´ä
		GUILD_SC_JOIN,		// ½ÇÆÐ¸é ´ë»óÀÚ¿¡°Ô ¼Û½Å, ¼º°øÀÌ¸é ÁÖº¯¿¡ ºê·ÎµåÄ³½ºÆÃ
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
		// PCGUILD			stPCGuild;							// Ä³¸¯ÅÍ ±æµåÁ¤º¸

		// ±æµåÅ»Åð
		GUILD_CS_SECEDE,

		// ±æµåÅ»Åð °á°ú.
		GUILD_SC_SECEDE,	// ÁÖÀ§¿¡ ºê·ÎµåÄ³½ºÆÃµÇ¾î¾ßÇÔ. 
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// ±æµå°­Åð
		GUILD_CS_EXPEL,
		// INT32			iRcvCharID;							// °­Åð´ë»óÀÚÀÇ DBNum

		// ±æµå°­Åð°á°ú
		GUILD_SC_EXPEL,		// ¿äÃ»ÀÚ¿¡°Ô º¸³»´Â ¿äÃ»¿¡ ´ëÇÑ °á°ú
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// ±æµå°­Åð°á°ú
		GUILD_SC_EXPEL_ME,	// °­Åð´ë»óÀÚ ÁÖÀ§¿¡°Ô ¼Û½Å(°­Åð´ë»óÀÚÀÇ ±æµå°¡ »ç¶óÁ®¾ßÇÏ¹Ç·Î).
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð

		// Á÷Ã¥º¯°æ
		GUILD_CS_CHANGEGRADE,
		// INT32			iRcvCharID;							// Á÷Ã¥º¯°æ´ë»óÀÚÀÇ DBNum
		// UINT8			byGrade;							// º¯°æ Á÷Ã¥

		// Á÷Ã¥º¯°æ
		GUILD_SC_CHANGEGRADE, // ¿äÃ»ÀÚ¿¡°Ô º¸³»´Â ¿äÃ»¿¡ ´ëÇÑ °á°ú
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// Á÷Ã¥º¯°æ
		GUILD_SC_CHANGEGRADE_ME, // ´ë»óÀÚ¿¡°Ô º¸³»´Â Åëº¸ÆÐÅ¶. Á÷Ã¥Àº º¸¿©Áö´Â Á¤º¸°¡ ¾Æ´Ï¹Ç·Î ºê·ÎµåÄ³½ºÆÃÇÏÁö ¾ÊÀ½
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT8			byGrade;							// º¯°æ Á÷Ã¥

		// ¸â¹öÁ¤º¸º¯°æ ¸Þ¼¼Áö
		GUILD_SC_MSG_MEMBER,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT8			byMemberMsgType;					// ¸â¹ö¸Þ¼¼ÁöÅ¸ÀÔ(0:°¡ÀÔ, 1:Å»Åð, 2:°­Åð, 3:Á÷À§º¯µ¿, 4:·Î±×ÀÎ, 5:·Î±×¾Æ¿ô)
		// GUILD_MEMBER		stGuildMember;						// ¸â¹öÁ¤º¸

		// ±æµåÁ¤º¸º¯°æ ¸Þ¼¼Áö
		GUILD_SC_MSG_GUILD,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT8			byGuildMsgType;						// ±æµå¸Þ¼¼ÁöÅ¸ÀÔ(0:½½·Î°Çº¯°æ, 1:·©Å©º¯°æ)
		// GUILD_COMMON		stGuildCommon;						// ÀÏ¹ÝÀûÀÎ ±æµåÁ¤º¸

		// ±æµå±ÇÇÑÀ§ÀÓ
		GUILD_CS_CHANGEMASTER,
		// INT32			iRcvCharID;							// ´ë»óÀÚÄ³¸¯ÅÍDBÅ°

		// ¿äÃ»ÀÚ¿¡°Ô º¸³»´Â ±æµå±ÇÇÑÀ§ÀÓ °á°ú. ¿¡·¯ÀÎ°æ¿ì´Â ¿äÃ»ÀÚ¿¡°Ô¸¸ ¼Û½Å, ¼º°øÀÎ°æ¿ì´Â ¿äÃ»ÀÚÁÖº¯¿¡ ºê·ÎµåÄ³½ºÆÃ(¿¹Àü±æ¸¶ÀÇ ±æµåÅ»Åð¸¦ ÁÖº¯»ç¶÷µé¿¡°Ô ¾Ë¸®´Â ¸ñÀû)
		GUILD_SC_CHANGEMASTER,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// ´ë»óÀÚ¿¡°Ô º¸³»´Â ±æµå±ÇÇÑÀ§ÀÓ °á°ú. ±æ¸¶Á÷±ÞÀ¸·Î º¯°æÀÌ ÇÊ¿äÇÏ°ÔµÇ¸é ¿©±â¼­ Ã³¸®ÇØ¾ßÇÕ´Ï´Ù. ÇöÀç´Â ±æµåÁ¤º¸Ã¢¿¡¸¸ ±æ¸¶Á÷±Þ¾×¼ÇÀÌ ¸ô·ÁÀÖ¾î¼­ ÇÊ¿ä¾øÀ½.
		GUILD_SC_CHANGEMASTER_ME,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð

		// ¹¦¸ñ½É±â
		GUILD_CS_PLANT,
		// UINT32			uiCropID;							// ¹¦¸ñ¾ÆÀÌµð
		// TRASH_ITEM		stConsumeItem;						// ¼Ò¸ðµÇ´Â¾ÆÀÌÅÛ

		// ¹¦¸ñ½É±â °á°ú. ÁÖº¯¿¡ ºê·ÎµåÄ³½ºÆ®ÇÕ´Ï´Ù.(´Ü, ±æµå¹æ¿¡¼­¸¸ °¡´ÉÇÑ ¾×¼ÇÀÌ°í ±æµå¹æÀº ÀüÃ¼°¡ ºê·ÎµåÄ³½ºÆ®¿µ¿ªÀ¸·Î ¼³Á¤ÇÑ´Ù°í ¾à¼ÓµÊ.)
		GUILD_SC_PLANT,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
		// UINT32			uiCropID;							// ¹¦¸ñ¾ÆÀÌµð

		// ¹¦¸ñ»Ì±â
		GUILD_CS_GRUB,

		// ¹¦¸ñ»Ì±â °á°ú. ÁÖº¯¿¡ ºê·ÎµåÄ³½ºÆ®ÇÕ´Ï´Ù.(´Ü, ±æµå¹æ¿¡¼­¸¸ °¡´ÉÇÑ ¾×¼ÇÀÌ°í ±æµå¹æÀº ÀüÃ¼°¡ ºê·ÎµåÄ³½ºÆ®¿µ¿ªÀ¸·Î ¼³Á¤ÇÑ´Ù°í ¾à¼ÓµÊ.)
		GUILD_SC_GRUB,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// ºñ·á/ÃËÁøÁ¦ ÁÖ±â
		GUILD_CS_NOURISH,
		// UINT32			uiBoosterID;						// ºñ·á/ÃËÁøÁ¦¾ÆÀÌµð
		// TRASH_ITEM		stConsumeItem;						// ¼Ò¸ðµÇ´Â¾ÆÀÌÅÛ

		// ºñ·á/ÃËÁøÁ¦ ÁÖ±â °á°ú. ÁÖº¯¿¡ ºê·ÎµåÄ³½ºÆ®ÇÕ´Ï´Ù.(´Ü, ±æµå¹æ¿¡¼­¸¸ °¡´ÉÇÑ ¾×¼ÇÀÌ°í ±æµå¹æÀº ÀüÃ¼°¡ ºê·ÎµåÄ³½ºÆ®¿µ¿ªÀ¸·Î ¼³Á¤ÇÑ´Ù°í ¾à¼ÓµÊ.)
		GUILD_SC_NOURISH,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
		// UINT32			uiRemainTime;						// ÇöÀç½Ã°£°ú ºñ±³ÇÏ¿© °è»êµÈ ¼ºÀå¿Ï·á±îÁö ³²Àº½Ã°£(ÃÊ´ÜÀ§)
		// UINT32			uiNourishTime;						// ÇöÀç½Ã°£°ú ºñ±³ÇÏ¿© °è»êµÈ ºñ·á¸¦ ¸ÔÀ» ½Ã°£(ÃÊ´ÜÀ§)
		// UINT32			uiHastenSumTime;					// ÃËÁøÁ¦ ´©Àû½Ã°£

		// ¹¦¸ñ¼ºÀå¿Ï·á. ±æµå¹æ³»ÀÇ ¸ðµç PC¿¡°Ô ºê·ÎµåÄ³½ºÆ®ÇÕ´Ï´Ù.
		GUILD_SC_GROWTHCROP,

		// ·©Å©¾÷
		GUILD_CS_RANKUP,
		// UINT8			byNewGRank;							// New±æµå·©Å©
		// TRASH_ITEM		stConsumeItem;						// ¼Ò¸ðµÇ´Â¾ÆÀÌÅÛ

		// ·©Å©¾÷. ±æµå¿øÁÖº¯¿¡ ºê·ÎµåÄ³½ºÆÃÇÏÁö ¾Ê½À´Ï´Ù.(2007.08.13 dhpark À¯¸í»óÆÀÀåÀÌ °áÁ¤)
		GUILD_SC_RANKUP,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
		// INT32			iGNum;								// ±æµåDBNum
		// UINT8			byNewGRank;							// New±æµå·©Å©

		// ±æµå¸¶Å©º¯°æ
		GUILD_CS_CHANGEMARK,
		// UINT32			uiGMarkID;							// ±æµå¸¶Å©LDT¾ÆÀÌµð
		// TRASH_ITEM		stConsumeItem;						// ¼Ò¸ðµÇ´Â¾ÆÀÌÅÛ

		// ±æµå¸¶Å©º¯°æ. ±æµå¿øÁÖº¯¿¡ ºê·ÎµåÄ³½ºÆÃÇÏÁö ¾Ê½À´Ï´Ù.(2007.08.13 dhpark À¯¸í»óÆÀÀåÀÌ °áÁ¤)
		GUILD_SC_CHANGEMARK,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
		// INT32			iGNum;								// ±æµåDBNum
		// UINT8			arrGMark[3];						// ±æµå¸¶Å©

	//////////////////////////////////////////////////////////////////////////
	//
	//	Indun
	//
		PI_CS_INDUN = PI_CS_LOGIC + 2900,

		// ÀÎ´ø ÀÔÀå ¿äÃ»
		INDUN_CS_IN = PI_CS_INDUN,
		// UINT32			uiIndunLDTID;						// ÀÔÀåÇÏ·Á°íÇÏ´Â ÀÎ´øÀÇ LDTID
			
		// ÀÎ´ø ÀÔÀå ¿äÃ» °á°ú
		INDUN_SC_IN,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
		// UINT32			uiIndunID;							// ÀÎ´ø ÀÎ½ºÅÏÆ® ¾ÆÀÌµð
		// UINT16			wServerID;							// ÀÎ´ø ¼­¹ö¾ÆÀÌµð

		// ÀÎ´ø ÅðÀå ¿äÃ» °á°ú. OnPortalEvent()³»¿¡¼­ ºÐ±âÇÏ¿© Ã³¸®µË´Ï´Ù.(ÀÎ´ø¼­¹öÀÌ¸é¼­ ¸ñÇ¥Áö°¡ ÀÎ´øÇü¸Ê±×·ìÅ¸ÀÔÀÌ ¾Æ´Ñ°æ¿ì¿¡ Ã¤³ÎÀÌµ¿À¸·Î ³ª°¡´Â°ÍÀ¸·Î ÆÇ´Ü)
		INDUN_SC_OUT,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
			
		// ÀÎ´ø ÃÊ±âÈ­ ¿äÃ»
		INDUN_CS_INIT,
			
		// ÀÎ´ø ÃÊ±âÈ­ ¿äÃ» °á°ú. ¼ÒÀ¯ÇÏ°í ÀÖ´Â ÀÎ´øÀÌ ¾øÀ¸¸é uiIndunLDTIDÀÌ 0ÀÔ´Ï´Ù.
		INDUN_SC_INIT,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT32			uiIndunLDTID;						// ÀÎ´ø LDT ¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð

	//////////////////////////////////////////////////////////////////////////
	//
	//	Lottery
	//
		PI_CS_LOTTERY = PI_CS_LOGIC + 3000,

		//»Ì±â ÆÇ ¸¸µé°í ½ÃÀÛ.
		LOTTERY_SC_START = PI_CS_LOTTERY, 

		// »Ì±â ¾÷µ¥ÀÌÆ® »Ì±â ÁøÀÔ½Ã³ª ¾÷µ¥ÀÌÆ®¹öÆ° ´©¸¦¶§
		LOTTERY_CS_UPDATE ,
		// UINT32			iVer;

		// »Ì±â ¾÷µ¥ÀÌÆ® °á°ú
		LOTTERY_SC_UPDATE,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
		// UINT32			iVer;								// ErrorID°¡ 0 °æ¿ì¸¸ Àü¼Û
		// UINT16			iLotteryCount;						// ErrorID°¡ 0 °æ¿ì¸¸ Àü¼Û
		// UINT8			iNumOfLotteryBoard;					// ErrorID°¡ 0 °æ¿ì¸¸ Àü¼Û
		// UINT32			iLotteryBoard[iNumOfLotteryBoard];					// ErrorID°¡ 0 °æ¿ì¸¸ Àü¼Û
		// UINT8			iNumOfRewardInfo;
		// LOTTERY_REWARD	RewardInfo[iNumOfRewardInfo];

		// »Ì±â
		LOTTERY_CS_CHOICE,
		// UINT32			iVer;								// ÆÇ¹öÀü
		// UINT16			iIndex;								// bitÀÇ¹è¿­ Ã·ÀÚ
		// CONTAINER_SLOT	UsingItemPos;				// »ç¿ëÇÒ ¾ÆÀÌÅÛ À§Ä¡Á¤º¸
		// ITEMNO			iUsingItemNo;				// »ç¿ëÇÒ ¾ÆÀÌÅÛ Å°¹øÈ£
		// int				iUsingEly;					// »ç¿ëÇÒ °ÔÀÓ ¸Ó´Ï	

		// 

		// »Ì±â °á°ú
		LOTTERY_SC_CHOICE,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
		// UINT16			iRewardID;							// ErrorID°¡ 0°æ¿ì¸¸ Àü¼Û
		// CONTAINER_ITEM   tItem;								// ErrorID°¡ 0°æ¿ì¸¸ Àü¼Û 
		// UINT64			iPCCurrEly;							// ErrorID°¡ 0°æ¿ì¸¸ Àü¼Û
		// TRASH_ITEM		TrashItem;							// ErrorID°¡ 0°æ¿ì¸¸ Àü¼Û
		// UINT16			iLotteryCount;						// ErrorID°¡ 0, 13001004 °æ¿ì¸¸ Àü¼Û.
		// UINT8			iNumOfLotteryBoard;					// ErrorID°¡ 0, 13001004 °æ¿ì¸¸ Àü¼Û.
		// UINT32			iLotteryBoard[iNumOfLotteryBoard];	// ErrorID°¡ 0, 13001004 °æ¿ì¸¸ Àü¼Û. iLotteryCount¸¦ Æ÷ÇÔÇÒ¼ö ÀÌ´Â ¹è¿­ °¹¼ö¸¸Å­¸¸

	//////////////////////////////////////////////////////////////////////////
	//
	//	Event Mission
	//
		PI_CS_EVENTMISSION = PI_CS_LOGIC + 3100,

		// ¿þÀÌÆ÷ÀÎÆ® ¸®½ºÆ®
		EVENTMISSION_SC_LIST = PI_CS_EVENTMISSION, // ºí·°¿ä¸Á
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// int				arrMissionFlag[MAX_EVENT_MISSION_DATA];	// CP_EVENT_MISSION.ldt::_Save ¸¶½ºÅ·°ª 

		// ÀÌº¥Æ® ¹ß»ý
		EVENTMISSION_CS_RAISE ,
		// int				iMissionID ;	// CP_EVENT_MISSION.ldt::_Item_ID

		// ÀÌº¥Æ® ¹ß»ý °á°ú. 
		// iMissionType ÀÌ Äù½ºÆ® ÀÌ°í iErrorMsg = 0 ÀÌ¸é ºí·° ÇØÁ¦ ÇÏÁö ¸»¾ÆÁÖ¼¼¿ä. 
		EVENTMISSION_SC_RAISE,
		// GU_ID			ObjectGUID;		// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	iErrorID;		// ¿¡·¯¾ÆÀÌµð,  0 ÀÎ°æ¿ì¸¸ ÀÌÇÏ µ¥ÀÌÅÍ¼Û½Å
		// int				iMissionID;		// CP_EVENT_MISSION.ldt::_Item_ID
		// UINT8			iMissionType;	// CP_EVENT_MISSION.ldt::_MissionType
		// int				iMissionFlag;	// ÀúÀåÇÒ ÀÌº¥Æ®¹Ì¼ÇÇÃ·¡±×°ª(0ÀÌ¸é ÀúÀå¾ÈÇÔ)
		// UINT8			iNumOfItem;		// ¹ÞÀº ¾ÆÀÌÅÛ¼ö
		// CONTAINER_ITEM   stItem[];		// ¹ÞÀº ¾ÆÀÌÅÛ


	//////////////////////////////////////////////////////////////////////////
	//
	//	GAMBLE
	//
		PI_CS_GAMBLE = PI_CS_LOGIC + 3200,

		// »Ì±â
		GAMBLE_CS_REQ	= PI_CS_GAMBLE,
		// int				iGambleID;					// GAMBLE.LDT::_ITEM_ID
		// UINT8			iBettingCount;				// 
		// UINT8			iNumOfUseItem;				// ¼Ò¸ðÇÒ ¾ÆÀÌÅÛ
		// TRASH_ITEM		TrashItem[];				// »ç¿ëÇÒ ¾ÆÀÌÅÛ À§Ä¡Á¤º¸
		// UINT8			iNumOfAutoSell;				// ÀÚµ¿¾ÆÀÌÅÛ ÆÈ±â
		// int				iAutoSellRewardID[];

		// »Ì±â °á°ú
		GAMBLE_SC_REQ,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
		// int				iGambleID;							// ErrorID°¡ 0°æ¿ì¸¸ Àü¼Û
		// UINT8			iBettingCount;
		// int				iGambleRewardID[];					// ErrorID°¡ 0°æ¿ì¸¸ Àü¼Û
		// UINT8			iAutoSell[];						// ÀÚµ¿ÆÇ¸Å
		// UINT8			iNumOfItem;							// ErrorID°¡ 0°æ¿ì¸¸ Àü¼Û
		// CONTAINER_ITEM   tItem;								// ErrorID°¡ 0°æ¿ì¸¸ Àü¼Û 
		// UINT64			iPCCurrEly;							// ErrorID°¡ 0°æ¿ì¸¸ Àü¼Û
		// UINT8			iNumOfTrashItem;					// ErrorID°¡ 0°æ¿ì¸¸ Àü¼Û
		// TRASH_ITEM		TrashItem;							// ErrorID°¡ 0°æ¿ì¸¸ Àü¼Û


	//////////////////////////////////////////////////////////////////////////
	//
	//	PVP
	//
		PI_CS_PVP = PI_CS_LOGIC + 3400,

		// PVP µî·Ï
		PVP_CS_REGIST = PI_CS_PVP,
		// UINT8			byPVPType;							// (1:FDEATH, 2:FTIME, 3:PDEATH, 4:PTIME, 5:GDEATH, 6:GTIME)
		// UINT32			uiPVPScale;							// ¸îÀÎ¿ëÀÎÁö. (1:2ÀÎ¿ë, 2:4ÀÎ¿ë, 4:6ÀÎ¿ë, 8:8ÀÎ¿ë)
			
		// PVP µî·Ï °á°ú
		PVP_SC_REGIST,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð

		// PVP µî·Ï Ãë¼Ò
		PVP_CS_CANCEL,
			
		// PVP µî·Ï Ãë¼Ò °á°ú
		PVP_SC_CANCEL,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
			
		// PVP ÀÔÀå °á°ú(¼­¹ö¿¡¼­ ¸ÅÄªÀÌ µÇ¸é º¸³»ÁÜ)
		PVP_SC_IN,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
		// UINT32			uiIndunID;							// PVP¹æ ÀÎ½ºÅÏÆ® ¾ÆÀÌµð
		// UINT16			wServerID;							// ÀÎ´ø ¼­¹ö¾ÆÀÌµð
			
		// ÀÔÀå½Ã ´ë±âÀÚµéÀÇ ·Îµù»óÅÂ¸¦ ¾Ë·ÁÁÜ(1¸í ·Îµù¿Ï·á½Ã¸¶´Ù ¸ðµç ´ë±âÀÚ¿¡°Ô ¼Û½Å)
		PVP_SC_LOADSTATUS,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// INT32			iMemberCnt;							// ´ë±âÀÚ¼ö
		// PVPLOADSTATUS	stPVPLoadStatus;					// ´ë±âÀÚÀÇ PVP·Îµå»óÅÂ
		// ...
			
		// ÀÔÀå½Ã ´ë±âÀÚµé¿¡°Ô PVP°¡ ½ÃÀÛµÇ¾ú´Ù°í ¾Ë·ÁÁÜ
		PVP_SC_START,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT32			uiLDTID;							// PVP LDT ¾ÆÀÌµð
			
		// PVP ÅðÀå¿äÃ»(PVPÀÎ´ø¿¡´Â ÅðÀå¿ë Æ÷Å»ÀÌ ¾ø¾î¼­ Á¾·áÈÄ ³ª°¥¶§ »ç¿ëÇÔ)
		PVP_CS_OUT,
			
		// PVP ÅðÀå¿äÃ» °á°ú. OnPortalEvent()³»¿¡¼­ ºÐ±âÇÏ¿© Ã³¸®µË´Ï´Ù.(ÀÎ´ø¼­¹öÀÌ¸é¼­ ¸ñÇ¥Áö°¡ ÀÎ´øÇü¸Ê±×·ìÅ¸ÀÔÀÌ ¾Æ´Ñ°æ¿ì¿¡ Ã¤³ÎÀÌµ¿À¸·Î ³ª°¡´Â°ÍÀ¸·Î ÆÇ´Ü)
		// Å¬¶óÀÌ¾ðÆ®°¡ PVPÁ¾·áÈÄ ³ª°¥¶§ PVP_CS_OUTÀ» º¸³»°í ¼­¹ö¿¡¼­´Â À§Ä¡ÀúÀå¸¸ÇÏ°í º¸³»±âµµ ÇÔ.
		PVP_SC_OUT,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
			
		// PVP¼ºÀûÀ» ¼Û½ÅÇÕ´Ï´Ù.
		PVP_SC_RANK,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// INT32			iMemberCnt;							// ´ë±âÀÚ¼ö
		// PVPRANK			stPVPRank;							// ´ë±âÀÚÀÇ PVP¼øÀ§Á¤º¸
		// ...
			
		// PVPÁ¾·á¸¦ ¼Û½ÅÇÕ´Ï´Ù.
		PVP_SC_FINISH,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// INT32			iMemberCnt;							// ´ë±âÀÚ¼ö
		// PVPRANK			stPVPRank;							// ´ë±âÀÚÀÇ PVP¼øÀ§Á¤º¸
		// ...


	//////////////////////////////////////////////////////////////////////////
	//
	//	SERVERINFO : Å¬¶óÀÌ¾ðÆ®ÀÇ ¿äÃ»¿¡ ÀÇÇØ ¼­¹öÁ¤º¸¸¦ ¼Û½ÅÇÕ´Ï´Ù.
	//
		PI_CS_SERVERINFO = PI_CS_LOGIC + 3500,

		// ¼­¹ö½Ã°£/Á¤º¸ ¿äÃ»
		SERVERTIME_CS_INFO = PI_CS_SERVERINFO,

		// ¼­¹ö½Ã°£/Á¤º¸ ¼Û½Å
		SERVERTIME_SC_INFO,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// tm				stServerTime;				// ¼­¹ö½Ã°£
		// WORLD_ID			wWorldID;					// ¿ùµå¾ÆÀÌµð
		// SERVER_ID		wServerID;					// ¼­¹ö¾ÆÀÌµð
		// char				szAccountID[MAX_LEN_ID+1];	// °èÁ¤¾ÆÀÌµð
		// char				szClientIP[LEN_IP+1];		// Å¬¶óÀÌ¾ðÆ®¾ÆÀÌÇÇ
		// TCHAR			szName[LEN_NAME+1];			// Ä³¸¯ÅÍ¸í
		// UINT				uiStageID;					// ½ºÅ×ÀÌÁö¾ÆÀÌµð
		// UINT				uiMapGroupID;				// ¸Ê±×·ì¾ÆÀÌµð
		// UINT				uiPosX;						// XÁÂÇ¥
		// UINT				uiPosY;						// YÁÂÇ¥


	//////////////////////////////////////////////////////////////////////////
	//
	//	Chat Room												// 2008/03/07 by Hoony0109
	//
		PI_CS_CHATROOM = PI_CS_LOGIC + 3600,

		// ´ëÈ­¹æ »ý¼º ¿äÃ»
		CHATROOM_CS_CREATE = PI_CS_CHATROOM, // (Ä³½ÃÅÛ ¼Ò¸ð·Î ¹æÁ¦¸ñ½ºÅ²°ú ¿ÀºêÁ§Æ®ÀÇ º¯°æ ±â´É È®Àå¼º °í·Á.)
		// CHATROOM_INFO	stChatRoomInfo;						// »ý¼ºÇÏ·Á°íÇÏ´Â ´ëÈ­¹æÀÇ Á¤º¸
		// UINT8			iAdvert;							// ±¤°í ¿©ºÎ (0 , 1)
		// if(iAdvert == 1)
			// TRASH_ITEM		stConsumeItem;						// »ç¿ëÇÒ ¾ÆÀÌÅÛ
			// UINT8			iChatRoomAdvertLen;					// ±¤°í ¸Þ¼¼Áö ±æÀÌ
			// TCHAR			szAdvert[iChatRoomAdvertLen];		// ±¤°í ¸Þ¼¼Áö (ÃÖ´ë: MAX_CHATROOM_ADVERT_LEN)

		// ´ëÈ­¹æ »ý¼º °á°ú.	+ INDUN_SC_IN Àü¼Û (±¤°í »ç¿ë½Ã + ITEM_SC_TRASH Àü¼Û)
		CHATROOM_SC_CREATE,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT32			uiMyCharID;
		// UINT8			iAdvert;							// ±¤°í ¿©ºÎ (0, 1)
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
		// if(ErrorID == 0)
			// CHATROOM_INFO	stCRInfo;							// »ý¼ºÇÏ·Á°íÇÏ´Â ´ëÈ­¹æÀÇ Á¤º¸

		// ´ëÈ­¹æ ¿ÀºêÁ§Æ® »ý¼º.
		// CHATROOM_SC_MAKE_OBJECT, => NAV_SC_INSIGHT ·Î º¯°æ
			// GU_ID					ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
			// BG_ID					Layer;
			// UINT						iPosX;						//
			// UINT						iPosY;						//
			// CHATROOM_OBJECT_INFO		stChatRoomObject_Info		//

		// ´ëÈ­¹æ ¿ÀºêÁ§Æ® ÆÄ±«.
		// CHATROOM_SC_DESTROY_OBJECT, => NAV_SC_OUTOFSIGHT ·Î º¯°æ.
			// GU_ID						ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð

		// ´ëÈ­¹æ Âü¿© Àü ¹æ Á¤º¸ È®ÀÎ.
		CHATROOM_CS_ROOMINFO_CHECK,
		// UINT32			uiRoomNo;

		// ¿äÃ»ÇÑ ¹æÁ¤º¸ °á°ú.
		CHATROOM_SC_ROOMINFO_CHECK,
		// GU_ID			globalID;							// »ç¿ëÀÚ ¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
		// if(0 == _ErrorID)
			// CHATROOM_INFO	stChatRoomInfo;						// »ý¼ºÇÏ·Á°íÇÏ´Â ´ëÈ­¹æÀÇ Á¤º¸

		// ´ëÈ­¹æ Âü¿© ¿äÃ». (ÀÌ¹Ì ActPC °¡ 2¹ø °­ÅðµÈ ¹æÀÎÁö Ã¼Å©.) (ÀÎ´ø, ´ëÈ­¹æ¿¡¼­ ¶Ç ÀÌµ¿ÇÏ´Â °æ¿ì Ã³¸®.)
		CHATROOM_CS_JOIN,
		// CHATROOM_INFO	stChatRoomInfo;						// ÀÔÀåÇÏ·Á°íÇÏ´Â ´ëÈ­¹æÀÇ Á¤º¸

		// ´ëÈ­¹æ Âü¿© °á°ú.	+ INDUN_SC_IN Àü¼Û
		CHATROOM_SC_JOIN,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT32			uiMyCharID;
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
		// if(ErrorID == 0)
			// CHATROOM_INFO	stCRInfo;							// ÀÔÀåÇÏ·Á°íÇÏ´Â ´ëÈ­¹æÀÇ Á¤º¸

		// ´ëÈ­¹æ Àç¼öÁ¤ ¿äÃ».
		CHATROOM_CS_MODIFY,
		// UINT8			iPassWord;							// 0: ÆÐ½º¿öµå ¹Ìº¯°æ, 1: ÆÐ½º¿öµå º¯°æ Æ÷ÇÔ½Ã.
		// CHATROOM_INFO	stCRInfo;							// ¼öÁ¤ÇÏ·Á°íÇÏ´Â ´ëÈ­¹æÀÇ Á¤º¸

		// ´ëÈ­¹æ Àç¼öÁ¤ °á°ú. (°­ÅðÀÚ ¸ñ·Ï ÀüºÎ »èÁ¦. - ÆÐ½º¿öµå¸¸ º¯°æ½Ã´Â Á¦¿Ü.)
		CHATROOM_SC_MODIFY,
		// GU_ID				globalID;						// »ç¿ëÀÚ ¾ÆÀÌµð
		// GLOBAL_STRING_ID		iError;							// 0 : ¼º°ø
		// CHATROOM_INFO		stCRInfo;						// ¼öÁ¤ÇÏ·Á°íÇÏ´Â ´ëÈ­¹æÀÇ Á¤º¸

		// ´ëÈ­¹æ ÅðÀå ¿äÃ».
		CHATROOM_CS_OUT,
		// UINT32			uiID;								// ´ëÈ­¹æ ÀÎ½ºÅÏ½º ¾ÆÀÌµð (Unique Key)
		// UINT32			uiRoomNo;							// ÅðÀåÇÏ·Á°í ÇÏ´Â ´ëÈ­¹æÀÇ ¹øÈ£.
		// UINT8			iBanOut;							// 0: ÀÏ¹ÝÅðÀå 1: °­Á¦ÅðÀå

		// ´ëÈ­¹æ ÅðÀå °á°ú.	+ INDUN_SC_OUT Àü¼Û
		CHATROOM_SC_OUT,	
		// GU_ID			globalID;							// »ç¿ëÀÚ ¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð

		// ¹æÀåÀÇ °­Á¦ ÅðÀå ¿äÃ». (ÃÖ´ë 2¹øÀÇ °­Åð È½¼ö ¹× ¹æ¹øÈ£ ÀúÀå.) ¹æÁ¤º¸¿¡ Âü¿©ÀÚ ¸ñ·Ï°ú °­ÅðÀÚ ¸ñ·Ï ¹× È½¼ö ÀúÀå.
		CHATROOM_CS_FORCEOUT,
		// UINT32			uiID;								// ´ëÈ­¹æ ÀÎ½ºÅÏ½º ¾ÆÀÌµð (Unique Key)
		// UINT32			uiRoomNo;							// ÅðÀå½ÃÅ°·Á°í ÇÏ´Â ´ëÈ­¹æÀÇ ¹øÈ£.
		// UINT32			uiCharID;							// °­Á¦ ÅðÀå ´ë»ó.

		// ¹æÀåÀÇ °­Á¦ ÅðÀå ¿äÃ» ½ÇÆÐ. (¼º°ø½Ã CHATROOM_SC_BROADCAST_UNITED_MESSAGE ¸Þ¼¼Áö ¼Û½Å.)
		CHATROOM_SC_FORCEOUT,
		// GU_ID				globalID;						// »ç¿ëÀÚ ¾ÆÀÌµð
		// GLOBAL_STRING_ID		iError;							// 0 : ¼º°ø
		// if(0 == iError)
			// UINT8				iFlag;						// 0: ¹æÀåÀÇ °­Åð ¿äÃ»ÀÇ ÀÀ´ä. 1:°­Á¦ ÅðÀå ´ë»óÀÚ.

		// ¹æÀå ¾çµµ ¿äÃ».
		CHATROOM_CS_CHANGEMASTER,
		// UINT32			uiTargetCharID;
		// UINT32			uiRoomID;
		// UINT32			uiRoomNo;

		// ¹æÀå ¾çµµ ¿äÃ» °á°ú.
		CHATROOM_SC_CHANGEMASTER,
		// GU_ID			globalID;							// »ç¿ëÀÚ ¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð

		CHATROOM_SC_BROADCAST_UNITED_MESSAGE, // (0:Join, 1:Out, 2:Forceout, 3:ChangeMaster)
		// UINT8			iFlag;
		// ROOM_MEMBER		stRoomMember;

		// ´ëÈ­¹æ Âü¿©ÀÚ ¸ñ·Ï Àü¼Û.
		CHATROOM_SC_MEMBERLIST,
		// UINT32			uiSize;
		// ROOM_MEMBER		stRoomMember[uiSize];

		// ´ëÈ­¹æ ±¤°í ¿äÃ». (¹æ »ý¼º°ú ÀÔÀå ÀÌÈÄ ÀÌÈÄ ...) -> SCOPE_CHATROOM_ADVERT Ãß°¡ÇÏ´Â ¹æ½ÄÀ¸·Î...
		CHATROOM_CS_ADVERT,
		// UINT32			uiRoomID;
		// UINT32			uiRoomNo;
		// TRASH_ITEM		stConsumeItem;								// »ç¿ëÇÒ ¾ÆÀÌÅÛ
		// UINT16			iAdvertSize;
		// TCHAR			szAdvertMessage[iAdvertSize]				// iAdvertSize <= MAX_CHATROOM_ADVERT_LEN

		// ´ëÈ­¹æ ±¤°í °á°ú. (¾ÆÀÌÅÛ ¼Ò¸ð => ITEM_SC_TRASH)
		CHATROOM_SC_ADVERT,
		// GU_ID			globalID;							// »ç¿ëÀÚ ¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð

		// ´ëÈ­¹æ ¸ñ·Ï ¿äÃ». (¿äÃ»½Ã ÃÊ±âÈ­)
		CHATROOM_CS_ROOMLIST,
		// UINT8			iRoomType;

		// ´ëÈ­¹æ ¸ñ·Ï °á°ú. (º¹¼ö°³ÀÇ ÆÐÅ¶À¸·Î ³ª´©¾î Àü¼Û.)
		CHATROOM_SC_ROOMLIST,
		// GU_ID			globalID;						// »ç¿ëÀÚ ¾ÆÀÌµð
		// GLOBAL_STRING_ID ErrorID;
		// if(ErrorID == 0)
			// UINT8			iResetFlag;						// 1 = ¸®½ºÆ® ÃÊ±âÈ­, 0 = ¸®½ºÆ® Àü¼ÛÁß, 2 = ¸®½ºÆ® ¸¶Áö¸·, 3 = ¸®½ºÆ® ½ÃÀÛÀÌÀÚ ¸¶Áö¸·(¹æÀÇ ÃÑ °¹¼ö°¡ 1°³ÀÇ ÆÐÅ¶¿¡ ´Ù ´ã°ÜÁüÀ» ÀÇ¹ÌÇÔ)
			// UINT8			iRoomType;
			// UINT32			uiCount;
			// CHATROOM_INFO	stChatRoomInfo[uiCount];

		// ´ëÈ­ »ó´ë ¸ñ·Ï ¿äÃ».
		CHATROOM_CS_USERLIST,
		// UINT8			iSex;
		// UINT16			iMinLevel;
		// UINT16			iMaxLevel;
		// PCSTATUS_STYLE	stStyle;

		// ´ëÈ­ »ó´ë ¸ñ·Ï °á°ú.
		CHATROOM_SC_USERLIST,
		// GU_ID				globalID;						// »ç¿ëÀÚ ¾ÆÀÌµð
		// GLOBAL_STRING_ID		ErrorID;
		// if(ErrorID == 0)
			// UINT32				uiCount;
			// CHARACTER_BASICINFO	stCharInfo[uiCount];

		// ÃÊ´ëÀÇ Á¤»ó Flow (A-S-B)
		// CHATROOM_CS_INVITE_REQ (A -> S) - CHATROOM_SC_INVITE_REQ (S -> B) - CHATROOM_CS_INVITE_ANS (B -> S) - CHATROOM_SC_INVITE_ANS (S -> A)
		// ÃÊ´ë ÀÚÃ¼ÀÇ ½ÇÆÐÀÇ °æ¿ì
		// CHATROOM_CS_INVITE_REQ (A -> S) - CHATROOM_SC_INVITE_REQ (S -> A)
		// ÃÊ´ë ÈÄ ÃÊ´ëÀÚ¿¡ ÀÇÇÑ ½ÇÆÐÀÇ °æ¿ì
		// CHATROOM_CS_INVITE_REQ (A -> S) - CHATROOM_SC_INVITE_REQ (S -> B) - CHATROOM_CS_INVITE_ANS (B -> S) - CHATROOM_SC_INVITE_ANS (S -> B)

		// ´ëÈ­ »ó´ë ÃÊ´ë ¿äÃ» (½ÅÃ»ÀÚ). // 1:1 ´ëÈ­ ÃÊ´ë Æ÷ÇÔ (Client A -> Sv)
		CHATROOM_CS_INVITE_REQ,
		// TCHAR			szRcvCharName[LEN_NAME+1];				// ´ë»óÀÚÀÌ¸§
		// UINT8			uiInviteInRoom;							// ÃÊ´ëÀÚ »óÅÂ.(ÀÌ¹Ì »ý¼ºµÈ ¹æ¿¡¼­ ÃÊ´ëÇÏ´Â °ÍÀÎÁö(1), 1:1´ëÈ­ ÃÊ´ëÀÎÁö(0))
		// CHATROOM_INFO	stChatRoomInfo;

		// ´ëÈ­ »ó´ë ÃÊ´ë (½ÅÃ» ´ë»ó). // 1:1 ´ëÈ­ ÃÊ´ë Æ÷ÇÔ (Sv -> Client B)
		CHATROOM_SC_INVITE_REQ,
		// GU_ID				globalID;						// »ç¿ëÀÚ ¾ÆÀÌµð
		// TCHAR				szTargetName[LEN_NAME+1];
		// GLOBAL_STRING_ID		ErrorID;
		// if(0 == ErrorID)
			// TCHAR				szActorName[LEN_NAME+1];
			// UINT8				uiInviteInRoom;
			// CHATROOM_INFO		stChatRoomInfo;

		// ´ëÈ­ »ó´ë ÃÊ´ë ÀÀ´ä (½ÅÃ» ´ë»ó). // 1:1 ´ëÈ­ ÃÊ´ë Æ÷ÇÔ (Client B -> Sv)
		CHATROOM_CS_INVITE_ANS,
		// UINT8				iAllowed;						// 0: ÃÊ´ë °ÅºÎ, 1: ÃÊ´ë ¼ö¶ô.
		// TCHAR				szActorName[LEN_NAME+1];
		// UINT8				uiInviteInRoom;
		// CHATROOM_INFO		stChatRoomInfo;

		// ´ëÈ­ »ó´ë ÃÊ´ë °á°ú (½ÅÃ»ÀÚ). // 1:1 ´ëÈ­ ÃÊ´ë Æ÷ÇÔ (Sv -> Client A)
		CHATROOM_SC_INVITE_ANS,
		// GU_ID				globalID;						// »ç¿ëÀÚ ¾ÆÀÌµð
		// TCHAR				szTargetName[LEN_NAME+1];
		// GLOBAL_STRING_ID		ErrorID;

		// ´ëÈ­»ó´ë Æ¯Á¤ PC Á¤º¸ ¿äÃ».
		CHATROOM_CS_OTHERPC_INFO,
		// TCHAR			szCharName[LEN_NAME+1];

		// ´ëÈ­»ó´ë ³» Æ¯Á¤ PC Á¤º¸ °á°ú.
		CHATROOM_SC_OTHERPC_INFO,
		// GU_ID			globalID;							// »ç¿ëÀÚ ¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;							// ¿¡·¯¾ÆÀÌµð
		// UINT8			iFlag;								// 0: Á¤»ó, 1: ÀÚ½ÅÀÌ ½ºÅ¸ÀÏ ºñ°ø°³, 2: ´ë»óÀÌ ½ºÅ¸ÀÏ ºñ°ø°³
		// if(0 == ErrorID)
			// CHARACTER_INFO2	stCharInfo2;
			// UINT16			iTotalSize;
			// FIGURE_ITEM		stFigureItemList[iTotalSize];
			// PCSTATUS_STYLE	stStyle;

		// ½ºÅ¸ÀÏ ¼³Á¤ ÀúÀå ¿äÃ». (º°µµ Tabel¿¡ ÀúÀå)
		CHATROOM_CS_SET_MYSTYLE,
		// PCSTATUS_STYLE	stStyle;

		// ½ºÅ¸ÀÏ Á¤º¸ °á°ú.
		CHATROOM_SC_GET_MYSTYLE,
		// GU_ID			ObjectGUID;							// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// PCSTATUS_STYLE	stStyle;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Auction												// 2008/03/07 by Hoony0109
	//
		PI_CS_AUCTION = PI_CS_LOGIC + 3700,		

		// °æ¸ÅÀå¿ÀÇÂ( °ÔÀÓÁøÇàÁß °æ¸ÅÀåÀÌ ¶ß°Ô µÈ°æ¿ì)
		AUCTION_SC_OPEN,
		// GLOBAL_STRING_ID	iErrorID;							// ¿¡·¯¾ÆÀÌµð

					
		// ¹°°Ç µî·Ï
		AUCTION_CS_CREATE,
		// UINT8	iType;				//°Å·¡Å¸ÀÔ(AUCTION_TYPE)
		// UINT8	iDuration;			//±â°£Å¸ÀÔ(AUCTION_DURATION )
		// __int64	iStartingPrice;		//½ÃÀÛ°¡
		// __int64	iBuyOutPrice;		//Áï±¸°¡
		// __int64	iEnrolPrice;		//µî·Ï¼ö¼ö·á
		// TRASH_ITEM stUseItem;		//ÇÊ¿ä¾ÆÀÌÅÛÀ§Ä¡¹× °¹¼ö(1)
		// ITEM_NO	iUseItemNo;			//ÇÊ¿ä¾ÆÀÌÅÛÅ°
		//-------------------------------------------------
		//	iType == AUCTION_TYPE_ITEM_BY_ELY
		//		CONTAINER_SLOT		stUpItemPos;
		//		ITEM_NO				iUpItemNo;
		//	else
		//		AUCTION_GOODS_POINT stUpPoint;

		// ¹°°Çµî·Ï °á°ú
		// ÆÇ¸Å¸®½ºÆ® ÇÑ¹øµµ ¹ÞÁö ¾ÊÀº°æ¿ì´Â ¹«½Ã
		AUCTION_SC_CREATE,
		// GU_ID			iObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	iErrorID;							// ¿¡·¯¾ÆÀÌµð
		// iErrorID == 0
			// AUCTION_HEADER	stAuctionHeader;					// ±âº»Á¤º¸
			// AUCTION_GOODS_XX stAuctionGoods;						// ¿Ã¸° ¹°°Ç Á¤º¸( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)
			// __int64			iCurrEly;							// ÇöÀç³²Àºµ· (µî·Ï¼ö¼ö·á or ¿Ã¸® µ·À» Á¦ÇÏ°í ÇöÀç °¡Áö°íÀÖ´Âµ·)
			// TRASH_ITEM		stUseItem;							// ÇÊ¿ä¾ÆÀÌÅÛ
			//-------------------------------------------------
			//	stAuctionHeader.iType == AUCTION_TYPE_ITEM_BY_ELY
			//		TRASH_ITEM			stUpItem;					// ¿Ã¸°Z¾ÆÀÌÅÛ»èÁ¦Á¤º¸
			//	else
			//		CHARACTER_CASH_INFO stCash;						// ÇöÀçÄ³½¬Á¤º¸
		// iErrorID != 0
			// CHARACTER_CASH_INFO stCash;	


		// Ãë¼Ò ¿äÃ»
		AUCTION_CS_CANCEL,
		//__int64	iAuctionID;									// °æ¸ÅÅ°
		// int		iCategory;									// Item.ldt::AuctionCategory or 8000 ÀÌ»ó
	
		// Ãë¼Ò ¿äÃ» °á°ú(µî·ÏÀÚ)
   		AUCTION_SC_CANCEL,
		// GU_ID			iObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	iErrorID;							// ¿¡·¯¾ÆÀÌµð
		// iErrorID == 0
			// __int64			iAuctionID;							// °æ¸ÅÅ°
			// CHARACTER_CASH_INFO stCash;							// 
		// iErrorID != 0
			// CHARACTER_CASH_INFO stCash;							// 


		// ¹°°ÇÁ¾·á
		//		- À¯Âû½Ã ÆÇ¸ÅÀÚ/ÀÔÂûÀÚ¿¡°Ô 
		//		- Ãë¼Ò½Ã ÀÔÂûÀÚ¿¡°Ô
		AUCTION_SC_FINISH,
		// GU_ID			iObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	iErrorID;							// ¿¡·¯¾ÆÀÌµð
		// iErrorID == 0
			// __int64			iAuctionID;							// °æ¸ÅÅ°
			// CHARACTER_CASH_INFO stCash;							// 
		// iErrorID != 0
			// CHARACTER_CASH_INFO stCash;							// 

		// ³«Âû¿Ï·á
		//		- ³«Âû½Ã ÆÇ¸ÅÀÚ/³«ÂûÀÚ¿¡°Ô
		//		- Áï±¸½Ã ÆÇ¸ÅÀÚ¿¡°Ô
		AUCTION_SC_FINISH_SELL,
		// GU_ID			iObjectID;
		// __int64			iAuctionID;							// °æ¸ÅÅ°
		// USERID			iBuyerID;							// ±¸¸ÅÀÚ
		// CHARACTER_CASH_INFO stCash;							// 

		// ÀÔÂûÇÏ±â¿äÃ»
		AUCTION_CS_BID,
		// __int64			iAuctionID;							// °æ¸ÅÅ°
		// int				iCategory;							// Item.ldt::AuctionCategory or 8000 ÀÌ»ó
		// __int64			iBidPrice;							// ÀÔÂû°¡

		// ÀÔÂûÁ¤º¸ : ÆÇ¸ÅÀÚ ÀÔÂûÀÚ¿¡°Ô °»½Å
		// ÆÇ¸Å¸®½ºÆ®/ÀÔÂû¸®½ºÆ®¸¦ ÇÑ¹øµµ ¹ÞÁö ¾ÊÀº°æ¿ì´Â ¹«½Ã
		AUCTION_SC_BID,
		// GU_ID			iObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	iErrorID;							// ¿¡·¯¾ÆÀÌµð
		// iErrorID == 0
			// AUCTION_HEADER	stAuctionHeader;					// ±âº»Á¤º¸
			// AUCTION_GOODS_XX stAuctionGoods;						// ¿Ã¸° ¹°°Ç Á¤º¸( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)
			// __int64			iCurrEly;							// ÇöÀç³²Àºµ· (µî·Ï¼ö¼ö·á or ¿Ã¸® µ·À» Á¦ÇÏ°í ÇöÀç °¡Áö°íÀÖ´Âµ·)
			// CHARACTER_CASH_INFO stCash;							// ÇöÀçÄ³½¬Á¤º¸							
		// iErrorID != 0
			// CHARACTER_CASH_INFO stCash;							// 


		// Áï±¸¿äÃ»
		AUCTION_CS_BUYOUT,
		// __int64			iAuctionID;							// °æ¸ÅÅ°
		// int				iCategory;							// Item.ldt::AuctionCategory or 8000 ÀÌ»ó
		// __int64			iBidPrice;							// Áï±¸°¡
		// UINT8			iRefresh;							// 1: °»½Å 0: °»½Å¾ÈÇÔ
		// int				iStartNumber;						// ¸î¹øÂ°°ÍºÎÅÍ ´õ ¹Þ°í ½ÍÀºÁö
		// int				iCategory;							// 
		// UINT8			iSortKey;							// AUCTION_SORT_ITEMKEY(À¯Àú°¡ ÁöÁ¤ÇÏÁö ¾ÊÀº°æ¿ìÀÌ¸é ±âÈ¹¼­¿¡ Á¤ÇÑ°ª)
		// UINT8			IsAsc;								// 1: ¿À¸§Â÷¼ø 0: ³»¸²Â÷¼ø
		// iCategory < 8000
			// TCHAR			szItemName[MAX_ITEM_NAME];		// ¾ÆÀÌÅÛ¸í
			// short			iItemLvMin;							// ¾ÆÀÌÅÛ·¹º§ÀÌ»ó: ±âº» 1
			// short			iItemLvMax;							// ¾ÆÀÌÅÛ·¹º§ÀÌÇÏ: ±âº» 999 (°Ë»ö°¡´É ¹üÀ§°¡ Ã¼Å©½Ã º»ÀÎ·¹º§±îÁö)
			// UINT8			iItemQuility;						// ¾ÆÀÌÅÛÇ°Áú
		// iCategory >= 8000
			// int				iMinPoint;							// ÀüÃ¼°Ë»öÀÏ°æ¿ì iMinPoint iMaxPoint 0
			// int				iMaxPoint;

		// Áï±¸¿äÃ»°á°ú
		AUCTION_SC_BUYOUT,
		// GU_ID			iObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	iErrorID;							// ¿¡·¯¾ÆÀÌµð
		// iErrorID == 0
			// __int64			iAuctionID;							// °æ¸ÅÅ°
			// __int64			iCurrMoney;							// °æ¸ÅÅ°
			// CHARACTER_CASH_INFO stCash;							// 
		// iErrorID != 0
			// CHARACTER_CASH_INFO stCash;							// 



		//-----------------------------------------------------------------
		// ¹°°Ç°Ë»ö(
		AUCTION_CS_SEARCH,
		// int				iCategory;							// 
		// UINT8			iSortKey;							// AUCTION_SORT_ITEMKEY(À¯Àú°¡ ÁöÁ¤ÇÏÁö ¾ÊÀº°æ¿ìÀÌ¸é ±âÈ¹¼­¿¡ Á¤ÇÑ°ª)
		// UINT8			IsAsc;								// 1: ¿À¸§Â÷¼ø 0: ³»¸²Â÷¼ø
		// iCategory < 8000
			// TCHAR			szItemName[MAX_ITEM_NAME];		// ¾ÆÀÌÅÛ¸í : ÀüÃ¼(NULL)
			// short			iItemLvMin;							// ¾ÆÀÌÅÛ·¹º§ÀÌ»ó: ±âº» 1
			// short			iItemLvMax;							// ¾ÆÀÌÅÛ·¹º§ÀÌÇÏ: ±âº» 999 (°Ë»ö°¡´É ¹üÀ§°¡ Ã¼Å©½Ã º»ÀÎ·¹º§±îÁö)
			// UINT8			iItemQuility;						// ¾ÆÀÌÅÛÇ°Áú : ÀüÃ¼Ç°Áú(0)
		// iCategory >= 8000
			// int				iMinPoint;							// ÀüÃ¼°Ë»öÀÏ°æ¿ì iMinPoint iMaxPoint 0
			// int				iMaxPoint;


		// ¹°°Ç°Ë»öÀÌÀÌ¹Ì ÀÌ·ç¾îÁø »óÅÂ¿¡¼­ ¼ÒÆ®¸¸ ÇØ¼­ ³»·Á°¥°æ¿ì
		// Item Auction/ ¾Æºñ¿Àº£ÀÌ °øÅë
		AUCTION_CS_SORTING, // ¹«Á¶°Ç Ã¹ÆäÀÌÁö·Î °£´Ù
		// int				iCategory;							// 
		// UINT8			iSortKey;							// AUCTION_SORT_ITEMKEY(À¯Àú°¡ ÁöÁ¤ÇÏÁö ¾ÊÀº°æ¿ìÀÌ¸é ±âÈ¹¼­¿¡ Á¤ÇÑ°ª)
		// UINT8			IsAsc;								// 1: ¿À¸§Â÷¼ø 0: ³»¸²Â÷¼ø
		// iCategory < 8000
			// TCHAR			szItemName[MAX_ITEM_NAME];		// ¾ÆÀÌÅÛ¸í
			// short			iItemLvMin;							// ¾ÆÀÌÅÛ·¹º§ÀÌ»ó: ±âº» 1
			// short			iItemLvMax;							// ¾ÆÀÌÅÛ·¹º§ÀÌÇÏ: ±âº» 999 (°Ë»ö°¡´É ¹üÀ§°¡ Ã¼Å©½Ã º»ÀÎ·¹º§±îÁö)
			// UINT8			iItemQuility;						// ¾ÆÀÌÅÛÇ°Áú								// 1: ¿À¸§Â÷¼ø 0: ³»¸²Â÷¼ø
		// iCategory >= 8000
			// int				iMinPoint;							// ÀüÃ¼°Ë»öÀÏ°æ¿ì iMinPoint iMaxPoint 0
			// int				iMaxPoint;

		// °Ë»ö°ú¼ÒÆ®°¡ ÀÌ·ç¾îÁø»óÅÂ¿¡¼­ °¹¼ö¸¦ ´õ ¹Þ¾Æ³»°í ½ÍÀ»¶§
		AUCTION_CS_GETNEXT,
		// int				iStartNumber;						// ¸î¹øÂ°°ÍºÎÅÍ ´õ ¹Þ°í ½ÍÀºÁö
		// int				iCategory;							// 
		// UINT8			iSortKey;							// AUCTION_SORT_ITEMKEY(À¯Àú°¡ ÁöÁ¤ÇÏÁö ¾ÊÀº°æ¿ìÀÌ¸é ±âÈ¹¼­¿¡ Á¤ÇÑ°ª)
		// UINT8			IsAsc;								// 1: ¿À¸§Â÷¼ø 0: ³»¸²Â÷¼ø
		// iCategory < 8000
			// TCHAR			szItemName[MAX_ITEM_NAME];		// ¾ÆÀÌÅÛ¸í
			// short			iItemLvMin;							// ¾ÆÀÌÅÛ·¹º§ÀÌ»ó: ±âº» 1
			// short			iItemLvMax;							// ¾ÆÀÌÅÛ·¹º§ÀÌÇÏ: ±âº» 999 (°Ë»ö°¡´É ¹üÀ§°¡ Ã¼Å©½Ã º»ÀÎ·¹º§±îÁö)
			// UINT8			iItemQuility;						// ¾ÆÀÌÅÛÇ°Áú		
		// iCategory >= 8000
			// int				iMinPoint;							// ÀüÃ¼°Ë»öÀÏ°æ¿ì iMinPoint iMaxPoint 0
			// int				iMaxPoint;

		// ¹°°Ç°Ë»ö/¼ÒÆ®/´õ¹Þ¾Æ¿À±â °á°ú
		AUCTION_SC_SEARCH_INFO,
		// GU_ID			iObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID iErrorMsg ;							// iErrorMsg != 0 ¸é ¿¡·¯ ¿¡·¯¸é ÀÌÈÄ ¹«½Ã
		// int				iNumOfTotalCount;					// ÃÑ°¹¼ö
		// int				iStartNumber;						// ÇöÀçÀü¼ÛÃ³À½°ªÀÌ ÃÑ°¹¼öÁß ¸î¹øÂ°°ª¿¡ ÇØ´çÇÏ´ÂÁö
		// UINT8			iNumOfAuction;						// Àü¼ÛµÉ¿Á¼ÇÁ¤º¸°¹¼ö
			// AUCTION_HEADER	stAuctionHeader;					// ±âº»Á¤º¸
			// AUCTION_GOODS_XX stAuctionGoods;						// ¿Ã¸° ¹°°Ç Á¤º¸( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)


		//--------------------------------------------------------------------

		// ÀÚ±âÀÔÂûÁ¤º¸ ¾ò¾î¿À±â 
		//		- °ÔÀÓÁøÀÔÀÌÈÄ ÃÖÃÊ ÀÔÂûÅÇÀ» ´­·¶À»¶§ °æ¿ìÀÌ°Å³ª 
		//		- AUCTION_SC_OPEN ÀÌÈÄ Ã³À½ ÀÔÂûÅÇÀ» ´­·¶À»¶§ ¹ß»ý.
		AUCTION_CS_INFO_4BIDDER,			

		AUCTION_SC_INFO_4BIDDER,
		// GU_ID			iObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	iErrorID;							// ¿¡·¯¾ÆÀÌµð

		AUCTION_SC_BIDLIST,
		// GU_ID			iObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT8			iNumOfAuction;						// Àü¼ÛµÉ¿Á¼ÇÁ¤º¸°¹¼ö
			// AUCTION_HEADER	stAuctionHeader;					// ±âº»Á¤º¸
			// AUCTION_GOODS_XX stAuctionGoods;						// ¿Ã¸° ¹°°Ç Á¤º¸( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)

		//	»óÈ¸ÀÔÂû½Ã ÀÌÀüÀÔÂûÀÚ¿¡°Ô ÀÔÂûÁ¤º¸»èÁ¦
		AUCTION_SC_BIDDEL,
		// GU_ID			iObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	iErrorID;							// ¿¡·¯¾ÆÀÌµð
		// __int64			iAuctionID;							// °æ¸ÅÅ°

		// ÀÚ±â °æ¸ÅÁ¤º¸ ¾ò¾î¿À±â
		//		- °ÔÀÓÁøÀÔÀÌÈÄ ÃÖÃÊ ÀÔÂûÅÇÀ» ´­·¶À»¶§¹ß»ý °æ¿ìÀÌ°Å³ª
		//		- AUCTION_SC_OPEN ÀÌÈÄ Ã³À½ ÀÔÂûÅÇÀ» ´­·¶À»¶§ ¹ß»ý.
		AUCTION_CS_INFO_4SELLER,

		// ÀÚ±â °æ¸ÅÁ¤º¸
		AUCTION_SC_INFO_4SELLER,
		// GU_ID			iObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	iErrorID;							// ¿¡·¯¾ÆÀÌµð
	
		AUCTION_SC_SELLLIST,
		// GU_ID			iObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT8			iNumOfAuction;						// Àü¼ÛµÉ¿Á¼ÇÁ¤º¸°¹¼ö
			// AUCTION_HEADER	stAuctionHeader;					// ±âº»Á¤º¸
			// AUCTION_GOODS_XX stAuctionGoods;						// ¿Ã¸° ¹°°Ç Á¤º¸( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)

		//--------------------------------------------------------------------
		// Greener Guide
		
		GREENERGUIDE_SC_EVENT,
		// UINT32	iStartLevel

		GREENERGUIDE_CS_FETCHAWARD,
		// UINT32	iFetchLevel

		GREENERGUIDE_SC_FETECHAWARD,
		// UINT32	iFetchLevel
		// GLOBAL_STRING_ID	bResult
		// UINT32	iNotifyCode	(0: ÄúµÄÉý¼¶ÀñÎï½«È«²¿·¢ËÍµ½ÄúµÄÓÊÏä£¬ÇëÈ¥¸÷³ÇÄÚÓÊµÝÔ±´¦ÁìÈ¡¡£
		//						(1: ÄúµÄ°ü¹üÒÑÂú£¬ÉÐÎ´ÁìÈ¡ÀñÎï½«·¢ÖÁÄúµÄÓÊÏä£¬ÇëÈ¥¸÷³ÇÄÚÓÊµÝÔ±´¦ÁìÈ¡¡£
		//						(2: ³É¹¦ÁìÈ¡


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

		//	°Ù±¦ÏäµÄÐ­Òé°ü
		//	Íæ¼Ò¹ºÂò°Ù±¦Ïä   2008£­9£­12 ÎÂÊ¤Ãû
		PI_GAMBLE_LOGIC		= PI_CS_LOGIC + 3900,

		GAMBLE_SC_INITIAL	= PI_GAMBLE_LOGIC,
		//  UINT64 gambleVersion;
		//	UNSIGNED SHORT      usServerPoit;		//	ÊÇ·ñÊÇ·þÎñÆ÷½Úµã¹ºÂò

		GAMBLE_CS_GET_STATUS,	
		//	
		GAMBLE_SC_GET_STATUS,
		//	GLOBAL_STRING_ID	ErrID;
		//	unsigned short		usFinished;    // ÊÇ·ñ·¢ËÍÍê±Ï
		//	UINT64				GambleVersion;
		//	UNSIGNED SHORT      usServerPoit;		//	ÊÇ·ñÊÇ·þÎñÆ÷½Úµã¹ºÂò
		//	UINT				count;
		//  t_Gamble_Status		status[];
		
		GAMBLE_SC_UPDATE,					//	¸üÐÂ°Ù±¦Ïä¶ÔÓ¦µÄ×´Ì¬ÐÅÏ¢
		//	UINT64 gambleVersion;			//	°Ù±¦ÏäµÄ°æ±¾ºÅ
		//	t_Gamble_Status status;			//	°Ù±¦ÏäµÄ×´Ì¬	
		//	char	szName[LEN_NAME+1];		//	Íæ¼ÒÃû³Æ
			
		GAMBLE_CS_BUY,
		//	UINT64  gambleVersion;		//	°æ±¾ºÅ
		//	UINT32	uIndex;				//	Î»ÖÃ
		//	CONTAINER_SLOT ContainerSlot;	//	ÎïÆ·Î»ÖÃ,ContainerSlot.ContainerTypeÎªCONTAINER_TYPE_INVALID±íÊ¾ÓÃµã¾í¹ºÂò
		//	ITEMNO	ItemNO;				//	ÎïÆ·µÄ±àºÅ

		GAMBLE_SC_BUY,
		//	GLOBAL_STRING_ID	iErrorID;			//	0ÎªÃ»ÓÐ´íÎó£¬1Îª°æ±¾³ö´í£¬¿Í»§¶ËÐèÒª¸üÐÂ°Ù±¦Ïä,2Îª°Ù±¦ÏäÔÝÊ±ÎÞ·¨Ê¹ÓÃ
													//	3Îªµã¾í²»×ã£¬4ÎªÎïÆ·´íÎó£¬5Îª¹ºÂòµÄ°Ù±¦ÏäÎ»ÖÃ²»¶Ô£¬6ÎªÒÑ¾­±»ËûÈË¹ºÂò
													//	7ÎªÍæ¼ÒÕýÔÚÉÏ´Î¹ºÂòµÄ×´Ì¬£¬¼´Î´Íê³ÉÉÏ´Î¹ºÂò
		//	CHARID				charID;				//	Íæ¼ÒµÄIDºÅ
		//	UINT64				gambleVersion;		//	°æ±¾ºÅ
		//	t_Gamble_Getted_Item	itemID;				//	Ã»ÓÐ³ö´íµÄÇé¿öÏÂ£¬¹ºÂòµ½µÄÎïÆ·
		//	UNSIGNED SHORT      usServerPoit;		//	ÊÇ·ñÊÇ·þÎñÆ÷½Úµã¹ºÂò
		//	UINT8				uCount;				//	µÚ¶þ´ÎÎïÆ·µÄ×ÜÊý
		//	t_Gamble_Getted_Item	itemID[];			//	µÚ¶þ´ÎµÄÎïÆ·	

		GAMBLE_SC_UN_BUY,				//	Íæ¼ÒÈ¡Ïû¹ºÂòÄ³¸ö°Ù±¦Ïä
		//	GLOBAL_STRING_ID	iErrorID;			//	errorID == 0 ±íÊ¾Ã»ÓÐ·¢Éú´íÎó£¬Ö»ÊÇÓÐÍæ¼ÒÈ¡ÁË¹ºÂò
		//	UINT64				gambleVersion;		//	°æ±¾ºÅ
		//	UINT				uIndex;				//	°Ù±¦ÏäµÄÄ³¸öÎ»ÖÃ

		GAMBLE_CS_ACCEPT_ITEM,
		//	UINT64  gambleVersion;		//	°æ±¾ºÅ
		//	UINT8	accept;				//	ÊÇ·ñ½ÓÊÜµÚÒ»´ÎµÄÉÌÆ·	0Îª½ÓÊÜ£¬1Îª²»½ÓÊÜ
		//	t_Gamble_Getted_Item  itemID;				//	µÚÒ»´ÎÉÌÆ·µÄID
		//	CONTAINER_SLOT ContainerSlot;	//	ÎïÆ·Î»ÖÃ,ContainerSlot.ContainerTypeÎªCONTAINER_TYPE_INVALID±íÊ¾ÓÃµã¾í¹ºÂò
		//	ITEMNO	ItemNO;				//	ÎïÆ·µÄ±àºÅ
		//	UNSIGNED SHORT      usServerPoit;		//	ÊÇ·ñÊÇ·þÎñÆ÷½Úµã¹ºÂò


		GAMBLE_SC_ACCEPT_ITEM,
		//	GLOBAL_STRING_ID	iErrorID;	//	´íÎóÐÅÏ¢
		//	UINT8				accept;		//	ÊÇ·ñ½ÓÊÜµÚÒ»´ÎµÄÉÌÆ·	1ÎªµÚÒ»´Î£¬2ÎªµÚ¶þ´ÎÎïÆ·
		//	t_Gamble_Getted_Item	itemID;		//	ÎïÆ·µÄÐÅÏ¢

		GAMBLE_CS_ACCEPT_SECOND_ITEM,		//	½ÓÊÜµÚ¶þ´ÎµÄÎïÆ·
		//	ITEMID  itemIDGetted;			//	µÚ¶þ´Î½ÓÊÜµÄÎïÆ·

		GAMBLE_SC_ACCEPT_SECOND_ITEM,
		//	GLOBAL_STRING_ID		iErrorID;	//	´íÎóÐÅÏ¢
		//	t_Gamble_Getted_Item	itemID;		//	ÎïÆ·µÄÐÅÏ¢

		GAMBLE_CS_ENTER_GAMBLE,				//	½øÈë°Ù±¦Ïä¹ºÂò×´Ì¬
		GAMBLE_SC_ENTER_GAMBLE,				//	·þÎñÆ÷·µ»Ø½øÈë¹ºÂò×´Ì¬ÊÇ·ñ³É¹¦
		//	GLOBAL_STRING_ID	iErrorID;
		GAMBLE_CS_LEAVER_GAMBLE,			//	¿Í»§¶ËÀë¿ª¹ºÂò×´Ì¬	
		GAMBLE_SC_LEAVER_GAMBLE,			//	Àë¿ª°Ù±¦Ïä
		//	GLOBAL_STRING_ID	iErrorID;
		GAMBLE_SC_BROADCAST_GETTED_ITEM,	//	¹ã²¥Ä³¸öÍæ¼Ò»ñµÃµÄÎïÆ·
		//	char   szName[LEN_NAME + 1];	//	»ñµÃÎïÆ·µÄÍæ¼ÒµÄÃû×Ö
		//	t_Gamble_Getted_Item itemID;					//	»ñµÃµÄÎïÆ·µÄID

		GAMBLE_CS_GET_NAME,				//	»ñÈ¡°Ù±¦ÏäÖÐ»ñµÃ½±Æ·µÄÍæ¼ÒµÄÃû×Ö
		//	UINT64  gambleVersion;		//	°æ±¾ºÅ
		//	UINT32	uIndex;				//	Î»ÖÃ
		GAMBLE_SC_GET_NAME,
		//	UINT32	uIndex;					//	Î»ÖÃ
		//	char    szName[LEN_NAME + 1];	//	Ãû×Ö


	//////////////////////////////////////////////////////////////////////////
	//
	//	NEW PET	
	//
		//	ÐÂ³èÎïÐ­Òé°ü  2008£­11£­5
		PI_PET_LOGIC	= PI_CS_LOGIC + 4000,

		PET_SC_GET_INFO = PI_PET_LOGIC,
		//	UINT8 slotIndex;			//	³èÎïÔÚ³èÎï×°±¸À¸ÖÐµÄÎ»ÖÃ
		//	ITEMNO itemNo;				//	³èÎïµÄÎ¨Ò»±àºÅ
		//	t_PetAttr_Info Info;		//	³èÎïµÄ»ù±¾ÊôÐÔ
		//	int	   nSkillCount;			//	³èÎï¼¼ÄÜ×ÜÊý
		//	t_Pet_Skill_To_Client petSkill[nSkillCount];	//	³èÎï¼¼ÄÜ
		
		PET_CS_OPEN_PET_SHOP,
		//	UINT8	slotIndex;				//	°ÚÌ¯³èÎïÔÚ×°±¸À¸µÄÎ»ÖÃ
		//	ITEMNO	itemNo;					//	°ÚÌ¯³èÎïµÄÎïÆ·±àºÅ
		//	char	szShopName[LEN_NAME];	//	ÉÌµêµÄÃû³Æ
		//	UINT8	uShopStyle;				//	ÉÌµêµÄ×°ÊÎ£¬´ýÈ·¶¨??????
		//	UINT8	uItemCount;				//	ÉÌÆ·µÄÊýÁ¿
		//	FM_SELLITEM items[];			//	ÎïÆ·µÄÎ»ÖÃ

		PET_SC_OPEN_PET_SHOP,
		//	GLOBAL_STRING_ID	iErrorID;		//	°ÚÌ¯ÊÇ·ñ³É¹¦

		PET_CS_GET_PET_SHOP_INFO,
		//	GU_ID shopGuid;					//	ÉÌµêµÄÎ¨Ò»±êÖ¾ºÅ
		PET_SC_GET_PET_SHOP_INFO,
		//	GLOBAL_STRING_ID	iErrorID;	
		//	char szShopName[LEN_NAME];		//	ÉÌµêÃû×Ö
		//	UINT32 itemCount;				//	ÉÌÆ·µÄÊýÁ¿
		//	FM_VIEWITEM items[];			//	ÉÌÆ·µÄÐÅÏ¢

		PET_CS_ENTER_PET_SHOP,		//	Íæ¼Ò½øÈë³èÎïÉÌµê  £¬ÎªÁË¹ã²¥¹ºÂòÏûÏ¢¸øÔÚÕâ¸öÉÌµêµÄÍæ¼Ò
		//	GU_ID shopguid;
		PET_SC_ENTER_PET_SHOP,
		//	GLOBAL_STRING_ID	iErrorID;		
		PET_CS_LEAVE_PET_SHOP,
		//	GU_ID shopguid;
		PET_SC_LEAVE_PET_SHOP,
		//	GLOBAL_STRING_ID	iErrorID;		
		PET_SC_UPDATE_PET_SHOP_ITEM,
		//	GU_ID shopguid;					// ÉÌ³ÇµÄID
		//	UINT8 uSlotIndex;				// ¸ÃÎ»ÖÃµÄÎïÆ·ÒÑÂô³ö	

		PET_CS_BUY_ITEM,					//	ÂòÎïÆ·
		//	GU_ID	shopID;					//	ÉÌµê±àºÅ
		//	UINT8	shopSlotIndex;			//	¹ºÂòµÄÎ»ÖÃ
		//	FM_VIEWITEM viewItem;			//	¹ºÂòÉÌÆ·µÄÐÅÏ¢£¬ÓÃÀ´¼ìÑéÓÃ

		PET_SC_BUY_ITEM,					
		//	GLOBAL_STRING_ID	iErrorID;	//	ÊÇ·ñ¹ºÂò³É¹¦	

		PET_CS_CLOSE_PET_SHOP,
		//	USERID  uID;		
		//	CHARID  charID;		
		//	ITEMNO  itemNo;			//	³èÎïµÄÎïÆ·±àºÅ

		PET_SC_CLOSE_PET_SHOP,
		//	GLOBAL_STRING_ID	iErrorID;
		//	ITEMNO  itemNo;			//	³èÎïµÄÎïÆ·±àºÅ

		PET_CS_LEARN_SKILL,		//	Ñ§Ï°¼¼ÄÜ
		//	CONTAINER_SLOT itemSlot;	//	³èÎï¼¼ÄÜÊéÔÚ²Ö¿âÖÐµÄÎ»ÖÃ
		//	ITEMNO itemNo;
		//	int nSkillID;
		//	int nBaseLevel;				//	¼¼ÄÜµÈ¼¶

		PET_SC_LEARN_SKILL,
		//	GLOBAL_STRING_ID			//	iErrorID;		
		//	ITEMNO	itemNo;				//	ÄÄ¸ö³èÎïÑ§µÄ
		//	int		nSkillID;			//	Ñ§Ï°µÄ¼¼ÄÜ
		//	int		nBaseLevel;			//	Ñ§Ï°¼¼ÄÜµÄµÈ¼¶
		
		PET_CS_DISCARD_SKILL,
		//	ITEMNO	itemNo;
		//	int		nSkillID;			//	È¥µô¼¼ÄÜµÄID
		//	int		nBaseLevel;			//	È¥µô¼¼ÄÜµÄµÈ¼¶

		PET_SC_DISCARD_SKILL,
		//	GLOBAL_STRING_ID	iErrorID;	//	ÊÇ·ñ³É¹¦
		//	ITEMNO itemNo;		//	³èÎïµÄ±àºÅ
		//	int nSkillID;		//	³èÎïÒÅÍüµÄ¼¼ÄÜµÄID
		//	int		nBaseLevel;			//	È¥µô¼¼ÄÜµÄµÈ¼¶

		PET_SC_PET_ATTR_CHANGE,			//	³èÎïµÄÊôÐÔ¸Ä±ä£¬ÐèÒª¹ã²¥¸ø¸½½üµÄÍæ¼Ò
		//	CHARID  charID;				//	Íæ¼ÒµÄID
		//	UINT8 slotIndex;			//	³èÎïÔÚ³èÎï×°±¸À¸ÖÐµÄÎ»ÖÃ
		//	ITEMNO itemNo;				//	³èÎïµÄÎ¨Ò»±àºÅ
		//	t_PetAttr_Info Info;		//	³èÎïµÄ»ù±¾ÊôÐÔ

		PET_SC_PET_TAKE_OFF,			//	Ð¶ÔØ³èÎïÊ±¹ã²¥
		//	CHARID	charID;
		//	UINT8	slotIndex;
		//	ITEMNO	itemNo;

		PET_CS_ADD_SSP,				//	¸ø³èÎïÌí¼ÓÏÖ½ðÏûºÄµÀ¾ß
		//	CONTAINER_SLOT slot;	//	ÏÖ½ðµÀ¾ßµÄÎ»ÖÃ
		//	ITEMNO	itemNo;			//	³èÎïµÄ±àºÅ
		PET_SC_ADD_SSP,				//	
		//	GLOBAL_STRING_ID	iErrorID;	//	ÊÇ·ñ³É¹¦

		PET_CS_CHANGE_NAME,			//	¸ø³èÎïÐÞ¸ÄÃû×Ö
		//	CONTAINER_SLOT slot;	//	ÏÖ½ðµÀ¾ßµÄÎ»ÖÃ
		//	ITEMNO itemNo;			//	³èÎïµÄ±àºÅ
		//	char   szPetName[LEN_NAME];	//	³èÎïµÄÐÂÃû³Æ
		PET_SC_CHANGE_NAME,
		//	GLOBAL_STRING_ID	iErrorID;

        PET_CS_SHOUT,
		//	ITEMNO itemNo;		//	¶ÔÓ¦µÄ³èÎï
		//	char   szContent[50];	//	º°»°ÄÚÈÝ

		PET_SC_SHOUT,
		//	GLOBAL_STRING_ID	iErrorID;
		//	GU_ID  guid;			//	Èç¹û³èÎïÔÚ°ÚÌ¯£¬ÄÇÃ´·¢ÉÌ³ÇµÄGUID£¬²»ÔÚ°ÚÌ¯£¬ÄÇÃ´·¢ËÍÍæ¼ÒµÄGUID
		//	char   szContent[50];	//	º°»°ÄÚÈÝ

		PET_SC_PET_SHOP_CLOSED,		//	³èÎïÉÌµê±»¹Ø±ÕµÄÍ¨Öª
		//	ITEMNO itemNo;


	//////////////////////////////////////////////////////////////////////////
	//
	//	WEDDING
	//

		PI_CS_WEDDING = PI_CS_LOGIC + 4200,

		WEDDING_SC_INFO = PI_CS_WEDDING,
		// GU_ID				ObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// WEDDING_INFO			WeddingInfo;					// ¿þµùÁ¤º¸
		
		// ½ÅÃ»
		WEDDING_CS_REQUEST,
		// UINT8				iWeddingType;					// 1:±³Á¦, 2:ÇÁ·ÎÆ÷Áî, 3:°áÈ¥
		// char					szRcvCharName[LEN_NAME+1];		// ½ÅÃ»´ë»óÀÚÀÌ¸§
		// int					iEly;							// ¼ÒºñµÇ¾ßÇÏ´Â ¿¤¸®
		// CONTAINER_SLOT		tItemSlot;					// ¼ÒºñµÇ¾ßÇÏ´Â ¾ÆÀÌÅÛ

		// ½ÅÃ»
		WEDDING_SC_REQUEST,
		// GU_ID				ObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT8				iWeddingType;					// 1:±³Á¦, 2:ÇÁ·ÎÆ÷Áî, 3:°áÈ¥
		// GLOBAL_STRING_ID		ErrorID;						// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
		// char					szRcvCharName[LEN_NAME+1];		// ½ÅÃ»´ë»óÀÚÀÌ¸§


		// ¼ö½ÅÀÚ¿¡°Ô ½ÅÃ» ¿äÃ»ÀÌ µé¾î¿ÔÀ»¶§
		WEDDING_SC_REQUEST_RECV,
		// GU_ID				ObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT8				iWeddingType;					// 1:±³Á¦, 2:ÇÁ·ÎÆ÷Áî, 3:°áÈ¥
		// char					szCharName[LEN_NAME+1];			// ÀÌ¸§ 

		// ½ÅÃ» ÀÀ´ä
		WEDDING_CS_RESPONSE,
		// UINT8				iWeddingType;					// 1:±³Á¦, 2:ÇÁ·ÎÆ÷Áî, 3:°áÈ¥
		// char					szActCharName[LEN_NAME+1];		// ¿äÃ»ÀÚÀÌ¸§
		// UINT8				byAccept;						// 0:°ÅÀý, 1:¼ö¶ô


		// ½ÅÃ» ÀÀ´ä
		WEDDING_SC_RESPONSE,
		// GU_ID				ObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// UINT8				iWeddingType;					// 1:±³Á¦, 2:ÇÁ·ÎÆ÷Áî, 3:°áÈ¥
		// char					szCharName[LEN_NAME+1];
		// GLOBAL_STRING_ID		ErrorID;						// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// errorID°¡ ¼º°øÀÏ¶§¸¸ accept,weddingInfo¸¦ º¸³½´Ù
		// UINT8				byAccept;						// 0:°ÅÀý, 1:¼ö¶ô
		// weddingInfo´Â accept°¡ ¼ö¶ôÀÏ°æ¿ì¸¸ º¸³½´Ù
		// WEDDING_INFO			WeddingInfo;					// ¿þµùÁ¤º¸
		// TRASH_ITEM			TrashItem;						// »ç¿ëÇÒ ¾ÆÀÌÅÛ


		// °áÈ¥ ¿ÜÄ¡±â
		WEDDING_CS_CHAT,
		//UINT8					iMsgLen;							// ¸Þ¼¼Áö ±æÀÌ
		//TCHAR					szMsg[LEN_CASHITEM_NORMAL_MSG+1] = {0,};	

		//±âÁ¸¿ÜÄ¡±â¿¡ Å¸ÀÔÃß°¡·Î °áÁ¤
		//CHAT_SC_MSG ,
		// GLOBAL_STRING_ID	itemID;				// GLOBAL_STRING.LDT »óÀÇ ¸Þ¼¼Áö¸¦ itemID °ª
		// BROADCAST_SCOPE	scope;				// ºê·ÎµåÄ³½ºÆÃ ¹üÀ§
		// UINT8			uiFontColor;		// ÆùÆ®»ö	: 0(default),1(Red),2(Orange),3(Yellow),4(white) : CHAT_COLOR.LDT::ItemID
		// UINT8			uiOptions;			// reserved for FontSize,FontType
		// UINT8			len;
		// TCHAR			szMsg[];		
		// GU_ID			guidSender;			// ¼Û½ÅÀÚ Á¤º¸
		// UINT8			nSenderName;
		// TCHAR			szSender[];
		// UINT16			ChannelID;			// Ã¤³Î ID	


		// °áÈ¥½Ä Ãë¼Ò
		WEDDING_SC_WEDDING_CANCEL,
		// GU_ID				ObjectGUID;						// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð


		// °áÈ¥½Ä ½ÃÀÛ/³¡ ¾Ë¸²
		WEDDING_SC_MOTION_STARTEND,
		// GU_ID				globalID; 						// »ç¿ëÀÚ ¾ÆÀÌµð
		// UINT8				iStartEnd; 						// 0:Start(¿ÜÄ¡±âÃ¢ºÎÅÍ½ÃÀÛ) 1:Start(¿ÜÄ¡±âÃ¢¾øÀÌ½ÃÀÛ) 2:End


		//¹Ì´Ï¸Ê, ¿ùµå¸Ê¿¡¼­ ¹è¿ìÀÚ À§Ä¡ º¸ÀÌ±â
		WEDDING_CS_UPDATEMEMBER,


		//·Î±×¿ÀÇÁ, Ã¤³ÎÀÌ ´Ù¸£¸é ¾Èº¸³½´Ù
		WEDDING_SC_UPDATEMEMBER,
		// GU_ID			globalID;							// »ç¿ëÀÚ ¾ÆÀÌµð
		// GU_ID			PartnerID;							// ¹è¿ìÀÚ GUID, ¿ÀÇÁ¶óÀÎÀÏ°æ¿ì 0À¸·Î
		// UINT8			iServerID;							// ¼­¹ö¾ÆÀÌµð, ¿ÀÇÁ¶óÀÎÀÏ°æ¿ì 0À¸·Î 
		// int				usStageID;							// stageID, ¿ÀÇÁ¶óÀÎÀÏ°æ¿ì³ª ´Ù¸¥Ã¤³ÎÀÏ¶§ -1À¸·Î
		// int				usMapGroupID;						// MapGroupID, ¿ÀÇÁ¶óÀÎÀÏ°æ¿ì³ª ´Ù¸¥Ã¤³ÎÀÏ¶§ -1À¸·Î


		//ÀÌº°
		WEDDING_CS_PARTING,
		// int			iEly;									// ¼Ò¸ðµÇ´Â¿¤¸®


		WEDDING_SC_PARTING,
		// GU_ID				globalID;						// »ç¿ëÀÚ ¾ÆÀÌµð
		// GLOBAL_STRING_ID		iError;							// 0 : ¼º°ø

		//WEDDING_CS_CHAR_INFO,
		//// char					szCharName[LEN_NAME+1];		// Á¤º¸º¸±âÇÏ±âÀ§ÇÑ Ä³¸¯ÅÍÀÌ¸§


		//WEDDING_SC_CHAR_INFO,
		//// GU_ID				globalID;					// »ç¿ëÀÚ ¾ÆÀÌµð
		//// GLOBAL_STRING_ID		iError;						// °áÈ¥ Á¤º¸°¡ ¾ø´Â»ç¶÷Àº ¿¡·¯·Î Ç¥½Ã
		//// int					iLv;
		//// int					iDay;
		//// char					szCharName[LEN_NAME+1];		// ±³Á¦ÀÚ Ä³¸¯ÅÍÀÌ¸§

		// °áÈ¥ ¸ð¼Ç ½ºÅ³
		WEDDING_SC_SKILL_USING,
		// RESULTCODE		resultCode;
		// GU_ID			iInstanceID;
		// UINT32			iSkillID;
		// LT_POSITION		pos;

		WEDDING_SC_SUMMON_REQUEST,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð

		// ¼ÒÈ¯ ½ÃÀüÀÚ¿¡°Ô ¿¡·¯ÀÏ¶§¸¸ º¸³½´Ù
		WEDDING_SC_SUMMON_REQUEST_ERR,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)



		// ¿¬ÀÎ¼ÒÈ¯¿äÃ»ÀÀ´ä
		WEDDING_CS_SUMMON_RESPONSE,
		// UINT8			byAccept;					// 0:°ÅÀý, 1:¼ö¶ô

		// ¿¬ÀÎ¼ÒÈ¯¿äÃ»ÀÀ´ä °á°ú
		// ¿¡·¯ÀÏ¶§´Â ´ë»óÀÚ¿¡°Ô¸¸ º¸³½´Ù
		WEDDING_SC_SUMMON_RESPONSE,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)

		// ¿¬ÀÎ¼ÒÈ¯¿äÃ»ÀÀ´ä
		// ¿äÃ»ÀÚ¿¡°Ô ¿¡·¯°¡ ¾Æ´Ò¶§ º¸³½´Ù
		WEDDING_SC_SUMMON_RESPONSE_RECV,
		// GU_ID			ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
		// GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
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
	UINT8			uiHair;			// ¸Ó¸®
	UINT8			uiFashion;		// ÆÐ¼Ç
	UINT8			uiPersonality;	// ¼º°Ý
	UINT8			uiCondition;	// »óÅÂ
} PCSTATUS_STYLE, *PPCSTATUS_STYLE;

typedef struct _tagPCSTATUS_POS
{
	int				iRevivalMap;	// ºÎÈ°À§Ä¡
	int				iTownMap;	// ºÎÈ°À§Ä¡
	UINT16			uiStageID;		// StageID
	UINT16			uiMapGroupID;	// MapGroupID
	UINT16			uiLayer;		// ·¹ÀÌ¾î
	int				iPosX;			// xÁÂÇ¥
	int				iPosY;			// yÁÂÇ¥
	UINT8			uiDir;			// PC°¡ ¹Ù¶óº¸´Â ¹æ“‡

} PCSTATUS_POS, *PPCSTATUS_POS;

//  [11/18/2004] Àåºñ ¾ÆÀÌÅÛ by metalgeni
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
	UINT    uGambleIndex;	//	°Ù±¦ÏäµÄ±àºÅ
	int		nBuyStep;	//	¹ºÂòµÄ½×¶Î£¬0Î´±»¹ºÂò£¬1ÎªÒÑ¾­¹ºÂò£¬µ«ÊÇÃ»ÓÐ¸üÐÂÎïÆ·£¬2ÎªÒÑ¾­¹ºÂò²¢ÇÒÒÑ¾­¸üÐÂÎïÆ·ÐÅÏ¢
	ITEMID	itemID;		//	ÒÑ¾­¹ºÂòÖ®ºóµÄitemID
	UINT32	uSetID;		//	½±Æ·µÄÀà±ðID

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
	ITEMID	itemID;		//	»ñµÃµÄ½±Æ·
	UINT8   usCount;	//	½±Æ·µÄÊýÁ¿
	UINT32	uSetID;		//	½±Æ·µÄÀà±ðID

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
	EFT_ADDPARCEL = 1,			//¼Ó´ó°ü¹üÀ¸
	EFT_ADDCONSUME = 2,			//¼Ó´óÏûºÄÀ¸
	EFT_ADDETC = 3,				//¼Ó´óÆäËüÀ¸
	EFT_ADDEVENT = 4,			//¼Ó´óÈÎÎñÀ¸
	EFT_ADDPET = 5,				//¼Ó´ó³èÎïÀ¸
	EFT_ADDWH = 6,				//¼Ó´ó²Ö¿âÀ¸
	
};


enum VALUE_TYPE
{
	VALUE_BASE, // °íÀ¯°ª
	VALUE_CURR, // ÃÖÁ¾°ª
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

// Å©¸®ÃÄ ±âº» Á¤º¸
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

// ½ºÅÈ Á¤º¸(LUK,ATK,DEF,ELE,WATER,EARTH,FIRE,WIND)
typedef struct _CHAR_STS
{
	UINT16		Sts[STAT_TOTAL][VALUE_MAX]; 
	UINT16		Ele[ELEMENT_TOTAL];				// ÃÖÁ¾°ª¸¸ °¡Áü.
	UINT16		Atk[ATK_MAX];					// ÃÖÁ¾°ª¸¸ °¡Áü.

} CHAR_STS, *PCHAR_STS;

// °æÇèÄ¡ Á¤º¸
typedef struct _CHAR_EXP
{
	UINT64  uiCurExp;
	UINT64  uiMinExp;
	UINT64  uiMaxExp;

} CHAR_EXP, *PCHAR_EXP;

// ¸ó½ºÅÍ Á¤º¸
typedef struct _MONSTER_INFO
{
	CREATURE_INFO	BaseInfo;
	MOVING_STATUS   MovingStatus;
	int iType;


} MONSTER_INFO,*PMONSTER_INFO;

// NPC Á¤º¸
typedef struct _NPC_INFO
{
	CREATURE_INFO	BaseInfo;

} NPC_INFO,*PNPC_INFO;

// Ä³¸¯ÅÍ ±âº» Á¤º¸
typedef struct _CHARACTER_INFO2
{
	CREATURE_INFO	BaseInfo;
	USERID			uiUserID;					// °èÁ¤¿¡ ´ëÇÑ ID
	UINT32			uiTitleID;					// Å¸ÀÌÆ²
	UINT64			uiMoney;					// °ÔÀÓ¸Ó´Ï
	UINT8			uiClassType;				// Class Type
	UINT8			uiSex;						// 1: ³², 2: ¿©
	UINT8			uiWeaponSet;				// 0: 13,14  1:15,16 
	UINT64			uiStorageMoney;				// Ã¢°í¸Ó´Ï
	MOVING_STATUS	MovingStatus;
	UINT32			Fame[VALUE_MAX];			// ¸í¼ºÄ¡
	INT32			iGameOptions;				// 2007.02.21 dhpark ¼­¹ö¿¡¼­ ÀúÀåÇØ¾ßÇÒ °ÔÀÓ¿É¼Çµé

	// ÀÌµ¿
} CHARACTER_INFO2, *PCHARACTER_INFO2;


// Ä³¸¯ÅÍ Ä³½¬Á¤º¸ 20080306 mijin
typedef struct _CHARACTER_CASH_INFO
{
	int			iCash;						// 20080306 mijin Ä³½Ã
	int			iMile;						// 20080306 mijin ¸¶ÀÏ¸®Áö
	UINT8		uiHotCashShopMode;			// 20080415 mijin ÃßÃµ»óÇ° ºí·¯Å· ¿©ºÎ (0: close, 1:open)
} CHARACTER_CASH_INFO, *PCHARACTER_CASH_INFO;



// Ä£±¸ ¸ñ·Ï»óÀÇ Ä³¸¯ÅÍ Á¤º¸
typedef struct _FRIEND_INFO
{
	TCHAR			szCharName[LEN_NAME+1];
	UINT8			uiClassType;				// °ø°ÝÇü,...
	UINT8			uiLevel;					// ÇØ´ç Ä³¸¯ÅÍÀÇ ÇöÀç ·¹º§
	UINT16			uiServerID;					// 0(off-line), >1(on-line)
	CHARID			friendCharID;

}FRIEND_INFO,*PFRIEND_INFO;

// ÆÀ¿ø Á¤º¸ Àü¼Û
typedef struct _TEAMMEMBER_INFO : public CREATURE_INFO
{

}TEAMMEMBER_INFO,*PTEAMMEMBER_INFO;


// ½ºÅÈ Á¤º¸
typedef struct _STATUS_INFO
{
	CHAR_EXP		ExpInfo;
	CHAR_STS		StsInfo;
	short			iSkillFreePoint;
	short iAmor;					// ¹æ¾î·Â
	short iResEarth;				// ÀúÇ×·Â
	short iResWind;				// ÀúÇ×·Â
	short iResWater;				// ÀúÇ×·Â
	short iResFire;				// ÀúÇ×·Â
	short iReqLv;					// Á¦ÇÑ·¹º§
	short iProbEnchat;			// ÀÎÃ¾Æ® ¼º°ø·ü.
} STATUS_INFO,*PSTATUS_INFO;

// ÀÌÆÑÆ®¿¡ ÀÇÇÏ¿© º¯°æµÉ¼ö ÀÖ´Â ½ºÅÈ°ªµé
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

// À§Ä¡ Á¤º¸
typedef struct _LT_POSITION
{
	float	fPosX;								// ÇöÀç À§Ä¡
	float	fPosY;			
	float	fVelocityX;							// °È±â ¼Óµµ		ACCUMULATE_PROCESS_TICK±âÁØ
	float	fVelocityY;							// ¶³¾îÁö´Â ¼Óµµ	ACCUMULATE_PROCESS_TICK±âÁØ
	float	fAccelX;							// °¡¼Óµµ			ACCUMULATE_PROCESS_TICK±âÁØ
	float	fAccelY;			
	int		eFacing;							// ¹æÇâ (FACING)
	int		bgStandLayer;						// ÇöÀç ¼­ÀÖ´Â ÅÍ·¹ÀÎ ·¹ÀÌ¾î (BG_ID)

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
	 UINT16	uiStageID;			// ½ºÅ×ÀÌÁö ID
	 UINT16	uiMapGroupID;		// ¸Ê±×·ì ID
	 UINT16	uiMapID;			// ¸Ê ID
}MAP_INFO,*PMAP_INFO;

enum EVENT_TYPE{EVENT_NULL,EVENT_NPC,EVENT_SAVEPOINT,EVENT_PORTAL,EVENT_QUEST,EVENT_MAILBOX,EVENT_BEAUTY,EVENT_CROPS,EVENT_CORE,EVENT_CHATROOMOBJECT,MAX_EVENT};

/**
* ÇöÀç NPC ÀÇ EVENT ¼¼ºÎ »çÇ× <br> 
*/
enum NPC_EVENT_TYPE {
	NPC_EVENT_TYPE_NULL				= 0,	/**< ¾Æ¹«°Íµµ ¾Æ´Ô <br> */
	NPC_EVENT_TYPE_TALK				= 1,	/**< ´ëÈ­ <br> */
	NPC_EVENT_TYPE_TRADE			= 2,	/**< »óÁ¡ <br> */
	NPC_EVENT_TYPE_STORAGE			= 3,	/**< Ã¢°í <br> */
	NPC_EVENT_TYPE_QUEST			= 4,	/**< Quest <br> */
	NPC_EVENT_TYPE_GIFT				= 5,	/**< Gift <br> */
	NPC_EVENT_TYPE_GUILD_CREATE		= 6,	/**< ±æµå »ý¼º<br> */
	NPC_EVENT_TYPE_GUILD_DISSOLVE	= 7,	/**< ±æµå ÇØÃ¼ <br> */
	NPC_EVENT_TYPE_GUILD_ENTER		= 8,	/**< ±æµå ÀÔÀå <br> */
	NPC_EVENT_TYPE_PORTAL			= 9, 	/**< NPC¸¦ ÅëÇÑ Æ÷Å» <br> */
	NPC_EVENT_TYPE_CHANGE			= 10,	/**< NPC¸¦ ÅëÇÑ ÀÌº¥Æ® ¾ÆÀÌÅÛ ±³È¯ <br> */
	NPC_EVENT_TYPE_GUILD_LEVELUP	= 11,	/**< ±æµå ·¹º§ ¾÷ <br> */
	NPC_EVENT_TYPE_GUILD_EMBLEM		= 12,	/**< ±æµå ¿¥ºí·½ º¯°æ <br> */	
	NPC_EVENT_TYPE_PVP_RESERVE		= 13,	/**< PVP ¿¹¾à <br> */
	NPC_EVENT_TYPE_AUCTION			= 14,	/**< °æ¸Å <br> */
	NPC_EVENT_TYPE_REOPTION				= 15,	/**< ¿É¼Ç ÀçºÐ¹è <br> */ 
	NPC_EVENT_TYPE_PARTY_PVP_RESERVE	= 16,	/**< ÆÄÆ¼ PVP ¿¹¾à <br> */  
	NPC_EVENT_TYPE_RANKING				= 17,
	NPC_EVENT_TYPE_WEDDING_RESERVE		= 18,	/**< °áÈ¥ NPC¸¦ ÅëÇÑ ½Ä ¿¹¾à <br> */  
	NPC_EVENT_TYPE_WEDDING_DIVORCE		= 19,	/**< °áÈ¥ NPC¸¦ ÅëÇÑ ÀÌÈ¥ <br> */  
};


// Àç±¸¼ºÇÒ MapEventInfo¿¡ µé¾î°¥ ¿ä±¸ Á¶°Ç
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


// MapEvent InfoÀÇ Àç±¸¼º
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

	// ¾ÆÀÌÅÛ ¼Ò¸ð °ü·Ã( Æ÷Å»ÀÌº¥Æ®½Ã¸¸ ÀÌ¿ë:±âÈ¹ÆÀÀÇµµ ) 2007/01/19 
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

// 4byte ½ºÅÈ
enum STATUS_TYPE 
{
	STATUS_NONE=0,
	STATUS_LV,
	STATUS_LUK_BASE,					// ·¹º§¾÷½Ã Ã¤ÆÃÃ¢¿¡ Ç¥½ÃµÇ´Â¹®Á¦·Î ¼ø¼­ °íÁ¤!!!! [2/6/2006 AJJIYA]
	STATUS_ATK_BASE,					// ·¹º§¾÷½Ã Ã¤ÆÃÃ¢¿¡ Ç¥½ÃµÇ´Â¹®Á¦·Î ¼ø¼­ °íÁ¤!!!! [2/6/2006 AJJIYA]
	STATUS_DEF_BASE,					// ·¹º§¾÷½Ã Ã¤ÆÃÃ¢¿¡ Ç¥½ÃµÇ´Â¹®Á¦·Î ¼ø¼­ °íÁ¤!!!! [2/6/2006 AJJIYA]
	STATUS_ELE_BASE,					// ·¹º§¾÷½Ã Ã¤ÆÃÃ¢¿¡ Ç¥½ÃµÇ´Â¹®Á¦·Î ¼ø¼­ °íÁ¤!!!! [2/6/2006 AJJIYA]
	STATUS_MHP_BASE,
	STATUS_MSP_BASE,
	STATUS_FAME_BASE,
	STATUS_LUK, 						// ÃÖÁ¾°ª
	STATUS_ATK,  						// ÃÖÁ¾°ª
	STATUS_DEF,	 						// ÃÖÁ¾°ª
	STATUS_ELE,  						// ÃÖÁ¾°ª
	STATUS_MHP,  						// ÇÇÅë
	STATUS_MSP,							// ¸¶³ªÅë
	STATUS_HP,  						// ÇöÀçÇÇ
	STATUS_SP,							// ÇöÀç¸¶³ª
	STATUS_FAME,
	STATUS_CLASS_TYPE,
	STATUS_PVP_LV,						// PVP Rank Level
	STATUS_PVP_POINT,					// PVP °Å·¡ Æ÷ÀÎÆ®
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
	STATUS_AMOR,						// ¹æ¾î·Â
	STATUS_RES_EARTH,					// ÀúÇ×·Â
	STATUS_RES_WIND,					// ÀúÇ×·Â
	STATUS_RES_WATER,					// ÀúÇ×·Â
	STATUS_RES_FIRE,					// ÀúÇ×·Â
	STATUS_REQ_LV,						// Á¦ÇÑ·¹º§½ºÅÝ
	STATUS_PROB_ENCHANT,				// ÀÎÃ¾Æ® ¼º°øÈ®·üº¸Á¤.		
	STATUS_CASH,						//20080310 mijin Ä³½¬(¿ÀÁî)
	STATUS_MILE,						//20080310 mijin ¸¶ÀÏ¸®Áö
	STATUS_PHYCRI,						// ¹°¸®Å©¸®
	STATUS_ELECRI,						// ¸¶¹ýÅ©¸®
	STATUS_PHYDODGE,					// ¹°¸®È¸ÇÇ
	STATUS_ELEDODGE,					// ¸¶¹ýÈ¸ÇÇ
	STATUS_PHYHITRATE,					// ¹°¸®¸íÁß
	STATUS_ELEHITRATE,
	STATUS_LOVE_LV,						// ¿¬¾Ö Level
	STATUS_LOVE_POINT,					// ¿¬¾Ö Æ÷ÀÎÆ® ÇöÀç°ª [10/2/2008 AJJIYA]
	STATUS_LOVE_POINT_MAX,				// ¿¬¾Ö Æ÷ÀÎÆ® ÃÖ´ë°ª [10/2/2008 AJJIYA]
	STATUS_LOVE_DAY,					// ¿¬¾Ö ±³Á¦ ÀÏ ¼ö [10/2/2008 AJJIYA]
	STATUS_LOVE_STATE,					// ¿¬¾Ö »óÅÂ ( 0 : None , 1 : ±³Á¦»óÅÂ , 2 : ÇÁ·ÎÆ÷Áî ¿Ï·á »óÅÂ , 4 : °áÈ¥»óÅÂ ) [10/2/2008 AJJIYA]
	STATUS_MOB_COUNT,					// ¼ÒÈ¯ÇÑ ¸÷°¹¼ö
	STATUS_EXTENDED_CHARSLOT,			// È®ÀåµÈ Ä³¸¯ÅÍ ½½·Ô¼ö
	STATUS_RANKING_LEVEL,				// ·©Å·½Ã½ºÅÛ : ·¹º§·©Å·
	STATUS_RANKING_PVP,					// ·©Å·½Ã½ºÅÛ : PVP
	STATUS_RANKING_LOVE,				// ·©Å·½Ã½ºÅÛ : ¾ÖÁ¤µµ
	STATUS_RANKING_GUILD,				// ·©Å·½Ã½ºÅÛ : ±æµå
	MAX_STATUS
};

// ±âº» 8BYTE ½ºÅÈ.
enum STATUS_EX_TYPE		
{
	STATUS_EX_NONE = 0,
	STATUS_EX_EXP,
	STATUS_EX_EXP_MAX,
	STATUS_EX_EXP_MIN,
	STATUS_EX_ELY,
	STATUS_EX_STORAGE_ELY,				//[2005/7/20]
	STATUS_EX_PVP_GLORY,				//[2007/11/27]	PVP ¸í¿¹ Æ÷ÀÎÆ®
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
		PM_INITIALIZING,				// ÃÊ±âÈ­Áß.
		PM_CHAR_MANAGING,				// Ä³¸¯ÅÍ »ý¼º/»èÁ¦/¼±ÅÃ Áß
		PM_PALYING,						// °ÔÀÓÁß
		PM_EXITING,						// °ÔÀÓ Á¾·áÁß
		MAX_PM};

// ¿ùµåº° Ä³¸¯ÅÍ¼ö Á¤º¸
//typedef struct _WORLD_INFO
//{
//	WORLD_ID	wid;
//	TCHAR		name[LEN_NAME];
//	TCHAR		ip[LEN_IP];
//	SERVER_PORT	port;
//	int			nCharCount;
//	float		fStatusValue;			// ¿ùµå³» ÃÑµ¿Á¢¼ö/ÃÖ´ëÁ¢¼Ó°¡´É¼ö
//}WORLD_INFO;
//
//typedef struct _CHANNEL_INFO
//{
//	SERVER_ID	uiChannelID;
//	UINT16		uiMaxUsers;
//	UINT16		uiCurUsers;
//	//float		fStatusValue;			// Ã¤³Î³» À¯Àú¼ö/ÃÖ´ë°¡´É¼ö
//}CHANNEL_INFO,*PCHANNEL_INFO;

// ¿ùµåº° Ä³¸¯ÅÍ¼ö Á¤º¸
typedef struct _WORLD_INFO
{
	UINT16		wid;
	TCHAR		name[LEN_NAME];
	SERVER_ID	usLobbySID;
	TCHAR		ip[LEN_IP];
	UINT16		port;
	UINT8		nCharCount;
	UINT8		status;					// ÄèÀû(0),º¸Åë(1),È¥Àâ(2)
}WORLD_INFO;

typedef struct _CHANNEL_INFO
{ 
	UINT16		channelID;
	TCHAR		name[LEN_NAME];
	UINT8		status;					// ÄèÀû(0),º¸Åë(1),È¥Àâ(2),Á¢¼ÓºÒ°¡(3)
}CHANNEL_INFO;


// Ã¤ÆÃ ¸Þ¼¼Áö¸¦ Àü¼ÛÇÒ ¹üÀ§¸¦ ÁöÁ¤
// Ã¤ÆÃÂÊ Ãß°¡½Ã LDT¿¡¼­ ÇØ´çºÎºÐ ÁöÁ¤µÇ¾î ÀÖÀ¸¹Ç·Î ÇÊÈ÷ »çÀü »óÀÇ ºÎÅ¹µå¸³´Ï´Ù. [4/23/2007 AJJIYA]
enum BROADCAST_SCOPE 
{
	SCOPE_BROADCAST			=	0		,
	SCOPE_AROUNDREGION					,			// ±ÙÃ³(ºê·ÎµåÄ³½ºÆÃ ¿µ¿ª³») PC			: Ã¤³Î³»
	SCOPE_FRIENDLIST					,			// Ä£±¸ ¸ñ·Ï»óÀÇ ¸ðµç À¯Àú				: µ¿ÀÏ Ã¤³Î³»·Î ÇÑÁ¤
	SCOPE_GUILD							,			// ±æµåÃ¤ÆÃ								: ¿ùµå³» ±æµå¿øµé
	SCOPE_TEAM							,			// ÆÀ									: µ¿ÀÏ Ã¤³Î³»·Î ÇÑÁ¤
	SCOPE_WHISPER						,			// Æ¯Á¤ À¯Àú(±Ó¼Ó¸»)					: ¿ùµå³»·Î ÇÑÁ¤
	SCOPE_BUGLE							,
	
	SCOPE_CHANNEL			=	1001	,			// Æ¯Á¤ Ã¤³Î³»ÀÇ ¸ðµç À¯Àú				: Ã¤³Î³»
	SCOPE_WORLD				=	1002	,			// Æ¯Á¤ ¿ùµå³»ÀÇ ¸ðµç À¯Àú				: ¿ùµå³»
	SCOPE_CHATROOM_ADVERT	=	1003	,			// ´ëÈ­¹æ »ý¼º½Ã ¿ùµå³»ÀÇ ¸ðµç À¯Àú¸¦ ´ë»óÀ¸·Î ÇÏ´Â ±¤°í ¸Þ¼¼Áö
	SCOPE_WEDDING			=	1004	,			// °áÈ¥½Ã ¿ùµå³»ÀÇ ¸ðµçÀ¯Àú¿¡°Ô ±¤°í

	SCOPE_NOTICE			=	2001	,			// °øÁö»çÇ×. Æ¯Á¤ ¿ùµå³»ÀÇ ¸ðµç À¯Àú(GM Only)	: ¿ùµå³»
	SCOPE_SYSTEMMSG			=	2002	,			// ½Ã½ºÅÛ ¸Þ¼¼Áö Àü¼Û¿ë(cheat °á°ú,¿À·ù/°æ°í ¸Þ¼¼Áö,...)
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
	INVENTORY_WEAR,        //[xialin 2008/07/21] add ÊÔ´©¼ÇÂ¼
	FIGURE_EFFECT,
	CONTAINER_MAX,
	CONTAINER_AUCTION = 88,
	CONTAINER_MAILBOX = 99,
};

// Packet¿ë 

/*-------------------------------------------------*/

// Ä³¸¯ÅÍÀÇ ±âº» ¿ÜÇü Á¤º¸

/*-------------------------------------------------*/

enum FIGURE_BASIC_TYPE
{
	FIGURE_BASIC_NULL,
	FIGURE_BASIC_UNDER_SHIRT, // ¼Ó¿Ê »óÀÇ
	FIGURE_BASIC_UNDER_PANTS, // ¼Ó¿Ê ÇÏÀÇ
	FIGURE_BASIC_HAIR,
	FIGURE_BASIC_FACE,
	FIGURE_BASIC_SKIN,
	FIGURE_BASIC_MAX,
};

/*-------------------------------------------------*/

// ÆÐ¼ÇÃ¢ ½½·Ô¹øÈ£

/*-------------------------------------------------*/

enum EQUIP_SLOT_NUMBER
{
	EQUIP_SLOT_NULL,
	EQUIP_SLOT_HEAD,		// ¸Ó¸®
	EQUIP_SLOT_FACE1,		// ¾È°æ
	EQUIP_SLOT_EARRING,		// ±Í°ÉÀÌ
	EQUIP_SLOT_SHIRT,		// ¼ÅÃ÷
	EQUIP_SLOT_PANTS,		// ¹ÙÁö
	EQUIP_SLOT_MANTLE,		// ¸ÁÅä
	EQUIP_SLOT_GLOVE,		// Àå°©
	EQUIP_SLOT_SHOES,		// ½Å¹ß
	EQUIP_SLOT_LEG,			// ´Ù¸®
	EQUIP_SLOT_FACE2,		// ¾ó±¼¿¡ Â÷´Â¾Ç¼¼¼­¸®
	EQUIP_SLOT_STONE,		// Á¤·É¼®		
	EQUIP_SLOT_RING,		// ¹ÝÁö	
	EQUIP_SLOT_WEAPON1,
	EQUIP_SLOT_BUCKLER1,
	EQUIP_SLOT_WEAPON2,
	EQUIP_SLOT_BUCKLER2,
	EQUIP_SLOT_MAX,
};

/*-------------------------------------------------*/

// ÆÐ¼ÇÃ¢ ½½·Ô¹øÈ£

/*-------------------------------------------------*/
enum FASHION_SLOT_NUMBER
{
	FASHION_SLOT_NULL,
	FASHION_SLOT_HEAD,			// ¸Ó¸®
	FASHION_SLOT_FACE1,			// ¾ó±¼¿¡ ÇÏ´Â ¾ÆÀÌÅÛ
	FASHION_SLOT_EARRING,		// ±Í°ÉÀÌ
	FASHION_SLOT_SHIRT,			// ¼ÅÃ÷
	FASHION_SLOT_PANTS,			// ¹ÙÁö
	FASHION_SLOT_MANTLE,		// ¸ÁÅä
	FASHION_SLOT_GLOVE,			// Àå°©
	FASHION_SLOT_SHOES,			// ½Å¹ß
	FASHION_SLOT_LEG,			// ´Ù¸®
	FASHION_SLOT_FACE2,			// ¾ó±¼¿¡ ÇÏ´Â ¾ÆÀÌÅÛ
	FASHION_SLOT_STONE,			// Á¤·É¼®
	FASHION_SLOT_RING,			// ¹ÝÁö
	FASHION_SLOT_MAX
};

//--------------------------------------------------
// effect slot number
enum EFFECT_SLOT_NUMBER
{
	EFFECT_SLOT_NULL,

	EFFECT_SLOT_HEAD,		// ¸Ó¸®
	EFFECT_SLOT_FACE1,		// ¾È°æ
	EFFECT_SLOT_EARRING,	// ±Í°ÉÀÌ
	EFFECT_SLOT_SHIRT,		// ¼ÅÃ÷
	EFFECT_SLOT_PANTS,		// ¹ÙÁö
	EFFECT_SLOT_MANTLE,		// ¸ÁÅä
	EFFECT_SLOT_GLOVE,		// Àå°©
	EFFECT_SLOT_SHOES,		// ½Å¹ß
	EFFECT_SLOT_LEG,		// ´Ù¸®
	EFFECT_SLOT_FACE2,		// ¾ó±¼¿¡ Â÷´Â¾Ç¼¼¼­¸®
	EFFECT_SLOT_STONE,		// Á¤·É¼®		
	EFFECT_SLOT_RING,		// ¹ÝÁö	

	EFFECT_SLOT_FORCEENUMMAX
};
//--------------------------------------------------

/*-------------------------------------------------*/

// ÆêÃ¢ ½½·Ô¹øÈ£

/*-------------------------------------------------*/
enum PET_SLOT_NUMBER
{
	PET_SLOT_NULL,
	PET_SLOT_PRIMARY,				// ÆêÀåÂø
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

//typedef UINT32	DUABILITY;	// ³»±¸µµ
typedef UINT32	DURATION;	// Áö¼Ó½Ã°£ 
typedef UINT32	MOBID;

// ÄÁÅ×ÀÌ³Ê ½½·Ô Á¤º¸
typedef struct _CONTAINER_SLOT
{
	CONTAINER_TYPE	ContainerType;
	SLOT_INDEX		SlotIndex;

} CONTAINER_SLOT, *PCONTAINER_SLOT;

typedef struct _CHAR_FIGURE
{
	ITEMID FIGURE[FIGURE_BASIC_MAX];
}CHAR_FIGURE,*PCHAR_FIGURE;

// pc Container Á¤º¸
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
	ITEMID		iItemID;			// ¾ÆÀÌÅÛ ¾ÆÀÌµð
	UINT8		iStackCount;		// ¾ÆÀÌÅÛ °¹¼ö
	int			iLifeTime;			// ³²Àº Å¸ÀÓÀÌ¹Ç·Î À½¼ö °ªÀÌ ¿Ã¼ö ÀÖÀ½. DB ¿Í ¿¬°üÀÖÀ½.
	UINT8		iRare;				// ·¹¾îµµ
	PACK_OPT	Opt[MAX_ITEM_OPT];	// ¿É¼Ç
	ITEMNO		iItemNo;
	int			iBackPrice;		// 
	short		iOptPoint;
	INT64		iItemExp;			// ¾ÆÀÌÅÛ°æÇèÄ¡
	INT16		iItemSP;			// ¾ÆÀÌÅÛSP
} ITEM, *PITEM;

// ½½·ÔÀÇ ¾ÆÀÌÅÛ sizeof(CONTAINER_ITEM) => 56
typedef struct _CONTAINER_ITEM
{
	CONTAINER_SLOT	SlotInfo;
	ITEM			Item;		// Item Object
} CONTAINER_ITEM, *PCONTAINER_ITEM;

// ¿ÜÇüÀûÀ¸·Î »Ñ·ÁÁÖ´Â ¾ÆÀÌÅÛ (ÁÖ·Î ºê·ÎµåÄÉ½ºÆ®¿ë...)
typedef struct _FIGURE_ITEM
{
	CONTAINER_SLOT  SlotInfo;
	ITEMID			iItemID;	// Item ID --> °¡Á®¿Í¼­ ÇØ´ç ID¸¦ Ã£¾Æ¼­ »Ñ·ÁÁØ´Ù.

}FIGURE_ITEM, *PFIGURE_ITEM;


typedef struct _LOOTING_ITEM
{
	SLOT_INDEX iSlotIndex;
	ITEMID	   iItemID;    // °ËÁõ¿ë.
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

// Æ®·¹ÀÌµå ¾ÆÀÌÅÛÃ¢¿¡ ¾ÆÀÌÅÛÀ» ¿Ã¸±¶§ »ç¿ëÇÏ´Â ±¸Á¶Ã¼
typedef struct _TRADE_ITEM
{
	CONTAINER_SLOT	stActSlotInfo;
	SLOT_INDEX		TradeSlotIndex;

} TRADE_ITEM, *PTRADE_ITEM;

// ¾ÆÀÌÅÛ ¶óÀÌÇÁÅ¸ÀÓ ±¸Á¶Ã¼
typedef struct _ITEM_LIFETIME
{
	CONTAINER_SLOT	stSlotInfo;
	int				iLifeTime;

} ITEM_LIFETIME, *PITEM_LIFETIME;

// ¾ÆÀÌÅÛ °æÇèÄ¡ ±¸Á¶Ã¼
typedef struct _ITEM_EXP
{
	CONTAINER_SLOT	stSlotInfo;
	INT64			iItemExp;

} ITEM_EXP, *PITEM_EXP;

// ¾ÆÀÌÅÛ SP ±¸Á¶Ã¼
typedef struct _ITEM_SP
{
	CONTAINER_SLOT	stSlotInfo;
	INT16			sItemSP;

} ITEM_SP, *PITEM_SP;

// Upgrade¿¡¼­ ÀÌ¿ëµÇ´Â

typedef struct _MATERIAL_DATA
{
	CONTAINER_TYPE	iContainerType;		// Àç·á ¾ÆÀÌÅÛ À§Ä¡  
	UINT8			iContainerSlot;		// Àç·á ¾ÆÀÌÅÛ À§Ä¡
	ITEMNO			iItemNo;			// Àç·á ¾ÆÀÌÅÛ ¹øÈ£
	UINT8			iItemCount;			// -- ÂüÁ¶¿ë
} MATERIAL_DATA, *PMATERIAL_DATA;

typedef struct _EFFECT_DATA
{
	unsigned int iInstanceID;
	int iEffectID;
	UINT8 iEffectLv;
	int iRemainTime; // ´ÜÀ§ :0.1ÃÊ
	GU_ID iCasterID;
} EFFECT_DATA, *PEFFECT_DATA;

// mijin ÄðÅ¸ÀÓ ±¸Á¶Ã¼
typedef struct _COOLTIME_DATA
{
	int iCoolTimeID;
	int	iMaxCoolTime;	// Ã³À½ Àû¿ëµÉ ÄðÅ¸ÀÓ ( ´ÜÀ§ : ÃÊ )
	int iApplyCoolTime; // ³²Àº ÄðÅ¸ÀÓ ( ´ÜÀ§ : ÃÊ )

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
	int		iSubTypeID;		/* ItemID */ // ¾ÆÀÌÅÛ½ºÅ³ÀúÀå½Ã »ç¿ë

}LINKED_DATA, *PLINKED_DATA;

typedef struct _QUICK_SLOT 
{
	LINKED_DATA LinkedData;
	UINT8		iSlotIndex; /* Quick slot position */ // Valid Range : 0~MAX_QUICK_SLOT -1 
} QUICK_SLOT, *PQUICK_SLOT;

#define START_EMOTICON_SLOT	36	// 2005.08.30 dhpark ÀÌ¸ðÆ¼ÄÜ ½ÃÀÛ ÀÎµ¦½º Á¤ÀÇ

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





//  À§Ä¡°¡ ¾Ö¸ÅÇØ¼­ ÀÓ½Ã·Î ..

//////////////////////////////////////////////////////////////////////////
//
// npc·ÎºÎÅÍ »ì¶§ ÀÌ¿ë
//
//////////////////////////////////////////////////////////////////////////
//
// ÆÄ¶ó¹ÌÅÍ ¼³¸í
// ItemPrice : »ç°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛÀÇ "ITEM.LDT"ÀÇ "_GamePrice".
// ItemCount : NPC ¿¡°Ô »ì ¾ÆÀÌÅÛ °¹¼ö.
// ShopSellPercent : "NPC_TRADE.LDT"ÀÇ ÇØ´ç ¾ÆÀÌÅÛ "_SellFactor". 
// PCBuyPercent : PC¿¡°Ô Àû¿ëµÈ ±¸ÀÔ¹èÀ². ÇöÀç´Â ±âº» 100 % ·Î ÀÔ·Â¿ä¸Á. 

#define GET_BUY_PRICE(ItemPrice,ItemCnt,ShopSellPercent,PCBuyPercent) \
	(ItemPrice*ShopSellPercent/100*PCBuyPercent/100*ItemCnt)

//////////////////////////////////////////////////////////////////////////
//
// NPC¿¡°Ô ÆÈ¶§ ÀÌ¿ë
//
// ItemPrice : ÆÈ°íÀÚÇÏ´Â ¾ÆÀÌÅÛÀÇ "ITEM.LDT"ÀÇ "_GamePrice".
// ItemCount : NPC ¿¡°Ô ÆÈ ¾ÆÀÌÅÛ °¹¼ö.
// ShopBuyPercent : "NPC_TRADE.LDT"ÀÇ "_BuyFactor". 
// PCSellPercent : PC¿¡°Ô Àû¿ëµÈ ÆÇ¸Å¹èÀ². ÇöÀç´Â ±âº» 100 % ·Î ÀÔ·Â¿ä¸Á. 
// 					

#define GET_SELL_PRICE(ItemPrice,ItemCnt,ShopBuyPercent,PCSellPercent) \
	(ItemPrice*ItemCnt*ShopBuyPercent/100*PCSellPercent/100)


//////////////////////////////////////////////////////////////////////////
//
// À¯Àú°£ Ä³½¬ °Å·¡½Ã ¼ö¼ö·á °è»ê
// 20080313 mijin
//	- °Å·¡ºñ¿ëÀÇ ¼Ò¼öÁ¡ ÀÌÇÏ´Â ¹ö¸²
//
// TradeCash : °Å·¡µÇ´Â Ä³½¬
// CommPercent : ¼ö¼ö·á ÆÛ¼¾Æ®(LOCALIZE.LDT ¿¡ Ç¥±â)
//////////////////////////////////////////////////////////////////////////					
#define GET_TRADE_CASH_COMM(TradeCash, CommPercent) (TradeCash*CommPercent/100)




// 2005.07.28 dhpark Trade½Ã °Å¸®°è»ê¿ë Define
const float CHECK_INTERVAL_X = 400.0f;			/**< »ó´ë PC¿Í ÀÇ °Å¸® Ã¼Å©¿ë <br> */
const float CHECK_INTERVAL_Y = 300.0f;			/**< »ó´ë PC¿Í ÀÇ °Å¸® Ã¼Å©¿ë <br> */

//------------------------------------------------------------------------------
// QUEST

#define QUEST_MAX_ACTIVE				15	// ÁøÇàÁßÀÎ Äù½ºÆ®ÀÇ ÃÖ´ë °³¼ö
#define QUEST_MAX_NAME					64	// LDT¿¡ ¼³Á¤µÈ txtÀÇ Å©±â±âÁØÀÓ
#define QUEST_MAX_REQUIRE				5	/**< ½Àµæ¿¡ ÇÊ¿äÇÑ ½ºÅÈ Á¾·ùÀÇ ÃÖ´ë °¹¼ö <br> */
#define QUEST_MAX_ACQUISITION_EFFECTID	5	/**< Äù½ºÆ® ½Àµæ½Ã ÇÔ²² ¹ß»ýÇÏ´Â È¿°úÀÇ ÃÖ´ë °¹¼ö <br> */
#define QUEST_MAX_ACQUISITION_ITEM		2	/**< Äù½ºÆ® ½Àµæ½Ã ÇÔ²² ½ÀµæÇÏ´Â ¾ÆÀÌÅÛ ÃÖ´ë °¹¼ö <br> */
#define QUEST_MAX_MISSION				4	/**< ¿Ï·á¿¡ ÇÊ¿äÇÑ ¹Ì¼ÇÀÇ °¹¼ö <br> */
#define QUEST_MAX_CONSUMPTION_ITEM		5	/**< Äù½ºÆ® ¿Ï·á(º¸»ó)½Ã ¼ÒºñµÇ´Â ¾ÆÀÌÅÛ Á¾·ùÀÇ ÃÖ´ë °¹¼ö <br> */
#define QUEST_MAX_RANK					3	// ·©Å©ÀÇ ¼ö
#define QUEST_MAX_RANK_REQUIRE			2	// ·©Å©¸¦ °áÁ¤ÇÏ´Â Á¶°ÇÀÇ ¼ö
#define QUEST_MAX_REWARD				3	// ·©Å©¿¡¼­ ¼±ÅÃÇÒ¼öÀÖ´Â º¸»óÀÇ ¼ö
#define QUEST_MAX_DIVIDE_ITEM			5	// ¿Ï·á½Ã »èÁ¦µÉ ¾ÆÀÌÅÛ ÇÑÁ¾·ù°¡ ºÐÇÒµÉ¼öÀÖ´Â ÃÖ´ë¼ö -- °Ë»ç±ÔÄ¢:1.½½·Ôº° ½ºÅÃ¼ö°¡ »èÁ¦µÇ¾î¾ßÇÒ ½ºÅÃ¼ö¸¦ ³Ñ´Â°ÍÀÌ ÀÖ´ÂÁö °Ë»ç. 2.¾Õ¿¡¼­ ´Ù¼¸°³ÀÇ ½½·ÔÀÇ ¾ÆÀÌÅÛÀÇ ½ºÅÃ¼öÀÇ ÇÕÀÌ »èÁ¦µÇ¾î¾ßÇÒ ½ºÅÃ¼ö¸¦ ³Ñ´ÂÁö °Ë»ç. ³ª¸ÓÁö °æ¿ì´Â Mission failed...
#define MAX_ITEM_UPGRADE_MATERIALS		4

#define QUESTREWARD_MAX_EFFECTID		4	// º¸»ó ÀÌÆåÆ®ÀÇ ¼ö
#define QUESTREWARD_MAX_ITEM			6	// º¸»ó ¾ÆÀÌÅÛÀÇ ¼ö

typedef enum _enumQUEST_ACCEPTTYPE	// ¼ö¶ôÅ¸ÀÔ
{
	QUEST_ACCEPTTYPE_NPC = 1,
	QUEST_ACCEPTTYPE_SHARE = 2,
	QUEST_ACCEPTTYPE_ITEM = 4,
	QUEST_ACCEPTTYPE_EVENT = 8,

} QUEST_ACCEPTTYPE;

typedef enum _enumREWARD_TYPE	// º¸»óÅ¸ÀÔ
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
	UINT32		uiQuestID;		// Äù½ºÆ®¾ÆÀÌµð
	int			iFinishCnt;		// ¿Ï·áÇÑ È½¼ö
	UINT8		uiStatus;		// 0:¿Ï·á, 1:¼öÇàÁß

} QUEST, *PQUEST;

typedef struct _FESTIVAL
{
	int		iAppliedLoginEvent;
	time_t	tmLastLoginTime;
} FESTIVAL, *PFESTIVAL;

typedef struct _tagMISSION
{
	UINT8		uiMissionType;	// ¹Ì¼ÇÅ¸ÀÔ(0:NULL, 1:Monster, 2:Event, 3:Item, 4:Action)
	UINT32		uiValueID;		// ¹Ì¼ÇÀÇ ´ë»óÀÇ ¾ÆÀÌµð(¹Ì¼ÇÅ¸ÀÔÀÌ MonsterÀÏ¶§´Â MonsterID)
	int			iFinishValue;	// ¿Ï·á°ª
	int			iCurValue;		// ÇöÀç°ª

} MISSION, *PMISSION;

#define CASHSHOP_ERROR_NOT_OPENED 9009006 //9000004 // Ä³½¬¼¥À» ÀÓ½Ã·Î ´ÝÀº °æ¿ì
#define CASHSHOP_ERROR_FAIL 4000020   // osp ¿¡¼­ µ·±îÁö´Â »¯´Âµ¥ ±×ÀÌÈÄ ÇàÀ§°¡ ½ÇÆÐÀÎ °æ¿ì.
#define DEFAULT_ERROR_ID		4000002		// ¿¡·¯¹®±¸¸¦ º¸¿©ÁÙ ÇÊ¿ä°¡ ¾ø°Å³ª Æ÷°ýÀûÀ¸·Î º¸¿©ÁÖ°í ½ÍÀ»¶§ »ç¿ëÇÏ´Â ±Û·Î¹ú½ºÆ®¸µÀÇ ¿¡·¯¾ÆÀÌµðÀÔ´Ï´Ù. ³»¿ëÀº Áö±ÝÀº ÀÌ ÀÛ¾÷À» ÇÒ¼ö¾ø´Ù´Â ³»¿ëÁ¤µµÀÏ°ÍÀ¸·Î ¿¹»óµË´Ï´Ù.(À¯¸í»ó ÆÀÀå´ÔÀÌ Á¤ÇÒ°ÍÀÓ)
#define DEFAULT_ERROR_ID_TARGET	4000003		// ¿¡·¯¹®±¸¸¦ º¸¿©ÁÙ ÇÊ¿ä°¡ ¾ø°Å³ª Æ÷°ýÀûÀ¸·Î º¸¿©ÁÖ°í ½ÍÀ»¶§ »ç¿ëÇÏ´Â ±Û·Î¹ú½ºÆ®¸µÀÇ ¿¡·¯¾ÆÀÌµðÀÔ´Ï´Ù. ³»¿ëÀº Áö±ÝÀº »ó´ë¹æÀÌ ÀÌ ÀÛ¾÷À» ÇÒ¼ö¾ø´Ù´Â ³»¿ëÁ¤µµÀÏ°ÍÀ¸·Î ¿¹»óµË´Ï´Ù.(À¯¸í»ó ÆÀÀå´ÔÀÌ Á¤ÇÒ°ÍÀÓ)
#define DEFAULT_ERROR_USERIN	30003		// Áßº¹·Î±×ÀÎ°Ë»ç½Ã ¿¡·¯.
#define DEFAULT_ERROR_FAIL_BY_BATTLE	4008001 // ¹èÆ²ÀÏ¶§ ¸øÇÔ.// Ä³½¬¼¥ Ã¤³ÎÀÌµ¿

// Auction ¼­¹ö¿ë ¿¡·¯
//
//¹°Ç°ÀÌ Á¸ÀçÇÏÁö ¾ÊÀ» ¶§ 	È®ÀÎ	52000009
//ÆäÀÌÁö°¡ Á¸ÀçÇÏÁö ¾ÊÀ» ¶§ 	È®ÀÎ	52000074
//»óÈ¸ ÀÔÂû°¡·Î ÀÎÇØ¼­ ÀÔÂû °¡°ÝÀÌ ¸ÂÁö ¾ÊÀ» ¶§ 	È®ÀÎ	52000073
//ÀÔÂû Áß ´Ù¸¥ À¯Àú¿¡ ÀÇÇØ¼­ ÀÔÂû¿¡ ¹æÇØµÇ´Â ÇàÀ§°¡ ¹ú¾îÁú ¶§(µî·Ï Ãë¼Ò, »óÈ¸ ÀÔÂû, Áï½Ã ±¸¸Å)	È®ÀÎ	52000080

#define AUCTION_ERROR_NOT_OPENED 52000075  // °æ¸Å¼­¹ö°¡ ¿ÀÇÂµÇÁö ¾ÊÀº °æ¿ì
#define AUCTION_ERROR_BLOCKED	52000080	// °æ¸Å¹°°ÇÀÌ ºí·°µÈ°æ¿ì
#define AUCTION_ERROR_BIDPRICE  52000073 // ÀÔÂû°¡°¡ ¸ÂÁö ¾ÊÀ»¶§
#define AUCTION_ERROR_NOT_EXIST 52000009 // °æ¸Å¹°°Ç ÆÈ·È°Å³ª Ãë¼ÒµÇ¼­ ¾ø¾îÁø °æ¿ì
#define AUCTION_ERROR_EXPIRED   52000108 // °æ¸Å¹°°ÇÀÌ ¸¶°¨µÈ°æ¿ì(ÀÔÂû/Áï±¸/Ãë¼Ò) ºÒ°¡´É
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Team

#define TEAM_MAX_MEMBER		4
#define TEAM_MIN_MEMBER		2

enum
{
	TEAM_GRADE_NONE = 0,	// ÆÀ¿ø
	TEAM_GRADE_CAPTAIN,		// ÆÀÀå
	TEAM_GRADE_MAX,			// MAX
};

enum
{
	TEAM_LOOT_ORDER = 0,	// Â÷·Ê´ë·Î
	TEAM_LOOT_RANDOM,		// ·£´ý
	TEAM_LOOT_FREE,			// ÀÚÀ¯
	TEAM_LOOT_MAX,			// MAX
};

typedef struct _tagTEAM_MEMBER
{
	GU_ID	CharGUID;
	UINT8	byGrade;	// µî±Þ(0:ÆÀ¿ø, 1:ÆÀÀå)
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
	UINT8	byWhere;	// 0:ÀÏ¹ÝÀûÀÎÀå¼Ò, 1:Ä³½¬¼¥
#if defined(__ADD_WORLDTEAM) || ( !defined(_CHANNEL_TEAM_VERSION_) && defined(_LATALE_CLIENT) )
	INT32	iCharID;	// Ä³¸¯ÅÍ DBÅ°
	UINT16	wServerID;	// ¼­¹ö¾ÆÀÌµð
#endif

} TEAM_MEMBER, *PTEAM_MEMBER;

#define TEAM_DIST_X		800.0f
#define TEAM_DIST_Y		600.0f

// Team
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Title

#define TITLE_ID_NOTUSE					1	// »ç¿ë¾ÈÇÔÀ» ´ë½ÅÇÏ´Â Å°°ª
#define TITLE_MAX_NAME					64	// LDT¿¡ ¼³Á¤µÈ txtÀÇ Å©±â±âÁØÀÓ
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
	ITEM			stItem;						// ¾ÆÀÌÅÛ¿¡ ´ëÇÑ »ó¼¼Á¤º¸¸¦ º¸³»Áà¼­ ¸¶¿ì½º¸¦ ¾ÆÀÌÅÛ¿¡ À§Ä¡½ÃÅ°¸é Á¤º¸¸¦ º¸¿©ÁÙ¼öÀÖµµ·ÏÇÔ

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

#define WAYPOINT_SIZE_BIT					128	// ºñÆ®·Î ±¸ºÐÇÒ µ¥ÀÌÅÍÀÇ ¼ö *** PacketID.h·Î ÀÌ»ç¿¹Á¤
#define WAYPOINT_SIZE_DATA					4	// ºñÆ®¸¦ ÀúÀåÇÏ±â À§ÇÑ longÇü µ¥ÀÌÅÍÀÇ ¼ö *** PacketID.h·Î ÀÌ»ç¿¹Á¤
#define WAYPOINT_MAX_NAME					64	// LDT¿¡ ¼³Á¤µÈ txtÀÇ Å©±â±âÁØÀÓ
#define WAYPOINT_MAX_RANK					2	// ·©Å©ÀÇ ¼ö
#define WAYPOINT_MAX_RANK_REQUIRE			2	// ·©Å©¸¦ °áÁ¤ÇÏ´Â Á¶°ÇÀÇ ¼ö
#define WAYPOINT_MAX_DIVIDE_ITEM			5	// ¿Ï·á½Ã »èÁ¦µÉ ¾ÆÀÌÅÛ ÇÑÁ¾·ù°¡ ºÐÇÒµÉ¼öÀÖ´Â ÃÖ´ë¼ö -- °Ë»ç±ÔÄ¢:1.½½·Ôº° ½ºÅÃ¼ö°¡ »èÁ¦µÇ¾î¾ßÇÒ ½ºÅÃ¼ö¸¦ ³Ñ´Â°ÍÀÌ ÀÖ´ÂÁö °Ë»ç. 2.¾Õ¿¡¼­ ´Ù¼¸°³ÀÇ ½½·ÔÀÇ ¾ÆÀÌÅÛÀÇ ½ºÅÃ¼öÀÇ ÇÕÀÌ »èÁ¦µÇ¾î¾ßÇÒ ½ºÅÃ¼ö¸¦ ³Ñ´ÂÁö °Ë»ç. ³ª¸ÓÁö °æ¿ì´Â Mission failed...
#define WAYPOINT_MAX_FIND_EFFECTID			2	// Ã¹¹ß°ß½Ã ÀÌÆåÆ® ¼ö
#define WAYPOINT_MAX_CONSUMPTION_ITEM		1	// »èÁ¦¾ÆÀÌÅÛÀÇ ÃÖ´ë¼ö

#define DISTANCE_DEFAULT_X				800.0f	// ÀÏ¹ÝÀûÀÎ °Å¸®°Ë»ç½Ã »ç¿ëÇÒ°ÍÀÓ
#define DISTANCE_DEFAULT_Y				600.0f	// ÀÏ¹ÝÀûÀÎ °Å¸®°Ë»ç½Ã »ç¿ëÇÒ°ÍÀÓ

// WayPoint
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Event Mission

#define MAX_EVENT_MISSION_BIT			1024	// ºñÆ®·Î ±¸ºÐÇÒ µ¥ÀÌÅÍÀÇ ¼ö *** PacketID.h·Î ÀÌ»ç¿¹Á¤
#define MAX_EVENT_MISSION_REQUIRES		8
#define MAX_EVENT_MISSION_EFFECT		4
#define	MAX_EVENT_MISSION_ITEM			2

const int MAX_EVENT_MISSION_DATA		= 32;// ( MAX_EVENT_MISSION_BIT / 32);	// ºñÆ®¸¦ ÀúÀåÇÏ±â À§ÇÑ longÇü µ¥ÀÌÅÍÀÇ ¼ö *** PacketID.h·Î ÀÌ»ç¿¹Á¤
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Å¬·¡½º º¯°æ

#define MAX_CLASS_CHANGE_REQUIRES		8
#define	MAX_CLASS_CHANGE_ITEM			4

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ItemAddTime

#define ITEMADDTIME_MAX_REQUIRE				4			// Á¶°ÇÀÇ ¼ö
#define ITEMADDTIME_MAX_ADDTIME				2100000000	// ½Ã°£Á¦¾ÆÀÌÅÛÀÌ °¡Áú¼öÀÖ´Â ½Ã½ºÅÛÀû ÃÖ´ë°ª ¾à 66³â

// ItemAddTime
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ¸ÔÀÌÁÖ±â

#define ITEM_MAX_ITEMSP						100			// ¾ÆÀÌÅÛ ÃÖ´ë SP

// ItemAddTime
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Gameble

#define MAX_GAMBLE_REWARD			30
#define MAX_GAMBLE_BETTING		3   
#define MAX_GAMBLE_MATERIALS		2
#define MAX_GAMBLE_REWARD_BIT		MAX_GAMBLE_REWARD	// ºñÆ®·Î ±¸ºÐÇÒ µ¥ÀÌÅÍÀÇ ¼ö 
//const int MAX_GAMBLE_REWARD_DATA	= 1;//( MAX_GAMBLE_REWARD_BIT / 32);	// ºñÆ®¸¦ ÀúÀåÇÏ±â À§ÇÑ longÇü µ¥ÀÌÅÍÀÇ ¼ö *** PacketID.h·Î ÀÌ»ç¿¹Á¤
//------------------------------------------------------------------------------

// Auction
// °æ¸Å°Å·¡Å¸ÀÔ
enum AUCTION_TYPE
{
	AUCTION_TYPE_NULL,
	AUCTION_TYPE_ITEM_BY_ELY,	// ÆÇ¸Å : ¾ÆÀÌÅÛ,	±¸¸Å¼ö´Ü : Ely
	AUCTION_TYPE_CASH_BY_ELY,	// ÆÇ¸Å : Cash,		±¸¸Å¼ö´Ü : Ely
	AUCTION_TYPE_MILEAGE_BY_ELY,// ÆÇ¸Å : Mileage,	±¸¸Å¼ö´Ü : Ely
	AUCTION_TYPE_ELY_BY_CASH,	// ÆÇ¸Å : Ely,		±¸¸Å¼ö´Ü : Cash
	AUCTION_TYPE_ELY_BY_MILEAGE,// ÆÇ¸Å : Ely,		±¸¸Å¼ö´Ü : Mileage
	AUCTION_TYPE_MAX,
};

//------------------------------------------------------------------------------
#define MAX_MAIl_BODY_LEN MAILBOX_MAX_MEMO_LEN 

// SYSTEM MAIL TYPE
// ½Ã½ºÅÛ¸ÞÀÏÀÎ°æ¿ì Å¸ÀÌÆ²ÀÌ ´Þ¶óÁø´Ù¸é
enum MAIL_TYPE
{
	MAIL_TYPE_NULL	,									// »ç¿ëÀÚ ¸ÞÀÏ
	MAIL_TYPE_AUCTION_BASE		= 50,					// °æ¸Å
	MAIL_TYPE_AUCTION_SELL		= MAIL_TYPE_AUCTION_BASE +1,	// ÆÇ¸Å¿Ï·á : µî·ÏÀÚ°¡ ¹ÞÀ»¶§
	MAIL_TYPE_AUCTION_BUY,								// ³«Âû(Áï±¸Æ÷ÇÔ): ÀÔÂûÀÚ°¡¹ÞÀ»¶§
	MAIL_TYPE_AUCTION_OUTBID,							// »óÈ¸ÀÔÂû : ±âÀÔÂûÀÚ°¡ÀÇ ÀÔÂû±ÝÀ» µ¹·Á¹ÞÀ»¶§
	MAIL_TYPE_AUCTION_CANCEL_BIDDER,							// °æ¸ÅÃë¼Ò : µî·ÏÀÚ°¡ °æ¸Å¹°°ÇÀ» Ãë¼Ò½Ã(µî·ÏÀÚ/ÀÔÂûÀÚ)
	MAIL_TYPE_AUCTION_CANCEL_SELLER,
	MAIL_TYPE_AUCTION_EXPIRED,							// À¯Âû : µî·ÏÀÚ°¡ ¿Ã¸° ¹°°ÇÀÇ ±â°£ÀÌ Áö³µÀ»¶§
};			   

#define AUCTION_MAIL_DELIMETER "_"						// °æ¸Å¸ÞÀÏ¿¡¼­ ³»¿ë ±¸ºÐÀÚ
//////////////////////////////////////////////////////////////////////
// MailBox

// Client <-> Server
typedef struct _MAIL_HEADER
{
	UINT64				uiMailID;				// Mail index
	TIMESTAMP_STRUCT	tmSendTime;				// Àü¼Û ½Ã°¢
	TCHAR				szSender[LEN_NAME+1];	// ¼Û½ÅÀÚ
	UINT8				iType;					// MAIL_TYPE 
}MAIL_HEADER,*PMAIL_HEADER;

typedef struct _MAIL_ATTACHMENT
{
	UINT64	uiEly;					// Ã·ºÎÇÒ ely amount
	UINT32	uiItemID;				// Ã·ºÎµÈ Item Type (tbluItem2::TypeID)
	UINT64	uiItemNo;				// Ã·ºÎµÈ Item NO (tbluItem2::ItemNo)
	UINT8	uiItemCount;			// Ã·ºÎµÈ Item °¹¼ö (tbluItem2::cnt)
	UINT8	uiContainerType;		// Ã·ºÎµÈ ItemÀÇ ÀÎº¥Åä¸® Å¸ÀÔ
	UINT8	uiContainerSlot;		// Ã·ºÎµÈ ItemÀÇ ÀÎº¥Åä¸®³»ÀÇ À§Ä¡
	UINT8	uiReadFlag;				// IsNewMail?  ÀÐÁö ¾ÊÀº »õ·Î¿î ¸ÞÀÏÀÎÁö¸¦ Ç¥ÇöÇÔ (tbluMailBox::ReadFlag)
}MAIL_ATTACHMENT,*PMAIL_ATTACHMENT;

// ¸ÞÀÏ ¼Û½Å Á¤º¸ + Á¦¸ñ + Ã·ºÎ Á¤º¸
typedef struct _MAIL_HEADER_ATTACHMENT: public MAIL_ATTACHMENT, public MAIL_HEADER
{
}MAIL_HEADER_ATTACHMENT,*PMAIL_HEADER_ATTACHMENT;


//////////////////////////////////////////////////////////////////////
// Server <-> DB : ¼­¹ö ³»ºÎ¿ë ±¸Á¶Ã¼
	typedef struct _MAIL_HEADER_ATTACHMENT_SS: public MAIL_ATTACHMENT, public MAIL_HEADER
	{
		TCHAR	szSubject[MAILBOX_MAX_SUBJECT_LEN+1];				// Á¦¸ñ : MAIL_HEADER::iType > MAIL_TYPE_AUCTION_BASE ÀÎ°æ¿ì´Â "\0"  
	}MAIL_HEADER_ATTACHMENT_SS,*PMAIL_HEADER_ATTACHMENT_SS;

	typedef struct _MAIL_INFO_SS: public MAIL_HEADER_ATTACHMENT_SS
	{
		TCHAR	szReceiver[LEN_NAME+1];
		USERID	iReceiverUserID;
		CHARID	iReceiverCharID;
		USERID	iSenderUserID;
		CHARID	iSenderCharID;

		// MAIL_HEADER::iType > MAIL_TYPE_AUCTION_BASE ÀÎ°æ¿ì 
		// "AUCTION-TYPE_¿ø±Ý_¼ö¼ö·á_ÃÖÁ¾°¡_ITEMID_VALUE" ¸¦ "%d_%d_%d_%d_%d_%d" Æ÷¸Ë·Î Àü´ÞÇÏ¸ç ±Ý¾×°ú °ü·ÃÀÌ ¾ø´Â ¾ÆÀÌÅÛ °ü·Ã¹°°ÇÀÌ¾îµµ Æ÷¸ËÀº ¸ÂÃß¾î º¸³½´Ù
		// ITEMID ´Â ÆÇ¸Å¿Ï·á½Ã ÆÇ¸ÅÇÑ ¹°°ÇÀÌ ¾ÆÀÌÅÛÀÎ °æ¿ì 
		// VALUE´Â AUCTION-TYPE µû¶ó ely / mile / cash ÀÏ¼ö ÀÖÀ½
		TCHAR	szMemo[MAILBOX_MAX_MEMO_LEN+1];						
		CONTAINER_SLOT StampItemPos;	// Ãß°¡
		ITEMNO		   iStampItemNo;	// Ãß°¡
	}MAIL_INFO_SS,*PMAIL_INFO_SS;


//////////////////////////////////////////////////////////////////////

// Ãß°¡
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


// 20080411 mijin ÃßÃµ»óÇ° 
typedef struct _CASHSHOP_HOT_INFO
{
	int  	iShopID;
	UINT8 	iDescLength;
	TCHAR	strDesc[50];

} CASHSHOP_HOT_INFO, *PCASHSHOP_HOT_INFO;

//	jmulro ¿ìËÙ¹ºÂò£¬2008£­8£­26
typedef struct _Quick_Buy_Info
{
	int  m_iPackageID;			//	ÉÌ³ÇµÄID
	int  m_nLevel;				//	Íæ¼ÒµÄµÈ¼¶
	int  m_nOrder;				//	°Ú·ÅµÄÎ»ÖÃ
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
// PCÀÇ Á¶ÇÕÁ¤º¸
typedef struct _ITEMMIX_INFO
{
	UINT8	iLv;		//  Á¶ÇÕ·¹º§
	int		iCurExp;	//  Á¶ÇÕ°æÇèÄ¡ ÇöÀç·®
	int		iMinExp;	//  Á¶ÇÕ°æÇèÄ¡ ½ÃÀÛ
	int		iMaxExp;	//  Á¶ÇÕ°æÇèÄ¡ ³¡
	UINT8	iRareProb;  //  Á¶ÇÕ½Ã ·¹¾îµµ °áÁ¤È®·üÆÑÅÍ
	UINT8   iSuccessProb;  // Á¶ÇÕ¼º°øÈ®·üÆÑÅÍ
	UINT8	iOptProb;	// Á¶ÇÕ½Ã ¿É¼ÇÆ÷ÀÎÆ® °áÁ¤È®·ü ÆÑÅÍ
}ITEMMIX_INFO;

//////////////////////////////////////////////////////////////////////
// PCÀÇ pvp Á¤º¸
typedef struct _PVP_STATUS
{
	UINT8		iLv;		//  PVP ¸í¿¹·¹º§
	__int64		iGlory;		//  ¸í¿¹ ÇöÀç·®
	int			iPoint;		//	PVP °Å·¡¿ë Æ÷ÀÎÆ®
}PVP_STATUS;

//////////////////////////////////////////////////////////////////////
// ½ºÅ³°ü·Ã
typedef struct _CHAR_SKILL
{
	int				iSkillID;	// SKILL.LDT::ItemID
	short			iSkillLv;	// PC°¡ Æ÷ÀÎÆ®¸¦ Âï¾î¼­ ¿Ã¸° °íÀ¯°ª.
	short			iSkillCurrLv;// PC°¡ ¾ÆÀÌÅÛ ÀåÂø/½ºÅ³/¹öÇÁ µîÀ» ÅëÇØ¼­ ¿Ã¸° ÀÓ½ÃÀûÀÎ °ª±îÁö °è»êµÈ ÃÖÁ¾°ª.
	UINT16			iSkillNo;	// ½ºÅ³ ¿À´õ¸µ¿ë
} CHAR_SKILL, *PCHAR_SKILL;

enum SKILL_SAVE_TYPE
{
	SKILL_SAVE_NULL,	// ÀÏ¹Ý½ºÅ³ ÃÊ±âÈ­½Ã »èÁ¦
	SKILL_SAVE_GENERAL, // ÀÏ¹Ý½ºÅ³ ÃÊ±âÈ­½Ã ³²°ÜµÒ
	SKILL_SAVE_ITEMMIX,	// Á¶ÇÕ½ºÅ³ ÃÊ±âÈ­½Ã »èÁ¦
};

//////////////////////////////////////////////////////////////////////
// °æ¸Å

// OSP °Å·¡Å¸ÀÔ 20080325 mijin
enum OSP_TRADE
{
	TRADE_TYPE_USER = 1,		// À¯Àú°£ Á÷°Å·¡
	TRADE_AUCTION_REGISTER,			// °æ¸Åµî·Ï
	TRADE_AUCTION_CASH_BUY,			// Ä³½¬(¸¶ÀÏ¸®Áö)·Î Áï½Ã±¸¸Å
	TRADE_AUCTION_ELY_BUY,			// ¾ÆÀÌÅÛ(°ÔÀÓ¸Ó´Ï)·Î Áï½Ã±¸¸Å
	TRADE_AUCTION_TENDER,			// °æ¸ÅÀÔÂû
	TRADE_AUCTION_ACCEPT,			// °æ¸Å³«Âû
	TRADE_AUCTION_CANCEL = 9,			// °æ¸ÅÃë¼Ò
};

// °æ¸Å±â°£ Å¸ÀÔ
enum AUCTION_DURATION
{
	PERIOD_SHORT,		// ´Ü±â		(6)
	PERIOD_LONG,		// Àå±â		(12)
	PERIOD_EXTREME,		// ÃÖÀå±â	(24)
};

// ¾ÆÀÌÅÛ°æ¸Å¼ÒÆ®Å°
// ³»¸²Â÷¼ø:0, ¿À¸§Â÷¼ø:1
enum AUCTION_SORT_KEY
{
	AUCTION_SORT_NULL,
	AUCTION_SORT_QUALITY,	// Ç°Áú
	AUCTION_SORT_ITEM_LV,		// ·¹º§
	AUCTION_SORT_ITEM_OPTPOINT,	// ³»±¸µµ
	AUCTION_SORT_DURATION,	// ¸¶°¨½Ã°£
	AUCTION_SORT_PRICE,			// ÀÔÂû°¡(ely,cash,mile)
	AUCTION_SORT_MAX,
};

// °æ¸Å±âº»Á¤º¸
typedef struct _AUCTION_HEADER
{
	__int64 iAuctionID;				// °æ¸ÅÅ°
	UINT8	iType;					// AUCTION_TYPE (°æ¸ÅÅ¸ÀÔ)
	UINT8	iDuration;				// AUCTION_DURATION (ÆÇ¸ÅÀÚ°¡ ¼¼ÆÃÇÑ)
	USERID	iSellerID;				// ÆÇ¸Å°¡ °èÁ¤Å°
	TCHAR	szSeller[LEN_NAME+1];	// ÆÇ¸ÅÀÚ Ä³¸¯ÅÍ
	UINT32	iRemainTime;			// ÇöÀç³²Àº½Ã°£

	__int64 iBidPrice;				// ÃÖÃÊÀÔÂû°¡ È¤Àº »óÈ¸ÀÔÂû°¡
	__int64 iBuyOutPrice;			// Áï±¸°¡
	__int64 iCurrPrice;				// ÇöÀç°¡
	USERID  iCurrUserID;			// ÇöÀçÀÔÂûÀÚ°èÁ¤Å°
	TCHAR	szBidder[LEN_NAME+1];	// ÃÖ°íÀÔÂûÀÚ
} AUCTION_HEADER, *PAUCTION_HEADER;

// °æ¸Å°Å·¡Å¸ÀÔº°Ãß°¡µ¥ÀÌÅ¸
typedef struct _AUCTION_GOODS_ITEM 
{
	ITEM stAuctionItem;
} AUCTION_GOODS_ITEM, *PAUCTION_GOODS_ITEM;

typedef struct _AUCTION_GOODS_POINT 
{
	int SellPoint; // Ely, Cash, Mileage
	int FeePoint;  // ¼ö¼ö·á
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
// PCÀÇ °áÈ¥Á¤º¸

enum WEDDING_PC_STATUS
{
	WEDDING_STATUS_NONE =		0,			// 0
	WEDDING_STATUS_LOVE =		1 << 0,		// 1 ±³Á¦»óÅÂ
	WEDDING_STATUS_PROPOSE =	1 << 1,		// 2 ÇÁ·¯Æ÷Áî»óÅÂ
	WEDDING_STATUS_WEDDING =	1 << 2,		// 4 °áÈ¥»óÅÂ
	//WEDDING_STATUS_PARTING =	1 << 3,		// 8 ÀÌº°»óÅÂ ÀÌº°À» ÇßÀ»¶§ WEDDING_STATUS_NONEÀ¸·Î ¼ÂÆÃÇÏ±â·ÎÇÔ
};

enum WEDDING_TYPE
{
	WEDDING_TYPE_NONE =		0,			// 0
	WEDDING_TYPE_LOVE,				//±³Á¦
	WEDDING_TYPE_PROPOSE,			//ÇÁ·ÎÆ÷Áî
	WEDDING_TYPE_WEDDING			//°áÈ¥
};

typedef struct _WEDDING_INFO
{
	UINT8				iLv;							//  Wedding Lv
	int					iLP;							//  Love Point
	int					iLPM;							//	Love Point Max
	int					iDay;							//	±³Á¦ÀÏ¼ö ¶Ç´Â °áÈ¥ÀÏ¼ö
	UINT8				iStatus;						//	ÇöÀç»óÅÂ
	int					iPartingNum;					//	ÀÌº°Áö³­½Ã°£(ÃÊ´ÜÀ§)
	TCHAR				szPartName[LEN_NAME+1];			//	»ó´ë¹æ ÀÌ¸§

}WEDDING_INFO;


#pragma pack(pop)
