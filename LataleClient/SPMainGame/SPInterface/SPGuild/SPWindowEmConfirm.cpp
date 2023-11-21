// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD CREATE WINDOW
// Comment     : 
// Creation    : DURAGON 2007-4-04    11:18 
//***************************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowNeedList.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include <algorithm>

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "SPGOBStatus.h"
//
//#include "SPItemCluster.h"

#include "SPResourceManager.h"

#include "SPEventDEF.h"			// for close
#include "SPEventManager.h"
#include "SPLocalizeManager.h"
//#include "SPAbuseFilter.h"

#include "SPUtil.h"
#include "SPDebug.h"

#include "SPGuildLDT.h"
#include "SPGuildArchive.h"
#include "SPWindowEmConfirm.h"


SPWindowBuyConfirm::SPWindowBuyConfirm(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_BUY_CONFIRM , InstanceID, iX, iY, iCX, iCY, pParent )	
, m_pItemInfo(NULL)
, m_pNeedList(NULL)
, m_pBGTexture(NULL)
{
	Init();
}

SPWindowBuyConfirm::~SPWindowBuyConfirm()
{
	Clean();
}

void SPWindowBuyConfirm::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
	m_pNeedList = NULL ;
}


void SPWindowBuyConfirm::Init()
{
	m_iListSize		= 0 ;
	m_strToolTip	=	"[SPWindowBuyConfirm]";

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[0]	, 363 , 308 , 367 , 312 );			//	배경 7
	SetRect( &m_rcBGSrc[1]	, 368 , 308 , 372 , 312 );			//	배경 8
	SetRect( &m_rcBGSrc[2]	, 373 , 308 , 377 , 312 );			//	배경 9
	SetRect( &m_rcBGSrc[3]	, 363 , 313 , 367 , 317 );			//	배경 4
	SetRect( &m_rcBGSrc[4]	, 368 , 313 , 372 , 317 );			//	배경 5
	SetRect( &m_rcBGSrc[5]	, 373 , 313 , 377 , 317 );			//	배경 6
	SetRect( &m_rcBGSrc[6]	, 363 , 318 , 367 , 322 );			//	배경 1
	SetRect( &m_rcBGSrc[7]	, 368 , 318 , 372 , 322 );			//	배경 2
	SetRect( &m_rcBGSrc[8]	, 373 , 318 , 377 , 322 );			//	배경 3
	SetRect( &m_rcBGSrc[9]	, 337 , 199 , 339 , 200 );			//	Line1
	SetRect( &m_rcBGSrc[10]	, 337 , 199 , 339 , 200 );			//	Line2

	SPWindow*	pWindow = NULL ;
	SPWindowStatic* pWindowStatic = NULL ;
	SPWindowButton* pWindowButton = NULL ;

	pWindowStatic = new SPWindowStatic(WIID_BUY_CONFIRM_TITLE, 32, 10, 114, 13, this ) ;
	pWindowStatic->SetFont(FONT_12_BOLD);
	pWindowStatic->SetFormat(DT_TOP | DT_LEFT) ;
	pWindowStatic->Hide() ;
	pWindowStatic = NULL ;

	pWindowStatic = new SPWindowStatic(WIID_BUY_CONFIRM_TITLE_ICON, 17, 10, 17, 13, this ) ;
	pWindowStatic->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 354 , 152 );
	pWindowStatic->Hide() ;
	pWindowStatic = NULL ;

	pWindowStatic = new SPWindowStatic(WIID_BUY_CONFIRM_DESC, 23, 31, 212, 44, this ) ;
	pWindowStatic->SetFont(FONT_12_NORMAL);
	pWindowStatic->SetFormat(DT_TOP | DT_LEFT | DT_WORDBREAK) ;
	pWindowStatic->Hide() ;
	pWindowStatic = NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_BUY_CONFIRM_COMMIT , 65 , 160 , 60 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 187 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 205 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 223 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 123 , 205 );
	pWindowButton->Hide() ;
	pWindowButton = NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_BUY_CONFIRM_CLOSE , 133 , 160 , 60 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 62 , 187 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 62 , 205 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 62 , 223 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 123 , 223 );
	pWindowButton->Hide() ;
	pWindowButton = NULL ;

	m_iListSY = 83 ;
	m_pNeedList = new SPWindowNeedList(WIID_BUY_CONFIRM_NEEDLIST, 2, 83, 256, 92, this ) ;
	m_pNeedList->Hide() ;
}


