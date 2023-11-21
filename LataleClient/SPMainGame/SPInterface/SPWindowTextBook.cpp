// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowTextBook.cpp
//	created:	2007/9/21   Time:11:32
//	purpose:	
//*********************************************************************/

#include "SPCommon.h"

#include "SPUtil.h"
#include "SPWindowDEF.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include "SPTextBookManager.h"
#include "SPWindowTextBook.h"

#include "SPSubGameManager.h"
#include "SPZone.h"

using namespace std;

//------------------------------------------------------------------------------------
SPWindowTextBook::SPWindowTextBook(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_pFadeTexture(NULL)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowTextBook::SPWindowTextBook(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_TEXTBOOK, InstanceID, iX, iY, iCX, iCY, pParent)	
, m_pFadeTexture(NULL)
, m_pBookText(NULL)
, m_pIllust(NULL)
, m_ppCurAni(NULL)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowTextBook::~SPWindowTextBook()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowTextBook::Clean()
{
	SPWindow::Clean();
	ClearAnimationTexture();

	SAFE_RELEASE(m_pFadeTexture) ;
	SAFE_RELEASE(m_pIllust) ;
}

//------------------------------------------------------------------------------------
void SPWindowTextBook::Init()
{
	SPWindowButton* pkButton;
	SPWindowStatic* pkStatic;

	m_iBlackAlpha = MAX_FADE_ALPHA ;
	m_iTextAlpha = MAX_FADE_ALPHA ;
	m_fFadeTime = 0 ;
	m_iBookAniIndex = 0 ;

	// Fade Texture
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &m_pFadeTexture);
	SetRect(&m_rtSrcFade, 255, 21, 257, 23);
	SetRect(&m_rtDestFade, 0, 0, 800, 600);
	SetRect(&m_rtDestIllust, 283, 160, 539, 416) ; // (283+256) (160+256)

	// Book Text
	m_pBookText	=	new	SPWindowStatic( WIID_TEXTBOOK_TEXT , 166 , 108 , 490 , 360 , this );
	m_pBookText->SetFont(FONT_18_BOLD) ;
	m_pBookText->SetFormat(DT_TOP | DT_LEFT | DT_WORDBREAK);
	m_pBookText->SetFontColor( RGBA(75, 45, 25, 255) ) ;
	m_pBookText->SetShowEnable(false) ;
	m_pBookText->Hide() ;

	pkButton = new SPWindowButton(WIID_TEXTBOOK_EXIT, 639, 23, 156, 32, this);
	pkButton->SetImage		 ("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 315, 1);
	pkButton->SetImageHit	 ("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 315, 34);
	pkButton->SetImagePush	 ("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 315, 67);
	pkButton->SetImageDisable("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 315, 100);

	pkStatic	=	new	SPWindowStatic( WIID_TEXTBOOK_PRE_PAGE_BG , 155 , 479 , 197 , 74 , this );
	pkStatic->SetImage		 ("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 1, 181);
	pkButton = new SPWindowButton(WIID_TEXTBOOK_PRE_PAGE, 18, 19, 156, 32, pkStatic);
	pkButton->SetImage		 ("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 1, 1);
	pkButton->SetImageHit	 ("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 1, 34);
	pkButton->SetImagePush	 ("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 1, 67);
	pkButton->SetImageDisable("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 1, 100);
	pkStatic = NULL ;
	pkButton = NULL ;

	pkStatic	=	new	SPWindowStatic( WIID_TEXTBOOK_NEXT_PAGE_BG , 475 , 479 , 197 , 74 , this );
	pkStatic->SetImage		 ("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 1, 181);
	pkButton = new SPWindowButton(WIID_TEXTBOOK_NEXT_PAGE, 18, 19, 156, 32, pkStatic);
	pkButton->SetImage		 ("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 158, 1);
	pkButton->SetImageHit	 ("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 158, 34);
	pkButton->SetImagePush	 ("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 158, 67);
	pkButton->SetImageDisable("DATA/INTERFACE/STORYBOOK/SB_UI001.PNG", 158, 100);
	pkStatic = NULL ;
	pkButton = NULL ;
}


//------------------------------------------------------------------------------------
// Page Animation Loading 
// SetFactor시에 실시간으로 로딩한다.
// Exit 될때 Release 한다.
void SPWindowTextBook::LoadPageAnimation()
{
	TCHAR szFileName[256] ;
	SPTexture* pkTexture = NULL;
	
	for( int i = 0 ; i < MAX_PAGEANIMAION ; i++)
	{
		_stprintf(szFileName, "DATA/INTERFACE/STORYBOOK/BOOK_ANI_%02d.PNG", i) ;
		if( !g_pVideo->GetTextureMgr()->LoadTexture(szFileName, &pkTexture) )
			pkTexture = NULL;

		m_vTexture.push_back(pkTexture);
	}
}



//------------------------------------------------------------------------------------
void SPWindowTextBook::ClearAnimationTexture()
{
	vector< SPTexture* >::iterator iter = m_vTexture.begin();
	while( iter != m_vTexture.end() )
	{
		SAFE_RELEASE(*iter);
		++iter;
	}
	m_vTexture.clear();
	m_ppCurAni = NULL ;
}


//------------------------------------------------------------------------------------
void SPWindowTextBook::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);
	ButtonHide() ;

	//if( m_pkCurStoryFactor )			// Set BGM
	//{
	//	if( g_pSubGameManager )
	//		g_pSubGameManager->SPPerformMessage(SPGM_SUG_SETBGM, (WPARAM)m_pkCurStoryFactor->strBGM.c_str(), 0);
	//}
}

//------------------------------------------------------------------------------------
void SPWindowTextBook::Process(float fTime)
{
	if( !m_bShow )
		return;

	ProcessFade(fTime) ;

	SPWindow::Process(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowTextBook::ProcessFade(float fTime)
{
	if(m_iViewState == TEXTBOOK_FADEIN ) {
		m_fFadeTime += fTime ;
		if( m_fFadeTime > 0.02f )	{
				m_fFadeTime = 0 ;
			if(m_iBlackAlpha > 0) {
				m_iBlackAlpha -= 10;
			}
			if(m_iBlackAlpha < 1) {
				m_iBlackAlpha = 0;
				// 최초 실행시 셋팅되므로 필요 없음
				SetViewState(TEXTBOOK_TEXT_IN) ;
			}
		}
	}
	else if( m_iViewState == TEXTBOOK_TEXT_IN ) {
		m_fFadeTime += fTime ;
		if( m_fFadeTime > 0.015f )	{
			m_fFadeTime = 0 ;
			if(m_iTextAlpha < MAX_FADE_ALPHA) {
				m_iTextAlpha += 10;
			}
			if(m_iTextAlpha >= MAX_FADE_ALPHA) {
				m_iTextAlpha = MAX_FADE_ALPHA;
				SetViewState(TEXTBOOK_TEXT_READ) ;
				ButtonShow() ;
			}
		}
	}
	else if( m_iViewState == TEXTBOOK_TEXT_OUT ) {
		m_fFadeTime += fTime ;
		if( m_fFadeTime > 0.015f )	{
			m_fFadeTime = 0 ;
			if(m_iTextAlpha > 0) {
				m_iTextAlpha -= 20;
			}
			if(m_iTextAlpha < 1) {
				m_iTextAlpha = 0;
				// Next String Read
				ReadTextStory(m_iReadIncrement) ;
				SetViewState(m_iDestState) ;
			}
		}
	}
	else if( m_iViewState == TEXTBOOK_PAGENEXT ) {
		m_fFadeTime += fTime ;
		if( m_fFadeTime > 0.05f )	{
			m_fFadeTime = 0 ;
			++m_iBookAniIndex ;
			// Texture Page Next
			if( (int)m_vTexture.size() > m_iBookAniIndex )
				m_ppCurAni = &m_vTexture[m_iBookAniIndex] ;
			else	{
				m_iBookAniIndex = 0 ;
				SetViewState(TEXTBOOK_TEXT_IN) ;
			}
		}
	}
	else if( m_iViewState == TEXTBOOK_PAGEPRE ) {
		m_fFadeTime += fTime ;
		if( m_fFadeTime > 0.05f )	{
			m_fFadeTime = 0 ;
			--m_iBookAniIndex ;
			// Texture Page Next
			if( m_iBookAniIndex >= 0 )	{
				m_ppCurAni = &m_vTexture[m_iBookAniIndex] ;
			}
			else	{
				m_iBookAniIndex = 0 ;
				SetViewState(TEXTBOOK_TEXT_IN) ;
			}
		}
	}
	else if( m_iViewState == TEXTBOOK_FADEOUT ) {
		m_fFadeTime += fTime ;
		if( m_fFadeTime > 0.015f )	{
			m_fFadeTime = 0 ;
			if(m_iBlackAlpha < MAX_FADE_ALPHA) {
				m_iBlackAlpha += 10;
			}
			if(m_iBlackAlpha >= MAX_FADE_ALPHA) {
				m_iBlackAlpha = MAX_FADE_ALPHA;
				
				ClearAnimationTexture() ;
				g_pSubGameManager->SPPerformMessage(SPGM_SET_BOOK_VIEW, 0, 0);
			}
		}
	}
}

//------------------------------------------------------------------------------------
void SPWindowTextBook::ImageRender()
{
	RECT rcDst, rcSrc;
	SetRect(&rcDst, 0, 0, 800, 600);
	SetRect(&rcSrc, 0, 0, 512, 512);

	if( m_ppCurAni && *m_ppCurAni )
	{
		(*m_ppCurAni)->RenderTexture(&rcDst, &rcSrc);
	}
	g_pVideo->Flush();
}


//------------------------------------------------------------------------------------
void SPWindowTextBook::BookTextRender(float fTime)
{
	m_pBookText->SetShowEnable(true) ;
	m_pBookText->Show() ;

	// Illustration
	if( m_pIllust ) {
		m_pIllust->SetColor(RGBA(255, 255, 255, m_iTextAlpha));
		m_pIllust->RenderTexture(&m_rtDestIllust, &m_rtSrcIllust);
		m_pIllust->SetColor(RGBA(255, 255, 255, 255));		
	}

	m_pBookText->SetFontColor(RGBA(75, 45, 25, m_iTextAlpha)) ;
	m_pBookText->RenderText() ;
	g_pVideo->Flush();

#ifdef _DEBUG
	RECT rcText ;
//	TCHAR szLog[64] ;
	m_pBookText->GetWindowRect(rcText) ;
	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));	// Text
	g_pVideo->GetSysLine()->DrawRect(rcText);
	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif

	m_pBookText->SetShowEnable(false) ;
	m_pBookText->Hide() ;
}

//------------------------------------------------------------------------------------
void SPWindowTextBook::Render(float fTime)
{
	if( !m_bShow )
		return;
	
	ImageRender();

	SPWindow::Render(fTime);

	BookTextRender(fTime) ;

	// Black Fade
	if(m_pFadeTexture && m_iBlackAlpha > 0) {
		m_pFadeTexture->SetColor(RGBA(255, 255, 255, m_iBlackAlpha));
		m_pFadeTexture->RenderTexture(&m_rtDestFade, &m_rtSrcFade);
		m_pFadeTexture->SetColor(RGBA(255, 255, 255, 255));		
	}
}

//------------------------------------------------------------------------------------
// Message
void SPWindowTextBook::SetViewState(TEXTBOOK_STATE iVeiwState)
{
	m_iViewState = iVeiwState ;
	m_fFadeTime = 0 ;

	if( iVeiwState == TEXTBOOK_PAGENEXT )
		m_iBookAniIndex = 0 ;
	else if( iVeiwState == TEXTBOOK_PAGEPRE )
		m_iBookAniIndex = MAX_PAGEANIMAION ;
}



//------------------------------------------------------------------------------------
// All Button Hide for Page Animation
void SPWindowTextBook::ButtonHide()
{
	SPWindow* pWindow = NULL ;
	pWindow = Find(WIID_TEXTBOOK_PRE_PAGE_BG) ;		// PreView Button
	if( pWindow )			pWindow->Hide() ;

	pWindow = Find(WIID_TEXTBOOK_NEXT_PAGE_BG ) ;		// NextView Button
	if( pWindow )			pWindow->Hide() ;

	pWindow = Find(WIID_TEXTBOOK_EXIT ) ;			// Exit Button
	if( pWindow )			pWindow->Hide() ;
}


//------------------------------------------------------------------------------------
// Show Button by m_iCurIndex & m_iMaxIdex
void SPWindowTextBook::ButtonShow()
{
	SPWindow* pWindow = NULL ;
	pWindow = Find(WIID_TEXTBOOK_PRE_PAGE_BG) ;		// PreView Button
	if( pWindow )
	{
		if( m_iPreRead < 1 ) 
			pWindow->Hide() ;
		else
			pWindow->Show() ;
	}

	pWindow = Find(WIID_TEXTBOOK_NEXT_PAGE_BG) ;		// NextView Button
	if( pWindow )
	{
		if( m_iNextRead < 1 ) 
			pWindow->Hide() ;
		else
			pWindow->Show() ;
	}

	pWindow = Find(WIID_TEXTBOOK_EXIT) ;			// Exit Button
	if( pWindow )			
		pWindow->Show() ;
}

//------------------------------------------------------------------------------------
void SPWindowTextBook::ReadTextStory(int iIncIndex)
{
	SPWindow* pWindow = NULL ;
	TEXTBOOK_FACTOR* pTextBook = NULL ;
	int iChangeIndex = 0 ;

	if( iIncIndex == 1 )		// NextPage
	{
		pTextBook = g_pkBookManger->GetStoryFactor(m_iNextRead) ;
		iChangeIndex = m_iNextRead ;
	}
	else if( iIncIndex == -1 )	// PrePage
	{
		pTextBook = g_pkBookManger->GetStoryFactor(m_iPreRead) ;
		iChangeIndex = m_iPreRead ;
	}
	else						// Current Page
	{
		pTextBook = g_pkBookManger->GetStoryFactor(m_iCurRead) ;
		iChangeIndex = m_iCurRead ;
	}

	if( pTextBook == NULL )
		return ;

	SAFE_RELEASE(m_pIllust) ;

	// Index Setting
	m_iCurRead = iChangeIndex ;
	m_iPreRead = pTextBook->iPrePage ;
	m_iNextRead = pTextBook->iNextPage ;

	// BookText Setting
	m_pBookText->SetWindowText(pTextBook->strText.c_str()) ;

	// Illustration Setting
	if( !pTextBook->strImage.empty() )
	{
		int iSrcX = 0;
		int iSrcY = 0;
		if( !g_pVideo->GetTextureMgr()->LoadTexture(pTextBook->strImage.c_str(), &m_pIllust) )
			m_pIllust = NULL;
		g_pkBookManger->GetImageSrc(pTextBook->iImageIndex, iSrcX, iSrcY) ;
		SetRect(&m_rtSrcIllust, iSrcX, iSrcY, iSrcX+256, iSrcY+256) ;
	}

	// BGM Setting
	if( !pTextBook->strBGM.empty() )
		g_pSubGameManager->SPPerformMessage(SPGM_SUG_SETBGM, (WPARAM)pTextBook->strBGM.c_str(), 0);
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowTextBook)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_TEXTBOOK_EXIT,			OnExit )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_TEXTBOOK_PRE_PAGE,		OnPrePage )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_TEXTBOOK_NEXT_PAGE,	OnNextPage )

SPIMESSAGE_COMMAND( SPIM_STORY_SET_FACTOR,			OnSetFactor )
SPIMESSAGE_COMMAND( SPIM_STORY_FORCE_EXIT,			OnExit )

SPIMESSAGE_OBJ_MAP_END(SPWindow)



//------------------------------------------------------------------------------------
// Message
int SPWindowTextBook::OnExit(WPARAM wParam, LPARAM lParam)
{
	m_iDestState = TEXTBOOK_FADEOUT ;
	SetViewState(TEXTBOOK_TEXT_OUT) ;

	m_iCurRead		= 0 ;
	m_iPreRead		= 0 ;
	m_iNextRead		= 0 ;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowTextBook::OnNextPage(WPARAM wParam, LPARAM lParam)
{
	if( m_iNextRead > 0 ) {
		ButtonHide() ;
		m_iDestState = TEXTBOOK_PAGENEXT ;
		m_iReadIncrement = 1 ;
		SetViewState(TEXTBOOK_TEXT_OUT) ;
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowTextBook::OnPrePage(WPARAM wParam, LPARAM lParam)
{
	if( m_iPreRead > 0 ) {
		ButtonHide() ;
		m_iDestState = TEXTBOOK_PAGEPRE ;
		m_iReadIncrement = -1 ;
		SetViewState(TEXTBOOK_TEXT_OUT) ;
	}
	return 1;
}

//------------------------------------------------------------------------------------
// 시작점 -> Text Load 이후 Animation Start
int SPWindowTextBook::OnSetFactor(WPARAM wParam, LPARAM lParam)
{
	ClearAnimationTexture();

	if( (int)lParam < 1 )
		return 0 ;
	
	m_iCurRead = (int)lParam ;
	ReadTextStory(0) ;
	LoadPageAnimation() ;
	m_ppCurAni = &m_vTexture[m_iBookAniIndex] ;

	m_iDestState = TEXTBOOK_TEXT_READ ;
	SetViewState(TEXTBOOK_FADEIN) ;

	return 1;
}