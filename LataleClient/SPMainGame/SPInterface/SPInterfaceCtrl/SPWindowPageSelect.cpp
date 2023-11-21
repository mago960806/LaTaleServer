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

#include "SPInterfaceCoordManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowPageSelect.h"





SPWindowPageSelect::SPWindowPageSelect(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_CTRL_PAGESELECT, InstanceID, iX, iY, iCX, iCY, pParent)
, m_iMaxPage(0)
, m_iBundleUnit(0)
, m_iPage(0)
, m_iBundle(0)
, m_iSelectPage(0)
, m_iViewPage(0)
, m_bShowDebugRect(false)
{
	for(int i = 0; i < PAGE_SELECT_MAX; i++) {
		m_pPageMove[i] = NULL;
		//SetRect(&m_rtPageMove[i], 0, 0, 0, 0);
	}

	for(int i = 0; i < MAX_BUNDLE; i++) {
		m_pNumber[i] = NULL;
	}

	SetRect(&m_rtSelect, 0, 0, 0, 0);	
	
	m_SelectColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	m_iInterval = 1;

	Init();
}


SPWindowPageSelect::~SPWindowPageSelect()
{	
	Clean();
}


void SPWindowPageSelect::Init()
{
	SPWindowStatic* pStatic = NULL;

	for(int i = 0; i < PAGE_SELECT_MAX; i++) {
		m_pPageMove[i] = new SPWindowButton(GetInstanceID() + PAGE_START + i, 0, 0, 0, 0, this);
		m_pPageMove[i]->Show();
	}

	for(int i = 0; i < MAX_BUNDLE; i++) {
		 pStatic = new SPWindowStatic(GetInstanceID() + NUMBER_START + i, 0, 0, 0, 0, this);
		 pStatic->SetTextPeriod(false);
		 pStatic->Show();
		 m_pNumber[i] = pStatic;
		 pStatic = NULL;
	}
}


void SPWindowPageSelect::Clean()
{
	SPWindow::Clean();
}


void SPWindowPageSelect::Process(float fTime)
{
	if(m_bShow == false)	return;
	SPWindow::Process(fTime);
}


