
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"

#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowEdit.h"
#include "SPWindowButton.h"
#include "SPWindowMenuBar.h"

#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"

#include "SPDebug.h"


SPWindowMenuBar::SPWindowMenuBar(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_iAlpha(255)
, m_fLastRenderDelay(0.0f)
, m_fAccumulateRender(0.0f)
, m_fCurDelay(0.0f)
{	
	Init();
}

SPWindowMenuBar::SPWindowMenuBar(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_MENUBAR, InstanceID, iX, iY, iCX, iCY, pParent)	
{	
	Init();
}

SPWindowMenuBar::~SPWindowMenuBar()
{
	Clean();
}

void SPWindowMenuBar::Init()
{	
	m_strToolTip = "[SPWindowMenuBar]";

	SPWindow*		pWindow			=	NULL;
	SPWindow*		pWindowSelect	=	NULL;
	SPWindowStatic*	pStatic			=	NULL;
	SPWindowButton*	pButton			=	NULL;
	int		iWindowCtrlYSize , iWindowYPos;
	RECT	rcWindowPos;

	//////////////////////////////////////////////////////////////////////////
	//
	//	제약조건
	//

	bool	bUsePet				=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_PET );
	bool	bUseGuild			=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_GUILD );
	bool	bUseChatMacro		=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_CHATMACRO );
	bool	bUseGuideBook		=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_GUIDEBOOK );
	bool	bUseChattingRoom	=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_CHATTINGROOM );
	bool	bUseLove			=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_LOVE );

	//////////////////////////////////////////////////////////////////////////
	//
	//	마우스 오버 백그라운드 자동 생성
	//

	for( int i = 0 ; i < MENU_BUTTON_MAX ; ++i )
	{
		pButton	=	new SPWindowButton( WIID_MENUBAR_SELECT + i , 0 , 0 , 140 , 16 , this );
		pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	234,	259);
		pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	234,	259);
	}

	//////////////////////////////////////////////////////////////////////////

	iWindowYPos			=	0;
	iWindowYPos			+=	6;		//	BG와 메뉴사이 공간

	if( SPLocalizeManager::GetInstance()->GetCountryType() == CT_CHINA )
	{
		//	RSCLIENT
		iWindowCtrlYSize	=	16;
		pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_RSCLIENT , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
		pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI105.PNG",	140,	183);
		pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI105.PNG",	140,	183);
		pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI105.PNG",	140,	183);
		pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI105.PNG",	140,	183);
		iWindowYPos	+=	iWindowCtrlYSize;
		iWindowYPos	+=	4;	//	간격

		// LINE2
		iWindowCtrlYSize	=	1;
		pWindow = new SPWindow( WNDID_CTRL_STATIC , WIID_MENUBAR_LINE1 , 6 - m_iAX , iWindowYPos - m_iAY , 147 , iWindowCtrlYSize , this );
		pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 158);
		iWindowYPos	+=	iWindowCtrlYSize;
		iWindowYPos	+=	4;	//	간격
	}

	//	ITEM
	iWindowCtrlYSize	=	16;
	pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_ITEM , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	285,	276);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	285,	276);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	285,	276);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	285,	276);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격

	//	EQUIP
	pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_EQUIP , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	285,	293);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	285,	293);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	285,	293);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	285,	293);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격

	//	STATUS
	pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_STATUS , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	285,	310);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	285,	310);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	285,	310);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	285,	310);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격

	//	SKILL
	pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_SKILL , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	327);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	327);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	327);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	327);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격

	//	QUEST
	pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_QUEST , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	344);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	344);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	344);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	344);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격

	//	GUILD
	if( bUseGuild )
	{
		pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_GUILD , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
		pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI105.PNG",	101,	95);
		pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI105.PNG",	101,	95);
		pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI105.PNG",	101,	95);
		pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI105.PNG",	101,	95);
		iWindowYPos	+=	iWindowCtrlYSize;
		iWindowYPos	+=	4;	//	간격
	}

	//	PARTY	
	pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_PARTY , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	361);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	361);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	361);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	361);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격

	//	FRIEND
	pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_FRIEND , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	378);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	378);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	378);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	205,	378);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격

	//	LOVE
	if( bUseLove )
	{
		pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_LOVE , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
		pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI_Object_003.PNG",	1,	478);
		pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI_Object_003.PNG",	1,	478);
		pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI_Object_003.PNG",	1,	478);
		pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI_Object_003.PNG",	1,	478);
		iWindowYPos	+=	iWindowCtrlYSize;
		iWindowYPos	+=	4;	//	간격
	}

	//	PET
	pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_PET , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	327);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	327);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	327);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	327);
	pButton->SetEnable( bUsePet );
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격

	//	GUIDEBOOK
	if( bUseGuideBook == true )
	{
		pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_GUIDEBOOK , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
		pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI107.PNG",	1,	37);
		pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI107.PNG",	1,	37);
		pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI107.PNG",	1,	37);
		pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI107.PNG",	1,	37);
		pButton->SetEnable( bUsePet );
		iWindowYPos	+=	iWindowCtrlYSize;
		iWindowYPos	+=	4;	//	간격
	}

	//	CHATTINGROOM
	if( bUseChattingRoom )
	{
		pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_CHATTINGROOM , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
		pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI_Object_003.PNG",	1,	495);
		pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI_Object_003.PNG",	1,	495);
		pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI_Object_003.PNG",	1,	495);
		pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI_Object_003.PNG",	1,	495);
		iWindowYPos	+=	iWindowCtrlYSize;
		iWindowYPos	+=	4;	//	간격
	}

	// LINE2
	iWindowCtrlYSize	=	1;
	pWindow = new SPWindow( WNDID_CTRL_STATIC , WIID_MENUBAR_LINE2 , 6 - m_iAX , iWindowYPos - m_iAY , 147 , iWindowCtrlYSize , this );
	pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 158);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격

	//	OPTION
	iWindowCtrlYSize	=	16;
	pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_OPTION , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	344);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	344);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	344);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	344);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격

	//	HELP
	pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_HELP , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	361);	
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	361);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	361);	
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	361);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격

	if( bUseChatMacro )
	{
		//	CHATMACRO
		iWindowCtrlYSize	=	16;
		pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_CHATMACRO , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
		pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI107.PNG",	199,	379);
		pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI107.PNG",	199,	379);
		pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI107.PNG",	199,	379);
		pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI107.PNG",	199,	379);
		iWindowYPos	+=	iWindowCtrlYSize;
		iWindowYPos	+=	4;	//	간격
	}

	//코밈꽉데팁귁
