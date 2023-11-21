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
#include "SPWindowGuildLevelup.h"


SPWindowGuildLevelup::SPWindowGuildLevelup(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_GUILD_LEVELUP , InstanceID, iX, iY, iCX, iCY, pParent )	
{
	Init();
}

SPWindowGuildLevelup::~SPWindowGuildLevelup()
{
	Clean();
}

void SPWindowGuildLevelup::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
	m_pNeedList = NULL ;
}


void SPWindowGuildLevelup::Init()
{
	m_pNeedList		= NULL ;
	m_iListSize		= 0 ;
	m_strToolTip	=	"[SPWindowGuildLevelup]";

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
	SetRect( &m_rcBGSrc[9]	, 336 , 152 , 353 , 165 );			//	Icon
	SetRect( &m_rcBGSrc[10]	, 337 , 199 , 339 , 200 );			//	Line1
	SetRect( &m_rcBGSrc[11]	, 337 , 199 , 339 , 200 );			//	Line2

	SPWindow*	pWindow = NULL ;
	SPWindowStatic* pWindowStatic = NULL ;
	SPWindowButton* pWindowButton = NULL ;

	pWindowStatic = new SPWindowStatic(WIID_GUILD_LEVELUP_TITLE, 39, 11, 211, 12, this ) ;
	// pWindowStatic->SetWindowText("TESTESTESTES!!") ;
	pWindowStatic->Hide() ;
	pWindowStatic = NULL ;

	pWindowStatic = new SPWindowStatic(WIID_GUILD_LEVELUP_DESC, 23, 31, 212, 44, this ) ;
	pWindowStatic->Hide() ;
	pWindowStatic = NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_LEVELUP_COMMIT , 66 , 184 , 60 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 187 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 205 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 223 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 123 , 205 );
	pWindowButton->Hide() ;
	pWindowButton = NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILD_LEVELUP_CLOSE , 134 , 184 , 60 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 62 , 187 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 62 , 205 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 62 , 223 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 123 , 223 );
	pWindowButton->Hide() ;
	pWindowButton = NULL ;

	m_iListSY = 83 ;
	m_pNeedList = new SPWindowNeedList(WIID_GUILD_LEVELUP_NEEDLIST, 2, 83, 256, 92, this ) ;
	m_pNeedList->Hide() ;
}


void SPWindowGuildLevelup::Show(bool bWithChild)
{
	// GuildMaster Only
	if( !g_pGOBManager->GetGuildArchive()->GetBasicAvailable() || !SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) || !g_pGOBManager->GetGuildArchive()->IsMaster() )
	{
		if( g_pResourceManager->GetGlobalString(27000006) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000006) );
		OnClose(0, 0) ;
		return ;
	}

	if( g_pGOBManager->GetGuildArchive()->GetGuildLevel() >= g_pkGuildManager->GetLimitGuildLevel() )		// Max Level -> 더이상 올릴수 없음
	{
		if( g_pResourceManager->GetGlobalString(27000082) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000082) );
		OnClose(0, 0) ;
		return ;
	}

	SetLevelUpNeedList() ;

	SPWindow::Show(bWithChild);
}



void SPWindowGuildLevelup::Hide(bool bSendServer)
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

void SPWindowGuildLevelup::ReposOwnImage()
{
	SetRect( &m_rcBGDst[0]	, m_iAX			, m_iAY			, m_iAX + 4		, m_iAY + 4 );		//	배경 
	SetRect( &m_rcBGDst[1]	, m_iAX + 4		, m_iAY			, m_iAX + 256	, m_iAY + 4 );		//	배경 
	SetRect( &m_rcBGDst[2]	, m_iAX + 256	, m_iAY 		, m_iAX + 260	, m_iAY + 4 );		//	배경 
	SetRect( &m_rcBGDst[3]	, m_iAX 		, m_iAY + 4		, m_iAX + 4		, m_iAY + 207 );	//	배경 
	SetRect( &m_rcBGDst[4]	, m_iAX + 4		, m_iAY + 4		, m_iAX + 256	, m_iAY + 207 );	//	배경 
	SetRect( &m_rcBGDst[5]	, m_iAX	+ 256	, m_iAY + 4		, m_iAX + 260	, m_iAY + 207 );	//	배경 
	SetRect( &m_rcBGDst[6]	, m_iAX 		, m_iAY + 207	, m_iAX + 4		, m_iAY + 211 );	//	배경 
	SetRect( &m_rcBGDst[7]	, m_iAX + 4		, m_iAY + 207	, m_iAX + 256	, m_iAY + 211 );	//	배경 
	SetRect( &m_rcBGDst[8]	, m_iAX + 256	, m_iAY + 207	, m_iAX + 260	, m_iAY + 211 );	//	배경 
	
	SetRect( &m_rcBGDst[9]	, m_iAX + 15	, m_iAY + 10	, m_iAX + 32	, m_iAY + 23 );		//	Icon
	SetRect( &m_rcBGDst[10]	, m_iAX + 10	, m_iAY + 82	, m_iAX + 250	, m_iAY + 83 );		//	Line1
	SetRect( &m_rcBGDst[11]	, m_iAX + 10	, m_iAY + m_iListSY + m_iListSize + 8	, m_iAX + 250	, m_iAY + m_iListSY + m_iListSize + 9 );	//	Line2
}


