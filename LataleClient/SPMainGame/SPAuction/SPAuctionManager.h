// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPAuctionManager.h
//	created:	2008/2/26   Time:11:00
//	purpose:	
//*********************************************************************/

#pragma once 

struct AUCTION_SEARCH_KEY ;
struct AUCTION_TYPE_INFO ;
class SPAuctionAttr ;
class SPAuctionArchive ;
class SPItem ;
class SPAuctionManager
{
public:

	static SPAuctionManager* GetInstance();
	static void Release();

	bool	Init();
	void	Clear();
	void	Process(float fTime);

	SPAuctionArchive* GetAuctionSearchArchive() ;
	SPAuctionArchive* GetAuctionBuyArchive() ;
	SPAuctionArchive* GetAuctionSaleArchive() ;

	bool	CheckAuctionValid() ;					// Auction 동작에 필요한 Valid 체크
	bool	CheckArchiveReset(int iTab) ;			// Archive 리셋이 필요한지 체크
	void	SetAuctionReset() ;						// Player변경시 Archive Reset
	void	SetServerOpen(bool bOpen) ;				// Set Auction Server Open
	bool	GetServerOpen() ;						// Get Auction Server Open

	// Packet
	void	SendAuctionSearch(AUCTION_SEARCH_KEY* pSearchKey) ;						// 검색
	void	SendAuctionSearchNext(int iWantIndex, AUCTION_SEARCH_KEY* pSearchKey) ;	// 다음
	void	SendAuctionSearchSort(AUCTION_SEARCH_KEY* pSearchKey) ;					// Sort
	void	SendAuctionBuyRequest() ;				// 입찰현황 리스트 요청
	void	SendAuctionSaleRequest() ;				// 경매하기(등록) 리스트 요청
	void	SendAuctionBidBuy(SPAuctionAttr* pAuctionAttr) ;				// 입찰
	void	SendAuctionSpotBuy(AUCTION_SEARCH_KEY* pSearchKey, int iWantIndex, SPAuctionAttr* pAuctionAttr) ;	// 즉시구매
	void	SendAuctionSaleItem(SPItem* pItem, INT64 iStMoney, INT64 iSpotMoney, int iDuration, INT64 iEnrolFee) ;			// 아이템 등록
	void	SendAuctionSaleMoney(int iType, int iMoney, INT64 iStMoney, INT64 iSpotMoney, int iDuration, INT64 iEnrolFee) ;	// 머니 등록
	void	SendAuctionSaleCancel(SPAuctionAttr* pAuctionAttr) ;		// 등록 취소

	void	OnRecvAuctionSearchList(void* pPacket) ;	// Search 결과
	void	OnRecvAuctionBuyList(void* pPacket) ;		// 입찰 목록
	void	OnRecvAuctionSaleList(void* pPacket) ;		// 판매 목록
	void	OnRecvAuctioinOpen() ;						// 경매서버 Reset
	void	OnRecvAuctionCreate(void* pPacket) ;		// 등록 결과
	void	OnRecvAuctionFinish(void* pPacket) ;		// 취소 결과(유찰, 취소시:입찰, 판매자)
	void	OnRecvAuctionFinishSell(void* pPacket) ;	// 낙찰 결과(낙찰시:입찰, 판매자)
	void	OnRecvAuctionCancel(void* pPacket) ;		// 취소 결과(등록자)
	void	OnRecvAuctionBuyOut(void* pPacket) ;		// 즉구 결과(입찰자)
	void	OnRecvAuctionBidUpdate(void* pPacket) ;		// 입찰 결과(입찰, 판매, 이전입찰자)
	void	OnRecvAuctionBidDelete(void* pPacket) ;		// 입찰 결과(전전입찰자)


	//////////////////////////////////////////////////////////////////////////
	// for Auction Type
	AUCTION_TYPE_INFO*	GetAuctionTypeInfo(int iAuctionTypeID) ;
	int		GetAuctionTypeID(int iIndex);		// 설정된 Type값 얻어오기
	int		GetAuctionTypeCount() ;
	int		GetAuctionFeeRate() ;		// 경매 수수료
	int		GetAuctionTimeShort() ;		// 경매 시간(단기)
	int		GetAuctionTimeMiddle() ;	// 경매 시간(중기)
	int		GetAuctionTimeLong() ;		// 경매 시간(최장기)
	int		GetSysMoneyID() ;
	int		GetSysGamePriceID() ;
	int		GetSysStandardPriceID() ;
	int		GetSysStartMoneyID() ;
	INT64	GetPlayerEly() ;		// Player Ely 정보
	int		GetPlayerCash() ;		// Player Cash 정보
	int		GetPlayerMilige() ;		// Player Milige 정보
	void	UpdatePlayerEly(INT64 iEly) ;					// PlayerStatus 정보 업데이트
	void	UpdatePlayerCash(int iCash, int iMilige) ;		// PlayerStatus 정보 업데이트
	INT64	ComputeEnrolFee(AUCTION_TYPE eType, INT64 iStart, int iDuration, int iValue, int iLv=0, int iGamePrice=0, int iGameCount=0);	// 등록비용		
	int		UpdateAuctionFinish(INT64 iAuctionID, bool bSearch) ;		// 삭제후 삭제된Archive 수 Return
	void	SetDefaultSort(int iArchiveType) ;				// Archive Sort Reset (0:Buy, 1:Sale)

protected:
	SPAuctionManager();
	virtual ~SPAuctionManager();

	bool LoadAuctionTypeLDT();


protected:
	static SPAuctionManager*	m_pkInstance;

	SPAuctionArchive*	m_pSearchArchive ;		// 검색 목록
	SPAuctionArchive*	m_pBuyArchive ;			// 입찰 목록
	SPAuctionArchive*	m_pSaleArchive ;		// 판매 목록

	bool				m_bOpen ;				// 경매장 오픈 여부
	bool				m_bValid;				// 경매장 오픈 여부
	int					m_iSearch ;				// Search여부 (0:GetNext, 1:Search, 2:Sort))
	int					m_iAuctionTypeCount ;
	int					m_iSysAuctionMoneyID ;	// GlobalSys의 설정값
	int					m_iSysGamePriceID ;		// GlobalSys의 설정값
	int					m_iSysStandardPriceID ;	// GlobalSys의 설정값
	int					m_iSysStartMoneyID ;	// GlobalSys의 설정값(시작가)
	AUCTION_TYPE_INFO*	m_ctAuactionType[5] ;
	bool				m_bTest ;				// 테스트 데이터 처리여부

};

extern SPAuctionManager* g_pkAuctionManager;
