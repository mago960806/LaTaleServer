

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowEditMultiLine.h"

#include "SPMouseCursor.h"

#include "SPSubGameManager.h"
#include "SPIMEPool.h"
#include "SPInterfaceManager.h"

#include "SPDebug.h"

#include "SPAbuseFilter.h"

extern HWND			g_hWnd;
extern HINSTANCE	g_hInstance;

#define EDIT_BUFSIZE 256

SPWindowEditMultiLine::SPWindowEditMultiLine( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_CTRL_MULTILINE_EDIT, InstanceID, iX, iY, iCX, iCY, pParent )
{
	m_iLimitText = 100;
	m_bTwinkleCursor = false;	
	m_fPrevTime = 0;

	m_dwAlign = DT_LEFT | DT_NOCLIP;
	m_bEnable = true;
	m_bSelect = false;

	m_iMargin = 4;
	m_iCurLine = 1; // 1 base
	m_iCurCaretInLine = 0;		
	m_bEnableEnter = false;
	

	GetWindowRect(m_rcWindowRect);
	CopyMemory(&m_rcCaretRect, &m_rcWindowRect, sizeof(RECT));

	CalcMaxLine();
}


SPWindowEditMultiLine::~SPWindowEditMultiLine()
{	
}


void SPWindowEditMultiLine::SetEnableEnter(bool bEnable)
{
	m_bEnableEnter = bEnable;
}

bool SPWindowEditMultiLine::IsEnableEnter()
{
	return m_bEnableEnter;
}



void SPWindowEditMultiLine::Process( float fTime )
{
	m_fPrevTime += fTime;
	if( m_fPrevTime > 0.5f ) {
		m_fPrevTime -= 0.5f;
		m_fPrevTime = fTime;
		m_bTwinkleCursor = !m_bTwinkleCursor;
	}

	if( g_pVideo == NULL )
		return;

	if( m_pFont == NULL )
		return;

	GetWindowRect(m_rcWindowRect);

	// Process String 
	bool bRefresh = ProcessString();
	
	// Process Caret 
	if( m_bFocus ) {		
		int iSelS, iSelE;	
		iSelS = iSelE = -1;		
		g_pInterfaceManager->GetIMEPool()->GetSelW( iSelS, iSelE );		

		if( iSelS == iSelE ) { // ｼｱﾅﾃﾀﾌ ｾﾆｴﾏｰ・Caret ﾀﾇ ﾀｧﾄ｡
			if(g_pInterfaceManager->GetIMEPool()->IsComposition() && g_pInterfaceManager->GetIMEPool()->GetCompLenW()) {
				ProcessComposition();
			} else {
				ProcessCaret();	
			}			
		} else {
			m_bSelect = true;
			ProcessSelect(iSelS, iSelE);
		}

		ProcessCandidate();
	}

	if(bRefresh) 
		SPSendMessageToParent( SPIM_REFRESH, (WPARAM)m_wstrWindowText.c_str(), 0, this );	
}


bool SPWindowEditMultiLine::ProcessString()
{
	bool bRefresh = false;		
	std::wstring wstrForInsert;
	std::wstring::iterator IterWStr;	
	SIZE strSize;
	WCHAR wszTemp[256];
	int iLen ;
	int iLastLen ;	

	// String Process
	if( m_bFocus && g_pInterfaceManager->GetIMEPool()->IsUpdate() )	{

		// Exception process for convert wide length to ansi length [9/27/2007 metalgeni]
		std::string strTemp;
		strTemp = g_pInterfaceManager->GetIMEPool()->GetString();
		if(strTemp.length() > m_iLimitText) {
			strTemp[m_iLimitText] = 0;
			g_pInterfaceManager->GetIMEPool()->SetString(strTemp.c_str());
			g_pInterfaceManager->GetIMEPool()->Deselect();
		}

		m_wstrWindowText = g_pInterfaceManager->GetIMEPool()->GetStringW();
		g_pInterfaceManager->GetIMEPool()->Refreshed();

		if(!m_wstrWindowText.empty()) {	
			m_vwstrWindowText.clear();
			m_vbWithNewLine.clear();

			iLen = 0;
			iLastLen = 0;
			IterWStr = m_wstrWindowText.begin() + 0;
			for(; IterWStr !=  m_wstrWindowText.end(); ++IterWStr, ++iLen) {
				wcsncpy(wszTemp, &(*(m_wstrWindowText.begin()+iLastLen)), iLen-iLastLen);
				wszTemp[iLen-iLastLen] = 0; //Null Terminator

				strSize = m_pFont->GetSizeW(wszTemp);

				//if(strSize.cx > (m_rcWindowRect.right - m_rcWindowRect.left - 12) ) { // 12 ｴﾂ ｿｩﾀｯｺﾐ
				if(strSize.cx > (m_rcWindowRect.right - m_rcWindowRect.left - 12) || (*IterWStr) == '\n' /* \n == 10 */ ) { // 12 ｴﾂ ｿｩﾀｯｺﾐ
					wstrForInsert.clear();					
					wstrForInsert = m_wstrWindowText.substr(iLastLen, iLen-iLastLen);
					iLastLen = iLen;
					m_vwstrWindowText.push_back( wstrForInsert );
					if(m_wstrWindowText.length() > (iLen+1)  && (*IterWStr) == '\n') {
						++IterWStr;
						++iLastLen;
						++iLen;
						m_vbWithNewLine.push_back(true);
						while( (*IterWStr) == '\n') {
							wstrForInsert = L"";
							m_vwstrWindowText.push_back( wstrForInsert );
							m_vbWithNewLine.push_back(true);
							++IterWStr;
							++iLastLen;
							++iLen;
							if(IterWStr ==  m_wstrWindowText.end()) {
								break;
							}
						}
					} else {
						m_vbWithNewLine.push_back(false);
					}

					if(IterWStr ==  m_wstrWindowText.end()) {
						break;
					}
				}
			}

			wstrForInsert.clear();
			wstrForInsert = m_wstrWindowText.substr(iLastLen);
			m_vwstrWindowText.push_back( wstrForInsert );
			m_vbWithNewLine.push_back(false);

			if(m_iMaxLine < m_vwstrWindowText.size()) { // string with line over
				m_wstrWindowText.clear();
				std::vector<std::wstring>::iterator IterStr = m_vwstrWindowText.begin();
				std::vector<bool>::iterator IterBool = m_vbWithNewLine.begin();
				for(int i = 0;i < m_iMaxLine; ++i) {
					m_wstrWindowText += (*(IterStr + i));
					if((*(IterBool + i) == true) && i != (m_iMaxLine-1)) 
						m_wstrWindowText += '\n';
				}
				g_pInterfaceManager->GetIMEPool()->SetStringW(m_wstrWindowText.c_str());
				g_pInterfaceManager->GetIMEPool()->Deselect();
			}


		} else {
			m_vwstrWindowText.clear();
			m_vbWithNewLine.clear();
		}
		bRefresh = true;
	}

	return bRefresh;

}


