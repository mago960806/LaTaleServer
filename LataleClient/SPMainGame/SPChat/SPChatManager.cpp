// ***************************************************************
//  SPChatManager   version:  1.0   ·  date: 04/07/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"

#include "SPFXDEF.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "GlobalDefines_Share.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPChatManager.h"

SPChatManager::SPChatManager()
{
}

SPChatManager::~SPChatManager()
{
}

//////////////////////////////////////////////////////////////////////////
//
//	싱글톤 인터페이스
//

SPChatManager* SPChatManager::m_pChatManagerInstance	=	NULL;

SPChatManager* SPChatManager::GetInstance()
{
	if( m_pChatManagerInstance	==	NULL )
		m_pChatManagerInstance	=	new	SPChatManager;

	return	m_pChatManagerInstance;
}

void SPChatManager::DelInstance()
{
	delete m_pChatManagerInstance;
}

//////////////////////////////////////////////////////////////////////////
//
//	초기화
//

void SPChatManager::Init()
{
	LoadGlobalSysLDT();
	LoadStyleLDT( RES_FNAME_LDT_MY_STYLE		, m_mMyStyle );
	LoadStyleLDT( RES_FNAME_LDT_ROOM_STYLE		, m_mRoomStyle );
	LoadStyleLDT( RES_FNAME_LDT_AUTO_SUBJECT	, m_mRoomAutoSubject );

	//////////////////////////////////////////////////////////////////////////

	ClearChatRoomInfo();
	SetChatRoomAdvert( NULL );
	SetSendPacket( true );
	SetLocalDBKey( 0 );
	SetJoin( false );
}

STYLE_LDT* SPChatManager::GetStyle( CHAT_LDT_TYPE eChatLDTType , int iStyleID , int iIndex )
{
	STYLE_LDT*	pStyleLDT	=	NULL;

	switch( eChatLDTType )
	{
		case CHAT_LDT_TYPE_ROOM_STYLE:		pStyleLDT	=	GetStyle( m_mRoomStyle			, iStyleID , iIndex );	break;
		case CHAT_LDT_TYPE_MY_STYLE:		pStyleLDT	=	GetStyle( m_mMyStyle			, iStyleID , iIndex );	break;
		case CHAT_LDT_TYPE_ROOM_TYPE:		pStyleLDT	=	GetStyle( m_mRoomAutoSubject	, iStyleID , iIndex );	break;
	}

	return pStyleLDT;
}

int SPChatManager::GetStyleCount( CHAT_LDT_TYPE eChatLDTType , int iStyleID )
{
	int			iSytleCount	=	0;

	switch( eChatLDTType )
	{
		case CHAT_LDT_TYPE_ROOM_STYLE:		iSytleCount	=	GetStyleCount( m_mRoomStyle			, iStyleID );	break;
		case CHAT_LDT_TYPE_MY_STYLE:		iSytleCount	=	GetStyleCount( m_mMyStyle			, iStyleID );	break;
		case CHAT_LDT_TYPE_ROOM_TYPE:		iSytleCount	=	GetStyleCount( m_mRoomAutoSubject	, iStyleID );	break;
	}

	return iSytleCount;
}

bool SPChatManager::IsChatRoom()
{
	if( m_stChatRoomInfo.uiID > 0 && m_stChatRoomInfo.uiRoomNo > 0 && m_stChatRoomInfo.iMasterCharID > 0 )
		return true;

	return false;
}

int SPChatManager::GetRequire( CHAT_LDT_REQUIRE eChatLDTRequire )
{
	int	iIndex	=	(int)eChatLDTRequire;

	if( iIndex <= CHAT_LDT_REQUIRE_NULL || iIndex >= CHAT_LDT_REQUIRE_MAX )
		return 0;

	return m_iRequireLDT[ iIndex ];
}

void SPChatManager::SetChatRoomInfo( CHATROOM_INFO& stChatRoomInfo )
{
	CopyMemory( &m_stChatRoomInfo , &stChatRoomInfo , sizeof( CHATROOM_INFO ) );
}

CHATROOM_INFO* SPChatManager::GetChatRoomInfo()
{
	return &m_stChatRoomInfo;
}

