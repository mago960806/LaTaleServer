/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/06/23
	created:	2006:6:23:   16:08
	filename: 	SPWindowQuestView.cpp
	
	purpose:	
*********************************************************************/

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowSlider.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"
#include "SPItemCluster.h"

#include "SPCheckManager.h"

#include "SPSkill.h"
#include "SPMotionStatus.h"
#include "SPSkillManager.h"

#include "SPEventDEF.h"
#include "SPEventArchive.h"
#include "SPEventManager.h"

#include "SPQuestManager.h"
#include "SPQuestArchive.h"
#include "SPQuestAttr.h"

#include "SPRewardArchive.h"
#include "SPRewardAttr.h"

#include "SPWindowToolTip.h"

#include "SPUIUnit.h"
#include "SPUIItemUnit.h"
#include "SPUIGOBUnit.h"
#include "SPUIUnitManager.h"

#include "PacketID.h"
#include "SPUtil.h"

#include "SPWindowQuestView.h"

//////////////////////////////////////////////////////////////////////////

#define QUESTVIEW_UNIT_X_COUNT						6
#define QUESTVIEW_UNIT_Y_COUNT						1

#define QUESTVIEW_UNIT_X_SIZE						38
#define QUESTVIEW_UNIT_Y_SIZE						32

#define QUESTVIEW_UNIT_X_OFFSET						0
#define QUESTVIEW_UNIT_Y_OFFSET						0

#define QUESTVIEW_TALK_TO_PURPOSE_TITLE				13
#define QUESTVIEW_PURPOSE_TITLE_TO_PURPOSE_LINE1	8
#define QUESTVIEW_PURPOSE_SHOW_LINE_TO_REWARD		6
#define QUESTVIEW_REWARD_TO_ITEM_ICON				10

#define QUESTVIEW_BRACE_X_SIZE						4
#define QUESTVIEW_BRACE_Y_SIZE						12

//////////////////////////////////////////////////////////////////////////

SPWindowQuestView::SPWindowQuestView(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowQuestView::SPWindowQuestView(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_QUESTVIEW, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowQuestView::~SPWindowQuestView()
{
	Clean();
}

void SPWindowQuestView::Init()
{
	m_strToolTip	= "[SPWindowQuestView]";

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" , &m_pBGTexture );
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , &m_pSubBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[0]		, 125 , 339 , 422 , 343 );		//	배경 상단
	SetRect( &m_rcBGSrc[1]		, 125 , 345 , 422 , 347 );		//	배경 중단
	SetRect( &m_rcBGSrc[2]		, 125 , 349 , 422 , 353 );		//	배경 하단
	
	SetRect( &m_rcSubBGSrc[0]	, 349 , 321 , 355 , 327 );		//	흰색 서브 백그라운드
	SetRect( &m_rcSubBGSrc[1]	, 349 , 329 , 355 , 330 );		//	경계선 1
	SetRect( &m_rcSubBGSrc[2]	, 202 ,  76 , 204 ,  92 );		//	랭크 페이지 백그라운드 왼쪽
	SetRect( &m_rcSubBGSrc[3]	, 206 ,  76 , 208 ,  92 );		//	랭크 페이지 백그라운드 가운데
	SetRect( &m_rcSubBGSrc[4]	, 210 ,  76 , 212 ,  92 );		//	랭크 페이지 백그라운드 오른쪽

	SPWindowButton*	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_QUESTVIEW_CLOSE , 276 , 9 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 217 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 230 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 244 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 256 );
	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_QUESTVIEW_TAB_TALK , 16 + 2 , 27 + 43 , 64 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 131 , 448 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 131 , 464 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 131 , 480 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 131 , 480 );
	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_QUESTVIEW_TAB_SUMMARY , 84 + 2 , 27 + 43 , 64 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 279 , 448 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 279 , 464 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 279 , 480 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 279 , 480 );
	pWindowButton	=	NULL;

	SPWindowStatic*	pWindowStatic;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_TITLE_TEXT , 10 + 2 , 7 , 239 , 15 , this );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_NPC_TEXT , 57 + 2 , 27 + 18 , 232 , 12 , this );
	pWindowStatic->SetFont( FONT_12_BOLD );
	pWindowStatic->SetFormat( DT_LEFT | DT_TOP );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 ,  70 , 140 , 124 ) );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_NPC_ICON , 16 + 2 , 27 + 7 , 32 , 32 , this );
	pWindowStatic	=	NULL;

	m_cQuestNameColor		=	D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );
	m_cQuestNameShadowColor	=	D3DCOLOR_ARGB( 255 ,  70 , 140 , 124 );

	m_iPlayerPosX	=	-1;
	m_iPlayerPosY	=	-1;

	InitItemUnit();

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_TALK , 16 + 2 , 27 + 62 , 262 , 100 , this );
	pWindowStatic->SetMultiLine( true , 2 );
	pWindowStatic->SetFormat( DT_LEFT | DT_TOP | DT_WORDBREAK );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_BRACE , 0 , 0 , m_iSX , 100 , this );
	InitBrace( pWindowStatic );
	pWindowStatic	=	NULL;

	InitPurpose( this );

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_REWARD , 0 , 0 , 293 , 109 , this );
	InitReward( pWindowStatic );
	pWindowStatic	=	NULL;

	m_iRewardRank		=	0;
	m_iRewardInfo		=	0;
	m_iRewardInfoMax	=	0;

	const char*	pszMission	=	g_pResourceManager->GetGlobalString( 6001007 );

	if( pszMission == NULL )
		m_strMissionView	=	"[ ????? ]";
	else
		m_strMissionView	=	pszMission;
}

void SPWindowQuestView::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
	SAFE_RELEASE( m_pSubBGTexture );

	m_UIUnitManager.ClearAll( TRUE );
}

void SPWindowQuestView::Show( bool bWithChild /* = true  */ )
{
	SPWindow::Show( bWithChild );

	if( m_iPlayerPosX != -1 && m_iPlayerPosY != -1 )
	{
		SPWindow*	pCloseWIndow	=	Find( WIID_QUESTVIEW_CLOSE );

		if( pCloseWIndow != NULL )
			pCloseWIndow->Close();
	}

	UpdatePosWindow();
}

void SPWindowQuestView::Hide(bool bSendServer)
{
	if( m_bShow )
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
		OnCursorOut( WIID_QUESTVIEW_REWARD_ITEM_ICON , NULL ,NULL );

	//	AJJIYA [9/8/2005]
	//	창 정렬을 위해서!!
	g_pInterfaceManager->OrderWindowPosMove();
}

void SPWindowQuestView::Process(float fTime)
{
	if( m_bShow == false ) return;

	SPWindow::Process( fTime );

	m_UIUnitManager.Process( fTime );
}