void SPWindowEditMultiLine::ProcessCaret()
{
	WCHAR wszText[256];
	std::wstring wstrTemp; 

	SIZE SizeCaret1, SizeCaret2;
	std::vector<std::wstring>::iterator IterVWStr;
	std::vector<bool>::iterator			IterBool;
	int iLen;
	int iRange;	

	int iLastLen = 0;	
	int iCaretPos = g_pInterfaceManager->GetIMEPool()->GetCaretPosW();
	
	m_iCurLine = 1;
	m_iCurCaretInLine = 0;
	m_bSelect = false;

	m_vrcCaretRect.clear();
	m_vrcCompositionCaret.clear();

	iLen = g_pInterfaceManager->GetIMEPool()->GetStringLenW();	

	// find number of newline within current cursor position
	int iNewline = 0;
	int iFind = 0;
	int iCurPos = 0;

	wstrTemp = g_pInterfaceManager->GetIMEPool()->GetStringW();
	while ( iCurPos != std::string::npos && iCurPos < iCaretPos ) {				
		iCurPos = wstrTemp.find('\n', iFind);
		if(iCurPos >= iCaretPos) {
			break;
		}
		if( iCurPos != std::string::npos) {			
			iFind = iCurPos + 1;
			++iNewline;
		}
	} 


	if( iLen <= iCaretPos ) {// caret ﾀﾌ ｹｮﾀ・ｳ｡ｿ｡ ｿﾀｴﾂ ｰ豼・
		if( ! m_vwstrWindowText.empty()) {
			IterVWStr = m_vwstrWindowText.begin() + (m_vwstrWindowText.size() - 1);
			wstrTemp = (*IterVWStr);
			m_iCurLine = m_vwstrWindowText.size();
		} 

		wcsncpy(wszText, wstrTemp.c_str(), iCaretPos);
		wszText[iCaretPos] = 0;
		if(wszText[0] == '\n') {
			SizeCaret1.cx = 0;
			SizeCaret2.cx = 6;
			m_iCurCaretInLine = 0;
		} else {
			SizeCaret2 = SizeCaret1 = m_pFont->GetSizeW(wszText);
			SizeCaret2.cx += 6;
			m_iCurCaretInLine = wstrTemp.length();
		}

	} else {

		iLen = 0;
		iLastLen  = iCaretPos;				

		IterVWStr = m_vwstrWindowText.begin();
		IterBool = m_vbWithNewLine.begin();

		for(; IterVWStr !=  m_vwstrWindowText.end(); ++IterVWStr, ++IterBool) {
			iLen = (*IterVWStr).length();
			iLastLen -= iLen;
			if(*IterBool) {
				iLastLen -= 1;
			}
			if(iLastLen  < 0) {
				iLastLen += iLen; // ﾇ・ﾄｿｼｭ ﾀｧﾄ｡ ｸｸﾅｭ ｹﾙｲ羸ﾟ ﾇﾑｴﾙ
				if(*IterBool) {
					iLastLen += 1;
				}
				//if((*(wstrTemp.begin() + iLastLen)) == '\n' ) {
				//	++m_iCurLine;
				//}
				break;
			}
			++m_iCurLine;
		}

		if( ! m_vwstrWindowText.empty() && IterVWStr!= m_vwstrWindowText.end()) {				
			wstrTemp = (*IterVWStr);
		}

		int iTempCurCaret = iLastLen;

		if(iTempCurCaret < 0) {
			//assert(0);
			iTempCurCaret = 0;	
		}

		m_iCurCaretInLine = iTempCurCaret;
		wcsncpy(wszText, wstrTemp.c_str(), iTempCurCaret);
		wszText[iTempCurCaret] = 0;
		SizeCaret1 = m_pFont->GetSizeW(wszText);

		if(g_pInterfaceManager->GetIMEPool()->GetCompLenW()) {		
			//iRange = iLastLen + 1;
			iRange = iLastLen + g_pInterfaceManager->GetIMEPool()->GetCompLenW();
		} else {
			iRange = iLastLen + 1;
		}

		wcsncpy(wszText, wstrTemp.c_str(), iRange);
		wszText[iRange] = 0;
		SizeCaret2 = m_pFont->GetSizeW(wszText);

	}

	// DT_LEFT and other..
	if(m_vwstrWindowText.empty()) {
		m_rcCaretRect.top = m_rcWindowRect.top + ((m_pFont->GetHeight() + m_iMargin)); 
	} else {
		m_rcCaretRect.top = m_rcWindowRect.top + (m_iCurLine * (m_pFont->GetHeight() + m_iMargin)); 
	}
	m_rcCaretRect.top -= 2;
	m_rcCaretRect.bottom = m_rcCaretRect.top; // + SizeCaret1.cy;
	m_rcCaretRect.left = m_rcWindowRect.left + SizeCaret1.cx;
	m_rcCaretRect.right = m_rcCaretRect.left + SizeCaret2.cx - SizeCaret1.cx;			

	if(m_rcCaretRect.left > m_rcWindowRect.right) 
		m_rcCaretRect.left = m_rcWindowRect.right;

	if(m_rcCaretRect.right > m_rcWindowRect.right) 
		m_rcCaretRect.right = m_rcWindowRect.right;	

	if(m_rcCaretRect.bottom > m_rcWindowRect.bottom) 
		m_rcCaretRect.bottom = m_rcWindowRect.bottom;

	if(m_rcCaretRect.top > m_rcWindowRect.bottom) 
		m_rcCaretRect.top = m_rcWindowRect.bottom;

}

