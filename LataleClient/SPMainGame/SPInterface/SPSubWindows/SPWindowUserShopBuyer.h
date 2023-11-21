#pragma once

class SPWindow;
class SPWindowStatic;
class SPWindowEdit;
//class SPWindowEditMultiLine;
class SPWindowButton;

#include "SPWindowUserShopSeller.h"

#define WIID_USERSHOP_BUY_TOP			(WIID_USERSHOP_BUY	+	10)
#define WIID_USERSHOP_BUY_CENTER		(WIID_USERSHOP_BUY	+	20)

#define WIID_USERSHOP_BUY_TITLE			(WIID_USERSHOP_BUY	+	22)
#define WIID_USERSHOP_BUY_CLOSE			(WIID_USERSHOP_BUY	+	25)
#define WIID_USERSHOP_BUY_MIDDLE		(WIID_USERSHOP_BUY	+	30)
#define WIID_USERSHOP_BUY_NAME_IMAGE	(WIID_USERSHOP_BUY	+	33)
#define WIID_USERSHOP_BUY_NAME			(WIID_USERSHOP_BUY	+	35)

#define WIID_USERSHOP_BUY_MIDDLE		(WIID_USERSHOP_BUY	+	30)

#define WIID_USERSHOP_BUY_ITEM_SKIN		(WIID_USERSHOP_BUY	+	40)		//4개씩 상점 아이템 리스트 리스트 스킨
#define WIID_USERSHOP_BUY_ITEM_ENABLE	(WIID_USERSHOP_BUY +	45)		//4개씩 아이템이 있는 경우 추가 스킨
#define WIID_USERSHOP_BUY_ITEM_LINE		(WIID_USERSHOP_BUY	+	50)		//4개씩 상점 아이템 경계
#define WIID_USERShOP_BUY_ITEM_ICONLINE (WIID_USERSHOP_BUY	+	55) //4개씩 아이템 아이콘 영역 외곽 라인
#define WIID_USERSHOP_BUY_ITEM_ICON		(WIID_USERSHOP_BUY	+	60)		//4개씩 아이템 아이콘 
#define WIID_USERSHOP_BUY_ITEM_NAME		(WIID_USERSHOP_BUY	+	70)		//4개씩 아이템 명
#define WIID_USERSHOP_BUY_ITEM_EMPTY	(WIID_USERSHOP_BUY	+	80)		//4개씩 아이템 명이 없는 경우
#define	WIID_USERSHOP_BUY_ITEM_ELY		(WIID_USERSHOP_BUY	+	90)		//4개씩 아이템 가격
#define	WIID_USERSHOP_BUY_ITEM_ELY_ICON	(WIID_USERSHOP_BUY	+	100)
#define WIID_USERSHOP_BUY_ITEM_BUTTON	(WIID_USERSHOP_BUY	+	110)	//4개씩 아이템 내부 버튼

#define WIID_USERSHOP_BUY_PREV			(WIID_USERSHOP_BUY	+	140)	//
#define WIID_USERSHOP_BUY_PAGE			(WIID_USERSHOP_BUY	+	145)	//
#define WIID_USERSHOP_BUY_NEXT			(WIID_USERSHOP_BUY	+	150)	//

#define WIID_USERSHOP_BUY_OK			(WIID_USERSHOP_BUY	+	160)	//
#define WIID_USERSHOP_BUY_CANCEL		(WIID_USERSHOP_BUY	+	165)	//

#define WIID_USERSHOP_BUY_BOTTOM		(WIID_USERSHOP_BUY	+	170)
#define WIID_USERSHOP_BUY_CALLHOST		(WIID_USERSHOP_BUY	+	171)

//class SPItem;
#include "SPItem.h"


struct SPUserSellItem {
	FM_ITEM m_SellInfo;
	SPItem* m_pItem;

	SPUserSellItem(int iIndex = 0) {
		m_pItem = new SPItem(STORAGE, iIndex);
		Clear();
	}

	~SPUserSellItem() {		
		Clear();
		delete m_pItem;
	}

	void Clear() {
		m_SellInfo.bySlot		= -1;
		m_SellInfo.uiItemID		= 0;
		m_SellInfo.byStackCount = 0;
		m_SellInfo.uiPrice		= 0;
		if(m_pItem)
			m_pItem->SetClear();
	}

	bool SetSellItem(FM_VIEWITEM* pViewItem) {
		m_SellInfo.bySlot		= pViewItem->stFM_Item.bySlot;
		m_SellInfo.uiItemID		= pViewItem->stFM_Item.uiItemID;
		m_SellInfo.byStackCount = pViewItem->stFM_Item.byStackCount;
		m_SellInfo.uiPrice		= pViewItem->stFM_Item.uiPrice;		
		
		if(m_pItem->SetItemStatus(&pViewItem->stItem, m_SellInfo.bySlot) == false)
			return false;

		return true;
	}
};



class SPWindowUserShopBuyer : public SPWindow 
{
public:
	SPWindowUserShopBuyer( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	virtual ~SPWindowUserShopBuyer();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	void SetPetShop( bool bPetShop );
	void SetPetShopStyle( int iShopStyle )
	{
		m_iShopStyle = iShopStyle;
	}
	void SetShopGUID( GU_ID shopGuid )
	{
		m_uiShopGUID = shopGuid;
	}

protected:
	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);		

