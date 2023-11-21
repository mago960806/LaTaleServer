// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPAuctionDef.h
//	created:	2008/2/26   Time:10:59
//	purpose:	
//*********************************************************************/

#pragma once

enum AUCTION_ARCH_DATA
{
	AUCTION_ARCH_NULL = 0,
	AUCTION_ARCH_ALL,			// 전체 목록관리
	AUCTION_ARCH_PART,			// 일부 목록 관리
} ;

enum AUCTION_ARCH_TAB
{
	AUCTOIN_ARCH_NULL = 0,
	AUCTOIN_ARCH_SEARCH ,
	AUCTOIN_ARCH_BUY ,
	AUCTOIN_ARCH_SALE ,
} ;

struct AUCTION_TYPE_INFO
{
	AUCTION_TYPE	iSaleType ;			// 경매 타입 (PacketID.h)
	bool			bRegistCharge ;		// 등록비용 징수여부
	UINT32			iConsumeItem ;		// 등록 소비아이템
	int				iShortRate ;		// 단기 등록비 비율
	int				iMiddleRate ;		// 중기 등록비 비율
	int				iLongRate ;			// 장기 등록비 비율

	void Clear()
	{
		iSaleType = AUCTION_TYPE_NULL ;
		bRegistCharge = false ;
		iConsumeItem = 0 ;
		iShortRate = 0 ;
		iMiddleRate = 0 ;
		iLongRate = 0 ;
	}
	int GetPeriodRate(int iPeriod)
	{
		if( iPeriod == 0)		return iShortRate ;
		else if( iPeriod == 1)	return iMiddleRate ;
		else					return iLongRate ;
	}
};


struct AUCTION_LIST_INFO
{
	AUCTION_TYPE	iSaleType ;			// 경매 타입 (PacketID.h)
	bool			bRegistCharge ;		// 등록비용 징수여부
	UINT32			iConsumeItem ;		// 등록 소비아이템
	float			fShortRate ;		// 단기 등록비 비율
	float			fMiddleRate ;		// 중기 등록비 비율
	float			fLongRate ;			// 장기 등록비 비율

	void Clear()
	{
		iSaleType = AUCTION_TYPE_NULL ;
		bRegistCharge = false ;
		iConsumeItem = 0 ;
		fShortRate = 0 ;
		fMiddleRate = 0 ;
		fLongRate = 0 ;
	}
};


/**
	경매에서 적용되는 Search Condition
*/
struct AUCTION_SEARCH_KEY
{
	int		iCategoryID ;
	short	iMinLevel ;		// 최소레벨 (1)
	short	iMaxLevel ;		// 최대레벨 (999)
	INT8	iQuality ;
	int		iMoneyMin ;		// 머니검색
	int		iMoneyMax ;		// 머니검색
	TCHAR	szName[MAX_ITEM_NAME+1] ;

	void Clear()
	{
		iCategoryID	= 0 ;
		iMinLevel	= 1 ;
		iMaxLevel	= 999 ;
		iQuality	= 0 ;	// Total(0)
		iMoneyMin	= 0 ;
		iMoneyMax	= 0 ;
		memset(&szName, 0, MAX_ITEM_NAME+1);
	}
	void Copy(AUCTION_SEARCH_KEY* Src) {
		iCategoryID = Src->iCategoryID ;
		iMinLevel	= Src->iMinLevel ;
		iMaxLevel	= Src->iMaxLevel ;
		iQuality	= Src->iQuality ;
		iMoneyMin	= Src->iMoneyMin ;
		iMoneyMax	= Src->iMoneyMax ;
		strcpy(szName, Src->szName) ;
	}
};