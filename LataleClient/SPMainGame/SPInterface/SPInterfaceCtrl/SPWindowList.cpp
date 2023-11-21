
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
#include "SPWindowList.h"


#define MAX_HISTORY_ITEM 100

SPWindowList::SPWindowList( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_CTRL_LISTBOX, InstanceID, iX, iY, iCX, iCY, pParent )
{
	Init();
}

SPWindowList::~SPWindowList()
{
	Clean();
}

void SPWindowList::Init()
{	
	m_strToolTip		= "[SPWindowList]";
	m_iBeginLine		= 0;
	m_iPrintLineCount	= 0;
	m_dwAlign			= DT_LEFT;
	m_RGBA				= RGBA(0xFF,0xFF,0xFF,0x00);
	m_bLatest			= true;
	m_bWheelUse			= false;
	m_bNextLine			= false;

	SetMargin( 0 , 0 , 0 , 0 );

}

void SPWindowList::Clean()
{
	SPWindow::Clean();

	m_iBeginLine		= 0;
	m_iPrintLineCount	= 0;

	ClearListItem();

	m_bLatest	=	true;
}

void SPWindowList::Process(float fTime)
{
	SPWindow::Process(fTime);	
}

void SPWindowList::Refresh()
{
	//m_iNOLine = m_iSY / m_pFont->GetHeight() - 1;
}

