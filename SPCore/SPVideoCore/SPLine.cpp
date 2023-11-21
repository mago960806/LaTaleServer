
#include <assert.h>
#include <D3D9.h>
#include "DXUtil.h"
#include "CD3DEnumeration.h"
#include "D3DSettings.h"

#include "SPVideoCommon.h"
#include "SPVideo.h"
#include "SPTexture.h"
#include "SPLine.h"

SPLine::SPLine()
{
}

SPLine::~SPLine()
{
	SAFE_RELEASE(m_pD3DXLine);
}

HRESULT SPLine::Init(D3DXCOLOR D3DColor)	
{
	HRESULT hr;	
	hr = D3DXCreateLine( g_pVideo->GetDevice(), &m_pD3DXLine );	

	SetColor(D3DColor);
	return hr;
}

void SPLine::SetColor(const D3DXCOLOR Color)
{ 
	m_D3DColor = Color; 
}

HRESULT SPLine::OnLostDevice()
{
	return m_pD3DXLine->OnLostDevice();
}

HRESULT SPLine::OnResetDevice()
{
	return m_pD3DXLine->OnResetDevice();
}


HRESULT SPLine::DrawLine( RECT rect )
{
	D3DXVECTOR2 Vec2[2];

	Vec2[0].x = (float)rect.left;
	Vec2[0].y = (float)rect.top;

	Vec2[1].x = (float)rect.right;
	Vec2[1].y = (float)rect.bottom;


	return m_pD3DXLine->Draw(Vec2, 2, m_D3DColor);
}

HRESULT SPLine::DrawRect( RECT rect )
{
	D3DXVECTOR2 Vec2[5];

	Vec2[0].x = (float)rect.left;
	Vec2[0].y = (float)rect.top;

	Vec2[1].x = (float)rect.right;
	Vec2[1].y = (float)rect.top;

	Vec2[2].x = (float)rect.right;
	Vec2[2].y = (float)rect.bottom;

	Vec2[3].x = (float)rect.left;
	Vec2[3].y = (float)rect.bottom;

	Vec2[4].x = (float)rect.left;
	Vec2[4].y = (float)rect.top;


	return m_pD3DXLine->Draw(Vec2, 5, m_D3DColor);
}






