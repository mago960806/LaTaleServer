
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"

#include "SPResourceDef.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPDebug.h"

#include "SPAbuseFilter.h"

#include "SPWindowListDEF.h"
#include "SPWindow.h"
#include "SPWindowRSList.h"


#define MAX_HISTORY_ITEM 100

SPWindowRSList::SPWindowRSList( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_CTRL_RS_LIST, InstanceID, iX, iY, iCX, iCY, pParent )
, m_pEnterItem(NULL)
, m_pBarTexture(NULL)
{
	Init();
}

SPWindowRSList::~SPWindowRSList()
{
	Clean();
}

void SPWindowRSList::Init()
{	
	m_strToolTip		= "[SPWindowRSList]";
	m_iBeginLine		= 0;
	m_iPrintLineCount	= 0;
	m_iTotalLineCount	= 0;
	m_dwAlign			= DT_LEFT;
	m_RGBA				= RGBA(0xFF,0xFF,0xFF,0x00);
	m_bLatest			= true;
	m_bWheelUse			= false;
	m_bNextLine			= false;
	m_bFontShadow		= false;
	m_bUseFontColor		= false;
	
	m_bUseSelectBar		= false ;
	m_rcRelPosX			= 0;
	m_rcRelPosY			= 0;
	SetMargin( 0 , 0 , 0 , 0 );

}

void SPWindowRSList::Clean()
{
	SPWindow::Clean();

	m_iBeginLine		= 0;
	m_iPrintLineCount	= 0;
	m_iTotalLineCount	= 0;

	ClearListItem();
	SAFE_RELEASE(m_pBarTexture) ;
	m_bLatest	=	true;
}


//------------------------------------------------------------------------------------
void SPWindowRSList::Show(bool bWithChild)
{
	m_pEnterItem = NULL ;
	SPWindow::Show(bWithChild);
}


//------------------------------------------------------------------------------------
void SPWindowRSList::Process(float fTime)
{
	SPWindow::Process(fTime);

	// 시작라인( m_iBeginLine ) 부터 Position 계산
	std::vector<RSLIST_ITEM>::iterator	IterItem		=	m_vListItem.begin()		+	m_iBeginLine;
	RSLIST_ITEM*	pCurItem;
	int		iCurLineCount	=	0;
	int		iOneLineHeight	=	(int)m_pFont->GetHeight() + m_iLineInterval;
	RECT	rcTextPos , rcCalPos;

	GetWindowRect( rcTextPos );
	rcTextPos.left		+=	m_rcMargin.left;
	rcTextPos.right		-=	m_rcMargin.right;
	rcTextPos.top		+=	m_rcMargin.top;
	rcTextPos.bottom	=	rcTextPos.top	+	iOneLineHeight;

	while( IterItem != m_vListItem.end() )
	{
		pCurItem	=	&(*IterItem);

		if( pCurItem != NULL )
		{
			rcCalPos.left		=	rcTextPos.left;
			rcCalPos.top		=	rcTextPos.top	-	1;
			rcCalPos.right		=	rcTextPos.right;
			rcCalPos.bottom		=	rcTextPos.bottom;

			pCurItem->m_rcDrawPos.left		=	rcCalPos.left;
			pCurItem->m_rcDrawPos.top		=	rcCalPos.top;
			pCurItem->m_rcDrawPos.right		=	rcCalPos.right;
			pCurItem->m_rcDrawPos.bottom	=	rcCalPos.bottom;

			// Next Top
			rcTextPos.top		=	rcTextPos.bottom;
			rcTextPos.bottom	=	rcTextPos.top	+	iOneLineHeight;
		}

		++IterItem;
		++iCurLineCount;

		if( iCurLineCount >= m_iPrintLineCount )
			break;
	}

}

void SPWindowRSList::Refresh()
{
	//m_iNOLine = m_iSY / m_pFont->GetHeight() - 1;
}