void SPWindowQuestView::Render(float fTime)
{
	if( m_bShow == false ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < QUESTVIEW_BG_COUNT ; ++i )
		{
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	SPWindow*	pWindow;

	if( m_pSubBGTexture )
	{
		pWindow	=	Find( WIID_QUESTVIEW_REWARD );

		for( int i = 0 ; i < QUESTVIEW_SUB_BG_COUNT ; ++i )
		{
			if( i >= 2 )
			{
				if( pWindow != NULL )
				{
					if( pWindow->IsShow() == false )
						continue;
				}
			}

			m_pSubBGTexture->RenderTexture( &m_rcSubBGDest[i] , &m_rcSubBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	SPWindow::Render(fTime);

	SPFont*	pFont	=	g_pVideo->GetFont( FONT_12_BOLD );

	if( pFont == NULL )
		return;

	pWindow	=	Find( WIID_QUESTVIEW_TITLE_TEXT );

	if( pWindow != NULL )
	{
		RECT	rcDest;
		pWindow->GetWindowRect( rcDest );

		if( m_strQuestName.empty() == false )
		{
			RECT	rcDestShadow;

			pFont->SetShadow( false );

			rcDestShadow	=	rcDest;
			OffsetRect( &rcDestShadow , 0 , 1 );
			pFont->SetColor( m_cQuestNameShadowColor );
			pFont->RenderText( m_strQuestName.c_str() , &rcDestShadow );

			rcDestShadow	=	rcDest;
			OffsetRect( &rcDestShadow , 1 , 0 );
			pFont->SetColor( m_cQuestNameShadowColor );
			pFont->RenderText( m_strQuestName.c_str() , &rcDestShadow );

			rcDestShadow	=	rcDest;
			OffsetRect( &rcDestShadow , 0 , -1 );
			pFont->SetColor( m_cQuestNameShadowColor );
			pFont->RenderText( m_strQuestName.c_str() , &rcDestShadow );

			rcDestShadow	=	rcDest;
			OffsetRect( &rcDestShadow , -1 , 0 );
			pFont->SetColor( m_cQuestNameShadowColor );
			pFont->RenderText( m_strQuestName.c_str() , &rcDestShadow );

			pFont->SetColor( m_cQuestNameColor );
			pFont->RenderText( m_strQuestName.c_str() , &rcDest );
			pFont->SetColor();

			pFont->SetShadow( true );
		}
	}

	g_pVideo->Flush();

	m_UIUnitManager.Render();

	g_pVideo->Flush();
}

void SPWindowQuestView::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
	UpdatePosWindow();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowQuestView )

	SPIMESSAGE_COMMAND(	SPIM_MOVE_BEGIN			,										OnBeginMove			)	// metalgeni [6/2/2006]
	SPIMESSAGE_COMMAND(	SPIM_MOVE_END			,										OnEndMove			)	// Related to window move
	SPIMESSAGE_COMMAND(	SPIM_MOVE				,										OnMove				)

	SPIMESSAGE_COMMAND(	SPIM_PURGE				,										OnPurge				)
	SPIMESSAGE_COMMAND(	SPIM_QUEST_REFRESH		,										OnRefreshQuest		)

	SPIMESSAGE_COMMAND(	SPIM_QUEST_DETAIL		,										OnQuestDetail		)
	SPIMESSAGE_COMMAND(	SPIM_QUEST_GET_REWARD	,										OnQuestGetReward	)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			,	WIID_QUESTVIEW_CLOSE			,	OnCloseLButtonUp	)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			,	WIID_QUESTVIEW_TAB_TALK			,	OnTalkLButtonUp		)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			,	WIID_QUESTVIEW_TAB_SUMMARY		,	OnSummaryLButtonUp	)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			,	WIID_QUESTVIEW_REWARD_LEFT		,	OnRewardLeftLButtonUp	)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			,	WIID_QUESTVIEW_REWARD_RIGHT		,	OnRewardRightLButtonUp	)

	SPIMESSAGE_CONTROL_RANGE(	SPIM_LBUTTON_UP	,	WIID_QUESTVIEW_REWARD_RANK_A	,	WIID_QUESTVIEW_REWARD_RANK_C	,	OnRankLButtonUp		)

	SPIMESSAGE_CONTROL_RANGE(	SPIM_CURSOR_ENTER,	WIID_QUESTVIEW_REWARD_ITEM_ICON	,	WIID_QUESTVIEW_REWARD_ITEM_ICON + QUESTVIEW_UNIT_X_COUNT * QUESTVIEW_UNIT_Y_COUNT	,	OnCursorEnter		)
	SPIMESSAGE_CONTROL_RANGE(	SPIM_CURSOR_OUT	,	WIID_QUESTVIEW_REWARD_ITEM_ICON	,	WIID_QUESTVIEW_REWARD_ITEM_ICON	+ QUESTVIEW_UNIT_X_COUNT * QUESTVIEW_UNIT_Y_COUNT	,	OnCursorOut			)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

//////////////////////////////////////////////////////////////////////////

int SPWindowQuestView::OnBeginMove		( WPARAM wParam, LPARAM lParam)
{
	return SPWindow::OnBeginMove(wParam, lParam);
}

int SPWindowQuestView::OnEndMove		( WPARAM wParam, LPARAM lParam)
{
	return SPWindow::OnEndMove(wParam, lParam);
}

int SPWindowQuestView::OnMove			( WPARAM wParam, LPARAM lParam)
{
	return SPWindow::OnMove(wParam, lParam);
}

int SPWindowQuestView::OnPurge( WPARAM wParam, LPARAM lParam )
{
	Hide();

	return 1;
}

int SPWindowQuestView::OnRefreshQuest(WPARAM wParam, LPARAM lParam )
{
	UpdateMission();

	return 1;
}

