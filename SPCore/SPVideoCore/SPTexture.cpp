
#include <assert.h>
#include <D3D9.h>
#include <d3dx9.h>

#include "SPStream.h"
#include "SPResourceBaseDEF.h"
#include "SPResourceBase.h"

#include "DXUtil.h"
#include "CD3DEnumeration.h"
#include "D3DSettings.h"

#include "SPVideoCommon.h"
#include "SPTextureMgr.h"
#include "SPLine.h"
#include "SPFont.h"
#include "SPVideo.h"
#include "SPTexture.h"
#include "SPUtil.h"
#include "../LataleClient/SPUtil/SPXTrace.h"

extern CRITICAL_SECTION g_TextureLoadSection;

extern CRITICAL_SECTION g_TextureLoadSection;
extern CRITICAL_SECTION g_TexLoadSection;
extern DWORD            g_RenderFrame;
extern float            g_TexSize[TYPE_NUM];

inline int RectWidth( RECT &rc )	{ return ((rc).right - (rc).left ); }
inline int RectHeight( RECT &rc )	{ return ((rc).bottom - (rc).top ); }

SPTexture::SPTexture(LPCSTR szFileName)
: m_pTexture	(NULL)
, m_D3DColor	(INITCOLOR)
, m_fScaleX		(1.0f)
, m_fScaleY		(1.0f)
, m_D3DFormat	(D3DFMT_UNKNOWN)
//, m_iInstanceID	(0)
, m_iRefCount	(0)
, m_fRotation	(0.0f)
, m_bLRSwap		(false)
, m_bTBSwap		(false)
,m_uiWidth		(0)
,m_uiHeight		(0)
,m_bTextureReleased(true)
,m_texType (TYPE_NUM)
{
	ZeroMemory(&m_RectSrc, sizeof(RECT)); 
	Init(szFileName);

	m_allrenderTimes = 0;
	m_renderFrame = 0;
	m_time = 0;
	m_strFileName.clear();
}

SPTexture::~SPTexture()
{
	try
	{
		SAFE_RELEASE(m_pTexture);
	}
	catch(...)
	{
		return;
	}
}

HRESULT SPTexture::Init(LPCSTR szFileName)
{		
	if(szFileName) {
		return LoadTexture(szFileName,g_pVideo->GetTextureCreateFormat());
	}
	return S_OK;
}


void SPTexture::SetColor(const D3DXCOLOR Color)
{ 
	m_D3DColor = Color; 
}

const char* SPTexture::GetResName()
{
	return m_strFileName.c_str();
}

HRESULT SPTexture::LoadTexture( LPCSTR szFileName, D3DFORMAT D3DFormat , D3DCOLOR dcColorKey /* = 0U  */)
{
	LPDIRECT3DTEXTURE9 pd3dTexture = NULL;
	D3DXIMAGE_INFO SrcInfo;		

	//////////////////////////////////////////////////////////////////////////
	// Start CriticalSection for LogoThread
	EnterCriticalSection(&g_TextureLoadSection) ;
	HRESULT hr = D3DXCreateTextureFromFileEx( 
		g_pVideo->GetDevice(), 
		szFileName,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		1, 
		0,
		D3DFormat,
		D3DPOOL_MANAGED, 
		D3DX_DEFAULT, 
		D3DX_DEFAULT,	
		dcColorKey, 
		&SrcInfo, 
		NULL, 
		&pd3dTexture );
	LeaveCriticalSection(&g_TextureLoadSection) ;
	// End CriticalSection
	//////////////////////////////////////////////////////////////////////////
	
	if( FAILED(hr) )
	{
		SAFE_RELEASE(pd3dTexture);
		return hr;
	}

	m_D3DFormat = SrcInfo.Format;
	m_ImageFileFormat = SrcInfo.ImageFileFormat;
	m_uiWidth	= SrcInfo.Width;
	m_uiHeight	= SrcInfo.Height;			
	m_pTexture	= pd3dTexture;	
	m_bTextureReleased = false;

	m_time = timeGetTime();

	// SPTextureMgr 에서 해준다 !!
	//m_strFileName = szFileName;

	return hr;
}


