
#include <assert.h>
#include <D3D9.h>
#include <vector>
#include <string>
#include "DXUtil.h"
#include "CD3DEnumeration.h"
#include "D3DSettings.h"

#include "SPVideoCommon.h"
#include "SPVideo.h"
#include "SPTexture.h"
#include "SPLine.h"
#include "SPFont.h"

#include "../spfont/FontManager.h"

bool IsWhiteSpace(WCHAR wchar)
{
	if( wchar == 0x20 || wchar == 0x8140 || wchar == 0x3000 ) { // white space half and full
		return true;
	}
	return false;
}

bool IsAlphanumeric(WCHAR wchar)
{
	if(( wchar >='0' && wchar <='9' ) || 
		( wchar >='a' && wchar <='z' ) || 
		( wchar >='A' && wchar <='Z' ) ||
		( wchar == 0 )) {
			return true;
		}
	return false;
}


SPFont::SPFont()
:m_fScaleX(1.0f)
,m_fScaleY(1.0f)
,m_pD3DXFont(0) 
,m_bPeriod(false)
,m_FontEffect(SPFONT_NULL)
{
}

SPFont::~SPFont()
{
	SAFE_RELEASE(m_pD3DXFont);
}

HRESULT SPFont::Init(LPCSTR szFaceName, INT iHeight, UINT iFontWeight, BOOL bItalic)
{
	HRESULT hr;
	m_fHeight = (float)iHeight;
	m_uiWeight = iFontWeight;
	m_D3DShadowColor = INITSHADOWCOLOR ;

	// Initialize the font
	hr = D3DXCreateFont( 
			g_pVideo->GetDevice(), 			
			(int)m_fHeight,					//Height
			0,								//Width			
			iFontWeight,					//Weight
			1,								//MipLevel
			bItalic,						//Italic
			DEFAULT_CHARSET,				//CharSet
			OUT_DEFAULT_PRECIS,				//OutputPrecision
			DEFAULT_QUALITY,				//Quality
			DEFAULT_PITCH | FF_DONTCARE,	//PitchAndFamily
			szFaceName,						//FaceName
			&m_pD3DXFont );					//Font

	//"HY°ß¸íÁ¶",
	
	if(hr == D3D_OK ) { 
		if( m_pD3DXFont )
			m_pD3DXFont->GetDesc(&m_FontDesc);
		SetColor();
	} else {
		m_pD3DXFont = NULL;
	}

	SetColor(0xffffffff);

	return hr;
}

void SPFont::SetColor(const D3DXCOLOR Color)
{ 
	m_D3DColor = Color; 
}

void SPFont::SetShadow(bool bShadow) 
{ 
	if(bShadow) {
		m_FontEffect = SPFONT_SHADOW;
	} else {
		m_FontEffect = SPFONT_NULL;
	}
}

void SPFont::SetShadowColor(const D3DXCOLOR Color)
{ 
	m_D3DShadowColor = Color; 
}

D3DXCOLOR SPFont::GetShadowColor()
{ 
	return m_D3DShadowColor; 
}

bool SPFont::IsShadow()	
{ 	
	return (m_FontEffect == SPFONT_SHADOW);
}

void SPFont::SetEdge(bool bShadow) 
{ 
	if(bShadow) {
		m_FontEffect = SPFONT_EDGE;
	} else {
		m_FontEffect = SPFONT_NULL;
	}
}

bool SPFont::IsEdge()	
{ 	
	return (m_FontEffect == SPFONT_EDGE);
}

void SPFont::SetEffect(SPFONT_EFFECT spfe)
{
	m_FontEffect = spfe;
}

SPFONT_EFFECT SPFont::GetEffect()
{
	return m_FontEffect;
}

void SPFont::SetPeriod(bool bPeriod)
{
	m_bPeriod = bPeriod;
}

void SPFont::SetScale(float fx, float fy)
{
	m_fScaleX = fx;
	m_fScaleY = fy;
}

