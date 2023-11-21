

/**
* @class	SPPlayerInvenArchive
* @brief	Player Inventory Archive class
* @author	kkandori
* @date		Jan, 20, 2005
*/

#pragma once

#define MAX_INVENTORY_NUM		16

struct SPItemSet;
class SPItemCluster;

//#include "SPDebug.h"

const int MAX_INVENTORY_SLOT		= 80;		/**< 최대 인벤토리 슬롯 <br> 변경될수 있슴 <br> */
const int MAX_INVENWEAR_SLOT		= 40;
const int VIEW_SLOT					= 16;
const int DEFAULT_INVENTORY_SLOT	= 28;		/**< 기본 인벤토리 슬롯 <br> 변경될수 있슴 <br> */
const int DEFAULT_EQUIP_SLOT		= 16;		/**< 기본 장비 슬롯 <br> */
const int INVENTORY_LINE			= 4;		/**< 인벤토리 각 라인별 슬롯 수 <br> */
const int EQUIP_LINE				= 3;		/**< 장비 각 라인변 슬롯 수 <br> */

const int INVENTORY_TYPE			= 6;		/**< 인벤토리 종류별 갯수 (장비, 소비, 기타 , 이벤트, 팻, 하우징의 여섯가지)<br> */
const int EQUIP_TYPE				= 3;		/**< 장비 종류별 갯수 (장비, 팻션, 팻 등) <br> */
const int MAX_INVENTORY_TYPE		= 10;		/**< 인벤토리 종류별 갯수 (인벤토리 종류 + 장비 종류) <br> */

const int DEFAULT_INVENTORY_PET_SLOT	= 4;	/**< 인벤토리 펫용 사이즈 <br> */

const int DEFAULT_STORAGE_SIZE		= 4;		/**< 기본 창고 사이즈 <br> */
const int VIEW_STORAGE_SLOT			= 16;		/**< 창고 뷰 스롯 <br> */
const int STORAGE_LINE				= 4;		/**< 창고의 한 라인별 슬롯 수 <br> */

const int MAX_PC_TRADE_SLOT			= 4;		/**< 거래 최대 슬롯 사이즈 <br> */

const int MAX_ITEM_BLOCK			= 8;		/**< 인첸트, 강화, 시간연장 등 아이템 블럭시 사용 <br> */

const int DEFAULT_PET_SLOT			= 1;		/**< 기본 PET 장비 슬롯 <br> */
const int MAX_PET_INVENTORY_SLOT	= 3;


/**
 * 현재 메시지창이 어떤 이유로 열려있는지에 이용한다 <br>
 * 추후 OnYes, OnNo 코드에서도 이 플래그를 사용하도록 수정할것 <br>
*/
enum NOTICE_USE {
    NOTICE_USE_NULL					,		/**< NULL <br> */
	NOTICE_USE_ITEM_TRASH			,		/**< Item Trash <br> */
	NOTICE_USE_ITEM_TRASH_FAIL		,		/**< 아이템 파괴 안됨 <br> */
	NOTICE_USE_ITEM_PARTITION		,		/**< Item Partition <br> */
	NOTICE_USE_TRADE_REQUEST		,		/**< Trade 요청 <br> */
	NOTICE_USE_PARTY_INVITE			,		/**< 파티 초대 <br> */	
	NOTICE_USE_PLAYER_DEAD			,		/**< 플레이어 사망 <br> */
	NOTICE_USE_PLAYER_DEAD_REDUCE	,		/**< 포탈시 사망 패널티 감소를 할것인지 <br> */
	NOTICE_USE_PLAYER_DEAD_PVP		,		/**< PVP 중 사망시 <br> */
	NOTICE_USE_PLAYER_REVIVE		,		/**< 플레이어 부활 <br> */  
	NOTICE_USE_GUILD_INVITE			,		/**< 길드 초대 <br>*/
	NOTICE_USE_GUILD_EXCEDE			,		/**< 길드 탈퇴 <br>*/
	NOTICE_USE_PVP_INVITE			,		/**< 1:1 PVP 초대<br>*/
	NOTICE_USE_SUMMON_INVITE		,		/**< PC소환 초대<br>*/
	NOTICE_USE_LOVE_SUMMON			,		/**< 연인 소환 <br> */ 
	NOTICE_USE_LOVE_DIVORCE			,		/**< 이별 신청 <br> */  
	NOTICE_USE_MAX,							/**< Notice Max <br> */
};


enum WINDOW_STATE
{
	WINDOW_NULL		=	0,
	WINDOW_ONE			 ,
	WINDOW_TWO			 ,
	WINDOW_THREE		 ,
};

enum PET_PLACE;

/** 
 * 아이템 관련해 픽업된 아이템의 위치 (컨테이너 타입과 해당 슬롯 인덱스를 기억하는 구조체 ) <br>
*/
typedef struct _SPSelectItem {
	CONTAINER_TYPE m_iContainer;				/**< 픽업 아이템의 컨터이너 종류 <br> */
	int m_iSlotIndex;							/**< 픽업 아이템의 슬롯 인덱스 <br> */

	_SPSelectItem() {
		Clear();
	}

	~_SPSelectItem() {
		Clear();
	}

	void Clear() {
		m_iContainer = CONTAINER_TYPE_INVALID;
		m_iSlotIndex = -1;
		//
		//DXUTOutputDebugString("SelectItem Clear Container[%d], Index[%d]\n", m_iContainer, m_iSlotIndex);
	}

	void SetSelectItem(CONTAINER_TYPE iContainerType, int iSlotIndex) {
		Clear();
		
		m_iContainer = iContainerType;
		m_iSlotIndex = iSlotIndex;
	}

	CONTAINER_TYPE GetContainerType() {
		return m_iContainer;
	}

	int GetSlotIndex() {
		return m_iSlotIndex;
	}

	/**
	 *	SPSelectItem에 값이 설정되어 있는지 
	 @param NONE
	 @return bool true이면 비어 있는 상태임.
	*/
	bool IsNull() {
		if(m_iContainer == CONTAINER_TYPE_INVALID && m_iSlotIndex == -1)
			return true;

		return false;
	}
} SPSelectItem;


class SPItem;
class SPItemStatus;

class SPPlayerInvenArchive
{
public:
	SPPlayerInvenArchive(SPGameObject* pParent);
	virtual ~SPPlayerInvenArchive();

