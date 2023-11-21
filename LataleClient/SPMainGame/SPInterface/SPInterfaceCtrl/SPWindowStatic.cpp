
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

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPAbuseFilter.h"

SPWindowStatic::SPWindowStatic(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_CTRL_STATIC, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}

SPWindowStatic::~SPWindowStatic()
{
	Clean();
}


void SPWindowStatic::InitFromXML(SPWindow_Base* rec_wnd_AttrMap)
{
	SPWindow::InitFromXML(rec_wnd_AttrMap);

	SetFormat(UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_ALIGN));
	SetWindowText(UI_STRING(rec_wnd_AttrMap,GET_SPWINDOW_STATIC_TEXT));
}

void SPWindowStatic::Init()
{
	SetFormat(DT_VCENTER | DT_CENTER | DT_WORDBREAK);

	m_fImageScaleX	=	1.0f;
	m_fImageScaleY	=	1.0f;

	m_bCenterRender	=	false;

	// 윈도퓖E맛惇煞”穗?병렬추가 [6/12/2006]
	m_pNormalHead	= NULL; 
	m_pNormalBody	= NULL; 
	m_pNormalTail	= NULL; 
	
	m_iSrcWidth		= 0;
	m_iSrcHeight 	= 0;
	m_iSrcMargin 	= 0;

	for( int i = 0 ; i < 3 ; i++ )	{
		ZeroMemory(&m_rcSrcHead[i]		,	sizeof(RECT));
		ZeroMemory(&m_rcSrcBody[i]		,	sizeof(RECT));
		ZeroMemory(&m_rcSrcTail[i]		,	sizeof(RECT));

		ZeroMemory(&m_rcCalcDestHead[i]	,	sizeof(RECT));
		ZeroMemory(&m_rcCalcDestBody[i]	,	sizeof(RECT));
		ZeroMemory(&m_rcCalcDestTail[i]	,	sizeof(RECT));
	}

	m_vpImageAttr.clear() ;	

	SetTextAutoWidthMin( 0 );
	SetTextAutoWidthMax( 0 );
}

void SPWindowStatic::Clean()
{
	SPWindow::Clean();

	std::vector<IMAGE_ATTR>::iterator Iter = m_vpImageAttr.begin();
	for(; Iter != m_vpImageAttr.end(); ++Iter) {
		SAFE_RELEASE((*Iter).pImage);
	}
	m_vpImageAttr.clear();
	m_vWindowTextList.clear();

	if( WND_TYPE_NINEBASE )
	{
		SAFE_RELEASE(m_pNormalHead);
		m_pNormalBody = NULL ;
		m_pNormalTail = NULL ;
	}
	else
	{
		SAFE_RELEASE(m_pNormalHead);
		SAFE_RELEASE(m_pNormalBody);
		SAFE_RELEASE(m_pNormalTail);
	}
}

void SPWindowStatic::SetImage(SPTexture* pImage, int iSrcX, int iSrcY)
{
	IMAGE_ATTR ImageAttr;
	ImageAttr.pImage = pImage;
	ImageAttr.SrcPoint.x = iSrcX;
	ImageAttr.SrcPoint.y = iSrcY;
	m_vpImageAttr.push_back(ImageAttr);	

	OnSetImageIndex(0,0);
	//ReposOwnImage() ;
}

bool SPWindowStatic::SetImage(const char* szImage, int iSrcX, int iSrcY)
{
	SPTexture* pImage = NULL;	
	std::string strResource;
	
	strResource = szImage;

	// "" 가 들엉倍경퓖E예외처리 [8/22/2006 AJJIYA]
	if( strResource.empty() == true )
		return false;

	// 텍스처 로탛E실패시 false 반환 [4/12/2006]
	if( g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false )
		return false;

	SetImage(pImage, iSrcX, iSrcY);	
	return true;
}

void SPWindowStatic::SetImagePos(int iSrcX, int iSrcY)
{
	m_iNormalSrcX = m_iSrcX = iSrcX ; 
	m_iNormalSrcY = m_iSrcY = iSrcY ;
	SetSourceRect(iSrcX, iSrcY, 0, 0, 0) ;
}