float SPFont::GetHeight()
{
	return m_fHeight * m_fScaleX;
}

int SPFont::GetWidth()
{
	return (int)(m_FontDesc.Width * m_fScaleX);
}

HRESULT SPFont::OnLostDevice()
{
	if(m_pD3DXFont)
		return m_pD3DXFont->OnLostDevice();
	else
		return D3DERR_NOTFOUND ;
}

HRESULT SPFont::OnResetDevice()
{
	if(m_pD3DXFont)
		return m_pD3DXFont->OnResetDevice();
	else 
		return D3DERR_NOTFOUND;
}

HRESULT SPFont::RenderText(LPCSTR szCaption, RECT* pRect, DWORD dwFormat, INT iCount)
{
	//return E_FAIL;

	//if( m_pD3DXFont == NULL )
	//	return E_FAIL;

	LPCSTR szString = szCaption;
	if( m_bPeriod )
		szString = ConvertStringPeriod(szCaption, *pRect);
	
	if( szString == NULL )
		return E_FAIL;

	//g_pVideo->Flush(); // Z-Order Reset

	//D3DXMATRIXA16 mat;
	//D3DXMatrixIdentity( &mat );

	//if( m_fScaleX != 1.0f && m_fScaleY != 1.0f) {
	//	D3DXVECTOR2 Center	= D3DXVECTOR2((float)pRect->left * 1.5f,		(float)pRect->top *1.5f);
	//	D3DXVECTOR2 Trans	= D3DXVECTOR2((float)pRect->left*(1-m_fScaleX), (float)pRect->top*(1-m_fScaleY));
	//	D3DXMatrixAffineTransformation2D(&mat, m_fScaleX, &Center, 0, &Trans);
	//} 
	//g_pVideo->GetSprite()->SetTransform(&mat);

	//HRESULT hr;


	//switch(m_FontEffect) {
	//case SPFONT_SHADOW:
	//	{
	//		//´Ü¼ø ±×¸²ÀÚ
	//		//RECT rcShadow = *pRect;
	//		//OffsetRect( &rcShadow, 1, 1 );
	//		//hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		//if( FAILED(hr) )
	//		//	return hr;

	//		//È®´ë ±×¸²ÀÚ
	//		RECT rcShadow;
	//		rcShadow = *pRect;
	//		OffsetRect( &rcShadow, 0, 1 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, &rcShadow, dwFormat, m_D3DShadowColor);

	//		rcShadow = *pRect;
	//		OffsetRect( &rcShadow, 1, 0 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, &rcShadow, dwFormat, m_D3DShadowColor);

	//		rcShadow = *pRect;
	//		OffsetRect( &rcShadow, 0, -1 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, &rcShadow, dwFormat, m_D3DShadowColor);

	//		rcShadow = *pRect;
	//		OffsetRect( &rcShadow, -1, 0 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, &rcShadow, dwFormat, m_D3DShadowColor);

	//	}
	//	break;
	//case SPFONT_EDGE:
	//	{			
	//		RECT rcShadow;
	//		rcShadow = *pRect;
	//		OffsetRect( &rcShadow, 0, 1 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, &rcShadow, dwFormat, m_D3DShadowColor);

	//		rcShadow = *pRect;
	//		OffsetRect( &rcShadow, 1, 0 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, &rcShadow, dwFormat, m_D3DShadowColor);

	//		rcShadow = *pRect;
	//		OffsetRect( &rcShadow, 0, -1 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, &rcShadow, dwFormat, m_D3DShadowColor);

	//		rcShadow = *pRect;
	//		OffsetRect( &rcShadow, -1, 0 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, &rcShadow, dwFormat, m_D3DShadowColor);

	//		//rcShadow = *pRect;
	//		//OffsetRect( &rcShadow, 1, 1 );
	//		//hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, &rcShadow, dwFormat, m_D3DShadowColor);

	//		//rcShadow = *pRect;
	//		//OffsetRect( &rcShadow, 1, -1 );
	//		//hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, &rcShadow, dwFormat, m_D3DShadowColor);

	//		//rcShadow = *pRect;
	//		//OffsetRect( &rcShadow, -1, -1 );
	//		//hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, &rcShadow, dwFormat, m_D3DShadowColor);

	//		//rcShadow = *pRect;
	//		//OffsetRect( &rcShadow, -1, 1 );
	//		//hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, &rcShadow, dwFormat, m_D3DShadowColor);



	//	}
	//	break;
	//case SPFONT_NULL:
	//default:
	//    break;
	//}

	g_pVideo->Flush();

//	if(m_iFontWight == 700)
//		dwFormat |= DTF_Shadow;

	if(strcmp(szCaption, "ÏòÕ½¶·ÓÃÆ·µêÒÆ¶¯") == 0)
	{
		int kk = 0;
	}

	//[xialin 2008/08/19]add >>>
	if ( m_FontEffect & SPFONT_SHADOW )
		dwFormat |= DTF_BlackFrame;
	if ( m_FontEffect & SPFONT_EDGE )
		dwFormat |= DTF_BlackFrame;
	if ( m_uiWeight == FW_BOLD && m_fHeight != 14.0f )
		dwFormat |= DTF_Bold;
	g_Font.SetFont(FONT_SONGTI);
	//[xialin 2008/08/19]add <<<

	
	g_Font.DrawText(pRect,szCaption,m_D3DColor, m_D3DShadowColor, m_fHeight,dwFormat);

	g_pVideo->Flush();
	//hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szString, iCount, pRect, dwFormat, m_D3DColor );
	//return hr;

	return E_FAIL;

}


