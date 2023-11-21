// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPAuctionManager.cpp
//	created:	2008/2/26   Time:11:00
//	purpose:	
//*********************************************************************/

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

#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
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

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPWindow.h"
#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"

#include "SPAuctionDef.h"
#include "SPAuctionAttr.h"
#include "SPAuctionArchive.h"
//#include "SPAuctionMock.h"
#include "SPAuctionManager.h"
#include "GlobalDefines_Share.h"

#include "_Error_log.h"





SPAuctionManager* g_pkAuctionManager = NULL;
SPAuctionManager* SPAuctionManager::m_pkInstance = NULL;
//------------------------------------------------------------------------------------
SPAuctionManager* SPAuctionManager::GetInstance()
{
	if( m_pkInstance == NULL )
	{
		m_pkInstance = new SPAuctionManager;
		g_pkAuctionManager = m_pkInstance;
	}

	return m_pkInstance;
}

//------------------------------------------------------------------------------------
void SPAuctionManager::Release()
{
	if( m_pkInstance )
	{
		g_pkAuctionManager = NULL; 

		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}

//------------------------------------------------------------------------------------
SPAuctionManager::SPAuctionManager()
: m_pSearchArchive(NULL)
, m_pBuyArchive(NULL)
, m_pSaleArchive(NULL)
, m_bOpen(true)
, m_bTest(false)
{
}

//------------------------------------------------------------------------------------
SPAuctionManager::~SPAuctionManager()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPAuctionManager::Clear()
{
	for( int i = 0; i < 5; i++ )	{
		SAFE_DELETE( m_ctAuactionType[i] ) ;
	}
	SAFE_DELETE( m_pSearchArchive) ;
	SAFE_DELETE( m_pBuyArchive) ;
	SAFE_DELETE( m_pSaleArchive) ;
}

//------------------------------------------------------------------------------------
bool SPAuctionManager::Init()
{
	m_iAuctionTypeCount = 0 ;
	m_bOpen = true ;
	m_iSearch = 0 ;
	m_iSysAuctionMoneyID	= 0 ;	// GlobalSys의 설정값
	m_iSysGamePriceID		= 0 ;	// GlobalSys의 설정값
	m_iSysStandardPriceID	= 0 ;	// GlobalSys의 설정값
	m_iSysStartMoneyID		= 0 ;	// GlobalSys의 설정값(시작가)

	m_pSearchArchive	= new SPAuctionArchive ;
	m_pSearchArchive->SetAuctionList(AUCTION_ARCH_PART, 6) ;
	m_pBuyArchive		= new SPAuctionArchive ;
	m_pBuyArchive->SetAuctionList(AUCTION_ARCH_ALL, 7) ;
	m_pSaleArchive		= new SPAuctionArchive ;
	m_pSaleArchive->SetAuctionList(AUCTION_ARCH_ALL, 7) ;

	LoadAuctionTypeLDT() ;

	return true ;
}

//------------------------------------------------------------------------------------

/**
	판매유형 정보 AUCTION_TYPE.LDT Load
*/
bool SPAuctionManager::LoadAuctionTypeLDT()
{
	int i ;

	// LDT Struct 초기화
	for( i = 0; i < 5; i++ )	{
		m_ctAuactionType[i] = new AUCTION_TYPE_INFO ;
		m_ctAuactionType[i]->Clear() ;
	}

	std::string strLDTFile;
	strLDTFile = "DATA/LDT/AUCTION_TYPE.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
	{
		ErrorFLogs->Error_Log("Can't Find AUCTION_TYPE.LDT[%s]", strLDTFile.c_str()) ;
		return false;
	}

	int iDataNum;
	int iIndex = 0 ;
	long lItemID;
	int iCategoryIndex = 0 ;

	LDT_Field Field;
	iDataNum = pkLDTFile->GetItemCount();
	m_iAuctionTypeCount = iDataNum ;
	for( i = 0; i < iDataNum; i++ )
	{
		lItemID = pkLDTFile->GetPrimaryKey(i);

		Field.Init() ;					// 판매유형
		pkLDTFile->GetField(lItemID, 0, Field);
		m_ctAuactionType[iIndex]->iSaleType = (AUCTION_TYPE)Field.uData.lValue ;

		// 한줄 데이터는 안되므로 잘못된 데이터는 스킵
		if( m_ctAuactionType[iIndex]->iSaleType <= AUCTION_TYPE_NULL )
			continue ;

		Field.Init() ;					// 등록비 사용유무
		pkLDTFile->GetField(lItemID, 1, Field);
		m_ctAuactionType[iIndex]->bRegistCharge = Field.uData.lValue ? true : false ;
		Field.Init() ;					// 등록비 소비아이템
		pkLDTFile->GetField(lItemID, 2, Field);
		m_ctAuactionType[iIndex]->iConsumeItem = Field.uData.lValue;
		Field.Init() ;					// 단기 등록비 비율
		pkLDTFile->GetField(lItemID, 3, Field);
		m_ctAuactionType[iIndex]->iShortRate = Field.uData.lValue ;
		Field.Init() ;					// 중기 등록비 비율
		pkLDTFile->GetField(lItemID, 4, Field);
		m_ctAuactionType[iIndex]->iMiddleRate = Field.uData.lValue ;
		Field.Init() ;					// 장기 등록비 비율
		pkLDTFile->GetField(lItemID, 5, Field);
		m_ctAuactionType[iIndex]->iLongRate = Field.uData.lValue ;
		++iIndex ;

	}
	SAFE_RELEASE(pkLDTFile) ;

	//////////////////////////////////////////////////////////////////////////
	// Auction 수수료, 시작가를 위한 Global_sys.ldt 읽기	
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_GLOBAL_SYS , &pkLDTFile );
	if( pkLDTFile == NULL )
	{
		return false ;
	}

	Field.Init() ;
	pkLDTFile->GetField( 91 , 0 ,	Field );		//	Auction Money ID
	m_iSysAuctionMoneyID	=	Field.uData.lValue;

	Field.Init() ;
	pkLDTFile->GetField( 92 , 0 ,	Field );		//	m_iSysGamePriceID
	m_iSysGamePriceID	=	Field.uData.lValue;

	Field.Init() ;
	pkLDTFile->GetField( 93 , 0 ,	Field );		//	m_iSysStandardPriceID
	m_iSysStandardPriceID	=	Field.uData.lValue;

	Field.Init() ;
	pkLDTFile->GetField( 94 , 0 ,	Field );		//	m_iSysStartMoneyID
	m_iSysStartMoneyID	=	Field.uData.lValue;
	SAFE_RELEASE(pkLDTFile) ;

	return true ;
}

//////////////////////////////////////////////////////////////////////////
//	Localize로 부터 읽어야할 데이터 셋팅
/**
	[수수료 비율]
*/
int	SPAuctionManager::GetAuctionFeeRate()
{
	return SPLocalizeManager::GetInstance()->GetAuctionFeeRate() ;
}
/**
	[단기] 시간 설정
*/
int	SPAuctionManager::GetAuctionTimeShort()
{
	return SPLocalizeManager::GetInstance()->GetAuctionTimeShort() ;
}
/**
	[중기]시간 설정
*/
int	SPAuctionManager::GetAuctionTimeMiddle()
{
	return SPLocalizeManager::GetInstance()->GetAuctionTimeMiddle() ;
}
/**
	[최장기]시간 설정
*/
int	SPAuctionManager::GetAuctionTimeLong()
{
	return SPLocalizeManager::GetInstance()->GetAuctionTimeLong() ;
}

/**
	Global_Sys.LDT에서 읽은 설정값
 */
