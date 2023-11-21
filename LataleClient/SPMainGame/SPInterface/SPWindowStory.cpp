#include "SPCommon.h"

#include "SPUtil.h"
#include "SPWindowDEF.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include "SPStoryManager.h"
#include "SPWindowStory.h"

#include "SPSubGameManager.h"
#include "SPZone.h"

using namespace std;

//------------------------------------------------------------------------------------
SPWindowStory::SPWindowStory(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_pTexture(NULL)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowStory::SPWindowStory(INSTANCE_ID InstanceID,
	int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_STORYVIEW, InstanceID, iX, iY, iCX, iCY, pParent)	
, m_pTexture(NULL)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowStory::~SPWindowStory()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowStory::Clean()
{
	SPWindow::Clean();
	ClearTexture();
	SAFE_RELEASE(m_pTexture) ;
}

//------------------------------------------------------------------------------------
void SPWindowStory::Init()
{
	SPWindowStatic* pkWindow;
	SPWindowButton* pkButton;

	m_iAlpha = MAX_FADE_ALPHA ;
	m_fFadeTime = 0 ;

	std::string strTextureFile = "";
	strTextureFile.clear();
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pTexture);
	SetRect(&m_rtSrcFade, 255, 21, 257, 23);
	SetRect(&m_rtDestFade, 0, 0, 800, 600);

	pkWindow = new SPWindowStatic(WIID_STORY_BUTTON_SKIN, 661, 501, 139, 88, this);
	pkWindow->SetWindowType(WND_TYPE_TOPBASE);
	pkWindow->SetImageNormalHead("DATA/PROLOGUE/UI/P_UI001.PNG", 1, 356, 140, 360);
	pkWindow->SetImageNormalBody("DATA/PROLOGUE/UI/P_UI001.PNG", 1, 360, 140, 364);
	pkWindow->SetImageNormalTail("DATA/PROLOGUE/UI/P_UI001.PNG", 1, 364, 140, 368);

	pkButton = new SPWindowButton(WIID_STORY_NEXT, 22, 11, 98, 32, pkWindow);
	pkButton->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 314, 345);
	pkButton->SetImageHit("DATA/PROLOGUE/UI/P_UI001.PNG", 314, 378);
	pkButton->SetImagePush("DATA/PROLOGUE/UI/P_UI001.PNG", 314, 411);
	pkButton->SetImageDisable("DATA/PROLOGUE/UI/P_UI001.PNG", 314, 444);

	pkButton = new SPWindowButton(WIID_STORY_SKIP, 22, 48, 98, 32, pkWindow);
	pkButton->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 413, 345);
	pkButton->SetImageHit("DATA/PROLOGUE/UI/P_UI001.PNG", 413, 378);
	pkButton->SetImagePush("DATA/PROLOGUE/UI/P_UI001.PNG", 413, 411);
	pkButton->SetImageDisable("DATA/PROLOGUE/UI/P_UI001.PNG", 413, 444);
	
	m_iCurIndex = 0;
	m_iMaxIndex = 0;
	m_pkCurStoryFactor = NULL;
}

//------------------------------------------------------------------------------------
void SPWindowStory::ClearTexture()
{
	vector< SPTexture* >::iterator iter = m_vTexture.begin();
	while( iter != m_vTexture.end() )
	{
		SAFE_RELEASE(*iter);
		++iter;
	}
	m_vTexture.clear();
}

//------------------------------------------------------------------------------------
void SPWindowStory::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);

	if( m_pkCurStoryFactor )
	{
		if( g_pSubGameManager )
			g_pSubGameManager->SPPerformMessage(SPGM_SUG_SETBGM, (WPARAM)m_pkCurStoryFactor->strBGM.c_str(), 0);
	}
}