void SPWindowBuyConfirm::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);
}



void SPWindowBuyConfirm::Hide(bool bSendServer)
{
	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorOut( NULL , NULL );
	}
	KillFocus() ;
}

void SPWindowBuyConfirm::ReposOwnImage()
{
	SetRect( &m_rcBGDst[0]	, m_iAX			, m_iAY			, m_iAX + 4		, m_iAY + 4 );		//	배경 
	SetRect( &m_rcBGDst[1]	, m_iAX + 4		, m_iAY			, m_iAX + 256	, m_iAY + 4 );		//	배경 
	SetRect( &m_rcBGDst[2]	, m_iAX + 256	, m_iAY 		, m_iAX + 260	, m_iAY + 4 );		//	배경 
	SetRect( &m_rcBGDst[3]	, m_iAX 		, m_iAY + 4		, m_iAX + 4		, m_iAY + m_iListSY + m_iListSize + 36 );	//	배경 
	SetRect( &m_rcBGDst[4]	, m_iAX + 4		, m_iAY + 4		, m_iAX + 256	, m_iAY + m_iListSY + m_iListSize + 36 );	//	배경 
	SetRect( &m_rcBGDst[5]	, m_iAX	+ 256	, m_iAY + 4		, m_iAX + 260	, m_iAY + m_iListSY + m_iListSize + 36);	//	배경 
	SetRect( &m_rcBGDst[6]	, m_iAX 		, m_iAY + m_iListSY + m_iListSize + 36	, m_iAX + 4		, m_iAY + m_iListSY + m_iListSize + 40 );	//	배경 
	SetRect( &m_rcBGDst[7]	, m_iAX + 4		, m_iAY + m_iListSY + m_iListSize + 36	, m_iAX + 256	, m_iAY + m_iListSY + m_iListSize + 40 );	//	배경 
	SetRect( &m_rcBGDst[8]	, m_iAX + 256	, m_iAY + m_iListSY + m_iListSize + 36	, m_iAX + 260	, m_iAY + m_iListSY + m_iListSize + 40 );	//	배경 
	
	SetRect( &m_rcBGDst[9]	, m_iAX + 10	, m_iAY + 82	, m_iAX + 250	, m_iAY + 83 );		//	Line1
	SetRect( &m_rcBGDst[10]	, m_iAX + 10	, m_iAY + m_iListSY + m_iListSize + 8	, m_iAX + 250	, m_iAY + m_iListSY + m_iListSize + 9 );	//	Line2
	//SetRect( &m_rcBGDst[14]	, m_iAX + 10	, m_iAY + 175	, m_iAX + 250	, m_iAY + 176 );	//	Line2

	//m_rcBGDst[CONFIRM_BG_IMAGE-1].bottom + 8 + 17 + 6 ;		// Line + Button + Margin
}


void SPWindowBuyConfirm::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}


void SPWindowBuyConfirm::Process(float fTime)
{
	SPWindow::Process( fTime );
}


void SPWindowBuyConfirm::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		RenderBackGround(fTime) ;
	}

	SPWindow::Render(fTime);
}


void SPWindowBuyConfirm::RenderBackGround(float fTime)
{
	int i ;
	for( i = 0 ; i < CONFIRM_BG_IMAGE ; i++ ) {
		m_pBGTexture->SetColor();
		m_pBGTexture->RenderTexture( &m_rcBGDst[i] , &m_rcBGSrc[i] );
	}
	g_pVideo->Flush();
}


