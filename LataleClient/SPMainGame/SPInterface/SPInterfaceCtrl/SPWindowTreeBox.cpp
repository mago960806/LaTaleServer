// ***************************************************************
//  SPWindowTreeBox.cpp   version:  1.0   date:2006/01/13
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 AJJIYA - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPGameObjectDEF.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowSlider.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"
#include "SPTreeBoxItemTexture.h"

#include "SPWindowTreeBox.h"

#include "SPDebug.h"

//////////////////////////////////////////////////////////////////////////

SPWindowTreeBox::SPWindowTreeBox( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent )
: SPWindow( WNDID_CTRL_TREEBOX , InstanceID , iX , iY , iCX , iCY , pParent )
{
	Init();
}

SPWindowTreeBox::~SPWindowTreeBox()
{
	Clean();
}

void SPWindowTreeBox::Init()
{
	m_strToolTip = "[SPWindowTreeBox]";

	m_vBGTexture.reserve( TREEBOX_TEXTURE_COUNT );

	TREEBOX_TEXTURE_INFO	stTexture;

	stTexture.m_pTexture		=	NULL;
	stTexture.m_rcPos.left		=	0;
	stTexture.m_rcPos.top		=	0;
	stTexture.m_rcPos.right		=	0;
	stTexture.m_rcPos.bottom	=	0;
	stTexture.m_rcSrc.left		=	0;
	stTexture.m_rcSrc.top		=	0;
	stTexture.m_rcSrc.right		=	0;
	stTexture.m_rcSrc.bottom	=	0;
	stTexture.m_ptSize.x		=	0;
	stTexture.m_ptSize.y		=	0;

	for( int i = 0 ; i < TREEBOX_TEXTURE_COUNT ; i++ )
	{
		m_vBGTexture.push_back( stTexture );
	}

	m_pRootItem		=	NULL;
	m_pStartItem	=	NULL;

	m_pSelectItem	=	NULL;
	m_pMouseOver	=	NULL;

	SetMargin( 0 , 0  , 0 , 0 );
	SetInterval( 0 );

	m_bScrollEnable	=	true;
	m_bAutoHide		=	true;
	m_pScrollUp		=	new SPWindowButton( GetInstanceID() + TREEBOX_BUTTON_UP		, 0 , 0 , 0 , 0 , this );
	m_pScrollDown	=	new SPWindowButton( GetInstanceID() + TREEBOX_BUTTON_DOWN	, 0 , 0 , 0 , 0 , this );
	m_pScrollSlider	=	new SPWindowSlider( GetInstanceID() + TREEBOX_BUTTON_SLIDER	, 0 , 0 , 0 , 0 , this );
	m_pScrollSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );

	m_iScrollCurPos	=	0;

	m_bDoubleClick	=	false;

	CreateRootItem();
}

void SPWindowTreeBox::Clean()
{
	DelItemAll();

	STD_VECTOR_TREEBOX_TEXTURE::iterator	IterTexture = m_vBGTexture.begin();

	while( IterTexture != m_vBGTexture.end() )
	{
		SAFE_RELEASE( (*IterTexture).m_pTexture );
		++IterTexture;
	}

	m_vBGTexture.clear();

	SPWindow::Clean();
}

void SPWindowTreeBox::Process( float fTime )
{
	if( !m_bShow )
		return;

	if( m_bUpdate == true )
	{
		m_bUpdate = false;

		int	iPosX	=	m_iAX	+	m_rcMargin.left;
		int	iPosY	=	m_iAY	+	m_rcMargin.top;
		int	iCurPos	=	0;

		m_vpViewItem.clear();

		FindViewItem( iCurPos , m_pRootItem , m_pStartItem );

		m_vpRenderItem.clear();

		int	iNewPos = ProcessItem( iCurPos , iPosX , iPosY , fTime );

		if( iNewPos != -1 )
		{
			m_vpRenderItem.clear();
			ProcessItem( iNewPos , iPosX , iPosY , fTime );
			iCurPos	=	iNewPos;
		}

		int	iSliderMaxCount	=	(int)m_vpViewItem.size() - (int)m_vpRenderItem.size() + 1;

		if( iSliderMaxCount <= 1 && m_bAutoHide == true )
		{
			m_pScrollUp->Hide();
			m_pScrollDown->Hide();
			m_pScrollSlider->Hide();
		}
		else
		{
			if( m_bScrollEnable == true )
			{
				m_pScrollUp->Show();
				m_pScrollDown->Show();
				m_pScrollSlider->Show();
			}
		}

		m_pScrollSlider->SPSendMessage( SPIM_SET_MAXVALUE , iSliderMaxCount );
		m_pScrollSlider->SPSendMessage( SPIM_SET_CURVALUE , iCurPos );

		if( g_pInterfaceManager != NULL )
			OnCursorMove( 0 , MAKELONG( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() ) );
	}

	SPWindow::Process( fTime );
}

void SPWindowTreeBox::Render( float fTime )
{
	if( !m_bShow )
		return;

	g_pVideo->Flush();

	TREEBOX_TEXTURE_INFO*	pHelpTexture	=	NULL;

	for( int i = 0 ; i < TREEBOX_TEXTURE_COUNT ; i++ )
	{
		pHelpTexture	=	&m_vBGTexture.at( i );

		if( pHelpTexture->m_pTexture == NULL )
			continue;

		pHelpTexture->m_pTexture->SetColor();
		pHelpTexture->m_pTexture->RenderTexture( &pHelpTexture->m_rcPos , &pHelpTexture->m_rcSrc );
	}

	g_pVideo->Flush();

	STD_VECTOR_TREEBOX_ITEM::iterator	IterRenderItem	=	m_vpRenderItem.begin();

	while ( IterRenderItem != m_vpRenderItem.end() )
	{
		(*IterRenderItem)->m_pItemBase->Render( fTime );
		++IterRenderItem;
	}

	g_pVideo->Flush();

	SPWindow::Render( fTime );

	g_pVideo->Flush();

	//RECT	rtTemp;

	//rtTemp.left	=	m_iAX + m_rcMargin.left;
	//rtTemp.top	=	m_iAY + m_rcMargin.top;
	//rtTemp.right	=	m_iAX + m_iSX - m_rcMargin.right;
	//rtTemp.bottom	=	m_iAY + m_iSY - m_rcMargin.bottom;

	//g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 1.0f , 0.0f , 0.0f , 1.0f ) );
	//g_pVideo->GetSysLine()->DrawRect( rtTemp );
	//g_pVideo->GetSysLine()->SetColor();
}

