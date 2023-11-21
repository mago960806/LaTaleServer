#pragma once

class SPWindow;
class SPWindowStatic;
class SPWindowEdit;
class SPWindowEditMultiLine;
class SPWindowButton;

#define WIID_MAILLIST_SEND			(WIID_MAILLIST	+	1)		//우편 전송 버튼
#define WIID_MAILLIST_VIEW			(WIID_MAILLIST	+	2)		//우편 내용 확인
#define WIID_MAILLIST_CLOSE			(WIID_MAILLIST	+	3)		//닫기

#define WIID_MAILLIST_PAGEBASE		(WIID_MAILLIST	+	11)		//페이지 베이스
#define WIID_MAILLIST_TOP			(WIID_MAILLIST	+	15)
#define WIID_MAILLIST_CENTER		(WIID_MAILLIST	+	20)
//#define WIID_MAILLIST_BOTTOM		(WIID_MAILLIST	+	25)

#define WIID_MAILLIST_TITLE			(WIID_MAILLIST	+	21)		//Title
#define WIID_MAILLIST_MENU_BASE 	(WIID_MAILLIST	+	25)		//메뉴바탕

#define WIID_MAILLIST_UPLINE		(WIID_MAILLIST	+	26)		//상단 경계선
 

#define WIID_MAILLIST_PREV			(WIID_MAILLIST	+	30)		//페이지 증가
#define WIID_MAILLIST_PAGE			(WIID_MAILLIST	+	35)		//페이지
#define WIID_MAILLIST_NEXT			(WIID_MAILLIST	+	40)		//페이지 감소

#define WIID_MAILLIST_LISTVIEW		(WIID_MAILLIST	+	45)		//List
#define WIID_MAILLIST_LIST_SKIN		(WIID_MAILLIST	+	46)		//List Skin
#define WIID_MAILLIST_ITEM_SKIN		(WIID_MAILLIST	+	50)		//4개씩 메일 리스트 에 사용될 각 개별 아이템 바닥
#define WIID_MAILLIST_ITEM_SEL		(WIID_MAILLIST	+	60)		//4개씩 아이템 아이콘 버튼
#define WIID_MAILLIST_ITEM_ICON		(WIID_MAILLIST	+	70)		//4개씩 아이템 아이콘 버튼
#define WIID_MAILLIST_ITEM_SENDER	(WIID_MAILLIST	+	80)		//4개씩 아이템 보낸이
#define WIID_MAILLIST_ITEM_NAME		(WIID_MAILLIST	+	90)		//4개씩 아이템 제목
#define WIID_MAILLIST_ITEM_DATE		(WIID_MAILLIST	+	100)		//4개씩 아이템 보낸 날자
#define WIID_MAILLIST_ITEM_BUTTON	(WIID_MAILLIST	+	110)	//4개씩 아이템 내부 버튼
#define WIID_MAILLIST_ITEM_READ		(WIID_MAILLIST	+	120)	//4개씩 아이템 읽은 메일인가?
#define WIID_MAILLIST_ITEM_ICON_LINE (WIID_MAILLIST	+	130)	//아이템 아이콘 외곽선
#define WIID_MAILLIST_ITEM_BOTTOM	(WIID_MAILLIST	+	150)	//List Bottom

#define WIID_MAILLIST_SENDVIEW		(WIID_MAILLIST	+	160)	//Send
#define WIID_MAILLIST_SEND_SKIN      (WIID_MAILLIST	+	161)	//Send Skin
#define WIID_MAILLIST_SEND_TITLEBASE (WIID_MAILLIST	+	165)	//제목 Base
#define WIID_MAILLIST_SEND_TITLE	(WIID_MAILLIST	+	170)	//제목
//#define WIID_MAILLIST_SEND_RECEIVERPLATE (WIID_MAILLIST	+	124)//보낸사람 -> 받을사람
#define WIID_MAILLIST_SEND_RECEIVERBASE	(WIID_MAILLIST	+	185)//받는사람 Base
#define WIID_MAILLIST_SEND_RECEIVER	(WIID_MAILLIST	+	190)	//받는사람
#define WIID_MAILLIST_SEND_MSG		(WIID_MAILLIST	+	200)	//내용
#define WIID_MAILLIST_SEND_MSG_BASE	(WIID_MAILLIST	+	201)	//내용

