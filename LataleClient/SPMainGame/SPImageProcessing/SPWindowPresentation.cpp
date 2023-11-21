
#include "SPCommon.h"
#include "SPImageProcessor.h"
#include "SPWindowPresentation.h"

//------------------------------------------------------------------------------------
SPWindowPresentation::SPWindowPresentation(unsigned int uiWIID, const char* pszImage,
	RECT rcSrc, float fStrPosX, float fStrPosY, float fDstPosX, float fDstPosY,
	float fVelocityX, float fVelocityY, bool bShow)
: SPImageProcessor(pszImage, rcSrc)
, m_uiWIID(uiWIID)
, m_fStrPosX(fStrPosX)
, m_fStrPosY(fStrPosY)
, m_fCurPosX(fStrPosX)
, m_fCurPosY(fStrPosY)
, m_fDstPosX(fDstPosX)
, m_fDstPosY(fDstPosY)
, m_fVelocityX(fVelocityX)
, m_fVelocityY(fVelocityY)
, m_fAlpha(0.0f)
, m_bShow(bShow)
{
	if( m_fCurPosX < m_fDstPosX )
		m_iDirectionX = 2;
	else if( m_fCurPosX > m_fDstPosX )
		m_iDirectionX = 1;
	else
		m_iDirectionX = 0;

	if( m_fCurPosY < m_fDstPosY )
		m_iDirectionY = 2;
	else if( m_fCurPosY > m_fDstPosY )
		m_iDirectionY = 1;
	else
		m_iDirectionY = 0;
}

//------------------------------------------------------------------------------------
SPWindowPresentation::SPWindowPresentation(unsigned int uiWIID, const char* pszImage,
	RECT rcSrc, float fStrPosX, float fStrPosY, float fDstPosX, float fDstPosY,
	float fLifeTime, bool bShow)
: SPImageProcessor(pszImage, rcSrc)
, m_uiWIID(uiWIID)
, m_fStrPosX(fStrPosX)
, m_fStrPosY(fStrPosY)
, m_fCurPosX(fStrPosX)
, m_fCurPosY(fStrPosY)
, m_fDstPosX(fDstPosX)
, m_fDstPosY(fDstPosY)
, m_fAlpha(0.0f)
, m_bShow(bShow)
{
	if( m_fCurPosX < m_fDstPosX )
		m_iDirectionX = 2;
	else if( m_fCurPosX > m_fDstPosX )
		m_iDirectionX = 1;
	else
		m_iDirectionX = 0;

	if( m_fCurPosY < m_fDstPosY )
		m_iDirectionY = 2;
	else if( m_fCurPosY > m_fDstPosY )
		m_iDirectionY = 1;
	else
		m_iDirectionY = 0;

	if( fLifeTime == 0.0f )
	{
		m_fVelocityX = 0.0f;
		m_fVelocityY = 0.0f;
	}
	else
	{
		float fDistance;
		fDistance = (float)abs(m_fStrPosX - m_fDstPosX);
		m_fVelocityX = fDistance / fLifeTime;

		fDistance = (float)abs(m_fStrPosY - m_fDstPosY);
		m_fVelocityY = fDistance / fLifeTime;
	}
}

//------------------------------------------------------------------------------------
SPWindowPresentation::~SPWindowPresentation()
{
}

//------------------------------------------------------------------------------------
unsigned int SPWindowPresentation::Process(float fElapsedTime)
{
	switch( m_iDirectionX )
	{
	case 1:
		{
			m_fCurPosX -= (fElapsedTime * m_fVelocityX);
			if( m_fCurPosX < m_fDstPosX )
			{
				m_fCurPosX = m_fDstPosX;
				m_iDirectionX = 0;
			}
		}
		break;

	case 2:
		{
			m_fCurPosX += (fElapsedTime * m_fVelocityX);
			if( m_fCurPosX > m_fDstPosX )
			{
				m_fCurPosX = m_fDstPosX;
				m_iDirectionX = 0;
			}
		}
		break;
	}

	switch( m_iDirectionY )
	{
	case 1:
		{
			m_fCurPosY -= (fElapsedTime * m_fVelocityY);
			if( m_fCurPosY < m_fDstPosY )
			{
				m_fCurPosY = m_fDstPosY;
				m_iDirectionY = 0;
			}
		}
		break;

	case 2:
		{
			m_fCurPosY += (fElapsedTime * m_fVelocityY);
			if( m_fCurPosY > m_fDstPosY )
			{
				m_fCurPosY = m_fDstPosY;
				m_iDirectionY = 0;
			}
		}
		break;
	}

	float fAlphaX, fAlphaY;
	if( m_fDstPosX == m_fStrPosX )
		fAlphaX = 1.0f;
	else
		fAlphaX = 1.0f - (float)abs(m_fDstPosX - m_fCurPosX) / (float)abs(m_fDstPosX - m_fStrPosX);

	if( m_fDstPosY == m_fStrPosY )
		fAlphaY = 1.0f;
	else
		fAlphaY = 1.0f - (float)abs(m_fDstPosY - m_fCurPosY) / (float)abs(m_fDstPosY - m_fStrPosY);

	m_fAlpha = fAlphaX * fAlphaY;
	if( !m_bShow )
		m_fAlpha = 1.0f - m_fAlpha;

	if( m_fAlpha < 0.1f )
		m_fAlpha = 0.1f;
	if( m_fAlpha > 0.9f )
		m_fAlpha = 0.9f;

	if( m_iDirectionX == 0 && m_iDirectionY == 0 ) 
		return m_uiWIID;

	return 0;
}

//------------------------------------------------------------------------------------
void SPWindowPresentation::Render()
{
	if( m_pkTexture == NULL )
		return;

	RECT rcDst;
	int iWidth = m_rcSrcRgn.right - m_rcSrcRgn.left;
	int iHeight = m_rcSrcRgn.bottom - m_rcSrcRgn.top;

	SetRect(&rcDst, (int)m_fCurPosX, (int)m_fCurPosY,
		(int)m_fCurPosX + iWidth, (int)m_fCurPosY + iHeight);

	m_pkTexture->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
	m_pkTexture->RenderTexture(&rcDst, &m_rcSrcRgn);
	m_pkTexture->SetColor();

	g_pVideo->Flush();
}