//------------------------------------------------------------------------------------
void SPWindowRSList::Render(float fTime)
{
	if(!m_bShow) return;

	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); ++Iter) {
		(*Iter)->Render(fTime);
	}
	// Window Render ============================================

	if( g_pVideo == NULL || m_pFont == NULL )
		return;

	// Select Bar ============================================
	if( m_pEnterItem && m_bUseSelectBar )
	{
		m_pBarTexture->RenderTexture( m_pEnterItem->m_rcDrawPos.left+m_rcRelPosX, m_pEnterItem->m_rcDrawPos.top+m_rcRelPosY , &m_rcSelectBarSrc );
	}

	// List ============================================
	// 시작라인( m_iBeginLine ) 부터 m_iPrintLineCount 만큼 렌더
	std::vector<RSLIST_ITEM>::iterator	IterItem		=	m_vListItem.begin()		+	m_iBeginLine;
	RSLIST_ITEM*	pCurItem;
	bool	bShadow			=	m_pFont->IsShadow();
	int		iCurLineCount	=	0;

	m_pFont->SetShadow( m_bFontShadow );

	while( IterItem != m_vListItem.end() )
	{
		pCurItem	=	&(*IterItem);

		if( pCurItem != NULL )
		{
			if( pCurItem->m_strText.empty() == false )
			{
				m_pFont->SetColor( pCurItem->m_TextColor );
				m_pFont->RenderText( pCurItem->m_strText.c_str() , &pCurItem->m_rcDrawPos, m_dwAlign );
			}
			
//#ifdef _DEBUG
//			// For Debug Box
//			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	// Image
//			g_pVideo->GetSysLine()->DrawRect(pCurItem->m_rcDrawPos);
//			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//#endif
		}

		++IterItem;
		++iCurLineCount;

		if( iCurLineCount >= m_iPrintLineCount )
			break;
	}

	m_pFont->SetShadow(	bShadow );
	m_pFont->SetColor();
}


//------------------------------------------------------------------------------------
/**
	리스트 아이템 넣기
 */
bool SPWindowRSList::InsertItem( const char* pszListItem , D3DXCOLOR color, D3DXCOLOR Rvscolor )
{
	if( pszListItem == NULL )
		return false;

	RSLIST_ITEM	stAddListItem;

	stAddListItem.m_bUseFontColor = m_bUseFontColor ;
	stAddListItem.m_strText		=	pszListItem;
	stAddListItem.m_TextColor	 =	color;
	stAddListItem.m_DefTextColor =	color;
	stAddListItem.m_RvsTextColor =	Rvscolor;

	InsertItem( &stAddListItem );

	return true;
}

