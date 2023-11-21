// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPAuctionMock.cpp
//	created:	2008/2/26   Time:10:59
//	purpose:	
//*********************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "SPCommon.h"
#include "SPUtil.h"
#include "PacketID.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"

#include "SPCommandConvert.h"
#include "SPGOBStatus.h"
#include "SPGOBManager.h"
#include "SPGOBCoordPhysics.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPAuctionDef.h"
#include "SPAuctionManager.h"
#include "SPAuctionMock.h"

#include "_Error_log.h"



SPAuctionMock::SPAuctionMock()
{

}

SPAuctionMock::~SPAuctionMock()
{
	Clear() ;
}


void SPAuctionMock::Init()
{
	m_iAuctionID = 100000 ;
}


void SPAuctionMock::Clear()
{
}

/*
	typedef struct _AUCTION_HEADER
	{
	__int64 iAuctionID;				// 경매키
	UINT8	iType;					// 경매타입
	UINT8	iDuration;				// AUCTION_DURATION (판매자가 세팅한)
	TCHAR	szName[LEN_NAME+1];		// 판매자 or 최고입찰자(등록리스트)
	UINT32	iRemainTime;			// 현재남은시간

	__int64 iBidPrice;				// 시작가 혹은 상회입찰가
	__int64 iBuyOutPrice;			// 즉구가
	__int64 iCurrPrice;				// 현재가
	USERID  iCurrUserID;			// 현재입찰자계정키
	} AUCTION_HEADER, *PAUCTION_HEADER;

	// 경매거래타입별추가데이타
	typedef struct _AUCTION_GOODS_ITEM : public _AUCTION_HEADER
	{
	ITEM stAuctionItem;
	} AUCTION_GOODS_ITEM, *PAUCTION_GOODS_ITEM;

	typedef struct _AUCTION_GOODS_POINT : public _AUCTION_HEADER
	{
	int SellPoint; // Ely, Cash, Mileage
	int FeePoint;  // 수수료
	} AUCTION_GOODS_POINT, *PAUCTION_GOODS_POINT;
*/

/**
	AUCTION_SC_SEARCH_INFO,
	// GU_ID			iObjectGUID;						// 게임오브젝트아이디
	// int				iNumOfTotalCount;					// 총갯수
	// int				iStartNumber;						// 현재전송처음값이 총갯수중 몇번째값에 해당하는지
	// UINT8			iNumOfAuction;						// 전송될옥션정보갯수
*/
void SPAuctionMock::SendAuctionSearchList(int iStart, int iSize)
{
	AUCTION_HEADER		ctSendHeader;
	AUCTION_GOODS_ITEM	ctSendData ;
	CPacket packet;
	int k ;
	int iTotalCnt = 100 ;
	k = MakeRandomValue(0, 15) ;

	packet.Clean() ;
	packet.AddUINT32( iTotalCnt ) ;
	packet.AddUINT32( iStart ) ;
	packet.AddUINT8( iSize );
	for( int i = 0; i < iSize; i++ )
	{
		MakeAuctionListHeader(ctSendHeader) ;
		if( false == MakeAuctionListItem(ctSendData.stAuctionItem, k++) )
		{
			ErrorFLogs->Error_Log("MakeAuctionListItem Failed!!! [%d,%d]", i, iSize) ;
			return ;
		}
		packet.AddData( &ctSendHeader, sizeof(AUCTION_HEADER));
		packet.AddData( &ctSendData, sizeof(AUCTION_GOODS_ITEM));
	}
	g_pkAuctionManager->OnRecvAuctionSearchList(&packet) ;
}


