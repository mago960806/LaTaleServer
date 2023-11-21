// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPAuctionArchive.cpp
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
#include "SPAuctionArchive.h"
#include "SPAuctionManager.h"

#include "_Error_log.h"


//////////////////////////////////////////////////////////////////////////
// Generic Sort Algorithm
// 품질
struct _AuctionSortByQual_A : binary_function<SPAuctionAttr*, SPAuctionAttr*, bool> {
	bool operator() (SPAuctionAttr* lv, SPAuctionAttr* rv) const
	{
		if(lv->GetItemQuality() < rv->GetItemQuality())
			return true;
		return false;
	}
} AuctionSortByQual_A;
struct _AuctionSortByQual_D : binary_function<SPAuctionAttr*, SPAuctionAttr*, bool> {
	bool operator() (SPAuctionAttr* lv, SPAuctionAttr* rv) const
	{
		if(lv->GetItemQuality() > rv->GetItemQuality())
			return true;
		return false;
	}
} AuctionSortByQual_D;

// 레벨
struct _AuctionSortByLevel_A : binary_function<SPAuctionAttr*, SPAuctionAttr*, bool> {
	bool operator() (SPAuctionAttr* lv, SPAuctionAttr* rv) const
	{
		if(lv->GetItemLevel() < rv->GetItemLevel())
			return true;
		return false;
	}
} AuctionSortByLevel_A;
struct _AuctionSortByLevel_D : binary_function<SPAuctionAttr*, SPAuctionAttr*, bool> {
	bool operator() (SPAuctionAttr* lv, SPAuctionAttr* rv) const
	{
		if(lv->GetItemLevel() > rv->GetItemLevel())
			return true;
		return false;
	}
} AuctionSortByLevel_D;

// 내구도
struct _AuctionSortByDur_A : binary_function<SPAuctionAttr*, SPAuctionAttr*, bool> {
	bool operator() (SPAuctionAttr* lv, SPAuctionAttr* rv) const
	{
		if(lv->GetItemDuration() < rv->GetItemDuration() )
			return true;
		return false;
	}
} AuctionSortByDur_A;
struct _AuctionSortByDur_D : binary_function<SPAuctionAttr*, SPAuctionAttr*, bool> {
	bool operator() (SPAuctionAttr* lv, SPAuctionAttr* rv) const
	{
		if(lv->GetItemDuration() > rv->GetItemDuration())
			return true;
		return false;
	}
} AuctionSortByDur_D;

// 마감 시간
struct _AuctionSortByFinish_A : binary_function<SPAuctionAttr*, SPAuctionAttr*, bool> {
	bool operator() (SPAuctionAttr* lv, SPAuctionAttr* rv) const
	{
		if(lv->GetFinishTime() < rv->GetFinishTime())
			return true;
		return false;
	}
} AuctionSortByFinish_A;
struct _AuctionSortByFinish_D : binary_function<SPAuctionAttr*, SPAuctionAttr*, bool> {
	bool operator() (SPAuctionAttr* lv, SPAuctionAttr* rv) const
	{
		if(lv->GetFinishTime() > rv->GetFinishTime())
			return true;
		return false;
	}
} AuctionSortByFinish_D;

// 입찰 가격
struct _AuctionSortByPrice_A : binary_function<SPAuctionAttr*, SPAuctionAttr*, bool> {
	bool operator() (SPAuctionAttr* lv, SPAuctionAttr* rv) const
	{
		if(lv->GetCurBidPrice() < rv->GetCurBidPrice())
			return true;
		return false;
	}
} AuctionSortByPrice_A;
struct _AuctionSortByPrice_D : binary_function<SPAuctionAttr*, SPAuctionAttr*, bool> {
	bool operator() (SPAuctionAttr* lv, SPAuctionAttr* rv) const
	{
		if(lv->GetCurBidPrice() > rv->GetCurBidPrice())
			return true;
		return false;
	}
} AuctionSortByPrice_D;



//////////////////////////////////////////////////////////////////////////
// SPAuctionArchive
SPAuctionArchive::SPAuctionArchive()
: m_bNeedReset(true)
{
	Init() ;
}

SPAuctionArchive::~SPAuctionArchive()
{
	Clear() ;
}


