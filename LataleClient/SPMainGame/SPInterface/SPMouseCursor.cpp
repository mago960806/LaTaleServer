


#include "SPCommon.h"

//#include "SPGameObjectDEF.h"
#include "SPUtil.h"
//#include "SPResourceDef.h"


#include "SPMouseCursor.h"
//#include "SPOptionDef.h"
//#include "SPClientOptionArchive.h"

SPMouseCursor* g_pMouseCursor = NULL;

SPMouseCursor::SPMouseCursor()
: m_MouseType(MOUSE_TYPE_DEFAULT)
, m_fAccumulateTime(0.0f)
, m_nSrcX(0)
, m_nSrcY(0)
, m_pMoveIconImage(NULL)
//, m_ppCurImage(NULL)
, m_bAutoHide(false)
, m_fAutoHideDelayTime(10.0f)
, m_pImage(NULL)
{
	Init();	
	g_pMouseCursor = this;
}

SPMouseCursor::~SPMouseCursor()
{
	SAFE_RELEASE(m_pImage);	
	g_pMouseCursor = NULL;
}

void SPMouseCursor::Init()
{
	char szFName[256];		
	SAFE_RELEASE(m_pImage);	

	g_pVideo->GetTextureMgr()->LoadTexture(	
		//GetAddResPath( "DATA/CURSOR/CURSOR.PNG", RES_WORK_RESOURCE_PATH), 
		"DATA/CURSOR/CURSOR.PNG", 
		&m_pImage);

	SetType();
	//if( g_pClientOption )
	//	SetAutoHide(g_pClientOption->GetOptionStruct()->bAutoHideMouse);
}


void SPMouseCursor::Process(float fTime)
{
	//if(m_fAccumulateTime > 10) m_fAccumulateTime = 10;

	//m_fAccumulateTime += fTime;
	//if(m_fAccumulateTime > 0.06f) {
	//	m_fAccumulateTime -= 0.06f;
	//}

	m_fAccumulateTime += fTime;
	if( m_bAutoHide )
	{
		m_fHideAlpha = 1.0f;
		if( m_fAccumulateTime >= m_fAutoHideDelayTime )
		{
			float fDiffTime = m_fAccumulateTime - m_fAutoHideDelayTime;
			m_fHideAlpha = 1.0f - fDiffTime / 2.0f;
			if( m_fHideAlpha < 0.0f )
				m_fHideAlpha = 0.0f;
		}
	}

	if( m_fAccumulateTime > 20.0f )
		m_fAccumulateTime = 20.0f;

	if( m_MouseType == MOUSE_TYPE_WAIT )
		ProcessWait( fTime );
}

void SPMouseCursor::Render(float fTime)
{	
	if( m_pImage == NULL )
		return;

	g_pVideo->Flush();

	float	fXPos	=	m_fX	+	m_fPosOffsetX;
	float	fYPos	=	m_fY	+	m_fPosOffsetY;
	
	if( m_bAutoHide )
	{
		m_pImage->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fHideAlpha));
		m_pImage->RenderTexture( (int)fXPos , (int)fYPos , &m_CurSrcRect);
		m_pImage->SetColor();
	}
	else
		m_pImage->RenderTexture( (int)fXPos , (int)fYPos , &m_CurSrcRect);
		
	if(m_pMoveIconImage)
	{		
		RECT rtTarget, rtSrc;
		rtTarget.left = (LONG)fXPos -16;
		rtTarget.top = (LONG)fYPos -16;
		rtTarget.right = rtTarget.left+32;
		rtTarget.bottom = rtTarget.top+32;

		rtSrc.left = m_nSrcX ;
		rtSrc.top = m_nSrcY ;
		rtSrc.right = rtSrc.left+32 ;
		rtSrc.bottom = rtSrc.top+32 ;
		m_pMoveIconImage->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );
		m_pMoveIconImage->RenderTexture(&rtTarget ,&rtSrc);	
	}

	//绘制IGW在鼠标之后,并且在绘制鼠标时才绘制IGW
	if ( g_pCIGWLoaderDx9 )
	{
		g_pCIGWLoaderDx9->RenderEx( 0, 0 );
	}
}

void SPMouseCursor::SetCurPos(float fX, float fY)
{
	if( m_bAutoHide )
	{
		if( m_fX != fX || m_fY != fY )
			m_fAccumulateTime = 0.0f;
	}

	m_fX = fX;
	m_fY = fY;
}

