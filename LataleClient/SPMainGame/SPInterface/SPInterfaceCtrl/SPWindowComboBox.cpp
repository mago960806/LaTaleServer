// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-11-2    14:41 
//***************************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPGameObjectDEF.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowSlider.h"
#include "SPWindowStatic.h"
#include "SPWindowGrid.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPComboBoxDEF.h"
#include "SPComboBoxItemBase.h"
#include "SPComboBoxItemTexture.h"

#include "SPWindowComboBox.h"

//////////////////////////////////////////////////////////////////////////

SPWindowComboBox::SPWindowComboBox( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent )
: SPWindow( WNDID_CTRL_COMBOBOX , InstanceID , iX , iY , iCX , iCY , pParent )
{
	Init();
}

SPWindowComboBox::~SPWindowComboBox()
{
	Clean();
}

void SPWindowComboBox::Init()
{
	m_strToolTip = "[SPWindowComboBox]";

	m_iItemDataRenderStart		=	0;
	m_iItemDataRenderCountMax	=	0;

	m_iItemDataInterval			=	0;
	m_rcItemDataMargin.left		=	0;
	m_rcItemDataMargin.top		=	0;
	m_rcItemDataMargin.right	=	0;
	m_rcItemDataMargin.bottom	=	0;

	m_ptItemDataLimit.x			=	0;
	m_ptItemDataLimit.y			=	0;

	m_vListBoxBGTexture.reserve( COMBOBOX_TEXTURE_COUNT );

	CBB_TEXTURE_INFO	stTexture;

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

	for( int i = 0 ; i < COMBOBOX_TEXTURE_COUNT ; ++i )
	{
		m_vListBoxBGTexture.push_back( stTexture );
	}

	for( i = 0 ; i < COMBOBOX_DATA_TEXTURE_COUNT ; ++i )
	{
		m_vEditBoxBGTexture[ i ].reserve( COMBOBOX_TEXTURE_COUNT );

		for( int j = 0 ; j < COMBOBOX_TEXTURE_COUNT ; ++j )
		{
			m_vEditBoxBGTexture[ i ].push_back( stTexture );
		}
	}

	m_eEditBoxStatus	=	COMBOBOX_DATA_TEXTURE_ENABLE;

	m_bListBoxRender	=	FALSE;
	m_ptListBoxOffset.x	=	0;
	m_ptListBoxOffset.y	=	0;

	m_pDropDown		=	new SPWindowButton( GetInstanceID() + DROPDOWN_BUTTON		, 0 , 0 , 0 , 0 , this );
	m_pEditText		=	new	SPWindowStatic( GetInstanceID() + EDITBOX_EDIT			, 0 , 0 , 0 , 0 , this );
	m_pScrollUp		=	new SPWindowButton( GetInstanceID() + LISTBOX_BUTTON_UP		, 0 , 0 , 0 , 0 , this );
	m_pScrollDown	=	new SPWindowButton( GetInstanceID() + LISTBOX_BUTTON_DOWN	, 0 , 0 , 0 , 0 , this );
	m_pScrollSlider	=	new SPWindowSlider( GetInstanceID() + LISTBOX_BUTTON_SLIDER	, 0 , 0 , 0 , 0 , this );
	m_pScrollSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pScrollSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );
	m_pScrollSlider->SPSendMessage( SPIM_SET_CURVALUE , 0 );

	m_bScrollUser			=	FALSE;
	m_bScrollData			=	FALSE;
	m_bScrollDir			=	FALSE;
	m_rcScrollMargin.left	=	0;
	m_rcScrollMargin.top	=	0;
	m_rcScrollMargin.right	=	0;
	m_rcScrollMargin.bottom	=	0;
	m_iScrollSelect			=	0;

	m_bShowEditBoxText		=	FALSE;
	m_pSelectItemData		=	NULL;

	m_ptEditBoxOffset.x		=	0;
	m_ptEditBoxOffset.y		=	0;

	m_ptEditBoxSize.x		=	0;
	m_ptEditBoxSize.y		=	0;

	m_bSelectValueApply		=	TRUE;
	m_bToolTipShow			=	FALSE;
}


void SPWindowComboBox::InitFromXML(SPWindow_Base* rec_wnd_AttrMap)
{
	//SPWindow::InitFromXML(rec_wnd_AttrMap);

	//-
	SetDropDownOffset		( 131 - 11 , 6 );
	SetDropDownSize			( 9 , 6 );
	SetScroll				( TRUE );
	SetScrollOffset			( 0 , 5 , 5 , 5 );
	SetScrollUpSize			( 12 , 11 );
	SetScrollDownSize		( 12 , 11 );
	SetScrollSliderSize		( 12 , 24 );
	SetListBoxItemMargin	( 1 , 1 , 1 , 1 );
	SetShowEditBoxText		( FALSE );
	SetEditBoxOffset		( 0 , 3 );
	SetEditBoxSize			(UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_REALSIZE_X),UI_INT(rec_wnd_AttrMap,GET_SPWINDOW_REALSIZE_Y));
	SetListBoxOffset		( 0, 17 );
	SetRenderItemCountMax	( 5 );
	SetSelectValueApply		( FALSE );
	SetToolTipShow			( TRUE );
	SetEditBoxItem			( 0 );
	SetDropDownStatus		( COMBOBOX_DATA_TEXTURE_ENABLE );	

	CHAR* sz_FileName	= UI_STRING(rec_wnd_AttrMap,GET_SPWINDOW_IMAGE_FILENAME);
	INT	  Size			= static_cast<INT>(strlen(sz_FileName));

	for(int  i = 0;i < Size;++i)
	{
		if(sz_FileName[i] == '\\') sz_FileName[i] = '/';
	}

	
	//- 
	s_Point4*	pt_Pos		= 0;
	
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_DROPDOWN_ENABLE);	SetDropDownTexture		(COMBOBOX_DATA_TEXTURE_ENABLE	,sz_FileName ,pt_Pos->x,pt_Pos->y);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_DROPDOWN_HIT);		SetDropDownTexture		(COMBOBOX_DATA_TEXTURE_HIT		,sz_FileName ,pt_Pos->x,pt_Pos->y);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_DROPDOWN_PUSH);		SetDropDownTexture		(COMBOBOX_DATA_TEXTURE_PUSH		,sz_FileName ,pt_Pos->x,pt_Pos->y);

	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SCROLLUP_ENABLE);	SetScrollUpTexture		(COMBOBOX_DATA_TEXTURE_ENABLE	,sz_FileName ,pt_Pos->x,pt_Pos->y);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SCROLLUP_HIT);		SetScrollUpTexture		(COMBOBOX_DATA_TEXTURE_HIT		,sz_FileName ,pt_Pos->x,pt_Pos->y);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SCROLLUP_PUSH);		SetScrollUpTexture		(COMBOBOX_DATA_TEXTURE_PUSH		,sz_FileName ,pt_Pos->x,pt_Pos->y);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SCROLLUP_DISABLE);	SetScrollUpTexture		(COMBOBOX_DATA_TEXTURE_DISABLE	,sz_FileName ,pt_Pos->x,pt_Pos->y);

	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SCROLLDOWN_ENABLE);	SetScrollDownTexture	(COMBOBOX_DATA_TEXTURE_ENABLE	,sz_FileName ,pt_Pos->x,pt_Pos->y);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SCROLLDOWN_HIT);	SetScrollDownTexture	(COMBOBOX_DATA_TEXTURE_HIT		,sz_FileName ,pt_Pos->x,pt_Pos->y);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SCROLLDOWN_PUSH);	SetScrollDownTexture	(COMBOBOX_DATA_TEXTURE_PUSH		,sz_FileName ,pt_Pos->x,pt_Pos->y);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SCROLLDOWN_DISABLE);SetScrollDownTexture	(COMBOBOX_DATA_TEXTURE_DISABLE	,sz_FileName ,pt_Pos->x,pt_Pos->y);

	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SLIDER_ENABLE);		SetScrollSliderTexture	(COMBOBOX_DATA_TEXTURE_ENABLE	,sz_FileName ,pt_Pos->x,pt_Pos->y);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SLIDER_HIT);		SetScrollSliderTexture	(COMBOBOX_DATA_TEXTURE_HIT		,sz_FileName ,pt_Pos->x,pt_Pos->y);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SLIDER_PUSH);		SetScrollSliderTexture	(COMBOBOX_DATA_TEXTURE_PUSH		,sz_FileName ,pt_Pos->x,pt_Pos->y);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SLIDER_DISABLE);	SetScrollSliderTexture	(COMBOBOX_DATA_TEXTURE_DISABLE	,sz_FileName ,pt_Pos->x,pt_Pos->y);

	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SLIDERBG_ENABLE);	SetScrollSliderBGTexture(COMBOBOX_DATA_TEXTURE_ENABLE	,sz_FileName ,pt_Pos->x,pt_Pos->y,pt_Pos->x2,pt_Pos->y2);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SLIDERBG_HIT);		SetScrollSliderBGTexture(COMBOBOX_DATA_TEXTURE_HIT		,sz_FileName ,pt_Pos->x,pt_Pos->y,pt_Pos->x2,pt_Pos->y2);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SLIDERBG_PUSH);		SetScrollSliderBGTexture(COMBOBOX_DATA_TEXTURE_PUSH		,sz_FileName ,pt_Pos->x,pt_Pos->y,pt_Pos->x2,pt_Pos->y2);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_SLIDERBG_DISABLE);	SetScrollSliderBGTexture(COMBOBOX_DATA_TEXTURE_DISABLE	,sz_FileName ,pt_Pos->x,pt_Pos->y,pt_Pos->x2,pt_Pos->y2);

	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_EDITBOX_ENABLE);	SetEditBoxTexture		(COMBOBOX_DATA_TEXTURE_ENABLE	,sz_FileName ,pt_Pos->x,pt_Pos->y,pt_Pos->x2,pt_Pos->y2);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_EDITBOX_HIT);		SetEditBoxTexture		(COMBOBOX_DATA_TEXTURE_HIT		,sz_FileName ,pt_Pos->x,pt_Pos->y,pt_Pos->x2,pt_Pos->y2);
	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_EDITBOX_PUSH);		SetEditBoxTexture		(COMBOBOX_DATA_TEXTURE_PUSH		,sz_FileName ,pt_Pos->x,pt_Pos->y,pt_Pos->x2,pt_Pos->y2);

	pt_Pos = (s_Point4*)rec_wnd_AttrMap->Get(GET_SPWINDOW_CB_TEXPOS_LISTBOX_ENABLE);	SetListBoxTexture		(sz_FileName ,pt_Pos->x,pt_Pos->y,pt_Pos->x2,pt_Pos->y2);

	Show(true);
}


