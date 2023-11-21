// ***************************************************************
//  SPWindowFindMessageOne   version:  1.0   ·  date: 07/09/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindowListDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEditMultiLine.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPGOBClusterDef.h"
#include "SPGameObject.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"
#include "SPAvatarModelUnitDef.h"

#include "SPGOBManager.h"
#include "SPPlayer.h"
#include "SPPlayerInvenArchive.h"
#include "SPTimer.h"

#include "SPWindowFindMessageOne.h"

SPWindowFindMessageOne::SPWindowFindMessageOne( WND_ID WndClassID , INSTANCE_ID InstanceID )
: SPWindow( WndClassID , InstanceID )
{	
	Init();	
}

SPWindowFindMessageOne::SPWindowFindMessageOne( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent /* = NULL  */ )
: SPWindow( WNDID_FIND_MESSAGE , InstanceID , iX , iY , iCX , iCY , pParent )
{	
	Init();	
}

SPWindowFindMessageOne::~SPWindowFindMessageOne()
{
	Clean();
}

int	SPWindowFindMessageOne::timer = 0;
int	SPWindowFindMessageOne::ShowTime = 5;
//------------------------------------------------------------------------------------------------------
void SPWindowFindMessageOne::Init()
{
	m_strToolTip	= "[SPWindowFindMessageOne]";

	SPWindowButton* pButton			=	NULL;
	SPWindow*		pWindow			=	NULL;
	SPWindow*		pWindowParent	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	SPWindow*		pWindowMinimized	=	new SPWindowStatic( WIID_FIND_MESSAGE_ONE_MINIMIZE , 0 , 0 , 28 , 18 , this );

	pWindowParent	= new SPWindowStatic( WIID_FIND_MESSAGE_ONE_MINIMIZE_BG , 0 , 0 , 28 , 18 , pWindowMinimized );
	pWindowParent->SetImage		("DATA/INTERFACE/CONCEPT/UI106.PNG", 331 , 16 );

	pButton = new SPWindowButton( WIID_FIND_MESSAGE_ONE_MINIMIZE_TO_MAXIMIZE , 12 , 2 , 14 , 14 , pWindowParent );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI106.PNG", 304 , 222 );
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI106.PNG", 319 , 222 );
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI106.PNG", 304 , 237 );
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI106.PNG", 319 , 237 );

	//////////////////////////////////////////////////////////////////////////

	SPWindow*		pWindowMaxinized	=	new SPWindowStatic( WIID_FIND_MESSAGE_ONE_MAXIMIZE , 0 , 0 , 286-90 , 80 , this );

	//////////////////////////////////////////////////////////////////////////

	pWindow	= new SPWindowStatic( WIID_FIND_MESSAGE_ONE_MAXIMIZE_TITLE_LEFT , 0 , 0 , 3 , 18 , pWindowMaxinized );
	pWindow->SetImage		("DATA/INTERFACE/CONCEPT/UI107.PNG", 501 , 164 );

	pWindow	= new SPWindowStatic( WIID_FIND_MESSAGE_ONE_MAXIMIZE_TITLE_RIGHT , 283 - 90 , 0 , 3 , 18 , pWindowMaxinized );
	pWindow->SetImage		("DATA/INTERFACE/CONCEPT/UI107.PNG", 508 , 164 );

	pWindowParent	= new SPWindowStatic( WIID_FIND_MESSAGE_ONE_MAXIMIZE_TITLE_CENTER , 3 , 0 , 280-90 , 18 , pWindowMaxinized );
	pWindowParent->SetImage		("DATA/INTERFACE/CONCEPT/UI107.PNG", 505 , 164 );
	pWindowParent->SetSrcSize( 2 , 18 );

	//////////////////////////////////////////////////////////////////////////

	pWindow	= new SPWindowStatic( WIID_FIND_MESSAGE_ONE_MAXIMIZE_TITLE_IMG , 6 - 3 , 7 , 80 , 5 , pWindowParent );
	pWindow->SetImage		("DATA/INTERFACE/CONCEPT/UI106.PNG", 171 , 207 );

	pButton = new SPWindowButton( WIID_FIND_MESSAGE_ONE_MAXIMIZE_TO_MINIMIZE , 268 - 3 - 90, 2 , 14 , 14 , pWindowParent );
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI106.PNG", 301 , 16 );
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI106.PNG", 316 , 16 );
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI106.PNG", 301 , 31 );
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI106.PNG", 316 , 31 );

	//////////////////////////////////////////////////////////////////////////

	pWindowParent	= new SPWindowStatic( WIID_FIND_MESSAGE_ONE_MAXIMIZE_WINDOW_TOP , 0 , 18 , 286-90 , 59 , pWindowMaxinized );
	pWindowParent->SetImage		("DATA/INTERFACE/CONCEPT/UI107.PNG", 186 , 2 );
	pWindowParent->SetSrcSize( 286 , 3 );

	pWindow	= new SPWindowStatic( WIID_FIND_MESSAGE_ONE_MAXIMIZE_WINDOW_BOTTOM , 0 , 77 , 286-90 , 3 , pWindowMaxinized );
	pWindow->SetImage			("DATA/INTERFACE/CONCEPT/UI107.PNG", 186 , 10 );
	//////////////////////////////////////////////////////////////////////////
	m_pEditMultiLine = new SPWindowEditMultiLine( WIID_FIND_MESSAGE_ONE_TEXT_BOX , 15-5 , 22 - 18 , 255-90+15 , 45 , pWindowParent );
	m_pEditMultiLine->SetFontEdge(true);
	m_pEditMultiLine->SetFontColor( D3DXCOLOR( 1.0f,  1.0f , 1.0f , 1.0f ) );
	m_pEditMultiLine->SetMultiLine( TRUE , 2 );
	m_pEditMultiLine->SetEnable(false);
	//////////////////////////////////////////////////////////////////////////

	m_iInitPosX				=	m_iAX;
	m_iInitPosY				=	m_iAY;
	m_iID					=	0;

	m_fLeavingTime			=	0;
	m_bSendMessage			=	false;

	m_pFindTimer			=	new SPTimer;
	m_pFindTimer->Clear();
	m_pFindTimer->SetExpired();
	//////////////////////////////////////////////////////////////////////////

	UpdateWindow( false );
}