void SPMouseCursor::SetMoveIconPos(SPTexture* pImage, int nXSrc, int nYSrc)
{
	m_pMoveIconImage = pImage;
	m_nSrcX = nXSrc;
	m_nSrcY = nYSrc;
}

void SPMouseCursor::ClearMoveIcon()
{
	if(m_pMoveIconImage)
	{
		m_pMoveIconImage = NULL;
		m_nSrcX = m_nSrcY = 0;
	}
}

void SPMouseCursor::SetType(MOUSE_TYPE type)
{
	//if( g_pClientOption )
	//	m_bAutoHide = g_pClientOption->GetOptionStruct()->bAutoHideMouse;

	m_MouseType = type;

	switch(m_MouseType) {
	case MOUSE_TYPE_DEFAULT	:
		m_CurSrcRect.left	= 0;
		m_CurSrcRect.top	= 0;
		m_CurSrcRect.right	= 32;
		m_CurSrcRect.bottom = 32;
		m_fPosOffsetX		= 0.0f;
		m_fPosOffsetY		= 0.0f;
		break;

	case MOUSE_TYPE_WAIT	:
		{
			int	iXIndex	=	m_iCurFrameWait;
			int	iYIndex	=	1;

			if( m_iCurFrameWait > 7 )
			{
				iXIndex	=	m_iCurFrameWait	-	8;
				iYIndex	=	2;
			}

			m_CurSrcRect.left			= 32 * iXIndex;
			m_CurSrcRect.top			= 32 * iYIndex;
			m_CurSrcRect.right			= m_CurSrcRect.left + 32;
			m_CurSrcRect.bottom			= m_CurSrcRect.top + 32;
			m_fPosOffsetX				= -16.0f;
			m_fPosOffsetY				= -16.0f;
			SetAutoHide( false );
		}
		break;

	case MOUSE_TYPE_HRESIZE	:
		m_CurSrcRect.left	= 0;
		m_CurSrcRect.top	= 32*3;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = m_CurSrcRect.top + 32;
		m_fPosOffsetX		= 0.0f;
		m_fPosOffsetY		= 0.0f;
		break;

	case MOUSE_TYPE_VRESIZE	:
		m_CurSrcRect.left	= 0;
		m_CurSrcRect.top	= 32*4;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = m_CurSrcRect.top + 32;
		m_fPosOffsetX		= 0.0f;
		m_fPosOffsetY		= 0.0f;
		break;

	case MOUSE_TYPE_NPC_TALK:
		m_CurSrcRect.left	= 32 * 1;
		m_CurSrcRect.top	= 0;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = 32;
		m_fPosOffsetX		= 0.0f;
		m_fPosOffsetY		= 0.0f;
		break;

	case MOUSE_TYPE_PC_TALK:
		m_CurSrcRect.left	= 32 * 2;
		m_CurSrcRect.top	= 0;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = 32;
		m_fPosOffsetX		= 0.0f;
		m_fPosOffsetY		= 0.0f;
		break;

	case MOUSE_TYPE_CHAT_BOARD:
		m_CurSrcRect.left	= 32 * 1;
		m_CurSrcRect.top	= 0;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = 32;
		m_fPosOffsetX		= 0.0f;
		m_fPosOffsetY		= 0.0f;
		break;

	case MOUSE_TYPE_WORLDMAP_LEFT:
		m_CurSrcRect.left	= 32 * 6;
		m_CurSrcRect.top	= 32 * 0;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = m_CurSrcRect.top + 32;
		m_fPosOffsetX		= -6.0f;
		m_fPosOffsetY		= -16.0f;
		break;

	case MOUSE_TYPE_WORLDMAP_TOP:
		m_CurSrcRect.left	= 32 * 4;
		m_CurSrcRect.top	= 32 * 0;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = m_CurSrcRect.top + 32;
		m_fPosOffsetX		= -16.0f;
		m_fPosOffsetY		= -6.0f;
		break;

	case MOUSE_TYPE_WORLDMAP_RIGHT:
		m_CurSrcRect.left	= 32 * 7;
		m_CurSrcRect.top	= 32 * 0;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = m_CurSrcRect.top + 32;
		m_fPosOffsetX		= -24.0f;
		m_fPosOffsetY		= -16.0f;
		break;

	case MOUSE_TYPE_WORLDMAP_BOTTOM:
		m_CurSrcRect.left	= 32 * 5;
		m_CurSrcRect.top	= 32 * 0;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = m_CurSrcRect.top + 32;
		m_fPosOffsetX		= -16.0f;
		m_fPosOffsetY		= -24.0f;
		break;

	case MOUSE_TYPE_WORLDMAP_LEFT_TOP:
		m_CurSrcRect.left	= 32 * 3;
		m_CurSrcRect.top	= 32 * 2;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = m_CurSrcRect.top + 32;
		m_fPosOffsetX		= -8.0f;
		m_fPosOffsetY		= -8.0f;
		break;

	case MOUSE_TYPE_WORLDMAP_LEFT_BOTTOM:
		m_CurSrcRect.left	= 32 * 5;
		m_CurSrcRect.top	= 32 * 2;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = m_CurSrcRect.top + 32;
		m_fPosOffsetX		= -23.0f;
		m_fPosOffsetY		= -23.0f;
		break;

	case MOUSE_TYPE_WORLDMAP_RIGHT_TOP:
		m_CurSrcRect.left	= 32 * 4;
		m_CurSrcRect.top	= 32 * 2;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = m_CurSrcRect.top + 32;
		m_fPosOffsetX		= -24.0f;
		m_fPosOffsetY		= -8.0f;
		break;

	case MOUSE_TYPE_WORLDMAP_RIGHT_BOTTOM:
		m_CurSrcRect.left	= 32 * 6;
		m_CurSrcRect.top	= 32 * 2;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = m_CurSrcRect.top + 32;
		m_fPosOffsetX		= -23.0f;
		m_fPosOffsetY		= -23.0f;
		break;

	case MOUSE_TYPE_WORLDMAP_DRAG:
		m_CurSrcRect.left	= 32 * 7;
		m_CurSrcRect.top	= 32 * 2;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = m_CurSrcRect.top + 32;
		m_fPosOffsetX		= -16.0f;
		m_fPosOffsetY		= -16.0f;
		break;

	case MOUSE_TYPE_WORLDMAP_DETAIL:
		m_CurSrcRect.left	= 32 * 3;
		m_CurSrcRect.top	= 32 * 0;
		m_CurSrcRect.right	= m_CurSrcRect.left + 32;
		m_CurSrcRect.bottom = m_CurSrcRect.top + 32;
		m_fPosOffsetX		= -12.0f;
		m_fPosOffsetY		= -12.0f;
		break;

	}

}