void SPWindowTreeBox::Show( bool bWithChild /* = true  */ )
{
//	SPWindow::Show( bWithChild );
	m_bShow			=	true;
	m_bLButtonDown	=	false;
}

void SPWindowTreeBox::Hide( bool bSendServer /* = true  */ )
{
//	SPWindow::Hide( bSendServer );
	m_bShow			=	false;
	m_bLButtonDown	=	false;
}

void SPWindowTreeBox::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	UpdateBGPos();
	UpdateScrollPos();
	Refresh();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowTreeBox )

SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP		,												OnLButtonUp			)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DBLCLICK,												OnDblLButtonUp		)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN	,												OnLButtonDown		)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE	,												OnCursorMove		)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT		,												OnCursorOut			)
SPIMESSAGE_COMMAND(	SPIM_WHEEL_UP		,												OnWheelUp			)
SPIMESSAGE_COMMAND(	SPIM_WHEEL_DOWN		,												OnWheelDown			)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	GetInstanceID() + TREEBOX_BUTTON_UP		,	OnWheelUp			)
SPIMESSAGE_CONTROL(	SPIM_CURSOR_ENTER	,	GetInstanceID() + TREEBOX_BUTTON_UP		,	OnSliderCursorEnter	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	GetInstanceID() + TREEBOX_BUTTON_DOWN	,	OnWheelDown			)
SPIMESSAGE_CONTROL(	SPIM_CURSOR_ENTER	,	GetInstanceID() + TREEBOX_BUTTON_DOWN	,	OnSliderCursorEnter	)