void SPChatManager::ClearChatRoomInfo()
{
	ZeroMemory( &m_stChatRoomInfo , sizeof( CHATROOM_INFO ) );
}

void SPChatManager::SetChatRoomAdvert( TCHAR* pstrAdvert )
{
	if( pstrAdvert == NULL )
	{
		memset( &( m_strChatRoomAdvert )	, 0 , sizeof( m_strChatRoomAdvert )	);
	}
	else
	{
		strncpy( m_strChatRoomAdvert , pstrAdvert , MAX_CHATROOM_ADVERT_LEN );
	}
}

TCHAR* SPChatManager::GetCharRoomAdvert()
{
	return	m_strChatRoomAdvert;
}

void SPChatManager::SetMasterDBKey( UINT32 uiMasterDBKey )
{
	if( IsChatRoom() == false )
		return;

	m_stChatRoomInfo.iMasterCharID	=	uiMasterDBKey;
}

UINT32 SPChatManager::GetMasterDBKey()
{
	if( IsChatRoom() == false )
		return 0;

	return m_stChatRoomInfo.iMasterCharID;
}

void SPChatManager::SetLocalDBKey( UINT32 uiLocalDBKey )
{
	m_uiLocalPlayerDBKey	=	uiLocalDBKey;
}

UINT32 SPChatManager::GetLocalDBKey()
{
	return m_uiLocalPlayerDBKey;
}

bool SPChatManager::IsMaster()
{
	if( IsChatRoom() == false )
		return false;

	if( GetMasterDBKey() == GetLocalDBKey() )
		return true;

	return false;
}

bool SPChatManager::IsSendPacket()
{
	return m_bSendPacket;
}

void SPChatManager::SetSendPacket( bool bSendPacket )
{
	m_bSendPacket	=	bSendPacket;
}

void SPChatManager::AddAdvert( int iKey , CHATROOM_INFO& stChatRoomInfo )
{
	CHATROOM_INFO*	pChatRoomInfo	=	GetAdvert( iKey );

	if( pChatRoomInfo != NULL )
	{
		CopyMemory( pChatRoomInfo , &stChatRoomInfo , sizeof( CHATROOM_INFO ) );
		return;
	}

	m_mAdvert.insert( STD_MAP_ADVERT::value_type( iKey , stChatRoomInfo ) );
}

CHATROOM_INFO* SPChatManager::GetAdvert( int iKey )
{
	CHATROOM_INFO*				pChatRoomInfo	=	NULL;
	STD_MAP_ADVERT::iterator	mIter			=	m_mAdvert.find( iKey );

	if( mIter != m_mAdvert.end() )
		pChatRoomInfo	=	&((*mIter).second);

	return pChatRoomInfo;
}

void SPChatManager::DelAdvert( int iKey )
{
	STD_MAP_ADVERT::iterator	mIter			=	m_mAdvert.find( iKey );

	if( mIter != m_mAdvert.end() )
		m_mAdvert.erase( mIter );
}

void SPChatManager::ClearAdvert()
{
	m_mAdvert.clear();
}

void SPChatManager::SetJoin( bool bJoin )
{
	m_bJoin	=	bJoin;
}

bool SPChatManager::GetJoin()
{
	return m_bJoin;
}

void SPChatManager::LoadGlobalSysLDT()
{
	for( int i = 0 ; i < CHAT_LDT_REQUIRE_MAX ; ++i )
	{
		m_iRequireLDT[ i ]	=	0;
	}

	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_GLOBAL_SYS , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [ %s ] \n" , RES_FNAME_LDT_GLOBAL_SYS );
#endif
		return;
	}

	LDT_Field	ldtFieldData;

	pLDTFile->GetField( 70 , 0 ,	ldtFieldData );		//	m_iRequireItem
	m_iRequireLDT[ CHAT_LDT_REQUIRE_ITEM ]	=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 71 , 0 ,	ldtFieldData );		//	m_iRequireRoomMax
	m_iRequireLDT[ CHAT_LDT_REQUIRE_ROOM_MAX ]	=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 72 , 0 ,	ldtFieldData );		//	m_iRequireSearchMax
	m_iRequireLDT[ CHAT_LDT_REQUIRE_SEARCH_MAX ]	=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 73 , 0 ,	ldtFieldData );		//	m_iRequireAdvertisingTime
	m_iRequireLDT[ CHAT_LDT_REQUIRE_ADVERTISINGTIME ]	=	ldtFieldData.uData.lValue;

	SAFE_RELEASE( pLDTFile );
}

