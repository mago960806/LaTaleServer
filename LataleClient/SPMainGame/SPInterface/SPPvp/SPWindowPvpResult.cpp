#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"
#include "GlobalDefines_Share.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"
#include "LT_Error.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowPvpResult.h"
#include "SPPvpManager.h"

#include "SPSoundArchive.h"
#include "SPSoundManager.h"

#include "SPTimer.h"

#include "_Error_log.h"


SPWindowPvpResult::SPWindowPvpResult(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_PVP_RESULT, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pTextureOBJ(NULL)
, m_pTextureBG(NULL)
, m_pExitTimer(NULL)
{
	Init();
}


SPWindowPvpResult::~SPWindowPvpResult()
{
	Clean();
}


void SPWindowPvpResult::Init()
{
	m_eShowMode = PVP_RESULT_MODE_NULL ;
	m_fAccmulateTime	= 0 ;
	m_fElapsedTime		= 0 ;
	m_fDelayTime		= 0 ;
	m_fEndTime			= 0 ;
	m_iAlpha			= 0 ;

	m_pExitTimer	= new SPTimer ;
	m_bIsExit		= false ;

	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , &m_pTextureOBJ);
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , &m_pTextureBG);

	// Background
	SetRect(&m_rtSrcBlur,	255, 183, 403, 280);	// Blur -> BG
	// Object
	SetRect(&m_rtSrcBG[0],	1, 106, 261, 140);		// Title
	SetRect(&m_rtSrcBG[1],	227, 496, 281, 511);	// Rank
	SetRect(&m_rtSrcBG[2],	282, 496, 385, 511);	// Name
	SetRect(&m_rtSrcBG[3],	386, 496, 444, 511);	// Kill Count
	SetRect(&m_rtSrcBG[4],	445, 496, 511, 511);	// Death Count
	SetRect(&m_rtSrcBG[5],	445, 480, 511, 495);	// Win Rate
	SetRect(&m_rtSrcBG[6],	445, 464, 511, 479);	// Pvp Point
	SetRect(&m_rtSrcBG[7],	507, 26, 509, 27);		// Top Line
	SetRect(&m_rtSrcBG[8],	507, 26, 509, 27);		// Bottom Line

	SetRect(&m_rtSrcFade,	502, 457, 506, 461);	// Fade BG
	SetRect(&m_rtDestFade,	0, 0, 800, 600);		// Fade BG

	InitSubControl();
	ReposOwnImage();
}


