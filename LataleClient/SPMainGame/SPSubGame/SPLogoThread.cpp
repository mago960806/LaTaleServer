
#include "SPCommon.h"
#include "SPUtil.h"
#include "SPResourceDEF.h"
#include "SPMainGameDEF.H"

#include "SPResourceManager.h"
#include "SPLogoThread.h"

#include "_Error_log.h"

#define CHECK_TIME_PARAM		8

extern INITTHREAD_INFO g_InitialThreadInfo ; 

SPLogoThread::SPLogoThread()
: m_pLogoImageCur	(NULL)
, m_pImageLoad		(NULL)
, m_eLogoState		(SHOWSTATE_LOGO)
{

}

SPLogoThread::~SPLogoThread()
{

}


bool SPLogoThread::InitLogo(int iLogoCount/*=3*/)
{
	m_iLogoCount = iLogoCount ;
	if( g_pVideo == NULL )		return false ;
	m_pImageBack[0] = NULL ;
	m_pImageBack[1] = NULL ;
	m_pLogoImage[0] = NULL ;
	m_pLogoImage[1] = NULL ;
	m_pLogoImage[2] = NULL ;

	//if( !g_pVideo->GetTextureMgr()->LoadTexture( "DATA/LOGGIN/LOGIN003.PNG", &(m_pImageBack[0])) )
	if( !g_pVideo->GetTextureMgr()->GetTexture( "DATA/LOGGIN/LOGIN003.PNG", &(m_pImageBack[0]),TEX_INTERFACE,LEVEL_NOW) )
		g_InitialThreadInfo.m_strThreadErr = "Load Fail LOGIN003.PNG" ;
	//if( !g_pVideo->GetTextureMgr()->LoadTexture( "DATA/LOGGIN/LOGIN002.PNG", &(m_pImageBack[1])) )
	if( !g_pVideo->GetTextureMgr()->GetTexture( "DATA/LOGGIN/LOGIN002.PNG", &(m_pImageBack[1]),TEX_INTERFACE,LEVEL_NOW) )
		g_InitialThreadInfo.m_strThreadErr = "Load Fail LOGIN002.PNG" ;

	//if( !g_pVideo->GetTextureMgr()->LoadTexture( "DATA/LOGO/NowLoading1.PNG", &m_pImageLoad) )
	if( !g_pVideo->GetTextureMgr()->GetTexture( "DATA/LOGO/NowLoading1.PNG", &m_pImageLoad,TEX_INTERFACE,LEVEL_NOW ))
		g_InitialThreadInfo.m_strThreadErr = "Load Fail NowLoading1.PNG" ;
	//if( !g_pVideo->GetTextureMgr()->LoadTexture( "DATA/LOGO/NowLoading2.PNG", &m_pImageLoadDot) )
	if( !g_pVideo->GetTextureMgr()->GetTexture( "DATA/LOGO/NowLoading2.PNG", &m_pImageLoadDot,TEX_INTERFACE,LEVEL_NOW) )
		g_InitialThreadInfo.m_strThreadErr = "Load Fail NowLoading2.PNG" ;

	//if( !g_pVideo->GetTextureMgr()->LoadTexture( "DATA/LOGO/LOGO_01.PNG", &(m_pLogoImage[0])) )
	if( !g_pVideo->GetTextureMgr()->GetTexture( "DATA/LOGO/LOGO_01.PNG", &(m_pLogoImage[0]),TEX_INTERFACE,LEVEL_NOW) )
		g_InitialThreadInfo.m_strThreadErr = "Load Fail LOGO_01.PNG" ;
	//if( !g_pVideo->GetTextureMgr()->LoadTexture( "DATA/LOGO/LOGO_02.PNG", &(m_pLogoImage[1])) )
	if( !g_pVideo->GetTextureMgr()->GetTexture( "DATA/LOGO/LOGO_02.PNG", &(m_pLogoImage[1]),TEX_INTERFACE,LEVEL_NOW) )
		g_InitialThreadInfo.m_strThreadErr = "Load Fail LOGO_02.PNG" ;
	//if( !g_pVideo->GetTextureMgr()->LoadTexture( "DATA/LOGO/LOGO_03.PNG", &(m_pLogoImage[2])) )
	if( !g_pVideo->GetTextureMgr()->GetTexture( "DATA/LOGO/LOGO_03.PNG", &(m_pLogoImage[2]),TEX_INTERFACE,LEVEL_NOW) )
		g_InitialThreadInfo.m_strThreadErr = "Load Fail LOGO_03.PNG" ;

	//if( !g_pVideo->GetTextureMgr()->LoadTexture("DATA/LOGGIN/LOADGAUGE.PNG", &m_pImageGauge) )
	if( !g_pVideo->GetTextureMgr()->GetTexture("DATA/LOGGIN/LOADGAUGE.PNG", &m_pImageGauge,TEX_INTERFACE,LEVEL_NOW ))
		g_InitialThreadInfo.m_strThreadErr = "Load Fail LOADGAUGE.PNG" ;
	//if( !g_pVideo->GetTextureMgr()->LoadTexture("DATA/LOGGIN/GAUGEBAR.PNG", &m_pImageWrap) )
	if( !g_pVideo->GetTextureMgr()->GetTexture("DATA/LOGGIN/GAUGEBAR.PNG", &m_pImageWrap,TEX_INTERFACE,LEVEL_NOW ))
		g_InitialThreadInfo.m_strThreadErr = "Load Fail GAUGEBAR.PNG" ;

	// Variable Initialize
	m_bFadeDir			= true ;
	m_fLogoShowTime		= 0 ;
	m_fProcessTime		= 0 ;
	m_fUpdateTime		= 0 ;
	m_iAlphaFade		= 0 ;
	m_iImageNo			= 0 ;
	m_pLogoImageCur		= m_pLogoImage[m_iImageNo] ;
	m_eLogoState		= SHOWSTATE_LOGO ;
	m_iPercent			= 0 ;
#ifdef _DEBUG
	m_fPercentTime		= 0.6f ;
#else
	m_fPercentTime		= 0.2f ;
#endif

	//gauge.png (166, 528) - 가로로 확대 (431px까지 확대 하면 100%가 됩니다)
	m_RectGauge.top	    = 528;
	m_RectGauge.bottom  = m_RectGauge.top + 32;
	m_RectGauge.left	= 206;
	m_RectGauge.right	= m_RectGauge.left;

	// Definition
	m_fSHOW_TIME		= 1.2f ;		// 1.5sec
	m_fHIDE_TIME		= 0.1f ;		// 0.2sec
	m_iALPHA_FADE_STEP	= 30 ;			// 50ms * 255 / 30 = 0.425sec

	SetRect(&m_rtWhiteSrc, 0, 0, 16, 16) ;
	SetRect(&m_rtWhiteDst, 0, 0, 800, 600) ;

	SetRect(&m_rtLogoSrc[0], 0, 0, 512, 512);
	SetRect(&m_rtLogoSrc[1], 512, 0, 800, 512);
	SetRect(&m_rtLogoSrc[2], 800, 0, 1024, 88);
	SetRect(&m_rtLogoSrc[3], 800, 88, 1024, 176);
	SetRect(&m_rtLogoSrc[4], 800, 176, 1024, 264);
	SetRect(&m_rtLogoSrc[5], 800, 264, 928, 352);

	SetRect(&m_rtLogoDst[0], 0, 0, 512, 512);
	SetRect(&m_rtLogoDst[1], 512, 0, 800, 512);
	SetRect(&m_rtLogoDst[2], 0, 512, 224, 600);
	SetRect(&m_rtLogoDst[3], 224, 512, 448, 600);
	SetRect(&m_rtLogoDst[4], 448, 512, 672, 600);
	SetRect(&m_rtLogoDst[5], 672, 512, 800, 600);

	return true ;
}