int SPWindowQuestView::OnQuestDetail( WPARAM wParam, LPARAM lParam )
{
	m_eQuestInstanceID	=	(INSTANCE_ID)wParam;
	m_iQuestID			=	(int)lParam;
	CUT_IN_STATE	eCutInState	=	g_pEventManager->GetCutInState();

	if( m_eQuestInstanceID == WIID_NPC_QUEST )
	{
		if( eCutInState == CUT_IN_STATE_QUEST )
		{
			if( m_iPlayerPosX == -1 && m_iPlayerPosY == -1 )
			{
				m_iPlayerPosX	=	m_iAX;
				m_iPlayerPosY	=	m_iAY;
			}

			AdjustCoord( 259 , 64 );
			SPWindow::RefreshRelationCoord();
			ReposOwnImage();

			SetMoveable( false );
		}
	}

	if ( eCutInState == CUT_IN_STATE_NULL )
	{
	//	else if( m_eQuestInstanceID == WIID_QUEST )
		{
			if( m_iPlayerPosX != -1 && m_iPlayerPosY != -1 )
			{
				AdjustCoord( m_iPlayerPosX , m_iPlayerPosY );
				SPWindow::RefreshRelationCoord();
				ReposOwnImage();

				SetMoveable( true );
			}

			m_iPlayerPosX	=	-1;
			m_iPlayerPosY	=	-1;
		}
	}

	SPQuestAttr*	pQuestAttr	=	GetQuestAttr( m_iQuestID , m_eQuestInstanceID );

	if( pQuestAttr == NULL )
		return 1;

	SPUIUnit	UIUnit;
	char		szBuf[ _MAX_PATH ];
	SPWindow*	pWindow;

	SPQuestNpc*	pQuestNpcGive	=	&pQuestAttr->m_QuestNpc[ QUEST_NPC_GIVE ];
	SPQuestNpc*	pQuestNpcReword	=	&pQuestAttr->m_QuestNpc[ QUEST_NPC_REWORD ];

	//	퀘스트 이름
	m_strQuestName	=	pQuestAttr->m_strQuestName;

	sprintf( szBuf , "%s (%s)" , pQuestNpcGive->m_strName.c_str() , pQuestNpcGive->m_strPlace.c_str() );

	//	NPC 이름과 마을
	pWindow	=	Find( WIID_QUESTVIEW_NPC_TEXT );

	if( pWindow != NULL )
	{
		pWindow->SetWindowText( szBuf );
	}
//	m_strNpcName	=	szBuf;

	//	주는 NPC 얼굴
	pWindow	=	Find( WIID_QUESTVIEW_NPC_ICON );

	if( pWindow != NULL )
	{
		UIUnit.Init();
		UIUnit.SetResource( pQuestNpcGive->m_iIcon , pQuestNpcGive->m_iIconIndex );

		RECT	rcSrc	=	UIUnit.GetRectSrc();

		pWindow->SPSendMessage( SPIM_SET_IMAGECLEAR );

		if( UIUnit.GetTexture() != NULL )
			pWindow->SetImage( UIUnit.GetTexture()->GetResName() , rcSrc.left , rcSrc.top );

		UIUnit.Clear();
	}

	//	대화
	UpdateTalk();

	//	목적
	UpdateMission();

	//	랭크 갯수를 파악한다.
	UpdateRankCount();

	//	지금 받을수 있는 랭크를 지정하고 그것이 유효하지 않다면 보정한다.
	DefaultRewardRank();

	UpdateRank();

	UpdateReward();

	//	위치 정보 계산은 Show에서만!!!!
	Show();

	return 1;
}

int SPWindowQuestView::OnQuestGetReward( WPARAM wParam, LPARAM lParam )
{
	return m_iRewardInfo;
}

int SPWindowQuestView::OnCloseLButtonUp( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

int SPWindowQuestView::OnTalkLButtonUp( WPARAM wParam, LPARAM lParam )
{
	UpdateTalk();

	UpdatePosWindow();

	return 1;
}

int SPWindowQuestView::OnSummaryLButtonUp( WPARAM wParam, LPARAM lParam )
{
	UpdateSummary();

	UpdatePosWindow();

	return 1;
}

int SPWindowQuestView::OnRewardLeftLButtonUp( WPARAM wParam, LPARAM lParam )
{
	UpdateRewardPage();

	--m_iRewardInfo;

	if( m_iRewardInfo < 0 )
		m_iRewardInfo	=	m_iRewardInfoMax - 1;

	UpdateReward();

	return 1;
}

int SPWindowQuestView::OnRewardRightLButtonUp( WPARAM wParam, LPARAM lParam )
{
	UpdateRewardPage();

	++m_iRewardInfo;

	if( m_iRewardInfo >= m_iRewardInfoMax )
		m_iRewardInfo	=	0;

	UpdateReward();

	return 1;
}

int SPWindowQuestView::OnRankLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	m_iRewardRank	=	iID - WIID_QUESTVIEW_REWARD_RANK_A;
	m_iRewardInfo	=	0;

	UpdateRank();
	UpdateReward();
	UpdatePosWindow();
	return 1;
}

int SPWindowQuestView::OnCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	int				iXPos			=	LOWORD(lParam);
	int				iYPos			=	HIWORD(lParam);
	SPUIUnit*		pUIUnit			=	m_UIUnitManager.GetUnitMouseOver( iXPos , iYPos );
	SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();

	if( pUIUnit != NULL )
	{
		if( pUIUnit->GetTexture() != NULL )
			pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_ENTER , (WPARAM)pUIUnit , SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );
	}

	return 1;
}

int SPWindowQuestView::OnCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow*	pWindow =	g_pInterfaceManager->GetToolTipWindow();

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );

	return 1;
}

//////////////////////////////////////////////////////////////////////////

void SPWindowQuestView::ReposOwnImage()
{
	SetRect( &m_rcBGDest[0]		, m_iAX					, m_iAY					, m_iAX + m_iSX			, m_iAY + 4 );							//	배경 상단
	SetRect( &m_rcBGDest[1]		, m_rcBGDest[0].left	, m_rcBGDest[0].bottom	, m_rcBGDest[0].right	, m_rcBGDest[0].bottom + m_iSY - 8 );	//	배경 중단
	SetRect( &m_rcBGDest[2]		, m_rcBGDest[1].left	, m_rcBGDest[1].bottom	, m_rcBGDest[1].right	, m_rcBGDest[1].bottom + 4 );			//	배경 하단

}

void SPWindowQuestView::InitItemUnit()
{
	char			szResPath[]		=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
	int				iXPos , iYPos;
	SPUIItemUnit*	pUIItemUnit		=	NULL;

	m_UIUnitManager.SetBufferSize( QUESTVIEW_UNIT_X_COUNT * QUESTVIEW_UNIT_Y_COUNT );

	for( int y = 0 ; y < QUESTVIEW_UNIT_Y_COUNT ; y++ )
	{
		for( int x = 0 ; x < QUESTVIEW_UNIT_X_COUNT ; x++ )
		{
			iXPos	=	m_iAX + QUESTVIEW_UNIT_X_OFFSET + ( x * QUESTVIEW_UNIT_X_SIZE );
			iYPos	=	m_iAY + QUESTVIEW_UNIT_Y_OFFSET + ( y * QUESTVIEW_UNIT_Y_SIZE );

			pUIItemUnit		=	new SPUIItemUnit;

			pUIItemUnit->SetBGTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" );
			pUIItemUnit->SetBGRectSrc( 358 , 382 , 390 , 414 );
			pUIItemUnit->SetPos( iXPos , iYPos );
			pUIItemUnit->SetDuration( TRUE );
			pUIItemUnit->SetInstanceID( WIID_ITEM );

			m_UIUnitManager.AddUnit( pUIItemUnit );
		}
	}
}