int	SPAuctionManager::GetSysMoneyID()
{
	return m_iSysAuctionMoneyID ;
}
int	SPAuctionManager::GetSysGamePriceID()
{
	return m_iSysGamePriceID ;
}
int	SPAuctionManager::GetSysStandardPriceID()
{
	return m_iSysStandardPriceID ;
}
int	SPAuctionManager::GetSysStartMoneyID()
{
	return m_iSysStartMoneyID ;
}


//---------------------------------------------------------------------
/**
	Player Money정보
 */
INT64 SPAuctionManager::GetPlayerEly()
{
	SPPlayer* pLocalPlayer = g_pGOBManager->GetLocalPlayer() ;
	if( pLocalPlayer ) 
		return pLocalPlayer->GetCheckStatusValue(CHECK_STATUS_MONEY) ;

	return 0 ;
}

int	SPAuctionManager::GetPlayerCash()
{
	SPPlayer* pLocalPlayer = g_pGOBManager->GetLocalPlayer() ;
	if( pLocalPlayer ) 
		return pLocalPlayer->GetStatus()->GetStatusValue(STATUS_CASH) ;

	return 0 ;
}

int	SPAuctionManager::GetPlayerMilige()
{
	SPPlayer* pLocalPlayer = g_pGOBManager->GetLocalPlayer() ;
	if( pLocalPlayer )
		return pLocalPlayer->GetStatus()->GetStatusValue(STATUS_MILE) ;

	return 0 ;
}


void SPAuctionManager::UpdatePlayerEly(INT64 iEly)
{
	SPPlayer* pLocalPlayer = g_pGOBManager->GetLocalPlayer() ;
	if( pLocalPlayer )	{
		pLocalPlayer->GetStatus()->SetStatusValueEX(STATUS_EX_ELY, iEly) ;
	}
}

void SPAuctionManager::UpdatePlayerCash(int iCash, int iMilige)
{
	SPPlayer* pLocalPlayer = g_pGOBManager->GetLocalPlayer() ;
	if( pLocalPlayer )	{
		pLocalPlayer->GetStatus()->SetStatusValue(STATUS_CASH, iCash) ;
		pLocalPlayer->GetStatus()->SetStatusValue(STATUS_MILE, iMilige) ;
	}
}

//------------------------------------------------------------------------------------
void SPAuctionManager::Process(float fTime)
{
}


/**
	설정된 경매 타입에 대한 정보를 읽는다.
*/
AUCTION_TYPE_INFO*	SPAuctionManager::GetAuctionTypeInfo(int iAuctionTypeID)
{
	for( int i = 0; i < m_iAuctionTypeCount ; i++ )
	{
		if( m_ctAuactionType[i]->iSaleType == iAuctionTypeID )
		{
			return m_ctAuactionType[i] ;
		}
	}
	return NULL ;
}

/**
	index를 통해서 설정된 TypeID를 얻어온다.
	@param int iIndex : 1BASE Index
 */
int SPAuctionManager::GetAuctionTypeID(int iIndex)
{
	if( m_iAuctionTypeCount < iIndex || iIndex <= 0 )
		return 0 ;
	return m_ctAuactionType[iIndex-1]->iSaleType ;
}

int	SPAuctionManager::GetAuctionTypeCount()
{
	return m_iAuctionTypeCount ;
}


/**
	Auction을 위한 Valid 체크
	1) 시작가, 등록비용을 위한 Global_Sys 체크 
 */
bool SPAuctionManager::CheckAuctionValid()
{
	if( m_iSysAuctionMoneyID < MIN_AUCTION_STARTPRICE_FACTOR || m_iSysAuctionMoneyID > MAX_AUCTION_STARTPRICE_FACTOR )	{
		return false ;
	}
	if( m_iSysGamePriceID < MIN_AUCTION_GAMEPRICE_FACTOR || m_iSysGamePriceID > MAX_AUCTION_GAMEPRICE_FACTOR )	{
		return false ;
	}
	if( m_iSysStandardPriceID < MIN_AUCTION_STANDARDPRICE_FACTOR || m_iSysStandardPriceID > MAX_AUCTION_STANDARDPRICE_FACTOR )	{
		return false ;
	}
	return true ;
}


/**
	Archive Reset 여부 확인
	NeedReset인 경우 서버에 리스트 요청
 */
bool SPAuctionManager::CheckArchiveReset(int iTab)
{
	if( iTab == 1 )			// Buy
	{
		return m_pBuyArchive->GetNeedReset() ;
	}
	else if( iTab == 2 )	// Sale
	{
		return m_pSaleArchive->GetNeedReset() ;
	}
	return false ;
}


/**
	Player변경시 Auction Reset
 */
void SPAuctionManager::SetAuctionReset()
{
	m_bOpen = true ;
	m_pSearchArchive->ClearList() ;
	m_pBuyArchive->ClearList() ;
	m_pBuyArchive->SetNeedReset(true) ;
	m_pSaleArchive->ClearList() ;
	m_pSaleArchive->SetNeedReset(true) ;
}

/**
	Server Down or Open 시 설정 변경
 */
void SPAuctionManager::SetServerOpen(bool bOpen)
{
	m_bOpen = bOpen ;
}

bool SPAuctionManager::GetServerOpen()
{
	return m_bOpen ;
}


//////////////////////////////////////////////////////////////////////////
// Archive
SPAuctionArchive*	SPAuctionManager::GetAuctionSearchArchive()
{
	return m_pSearchArchive ;
}
SPAuctionArchive*	SPAuctionManager::GetAuctionBuyArchive()
{
	return m_pBuyArchive ;
}
SPAuctionArchive*	SPAuctionManager::GetAuctionSaleArchive()
{
	return m_pSaleArchive ;
}

/**
	Archive List를 모두 받으면 기본 Sort를 해주도록 한다.
	Archive Sort Reset (0:Buy, 1:Sale)
	리스트를 모두 받은이후 Sort를 하고 List를 업데이트 한다.
 */
void	SPAuctionManager::SetDefaultSort(int iArchiveType)
{
	SPWindow* pWindowAuction = g_pInterfaceManager->GetAuctionWindow() ;
	if( iArchiveType == 0 )	{
		m_pBuyArchive->SetDefaultSort(true) ;
		if(pWindowAuction && pWindowAuction->IsShow()) {
			
			pWindowAuction->SPSendMessage(SPIM_AUCTION_BUY_LIST, (WPARAM)0, (LPARAM)0);
		}
	}
	else	{
		m_pSaleArchive->SetDefaultSort(true) ;
		if(pWindowAuction && pWindowAuction->IsShow()) {
			pWindowAuction->SPSendMessage(SPIM_AUCTION_SALE_LIST, (WPARAM)0, (LPARAM)0);
		}
	}
}


/**
	Finish와 같은 상황일경우, Auction Archive의 데이터를 삭제후 삭제여부를 알려준다.
	자신의 동작에 의한경우 SearchArchive를 삭제하고, 타인에 의한경우 받지 않는다.
	@param INT64 iAuctionID : 찾을 AuctionID
	@param bool bSearch : SearchArchive의 삭제여부
	@return int : Update Count
*/
int SPAuctionManager::UpdateAuctionFinish(INT64 iAuctionID, bool bSearch)
{
	int iRet = 0 ;
	SPAuctionAttr* pAttr = NULL ;
	if( iAuctionID < 0 )
		return 0 ;

	if( bSearch )	{
		if( m_pSearchArchive->DeleteAuctionItem(iAuctionID, true) )
			++iRet ;
	}
	if( m_pBuyArchive->DeleteAuctionItem(iAuctionID, true) )
		++iRet ;
	if( m_pSaleArchive->DeleteAuctionItem(iAuctionID, true)  )
		++iRet ;
	return iRet ;
}