void SPWindowPvpResult::InitSubControl()
{
	SPWindowStatic* pStaticBG = NULL;
	SPWindowStatic* pStatic = NULL;
	SPWindowStatic* pChild = NULL;
	SPWindowButton* pButton = NULL;

	pStaticBG = new SPWindowStatic( WIID_PVP_RESULT_BACKGROUND, 0,  0, 800, 600, this);
	pStaticBG->Hide();

	pStatic = new SPWindowStatic( WIID_PVP_RESULT_MARK_ME, 33,  185, 450, 19, pStaticBG);
	pStatic->SetWindowType(WND_TYPE_LEFTBASE) ;
	pStatic->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 493, 1, 495, 20) ;
	pStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 497, 1, 499, 20) ;
	pStatic->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 501, 1, 503, 20) ;
	pStatic->Hide();

	pChild = new SPWindowStatic( WIID_PVP_RESULT_MARK_ARROW , 8,  6, 9, 8, pStatic);			// MARK's child
	pChild->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 493, 20) ;
	pChild->Hide();

	for( int i = 0 ; i < PVP_MAX_ROOMUSER; i++)
	{
		int j = i ? i / 4 : 0 ;
		int k = i ? i % 4 : 0 ;
		pStatic = new SPWindowStatic( WIID_PVP_RESULT_RANKICON + i  , 59,  89+i*24,  20,  18, pStaticBG);
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 427+k*21, 29+j*19) ;
		pStatic->Hide();

		pStatic = new SPWindowStatic( WIID_PVP_RESULT_CHARNAME + i , 97,  92+i*24, 105, 13, pStaticBG);
		i < 3 ? pStatic->SetFont(FONT_12_BOLD) : pStatic->SetFont(FONT_12_NORMAL);
		i < 3 ? pStatic->SetFontColor(RGBA(131, 148, 208, 255)) : pStatic->SetFontColor(RGBA(0, 0, 0, 255)) ;
		pStatic->SetFormat(DT_TOP | DT_CENTER);
		pStatic->Hide();

		pStatic = new SPWindowStatic( WIID_PVP_RESULT_KILLCOUNT + i , 208,  92+i*24, 29, 13, pStaticBG);
		i < 3 ? pStatic->SetFont(FONT_12_BOLD) : pStatic->SetFont(FONT_12_NORMAL);
		i < 3 ? pStatic->SetFontColor(RGBA(131, 148, 208, 255)) : pStatic->SetFontColor(RGBA(0, 0, 0, 255)) ;
		pStatic->SetFormat(DT_TOP | DT_RIGHT);
		pStatic->Hide();

		pStatic = new SPWindowStatic( WIID_PVP_RESULT_KILLICON + i , 240,  98+i*24, 22, 5, pStaticBG);
		i < 3 ? pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 344, 61) : pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 321, 61);
		pStatic->Hide();

		pStatic = new SPWindowStatic( WIID_PVP_RESULT_DEATHCOUNT + i , 285,  92+i*24, 29, 13, pStaticBG);
		i < 3 ? pStatic->SetFont(FONT_12_BOLD) : pStatic->SetFont(FONT_12_NORMAL);
		i < 3 ? pStatic->SetFontColor(RGBA(131, 148, 208, 255)) : pStatic->SetFontColor(RGBA(0, 0, 0, 255)) ;
		pStatic->SetFormat(DT_TOP | DT_CENTER);
		pStatic->Hide();

		pStatic = new SPWindowStatic( WIID_PVP_RESULT_WINRATE + i , 356,  92+i*24, 31, 13, pStaticBG);
		i < 3 ? pStatic->SetFont(FONT_12_BOLD) : pStatic->SetFont(FONT_12_NORMAL);
		i < 3 ? pStatic->SetFontColor(RGBA(131, 148, 208, 255)) : pStatic->SetFontColor(RGBA(0, 0, 0, 255)) ;
		pStatic->SetTextPeriod(false) ;
		pStatic->SetFormat(DT_TOP | DT_CENTER);
		pStatic->Hide();

		pStatic = new SPWindowStatic( WIID_PVP_RESULT_PVPPOINT + i , 412,  92+i*24, 60, 13, pStaticBG);
		i < 3 ? pStatic->SetFont(FONT_12_BOLD) : pStatic->SetFont(FONT_12_NORMAL);
		i < 3 ? pStatic->SetFontColor(RGBA(131, 148, 208, 255)) : pStatic->SetFontColor(RGBA(0, 0, 0, 255)) ;
		//pStatic->SetFormat(DT_TOP | DT_RIGHT);
		pStatic->SetFormat(DT_TOP | DT_CENTER);
		pStatic->Hide();
	}

	pButton = new SPWindowButton(WIID_PVP_RESULT_EXIT , 225, 317, 68, 17, pStaticBG);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 1,  217);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 1,  235);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 1,  253);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI_Button_000.PNG", 1,  271);
	pButton->Hide();
}


/**
	Scroll 이 펼쳐지는 Animation
 */
void SPWindowPvpResult::SetUnrollAnimation(float fLimitTime)
{
	m_eShowMode = PVP_RESULT_MODE_ROLL ;

	m_fAccmulateTime	= 0 ;
	m_fElapsedTime		= 0 ;
	m_fDelayTime		= 0.02f ;
	m_fEndTime			= fLimitTime ;

	SetRect(&m_stBGAnimation[0].rcSrc		, 1, 281, 494, 315) ;
	SetRect(&m_stBGAnimation[0].rcDstStart	, 162, 282, 642, 316) ;
	SetRect(&m_stBGAnimation[0].rcDstEnd	, 146, 119, 666, 156) ;
	g_pkPvpManager->RectMinus(m_stBGAnimation[0].rcDstEnd, m_stBGAnimation[0].rcDstStart, m_stBGAnimation[0].rcDstDiff) ;

	SetRect(&m_stBGAnimation[1].rcSrcStart	, 255, 91, 511, 91) ;
	SetRect(&m_stBGAnimation[1].rcSrcEnd	, 255,  1, 511, 181) ;
	g_pkPvpManager->RectMinus(m_stBGAnimation[1].rcSrcEnd, m_stBGAnimation[1].rcSrcStart, m_stBGAnimation[1].rcSrcDiff) ;
	SetRect(&m_stBGAnimation[1].rcDstStart	, 163, 299, 639, 308) ;
	SetRect(&m_stBGAnimation[1].rcDstEnd	, 152, 124, 662, 485) ;
	g_pkPvpManager->RectMinus(m_stBGAnimation[1].rcDstEnd, m_stBGAnimation[1].rcDstStart, m_stBGAnimation[1].rcDstDiff) ;

	SetRect(&m_stBGAnimation[2].rcSrc		, 1, 317, 497, 349) ;
	SetRect(&m_stBGAnimation[2].rcDstStart	, 159, 299, 641, 330) ;
	SetRect(&m_stBGAnimation[2].rcDstEnd	, 141, 458, 660, 496) ;
	g_pkPvpManager->RectMinus(m_stBGAnimation[2].rcDstEnd, m_stBGAnimation[2].rcDstStart, m_stBGAnimation[2].rcDstDiff) ;
}