//------------------------------------------------------------------------------------------------------
void SPWindowFindMessageOne::Clean()
{
	SPWindow::Clean();
}

//------------------------------------------------------------------------------------------------------
void SPWindowFindMessageOne::Process(float fTime)
{	
	if( !m_bShow )	
	{
		return;
	}
	if ( m_pFindTimer->IsExpired() )
	{
		timer = 0;
		ShowTime = 5;
		OutputDebugString("you can not show\n");
		Hide();
		return;
	}

	if ( m_pFindTimer->IsEnable() )
	{	
		if ( m_pFindTimer->CheckTimerEx( fTime ) == false )
		{
			if( 0 == timer)
			{	
				m_pEditMultiLine->SetWindowText( "正在查询中...\n目前没有相应位置信息" );
				OutputDebugString("SendMessage first time\n");
				SendSearchName();					//发出搜索消息
				timer++;
			}
			if ( ShowTime < m_pFindTimer->GetLimitTime() )
			{
				if ( m_pFindTimer->GetShowTime() >= ShowTime )
				{
					if ( ShowTime/5 == timer )
					{
						if( m_bSendMessage )
						{
							SendSearchName();
							//m_bSendMessage = false;
							OutputDebugString("It's time to SendMessage\n");
						}

						ShowTime = ShowTime + 5;
						timer++;
					}

				}
			}
		}
		else
		{
			timer = 0;
			ShowTime = 5;
			m_bSendMessage = false;
			OutputDebugString("Time out\n");
			Hide();
			return;
		}
	}
	SPWindow::Process( fTime );
}

//------------------------------------------------------------------------------------------------------
void SPWindowFindMessageOne::Render(float fTime)
{
	if( !m_bShow )	return;

	g_pVideo->Flush();

	SPWindow::Render( fTime );

	g_pVideo->Flush();
}

