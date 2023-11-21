

#include "SPCommon.h"

#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowEdit.h"

#include "SPMouseCursor.h"

#include "SPSubGameManager.h"
#include "SPIMEPool.h"
#include "SPInterfaceManager.h"

#include "SPDebug.h"

#include "SPAbuseFilter.h"

extern HWND			g_hWnd;
extern HINSTANCE	g_hInstance;

#define EDIT_BUFSIZE 256

SPWindowEdit::SPWindowEdit( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_CTRL_EDIT, InstanceID, iX, iY, iCX, iCY, pParent )
{
	m_bPassword = false;

	m_bNumber = false;
	m_iwMinValue = LLONG_MIN;	
	m_iwMaxValue = LLONG_MAX;	
	m_iLimitText = 255;
	m_bTwinkleCursor = false;	
	m_fPrevTime = 0;

	m_dwAlign = DT_LEFT | DT_VCENTER;	
	m_bEnable = true;
	m_bSelect = false;

	GetWindowRect(m_rcWindowRect);
	CopyMemory(&m_rcCaretRect, &m_rcWindowRect, sizeof(RECT));	
	CopyMemory(&m_rcCompositionCaret, &m_rcWindowRect, sizeof(RECT));	

	m_pNormalHead	= NULL; 
	m_pNormalBody	= NULL; 
	m_pNormalTail	= NULL; 
	m_pFocusHead	= NULL; 
	m_pFocusBody	= NULL; 
	m_pFocusTail	= NULL; 
	m_pImageDeco	= NULL;

	m_iMarginX		= 0;
	m_iMarginY		= 0;

	m_bEnableIME = true;
}


SPWindowEdit::~SPWindowEdit()
{
	SAFE_RELEASE(m_pNormalHead);
	SAFE_RELEASE(m_pNormalBody);
	SAFE_RELEASE(m_pNormalTail);
	SAFE_RELEASE(m_pFocusHead);
	SAFE_RELEASE(m_pFocusBody);
	SAFE_RELEASE(m_pFocusTail);
}

