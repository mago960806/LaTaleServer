


#include <assert.h>
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include <D3D9.h>
#include <d3dx9.h>
#include <vector>
#include <string>

#include "DXUtil.h"
#include "CD3DEnumeration.h"
#include "D3DSettings.h"
#include "SPFont.h"
#include "SPLine.h"

#include "SPTextureMgr.h"
#include "SPVideoCommon.h"
#include "SPVideo.h"

#include "IGWInterface.h"
#include "IGWLoaderDx9.h"

SPVideo* g_pVideo = NULL;


const char GLOBAL_FONT_KOREAN[] = { 0xb1, 0xbc, 0xb8, 0xb2, '\0', 0 }; //"奔覆"; font name GULIM
//const char GLOBAL_FONT_JAPANESE[] = "俵俽 俹僑僔僢僋";

const char GLOBAL_FONT_JAPANESE[] = { 
	0x82, 0x6c, 0x82, 0x72, 0x20, 0x82, 0x6f, 0x83, 0x53, 0x83, 0x56, 0x83, 0x62, 0x83, 0x4e, 0x00
}; //"俵俽 俹僑僔僢僋"; font name MS P Godic


CRITICAL_SECTION g_TextureLoadSection;


inline int RampVal(UINT i, float fRecipGamma, int iScale = 65535)
{
	return static_cast<int>(iScale*pow(i/255.f, fRecipGamma));
}

inline float RecipGamma(UINT i)
{
	return logf(i/255.f)/logf(0.5f);
}

//-----------------------------------------------------------------------------
// Name: D3DUtil_SetDeviceCursor
// Desc: Gives the D3D device a cursor with image and hotspot from hCursor.
//-----------------------------------------------------------------------------
HRESULT D3DUtil_SetDeviceCursor( LPDIRECT3DDEVICE9 pd3dDevice, HCURSOR hCursor,
								BOOL bAddWatermark )
{
	HRESULT hr = E_FAIL;
	ICONINFO iconinfo;
	BOOL bBWCursor;
	LPDIRECT3DSURFACE9 pCursorSurface = NULL;
	HDC hdcColor = NULL;
	HDC hdcMask = NULL;
	HDC hdcScreen = NULL;
	BITMAP bm;
	DWORD dwWidth;
	DWORD dwHeightSrc;
	DWORD dwHeightDest;
	COLORREF crColor;
	COLORREF crMask;
	UINT x;
	UINT y;
	BITMAPINFO bmi;
	COLORREF* pcrArrayColor = NULL;
	COLORREF* pcrArrayMask = NULL;
	DWORD* pBitmap;
	HGDIOBJ hgdiobjOld;

	ZeroMemory( &iconinfo, sizeof(iconinfo) );
	if( !GetIconInfo( hCursor, &iconinfo ) )
		goto End;

	if (0 == GetObject((HGDIOBJ)iconinfo.hbmMask, sizeof(BITMAP), (LPVOID)&bm))
		goto End;
	dwWidth = bm.bmWidth;
	dwHeightSrc = bm.bmHeight;

	if( iconinfo.hbmColor == NULL )
	{
		bBWCursor = TRUE;
		dwHeightDest = dwHeightSrc / 2;
	}
	else 
	{
		bBWCursor = FALSE;
		dwHeightDest = dwHeightSrc;
	}

	// Create a surface for the fullscreen cursor
	if( FAILED( hr = pd3dDevice->CreateOffscreenPlainSurface( dwWidth, dwHeightDest, 
		D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &pCursorSurface, NULL ) ) )
	{
		goto End;
	}

	pcrArrayMask = new DWORD[dwWidth * dwHeightSrc];

	ZeroMemory(&bmi, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = dwWidth;
	bmi.bmiHeader.biHeight = dwHeightSrc;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	hdcScreen = GetDC( NULL );
	hdcMask = CreateCompatibleDC( hdcScreen );
	if( hdcMask == NULL )
	{
		hr = E_FAIL;
		goto End;
	}
	hgdiobjOld = SelectObject(hdcMask, iconinfo.hbmMask);
	GetDIBits(hdcMask, iconinfo.hbmMask, 0, dwHeightSrc, 
		pcrArrayMask, &bmi, DIB_RGB_COLORS);
	SelectObject(hdcMask, hgdiobjOld);

	if (!bBWCursor)
	{
		pcrArrayColor = new DWORD[dwWidth * dwHeightDest];
		hdcColor = CreateCompatibleDC( hdcScreen );
		if( hdcColor == NULL )
		{
			hr = E_FAIL;
			goto End;
		}
		SelectObject(hdcColor, iconinfo.hbmColor);
		GetDIBits(hdcColor, iconinfo.hbmColor, 0, dwHeightDest, 
			pcrArrayColor, &bmi, DIB_RGB_COLORS);
	}

	// Transfer cursor image into the surface
	D3DLOCKED_RECT lr;
	pCursorSurface->LockRect( &lr, NULL, 0 );
	pBitmap = (DWORD*)lr.pBits;
	for( y = 0; y < dwHeightDest; y++ )
	{
		for( x = 0; x < dwWidth; x++ )
		{
			if (bBWCursor)
			{
				crColor = pcrArrayMask[dwWidth*(dwHeightDest-1-y) + x];
				crMask = pcrArrayMask[dwWidth*(dwHeightSrc-1-y) + x];
			}
			else
			{
				crColor = pcrArrayColor[dwWidth*(dwHeightDest-1-y) + x];
				crMask = pcrArrayMask[dwWidth*(dwHeightDest-1-y) + x];
			}
			if (crMask == 0)
				pBitmap[dwWidth*y + x] = 0xff000000 | crColor;
			else
				pBitmap[dwWidth*y + x] = 0x00000000;

			// It may be helpful to make the D3D cursor look slightly 
			// different from the Windows cursor so you can distinguish 
			// between the two when developing/testing code.  When
			// bAddWatermark is TRUE, the following code adds some
			// small grey "D3D" characters to the upper-left corner of
			// the D3D cursor image.
			if( bAddWatermark && x < 12 && y < 5 )
			{
				// 11.. 11.. 11.. .... CCC0
				// 1.1. ..1. 1.1. .... A2A0
				// 1.1. .1.. 1.1. .... A4A0
				// 1.1. ..1. 1.1. .... A2A0
				// 11.. 11.. 11.. .... CCC0

				const WORD wMask[5] = { 0xccc0, 0xa2a0, 0xa4a0, 0xa2a0, 0xccc0 };
				if( wMask[y] & (1 << (15 - x)) )
				{
					pBitmap[dwWidth*y + x] |= 0xff808080;
				}
			}
		}
	}
	pCursorSurface->UnlockRect();

	// Set the device cursor
	if( FAILED( hr = pd3dDevice->SetCursorProperties( iconinfo.xHotspot, 
		iconinfo.yHotspot, pCursorSurface ) ) )
	{
		goto End;
	}

	hr = S_OK;

End:
	if( iconinfo.hbmMask != NULL )
		DeleteObject( iconinfo.hbmMask );
	if( iconinfo.hbmColor != NULL )
		DeleteObject( iconinfo.hbmColor );
	if( hdcScreen != NULL )
		ReleaseDC( NULL, hdcScreen );
	if( hdcColor != NULL )
		DeleteDC( hdcColor );
	if( hdcMask != NULL )
		DeleteDC( hdcMask );
	SAFE_DELETE_ARRAY( pcrArrayColor );
	SAFE_DELETE_ARRAY( pcrArrayMask );
	SAFE_RELEASE( pCursorSurface );
	return hr;
}


SPVideo::SPVideo()
{    
	m_pD3D              = NULL;
	m_pd3dDevice        = NULL;
	m_hWnd              = NULL;
	m_hWndFocus         = NULL;	
	m_TextureCreateFormat = D3DFMT_UNKNOWN;
	m_pBackBuffer		= NULL;	
	
	//m_bActive           = false;
	m_bDeviceObjectsInited = false;
	m_bDeviceObjectsRestored = false;
	m_dwCreateFlags     = 0;

	m_bDeviceLost       = false;
	m_bMinimized        = false;
	m_bMaximized        = false;
	m_bIgnoreSizeChange = false;
	m_bCanGamma			= false;

	m_strDeviceStats[0] = _T('\0');
	m_strFrameStats[0]  = _T('\0');

	m_strWindowTitle    = _T("SPVideo");
	m_dwCreationWidth   = 0;
	m_dwCreationHeight  = 0;
	m_bShowCursorWhenFullscreen = true;
	m_bStartFullscreen  = false;

	m_bWindowed         = true;

	m_bIsRender			= false;
	m_bIsRenderSprite	= false;
	m_pd3dxSrite		= NULL;
	m_pSysLine			= NULL;
	ZeroMemory(m_paFont, sizeof(SPFont*) * FONT_MAX);

	m_pTextureMgr		= NULL;
	m_bActive			= true;
	m_bActiveReady		= false;
	m_bFirstFrame		= false;
	m_bDialogBoxMode	= false;

	strcpy(m_strCompiled, COMPILED);
	m_d3dColorClear = D3DCOLOR_ARGB(0, 128, 128, 128);

	// When m_bClipCursorWhenFullscreen is true, the cursor is limited to
	// the device window when the app goes fullscreen.  This prevents users
	// from accidentally clicking outside the app window on a multimon system.
	// This flag is turned off by default for debug builds, since it makes 
	// multimon debugging difficult.
#if defined(_DEBUG) || defined(DEBUG)
	m_bClipCursorWhenFullscreen = false;	
#else
	m_bClipCursorWhenFullscreen = true;
#endif

}

SPVideo::~SPVideo()
{
	SAFE_RELEASE( m_pTextureMgr );
	DeleteDeviceObjects();

	SAFE_RELEASE( m_pBackBuffer );
	SAFE_RELEASE( m_pd3dDevice );
	SAFE_RELEASE( m_pD3D );
}

HRESULT SPVideo::Init( HWND hWnd, DWORD uiWidth, DWORD uiHeight, bool bWindowed, VIDEO_QUALITY VideoQuality, bool bClipChild  )
{	
	m_hWnd				= hWnd;
	m_dwCreationWidth   = uiWidth;
	m_dwCreationHeight  = uiHeight;
	m_bWindowed			= bWindowed;
	m_bStartFullscreen	= !bWindowed;	
	m_bClipChild		= bClipChild;

	HRESULT hr;

	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );//产生IDirect3D9*
	if( NULL == m_pD3D )
	{
		MessageBox( m_hWnd, "Direct3D can't create. 扼抛老篮 DirectX 9.0c客 弊客 龋券登绰 弊贰侨 墨靛啊 鞘夸钦聪促.", "Display Error", MB_OK|MB_ICONERROR );

		return E_FAIL;
	}

	m_d3dEnumeration.SetD3D( m_pD3D );
	m_d3dEnumeration.ConfirmDeviceCallback = ConfirmDeviceHelper;
	//if( FAILED( hr = m_d3dEnumeration.Enumerate(m_dwCreationWidth, m_dwCreationHeight) ) )
	if( FAILED( hr = m_d3dEnumeration.Enumerate() ) )
	{
		SAFE_RELEASE( m_pD3D );
		MessageBox( m_hWnd, "EnumerateDevices failed.", "Display Error", MB_OK|MB_ICONERROR );
		return E_FAIL;
	}

	if( m_hWndFocus == NULL )
		m_hWndFocus = m_hWnd;


	// Save window properties
	m_dwWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );
	GetWindowRect( m_hWnd, &m_rcWindowBounds );
	GetClientRect( m_hWnd, &m_rcWindowClient );

	if( FAILED( hr = ChooseInitialD3DSettings() ) )
	{
		SAFE_RELEASE( m_pD3D );
		MessageBox( m_hWnd, "Device choose failed.", "Display Error", MB_OK|MB_ICONERROR );
		return E_FAIL;
	}	
	
	if( FAILED( hr = Initialize3DEnvironment() ) )
	{
		SAFE_RELEASE( m_pD3D );
		MessageBox( m_hWnd, "Device create failed.", "Display Error", MB_OK|MB_ICONERROR );
		return E_FAIL;
	}


	switch( VideoQuality ) {
	case VIDEOQUAL_HIGH:
		m_TextureCreateFormat = D3DFMT_A8R8G8B8;
		break;
		
	case VIDEOQUAL_LOW:
		m_TextureCreateFormat = D3DFMT_DXT5;
		break;

	case VIDEOQUAL_NULL:
		m_TextureCreateFormat = D3DFMT_A8R8G8B8;
		assert(0 && "Invalid Video Option");
		break;
	}

	ZeroMemory( &m_GammaRamp, sizeof(m_GammaRamp));
	ZeroMemory( &m_ColorRampMin, sizeof(D3DXCOLOR));
	ZeroMemory( &m_ColorRampMax, sizeof(D3DXCOLOR));

	m_ColorRampMin.r = m_ColorRampMin.g = m_ColorRampMin.b = 0.5f;
	m_ColorRampMin.a = 1.0f;
	m_ColorRampMax.r = m_ColorRampMax.g = m_ColorRampMax.b = 1.0f;
	m_ColorRampMax.a = 1.0f;


	m_iGammaGray = GAMMA_INITIAL;

	//const float fRecip = RecipGamma( m_iGammaGray );
	//m_WindowRamp = D3DCOLOR_XRGB(	RampVal(6,	fRecip, 255),
	//								RampVal(19,	fRecip, 255),
	//								RampVal(19,	fRecip, 255));


	//m_pd3dDevice->GetGammaRamp(0, &m_GammaRamp);	

	m_pTextureMgr = SPTextureMgr::Create();

	return S_OK;
}

