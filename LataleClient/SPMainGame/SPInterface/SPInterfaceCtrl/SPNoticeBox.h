// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2005-07-12 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

//#define WIID_NOTICEBOX_SKIN_UPPER		(WIID_NOTICEBOX + 1)
//#define WIID_NOTICEBOX_SKIN_MIDDLE		(WIID_NOTICEBOX + 2)
//#define WIID_NOTICEBOX_SKIN_LOWER		(WIID_NOTICEBOX + 3)
#define WIID_NOTICEBOX_TEXT				(WIID_NOTICEBOX + 4)

#define WIID_NOTICEBOX_LEFT				(WIID_NOTICEBOX + 5)
#define WIID_NOTICEBOX_RIGHT			(WIID_NOTICEBOX + 6)
#define WIID_NOTICEBOX_INPUT_EDIT		(WIID_NOTICEBOX + 7)
#define WIID_NOTICEBOX_COUNT_TEXT		(WIID_NOTICEBOX + 8)

#define WIID_NOTICEBOX_OK				(WIID_NOTICEBOX + 9)
#define WIID_NOTICEBOX_CANCEL			(WIID_NOTICEBOX + 10)
#define WIID_NOTICEBOX_IGNORE			(WIID_NOTICEBOX + 11)

enum NOTICE_TYPE {
	NOTICE_TYPE_MSG					= 0,
	NOTICE_TYPE_YES					= 1,
	NOTICE_TYPE_YESNO				= 2,
	NOTICE_TYPE_YESNOSLIDER			= 3,
	NOTICE_TYPE_YESNOIGNORE			= 4,
	NOTICE_TYPE_MAX					= 5,
};

enum NOTICE_ANSWER {
	NOTICE_ANSWER_YES,
	NOTICE_ANSWER_NO,
	NOTICE_ANSWER_IGNORE,
};


class SPNoticeBox :	public SPWindow
{
public:	
	SPNoticeBox(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent);
	~SPNoticeBox();

	virtual void Init();
	virtual void Clean();	
	virtual void Process(float fTime);	
	virtual void Render(float fTime);
	virtual void Show(bool bWithChild = true);
	virtual void Hide();

	bool Reply(SPIM SpimYesNo);
	bool ForceReply();

	bool Notice( const char* szMsg, SPWindow* pSender, NOTICE_TYPE NoticeType, int iShowDelay = 0, DWORD dwAlign = DT_CENTER | DT_VCENTER , SPIM IMDefaultAnswer = SPIM_NOTICE_NO  );
	bool Notice( const char* szMsg, SPGameObject* pSender, NOTICE_TYPE NoticeType, int iShowDelay = 0, DWORD dwAlign = DT_CENTER | DT_VCENTER , SPGM GMDefaultAnswer = SPGM_NOTICE_NO );


protected:

	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnMaxValue		( WPARAM, LPARAM );
	virtual int OnMinValue		( WPARAM, LPARAM );
	virtual int OnScrollLeft	( WPARAM, LPARAM );
	virtual int OnScrollRight	( WPARAM, LPARAM );
	int OnUpdateCount		(WPARAM wParam, LPARAM lParam);
	virtual int OnYes			( WPARAM, LPARAM );
	virtual int OnNo			( WPARAM, LPARAM );	
	int OnKeyReturn	( WPARAM wParam, LPARAM lParam);

	void SetWindowType();
	void UpdateInputString(const char* pszString);	
	void ResizeWindow();
	void ArrangeControl();
	void ArrangeButton();	


protected:

	string			m_strCurMessage;
	SPWindow*		m_pSenderWindow;
	SPGameObject*	m_pSenderGOB;

	SPIM			m_IMDefaultAnswer;
	SPGM			m_GMDefaultAnswer;

	float			m_fDelay;
	float			m_fAccumulate;
	int				m_iRemainSecond;
	float			m_fSecondDelay;
	float			m_fSecondAccumulate;
	
	int				m_iSelectNumber;	
	bool			m_bNoticeActive;

	int				m_iMaxNumber;
	int				m_iMinNumber;

	NOTICE_TYPE		m_eNoticeType;
	
	SPTexture*		m_pTextureBase;
	RECT			m_rtSrcBase[WINDOW_BASE_MAX];
	RECT			m_rtDestBase[WINDOW_BASE_MAX];

	SPWindowButton*		m_pYes;
	SPWindowButton*		m_pNo;
	SPWindowButton*		m_pIgnore;
	SPWindowEdit*		m_pEdit;
};