void SPWindowEditMultiLine::ProcessComposition()
{

	int iLen = 0;
	int iLastLen;

	int iBeginLine = 0;
	int iEndLine = 0;
	int iBeginCaretInLine = 0;
	int iEndCaretInLine = 0;

	int iBeginSel;
	int iEndSel;	
	int iCaretPos = g_pInterfaceManager->GetIMEPool()->GetCaretPosW();	

	iBeginSel = iCaretPos;
	iEndSel = iBeginSel + g_pInterfaceManager->GetIMEPool()->GetCompLenW();

	m_iCurCaretInLine = 0;

	std::vector<std::wstring>::iterator IterVWStr;
	iLastLen = 0;
	IterVWStr = m_vwstrWindowText.begin();
	for(; IterVWStr !=  m_vwstrWindowText.end(); ++IterVWStr) {
		iLen = (*IterVWStr).length();
		iLastLen += iLen;
		if(iLastLen  > iBeginSel) {
			iLastLen -= iLen;
			iBeginCaretInLine = iBeginSel - iLastLen;			
			break;
		}
		++iBeginLine;
	}

	iLastLen = 0;
	IterVWStr = m_vwstrWindowText.begin();
	for(; IterVWStr !=  m_vwstrWindowText.end(); ++IterVWStr) {
		iLen = (*IterVWStr).length();
		iLastLen += iLen;
		if(iLastLen  >= iEndSel) {
			iLastLen -= iLen;
			iEndCaretInLine = iEndSel - iLastLen;			
			break;
		}
		++iEndLine;
	}

	m_iCurLine = iBeginLine + 1;
	m_iCurCaretInLine = iBeginCaretInLine;

	SIZE SizeCaret1, SizeCaret2; 
	WCHAR wszText[256];
	RECT rcTemp;

	if(m_vwstrWindowText.size() <= iBeginLine) {
		iBeginLine = m_vwstrWindowText.size() - 1;
	}


	m_vrcCaretRect.clear();
	m_vrcCompositionCaret.clear();

	if(iBeginLine == iEndLine) {		
		//WILL once crashed by this point, because m_vwstrWindowText was empty.
		wcsncpy( wszText, (*(m_vwstrWindowText.begin() + iBeginLine)).c_str(), iBeginCaretInLine);
		wszText[iBeginCaretInLine] = 0;
		SizeCaret1 = m_pFont->GetSizeW(wszText);

		wcsncpy( wszText, (*(m_vwstrWindowText.begin() + iBeginLine)).c_str(), iEndCaretInLine);
		wszText[iEndCaretInLine] = 0;
		SizeCaret2 = m_pFont->GetSizeW(wszText);

		rcTemp.bottom = rcTemp.top = 
			m_rcWindowRect.top + ((iBeginLine + 1) * (m_pFont->GetHeight() + m_iMargin)) - 2;  
		rcTemp.left = m_rcWindowRect.left + SizeCaret1.cx;
		rcTemp.right = m_rcWindowRect.left + SizeCaret2.cx;

		m_vrcCaretRect.push_back(rcTemp);	

	} else {
		wcsncpy( wszText, (*(m_vwstrWindowText.begin() + iBeginLine)).c_str(), iBeginCaretInLine);
		wszText[iBeginCaretInLine] = 0;
		SizeCaret1 = m_pFont->GetSizeW(wszText);

		rcTemp.bottom = rcTemp.top = 
			m_rcWindowRect.top + ((iBeginLine + 1) * (m_pFont->GetHeight() + m_iMargin)) - 2;  
		rcTemp.left = m_rcWindowRect.left + SizeCaret1.cx;
		rcTemp.right = m_rcWindowRect.right;

		m_vrcCaretRect.push_back(rcTemp);


		wcsncpy( wszText, (*(m_vwstrWindowText.begin() + iEndLine)).c_str(), iEndCaretInLine);
		wszText[iEndCaretInLine] = 0;
		SizeCaret2 = m_pFont->GetSizeW(wszText);

		rcTemp.bottom = rcTemp.top = 
			m_rcWindowRect.top + ((iEndLine + 1) * (m_pFont->GetHeight() + m_iMargin)) - 2;  
		rcTemp.left = m_rcWindowRect.left;
		rcTemp.right = m_rcWindowRect.left + SizeCaret2.cx;

		m_vrcCaretRect.push_back(rcTemp);


		rcTemp.left = m_rcWindowRect.left;
		rcTemp.right = m_rcWindowRect.right;
		for(int i = iBeginLine+1; i < iEndLine; ++i) {
			rcTemp.bottom = rcTemp.top = 
				m_rcWindowRect.top + ((i + 1) * (m_pFont->GetHeight() + m_iMargin)) - 2;

			m_vrcCaretRect.push_back(rcTemp);
		}
	}



	//////////////////////////////////////////////////////////////////////////
	// Composition Attr Caret
	const BYTE* pCaretAttr = g_pInterfaceManager->GetIMEPool()->GetCompositionAttr();	

	// Use Composition Caret && Exist Composotion Attr?
	//if(g_pInterfaceManager->GetIMEPool()->IsWideCaret() && pCaretAttr[0] != 0) {


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

		if(iCompCaretBegin == -1)
			iCompCaretBegin = iCompCaretEnd;

//////////////////////////////////////////////////////////////////////////
		iEndCaretInLine = iBeginCaretInLine + iCompCaretEnd;
		iBeginCaretInLine += iCompCaretBegin;		
		if( iEndCaretInLine > g_pInterfaceManager->GetIMEPool()->GetCompLenW() + iCaretPos) {
			iEndCaretInLine = g_pInterfaceManager->GetIMEPool()->GetCompLenW() + iCaretPos;
		}

		iBeginSel = g_pInterfaceManager->GetIMEPool()->GetCaretPosW() + iCompCaretBegin;
		iEndSel = g_pInterfaceManager->GetIMEPool()->GetCaretPosW() + iCompCaretEnd;
		iBeginLine = 0;
		iEndLine = 0;

		iLastLen = 0;
		IterVWStr = m_vwstrWindowText.begin();
		for(; IterVWStr !=  m_vwstrWindowText.end(); ++IterVWStr) {
			iLen = (*IterVWStr).length();
			iLastLen += iLen;
			if(iLastLen  >= iBeginSel) {
				iLastLen -= iLen;
				iBeginCaretInLine = iBeginSel - iLastLen;			
				break;
			}
			++iBeginLine;
		}

		iLastLen = 0;
		IterVWStr = m_vwstrWindowText.begin();
		for(; IterVWStr !=  m_vwstrWindowText.end(); ++IterVWStr) {
			iLen = (*IterVWStr).length();
			iLastLen += iLen;
			if(iLastLen  >= iEndSel) {
				iLastLen -= iLen;
				iEndCaretInLine = iEndSel - iLastLen;			
				break;
			}
			++iEndLine;
		}

		if(m_vwstrWindowText.size() <= iBeginLine) {
			iBeginLine = m_vwstrWindowText.size() - 1;
		}

		if(m_vwstrWindowText.size() <= iEndLine) {
			iEndLine = m_vwstrWindowText.size() - 1;
		}		
		
		if(iBeginLine == iEndLine) {		
			wcsncpy( wszText, (*(m_vwstrWindowText.begin() + iBeginLine)).c_str(), iBeginCaretInLine);
			wszText[iBeginCaretInLine] = 0;
			SizeCaret1 = m_pFont->GetSizeW(wszText);

			wcsncpy( wszText, (*(m_vwstrWindowText.begin() + iBeginLine)).c_str(), iEndCaretInLine);
			wszText[iEndCaretInLine] = 0;
			SizeCaret2 = m_pFont->GetSizeW(wszText);

			rcTemp.bottom = rcTemp.top = 
				m_rcWindowRect.top + ((iBeginLine + 1) * (m_pFont->GetHeight() + m_iMargin));  
			rcTemp.left = m_rcWindowRect.left + SizeCaret1.cx;
			rcTemp.right = m_rcWindowRect.left + SizeCaret2.cx;

			m_vrcCompositionCaret.push_back(rcTemp);	

		} else {
			wcsncpy( wszText, (*(m_vwstrWindowText.begin() + iBeginLine)).c_str(), iBeginCaretInLine);
			wszText[iBeginCaretInLine] = 0;
			SizeCaret1 = m_pFont->GetSizeW(wszText);

			rcTemp.bottom = rcTemp.top = 
				m_rcWindowRect.top + ((iBeginLine + 1) * (m_pFont->GetHeight() + m_iMargin));  
			rcTemp.left = m_rcWindowRect.left + SizeCaret1.cx;
			rcTemp.right = m_rcWindowRect.right;

			m_vrcCompositionCaret.push_back(rcTemp);


			wcsncpy( wszText, (*(m_vwstrWindowText.begin() + iEndLine)).c_str(), iEndCaretInLine);
			wszText[iEndCaretInLine] = 0;
			SizeCaret2 = m_pFont->GetSizeW(wszText);

			rcTemp.bottom = rcTemp.top = 
				m_rcWindowRect.top + ((iEndLine + 1) * (m_pFont->GetHeight() + m_iMargin));  
			rcTemp.left = m_rcWindowRect.left;
			rcTemp.right = m_rcWindowRect.left + SizeCaret2.cx;

			m_vrcCompositionCaret.push_back(rcTemp);


			rcTemp.left = m_rcWindowRect.left;
			rcTemp.right = m_rcWindowRect.right;
			for(int i = iBeginLine+1; i < iEndLine; ++i) {
				rcTemp.bottom = rcTemp.top = 
					m_rcWindowRect.top + ((i + 1) * (m_pFont->GetHeight() + m_iMargin));

				m_vrcCompositionCaret.push_back(rcTemp);
			}
		}
	//} 
}