void SPVideo::SetClearColor(D3DCOLOR d3dColorClear)
{
	m_d3dColorClear = d3dColorClear;
}

D3DCOLOR SPVideo::GetClearColor()
{
	return m_d3dColorClear;
}


bool SPVideo::IsWindowed()
{
	return m_bWindowed;
}

HWND SPVideo::GetMainHWND()
{
	return m_hWnd;
}


bool SPVideo::SetClipChildMode(bool bClipChild)
{
	m_bDialogBoxMode = bClipChild;
	HRESULT hr;
	//HRESULT hr = m_pd3dDevice->SetDialogBoxMode(bClipChild);
	//if(SUCCEEDED(hr))

	if(m_bDialogBoxMode) {
		hr = m_pd3dDevice->SetDialogBoxMode(true);
	}


	return true;
	//else 
	//	return false;
}


void SPVideo::UpdateRamp()
{
	if ( m_bCanGamma ) {
		// Only Single Gamma

		// compute 1/gamma
		const float fRecipGray = RecipGamma(m_iGammaGray);
		// compute i**(1/gamma) for all i and scale to range
		for (UINT i = 0; i < 256; i++) {
			m_GammaRamp.red[i] = m_GammaRamp.green[i] = m_GammaRamp.blue[i] = RampVal(i, fRecipGray);
		}
	} else {
		for (UINT i = 0; i < 256; i++) {			
			m_GammaRamp.red[i] = m_GammaRamp.green[i] = m_GammaRamp.blue[i] = 257 * i; // 65535 * i/255
		}
	}
	
	//const float fRecip = RecipGamma( m_iGammaGray );
	//m_WindowRamp = D3DCOLOR_XRGB(	RampVal(6,	fRecip, 255),
	//								RampVal(19,	fRecip, 255),
	//								RampVal(19,	fRecip, 255));

	if (m_bCanGamma) {
		m_pd3dDevice->SetGammaRamp(0, D3DSGR_NO_CALIBRATION, &m_GammaRamp);
	}


}


