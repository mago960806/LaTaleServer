// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-9-23    14:57 
//***************************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowSlider.h"

#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"
#include "SPTreeBoxItemQuest.h"

#include "SPWindowTreeBox.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

//#include "SPItemAttr.h"
//#include "SPItemCluster.h"
//
//#include "SPCheckManager.h"
//
//#include "SPSkill.h"
//#include "SPMotionStatus.h"
//#include "SPSkillManager.h"
//#include "SPEffect.h"
//#include "SPEffectManager.h"
//
#include "SPEventDEF.h"
#include "SPEventManager.h"
#include "SPEventNpcHandler.h"

#include "SPQuestManager.h"
#include "SPQuestArchive.h"
#include "SPQuestAttr.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "SPGOBStatus.h"
#include "SPPlayerInvenArchive.h"

//#include "SPUIUnit.h"
//#include "SPUIItemUnit.h"
//#include "SPUISkillUnit.h"
//#include "SPUIGOBUnit.h"
//#include "SPUIQuestUnit.h"
//#include "SPUIUnitManager.h"
//
//#include "Packet.h"
//#include "PacketID.h"

#include "SPWindowQuest.h"

SPWindowQuest::SPWindowQuest(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowQuest::SPWindowQuest(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_QUEST, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowQuest::~SPWindowQuest()
{
	Clean();
}

void SPWindowQuest::Init()
{
	m_strToolTip	= "[SPWindowQuest]";

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" , &m_pBGTexture );
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , &m_pSubBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[0]		, 125 , 354 , 372 , 358 );		//	배경 상단
	SetRect( &m_rcBGSrc[1]		, 125 , 360 , 372 , 362 );		//	배경 중단
	SetRect( &m_rcBGSrc[2]		, 125 , 364 , 372 , 368 );		//	배경 하단
	
	SetRect( &m_rcSubBGSrc[0]	, 349 , 312 , 355 , 318 );		//	옅은 초록 서브 백그라운드
	SetRect( &m_rcSubBGSrc[1]	, 349 , 329 , 355 , 330 );		//	경계선 1
	SetRect( &m_rcSubBGSrc[2]	, 349 , 329 , 355 , 330 );		//	경계선 2

	SPWindowButton*	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_QUEST_CLOSE , 226 , 9 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 217 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 230 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 243 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 256 );
	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_QUEST_ACCEPT , 169 , 249 , 68 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 373 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 392 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 430 );
	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_QUEST_COMPLETE , 169 , 249 , 68 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 70 , 373 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 70 , 392 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 70 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 70 , 430 );
	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_QUEST_GIVEUP , 169 , 249 , 68 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 139 , 373 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 139 , 392 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 139 , 411 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 139 , 430 );
	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_QUEST_SHARE , 10 , 249 , 68 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 186 , 14 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 255 , 14 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 324 , 14 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 393 , 14 );
	pWindowButton	=	NULL;

	SPWindowStatic*	pWindowStatic;

	pWindowStatic	=	new SPWindowStatic( WIID_QUEST_TITLE_IMAGE , 7 , 3 , 30 , 23 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 1 , 314 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUEST_TITLE_TEXT , 42 , 7 , 75 , 15 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 49 , 354 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_QUEST_SUB_CATEGORY , 10 , 29 , 200 , 15 , this );
	pWindowStatic->SetFont( FONT_12_BOLD );
	pWindowStatic->SetFontColor( D3DCOLOR_ARGB( 255 ,  70 , 140 , 124 ) );
	pWindowStatic->SetFormat( DT_LEFT | DT_TOP );
	pWindowStatic	=	NULL;

	SPWindowTreeBox*	pTreeBox	=	new SPWindowTreeBox( WIID_QUEST_TREE_LIST , 2 , 46 , 243 , 198 , this );
	pTreeBox->SetBoxTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 349 , 321 , 355 , 327 , 2 );
	pTreeBox->SetMargin( 8 , 4 , 20 , 4 );
	pTreeBox->UpdateBGPos();

	pTreeBox->SetScrollAutoHide( false );

	pTreeBox->SetScrollUpSize( 12 , 11 );
	pTreeBox->SetScrollUpTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	pTreeBox->SetScrollUpTexture( DATA_TEXTURE_HIT			, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	pTreeBox->SetScrollUpTexture( DATA_TEXTURE_PUSH			, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	pTreeBox->SetScrollUpTexture( DATA_TEXTURE_DISABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );

	pTreeBox->SetScrollDownSize( 12 , 11 );
	pTreeBox->SetScrollDownTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	pTreeBox->SetScrollDownTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	pTreeBox->SetScrollDownTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	pTreeBox->SetScrollDownTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );

	pTreeBox->SetScrollSliderSize( 12 , 24 );
	pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 470 );
	pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 470 );
	pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 470 );
	pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 470 );

	pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 496 , 472 , 498 );
	pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_HIT	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 496 , 485 , 498 );
	pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 496 , 498 , 498 );
	pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_DISABLE, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 496 , 511 , 498 );

	pTreeBox->SetScrollMargin( 0 , 4 , 4 , 4 );
	pTreeBox->UpdateScrollPos();

	pTreeBox->Refresh();

	ResetCategoryItem();

	m_cStringColor		=	D3DCOLOR_ARGB( 255 , 49 , 76 , 97 );
	m_bClickNotify		=	false;

	m_iOldAX			=	m_iAX;
	m_iOldAY			=	m_iAY;
}

