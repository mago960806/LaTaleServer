#ifndef __SPWINDOW_TEAM_H__
#define __SPWINDOW_TEAM_H__


#define WIID_TEAM_SKIN_UPPER			(WIID_TEAM + 1)
#define WIID_TEAM_SKIN_MIDDLE			(WIID_TEAM + 2)
#define WIID_TEAM_SKIN_LOWER			(WIID_TEAM + 3)
#define WIID_TEAM_TITLE_IMAGE			(WIID_TEAM + 4)
#define WIID_TEAM_TITLE_TEXT			(WIID_TEAM + 5)
#define WIID_TEAM_SUB_BG				(WIID_TEAM + 6)
#define WIID_TEAM_INVITE_EDIT			(WIID_TEAM + 7)
#define WIID_TEAM_INVITE_BTN			(WIID_TEAM + 8)
#define WIID_TEAM_SLOT_LEFT				(WIID_TEAM + 10)		// +4
#define WIID_TEAM_SLOT_RIGHT			(WIID_TEAM + 14)		// +4
#define WIID_TEAM_SLOT_MIDDLE			(WIID_TEAM + 18)		// +4
#define WIID_TEAM_SLOT_NUMBER			(WIID_TEAM + 22)		// +4
#define WIID_TEAM_SLOT_PLAYER_TYPE		(WIID_TEAM + 26)		// +4
#define WIID_TEAM_SLOT_PLAYER_ID		(WIID_TEAM + 30)		// +4
#define WIID_TEAM_SLOT_PLAYER_POS		(WIID_TEAM + 34)		// +4
#define WIID_TEAM_SLOT_PLAYER_SECEDE	(WIID_TEAM + 38)		// +4

#define WIID_TEAM_LOOTING_RULE_IMAGE	(WIID_TEAM + 50)
#define WIID_TEAM_LOOTING_RULE_COMBO	(WIID_TEAM + 51)
#define WIID_TEAM_CHANGE_MASTER_IMAGE	(WIID_TEAM + 60)
#define WIID_TEAM_CHANGE_MASTER_COMBO	(WIID_TEAM + 61)
#define WIID_TEAM_EXIT					(WIID_TEAM + 70)

class SPWindow;
class SPWindowTeam : public SPWindow
{
public:
	SPWindowTeam(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowTeam(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowTeam();

	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Render(float fTime);

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnExit(WPARAM, LPARAM);
	virtual int OnPlayerInvite(WPARAM, LPARAM);
	virtual int OnInviteEditSetFocus(WPARAM, LPARAM);
	virtual int OnInviteEditKillFocus(WPARAM, LPARAM);

	virtual int OnJoin(WPARAM, LPARAM);
	virtual int OnSecede(WPARAM, LPARAM);
	virtual int OnUpdate(WPARAM, LPARAM);
	virtual int OnUpdateLootingRule(WPARAM, LPARAM);

	virtual int OnSecedeReq(unsigned int uiID, WPARAM, LPARAM);
	virtual int OnChangeLootingRuleReq(WPARAM, LPARAM);
	virtual int OnChangeMasterReq(WPARAM, LPARAM);

	void InitPlayerSlot(SPWindow* pkParent);
	void InitComboBox(SPWindow* pkParent);

	void SetCategory(int iType);
	void SetTeamPlayer(int iIndex, TEAM_MEMBER* pMemberInfo);

	void ClearTeamPlayer();

protected:

	int m_iPlayerNum;
	int m_iLootingRule;
	std::string m_strMasterName;
};

#endif