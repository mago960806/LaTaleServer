// ***************************************************************
//  SPWindowLoginIDC   version:  1.0   ·  date: 05/09/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowSlider.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowChat.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPWindowLoginIDC.h"

//////////////////////////////////////////////////////////////////////////

extern char g_szLoginServerIP[];
extern int g_iLoginServerPort;

//////////////////////////////////////////////////////////////////////////

SPWindowLoginIDC::SPWindowLoginIDC(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowLoginIDC::SPWindowLoginIDC(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_LOGINIDC, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowLoginIDC::~SPWindowLoginIDC()
{
	Clean();
}

void SPWindowLoginIDC::Init()
{
	return;

	m_strToolTip		=	"[SPWindowLoginIDC]";

	m_iAX	+=	166;
	m_iAY	+=	67;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/LOGGIN/LOGIN005.PNG" , &m_pBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[ LOGIN_IDC_BG_LEFT_TOP ]		, 478 , 89 , 486 , 97 );
	SetRect( &m_rcBGSrc[ LOGIN_IDC_BG_CENTER_TOP ]		, 489 , 89 , 493 , 97 );
	SetRect( &m_rcBGSrc[ LOGIN_IDC_BG_RIGHT_TOP ]		, 496 , 89 , 504 , 97 );

	SetRect( &m_rcBGSrc[ LOGIN_IDC_BG_LEFT_MIDDLE ]		, 478 , 100 , 486 , 104 );
	SetRect( &m_rcBGSrc[ LOGIN_IDC_BG_CENTER_MIDDLE ]	, 489 , 100 , 493 , 104 );
	SetRect( &m_rcBGSrc[ LOGIN_IDC_BG_RIGHT_MIDDLE ]	, 496 , 100 , 504 , 104 );

	SetRect( &m_rcBGSrc[ LOGIN_IDC_BG_LEFT_BOTTOM ]		, 478 , 107 , 486 , 115 );
	SetRect( &m_rcBGSrc[ LOGIN_IDC_BG_CENTER_BOTTOM ]	, 489 , 107 , 493 , 115 );
	SetRect( &m_rcBGSrc[ LOGIN_IDC_BG_RIGHT_BOTTOM ]	, 496 , 107 , 504 , 115 );

	SetRect( &m_rcBGSrc[ LOGIN_IDC_BG_LINE1 ]			, 501 , 2 , 503 , 3 );
	SetRect( &m_rcBGSrc[ LOGIN_IDC_BG_SUB_WHITE ]		, 14 , 128 , 17 , 131 );
	SetRect( &m_rcBGSrc[ LOGIN_IDC_BG_IDC_TITLE ]		, 0 , 464 , 130 , 484 );

	//////////////////////////////////////////////////////////////////////////

	SPWindowButton*	pWindowButton;

	pWindowButton	=	new	SPWindowButton( WIID_LOGINIDC_CLOSE_ICON , 445 , 10 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 217 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 230 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 243 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 256 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_LOGINIDC_CLOSE , 385 , 429 , 63 , 24 , this );
	pWindowButton->SetImage			( "DATA/LOGGIN/LOGIN005.PNG" , 414 , 101 );
	pWindowButton->SetImageHit		( "DATA/LOGGIN/LOGIN005.PNG" , 414 , 126 );
	pWindowButton->SetImagePush		( "DATA/LOGGIN/LOGIN005.PNG" , 414 , 151 );
	pWindowButton->SetImageDisable	( "DATA/LOGGIN/LOGIN005.PNG" , 414 , 176 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_LOGINIDC_OK , 314 , 429 , 63 , 24 , this );
	pWindowButton->SetImage			( "DATA/LOGGIN/LOGIN005.PNG" , 414 , 1 );
	pWindowButton->SetImageHit		( "DATA/LOGGIN/LOGIN005.PNG" , 414 , 26 );
	pWindowButton->SetImagePush		( "DATA/LOGGIN/LOGIN005.PNG" , 414 , 51 );
	pWindowButton->SetImageDisable	( "DATA/LOGGIN/LOGIN005.PNG" , 414 , 76 );
	pWindowButton->SetEnable( false );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_LOGINIDC_SLIDER_UP , 436 , 44 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_LOGINIDC_SLIDER_DOWN , 436 , 410 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );
	pWindowButton =	NULL;

	SPWindow*	pWindowLoading	=	new SPWindowStatic( WIID_LOGINIDC_LOADING , 131 , 214 , 190 , 30 , this );
	pWindowLoading->SetImage( "DATA/LOGGIN/LOGIN005.PNG" , 321 , 247 );
	pWindowLoading->SetImage( "DATA/LOGGIN/LOGIN005.PNG" , 321 , 278 );
	pWindowLoading->SetImage( "DATA/LOGGIN/LOGIN005.PNG" , 321 , 309 );
	pWindowLoading->SetImage( "DATA/LOGGIN/LOGIN005.PNG" , 321 , 340 );

	m_iLoadingFrame	=	0;

	RECT rtRect;
	m_pSlider	=	new	SPWindowSlider( WIID_LOGINIDC_SLIDER , 436 , 55 , 12 , 355 , this );

	SetRect( &rtRect , 460 , 470 , 472 , 494 );	//	일반
	m_pSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 473 , 470 , 485 , 494 );	//	마우스 오버
	m_pSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 486 , 470 , 498 , 494 );	//	마우스 클릭
	m_pSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 499 , 470 , 511 , 494 );
	m_pSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 460 , 496 , 472 , 498 );	//	일반
	m_pSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 473 , 496 , 485 , 498 );	//	마우스 오버
	m_pSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 486 , 496 , 498 , 498 );	//	마우스 클릭
	m_pSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 499 , 496 , 511 , 498 );
	m_pSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	m_iCurPage	=	0;

	m_pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pSlider->SetCurpage( m_iCurPage );
	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );
	m_pSlider->Refresh();

	m_bInitAutoShow	=	false;
	m_eSelectID		=	0;
	m_pSelectIDC	=	NULL;

	InitChildZone();

	InitLDTLoad();

	RefreshPage();

	m_iCurPingZonePos		=	0;
	m_iCurPingIDCPos		=	0;
	m_iCurPingIPCPos		=	0;
	m_bPingComplete			=	false;

	m_fPingLimitTime		=	1.0f;
	m_fPingAccmulateTime	=	0.0f;

	m_fLoadingLimitTime		=	0.5f;
	m_fLoadingAccmulateTime	=	0.0f;

	m_bPingInit				=	false;

	if( g_szLoginServerIP != NULL && strlen( g_szLoginServerIP ) > 0 && g_iLoginServerPort > 0 )
		m_bInitAutoShow	=	true;

	LastLoginServer();

	OnOk( NULL , NULL );
}

