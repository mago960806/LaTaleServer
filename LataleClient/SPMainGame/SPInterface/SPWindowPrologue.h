
#ifndef __SPWINDOW_PROLOGUE_H__
#define __SPWINDOW_PROLOGUE_H__

//------------------------------------------------------------------------------------
#define WIID_PROLOGUE_BUTTON_SKIN			(WIID_PROLOGUE + 1)
#define WIID_PROLOGUE_SKIP					(WIID_PROLOGUE + 2)
#define WIID_PROLOGUE_NEXT					(WIID_PROLOGUE + 3)

#define WIID_PROLOGUE_TALK_BOX				(WIID_PROLOGUE + 10)
//#define WIID_PROLOGUE_TALK_LEFT				(WIID_PROLOGUE + 11)
//#define WIID_PROLOGUE_TALK_MID				(WIID_PROLOGUE + 12)
//#define WIID_PROLOGUE_TALK_RIGHT			(WIID_PROLOGUE + 13)
//#define WIID_PROLOGUE_TALK_TAIL				(WIID_PROLOGUE + 14)
#define WIID_PROLOGUE_TALK_IMAGE			(WIID_PROLOGUE + 15)
#define WIID_PROLOGUE_TALK_MSG				(WIID_PROLOGUE + 16)
#define WIID_PROLOGUE_TALK_IMAGE_NAME		(WIID_PROLOGUE + 17)
#define WIID_PROLOGUE_TALK_LINE				(WIID_PROLOGUE + 18)

#define WIID_PROLOGUE_HELP_BOX				(WIID_PROLOGUE + 20)
#define WIID_PROLOGUE_HELP_LEFT				(WIID_PROLOGUE + 21)
#define WIID_PROLOGUE_HELP_MID				(WIID_PROLOGUE + 22)
#define WIID_PROLOGUE_HELP_RIGHT			(WIID_PROLOGUE + 23)
#define WIID_PROLOGUE_HELP_MSG				(WIID_PROLOGUE + 24)

//------------------------------------------------------------------------------------
enum PROLOGUE_STEP
{
	PLS_NULL = 0,
	PLS_COMIC1_CUT,
	PLS_ANIMATION_CUT,
	PLS_COMIC2_CUT,
	PLS_COMPLETE,
};

enum ANIMATION_STEP
{
	AS_NULL = 0,
	AS_ANIMATION,
	AS_TALK,
	AS_HELP,
};

class SPStoryScene;

class SPWindowPrologue : public SPWindow
{
public:
	SPWindowPrologue(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowPrologue(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	virtual ~SPWindowPrologue();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);
	virtual void Show(bool bWithChild = true);

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()
	
	virtual int OnSkip(WPARAM wParam, LPARAM lParam);
	virtual int OnNext(WPARAM wParam, LPARAM lParam);

	virtual int OnSetStep(WPARAM wParam, LPARAM lParam);
	virtual int OnActorSceneComplete(WPARAM wParam, LPARAM lParam);
	virtual int OnTalkMsgComplete(WPARAM wParam, LPARAM lParam);
	virtual int OnTalkMsgLButtonUp(WPARAM wParam, LPARAM lParam);

	void InitTalkBox();
	void InitHelpBox();
	void InitScene();
	void InitActorIris();
	void InitActorMonster();
	void ProcessScene(float fTime);
	void ProcessBlank(float fTime);
	void RenderBackground();
	void RenderScene();
	void RenderBlank();
	void SetWaitTime();

	bool CheckWaitTime(float fTime);
	bool CheckHelpKeyValue(SPInputStruct* pInput);
	bool CheckTalkKeyValue(SPInputStruct* pInput);

	void SetNextButton(bool bEnable);
	void SetSkipButton(bool bEnable);
	void SetTalkBoxShow(bool bShow);
	void SetTalkBox(int iNo, int iTailPos);
	void SetHelpBoxShow(bool bShow);
	void SetHelpBox(int iNo);

	void SetDamageFX(const char* pszActor, int iDamage, int iCritical);
	void SetBGM();
	void SetFacing(const char* pszActor, int iFacing);

protected:
	PROLOGUE_STEP m_eStep;
	ANIMATION_STEP m_eAniStep;
	int m_iCurComicIndex;

	int m_iCurHelpStringNo;
	int m_iCurTalkStringNo;
	int m_iCurSceneNo;
	bool m_bTalkComplete;
	bool m_bMouseEventComplete;
	POINT m_ptTalkTail[2];

	float m_fWaitTime;
	float m_fWaitAccmulateTime;
	unsigned __int64 m_uiAction;
	
	SPTexture* m_pkAniBgnTexture;
	SPTexture* m_pkLightBgnTexture;
	SPTexture* m_pkComicTexture[7];
	
	RECT m_rcSrcBlank[2];
	RECT m_rcDstBlank;
	bool m_bBlank;
	float m_fBlankAccumlateTime;
	SPTexture* m_pkBlankTexture;

	SPStoryScene* m_pkStoryScene;
};

#endif