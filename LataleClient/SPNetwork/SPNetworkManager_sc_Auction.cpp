
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
#include "SPGuildArchive.h"
#include "SPSubGameManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"
#include "SPAuctionManager.h"

#include "_Error_log.h"

extern SPNetworkManager* g_pNetworkManager;

#define ERROR_AUCTION_SERVER_DOWN	52000075			// 경매 서버 Down

// AUCTION_SC_OPEN,
// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
DEFINE_SC_PACKET_PROC(AUCTION_SC_OPEN)
{
	g_pkAuctionManager->OnRecvAuctioinOpen() ;
}


// AUCTION_SC_CREATE,
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
DEFINE_SC_PACKET_PROC(AUCTION_SC_CREATE)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	SPWindow*			pAuctionWindow ;
	SPWindow*			pAuctionMsg;
	CHARACTER_CASH_INFO ctCashInfo ;
	packet->ExtractUINT64(&ObjectGUID);	
	packet->ExtractUINT32(&ErrorID);

	pAuctionWindow	= g_pInterfaceManager->GetAuctionWindow() ;
	pAuctionMsg		= g_pInterfaceManager->GetAuctionMsgWindow() ;
	GU_ID LocalGUID = g_pGOBManager->GetLocalPlayer()->GetGUID() ;
	if( ObjectGUID != LocalGUID )	{
		ErrorFLogs->Error_Log("AUCTION_SC_CREATE ObjectGUID ERROR[%I64d]-[%I64d]", ObjectGUID, LocalGUID) ;
		// Progress Close
		if( pAuctionMsg )
			pAuctionMsg->SPSendMessage(SPIM_AUCTION_WAIT_CLOSE, ErrorID, 0) ;
		return ;
	}

	// Error인 경우 ErrorID 뒤에 CHARACTER_CASH_INFO를 넣어서 캐쉬정보 업데이트
	if( ErrorID != 0 )	{
		ErrorFLogs->Error_Log("AUCTION_SC_CREATE ERROR [%d]", ErrorID) ;
		if( ErrorID == ERROR_AUCTION_SERVER_DOWN )	{
			g_pkAuctionManager->SetServerOpen(false) ;
			if( pAuctionMsg )
				pAuctionMsg->SPSendMessage(SPIM_AUCTION_SERVER_CLOSE, ErrorID, 0) ;
		}
		else if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
		// Player CASH_INFO 업데이트
		packet->ExtractStruct(&ctCashInfo, sizeof(CHARACTER_CASH_INFO));
		g_pkAuctionManager->UpdatePlayerCash(ctCashInfo.iCash, ctCashInfo.iMile) ;
		if( pAuctionWindow )
			pAuctionWindow->SPSendMessage(SPIM_AUCTION_MONEY_UPDATE, (WPARAM)0, (LPARAM)0);
	}
	else	{
		g_pkAuctionManager->OnRecvAuctionCreate((void*)packet) ;	
	}

	// Progress Close
	if( pAuctionMsg )
		pAuctionMsg->SPSendMessage(SPIM_AUCTION_WAIT_CLOSE, ErrorID, 0) ;
}



