// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPAuctionMock.h
//	created:	2008/2/26   Time:10:59
//	purpose:	
//*********************************************************************/


#pragma once 

enum AUCTION_TYPE ;
class SPItem ;
class SPAuctionMock
{

public:	
	SPAuctionMock() ;
	~SPAuctionMock() ;

	void Init() ;
	void Clear() ;

	void SendAuctionSearchList(int iStart, int iSize) ;
	void SendAuctionBuyList(int iSize) ;
	void SendAuctionSaleList(int iSize) ;

	void ProcessAuctionCreate(CPacket* pRecvPacket) ;		// 물품등록 패킷
	void ProcessAuctionBid(CPacket* pRecvPacket) ;			// 입찰 패킷
	void ProcessAuctionSpot(CPacket* pRecvPacket) ;			// 즉구 패킷
	void ProcessAuctionCancel(CPacket* pRecvPacket) ;		// 입찰 취소 패킷

	void MakeAuctionListMoney(int& iMoney, int& iFeeMoney) ;
	bool MakeAuctionListItem(ITEM& ctItem, int iIndex = 0);

	void MakeAuctionListHeader(AUCTION_HEADER& ctHeader);

	void MakePreviousBidderString(std::string& str) ;
	void MakePreviousSellerString(std::string& str) ;
	void MakePreviousItemID(ITEMID& iID) ;
	void MakePreviousItemID(ITEMID& iID, int iIndex) ;

	template<class T> 
		T MakeRandomValue(T tStart, T tEnd) ;

private:
	INT64 m_iAuctionID ;
};


