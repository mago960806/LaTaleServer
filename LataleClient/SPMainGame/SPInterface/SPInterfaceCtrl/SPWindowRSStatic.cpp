
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

#include "SPResourceDef.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPWindowListDEF.h"
#include "SPWindow.h"
#include "SPWindowRSStatic.h"
#include "SPAbuseFilter.h"

SPWindowRSStatic::SPWindowRSStatic(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_CTRL_RS_STATIC, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pBarTexture(NULL)
{
	Init();
}

SPWindowRSStatic::~SPWindowRSStatic()
{
	Clean();
}

void SPWindowRSStatic::Init()
{
	SetFormat(DT_VCENTER | DT_CENTER | DT_WORDBREAK);

	m_bCenterRender	=	false;
	m_bUseFontOver = false ;
	m_bFontShadow = false ;

	m_iSrcWidth		= 0;
	m_iSrcHeight 	= 0;
	m_iSrcMargin 	= 0;

	m_CurFontColor = D3DXCOLOR(0,0,0,1);
}

void SPWindowRSStatic::Clean()
{
	SPWindow::Clean();

	m_vWindowTextList.clear();
	SAFE_RELEASE(m_pBarTexture) ;
}


void SPWindowRSStatic::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
}

void SPWindowRSStatic::SetWindowText(const char* pStr)
{
	SPWindow::SetWindowText(pStr);

	m_vWindowTextList.clear();

	if( g_pVideo == NULL )
		return;

	if( m_pFont == NULL || pStr == NULL )
		return;

	if( m_bMultiLine )
	{
		char szLineFeedBuf[MAX_STR_LENGTH];
		SPAbuseFilter::GetInstance()->ReplaceLineFeed(pStr, szLineFeedBuf, MAX_STR_LENGTH);

		RECT TarRect;
		GetWindowRect(TarRect);

		m_pFont->ConvertMultiLineW(szLineFeedBuf, TarRect, m_vWindowTextList);
	}


	/*
	if( m_bMultiLine )
	{
	int iCharCount = 0;

	string strText = m_strWindowText;
	string strLineText;

	RECT TarRect;
	GetWindowRect(TarRect);

	while( 1 )
	{
	strLineText = m_pFont->GetStringClipping(strText.c_str(), TarRect, iCharCount);
	if( strLineText.empty() )
	break;

	m_vWindowTextList.push_back(strLineText);

	int iSize = (int)strText.size();
	int iCopyCount = iSize - iCharCount;
	if( iCopyCount <= 0 )
	break;

	strText = strText.substr(iCharCount + 1);
	}
	}
	*/
}

void SPWindowRSStatic::Process(float fTime)
{
	SPWindow::Process(fTime);

	// Compute Destination Rect ============================================
	if( m_bMultiLine )
	{
		GetWindowRect(m_rcTgtRect);

		m_dwFormat = m_dwAlign;
		m_dwFormat &= ~DT_WORDBREAK;

		if( ( m_dwFormat & DT_VCENTER ) == DT_VCENTER )
		{
			m_dwFormat &= ~DT_VCENTER;
			int	iTotalHeight		=	m_rcTgtRect.bottom	-	m_rcTgtRect.top;
			int	iTotalLineHeight	=	GetTextPixelHeight()	-	m_iLineInterval;
			int iTopMargin			=	int( ( iTotalHeight	-	iTotalLineHeight ) / 2 );
			m_rcTgtRect.top	+=	iTopMargin;
		}
	} 
	else 
	{
		if(!m_strWindowText.empty()) {
			GetWindowRect( m_rcTgtRect );
			if(m_dwAlign & DT_VCENTER) {				
				OffsetRect( &m_rcTgtRect , 0, (int)((m_iSY- m_pFont->GetHeight())/2) );
			}
		}
	}
}

/**
	RSStatic의 주요기능은 TextRendering & MouseOver & ReturnValue
	SPWindow의 Render -> 배경 Image 사용 가능
*/
void SPWindowRSStatic::Render(float fTime)
{
	if(!m_bShow) return;

	SPWindow::Render(fTime) ;

	// Select Bar ============================================
	if( m_bUseSelectBar && IsCursorIn() && m_pBarTexture && bHasReturn() )
	{
		m_pBarTexture->RenderTexture( m_rcTgtRect.left+m_rcRelPosX, m_rcTgtRect.top+m_rcRelPosY , &m_rcSelectBarSrc );
	}

	RenderText();
}