HRESULT SPFont::RenderTextPos(int iX, int iY, LPCSTR szCaption, float fRadian, DWORD dwFormat, INT iCount)
{
	if( m_pD3DXFont == NULL )
		return E_FAIL;

	g_pVideo->Flush(); // Z-Order Reset

	//D3DXMATRIXA16 mat;
	//D3DXMatrixIdentity( &mat );
	//D3DXVECTOR2 Center = D3DXVECTOR2((float)iX*m_fScaleX, (float)iY*m_fScaleY);
	//D3DXVECTOR2 Trans = D3DXVECTOR2((float)iX* (1.f - m_fScaleX), (float)iY* (1.f - m_fScaleY));	

	//D3DXMatrixAffineTransformation2D(&mat, m_fScaleX, &Center, fRadian, &Trans);
	//g_pVideo->GetSprite()->SetTransform(&mat);

	//HRESULT hr;
	RECT Rect;
	SetRect(&Rect, iX, iY, 0, 0);

	RECT RectEx = Rect;
	//RectEx.left -= 1;
	//RectEx.top  -= 1;
	//RectEx.right += 1;
	//RectEx.bottom += 1;

	//if(m_iFontWight == 700)
	//	dwFormat |= DTF_Shadow;

	g_Font.DrawText(&RectEx,szCaption,m_D3DColor,m_D3DShadowColor,m_fHeight,dwFormat);

	return E_FAIL;

	//RECT rcShadow;

	//switch(m_FontEffect) {
	//case SPFONT_SHADOW:
	//	{
	//		rcShadow = RectEx;
	//		OffsetRect( &rcShadow, 0, 1 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		if( FAILED(hr) )
	//			return hr;

	//		rcShadow = RectEx;
	//		OffsetRect( &rcShadow, 1, 0 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		if( FAILED(hr) )
	//			return hr;

	//		rcShadow = RectEx;
	//		OffsetRect( &rcShadow, 0, -1 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		if( FAILED(hr) )
	//			return hr;

	//		rcShadow = RectEx;
	//		OffsetRect( &rcShadow, -1, 0 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		if( FAILED(hr) )
	//			return hr;
	//	}
	//	break;
	//case SPFONT_EDGE:
	//	{			
	//		rcShadow = RectEx;
	//		OffsetRect( &rcShadow, 0, 2 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		if( FAILED(hr) )
	//			return hr;

	//		rcShadow = RectEx;
	//		OffsetRect( &rcShadow, 2, 0 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		if( FAILED(hr) )
	//			return hr;

	//		rcShadow = RectEx;
	//		OffsetRect( &rcShadow, 0, -2 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		if( FAILED(hr) )
	//			return hr;

	//		rcShadow = RectEx;
	//		OffsetRect( &rcShadow, -2, 0 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		if( FAILED(hr) )
	//			return hr;

	//		rcShadow = RectEx;
	//		OffsetRect( &rcShadow, 2, 2 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		if( FAILED(hr) )
	//			return hr;

	//		rcShadow = RectEx;
	//		OffsetRect( &rcShadow, -2, -2 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		if( FAILED(hr) )
	//			return hr;

	//		rcShadow = RectEx;
	//		OffsetRect( &rcShadow, 2, -2 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		if( FAILED(hr) )
	//			return hr;

	//		rcShadow = RectEx;
	//		OffsetRect( &rcShadow, -2, 2 );
	//		hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &rcShadow, dwFormat, D3DCOLOR_ARGB(DWORD(m_D3DColor.a * 255), 0, 0, 0) );
	//		if( FAILED(hr) )
	//			return hr;

	//	}
	//	break;
	//case SPFONT_NULL:
	//default:
	//	break;
	//}



	//hr = m_pD3DXFont->DrawText( g_pVideo->GetSprite(), szCaption, iCount, &Rect, dwFormat, m_D3DColor );	
	//return hr;

}

