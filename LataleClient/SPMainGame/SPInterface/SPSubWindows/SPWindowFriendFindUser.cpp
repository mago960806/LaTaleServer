// Copyright (C) liuyang
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : liuyang 2008-8-28   
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

//#include "SPResourceManager.h"
//#include "Packet.h"
//
//#include "SPMouseCursor.h"
//#include "SPInterfaceManager.h"
//
//#include "SPGOBClusterDef.h"
//#include "SPAvatarModelUnitDef.h"
//#include "SPGameObject.h"
//#include "SPPlayer.h"
//#include "SPGOBManager.h"
//
//#include "SPWindowEdit.h"
//#include "SPWindowList.h"
//#include "SPWindowSlider.h"
//#include "SPWindowChat.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"

#include "SPWindowToolTip.h"

#include "PacketID.h"

#include "SPWindowFriendFindUser.h"

SPWindowFriendFindUser::SPWindowFriendFindUser(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_FRIEND_FIND , InstanceID, iX, iY, iCX, iCY, pParent )	
{
	Init();
}

SPWindowFriendFindUser::~SPWindowFriendFindUser()
{
	Clean();
}

void SPWindowFriendFindUser::Init()
{
	m_strToolTip	=	"[SPWindowFriendFindUser]";

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , &m_pBGTexture );

	ReposOwnImage(); // metalgeni [5/25/2006]

	SetRect( &m_rcBGSrc[0]	, 236 , 245 , 238 , 268 );			//	배경 왼쪽
	SetRect( &m_rcBGSrc[1]	, 240 , 245 , 242 , 268 );			//	배경 중간
	SetRect( &m_rcBGSrc[2]	, 244 , 245 , 246 , 268 );			//	배경 오른쪽

	int	iInstanceID	=	m_InstanceID	+	1;
	m_iStartID		=	iInstanceID;

	SPWindowButton*	pWindowButton	=	NULL;

	m_pIcon			=	new	SPWindowStatic( iInstanceID , 0 , 0 , 0 , 0 , this );

	iInstanceID++;

	m_pName			=	new	SPWindowStatic( iInstanceID , 0 , 0 , 0 , 0 , this );
	m_pName->SetFontShadow( false );

	m_iEndID		=	iInstanceID;

	m_bShowToolTip	=	true;
	m_pFriendInfo	=	NULL;
}

void SPWindowFriendFindUser::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
}

void SPWindowFriendFindUser::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorEnter( NULL , NULL );
	}
}

void SPWindowFriendFindUser::Hide(bool bSendServer)
{
	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorOut( NULL , NULL );
	}
}

void SPWindowFriendFindUser::ReposOwnImage()
{
	SetRect( &m_rcBGDest[0]	, m_iAX					, m_iAY		, m_iAX + 2					, m_iAY + m_iSY );		//	BG Left
	SetRect( &m_rcBGDest[1]	, m_rcBGDest[0].right	, m_iAY 	, m_rcBGDest[0].right + 188	, m_iAY + m_iSY );		//	BG Middle
	SetRect( &m_rcBGDest[2]	, m_rcBGDest[1].right	, m_iAY		, m_rcBGDest[1].right + 2	, m_iAY + m_iSY );		//	BG Right
}

void SPWindowFriendFindUser::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowFriendFindUser::Process(float fTime)
{
	SPWindow::Process( fTime );
}

void SPWindowFriendFindUser::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_WindowState == WND_STATE_CLICK || m_WindowState == WND_STATE_HITTEST )
	{
		if( m_pBGTexture )
		{
			for( int i = 0 ; i < FRIENDFINDUSER_BG_COUNT ; i++ )
			{
				m_pBGTexture->SetColor();
				m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
			}

			g_pVideo->Flush();
		}
	}

	SPWindow::Render(fTime);
}

bool SPWindowFriendFindUser::IsFriendInfo()
{
	bool bResult	=	false;

	if( m_pFriendInfo != NULL )
	{
		std::string	strName	=	m_pFriendInfo->szCharName;

		if( strName.empty() == false )
			bResult	=	true;
	}

	return bResult;
}
void SPWindowFriendFindUser::SetFriendInfo( FRIEND_INFO* pFriendInfo )
{
	m_pFriendInfo	=	pFriendInfo;

	const char*	pszName	=	NULL;
	bool	bOnline		=	false;

	if( m_pFriendInfo != NULL )
	{
		pszName	=	m_pFriendInfo->szCharName;

		if( m_pFriendInfo->uiServerID > 0 )
			bOnline	=	true;
	}

	SetName( pszName );
	SetOnline( bOnline );
}

void SPWindowFriendFindUser::ShowToolTip( bool bShowToolTip )
{
	m_bShowToolTip	=	bShowToolTip;
}

const char* SPWindowFriendFindUser::GetName()
{
	return	m_pName->GetWindowText();
}

void SPWindowFriendFindUser::SetIconImage( const char* szImage , int iSrcX , int iSrcY )
{
	m_pIcon->SetImage( szImage , iSrcX , iSrcY );
}

void SPWindowFriendFindUser::SetIconImagePos( int iXPos , int iYPos )
{
	m_pIcon->AdjustCoord( iXPos , iYPos );
}

