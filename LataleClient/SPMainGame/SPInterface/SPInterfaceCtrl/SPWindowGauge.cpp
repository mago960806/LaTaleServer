
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"
#include "SPResourceDef.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPSubGameManager.h"

#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"
#include "SPWindow.h"
#include "SPWindowGauge.h"

SPWindowGauge::SPWindowGauge(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_CTRL_GAUGE, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pImageEdge(NULL)
, m_pImageFrameHead(NULL)
, m_pImageFrameBody(NULL)
, m_pImageFrameTail(NULL)
, m_pImageGauge(NULL)		
, m_pImageDeco(NULL)		
, m_pImageFlickerEdge(NULL)
, m_pImageFlickerGauge(NULL)
, m_pImageFlickerDeco(NULL)
{	
	ZeroMemory(&m_rcGaugeSrc,			sizeof(RECT));
	ZeroMemory(&m_rcGaugeEdgeSrc,		sizeof(RECT));
	ZeroMemory(&m_rcCalcGauge,			sizeof(RECT));
	ZeroMemory(&m_rcCalcGaugeEdge,		sizeof(RECT));

	ZeroMemory(&m_rcFrameHeadSrc,		sizeof(RECT));
	ZeroMemory(&m_rcFrameBodySrc,		sizeof(RECT));
	ZeroMemory(&m_rcFrameTailSrc,		sizeof(RECT));
	ZeroMemory(&m_rcCalcFrameHead,		sizeof(RECT));
	ZeroMemory(&m_rcCalcFrameBody,		sizeof(RECT));
	ZeroMemory(&m_rcCalcFrameTail,		sizeof(RECT));

	ZeroMemory(&m_rcFlickerEdgeSrc,		sizeof(RECT));
	ZeroMemory(&m_rcFlickerGaugeSrc,	sizeof(RECT));
	ZeroMemory(&m_rcFlickerDecoSrc,		sizeof(RECT));

	Init();
}

SPWindowGauge::~SPWindowGauge()
{
	Clean();
}

void SPWindowGauge::Init()
{
	m_strToolTip = "[SPWindowGauge]";

	m_iMaxValue = 10;
	m_iCurValue = 6;
	m_WndType = WND_TYPE_RIGHTBASE;	

	m_bShowFlicker				=	false;
	m_fFlickerPercent			=	0.0f;
	m_fFlickerAccmulateTime		=	0.0f;
	m_fFlickerHideLimitTime		=	0.3f;
	m_fFlickerShowLimitTime		=	0.1f;
	m_fFlickerTotalLimitTime	=	m_fFlickerHideLimitTime + m_fFlickerShowLimitTime;

	Refresh();
}


void SPWindowGauge::InitFromXML(SPWindow_Base* rec_wnd_AttrMap)
{
	SPWindow::InitFromXML(rec_wnd_AttrMap);

	s_Point4*	pt_Pos		= 0;
	CHAR*		sz_FileName = reinterpret_cast<CHAR*>(rec_wnd_AttrMap->Get(GET_SPWINDOW_IMAGE_FILENAME));

	SetWindowType(WND_TYPE_LEFTBASE);

	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_GAUGE_SET);			SetImageGauge(sz_FileName,*((RECT*)pt_Pos));
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_GAUGE_EDGE_SET);		SetImageEdge(sz_FileName,*((RECT*)pt_Pos));
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_GAUGE_DECO_SET);		SetImageDeco(sz_FileName,*((RECT*)pt_Pos));

	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_GAUGE_FRAMEHEAD_SET);	SetImageFrameHead(sz_FileName,pt_Pos->x,pt_Pos->y,pt_Pos->x2,pt_Pos->y2);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_GAUGE_FRAMEBODY_SET);	SetImageFrameBody(sz_FileName,pt_Pos->x,pt_Pos->y,pt_Pos->x2,pt_Pos->y2);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_GAUGE_FRAMETAIL_SET);	SetImageFrameTail(sz_FileName,pt_Pos->x,pt_Pos->y,pt_Pos->x2,pt_Pos->y2);

	//- Flicker추가 바람....
}


void SPWindowGauge::Clean()
{
	SPWindow::Clean();
	SAFE_RELEASE( m_pImageEdge );
	SAFE_RELEASE( m_pImageGauge );
	SAFE_RELEASE( m_pImageDeco );

	SAFE_RELEASE( m_pImageFrameHead );
	SAFE_RELEASE( m_pImageFrameBody );
	SAFE_RELEASE( m_pImageFrameTail );

	SAFE_RELEASE( m_pImageFlickerEdge );
	SAFE_RELEASE( m_pImageFlickerGauge );
	SAFE_RELEASE( m_pImageFlickerDeco );
}

