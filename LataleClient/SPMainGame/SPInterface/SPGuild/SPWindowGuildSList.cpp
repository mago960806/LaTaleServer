// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD USER UNIT
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

#include "SPWindowGuildSList.h"
#include "GlobalDefines_Share.h"
#include "SPLocalizeManager.h"

SPWindowGuildSimpleList::SPWindowGuildSimpleList(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_GUILD_INFO , InstanceID, iX, iY, iCX, iCY, pParent )	
, m_pBGTexture(NULL)
{
	Init();
}

SPWindowGuildSimpleList::~SPWindowGuildSimpleList()
{
	Clean();
}

void SPWindowGuildSimpleList::Init()
{
	m_strToolTip	=	"[SPWindowGuildSimpleList]";
	m_bShowToolTip	= false ;
	m_bSelectAble	= true ;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[0]	, 484 , 282 , 486 , 298 );			//	배경 왼쪽
	SetRect( &m_rcBGSrc[1]	, 488 , 282 , 490 , 298 );			//	배경 중간
	SetRect( &m_rcBGSrc[2]	, 492 , 282 , 494 , 298 );			//	배경 오른쪽


	int	iInstanceID	=	m_InstanceID	+	1;
	m_iStartID		=	iInstanceID;

	SPWindowButton*	pWindowButton	=	NULL;

	m_pIcon	=	new	SPWindowStatic( iInstanceID , 8 , 1 , 16 , 13 , this );
	iInstanceID++;

	m_pText		=	new	SPWindowStatic( iInstanceID , 30 , 2 , 53 , 12 , this );
	m_pText->SetWindowText(NULL) ;
	m_pText->SetFormat(DT_VCENTER | DT_CENTER);
	m_pText->SetFontShadow( false );

	m_iEndID	=	iInstanceID;
}

void SPWindowGuildSimpleList::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
}

void SPWindowGuildSimpleList::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorEnter( NULL , NULL );
	}
}

void SPWindowGuildSimpleList::Hide(bool bSendServer)
{
	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorOut( NULL , NULL );
	}
}

void SPWindowGuildSimpleList::ReposOwnImage()
{
	//if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_SANDA )
	//{
	//	SetRect( &m_rcBGDest[0]	, m_iAX					, m_iAY		, m_iAX + 2					, m_iAY + m_iSY );		//	BG Left
	//	SetRect( &m_rcBGDest[1]	, m_rcBGDest[0].right	, m_iAY 	, m_iAX + m_iSX -4			, m_iAY + m_iSY );		//	BG Middle
	//	SetRect( &m_rcBGDest[2]	, m_rcBGDest[1].right	, m_iAY		, m_rcBGDest[1].right + 2	, m_iAY + m_iSY );		//	BG Right
	//}
	//else
	//{
		SetRect( &m_rcBGDest[0]	, m_iAX	+ 4				, m_iAY		, m_iAX + 6					, m_iAY + m_iSY );		//	BG Left
		SetRect( &m_rcBGDest[1]	, m_rcBGDest[0].right	, m_iAY 	, m_iAX + m_iSX -4			, m_iAY + m_iSY );		//	BG Middle
		SetRect( &m_rcBGDest[2]	, m_rcBGDest[1].right	, m_iAY		, m_iAX + m_iSX -2			, m_iAY + m_iSY );		//	BG Right
	//}
}

void SPWindowGuildSimpleList::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowGuildSimpleList::Process(float fTime)
{
	SPWindow::Process( fTime );
}

void SPWindowGuildSimpleList::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_WindowState == WND_STATE_CLICK || m_WindowState == WND_STATE_HITTEST )
	{
		if( m_pBGTexture )
		{
			for( int i = 0 ; i < SIMPLE_SELECT_BG_COUNT ; i++ )
			{
				m_pBGTexture->SetColor();
				m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
			}

			g_pVideo->Flush();
		}
	}
	SPWindow::Render(fTime);

	g_pVideo->Flush();
}

bool SPWindowGuildSimpleList::IsHaveInfo()
{
	bool bResult	=	false;

	std::string	strName	=	GetText() ;
	if( strName.empty() == false )
		bResult	=	true;

	return bResult;
}

void SPWindowGuildSimpleList::SetSelectAble(bool bChange)
{
	m_bSelectAble = bChange ;
}


bool SPWindowGuildSimpleList::GetSelectAble()
{
	return m_bSelectAble ;
}

//////////////////////////////////////////////////////////////////////////
// Icon Setting
void SPWindowGuildSimpleList::SetIconImage( const char* szImage , int iSrcX , int iSrcY )
{
	m_pIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_pIcon->SetImage( szImage , iSrcX , iSrcY );
}

void SPWindowGuildSimpleList::SetIconImagePos( int iXPos , int iYPos )
{
	m_pIcon->AdjustCoord( iXPos , iYPos );
}

void SPWindowGuildSimpleList::SetIconImageSize( int iXSize , int iYSize )
{
	m_pIcon->SetRectSize( iXSize , iYSize );
}

