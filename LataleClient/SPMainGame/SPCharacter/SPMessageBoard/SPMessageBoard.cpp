// ***************************************************************
//  SPChatBoard   version:  1.0   ¡¤  date: 04/10/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"
#include "SPGameObject.h"

#include "SPCameraManager.h"

#include "GlobalDefines_Share.h"

#include "SPFont.h"
#include "SPAbuseFilter.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPCommandConvert.h"
#include "SPGOBStatus.h"
#include "SPGOBManager.h"
#include "SPGOBCoordPhysics.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"

#include "SPChatManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowDestoryMsg.h"
#include "SPWindowChattingRoom.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPResourceManager.h"

#include "SPDebug.h"

#include "SPMessageBoard.h"


SPMessageBoard::SPMessageBoard()
{
	m_iType = 0;
	m_pTexture = NULL;
}

SPMessageBoard::SPMessageBoard( INSTANCE_ID instanceID , CLASS_ID ClassID )
: SPGameObject( instanceID , ClassID )
{
	m_iType = 0;
	m_pTexture = NULL;
}

SPMessageBoard::~SPMessageBoard()
{
	Destroy();
}

void SPMessageBoard::Init()
{
	Destroy();
}

void SPMessageBoard::Destroy()
{
	SAFE_RELEASE( m_pTexture );
}

void SPMessageBoard::Process( float fTime )
{
	SPGameObject::Process( fTime );

	if( g_pCameraManagr != NULL )
	{
		SPCameraStruct*	pCameraStruct	=	g_pCameraManagr->GetCameraStruct();

		if( pCameraStruct != NULL )
		{
			m_fCamX	=	pCameraStruct->fOffsetX;
			m_fCamY	=	pCameraStruct->fOffsetY;
		}
	}
	
	m_iScreenX = m_iPosX - m_fCamX;
	m_iScreenY = m_iPosY - m_fCamY;
		 
	MSG_BOARD * pMsgBoard = g_pGOBCluster->GetMsgBoardByType(m_iType);
	if ( !pMsgBoard )
		return;

	m_rcTextureSrc.left		= pMsgBoard->m_iImageX1;
	m_rcTextureSrc.right	= pMsgBoard->m_iImageX2;
	m_rcTextureSrc.top		= pMsgBoard->m_iImageY1;
	m_rcTextureSrc.bottom	= pMsgBoard->m_iImageY2;
	m_rcTextDest.left		= pMsgBoard->m_iTextX1 + m_iScreenX;
	m_rcTextDest.right		= pMsgBoard->m_iTextX2 + m_iScreenX;
	m_rcTextDest.top		= pMsgBoard->m_iTextY1 + m_iScreenY;
	m_rcTextDest.bottom		= pMsgBoard->m_iTextY2 + m_iScreenY;
	m_rcTitleDest.left		= pMsgBoard->m_iTitleX + m_iScreenX;
	m_rcTitleDest.top		= pMsgBoard->m_iTitleY + m_iScreenY;
	m_rcTitleDest.right		= m_rcTitleDest.left + 200;
	m_rcTitleDest.bottom	= m_rcTitleDest.top  + 16;

	if ( !m_pTexture )
	{
		string   strTexName;
		if ( g_pResourceManager->GetGlobalFileName(pMsgBoard->m_iIcon) )
			strTexName = g_pResourceManager->GetGlobalFileName(pMsgBoard->m_iIcon);
		else
			strTexName = "DATA/INTERFACE/CONCEPT/MessageBoard1.png";

		g_pVideo->GetTextureMgr()->LoadTexture(strTexName.c_str(), &m_pTexture);
	}


}

void SPMessageBoard::Render( float fTime )
{
	if( m_pTexture )
	{
		int  iScreenWidth = 0;
		int  iScreenHeight = 0;
		int  iScreenX2 = m_iScreenX + (m_rcTextureSrc.right - m_rcTextureSrc.left);
		int  iScreenY2 = m_iScreenY + (m_rcTextureSrc.bottom - m_rcTextureSrc.top);

		if ( iScreenX2 >= 0 && m_iScreenX <= 800
			&& iScreenY2 >= 0 && m_iScreenY <= 600 )
		{
			m_pTexture->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );
			m_pTexture->RenderTexture(m_iScreenX, m_iScreenY, &m_rcTextureSrc );

			if ( g_pVideo && g_pVideo->GetFont() )
			{
				char  szTitle[256], szText[1024];
				sprintf(szTitle, "%s %dÔÂ%dÈÕÁô", m_strName.c_str(), m_tmDate.GetMonth(), m_tmDate.GetDay());
				sprintf(szText, "%s", m_strMsg.c_str());

				D3DXCOLOR  oldShadowColor = g_pVideo->GetFont(FONT_12_NORMAL)->GetShadowColor();

				g_pVideo->GetFont(FONT_12_NORMAL)->SetColor(ARGB(255, 157, 4, 85));
				g_pVideo->GetFont(FONT_12_NORMAL)->SetShadow(true);
				g_pVideo->GetFont(FONT_12_NORMAL)->SetShadowColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				g_pVideo->GetFont(FONT_12_NORMAL)->RenderText(szTitle, &m_rcTitleDest);

				g_pVideo->GetFont(FONT_12_NORMAL)->SetColor(ARGB(255, 157, 4, 85));
				g_pVideo->GetFont(FONT_12_NORMAL)->SetShadow(true);
				g_pVideo->GetFont(FONT_12_NORMAL)->SetShadowColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				g_pVideo->GetFont(FONT_12_NORMAL)->RenderText(szText, &m_rcTextDest, DT_VCENTER | DT_WORDBREAK);

				g_pVideo->GetFont(FONT_12_NORMAL)->SetShadowColor(oldShadowColor);
			}

			if( g_pVideo != NULL )
				g_pVideo->Flush();
		}
	}

}

