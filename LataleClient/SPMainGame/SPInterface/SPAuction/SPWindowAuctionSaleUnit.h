// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowAuctionSaleUnit.h
//	created:	2008/2/26   Time:10:59
//	purpose:	
//*********************************************************************/


#pragma once

#include "PacketID.h"

class SPItem ;
class SPAuctionAttr ;
class SPWindowAuctionSaleUNIT : public SPWindow
{
public:
	SPWindowAuctionSaleUNIT( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual	~SPWindowAuctionSaleUNIT();

	virtual	void	Init();
	virtual	void	Clean();
	virtual void	Show(bool bWithChild = true);
	virtual void	Hide(bool bSendServer = true);

	virtual	void	Process( float fTime );
	virtual	void	Render( float fTime );
	virtual void	RefreshRelationCoord();

	void			SetAuctionItem( SPAuctionAttr* pAuctionAttr ) ;
	void			ClearAuctionInfo() ;						// Auction 정보 초기화
	void			SetItemField( SPItem* pItem) ;				// Item
	void			SetItemField( int iType, INT64 iMoney ) ;	// Oz, Point, Ely
	

	int				GetAuctionType()				{	return m_iAuctionType ;		}
	INT64			GetAuctionID()					{	return m_iAuctionID;		}
	SPAuctionAttr*	GetAuctionAttr()				{	return m_pAuctionAttr;		}

	void			SetFinishTime() ;							// 마감시간
	void			SetBidder( const char* szBidder) ;			// 입찰자
	void			SetPrice( INT64 iBidPrice, INT64 iSpotPrice, USERID iBidUser ) ;		// 경매 입찰가 설정
	
	void			SetSelectBG( char* pTextureFile, RECT* prcSrc ) ;		// 선택 박스 영역설정
	void			SetWndState( WND_STATE eWndState );
	void			SetRelatedPos(int iX, int iY) ;
	void			SetSelectAble(bool bSelectAble)		{	 m_bSelectAble = bSelectAble;	}		// 배경 출력 여부

private:
	void			ReposOwnImage();
	DWORD			GetRareLevelColor(int iRLevel) ;
	void			SetItemCount(int iCount) ;


protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int				OnCursorEnter		( WPARAM wParam, LPARAM lParam );
	int				OnCursorOut			( WPARAM wParam, LPARAM lParam );
	int				OnCursorMove		( WPARAM wParam, LPARAM lParam );
	int				OnCursorEnter		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int				OnCursorOut			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	int				OnCursorMove		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual int		OnToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int		OnToolTipOut(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int		OnToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam);

	int				OnSaleCancel		( WPARAM wParam, LPARAM lParam );
	// }}


protected:
	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//
	int					m_iAuctionType ;		// 경매 타입
	INT64				m_iAuctionID ;			// 경매 ID
	SPAuctionAttr*		m_pAuctionAttr ;
	SPItem*				m_pItemInfo ;

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
	INSTANCE_ID			m_iItemIcon;	// Item Icon for Tooltip
	INSTANCE_ID			m_iCancelButton;	// 입찰 취소 버튼

	bool				m_bShowToolTip;
	bool				m_bSelectAble ;
	bool				m_bUsable ;				// 사용 가능여부
};