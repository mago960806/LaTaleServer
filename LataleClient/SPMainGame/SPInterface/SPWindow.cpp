


#include <vector>
#include <string>
#include <algorithm>


#include "SPCommon.h"
#include "SPUtil.h"

#include "SPGameObjectDEF.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPWindow.h"


struct _IsEqualWindowInstance : public binary_function<SPWindow*, SPWindow*, bool> {
	bool operator() ( SPWindow* pLeftWindow, SPWindow* pRightWindow) const
	{
		if(pLeftWindow->GetInstanceID() == pRightWindow->GetInstanceID()) { 
			return true;
		}
		return false;
	}
} IsEqualWindowInstance;





SPWindow::SPWindow(WND_ID WndClassID, INSTANCE_ID InstanceID, SPWindow* pParent)
: m_WndClassID		( WndClassID )
, m_InstanceID		( InstanceID )
, m_pParent			( pParent )
//, m_bFontShadow		( true )
, m_WndType			( WND_TYPE_NULL )
{
	Init();
}

SPWindow::SPWindow(WND_ID WndClassID, INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)	
: m_WndClassID		( WndClassID )
, m_InstanceID		( InstanceID )
, m_pParent			( pParent )
, m_WndType			( WND_TYPE_NULL )
{	
	Init();
	AdjustCoord(iX, iY, iCX, iCY);
}

SPWindow::~SPWindow()
{
	Clean();
}

void SPWindow::InitFromXML(SPWindow_Base* rec_wnd_AttrMap)
{
	CHAR* sz_FileName	= UI_STRING(rec_wnd_AttrMap,GET_SPWINDOW_IMAGE_FILENAME);
	INT	  Size			= static_cast<INT>(strlen(sz_FileName));

	for(int  i = 0;i < Size;++i)
	{
        if(sz_FileName[i] == '\\') sz_FileName[i] = '/';
	}
	
	SetImage	(sz_FileName,UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_IMAGE_X),UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_IMAGE_Y));
	SetRectSize	(UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_REALSIZE_X),UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_REALSIZE_Y));
	SetSrcSize	(UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_CX),UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_CY));
	SetFormat	(UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_ALIGN)); 
	SetFont		(FONT_ENUMERATE(UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_FONTTYPE)));


	D3DXCOLOR	d3dxcolor_Temp(UI_FLOAT(rec_wnd_AttrMap,GET_SPWINDOW_FONT_R),
		UI_FLOAT(rec_wnd_AttrMap,GET_SPWINDOW_FONT_G),
		UI_FLOAT(rec_wnd_AttrMap,GET_SPWINDOW_FONT_B),
		UI_FLOAT(rec_wnd_AttrMap,GET_SPWINDOW_FONT_A));

	SetFontColor(d3dxcolor_Temp);
}

INSTANCE_ID SPWindow::GetWidnowInstanceID()
{
	return m_InstanceID;
}

void SPWindow::SetWidnowInstanceID(INSTANCE_ID InstanceID)
{
	m_InstanceID = InstanceID;
}

void SPWindow::Init()
{
	m_pWindowTabStop	= NULL;
	m_pWindowEnterFocus = NULL;

	m_pImageNormal	= NULL;	
	m_ppImageActive	= &m_pImageNormal;

	m_bEnableToolTip	= false;
	m_bHideEnable		= true; 
	m_bShowEnable		= true;
	m_strToolTip.clear();
	m_WindowState = WND_STATE_NULL;
	
	m_bShow		= false;
	m_bEnable	= false;
	m_bTwinkle	= false;
	m_bFocus	= false;	
	m_bTabStop	= false;
	m_bMultiLine = false;
	m_iLineInterval = 0;
	m_bLightMap = false;
	m_bMoveable = false;
	m_bMove		= false;

	m_iAX = 0;
	m_iAY = 0;
	m_iRX = 0;
	m_iRY = 0;
	m_iSX = 0;
	m_iSY = 0;
	m_iSrcX = 0 ;
	m_iSrcY = 0 ;
	m_iNormalSrcX = 0 ;
	m_iNormalSrcY = 0 ;
	m_iSrcSX = 0;
	m_iSrcSY = 0;

	m_ptMoveStart.x = 0;
	m_ptMoveStart.y = 0; 

	m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);

	//m_bFontShadow = false;	
	m_FontEffect = SPFONT_NULL;
	m_FontColor = D3DXCOLOR(0,0,0,1);
	m_FontShadowColor = D3DXCOLOR(0,0,0,0.7f);
	m_dwAlign	= DT_VCENTER | DT_CENTER; /* | DT_WORDBREAK; */
	m_FontSize	= FONT_SIZE_MEDIUM;
	m_pFont = g_pVideo->GetFont(FONT_12_NORMAL);

	//m_bTextPeriod = false;
	m_bTextPeriod = true; // ...이 기본

	m_flg_NullWindow = 0;
	m_flg_Flip_LR	 = 0;
	m_flg_Flip_TB	 = 0;


	if(m_pParent) m_pParent->AddChild(this);	
}