void SPWindowStatic::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowStatic::SetWindowText(const char* pStr)
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

void SPWindowStatic::Process(float fTime)
{
	// Render에서 SourceSize에 대한 계산을 해주던것을 Process로 옮김
	if( !m_bMoveable )
		ReposOwnImage();
	SPWindow::Process(fTime);
}

void SPWindowStatic::Render(float fTime)
{
	if(!m_bShow) return;

//	SPWindow::Render(fTime);

	switch(m_WndType) {
	case WND_TYPE_LEFTBASE:
	case WND_TYPE_TOPBASE:
		{		
			g_pVideo->Flush();

			if(m_pNormalHead) {
				m_pNormalHead->RenderTexture(&m_rcCalcDestHead[0],		&m_rcSrcHead[0]);
			}
			if(m_pNormalBody ) {
				m_pNormalBody->RenderTexture(&m_rcCalcDestBody[0],		&m_rcSrcBody[0]);
			}
			if(m_pNormalTail) {
				m_pNormalTail->RenderTexture(&m_rcCalcDestTail[0],		&m_rcSrcTail[0]);
			}
		}
		break;
	case WND_TYPE_NINEBASE:
		{
			g_pVideo->Flush();

			if(m_pNormalHead) {
				m_pNormalHead->RenderTexture(&m_rcCalcDestHead[0],		&m_rcSrcHead[0]);
				m_pNormalHead->RenderTexture(&m_rcCalcDestHead[1],		&m_rcSrcHead[1]);
				m_pNormalHead->RenderTexture(&m_rcCalcDestHead[2],		&m_rcSrcHead[2]);
			}
			if(m_pNormalBody ) {
				m_pNormalBody->RenderTexture(&m_rcCalcDestBody[0],		&m_rcSrcBody[0]);
				m_pNormalBody->RenderTexture(&m_rcCalcDestBody[1],		&m_rcSrcBody[1]);
				m_pNormalBody->RenderTexture(&m_rcCalcDestBody[2],		&m_rcSrcBody[2]);
			}
			if(m_pNormalTail) {
				m_pNormalTail->RenderTexture(&m_rcCalcDestTail[0],		&m_rcSrcTail[0]);
				m_pNormalTail->RenderTexture(&m_rcCalcDestTail[1],		&m_rcSrcTail[1]);
				m_pNormalTail->RenderTexture(&m_rcCalcDestTail[2],		&m_rcSrcTail[2]);
			}
		}
		break ;
	case WND_TYPE_RECT:
		{		
			g_pVideo->Flush();

			if(m_pNormalHead) {
				m_pNormalHead->RenderTexture(&m_rcCalcDestHead[0],		&m_rcSrcHead[0]);
			}
		}
		break;
	case WND_TYPE_NULL:
	default:
		{		
			if(m_ppImageActive != NULL && *m_ppImageActive) {		//[2005/5/18] - jinhee

				g_pVideo->Flush();

				//RECT DestRect;
				//DestRect.top	= m_iAY;
				//DestRect.left	= m_iAX;
				//DestRect.bottom	= m_iAY + m_iSY;
				//DestRect.right  = m_iAX + m_iSX;

				//int iSrcX, iSrcY;
				//if( m_iSrcSX && m_iSrcSY )
				//{
				//	iSrcX = m_iSrcSX;
				//	iSrcY = m_iSrcSY;
				//}
				//else
				//{
				//	iSrcX = m_iSX;
				//	iSrcY = m_iSY;
				//}

				//RECT SrcRect;
				//SrcRect.top		= m_iSrcY;
				//SrcRect.left	= m_iSrcX;
				//SrcRect.bottom	= m_iSrcY+iSrcY;
				//SrcRect.right	= m_iSrcX+iSrcX;

				if( m_bLightMap )
					g_pVideo->SetAlphaTexture(true);

				(*m_ppImageActive)->SetScale( m_fImageScaleX , m_fImageScaleY );
				(*m_ppImageActive)->SetColor(m_RGBA);

				if( m_bCenterRender == false )
					(*m_ppImageActive)->RenderTexture(&m_rcCalcDestHead[0], &m_rcSrcHead[0]);
				else
				{
					(*m_ppImageActive)->SetSrcRect( m_rcSrcHead[0] );
					(*m_ppImageActive)->RenderTextureImmediateCenterBase( m_iAX , m_iAY );
				}

				(*m_ppImageActive)->SetColor( INITCOLOR );
				(*m_ppImageActive)->SetScale( 1.0f , 1.0f );

				if( m_bLightMap )
					g_pVideo->SetAlphaTexture(false);
			}
		}
		break;
	};

	RenderText();

	//RECT TarRect;
	//GetWindowRect(TarRect);
	//g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 1.0f , 0.0f , 0.0f , 1.0f ) );
	//g_pVideo->GetSysLine()->DrawRect( TarRect );

	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); ++Iter) {
		(*Iter)->Render(fTime);
	}

	//if(!m_strWindowText.empty()) { 	 
	//	RECT TarRect;
	//	GetWindowRect(TarRect);
	//	
	//	g_pVideo->GetFont()->RenderText(m_strWindowText.c_str(), &TarRect, m_dwAlign);			
	//}
}