	virtual void	Init();
	void			Clear();	


	/**
	* 아이템을 세팅한다. <br>
	* 해당 함수의 호줄 내부에서 인벤토리 아이템을 지정해주고 <br>
	* 장비에 관련한 아바타 렌더 메시지를 발생시킨다 <br>
	@param CONTAINER_ITEM item 설정할 아이템의 베이스
	@param bool bReloadEquipShow = false 현재 사용하지 않는 파라미터임
	@return bool
	@warning 몇몇 예외 되는 상황이 있으니 주의할것 ㅠ.ㅠ
	*/
	bool ItemSetting( CONTAINER_ITEM item, bool bSetEquip = true);

	/**
	* 아이템을 삭제한다 <br>
	* 스텍감소 또는 완전 삭제가 이루어진다 <br>
	@param TRASH_ITEM item
	@return bool
	@warning 함수에 들어오기 전에 슬롯인덱스 정보는 0베이스의 것으로 조정된다
	*/
	bool TrashItem(TRASH_ITEM item);

	/**
	 * 주어진 컨테이너 타입의 인벤 내의 정보를 초기화 한다 <br> 
	 @param CONTAINER_TYPE eType
	 @return  NULL
	*/
	void ClearInventory(CONTAINER_TYPE eType);

	/**
	 * 창고 인터페이스 갱신 <br>
	 @param NULL
	 @return  int 
	*/
	int ReflashStorage();

public:	
	int	OnSendMessage(SPGM msg, LPARAM lParam);
	
protected:
	/**
	 * 해당 인덱스의 무기 슬롯을 활성화 시킨다 <br>
	 @param int iCurSlot
	 @return NONE
	*/
	void SetActiveWeaponSlot(int iCurSlot);	
	
	/**
	 * 아이템 패기에 대한 처리 및 패킷전송 <br> 
	 @param NONE
	 @return int
	*/
	int	SetTrash();	

	/////// Inventory	
	/**
	 * 아이템인벤토리 창의 클릭 메세지를 받는다. <br>
	 @param int iSlotIndex 들어올린 위치 <br>
	 @return int
	 @warning 해당 인덱스는 0베이스이다 
	*/
	int	ItemGridClickDown(int iSlotIndex);	
	int	ItemGridClickUp(int iSlotIndex);
	int	ItemGridCursorMove(LPARAM lParam);	
	int	ItemPickUp(int iSlotIndex);	
	int ItemGridDBK(int iSlotIndex);				/**< 아이템 그리드 더블클릭시 장비 자동 착용<br> */

	int ItemPageUp();
	int ItemPageDown();
	int ItemSetPage(int iPage);
	
	/**
	 * 컷인 트레이드 상황에서 아이템 판매창에 바로 등록 <br>
	 @param int iSlotIndex 사용할 아이템의 인덱스 
	 @return int
	 @warning 인덱스는 0베이스
	*/
	int	ItemGridRClick(int iSlotIndex);
	
	/**
	 * 현재 아이템 페이지에 대한 정보 세팅 <br>
	 @param CONTAINER_TYPE
	 @return NONE
	*/
	void SetCurrentItemPage(CONTAINER_TYPE);

	/**
	 * 현재 장비 페이지에 대한 정보 세팅 <br>
	 @param CONTAINER_TYPE
	 @return NONE
	*/
	void SetCurrentEquipPage(CONTAINER_TYPE);	
	

	/**
	 * 아이템 인벤토리 종류 변환 <br>
	 @param  LPARAM lParam
	 @return int
	*/
	int	OnChangeInventory( LPARAM lParam);
	
	/**
	* 인벤토리를 다시 그리도록 한다 <br>
	@param  NONE
	@return int
	*/
	int ReflashInventory();
	
	/**
	 * 두 아이템을 바꾼다. <br>
	 @param CHANGE_SLOT item
	 @return NONE
	 @warning 함수에 들어오기 전에 슬롯인덱스 정보는 0베이스의 것으로 조정된다
	*/
	void SlotChange(CHANGE_SLOT item);

	/**
	 * 주어진 동일한 두 아이템을 합친다 <br> 
	 @param CHANGE_SLOT item
	 @return NONE
	 @warning 함수에 들어오기 전에 슬롯인덱스 정보는 0베이스의 것으로 조정된다
	*/
	void SlotMerge(CHANGE_SLOT item);
	

	/**
	 * 현재 아이템인벤토리로 정보를 갱신한다. <br>
	 @param NONE
	 @return NONE
	*/
	void SettingCurrentInventory();
	
	/**
	 *  현재 장비인벤토리로 정보를 갱신한다. <br>
	 @param NONE
	 @return NONE
	*/
	void SettingCurrentEquip();		

	/////// Equip
	/**
	 * 장비 인벤토리 종류 변환 <br>
	 @param LPARAM lParam
	 @return int
	*/
	int	OnChangeEquip( LPARAM lParam );

	/**
	 * 장비 인벤토리 창의 클릭 메세지를 받는다. <br>
	 @param int iSlotIndex
	 @return int
	 @warning 해당 슬롯 인덱스는 0베이스이다
	*/
	int	EquipGridClickDown(int iSlotIndex);
	int	EquipGridClickUp(int iSlotIndex);
	int	EquipGridCursorMove(LPARAM lParam);
	int	EquipPickUp(int iSlotIndex);
	int EquipGridRClickUp(int iSlotIndex);

	/**
	 * 무기 1 에 대한 클릭 메세지를 받는다. <br>
	 @param int iSlotIndex
	 @return int
	 @warning 해당 슬롯 인덱스는 0베이스이며 데이터와 맞추려면 약간의 변환이 필요하다 
	*/
	int	Weapon1ClickDown(int iSlotIndex);
	int	Weapon1ClickUp(int iSlotIndex);
	int	Weapon1CursorMove(LPARAM lParam);
	int	Weapon1PickUp(int iSlotIndex);
	int	Weapon1RClickUp(int iSlotIndex);
	
	/**
	 * 무기 2 에 대한 클릭 메세지를 받는다. <br>
	 @param int iSlotIndex
	 @return int
	 @warning 해당 슬롯 인덱스는 0베이스이며 데이터와 맞추려면 약간의 변환이 필요하다 
	*/
	int	Weapon2ClickDown(int iSlotIndex);
	int	Weapon2ClickUp(int iSlotIndex);
	int	Weapon2CursorMove(LPARAM lParam);
	int	Weapon2PickUp(int iSlotIndex);
	int	Weapon2RClickUp(int iSlotIndex);
	
