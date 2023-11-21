#pragma once


#define WIID_NPC_EVENTLIST_TITLE							(WIID_NPC_EVENTLIST + 1)
//#define WIID_NPC_EVENTLIST_EXIT								(WIID_NPC_EVENTLIST + 5)
#define WIID_NPC_EVENTLIST_LINE								(WIID_NPC_EVENTLIST + 7)

#define WIID_NPC_EVENTLIST_BASE								(WIID_NPC_EVENTLIST + 9)

//#define WIID_NPC_EVENTLIST_LIST_TITLE_SKIN					(WIID_NPC_EVENTLIST + 10)	//이벤트 아이템 목록 BG
#define WIID_NPC_EVENTLIST_LIST_TITLE						(WIID_NPC_EVENTLIST + 11)

#define WIID_NPC_EVENTLIST_PAGE_SKIN						(WIID_NPC_EVENTLIST + 20)
#define WIID_NPC_EVENTLIST_PAGE								(WIID_NPC_EVENTLIST + 21)
#define WIID_NPC_EVENTLIST_PAGE_PREV						(WIID_NPC_EVENTLIST + 30)
#define WIID_NPC_EVENTLIST_PAGE_NEXT						(WIID_NPC_EVENTLIST + 40)

#define WIID_NPC_EVENTLIST_LIST_SKIN						(WIID_NPC_EVENTLIST + 300)		//11개씩
#define WIID_NPC_EVENTLIST_LIST_BUTTON						(WIID_NPC_EVENTLIST + 320)
#define WIID_NPC_EVENTLIST_LIST_ICON						(WIID_NPC_EVENTLIST + 340)
#define WIID_NPC_EVENTLIST_LIST_NAME						(WIID_NPC_EVENTLIST + 360)
#define WIID_NPC_EVENTLIST_LIST_SELECT						(WIID_NPC_EVENTLIST + 380)


const int EVENTLIST_PAGE			= 10;


class SPWindowNpcEventList : public SPWindow {
public:
	SPWindowNpcEventList( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowNpcEventList();

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

	void UpdateEventList();

	void UpdateControl();

	SPIMESSAGE_OBJ_MAP_DECLARE()
	//int OnExit						(WPARAM wParam, LPARAM lParam);						/**< 종료 버튼 <br> */
	int OnPrev						(WPARAM wParam, LPARAM lParam);						/**< 이전 버튼 <br> */
	int OnNext						(WPARAM wParam, LPARAM lParam);						/**< 다음 버튼 <br> */	
	int OnItemClick					(unsigned int uiID, WPARAM wParam, LPARAM lParam);
	

private:
	SPTexture*		m_pTextureBase;							/**< 윈도우 바탕용 리소스 <br> */
	RECT			m_rtSrcBase[WINDOW_BASE_MAX];			/**< 윈도우 바탕 소스 영역 <br> */
	RECT			m_rtDestBase[WINDOW_BASE_MAX];			/**< 윈도우 바탕 대상 <br> */	
	
	SPWindow*		m_pIcon[EVENTLIST_PAGE];				/**< 리스트 아이콘 <br> */
	SPWindow*		m_pSelect[EVENTLIST_PAGE];				/**< 리스트 선택표현용 <br> */
	SPWindow*		m_pButton[EVENTLIST_PAGE];				/**< 리스트 이벤트 처리용 <br> */
	SPWindow*		m_pName[EVENTLIST_PAGE];				/**< 이름 <br> */	

	SPWindow*		m_pPage;								/**< 보상 아이템 리스트 페이지 <br> */
	SPWindowButton*	m_pPrev;								/**< 보상 아이템 리스트 이전 버튼 <br> */
	SPWindowButton*	m_pNext;								/**< 보상 아이템 리스트 다음 버튼 <br> */

	int				m_iMaxPage;								/**< 최대 페이지 <br> */
	int				m_iCurPage;								/**< 현재 페이지 <br> */
	int				m_iSelect;								/**< 아이템 목록인터페이스 상 선택한 인덱스 <br> 랜덤, 고정인 경우 보여줄수 없다 <br> */
	int				m_iSelectIndex;							/**< 아이템 데이터상 선택한 인덱스 <br> */

	char			m_szPage[12];							/**< 페이지 문자열 <br> */
};

