#ifndef __SPWINDOW_MINI_TEAM_H__
#define __SPWINDOW_MINI_TEAM_H__

#define WIID_MINI_TEAM_SKIN_UPPER				(WIID_MINI_TEAM + 1)
#define WIID_MINI_TEAM_SKIN_MIDDLE				(WIID_MINI_TEAM + 2)
#define WIID_MINI_TEAM_SKIN_LOWER				(WIID_MINI_TEAM + 3)

#define WIID_MINI_TEAM_HEAD						(WIID_MINI_TEAM + 10)	// +4
#define WIID_MINI_TEAM_TYPE						(WIID_MINI_TEAM + 14)	// +4
#define WIID_MINI_TEAM_NAME						(WIID_MINI_TEAM + 18)	// +4
#define WIID_MINI_TEAM_HP_BG_L					(WIID_MINI_TEAM + 22)	// +4
#define WIID_MINI_TEAM_HP_BG_M					(WIID_MINI_TEAM + 26)	// +4
#define WIID_MINI_TEAM_HP_BG_R					(WIID_MINI_TEAM + 30)	// +4
#define WIID_MINI_TEAM_HP_C						(WIID_MINI_TEAM + 34)	// +4
#define WIID_MINI_TEAM_HP_R						(WIID_MINI_TEAM + 38)	// +4
#define WIID_MINI_TEAM_SP_BG_L					(WIID_MINI_TEAM + 42)	// +4
#define WIID_MINI_TEAM_SP_BG_M					(WIID_MINI_TEAM + 46)	// +4
#define WIID_MINI_TEAM_SP_BG_R					(WIID_MINI_TEAM + 50)	// +4
#define WIID_MINI_TEAM_SP_C						(WIID_MINI_TEAM + 54)	// +4
#define WIID_MINI_TEAM_SP_R						(WIID_MINI_TEAM + 58)	// +4
#define WIID_MINI_TEAM_OUT_RANGE				(WIID_MINI_TEAM + 62)	// +4
#define WIID_MINI_TEAM_SEPARATE_LINE			(WIID_MINI_TEAM + 66)	// +3

class SPWindow;
class SPWindowMiniTeam : public SPWindow
{
public:
	SPWindowMiniTeam(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowMiniTeam(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowMiniTeam();

	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);
	virtual void Close();

	struct STATUS_INFO
	{
		int iMaxHP;
		int iCurHP;
		int iMaxSP;
		int iCurSP;
		bool bOutofRange;
#if !defined(_CHANNEL_TEAM_VERSION_)
		int iServerID;
		bool bDifferentChannel;
#endif

		char szCharName[LEN_NAME + 1];
	};

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnJoin(WPARAM, LPARAM);
	virtual int OnSecede(WPARAM, LPARAM);
	virtual int OnUpdate(WPARAM, LPARAM);
	virtual int OnUpdateStatus(WPARAM, LPARAM);

	void HideSkinWindow();
	void ShowTeamWindow(int iIndex, TEAM_MEMBER* pMemberInfo);

protected:

	bool m_bOutofRange;
	int m_iInterval;
	int m_iGageWidth;
};

#endif