void SPWindow::SetWindowType(WND_TYPE WndType)
{
	m_WndType = WndType;
}

void SPWindow::SetFont( FONT_ENUMERATE fontEnum )
{
	m_pFont = g_pVideo->GetFont( fontEnum );
}

void SPWindow::Clean()
{
	SAFE_RELEASE( m_pImageNormal	);	

	m_strWindowText.clear();

	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); Iter++)
	{
		g_pInterfaceManager->PreDeleteWindow(*Iter);
		delete (*Iter);
	}
	m_vpChildWindow.clear();

	m_vpWindowTabStop.clear();
}

bool SPWindow::CleanChild(SPWindow* pWindow, bool bChild)
{
	bool bDelete;

	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); Iter++)
	{
		if( (*Iter) == pWindow )
		{
			g_pInterfaceManager->PreDeleteWindow(*Iter);

			delete (*Iter);
			m_vpChildWindow.erase(Iter);
			return true;
		}

		if( bChild )
		{
			bDelete = (*Iter)->CleanChild(pWindow, bChild);
			if( bDelete )
				return true;
		}
	}

	return false;
}

void SPWindow::CleanChildAll()
{
	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); Iter++)
	{
		g_pInterfaceManager->PreDeleteWindow(*Iter);
		delete (*Iter);
	}
	m_vpChildWindow.clear();
}

void SPWindow::AdjustCoord( int iX, int iY, int iCX, int iCY)
{	
	if(m_pParent && m_pParent->GetClassID() == WNDID_ROOT )	{ 
		// Parent window.
		SetAbsCoord(iX, iY);
		SetRelCoord(0, 0);		
		RefreshChildPos();

	} else {
		// Control window.
		SetRelCoord(iX, iY);
		int iPX,iPY;
		m_pParent->GetAbsCoord(iPX, iPY);
		SetAbsCoord(iX+iPX, iY+iPY);
	}

	// window size
	if(iCX != -1 && iCY != -1)
		SetRectSize(iCX, iCY);
}

void SPWindow::SetWindowRelPos( int ix,	int iy )
{
	AdjustCoord(ix, iy);
}

void SPWindow::GetAbsCoord( int& iX, int& iY)
{
	iX = m_iAX;
	iY = m_iAY;
}
void SPWindow::GetRelCoord( int& iX, int& iY)
{
	iX = m_iRX;
	iY = m_iRY;
}

void SPWindow::SetAbsCoord( int iX, int iY )
{
	m_iAX = iX;
	m_iAY = iY;
}

void SPWindow::SetRelCoord( int iX, int iY )
{
	m_iRX = iX;
	m_iRY = iY;
}

void SPWindow::RefreshRelationCoord()
{
	if(GetWrapWindow()->IsMoveable() == false)
		return;

	int iPX,iPY;
	//GetWrapWindow()->GetAbsCoord(iPX, iPY);
	
	if(this->IsWrapWindow()) {
		GetAbsCoord(iPX, iPY);
	} else {
		m_pParent->GetAbsCoord(iPX, iPY);
	}

	m_iAX = iPX + m_iRX;
	m_iAY = iPY + m_iRY;

	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); ++Iter) {
		(*Iter)->RefreshRelationCoord();
	}
}

void SPWindow::RefreshChildPos()
{
	//	RefreshRelationCoord 랑 동일함 단 무브 체크 안함. AJJIYA  [6/14/2006]

	int iPX,iPY;

	if(this->IsWrapWindow()) {
		GetAbsCoord(iPX, iPY);
	} else {
		m_pParent->GetAbsCoord(iPX, iPY);
	}

	m_iAX = iPX + m_iRX;
	m_iAY = iPY + m_iRY;

	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); ++Iter) {
		(*Iter)->RefreshChildPos();
	}
}


void SPWindow::RefreshChildPos(REFRESHCHILD_METHOD rec_Method)
{
	switch(rec_Method)
	{
	case REFRESHCHILD_METHOD_NORMAL:
		{
			RefreshChildPos();
		}
		break;
	case REFRESHCHILD_METHOD_WITHOUTME:
		{
			std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
			for(;Iter != m_vpChildWindow.end(); ++Iter) {
				(*Iter)->RefreshChildPos();
			}
		}
		break;
	
	default:
		break;
	}
}


void SPWindow::SetRenderingMethod(RENDERING_METHOD rec_Method,bool rec_Flags)
{
	switch(rec_Method)
	{
	case RENDERING_METHOD_FLIP_LR:
		(*m_ppImageActive)->SetLRSwap(rec_Flags);
		m_flg_Flip_LR = rec_Flags;
		break;
	case RENDERING_METHOD_FLIP_TB:
		(*m_ppImageActive)->SetTBSwap(rec_Flags);
		m_flg_Flip_TB = rec_Flags;
		break;
	default:
		break;
	}

		/*
#ifdef _DEBUG
		DXUTOutputDebugString("SPWindow::RenderingMethod - Nothing has been changed");
#endif
		break;
	}

#ifdef _DEBUG
	DXUTOutputDebugString("SPWindow::RenderingMethod - Selected : %d / Flags : %d\n",rec_Method,rec_Flags);
#endif
	*/

}

