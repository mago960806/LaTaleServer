// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPItemStatus
// Comment     : 
// Creation    : metalgeni 2004-11-05 오전 10:26:19
//***************************************************************************

#pragma once

#include <string>

using namespace std;


enum EQUIP_ID;

class SPItemStatus {
public:
	SPItemStatus();
	SPItemStatus(ITEMSTATUS_ID );	
	SPItemStatus( const SPItemStatus& src );

	virtual ~SPItemStatus();
	virtual LRESULT SPSendMessage(MODEL_VALUE_TYPE mvMsg, LPARAM lParam = 0);

	bool		IsNULL();
	bool		SetItemID(UINT32 iItemID);
	SPItemAttr*	GetItemAttr();

	UINT32		GetItemID();
	int			GetStackCount();
	int			GetRareLevel();

	PACK_OPT*	GetOption(int iOrder);
	int			GetOptionCount();

	int			GetStatusCount();
	int			GetLifeTime();
	void		SetLifeTime(int iLifeTime);

	void		SetExp(INT64 iExp);
	INT64		GetExp();

	void		SetSp(INT16 iSp);	
	INT16		GetSp();

	/**
	 * 시간제 아이템의 유효상태를 설정한다 <br>
	 * 디폴트 유효상태 해제 <br>
	 @param 
	 @return
	*/
	void		SetTimeAvailable(bool bAvailable = false);

	/**
	 * 시간제 아이템이 유효한 상태인지 확인한다 <br> 
	 @param 
	 @return
	*/
	bool		IsTimeAvailable();

	/**
	 * LifeTime 감소 <br> 
	 @param 
	 @return
	*/
	void		DecreaseLifeTime(int iTime);
	
	/**
	 *
	 @param
	 @return
	*/
	int			GetItemSlotIndex() { return m_iItemSlotIndex; }
	
	/**
	 *
	 @param
	 @return
	*/
	int			GetItemStatusID()  { return m_ItemStatusID; }

	/**
	 * <br>
	 @param
	 @return
	*/
	int			GetItemBackPrice()	{ return m_iBackPrice; }

	/**
	 * 추천 상품 툴팁 내용을 임시로 업데이트 해준다 <br>
	 @param 
	 @return  
	*/
	void		SetHotDesc(std::string strDesc);
	
	void		SetLotteryName(std::string strName);
	/**
	 *
	 @param 
	 @return  
	*/
	std::string GetHotDesc()		{ return m_strHotDesc; }

	std::string GetLotteryName()	{ return m_strLotteryName; }

	virtual bool Copy(SPItemStatus* pSrc);

protected:
	//virtual bool Copy(SPItemStatus* pSrc);
	virtual void SetNull();


protected:
	
	UINT32			m_iItemID;
	SPItemAttr*		m_pItemAttr;

	// 가변적인 부분들
	ITEMSTATUS_ID	m_ItemStatusID;			// Item 종류 //이아니라 위치저장이다.  영현씨 확장에 의해 용도변경..

	//int			m_nItemSlotIndex;		// Item Slot index (인벤토리 그리드 위치)
	int				m_iItemSlotIndex;

	//int			m_nStackCount;			// 갯수 - 아이템테이블에서는 무의미하다..
	int				m_iStackCount;
	int				m_iLifeTime;			/**< 유효시간 <br> 클라이언트 정보이므로 서버와 일치하지 않음 <br> */
	int				m_iBackPrice;			// 되팔기
	ITEMNO			m_iItemNo;				// 아이템 고유 번호
	int				m_iOptPoint;			// 내구도

	UINT8			m_iRare;				// 레어도
	PACK_OPT		m_Opt[MAX_ITEM_OPT];	// 옵션

	bool			m_bTimeAvailable;		/**< 유효시간 <br> 서버에서 보내주는 정보로 설정 <br> true인 경우 유효 <br> */
	
	INT64			m_iItemExp;				// 아이템경험치
	INT16			m_iItemSP;				// 아이템SP

	std::string		m_strHotDesc;			/**< 추천 상품 툴팁 메시지를 위한 임시 버퍼 <br> */

	std::string		m_strLotteryName;		/**[xialin 2008/10/22]add */
};