void SPWindowGuildLevelup::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}


void SPWindowGuildLevelup::Process(float fTime)
{
	SPWindow::Process( fTime );
}


void SPWindowGuildLevelup::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		RenderBackGround(fTime) ;
	}

	SPWindow::Render(fTime);
}


void SPWindowGuildLevelup::RenderBackGround(float fTime)
{
	int i ;
	for( i = 0 ; i < GUILD_LEVELUP_BG_IMG ; i++ ) {
		m_pBGTexture->SetColor();
		m_pBGTexture->RenderTexture( &m_rcBGDst[i] , &m_rcBGSrc[i] );
	}
	g_pVideo->Flush();
}


void SPWindowGuildLevelup::SetLevelUpNeedList()
{
	RECT rcSrc ;
	SPWindowNeedList* pNeedList ;
	pNeedList = (SPWindowNeedList*)m_pNeedList ;
	if( pNeedList == NULL )
		return ;

	pNeedList->ClearListItem() ;

	int iGType = g_pGOBManager->GetGuildArchive()->GetGuildProperty() ;
	int iGLevel = g_pGOBManager->GetGuildArchive()->GetGuildLevel() ;
	int iNestKey = (iGLevel+1)*10 + iGType ;
	PGUILD_ATTR pGuildData = NULL ;
	TCHAR szItemText[256] ;

	// Title Setting
	SPWindowStatic* pWindow = (SPWindowStatic*)Find(WIID_GUILD_LEVELUP_TITLE) ;
	if( pWindow != NULL ) {
		if( g_pResourceManager->GetGlobalString(27000105) )
		{
			_stprintf(szItemText, g_pResourceManager->GetGlobalString(27000105), iGLevel, iGLevel+1 ) ;
			pWindow->SetFont(FONT_12_BOLD);
			pWindow->SetFormat(DT_TOP | DT_LEFT) ;
			pWindow->SetWindowText(szItemText) ;
		}
	}
	// Desc
	pWindow = (SPWindowStatic*)Find(WIID_GUILD_LEVELUP_DESC) ;
	if( pWindow != NULL ) {
		if( g_pResourceManager->GetGlobalString(27000110) )
		{
			_stprintf(szItemText, g_pResourceManager->GetGlobalString(27000110)) ;
			pWindow->SetFont(FONT_12_NORMAL);
			pWindow->SetFormat(DT_TOP | DT_LEFT | DT_WORDBREAK) ;
			pWindow->SetWindowText(szItemText) ;
		}
	}
	
	// Data Setting
	pGuildData = g_pkGuildManager->FindGuildAttribute(iNestKey) ;
	if( pGuildData == NULL )
		return ;

	if( pGuildData->iCreateItemID != 0 )
		pNeedList->InsertItem(pGuildData->iCreateItemID, pGuildData->iCreateItemCount) ;
	if( pGuildData->lCreateMoney != 0 )
	{
		if( g_pResourceManager->GetGlobalString(27000099) )
			_stprintf(szItemText, _T("%s %s"), ConvertMoneyToString(pGuildData->lCreateMoney), g_pResourceManager->GetGlobalString(27000099)) ;
		else
			_stprintf(szItemText, _T("%s ely"), ConvertMoneyToString(pGuildData->lCreateMoney)) ;
		SetRect(&rcSrc, 369, 280, 405, 293) ;
		pNeedList->InsertItem("DATA/INTERFACE/CONCEPT/UI105.PNG", rcSrc, szItemText, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)) ;
	}
	if( pGuildData->iPointLimit != 0 )
	{
		if( g_pResourceManager->GetGlobalString(27000100) )
			_stprintf(szItemText, _T("%s %s"), ConvertMoneyToString(pGuildData->iPointLimit), g_pResourceManager->GetGlobalString(27000100)) ;
		else
			_stprintf(szItemText, _T("%s GPoint"), ConvertMoneyToString(pGuildData->iPointLimit)) ;
		SetRect(&rcSrc, 369, 266, 405, 279) ;
		pNeedList->InsertItem("DATA/INTERFACE/CONCEPT/UI105.PNG", rcSrc, szItemText, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)) ;
	}
}



SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowGuildLevelup )

SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP			, WIID_GUILD_LEVELUP_CLOSE		,	OnClose		)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP			, WIID_GUILD_LEVELUP_COMMIT		,	OnCommit	)