// AUCTION_CS_CANCEL
// GU_ID			iObjectGUID;						// 게임오브젝트아이디
// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
// iErrorID == 0
// __int64			iAuctionID;							// 경매키
// CHARACTER_CASH_INFO stCash;							// 
// iErrorID != 0
// CHARACTER_CASH_INFO stCash;							//							// 
DEFINE_SC_PACKET_PROC(AUCTION_SC_CANCEL)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	CHARACTER_CASH_INFO ctCashInfo ;
	SPWindow* pAuctionWindow ;
	SPWindow* pAuctionMsg ;
	packet->ExtractUINT64(&ObjectGUID);	
	packet->ExtractUINT32(&ErrorID);

	pAuctionWindow = g_pInterfaceManager->GetAuctionWindow() ;
	pAuctionMsg	   = g_pInterfaceManager->GetAuctionMsgWindow() ;
	// Error인 경우 ErrorID 뒤에 CHARACTER_CASH_INFO를 넣어서 캐쉬정보 업데이트
	if( ErrorID != 0 )	{
		ErrorFLogs->Error_Log("AUCTION_SC_CANCEL ERROR [%d]", ErrorID) ;

		// Player CASH_INFO 업데이트
		packet->ExtractStruct(&ctCashInfo, sizeof(CHARACTER_CASH_INFO));
		g_pkAuctionManager->UpdatePlayerCash(ctCashInfo.iCash, ctCashInfo.iMile) ;
		if( pAuctionWindow )
			pAuctionWindow->SPSendMessage(SPIM_AUCTION_MONEY_UPDATE, (WPARAM)0, (LPARAM)0);
		
		// Server Down 처리 
		if( ErrorID == ERROR_AUCTION_SERVER_DOWN )	{
			g_pkAuctionManager->SetServerOpen(false) ;
			if( pAuctionMsg )
				pAuctionMsg->SPSendMessage(SPIM_AUCTION_SERVER_CLOSE, ErrorID, 0) ;
		}
		else if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
	else	{
		g_pkAuctionManager->OnRecvAuctionCancel((void*)packet) ;	
	}

	// Progress Close
	if( pAuctionMsg )
		pAuctionMsg->SPSendMessage(SPIM_AUCTION_WAIT_CLOSE, ErrorID, 0) ;
}


// AUCTION_SC_FINISH, (판매자, 이전입찰자, 전전입찰자 받음 -> 해당 경매 삭제)
// GU_ID			iObjectGUID;						// 게임오브젝트아이디
// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
// iErrorID == 0
// __int64			iAuctionID;							// 경매키
// CHARACTER_CASH_INFO stCash;							// 
// iErrorID != 0
// CHARACTER_CASH_INFO stCash;							//							// 
DEFINE_SC_PACKET_PROC(AUCTION_SC_FINISH)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	CHARACTER_CASH_INFO ctCashInfo ;
	SPWindow* pAuctionWindow ;
	SPWindow* pAuctionMsg ;
	packet->ExtractUINT64(&ObjectGUID);	
	packet->ExtractUINT32(&ErrorID);

	pAuctionWindow = g_pInterfaceManager->GetAuctionWindow() ;
	pAuctionMsg	   = g_pInterfaceManager->GetAuctionMsgWindow() ;
	// Error인 경우 ErrorID 뒤에 CHARACTER_CASH_INFO를 넣어서 캐쉬정보 업데이트
	if( ErrorID != 0 )	{
		ErrorFLogs->Error_Log("AUCTION_SC_FINISH ERROR [%d]", ErrorID) ;

		// Player CASH_INFO 업데이트
		packet->ExtractStruct(&ctCashInfo, sizeof(CHARACTER_CASH_INFO));
		g_pkAuctionManager->UpdatePlayerCash(ctCashInfo.iCash, ctCashInfo.iMile) ;
		if( pAuctionWindow )
			pAuctionWindow->SPSendMessage(SPIM_AUCTION_MONEY_UPDATE, (WPARAM)0, (LPARAM)0);
		
		// Server Down 처리 
		if( ErrorID == ERROR_AUCTION_SERVER_DOWN )	{
			g_pkAuctionManager->SetServerOpen(false) ;
			if( pAuctionMsg )
				pAuctionMsg->SPSendMessage(SPIM_AUCTION_SERVER_CLOSE, ErrorID, 0) ;
		}
		else if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
	else	{
		g_pkAuctionManager->OnRecvAuctionFinish((void*)packet) ;	
	}
}



// 낙찰완료
//		- 낙찰시 판매자/낙찰자에게
//		- 즉구시 판매자에게
// AUCTION_SC_FINISH_SELL,
// GU_ID			iObjectID;
// __int64			iAuctionID;							// 경매키
// CHARID			iBuyerID;							// 구매자
// CHARACTER_CASH_INFO stCash;							// 
DEFINE_SC_PACKET_PROC(AUCTION_SC_FINISH_SELL)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디

	packet->ExtractUINT64(&ObjectGUID);	
	g_pkAuctionManager->OnRecvAuctionFinishSell((void*)packet) ;	
}