void SPWindow::SetFormat(DWORD dwFormat)
{
	m_dwAlign = dwFormat;
}

void SPWindow::SetFontColor(D3DXCOLOR FontColor)
{
	m_FontColor = FontColor;
}

void SPWindow::SetFontShadowColor(D3DXCOLOR FontColor)
{
	m_FontShadowColor = FontColor;
}

void SPWindow::SetFontShadow(bool bFontShadow)
{
	//m_bFontShadow = bFontShadow;
	if(bFontShadow)
		m_FontEffect = SPFONT_SHADOW;
	else 
		m_FontEffect = SPFONT_NULL;
}

void SPWindow::SetFontEdge(bool bEdge)
{
	if(bEdge)
		m_FontEffect = SPFONT_EDGE;
	else 
		m_FontEffect = SPFONT_NULL;

}

void SPWindow::SetFontEffect(SPFONT_EFFECT FontEffect)
{
	m_FontEffect = FontEffect;
}

SPFONT_EFFECT SPWindow::GetFontEffect()
{
	return m_FontEffect;
}


bool SPWindow::SetEnableTabStop(bool bTabStop)
{ 
	m_bTabStop = bTabStop;
	SPWindow* pWindow = GetWrapWindow();
	if(pWindow == NULL) {
		m_bTabStop = false;
		return false;
	}

	if(m_bTabStop)
		pWindow->InsertTabStop(this);
	else 
		pWindow->EraseTabStop(this);

	return true;
}

bool SPWindow::InsertTabStop(SPWindow* pWindow)
{ 	
	if(pWindow == NULL)
		return false;
		
	if( pWindow->IsTabStopWindow() == false)
		return false;	

	m_vpWindowTabStop.push_back(pWindow);
	return true;
}

bool SPWindow::EraseTabStop(SPWindow* pWindow)
{ 	
	if(pWindow == NULL)
		return false;

	if( pWindow->IsTabStopWindow() == false)
		return false;	

	std::vector<SPWindow*>::iterator Iter = m_vpWindowTabStop.begin();
	for(;Iter != m_vpWindowTabStop.end(); ++Iter) {
		if( (*Iter)->GetInstanceID() == pWindow->GetInstanceID() &&
			(*Iter)->GetClassID() == pWindow->GetClassID()	) {
				m_vpWindowTabStop.erase(Iter);
				return true;
		}
	}	
	return false;
}

void SPWindow::ClearTabStopWindow()
{
	m_vpWindowTabStop.clear() ;
}


void SPWindow::SetRectSize( int iSX, int iSY )
{
	m_iSX = iSX;
	m_iSY = iSY;
}

void SPWindow::SetSrcSize(int isx, int isy)
{
	m_iSrcSX = isx;
	m_iSrcSY = isy;
}

void SPWindow::GetWindowRect( RECT& Rect )
{	
	Rect.top	= m_iAY;
	Rect.left	= m_iAX;
	Rect.bottom	= m_iAY + m_iSY;
	Rect.right	= m_iAX + m_iSX;	
}

void SPWindow::SetMultiLine(bool bEnable, int iInterval)
{
	m_bMultiLine = bEnable;
	m_iLineInterval = iInterval;
}

void SPWindow::Process(float fTime) 
{
	if( IsWrapWindow() == true) {	
		if( IsMoveable() == true ) {
			if( m_bMove == true) {
				POINT ptStep;
				ptStep.x = g_pInterfaceManager->GetCursorX() - m_ptMoveStart.x;
				ptStep.y = g_pInterfaceManager->GetCursorY() - m_ptMoveStart.y;
				SetAbsCoord(ptStep.x, ptStep.y);
				RefreshRelationCoord();
			}
		}
	}

	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); ++Iter) {
		(*Iter)->Process(fTime);
	}
}