void SPWindowGauge::SetImageEdge(const char* pszImage, RECT RectEdge)
{	
	SPTexture* pImage = NULL;	
	std::string strResource;
	
	strResource = pszImage;

	SAFE_RELEASE(m_pImageEdge);

	g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage);
	m_pImageEdge	= pImage;	
	m_rcGaugeEdgeSrc		= RectEdge;
	Refresh();
}

void SPWindowGauge::SetImageGauge(const char* pszImage, RECT rcGaugeSrc)
{
	SPTexture* pImage = NULL;	
	std::string strResource;

	strResource = pszImage;

	SAFE_RELEASE(m_pImageGauge);

	g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage);
	m_pImageGauge	= pImage;	
	m_rcGaugeSrc	= rcGaugeSrc;
	Refresh();
}

void SPWindowGauge::SetImageDeco(const char* pszImage, RECT rcDecoSrc)
{
	SPTexture* pImage = NULL;	
	std::string strResource;

	strResource = pszImage;

	SAFE_RELEASE(m_pImageDeco);

	g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage);
	m_pImageDeco	= pImage;	
	m_rcDecoSrc		= rcDecoSrc;
	Refresh();
}

void SPWindowGauge::Process(float fTime)
{
	Refresh(); // test
	SPWindow::Process(fTime);
}

void SPWindowGauge::Render(float fTime)
{
	if(!m_bShow) return;	
	g_pVideo->Flush();

	if(m_pImageGauge)
		m_pImageGauge->RenderTexture(&m_rcCalcGauge, &m_rcGaugeSrc);	

	if(m_pImageDeco)
		m_pImageDeco->RenderTexture(&m_rcCalcDeco, &m_rcDecoSrc);

	if(m_pImageEdge)
		m_pImageEdge->RenderTexture(&m_rcCalcGaugeEdge, &m_rcGaugeEdgeSrc);

	
	if( m_bShowFlicker == true )
	{
		m_fFlickerAccmulateTime	+=	fTime;

		if( m_fFlickerAccmulateTime > m_fFlickerTotalLimitTime )
		{
			m_fFlickerAccmulateTime	=	0.0f;
		}

		if( m_fFlickerAccmulateTime > m_fFlickerHideLimitTime )
		{
			if( m_pImageFlickerGauge )
				m_pImageFlickerGauge->RenderTexture( &m_rcCalcGauge, &m_rcFlickerGaugeSrc);

			if( m_pImageFlickerDeco )
				m_pImageFlickerDeco->RenderTexture( &m_rcCalcDeco, &m_rcFlickerDecoSrc);

			if( m_pImageFlickerEdge )
				m_pImageFlickerEdge->RenderTexture( &m_rcCalcGaugeEdge, &m_rcFlickerEdgeSrc);
		}
	}

	if( m_pImageFrameHead )
		m_pImageFrameHead->RenderTexture( &m_rcCalcFrameHead, &m_rcFrameHeadSrc);

	if( m_pImageFrameBody )
		m_pImageFrameBody->RenderTexture( &m_rcCalcFrameBody, &m_rcFrameBodySrc);

	if( m_pImageFrameTail )
		m_pImageFrameTail->RenderTexture( &m_rcCalcFrameTail, &m_rcFrameTailSrc);
}