/**
	등록 비용 계산 (등록비용은 Ely, AUCTION_TYPE.LDT 정보에서 처리)
	AUCTION_TYPE_ITEM_BY_ELY : Auction_Enrol = [{(Item_Lv*1.5)+(Item_Quilty*5)*(시작가*0.2)+(내구도*10)}/2] * 기간비율
	AUCTION_TYPE_ELY_BY_CASH : 없음
	AUCTION_TYPE_CASH_BY_ELY : Auction_Enrol = {(Cash_money*30)+(시작가*0.2)} * 기간비율
	AUCTION_TYPE_ELY_BY_MILEAGE : 없음
	AUCTION_TYPE_MILEAGE_BY_ELY : Auction_Enrol = {(Mile_Money*25)*(시작가*0..2)} * 기간비율
*/
INT64 SPAuctionManager::ComputeEnrolFee(AUCTION_TYPE eType, INT64 iStart, int iDuration, int iValue, int iLv/*=0*/, int iGamePrice/*=0*/, int iItemCount/*=0*/)
{
	AUCTION_TYPE_INFO* pTypeInfo = NULL ;
	INT64 iRetFee = 0 ;
	INT64 iEnrolFee = 0 ;

	pTypeInfo = GetAuctionTypeInfo(eType) ;
	if( pTypeInfo == NULL )	{
		ErrorFLogs->Error_Log("Can't Find Auction type info [%d]", eType) ;
		return -1 ;
	}

	if( pTypeInfo->bRegistCharge == false )		// 등록비용이 셋팅되지 않은 경우
		return 0 ;

	INT64 iItemFee = 0 ;
	INT64 iStandFee = 0 ;
	switch(eType)
	{
	case AUCTION_TYPE_ITEM_BY_ELY:			// 판매 : 아이템,	구매수단 : Ely
		// Auction_Enrol = [{(Item_Lv*1.5)+(Item_Quilty*5)+(Auction_Money*0.2)+(내구도*10)}/40]
		// iEnrolFee = (INT64)( ((float)iLv*1.5f + (iQuality*5) + iStart*0.2f + iDurability*10 ) / 40 + 0.5f ) ;
		iItemFee = (INT64)GET_AUCTION_ENROLFEE_ITEM(iStart, m_iSysAuctionMoneyID, iGamePrice, m_iSysGamePriceID, iItemCount) ;
		iStandFee = (INT64)GET_AUCTION_ENROLFEE_ITEM_STANDARD(iLv, m_iSysStandardPriceID) ;
		iEnrolFee = (INT64)max( iItemFee, iStandFee ) ;
		break ;
	case AUCTION_TYPE_CASH_BY_ELY:			// 판매 : Cash,		구매수단 : Ely
		// Auction_Enrol = {(Cash_money*30)+(시작가*0.2)}/2
		// iEnrolFee = (INT64)( (iValue*30 + (float)iStart*0.2f) /2 + 0.5f ) ;
		iEnrolFee = (INT64)GET_AUCTION_ENROLFEE_CASH(iStart,  iValue) ;
		break ;
	case AUCTION_TYPE_MILEAGE_BY_ELY:		// 판매 : Mileage,	구매수단 : Ely
		// Auction_Enrol = {(Mile_Money*25)+(시작가*0.2)}/2
		// iEnrolFee = (INT64)( (iValue*25 + (float)iStart*0.2f) /2 + 0.5f ) ;
		iEnrolFee = (INT64)GET_AUCTION_ENROLFEE_MILE(iStart,  iValue) ;
		break ;
	case AUCTION_TYPE_ELY_BY_CASH:			// 판매 : Ely,		구매수단 : Cash
	case AUCTION_TYPE_ELY_BY_MILEAGE:		// 판매 : Ely,		구매수단 : Mileage
		// Auction_Enrol = {(Game_Money*0.05)+(시작가*2)}/2
		// iEnrolFee = (INT64)( ((float)iValue*0.05f + iStart*2) /2 + 0.5f ) ;
		iEnrolFee = (INT64)GET_AUCTION_ENROLFEE_ELY(iStart,  iValue) ;
		break ;
	}
	// 실제 등록비용 = Auction_Enrol + (Auction_Enrol*Auction_Time*0.01)		//pTypeInfo->GetPeriodRate(iDuration) 
	// iRetFee = (INT64)(iEnrolFee + (iEnrolFee * pTypeInfo->GetPeriodRate(iDuration) * 0.01f) + 0.5f ) ;
	iRetFee = (INT64)GET_AUCTION_ENROLFEE(iEnrolFee, pTypeInfo->GetPeriodRate(iDuration)) ;
	return iRetFee ;
}

//------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
// Send Packet
/**
	Auction Search
	@param int iCategory	: Category Key
	@param int iLvMin	: 최소레벨
	@param int iLvMax	: 최대레벨
	@param int iQuality	: 품질
	@param const char* szText= NULL: 검색 이름
*/
void	SPAuctionManager::SendAuctionSearch(AUCTION_SEARCH_KEY* pSearchKey)
{
	AUCTION_SORT_KEY eSortKey ;
	int iSortAscend ;
	CPacket packet;

	m_pSearchArchive->GetLatestSortKey(eSortKey, iSortAscend) ;

	packet.AddUINT32( AUCTION_CS_SEARCH );	
	packet.AddUINT32( pSearchKey->iCategoryID );	
	packet.AddUINT8( eSortKey );
	packet.AddUINT8( iSortAscend );
	if( CHECK_AUCTION_ITEM_CATEGORY(pSearchKey->iCategoryID))
	{
		packet.AddData(pSearchKey->szName, MAX_ITEM_NAME) ;
		packet.AddUINT16(pSearchKey->iMinLevel) ;
		packet.AddUINT16(pSearchKey->iMaxLevel) ;
		packet.AddUINT8(pSearchKey->iQuality) ;
	}
	else
	{
		packet.AddUINT32(pSearchKey->iMoneyMin) ;
		packet.AddUINT32(pSearchKey->iMoneyMax) ;
	}

	m_iSearch = 1;
	SENDPACKET(packet);
}


/**
	Auction Search 다음 페이지 가져올때
	@param int iCategory	: Category Key
	@param int iWantIndex	: 원하는 시작 Index
	@param int iLvMin	: 최소레벨
	@param int iLvMax	: 최대레벨
	@param int iQuality	: 품질
	@param const char* szText= NULL: 검색 이름
*/
void	SPAuctionManager::SendAuctionSearchNext(int iWantIndex, AUCTION_SEARCH_KEY* pSearchKey)
{
	AUCTION_SORT_KEY eSortKey ;
	int iSortAscend ;
	CPacket packet;

	m_pSearchArchive->GetLatestSortKey(eSortKey, iSortAscend) ;

	packet.AddUINT32( AUCTION_CS_GETNEXT );	
	packet.AddUINT32( iWantIndex );	
	packet.AddUINT32( pSearchKey->iCategoryID );	
	packet.AddUINT8( eSortKey );
	packet.AddUINT8( iSortAscend );
	if( CHECK_AUCTION_ITEM_CATEGORY(pSearchKey->iCategoryID) )
	{
		packet.AddData(pSearchKey->szName, MAX_ITEM_NAME) ;
		packet.AddUINT16(pSearchKey->iMinLevel) ;
		packet.AddUINT16(pSearchKey->iMaxLevel) ;
		packet.AddUINT8(pSearchKey->iQuality) ;
	}
	else 
	{
		packet.AddUINT32(pSearchKey->iMoneyMin) ;
		packet.AddUINT32(pSearchKey->iMoneyMax) ;
	}

	m_iSearch = 0;
	SENDPACKET(packet);
}