void SPAuctionArchive::Init()
{
	//m_eSortKey = AUCTION_SORT_QUALITY ;
	m_vpAuctionList.clear() ;
	m_bIsLast = false ;
	m_iOnePage = -1 ;
	m_iTotalItemCount = 0 ;
	m_iArchiveType = AUCTION_ARCH_NULL ;
	SetDefaultSort(false) ;
}

void SPAuctionArchive::Clear()
{
	std::vector<SPAuctionAttr*>::iterator mIter ;
	mIter	=	m_vpAuctionList.begin() ;
	while ( mIter != m_vpAuctionList.end() )
	{
		SAFE_DELETE( (*mIter) );
		m_vpAuctionList.erase(mIter) ;
	}
	m_vpAuctionList.clear() ;
	m_iOnePage = -1 ;
	m_iArchiveType = AUCTION_ARCH_NULL ;
	m_iTotalItemCount = 0 ;
	m_iHaveSize = 0 ;
	m_bIsLast = false ;
}


void SPAuctionArchive::ClearList()
{
	std::vector<SPAuctionAttr*>::iterator mIter ;
	mIter	=	m_vpAuctionList.begin() ;
	while ( mIter != m_vpAuctionList.end() )
	{
		SAFE_DELETE( (*mIter) );
		m_vpAuctionList.erase(mIter) ;
	}
	m_vpAuctionList.clear() ;

	m_iTotalItemCount = 0 ;
	m_iStartIndex = 0 ;
	m_iHaveSize = 0 ;
	m_bIsLast = false ;
}


//////////////////////////////////////////////////////////////////////////
// List Function
/**
	@param AUCTION_ARCH_DATA	: Auction List Data
	@param int iOnePage			: OnePage 단위 크기 (Default:7)
	@param int iTotalCount		: 전체 페이지 크기 (for Part)
	@param int iStart			: 시작 위치 (for Part)
	@param int iEnd				: 끝 위치 (for Part)
*/
bool SPAuctionArchive::SetAuctionList(AUCTION_ARCH_DATA eType, int iOnePage/*=7*/, int iTotalCount/*= 0*/, int iStart/*=0*/, int iSize/*=0*/)
{
	Clear() ;
	m_iOnePage = iOnePage ;
	m_iArchiveType = eType ;
	m_bNeedReset = false ;

	// for PART List
	m_iTotalItemCount = iTotalCount ;
	m_iStartIndex = iStart ;

	if( iTotalCount == (iStart + iSize) )
		m_bIsLast = true ;

	return false ;
}

/**
	@param SPAuctionAttr* pAuctionItem	: Item Data
	@param int iIndex					: InsertPosition (-1:END)
*/
bool SPAuctionArchive::AddAuctionList(SPAuctionAttr* pAuctionItem, bool bCount/*=false*/)
{
	// [20080616]추가시에는 무조건 추가해서 필요한 정보를 찾을수 있도록 하고
	// BuyList, SaleList를 받기전에 리스트를 초기화해서 다시 받는다.
	//if( m_bNeedReset )
	//	return false ;

	m_vpAuctionList.push_back(pAuctionItem) ;
	if( bCount )		++m_iTotalItemCount ;

	m_iHaveSize = (int)m_vpAuctionList.size() ;
	if( m_iArchiveType == AUCTION_ARCH_ALL)
		m_iTotalItemCount = m_iHaveSize ;
	return true ;
}

/**
	해당페이지 Index의 데이터를 변경한다.
	변경은 Aution_Header 정보만 변경된다.
	@param int iPage		: Page Number
	@param int iIndex		: List Index
	@param SPAuctionAttr* pAuctionItem	: Data
*/
bool SPAuctionArchive::ChangeAuctionList(int iPage, int iIndex, SPAuctionAttr* pAuctionItem)
{
	int iListIndex = iPage * m_iOnePage + iIndex ;
	int iSize = (int)m_vpAuctionList.size() ;

	if( m_iArchiveType == AUCTION_ARCH_ALL)
	{
		iListIndex = iPage * m_iOnePage + iIndex ;
		iSize = (int)m_vpAuctionList.size() ;
	}
	else
	{
		iListIndex = iPage * m_iOnePage + iIndex - m_iStartIndex ;		// 일부인경우 시작 Index 계산
		iSize = (int)m_vpAuctionList.size() ;
	}

	if( iListIndex < iSize ) {
		return m_vpAuctionList[iListIndex]->SetChangeHeader(pAuctionItem->GetHeader()) ;
	}

	return false ;
}