#define WIID_MAILLIST_SEND_APPEND_1 (WIID_MAILLIST	+	205)	//첨부 이미지 1
#define WIID_MAILLIST_SEND_APPEND_2 (WIID_MAILLIST	+	206)	//점부 이미지 2

#define WIID_MAILLIST_SEND_ITEM		(WIID_MAILLIST	+	210)	//Item Button
#define WIID_MAILLIST_SEND_ITEMICON (WIID_MAILLIST	+	220)	//Item Icon
#define WIID_MAILLIST_SEND_ITEMICON_LINE  (WIID_MAILLIST	+	221)	//Item Icon Line
#define WIID_MAILLIST_SEND_ITEMNAME (WIID_MAILLIST	+	230)	//Item Name

#define WIID_MAILLIST_SEND_ELYBASE	(WIID_MAILLIST	+	240)	//금액 이미지 
#define WIID_MAILLIST_SEND_ELY		(WIID_MAILLIST	+	250)	//Ely
#define WIID_MAILLIST_SEND_ELY_IMG	(WIID_MAILLIST	+	251)	//Ely Image

#define WIID_MAILLIST_SEND_MAIL		(WIID_MAILLIST	+	290)	//Send Button
#define WIID_MAILLIST_SEND_CANCEL	(WIID_MAILLIST	+	291)	//Send Button

#define WIID_MAILLIST_SEND_BOTTOM	(WIID_MAILLIST	+	295)	//Send Bottom


const int MAX_MAIL_LIST				= 4;			/**< 한 리스트에 나오는 최대 메일 갯수 <br> */
//const int MAX_MAIL_PAGE				= 3;			/**< 3 페이지 제한 <br> */
const int MAX_MAIL_PAGE				= 10;			/**< 10 페이지 제한 <br> */
//const int MAIL_FEE					= 50;			/**< 우편물 수수료 <br> */
const int TITLE_LENGTH				= 24;			/**< 리스트 우편물 이름이 너무 긴경우 자르는 기점 <br> */

enum MAILLIST_MODE {	
	MAILLIST_MODE_VIEW				= 0,
	MAILLIST_MODE_SEND				= 1,
};

enum ICON_MARK;

