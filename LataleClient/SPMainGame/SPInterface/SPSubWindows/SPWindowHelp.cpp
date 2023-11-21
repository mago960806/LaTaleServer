// ***************************************************************
//  SPWindowHelp.cpp   version:  1.0   date:2006/01/13
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

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"
#include "SPTreeBoxItemTexture.h"

#include "SPTreeBoxDEF.h"
#include "SPWindowTreeBox.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPDebug.h"

#include "SPWindowHelp.h"

#define HELP_CONTENTS_POS_X		169
#define HELP_CONTENTS_POS_Y		27
#define HELP_CONTENTS_SIZE_X	341
#define HELP_CONTENTS_SIZE_Y	319

//////////////////////////////////////////////////////////////////////////

SPWindowHelp::SPWindowHelp( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent )
: SPWindow( WNDID_CTRL_TREEBOX , InstanceID , iX , iY , iCX , iCY , pParent )
{
	Init();
}

SPWindowHelp::~SPWindowHelp()
{
	Clean();
}

void SPWindowHelp::Init()
{
	m_strToolTip = "[SPWindowHelp]";

	SPWindowStatic*	pWindowStatic	=	NULL;
	
	pWindowStatic	=	new SPWindowStatic( WIID_HELP_TITLE , 8 , 7 , 24 , 19 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 337 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_HELP_TITLE_TEXT , 40 , 7 , 70 , 15 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 357 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_HELP_LINE_1 , 2 , 26 , 508 , 1 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 349 , 329 );
	pWindowStatic->SetSrcSize( 5 , 1 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_HELP_LINE_2 , 2 , 346 , 508 , 1 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 349 , 329 );
	pWindowStatic->SetSrcSize( 5 , 1 );
	pWindowStatic	=	NULL;

	SPWindowButton*	pWindowButton	=	new SPWindowButton( WIID_HELP_CLOSE , 0 , 0 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 217 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 230 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 244 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 256 );
	m_pClose	=	pWindowButton;

	m_vBGTexture.reserve( HELP_TEXTURE_COUNT );
	m_vContentTexture.reserve( HELP_TEXTURE_COUNT );

	HELP_TEXTURE_INFO	stTexture;

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

	for( int i = 0 ; i < HELP_TEXTURE_COUNT ; i++ )
	{
		m_vBGTexture.push_back( stTexture );
		m_vContentTexture.push_back( stTexture );
	}

	SetTexture( HELP_TEXTURE_LEFT_TOP		, "DATA/INTERFACE/CONCEPT/UI101.PNG" ,    0 , 0 ,  10 , 4 , m_vBGTexture );
	SetTexture( HELP_TEXTURE_CENTER_TOP		, "DATA/INTERFACE/CONCEPT/UI101.PNG" ,   10 , 0 , 502 , 4 , m_vBGTexture );
	SetTexture( HELP_TEXTURE_RIGHT_TOP		, "DATA/INTERFACE/CONCEPT/UI101.PNG" ,  502 , 0 , 512 , 4 , m_vBGTexture );

	SetTexture( HELP_TEXTURE_LEFT_MIDDLE	, "DATA/INTERFACE/CONCEPT/UI101.PNG" ,    0 , 6 ,  10 , 8 , m_vBGTexture );
	SetTexture( HELP_TEXTURE_CENTER_MIDDLE	, "DATA/INTERFACE/CONCEPT/UI101.PNG" ,   10 , 6 , 502 , 8 , m_vBGTexture );
	SetTexture( HELP_TEXTURE_RIGHT_MIDDLE	, "DATA/INTERFACE/CONCEPT/UI101.PNG" ,  502 , 6 , 512 , 8 , m_vBGTexture );

	SetTexture( HELP_TEXTURE_LEFT_BOTTOM	, "DATA/INTERFACE/CONCEPT/UI101.PNG" ,    0 , 10 ,  10 , 14 , m_vBGTexture );
	SetTexture( HELP_TEXTURE_CENTER_BOTTOM	, "DATA/INTERFACE/CONCEPT/UI101.PNG" ,   10 , 10 , 502 , 14 , m_vBGTexture );
	SetTexture( HELP_TEXTURE_RIGHT_BOTTOM	, "DATA/INTERFACE/CONCEPT/UI101.PNG" ,  502 , 10 , 512 , 14 , m_vBGTexture );

	UpdateBGPos();

	m_pTreeBox	=	new SPWindowTreeBox( WIID_HELP_TREEBOX , 2 , 27 , 167 , 319 , this );
	m_pTreeBox->SetBoxTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 349 , 321 , 355 , 327 , 2 );
	m_pTreeBox->SetMargin( 8 , 10 , 8 , 10 );
	m_pTreeBox->UpdateBGPos();

	m_pTreeBox->SetScrollUpSize( 12 , 11 );
	m_pTreeBox->SetScrollUpTexture( DATA_TEXTURE_ENABLE			, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	m_pTreeBox->SetScrollUpTexture( DATA_TEXTURE_HIT			, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	m_pTreeBox->SetScrollUpTexture( DATA_TEXTURE_PUSH			, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	m_pTreeBox->SetScrollUpTexture( DATA_TEXTURE_DISABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );

	m_pTreeBox->SetScrollDownSize( 12 , 11 );
	m_pTreeBox->SetScrollDownTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	m_pTreeBox->SetScrollDownTexture( DATA_TEXTURE_HIT			, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	m_pTreeBox->SetScrollDownTexture( DATA_TEXTURE_PUSH			, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	m_pTreeBox->SetScrollDownTexture( DATA_TEXTURE_DISABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );

	m_pTreeBox->SetScrollSliderSize( 12 , 24 );
	m_pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 470 );
	m_pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 470 );
	m_pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 470 );
	m_pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 470 );

	m_pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 496 , 472 , 498 );
	m_pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 496 , 485 , 498 );
	m_pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 496 , 498 , 498 );
	m_pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 496 , 511 , 498 );

	m_pTreeBox->SetScrollMargin( 0 , 8 , 0 , 8 );
	m_pTreeBox->UpdateScrollPos();

	LoadLDTLeftMenu();

	m_pTreeBox->Refresh();

//	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI007.PNG" , &m_stContentGradation.m_pTexture );

	//m_stContentGradation.m_ptSize.x		=	HELP_CONTENTS_SIZE_X;
	//m_stContentGradation.m_ptSize.y		=	HELP_CONTENTS_SIZE_Y;
	//m_stContentGradation.m_rcSrc.left	=	0;
	//m_stContentGradation.m_rcSrc.top	=	0;
	//m_stContentGradation.m_rcSrc.right	=	339;
	//m_stContentGradation.m_rcSrc.bottom	=	174;

	SetBoxTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 349 , 321 , 355 , 327 , 2 , m_vContentTexture );

	m_pContent	=	new	SPWindow( WNDID_INVALID , 0 );
	m_pContent->SetAbsCoord( m_iAX + HELP_CONTENTS_POS_X , m_iAY + HELP_CONTENTS_POS_Y );
	m_pContent->RefreshChildPos();
	m_pContent->Show();

	LoadLDTRightContents();

	UpdateContentPos();
}

