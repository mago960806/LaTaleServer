#ifndef __SPWINDOW_LOOTING_H__
#define __SPWINDOW_LOOTING_H__

#define ITEM_MAX_NUM				10

#define WIID_LOOTING_SKIN_UPPER			(WIID_LOOTING + 1)
#define WIID_LOOTING_SKIN_MIDDLE		(WIID_LOOTING + 2)
#define WIID_LOOTING_SKIN_LOWER			(WIID_LOOTING + 3)
#define WIID_LOOTING_TITLE_TEXT			(WIID_LOOTING + 4)
#define WIID_LOOTING_EXIT				(WIID_LOOTING + 5)
#define WIID_LOOTING_SEPARATE_LINEUP	(WIID_LOOTING + 5)
#define WIID_LOOTING_SEPARATE_LINEDOWN	(WIID_LOOTING + 6)
#define WIID_LOOTING_SUB_BG				(WIID_LOOTING + 7)

#define WIID_LOOTING_ITEM_SLOT_IMAGE	(WIID_LOOTING + 8)		// +4
#define WIID_LOOTING_ITEM_SLOT			(WIID_LOOTING + 12)		// +4
#define WIID_LOOTING_ITEM_EMPTY			(WIID_LOOTING + 16)		// +4
#define WIID_LOOTING_ITEM_WHITE_BG		(WIID_LOOTING + 20)		// +4
#define WIID_LOOTING_ITEM_TEXT			(WIID_LOOTING + 24)		// +4
#define WIID_LOOTING_ITEM_SEL_BG		(WIID_LOOTING + 28)		// +4
#define WIID_LOOTING_ITEM_BTN			(WIID_LOOTING + 32)		// +4

#define WIID_LOOTING_DESCRIPTION		(WIID_LOOTING + 40)
#define WIID_LOOTING_PAGE_BG			(WIID_LOOTING + 41)
#define WIID_LOOTING_PAGE_TEXT			(WIID_LOOTING + 42)
#define WIID_LOOTING_PAGE_PREV			(WIID_LOOTING + 43)
#define WIID_LOOTING_PAGE_NEXT			(WIID_LOOTING + 44)


class SPWindow;
class SPWindowLooting : public SPWindow
{
public:
	SPWindowLooting(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowLooting(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowLooting();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);
	void ProcessHotKey();
	void ProcessJoyPad(float fTime);

protected:

	SPIMESSAGE_OBJ_MAP_DECLARE()

	int OnCursorEnter(WPARAM, LPARAM);
	int OnCursorOut(WPARAM, LPARAM);
	int OnItemSetting(WPARAM, LPARAM);
	int OnItemDelete(WPARAM, LPARAM);
	int OnItemReDraw(WPARAM, LPARAM);
	int OnInventoryCheck(WPARAM, LPARAM);

	int OnPagePrev(WPARAM, LPARAM);
	int OnPageNext(WPARAM, LPARAM);
	int OnItemClick(unsigned int, WPARAM, LPARAM);
	int OnItemLootingClick(unsigned int, WPARAM, LPARAM);
	int OnExit(WPARAM, LPARAM);

	int OnToolTipMove(unsigned int, WPARAM, LPARAM);
	int OnToolTipEnter(unsigned int, WPARAM, LPARAM);
	int OnToolTipOut(unsigned int, WPARAM, LPARAM);

	void Clear();
	void SetEmptySlot(int iIndex);
	void SetItemWindow();
	void SetPageText();

	void ItemGain();
	void ItemAllGain();

protected:
	int m_iCurPage;
	int m_iMaxPage;
	int m_iSelectItem;
	
	bool m_bJoyPadUpPush;
	bool m_bJoyPadDownPush;
	bool m_bJoyPadSpacePush;	
	bool m_bJoyPadShiftPush;

	int m_iValidItemNum;
	bool m_bInventoryFull[ITEM_MAX_NUM];
	CONTAINER_ITEM m_ContainerItem[ITEM_MAX_NUM];
};

#endif