void SPVideo::SetContrast(int iContrast)
{

	m_iGammaGray = iContrast;
	if(m_iGammaGray > GAMMA_MAX) 
		m_iGammaGray = GAMMA_MAX;
	if(m_iGammaGray < GAMMA_MIN) 
		m_iGammaGray = GAMMA_MIN;

	UpdateRamp();

	return ;



	if(iContrast == 0)
		iContrast = 1;

	int iLow = iContrast;
	int iHigh = 256;
	int iResult;

	for(int i = 0; i < 256; ++i)  {
		iResult = (int)(((1-i/255.f)*iLow + i/255.f*iHigh)*257.f);
		for(int j = 0; j < iContrast && i+j < 256; ++j)  {
			m_GammaRamp.red		[i+j] = iResult;
			m_GammaRamp.green	[i+j] = iResult;
			m_GammaRamp.blue	[i+j] = iResult;			
		}
	}
	m_pd3dDevice->SetGammaRamp(0, D3DSGR_NO_CALIBRATION, &m_GammaRamp);	
}



D3DFORMAT SPVideo::GetTextureCreateFormat() 
{
	return m_TextureCreateFormat;
}

bool SPVideo::SurfaceCaptureFileName(const char* pszFileName)
{	
	HRESULT hrRet = S_FALSE;

	if(pszFileName == NULL) return false;
	if(strlen(pszFileName) == 0) return false;

	EnterCriticalSection(&g_TextureLoadSection) ;

	if(m_bDeviceLost == false) {
		hrRet = D3DXSaveSurfaceToFile(pszFileName, D3DXIFF_JPG, m_pBackBuffer, NULL, NULL);
	}

	LeaveCriticalSection(&g_TextureLoadSection) ;

	if(hrRet != S_OK)
		return false;	

	return true;
}



bool SPVideo::SurfaceCaptureJPG(IN int* piCount)
{
	static int iFileCount = 0;
	char strFileName[256];
	HRESULT hrRet = S_FALSE;

	EnterCriticalSection(&g_TextureLoadSection) ;

	CreateDirectory("Screenshots", NULL);

	for(;iFileCount < 9999;++iFileCount)  {
		sprintf(strFileName,	"Screenshots\\SPSCF%04d.JPG", iFileCount);
		FILE *hFile = fopen(strFileName, "rb");
		if(hFile == NULL) break;
		fclose(hFile);
	}

	if(piCount)
		*piCount = iFileCount;

	if(m_bDeviceLost == false) {
		hrRet = D3DXSaveSurfaceToFile(strFileName, D3DXIFF_JPG, m_pBackBuffer, NULL, NULL);
	}

	LeaveCriticalSection(&g_TextureLoadSection) ;

	if(hrRet != S_OK)
		return false;	

	return true;
}

bool SPVideo::SurfaceCapturePNG(IN int* piCount)
{
	static int iFileCount = 0;
	char strFileName[256];
	HRESULT hrRet = S_FALSE;

	EnterCriticalSection(&g_TextureLoadSection) ;

	CreateDirectory("Screenshots", NULL);

	for(;iFileCount < 9999; ++iFileCount)  {
		sprintf(strFileName,	"Screenshots\\SPSCF%04d.PNG", iFileCount);
		FILE *hFile = fopen(strFileName, "rb");
		if(hFile == NULL) break;
		fclose(hFile);
	}

	if(piCount)
		*piCount = iFileCount;

	if(m_bDeviceLost == false) {
		hrRet = D3DXSaveSurfaceToFile(strFileName, D3DXIFF_PNG, m_pBackBuffer, NULL, NULL);
	}

	LeaveCriticalSection(&g_TextureLoadSection) ;

	if(hrRet != S_OK)
		return false;	

	return true;
}


bool SPVideo::SurfaceCaptureBMP(IN int* piCount)
{
	static int iFileCount = 0;
	char strFileName[256];
	HRESULT hrRet = S_FALSE;

	EnterCriticalSection(&g_TextureLoadSection) ;

	CreateDirectory("Screenshots", NULL);

	for(;iFileCount < 9999;++iFileCount)  {
		sprintf(strFileName,	"Screenshots\\SPSCF%04d.BMP", iFileCount);
		FILE *hFile = fopen(strFileName, "rb");
		if(hFile == NULL) break;
		fclose(hFile);
	}

	if(piCount)
		*piCount = iFileCount;

	if(m_bDeviceLost == false) {
		hrRet = D3DXSaveSurfaceToFile(strFileName, D3DXIFF_BMP, m_pBackBuffer, NULL, NULL);
	}

	LeaveCriticalSection(&g_TextureLoadSection) ;

	if(hrRet != S_OK)
		return false;	

	return true;
}



bool SPVideo::Flush()
{
	if( !m_bIsRenderSprite || m_pd3dxSrite == NULL || m_bDeviceLost )
		return false;

	if(m_bActive == false)
		return false;

	if(m_bIsRender == false)
		return false;

	if(m_bFirstFrame == true)
		return false;

	HRESULT result;
	//
	try
	{
		result = m_pd3dxSrite->Flush();
	}
	catch (...)
	{
		
	}
	return true;
}


void SPVideo::Active()
{	
	//m_bActive = true;
	m_bActiveReady = true;
}

void SPVideo::Deactive()
{
	if(m_bWindowed == false)
		m_bActive = false;
}


bool SPVideo::IsActive()
{
	return m_bActive;
}

bool SPVideo::CheckActiveReady()
{
	if(m_bActiveReady == true) {
		m_bActiveReady = false;
		m_bActive = true;
		m_bFirstFrame = true;
		return true;
	}

	return false;
}


bool SPVideo::TestToRestore()
{
	HRESULT hr = 0;
	// Test the cooperative level to see if it's okay to render
	if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) ) {
		// If the device was lost, do not render until we get it back
		if( D3DERR_DEVICELOST == hr )
			return false;//return S_OK;
		//return false;

		// Check if the device needs to be reset.
		if( D3DERR_DEVICENOTRESET == hr ) {
			// If we are windowed, read the desktop mode and use the same format for
			// the back buffer
			//if( m_bWindowed ) {
			if( m_d3dSettings.IsWindowed ) {

				D3DAdapterInfo* pAdapterInfo = m_d3dSettings.PAdapterInfo();
				m_pD3D->GetAdapterDisplayMode( pAdapterInfo->AdapterOrdinal, &m_d3dSettings.Windowed_DisplayMode );
				m_d3dpp.BackBufferFormat = m_d3dSettings.Windowed_DisplayMode.Format;
			} else {
				D3DAdapterInfo* pAdapterInfo = m_d3dSettings.PAdapterInfo();
				m_pD3D->GetAdapterDisplayMode( pAdapterInfo->AdapterOrdinal, &m_d3dSettings.Fullscreen_DisplayMode);
				m_d3dpp.BackBufferFormat = m_d3dSettings.Fullscreen_DisplayMode.Format;

			}

			if( FAILED( hr = Reset3DEnvironment() ) )
				return false;
		}
		return false;//return true;
	}
	m_bDeviceLost = false;
	return true;
}


void SPVideo::SetIngameDM	(DEVMODE* pDM)
{
	if(pDM != NULL) 
		CopyMemory(&m_dmIngame, pDM, sizeof(DEVMODE));
}

bool SPVideo::GetIngameDM	(DEVMODE* pDM)
{
	if(pDM != NULL) 
		CopyMemory(pDM, &m_dmIngame, sizeof(DEVMODE));	

	return true;
}


bool SPVideo::BeginRender()
{
	HRESULT hr = 0;
	assert(m_bIsRender == false);

	if( m_bDeviceLost ) {
		TestToRestore();
	}

	try
	{
		//m_pd3dxSrite->Flush();
		//hr = m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000F0F, 1.0f, 0L );
		//hr = m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET , D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);
		hr = m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET , m_d3dColorClear, 1.0f, 0);
		//hr = m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET , m_WindowRamp, 1.0f, 0);	

		if( SUCCEEDED( m_pd3dDevice->BeginScene() ) ) {
			if(m_pd3dxSrite) {			
				m_pd3dxSrite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE); // org
				//m_pd3dxSrite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE | D3DXSPRITE_DO_NOT_ADDREF_TEXTURE );
				
				//m_pd3dxSrite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT);			
				//m_pd3dxSrite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT);
				//m_pd3dxSrite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK);
			}
			m_bIsRenderSprite = true;
			return m_bIsRender = true;
		}
		return m_bIsRender = false;

	}
	catch (...)
	{
		m_bIsRenderSprite = false;
		return m_bIsRender = false;		
	}
}