void SPWindowEdit::Process( float fTime )
{
	m_fPrevTime += fTime;
	if( m_fPrevTime > 0.5f ) {
		m_fPrevTime -= 0.5f;
		m_fPrevTime = fTime;
		m_bTwinkleCursor = !m_bTwinkleCursor;
	}

	WCHAR wszTextBuf[EDIT_BUFSIZE];
	//ZeroMemory( wszTextBuf, sizeof(WCHAR) * EDIT_BUFSIZE );
	WCHAR wszTextBuf2[EDIT_BUFSIZE];
	//ZeroMemory( wszTextBuf2, sizeof(WCHAR) * EDIT_BUFSIZE );

	int nTextLen = 0;

	// Process String
	bool bRefresh = ProcessString(wszTextBuf2);

	// Process Caret
	int iSelS, iSelE;
	if( m_bFocus ) {
		// 선택 영역 & Caret 의 위치
		
		//iSelS = iSelE = -1;
		g_pInterfaceManager->GetIMEPool()->GetSelW( iSelS, iSelE );

		if( iSelS == iSelE ) { // 선택이 아니컖ECaret 의 위치
			m_bSelect = false;
			int nCaretPos = g_pInterfaceManager->GetIMEPool()->GetCaretPosW();
			nTextLen = g_pInterfaceManager->GetIMEPool()->GetStringLenW();

			//DXUTOutputDebugString(" nTextLen == %d \n", nTextLen);
			//DXUTOutputDebugString(" nCaretPos == %d \n", nCaretPos);

			if( nTextLen <= nCaretPos ) {// caret 이 문픸E끝에 오는 경퓖E				
				swprintf( wszTextBuf, L"%s ", wszTextBuf2 );
			} else {
				std::wstring temp = wszTextBuf2;
				swprintf( wszTextBuf, L"%c", wszTextBuf2[nCaretPos] );

				if( temp.length() > 0 ) {
					if(nCaretPos > temp.length()) {
						nCaretPos = temp.length();
					}
					temp.replace( nCaretPos, 1, wszTextBuf );	// temp의 길이보다 nCaretPos가 큰값이 들엉龐다.
				}

				wcscpy( wszTextBuf, temp.data() );
			}				
		} else {
			m_bSelect = true;
			//셀렉트시 
			swprintf( wszTextBuf, L"%s ", wszTextBuf2 );
		}

	} else {
		m_bSelect = false;
		wcscpy( wszTextBuf, wszTextBuf2 );
	}
	m_wstrWindowTextBuf = wszTextBuf;

	if(m_bFocus) {	
		int nCaretPos; 
		int iRange;		

		if(iSelS == iSelE) {
			nCaretPos = g_pInterfaceManager->GetIMEPool()->GetCaretPosW();
			//iRange = (wszTextBuf[nCaretPos+1] & 0x80) ? (nCaretPos+2) : (nCaretPos+1) ;
			if(g_pInterfaceManager->GetIMEPool()->GetCompLenW() == 0) {
				iRange = nCaretPos + 1;
			} else {
				iRange = nCaretPos + g_pInterfaceManager->GetIMEPool()->GetCompLenW();
			}
			
		} else {
			nCaretPos = iSelS;
			if(nCaretPos < 0) 
				nCaretPos = 0;

			iRange = iSelE;
			if(iRange < 0) 
				iRange = 0;
		}


		WCHAR wszText[EDIT_BUFSIZE];
		SIZE SizeCaret1, SizeCaret2;

		wcsncpy(wszText, wszTextBuf, nCaretPos);
		wszText[nCaretPos] = 0;
		SizeCaret1.cx	=	12; // caret begin point
		SizeCaret1.cy	=	12;

		if( g_pVideo != NULL ) {
			if( m_pFont != NULL ) {
				SizeCaret1 = m_pFont->GetSizeW(wszText);
			}
		}

		wcsncpy(wszText, wszTextBuf, iRange);
		if(iRange > 0) {
			if(wszText[iRange-1] == 0) {
				wszText[iRange-1] = 0x20; // space
			}
		}

		wszText[iRange] = 0;
		SizeCaret2.cx	=	12; // caret end point
		SizeCaret2.cy	=	12;

		if( g_pVideo != NULL )
		{
			if( m_pFont != NULL ) {
				SizeCaret2 = m_pFont->GetSizeW(wszText);
			}
		}

		m_sizeCaretLen.cx = SizeCaret1.cx;
		m_sizeCaretLen.cy = SizeCaret2.cx - SizeCaret1.cx; // 커서 길이		
		
		GetWindowRect(m_rcWindowRect);
		
		m_rcTextRect = m_rcWindowRect;
		m_rcTextRect.left += m_iMarginX;
		m_rcTextRect.top += m_iMarginY;		

		if(m_dwAlign & DT_RIGHT) {
			m_rcCaretRect.left = m_rcWindowRect.left;
			m_rcCaretRect.bottom = m_rcWindowRect.bottom;			
			m_rcCaretRect.left += m_sizeCaretLen.cx;
			m_rcCaretRect.right = m_rcCaretRect.left + m_sizeCaretLen.cy;

			if( g_pVideo != NULL )
			{
				if( m_pFont != NULL )
				{
					SizeCaret1 = m_pFont->GetSizeW(wszTextBuf);
					SizeCaret2 = m_pFont->GetSizeW(L" ");
				}
			}

			if(iSelS == iSelE) {
				m_rcCaretRect.left += ((m_rcWindowRect.right - m_rcWindowRect.left) - SizeCaret1.cx );
				m_rcCaretRect.right += ((m_rcWindowRect.right - m_rcWindowRect.left) - SizeCaret1.cx );
			} else {
				m_rcCaretRect.left += ((m_rcWindowRect.right - m_rcWindowRect.left) - SizeCaret1.cx + SizeCaret2.cx);
				m_rcCaretRect.right += ((m_rcWindowRect.right - m_rcWindowRect.left) - SizeCaret1.cx + SizeCaret2.cx);
			}

		} else {

			// DT_LEFT and other..
			m_rcCaretRect.left = m_rcWindowRect.left;
			m_rcCaretRect.bottom = m_rcWindowRect.bottom;
			m_rcCaretRect.left += m_sizeCaretLen.cx;
			m_rcCaretRect.right = m_rcCaretRect.left + m_sizeCaretLen.cy;
		}

		if(iSelS == iSelE) {
			m_rcCaretRect.top = (m_rcCaretRect.bottom -= 2); // 퍊E怠?2도뜨 올려준다.
		} else {
			m_rcCaretRect.bottom = m_rcCaretRect.top = (m_rcWindowRect.top + (m_rcWindowRect.bottom - m_rcWindowRect.top)/2 );
		}

		if(m_iMarginX != 0 || m_iMarginY != 0) {
			OffsetRect(&m_rcCaretRect, m_iMarginX, m_iMarginY/2 - 1);
		}


		// Composition Attr Caret
		const BYTE* pCaretAttr = g_pInterfaceManager->GetIMEPool()->GetCompositionAttr();


		//?Not Select && Use Composition Caret && Exist Composotion Attr?
		if(iSelS == iSelE && g_pInterfaceManager->GetIMEPool()->IsWideCaret() && pCaretAttr[0] != 0) {
			

			CopyMemory(&m_rcCompositionCaret, &m_rcCaretRect, sizeof(RECT));
			OffsetRect(&m_rcCompositionCaret, 0, -1); // attr caret will be under main caret

			// attribute for COMPOSITIONSTRING Structure
			//#define ATTR_INPUT                      0x00
			//#define ATTR_TARGET_CONVERTED           0x01
			//#define ATTR_CONVERTED                  0x02
			//#define ATTR_TARGET_NOTCONVERTED        0x03
			//#define ATTR_INPUT_ERROR                0x04
			//#define ATTR_FIXEDCONVERTED             0x05

			int iCompCaretBegin = -1;
			int iCompCaretEnd = g_pInterfaceManager->GetIMEPool()->GetCompLenW();

			for (int i = 0; i < g_pInterfaceManager->GetIMEPool()->GetCompLenW() ; ++i, ++pCaretAttr) {

				// Find Convert begin
				if(*pCaretAttr == ATTR_TARGET_CONVERTED && iCompCaretBegin == -1) {
					iCompCaretBegin = i;
				}
				// Find Convert end
				if(iCompCaretBegin != -1 && *pCaretAttr == ATTR_CONVERTED) { 
					iCompCaretEnd = i;
					break;
				}
			}

			//assert(iCompCaretBegin != -1);

			if(iCompCaretBegin == -1)
				iCompCaretBegin = iCompCaretEnd;



			std::wstring wstrComp;
			
			wstrComp = g_pInterfaceManager->GetIMEPool()->GetCompStringW();

			wcsncpy( wszTextBuf, wstrComp.c_str(), iCompCaretBegin );
			wszTextBuf[iCompCaretBegin] = 0;
			wcsncpy( wszTextBuf2, wstrComp.c_str(), iCompCaretEnd );
			wszTextBuf2[iCompCaretEnd] = 0;

			SizeCaret2 = m_pFont->GetSizeW(wszTextBuf2);
			m_rcCompositionCaret.right = m_rcCompositionCaret.left + SizeCaret2.cx;
			SizeCaret1 = m_pFont->GetSizeW(wszTextBuf);
			m_rcCompositionCaret.left += SizeCaret1.cx;


		} else {			
			ZeroMemory(&m_rcCompositionCaret, sizeof(RECT));
		}


		// Adjust Caret for Window RECT
		if(m_rcCaretRect.left > m_rcWindowRect.right) 
			m_rcCaretRect.left = m_rcWindowRect.right;

		if(m_rcCaretRect.right > m_rcWindowRect.right) 
			m_rcCaretRect.right = m_rcWindowRect.right;


		// Adjust CompositionCaret for Window RECT
		if(m_rcCompositionCaret.left > m_rcWindowRect.right) 
			m_rcCompositionCaret.left = m_rcWindowRect.right;

		if(m_rcCompositionCaret.right > m_rcWindowRect.right) 
			m_rcCompositionCaret.right = m_rcWindowRect.right;


		//////////////////////////////////////////////////////////////////////////
		// Candidate Window
		SPWindow*	pWindow;
		RECT		rcCand;
		int			iLen;
		int			iPosX, iPosY;

		if(g_pInterfaceManager->GetIMEPool()->IsCandUpdate()) {
			pWindow = g_pInterfaceManager->GetCandVertical();
			if(pWindow) {
				pWindow->GetWindowRect(rcCand);
				iLen = rcCand.bottom - rcCand.top;

				if(m_rcCaretRect.top < g_pVideo->GetScreenLenY() / 2) {
					iPosY = m_rcCaretRect.bottom + 2;
				} else {
					if(iSelS == iSelE) {
						iPosY = m_rcCaretRect.top - iLen - 14;
					} else {
						iPosY = m_rcCaretRect.top - iLen - 2;
					}
				}
				

				iLen = rcCand.right - rcCand.left;
				iPosX = m_rcCaretRect.left + 2;
				if( iPosX + iLen > g_pVideo->GetScreenLenX()) {
					iPosX = g_pVideo->GetScreenLenX() - iLen;
				}
				pWindow->SetAbsCoord(iPosX, iPosY);
				g_pInterfaceManager->GetIMEPool()->SetStatusWindowPos(iPosX, iPosY);

				pWindow->SPSendMessage(SPIM_REFRESH);
			}
		}
	}


	if(bRefresh) 
		SPSendMessageToParent( SPIM_REFRESH, (WPARAM)m_wstrWindowText.c_str(), 0, this );

	//SPWindow::Process(fTime);
}