void SPWindowHelp::Clean()
{
	SPWindow::Clean();

	STD_VECTOR_HELP_TEXTURE::iterator	IterTexture = m_vBGTexture.begin();

	while( IterTexture != m_vBGTexture.end() )
	{
		SAFE_RELEASE( (*IterTexture).m_pTexture );
		IterTexture++;
	}

	m_vBGTexture.clear();

	IterTexture = m_vContentTexture.begin();

	while( IterTexture != m_vContentTexture.end() )
	{
		SAFE_RELEASE( (*IterTexture).m_pTexture );
		IterTexture++;
	}

	m_vContentTexture.clear();

	SAFE_DELETE( m_pContent );
}

void SPWindowHelp::Process( float fTime )
{
	if( !m_bShow )
		return;

	SPWindow::Process( fTime );

	if( m_pContent != NULL )
		m_pContent->Process( fTime );
}

void SPWindowHelp::Render( float fTime )
{
	if( !m_bShow )
		return;

	g_pVideo->Flush();

	HELP_TEXTURE_INFO*	pHelpTexture	=	NULL;

	for( int i = 0 ; i < HELP_TEXTURE_COUNT ; i++ )
	{
		pHelpTexture	=	&m_vBGTexture.at( i );

		if( pHelpTexture->m_pTexture == NULL )
			continue;

		pHelpTexture->m_pTexture->SetColor();
		pHelpTexture->m_pTexture->RenderTexture( &pHelpTexture->m_rcPos , &pHelpTexture->m_rcSrc );
	}

	g_pVideo->Flush();

	SPWindow::Render( fTime );

	g_pVideo->Flush();

	for( i = 0 ; i < HELP_TEXTURE_COUNT ; i++ )
	{
		pHelpTexture	=	&m_vContentTexture.at( i );

		if( pHelpTexture->m_pTexture == NULL )
			continue;

		pHelpTexture->m_pTexture->SetColor();
		pHelpTexture->m_pTexture->RenderTexture( &pHelpTexture->m_rcPos , &pHelpTexture->m_rcSrc );
	}

	g_pVideo->Flush();

	//if( m_stContentGradation.m_pTexture != NULL )
	//{
	//	m_stContentGradation.m_pTexture->SetColor();
	//	m_stContentGradation.m_pTexture->RenderTexture( &m_stContentGradation.m_rcPos , &m_stContentGradation.m_rcSrc );
	//}

	//g_pVideo->Flush();

	if( m_pContent != NULL )
		m_pContent->Render( fTime );

	g_pVideo->Flush();
}

void SPWindowHelp::Show( bool bWithChild /* = true  */ )
{
	SPWindow::Show( bWithChild );
}

void SPWindowHelp::Hide( bool bSendServer /* = true  */ )
{
	SPWindow::Hide( bSendServer );

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		SPWindow*	pWindowToolTip	=	g_pInterfaceManager->GetToolTipWindow();

		if( pWindowToolTip != NULL )
			pWindowToolTip->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , -2 );
	}
}

void SPWindowHelp::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	UpdateBGPos();
	UpdateContentPos();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowHelp )

//SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP		,												OnListBoxLButtonUp		)
//SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN	,												OnListBoxLButtonDown	)
//SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE	,												OnListBoxCursorMove		)
//SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT		,												OnListBoxCursorOut		)
//SPIMESSAGE_COMMAND(	SPIM_WHEEL_UP		,												OnListBoxWheelUp		)
//SPIMESSAGE_COMMAND(	SPIM_WHEEL_DOWN		,												OnListBoxWheelDown		)
//
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	GetInstanceID() + DROPDOWN_BUTTON		,	OnDropDownButtonUp		)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	GetInstanceID() + LISTBOX_BUTTON_UP		,	OnListBoxUpButtonUp		)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	GetInstanceID() + LISTBOX_BUTTON_DOWN	,	OnListBoxDownButtonUp	)
//
//SPIMESSAGE_CONTROL(	SPIM_SLIDER_UP		,	GetInstanceID() + LISTBOX_BUTTON_SLIDER	,	OnListBoxSliderPage		)
//SPIMESSAGE_CONTROL(	SPIM_SLIDER_DOWN	,	GetInstanceID() + LISTBOX_BUTTON_SLIDER	,	OnListBoxSliderPage		)
//SPIMESSAGE_CONTROL(	SPIM_SLIDER_PAGEUP	,	GetInstanceID() + LISTBOX_BUTTON_SLIDER	,	OnListBoxSliderPage		)
//SPIMESSAGE_CONTROL(	SPIM_SLIDER_PAGEDOWN,	GetInstanceID() + LISTBOX_BUTTON_SLIDER	,	OnListBoxSliderPage		)
//SPIMESSAGE_CONTROL(	SPIM_SET_CURVALUE	,	GetInstanceID() + LISTBOX_BUTTON_SLIDER	,	OnListBoxSliderPage		)

	SPIMESSAGE_COMMAND( SPIM_PURGE					,											OnPurgeAll				)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_HELP_TREEBOX					,	 OnTreeBoxLButtonUp		)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_HELP_CLOSE						,	 OnCloseLButtonUp		)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowHelp::OnPurgeAll( WPARAM wParam, LPARAM lParam )
{
	SetContent( 1 , m_pContent );
	m_pContent->Show();
	m_pTreeBox->Reset();
	return 1;
}

int SPWindowHelp::OnTreeBoxLButtonUp( WPARAM wParam, LPARAM lParam )
{
	TREEBOX_ITEM*	pItem	=	(TREEBOX_ITEM*)wParam;

	if( pItem == NULL )
		return 1;

	SPTreeBoxItemTexture*	pItemTexture = (SPTreeBoxItemTexture*)pItem->m_pItemBase;

	if( pItemTexture == NULL )
		return 1;

	SetContent( pItemTexture->GetReturnValue() , m_pContent );

	if( m_pContent != NULL )
		m_pContent->Show();

	return 1;
}

