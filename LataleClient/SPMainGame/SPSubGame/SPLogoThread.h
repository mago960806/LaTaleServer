
// Copyright (C) duragon
//***************************************************************************
// System Name : SPLogoThread
// Comment     : 
// Creation    : duragon 2007-02-26 ¿ÀÈÄ 2:56:36
//***************************************************************************

#pragma once

typedef struct INITTHREAD_INFO
{
	bool m_bDoingThread ;		// Doing Logo Thread
	bool m_bExit ;				// Trigger Thread Exit
	bool m_bFinish ;			// Thread Job Finish
	bool m_bLogoSkip ;			// Logo Skip
	float m_fSleepTime ;			// Thread SleepTime
	int  m_iCheckTime ;			// Loading Time Compute Time
	int	 m_iDestProtege ;		// Loading Percentage
	int	 m_iMinProtege ;
	std::string m_strThreadErr ;	// ErrorString
	std::string m_strInitErr ;	// ErrorString
	
	INITTHREAD_INFO()
	{
		m_bExit			= false ;
		m_bFinish		= false ;
		m_bLogoSkip		= false ;
		m_bDoingThread	= false ;
		m_fSleepTime	= 0.05f ;
		m_iCheckTime	= 0 ;
		m_iDestProtege	= 0 ;
		m_iMinProtege	= 0 ;
		m_strInitErr.clear() ;
		m_strThreadErr.clear() ;
	}
	void SetInitMsg(const char* pMsg)
	{
		m_strInitErr = pMsg ;
	}
	void SetThreadMsg(const char* pMsg)
	{
		m_strThreadErr = pMsg ;
	}
	void SetProtege(int iMin, int iMax)
	{
		m_iMinProtege	= iMin ;
		m_iDestProtege	= iMax ;
	}

} INITTHREAD_INFO ;


class SPLogoThread {
public:
	SPLogoThread();
	~SPLogoThread() ;

	bool			InitLogo(int iLogoCount = 3) ;
	void			ProcessLogo(float fTime = 0.0f) ;
	void			ClearLogo() ;

protected:
	void			ProcessLogoFade(float fTime = 0.0f) ;
	void			RenderLogoFade() ;

	bool			InitLoginLoad();
	void			ProcessLoginLoad(float fTime = 0.0f);
	void			RenderLoginLoad() ;
	void			RenderProgress() ;
	void			RenderPercentText();

	bool			ComputeFadeValue(int& iAlphaFade, bool bFadeIn) ;
	void			ComputeProetgeTime() ;


private:
	enum LOGO_SHOWSTATE { SHOWSTATE_LOGO = 0, SHOWSTATE_BACK } ;

	// For Logo Thread Process
	SPTexture*		m_pLogoImageCur ;
	SPTexture*		m_pLogoImage[3] ;		// Site Logo
	SPTexture*		m_pImageBack[2] ;		// Same as Login Background
	SPTexture*		m_pImageLoad ;			// Load Literature
	SPTexture*		m_pImageLoadDot ;		// Load Literature
	SPTexture*		m_pImageGauge;
	SPTexture*		m_pImageWrap;
	
	
	LOGO_SHOWSTATE	m_eLogoState ;
	float			m_fSHOW_TIME ;
	float			m_fHIDE_TIME ;
	float			m_fProcessTime ;
	float			m_fUpdateTime ;
	float			m_fLogoShowTime ;
	float			m_fPercentTime ;

	int				m_iALPHA_FADE_STEP ;
	int				m_iPercent ;			// Loading Protege
	int				m_iImageNo ;			// Current Display Image Number
	int				m_iLogoCount ;			// Logo Image Count
	int				m_iAlphaFade ;			// Fade Alpha Value ( 0 ~ 255 )
	int				m_iLoadFade_00 ;
	int				m_iLoadFade_01 ;
	int				m_iLoadFade_02 ;
	int				m_iLoadFade_03 ;
	int				m_iLoadFade_04 ;
	int				m_iLoadFade_05 ;
	bool			m_bFadeDir ;			// Fade Logo In(true), Out(false)
	bool			m_bFadeDir_00 ;			// Fade LoadBack In(true), Out(false)
	bool			m_bFadeDir_01 ;			// Fade Letter In(true), Out(false)
	bool			m_bFadeDir_02 ;			// Fade Letter In(true), Out(false)
	bool			m_bFadeDir_03 ;			// Fade Letter In(true), Out(false)
	bool			m_bFadeDir_04 ;			// Fade Letter In(true), Out(false)
	bool			m_bFadeDir_05 ;			// Fade Letter In(true), Out(false)

	RECT			m_rtWhiteSrc ;
	RECT			m_rtWhiteDst ;
	RECT			m_rtLogoSrc[6] ;
	RECT			m_rtLogoDst[6] ;
	RECT			m_RectGauge;
	
};

