// ***************************************************************
//  SPLoveManager		Version:  1.0		Date: 2008/09/25
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

#include "SPManager.h"
#include "SPSubGameManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModel.h"
#include "SPGOBModelUnit.h"
#include "SPMonsterModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPGOBCluster.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBModelUnit.h"			
#include "SPPlayerEquipInfo.h"
#include "SPGOBStatus.h"			
#include "SPPlayerInvenArchive.h"	
#include "SPPlayerStatusModel.h"
#include "SPEventStatusModel.h"
#include "SPPlayerEquipInfo.h"
//#include "SPItemAttr.h"
//#include "SPItemStatus.h"
//#include "SPGuildArchive.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPWindow.h"
#include "SPWindowEdit.h"
#include "SPWindowButton.h"
#include "SPNoticeBox.h"
#include "SPInterfaceManager.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPGlobalTimer.h"

#include "SPChatManager.h"

#include "SPCheckManager.h"

#include "SPLoveManager.h"
#include "d3dx9math.h"
#define D3DXCOLOR_ARGB2(a,r,g,b) \
	(((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
SPLoveManager::SPLoveManager()
{
}

SPLoveManager::~SPLoveManager()
{
	CleanColor2();
}

//////////////////////////////////////////////////////////////////////////
//
//	싱글톤 인터페이스
//

SPLoveManager* SPLoveManager::m_pLoveManagerInstance	=	NULL;

SPLoveManager* SPLoveManager::GetInstance()
{
	if( m_pLoveManagerInstance	==	NULL )
		m_pLoveManagerInstance	=	new	SPLoveManager;

	return	m_pLoveManagerInstance;
}

void SPLoveManager::DelInstance()
{
	delete m_pLoveManagerInstance;
}

//////////////////////////////////////////////////////////////////////////
//
//	초기화
//

void SPLoveManager::Init()
{
	LoadGlobalSysLDT();
	LoadLDT();

	ClearLoveInfo();
	SetLifePartnerGUID( 0 );
}

void SPLoveManager::ClearVariables()
{
//	ClearLoveInfo();
	SetLifePartnerGUID( 0 );
}

int SPLoveManager::GetRequire( LOVE_LDT_REQUIRE eChatLDTRequire )
{
	int	iIndex	=	(int)eChatLDTRequire;

	if( iIndex <= LOVE_LDT_REQUIRE_NULL || iIndex >= LOVE_LDT_REQUIRE_MAX )
		return 0;

	return m_iRequireLDT[ iIndex ];
}

LOVE_LEVEL_DATA* SPLoveManager::GetLevelData( int iLevel )
{
	STD_MAP_LOVE_LEVEL_DATA::iterator	mIter	=	m_mLoveLevelData.find( iLevel );

	if( mIter == m_mLoveLevelData.end() )
		return NULL;

	return &((*mIter).second);
}

void SPLoveManager::SetLoveInfo( WEDDING_INFO& stLoveInfo )
{
	CopyMemory( &m_stLoveInfo , &stLoveInfo , sizeof( WEDDING_INFO ) );

	m_iAbsoluteTime	=	GetGlobalAbsoluteTime();
}

WEDDING_INFO* SPLoveManager::GetLoveInfo()
{
	return	&m_stLoveInfo;
}

void SPLoveManager::ClearLoveInfo()
{
	ZeroMemory( &m_stLoveInfo , sizeof( WEDDING_INFO ) );

	m_iAbsoluteTime	=	0;
}

int SPLoveManager::GetCurPartingTime()
{
	double	iCurAbsoluteTime	=	GetGlobalAbsoluteTime() - m_iAbsoluteTime;

	if( iCurAbsoluteTime < 0 )
		iCurAbsoluteTime = 0;

	return	(int)( m_stLoveInfo.iPartingNum	+	iCurAbsoluteTime );
}

void SPLoveManager::SetLifePartnerGUID( GU_ID eLifePartner )
{
	m_eLifePartner	=	eLifePartner;
}

GU_ID SPLoveManager::GetLifePartnerGUID()
{
	return m_eLifePartner;
}

const char* SPLoveManager::GetLifePartnerString()
{
	return m_stLoveInfo.szPartName;
}

SPPlayer* SPLoveManager::GetLifePartner()
{
	SPPlayer*	pLifePartner	=	NULL;

	pLifePartner	=	(SPPlayer*)g_pGOBManager->Find( GetLifePartnerGUID() );

	const char*	pstrLifePartner	=	GetLifePartnerString();

	if( pLifePartner == NULL )
		pLifePartner	=	g_pGOBManager->FindPlayerByName( pstrLifePartner );

	if( pLifePartner == NULL )
		return NULL;

	char		szFindLifePartner[ LEN_NAME + 1 ];
	pLifePartner->SPGOBPerformMessage( SPGM_GETGOBNAME , (LPARAM)szFindLifePartner );

	if( g_pCheckManager->IsCharNameCompare( pstrLifePartner , szFindLifePartner , false , true ) == false )
		return NULL;

	if( g_pCheckManager->IsCharNameCompare( NULL , pstrLifePartner , true , true ) == true )
		return NULL;

	return pLifePartner;
}

void SPLoveManager::SetCutIn( bool bCutIn )
{
	if( g_pEventManager != NULL )
	{
		if( bCutIn == true )
		{
			g_pEventManager->SetCutIn( true );
			g_pEventManager->SetCutInState( CUT_IN_STATE_LOVE );
		}
		else
		{
			if( g_pEventManager->GetCutInState() == CUT_IN_STATE_LOVE )
			{
				g_pEventManager->OnCutInEnd();

				if( g_pInterfaceManager != NULL )
					g_pInterfaceManager->SetShowDefaultWindow( true );
			}
		}
	}
}

void SPLoveManager::LoadGlobalSysLDT()
{
	for( int i = 0 ; i < LOVE_LDT_REQUIRE_MAX ; ++i )
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

	pLDTFile->GetField( 133 , 0 ,	ldtFieldData );
	m_iRequireLDT[ LOVE_LDT_REQUIRE_FRIENDSHIP_CONSUME_ELY ]		=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 14 , 0 ,	ldtFieldData );
	m_iRequireLDT[ LOVE_LDT_REQUIRE_FRIENDSHIP_CONSUME_ITEM ]		=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 105 , 0 ,	ldtFieldData );
	m_iRequireLDT[ LOVE_LDT_REQUIRE_PROPOSE_COMSUME_ELY ]			=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 18 , 0 ,	ldtFieldData );
	m_iRequireLDT[ LOVE_LDT_REQUIRE_PROPOSE_COMSUME_ITEM ]			=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 19 , 0 ,	ldtFieldData );
	m_iRequireLDT[ LOVE_LDT_REQUIRE_PROPOSE_POSSIBLE_DISTANCE ]		=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 20 , 0 ,	ldtFieldData );
	m_iRequireLDT[ LOVE_LDT_REQUIRE_PROPOSE_POSSIBLE_LOVEPOINT ]	=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 26 , 0 ,	ldtFieldData );
	m_iRequireLDT[ LOVE_LDT_REQUIRE_PARTING_CONSUME_ELY ]			=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 27 , 0 ,	ldtFieldData );
	m_iRequireLDT[ LOVE_LDT_REQUIRE_PARTING_PENALTY_TIME ]			=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 28 , 0 ,	ldtFieldData );
	m_iRequireLDT[ LOVE_LDT_REQUIRE_COMMON_MESSAGEBOX_REMAIN_TIME ]	=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 29 , 0 ,	ldtFieldData );
	m_iRequireLDT[ LOVE_LDT_REQUIRE_WEDDING_POSSIBLE_DISTANCE ]		=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 30 , 0 ,	ldtFieldData );
	m_iRequireLDT[ LOVE_LDT_REQUIRE_WEDDING_SHOUT_REMAIN_TIME ]		=	ldtFieldData.uData.lValue;

	pLDTFile->GetField( 24 , 0 ,	ldtFieldData );
	m_iRequireLDT[ LOVE_LDT_REQUIRE_WEDDING_EQUIP_SET_EFFECT ]		=	ldtFieldData.uData.lValue;

	SAFE_RELEASE( pLDTFile );
}

