// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPAuctionArchive.h
//	created:	2008/2/26   Time:10:59
//	purpose:	
//*********************************************************************/

#pragma once 

enum AUCTION_ARCH_DATA ;
class SPAuctionAttr ;
class SPAuctionArchive 
{
public:
	SPAuctionArchive();
	~SPAuctionArchive();

	void Init();
	void Clear();
	void ClearList();

	bool SetAuctionList(AUCTION_ARCH_DATA eType, int iOnePage = 7, int iTotalCount = 0, int iStart = 0, int iEnd = 0) ;
	bool AddAuctionList(SPAuctionAttr* pAuctionItem, bool bCount = false) ;			// 추가
	bool ChangeAuctionList(int iPage, int iIndex, SPAuctionAttr* pAuctionItem) ;	// 변경
	bool UpdateAuctionArchive(INT64 iAuctionID, void* pAuctionHeader = NULL) ;		// 변경
	bool DeleteAuctionItem(INT64 iAuctionID, bool bCount = false) ;					// 삭제
	

	SPAuctionAttr* GetAuctionItem(int iPage, int iIndex) ;
	SPAuctionAttr* FindAuctionItem(INT64 iAuctionID) ;
	int GetAuctionListPage(std::vector<SPAuctionAttr*>& vAuctionList, int iPage) ;		// 페이지 단위로 가져온다

	int GetListCount() ;				// Archive의 Size를 가져온다
	int GetTotalPageCount() ;			// 리스트의 페이지 갯수를 가져온다.
	bool HasPageInfo(int iPage) ;		// 해당 페이지의 정보가 Archive에 있는지 확인
	bool IsPartLastPage(int iPage) ;	// PART의 마지막 유효 페이지가 업데이트 되어야 하는지 여부 체크

	void SetDefaultSort(bool bReal=true) ;			// Archive의 기본 Sort룰 정의
	bool SetSort(AUCTION_SORT_KEY eSortKey, bool bReal= true) ;		// SortKey로 Archive를 Sort
	void GetLatestSortKey(AUCTION_SORT_KEY& eSortKey, int& iAscend) ;
	int  GetSortAscendKey(AUCTION_SORT_KEY eSortKey) ;

	bool GetNeedReset()						{	return m_bNeedReset ;	}		// 최초, 서버리셋시 설정됨
	void SetNeedReset(bool bReset)			{	m_bNeedReset = bReset ;	}		// 최초, 서버리셋시 설정됨
private:



private:
	bool		m_bNeedReset ;				// 리스를 받아야 하는 경우
	AUCTION_ARCH_DATA	m_iArchiveType ;	// 전체를 관리하는 경우와 일부를 관리하는 경우
	int			m_iTotalItemCount ;
	int			m_iStartIndex ;		// PART 타입인경우 사용
	int			m_iHaveSize ;		// PART 타입인경우 사용
	int			m_iOnePage ;		// OnePage Size
	bool		m_bIsLast ;			// PART의 마지막 데이터 별도 처리
	AUCTION_SORT_KEY m_eSortKey ;	// SortKey
	int			m_iSortAscend[AUCTION_SORT_MAX] ;		// 0:Descend, 1:Ascend
	std::vector<SPAuctionAttr*>		m_vpAuctionList;
};