/**
	BG가 줄어드는 Animation
*/
void SPWindowPvpResult::SetWaneAnimation(float fLimitTime)
{
	m_eShowMode = PVP_RESULT_MODE_WANE ;

	m_fAccmulateTime	= 0 ;
	m_fElapsedTime		= 0 ;
	m_fDelayTime		= 0.02f ;
	m_fEndTime			= fLimitTime ;

	SetRect(&m_stBGAnimation[0].rcSrc		,   1, 281, 494, 315) ;
	SetRect(&m_stBGAnimation[0].rcDstStart	, 146, 119, 666, 156) ;
	SetRect(&m_stBGAnimation[0].rcDstEnd	, 148, 119, 655, 155) ;
	g_pkPvpManager->RectMinus(m_stBGAnimation[0].rcDstEnd, m_stBGAnimation[0].rcDstStart, m_stBGAnimation[0].rcDstDiff) ;

	SetRect(&m_stBGAnimation[1].rcSrc		, 255,  1, 511, 181) ;
	SetRect(&m_stBGAnimation[1].rcDstStart	, 152, 124, 662, 485) ;
	SetRect(&m_stBGAnimation[1].rcDstEnd	, 157, 128, 653, 479) ;
	g_pkPvpManager->RectMinus(m_stBGAnimation[1].rcDstEnd, m_stBGAnimation[1].rcDstStart, m_stBGAnimation[1].rcDstDiff) ;

	SetRect(&m_stBGAnimation[2].rcSrc		, 1, 317, 497, 349) ;
	SetRect(&m_stBGAnimation[2].rcDstStart	, 141, 458, 660, 496) ;
	SetRect(&m_stBGAnimation[2].rcDstEnd	, 145, 457, 656, 490) ;
	g_pkPvpManager->RectMinus(m_stBGAnimation[2].rcDstEnd, m_stBGAnimation[2].rcDstStart, m_stBGAnimation[2].rcDstDiff) ;
}


void SPWindowPvpResult::SetFadeAnimation(float fLimitTime)
{
	m_eShowMode = PVP_RESULT_MODE_FADE ;

	m_fAccmulateTime	= 0 ;
	m_fElapsedTime		= 0 ;
	m_fDelayTime		= 0.02f ;
	m_iAlpha			= 0 ;
	m_fEndTime			= fLimitTime ;
}


void SPWindowPvpResult::SetEndAnimation()
{
	m_eShowMode = PVP_RESULT_MODE_END ;
	SPWindow::Show() ;
}


void SPWindowPvpResult::ReposOwnImage()
{
	SetRect(&m_rtDestBlur,	m_iAX +  12,	m_iAY +  23,	m_iAX + 501,	m_iAY + 345);

	SetRect(&m_rtDestBG[0],	m_iAX + 126,	m_iAY +  26,	m_iAX + 386,	m_iAY + 60);	// Title
	SetRect(&m_rtDestBG[1],	m_iAX +  41,	m_iAY +  65,	m_iAX +  95,	m_iAY + 80);	// Rank
	SetRect(&m_rtDestBG[2],	m_iAX +  99,	m_iAY +  65,	m_iAX + 202,	m_iAY + 80);	// Name
	SetRect(&m_rtDestBG[3],	m_iAX + 206,	m_iAY +  65,	m_iAX + 264,	m_iAY + 80);	// Kill Count
	SetRect(&m_rtDestBG[4],	m_iAX + 268,	m_iAY +  65,	m_iAX + 334,	m_iAY + 80);	// Death Count
	SetRect(&m_rtDestBG[5],	m_iAX + 338,	m_iAY +  65,	m_iAX + 404,	m_iAY + 80);	// Win Rate
	SetRect(&m_rtDestBG[6],	m_iAX + 408,	m_iAY +  65,	m_iAX + 474,	m_iAY + 80);	// Pvp Point
	SetRect(&m_rtDestBG[7],	m_iAX +  33,	m_iAY +  84,	m_iAX + 482,	m_iAY + 85);	// Top Line
	SetRect(&m_rtDestBG[8],	m_iAX +  33,	m_iAY + 304,	m_iAX + 482,	m_iAY + 305);	// Bottom Line
}


