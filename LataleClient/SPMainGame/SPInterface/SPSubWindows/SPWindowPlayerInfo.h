#ifndef __SPWINDOW_PLAYERINFO_H__
#define __SPWINDOW_PLAYERINFO_H__


#define WIID_PC_INFO_SKIN_UPPER				(WIID_PC_INFO + 1)
#define WIID_PC_INFO_SKIN_MIDDLE			(WIID_PC_INFO + 2)
#define WIID_PC_INFO_SKIN_LOWER				(WIID_PC_INFO + 3)
#define WIID_PC_INFO_TITLE_TEXT				(WIID_PC_INFO + 4)
#define WIID_PC_INFO_EXIT					(WIID_PC_INFO + 5)
#define WIID_PC_INFO_SUB_BG					(WIID_PC_INFO + 6)
#define WIID_PC_INFO_TAB_STATUS				(WIID_PC_INFO + 7)
#define WIID_PC_INFO_TAB_EQUIP				(WIID_PC_INFO + 8)
#define WIID_PC_INFO_TAB_LOVE				(WIID_PC_INFO + 9)
#define WIID_PC_INFO_SEPARATE_LINEUP		(WIID_PC_INFO + 10)
#define WIID_PC_INFO_SEPARATE_LINEDOWN		(WIID_PC_INFO + 11)
#define WIID_PC_INFO_SUB_WHITE_BG			(WIID_PC_INFO + 12)

#define WIID_PC_INFO_STATUS					(WIID_PC_INFO + 20)
#define WIID_PC_INFO_STATUS_CHAR_BG			(WIID_PC_INFO + 21)
#define WIID_PC_INFO_STATUS_CHAR_CLASS_BG	(WIID_PC_INFO + 22)
#define WIID_PC_INFO_STATUS_CHAR_CLASS_IMG	(WIID_PC_INFO + 23)
#define WIID_PC_INFO_STATUS_NAME			(WIID_PC_INFO + 24)
#define WIID_PC_INFO_STATUS_CLASS			(WIID_PC_INFO + 25)
#define WIID_PC_INFO_STATUS_LEVEL			(WIID_PC_INFO + 26)
#define WIID_PC_INFO_STATUS_TITLE_BG		(WIID_PC_INFO + 27)
#define WIID_PC_INFO_STATUS_TITLE			(WIID_PC_INFO + 28)
#define WIID_PC_INFO_STATUS_FAME			(WIID_PC_INFO + 29)
#define WIID_PC_INFO_STATUS_PVP				(WIID_PC_INFO + 30)
#define WIID_PC_INFO_STATUS_TRADE			(WIID_PC_INFO + 31)
#define WIID_PC_INFO_STATUS_INVITE			(WIID_PC_INFO + 32)
#define WIID_PC_INFO_STATUS_WHISPER			(WIID_PC_INFO + 33)
#define WIID_PC_INFO_STATUS_CLOSED			(WIID_PC_INFO + 34)
#define WIID_PC_INFO_STATUS_FRIEND			(WIID_PC_INFO + 35)
#define WIID_PC_INFO_STATUS_GUILD			(WIID_PC_INFO + 36)


#define WIID_PC_INFO_EQUIP					(WIID_PC_INFO + 40)
#define WIID_PC_INFO_EQUIP_BATTLE			(WIID_PC_INFO + 41)
#define WIID_PC_INFO_EQUIP_FASHION			(WIID_PC_INFO + 42)
#define WIID_PC_INFO_EQUIP_CHAR_BG			(WIID_PC_INFO + 43)
#define WIID_PC_INFO_EQUIP_SLOT_IMAGE		(WIID_PC_INFO + 44)		// +12
#define WIID_PC_INFO_EQUIP_SLOT_ICON		(WIID_PC_INFO + 56)		// +12
#define WIID_PC_INFO_EQUIP_SLOT				(WIID_PC_INFO + 68)		// +12
#define WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE	(WIID_PC_INFO + 80)		// +2
#define WIID_PC_INFO_EQUIP_WP1_SLOT_ICON	(WIID_PC_INFO + 82)		// +2
#define WIID_PC_INFO_EQUIP_WP1_SLOT			(WIID_PC_INFO + 84)		// +2
#define WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE	(WIID_PC_INFO + 86)		// +2
#define WIID_PC_INFO_EQUIP_WP2_SLOT_ICON	(WIID_PC_INFO + 88)		// +2
#define WIID_PC_INFO_EQUIP_WP2_SLOT			(WIID_PC_INFO + 90)		// +2