bool SPVideo::EndRender()
{
	HRESULT hr = D3DERR_DEVICELOST;

	try {	
		if(m_bActive != false)
		{
			if(m_bIsRenderSprite) 
			{ 
				if( m_bFirstFrame == false)
				{
					if(m_pd3dxSrite) 
						m_pd3dxSrite->End();
					m_bIsRenderSprite = false;
				} 
			}
		}
		hr = m_pd3dDevice->EndScene();
		hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	}
	catch(...)
	{

	}
	
	if( D3DERR_DEVICELOST == hr )
		m_bDeviceLost = true;

	if( m_bFirstFrame == true)
		m_bFirstFrame = false;
	
	return m_bIsRender = false;
}


HRESULT SPVideo::SPSetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
	try	{
		return m_pd3dDevice->SetRenderState(State, Value);
	}
	catch (...)
	{
		return E_FAIL;
	}	
}


void SPVideo::SetAlphaTexture(bool bVal)
{
	if(bVal) {
		Flush(); 		
		SPSetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		SPSetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		
	} else {
		Flush();
		SPSetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		SPSetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		Flush(); // Z-Order Reset
	}
}


void SPVideo::SetTextureEffect( TEXTURE_EFFECT eEffectType )
{
	//////////////////////////////////////////////////////////////////////////
	//
	// AJJIYA [12/1/2005]
	//
	// 酗壅? ???E?绚? 师?
	//

	DWORD	dwSrcValue	=	D3DBLEND_SRCALPHA;
	DWORD	dwDestValue	=	D3DBLEND_INVSRCALPHA;
	DWORD	dwBlendOP	=	D3DBLENDOP_ADD;

	switch( eEffectType)
	{
	case TEXTURE_EFFECT_NORMAL:
		dwSrcValue	=	D3DBLEND_SRCALPHA;
		dwDestValue	=	D3DBLEND_ONE;
		break;
	case TEXTURE_EFFECT_MULTIPLY:
		dwSrcValue	=	D3DBLEND_DESTCOLOR;
		dwDestValue	=	D3DBLEND_ZERO;
		break;
	case TEXTURE_EFFECT_ADDITIVE:
		dwSrcValue	=	D3DBLEND_ONE;
		dwDestValue	=	D3DBLEND_ONE;
		break;
	case TEXTURE_EFFECT_SUBTRACTIVE:
		dwSrcValue	=	D3DBLEND_ONE;
		dwBlendOP	=	D3DBLENDOP_SUBTRACT;
		dwDestValue	=	D3DBLEND_ONE;
		break;
	case TEXTURE_EFFECT_SCREEN:
		dwSrcValue	=	D3DBLEND_INVDESTCOLOR;
		dwDestValue	=	D3DBLEND_ONE;
		break;
	case TEXTURE_EFFECT_LIGHTEN:
		dwSrcValue	=	D3DBLEND_ONE;
		dwBlendOP	=	D3DBLENDOP_MAX;
		dwDestValue	=	D3DBLEND_ONE;
		break;
	case TEXTURE_EFFECT_DARKEN:
		dwSrcValue	=	D3DBLEND_ONE;
		dwBlendOP	=	D3DBLENDOP_MIN;
		dwDestValue	=	D3DBLEND_ONE;
		break;
	}

	Flush();
	SPSetRenderState( D3DRS_SRCBLEND	,	dwSrcValue	);
	SPSetRenderState( D3DRS_BLENDOP		,	dwBlendOP	);
	SPSetRenderState( D3DRS_DESTBLEND	,	dwDestValue	);

	if( eEffectType == TEXTURE_EFFECT_NULL )
		Flush();
}


bool SPVideo::ConfirmDeviceHelper( D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType, 
										  D3DFORMAT backBufferFormat )
{
	DWORD dwBehavior;

	if (vertexProcessingType == SOFTWARE_VP)
		dwBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else if (vertexProcessingType == MIXED_VP)
		dwBehavior = D3DCREATE_MIXED_VERTEXPROCESSING;
	else if (vertexProcessingType == HARDWARE_VP)
		dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else if (vertexProcessingType == PURE_HARDWARE_VP)
		dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_MULTITHREADED;
	else
		dwBehavior = 0; // TODO: throw exception

	return SUCCEEDED( ConfirmDevice( pCaps, dwBehavior, backBufferFormat ) );
}


//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device initialization, this code checks the display device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT SPVideo::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
										 D3DFORMAT Format )
{
	UNREFERENCED_PARAMETER( Format );
	UNREFERENCED_PARAMETER( dwBehavior );
	UNREFERENCED_PARAMETER( pCaps );

	BOOL bCapsAcceptable;

	// TODO: Perform checks to see if these display caps are acceptable.
	bCapsAcceptable = TRUE;

	if( bCapsAcceptable )         
		return S_OK;
	else
		return E_FAIL;
}


//-----------------------------------------------------------------------------
// Name: FindBestWindowedMode()
// Desc: Sets up m_d3dSettings with best available windowed mode, subject to 
//       the bRequireHAL and bRequireREF constraints.  Returns false if no such
//       mode can be found.
//-----------------------------------------------------------------------------
bool SPVideo::FindBestWindowedMode( bool bRequireHAL, bool bRequireREF )
{
	// Get display mode of primary adapter (which is assumed to be where the window 
	// will appear)
	D3DDISPLAYMODE primaryDesktopDisplayMode;
	m_pD3D->GetAdapterDisplayMode(0, &primaryDesktopDisplayMode);

	D3DAdapterInfo* pBestAdapterInfo = NULL;
	D3DDeviceInfo* pBestDeviceInfo = NULL;
	D3DDeviceCombo* pBestDeviceCombo = NULL;

	for( UINT iai = 0; iai < m_d3dEnumeration.m_pAdapterInfoList->Count(); iai++ )
	{
		D3DAdapterInfo* pAdapterInfo = (D3DAdapterInfo*)m_d3dEnumeration.m_pAdapterInfoList->GetPtr(iai);
		for( UINT idi = 0; idi < pAdapterInfo->pDeviceInfoList->Count(); idi++ )
		{
			D3DDeviceInfo* pDeviceInfo = (D3DDeviceInfo*)pAdapterInfo->pDeviceInfoList->GetPtr(idi);
			if (bRequireHAL && pDeviceInfo->DevType != D3DDEVTYPE_HAL)
				continue;
			if (bRequireREF && pDeviceInfo->DevType != D3DDEVTYPE_REF)
				continue;
			for( UINT idc = 0; idc < pDeviceInfo->pDeviceComboList->Count(); idc++ )
			{
				D3DDeviceCombo* pDeviceCombo = (D3DDeviceCombo*)pDeviceInfo->pDeviceComboList->GetPtr(idc);
				bool bAdapterMatchesBB = (pDeviceCombo->BackBufferFormat == pDeviceCombo->AdapterFormat);
				if (!pDeviceCombo->IsWindowed)
					continue;
				if (pDeviceCombo->AdapterFormat != primaryDesktopDisplayMode.Format)
					continue;
				// If we haven't found a compatible DeviceCombo yet, or if this set
				// is better (because it's a HAL, and/or because formats match better),
				// save it
				if( pBestDeviceCombo == NULL || 
					pBestDeviceCombo->DevType != D3DDEVTYPE_HAL && pDeviceCombo->DevType == D3DDEVTYPE_HAL ||
					pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesBB )
				{
					pBestAdapterInfo = pAdapterInfo;
					pBestDeviceInfo = pDeviceInfo;
					pBestDeviceCombo = pDeviceCombo;
					if( pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesBB )
					{
						// This windowed device combo looks great -- take it
						goto EndWindowedDeviceComboSearch;
					}
					// Otherwise keep looking for a better windowed device combo
				}
			}
		}
	}
EndWindowedDeviceComboSearch:
	if (pBestDeviceCombo == NULL )
		return false;

	m_d3dSettings.pWindowed_AdapterInfo = pBestAdapterInfo;
	m_d3dSettings.pWindowed_DeviceInfo = pBestDeviceInfo;
	m_d3dSettings.pWindowed_DeviceCombo = pBestDeviceCombo;
	m_d3dSettings.IsWindowed = true;
	m_d3dSettings.Windowed_DisplayMode = primaryDesktopDisplayMode;
	m_d3dSettings.Windowed_Width = m_rcWindowClient.right - m_rcWindowClient.left;
	m_d3dSettings.Windowed_Height = m_rcWindowClient.bottom - m_rcWindowClient.top;
	if (m_d3dEnumeration.AppUsesDepthBuffer)
		m_d3dSettings.Windowed_DepthStencilBufferFormat = *(D3DFORMAT*)pBestDeviceCombo->pDepthStencilFormatList->GetPtr(0);
	m_d3dSettings.Windowed_MultisampleType = *(D3DMULTISAMPLE_TYPE*)pBestDeviceCombo->pMultiSampleTypeList->GetPtr(0);
	m_d3dSettings.Windowed_MultisampleQuality = 0;
	m_d3dSettings.Windowed_VertexProcessingType = *(VertexProcessingType*)pBestDeviceCombo->pVertexProcessingTypeList->GetPtr(0);
	m_d3dSettings.Windowed_PresentInterval = *(UINT*)pBestDeviceCombo->pPresentIntervalList->GetPtr(0);

	return true;
}




