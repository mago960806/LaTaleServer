
#ifndef __SPWINDOW_ENCHANT_H__
#define __SPWINDOW_ENCHANT_H__

//------------------------------------------------------------------------------------
#define WIID_ENCHANT_SKIN_UPPER					(WIID_ENCHANT + 1)
#define WIID_ENCHANT_SKIN_MIDDLE				(WIID_ENCHANT + 2)
#define WIID_ENCHANT_SKIN_LOWER					(WIID_ENCHANT + 3)

#define WIID_ENCHANT_TITLE						(WIID_ENCHANT + 4)
#define WIID_ENCHANT_CLOSE						(WIID_ENCHANT + 5)
#define WIID_ENCHANT_OK							(WIID_ENCHANT + 6)
#define WIID_ENCHANT_CANCEL						(WIID_ENCHANT + 7)

#define WIID_ENCHANT_MIDDLE_BGK					(WIID_ENCHANT + 8)
#define WIID_ENCHANT_EQUIP						(WIID_ENCHANT + 9)
#define WIID_ENCHANT_MATERIAL					(WIID_ENCHANT + 10)
#define WIID_ENCHANT_RESULT						(WIID_ENCHANT + 11)
#define WIID_ENCHANT_EQUIP_SKIN					(WIID_ENCHANT + 12)
#define WIID_ENCHANT_MATERIAL_SKIN				(WIID_ENCHANT + 13)
#define WIID_ENCHANT_RESULT_SKIN				(WIID_ENCHANT + 14)
#define WIID_ENCHANT_RESULT_QUESTION			(WIID_ENCHANT + 15)
#define WIID_ENCHANT_PLUS						(WIID_ENCHANT + 16)
#define WIID_ENCHANT_EQUAL						(WIID_ENCHANT + 17)
#define WIID_ENCHANT_EQUIP_HELP					(WIID_ENCHANT + 18)
#define WIID_ENCHANT_MATERIAL_HELP				(WIID_ENCHANT + 19)
#define WIID_ENCHANT_PERCENTAGE_SKIN			(WIID_ENCHANT + 20)
#define WIID_ENCHANT_PERCENTAGE					(WIID_ENCHANT + 21)
#define WIID_ENCHANT_CURSOR						(WIID_ENCHANT + 22)
#define WIID_ENCHANT_MATERAIL_NUM1				(WIID_ENCHANT + 23)
#define WIID_ENCHANT_MATERAIL_NUM10				(WIID_ENCHANT + 24)
#define WIID_ENCHANT_RESULT_HELP				(WIID_ENCHANT + 25)

#define WIID_ENCHANT_PROGRASS					(WIID_ENCHANT + 30)
#define WIID_ENCHANT_PROGRASS_BG_LEFT			(WIID_ENCHANT + 31)
#define WIID_ENCHANT_PROGRASS_BG_MIDDLE			(WIID_ENCHANT + 32)
#define WIID_ENCHANT_PROGRASS_BG_RIGHT			(WIID_ENCHANT + 33)
#define WIID_ENCHANT_PROGRASS_GAGE				(WIID_ENCHANT + 34)
#define WIID_ENCHANT_PROGRASS_GAGE_RIGHT		(WIID_ENCHANT + 35)
#define WIID_ENCHANT_PROGRASS_GAGE_DECORATION	(WIID_ENCHANT + 36)
#define WIID_ENCHANT_PROGRASS_BRIGHT			(WIID_ENCHANT + 37)
#define WIID_ENCHANT_PROGRASS_NUMBER_BG			(WIID_ENCHANT + 38)
#define WIID_ENCHANT_PROGRASS_NUMBER_PER		(WIID_ENCHANT + 39)
#define WIID_ENCHANT_PROGRASS_NUMBER			(WIID_ENCHANT + 40)		// +3