void SPWindowStatic::RenderText()
{
	if( m_strWindowText.empty() )
		return;

	if( g_pVideo == NULL )
		return;

	if( m_pFont == NULL )
		return;

	if( m_bMultiLine )
	{
		bool bTemp = m_pFont->IsShadow();
		m_pFont->SetColor(m_FontColor);
		m_pFont->SetEffect( m_FontEffect );
		//m_pFont->SetPeriod(m_bTextPeriod);
		//g_pVideo->GetFont()->SetScale( m_fFontScaleX , m_fFontScaleY );
		
		RECT TarRect;
		GetWindowRect(TarRect);

		DWORD dwFormat;
		dwFormat = m_dwAlign;
		dwFormat &= ~DT_WORDBREAK;

		// AJJIYA [9/27/2005]
		// DT_VCENTER 기능 구?E
		if( ( dwFormat & DT_VCENTER ) == DT_VCENTER )
		{
			dwFormat &= ~DT_VCENTER;

			int	iTotalHeight		=	TarRect.bottom	-	TarRect.top;
			int	iTotalLineHeight	=	GetTextPixelHeight()	-	m_iLineInterval;
			int iTopMargin			=	int( ( iTotalHeight	-	iTotalLineHeight ) / 2 );

			TarRect.top	+=	iTopMargin;
		}

		vector< std::string >::iterator iter = m_vWindowTextList.begin();
		while( iter != m_vWindowTextList.end() )
		{
			m_pFont->RenderText((*iter).c_str(), &TarRect, dwFormat);

			TarRect.top += (int)m_pFont->GetHeight() + m_iLineInterval;
			++iter;
		}
		
		m_pFont->SetColor();
		m_pFont->SetShadow(	bTemp );
		//m_pFont->SetPeriod(false);


	} else {
		if(!m_strWindowText.empty()) {
			RECT TarRect;
			bool bTemp = m_pFont->IsShadow();
			D3DXCOLOR colTemp = m_pFont->GetShadowColor() ;
			GetWindowRect( TarRect );

			if(m_dwAlign & DT_VCENTER) {				
				OffsetRect( &TarRect , 
					0,  
					(m_iSY- m_pFont->GetHeight())/2);
			}
			
			m_pFont->SetColor(	m_FontColor );
			m_pFont->SetEffect( m_FontEffect );
			m_pFont->SetShadowColor(	m_FontShadowColor );
			m_pFont->SetPeriod(m_bTextPeriod);
			m_pFont->RenderText(m_strWindowText.c_str(), &TarRect, m_dwAlign);

			m_pFont->SetColor();
			m_pFont->SetShadow(	bTemp );
			m_pFont->SetShadowColor( colTemp );
			m_pFont->SetPeriod(false);
		}
	}
}