//------------------------------------------------------------------------------------
void SPWindowStory::Process(float fTime)
{
	if( !m_bShow )
		return;

	ProcessFade(fTime) ;

	SPWindow::Process(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowStory::ProcessFade(float fTime)
{
	if(m_iViewState == STORYVIEW_FADEIN ) {
		m_fFadeTime += fTime ;
		if( m_fFadeTime > 0.02f )	{
				m_fFadeTime = 0 ;
			if(m_iAlpha > 0) {
				m_iAlpha -= 10;
			}
			if(m_iAlpha < 1) {
				m_iAlpha = 0;
				SetViewState(STORYVIEW_STORY) ;
			}
		}
	}	
	else if(m_iViewState == STORYVIEW_FADEOUT ) {
		m_fFadeTime += fTime ;
		if( m_fFadeTime > 0.015f )	{
			m_fFadeTime = 0 ;
			if(m_iAlpha < MAX_FADE_ALPHA) {
				m_iAlpha += 10;
			}

			if(m_iAlpha >= MAX_FADE_ALPHA) {
				m_iAlpha = MAX_FADE_ALPHA;
				if( g_pSubGameManager )
					g_pSubGameManager->SPPerformMessage(SPGM_SET_STORY_VIEW, 0, 0);
			}
		}
	}
}

//------------------------------------------------------------------------------------
void SPWindowStory::ImageRender()
{
	RECT rcDst, rcSrc;
	SetRect(&rcDst, 0, 0, 800, 600);
	SetRect(&rcSrc, 0, 0, 800, 600);

	if( m_iCurIndex >= m_iMaxIndex )
		return;

	if( m_iCurIndex >= (int)m_vTexture.size() )
		return;

	vector< SPTexture* >::iterator iter = m_vTexture.begin() + m_iCurIndex;
	if( (*iter) == NULL )
		return;

	(*iter)->RenderTexture(&rcDst, &rcSrc);
	g_pVideo->Flush();
}

//------------------------------------------------------------------------------------
void SPWindowStory::Render(float fTime)
{
	if( !m_bShow )
		return;
	
	ImageRender();
	SPWindow::Render(fTime);

	if(m_pTexture && m_iAlpha > 0) {
		m_pTexture->SetColor(RGBA(255, 255, 255, m_iAlpha));
		m_pTexture->RenderTexture(&m_rtDestFade, &m_rtSrcFade);
		m_pTexture->SetColor(RGBA(255, 255, 255, 255));		
	}
}

//------------------------------------------------------------------------------------
// Message
void SPWindowStory::SetViewState(STORYVIEW_STATE iVeiwState)
{
	m_iViewState = iVeiwState ;
	m_fFadeTime = 0 ;
}



//------------------------------------------------------------------------------------
// Message
int SPWindowStory::OnSkip(WPARAM wParam, LPARAM lParam)
{
	SetViewState(STORYVIEW_FADEOUT) ;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowStory::OnNext(WPARAM wParam, LPARAM lParam)
{
	m_iCurIndex++;
	if( m_iCurIndex >= m_iMaxIndex ) {
		--m_iCurIndex ;
		OnSkip(0, 0);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowStory::OnSetFactor(WPARAM wParam, LPARAM lParam)
{
	m_iCurIndex = 0;
	m_iMaxIndex = 0;
	m_pkCurStoryFactor = NULL;
	ClearTexture();

	if( g_pkStoryManger )
	{
		m_pkCurStoryFactor = g_pkStoryManger->GetStoryFactor(lParam);
		if( m_pkCurStoryFactor )
		{
			m_iMaxIndex = (int)m_pkCurStoryFactor->vstrImage.size();
			SPTexture* pkTexture;

			vector< string >::iterator iter = m_pkCurStoryFactor->vstrImage.begin();
			while( iter != m_pkCurStoryFactor->vstrImage.end() )
			{
				if( !g_pVideo->GetTextureMgr()->LoadTexture((*iter).c_str(), &pkTexture) )
					pkTexture = NULL;
				
				m_vTexture.push_back(pkTexture);
				++iter;
			}
		}
		SetViewState(STORYVIEW_FADEIN) ;
	}

	return 1;
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowStory)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_STORY_SKIP,		OnSkip )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_STORY_NEXT,		OnNext )

SPIMESSAGE_COMMAND( SPIM_STORY_SET_FACTOR,			OnSetFactor )
SPIMESSAGE_COMMAND( SPIM_STORY_FORCE_EXIT,			OnSkip )

SPIMESSAGE_OBJ_MAP_END(SPWindow)