void SPLoveManager::LoadLDT()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_LOVE_LEVEL , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [ %s ] \n" , RES_FNAME_LDT_LOVE_LEVEL );
#endif
		return;
	}

	int					iItemID , i;
	int					iRecordCount	= pLDTFile->GetItemCount();

	LOVE_LEVEL_DATA		stLoveLevelData;

	LDT_Field			LFD_String;
	long				lFieldNum_String						=	pLDTFile->GetFieldNum( "_String" );
	LDT_Field			Red_Field;
	LDT_Field			Green_Field;
	LDT_Field			Bule_Field;

	UINT				_Red;
	UINT				_Green;
	UINT				_Blue;

	for( i = 0; i < iRecordCount; ++i )
	{
		iItemID	=	pLDTFile->GetPrimaryKey( i );

		pLDTFile->GetField( iItemID , lFieldNum_String				, LFD_String			);

		if ( LFD_String.uData.pValue == NULL || (int)strlen( LFD_String.uData.pValue ) <= 0 )
		{
#ifdef _DEBUG
			DXUTOutputDebugString( "----------------------------------->[%s] File String Zero [%d] \n" , RES_FNAME_LDT_LOVE_LEVEL , iItemID );
#endif
			continue;
		}
		pLDTFile->GetField( iItemID, pLDTFile->GetFieldNum( "_Red" ), Red_Field );
		_Red = Red_Field.uData.lValue;

		pLDTFile->GetField( iItemID, pLDTFile->GetFieldNum( "_Green" ), Green_Field );
		_Green	= Green_Field.uData.lValue;

		pLDTFile->GetField( iItemID, pLDTFile->GetFieldNum( "_Blue" ), Bule_Field );
		_Blue	= Bule_Field.uData.lValue;

		stLoveLevelData.m_iLevel	=	iItemID;
		stLoveLevelData.m_strTitle	=	LFD_String.uData.pValue;

		//m_ColorName[iItemID] = D3DCOLOR_ARGB( 255, _Red, _Green, _Blue );
		m_ColorNameTwo[iItemID] = new D3DXCOLOR( D3DXCOLOR_ARGB2( 255, _Red, _Green, _Blue ) );

		AddLevelData( stLoveLevelData );
	}

	SAFE_RELEASE( pLDTFile );
}