	/**
	 * 창고에 대한 크릭 메시지를 받는다 <br>
	 @param
	 @return
	*/
	int	StorageClickDown(int iSlotIndex);
	int	StorageClickUp(int iSlotIndex);
	int	StorageCursorMove(LPARAM lParam);
	int	StoragePickUp(int iSlotIndex);
	int StoragePageUp();
	int StoragePageDown();
	int StorageSetPage(int iPage);	

	/**
	* 펫에 대한 크릭 메시지를 받는다 <br>
	@param
	@return
	*/
	int PetClickDown(int iSlotIndex);
	int PetClickUp(int iSlotIndex);
	int PetCursorMove(LPARAM lParam);
	int	PetPickUp(int iSlotIndex);

	/**
	 * 무기 교환 <br>
	 @param NONE
	 @return NONE
	*/
	void WeaponToggle();
	
	/**
	 * 무기 교환 <br> 
	 @param NONE
	 @return NONE
	*/
	void WeaponChange();
	
	/**
	 * 실제 무기 슬롯 교환 작업 <br> 
	 @param NONE
	 @return NONE
	*/
	void WeaponChangeInterface();	
	

//------ Receive
	// Inventory
	int	OnSetInventory		( LPARAM lParam );	/**< 인벤토리 정보 (1개) <br> */
	int	OnSetInventoryEx	( LPARAM lParam );	/**< 인벤토리 정보 (다수) <br> */
	int	OnSetChangeSlot		( LPARAM lParam );	/**< 아이템 슬롯 교환 <br> */
	int OnSetDivideSlot		( LPARAM lParam );	/**< 아이템 분할 <br> */
	int	OnSetTrash			( LPARAM lParam );	/**< 아이템 파괴 <br> */
	int OnSetTrashByTime	( LPARAM lParam );	/**< 아이템 지속시간에 의한 삭제 <br> */
	int	OnSetExceptionMsg	( LPARAM lParam );	/**< 오류.. <br> 사용되지 않음 <br> */
	int OnSetUsing			( LPARAM lParam );	/**< 아이템 사용 <br> */
	int OnSetExpire			( LPARAM lParam );	/**< 아이템 지속시간에 의한 만료 <br> */
	int OnAddTime			( LPARAM lParam );	/**< 아이템 지속시간 연장 <br> */
	int OnSetExp			( LPARAM lParam );	/**< 아이템 경험치 설정 <br> */
	int OnAddSp				( LPARAM lParam );	/**< 아이템 SP 설정 <br> */
	int OnSetChatUsing		( LPARAM lParam );	/**< Mic 아이템 사용 응답 <br> */
	int	OnSetFindUsing		( LPARAM lParam );  //왱冷훙돛야돨鑒커
	// Equipment

//------ Send
	// Inventory
	int	SendReqLooting		(LOOTING_ITEM item);    /**< 아이템 루팅 요청 <br> */
	int	SendReqChangeSlot	(CHANGE_SLOT   item);	/**< 아이템을 슬롯 교환 요청 <br> */
	int SendReqDivide		(CHANGE_SLOT item);		/**< 아이템 분할의 경우 요청 <br> */
	int	SendReqTrash		(TRASH_ITEM item);		/**< 파괴 요청 <br> */

	
	/**
	 * Notice 의 Yes 응답 처리기 <br>
	 @param
	 @return
	*/
	int OnYes			(LPARAM lParam);

	/**
	 * Notice 의 No 응답 처리기 <br>
	 @param
	 @return
	*/
	int OnNo			(LPARAM lParam);

//////////////////////////////////////////////////////////////////////////
private:
	//PC TRADE
	int	OnTradeRequest		(LPARAM lParam);		/**< 유저간 거래 요청 <br> */
	int	OnTradeResponse		(LPARAM lParam);		/**< 유저가 거래 요청 응답 <br> */
	int OnTradePushMoney	(LPARAM lParam);		/**< 거래중 Ely 변경 <br> */
	int OnTradePushItem		(LPARAM lParam);		/**< 거래중 Item 변경 <br> */
	int OnTradePopItem		(LPARAM lParam);		/**< 거래중 Item 변경 <br> */
	int OnTradeAccept		(LPARAM lParam);		/**< 거래 완료 요청 <br> */
	int OnTradeCancel		(LPARAM lParam);		/**< 거래 취소 <br> */
	int OnTradeWait			(LPARAM lParam);		/**< 거래 대기 <br> */

	//NPC TRADE 
	int OnNpcTrade			(LPARAM lParam);		/**< NPC에게 물건을 산 경우 응답 <br> */

	//Another PC Item Info
	int OnPlayerInfoEquip	(LPARAM lParam);		/**< Another PC Item Info <br> */

public:
	bool SendTradeResponse	(bool bAccept = false);		
	
	bool IsSendTradeRequest();						/**< 로컬 플레이어가 트레이드 요청 패킷을 보냈는지 <br> */
	void SetSendTradeRequest(bool bSend);			/**< 로컬 플레이어의 트레이드 요청 패킷 처리 상황을 설정 <br> */

	//[xialin 2008/07/21] add
	void ClearWearItem();
	void DeleteWearItem();

public:
	/**
	* 로컬 플레이어인 경우 아이템 인벤토리 공간을 생성한다 <br>
	* 생성하는 공간은 인벤토리 공간과 저장 공간이다 <br>
	@param CONTAINER_TYPE
	@return NONE
	@warning 장비의 경우 플레이어를 생성하면서 공간을 생성한다 
	*/
	bool CreateLocalInventory();
	
	/**
	* 현재 아이템 페이지에 대한 정보 리턴 <br>
	@param CONTAINER_TYPE
	@return NONE
	*/
	CONTAINER_TYPE	GetCurrentItemPage()	{ return m_iCurrentInventoryType;};

	/**
	* 현재 장비 페이지에 대한 정보 리턴 <br>
	@param CONTAINER_TYPE
	@return NONE
	*/
	CONTAINER_TYPE	GetCurrentEquipPage()	{ return m_iCurrentFigureType;};		