bool SPWindowEdit::ProcessString(OUT WCHAR* pwszBuf)
{
	int nTextLen;
	bool bRefresh = false;
	WCHAR wszTempBuf[EDIT_BUFSIZE];

	if( m_bFocus && g_pInterfaceManager->GetIMEPool()->IsUpdate() )	{
		m_wstrWindowText = g_pInterfaceManager->GetIMEPool()->GetStringW();		
		g_pInterfaceManager->GetIMEPool()->Refreshed();
		bRefresh = true;
	}

	nTextLen = m_wstrWindowText.length();

	if( nTextLen > 0 ) {
		// 숫자 모탛E인경퓖E최큱E玲柰?검퍊E
		if( m_bNumber )	{
			INT64 iwValue = _wtoi64(m_wstrWindowText.c_str());
			bool bUpdate = false;
			if( iwValue < m_iwMinValue ) {
				iwValue = m_iwMinValue;
				bUpdate = true;
			} else if( iwValue > m_iwMaxValue ) {
				iwValue = m_iwMaxValue;
				bUpdate = true;
			}

			_i64tow(iwValue, wszTempBuf, 10);
			if( bUpdate )
				g_pInterfaceManager->GetIMEPool()->SetStringW(wszTempBuf);
		}
		else
			wcscpy( wszTempBuf, m_wstrWindowText.c_str() );

		// 암호 모탛E인 경퓖E
		if( m_bPassword ) {			
			//FillMemory( wszTempBuf, L'*', lstrlenW(wszTempBuf) * sizeof(WCHAR) );
			int tCount = 0;
			for(; tCount < lstrlenW(wszTempBuf); ++tCount) {
				wszTempBuf[tCount] = L'*';
			}
			wszTempBuf[lstrlenW(wszTempBuf)] = 0;
		}
	} else {
		wszTempBuf[0] = 0;
	}

	if(pwszBuf)
		wcscpy(pwszBuf, wszTempBuf);

	return bRefresh;

}