bool SPWindowRSList::InsertItem( RSLIST_ITEM* pListItem )
{
	if( g_pVideo == NULL || m_pFont == NULL || pListItem == NULL )
		return false;

	RECT	rcImagePos , rcTextPos;

	GetWindowRect( rcTextPos );

	rcTextPos.left		+=	m_rcMargin.left;
	rcTextPos.top		+=	m_rcMargin.top;
	rcTextPos.right		-=	m_rcMargin.right;
	rcTextPos.bottom	-=	m_rcMargin.bottom;

	rcImagePos.left		=	rcTextPos.left;
	rcImagePos.top		=	rcTextPos.top;
	rcImagePos.right	=	rcTextPos.right;
	rcImagePos.bottom	=	rcTextPos.bottom;

	RSLIST_ITEM	stAddListItem;
	std::string	strTempListItem		=	pListItem->m_strText;

	if( m_bNextLine == true )
	{
		char szLineFeedBuf[MAX_STR_LENGTH+1];
		SPAbuseFilter::GetInstance()->ReplaceLineFeed( pListItem->m_strText.c_str() , szLineFeedBuf , MAX_STR_LENGTH );
		m_bNextLine		=	false;
		strTempListItem	=	szLineFeedBuf;
	}

	// Add Text
	if( strTempListItem.empty() == false )
	{
		std::vector< std::string >				vTextList;
		std::vector< std::string >::iterator	IterTextList;

		m_pFont->ConvertMultiLineW( strTempListItem.c_str() , rcTextPos, vTextList );

		IterTextList	=	vTextList.begin();

		while ( IterTextList != vTextList.end() )
		{
			if( (*IterTextList).empty() == false )
			{
				stAddListItem.m_strText			=	(*IterTextList);
				stAddListItem.m_TextColor		=	pListItem->m_DefTextColor;
				stAddListItem.m_DefTextColor	=	pListItem->m_DefTextColor;
				stAddListItem.m_RvsTextColor	=	pListItem->m_RvsTextColor;
				stAddListItem.m_stReturnValue	=	pListItem->m_stReturnValue;

				PushBackItem( stAddListItem );
				++m_iTotalLineCount;
			}
			++IterTextList;
		}
	}

	// Compute Draw Area
	int	iOneLineHeight	=	(int)m_pFont->GetHeight() + m_iLineInterval;
	int	iListItemSize	=	(int)m_vListItem.size();
	m_iPrintLineCount	=	( m_iSY - m_rcMargin.top - m_rcMargin.bottom + 1 ) / iOneLineHeight;

	if( m_bLatest == true )
	{
		m_iBeginLine	=	( iListItemSize > m_iPrintLineCount ) ? iListItemSize - m_iPrintLineCount : 0;
		m_bAddString	=	false;
	}
	else
		m_bAddString	=	true;

	return true;
}


/**
	Item이 MAX_HISTORY_ITEM 이면 제일 앞의 데이터를 지우고 뒤에 추가
 */
void SPWindowRSList::PushBackItem( RSLIST_ITEM& stListItem )
{
	if( m_vListItem.size() >= MAX_HISTORY_ITEM ) 
	{
		std::vector<RSLIST_ITEM>::iterator	Iter	=	m_vListItem.begin();

		if( Iter != m_vListItem.end() )
		{
			(*Iter).Clear();
		}
		m_vListItem.erase( m_vListItem.begin() );
	}

	m_vListItem.push_back( stListItem );
}


void SPWindowRSList::ClearListItem()
{
	std::vector<RSLIST_ITEM>::iterator	Iter	=	m_vListItem.begin();

	while( Iter != m_vListItem.end() )
	{
		(*Iter).Clear();
		++Iter;
	}

	m_vListItem.clear();
	m_iTotalLineCount = 0;
}


//------------------------------------------------------------------------------------
/**
	리스트가 가지고 있는 결과 값 받아오기
 */
LIST_RETURNVALUE* SPWindowRSList::GetReturnValue( int iXPos , int iYPos )
{
	LIST_RETURNVALUE*					pFindData	=	NULL;
	std::vector<RSLIST_ITEM>::iterator	Iter		=	m_vListItem.begin()	+	m_iBeginLine;
	

	while( Iter != m_vListItem.end() )
	{
		if( (*Iter).m_rcDrawPos.top <= iYPos && (*Iter).m_rcDrawPos.bottom >= iYPos &&
			(*Iter).m_rcDrawPos.left <= iXPos && (*Iter).m_rcDrawPos.right >= iXPos )
		{
			pFindData	=	&((*Iter).m_stReturnValue);
			break;
		}
		++Iter;
	}

	return pFindData;
}