SIZE SPFont::GetSize(const char* pszString)
{
	SIZE szRet = {0, 0};

	if( m_pD3DXFont == NULL || pszString == NULL )
		return szRet;

	HDC hDC = m_pD3DXFont->GetDC();	
	GetTextExtentPoint32A(hDC, pszString, strlen(pszString), &szRet);

	return szRet;
}


SIZE SPFont::GetSizeW(const WCHAR* pwszString)
{
	SIZE szRet = {0, 0};

	if( m_pD3DXFont == NULL || pwszString == NULL )
		return szRet;

	HDC hDC = m_pD3DXFont->GetDC();	
	GetTextExtentPoint32W(hDC, pwszString, lstrlenW(pwszString), &szRet);

	return szRet;
}

SIZE SPFont::GetSizeW(const char *pszString)
{
	WCHAR szTemp[MAX_STR_LENGTH];
	int iLen = strlen(pszString)+1;
	MultiByteToWideChar( CP_ACP, 0, pszString, -1, 
		szTemp, iLen - 1);	
	szTemp[iLen - 1] = 0;
	return GetSizeW( szTemp );
}

const char* SPFont::GetStringClipping(const char* pszString, const RECT& rcRgn, int& iWidthCharCount)
{
	static char s_szRetBuf[MAX_STR_LENGTH];
	s_szRetBuf[0] = '\0';

	if( pszString == NULL )
		return NULL;

	int iWidthClipping = rcRgn.right - rcRgn.left;
	int iStringLength = (int)strlen(pszString);

	iWidthCharCount = 0;
	int iFirstByte = 0;
	bool bHangleExist = false;

	char szTemp[MAX_STR_LENGTH];
	SIZE csSize;
	while( 1 )
	{
		if( iWidthCharCount >= iStringLength )
			break;

		if( pszString[iWidthCharCount] == '\n' || pszString[iWidthCharCount] == '\0' )
			break;

		if( pszString[iWidthCharCount] & 0x80 )
		{
			iFirstByte = 1 - iFirstByte;
			bHangleExist = true;
		}
		else if( iFirstByte )
		{
			iFirstByte = 0;
			bHangleExist = true;
		}
		else
		{
			iFirstByte = 0;
			bHangleExist = false;
		}

		strncpy(szTemp, pszString, iWidthCharCount + 1);
		szTemp[iWidthCharCount + 1] = '\0';

		csSize = GetSize(szTemp);
		if( csSize.cx > iWidthClipping )
		{
			if( bHangleExist && !iFirstByte )
				iWidthCharCount -= 2;
			else
				iWidthCharCount -= 1;

			break;
		}

		iWidthCharCount++;
	}

	strncpy(s_szRetBuf, pszString, iWidthCharCount + 1);
	s_szRetBuf[iWidthCharCount + 1] = '\0';

	return s_szRetBuf;
}