bool SPLogoThread::InitLoginLoad()
{
	m_iAlphaFade	= 0 ;
	m_bFadeDir		= true ;
	m_iALPHA_FADE_STEP = 20 ;
	m_iLoadFade_00	= 0 ;
	m_iLoadFade_01	= 200 ;
	m_iLoadFade_02	= 150 ;
	m_iLoadFade_03	= 100 ;
	m_iLoadFade_04	= 50 ;
	m_iLoadFade_05	= 0 ;
	m_bFadeDir_01	= true ;
	m_bFadeDir_02	= true ;
	m_bFadeDir_03	= true ;
	m_bFadeDir_04	= true ;
	m_bFadeDir_05	= true ;
	return true ;
}


void SPLogoThread::ClearLogo()
{
	SAFE_RELEASE(m_pLogoImage[0]) ;
	SAFE_RELEASE(m_pLogoImage[1]) ;
	SAFE_RELEASE(m_pLogoImage[2]) ;

	SAFE_RELEASE(m_pImageBack[0]) ;
	SAFE_RELEASE(m_pImageBack[1]) ;

	SAFE_RELEASE(m_pImageLoad) ;
	SAFE_RELEASE(m_pImageLoadDot) ;

	SAFE_RELEASE(m_pImageWrap) ;
	SAFE_RELEASE(m_pImageGauge) ;
}



