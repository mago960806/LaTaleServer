
#include <vector>
#include <string>

#include <stdio.h>


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
#include "SPWindowSlider.h"

SPWindowSlider::SPWindowSlider(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent, WND_ID CtrlType )
: SPWindow(CtrlType, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pImageHandle(NULL)
, m_iPageSize (1)
, m_bPush(false)
, m_iRangeValue(1) 
, m_iHandleHalfSize(2)
, m_pImageHandleHit(NULL)
, m_pImageHandleDisable(NULL)
, m_bRenderHandle(false)
{
	ZeroMemory(&m_DestRect,			sizeof(RECT));
	ZeroMemory(&m_SrcRect,			sizeof(RECT));
	ZeroMemory(&m_rcSrcHandleNormal,		sizeof(RECT));
	ZeroMemory(&m_rcTarHandle,	sizeof(RECT));
	ZeroMemory(&m_rcSrcHandleDisable,		sizeof(RECT));	
	ZeroMemory(&m_rcSrcHandlePush,	sizeof(RECT));
	ZeroMemory(&m_rcSrcPageShadow,	sizeof(RECT));
	ZeroMemory(&m_rcSrcPageNormal	,sizeof(RECT));	
	ZeroMemory(&m_rcSrcPageHit		,sizeof(RECT));

	Init();
}

SPWindowSlider::~SPWindowSlider()
{
	Clean();
}

void SPWindowSlider::Init()
{
	m_strToolTip = "[SPWindowSlider]";
	m_DestRect.left = m_iAX;
	m_DestRect.top	= m_iAY;
	m_DestRect.right = m_DestRect.left + m_iSX;
	m_DestRect.bottom = m_DestRect.top + m_iSY;

	m_iRangeValue = 10;
	m_iCurValue = 0;

	// default
	m_WndType = WND_TYPE_TOPBASE;
	
	m_bOnlyDrag = FALSE;

	m_pImageHandleHit	=	NULL;
	m_prcDestActive		=	NULL;
	m_prcActiveSrcHandle		=	NULL;
	m_prcActiveSrcPage	=	NULL;

	m_bWheelMouseUse	=	TRUE;

	m_pImageHandleHit	=	NULL;
	m_pImageHandleDisable	=	NULL;
	m_pImageHandlePush	=	NULL;
	m_pImagePageShadow	=	NULL;

	m_pImagePageNormal	=	NULL;
	m_pImagePageHit		=	NULL;
	m_pImagePageDisable	=	NULL;

	ZeroMemory(&m_rcCalcTargetPageShadow, sizeof(RECT));
	m_bRenderPageShadow = false;	
	m_bEnable = true;

	Refresh();
}

void SPWindowSlider::Clean()
{
	SPWindow::Clean();
	SAFE_RELEASE(m_pImageHandle			);
	SAFE_RELEASE(m_pImageHandleHit		);
	SAFE_RELEASE(m_pImageHandleDisable	);
	SAFE_RELEASE(m_pImageHandlePush		);
	SAFE_RELEASE(m_pImagePageShadow		);
	SAFE_RELEASE(m_pImagePageNormal		);
	SAFE_RELEASE(m_pImagePageHit		);
}


void SPWindowSlider::SetHandlerRender(bool bRender)
{
	m_bRenderHandle = bRender;
}

void SPWindowSlider::SetImageHandle(const char* pszImage, RECT RectEdge)
{
	SPTexture* pImage = NULL;	
	std::string strResource;

	strResource = pszImage;

	g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage);

	m_pImageHandle	=	pImage;
	m_rcSrcHandleNormal	=	RectEdge;

	switch(m_WndType) {
	case WND_TYPE_TOPBASE:		
	default:
		m_iHandleHalfSize	=  (m_rcSrcHandleNormal.bottom - m_rcSrcHandleNormal.top + 1) / 2;
		break;
	case WND_TYPE_LEFTBASE:
		m_iHandleHalfSize	=  (m_rcSrcHandleNormal.right - m_rcSrcHandleNormal.left + 1) / 2;
		
		int iCurValueX = ((m_DestRect.right - m_DestRect.left) / m_iRangeValue) * m_iCurValue;		
		m_rcTarHandle.left = m_DestRect.left + iCurValueX - m_iHandleHalfSize;
		m_rcTarHandle.right = m_rcTarHandle.left + (m_iHandleHalfSize * 2);
		break;
	}
	Refresh();

	m_ppImageActiveHandle	=	&m_pImageHandle;
	m_prcDestActive			=	&m_rcTarHandle;
	m_prcActiveSrcHandle	=	&m_rcSrcHandleNormal;
}