void SPWindowLoginIDC::ReposOwnImage()
{
	SetRect( &m_rcBGDest[ LOGIN_IDC_BG_LEFT_TOP ]		, m_iAX					, m_iAY					, m_iAX + 8			, m_iAY + 8 );
	SetRect( &m_rcBGDest[ LOGIN_IDC_BG_CENTER_TOP ]		, m_iAX + 8				, m_iAY					, m_iAX + 8 + 452	, m_iAY + 8 );
	SetRect( &m_rcBGDest[ LOGIN_IDC_BG_RIGHT_TOP ]		, m_iAX + 460			, m_iAY					, m_iAX + 460 + 8	, m_iAY + 8 );

	SetRect( &m_rcBGDest[ LOGIN_IDC_BG_LEFT_MIDDLE ]	, m_iAX					, m_iAY + 8				, m_iAX + 8			, m_iAY + 8 + 449 );
	SetRect( &m_rcBGDest[ LOGIN_IDC_BG_CENTER_MIDDLE ]	, m_iAX + 8				, m_iAY	+ 8				, m_iAX + 8 + 452	, m_iAY + 8 + 449 );
	SetRect( &m_rcBGDest[ LOGIN_IDC_BG_RIGHT_MIDDLE ]	, m_iAX + 460			, m_iAY + 8				, m_iAX + 460 + 8	, m_iAY + 8 + 449 );

	SetRect( &m_rcBGDest[ LOGIN_IDC_BG_LEFT_BOTTOM ]	, m_iAX					, m_iAY + 457			, m_iAX + 8			, m_iAY + 457 + 8 );
	SetRect( &m_rcBGDest[ LOGIN_IDC_BG_CENTER_BOTTOM ]	, m_iAX + 8				, m_iAY	+ 457			, m_iAX + 8 + 452	, m_iAY + 457 + 8 );
	SetRect( &m_rcBGDest[ LOGIN_IDC_BG_RIGHT_BOTTOM ]	, m_iAX + 460			, m_iAY + 457			, m_iAX + 460 + 8	, m_iAY + 457 + 8 );

	SetRect( &m_rcBGDest[ LOGIN_IDC_BG_LINE1 ]			, m_iAX + 4				, m_iAY	+ 27			, m_iAX + 4 + 460	, m_iAY + 27 + 1 );
	SetRect( &m_rcBGDest[ LOGIN_IDC_BG_SUB_WHITE ]		, m_iAX + 4				, m_iAY	+ 28			, m_iAX + 4 + 460	, m_iAY + 28 + 429 );
	SetRect( &m_rcBGDest[ LOGIN_IDC_BG_IDC_TITLE ]		, m_iAX + 12			, m_iAY + 6 			, m_iAX + 12 + 130	, m_iAY + 6 + 20 );
}

void SPWindowLoginIDC::Clean()
{
	return;

	int	iCount = LOGIN_IDC_ZONE_COUNT * LOGIN_IDC_ZONE_SUB_COUNT;
	SPWindow*	pWindow;
	SPTexture*	pTexture	=	NULL;

	for( int i = 0 ; i < iCount ; ++i )
	{
		pWindow	=	Find( WIID_LOGINIDC_SUB_IDC_BG + i , true );

		if( pWindow != NULL )
			pWindow->SetImage( pTexture );
	}

	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );

	DeleteLoginZoneIDC();

	m_PingManager.Exit();
}

void SPWindowLoginIDC::Show(bool bWithChild)
{
	return;

	if( m_bInitAutoShow == true )
		return;

	m_bInitAutoShow	=	true;

	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Show(bWithChild);

	UpdateIDC();
}

void SPWindowLoginIDC::Hide(bool bSendServer)
{
	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide(bSendServer);
}

void SPWindowLoginIDC::Process(float fTime)
{
	return;

	if( m_bShow == FALSE ) return;

	SPWindow::Process( fTime );

	if( m_bPingComplete == false )
	{
		m_fPingAccmulateTime	+=	fTime;

		if( m_fPingAccmulateTime > m_fPingLimitTime )
		{
			m_fPingAccmulateTime	=	0.0f;
			ProcessPing();

			if( m_bPingComplete == true )
				UpdateIDC();
		}

		m_fLoadingAccmulateTime	+=	fTime;

		if( m_fLoadingAccmulateTime > m_fLoadingLimitTime )
		{
			m_fLoadingAccmulateTime	=	0.0f;

			++m_iLoadingFrame;

			if( m_iLoadingFrame >= LOGIN_IDC_LOADING_IMAGE_COUNT )
				m_iLoadingFrame = 0;

			SPWindow*	pWindowLoading	=	Find( WIID_LOGINIDC_LOADING );

			if( pWindowLoading != NULL )
				pWindowLoading->SPSendMessage( SPIM_SET_IMAGEINDEX , m_iLoadingFrame );
		}
	}
}

void SPWindowLoginIDC::Render(float fTime)
{
	return;

	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < LOGIN_IDC_BG_COUNT ; ++i )
		{
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	SPWindow::Render(fTime);
}

