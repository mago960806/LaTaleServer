
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
#include "SPWindowTypeWriter.h"

#include "SPAbuseFilter.h"

SPWindowTypeWriter::SPWindowTypeWriter(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_CTRL_TYPEWRITER, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}

SPWindowTypeWriter::~SPWindowTypeWriter()
{
	Clean();
}

void SPWindowTypeWriter::Init()
{
	//SetFormat(DT_VCENTER | DT_LEFT );
	SetFormat(DT_LEFT);

	m_pImageBlind = NULL;

	m_fImageScaleX	=	1.0f;
	m_fImageScaleY	=	1.0f;
	//m_iCurTypeLen	=	0;

	m_fTypeDelay	=	0.1f;
	m_fAccmulateTime =	0.0f;
	m_bCenterRender	=	false;
	m_iMargin		= 4;
	m_bTypeWrite	= false;

	m_iCurTextLine	= 0;
	m_iCurTextPos	= 0;

	SetFont(FONT_14_BOLD);	
}

void SPWindowTypeWriter::Clean()
{
	SPWindow::Clean();
	m_vwstrWindowText.clear();

	SAFE_RELEASE(m_pImageBlind);
}

//bool SPWindowTypeWriter::SetImageBlind(const char* szImage, int iXSrc, int iYSrc, int iXLen, int iYLen)
//{
//	SPTexture* pImage = NULL;
//	std::string strResource;
//	
//	strResource = szImage;
//
//	g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage);
//
//	m_pImageBlind	= pImage;
//
//	m_rcBlindSrc.left	= iXSrc;
//	m_rcBlindSrc.top	= iYSrc;
//	m_rcBlindSrc.right		= m_rcBlindSrc.left + iXLen;
//	m_rcBlindSrc.bottom		= m_rcBlindSrc.top	+ iYLen;
//
//	return true;
//}

void SPWindowTypeWriter::SetWindowText(const char* pStr)
{
	SPWindow::SetWindowText(pStr);

	m_fAccmulateTime = 0.0f;

	WCHAR wszTemp[MAX_TYPE_LEN];
	SPAbuseFilter::GetInstance()->ConvertAnsiStringToWide(wszTemp, m_strWindowText.c_str());
	m_wstrWindowText  = wszTemp;	

	// String Align
	if(!m_wstrWindowText.empty()) {	
		GetWindowRect(m_rcWindowRect);
		m_vwstrWindowText.clear();		

		char szLineFeedBuf[MAX_STR_LENGTH];
		SPAbuseFilter::GetInstance()->ReplaceLineFeed(pStr, szLineFeedBuf, MAX_STR_LENGTH);
		m_pFont->ConvertMultiLineW(szLineFeedBuf, m_rcWindowRect,m_vwstrWindowText);

		m_iCurTextLine	= 0;
		m_iCurTextPos	= 0;
	}
}

void SPWindowTypeWriter::Process(float fTime)
{
	if( m_wstrWindowText.empty() )
		return;

	if( m_bTypeWrite )	{
		m_fAccmulateTime += fTime;
		if( m_fAccmulateTime > m_fTypeDelay ) {
			m_fAccmulateTime -= m_fTypeDelay;

			++m_iCurTextPos;
			std::vector<std::wstring>::iterator Iter = 	m_vwstrWindowText.begin() + m_iCurTextLine;			
			if( m_iCurTextPos > (*Iter).length() ) {
				m_iCurTextPos = 1;
				++m_iCurTextLine;
			}

			if(!m_strTypeSound.empty()) {
				g_pInterfaceManager->RegisterSoundUnit(m_strTypeSound.c_str());
			}


			if(m_iCurTextLine >= m_vwstrWindowText.size()) {
			//if(m_iCurTypeLen >= m_wstrWindowText.length()) {
				m_iCurTextLine = m_vwstrWindowText.size() - 1;
				m_iCurTextPos = (*(m_vwstrWindowText.rbegin())).length();

				m_bTypeWrite = false;
				SPSendMessageToParent(SPIM_TYPE_END, 0,0,this);
				//m_iCurTypeLen = 0;
			}
		}
	}
	SPWindow::Process(fTime);
}

