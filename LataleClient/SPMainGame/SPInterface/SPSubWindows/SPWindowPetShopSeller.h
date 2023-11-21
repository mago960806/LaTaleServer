#pragma once

class SPWindow;
class SPWindowStatic;
class SPWindowEdit;
//class SPWindowEditMultiLine;
class SPWindowButton;

#define WIID_PETSHOP_SELL_TOP			(WIID_PETSHOP_SELL	+	10)
#define WIID_PETSHOP_SELL_CENTER		(WIID_PETSHOP_SELL	+	20)

#define WIID_PETSHOP_SELL_TITLE		(WIID_PETSHOP_SELL	+	22)
#define WIID_PETSHOP_SELL_CLOSE		(WIID_PETSHOP_SELL	+	25)

#define WIID_PETSHOP_SELL_MIDDLE		(WIID_PETSHOP_SELL	+	30)
#define WIID_PETSHOP_SELL_NAME			(WIID_PETSHOP_SELL	+	33)
#define WIID_PETSHOP_SELL_NAME_EDIT	(WIID_PETSHOP_SELL	+	35)

#define WIID_PETSHOP_SELL_ITEM_SKIN	(WIID_PETSHOP_SELL	+	40)		//4개씩 상점 아이템 리스트 리스트 스킨
#define WIID_PETSHOP_SELL_ITEM_ENABLE	(WIID_PETSHOP_SELL +	40)		//4개씩 아이템이 있는 경우 추가 스킨
#define WIID_PETSHOP_SELL_ITEM_LINE	(WIID_PETSHOP_SELL	+	50)		//4개씩 상점 아이템 경계
#define WIID_PETSHOP_SELL_ITEM_ICONLINE (WIID_PETSHOP_SELL	+	55) //4개씩 아이템 아이콘 영역 외곽 라인
#define WIID_PETSHOP_SELL_ITEM_ICON	(WIID_PETSHOP_SELL	+	60)		//4개씩 아이템 아이콘 
#define WIID_PETSHOP_SELL_ITEM_NAME	(WIID_PETSHOP_SELL	+	70)		//4개씩 아이템 명
#define WIID_PETSHOP_SELL_ITEM_EMPTY	(WIID_PETSHOP_SELL	+	80)		//4개씩 아이템 명이 없는 경우
#define	WIID_PETSHOP_SELL_ITEM_ELY		(WIID_PETSHOP_SELL	+	90)		//4개씩 아이템 가격
#define	WIID_PETSHOP_SELL_ITEM_ELY_ICON	(WIID_PETSHOP_SELL	+	100)
#define WIID_PETSHOP_SELL_ITEM_BUTTON	(WIID_PETSHOP_SELL	+	110)	//4개씩 아이템 내부 버튼

#define WIID_PETSHOP_SELL_PREV			(WIID_PETSHOP_SELL	+	140)	//
#define WIID_PETSHOP_SELL_PAGE			(WIID_PETSHOP_SELL	+	145)	//
#define WIID_PETSHOP_SELL_NEXT			(WIID_PETSHOP_SELL	+	150)	//

#define WIID_PETSHOP_SELL_OK			(WIID_PETSHOP_SELL	+	160)	//
#define WIID_PETSHOP_SELL_CANCEL		(WIID_PETSHOP_SELL	+	165)	//

#define WIID_PETSHOP_SELL_BOTTOM		(WIID_PETSHOP_SELL	+	170)


/**
 * <br>
*/
enum PETSHOP_STATE {
	PETSHOP_STATE_NULL = 0,				/**< 아무런 상태도 아님 <br> */
	PETSHOP_STATE_CREATE = 1,				/**< 상점 개설 준비 상태 <br> */
	PETSHOP_STATE_CREATE_REQUEST = 2,		/**< 상점 개설 요청 <br> */
	PETSHOP_STATE_SELL = 3,				/**< 상점 개설 요청의 응답으로 실제 상점 모드 <br> */
};