void SPLoveManager::AddLevelData( LOVE_LEVEL_DATA& stLoveLevelData )
{
	LOVE_LEVEL_DATA*	pLoveLevelData	=	GetLevelData( stLoveLevelData.m_iLevel );

	if( pLoveLevelData == NULL )
		m_mLoveLevelData.insert( STD_MAP_LOVE_LEVEL_DATA::value_type( stLoveLevelData.m_iLevel , stLoveLevelData ) );
	else
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->[%s] ID Overlapping [%d] : [%s] [%s] \n" , RES_FNAME_LDT_LOVE_LEVEL , stLoveLevelData.m_iLevel , pLoveLevelData->m_strTitle.c_str() , stLoveLevelData.m_strTitle.c_str() );
#endif
		pLoveLevelData->m_strTitle	=	stLoveLevelData.m_strTitle;
	}
}


bool SPLoveManager::SetWeddingRequest()
{
	//로컬 플레이어가 교제중인 상태인지 체크
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	SPPlayer* pLocalPlayer = g_pGOBManager->GetLocalPlayer();
	
	if(pLocalPlayer->GetStatus()->GetStatusValue(STATUS_LOVE_STATE) == WEDDING_STATUS_PROPOSE) {
		bool bResult = true;

		if(GetLifePartnerString() == NULL)
			return false;

		//교제 대상 플레이어의 존재 여부와 거리 체크
		SPPlayer* pPartnerPlayer = GetLifePartner();
		if(pPartnerPlayer == NULL) {
			if(g_pResourceManager->GetGlobalString(5208145)){
				std::string strMsg = g_pResourceManager->GetGlobalString(5208145);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return false;
		}
		else {
			bool bInterval = true;
			if(pLocalPlayer->GetCurStageID() != pPartnerPlayer->GetCurStageID()) 
				bInterval &= false;
			
			if(pLocalPlayer->GetCurMapGroupID() != pPartnerPlayer->GetCurMapGroupID())
				bInterval &= false;

			float fLocalX = pLocalPlayer->GetPosX();
			float fLocalY = pLocalPlayer->GetPosY();
			float fPcX = pPartnerPlayer->GetPosX();
			float fPcY = pPartnerPlayer->GetPosY();
			int iCheckInterval = GetRequire(LOVE_LDT_REQUIRE_WEDDING_POSSIBLE_DISTANCE);		

			if((fPcX > fLocalX - (iCheckInterval) && fPcX < fLocalX + (iCheckInterval)) && 
				(fPcY > fLocalY - (iCheckInterval) && fPcY < fLocalY + (iCheckInterval)))
			{
				bInterval &= true;
			}
			else {
				bInterval &= false;
			}

			if(!bInterval) {
				if(g_pResourceManager->GetGlobalString(5208145)){
					std::string strMsg = g_pResourceManager->GetGlobalString(5208145);
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
				}
				return false;
			}
		}

		//교제 아이템 에 의한 이팩트 체크
		int iEffectID = GetRequire(LOVE_LDT_REQUIRE_WEDDING_EQUIP_SET_EFFECT);		
		if(iEffectID) {
			bResult &= g_pCheckManager->CheckPCEffect(iEffectID, 0, 0);
		}
		else {
			return false;
		}

		if(bResult == false) {										//결혼식 신청자의 조건이 만족하지 않는 경우
			if(g_pResourceManager->GetGlobalString(5208121)){
				std::string strMsg = g_pResourceManager->GetGlobalString(5208121);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return false;
		}
		else {														//결혼식 요청 패킷
			CONTAINER_SLOT Slot;
			Slot.ContainerType = CONTAINER_TYPE_INVALID;
			Slot.SlotIndex = -1;
			SendWeddingRequest(WEDDING_TYPE_WEDDING, 0, Slot, GetLifePartnerString());
			return true;
		}		
	}
	else if( g_pCheckManager->CheckPCStatus( CHECK_STATUS_LOVE_STATE , WEDDING_STATUS_WEDDING , WEDDING_STATUS_WEDDING ) == true )
	{
		if(g_pResourceManager->GetGlobalString(5208152)){
			std::string strMsg = g_pResourceManager->GetGlobalString(5208152);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
	}
	else
	{
		if(g_pResourceManager->GetGlobalString(5208147)){
			std::string strMsg = g_pResourceManager->GetGlobalString(5208147);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
	}

	return false;
}


bool SPLoveManager::SendWeddingRequest(WEDDING_TYPE iType, int iEly, CONTAINER_SLOT SlotInfo, std::string strName)
{
	if(iType < WEDDING_TYPE_LOVE)
		return false;

	if(strName.empty())
		return false;

	if(strName.length() > LEN_NAME)
		return false;

	char szResponser[LEN_NAME+1];
	ZeroMemory(&szResponser, LEN_NAME+1);
	_tcsncpy(szResponser, strName.c_str(), LEN_NAME);

	//WEDDING_CS_REQUEST,
	// UINT8				iWeddingType;					// 1:교제, 2:프로포즈, 3:결혼
	// char					szRcvCharName[LEN_NAME+1];		// 신청대상자이름
	// int					iEly;							// 소비되야하는 엘리
	// CONTAINER_SLOT		SlotInfo;					// 소비되야하는 아이템

	CPacket Packet;
	Packet.Add((UINT32)WEDDING_CS_REQUEST);
	Packet.AddUINT8(iType);
	Packet.Add(szResponser, LEN_NAME+1);
	Packet.AddUINT32(iEly);
	Packet.AddData(&SlotInfo, sizeof(CONTAINER_SLOT));

	g_pNetworkManager->GetPacketHandler()->PostMsg(&Packet);

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_LOVE , SPIM_LOVE_REFRESH_BUTTON , (WPARAM)1 , NULL );

	return true;
}


bool SPLoveManager::OnWeddingRequest(CPacket* pPacket)
{
	if( pPacket == NULL || g_pGOBManager == NULL )
		return false;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL)
		return false;

	//WEDDING_SC_REQUEST,
	GU_ID					ObjectGUID;						// 게임오브젝트아이디
	UINT8					iWeddingType;					// 1:교제, 2:프로포즈, 3:결혼
	GLOBAL_STRING_ID		ErrorID;						// 에러아이디(0이면 에러없음)
	char					szRcvCharName[LEN_NAME+1];		// 신청대상자이름

	pPacket->ExtractUINT64(&ObjectGUID);
	if(pLocalPlayer->GetGUID() != ObjectGUID) {
		return false;
	}

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_LOVE , SPIM_LOVE_REFRESH_BUTTON , (WPARAM)0 , NULL );

	ZeroMemory(szRcvCharName, LEN_NAME+1);
	
	pPacket->ExtractUINT8(&iWeddingType);
	pPacket->ExtractUINT32(&ErrorID);
	pPacket->ExtractData(szRcvCharName, LEN_NAME+1);

	if( ErrorID ) // 실패
	{
		if( g_pResourceManager != NULL )
		{
			// 에러입니다.
			const char* pstrGlobalString	=	g_pResourceManager->GetGlobalString( ErrorID );

			if( pstrGlobalString != NULL )
			{
				char	szTemp[ _MAX_PATH ];
				sprintf( szTemp , pstrGlobalString , szRcvCharName );
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
			}
		}

		return false;
	}
	
	return true;
}

// SPWIndowLove 인터페이스 처리함 [11/10/2008 AJJIYA]
//bool SPLoveManager::OnWeddingRequestRecv(CPacket* pPacket)
//{
//	if( pPacket == NULL )
//		return false;
//
//	if(g_pGOBManager->GetLocalPlayer() == NULL)
//		return false;
//
//	//WEDDING_SC_REQUEST_RECV,
//	GU_ID				ObjectGUID;						// 게임오브젝트아이디
//	UINT8				iWeddingType;					// 1:교제, 2:프로포즈, 3:결혼
//	char				szCharName[LEN_NAME+1];			// 이름 
//
//	pPacket->ExtractUINT64(&ObjectGUID);
//	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
//		return false;
//	}
//
//	pPacket->ExtractUINT8(&iWeddingType);
//
//	ZeroMemory(szCharName, LEN_NAME+1);
//	pPacket->ExtractData(szCharName, LEN_NAME+1);
//
//	//Notice처리를 한다 
//	
//	return true;
//}

// SPWIndowLove 인터페이스 처리함 [11/10/2008 AJJIYA]
//bool SPLoveManager::SendWeddingResponse(WEDDING_TYPE iType, bool bAccept, std::string strName)
//{
//	if(iType < WEDDING_TYPE_LOVE)
//		return false;
//
//	if(strName.empty())
//		return false;
//
//	if(strName.length() > LEN_NAME)
//		return false;
//
//	char szRequester[LEN_NAME+1];
//	ZeroMemory(&szRequester, LEN_NAME+1);
//	_tcsncpy(szRequester, strName.c_str(), LEN_NAME);
//	
//	//WEDDING_CS_RESPONSE,
//	// UINT8				iWeddingType;					// 1:교제, 2:프로포즈, 3:결혼
//	// char					szActCharName[LEN_NAME+1];		// 요청자이름
//	// UINT8				byAccept;						// 1:수락, 2:거절
//
//	CPacket Packet;
//	Packet.Add((UINT32)WEDDING_CS_RESPONSE);
//	Packet.AddUINT8(iType);	
//	Packet.Add(szRequester, LEN_NAME);
//	Packet.AddUINT8(bAccept);
//
//	g_pNetworkManager->GetPacketHandler()->PostMsg(&Packet);
//	
//	return true;
//}


// SPWIndowLove 인터페이스 처리함 [11/10/2008 AJJIYA]
//bool SPLoveManager::OnWeddingResponse(CPacket* pPacket)
//{
//	if( pPacket == NULL )
//		return false;
//
//	if(g_pGOBManager->GetLocalPlayer() == NULL)
//		return false;
//
//	//WEDDING_SC_RESPONSE,
//	GU_ID					ObjectGUID;						// 게임오브젝트아이디
//	UINT8					iWeddingType;					// 1:교제, 2:프로포즈, 3:결혼
//	GLOBAL_STRING_ID		ErrorID;						// 에러아이디(0이면 에러없음)
//	char					szCharName[LEN_NAME+1];
//	UINT8					byAccept;						// 1:수락, 2:거절
//	WEDDING_INFO			WeddingInfo;					// 웨딩정보
//
//	pPacket->ExtractUINT64(&ObjectGUID);
//	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
//		return false;
//	}
//
//	pPacket->ExtractUINT8(&iWeddingType);
//	
//	pPacket->ExtractUINT32(&ErrorID);
//	if(ErrorID) // 실패
//	{
//		// 에러입니다.
//		if(g_pResourceManager->GetGlobalString(ErrorID)) {
//			std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
//			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
//		}
//		return false;
//	}
//
//	ZeroMemory(szCharName, LEN_NAME+1);
//	pPacket->ExtractData(szCharName, LEN_NAME+1);
//
//	pPacket->ExtractUINT8(&byAccept);
//
//	pPacket->ExtractStruct(&WeddingInfo, sizeof(WEDDING_INFO));
//
//	return true;
//}


bool SPLoveManager::GetWeddingDivorceMsgIndex()
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	WEDDING_PC_STATUS iWeddingStatus = static_cast<WEDDING_PC_STATUS>(g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_LOVE_STATE));

	if(iWeddingStatus < WEDDING_STATUS_LOVE) {
		if(g_pResourceManager->GetGlobalString(4000002)){									//지금은 그일을 할수 없습니다.
				std::string strMsg = g_pResourceManager->GetGlobalString(4000002);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
		return false;
	}

	////이별 관련 아이템 해제 요청
	//int iEffectID = GetRequire(LOVE_LDT_REQUIRE_WEDDING_EQUIP_SET_EFFECT);
	//if(iEffectID) {
	//	if(g_pCheckManager->CheckPCEffect(iEffectID, 0, 0)){
	//		if(g_pResourceManager->GetGlobalString(5208138)){
	//			std::string strMsg = g_pResourceManager->GetGlobalString(5208138);
	//			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
	//		}
	//		return false;
	//	}		
	//}
	SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if(pInven) {
		if(pInven->CheckEquipWeddingItem())	{		//true인 경우 결혼 아이템을 장비하고 있음
			if(g_pResourceManager->GetGlobalString(5208139)){
				std::string strMsg = g_pResourceManager->GetGlobalString(5208139);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return false;
		}
	}
	
	int iFee = GetRequire(LOVE_LDT_REQUIRE_PARTING_CONSUME_ELY);
	int iPenaltyTime = GetRequire(LOVE_LDT_REQUIRE_PARTING_PENALTY_TIME);
	UINT64 iUserEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY);
	int iReturnVal = 0;
	char szMsg[256];
	ZeroMemory(szMsg, 256);	

	if(iUserEly < iFee) {																	//소비될 Ely가 부족한 경우
		if(g_pResourceManager->GetGlobalString(5208131)){
			std::string strMsg = g_pResourceManager->GetGlobalString(5208131);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}

		return false;
	}

	if(iWeddingStatus > WEDDING_STATUS_NONE && iWeddingStatus < WEDDING_STATUS_WEDDING)
	{																								//교제중		
		if(iFee) {									//Ely 소비
			iReturnVal = 5208130;
		}
		else {										//Ely 소비 없음
			iReturnVal = 5208129;
		}
	}
	else if(iWeddingStatus == WEDDING_STATUS_WEDDING) {	//결혼중
		if(iPenaltyTime) {						//기간 패널티 
			if(iFee) {								//Ely 소비
				iReturnVal = 5208140;
			}
			else {									//Ely 소비 없음
				iReturnVal = 5208138;
			}
		}
		else {									//기간 패널티 없음
			if(iFee) {								//Ely 소비
				iReturnVal = 5208130;
			}
			else {									//Ely 소비 없음
				iReturnVal = 5208129;
			}
		}
	}	

	if(iReturnVal && g_pResourceManager->GetGlobalString(iReturnVal)) {
		std::string strMsg = g_pResourceManager->GetGlobalString(iReturnVal);
		sprintf(szMsg, strMsg.c_str(), iFee);
		g_pInterfaceManager->SetNotice(szMsg, static_cast<SPGameObject*>(g_pGOBManager->GetLocalPlayer()));
		g_pCheckManager->SetNoticeUse(NOTICE_USE_LOVE_DIVORCE);
	}

	return true;
}