void SPWindowQuestView::InitBrace( SPWindow* pWindow )
{
	if( pWindow == NULL )
		return;

	SPWindowStatic*	pWindowStatic;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_BRACE_LEFT_TOP , 4 + 2 , 0 , 4 , 14 , pWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 120 , 130 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_BRACE_LEFT_MIDDLE , 4 + 2 , 0 , 4 , 14 , pWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 120 , 146 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_BRACE_LEFT_BOTTOM , 4 + 2 , 0 , 4 , 14 , pWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 120 , 160 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_BRACE_RIGHT_TOP , 285 + 2 , 0 , 4 , 14 , pWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 125 , 130 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_BRACE_RIGHT_MIDDLE , 285 + 2 , 0 , 4 , 14 , pWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 125 , 146 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_BRACE_RIGHT_BOTTOM , 285 + 2 , 0 , 4 , 14 , pWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 125 , 160 );
	pWindowStatic	=	NULL;
}

void SPWindowQuestView::InitPurpose( SPWindow* pWindow )
{
	if( pWindow == NULL )
		return;

	SPWindowStatic*	pWindowStatic;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_PURPOSE_TITLE_IMG , 17 + 2 , 0 , 18 , 18 , pWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 115 , 76 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 134 , 76 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_PURPOSE_TITLE_TEXT , 42 + 2 , 2 , 23 , 11 , pWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 49 , 163 );
	pWindowStatic	=	NULL;

	m_cPurposeColor[ 0 ]	=	D3DCOLOR_ARGB( 255 , 49 , 76 , 97 );
	m_cPurposeColor[ 1 ]	=	D3DCOLOR_ARGB( 255 , 181 , 181 , 181 );

	for( int i = 0 ; i < 4 ; ++i )
	{
		pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_PURPOSE_LINE_1_IMG + i , 29 + 2 , 1 , 14 , 12 , pWindow );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 172 , 76 );	//	수행중
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 187 , 76 );	//	수행완료
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_PURPOSE_LINE_1_TEXT + i , 47 + 2 , 1 , 242 , 12 , pWindow );
		pWindowStatic->SetFormat( DT_LEFT | DT_TOP );
		pWindowStatic->SetFontColor( m_cPurposeColor[ 0 ] );
		pWindowStatic	=	NULL;
	}
}

void SPWindowQuestView::InitReward( SPWindow* pWindow )
{
	if( pWindow == NULL )
		return;

	SPWindowStatic*	pWindowStatic;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_REWARD_TITLE_IMG , 17 , 1 , 17 , 13 , pWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 153 , 78 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_REWARD_TITLE_TEXT , 42 , 2 , 51 , 12 , pWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 373 , 355 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 373 , 355 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 426 , 355 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 426 , 340 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_REWARD_PAGE_IMG , 209 , 6 , 27 , 7 , pWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 213 , 76 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_REWARD_PAGE_TEXT , 240 , 3 , 27 , 12 , pWindow );
	pWindowStatic->SetFont( FONT_12_BOLD );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) );
	pWindowStatic->SetFormat( DT_LEFT | DT_TOP );
	pWindowStatic	=	NULL;

	SPWindowButton*	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_QUESTVIEW_REWARD_RANK_A , 102 , 0 , 15 , 16 , pWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 1 , 121 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 1 , 138 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 1 , 155 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 239 , 181 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 1 , 155 );
	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_QUESTVIEW_REWARD_RANK_B , 121 , 0 , 15 , 16 , pWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 17 , 121 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 17 , 138 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 17 , 155 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 239 , 181 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 17 , 155 );
	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_QUESTVIEW_REWARD_RANK_C , 140 , 0 , 15 , 16 , pWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 33 , 121 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 33 , 138 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 33 , 155 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 239 , 181 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 33 , 155 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_REWARD_XP_ELY_FAME_IMG , 31 , 26 , 21 , 37 , pWindow );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 96 , 121 );
	pWindowStatic	=	NULL;


	int	iUnitNumber	=	0;

	for( int y = 0 ; y < QUESTVIEW_UNIT_Y_COUNT ; ++y )
	{
		for( int x = 0 ; x < QUESTVIEW_UNIT_X_COUNT ; ++x )
		{
			pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_REWARD_ITEM_ICON + iUnitNumber , 36 + x * QUESTVIEW_UNIT_X_SIZE , 71 + y * QUESTVIEW_UNIT_Y_SIZE , 32 , 32 , pWindow );
			pWindowStatic	=	NULL;
		}
	}

	pWindowStatic	=	new SPWindowStatic( WIID_QUESTVIEW_REWARD_ITEM_ICON + iUnitNumber , 36 , 71 , 32 , 32 , pWindow );
	pWindowStatic	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_QUESTVIEW_REWARD_LEFT , 8 , 37 , 9 , 13 , pWindow );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 76 , 121 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 76 , 135 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 76 , 149 );
	pWindowButton	=	NULL;

}

SPQuestAttr* SPWindowQuestView::GetQuestAttr( int iQuestID , INSTANCE_ID eInstanceID )
{
	SPQuestAttr*	pQuestAttr		=	NULL;
	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return pQuestAttr;

	SPQuestArchive*	pQuestArchive		=	NULL;
	SPQuestArchive*	pPlayerQuestArchive	=	pQuestManager->GetPlayerArchive();

	if( eInstanceID == WIID_QUEST )
		pQuestArchive	=	pQuestManager->GetPlayerArchive();
	else
		pQuestArchive	=	pQuestManager->GetNpcArchive();

	if( pQuestArchive == NULL )
		return pQuestAttr;

	pQuestAttr	=	pQuestArchive->GetQuest( iQuestID );

	if( pPlayerQuestArchive != NULL )
	{
		SPQuestAttr*	pPlayerQuestAttr	=	pPlayerQuestArchive->GetQuest( iQuestID );

		if( pPlayerQuestAttr != NULL )
			pQuestAttr	=	pPlayerQuestAttr;
	}

	return pQuestAttr;
}

void SPWindowQuestView::UpdatePosWindow()
{
	SPWindowStatic*	pWindowStatic;
	RECT			rcWindow , rcWindowTemp;
	int				iPosY , iPosYTemp;
	int				iSizeX , iSizeY;
	int				iHeight;

	pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_TALK ) );

	if( pWindowStatic == NULL )
		return;

	iHeight	=	pWindowStatic->GetTextPixelHeight();

	pWindowStatic->GetWindowRect( rcWindow );

	iSizeX	=	rcWindow.right	-	rcWindow.left;