bool SPWindowBuyConfirm::SetNeedList(EMBLEM_INFO* pEmblem)
{
	RECT rcSrc ;
	SPWindowNeedList* pNeedList ;
	pNeedList = (SPWindowNeedList*)m_pNeedList ;
	if( pNeedList == NULL )
		return false;

	pNeedList->ClearListItem() ;

	if( pEmblem == NULL )
		return false ;

	m_pItemInfo = (void*)pEmblem ;
	TCHAR szItemText[64] ;
	// Title Setting
	SPWindowStatic* pWindow = (SPWindowStatic*)Find(WIID_BUY_CONFIRM_TITLE) ;
	if( pWindow != NULL ) {
		if( g_pResourceManager->GetGlobalString(27000109) )		{
			_stprintf(szItemText, g_pResourceManager->GetGlobalString(27000109)) ;
			pWindow->SetWindowText(szItemText) ;
		}
		else		{
			_stprintf(szItemText, "Need GString(27000109)") ;
			pWindow->SetWindowText(szItemText) ;
		}
	}
	// Desc
	pWindow = (SPWindowStatic*)Find(WIID_BUY_CONFIRM_DESC) ;
	if( pWindow != NULL ) {
		if( g_pResourceManager->GetGlobalString(27000117) )		{
			_stprintf(szItemText, g_pResourceManager->GetGlobalString(27000117)) ;
			pWindow->SetWindowText(szItemText) ;
		}
		else		{
			_stprintf(szItemText, "Need GString(27000117)") ;
			pWindow->SetWindowText(szItemText) ;
		}
	}
	
	// Data Setting (Item & Ely -> GuildLevel not display)
	if( pEmblem->iConsumeItem != 0 )
		pNeedList->InsertItem(pEmblem->iConsumeItem, pEmblem->iConsumeItemCount) ;
	if( pEmblem->iConsumeEly != 0 )
	{
		if( g_pResourceManager->GetGlobalString(27000099) )
			_stprintf(szItemText, _T("%s %s"), ConvertMoneyToString(pEmblem->iConsumeEly), g_pResourceManager->GetGlobalString(27000099)) ;
		else
			_stprintf(szItemText, _T("%s ely"), ConvertMoneyToString(pEmblem->iConsumeEly)) ;
		SetRect(&rcSrc, 369, 280, 405, 293) ;
		pNeedList->InsertItem("DATA/INTERFACE/CONCEPT/UI105.PNG", rcSrc, szItemText) ;
	}
	return true ;
}



SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowBuyConfirm )

SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP			, WIID_BUY_CONFIRM_CLOSE		,	OnClose		)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP			, WIID_BUY_CONFIRM_COMMIT		,	OnCommit	)

SPIMESSAGE_CONTROL(SPIM_LIST_RESIZE			, WIID_BUY_CONFIRM_NEEDLIST	,	OnListResize	)
// SPIMESSAGE_COMMAND(SPIM_LIST_RESIZE,			OnListResize	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



/**
	레벨업 버튼을 누른경우
	1) 마스터체크 -> 최종레벨체크 -> 아이템 체크 -> 머니체크 -> 길드포인트 체크
*/
int SPWindowBuyConfirm::OnCommit( WPARAM wParam , LPARAM lParam  )
{
	SPWindowNeedList* pNeedList ;
	pNeedList = (SPWindowNeedList*)m_pNeedList ;
	pNeedList->ClearListItem() ;
	SPSendMessageToParent(SPIM_GUILD_EM_CONFIRM_YES, (WPARAM)m_pItemInfo, 0, this) ;
	Hide() ;
	return 1 ;
}

int SPWindowBuyConfirm::OnClose( WPARAM wParam , LPARAM lParam  )
{
	SPWindowNeedList* pNeedList ;
	pNeedList = (SPWindowNeedList*)m_pNeedList ;
	pNeedList->ClearListItem() ;
	SPSendMessageToParent(SPIM_GUILD_EM_CONFIRM_NO, (WPARAM)m_pItemInfo, 0, this) ;
	Hide() ;
	return 1 ;
}

int SPWindowBuyConfirm::OnListResize( WPARAM wParam , LPARAM lParam  )
{
	m_iListSize = (int)lParam ;

	SetRect( &m_rcBGDst[CONFIRM_BG_IMAGE-1]	, m_iAX + 10	, m_iAY + m_iListSY + m_iListSize + 8	, m_iAX + 250	, m_iAY + m_iListSY + m_iListSize + 9 );	//	Line2

	SPWindowButton* pWindowButton = NULL ;
	pWindowButton = (SPWindowButton*)Find(WIID_BUY_CONFIRM_COMMIT) ;
	if( pWindowButton != NULL )
		pWindowButton->AdjustCoord(66, m_rcBGDst[CONFIRM_BG_IMAGE-1].bottom - m_iAY + 8 ) ;
	pWindowButton = (SPWindowButton*)Find(WIID_BUY_CONFIRM_CLOSE) ;
	if( pWindowButton != NULL )
		pWindowButton->AdjustCoord(134, m_rcBGDst[CONFIRM_BG_IMAGE-1].bottom - m_iAY + 8 ) ;

	// Window Resize (Default 260, 211)
	int iSizeY = m_rcBGDst[CONFIRM_BG_IMAGE-1].bottom + 8 + 17 + 6 ;		// Line + Button + Margin
	SetRectSize(260, iSizeY) ;
	ReposOwnImage() ;

	return 1 ;
}