void SPWindow::Render(float fTime) 
{
	if(!m_bShow) return;	

	if(m_ppImageActive != NULL && *m_ppImageActive) {		//[2005/5/18] - jinhee

		g_pVideo->Flush();

		RECT DestRect;
		DestRect.top	= m_iAY;
		DestRect.left	= m_iAX;
		DestRect.bottom	= m_iAY + m_iSY;
		DestRect.right  = m_iAX + m_iSX;

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

		RECT SrcRect;
		if(m_flg_Flip_LR)
		{
			SrcRect.left	= m_iSrcX+iSrcX;
			SrcRect.top		= m_iSrcY;
			SrcRect.right	= m_iSrcX;
			SrcRect.bottom	= m_iSrcY+iSrcY;
		}
		else
		{
			SrcRect.left	= m_iSrcX;
			SrcRect.top		= m_iSrcY;
			SrcRect.right	= m_iSrcX+iSrcX;
			SrcRect.bottom	= m_iSrcY+iSrcY;
		}

		if( m_bLightMap )
			g_pVideo->SetAlphaTexture(true);

		(*m_ppImageActive)->SetColor(m_RGBA);
		(*m_ppImageActive)->RenderTexture(&DestRect, &SrcRect);

		if( m_bLightMap )
			g_pVideo->SetAlphaTexture(false);
		
	}

	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); ++Iter) {
		(*Iter)->Render(fTime);
	}

}

void SPWindow::RenderReverse(float fTime) 
{
	if(!m_bShow) return;

	if(*m_ppImageActive) {		

		g_pVideo->Flush();

		RECT DestRect;
		DestRect.top	= m_iAY;
		DestRect.left	= m_iAX;
		DestRect.bottom	= m_iAY + m_iSY;
		DestRect.right  = m_iAX + m_iSX;

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

		RECT SrcRect;
		if(m_flg_Flip_LR)
		{
			SrcRect.left	= m_iSrcX+iSrcX;
			SrcRect.top		= m_iSrcY;
			SrcRect.right	= m_iSrcX;
			SrcRect.bottom	= m_iSrcY+iSrcY;
		}
		else
		{
			SrcRect.top		= m_iSrcY;
			SrcRect.left	= m_iSrcX;
			SrcRect.bottom	= m_iSrcY+iSrcY;
			SrcRect.right	= m_iSrcX+iSrcX;
		}
		
		if( m_bLightMap )
			g_pVideo->SetAlphaTexture(true);

		(*m_ppImageActive)->SetColor(m_RGBA);
		(*m_ppImageActive)->RenderTexture(&DestRect, &SrcRect);

		if( m_bLightMap )
			g_pVideo->SetAlphaTexture(false);
	}


	std::vector<SPWindow*>::reverse_iterator rIter = m_vpChildWindow.rbegin();
	for(;rIter != m_vpChildWindow.rend(); ++rIter) {
		(*rIter)->Render(fTime);
	}	
}


void SPWindow::RenderReverse(float fTime,int Reserved) 
{
	if(!m_bShow) return;

	if(*m_ppImageActive) {		

		g_pVideo->Flush();

		RECT DestRect;
		DestRect.top	= m_iAY;
		DestRect.left	= m_iAX;
		DestRect.bottom	= m_iAY + m_iSY;
		DestRect.right  = m_iAX + m_iSX;

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

		
		RECT SrcRect;

		if(m_flg_Flip_LR)
		{
			SrcRect.left	= m_iSrcX+iSrcX;
			SrcRect.top		= m_iSrcY;
			SrcRect.right	= m_iSrcX;
			SrcRect.bottom	= m_iSrcY+iSrcY;
		}
		else
		{
			SrcRect.top		= m_iSrcY;
			SrcRect.left	= m_iSrcX;
			SrcRect.bottom	= m_iSrcY+iSrcY;
			SrcRect.right	= m_iSrcX+iSrcX;
		}
		
		if( m_bLightMap )
			g_pVideo->SetAlphaTexture(true);

		(*m_ppImageActive)->SetColor(m_RGBA);
		(*m_ppImageActive)->RenderTexture(&DestRect, &SrcRect);

		if( m_bLightMap )
			g_pVideo->SetAlphaTexture(false);
	}


	std::vector<SPWindow*>::reverse_iterator rIter = m_vpChildWindow.rbegin();
	for(;rIter != m_vpChildWindow.rend(); ++rIter) 
	{
		if((*rIter)->IsNullWindow())
			(*rIter)->RenderReverse(fTime,0); else
			(*rIter)->Render(fTime);
	}	
}


void SPWindow::RenderTextDebug()
{
#ifdef _DEBUG
	if(!m_strDebugText.empty())	{

		m_pFont->SetColor( D3DXCOLOR( 0,0,1,1 ));		
		m_pFont->RenderTextPos(m_iAX+10, m_iAY+10, m_strDebugText.c_str() );
	}		

	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); ++Iter) {
		(*Iter)->RenderTextDebug();
	}	

#endif
}

SIZE SPWindow::GetSizeText()
{
	SIZE tSize = {0,0};
	if(m_pFont)
		tSize = m_pFont->GetSize( m_strWindowText.c_str() );

	return tSize;
}