void SPWindowTypeWriter::Render(float fTime)
{
	if(!m_bShow) return;

	SPWindow::Render(fTime);

	//if(m_pImageBlind != NULL ) {

#ifdef _DEBUG
	g_pVideo->GetSysLine()->DrawRect(m_rcWindowRect);
#endif

	g_pVideo->Flush();
	RenderText();
	g_pVideo->Flush();

		//std::vector<RECT>::iterator Iter = m_vrcBlindTar.begin();
		//for(; Iter != m_vrcBlindTar.end(); ++Iter) {
		//	m_pImageBlind->RenderTexture(&(*Iter), &m_rcBlindSrc);
		//	//g_pVideo->Flush();
		//	//g_pVideo->GetSysLine()->DrawRect( (*Iter) );
		//}		
	//}
	
}

void SPWindowTypeWriter::RenderText()
{
	if( m_wstrWindowText.empty() )
		return;

	if( g_pVideo == NULL )
		return;

	if( m_pFont == NULL )
		return;

	if( !m_vwstrWindowText.empty()) {

		m_pFont->SetColor( m_FontColor		);
		m_pFont->SetEffect( m_FontEffect );
		m_pFont->SetScale( 1.0f, 1.0f);

		RECT rcLine;		
		std::wstring wstrTemp;
		char szTempBuf[512];
		int iCurLine = 0;
		std::vector<std::wstring>::iterator Iter = m_vwstrWindowText.begin();

		rcLine = m_rcWindowRect;
		rcLine.bottom = rcLine.top + m_pFont->GetHeight();

		for( ;iCurLine < m_iCurTextLine; ++iCurLine  ) {
			if(iCurLine >= m_wstrWindowText.size()) {
				assert(0);
				break;
			}		
			
			SPAbuseFilter::GetInstance()->ConvertWideToAnsiString(szTempBuf, (*Iter).c_str());
			m_pFont->RenderText( szTempBuf, &rcLine, m_dwAlign);			

			rcLine.top += (m_pFont->GetHeight() + m_iMargin);
			rcLine.bottom = rcLine.top + m_pFont->GetHeight();

			++Iter;
		}

		// last line
		if(Iter != m_vwstrWindowText.end()) {
			if(m_iCurTextPos > (*Iter).length()) {
				m_iCurTextPos = (*Iter).length();
			}
			wstrTemp = (*Iter).substr(0,m_iCurTextPos);
			wstrTemp[m_iCurTextPos] = 0;
			SPAbuseFilter::GetInstance()->ConvertWideToAnsiString(szTempBuf, wstrTemp.c_str());
			m_pFont->RenderText( szTempBuf, &rcLine, m_dwAlign);
			wstrTemp.clear();
		}


		//RECT rcLine;
		//std::wstring* pwstrLine;
		//char szTempBuf[512];

		//rcLine = m_rcWindowRect;
		//rcLine.bottom = rcLine.top + m_pFont->GetHeight();

		//std::vector<std::wstring>::iterator IterWindowText = m_vwstrWindowText.begin();
		//for(; IterWindowText != m_vwstrWindowText.end(); ++IterWindowText) {
		//	pwstrLine = &( *IterWindowText );
		//	if(pwstrLine->empty())
		//		break;
		//	
		//	m_pFont->SetScale( 1.0f, 1.0f);
		//	SPAbuseFilter::GetInstance()->ConvertWideToAnsiString(szTempBuf, pwstrLine->c_str());
		//	m_pFont->RenderText( szTempBuf, &rcLine, m_dwAlign);
		//	//m_pFont->RenderText( pstrLine->c_str(), &rcLine, m_dwAlign);

		//	rcLine.top += (m_pFont->GetHeight() + m_iMargin);
		//	rcLine.bottom = rcLine.top + m_pFont->GetHeight();
		//}
	}
}

int SPWindowTypeWriter::GetTextPixelHeight()
{
	if( g_pVideo == NULL )
		return 0;

	if( m_pFont == NULL )
		return 0;

	return GetTextCount() * ( (int)m_pFont->GetHeight() + m_iLineInterval );
}

void SPWindowTypeWriter::SetColor( DWORD rgba )
{
	m_RGBA	=	rgba;
}

DWORD SPWindowTypeWriter::GetColor()
{
	return m_RGBA;
}