HRESULT SPTexture::LoadTexturePack( LPCSTR szFileName, D3DCOLOR dcColorKey )
{
	int iResourceInstance;	
	iResourceInstance = SPResourceBase::GetInstance()->FilenameToInstance( szFileName);
	return LoadTexturePack( iResourceInstance, dcColorKey );
}

HRESULT SPTexture::LoadTexturePack( int iResourceInstance, D3DCOLOR dcColorKey )
{
	SPStream* pStream;

	bool bRet = SPResourceBase::GetInstance()->GetStreamData( iResourceInstance, &pStream);	
	if(bRet == false)
		return D3DXERR_INVALIDDATA;
	
	LPDIRECT3DTEXTURE9 pd3dTexture = NULL;
	D3DXIMAGE_INFO SrcInfo;
	
	//////////////////////////////////////////////////////////////////////////
	// Start CriticalSection for LogoThread
	EnterCriticalSection(&g_TextureLoadSection) ;
	HRESULT hr = D3DXCreateTextureFromFileInMemoryEx( 
		g_pVideo->GetDevice(), 

		((SPManagedStream*)pStream)->Buf(),				// buffer
		pStream->Length(),			// size

		D3DX_DEFAULT, 
		D3DX_DEFAULT, 
		1, 
		0,
		g_pVideo->GetTextureCreateFormat(), 		
		D3DPOOL_MANAGED, 
		D3DX_DEFAULT, 
		D3DX_DEFAULT,	
		dcColorKey, 
		&SrcInfo, 
		NULL, 
		&pd3dTexture );

	pStream->Release();
	LeaveCriticalSection(&g_TextureLoadSection);
	// End CriticalSection
	//////////////////////////////////////////////////////////////////////////

	if( FAILED(hr) )
	{
		//pStream->Release();
		SAFE_RELEASE(pd3dTexture);

		//if( hr == E_OUTOFMEMORY )
		//{
		//	unsigned int uiMem = g_pVideo->GetDevice()->GetAvailableTextureMem();
		//}

		return hr;
	}

	m_D3DFormat = SrcInfo.Format;
	m_ImageFileFormat = SrcInfo.ImageFileFormat;
	m_uiWidth	= SrcInfo.Width;
	m_uiHeight	= SrcInfo.Height;			
	m_pTexture	= pd3dTexture;	
	m_bTextureReleased = false;

	m_time = timeGetTime();

	return hr;
}

bool  SPTexture::CreateFontTexture(int w, int h, const char * buf)
{
	LPDIRECT3DTEXTURE9 pd3dTexture = NULL;
	D3DXIMAGE_INFO SrcInfo;		

	HRESULT hr = D3DXCreateTexture(g_pVideo->GetDevice(),w,h,D3DX_DEFAULT,D3DX_DEFAULT,D3DFMT_A4R4G4B4,D3DPOOL_MANAGED,&pd3dTexture);
	if(FAILED(hr))
	{
		SAFE_RELEASE(pd3dTexture);
		return false;
	}

	D3DLOCKED_RECT cl;
	hr = pd3dTexture->LockRect(0, &cl,NULL,0);
	if(FAILED(hr))
	{
		return false;
	}
	
	WORD *pSrc = (WORD*)cl.pBits;
	WORD *pBuf = (WORD*)buf;

	for(int i=0;i<h;++i)
	{
		for(int j=0;j<w;++j)
		{
			*pSrc = *pBuf;

			pBuf ++;
			pSrc ++;
		}

		pSrc += (cl.Pitch >> 1) - w;
	}

	pd3dTexture->UnlockRect(0);

	//m_D3DFormat = SrcInfo.Format;
	//m_ImageFileFormat = SrcInfo.ImageFileFormat;
	m_uiWidth	= w;
	m_uiHeight	= h;			
	m_pTexture	= pd3dTexture;	

//	m_time = timeGetTime();
//	m_allrenderTimes = 0; //삥齡늴鑒


	return true;
}

int	SPTexture::GetRefCount()
{
	return m_iRefCount;
}

