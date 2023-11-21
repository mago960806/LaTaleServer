#pragma once


#define WIID_ITEM_TIMEADD_TOP				(WIID_ITEM_TIMEADD	+	1)
#define WIID_ITEM_TIMEADD_CENTER			(WIID_ITEM_TIMEADD	+	2)
#define WIID_ITEM_TIMEADD_BOTTOM			(WIID_ITEM_TIMEADD	+	3)
#define WIID_ITEM_TIMEADD_TITLE				(WIID_ITEM_TIMEADD	+	5)
#define WIID_ITEM_TIMEADD_CLOSE				(WIID_ITEM_TIMEADD	+	6)

#define WIID_ITEM_TIMEADD_LINE1				(WIID_ITEM_TIMEADD	+	8)
#define WIID_ITEM_TIMEADD_LINE2				(WIID_ITEM_TIMEADD	+	9)

#define WIID_ITEM_TIMEADD_SKIN				(WIID_ITEM_TIMEADD	+	10)			//흰색 바탕

#define WIID_ITEM_TIMEADD_SLOTLINE			(WIID_ITEM_TIMEADD	+	20)			//3개의 슬롯 외곽선
#define WIID_ITEM_TIMEADD_SLOTICON			(WIID_ITEM_TIMEADD	+	30)			//3개의 슬롯 아이콘
#define WIID_ITEM_TIMEADD_SLOTBUTTON		(WIID_ITEM_TIMEADD	+	40)			//3개의 슬롯 버튼

#define WIID_ITEM_TIMEADD_CURSOR1			(WIID_ITEM_TIMEADD	+	100)
#define WIID_ITEM_TIMEADD_CURSOR2			(WIID_ITEM_TIMEADD	+	110)
#define WIID_ITEM_TIMEADD_TOOLTIP			(WIID_ITEM_TIMEADD	+	120)

#define WIID_ITEM_TIMEADD_PLUS				(WIID_ITEM_TIMEADD	+	150)
#define WIID_ITEM_TIMEADD_EQUAL				(WIID_ITEM_TIMEADD	+	160)
#define WIID_ITEM_TIMEADD_QUESTION			(WIID_ITEM_TIMEADD	+	165)

#define WIID_ITEM_TIMEADD_OK				(WIID_ITEM_TIMEADD	+	170)
#define WIID_ITEM_TIMEADD_CANCEL			(WIID_ITEM_TIMEADD	+	180)



const int MAX_TIMEADD_SLOT					= 3;								//슬롯갯수
class SPWindow;
class SPWindowButton;

enum TIMEADD_STATUS {
	TIMEADD_STATUS_FIRST			= 0,
	TIMEADD_STATUS_SECOND			= 1,
	TIMEADD_STATUS_READY			= 2,
	TIMEADD_STATUS_RESULT			= 3,
	TIMEADD_STATUS_END				= 4,
};

#include "SPPlayerInvenArchive.h"


class SPWindowItemTimeAdd : public SPWindow
{

public:
	SPWindowItemTimeAdd(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowItemTimeAdd(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowItemTimeAdd();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);


protected:
	void ProcessCursor(float fTime);
	void CreateSubWindow();	
	void SetStatus(TIMEADD_STATUS iStatus);
	void ClearAllSlotInterface();
	void ClearSlotInterface(int iIndex);
	void SetSlotInterface(int iIndex, SPItem* pItem);

	bool SetFirstItem();
	bool SetSecondItem();
	bool CheckTimeAddItem(SPItem* pItem);

	bool SendAddTime();

	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnResult(WPARAM wParam, LPARAM lParam);
	int OnClose(WPARAM wParam, LPARAM lParam);
	int OnOK(WPARAM wParam, LPARAM lParam);
	int OnCancel(WPARAM wParam, LPARAM lParam);	
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	int OnItemClick(unsigned int iUiID, WPARAM wParam, LPARAM lParam);	
	int OnToolTipMove			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipEnter			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	int OnToolTipOut			(unsigned int iUiID, WPARAM wParam, LPARAM lParam);
	

private:	
	SPTexture* m_pQuestion;
	SPTexture* m_pDisable;		
	RECT	m_rtSrcDisable;												/**< 비활성 표현용 소스 <br> */
	
	int		m_iCursorDirection;
	float	m_fCursorAccmulateTime;
	int		m_iFrame;

	SPWindow* m_pIcon[MAX_TIMEADD_SLOT];
	SPWindow* m_pCursorFirst;
	SPWindow* m_pCursorSecond;
	SPWindow* m_pToolTip;
	SPWindowButton* m_pOK;
	SPWindowButton* m_pCancel;
	
	SPItem*	m_pItem[MAX_TIMEADD_SLOT];									/**< 마지막 슬롯은 사용하지 않는다 !!! <br> */
	SPItem* m_pItemResult;												/**< 성공시 가상의 아이템 표현용 <br> */
	SPSelectItem m_SelectItem[MAX_TIMEADD_SLOT];
	
	RECT	m_rtMarkSrc[4];												/**< ICON MARK Src <br> */	
	
	RECT	m_rtStack[MAX_TIMEADD_SLOT];								//List Item Stack Count

	bool	m_bItemAble[MAX_TIMEADD_SLOT];								/**< 내가 올려논 아이템이 내가 착용가능한 아이템인지 <br> */
	int		m_iStack[MAX_TIMEADD_SLOT];									/**< 받은 Item의 갯수 <br> */
	ICON_MARK	m_iItemMark[MAX_TIMEADD_SLOT];							/**< ICON MARK <br> */
	
	TIMEADD_STATUS	m_iTimeAddStatus;

	bool	m_bUse;
};



