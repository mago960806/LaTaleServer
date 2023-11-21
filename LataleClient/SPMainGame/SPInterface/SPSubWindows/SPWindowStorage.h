#pragma once


#define WIID_STORAGE_TOP			(WIID_STORAGE	+	1)		//스킨 TOP
#define WIID_STORAGE_CENTER			(WIID_STORAGE	+	2)		//스킨 CENTER
#define WIID_STORAGE_BOTTOM			(WIID_STORAGE	+	3)		//스킨 BOTTOM

#define WIID_STORAGE_CLOSE			(WIID_STORAGE	+	5)		//Close

#define WIID_STORAGE_MIDDLE			(WIID_STORAGE	+	10)		//스킨 밝은 색

#define WIID_STORAGE_TITLE			(WIID_STORAGE	+	15)		//타이틀

#define WIID_STORAGE_SKIN		 	(WIID_STORAGE	+	30)		//흰바닥 스킨
#define WIID_STORAGE_SKIN_UP_LINE	(WIID_STORAGE	+	31)		//흰바닥 스킨 상단 경계
#define WIID_STORAGE_SKIN_DOWN_LINE	(WIID_STORAGE	+	32)		//흰바닥 스킨 하단 경계

#define WIID_STORAGE_ELY_SAVE		(WIID_STORAGE	+	40)		//Ely 저금
#define WIID_STORAGE_ELY_LOAD		(WIID_STORAGE	+	50)		//Ely 출금

#define WIID_STORAGE_PAGE_UP		(WIID_STORAGE	+	60)		//스크롤바
#define WIID_STORAGE_PAGE_DOWN		(WIID_STORAGE	+	70)		//스크롤바
#define WIID_STORAGE_SLIDER			(WIID_STORAGE	+	80)		//스크롤바

#define WIID_STORAGE_GRID			(WIID_STORAGE	+	90)		//실 Grid 

#define WIID_STORAGE_INPUT_ELY		(WIID_STORAGE	+	100)	//Ely 입력

#define WIID_STORAGE_ELY_TEXT		(WIID_STORAGE	+	110)	//보관금액 글자
#define WIID_STORAGE_ELY			(WIID_STORAGE	+	115)	//저장되어 있는 Ely
#define WIID_STORAGE_ELY_IMG		(WIID_STORAGE	+	120)	//Ely Image

#define WIID_STORAGE_GRID_LINE		(WIID_STORAGE	+	130)	//Grid Line


const int DEFAULT_STORAGE_SLOT	= 4;			/**< 기본 슬롯 수 <br> */
const int LINE_SLOT				= 4;			/**< 한 라인에 들어가는 슬롯 수 <br> */
const int MAX_STORAGE_SLOT		= 16;			/**< 최대 슬롯 수 <br> 저장공간이 아닌 인테페이스임 <br> */

class SPWindow;
class SPWindowStatic;
class SPWindowEdit;
class SPWindowButton;
class SPWindowGrid;
class SPWindowSlider;


/**
* @class SPWindowStorage
* @brief 창고 윈도우
* @author Jinhee
* @version 00.00.01
* @date 2005.7.14
* @bug 없음
* @warning 
*/
class SPWindowStorage : public SPWindow {

public:
	SPWindowStorage( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowStorage();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);

	void Refresh();

protected:
	/**
	 * Sub Control을 만든다 <br> 
	 @param NONE
	 @return NONE
	*/
	void CreateSubWindow();
	
	/**
	* Slot 을 렌더할 Dest영역을 설정한다 <br> 
	* //[2006/6/22] 기능 대체
	@param NONE
	@return NONE
	*/
	//void SetSlotPos();	
	
	/**
	 * 슬롯을 렌더한다 <br> 
	 * //[2006/6/22] 기능 대체
	 @param NONE
	 @return NONE
	*/
	//void SlotRender();

	/**
	 * 서버로 저장하는 금액을 보내준다 <br>
	 @param  UINT64	iSaveEly
	 @return bool
	*/
	bool SendSaveEly(UINT64	iSaveEly);
	
	/**
	 * 서버로 찾는 금액을 보내준다 <br>
	 @param UINT64 iLoadEly
	 @return bool
	*/
	bool SendLoadEly(UINT64 iLoadEly);

public:		

protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()	

	virtual int OnCursorEnter	( WPARAM, LPARAM );
	virtual int OnCursorOut		( WPARAM, LPARAM );
	virtual int OnCursorLDown	( WPARAM, LPARAM );
	virtual int OnCursorLUp		( WPARAM, LPARAM );	
	virtual int OnRefresh		( WPARAM, LPARAM );	

	int OnCursorMove	(WPARAM wParam, LPARAM lParam);
	int OnGridDown		(WPARAM wParam, LPARAM lParam);
	int OnGridUp		(WPARAM wParam, LPARAM lParam);

	virtual int OnToolTipEnter	( WPARAM, LPARAM );
	virtual int OnToolTipOut	( WPARAM, LPARAM );
	virtual int OnToolTipMove	( WPARAM, LPARAM );
	
	int InitSlot		(WPARAM wParam, LPARAM lParam);		//슬롯을 초기화 한다
	int SetExtendSlot	(WPARAM wParam, LPARAM lParam);		//슬롯을 확장한다

	int OnUpdateEly		(WPARAM wParam, LPARAM lParam);
	int OnSaveEly		(WPARAM wParam, LPARAM lParam);
	int OnLoadEly		(WPARAM wParam, LPARAM lParam);
	int	OnInputEly		(WPARAM wParam, LPARAM lParam);

	int OnItemSetting	(WPARAM wParam, LPARAM lParam);
	int OnItemClear		(WPARAM wParam, LPARAM lParam);
	int OnItemAllClear	(WPARAM wParam, LPARAM lParam);
	int OnItemSetStack	(WPARAM wParam, LPARAM lParam);
	int OnItemSetIconSelect(WPARAM wParam, LPARAM lParam);
	int OnItemDisable	(WPARAM wParam, LPARAM lParam);

	int	OnButtonPageUp		( WPARAM wParam, LPARAM lParam );
	int	OnButtonPageDown	( WPARAM wParam, LPARAM lParam );
	int	OnSliderPageUp		( WPARAM wParam, LPARAM lParam );
	int	OnSliderPageDown	( WPARAM wParam, LPARAM lParam );
	int	OnSliderPageNum		( WPARAM wParam, LPARAM lParam );
	int OnSliderButtonUp	( WPARAM wParam, LPARAM lParam );

	int OnClose				( WPARAM wParam, LPARAM lParam );

	// }}

protected:	
	SPWindowSlider* m_pSlider;
	SPWindowGrid*	m_pGrid;

	SPWindowEdit*	m_pElyEdit;
	SPWindowButton*	m_pElySave;	
	SPWindowButton* m_pElyLoad;
	SPWindowStatic*	m_pStorageEly;

	SPWindow*		m_pGridLine[MAX_STORAGE_SLOT]; /**< 그리드별 외각선 <br> */

	int		m_iSlotCount;						/**< Real Slot <br> */
	int		m_iViewSlotCount;					/**< Render Slot Count <br> */
	
	int		m_iMaxPage;							/**< 최대 페이지(라인)수 <br> */
	int		m_iCurPage;							/**< 현재 페이지(라인)수 <br> */

	char	m_szStorageEly[64];					/**< 창고에 저장되어 있는 금액 문자열 <br> */
};