/**
	직업별 이미지 아이콘
	[CLASS_A],		395, 324, 411, 340);		// 공격형
	[CLASS_B],		412, 324, 428, 340);		// 방어형
	[CLASS_C],		395, 307, 411, 323);		// 마법형
	[CLASS_D],		412, 307, 428, 323);		// 정찰형
*/
void SPWindowGuildSimpleList::SetIClassImage( int iClassType )
{
	m_pIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
	const char*	pTextureName;
	RECT*		pTextureSrc;
	pTextureName	=	g_pInterfaceManager->GetClassTextureName( (CLASS_TYPE)iClassType );
	pTextureSrc		=	g_pInterfaceManager->GetClassTextureSrc( (CLASS_TYPE)iClassType );
	if( pTextureName != NULL && pTextureSrc != NULL )
		m_pIcon->SetImage( pTextureName , pTextureSrc->left , pTextureSrc->top );
}

//////////////////////////////////////////////////////////////////////////
// Text Setting
void SPWindowGuildSimpleList::SetTextString( const char* strText)
{
	m_pText->SetWindowText(strText);
}

void SPWindowGuildSimpleList::SetTextPos( int iXPos , int iYPos )
{
	m_pText->AdjustCoord( iXPos , iYPos );
}

void SPWindowGuildSimpleList::SetTextSize( int iXSize , int iYSize )
{
	m_pText->SetRectSize( iXSize , iYSize );
}

void SPWindowGuildSimpleList::SetTextColor( D3DXCOLOR cColor )
{
	m_pText->SetColor(D3DXCOLOR(cColor)) ;
}


void SPWindowGuildSimpleList::ShowToolTip( bool bShowToolTip )
{
	m_bShowToolTip	=	bShowToolTip;
}

const char* SPWindowGuildSimpleList::GetText()
{
	return	m_pText->IsShow() ? m_pText->GetWindowText() : NULL ;
}

void SPWindowGuildSimpleList::SetWndState( WND_STATE eWndState )
{
	m_WindowState	=	eWndState;
}


SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowGuildSimpleList )

SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER		,										OnCursorEnter		)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT			,										OnCursorOut			)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE		,										OnCursorMove		)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP			,										OnLButtonUp			)

SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER	,	m_iStartID	,	m_iEndID		,	OnCursorEnter		)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT	,	m_iStartID	,	m_iEndID		,	OnCursorOut			)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_MOVE	,	m_iStartID	,	m_iEndID		,	OnCursorMove		)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	,	m_iStartID	,	m_iEndID		,	OnLButtonUp			)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


/**
	Mouse가 영역내에 있으면 m_WindowState를 변경해서 MouseOn Image 처리를 한다.
*/
int SPWindowGuildSimpleList::OnCursorEnter( WPARAM wParam, LPARAM lParam )
{
	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == FALSE )
		return 1;
	
	if( m_bSelectAble)
	{
		if( m_WindowState != WND_STATE_CLICK )
			m_WindowState	=	WND_STATE_HITTEST;
	}

	return 1;
}
int SPWindowGuildSimpleList::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	int	iType	=	(int)wParam;

	if( m_WindowState != WND_STATE_CLICK || iType != 0 )
		m_WindowState	=	WND_STATE_NORMAL;

	return 1;
}
int SPWindowGuildSimpleList::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == FALSE )
		return 1;

	if( m_bSelectAble)
	{
		if( m_WindowState != WND_STATE_CLICK )
			m_WindowState	=	WND_STATE_HITTEST;
	}

	return 1;
}

int SPWindowGuildSimpleList::OnCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnCursorEnter( wParam , lParam );
	return 1;
}
int SPWindowGuildSimpleList::OnCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnCursorOut( wParam , lParam );
	return 1;
}
int SPWindowGuildSimpleList::OnCursorMove( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnCursorMove( wParam , lParam );
	return 1;
}


/**
	리스트 클릭 관련 처리
*/
int SPWindowGuildSimpleList::OnLButtonUp( WPARAM wParam, LPARAM lParam )
{
	if( m_bSelectAble )
	{
		SPSendMessageToParent( SPIM_LBUTTON_UP , wParam , lParam , this );
		m_WindowState	=	WND_STATE_CLICK;
	}
	return 1;
}

int SPWindowGuildSimpleList::OnLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnLButtonUp( wParam , lParam );
	return 1;
}

/**
휠 스크롤 관련 처리
*/
int SPWindowGuildSimpleList::OnWheelUp( WPARAM wParam, LPARAM lParam )
{
	SPSendMessageToParent( SPIM_WHEEL_UP , wParam , lParam , this );
	return 1;
}

int SPWindowGuildSimpleList::OnWheelDown( WPARAM wParam, LPARAM lParam )
{
	SPSendMessageToParent( SPIM_WHEEL_DOWN , wParam , lParam , this );
	return 1;
}

int SPWindowGuildSimpleList::OnWheelUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnWheelUp( wParam , lParam );
	return 1;
}

int SPWindowGuildSimpleList::OnWheelDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnWheelDown( wParam , lParam );
	return 1;
}