void SPWindowLoginIDC::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowLoginIDC )
	SPIMESSAGE_COMMAND( SPIM_LOGIN_IDC_SHOW		,										OnIDCShow			)
	SPIMESSAGE_COMMAND( SPIM_WHEEL_UP			,										OnSliderUp			)
	SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN			,										OnSliderDown		)
	SPIMESSAGE_COMMAND( SPIM_KEY_ESCAPE			,										OnEscape			)
	SPIMESSAGE_COMMAND( SPIM_KEY_RETURN			,										OnKeyReturn			)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_LOGINIDC_CLOSE_ICON		,	OnClose				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_LOGINIDC_CLOSE				,	OnClose				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_LOGINIDC_OK				,	OnOk				)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_LOGINIDC_SLIDER_UP			,	OnSliderUp			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_LOGINIDC_SLIDER_DOWN		,	OnSliderDown		)

	SPIMESSAGE_CONTROL( SPIM_SLIDER_UP			,	WIID_LOGINIDC_SLIDER			,	OnSlider			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN		,	WIID_LOGINIDC_SLIDER			,	OnSlider			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP		,	WIID_LOGINIDC_SLIDER			,	OnSlider			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN	,	WIID_LOGINIDC_SLIDER			,	OnSlider			)
	SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE		,	WIID_LOGINIDC_SLIDER			,	OnSlider			)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	,	WIID_LOGINIDC_SUB_IDC_BG		,	WIID_LOGINIDC_SUB_IDC_BG + LOGIN_IDC_ZONE_COUNT * LOGIN_IDC_ZONE_SUB_COUNT	,	OnIDCSelect			)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DBLCLICK	,	WIID_LOGINIDC_SUB_IDC_BG	,	WIID_LOGINIDC_SUB_IDC_BG + LOGIN_IDC_ZONE_COUNT * LOGIN_IDC_ZONE_SUB_COUNT	,	OnLButtonDblClick	)

	SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP		,	WIID_LOGINIDC_SLIDER			,	WIID_LOGINIDC_SUB_IDC_BG_END	,	OnSliderUp		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN	,	WIID_LOGINIDC_SLIDER			,	WIID_LOGINIDC_SUB_IDC_BG_END	,	OnSliderDown	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowLoginIDC::OnIDCShow( WPARAM wParam, LPARAM lParam )
{
	m_bInitAutoShow	=	false;
	Show();
	return 1;
}

int SPWindowLoginIDC::OnEscape( WPARAM wParam, LPARAM lParam )
{	
	if( IsShow() )
	{
		SPWindow*	pWindowClose	=	Find( WIID_LOGINIDC_CLOSE );

		if( pWindowClose != NULL )
		{
			if( pWindowClose->IsShow() && pWindowClose->IsEnable() )
			{
				OnClose( wParam , lParam );
				return 1;
			}
		}
	}

	return -1;	
}

int SPWindowLoginIDC::OnKeyReturn( WPARAM wParam, LPARAM lParam )
{	
	if( IsShow() )
	{
		SPWindow*	pWindowOK	=	Find( WIID_LOGINIDC_OK );

		if( pWindowOK != NULL )
		{
			if( pWindowOK->IsShow() && pWindowOK->IsEnable() )
			{
				OnOk( wParam , lParam );
				return 1;
			}
		}

	}
	return -1;	
}

int SPWindowLoginIDC::OnClose( WPARAM wParam, LPARAM lParam )
{
	if( m_pSelectIDC != NULL )
		m_pSelectIDC->m_bSelect	=	false;

	LastLoginServer();

	Hide();
	return 1;
}

int SPWindowLoginIDC::OnOk( WPARAM wParam, LPARAM lParam )
{
	if( m_pSelectIDC != NULL )
	{
		int	iLoginServerIPCount	=	(int)m_pSelectIDC->m_strLoginServerIPList.size();
		int	iAdjustValue		=	10000;

		int iRandomNumber		=	GetRandom( iLoginServerIPCount * iAdjustValue - 1 ) / iAdjustValue + 1;

		if( iRandomNumber >= (int)iLoginServerIPCount )
			iRandomNumber = (int)iLoginServerIPCount - 1;

		std::string	strLoginServerIP	=	m_pSelectIDC->m_strLoginServerIPList.at( iRandomNumber );
		size_t	colunPos = strLoginServerIP.find( ":" );

		if( colunPos == std::string::npos )
		{
			strncpy( g_szLoginServerIP , strLoginServerIP.c_str() , MAX_PATH - 1 );
			g_iLoginServerPort = 10000;
		}
		else
		{
			std::string	strParsingIP	=	strLoginServerIP.substr( 0 , colunPos );
			std::string	strParsingPort	=	strLoginServerIP.substr( colunPos + 1 , strLoginServerIP.length() - ( colunPos + 1 ) );

			strncpy( g_szLoginServerIP , strParsingIP.c_str() , MAX_PATH - 1 );
			g_iLoginServerPort	=	atoi( strParsingPort.c_str() );
		}

		if( g_pClientOption != NULL )
			g_pClientOption->SaveLastLoginServer( m_pSelectIDC->m_strLabel );
	}

	Hide();
	return 1;
}

int SPWindowLoginIDC::OnSliderUp( WPARAM wParam, LPARAM lParam )
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 1;
}

int SPWindowLoginIDC::OnSliderDown( WPARAM wParam, LPARAM lParam )
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 1;
}

int SPWindowLoginIDC::OnSliderUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnSliderUp( wParam , lParam );
}

int SPWindowLoginIDC::OnSliderDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnSliderDown( wParam , lParam );
}

int SPWindowLoginIDC::OnSlider( WPARAM wParam, LPARAM lParam )
{
	int	iNewPage	=	(int)wParam;

	m_iCurPage	=	iNewPage;

	UpdateIDC();

	return 1;
}