// AUCTION_SC_BUYOUT,
//** Item Blocking 및 해제가 필요함
// GU_ID			iObjectGUID;						// 게임오브젝트아이디
// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
// iErrorID == 0
// __int64			iAuctionID;							// 경매키
// __int64			iCurrMoney;							// 경매키
// CHARACTER_CASH_INFO stCash;							// 
// iErrorID != 0
// CHARACTER_CASH_INFO stCash;							//
DEFINE_SC_PACKET_PROC(AUCTION_SC_BUYOUT)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	CHARACTER_CASH_INFO ctCashInfo ;
	SPWindow* pAuctionWindow ;
	SPWindow* pAuctionMsg ;
	packet->ExtractUINT64(&ObjectGUID);	
	packet->ExtractUINT32(&ErrorID);

	pAuctionWindow = g_pInterfaceManager->GetAuctionWindow() ;
	pAuctionMsg	   = g_pInterfaceManager->GetAuctionMsgWindow() ;
	// Progress Close
	if( pAuctionMsg )
		pAuctionMsg->SPSendMessage(SPIM_AUCTION_WAIT_CLOSE, 0, 0) ;
	// Error인 경우 ErrorID 뒤에 CHARACTER_CASH_INFO를 넣어서 캐쉬정보 업데이트
	if( ErrorID != 0 )	{
		ErrorFLogs->Error_Log("AUCTION_SC_BUYOUT ERROR [%d]", ErrorID) ;

		// Player CASH_INFO 업데이트
		packet->ExtractStruct(&ctCashInfo, sizeof(CHARACTER_CASH_INFO));
		g_pkAuctionManager->UpdatePlayerCash(ctCashInfo.iCash, ctCashInfo.iMile) ;
		if( pAuctionWindow )
			pAuctionWindow->SPSendMessage(SPIM_AUCTION_MONEY_UPDATE, (WPARAM)0, (LPARAM)0);
		
		// Server Down 처리 
		if( ErrorID == ERROR_AUCTION_SERVER_DOWN )	{
			g_pkAuctionManager->SetServerOpen(false) ;
			if( pAuctionMsg )
				pAuctionMsg->SPSendMessage(SPIM_AUCTION_SERVER_CLOSE, ErrorID, 0) ;
		}
		// 52000073(입찰가 낮음), 52000074(없는페이지), 52000009(없는아이템) => 재검색
		else if( ErrorID == 52000073 || ErrorID == 52000074 || ErrorID == 52000009)	{
			//pAuctionWindow->SPSendMessage(SPIM_AUCTION_ERROR_NOTICE, AUCTION_MSG_REFRESH, (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
			pAuctionMsg->SPSendMessage(SPIM_AUCTION_ERROR_NOTICE, 2, (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
		}
		else if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
	else	{
		g_pkAuctionManager->OnRecvAuctionBuyOut((void*)packet) ;	
	}
}


// AUCTION_SC_BID,	
//	>> 현재 입찰자	: 입찰 정보 추가
//	>> 판매자		: 판매 정보 갱신
//	>> 이전입찰자	: 입찰 정보 갱신
// GU_ID			iObjectGUID;						// 게임오브젝트아이디
// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
// AUCTION_HEADER	stAuctionHeader;					// 기본정보
// AUCTION_GOODS_XX stAuctionGoods;						// 올린 물건 정보( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)
// __int64			iCurrEly;							// 현재남은돈 (등록수수료 or 올리 돈을 제하고 현재 가지고있는돈)
// CHARACTER_CASH_INFO stCash;							// 현재캐쉬정보							
DEFINE_SC_PACKET_PROC(AUCTION_SC_BID)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	CHARACTER_CASH_INFO ctCashInfo ;
	SPWindow* pAuctionWindow ;
	SPWindow* pAuctionMsg ;
	packet->ExtractUINT64(&ObjectGUID);	
	packet->ExtractUINT32(&ErrorID);

	pAuctionWindow = g_pInterfaceManager->GetAuctionWindow() ;
	pAuctionMsg	   = g_pInterfaceManager->GetAuctionMsgWindow() ;
	// Error인 경우 ErrorID 뒤에 CHARACTER_CASH_INFO를 넣어서 캐쉬정보 업데이트
	if( ErrorID != 0 )	{
		ErrorFLogs->Error_Log("AUCTION_SC_BID ERROR [%d]", ErrorID) ;
		// Progress Close
		if( pAuctionMsg )
			pAuctionMsg->SPSendMessage(SPIM_AUCTION_WAIT_CLOSE, ErrorID, 0) ;

		// Server Down 처리 
		if( ErrorID == ERROR_AUCTION_SERVER_DOWN )	{
			g_pkAuctionManager->SetServerOpen(false) ;
			if( pAuctionMsg )
				pAuctionMsg->SPSendMessage(SPIM_AUCTION_SERVER_CLOSE, ErrorID, 0) ;
		}
		// 52000073(입찰가 낮음), 52000074(없는페이지), 52000009(없는아이템) => 재검색
		else if( ErrorID == 52000073 || ErrorID == 52000074 || ErrorID == 52000009)	{
			//pAuctionWindow->SPSendMessage(SPIM_AUCTION_ERROR_NOTICE, AUCTION_MSG_REFRESH, (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
			pAuctionMsg->SPSendMessage(SPIM_AUCTION_ERROR_NOTICE, 2, (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
		}
		else if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );

		// Player CASH_INFO 업데이트
		packet->ExtractStruct(&ctCashInfo, sizeof(CHARACTER_CASH_INFO));
		g_pkAuctionManager->UpdatePlayerCash(ctCashInfo.iCash, ctCashInfo.iMile) ;
		if( pAuctionWindow )
			pAuctionWindow->SPSendMessage(SPIM_AUCTION_MONEY_UPDATE, (WPARAM)0, (LPARAM)0);

	}
	else	{
		g_pkAuctionManager->OnRecvAuctionBidUpdate((void*)packet) ;	
	}
}


// AUCTION_SC_SEARCH_INFO,
// GU_ID			iObjectGUID;						// 게임오브젝트아이디
// int				iNumOfTotalCount;					// 총갯수
// int				iStartNumber;						// 현재전송처음값이 총갯수중 몇번째값에 해당하는지
// UINT8			iNumOfAuction;						// 전송될옥션정보갯수
	// AUCTION_HEADER	stAuctionHeader;					// 기본정보
	// AUCTION_GOODS_XX stAuctionGoods;						// 올린 물건 정보( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)
DEFINE_SC_PACKET_PROC(AUCTION_SC_SEARCH_INFO)
{
	GU_ID		ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	SPWindow*	pAuctionMsg ;
	packet->ExtractUINT64(&ObjectGUID);	
	packet->ExtractUINT32(&ErrorID);

	// Progress Close
	pAuctionMsg = g_pInterfaceManager->GetAuctionMsgWindow() ;
	if( pAuctionMsg )
		pAuctionMsg->SPSendMessage(SPIM_AUCTION_WAIT_CLOSE, 0, 0) ;

	if( ErrorID != 0 )	{
		ErrorFLogs->Error_Log("AUCTION_SC_SEARCH_INFO ERROR [%d]", ErrorID) ;

		// Server Down 처리 
		if( ErrorID == ERROR_AUCTION_SERVER_DOWN )	{
			g_pkAuctionManager->SetServerOpen(false) ;
			if( pAuctionMsg )
				pAuctionMsg->SPSendMessage(SPIM_AUCTION_SERVER_CLOSE, ErrorID, 0) ;
		}
		else if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
	else {
		g_pkAuctionManager->OnRecvAuctionSearchList((void*)packet) ;
	}
}


// AUCTION_SC_INFO_4BIDDER,
// GU_ID			iObjectGUID;						// 게임오브젝트아이디
// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
// Error Process
DEFINE_SC_PACKET_PROC(AUCTION_SC_INFO_4BIDDER)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	SPWindow* pAuctionMsg ;
	packet->ExtractUINT64(&ObjectGUID);	
	packet->ExtractUINT32(&ErrorID);

	pAuctionMsg = g_pInterfaceManager->GetAuctionMsgWindow() ;
	if( ErrorID != 0 )	{
		ErrorFLogs->Error_Log("AUCTION_SC_CREATE ERROR [%d]", ErrorID) ;

		// Server Down 처리 
		if( ErrorID == ERROR_AUCTION_SERVER_DOWN )	{
			g_pkAuctionManager->SetServerOpen(false) ;
			if( pAuctionMsg )
				pAuctionMsg->SPSendMessage(SPIM_AUCTION_SERVER_CLOSE, ErrorID, 0) ;
		}
		else if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
	else	{
		// 정상목록을 모두 받은경우 Default Sort를 해준다.
		g_pkAuctionManager->SetDefaultSort(0) ;
	}
}


// AUCTION_SC_BIDLIST,
// GU_ID			iObjectGUID;						// 게임오브젝트아이디
// UINT8			iNumOfAuction;						// 전송될옥션정보갯수
	// AUCTION_HEADER	stAuctionHeader;					// 기본정보
	// AUCTION_GOODS_XX stAuctionGoods;						// 올린 물건 정보( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)
DEFINE_SC_PACKET_PROC(AUCTION_SC_BIDLIST)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	packet->ExtractUINT64(&ObjectGUID);	

	g_pkAuctionManager->OnRecvAuctionBuyList((void*)packet) ;
}


// AUCTION_SC_BIDDEL,	(전전 입찰자)
// GU_ID			iObjectGUID;						// 게임오브젝트아이디
// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
// __int64			iAuctionID;							// 경매키
DEFINE_SC_PACKET_PROC(AUCTION_SC_BIDDEL)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	SPWindow* pAuctionMsg ;
	packet->ExtractUINT64(&ObjectGUID);	
	packet->ExtractUINT32(&ErrorID);

	pAuctionMsg = g_pInterfaceManager->GetAuctionMsgWindow() ;
	if( ErrorID != 0 )	{
		ErrorFLogs->Error_Log("AUCTION_SC_BIDDEL ERROR [%d]", ErrorID) ;

		// Server Down 처리 
		if( ErrorID == ERROR_AUCTION_SERVER_DOWN )	{
			g_pkAuctionManager->SetServerOpen(false) ;
			if( pAuctionMsg )
				pAuctionMsg->SPSendMessage(SPIM_AUCTION_SERVER_CLOSE, ErrorID, 0) ;
		}
		else if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
	else	{
		g_pkAuctionManager->OnRecvAuctionBidDelete((void*)packet) ;
	}
}



// AUCTION_SC_INFO_4SELLER,
// GU_ID			iObjectGUID;						// 게임오브젝트아이디
// GLOBAL_STRING_ID	iErrorID;							// 에러아이디
// Error Process
DEFINE_SC_PACKET_PROC(AUCTION_SC_INFO_4SELLER)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	SPWindow* pAuctionMsg ;
	packet->ExtractUINT64(&ObjectGUID);	
	packet->ExtractUINT32(&ErrorID);

	pAuctionMsg = g_pInterfaceManager->GetAuctionMsgWindow() ;
	if( ErrorID != 0 )	{
		ErrorFLogs->Error_Log("AUCTION_SC_INFO_4SELLER ERROR [%d]", ErrorID) ;

		// Server Down 처리 
		if( ErrorID == ERROR_AUCTION_SERVER_DOWN )	{
			g_pkAuctionManager->SetServerOpen(false) ;
			if( pAuctionMsg )
				pAuctionMsg->SPSendMessage(SPIM_AUCTION_SERVER_CLOSE, ErrorID, 0) ;
		}
		else if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
	}
	else	{
		// 정상목록을 모두 받은경우 Default Sort를 해준다.
		g_pkAuctionManager->SetDefaultSort(1) ;
	}
}



// AUCTION_SC_SELLLIST,
// GU_ID			iObjectGUID;						// 게임오브젝트아이디
// UINT8			iNumOfAuction;						// 전송될옥션정보갯수
	// AUCTION_HEADER	stAuctionHeader;					// 기본정보
	// AUCTION_GOODS_XX stAuctionGoods;						// 올린 물건 정보( AUCTION_GOODS_ITEM or AUCTION_GOODS_POINT)
DEFINE_SC_PACKET_PROC(AUCTION_SC_SELLLIST)
{
	GU_ID				ObjectGUID;							// 게임오브젝트아이디
	packet->ExtractUINT64(&ObjectGUID);	

	g_pkAuctionManager->OnRecvAuctionSaleList((void*)packet) ;
}