void SPWindowList::Render(float fTime)
{
	if(!m_bShow) return;
	if(*m_ppImageActive) {		

		g_pVideo->Flush();

		RECT DestRect;
		DestRect.top	= m_iAY;
		DestRect.left	= m_iAX;
		DestRect.bottom	= m_iAY + m_iSY;
		DestRect.right  = m_iAX + m_iSX;

		(*m_ppImageActive)->SetColor( m_RGBA );
		(*m_ppImageActive)->RenderTexture( &DestRect, &m_rcImageSrc );
		(*m_ppImageActive)->SetColor();
	}

	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); ++Iter) {
		(*Iter)->Render(fTime);
	}

	if( g_pVideo == NULL || m_pFont == NULL )
		return;

	// Ω√¿€∂Û¿Œ( m_iBeginLine ) ∫Œ≈Õ m_iPrintLineCount ∏∏≈≠ ∑ª¥ı
	std::vector<LIST_ITEM>::iterator	IterItem		=	m_vListItem.begin()		+	m_iBeginLine;

	bool	bShadow			=	m_pFont->IsShadow();
	int		iCurLineCount	=	0;
	int		iOneLineHeight	=	(int)m_pFont->GetHeight() + m_iLineInterval;
	RECT	rcTextPos , rcCalPos;

	GetWindowRect( rcTextPos );

	rcTextPos.left		+=	m_rcMargin.left;
	rcTextPos.right		-=	m_rcMargin.right;
	rcTextPos.top		+=	m_rcMargin.top;
	rcTextPos.bottom	=	rcTextPos.top	+	iOneLineHeight;

	LIST_ITEM*	pCurItem;
	std::vector<LIST_ICON>::iterator	IterIcon;

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

			if( pCurItem->m_stBGImage.m_pImage != NULL )
			{
				rcCalPos.right	=	rcCalPos.left	+	pCurItem->m_stBGImage.m_iSX;
				rcCalPos.bottom	=	rcCalPos.top	+	pCurItem->m_stBGImage.m_iSY;

				pCurItem->m_stBGImage.m_pImage->RenderTexture( &rcCalPos , &(pCurItem->m_stBGImage.m_rcSrc) );
				g_pVideo->Flush();
			}

			IterIcon	=	pCurItem->m_vIconImage.begin();

			if (pCurItem->m_bFirstLine)
			{
				// channel backgroud
				if (pCurItem->m_imgChannel.m_pImage)
				{
					rcCalPos.left = rcCalPos.left;
					rcCalPos.right = rcCalPos.left + pCurItem->m_imgChannel.m_iSX;

					rcCalPos.top = rcTextPos.top;
					rcCalPos.bottom	= rcCalPos.top + pCurItem->m_imgChannel.m_iSY;

					pCurItem->m_imgChannel.m_pImage->RenderTexture( &rcCalPos , &(pCurItem->m_imgChannel.m_rcSrc) );
					g_pVideo->Flush();
				}

				while ( IterIcon != pCurItem->m_vIconImage.end() )
				{
					if( (*IterIcon).m_pImage != NULL )
					{
						rcCalPos.right	=	rcCalPos.left	+	(*IterIcon).m_iSX;
						rcCalPos.bottom	=	rcCalPos.top	+	(*IterIcon).m_iSY;

						(*IterIcon).m_pImage->RenderTexture( &rcCalPos , &( (*IterIcon).m_rcSrc ) );
						g_pVideo->Flush();
						rcCalPos.left	+=	(*IterIcon).m_iSX;
					}

					++IterIcon;
				}
					
				// sender text
				if (pCurItem->m_strSender.empty() == false)
				{
					m_pFont->SetColor( pCurItem->m_stTextColor );
					m_pFont->SetEffect( pCurItem->m_fontEffect );

					int iTextRasterLen = m_pFont->GetSizeW( pCurItem->m_strSender.c_str() ).cx;
					rcCalPos.right = rcCalPos.left + iTextRasterLen;
					rcCalPos.top = rcTextPos.top;
					rcCalPos.bottom = rcTextPos.bottom;

					m_pFont->RenderText( pCurItem->m_strSender.c_str(), &rcCalPos, m_dwAlign );
					g_pVideo->Flush();

					rcCalPos.left += iTextRasterLen;	// left renew
				}

				// header icon
				if (pCurItem->m_imgHeader.m_pImage)
				{
					rcCalPos.left += 3;
					rcCalPos.right = rcCalPos.left + pCurItem->m_imgHeader.m_iSX;
					rcCalPos.top = rcTextPos.top - 1;
					rcCalPos.bottom	=	rcTextPos.bottom - 1;

	                pCurItem->m_imgHeader.m_pImage->RenderTexture( &rcCalPos, &(pCurItem->m_imgHeader.m_rcSrc) );
					g_pVideo->Flush();

					rcCalPos.left += pCurItem->m_imgHeader.m_iSX;
				}
			}

			// text
			if( pCurItem->m_strText.empty() == false )
			{
				m_pFont->SetColor( pCurItem->m_stTextColor );
				m_pFont->SetEffect( pCurItem->m_fontEffect );

				rcCalPos.right		=	rcTextPos.right;
				rcCalPos.top		=	rcTextPos.top;
				rcCalPos.bottom		=	rcTextPos.bottom;

				m_pFont->RenderText( pCurItem->m_strText.c_str() , &rcCalPos , m_dwAlign );
				g_pVideo->Flush();
			}

			if (pCurItem->m_bLastLine)
			{
				// tail icon
				if (pCurItem->m_imgTail.m_pImage)
				{
					rcCalPos.left = rcCalPos.left + m_pFont->GetSizeW( pCurItem->m_strText.c_str() ).cx + 3;
					rcCalPos.right	=	rcCalPos.left + pCurItem->m_imgTail.m_iSX;
					if (rcCalPos.right > rcTextPos.right)
					{
						rcCalPos.right = rcTextPos.right + 7;
						rcCalPos.left = rcCalPos.right - pCurItem->m_imgTail.m_iSX;
					}
					rcCalPos.top = rcTextPos.top - 1;
					rcCalPos.bottom	=	rcTextPos.bottom - 1;

					pCurItem->m_imgTail.m_pImage->RenderTexture( &rcCalPos , &(pCurItem->m_imgTail.m_rcSrc) );
					g_pVideo->Flush();
				}
			}

			rcTextPos.top		=	rcTextPos.bottom;
			rcTextPos.bottom	=	rcTextPos.top	+	iOneLineHeight;
		}

		++IterItem;
		++iCurLineCount;

		if( iCurLineCount >= m_iPrintLineCount )
			break;
	}

	m_pFont->SetShadow(	bShadow );
	m_pFont->SetEffect( m_FontEffect );
	m_pFont->SetColor();
	g_pVideo->Flush();
}