void SPWindowPvpResult::Clean()
{
	SAFE_RELEASE(m_pTextureOBJ);
	SAFE_RELEASE(m_pTextureBG);
	SAFE_DELETE(m_pExitTimer) ;
	SPWindow::Clean();
}


void SPWindowPvpResult::Show(bool bWithChild /*= true*/)
{
	m_bShow = true;
	m_bIsExit = false ;
	m_pExitTimer->Start(1, PVP_RESULT_CLOSE_TIMER) ;		// Timer Setting at 1 minute

	// Animation -> Control Show
	SetUnrollAnimation(PVP_RESULT_UNROLL_TIME) ;
}


void SPWindowPvpResult::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}



/**
	Scroll이 펼쳐지는 Animation
*/
void SPWindowPvpResult::ProcessUnroll(float fTime)
{
	m_fAccmulateTime	+= fTime ;			// 누적 시간
	m_fElapsedTime		+= fTime ;			// 경과 시간
	if( m_fElapsedTime > m_fDelayTime )
	{
		m_fElapsedTime -= m_fDelayTime ;
		m_fEndTime -= m_fDelayTime ;
		float fMoveRate = m_fAccmulateTime / m_fEndTime ;
		// Process Time Over
		if( m_fAccmulateTime >= m_fEndTime )
		{
			SetRect(&m_stBGAnimation[0].rcDst, m_stBGAnimation[0].rcDstEnd.left, m_stBGAnimation[0].rcDstEnd.top, m_stBGAnimation[0].rcDstEnd.right, m_stBGAnimation[0].rcDstEnd.bottom) ;
			SetRect(&m_stBGAnimation[1].rcSrc, m_stBGAnimation[1].rcSrcEnd.left, m_stBGAnimation[1].rcSrcEnd.top, m_stBGAnimation[1].rcSrcEnd.right, m_stBGAnimation[1].rcSrcEnd.bottom) ;
			SetRect(&m_stBGAnimation[1].rcDst, m_stBGAnimation[1].rcDstEnd.left, m_stBGAnimation[1].rcDstEnd.top, m_stBGAnimation[1].rcDstEnd.right, m_stBGAnimation[1].rcDstEnd.bottom) ;
			SetRect(&m_stBGAnimation[2].rcDst, m_stBGAnimation[2].rcDstEnd.left, m_stBGAnimation[2].rcDstEnd.top, m_stBGAnimation[2].rcDstEnd.right, m_stBGAnimation[2].rcDstEnd.bottom) ;
			SetWaneAnimation(PVP_RESULT_WANE_TIME) ;
		}
		// Process
		else
		{
			m_stBGAnimation[0].rcDst.left	= m_stBGAnimation[0].rcDstStart.left	+ int(m_stBGAnimation[0].rcDstDiff.left * fMoveRate) ;
			m_stBGAnimation[0].rcDst.top	= m_stBGAnimation[0].rcDstStart.top		+ int(m_stBGAnimation[0].rcDstDiff.top * fMoveRate) ;
			m_stBGAnimation[0].rcDst.right	= m_stBGAnimation[0].rcDstStart.right	+ int(m_stBGAnimation[0].rcDstDiff.right * fMoveRate) ;
			m_stBGAnimation[0].rcDst.bottom = m_stBGAnimation[0].rcDstStart.bottom	+ int(m_stBGAnimation[0].rcDstDiff.bottom * fMoveRate) ;

			m_stBGAnimation[1].rcSrc.left	= m_stBGAnimation[1].rcSrcStart.left	+ int(m_stBGAnimation[1].rcSrcDiff.left * fMoveRate) ;
			m_stBGAnimation[1].rcSrc.top	= m_stBGAnimation[1].rcSrcStart.top		+ int(m_stBGAnimation[1].rcSrcDiff.top * fMoveRate) ;
			m_stBGAnimation[1].rcSrc.right	= m_stBGAnimation[1].rcSrcStart.right	+ int(m_stBGAnimation[1].rcSrcDiff.right * fMoveRate) ;
			m_stBGAnimation[1].rcSrc.bottom = m_stBGAnimation[1].rcSrcStart.bottom	+ int(m_stBGAnimation[1].rcSrcDiff.bottom * fMoveRate) ;

			m_stBGAnimation[1].rcDst.left	= m_stBGAnimation[1].rcDstStart.left	+ int(m_stBGAnimation[1].rcDstDiff.left * fMoveRate) ;
			m_stBGAnimation[1].rcDst.top	= m_stBGAnimation[1].rcDstStart.top		+ int(m_stBGAnimation[1].rcDstDiff.top * fMoveRate) ;
			m_stBGAnimation[1].rcDst.right	= m_stBGAnimation[1].rcDstStart.right	+ int(m_stBGAnimation[1].rcDstDiff.right * fMoveRate) ;
			m_stBGAnimation[1].rcDst.bottom = m_stBGAnimation[1].rcDstStart.bottom	+ int(m_stBGAnimation[1].rcDstDiff.bottom * fMoveRate) ;

			m_stBGAnimation[2].rcDst.left	= m_stBGAnimation[2].rcDstStart.left	+ int(m_stBGAnimation[2].rcDstDiff.left * fMoveRate) ;
			m_stBGAnimation[2].rcDst.top	= m_stBGAnimation[2].rcDstStart.top		+ int(m_stBGAnimation[2].rcDstDiff.top * fMoveRate) ;
			m_stBGAnimation[2].rcDst.right	= m_stBGAnimation[2].rcDstStart.right	+ int(m_stBGAnimation[2].rcDstDiff.right * fMoveRate) ;
			m_stBGAnimation[2].rcDst.bottom	= m_stBGAnimation[2].rcDstStart.bottom	+ int(m_stBGAnimation[2].rcDstDiff.bottom * fMoveRate) ;
		}
	}
}