void SPWindowQuest::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
	SAFE_RELEASE( m_pSubBGTexture );

	ResetCategoryItem();
}

void SPWindowQuest::Show( bool bWithChild /* = true  */ )
{
	CUT_IN_STATE	eCutInState	=	g_pEventManager->GetCutInState();
	bool			bMoveable	=	true;

	if( m_InstanceID == WIID_NPC_QUEST )
	{
		if( !( eCutInState == CUT_IN_STATE_NULL || eCutInState == CUT_IN_STATE_QUEST ) )
			return;

		if( eCutInState == CUT_IN_STATE_QUEST )
		{
			bMoveable	=	false;
			AdjustCoord( m_iOldAX , m_iOldAY );
		}
		else
		{
			SPWindow*	pQuestWindow	=	g_pInterfaceManager->FindWindow( WIID_QUEST );

			if( pQuestWindow != NULL )
			{
				int	iPosX , iPosY;
				pQuestWindow->GetAbsCoord( iPosX , iPosY );
				AdjustCoord( iPosX , iPosY );
			}
		}

		SetMoveable( true );
		RefreshRelationCoord();
	}

	SetMoveable( bMoveable );

	SPWindow::Show( bWithChild );

	//[2006/9/14] - 보여지면서 패킷을 보낼수 있도록 초기화
	g_pEventManager->GetQuestManager()->ResetSendBlock();

	std::string		strSubCategory;
	const char*		pszString;
	SPWindow*		pWindow		=	Find( WIID_QUEST_SUB_CATEGORY );

	if( m_InstanceID == WIID_NPC_QUEST && eCutInState == CUT_IN_STATE_QUEST )
	{
		SPWindow*	pCloseWIndow	=	Find( WIID_QUEST_CLOSE );

		if( pCloseWIndow != NULL )
			pCloseWIndow->Hide();

		pCloseWIndow	=	Find( WIID_QUEST_SHARE );

		if( pCloseWIndow != NULL )
			pCloseWIndow->Hide();

		MAPEVENT_INFO*	pNpcInfo	=	g_pEventManager->GetCurEvent();

		if( pNpcInfo != NULL )
		{
			pszString	=	g_pResourceManager->GetGlobalString( GC_SUB_CATEGORY_NPC );

			if( pszString != NULL )
			{
				char szBuf[ _MAX_PATH ];
				sprintf( szBuf , pszString , pNpcInfo->name );
				strSubCategory	=	szBuf;
			}
		}
	}
	else
	{
		int	iGlobalStringNum	=	GC_SUB_CATEGORY_PC;

		if( m_InstanceID == CUT_IN_STATE_NULL && eCutInState == CUT_IN_STATE_QUEST )
			iGlobalStringNum	=	GC_SUB_CATEGORY_SHARE;

		pszString	=	g_pResourceManager->GetGlobalString( iGlobalStringNum );

		if( pszString != NULL )
			strSubCategory	=	pszString;
	}

	if( pWindow != NULL )
		pWindow->SetWindowText( strSubCategory.c_str() );

	if( m_InstanceID == WIID_NPC_QUEST && eCutInState == CUT_IN_STATE_NULL )
		ShowFirstQuest( false );

	//////////////////////////////////////////////////////////////////////////

	int	iSelectQuestID	=	GetSelectItem();
	int	iAction			=	0;
	int	iShare			=	0;

	if( iSelectQuestID > 0 )
	{
		SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();		

		if( pQuestManager != NULL )
		{
			pQuestManager->ResetSendBlock();			//[2006/9/12] 패킷 전송 플래그 초기화
			
			SPQuestArchive*	pQuestArchive	=	NULL;

			if( m_InstanceID == WIID_QUEST )
				pQuestArchive	=	pQuestManager->GetPlayerArchive();
			else
				pQuestArchive	=	pQuestManager->GetNpcArchive();

			if( pQuestArchive != NULL )
			{
				SPQuestAttr*	pQuestAttr	=	pQuestArchive->GetQuest( iSelectQuestID );

				if( pQuestAttr != NULL )
				{
					iAction	=	(int)pQuestAttr->m_iUiShow;
					iShare	=	(int)pQuestAttr->m_eAcceptType;
				}
			}
		}
	}

	SetActionButton( iAction , iShare );
}

void SPWindowQuest::Hide(bool bSendServer)
{
	if( m_bShow )
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide(bSendServer);

	if( m_InstanceID == WIID_NPC_QUEST )
	{
		SPQuestManager::GetInstance()->DeleteNpcQuest();
	}

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
		OnCursorOut( NULL ,NULL );

	//	AJJIYA [9/8/2005]
	//	창 정렬을 위해서!!
	g_pInterfaceManager->OrderWindowPosMove();
}

void SPWindowQuest::Process(float fTime)
{
	if( m_bShow == false ) return;

	SPWindow::Process( fTime );
}