void SPWindowList::SetImageRect( const char* pszImage, RECT rcImageSrc )
{
	SAFE_RELEASE( m_pImageNormal );

	g_pVideo->GetTextureMgr()->LoadTexture( pszImage , &m_pImageNormal );
	m_rcImageSrc	= rcImageSrc;
	m_ppImageActive	= &m_pImageNormal;
}

bool SPWindowList::InsertItem( const char* pszListItem , D3DXCOLOR color )
{
	if( pszListItem == NULL )
		return false;

	LIST_ITEM	stAddListItem;

	stAddListItem.m_strText		=	pszListItem;
	stAddListItem.m_stTextColor	=	color;

	InsertItem( &stAddListItem );

	return true;
}

bool SPWindowList::InsertItem( LIST_ITEM* pListItem )
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

	std::vector<LIST_ICON>&				vSrcIconImage	=	pListItem->m_vIconImage;
	std::vector<LIST_ICON>::iterator	Iter			=	vSrcIconImage.begin();
	
	LIST_ICON	stAddListIcon;
	LIST_ITEM	stAddListItem;

	CopyListIcon( stAddListItem.m_stBGImage , pListItem->m_stBGImage );
	CopyListIcon( stAddListItem.m_imgTail , pListItem->m_imgTail );
	CopyListIcon( stAddListItem.m_imgHeader, pListItem->m_imgHeader );
	CopyListIcon( stAddListItem.m_imgChannel, pListItem->m_imgChannel );
	stAddListItem.m_fontEffect = pListItem->m_fontEffect;

	while ( Iter != vSrcIconImage.end() )
	{
		stAddListItem.m_stReturnValue	=	pListItem->m_stReturnValue;

		CopyListIcon( stAddListIcon , (*Iter) );

		rcImagePos.left	+=	stAddListIcon.m_iSX;

		if( rcImagePos.left == rcImagePos.right )
		{
			stAddListItem.m_vIconImage.push_back( stAddListIcon );
//			stAddListIcon.Clear();

			PushBackItem( stAddListItem );

			CopyListIcon( stAddListItem.m_stBGImage , pListItem->m_stBGImage );

			rcImagePos.left	=	rcTextPos.left;
		}
		else if( rcImagePos.left > rcImagePos.right )
		{
			if( (int)stAddListItem.m_vIconImage.size() <= 0 )
			{
				stAddListItem.m_vIconImage.push_back( stAddListIcon );
//				stAddListIcon.Clear();

				PushBackItem( stAddListItem );

				CopyListIcon( stAddListItem.m_stBGImage , pListItem->m_stBGImage );

				rcImagePos.left	=	rcTextPos.left;
			}
			else
			{
				PushBackItem( stAddListItem );

				CopyListIcon( stAddListItem.m_stBGImage , pListItem->m_stBGImage );

				stAddListItem.m_vIconImage.push_back( stAddListIcon );
//				stAddListIcon.Clear();
			}
		}
		else
		{
			stAddListItem.m_vIconImage.push_back( stAddListIcon );
//			stAddListIcon.Clear();
		}

		++Iter;
	}

	//--------------------------------------------------
	// ¥¶¿ÌÕº±Í¿Æ∞»“˝∆µƒ◊÷∑˚¥Æ’€––≈–∂œ.
	if (stAddListItem.m_imgHeader.m_pImage)
	{
		rcImagePos.left += stAddListItem.m_imgHeader.m_iSX;
	}
	//--------------------------------------------------

	//////////////////////////////////////////////////////////////////////////

	std::string		strTempListItem		=	pListItem->m_strText;

	if( m_bNextLine == true )
	{
		char szLineFeedBuf[MAX_STR_LENGTH+1];
		SPAbuseFilter::GetInstance()->ReplaceLineFeed( pListItem->m_strText.c_str() , szLineFeedBuf , MAX_STR_LENGTH );
		m_bNextLine		=	false;
		strTempListItem	=	szLineFeedBuf;
	}

	//////////////////////////////////////////////////////////////////////////

	if( strTempListItem.empty() == false )
	{
		std::vector< std::string >				vTextList;
		std::vector< std::string >::iterator	IterTextList;

		if( (int)stAddListItem.m_vIconImage.size() > 0  )
		{
			m_pFont->ConvertMultiLineW( strTempListItem.c_str() , rcImagePos, vTextList );

			if( (int)vTextList.size() > 1 )
			{
				std::string	strFirst;

				IterTextList	=	vTextList.begin();

				if( IterTextList != vTextList.end() )
				{
					strFirst	=	(*IterTextList);
					++IterTextList;
				}

				strTempListItem.clear();

				while ( IterTextList != vTextList.end() )
				{
					strTempListItem	+=	(*IterTextList);
					++IterTextList;
				}

				if( strTempListItem.empty() == false )
				{
					vTextList.clear();
					m_pFont->ConvertMultiLineW( strTempListItem.c_str() , rcTextPos, vTextList );
					vTextList.insert( vTextList.begin() , strFirst );
				}
			}
		}
		else
		{
			m_pFont->ConvertMultiLineW( strTempListItem.c_str() , rcTextPos, vTextList );
		}

		IterTextList	=	vTextList.begin();
		// process sender & msg
		// sender ends with "]£∫"
		std::string &first_msg = (*IterTextList);
		string::size_type idx = first_msg.find( "]£∫" );
		if ( idx != string::npos)
		{
			stAddListItem.m_strSender = first_msg.substr( 0, idx+3 );
			first_msg = first_msg.substr( idx+3 );
		}

		while ( IterTextList != vTextList.end() )
		{
			if( (*IterTextList).empty() == false )
			{
				stAddListItem.m_strText			=	(*IterTextList);
				stAddListItem.m_stTextColor		=	pListItem->m_stTextColor;
				stAddListItem.m_stReturnValue	=	pListItem->m_stReturnValue;

				if (IterTextList == vTextList.begin())
					stAddListItem.m_bFirstLine = true;
				else
					stAddListItem.m_bFirstLine = false;

				if (IterTextList == --vTextList.end())
					stAddListItem.m_bLastLine = true;
				else
					stAddListItem.m_bLastLine = false;

				PushBackItem( stAddListItem );
				CopyListIcon( stAddListItem.m_stBGImage , pListItem->m_stBGImage );
			}

			++IterTextList;
		}

	}

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