void SPWindow::RenderText()
{
	//////////////////////////////////////////////////////////////////////////	
	//  Render Text Only Current Window..!! 
	//
	if(m_pFont == NULL)
		return;

	if(!m_strWindowText.empty()) {
		RECT TarRect;
		bool bTemp = m_pFont->IsShadow();

		GetWindowRect(TarRect);
		m_pFont->SetColor(	m_FontColor );
		//m_pFont->SetShadow(	m_bFontShadow);
		m_pFont->SetEffect( m_FontEffect );

		m_pFont->SetPeriod(m_bTextPeriod);
		m_pFont->RenderText(m_strWindowText.c_str(), &TarRect, m_dwAlign);

		m_pFont->SetColor();
		m_pFont->SetShadow(	bTemp );
		m_pFont->SetPeriod(false);
	}
}


void SPWindow::Show(bool bWithChild)
{
	//if(m_bShow) return;
	if( m_bShowEnable == false )
		return ;
	
	if(bWithChild) {
		std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
		for(; Iter != m_vpChildWindow.end(); ++Iter) {
			(*Iter)->Show();
		}
	}	

	if( IsWrapWindow() )
		GetWrapWindow()->InitTabStop();

	if(m_bShow == false && IsMoveable() ) {
		GetParent()->SwapFocusWrap(this);
	}

	m_bShow = true;
}


void SPWindow::InitTabStop()
{
	if(m_vpWindowTabStop.empty() == false) {

		SPWindow* pWindowRet = NULL;
		std::vector<SPWindow*>::iterator Iter = m_vpWindowTabStop.begin();

		for(;Iter!=m_vpWindowTabStop.end(); ++Iter) {
			pWindowRet = (*Iter) ;
			if(pWindowRet->IsShow() && pWindowRet->IsEnable()) {
				pWindowRet = (*Iter);
				break;
			}			
		}

		if(pWindowRet) {
			g_pInterfaceManager->SetFocusWindow( pWindowRet );
			GetWrapWindow()->SetTabStopWindow( pWindowRet );
		}

	}
}


void SPWindow::Hide(bool bSendServer /*= true*/)
{
	//if(!m_bShow) return;

	if(m_bHideEnable == false) {
		return;
	}

	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(; Iter != m_vpChildWindow.end(); ++Iter) {
		(*Iter)->Hide(bSendServer);
	}
	m_bShow = false;
	
	////[2005/9/7] - 중첩으로 사운드가 나와 여기서는 사용할수 없음	
	//if(bSound) {
	//	if(g_pMainGameManager->GetSubGameManager() && g_pMainGameManager->GetSubGameID() == SUBGAME_ZONE) 
	//		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	//}
	//m_bFocus = false;
}

void SPWindow::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(; Iter != m_vpChildWindow.end(); ++Iter) {
		(*Iter)->SetEnable(bEnable);
	}
}

bool SPWindow::IsEnable()
{
	return m_bEnable;
}

void SPWindow::SetShowEnable(bool bEnable, bool bUpdate/*=true*/)
{
	m_bShowEnable = bEnable; 
	if( bUpdate )	
		(bEnable) ? Show() : Hide() ;
}


void SPWindow::Close()
{
	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(; Iter != m_vpChildWindow.end(); Iter++) {
		(*Iter)->Close();
	}
	m_bShow = false;
}

bool SPWindow::IsShow()
{
	return m_bShow;
}

void SPWindow::SetFocus()
{
	m_bFocus = true;	
	if(m_bTabStop) {
		SPWindow* pWindow= GetWrapWindow();
		if( pWindow ) {
			pWindow->SetTabStopWindow(this);
		}
	}
	g_pInterfaceManager->SetFocusWindow(this);
}

void SPWindow::KillFocus()
{ 
	m_bFocus = false; 
}	


void SPWindow::SetImage(SPTexture* pImage, int iSrcX, int iSrcY)
{
	m_pImageNormal = pImage;
	m_iNormalSrcX = m_iSrcX =iSrcX ; 
	m_iNormalSrcY = m_iSrcY =iSrcY ;
}

bool SPWindow::SetImage(const char* szImage, int iSrcX, int iSrcY)
{
	SPTexture* pImage = NULL;	
	std::string strResource;

	//strResource = RES_WORK_RESOURCE_PATH;
	strResource = szImage;

	// 텍스처 로드 실패시 false 반환 [4/12/2006]
	if( g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false )
		return false;

	SetImage(pImage, iSrcX, iSrcY);	

	return true;
}

void SPWindow::SetImagePos(int iSrcX, int iSrcY)
{
	m_iNormalSrcX = m_iSrcX = iSrcX ; 
	m_iNormalSrcY = m_iSrcY = iSrcY ;
}


bool SPWindow::IsCursorIn( int iX, int iY )
{
	if( iX >= m_iAX && iY >= m_iAY && 
		iX <= (m_iAX + m_iSX) && iY <= (m_iAY + m_iSY)	)
		return true;
	return false;
}

bool SPWindow::IsCursorIn ()
{
	if( g_pInterfaceManager->GetCursorX() >= m_iAX && 
		g_pInterfaceManager->GetCursorY() >= m_iAY && 
		g_pInterfaceManager->GetCursorX() <= (m_iAX + m_iSX) && 
		g_pInterfaceManager->GetCursorY() <= (m_iAY + m_iSY))
		return true;
	return false;
}