void SPWindowEditMultiLine::ProcessCandidate()
{
	//////////////////////////////////////////////////////////////////////////
	// Candidate Window
	SPWindow*	pWindow;
	RECT		rcCand;
	int			iPosX, iPosY, iLen;

	if(g_pInterfaceManager->GetIMEPool()->IsCandUpdate()) {
		pWindow = g_pInterfaceManager->GetCandVertical();
		if(pWindow && m_vrcCompositionCaret.empty() == false) {
			pWindow->GetWindowRect(rcCand);
			iLen = rcCand.bottom - rcCand.top;

			if( (*(m_vrcCompositionCaret.begin())).top < g_pVideo->GetScreenLenY() / 2) {
				iPosY = (*(m_vrcCompositionCaret.end() - 1)).bottom + 2;
			} else {
				iPosY = (*(m_vrcCompositionCaret.begin())).top - m_pFont->GetHeight() - 10;
			}

			iLen = rcCand.right - rcCand.left;
			iPosX = (*(m_vrcCompositionCaret.end() - 1)).left + 2;
			if( iPosX + iLen > g_pVideo->GetScreenLenX()) {
				iPosX = g_pVideo->GetScreenLenX() - iLen;
			}
			pWindow->SetAbsCoord(iPosX, iPosY);
			g_pInterfaceManager->GetIMEPool()->SetStatusWindowPos(iPosX, iPosY);
		}
		pWindow->SPSendMessage(SPIM_REFRESH);
		g_pInterfaceManager->GetIMEPool()->CandRefreshed();
	}
}