int SPWindowLoginIDC::OnIDCSelect( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	int iChildZoneSubPos	=	(int)m_eSelectID - (int)WIID_LOGINIDC_SUB_IDC_BG;
	int iChildZonePos		=	iChildZoneSubPos / LOGIN_IDC_ZONE_SUB_COUNT;

	SPWindow*	pWindowSubPannel	=	Find( WIID_LOGINIDC_SUB_PANNEL + iChildZonePos );
	SPWindow*	pWindowSubIDCText	=	NULL;
	SPWindow*	pWindowSubIDCPing	=	NULL;

	if( pWindowSubPannel != NULL )
	{
		pWindowSubIDCText	=	pWindowSubPannel->Find( WIID_LOGINIDC_SUB_IDC_TEXT + iChildZoneSubPos );;
		pWindowSubIDCPing	=	pWindowSubPannel->Find( WIID_LOGINIDC_SUB_IDC_PING + iChildZoneSubPos );
	}

	SPWindow*	pWindowSubIDCBGOld	=	Find( m_eSelectID , true );

	SetTextEnable( pWindowSubIDCText , true );
	SetTextEnable( pWindowSubIDCPing , true );
	SetBGButtonEnable( pWindowSubIDCBGOld , true );

	m_eSelectID	=	iID;

	SPWindow*	pWindow	=	Find( m_eSelectID , true );

	SetBGButtonEnable( pWindow , false );

	iChildZoneSubPos	=	(int)m_eSelectID - (int)WIID_LOGINIDC_SUB_IDC_BG;
	iChildZonePos		=	iChildZoneSubPos / LOGIN_IDC_ZONE_SUB_COUNT;

	pWindowSubPannel	=	Find( WIID_LOGINIDC_SUB_PANNEL + iChildZonePos );
	
	SPWindow*	pWindowSubTitle		=	NULL;

	if( pWindowSubPannel != NULL )
	{
		pWindowSubTitle		=	pWindowSubPannel->Find( WIID_LOGINIDC_SUB_TITLE + iChildZonePos );
		pWindowSubIDCText	=	pWindowSubPannel->Find( WIID_LOGINIDC_SUB_IDC_TEXT + iChildZoneSubPos );
		pWindowSubIDCPing	=	pWindowSubPannel->Find( WIID_LOGINIDC_SUB_IDC_PING + iChildZoneSubPos );
	}

	if( pWindowSubTitle == NULL || pWindowSubIDCText == NULL || pWindowSubIDCPing == NULL )
		return 1;

	SetTextEnable( pWindowSubIDCText , false );
	SetTextEnable( pWindowSubIDCPing , false );

	if( m_pSelectIDC != NULL )
		m_pSelectIDC->m_bSelect	=	false;

	m_pSelectIDC	=	GetLoingIDC( pWindowSubTitle->GetWindowText() , pWindowSubIDCText->GetWindowText() );

	if( m_pSelectIDC != NULL )
		m_pSelectIDC->m_bSelect	=	true;

	CheckButton();

	return 1;
}

int SPWindowLoginIDC::OnLButtonDblClick( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnIDCSelect( iID , wParam , lParam );
	OnOk( wParam , lParam );
	return 1;
}

void SPWindowLoginIDC::InitLDTLoad()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/IDC.LDT" , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [ DATA/LDT/IDC.LDT ] \n" );
#endif
		return;
	}

	LDT_Field ldtFieldGroupName;
	LDT_Field ldtFieldIDCName;
	LDT_Field ldtFieldIP;
	LDT_Field ldtFieldLabel;

	int		iRecordCount	= pLDTFile->GetItemCount();
	int		iItemID;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetField( iItemID, 0,	ldtFieldGroupName );		//	_Group_Name
		pLDTFile->GetField( iItemID, 1,	ldtFieldIDCName );			//	_IDC_Name
		pLDTFile->GetField( iItemID, 2,	ldtFieldIP );				//	_IP
		pLDTFile->GetField( iItemID, 3,	ldtFieldLabel );			//	_Label

		AddZoneIDC( ldtFieldGroupName.uData.pValue , ldtFieldIDCName.uData.pValue , ldtFieldIP.uData.pValue , ldtFieldLabel.uData.pValue );
	}

	SAFE_RELEASE(pLDTFile);
}

