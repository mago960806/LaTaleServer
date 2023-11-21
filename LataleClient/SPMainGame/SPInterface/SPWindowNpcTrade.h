#pragma once

#define WIID_NPCTRADE_TOP			(WIID_NPCTRADE	+	1)		//
#define WIID_NPCTRADE_CENTER		(WIID_NPCTRADE	+	2)		//
#define WIID_NPCTRADE_BOTTOM		(WIID_NPCTRADE	+	3)		//

#define WIID_NPCTRADE_TITLE			(WIID_NPCTRADE	+	5)		//타이틀
#define WIID_NPCTRADE_EXIT			(WIID_NPCTRADE	+	6)		//나가기

#define WIID_NPCTRADE_MIDDLE		(WIID_NPCTRADE	+	10)		//가운데 스킨

//#define WIID_NPCTRADE_COUNT			(WIID_NPCTRADE	+	10)	//Item 갯수 증가
//#define WIID_NPCTRADE_COUNTER		(WIID_NPCTRADE	+	15)		//Itme 갯수 표시
//#define WIID_NPCTRADE_DOWNCOUNT		(WIID_NPCTRADE	+	20)	//Item 갯수 감소

#define WIID_NPCTRADE_LIST_IMG_BASE (WIID_NPCTRADE	+	11)		//리스트 외곽 이미지
#define WIID_NPCTRADE_LIST_IMG		(WIID_NPCTRADE	+	12)		//리스트 이미지

#define WIID_NPCTRADE_PRICE_IMG_BASE	(WIID_NPCTRADE	+	15)	//가격 외곽 이미지
#define WIID_NPCTRADE_PRICE_IMG		(WIID_NPCTRADE	+	16)		//가격 이미지

#define WIID_NPCTRADE_BUY			(WIID_NPCTRADE	+	25)		//Item 구매 버튼

#define WIID_NPCTRADE_ITEM_SKIN		(WIID_NPCTRADE	+	30)		//6개씩 아이템 리스트 에 사용될 각 개별 아이템 바닥
#define WIID_NPCTRADE_ITEM_SEL		(WIID_NPCTRADE	+	50)		//6개씩 아이템 리스트 선택시 셀렉트 
#define WIID_NPCTRADE_ITEM_GRID		(WIID_NPCTRADE	+	70)		//6개씩 아이템 아이콘 바닥 이미지 
#define WIID_NPCTRADE_ITEM_ICON		(WIID_NPCTRADE	+	90)		//6개씩 아이템 아이콘 버튼
#define WIID_NPCTRADE_ITEM_NAME		(WIID_NPCTRADE	+	110)	//6개씩 아이템 이름 표시
#define WIID_NPCTRADE_ITEM_CASH		(WIID_NPCTRADE	+	130)	//6개씩 아이템 가격 표시
#define WIID_NPCTRADE_ITEM_ELY_IMG	(WIID_NPCTRADE	+	150)	//6개씩 아이템 엘리 이미지
#define WIID_NPCTRADE_ITEM_BUTTON	(WIID_NPCTRADE	+	170)	//6개씩 아이템 내부 버튼

#define WIID_NPCTRADE_PAGEUP		(WIID_NPCTRADE	+	200)		//페이지 증가
#define WIID_NPCTRADE_PAGE			(WIID_NPCTRADE	+	210)		//페이지
#define WIID_NPCTRADE_PAGEDOWN		(WIID_NPCTRADE	+	220)		//페이지 감소

#define WIID_USER_ELY_TEXT_IMG		(WIID_NPCTRADE	+	230)		//페이지 감소
#define WIID_USER_ELY				(WIID_NPCTRADE	+	235)		//페이지 감소
#define WIID_USER_ELY_IMG			(WIID_NPCTRADE	+	236)		//페이지 감소


const int	TRADE_PAGE = 10;					/**< 한페이지에 보여지는 아이템의 갯수 사이즈 <br> */
const int	MAX_BUY_COUNT = 99;				/**< 한번에 구매 가능한 최대 아이템의 갯수 <br> */

class SPWindow;
class SPWindowEdit;
struct SPNpcShop;

#include "SPPlayerInvenArchive.h"


