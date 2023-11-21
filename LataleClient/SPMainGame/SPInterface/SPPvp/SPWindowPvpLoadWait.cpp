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

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowPvpLoadWait.h"

#include "GlobalDefines_Share.h"
#include "SPPvpManager.h"

#include "_Error_log.h"


SPWindowPvpLoadWait::SPWindowPvpLoadWait(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_PVP_LOADWAIT, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pTextureBG(NULL)
, m_pTextureWnd(NULL)
, m_pTextureText(NULL)
{
	Init();
}


SPWindowPvpLoadWait::~SPWindowPvpLoadWait()
{
	Clean();
}


void SPWindowPvpLoadWait::Init()
{
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , &m_pTextureWnd);	
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , &m_pTextureText);	

	// WALL Background
	SetRect(&m_rtSrc1, 0, 0, 512, 512);
	SetRect(&m_rtSrc2, 512, 0, 800, 512);
	SetRect(&m_rtSrc3, 800, 0, 1024, 88);
	SetRect(&m_rtSrc4, 800, 88, 1024, 176);
	SetRect(&m_rtSrc5, 800, 176, 1024, 264);
	SetRect(&m_rtSrc6, 800, 264, 928, 352);

	SetRect(&m_rtDest1, 0, 0, 512, 512);
	SetRect(&m_rtDest2, 512, 0, 800, 512);
	SetRect(&m_rtDest3, 0, 512, 224, 600);
	SetRect(&m_rtDest4, 224, 512, 448, 600);
	SetRect(&m_rtDest5, 448, 512, 672, 600);
	SetRect(&m_rtDest6, 672, 512, 800, 600);

	// Background
	SetRect(&m_rtSrcBG,	1, 1, 254, 280);
	//SetRect(&m_rtDstBG,	m_iAX + 9, m_iAY + 312, m_iAX + 262, m_iAY + 591);	
	SetRect(&m_rtDstBG,	9, 312, 262, 591);	
	
	// Animation
	m_iAlphaInc		= 10;
	m_iAlpha		= 255;
	m_fAniRender	= 0.0f;
	m_fAniDelay		= 0.02f;
	SetRect(&m_rtAniSrc,		267, 47, 426, 60);
	//SetRect(&m_rtAniDest,		m_iAX + 75, m_iAY + 528, m_iAX + 234, m_iAY + 541);
	SetRect(&m_rtAniDest,		75, 528, 234, 541);

	InitSubControl();
}


void SPWindowPvpLoadWait::InitSubControl()
{
	//// Control
	SPWindowStatic* pStaticBG = NULL;
	SPWindowStatic* pStatic = NULL;

	// 마우스 클릭을 막기 위해서 배경을 넣음.
	// 실제 배경은 Texture 처리
	pStaticBG = new SPWindowStatic( WIID_PVP_LOADWAIT_BG  , 0,  0, 800, 600, this);
	pStaticBG->Hide();

	// Init Pos WIID_PVP_LOADWAIT, 9, 312, 253, 279
	//WIID_PVP_LOADWAIT_PLAYER
	for( int i = 0 ; i < PVP_MAX_ROOMUSER; i++)
	{
		pStatic = new SPWindowStatic( WIID_PVP_LOADWAIT_PLAYER+i  , 42+9,  48+i*19+312, 128, 12, pStaticBG);
		pStatic->SetFont(FONT_12_BOLD) ;
		pStatic->SetFormat(DT_TOP | DT_LEFT);
		pStatic->Hide();

		pStatic = new SPWindowStatic( WIID_PVP_LOADWAIT_READY+i  , 174+9,  54+i*19+312,  29,  5, pStaticBG);
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 397, 61) ;
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 367, 61) ;
		pStatic->Hide();
	}
}


void SPWindowPvpLoadWait::Clean()
{
	SAFE_RELEASE(m_pTextureWnd);
	SAFE_RELEASE(m_pTextureText);
	SAFE_RELEASE(m_pTextureBG) ;
	SPWindow::Clean();
}


void SPWindowPvpLoadWait::SetLoadBGTexture(const char* strFileName)
{
	char szFullPath[256] ;

	// Loading Background Image Clear & Reload
	SAFE_RELEASE(m_pTextureBG);

	if( strFileName == NULL || strlen(strFileName) < 2 )	
	{
		//ErrorFLogs->Error_Log("SPWindowPvpLoadWait::SetBGTexture FileName NULL") ;
		sprintf(szFullPath, "DATA/LOGGIN/LOADING.PNG" ) ;			// Set Default
	}
	else
	{
		sprintf(szFullPath, "%s", strFileName ) ;
	}
	g_pVideo->GetTextureMgr()->LoadTexture(szFullPath , &m_pTextureBG);	
}



void SPWindowPvpLoadWait::Show(bool bWithChild /*= true*/)
{
	m_iAlpha		= 255;
	m_fAniRender	= 0.0f;

	if( g_pkPvpManager->GetLoadBGFileName() != NULL )
		ErrorFLogs->Error_Log("PVPLoadWait Show BG[%s]", g_pkPvpManager->GetLoadBGFileName()) ;
	else
		ErrorFLogs->Error_Log("PVPLoadWait Show BG NULL!!!!") ;

	SetLoadBGTexture( g_pkPvpManager->GetLoadBGFileName() ) ;

	SPWindow::Show(bWithChild);
}