SPIMESSAGE_CONTROL(	SPIM_SLIDER_UP		,	GetInstanceID() + TREEBOX_BUTTON_SLIDER	,	OnSliderPage		)
SPIMESSAGE_CONTROL(	SPIM_SLIDER_DOWN	,	GetInstanceID() + TREEBOX_BUTTON_SLIDER	,	OnSliderPage		)
SPIMESSAGE_CONTROL(	SPIM_SLIDER_PAGEUP	,	GetInstanceID() + TREEBOX_BUTTON_SLIDER	,	OnSliderPage		)
SPIMESSAGE_CONTROL(	SPIM_SLIDER_PAGEDOWN,	GetInstanceID() + TREEBOX_BUTTON_SLIDER	,	OnSliderPage		)
SPIMESSAGE_CONTROL(	SPIM_SET_CURVALUE	,	GetInstanceID() + TREEBOX_BUTTON_SLIDER	,	OnSliderPage		)
SPIMESSAGE_CONTROL(	SPIM_CURSOR_ENTER	,	GetInstanceID() + TREEBOX_BUTTON_SLIDER	,	OnSliderCursorEnter	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

void SPWindowTreeBox::SetTexture( TREEBOX_TEXTURE eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY )
{
	int	iTextureType	=	(int)eTextureType;

	if( iTextureType >= TREEBOX_TEXTURE_COUNT )
		return;

	SPTexture*		pTexture	=	NULL;

	g_pVideo->GetTextureMgr()->LoadTexture( strFileName , &pTexture );

	TREEBOX_TEXTURE_INFO&	stTexture	=	m_vBGTexture.at( iTextureType );

	stTexture.m_rcSrc.left		=	iSrcSX;
	stTexture.m_rcSrc.top		=	iSrcSY;
	stTexture.m_rcSrc.right		=	iSrcEX;
	stTexture.m_rcSrc.bottom	=	iSrcEY;
	stTexture.m_ptSize.x		=	iSrcEX	-	iSrcSX;
	stTexture.m_ptSize.y		=	iSrcEY	-	iSrcSY;

	SAFE_RELEASE( stTexture.m_pTexture );

	stTexture.m_pTexture	=	pTexture;
}

void SPWindowTreeBox::SetBoxTexture( const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY , int iStep )
{
	SetTexture( TREEBOX_TEXTURE_LEFT_TOP		, strFileName , iSrcSX			, iSrcSY , iSrcSX + iStep	, iSrcSY + iStep );
	SetTexture( TREEBOX_TEXTURE_CENTER_TOP		, strFileName , iSrcSX + iStep	, iSrcSY , iSrcEX - iStep	, iSrcSY + iStep );
	SetTexture( TREEBOX_TEXTURE_RIGHT_TOP		, strFileName , iSrcEX - iStep	, iSrcSY , iSrcEX			, iSrcSY + iStep );

	SetTexture( TREEBOX_TEXTURE_LEFT_MIDDLE		, strFileName , iSrcSX			, iSrcSY + iStep , iSrcSX + iStep	, iSrcSY + iStep * 2 );
	SetTexture( TREEBOX_TEXTURE_CENTER_MIDDLE	, strFileName , iSrcSX + iStep	, iSrcSY + iStep , iSrcEX - iStep	, iSrcSY + iStep * 2);
	SetTexture( TREEBOX_TEXTURE_RIGHT_MIDDLE	, strFileName , iSrcEX - iStep	, iSrcSY + iStep , iSrcEX			, iSrcSY + iStep * 2);

	SetTexture( TREEBOX_TEXTURE_LEFT_BOTTOM		, strFileName , iSrcSX			, iSrcEY - iStep , iSrcSX + iStep	, iSrcEY );
	SetTexture( TREEBOX_TEXTURE_CENTER_BOTTOM	, strFileName , iSrcSX + iStep	, iSrcEY - iStep , iSrcEX - iStep	, iSrcEY );
	SetTexture( TREEBOX_TEXTURE_RIGHT_BOTTOM	, strFileName , iSrcEX - iStep	, iSrcEY - iStep , iSrcEX			, iSrcEY );
}

void SPWindowTreeBox::UpdateBGPos()
{
	int		iLeft	=	m_iAX;
	int		iTop	=	m_iAY;
	int		iRight	=	m_iAX + m_iSX;
	int		iBottom	=	m_iAY + m_iSY;

	TREEBOX_TEXTURE_INFO*	pBGInfo	=	NULL;

	//////////////////////////////////////////////////////////////////////////
	//
	//	탑라인
	//

	pBGInfo	=	&m_vBGTexture.at( TREEBOX_TEXTURE_LEFT_TOP );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left	+	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	pBGInfo->m_rcPos.top	+	pBGInfo->m_ptSize.y;

	int		iMiddleSX	=	pBGInfo->m_rcPos.right;
	int		iMiddleSY	=	pBGInfo->m_rcPos.bottom;

	pBGInfo	=	&m_vBGTexture.at( TREEBOX_TEXTURE_RIGHT_TOP );

	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.left	=	pBGInfo->m_rcPos.right	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	pBGInfo->m_rcPos.top	+	pBGInfo->m_ptSize.y;

	int		iMiddleEX	=	pBGInfo->m_rcPos.left;

	pBGInfo	=	&m_vBGTexture.at( TREEBOX_TEXTURE_CENTER_TOP );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iMiddleSY;

	//////////////////////////////////////////////////////////////////////////
	//
	//	BOTTOM라인

	pBGInfo	=	&m_vBGTexture.at( TREEBOX_TEXTURE_LEFT_BOTTOM );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.bottom	=	iBottom;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left	+	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	pBGInfo->m_rcPos.bottom	-	pBGInfo->m_ptSize.y;

	pBGInfo	=	&m_vBGTexture.at( TREEBOX_TEXTURE_RIGHT_BOTTOM );

	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.bottom	=	iBottom;
	pBGInfo->m_rcPos.left	=	pBGInfo->m_rcPos.right	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	pBGInfo->m_rcPos.bottom	-	pBGInfo->m_ptSize.y;

	int		iMiddleEY	=	pBGInfo->m_rcPos.top;

	pBGInfo	=	&m_vBGTexture.at( TREEBOX_TEXTURE_CENTER_BOTTOM );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iMiddleEY;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iBottom;

	//////////////////////////////////////////////////////////////////////////
	//
	//	가운데라인

	pBGInfo	=	&m_vBGTexture.at( TREEBOX_TEXTURE_LEFT_MIDDLE );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left + pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;

	pBGInfo	=	&m_vBGTexture.at( TREEBOX_TEXTURE_CENTER_MIDDLE );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;

	pBGInfo	=	&m_vBGTexture.at( TREEBOX_TEXTURE_RIGHT_MIDDLE );

	pBGInfo->m_rcPos.left	=	iRight	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;
}

void SPWindowTreeBox::UpdateScrollPos()
{
	TREEBOX_TEXTURE_INFO*	pTopBGInfo		=	NULL;
	TREEBOX_TEXTURE_INFO*	pBottomBGInfo	=	NULL;
	int						iSX				=	0;
	int						iSY				=	0;
	int						iEY				=	0;
	int						iScrollWidth	=	GetScrollWidth();
	RECT					rcScroll;

//	iScrollWidth	+=	m_rcScrollMargin.right;

	pTopBGInfo		=	&m_vBGTexture.at( TREEBOX_TEXTURE_RIGHT_TOP );
	pBottomBGInfo	=	&m_vBGTexture.at( TREEBOX_TEXTURE_RIGHT_BOTTOM );
	iSX				=	pTopBGInfo->m_rcPos.right	-	m_iAX	-	iScrollWidth;

	iSY	=	pTopBGInfo->m_rcPos.top			+	m_rcScrollMargin.top	-	m_iAY;
	iEY	=	pBottomBGInfo->m_rcPos.bottom	-	m_rcScrollMargin.bottom	-	m_iAY;

	m_pScrollUp->AdjustCoord( iSX , iSY );
	m_pScrollUp->GetWindowRect( rcScroll );

	iSY	+=	rcScroll.bottom	-	rcScroll.top;

	m_pScrollDown->GetWindowRect( rcScroll );

	iEY	-=	rcScroll.bottom	-	rcScroll.top;

	m_pScrollDown->AdjustCoord( iSX , iEY );

	m_pScrollSlider->GetWindowRect( rcScroll );
	m_pScrollSlider->AdjustCoord( iSX , iSY , rcScroll.right - rcScroll.left , iEY - iSY );
	m_pScrollSlider->SPSendMessage( SPIM_POS_UPDATE );
}

bool SPWindowTreeBox::IsEmpty()
{
	bool	bResult	=	true;

	if( m_pRootItem != NULL )
	{
		if( (int)m_pRootItem->m_vpChild.size() > 0 )
			bResult	=	false;
	}

	return bResult;
}

TREEBOX_ITEM*	SPWindowTreeBox::AddItem( TREEBOX_ITEM* pParent , SPTreeBoxItemBase* pItemBase )
{
	if( pParent == NULL )
	{
		CreateRootItem();

		pParent	=	m_pRootItem;
	}
	else
	{
		if( FindItem( pParent ) == false )
			return NULL;
	}

	TREEBOX_ITEM*	pItem	=	new	TREEBOX_ITEM;

	pItem->m_bSelect	=	false;
	pItem->m_bShow		=	true;
	pItem->m_pItemBase	=	pItemBase;
	pItem->m_pParent	=	pParent;
	pItem->m_vpChild.clear();

	pParent->m_vpChild.push_back( pItem );

	//if( pParent->m_pItemBase != NULL )
	//	pParent->m_pItemBase->AddStatus( DATA_STATUS_PULLDOWN );
	
	return pItem;
}

TREEBOX_ITEM*	SPWindowTreeBox::InsertItem( TREEBOX_ITEM* pParent , SPTreeBoxItemBase* pItemBase , int iPos )
{
	if( pParent == NULL )
	{
		CreateRootItem();

		pParent	=	m_pRootItem;
	}
	else
	{
		if( FindItem( pParent ) == false )
			return NULL;
	}

	TREEBOX_ITEM*	pItem	=	new	TREEBOX_ITEM;

	pItem->m_bSelect	=	false;
	pItem->m_bShow		=	true;
	pItem->m_pItemBase	=	pItemBase;
	pItem->m_pParent	=	pParent;
	pItem->m_vpChild.clear();

	if( iPos >= (int)pParent->m_vpChild.size() )
		pParent->m_vpChild.push_back( pItem );
	else
		pParent->m_vpChild.insert( pParent->m_vpChild.begin() + iPos , pItem );

	//if( pParent->m_pItemBase != NULL )
	//	pParent->m_pItemBase->AddStatus( DATA_STATUS_PULLDOWN );

	return pItem;
}

TREEBOX_ITEM*	SPWindowTreeBox::GetSelectItem()
{
	return m_pSelectItem;
}

void SPWindowTreeBox::SetSelectItem( TREEBOX_ITEM* pSelectItem )
{
	m_pSelectItem = pSelectItem;
}

bool SPWindowTreeBox::DelItem( TREEBOX_ITEM** ppItem , bool bParent /* = false  */ )
{
	if( ppItem == NULL || (*ppItem) == NULL )
		return false;

	STD_VECTOR_TREEBOX_ITEM::iterator		IterItem	=	(*ppItem)->m_vpChild.begin();
	TREEBOX_ITEM*							pFindItem	=	NULL;

	while ( IterItem != (*ppItem)->m_vpChild.end() )
	{
		pFindItem	=	(*IterItem);

		if( DelItem( &pFindItem , true ) == true )
		{
			IterItem = (*ppItem)->m_vpChild.erase( IterItem );

			Refresh();
			Process( 0.0f );
		}
		else
            ++IterItem;
	}

	if( (*ppItem)->m_vpChild.empty() == true )
	{
		if( (*ppItem)->m_pParent != NULL )
		{
			if( (*ppItem)->m_pParent->m_pItemBase != NULL )
			{
				if( (*ppItem)->m_pParent->m_bSelect == true && (int)(*ppItem)->m_pParent->m_vpChild.size() > 0 )
					(*ppItem)->m_pParent->m_pItemBase->AddStatus( DATA_STATUS_PULLDOWN );
				else
					(*ppItem)->m_pParent->m_pItemBase->DelStatus( DATA_STATUS_PULLDOWN );
			}
		}

		if( m_pSelectItem == (*ppItem) )
			m_pSelectItem	=	NULL;

		if( m_pMouseOver == (*ppItem) )
			m_pMouseOver	=	NULL;

		if( m_pStartItem == (*ppItem) )
		{
			OnWheelUp( NULL , NULL );
		}

		if( bParent == false )
		{
			std::vector< TREEBOX_ITEM* >::iterator	Iter	=	(*ppItem)->m_pParent->m_vpChild.begin();
			TREEBOX_ITEM*							pIterItem;

			while( Iter != (*ppItem)->m_pParent->m_vpChild.end() )
			{
				pIterItem	=	(*Iter);

				if( (*ppItem) == pIterItem )
				{
					(*ppItem)->m_pParent->m_vpChild.erase( Iter );
					break;
				}

				++Iter;
			}

		}

		SAFE_DELETE( (*ppItem)->m_pItemBase );
		SAFE_DELETE( (*ppItem) );
		ppItem	=	NULL;

		return true;
	}

	return false;
}

void SPWindowTreeBox::DelItemAll()
{
	DelItem( &m_pRootItem , true );

	m_vpRenderItem.clear();
	m_vpViewItem.clear();

	m_pRootItem		=	NULL;
	m_pStartItem	=	NULL;

	m_pSelectItem	=	NULL;
	m_pMouseOver	=	NULL;
}

bool SPWindowTreeBox::FindItem( TREEBOX_ITEM* pItem , TREEBOX_ITEM* pStartItem /* = NULL  */ )
{
	if( pStartItem == NULL )
	{
		if( m_pRootItem == NULL )
			return false;

		pStartItem	=	m_pRootItem;
	}

	if( pItem == NULL || pStartItem == NULL )
		return false;

	STD_VECTOR_TREEBOX_ITEM::iterator		IterItem	=	pStartItem->m_vpChild.begin();
	TREEBOX_ITEM*							pFindItem	=	NULL;

	while ( IterItem != pStartItem->m_vpChild.end() )
	{
		pFindItem	=	(*IterItem);

		if( pFindItem != NULL)
		{
			if( pFindItem == pItem )
				return true;

			if( pFindItem->m_vpChild.empty() == false )
			{
				if( FindItem( pItem , pFindItem ) == true )
					return true;
			}
		}

		++IterItem;
	}

	return false;
}

TREEBOX_ITEM* SPWindowTreeBox::GetRootItem()
{
	return m_pRootItem;
}

void SPWindowTreeBox::Refresh()
{
	m_bUpdate	=	true;
}

void SPWindowTreeBox::SetMargin( int iLeft , int iTop , int iRight , int iBottom )
{
	m_rcMargin.left		=	iLeft;
	m_rcMargin.top		=	iTop;
	m_rcMargin.right	=	iRight;
	m_rcMargin.bottom	=	iBottom;
	Refresh();
}

void SPWindowTreeBox::SetInterval( int iInterval )
{
	m_iInterval	=	iInterval;
	Refresh();
}

void SPWindowTreeBox::SetScrollEnable( bool bEnable )
{
	m_bScrollEnable	=	bEnable;
}

void SPWindowTreeBox::SetScrollAutoHide( bool bAutoHide )
{
	m_bAutoHide	=	bAutoHide;
}

void SPWindowTreeBox::SetScrollMargin( int iLeft , int iTop , int iRight , int iBottom )
{
	m_rcScrollMargin.left	=	iLeft;
	m_rcScrollMargin.top	=	iTop;
	m_rcScrollMargin.right	=	iRight;
	m_rcScrollMargin.bottom	=	iBottom;

	UpdateScrollPos();
}

void SPWindowTreeBox::SetScrollUpSize( int iX , int iY )
{
	if( m_pScrollUp == NULL )
		return;

	RECT	rcScrollUp;

	m_pScrollUp->GetWindowRect( rcScrollUp );

	int		iPosX	=	rcScrollUp.left	-	m_iAX;
	int		iPosY	=	rcScrollUp.top	-	m_iAY;


	m_pScrollUp->AdjustCoord( iPosX , iPosY , iX , iY );
}

void SPWindowTreeBox::SetScrollDownSize( int iX , int iY )
{
	if( m_pScrollDown == NULL )
		return;

	RECT	rcScrollDown;

	m_pScrollDown->GetWindowRect( rcScrollDown );

	int		iPosX	=	rcScrollDown.left	-	m_iAX;
	int		iPosY	=	rcScrollDown.top	-	m_iAY;


	m_pScrollDown->AdjustCoord( iPosX , iPosY , iX , iY );
}

void SPWindowTreeBox::SetScrollSliderSize( int iX , int iY )
{
	if( m_pScrollSlider == NULL )
		return;

	RECT	rcScrollSlider;

	m_pScrollSlider->GetWindowRect( rcScrollSlider );

	int		iPosX	=	rcScrollSlider.left	-	m_iAX;
	int		iPosY	=	rcScrollSlider.top	-	m_iAY;


	m_pScrollSlider->AdjustCoord( iPosX , iPosY , iX , iY );
}

void SPWindowTreeBox::SetScrollUpTexture( TREEBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY )
{
	if( m_pScrollUp == NULL )
		return;

	switch( eTextureType)
	{
	case DATA_TEXTURE_ENABLE:
		m_pScrollUp->SetImage( strFileName , iSrcSX , iSrcSY );
		break;
	case DATA_TEXTURE_HIT:
		m_pScrollUp->SetImageHit( strFileName , iSrcSX , iSrcSY );
		break;
	case DATA_TEXTURE_PUSH:
		m_pScrollUp->SetImagePush( strFileName , iSrcSX , iSrcSY );
		break;
	case DATA_TEXTURE_DISABLE:
		m_pScrollUp->SetImageDisable( strFileName , iSrcSX , iSrcSY );
		break;
	}
}

void SPWindowTreeBox::SetScrollDownTexture( TREEBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY )
{
	if( m_pScrollDown == NULL )
		return;

	switch( eTextureType)
	{
	case DATA_TEXTURE_ENABLE:
		m_pScrollDown->SetImage( strFileName , iSrcSX , iSrcSY );
		break;
	case DATA_TEXTURE_HIT:
		m_pScrollDown->SetImageHit( strFileName , iSrcSX , iSrcSY );
		break;
	case DATA_TEXTURE_PUSH:
		m_pScrollDown->SetImagePush( strFileName , iSrcSX , iSrcSY );
		break;
	case DATA_TEXTURE_DISABLE:
		m_pScrollDown->SetImageDisable( strFileName , iSrcSX , iSrcSY );
		break;
	}
}

void SPWindowTreeBox::SetScrollSliderTexture( TREEBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY )
{
	if( m_pScrollSlider == NULL )
		return;

	RECT	rcScrollSlider;

	m_pScrollSlider->GetWindowRect( rcScrollSlider );

	int		iSX	=	rcScrollSlider.right	-	rcScrollSlider.left;
	int		iSY	=	rcScrollSlider.bottom	-	rcScrollSlider.top;

	rcScrollSlider.left		=	iSrcSX;
	rcScrollSlider.top		=	iSrcSY;
	rcScrollSlider.right	=	rcScrollSlider.left	+	iSX;
	rcScrollSlider.bottom	=	rcScrollSlider.top	+	iSY;

	switch( eTextureType)
	{
	case DATA_TEXTURE_ENABLE:
		m_pScrollSlider->SetImageHandle( strFileName , rcScrollSlider );
		break;
	case DATA_TEXTURE_HIT:
		m_pScrollSlider->SetImageHandleHit( strFileName , rcScrollSlider );
		break;
	case DATA_TEXTURE_PUSH:
		m_pScrollSlider->SetImageHandlePush( strFileName , rcScrollSlider );
		break;
	case DATA_TEXTURE_DISABLE:
		m_pScrollSlider->SetImageHandleDisable( strFileName , rcScrollSlider );
		break;
	}
}

void SPWindowTreeBox::SetScrollSliderBGTexture( TREEBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY )
{
	if( m_pScrollSlider == NULL )
		return;

	RECT	rcScrollSlider;

	rcScrollSlider.left		=	iSrcSX;
	rcScrollSlider.top		=	iSrcSY;
	rcScrollSlider.right	=	iSrcEX;
	rcScrollSlider.bottom	=	iSrcEY;

	switch( eTextureType)
	{
	case DATA_TEXTURE_ENABLE:
		m_pScrollSlider->SetImagePageNormal( strFileName , rcScrollSlider );
		break;
	case DATA_TEXTURE_HIT:
		m_pScrollSlider->SetImagePageHit( strFileName , rcScrollSlider );
		break;
	case DATA_TEXTURE_PUSH:
		m_pScrollSlider->SetImagePageShadow( strFileName , rcScrollSlider );
		break;
	case DATA_TEXTURE_DISABLE:
		m_pScrollSlider->SetImagePageDisable( strFileName , rcScrollSlider );
		break;
	}
}

int SPWindowTreeBox::GetTreeBoxWidth( int iSizeX )
{
	return iSizeX + m_rcMargin.left + m_rcMargin.right + GetScrollWidth();
}

int SPWindowTreeBox::GetScrollWidth()
{
	int						iTemp			=	0;
	int						iScrollWidth	=	0;
	RECT					rcScroll;

	m_pScrollUp->GetWindowRect( rcScroll );

	iScrollWidth	=	rcScroll.right	-	rcScroll.left;

	m_pScrollDown->GetWindowRect( rcScroll );

	iTemp	=	rcScroll.right	-	rcScroll.left;

	if( iScrollWidth < iTemp )
		iScrollWidth	=	iTemp;

	m_pScrollSlider->GetWindowRect( rcScroll );

	iTemp	=	rcScroll.right	-	rcScroll.left;

	if( iScrollWidth < iTemp )
		iScrollWidth	=	iTemp;

	iScrollWidth	+=	m_rcScrollMargin.right;

	return iScrollWidth;
}

void SPWindowTreeBox::SetDoubleClick( bool bDoubleClick )
{
	m_bDoubleClick	=	bDoubleClick;
}

void SPWindowTreeBox::Reset()
{
	ResetItem( m_pRootItem );
	if( m_pRootItem != NULL )
		m_pRootItem->m_bSelect	=	true;
	Refresh();
}

int SPWindowTreeBox::GetItemCount( bool bRootStart , TREEBOX_ITEM* pItem /* = NULL  */ )
{
	int	iCount = 0;

	if( bRootStart == true )
		pItem	=	m_pRootItem;

	if( pItem == NULL )
		return iCount;

	std :: vector< TREEBOX_ITEM* >::iterator	Iter	=	pItem->m_vpChild.begin();

	while( Iter != pItem->m_vpChild.end() )
	{
		iCount	+=	GetItemCount( false , (*Iter) );
		++Iter;
	}

	if( bRootStart == false )
		++iCount;

	return iCount;
}

int SPWindowTreeBox::OnLButtonUp( WPARAM wParam, LPARAM lParam )
{
	if( m_bLButtonDown == false )
		return 0;

	TREEBOX_ITEM*	pItem	=	m_pMouseOver;
//	TREEBOX_ITEM*	pItem	=	FindRenderItem( lParam );

	if( m_pMouseOver != NULL )
	{
		if( m_pMouseOver->m_pItemBase != NULL )
			m_pMouseOver->m_pItemBase->DelStatus( DATA_STATUS_PUSH );
	}

	if( pItem != NULL )
	{
		if( m_pSelectItem != NULL )
		{
			m_pSelectItem->m_pItemBase->SetStatus( DATA_STATUS_ENABLE );

			if( m_pSelectItem->m_bSelect == true && (int)m_pSelectItem->m_vpChild.size() > 0 )
				m_pSelectItem->m_pItemBase->AddStatus( DATA_STATUS_PULLDOWN );
			else
				m_pSelectItem->m_pItemBase->DelStatus( DATA_STATUS_PULLDOWN );

		}

		m_pSelectItem				=	pItem;
		m_pSelectItem->m_bSelect	=	!m_pSelectItem->m_bSelect;
		m_pSelectItem->m_pItemBase->SetStatus( DATA_STATUS_SELECT );

		if( m_pSelectItem->m_bSelect == true && (int)m_pSelectItem->m_vpChild.size() > 0 )
			m_pSelectItem->m_pItemBase->AddStatus( DATA_STATUS_PULLDOWN );
		else
			m_pSelectItem->m_pItemBase->DelStatus( DATA_STATUS_PULLDOWN );

		if( (int)m_pSelectItem->m_vpChild.size() != 0)
			Refresh();

		if( m_pSelectItem != NULL )
		{
			if( m_pSelectItem->m_pItemBase != NULL )
			{
				if( m_pSelectItem->m_pItemBase->GetStatus( DATA_STATUS_DISABLE ) == false )
				{
					SPSendMessageToParent( SPIM_LBUTTON_UP , (WPARAM)m_pSelectItem , 0 , this );
				}
			}
		}

	}

	m_bLButtonDown	=	false;

	return 1;
}

int SPWindowTreeBox::OnDblLButtonUp( WPARAM wParam, LPARAM lParam )
{
	if( m_bDoubleClick == false )
		return 1;

	TREEBOX_ITEM*	pItem	=	m_pMouseOver;
//	TREEBOX_ITEM*	pItem	=	FindRenderItem( lParam );

	if( m_pMouseOver != NULL )
	{
		if( m_pMouseOver->m_pItemBase != NULL )
			m_pMouseOver->m_pItemBase->DelStatus( DATA_STATUS_PUSH );
	}

	if( pItem != NULL )
	{
		if( m_pSelectItem != NULL )
		{
			m_pSelectItem->m_pItemBase->SetStatus( DATA_STATUS_ENABLE );

			if( m_pSelectItem->m_bSelect == true && (int)m_pSelectItem->m_vpChild.size() > 0 )
				m_pSelectItem->m_pItemBase->AddStatus( DATA_STATUS_PULLDOWN );
			else
				m_pSelectItem->m_pItemBase->DelStatus( DATA_STATUS_PULLDOWN );

		}

		m_pSelectItem				=	pItem;
		m_pSelectItem->m_bSelect	=	!m_pSelectItem->m_bSelect;
		m_pSelectItem->m_pItemBase->SetStatus( DATA_STATUS_SELECT );

		if( m_pSelectItem->m_bSelect == true && (int)m_pSelectItem->m_vpChild.size() > 0 )
			m_pSelectItem->m_pItemBase->AddStatus( DATA_STATUS_PULLDOWN );
		else
			m_pSelectItem->m_pItemBase->DelStatus( DATA_STATUS_PULLDOWN );

		if( (int)m_pSelectItem->m_vpChild.size() != 0)
			Refresh();

		if( m_pSelectItem != NULL )
		{
			if( m_pSelectItem->m_pItemBase != NULL )
			{
				if( m_pSelectItem->m_pItemBase->GetStatus( DATA_STATUS_DISABLE ) == false )
				{
					SPSendMessageToParent( SPIM_LBUTTON_DBLCLICK , (WPARAM)m_pSelectItem , 0 , this );
				}
			}
		}

	}

	return 1;
}

int SPWindowTreeBox::OnLButtonDown( WPARAM wParam, LPARAM lParam )
{
	if( m_pSelectItem != m_pMouseOver )
	{
		if( m_pMouseOver != NULL )
			m_pMouseOver->m_pItemBase->DelStatus( DATA_STATUS_PUSH );
	}

	m_pMouseOver	=	FindRenderItem( lParam );

	if( m_pMouseOver != NULL )
	{
		if( m_pMouseOver->m_pItemBase != NULL )
		{
			if( m_pMouseOver->m_pItemBase->GetStatus( DATA_STATUS_DISABLE ) == true )
			{
				m_pMouseOver	=	NULL;
				return 1;
			}
		}
	}

	if( m_pSelectItem != m_pMouseOver )
	{
		if( m_pMouseOver != NULL )
			m_pMouseOver->m_pItemBase->AddStatus( DATA_STATUS_PUSH );
	}

	m_bLButtonDown	=	true;
	return 1;
}

int SPWindowTreeBox::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	if( m_pSelectItem != m_pMouseOver )
	{
		if( m_pMouseOver != NULL )
		{
			m_pMouseOver->m_pItemBase->DelStatus( DATA_STATUS_HIT );

			if( m_bLButtonDown == true )
				m_pMouseOver->m_pItemBase->DelStatus( DATA_STATUS_PUSH );
		}
	}

	m_pMouseOver	=	FindRenderItem( lParam );

	if( m_pSelectItem != m_pMouseOver )
	{
		if( m_pMouseOver != NULL )
		{
			m_pMouseOver->m_pItemBase->AddStatus( DATA_STATUS_HIT );

			if( m_bLButtonDown == true )
				m_pMouseOver->m_pItemBase->AddStatus( DATA_STATUS_PUSH );
		}
	}

	return 1;
}