void SPWindowFriendFindUser::SetIconImageSize( int iXSize , int iYSize )
{
	m_pIcon->SetRectSize( iXSize , iYSize );
}

void SPWindowFriendFindUser::SetTextPos( int iXPos , int iYPos )
{
	m_pName->AdjustCoord( iXPos , iYPos );
}

void SPWindowFriendFindUser::SetTextSize( int iXSize , int iYSize )
{
	m_pName->SetRectSize( iXSize , iYSize );
}

void SPWindowFriendFindUser::SetTextColorOnLine( D3DXCOLOR cColor )
{
	m_cOnLineColor	=	cColor;
}

void SPWindowFriendFindUser::SetTextColorOffLine( D3DXCOLOR cColor )
{
	m_cOffLineColor	=	cColor;
}

void SPWindowFriendFindUser::SetWndState( WND_STATE eWndState )
{
	m_WindowState	=	eWndState;
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowFriendFindUser )
SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER		,										OnCursorEnter		)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT			,										OnCursorOut			)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE		,										OnCursorMove		)

SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP			,										OnLButtonUp			)

SPIMESSAGE_COMMAND( SPIM_WHEEL_UP			,										OnWheelUp			)
SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN			,										OnWheelDown			)

SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER	,	m_iStartID	,	m_iEndID		,	OnCursorEnter		)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT	,	m_iStartID	,	m_iEndID		,	OnCursorOut			)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_MOVE	,	m_iStartID	,	m_iEndID		,	OnCursorMove		)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	,	m_iStartID	,	m_iEndID		,	OnLButtonUp			)

SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP		,	m_iStartID	,	m_iEndID		,	OnWheelUp			)
SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN	,	m_iStartID	,	m_iEndID		,	OnWheelDown			)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowFriendFindUser::OnCursorEnter( WPARAM wParam, LPARAM lParam )
{
	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == FALSE )
		return 1;

	if( m_WindowState != WND_STATE_CLICK )
		m_WindowState	=	WND_STATE_HITTEST;

	if( IsShow() == false )
		return 1;

	if( m_bShowToolTip == false )
		return 1;

	SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();

	if( pToolTipWindow == NULL )
		return 1;

	if( m_pFriendInfo != NULL )
	{
		if( m_pFriendInfo->uiServerID <= 0 )
			return 1;
	}

	pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_ENTER , (WPARAM)m_pFriendInfo , SPWindowToolTip::TOOLTIP_SHOW_TYPE_FRIEND_WHITE );

	return 1;
}

int SPWindowFriendFindUser::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	int	iType	=	(int)wParam;

	if( m_WindowState != WND_STATE_CLICK || iType != 0 )
		m_WindowState	=	WND_STATE_NORMAL;

	if( IsShow() == false )
		return 1;

	if( m_bShowToolTip == false )
		return 1;

	SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();

	if( pToolTipWindow == NULL )
		return 1;

	pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , NULL );

	return 1;
}

int SPWindowFriendFindUser::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == FALSE )
		return 1;

	if( m_WindowState != WND_STATE_CLICK )
		m_WindowState	=	WND_STATE_HITTEST;
	return 1;
}

int SPWindowFriendFindUser::OnLButtonUp( WPARAM wParam, LPARAM lParam )
{
	m_WindowState	=	WND_STATE_CLICK;
	SPSendMessageToParent( SPIM_LBUTTON_UP , wParam , lParam , this );
	return 1;
}

int SPWindowFriendFindUser::OnWheelUp( WPARAM wParam, LPARAM lParam )
{
	SPSendMessageToParent( SPIM_WHEEL_UP , wParam , lParam , this );
	return 1;
}

int SPWindowFriendFindUser::OnWheelDown( WPARAM wParam, LPARAM lParam )
{
	SPSendMessageToParent( SPIM_WHEEL_DOWN , wParam , lParam , this );
	return 1;
}

int SPWindowFriendFindUser::OnCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnCursorEnter( wParam , lParam );
	return 1;
}

int SPWindowFriendFindUser::OnCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnCursorOut( wParam , lParam );
	return 1;
}

int SPWindowFriendFindUser::OnCursorMove( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnCursorMove( wParam , lParam );
	return 1;
}

int SPWindowFriendFindUser::OnLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnLButtonUp( wParam , lParam );
	return 1;
}

int SPWindowFriendFindUser::OnWheelUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnWheelUp( wParam , lParam );
	return 1;
}

int SPWindowFriendFindUser::OnWheelDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnWheelDown( wParam , lParam );
	return 1;
}

void SPWindowFriendFindUser::SetName( const char* pstrName )
{
	m_pName->SetWindowText( pstrName );
}

void SPWindowFriendFindUser::SetLevel( UINT8 uiLevel )
{
}

void SPWindowFriendFindUser::SetOnline( BOOL bOnline )
{
	D3DXCOLOR	cNameColor;

	if( bOnline == TRUE )
	{
		m_pIcon->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );
		cNameColor	=	m_cOnLineColor;
	}
	else
	{
		m_pIcon->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );
		cNameColor	=	m_cOffLineColor;
	}

	m_pName->SetFontColor( cNameColor );
}