int SPWindowHelp::OnCloseLButtonUp( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

void SPWindowHelp::LoadLDTLeftMenu()
{
	SPLDTFile*	pLDTFile	=	NULL;

	//
	// [1/23/2006 AJJIYA]
	// HELP_MENU.LDT LOAD
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_HELP_MENU , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->LDT Load Fail [%s] \n" , RES_FNAME_LDT_HELP_MENU );
#endif
		return;
	}

	LDT_Field ldtField_Type;
	LDT_Field ldtField_Name;
	LDT_Field ldtField_Image;
	LDT_Field ldtField_Disable_Left;
	LDT_Field ldtField_Disable_Top;
	LDT_Field ldtField_Disable_Right;
	LDT_Field ldtField_Disable_Bottom;
	LDT_Field ldtField_Enable_Left;
	LDT_Field ldtField_Enable_Top;
	LDT_Field ldtField_Enable_Right;
	LDT_Field ldtField_Enable_Bottom;
	LDT_Field ldtField_MouseOver_Left;
	LDT_Field ldtField_MouseOver_Top;
	LDT_Field ldtField_MouseOver_Right;
	LDT_Field ldtField_MouseOver_Bottom;
	LDT_Field ldtField_Click_Left;
	LDT_Field ldtField_Click_Top;
	LDT_Field ldtField_Click_Right;
	LDT_Field ldtField_Click_Bottom;
	LDT_Field ldtField_Select_Left;
	LDT_Field ldtField_Select_Top;
	LDT_Field ldtField_Select_Right;
	LDT_Field ldtField_Select_Bottom;
	LDT_Field ldtField_PullDown_Enable_Left;
	LDT_Field ldtField_PullDown_Enable_Top;
	LDT_Field ldtField_PullDown_Enable_Right;
	LDT_Field ldtField_PullDown_Enable_Bottom;
	LDT_Field ldtField_PullDown_MouseOver_Left;
	LDT_Field ldtField_PullDown_MouseOver_Top;
	LDT_Field ldtField_PullDown_MouseOver_Right;
	LDT_Field ldtField_PullDown_MouseOver_Bottom;
	LDT_Field ldtField_PullDown_Click_Left;
	LDT_Field ldtField_PullDown_Click_Top;
	LDT_Field ldtField_PullDown_Click_Right;
	LDT_Field ldtField_PullDown_Click_Bottom;
	LDT_Field ldtField_PullDown_Select_Left;
	LDT_Field ldtField_PullDown_Select_Top;
	LDT_Field ldtField_PullDown_Select_Right;
	LDT_Field ldtField_PullDown_Select_Bottom;
	LDT_Field ldtField_Disable_Red;
	LDT_Field ldtField_Disable_Green;
	LDT_Field ldtField_Disable_Blue;
	LDT_Field ldtField_Enable_Red;
	LDT_Field ldtField_Enable_Green;
	LDT_Field ldtField_Enable_Blue;
	LDT_Field ldtField_MouseOver_Red;
	LDT_Field ldtField_MouseOver_Green;
	LDT_Field ldtField_MouseOver_Blue;
	LDT_Field ldtField_Click_Red;
	LDT_Field ldtField_Click_Green;
	LDT_Field ldtField_Click_Blue;
	LDT_Field ldtField_Select_Red;
	LDT_Field ldtField_Select_Green;
	LDT_Field ldtField_Select_Blue;
	LDT_Field ldtField_PullDown_Enable_Red;
	LDT_Field ldtField_PullDown_Enable_Green;
	LDT_Field ldtField_PullDown_Enable_Blue;
	LDT_Field ldtField_PullDown_MouseOver_Red;
	LDT_Field ldtField_PullDown_MouseOver_Green;
	LDT_Field ldtField_PullDown_MouseOver_Blue;
	LDT_Field ldtField_PullDown_Click_Red;
	LDT_Field ldtField_PullDown_Click_Green;
	LDT_Field ldtField_PullDown_Click_Blue;
	LDT_Field ldtField_PullDown_Select_Red;
	LDT_Field ldtField_PullDown_Select_Green;
	LDT_Field ldtField_PullDown_Select_Blue;
	LDT_Field ldtField_Bold;
	LDT_Field ldtField_Horizontal_Align;
	LDT_Field ldtField_Vertical_Align;
	LDT_Field ldtField_Left_Margin;

	int		iRecordCount	=	pLDTFile->GetItemCount();
	int		iItemID;

	int		iWidthMax		=	0;
	int		iHeightMax		=	0;
	int		iWidth , iHeight;

	std::string		strString;
	std::string		strImagePath;
	int				iType	=	0;
	int				iSrcSX	,	iSrcSY	,	iSrcEX	,	iSrcEY;
	DWORD			dwAlign;
	float			fColorR	,	fColorG	,	fColorB;

	TREEBOX_ITEM*			pParentItem		=	NULL;
	TREEBOX_ITEM*			pItem			=	NULL;
	SPTreeBoxItemTexture*	pItemTexture	=	NULL;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetFieldFromLable( iItemID, "_Type",							ldtField_Type );
		pLDTFile->GetFieldFromLable( iItemID, "_Name",							ldtField_Name );
		pLDTFile->GetFieldFromLable( iItemID, "_Image",							ldtField_Image );
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Left",					ldtField_Disable_Left );
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Top",					ldtField_Disable_Top );
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Right",					ldtField_Disable_Right );
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Bottom",				ldtField_Disable_Bottom );
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Left",					ldtField_Enable_Left );
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Top",					ldtField_Enable_Top );
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Right",					ldtField_Enable_Right );
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Bottom",					ldtField_Enable_Bottom );
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Left",				ldtField_MouseOver_Left );
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Top",					ldtField_MouseOver_Top );
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Right",				ldtField_MouseOver_Right );
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Bottom",				ldtField_MouseOver_Bottom );
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Left",					ldtField_Click_Left );
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Top",						ldtField_Click_Top );
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Right",					ldtField_Click_Right );
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Bottom",					ldtField_Click_Bottom );
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Left",					ldtField_Select_Left );
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Top",					ldtField_Select_Top );
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Right",					ldtField_Select_Right );
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Bottom",					ldtField_Select_Bottom );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Enable_Left",			ldtField_PullDown_Enable_Left );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Enable_Top",			ldtField_PullDown_Enable_Top );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Enable_Right",			ldtField_PullDown_Enable_Right );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Enable_Bottom",		ldtField_PullDown_Enable_Bottom );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_MouseOver_Left",		ldtField_PullDown_MouseOver_Left );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_MouseOver_Top",		ldtField_PullDown_MouseOver_Top );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_MouseOver_Right",		ldtField_PullDown_MouseOver_Right );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_MouseOver_Bottom",		ldtField_PullDown_MouseOver_Bottom );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Click_Left",			ldtField_PullDown_Click_Left );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Click_Top",			ldtField_PullDown_Click_Top );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Click_Right",			ldtField_PullDown_Click_Right );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Click_Bottom",			ldtField_PullDown_Click_Bottom );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Select_Left",			ldtField_PullDown_Select_Left );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Select_Top",			ldtField_PullDown_Select_Top );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Select_Right",			ldtField_PullDown_Select_Right );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Select_Bottom",		ldtField_PullDown_Select_Bottom );
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Red",					ldtField_Disable_Red );
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Green",					ldtField_Disable_Green );
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Blue",					ldtField_Disable_Blue );
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Red",					ldtField_Enable_Red );
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Green",					ldtField_Enable_Green );
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Blue",					ldtField_Enable_Blue );
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Red",					ldtField_MouseOver_Red );
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Green",				ldtField_MouseOver_Green );
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Blue",				ldtField_MouseOver_Blue );
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Red",						ldtField_Click_Red );
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Green",					ldtField_Click_Green );
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Blue",					ldtField_Click_Blue );
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Red",					ldtField_Select_Red );
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Green",					ldtField_Select_Green );
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Blue",					ldtField_Select_Blue );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Enable_Red",			ldtField_PullDown_Enable_Red );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Enable_Green",			ldtField_PullDown_Enable_Green );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Enable_Blue",			ldtField_PullDown_Enable_Blue );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_MouseOver_Red",		ldtField_PullDown_MouseOver_Red );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_MouseOver_Green",		ldtField_PullDown_MouseOver_Green );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_MouseOver_Blue",		ldtField_PullDown_MouseOver_Blue );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Click_Red",			ldtField_PullDown_Click_Red );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Click_Green",			ldtField_PullDown_Click_Green );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Click_Blue",			ldtField_PullDown_Click_Blue );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Select_Red",			ldtField_PullDown_Select_Red );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Select_Green",			ldtField_PullDown_Select_Green );
		pLDTFile->GetFieldFromLable( iItemID, "_PullDown_Select_Blue",			ldtField_PullDown_Select_Blue );
		pLDTFile->GetFieldFromLable( iItemID, "_Bold",							ldtField_Bold );
		pLDTFile->GetFieldFromLable( iItemID, "_Horizontal_Align",				ldtField_Horizontal_Align );
		pLDTFile->GetFieldFromLable( iItemID, "_Vertical_Align",				ldtField_Vertical_Align );
		pLDTFile->GetFieldFromLable( iItemID, "_Left_Margin",					ldtField_Left_Margin );

		strImagePath	=	ldtField_Image.uData.pValue;

		if( strImagePath.empty() == true )
			continue;

		//	하위로 들어갈때
		if( iType < (int)ldtField_Type.uData.lValue )
		{
			pParentItem	=	pItem;
		}
		//	상위로 올라갈때
		else if( iType > (int)ldtField_Type.uData.lValue )
		{
			if( pItem->m_pParent != NULL )
				pParentItem	=	pItem->m_pParent->m_pParent;
			else
				pParentItem = NULL;
		}

		iType	=	(int)ldtField_Type.uData.lValue;

		//	iType 1이면 최상위로
		if( iType == 1 )
		{
			pParentItem = NULL;
		}

		pItemTexture	=	new SPTreeBoxItemTexture;

		strString		=	ldtField_Name.uData.pValue;

		pItemTexture->SetText( strString.c_str() );

		iSrcSX	=	ldtField_Disable_Left.uData.lValue;
		iSrcSY	=	ldtField_Disable_Top.uData.lValue;
		iSrcEX	=	ldtField_Disable_Right.uData.lValue + 1;
		iSrcEY	=	ldtField_Disable_Bottom.uData.lValue + 1;

		iWidth	=	iSrcEX - iSrcSX;
		iHeight	=	iSrcEY - iSrcSY;
		if( iWidthMax < iWidth )
			iWidthMax = iWidth;
		if( iHeightMax < iHeight )
			iHeightMax = iHeight;

		pItemTexture->SetTexture( DATA_TEXTURE_DISABLE , strImagePath.c_str() , iSrcSX , iSrcSY , iSrcEX , iSrcEY );
		pItemTexture->SetTexture( DATA_TEXTURE_DISABLE + 1 , strImagePath.c_str() , iSrcSX , iSrcSY , iSrcEX , iSrcEY );

		iSrcSX	=	ldtField_Enable_Left.uData.lValue;
		iSrcSY	=	ldtField_Enable_Top.uData.lValue;
		iSrcEX	=	ldtField_Enable_Right.uData.lValue + 1;
		iSrcEY	=	ldtField_Enable_Bottom.uData.lValue + 1;

		iWidth	=	iSrcEX - iSrcSX;
		iHeight	=	iSrcEY - iSrcSY;
		if( iWidthMax < iWidth )
			iWidthMax = iWidth;
		if( iHeightMax < iHeight )
			iHeightMax = iHeight;

		pItemTexture->SetTexture( DATA_TEXTURE_ENABLE * 2 , strImagePath.c_str() , iSrcSX , iSrcSY , iSrcEX , iSrcEY );

		iSrcSX	=	ldtField_MouseOver_Left.uData.lValue;
		iSrcSY	=	ldtField_MouseOver_Top.uData.lValue;
		iSrcEX	=	ldtField_MouseOver_Right.uData.lValue + 1;
		iSrcEY	=	ldtField_MouseOver_Bottom.uData.lValue + 1;

		iWidth	=	iSrcEX - iSrcSX;
		iHeight	=	iSrcEY - iSrcSY;
		if( iWidthMax < iWidth )
			iWidthMax = iWidth;
		if( iHeightMax < iHeight )
			iHeightMax = iHeight;

		pItemTexture->SetTexture( DATA_TEXTURE_HIT * 2 , strImagePath.c_str() , iSrcSX , iSrcSY , iSrcEX , iSrcEY );

		iSrcSX	=	ldtField_Click_Left.uData.lValue;
		iSrcSY	=	ldtField_Click_Top.uData.lValue;
		iSrcEX	=	ldtField_Click_Right.uData.lValue + 1;
		iSrcEY	=	ldtField_Click_Bottom.uData.lValue + 1;

		iWidth	=	iSrcEX - iSrcSX;
		iHeight	=	iSrcEY - iSrcSY;
		if( iWidthMax < iWidth )
			iWidthMax = iWidth;
		if( iHeightMax < iHeight )
			iHeightMax = iHeight;

		pItemTexture->SetTexture( DATA_TEXTURE_PUSH * 2 , strImagePath.c_str() , iSrcSX , iSrcSY , iSrcEX , iSrcEY );

		iSrcSX	=	ldtField_Select_Left.uData.lValue;
		iSrcSY	=	ldtField_Select_Top.uData.lValue;
		iSrcEX	=	ldtField_Select_Right.uData.lValue + 1;
		iSrcEY	=	ldtField_Select_Bottom.uData.lValue + 1;

		iWidth	=	iSrcEX - iSrcSX;
		iHeight	=	iSrcEY - iSrcSY;
		if( iWidthMax < iWidth )
			iWidthMax = iWidth;
		if( iHeightMax < iHeight )
			iHeightMax = iHeight;

		pItemTexture->SetTexture( DATA_TEXTURE_SELECT * 2 , strImagePath.c_str() , iSrcSX , iSrcSY , iSrcEX , iSrcEY );

		iSrcSX	=	ldtField_PullDown_Enable_Left.uData.lValue;
		iSrcSY	=	ldtField_PullDown_Enable_Top.uData.lValue;
		iSrcEX	=	ldtField_PullDown_Enable_Right.uData.lValue + 1;
		iSrcEY	=	ldtField_PullDown_Enable_Bottom.uData.lValue + 1;

		iWidth	=	iSrcEX - iSrcSX;
		iHeight	=	iSrcEY - iSrcSY;
		if( iWidthMax < iWidth )
			iWidthMax = iWidth;
		if( iHeightMax < iHeight )
			iHeightMax = iHeight;

		pItemTexture->SetTexture( DATA_TEXTURE_ENABLE * 2 + 1 , strImagePath.c_str() , iSrcSX , iSrcSY , iSrcEX , iSrcEY );

		iSrcSX	=	ldtField_PullDown_MouseOver_Left.uData.lValue;
		iSrcSY	=	ldtField_PullDown_MouseOver_Top.uData.lValue;
		iSrcEX	=	ldtField_PullDown_MouseOver_Right.uData.lValue + 1;
		iSrcEY	=	ldtField_PullDown_MouseOver_Bottom.uData.lValue + 1;

		iWidth	=	iSrcEX - iSrcSX;
		iHeight	=	iSrcEY - iSrcSY;
		if( iWidthMax < iWidth )
			iWidthMax = iWidth;
		if( iHeightMax < iHeight )
			iHeightMax = iHeight;

		pItemTexture->SetTexture( DATA_TEXTURE_HIT * 2 + 1 , strImagePath.c_str() , iSrcSX , iSrcSY , iSrcEX , iSrcEY );

		iSrcSX	=	ldtField_PullDown_Click_Left.uData.lValue;
		iSrcSY	=	ldtField_PullDown_Click_Top.uData.lValue;
		iSrcEX	=	ldtField_PullDown_Click_Right.uData.lValue + 1;
		iSrcEY	=	ldtField_PullDown_Click_Bottom.uData.lValue + 1;

		iWidth	=	iSrcEX - iSrcSX;
		iHeight	=	iSrcEY - iSrcSY;
		if( iWidthMax < iWidth )
			iWidthMax = iWidth;
		if( iHeightMax < iHeight )
			iHeightMax = iHeight;

		pItemTexture->SetTexture( DATA_TEXTURE_PUSH * 2 + 1 , strImagePath.c_str() , iSrcSX , iSrcSY , iSrcEX , iSrcEY );

		iSrcSX	=	ldtField_PullDown_Select_Left.uData.lValue;
		iSrcSY	=	ldtField_PullDown_Select_Top.uData.lValue;
		iSrcEX	=	ldtField_PullDown_Select_Right.uData.lValue + 1;
		iSrcEY	=	ldtField_PullDown_Select_Bottom.uData.lValue + 1;

		iWidth	=	iSrcEX - iSrcSX;
		iHeight	=	iSrcEY - iSrcSY;
		if( iWidthMax < iWidth )
			iWidthMax = iWidth;
		if( iHeightMax < iHeight )
			iHeightMax = iHeight;

		pItemTexture->SetTexture( DATA_TEXTURE_SELECT * 2 + 1 , strImagePath.c_str() , iSrcSX , iSrcSY , iSrcEX , iSrcEY );

		//////////////////////////////////////////////////////////////////////////

		fColorR	=	ldtField_Disable_Red.uData.lValue		/ 255.0f;
		fColorG	=	ldtField_Disable_Green.uData.lValue		/ 255.0f;
		fColorB	=	ldtField_Disable_Blue.uData.lValue		/ 255.0f;
		pItemTexture->SetTextColor( DATA_STRING_DISABLE		, D3DXCOLOR( fColorR , fColorG , fColorB , 1.0f ) );
		pItemTexture->SetTextColor( DATA_STRING_DISABLE * 2 + 1 , D3DXCOLOR( fColorR , fColorG , fColorB , 1.0f ) );

		fColorR	=	ldtField_Enable_Red.uData.lValue		/ 255.0f;
		fColorG	=	ldtField_Enable_Green.uData.lValue		/ 255.0f;
		fColorB	=	ldtField_Enable_Blue.uData.lValue		/ 255.0f;
		pItemTexture->SetTextColor( DATA_STRING_ENABLE * 2	, D3DXCOLOR( fColorR , fColorG , fColorB , 1.0f ) );

		fColorR	=	ldtField_MouseOver_Red.uData.lValue		/ 255.0f;
		fColorG	=	ldtField_MouseOver_Green.uData.lValue	/ 255.0f;
		fColorB	=	ldtField_MouseOver_Blue.uData.lValue	/ 255.0f;
		pItemTexture->SetTextColor( DATA_STRING_HIT * 2		, D3DXCOLOR( fColorR , fColorG , fColorB , 1.0f ) );

		fColorR	=	ldtField_Click_Red.uData.lValue			/ 255.0f;
		fColorG	=	ldtField_Click_Green.uData.lValue		/ 255.0f;
		fColorB	=	ldtField_Click_Blue.uData.lValue		/ 255.0f;
		pItemTexture->SetTextColor( DATA_STRING_PUSH * 2	, D3DXCOLOR( fColorR , fColorG , fColorB , 1.0f ) );

		fColorR	=	ldtField_Select_Red.uData.lValue		/ 255.0f;
		fColorG	=	ldtField_Select_Green.uData.lValue		/ 255.0f;
		fColorB	=	ldtField_Select_Blue.uData.lValue		/ 255.0f;
		pItemTexture->SetTextColor( DATA_STRING_SELECT * 2	, D3DXCOLOR( fColorR , fColorG , fColorB , 1.0f ) );

		fColorR	=	ldtField_PullDown_Enable_Red.uData.lValue		/ 255.0f;
		fColorG	=	ldtField_PullDown_Enable_Green.uData.lValue		/ 255.0f;
		fColorB	=	ldtField_PullDown_Enable_Blue.uData.lValue		/ 255.0f;
		pItemTexture->SetTextColor( DATA_STRING_ENABLE * 2 + 1	, D3DXCOLOR( fColorR , fColorG , fColorB , 1.0f ) );

		fColorR	=	ldtField_PullDown_MouseOver_Red.uData.lValue	/ 255.0f;
		fColorG	=	ldtField_PullDown_MouseOver_Green.uData.lValue	/ 255.0f;
		fColorB	=	ldtField_PullDown_MouseOver_Blue.uData.lValue	/ 255.0f;
		pItemTexture->SetTextColor( DATA_STRING_HIT * 2 + 1		, D3DXCOLOR( fColorR , fColorG , fColorB , 1.0f ) );

		fColorR	=	ldtField_PullDown_Click_Red.uData.lValue		/ 255.0f;
		fColorG	=	ldtField_PullDown_Click_Green.uData.lValue		/ 255.0f;
		fColorB	=	ldtField_PullDown_Click_Blue.uData.lValue		/ 255.0f;
		pItemTexture->SetTextColor( DATA_STRING_PUSH * 2 + 1	, D3DXCOLOR( fColorR , fColorG , fColorB , 1.0f ) );

		fColorR	=	ldtField_PullDown_Select_Red.uData.lValue		/ 255.0f;
		fColorG	=	ldtField_PullDown_Select_Green.uData.lValue		/ 255.0f;
		fColorB	=	ldtField_PullDown_Select_Blue.uData.lValue		/ 255.0f;
		pItemTexture->SetTextColor( DATA_STRING_SELECT * 2 + 1	, D3DXCOLOR( fColorR , fColorG , fColorB , 1.0f ) );

		pItemTexture->SetBold( ( ldtField_Bold.uData.lValue ) ? true : false );

		dwAlign			=	NULL;

		switch( ldtField_Horizontal_Align.uData.lValue )
		{
		case 1:			dwAlign	|=	DT_LEFT;			break;
		case 2:			dwAlign	|=	DT_CENTER;		    break;
		case 3:			dwAlign	|=	DT_RIGHT;			break;
		}

		switch( ldtField_Vertical_Align.uData.lValue )
		{
		case 1:			dwAlign	|=	DT_TOP;				break;
		case 2:			dwAlign	|=	DT_VCENTER;		    break;
		case 3:			dwAlign	|=	DT_BOTTOM;			break;
		}

		pItemTexture->SetAlign( dwAlign );
		pItemTexture->SetTextMargin( ldtField_Left_Margin.uData.lValue , 0 );
		pItemTexture->SetReturnValue( iItemID );

		pItemTexture->SetSize( iWidthMax , iHeightMax );

		pItemTexture->SetStatus( DATA_STATUS_ENABLE );

		pItem	=	m_pTreeBox->AddItem( pParentItem , pItemTexture );
	}

	RECT	rcTreeBox;

	m_pTreeBox->GetWindowRect( rcTreeBox );

	int	iTreeBoxPosX	=	rcTreeBox.left	-	m_iAX;
	int iTreeBoxPosY	=	rcTreeBox.top	-	m_iAY;
	int iTreeBoxSizeY	=	rcTreeBox.bottom	-	rcTreeBox.top;
	int	iTreeBoxSizeX;

	iTreeBoxSizeX		=	m_pTreeBox->GetTreeBoxWidth( iWidthMax );

	m_pTreeBox->AdjustCoord( iTreeBoxPosX , iTreeBoxPosY , iTreeBoxSizeX , iTreeBoxSizeY );
	m_pTreeBox->RefreshRelationCoord();

	SAFE_RELEASE(pLDTFile);
}