void SPWindowSlider::Process(float fTime)
{
	//Refresh(); // test

	// Slider는 차일드를 안갖음.
	//SPWindow::Process(fTime);
}

void SPWindowSlider::Render(float fTime)
{
	if(!m_bShow) return;

	// Body
	if( m_ppImageActive && *m_ppImageActive ) {
		g_pVideo->Flush();	
		(*m_ppImageActive)->RenderTexture( &m_DestRect, m_prcActiveSrcPage );
	}

	// PageShadow
	if(m_pImagePageShadow && m_bRenderPageShadow) {
		g_pVideo->Flush();	
		m_pImagePageShadow->RenderTexture( &m_rcCalcTargetPageShadow, &m_rcSrcPageShadow);
	}
	
	// Handler
	if (m_bEnable == false) {
		// When Disable, not render handler
		if(m_bRenderHandle){
			m_pImageHandleDisable->RenderTexture( m_prcDestActive , &m_rcSrcHandleDisable );
		}

	} else {
		if( *m_ppImageActiveHandle && m_prcDestActive != NULL && m_prcActiveSrcHandle != NULL )
		{
			g_pVideo->Flush();	
			(*m_ppImageActiveHandle)->SetColor( m_RGBA );
			(*m_ppImageActiveHandle)->RenderTexture( m_prcDestActive , m_prcActiveSrcHandle );
			(*m_ppImageActiveHandle)->SetColor();
		}
	}

	// Slider는 차일드를 안갖음(안갖아도 될거 같음.. 므흣).
	// Parent 호출안함..
	//SPWindow::Render(fTime);
}

void SPWindowSlider::Refresh()
{	
	//m_rcTarHandle.left		= m_DestRect.left + 1;
	//m_rcTarHandle.right		= m_DestRect.right - 1;
	//m_rcTarHandle.top		= m_DestRect.top + (m_iSY - m_iHandleHalfSize * 2) * m_iCurValue / (m_iRangeValue - m_iPageSize - 1);	
	//m_rcTarHandle.bottom	= m_rcTarHandle.top + m_iHandleHalfSize * 2;		

	// 마지막 페이지를 가르킬때..
	float fDelta;
	if( m_iRangeValue <= m_iPageSize ) {
		fDelta = 0;
	} else {
		int iCurIndicate = m_iCurValue;	
		if(iCurIndicate > m_iRangeValue - m_iPageSize + 1) {
			iCurIndicate = m_iRangeValue - m_iPageSize + 1;
		}
		fDelta = (float)iCurIndicate / (m_iRangeValue - m_iPageSize);
	}

	////
	//m_rcTarHandle.top		= m_DestRect.top + LONG( (m_iSY - m_iHandleHalfSize * 2 ) * fDelta );	
	//m_rcTarHandle.bottom	= m_rcTarHandle.top + m_iHandleHalfSize * 2;	

	//[2005/11/14]
	int iCurValueX = 0;
	switch(m_WndType) {
		case WND_TYPE_TOPBASE:
			m_rcTarHandle.top		= m_DestRect.top + LONG( (m_iSY - m_iHandleHalfSize * 2 ) * fDelta );	
			m_rcTarHandle.bottom	= m_rcTarHandle.top + m_iHandleHalfSize * 2;	
			m_rcTarHandle.left		= m_DestRect.left ;
			m_rcTarHandle.right		= m_DestRect.right;
			break;
		case WND_TYPE_LEFTBASE:
			m_rcTarHandle.left		= m_DestRect.left + LONG( (m_iSX - m_iHandleHalfSize * 2 ) * fDelta );	
			m_rcTarHandle.right	= m_rcTarHandle.left + m_iHandleHalfSize * 2;			
			m_rcTarHandle.top = m_DestRect.top;
			m_rcTarHandle.bottom = m_DestRect.bottom;
			break;

			//iCurValueX = static_cast<int>((m_DestRect.right - m_DestRect.left) * fDelta);
			//m_rcTarHandle.left = m_DestRect.left + iCurValueX - m_iHandleHalfSize;			
			//m_rcTarHandle.right = m_rcTarHandle.left + (m_iHandleHalfSize * 2);
			//m_rcTarHandle.top = m_DestRect.top;
			//m_rcTarHandle.bottom = m_DestRect.bottom;
			//break;
		default:
			break;
	}

	//단지 값만 바뀌었을경우 SPIM_REFRESH만 체크하면 된다 by metalgeni [11/9/2005]
	SPSendMessageToParent(SPIM_REFRESH, m_iCurValue, 0, this);
}