//-----------------------------------------------------------------------------
// Name: FindBestFullscreenMode()
// Desc: Sets up m_d3dSettings with best available fullscreen mode, subject to 
//       the bRequireHAL and bRequireREF constraints.  Returns false if no such
//       mode can be found.
//-----------------------------------------------------------------------------
bool SPVideo::FindBestFullscreenMode( bool bRequireHAL, bool bRequireREF, int iScrWidth, int iScrHeight )
{
	// For fullscreen, default to first HAL DeviceCombo that supports the current desktop 
	// display mode, or any display mode if HAL is not compatible with the desktop mode, or 
	// non-HAL if no HAL is available
	D3DDISPLAYMODE adapterDesktopDisplayMode;
	D3DDISPLAYMODE bestAdapterDesktopDisplayMode;
	D3DDISPLAYMODE bestDisplayMode;
	D3DDISPLAYMODE wantedAdapterDesktopDisplayMode;

	bestAdapterDesktopDisplayMode.Width = 0;
	bestAdapterDesktopDisplayMode.Height = 0;
	bestAdapterDesktopDisplayMode.Format = D3DFMT_UNKNOWN;
	bestAdapterDesktopDisplayMode.RefreshRate = 0;

	D3DAdapterInfo* pBestAdapterInfo = NULL;
	D3DDeviceInfo* pBestDeviceInfo = NULL;
	D3DDeviceCombo* pBestDeviceCombo = NULL;

	for( UINT iai = 0; iai < m_d3dEnumeration.m_pAdapterInfoList->Count(); iai++ )
	{
		D3DAdapterInfo* pAdapterInfo = (D3DAdapterInfo*)m_d3dEnumeration.m_pAdapterInfoList->GetPtr(iai);
		
		m_pD3D->GetAdapterDisplayMode( pAdapterInfo->AdapterOrdinal, &adapterDesktopDisplayMode );
		for( UINT idi = 0; idi < pAdapterInfo->pDeviceInfoList->Count(); idi++ )
		{
			D3DDeviceInfo* pDeviceInfo = (D3DDeviceInfo*)pAdapterInfo->pDeviceInfoList->GetPtr(idi);
			if (bRequireHAL && pDeviceInfo->DevType != D3DDEVTYPE_HAL)
				continue;
			if (bRequireREF && pDeviceInfo->DevType != D3DDEVTYPE_REF)
				continue;
			for( UINT idc = 0; idc < pDeviceInfo->pDeviceComboList->Count(); idc++ )
			{
				D3DDeviceCombo* pDeviceCombo = (D3DDeviceCombo*)pDeviceInfo->pDeviceComboList->GetPtr(idc);
				bool bAdapterMatchesBB = (pDeviceCombo->BackBufferFormat == pDeviceCombo->AdapterFormat);
				bool bAdapterMatchesDesktop = (pDeviceCombo->AdapterFormat == adapterDesktopDisplayMode.Format);
				if (pDeviceCombo->IsWindowed)
					continue;
				
				// If we haven't found a compatible set yet, or if this set
				// is better (because it's a HAL, and/or because formats match better),
				// save it
				if (pBestDeviceCombo == NULL ||
					pBestDeviceCombo->DevType != D3DDEVTYPE_HAL && pDeviceInfo->DevType == D3DDEVTYPE_HAL ||
					pDeviceCombo->DevType == D3DDEVTYPE_HAL && pBestDeviceCombo->AdapterFormat != adapterDesktopDisplayMode.Format && bAdapterMatchesDesktop ||
					pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB 
					)
				{
					bestAdapterDesktopDisplayMode = adapterDesktopDisplayMode;
					pBestAdapterInfo = pAdapterInfo;
					pBestDeviceInfo = pDeviceInfo;
					pBestDeviceCombo = pDeviceCombo;
					if (pDeviceInfo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB)
					{
						// This fullscreen device combo looks great -- take it
						goto EndFullscreenDeviceComboSearch;
					}
					// Otherwise keep looking for a better fullscreen device combo
				}
			}
		}
	}
EndFullscreenDeviceComboSearch:
	if (pBestDeviceCombo == NULL)
		return false;

	// Need to find a display mode on the best adapter that uses pBestDeviceCombo->AdapterFormat
	// and is as close to bestAdapterDesktopDisplayMode's res as possible

	bestDisplayMode.Width = iScrWidth;
	bestDisplayMode.Height = iScrHeight;
	bestDisplayMode.Format = D3DFMT_UNKNOWN;
	bestDisplayMode.RefreshRate = 0;

	//for( UINT idm = 0; idm < pBestAdapterInfo->pDisplayModeList->Count(); idm++ )
	//{
	//	D3DDISPLAYMODE* pdm = (D3DDISPLAYMODE*)pBestAdapterInfo->pDisplayModeList->GetPtr(idm);
	//	if( pdm->Format != pBestDeviceCombo->AdapterFormat )
	//		continue;
	//	if( pdm->Width == bestAdapterDesktopDisplayMode.Width &&
	//		pdm->Height == bestAdapterDesktopDisplayMode.Height && 
	//		pdm->RefreshRate == bestAdapterDesktopDisplayMode.RefreshRate )
	//	{
	//		// found a perfect match, so stop
	//		bestDisplayMode = *pdm;
	//		break;
	//	}
	//	else if( pdm->Width == bestAdapterDesktopDisplayMode.Width &&
	//		pdm->Height == bestAdapterDesktopDisplayMode.Height && 
	//		pdm->RefreshRate > bestDisplayMode.RefreshRate )
	//	{
	//		// refresh rate doesn't match, but width/height match, so keep this
	//		// and keep looking
	//		bestDisplayMode = *pdm;
	//	}
	//	else if( pdm->Width == bestAdapterDesktopDisplayMode.Width )
	//	{
	//		// width matches, so keep this and keep looking
	//		bestDisplayMode = *pdm;
	//	}
	//	else if( bestDisplayMode.Width == 0 )
	//	{
	//		// we don't have anything better yet, so keep this and keep looking
	//		bestDisplayMode = *pdm;
	//	}
	//}


	wantedAdapterDesktopDisplayMode.Width = iScrWidth;
	wantedAdapterDesktopDisplayMode.Height = iScrHeight;
	wantedAdapterDesktopDisplayMode.Format = D3DFMT_X8R8G8B8;	
	wantedAdapterDesktopDisplayMode.RefreshRate = 60; //?E??湮?
	//wantedAdapterDesktopDisplayMode.RefreshRate = 75;
	//wantedAdapterDesktopDisplayMode.RefreshRate = 100;


	for( UINT idm = 0; idm < pBestAdapterInfo->pDisplayModeList->Count(); idm++ )
	{
		D3DDISPLAYMODE* pdm = (D3DDISPLAYMODE*)pBestAdapterInfo->pDisplayModeList->GetPtr(idm);
		if( pdm->Format != pBestDeviceCombo->AdapterFormat )
			continue;
		if( pdm->Width == wantedAdapterDesktopDisplayMode.Width &&
			pdm->Height == wantedAdapterDesktopDisplayMode.Height && 
			pdm->RefreshRate == wantedAdapterDesktopDisplayMode.RefreshRate )
		{
			// found a perfect match, so stop
			bestDisplayMode = *pdm;
			break;
		}
		else if( pdm->Width == wantedAdapterDesktopDisplayMode.Width &&
			pdm->Height == wantedAdapterDesktopDisplayMode.Height && 			
			pdm->RefreshRate <= wantedAdapterDesktopDisplayMode.RefreshRate )
		{
			// refresh rate doesn't match, but width/height match, so keep this
			// and keep looking
			bestDisplayMode = *pdm;
		}
		else if( pdm->Width == wantedAdapterDesktopDisplayMode.Width )
		{
			// width matches, so keep this and keep looking
			bestDisplayMode = *pdm;
		}
		else if( wantedAdapterDesktopDisplayMode.Width == 0 )
		{
			// we don't have anything better yet, so keep this and keep looking
			bestDisplayMode = *pdm;
		}
	}

	m_d3dSettings.pFullscreen_AdapterInfo = pBestAdapterInfo;
	m_d3dSettings.pFullscreen_DeviceInfo = pBestDeviceInfo;
	m_d3dSettings.pFullscreen_DeviceCombo = pBestDeviceCombo;

	// current
	m_d3dSettings.IsWindowed = false;

	m_d3dSettings.Fullscreen_DisplayMode = bestDisplayMode;
	if (m_d3dEnumeration.AppUsesDepthBuffer)
		m_d3dSettings.Fullscreen_DepthStencilBufferFormat = *(D3DFORMAT*)pBestDeviceCombo->pDepthStencilFormatList->GetPtr(0);
	m_d3dSettings.Fullscreen_MultisampleType = *(D3DMULTISAMPLE_TYPE*)pBestDeviceCombo->pMultiSampleTypeList->GetPtr(0);
	m_d3dSettings.Fullscreen_MultisampleQuality = 0;
	m_d3dSettings.Fullscreen_VertexProcessingType = *(VertexProcessingType*)pBestDeviceCombo->pVertexProcessingTypeList->GetPtr(0);
	//m_d3dSettings.Fullscreen_PresentInterval = D3DPRESENT_INTERVAL_DEFAULT;
	m_d3dSettings.Fullscreen_PresentInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	if(bestDisplayMode.RefreshRate == 0)
		return false;
	
	return true;
}