void SPWindowHelp::LoadLDTRightContents()
{
	SPLDTFile*	pLDTFile	=	NULL;

	//
	// [1/23/2006 AJJIYA]
	// HELP_MENU.LDT LOAD
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_HELP_CONTENT , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->LDT Load Fail [%s] \n" , RES_FNAME_LDT_HELP_CONTENT );
#endif
		return;
	}

	LDT_Field ldtField_ID;
	LDT_Field ldtField_Image;
	LDT_Field ldtField_Left;
	LDT_Field ldtField_Top;
	LDT_Field ldtField_Right;
	LDT_Field ldtField_Bottom;
	LDT_Field ldtField_Image_PosX;
	LDT_Field ldtField_Image_PosY;
	LDT_Field ldtField_Width;
	LDT_Field ldtField_Height;
	LDT_Field ldtField_Text_PosX;
	LDT_Field ldtField_Text_PosY;
	LDT_Field ldtField_Red;
	LDT_Field ldtField_Green;
	LDT_Field ldtField_Blue;
	LDT_Field ldtField_Bold;
	LDT_Field ldtField_Horizontal_Align;
	LDT_Field ldtField_Vertical_Align;
	LDT_Field ldtField_Text;

	int		iRecordCount	=	pLDTFile->GetItemCount();
	int		iItemID;

	HELP_CONTENT_INFO	stContent;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetFieldFromLable( iItemID, "_ID",					ldtField_ID );
		pLDTFile->GetFieldFromLable( iItemID, "_Image",					ldtField_Image );
		pLDTFile->GetFieldFromLable( iItemID, "_Left",					ldtField_Left );
		pLDTFile->GetFieldFromLable( iItemID, "_Top",					ldtField_Top );
		pLDTFile->GetFieldFromLable( iItemID, "_Right",					ldtField_Right );
		pLDTFile->GetFieldFromLable( iItemID, "_Bottom",				ldtField_Bottom );
		pLDTFile->GetFieldFromLable( iItemID, "_Image_PosX",			ldtField_Image_PosX );
		pLDTFile->GetFieldFromLable( iItemID, "_Image_PosY",			ldtField_Image_PosY );
		pLDTFile->GetFieldFromLable( iItemID, "_Width",					ldtField_Width );
		pLDTFile->GetFieldFromLable( iItemID, "_Height",				ldtField_Height );
		pLDTFile->GetFieldFromLable( iItemID, "_Text_PosX",				ldtField_Text_PosX );
		pLDTFile->GetFieldFromLable( iItemID, "_Text_PosY",				ldtField_Text_PosY );
		pLDTFile->GetFieldFromLable( iItemID, "_Red",					ldtField_Red );
		pLDTFile->GetFieldFromLable( iItemID, "_Green",					ldtField_Green );
		pLDTFile->GetFieldFromLable( iItemID, "_Blue",					ldtField_Blue );
		pLDTFile->GetFieldFromLable( iItemID, "_Bold",					ldtField_Bold );
		pLDTFile->GetFieldFromLable( iItemID, "_Horizontal_Align",		ldtField_Horizontal_Align );
		pLDTFile->GetFieldFromLable( iItemID, "_Vertical_Align",		ldtField_Vertical_Align );
		pLDTFile->GetFieldFromLable( iItemID, "_Text",					ldtField_Text );

		stContent.m_iID				=	1;

		stContent.m_strTexture.clear();
		stContent.m_ptTexturePos.x	=	0;
		stContent.m_ptTexturePos.y	=	0;
		stContent.m_ptTextureSize.x	=	0;
		stContent.m_ptTextureSize.y	=	0;
		stContent.m_ptTextureSrc.x	=	0;
		stContent.m_ptTextureSrc.y	=	0;

		stContent.m_strText.clear();
		stContent.m_ptTextPos.x		=	0;
		stContent.m_ptTextPos.y		=	0;
		stContent.m_ptTextSize.x	=	0;
		stContent.m_ptTextSize.y	=	0;

		stContent.m_cTextColor		=	D3DXCOLOR( 0.0f , 0.0f , 0.0f , 1.0f );
		stContent.m_bTextBold		=	FALSE;
		stContent.m_dwTextAlign		=	NULL;

		if( ldtField_Image.uData.pValue != NULL || strlen( ldtField_Image.uData.pValue ) > 0 )
		{
			stContent.m_strTexture	=	ldtField_Image.uData.pValue;
		}
		else
		{
			continue;
		}

		stContent.m_iID					=	ldtField_ID.uData.lValue;

		stContent.m_ptTexturePos.x		=	ldtField_Image_PosX.uData.lValue;
		stContent.m_ptTexturePos.y		=	ldtField_Image_PosY.uData.lValue;
		stContent.m_ptTextureSize.x		=	ldtField_Right.uData.lValue - ldtField_Left.uData.lValue + 1;
		stContent.m_ptTextureSize.y		=	ldtField_Bottom.uData.lValue - ldtField_Top.uData.lValue + 1;
		stContent.m_ptTextureSrc.x		=	ldtField_Left.uData.lValue;
		stContent.m_ptTextureSrc.y		=	ldtField_Top.uData.lValue;

		if( ldtField_Text.uData.pValue != NULL || strlen( ldtField_Text.uData.pValue ) > 0 )
		{
			stContent.m_strText			=	ldtField_Text.uData.pValue;
			stContent.m_cTextColor.r	=	ldtField_Red.uData.lValue	/	255.0f;
			stContent.m_cTextColor.g	=	ldtField_Green.uData.lValue	/	255.0f;
			stContent.m_cTextColor.b	=	ldtField_Blue.uData.lValue	/	255.0f;
			stContent.m_cTextColor.a	=	1.0f;

			stContent.m_dwTextAlign		=	NULL;

			switch( ldtField_Horizontal_Align.uData.lValue )
			{
			case 1:			stContent.m_dwTextAlign	|=	DT_LEFT;			break;
			case 2:			stContent.m_dwTextAlign	|=	DT_CENTER;		    break;
			case 3:			stContent.m_dwTextAlign	|=	DT_RIGHT;			break;
			}

			switch( ldtField_Vertical_Align.uData.lValue )
			{
			case 1:			stContent.m_dwTextAlign	|=	DT_TOP;				break;
			case 2:			stContent.m_dwTextAlign	|=	DT_VCENTER;		    break;
			case 3:			stContent.m_dwTextAlign	|=	DT_BOTTOM;			break;
			}

			stContent.m_ptTextPos.x		=	ldtField_Text_PosX.uData.lValue;
			stContent.m_ptTextPos.y		=	ldtField_Text_PosY.uData.lValue;
			stContent.m_ptTextSize.x	=	ldtField_Width.uData.lValue + 1;
			stContent.m_ptTextSize.y	=	ldtField_Height.uData.lValue + 1;

			if( ldtField_Bold.uData.lValue == 1 )
				stContent.m_bTextBold	=	TRUE;
			else
				stContent.m_bTextBold	=	FALSE;
		}

		AddContent( ldtField_ID.uData.lValue , stContent );
	}

	SetContent( 1 , m_pContent );

	if( m_pContent != NULL )
		m_pContent->Show();

	SAFE_RELEASE(pLDTFile);
}