/**
	모두 펼쳐진 이후 다시 줄어드는 Animation
*/
void SPWindowPvpResult::ProcessWane(float fTime)
{
	m_fAccmulateTime	+= fTime ;			// 누적 시간
	m_fElapsedTime		+= fTime ;			// 경과 시간
	if( m_fElapsedTime > m_fDelayTime )
	{
		m_fElapsedTime -= m_fDelayTime ;
		m_fEndTime -= m_fDelayTime ;
		float fMoveRate = m_fAccmulateTime / m_fEndTime ;
		// Process Time Over
		if( m_fAccmulateTime > m_fEndTime )
		{
			SetRect(&m_stBGAnimation[0].rcDst, m_stBGAnimation[0].rcDstEnd.left, m_stBGAnimation[0].rcDstEnd.top, m_stBGAnimation[0].rcDstEnd.right, m_stBGAnimation[0].rcDstEnd.bottom) ;
			SetRect(&m_stBGAnimation[1].rcDst, m_stBGAnimation[1].rcDstEnd.left, m_stBGAnimation[1].rcDstEnd.top, m_stBGAnimation[1].rcDstEnd.right, m_stBGAnimation[1].rcDstEnd.bottom) ;
			SetRect(&m_stBGAnimation[2].rcDst, m_stBGAnimation[2].rcDstEnd.left, m_stBGAnimation[2].rcDstEnd.top, m_stBGAnimation[2].rcDstEnd.right, m_stBGAnimation[2].rcDstEnd.bottom) ;
			SetFadeAnimation(0.25f) ;
		}
		// Process
		else
		{
			m_stBGAnimation[0].rcDst.left	= m_stBGAnimation[0].rcDstStart.left	+ int(m_stBGAnimation[0].rcDstDiff.left * fMoveRate) ;
			m_stBGAnimation[0].rcDst.top	= m_stBGAnimation[0].rcDstStart.top		+ int(m_stBGAnimation[0].rcDstDiff.top * fMoveRate) ;
			m_stBGAnimation[0].rcDst.right	= m_stBGAnimation[0].rcDstStart.right	+ int(m_stBGAnimation[0].rcDstDiff.right * fMoveRate) ;
			m_stBGAnimation[0].rcDst.bottom = m_stBGAnimation[0].rcDstStart.bottom	+ int(m_stBGAnimation[0].rcDstDiff.bottom * fMoveRate) ;

			m_stBGAnimation[1].rcDst.left	= m_stBGAnimation[1].rcDstStart.left	+ int(m_stBGAnimation[1].rcDstDiff.left * fMoveRate) ;
			m_stBGAnimation[1].rcDst.top	= m_stBGAnimation[1].rcDstStart.top		+ int(m_stBGAnimation[1].rcDstDiff.top * fMoveRate) ;
			m_stBGAnimation[1].rcDst.right	= m_stBGAnimation[1].rcDstStart.right	+ int(m_stBGAnimation[1].rcDstDiff.right * fMoveRate) ;
			m_stBGAnimation[1].rcDst.bottom = m_stBGAnimation[1].rcDstStart.bottom	+ int(m_stBGAnimation[1].rcDstDiff.bottom * fMoveRate) ;

			m_stBGAnimation[2].rcDst.left	= m_stBGAnimation[2].rcDstStart.left	+ int(m_stBGAnimation[2].rcDstDiff.left * fMoveRate) ;
			m_stBGAnimation[2].rcDst.top	= m_stBGAnimation[2].rcDstStart.top		+ int(m_stBGAnimation[2].rcDstDiff.top * fMoveRate) ;
			m_stBGAnimation[2].rcDst.right	= m_stBGAnimation[2].rcDstStart.right	+ int(m_stBGAnimation[2].rcDstDiff.right * fMoveRate) ;
			m_stBGAnimation[2].rcDst.bottom	= m_stBGAnimation[2].rcDstStart.bottom	+ int(m_stBGAnimation[2].rcDstDiff.bottom * fMoveRate) ;
		}
	}
}