void SPAuctionMock::SendAuctionBuyList(int iSize)
{
	AUCTION_HEADER		ctSendHeader;
	AUCTION_GOODS_ITEM	ctSendData ;
	CPacket packet;
	int k ;
	k = MakeRandomValue(0, 15) ;

	packet.Clean() ;
	packet.AddUINT8( (UINT8)iSize );
	for( int i = 0; i < iSize; i++ )
	{
		MakeAuctionListHeader(ctSendHeader) ;
		ctSendHeader.iCurrUserID = g_pNetworkManager->GetUserID() ;		// 내가 입찰
		strcpy( ctSendHeader.szBidder, g_pNetworkManager->GetCharName().c_str()) ;

		if( false == MakeAuctionListItem(ctSendData.stAuctionItem, k++) )
		{
			ErrorFLogs->Error_Log("MakeAuctionListItem Failed!!! [%d,%d]", i, iSize) ;
			return ;
		}
		packet.AddData( &ctSendHeader, sizeof(AUCTION_HEADER));
		packet.AddData( &ctSendData, sizeof(AUCTION_GOODS_ITEM));
	}
	g_pkAuctionManager->OnRecvAuctionBuyList(&packet) ;
}


void SPAuctionMock::SendAuctionSaleList(int iSize)
{
	AUCTION_HEADER		ctSendHeader;
	AUCTION_GOODS_ITEM	ctSendData ;
	CPacket packet;
	int k ;
	k = MakeRandomValue(0, 15) ;

	packet.Clean() ;
	packet.AddUINT8( (UINT8)iSize );
	for( int i = 0; i < iSize; i++ )
	{
		MakeAuctionListHeader(ctSendHeader) ;
		ctSendHeader.iSellerID = g_pNetworkManager->GetUserID() ;		// 내가 판매
		strcpy( ctSendHeader.szSeller, g_pNetworkManager->GetCharName().c_str()) ;

		if( false == MakeAuctionListItem(ctSendData.stAuctionItem, k++) )
		{
			ErrorFLogs->Error_Log("MakeAuctionListItem Failed!!! [%d,%d]", i, iSize) ;
			return ;
		}
		packet.AddData( &ctSendHeader, sizeof(AUCTION_HEADER));
		packet.AddData( &ctSendData, sizeof(AUCTION_GOODS_ITEM));
	}
	g_pkAuctionManager->OnRecvAuctionSaleList(&packet) ;
}



/**
	아이템 등록정보를 받아서 결과를 알려주는 패킷
 */
