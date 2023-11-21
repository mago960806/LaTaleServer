
// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment : 
// Creation : metalgeni 2004-08-23 오후 4:48:46
//***************************************************************************
#pragma once

#ifndef INITCOLOR
#define INITCOLOR		0xffffffff
#endif

#define INITSHADOWCOLOR		0xb2000000			// D3DCOLOR_ARGB(178, 0, 0, 0)

#define MAX_STR_LENGTH		512

struct D3DXCOLOR;

#include <vector>

enum SPFONT_EFFECT {
	SPFONT_NULL		=		0x01,
	SPFONT_SHADOW	=		0x02,
	SPFONT_EDGE		=		0x04,
};


bool IsWhiteSpace(WCHAR wchar);
bool IsAlphanumeric(WCHAR wchar);

class SPFont {
public:
	SPFont();
	~SPFont();

	HRESULT Init(LPCSTR szFaceName = "Arial", INT iHeight = 20, UINT iFontWeight = FW_BOLD, BOOL bItalic = FALSE);
	HRESULT OnLostDevice();
	HRESULT OnResetDevice();

	HRESULT RenderText(LPCSTR szCaption, RECT* pRect, DWORD dwFormat = DT_TOP | DT_LEFT, INT iCount = -1);
	HRESULT RenderTextPos(int iX, int iY, LPCSTR szCaption, float fRadian = 0.f ,DWORD dwFormat = DT_TOP | DT_LEFT | DT_NOCLIP, INT iCount = -1);

	void SetColor(const D3DXCOLOR Color = INITCOLOR);//RGBA
	void SetScale(float fx = 1.0f, float fy = 1.0f);

	void SetShadowColor(const D3DXCOLOR Color = INITSHADOWCOLOR);
	D3DXCOLOR GetShadowColor() ;

	//기존버젼 호환때문에 
	void SetShadow(bool bShadow = true); 
	bool IsShadow();
	void SetEdge(bool bShadow = true);
	bool IsEdge();

	// new~~
	void SetEffect(SPFONT_EFFECT spfe);
	SPFONT_EFFECT GetEffect();

	void SetPeriod(bool bPeriod = true);

	float GetHeight();
	int GetWidth();
	SIZE GetSize(const char* pszString);
	SIZE GetSizeW(const WCHAR* pwszString);
	SIZE GetSizeW( const char *pszString );

	const char* GetStringClipping(const char* pszString, const RECT& rcRgn, int& iWidthCharCount);
	const WCHAR* GetStringClippingW(const WCHAR* pszString, const RECT& rcRgn, int& iWidthCharCount);
	const WCHAR* ConvertStringPeriod(const WCHAR* pszString, const RECT& rcRgn);
	const char* ConvertStringPeriod(const char* pszString, const RECT& rcRgn);

	bool ConvertMultiLineW(const char* pszString, const RECT& rcRgn, std::vector< std::string >& vTextList);
	bool ConvertMultiLineW(const char* pszString, const RECT& rcRgn, std::vector< std::wstring >& vwTextList);

	
protected:
	float					m_fScaleX;
	float					m_fScaleY;

	ID3DXFont*				m_pD3DXFont;
	D3DXFONT_DESC			m_FontDesc;

	D3DXCOLOR				m_D3DColor;
	D3DXCOLOR				m_D3DShadowColor;
	//bool					m_bShadow;

	SPFONT_EFFECT			m_FontEffect;

	float					m_fHeight;
	UINT					m_uiWeight;   //[xialin 2008/08/19]add
	bool					m_bPeriod;
	
};