void SPWindowEditMultiLine::ProcessSelect(int iBeginLen, int iEndLen)
{
	int iLen = 0;
	int iLastLen;

	int iBeginLine = 0;
	int iEndLine = 0;
	int iBeginCaretInLine = 0;
	int iEndCaretInLine = 0;

	int iBeginSel = iBeginLen;
	int iEndSel = iEndLen;
	bool bCaretBegin;


	if( iBeginSel > iEndSel ) {
		swap(iBeginSel, iEndSel);
	}

	m_iCurCaretInLine = 0;

	int iCaretPos = g_pInterfaceManager->GetIMEPool()->GetCaretPosW();
	if(iCaretPos == iBeginSel) {
		bCaretBegin = true;
	} else {
		bCaretBegin = false;
	}

	std::vector<std::wstring>::iterator IterVWStr;
	std::vector<bool>::iterator			IterBool;
	iLastLen = 0;
	IterVWStr = m_vwstrWindowText.begin();
	IterBool = m_vbWithNewLine.begin();

	for(; IterVWStr !=  m_vwstrWindowText.end(); ++IterVWStr, ++IterBool) {
		iLen = (*IterVWStr).length();
		iLastLen += iLen;
		if(*IterBool) {
			iLastLen += 1;
		}

		if(iLastLen  > iBeginSel) {
			iLastLen -= iLen;
			if(*IterBool) {
				iLastLen -= 1;
			}
			iBeginCaretInLine = iBeginSel - iLastLen;			
			break;
		}
		++iBeginLine;
	}

	iLastLen = 0;
	IterVWStr = m_vwstrWindowText.begin();
	IterBool = m_vbWithNewLine.begin();
	for(; IterVWStr !=  m_vwstrWindowText.end(); ++IterVWStr, ++IterBool) {
		iLen = (*IterVWStr).length();
		iLastLen += iLen;
		if(*IterBool) {
			iLastLen += 1;
		}

		if(iLastLen  >= iEndSel) {
			iLastLen -= iLen;
			if(*IterBool) {
				iLastLen -= 1;
			}
			iEndCaretInLine = iEndSel - iLastLen;			
			break;
		}
		++iEndLine;
	}

	if(bCaretBegin == true) {
		m_iCurLine = iBeginLine + 1;
		m_iCurCaretInLine = iBeginCaretInLine;
	} else {
		m_iCurLine = iEndLine + 1;
		m_iCurCaretInLine = iEndCaretInLine;
	}

	SIZE SizeCaret1, SizeCaret2; 
	WCHAR wszText[256];
	RECT rcTemp;

	m_vrcCaretRect.clear();
	m_vrcCompositionCaret.clear();


	if(m_vwstrWindowText.empty())
		return;

	if(iBeginLine == iEndLine) {		
		wcsncpy( wszText, (*(m_vwstrWindowText.begin() + iBeginLine)).c_str(), iBeginCaretInLine);
		wszText[iBeginCaretInLine] = 0;
		SizeCaret1 = m_pFont->GetSizeW(wszText);

		wcsncpy( wszText, (*(m_vwstrWindowText.begin() + iBeginLine)).c_str(), iEndCaretInLine);
		wszText[iEndCaretInLine] = 0;
		SizeCaret2 = m_pFont->GetSizeW(wszText);

		rcTemp.bottom = rcTemp.top = 
			m_rcWindowRect.top + ((iBeginLine + 1) * (m_pFont->GetHeight() + m_iMargin));  
		rcTemp.left = m_rcWindowRect.left + SizeCaret1.cx;
		rcTemp.right = m_rcWindowRect.left + SizeCaret2.cx;

		m_vrcCaretRect.push_back(rcTemp);	

	} else {
		wcsncpy( wszText, (*(m_vwstrWindowText.begin() + iBeginLine)).c_str(), iBeginCaretInLine);
		wszText[iBeginCaretInLine] = 0;
		SizeCaret1 = m_pFont->GetSizeW(wszText);

		rcTemp.bottom = rcTemp.top = 
			m_rcWindowRect.top + ((iBeginLine + 1) * (m_pFont->GetHeight() + m_iMargin));  
		rcTemp.left = m_rcWindowRect.left + SizeCaret1.cx;
		rcTemp.right = m_rcWindowRect.right;

		m_vrcCaretRect.push_back(rcTemp);


		wcsncpy( wszText, (*(m_vwstrWindowText.begin() + iEndLine)).c_str(), iEndCaretInLine);
		wszText[iEndCaretInLine] = 0;
		SizeCaret2 = m_pFont->GetSizeW(wszText);

		rcTemp.bottom = rcTemp.top = 
			m_rcWindowRect.top + ((iEndLine + 1) * (m_pFont->GetHeight() + m_iMargin));  
		rcTemp.left = m_rcWindowRect.left;
		rcTemp.right = m_rcWindowRect.left + SizeCaret2.cx;

		m_vrcCaretRect.push_back(rcTemp);


		rcTemp.left = m_rcWindowRect.left;
		rcTemp.right = m_rcWindowRect.right;
		for(int i = iBeginLine+1; i < iEndLine; ++i) {
			rcTemp.bottom = rcTemp.top = 
				m_rcWindowRect.top + ((i + 1) * (m_pFont->GetHeight() + m_iMargin));

			m_vrcCaretRect.push_back(rcTemp);
		}
	}
}

void SPWindowEditMultiLine::CalcMaxLine()
{
	int iLineSize = m_pFont->GetHeight() + m_iMargin;
	int iWindowSize = m_rcWindowRect.bottom - m_rcWindowRect.top;
	int iTemp = iLineSize;
	m_iMaxLine = 1;
	while(1) {
		iTemp += iLineSize;
		if(iTemp > iWindowSize)
			break;
		++m_iMaxLine;
	}	
}