/**
	해당 AuctionID를 찾아서 업데이트 한다.
	@param INT64 iAuctionID
	@param void* pAuctionHeader (AUCTION_HEADER*)
 */
bool SPAuctionArchive::UpdateAuctionArchive(INT64 iAuctionID, void* pAuctionHeader)
{
	SPAuctionAttr* pAttr = NULL ;
	if( ( pAttr = FindAuctionItem(iAuctionID) ) == NULL )
		return false ;

	return pAttr->SetChangeHeader((AUCTION_HEADER*)pAuctionHeader) ;
}




/**
	해당 AuctionID 의 데이터를 삭제
	@param int iAuctionID
*/
bool SPAuctionArchive::DeleteAuctionItem(INT64 iAuctionID, bool bCount/*=false*/)
{
	//if( m_bNeedReset )
	//	return false ;

	SPAuctionAttr* pAttr ;
	std::vector<SPAuctionAttr*>::iterator mIter ;
	mIter	=	m_vpAuctionList.begin() ;
	while ( mIter != m_vpAuctionList.end() )
	{
		pAttr = (*mIter) ;
		if( pAttr->GetAuctionID() == iAuctionID )	{
			SAFE_DELETE( (*mIter) );
			m_vpAuctionList.erase(mIter) ;
			if( bCount )	--m_iTotalItemCount ;

			m_iHaveSize = (int)m_vpAuctionList.size() ;
			if( m_iArchiveType == AUCTION_ARCH_ALL)
				m_iTotalItemCount = m_iHaveSize ;
			return true ;
		}
		++mIter ;
	}

	return false ;
}


/**
	해당페이지 Index의 데이터를 가져온다.
	@param int iPage		: Page Number (1 Base)
	@param int iIndex		: List Index (0 Base)
	@return SPAuctionAttr* pAuctionItem	: return Data
*/
SPAuctionAttr* SPAuctionArchive::GetAuctionItem(int iPage, int iIndex)
{
	if( m_bNeedReset )
		return NULL ;

	int iListIndex ;
	int iSize ;
	if( m_iArchiveType == AUCTION_ARCH_ALL)
	{
		iListIndex = iPage * m_iOnePage + iIndex ;
		iSize = (int)m_vpAuctionList.size() ;
	}
	else
	{
		iListIndex = iPage * m_iOnePage + iIndex - m_iStartIndex ;		// 일부인경우 시작 Index 계산
		iSize = (int)m_vpAuctionList.size() ;
	}

	if( iListIndex < iSize )
		return m_vpAuctionList[iListIndex] ;
	return NULL ;
}


/**
	해당페이지 Index의 데이터를 가져온다.
	@param int iPage		: Page Number (1 Base)
	@param int iIndex		: List Index (0 Base)
	@return SPAuctionAttr* pAuctionItem	: return Data
*/
SPAuctionAttr* SPAuctionArchive::FindAuctionItem(INT64 iAuctionID)
{
	//if( m_bNeedReset )	
	//	return NULL ;

	SPAuctionAttr* pAttr ;
	std::vector<SPAuctionAttr*>::iterator mIter ;
	mIter	=	m_vpAuctionList.begin() ;
	while ( mIter != m_vpAuctionList.end() )
	{
		pAttr = (*mIter) ;
		if( pAttr->GetAuctionID() == iAuctionID )	{
			return pAttr ;
		}
		++mIter ;
	}
	return NULL ;
}