/**
	Search 이후 Sort를 하는 경우 : 무조건 첫페이지로 간다. 
	@param int iCategory	: Category Key
	@param int iLvMin	: 최소레벨
	@param int iLvMax	: 최대레벨
	@param int iQuality	: 품질
	@param const char* szText= NULL: 검색 이름
*/
void	SPAuctionManager::SendAuctionSearchSort(AUCTION_SEARCH_KEY* pSearchKey)
{
	AUCTION_SORT_KEY eSortKey ;
	int iSortAscend ;
	CPacket packet;

	m_pSearchArchive->GetLatestSortKey(eSortKey, iSortAscend) ;

	packet.AddUINT32( AUCTION_CS_SORTING );	
	packet.AddUINT32( pSearchKey->iCategoryID );	
	packet.AddUINT8( eSortKey );
	packet.AddUINT8( iSortAscend );
	if( CHECK_AUCTION_ITEM_CATEGORY(pSearchKey->iCategoryID) )
	{
		packet.AddData(pSearchKey->szName, MAX_ITEM_NAME) ;
		packet.AddUINT16(pSearchKey->iMinLevel) ;
		packet.AddUINT16(pSearchKey->iMaxLevel) ;
		packet.AddUINT8(pSearchKey->iQuality) ;
	}
	else 	
	{
		packet.AddUINT32(pSearchKey->iMoneyMin) ;
		packet.AddUINT32(pSearchKey->iMoneyMax) ;
	}

	m_iSearch = 2;		// Sort의 경우에도 SearchList가 초기화 되어야함(Page포함)
	SENDPACKET(packet);
}




/**
	[입찰 현황] 요청
*/
void	SPAuctionManager::SendAuctionBuyRequest()
{
	m_pBuyArchive->ClearList() ;
	m_pBuyArchive->SetNeedReset(false) ;

	CPacket packet;
	packet.AddUINT32( AUCTION_CS_INFO_4BIDDER );

	SENDPACKET(packet);
}


/**
	[경매하기] 요청
*/
void	SPAuctionManager::SendAuctionSaleRequest()
{
	m_pSaleArchive->ClearList() ;
	m_pSaleArchive->SetNeedReset(false) ;

	CPacket packet;
	packet.AddUINT32( AUCTION_CS_INFO_4SELLER );	

	SENDPACKET(packet);
}



/**
	입찰
	@param int iSelectedTab
	@param INT64 iAuctionID
	@param INT64 iBidPrice	: 즉구의 경우 즉구가로 입력(즉구보다 높으면 즉구로 처리된다고함)
*/
void	SPAuctionManager::SendAuctionBidBuy(SPAuctionAttr* pAuctionAttr)
{
	CPacket packet;
	packet.AddUINT32( AUCTION_CS_BID );	
	packet.AddUINT64( pAuctionAttr->GetAuctionID() );
	packet.AddUINT32( pAuctionAttr->GetCategory() ) ;
	packet.AddUINT64( pAuctionAttr->GetNextBidPrice() );	

	SENDPACKET(packet);
}


/**
	즉시구매 요청
	:: 즉구시에는 SearchList를 검사해서 현재 즉구로 보고 있는것이 있는지 확인해야 한다.
	@param AUCTION_SEARCH_KEY* pSearchKey : Search업데이트를 위한 Key정보
	@param int iWantIndex	: Search가 현재 보고 있는 페이지
	@param INT64 iAuctionID	: Auction ID
	@param INT64 iSpotPrice	: 즉구 가격
 */
void	SPAuctionManager::SendAuctionSpotBuy(AUCTION_SEARCH_KEY* pSearchKey, int iWantIndex, SPAuctionAttr* pAuctionAttr)
{
	INT8	iRefresh = 0 ;
	AUCTION_SORT_KEY eSortKey ;
	int iSortAscend ;
	CPacket packet;

	if( pAuctionAttr == NULL )
		return ;

	m_pBuyArchive->GetLatestSortKey(eSortKey, iSortAscend) ;

	packet.AddUINT32( AUCTION_CS_BUYOUT );	
	packet.AddUINT64( pAuctionAttr->GetAuctionID() );
	packet.AddUINT32( pAuctionAttr->GetCategory() ) ;
	packet.AddUINT64( pAuctionAttr->GetSpotPrice() );	
	
	// Search에 해당 경매품목이 있으면 Update
	if( m_pSearchArchive->FindAuctionItem(pAuctionAttr->GetAuctionID()) )	{
		int iSearchPage = iWantIndex / 6 ;		// AUCTION_SEARCH_LIST ;
		if( m_pSearchArchive->IsPartLastPage(iSearchPage) == true )
			iRefresh = 1 ;
	}
	packet.AddUINT8( iRefresh );

	packet.AddUINT32( iWantIndex );
	packet.AddUINT32( pSearchKey->iCategoryID );
	packet.AddUINT8( eSortKey );
	packet.AddUINT8( iSortAscend );
	if( CHECK_AUCTION_ITEM_CATEGORY(pSearchKey->iCategoryID) )
	{
		packet.AddData(pSearchKey->szName, MAX_ITEM_NAME) ;
		packet.AddUINT16(pSearchKey->iMaxLevel) ;
		packet.AddUINT16(pSearchKey->iMaxLevel) ;
		packet.AddUINT8(pSearchKey->iQuality) ;
	}
	else 
	{
		packet.AddUINT32(pSearchKey->iMoneyMin) ;
		packet.AddUINT32(pSearchKey->iMoneyMax) ;
	}

	SENDPACKET(packet);
}




/**
	경매 아아템 등록 (Type is AUCTION_TYPE_ITEM_BY_ELY)
	@param INT64 iMoney : 판매 금액
	@param INT64 iBidMoney : 시작가(입찰가)
	@param INT64 iSpotMoney : 즉구가
	@param int iDuragon	: 경매기간 (AUCTION_DURATION) : packetID.h
*/
void	SPAuctionManager::SendAuctionSaleItem(SPItem* pItem, INT64 iStMoney, INT64 iSpotMoney, int iDuration, INT64 iEnrolFee)
{
	if( pItem == NULL )
		return ;
	AUCTION_TYPE_INFO* pAuctionTypeInfo = g_pkAuctionManager->GetAuctionTypeInfo(AUCTION_TYPE_ITEM_BY_ELY) ;
	if( pAuctionTypeInfo == NULL )
		return ;

	TRASH_ITEM	stConsumeItem;
	ITEMNO		iConsumeNo = 0;
	CONTAINER_SLOT	stItemSlot ;
	ITEMNO			iItemNo = 0;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive() ;
	if(pkInvenArchive == NULL)
		return ;

	if( pAuctionTypeInfo->iConsumeItem > 0 )	{
		SPItem* pSearchItem = pkInvenArchive->GetSPItem(INVENTORY_EVENT, pAuctionTypeInfo->iConsumeItem ) ;
		if( pSearchItem == NULL || pSearchItem->GetSlotIndex() < 0 ) {
			return ;
		}
		stConsumeItem.iContainerSlot.ContainerType = pSearchItem->GetContainerType() ;
		stConsumeItem.iContainerSlot.SlotIndex = pSearchItem->GetSlotIndex() + 1 ;
		stConsumeItem.iCount = 1 ;
		iConsumeNo = *(ITEMNO*)pSearchItem->GetItemStatus()->SPSendMessage(MV_ITEM_GET_ITEMNO);
	}
	else
	{
		stConsumeItem.iContainerSlot.ContainerType = CONTAINER_TYPE_INVALID ;
		stConsumeItem.iContainerSlot.SlotIndex = 0 ;
		stConsumeItem.iCount = 0 ;
	}

	stItemSlot.ContainerType = pItem->GetContainerType() ;
	stItemSlot.SlotIndex = pItem->GetSlotIndex() + 1;
	iItemNo = *(ITEMNO*)pItem->GetItemStatus()->SPSendMessage(MV_ITEM_GET_ITEMNO);

	CPacket packet;
	packet.AddUINT32( AUCTION_CS_CREATE );
	packet.AddUINT8((INT8)AUCTION_TYPE_ITEM_BY_ELY) ;
	packet.AddUINT8((INT8)iDuration) ;
	packet.AddUINT64(iStMoney) ;
	packet.AddUINT64(iSpotMoney) ;
	packet.AddUINT64(iEnrolFee) ;
	packet.AddData( &stConsumeItem, sizeof(TRASH_ITEM));
	packet.AddUINT64((UINT64)iConsumeNo);
	packet.AddData( &stItemSlot, sizeof(CONTAINER_SLOT));
	packet.AddUINT64((UINT64)iItemNo);

	SENDPACKET(packet);
}