bool SPLoveManager::SetWeddingDivorce()
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	if(g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_LOVE_STATE) > 0) {	//커플 또는 결혼 상태
		int iFee = SPLoveManager::GetInstance()->GetRequire(LOVE_LDT_REQUIRE_PARTING_CONSUME_ELY);
		UINT64 iUserEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY);

		if(iUserEly < iFee) {																	//소비될 Ely가 부족한 경우
			if(g_pResourceManager->GetGlobalString(5208131)){
				std::string strMsg = g_pResourceManager->GetGlobalString(5208131);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}

			return false;
		}

		//
		SendWeddingDivorce(iFee);
		return true;
	}

	return false;
}


bool SPLoveManager::SendWeddingDivorce(int iEly)
{	
	//WEDDING_CS_PARTING,
	// int			iEly;						// 소모되는엘리
	
	CPacket Packet;
	Packet.Add((UINT32)WEDDING_CS_PARTING);
	Packet.AddUINT32(iEly);	

	g_pNetworkManager->GetPacketHandler()->PostMsg(&Packet);

	return true;
}


bool SPLoveManager::OnWeddingDivorce(CPacket* pPacket)
{
	//WEDDING_SC_PARTING,
	GU_ID				ObjectGUID;					// 사용자 아이디
	GLOBAL_STRING_ID	ErrorID;					// 0 : 성공
	
	if( pPacket == NULL )
		return false;
	
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	pPacket->ExtractUINT64(&ObjectGUID);
	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
		return false;
	}

	pPacket->ExtractUINT32(&ErrorID);
	if(ErrorID) // 실패
	{
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(ErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}
	
	if(g_pResourceManager->GetGlobalString(5208134)) {						//성공시 메시지
		if(GetLifePartnerString() == NULL)
			return false;
		
		std::string strMsg = g_pResourceManager->GetGlobalString(5208134);
		char szMsg[256];
		ZeroMemory(szMsg, 256);
		sprintf(szMsg, strMsg.c_str(), GetLifePartnerString());
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
	}

	return true;
}