void SPWindowPvpLoadWait::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowPvpLoadWait::Process(float fTime)
{
	if(!m_bShow)			return;
	
	SPWindow::Process(fTime);

	m_fAniRender += fTime;
	if(m_fAniRender > m_fAniDelay) {
		m_fAniRender -= m_fAniDelay;

		m_iAlpha += m_iAlphaInc ;
		if(m_iAlpha >= 250 )
			m_iAlphaInc = -10 ;
		else if( m_iAlpha <= 10 )
			m_iAlphaInc = 10;
	}
}


void SPWindowPvpLoadWait::Render(float fTime)
{
	if(!m_bShow)		return;

	// Loading Background
	if( m_pTextureBG ) {
		g_pVideo->Flush();
		m_pTextureBG->RenderTexture(&m_rtDest1, &m_rtSrc1);
		m_pTextureBG->RenderTexture(&m_rtDest2, &m_rtSrc2);
		m_pTextureBG->RenderTexture(&m_rtDest3, &m_rtSrc3);
		m_pTextureBG->RenderTexture(&m_rtDest4, &m_rtSrc4);
		m_pTextureBG->RenderTexture(&m_rtDest5, &m_rtSrc5);
		m_pTextureBG->RenderTexture(&m_rtDest6, &m_rtSrc6);
	}

	// Window Background
	if(m_pTextureWnd) {
		g_pVideo->Flush();
		m_pTextureWnd->RenderTexture(&m_rtDstBG, &m_rtSrcBG);
	}

	// UseID & Ready
	SPWindow::Render(fTime);

	// Animation
	if( m_pTextureText )	{
		m_pTextureText->SetColor(RGBA(255, 255, 255, m_iAlpha));
		m_pTextureText->RenderTexture(&m_rtAniDest, &m_rtAniSrc);		// White
		m_pTextureText->SetColor(RGBA(255, 255, 255, 255));
	}
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowPvpLoadWait )

SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	, WIID_PVP_LOADWAIT_BG		, OnClose	)
SPIMESSAGE_COMMAND(SPIM_PVP_LOADWAIT_UPDATE						, OnUpdatePVPWait	)


SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowPvpLoadWait::OnClose( WPARAM wParam, LPARAM lParam ) 
{
	//Hide() ;
	g_pInterfaceManager->SetFocusWindow(this) ;
	return 1;
}


// GU_ID			ObjectGUID;							// 게임오브젝트아이디
// INT32			iMemberCnt;							// 대기자수
// PVPLOADSTATUS	stPVPLoadStatus;					// 대기자의 PVP로드상태
// ...
// PVPLOADSTATUS
//		UINT32		uiTeamID;					// 팀아이디(팀전인경우 팀아이디, 길드전인 경우 길드DB키, 0인 경우는 개인전)
//		char		szCharName[LEN_NAME+1];		// 캐릭명
//		UINT8		byStatus;					// (0:로드전, 1:로드완료)
int SPWindowPvpLoadWait::OnUpdatePVPWait( WPARAM wParam, LPARAM lParam )
{
	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 0;

	GU_ID ObjectGUID;
	UINT32 iMenberCount = 0;
	PVPLOADSTATUS stLoadStatus ;
	SPWindowStatic* pStaticName = NULL ;
	SPWindowStatic* pStaticReady = NULL ;

	pkPacket->ExtractUINT64(&ObjectGUID);
	pkPacket->ExtractUINT32(&iMenberCount);

	//ErrorFLogs->Error_Log("OnUpdatePVPWait Members[%d]", iMenberCount) ;

	for( int i = 0; i < PVP_MAX_ROOMUSER; i ++)
	{
		pStaticName = (SPWindowStatic*)Find(WIID_PVP_LOADWAIT_PLAYER+i, true) ;
		pStaticReady = (SPWindowStatic*)Find(WIID_PVP_LOADWAIT_READY+i, true) ;
		if( i < iMenberCount )
		{
			pkPacket->ExtractStruct(&stLoadStatus, sizeof(PVPLOADSTATUS));
			//ErrorFLogs->Error_Log("[%d]PVP Join Member[%d][%s]", i, stLoadStatus.byStatus, stLoadStatus.szCharName) ;
			int iReady = stLoadStatus.byStatus ;
			if( pStaticName ) {
				pStaticName->SetShowEnable(true) ;
				iReady ? pStaticName->SetFont(FONT_12_BOLD) : pStaticName->SetFont(FONT_12_NORMAL) ;
				iReady ? pStaticName->SetFontColor(PVP_LOADCOMPLETE_COLOR) : pStaticName->SetFontColor(PVP_LOADWAIT_COLOR) ;
				pStaticName->SetWindowText( stLoadStatus.szCharName ) ;
			}
			if( pStaticReady )	{
				pStaticReady->SetShowEnable(true) ;
				pStaticReady->SPSendMessage(SPIM_SET_IMAGEINDEX, iReady) ;
			}
		}
		else
		{
			if( pStaticName ) {
				pStaticName->SetShowEnable(false) ;
				pStaticName->Hide() ;
			}
			if( pStaticReady )	{
				pStaticReady->SetShowEnable(false) ;
				pStaticReady->Hide() ;
			}
		}
	}

	Show() ;

	return 1 ;
}
