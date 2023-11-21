#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowError.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPAbuseFilter.h"

using namespace std;

//------------------------------------------------------------------------------------
SPWindowError::SPWindowError(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowError::SPWindowError(INSTANCE_ID InstanceID,
	int iX, int iY, int iCX, int iCY, SPWindow* pParent)
	: SPWindow(WNDID_LOBBY_SERVER, InstanceID, iX, iY, iCX, iCY, pParent)	
							 
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowError::~SPWindowError()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowError::Clean()
{
	m_vErrorMsg.clear();
	SPWindow::Clean();
}

//------------------------------------------------------------------------------------
void SPWindowError::Init()
{
	SPWindowStatic* pkSkinWindow;
	SPWindowButton* pkButton;

	pkSkinWindow = new SPWindowStatic(WIID_ERROR_SKIN_UPPER, 0, 0, 254, 29, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1, 15);

	pkSkinWindow = new SPWindowStatic(WIID_ERROR_SKIN_LOWER, 0, 58, 254, 29, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1, 50);

	pkSkinWindow = new SPWindowStatic(WIID_ERROR_SKIN_MIDDLE, 0, 29, 254, 29, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1, 46);
	pkSkinWindow->SetSrcSize(254, 2);

	pkButton = new SPWindowButton(WIID_ERROR_OK, 91, 8, 68, 17, pkSkinWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 443, 241);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI101.PNG", 443, 260);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI101.PNG", 443, 279);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI101.PNG", 443, 298);
	
	m_iErrorType = 0;
}

//------------------------------------------------------------------------------------
void SPWindowError::SetErrorMsg(const char* pszErrorMsg, int iType)
{
	m_iErrorType = iType;

	m_vErrorMsg.clear();
	if( pszErrorMsg == NULL )
		return;

	SPFont* pFont;
	if( g_pVideo == NULL || (pFont = g_pVideo->GetFont()) == NULL )
		return;

	int iFontHeight = (int)pFont->GetHeight();

	RECT	rcDest;
	SetRect(&rcDest, 0, 0, 245, iFontHeight);

	char szLineFeedBuf[MAX_PATH];
	SPAbuseFilter::GetInstance()->ReplaceLineFeed(pszErrorMsg, szLineFeedBuf, MAX_PATH);
	pFont->ConvertMultiLineW(szLineFeedBuf, rcDest, m_vErrorMsg);

	int iMsgCount = (int)m_vErrorMsg.size();
	if( iMsgCount == 0 )
		return;

	int iSize = (iFontHeight + 1) * iMsgCount + 23;

	SPWindow* pkWindow = Find(WIID_ERROR_SKIN_MIDDLE, true);
	if( pkWindow )
	{
		pkWindow->AdjustCoord(0, 29, 254, iSize);
		pkWindow->RefreshRelationCoord();
		SPWindow* pkChildWindow = pkWindow->Find(WIID_ERROR_OK, true);
		if( pkChildWindow )
		{
			pkChildWindow->AdjustCoord(91, iSize - 12);
			pkChildWindow->RefreshRelationCoord();
		}
	}

	pkWindow = Find(WIID_ERROR_SKIN_LOWER, true);
	if( pkWindow )
	{
		pkWindow->AdjustCoord(0, iSize + 29);
		pkWindow->RefreshRelationCoord();
	}
}

//------------------------------------------------------------------------------------
void SPWindowError::Show(bool bWithChild)
{
}

//------------------------------------------------------------------------------------
void SPWindowError::Process(float fTime)
{
	if( !m_bShow )
		return;
}

//------------------------------------------------------------------------------------
void SPWindowError::Render(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Render(fTime);
	if( g_pVideo == NULL )
		return;

	if( g_pVideo->GetFont() == NULL )
		return;

	vector< string >::iterator iter = m_vErrorMsg.begin();
	if( iter == m_vErrorMsg.end() )
		return;

	int iFontHeight = (int)g_pVideo->GetFont()->GetHeight();
	bool bShadow = g_pVideo->GetFont()->IsShadow();

	RECT rcErrorRgn;
	SetRect(&rcErrorRgn, 280, 235, 534, 235 + iFontHeight);
	g_pVideo->GetFont()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	g_pVideo->GetFont()->SetShadow(false);

	while( iter != m_vErrorMsg.end() )
	{
		g_pVideo->GetFont()->RenderText((*iter).c_str(), &rcErrorRgn, DT_VCENTER | DT_CENTER);

		rcErrorRgn.top += iFontHeight;
		rcErrorRgn.bottom += iFontHeight;
		++iter;
	}

	g_pVideo->GetFont()->SetColor();
	g_pVideo->GetFont()->SetShadow(bShadow);
}

//------------------------------------------------------------------------------------
// Message

int SPWindowError::OnErrorMsg(WPARAM wParam, LPARAM lParam)
{
	SetErrorMsg((const char*)wParam, lParam);
	SPWindow::Show();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowError::OnErrorOk(WPARAM wParam, LPARAM lParam)
{
	switch( m_iErrorType )
	{
	case 1:
		Hide();
		if( g_pMainGameManager )
			g_pMainGameManager->ChangeSubGame(SUBGAME_LOGGIN);
		else
			SendMessage(g_hWnd, WM_CLOSE, 1, 1);
		break;
	}

	return 1;
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowError)

SPIMESSAGE_COMMAND(	SPIM_ERROR_SET_MSG,		OnErrorMsg	)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_ERROR_OK,		OnErrorOk	)

SPIMESSAGE_OBJ_MAP_END(SPWindow)
