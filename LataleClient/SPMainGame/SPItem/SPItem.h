#pragma once

#include <vector>
#include <string>

using namespace std;

struct SPItemAttr;
class SPItemStatus;

/**
* @class SPItem
* @brief 플레이어가 사용하는 Item 
* @author Jinhee
* @version 00.00.01
* @date 2005.6.16
* @bug 없음
* @warning 없음
*/
class SPItem {
public:
	/**
	 * 생성자
	 @param NONE
	 @return NONE
	*/
	SPItem();
	
	/**
	 * 생성자 <br> 
	 * 아이템이 사용될 Container Type 과 생성되는 SlotIndex(위치)를 지니고 생성 <br> 
	 @param CONTAINER_TYPE iContainerType
	 @parma int iSlotIndex
	 @return NONE
	*/
	SPItem(CONTAINER_TYPE iContainerType, int iSlotIndex);
	
	/**
	 * 제거 <br> 
	 @param NONE
	 @return NONE
	*/
	~SPItem();
	
	
	/**
	 * ItemAttr 을 설정 <br> 
	 * ContainerType과 Container_item 에 의거함 <br> 
	 * ItemStatus 에 의해서 결정 되어질수도 있음 <br> 
	 * 현재 내부 기능 없음... <br> 
	 @param CONTAINER_TYPE iContainerType
	 @param CONTAINER_ITEM* item = NULL
	 @return bool
	*/
	bool SetItemAttr(CONTAINER_TYPE iContainerType, CONTAINER_ITEM* item = NULL);

	/**
	 * ItemStatus 를 생성 <br>
	 * ContainerType에 의거해 생성함 <br> 
	 @param CONTAINER_TYPE iContainerType
	 @param CONTAINER_ITEM* item = NULL
	 @return bool
	*/
	bool CreateItemStatus(CONTAINER_TYPE iContainerType, CONTAINER_ITEM* item = NULL);

	/**
	 * ItemID와 Rear도를 가지고 임시의 ItemStatus공간을 만듬 <br>
	 * 툴팁용 <br>
	 @param int iItemID
	 @param int iRear = 1
	 @return bool 
	*/	
	bool SetItemStatus(int iItemID, int iRear = 1);

	/**
	 * ItemStatus 를 샛팅한다 <br>
	 @param CONTAINER_ITEM* item
	 @return bool
	*/
	bool SetItemStatus(CONTAINER_ITEM* item);

	/**
	 * ItemStatus 를 셋팅한다 <br>
	 @param ITEM* item
	 @return bool
	*/
	bool SetItemStatus(ITEM* Item, int iIndex = 0);

	/**
	 * ItemStatus 를 셋팅한다 <br>
	 @ param 
	 @ return
	*/
	bool SetItemStatus(FIGURE_ITEM* Item, int iIndex = 0);

	/**
	 * 아이템 설정 <br> 
	 * 컨테이너 타입과 슬롯 인덱스 변경용 <br> 
	 @param CONTAINER_TYPE iType
	 @param int iSlotIndex
	 @param SPItem* pItem = NULL
	 @return bool
	*/
	bool SetItem(CONTAINER_TYPE iType, int iSlotIndex, SPItem* pItem = NULL);

	/**
	 * 셋팅된 아이템을 클리어 해준다 <br>
	 @param  NONE
	 @return bool
	*/	
	bool SetClear();
	
	/**
	 * 아이템 Attribute 를 가져온다 <br> 
	 @param NONE
	 @return SPItemAttr*
	*/
	SPItemAttr*	GetItemAttr();

	/**
	 * 아이템 Status를 가져온다 <br>
	 @param NONE
	 @return SPItemStatus*
	*/
	SPItemStatus* GetItemStatus();

	/**
	 * 현 아이템이 비어 있는지 확인한다 <br>
	 @ param  NONE
	 @ return bool  true인 경우 비어 있는 아이템임 
	*/
	bool IsEmptyItem();

	/**
	 * 컨테이너 타입을 가져온다 <br>
	 @param NONE
	 @return CONTAINER_TYPE
	*/
	CONTAINER_TYPE GetContainerType();
	
	/**
	 * 슬롯 인덱스를 가져온다 <br>
	 @param NONE
	 @return int
	*/
	int	GetSlotIndex();

	/**
	 * 퀵 슬롯 인덱스를 가져온다 <br> 
	 @param NONE
	 @return int
	*/
	int GetQuickSlotIndex();
	
	/**
	 * SPItem을 CONTAINER_ITEM으로 재구성한다 <br> 
	 * SPPlayerInventory의 MakeContainerItem()함수와 비슷한 역할을 한다 <br> 
	 @param CONTAINER_ITEM& item
	 @return bool
	*/
	bool ConvertContainerItem(CONTAINER_ITEM* item);
	
	/**
	 * ItemStauts에 메시지를 보내 값을 받아온다 <br>
	 @param MODEL_VALUE_TYPE iMsgID
	 @return int
	*/
	int GetItemStatusValue(MODEL_VALUE_TYPE iMsgID);

	/**
	 * ItemStauts에 메시지를 보내 값을 설정한다 <br>
	 @param MODEL_VALUE_TYPE iMsgID
	 @param int iValue = 0
	 @return int
	*/
	int SetItemStatusValue(MODEL_VALUE_TYPE iMsgID, int iValue = 0);

	/**
	 * 
	 @param 
	 @return
	*/
	bool DecreaseLifeTime(int iTime);

protected:
	/**
	 * Item 맴버를 초기화 한다 <br> 
	 @param NONE
	 @return NONE
	*/
	void Init();
	
	/**
	* Item 정보를 지운다 
	@param NONE
	@return NONE
	*/
	void Clear();

private:
	//SPItemAttr*		m_pItemAttr;			/**< Item Table상의 설정 <br> */
	SPItemStatus*	m_pItemStatus;				/**< 실제 Player가 지닌 상황에서의 Item 상태 <br> */
	
	CONTAINER_TYPE	m_iContainerType;			/**< 아이템이 위치한 컨테이너의 타입 <br> */
	int				m_iSlotIndex;				/**< 아이템이 위치한 슬롯의 위치 <br> */

	unsigned int	m_iInstanceID;				/**< Server에서 사용할지도 모르는 Instance ID <br> */	
	
	int				m_iQuickSlotIndex;			/**< Quick Slot 에 올라간 경우 Quick Slot Index <br> */
};

