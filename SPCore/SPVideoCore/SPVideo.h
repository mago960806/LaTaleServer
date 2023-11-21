// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment : 
// Creation : metalgeni 2004-08-16 오후 4:48:46
//***************************************************************************

//////////////////////////////////////////
// Referenced from  DX9 Sample, Noeejang's CNVideo Sample
// thanx to Bill, Noeejang
//								by metalgeni.

#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

enum VIDEO_QUALITY {
	VIDEOQUAL_NULL,
	VIDEOQUAL_HIGH,
	VIDEOQUAL_LOW,
};

enum TEXTURE_EFFECT
{
	TEXTURE_EFFECT_NULL			=	0	,
	TEXTURE_EFFECT_NORMAL				,
	TEXTURE_EFFECT_MULTIPLY				,
	TEXTURE_EFFECT_ADDITIVE				,
	TEXTURE_EFFECT_SUBTRACTIVE			,
	TEXTURE_EFFECT_SCREEN				,
	TEXTURE_EFFECT_LIGHTEN				,
	TEXTURE_EFFECT_DARKEN				,
	TEXTURE_EFFECT_COUNT				,
};


HRESULT D3DUtil_SetDeviceCursor( LPDIRECT3DDEVICE9 pd3dDevice, HCURSOR hCursor,
								BOOL bAddWatermark );

class SPFont;
class SPLine;
class SPTextureMgr;

class SPVideo {
public:
	SPVideo();
	~SPVideo();

	HRESULT Init( HWND hWnd, DWORD uiWidth, DWORD uiHeight, bool bWindowed, VIDEO_QUALITY VideoQuality = VIDEOQUAL_HIGH, bool bClipChild = false );	

	bool	BeginRender();
	bool	EndRender();

	ID3DXSprite*		GetSprite()		{ return m_pd3dxSrite;	}
	LPDIRECT3DDEVICE9	GetDevice()		{ return m_pd3dDevice;	}	
	SPFont*				GetFont			( FONT_ENUMERATE FontEnumerate =  FONT_12_NORMAL );

	SPLine*				GetSysLine()	{ return m_pSysLine; 	}

	DWORD				GetScreenLenX() { return m_dwCreationWidth;}
	DWORD				GetScreenLenY() { return m_dwCreationHeight; }

	HRESULT				SPSetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
	void				SetAlphaTexture(bool bVal);
	void				SetTextureEffect	( TEXTURE_EFFECT eEffectType );		// AJJIYA [12/1/2005]

	TCHAR*				GetCompileType()	{ return m_strCompiled; 	}

	SPTextureMgr*		GetTextureMgr()		{ return m_pTextureMgr; 	}
	D3DFORMAT			GetTextureCreateFormat();

	void				SetContrast(int iContrast); // 0 ~ 255.
	bool				SetClipChildMode(bool bClipChild);

	bool				SurfaceCaptureFileName(const char* pszFileName);
	bool				SurfaceCaptureJPG(IN int* piCount = NULL);	// 스크린샷.
	bool				SurfaceCapturePNG(IN int* piCount = NULL);	// 스크린샷.
	bool				SurfaceCaptureBMP(IN int* piCount = NULL);	// 스크린샷.

	bool				Flush();
	const char*			GetGlobalFontName() { return  m_strGlobalFontName; }

	void				Active();
	void				Deactive();
	bool				IsActive();
	bool				CheckActiveReady();

	bool				TestToRestore();
	void				SetIngameDM		(DEVMODE* IN pDM);
	bool				GetIngameDM		(DEVMODE* OUT pDM);
	bool				IsWindowed();

	void				SetClearColor(D3DCOLOR d3dColorClear);
	D3DCOLOR			GetClearColor();


	HWND				GetMainHWND();



protected:

	static HRESULT	ConfirmDevice		(	D3DCAPS9* pCaps, DWORD dwBehavior,	D3DFORMAT Format );
	static bool		ConfirmDeviceHelper	(	D3DCAPS9* pCaps, 
									VertexProcessingType vertexProcessingType, 
									D3DFORMAT backBufferFormat );

	bool	FindBestWindowedMode( bool bRequireHAL, bool bRequireREF );
	bool	FindBestFullscreenMode( bool bRequireHAL, bool bRequireREF, int iScrWidth = 0, int iScrHeight = 0 );
	HRESULT ChooseInitialD3DSettings();
	HRESULT Initialize3DEnvironment();
	void	BuildPresentParamsFromSettings();

	void	Cleanup3DEnvironment();
	HRESULT Reset3DEnvironment();

	// Device Object 
	HRESULT RestoreDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT InitDeviceObjects();	

	void	UpdateRamp();
	

protected:
	
	HWND	m_hWnd;
	HWND	m_hWndFocus;
	bool	m_bWindowed;
	DWORD	m_dwWindowStyle;
	bool	m_bStartFullscreen;

	RECT	m_rcWindowBounds;
	RECT	m_rcWindowClient; 

	TCHAR	m_strDeviceStats[90];
	TCHAR	m_strFrameStats[90];
	TCHAR	m_strCompiled[90];
	DWORD	m_dwCreateFlags;

	bool	m_bDeviceObjectsInited;
	bool	m_bDeviceObjectsRestored;
	bool	m_bClipCursorWhenFullscreen;

	bool	m_bDeviceLost;       
	bool	m_bMinimized;        
	bool	m_bMaximized;       
	bool	m_bIgnoreSizeChange;

	bool	m_bIsRender;
	bool	m_bIsRenderSprite;
	
	TCHAR*  m_strWindowTitle;				
	DWORD   m_dwCreationWidth;				
	DWORD   m_dwCreationHeight;				
	bool    m_bShowCursorWhenFullscreen;	// Whether to show cursor when fullscreen
	bool	m_bExternalCursor;
	
	LPDIRECT3D9				m_pD3D;
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	D3DPRESENT_PARAMETERS	m_d3dpp;
	D3DCAPS9				m_d3dCaps;

	LPDIRECT3DSURFACE9		m_pBackBuffer;
	D3DSURFACE_DESC			m_d3dsdBackBuffer;   // Surface desc of the backbuffer

	ID3DXSprite*			m_pd3dxSrite;

	// 기본 폰트		
	SPFont*					m_paFont[FONT_MAX];

	SPLine*					m_pSysLine;

	CD3DEnumeration			m_d3dEnumeration;
	CD3DSettings			m_d3dSettings;

	SPTextureMgr*			m_pTextureMgr;

	D3DGAMMARAMP			m_GammaRamp;
	bool					m_bCanGamma;
	int						m_iGammaGray;
	D3DXCOLOR				m_ColorRampMin;
	D3DXCOLOR				m_ColorRampMax;
	D3DCOLOR				m_WindowRamp; 

	// Option list
	D3DFORMAT				m_TextureCreateFormat;

	TCHAR					m_strGlobalFontName[128];

	bool					m_bActive;
	bool					m_bActiveReady;
	bool					m_bFirstFrame;
	bool					m_bClipChild;
	bool					m_bDialogBoxMode;

	DEVMODE					m_dmIngame;
	D3DCOLOR				m_d3dColorClear;


};


extern SPVideo* g_pVideo;