//LOVE
#define WIID_OPPSTATUS_LOVE								(WIID_PC_INFO + 200)
#define WIID_OPPSTATUS_LOVE_BG_T						(WIID_PC_INFO + 201)
#define WIID_OPPSTATUS_LOVE_BG_ML						(WIID_PC_INFO + 202)
#define WIID_OPPSTATUS_LOVE_BG_MM						(WIID_PC_INFO + 203)
#define WIID_OPPSTATUS_LOVE_BG_MR						(WIID_PC_INFO + 204)
#define WIID_OPPSTATUS_LOVE_BG_B						(WIID_PC_INFO + 205)
#define WIID_OPPSTATUS_LOVE_ICON						(WIID_PC_INFO + 206)
#define WIID_OPPSTATUS_LOVE_TITLE						(WIID_PC_INFO + 207)
#define WIID_OPPSTATUS_LOVE_LV_BG						(WIID_PC_INFO + 208)
#define WIID_OPPSTATUS_LOVE_LV_TITLE					(WIID_PC_INFO + 209)
#define WIID_OPPSTATUS_LOVE_LVINFO_BG					(WIID_PC_INFO + 210)
#define WIID_OPPSTATUS_LOVE_LV_TEXT						(WIID_PC_INFO + 211)
#define WIID_OPPSTATUS_LOVE_PARTNER_BG					(WIID_PC_INFO + 212)
#define WIID_OPPSTATUS_LOVE_PARTNER_TITLE				(WIID_PC_INFO + 213)
#define WIID_OPPSTATUS_LOVE_PARTNERINFO_BG				(WIID_PC_INFO + 214)
#define WIID_OPPSTATUS_LOVE_PARTNER_TEXT				(WIID_PC_INFO + 215)
#define WIID_OPPSTATUS_LOVE_DAY_BG						(WIID_PC_INFO + 216)
#define WIID_OPPSTATUS_LOVE_DAY_TITLE					(WIID_PC_INFO + 217)
#define WIID_OPPSTATUS_LOVE_DAYINFO_BG					(WIID_PC_INFO + 218)
#define WIID_OPPSTATUS_LOVE_DAY_TEXT					(WIID_PC_INFO + 219)



const int MAX_PLAYER_EQUIP_CONTAINER = 3;

class SPWindow;
class SPPlayer;
class SPTitleRenderBase;
class SPItem;

class SPWindowPlayerInfo : public SPWindow
{
public:
	SPWindowPlayerInfo(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowPlayerInfo(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowPlayerInfo();

	virtual void Init();
	virtual void Clean();
	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

	virtual void Process(float fTime);
	virtual void Render(float fTime);	

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnExit(WPARAM wParam, LPARAM lParam);
	virtual int OnTabStatus(WPARAM wParam, LPARAM lParam);
	virtual int OnTabEquip(WPARAM wParam, LPARAM lParam);
	virtual int OnTabLove(WPARAM wParam, LPARAM lParam);
	virtual int OnEquipBattle(WPARAM wParam, LPARAM lParam);
	virtual int OnEquipFashion(WPARAM wParam, LPARAM lParam);
	virtual int OnTrade(WPARAM wParam, LPARAM lParam);
	virtual int OnWhisper(WPARAM wParam, LPARAM lParam);
	virtual int OnInvite(WPARAM wParam, LPARAM lParam);
	virtual int OnFriend(WPARAM wParam, LPARAM lParam);
	virtual int OnClosed(WPARAM wParam, LPARAM lParam);
	virtual int OnSetPlayerID(WPARAM wParam, LPARAM lParam);
	virtual int OnTradeRequest(WPARAM wParam, LPARAM lParam);
	virtual int OnSetEquip(WPARAM wParam, LPARAM lParam);
	virtual int OnInviteGuild(WPARAM wParam, LPARAM lParam);
	virtual int OnLoveCharInfo(WPARAM wParam, LPARAM lParam);
	
	virtual int OnSlotToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int OnSlotToolTipOut(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int OnSlotToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam);

	virtual int OnWP1ToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int OnWP1ToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam);

	virtual int OnWP2ToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int OnWP2ToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam);

	int OnSlotRClick(unsigned int iID, WPARAM wParam, LPARAM lParam);
	int OnWP1RClick(unsigned int iID, WPARAM wParam, LPARAM lParam);
	int OnWP2RClick(unsigned int iID, WPARAM wParam, LPARAM lParam);

	void InitStatusWindow(SPWindow* pkParent);
	void InitEquipWindow(SPWindow* pkParent);
	void InitLoveWindow(SPWindow* pkParent);

	bool CheckInterval();
	bool SendTradeRequest();
	bool SendPcEquipItem();

	void SetCategory(int iType);
	void SetTradeButton(bool bEnable);
	bool IsTradeButtonEnable();

	void SetStatusInfo();
	void SetEquipInfo();

	void CreatePlayerEquipSlot();
	void DeletePlayerEquipSlot();
	void ClearPlayerEquipSlot();
	void ClearEquipWindow(bool bWeaponClean = true);
	void SetEquipWindow(CONTAINER_ITEM* pItem);
	void ItemSetting(CONTAINER_ITEM item);

	void ShowInfo_Love();
	void HideInfo_Love();
	void SetLoveCharInfo( int iLevel , int iDay , const char* pszCharName );
	
protected:
	int m_iCategory;
	RECT m_rcNameDst;
	GU_ID m_uiPlayerGUID;

	char m_szBuf[128];
	SPPlayer* m_pkPlayer;

	bool	m_bTradeRequest;
	float	m_fTradeDeley;
	float	m_fTradeAccumulate;
	bool	m_bOnceEquipSend;
	int		m_iEquipType;

	SPTitleRenderBase* m_pkNoUseTitle;
	SPTitleRenderBase* m_pkTitle;

	std::vector<SPItem*> m_vpPlayerEquip[MAX_PLAYER_EQUIP_CONTAINER];
};

#endif