void SPWindowEdit::Render(float fTime)
{
	if ( !m_bShow ) return;		

	switch(m_WndType) {
	case WND_TYPE_LEFTBASE:
	case WND_TYPE_TOPBASE:
		{		
			if( m_bEnable && (m_bFocus || IsCursorIn())  ) {			
				if(m_pFocusHead)
					m_pFocusHead->RenderTexture(&m_rcCalcDestHead,		&m_rcSrcHeadFocus);				

				if(m_pFocusBody)
					m_pFocusBody->RenderTexture(&m_rcCalcDestBody,		&m_rcSrcBodyFocus);				

				if(m_pFocusTail)
					m_pFocusTail->RenderTexture(&m_rcCalcDestTail,		&m_rcSrcTailFocus);				
			} else {
				if(m_pNormalHead) {
					if(m_bEnable == false) 
						m_pNormalHead->SetColor(RGBA(255, 255, 255, 127));
					m_pNormalHead->RenderTexture(&m_rcCalcDestHead,		&m_rcSrcHead);
				}
				if(m_pNormalBody ) {
					if(m_bEnable == false) 
						m_pNormalBody->SetColor(RGBA(255, 255, 255, 127));
					m_pNormalBody->RenderTexture(&m_rcCalcDestBody,		&m_rcSrcBody);
				}
				if(m_pNormalTail) {
					if(m_bEnable == false) 
						m_pNormalTail->SetColor(RGBA(255, 255, 255, 127));
					m_pNormalTail->RenderTexture(&m_rcCalcDestTail,		&m_rcSrcTail);
				}
				if(m_bEnable == false) {
					if(m_pNormalHead) m_pNormalHead->SetColor(RGBA(255, 255, 255, 255));
					if(m_pNormalBody) m_pNormalBody->SetColor(RGBA(255, 255, 255, 255));
					if(m_pNormalTail) m_pNormalTail->SetColor(RGBA(255, 255, 255, 255));
				}
			}			
		}
		break;
	case WND_TYPE_NULL:
	default:
		{		
			SPWindow::Render(fTime);
		}
		break;
	};
		

	if( g_pVideo == NULL )
		return;

	if( m_pFont == NULL )
		return;

	if ( !m_wstrWindowTextBuf.empty() ) {
		//RECT TarRect;
		bool bTemp = m_pFont->IsShadow();

		m_pFont->SetColor( m_FontColor		);
		m_pFont->SetEffect( m_FontEffect );


		char szTempBuf[512];
		SPAbuseFilter::GetInstance()->ConvertWideToAnsiString(szTempBuf, m_wstrWindowTextBuf.c_str());
		m_pFont->RenderText( szTempBuf, &m_rcTextRect, m_dwAlign);

		
		if(m_bFocus && (m_bTwinkleCursor || m_bSelect)) {
			if(m_bSelect) {
				g_pVideo->GetSysLine()->SetColor(0x88000000);
				float fRestore = g_pVideo->GetSysLine()->GetWidth();
				g_pVideo->GetSysLine()->SetWidth( m_pFont->GetHeight() );
				g_pVideo->GetSysLine()->DrawLine(m_rcCaretRect);
				g_pVideo->GetSysLine()->SetWidth(fRestore);
			} else {
				if(m_FontColor.r == 0 && m_FontColor.g == 0  && m_FontColor.b == 0) {
					g_pVideo->GetSysLine()->SetColor(0x88000000);
				} else {				
					g_pVideo->GetSysLine()->SetColor(m_FontColor);
				}
				g_pVideo->GetSysLine()->DrawLine(m_rcCaretRect);				
			}			
		}

		//if( m_bFocus && g_pInterfaceManager->GetIMEPool()->IsComposition()) {
		if( m_bFocus && !m_bSelect) {
			if(m_FontColor.r == 0 && m_FontColor.g == 0  && m_FontColor.b == 0) {
				g_pVideo->GetSysLine()->SetColor(0x88000000);
			} else {				
				g_pVideo->GetSysLine()->SetColor(m_FontColor);
			}

			if(g_pInterfaceManager->GetIMEPool()->IsInsertOnType() == false) {
				g_pVideo->GetSysLine()->DrawLine(m_rcCaretRect);
			}			
			g_pVideo->GetSysLine()->DrawLine(m_rcCompositionCaret);
		}	
		


		m_pFont->SetColor();
		m_pFont->SetShadow(	bTemp );
	}

	if(m_pImageDeco) {
		m_pImageDeco->RenderTexture(&m_rcCalcDestDeco, &m_rcSrcDeco);
	}

}