void SPAuctionMock::ProcessAuctionCreate(CPacket* pRecvPacket)
{
	UINT8	iType;				//거래타입(AUCTION_TYPE)
	UINT8	iDuration;			//기간타입(AUCTION_DURATION )
	UINT64	iStartingPrice;		//시작가
	UINT64	iBuyOutPrice;		//즉구가
	UINT64	iEnrolPrice;		//등록수수료
	TRASH_ITEM	stUseItem;		//필요아이템위치및 갯수(1)
	ITEMNO	iUseItemNo;			//필요아이템키
	CONTAINER_SLOT		stUpItemPos;
	ITEMNO	iUpItemNo;
	AUCTION_GOODS_POINT stUpPoint;

	pRecvPacket->ExtractUINT8((UINT8*)&iType);
	pRecvPacket->ExtractUINT8((UINT8*)&iDuration);
	pRecvPacket->ExtractUINT64((UINT64*)&iStartingPrice);
	pRecvPacket->ExtractUINT64((UINT64*)&iBuyOutPrice);
	pRecvPacket->ExtractUINT64((UINT64*)&iEnrolPrice);
	pRecvPacket->ExtractStruct(&stUseItem, sizeof(TRASH_ITEM));
	pRecvPacket->ExtractUINT64((UINT64*)&iUseItemNo);
	ErrorFLogs->Error_Log("AuctionCreate Type[%d], 기간[%d], 시작가[%d], 즉구가[%d], 등록비[%d], 등록아이템[%d]", 
							iType, iDuration, iStartingPrice, iBuyOutPrice, iEnrolPrice, iUseItemNo) ;
	if( iType == AUCTION_TYPE_ITEM_BY_ELY )	{
		pRecvPacket->ExtractStruct(&stUpItemPos, sizeof(CONTAINER_SLOT));
		pRecvPacket->ExtractUINT64((UINT64*)&iUpItemNo);
		ErrorFLogs->Error_Log("AuctionCreate 등록아이템 Slot[%d] Index[%d] ITEMNO[%d]", 
			stUpItemPos.ContainerType, stUpItemPos.SlotIndex, iUpItemNo) ;
	}
	else	{
		pRecvPacket->ExtractStruct(&stUpPoint, sizeof(AUCTION_GOODS_POINT));
		ErrorFLogs->Error_Log("AuctionCreate 등록금액 [%d] 수수료[%d]", stUpPoint.SellPoint, stUpPoint.FeePoint) ;
	}

	// Make Return packet

	CPacket SendPacket;
	AUCTION_HEADER		ctSendHeader;
	AUCTION_GOODS_ITEM	ctSendData ;
	CHARACTER_CASH_INFO ctCash ;
	TRASH_ITEM			stUpItem;		
	INT64 iEly = 0 ;
	PACK_OPT*			pOpt = NULL ;

	ctSendHeader.iAuctionID = ++m_iAuctionID ;
	ctSendHeader.iType = iType ;
	ctSendHeader.iDuration = iDuration ;
	ctSendHeader.iCurrPrice = iStartingPrice ;
	ctSendHeader.iBidPrice = iStartingPrice + 100 ;
	ctSendHeader.iBuyOutPrice = iBuyOutPrice ;
	ctSendHeader.iCurrUserID = 0 ;
	memset(ctSendHeader.szBidder, 0, LEN_NAME+1) ;
	ctSendHeader.iSellerID = g_pNetworkManager->GetUserID() ;
	strcpy( ctSendHeader.szSeller, g_pNetworkManager->GetCharName().c_str() ) ;

	if( iType == AUCTION_TYPE_ITEM_BY_ELY )	{
		SPPlayerInvenArchive* pInven = NULL;
		if(g_pGOBManager->GetLocalPlayer())
			pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		else 
			return ;
		SPItem* pItem = pInven->GetInventoryItem(stUpItemPos.ContainerType, stUpItemPos.SlotIndex);	
		if( pItem == NULL )
			return ;

		ctSendData.stAuctionItem.iBackPrice = pItem->GetItemStatus()->GetItemBackPrice() ;
		ctSendData.stAuctionItem.iItemID = pItem->GetItemStatus()->GetItemID() ;
		ctSendData.stAuctionItem.iItemExp = pItem->GetItemStatus()->GetExp() ;
		ctSendData.stAuctionItem.iItemNo = *(ITEMNO*)pItem->GetItemStatus()->SPSendMessage(MV_ITEM_GET_ITEMNO);
		ctSendData.stAuctionItem.iItemSP = pItem->GetItemStatus()->GetSp();
		ctSendData.stAuctionItem.iLifeTime = pItem->GetItemStatus()->GetLifeTime();
		ctSendData.stAuctionItem.iOptPoint = (short)pItem->GetItemStatus()->SPSendMessage(MV_ITEM_GET_OPTPOINT);
		ctSendData.stAuctionItem.iRare = pItem->GetItemStatus()->GetRareLevel();
		ctSendData.stAuctionItem.iStackCount = pItem->GetItemStatus()->GetStackCount();
		
		for( int i = 0 ; i < MAX_ITEM_OPT ; ++i )
		{
			pOpt =  pItem->GetItemStatus()->GetOption(i);
			ctSendData.stAuctionItem.Opt[i].iID = pOpt->iID ;
			ctSendData.stAuctionItem.Opt[i].iValue = pOpt->iValue ;
		}
	}
	else	{
		ctCash.iCash = 10000000 ;
		ctCash.iMile = 10000000 ;
	}


	// 올린아이템
	stUpItem.iContainerSlot.ContainerType = stUpItemPos.ContainerType ;
	stUpItem.iContainerSlot.SlotIndex = stUpItemPos.SlotIndex ;
	stUpItem.iCount = 1 ;

	SendPacket.AddData( &ctSendHeader, sizeof(AUCTION_HEADER));
	SendPacket.AddData( &ctSendData, sizeof(AUCTION_GOODS_ITEM));
	SendPacket.AddUINT64( iEly );
	SendPacket.AddData( &stUseItem, sizeof(TRASH_ITEM));
	if( iType == AUCTION_TYPE_ITEM_BY_ELY )
		SendPacket.AddData( &stUseItem, sizeof(TRASH_ITEM));
	else
		SendPacket.AddData( &ctCash, sizeof(CHARACTER_CASH_INFO));


	g_pkAuctionManager->OnRecvAuctionCreate((void*)&SendPacket) ;
}