int SPWindowTreeBox::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	if( m_pMouseOver != NULL )
		m_pMouseOver->m_pItemBase->DelStatus( DATA_STATUS_HIT );

	m_bLButtonDown	=	false;

	return 1;
}

int SPWindowTreeBox::OnWheelUp( WPARAM wParam, LPARAM lParam )
{
	m_pScrollSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 0;
}

int SPWindowTreeBox::OnWheelDown( WPARAM wParam, LPARAM lParam )
{
	m_pScrollSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 0;
}

int SPWindowTreeBox::OnSliderPage( WPARAM wParam, LPARAM lParam )
{
	if( m_pStartItem == NULL )
		return 0;

	int	iScrollSelect	=	(int)wParam;

	if( m_iScrollCurPos == iScrollSelect )
		return 1;

	m_iScrollCurPos	=	iScrollSelect;

	int	iViewCount		=	(int)m_vpViewItem.size();

	if( iScrollSelect >= iViewCount )
		iScrollSelect	=	iViewCount	-	1;

	if( iViewCount <= 0 )
		m_pStartItem	=	m_pRootItem;
	else
        m_pStartItem	=	m_vpViewItem.at( iScrollSelect );

	Refresh();

	return 1;
}

int SPWindowTreeBox::OnSliderCursorEnter( WPARAM wParam, LPARAM lParam )
{
	if( m_pMouseOver != NULL )
		m_pMouseOver->m_pItemBase->DelStatus( DATA_STATUS_HIT );

	m_bLButtonDown	=	false;

	return 0;
}