int SPWindowSlider::CalcPos(int iX, int iY)
{
	int iRetPos = 0;

	switch (m_WndType) {
	case WND_TYPE_TOPBASE:
		iRetPos = m_iRangeValue * ( iY - m_iAY) / m_iSY;
		if(iRetPos > (m_iRangeValue - m_iPageSize)) {
			iRetPos = (m_iRangeValue - m_iPageSize);
		}
		if(iRetPos < 0) {
			iRetPos = 0;
		}
		break;

	case WND_TYPE_LEFTBASE:
		iRetPos = m_iRangeValue * ( iX - m_iAX) / m_iSX;
		if(iRetPos > (m_iRangeValue - m_iPageSize)) {
			iRetPos = (m_iRangeValue - m_iPageSize);
		}
		if(iRetPos < 0) {
			iRetPos = 0;
		}
		break;
	}
	return iRetPos;
}

void SPWindowSlider::RefreshRelationCoord()
{
	int iPX,iPY;
	m_pParent->GetAbsCoord(iPX, iPY);

	m_iAX = iPX + m_iRX;
	m_iAY = iPY + m_iRY;

	OnWndPosUpdate();
}

void SPWindowSlider::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
	if(m_bEnable == false) 	{
		
		if( m_pImageHandleDisable ) {
			m_ppImageActiveHandle = &m_pImageHandleDisable;
			m_prcActiveSrcHandle	= &m_rcSrcHandleDisable;
		}	

		if( m_pImagePageDisable ) {
			m_ppImageActive =	&m_pImagePageDisable;
			m_prcActiveSrcPage =  &m_rcSrcPageDisable;
		}

	} else {		
		if( m_pImageHandle ) {
			m_ppImageActiveHandle = &m_pImageHandle;
			m_prcActiveSrcHandle	= &m_rcSrcHandleNormal;
		}

		if( m_pImagePageNormal ) {
			m_ppImageActive =	&m_pImagePageNormal;
			m_prcActiveSrcPage =  &m_rcSrcPageNormal;
		}
	}

	SPWindow::SetEnable(bEnable);
}

bool SPWindowSlider::CursorPageMove( bool bUp, bool bOff)
{
	if(bOff == true) {
		ZeroMemory(&m_rcCalcTargetPageShadow, sizeof(RECT));
		m_bRenderPageShadow = false;
		return true;
	} 

	RECT rcWindow;
	GetWindowRect(rcWindow);

	switch(m_WndType) {
	case WND_TYPE_TOPBASE:
		{
			if(bUp) {
				SetRect(&m_rcCalcTargetPageShadow,
					rcWindow.left , rcWindow.top , m_rcTarHandle.right, m_rcTarHandle.top);
			} else {
				SetRect(&m_rcCalcTargetPageShadow,
					m_rcTarHandle.left , m_rcTarHandle.top , rcWindow.right, rcWindow.bottom);
			}			
		}
		break;
	case WND_TYPE_LEFTBASE:
		{
			if(bUp) {
				SetRect(&m_rcCalcTargetPageShadow,
					rcWindow.left , rcWindow.top , m_rcTarHandle.right, m_rcTarHandle.top);
			} else {
				SetRect(&m_rcCalcTargetPageShadow,
					m_rcTarHandle.left , m_rcTarHandle.top , rcWindow.right, rcWindow.bottom);
			}
		}
		break;
	};
	m_bRenderPageShadow = true;

	return true;
}



SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowSlider )	
	SPIMESSAGE_COMMAND( SPIM_SET_MAXVALUE	,	OnSetMaxValue	)
	SPIMESSAGE_COMMAND( SPIM_SET_CURVALUE	,	OnSetCurValue	)	

	SPIMESSAGE_COMMAND( SPIM_REFRESH		,	OnRefresh		)

	SPIMESSAGE_COMMAND( SPIM_CURSOR_MOVE	,	OnCursorMove	)
	SPIMESSAGE_COMMAND( SPIM_CURSOR_ENTER	,	OnCursorEnter	)
	SPIMESSAGE_COMMAND( SPIM_CURSOR_OUT		,	OnCursorOut		)

	SPIMESSAGE_COMMAND( SPIM_LBUTTON_DOWN	,	OnLButtonDown	)
	SPIMESSAGE_COMMAND( SPIM_LBUTTON_UP		,	OnLButtonUp		)

	SPIMESSAGE_COMMAND( SPIM_SLIDER_PAGEUP	,	OnPageUp		)
	SPIMESSAGE_COMMAND( SPIM_SLIDER_PAGEDOWN,	OnPageDown		)
	SPIMESSAGE_COMMAND( SPIM_SLIDER_PAGESIZE,	OnSetPageSize	)
	SPIMESSAGE_COMMAND( SPIM_SLIDER_CURPAGE,	OnGetPage		)
	
	SPIMESSAGE_COMMAND( SPIM_SLIDER_UP		,	OnScrollUp		)			
	SPIMESSAGE_COMMAND( SPIM_SLIDER_DOWN	,	OnScrollDown	)

	SPIMESSAGE_COMMAND( SPIM_WHEEL_UP		,	OnWheelUp		)			
	SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN		,	OnWheelDown		)

	SPIMESSAGE_COMMAND( SPIM_KILLFOCUS,			OnKillFocus		)

	SPIMESSAGE_COMMAND( SPIM_POS_UPDATE		,	OnWndPosUpdate	)	// AJJIYA [9/8/2005]

	SPIMESSAGE_COMMAND( SPIM_MOVE_BEGIN	,		OnNULL			)	//  [5/24/2006] Ignore mouse move
	SPIMESSAGE_COMMAND( SPIM_MOVE_END	,		OnNULL			)
	SPIMESSAGE_COMMAND( SPIM_MOVE		,		OnNULL			)
	
SPIMESSAGE_OBJ_MAP_END( SPWindow )


int SPWindowSlider::OnSetMaxValue	( WPARAM wParam, LPARAM lParam )
{	
	m_iRangeValue = (int)wParam;
	Refresh();	
	return 0;
}

int SPWindowSlider::OnSetCurValue	( WPARAM wParam, LPARAM lParam )
{
	m_iCurValue = (int)wParam;
	Refresh();
	return 0;
}

int SPWindowSlider::OnRefresh( WPARAM wParam, LPARAM lParam )
{
	Refresh();
	return 0;
}