//------------------------------------------------------------------------------------
int SPWindowRSList::ScrollList( int iSubSpim )
{
	int iStrSize	= (int)m_vListItem.size();

	switch( iSubSpim )
	{
	case SUB_SPIM_SCROLL_UP		:
		{
			if( m_iBeginLine > 0 )
				--m_iBeginLine;
		}
		break;

	case SUB_SPIM_SCROLL_DOWN	:
		{
			if( iStrSize > m_iBeginLine + m_iPrintLineCount )
				++m_iBeginLine;
		}
		break;

	case SUB_SPIM_SCROLL_LATEST	:
		{
			if( g_pVideo == NULL )
				return 0;

			if( m_pFont == NULL )
				return 0;

			int	iOneLineHeight	=	(int)m_pFont->GetHeight() + m_iLineInterval;
			m_iPrintLineCount	=	( m_iSY - m_rcMargin.top - m_rcMargin.bottom + 1 ) / iOneLineHeight;
			m_iBeginLine		=	0;
			if( iStrSize > m_iPrintLineCount )
				m_iBeginLine = iStrSize - m_iPrintLineCount;
		}
		break;
	case SUB_SPIM_SCROLL_FIRST:
		m_iBeginLine = 0 ;
		break ;
	}

	if( iStrSize <= m_iBeginLine + m_iPrintLineCount )
	{
		m_bLatest	=	true;
		m_bAddString	=	false;
	}
	else
		m_bLatest	=	false;

	return m_iBeginLine;
}

void SPWindowRSList::SetMargin( int iLeft , int iTop , int iRight , int iBottom )
{
	SetRect( &m_rcMargin , iLeft , iTop , iRight , iBottom );
}

bool SPWindowRSList::GetLatest()
{
	return m_bLatest;
}

bool SPWindowRSList::IsAddString()
{
	return m_bAddString;
}

void SPWindowRSList::SetWheelUse( bool bUse )
{
	m_bWheelUse	=	bUse;
}

void SPWindowRSList::SetNextLine( bool bUse )
{
	m_bNextLine	=	bUse;
}


/**
	Mouse Select Bar 사용 여부
 */
void SPWindowRSList::SetSelectBar(const char* pszImage, RECT rcImageSrc, int iRelPosX, int iRelPosY)
{
	if( pszImage == NULL )
		return ;

	if( m_pBarTexture )
		SAFE_RELEASE( m_pBarTexture ) ;

	if( g_pVideo->GetTextureMgr()->LoadTexture( pszImage , &m_pBarTexture ) )
	{
		m_bUseSelectBar = true ;
		SetRect(&m_rcSelectBarSrc, rcImageSrc.left, rcImageSrc.top, rcImageSrc.right, rcImageSrc.bottom) ;
		m_rcRelPosX = iRelPosX ;
		m_rcRelPosY = iRelPosY ;
	}
}


RSLIST_ITEM* SPWindowRSList::GetMouseOverItem()
{
	std::vector<RSLIST_ITEM>::iterator	Iter		=	m_vListItem.begin()	+	m_iBeginLine;
	int iXPos = g_pInput->GetMousePosX();
	int iYPos = g_pInput->GetMousePosY();

	while( Iter != m_vListItem.end() )
	{
		if( (*Iter).m_rcDrawPos.top <= iYPos && (*Iter).m_rcDrawPos.bottom >= iYPos &&
			(*Iter).m_rcDrawPos.left <= iXPos && (*Iter).m_rcDrawPos.right >= iXPos )
		{
			return &(*Iter) ;
			break;
		}
		++Iter;
	}
	return NULL ;
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowRSList )
SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE,		OnCursorMove	)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,		OnLButtonDown	)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnLButtonUP		)
SPIMESSAGE_COMMAND(	SPIM_ADD_ITEM,			OnAddItem		)
SPIMESSAGE_COMMAND(	SPIM_INSERT_ITEM,		OnAddItem		)	
SPIMESSAGE_COMMAND(	SPIM_SCROLL			,	OnScroll		)
SPIMESSAGE_COMMAND(	SPIM_PURGE			,	OnPurge			)
SPIMESSAGE_COMMAND(	SPIM_WHEEL_UP		,	OnWheelUp		)
SPIMESSAGE_COMMAND(	SPIM_WHEEL_DOWN		,	OnWheelDown		)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowRSList::OnCursorEnter( WPARAM wParam, LPARAM lParam)
{
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	if( (m_pEnterItem = GetMouseOverItem()) != NULL )
	{
		// If it has return value then change text color
		if( m_pEnterItem->m_bUseFontColor && (m_pEnterItem->m_stReturnValue.m_iReturnValue > -1) )
			m_pEnterItem->m_TextColor = m_pEnterItem->m_RvsTextColor ;
	}
	return SPSendMessageToParent(SPIM_CURSOR_ENTER, wParam, lParam, this);	
}