void SPWindowPvpResult::ProcessFade(float fTime)
{
	m_fElapsedTime += fTime;
	if(m_fElapsedTime > m_fDelayTime ) 
	{
		m_fElapsedTime -= m_fDelayTime;
		m_iAlpha += 10 ;
		if(m_iAlpha >= 250 ) {
			m_iAlpha = 255 ;
			SetEndAnimation() ;
		}
	}
}


void SPWindowPvpResult::Process(float fTime)
{
	if(!m_bShow)			return;

	if( m_pExitTimer->CheckTimer(fTime) )	
	{
		// Timer is Expired then Exit PVP Indun
		if( m_pExitTimer->IsExpired() && !m_bIsExit )
		{
			OnExitPvp(0, 0) ;
			m_bIsExit = true ;
		}
	}
	
	if( m_eShowMode == PVP_RESULT_MODE_ROLL )
		ProcessUnroll(fTime) ;
	else if( m_eShowMode == PVP_RESULT_MODE_WANE )
		ProcessWane(fTime) ;
	else if( m_eShowMode == PVP_RESULT_MODE_FADE )
		ProcessFade(fTime) ;

	SPWindow::Process(fTime);
}


void SPWindowPvpResult::Render(float fTime)
{
	if(!m_bShow)		return;

	if( m_pTextureBG )	{
		m_pTextureBG->RenderTexture(&m_rtDestFade, &m_rtSrcFade);
	}

	if( m_eShowMode == PVP_RESULT_MODE_ROLL || m_eShowMode == PVP_RESULT_MODE_WANE )
	{
		if( m_pTextureBG )	{
			m_pTextureBG->RenderTexture(&m_stBGAnimation[1].rcDst, &m_stBGAnimation[1].rcSrc);
			m_pTextureBG->RenderTexture(&m_stBGAnimation[0].rcDst, &m_stBGAnimation[0].rcSrc);
			m_pTextureBG->RenderTexture(&m_stBGAnimation[2].rcDst, &m_stBGAnimation[2].rcSrc);
		}
	}
	if( m_eShowMode == PVP_RESULT_MODE_FADE || m_eShowMode == PVP_RESULT_MODE_END )
	{
		if( m_pTextureBG )	{
			m_pTextureBG->RenderTexture(&m_stBGAnimation[1].rcDst, &m_stBGAnimation[1].rcSrc);
			m_pTextureBG->RenderTexture(&m_stBGAnimation[0].rcDst, &m_stBGAnimation[0].rcSrc);
			m_pTextureBG->RenderTexture(&m_stBGAnimation[2].rcDst, &m_stBGAnimation[2].rcSrc);

			m_pTextureBG->SetColor(RGBA(255, 255, 255, m_iAlpha));
			m_pTextureBG->RenderTexture(&m_rtDestBlur, &m_rtSrcBlur);
			m_pTextureBG->SetColor(RGBA(255, 255, 255, 255));
		}
		if(m_pTextureOBJ) {
			g_pVideo->Flush();
			m_pTextureOBJ->SetColor(RGBA(255, 255, 255, m_iAlpha));
			for(int i = 0; i < PVP_RESULT_BG; i++) {
				m_pTextureOBJ->RenderTexture(&m_rtDestBG[i], &m_rtSrcBG[i]);
			}
			m_pTextureOBJ->SetColor(RGBA(255, 255, 255, 255));
		}
	}

	SPWindow::Render(fTime);
}


/**
	Result 창이 나올때 Sound Effect
 */
