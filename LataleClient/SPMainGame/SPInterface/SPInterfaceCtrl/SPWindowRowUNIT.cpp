// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD USER UNIT
// Comment     : 아이콘과 Text로 이루어진 리스트중 하나의 Unit 정의
//				 아이콘은 셋팅가능하고 직업은 직업 코드로 설정 된다. (for Guild)
//				 선택 되었을때 배경 선택이 가능하고, 배경색이 바뀌지 않게 할수 있다.(SetSelectAble)
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

#include "SPItemAttr.h"
#include "SPItemStatus.h"

#include "SPWindowToolTip.h"

#include "PacketID.h"

#include "SPWindowRowUNIT.h"
#include "GlobalDefines_Share.h"


SPWindowRowUNIT::SPWindowRowUNIT(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_CTRL_ROW_UNIT , InstanceID, iX, iY, iCX, iCY, pParent )	
, m_pBGTexture(NULL)
{
	Init();
}

SPWindowRowUNIT::~SPWindowRowUNIT()
{
	Clean();
}

void SPWindowRowUNIT::Init()
{
	m_strToolTip	=	"[SPWindowRowUNIT]";
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

void SPWindowRowUNIT::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
}

void SPWindowRowUNIT::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorEnter( NULL , NULL );
	}
}

void SPWindowRowUNIT::Hide(bool bSendServer)
{
	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorOut( NULL , NULL );
	}
}

void SPWindowRowUNIT::ReposOwnImage()
{
	SetRect( &m_rcBGDest[0]	, m_iAX	+ 4				, m_iAY		, m_iAX + 6					, m_iAY + m_iSY );		//	BG Left
	SetRect( &m_rcBGDest[1]	, m_rcBGDest[0].right	, m_iAY 	, m_iAX + m_iSX -4			, m_iAY + m_iSY );		//	BG Middle
	SetRect( &m_rcBGDest[2]	, m_rcBGDest[1].right	, m_iAY		, m_iAX + m_iSX -2			, m_iAY + m_iSY );		//	BG Right
}

void SPWindowRowUNIT::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowRowUNIT::Process(float fTime)
{
	SPWindow::Process( fTime );
}

void SPWindowRowUNIT::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_WindowState == WND_STATE_CLICK || m_WindowState == WND_STATE_HITTEST )
	{
		m_pText->SetFontColor(m_cOnColor) ;
		m_pText->SetFont(FONT_12_BOLD);
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
	else
	{
		m_pText->SetFontColor(m_FontColor) ;
		m_pText->SetFont(FONT_12_NORMAL);
	}
	SPWindow::Render(fTime);

	g_pVideo->Flush();
}

bool SPWindowRowUNIT::IsHaveInfo()
{
	bool bResult	=	false;

	std::string	strName	=	GetText() ;
	if( strName.empty() == false )
		bResult	=	true;

	return bResult;
}

void SPWindowRowUNIT::SetSelectAble(bool bChange)
{
	m_bSelectAble = bChange ;
}


bool SPWindowRowUNIT::GetSelectAble()
{
	return m_bSelectAble ;
}

//////////////////////////////////////////////////////////////////////////
// Icon Setting
void SPWindowRowUNIT::SetIconImage( const char* szImage , int iSrcX , int iSrcY )
{
	m_pIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_pIcon->SetImage( szImage , iSrcX , iSrcY );
}

void SPWindowRowUNIT::SetIconImagePos( int iXPos , int iYPos )
{
	m_pIcon->AdjustCoord( iXPos , iYPos );
}

void SPWindowRowUNIT::SetIconImageSize( int iXSize , int iYSize )
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
void SPWindowRowUNIT::SetIClassImage( int iClassType )
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
void SPWindowRowUNIT::SetTextString( const char* strText)
{
	m_pText->SetWindowText(strText);
}