void SPWindowEdit::Hide( bool bSendServer)
{
	if ( !m_bShow ) return;

	m_bFocus = false;
	m_bShow = false;	
	//SPIMEPool* pIMEPool = g_pInterfaceManager->GetIMEPool();
	//(pIMEPool)->SetFocus( false );

	KillFocus();
}

void SPWindowEdit::SetFocus()
{
	if( m_bEnable ) {
		g_pInterfaceManager->GetIMEPool()->SetFocus( true );
		SPWindow::SetFocus();

		if(m_bEnableIME == false || m_bNumber == true) {
			g_pInterfaceManager->GetIMEPool()->DisableIME();
		}

		g_pInterfaceManager->GetIMEPool()->SetNumber( m_bNumber );
		g_pInterfaceManager->GetIMEPool()->SetPassword( m_bPassword );
	}
}

void SPWindowEdit::KillFocus()
{
	//SetWindowText(g_pInterfaceManager->GetIMEPool()->GetString());

	char wszTextBuf[EDIT_BUFSIZE];
	FillMemory( wszTextBuf, 0, EDIT_BUFSIZE );

	if ( !m_wstrWindowText.empty() ) {
		if( m_bPassword ) {			
			FillMemory( wszTextBuf, L'*', m_wstrWindowText.length() * sizeof(WCHAR) );
		}
	} else {
		m_wstrWindowTextBuf.clear();
	}

	if(m_bNumber)
		g_pInterfaceManager->GetIMEPool()->SetNumber( false );
	if (m_bPassword)
		g_pInterfaceManager->GetIMEPool()->SetPassword( false );

	
	int iWideLen =SPAbuseFilter::GetInstance()->GetAnsiLenToWideLen( m_iLimitText - 1, m_wstrWindowText.c_str());

	//int iWideLen = m_wstrWindowText.length();
	if(iWideLen > m_iLimitText) {
		iWideLen = m_iLimitText;
		m_wstrWindowText[iWideLen] = '\0'; 
	}	

	g_pInterfaceManager->GetIMEPool()->SetFocus( false );
	g_pInterfaceManager->GetCandVertical()->SPSendMessage(SPIM_REFRESH);

	

	SPWindow::KillFocus();		
}

void SPWindowEdit::SetLimitText( int nLength )
{
	m_iLimitText = nLength;
	g_pInterfaceManager->GetIMEPool()->SetLimitText( nLength );
}