int SPWindowTreeBox::ProcessItem( int iCurPos , int iPosX , int iPosY , float fTime )
{
	STD_VECTOR_TREEBOX_ITEM::iterator	IterItem	=	m_vpViewItem.begin() + iCurPos;
	SPTreeBoxItemBase*					pItemBase	=	NULL;
	bool								bRectOut	=	false;

	while ( IterItem != m_vpViewItem.end() )
	{
		if( (*IterItem)->m_bShow == true )
		{
			pItemBase	=	(*IterItem)->m_pItemBase;

			if( pItemBase != NULL )
			{
				pItemBase->SetPos( iPosX , iPosY );
				pItemBase->Process( fTime );

				RECT	rcPos	=	pItemBase->GetPosRect();

				if( rcPos.left	>=	m_iAX + m_rcMargin.left	&&	rcPos.right		<=	m_iAX + m_iSX - m_rcMargin.right &&
					rcPos.top	>=	m_iAY + m_rcMargin.top	&&	rcPos.bottom	<=	m_iAY + m_iSY - m_rcMargin.bottom )
				{
					m_vpRenderItem.push_back( (*IterItem) );
				}
				else
				{
					bRectOut	=	true;
					break;
				}

				int	iNextPosX	=	pItemBase->GetNextPosX();
				int	iNextPosY	=	pItemBase->GetNextPosY();

				if( iNextPosX == iPosX )
					iNextPosY	+=	m_iInterval;
				else
					iNextPosX	+=	m_iInterval;

				iPosX	=	iNextPosX;
				iPosY	=	iNextPosY;
			}
		}

		++IterItem;
	}

	//RECT	rtTemp;

	//rtTemp.left	=	m_iAX + m_rcMargin.left;
	//rtTemp.top	=	m_iAY + m_rcMargin.top;
	//rtTemp.right	=	m_iAX + m_iSX - m_rcMargin.right;
	//rtTemp.bottom	=	m_iAY + m_iSY - m_rcMargin.bottom;

	//g_pVideo->GetSysLine()->SetColor( D3DXCOLOR( 1.0f , 0.0f , 1.0f , 1.0f ) );
	//g_pVideo->GetSysLine()->DrawRect( rtTemp );
	//g_pVideo->GetSysLine()->SetColor();

	int iResult	=	-1;

	if( bRectOut == false )
	{
		iCurPos--;

		if( iCurPos < 0 || iCurPos >= (int)m_vpViewItem.size() )
			return iResult;

		int				iCurDistance	=	CalRenderItemDistance();
		TREEBOX_ITEM*	pItem			=	NULL;

		for( int i = iCurPos ; i >= 0 ; --i )
		{
			pItem	=	m_vpViewItem.at( i );

			if( pItem == NULL )
				continue;

			pItemBase	=	pItem->m_pItemBase;

			if( pItemBase == NULL )
				continue;

			iCurDistance	-=	m_iInterval;
			iCurDistance	-=	pItemBase->GetSize().y;

			if( iCurDistance >= 0 )
			{
				iResult			=	i;
				m_pStartItem	=	pItem;
			}
			else
				break;
		}
	}

	return iResult;
}