/**
	[2008.08.16]duragon
	SPWindow의 경우 Render시에 SrcSize를 통해서 이미지의 크기를 계산하도록 되어 있음
	Static control의 경우 RECT에 저장해서 사용하도록 설정을 수정한다.
*/
void SPWindowStatic::SetSrcSize(int isx, int isy)
{
	m_iSrcSX = isx;
	m_iSrcSY = isy;
	SetSourceRect(isx, isy, 0, 0, 0) ;
}


int SPWindowStatic::GetTextPixelHeight()
{
	if( g_pVideo == NULL )
		return 0;

	if( m_pFont == NULL )
		return 0;

	return GetTextCount() * ( (int)m_pFont->GetHeight() + m_iLineInterval );
}

void SPWindowStatic::SetColor( DWORD rgba )
{
	m_RGBA	=	rgba;
}

DWORD SPWindowStatic::GetColor()
{
	return m_RGBA;
}

void SPWindowStatic::SetImageScaleX( float fScaleX )
{
	m_fImageScaleX	=	fScaleX;
}

void SPWindowStatic::SetImageScaleY( float fScaleY )
{
	m_fImageScaleY	=	fScaleY;
}

float SPWindowStatic::GetIamgeScaleY()
{
	return m_fImageScaleX;
}

float SPWindowStatic::GetImageScaleX()
{
	return m_fImageScaleY;
}

void SPWindowStatic::SetCenterRender( bool bCenter )
{
	m_bCenterRender	=	bCenter;
}

bool SPWindowStatic::GetCenterRender()
{
	return m_bCenterRender;
}

//////////////////////////////////////////////////////////////////////////
//  [6/12/2006]