/**
	경매 머니 등록
	@param int iType : 판매타입 (AUCTION_TYPE) : PaketID.h
	@param int iMoney : 판매 금액 (최대 9억)
	@param INT64 iBidMoney : 시작가(입찰가)
	@param INT64 iSpotMoney : 즉구가
	@param int iDuragon	: 경매기간 (AUCTION_DURATION) : packetID.h
*/
void	SPAuctionManager::SendAuctionSaleMoney(int iType, int iMoney, INT64 iStMoney, INT64 iSpotMoney, int iDuration, INT64 iEnrolFee)
{
	int iFeePoint = 0 ;
	AUCTION_TYPE_INFO* pAuctionTypeInfo = g_pkAuctionManager->GetAuctionTypeInfo((AUCTION_TYPE)iType) ;
	if( pAuctionTypeInfo == NULL )
		return ;
	
	TRASH_ITEM	stConsumeItem;
	ITEMNO		iConsumeNo = 0;
	AUCTION_GOODS_POINT	stSellPoint ;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive() ;
	if(pkInvenArchive == NULL)
		return ;

	if( pAuctionTypeInfo->iConsumeItem > 0 )	{
		SPItem* pSearchItem = pkInvenArchive->GetSPItem(INVENTORY_EVENT, pAuctionTypeInfo->iConsumeItem ) ;
		if( pSearchItem == NULL || pSearchItem->GetSlotIndex() < 0 ) {
			return ;
		}
		stConsumeItem.iContainerSlot.ContainerType = pSearchItem->GetContainerType() ;
		stConsumeItem.iContainerSlot.SlotIndex = pSearchItem->GetSlotIndex() + 1 ;
		stConsumeItem.iCount = 1 ;
		iConsumeNo = *(ITEMNO*)pSearchItem->GetItemStatus()->SPSendMessage(MV_ITEM_GET_ITEMNO);
	}
	else
	{
		stConsumeItem.iContainerSlot.ContainerType = CONTAINER_TYPE_INVALID ;
		stConsumeItem.iContainerSlot.SlotIndex = 0 ;
		stConsumeItem.iCount = 0 ;
	}

	//? 판매수수료는 어디서??
	stSellPoint.SellPoint = iMoney ;
	if( iType == AUCTION_TYPE_CASH_BY_ELY || iType == AUCTION_TYPE_MILEAGE_BY_ELY )	{
		SET_AUCTION_FEE(iMoney, GetAuctionFeeRate(), iFeePoint) ;
		stSellPoint.FeePoint = iFeePoint ;
	}
	else
		stSellPoint.FeePoint = 0 ;

	CPacket packet;
	packet.AddUINT32( AUCTION_CS_CREATE );
	packet.AddUINT8((INT8)iType) ;
	packet.AddUINT8((INT8)iDuration) ;
	packet.AddUINT64(iStMoney) ;
	packet.AddUINT64(iSpotMoney) ;
	packet.AddUINT64(iEnrolFee) ;
	packet.AddData( &stConsumeItem, sizeof(TRASH_ITEM));
	packet.AddUINT64((UINT64)iConsumeNo);
	packet.AddData( &stSellPoint, sizeof(AUCTION_GOODS_POINT));

	SENDPACKET(packet);
}


/**
	경매 등록 취소
	@param int iAuctionID
*/
void	SPAuctionManager::SendAuctionSaleCancel(SPAuctionAttr* pAuctionAttr)
{
	if( pAuctionAttr == NULL )
		return ;

	CPacket packet;
	packet.AddUINT32( AUCTION_CS_CANCEL );
	packet.AddUINT64( pAuctionAttr->GetAuctionID() );
	packet.AddUINT32( pAuctionAttr->GetCategory() ) ;

	SENDPACKET(packet);
}




//////////////////////////////////////////////////////////////////////////
// Recv Packet
/**
	[AUCTION_SC_SEARCH_INFO]
	INT32	iListCnt
	AUCTION_GOODS_ITEM	ctAuctionItem (...)
*/
void	SPAuctionManager::OnRecvAuctionSearchList(void* pPacket)
{
	CPacket* pRecvPacket = NULL ;
	INT32	iTotalCnt;		// 전체 갯수
	INT32	iStart;			// 시작위치
	INT8	iListCnt;		// 리스트 갯수
	SPAuctionAttr* pAuctionAttr ;
	AUCTION_HEADER		ctAuctionHeader ;
	AUCTION_GOODS_ITEM	ctAuctionItem ;
	AUCTION_GOODS_POINT ctAuctionPoint ;

	pRecvPacket = (CPacket*)pPacket ;
	if( pRecvPacket == NULL )
	{
		ErrorFLogs->Error_Log("#AuctionList::OnRecvAuctionSearchList is NULL") ;
		return ;
	}
	pRecvPacket->ExtractUINT32((UINT32*)&iTotalCnt);
	pRecvPacket->ExtractUINT32((UINT32*)&iStart);
	pRecvPacket->ExtractUINT8((UINT8*)&iListCnt);
	//ErrorFLogs->Error_Log("#AuctionList::Total[%d], Start[%d], Size [%d]", iTotalCnt, iStart, iListCnt) ;

	m_pSearchArchive->ClearList() ;
	m_pSearchArchive->SetAuctionList(AUCTION_ARCH_PART, 6, iTotalCnt, iStart, iListCnt) ;
	for(int i = 0; i < iListCnt; i++)
	{
		pAuctionAttr = new SPAuctionAttr ;
		pRecvPacket->ExtractStruct(&ctAuctionHeader, sizeof(AUCTION_HEADER));

		// Auction Type별로 패킷을 저장
		if( ctAuctionHeader.iType == AUCTION_TYPE_ITEM_BY_ELY )	{
			pRecvPacket->ExtractStruct(&ctAuctionItem, sizeof(AUCTION_GOODS_ITEM));
			pAuctionAttr->SetAuctionAttr(&ctAuctionHeader, &ctAuctionItem) ;
		}
		else	{
			pRecvPacket->ExtractStruct(&ctAuctionPoint, sizeof(AUCTION_GOODS_POINT));
			pAuctionAttr->SetAuctionAttr(&ctAuctionHeader, &ctAuctionPoint) ;
		}

		m_pSearchArchive->AddAuctionList(pAuctionAttr) ;
	}

	// 새로 검색했으면 SortKey도 재설정 해준다.
	if( m_iSearch == 1 )		
		m_pSearchArchive->SetDefaultSort(false) ;
	// Interface Window Update Message
	SPWindow* pWindowAuction = g_pInterfaceManager->GetAuctionWindow() ;
	if(pWindowAuction && pWindowAuction->IsShow()) {
		pWindowAuction->SPSendMessage(SPIM_AUCTION_SEARCH_LIST, (WPARAM)m_iSearch, (LPARAM)m_iSearch);
		m_iSearch = 0 ;
	}
}