bool SPWindowList::CopyListIcon( LIST_ICON& stDestListIcon , LIST_ICON& stSrcListIcon )
{
	SPTextureMgr*	pTextureMgr	=	g_pVideo->GetTextureMgr();

	if( pTextureMgr == NULL )
		return false;

	stDestListIcon.m_strImageName	=	stSrcListIcon.m_strImageName;
	stDestListIcon.m_rcSrc			=	stSrcListIcon.m_rcSrc;

	if( stSrcListIcon.m_iSX > 0 )
		stDestListIcon.m_iSX	=	stSrcListIcon.m_iSX;
	else
		stDestListIcon.m_iSX	=	stSrcListIcon.m_rcSrc.right		-	stSrcListIcon.m_rcSrc.left;

	if( stSrcListIcon.m_iSY > 0 )
		stDestListIcon.m_iSY	=	stSrcListIcon.m_iSY;
	else
		stDestListIcon.m_iSY	=	stSrcListIcon.m_rcSrc.bottom	-	stSrcListIcon.m_rcSrc.top;

	if( stSrcListIcon.m_pImage == NULL )
	{
		SPTexture* pImage;

		if( pTextureMgr->LoadTexture( stDestListIcon.m_strImageName.c_str() , &pImage ) == true )
		{
			stDestListIcon.m_pImage		=	pImage;
			stDestListIcon.m_bDelete	=	true;
		}
	}
	else
	{
		stDestListIcon.m_pImage		=	stSrcListIcon.m_pImage;
		stDestListIcon.m_bDelete	=	stSrcListIcon.m_bDelete;
	}

	return true;
}