	/**
	* 확장슬롯을 설정한다 <br> 
	* 확장슬롯은 캐릭터 선택시 정보로 들어와야 한다 <br> 
	* 뒤 에 주어진 사이즈 만큼 더 생성한다 <br> 
	@param CONTAINER_TYPE iType
	@param int iExtendSize = 0
	@return NONE
	*/
	void SetExtendSlot(CONTAINER_TYPE iType, int iTotalCount = 0, int iExtendSize = 0);

	/**
	 * 디버깅용 <br> 
	 @param
	 @return
	*/
	void PrintInventoryInfo();

	/**
	 * player 인벤토리 공간을 만든다 <br> 
	 @param CONTAINER_TYPE iType				컨테이너 종류
	 @param std::vector<SPItem*>* vpInventory	데이터리스트를 지닐 인벤토리 백터 포인터
	 @return
	*/
	bool CreateInventory(CONTAINER_TYPE iType, std::vector<SPItem*>* vpInventory, int iSize = DEFAULT_INVENTORY_SLOT);	

	/**
	* 모든 인벤토리의 라이프 타입의 아이템의 시간을 감소시킨다 <br> 
	@param int iTime = 60					//이벤트 시점에 일괄적으로 60초 감소
	@return
	*/
	void DecreaseLifeTime(int iTime = 60);

	/**
	*  아이템이 존재하는지 확인한다 <br>
	@param int iIndex
	@param CONTAINER_TYPE iType
	@return bool 존재시 true
	*/
	bool IsItemExist(int iIndex, CONTAINER_TYPE iType);

	/**
	 * 인벤토리 공간에서 아이템을 얻어온다 <br>
	 @param CONTAINER_TYPE iType		컨테이너 타입
	 @param int iSlotIndex				슬롯 인덱스
	 @return SPItem*
	*/
	SPItem* GetInventoryItem(CONTAINER_TYPE iType, int iSlotIndex);

	/**
	* 들어올린 아이템의 위지 정보데이터를 가져온다 <br>
	@param NONE
	@return SPSelectItem 들어올린 아이템의 위치 정보
	*/
	SPSelectItem GetPickupItem();

	/**
	 * 해당 컨테이너의 인벤토리 크기를 얻어온다 <br>
	 @param CONTAINER_TYPE iType
	 @return int
	*/
	int	GetInventorySize(CONTAINER_TYPE iType);

	/**
	 * 해당 컨테인의 현재 페이지(스크롤바 라인)를 얻어온다 <br>
	 @param CONTAINER_TYPE iType
	 @return int
	*/
	int GetInventoryCurPage(CONTAINER_TYPE iType);

	/**
	* 해당 컨테이너의 인벤토리슬롯이 모두 아이템으로 차 있는지 확인한다 <br> 
	@param CONTAINER_TYPE iContainerType
	@return bool 찬경우 true
	*/
	bool IsFullInventory(CONTAINER_TYPE iContainerType);

	/**
	 * 해당 컨테이너의 인벤토리슬로이 완전히 비어있는지 확인한다 <br> 
	 @param CONTAINER_TYPE iContainerType
	 @return  모두 검사해서 아무것도 없어야 true
	*/
	bool IsEmptyInventory(CONTAINER_TYPE iContainerType);

	/**
	* 해당 컨테이너의 인벤토리 슬롯에 해당 아이템이 존재하며 <br>
	* 해당 아이템의 스텍에 여유가 있는지 확인한다 <br>
	@param CONTAINER_TYPE iContainerType 컨테이너 타입
	@param int iItemID 확인할 아이템 ID
	@return true인 경우 여분의 공간이 있음
	*/
	bool IsEnableSpace(CONTAINER_TYPE iContainerType, int iItemID, int iStack = 1);

	/**
	 * 해당 컨테이너의 인벤토리에 비어 있는 슬롯이 몇개인지 확인한다 <br>
	 @param CONTAINER_TYPE iContainerType
	 @return int
	*/
	int GetEmptySlotCount(CONTAINER_TYPE iContainerType);

	/**
	 * 주어진 ItemID 와 Stack갯수를 만족(같거나 큰) 슬롯을 반환 <br>
	 * Inventory만 확인 만족한 Item을 찾은 경우 true값과 pSelectItem에 정보를 기록하여 반환 <br> 
	 * 앞에서 부터 찾음 <br>
	 @param int iItemID
	 @param int iStack = 1
	 @param SPSelectItem* pSelectItem = NULL			true시 내부에 정보를 기록
	 @return  bool
	*/
	bool FindInvenItem(int iItemID, int iStack = 1, SPSelectItem* pSelectItem = NULL);


	/**  
	 * 주어진 ItemID와 Stack갯수를 만족하는 SPItem을 반환 <br> 
	 * 사용자의 필요에 의해 해당 슬롯의 SPItem을 Block가능 <br> 
	 @param int iItemID
	 @param int iStack = 1
	 @param UINT8 iRare = 1				(1이하 경우 체크 안함)
	 @param int iBlockIndex = MAX_ITEM_BLOCK (0 ~ MAX_ITEM_BLOCK의 수를 지정하면 아이템을 블럭)
	 @return 
	*/  
	SPItem* GetFindItem(int iItemID, int iStack = 1, int iRare = 0, int iBlockIndex = MAX_ITEM_BLOCK);

	//
	SPItem * GetInvenItem(int iItemID);   //[xialin 2008/07/25]add

	/**
	 * 주어진 ItemID 의 해당하는 SetItem을 몇개 착용하고 있는지 확인 <br> 
	 @param int iItemID
	 @return  int
	*/
	int GetSetItemPieces(int iItemID);

	/**
	 * 주어진 ItemID에 해당하는 아이템이 셋트 아이템중 하나인지 확인 <br>
	 @param int iItemID
	 @return  bool
	*/
	bool IsSetItem(int iItemID);

	/**
	 * 주어진 ItemID에 해당하는 Set의 현재 착용 갯수를 갱신해준다 <br>
	 * 갱신된 착용갯수는 ItemCluster의 SPItemSet->m_iCurrentPieces 에 기록한다 <br>
	 @param int iItemID
	 @return  
	*/
	void CheckSetItem(int iItemID);
	
	/**
	* Quick Slot에서 아이템 사용할경우... <br>
	@param int iItemID
	@param CONTAINER_TYPE iContainerType
	@return bool
	*/
	bool ItemQuickUse(CONTAINER_TYPE iContainerType, int iItemID);
	