//	iSizeY	=	rcWindow.bottom	-	rcWindow.top;

	pWindowStatic->AdjustCoord( rcWindow.left - m_iAX , rcWindow.top - m_iAY , iSizeX , iHeight );
	pWindowStatic->GetWindowRect( rcWindow );

	if( pWindowStatic->IsShow() == false )
	{
		rcWindow.bottom	=	rcWindow.top;
	}

	//////////////////////////////////////////////////////////////////////////

	int	iTop	=	rcWindow.top;
	int	iBottom	=	rcWindow.bottom;

	int	iMiddleSY		=	iTop	+	QUESTVIEW_BRACE_Y_SIZE;
	int	iMiddleEY		=	iBottom	-	QUESTVIEW_BRACE_Y_SIZE;
	int	iMiddleSizeY	=	iMiddleEY - iMiddleSY;

	pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_BRACE_LEFT_TOP , true ) );

	if( pWindowStatic == NULL )
		return;

	pWindowStatic->GetWindowRect( rcWindowTemp );
	pWindowStatic->AdjustCoord( rcWindowTemp.left - m_iAX , iTop - m_iAY );

	pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_BRACE_LEFT_MIDDLE , true ) );

	if( pWindowStatic == NULL )
		return;

	pWindowStatic->GetWindowRect( rcWindowTemp );
	pWindowStatic->AdjustCoord( rcWindowTemp.left - m_iAX , iMiddleSY - m_iAY , QUESTVIEW_BRACE_X_SIZE , iMiddleSizeY );
	pWindowStatic->SetSrcSize( QUESTVIEW_BRACE_X_SIZE , QUESTVIEW_BRACE_Y_SIZE );

	pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_BRACE_LEFT_BOTTOM , true ) );

	if( pWindowStatic == NULL )
		return;

	pWindowStatic->GetWindowRect( rcWindowTemp );
	pWindowStatic->AdjustCoord( rcWindowTemp.left - m_iAX , iMiddleEY - m_iAY );

	pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_BRACE_RIGHT_TOP , true ) );

	if( pWindowStatic == NULL )
		return;

	pWindowStatic->GetWindowRect( rcWindowTemp );
	pWindowStatic->AdjustCoord( rcWindowTemp.left - m_iAX , iTop - m_iAY );

	pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_BRACE_RIGHT_MIDDLE , true ) );

	if( pWindowStatic == NULL )
		return;

	pWindowStatic->GetWindowRect( rcWindowTemp );
	pWindowStatic->AdjustCoord( rcWindowTemp.left - m_iAX , iMiddleSY - m_iAY , QUESTVIEW_BRACE_X_SIZE , iMiddleSizeY );
	pWindowStatic->SetSrcSize( QUESTVIEW_BRACE_X_SIZE , QUESTVIEW_BRACE_Y_SIZE );

	pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_BRACE_RIGHT_BOTTOM , true ) );

	if( pWindowStatic == NULL )
		return;

	pWindowStatic->GetWindowRect( rcWindowTemp );
	pWindowStatic->AdjustCoord( rcWindowTemp.left - m_iAX , iMiddleEY - m_iAY );

	//////////////////////////////////////////////////////////////////////////

	iPosY	=	rcWindow.bottom	+	QUESTVIEW_TALK_TO_PURPOSE_TITLE;

	pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_PURPOSE_TITLE_TEXT ) );

	if( pWindowStatic == NULL )
		return;

	pWindowStatic->GetWindowRect( rcWindow );
	pWindowStatic->AdjustCoord( rcWindow.left - m_iAX , iPosY + 2 - m_iAY );

	//////////////////////////////////////////////////////////////////////////
	
	pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_PURPOSE_TITLE_IMG ) );

	if( pWindowStatic == NULL )
		return;

	pWindowStatic->GetWindowRect( rcWindow );
	pWindowStatic->AdjustCoord( rcWindow.left - m_iAX , iPosY - m_iAY );
	pWindowStatic->GetWindowRect( rcWindow );

	//////////////////////////////////////////////////////////////////////////

	iPosY		=	rcWindow.bottom	+	QUESTVIEW_PURPOSE_TITLE_TO_PURPOSE_LINE1;
	iPosYTemp	=	iPosY;
	std::string strString;
	bool		bShow	=	false;

	for( int i = 0 ; i < 4 ; ++i )
	{
		strString.clear();

		pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_PURPOSE_LINE_1_TEXT + i ) );

		if( pWindowStatic != NULL )
		{
			pWindowStatic->GetWindowRect( rcWindow );
			pWindowStatic->AdjustCoord( rcWindow.left - m_iAX , iPosYTemp - m_iAY );

			strString	=	pWindowStatic->GetWindowText();

			if( strString.empty() == false )
			{
				pWindowStatic->Show();
				bShow	=	true;
			}
			else
			{
				pWindowStatic->Hide();
			}
		}

		//////////////////////////////////////////////////////////////////////////

		pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_PURPOSE_LINE_1_IMG + i ) );

		if( pWindowStatic != NULL )
		{
			pWindowStatic->GetWindowRect( rcWindow );
			pWindowStatic->AdjustCoord( rcWindow.left - m_iAX , iPosYTemp + 1 - m_iAY );
			pWindowStatic->GetWindowRect( rcWindow );

			iPosYTemp	=	rcWindow.bottom;

			if( strString.empty() == true )
			{
				pWindowStatic->Hide();
			}
			else
			{
				pWindowStatic->Show();
			}

			if( pWindowStatic->IsShow() == true )
			{
				iPosY	=	iPosYTemp;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_PURPOSE_TITLE_TEXT ) );

	if( pWindowStatic == NULL )
		return;

	if( bShow == false )
		pWindowStatic->Hide();
	else
		pWindowStatic->Show();

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_PURPOSE_TITLE_IMG ) );

	if( pWindowStatic == NULL )
		return;

	if( bShow == false )
		pWindowStatic->Hide();
	else
		pWindowStatic->Show();

	//////////////////////////////////////////////////////////////////////////

	if( bShow == false )
	{
		pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_TALK ) );

		if( pWindowStatic != NULL )
		{
			pWindowStatic->GetWindowRect( rcWindow );

			if( pWindowStatic->IsShow() == true )
				iPosY	=	rcWindow.bottom;
			else
				iPosY	=	rcWindow.top;

			iPosY	+=	QUESTVIEW_TALK_TO_PURPOSE_TITLE;
		}
	}
	else
	{
		iPosY		+=	QUESTVIEW_PURPOSE_SHOW_LINE_TO_REWARD;
	}

	pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_REWARD ) );

	if( pWindowStatic == NULL )
		return;

	UpdateRankCount();

	if( m_iRankCount > 0 )
		pWindowStatic->Show();
	else
        pWindowStatic->Hide();

	UpdateRank();

	UpdateRewardPage();

	if( pWindowStatic->IsShow() == false )
	{
		if( bShow == false )
		{
			pWindowStatic	=	(SPWindowStatic*)( Find( WIID_QUESTVIEW_TALK ) );

			if( pWindowStatic != NULL )
			{
				pWindowStatic->GetWindowRect( rcWindow );
				
				if( pWindowStatic->IsShow() == true )
					rcWindow.bottom	+=	QUESTVIEW_PURPOSE_SHOW_LINE_TO_REWARD;
				else
					rcWindow.bottom	=	rcWindow.top	+	QUESTVIEW_PURPOSE_SHOW_LINE_TO_REWARD;
			}
		}
		else
		{
			rcWindow.bottom	=	iPosY	+	QUESTVIEW_PURPOSE_SHOW_LINE_TO_REWARD;
		}
	}
	else
	{
		pWindowStatic->GetWindowRect( rcWindow );
		pWindowStatic->AdjustCoord( rcWindow.left - m_iAX , iPosY - m_iAY );
		pWindowStatic->RefreshChildPos();
		pWindowStatic->GetWindowRect( rcWindow );
	}

	SetRect( &m_rcSubBGDest[2]	, m_iAX + 97  , rcWindow.top , m_iAX +  97 + 2   , rcWindow.top + 16 );											//	랭크 페이지 백그라운드 왼쪽
	SetRect( &m_rcSubBGDest[3]	, m_iAX + 99  , rcWindow.top , m_iAX +  99 + 164 , rcWindow.top + 16 );											//	랭크 페이지 백그라운드 가운데
	SetRect( &m_rcSubBGDest[4]	, m_iAX + 263 , rcWindow.top , m_iAX + 263 + 2   , rcWindow.top + 16 );											//	랭크 페이지 백그라운드 오른쪽

	//////////////////////////////////////////////////////////////////////////

	iPosY	=	rcWindow.bottom;

	GetWindowRect( rcWindow );

	iSizeY	=	iPosY	-	rcWindow.bottom;

	m_iSY	+=	iSizeY;

	SetRect( &m_rcBGDest[0]		, m_iAX					, m_iAY					, m_iAX + m_iSX			, m_iAY + 4 );							//	배경 상단
	SetRect( &m_rcBGDest[1]		, m_rcBGDest[0].left	, m_rcBGDest[0].bottom	, m_rcBGDest[0].right	, m_rcBGDest[0].bottom + m_iSY - 8 );	//	배경 중단
	SetRect( &m_rcBGDest[2]		, m_rcBGDest[1].left	, m_rcBGDest[1].bottom	, m_rcBGDest[1].right	, m_rcBGDest[1].bottom + 4 );			//	배경 하단

	SetRect( &m_rcSubBGDest[0]	, m_iAX + 2	, m_iAY + 27 , m_iAX + 2 + 293 , m_iAY + 27 + m_iSY - 31 );											//	흰색 서브 백그라운드
	SetRect( &m_rcSubBGDest[1]	, m_iAX + 2	, m_iAY + 26 , m_iAX + 2 + 293 , m_iAY + 26 + 1 );													//	경계선 1

}

