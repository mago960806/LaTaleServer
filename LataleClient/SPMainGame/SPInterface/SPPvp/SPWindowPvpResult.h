// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowPvpResult.h
//	created:	2007/11/14   Time:15:29
//	purpose:	
//*********************************************************************/

#pragma once

#define WIID_PVP_RESULT_BACKGROUND		(WIID_PVP_RESULT + 90)		// Event 막기위한 BG
#define WIID_PVP_RESULT_RANKICON		(WIID_PVP_RESULT +  1)		// MAX 8
#define WIID_PVP_RESULT_CHARNAME		(WIID_PVP_RESULT + 11)		// MAX 8
#define WIID_PVP_RESULT_KILLCOUNT		(WIID_PVP_RESULT + 21)		// MAX 8
#define WIID_PVP_RESULT_KILLICON		(WIID_PVP_RESULT + 31)		// MAX 8
#define WIID_PVP_RESULT_DEATHCOUNT		(WIID_PVP_RESULT + 41)		// MAX 8
#define WIID_PVP_RESULT_WINRATE			(WIID_PVP_RESULT + 51)		// MAX 8
#define WIID_PVP_RESULT_PVPPOINT		(WIID_PVP_RESULT + 61)		// MAX 8
#define WIID_PVP_RESULT_MARK_ME			(WIID_PVP_RESULT + 81)
#define WIID_PVP_RESULT_MARK_ARROW		(WIID_PVP_RESULT + 82)
#define WIID_PVP_RESULT_EXIT			(WIID_PVP_RESULT + 83)

#define PVP_RESULT_BG				9
#define PVP_RESULT_UNROLL_TIME		1.0f
#define PVP_RESULT_WANE_TIME		0.2f
#define PVP_RESULT_FADE_TIME		0.5f
#define PVP_RESULT_CLOSE_TIMER		60.0f
#define PVP_RESULT_SOUND_EFFECT		886

struct PVP_RESULT_ANIMATION
{
	float fAccmulateTime;
	float fDelayTime;		// Frame Delay Time
	float fEndTime ;		// Animation Limit Time
	float fElapsedTime ;	// Animation Elapsed Time

	RECT rcDst;				// Start + (Move / EndTime * ProcessTime)
	RECT rcDstStart;		// Start
	RECT rcDstEnd;			// End
	RECT rcDstDiff;			// Difference (Start - End)
	RECT rcSrc;
	RECT rcSrcStart;
	RECT rcSrcEnd;
	RECT rcSrcDiff;			// Difference (Start - End)
};

enum PVP_RESULT_MODE {
	PVP_RESULT_MODE_NULL = 0,
	PVP_RESULT_MODE_ROLL,			// Scroll scrolling
	PVP_RESULT_MODE_WANE,			// Scroll scrolling
	PVP_RESULT_MODE_FADE,			// BG & Text Fade
	PVP_RESULT_MODE_END	,			// Result had showed
} ;


class SPTimer ;
class SPWindowPvpResult : public SPWindow {
public:	
	SPWindowPvpResult(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	
	virtual ~SPWindowPvpResult();

	virtual void Init();
	virtual void Clean();

	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	//void SetTestData(int iCount);

protected:
	void ReposOwnImage() ;							// Position Reposition
	void InitSubControl();							// SPWindow Control Create
	void SetUnrollAnimation(float fLimitTime) ;		// PVP Result Animation Create
	void SetWaneAnimation(float fLimitTime) ;		// PVP Result Animation Create
	void SetFadeAnimation(float fLimitTime) ;		// PVP Result Animation Create
	void SetEndAnimation() ;						// Animation End
	void ProcessUnroll(float fTime) ;				// PVP Result Animation coordinates Processing
	void ProcessWane(float fTime) ;					// PVP Result Animation coordinates Processing
	void ProcessFade(float fTime) ;					// PVP Result Animation coordinates Processing

	void ResultEffectSound() ;						// Result Effect Sound

	//void SetRankData(int iUserCount, const char* pUserNameList) ;		// Set TEST DATA


protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()	
	int OnExitPvp				( WPARAM wParam, LPARAM lParam );
	int OnSetResultData			( WPARAM wParam, LPARAM lParam );
	//}}

private:
	SPTexture*		m_pTextureOBJ ;
	SPTexture*		m_pTextureBG;
	RECT			m_rtSrcFade;
	RECT			m_rtDestFade;
	RECT			m_rtSrcBlur;
	RECT			m_rtDestBlur;
	RECT			m_rtSrcBG[PVP_RESULT_BG];
	RECT			m_rtDestBG[PVP_RESULT_BG];
	PVP_RESULT_MODE m_eShowMode ;

	float			m_fAccmulateTime;
	float			m_fDelayTime;		// Frame Delay Time
	float			m_fEndTime ;		// Animation Limit Time
	float			m_fElapsedTime ;	// Animation Elapsed Time
	PVP_RESULT_ANIMATION m_stBGAnimation[3] ;
	int				m_iAlpha ;

	SPTimer*		m_pExitTimer ;
	bool			m_bIsExit ;
};