void SPWindowStatic::ReposOwnImage()
{
	switch(m_WndType) {
	case WND_TYPE_LEFTBASE:
		{
			SetRect(&m_rcCalcDestHead[0], 
				m_iAX,
				m_iAY, 
				m_iAX + m_rcSrcHead[0].right - m_rcSrcHead[0].left, 
				m_iAY + m_rcSrcHead[0].bottom - m_rcSrcHead[0].top);

			SetRect(&m_rcCalcDestBody[0],  
				m_iAX + m_rcSrcHead[0].right - m_rcSrcHead[0].left, 
				m_iAY, 
				m_iAX + m_iSX - (m_rcSrcTail[0].right - m_rcSrcTail[0].left), 
				m_iAY + m_rcSrcHead[0].bottom - m_rcSrcHead[0].top);

			SetRect(&m_rcCalcDestTail[0], 
				m_rcCalcDestBody[0].right,
				m_rcCalcDestBody[0].top, 
				m_iAX + m_iSX, 
				m_rcCalcDestBody[0].bottom);
		}
		break;
	case WND_TYPE_TOPBASE:
		{
			SetRect(&m_rcCalcDestHead[0], 
				m_iAX,
				m_iAY, 
				m_iAX + m_rcSrcHead[0].right - m_rcSrcHead[0].left, 
				m_iAY + m_rcSrcHead[0].bottom - m_rcSrcHead[0].top);

			SetRect(&m_rcCalcDestBody[0],
				m_iAX, 
				m_iAY + m_rcSrcHead[0].bottom - m_rcSrcHead[0].top, 
				//m_iAX + m_rcSrcHead.right - m_rcSrcHead.left, 
				m_iAX + m_iSX, 
				m_iAY + m_iSY - (m_rcSrcTail[0].bottom - m_rcSrcTail[0].top));

			SetRect(&m_rcCalcDestTail[0],
				m_rcCalcDestBody[0].left,
				m_rcCalcDestBody[0].bottom, 
				m_rcCalcDestBody[0].right, 
				m_iAY + m_iSY);
		}
		break;
	case WND_TYPE_NINEBASE:
		{
			SetRect(&m_rcCalcDestHead[0], 
				m_iAX,						m_iAY, m_iAX+m_iSrcWidth, m_iAY+m_iSrcHeight);
			SetRect(&m_rcCalcDestHead[1], 
				m_rcCalcDestHead[0].right,	m_iAY, m_iAX+m_iSX-m_iSrcWidth, m_iAY+m_iSrcHeight);
			SetRect(&m_rcCalcDestHead[2], 
				m_rcCalcDestHead[1].right,	m_iAY, m_iAX+m_iSX, m_iAY+m_iSrcHeight);

			SetRect(&m_rcCalcDestBody[0], 
				m_iAX,						m_iAY+m_iSrcHeight, m_iAX+m_iSrcWidth, m_iAY+m_iSY-m_iSrcHeight);
			SetRect(&m_rcCalcDestBody[1], 
				m_rcCalcDestBody[0].right,	m_iAY+m_iSrcHeight, m_iAX+m_iSX-m_iSrcWidth, m_iAY+m_iSY-m_iSrcHeight);
			SetRect(&m_rcCalcDestBody[2], 
				m_rcCalcDestBody[1].right,	m_iAY+m_iSrcHeight, m_iAX+m_iSX, m_iAY+m_iSY-m_iSrcHeight);

			SetRect(&m_rcCalcDestTail[0], 
				m_iAX,						m_iAY+m_iSY-m_iSrcHeight, m_iAX+m_iSrcWidth, m_iAY+m_iSY);
			SetRect(&m_rcCalcDestTail[1], 
				m_rcCalcDestTail[0].right,	m_iAY+m_iSY-m_iSrcHeight, m_iAX+m_iSX-m_iSrcWidth, m_iAY+m_iSY);
			SetRect(&m_rcCalcDestTail[2], 
				m_rcCalcDestTail[1].right,	m_iAY+m_iSY-m_iSrcHeight, m_iAX+m_iSX, m_iAY+m_iSY);
		}
		break ;
	case WND_TYPE_RECT:
	case WND_TYPE_NULL:
	default:
		{
			//RECT DestRect;
			if(m_flg_Flip_LR)
			{
				m_rcCalcDestHead[0].left	= m_iAX + m_iSX;
				m_rcCalcDestHead[0].top		= m_iAY + m_iSY;
				m_rcCalcDestHead[0].right	= m_iAX;
				m_rcCalcDestHead[0].bottom	= m_iAY;
			}
			else
			{
				m_rcCalcDestHead[0].top		= m_iAY;
				m_rcCalcDestHead[0].left	= m_iAX;
				m_rcCalcDestHead[0].bottom	= m_iAY + m_iSY;
				m_rcCalcDestHead[0].right	= m_iAX + m_iSX;
			}
		}
		break;
	}
}