void SPWindowQuestView::UpdatePosItemUnit( int iXPos , int iYPos , bool bShow )
{
	SPUIItemUnit*	pUIItemUnit;
	int				iUnitNumber	=	0;

	for( int y = 0 ; y < QUESTVIEW_UNIT_Y_COUNT ; ++y )
	{
		for( int x = 0 ; x < QUESTVIEW_UNIT_X_COUNT ; ++x )
		{
			pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( iUnitNumber ) );

			++iUnitNumber;

			if( pUIItemUnit == NULL )
				continue;

			pUIItemUnit->SetPos(	iXPos + QUESTVIEW_UNIT_X_OFFSET + ( x * QUESTVIEW_UNIT_X_SIZE ) ,
									iYPos + QUESTVIEW_UNIT_Y_OFFSET + ( y * QUESTVIEW_UNIT_Y_SIZE ) );
			pUIItemUnit->ProcessCount();

			if( bShow == true )
				pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			else
				pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		}
	}

	m_UIUnitManager.ReCalRect();
}

void SPWindowQuestView::UpdateMission()
{
	SPQuestAttr*	pQuestAttr	=	GetQuestAttr( m_iQuestID , m_eQuestInstanceID );

	if( pQuestAttr == NULL )
		return;

	//	목적
	SPMission*				pMission;
	SPItemAttr*				pItemAttr;
	std::string				strName;
	SPUIGOBUnit				UIGOBUnit;
	SPSkillManager*			pSkillManager		=	SPSkillManager::GetInstance();
	SPActionInterface*		pActionInterface	=	NULL;
	SPEventArchive*			pEventArchive		=	g_pEventManager->GetEventArchive();
	SPWindow*				pWindow;
	SPWindow*				pWindowImg;
	char					szBuf[ _MAX_PATH ];
	int						iNumber				=	0;
	bool					bComplete			=	true;

	for( int i = 0 ; i < QUEST_MAX_MISSION ; i++ )
	{
		pMission	=	&pQuestAttr->m_Mission[ i ];

		pWindowImg	=	Find( WIID_QUESTVIEW_PURPOSE_LINE_1_IMG + i );
		pWindow		=	Find( WIID_QUESTVIEW_PURPOSE_LINE_1_TEXT + i );

		if( pWindow == NULL || pWindowImg == NULL)
			continue;

		strName.clear();

		// 미션 Y/n에 따라 처리한다. [1/8/2007 AJJIYA]
		if( pMission->m_bShow == true )
		{
			if( pMission->m_iMissionType == MISSION_TYPE_ITEM )
			{
				if( g_pItemCluster->GetItemInfo( pMission->m_iMissionTypeID , pItemAttr ) == true )
				{
					strName	=	pItemAttr->m_strName.c_str();
				}
			}
			else if( pMission->m_iMissionType == MISSION_TYPE_MONSTER )
			{
				UIGOBUnit.SetClassID( (CLASS_ID)pMission->m_iMissionTypeID );
				UIGOBUnit.ReResourceLoad();

				strName	=	UIGOBUnit.GetName();
			}
			else if( pMission->m_iMissionType == MISSION_TYPE_ACTION )
			{
				pActionInterface	=	pSkillManager->GetActionInterface( pMission->m_iMissionTypeID );

				if( pActionInterface != NULL )
				{
					strName	=	pActionInterface->GetName();
				}
			}
			else if( pMission->m_iMissionType == MISSION_TYPE_EVENT )
			{
				if( pEventArchive != NULL )
				{
					MAPEVENT_INFO*	pMapEventInfo	=	pEventArchive->GetEvent( pMission->m_iMissionTypeID );

					if( pMapEventInfo != NULL )
					{
						strName	=	pMapEventInfo->name;
					}
				}
			}
		}
		else
		{
			if( pMission->m_iMissionType != MISSION_TYPE_NULL )
				strName	=	m_strMissionView;
		}

		if( strName.empty() == false )
		{
			if( pMission->m_bShow == true )
				sprintf( szBuf , "%s (%d/%d)" , strName.c_str() , pMission->m_iCurCount , pMission->m_iMissionCount );
			else
				sprintf( szBuf , "%s" , strName.c_str() );

			iNumber =	0;

			if( pMission->m_bClear == true && pMission->m_iCurCount >= pMission->m_iMissionCount )
				iNumber	=	1;

			pWindowImg->SPSendMessage( SPIM_SET_IMAGEINDEX , iNumber );
			pWindowImg->Show();

			pWindow->SetFontColor( m_cPurposeColor[ iNumber ] );
			pWindow->SetWindowText( szBuf );
			pWindow->Show();
		}
		else
		{
			pWindow->SetWindowText( NULL );
			pWindow->Hide();
			pWindowImg->Hide();
		}

		if( pWindow->IsShow() == true )
		{
			if( iNumber == 0 )
				bComplete	=	false;
		}
	}

	pWindowImg	=	Find( WIID_QUESTVIEW_PURPOSE_TITLE_IMG );

	if( bComplete == true )
	{
		iNumber	=	1;
	}
	else
	{
		iNumber	=	0;
	}

	if( pWindowImg != NULL)
		pWindowImg->SPSendMessage( SPIM_SET_IMAGEINDEX , iNumber );
}