void SPWindowComboBox::Clean()
{
	m_vItemRender.clear();

	DelAllItem();

	STD_VECTOR_CBB_TEXTURE::iterator	IterTexture = m_vListBoxBGTexture.begin();

	while( IterTexture != m_vListBoxBGTexture.end() )
	{
		SAFE_RELEASE( (*IterTexture).m_pTexture );
		++IterTexture;
	}

	m_vListBoxBGTexture.clear();

	//////////////////////////////////////////////////////////////////////////

	for( int i = 0 ; i < COMBOBOX_DATA_TEXTURE_COUNT ; ++i )
	{
		IterTexture = m_vEditBoxBGTexture[ i ].begin();

		while( IterTexture != m_vEditBoxBGTexture[ i ].end() )
		{
			SAFE_RELEASE( (*IterTexture).m_pTexture );
			++IterTexture;
		}

		m_vEditBoxBGTexture[ i ].clear();
	}
	
	SPWindow::Clean();
}

void SPWindowComboBox::Process( float fTime )
{
	if( !m_bShow )
		return;

	SPWindow::Process( fTime );

	int	iRenderCount	=	(int)m_vItemRender.size();
	int	iItemDataCount	=	(int)m_vItemData.size();
	int	iItemDataNumber	=	0;

	CBB_RENDER_INFO*	pRenderInfo	=	NULL;
	SPComboBoxItemBase*	pItemData	=	NULL;

	for( int i = 0 ; i < iRenderCount ; i++ )
	{
		pRenderInfo	=	&m_vItemRender.at( i );

		iItemDataNumber	=	m_iItemDataRenderStart + i;

		if( iItemDataNumber >= iItemDataCount )
			iItemDataNumber	=	iItemDataCount - 1;

		if( iItemDataCount <= 0 )
			continue;

		pItemData	=	m_vItemData.at( iItemDataNumber );

		if( pRenderInfo == NULL || pItemData == NULL )
			continue;

		pItemData->SetRenderInfo( pRenderInfo );
		pItemData->Process( fTime );
	}
}

void SPWindowComboBox::Render( float fTime )
{
	if( !m_bShow )
		return;

	g_pVideo->Flush();

	CBB_TEXTURE_INFO*	pCbbTexture	=	NULL;
	int					i			=	0;
	int					iEditBoxStatus	=	(int)m_eEditBoxStatus;

	if( iEditBoxStatus < COMBOBOX_DATA_TEXTURE_COUNT && iEditBoxStatus >= COMBOBOX_DATA_TEXTURE_ENABLE )
	{
		for( i = 0 ; i < COMBOBOX_TEXTURE_COUNT ; ++i )
		{
			pCbbTexture	=	&m_vEditBoxBGTexture[ iEditBoxStatus ].at( i );

			if( pCbbTexture->m_pTexture == NULL )
				continue;

			pCbbTexture->m_pTexture->SetColor();
			pCbbTexture->m_pTexture->RenderTexture( &pCbbTexture->m_rcPos , &pCbbTexture->m_rcSrc );
			g_pVideo->Flush();
		}
	}

	int	iRenderCount	=	(int)m_vItemRender.size();

	if( m_bListBoxRender == TRUE && iRenderCount > 0 )
	{
		for( i = 0 ; i < COMBOBOX_TEXTURE_COUNT ; ++i )
		{
			pCbbTexture	=	&m_vListBoxBGTexture.at( i );

			if( pCbbTexture->m_pTexture == NULL )
				continue;

			pCbbTexture->m_pTexture->SetColor();
			pCbbTexture->m_pTexture->RenderTexture( &pCbbTexture->m_rcPos , &pCbbTexture->m_rcSrc );
			g_pVideo->Flush();
		}
	}

	SPWindow::Render( fTime );
	g_pVideo->Flush();

	if( m_pSelectItemData != NULL )
	{
		COMBOBOX_DATA_TEXTURE_LABEL	eStatus		=	m_pSelectItemData->GetStatus();
		CBB_RENDER_INFO*			pRenderInfo	=	m_pSelectItemData->GetRenderInfo();

		m_pSelectItemData->SetStatus( COMBOBOX_DATA_TEXTURE_ENABLE );

		m_pSelectItemData->SetRenderInfo( &m_stSelectItemRender );
		m_pSelectItemData->Process( fTime );
		m_pSelectItemData->Render( fTime );

		m_pSelectItemData->SetStatus( eStatus );
		m_pSelectItemData->SetRenderInfo( pRenderInfo );
		m_pSelectItemData->Process( fTime );
	}

	if( m_bListBoxRender == FALSE )
		return;

	SPComboBoxItemBase*	pItemData	=	NULL;

	for( i = 0 ; i < iRenderCount ; ++i )
	{
		pItemData	=	m_vItemData.at( m_iItemDataRenderStart + i );

		if( pItemData == NULL )
			continue;

		pItemData->Render( fTime );
		g_pVideo->Flush();
	}
}

void SPWindowComboBox::Show( bool bWithChild /* = true  */ )
{
	SPWindow::Show( bWithChild );

	if( m_bScrollUser == FALSE || m_bScrollData == FALSE || m_bListBoxRender == FALSE )
	{
		m_pScrollUp->Hide();
		m_pScrollDown->Hide();
		m_pScrollSlider->Hide();
	}
}

void SPWindowComboBox::Hide( bool bSendServer /* = true  */ )
{
	SPWindow::Hide( bSendServer );

	m_eEditBoxStatus	=	COMBOBOX_DATA_TEXTURE_ENABLE;
	m_bListBoxRender	=	FALSE;

	UpdateComboBoxSize();
}

bool SPWindowComboBox::IsCursorIn( int iX, int iY )
{
	bool	bResult	=	false;

	if( m_bScrollDir == FALSE )
	{
		bResult	=	SPWindow::IsCursorIn( iX , iY );
	}
	else
	{
		if( iX >= m_iAX && iY <= m_iAY && 
			iX <= (m_iAX + m_iSX) && iY >= (m_iAY - m_iSY)	)
			bResult	=	true;
		else
			bResult	=	false;
	}

	//[xialin 2008/08/11]add >>>
	int	iRenderCount	=	(int)m_vItemRender.size();
	if( m_bListBoxRender == TRUE && iRenderCount > 0 )
	{
		for( int i = 0 ; i < COMBOBOX_TEXTURE_COUNT ; ++i )
		{
			CBB_TEXTURE_INFO * pCbbTexture	=	&m_vListBoxBGTexture.at( i );

			if( pCbbTexture->m_pTexture )
			{
				if( iX >= pCbbTexture->m_rcPos.left && iY >= pCbbTexture->m_rcPos.top && 
					iX <= pCbbTexture->m_rcPos.right && iY <= pCbbTexture->m_rcPos.bottom )
				{
					bResult	= true;
					break;
				}
			}
		}
	}
	//[xialin 2008/08/11]add <<<

	return bResult;
}