/**
* @class SPWindowShopTalk
* @brief NPC 상점 퀘스트의 경우 상단 메시지창
* @author Jinhee
* @version 00.00.01
* @date 2005.5.16
* @bug 없음
* @warning 
*/
class SPWindowNpcTrade : public SPWindow {

public:	
	SPWindowNpcTrade(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowNpcTrade();

	virtual void Init();
	virtual void Clean();
	//virtual void SetFocus();
	virtual void Process(float fTime);
	virtual void Render(float fTime);
	virtual void RenderText();

	virtual void Show(bool bWithChild = true);	
	//virtual bool IsShow();
	//virtual void Hide();  // 서브게임 내에서 만 쓰임..

public:

protected:
	/**
	 * 하위 컨트롤을 만든다 <br> 
	 @param NONE
	 @return NONE
	*/
	void CreateSubControl();
	
	/**
	 * 아이템 리스트 컨트롤들을 초기화 한다 <br>
	 @param NONE
	 @return NONE
	*/
	void ResetItemListControl();
	
	/**
	 * 아이템 리스트를 업데이트 한다 <br> 
	 @param NONE
	 @return NONE
	*/
	void UpdateItemList();	

	/**
	 * 로컬 PC의 스테이터스를 체크하여 NPC아이템리스트의 아이템이 <br>
	 * 현재 사용할수 있는지를 설정한다 <br>
	 @param int iItemID
	 @return 
	*/
	bool CheckItemStatus(int iItemID);

	/**
	 * 아이템 구매 카운터를 업데이트 한다 <br> 
	 * //[2006/6/23] 기능 제거
	 @param NONE
	 @return NONE
	*/
	//void UpdateBuyCount();	
	
	/**
	* ItemButton control List 의 select 상태를 모두 리셋한다 <br>
	@param NONE
	@return NONE
	*/
	void ResetItemButton();

	/**
	 * 판매후 또는 판매 로직 에러시 선택된 아이템 설정을 초기화 한다 <br>
	 @param
	 @return
	*/
	void ResetSelectItem();

	/**
	 * 빠른 판매용인지 확인한다 <br>
	 @param NONE
	 @return bool 판매 팻킷을 보낸경우 true
	*/
	bool IsQuickItemSell();

protected:
	// {{ ShopTalk Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	int OnUpdateShopData		( WPARAM wParam, LPARAM lParam );	//shop Data 를 업데이트
	int OnClose					( WPARAM wParam, LPARAM lParam );	//
	int OnPageUp				( WPARAM wParam, LPARAM lParam );	//Page Up
	int OnPageDown				( WPARAM wParam, LPARAM lParam );	//Page Down
	int OnCount					( WPARAM wParam, LPARAM lParam );	//Buy Count 증가
	int OnCountDown				( WPARAM wParam, LPARAM lParam );	//Buy Count 감소
	int OnUpdateCount			(WPARAM wParam, LPARAM lParam);		/**< Count 직접 입력시 <br> */
	int OnInputFocus			(WPARAM wParam, LPARAM lParam);		/**< 에디트 컨트롤이 포커스를 받는 경우 <br> */
	int OnBuyItem				( WPARAM wParam, LPARAM lParam );	//구매 버튼
	
	int OnLBuyItemClick			( unsigned int uiID, WPARAM wParam, LPARAM lParam );	//구매할 아이템 클릭 (컨트롤 클릭)
	int OnRBuyItemClick     	( unsigned int uiID, WPARAM wParam, LPARAM lParam );	//구매할 아이템 클릭 (컨트롤 클릭)

	int OnToolTipMove			(unsigned int, WPARAM, LPARAM);
	int OnToolTipEnter			(unsigned int, WPARAM, LPARAM);
	int OnToolTipOut			(unsigned int, WPARAM, LPARAM);
	int OnLButtonUp				(WPARAM wParam, LPARAM lParam);

	//virtual int OnShopTalkClose		( WPARAM wParam, LPARAM lParam );
	//virtual int OnShopTalkShow		( WPARAM wParam, LPARAM lParam );
	//virtual int OnShopTalkTextSet	( WPARAM wParam, LPARAM lParam );


protected:	
	SPTexture* m_pDisable;						/**< Disable 표현용 Texture <br> */	
	SPWindow* m_pBuy;							/**< button Window Buy <br> */ 
	
	SPWindowStatic* m_pSkin[TRADE_PAGE];
	SPWindow* m_pIcon[TRADE_PAGE];				/**< Static Window Item Icon <br> */	
	SPWindow* m_pName[TRADE_PAGE];				/**< Static Window Item Name <br> */
	SPWindow* m_pCash[TRADE_PAGE];				/**< Static window Item Cash <br> */
	SPWindow* m_pButton[TRADE_PAGE];			/**< button window Item Button <br> */

	RECT m_rtStack[TRADE_PAGE];
	int	m_iStack[TRADE_PAGE];					/**< Static 표현 <br> */
	
	SPWindow* m_pPageUp;						/**< Static Window Page up <br> */
	SPWindow* m_pPageInfo;						/**< Static window Page <br> */
	SPWindow* m_pPageDown;						/**< Static Window Page Down <br> */

	SPWindow* m_pUserEly;						/**< 유저 보유 Ely <br> */

	bool	m_bSend;							/**< Message Send <br> */

	SPNpcShop* m_pNpcShop;						/**< Static window Item Cash <br> */
	int	m_iMaxPage;								/**< MAX Page <br> */
	int m_iPage;								/**< current Page <br> */

	char m_szPage[64];							/**< page Info <br> */

	int m_iBuyCount;							/**< Buy Item Counter <br> */
	char m_szBuyCount[64];						/**< Buy Count Info <br> */
	
	int m_iSelectItemIndex;						/**< 실제 판매하기 위해 선택한 Item List상의 Index <br> */
	int m_iViewItemIndex;						/**< 컨트롤에 보여주기 위해 선택한 컨트롤 List상의 Index <br> */

	bool m_bAble[TRADE_PAGE];					/**< false일때 비활성화 표현 <br> */
	RECT m_rtSrcDisable;						/**< Diable Textrue Src <br> */

	SPSelectItem m_PickUpItem;					/**< 인벤토리에서 픽업된 아이템 링크 <br> */

	RECT m_rtMarkSrc[4];						/**< Icon Mark Src <br> */
	ICON_MARK m_iMark[TRADE_PAGE];				/**< Icon Mark <br> */

protected:


};
