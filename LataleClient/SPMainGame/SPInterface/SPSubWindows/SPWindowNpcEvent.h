#pragma once


#define WIID_NPC_EVENT_TITLE							(WIID_NPC_EVENT + 1)
#define WIID_NPC_EVENT_EXIT								(WIID_NPC_EVENT + 5)
#define WIID_NPC_EVENT_LINE								(WIID_NPC_EVENT + 7)

#define WIID_NPC_EVENT_BASE								(WIID_NPC_EVENT + 9)

#define WIID_NPC_EVENT_LIST_TITLE_SKIN					(WIID_NPC_EVENT + 10)	//이벤트 아이템 목록 BG
#define WIID_NPC_EVENT_LIST_TITLE						(WIID_NPC_EVENT + 11)

#define WIID_NPC_EVENT_LIST_SKIN						(WIID_NPC_EVENT + 20)
#define WIID_NPC_EVENT_LIST_BUTTON						(WIID_NPC_EVENT + 30)
#define WIID_NPC_EVENT_LIST_ICONSKIN					(WIID_NPC_EVENT + 40)
#define WIID_NPC_EVENT_LIST_ICON						(WIID_NPC_EVENT + 50)
#define WIID_NPC_EVENT_LIST_NAME						(WIID_NPC_EVENT + 60)
#define WIID_NPC_EVENT_LIST_SELECT						(WIID_NPC_EVENT + 70)

#define WIID_NPC_EVENT_PAGE_SKIN						(WIID_NPC_EVENT + 100)
#define WIID_NPC_EVENT_PAGE_PREV						(WIID_NPC_EVENT + 101)
#define WIID_NPC_EVENT_PAGE_NEXT						(WIID_NPC_EVENT + 102)

#define WIID_NPC_EVENT_ARROW							(WIID_NPC_EVENT + 110)	//삼각 화살표 이미지

#define WIID_NPC_EVENT_REQ_ITEM_SKIN					(WIID_NPC_EVENT + 120)	//필요 아이템 영역 
#define WIID_NPC_EVENT_REQ_ITEM_TEXT_SKIN				(WIID_NPC_EVENT + 125)
#define WIID_NPC_EVENT_REQ_ITEM_TEXT					(WIID_NPC_EVENT + 126)

#define WIID_NPC_EVENT_REQ_ITEM_ELY_SKIN				(WIID_NPC_EVENT + 130)
#define WIID_NPC_EVENT_REQ_ITEM_ELY						(WIID_NPC_EVENT + 131)
#define WIID_NPC_EVENT_REQ_ITEM_ELY_IMG_L				(WIID_NPC_EVENT + 132)	//L ElyImage
#define WIID_NPC_EVENT_REQ_ITEM_ELY_IMG_R				(WIID_NPC_EVENT + 133)	//R ElyImage

#define WIID_NPC_EVENT_REQ_ITEM_PVP_SKIN				(WIID_NPC_EVENT + 135)
#define WIID_NPC_EVENT_REQ_ITEM_PVP						(WIID_NPC_EVENT + 136)
#define WIID_NPC_EVENT_REQ_ITEM_PVP_IMG_L				(WIID_NPC_EVENT + 137)	//L PvpPoint Image
#define WIID_NPC_EVENT_REQ_ITEM_PVP_IMG_R				(WIID_NPC_EVENT + 138)	//R PvpPoint Image

#define WIID_NPC_EVENT_REQ_ITEM_ICONSKIN				(WIID_NPC_EVENT + 140)	//필요 아이템 리스트 용
#define WIID_NPC_EVENT_REQ_ITEM_ICONBUTTON				(WIID_NPC_EVENT + 150)
#define WIID_NPC_EVENT_REQ_ITEM_ICON					(WIID_NPC_EVENT + 160)

#define WIID_NPC_EVENT_TIP_SKIN							(WIID_NPC_EVENT + 180)	//팁 출력 영역
#define WIID_NPC_EVENT_TIP								(WIID_NPC_EVENT + 181)
#define WIID_NPC_EVENT_REQ_ITEM_PLUS1					(WIID_NPC_EVENT + 181)	//+표시
#define WIID_NPC_EVENT_REQ_ITEM_PLUS2					(WIID_NPC_EVENT + 182)
#define WIID_NPC_EVENT_REQ_ITEM_PLUS3					(WIID_NPC_EVENT + 183)