/**
	[AUCTION_SC_BIDLIST]
	나의 입찰 목록을 받는다
	입찰 목록은 Client에서 관리된다.
*/
void	SPAuctionManager::OnRecvAuctionBuyList(void* pPacket)
{
	CPacket* pRecvPacket = NULL ;
	UINT8	iListCnt;							// 리스트 갯수
	SPAuctionAttr* pAuctionAttr ;
	AUCTION_HEADER		ctAuctionHeader ;
	AUCTION_GOODS_ITEM	ctAuctionItem ;
	AUCTION_GOODS_POINT ctAuctionPoint ;


	pRecvPacket = (CPacket*)pPacket ;
	if( pRecvPacket == NULL )
	{
		ErrorFLogs->Error_Log("#AuctionList::OnRecvAuctionBuyList is NULL") ;
		return ;
	}
	pRecvPacket->ExtractUINT8((UINT8*)&iListCnt);
	//ErrorFLogs->Error_Log("#AuctionList::Size [%d]", iListCnt) ;

	//m_pBuyArchive->ClearList() ;
	for(int i = 0; i < iListCnt; i++)
	{
		pAuctionAttr = new SPAuctionAttr ;
		pRecvPacket->ExtractStruct(&ctAuctionHeader, sizeof(AUCTION_HEADER));

		// Auction Type별로 패킷을 저장
		if( ctAuctionHeader.iType == AUCTION_TYPE_ITEM_BY_ELY )	{
			pRecvPacket->ExtractStruct(&ctAuctionItem, sizeof(AUCTION_GOODS_ITEM));
			pAuctionAttr->SetAuctionAttr(&ctAuctionHeader, &ctAuctionItem) ;
		}
		else	{
			pRecvPacket->ExtractStruct(&ctAuctionPoint, sizeof(AUCTION_GOODS_POINT));
			pAuctionAttr->SetAuctionAttr(&ctAuctionHeader, &ctAuctionPoint) ;
		}

		m_pBuyArchive->AddAuctionList(pAuctionAttr) ;
	}
}


/**
	[AUCTION_SC_SELLLIST]
	나의 판매 목록을 받는다
	판매 목록은 Client에서 관리된다.
*/
void	SPAuctionManager::OnRecvAuctionSaleList(void* pPacket)
{
	CPacket* pRecvPacket = NULL ;
	UINT8	iListCnt;							// 리스트 갯수
	SPAuctionAttr* pAuctionAttr ;
	AUCTION_HEADER		ctAuctionHeader ;
	AUCTION_GOODS_ITEM	ctAuctionItem ;
	AUCTION_GOODS_POINT ctAuctionPoint ;

	pRecvPacket = (CPacket*)pPacket ;
	if( pRecvPacket == NULL )
	{
		ErrorFLogs->Error_Log("#AuctionList::OnRecvAuctionSaleList is NULL") ;
		return ;
	}
	pRecvPacket->ExtractUINT8((UINT8*)&iListCnt);
	//ErrorFLogs->Error_Log("#AuctionList::Size [%d]", iListCnt) ;

	//m_pSaleArchive->ClearList() ;
	for(int i = 0; i < iListCnt; i++)
	{
		pAuctionAttr = new SPAuctionAttr ;
		pRecvPacket->ExtractStruct(&ctAuctionHeader, sizeof(AUCTION_HEADER));

		// Auction Type별로 패킷을 저장
		if( ctAuctionHeader.iType == AUCTION_TYPE_ITEM_BY_ELY )	{
			pRecvPacket->ExtractStruct(&ctAuctionItem, sizeof(AUCTION_GOODS_ITEM));
			pAuctionAttr->SetAuctionAttr(&ctAuctionHeader, &ctAuctionItem) ;
		}
		else	{
			pRecvPacket->ExtractStruct(&ctAuctionPoint, sizeof(AUCTION_GOODS_POINT));
			pAuctionAttr->SetAuctionAttr(&ctAuctionHeader, &ctAuctionPoint) ;
		}

		m_pSaleArchive->AddAuctionList(pAuctionAttr) ;
	}
}


/**
	[AUCTION_SC_OPEN]
	경매서버의 Reset으로 Archive의 Reset이 필요한 경우
 */
void	SPAuctionManager::OnRecvAuctioinOpen()
{
	if( m_pSaleArchive )
		m_pSaleArchive->SetNeedReset(true) ;
	if( m_pBuyArchive )
		m_pBuyArchive->SetNeedReset(true) ;
	SetServerOpen(true) ;

	if( g_pResourceManager->GetGlobalString(52000085))
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(52000085));
	else	{
		TCHAR szMesg[64] ;
		sprintf(szMesg, "Auction Server is Open") ;
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMesg) ;
	}
}


/**
	[AUCTION_SC_CREATE]
	등록 결과
 */
void	SPAuctionManager::OnRecvAuctionCreate(void* pPacket)
{
	CPacket* pRecvPacket = NULL ;
	SPAuctionAttr* pAuctionAttr ;
	AUCTION_HEADER		ctAuctionHeader ;
	AUCTION_GOODS_ITEM	ctAuctionItem ;
	AUCTION_GOODS_POINT ctAuctionPoint ;
	UINT64				iPlayerEly = 0;
	TRASH_ITEM			ctTrashItem ;
	TRASH_ITEM			ctUploadItem ;
	CHARACTER_CASH_INFO ctCashInfo ;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return ;

	pRecvPacket = (CPacket*)pPacket ;
	pAuctionAttr = new SPAuctionAttr ;
	pRecvPacket->ExtractStruct(&ctAuctionHeader, sizeof(AUCTION_HEADER));

	// Auction Type별로 패킷을 저장
	if( ctAuctionHeader.iType == AUCTION_TYPE_ITEM_BY_ELY )	{
		pRecvPacket->ExtractStruct(&ctAuctionItem, sizeof(AUCTION_GOODS_ITEM));
		pAuctionAttr->SetAuctionAttr(&ctAuctionHeader, &ctAuctionItem) ;
	}
	else	{
		pRecvPacket->ExtractStruct(&ctAuctionPoint, sizeof(AUCTION_GOODS_POINT));
		pAuctionAttr->SetAuctionAttr(&ctAuctionHeader, &ctAuctionPoint) ;
	}
	if( m_pSaleArchive->AddAuctionList(pAuctionAttr) == false )
		SAFE_DELETE(pAuctionAttr) ;

	// Ely 정보 업데이트
	pRecvPacket->ExtractUINT64(&iPlayerEly);
	UpdatePlayerEly(iPlayerEly) ;

	// 등록에 소비되는 등록아이템 차감
	pRecvPacket->ExtractStruct( &ctTrashItem , sizeof( TRASH_ITEM ) );
	ctTrashItem.iContainerSlot.SlotIndex -= 1;
	pkInvenArchive->TrashItem(ctTrashItem);

	// 등록한 아이템 또는 머니 정보 차감
	// Auction Type별로 패킷을 저장
	if( ctAuctionHeader.iType == AUCTION_TYPE_ITEM_BY_ELY )	{
		pRecvPacket->ExtractStruct( &ctUploadItem , sizeof( TRASH_ITEM ) );
		ctUploadItem.iContainerSlot.SlotIndex -= 1;
		pkInvenArchive->TrashItem(ctUploadItem);
	}
	else	{
		// Ely & Cash 정보 업데이트
		pRecvPacket->ExtractStruct(&ctCashInfo, sizeof(CHARACTER_CASH_INFO));
		UpdatePlayerCash(ctCashInfo.iCash, ctCashInfo.iMile) ;
	}
	
	// Interface Window Update Message
	SPWindow* pWindowAuction = g_pInterfaceManager->GetAuctionWindow() ;
	if(pWindowAuction && pWindowAuction->IsShow()) {
		pWindowAuction->SPSendMessage(SPIM_AUCTION_SALE_LIST, (WPARAM)0, (LPARAM)0);
		//pWindowAuction->SPSendMessage(SPIM_AUCTION_MONEY_UPDATE, (WPARAM)0, (LPARAM)0);
	}
	// 등록 OK Message
	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(52000016));
}