void SPWindow::SetWindowText(const char* pStr)
{
	if(!pStr) {
		m_strWindowText.clear();		
	} else {
		m_strWindowText = pStr;

		const char szTemp3[3] = { 92, 110, '\0' }; // '\','n' 을 '\n'으로 바뀌기 위해~~

		int iLine = 1;
		int iFind = 0;
		int iCurPos = 0;
		
		while ( iCurPos != std::string::npos ) {				
			iCurPos = m_strWindowText.find(szTemp3, iFind);
			if(iCurPos != std::string::npos ) {
				m_strWindowText[iCurPos] = ' ';
				m_strWindowText[iCurPos+1] = '\n';
				iFind = iCurPos + 2;
				++iLine;
			}
		} 

	}	
}

void SPWindow::SetToolTipStr(const char* pStr)
{
	if(!pStr) {
		m_strToolTip.clear();		
		m_bEnableToolTip = false;
	} else {
		m_strToolTip = pStr;
		m_bEnableToolTip = true;
	}
}

void SPWindow::AddChild(SPWindow* pWindow)
{
	if(pWindow) m_vpChildWindow.push_back(pWindow);
}

SPWindow* SPWindow::GetWrapWindow()
{
	SPWindow* pMotherParent = this;
	SPWindow* pParent = this;
	while( pMotherParent && pMotherParent->GetClassID() != WNDID_ROOT ) {
		pParent = pMotherParent;
		pMotherParent = pMotherParent->GetParent();
	}
	return pParent;
}

bool SPWindow::IsWrapWindow()
{
	if( this == GetWrapWindow() )
		return true;

	return false;
}

bool SPWindow::SwapFocusWrap(SPWindow* pWindow)
{
	//if(GetClassID() != WNDID_ROOT)
	//	return false;

	if(GetInstanceID() != WIID_SUBGAMEZONE) // 존에서만 쓴다..
		return false;

	if(pWindow->IsWrapWindow() == false) 
		return false;

	//if(pWindow->IsMoveable() == false)
	//	return false;


	std::vector<SPWindow*>::iterator Iter;

	// 1st SPWindowError
	// 2nd SPWindowWorldMap
	// 3rd SPWindowToolTip
	// 4th SPWindowNoticeBox		
#define NUM_NOTMOVE 4

	Iter = m_vpChildWindow.begin();
	for(; Iter != m_vpChildWindow.begin() + NUM_NOTMOVE ; ++Iter) {
		if((*Iter)->GetInstanceID() == pWindow->GetInstanceID() &&
			(*Iter)->GetClassID() == pWindow->GetClassID() ) {
			return false;
		}
	}

	Iter = std::find_if(m_vpChildWindow.begin(), m_vpChildWindow.end(), std::bind2nd(IsEqualWindowInstance, pWindow));
	if(Iter == m_vpChildWindow.end())
		return false;
	
	if(m_vpChildWindow.size() > NUM_NOTMOVE) {
		//std::iter_swap(Iter, m_vpChildWindow.begin() + (NUM_NOTMOVE-1));

		SPWindow* pWindowFront;
		pWindowFront = (*Iter);
		m_vpChildWindow.erase(Iter);
		m_vpChildWindow.insert(m_vpChildWindow.begin() + NUM_NOTMOVE, pWindowFront);
				
		return true;
	} else {
		assert(0 && "Not created windows..");
	}		
	return false;
}

void SPWindow::SetMoveable(bool bMoveable)
{
	m_bMoveable = bMoveable;
}

bool SPWindow::IsMoveable()
{
	return m_bMoveable;
}

bool SPWindow::IsMoveOn()
{
	return m_bMove;
}

SPWindow* SPWindow::Find(INSTANCE_ID iID, bool bSubWindow )
{
	SPWindow* pWindow = NULL;
	
	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); ++Iter) {
		if((*Iter)->m_InstanceID == iID) 
			return (*Iter);

		if(bSubWindow) { // Warning..!! Recursive Call
			if( pWindow = (*Iter)->Find(iID, bSubWindow) ) {
				return pWindow;
			}
		}
	}
	return NULL;
}