/**
	Logo Process 
*/
void SPLogoThread::ProcessLogo(float fTime /*=0.0f*/)
{
	if( g_InitialThreadInfo.m_iCheckTime > 0 )
		ComputeProetgeTime() ;

	if( g_InitialThreadInfo.m_bLogoSkip )	{
		m_eLogoState = SHOWSTATE_BACK ;
		InitLoginLoad() ;
	}

	if( m_eLogoState == SHOWSTATE_LOGO ) 
	{
		ProcessLogoFade(fTime) ;
		RenderLogoFade() ;
	}
	else
	{
		ProcessLoginLoad(fTime) ;
		RenderLoginLoad() ;
	}
}



/**
	Render Logo
*/
void SPLogoThread::ProcessLogoFade(float fTime/*=0.0f*/)
{
	if( ComputeFadeValue(m_iAlphaFade, m_bFadeDir) == true )
	{
		// Delay
		m_fLogoShowTime += fTime ;
		if( m_bFadeDir == true && m_fLogoShowTime > m_fSHOW_TIME || m_bFadeDir == false && m_fLogoShowTime > m_fHIDE_TIME )
		{
			m_bFadeDir = ( m_bFadeDir ) ? false : true ;
			m_fLogoShowTime = 0.0f ;

			if( m_bFadeDir == true )		// Logo Change
			{
				++m_iImageNo ;
				while( m_iImageNo <= 2 && m_pLogoImage[m_iImageNo] == NULL )	{
					++m_iImageNo ;
				}
				if( m_iImageNo > 2 ) {
					m_iImageNo		= 0 ;
					m_eLogoState	= SHOWSTATE_BACK ;
					InitLoginLoad() ;
				}
				else 
					m_pLogoImageCur = m_pLogoImage[m_iImageNo] ;
			}
		}
	}

}

void SPLogoThread::RenderLogoFade()
{
	if( !g_pVideo->BeginRender() )		
		return;

	if( m_pLogoImage[0]) {
		m_pLogoImage[0]->RenderTexture(&m_rtWhiteDst, &m_rtWhiteSrc);		// White Background
		g_pVideo->Flush();
	}

	if(m_pLogoImageCur) {
		m_pLogoImageCur->SetColor(RGBA(255, 255, 255, m_iAlphaFade));
		m_pLogoImageCur->RenderTexture(&m_rtLogoDst[0], &m_rtLogoSrc[0]);
		m_pLogoImageCur->RenderTexture(&m_rtLogoDst[1], &m_rtLogoSrc[1]);
		m_pLogoImageCur->RenderTexture(&m_rtLogoDst[2], &m_rtLogoSrc[2]);
		m_pLogoImageCur->RenderTexture(&m_rtLogoDst[3], &m_rtLogoSrc[3]);
		m_pLogoImageCur->RenderTexture(&m_rtLogoDst[4], &m_rtLogoSrc[4]);
		m_pLogoImageCur->RenderTexture(&m_rtLogoDst[5], &m_rtLogoSrc[5]);
		m_pLogoImageCur->SetColor(RGBA(255, 255, 255, 255));
		g_pVideo->Flush();
	}
	g_pVideo->EndRender();
}