void SPWindowRSStatic::RenderText()
{
	if( m_strWindowText.empty() )
		return;

	if( g_pVideo == NULL )
		return;

	if( m_pFont == NULL )
		return;

	bool bTemp = m_pFont->IsShadow();
	D3DXCOLOR colTemp = m_pFont->GetShadowColor() ;
	m_pFont->SetShadow(m_bFontShadow);
	m_pFont->SetShadowColor(m_FontShadowColor );
	m_bUseFontOver ? m_pFont->SetColor(m_CurFontColor) : m_pFont->SetColor(m_FontColor) ;
	m_pFont->SetPeriod(m_bTextPeriod);

	if( m_bMultiLine )
	{
		vector< std::string >::iterator iter = m_vWindowTextList.begin();
		while( iter != m_vWindowTextList.end() )
		{
			m_pFont->RenderText((*iter).c_str(), &m_rcTgtRect, m_dwFormat);
//#ifdef _DEBUG
//			// For Debug Box
//			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	// Image
//			g_pVideo->GetSysLine()->DrawRect(m_rcTgtRect);
//			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//#endif
			m_rcTgtRect.top += (int)m_pFont->GetHeight() + m_iLineInterval;
			++iter;
		}
	} 
	else 
	{
		if(!m_strWindowText.empty()) 
		{
			m_bUseFontOver ? m_pFont->SetColor(m_CurFontColor) : m_pFont->SetColor(m_FontColor) ;
			m_pFont->RenderText(m_strWindowText.c_str(), &m_rcTgtRect, m_dwAlign);
//#ifdef _DEBUG
//			// For Debug Box
//			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	// Image
//			g_pVideo->GetSysLine()->DrawRect(m_rcTgtRect);
//			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//#endif
		}
	}
	m_pFont->SetColor();
	m_pFont->SetPeriod(false);
	m_pFont->SetShadow(	bTemp );
	m_pFont->SetShadowColor( colTemp );
}


/**
	Font Color & Mouse Over Font Color Setting
*/
void SPWindowRSStatic::SetSelectFontcolor( bool bSet, D3DXCOLOR FontColor /*= D3DXCOLOR(0,0,0,1.0f)*/, D3DXCOLOR RvsColor /*= D3DXCOLOR(1.0f,0,0,1.0f)*/ )
{
	m_bUseFontOver = bSet ;
	if( bSet == false )
		return ;
	m_FontColor = FontColor ;
	m_CurFontColor = FontColor ;

	if( RvsColor == D3DXCOLOR(1.0f,0,0,1.0f) )	{
		m_RvsFontColor = D3DXCOLOR(1-FontColor.r,1-FontColor.g,1-FontColor.b,1) ;
	}
	else {
		m_RvsFontColor = RvsColor ;
	}
}


/**
	BG Select Bar 사용 여부
*/
void SPWindowRSStatic::SetSelectBar(const char* pszImage, RECT rcImageSrc, int iRelPosX, int iRelPosY)
{
	if( pszImage == NULL )
		return ;

	if( m_pBarTexture )
		SAFE_RELEASE( m_pBarTexture ) ;

	if( g_pVideo->GetTextureMgr()->LoadTexture( pszImage , &m_pBarTexture ) )
	{
		m_bUseSelectBar = true ;
		SetRect(&m_rcSelectBarSrc, rcImageSrc.left, rcImageSrc.top, rcImageSrc.right, rcImageSrc.bottom) ;
		m_rcRelPosX = iRelPosX ;
		m_rcRelPosY = iRelPosY ;
	}
}


/**
	Set Return Value
 */
void SPWindowRSStatic::SetReturnValue(int iValue, const char* pRetStr/*=NULL*/)
{
	m_stReturnValue.m_iReturnValue = iValue ;
	m_stReturnValue.m_strReturnValue = pRetStr ? pRetStr : "" ;
}

LIST_RETURNVALUE* SPWindowRSStatic::GetReturnValue()
{
	return &m_stReturnValue ;
}

bool SPWindowRSStatic::bHasReturn()
{
	return m_stReturnValue.m_iReturnValue ? true : false ;		
}


int SPWindowRSStatic::GetTextPixelHeight()
{
	if( g_pVideo == NULL )
		return 0;

	if( m_pFont == NULL )
		return 0;

	return GetTextCount() * ( (int)m_pFont->GetHeight() + m_iLineInterval );
}


void SPWindowRSStatic::SetCenterRender( bool bCenter )
{
	m_bCenterRender	=	bCenter;
}

bool SPWindowRSStatic::GetCenterRender()
{
	return m_bCenterRender;
}





//////////////////////////////////////////////////////////////////////////


SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowRSStatic )
SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnLButtonUp )

SPIMESSAGE_COMMAND( SPIM_WHEEL_UP,			OnWheelUp		)
SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN,		OnWheelDown		)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


/**
CursorEnter -> Text Color Change
*/
int SPWindowRSStatic::OnCursorEnter( WPARAM wParam, LPARAM lParam)
{	
	if( m_bUseFontOver )
		m_CurFontColor = m_RvsFontColor ;
	return SPSendMessageToParent(SPIM_CURSOR_ENTER, wParam, lParam, this);	
}

int SPWindowRSStatic::OnCursorOut( WPARAM wParam, LPARAM lParam)
{
	if( m_bUseFontOver )
		m_CurFontColor = m_FontColor ;
	return SPSendMessageToParent(SPIM_CURSOR_OUT, wParam, lParam, this);
}


/**
Link Return
*/
int SPWindowRSStatic::OnLButtonUp( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, lParam, this);
}


int SPWindowRSStatic::OnWheelUp( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_WHEEL_UP, wParam, lParam, this);
}


int SPWindowRSStatic::OnWheelDown( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_WHEEL_DOWN, wParam, lParam, this);
}