void SPWindowPvpResult::ResultEffectSound()
{
	SPSoundArchive* pSoundArchive	=	NULL;
	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	pLocalPlayer->SPGOBPerformMessage( SPGM_GETSOUNDARCHIVE , (LPARAM)&pSoundArchive );

	if( pSoundArchive != NULL )
	{
		SOUND_TABLE_INFO* pInfo = SPSoundManager::GetInstance()->GetSoundTable( (unsigned int)PVP_RESULT_SOUND_EFFECT );

		if( pInfo != NULL )
		{
			std::string strFullName;
			if( !pInfo->strFileName[0].empty() )
			{
				strFullName = "DATA/SOUND/";
				strFullName += pInfo->strFileName[ 0 ];

				pSoundArchive->InsertItem( strFullName.c_str() , pInfo->fDelayTime[ 0 ] , pInfo->bLoop[ 0 ] );
			}
		}
	}
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowPvpResult )

SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP		, WIID_PVP_RESULT_EXIT	, OnExitPvp	)
SPIMESSAGE_COMMAND(SPIM_PVP_SET_RESULT							, OnSetResultData )


SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


/**
	PVP Exit

 */
int SPWindowPvpResult::OnExitPvp( WPARAM wParam, LPARAM lParam ) 
{
	g_pkPvpManager->SendPVPExit() ;
	g_pInterfaceManager->SetOnlyWindowView(WIID_PVP_RESULT, false, false) ;
	return 1;
}