void SPWindowQuestView::UpdateReward()
{
	int	iExp		=	0;
	int	iEly		=	0;
	int	iFame		=	0;
	int	iGuildPoint	=	0;

	SPWindow*	pWindow;
	char		szBuf[ _MAX_PATH ];

	pWindow	=	Find( WIID_QUESTVIEW_REWARD_XP_TEXT , true );
	sprintf( szBuf , "%d" , iExp );
	pWindow->SetWindowText( szBuf );

	pWindow	=	Find( WIID_QUESTVIEW_REWARD_ELY_TEXT , true );
	sprintf( szBuf , "%d" , iEly );
	pWindow->SetWindowText( szBuf );

	pWindow	=	Find( WIID_QUESTVIEW_REWARD_FAME_TEXT , true );
	sprintf( szBuf , "%d" , iFame );
	pWindow->SetWindowText( szBuf );

	pWindow	=	Find( WIID_QUESTVIEW_REWARD_GUIDL_POINT_TEXT , true );
	sprintf( szBuf , "%d" , iGuildPoint );
	pWindow->SetWindowText( szBuf );

	SPQuestItem*	pQuestRewardItem;
	SPUIItemUnit*	pUIItemUnit;

	for( int i = 0 ; i < QUESTREWARD_MAX_ITEM ; ++i )
	{
		pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( i ) );

		if( pUIItemUnit == NULL )
			continue;

		pUIItemUnit->SetStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
		pUIItemUnit->SetTexture( (SPTexture*)NULL );
	}

	SPQuestAttr*	pQuestAttr	=	GetQuestAttr( m_iQuestID , m_eQuestInstanceID );

	if( pQuestAttr == NULL )
		return;

	UpdateRewardPage();

	if( m_iRewardRank < 0 || m_iRewardRank >= QUEST_MAX_RANK )
		return;

	if( m_iRewardInfo < 0 || m_iRewardInfo >= QUEST_MAX_REWARD )
		return;

	SPRewordRank*	pRewardRank	=	&( pQuestAttr->m_RewordRank[ m_iRewardRank ] );

	if( pRewardRank == NULL )
		return;

	SPRewordInfo*	pRewardInfo	=	&( pRewardRank->m_Info[ m_iRewardInfo ] );

	if( pRewardInfo == NULL )
		return;

	if( pRewardInfo->m_iID == 0 )
		return;

	SPRewardAttr*	pRewardAttr		=	pRewardInfo->m_pRewardInfo;
	SPQuestManager* pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return;

	if( pRewardAttr == NULL )
	{
		SPRewardArchive*	pRewardArcive	=	pQuestManager->GetRewardInfo();

		if( pRewardArcive != NULL )
		{
			pRewardAttr = pRewardArcive->GetReward( pRewardInfo->m_iID );
		}
	}

	if( pRewardAttr != NULL )
	{
		iExp		=	pRewardAttr->m_iExp;
		iEly		=	pRewardAttr->m_iEly;
		iFame		=	pRewardAttr->m_iFame;
		iGuildPoint	=	pRewardAttr->m_iGuildPoint;
	}

	//SPWindow*	pWindow;
	//char		szBuf[ _MAX_PATH ];

	pWindow	=	Find( WIID_QUESTVIEW_REWARD_XP_TEXT , true );
	pWindow->SetWindowText( ConvertMoneyToString( iExp ) );

	pWindow	=	Find( WIID_QUESTVIEW_REWARD_ELY_TEXT , true );
	pWindow->SetWindowText( ConvertMoneyToString( iEly ) );

	pWindow	=	Find( WIID_QUESTVIEW_REWARD_FAME_TEXT , true );
	pWindow->SetWindowText( ConvertMoneyToString( iFame ) );

	pWindow	=	Find( WIID_QUESTVIEW_REWARD_GUIDL_POINT_TEXT , true );
	pWindow->SetWindowText( ConvertMoneyToString( iGuildPoint ) );

	if( iGuildPoint > 0 )
	{
		pWindow	=	Find( WIID_QUESTVIEW_REWARD_GUIDL_POINT_TEXT , true );
		pWindow->SetShowEnable( true );

		pWindow	=	Find( WIID_QUESTVIEW_REWARD_GUIDL_POINT_IMG , true );
		pWindow->SetShowEnable( true );
	}
	else
	{
		pWindow	=	Find( WIID_QUESTVIEW_REWARD_GUIDL_POINT_TEXT , true );
		pWindow->SetShowEnable( false );
		pWindow->Hide();

		pWindow	=	Find( WIID_QUESTVIEW_REWARD_GUIDL_POINT_IMG , true );
		pWindow->SetShowEnable( false );
		pWindow->Hide();
	}

	for( i = 0 ; i < QUESTREWARD_MAX_ITEM ; ++i )
	{
		pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( i ) );

		if( pUIItemUnit == NULL )
			continue;

		if( pRewardAttr == NULL )
			continue;

		pQuestRewardItem	=	&( pRewardAttr->m_Item[ i ] );

		if( pQuestRewardItem == NULL )
			continue;

		pUIItemUnit->SetItemID( pQuestRewardItem->m_iItemID );
		pUIItemUnit->SetCount( pQuestRewardItem->m_iStack );
		pUIItemUnit->ReResourceLoad();
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );

		if( g_pCheckManager->CheckItemStatus( pQuestRewardItem->m_iItemID ) == false )
			pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DISABLE );
		else
			pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DISABLE );
	}
}