void SPWindowGauge::Refresh()
{
	float fPercent;
	if(m_iCurValue == 0) {
		fPercent = 0.0f;
	} else if(m_iMaxValue <= m_iCurValue) {
		fPercent = 1.0f;
	} else {
		fPercent = ((float)m_iCurValue / m_iMaxValue);	
		if(fPercent < 0)
			fPercent = 0.0;
	}

	if( fPercent <= m_fFlickerPercent )
	{
		m_bShowFlicker			=	true;
	}
	else
	{
		m_bShowFlicker			=	false;
		m_fFlickerAccmulateTime	=	0.0f;
	}

	switch(m_WndType) {
	case WND_TYPE_RIGHTBASE:
		{
			m_rcCalcGauge.left		= m_iAX + (m_iSX * (1.0f - fPercent));
			m_rcCalcGauge.top		= m_iAY;
			m_rcCalcGauge.right		= m_iAX + m_iSX;
			m_rcCalcGauge.bottom	= m_iAY + m_iSY;			

			if( (m_rcCalcGauge.right - m_rcCalcGauge.left) > (m_rcGaugeEdgeSrc.right - m_rcGaugeEdgeSrc.left)) {			
				m_rcCalcGaugeEdge.left		= m_rcCalcGauge.right - (m_rcGaugeEdgeSrc.right - m_rcGaugeEdgeSrc.left);
				m_rcCalcGaugeEdge.top		= m_rcCalcGauge.top;
				m_rcCalcGaugeEdge.right		= m_rcCalcGauge.right;
				m_rcCalcGaugeEdge.bottom	= m_rcCalcGauge.bottom;
			} else {
				SetRect(&m_rcCalcGaugeEdge, 0,0,0,0);
			}

			if( (m_rcCalcGauge.right - m_rcCalcGauge.left) > (m_rcDecoSrc.right - m_rcDecoSrc.left)) {
				m_rcCalcDeco.left		= m_rcCalcGauge.left ;
				m_rcCalcDeco.top		= m_rcCalcGauge.top;
				m_rcCalcDeco.right		= m_rcCalcGauge.left + (m_rcDecoSrc.right - m_rcDecoSrc.left);
				m_rcCalcDeco.bottom		= m_rcCalcGauge.bottom;
			} else {
				SetRect(&m_rcCalcDeco, 0,0,0,0);
			}
		}
		break;
	case WND_TYPE_LEFTBASE:
		{
			m_rcCalcGauge.left		= m_iAX + 1;
			m_rcCalcGauge.top		= m_iAY + 1;
			//m_rcCalcGauge.right		= m_iAX + (m_iSX * fPercent) - 1;
			m_rcCalcGauge.right		= m_iAX + (m_iSX * fPercent);
			m_rcCalcGauge.bottom	= m_iAY + m_iSY - 1;

			if(fPercent != 1.0f && (m_rcCalcGauge.right - m_rcCalcGauge.left) > (m_rcGaugeEdgeSrc.right - m_rcGaugeEdgeSrc.left)) {			
				m_rcCalcGaugeEdge.left		= m_rcCalcGauge.right - (m_rcGaugeEdgeSrc.right - m_rcGaugeEdgeSrc.left);
				m_rcCalcGaugeEdge.top		= m_rcCalcGauge.top;
				m_rcCalcGaugeEdge.right		= m_rcCalcGauge.right;
				m_rcCalcGaugeEdge.bottom	= m_rcCalcGauge.bottom;
			} else {
				SetRect(&m_rcCalcGaugeEdge, 0,0,0,0);
			}

			if( (m_rcCalcGauge.right - m_rcCalcGauge.left) > (m_rcDecoSrc.right - m_rcDecoSrc.left + (m_rcGaugeEdgeSrc.right - m_rcGaugeEdgeSrc.left))) {
				m_rcCalcDeco.left		= m_rcCalcGauge.right - (m_rcDecoSrc.right - m_rcDecoSrc.left + (m_rcGaugeEdgeSrc.right - m_rcGaugeEdgeSrc.left));
				m_rcCalcDeco.top		= m_rcCalcGauge.top;
				m_rcCalcDeco.right		= m_rcCalcGauge.right - (m_rcGaugeEdgeSrc.right - m_rcGaugeEdgeSrc.left);
				m_rcCalcDeco.bottom		= m_rcCalcGauge.bottom;			
			} else {
				SetRect(&m_rcCalcDeco, 0,0,0,0);
			}

		}
		break;
	}
}


void SPWindowGauge::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowGauge::ReposOwnImage()
{
	SetRect(&m_rcCalcFrameHead, 
		m_iAX,
		m_iAY, 
		m_iAX + m_rcFrameHeadSrc.right - m_rcFrameHeadSrc.left, 
		m_iAY + m_rcFrameHeadSrc.bottom - m_rcFrameHeadSrc.top);

	SetRect(&m_rcCalcFrameBody,  
		m_iAX + m_rcFrameHeadSrc.right - m_rcFrameHeadSrc.left, 
		m_iAY, 
		m_iAX + m_iSX - (m_rcFrameTailSrc.right - m_rcFrameTailSrc.left), 
		m_iAY + m_rcFrameHeadSrc.bottom - m_rcFrameHeadSrc.top);

	SetRect(&m_rcCalcFrameTail, 
		m_rcCalcFrameBody.right,
		m_rcCalcFrameBody.top, 
		m_iAX + m_iSX, 
		m_rcCalcFrameBody.bottom);
}