void SPTexture::Release()
{
	try
	{
	assert(m_iRefCount > 0);

	if(--m_iRefCount <= 0) {
		if(SPTextureMgr::GetInstance()) {
			//char szBuf[512] = {0};
			//_snprintf( szBuf, 511, "-----Released Texture %s.\n", m_strFileName.c_str() );
			//xTrace( szBuf );
			SPTextureMgr::GetInstance()->UnloadTexture( m_strFileName.c_str(),m_texType)	;
		} else {
			assert(0 && "Texture Manager Error");
		}
	}
	}
	catch(...)
	{
		return;
	}
}

void SPTexture::IncreaseRef()
{
	++m_iRefCount;
}


void SPTexture::ToggleLRSwap() 
{
	m_bLRSwap = !m_bLRSwap;
}


// 이미지 전체를 렌더하며 회전값을 줄수 있다.
HRESULT	SPTexture::RenderTexture(int iX, int iY, float fRotation)
{
	if(m_bTextureReleased)
	{
		_autoReloadTexture();
	}

	if(!m_pTexture) 
		return S_FALSE;

	m_allrenderTimes ++;
	m_renderFrame = g_RenderFrame;

	D3DXMATRIX		matTransform;
	LPD3DXSPRITE	pSprite = g_pVideo->GetSprite();
	RECT			SrcRect;

	if( pSprite == NULL )
		return S_FALSE;

	SetRect(&SrcRect, 0, 0, m_uiWidth, m_uiHeight);	
	
	D3DXMatrixIdentity( &matTransform);

	D3DXVECTOR2 Center	= D3DXVECTOR2(
		abs((float)(RectWidth(SrcRect)/2)*m_fScaleX), 
		abs((float)(RectHeight(SrcRect)/2)*m_fScaleY));

	D3DXVECTOR2 Trans = D3DXVECTOR2(
		(float)iX, 
		(float)iY);
	
	D3DXMatrixAffineTransformation2D(&matTransform, 1.0f, &Center, fRotation, &Trans);		

	D3DXMATRIX matTransformRotaion;		
	D3DXMatrixIdentity(&matTransformRotaion);

	D3DXMatrixScaling(	&matTransformRotaion, m_fScaleX, m_fScaleY, 1.0f);		
	D3DXMatrixMultiply(	&matTransform, &matTransformRotaion, &matTransform);

	pSprite->SetTransform(&matTransform);
	
	return pSprite->Draw( m_pTexture, &SrcRect, NULL, NULL, m_D3DColor);;
}