/**
	이미지가 셋팅되거나 바뀌는 경우 이미지의 Source 좌표를 설정하도록 한다.
	WND_TYPE_NULL : 통짜 이미지 (0)
	WND_TYPE_LEFTBASE : 각각이미지를 설정하도록 되어 있음(X)
	WND_TYPE_TOPBASE : 각각이미지를 설정하도록 되어 있음(X)
	WND_TYPE_NINEBASE : 규칙에 따라 만들면 사용가능(0)
*/
bool SPWindowStatic::SetSourceRect(int iXSrc, int iYSrc, int iWidthSrc, int iHeightSrc, int iMargin)
{
	switch(m_WndType) {
	case WND_TYPE_LEFTBASE:
		{
		}
		break;
	case WND_TYPE_TOPBASE:
		{
		}
		break;
	case WND_TYPE_NINEBASE:
		{
			for( int i = 0; i < 3; i++ )
			{
				m_rcSrcHead[i].left		= iXSrc + (iWidthSrc+iMargin)*i;
				m_rcSrcHead[i].top		= iYSrc;
				m_rcSrcHead[i].right	= m_rcSrcHead[i].left + iWidthSrc ;
				m_rcSrcHead[i].bottom	= m_rcSrcHead[i].top + iWidthSrc;

				m_rcSrcBody[i].left		= iXSrc + (iWidthSrc+iMargin)*i ;
				m_rcSrcBody[i].top		= iYSrc + iMargin+iWidthSrc ;
				m_rcSrcBody[i].right	= m_rcSrcBody[i].left + iWidthSrc ;
				m_rcSrcBody[i].bottom	= m_rcSrcBody[i].top + iWidthSrc;

				m_rcSrcTail[i].left		= iXSrc + (iWidthSrc+iMargin)*i;
				m_rcSrcTail[i].top		= iYSrc + iMargin+iWidthSrc + iMargin+iWidthSrc;
				m_rcSrcTail[i].right	= m_rcSrcTail[i].left + iWidthSrc ;
				m_rcSrcTail[i].bottom	= m_rcSrcTail[i].top + iWidthSrc;
			}
		}
		break ;

	case WND_TYPE_NULL:
	default:
		{
			int iSrcX, iSrcY;
			if( m_iSrcSX && m_iSrcSY )
			{
				iSrcX = m_iSrcSX;
				iSrcY = m_iSrcSY;
			}
			else
			{
				iSrcX = m_iSX;
				iSrcY = m_iSY;
			}

			//RECT SrcRect;
			m_rcSrcHead[0].top		= m_iSrcY;
			m_rcSrcHead[0].left		= m_iSrcX;
			m_rcSrcHead[0].bottom	= m_iSrcY+iSrcY;
			m_rcSrcHead[0].right	= m_iSrcX+iSrcX;
		}
		break;
	}
	return true ;
}

void SPWindowStatic::SetTextAutoWidthMin( int iTextAutoWidthMin )
{
	m_iTextAutoWidthMin	=	iTextAutoWidthMin;
}

void SPWindowStatic::SetTextAutoWidthMax( int iTextAutoWidthMax )
{
	m_iTextAutoWidthMax	=	iTextAutoWidthMax;
}

void SPWindowStatic::CalTextAutoWidth()
{
	if( m_iTextAutoWidthMin <= 0 && m_iTextAutoWidthMax <= 0 )
		return;

	if( m_pFont == NULL )
		return;

	SIZE	stTextSize		=	{0,0};

	if( m_bMultiLine == false )
	{
		stTextSize		=	m_pFont->GetSize( m_strWindowText.c_str() );
		m_iSX			=	stTextSize.cx;
	}
	else
	{
		vector< std::string >::iterator iter = m_vWindowTextList.begin();
		while( iter != m_vWindowTextList.end() )
		{
			stTextSize	=	m_pFont->GetSize( (*iter).c_str() );
			m_iSX		=	max( m_iSX , stTextSize.cx );

			++iter;
		}
	}

	if( m_iTextAutoWidthMin > 0 && m_iSX < m_iTextAutoWidthMin )
		m_iSX = m_iTextAutoWidthMin;

	if( m_iTextAutoWidthMax > 0 && m_iSX > m_iTextAutoWidthMax )
		m_iSX = m_iTextAutoWidthMax;
}

//[xialin 2008/07/08]add
bool SPWindowStatic::SetImageRect(SPTexture * pTexture, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	if(!pTexture)
		return false;

	m_pNormalHead		= pTexture;

	m_rcSrcHead[0].left	= iXSrc;
	m_rcSrcHead[0].top		= iYSrc;
	m_rcSrcHead[0].right	= iX2Src;
	m_rcSrcHead[0].bottom	= iY2Src;
	ReposOwnImage();
	return true;
}

bool SPWindowStatic::SetImageNormalHead	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	m_pNormalHead		= pImage;

	m_rcSrcHead[0].left	= iXSrc;
	m_rcSrcHead[0].top		= iYSrc;
	m_rcSrcHead[0].right	= iX2Src;
	m_rcSrcHead[0].bottom	= iY2Src;
	ReposOwnImage();
	return true;
}