bool SPWindowGauge::SetImageFrameHead	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	m_pImageFrameHead		= pImage;

	m_rcFrameHeadSrc.left	= iXSrc;
	m_rcFrameHeadSrc.top	= iYSrc;
	m_rcFrameHeadSrc.right	= iX2Src;
	m_rcFrameHeadSrc.bottom	= iY2Src;
	ReposOwnImage();
	return true;
}

bool SPWindowGauge::SetImageFrameBody	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	m_pImageFrameBody		= pImage;

	m_rcFrameBodySrc.left	= iXSrc;
	m_rcFrameBodySrc.top	= iYSrc;
	m_rcFrameBodySrc.right	= iX2Src;
	m_rcFrameBodySrc.bottom	= iY2Src;
	ReposOwnImage();
	return true;
}

bool SPWindowGauge::SetImageFrameTail	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	m_pImageFrameTail		= pImage;

	m_rcFrameTailSrc.left	= iXSrc;
	m_rcFrameTailSrc.top	= iYSrc;
	m_rcFrameTailSrc.right	= iX2Src;
	m_rcFrameTailSrc.bottom	= iY2Src;
	ReposOwnImage();
	return true;
}

void SPWindowGauge::SetFlicker( float fPercent )
{
	m_fFlickerPercent	=	fPercent;
}

bool SPWindowGauge::SetImageFlickerEdge( const char* szImage , int iXSrc, int iYSrc, int iX2Src, int iY2Src )
{
	SPTexture* pImage = NULL;
	std::string strResource;

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	SAFE_RELEASE(m_pImageFlickerEdge);

	m_pImageFlickerEdge			= pImage;

	m_rcFlickerEdgeSrc.left		= iXSrc;
	m_rcFlickerEdgeSrc.top		= iYSrc;
	m_rcFlickerEdgeSrc.right	= iX2Src;
	m_rcFlickerEdgeSrc.bottom	= iY2Src;
	return true;
}

bool SPWindowGauge::SetImageFlickerGauge( const char* szImage , int iXSrc, int iYSrc, int iX2Src, int iY2Src )
{
	SPTexture* pImage = NULL;
	std::string strResource;

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	SAFE_RELEASE(m_pImageFlickerGauge);

	m_pImageFlickerGauge		= pImage;

	m_rcFlickerGaugeSrc.left	= iXSrc;
	m_rcFlickerGaugeSrc.top		= iYSrc;
	m_rcFlickerGaugeSrc.right	= iX2Src;
	m_rcFlickerGaugeSrc.bottom	= iY2Src;
	return true;
}

bool SPWindowGauge::SetImageFlickerDeco( const char* szImage , int iXSrc, int iYSrc, int iX2Src, int iY2Src )
{
	SPTexture* pImage = NULL;
	std::string strResource;

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	SAFE_RELEASE(m_pImageFlickerDeco);

	m_pImageFlickerDeco			= pImage;

	m_rcFlickerDecoSrc.left		= iXSrc;
	m_rcFlickerDecoSrc.top		= iYSrc;
	m_rcFlickerDecoSrc.right	= iX2Src;
	m_rcFlickerDecoSrc.bottom	= iY2Src;
	return true;
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowGauge )	
	SPIMESSAGE_COMMAND( SPIM_SET_MAXVALUE	,	OnSetMaxValue	)
	SPIMESSAGE_COMMAND( SPIM_SET_CURVALUE	,	OnSetCurValue	)	
	SPIMESSAGE_COMMAND( SPIM_REFRESH		,	OnRefresh		)	
SPIMESSAGE_OBJ_MAP_END( SPWindow )

int SPWindowGauge::OnSetMaxValue	( WPARAM wParam, LPARAM lParam )
{	
	m_iMaxValue = (INT64)wParam;
	Refresh();
	return 0;
}

int SPWindowGauge::OnSetCurValue	( WPARAM wParam, LPARAM lParam )
{
	m_iCurValue = (INT64)wParam;
	Refresh();
	return 0;
}

int SPWindowGauge::OnRefresh( WPARAM wParam, LPARAM lParam )
{
	Refresh();
	return 0;
}
