void SPWindowComboBox::NoRender()
{
	m_bShow = false;
}

void SPWindowComboBox::PosUpdate()
{
	ProcessRenderInfo();
}

void SPWindowComboBox::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ProcessRenderInfo();
}

void SPWindowComboBox::SetRenderItemCountMax( int iCount )
{
	m_iItemDataRenderCountMax	=	iCount;
}

void SPWindowComboBox::SetListBoxTexture( COMBOBOX_TEXTURE eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY )
{
	int	iTextureType	=	(int)eTextureType;

	if( iTextureType >= COMBOBOX_TEXTURE_COUNT )
		return;

	SPTexture*		pTexture	=	NULL;

	g_pVideo->GetTextureMgr()->LoadTexture( strFileName , &pTexture );

	CBB_TEXTURE_INFO&	stTexture	=	m_vListBoxBGTexture.at( iTextureType );

	stTexture.m_rcSrc.left		=	iSrcSX;
	stTexture.m_rcSrc.top		=	iSrcSY;
	stTexture.m_rcSrc.right		=	iSrcEX;
	stTexture.m_rcSrc.bottom	=	iSrcEY;
	stTexture.m_ptSize.x		=	iSrcEX	-	iSrcSX;
	stTexture.m_ptSize.y		=	iSrcEY	-	iSrcSY;

	SAFE_RELEASE( stTexture.m_pTexture );

	stTexture.m_pTexture	=	pTexture;
}

void SPWindowComboBox::SetListBoxTexture( const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY , int iStep /* = 5  */ )
{
	SetListBoxTexture( COMBOBOX_TEXTURE_LEFT_TOP		, strFileName , iSrcSX			, iSrcSY , iSrcSX + iStep	, iSrcSY + iStep );
	SetListBoxTexture( COMBOBOX_TEXTURE_CENTER_TOP		, strFileName , iSrcSX + iStep	, iSrcSY , iSrcEX - iStep	, iSrcSY + iStep );
	SetListBoxTexture( COMBOBOX_TEXTURE_RIGHT_TOP		, strFileName , iSrcEX - iStep	, iSrcSY , iSrcEX			, iSrcSY + iStep );

	SetListBoxTexture( COMBOBOX_TEXTURE_LEFT_MIDDLE		, strFileName , iSrcSX			, iSrcSY + iStep , iSrcSX + iStep	, iSrcSY + iStep * 2 );
	SetListBoxTexture( COMBOBOX_TEXTURE_CENTER_MIDDLE	, strFileName , iSrcSX + iStep	, iSrcSY + iStep , iSrcEX - iStep	, iSrcSY + iStep * 2);
	SetListBoxTexture( COMBOBOX_TEXTURE_RIGHT_MIDDLE	, strFileName , iSrcEX - iStep	, iSrcSY + iStep , iSrcEX			, iSrcSY + iStep * 2);

	SetListBoxTexture( COMBOBOX_TEXTURE_LEFT_BOTTOM		, strFileName , iSrcSX			, iSrcEY - iStep , iSrcSX + iStep	, iSrcEY );
	SetListBoxTexture( COMBOBOX_TEXTURE_CENTER_BOTTOM	, strFileName , iSrcSX + iStep	, iSrcEY - iStep , iSrcEX - iStep	, iSrcEY );
	SetListBoxTexture( COMBOBOX_TEXTURE_RIGHT_BOTTOM	, strFileName , iSrcEX - iStep	, iSrcEY - iStep , iSrcEX			, iSrcEY );
}

void SPWindowComboBox::SetListBoxItemInterval( int iInterval )
{
	m_iItemDataInterval	=	iInterval;
}

void SPWindowComboBox::SetListBoxItemMargin( int iLeft , int iTop , int iRight , int iBottom )
{
	m_rcItemDataMargin.left		=	iLeft;
	m_rcItemDataMargin.top		=	iTop;
	m_rcItemDataMargin.right	=	iRight;
	m_rcItemDataMargin.bottom	=	iBottom;
}

void SPWindowComboBox::SetListBoxItemLimit( int iX , int iY )
{
	m_ptItemDataLimit.x	=	iX;
	m_ptItemDataLimit.y	=	iY;
}

void SPWindowComboBox::SetListBoxOffset( int iX , int iY )
{
	m_ptListBoxOffset.x	=	iX;
	m_ptListBoxOffset.y	=	iY;
}

void SPWindowComboBox::SetDropDownStatus( COMBOBOX_DATA_TEXTURE_LABEL eType )
{
	if( m_pDropDown == NULL )
		return;

	m_pDropDown->SPSendMessage( SPIM_BTN_ENABLE, TRUE );

	switch( eType )
	{
	case COMBOBOX_DATA_TEXTURE_HIT:
		m_pDropDown->SPSendMessage( SPIM_BTN_SELECT, TRUE );
		break;
	case COMBOBOX_DATA_TEXTURE_DISABLE:
		m_pDropDown->SPSendMessage( SPIM_BTN_ENABLE, FALSE );
		break;
	}
}

void SPWindowComboBox::SetDropDownOffset( int iX , int iY )
{
	if( m_pDropDown == NULL )
		return;

	m_pDropDown->AdjustCoord( iX , iY );
}

void SPWindowComboBox::SetDropDownSize( int iX , int iY )
{
	if( m_pDropDown == NULL )
		return;

	RECT	rcDropDown;

	m_pDropDown->GetWindowRect( rcDropDown );

	int		iPosX	=	rcDropDown.left	-	m_iAX;
	int		iPosY	=	rcDropDown.top	-	m_iAY;


	m_pDropDown->AdjustCoord( iPosX , iPosY , iX , iY );
}

void SPWindowComboBox::SetDropDownTexture( COMBOBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY )
{
	if( m_pDropDown == NULL )
		return;

	switch( eTextureType)
	{
	case COMBOBOX_DATA_TEXTURE_ENABLE:
		m_pDropDown->SetImage( strFileName , iSrcSX , iSrcSY );
		break;
	case COMBOBOX_DATA_TEXTURE_HIT:
		m_pDropDown->SetImageHit( strFileName , iSrcSX , iSrcSY );
		break;
	case COMBOBOX_DATA_TEXTURE_PUSH:
		m_pDropDown->SetImagePush( strFileName , iSrcSX , iSrcSY );
		break;
	case COMBOBOX_DATA_TEXTURE_DISABLE:
		m_pDropDown->SetImageDisable( strFileName , iSrcSX , iSrcSY );
		break;
	}
}

void SPWindowComboBox::SetScroll( BOOL bScroll )
{
	m_bScrollUser	=	bScroll;
}

void SPWindowComboBox::SetScrollDir( BOOL bScrollDir )
{
	m_bScrollDir	=	bScrollDir;
}

void SPWindowComboBox::SetScrollOffset( int iLeft , int iTop , int iRight , int iBottom )
{
	m_rcScrollMargin.left	=	iLeft;
	m_rcScrollMargin.top	=	iTop;
	m_rcScrollMargin.right	=	iRight;
	m_rcScrollMargin.bottom	=	iBottom;
}

void SPWindowComboBox::SetScrollUpSize( int iX , int iY )
{
	if( m_pScrollUp == NULL )
		return;

	RECT	rcScrollUp;

	m_pScrollUp->GetWindowRect( rcScrollUp );

	int		iPosX	=	rcScrollUp.left	-	m_iAX;
	int		iPosY	=	rcScrollUp.top	-	m_iAY;


	m_pScrollUp->AdjustCoord( iPosX , iPosY , iX , iY );
}

void SPWindowComboBox::SetScrollDownSize( int iX , int iY )
{
	if( m_pScrollDown == NULL )
		return;

	RECT	rcScrollDown;

	m_pScrollDown->GetWindowRect( rcScrollDown );

	int		iPosX	=	rcScrollDown.left	-	m_iAX;
	int		iPosY	=	rcScrollDown.top	-	m_iAY;


	m_pScrollDown->AdjustCoord( iPosX , iPosY , iX , iY );
}

