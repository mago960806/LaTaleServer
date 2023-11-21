// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD Destroy WINDOW
// Comment     : 
// Creation    : DURAGON 2007-4-04    11:18 
//***************************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "SPGOBStatus.h"

#include "SPEventDEF.h"			// for close
#include "SPEventManager.h"

#include "PacketID.h"

#include "SPGuildArchive.h"
#include "SPWindowGuildDestroy.h"

SPWindowGuildDestroy::SPWindowGuildDestroy(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_GUILD_DESTROY , InstanceID, iX, iY, iCX, iCY, pParent )	
{
	Init();
}

SPWindowGuildDestroy::~SPWindowGuildDestroy()
{
	Clean();
}

void SPWindowGuildDestroy::Init()
{
	m_strToolTip	=	"[SPWindowGuildDestroy]";

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[0]	, 482 , 346 , 486 , 350 );			//	배경 7
	SetRect( &m_rcBGSrc[1]	, 488 , 346 , 490 , 350 );			//	배경 8
	SetRect( &m_rcBGSrc[2]	, 492 , 346 , 496 , 350 );			//	배경 9
	SetRect( &m_rcBGSrc[3]	, 482 , 352 , 486 , 354 );			//	배경 4
	SetRect( &m_rcBGSrc[4]	, 488 , 352 , 490 , 354 );			//	배경 5
	SetRect( &m_rcBGSrc[5]	, 492 , 352 , 496 , 354 );			//	배경 6
	SetRect( &m_rcBGSrc[6]	, 482 , 341 , 486 , 345 );			//	배경 1
	SetRect( &m_rcBGSrc[7]	, 488 , 341 , 490 , 345 );			//	배경 2
	SetRect( &m_rcBGSrc[8]	, 492 , 341 , 496 , 345 );			//	배경 3

	SetRect( &m_rcBGSrc[9]	, 101 , 75  , 238 , 94 );			//	Title
	SetRect( &m_rcBGSrc[10]	, 488 , 346 , 490 , 347 );			//	Line
	SetRect( &m_rcBGSrc[11]	, 488 , 337 , 490 , 339 );			//	SubBG


	SPWindowButton*	pWindowButton	=	NULL;
	SPWindowStatic* pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_DESTROYGUILD_ICON , 10 , 49 , 37 , 37 , this );
	pWindowStatic->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 101 , 161 );
	pWindowStatic->Hide() ;
	pWindowStatic	= NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_DESTROYGUILD_TEXT , 40 , 47 , 213 , 42 , this );
	//pWindowStatic->SetFormat(DT_VCENTER | DT_LEFT | DT_WORDBREAK);
	pWindowStatic->SetFormat(DT_TOP | DT_LEFT | DT_WORDBREAK);
	pWindowStatic->SetFontColor(RGBA(180, 63, 81, 255));
	pWindowStatic->SetMultiLine(true, 3);
	pWindowStatic->Hide() ;
	pWindowStatic	= NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_DESTROYGUILD_COMMIT , 64 , 106 , 68 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 296 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 314 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 332 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 350 );
	pWindowButton->Hide() ;
	pWindowButton = NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_DESTROYGUILD_CANCEL , 140 , 106 , 68 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 415 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 415 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 415 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 415 , 494 );
	pWindowButton->Hide() ;
	pWindowButton = NULL ;

}

void SPWindowGuildDestroy::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
}

void SPWindowGuildDestroy::Show(bool bWithChild)
{
	int iRet ;
	SPWindowStatic* pWindowStatic = NULL ;
	
	if( ( iRet = g_pGOBManager->GetGuildArchive()->CheckGuildDestroy() ) != 0 )
	{
		if( iRet == 1 )
		{
			if( g_pResourceManager->GetGlobalString(27000060))		// Not Guild Member
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000060) );
		}
		else if( iRet == 2 )
		{
			if( g_pResourceManager->GetGlobalString(27000006) )		// Not Master
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000006) );
		}
		
		if( g_pInterfaceManager->IsCutIn() )
			g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	}
	else
	{
		SPWindow::Show(bWithChild);
		pWindowStatic = (SPWindowStatic*)Find(WIID_DESTROYGUILD_TEXT, true) ;
		pWindowStatic->SetWindowText(g_pResourceManager->GetGlobalString(27000032)) ;		// Destroy
	}

	//int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	//int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	//if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	//{
	//	OnCursorEnter( NULL , NULL );
	//}
}

