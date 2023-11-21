#pragma once

class SPWindow;
class SPWindowStatic;
class SPWindowEdit;
class SPWindowButton;
class SPWindowSlider;

#define WIID_GIFT_TOP						(WIID_GIFT	+	1)
#define WIID_GIFT_CENTER					(WIID_GIFT	+	2)
#define WIID_GIFT_BOTTOM					(WIID_GIFT	+	3)

#define WIID_GIFT_TITLE						(WIID_GIFT	+	5)
#define WIID_GIFT_CLOSE						(WIID_GIFT	+	6)

#define WIID_GIFT_LINE_L_TOP				(WIID_GIFT	+	10)
#define WIID_GIFT_LINE_L_BOTTOM				(WIID_GIFT	+	11)
#define WIID_GIFT_LINE_R_TOP				(WIID_GIFT	+	15)
#define WIID_GIFT_LINE_R_BOTTOM				(WIID_GIFT	+	16)

#define WIID_GIFT_L_SKIN					(WIID_GIFT	+	20)
#define WIID_GIFT_R_SKIN					(WIID_GIFT	+	25)

#define WIID_GIFT_REWORD_IMG				(WIID_GIFT	+	30)
#define WIID_GIFT_LIST_IMG					(WIID_GIFT	+	30)

#define WIID_GIFT_LIST_SEL					(WIID_GIFT	+	90)
#define WIID_GIFT_LIST_ICON_LINE			(WIID_GIFT	+	100)
#define WIID_GIFT_LIST_ICON					(WIID_GIFT	+	110)		//리스트 아이콘
#define WIID_GIFT_LIST_NAME					(WIID_GIFT	+	120)		//리스트 명
#define WIID_GIFT_LIST_ITEM					(WIID_GIFT	+	130)		//리스트 아이템 표시
#define WIID_GIFT_LIST_ELY					(WIID_GIFT	+	140)		//리스트 돈 표시
#define WIID_GIFT_LIST_BTN					(WIID_GIFT	+	150)		//리스트 버튼
#define WIID_GIFT_LIST_SKIN					(WIID_GIFT	+	160)		//

#define WIID_GIFT_PREV						(WIID_GIFT	+	170)		//리스트 이전
#define WIID_GIFT_PAGE						(WIID_GIFT	+	175)		//리스트 페이지 정보
#define WIID_GIFT_NEXT						(WIID_GIFT	+	180)		//리스트 다음
#define WIID_GIFT_NAME						(WIID_GIFT	+	185)		//내용 이름
#define WIID_GIFT_REFLASH 					(WIID_GIFT	+	190)		//리스트 갱신
#define WIID_GIFT_RECEIVE					(WIID_GIFT	+	195)		//받기

#define WIID_GIFT_TEXT						(WIID_GIFT	+	200)		//내용 내용
#define WIID_GIFT_ELY						(WIID_GIFT	+	210)		//내용 엘리
#define WIID_GIFT_ELY_TEXT_IMG				(WIID_GIFT	+	211)		//보상금액 이미지
#define WIID_GIFT_ELY_IMG					(WIID_GIFT	+	212)		//엘리 이미지

enum ICON_MARK;

const int MAX_GIFT_ITEM_PAGE = 6;			/**< 페이지에 표시되는 Gift Item의 갯수 <br> */

class SPWindowGift : public SPWindow 
{
public:
	SPWindowGift( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	virtual ~SPWindowGift();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

protected:
	void InitSubWindow();
	
	/**
	 * Gfit List 갱신 <br>
	 @param 
	 @return
	*/
	void UpdateList();

	virtual void Show(bool bWithChild = true);
	//void Refresh();

	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnUpdateGiftList	(WPARAM wParam, LPARAM lParam);			//외부에서 리스트 갱신을 요구
	int OnListReflash		(WPARAM wParam, LPARAM lParam);			//갱신버튼
	int OnItemReceive		(WPARAM wParam, LPARAM lParam);
	int OnClose				(WPARAM wParam, LPARAM lParam);
	int OnPrev				(WPARAM wParam, LPARAM lParam);
	int OnNext				(WPARAM wParam, LPARAM lParam);	
	int OnItemClick			( unsigned int uiID, WPARAM wParam, LPARAM lParam );
	int OnToolTipMove			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipEnter			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipOut			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);	

private:
	SPTexture* m_pDisable;

	SPWindow*		m_pName;
	SPWindow*		m_pText;
	SPWindow*		m_pPage;
	SPWindow*		m_pEly;
	SPWindowButton* m_pPrev;
	SPWindowButton* m_pNext;
	SPWindowButton*	m_pReflash;
	SPWindowButton* m_pReceive;    
	
	SPWindow*			m_pListSelect[MAX_GIFT_ITEM_PAGE];
	SPWindow*			m_pListIcon[MAX_GIFT_ITEM_PAGE];
	SPWindow*			m_pListName[MAX_GIFT_ITEM_PAGE];
	SPWindowButton*		m_pListButton[MAX_GIFT_ITEM_PAGE];
	//SPWindow*			m_pListItem[MAX_GIFT_ITEM_PAGE];
	//SPWindow*			m_pListEly[MAX_GIFT_ITEM_PAGE];
	RECT				m_rtDestItem[MAX_GIFT_ITEM_PAGE];
	RECT				m_rtDestEly[MAX_GIFT_ITEM_PAGE];
	RECT				m_rtSrcItem;
	RECT				m_rtSrcEly;

	bool	m_bListRequest;				/**< 리플래쉬 요청 버튼을 눌렀는지 <br> */
	float	m_fListDeley;				/**< 리플래쉬 요청 버튼 비활성 시간 <br> */
	float	m_fListAccumulate;			/**< 리플래쉬 요청 시간 갱신용 <br> */

	int		m_iMaxPage;					/**< Gift Item List를 페이지화 해 나온 총 페이지수 <br> */
	int		m_iCurPage;					/**< 현재 선택한 페이지 수 <br> */
	int		m_iStartIndex;				/**< 리스트 표현을 위해 ? <br> */
	int		m_iSelectItem;				/**< 4개의 리스트중 선택된 아이템 <br> */

	bool		m_bAble[MAX_GIFT_ITEM_PAGE];		/**< 받은 Item이 사용가능한지 <br> */
	ICON_MARK	m_iItemMark[MAX_GIFT_ITEM_PAGE];	/**< 받은 Item의 마크 <br> */
	int			m_iStackCount[MAX_GIFT_ITEM_PAGE];	/**< 받은 Item의 갯수 <br> */

	RECT m_rtSrcDisable;							/**< 비활성 표현용 소스 <br> */
	RECT m_rtMarkSrc[4];							/**< ICON MARK Src <br> */
	RECT m_rtStack[MAX_GIFT_ITEM_PAGE];				/**< 스택표현시 참고 영역 Icon 영역과 동일 <br> */
};