void SPWindowComboBox::SetScrollSliderSize( int iX , int iY )
{
	if( m_pScrollSlider == NULL )
		return;

	RECT	rcScrollSlider;

	m_pScrollSlider->GetWindowRect( rcScrollSlider );

	int		iPosX	=	rcScrollSlider.left	-	m_iAX;
	int		iPosY	=	rcScrollSlider.top	-	m_iAY;


	m_pScrollSlider->AdjustCoord( iPosX , iPosY , iX , iY );
}

void SPWindowComboBox::SetScrollUpTexture( COMBOBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY )
{
	if( m_pScrollUp == NULL )
		return;

	switch( eTextureType)
	{
	case COMBOBOX_DATA_TEXTURE_ENABLE:
		m_pScrollUp->SetImage( strFileName , iSrcSX , iSrcSY );
		break;
	case COMBOBOX_DATA_TEXTURE_HIT:
		m_pScrollUp->SetImageHit( strFileName , iSrcSX , iSrcSY );
		break;
	case COMBOBOX_DATA_TEXTURE_PUSH:
		m_pScrollUp->SetImagePush( strFileName , iSrcSX , iSrcSY );
		break;
	case COMBOBOX_DATA_TEXTURE_DISABLE:
		m_pScrollUp->SetImageDisable( strFileName , iSrcSX , iSrcSY );
		break;
	}
}

void SPWindowComboBox::SetScrollDownTexture( COMBOBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY )
{
	if( m_pScrollDown == NULL )
		return;

	switch( eTextureType)
	{
	case COMBOBOX_DATA_TEXTURE_ENABLE:
		m_pScrollDown->SetImage( strFileName , iSrcSX , iSrcSY );
		break;
	case COMBOBOX_DATA_TEXTURE_HIT:
		m_pScrollDown->SetImageHit( strFileName , iSrcSX , iSrcSY );
		break;
	case COMBOBOX_DATA_TEXTURE_PUSH:
		m_pScrollDown->SetImagePush( strFileName , iSrcSX , iSrcSY );
		break;
	case COMBOBOX_DATA_TEXTURE_DISABLE:
		m_pScrollDown->SetImageDisable( strFileName , iSrcSX , iSrcSY );
		break;
	}
}

void SPWindowComboBox::SetScrollSliderTexture( COMBOBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY )
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
	case COMBOBOX_DATA_TEXTURE_ENABLE:
		m_pScrollSlider->SetImageHandle( strFileName , rcScrollSlider );
		break;
	case COMBOBOX_DATA_TEXTURE_HIT:
		m_pScrollSlider->SetImageHandleHit( strFileName , rcScrollSlider );
		break;
	case COMBOBOX_DATA_TEXTURE_PUSH:
		m_pScrollSlider->SetImageHandlePush( strFileName , rcScrollSlider );
		break;
	case COMBOBOX_DATA_TEXTURE_DISABLE:
		m_pScrollSlider->SetImageHandleDisable( strFileName , rcScrollSlider );
		break;
	}
}

void SPWindowComboBox::SetScrollSliderBGTexture( COMBOBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY )
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
	case COMBOBOX_DATA_TEXTURE_ENABLE:
		m_pScrollSlider->SetImagePageNormal( strFileName , rcScrollSlider );
		break;
	case COMBOBOX_DATA_TEXTURE_HIT:
		m_pScrollSlider->SetImagePageHit( strFileName , rcScrollSlider );
		break;
	case COMBOBOX_DATA_TEXTURE_PUSH:
		m_pScrollSlider->SetImagePageShadow( strFileName , rcScrollSlider );
		break;
	case COMBOBOX_DATA_TEXTURE_DISABLE:
		m_pScrollSlider->SetImagePageDisable( strFileName , rcScrollSlider );
		break;
	}
}

void SPWindowComboBox::SetShowEditBoxText( BOOL bShowEditBoxText )
{
	m_bShowEditBoxText	=	bShowEditBoxText;

	if( m_bSelectValueApply == TRUE )
		AcceptSelectValueApply();
}

void SPWindowComboBox::SetEditBoxStatus( COMBOBOX_DATA_TEXTURE_LABEL eType )
{
	m_eEditBoxStatus	=	eType;
}

void SPWindowComboBox::SetEditBoxOffset( int iX , int iY )
{
	m_ptEditBoxOffset.x	=	iX;
	m_ptEditBoxOffset.y	=	iY;
}

void SPWindowComboBox::SetEditBoxSize( int iX , int iY )
{
	m_ptEditBoxSize.x	=	iX;
	m_ptEditBoxSize.y	=	iY;
}

void SPWindowComboBox::SetEditBoxItem( int iNumber )
{
	int	iItemCount	=	(int)m_vItemData.size();

	if( iNumber < 0 || iNumber > iItemCount )
		return;

	OnListBoxSliderPage( iNumber , NULL );

	SetShowEditBoxText( m_bShowEditBoxText );
}

void SPWindowComboBox::SetEditBoxTextColor( D3DXCOLOR stColor )
{
	m_pEditText->SetFontColor( stColor );
}

void SPWindowComboBox::SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_LABEL eTextureLabel , COMBOBOX_TEXTURE eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY )
{
	int	iTextureType	=	(int)eTextureType;

	if( iTextureType >= COMBOBOX_TEXTURE_COUNT )
		return;

	int iTextureLabel	=	(int)eTextureLabel;

	if( iTextureLabel >= COMBOBOX_DATA_TEXTURE_COUNT && iTextureLabel < COMBOBOX_DATA_TEXTURE_ENABLE )
		return;

	SPTexture*		pTexture	=	NULL;

	g_pVideo->GetTextureMgr()->LoadTexture( strFileName , &pTexture );

	CBB_TEXTURE_INFO&	stTexture	=	m_vEditBoxBGTexture[ iTextureLabel ].at( iTextureType );

	stTexture.m_rcSrc.left		=	iSrcSX;
	stTexture.m_rcSrc.top		=	iSrcSY;
	stTexture.m_rcSrc.right		=	iSrcEX;
	stTexture.m_rcSrc.bottom	=	iSrcEY;
	stTexture.m_ptSize.x		=	iSrcEX	-	iSrcSX;
	stTexture.m_ptSize.y		=	iSrcEY	-	iSrcSY;

	SAFE_RELEASE( stTexture.m_pTexture );

	stTexture.m_pTexture	=	pTexture;
}

void SPWindowComboBox::SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_LABEL eTextureLabel , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY , int iStep /* = 4  */ )
{
	SetEditBoxTexture( eTextureLabel , COMBOBOX_TEXTURE_LEFT_TOP		, strFileName , iSrcSX			, iSrcSY , iSrcSX + iStep	, iSrcSY + iStep );
	SetEditBoxTexture( eTextureLabel , COMBOBOX_TEXTURE_CENTER_TOP		, strFileName , iSrcSX + iStep	, iSrcSY , iSrcEX - iStep	, iSrcSY + iStep );
	SetEditBoxTexture( eTextureLabel , COMBOBOX_TEXTURE_RIGHT_TOP		, strFileName , iSrcEX - iStep	, iSrcSY , iSrcEX			, iSrcSY + iStep );

	SetEditBoxTexture( eTextureLabel , COMBOBOX_TEXTURE_LEFT_MIDDLE		, strFileName , iSrcSX			, iSrcSY + iStep , iSrcSX + iStep	, iSrcSY + iStep * 2 );
	SetEditBoxTexture( eTextureLabel , COMBOBOX_TEXTURE_CENTER_MIDDLE	, strFileName , iSrcSX + iStep	, iSrcSY + iStep , iSrcEX - iStep	, iSrcSY + iStep * 2);
	SetEditBoxTexture( eTextureLabel , COMBOBOX_TEXTURE_RIGHT_MIDDLE	, strFileName , iSrcEX - iStep	, iSrcSY + iStep , iSrcEX			, iSrcSY + iStep * 2);

	SetEditBoxTexture( eTextureLabel , COMBOBOX_TEXTURE_LEFT_BOTTOM		, strFileName , iSrcSX			, iSrcEY - iStep , iSrcSX + iStep	, iSrcEY );
	SetEditBoxTexture( eTextureLabel , COMBOBOX_TEXTURE_CENTER_BOTTOM	, strFileName , iSrcSX + iStep	, iSrcEY - iStep , iSrcEX - iStep	, iSrcEY );
	SetEditBoxTexture( eTextureLabel , COMBOBOX_TEXTURE_RIGHT_BOTTOM	, strFileName , iSrcEX - iStep	, iSrcEY - iStep , iSrcEX			, iSrcEY );
}

void SPWindowComboBox::AddItem( SPComboBoxItemBase* pstData )
{
	m_vItemData.push_back( pstData );

	ProcessRenderInfo();
}