	/**
	 * 아이템 사용 <br> 
	 * 함수 내부에서 예외처리를 한뒤 서버로 전송하도록 한다 <br>
	 @param CONTAINER_TYPE iContainerType
	 @param int iSlotIndex
	 @return bool
	*/
	bool ItemUse(CONTAINER_TYPE iContainerType, int iSlotIndex);
	
	/**
	 * 아이템 사용패킷을 보내기전에 다른 작업을 해주어야 하는 경우 처리 <br>
	 * 현재는 Mic아이템만 해당한다 <br> 
	 @param CONTAINER_TYPE iContainerType
	 @param int iSlotIndex
	 @return bool  
	*/
	bool ItemPreUse(CONTAINER_TYPE iContainerType, int iSlotIndex);
	
	bool ItemPreUseEx( CONTAINER_TYPE iContainerType, int iSlotIndex );					//冷훙

	bool FWItemPreUseEx( CONTAINER_TYPE iContainerType, int iSlotIndex );

	bool SItemPreUseEx( CONTAINER_TYPE iContainerType, int iSlotIndex );
	/**
	* 쿨타임을 돌린다 <br>
	@param TRASH_ITEM item
	@return bool
	*/
	bool RunCoolTime(TRASH_ITEM item);
	
	/**
	 * 탭을 눌렀을때 서버로 활성무기슬롯을 변경하도록 요청한다 <br>
	 @param NONE
	 @return int
	*/
	int	SendReqWeaponExchange();

	/**
	* 활성된 무기 슬롯 위치를 얻어온다 1, 2 <br>
	@param NONE
	@return int
	*/
	int	GetActiveWeaponSlot();

	/**
	* 판매창에 올라간 아이템의 위치 정보를 기억시키며 <br>
	* 해당 위치의 아이콘을 비활성화로 표시하도록 설정한다 <br>
	@param SPSelectItem SellItem		판매할 아이템 위치 정보
	@return NONE
	*/
	void SetSellSelectItem(SPSelectItem SellItem, bool bSell = true);

	/**
	 * 컨테이너 내 동일한 ItemID를 지니는 SPItem* 를 얻어온다 <br> 
	 * Quick Slot용 <br> 
	 @param CONTAINER_TYPE iType
	 @param int iItemID
	 @param int iStack					주어진 아이템의 스택과 크거나 같아야 한다 디폴트 1개
	 @return SPItem*
	*/
	SPItem* GetSPItem(CONTAINER_TYPE iType, int iItemID, int iStack = 1);
	
	/**
	 * 해당 컨테이너 내 동일한 ItemID를 지니는 것들의 총 Stack Count 를 얻어옴<br> 
	 @param CONTAINER_TYPE iType
	 @param int iItemID
	 @param bool bCheckRequire			true인 경우 시간제한에 걸린 아이템은 제외한다
	 @return int
	*/
	int	GetTotalStackCount(CONTAINER_TYPE iType, int iItemID, bool bCheckRequire = true);	

	/**
	 * 해당 컨테이너 내 주어진 아이템의 스택이 조건보다 크거나 같은지 확인 <br> 
	 * 슬롯 개별로 체크한다 <br>
	 * 퀘스트 용 <br>
	 @param CONTAINER_TYPE iType		컨테이너 타입
	 @param int iItemID					아이템 아이디
	 @param int iCheckStack				확인할 스택수
	 @return bool						iCheckStack보다 크면 true
	*/
	bool IsBiggerStack(CONTAINER_TYPE iType, int iItemID, int iCheckStack);

    /**
	* 해당 컨테이너 내 주어진 아이템의 스택이 조건보다 크거나 같은지 확인 <br> 
	* 처음 슬롯부터 나오는 순서대로 5개를 더해 체크한다 <br>
	* 퀘스트 용 <br>
	@param CONTAINER_TYPE iType		컨테이너 타입
	@param int iItemID					아이템 아이디
	@param int iCheckStack				확인할 스택수
	@return bool						iCheckStack보다 크면 true
    */
	bool IsBiggerStack5(CONTAINER_TYPE iType, int iItemID, int iCheckStack);

	/**
	 * 주어진 타입에 의거 장비(전투,패션)의 착용 현황을 확인한다 <br>	 
	 @param  int iCheckType			1, 3인 경우 전투 2,4인경우 패션
	 @param  int iCheckSlotIndex = 0  EQUIP_SLOT_MAX - 1 인 경우 모든 슬롯 그외 0-15까지 슬롯체크
	 @return  bool					조건이 충족한 경우 true
	*/
	bool CheckEquipInven(int iCheckType, int iCheckSlotIndex = 0);

	/**
	 * 주어진 타입에 의거 장비의 내부 모션 ID값을 확인한다 <br>
	 @param  int iCheckType			1, 3인 경우 전투 2,4인경우 패션
	 @param  int iCheckSlotIndex = 0  0-15까지 슬롯체크
     @param	 int iCheckMotionID = 0	확인할 모션 값
	 @return  bool					조건이 충족한 경우 true
	*/
	bool CheckEquipMotion(int iCheckType, int iCheckSlotIndex = 0, int iCheckMotionID = 0);

	/**
	 * 픽업한 아이템의 ItemID를 얻어온다 <br>
	 * Quick Slot용 <br>
	 @param NONE
	 @return int (0인경우 뭔가 문제가 있는것임)
	*/
	int GetPickUpItemID();

	/**
	* 파괴 아이템으로 등록 된것인지 확인 <br>
	@param 
	@return 
	*/
	bool IsTrashItem() {
		return m_TrashItem.IsNull();
	}

	/**
	* 아이템을 장착할시 장창 가능성에 관한 조건 검사를 한다 <br>
	@param int iItemID = 0 검사할 아이템 ID (0인경우 Pick Up한 아이템을 검사)
	@param bool bPvp = false	PVP 상황의 체크여부 (true인경우 Pvp 조건 체크)
	@return bool
	*/
	bool CheckPCStatus(int iItemID = 0, bool bPvp = false);

	/**  
	 * 인벤토리 장비, 패션에 결혼 아이템을 착용하고 있는지 확인한다 <br>
	 @param  
	 @return bool								착용시 true
	*/  
	bool CheckEquipWeddingItem();

	/**
	 * 트레이드에 올라간 모든 아이템 정보 리스트를 삭제한다 <br>
	 @param NONE
	 @return NONE
	*/
	void ClearAllTradeItem();

