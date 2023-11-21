// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPAuctionAttr.cpp
//	created:	2008/2/26   Time:10:59
//	purpose:	
//*********************************************************************/



#include "SPCommon.h"
#include "SPUtil.h"
#include "PacketID.h"

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

#include "SPAuctionDef.h"
#include "SPAuctionAttr.h"

#include "GlobalDefines_Share.h"
#include "_Error_log.h"


SPAuctionAttr::SPAuctionAttr()
: m_pAuctionItem(NULL)
{

}

SPAuctionAttr::~SPAuctionAttr()
{
	Clear() ;
}


void SPAuctionAttr::Init()
{
	m_iAuctionType = AUCTION_TYPE_NULL ;
}


void SPAuctionAttr::Clear()
{
	SAFE_DELETE(m_pAuctionItem) ;
	m_iAuctionType = AUCTION_TYPE_NULL ;
}


bool SPAuctionAttr::SetAuctionAttr(AUCTION_HEADER* pHeader, AUCTION_GOODS_ITEM* pGoodsItem)
{
	SetHeader(pHeader) ;
	SetItem(&pGoodsItem->stAuctionItem) ;
	SetItem(0, 0) ;

	m_iAuctionType = (AUCTION_TYPE)m_ctAuctionHeader.iType ;
	return false ;
}


bool SPAuctionAttr::SetAuctionAttr(AUCTION_HEADER* pHeader, AUCTION_GOODS_POINT* pGoodsItem)
{
	SetHeader(pHeader) ;
	SetItem(pGoodsItem->SellPoint, pGoodsItem->FeePoint) ;
	m_pAuctionItem = NULL ;

	m_iAuctionType = (AUCTION_TYPE)m_ctAuctionHeader.iType ;
	return false ;
}


bool SPAuctionAttr::SetChangeHeader(AUCTION_HEADER* pHeader)
{
	m_ctAuctionHeader.iAuctionID	= pHeader->iAuctionID ;
	m_ctAuctionHeader.iBidPrice		= pHeader->iBidPrice ;
	m_ctAuctionHeader.iBuyOutPrice	= pHeader->iBuyOutPrice ;
	m_ctAuctionHeader.iCurrPrice	= pHeader->iCurrPrice ;
	m_ctAuctionHeader.iDuration		= pHeader->iDuration ;
	m_ctAuctionHeader.iRemainTime	= pHeader->iRemainTime ;
	m_ctAuctionHeader.iType			= pHeader->iType ;
	m_ctAuctionHeader.iCurrUserID	= pHeader->iCurrUserID ;
	m_ctAuctionHeader.iSellerID		= pHeader->iSellerID ;
	strcpy( m_ctAuctionHeader.szBidder, pHeader->szBidder ) ;
	strcpy( m_ctAuctionHeader.szSeller, pHeader->szSeller ) ;
	m_iRecvTime = timeGetTime() ;

	return true ;
}

//////////////////////////////////////////////////////////////////////////
// for Interface
SPItem*	SPAuctionAttr::GetItemInfo()
{
	return m_pAuctionItem ;
}

AUCTION_HEADER*	SPAuctionAttr::GetHeader()
{
	return &m_ctAuctionHeader ;
}

INT64 SPAuctionAttr::GetAuctionID()
{
	return m_ctAuctionHeader.iAuctionID ;
}

const char* SPAuctionAttr::GetItemName()
{
	if( m_iAuctionType == AUCTION_TYPE_ITEM_BY_ELY )	{
		return m_pAuctionItem->GetItemStatus()->GetItemAttr()->m_strName.c_str() ;
	}
	else	{
		return ConvertMoneyToString( (INT64)GetSellPoint() ) ;
	}
}

/**
	정렬을 위해 사용되는 품질은
	아이템의 AuctionQuality가 있는경우 AuctionQuality를 사용
	AuctionQuality가 없으면 RareLevel을 사용한다.
 */
int	SPAuctionAttr::GetItemQuality()
{
	if( m_iAuctionType == AUCTION_TYPE_ITEM_BY_ELY )	{
		if( m_pAuctionItem->GetItemAttr()->m_iAuctionQuality == 0 )
			return m_pAuctionItem->GetItemStatus()->GetRareLevel() ;
		else
			return m_pAuctionItem->GetItemAttr()->m_iAuctionQuality ;
	}
	else
		return  GetSellPoint() ;
}

int	SPAuctionAttr::GetItemLevel()
{
	if( m_iAuctionType == AUCTION_TYPE_ITEM_BY_ELY )
		return m_pAuctionItem->GetItemStatus()->GetItemAttr()->m_iItemLevel ;
	else
		return 0 ;
}

int	SPAuctionAttr::GetItemDuration()
{
	if( m_iAuctionType == AUCTION_TYPE_ITEM_BY_ELY )
		return m_pAuctionItem->GetItemStatus()->SPSendMessage(MV_ITEM_GET_OPTPOINT);
	else
		return 0 ;
}