void SPLoveManager::OnWeddingCancel( CPacket* pPacket )
{
	if( pPacket == NULL )
		return;

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return;

	SetCutIn( false );

	if( g_pResourceManager != NULL )
	{
		const char*	pszGlobalString	=	g_pResourceManager->GetGlobalString( 5208151 );

		if( pszGlobalString != NULL )
		{
			if( g_pInterfaceManager != NULL )
			{
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pszGlobalString);
			}
		}
	}

	if( g_pInterfaceManager != NULL )
	{
		g_pInterfaceManager->ShowWindow( WIID_LOVEMSG_INPUT , SP_HIDE );
	}
}

void SPLoveManager::OnMotionStartEnd( CPacket* pPacket )
{
	if( pPacket == NULL )
		return;

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return;

	bool				bCutIn		=	true;
	UINT8				iStartEnd; 						// 0:Start(외치기창부터시작) 1:Start(외치기창없이시작) 2:End

	pPacket->ExtractUINT8( &iStartEnd );

	if( iStartEnd == 2 )
		bCutIn	=	false;

	SetCutIn( bCutIn );

	if( iStartEnd == 1 && g_pInterfaceManager != NULL )
		g_pInterfaceManager->ShowWindow( WIID_LOVEMSG_INPUT , false );
}