	/**
	* 내부 컨트롤 생성 <br>
	@param 
	@return
	*/
	void InitSubControl();

	/**
	* 4개의 리스트 슬롯을 업데이트 <br>
	@param 
	@return
	*/
	void UpdateList();

	/**
	 * 
	 @param 
	 @return
	*/
	void CreateItemList();
	
	/**
	 * 
	 @param 
	 @return
	*/
	void DeleteAllItemList();

	/**
	 * 
	 @param 
	 @return
	*/
	void ClearAllItemList();

	/**
	* 프로세스마다 내가 거래할수 있는 상황인지 체크 <br> 
	@param 
	@return
	*/
	bool IsTradeEnable();

	/**
	 * 유저 상점에서 나감 <br> 
	 @param 
	 @return
	*/
	bool SendUserShopOut();

	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnShopIn				(WPARAM wParam, LPARAM lParam);
	int OnShopOut				(WPARAM wParam, LPARAM lParam);
	int OnShopBuy				(WPARAM wParam, LPARAM lParam);
	int OnShopSetClose			(WPARAM wParam, LPARAM lParam);			/**< Esc등 인터페이스에 의한 강제 종료 요청 <br> */
	int OnClose					(WPARAM wParam, LPARAM lParam);
	int OnPrev					(WPARAM wParam, LPARAM lParam);
	int OnNext					(WPARAM wParam, LPARAM lParam);
	int OnOk					(WPARAM wParam, LPARAM lParam);
	int OnCancel				(WPARAM wParam, LPARAM lParam);
	int	OnCallHost				(WPARAM wParam, LPARAM lParam);
	int OnItemClickUp			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int	OnItemClickDown			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipEnter			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipMove			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipOut			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);	

private:
	SPTexture*		m_pDisable;									/**< 비활성 표현용 <br> */
	RECT			m_rtSrcDisable;								/**< 비활성 표현용 소스 <br> */
	
	SPWindowButton*	m_pClose;

	SPWindow*		m_pTitle;									/**< 개인상점 명 <br> */
	SPWindowButton	*m_btnCallHost;								/**< Send msg to shop host	*/
	
	SPWindow*		m_pItemSkin[MAX_USER_SHOP_LIST];			/**< 리스트 스킨 <br> */
	SPWindowStatic* m_pItemSkinSelect[MAX_USER_SHOP_LIST];		/**< Select 표현용 <br> */
	SPWindowButton* m_pItem[MAX_USER_SHOP_LIST];				/**< 리스트 버튼 <br> */
	SPWindow*		m_pItemIconLine[MAX_USER_SHOP_LIST];		/**< 리스트 아이템 아이콘 테두리 <br> */
	SPWindow*		m_pItemIcon[MAX_USER_SHOP_LIST];			/**< 리스트 아이템 아이콘 <br> */
	SPWindow*		m_pItemName[MAX_USER_SHOP_LIST];			/**< 리스트 아이템 명 <br> */
	SPWindow*		m_pItemEmpty[MAX_USER_SHOP_LIST];			/**< 리스트 아이템 없음 표시 <br> */
	//SPWindowEdit*	m_pItemEly[MAX_USER_SHOP_LIST];				/**< 리스트 돈 입력 <br> */
	SPWindow*		m_pItemEly[MAX_USER_SHOP_LIST];				/**< 리스트 돈 입력 <br> */
	SPWindow*		m_pItemElyIcon[MAX_USER_SHOP_LIST];			/**< 리스트 돈 아이콘 <br> */	
	
	bool			m_bItemAble[MAX_USER_SHOP_LIST];			/**< 내가 올려논 아이템이 내가 착용가능한 아이템인지 <br> */
	int				m_iStack[MAX_USER_SHOP_LIST];				/**< 로컬 플레이어용 스택 <br> */
	RECT			m_rtStack[MAX_USER_SHOP_LIST];				/**< 스택표현시 참고 영역 Icon 영역과 동일 <br> */
	ICON_MARK		m_iItemMark[MAX_USER_SHOP_LIST];			/**< ICON MARK <br> */

	RECT			m_rtMarkSrc[4];								/**< ICON MARK Src <br> */

	SPWindowButton* m_pPrev;
	SPWindowButton* m_pNext;
	SPWindow*		m_pPage;									/**< Page <br> */

	int				m_iMaxPage;									/**< 최대 페이지 <br> */
	int				m_iCurPage;									/**< 현재 페이지 <br> */
	int				m_iSelectItem;								/**< 인터페이스상 리스트상 선택한 인덱스 <br> */
	int				m_iSelectIndex;								/**< 데이터상 리스트중 선택된 인덱스 <br> */	

	SPWindowButton* m_pOK;										/**< 개설 <br> */
	SPWindowButton* m_pCancel;									/**< 취소 <br> */

	SPPlayer*		m_pTargetPlayer;							/**< 거래 대상의 Player정보 <br> */
	float			m_fStartPosX;								/**< 거래를 시작할때 내 위치 X <br> */
	float			m_fStartPosY;								/**< 거래를 시작할때 내 위치 Y <br> */

	std::vector<SPUserSellItem*> m_vpSellItem;					/**< 판매하는 아이템 <br> */
	std::vector<FM_VIEWITEM> m_vpViewItem;					/**< 판매하는 아이템 <br> */

	bool			m_bPetShop;
	int				m_iShopStyle;
	GU_ID			m_uiShopGUID;
};