bool SPWindowStatic::SetImageNormalBody	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	m_pNormalBody		= pImage;

	m_rcSrcBody[0].left	= iXSrc;
	m_rcSrcBody[0].top		= iYSrc;
	m_rcSrcBody[0].right	= iX2Src;
	m_rcSrcBody[0].bottom	= iY2Src;
	ReposOwnImage();
	return true;
}

bool SPWindowStatic::SetImageNormalTail	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}

	m_pNormalTail		= pImage;
	m_rcSrcTail[0].left	= iXSrc;
	m_rcSrcTail[0].top		= iYSrc;
	m_rcSrcTail[0].right	= iX2Src;
	m_rcSrcTail[0].bottom	= iY2Src;
	ReposOwnImage();
	return true;
}


bool SPWindowStatic::SetImageNine(const char* szImage, int iXSrc, int iYSrc, int iWidth, int iMargin)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	m_pNormalHead		= pImage;		// Left Top
	m_pNormalBody		= pImage;		// Left Middle
	m_pNormalTail		= pImage;		// Left Bottom
	m_iSrcX				= iXSrc;		// Image Source X
	m_iSrcY				= iYSrc;		// Image Source Y
	m_iSrcWidth			= iWidth ;
	m_iSrcHeight		= iWidth ;
	m_iSrcMargin		= iMargin ;

	SetSourceRect(m_iSrcX, m_iSrcY, m_iSrcWidth, m_iSrcHeight, m_iSrcMargin) ;
	ReposOwnImage() ;
	//SetImage(pImage, m_iSrcX, m_iSrcY);	
	return true;
}

//////////////////////////////////////////////////////////////////////////


SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowStatic )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
	SPIMESSAGE_COMMAND(	SPIM_SET_IMAGEINDEX,	OnSetImageIndex )
	SPIMESSAGE_COMMAND(	SPIM_SET_IMAGECLEAR,	OnImageClear )
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnLButtonUp )
	SPIMESSAGE_COMMAND(	SPIM_RBUTTON_UP,		OnRButtonUp )
	
	SPIMESSAGE_COMMAND( SPIM_WHEEL_UP,			OnWheelUp		)
	SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN,		OnWheelDown		)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowStatic::OnSetImageIndex( WPARAM wParam, LPARAM lParam)
{
	int iIndex = (int)wParam;
	if( iIndex >= 0 && iIndex < (int)m_vpImageAttr.size()) {
		m_iCurImage	= iIndex;
	}

	if(!m_vpImageAttr.empty()) {
		m_ppImageActive = &(m_vpImageAttr[m_iCurImage].pImage);
		m_iSrcX = m_vpImageAttr[m_iCurImage].SrcPoint.x;
		m_iSrcY = m_vpImageAttr[m_iCurImage].SrcPoint.y;

		SetSourceRect(m_iSrcX, m_iSrcY, m_iSrcWidth, m_iSrcHeight, m_iSrcMargin) ;
		ReposOwnImage() ;
	}

	return 0;
}

int SPWindowStatic::OnImageClear( WPARAM wParam, LPARAM lParam)
{
	m_vpImageAttr.clear();
	m_iCurImage = 0;
	m_ppImageActive = NULL;

	return 0;
}

int SPWindowStatic::OnCursorEnter( WPARAM wParam, LPARAM lParam)
{	
	return SPSendMessageToParent(SPIM_CURSOR_ENTER, wParam, lParam, this);	
}

int SPWindowStatic::OnCursorOut( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_CURSOR_OUT, wParam, lParam, this);
}


int SPWindowStatic::OnLButtonUp( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, lParam, this);
}


int SPWindowStatic::OnRButtonUp(WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_RBUTTON_UP, wParam, lParam, this);
}


int SPWindowStatic::OnWheelUp( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_WHEEL_UP, wParam, lParam, this);
}


int SPWindowStatic::OnWheelDown( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_WHEEL_DOWN, wParam, lParam, this);
}

void SPWindowStatic::DeleteLastTexture()
{
	std::vector<IMAGE_ATTR>::iterator itr = m_vpImageAttr.begin(); itr++;

	m_vpImageAttr.erase(itr,m_vpImageAttr.end());
}