void SPWindowComboBox::AddItemTop( SPComboBoxItemBase* pstData )
{
	m_vItemData.insert( m_vItemData.begin() , pstData );

	int	iCount	=	(int)m_vItemData.size();

	++m_iScrollSelect;

	if( m_iScrollSelect >= iCount )
		m_iScrollSelect	=	iCount - 1;

	ProcessRenderInfo();
}

SPComboBoxItemBase* SPWindowComboBox::GetItem( int iNumber )
{
	if( iNumber >= (int)m_vItemData.size() )
		return NULL;

	return m_vItemData.at( iNumber );
}

void SPWindowComboBox::DelItem( int iNumber )
{
	if( iNumber >= (int)m_vItemData.size() )
		return;

	SPComboBoxItemBase*	pItemBase	=	m_vItemData.at( iNumber );

	SAFE_DELETE( pItemBase );

	m_vItemData.erase( m_vItemData.begin() + iNumber );

	if( (int)m_vItemData.size() == 0 )
		DelAllItem();
	else
        ProcessRenderInfo();
}

void SPWindowComboBox::DelAllItem()
{
	STD_VECTOR_CBB_DATA::iterator		IterData	=	m_vItemData.begin();

	while( IterData != m_vItemData.end() )
	{
		SAFE_DELETE( *IterData );
		IterData++;
	}

	m_vItemData.clear();
	m_vItemRender.clear();

	m_bListBoxRender		=	FALSE;
	m_pSelectItemData		=	NULL;
	m_iItemDataRenderStart	=	0;

	m_pEditText->SetWindowText( NULL );

	ProcessRenderInfo();
}

int SPWindowComboBox::GetItemCount()
{
	return (int)m_vItemData.size();
}

int SPWindowComboBox::GetSelectNumber()
{
	return m_iScrollSelect;
}

void SPWindowComboBox::SetSelectNumber( int iSelectNumber )
{
	if( iSelectNumber < 0 || (int)m_vItemData.size() <= iSelectNumber )
		return;

	m_iScrollSelect	=	iSelectNumber;
}

void SPWindowComboBox::SetSelectValueApply( BOOL bSelectValueApply )
{
	m_bSelectValueApply	=	bSelectValueApply;
}

BOOL SPWindowComboBox::GetSelectValueApply()
{
	return m_bSelectValueApply;
}

void SPWindowComboBox::AcceptSelectValueApply()
{
	if( IsShow() == false )
		return;

	const char*	pstrText	=	NULL;

	int	iItemDataCount	=	(int)m_vItemData.size();

	if( iItemDataCount != 0 )
	{
		if( m_iScrollSelect >= iItemDataCount )
			m_iScrollSelect	=	iItemDataCount - 1;

		m_pSelectItemData	=	m_vItemData.at( m_iScrollSelect );
	}

	OnListBoxSliderPage( (WPARAM)m_iScrollSelect , NULL );

	if( m_pSelectItemData == NULL )
		return;

	if( m_bShowEditBoxText == TRUE )
	{
		pstrText			=	m_pSelectItemData->GetText();
		m_pSelectItemData	=	NULL;
	}

	m_pEditText->SetWindowText( pstrText );

	m_bListBoxRender	=	FALSE;

	ProcessRenderInfo();
}

void SPWindowComboBox::SetToolTipShow( BOOL bToolTipShow )
{
	m_bToolTipShow	=	bToolTipShow;
}

BOOL SPWindowComboBox::GetToolTipShow()
{
	return m_bToolTipShow;
}

void SPWindowComboBox::SetListBoxRender( BOOL bListBoxRender )
{
	m_bListBoxRender	=	bListBoxRender;

	UpdateComboBoxSize();
}

BOOL SPWindowComboBox::GetListBoxRender()
{
	return m_bListBoxRender;
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowComboBox )

	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP		,												OnListBoxLButtonUp		)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN	,												OnListBoxLButtonDown	)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE	,												OnListBoxCursorMove		)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT		,												OnListBoxCursorOut		)
	SPIMESSAGE_COMMAND(	SPIM_WHEEL_UP		,												OnListBoxWheelUp		)
	SPIMESSAGE_COMMAND(	SPIM_WHEEL_DOWN		,												OnListBoxWheelDown		)

	SPIMESSAGE_COMMAND( SPIM_TOOLTIP_ENTER	,												OnToolTipEnter			)
	SPIMESSAGE_COMMAND( SPIM_TOOLTIP_OUT	,												OnToolTipOut			)
	SPIMESSAGE_COMMAND( SPIM_TOOLTIP_MOVE	,												OnToolTipMove			)

	SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE	,	GetInstanceID() + EDITBOX_EDIT			,	OnListBoxCursorMove		)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT		,	GetInstanceID() + EDITBOX_EDIT			,	OnListBoxCursorOut		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	GetInstanceID() + EDITBOX_EDIT			,	OnDropDownButtonUp		)

	SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE	,	GetInstanceID() + DROPDOWN_BUTTON		,	OnListBoxCursorMove		)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT		,	GetInstanceID() + DROPDOWN_BUTTON		,	OnListBoxCursorOut		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	GetInstanceID() + DROPDOWN_BUTTON		,	OnDropDownButtonUp		)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	GetInstanceID() + LISTBOX_BUTTON_UP		,	OnListBoxUpButtonUp		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	GetInstanceID() + LISTBOX_BUTTON_DOWN	,	OnListBoxDownButtonUp	)

	SPIMESSAGE_CONTROL(	SPIM_SLIDER_UP		,	GetInstanceID() + LISTBOX_BUTTON_SLIDER	,	OnListBoxSliderPage		)
	SPIMESSAGE_CONTROL(	SPIM_SLIDER_DOWN	,	GetInstanceID() + LISTBOX_BUTTON_SLIDER	,	OnListBoxSliderPage		)
	SPIMESSAGE_CONTROL(	SPIM_SLIDER_PAGEUP	,	GetInstanceID() + LISTBOX_BUTTON_SLIDER	,	OnListBoxSliderPage		)
	SPIMESSAGE_CONTROL(	SPIM_SLIDER_PAGEDOWN,	GetInstanceID() + LISTBOX_BUTTON_SLIDER	,	OnListBoxSliderPage		)
	SPIMESSAGE_CONTROL(	SPIM_SET_CURVALUE	,	GetInstanceID() + LISTBOX_BUTTON_SLIDER	,	OnListBoxSliderPage		)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowComboBox::OnListBoxLButtonUp( WPARAM wParam, LPARAM lParam )
{
	if( m_pDropDown->IsEnable() == false )
		return 1;

	int	iXPos	=	LOWORD(lParam);
	int	iYPos	=	HIWORD(lParam);

	if( m_pEditText->IsCursorIn( iXPos , iYPos ) == true )
	{
		m_eEditBoxStatus	=	COMBOBOX_DATA_TEXTURE_HIT;
	}
	else
	{
		m_eEditBoxStatus	=	COMBOBOX_DATA_TEXTURE_ENABLE;
	}

	if( m_bListBoxRender == FALSE )
		return 1;

	int	iSelectNumber	=	CheckListBox( COMBOBOX_DATA_TEXTURE_ENABLE , LOWORD(lParam) , HIWORD(lParam) );

	if( iSelectNumber == -1 )
		return 1;

	OnListBoxSliderPage( m_iItemDataRenderStart	+	iSelectNumber , NULL );

	m_bListBoxRender	=	FALSE;

	Show();

	SetShowEditBoxText( m_bShowEditBoxText );

	ProcessRenderInfo();

	if( m_bToolTipShow == TRUE )
		SPSendMessageToParent( SPIM_TOOLTIP_OUT , NULL , NULL , this );

	return SPSendMessageToParent( SPIM_COMBOBOX_CHANGE_VALUE , m_iScrollSelect , 0 , this );
}

int SPWindowComboBox::OnListBoxLButtonDown( WPARAM wParam, LPARAM lParam )
{
	if( m_bListBoxRender == FALSE )
		return 1;

	CheckListBox( COMBOBOX_DATA_TEXTURE_PUSH , LOWORD(lParam) , HIWORD(lParam) );
	return 0;
}