void SPWindowLoginIDC::InitChildZone()
{
	SPWindow*	pWindowSubPannel	=	NULL;
	SPWindow*	pWindowSubTitleBG	=	NULL;
	SPWindow*	pWindowSubTitle		=	NULL;
	SPWindow*	pWindowSubIDCIcon	=	NULL;
	SPWindow*	pWindowSubIDCText	=	NULL;
	SPWindow*	pWindowSubIDCPing	=	NULL;

	int			iChildZonePos		=	0;
	int			iChildZoneSubPos	=	0;

	SPWindowButton*	pWindowButton;

	for( int y = 0 ; y < LOGIN_IDC_ZONE_Y_MAX ; ++y )
	{
		for( int x = 0 ; x < LOGIN_IDC_ZONE_X_MAX ; ++x )
		{
			iChildZonePos	=	( x + y * LOGIN_IDC_ZONE_X_MAX );

			pWindowSubPannel	=	new SPWindowStatic( WIID_LOGINIDC_SUB_PANNEL + iChildZonePos , 20 + x * 208 , 44 + y * 127 , 192 , 123 , this );

			pWindowSubTitleBG	=	new SPWindowStatic( WIID_LOGINIDC_SUB_TITLE_BG + iChildZonePos , 0 , 0 , 192 , 15 , pWindowSubPannel );
			pWindowSubTitleBG->SetImage( "DATA/LOGGIN/LOGIN005.PNG" , 0 , 448 );

			pWindowSubTitle		=	new SPWindowStatic( WIID_LOGINIDC_SUB_TITLE + iChildZonePos , 4 , 2 , 184 , 12 , pWindowSubPannel );
			pWindowSubTitle->SetFont( FONT_12_BOLD );
			pWindowSubTitle->SetFontColor( RGBA( 255 , 255 , 255 , 255 ) );
			pWindowSubTitle->SetWindowText( "구역이름" );

			for( int i = 0 ; i < LOGIN_IDC_ZONE_SUB_COUNT ; ++i )
			{
				iChildZoneSubPos	=	iChildZonePos * LOGIN_IDC_ZONE_SUB_COUNT + i;

				pWindowButton	=	new	SPWindowButton( WIID_LOGINIDC_SUB_IDC_BG + iChildZoneSubPos , 0 , 22 + i * 25 , 192 , 23 , pWindowSubPannel );
//				pWindowButton->SetImage			( "DATA/LOGGIN/LOGIN005.PNG" , 131 , 485 );
				pWindowButton->SetImageHit		( "DATA/LOGGIN/LOGIN005.PNG" ,   0 , 485 );
				pWindowButton->SetImagePush		( "DATA/LOGGIN/LOGIN005.PNG" ,   0 , 485 );
//				pWindowButton->SetImageDisable	( "DATA/LOGGIN/LOGIN005.PNG" , 193 , 485 );

				pWindowSubIDCIcon	=	new SPWindowStatic( WIID_LOGINIDC_SUB_IDC_ICON + iChildZoneSubPos , 8 , 23 + i * 25 , 21 , 21 , pWindowSubPannel );
				pWindowSubIDCIcon->SetImage( "DATA/LOGGIN/LOGIN005.PNG" , 478 , 116 );
				pWindowSubIDCIcon->SetImage( "DATA/LOGGIN/LOGIN005.PNG" , 478 , 1 );
				pWindowSubIDCIcon->SetImage( "DATA/LOGGIN/LOGIN005.PNG" , 478 , 23 );
				pWindowSubIDCIcon->SetImage( "DATA/LOGGIN/LOGIN005.PNG" , 478 , 45 );
				pWindowSubIDCIcon->SetImage( "DATA/LOGGIN/LOGIN005.PNG" , 478 , 67 );

				pWindowSubIDCText	=	new SPWindowStatic( WIID_LOGINIDC_SUB_IDC_TEXT + iChildZoneSubPos , 34 , 28 + i * 25 , 108 , 12 , pWindowSubPannel );
				pWindowSubIDCText->SetWindowText( "IDC서버이름1" );

				pWindowSubIDCPing	=	new SPWindowStatic( WIID_LOGINIDC_SUB_IDC_PING + iChildZoneSubPos , 113 , 28 + i * 25 , 72 , 12 , pWindowSubPannel );
				pWindowSubIDCPing->SetWindowText( "[ 10ms ]" );
				pWindowSubIDCPing->SetFormat( DT_VCENTER | DT_RIGHT | DT_WORDBREAK );
			}
		}
	}
}

void SPWindowLoginIDC::AddZoneIDC( const char* pstrZone , const char* pstrIDC , const char* pstrLoginServerIP , const char* pstrLabel )
{
	if( pstrZone == NULL || pstrIDC == NULL || pstrLoginServerIP == NULL || pstrLabel == NULL )
		return;

	if( strlen( pstrZone ) <= 0 || strlen( pstrIDC ) <= 0 || strlen( pstrLoginServerIP ) <= 0 || strlen( pstrLabel ) <= 0 )
		return;

	LoginZone*	pLoginZone	=	GetLoginZone( pstrZone );

	if( pLoginZone == NULL )
	{
		pLoginZone	=	new LoginZone;

		if( pLoginZone != NULL )
		{
			pLoginZone->m_strZone	=	pstrZone;
			pLoginZone->m_strIDCList.clear();

			m_vLoginZone.push_back( pLoginZone );

			m_hmLoginZone.insert( STDEXT_HASH_MAP_LOGINZONE::value_type( pLoginZone->m_strZone , pLoginZone ) );
		}
	}

	if( pLoginZone == NULL )
		return;

	LoginIDC*	pLoginIDC	=	GetLoingIDC( pstrZone , pstrIDC );

	if( pLoginIDC == NULL )
	{
		pLoginIDC	=	new LoginIDC;

		if( pLoginIDC != NULL )
		{
			pLoginIDC->m_strZone	=	pstrZone;
			pLoginIDC->m_strIDC		=	pstrIDC;
			pLoginIDC->m_strLabel	=	pstrLabel;

			pLoginZone->m_strIDCList.push_back( pLoginIDC );

			std::string	strZoneIDC	=	pLoginIDC->m_strZone	+	pLoginIDC->m_strIDC;

			m_hmLoginIDC.insert( STDEXT_HASH_MAP_LOGINIDC::value_type( strZoneIDC , pLoginIDC ) );
			m_hmLoginLabel.insert( STDEXT_HASH_MAP_LOGINLABEL::value_type( pLoginIDC->m_strLabel , pLoginIDC ) );
		}
	}

	if( pLoginIDC == NULL )
		return;

	pLoginIDC->m_bSelect			=	false;
	pLoginIDC->m_iPing				=	-1;
	std::string	strLoginServerIP	=	pstrLoginServerIP;

	pLoginIDC->m_strLoginServerIPList.push_back( strLoginServerIP );
}

void SPWindowLoginIDC::RefreshPage()
{
	int	iMaxPage = CalPage( (int)m_vLoginZone.size() );

	if( m_pSlider != NULL )
		m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , iMaxPage );
}

LoginZone* SPWindowLoginIDC::GetLoginZone( const char* pstrZone )
{
	if( pstrZone == NULL )
		return NULL;

	if( strlen( pstrZone ) <= 0 )
		return NULL;

	std::string	strZone	=	pstrZone;

	STDEXT_HASH_MAP_LOGINZONE::iterator		hmIter	=	m_hmLoginZone.find( strZone );

	if( hmIter == m_hmLoginZone.end() )
		return NULL;

	return ((*hmIter).second);
}

LoginIDC* SPWindowLoginIDC::GetLoingIDC( const char* pstrZone , const char* pstrIDC )
{
	if( pstrZone == NULL || pstrIDC == NULL )
		return NULL;

	if( strlen( pstrZone ) <= 0 || strlen( pstrIDC ) <= 0 )
		return NULL;

	std::string	strZone		=	pstrZone;
	std::string	strIDC		=	pstrIDC;
	std::string	strZoneIDC	=	strZone	+	strIDC;

	STDEXT_HASH_MAP_LOGINIDC::iterator		hmIter	=	m_hmLoginIDC.find( strZoneIDC );

	if( hmIter == m_hmLoginIDC.end() )
		return NULL;

	return ((*hmIter).second);
}