#define WIID_NPC_EVENT_RECEIVE							(WIID_NPC_EVENT + 190)


const int EVENT_PAGE = 6;						/**< 한페이지에 보여지는 이벤트 아이템 목록의 갯수 <br> */
const int EVENT_REQ_ITEM = 4;					/**< 보여지는 필요 아이템 갯수 <br> 선택의 경우에만 4개 그외의 경우 2개 <br> */


#include "SPItemStatus.h"
struct SPNpcEventToolTip {
	std::string		m_strTitle;					/**< 타이틀 값이 있으면 아이템이 없음 <br> */
	SPItemStatus	m_pItem;	
	std::string		m_strDesc;

	SPNpcEventToolTip(){
		Clear();
	}

	~SPNpcEventToolTip(){
		Clear();
	}	

	void Clear();
	void SetTitle(std::string strTitle);
};

class SPWindow;
class SPWindowButton;
#include "SPEventDEF.h"


class SPWindowNpcEvent : public SPWindow {
public:
	SPWindowNpcEvent( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowNpcEvent();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

protected:
	/**
	 * 내부 컨트롤 생성 <br>
	 @param 
	 @return  
	*/
	void InitSubControl();
	
	/**
	 * 내부 컨트롤 표시 정보 초기화 <br>
	 @param 
	 @return  
	*/
	void ResetSubControl();
	
	/**
	 * 보상 리스트 컨트롤 갱신 <br>
	 @param 
	 @return  
	*/
	void UpdateRewordList();
	
	/**
	 * 주어진 인덱스의 리스트 컨트롤 정보 초기화 <br>
	 @param 
	 @return  
	*/
	inline void ClearRewordList(int iIndex);
	
	/**
	 * 주어진 인덱스의 리스트 컨트롤에 아이콘 셋팅 <br>
	 @param 
	 @return  
	*/
	bool SetRewordListIcon(SPNpcEventInfo* pInfo, int iListIndex);		
	
	/**
	 * 필요 아이템 리스트 정보를 초기화 <br>
	 @param 
	 @return  
	*/
	void ResetRequrieList();
	
	/**
	 * 주어진 인덱스의 필요 리스트 정보를 초기화 <br>
	 @param 
	 @return  
	*/
	inline void ClearRequireList(int iIndex);
	
	/**
	 * 필요 리스트 정보를 갱신 <br>
	 @param  
	 @return  
	*/
	void UpdateRequireList();
	
	/**
	 * 주어진 인덱스의 리스트 컨트롤에 아이콘 셋팅 <br>
	 @param 
	 @return  
	*/
	bool SetRequestListIcon(SPQuestItem* pItemInfo, int iListIndex);

	/**
	 * 필요 조건을 확인 <br>
	 * 상황에 따른 모든 조건을 확인한다 <br>
	 @param 
	 @return  
	*/
	bool CheckRequire();
	
	/**
	 * 필요 아이템을 확인 <br>
	 * 상황에 따른 모든 아이템과 Ely 조건을 확인한다 <br>
	 @param 
	 @return  
	*/
	bool CheckRequireItem();
	
	/**
	 * 보상을 받을 인벤토리 공간상황을 점검한다 <br>
	 @param 
	 @return  
	*/
	bool CheckRewordInvenSlot();
	
	/**
	 * 인벤토리 풀시 시스템 메시지 출력을 한다 <br>
	 @param 
	 @return  
	*/
	void SetInvenFullMsg(int i);

	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnExit						(WPARAM wParam, LPARAM lParam);						/**< 종료 버튼 <br> */
	int OnPrev						(WPARAM wParam, LPARAM lParam);						/**< 이전 버튼 <br> */
	int OnNext						(WPARAM wParam, LPARAM lParam);						/**< 다음 버튼 <br> */
	int OnReceive					(WPARAM wParam, LPARAM lParam);						/**< 받기 버튼 <br> */
	int OnItemClick					(unsigned int uiID, WPARAM wParam, LPARAM lParam);
	int OnRewordToolTipMove			(unsigned int, WPARAM, LPARAM);
	int OnRewordToolTipEnter		(unsigned int, WPARAM, LPARAM);
	int OnRewordToolTipOut			(unsigned int, WPARAM, LPARAM);
	int OnReqToolTipMove			(unsigned int, WPARAM, LPARAM);
	int OnReqToolTipEnter			(unsigned int, WPARAM, LPARAM);
	int OnReqToolTipOut				(unsigned int, WPARAM, LPARAM);


private:
	SPTexture*		m_pTextureBase;							/**< 윈도우 바탕용 리소스 <br> */
	RECT			m_rtSrcBase[WINDOW_BASE_MAX];			/**< 윈도우 바탕 소스 영역 <br> */
	RECT			m_rtDestBase[WINDOW_BASE_MAX];			/**< 윈도우 바탕 대상 <br> */	

	SPWindow*		m_pTitle;								/**< Title <br> */

	SPWindow*		m_pSelect[EVENT_PAGE];					/**< 리스트 선택표현용 <br> */
	SPWindow*		m_pButton[EVENT_PAGE];					/**< 리스트 이벤트 처리용 <br> */
	SPWindow*		m_pName[EVENT_PAGE];					/**< 이름 <br> */	
	SPWindow*		m_pItemSkin[EVENT_PAGE];
	SPWindow*		m_pItemIcon[EVENT_PAGE];				/**< 아이템 아이콘 <br> */

	SPWindow*		m_pReqItemButton[EVENT_REQ_ITEM];		/**< 필요 아이템 툴팁용 <br> */
	SPWindow*		m_pReqItemIcon[EVENT_REQ_ITEM];			/**< 필요 아이템 아이콘 <br> */
	
	SPWindow*		m_pPage;								/**< 보상 아이템 리스트 페이지 <br> */
	SPWindowButton*	m_pPrev;								/**< 보상 아이템 리스트 이전 버튼 <br> */
	SPWindowButton*	m_pNext;								/**< 보상 아이템 리스트 다음 버튼 <br> */

	SPWindow*		m_pReqEly;								/**< 필요 Ely <br> */
	UINT64			m_iTotalReqEly;							/**< 총 필요 Ely <br> 내부적 계산시 사용 <br> */

	SPWindow*		m_pReqPvpPoint;							/**< 필요 PvpPoint <br> */
	UINT32			m_iTotalReqPvpPoint;					/**< 총 필요 PvPPoint <br> 내부적 계산시 사용 <br> */

	SPWindow*		m_pTip;									/**< 팁 메시지 <br> */
	SPWindowButton*	m_pRecevie;								/**< 받기 버튼 <br> */

	int				m_iMaxPage;								/**< 최대 페이지 <br> */
	int				m_iCurPage;								/**< 현재 페이지 <br> */
	int				m_iSelect;								/**< 아이템 목록인터페이스 상 선택한 인덱스 <br> 랜덤, 고정인 경우 보여줄수 없다 <br> */
	int				m_iSelectIndex;							/**< 아이템 데이터상 선택한 인덱스 <br> */

	char			m_szPage[12];							/**< 페이지 문자열 <br> */

	SPTexture*		m_pDisable;								/**< Disable 표현용 Texture <br> */	
	RECT			m_rtSrcDisable;							/**< Diable Textrue Src <br> */
	RECT			m_rtMarkSrc[4];							/**< Icon Mark Src <br> */

	RECT			m_rtRewordStack[EVENT_PAGE];			/**< 보상 아이템 스택 표현 영역 <br> */
	int				m_iRewordStack[EVENT_PAGE];				/**< Static 표현 <br> */
	bool			m_bRewordAble[EVENT_PAGE];				/**< false일때 비활성화 표현 <br> */	
	ICON_MARK		m_iRewordMark[EVENT_PAGE];				/**< Icon Mark <br> */

	RECT			m_rtReqStack[EVENT_REQ_ITEM];
	int				m_iReqStack[EVENT_REQ_ITEM];
	bool			m_bReqAble[EVENT_REQ_ITEM];
	ICON_MARK		m_iReqMark[EVENT_REQ_ITEM];

	UINT8			m_iReqItemCount;						/**< 필요 아이템 갯수 <br> */
	TRASH_ITEM		m_ReqItem[EVENT_REQ_ITEM];				/**< 필요 아이템 슬롯 정보 <br> */
	UINT64			m_iReqItemNo[EVENT_REQ_ITEM];			/**< 필요 아이템 Item No 정보 <br> */
	SPQuestItem		m_ReqToolTipItem[EVENT_REQ_ITEM];		/**< 필요 아이템 툴팁용 정보 <br> */
};