int SPWindowComboBox::OnListBoxCursorMove( WPARAM wParam, LPARAM lParam )
{
	int	iXPos	=	LOWORD(lParam);
	int	iYPos	=	HIWORD(lParam);

	if( m_pEditText->IsCursorIn( iXPos , iYPos ) == true )
	{
		if( m_eEditBoxStatus != COMBOBOX_DATA_TEXTURE_PUSH )
			m_eEditBoxStatus	=	COMBOBOX_DATA_TEXTURE_HIT;
	}
	else
	{
		if( m_eEditBoxStatus != COMBOBOX_DATA_TEXTURE_PUSH )
			m_eEditBoxStatus	=	COMBOBOX_DATA_TEXTURE_ENABLE;
	}

	if( m_bToolTipShow == TRUE )
	{
		if( m_pSelectItemData != NULL )
		{
			if( iXPos >= m_stSelectItemRender.m_rcRenderPos.left	&& iXPos <= m_stSelectItemRender.m_rcRenderPos.right &&
				iYPos >= m_stSelectItemRender.m_rcRenderPos.top		&& iYPos <= m_stSelectItemRender.m_rcRenderPos.bottom )
			{
				return SPSendMessageToParent( SPIM_TOOLTIP_MOVE , m_iScrollSelect , 0 , this );
			}

		}
	}

	if( m_bListBoxRender == FALSE )
		return 1;

	int	iSelectNum	=	CheckListBox( COMBOBOX_DATA_TEXTURE_HIT , iXPos , iYPos );

	if( m_bToolTipShow == TRUE && iSelectNum >= 0 )
		return SPSendMessageToParent( SPIM_TOOLTIP_MOVE , m_iItemDataRenderStart + iSelectNum , 0 , this );

	return 0;
}

int SPWindowComboBox::OnListBoxCursorOut( WPARAM wParam, LPARAM lParam )
{
	int	iXPos	=	LOWORD(lParam);
	int	iYPos	=	HIWORD(lParam);

	if( m_pEditText->IsCursorIn( iXPos , iYPos ) == true )
	{
		if( m_eEditBoxStatus != COMBOBOX_DATA_TEXTURE_PUSH )
			m_eEditBoxStatus	=	COMBOBOX_DATA_TEXTURE_HIT;
	}
	else
	{
		if( m_eEditBoxStatus != COMBOBOX_DATA_TEXTURE_PUSH )
			m_eEditBoxStatus	=	COMBOBOX_DATA_TEXTURE_ENABLE;
	}

	if( m_bListBoxRender == FALSE )
		return 1;

	CheckListBox( COMBOBOX_DATA_TEXTURE_ENABLE , -1 , -1 );

	if( m_bToolTipShow == TRUE )
		return SPSendMessageToParent( SPIM_TOOLTIP_OUT , NULL , NULL , this );

	return 0;
}

int SPWindowComboBox::OnListBoxWheelUp( WPARAM wParam, LPARAM lParam )
{
	if( m_bListBoxRender == FALSE )
		return 1;

	m_pScrollSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 0;
}

int SPWindowComboBox::OnListBoxWheelDown( WPARAM wParam, LPARAM lParam )
{
	if( m_bListBoxRender == FALSE )
		return 1;

	m_pScrollSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 0;
}