void SPWindowHelp::SetTexture( HELP_TEXTURE eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY , STD_VECTOR_HELP_TEXTURE& vHelpTexture )
{
	int	iTextureType	=	(int)eTextureType;

	if( iTextureType >= HELP_TEXTURE_COUNT )
		return;

	SPTexture*		pTexture	=	NULL;

	g_pVideo->GetTextureMgr()->LoadTexture( strFileName , &pTexture );

	HELP_TEXTURE_INFO&	stTexture	=	vHelpTexture.at( iTextureType );

	stTexture.m_rcSrc.left		=	iSrcSX;
	stTexture.m_rcSrc.top		=	iSrcSY;
	stTexture.m_rcSrc.right		=	iSrcEX;
	stTexture.m_rcSrc.bottom	=	iSrcEY;
	stTexture.m_ptSize.x		=	iSrcEX	-	iSrcSX;
	stTexture.m_ptSize.y		=	iSrcEY	-	iSrcSY;

	SAFE_RELEASE( stTexture.m_pTexture );

	stTexture.m_pTexture	=	pTexture;
}

void SPWindowHelp::SetBoxTexture( const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY , int iStep , STD_VECTOR_HELP_TEXTURE& vHelpTexture )
{
	SetTexture( HELP_TEXTURE_LEFT_TOP		, strFileName , iSrcSX			, iSrcSY , iSrcSX + iStep	, iSrcSY + iStep , vHelpTexture );
	SetTexture( HELP_TEXTURE_CENTER_TOP		, strFileName , iSrcSX + iStep	, iSrcSY , iSrcEX - iStep	, iSrcSY + iStep , vHelpTexture );
	SetTexture( HELP_TEXTURE_RIGHT_TOP		, strFileName , iSrcEX - iStep	, iSrcSY , iSrcEX			, iSrcSY + iStep , vHelpTexture );

	SetTexture( HELP_TEXTURE_LEFT_MIDDLE	, strFileName , iSrcSX			, iSrcSY + iStep , iSrcSX + iStep	, iSrcSY + iStep * 2 , vHelpTexture );
	SetTexture( HELP_TEXTURE_CENTER_MIDDLE	, strFileName , iSrcSX + iStep	, iSrcSY + iStep , iSrcEX - iStep	, iSrcSY + iStep * 2 , vHelpTexture );
	SetTexture( HELP_TEXTURE_RIGHT_MIDDLE	, strFileName , iSrcEX - iStep	, iSrcSY + iStep , iSrcEX			, iSrcSY + iStep * 2 , vHelpTexture );

	SetTexture( HELP_TEXTURE_LEFT_BOTTOM	, strFileName , iSrcSX			, iSrcEY - iStep , iSrcSX + iStep	, iSrcEY , vHelpTexture );
	SetTexture( HELP_TEXTURE_CENTER_BOTTOM	, strFileName , iSrcSX + iStep	, iSrcEY - iStep , iSrcEX - iStep	, iSrcEY , vHelpTexture );
	SetTexture( HELP_TEXTURE_RIGHT_BOTTOM	, strFileName , iSrcEX - iStep	, iSrcEY - iStep , iSrcEX			, iSrcEY , vHelpTexture );
}