/**
	Render Login Background & Loading...
	Change AlphaVaule Effect
*/
void SPLogoThread::ProcessLoginLoad(float fTime /*=0.0f*/)
{
	m_fProcessTime += fTime ;
	if( m_fProcessTime > m_fPercentTime )
	{
		if( m_iPercent < g_InitialThreadInfo.m_iDestProtege )
			++m_iPercent ;
		else
			m_iPercent = g_InitialThreadInfo.m_iDestProtege ;
			
		m_fProcessTime = 0 ;
		
		//if( m_iPercent != 0 && (m_iPercent % 4 == 0) )
		//	ErrorFLogs->Error_Log("----->>> [ %d Percent ]", m_iPercent ) ;		
	}
	m_fUpdateTime += fTime ;
	if( m_fUpdateTime > 0.02f )
	{
		m_fUpdateTime = 0 ;
		if( m_iPercent < g_InitialThreadInfo.m_iMinProtege )
			++m_iPercent ;
		if( g_InitialThreadInfo.m_iMinProtege == 100 )
			m_iPercent = 100 ;
	}

	m_RectGauge.right	= m_RectGauge.left + (395.0f * ((float)m_iPercent / 100));

	if( ComputeFadeValue(m_iAlphaFade, true) == true )
	{
		if( ComputeFadeValue(m_iLoadFade_00, m_bFadeDir_00) == true )	
			m_bFadeDir_00 = ( m_bFadeDir_00 ) ? false : true ;
		if( ComputeFadeValue(m_iLoadFade_01, m_bFadeDir_01) == true )	
			m_bFadeDir_01 = ( m_bFadeDir_01 ) ? false : true ;
		if( ComputeFadeValue(m_iLoadFade_02, m_bFadeDir_02) == true )	
			m_bFadeDir_02 = ( m_bFadeDir_02 ) ? false : true ;
		if( ComputeFadeValue(m_iLoadFade_03, m_bFadeDir_03) == true )	
			m_bFadeDir_03 = ( m_bFadeDir_03 ) ? false : true ;
		if( ComputeFadeValue(m_iLoadFade_04, m_bFadeDir_04) == true )	
			m_bFadeDir_04 = ( m_bFadeDir_04 ) ? false : true ;
		if( ComputeFadeValue(m_iLoadFade_05, m_bFadeDir_05) == true )	
			m_bFadeDir_05 = ( m_bFadeDir_05 ) ? false : true ;
	}
}