void SPWindowEdit::SetNULL()
{
	g_pInterfaceManager->GetIMEPool()->ClearBuffer();
}

void SPWindowEdit::SetMargin(int iX, int iY)
{
	m_iMarginX = iX;
	m_iMarginY = iY;
}

void SPWindowEdit::ClearStringBuffer()
{
	m_wstrWindowText.assign( m_wstrWindowText.size() , ' ' );
	m_wstrWindowText.clear();
	memset( m_szWindowText , 0 , sizeof( m_szWindowText ) );
}

void SPWindowEdit::SetWindowText(const char* pStr)
{
	WCHAR wszTemp[EDIT_BUFSIZE];	

	ZeroMemory(wszTemp, sizeof(wszTemp));
	SPAbuseFilter::GetInstance()->ConvertAnsiStringToWide( wszTemp, pStr);

	// SetWindowFormatText(wszTemp);
	// 기존의경퓖E%가 없엉碑다. [7/4/2007 AJJIYA]
	SetWindowFormatText( L"%s" , wszTemp );

	GetWindowRect(m_rcWindowRect);
	m_rcTextRect = m_rcWindowRect;
	m_rcTextRect.left += m_iMarginX;
	m_rcTextRect.top += m_iMarginY;		

	return;
}

void SPWindowEdit::SetWindowFormatText( const WCHAR* wszText, ... ) 
{ 
	if( wszText == NULL || lstrlenW(wszText) <= 0 )	{
		m_wstrWindowText.clear();
		m_wstrWindowTextBuf.clear();
		g_pInterfaceManager->GetIMEPool()->SetStringW( m_wstrWindowText.c_str() );
		return;
	}

	WCHAR	buff[4096];
	va_list va;

	va_start( va, wszText );
	vswprintf( buff, wszText, va );
	va_end( va );

	m_wstrWindowText = buff;
	m_wstrWindowTextBuf = buff;

	if( m_bFocus )
		g_pInterfaceManager->GetIMEPool()->SetStringW( m_wstrWindowText.c_str() );

}

const char* SPWindowEdit::GetWindowText()
{	
	SPAbuseFilter::GetInstance()->ConvertWideToAnsiString(m_szWindowText, m_wstrWindowText.c_str());

	return m_szWindowText;
	//return m_wstrWindowText.c_str();	
}


bool SPWindowEdit::SetImageNormalHead	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	SAFE_RELEASE(m_pNormalHead);

	strResource = szImage;
	//if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
	if(g_pVideo->GetTextureMgr()->GetTexture(strResource.c_str(), &pImage,TEX_INTERFACE) == false) {
		return false;
	}
	m_pNormalHead		= pImage;

	m_rcSrcHead.left	= iXSrc;
	m_rcSrcHead.top		= iYSrc;
	m_rcSrcHead.right	= iX2Src;
	m_rcSrcHead.bottom	= iY2Src;
	ReposOwnImage();
	return true;
}

bool SPWindowEdit::SetImageNormalBody	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	SAFE_RELEASE(m_pNormalBody);

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	m_pNormalBody		= pImage;

	m_rcSrcBody.left	= iXSrc;
	m_rcSrcBody.top		= iYSrc;
	m_rcSrcBody.right	= iX2Src;
	m_rcSrcBody.bottom	= iY2Src;
	ReposOwnImage();
	return true;
}



bool SPWindowEdit::SetImageNormalTail	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	SAFE_RELEASE(m_pNormalTail);

	strResource = szImage;
	//if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
	if(g_pVideo->GetTextureMgr()->GetTexture(strResource.c_str(), &pImage,TEX_INTERFACE) == false) {
		return false;
	}
	m_pNormalTail		= pImage;

	m_rcSrcTail.left	= iXSrc;
	m_rcSrcTail.top		= iYSrc;
	m_rcSrcTail.right	= iX2Src;
	m_rcSrcTail.bottom	= iY2Src;
	ReposOwnImage();
	return true;
}

bool SPWindowEdit::SetImageFocusHead	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	SAFE_RELEASE(m_pFocusHead);

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	m_pFocusHead	= pImage;

	m_rcSrcHeadFocus.left	= iXSrc;
	m_rcSrcHeadFocus.top	= iYSrc;
	m_rcSrcHeadFocus.right	= iX2Src;
	m_rcSrcHeadFocus.bottom	= iY2Src;
	ReposOwnImage();
	return true;
}