//------------------------------------------------------------------------------------
enum ENCHANT_STEP
{
	ECS_NULL = 0,
	ECS_READY_EQUIP,
	ECS_READY_MATERIAL,
	ECS_READY_ENCHANT,
	ECS_ENCHANTTING,
	ECS_UNENCHANTTING,
	ECS_BRIGHTTING,
	ECS_ENCHANT_WAIT,
};

class SPItem;
struct SPItemEnchant;

class SPWindowEnchant : public SPWindow
{
public:
	SPWindowEnchant(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowEnchant(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	virtual ~SPWindowEnchant();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

protected:

	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnExit(WPARAM wParam, LPARAM lParam);
	virtual int OnOK(WPARAM wParam, LPARAM lParam);
	virtual int OnCancel(WPARAM wParam, LPARAM lParam);

	virtual int OnEnchantLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual int OnMaterailLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual int OnEnchantLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual int OnMaterailLButtonUp(WPARAM wParam, LPARAM lParam);

	virtual int OnEquipToolTipEnter(WPARAM wParam, LPARAM lParam);
	virtual int OnEquipToolTipOut(WPARAM wParam, LPARAM lParam);
	virtual int OnMaterialToolTipEnter(WPARAM wParam, LPARAM lParam);
	virtual int OnMaterialToolTipOut(WPARAM wParam, LPARAM lParam);
	virtual int OnResultToolTipEnter(WPARAM wParam, LPARAM lParam);
	virtual int OnResultToolTipOut(WPARAM wParam, LPARAM lParam);

	virtual int OnEnchantReceive(WPARAM wParam, LPARAM lParam);

	void InitMiddleWindow(SPWindow* pkParent);
	void InitPrograssBarWindow(SPWindow* pkParent);

	void InitEnchantValue();
	void SetEnchantStep(ENCHANT_STEP eStep);
	void ProcessCursor(float fTime);

	void SetEnchantItem(SPItem* pkItem);
	void SetMaterialItem(SPItem* pkItem);
	void SetResultItem(bool bFlag);
	void SetPercentage(int iPercentage);

	SPItem* GetInvenPickUpItem();
	SPItem* GetInvenItem(CONTAINER_TYPE iType, int iSlotIndex);
	bool EnableEnchantEquip(SPItem* pkItem);
	SPItemEnchant* EnableEnchantMaterial(SPItem* pkItem);

	void SetPrograss(int iType);
	void ProcessPrograss(float fTime);

	void SendParentAction(UINT64 uiAction,  unsigned int uiIndex = 0, bool bSendInit = false);
	void SendItemEnchant();
	void SetError(const char* pszErrorMsg);

	void SetInvenItemLock(SPItem* pkItem, int iIndex);
	void SetInvenItemUnlock(SPItem* pkItem, int iIndex);
	void SetInvenItemAllUnlock();

	void ToolTipEnter(SPItem* pkItem, SPItemEnchant* pResult = NULL);
	void ToolTipOut();

protected:
	ENCHANT_STEP m_eEnchantStep;

	int m_iCursorCurFrame;
	int m_iCursorDirection;
	float m_fCursorAccmulateTime;

	SPItem* m_pkEnchantEquip;
	SPItem* m_pkEnchantMaterial;
	SPItemEnchant* m_pItemEnchant;
	bool m_bResultItem;
	bool m_bEnchantBlock;
	int m_iSuccessedPercentage;
	CONTAINER_TYPE m_eEnchantEquipType;
	int m_iEnchantEquipSlotIndex;

	int m_iPrograssWidth;
	int m_iPrograssRightWidth;
	int m_iPrograssDecorationWidth;
	int m_iPrograssStartPosX;
	int m_iPrograssStartPosY;
	float m_fPrograssBrightAlpha;
	float m_fPrograssAccmulateTime;
	float m_fPrograssMaxTime;
	POINT m_ptPercentageNum[10];

	SPTexture* m_pkCursorTexture;

	bool	m_bUse;
};


#endif