void SPWindowList::PushBackItem( LIST_ITEM& stListItem )
{
	if( m_vListItem.size() >= MAX_HISTORY_ITEM ) 
	{
		std::vector<LIST_ITEM>::iterator	Iter	=	m_vListItem.begin();

		if( Iter != m_vListItem.end() )
		{
			(*Iter).Clear();
		}

		m_vListItem.erase( m_vListItem.begin() );
	}

	m_vListItem.push_back( stListItem );

	stListItem.m_stBGImage.m_strImageName.clear();

	stListItem.m_stBGImage.m_rcSrc.left		=	0;
	stListItem.m_stBGImage.m_rcSrc.top		=	0;
	stListItem.m_stBGImage.m_rcSrc.right	=	0;
	stListItem.m_stBGImage.m_rcSrc.bottom	=	0;

	stListItem.m_stBGImage.m_pImage			=	NULL;

	stListItem.m_stBGImage.m_iSX			=	0;
	stListItem.m_stBGImage.m_iSY			=	0;
	stListItem.m_stBGImage.m_bDelete		=	false;

	stListItem.m_vIconImage.clear();

	stListItem.m_strText.clear();

	stListItem.m_stTextColor		=	D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

	stListItem.m_rcDrawPos.left		=	0;
	stListItem.m_rcDrawPos.top		=	0;
	stListItem.m_rcDrawPos.right	=	0;
	stListItem.m_rcDrawPos.bottom	=	0;
}

void SPWindowList::ClearListItem()
{
	std::vector<LIST_ITEM>::iterator	Iter	=	m_vListItem.begin();

	while( Iter != m_vListItem.end() )
	{
		(*Iter).Clear();
		++Iter;
	}

	m_vListItem.clear();
}

LIST_RETURNVALUE* SPWindowList::GetReturnValue( int iXPos , int iYPos )
{
	LIST_RETURNVALUE*					pFindData	=	NULL;
	std::vector<LIST_ITEM>::iterator	Iter		=	m_vListItem.begin()	+	m_iBeginLine;

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

int SPWindowList::ScrollList( int iSubSpim /* = SUB_SPIM_SCROLL_LATEST  */, LPARAM lParam /* = 0  */ )
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
	case SUB_SPIM_SCROLL_SET_LINE:
		{
			int	iSetLIne	=	(int)lParam;

			if( iSetLIne >= 0 && iSetLIne <= iStrSize - m_iPrintLineCount )
				m_iBeginLine = iSetLIne;
		}
		break;
	}

	if( iStrSize <= m_iBeginLine + m_iPrintLineCount )
	{
		m_bLatest		=	true;
		m_bAddString	=	false;
	}
	else
		m_bLatest	=	false;

	return m_iBeginLine;
}

void SPWindowList::SetMargin( int iLeft , int iTop , int iRight , int iBottom )
{
	SetRect( &m_rcMargin , iLeft , iTop , iRight , iBottom );
}

bool SPWindowList::GetLatest()
{
	return m_bLatest;
}

bool SPWindowList::IsAddString()
{
	return m_bAddString;
}

void SPWindowList::SetWheelUse( bool bUse )
{
	m_bWheelUse	=	bUse;
}

