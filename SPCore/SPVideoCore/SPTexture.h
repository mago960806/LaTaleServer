
// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPTexture
// Comment : 
// Creation : metalgeni 2004-08-23 坷饶 4:48:46
//***************************************************************************
#pragma once

#include <string>

#ifndef INITCOLOR
#define INITCOLOR 0xffffffff
#endif

extern CRITICAL_SECTION g_TextureLoadSection;
class SPTextureMgr;	
enum TEX_TYPE;

class SPTexture
{
public:	
	~SPTexture();

	//sprite
	HRESULT	RenderTexture(int ix, int iy, float fRotation = 0.f);
	HRESULT	RenderTexture(RECT* pTarRect, RECT* pSrcRect, float fRotation = 0.f);	
	HRESULT	RenderTexture(int iX, int iY, RECT* pSrcRect, bool bLRSwap = false, bool bTBSwap= false, float fRotation = 0.0f);
	HRESULT	RenderTextureImmediate(int iX, int iY);	
	HRESULT	RenderTextureImmediateCenterBase(int iX, int iY);
	HRESULT RenderTexture(RECT* pTarRect, RECT* pSrcRect,
	float fOffsetX, float fOffsetY, float fCenterOffsetX, float fCenterOffsetY, float fRotation = 0.f);	

	UINT	GetLenX() { return m_uiWidth;	}
	UINT	GetLenY() { return m_uiHeight;	}

	void	SetColor(const D3DXCOLOR Color = INITCOLOR);	
	void	SetScale(float fScaleX = 1.0f, float fScaleY= 1.0f) { m_fScaleX = fScaleX; m_fScaleY = fScaleY; }

	void	SetRotation (float	fRotation)	{ m_fRotation = fRotation; } 
	void    SetSrcRect	(RECT	RectSrc)	{ CopyMemory( &m_RectSrc, &RectSrc, sizeof(RECT)); }

	bool   CreateFontTexture(int w, int h, const char * buf);

	void	SetLRSwap(bool bLRSwap) { m_bLRSwap = bLRSwap; }
	void	SetTBSwap(bool bTBSwap) { m_bTBSwap = bTBSwap; }
	void	ToggleLRSwap();
	
	void	Release();
	void	IncreaseRef();
	
	const char* GetResName();
	LPDIRECT3DTEXTURE9 GetD3DTexture()		{	return m_pTexture ;		}

	//bool  SPTexture::CreateFontTexture(int w, int h, const char * buf)
	//--------------------------------------------------
	// added by jmulro, 2008-06-18
	void ReleaseTexture( void );
	void setTexType( TEX_TYPE type )
	{
		m_texType = type;
	}
	void SetTexName(const char *pName){m_strFileName = pName;};
	SPTexture( LPCSTR szFileName = NULL);

	DWORD GetCreateTime(){return m_time;};
	DWORD GetRenderTimes(){return m_allrenderTimes ++;};

	TEX_TYPE getTexType( void ) const
	{
		return m_texType;
	}
	//--------------------------------------------------

	//--------------------------------------------------
	bool getTextureReleasedStatus( void ) const
	{
		return m_bTextureReleased;
	}
	void setTextureReleasedStatus( bool status )
	{
		m_bTextureReleased = status;
	}
	//--------------------------------------------------

	//--------------------------------------------------
	// added by jmulro, 2008-06-18
	void _autoReloadTexture( void );
	//--------------------------------------------------

	void SetD3DFormat(D3DFORMAT d3df){m_D3DFormat = d3df;};
	DWORD   GetRenderFrame(){return m_renderFrame;};

	int		GetRefCount();
	
	HRESULT LoadTexture( LPCSTR szFileName, D3DFORMAT D3DFormat , D3DCOLOR dcColorKey = 0U ); //Texture Manager父 龋免秦具窃.
	HRESULT LoadTexturePack( LPCSTR szFileName, D3DCOLOR dcColorKey = 0U );
	HRESULT LoadTexturePack( int iResourceInstance, D3DCOLOR dcColorKey = 0U );

	std::string				m_strFileName;

protected:
	HRESULT Init( LPCSTR szFileName = NULL);
	void	SetInstanceID(int iInstanceID);

protected:

	bool					m_bTextureReleased;		//!< true if LPDIRECT3DTEXTURE9 has been released. else false.					
	TEX_TYPE				m_texType;		//!< added by jmulro, 2008-06-18, type of texture, @see SPTextureMgr.

	//int					m_iInstanceID;
	int						m_iRefCount;

	LPDIRECT3DTEXTURE9		m_pTexture;

	D3DFORMAT				m_D3DFormat;
	D3DXIMAGE_FILEFORMAT	m_ImageFileFormat;
	UINT                    m_uiWidth;
	UINT                    m_uiHeight;
	D3DXCOLOR				m_D3DColor;
	
	float					m_fScaleX;
	float					m_fScaleY;
	bool					m_bLRSwap;
	bool					m_bTBSwap;

	// Only Immediate
	float					m_fRotation; 
	RECT					m_RectSrc;

	DWORD                   m_renderFrame;
	DWORD                   m_time; //纹理创建时间
	DWORD                   m_allrenderTimes; //绘制次数

};