void SPWindowLoginIDC::DeleteLoginZoneIDC()
{
	m_hmLoginZone.clear();
	m_hmLoginIDC.clear();
	m_hmLoginLabel.clear();

	LoginZone*	pLoginZone	=	NULL;

	STD_VECTOR_LOGINZONE::iterator	zoneIter	=	m_vLoginZone.begin();
	STD_VECTOR_LOGINIDC::iterator	idcIter;

	while( zoneIter != m_vLoginZone.end() )
	{
		pLoginZone	=	(*zoneIter);

		idcIter	=	pLoginZone->m_strIDCList.begin();

		while( idcIter != pLoginZone->m_strIDCList.end() )
		{
			delete (*idcIter);
			++idcIter;
		}

		delete (*zoneIter);
		++zoneIter;
	}

	m_vLoginZone.clear();
}

void SPWindowLoginIDC::UpdateIDC()
{
	SPWindow*	pWindowSliderUp		=	Find( WIID_LOGINIDC_SLIDER_UP );
	SPWindow*	pWindowSliderDown	=	Find( WIID_LOGINIDC_SLIDER_DOWN );
	SPWindow*	pWindowOK			=	Find( WIID_LOGINIDC_OK );
	SPWindow*	pWindowLoading		=	Find( WIID_LOGINIDC_LOADING );
	SPWindow*	pWindowCloseIcon	=	Find( WIID_LOGINIDC_CLOSE_ICON );
	SPWindow*	pWindowClose		=	Find( WIID_LOGINIDC_CLOSE );

	if( m_bPingComplete == false )
	{
		for( int i = 0 ; i < LOGIN_IDC_ZONE_COUNT ; ++i )
		{
			UpdateIDC( i , NULL );
		}

		if( pWindowSliderUp != NULL )
			pWindowSliderUp->Hide();

		if( pWindowSliderDown != NULL )
			pWindowSliderDown->Hide();

		if( m_pSlider != NULL )
			m_pSlider->Hide();

		if( pWindowOK != NULL )
			pWindowOK->Hide();

		if( pWindowCloseIcon != NULL )
			pWindowCloseIcon->Hide();

		if( pWindowClose != NULL )
			pWindowClose->Hide();

		if( pWindowLoading != NULL )
			pWindowLoading->Show();

		return;
	}

	if( pWindowLoading != NULL )
		pWindowLoading->Hide();

	if( pWindowOK != NULL )
		pWindowOK->Show();

	if( pWindowCloseIcon != NULL )
		pWindowCloseIcon->Show();

	if( pWindowClose != NULL )
		pWindowClose->Show();

	int	iCurZonePos			=	0;
	int	iStartZonePos		=	m_iCurPage * LOGIN_IDC_ZONE_X_MAX;
	int	iLoginZoneCount		=	(int)m_vLoginZone.size();

	if( iLoginZoneCount > LOGIN_IDC_ZONE_COUNT )
	{
		if( pWindowSliderUp != NULL )
			pWindowSliderUp->Show();

		if( pWindowSliderDown != NULL )
			pWindowSliderDown->Show();

		if( m_pSlider != NULL )
		{
			m_pSlider->Refresh();

			if( g_pInterfaceManager!= NULL )
				m_pSlider->SPSendMessage( SPIM_CURSOR_MOVE , 0 , MAKELONG( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() ) );

			m_pSlider->Show();
		}
	}
	else
	{
		if( pWindowSliderUp != NULL )
			pWindowSliderUp->Hide();

		if( pWindowSliderDown != NULL )
			pWindowSliderDown->Hide();

		if( m_pSlider != NULL )
			m_pSlider->Hide();
	}

	if( iStartZonePos >= iLoginZoneCount || iStartZonePos < 0 )
		return;

	STD_VECTOR_LOGINZONE::iterator	Iter	=	m_vLoginZone.begin() + iStartZonePos;

	LoginZone*	pLoginZone	=	NULL;

	while( Iter != m_vLoginZone.end() )
	{
		pLoginZone	=	(*Iter);

		UpdateIDC( iCurZonePos , pLoginZone );
		
		++iCurZonePos;
		++Iter;
	}

	for( int i = iCurZonePos ; i < LOGIN_IDC_ZONE_COUNT ; ++i )
	{
		UpdateIDC( i , NULL );
	}

	CheckButton();

	g_pInterfaceManager->SetCursorWindow( NULL );
	g_pInterfaceManager->PerformMouseMove( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() );
}