int SPWindowComboBox::OnToolTipEnter( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowComboBox::OnToolTipOut( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowComboBox::OnToolTipMove( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowComboBox::OnDropDownButtonUp( WPARAM wParam, LPARAM lParam )
{
	int	iItemDataCount		=	(int)m_vItemData.size();

	if( iItemDataCount <= 0 )
		return 1;

	if( m_pDropDown->IsEnable() == false )
		return 1;

	int	iXPos	=	LOWORD(lParam);
	int	iYPos	=	HIWORD(lParam);

	if( m_bListBoxRender == TRUE )
	{
		m_bListBoxRender	=	FALSE;

		if( m_pEditText->IsCursorIn( iXPos , iYPos ) == true )
		{
			m_eEditBoxStatus	=	COMBOBOX_DATA_TEXTURE_HIT;
		}
		else
		{
			m_eEditBoxStatus	=	COMBOBOX_DATA_TEXTURE_ENABLE;
		}
	}
	else
	{
		m_eEditBoxStatus	=	COMBOBOX_DATA_TEXTURE_PUSH;
		m_bListBoxRender	=	TRUE;
		m_pScrollSlider->SPSendMessage( SPIM_SET_CURVALUE , m_iItemDataRenderStart );
	}

	Show();

	ProcessRenderInfo();

	return SPSendMessageToParent( SPIM_COMBOBOX_DROPDOWN_CLICK , (WPARAM)m_bListBoxRender , NULL , this );
}

int SPWindowComboBox::OnListBoxUpButtonUp( WPARAM wParam, LPARAM lParam )
{
	m_pScrollSlider->SPSendMessage( SPIM_SLIDER_UP );

	ProcessRenderInfo();
	return 1;
}

int SPWindowComboBox::OnListBoxDownButtonUp( WPARAM wParam, LPARAM lParam )
{
	m_pScrollSlider->SPSendMessage( SPIM_SLIDER_DOWN );

	ProcessRenderInfo();
	return 1;
}

int SPWindowComboBox::OnListBoxSliderPage( WPARAM wParam, LPARAM lParam )
{
	m_iScrollSelect	=	(int)wParam;

	int	iItemCount	=	(int)m_vItemData.size()	-	m_iItemDataRenderCountMax;

	if( iItemCount < 0 )
		iItemCount	=	0;

	if( m_iScrollSelect > iItemCount )
		m_iItemDataRenderStart	=	iItemCount;
	else
		m_iItemDataRenderStart	=	m_iScrollSelect;

	ProcessRenderInfo();

	if( m_bToolTipShow == TRUE )
	{
		OnListBoxCursorMove( NULL , MAKELONG( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() ) );
	}

	return 1;
}

void SPWindowComboBox::ProcessRenderInfo()
{
	UpdateRenderCount();
	UpdateRenderPos();
	UpdateBGPos();
	UpdateScrollPos();
	UpdateEditBoxPos();
}

void SPWindowComboBox::UpdateRenderCount()
{
	int	iItemRenderCount	=	(int)m_vItemRender.size();
	int	iItemDataCount		=	(int)m_vItemData.size();

	if( iItemDataCount <= 0 )
		return;

	int	iChangeItemDataCount	=	iItemDataCount;

	if( iChangeItemDataCount > m_iItemDataRenderCountMax )
	{
		iChangeItemDataCount	=	m_iItemDataRenderCountMax;
		m_bScrollData	=	TRUE;
	}
	else
	{
		m_bScrollData	=	FALSE;
	}

	int	iSliderCount	=	iItemDataCount	-	m_iItemDataRenderCountMax	+	1;

	if( iSliderCount < 0 )
		iSliderCount	=	0;

	m_pScrollSlider->SPSendMessage( SPIM_SET_MAXVALUE , iSliderCount );

	Show();

	iChangeItemDataCount	-=	iItemRenderCount;

	STD_VECTOR_CBB_RENDER::reverse_iterator		rIter	=	m_vItemRender.rbegin();
	CBB_RENDER_INFO		rtRender;

	rtRender.m_eStatus				=	COMBOBOX_DATA_TEXTURE_ENABLE;
	rtRender.m_rcRenderPos.left		=	0;
	rtRender.m_rcRenderPos.top		=	0;
	rtRender.m_rcRenderPos.right	=	0;
	rtRender.m_rcRenderPos.bottom	=	0;

	while( iChangeItemDataCount != 0 )
	{
		if( iChangeItemDataCount < 0 )
		{
			if( rIter == m_vItemRender.rend() )
				break;

			m_vItemRender.pop_back();

			iChangeItemDataCount++;
			rIter	=	m_vItemRender.rbegin();
		}
		else if( iChangeItemDataCount > 0 )
		{
			m_vItemRender.push_back( rtRender );
			iChangeItemDataCount--;
		}
	}
}

void SPWindowComboBox::UpdateRenderPos()
{
	m_stSelectItemRender.m_eStatus				=	COMBOBOX_DATA_TEXTURE_ENABLE;
	m_stSelectItemRender.m_rcRenderPos.left		=	m_iAX + m_ptEditBoxOffset.x;
	m_stSelectItemRender.m_rcRenderPos.top		=	m_iAY + m_ptEditBoxOffset.y;
	m_stSelectItemRender.m_rcRenderPos.right	=	m_stSelectItemRender.m_rcRenderPos.left + m_ptEditBoxSize.x;
	m_stSelectItemRender.m_rcRenderPos.bottom	=	m_stSelectItemRender.m_rcRenderPos.top + m_ptEditBoxSize.y;

	m_pEditText->AdjustCoord( m_ptEditBoxOffset.x , m_ptEditBoxOffset.y , m_ptEditBoxSize.x , m_ptEditBoxSize.y );

	if( m_pSelectItemData != NULL )
	{
		m_stSelectItemRender.m_rcRenderPos.right	=	m_stSelectItemRender.m_rcRenderPos.left + m_pSelectItemData->GetSize().x;
		m_stSelectItemRender.m_rcRenderPos.bottom	=	m_stSelectItemRender.m_rcRenderPos.top + m_pSelectItemData->GetSize().y;
	}

	CBB_RENDER_INFO*		prtRender;
	SPComboBoxItemBase*		pItemData;
	POINT					ptItemDataSize;
	POINT					ptItemDataPos;
	int						iDirOffsetY	=	0;

	if( (int)m_vItemData.size() == 0 )
		return;

	int	iItemRenderCount	=	(int)m_vItemRender.size();

	if( m_bScrollDir == TRUE )
	{
		iDirOffsetY	+=	m_ptListBoxOffset.y;
		iDirOffsetY	+=	m_rcItemDataMargin.top * 2;
		iDirOffsetY	+=	m_rcItemDataMargin.bottom * 2;

		for( int i = 0 ; i < iItemRenderCount ; i++ )
		{
			pItemData	=	m_vItemData.at( m_iItemDataRenderStart + i );

			if( pItemData == NULL )
				continue;

			iDirOffsetY	+=	pItemData->GetSize().y;
			iDirOffsetY	+=	m_iItemDataInterval;		
		}
	}

	ptItemDataPos.x			=	m_iAX + m_ptListBoxOffset.x	+	m_rcItemDataMargin.left	+	GetScrollWidth( COMBOBOX_SCROLL_LEFT );
	ptItemDataPos.y			=	m_iAY + m_ptListBoxOffset.y	+	m_rcItemDataMargin.top	-	m_iItemDataInterval	-	iDirOffsetY;

	for( int i = 0 ; i < iItemRenderCount ; i++ )
	{
		prtRender	=	&m_vItemRender.at( i );
		pItemData	=	m_vItemData.at( m_iItemDataRenderStart + i );

		if( prtRender == NULL || pItemData == NULL )
			continue;

		ptItemDataSize	=	pItemData->GetSize();

		prtRender->m_rcRenderPos.left	=	ptItemDataPos.x;
		prtRender->m_rcRenderPos.top	=	ptItemDataPos.y + m_iItemDataInterval;
		prtRender->m_rcRenderPos.right	=	prtRender->m_rcRenderPos.left	+ ptItemDataSize.x;
		prtRender->m_rcRenderPos.bottom	=	prtRender->m_rcRenderPos.top	+ ptItemDataSize.y;

		ptItemDataPos.y	=	prtRender->m_rcRenderPos.bottom;
	}

	UpdateRenderPosLimit();
}

void SPWindowComboBox::UpdateRenderPosLimit()
{
	if( m_ptItemDataLimit.x == 0 && m_ptItemDataLimit.y == 0 )
		return;

	STD_VECTOR_CBB_RENDER::iterator		IterRender	=	m_vItemRender.begin();

	int		iSizeX , iSizeY;
	int		iLimitX	=	m_ptItemDataLimit.x;
	int		iLimitY	=	m_ptItemDataLimit.y;
	BOOL	bDel	=	FALSE;

	while( IterRender != m_vItemRender.end() )
	{
		if( bDel == TRUE )
		{
			IterRender	=	m_vItemRender.erase( IterRender );
		}
		else
		{
			iSizeX	=	(*IterRender).m_rcRenderPos.right	-	(*IterRender).m_rcRenderPos.left;
			iSizeY	=	(*IterRender).m_rcRenderPos.bottom	-	(*IterRender).m_rcRenderPos.top;

			if( iSizeY >= iLimitY && iLimitY != 0 )
			{
				bDel	=	TRUE;
				(*IterRender).m_rcRenderPos.bottom	=	(*IterRender).m_rcRenderPos.top + iLimitY;
			}

			iLimitY	-=	iSizeY;

			if( iLimitY < 0 )
				iLimitY = 0;

			if( iSizeX >= iLimitX )
				(*IterRender).m_rcRenderPos.right	=	(*IterRender).m_rcRenderPos.left + iLimitX;

			IterRender++;
		}
	}
}

void SPWindowComboBox::UpdateBGPos()
{
	int	iItemRenderCount	=	(int)m_vItemRender.size();

	if( iItemRenderCount <= 0 )
		return;

	RECT	rcBegin	=	m_vItemRender.at( 0 ).m_rcRenderPos;
	RECT	rcEnd	=	m_vItemRender.at( iItemRenderCount - 1 ).m_rcRenderPos;

	int		iLeft	=	rcBegin.left	-	m_rcItemDataMargin.left		- GetScrollWidth( COMBOBOX_SCROLL_LEFT );
	int		iTop	=	rcBegin.top		-	m_rcItemDataMargin.top;
	int		iRight	=	rcBegin.right	+	m_rcItemDataMargin.right	+ GetScrollWidth( COMBOBOX_SCROLL_RIGHT );
	int		iBottom	=	rcEnd.bottom	+	m_rcItemDataMargin.bottom;

	if( iRight - iLeft < m_iSX )
	{
		iRight	=	iLeft	+	m_iSX;
	}

	CBB_TEXTURE_INFO*	pBGInfo	=	NULL;
	
	//////////////////////////////////////////////////////////////////////////
	//
	//	탑라인
	//

	pBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_LEFT_TOP );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left	+	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	pBGInfo->m_rcPos.top	+	pBGInfo->m_ptSize.y;

	int		iMiddleSX	=	pBGInfo->m_rcPos.right;
	int		iMiddleSY	=	pBGInfo->m_rcPos.bottom;

	pBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_RIGHT_TOP );

	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.left	=	pBGInfo->m_rcPos.right	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	pBGInfo->m_rcPos.top	+	pBGInfo->m_ptSize.y;

	int		iMiddleEX	=	pBGInfo->m_rcPos.left;

	pBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_CENTER_TOP );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iMiddleSY;

	//////////////////////////////////////////////////////////////////////////
	//
	//	BOTTOM라인

	pBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_LEFT_BOTTOM );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.bottom	=	iBottom;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left	+	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	pBGInfo->m_rcPos.bottom	-	pBGInfo->m_ptSize.y;

	pBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_RIGHT_BOTTOM );

	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.bottom	=	iBottom;
	pBGInfo->m_rcPos.left	=	pBGInfo->m_rcPos.right	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	pBGInfo->m_rcPos.bottom	-	pBGInfo->m_ptSize.y;

	int		iMiddleEY	=	pBGInfo->m_rcPos.top;

	pBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_CENTER_BOTTOM );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iMiddleEY;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iBottom;

	//////////////////////////////////////////////////////////////////////////
	//
	//	가운데라인

	pBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_LEFT_MIDDLE );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left + pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;

	pBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_CENTER_MIDDLE );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;

	pBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_RIGHT_MIDDLE );

	pBGInfo->m_rcPos.left	=	iRight	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;

	UpdateComboBoxSize();
}

void SPWindowComboBox::UpdateScrollPos()
{
	if( m_bScrollUser == FALSE || m_bScrollData == FALSE )
		return;

	CBB_TEXTURE_INFO*	pTopBGInfo		=	NULL;
	CBB_TEXTURE_INFO*	pBottomBGInfo	=	NULL;
	int					iSX				=	0;
	int					iSY				=	0;
	int					iEY				=	0;

	if( m_rcScrollMargin.left == 0 )
	{
		pTopBGInfo		=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_RIGHT_TOP );
		pBottomBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_RIGHT_BOTTOM );
		iSX				=	pTopBGInfo->m_rcPos.right	-	m_iAX	-	GetScrollWidth( COMBOBOX_SCROLL_RIGHT );
	}
	else
	{
		pTopBGInfo		=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_LEFT_TOP );
		pBottomBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_LEFT_BOTTOM );
		iSX				=	pTopBGInfo->m_rcPos.left	-	m_iAX	+	m_rcScrollMargin.left;
	}

	iSY	=	pTopBGInfo->m_rcPos.top			+	m_rcScrollMargin.top	-	m_iAY;
	iEY	=	pBottomBGInfo->m_rcPos.bottom	-	m_rcScrollMargin.bottom	-	m_iAY;

	m_pScrollUp->AdjustCoord( iSX , iSY );

	RECT	rcScroll;

	m_pScrollUp->GetWindowRect( rcScroll );

	iSY	+=	rcScroll.bottom	-	rcScroll.top;

	m_pScrollDown->GetWindowRect( rcScroll );

	iEY	-=	rcScroll.bottom	-	rcScroll.top;

	m_pScrollDown->AdjustCoord( iSX , iEY );

	m_pScrollSlider->GetWindowRect( rcScroll );

	m_pScrollSlider->AdjustCoord( iSX , iSY , rcScroll.right - rcScroll.left , iEY - iSY );

	m_pScrollSlider->SPSendMessage( SPIM_POS_UPDATE );
}