int SPWindowSlider::OnCursorMove	( WPARAM wParam, LPARAM lParam )
{
	if(m_bEnable == false) return 0;

	int iXPos = LOWORD(lParam);
	int iYPos = HIWORD(lParam);

	if(m_bPush == true) {
		int iPos = CalcPos( iXPos, iYPos );
		if(m_iCurValue != iPos) {
			int iOldValue = m_iCurValue;
			m_iCurValue = iPos;
			Refresh();		
			return SPSendMessageToParent(SPIM_SET_CURVALUE, m_iCurValue, iOldValue, this);
		}
	} else {
		RECT rcWindow;
		GetWindowRect(rcWindow);

		// Handle Hittest OK
		if( m_rcTarHandle.left <= iXPos && m_rcTarHandle.right >= iXPos &&
			m_rcTarHandle.top	<= iYPos && m_rcTarHandle.bottom >= iYPos )	{
			
			if( m_pImageHandleHit ) {
				m_ppImageActiveHandle	=	&m_pImageHandleHit;
				m_prcActiveSrcHandle	=	&m_rcSrcHandleHit;
			}
			if( m_pImagePageHit ) {
				m_ppImageActive =	&m_pImagePageHit;
				m_prcActiveSrcPage =  &m_rcSrcPageHit;
			}
		// Window Hittest OK
		} else if( rcWindow.left	<= iXPos && rcWindow.right >= iXPos && 
				rcWindow.top	<= iYPos && rcWindow.bottom >= iYPos ) {
			
			if( m_pImageHandle && m_bPush == false ) {
				m_ppImageActiveHandle	=	&m_pImageHandle;
				m_prcActiveSrcHandle	=	&m_rcSrcHandleNormal;
			}
			if( m_pImagePageHit ) {
				m_ppImageActive =	&m_pImagePageHit;
				m_prcActiveSrcPage =  &m_rcSrcPageHit;
			}
		//Not hit
		} else {
			if( m_pImageHandle && m_bPush == false ) {
				m_ppImageActiveHandle	=	&m_pImageHandle;
				m_prcActiveSrcHandle	=	&m_rcSrcHandleNormal;
			}
			if( m_pImagePageNormal ) {
				m_ppImageActive =	&m_pImagePageNormal;
				m_prcActiveSrcPage =  &m_rcSrcPageNormal;
			}
		}
	}
	return SPSendMessageToParent(SPIM_CURSOR_MOVE, wParam, lParam, this);		
}

int SPWindowSlider::OnCursorEnter	( WPARAM wParam, LPARAM lParam )
{
	if(m_bEnable == false) return 0;

	//Enter처리는 모두 Move 에서
	return SPSendMessageToParent(SPIM_CURSOR_ENTER, 0, 0, this);	
}

int SPWindowSlider::OnCursorOut		( WPARAM wParam, LPARAM lParam )
{
	if(m_bEnable == false) return 0;

	if( m_pImageHandle && m_bPush == false )
	{
		m_ppImageActiveHandle	=	&m_pImageHandle;
		m_prcActiveSrcHandle	=	&m_rcSrcHandleNormal;

		m_ppImageActive			=	&m_pImagePageNormal;
		m_prcActiveSrcPage		=	&m_rcSrcPageNormal;
	}
	
	return SPSendMessageToParent(SPIM_CURSOR_OUT, wParam, lParam, this);	
}

int SPWindowSlider::OnLButtonDown	( WPARAM wParam, LPARAM lParam )
{
	if(m_bEnable == false) return 0;

	int iXPos = LOWORD(lParam);
	int iYPos = HIWORD(lParam);

	switch(m_WndType) {
	case WND_TYPE_TOPBASE:
		if(m_rcTarHandle.top > iYPos) {
			if( m_bOnlyDrag == FALSE ) {
				OnPageUp(0,0);		//  Call SendMessageToParent
				CursorPageMove(true, false);
			}
			else
				return 1;
		} else if(m_rcTarHandle.bottom < iYPos) {
			if( m_bOnlyDrag == FALSE ) {
				OnPageDown(0,0);	//  Call SendMessageToParent
				CursorPageMove(false, false);
			} else
				return 1;
		} else {
			m_bPush = true;
		}
		break;
	case WND_TYPE_LEFTBASE:		
		if(m_rcTarHandle.left > iXPos) {		
			if( m_bOnlyDrag == FALSE ) { 
				OnPageUp(0,0);		//  Call SendMessageToParent
				CursorPageMove(true, false);
			} else
				return 1;
		} 
		else if(m_rcTarHandle.right < iXPos) {
			if( m_bOnlyDrag == FALSE ) {
				OnPageDown(0,0);	//  Call SendMessageToParent
				CursorPageMove(false, false);
			} else
				return 1;
		} else {
			m_bPush = true;
		}
		break;
	};

	if( m_pImageHandleHit ) {
		m_ppImageActiveHandle	=	&m_pImageHandleHit;
		m_prcActiveSrcHandle	=	&m_rcSrcHandleHit;
	}

	if( m_pImagePageHit ) {
		m_ppImageActive			=	&m_pImagePageHit;
		m_prcActiveSrcPage		=	&m_rcSrcPageHit;
	}

	return SPSendMessageToParent(SPIM_LBUTTON_DOWN, wParam, lParam, this);
}