void SPWindowQuest::Render(float fTime)
{
	if( m_bShow == false ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < QUEST_BG_COUNT ; ++i )
		{
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	if( m_pSubBGTexture )
	{
		for( int i = 0 ; i < QUEST_SUB_BG_COUNT ; ++i )
		{
			m_pSubBGTexture->RenderTexture( &m_rcSubBGDest[i] , &m_rcSubBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	SPWindow::Render(fTime);
}

void SPWindowQuest::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

//void SPWindowQuest::SetCutInPos()
//{
//}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowQuest )

	SPIMESSAGE_COMMAND(	SPIM_MOVE_BEGIN				,										OnBeginMove			)	// metalgeni [6/2/2006]
	SPIMESSAGE_COMMAND(	SPIM_MOVE_END				,										OnEndMove			)	// Related to window move
	SPIMESSAGE_COMMAND(	SPIM_MOVE					,										OnMove				)

	SPIMESSAGE_COMMAND(	SPIM_PURGE					,										OnPurge				)
	SPIMESSAGE_COMMAND(	SPIM_QUEST_REFRESH			,										OnRefreshQuest		)

	SPIMESSAGE_COMMAND(	SPIM_QUEST_ADD				,										OnQuestAdd			)
	SPIMESSAGE_COMMAND(	SPIM_QUEST_DEL				,										OnQuestDel			)

	SPIMESSAGE_COMMAND(	SPIM_QUEST_NOTIFY			,										OnQuestNotify		)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_QUEST_CLOSE				,	OnCloseLButtonUp	)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_QUEST_TREE_LIST			,	OnTreeBoxLButtonUp	)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_QUEST_ACCEPT				,	OnAcceptLButtonUp	)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_QUEST_COMPLETE				,	OnCompleteLButtonUp	)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_QUEST_GIVEUP				,	OnGiveUpLButtonUp	)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_QUEST_SHARE				,	OnShareLButtonUp	)


SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

//////////////////////////////////////////////////////////////////////////

int SPWindowQuest::OnBeginMove		( WPARAM wParam, LPARAM lParam)
{
	//int				iXPos		=	LOWORD(lParam);
	//int				iYPos		=	HIWORD(lParam);
	//SPUIUnit*		pUIUnit		=	m_UIUnitManager.GetUnitMouseOver( iXPos , iYPos );

	//if( pUIUnit == NULL )
	//{
	//	return SPWindow::OnBeginMove(wParam, lParam);
	//}
	//	return 0;

	return SPWindow::OnBeginMove(wParam, lParam);
}

int SPWindowQuest::OnEndMove		( WPARAM wParam, LPARAM lParam)
{
	return SPWindow::OnEndMove(wParam, lParam);
}

int SPWindowQuest::OnMove			( WPARAM wParam, LPARAM lParam)
{
	return SPWindow::OnMove(wParam, lParam);
}

int SPWindowQuest::OnPurge( WPARAM wParam, LPARAM lParam )
{
	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_QUEST_TREE_LIST ) );

	if( pTreeBox == NULL )
		return 1;

	SetActionButton( 0 , 0 );

	pTreeBox->DelItemAll();
	ResetCategoryItem();

	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUESTVIEW		, SPIM_PURGE , NULL , NULL );

	if( m_InstanceID == WIID_QUEST )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUESTNOTIFY	, SPIM_PURGE , NULL , NULL );

	return 1;
}

int SPWindowQuest::OnRefreshQuest( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUESTVIEW		, SPIM_QUEST_REFRESH , wParam , lParam );
	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUESTNOTIFY	, SPIM_QUEST_REFRESH , wParam , lParam );

	int		iQuestID	=	(int)wParam;

	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return 1;

	SPQuestArchive*	pQuestArchive	=	NULL;

	if( m_InstanceID == WIID_QUEST )
		pQuestArchive	=	pQuestManager->GetPlayerArchive();
	else
		pQuestArchive	=	pQuestManager->GetNpcArchive();

	if( pQuestArchive == NULL )
		return 1;

	SPQuestAttr*	pQuestAttr	=	pQuestArchive->GetQuest( iQuestID );

	if( pQuestAttr == NULL )
		return 1;

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_QUEST_TREE_LIST ) );

	if( pTreeBox == NULL )
		return 1;

	TREEBOX_ITEM*	pCurItem			=	NULL;

	if( m_InstanceID == WIID_QUEST )
	{
		pCurItem			=	FindQuestItem( iQuestID , pQuestAttr->m_iCityName );
	}
	else
	{
		int	iCategory	=	-1;
		CUT_IN_STATE	eCutInState	=	g_pEventManager->GetCutInState();

		if( eCutInState == CUT_IN_STATE_NULL )
		{
			if( pQuestAttr->m_iCurrentAccept == QUEST_ACCEPTTYPE_SHARE )
				iCategory	=	3;
			else if( pQuestAttr->m_iCurrentAccept == QUEST_ACCEPTTYPE_ITEM )
				iCategory	=	4;
		}
		else
		{
			if( pQuestAttr->m_iUiShow == QUEST_UI_SHOW_COMPLETE )
				iCategory	=	1;
			else if( pQuestAttr->m_iUiShow == QUEST_UI_SHOW_ABLE )
				iCategory	=	2;
		}

		pCurItem			=	FindQuestItem( iQuestID , iCategory );
	}

	if( pCurItem == NULL )
		return 1;

	SPTreeBoxItemQuest*	pQuestItem	=	(SPTreeBoxItemQuest*)pCurItem->m_pItemBase;

	if( pQuestItem == NULL )
		return 1;

	bool	bComplete	=	false;

	if( pQuestAttr->m_iUiShow == QUEST_UI_SHOW_COMPLETE )
	{
		bComplete	=	true;
	}

	pQuestItem->SetQuestComplete( bComplete );

	return 1;
}