/**
	한페이지 데이터를 가져온다.
	@param std::vector<SPAuctionAttr*>& pAuctionItem	: return Data
	@param int iPage		: Page Number(1 Base)
	@return int				: Item count 	
*/
int SPAuctionArchive::GetAuctionListPage(std::vector<SPAuctionAttr*>& vAuctionList, int iPage)
{
	int iSize ;
	int iStart, iNext, iEnd ;
	int iGetStart, iGetCount ;
	SPAuctionAttr* pAttr ;

	if( m_iArchiveType == AUCTION_ARCH_ALL)
	{
		iStart = iPage * m_iOnePage ;
		if( iStart < 0 )	iStart = 0 ;
		iSize = (int)m_vpAuctionList.size() ;
		iNext = iStart + m_iOnePage ;
		iEnd = (iNext < iSize ) ? iNext : iSize ;

		iGetStart = iStart ;
		iGetCount = iEnd - iStart ;

		for( int i = iGetStart; i < iGetStart+iGetCount; i++ )
		{
			pAttr = m_vpAuctionList[i] ;
			vAuctionList.push_back(pAttr) ;
		}
		return iGetCount ;
	}
	else
	{
		iStart = iPage * m_iOnePage ;			// Abs Index
		if( iStart < 0 )	iStart = 0 ;
		iSize = (int)m_vpAuctionList.size() + m_iStartIndex ;
		iNext = iStart + m_iOnePage ;
		iEnd = (iNext < iSize ) ? iNext : iSize ;

		if( m_iStartIndex > iStart )
			return 0 ;

		iGetStart = iStart - m_iStartIndex ;
		iGetCount = iEnd - iStart ;


		for( int i = iGetStart; i < iGetStart+iGetCount; i++ )
		{
			pAttr = m_vpAuctionList[i] ;
			vAuctionList.push_back(pAttr) ;
		}
		return iGetCount ;
	}
	return 0 ;
}


/**
	리스트의 페이지 갯수를 가져온다.
	물품목록의 일부를 받는 경우가 있으므로 TotalCount가 있으면 그것으로 계산한다.
*/
int SPAuctionArchive::GetTotalPageCount()
{
	int iSize = ( m_iTotalItemCount ) ? m_iTotalItemCount : (int)m_vpAuctionList.size() ;
	if( iSize == 0 || m_iOnePage == 0 )
		return 0 ;
	int iExtra = ( (iSize % m_iOnePage) == 0 ) ? 0 : 1 ;

	return (iSize / m_iOnePage) + iExtra  ;
}


int SPAuctionArchive::GetListCount()
{
	return (int)m_vpAuctionList.size() ;
}


/**
	해당 페이지의 정보가 Archive에 있는지 확인	
	@param int iPage : 0 Base Page
*/
bool SPAuctionArchive::HasPageInfo(int iPage)
{
	int iSize ;
	int iStartPage ;
	int iEndPage ;
	int iExtra = 0 ;

	iSize = (int)m_vpAuctionList.size() ;				// 리스트 크기
	iExtra = ( (iSize % m_iOnePage) == 0 ) ? 0 : 1 ;	// 마지막 여분페이지
	if( m_iArchiveType == AUCTION_ARCH_ALL)
	{
		iEndPage = iSize / m_iOnePage + iExtra ;
		return ( iEndPage > iPage ) ? true : false ;
	}
	else
	{
		// 일부만 가지고 있는 경우
		iStartPage = (m_iStartIndex+1) / m_iOnePage ;
		if( iStartPage > iPage )	
			return false ;

		iEndPage = iStartPage + (iSize / m_iOnePage) -1;	// 여분의 페이지는 버림
		if( m_bIsLast && iExtra )		++iEndPage ;	// 끝 데이터이고 여분이 있으면 마지막 남은 페이지 추가
		if( iEndPage < iPage )
			return false ;
		return true ;
	}
	return false ;
}


/**
	AUCTION_ARCH_PART 타입중에서
	Part의 마지막 유효한 페이지 인지 여부를 판단한다.
	SearchArchive에서 즉시구매가 발생되었을때 리스트의 업데이트를 위해서 사용된다.
 */
bool SPAuctionArchive::IsPartLastPage(int iPage)
{
	int iStartPage ;
	int iEndPage ;
	int iExtra = 0 ;

	if( m_iArchiveType == AUCTION_ARCH_ALL )	// Part리스트가 아니면 false
		return false ;
	if( m_bIsLast )			// 전체리스트의 끝이면 false 
		return false ;

	iStartPage = (m_iStartIndex+1) / m_iOnePage ;
	iEndPage = iStartPage + (m_iHaveSize / m_iOnePage) -1 ;		// 여분의 페이지는 버림
	iExtra = ( (m_iHaveSize % m_iOnePage) == 0 ) ? 0 : 1 ;	// 마지막 여분페이지

	// 마지막 페이지 이고 여분의 데이터가 없으면 Part의 마지막 데이터 이다.
	// 이 경우 다음 데이터를 요청해야 한다.
	if( iPage == iEndPage && iExtra == 0 )	
		return true ;

	return false ;
}



