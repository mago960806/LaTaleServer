// ***************************************************************
//  SPChatManager   version:  1.0   ·  date: 04/07/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPChatManager_h__
#define SPChatManager_h__

#pragma once

enum ROOM_STYLE_ID
{
	ROOM_STYLE_NULL				=	0		,
	ROOM_STYLE_TYPE							,
	ROOM_STYLE_GENDER						,
	ROOM_STYLE_AGE							,
	ROOM_STYLE_MAX							,
};

enum MY_STYLE_ID
{
	MY_STYLE_NULL				=	0		,
	MY_STYLE_HAIR							,
	MY_STYLE_FASHION						,
	MY_STYLE_PERSONALITY					,
	MY_STYLE_STATE							,
	MY_STYLE_MAX							,
};

enum ROOM_TYPE_ID
{
	ROOM_TYPE_NULL				=	0		,
	ROOM_TYPE_NORMAL						,
	ROOM_TYPE_MARKET						,
	ROOM_TYPE_WANTED						,
	ROOM_TYPE_MEETING						,
	ROOM_TYPE_MAX							,
};

enum CHAT_LDT_TYPE
{
	CHAT_LDT_TYPE_NULL			=	0		,
	CHAT_LDT_TYPE_ROOM_STYLE				,
	CHAT_LDT_TYPE_MY_STYLE					,
	CHAT_LDT_TYPE_ROOM_TYPE					,
	CHAT_LDT_TYPE_MAX						,
};

enum CHAT_LDT_REQUIRE
{
	CHAT_LDT_REQUIRE_NULL		=	0		,
	CHAT_LDT_REQUIRE_ITEM					,
	CHAT_LDT_REQUIRE_ROOM_MAX				,
	CHAT_LDT_REQUIRE_SEARCH_MAX				,
	CHAT_LDT_REQUIRE_ADVERTISINGTIME		,
	CHAT_LDT_REQUIRE_MAX					,
};

struct STYLE_LDT
{
	int												m_iStyleID;
	std::string										m_strTitle;
};

typedef std::vector< STYLE_LDT >					STD_VECTOR_STYLE_LDT;

typedef	std::map< int , STD_VECTOR_STYLE_LDT >		STD_MAP_STYLE;

typedef	std::map< int , CHATROOM_INFO >				STD_MAP_ADVERT;

class SPChatManager
{
private:
	SPChatManager									();

	static	SPChatManager*							m_pChatManagerInstance;

public:
	virtual ~SPChatManager							();

	static	SPChatManager*							GetInstance(void);
	static	void									DelInstance(void);

	//	초기화
	void											Init					();

	STYLE_LDT*										GetStyle				( CHAT_LDT_TYPE eChatLDTType , int iStyleID , int iIndex );
	int												GetStyleCount			( CHAT_LDT_TYPE eChatLDTType , int iStyleID );
	int												GetRequire				( CHAT_LDT_REQUIRE eChatLDTRequire );

	bool											IsChatRoom				();

	void											SetChatRoomInfo			( CHATROOM_INFO& stChatRoomInfo );
	CHATROOM_INFO*									GetChatRoomInfo			();
	void											ClearChatRoomInfo		();

	void											SetChatRoomAdvert		( TCHAR* pstrAdvert );
	TCHAR*											GetCharRoomAdvert		();

	void											SetMasterDBKey			( UINT32 uiMasterDBKey );
	UINT32											GetMasterDBKey			();

	void											SetLocalDBKey			( UINT32 uiLocalDBKey );
	UINT32											GetLocalDBKey			();

	bool											IsMaster				();

	bool											IsSendPacket			();
	void											SetSendPacket			( bool bSendPacket );

	void											AddAdvert				( int iKey , CHATROOM_INFO& stChatRoomInfo );
	CHATROOM_INFO*									GetAdvert				( int iKey );
	void											DelAdvert				( int iKey );
	void											ClearAdvert				();

	void											SetJoin					( bool bJoin );
	bool											GetJoin					();

protected:

	void											LoadGlobalSysLDT		();
	void											LoadStyleLDT			( const char* pszLDTFile , STD_MAP_STYLE& mStyle );

	void											AddStyle				( STD_MAP_STYLE& mStyle , int iStyleID , STYLE_LDT& stStyleLDT );
	int												GetStyleCount			( STD_MAP_STYLE& mStyle , int iStyleID );
	STD_VECTOR_STYLE_LDT*							GetStyle				( STD_MAP_STYLE& mStyle , int iStyleID );
	STYLE_LDT*										GetStyle				( STD_MAP_STYLE& mStyle , int iStyleID , int iIndex );
	STYLE_LDT*										GetStyle				( STD_VECTOR_STYLE_LDT* pvStyle , int iIndex );

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	int												m_iRequireLDT[ CHAT_LDT_REQUIRE_MAX ];

	int												m_iRequireItem;
	int												m_iRequireRoomMax;
	int												m_iRequireSearchMax;
	int												m_iRequireAdvertisingTime;

	STD_MAP_STYLE									m_mMyStyle;
	STD_MAP_STYLE									m_mRoomStyle;
	STD_MAP_STYLE									m_mRoomAutoSubject;

	STD_MAP_ADVERT									m_mAdvert;

	//////////////////////////////////////////////////////////////////////////

	bool											m_bSendPacket;
	CHATROOM_INFO									m_stChatRoomInfo;
	TCHAR											m_strChatRoomAdvert[ MAX_CHATROOM_ADVERT_LEN ];
	UINT32											m_uiLocalPlayerDBKey;

	//////////////////////////////////////////////////////////////////////////

	bool											m_bJoin;

	
};

#endif // SPChatManager_h__