//
//bool SPLoveManager::CheckRecallEffect(int iEffectID)
//{
//	if(iEffectID == 0)
//		return false;
//
//	if(RECALL_EFFECT_ID != iEffectID)
//		return false;
//
//	if(IsEnableRecall() == false) {
//		return false;
//	}
//
//	if(GetLifePartnerString() == NULL)
//		return false;
//
//	if(g_pResourceManager->GetGlobalString(10020006)) {
//		std::string strMsg = g_pResourceManager->GetGlobalString(10020006);
//		char szMsg[256];
//		ZeroMemory(szMsg, 256);
//		sprintf(szMsg, strMsg.c_str(), GetLifePartnerString());
//		int iWaitTime = GetRequire(LOVE_LDT_REQUIRE_COMMON_MESSAGEBOX_REMAIN_TIME) * 60;
//		g_pInterfaceManager->SetNotice(szMsg, static_cast<SPGameObject*>(g_pGOBManager->GetLocalPlayer()), NOTICE_TYPE_YESNO, iWaitTime);
//		g_pCheckManager->SetNoticeUse(NOTICE_USE_LOVE_SUMMON);
//	}
//
//	return true;
//}


bool SPLoveManager::OnRequestRecall(CPacket* pPacket)
{
	if( pPacket == NULL )
		return false;
	
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;	
	
	//WEDDING_SC_SUMMON_REQUEST,	
	GU_ID			ObjectGUID;					// 게임오브젝트아이디
	
	pPacket->ExtractUINT64(&ObjectGUID);
	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
		return false;
	}

	if(GetLifePartnerString() == NULL)
		return false;

	if(g_pResourceManager->GetGlobalString(10020006)) {
		std::string strMsg = g_pResourceManager->GetGlobalString(10020006);
		char szMsg[256];
		ZeroMemory(szMsg, 256);
		sprintf(szMsg, strMsg.c_str(), GetLifePartnerString());
		int iWaitTime = GetRequire(LOVE_LDT_REQUIRE_COMMON_MESSAGEBOX_REMAIN_TIME);
		g_pInterfaceManager->SetNotice(szMsg, static_cast<SPGameObject*>(g_pGOBManager->GetLocalPlayer()), NOTICE_TYPE_YESNO, iWaitTime);
		g_pCheckManager->SetNoticeUse(NOTICE_USE_LOVE_SUMMON);
	}
	
	return true;
}


