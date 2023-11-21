/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/04/13
	created:	2006:4:13:   17:08
	filename: 	SPWindowWorldMapToolTip.cpp
	
	purpose:	
*********************************************************************/

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPQuestAttr.h"
#include "SPQuestArchive.h"
#include "SPQuestManager.h"

#include "SPEventDEF.h"
#include "SPEventArchive.h"
#include "SPEventManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPResourceManager.h"

#include "SPWindowWorldMap.h"
#include "SPWindowWorldMapToolTip.h"

const int MOUSE_GAP_Y = 20;


SPWindowWorldMapToolTip::SPWindowWorldMapToolTip(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_TOOLTIP, InstanceID, iX, iY, iCX, iCY, pParent)	
{	
	Init();
}

SPWindowWorldMapToolTip::~SPWindowWorldMapToolTip()
{
	Clean();
}

void SPWindowWorldMapToolTip::Init()
{
	m_vBGTexture.reserve( WMTOOLTIP_TEXTURE_COUNT );

	WM_TEXTURE_INFO		stTexture;

	stTexture.m_pTexture		=	NULL;
	stTexture.m_rcPos.left		=	0;
	stTexture.m_rcPos.top		=	0;
	stTexture.m_rcPos.right		=	0;
	stTexture.m_rcPos.bottom	=	0;
	stTexture.m_rcSrc.left		=	0;
	stTexture.m_rcSrc.top		=	0;
	stTexture.m_rcSrc.right		=	0;
	stTexture.m_rcSrc.bottom	=	0;
	stTexture.m_ptSize.x		=	0;
	stTexture.m_ptSize.y		=	0;

	for( int i = 0 ; i < WMTOOLTIP_TEXTURE_COUNT ; ++i )
	{
		m_vBGTexture.push_back( stTexture );
	}

	SPWindowStatic*	pWindowStatic;

	pWindowStatic	=	new	SPWindowStatic( m_InstanceID + WIID_WORLDMAPTOOLTIP_ICON_TITLE_OUTLINE , 11 , 9 , 203 , 35 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/WORLDMAP/TIP01.PNG" , 37 , 0 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new	SPWindowStatic( m_InstanceID + WIID_WORLDMAPTOOLTIP_ICON , 12 , 11 , 32 , 32 , this );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new	SPWindowStatic( m_InstanceID + WIID_WORLDMAPTOOLTIP_NAME_LINE , 47 , 23 , 159 , 2 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/WORLDMAP/TIP01.PNG" , 73 , 33 );
	pWindowStatic	=	NULL;

	m_color14		=	D3DXCOLOR((255.0f / 255.0f), (229.0f / 255.0f), (106.0f / 255.0f), 1.0f);

	pWindowStatic	=	new	SPWindowStatic( m_InstanceID + WIID_WORLDMAPTOOLTIP_NAME , 48 , 10 , 165 , 13 , this );
	pWindowStatic->SetFontColor( m_color14 );
	pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new	SPWindowStatic( m_InstanceID + WIID_WORLDMAPTOOLTIP_TITLE , 49 , 28 , 164 , 13 , this );
	pWindowStatic->SetFontColor( INITCOLOR );
	pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new	SPWindowStatic( m_InstanceID + WIID_WORLDMAPTOOLTIP_DESC , 16 , 58 , 186 , 89 , this );
	pWindowStatic->SetMultiLine( true , 5 );
	pWindowStatic->SetFontColor( INITCOLOR );
	pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new	SPWindowStatic( m_InstanceID + WIID_WORLDMAPTOOLTIP_QUEST_NPC_TITLE , 18 , 166 , 168 , 14 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/WORLDMAP/TIP01.PNG" , 0 , 37 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new	SPWindowStatic( m_InstanceID + WIID_WORLDMAPTOOLTIP_QUEST_NPC_VALUE , 141 , 167 , 21 , 13 , this );
	pWindowStatic->SetFontColor( INITCOLOR );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new	SPWindowStatic( m_InstanceID + WIID_WORLDMAPTOOLTIP_NEAR_PLACE_TITLE , 18 , 198 , 168 , 14 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/WORLDMAP/TIP01.PNG" , 0 , 53 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new	SPWindowStatic( m_InstanceID + WIID_WORLDMAPTOOLTIP_NEAR_PLACE_VALUE , 87 , 198 , 112 , 74 , this );
	pWindowStatic->SetMultiLine( true , 5 );
	pWindowStatic->SetFontColor( INITCOLOR );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic	=	NULL;

	SetBGTexture( "DATA/INTERFACE/WORLDMAP/TIP01.PNG" , 0 , 0 , 36 , 36 , 12 );

}

void SPWindowWorldMapToolTip::Clean()
{
	SPWindow::Clean();

	STD_VECTOR_WM_TEXTURE::iterator	IterTexture = m_vBGTexture.begin();

	while( IterTexture != m_vBGTexture.end() )
	{
		SAFE_RELEASE( (*IterTexture).m_pTexture );
		++IterTexture;
	}

	m_vBGTexture.clear();	
}

void SPWindowWorldMapToolTip::Process(float fTime)
{
	if( m_bShow == false ) return;

	SPWindow::Process(fTime);

	m_bShow	=	g_pMouseCursor->IsShowCursor();
}

void SPWindowWorldMapToolTip::Render(float fTime)
{
	if( m_bShow == false ) return;

	g_pVideo->Flush();

	WM_TEXTURE_INFO*	pWMTexture	=	NULL;

	for( int i = 0 ; i < WMTOOLTIP_TEXTURE_COUNT ; ++i )
	{
		pWMTexture	=	&m_vBGTexture.at( i );

		if( pWMTexture->m_pTexture == NULL )
			continue;

		pWMTexture->m_pTexture->SetColor();
		pWMTexture->m_pTexture->RenderTexture( &pWMTexture->m_rcPos , &pWMTexture->m_rcSrc );
		g_pVideo->Flush();
	}

	SPWindow::Render(fTime);
	
	if( (int)m_strName14.size() > 0 && g_pVideo->GetFont(FONT_14_BOLD))
	{
		RECT	rtDest;

		SPWindow*	pWindow	=	Find( WIID_WORLDMAPTOOLTIP_TITLE , true );

		if( pWindow != NULL )
		{
			pWindow->GetWindowRect( rtDest );

			rtDest.left		-=	1;
			rtDest.right	-=	1;
			rtDest.right	+=	165	-	112;
			rtDest.top		-=	4;
			rtDest.bottom	-=	4;
			rtDest.bottom	+=	15	-	13;

			g_pVideo->GetFont(FONT_14_BOLD)->SetColor( m_color14 );
			g_pVideo->GetFont(FONT_14_BOLD)->SetShadow( false );
			g_pVideo->GetFont(FONT_14_BOLD)->RenderText( m_strName14.c_str() , &rtDest , DT_LEFT | DT_VCENTER );
			g_pVideo->GetFont(FONT_14_BOLD)->SetShadow( true );
			g_pVideo->GetFont(FONT_14_BOLD)->SetColor();
		}
	}
}

SPWindow* SPWindowWorldMapToolTip::Find( INSTANCE_ID iID, bool bChild /* = false  */)
{
	return SPWindow::Find( m_InstanceID + iID , bChild );
}


void SPWindowWorldMapToolTip::SetBGTexture( const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY , int iStep /* = 4  */)
{
	SetBGTexture( WMTOOLTIP_TEXTURE_LEFT_TOP		, strFileName , iSrcSX			, iSrcSY , iSrcSX + iStep	, iSrcSY + iStep );
	SetBGTexture( WMTOOLTIP_TEXTURE_CENTER_TOP		, strFileName , iSrcSX + iStep	, iSrcSY , iSrcEX - iStep	, iSrcSY + iStep );
	SetBGTexture( WMTOOLTIP_TEXTURE_RIGHT_TOP		, strFileName , iSrcEX - iStep	, iSrcSY , iSrcEX			, iSrcSY + iStep );

	SetBGTexture( WMTOOLTIP_TEXTURE_LEFT_MIDDLE		, strFileName , iSrcSX			, iSrcSY + iStep , iSrcSX + iStep	, iSrcSY + iStep * 2);
	SetBGTexture( WMTOOLTIP_TEXTURE_CENTER_MIDDLE	, strFileName , iSrcSX + iStep	, iSrcSY + iStep , iSrcEX - iStep	, iSrcSY + iStep * 2);
	SetBGTexture( WMTOOLTIP_TEXTURE_RIGHT_MIDDLE	, strFileName , iSrcEX - iStep	, iSrcSY + iStep , iSrcEX			, iSrcSY + iStep * 2);

	SetBGTexture( WMTOOLTIP_TEXTURE_LEFT_BOTTOM		, strFileName , iSrcSX			, iSrcEY - iStep , iSrcSX + iStep	, iSrcEY );
	SetBGTexture( WMTOOLTIP_TEXTURE_CENTER_BOTTOM	, strFileName , iSrcSX + iStep	, iSrcEY - iStep , iSrcEX - iStep	, iSrcEY );
	SetBGTexture( WMTOOLTIP_TEXTURE_RIGHT_BOTTOM	, strFileName , iSrcEX - iStep	, iSrcEY - iStep , iSrcEX			, iSrcEY );
}

void SPWindowWorldMapToolTip::SetBGTexture( WMTOOLTIP_TEXTURE eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY )
{
	int	iTextureType	=	(int)eTextureType;

	if( iTextureType >= WMTOOLTIP_TEXTURE_COUNT )
		return;

	SPTexture*		pTexture	=	NULL;

	g_pVideo->GetTextureMgr()->LoadTexture( strFileName , &pTexture );

	WM_TEXTURE_INFO&	stTexture	=	m_vBGTexture.at( iTextureType );

	stTexture.m_rcSrc.left		=	iSrcSX;
	stTexture.m_rcSrc.top		=	iSrcSY;
	stTexture.m_rcSrc.right		=	iSrcEX;
	stTexture.m_rcSrc.bottom	=	iSrcEY;
	stTexture.m_ptSize.x		=	iSrcEX	-	iSrcSX;
	stTexture.m_ptSize.y		=	iSrcEY	-	iSrcSY;

	SAFE_RELEASE( stTexture.m_pTexture );

	stTexture.m_pTexture	=	pTexture;
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowWorldMapToolTip )
	SPIMESSAGE_COMMAND(SPIM_TOOLTIP_ENTER,	OnToolTipEnter	)
	SPIMESSAGE_COMMAND(SPIM_TOOLTIP_OUT,	OnToolTipOut	)
	SPIMESSAGE_COMMAND(SPIM_TOOLTIP_MOVE,	OnToolTipMove	)	
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowWorldMapToolTip::OnToolTipEnter	( WPARAM wparam, LPARAM lparam )
{
	Hide();
	OnToolTipShow( wparam , lparam );
	return 1;
}

int SPWindowWorldMapToolTip::OnToolTipOut	( WPARAM, LPARAM lparam )
{
	if( IsShow() == false )
		return 1;

	Hide();				//	숨기기
	return 1;
}

int SPWindowWorldMapToolTip::OnToolTipMove	( WPARAM wparam, LPARAM lparam )
{
	UpdatePos();
	UpdateBGPos();
	return 1;
}

void SPWindowWorldMapToolTip::UpdatePos()
{
	SIZE ptMouseSize	= g_pMouseCursor->GetMouseSize();
	int iXPos			= g_pInterfaceManager->GetCursorX();
	int iYPos			= g_pInterfaceManager->GetCursorY() + ptMouseSize.cy;
	int	iXScreenSize	= (int)g_pVideo->GetScreenLenX();
	int iYScreenSize	= (int)g_pVideo->GetScreenLenY();


	//  마우스 X 위치가 화면 오른쪽으로 툴팁이 잘릴경우  [6/30/2006]

	if( iXPos + m_iSX >= iXScreenSize )
	{
		iXPos	=	iXPos - m_iSX;
	}

	//	마우스 Y 위치가 화면 아래쪽으로 툴팁이 잘릴경우  [6/30/2006]

	if( iYPos + ptMouseSize.cy + m_iSY >= iYScreenSize )
	{
		iYPos	=	iYPos - ptMouseSize.cy - m_iSY;
	}

	if( iXPos < 0 )
		iXPos	=	0;

	if( iYPos < 0 )
		iYPos	=	0;

	AdjustCoord( iXPos , iYPos );
	RefreshChildPos();
}

void SPWindowWorldMapToolTip::UpdateBGPos()
{
	int		iLeft	=	m_iAX;
	int		iTop	=	m_iAY;
	int		iRight	=	m_iAX + m_iSX;
	int		iBottom	=	m_iAY + m_iSY;

	WM_TEXTURE_INFO*	pBGInfo	=	NULL;

	//////////////////////////////////////////////////////////////////////////
	//
	//	탑라인
	//

	pBGInfo	=	&m_vBGTexture.at( WMTOOLTIP_TEXTURE_LEFT_TOP );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left	+	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	pBGInfo->m_rcPos.top	+	pBGInfo->m_ptSize.y;

	int		iMiddleSX	=	pBGInfo->m_rcPos.right;
	int		iMiddleSY	=	pBGInfo->m_rcPos.bottom;

	pBGInfo	=	&m_vBGTexture.at( WMTOOLTIP_TEXTURE_RIGHT_TOP );

	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.left	=	pBGInfo->m_rcPos.right	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	pBGInfo->m_rcPos.top	+	pBGInfo->m_ptSize.y;

	int		iMiddleEX	=	pBGInfo->m_rcPos.left;

	pBGInfo	=	&m_vBGTexture.at( WMTOOLTIP_TEXTURE_CENTER_TOP );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iMiddleSY;

	//////////////////////////////////////////////////////////////////////////
	//
	//	BOTTOM라인

	pBGInfo	=	&m_vBGTexture.at( WMTOOLTIP_TEXTURE_LEFT_BOTTOM );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.bottom	=	iBottom;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left	+	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	pBGInfo->m_rcPos.bottom	-	pBGInfo->m_ptSize.y;

	pBGInfo	=	&m_vBGTexture.at( WMTOOLTIP_TEXTURE_RIGHT_BOTTOM );

	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.bottom	=	iBottom;
	pBGInfo->m_rcPos.left	=	pBGInfo->m_rcPos.right	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	pBGInfo->m_rcPos.bottom	-	pBGInfo->m_ptSize.y;

	int		iMiddleEY	=	pBGInfo->m_rcPos.top;

	pBGInfo	=	&m_vBGTexture.at( WMTOOLTIP_TEXTURE_CENTER_BOTTOM );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iMiddleEY;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iBottom;

	//////////////////////////////////////////////////////////////////////////
	//
	//	가운데라인

	pBGInfo	=	&m_vBGTexture.at( WMTOOLTIP_TEXTURE_LEFT_MIDDLE );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left + pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;

	pBGInfo	=	&m_vBGTexture.at( WMTOOLTIP_TEXTURE_CENTER_MIDDLE );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;

	pBGInfo	=	&m_vBGTexture.at( WMTOOLTIP_TEXTURE_RIGHT_MIDDLE );

	pBGInfo->m_rcPos.left	=	iRight	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;
}

void SPWindowWorldMapToolTip::CalEndPos()
{
	SPWindow*		pWindow		=	Find( WIID_WORLDMAPTOOLTIP_DESC , true );

	if( pWindow == NULL )
		return;

	RECT	rtLastWindow;

	int			iPosX , iPosY;
	int			iLastPosY;
	int			iStep	=	19;
	int			iCount	=	WIID_WORLDMAPTOOLTIP_CAL_END	-	WIID_WORLDMAPTOOLTIP_CAL_START;
	INSTANCE_ID	InstanceID;

	pWindow->GetWindowRect( rtLastWindow );

	if( pWindow->IsShow() == false )
		rtLastWindow.bottom	=	rtLastWindow.top - iStep;

	for( int i = 0 ; i < iCount ; ++i )
	{
		InstanceID = WIID_WORLDMAPTOOLTIP_CAL_START + i;
		pWindow	=	Find( InstanceID , true );

		if( pWindow == NULL )
			continue;

		if( pWindow->IsShow() == true )
		{
			if( InstanceID != WIID_WORLDMAPTOOLTIP_QUEST_NPC_VALUE && InstanceID != WIID_WORLDMAPTOOLTIP_NEAR_PLACE_VALUE )
				iLastPosY	=	rtLastWindow.bottom	+	iStep;

			pWindow->GetAbsCoord( iPosX , iPosY );
			pWindow->AdjustCoord( iPosX - m_iAX , iLastPosY - m_iAY );

			if( InstanceID != WIID_WORLDMAPTOOLTIP_QUEST_NPC_VALUE && InstanceID != WIID_WORLDMAPTOOLTIP_NEAR_PLACE_VALUE )
				pWindow->GetWindowRect( rtLastWindow );
		}
	}

	for( i = WIID_WORLDMAPTOOLTIP_CAL_END ; i > WIID_WORLDMAPTOOLTIP_CAL_START ; --i )
	{
		pWindow	=	Find( i , true );

		if( pWindow == NULL )
			continue;

		if( pWindow->IsShow() == false )
			continue;

		pWindow->GetWindowRect( rtLastWindow );
		break;
	}

	m_iSY	=	rtLastWindow.bottom + iStep	-	m_iAY;
}

void SPWindowWorldMapToolTip::OnToolTipShow( WPARAM wParam , LPARAM lParam )
{
	WMTOOLTIP_SHOW_TYPE	eShowType	=	(WMTOOLTIP_SHOW_TYPE)wParam;

	if( eShowType == TOOLTIP_SHOW_TYPE_NULL )
		return;

	m_bShow	=	g_pMouseCursor->IsShowCursor();

	if( m_bShow == false )
		return;

	UpdatePos();

	switch( eShowType)
	{
	case TOOLTIP_SHOW_TYPE_WORLD:
		OnWorldShow( (SPWorldMapStageAttr*)lParam );
		break;
	case TOOLTIP_SHOW_TYPE_STAGE:
		OnStageShow( (SPStageToolTipData*)lParam );
		break;
	}


	CalEndPos();
	UpdateBGPos();
}

void SPWindowWorldMapToolTip::OnWorldShow( SPWorldMapStageAttr* pStageAttr )
{
	if( pStageAttr == NULL )
		return;

	m_bShow	=	true;
	SPWindow*	pWindow;

	SetIcon( pStageAttr->m_iMapIcon , pStageAttr->m_iMapIconIndex );

	pWindow = Find( WIID_WORLDMAPTOOLTIP_TITLE , true );

	if( pWindow != NULL && (int)pStageAttr->m_strName.size() > 0 )
	{
		m_strName14	=	pStageAttr->m_strName.c_str();
//		pWindow->SetWindowText( pStageAttr->m_strName.c_str() );
//		pWindow->Show();
	}

	CheckQuestNPC( pStageAttr->m_iStage );

	SetWindowMultiText( WIID_WORLDMAPTOOLTIP_DESC , pStageAttr->m_strDesc.c_str() );

	std::string	strNearPlace;
	strNearPlace.clear();

	for( int i = 0 ; i < WORLDMAP_NEARMAPINFO ; ++i )
	{
		if( (int)pStageAttr->m_strNearMapInfo[ i ].size() > 0 )
		{
			if( (int)strNearPlace.size() > 0 )
			{
				strNearPlace	+=	"\n";
			}

			strNearPlace	+=	pStageAttr->m_strNearMapInfo[ i ];
		}
	}

	SetWindowMultiText( WIID_WORLDMAPTOOLTIP_NEAR_PLACE_VALUE , strNearPlace.c_str() );
	if( (int)strNearPlace.size() > 0 )
	{
		pWindow	=	Find( WIID_WORLDMAPTOOLTIP_NEAR_PLACE_TITLE , true );

		if( pWindow != NULL )
			pWindow->Show();
	}
}

void SPWindowWorldMapToolTip::CheckQuestNPC( int iStage )
{
	SPQuestManager* pQuestManager = SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return;
    
	SPQuestArchive*	pQuestArchive = pQuestManager->GetPlayerArchive();

	if( pQuestArchive == NULL )
		return;

	SPEventArchive*	pEventArchive	=	g_pEventManager->GetEventArchive();

	if( pEventArchive == NULL )
		return;

	std::vector<MAPEVENT_INFO*>	vMapEventInfo;

	if( pEventArchive->FindEvent( &vMapEventInfo , iStage ) == false )
		return;

	std::vector<MAPEVENT_INFO*>::iterator	Iter	=	vMapEventInfo.begin();
	MAPEVENT_INFO*	pMapEventInfo;
	int	iNPCCount	=	0;
	int	iQuest		=	0;
	int	iQuestCount	=	pQuestArchive->GetSize();
	SPQuestAttr*	pQuestAttr		=	NULL;

	for( iQuest = 0 ; iQuest < iQuestCount ; ++iQuest )
	{
		pQuestAttr	=	pQuestArchive->GetQuestAttr( iQuest );

		if( pQuestAttr == NULL )
			continue;

		Iter	=	vMapEventInfo.begin();

		while( Iter != vMapEventInfo.end() )
		{
			pMapEventInfo	=	(*Iter);
			++Iter;

			if( pMapEventInfo == NULL )
				continue;

			if( pMapEventInfo->eventType != EVENT_NPC )
				continue;

			if( pQuestAttr->m_iUiShow == QUEST_UI_SHOW_COMPLETE )
			{
				if( pMapEventInfo->eventTypeID == pQuestAttr->m_iRewordNpcID )
				{
					++iNPCCount;
					break;
				}
			}
		}
	}

	if( iNPCCount <= 0 )
		return;

	char	szBuf[_MAX_PATH];

	sprintf( szBuf , "%d" , iNPCCount );

	SPWindow* pWindow;

	pWindow = Find( WIID_WORLDMAPTOOLTIP_QUEST_NPC_TITLE , true );
	if( pWindow != NULL )
		pWindow->Show();

	pWindow = Find( WIID_WORLDMAPTOOLTIP_QUEST_NPC_VALUE , true );
	if( pWindow != NULL )
	{
		pWindow->SetWindowText( szBuf );
		pWindow->Show();
	}
}

void SPWindowWorldMapToolTip::OnStageShow( SPStageToolTipData* pStageData )
{
	if( pStageData == NULL )
		return;

	m_bShow	=	true;
	m_strName14.clear();

	SPWindow*	pWindow;

	if( pStageData->m_iResID != 0 )
	{
		SetIcon( pStageData->m_iResID , pStageData->m_iResIndex );
	}
	else
	{
		pWindow = Find( WIID_WORLDMAPTOOLTIP_ICON , true );
		SPWindowStatic*	pWindowStatic	=	(SPWindowStatic*)pWindow;

		if( pWindowStatic != NULL )
		{
			int	iXPos		=	( pStageData->m_uiClassType - 1 ) % 4 * 33;
			int	iYPos		=	( pStageData->m_uiClassType - 1 ) / 4 * 33 + 69;

			pWindowStatic->Clean();
			pWindowStatic->SetImage( "DATA/INTERFACE/WORLDMAP/TIP01.PNG" , iXPos , iYPos );
			pWindowStatic->Show();

			pWindow	=	Find( WIID_WORLDMAPTOOLTIP_ICON_TITLE_OUTLINE , true );
			if( pWindow != NULL )
				pWindow->Show();
		}
	}

	pWindow = Find( WIID_WORLDMAPTOOLTIP_NAME , true );

	if( pWindow != NULL )
	{
		pWindow->SetWindowText( pStageData->m_strName.c_str() );
		pWindow->Show();

		pWindow = Find( WIID_WORLDMAPTOOLTIP_NAME_LINE , true );

		if( pWindow != NULL )
			pWindow->Show();
	}

	pWindow = Find( WIID_WORLDMAPTOOLTIP_TITLE , true );

	if( pWindow != NULL )
	{
		pWindow->SetWindowText( pStageData->m_strPos.c_str() );
		pWindow->Show();
	}

	if( (int)pStageData->m_strDesc.size() > 0 )
	{
		SetWindowMultiText( WIID_WORLDMAPTOOLTIP_DESC , pStageData->m_strDesc.c_str() );
	}
}

void SPWindowWorldMapToolTip::SetIcon( int iResID , int iResIndex )
{
	if( iResID <= 0 || iResIndex <= 0 )
		return;

	const char*	szFileName = g_pResourceManager->GetGlobalFileName( iResID );

	if( szFileName == NULL )
		return;

	std::string strResourceFile = szFileName;

	if( strResourceFile.empty() )
		return;

	SPTexture*	pThemeIcon	=	NULL;
	std::string strFullRes; //	=	RES_WORK_RESOURCE_PATH;
	strFullRes	=	strResourceFile;

	g_pVideo->GetTextureMgr()->LoadTexture( strFullRes.c_str() , &pThemeIcon );

	if( pThemeIcon == NULL )
		return;

	//	시작은 0이 아닌 1부터 시작하기에 1을 빼준다!!!
	iResIndex--;

	int iIconSizeX = 32;
	int	iIconSizeY = 32;

	int iIconWidth	= pThemeIcon->GetLenX() / iIconSizeX;
	int iIconYIndex = iResIndex / iIconWidth;
	int iIconXIndex = iResIndex - iIconYIndex * iIconWidth;

	SPWindow*	pWindow = Find( WIID_WORLDMAPTOOLTIP_ICON , true );

	if( pWindow != NULL )
	{
		pWindow->Clean();
		pWindow->SetImage( pThemeIcon , iIconXIndex * iIconSizeX , iIconYIndex * iIconSizeY );
		pWindow->Show();

		pWindow	=	Find( WIID_WORLDMAPTOOLTIP_ICON_TITLE_OUTLINE , true );
		if( pWindow != NULL )
			pWindow->Show();
	}
}

void SPWindowWorldMapToolTip::SetWindowMultiText( INSTANCE_ID WindowID , const char* pstrString )
{
	SPWindow*		pWindow		=	Find( WindowID , true );
	SPWindowStatic*	pWindowDesc	=	(SPWindowStatic*)pWindow;

	if( pWindowDesc == NULL )
		return;

	RECT	rtDesc;

	pWindowDesc->GetWindowRect( rtDesc );
	pWindowDesc->SetWindowText( pstrString );

	int iDescWidth	=	rtDesc.right - rtDesc.left;
	int	iDescHeight	=	pWindowDesc->GetTextPixelHeight();

	pWindowDesc->AdjustCoord( rtDesc.left - m_iAX , rtDesc.top - m_iAY , iDescWidth , iDescHeight );

	if( pstrString != NULL && strlen( pstrString ) > 0 )
		pWindowDesc->Show();
}