HRESULT SPVideo::ChooseInitialD3DSettings()
{
	//bool bFoundFullscreen = FindBestFullscreenMode( false, false, m_dwCreationWidth, m_dwCreationHeight );
	bool bFoundFullscreen = FindBestFullscreenMode( false, false, m_dwCreationWidth, m_dwCreationHeight );
	bool bFoundWindowed = FindBestWindowedMode( false, false );

	//china 
	if( m_bStartFullscreen && bFoundFullscreen ) {
		if(m_bClipChild)
			m_d3dSettings.IsWindowed = true;
		else 
			m_d3dSettings.IsWindowed = false;
	}

	if( !bFoundWindowed && bFoundFullscreen ) {
		if(m_bClipChild)
			m_d3dSettings.IsWindowed = true;
		else 
			m_d3dSettings.IsWindowed = false;
	}

	if( !bFoundFullscreen && !bFoundWindowed )
		return D3DAPPERR_NOCOMPATIBLEDEVICES;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc: Usually this function is not overridden.  Here's what this function does:
//       - Sets the windowed flag to be either windowed or fullscreen
//       - Sets parameters for z-buffer depth and back buffer
//       - Creates the D3D device
//       - Sets the window position (if windowed, that is)
//       - Makes some determinations as to the abilites of the driver (HAL, etc)
//       - Sets up some cursor stuff
//       - Calls InitDeviceObjects()
//       - Calls RestoreDeviceObjects()
//       - If all goes well, m_bActive is set to TRUE, and the function returns
//       - Otherwise, initialization is reattempted using the reference device
//-----------------------------------------------------------------------------
HRESULT SPVideo::Initialize3DEnvironment()
{
	HRESULT hr;

	D3DAdapterInfo* pAdapterInfo = m_d3dSettings.PAdapterInfo();
	D3DDeviceInfo* pDeviceInfo = m_d3dSettings.PDeviceInfo();

	//m_bWindowed = m_d3dSettings.IsWindowed;

	// Prepare window for possible windowed/fullscreen change
	//AdjustWindowForChange();

	// Set up the presentation parameters
	BuildPresentParamsFromSettings();

	if( pDeviceInfo->Caps.PrimitiveMiscCaps & D3DPMISCCAPS_NULLREFERENCE )
	{
		// Warn user about null ref device that can't render anything
		//DisplayErrorMsg( D3DAPPERR_NULLREFDEVICE, 0 );
	}

	DWORD behaviorFlags;
	if (m_d3dSettings.GetVertexProcessingType() == SOFTWARE_VP)
		behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else if (m_d3dSettings.GetVertexProcessingType() == MIXED_VP)
		behaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
	else if (m_d3dSettings.GetVertexProcessingType() == HARDWARE_VP)
		behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else if (m_d3dSettings.GetVertexProcessingType() == PURE_HARDWARE_VP)
		behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_MULTITHREADED;
	else
		behaviorFlags = 0; // TODO: throw exception

	// Create the device
	hr = m_pD3D->CreateDevice( m_d3dSettings.AdapterOrdinal(), pDeviceInfo->DevType,
		m_hWndFocus, behaviorFlags, &m_d3dpp,
		&m_pd3dDevice );

	if( SUCCEEDED(hr) )
	{
		// When moving from fullscreen to windowed mode, it is important to
		// adjust the window size after recreating the device rather than
		// beforehand to ensure that you get the window size you want.  For
		// example, when switching from 640x480 fullscreen to windowed with
		// a 1000x600 window on a 1024x768 desktop, it is impossible to set
		// the window size to 1000x600 until after the display mode has
		// changed to 1024x768, because windows cannot be larger than the
		// desktop.
		if( m_bWindowed )
		{
			SetWindowPos( m_hWnd, HWND_NOTOPMOST,
				m_rcWindowBounds.left, m_rcWindowBounds.top,
				( m_rcWindowBounds.right - m_rcWindowBounds.left ),
				( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
				SWP_SHOWWINDOW );
		}

		// Store device Caps
		m_pd3dDevice->GetDeviceCaps( &m_d3dCaps );
		m_dwCreateFlags = behaviorFlags;


		m_bCanGamma = (m_d3dCaps.Caps2 & D3DCAPS2_FULLSCREENGAMMA) ? true : false;


		// Store device description
		if( pDeviceInfo->DevType == D3DDEVTYPE_REF )
			lstrcpy( m_strDeviceStats, TEXT("REF") );
		else if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
			lstrcpy( m_strDeviceStats, TEXT("HAL") );
		else if( pDeviceInfo->DevType == D3DDEVTYPE_SW )
			lstrcpy( m_strDeviceStats, TEXT("SW") );

		if( behaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING &&
			behaviorFlags & D3DCREATE_PUREDEVICE )
		{
			if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
				lstrcat( m_strDeviceStats, TEXT(" (pure hw vp)") );
			else
				lstrcat( m_strDeviceStats, TEXT(" (simulated pure hw vp)") );
		}
		else if( behaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING )
		{
			if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
				lstrcat( m_strDeviceStats, TEXT(" (hw vp)") );
			else
				lstrcat( m_strDeviceStats, TEXT(" (simulated hw vp)") );
		}
		else if( behaviorFlags & D3DCREATE_MIXED_VERTEXPROCESSING )
		{
			if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
				lstrcat( m_strDeviceStats, TEXT(" (mixed vp)") );
			else
				lstrcat( m_strDeviceStats, TEXT(" (simulated mixed vp)") );
		}
		else if( behaviorFlags & D3DCREATE_SOFTWARE_VERTEXPROCESSING )
		{
			lstrcat( m_strDeviceStats, TEXT(" (sw vp)") );
		}

		if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
		{
			// Be sure not to overflow m_strDeviceStats when appending the adapter 
			// description, since it can be long.  Note that the adapter description
			// is initially CHAR and must be converted to TCHAR.
			lstrcat( m_strDeviceStats, TEXT(": ") );
			const int cchDesc = sizeof(pAdapterInfo->AdapterIdentifier.Description);
			TCHAR szDescription[cchDesc];
			DXUtil_ConvertAnsiStringToGenericCch( szDescription, 
				pAdapterInfo->AdapterIdentifier.Description, cchDesc );
			int maxAppend = sizeof(m_strDeviceStats) / sizeof(TCHAR) -
				lstrlen( m_strDeviceStats ) - 1;
			_tcsncat( m_strDeviceStats, szDescription, maxAppend );
		}

		// Store render target surface desc
		//LPDIRECT3DSURFACE9 pBackBuffer = NULL;

		SAFE_RELEASE( m_pBackBuffer );
		m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
		m_pBackBuffer->GetDesc( &m_d3dsdBackBuffer );

		//pBackBuffer->Release();


			// Set up the fullscreen cursor
		//if( m_bShowCursorWhenFullscreen && !m_bWindowed )
		if( m_bShowCursorWhenFullscreen && !m_d3dSettings.IsWindowed )			
		{
			HCURSOR hCursor;
#ifdef _WIN64
			hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
			hCursor = (HCURSOR)ULongToHandle( GetClassLong( m_hWnd, GCL_HCURSOR ) );
#endif
			D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor, false );
			m_pd3dDevice->ShowCursor( true );
		}

		// Confine cursor to fullscreen window
		if( m_bClipCursorWhenFullscreen )
		{
			if (!m_bWindowed )		
			//if (!m_d3dSettings.IsWindowed )				
			{
				RECT rcWindow;
				GetWindowRect( m_hWnd, &rcWindow );
				ClipCursor( &rcWindow );
			}
			else
			{
				ClipCursor( NULL );
			}
		}



		// Initialize the app's device-dependent objects
		hr = InitDeviceObjects();
		if( FAILED(hr) )
		{
			DeleteDeviceObjects();
		}
		else
		{
			m_bDeviceObjectsInited = true;
			hr = RestoreDeviceObjects();
			if( FAILED(hr) )
			{
				InvalidateDeviceObjects();
			}
			else
			{
				m_bDeviceObjectsRestored = true;
				return S_OK;
			}
		}

		// Cleanup before we try again
		Cleanup3DEnvironment();
	}

	// If that failed, fall back to the reference rasterizer
	if( hr != D3DAPPERR_MEDIANOTFOUND && 
		hr != HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) && 
		pDeviceInfo->DevType == D3DDEVTYPE_HAL )
	{
		if (FindBestWindowedMode(false, true))
		{
			m_bWindowed = true;
			//AdjustWindowForChange();

			// Make sure main window isn't topmost, so error message is visible
			SetWindowPos( m_hWnd, HWND_NOTOPMOST,
				m_rcWindowBounds.left, m_rcWindowBounds.top,
				( m_rcWindowBounds.right - m_rcWindowBounds.left ),
				( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
				SWP_SHOWWINDOW );

			// Let the user know we are switching from HAL to the reference rasterizer
			//DisplayErrorMsg( hr, MSGWARN_SWITCHEDTOREF );

			hr = Initialize3DEnvironment();
		}
	}
	return hr;
}

void SPVideo::Cleanup3DEnvironment()
{
	if( m_pd3dDevice != NULL )
	{
		if( m_bDeviceObjectsRestored )
		{
			m_bDeviceObjectsRestored = false;
			InvalidateDeviceObjects();
		}
		if( m_bDeviceObjectsInited )
		{
			m_bDeviceObjectsInited = false;
			DeleteDeviceObjects();
		}

		m_pd3dDevice->Release();
		//if( m_pd3dDevice->Release() > 0 )
		//	DisplayErrorMsg( D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );
		m_pd3dDevice = NULL;
	}
}

HRESULT SPVideo::Reset3DEnvironment()
{
	HRESULT hr;

	// Release all vidmem objects
	if( m_bDeviceObjectsRestored )
	{
		m_bDeviceObjectsRestored = false;
		InvalidateDeviceObjects();
	}
	// Reset the device	
	SAFE_RELEASE(m_pBackBuffer);
	if( FAILED( hr = m_pd3dDevice->Reset( &m_d3dpp ) ) )
		return hr;
	

	// Store render target surface desc	
	m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer );
	m_pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
	
	
	// Set up the fullscreen cursor
	//if( m_bShowCursorWhenFullscreen && !m_bWindowed )
	if( m_bShowCursorWhenFullscreen && !m_d3dSettings.IsWindowed)
		
	{
		HCURSOR hCursor;
#ifdef _WIN64
		hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
		hCursor = (HCURSOR)ULongToHandle( GetClassLong( m_hWnd, GCL_HCURSOR ) );
#endif
		D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor, false );			
		m_pd3dDevice->ShowCursor( false );
	}

	// Confine cursor to fullscreen window
	if( m_bClipCursorWhenFullscreen )
	{
		if (!m_bWindowed )
		{
			RECT rcWindow;
			GetWindowRect( m_hWnd, &rcWindow );
			ClipCursor( &rcWindow );
		}
		else
		{
			ClipCursor( NULL );
		}
	}

	// Initialize the app's device-dependent objects
	hr = RestoreDeviceObjects();
	if( FAILED(hr) )
	{
		InvalidateDeviceObjects();
		return hr;
	}

	UpdateRamp();

	m_bDeviceObjectsRestored = true;
	return S_OK;
}