TREEBOX_ITEM* SPWindowTreeBox::FindRenderItem( LPARAM lParam )
{
	int	iXPos	=	LOWORD(lParam);
	int	iYPos	=	HIWORD(lParam);

	STD_VECTOR_TREEBOX_ITEM::iterator	IterRenderItem	=	m_vpRenderItem.begin();

	while ( IterRenderItem != m_vpRenderItem.end() )
	{
		if( (*IterRenderItem)->m_pItemBase->IsCursorIn( iXPos ,iYPos ) == true )
			return (*IterRenderItem);

		++IterRenderItem;
	}

	return NULL;
}

void SPWindowTreeBox::FindViewItem( int& iOutCurPos , TREEBOX_ITEM* pStartItem , TREEBOX_ITEM* pCurItem )
{
	if( pStartItem == NULL || pCurItem == NULL )
		return;

	if( pStartItem != m_pRootItem )
	{
		if( pStartItem->m_bShow == true )
			m_vpViewItem.push_back( pStartItem );
	}

	if( pStartItem == pCurItem )
	{
		iOutCurPos	=	(int)m_vpViewItem.size() - 1;
		if( iOutCurPos < 0 )
			iOutCurPos = 0;
	}

	if( pStartItem->m_bSelect == false )
		return;

	STD_VECTOR_TREEBOX_ITEM::iterator	IterItem	=	pStartItem->m_vpChild.begin();

	while ( IterItem != pStartItem->m_vpChild.end() )
	{
		FindViewItem( iOutCurPos , (*IterItem) , pCurItem );
		++IterItem;
	}
}