bool SPMouseCursor::IsSetType()
{
	bool	bResult	=	false;

	if( m_MouseType != MOUSE_TYPE_DEFAULT )
		bResult	=	true;

	return bResult;
}

void SPMouseCursor::SetAutoHide(bool bEnable)
{
	m_bAutoHide = bEnable;
	m_fAccumulateTime = 0.0f;

	m_fHideAlpha = 1.0f;
}

bool SPMouseCursor::IsShowCursor()
{
	if( m_fHideAlpha == 0.0f )
		return false;
	else
		return true;
}

SPMouseCursor::MOUSE_TYPE SPMouseCursor::GetMouseType()
{
	return m_MouseType;
}

void SPMouseCursor::ResetWaitData()
{
	m_fAccumulateTimeWait		=	0.0f;
	m_fAccumulateTimeLimitWait	=	0.15f;
	m_iCurFrameWait				=	0;
	m_iMaxFrameWait				=	11;
}

SIZE SPMouseCursor::GetMouseSize()
{
	SIZE	ptMouseSize;
	

	ptMouseSize.cx	=	m_CurSrcRect.right	-	m_CurSrcRect.left;
	ptMouseSize.cy	=	m_CurSrcRect.bottom	-	m_CurSrcRect.top;

	return ptMouseSize;
}

void SPMouseCursor::ProcessWait( float fTime )
{
	m_fAccumulateTimeWait	+=	fTime;

	if( m_fAccumulateTimeWait < m_fAccumulateTimeLimitWait )
		return;

	m_fAccumulateTimeWait	=	0.0f;

	m_iCurFrameWait++;

	if( m_iCurFrameWait >= m_iMaxFrameWait )
		m_iCurFrameWait	=	0;

	int	iXIndex	=	m_iCurFrameWait;
	int	iYIndex	=	1;

	if( m_iCurFrameWait > 7 )
	{
		iXIndex	=	m_iCurFrameWait	-	8;
		iYIndex	=	2;
	}

	m_CurSrcRect.left			= 32 * iXIndex;
	m_CurSrcRect.top			= 32 * iYIndex;
	m_CurSrcRect.right			= m_CurSrcRect.left + 32;
	m_CurSrcRect.bottom			= m_CurSrcRect.top + 32;
}