HRESULT SPVideo::InitDeviceObjects()
{
	// TODO: create device objects
	HRESULT hr;
	if(FAILED(hr = D3DXCreateSprite(m_pd3dDevice, &m_pd3dxSrite)))
		return hr;

	char szGlobalFont[128];


	HKL s_hklCurrent = GetKeyboardLayout( 0 );
	WORD PriLang = PRIMARYLANGID ( LOWORD( s_hklCurrent ));

	switch(PriLang) {
	case LANG_KOREAN:
		strcpy( szGlobalFont, GLOBAL_FONT_KOREAN );
		break;

	case LANG_JAPANESE:
		strcpy( szGlobalFont, GLOBAL_FONT_JAPANESE );
		break;
	
	case LANG_CHINESE:
	default:
		strcpy( szGlobalFont, GLOBAL_FONT_KOREAN );
		break;	
	}

	//test
	//strcpy( szGlobalFont, GLOBAL_FONT_JAPANESE );

	strcpy( m_strGlobalFontName , szGlobalFont);


	m_paFont[FONT_12_NORMAL] = new SPFont;
	if(m_paFont[FONT_12_NORMAL]) {		
		if(FAILED( (m_paFont[FONT_12_NORMAL])->Init(szGlobalFont, 12, FW_NORMAL ))) {
			SAFE_DELETE(m_paFont[FONT_12_NORMAL]);
		}		
	}

	m_paFont[FONT_12_BOLD] = new SPFont;
	if(m_paFont[FONT_12_BOLD]) {		
		if(FAILED( (m_paFont[FONT_12_BOLD])->Init(szGlobalFont, 12, FW_BOLD ))) {
			SAFE_DELETE(m_paFont[FONT_12_BOLD]);
		}		
	}

	m_paFont[FONT_14_NORMAL] = new SPFont;
	if(m_paFont[FONT_14_NORMAL]) {		
		if(FAILED( (m_paFont[FONT_14_NORMAL])->Init(szGlobalFont, 14, FW_NORMAL ))) {
			SAFE_DELETE(m_paFont[FONT_14_NORMAL]);
		}		
	}

	m_paFont[FONT_14_BOLD] = new SPFont;
	if(m_paFont[FONT_14_BOLD]) {		
		if(FAILED( (m_paFont[FONT_14_BOLD])->Init(szGlobalFont, 14, FW_BOLD ))) {
			SAFE_DELETE(m_paFont[FONT_14_BOLD]);
		}		
	}

	m_paFont[FONT_16_BOLD] = new SPFont;
	if(m_paFont[FONT_16_BOLD]) {		
		if(FAILED( (m_paFont[FONT_16_BOLD])->Init(szGlobalFont, 16, FW_BOLD ))) {
			SAFE_DELETE(m_paFont[FONT_16_BOLD]);
		}		
	}

	m_paFont[FONT_18_BOLD] = new SPFont;
	if(m_paFont[FONT_18_BOLD]) {		
		if(FAILED( (m_paFont[FONT_18_BOLD])->Init(szGlobalFont, 18, FW_BOLD ))) {
			SAFE_DELETE(m_paFont[FONT_18_BOLD]);
		}		
	}

	m_pSysLine = new SPLine;
	if(m_pSysLine) {
		if(FAILED(m_pSysLine->Init())) {
			SAFE_DELETE(m_pSysLine);
		}
	}
	//图型化IGW,一些初始信息
	if ( g_pCIGWLoaderDx9 )//gai
	{	
		if ( !g_pCIGWLoaderDx9->InitializeGraphic ( m_pd3dDevice, &m_d3dpp, false ) )
		{
			delete g_pCIGWLoaderDx9;
			g_pCIGWLoaderDx9 = NULL;
		}
	}	
	if ( g_pCIGWLoaderDx9 )
	{
		g_pCIGWLoaderDx9->SetSelfCursor( false );
		g_pCIGWLoaderDx9->SetScreenStatus( igwScreenMini );//初始状态为最小化
		g_pCIGWLoaderDx9->SetFocus( false );
		POINT IGWLocation;
		IGWLocation.x = 755;
		IGWLocation.y = 125;
		g_pCIGWLoaderDx9->SetTaskBarPosition( IGWLocation );//初始位置
	}

	return S_OK;
}