void SPWindowEditMultiLine::Render(float fTime)
{	
	if ( !m_bShow ) 
		return;	

	SPWindow::Render(fTime);	

	if( g_pVideo == NULL )
		return;

	if( m_pFont == NULL )
		return;
	
	g_pVideo->Flush();

	// Render String
	bool bTemp = m_pFont->IsShadow();	
	if( !m_vwstrWindowText.empty()) {

		m_pFont->SetColor( m_FontColor		);
		m_pFont->SetEffect( m_FontEffect );

		RECT rcLine;
		std::wstring *pwstrLine;
		char szTempBuf[512];

		rcLine = m_rcWindowRect;
		rcLine.bottom = rcLine.top + m_pFont->GetHeight();		

		std::vector<std::wstring>::iterator IterWindowText = m_vwstrWindowText.begin();
		for(; IterWindowText != m_vwstrWindowText.end(); ++IterWindowText) {
			pwstrLine = &( *IterWindowText );
			//if(pwstrLine->empty())
			//	break;

			SPAbuseFilter::GetInstance()->ConvertWideToAnsiString(szTempBuf, pwstrLine->c_str());
			m_pFont->RenderText( szTempBuf, &rcLine, m_dwAlign);

			rcLine.top += (m_pFont->GetHeight() + m_iMargin);
			rcLine.bottom = rcLine.top + m_pFont->GetHeight();
		}
	}


	// Render Caret
	RECT rcTemp;
	float fRestore = g_pVideo->GetSysLine()->GetWidth();

	if(m_FontColor.r == 0 && m_FontColor.g == 0  && m_FontColor.b == 0) {
		g_pVideo->GetSysLine()->SetColor(0x88000000);
	} else {				
		g_pVideo->GetSysLine()->SetColor(m_FontColor);
	}			


	if(m_bFocus && (m_bTwinkleCursor || m_bSelect)) {

		if(m_bSelect) {
			g_pVideo->GetSysLine()->SetColor(0x88000000);			
			g_pVideo->GetSysLine()->SetWidth( m_pFont->GetHeight() );

			
			std::vector<RECT>::iterator IterRC = m_vrcCaretRect.begin();
			for(;IterRC != m_vrcCaretRect.end(); ++IterRC) {
				rcTemp = (*IterRC);
				rcTemp.top -= (m_pFont->GetHeight()/2 + m_iMargin);
				rcTemp.bottom -= (m_pFont->GetHeight()/2 + m_iMargin);
				g_pVideo->GetSysLine()->DrawLine(rcTemp);
			}			
		} else {	
			//if(m_FontColor.r == 0 && m_FontColor.g == 0  && m_FontColor.b == 0) {
			//	g_pVideo->GetSysLine()->SetColor(0x88000000);
			//} else {				
			//	g_pVideo->GetSysLine()->SetColor(m_FontColor);
			//}			

			if(m_vrcCaretRect.empty()) {
				g_pVideo->GetSysLine()->DrawLine(m_rcCaretRect);
			} else {
				std::vector<RECT>::iterator IterRC = m_vrcCaretRect.begin();
				for(;IterRC != m_vrcCaretRect.end(); ++IterRC) {
					rcTemp = (*IterRC);
					g_pVideo->GetSysLine()->DrawLine(rcTemp);
				}

				IterRC = m_vrcCompositionCaret.begin();
				for(;IterRC != m_vrcCompositionCaret.end(); ++IterRC) {
					rcTemp = (*IterRC);
					rcTemp.top -= 1;
					rcTemp.bottom -= 1;
					g_pVideo->GetSysLine()->DrawLine(rcTemp);
				}
			}
		}
		
	}

	if(m_bFocus && !m_bSelect) {
		g_pVideo->Flush();
		std::vector<RECT>::iterator IterRC = m_vrcCaretRect.begin();
		for(;IterRC != m_vrcCaretRect.end(); ++IterRC) {
			rcTemp = (*IterRC);
			g_pVideo->GetSysLine()->DrawLine(rcTemp);
		}

		IterRC = m_vrcCompositionCaret.begin();
		for(;IterRC != m_vrcCompositionCaret.end(); ++IterRC) {
			rcTemp = (*IterRC);
			rcTemp.top -= 1;
			rcTemp.bottom -= 1;
			g_pVideo->GetSysLine()->DrawLine(rcTemp);
		}
	} 
	g_pVideo->GetSysLine()->SetWidth(fRestore);

	// for Debug
#ifdef _DEBUG	
	g_pVideo->GetSysLine()->SetColor(0x8800FF00);
	g_pVideo->GetSysLine()->DrawRect(m_rcWindowRect);
#endif

	m_pFont->SetColor();
	m_pFont->SetShadow(	bTemp );
	
}

void SPWindowEditMultiLine::Hide( bool bSendServer)
{
	if ( !m_bShow ) return;

	m_bFocus = false;
	m_bShow = false;	
	SPIMEPool* pIMEPool = g_pInterfaceManager->GetIMEPool();
	(pIMEPool)->SetFocus( false );
}

void SPWindowEditMultiLine::SetFocus()
{
	if( m_bEnable ) {
		g_pInterfaceManager->GetIMEPool()->SetFocus( true );
		SPWindow::SetFocus();
	}
}

void SPWindowEditMultiLine::KillFocus()
{
	if ( m_wstrWindowText.empty() ) {
		m_wstrWindowTextBuf.clear();
	}
	g_pInterfaceManager->GetIMEPool()->SetFocus( false );
	SPWindow::KillFocus();		
}

void SPWindowEditMultiLine::SetLimitText( int nLength )
{
	m_iLimitText = nLength;
	g_pInterfaceManager->GetIMEPool()->SetLimitText( nLength );
}

void SPWindowEditMultiLine::SetNULL()
{
	g_pInterfaceManager->GetIMEPool()->ClearBuffer();
}

