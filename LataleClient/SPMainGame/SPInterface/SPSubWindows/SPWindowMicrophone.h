#pragma once


#define WIID_MICROPHONE_TOP					(WIID_MICROPHONE	+	1)		//스킨 TOP
#define WIID_MICROPHONE_CENTER				(WIID_MICROPHONE	+	2)		//스킨 CENTER
#define WIID_MICROPHONE_BOTTOM				(WIID_MICROPHONE	+	3)		//스킨 BOTTOM

//#define WIID_MICROPHONE_CLOSE				(WIID_MICROPHONE	+	5)		//Close
#define WIID_MICROPHONE_OK					(WIID_MICROPHONE	+	6)		//OK
#define WIID_MICROPHONE_CANCEL				(WIID_MICROPHONE	+	7)		//Cancel

#define WIID_MICROPHONE_TITLE				(WIID_MICROPHONE	+	10)		//Title

#define WIID_MICROPHONE_SERVER_HELP			(WIID_MICROPHONE	+	30)		//서버
#define WIID_MICROPHONE_CHANNEL_HELP		(WIID_MICROPHONE	+	31)		//채널

#define WIID_MICROPHONE_SEND_MSG			(WIID_MICROPHONE	+	100)	//내용
#define WIID_MICROPHONE_SEND_MSG_BASE		(WIID_MICROPHONE	+	101)	//내용


enum MIC_MODE {
	MIC_MODE_SERVER,
	MIC_MODE_CHANNEL,
	MIC_MODE_ITEM,
	MIC_MODE_MAX,
};

class SPWindow;
class SPWindowButton;
class SPWindowEditMultiLine;

class SPWindowMicropone : public SPWindow {

public:
	SPWindowMicropone( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowMicropone();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

protected:
	void InitSubControl();

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnMode				(WPARAM wParam, LPARAM lparam);
	int OnResult			(WPARAM wParam, LPARAM lparam);
	int	OnEditSetFocusRange		(WPARAM wParam, LPARAM lParam);
	int	OnEditKillFocusRange	(WPARAM wParam, LPARAM lParam);	
	//int OnClose				(WPARAM wParam, LPARAM lparam);
	int OnOk				(WPARAM wParam, LPARAM lparam);	
	int OnCancel			(WPARAM wParam, LPARAM lparam);		

private:
	SPWindowEditMultiLine*	m_pMsg;							/**< 내용 <br> */	
	SPWindow*				m_pHelp[MIC_MODE_MAX];			/**< 도움말 선택 버튼 <br> */
	SPWindowButton*			m_pOk;
	SPWindowButton*			m_pCancel;
	
	MIC_MODE				m_iMode;
};