/**
	[AUCTION_SC_FINISH]
	취소 결과(낙찰, 유찰, 취소시: 판매자, 입찰자 -> 즉구는 별도처리)	
	@param void* pPacket
 */
void	SPAuctionManager::OnRecvAuctionFinish(void* pPacket)
{
	CPacket* pRecvPacket = NULL ;
	UINT64				iAuctionID = 0;
	CHARACTER_CASH_INFO ctCashInfo ;

	pRecvPacket = (CPacket*)pPacket ;
	pRecvPacket->ExtractUINT64(&iAuctionID);

	// Cash 정보 업데이트
	pRecvPacket->ExtractStruct(&ctCashInfo, sizeof(CHARACTER_CASH_INFO));
	UpdatePlayerCash(ctCashInfo.iCash, ctCashInfo.iMile) ;

	// 해당 물품 삭제 후, 삭제된것이 있다면 보고있는 WindowUpdate
	int iTab = UpdateAuctionFinish(iAuctionID, false) ;
	if( iTab > 0 )	{
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_AUCTION);
		if(pWindow && pWindow->IsShow()) {
			pWindow->SPSendMessage(SPIM_AUCTION_BUY_LIST, (WPARAM)0, (LPARAM)0);
			pWindow->SPSendMessage(SPIM_AUCTION_SALE_LIST, (WPARAM)0, (LPARAM)0);
		}
	}
}


/**
	[AUCTION_SC_FINISH_SELL]
	낙찰완료
	//		- 낙찰시 판매자/낙찰자에게
	//		- 즉구시 판매자에게	
	@param void* pPacket
 */
void	SPAuctionManager::OnRecvAuctionFinishSell(void* pPacket)
{
	CPacket* pRecvPacket = NULL ;
	UINT64			iAuctionID = 0;
	USERID			iBuyerID;							// 구매자
	CHARACTER_CASH_INFO ctCashInfo ;
	D3DXCOLOR color( 0.62745f , 1.0f, 0.0f, 1.0f);
	TCHAR			szMessage[256] ;
	SPAuctionAttr* pAuctionAttr = NULL ;

	pRecvPacket = (CPacket*)pPacket ;
	pRecvPacket->ExtractUINT64(&iAuctionID);
	pRecvPacket->ExtractUINT32(&iBuyerID);

	// Cash 정보 업데이트
	pRecvPacket->ExtractStruct(&ctCashInfo, sizeof(CHARACTER_CASH_INFO));
	UpdatePlayerCash(ctCashInfo.iCash, ctCashInfo.iMile) ;
	
	if( iBuyerID == g_pNetworkManager->GetUserID() )	{
		// 낙찰 받음 메시지 [52000005]
		pAuctionAttr = m_pBuyArchive->FindAuctionItem(iAuctionID) ;
		if( pAuctionAttr )	{
			_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000005), pAuctionAttr->GetItemName()) ;
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMessage);
			g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMessage , (LPARAM)&color );
		}
	}
	else {
		// 판매 완료 메시지 [52000094]
		pAuctionAttr = m_pSaleArchive->FindAuctionItem(iAuctionID) ;
		if( pAuctionAttr )	{
			_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000094), pAuctionAttr->GetItemName()) ;
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMessage);
			g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMessage , (LPARAM)&color );
		}
	}

	// 해당 물품 삭제 후, 삭제된것이 있다면 보고있는 WindowUpdate
	int iTab = UpdateAuctionFinish(iAuctionID, false) ;
	if( iTab > 0 )	{
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_AUCTION);
		if(pWindow && pWindow->IsShow()) {
			pWindow->SPSendMessage(SPIM_AUCTION_BUY_LIST, (WPARAM)0, (LPARAM)0);
			pWindow->SPSendMessage(SPIM_AUCTION_SALE_LIST, (WPARAM)0, (LPARAM)0);
		}
	}
}


/**
	[AUCTION_SC_CANCEL]
	취소 결과(등록자)
	@param void* pPacket
 */
void	SPAuctionManager::OnRecvAuctionCancel(void* pPacket)
{
	CPacket* pRecvPacket = NULL ;
	UINT64				iAuctionID = 0;
	CHARACTER_CASH_INFO ctCashInfo ;

	pRecvPacket = (CPacket*)pPacket ;
	pRecvPacket->ExtractUINT64(&iAuctionID);

	// Cash 정보 업데이트
	pRecvPacket->ExtractStruct(&ctCashInfo, sizeof(CHARACTER_CASH_INFO));
	UpdatePlayerCash(ctCashInfo.iCash, ctCashInfo.iMile) ;

	// 해당 물품 삭제 후, 삭제된것이 있다면 보고있는 WindowUpdate
	int iTab = UpdateAuctionFinish(iAuctionID, true) ;
	if( iTab > 0 )	{
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_AUCTION);
		if(pWindow && pWindow->IsShow()) {
			//pWindow->SPSendMessage(SPIM_AUCTION_BUY_LIST, (WPARAM)0, (LPARAM)0);
			pWindow->SPSendMessage(SPIM_AUCTION_SALE_LIST, (WPARAM)0, (LPARAM)0);
		}
	}
	if( g_pResourceManager->GetGlobalString(52000006))		// 경매 취소 메시지
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(52000006));
}




/**
	[AUCTION_SC_BUYOUT]
	즉시구매시 입찰자만 받음
 */
void	SPAuctionManager::OnRecvAuctionBuyOut(void* pPacket)
{
	CPacket* pRecvPacket = NULL ;
	UINT64				iAuctionID = 0;
	UINT64				iPlayerEly = 0;
	CHARACTER_CASH_INFO ctCashInfo ;
	D3DXCOLOR		color( 0.62745f , 1.0f, 0.0f, 1.0f);
	TCHAR			szMessage[256] ;
	SPAuctionAttr*	pAuctionAttr = NULL ;

	pRecvPacket = (CPacket*)pPacket ;
	pRecvPacket->ExtractUINT64(&iAuctionID);

	// Ely 정보 업데이트
	pRecvPacket->ExtractUINT64(&iPlayerEly);
	UpdatePlayerEly(iPlayerEly) ;

	// Cash 정보 업데이트
	pRecvPacket->ExtractStruct(&ctCashInfo, sizeof(CHARACTER_CASH_INFO));
	UpdatePlayerCash(ctCashInfo.iCash, ctCashInfo.iMile) ;

	// 낙찰 받음 메시지 [52000005]
	pAuctionAttr = m_pBuyArchive->FindAuctionItem(iAuctionID) ;
	if( pAuctionAttr == NULL )
		pAuctionAttr = m_pSearchArchive->FindAuctionItem(iAuctionID) ;
	if( pAuctionAttr )	{
		_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000005), pAuctionAttr->GetItemName()) ;
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMessage);
		g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMessage , (LPARAM)&color );
	}

	// 해당 물품 삭제 후, 삭제된것이 있다면 보고있는 WindowUpdate
	int iTab =  UpdateAuctionFinish(iAuctionID, true) ;
	if( iTab > 0 )	{
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_AUCTION);
		if(pWindow && pWindow->IsShow()) {
			pWindow->SPSendMessage(SPIM_AUCTION_SEARCH_LIST, (WPARAM)0, (LPARAM)1);
			pWindow->SPSendMessage(SPIM_AUCTION_BUY_LIST, (WPARAM)0, (LPARAM)0);
			pWindow->SPSendMessage(SPIM_AUCTION_SALE_LIST, (WPARAM)0, (LPARAM)0);
		}
	}
}