#ifdef _DEBUG
	pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_AUTOSET , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI112.PNG",	1,  361);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI112.PNG",	1,  361);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI112.PNG",	1,  361);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI112.PNG",	1,  361);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격
#endif

	// LINE3
	iWindowCtrlYSize	=	1;
	pWindow = new SPWindow( WNDID_CTRL_STATIC , WIID_MENUBAR_LINE3 , 6 - m_iAX , iWindowYPos - m_iAY , 147 , iWindowCtrlYSize , this );
	pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 158);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	4;	//	간격

	//	EXIT
	iWindowCtrlYSize	=	16;
	pButton = new SPWindowButton( WIID_MENUBAR_BUTTON + MENU_BUTTON_EXIT , 10 - m_iAX , iWindowYPos - m_iAY , 140 , iWindowCtrlYSize , this );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	378);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	378);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	378);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	346,	378);
	iWindowYPos	+=	iWindowCtrlYSize;
	iWindowYPos	+=	6;	//	간격

	//////////////////////////////////////////////////////////////////////////
	//
	//	사이즈 계산
	//

	m_iSY	=	iWindowYPos;
	m_iAY	=	m_iAY		-	m_iSY;

	int iPX,iPY;

	if( IsWrapWindow() )
		GetAbsCoord( iPX , iPY );
	else
		m_pParent->GetAbsCoord(iPX, iPY);

	m_iRX	=	m_iAX - iPX;
	m_iRY	=	m_iAY - iPY;

	//////////////////////////////////////////////////////////////////////////
	//
	//	마우스 오버 백그라운드 위치 보정
	//

	for( i = 0 ; i < MENU_BUTTON_MAX ; ++i )
	{
		pWindow			=	Find( WIID_MENUBAR_BUTTON + i );
		pWindowSelect	=	Find( WIID_MENUBAR_SELECT + i );

		if( pWindow == NULL )
		{
			CleanChild( pWindowSelect );
			continue;
		}

		pWindow->GetWindowRect( rcWindowPos );
		pWindow->AdjustCoord( rcWindowPos.left , rcWindowPos.top );
		pWindow->GetWindowRect( rcWindowPos );

		if( pWindowSelect != NULL )
			pWindowSelect->AdjustCoord( rcWindowPos.left - m_iAX - 4 , rcWindowPos.top - m_iAY );
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	라인 위치 보정
	//

	for( i = 0 ; i < LINE_COUNT ; ++i )
	{
		pWindow			=	Find( WIID_MENUBAR_LINE1 + i );

		if( pWindow == NULL )
			continue;

		pWindow->GetWindowRect( rcWindowPos );
		pWindow->AdjustCoord( rcWindowPos.left , rcWindowPos.top );
		pWindow->GetWindowRect( rcWindowPos );
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	배경
	//

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &m_pBGTexture );

	SetRect( &m_rcBGSrc[ MENUBAR_BG_TOP ]		, 1 , 176 , 161 , 180 );
	SetRect( &m_rcBGSrc[ MENUBAR_BG_MIDDLE ]	, 1 , 182 , 161 , 184 );

	SetRect( &m_rcBGDest[ MENUBAR_BG_TOP ]		, m_iAX	, m_iAY		, m_iAX + 160	, m_iAY + 4 );
	SetRect( &m_rcBGDest[ MENUBAR_BG_MIDDLE ]	, m_iAX , m_iAY + 4	, m_iAX + 160	, m_iAY + m_iSY );

	//////////////////////////////////////////////////////////////////////////

	m_fAccumulateRender = 0.0f;
	m_fCurDelay = 0.005f;
}

void SPWindowMenuBar::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
}