void SPWindowHelp::UpdateBGPos()
{
	int		iLeft	=	m_iAX;
	int		iTop	=	m_iAY;
	int		iRight	=	m_iAX + m_iSX;
	int		iBottom	=	m_iAY + m_iSY;

	HELP_TEXTURE_INFO*	pBGInfo	=	NULL;

	//////////////////////////////////////////////////////////////////////////
	//
	//	탑라인
	//

	pBGInfo	=	&m_vBGTexture.at( HELP_TEXTURE_LEFT_TOP );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left	+	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	pBGInfo->m_rcPos.top	+	pBGInfo->m_ptSize.y;

	int		iMiddleSX	=	pBGInfo->m_rcPos.right;
	int		iMiddleSY	=	pBGInfo->m_rcPos.bottom;

	pBGInfo	=	&m_vBGTexture.at( HELP_TEXTURE_RIGHT_TOP );

	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.left	=	pBGInfo->m_rcPos.right	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	pBGInfo->m_rcPos.top	+	pBGInfo->m_ptSize.y;

	//////////////////////////////////////////////////////////////////////////
	//
	//	종료 버튼 위치

	if( m_pClose != NULL )
	{
		RECT	rcClose;
		m_pClose->GetWindowRect( rcClose );
		m_pClose->AdjustCoord( iRight - 21 - m_iAX , iTop + 9 - m_iAY , rcClose.right - rcClose.left , rcClose.bottom - rcClose.top );
	}

	//////////////////////////////////////////////////////////////////////////

	int		iMiddleEX	=	pBGInfo->m_rcPos.left;

	pBGInfo	=	&m_vBGTexture.at( HELP_TEXTURE_CENTER_TOP );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iMiddleSY;

	//////////////////////////////////////////////////////////////////////////
	//
	//	BOTTOM라인

	pBGInfo	=	&m_vBGTexture.at( HELP_TEXTURE_LEFT_BOTTOM );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.bottom	=	iBottom;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left	+	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	pBGInfo->m_rcPos.bottom	-	pBGInfo->m_ptSize.y;

	pBGInfo	=	&m_vBGTexture.at( HELP_TEXTURE_RIGHT_BOTTOM );

	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.bottom	=	iBottom;
	pBGInfo->m_rcPos.left	=	pBGInfo->m_rcPos.right	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	pBGInfo->m_rcPos.bottom	-	pBGInfo->m_ptSize.y;

	int		iMiddleEY	=	pBGInfo->m_rcPos.top;

	pBGInfo	=	&m_vBGTexture.at( HELP_TEXTURE_CENTER_BOTTOM );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iMiddleEY;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iBottom;

	//////////////////////////////////////////////////////////////////////////
	//
	//	가운데라인

	pBGInfo	=	&m_vBGTexture.at( HELP_TEXTURE_LEFT_MIDDLE );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left + pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;

	pBGInfo	=	&m_vBGTexture.at( HELP_TEXTURE_CENTER_MIDDLE );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;

	pBGInfo	=	&m_vBGTexture.at( HELP_TEXTURE_RIGHT_MIDDLE );

	pBGInfo->m_rcPos.left	=	iRight	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;
}