bool SPWindowEdit::SetImageFocusBody	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	SAFE_RELEASE(m_pFocusBody);

	strResource = szImage;
	//if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
	if(g_pVideo->GetTextureMgr()->GetTexture(strResource.c_str(), &pImage,TEX_INTERFACE) == false) {
		return false;
	}
	m_pFocusBody			= pImage;

	m_rcSrcBodyFocus.left	= iXSrc;
	m_rcSrcBodyFocus.top	= iYSrc;
	m_rcSrcBodyFocus.right	= iX2Src;
	m_rcSrcBodyFocus.bottom	= iY2Src;
	ReposOwnImage();
	return true;
}


bool SPWindowEdit::SetImageFocusTail	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	SAFE_RELEASE(m_pFocusTail);

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	m_pFocusTail			= pImage;

	m_rcSrcTailFocus.left	= iXSrc;
	m_rcSrcTailFocus.top	= iYSrc;
	m_rcSrcTailFocus.right	= iX2Src;
	m_rcSrcTailFocus.bottom	= iY2Src;
	ReposOwnImage();
	return true;
}


bool SPWindowEdit::SetImageDeco (const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src)
{
	SPTexture* pImage = NULL;
	std::string strResource;

	SAFE_RELEASE(m_pImageDeco);

	strResource = szImage;
	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage) == false) {
		return false;
	}
	m_pImageDeco		= pImage;

	m_rcSrcDeco.left	= iXSrc;
	m_rcSrcDeco.top		= iYSrc;
	m_rcSrcDeco.right	= iX2Src;
	m_rcSrcDeco.bottom	= iY2Src;
	ReposOwnImage();
	return true;

}

void SPWindowEdit::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowEdit::ReposOwnImage()
{
	switch(m_WndType) {
	case WND_TYPE_LEFTBASE:
		{
			SetRect(&m_rcCalcDestHead, 
				m_iAX,
				m_iAY, 
				m_iAX + m_rcSrcHead.right - m_rcSrcHead.left, 
				m_iAY + m_rcSrcHead.bottom - m_rcSrcHead.top);

			SetRect(&m_rcCalcDestBody,  
				m_rcCalcDestHead.right, 
				m_rcCalcDestHead.top, 
				m_iAX + m_iSX - (m_rcSrcTail.right - m_rcSrcTail.left), 
				m_rcCalcDestHead.bottom);

			SetRect(&m_rcCalcDestTail, 
				m_rcCalcDestBody.right,
				m_rcCalcDestBody.top, 
				m_iAX + m_iSX, 
				m_rcCalcDestBody.bottom);

			if(m_pImageDeco) {
				SetRect( &m_rcCalcDestDeco,
					m_rcCalcDestTail.left - (m_rcSrcDeco.right - m_rcSrcDeco.left),
					m_iAY + ((m_iSY - (m_rcSrcDeco.bottom - m_rcSrcDeco.top)))/2, 
					m_rcCalcDestTail.left, 
					m_iAY + ((m_iSY - (m_rcSrcDeco.bottom - m_rcSrcDeco.top)))/2 + (m_rcSrcDeco.bottom - m_rcSrcDeco.top));
			}
		}
		break;
	case WND_TYPE_TOPBASE:
		{
			SetRect(&m_rcCalcDestHead, 
				m_iAX,
				m_iAY, 
				m_iAX + m_rcSrcHead.right - m_rcSrcHead.left, 
				m_iAY + m_rcSrcHead.bottom - m_rcSrcHead.top);

			SetRect(&m_rcCalcDestBody,
				m_rcCalcDestHead.left, 
				m_rcCalcDestHead.bottom, 
				m_rcCalcDestHead.right, 
				m_iAY + m_iSY - (m_rcSrcTail.bottom - m_rcSrcTail.top));

			SetRect(&m_rcCalcDestTail,
				m_rcCalcDestBody.left,
				m_rcCalcDestBody.bottom, 
				m_rcCalcDestBody.right, 
				m_iAY + m_iSY);

			if(m_pImageDeco) {
				SetRect( &m_rcCalcDestDeco,
					m_iAX + m_iSY - 2 - (m_rcSrcDeco.right - m_rcSrcDeco.left),	
					m_iAY + ((m_iSY - (m_rcSrcDeco.bottom - m_rcSrcDeco.top)))/2, 
					m_iAX + m_iSY - 2, // 그냥 마햨E
					m_iAY + ((m_iSY - (m_rcSrcDeco.bottom - m_rcSrcDeco.top)))/2 + (m_rcSrcDeco.bottom - m_rcSrcDeco.top));
			}
		}
		break;

	case WND_TYPE_NULL:
	default:
		{
			if(m_pImageDeco) {
				SetRect( &m_rcCalcDestDeco,
					m_iAX + m_iSY - 2 - (m_rcSrcDeco.right - m_rcSrcDeco.left),	
					m_iAY + ((m_iSY - (m_rcSrcDeco.bottom - m_rcSrcDeco.top)))/2, 
					m_iAX + m_iSY - 2, // 그냥 마햨E
					m_iAY + ((m_iSY - (m_rcSrcDeco.bottom - m_rcSrcDeco.top)))/2 + (m_rcSrcDeco.bottom - m_rcSrcDeco.top));
			}
		}
		break;
	}

	GetWindowRect(m_rcWindowRect);
	m_rcTextRect = m_rcWindowRect;
	m_rcTextRect.left += m_iMarginX;
	m_rcTextRect.top += m_iMarginY;		

	
}