const WCHAR* SPFont::GetStringClippingW(const WCHAR* pszString, const RECT& rcRgn, int& iWidthCharCount)
{
	static WCHAR s_szRetBuf[MAX_STR_LENGTH];
	s_szRetBuf[0] = '\0';

	if( pszString == NULL )
		return NULL;

	int iWidthClipping = rcRgn.right - rcRgn.left;
	int iStringLength = (int)lstrlenW(pszString);
	SIZE csSize;
	WCHAR szTemp[MAX_STR_LENGTH];
	int iLastSpace;

	iWidthCharCount = 0;
	iLastSpace = -1;
	while( 1 )
	{
		if( iWidthCharCount >= iStringLength )
			break;

		if( pszString[iWidthCharCount] == '\n' || pszString[iWidthCharCount] == '\0' )
			break;

		if( IsWhiteSpace(pszString[iWidthCharCount])) {
			iLastSpace = iWidthCharCount;
		}

		wcsncpy(szTemp, pszString, sizeof(WCHAR) * (iWidthCharCount + 1));
		szTemp[iWidthCharCount + 1] = '\0';
		
		csSize = GetSizeW(szTemp);
		if( csSize.cx >= iWidthClipping ) {
			if(iLastSpace != -1 && IsAlphanumeric(pszString[iWidthCharCount])) {
				iWidthCharCount = iLastSpace;
			} else {
				iWidthCharCount--;
			}			
			break;
		}
		iWidthCharCount++;
	}

	wcsncpy(s_szRetBuf, pszString, sizeof(WCHAR) * (iWidthCharCount + 1));
	s_szRetBuf[iWidthCharCount + 1] = '\0';

	return s_szRetBuf;
}

const WCHAR* SPFont::ConvertStringPeriod(const WCHAR* pszString, const RECT& rcRgn)
{
	static WCHAR s_szRetBuf[MAX_STR_LENGTH];
	s_szRetBuf[0] = '\0';

	if( pszString == NULL )
		return NULL;

	int i;
	int iWidthClipping = rcRgn.right - rcRgn.left;
	int iStringLength = (int)lstrlenW(pszString);
	int iWidthCharCount;
	
	const WCHAR* pszClipping = GetStringClippingW(pszString, rcRgn, iWidthCharCount);
	if( pszClipping == NULL )
		return NULL;

	wcscpy(s_szRetBuf, pszClipping);
	if( iWidthCharCount != iStringLength )
	{
		int iIndex, iLoopCount;
		iLoopCount = 0;

		while( 1 )
		{
			iIndex = iWidthCharCount - iLoopCount;
			if( iIndex < 0 )
				break;

			for( i = 0; i < 3; i++ )
				s_szRetBuf[iIndex + i] = '.';
			
			s_szRetBuf[iIndex + 3] = '\0';
			iStringLength = (int)lstrlenW(s_szRetBuf);
			GetStringClippingW(s_szRetBuf, rcRgn, iWidthCharCount);
			
			if( iWidthCharCount == iStringLength )
				break;

			iLoopCount++;
		}
	}

	return s_szRetBuf;
}