void SPWindowHelp::UpdateContentPos()
{
	int		iLeft	=	m_iAX	+ HELP_CONTENTS_POS_X;
	int		iTop	=	m_iAY	+ HELP_CONTENTS_POS_Y;

	if( m_pTreeBox != NULL )
	{
		RECT	rcPos;

		m_pTreeBox->GetWindowRect( rcPos );

		iLeft	=	rcPos.right;
		iTop	=	rcPos.top;
	}

	if( m_pContent != NULL )
	{
		m_pContent->SetAbsCoord( m_iAX + HELP_CONTENTS_POS_X , m_iAY + HELP_CONTENTS_POS_Y );
		m_pContent->RefreshChildPos();
	}

	int		iRight	=	iLeft	+ HELP_CONTENTS_SIZE_X;
	int		iBottom	=	iTop	+ HELP_CONTENTS_SIZE_Y;

	HELP_TEXTURE_INFO*	pBGInfo	=	NULL;

	//////////////////////////////////////////////////////////////////////////
	//
	//	탑라인
	//

	pBGInfo	=	&m_vContentTexture.at( HELP_TEXTURE_LEFT_TOP );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left	+	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	pBGInfo->m_rcPos.top	+	pBGInfo->m_ptSize.y;

	int		iMiddleSX	=	pBGInfo->m_rcPos.right;
	int		iMiddleSY	=	pBGInfo->m_rcPos.bottom;

	pBGInfo	=	&m_vContentTexture.at( HELP_TEXTURE_RIGHT_TOP );

	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.left	=	pBGInfo->m_rcPos.right	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	pBGInfo->m_rcPos.top	+	pBGInfo->m_ptSize.y;

	int		iMiddleEX	=	pBGInfo->m_rcPos.left;

	pBGInfo	=	&m_vContentTexture.at( HELP_TEXTURE_CENTER_TOP );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iTop;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iMiddleSY;

	//////////////////////////////////////////////////////////////////////////
	//
	//	BOTTOM라인

	pBGInfo	=	&m_vContentTexture.at( HELP_TEXTURE_LEFT_BOTTOM );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.bottom	=	iBottom;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left	+	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	pBGInfo->m_rcPos.bottom	-	pBGInfo->m_ptSize.y;

	pBGInfo	=	&m_vContentTexture.at( HELP_TEXTURE_RIGHT_BOTTOM );

	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.bottom	=	iBottom;
	pBGInfo->m_rcPos.left	=	pBGInfo->m_rcPos.right	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	pBGInfo->m_rcPos.bottom	-	pBGInfo->m_ptSize.y;

	int		iMiddleEY	=	pBGInfo->m_rcPos.top;

	pBGInfo	=	&m_vContentTexture.at( HELP_TEXTURE_CENTER_BOTTOM );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iMiddleEY;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iBottom;

	//////////////////////////////////////////////////////////////////////////
	//
	//	가운데라인

	pBGInfo	=	&m_vContentTexture.at( HELP_TEXTURE_LEFT_MIDDLE );

	pBGInfo->m_rcPos.left	=	iLeft;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	pBGInfo->m_rcPos.left + pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;

	pBGInfo	=	&m_vContentTexture.at( HELP_TEXTURE_CENTER_MIDDLE );

	pBGInfo->m_rcPos.left	=	iMiddleSX;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	iMiddleEX;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;

	pBGInfo	=	&m_vContentTexture.at( HELP_TEXTURE_RIGHT_MIDDLE );

	pBGInfo->m_rcPos.left	=	iRight	-	pBGInfo->m_ptSize.x;
	pBGInfo->m_rcPos.top	=	iMiddleSY;
	pBGInfo->m_rcPos.right	=	iRight;
	pBGInfo->m_rcPos.bottom	=	iMiddleEY;

	//////////////////////////////////////////////////////////////////////////
	//
	//	그라데이션

	//m_stContentGradation.m_rcPos.left	=	iLeft;
	//m_stContentGradation.m_rcPos.top	=	iTop	+	2;
	//m_stContentGradation.m_rcPos.right	=	m_stContentGradation.m_rcPos.left	+	m_stContentGradation.m_ptSize.x;
	//m_stContentGradation.m_rcPos.bottom	=	m_stContentGradation.m_rcPos.top	+	m_stContentGradation.m_ptSize.y;
}