	/**
	 * 해당인덱스의 트레이드 아이템 정보를 삭제한다 <br>
	 @param int iIndex = -1
	 @return bool
	*/
	bool ClearTradeItem(int iIndex = -1);

	/**
	 * 현재 픽업된 아이템이 존재한다면 트레이드가 가능한 아이템인지를 확인한다 <br> 
	 @param
	 @return
	*/
	bool IsTradeEnable(CONTAINER_TYPE iType = CONTAINER_TYPE_INVALID, int iSlotIndex = -1);

	/**
	 * 해당 트레이드 아이템 정보에 아이템 정보를 설정한다 <br>
	 @param int iIndex
	 @param CONTAINER_TYPE iType = CONTAINER_TYPE_INVALID
	 @param int iSlotIndex = -1
	 @return bool
	*/
	bool SetTradeItem(int iIndex, CONTAINER_TYPE iType = CONTAINER_TYPE_INVALID, int iSlotIndex = -1);

	/**
	 * 해당인덱스의 트레이드 아이템 정보를 가져온다 <br>
	 @param int iIndex
	 @return SPSelectItem*
	*/
	SPSelectItem* GetTradeItem(int iIndex);

	/**
	 * 해당 컨테이너의 슬롯 아이템이 현재 트레이드 상 아이템인지를 확인한다 <br>
	 @param CONTAINER_TYPE iType
	 @param int iSlotIndex
	 @return int 트레이드 리스트상의 인덱스 (없는경우 -1)
	*/
	int IsTradeItem(CONTAINER_TYPE iType, int iSlotIndex);
	
	/**
	 * 개인상점에 올라간 모든 아이템 정보 리스트를 삭제한다 <br>
	 @param 
	 @return
	*/
	void ClearAllUserShopItem();
	
	/**
	 * 해당인덱스에 올린 아이템 정보 를 삭제한다 <br>
	 @param 
	 @return
	*/
	bool ClearUserShopItem(int iIndex = -1);
	
	/**
	 * 개인상점 아이템 올림 설정 <br>
	 @param int iIndex					개인상점 리스트상 인덱스
	 @param CONTAINER_TYPE iType		해당 아이템의 컨테이너 타입
	 @param int iSlotIndex				해당 아이템의 슬롯 인덱스
	 @return bool
	*/
	bool SetUserShopItem(int iIndex, CONTAINER_TYPE iType = CONTAINER_TYPE_INVALID, int iSlotIndex = -1);

	/**
	 * 아이템을 픽업 시키지 못하도록 하는 경우 블럭을 위해 사용 <br>
	 * 정보저장 공간이 개인상점 공간과 같지만 판매 가능 여부 체크를 무시하도록 되어 있음 <br>
	 @param int iIndex					개인상점 리스트상 인덱스
	 @param CONTAINER_TYPE iType		해당 아이템의 컨테이너 타입
	 @param int iSlotIndex				해당 아이템의 슬롯 인덱스
	 @return bool
	 @warning SetBlockItem 을 이용할것
	*/
	bool SetUserShopItemFake(int iIndex, CONTAINER_TYPE iType = CONTAINER_TYPE_INVALID, int iSlotIndex = -1);
	
	/**
	 * 개인상점 리스트상의 올린 아이템 정보를 확인 <br>
	 @param int iIndex
	 @return SPSelectItem*
	*/
	SPSelectItem* GetUserShopItem(int iIndex);
	
	/**
	 * 주어진 위치의 인벤토리 아이템이 현재 개인상점에 올라가 있는지 확인한다 <br>
	 @param CONTAINER_TYPE iType
	 @param int iSlotIndex
	 @return int  -1인 경우 등록되어 있지 않은 아이템
	*/
	int	IsUserShopItem(CONTAINER_TYPE iType, int iSlotIndex);

	void ClearAllPetShopItem( PET_PLACE iPlace );
	bool ClearPetShopItem( PET_PLACE iPlace, int iIndex = -1 );
	bool SetPetShopItem( PET_PLACE iPlace, int iIndex, CONTAINER_TYPE iType = CONTAINER_TYPE_INVALID, int iSlotIndex = -1 );
	bool SetPetShopItemFake( PET_PLACE iPlace, int iIndex, CONTAINER_TYPE iType = CONTAINER_TYPE_INVALID, int iSlotIndex = -1 );
	SPSelectItem *GetPetShopItem( PET_PLACE iPlace, int iIndex );
	int IsPetShopItem( CONTAINER_TYPE iType, int iSlotIndex );
	
	/**
	 * 아이템을 픽업 시키지 못하도록 하는 경우 블럭을 위해 사용 <br>
	 @param 
	 @return  
	*/
	bool SetBlockItem(int iIndex, CONTAINER_TYPE iType = CONTAINER_TYPE_INVALID, int iSlotIndex = -1);
	
	/**
	 * 모든 블럭 아이템을 풀어준다 <br>
	 @param 
	 @return  
	*/
	void ClearAllBlockItem();
	
	/**
	 * 주어진 위치의 아이템의 블럭을 풀어준다 <br>
	 @param 
	 @return  
	*/
	bool ClearBlockItem(int iIndex = -1);
	
	/**
	 * 주어진 위치의 블럭된 아이템 정보를 확인한다 <br>
	 @param 
	 @return  
	*/
	SPSelectItem* GetBlockItem(int iIndex);
	
	/**
	 * 주어진 위치의 아이템이 현재 블럭되어 있는지 확인한다 <br> 
	 @param 
	 @return  
	*/
	int	IsBlockItem(CONTAINER_TYPE iType, int iSlotIndex);

	/**
	 * 펫 먹이 아이템으로 등록 <br>
	 * 등록시 아이콘 고정 <br> 
	 @param CONTAINER_TYPE iType
	 @param int iSlotIndex
	 @return bool
	*/
	bool SetPetFeedItem(PET_PLACE iPlace, CONTAINER_TYPE iType, int iSlotIndex);

	/**
	 * 펫 먹이 아이템으로 등록 해제 <br>
	 @param NULL
	 @return NULL
	*/
	void ClearPetFeedItem( PET_PLACE iPlace );

	void ClearPetFeedAll( void );

	bool SetPetFeedEnergyItem( PET_PLACE iPlace, CONTAINER_TYPE iType, int iSlotIndex );
	void ClearPetFeedEnergyItem( PET_PLACE iPlace );
	void ClearPetFeedEnergyAll( void );

