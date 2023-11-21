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
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"

#include "SPChatManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowChattingRoom.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPDebug.h"

#include "SPChatBoard.h"

SPChatBoard::SPChatBoard()
{
	for( m_iTempIndex = 0 ; m_iTempIndex < CHATBOARD_TEXTURE_MAX ; ++m_iTempIndex )
	{
		m_pTexture[ m_iTempIndex ]	=	NULL;
	}
}

SPChatBoard::SPChatBoard( INSTANCE_ID instanceID , CLASS_ID ClassID )
: SPGameObject( instanceID , ClassID )
{
	for( m_iTempIndex = 0 ; m_iTempIndex < CHATBOARD_TEXTURE_MAX ; ++m_iTempIndex )
	{
		m_pTexture[ m_iTempIndex ]	=	NULL;
	}
}

SPChatBoard::~SPChatBoard()
{
	Destroy();
}

void SPChatBoard::Init()
{
	Destroy();

	SPGameObject::Init();

	m_iStandLayer	=	0;

	m_fCamX			=	0.0f;
	m_fCamY			=	0.0f;

	if( g_pVideo != NULL )
	{
		SPTextureMgr*	pTextureMgr	=	g_pVideo->GetTextureMgr();

		if( pTextureMgr != NULL )
		{
			pTextureMgr->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_CHAT_TB_000.PNG" , &m_pTexture[ CHATBOARD_TEXTURE_BG ] );
			pTextureMgr->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_CHAT_BG_000.PNG" , &m_pTexture[ CHATBOARD_TEXTURE_OBJECT ] );
			pTextureMgr->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_OBJECT_002.PNG" , &m_pTexture[ CHATBOARD_TEXTURE_LOCK ] );
			pTextureMgr->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_OBJECT_000.PNG" , &m_pTexture[ CHATBOARD_TEXTURE_TYPE ] );
			pTextureMgr->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_OBJECT_002.PNG" , &m_pTexture[ CHATBOARD_TEXTURE_NO ] );
			pTextureMgr->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_OBJECT_000.PNG" , &m_pTexture[ CHATBOARD_TEXTURE_NO_1 ] );
			pTextureMgr->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_OBJECT_000.PNG" , &m_pTexture[ CHATBOARD_TEXTURE_NO_10 ] );
			pTextureMgr->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_OBJECT_000.PNG" , &m_pTexture[ CHATBOARD_TEXTURE_NO_100 ] );
			pTextureMgr->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_OBJECT_002.PNG" , &m_pTexture[ CHATBOARD_TEXTURE_FULL ] );
		}

		m_pFont = g_pVideo->GetFont( FONT_12_NORMAL );
	}
	else
	{
		m_pFont	=	NULL;
	}

	//////////////////////////////////////////////////////////////////////////

	SetRect( &m_rcSrcType[ CHATBOARD_TYPE_NORMAL	] , 346 , 252 , 362 , 268 );
	SetRect( &m_rcSrcType[ CHATBOARD_TYPE_WANTED	] , 346 , 269 , 362 , 285 );
	SetRect( &m_rcSrcType[ CHATBOARD_TYPE_MARKET	] , 363 , 252 , 379 , 268 );
	SetRect( &m_rcSrcType[ CHATBOARD_TYPE_MEETING	] , 363 , 269 , 379 , 285 );

	int	iTempPosX , iTempPosY;
	int	iTempSizeX , iTempSizeY;

	for( m_iTempIndex = 0 ; m_iTempIndex < CHATBOARD_NO_MAX ; ++m_iTempIndex )
	{
		iTempPosX	=	363 + ( 7 * m_iTempIndex );

		SetRect( &m_rcSrcNo[ m_iTempIndex	] , iTempPosX , 93 , iTempPosX + 6 , 100 );
	}

	m_iTempIndex	=	CHATBOARD_LOCK_NO;
	iTempPosX		=	0;
	iTempPosY		=	0;
	iTempSizeX		=	0;
	iTempSizeY		=	0;
	SetRect( &m_rcSrcLock[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_LOCK_YES;
	iTempPosX		=	36;
	iTempPosY		=	16;
	iTempSizeX		=	10;
	iTempSizeY		=	11;
	SetRect( &m_rcSrcLock[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_FULL_NO;
	iTempPosX		=	0;
	iTempPosY		=	0;
	iTempSizeX		=	0;
	iTempSizeY		=	0;
	SetRect( &m_rcSrcFull[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_FULL_YES;
	iTempPosX		=	85;
	iTempPosY		=	67;
	iTempSizeX		=	52;
	iTempSizeY		=	12;
	SetRect( &m_rcSrcFull[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	//////////////////////////////////////////////////////////////////////////

	m_iTempIndex	=	CHATBOARD_TEXTURE_BG;
	iTempPosX		=	-128;
	iTempPosY		=	-227;
	iTempSizeX		=	256;
	iTempSizeY		=	128;
	SetRect( &m_rcDestOriginal[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_TEXTURE_OBJECT;
	iTempPosX		=	-64;
	iTempPosY		=	-128;
	iTempSizeX		=	128;
	iTempSizeY		=	128;
	SetRect( &m_rcDestOriginal[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_TEXTURE_LOCK;
	iTempPosX		=	-85;
	iTempPosY		=	-190;
	iTempSizeX		=	10;
	iTempSizeY		=	11;
	SetRect( &m_rcDestOriginal[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_TEXTURE_TYPE;
	iTempPosX		=	-72;
	iTempPosY		=	-179;
	iTempSizeX		=	16;
	iTempSizeY		=	16;
	SetRect( &m_rcDestOriginal[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_TEXTURE_NO;
	iTempPosX		=	-79;
	iTempPosY		=	-157;
	iTempSizeX		=	13;
	iTempSizeY		=	4;
	SetRect( &m_rcDestOriginal[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_TEXTURE_NO_100;
	iTempPosX		=	-65;
	iTempPosY		=	-159;
	iTempSizeX		=	6;
	iTempSizeY		=	7;
	SetRect( &m_rcDestOriginal[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_TEXTURE_NO_10;
	iTempPosX		=	-65 + 6;
	iTempPosY		=	-159;
	iTempSizeX		=	6;
	iTempSizeY		=	7;
	SetRect( &m_rcDestOriginal[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_TEXTURE_NO_1;
	iTempPosX		=	-65 + 12;
	iTempPosY		=	-159;
	iTempSizeX		=	6;
	iTempSizeY		=	7;
	SetRect( &m_rcDestOriginal[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_TEXTURE_FULL;
	iTempPosX		=	37;
	iTempPosY		=	-149;
	iTempSizeX		=	52;
	iTempSizeY		=	12;
	SetRect( &m_rcDestOriginal[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	SetRect( &m_rcTextDestOriginal , -40 , -179 , -40 + 124 , -179 + 26 );

	//////////////////////////////////////////////////////////////////////////

	m_iTempIndex	=	CHATBOARD_TEXTURE_BG;
	iTempPosX		=	0;
	iTempPosY		=	0;
	iTempSizeX		=	256;
	iTempSizeY		=	128;
	SetRect( &m_rcSrc[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_TEXTURE_OBJECT;
	iTempPosX		=	0;
	iTempPosY		=	0;
	iTempSizeX		=	128;
	iTempSizeY		=	128;
	SetRect( &m_rcSrc[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iTempIndex	=	CHATBOARD_TEXTURE_NO;
	iTempPosX		=	49;
	iTempPosY		=	57;
	iTempSizeX		=	13;
	iTempSizeY		=	4;
	SetRect( &m_rcSrc[ m_iTempIndex ] , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_rcSrc[ CHATBOARD_TEXTURE_LOCK ]	=	m_rcSrcLock[ CHATBOARD_LOCK_NO ];
	m_rcSrc[ CHATBOARD_TEXTURE_TYPE ]	=	m_rcSrcType[ CHATBOARD_TYPE_NORMAL ];
	m_rcSrc[ CHATBOARD_TEXTURE_NO_100 ]	=	m_rcSrcNo[ CHATBOARD_NO_0 ];
	m_rcSrc[ CHATBOARD_TEXTURE_NO_10 ]	=	m_rcSrcNo[ CHATBOARD_NO_0 ];
	m_rcSrc[ CHATBOARD_TEXTURE_NO_1 ]	=	m_rcSrcNo[ CHATBOARD_NO_0 ];
	m_rcSrc[ CHATBOARD_TEXTURE_FULL ]	=	m_rcSrcFull[ CHATBOARD_FULL_NO ];

	//////////////////////////////////////////////////////////////////////////

	iTempPosX		=	-128;
	iTempPosY		=	-227;
	iTempSizeX		=	256;
	iTempSizeY		=	227;
	SetRect( &m_rcBoundOriginal , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	iTempPosX		=	-93;
	iTempPosY		=	-198;
	iTempSizeX		=	186;
	iTempSizeY		=	70;
	SetRect( &m_rcBoundClickOriginal , iTempPosX , iTempPosY , iTempPosX + iTempSizeX , iTempPosY + iTempSizeY );

	m_iScreenX	=	800;
	m_iScreenY	=	600;

	if( g_pVideo != NULL )
	{
		m_iScreenX	=	g_pVideo->GetScreenLenX();
		m_iScreenY	=	g_pVideo->GetScreenLenY();
	}
}

void SPChatBoard::Destroy()
{
	for( m_iTempIndex = 0 ; m_iTempIndex < CHATBOARD_TEXTURE_MAX ; ++m_iTempIndex )
	{
		SAFE_RELEASE( m_pTexture[ m_iTempIndex ] );
	}
}

void SPChatBoard::Process( float fTime )
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

	m_iPosX	=	m_iPosXOriginal - (int)m_fCamX;
	m_iPosY	=	m_iPosYOriginal - (int)m_fCamY;

	m_rcBound.left			=	m_iPosX	+	m_rcBoundOriginal.left;
	m_rcBound.top			=	m_iPosY	+	m_rcBoundOriginal.top;
	m_rcBound.right			=	m_iPosX	+	m_rcBoundOriginal.right;
	m_rcBound.bottom		=	m_iPosY	+	m_rcBoundOriginal.bottom;

	m_rcBoundClick.left		=	m_iPosX	+	m_rcBoundClickOriginal.left;
	m_rcBoundClick.top		=	m_iPosY	+	m_rcBoundClickOriginal.top;
	m_rcBoundClick.right	=	m_iPosX	+	m_rcBoundClickOriginal.right;
	m_rcBoundClick.bottom	=	m_iPosY	+	m_rcBoundClickOriginal.bottom;

	if( m_rcBound.right < 0 || m_rcBound.left > m_iScreenX || m_rcBound.bottom < 0 || m_rcBound.top > m_iScreenY )
	{
		m_bClipping = true;
		return;
	}
	else
		m_bClipping	= false;

	//////////////////////////////////////////////////////////////////////////

	for( m_iTempIndex = 0 ; m_iTempIndex < CHATBOARD_TEXTURE_MAX ; ++m_iTempIndex )
	{
		m_rcDest[ m_iTempIndex ].left		=	m_iPosX + m_rcDestOriginal[ m_iTempIndex ].left;
		m_rcDest[ m_iTempIndex ].top		=	m_iPosY + m_rcDestOriginal[ m_iTempIndex ].top;
		m_rcDest[ m_iTempIndex ].right		=	m_iPosX + m_rcDestOriginal[ m_iTempIndex ].right;
		m_rcDest[ m_iTempIndex ].bottom		=	m_iPosY + m_rcDestOriginal[ m_iTempIndex ].bottom;
	}

	int	iTextBufCount	=	(int)m_vTextDestOriginal.size();
	RECT*	pDestBuf;
	RECT*	pDestBufOriginal;

	for( m_iTempIndex = 0 ; m_iTempIndex < iTextBufCount ; ++m_iTempIndex )
	{
		pDestBuf			=	&( m_vTextDest.at( m_iTempIndex ) );
		pDestBufOriginal	=	&( m_vTextDestOriginal.at( m_iTempIndex ) );

		if( pDestBuf != NULL && pDestBufOriginal != NULL )
		{
			pDestBuf->left		=	m_iPosX	+	pDestBufOriginal->left;
			pDestBuf->top		=	m_iPosY	+	pDestBufOriginal->top;
			pDestBuf->right		=	m_iPosX	+	pDestBufOriginal->right;
			pDestBuf->bottom	=	m_iPosY	+	pDestBufOriginal->bottom;
		}
	}
}

void SPChatBoard::Render( float fTime )
{
	if( !m_bClipping )
	{
		for( m_iTempIndex = 0 ; m_iTempIndex < CHATBOARD_TEXTURE_MAX ; ++m_iTempIndex )
		{
			if( m_pTexture[ m_iTempIndex ] != NULL )
			{
				m_pTexture[ m_iTempIndex ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );
				m_pTexture[ m_iTempIndex ]->RenderTexture( &m_rcDest[ m_iTempIndex ] , &m_rcSrc[ m_iTempIndex ] );

				if( g_pVideo != NULL )
					g_pVideo->Flush();
			}
		}

		int	iTextDestCount	=	(int)m_vTextDest.size();
		int	iTextBufCount	=	(int)m_vTextBuf.size();

		if( m_pFont != NULL && iTextDestCount > 0 && iTextBufCount > 0 && iTextDestCount == iTextBufCount )
		{
			for( m_iTempIndex = 0 ; m_iTempIndex < iTextBufCount ; ++m_iTempIndex )
			{
				m_pFont->SetColor( D3DXCOLOR( 0.0f , 0.0f , 0.0f , 1.0f ) );
				m_pFont->SetShadow( false );

				m_pFont->RenderText( m_vTextBuf.at( m_iTempIndex ).c_str() , &( m_vTextDest.at( m_iTempIndex ) ) , DT_LEFT | DT_TOP );

				if( g_pVideo != NULL )
					g_pVideo->Flush();
			}
		}

#if defined(_DEBUG)
		g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ) );
		g_pVideo->GetSysLine()->DrawRect( m_rcBoundClick );

		g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 1.0f , 0.0f , 0.0f , 1.0f ) );
		g_pVideo->GetSysLine()->DrawRect( m_rcBound );

#endif
	}
}

int SPChatBoard::PerformMouseMove( int iX , int iY )
{
	return (int)IsCursorIn( iX , iY );
}

int SPChatBoard::PerformMouseLUp( int iX , int iY )
{
	if( IsCursorIn( iX , iY ) == true )
	{
		GU_ID				ObjectGUID		=	0;
		CHATROOM_INFO		stChatRoomInfo	=	{ 0, };

		if( g_pGOBManager != NULL )
		{
			SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

			if( pLocalPlayer != NULL )
				ObjectGUID	=	pLocalPlayer->GetGUID();
		}

		stChatRoomInfo.iCategory	=	m_stChatRoomInfo.iCategory;
		stChatRoomInfo.uiID			=	m_stChatRoomInfo.uiID;
		stChatRoomInfo.uiRoomNo		=	m_stChatRoomInfo.uiRoomNo;

		if( m_stChatRoomInfo.iPassword > 0 )
			strncpy( stChatRoomInfo.szPassword , "***" , MAX_CHATROOM_PASSWORD_LEN );

		CPacket	Packet;
//		Packet.AddUINT32( CHATROOM_SC_ROOMINFO_CHECK );
		Packet.AddUINT64( ObjectGUID );
		Packet.AddUINT32( 0 );
		Packet.AddData( &( stChatRoomInfo ) , sizeof( CHATROOM_INFO ) );

		if( g_pInterfaceManager != NULL )
			g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_ROOM_CHECK , (WPARAM)1 ,(LPARAM)&Packet );
	}

	return 0;
}

bool SPChatBoard::IsCursorIn( int iX , int iY )
{
	if( m_rcBoundClick.left <= iX && m_rcBoundClick.top <= iY && m_rcBoundClick.right >= iX && m_rcBoundClick.bottom >= iY )
		return true;

	return false;
}

void SPChatBoard::SetPosX( int iPosX )
{
	m_iPosXOriginal	=	iPosX;
}

void SPChatBoard::SetPosY( int iPosY )
{
	m_iPosYOriginal	=	iPosY;
}

int SPChatBoard::GetPosX()
{
	return m_iPosXOriginal;
}

int SPChatBoard::GetPosY()
{
	return m_iPosYOriginal;
}

void SPChatBoard::SetStandLayer( int iLayer )
{
	m_iStandLayer	=	iLayer;
}

int SPChatBoard::GetStandLayer()
{
	return m_iStandLayer;
}

void SPChatBoard::SetChatRoomInfo( CHATROOM_OBJECT_INFO& stChatRoomInfo )
{
	CopyMemory( &m_stChatRoomInfo , &stChatRoomInfo , sizeof( CHATROOM_OBJECT_INFO ) );

	if( m_stChatRoomInfo.iCategory >= 0 && m_stChatRoomInfo.iCategory < CHATBOARD_TYPE_MAX )
		m_rcSrc[ CHATBOARD_TEXTURE_TYPE ]	=	m_rcSrcType[ m_stChatRoomInfo.iCategory ];
	else
		m_rcSrc[ CHATBOARD_TEXTURE_TYPE ]	=	m_rcSrcType[ CHATBOARD_TYPE_NORMAL ];

	if( m_stChatRoomInfo.iPassword > 0 )
		m_rcSrc[ CHATBOARD_TEXTURE_LOCK ]	=	m_rcSrcLock[ CHATBOARD_LOCK_YES ];
	else
		m_rcSrc[ CHATBOARD_TEXTURE_LOCK ]	=	m_rcSrcLock[ CHATBOARD_LOCK_NO ];

	if( stChatRoomInfo.iNowPCNum >= stChatRoomInfo.iLimitNum )
		m_rcSrc[ CHATBOARD_TEXTURE_FULL ]	=	m_rcSrcFull[ CHATBOARD_FULL_YES ];
	else
		m_rcSrc[ CHATBOARD_TEXTURE_FULL ]	=	m_rcSrcFull[ CHATBOARD_FULL_NO ];

	char	szBuf[_MAX_PATH]	=	{};

	int	iRoomNo	=	min( m_stChatRoomInfo.uiRoomNo , 999 );

	sprintf( szBuf , "%3d" , iRoomNo );

	int	istrLength	=	(int)strlen( szBuf );

	if( istrLength > 3 )
		istrLength	=	3;

	std::string	strNumber;
	int	iNumber;

	for( int i = 0 ; i < istrLength ; ++i )
	{
		strNumber	=	szBuf[ i ];

		iNumber	=	atoi( strNumber.c_str() );

		if( iNumber < 0 && iNumber > 10 )
			iNumber	=	0;

		m_rcSrc[ CHATBOARD_TEXTURE_NO_100 + i ]	=	m_rcSrcNo[ iNumber ];
	}

	//////////////////////////////////////////////////////////////////////////

	m_vTextDest.clear();
	m_vTextDestOriginal.clear();
	m_vTextBuf.clear();

	char szLineFeedBuf[ MAX_STR_LENGTH ];

	SPAbuseFilter*	pAbuseFilter = SPAbuseFilter::GetInstance();
	
	if( pAbuseFilter != NULL )
		pAbuseFilter->ReplaceLineFeed( stChatRoomInfo.szTitle , szLineFeedBuf , MAX_STR_LENGTH );

	if( m_pFont != NULL )
	{
		m_pFont->ConvertMultiLineW( szLineFeedBuf , m_rcTextDestOriginal , m_vTextBuf );

		int	iLineInterval		=	3;
		int	iFontHeight			=	(int)m_pFont->GetHeight();
		int	iTotalHeight		=	m_rcTextDestOriginal.bottom	-	m_rcTextDestOriginal.top;
		int	iTextCount			=	(int)m_vTextBuf.size();
		int	iTotalLineHeight	=	( iTextCount * ( iFontHeight + iLineInterval ) ) - iLineInterval;
		int iTopMargin			=	int( ( iTotalHeight	-	iTotalLineHeight ) / 2 );

		RECT	rcDestText;

		rcDestText		=	m_rcTextDestOriginal;
		rcDestText.top	+=	iTopMargin;

		vector< std::string >::iterator Iter = m_vTextBuf.begin();
		while( Iter != m_vTextBuf.end() )
		{
			m_vTextDest.push_back( rcDestText );
			m_vTextDestOriginal.push_back( rcDestText );

			rcDestText.top += iFontHeight + iLineInterval;

			++Iter;
		}
	}
}

bool SPChatBoard::IsCursorIn( RECT rcBound )
{
	bool	bLeft			=	( m_rcBound.left	<= rcBound.left		&& m_rcBound.right	>= rcBound.left		);
	bool	bTop			=	( m_rcBound.top		<= rcBound.top		&& m_rcBound.bottom	>= rcBound.top		);
	bool	bRight			=	( m_rcBound.left	<= rcBound.right	&& m_rcBound.right	>= rcBound.right	);
	bool	bBottom			=	( m_rcBound.top		<= rcBound.bottom	&& m_rcBound.bottom	>= rcBound.bottom	);

	bool	bLeftTop		=	bLeft	& bTop;
	bool	bLeftBottom		=	bLeft	& bBottom;
	bool	bRightTop		=	bRight	& bTop;
	bool	bRightBottom	=	bRight	& bBottom;

	return bLeftTop | bLeftBottom | bRightTop | bRightBottom;
}

SPGMESSAGE_OBJ_MAP_BEGIN( SPChatBoard )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETSTANDLAYER			,	OnGetStandlayer			)
SPGMESSAGE_OBJ_MAP_END( SPGameObject )

int SPChatBoard::OnGetStandlayer( LPARAM lParam )
{
	return GetStandLayer();
}