//------------------------------------------------------------------------------------------------------
void SPWindowFindMessageOne::Show(bool bWithChild)
{	
	m_bMinimized = false;

	SPWindow::Show( bWithChild );

	UpdateWindow( m_bMinimized );
}

//------------------------------------------------------------------------------------------------------
void SPWindowFindMessageOne::SetMinimized( bool bMinimized )
{
	UpdateWindow( bMinimized );
}

//------------------------------------------------------------------------------------------------------
bool SPWindowFindMessageOne::GetMinimized()
{
	return m_bMinimized;
}

//------------------------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowFindMessageOne )
SPIMESSAGE_COMMAND(	SPIM_PURGE																	,	OnPurge					)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FIND_MESSAGE_ONE_MINIMIZE_TO_MAXIMIZE			,	OnMininize				)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_FIND_MESSAGE_ONE_MAXIMIZE_TO_MINIMIZE			,	OnMaximize				)

SPIMESSAGE_OBJ_MAP_END(	SPWindow )

//------------------------------------------------------------------------------------------------------
int SPWindowFindMessageOne::OnMininize( WPARAM wParam, LPARAM lParam)
{
	UpdateWindow( false );

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFindMessageOne::OnMaximize( WPARAM wParam, LPARAM lParam)
{
	if( g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL )
		UpdateWindow( true );

	return 1;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFindMessageOne::OnPurge( WPARAM wParam, LPARAM lParam )
{
	SetAbsCoord( m_iInitPosX , m_iInitPosY );
	RefreshChildPos();
	UpdateWindow( false );
	return 1;
}

//------------------------------------------------------------------------------------------------------
void SPWindowFindMessageOne::UpdateWindow( bool bMinimized )
{
	SPWindow*	pWindowMinimize	=	Find( WIID_FIND_MESSAGE_ONE_MINIMIZE );
	SPWindow*	pWindowMaximize	=	Find( WIID_FIND_MESSAGE_ONE_MAXIMIZE );

	if( pWindowMinimize == NULL || pWindowMaximize == NULL )
		return;

	m_bMinimized = bMinimized;
	RECT	rcSize;

	if( m_bMinimized == true )
	{
		pWindowMinimize->Show();
		pWindowMaximize->Hide();

		pWindowMinimize->GetWindowRect( rcSize );
	}
	else
	{
		pWindowMaximize->Show();
		pWindowMinimize->Hide();

		pWindowMaximize->GetWindowRect( rcSize );
	}

	SetRectSize( rcSize.right - rcSize.left , rcSize.bottom - rcSize.top );

}

//------------------------------------------------------------------------------------------------------
int	SPWindowFindMessageOne::setFindName( std::string Name )
{
	m_strFindName = Name;

	return 1;
}

//------------------------------------------------------------------------------------------------------
std::string	SPWindowFindMessageOne::getFindName()
{
	return m_strFindName;
}

//------------------------------------------------------------------------------------------------------
int SPWindowFindMessageOne::SendSearchName()
{
	if(g_pGOBManager->GetLocalPlayer()) 
	{
		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();

		if(pInven)
		{
			pInven->SendWhere( m_strFindName, m_iID, WINDOW_TWO );
		}
	}

	return 1;
}
//------------------------------------------------------------------------------------------------------
void SPWindowFindMessageOne::setID( int iID )
{
	m_iID = iID;
}

int SPWindowFindMessageOne::getID()
{
	return m_iID;
}

//------------------------------------------------------------------------------------------------------
SPWindowEditMultiLine* SPWindowFindMessageOne::getEditMultiLine()
{
	return m_pEditMultiLine;
}

//------------------------------------------------------------------------------------------------------
//设置是否发消息,false不发,ture发
void SPWindowFindMessageOne::setSendMessage( bool isSend )
{
	m_bSendMessage = isSend;
}

void SPWindowFindMessageOne::SetToStart()
{
	timer	  = 0;
	ShowTime  = 5;
	return;
}

SPTimer* SPWindowFindMessageOne::getTimer()
{
	return m_pFindTimer;
}