void SPWindowGuildDestroy::Hide(bool bSendServer)
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

void SPWindowGuildDestroy::ReposOwnImage()
{
	//54 , 47 , 213(267) , 42(89)
	SetRect( &m_rcBGDest[0]	, m_iAX			, m_iAY			, m_iAX + 4		, m_iAY + 4  );		//	BG 7
	SetRect( &m_rcBGDest[1]	, m_iAX + 4		, m_iAY 		, m_iAX + 268	, m_iAY + 4 );		//	BG 8
	SetRect( &m_rcBGDest[2]	, m_iAX	+ 268	, m_iAY			, m_iAX + 272	, m_iAY + 4 );		//	BG 9
	SetRect( &m_rcBGDest[3]	, m_iAX			, m_iAY + 4		, m_iAX + 4		, m_iAY + 129 );	//	BG 4
	SetRect( &m_rcBGDest[4]	, m_iAX	+4		, m_iAY + 4 	, m_iAX + 268	, m_iAY + 129 );	//	BG 5
	SetRect( &m_rcBGDest[5]	, m_iAX	+ 268	, m_iAY + 4		, m_iAX + 272	, m_iAY + 129 );	//	BG 6
	SetRect( &m_rcBGDest[6]	, m_iAX			, m_iAY + 129	, m_iAX + 4		, m_iAY + 133 );	//	BG 1
	SetRect( &m_rcBGDest[7]	, m_iAX	+ 4		, m_iAY + 129	, m_iAX + 268	, m_iAY + 133 );	//	BG 2
	SetRect( &m_rcBGDest[8]	, m_iAX	+ 268	, m_iAY	+ 129	, m_iAX + 272	, m_iAY + 133 );	//	BG 3

	SetRect( &m_rcBGDest[9]	, m_iAX	+ 9		, m_iAY	+ 5		, m_iAX + 146	, m_iAY + 24 );		//	Title
	SetRect( &m_rcBGDest[10], m_iAX	+ 2		, m_iAY + 26	, m_iAX + 270	, m_iAY + 27 );		//	Line
	SetRect( &m_rcBGDest[11], m_iAX	+ 2		, m_iAY	+ 27	, m_iAX + 270	, m_iAY + 129 );	//	SubBG
}

void SPWindowGuildDestroy::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowGuildDestroy::Process(float fTime)
{
	SPWindow::Process( fTime );
}

void SPWindowGuildDestroy::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < GUILD_DESTROY_BG_COUNT ; i++ )
		{
			m_pBGTexture->SetColor();
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	//SetRect( &m_rcWindowRect, m_iAX + 54	, m_iAY + 47	, m_iAX + 267	, m_iAY + 89 ) ;
	//g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	//g_pVideo->GetSysLine()->DrawRect(m_rcWindowRect);

	SPWindow::Render(fTime);
}


SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowGuildDestroy )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP		,	WIID_DESTROYGUILD_COMMIT			,	OnDestroyOK		)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP		,	WIID_DESTROYGUILD_CANCEL			,	OnCancel		)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)




/**
	Destroy
*/
int SPWindowGuildDestroy::OnDestroyOK( WPARAM wParam, LPARAM lParam )
{
	// Modal Window Show
	g_pGOBManager->GetGuildArchive()->SendGuildDestroy() ;
	if( g_pInterfaceManager->IsCutIn() )
		g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	return 1;
}

int SPWindowGuildDestroy::OnCancel( WPARAM wParam , LPARAM lParam )
{
	if( g_pInterfaceManager->IsCutIn() )
		g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	return 1;
}