/**
	[AUCTION_SC_BID]
	입찰 결과
	>> 현재 입찰자	: 입찰 정보 추가
	>> 판매자		: 판매 정보 갱신
	>> 이전입찰자	: 입찰 정보 갱신
 */
void	SPAuctionManager::OnRecvAuctionBidUpdate(void* pPacket)
{
	CPacket* pRecvPacket = NULL ;
	SPAuctionAttr* pAuctionAttr = NULL ;
	USERID			iMyUserID = 0 ;
	AUCTION_HEADER		ctAuctionHeader ;
	AUCTION_GOODS_ITEM	ctAuctionItem ;
	AUCTION_GOODS_POINT ctAuctionPoint ;
	UINT64				iPlayerEly = 0;
	CHARACTER_CASH_INFO ctCashInfo ;
	TCHAR				szItemName[MAX_ITEM_NAME] ;
	TCHAR				szMessage[256] ;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return ;

	iMyUserID = g_pNetworkManager->GetUserID() ;

	pRecvPacket = (CPacket*)pPacket ;
	pAuctionAttr = new SPAuctionAttr ;
	pRecvPacket->ExtractStruct(&ctAuctionHeader, sizeof(AUCTION_HEADER));

	// Auction Type별로 패킷을 저장
	if( ctAuctionHeader.iType == AUCTION_TYPE_ITEM_BY_ELY )	{
		pRecvPacket->ExtractStruct(&ctAuctionItem, sizeof(AUCTION_GOODS_ITEM));
		pAuctionAttr->SetAuctionAttr(&ctAuctionHeader, &ctAuctionItem) ;
	}
	else	{
		pRecvPacket->ExtractStruct(&ctAuctionPoint, sizeof(AUCTION_GOODS_POINT));
		pAuctionAttr->SetAuctionAttr(&ctAuctionHeader, &ctAuctionPoint) ;
	}
	_stprintf(szItemName, "%s", pAuctionAttr->GetItemName() ) ;
	
	// Ely 정보 업데이트
	pRecvPacket->ExtractUINT64(&iPlayerEly);
	UpdatePlayerEly(iPlayerEly) ;
	// Ely & Cash 정보 업데이트
	pRecvPacket->ExtractStruct(&ctCashInfo, sizeof(CHARACTER_CASH_INFO));
	UpdatePlayerCash(ctCashInfo.iCash, ctCashInfo.iMile) ;


	SPWindow* pWindow = g_pInterfaceManager->GetAuctionWindow() ;
	SPWindow* pWindowMsg = g_pInterfaceManager->GetAuctionMsgWindow() ;
	//pWindow->SPSendMessage(SPIM_AUCTION_MONEY_UPDATE, (WPARAM)0, (LPARAM)0);
	// 판매자의 경우 -> 판매 정보 갱신
	if( ctAuctionHeader.iSellerID == iMyUserID )	{
		m_pSaleArchive->UpdateAuctionArchive(ctAuctionHeader.iAuctionID, &ctAuctionHeader) ;
		if(pWindow && pWindow->IsShow()) {
			pWindow->SPSendMessage(SPIM_AUCTION_SALE_LIST, (WPARAM)0, (LPARAM)0);
			// 검색에 있으면 갱신
			if( m_pSearchArchive->UpdateAuctionArchive(ctAuctionHeader.iAuctionID, &ctAuctionHeader) )	
				pWindow->SPSendMessage(SPIM_AUCTION_SEARCH_LIST, (WPARAM)0, (LPARAM)0);
		}
		// 새로운 입찰자가 발생했습니다.
		if( g_pResourceManager->GetGlobalString(52000007))	
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(52000007));
		SAFE_DELETE(pAuctionAttr) ;
	}
	// 현재 입찰자 : 입찰 정보 추가 & Search 갱신
	else if (ctAuctionHeader.iCurrUserID == iMyUserID )	{
		pWindowMsg->SPSendMessage(SPIM_AUCTION_WAIT_CLOSE, 0, 0) ;
		if( m_pBuyArchive->AddAuctionList(pAuctionAttr, true) == false )
			SAFE_DELETE(pAuctionAttr) ;
		if(pWindow && pWindow->IsShow()) {
			pWindow->SPSendMessage(SPIM_AUCTION_BUY_LIST, (WPARAM)0, (LPARAM)0);
			// 검색에 있으면 갱신
			if( m_pSearchArchive->UpdateAuctionArchive(ctAuctionHeader.iAuctionID, &ctAuctionHeader) )
				pWindow->SPSendMessage(SPIM_AUCTION_SEARCH_LIST, (WPARAM)0, (LPARAM)0);
		}
		// %s 경매 물품에 입찰하였습니다.
		if( g_pResourceManager->GetGlobalString(52000003))	{
			_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000003), szItemName) ;
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMessage);
		}
	}
	// 이전 입찰자 : 입찰 정보 갱신
	else	{
		m_pBuyArchive->UpdateAuctionArchive(ctAuctionHeader.iAuctionID, &ctAuctionHeader) ;
		if(pWindow && pWindow->IsShow()) {
			pWindow->SPSendMessage(SPIM_AUCTION_BUY_LIST, (WPARAM)0, (LPARAM)0);
			// 검색에 있으면 갱신
			if( m_pSearchArchive->UpdateAuctionArchive(ctAuctionHeader.iAuctionID, &ctAuctionHeader) )
				pWindow->SPSendMessage(SPIM_AUCTION_SEARCH_LIST, (WPARAM)0, (LPARAM)0);
		}
		// %s 물품이 상회입찰 되었습니다. (Chat & SystemMessage)
		if( g_pResourceManager->GetGlobalString(52000004))	{
			_stprintf(szMessage, g_pResourceManager->GetGlobalString(52000004), szItemName) ;
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMessage);
			D3DXCOLOR color( 0.62745f , 1.0f, 0.0f, 1.0f);
			g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMessage , (LPARAM)&color );
		}
		SAFE_DELETE(pAuctionAttr) ;
	}
}



/**
	[AUCTION_SC_BIDDEL]
	전전 입찰자의 경우 상회입찰시 입찰정보를 지워야 한다.
 */
void	SPAuctionManager::OnRecvAuctionBidDelete(void* pPacket)
{
	CPacket* pRecvPacket = NULL ;
	UINT64	iAuctionID = 0;
	SPWindow* pWindow = g_pInterfaceManager->GetAuctionWindow() ;

	pRecvPacket = (CPacket*)pPacket ;
	pRecvPacket->ExtractUINT64(&iAuctionID);

	m_pBuyArchive->DeleteAuctionItem(iAuctionID, true) ;
	if(pWindow && pWindow->IsShow()) {
		pWindow->SPSendMessage(SPIM_AUCTION_BUY_LIST, (WPARAM)0, (LPARAM)0);
	}
}