/**
	입찰 패킷
 */
void SPAuctionMock::ProcessAuctionBid(CPacket* pRecvPacket)
{
	return ;
}

/**
	즉구 패킷
 */
void SPAuctionMock::ProcessAuctionSpot(CPacket* pRecvPacket)
{
	return ;
}

/**
	입찰 취소 패킷
 */
void SPAuctionMock::ProcessAuctionCancel(CPacket* pRecvPacket)
{
	return ;
}


//////////////////////////////////////////////////////////////////////////
// 패킷을 만들기 위해 정보 생성
void SPAuctionMock::MakeAuctionListHeader(AUCTION_HEADER& ctHeader)
{
	int iRand ;
	std::string strBidderName ;
	std::string strSellerName ;
	ctHeader.iAuctionID = MakeRandomValue((INT64)100, (INT64)100000) ;
	ctHeader.iType = AUCTION_TYPE_ITEM_BY_ELY ;
	iRand = MakeRandomValue(0, 3) ;
	ctHeader.iDuration = AUCTION_DURATION(iRand) ;
	MakePreviousBidderString(strBidderName) ;
	MakePreviousSellerString(strSellerName) ;
	strcpy( ctHeader.szSeller, strBidderName.c_str()) ;
	strcpy( ctHeader.szBidder, strSellerName.c_str()) ;
	ctHeader.iRemainTime	= MakeRandomValue( (INT32)100, (INT32)1000) ;
	ctHeader.iBidPrice		= MakeRandomValue( (INT64)10000, (INT64)10000000) ;
	ctHeader.iBuyOutPrice	= MakeRandomValue( ctHeader.iBidPrice, (INT64)100000000) ;
	ctHeader.iCurrPrice		= MakeRandomValue( (INT64)10000, (INT64)100000000) ;
	ctHeader.iCurrUserID	= MakeRandomValue( (UINT32)10000, (UINT32)1000000) ;
	ctHeader.iSellerID		= MakeRandomValue( (UINT32)10000, (UINT32)1000000) ;
}


bool SPAuctionMock::MakeAuctionListItem(ITEM& ctItem, int iIndex)
{
	iIndex ? MakePreviousItemID(ctItem.iItemID, iIndex) : MakePreviousItemID(ctItem.iItemID) ;
	SPItemAttr* pItemAttr = NULL;
	if( g_pItemCluster->GetItemInfo(ctItem.iItemID, pItemAttr) ) {
		ctItem.iStackCount	= pItemAttr->m_iStackLimit;		// 아이템 갯수
		ctItem.iLifeTime	= pItemAttr->m_iLifeTime;			// 남은 타임이므로 음수 값이 올수 있음. DB 와 연관있음.
		ctItem.iRare		= MakeRandomValue((INT8)0, (INT8)6);				// 레어도
		ctItem.Opt[0].iID	= pItemAttr->m_iOptionID1;	// 옵션
		ctItem.Opt[1].iID	= pItemAttr->m_iOptionID2;	// 옵션
		ctItem.Opt[2].iID	= pItemAttr->m_iOptionID3;	// 옵션
		ctItem.Opt[3].iID	= pItemAttr->m_iOptionID4;	// 옵션
		ctItem.Opt[4].iValue	= 0;	// 옵션
		ctItem.Opt[0].iValue	= 0;	// 옵션
		ctItem.Opt[1].iValue	= 0;	// 옵션
		ctItem.Opt[2].iValue	= 0;	// 옵션
		ctItem.Opt[3].iValue	= 0;	// 옵션
		ctItem.Opt[4].iValue	= 0;	// 옵션
		ctItem.iItemNo		= pItemAttr->m_iNum;
		ctItem.iBackPrice	= pItemAttr->m_iCashPrice;			// 
		ctItem.iOptPoint	= pItemAttr->m_iCashPoint;
		ctItem.iItemExp		= 0;			// 아이템경험치
		ctItem.iItemSP		= 0;			// 아이템SP
		return true ;
	}
	return false ;
}