SPWindow* SPWindow::NextTabStop()
{
	int iSize = (int)m_vpWindowTabStop.size();
	if(iSize == 0)
		return NULL;

	SPWindow* pWindowRet = NULL;

	std::vector<SPWindow*>::iterator Iter = m_vpWindowTabStop.begin();
	if(m_pWindowTabStop == NULL) {
		m_pWindowTabStop = *(m_vpWindowTabStop.begin());
	}

	for(; Iter != m_vpWindowTabStop.end() ; ++Iter) {
		if( (*Iter)->GetInstanceID() == m_pWindowTabStop->GetInstanceID() &&
			(*Iter)->GetClassID() == m_pWindowTabStop->GetClassID()	) {				
				break;
		}
	}

	// 포커스 가능한 다음 탭스탑을 찾는다
	int iFindCount = 0;
	do {
		if(Iter != m_vpWindowTabStop.end()) {
			++Iter;
			if( Iter == m_vpWindowTabStop.end() )
				Iter = m_vpWindowTabStop.begin();
		}
		else	{
			Iter = m_vpWindowTabStop.begin();
		}

		pWindowRet = (*Iter);

		++iFindCount;
		if(iFindCount >= m_vpWindowTabStop.size()) { // 가능한게 없으면 NULL
			pWindowRet = NULL;
			break;
		}
	} while(!(pWindowRet->IsShow() && pWindowRet->IsEnable()));


	if(pWindowRet) {
		g_pInterfaceManager->SetFocusWindow( pWindowRet );
		GetWrapWindow()->SetTabStopWindow( pWindowRet );
	}

	return pWindowRet;
}

SPWindow* SPWindow::PrevTabStop()
{
	int iSize = (int)m_vpWindowTabStop.size();
	if(iSize == 0)
		return NULL;
	
	SPWindow* pWindowRet = NULL;

	std::vector<SPWindow*>::reverse_iterator RIter = m_vpWindowTabStop.rbegin();	
	if(m_pWindowTabStop == NULL) {
		m_pWindowTabStop = *(m_vpWindowTabStop.rbegin());
	}

	for(; RIter != m_vpWindowTabStop.rend() ; ++RIter) {
		if( (*RIter)->GetInstanceID() == m_pWindowTabStop->GetInstanceID() &&
			(*RIter)->GetClassID() == m_pWindowTabStop->GetClassID()	) {				
				break;
		}
	}

	// 포커스 가능한 이전 탭스탑을 찾는다
	int iFindCount = 0;
	do {
		if(RIter != m_vpWindowTabStop.rend()) {
			++RIter;
			if( RIter == m_vpWindowTabStop.rend() )
				RIter = m_vpWindowTabStop.rbegin();
		}
		else	{
			RIter = m_vpWindowTabStop.rbegin();
		}

		pWindowRet = (*RIter);

		++iFindCount;
		if(iFindCount >= m_vpWindowTabStop.size()) { // 가능한게 없으면 NULL
			pWindowRet = NULL;
			break;
		}
	} while(!(pWindowRet->IsShow() && pWindowRet->IsEnable()));

	if(pWindowRet) {
		g_pInterfaceManager->SetFocusWindow( pWindowRet );
		GetWrapWindow()->SetTabStopWindow( pWindowRet );
	}

	return pWindowRet;


}

void SPWindow::SetEnterFocusWindow(SPWindow* pWindow)
{ 
	assert( pWindow ? pWindow->GetClassID() == WNDID_CTRL_BUTTON : true );
	m_pWindowEnterFocus = pWindow;
}

SPWindow* SPWindow::GetEnterFocusWindow()
{
	return m_pWindowEnterFocus;
}

int	SPWindow::SPSendMessage( SPIM Spim, WPARAM wParam, LPARAM lParam, SPWindow* pSender )
{
	int nID = 0;
	if ( pSender != NULL ) nID = pSender->GetInstanceID();
	return SPIMESSAGE_OBJ_FUNC( Spim, nID, wParam, lParam );
}

int SPWindow::SPSendMessageToChild( SPIM Spim, WPARAM wParam, LPARAM lParam, SPWindow* pSender )
{
	int nID = 0;
	if( pSender != NULL ) {
		SPWindow* pParent = pSender->GetParent();
		while( pParent != NULL ) {
			if( 1 == pParent->SPSendMessage( Spim, wParam, lParam, pSender ) )
				return 1;
			pParent = pParent->GetParent();
		}		
	}
	return 0;
}

int SPWindow::SPSendMessageToParent( SPIM Spim, WPARAM wParam, LPARAM lParam, SPWindow* pSender )
{
	int nID = 0;
	if( pSender != NULL ) {
		SPWindow* pParent = pSender->GetParent();
		while( pParent != NULL ) {
			if( 1 == pParent->SPSendMessage( Spim, wParam, lParam, pSender ) )
				return 1;
			pParent = pParent->GetParent();
		}
	}
	return 0;
}

bool SPWindow::SPSendMessage( SPWindow* pSender, SPIM SpimID, WPARAM wParam, LPARAM lParam )
{
	int nID = 0;
	
	if ( pSender != NULL ) 
		nID = pSender->GetInstanceID();
	
	if ( this == pSender )
		nID = 0;

	if ( SPIMESSAGE_OBJ_FUNC( SpimID, nID, wParam, lParam ) != -1 )  return true;	
	
	if ( m_pParent == NULL ) return false;
	return m_pParent->SPSendMessage( pSender, SpimID, wParam, lParam );	
}