void SPWindowQuestView::UpdateRewardPage()
{
	SPQuestAttr*	pQuestAttr	=	GetQuestAttr( m_iQuestID , m_eQuestInstanceID );

	if( pQuestAttr == NULL )
		return;

	if( m_iRewardRank < 0 || m_iRewardRank >= QUEST_MAX_RANK )
		return;

	SPRewordRank*	pRewardRank	=	&( pQuestAttr->m_RewordRank[ m_iRewardRank ] );

	if( pRewardRank == NULL )
		return;

	SPRewordInfo*	pRewardInfo;

	m_iRewardInfoMax	=	0;

	for( int i = 0 ; i < QUEST_MAX_REWARD ; ++i )
	{
		pRewardInfo	=	&( pRewardRank->m_Info[ i ] );

		if( pRewardInfo == NULL )
			continue;

		if( pRewardInfo->m_iID == 0 )
			break;

		++m_iRewardInfoMax;
	}

	SPWindow*	pWindow;

	REWARD_TYPE		eRewardType	=	pRewardRank->m_iType;

	pWindow	=	Find( WIID_QUESTVIEW_REWARD_TITLE_TEXT , true );

	if( pWindow != NULL )
	{
		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , (int)eRewardType );
	}

	if( eRewardType == REWARD_TYPE_FIX )
		m_iRewardInfo = 0;

	pWindow	=	Find( WIID_QUESTVIEW_REWARD_PAGE_TEXT , true );

	char	szBuf[ _MAX_PATH ];

	if( m_iRewardInfoMax == 0 )
		sprintf( szBuf , "%d/%d", m_iRewardInfo + 1 , m_iRewardInfoMax + 1 );
	else
		sprintf( szBuf , "%d/%d", m_iRewardInfo + 1 , m_iRewardInfoMax );

	if( pWindow != NULL )
	{
		pWindow->SetWindowText( szBuf );
	}

	pWindow	=	Find( WIID_QUESTVIEW_REWARD_LEFT , true );

	if( pWindow != NULL )
	{
		if( m_iRewardInfoMax > 1 )
			pWindow->Show();
		else
			pWindow->Hide();
	}

	pWindow	=	Find( WIID_QUESTVIEW_REWARD_RIGHT , true );

	if( pWindow != NULL )
	{
		if( m_iRewardInfoMax > 1 )
			pWindow->Show();
		else
			pWindow->Hide();
	}
}

void SPWindowQuestView::UpdateTalk()
{
	SPWindow*	pWindow;

	pWindow	=	Find( WIID_QUESTVIEW_TAB_TALK );

	if( pWindow != NULL )
	{
		pWindow->SPSendMessage( SPIM_BTN_SELECT , TRUE );
	}

	pWindow	=	Find( WIID_QUESTVIEW_TAB_SUMMARY );

	if( pWindow != NULL )
	{
		pWindow->SPSendMessage( SPIM_BTN_SELECT , FALSE );
	}

	SPQuestAttr*	pQuestAttr	=	GetQuestAttr( m_iQuestID , m_eQuestInstanceID );

	if( pQuestAttr == NULL )
		return;

	pWindow	=	Find( WIID_QUESTVIEW_TALK );

	if( pWindow != NULL )
	{
		std::string	strTalk;

		strTalk.clear();

		if( pQuestAttr->m_strTalk1.empty() == false )
		{
			strTalk	+=	pQuestAttr->m_strTalk1;
			strTalk	+=	"\n\n";
		}

		if( pQuestAttr->m_strTalk2.empty() == false )
		{
			strTalk	+=	pQuestAttr->m_strTalk2;
		}

		if( strTalk.empty() == true )
		{
			pWindow->Hide();
		}
		else
		{
			pWindow->SetWindowText( strTalk.c_str() );
			pWindow->Show();
		}
	}
}

void SPWindowQuestView::UpdateSummary()
{
	SPWindow*	pWindow;

	pWindow	=	Find( WIID_QUESTVIEW_TAB_SUMMARY );

	if( pWindow != NULL )
	{
		pWindow->SPSendMessage( SPIM_BTN_SELECT , TRUE );
	}

	pWindow	=	Find( WIID_QUESTVIEW_TAB_TALK );

	if( pWindow != NULL )
	{
		pWindow->SPSendMessage( SPIM_BTN_SELECT , FALSE );
	}

	SPQuestAttr*	pQuestAttr	=	GetQuestAttr( m_iQuestID , m_eQuestInstanceID );

	if( pQuestAttr == NULL )
		return;

	pWindow	=	Find( WIID_QUESTVIEW_TALK );

	if( pWindow != NULL )
	{
		if( pQuestAttr->m_strPurPose.empty() == true )
		{
			pWindow->Hide();
		}
		else
		{
			pWindow->SetWindowText( pQuestAttr->m_strPurPose.c_str() );
			pWindow->Show();
		}
	}
}

void SPWindowQuestView::UpdateRank()
{
	int	iCount		=	WIID_QUESTVIEW_REWARD_RANK_C - WIID_QUESTVIEW_REWARD_RANK_A;

	SPWindow*	pWindow;
	BOOL		bShow;
	int			i;

	for( i = 0 ; i <= iCount ; ++i )
	{
		pWindow	=	Find( WIID_QUESTVIEW_REWARD_RANK_A + i , true );

		if( pWindow == NULL )
			continue;

		if( i == m_iRewardRank )
			bShow	=	TRUE;
		else
			bShow	=	FALSE;

		pWindow->SPSendMessage( SPIM_BTN_SELECT , bShow );
	}
}

void SPWindowQuestView::UpdateRankCount()
{
	SPQuestAttr*	pQuestAttr	=	GetQuestAttr( m_iQuestID , m_eQuestInstanceID );

	if( pQuestAttr == NULL )
		return;

	REWARD_TYPE	iType;
	SPWindow*	pWindow;
	bool		bEnable;
	bool		bNoAdd	=	false;

	m_iRankCount	=	0;

	for( int i = 0 ; i < QUEST_MAX_RANK ; ++i )
	{
		iType	=	pQuestAttr->m_RewordRank[ i ].m_iType;
		pWindow	=	Find( WIID_QUESTVIEW_REWARD_RANK_A + i , true );

		if( pWindow == NULL )
			continue;

		bEnable	=	false;

		if( (int)iType == 0 )
		{
			bNoAdd	=	true;
		}
		else
		{
			if( bNoAdd == false )
			{
				bEnable	=	true;
				++m_iRankCount;
			}
		}

		if( m_eQuestInstanceID == WIID_NPC_QUEST && i != m_iRewardRank )
			bEnable	=	false;

		pWindow->SetEnable( bEnable );
	}
}

void SPWindowQuestView::DefaultRewardRank()
{
	SPQuestAttr*	pQuestAttr	=	GetQuestAttr( m_iQuestID , m_eQuestInstanceID );

	if( pQuestAttr == NULL )
		return;

	m_iRewardRank	=	pQuestAttr->IsRewardRequire();

	if( m_iRewardRank >= m_iRankCount )
		m_iRewardRank = m_iRankCount - 1; 

	m_iRewardInfo	=	0;
}
