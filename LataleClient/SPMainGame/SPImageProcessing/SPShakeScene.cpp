#include "SPCommon.h"
#include "SPImageProcessor.h"
#include "SPShakeScene.h"

//------------------------------------------------------------------------------------
SPShakeScene::SPShakeScene()
: m_pOffscreenSurface(NULL)
, m_iScreenWidth(0)
, m_iScreenHeight(0)
, m_fAccmulateTime(0.0f)
, m_bActive(false)
{
	InitializeSurface();
}

//------------------------------------------------------------------------------------
SPShakeScene::~SPShakeScene()
{
	Clear();
}

//------------------------------------------------------------------------------------
bool SPShakeScene::InitializeSurface()
{
	LPDIRECT3DSURFACE9 pBackSurface = NULL;
	g_pVideo->GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurface);
	if( pBackSurface == NULL )
		return false;

	D3DSURFACE_DESC desc;
	pBackSurface->GetDesc(&desc);
	pBackSurface->Release();

	g_pVideo->GetDevice()->CreateOffscreenPlainSurface(desc.Width, desc.Height,
		desc.Format, D3DPOOL_SYSTEMMEM , &m_pOffscreenSurface, NULL);

	if( m_pOffscreenSurface == NULL )
		return false;

	m_iScreenWidth = desc.Width;
	m_iScreenHeight = desc.Height;
	return true;
}

//------------------------------------------------------------------------------------
bool SPShakeScene::InitializeValue()
{
	SetRect(&m_rcOffscreenRgn, 0, 0, m_iScreenWidth, m_iScreenHeight);
	m_ptRenderTargetPos.x = m_ptRenderTargetPos.y = 0;

	m_fAccmulateTime = 0.0f;
	return true;
}

//------------------------------------------------------------------------------------
void SPShakeScene::Clear()
{
	SAFE_RELEASE(m_pOffscreenSurface);
}

//------------------------------------------------------------------------------------
void SPShakeScene::SetActive(bool bActive)
{
	m_bActive = bActive;
	InitializeValue();
}

//------------------------------------------------------------------------------------
unsigned int SPShakeScene::Process(float fElapsedTime)
{
	if( !m_bActive )
		return 0;

	/*
		static float fAcculTime = 0.0f;
		fAcculTime += fTime;

		static float fDiffX = 0, fDiffY = 0;
		if( fAcculTime > 0.01f )
		{
		fDiffX = ((float)(rand() % 100) / 100.0f * 5.0f - 2.5f);
		fDiffY = ((float)(rand() % 100) / 100.0f * 5.0f - 2.5f);

		fAcculTime = 0.0f;
		}

		POINT pt = {100, 0};
		RECT rc = {0, 0, 700, 600};
		if( fDiffX < 0.0f )
		{
		rc.left = (int)-fDiffX;
		rc.right = 800;
		pt.x = 0;
		}
		else
		{
		rc.left = 0;
		rc.right = 800 - (int)fDiffX;

		pt.x = (int)fDiffX;
		}

		if( fDiffY < 0.0f )
		{
		rc.top = (int)-fDiffY;
		rc.bottom = 600;
		pt.y = 0;
		}
		else
		{
		rc.top = 0;
		rc.bottom = 600 - (int)fDiffY;

		pt.y = (int)fDiffY;
		}
	*/

	return 0;
}

//------------------------------------------------------------------------------------
void SPShakeScene::Render()
{
	if( m_pOffscreenSurface == NULL || !m_bActive )
		return;

	LPDIRECT3DSURFACE9 pBackSurface = NULL;
	g_pVideo->GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurface);
	if( pBackSurface == NULL )
		return;

	HRESULT hResult;
	hResult = g_pVideo->GetDevice()->GetRenderTargetData(pBackSurface, m_pOffscreenSurface);
	if( hResult != S_OK )
		return;

	g_pVideo->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET, 0, 1.0f, 0);
	g_pVideo->GetDevice()->UpdateSurface(m_pOffscreenSurface, &m_rcOffscreenRgn, pBackSurface, &m_ptRenderTargetPos);

	pBackSurface->Release();
}