int SPWindow::PerformMouseMove(int iX, int iY)
{
	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end() ; ++Iter) {
		if( (*Iter)->IsShow() && (*Iter)->PerformMouseMove(iX, iY))
			return 1;
	}

	if(IsShow() && IsCursorIn(iX, iY)) {
		g_pInterfaceManager->SetCursorWindow(this);
		return 1;
	}
	return 0;
}

int SPWindow::PerformMouseLDown(int iX, int iY)
{
	return 0;
}

int SPWindow::PerformMouseLUp(int iX, int iY)
{
	return 0;
}

int SPWindow::PerformMouseRDown(int iX, int iY)
{
	return 0;
}

int SPWindow::PerformMouseRUp(int iX, int iY)
{
	return 0;
}

int SPWindow::PerformMouseWheelUp( int iDelta )
{
	return 0;
}

int SPWindow::PerformMouseWheelDown( int iDelta )
{
	return 0;
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindow )
	SPIMESSAGE_COMMAND(SPIM_CURSOR_ENTER,	OnCursorEnter	)
	SPIMESSAGE_COMMAND(SPIM_CURSOR_OUT,		OnCursorOut		)
	SPIMESSAGE_COMMAND(SPIM_KEY_TAB,		OnTab			)

	SPIMESSAGE_COMMAND(SPIM_SETFOCUS	,	OnSetFocus		)	
	SPIMESSAGE_COMMAND(SPIM_MOVE_BEGIN	,	OnBeginMove		)
	SPIMESSAGE_COMMAND(SPIM_MOVE_END	,	OnEndMove		)
	SPIMESSAGE_COMMAND(SPIM_MOVE		,	OnMove			)
SPIMESSAGE_OBJ_MAP_END_BASE()

int SPWindow::OnNULL( WPARAM, LPARAM )
{
	return 0;
}

int SPWindow::OnTab( WPARAM wParam, LPARAM lParam)
{
	if(!wParam) {
		NextTabStop();
	} else {
		PrevTabStop();
	}
	return 0;
}

int SPWindow::OnHide (WPARAM, LPARAM )
{
	Hide();
	return 1;
}

int SPWindow::OnCursorEnter( WPARAM, LPARAM )
{
	return 0;
}

int SPWindow::OnCursorOut( WPARAM wparam, LPARAM lparam)
{
	if(IsWrapWindow() == false) {
		GetWrapWindow()->SPSendMessage( SPIM_CURSOR_OUT, wparam, lparam);
		return 0;
	}

	if( IsMoveable() == false ) {
		return 0;
	}

	//m_bMove = false;

	return 0;
}

int SPWindow::OnSetFocus( WPARAM wparam, LPARAM lparam)
{
	if(IsWrapWindow() == false) {
		GetWrapWindow()->SPSendMessage( SPIM_SETFOCUS, wparam, lparam);
		return 0;
	}

	GetParent()->SwapFocusWrap(this);
	return 0;
}


int SPWindow::OnBeginMove( WPARAM wparam, LPARAM lparam)
{
	if(IsWrapWindow() == false) {
		GetWrapWindow()->SPSendMessage( SPIM_MOVE_BEGIN , wparam, lparam);
		return 0;
	}

	if( IsMoveable() == false ) {
		g_pInterfaceManager->SetMoveWindow(NULL);
		return 0;
	}


	GetParent()->SwapFocusWrap(this);
	//g_pInterfaceManager->SetMoveWindow(this);
	m_bMove = true;

	int iX, iY;
	GetAbsCoord(iX, iY);
	
	m_ptMoveStart.x = g_pInterfaceManager->GetCursorX() - iX;
	m_ptMoveStart.y = g_pInterfaceManager->GetCursorY() - iY;

	return 0;
}

int SPWindow::OnEndMove( WPARAM wparam, LPARAM lparam)
{
	if(IsWrapWindow() == false) {
		GetWrapWindow()->SPSendMessage( SPIM_MOVE_END , wparam, lparam);
		return 0;
	}

	if( IsMoveable() == false ) {
		return 0;
	}

	//g_pInterfaceManager->SetMoveWindow(NULL);

	m_bMove = false;
	return 0;
}

int SPWindow::OnMove( WPARAM wparam, LPARAM lparam)
{
	if(IsWrapWindow() == false) {
		GetWrapWindow()->SPSendMessage( SPIM_MOVE , wparam, lparam);
		return 0;
	}

	if( IsMoveable() == false ) {
		return 0;
	}

	if( IsMoveOn() == false) {
		return 0;
	}

	GetParent()->SwapFocusWrap(this);

	POINT ptStep;
	ptStep.x = g_pInterfaceManager->GetCursorX() - m_ptMoveStart.x;
	ptStep.y = g_pInterfaceManager->GetCursorY() - m_ptMoveStart.y;

	SetAbsCoord(ptStep.x, ptStep.y);
	RefreshRelationCoord();
	return 0;

}