int SPMessageBoard::PerformMouseMove( int iX , int iY )
{
	if ( !IsCursorIn( iX , iY ))
	{
		SetStandLayer(BG_ID_LAYER_TERRAIN0);
		return 0;
	}
	return 1;
}

int SPMessageBoard::PerformMouseLUp( int iX , int iY )
{
	if ( IsCursorIn(iX , iY) )
	{
		//SetStandLayer(BG_ID_LAYER_TERRAIN2);
		SPWindowDestoryMsg * pWindow = static_cast<SPWindowDestoryMsg *>(g_pInterfaceManager->FindWindow(WIID_DESTORYMSG));
		if ( pWindow )
		{
			pWindow->DestoryMsgBoard(m_InstanceID, m_ClassID, m_iType);
		}
		return 1;
	}
	return 0;
}

int SPMessageBoard::PerformMouseLDBLClick( int iX , int iY )
{
	return 0;
}


bool SPMessageBoard::IsCursorIn( int iX , int iY )
{
	int  iScreenX2 = m_iScreenX + (m_rcTextureSrc.right - m_rcTextureSrc.left);
	int  iScreenY2 = m_iScreenY + (m_rcTextureSrc.bottom - m_rcTextureSrc.top);

	bool	bLeftTop		=	( m_iScreenX <= iX && iScreenX2   >= iX );
	bool	bRightBottom	=	( m_iScreenY  <= iY && iScreenY2  >= iY );

	return bLeftTop && bRightBottom;
}

bool SPMessageBoard::IsCursorIn( RECT & rc )
{
	int  iScreenX2 = m_iScreenX + (m_rcTextureSrc.right - m_rcTextureSrc.left);
	int  iScreenY2 = m_iScreenY + (m_rcTextureSrc.bottom - m_rcTextureSrc.top);

	bool	bLeft			=	( m_iScreenX	<= rc.left	 && iScreenX2	>= rc.left		);
	bool	bTop			=	( m_iScreenY	<= rc.top	 && iScreenY2   >= rc.top		);
	bool	bRight			=	( m_iScreenX	<= rc.right	 && iScreenX2	>= rc.right		);
	bool	bBottom			=	( m_iScreenY	<= rc.bottom && iScreenY2   >= rc.bottom	);

	bool	bLeftTop		=	bLeft	& bTop;
	bool	bLeftBottom		=	bLeft	& bBottom;
	bool	bRightTop		=	bRight	& bTop;
	bool	bRightBottom	=	bRight	& bBottom;

	//
	bool	bLeft2			=	( m_iScreenX	>= rc.left	 && m_iScreenX	<= rc.right		);
	bool	bTop2			=	( m_iScreenY	>= rc.top	 && m_iScreenY  <= rc.bottom	);
	bool	bRight2			=	( iScreenX2		>= rc.left	 && iScreenX2	<= rc.right		);
	bool	bBottom2		=	( iScreenY2		>= rc.top	 && iScreenY2   <= rc.bottom	);

	bool	bLeftTop2		=	bLeft2	& bTop2;
	bool	bLeftBottom2	=	bLeft2	& bBottom2;
	bool	bRightTop2		=	bRight2	& bTop2;
	bool	bRightBottom2	=	bRight2	& bBottom2;

	return bLeftTop | bLeftBottom | bRightTop | bRightBottom
			| bLeftTop2 | bLeftBottom2 | bRightTop2 | bRightBottom2;
}

void SPMessageBoard::SetPosX( int iPosX )
{
	m_iPosX	=	iPosX;
}

void SPMessageBoard::SetPosY( int iPosY )
{
	m_iPosY	=	iPosY;
}

int SPMessageBoard::GetPosX()
{
	return m_iPosX;
}

int SPMessageBoard::GetPosY()
{
	return m_iPosY;
}

void SPMessageBoard::SetStandLayer( int iLayer )
{
	m_iStandLayer	=	iLayer;
}

int SPMessageBoard::GetStandLayer()
{
	return m_iStandLayer;
}

int SPMessageBoard::OnGetStandlayer( LPARAM lParam )
{
	return GetStandLayer();
}


SPGMESSAGE_OBJ_MAP_BEGIN( SPMessageBoard )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETSTANDLAYER			,	OnGetStandlayer			)
SPGMESSAGE_OBJ_MAP_END( SPGameObject )