//////////////////////////////////////////////////////////////////////////
// Message Map
SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowEdit )
	SPIMESSAGE_COMMAND( SPIM_SETFOCUS,		OnSelFocus		)
	SPIMESSAGE_COMMAND( SPIM_KILLFOCUS,		OnKillFocus		)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,	OnCursorLDown	)
	
	SPIMESSAGE_COMMAND( SPIM_WHEEL_UP,		OnWheelUp		)
	SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN,	OnWheelDown		)
	SPIMESSAGE_COMMAND( SPIM_KEY_DOWN,		OnKeyDown		)
	SPIMESSAGE_COMMAND( SPIM_SET_CARET_POS, OnSetCaretPos	)

	SPIMESSAGE_COMMAND( SPIM_CURSOR_OUT,	OnCursorOut		)
	SPIMESSAGE_COMMAND( SPIM_CURSOR_MOVE,	OnCursorMove	)
	SPIMESSAGE_COMMAND( SPIM_KEY_RETURN,	OnKeyReturn		)	
	
SPIMESSAGE_OBJ_MAP_END( SPWindow )

int SPWindowEdit::OnKeyReturn ( WPARAM wParam, LPARAM lParam )
{
	return SPSendMessageToParent(SPIM_KEY_RETURN, wParam, lParam, this);
}


int SPWindowEdit::OnCursorLDown( WPARAM wParam, LPARAM lParam )
{
	return OnSelFocus(wParam, lParam);
}

int SPWindowEdit::OnSelFocus( WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable )	{
		SPIMEPool* pIMEPool = g_pInterfaceManager->GetIMEPool();
		pIMEPool->SetFocus( true );
		if(m_bEnableIME == false || m_bNumber == true) {
			pIMEPool->DisableIME();
		}
		
		pIMEPool->SetLimitText( m_iLimitText );
		pIMEPool->SetNumber( m_bNumber );
		pIMEPool->SetStringW( m_wstrWindowText.c_str() );
		pIMEPool->SetPassword( m_bPassword );

		m_bFocus = true;

	}
	return SPSendMessageToParent( SPIM_SETFOCUS, 0, 0, this );
}

int SPWindowEdit::OnKillFocus( WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable )	{ // 영문 모드로 바꾼다

		// Composition String 처리
		g_pInterfaceManager->GetIMEPool()->PreKillFocus();		
		ProcessString();
		g_pInterfaceManager->GetIMEPool()->KillFocus();


		KillFocus();
	}
	return SPSendMessageToParent( SPIM_KILLFOCUS, 0, 0, this );
}


int SPWindowEdit::OnWheelUp( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_WHEEL_UP, wParam, lParam, this);
}

int SPWindowEdit::OnWheelDown( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_WHEEL_DOWN, wParam, lParam, this);
}

int SPWindowEdit::OnKeyDown( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_KEY_DOWN, wParam, lParam, this);
}

int SPWindowEdit::OnSetCaretPos( WPARAM wParam, LPARAM lParam)
{
	switch(wParam) {	
	case CPOS_BEGIN	:
		g_pInterfaceManager->GetIMEPool()->SetCaretPos(0);
		break;
	case CPOS_END	:
		g_pInterfaceManager->GetIMEPool()->SetCaretPos(-1);
		break;
	case CPOS_CUR	:
		g_pInterfaceManager->GetIMEPool()->SetCaretPos((int)lParam);
		break;	
	}
	return -1;
}

int SPWindowEdit::OnCursorOut( WPARAM wParam, LPARAM lParam)
{
	return 1;
}
	
int SPWindowEdit::OnCursorMove( WPARAM wParam, LPARAM lParam)
{
	return 1;
}