int SPWindowQuest::OnQuestAdd( WPARAM wParam, LPARAM lParam )
{
	int	iQuestID	=	(int)lParam;

	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return 1;

	SPQuestArchive*	pQuestArchive	=	NULL;

	if( m_InstanceID == WIID_QUEST )
		pQuestArchive	=	pQuestManager->GetPlayerArchive();
	else
		pQuestArchive	=	pQuestManager->GetNpcArchive();

	if( pQuestArchive == NULL )
		return 1;

	SPQuestAttr*	pQuestAttr	=	pQuestArchive->GetQuest( iQuestID );

	if( pQuestAttr == NULL )
		return 1;

	if( pQuestAttr->m_iUiShow != QUEST_UI_SHOW_COMPLETE && pQuestAttr->m_iUiShow != QUEST_UI_SHOW_ABLE )
		return 1;

	if( pQuestAttr->m_iCityName == CITY_NAME_NULL )
		return 1;

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_QUEST_TREE_LIST ) );

	if( pTreeBox == NULL )
		return 1;

	SPTreeBoxItemQuest*	pQuestItem	=	CreateTreeBoxItem( iQuestID );

	if( pQuestItem == NULL )
		return 1;

	TREEBOX_ITEM*	pParentItem	=	NULL;

	if( m_InstanceID == WIID_QUEST )
	{
		pParentItem	=	CreateCategoryItem( pQuestAttr->m_iCityName );
	}
	else if( m_InstanceID == WIID_NPC_QUEST )
	{
		int	iCategory	=	-1;
		CUT_IN_STATE	eCutInState	=	g_pEventManager->GetCutInState();

		if( eCutInState == CUT_IN_STATE_NULL )
		{
			if( pQuestAttr->m_iCurrentAccept == QUEST_ACCEPTTYPE_SHARE )
				iCategory	=	3;
			else if( pQuestAttr->m_iCurrentAccept == QUEST_ACCEPTTYPE_ITEM )
				iCategory	=	4;
		}
		else
		{
			if( pQuestAttr->m_iUiShow == QUEST_UI_SHOW_COMPLETE )
				iCategory	=	1;
			else if( pQuestAttr->m_iUiShow == QUEST_UI_SHOW_ABLE )
				iCategory	=	2;
			else
			{
				SAFE_DELETE( pQuestItem );
			}
		}

		if( iCategory > 0 )
			pParentItem	=	CreateCategoryItem( iCategory );
	}

	bool	bComplete	=	false;

	if( pQuestAttr->m_iUiShow == QUEST_UI_SHOW_COMPLETE )
	{
		bComplete	=	true;
	}

	pQuestItem->SetQuestComplete( bComplete );

	pTreeBox->AddItem( pParentItem , pQuestItem );
	pTreeBox->Refresh();

	return 1;
}

int SPWindowQuest::OnQuestDel( WPARAM wParam, LPARAM lParam )
{
	int	iQuestID	=	(int)lParam;

	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return 1;

	SPQuestArchive*	pQuestArchive	=	NULL;

	if( m_InstanceID == WIID_QUEST )
		pQuestArchive	=	pQuestManager->GetPlayerArchive();
	else
		pQuestArchive	=	pQuestManager->GetNpcArchive();

	if( pQuestArchive == NULL )
		return 1;

	SPQuestAttr*	pQuestAttr	=	pQuestArchive->GetQuest( iQuestID );

	if( pQuestAttr == NULL )
		return 1;

	if( m_InstanceID == WIID_QUEST )
	{
		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUESTNOTIFY , SPIM_QUEST_NOTIFY , (WPARAM)iQuestID , (LPARAM)false );
		g_pInterfaceManager->ShowWindow( WIID_QUESTVIEW , false );
	}

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_QUEST_TREE_LIST ) );

	if( pTreeBox == NULL )
		return 1;

	TREEBOX_ITEM*	pQuestItem			=	NULL;

	if( m_InstanceID == WIID_QUEST )
	{
		pQuestItem			=	FindQuestItem( iQuestID , pQuestAttr->m_iCityName );
	}
	else
	{
		int	iCategory	=	-1;
		CUT_IN_STATE	eCutInState	=	g_pEventManager->GetCutInState();

		if( eCutInState == CUT_IN_STATE_NULL )
		{
			if( pQuestAttr->m_iCurrentAccept == QUEST_ACCEPTTYPE_SHARE )
				iCategory	=	3;
			else if( pQuestAttr->m_iCurrentAccept == QUEST_ACCEPTTYPE_ITEM )
				iCategory	=	4;
		}
		else
		{
			if( pQuestAttr->m_iUiShow == QUEST_UI_SHOW_COMPLETE )
				iCategory	=	1;
			else if( pQuestAttr->m_iUiShow == QUEST_UI_SHOW_ABLE )
				iCategory	=	2;
		}

		pQuestItem			=	FindQuestItem( iQuestID , iCategory );
	}

	if( pQuestItem == NULL )
		return 1;

	TREEBOX_ITEM*	pParentQuestItem	=	pQuestItem->m_pParent;

	pTreeBox->DelItem( &pQuestItem );

	if( pParentQuestItem != NULL )
	{
		if( (int)pParentQuestItem->m_vpChild.size() <= 0 )
		{
			DeleteCategoryItem( pQuestAttr->m_iCityName );
			pTreeBox->DelItem( &pParentQuestItem );
		}
	}

	pTreeBox->Refresh();

	if( m_InstanceID == WIID_NPC_QUEST )
	{
		CUT_IN_STATE	eCutInState	=	g_pEventManager->GetCutInState();

		if( eCutInState == CUT_IN_STATE_NULL )
		{
			ShowFirstQuest( true );
		}

		if( pTreeBox->IsEmpty() == true )
		{
			Hide();
		}

		int	iSelectQuestID	=	GetSelectItem();
		int	iAction			=	0;
		int	iShare			=	0;

		if( iSelectQuestID > 0 )
		{
			SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();		

			if( pQuestManager != NULL )
			{
				pQuestManager->ResetSendBlock();			//[2006/9/12] 패킷 전송 플래그 초기화

				SPQuestArchive*	pQuestArchive	=	NULL;

				if( m_InstanceID == WIID_QUEST )
					pQuestArchive	=	pQuestManager->GetPlayerArchive();
				else
					pQuestArchive	=	pQuestManager->GetNpcArchive();

				if( pQuestArchive != NULL )
				{
					SPQuestAttr*	pQuestAttr	=	pQuestArchive->GetQuest( iSelectQuestID );

					if( pQuestAttr != NULL )
					{
						iAction	=	(int)pQuestAttr->m_iUiShow;
						iShare	=	(int)pQuestAttr->m_eAcceptType;
					}
				}
			}
		}

		SetActionButton( iAction , iShare );
	}

	return 1;
}