/**
	Set Result DATA
	@param int iUserCount : 사용자수
	@param const char* pUserNameList : 사용자 ID 목록 (ID : ID :.....)
	// GU_ID			ObjectGUID;							// 게임오브젝트아이디
	// UINT32			iMemberCnt;							// 대기자수
	// PVPRANK			stPVPRank;							// 대기자의 PVP순위정보
*/
int SPWindowPvpResult::OnSetResultData( WPARAM wParam, LPARAM lParam )
{
	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 0;

	int	iMyRank = -1 ;
	GU_ID ObjectGUID;
	UINT32 iMemberCount = 0;
	int	iWinScore = 0 ;
	PVPRANK stPVPResult ;
	SPWindowStatic* pStaticName = NULL ;
	SPWindowStatic* pStaticReady = NULL ;
	std::vector<PVPRANK> vecResult ;
	vecResult.reserve(8) ;
	
	TCHAR szPlayerName[MAX_LEN_NAME] ;
	memset(szPlayerName, 0, MAX_LEN_NAME) ;
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szPlayerName);

	pkPacket->ExtractUINT64(&ObjectGUID);
	pkPacket->ExtractUINT32(&iMemberCount);

	ErrorFLogs->Error_Log("OnSetResultData Members[%d]", iMemberCount) ;
	if( iMemberCount > 0 )
	{
		for( int i = 0; i < iMemberCount; i ++)
		{
			pkPacket->ExtractStruct(&stPVPResult, sizeof(PVPRANK));
			if( stPVPResult.iRank < 0 || stPVPResult.iRank > PVP_MAX_ROOMUSER )
			{
				ErrorFLogs->Error_Log("PVPResult Member Rank ERROR [%d]", stPVPResult.iRank) ;
				return 0 ;
			}

			vecResult.push_back(stPVPResult) ;
		}
		sort( vecResult.begin(), vecResult.end(), PVPSortByRank) ;
		iWinScore = vecResult[0].iScore ;
		ErrorFLogs->Error_Log("PVPResult Winner Score [%s][%d]", vecResult[0].szCharName, iWinScore ) ;

		// After sort Ranking Check
		for( int i = 0; i < iMemberCount; i ++)
		{
			if( strcmp(vecResult[i].szCharName, szPlayerName) == 0 )
			{
				iMyRank = i ;
				ErrorFLogs->Error_Log("My PVP Result Members[%s][%d]", stPVPResult.szCharName, iMyRank) ;
				break ;
			}
		}
	}
	else
	{
		ErrorFLogs->Error_Log("Oh My God.. OnSetResultData Members[%d]", iMemberCount) ;
		ErrorFLogs->Error_Log("PVPResult Winner Score [%d]", iWinScore ) ;
	}
	iWinScore = iWinScore ? iWinScore : 1 ;

	// Setting Item
	SPWindowStatic* pWinRankIcon = NULL ;
	SPWindowStatic* pWinCharName = NULL ;
	SPWindowStatic* pWinKillCount = NULL ;
	SPWindowStatic* pWinKillIcon = NULL ;
	SPWindowStatic* pWinDeathCount = NULL ;
	SPWindowStatic* pWinWinRate = NULL ;
	SPWindowStatic* pWinPPoint = NULL ;
	SPWindowStatic* pMyArrow = NULL ;
	char szText[64] ;

	for( int i = 0; i < PVP_MAX_ROOMUSER; i ++)
	{
		pWinRankIcon	= (SPWindowStatic*)Find( WIID_PVP_RESULT_RANKICON + i, true ) ;
		pWinCharName	= (SPWindowStatic*)Find( WIID_PVP_RESULT_CHARNAME + i, true ) ;
		pWinKillCount	= (SPWindowStatic*)Find( WIID_PVP_RESULT_KILLCOUNT + i, true ) ;
		pWinKillIcon	= (SPWindowStatic*)Find( WIID_PVP_RESULT_KILLICON + i, true ) ;
		pWinDeathCount	= (SPWindowStatic*)Find( WIID_PVP_RESULT_DEATHCOUNT + i, true ) ;
		pWinWinRate		= (SPWindowStatic*)Find( WIID_PVP_RESULT_WINRATE + i, true ) ;
		pWinPPoint		= (SPWindowStatic*)Find( WIID_PVP_RESULT_PVPPOINT + i, true ) ;
		if( i < iMemberCount )
		{
			int iRank = vecResult[i].iRank -1;
			if( pWinRankIcon )	{
				int j = iRank ? iRank / 4 : 0 ;
				int k = iRank ? iRank % 4 : 0 ;
				pWinRankIcon->SPSendMessage(SPIM_SET_IMAGECLEAR) ;
				pWinRankIcon->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 427+k*21, 29+j*19) ;
				pWinRankIcon->SetShowEnable(true) ;
			}
			if( pWinCharName )	{
				pWinCharName->SetShowEnable(true) ;
				pWinCharName->SetWindowText(vecResult[i].szCharName) ;
			}
			if( pWinKillCount )	{
				sprintf(szText, "%d", vecResult[i].iKillCnt);
				pWinKillCount->SetShowEnable(true) ;
				pWinKillCount->SetWindowText(szText) ;
			}
			if( pWinKillIcon )	{
				pWinKillIcon->SetShowEnable(true) ;
			}
			if( pWinDeathCount )	{
				sprintf(szText, "%d", vecResult[i].iDeathCnt);
				pWinDeathCount->SetShowEnable(true) ;
				pWinDeathCount->SetWindowText(szText) ;
			}
			if( pWinWinRate )	{
				sprintf(szText, "%d%%", (int)(vecResult[i].iScore * 100 / iWinScore));
				pWinWinRate->SetShowEnable(true) ;
				pWinWinRate->SetWindowText(szText) ;
			}
			if( pWinPPoint )	{
				strcpy(szText, ConvertMoneyToString(vecResult[i].iPVPPoint)) ;
				pWinPPoint->SetShowEnable(true) ;
				pWinPPoint->SetWindowText(szText) ;
			}
		}
		else
		{
			if( pWinRankIcon )	{
				pWinRankIcon->SetShowEnable(false) ;
				pWinRankIcon->Hide() ;
			}
			if( pWinCharName )	{
				pWinCharName->SetShowEnable(false) ;
				pWinCharName->Hide() ;
			}
			if( pWinKillCount )	{
				pWinKillCount->SetShowEnable(false) ;
				pWinKillCount->Hide() ;
			}
			if( pWinKillIcon )	{
				pWinKillIcon->SetShowEnable(false) ;
				pWinKillIcon->Hide() ;
			}
			if( pWinDeathCount )	{
				pWinDeathCount->SetShowEnable(false) ;
				pWinDeathCount->Hide() ;
			}
			if( pWinWinRate )	{
				pWinWinRate->SetShowEnable(false) ;
				pWinWinRate->Hide() ;
			}
			if( pWinPPoint )	{
				pWinPPoint->SetShowEnable(false) ;
				pWinPPoint->Hide() ;
			}
		}
		// Show My Rank
		pWinRankIcon	= (SPWindowStatic*)Find( WIID_PVP_RESULT_MARK_ME, true ) ;
		pMyArrow		= (SPWindowStatic*)Find( WIID_PVP_RESULT_MARK_ARROW, true ) ;
		if( iMyRank >= 0 ) 		{
			pWinRankIcon->SetShowEnable(true) ;
			pWinRankIcon->AdjustCoord(33, 89+iMyRank*24) ;
			pMyArrow->SetShowEnable(true) ;
			pMyArrow->AdjustCoord(8, 6) ;		// Mark's Child
		}
		else	{
			pWinRankIcon->SetShowEnable(false) ;
			pWinRankIcon->Hide() ;
			pMyArrow->SetShowEnable(false) ;
			pMyArrow->Hide() ;
		}

	}

	// HotKey Block & Show Effect & Show Result Window
	g_pInterfaceManager->SetOnlyWindowView(WIID_PVP_RESULT, true, false) ;
	ResultEffectSound() ;
	
	return 1;
}