bool SPLoveManager::OnRequestRecallErr( CPacket* pPacket )
{
	if( pPacket == NULL )
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;	

	// 소환 시전자에게 에러일때만 보낸다
	//WEDDING_SC_SUMMON_REQUEST_ERR,
	GU_ID			ObjectGUID;					// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)

	pPacket->ExtractUINT64(&ObjectGUID);
	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
		return false;
	}
	
	pPacket->ExtractUINT32(&ErrorID);
	if(ErrorID) // 실패
	{
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(ErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}

	return true;
}


//bool SPLoveManager::IsEnableRecall()
//{
//	if(g_pGOBManager->GetLocalPlayer() == NULL)
//		return false;
//
//	int iStageID = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
//	int iMapGroupID = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();
//
//	SPStage* pStage = g_StageManager.GetCurStage(iStageID);
//	if(pStage) {
//		SPMapGroup* pMapGroup = pStage->GetMapGroup(iMapGroupID);
//		if(pMapGroup && (pMapGroup->IsMapGroupType(GROUP_TYPE_SUMMON) == false)) {
//			if( g_pResourceManager->GetGlobalString(10020003))
//				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(10020003));
//
//			return false; 
//		}
//	}
//
//	if(g_pCheckManager->CheckPlayerState() != PLAYER_STATE_NULL) {
//		if(g_pResourceManager->GetGlobalString(10020008)) {
//			std::string strMsg = g_pResourceManager->GetGlobalString(10020008);
//			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
//		}
//		return false;
//	}
//
//	SPChatManager* pChatManager = SPChatManager::GetInstance();
//	if(pChatManager && pChatManager->IsChatRoom()) {
//		if(g_pResourceManager->GetGlobalString(10020008)) {
//			std::string strMsg = g_pResourceManager->GetGlobalString(10020008);
//			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
//		}
//		return false;
//	}
//
//	if( g_pSubGameManager && g_pSubGameManager->GetGameState() == GS_CASHSHOP ) {
//		if(g_pResourceManager->GetGlobalString(10020008)) {
//			std::string strMsg = g_pResourceManager->GetGlobalString(10020008);
//			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
//		}
//		return false;
//	}
//
//	return true;
//}


