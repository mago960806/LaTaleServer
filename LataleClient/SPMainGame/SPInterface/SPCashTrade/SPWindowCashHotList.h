#pragma once


#define WIID_CASH_HOTLIST_BASE					(WIID_CASH_HOTLIST	+	1 )
#define WIID_CASH_HOTLIST_ITEM_NAME				(WIID_CASH_HOTLIST	+	10 )

#define WIID_CASH_HOTLIST_CLOSE					(WIID_CASH_HOTLIST	+	20 )

#define WIID_CASH_HOTLIST_SKIN					(WIID_CASH_HOTLIST	+	30 )
#define WIID_CASH_HOTLIST_LINE					(WIID_CASH_HOTLIST	+	40 )
#define WIID_CASH_HOTLIST_TEXTIMG				(WIID_CASH_HOTLIST	+	50 )

#define WIID_CASH_HOTLIST_ITEMBUTTON			(WIID_CASH_HOTLIST	+	70 )
#define WIID_CASH_HOTLIST_ITEMSKIN				(WIID_CASH_HOTLIST	+	80 )
#define WIID_CASH_HOTLIST_ITEMICON				(WIID_CASH_HOTLIST	+	90 )

const int MAX_HOTITEM_VIEW		= 4;

enum ICON_MARK;

class SPWindowCashHotList : public SPWindow {

public:	
	SPWindowCashHotList(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowCashHotList();

	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);


	virtual void Process(float fTime);
	virtual void Render(float fTime);

protected:
	void SetWindowBase();
	void InitSubControl();

	void UpdateControl();

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnClose					(WPARAM wParam, LPARAM lParam);	
	int OnToolTipMove			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipEnter			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipOut			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnItemSelect			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	// }}

private:
	SPTexture* m_pBaseSkin;
	RECT m_rtBaseSrc[WINDOW_BASE_MAX];
	RECT m_rtBaseDest[WINDOW_BASE_MAX];

	SPTexture* m_pDisable;							/**< 비활성 표현용 <br> */
	RECT m_rtSrcDisable;							/**< 비활성 표현용 소스 <br> */
	RECT m_rtMarkSrc[4];							/**< ICON MARK Src <br> */

	SPWindow*	m_pItemIcon[MAX_HOTITEM_VIEW];	
	
	bool		m_bItemAble[MAX_HOTITEM_VIEW];		/**< 내가 올려논 아이템이 내가 착용가능한 아이템인지 <br> */	
	ICON_MARK	m_iItemMark[MAX_HOTITEM_VIEW];		/**< ICON MARK <br> */
	int			m_iStack[MAX_HOTITEM_VIEW];			/**< 스택 <br> */
	RECT		m_rtStack[MAX_HOTITEM_VIEW];		/**< 스택 영역 <br> */
	
};