// 부분렌더와 전체렌더가능, 소스와 타겟영역이 NULL일경우 왼쪽상단부터 전체를 지정한다.
HRESULT	SPTexture::RenderTexture(RECT* pTarRect, RECT* pSrcRect, float fRotation)
{
	if(m_bTextureReleased)
	{
		_autoReloadTexture();
	}

	if(!m_pTexture) 
		return S_FALSE;

	HRESULT hr;
	RECT TarRect;
	RECT SrcRect;

	float fScaleX, fScaleY;
	
	m_allrenderTimes ++;
	m_renderFrame = g_RenderFrame;

	if(pTarRect)  (TarRect = *pTarRect); else (SetRect(&TarRect, 0, 0, m_uiWidth, m_uiHeight));
	if(pSrcRect)  (SrcRect = *pSrcRect); else (SetRect(&SrcRect, 0, 0, m_uiWidth, m_uiHeight));	

	LPD3DXSPRITE	pSprite = g_pVideo->GetSprite();
	if( pSprite == NULL )
		return S_FALSE;

	D3DXMATRIX matResult, matRotation, matScale, matTemp;
	D3DXMatrixIdentity(&matResult);
	
	// Src
	D3DXVECTOR2 vSrcCenter, vRotOffset;
	vSrcCenter = D3DXVECTOR2(
		(RectWidth(SrcRect) >> 1) * m_fScaleX, (RectHeight(SrcRect) >> 1) * m_fScaleY);
	vRotOffset = vSrcCenter;

	fScaleX = fScaleY = 1.0f;
	if( vRotOffset.x < 0.0f )
	{
		vRotOffset.x = -vRotOffset.x;
		fScaleX = -1.0f;
	}
	if( vRotOffset.y < 0.0f )
	{
		vRotOffset.y = -vRotOffset.y;
		fScaleY = -1.0f;
	}

	if( RectWidth(TarRect) < 0 )
		fRotation = -fRotation;

	D3DXMatrixScaling(&matScale, m_fScaleX * fScaleX, m_fScaleY * fScaleY, 1.0f);
	D3DXMatrixRotationZ(&matRotation, fRotation);
	D3DXMatrixTranslation(&matTemp, -vRotOffset.x, -vRotOffset.y, 0.0f);
	
	D3DXMatrixMultiply(&matResult, &matScale, &matTemp);
	D3DXMatrixMultiply(&matResult, &matResult, &matRotation);

	D3DXMatrixTranslation(&matTemp, vRotOffset.x, vRotOffset.y, 0.0f);
	D3DXMatrixMultiply(&matResult, &matResult, &matTemp);
	
	fScaleX = (float) RectWidth( TarRect ) / (float)(abs((float)RectWidth(SrcRect)));
	fScaleY = (float) RectHeight( TarRect ) / (float)(abs((float)RectHeight(SrcRect)));
	
	/*
	// dst
	fScaleX = fScaleY = 1.0f;
	if( RectWidth(TarRect) < 0 )
		fScaleX = -1.0f;

	if( RectHeight(TarRect) < 0 )
		fScaleY = -1.0f;
	*/

	D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
	D3DXMatrixTranslation(&matTemp, (float)TarRect.left, (float)TarRect.top, 0.0f);
	
	D3DXMatrixMultiply(&matResult, &matResult, &matScale);
	D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

	pSprite->SetTransform(&matResult);	
	hr = pSprite->Draw( m_pTexture, &SrcRect, NULL, NULL, m_D3DColor);

	/*
	if(!m_pTexture) 
		return S_FALSE;

	HRESULT hr;
	RECT TarRect;
	RECT SrcRect;

	float	fScaleX = m_fScaleX;
	float	fScaleY = m_fScaleY;
	int		OffSetX = 0;
	int		OffSetY = 0;
	
	if(pTarRect)  (TarRect = *pTarRect); else (SetRect(&TarRect, 0, 0, m_uiWidth, m_uiHeight));
	if(pSrcRect)  (SrcRect = *pSrcRect); else (SetRect(&SrcRect, 0, 0, m_uiWidth, m_uiHeight));	

	LPD3DXSPRITE	pSprite = g_pVideo->GetSprite();
	
	fScaleX = (float) RectWidth( TarRect ) / RectWidth( SrcRect );
	fScaleY = (float) RectHeight( TarRect ) / RectHeight( SrcRect );

	if(fScaleX < 0) {		
		OffSetX = RectWidth(SrcRect)*m_fScaleX;
	}

	if(fScaleY < 0) {		
		OffSetY = RectHeight(SrcRect)*m_fScaleY;		
	}

	D3DXMATRIX matTransform;
	D3DXMatrixIdentity( &matTransform );

	D3DXVECTOR2 v2Center	= D3DXVECTOR2(
		abs((float)(RectWidth(TarRect))/2	*m_fScaleX)	-OffSetX, 
		abs((float)(RectHeight(TarRect))/2	*m_fScaleY)	-OffSetY
		);

	D3DXVECTOR2 v2Trans = D3DXVECTOR2(
		(float)TarRect.left, 
		(float)TarRect.top   );	

	D3DXMATRIX matTransformRotaion;		
	D3DXMatrixIdentity(&matTransformRotaion);

	D3DXMatrixAffineTransformation2D(&matTransform, 1.0f, &v2Center, fRotation, &v2Trans);		

	D3DXMatrixScaling(	&matTransformRotaion, m_fScaleX*fScaleX, m_fScaleY*fScaleY, 1.0f);		
	//D3DXMatrixScaling(	&matTransformRotaion, fScaleX, fScaleY, 1.0f);		
	D3DXMatrixMultiply(	&matTransform, &matTransformRotaion, &matTransform);

	pSprite->SetTransform(&matTransform);	
	hr = pSprite->Draw( m_pTexture, &SrcRect, NULL, NULL, m_D3DColor);
	*/
	
	
	return hr;
}