/**
	가장 최근에 업데이트된 SortKey를 반환한다.
*/
void SPAuctionArchive::GetLatestSortKey(AUCTION_SORT_KEY& eSortKey, int& iAscend)
{
	eSortKey = m_eSortKey ;
	iAscend = m_iSortAscend[eSortKey] ;
}


int SPAuctionArchive::GetSortAscendKey(AUCTION_SORT_KEY eSortKey)
{
	return m_iSortAscend[eSortKey] ;
}

/**
	iSortKey를 통해서 Archive를 sort한다.
	@param AUCTION_SORT_KEY eSortKey
	@param int iAscend	: 0:내림차순, 1:오름차순
*/
bool SPAuctionArchive::SetSort(AUCTION_SORT_KEY eSortKey, bool bReal /*= true*/)
{
	bool bRet = false ;

	if( bReal == false ) {
		m_eSortKey = eSortKey ;
		m_iSortAscend[eSortKey] = m_iSortAscend[eSortKey] ? 0 : 1 ;		// Toogle
		return true ;
	}

	if( m_bNeedReset || m_iArchiveType != AUCTION_ARCH_ALL )
		return false ;

	// 실제 Sort
	int iAscend = m_iSortAscend[eSortKey] ? 0 : 1 ;		// Toogle
	switch(eSortKey)	{
		case AUCTION_SORT_QUALITY:		// 품질
			iAscend ? sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByQual_A) 
				: sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByQual_D) ;
			bRet = true ;
			break ;
		case AUCTION_SORT_ITEM_LV:			// 레벨
			iAscend ? sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByLevel_A) 
				: sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByLevel_D) ;
			bRet = true ;
			break ;
		case AUCTION_SORT_ITEM_OPTPOINT:	// 내구도
			iAscend ? sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByDur_A) 
				: sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByDur_D) ;
			bRet = true ;
			break ;
		case AUCTION_SORT_DURATION:	// 마감시간
			iAscend ? sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByFinish_A) 
				: sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByFinish_D) ;
			bRet = true ;
			break ;
		case AUCTION_SORT_PRICE:			// 입찰가(ely,cash,mile)
			iAscend ? sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByPrice_A) 
				: sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByPrice_D) ;
			bRet = true ;
			break ;

		default:
			bRet = false ;
			break ;
	}

	// Sort성공시 현재 Sort 저장
	if( bRet )	{
		m_eSortKey = eSortKey ;
		m_iSortAscend[eSortKey] = iAscend ;
	}
	return bRet ;
}


/**
	기본 Sort설정은 Price를 제외한 모든 Sort는 Descend
	마감시간은 Default에 설정하지 않고 Ascend로 셋팅
	Descend: 0, Ascend: 1
 */
void SPAuctionArchive::SetDefaultSort(bool bReal/*=true*/)
{
	m_iSortAscend[AUCTION_SORT_NULL] = 0 ;
	m_iSortAscend[AUCTION_SORT_DURATION] = 1 ;

	m_iSortAscend[AUCTION_SORT_ITEM_OPTPOINT] = 0 ;
	//if( bReal ) sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByDur_D) ;

	m_iSortAscend[AUCTION_SORT_PRICE] = 1 ;
	//if( bReal ) sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByPrice_A) ;

	m_iSortAscend[AUCTION_SORT_ITEM_LV] = 0 ;
	if( bReal ) sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByLevel_D) ;

	m_eSortKey = AUCTION_SORT_QUALITY ;
	m_iSortAscend[AUCTION_SORT_QUALITY] = 0 ;
	if( bReal ) sort(m_vpAuctionList.begin(), m_vpAuctionList.end(), AuctionSortByQual_D) ;
}