	/**
	 * 펫 먹이용으로 등록된 아이템인지 확인<br>
	 @param CONTAINER_TYPE iType
	 @param int iSlotIndex
	 @return bool
	*/
	bool IsPetFeedItem(CONTAINER_TYPE iType, int iSlotIndex);
	bool IsPetFeedEnergyItem(CONTAINER_TYPE iType, int iSlotIndex);

	/**
	 * 외치기 아이템 사용시 설정<br>
	 * 등록시 아이콘 고정 <br> 
	 @param
	 @return
	*/
	bool SetMicItem(CONTAINER_TYPE iType, int iSlotIndex);

	bool SetFindItem( CONTAINER_TYPE iType, int iSlotIndex );
	
	bool SetFollowItem( CONTAINER_TYPE iType, int iSlotIndex );

	bool SetSnatchItem( CONTAINER_TYPE iType, int iSlotIndex );
	/**
	 * 외치기 아이템 사용 해제 <br>
	 @param
	 @return
	*/
	void ClearMicItem();
	
	void ClearFindItem();

	void ClearFollowItem();

	void ClearSnatchItem();
	/**
	 * 외치기 아이템으로 설정 되어 있는지 확인한다 <br>
	 @param
	 @return
	*/
	bool IsMicItem(CONTAINER_TYPE iType, int iSlotIndex);
	
	bool IsFindItem( CONTAINER_TYPE iType, int iSlotIndex );

	bool IsFollowItem( CONTAINER_TYPE iType, int iSlotIndex );

	bool IsSnatchItem( CONTAINER_TYPE iType, int iSlotIndex );
	/**
	 * 현재 Notice Box의 사용상황을 확인한다 <br>
	 @param NONE
	 @return NOTICE_USE
	*/
	NOTICE_USE GetCurNoticeUse();
	
	/**
	 * 현재 Notice Box의 사용상황을 설정한다 <br>
	 @param NOTICE_USE iNoticeUse
	 @return NONE
	*/
	void SetNoticeUse(NOTICE_USE iNoticeUse);	
	
	/**
	 * 해당인벤토리 타입의 최대 페이지 를 확인 <br>
	 * 페이지는 스크롤 라인을 의미 <br> 
	 @param int iType		컨테이너 타입
	 @return  int
	*/
	int GetMaxInvenPage(int iType);

	/**
	* 마이크 아이템을 사용하는 경우 패킷을 보냄 <br>
	* 호출은 마이크 인터페이스에서 함 <br>
	@param std::string Msg = ""		함께 보내는 메시지
	@return bool
	*/
	bool SendMicItemUseMsg(std::string Msg = "");

	bool SendBugle( std::string Msg = "");
	
	bool SendWhere ( std::string Msg = "", int iID = 0, WINDOW_STATE WinState = WINDOW_NULL );//[liuyang, 2008.9.2]add
		
	bool SendWhereEx( std::string Msg = "", int iID = 0, WINDOW_STATE WinState = WINDOW_NULL );//[liuyang, 2008.9.8]add
	
	bool FollowPeople( std::string Msg = "" );//[liuyang, 2008.10.10]

	bool SnatchFriend( std::string Msg = "" );//[liuyang, 2008.10.10]
protected:
	/**  
	*  
	@param  
	@return 
	*/  
	inline bool IsEnableContainer(CONTAINER_TYPE iContainerType);

	/**
	* 인벤토리 공간 생성 <br>
	@param NONE
	@return NONE
	*/
	void InitInventory();

	/**
	* 장비 공간 생성 <br> 
	@param NONE
	@return NONE
	*/
	void InitFigure();

	/**
	* 창고 공간 생성 <br>
	@param NONE
	@return NONE
	*/
	void InitStorage();
	
	/**
	 * 캐시 인벤 공간 생성 <br>
	 @param 
	 @return  
	*/
	void InitCashItem();


	//[xialin 2008/07/21] add
	void InitWearItem();

	/**
	 * PET 장비 공간 생성 <br>  
	 @param 
	 @return
	*/
	void InitPetEquip();

	/**
	 * 생성한 모든 Inventory를 제거한다 <br>
	 @param NONE
	 @return NONE
	*/
	void ClearAllInventory();

	/**
	 * 생성한 모든 Figure Inventory를 제거한다 <br>
	 @param NONE
	 @return NONE
	*/
	void ClearAllFigure();

	/**
	 * 생성한 모든 Storage 를 제거한다 <br>
	 @param NONE
	 @return NONE
	*/
	void ClearAllStorage();

	/**
	 * 생성한 모든 Cash 를 제거한다 <br>
	 @param 
	 @return  
	*/
	void ClearCashItem();	

	/**
	 * 생성한 모든 Pet 장비를 제거한다 <br> 
	 @param 
	 @return
	*/
	void ClearPetEquip();

	/**
	* 인벤토리의 경우 스크롤바를 설정 <br>
	@param NONE
	@return NONE
	*/
	void SetItemScroll(CONTAINER_TYPE iContainerType = CONTAINER_TYPE_INVALID);

	/**
	 * 마우스 아이콘을 설정한다 <br>
	 @param NONE
	 @return bool
	*/
	bool SetMouseIcon(/*SPItem* pItem*/);

	/**
	 * Pick Up한 아이콘을 원래 포지션으로 복구 한다 <br>
	 @param NONE
	 @return bool
	*/
	bool RecoverIcon();

	/**
	* 들어올린 아이템을 지운다. <br>
	* 픽업 상태를 해제하고, 픽업 위치 정보를 지우며<br>
	* 마우스 커서에 올라간 아이콘을 지운다 <br>
	@param NONE
	@return NONE
	*/
	void DeletePickupItem();	

	/** 
	 * 인벤토리 아이콘을 설정한다 <br>
	 @param CONTAINER_ITEM *pContainerItem
	 @return bool
	 @warning 내부에 꽤 귀찮은 예외처리가 많다
	*/
	bool SetInventoryIcon(CONTAINER_ITEM *pContainerItem, bool bSetEquip = true);

	/**
	* 옷이 벗겨지는 경우 처리 <br>
	@param CONTAINER_ITEM *pContainerItem
	@return bool
	*/
	bool SetEquipTakeOff( CONTAINER_ITEM *pContainerItem );