HRESULT SPTexture::RenderTexture(RECT* pTarRect, RECT* pSrcRect,
	float fOffsetX, float fOffsetY, float fCenterOffsetX, float fCenterOffsetY, float fRotation)
{
	if(m_bTextureReleased)
	{
		_autoReloadTexture();
	}

	if(!m_pTexture) 
		return S_FALSE;

	HRESULT hr;
	RECT TarRect;
	RECT SrcRect;

	float fScaleX, fScaleY;

	m_allrenderTimes ++;
	m_renderFrame = g_RenderFrame;

	if(pTarRect)  (TarRect = *pTarRect); else (SetRect(&TarRect, 0, 0, m_uiWidth, m_uiHeight));
	if(pSrcRect)  (SrcRect = *pSrcRect); else (SetRect(&SrcRect, 0, 0, m_uiWidth, m_uiHeight));	

	LPD3DXSPRITE	pSprite = g_pVideo->GetSprite();
	if( pSprite == NULL )
		return S_FALSE;

	D3DXMATRIX matResult, matScale, matRotation, matTemp;
	D3DXMatrixIdentity(&matResult);

	// Src
	//D3DXVECTOR2 vRotOffset = D3DXVECTOR2(fCenterOffsetX, fCenterOffsetY);
	D3DXVECTOR2 vRotOffset = D3DXVECTOR2(fCenterOffsetX * m_fScaleX, fCenterOffsetY * m_fScaleY);
	if( RectWidth(TarRect) < 0 )
		fRotation = -fRotation;

	fScaleX = fScaleY = 1.0f;
	if( RectWidth(SrcRect) < 0 )
	{
		fScaleX = -1.0f;
		vRotOffset.x = -((float)abs(RectWidth(SrcRect)) + vRotOffset.x);
	}
	if( RectHeight(SrcRect) < 0 )
	{
		fScaleY = -1.0f;
		vRotOffset.y = -((float)abs(RectWidth(SrcRect)) + vRotOffset.y);
	}
	
	D3DXMatrixScaling(&matScale, m_fScaleX * fScaleX, m_fScaleY * fScaleY, 1.0f);
	D3DXMatrixRotationZ(&matRotation, fRotation);

	if( m_fScaleX != 1.0f || m_fScaleY != 1.0f )
	{
		D3DXMatrixTranslation(&matTemp, fOffsetX, fOffsetY, 0.0f);
		D3DXMatrixMultiply(&matResult, &matTemp, &matScale);

		D3DXMatrixTranslation(&matTemp, -fOffsetX, -fOffsetY, 0.0f);
		D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

		D3DXMatrixTranslation(&matTemp, vRotOffset.x, vRotOffset.y, 0.0f);
		D3DXMatrixMultiply(&matResult, &matResult, &matTemp);
	}
	else
	{
		D3DXMatrixTranslation(&matTemp, vRotOffset.x, vRotOffset.y, 0.0f);
		D3DXMatrixMultiply(&matResult, &matScale, &matTemp);
	}
	
	D3DXMatrixMultiply(&matResult, &matResult, &matRotation); 

	D3DXMatrixTranslation(&matTemp, -vRotOffset.x, -vRotOffset.y, 0.0f);
	D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

	fScaleX = (float) RectWidth( TarRect ) / (float)(abs((float)RectWidth(SrcRect)));
	fScaleY = (float) RectHeight( TarRect ) / (float)(abs((float)RectHeight(SrcRect)));

	D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
	D3DXMatrixTranslation(&matTemp, (float)TarRect.left, (float)TarRect.top, 0.0f);

	D3DXMatrixMultiply(&matResult, &matResult, &matScale);
	D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

	pSprite->SetTransform(&matResult);	
	hr = pSprite->Draw( m_pTexture, &SrcRect, NULL, NULL, m_D3DColor);

	return hr;
}