void SPWindowLoginIDC::UpdateIDC( int iPos , LoginZone* pLoginIDCZone )
{
	SPWindow*	pWindowSubPannel	=	Find( WIID_LOGINIDC_SUB_PANNEL + iPos );

	if( pWindowSubPannel == NULL )
		return;

	pWindowSubPannel->Hide();

	if( pLoginIDCZone == NULL )
		return;

	pWindowSubPannel->Show( false );

	SPWindow*	pWindowSubTitleBG	=	pWindowSubPannel->Find( WIID_LOGINIDC_SUB_TITLE_BG + iPos );

	if( pWindowSubTitleBG != NULL )
		pWindowSubTitleBG->Show();

	SPWindow*	pWindowSubTitle		=	pWindowSubPannel->Find( WIID_LOGINIDC_SUB_TITLE + iPos );

	if( pWindowSubTitle != NULL )
	{
		pWindowSubTitle->SetWindowText( pLoginIDCZone->m_strZone.c_str() );
		pWindowSubTitle->Show();
	}

	SPWindow*	pWindowSubIDCIcon;
	SPWindow*	pWindowSubIDCText;
	SPWindow*	pWindowSubIDCPing;
	SPWindow*	pWindowSubIDCBG;

	STD_VECTOR_LOGINIDC::iterator	Iter	=	pLoginIDCZone->m_strIDCList.begin();
	int	iCurSubPos			=	0;
	int	iChildZoneSubPos	=	0;
	LoginIDC*	pLoginIDC	=	NULL;
	char	szBuf[_MAX_PATH];
	int		iIconType		=	0;

	while ( Iter != pLoginIDCZone->m_strIDCList.end() )
	{
		iChildZoneSubPos	=	iPos * LOGIN_IDC_ZONE_SUB_COUNT + iCurSubPos;

		pWindowSubIDCBG		=	pWindowSubPannel->Find( WIID_LOGINIDC_SUB_IDC_BG + iChildZoneSubPos );
		pWindowSubIDCIcon	=	pWindowSubPannel->Find( WIID_LOGINIDC_SUB_IDC_ICON + iChildZoneSubPos );
		pWindowSubIDCText	=	pWindowSubPannel->Find( WIID_LOGINIDC_SUB_IDC_TEXT + iChildZoneSubPos );
		pWindowSubIDCPing	=	pWindowSubPannel->Find( WIID_LOGINIDC_SUB_IDC_PING + iChildZoneSubPos );

		pLoginIDC	=	(*Iter);

		if( pLoginIDC != NULL )
		{
			if( pWindowSubIDCBG != NULL )
			{
				if( pLoginIDC->m_bSelect == true )
				{
					SetBGButtonEnable( pWindowSubPannel->Find( m_eSelectID ) , true );

					if( m_pSelectIDC != NULL )
					{
						if( m_pSelectIDC->m_strIDC.compare( pLoginIDC->m_strIDC ) != 0 && m_pSelectIDC->m_strZone.compare( pLoginIDC->m_strZone ) != 0 )
							m_pSelectIDC->m_bSelect	=	false;
					}

					m_eSelectID		=	pWindowSubIDCBG->GetWidnowInstanceID();
					m_pSelectIDC	=	pLoginIDC;
				}

				pWindowSubIDCBG->Show();

				SetBGButtonEnable( pWindowSubIDCBG , !pLoginIDC->m_bSelect );
			}

			if( pWindowSubIDCIcon != NULL )
			{
				if( pLoginIDC->m_iPing >= 90 )
					iIconType		=	4;
				else if( pLoginIDC->m_iPing < 90 && pLoginIDC->m_iPing >= 60 )
					iIconType		=	3;
				else if( pLoginIDC->m_iPing < 60 && pLoginIDC->m_iPing >= 30 )
					iIconType		=	2;
				else if( pLoginIDC->m_iPing < 30 && pLoginIDC->m_iPing >= 0 )
					iIconType		=	1;
				else
					iIconType		=	0;

				pWindowSubIDCIcon->SPSendMessage( SPIM_SET_IMAGEINDEX , iIconType );
				pWindowSubIDCIcon->Show();
			}

			if( pWindowSubIDCText != NULL )
			{
				pWindowSubIDCText->SetWindowText( pLoginIDC->m_strIDC.c_str() );
				pWindowSubIDCText->Show();

				SetTextEnable( pWindowSubIDCText , !pLoginIDC->m_bSelect );
			}

			if( pWindowSubIDCPing != NULL )
			{
				if( pLoginIDC->m_iPing > 0 )
				{
					wsprintf( szBuf , "[ ●%dms ]" , pLoginIDC->m_iPing );
					pWindowSubIDCPing->SetWindowText( szBuf );
				}
				else
				{
					pWindowSubIDCPing->SetWindowText( NULL );
				}

				pWindowSubIDCPing->Show();

				SetTextEnable( pWindowSubIDCPing , !pLoginIDC->m_bSelect );
			}
		}

		++iCurSubPos;
		++Iter;
	}
}

void SPWindowLoginIDC::ProcessPing()
{
	if( m_bPingInit == false )
	{
		m_PingManager.Init();
		m_bPingInit	=	true;
	}

	int	iLoginZoneCount	=	(int)m_vLoginZone.size();

	if( iLoginZoneCount <= 0 )
		return;

	if( m_iCurPingZonePos < 0 || m_iCurPingZonePos >= iLoginZoneCount )
		m_iCurPingZonePos = 0;

	LoginZone*	pLoginZone	=	m_vLoginZone.at( m_iCurPingZonePos );

	if( pLoginZone == NULL )
	{
		m_iCurPingZonePos	=	0;
		return;
	}

	int	iLoginIDCCount	=	(int)pLoginZone->m_strIDCList.size();

	if( m_iCurPingIDCPos < 0 || m_iCurPingIDCPos >= iLoginIDCCount )
		m_iCurPingIDCPos = 0;

	if( iLoginIDCCount <= 0 )
		return;

	int			iLoginIPCount	=	0;
	LoginIDC*	pLoginIDC		=	pLoginZone->m_strIDCList.at( m_iCurPingIDCPos );

	if( pLoginIDC != NULL )
	{
		iLoginIPCount	=	(int)pLoginIDC->m_strLoginServerIPList.size();

		if( m_iCurPingIPCPos < 0 || m_iCurPingIPCPos >= iLoginIPCount )
			m_iCurPingIPCPos = 0;

		if( iLoginIPCount <= 0 )
			return;

		std::string	strLoginIP	=	pLoginIDC->m_strLoginServerIPList.at( m_iCurPingIPCPos );

		//////////////////////////////////////////////////////////////////////////

		size_t	colunPos = strLoginIP.find( ":" );

		if( colunPos != std::string::npos )
		{
			std::string	strParsingIP	=	strLoginIP.substr( 0 , colunPos );
			strLoginIP	=	strParsingIP;
		}

		//////////////////////////////////////////////////////////////////////////

		int iPingTime	=	m_PingManager.Ping( (char*)strLoginIP.c_str() , 200 );

		if( iPingTime <= 0 )
			iPingTime	=	200;

		iPingTime	=	min( iPingTime , m_PingManager.Ping( (char*)strLoginIP.c_str() , 200 ) );

		if( iPingTime < 0 )
			iPingTime	=	200;
		else if( iPingTime == 0 )
			iPingTime = 1;

		if( pLoginIDC->m_iPing > 0 )
			pLoginIDC->m_iPing	=	( iPingTime + pLoginIDC->m_iPing ) / 2;
		else
			pLoginIDC->m_iPing	=	iPingTime;
	}

	++m_iCurPingIPCPos;

	if( m_iCurPingIPCPos >= iLoginIPCount )
	{
		++m_iCurPingIDCPos;
		m_iCurPingIPCPos	=	0;
	}

	if( m_iCurPingIDCPos >= iLoginIDCCount )
	{
		++m_iCurPingZonePos;
		m_iCurPingIDCPos	=	0;
	}

	if( m_iCurPingZonePos >= iLoginZoneCount )
	{
		m_iCurPingZonePos	=	0;
		m_iCurPingIDCPos	=	0;
		m_bPingComplete		=	true;
	}
}