class SPWindowMailList : public SPWindow 
{
public:
	SPWindowMailList( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	virtual ~SPWindowMailList();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

protected:
	void InitSubWindow();
	void SetModeButton();
	
	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

	/**
	 * Send 종료시 인테페이스 초기화 <br> 
	 @param 
	 @return
	*/
	void ClearSend();

	/**
	 * MailList 인터페이스 업데이트 <br> 
	 @param 
	 @return
	*/
	void UpdateList();

	void ResetItemControl(bool bRestore = true);
	void RestoreInven();
	

	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnUpdateMailList	(WPARAM wParam, LPARAM lParam);			//외부에서 리스트 갱신을 요구
	int OnClearSend			(WPARAM wParam, LPARAM lParam);			//Send후 Send 인터페이스 클리어
	int	OnResetSendItem		(WPARAM wParam, LPARAM lParam);			//인벤토리에서 메일로 보내는 아이템을 취소 하는 경우
	int OnReadCheck			(WPARAM wParam, LPARAM lParam);			//새메일을 읽은 경우 메일 체크 갱신
	int OnTab				(WPARAM wParam, LPARAM lParam);
	int OnMailView			(WPARAM wParam, LPARAM lParam);
	int OnMailSend			(WPARAM wParam, LPARAM lParam);
	int OnPrev				(WPARAM wParam, LPARAM lParam);
	int OnNext				(WPARAM wParam, LPARAM lParam);
	int OnSend				(WPARAM wParam, LPARAM lParam);
	int OnItemClick			(unsigned int uiID, WPARAM wParam, LPARAM lParam );
	int OnToolTipMove		(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipEnter		(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipOut		(unsigned int iUiID, WPARAM wParam, LPARAM lParam);	
	
	int OnUpdateEly			(WPARAM wParam, LPARAM lParam);
	int	OnButtonUpItem		(WPARAM wParam, LPARAM lParam);
	int OnSendItemToolTipMove	(WPARAM wParam, LPARAM lParam);
	int OnSendItemToolTipEnter	(WPARAM wParam, LPARAM lParam);
	int OnSendItemToolTipOut	(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp			(WPARAM wParam, LPARAM lParam);
	int OnClose				(WPARAM wParam, LPARAM lParam);

private:
	MAILLIST_MODE	m_iMailListMode;
	int		m_iMaxPage;					/**< Mail List를 페이지화 해 나온 총 페이지수 <br> */
	int		m_iCurPage;					/**< 현재 선택한 페이지 수 <br> */
	int		m_iStartIndex;				/**< 리스트 표현을 위해 ? <br> */
	int		m_iSelectItem;				/**< 4개의 리스트중 선택된 아이템 <br> */
	
	SPTexture* m_pDisable;
	SPTexture* m_pCheck;

	RECT			m_rtSrcCenter;
	RECT			m_rtDestCenter;

	SPWindowButton* m_pSend;
	SPWindowButton* m_pView;
	
	SPWindow*		m_pPageBase;
	SPWindowButton* m_pPrev;
	SPWindowButton* m_pNext;
	SPWindow*		m_pPage;	

	SPWindow*			m_pListView;
	SPWindow*			m_pListSelect[MAX_MAIL_LIST];			//선택 스킨
	SPWindow*			m_pListIcon[MAX_MAIL_LIST];				//Icon
	SPWindow*			m_pListSender[MAX_MAIL_LIST];			//발송인
	SPWindow*			m_pListName[MAX_MAIL_LIST];				//편지제목
	SPWindowButton*		m_pListButton[MAX_MAIL_LIST];			//버튼
	SPWindow*			m_pListDate[MAX_MAIL_LIST];				//날자
	SPWindow*			m_pListCheck[MAX_MAIL_LIST];			//Read Check
	//SPWindow*			m_pListNCheck[MAX_MAIL_LIST];			//Not Read Check

	SPWindow*			m_pSendView;
	SPWindowEdit*		m_pTitle;								//제목
	SPWindowEdit*		m_pReceiver;							//받는사람
	SPWindowEditMultiLine*	m_pMsg;								//내용
	
	SPWindowButton*		m_pItemButton;							//Item
	SPWindow*			m_pItemIcon;
	SPWindow*			m_pItemName;	
	
	SPWindowEdit*		m_pEly;									//Ely

	SPWindowButton*		m_pMailSend;							//Send Button
	
	RECT m_rtMarkSrc[4];										/**< ICON MARK Src <br> */

	RECT m_rtStack[MAX_MAIL_LIST];								//List Item Stack Count
	int	m_iStackCount[MAX_MAIL_LIST];							/**< 받은 Item의 갯수 <br> */
	
	RECT m_rtSrcCheck;											/**< Check 표시 소스 <br> */
	RECT m_rtSrcNCheck;
	ICON_MARK	m_iStackMark[MAX_MAIL_LIST];					/**< 받은 Item의 마크 <br> */
	bool m_bRead[MAX_MAIL_LIST];								/**< 받은 메일이 읽은것인지 표시 <br> */
	
	RECT m_rtItemStack;											//Send Item Stack Count
	int m_iItemCount;											//Send Item Stack Count
	ICON_MARK m_iItemMark;										//Send Item Mark

	SPSelectItem m_PickUpItem;
	SPItemStatus* m_pItemStatus;				/**< Sell ItemStatus <br> */
	CONTAINER_TYPE m_iStorage;					/**< Current STORAGE_PLAYER <br> */
};