SPFont*	SPVideo::GetFont( FONT_ENUMERATE FontEnumerate)
{ 
	return m_paFont[FontEnumerate];		
}


HRESULT SPVideo::RestoreDeviceObjects()
{
	// TODO: setup render states

	//-----------------------
	// noeejang's state
	// ? ?诂 ???E???
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_CURRENT );
	//----------------------

	
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR  );
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR  );

	// Set miscellaneous render states
	SPSetRenderState( D3DRS_DITHERENABLE,   FALSE );
	SPSetRenderState( D3DRS_SPECULARENABLE, FALSE );
	SPSetRenderState( D3DRS_AMBIENT,        0x000F0F0F );

	// 2D?烷 ?醯	
	//SPSetRenderState( D3DRS_ZENABLE,        D3DZB_FALSE );
	//SPSetRenderState( D3DRS_ZENABLE,        D3DZB_TRUE );

	//----------------------
	// ? 郓?E???
	SPSetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	SPSetRenderState( D3DRS_LIGHTING, FALSE );	
	SPSetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	SPSetRenderState( D3DRS_ALPHATESTENABLE, FALSE );

	SPSetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	SPSetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	//---------------------------

	// ?E ?足? ??莜? 秀?
	D3DXMATRIX mat;
	D3DXMatrixIdentity( &mat );//定义一个单位矩阵
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &mat );
	m_pd3dDevice->SetTransform( D3DTS_VIEW,  &mat );

	D3DXMatrixOrthoOffCenterLH( &mat, 0.0f, (FLOAT)m_dwCreationWidth, (FLOAT)m_dwCreationHeight, 0.0f, 0.0f, 1.0f );//正交投影
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat );

	//TODO: RestoreDeviceObjects 	
	if(m_pd3dxSrite)	m_pd3dxSrite->OnResetDevice();	
	for (int ife = 0 ; ife < FONT_MAX ; ++ife ) {
		if(m_paFont[ife]) 
			(m_paFont[ife])->OnResetDevice();		
	}	
	if(m_pSysLine)		m_pSysLine->OnResetDevice();
	//ResetDevice IGW
	if( g_pCIGWLoaderDx9 )
		g_pCIGWLoaderDx9->OnDeviceReset ( &m_d3dpp ) ;
	return S_OK;
}

HRESULT SPVideo::InvalidateDeviceObjects()
{
	// TODO: Cleanup any objects created in RestoreDeviceObjects()	

	if(m_pd3dxSrite)	m_pd3dxSrite->OnLostDevice();	
	for (int ife = 0 ; ife < FONT_MAX ; ++ife ) {
		if(m_paFont[ife]) 
			(m_paFont[ife])->OnLostDevice();		
	}

	if(m_pSysLine)		m_pSysLine->OnLostDevice();
	
	//DeviceLost IGW
	if( g_pCIGWLoaderDx9 )
		g_pCIGWLoaderDx9->OnDeviceLost () ;
	return S_OK;
}

HRESULT SPVideo::DeleteDeviceObjects()
{
	// TODO: Cleanup any objects created in InitDeviceObjects()

	SAFE_RELEASE(m_pd3dxSrite);	
	SAFE_DELETE(m_pSysLine);
	//SAFE_DELETE_ARRAY(m_paFont);
	for (int ife = 0 ; ife < FONT_MAX ; ++ife ) {
		SAFE_DELETE(m_paFont[ife]);		
	}
//DeleteDevice IGW
	if( g_pCIGWLoaderDx9 )
		g_pCIGWLoaderDx9->FinalizeGraphic () ;
	return S_OK;
}

void SPVideo::BuildPresentParamsFromSettings()
{
	m_d3dpp.Windowed               = m_d3dSettings.IsWindowed;
	m_d3dpp.BackBufferCount        = 1;//1个后台缓存区
	m_d3dpp.MultiSampleType        = m_d3dSettings.MultisampleType();
	m_d3dpp.MultiSampleQuality     = m_d3dSettings.MultisampleQuality();
	m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;	//后台缓存复制到前台缓存后,清除后台缓存内容
	m_d3dpp.EnableAutoDepthStencil = m_d3dEnumeration.AppUsesDepthBuffer;
	m_d3dpp.hDeviceWindow          = m_hWnd;
	if( m_d3dEnumeration.AppUsesDepthBuffer )
	{
		//m_d3dpp.Flags              = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		//m_d3dpp.AutoDepthStencilFormat = m_d3dSettings.DepthStencilBufferFormat();

		m_d3dpp.Flags              = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;		

		//m_d3dpp.Flags              = 0;		
		
	}
	else
	{
		//m_d3dpp.Flags              = 0;
		m_d3dpp.Flags              = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;		

		//m_d3dpp.Flags              = 0;		
	}

	//if( m_bWindowed )
	if(m_d3dSettings.IsWindowed)
	{
		m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
		m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
		m_d3dpp.BackBufferFormat = m_d3dSettings.PDeviceCombo()->BackBufferFormat;
		m_d3dpp.FullScreen_RefreshRateInHz = 0;
		m_d3dpp.PresentationInterval = m_d3dSettings.PresentInterval();
	}
	else
	{
		m_d3dpp.BackBufferWidth  = m_d3dSettings.DisplayMode().Width;
		m_d3dpp.BackBufferHeight = m_d3dSettings.DisplayMode().Height;
		m_d3dpp.BackBufferFormat = m_d3dSettings.PDeviceCombo()->BackBufferFormat;
		m_d3dpp.FullScreen_RefreshRateInHz = m_d3dSettings.Fullscreen_DisplayMode.RefreshRate;
		m_d3dpp.PresentationInterval = m_d3dSettings.PresentInterval();
	}
}




