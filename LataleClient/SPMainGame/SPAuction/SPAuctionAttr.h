// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPAuctionAttr.h
//	created:	2008/2/26   Time:10:59
//	purpose:	
//*********************************************************************/

#pragma once 

//typedef struct _AUCTION_HEADER
//{
//	__int64 iAuctionID;				// 경매키
//	UINT8	iType;					// 경매타입
//	UINT8	iDuration;				// AUCTION_DURATION (판매자가 세팅한)
//	TCHAR	szName[LEN_NAME+1];		// 판매자 or 최고입찰자(등록리스트)
//	UINT32	iRemainTime;			// 현재남은시간
//
//	__int64 iBidPrice;				// 시작가 혹은 상회입찰가
//	__int64 iBuyOutPrice;			// 즉구가
//	__int64 iCurrPrice;				// 현재가
//	USERID  iCurrUserID;			// 현재입찰자계정키
//} AUCTION_HEADER, *PAUCTION_HEADER;

enum AUCTION_TYPE ;
class SPItem ;
class SPAuctionAttr
{

public:	
	SPAuctionAttr() ;
	~SPAuctionAttr() ;

	void Init() ;
	void Clear() ;

	bool SetAuctionAttr(AUCTION_HEADER* pHeader, AUCTION_GOODS_ITEM* pGoodsItem) ;
	bool SetAuctionAttr(AUCTION_HEADER* pHeader, AUCTION_GOODS_POINT* pGoodsItem) ;
	bool SetChangeHeader(AUCTION_HEADER* pHeader);		// For Header Information Change

	AUCTION_TYPE GetType()			{	return m_iAuctionType ;		}

	// For Interface
	SPItem*		GetItemInfo() ;			// SPItem*
	AUCTION_HEADER*	GetHeader() ;		// Auction Information
	INT64		GetAuctionID() ;		// Auction ID
	const char* GetItemName() ;			// Item Name
	int			GetItemQuality() ;		// Item Rare
	int			GetItemLevel() ;		// Level
	int			GetItemDuration() ;		// Duration
	UINT32		GetFinishTime() ;		// FinishTime
	const char* GetBidderName() ;		// BidUser Name
	const char* GetSellerName() ;		// SaleUser Name
	USERID		GetBidderUserID() ;		// BidUserID
	USERID		GetSellerUserID() ;		// SaleUserID
	INT64		GetCurBidPrice() ;		// Current BidPrice
	INT64		GetNextBidPrice() ;		// Next BidPrice
	INT64		GetSpotPrice() ;		// SPotPrice
	int			GetCategory() ;			// 경매 카테고리

	int			GetSellPoint()		{	return m_iSellPoint;	}		// 돈을 파는 경우 금액
	int			GetFeePoint()		{	return m_iFeePoint ;	}

private:
	bool SetItem(ITEM* pItem) ;
	bool SetItem(int iSellPoint, int iFeePoint) ;
	bool SetHeader(AUCTION_HEADER* pHeader);


private:
	AUCTION_TYPE	m_iAuctionType ;		//
	AUCTION_HEADER	m_ctAuctionHeader ;

	SPItem*	m_pAuctionItem;
	int		m_iSellPoint;			// Ely, Cash, Mileage
	int		m_iFeePoint;			// 수수료
	int		m_iRecvTime ;			// 남은 시간 계산을 위해 필요(Buy, Sale List)

};