void SPAuctionMock::MakeAuctionListMoney(int& iMoney, int& iFeeMoney)
{
	iMoney = MakeRandomValue((int)100, (int)20000000) ;
	iFeeMoney = MakeRandomValue((int)10, (int)200000) ;
}



//////////////////////////////////////////////////////////////////////////
// 각 항목을 랜덤하게 생성

void SPAuctionMock::MakePreviousItemID(ITEMID& iID)
{
	int iIDS[15] = {	160201701, 160201702, 160201703, 160201704, 160201705, 
		160201706, 160201707, 160201708, 160201709, 160201710, 
		160201711, 160201712, 160201713, 160201714, 160201715} ;

	int iIndex = MakeRandomValue(0, 15) ;
	iID = iIDS[iIndex];
}

void SPAuctionMock::MakePreviousItemID(ITEMID& iID, int iIndex)
{
	int iIDS[15] = {	160201701, 160201702, 160201703, 160201704, 160201705, 
		180190101, 180190102, 180190103, 180190108, 180190109, 
		180190114, 180190120, 160201713, 160201714, 160201715} ;

	int k = iIndex % 15 ;
	iID = iIDS[k];
}





void SPAuctionMock::MakePreviousBidderString(std::string& str)
{
	//TCHAR szText[15][LEN_NAME+1] = {	{"내이름은"}, {"김삼순"}, {"어쩌라고"}, {"그러게말야"}, {"쩝쩝쩝"},
	//{"이런젠장"}, {"우찌이런일이"}, {"세상에"}, {"말세로다"}, {"님아제발"},
	//{"아싸가오리"}, {"아비오만세"}, {"크어어어억"}, {"씁쓸하네"}, {"에구이뽀"}
	//};

	//int iIndex = MakeRandomValue(0, 15) ;
	//str = szText[iIndex] ;
}


void SPAuctionMock::MakePreviousSellerString(std::string& str)
{
	//TCHAR szText[15][LEN_NAME+1] = {	
	//	{"S내이름은"}, {"S김삼순"}, {"S어쩌라고"}, {"S그러게말야"}, {"S쩝쩝쩝"},
	//	{"S이런젠장"}, {"S우찌이런일이"}, {"S세상에"}, {"S말세로다"}, {"S님아제발"},
	//	{"S아싸가오리"}, {"S아비오만세"}, {"S크어어어억"}, {"S씁쓸하네"}, {"S에구이뽀"}
	//};

	//int iIndex = MakeRandomValue(0, 15) ;
	//str = szText[iIndex] ;
}


//////////////////////////////////////////////////////////////////////////
template<class T>
T SPAuctionMock::MakeRandomValue(T tStart, T tEnd)
{
	srand( (unsigned)time( NULL ) );
	float fRand = (float)rand() / RAND_MAX ;		// 0 ~ 1.0f
	//ErrorFLogs->Error_Log("Make RandomValue [%f]", fRand) ;
	return (T)( fRand*tEnd + tStart ) ; 
}