bool SPLoveManager::SendAcceptRecall( bool bAccept )
{
	////[2008/12/4] - 자신에게 취소(시간아웃 또는 직접) 메시지 는 안보이도록 변경
	//if(bAccept == false) {
	//	if(g_pResourceManager->GetGlobalString(5208127)) {
	//		std::string strMsg = g_pResourceManager->GetGlobalString(5208127);
	//		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
	//	}		
	//}

	//WEDDING_CS_SUMMON_RESPONSE,
	// UINT8			byAccept;

	CPacket Packet;
	Packet.Add((UINT32)WEDDING_CS_SUMMON_RESPONSE);
	Packet.AddUINT8(bAccept);
	g_pNetworkManager->GetPacketHandler()->PostMsg(&Packet);
	
	return true;
}


bool SPLoveManager::OnAcceptRecall( CPacket* pPacket )
{
	if( pPacket == NULL )
		return false;
	
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;	
	
	//WEDDING_SC_SUMMON_RESPONSE,
	GU_ID			ObjectGUID;					// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)
	
	pPacket->ExtractUINT64(&ObjectGUID);
	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
		return false;
	}

	pPacket->ExtractUINT32(&ErrorID);
	if(ErrorID) // 실패
	{
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(ErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}

	return true;
}


bool SPLoveManager::OnResultRecall( CPacket* pPacket )
{
	if( pPacket == NULL )
		return false;
	
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;	
	
	//WEDDING_SC_SUMMON_RESPONSE_RECV,
	GU_ID			ObjectGUID;					// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)
	UINT8			byAccept;
	
	pPacket->ExtractUINT64(&ObjectGUID);
	if(g_pGOBManager->GetLocalPlayer()->GetGUID() != ObjectGUID) {
		return false;
	}

	pPacket->ExtractUINT32(&ErrorID);
	if(ErrorID) // 실패
	{
		// 에러입니다.
		if(g_pResourceManager->GetGlobalString(ErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		//return false;
	}

	pPacket->ExtractUINT8(&byAccept);			//소환 대상자가 소환을 거절한 경우
	if(byAccept == 0) {
		if(g_pResourceManager->GetGlobalString(5208127)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(5208127);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
	}
	
	return true;
}

D3DXCOLOR SPLoveManager::GetD3DColor( int ColorIndex )
{
	return m_ColorName[ColorIndex];
}

D3DXCOLOR*	SPLoveManager::GetD3DColor2( int ColorIndex )
{
	if ( m_ColorNameTwo[ColorIndex] )
	{
		return m_ColorNameTwo[ColorIndex];
	}
	return NULL;
}

void	SPLoveManager::CleanColor2()
{
	int Index = ColorNameNum;
	do {
		SAFE_DELETE( m_ColorNameTwo[Index - 1] );
		Index--;		
	} while( ( Index - 1 ) == 0 );
}