void SPWindowLoginIDC::LastLoginServer()
{
	if( g_pClientOption == NULL )
		return;

	std::string	strLastLoginServer	=	"";

	g_pClientOption->LoadLastLoginServer( strLastLoginServer );

	if( strLastLoginServer.empty() == true )
		return;
    
	STDEXT_HASH_MAP_LOGINLABEL::iterator	hmIter	=	m_hmLoginLabel.find( strLastLoginServer );

	if( hmIter == m_hmLoginLabel.end() )
		m_pSelectIDC	=	NULL;
	else
		m_pSelectIDC	=	(*hmIter).second;

	CheckButton();

	if( m_pSelectIDC == NULL )
		return;

	STD_VECTOR_LOGINZONE::iterator	Iter	=	m_vLoginZone.begin();
	LoginZone*	pLoginZone;
	int	iLastPos	=	0;

	while ( Iter != m_vLoginZone.end() )
	{
		pLoginZone	=	(*Iter);

		if( pLoginZone != NULL )
		{
			if( m_pSelectIDC->m_strZone.compare( pLoginZone->m_strZone ) == 0 )
			{
				break;
			}
		}

		++iLastPos;
		++Iter;
	}

	m_pSelectIDC->m_bSelect	=	true;

	int	iMaxPage = CalPage( (int)m_vLoginZone.size() );
	int	iNewPage = CalPage( iLastPos );

	m_iCurPage = min( iMaxPage , iNewPage );

	if( m_pSlider != NULL )
	{
		m_pSlider->SetCurpage( m_iCurPage );
		m_pSlider->Refresh();
	}
}

void SPWindowLoginIDC::SetBGButtonEnable( SPWindow* pWindow , bool bEnable )
{
	SPWindowButton*	pWindowButton	=	(SPWindowButton*)pWindow;

	if( pWindowButton == NULL )
		return;

	if( bEnable == true )
	{
		SPTexture*	pTexture	=	NULL;

		pWindowButton->SetImage			( pTexture					 ,   0 ,   0 );
		pWindowButton->SetImageHit		( "DATA/LOGGIN/LOGIN005.PNG" ,   0 , 485 );
		pWindowButton->SetImagePush		( "DATA/LOGGIN/LOGIN005.PNG" ,   0 , 485 );
	}
	else
	{
		pWindowButton->SetImage			( m_pBGTexture , 193 , 485 );
		pWindowButton->SetImageHit		( "DATA/LOGGIN/LOGIN005.PNG" , 193 , 485 );
		pWindowButton->SetImagePush		( "DATA/LOGGIN/LOGIN005.PNG" , 193 , 485 );
	}

	pWindowButton->SetEnable( true );
}

void SPWindowLoginIDC::SetTextEnable( SPWindow* pWindow , bool bEnable )
{
	if( pWindow == NULL )
		return;

	D3DXCOLOR	cTextColor	=	D3DXCOLOR( 0.0f , 0.0f , 0.0f , 1.0f );

	if( bEnable == false )
		cTextColor	=	D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

	pWindow->SetFontColor( cTextColor );
}

int SPWindowLoginIDC::CalPage( int iItemCount )
{
	int	iCalPage	=	iItemCount - LOGIN_IDC_ZONE_X_MAX * ( LOGIN_IDC_ZONE_Y_MAX - 1 );

	if( iCalPage < LOGIN_IDC_ZONE_X_MAX * ( LOGIN_IDC_ZONE_Y_MAX - 1 ) )
	{
		iCalPage	=	0;
	}
	else
	{
		if( iCalPage % LOGIN_IDC_ZONE_X_MAX )
			iCalPage	=	iCalPage	/	LOGIN_IDC_ZONE_X_MAX	+	1;
		else
			iCalPage	=	iCalPage	/	LOGIN_IDC_ZONE_X_MAX;
	}

	if( iCalPage < 0 )
		iCalPage = 0;

	return iCalPage;
}

void SPWindowLoginIDC::CheckButton()
{
	SPWindow*	pWindowCloseIcon	=	Find( WIID_LOGINIDC_CLOSE_ICON );
	SPWindow*	pWindowClose		=	Find( WIID_LOGINIDC_CLOSE );
	SPWindow*	pWindowOK			=	Find( WIID_LOGINIDC_OK );

	bool	bCloseIcon	=	false;
	bool	bClose		=	false;
	bool	bOk			=	false;

	if( m_pSelectIDC != NULL )
	{
		std::string	strLastLoginServer	=	"";

		g_pClientOption->LoadLastLoginServer( strLastLoginServer );

		if( strLastLoginServer.empty() == false )
		{
			bCloseIcon		=	true;
			bClose			=	true;
		}

		bOk				=	true;
	}

	if( pWindowCloseIcon != NULL )
		pWindowCloseIcon->SetEnable( bCloseIcon );

	if( pWindowClose != NULL )
		pWindowClose->SetEnable( bClose );

	if( pWindowOK != NULL )
		pWindowOK->SetEnable( bOk );
}