void SPWindowEditMultiLine::SetWindowText(const char* pStr)
{
	WCHAR wszTemp[EDIT_BUFSIZE];	

	ZeroMemory(wszTemp, sizeof(wszTemp));
	SPAbuseFilter::GetInstance()->ConvertAnsiStringToWide( wszTemp, pStr);

	SetWindowFormatText(L"%s", wszTemp);


	std::wstring wstrForInsert;
	std::wstring::iterator IterWStr;	
	SIZE strSize;
	//WCHAR wszTemp[256];
	int iLen ;
	int iLastLen ;	

	m_vwstrWindowText.clear();
	m_vbWithNewLine.clear();

	iLen = 0;
	iLastLen = 0;
	IterWStr = m_wstrWindowText.begin() + 0;
	for(; IterWStr !=  m_wstrWindowText.end(); ++IterWStr, ++iLen) {				
		wcsncpy(wszTemp, &(*(m_wstrWindowText.begin()+iLastLen)), iLen-iLastLen);
		wszTemp[iLen-iLastLen] = 0; //Null Terminator

		strSize = m_pFont->GetSizeW(wszTemp);

		if(strSize.cx > (m_rcWindowRect.right - m_rcWindowRect.left - 12)  || (*IterWStr) == '\n' /* \n == 10 */ ) { // 12 ｴﾂ ｿｩﾀｯｺﾐ
			wstrForInsert.clear();					
			wstrForInsert = m_wstrWindowText.substr(iLastLen, iLen-iLastLen);
			iLastLen = iLen;
			m_vwstrWindowText.push_back( wstrForInsert );
			if(m_wstrWindowText.length() > (iLen+1)  && (*IterWStr) == '\n') {
				++IterWStr;
				++iLastLen;
				++iLen;
				m_vbWithNewLine.push_back(true);
			} else {
				m_vbWithNewLine.push_back(false);
			}			
		}
	}

	wstrForInsert.clear();
	wstrForInsert = m_wstrWindowText.substr(iLastLen);

	m_vwstrWindowText.push_back( wstrForInsert );			
	m_vbWithNewLine.push_back(false);



	//GetWindowRect(m_rcWindowRect);
	//m_rcTextRect = m_rcWindowRect;
	//m_rcTextRect.left += m_iMarginX;
	//m_rcTextRect.top += m_iMarginY;
	return;;


	// Old 
	//SetWindowFormatText(pStr);
	//return;
}

void SPWindowEditMultiLine::SetWindowFormatText( const WCHAR* wszText, ... ) 
{ 
	if( wszText == NULL || lstrlenW(wszText) <= 0 )	{
		m_wstrWindowText.clear();
		m_wstrWindowTextBuf.clear();
		g_pInterfaceManager->GetIMEPool()->SetStringW( m_wstrWindowText.c_str() );

		m_vwstrWindowText.clear();
		m_vbWithNewLine.clear();
		return;
	}

	WCHAR wszbuff[4096];
	va_list va;

	va_start( va, wszText );
	vswprintf( wszbuff, wszText, va );
	va_end( va );

	m_wstrWindowText = wszbuff;

	if( m_bFocus )
		g_pInterfaceManager->GetIMEPool()->SetStringW( m_wstrWindowText.c_str() );
}

const char* SPWindowEditMultiLine::GetWindowText()
{
	SPAbuseFilter::GetInstance()->ConvertWideToAnsiString(m_szWindowText, m_wstrWindowText.c_str());
	m_szWindowText[m_iLimitText] = 0;
	return m_szWindowText;	
}

//////////////////////////////////////////////////////////////////////////
// Message Map
SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowEditMultiLine )
	SPIMESSAGE_COMMAND( SPIM_SETFOCUS,			OnSelFocus		)
	SPIMESSAGE_COMMAND( SPIM_KILLFOCUS,			OnKillFocus		)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,		OnCursorLDown	)
	
	SPIMESSAGE_COMMAND( SPIM_WHEEL_UP,			OnWheelUp		)
	SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN,		OnWheelDown		)
	SPIMESSAGE_COMMAND( SPIM_KEY_DOWN,			OnKeyDown		)
	SPIMESSAGE_COMMAND( SPIM_SET_CARET_POS,		OnSetCaretPos	)
	SPIMESSAGE_COMMAND( SPIM_SET_MARGIN,		OnSetMargin		)
	SPIMESSAGE_COMMAND( SPIM_GET_ENTER_ENABLE,	OnGetEnableEnter)		
	
SPIMESSAGE_OBJ_MAP_END( SPWindow )

int SPWindowEditMultiLine::OnCursorLDown( WPARAM wParam, LPARAM lParam )
{
	return OnSelFocus(wParam, lParam);
}

int SPWindowEditMultiLine::OnSelFocus( WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable )	{
		SPIMEPool* pIMEPool = g_pInterfaceManager->GetIMEPool();

		if(m_bEnableEnter)
			pIMEPool->SetMultiLine();

		pIMEPool->SetLimitText( m_iLimitText );		
		pIMEPool->SetStringW( m_wstrWindowText.c_str() );
		pIMEPool->SetFocus( true );
		pIMEPool->SetSelW(); // Deselect

		m_bFocus = true;
		
	}
	return SPSendMessageToParent( SPIM_SETFOCUS, 0, 0, this );
}

int SPWindowEditMultiLine::OnKillFocus( WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable )	{ // ｿｵｹｮ ｸ蟾ﾎ ｹﾙｲﾛｴﾙ
		g_pInterfaceManager->GetIMEPool()->PreKillFocus();
		ProcessString();	
		g_pInterfaceManager->GetIMEPool()->KillFocus();
		// AJJIYA [9/7/2005]
		// ｿｵﾇﾑ ｹｰﾀｸｷﾎ ｺｯｰ貮ﾏｱ・ﾀｧﾇﾘｼｭﾀﾓ.
//		g_pInterfaceManager->GetIMEPool()->EnableEnglishMode();
		KillFocus();
	}
	return SPSendMessageToParent( SPIM_KILLFOCUS, 0, 0, this );
}


int SPWindowEditMultiLine::OnWheelUp( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_WHEEL_UP, wParam, lParam, this);
}