void SPWindowMenuBar::Process(float fTime)
{
	if(m_bShow == false)
		return;

	if(m_iAlpha < 255)
	{
		m_fAccumulateRender += fTime;
		if( m_fAccumulateRender > m_fCurDelay) {
			m_fAccumulateRender -= m_fCurDelay;
			FadeProcess(fTime);
		}
	}

	SPWindow::Process( fTime );
}

void SPWindowMenuBar::FadeProcess(float fTime)
{
	m_iAlpha += 15;
	if(m_iAlpha > 254)
		m_iAlpha = 255;

	DWORD m_RGBA	=	RGBA( 255 , 255 , 255 , m_iAlpha );
	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); ++Iter) {
		//(*Iter)->SPSendMessage( SPIM_SET_COLOR , (WPARAM)m_RGBA );
		(*Iter)->SetColor(m_RGBA);
		//(*Iter)->Process(fTime);
	}
}

void SPWindowMenuBar::Render(float fTime)
{
	if(m_bShow == false)
		return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < MENUBAR_BG_COUNT ; ++i )
		{
			m_pBGTexture->SetColor( m_RGBA );
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
			m_pBGTexture->SetColor();
		}

		g_pVideo->Flush();
	}

	SPWindow::Render( fTime );
}

void SPWindowMenuBar::Show(bool bWithChild /*= true*/)
{
	m_fAccumulateRender = 0.0f;	
	m_iAlpha = 70;

	SPWindow::Show(bWithChild);

	//SPWindow*	pWindowPet	=	Find( WIID_MENUBAR_BUTTON + MENU_BUTTON_PET );

	//if( pWindowPet != NULL )
	//	pWindowPet->SetEnable( SPLocalizeManager::GetInstance()->IsEnableValue( LV_PET ) );

	DWORD m_RGBA	=	RGBA( 255 , 255 , 255 , m_iAlpha );

	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); ++Iter)
	{
		//(*Iter)->SPSendMessage( SPIM_SET_COLOR , (WPARAM)m_RGBA );
		(*Iter)->SetColor(m_RGBA);
		//(*Iter)->Process(fTime);
	}
}


void SPWindowMenuBar::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowMenuBar )
	SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP,	WIID_MENUBAR_SELECT, WIID_MENUBAR_SELECT + MENU_BUTTON_MAX,	OnMenuClick)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowMenuBar::OnMenuClick(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	int iSelect = uiID - WIID_MENUBAR_SELECT;	
	//DXUTOutputDebugString("	* Click Item [%d] \n", iSelect);
	
	switch(iSelect) {
		case MENU_BUTTON_CHATMACRO:
			if( SPLocalizeManager::GetInstance()->IsEnableValue( LV_CHATMACRO ) )
				g_pInterfaceManager->ShowWindow(WIID_CHAT_MACRO, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_RSCLIENT:
			if( SPLocalizeManager::GetInstance()->GetCountryType() == CT_CHINA )
				g_pInterfaceManager->ShowWindow(WIID_RSCLIENT, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_ITEM:
			g_pInterfaceManager->ShowWindow(WIID_ITEM, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_EQUIP:
			g_pInterfaceManager->ShowWindow(WIID_EQUIP, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_STATUS:
			g_pInterfaceManager->ShowWindow(WIID_STATUS, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_SKILL:
			g_pInterfaceManager->ShowWindow(WIID_SKILL, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_QUEST:
			g_pInterfaceManager->ShowWindow(WIID_QUEST, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_PARTY:
			g_pInterfaceManager->ShowWindow(WIID_TEAM, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_GUILD:
			if( SPLocalizeManager::GetInstance()->IsEnableValue( LV_GUILD ) )
				g_pInterfaceManager->ShowWindow(WIID_GUILD_INFO, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_FRIEND:
			g_pInterfaceManager->ShowWindow(WIID_FRIEND, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_PET:
			if( SPLocalizeManager::GetInstance()->IsEnableValue( LV_PET ) )
				g_pInterfaceManager->ShowWindow(WIID_PET, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_GUIDEBOOK:
			if( SPLocalizeManager::GetInstance()->IsEnableValue( LV_GUIDEBOOK ) )
				g_pInterfaceManager->ShowWindow(WIID_GUIDE_BOOK, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_OPTION:
			g_pInterfaceManager->ShowWindow(WIID_OPTION, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_HELP:
			g_pInterfaceManager->ShowWindow(WIID_HELP, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_EXIT:
			g_pInterfaceManager->ShowWindow(WIID_EXIT, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_CHATTINGROOM:
			if( SPLocalizeManager::GetInstance()->IsEnableValue( LV_CHATTINGROOM ) )
				g_pInterfaceManager->ShowWindow( WIID_CHATTING_ROOM , SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_AUTOSET:
			g_pInterfaceManager->ShowWindow(WIID_AUTOSET, SP_NONE, SP_TOGGLE_ON);
			break;
		case MENU_BUTTON_LOVE:
			g_pInterfaceManager->ShowWindow( WIID_LOVE , SP_NONE, SP_TOGGLE_ON);
			break;
		default:
			break;
	}

	Hide();

	return 1;
}