UINT32	SPAuctionAttr::GetFinishTime()
{
	int iCurTime = timeGetTime() ;
	int iElapsedTime = (int)((iCurTime - m_iRecvTime) / 1000 ) ;
	int iRemainTime = m_ctAuctionHeader.iRemainTime - iElapsedTime ;
	//ErrorFLogs->Error_Log("FinishTime [%d]-Elaps[%d] = [%d]", m_ctAuctionHeader.iRemainTime, iElapsedTime, iRemainTime) ;

	if( iRemainTime < 0 )	{
		ErrorFLogs->Error_Log("FinishTime Over Remain[%d]-Elaps[%d] = [%d]", m_ctAuctionHeader.iRemainTime, iElapsedTime, iRemainTime) ;
		return 0 ;
	}
	else
		return m_ctAuctionHeader.iRemainTime - iElapsedTime ;
}

const char* SPAuctionAttr::GetBidderName()
{
	return m_ctAuctionHeader.szBidder ;
}

const char* SPAuctionAttr::GetSellerName()
{
	return m_ctAuctionHeader.szSeller ;
}

USERID	SPAuctionAttr::GetBidderUserID()
{
	return m_ctAuctionHeader.iCurrUserID ;
}

USERID	SPAuctionAttr::GetSellerUserID()
{
	return m_ctAuctionHeader.iSellerID ;
}

INT64	SPAuctionAttr::GetCurBidPrice()
{
	return m_ctAuctionHeader.iCurrPrice ;
}

INT64	SPAuctionAttr::GetNextBidPrice()
{
	return m_ctAuctionHeader.iBidPrice ;
}

INT64	SPAuctionAttr::GetSpotPrice()
{
	return m_ctAuctionHeader.iBuyOutPrice ;
}

/*
	AUCTION_TYPE_ITEM_BY_ELY,	// 판매 : 아이템,	구매수단 : Ely
	AUCTION_TYPE_CASH_BY_ELY,	// 판매 : Cash,		구매수단 : Ely
	AUCTION_TYPE_MILEAGE_BY_ELY,// 판매 : Mileage,	구매수단 : Ely
	AUCTION_TYPE_ELY_BY_CASH,	// 판매 : Ely,		구매수단 : Cash
	AUCTION_TYPE_ELY_BY_MILEAGE,// 판매 : Ely,		구매수단 : Mileage
	*/
int		SPAuctionAttr::GetCategory()
{
	if( m_iAuctionType == AUCTION_TYPE_ITEM_BY_ELY )
		return m_pAuctionItem->GetItemStatus()->GetItemAttr()->m_iAuctionCategory ;
	else if( m_iAuctionType == AUCTION_TYPE_CASH_BY_ELY )
		return  GET_CATEGORY( AUCTION_CATEGORY_CASH ) ;
	else if( m_iAuctionType == AUCTION_TYPE_MILEAGE_BY_ELY )
		return  GET_CATEGORY( AUCTION_CATEGORY_MILEAGE ) ;
	else //if( m_iAuctionType == AUCTION_TYPE_ELY_BY_CASH || m_iAuctionType == AUCTION_TYPE_ELY_BY_MILEAGE) 
		return  GET_CATEGORY( AUCTION_CATEGORY_ELY ) ;
}


//////////////////////////////////////////////////////////////////////////
// Private Function
/**
	SetAuction Item Information
	SPItem* m_pAuctionItem
*/
bool SPAuctionAttr::SetItem(ITEM* pItem)
{
	SAFE_DELETE( m_pAuctionItem ) ;
	m_pAuctionItem = new SPItem(STORAGE, 0) ;

	return m_pAuctionItem->SetItemStatus(pItem);
}


/**
	SetAuction Money Price Information
	@param int iSellPoint	판매 금액
	@param int iFeePoint	수수료
*/
bool SPAuctionAttr::SetItem(int iSellPoint, int iFeePoint)
{
	m_iSellPoint = iSellPoint ;
	m_iFeePoint = iFeePoint ;

	return true;
}


/**
	Set Auction Header Information
	AUCTION_HEADER m_ctAuctionHeader
*/
bool SPAuctionAttr::SetHeader(AUCTION_HEADER* pHeader)
{
	m_ctAuctionHeader.iAuctionID	= pHeader->iAuctionID ;
	m_ctAuctionHeader.iBidPrice		= pHeader->iBidPrice ;
	m_ctAuctionHeader.iBuyOutPrice	= pHeader->iBuyOutPrice ;
	m_ctAuctionHeader.iCurrPrice	= pHeader->iCurrPrice ;
	m_ctAuctionHeader.iDuration		= pHeader->iDuration ;
	m_ctAuctionHeader.iRemainTime	= pHeader->iRemainTime ;
	m_ctAuctionHeader.iType			= pHeader->iType ;
	m_ctAuctionHeader.iCurrUserID	= pHeader->iCurrUserID ;
	m_ctAuctionHeader.iSellerID		= pHeader->iSellerID ;
	strcpy( m_ctAuctionHeader.szBidder, pHeader->szBidder ) ;
	strcpy( m_ctAuctionHeader.szSeller, pHeader->szSeller ) ;
	m_iRecvTime = timeGetTime() ;

	return true ;
}