void SPWindowList::SetNextLine( bool bUse )
{
	m_bNextLine	=	bUse;
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowList )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER			,	OnCursorEnter		)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT				,	OnCursorOut			)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN			,	OnLButtonDown		)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP				,	OnLButtonUP			)
	SPIMESSAGE_COMMAND(	SPIM_ADD_ITEM				,	OnAddItem			)
	SPIMESSAGE_COMMAND(	SPIM_INSERT_ITEM			,	OnAddItem			)	
	SPIMESSAGE_COMMAND(	SPIM_SCROLL					,	OnScroll			)
	SPIMESSAGE_COMMAND(	SPIM_PURGE					,	OnPurge				)
	SPIMESSAGE_COMMAND(	SPIM_WHEEL_UP				,	OnWheelUp			)
	SPIMESSAGE_COMMAND(	SPIM_WHEEL_DOWN				,	OnWheelDown			)
	SPIMESSAGE_COMMAND(	SPIM_GET_CALC_LUMP_STRING	,	OnGetCalcLumpString	)
	
	
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowList::OnCursorEnter( WPARAM wParam, LPARAM lParam)
{
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	return SPSendMessageToParent(SPIM_CURSOR_ENTER, wParam, lParam, this);	
}

int SPWindowList::OnCursorOut( WPARAM wParam, LPARAM lParam)
{
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0x00);
	return SPSendMessageToParent(SPIM_CURSOR_OUT, wParam, lParam, this);
}

int SPWindowList::OnLButtonDown( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_LBUTTON_DOWN, wParam, lParam, this);
}

int SPWindowList::OnLButtonUP( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, lParam, this);
}

int SPWindowList::OnAddItem( WPARAM wParam, LPARAM lParam)
{
	D3DXCOLOR*	pColor	=	(D3DXCOLOR*)lParam;	

	if( pColor == NULL ) {
		return InsertItem( (char*)wParam , D3DXCOLOR(1,1,1,1) );
	}
	//else {
	//	
	//	DXUTOutputDebugString(" - SPWindowList::OnAddItem [%.2f] [%.2f] [%.2f]\n", pColor->r, pColor->g, pColor->b);
	//}

	return InsertItem( (char*)wParam , *pColor );
}

int SPWindowList::OnScroll	( WPARAM wParam, LPARAM lParam)
{
	int iCurLine = ScrollList( (int)wParam , lParam );
	return SPSendMessageToParent(SPIM_SCROLL, iCurLine, lParam, this);
}

int SPWindowList::OnPurge	( WPARAM wParam, LPARAM lParam)
{
	m_iBeginLine		= 0;
	m_iPrintLineCount	= 0;

	ClearListItem();

	m_bLatest	=	true;

	return SPSendMessageToParent(SPIM_PURGE, wParam, lParam, this);
}

int SPWindowList::OnWheelUp( WPARAM wParam , LPARAM lParam )
{
	if( m_bWheelUse == true )
		ScrollList( SUB_SPIM_SCROLL_UP );

	return SPSendMessageToParent( SPIM_WHEEL_UP , wParam , lParam , this );
}

int SPWindowList::OnWheelDown( WPARAM wParam , LPARAM lParam )
{
	if( m_bWheelUse == true )
		ScrollList( SUB_SPIM_SCROLL_DOWN );

	return SPSendMessageToParent( SPIM_WHEEL_DOWN , wParam , lParam , this );
}


int SPWindowList::OnGetCalcLumpString( WPARAM wParam , LPARAM lParam )
{
	m_strLogString.clear();

	std::vector<LIST_ITEM>::iterator Iter = m_vListItem.begin();

	for(; Iter != m_vListItem.end(); ++Iter) {
		m_strLogString += (*Iter).m_strText.c_str();
		m_strLogString += '\n';
	}

	return (int)m_strLogString.c_str();
}

int SPWindowList::GetBeginLine()
{
	return m_iBeginLine;
}

int SPWindowList::GetEndLine()
{
	int iStrSize	= (int)m_vListItem.size();

	return iStrSize - m_iPrintLineCount;
}
