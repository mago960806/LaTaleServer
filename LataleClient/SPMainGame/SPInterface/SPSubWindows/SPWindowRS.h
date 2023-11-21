// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowRS.h
//	created:	2007/9/28   Time:14:48
//	purpose:	
//		SNDA의 RS 서비스를 위해서 만든 창
//		RSSDKLDD.LIB, RSSDKDLL.DLL : 라이브러리
//		IRSSDK.h, RSSDKdefine.h, RSSMLTags.h : SNDA 정의 파일
//		tinystr.h, tinyxml.h : XML Parse를 위한 파일 (TinyXML2.5.3 사용)
//		tinyxml.lib : Root 폴더에 필요
//*********************************************************************/
#pragma once


#define WIID_RS_SKIN_PARENT			(WNDID_RSCLIENT + 1)
#define WIID_RS_SKIN_SUBBG			(WNDID_RSCLIENT + 2)
#define WIID_RS_SKIN_BG				(WNDID_RSCLIENT + 3)
#define WIID_RS_TITLE_IMG			(WNDID_RSCLIENT + 4)
#define WIID_RS_EXIT				(WNDID_RSCLIENT + 5)
#define WIID_RS_SKIN_LINE			(WNDID_RSCLIENT + 6)
#define WIID_RS_TAB_MENU_01			(WNDID_RSCLIENT + 11)
#define WIID_RS_TAB_MENU_02			(WNDID_RSCLIENT + 12)
#define WIID_RS_TAB_MENU_03			(WNDID_RSCLIENT + 13)
#define WIID_RS_TAB_MENU_04			(WNDID_RSCLIENT + 14)
#define WIID_RS_TAB_MENU_05			(WNDID_RSCLIENT + 15)
#define WIID_RS_MODAL_MSG_BG		(WNDID_RSCLIENT + 16)		// Select Tab -> Show Modal MessageBox
#define WIID_RS_MODAL_MSG_TEXT		(WNDID_RSCLIENT + 17)		// Receive data -> Hide Modal MessageBox

//---------------------------------------------------------------------
#define WIID_RS_WND_HOTMAIN			(WNDID_RSCLIENT + 101)
#define WIID_RS_HOT_ICON			(WNDID_RSCLIENT + 102)
#define WIID_RS_HOT_TITLE			(WNDID_RSCLIENT + 103)
#define WIID_RS_HOT_ICON_URL		(WNDID_RSCLIENT + 111)		// MAX 10
#define WIID_RS_HOT_TEXT_URL		(WNDID_RSCLIENT + 121)		// MAX 10
#define WIID_RS_HOT_SELECT_BG		(WNDID_RSCLIENT + 132)
#define WIID_RS_QUESTION_TITLE		(WNDID_RSCLIENT + 133)
#define WIID_RS_QUESTION_BG			(WNDID_RSCLIENT + 134)
#define WIID_RS_QUESTION_EDIT		(WNDID_RSCLIENT + 135)
#define WIID_RS_QUESTION_BTN		(WNDID_RSCLIENT + 136)

//---------------------------------------------------------------------
#define WIID_RS_WND_MASTERANS		(WNDID_RSCLIENT + 201)
#define WIID_RS_MSTANS_ICON			(WNDID_RSCLIENT + 202)
#define WIID_RS_MSTANS_TITLE		(WNDID_RSCLIENT + 203)
#define WIID_RS_MSTANS_BRACKET_L	(WNDID_RSCLIENT + 204)
#define WIID_RS_MSTANS_BRACKET_R	(WNDID_RSCLIENT + 205)
#define WIID_RS_MSTANS_TEXT			(WNDID_RSCLIENT + 206)
#define WIID_RS_MSTANS_LIST_UP		(WNDID_RSCLIENT + 210)
#define WIID_RS_MSTANS_LIST_DOWN	(WNDID_RSCLIENT + 211)
#define WIID_RS_MSTANS_LIST_SLIDER	(WNDID_RSCLIENT + 212)
#define WIID_RS_OTHANS_ICON			(WNDID_RSCLIENT + 213)
#define WIID_RS_OTHANS_TITLE		(WNDID_RSCLIENT + 214)
#define WIID_RS_OTHANS_LINE_T		(WNDID_RSCLIENT + 215)
#define WIID_RS_OTHANS_LINE_B		(WNDID_RSCLIENT + 216)
#define WIID_RS_OTHANS_LIST			(WNDID_RSCLIENT + 217)		// MAX 10 
#define WIID_RS_OTHANS_LIST_UP		(WNDID_RSCLIENT + 230)
#define WIID_RS_OTHANS_LIST_DOWN	(WNDID_RSCLIENT + 231)
#define WIID_RS_OTHANS_LIST_SLIDER	(WNDID_RSCLIENT + 232)
#define WIID_RS_OTHANS_JUDGE_TEXT	(WNDID_RSCLIENT + 233)
#define WIID_RS_OTHANS_JUDGE_YES	(WNDID_RSCLIENT + 234)
#define WIID_RS_OTHANS_JUDGE_NO		(WNDID_RSCLIENT + 235)