int SPWindowQuest::OnQuestNotify( WPARAM wParam, LPARAM lParam )
{
	SPTreeBoxItemQuest*	pQuestItem	=	(SPTreeBoxItemQuest*)wParam;

	if( pQuestItem == NULL )
		return 1;

	int	iResult = g_pInterfaceManager->SPChildWndSendMessage( WIID_QUESTNOTIFY , SPIM_QUEST_NOTIFY , (WPARAM)(pQuestItem->GetReturnValue()) , (LPARAM)!pQuestItem->GetQuestMiniWindow() );

	pQuestItem->SetQuestMiniWindow( (bool)iResult );

	m_bClickNotify	=	true;

	return 1;
}

int SPWindowQuest::OnCloseLButtonUp( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

int SPWindowQuest::OnTreeBoxLButtonUp( WPARAM wParam, LPARAM lParam )
{
	if( m_bClickNotify == true )
	{
		m_bClickNotify	=	false;
		return 1;
	}

	SetActionButton( 0 , 0 );

	TREEBOX_ITEM*	pSelectQuestItem	=	(TREEBOX_ITEM*)wParam;

	if( pSelectQuestItem == NULL )
		return 1;

	SPTreeBoxItemQuest*	pQuestItem		=	(SPTreeBoxItemQuest*)( pSelectQuestItem->m_pItemBase );

	if( pQuestItem == NULL )
		return 1;

	int	iQuestID	=	pQuestItem->GetReturnValue();

	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return 1;

	SPQuestArchive*	pQuestArchive	=	NULL;

	if( m_InstanceID == WIID_QUEST )
		pQuestArchive	=	pQuestManager->GetPlayerArchive();
	else
		pQuestArchive	=	pQuestManager->GetNpcArchive();

	if( pQuestArchive == NULL )
		return 1;

	SPQuestAttr*	pQuestAttr	=	pQuestArchive->GetQuest( iQuestID );

	if( pQuestAttr == NULL )
		return 1;

	SetActionButton( pQuestAttr->m_iUiShow , pQuestAttr->m_eAcceptType );

	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUESTVIEW , SPIM_QUEST_DETAIL , (WPARAM)m_InstanceID , (LPARAM)iQuestID );

	return 1;
}

int SPWindowQuest::OnAcceptLButtonUp( WPARAM wParam, LPARAM lParam )
{
	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return 1;

	int	iSelectQuestID	=	GetSelectItem();

	if( iSelectQuestID <= 0 )
		return 1;

	pQuestManager->SetAcceptQuest( iSelectQuestID );

	CUT_IN_STATE	eCutInState	=	g_pEventManager->GetCutInState();

	if( m_InstanceID != WIID_NPC_QUEST && eCutInState != CUT_IN_STATE_NULL )
		g_pInterfaceManager->ShowWindow( WIID_QUESTVIEW , false );

	return 1;
}

int SPWindowQuest::OnCompleteLButtonUp( WPARAM wParam, LPARAM lParam )
{
	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return 1;

	int	iSelectQuestID	=	GetSelectItem();

	if( iSelectQuestID <= 0 )
		return 1;

	pQuestManager->SetFinishQuest( iSelectQuestID , g_pInterfaceManager->SPChildWndSendMessage( WIID_QUESTVIEW , SPIM_QUEST_GET_REWARD , NULL , NULL ) );
	g_pInterfaceManager->ShowWindow( WIID_QUESTVIEW , false );

	return 1;
}

int SPWindowQuest::OnGiveUpLButtonUp( WPARAM wParam, LPARAM lParam )
{
	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return 1;

	int	iSelectQuestID	=	GetSelectItem();

	if( iSelectQuestID <= 0 )
		return 1;

	pQuestManager->SetGiveUpQuest( iSelectQuestID );
	g_pInterfaceManager->ShowWindow( WIID_QUESTVIEW , false );

	return 1;
}

int SPWindowQuest::OnShareLButtonUp( WPARAM wParam, LPARAM lParam )
{
	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return 1;

	if( g_pGOBManager != NULL )
	{
		SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer != NULL )
		{
			if( pLocalPlayer->GetTeamActive() == false )
			{
				D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString( 6002001 ) , (LPARAM)&color );
				return 1;
			}
		}
	}

	int	iSelectQuestID	=	GetSelectItem();

	if( iSelectQuestID <= 0 )
		return 1;

	pQuestManager->SendQuestSharePacket( iSelectQuestID );
	return 1;
}

//////////////////////////////////////////////////////////////////////////