SPIMESSAGE_CONTROL(SPIM_LIST_RESIZE			, WIID_GUILD_LEVELUP_NEEDLIST	,	OnListResize	)
//SPIMESSAGE_COMMAND(SPIM_LIST_RESIZE,			OnListResize	)
SPIMESSAGE_COMMAND(SPIM_GUILD_LEVELUP_RESULT,	OnLevelupResult	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



/**
	레벨업 버튼을 누른경우
	1) 마스터체크 -> 최종레벨체크 -> 아이템 체크 -> 머니체크 -> 길드포인트 체크
*/
int SPWindowGuildLevelup::OnCommit( WPARAM wParam , LPARAM lParam  )
{
	int iRet ;
	int iRetValue = 0;
	TCHAR szTemp[512] ;

	if( (iRet = g_pGOBManager->GetGuildArchive()->CheckGuildLevelup(iRetValue)) == 0)
	{
		// CheckGuildLevelup에서 PacketSend
		OnClose(0, 0) ;
	}
	else
	{
		switch( iRet)
		{
			case ERROR_GUILD_CREATE_LDT:
			case ERROR_GUILD_CREATE_MONEY:		// 제한 2 (GuildLevel)
				if( g_pResourceManager->GetGlobalString(27000082) )
					sprintf(szTemp, g_pResourceManager->GetGlobalString(27000082));
				else
					sprintf(szTemp, "It is Max-Level. Can't find Next-Level infomation");
				break ;

			case ERROR_GUILD_CREATE_ITEM:		// 제한 3 (아이템)
				if( g_pResourceManager->GetGlobalString(27000080) )
					sprintf(szTemp, g_pResourceManager->GetGlobalString(27000080));
				else
					sprintf(szTemp, "You need Item[%d] for guild level up", iRetValue);
				break ;

			case ERROR_GUILD_CREATE_REQ4:		// 제한 4 (Money)
				if( g_pResourceManager->GetGlobalString(27000005) )
					sprintf(szTemp, g_pResourceManager->GetGlobalString(27000005));
				else
					sprintf(szTemp, "You Need more Money(Ely)");
				break ;

			case ERROR_PKTSEND_FAIL:				// 길드 포인트 오류
				if( g_pResourceManager->GetGlobalString(27000081) )
					sprintf(szTemp, g_pResourceManager->GetGlobalString(27000081));
				else
					sprintf(szTemp, "You Need more GuildPoint[%d]", iRetValue);
				break ;
				
			default:
				sprintf(szTemp, "Guild Level-Up Fail[%d]", iRet);
		}
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
		OnClose(0, 0) ;
	}
	return 1 ;
}

int SPWindowGuildLevelup::OnClose( WPARAM wParam , LPARAM lParam  )
{
	SPWindowNeedList* pNeedList ;
	pNeedList = (SPWindowNeedList*)m_pNeedList ;
	pNeedList->ClearListItem() ;

	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	return 1 ;
}

int SPWindowGuildLevelup::OnListResize( WPARAM wParam , LPARAM lParam  )
{
	m_iListSize = (int)lParam ;

	SetRect( &m_rcBGDst[GUILD_LEVELUP_BG_IMG-1]	, m_iAX + 10	, m_iAY + m_iListSY + m_iListSize + 8	, m_iAX + 250	, m_iAY + m_iListSY + m_iListSize + 9 );	//	Line2

	SPWindowButton* pWindowButton = NULL ;
	pWindowButton = (SPWindowButton*)Find(WIID_GUILD_LEVELUP_COMMIT) ;
	if( pWindowButton != NULL )
		pWindowButton->AdjustCoord(66, m_rcBGDst[GUILD_LEVELUP_BG_IMG-1].bottom - m_iAY + 8 ) ;
	pWindowButton = (SPWindowButton*)Find(WIID_GUILD_LEVELUP_CLOSE) ;
	if( pWindowButton != NULL )
		pWindowButton->AdjustCoord(134, m_rcBGDst[GUILD_LEVELUP_BG_IMG-1].bottom - m_iAY + 8 ) ;

	// Window Resize (Default 260, 211)
	int iSizeY = m_rcBGDst[GUILD_LEVELUP_BG_IMG-1].bottom + 8 + 17 + 6 ;		// Line + Button + Margin
	SetRectSize(260, iSizeY) ;

	return 1 ;
}


int SPWindowGuildLevelup::OnLevelupResult( WPARAM wParam , LPARAM lParam  )
{
	char szTemp[512] ;
	int iReault = (int)wParam ;

	if( iReault == 0 )		// Success
	{
		sprintf(szTemp, g_pResourceManager->GetGlobalString(27000029));
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
		OnClose(0, 0) ;
	}
	else		// Create Fail
	{
		if( g_pResourceManager->GetGlobalString(iReault) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(iReault) );
		else	{
			sprintf(szTemp, "Guild levelup Fail[%d]", iReault);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
		}
	}
	return 1 ;
}