void SPChatManager::LoadStyleLDT( const char* pszLDTFile , STD_MAP_STYLE& mStyle )
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( pszLDTFile , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [ %s ] \n" , pszLDTFile );
#endif
		return;
	}

	mStyle.clear();

	LDT_Field					ldtFieldType;
	LDT_Field					ldtFieldValue;

	int							iRecordCount	=	pLDTFile->GetItemCount();
	int							iItemID;

	STYLE_LDT					stStyleLDT;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID		=	pLDTFile->GetPrimaryKey( iRecord );

		if( iItemID <= 0 )
			continue;

		pLDTFile->GetField( iItemID, 0	,	ldtFieldType	);			//	_Type
		pLDTFile->GetField( iItemID, 1	,	ldtFieldValue	);			//	_Value

		stStyleLDT.m_iStyleID			=	ldtFieldType.uData.lValue;

		if( ldtFieldValue.uData.pValue != NULL )
			stStyleLDT.m_strTitle	=	ldtFieldValue.uData.pValue;

		AddStyle( mStyle , stStyleLDT.m_iStyleID , stStyleLDT );

		stStyleLDT.m_iStyleID			=	0;
		stStyleLDT.m_strTitle.clear();
	}

	SAFE_RELEASE( pLDTFile );
}

void SPChatManager::AddStyle( STD_MAP_STYLE& mStyle , int iStyleID , STYLE_LDT& stStyleLDT )
{
	STD_VECTOR_STYLE_LDT*	pvStyleLDT	=	GetStyle( mStyle , iStyleID );

	if( pvStyleLDT == NULL )
	{
		STD_VECTOR_STYLE_LDT	vStyleLDT;

		vStyleLDT.push_back( stStyleLDT );

		mStyle.insert( STD_MAP_STYLE::value_type( iStyleID , vStyleLDT ) );
	}
	else
	{
		pvStyleLDT->push_back( stStyleLDT );
	}
}

int SPChatManager::GetStyleCount( STD_MAP_STYLE& mStyle , int iStyleID )
{
	int						iCount		=	0;
	STD_VECTOR_STYLE_LDT*	pvStyleLDT	=	GetStyle( mStyle , iStyleID );

	if( pvStyleLDT != 0 )
		iCount	=	(int)pvStyleLDT->size();

	return iCount;
}

STD_VECTOR_STYLE_LDT* SPChatManager::GetStyle( STD_MAP_STYLE& mStyle , int iStyleID )
{
	STD_VECTOR_STYLE_LDT*	pvStyleLDT	=	NULL;
	STD_MAP_STYLE::iterator	mIter		=	mStyle.find( iStyleID );

	if( mIter != mStyle.end() )
		pvStyleLDT	=	&((*mIter).second);

	return pvStyleLDT;
}

STYLE_LDT* SPChatManager::GetStyle( STD_MAP_STYLE& mStyle , int iStyleID , int iIndex )
{
	STYLE_LDT*				pStyleLDT	=	NULL;
	STD_VECTOR_STYLE_LDT*	pvStyleLDT	=	GetStyle( mStyle , iStyleID );

	if( pvStyleLDT == NULL )
		return pStyleLDT;

	int	iCount	=	(int)pvStyleLDT->size();

	if( iIndex < iCount && iIndex >= 0 )
		pStyleLDT	=	&( pvStyleLDT->at( iIndex ) );

	if( iIndex < 0 )
		pStyleLDT	=	&( pvStyleLDT->at( (int)GetRandom( iCount - 1) ) );

	return pStyleLDT;
}

STYLE_LDT* SPChatManager::GetStyle( STD_VECTOR_STYLE_LDT* pvStyle , int iIndex )
{
	STYLE_LDT*				pStyleLDT	=	NULL;

	if( pvStyle == NULL )
		return pStyleLDT;

	int	iCount	=	(int)pvStyle->size();

	if( iIndex < iCount && iIndex >= 0 )
		pStyleLDT	=	&( pvStyle->at( iIndex ) );

	return pStyleLDT;
}