void SPWindowQuest::ReposOwnImage()
{
	SetRect( &m_rcBGDest[0]		, m_iAX					, m_iAY					, m_iAX + m_iSX			, m_iAY + 4 );						//	배경 상단
	SetRect( &m_rcBGDest[1]		, m_rcBGDest[0].left	, m_rcBGDest[0].bottom	, m_rcBGDest[0].right	, m_rcBGDest[0].bottom + 264 );		//	배경 중단
	SetRect( &m_rcBGDest[2]		, m_rcBGDest[1].left	, m_rcBGDest[1].bottom	, m_rcBGDest[1].right	, m_rcBGDest[1].bottom + 4 );		//	배경 하단

	SetRect( &m_rcSubBGDest[0]	, m_iAX + 2	, m_iAY + 26 , m_iAX + 2 + 243 , m_iAY + 26 + 19 );												//	옅은 초록 서브 백그라운드
	SetRect( &m_rcSubBGDest[1]	, m_iAX + 2	, m_iAY + 45 , m_iAX + 2 + 243 , m_iAY + 45 + 1 );												//	경계선 1
	SetRect( &m_rcSubBGDest[2]	, m_iAX + 2	, m_iAY + 244 , m_iAX + 2 + 243 , m_iAY + 244 + 1 );											//	경계선 2
}

void SPWindowQuest::ResetCategoryItem()
{
	m_mpCategory.clear();
}

TREEBOX_ITEM* SPWindowQuest::CreateCategoryItem( int iCategory )
{
	STD_MAP_TREEBOX_ITEM::iterator	mIter	=	m_mpCategory.find( iCategory );

	if( mIter != m_mpCategory.end() )
		return	(*mIter).second;

	std::string	strCategory;
	int	iGSItemID;

	if( m_InstanceID == WIID_QUEST )
	{
		iGSItemID	=	GS_CATEGORY_CITYNAME_START;
	}
	else if( m_InstanceID == WIID_NPC_QUEST )
	{
		iGSItemID	=	GC_CATEGORY_QUESTTYPE_START;
	}

	const char*	pszCategory	=	g_pResourceManager->GetGlobalString( iGSItemID + iCategory );

	if( pszCategory == NULL )
		pszCategory	=	g_pResourceManager->GetGlobalString( iGSItemID );

	if( pszCategory == NULL )
		strCategory	=	"No Data";
	else
		strCategory	=	pszCategory;

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_QUEST_TREE_LIST ) );

	if( pTreeBox == NULL )
		return NULL;

	SPTreeBoxItemQuest*	pQuestItem	=	CreateTreeBoxItem( true );

	if( pQuestItem == NULL )
		return NULL;

	pQuestItem->SetText( strCategory.c_str() );
	pQuestItem->SetReturnValue( 0 );

	TREEBOX_ITEM*	pTreeBoxItem	=	pTreeBox->InsertItem( NULL , pQuestItem , iCategory - 1 );

	if( m_InstanceID == WIID_NPC_QUEST )
	{
		if( pTreeBoxItem != NULL )
		{
			pTreeBoxItem->m_bSelect	=	true;
			if( pTreeBoxItem->m_pItemBase != NULL )
				pTreeBoxItem->m_pItemBase->AddStatus( DATA_STATUS_PULLDOWN );
		}
	}

	m_mpCategory.insert( STD_MAP_TREEBOX_ITEM::value_type( iCategory , pTreeBoxItem ) );

	return pTreeBoxItem;
}

bool SPWindowQuest::DeleteCategoryItem( int iCategory )
{
	STD_MAP_TREEBOX_ITEM::iterator	mIter	=	m_mpCategory.find( iCategory );

	if( mIter != m_mpCategory.end() )
	{
		m_mpCategory.erase( mIter );
		return	true;
	}

	return false;
}

TREEBOX_ITEM* SPWindowQuest::FindQuestItem( int iQuestID , int iCategory )
{
	TREEBOX_ITEM*	pItem			=	NULL;
	TREEBOX_ITEM*	pCategoryItem	=	CreateCategoryItem( iCategory );

	if( pCategoryItem == NULL )
		return pItem;

	std::vector< TREEBOX_ITEM* >*			pvpChild		=	&( pCategoryItem->m_vpChild );
	std::vector< TREEBOX_ITEM* >::iterator	Iter			=	pvpChild->begin();
	TREEBOX_ITEM*							pIterItem		=	NULL;
	SPTreeBoxItemQuest*						pIterQuestItem	=	NULL;

	while( Iter != pvpChild->end() )
	{
		pIterItem	=	(*Iter);

		if( pIterItem != NULL )
		{
			pIterQuestItem	=	(SPTreeBoxItemQuest*)( pIterItem->m_pItemBase );

			if( pIterQuestItem != NULL )
			{
				if( pIterQuestItem->GetReturnValue() == iQuestID )
				{
					pItem	=	pIterItem;
					break;
				}

			}
		}
		++Iter;
	}

	return pItem;
}