// 부분렌더와 전체렌더가능, 소스영역이 NULL일경우 왼쪽상단부터 전체를 지정한다.
HRESULT	SPTexture::RenderTexture(int iX, int iY, RECT* pSrcRect, bool bLRSwap, bool bTBSwap, float fRotation )
{
	if(m_bTextureReleased)
	{
		_autoReloadTexture();
	}

	if(!m_pTexture) 
		return S_FALSE;

	HRESULT hr;
	RECT	TarRect;
	RECT	SrcRect;
	float	fScaleX = m_fScaleX;
	float	fScaleY = m_fScaleY;
	int		OffSetX = 0;
	int		OffSetY = 0;
	
	m_allrenderTimes ++;
	m_renderFrame = g_RenderFrame;

	if(pSrcRect)  
		SrcRect = *pSrcRect; 
	else 
		SetRect(&SrcRect, 0, 0, m_uiWidth, m_uiHeight);

	if(bLRSwap) {
		fScaleX = -fScaleX;
		OffSetX = (int)(RectWidth(SrcRect)*m_fScaleX);
	}

	if(bTBSwap) {
		fScaleY = -fScaleY;
		OffSetY = (int)(RectHeight(SrcRect)*m_fScaleY);
	}
	
	SetRect(&TarRect, iX, iY, iX + (int)(RectWidth(SrcRect)*m_fScaleX), iY + (int)(RectHeight(SrcRect)*m_fScaleY));	

	LPD3DXSPRITE	pSprite = g_pVideo->GetSprite();
	if( pSprite == NULL )
		return S_FALSE;

	D3DXMATRIX matTransform;
	D3DXMatrixIdentity( &matTransform);	
	
	D3DXVECTOR2 v2Center	= D3DXVECTOR2(
		abs((float)(RectWidth(TarRect))/2)	-OffSetX	, 
		abs((float)(RectHeight(TarRect))/2)	-OffSetY	);	

	D3DXVECTOR2 v2Trans = D3DXVECTOR2(
		(float)TarRect.left + OffSetX, 
		(float)TarRect.top  + OffSetY );	

	D3DXMatrixAffineTransformation2D(&matTransform, 1.0f, &v2Center, fRotation, &v2Trans);	

	D3DXMATRIX matTransformRotaion;		
	D3DXMatrixIdentity(&matTransformRotaion);

	D3DXMatrixScaling(	&matTransformRotaion, fScaleX, fScaleY, 1.0f);		
	D3DXMatrixMultiply(	&matTransform, &matTransformRotaion, &matTransform);	

	pSprite->SetTransform(&matTransform);	
	hr = pSprite->Draw( m_pTexture, &SrcRect, NULL, NULL, m_D3DColor);
	
	return hr;
}



HRESULT	SPTexture::RenderTextureImmediate(int iX, int iY)
{
	if(m_bTextureReleased)
	{
		_autoReloadTexture();
	}

	if(!m_pTexture) 
		return S_FALSE;
	
	LPD3DXSPRITE	pSprite = g_pVideo->GetSprite();
	if( pSprite == NULL )
		return S_FALSE;

	float	fScaleX = m_fScaleX;
	float	fScaleY = m_fScaleY;
	int		OffSetX = 0;
	int		OffSetY = 0;
	RECT	TarRect;

	m_allrenderTimes ++;
	m_renderFrame = g_RenderFrame;

	if(m_bLRSwap) {
		fScaleX = -fScaleX;
		OffSetX = (int)(RectWidth(m_RectSrc)*m_fScaleX);
	}

	if(m_bTBSwap) {
		fScaleY = -fScaleY;
		OffSetY = (int)(RectHeight(m_RectSrc)*m_fScaleY);
	}

	SetRect(&TarRect, iX, iY, iX + (int)(RectWidth(m_RectSrc)*m_fScaleX), (int)(iY + RectHeight(m_RectSrc)*m_fScaleY));	

	D3DXMATRIX matTransform;
	D3DXMatrixIdentity( &matTransform);	

	D3DXVECTOR2 v2Center	= D3DXVECTOR2(
		abs((float)(RectWidth(TarRect))/2)	-OffSetX	, 
		abs((float)(RectHeight(TarRect))/2)	-OffSetY	);	

	D3DXVECTOR2 v2Trans = D3DXVECTOR2(
		(float)TarRect.left + OffSetX, 
		(float)TarRect.top  + OffSetY );	

	D3DXMatrixAffineTransformation2D(&matTransform, 1.0f, &v2Center, m_fRotation, &v2Trans);	

	D3DXMATRIX matTransformRotaion;		
	D3DXMatrixIdentity(&matTransformRotaion);

	D3DXMatrixScaling(	&matTransformRotaion, fScaleX, fScaleY, 1.0f);		
	D3DXMatrixMultiply(	&matTransform, &matTransformRotaion, &matTransform);	

	pSprite->SetTransform(&matTransform);	
	return pSprite->Draw( m_pTexture, &m_RectSrc, NULL, NULL, m_D3DColor);
	
}