int SPWindowSlider::OnLButtonUp		( WPARAM wParam, LPARAM lParam )
{
	if(m_bEnable == false) return 0;

	m_bPush = false;
	CursorPageMove(false, true);

	if(IsCursorIn()) {
		if( m_pImageHandle ) {
			m_ppImageActiveHandle	=	&m_pImageHandleHit;
			m_prcActiveSrcHandle	=	&m_rcSrcHandleHit;
		}

		if(m_pImagePageNormal) {	
			m_ppImageActive			=	&m_pImagePageHit;
			m_prcActiveSrcPage		=	&m_rcSrcPageHit;
		}
	} else {
		if( m_pImageHandle ) {
			m_ppImageActiveHandle	=	&m_pImageHandle;
			m_prcActiveSrcHandle	=	&m_rcSrcHandleNormal;
		}

		if(m_pImagePageNormal) {	
			m_ppImageActive			=	&m_pImagePageNormal;
			m_prcActiveSrcPage		=	&m_rcSrcPageNormal;
		}
	}


	return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, lParam, this);
}

int SPWindowSlider::OnKillFocus		( WPARAM wParam, LPARAM lParam )
{
	if(m_bEnable == false) return 0;

	m_bPush = false;
	CursorPageMove(false, true);

	if( m_pImageHandle ) {
		m_ppImageActiveHandle	=	&m_pImageHandle;
		m_prcActiveSrcHandle	=	&m_rcSrcHandleNormal;
	}

	if(m_pImagePageNormal) {	
		m_ppImageActive			=	&m_pImagePageNormal;
		m_prcActiveSrcPage		=	&m_rcSrcPageNormal;
	}

	SPSendMessageToParent( SPIM_KILLFOCUS , wParam , lParam , this );
	return 0;
}

int SPWindowSlider::OnPageUp		( WPARAM wParam, LPARAM lParam )
{
	if(m_bEnable == false) return 0;

	m_iCurValue -= m_iPageSize;
	if(m_iCurValue < 0) {
		m_iCurValue = 0;
	}
	Refresh();
	return SPSendMessageToParent(SPIM_SLIDER_PAGEUP, m_iCurValue, 0, this);
}

int SPWindowSlider::OnPageDown		( WPARAM wParam, LPARAM lParam )
{
	if(m_bEnable == false) return 0;

	m_iCurValue += m_iPageSize;
	if(m_iCurValue > (m_iRangeValue - m_iPageSize)) {
		m_iCurValue = (m_iRangeValue - m_iPageSize);
	}
	if(m_iCurValue < 0) {
		m_iCurValue = 0;
	}
	Refresh();
	return SPSendMessageToParent(SPIM_SLIDER_PAGEDOWN, m_iCurValue, 0, this);
}

int SPWindowSlider::OnScrollUp			( WPARAM wParam, LPARAM lParam )
{
	if(m_bEnable == false) return 0;

	--m_iCurValue;
	if(m_iCurValue < 0) {
		m_iCurValue = 0;
	}
	Refresh();
	return SPSendMessageToParent(SPIM_SLIDER_UP, m_iCurValue, 0, this);
}

int SPWindowSlider::OnScrollDown	( WPARAM wParam, LPARAM lParam )
{
	if(m_bEnable == false) return 0;

	++m_iCurValue;
	if(m_iCurValue > (m_iRangeValue - m_iPageSize)) {
		m_iCurValue = (m_iRangeValue - m_iPageSize);
	}
	if(m_iCurValue < 0) {
		m_iCurValue = 0;
	}
	Refresh();
	return SPSendMessageToParent(SPIM_SLIDER_DOWN, m_iCurValue, 0, this);
}

int SPWindowSlider::OnWheelUp			( WPARAM wParam, LPARAM lParam )
{
	if(m_bEnable == false) return 0;

	if( m_bWheelMouseUse == TRUE )
		OnScrollUp();

	return SPSendMessageToParent( SPIM_WHEEL_UP , wParam , lParam , this );
}