void SPWindowQuest::SetActionButton( int iAction , int iShare )
{
	SPWindow*	pWindowAccept	=	Find( WIID_QUEST_ACCEPT );
	SPWindow*	pWindowComplete	=	Find( WIID_QUEST_COMPLETE );
	SPWindow*	pWindowGiveUp	=	Find( WIID_QUEST_GIVEUP );
	SPWindow*	pWindowShare	=	Find( WIID_QUEST_SHARE );

	if( pWindowAccept != NULL )
		pWindowAccept->Hide();

	if( pWindowComplete != NULL )
		pWindowComplete->Hide();

	if( pWindowGiveUp != NULL )
		pWindowGiveUp->Hide();

	if( pWindowShare != NULL )
		pWindowShare->Hide();

	bool			bFull			=	false;
	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager != NULL )
	{
		SPQuestArchive*	pQuestArchive	=	pQuestManager->GetPlayerArchive();

		if( pQuestArchive != NULL )
		{
			if( pQuestArchive->GetSize() >= QUEST_MAX_ACTIVE )
				bFull	=	true;
		}
	}

	switch( iAction )
	{
	case QUEST_UI_SHOW_ABLE:
		{
			if( m_InstanceID == WIID_QUEST )
			{
				if( pWindowGiveUp != NULL )
					pWindowGiveUp->Show();
			}
			else if( m_InstanceID == WIID_NPC_QUEST )
			{
				if( pWindowAccept != NULL && bFull == false )
					pWindowAccept->Show();
			}
		}
		break;
	case QUEST_UI_SHOW_COMPLETE:
		{
			if( m_InstanceID == WIID_QUEST )
			{
				if( pWindowGiveUp != NULL )
					pWindowGiveUp->Show();
			}
			else if( m_InstanceID == WIID_NPC_QUEST )
			{
				if( pWindowComplete != NULL )
					pWindowComplete->Show();
			}
		}
		break;
	}

	if( g_pEventManager != NULL )
	{
		CUT_IN_STATE	eCutInState	=	g_pEventManager->GetCutInState();

		if( m_InstanceID == WIID_QUEST )
		{
			if( eCutInState == CUT_IN_STATE_NULL )
			{
				if( pWindowShare != NULL )
				{
					bool	bShareEnable	=	false;

					if( ( iShare & QUEST_ACCEPTTYPE_SHARE ) == QUEST_ACCEPTTYPE_SHARE )
						bShareEnable	=	true;

					pWindowShare->SetEnable( bShareEnable );

					if( iAction == QUEST_UI_SHOW_ABLE || iAction == QUEST_UI_SHOW_COMPLETE )
						pWindowShare->Show();
				}
			}
		}
	}
}

int SPWindowQuest::GetSelectItem()
{
	SPWindowTreeBox*	pTreeWindow	=	(SPWindowTreeBox*)( Find( WIID_QUEST_TREE_LIST ) );

	if( pTreeWindow == NULL )
		return -1;

	TREEBOX_ITEM*	pSelectQuestItem	=	pTreeWindow->GetSelectItem();

	if( pSelectQuestItem == NULL )
		return -1;

	SPTreeBoxItemQuest*	pQuestItem		=	(SPTreeBoxItemQuest*)( pSelectQuestItem->m_pItemBase );

	if( pQuestItem == NULL )
		return -1;

	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return -1;

	SPQuestArchive*	pQuestArchive	=	NULL;

	if( m_InstanceID == WIID_QUEST )
		pQuestArchive	=	pQuestManager->GetPlayerArchive();
	else
		pQuestArchive	=	pQuestManager->GetNpcArchive();

	if( pQuestArchive == NULL )
		return -1;

	int	iSelectQuestID	=	pQuestItem->GetReturnValue();

	if( iSelectQuestID == 0 )
		return -1;

	SPQuestAttr*	pQuestAttr	=	pQuestArchive->GetQuest( iSelectQuestID );

	if( pQuestAttr == NULL )
		return -1;

	return iSelectQuestID;
}

SPTreeBoxItemQuest* SPWindowQuest::CreateTreeBoxItem( bool bMenuType )
{
	SPTreeBoxItemQuest*	pQuestItem	=	new SPTreeBoxItemQuest;

	if( pQuestItem == NULL )
		return pQuestItem;

	pQuestItem->SetWindow( this );
	pQuestItem->SetTextPeriod( true );

	if( bMenuType == true )
	{
		pQuestItem->SetSize( 187 , 12 );
		pQuestItem->SetTextMargin( 20 , 3 );
	}
	else
	{
		pQuestItem->SetSize( 152 , 12 );
        pQuestItem->SetTextMargin( 40 - 12 , 3 );
	}

	for( int j = 0 ; j < DATA_STRING_COUNT ; ++j )
	{
		pQuestItem->SetTextColor( j , m_cStringColor );
	}

	if( bMenuType == true )
	{
		pQuestItem->SetQuestSize( 215 , 19 );
		pQuestItem->SetQuestOffset( 0 , 0 );

		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,				"DATA/INTERFACE/CONCEPT/UI102.PNG" , 96 , 76 , 106 , 86 );
		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,			"DATA/INTERFACE/CONCEPT/UI102.PNG" , 85 , 76 ,  95 , 86 );
	}
	else
	{
		pQuestItem->SetQuestSize( 203 , 19 );
		pQuestItem->SetQuestOffset( 12 , 0 );

		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,				"DATA/INTERFACE/CONCEPT/UI102.PNG" , 115 , 76 , 133 , 94 );
		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,			"DATA/INTERFACE/CONCEPT/UI102.PNG" , 134 , 76 , 152 , 94 );

		if( m_InstanceID == WIID_QUEST )
		{
			pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_ENABLE * 2 ,			"DATA/INTERFACE/CONCEPT/UI102.PNG" , 62 , 121 , 74 , 134 );
			pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_ENABLE * 2 + 1 ,		"DATA/INTERFACE/CONCEPT/UI102.PNG" , 49 , 121 , 61 , 134 );

			pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_MOUSE_OVER * 2 ,		"DATA/INTERFACE/CONCEPT/UI102.PNG" , 62 , 135 , 74 , 148 );
			pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_MOUSE_OVER * 2 + 1 ,	"DATA/INTERFACE/CONCEPT/UI102.PNG" , 49 , 135 , 61 , 148 );

			pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_CLICK * 2 ,				"DATA/INTERFACE/CONCEPT/UI102.PNG" , 62 , 149 , 74 , 162 );
			pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_CLICK * 2 + 1 ,			"DATA/INTERFACE/CONCEPT/UI102.PNG" , 49 , 149 , 61 , 162 );
		}

		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_MOUSE_OVER * 2 ,				"DATA/INTERFACE/CONCEPT/UI100.PNG" , 252 , 368 , 455 , 384 );
	}

	//////////////////////////////////////////////////////////////////////////

	if( bMenuType == true )
	{
		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,			0 , 4 );
		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,		0 , 4 );
	}
	else
	{
		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,			19 - 12 , 1 );
		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,		19 - 12 , 1 );

		if( m_InstanceID == WIID_QUEST )
		{
			pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_ENABLE * 2 ,			196 - 12 , 2 );
			pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_ENABLE * 2 + 1 ,		196 - 12 , 2 );

			pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_MOUSE_OVER * 2 ,		196 - 12 , 2 );
			pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_MOUSE_OVER * 2 + 1 ,	196 - 12 , 2 );

			pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_CLICK * 2 ,			196 - 12 , 2 );
			pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_CLICK * 2 + 1 ,		196 - 12 , 2 );
		}

		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_MOUSE_OVER * 2 ,			12 - 12 , 0 );
	}

	//////////////////////////////////////////////////////////////////////////

	return pQuestItem;
}