void SPWindowTypeWriter::SetImageScaleX( float fScaleX )
{
	m_fImageScaleX	=	fScaleX;
}

void SPWindowTypeWriter::SetImageScaleY( float fScaleY )
{
	m_fImageScaleY	=	fScaleY;
}

float SPWindowTypeWriter::GetIamgeScaleY()
{
	return m_fImageScaleX;
}

float SPWindowTypeWriter::GetImageScaleX()
{
	return m_fImageScaleY;
}

void SPWindowTypeWriter::SetCenterRender( bool bCenter )
{
	m_bCenterRender	=	bCenter;
}

bool SPWindowTypeWriter::GetCenterRender()
{
	return m_bCenterRender;
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowTypeWriter )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnLButtonUp		)	
	SPIMESSAGE_COMMAND( SPIM_SET_MARGIN,		OnSetMargin		)
	
	SPIMESSAGE_COMMAND(	SPIM_TYPE_SPEED	,		OnTypeDelay		)
	SPIMESSAGE_COMMAND(	SPIM_TYPE_CLEAR	,		OnTypeClear		)
	SPIMESSAGE_COMMAND(	SPIM_TYPE_START	,		OnTypeStart		)
	SPIMESSAGE_COMMAND(	SPIM_TYPE_END	,		OnTypeEnd		)
	SPIMESSAGE_COMMAND( SPIM_TYPE_SKIP	,		OnTypeSkip		)
	SPIMESSAGE_COMMAND( SPIM_TYPE_SOUND	,		OnTypeSound		)

	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,		OnLButtonDown	)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnLButtonUP		)
	
SPIMESSAGE_OBJ_MAP_END	( SPWindow )


int SPWindowTypeWriter::OnCursorEnter( WPARAM wParam, LPARAM lParam)
{	
	return SPSendMessageToParent(SPIM_CURSOR_ENTER, wParam, lParam, this);	
}

int SPWindowTypeWriter::OnCursorOut( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_CURSOR_OUT, wParam, lParam, this);
}

int SPWindowTypeWriter::OnLButtonUp( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, lParam, this);
}

int SPWindowTypeWriter::OnSetMargin( WPARAM wParam, LPARAM lParam)
{
	m_iMargin = (int)wParam;
	return 1;
}

int SPWindowTypeWriter::OnTypeSound	( WPARAM wParam, LPARAM lParam)
{
	if(wParam == 0) {
		m_strTypeSound.clear();
	} else {
		m_strTypeSound = (const char*)wParam;
	}
	return 0;
}

int SPWindowTypeWriter::OnTypeDelay	( WPARAM wParam, LPARAM lParam)
{
	m_fTypeDelay = (float)wParam;
	m_fTypeDelay /= 1000.0f;
	return 0;
}

int SPWindowTypeWriter::OnTypeClear		( WPARAM wParam, LPARAM lParam)
{
	m_bTypeWrite = false;
	//m_iCurTypeLen = 0;
	m_iCurTextLine	= 0;
	m_iCurTextPos	= 0;


	return 0;
}

int SPWindowTypeWriter::OnTypeStart		( WPARAM wParam, LPARAM lParam)
{
	m_bTypeWrite = true;

	m_iCurTextLine	= 0;
	m_iCurTextPos	= 0;

	//m_iCurTypeLen = 0;
	m_fAccmulateTime = 0.0f;
	return 0;
}

int SPWindowTypeWriter::OnTypeEnd		( WPARAM wParam, LPARAM lParam)
{
	return 0;
}

int SPWindowTypeWriter::OnTypeSkip		( WPARAM wParam, LPARAM lParam)
{
	//m_bTypeWrite = true;
	//m_iCurTypeLen = (int)m_wstrWindowText.length();

	if(m_vwstrWindowText.empty())
		return 0;

	m_iCurTextLine = m_vwstrWindowText.size() - 1;
	m_iCurTextPos = (*(m_vwstrWindowText.rbegin())).length();


	return SPSendMessageToParent(SPIM_TYPE_SKIP, wParam, lParam, this);
}


int SPWindowTypeWriter::OnLButtonUP		( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, lParam, this);
}

int SPWindowTypeWriter::OnLButtonDown	( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_LBUTTON_DOWN, wParam, lParam, this);
}