int SPWindowSlider::OnWheelDown			( WPARAM wParam, LPARAM lParam )
{
	if(m_bEnable == false) return 0;

	if( m_bWheelMouseUse == TRUE )
		OnScrollDown();

	return SPSendMessageToParent( SPIM_WHEEL_DOWN , wParam , lParam , this );
}

int SPWindowSlider::OnSetPageSize	( WPARAM wParam, LPARAM lParam )
{
	m_iPageSize = (int)wParam;
	return 0;
}

int SPWindowSlider::OnGetPage	( WPARAM wParam, LPARAM lParam )
{
	return SPSendMessageToParent(SPIM_SLIDER_CURPAGE, m_iCurValue, 0, this);
}

void SPWindowSlider::SetCurpage(int iPage /*= 0*/)
{
	m_iCurValue = iPage;
}

int	SPWindowSlider::GetCurPage()
{
	return m_iCurValue;
}

void SPWindowSlider::SetDrag( BOOL bDrag )
{
	m_bOnlyDrag	=	bDrag;
}

void SPWindowSlider::SetColor( DWORD rgba )
{
	m_RGBA	=	rgba;
}

DWORD SPWindowSlider::GetColor()
{
	return m_RGBA;
}

RECT SPWindowSlider::GetHandleTarget()
{
	return m_rcTarHandle;
}

void SPWindowSlider::SetImageHandleHit( const char* pszImageHit , RECT rcSrc )
{
	if( g_pVideo->GetTextureMgr()->LoadTexture( pszImageHit , &m_pImageHandleHit ) == false )
		return;	

	m_rcSrcHandleHit			=	rcSrc;
}

void SPWindowSlider::SetImageHandleRect( RECT RectEdge )
{
	m_rcSrcHandleNormal	=	RectEdge;
}

void SPWindowSlider::SetImageHandleDisable( const char* pszImageHit , RECT rcSrc )
{
	if( g_pVideo->GetTextureMgr()->LoadTexture( pszImageHit , &m_pImageHandleDisable ) == false )
		return;

	m_rcSrcHandleDisable = rcSrc;
}

void SPWindowSlider::SetImageHandlePush( const char* pszImage, RECT rcSrc )
{
	if( g_pVideo->GetTextureMgr()->LoadTexture( pszImage , &m_pImageHandlePush ) == false )
		return;
	m_rcSrcHandlePush = rcSrc;
}

void SPWindowSlider::SetImagePageShadow( const char* pszImage, RECT rcSrc )
{
	if( g_pVideo->GetTextureMgr()->LoadTexture( pszImage , &m_pImagePageShadow ) == false )
		return;

	m_rcSrcPageShadow = rcSrc;
}

void SPWindowSlider::SetImagePageNormal	( const char* pszImage, RECT rcSrc )
{
	if( g_pVideo->GetTextureMgr()->LoadTexture( pszImage , &m_pImagePageNormal) == false )
		return;

	 m_rcSrcPageNormal = rcSrc;
}

void SPWindowSlider::SetImagePageHit	( const char* pszImage, RECT rcSrc )
{
	if( g_pVideo->GetTextureMgr()->LoadTexture( pszImage , &m_pImagePageHit) == false )
		return;
	m_rcSrcPageHit = rcSrc;	
}

void SPWindowSlider::SetImagePageDisable	( const char* pszImage, RECT rcSrc )
{
	if( g_pVideo->GetTextureMgr()->LoadTexture( pszImage , &m_pImagePageDisable) == false )
		return;
	m_rcSrcPageDisable = rcSrc;	
}

void SPWindowSlider::SetImageHandleHitRect( RECT rcSrc )
{
	m_rcSrcHandleHit	=	rcSrc;
}

void SPWindowSlider::SetWheelMouseUse( BOOL bUse )
{
	m_bWheelMouseUse	=	bUse;
}

int SPWindowSlider::OnWndPosUpdate(WPARAM wParam, LPARAM lParam )
{
	// AJJIYA [9/8/2005]
	// 윈도우 포지션 이동시 슬라이더도 옮기자
	m_DestRect.left = m_iAX;
	m_DestRect.top	= m_iAY;
	m_DestRect.right = m_DestRect.left + m_iSX;
	m_DestRect.bottom = m_DestRect.top + m_iSY;
	Refresh();
	return 1;
}