void SPLogoThread::RenderLoginLoad()
{
	if( !g_pVideo->BeginRender() )		
		return;

	if( m_pLogoImage[0]) {
		m_pLogoImage[0]->RenderTexture(&m_rtWhiteDst, &m_rtWhiteSrc);		// White Background
		g_pVideo->Flush();
	}

	if(m_pImageBack[0]) {
		m_pImageBack[0]->SetColor(RGBA(255, 255, 255, m_iAlphaFade));		// Forest
		m_pImageBack[0]->RenderTexture(&m_rtLogoDst[0], &m_rtLogoSrc[0]);
		m_pImageBack[0]->RenderTexture(&m_rtLogoDst[1], &m_rtLogoSrc[1]);
		m_pImageBack[0]->RenderTexture(&m_rtLogoDst[2], &m_rtLogoSrc[2]);
		m_pImageBack[0]->RenderTexture(&m_rtLogoDst[3], &m_rtLogoSrc[3]);
		m_pImageBack[0]->RenderTexture(&m_rtLogoDst[4], &m_rtLogoSrc[4]);
		m_pImageBack[0]->RenderTexture(&m_rtLogoDst[5], &m_rtLogoSrc[5]);
		m_pImageBack[0]->SetColor(RGBA(255, 255, 255, 255));
		g_pVideo->Flush();
	}
	if( m_pImageBack[1] ) {
		m_pImageBack[1]->SetColor(RGBA(255, 255, 255, m_iAlphaFade));
		m_pImageBack[1]->RenderTexture(&m_rtLogoDst[0], &m_rtLogoSrc[0]);	// Character
		m_pImageBack[1]->RenderTexture(&m_rtLogoDst[1], &m_rtLogoSrc[1]);
		m_pImageBack[1]->RenderTexture(&m_rtLogoDst[2], &m_rtLogoSrc[2]);
		m_pImageBack[1]->RenderTexture(&m_rtLogoDst[3], &m_rtLogoSrc[3]);
		m_pImageBack[1]->RenderTexture(&m_rtLogoDst[4], &m_rtLogoSrc[4]);
		m_pImageBack[1]->RenderTexture(&m_rtLogoDst[5], &m_rtLogoSrc[5]);
		m_pImageBack[1]->SetColor(RGBA(255, 255, 255, 255));
		g_pVideo->Flush();
	}

	if( m_iAlphaFade == 255 && m_pImageLoad)
	{
		m_pImageLoad->SetColor(RGBA(255, 255, 255, m_iLoadFade_00));
		m_pImageLoad->RenderTexture(313, 238);								// Loading
		m_pImageLoad->SetColor(RGBA(255, 255, 255, 255));

		m_pImageLoadDot->SetColor(RGBA(255, 255, 255, m_iLoadFade_01));
		m_pImageLoadDot->RenderTexture(440, 267);							// .
		m_pImageLoadDot->SetColor(RGBA(255, 255, 255, m_iLoadFade_02));
		m_pImageLoadDot->RenderTexture(454, 267);
		m_pImageLoadDot->SetColor(RGBA(255, 255, 255, m_iLoadFade_03));
		m_pImageLoadDot->RenderTexture(468, 267);
		m_pImageLoadDot->SetColor(RGBA(255, 255, 255, m_iLoadFade_04));
		m_pImageLoadDot->RenderTexture(482, 267);
		m_pImageLoadDot->SetColor(RGBA(255, 255, 255, m_iLoadFade_05));
		m_pImageLoadDot->RenderTexture(496, 267);
		m_pImageLoadDot->SetColor(RGBA(255, 255, 255, 255));
		g_pVideo->Flush();
	}

	RenderProgress() ;

#ifdef _DEBUG
	RenderPercentText() ;
#endif

	g_pVideo->EndRender();
}


void SPLogoThread::RenderPercentText()
{
	TCHAR szMsg[MAX_PATH] = TEXT("");

	g_pVideo->GetFont(FONT_12_BOLD)->SetScale(1.0f, 1.0f);
	wsprintf( szMsg,  "%d Percent", m_iPercent);
	g_pVideo->GetFont(FONT_12_BOLD)->RenderTextPos( 333, 210, szMsg);
}


void SPLogoThread::RenderProgress()
{
	if( m_pImageGauge != NULL )
		m_pImageGauge->RenderTexture(&m_RectGauge, NULL);
	g_pVideo->Flush();

	if( m_pImageWrap != NULL )
		m_pImageWrap->RenderTexture(144, 512);
	g_pVideo->Flush();
}



/**
	According Direction, Compute Alpha channel value
	if Alpha channel is full Then return true ;
*/
bool SPLogoThread::ComputeFadeValue(int &iAlphaFade, bool bFadeIn)
{
	if( bFadeIn == true )
	{
		iAlphaFade += m_iALPHA_FADE_STEP ;
		if( iAlphaFade > 255 ) {
			iAlphaFade = 255 ;
			return true ;
		}
	}
	else
	{
		iAlphaFade -= m_iALPHA_FADE_STEP ;
		if( iAlphaFade < 0 )	{
			iAlphaFade = 0 ;
			return true ;
		}
	}
	return false ;
}



/**
	g_InitialThreadInfo.m_iCheckTime is 1/7 Logo Time value
	
*/
void SPLogoThread::ComputeProetgeTime()
{
	if( g_InitialThreadInfo.m_iCheckTime == 0 )	return ;
	m_fPercentTime = (float)( g_InitialThreadInfo.m_iCheckTime * CHECK_TIME_PARAM ) / 100000.0f ;
	//ErrorFLogs->Error_Log("----->>> Compute ViewTime : [1Percent = %.3f][%d]", m_fPercentTime, g_InitialThreadInfo.m_iCheckTime ) ;
	g_InitialThreadInfo.m_iCheckTime = 0 ;
}