const char* SPFont::ConvertStringPeriod(const char* pszString, const RECT& rcRgn)
{
	static char s_szRetBuf[MAX_STR_LENGTH];
	s_szRetBuf[0] = '\0';

	WCHAR szTemp[MAX_STR_LENGTH];
	int iLen = strlen(pszString)+1;
	MultiByteToWideChar( CP_ACP, 0, pszString, -1, 
		szTemp, iLen - 1);	
	szTemp[iLen - 1] = 0;

	const WCHAR* pszConvertString = ConvertStringPeriod(szTemp, rcRgn);
	if( pszConvertString == NULL )
		return NULL;

	iLen = WideCharToMultiByte( CP_ACP, 0, pszConvertString, -1, 
		s_szRetBuf, MAX_STR_LENGTH, NULL, NULL );	

	s_szRetBuf[iLen] = '\0';
	return s_szRetBuf;
}

bool SPFont::ConvertMultiLineW(const char* pszString, const RECT& rcRgn, std::vector< std::string >& vTextList)
{
	if( pszString == NULL )
		return false;

	WCHAR szTemp[MAX_STR_LENGTH], szClipTemp[MAX_STR_LENGTH], szClipString[MAX_STR_LENGTH];
	char szBuf[MAX_STR_LENGTH];
	const WCHAR* pcwLineText;

	int iLen = strlen(pszString)+1;
	MultiByteToWideChar( CP_ACP, 0, pszString, -1, szTemp, iLen - 1);	
	szTemp[iLen - 1] = 0;
	wcscpy(szClipTemp, szTemp);
	wcscpy(szClipString, szTemp);

	int iCharCount = 0;
	while( 1 )
	{
		pcwLineText = GetStringClippingW(szClipString, rcRgn, iCharCount);
		if( pcwLineText == NULL )
			break;

		iLen = WideCharToMultiByte( CP_ACP, 0, pcwLineText, -1, szBuf, MAX_STR_LENGTH, NULL, NULL );	
		szBuf[iLen] = '\0';
		vTextList.push_back(szBuf);

		int iSize = (int)lstrlenW(szClipString);
		int iCopyCount = iSize - iCharCount;
		if( iCopyCount <= 0 )
			break;

		wcsncpy(szClipString, &szClipTemp[iCharCount + 1], sizeof(WCHAR) * iCopyCount);
		szClipString[iCopyCount] = '\0';
		wcscpy(szClipTemp, szClipString);
	}
	return true;
}


bool SPFont::ConvertMultiLineW(const char* pszString, const RECT& rcRgn, std::vector< std::wstring >& vwTextList)
{
	if( pszString == NULL )
		return false;

	WCHAR szTemp[MAX_STR_LENGTH], szClipTemp[MAX_STR_LENGTH], szClipString[MAX_STR_LENGTH];
	char szBuf[MAX_STR_LENGTH];
	const WCHAR* pcwLineText;

	int iLen = strlen(pszString)+1;
	MultiByteToWideChar( CP_ACP, 0, pszString, -1, szTemp, iLen - 1);	
	szTemp[iLen - 1] = 0;
	wcscpy(szClipTemp, szTemp);
	wcscpy(szClipString, szTemp);

	int iCharCount = 0;
	while( 1 )
	{
		pcwLineText = GetStringClippingW(szClipString, rcRgn, iCharCount);
		if( pcwLineText == NULL )
			break;

		//iLen = WideCharToMultiByte( CP_ACP, 0, pcwLineText, -1, szBuf, MAX_STR_LENGTH, NULL, NULL );	
		//szBuf[iLen] = '\0';
		vwTextList.push_back(pcwLineText);

		int iSize = (int)lstrlenW(szClipString);
		int iCopyCount = iSize - iCharCount;
		if( iCopyCount <= 0 )
			break;

		wcsncpy(szClipString, &szClipTemp[iCharCount + 1], sizeof(WCHAR) * iCopyCount);
		szClipString[iCopyCount] = '\0';
		wcscpy(szClipTemp, szClipString);
	}
	return true;
}