//---------------------------------------------------------------------
#define WIID_RS_WND_JUDGEANS		(WNDID_RSCLIENT + 301)
#define WIID_RS_JUDGE_ICON			(WNDID_RSCLIENT + 302)
#define WIID_RS_JUDGE_RADIO			(WNDID_RSCLIENT + 311)		// MAX 9
#define WIID_RS_JUDGE_TEXT			(WNDID_RSCLIENT + 321)		// MAX 9
#define WIID_RS_JUDGE_EDIT_BG		(WNDID_RSCLIENT + 331)
#define WIID_RS_JUDGE_EDIT			(WNDID_RSCLIENT + 332)
#define WIID_RS_JUDGE_BTN			(WNDID_RSCLIENT + 333)

//---------------------------------------------------------------------
#define MAX_RS_URL_SIZE				10
#define MAX_RS_RADIO_SIZE			9
#define MAX_RS_MENU_SIZE			5

//---------------------------------------------------------------------
// RS Menu
enum RS_SHOW_MODE
{
	RS_SHOW_WAIT		= 0,
	RS_SHOW_SUBMENU		= 1,
	RS_SHOW_MENUDETAIL	= 2,
	RS_SHOW_HOTTOPIC	= 3,
	RS_SHOW_ANSWER		= 4,
	RS_SHOW_JUDGE		= 5,
} ;
//---------------------------------------------------------------------
class SPWindow;
class SPWindowRSList;
class SPWindowRS : public SPWindow
{
public:
	SPWindowRS(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowRS(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowRS();

	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Render(float fTime);

	void ShowRsWindow(RS_SHOW_MODE iMode) ;

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnExit				(WPARAM wParam, LPARAM lParam);
	virtual int OnTabMemuSelect		(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int OnHotURLClick		(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int OnSendQuestion		(WPARAM wParam, LPARAM lParam);

	virtual int OnOtherAnsClick		(WPARAM wParam, LPARAM lParam);
	virtual int OnJudgeYes			(WPARAM wParam, LPARAM lParam);
	virtual int OnJudgeNo			(WPARAM wParam, LPARAM lParam);
	virtual int OnMASliderUp		(WPARAM wParam, LPARAM lParam);
	virtual int OnMASliderDown		(WPARAM wParam, LPARAM lParam);
	virtual int OnOASliderUp		(WPARAM wParam, LPARAM lParam);
	virtual int OnOASliderDown		(WPARAM wParam, LPARAM lParam);
	virtual int OnWheelUp			(WPARAM wParam, LPARAM lParam);
	virtual int OnWheelDown			(WPARAM wParam, LPARAM lParam);

	virtual int OnJudgeSelect		(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int OnJudgeSelectText	(unsigned int iID, WPARAM wParam, LPARAM lParam);
	virtual int OnJudgeSend			(WPARAM wParam, LPARAM lParam);

	//---------------------------------------------------------------------
	// On RsManager_Proc
	virtual int OnRecvMainMenu		(WPARAM wParam, LPARAM lParam);
	virtual int OnRecvSubMenu		(WPARAM wParam, LPARAM lParam);
	virtual int OnRecvTopTopic		(WPARAM wParam, LPARAM lParam);
	virtual int OnRecvAnswer		(WPARAM wParam, LPARAM lParam);
	virtual int OnRecvUnSatReason	(WPARAM wParam, LPARAM lParam);

	int			OnEditSetFocus		( WPARAM wParam, LPARAM lParam );
	int			OnEditKillFocus		( WPARAM wParam, LPARAM lParam );
	virtual int OnHitTest(unsigned int iID, WPARAM wParam, LPARAM lParam);
	// End SPIMESSAGE

private:			// Local Function
	void		InitHotWindow() ;
	void		InitAnswerWindow() ;
	void		InitJudgeWindow() ;

	void		AddMasterAnswer(const char* pAddMesg) ;
	void		AddOtherAnswer(const char* pAddMesg, int iRetValue = 0, const char* pRetStr = NULL) ;
	void		ClearAnswerWindow() ;
	void		ClearJudgeWindow() ;

//	void		ShowModalMessage() ;		// if Send Packet then Show modal message box

protected:			// Local Variable
	SPWindow*	m_pkSkinWindow ;
	SPWindow*	m_pkHotWindow ;
	SPWindow*	m_pkAnsWindow ;
	SPWindow*	m_pkJudgeWindow ;
	SPWindowRSList* m_pkMasterAns ;
	SPWindowRSList* m_pkAnsList ;

	int			m_iJudgeIndex ;

	RS_SHOW_MODE m_eCurMode ;
};
