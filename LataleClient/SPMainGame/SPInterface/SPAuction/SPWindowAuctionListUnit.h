// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowAuctionRegist.h
//	created:	2008/2/26   Time:10:59
//	purpose:	
//*********************************************************************/


#pragma once

#include "PacketID.h"

enum AUCTION_BID_TYPE	{
	BID_ALLENABLE = 0,	// 입찰 가능
	BID_ENABLE,			// 시작가만 있으면 즉구 불가
	BID_SELF,			// 내입찰물품 (즉구 가능)
	BID_OWN,			// 내 등록 물품
	BID_OVER,			// 상회입찰 물품
	BID_DISABLE,		// 내입찰에 시작가만 있는경우(모두 불가)
} ;

class SPItem ;
class SPAuctionAttr ;
class SPWindowAuctionListUNIT : public SPWindow
{
public:
	SPWindowAuctionListUNIT( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual	~SPWindowAuctionListUNIT();

	virtual	void	Init();
	virtual	void	Clean();
	virtual void	Show(bool bWithChild = true);
	virtual void	Hide(bool bSendServer = true);

	virtual	void	Process( float fTime );
	virtual	void	Render( float fTime );
	virtual void	RefreshRelationCoord();

	void			SetAuctionItem( SPAuctionAttr* pAuctionAttr ) ;			// SPAuctionAttr 셋팅
	void			ClearAuctionInfo() ;						// Auction 정보 초기화
	void			SetItemField( SPItem* pItem) ;				// Item
	void			SetItemField( int iType, INT64 iMoney ) ;	// Oz, Point, Ely
	

	int				GetAuctionType()				{	return m_iAuctionType ;		}
	INT64			GetAuctionID()					{	return m_iAuctionID;		}
	SPAuctionAttr*	GetAuctionAttr()				{	return m_pAuctionAttr;		}

	void			SetLevel( int iLevel) ;					// 경매 레벨 설정
	void			SetDuration( int iDuration) ;			// 내구도 설정
	void			SetFinishTime() ;						// 마감시간
	void			SetSeller( const char* szSeller, USERID iSellUser) ;					// 판매자
	void			SetPrice( INT64 iBidPrice, INT64 iSpotPrice, USERID iBidUser ) ;		// 경매 입찰가 설정
	void			SetOutBid() ;				// [상회입찰] 표시
	void			UpdateBidButtonEnable() ;

	void			SetSelectBG( char* pTextureFile, RECT* prcSrc ) ;		// 선택 박스 영역설정
	void			SetWndState( WND_STATE eWndState );
	void			SetRelatedPos(int iX, int iY) ;
	void			SetSelectAble(bool bSelectAble)		{	 m_bSelectAble = bSelectAble;	}		// 배경 출력 여부

private:
	void			ReposOwnImage();
	void			SetItemCount(int iCount) ;


protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int		OnCursorEnter( WPARAM wParam, LPARAM lParam );
	virtual int		OnCursorOut( WPARAM wParam, LPARAM lParam );
	virtual int		OnCursorMove( WPARAM wParam, LPARAM lParam );
	virtual int		OnCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual int		OnCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual int		OnCursorMove( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual int		OnToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int		OnToolTipOut(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int		OnToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam);

	virtual int		OnLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam );
	// }}


protected:
	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//
	bool				m_bShowToolTip;
	bool				m_bSelectAble ;
	bool				m_bExpired ;			// 경매시간 종료 여부
	bool				m_bUsable ;				// 사용 가능여부
	
	int					m_iAuctionType ;		// 경매 타입
	int					m_iPriceType ;			// 입찰가격 유형 (Ely, Cash, Point)
	AUCTION_BID_TYPE	m_iBidEnable ;			// 입찰 가능 (0:가능, 1:본인입찰, 2:본인물품, 3:상회입찰)
	INT64				m_iAuctionID ;			// 경매 ID
	SPAuctionAttr*		m_pAuctionAttr ;		// 현재 올라간 아이템 정보
	SPItem*				m_pItemInfo ;			// 현재 올라간 아이템 정보

	SPTexture*			m_pBGTexture;
	SPTexture*			m_pDisTexture;
	RECT				m_rcBGDest;				// 배경 선택 이미지
	RECT				m_rcBGSrc;
	RECT				m_rcLineDest;			// 하단 구분 라인
	RECT				m_rcLineSrc;
	RECT				m_rcDisDest;			// Disable
	RECT				m_rcDisSrc;

	INSTANCE_ID			m_iStartID;
	INSTANCE_ID			m_iEndID;
	INSTANCE_ID			m_iItemIcon;		// Item Icon for Tooltip
	INSTANCE_ID			m_iBidButton;		// 입찰 버튼
	INSTANCE_ID			m_iSpotButton;		// 즉시구매 버튼

};