	/**
	 * Disable 되어야 할 아이템들의 슬롯을 반투명으로 표시하도록 한다 <br>
	 * Npc Sell, PC Trade 에 올라가있는 아이템들 <br>
	 * OnChangeInventory 에서 호출하며 Scroll 이 붙으면 수정해야 할것이다 <br>
	 @param NONE
	 @return NONE
	*/
	void SetDisableIcon();
	
	/**
	 * 해당 아이템이 차지하는 장비(?) 슬롯 칸수
	 @param
	 @return
	*/
	int GetSlotCount(CONTAINER_TYPE iType, int iSlotIndex);

	/**
	* 주어진 setID 의 셋트 아이템을 몇개 착용하고 있는지 확인 <br>
	* FIGURE_EQUIP,	FIGURE_FASHION 를 검색하여 확인 <br>
	 @param int iSetID 확인할 SetItemID
	 @return  int 착용하고 있는 해당 셋아이템의 갯수 <br>
	*/
	int GetTotalSetItemPieces(int iSetID);

	/**
	 * 아이템 사용 메시지를 서버에 전송한다 <br>
	 @param CONTAINER_TYPE iContainerType
	 @param int iSlotIndex
	 @return bool
	*/
	bool SendItemUseMsg(CONTAINER_TYPE iContainerType, int iSlotIndex);	

	/**
	 * 퀵슬롯에 아이템 컨테이너가 변경됨을 알림 <br> 
	 @param int iItemID
	 @return bool
	*/
	bool SendQuickSlotUpdate(int iItemID);		

	/**
	 * 아이템 판매창, 우편창 등에 링크되는 아이템들을 초기화 한다 <br> 
	 @param 
	 @return
	*/
	void ClearOutSideControl();

	/**
	 * 로컬 플레이어의 장비, 패션 정보를 업데이트 한다 <br> 
	 * 인터페이스 표현과 무관한 <br>
	 * 피규어 외형만 업데이트 한다 <br> 
	 @param 
	 @return
	*/
	void ReflashFigure();	
	

public:
	bool				m_bPickup;					/**< 아이템이 들여 올려졌는지 <br> */

private:
	SPGameObject*		m_pParent;					/**< 상위 게임 Object (Player) <br> */
	
	bool				m_bClickMove;				/**< 드래그 상태 <br> */
	int					m_iItemLClickIndex;			/**< LButton으로 클릭된 인벤토리 인덱스 <br> */
	int					m_iEquipLClickIndex;		/**< LButton으로 클릭된 장비 인덱스 <br> */
	int					m_iStorageLClickIndex;		/**< LButton으로 클릭된 창고 인덱스 <br> */
	int					m_iPetLClickIndex;			/**< LButton으로 클릭된 펫 인덱스 <br> */	
	
	int					m_iCurrentWeaponSlot;		/**< 현재 선택된 무기 슬롯 <br> */

	std::vector<SPItem*> m_vpInventory[CONTAINER_MAX];	/**< 인벤토리, 장비, 창고 의 모든 저장 공간 <br> */

	int m_iInventorySlot[CONTAINER_MAX];			/**< 인벤토리 슬롯 수 <br> */	
	int m_iExtendSlot[CONTAINER_MAX];				/**< 인벤토리 저장 공간 확장 <br> */
	int m_iInvenPage[CONTAINER_MAX];				/**< 인벤토리 Type별 스크롤 페이지 사이즈 <br> */	
	int m_iCurInvenPage[CONTAINER_MAX];				/**< 인벤토리 Type별 스크롤 위치 기억용 <br> */
	
	int m_iStorageSlot;								/**< 창고 저장 공간 <br> */
	int m_iStorageExtendSlot;						/**< 창고 확장 공간 <br> */
	int m_iStoragePage;								/**< 창고 스크롤 페이지 사이즈 <br> */
	int m_iCurStoragePage;							/**< 현재 창고 스크롤 페이지 사이즈 <br> */
	
	//[2005/6/21] Current Select Container
	CONTAINER_TYPE m_iCurrentContainer;				/**< 모든 컨테이너중 현재 선택된것 <br> */
	CONTAINER_TYPE m_iCurrentInventoryType;			/**< 현재 선택된 인벤토리 창 타입 <br> */
	CONTAINER_TYPE m_iCurrentFigureType;			/**< 현재 선택된 장비 창 타입 <br> */

	//[2005/6/22]
	SPSelectItem m_PickUpItem;						/**< 현재 픽업된 아이템 링크 정보 <br> */	

	SPSelectItem m_TrashItem;						/**< 파괴를 하려고 올려놓은 아이템의 정보 <br> */
	
	SPSelectItem m_SellItem;						/**< 현재 판매 아이템 링크 정보 <br> */

	SPSelectItem m_MicItem;							/**< 외치기 아이템 링크 정보 <br> */
	
	SPSelectItem m_FindItem;						//冷훙
	
	SPSelectItem m_FollowItem;						//瀏吏

	SPSelectItem m_SnatchItem;						//婁훙

	SPSelectItem m_TradeItem[MAX_PC_TRADE_SLOT];	/**< 자신의 거래 아이템을 올리는데 사용 인터페이스 용도..<br> */
	
	SPSelectItem m_UserShopItem[FM_MAX_SELLITEM];	/**< 개인상점에 올려놓은 아이템 리스트 <br> */
	SPSelectItem m_PetShopItem[PET_PLACE_TYPE_NUMBER][FM_MAX_SELLITEM];	//!< pet's shop items

	struct t_PetFeed
	{
		SPSelectItem	feedNormal;
		SPSelectItem	feedEnergy;
	};
	t_PetFeed m_vecPetFeeds[PET_PLACE_TYPE_NUMBER];

	SPSelectItem m_BlockItem[MAX_ITEM_BLOCK];		/**< <br> */

	bool	m_bSendRequest;							/**< 자신이 거래 요청 패킷을 보냈는지 <br> true인 경우 패킷을 보내고 응답을 기다리는 상황 <br> */
	
	bool	m_bRequested;							/**< 자신의 거래 요청을 받아 거래요청 메시지 창이 뜬 경우 <br> */

	CHANGE_SLOT m_ChangeSlot;						/**< 아이템 분할의 경우에만 사용한다... <br> */

	NOTICE_USE	m_iNoticeUse;						/**< 현재 Notice창의 이용 용도 <br> */
};