int SPWindowEditMultiLine::OnWheelDown( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_WHEEL_DOWN, wParam, lParam, this);
}

int SPWindowEditMultiLine::OnGetEnableEnter( WPARAM wParam, LPARAM lParam)
{
	BOOL BEnable;
	if(IsEnableEnter()) BEnable = TRUE;
	else BEnable = FALSE;	

	return BEnable;
}


int SPWindowEditMultiLine::OnKeyDown( WPARAM wParam, LPARAM lParam)
{
	std::wstring wstrTemp;
	std::wstring wstrTemp2;

	switch(wParam) {
	case VK_UP:
		{
			if(m_vwstrWindowText.empty())
				return 0;

			if(m_iCurLine == 1) {
				return 0; // ﾃｳﾀｽﾀﾌｸ・ｸｮﾅﾏ
			} else {			
				if(m_iCurLine > m_vwstrWindowText.size()) {
					assert(0);
					m_iCurLine = m_vwstrWindowText.size();
				}

				if(m_iCurLine < 1) {
					assert(0);
					m_iCurLine = 1;					
				}


				wstrTemp = *(m_vwstrWindowText.begin() + (m_iCurLine-1));		// CurLine
				wstrTemp2 = *(m_vwstrWindowText.begin() + (m_iCurLine-2));	// PrevLine

				std::vector<bool>::iterator Iterb = m_vbWithNewLine.begin() + (m_iCurLine-2);


				int iCaretPos = g_pInterfaceManager->GetIMEPool()->GetCaretPosW();
				int iAdjust = iCaretPos - m_iCurCaretInLine - wstrTemp2.length();
				if(m_iCurCaretInLine > wstrTemp2.length()) {
					if(*Iterb) {
						iAdjust += (wstrTemp2.length() -1 );
						m_iCurCaretInLine = (wstrTemp2.length() -1 );
					} else {
						iAdjust += (wstrTemp2.length());
						m_iCurCaretInLine = (wstrTemp2.length());
					}

				} else {					
					if(*Iterb) {
						iAdjust += ((m_iCurCaretInLine)-1);
						//m_iCurCaretInLine = m_iCurCaretInLine-1;
					} else {
						iAdjust += m_iCurCaretInLine;
					}
				}

				if(*(m_wstrWindowText.begin() + iAdjust) == L'\n')
					--iAdjust;

				if(iAdjust < 0) {
					iAdjust = 0;					
				}

				int iSelBegin, iSelEnd;
				g_pInterfaceManager->GetIMEPool()->SetCaretPosW(iAdjust);
				g_pInterfaceManager->GetIMEPool()->GetSelW(iSelBegin, iSelEnd);

				if( GetKeyState( VK_SHIFT ) >= 0 ) {
					g_pInterfaceManager->GetIMEPool()->Deselect();
				}				
			}
		}
		break;

	case VK_DOWN:
		{
			if(m_vwstrWindowText.empty())
				return 0;

			if(m_iCurLine == m_vwstrWindowText.size()) {
				return 0; // ｸｶﾁｷﾀﾌｸ・ｸｮﾅﾏ
			} else {	

				if(m_iCurLine > m_vwstrWindowText.size()) {
					assert(0);
					m_iCurLine = m_vwstrWindowText.size();					
				}

				if(m_iCurLine < 1) {
					assert(0);
					m_iCurLine = 1;					
				}

				wstrTemp = *(m_vwstrWindowText.begin() + (m_iCurLine-1));		// CurLine
				wstrTemp2 = *(m_vwstrWindowText.begin() + (m_iCurLine));	// NextLine

				std::vector<bool>::iterator Iterb = m_vbWithNewLine.begin() + (m_iCurLine-1);

				int iCaretPos = g_pInterfaceManager->GetIMEPool()->GetCaretPosW();
				int iAdjust = iCaretPos + (wstrTemp.length() - m_iCurCaretInLine)  ;
				if(m_iCurCaretInLine > wstrTemp2.length()) {
					if(*Iterb) {
						iAdjust += (wstrTemp2.length());
						m_iCurCaretInLine = (wstrTemp2.length());
					} else {
						iAdjust += (wstrTemp2.length());
						m_iCurCaretInLine = (wstrTemp2.length());
					}

				} else {
					if(*Iterb) {
						iAdjust += (m_iCurCaretInLine + 1);						
					} else {
						iAdjust += (m_iCurCaretInLine + 1);
					}
				}

				if(*(m_wstrWindowText.begin() + iAdjust) == L'\n')					
					++iAdjust;

				if(iAdjust >= m_wstrWindowText.length()) {
					iAdjust = m_wstrWindowText.length();					
				}


				int iSelBegin, iSelEnd;
				g_pInterfaceManager->GetIMEPool()->SetCaretPosW(iAdjust);
				g_pInterfaceManager->GetIMEPool()->GetSelW(iSelBegin, iSelEnd);

				if( GetKeyState( VK_SHIFT ) >= 0 ) {
					g_pInterfaceManager->GetIMEPool()->Deselect();
				}
			}
		}
		break;
	default:
		break;
	}

	return SPSendMessageToParent(SPIM_KEY_DOWN, wParam, lParam, this);
}

int SPWindowEditMultiLine::OnSetCaretPos( WPARAM wParam, LPARAM lParam)
{
	switch(wParam) {	
	case CPOS_BEGIN	:
		g_pInterfaceManager->GetIMEPool()->SetCaretPosW(0);
		break;
	case CPOS_END	:
		g_pInterfaceManager->GetIMEPool()->SetCaretPosW(-1);
		break;
	case CPOS_CUR	:
		g_pInterfaceManager->GetIMEPool()->SetCaretPosW((int)lParam);
		break;	
	}
	return -1;
}

int SPWindowEditMultiLine::OnSetMargin( WPARAM wParam, LPARAM lParam)
{
	m_iMargin = (int)wParam;
	CalcMaxLine();
	return 1;
}