int SPWindowTreeBox::CalRenderItemDistance()
{
	int	iDistance			=	0;
	int	iRenderItemCount	=	(int)m_vpRenderItem.size();

	if( iRenderItemCount <= 0 )
		return iDistance;

	TREEBOX_ITEM*	pItem	=	m_vpRenderItem.at( iRenderItemCount - 1 );

	if( pItem != NULL )
	{
		SPTreeBoxItemBase*	pItemBase	=	pItem->m_pItemBase;

		if( pItemBase != NULL )
			iDistance	=	m_iAY + m_iSY - m_rcMargin.top - m_rcMargin.bottom - pItemBase->GetNextPosY();
	}

	return iDistance;
}

void SPWindowTreeBox::AdjustRenderItemDistance( int iCurPos )
{
	STD_VECTOR_TREEBOX_ITEM::reverse_iterator	rIterItem	=	m_vpViewItem.rbegin() + (int)m_vpViewItem.size() - iCurPos;
	TREEBOX_ITEM*								pItem;
	int											iCurDistance	=	CalRenderItemDistance();

	while ( rIterItem != m_vpViewItem.rend() )
	{
		pItem	=	(*rIterItem);

		if( pItem->m_pItemBase != NULL )
		{
			iCurDistance	-=	pItem->m_pItemBase->GetSize().y;
			iCurDistance	-=	m_iInterval;
		}

		++rIterItem;

		if( iCurDistance < 0 )
		{
			--rIterItem;
			m_pStartItem	=	(*rIterItem);
			break;
		}
	}
}

void SPWindowTreeBox::ResetItem( TREEBOX_ITEM* pStartItem )
{
	if( pStartItem == NULL )
		return;

	pStartItem->m_bSelect	=	false;

	STD_VECTOR_TREEBOX_ITEM::iterator	IterItem	=	pStartItem->m_vpChild.begin();

	while ( IterItem != pStartItem->m_vpChild.end() )
	{
		ResetItem( (*IterItem) );
		++IterItem;
	}
}

bool SPWindowTreeBox::CreateRootItem()
{
	if( m_pRootItem == NULL )
	{
		m_pRootItem	=	new	TREEBOX_ITEM;
		m_pRootItem->m_bSelect		=	true;
		m_pRootItem->m_bShow		=	true;
		m_pRootItem->m_pItemBase	=	NULL;
		m_pRootItem->m_pParent		=	NULL;
		m_pRootItem->m_vpChild.clear();

		m_pStartItem				=	m_pRootItem;

		return true;
	}

	return false;
}