void SPWindowComboBox::UpdateComboBoxSize()
{
	CBB_TEXTURE_INFO*	pBGInfo	=	NULL;
	int					iSX		=	0;
	int					iSY		=	0;

	if( m_bListBoxRender == TRUE )
	{
		if( m_bScrollDir == FALSE )
		{
			pBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_RIGHT_BOTTOM );

			if( pBGInfo == NULL )
				return;

			iSX	=	pBGInfo->m_rcPos.right - m_iAX;
			iSY	=	pBGInfo->m_rcPos.bottom - m_iAY;
		}
		else
		{
			pBGInfo	=	&m_vListBoxBGTexture.at( COMBOBOX_TEXTURE_RIGHT_TOP );

			if( pBGInfo == NULL )
				return;

			iSX	=	pBGInfo->m_rcPos.right - m_iAX;
			iSY	=	m_iAY - pBGInfo->m_rcPos.bottom;
		}
	}
	else
	{
		iSX	=	m_ptEditBoxSize.x;
		iSY	=	m_ptEditBoxSize.y;
	}

	SetRectSize( iSX , iSY );
}

void SPWindowComboBox::UpdateEditBoxPos()
{
	RECT	rcEditBox;

	m_pEditText->GetWindowRect( rcEditBox );

	int		iLeft	=	rcEditBox.left - m_ptEditBoxOffset.x;
	int		iTop	=	rcEditBox.top - m_ptEditBoxOffset.y;
	int		iRight	=	rcEditBox.right - m_ptEditBoxOffset.x;
	int		iBottom	=	rcEditBox.bottom - m_ptEditBoxOffset.y;

	CBB_TEXTURE_INFO*	pBGInfo	=	NULL;

	for( int i = 0 ; i < COMBOBOX_DATA_TEXTURE_COUNT ; ++i )
	{
		//////////////////////////////////////////////////////////////////////////
		//
		//	탑라인
		//

		pBGInfo	=	&m_vEditBoxBGTexture[ i ].at( COMBOBOX_TEXTURE_LEFT_TOP );

		pBGInfo->m_rcPos.left	=	iLeft;
		pBGInfo->m_rcPos.top	=	iTop;
		pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left	+	pBGInfo->m_ptSize.x;
		pBGInfo->m_rcPos.bottom	=	pBGInfo->m_rcPos.top	+	pBGInfo->m_ptSize.y;

		int		iMiddleSX	=	pBGInfo->m_rcPos.right;
		int		iMiddleSY	=	pBGInfo->m_rcPos.bottom;

		pBGInfo	=	&m_vEditBoxBGTexture[ i ].at( COMBOBOX_TEXTURE_RIGHT_TOP );

		pBGInfo->m_rcPos.right	=	iRight;
		pBGInfo->m_rcPos.top	=	iTop;
		pBGInfo->m_rcPos.left	=	pBGInfo->m_rcPos.right	-	pBGInfo->m_ptSize.x;
		pBGInfo->m_rcPos.bottom	=	pBGInfo->m_rcPos.top	+	pBGInfo->m_ptSize.y;

		int		iMiddleEX	=	pBGInfo->m_rcPos.left;

		pBGInfo	=	&m_vEditBoxBGTexture[ i ].at( COMBOBOX_TEXTURE_CENTER_TOP );

		pBGInfo->m_rcPos.left	=	iMiddleSX;
		pBGInfo->m_rcPos.top	=	iTop;
		pBGInfo->m_rcPos.right	=	iMiddleEX;
		pBGInfo->m_rcPos.bottom	=	iMiddleSY;

		//////////////////////////////////////////////////////////////////////////
		//
		//	BOTTOM라인

		pBGInfo	=	&m_vEditBoxBGTexture[ i ].at( COMBOBOX_TEXTURE_LEFT_BOTTOM );

		pBGInfo->m_rcPos.left	=	iLeft;
		pBGInfo->m_rcPos.bottom	=	iBottom;
		pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left	+	pBGInfo->m_ptSize.x;
		pBGInfo->m_rcPos.top	=	pBGInfo->m_rcPos.bottom	-	pBGInfo->m_ptSize.y;

		pBGInfo	=	&m_vEditBoxBGTexture[ i ].at( COMBOBOX_TEXTURE_RIGHT_BOTTOM );

		pBGInfo->m_rcPos.right	=	iRight;
		pBGInfo->m_rcPos.bottom	=	iBottom;
		pBGInfo->m_rcPos.left	=	pBGInfo->m_rcPos.right	-	pBGInfo->m_ptSize.x;
		pBGInfo->m_rcPos.top	=	pBGInfo->m_rcPos.bottom	-	pBGInfo->m_ptSize.y;

		int		iMiddleEY	=	pBGInfo->m_rcPos.top;

		pBGInfo	=	&m_vEditBoxBGTexture[ i ].at( COMBOBOX_TEXTURE_CENTER_BOTTOM );

		pBGInfo->m_rcPos.left	=	iMiddleSX;
		pBGInfo->m_rcPos.top	=	iMiddleEY;
		pBGInfo->m_rcPos.right	=	iMiddleEX;
		pBGInfo->m_rcPos.bottom	=	iBottom;

		//////////////////////////////////////////////////////////////////////////
		//
		//	가운데라인

		pBGInfo	=	&m_vEditBoxBGTexture[ i ].at( COMBOBOX_TEXTURE_LEFT_MIDDLE );

		pBGInfo->m_rcPos.left	=	iLeft;
		pBGInfo->m_rcPos.top	=	iMiddleSY;
		pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left + pBGInfo->m_ptSize.x;
		pBGInfo->m_rcPos.bottom	=	iMiddleEY;

		pBGInfo	=	&m_vEditBoxBGTexture[ i ].at( COMBOBOX_TEXTURE_CENTER_MIDDLE );

		pBGInfo->m_rcPos.left	=	iMiddleSX;
		pBGInfo->m_rcPos.top	=	iMiddleSY;
		pBGInfo->m_rcPos.right	=	iMiddleEX;
		pBGInfo->m_rcPos.bottom	=	iMiddleEY;

		pBGInfo	=	&m_vEditBoxBGTexture[ i ].at( COMBOBOX_TEXTURE_RIGHT_MIDDLE );

		pBGInfo->m_rcPos.left	=	iRight	-	pBGInfo->m_ptSize.x;
		pBGInfo->m_rcPos.top	=	iMiddleSY;
		pBGInfo->m_rcPos.right	=	iRight;
		pBGInfo->m_rcPos.bottom	=	iMiddleEY;
	}
}

int SPWindowComboBox::GetScrollWidth( COMBOBOX_SCROLL_DIR eDir )
{
	int	iScrollWidth	=	0;

	if( m_bScrollUser == FALSE || m_bScrollData == FALSE )
		return iScrollWidth;

	if( eDir == COMBOBOX_SCROLL_LEFT && m_rcScrollMargin.left <= 0 )
		return iScrollWidth;
	else if( eDir == COMBOBOX_SCROLL_RIGHT && m_rcScrollMargin.right <= 0 )
		return iScrollWidth;

	RECT	rcScroll;
	int		iTemp;

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

	if( eDir == COMBOBOX_SCROLL_LEFT )
		iScrollWidth	+=	m_rcScrollMargin.left;
	else if( eDir == COMBOBOX_SCROLL_RIGHT )
		iScrollWidth	+=	m_rcScrollMargin.right;

	return iScrollWidth;
}

int SPWindowComboBox::CheckListBox( COMBOBOX_DATA_TEXTURE_LABEL eStatus , int iXPos , int iYPos )
{
	int		iItemRenderCount	=	(int)m_vItemRender.size();
	RECT*	prcRenderPos		=	NULL;
	int		iSelectNumber		=	-1;

	for( int i = 0 ; i < iItemRenderCount ; i++ )
	{
		prcRenderPos	=	&( m_vItemRender.at( i ).m_rcRenderPos );

		if( iXPos >= prcRenderPos->left	&& iXPos <= prcRenderPos->right &&
			iYPos >= prcRenderPos->top	&& iYPos <= prcRenderPos->bottom )
		{
			m_vItemRender.at( i ).m_eStatus	=	eStatus;
			iSelectNumber	=	i;
		}
		else
			m_vItemRender.at( i ).m_eStatus	=	COMBOBOX_DATA_TEXTURE_ENABLE;
	}

	return	iSelectNumber;
}

int SPWindowComboBox::GetComboBoxPosY()
{

	return m_iAX;
}