SPTreeBoxItemQuest* SPWindowQuest::CreateTreeBoxItem( int iQuestID )
{
	SPTreeBoxItemQuest*	pQuestItem	=	NULL;
	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return pQuestItem;

	SPQuestArchive*	pQuestArchive	=	NULL;

	if( m_InstanceID == WIID_QUEST )
		pQuestArchive	=	pQuestManager->GetPlayerArchive();
	else
		pQuestArchive	=	pQuestManager->GetNpcArchive();

	if( pQuestArchive == NULL )
		return pQuestItem;

	SPQuestAttr*	pQuestAttr	=	pQuestArchive->GetQuest( iQuestID );

	if( pQuestAttr == NULL )
		return pQuestItem;

	pQuestItem	=	CreateTreeBoxItem( false );

	char	szBuf[ MAX_PATH ]	=	{};

	if( pQuestAttr->m_iRewordCount > 1 )
		sprintf( szBuf , "%s (%d)", pQuestAttr->m_strQuestName.c_str() , pQuestAttr->m_iRewordCount );
	else
		sprintf( szBuf , "%s", pQuestAttr->m_strQuestName.c_str() );

	if( pQuestItem != NULL )
	{
		pQuestItem->SetText( szBuf );
		pQuestItem->SetReturnValue( iQuestID );
	}

	return pQuestItem;
}

void SPWindowQuest::ShowFirstQuest( bool bOverWrite )
{
	SPWindow*	pWindowQuestView	=	g_pInterfaceManager->FindWindow( WIID_QUESTVIEW );

	if( pWindowQuestView == NULL )
		return;

	if( pWindowQuestView->IsShow() == true && bOverWrite == false )
		return;

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_QUEST_TREE_LIST ) );

	if( pTreeBox == NULL )
		return;

	TREEBOX_ITEM*	pStartItem	=	pTreeBox->GetRootItem();

	pTreeBox->Reset();

	int	iQuestID	=	FindFirstQuestID( (SPWindow*)pTreeBox , pStartItem );

	if( iQuestID <= 0 )
		return;

	pWindowQuestView->SPSendMessage( SPIM_QUEST_DETAIL , (WPARAM)WIID_NPC_QUEST , (LPARAM)iQuestID );
}

int SPWindowQuest::FindFirstQuestID( SPWindow* pTreeWindow , TREEBOX_ITEM* pStartItem )
{
	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( pTreeWindow );

	if( pTreeBox == NULL || pStartItem == NULL )
		return -1;

	int	iReturnValue;

	if( pStartItem->m_pItemBase != NULL )
	{
		iReturnValue = pStartItem->m_pItemBase->GetReturnValue();

		if( iReturnValue > 0 )
		{
			pTreeBox->SetSelectItem( pStartItem );

			pStartItem->m_pItemBase->SetStatus( DATA_STATUS_SELECT );
			pStartItem->m_bSelect	=	true;

			if( pStartItem->m_bSelect == true && (int)pStartItem->m_vpChild.size() > 0 )
				pStartItem->m_pItemBase->AddStatus( DATA_STATUS_PULLDOWN );

			return iReturnValue;
		}
	}

	std::vector< TREEBOX_ITEM* >::iterator		IterItem	=	pStartItem->m_vpChild.begin();
	TREEBOX_ITEM*								pFindItem	=	NULL;

	while ( IterItem != pStartItem->m_vpChild.end() )
	{
		pFindItem	=	(*IterItem);

		if( pFindItem != NULL)
		{
			iReturnValue	=	FindFirstQuestID( pTreeWindow , pFindItem );

			if( iReturnValue > 0 )
			{
				pStartItem->m_bSelect	=	true;
				return iReturnValue;
			}
		}

		++IterItem;
	}

	return -1;
}