void SPWindowHelp::AddContent(int iValue , HELP_CONTENT_INFO& stInfoData )
{
	m_mmContent.insert( STD_MAP_HELP_CONTENT::value_type( iValue , stInfoData ) );
}

void SPWindowHelp::SetContent( int iValue , SPWindow* pWindow )
{
	if( pWindow == NULL )
		return;

	pWindow->CleanChildAll();

	STD_MAP_HELP_CONTENT::iterator	mmIter	=	m_mmContent.find( iValue );
	HELP_CONTENT_INFO*		pContentInfo	=	NULL;
	SPWindowStatic*			pWindowStatic	=	NULL;

	while( mmIter != m_mmContent.end() )
	{
		pContentInfo	=	&(mmIter->second);

		if( iValue != pContentInfo->m_iID )
			break;

		if( pContentInfo != NULL )
		{
			//	이미지 윈도우 세팅
			if( pContentInfo->m_strTexture.empty() == false )
			{
				pWindowStatic	=	new SPWindowStatic( 0 , pContentInfo->m_ptTexturePos.x , pContentInfo->m_ptTexturePos.y , pContentInfo->m_ptTextureSize.x , pContentInfo->m_ptTextureSize.y , pWindow );

				if( pWindowStatic != NULL )
					pWindowStatic->SetImage( pContentInfo->m_strTexture.c_str() , pContentInfo->m_ptTextureSrc.x , pContentInfo->m_ptTextureSrc.y );
			}

			//	텍스트 윈도우 세팅
			if( pContentInfo->m_strText.empty() == false )
			{
				pWindowStatic	=	new SPWindowStatic( 0 , pContentInfo->m_ptTextPos.x , pContentInfo->m_ptTextPos.y , pContentInfo->m_ptTextSize.x , pContentInfo->m_ptTextSize.y , pWindow );

				if( pWindowStatic != NULL )
				{
					pWindowStatic->SetWindowText( pContentInfo->m_strText.c_str() );
					pWindowStatic->SetFontColor( pContentInfo->m_cTextColor );
					pWindowStatic->SetFormat( pContentInfo->m_dwTextAlign );
				}

				if( pContentInfo->m_bTextBold == TRUE )
				{
					pWindowStatic	=	new SPWindowStatic( 0 , pContentInfo->m_ptTextPos.x + 1 , pContentInfo->m_ptTextPos.y + 1 , pContentInfo->m_ptTextSize.x , pContentInfo->m_ptTextSize.y , pWindow );

					if( pWindowStatic != NULL )
					{
						pWindowStatic->SetWindowText( pContentInfo->m_strText.c_str() );
						pWindowStatic->SetFontColor( pContentInfo->m_cTextColor );
						pWindowStatic->SetFormat( pContentInfo->m_dwTextAlign );
					}
				}
			}
		}

		++mmIter;
	}
}