/**
	CursorMove : if CursorEnter then change text color (If it has return value)
 */
int SPWindowRSList::OnCursorMove( WPARAM wParam, LPARAM lParam)
{
	RSLIST_ITEM* pMoveItem = GetMouseOverItem() ;
	if( pMoveItem == NULL )
		return 0 ;
	if( m_pEnterItem != pMoveItem)
	{
		if( m_pEnterItem )
			m_pEnterItem->m_TextColor = m_pEnterItem->m_DefTextColor ;
		m_pEnterItem = pMoveItem ;
		// If it has return value then change text color
		if( m_pEnterItem->m_bUseFontColor && (m_pEnterItem->m_stReturnValue.m_iReturnValue > -1) )
			m_pEnterItem->m_TextColor = m_pEnterItem->m_RvsTextColor ;
	}

	return SPSendMessageToParent(SPIM_CURSOR_ENTER, wParam, lParam, this);	
}

/**
	CursorOut : All Item color set default
 */
int SPWindowRSList::OnCursorOut( WPARAM wParam, LPARAM lParam)
{
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0x00);
	std::vector<RSLIST_ITEM>::iterator	Iter		=	m_vListItem.begin()	+	m_iBeginLine;
	while( Iter != m_vListItem.end() )
	{
		(*Iter).m_TextColor = (*Iter).m_DefTextColor ;
		++Iter;
	}
	m_pEnterItem = NULL ;
	return SPSendMessageToParent(SPIM_CURSOR_OUT, wParam, lParam, this);
}

int SPWindowRSList::OnLButtonDown( WPARAM wParam, LPARAM lParam)
{
	lParam = (LPARAM)(&m_pEnterItem->m_stReturnValue) ;
	return SPSendMessageToParent(SPIM_LBUTTON_DOWN, wParam, lParam, this);
}

int SPWindowRSList::OnLButtonUP( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, lParam, this);
}

int SPWindowRSList::OnAddItem( WPARAM wParam, LPARAM lParam)
{
	D3DXCOLOR*	pColor	=	(D3DXCOLOR*)lParam;	

	if( pColor == NULL ) {
		return InsertItem( (char*)wParam , D3DXCOLOR(1,1,1,1), D3DXCOLOR(0,0,0,1) );
	}
	return InsertItem( (char*)wParam , *pColor, D3DXCOLOR(1-pColor->r,1-pColor->g,1-pColor->b,1) );
}

int SPWindowRSList::OnScroll	( WPARAM wParam, LPARAM lParam)
{
	int iCurLine = ScrollList( (int)wParam );
	return SPSendMessageToParent(SPIM_SCROLL, iCurLine, lParam, this);
}

int SPWindowRSList::OnPurge	( WPARAM wParam, LPARAM lParam)
{
	m_iBeginLine		= 0;
	m_iPrintLineCount	= 0;

	ClearListItem();

	m_bLatest	=	true;

	return SPSendMessageToParent(SPIM_PURGE, wParam, lParam, this);
}

int SPWindowRSList::OnWheelUp( WPARAM wParam , LPARAM lParam )
{
	if( m_bWheelUse == true )
		ScrollList( SUB_SPIM_SCROLL_UP );

	return SPSendMessageToParent( SPIM_WHEEL_UP , wParam , lParam , this );
}

int SPWindowRSList::OnWheelDown( WPARAM wParam , LPARAM lParam )
{
	if( m_bWheelUse == true )
		ScrollList( SUB_SPIM_SCROLL_DOWN );

	return SPSendMessageToParent( SPIM_WHEEL_DOWN , wParam , lParam , this );
}