class SPWindowPetShopSeller : public SPWindow 
{
public:
	SPWindowPetShopSeller( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	virtual ~SPWindowPetShopSeller();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	void SetPet( SPPet	*pet, PET_PLACE iPlace )
	{
		m_pPet = pet;
		m_iCurPetPlace = iPlace;
	}

public:
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
	 * 상점을 닫거나 취소하는 경우 처리 <br> 
	 @param 
	 @return
	*/
	void CloseShop(bool bAutoClose = false);

	/**
	 * 16개의 Ely란을 0으로 초기화 <br>
	 @param 
	 @return
	*/
	void ClearEly();
	
	/**
	 * 16개의 판매 아이템란을 초기화 <br>
	 @param 
	 @return
	*/
	void ClearSellItem();

	/**
	 * 
	 @param 
	 @return
	*/
	void SetSellCount(int iPage);
	
	/**
	 * 판매 아이템 리스트를 생성 <br>
	 @param 
	 @return
	*/
	bool CreateSellItemList();

	/**
	 * 판매 아이템 정보(서버로 보내줄)를 초기화 <br>
	 @param 
	 @return
	*/
	bool InitFMSELLITEM(int iSlotIndex, FM_SELLITEM* pSellItem);

	/**
	 * 판매 아이템 정보(서버로 보내줄)를  설정 <br>
	 @param 
	 @return
	*/
	bool SetFMSLEEITEM(FM_SELLITEM* pSellItem, SPSelectItem* pSelectItem, INT64 iSellPrice);

	/**
	 * 해당 판매리스트상의 인덱스를 지니는 판매 아이템 정보를 확인 <br>
	 @param  int iSlotIndex 판매리스트상의 인덱스
	 @return
	*/
	FM_SELLITEM* GetFMSELLITEM(int iSlotIndex);
	
	/**
	 * 상접 개설을 서버에 요청 <br>
	 @param 
	 @return
	*/
	bool SendPetShopOpen();

	/**
	 * 상점 닫음을 서버에 요청 <br>
	 @param 
	 @return
	*/
	bool SendPetShopClose();

	/**
	 * 프로세스마다 내가 거래할수 있는 상황인지 체크 <br> 
	 @param 
	 @return
	*/
	bool IsTradeEnable();

	/**
	 * 상점 물건이 팔리는 팻킷이올때마다 판매 품목이 남아있는지 확인 <br>
	 * true인 경우 물건이 매진된 상태 <br>
	 @param 
	 @return
	*/
	bool IsEmptyShop();

	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnShopOpen				(WPARAM wParam, LPARAM lParam);
	int OnShopClose				(WPARAM wParam, LPARAM lParam);
	int OnShopBuy				(WPARAM wParam, LPARAM lParam);
	int OnShopSetClose			(WPARAM wParam, LPARAM lParam);			/**< Esc등 인터페이스에 의한 강제 종료 요청 <br> */
	int	OnEditSetFocus			(WPARAM wParam, LPARAM lParam);
	int	OnEditKillFocus			(WPARAM wParam, LPARAM lParam);
	int OnClose					(WPARAM wParam, LPARAM lParam);
	int OnPrev					(WPARAM wParam, LPARAM lParam);
	int OnNext					(WPARAM wParam, LPARAM lParam);
	int OnOk					(WPARAM wParam, LPARAM lParam);
	int OnCancel				(WPARAM wParam, LPARAM lParam);	
	int ItemClear				(WPARAM wParam, LPARAM lParam);			/**< 로컬 플레이어의 아이템 제거 <br> */
	int	OnEditSetFocusRange		(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int	OnEditKillFocusRange	(unsigned int iUiID, WPARAM wParam, LPARAM lParam);	
	int OnItemClickUp			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnItemClickDown			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnUpdateEly				(unsigned int iUiID, WPARAM wParam, LPARAM lParam); //Ely Update
	int OnToolTipMove			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipEnter			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipOut			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);	

private:
	SPTexture*		m_pTexture;
	SPWindowEdit*	m_pName;
	
	SPWindowButton* m_pClose;

	SPWindow*		m_pItemSkin[MAX_USER_SHOP_LIST];			/**< 리스트 스킨 <br> */
	SPWindowButton* m_pItem[MAX_USER_SHOP_LIST];				/**< 리스트 버튼 <br> */
	SPWindow*		m_pItemIconLine[MAX_USER_SHOP_LIST];		/**< 리스트 아이템 아이콘 테두리 <br> */
	SPWindow*		m_pItemIcon[MAX_USER_SHOP_LIST];			/**< 리스트 아이템 아이콘 <br> */
	SPWindow*		m_pItemName[MAX_USER_SHOP_LIST];			/**< 리스트 아이템 명 <br> */
	SPWindow*		m_pItemEmpty[MAX_USER_SHOP_LIST];			/**< 리스트 아이템 없음 표시 <br> */
	SPWindowEdit*	m_pItemEly[MAX_USER_SHOP_LIST];				/**< 리스트 돈 입력 <br> */
	SPWindow*		m_pItemElyIcon[MAX_USER_SHOP_LIST];			/**< 리스트 돈 아이콘 <br> */

	SPWindowButton* m_pPrev;
	SPWindowButton* m_pNext;
	SPWindow*		m_pPage;									/**< Page <br> */

	SPWindowButton* m_pOK;										/**< 개설 <br> */
	SPWindowButton* m_pCancel;									/**< 취소 <br> */

	SPTexture*		m_pDisable;									/**< 비활성 표현용 <br> */
	RECT			m_rtSrcDisable;								/**< 비활성 표현용 소스 <br> */
	
	//bool			m_bItemEnable[MAX_USER_SHOP_LIST];			/**< 아이템이 있는지 <br> */
	bool			m_bItemAble[MAX_USER_SHOP_LIST];			/**< 내가 올려논 아이템이 내가 착용가능한 아이템인지 <br> */
	int				m_iStack[MAX_USER_SHOP_LIST];				/**< 로컬 플레이어용 스택 <br> */
	RECT			m_rtStack[MAX_USER_SHOP_LIST];				/**< 스택표현시 참고 영역 Icon 영역과 동일 <br> */
	ICON_MARK		m_iItemMark[MAX_USER_SHOP_LIST];			/**< ICON MARK <br> */

	RECT			m_rtMarkSrc[4];								/**< ICON MARK Src <br> */

	int				m_iSellCount;								/**< 내가 판매할수 있는 최대 페이지 <br> */
	int				m_iMaxPage;									/**< 최대 페이지 <br> */
	int				m_iCurPage;									/**< 현재 페이지 <br> */
	int				m_iSelectItem;								/**< 인터페이스상 리스트상 선택한 인덱스 <br> */
	int				m_iSelectIndex;								/**< 데이터상 리스트중 선택된 인덱스 <br> */	

	PETSHOP_STATE	m_iPetShopState;							/**< 현재 개인 상점 상황 <br> */
	INT64			m_iEly[FM_MAX_SELLITEM];					/**< 가격정보 <br> */
	std::string		m_strBuyer[FM_MAX_SELLITEM];				/**< 구매자 정보 <br> */

	float			m_fStartPosX;								/**< 거래를 시작할때 내 위치 X <br> */
	float			m_fStartPosY;								/**< 거래를 시작할때 내 위치 Y <br> */
	bool			m_bAutoClose;								/**< 물건 소진후 자동으로 닫힌경우 <br> 종료는 하지만 윈도우는 남아 있음 */	

	int				m_iPetShopType;							/**< 내 상점 타입 <br> */
	
	bool			m_bSendClose;								/**< 종료 패킷을 보낸경우 true <br> */

	std::vector<FM_SELLITEM*> m_vpSellItem;						/**< 판매할 아이템 리스트 <br> */
	SPPet			*m_pPet;
	PET_PLACE		m_iCurPetPlace;
};