void SPWindowRowUNIT::SetTextPos( int iXPos , int iYPos )
{
	m_pText->AdjustCoord( iXPos , iYPos );
}

void SPWindowRowUNIT::SetTextSize( int iXSize , int iYSize )
{
	m_pText->SetRectSize( iXSize , iYSize );
}

void SPWindowRowUNIT::SetTextColor( D3DXCOLOR cColor )
{
	m_pText->SetColor(D3DXCOLOR(cColor)) ;
	m_cOnColor = cColor ;
}

void SPWindowRowUNIT::SetOnTextColor( D3DXCOLOR cColor )
{
	m_cOnColor = cColor ;
}

void SPWindowRowUNIT::ShowToolTip( bool bShowToolTip )
{
	m_bShowToolTip	=	bShowToolTip;
}

const char* SPWindowRowUNIT::GetText()
{
	return	m_pText->IsShow() ? m_pText->GetWindowText() : NULL ;
}

void SPWindowRowUNIT::SetWndState( WND_STATE eWndState )
{
	m_WindowState	=	eWndState;
}


void SPWindowRowUNIT::SetSelectBG(char* pTextureFile, RECT* prcLeft, RECT* prcMiddle, RECT* prcRight)
{
	if( m_pBGTexture )
		SAFE_RELEASE( m_pBGTexture ) ;

	g_pVideo->GetTextureMgr()->LoadTexture( pTextureFile , &m_pBGTexture );

	SetRect( &m_rcBGSrc[0]	, prcLeft->left , prcLeft->top , prcLeft->right , prcLeft->bottom );			//	배경 왼쪽
	SetRect( &m_rcBGSrc[1]	, prcMiddle->left , prcMiddle->top , prcMiddle->right , prcMiddle->bottom );	//	배경 중간
	SetRect( &m_rcBGSrc[2]	, prcRight->left , prcRight->top , prcRight->right , prcRight->bottom );		//	배경 오른쪽
}




SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowRowUNIT )

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
int SPWindowRowUNIT::OnCursorEnter( WPARAM wParam, LPARAM lParam )
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
int SPWindowRowUNIT::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	int	iType	=	(int)wParam;

	if( m_WindowState != WND_STATE_CLICK || iType != 0 )
		m_WindowState	=	WND_STATE_NORMAL;

	return 1;
}
int SPWindowRowUNIT::OnCursorMove( WPARAM wParam, LPARAM lParam )
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

int SPWindowRowUNIT::OnCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnCursorEnter( wParam , lParam );
	return 1;
}
int SPWindowRowUNIT::OnCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnCursorOut( wParam , lParam );
	return 1;
}
int SPWindowRowUNIT::OnCursorMove( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnCursorMove( wParam , lParam );
	return 1;
}


/**
	리스트 클릭 관련 처리
*/
int SPWindowRowUNIT::OnLButtonUp( WPARAM wParam, LPARAM lParam )
{
	if( m_bSelectAble )
	{
		SPSendMessageToParent( SPIM_LBUTTON_UP , wParam , lParam , this );
		m_WindowState	=	WND_STATE_CLICK;
	}
	return 1;
}

int SPWindowRowUNIT::OnLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnLButtonUp( wParam , lParam );
	return 1;
}

/**
휠 스크롤 관련 처리
*/
int SPWindowRowUNIT::OnWheelUp( WPARAM wParam, LPARAM lParam )
{
	SPSendMessageToParent( SPIM_WHEEL_UP , wParam , lParam , this );
	return 1;
}

int SPWindowRowUNIT::OnWheelDown( WPARAM wParam, LPARAM lParam )
{
	SPSendMessageToParent( SPIM_WHEEL_DOWN , wParam , lParam , this );
	return 1;
}

int SPWindowRowUNIT::OnWheelUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnWheelUp( wParam , lParam );
	return 1;
}

int SPWindowRowUNIT::OnWheelDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	OnWheelDown( wParam , lParam );
	return 1;
}