void SPWindowPageSelect::Render(float fTime)
{
	if(m_bShow == false)	return;
	SPWindow::Render(fTime);

	//if(m_iPage >= 0 && m_iPage < m_iBundleUnit){
		g_pVideo->GetSysLine()->SetColor(m_SelectColor);
		g_pVideo->GetSysLine()->DrawRect(m_rtSelect);
		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//}		
	

	if( m_bShowDebugRect )	{
		RECT rtRect = {0, 0, 0, 0};
		GetWindowRect(rtRect);
		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		g_pVideo->GetSysLine()->DrawRect(rtRect);
		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// PageSelect
		for(int i = 0; i < PAGE_SELECT_MAX; i++) {
			m_pPageMove[i]->GetWindowRect(rtRect);
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			g_pVideo->GetSysLine()->DrawRect(rtRect);
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	
		for(int i = 0; i < m_iViewPage; i++) {
			m_pNumber[i]->GetWindowRect(rtRect);
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
			g_pVideo->GetSysLine()->DrawRect(rtRect);
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}


void SPWindowPageSelect::Show(bool bWithChild)
{
	////m_iPage				= 0;
	////m_iBundle			= 0;
	////m_iSelectPage		= 0;	
	//m_iViewPage			= 0;
	SPWindow::Show(bWithChild);


	// 필요에따라 위치 이동후 보여준다.
	if(m_iMaxBundle == 1) {									//단위이동보다 페이지의 수가 모자른 경우
		ArrangeControl(m_iBundleUnit);
		m_pPageMove[PAGE_SELECT_BUNDLE_PREV]->SetEnable(false);
		m_pPageMove[PAGE_SELECT_BUNDLE_NEXT]->SetEnable(false);
	}
	else if( m_iMaxBundle-1 == m_iBundle )	{				// 마지막 Bundle Page의 경우
		int iShowPage = ( m_iMaxPage % MAX_BUNDLE ) ;
		ArrangeControl( iShowPage );
	}
	else	{
		ArrangeControl(m_iBundleUnit);

	}
	UpdateContorl();
}


void SPWindowPageSelect::Hide(bool bSendServer)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowPageSelect::SetPageButtonSize(PAGE_SELECT iButton, int iWidth, int iHeight)
{
	if(m_pPageMove[iButton]) {
		//SetRect(&m_rtPageMove[iButton], 0, 0, iWidth, iHeight);
		m_pPageMove[iButton]->SetRectSize(iWidth, iHeight);
	}
}


void SPWindowPageSelect::SetNumberSize(int iWidth, int iHeight)
{
	for(int i = 0; i < MAX_BUNDLE; i++) {
		m_pNumber[i]->SetRectSize(iWidth, iHeight);
	}
}


void SPWindowPageSelect::SetNumberColor(D3DXCOLOR FontColor)
{
	for(int i = 0; i < MAX_BUNDLE; i++) {
		m_pNumber[i]->SetFontColor(FontColor);
	}

	m_SelectColor = FontColor;
}


void SPWindowPageSelect::SetControlInterval(int iInterval)
{
	m_iInterval = iInterval;
}


void SPWindowPageSelect::SetPageButtonImage(PAGE_SELECT iButton, const char* szImage, int iSrcX, int iSrcY)
{
	if(m_pPageMove[iButton])
		m_pPageMove[iButton]->SetImage(szImage, iSrcX, iSrcY);
}


void SPWindowPageSelect::SetPageButtonImageHit(PAGE_SELECT iButton, const char* szImage, int iSrcX, int iSrcY)
{
	if(m_pPageMove[iButton])
		m_pPageMove[iButton]->SetImageHit(szImage, iSrcX, iSrcY);
}


void SPWindowPageSelect::SetPageButtonImagePush(PAGE_SELECT iButton, const char* szImage, int iSrcX, int iSrcY)
{
	if(m_pPageMove[iButton])
		m_pPageMove[iButton]->SetImagePush(szImage, iSrcX, iSrcY);
}


void SPWindowPageSelect::SetPageButtonImageDisable(PAGE_SELECT iButton, const char* szImage, int iSrcX, int iSrcY)
{
	if(m_pPageMove[iButton])
		m_pPageMove[iButton]->SetImageDisable(szImage, iSrcX, iSrcY);
}


void SPWindowPageSelect::SetMaxPageInfo(int iMaxPage, bool bInit/*=true*/, int iBundle /*= MAX_BUNDLE*/)
{
	if( bInit )	{
		m_iPage				= 0;		// 현재 Page (m_iPage + m_iBundle*m_iBundleUnit == m_iSelectPage)
		m_iSelectPage		= 0;		// 현재 선택 Page (총 페이지)
		m_iBundle			= 0;		// 현재 Bundle Count
		m_iMaxBundle		= 0;		// 설정된 최대 Bundle
	}
	
	m_iMaxPage = iMaxPage;
	m_iBundleUnit = iBundle;	

	for(int i = 0; i < MAX_BUNDLE; i++) {
		m_pNumber[i]->Hide();
	}

	m_iMaxBundle = m_iMaxPage / m_iBundleUnit;
	if(m_iMaxPage % m_iBundleUnit)
		m_iMaxBundle++;

	if(m_iMaxBundle == 1) {									//단위이동보다 페이지의 수가 모자른 경우
		//m_iBundleUnit = 0;
		m_iBundleUnit = iMaxPage;
		ArrangeControl(iMaxPage);
		m_pPageMove[PAGE_SELECT_BUNDLE_PREV]->SetEnable(false);
		m_pPageMove[PAGE_SELECT_BUNDLE_NEXT]->SetEnable(false);
	}
	else if( m_iMaxBundle-1 == m_iBundle )	{				// 마지막 Bundle Page의 경우
		int iShowPage = ( m_iMaxPage % iBundle ) ;
		ArrangeControl( iShowPage );
	}
	else	{
		ArrangeControl(m_iBundleUnit);

	}
	
	UpdateContorl();
}


/**
	해당 페이지가 선택된것으로 출력한다.
 */
void SPWindowPageSelect::SetSelectPage(int iPage)
{	
	m_iPage = iPage ;

	if((m_iBundle * m_iBundleUnit + m_iPage) == m_iSelectPage)
		return ;

	m_iSelectPage = m_iBundle * m_iBundleUnit + m_iPage;
	UpdateContorl();
}


void SPWindowPageSelect::ArrangeControl(int iViewPage /*= 0*/)
{
	//m_iViewPage = iViewPage;	
	//int iStartPage = m_iBundle * m_iBundleUnit + m_iPage;
    //if((m_iMaxPage - iStartPage) / m_iBundleUnit)
	m_iViewPage = iViewPage;	
	int i = 0;	
	for(i = 0; i < MAX_BUNDLE; i++)
		m_pNumber[i]->Hide();

	RECT rect;
	m_pNumber[0]->GetWindowRect(rect);
	int iWidth = rect.right - rect.left;
	
	int iNumberWidth = iViewPage * (iWidth + m_iInterval);
	int iNumberStartX = (m_iSX / 2) - (iNumberWidth / 2);
	
	int iStartX = 0;
	int iStartY = 0 ;
	RECT rtWindowRect;
	int iControlWide = 0;
	int iControlHeight = 0;
	
	//이전버튼
	m_pPageMove[PAGE_SELECT_PREV]->GetWindowRect(rtWindowRect);
	iControlWide = rtWindowRect.right - rtWindowRect.left;
	iControlHeight = rtWindowRect.bottom - rtWindowRect.top;
	iStartX = iNumberStartX - iControlWide - m_iInterval - 2;
	iStartY = ((m_iSY / 2) - (iControlHeight / 2)) / 2;
	m_pPageMove[PAGE_SELECT_PREV]->AdjustCoord(iStartX, iStartY);		
	
	m_pPageMove[PAGE_SELECT_BUNDLE_PREV]->GetWindowRect(rtWindowRect);
	iControlWide = rtWindowRect.right - rtWindowRect.left;	
	iControlHeight = rtWindowRect.bottom - rtWindowRect.top;
	iStartX = iStartX - iControlWide - m_iInterval - 2;
	iStartY = ((m_iSY / 2) - (iControlHeight / 2)) / 2;
	m_pPageMove[PAGE_SELECT_BUNDLE_PREV]->AdjustCoord(iStartX, iStartY);
	
	//Number
	m_pNumber[0]->GetWindowRect(rtWindowRect);
	iControlWide = rtWindowRect.right - rtWindowRect.left;	
	iControlHeight = rtWindowRect.bottom - rtWindowRect.top;
	iStartY = ((m_iSY / 2) - (iControlHeight / 2)) / 2;
	iStartX = iNumberStartX;
	int iStartIndex = (m_iBundle * m_iBundleUnit + m_iPage) + 1;
	char szMsg[16];	
	for(int i = 0; i < iViewPage; i++) {
		m_pNumber[i]->AdjustCoord(iStartX, iStartY);
		ZeroMemory(szMsg, 16);
		sprintf(szMsg, "%d", iStartIndex);
		m_pNumber[i]->SetWindowText(szMsg);
		iStartX = iStartX + iControlWide + m_iInterval;
		iStartIndex++;
		m_pNumber[i]->Show();
	}
	
	//다음 버튼
	m_pPageMove[PAGE_SELECT_NEXT]->GetWindowRect(rtWindowRect);
	iControlWide = rtWindowRect.right - rtWindowRect.left;	
	iControlHeight = rtWindowRect.bottom - rtWindowRect.top;
	iStartX = iNumberStartX + iNumberWidth + 2;
	iStartY = ((m_iSY / 2) - (iControlHeight / 2)) / 2;
	m_pPageMove[PAGE_SELECT_NEXT]->AdjustCoord(iStartX, iStartY);
	
	m_pPageMove[PAGE_SELECT_BUNDLE_NEXT]->GetWindowRect(rtWindowRect);
	iControlWide = rtWindowRect.right - rtWindowRect.left;	
	iControlHeight = rtWindowRect.bottom - rtWindowRect.top;
	iStartX = iStartX + iControlWide + 2 + m_iInterval;
	iStartY = ((m_iSY / 2) - (iControlHeight / 2)) / 2;
	m_pPageMove[PAGE_SELECT_BUNDLE_NEXT]->AdjustCoord(iStartX, iStartY);
}


void SPWindowPageSelect::UpdateContorl(int iBundle /*= MAX_BUNDLE*/)
{
	////ArrangeControl(iBundle);	
	//if(m_iBundleUnit == iBundle) {				
	//}
	//else {
	//}

	int iStartIndex = (m_iBundle * m_iBundleUnit) + 1;
	char szMsg[16];	
	for(int i = 0; i < m_iBundleUnit; i++) {
		ZeroMemory(szMsg, 16);
		sprintf(szMsg, "%d", iStartIndex);
		m_pNumber[i]->SetWindowText(szMsg);
		iStartIndex++;
		if(i == m_iPage) {
			m_pNumber[i]->SetFont(FONT_12_BOLD);
			
			RECT rtRect;			
			SIZE szRet = {0, 0};
			m_pNumber[i]->GetWindowRect(rtRect);
			szRet = m_pNumber[i]->GetSizeText();
			int iStartX = (rtRect.right - rtRect.left) / 2 - szRet.cx / 2;
			SetRect(&m_rtSelect, rtRect.left + iStartX - 1, rtRect.bottom - 1, rtRect.left + iStartX + szRet.cx, rtRect.bottom);
		}
		else {
			m_pNumber[i]->SetFont(FONT_12_NORMAL);
		}
	}

	m_pPageMove[PAGE_SELECT_BUNDLE_PREV]->SetEnable(true);
	m_pPageMove[PAGE_SELECT_PREV]->SetEnable(true);
	m_pPageMove[PAGE_SELECT_BUNDLE_NEXT]->SetEnable(true);
	m_pPageMove[PAGE_SELECT_NEXT]->SetEnable(true);

	if(m_iSelectPage == 0) {
		m_pPageMove[PAGE_SELECT_BUNDLE_PREV]->SetEnable(false);
		m_pPageMove[PAGE_SELECT_PREV]->SetEnable(false);
	}
	
	if(m_iSelectPage == m_iMaxPage - 1) {
		m_pPageMove[PAGE_SELECT_BUNDLE_NEXT]->SetEnable(false);
		m_pPageMove[PAGE_SELECT_NEXT]->SetEnable(false);
	}
	//else {
		if(m_iBundle == 0) {
			m_pPageMove[PAGE_SELECT_BUNDLE_PREV]->SetEnable(false);
		}		
		if(m_iBundle == m_iMaxBundle -1) {
			m_pPageMove[PAGE_SELECT_BUNDLE_NEXT]->SetEnable(false);
		}
	//}
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowPageSelect )
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, GetInstanceID() + PAGE_START,  GetInstanceID() + PAGE_START + PAGE_SELECT_MAX,	OnPageClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, GetInstanceID() + NUMBER_START,  GetInstanceID() + NUMBER_START + MAX_BUNDLE,	OnNumberClick)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



int SPWindowPageSelect::OnPageClick(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	int iSelect = uiID - (GetInstanceID() + PAGE_START);
	
	switch(iSelect) {
		case PAGE_SELECT_BUNDLE_PREV:
			m_iBundle--;
			m_iPage = 0;
			break;
		case PAGE_SELECT_PREV:
			m_iPage--;
			
			if(m_iPage < 0){
				m_iBundle--;
				m_iPage = m_iBundleUnit - 1;
			}
			
			break;
		case PAGE_SELECT_NEXT:
			m_iPage++;

			if(m_iPage >= m_iBundleUnit) {
				m_iBundle++;
				m_iPage = 0;
			}
			break;
		case PAGE_SELECT_BUNDLE_NEXT:
			m_iBundle++;
			m_iPage = 0;
			
			break;
	}	
	
	
	if(m_iMaxPage > m_iBundleUnit && m_iBundle == (m_iMaxBundle - 1)) {		
		int iPage = m_iMaxPage % m_iBundleUnit;		
		if(iPage){
			ArrangeControl(iPage);
			
			if(m_iPage >= iPage) {
				m_iPage = iPage - 1;
			}
		}
		else										//나머지가 0인경우 10페이지 단위인경우
			ArrangeControl(m_iBundleUnit);		
	}
	else {
		ArrangeControl(m_iBundleUnit);
	}	


	if((m_iBundle * m_iBundleUnit + m_iPage) == m_iSelectPage)
		return 1;

	m_iSelectPage = m_iBundle * m_iBundleUnit + m_iPage;
	
	UpdateContorl();
	
	if(m_iSelectPage >= 0 && m_iSelectPage < m_iMaxPage) {
		SPSendMessageToParent( SPIM_PAGE , (WPARAM)m_iSelectPage, (LPARAM)m_iBundle, this );
	}
	
	SPSendMessageToParent(SPIM_LBUTTON_UP, m_iSelectPage, m_iBundle, this);
	return 1;
}


int SPWindowPageSelect::OnNumberClick(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{	
	int iSelect = uiID - (GetInstanceID() + NUMBER_START);
	m_iPage = iSelect;
	
	if((m_iBundle * m_iBundleUnit + m_iPage) == m_iSelectPage)
		return 1;

	m_iSelectPage = m_iBundle * m_iBundleUnit + m_iPage;
	UpdateContorl();
	
	if(m_iSelectPage >= 0 && m_iSelectPage < m_iMaxPage) {
		SPSendMessageToParent( SPIM_PAGE , (WPARAM)m_iSelectPage, (LPARAM)m_iBundle, this );
	}
	SPSendMessageToParent(SPIM_LBUTTON_UP, m_iSelectPage, m_iBundle, this);
	return 1;
}