HRESULT	SPTexture::RenderTextureImmediateCenterBase(int iX, int iY)
{
	if(m_bTextureReleased)
	{
		_autoReloadTexture();
	}

	if(!m_pTexture) 
		return S_FALSE;

	D3DXMATRIX		matTransform;
	LPD3DXSPRITE	pSprite = g_pVideo->GetSprite();
	if( pSprite == NULL )
		return S_FALSE;

	m_allrenderTimes ++;
	m_renderFrame = g_RenderFrame;

	float fScaleX;
	float fScaleY;
	float fOffsetX = 0.0f;
	float fOffsetY = 0.0f;

	if(m_bLRSwap) {
		fScaleX = -m_fScaleX;
		fOffsetX = (float)RectWidth(m_RectSrc)*m_fScaleX;
	} else {
		fScaleX = m_fScaleX;		
	}

	if(m_bTBSwap) {
		fScaleY = -m_fScaleY;
		fOffsetY = (float)RectHeight(m_RectSrc)*m_fScaleY;
	} else {
		fScaleY = m_fScaleY;
	}


	D3DXVECTOR2		v2Trans = D3DXVECTOR2(	(float)iX - ((float)RectWidth(m_RectSrc) * m_fScaleX )/2  + fOffsetX, 
											(float)iY - ((float)RectHeight(m_RectSrc) * m_fScaleY )/2  + fOffsetY);

	D3DXVECTOR2		v2Center = D3DXVECTOR2( abs((float)RectWidth(m_RectSrc) * m_fScaleX / 2 ) - fOffsetX, 
											abs((float)RectHeight(m_RectSrc) * m_fScaleY / 2 )  - fOffsetY);
	
	D3DXMatrixAffineTransformation2D(&matTransform, 1.0f, &v2Center, m_fRotation, &v2Trans);
	D3DXMATRIX matTransformRotaion;			
	
	D3DXMatrixScaling(	&matTransformRotaion, fScaleX, fScaleY, 1.0f);		
	D3DXMatrixMultiply(	&matTransform, &matTransformRotaion, &matTransform);

	pSprite->SetTransform(&matTransform);

	return pSprite->Draw( m_pTexture, &m_RectSrc, NULL, NULL, m_D3DColor);
}

//--------------------------------------------------
void SPTexture::_autoReloadTexture( void )
{
	string strFileName;

	if (NULL == m_pTexture && !m_strFileName.empty())
	{
		HRESULT hr = NULL;
		if(SPResourceBase::GetInstance()->IsManaged() == false)
		{
			string strKey;
			strFileName = SPResourceBase::GetInstance()->GetWorkPath();
			GetUpperString3(strKey, m_strFileName.c_str());
			strFileName += strKey;
			hr = LoadTexture(strFileName.c_str(), m_D3DFormat);
		}
		else
		{
			GetUpperString3(strFileName, m_strFileName.c_str());
			hr = LoadTexturePack(strFileName.c_str(),m_D3DFormat);
		}

		if (FAILED( hr ))
		{
			//ReleaseTexture();
		}
		else
		{
			m_bTextureReleased = false;
			g_TexSize[m_texType] += m_uiWidth * m_uiHeight / 262144.0f;
#ifdef _DEBUG
			static int count = 0;
			++count;
			char buf[512] = {0};
			_snprintf( buf, 511, "[++%d++] Texture Auto Reloaded: %s.\n", count, m_strFileName.c_str() );
			OutputDebugString( buf );
#endif
		}
	}
}

void SPTexture::ReleaseTexture( void )
{
	try
	{
	SAFE_RELEASE( m_pTexture );
	}
	catch(...)
	{
		return;
	}
	m_uiWidth = 0;
	m_uiHeight = 0;
	m_bTextureReleased = true;

	m_allrenderTimes = 0;
	m_renderFrame = 0;
	m_time = 0;

#ifdef _DEBUG
	static int count = 0;
	++count;
	char buf[512] = {0};
	_snprintf( buf, 511, "[--%d--] Texture Released: %s.\n", count, m_strFileName.c_str() );
	OutputDebugString( buf );
#endif
}
//--------------------------------------------------




