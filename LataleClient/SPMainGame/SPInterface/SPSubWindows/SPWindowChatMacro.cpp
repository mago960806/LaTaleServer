// ***************************************************************
//  SPWindowChatMacro   version:  1.0   ·  date: 08/07/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowEditMultiLine.h"
#include "SPWindowSlider.h"

#include "SPUIUnit.h"
#include "SPUIChatMacro.h"
#include "SPUIUnitManager.h"
#include "SPDragnDropManager.h"

#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPAbuseFilter.h"
#include "SPNoticeBox.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"
#include "SPManager.h"
#include "SPItem.h"
#include "SPItemCluster.h"

#include "SPLocalizeManager.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPWindowChatMacro.h"

SPWindowChatMacro::SPWindowChatMacro(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowChatMacro::SPWindowChatMacro(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_CHAT_MACRO, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowChatMacro::~SPWindowChatMacro()
{
	Clean();
}

void SPWindowChatMacro::Init()
{
	m_strToolTip		=	"[SPWindowChatMacro]";

	//////////////////////////////////////////////////////////////////////////

	LoadChatMacroLDT();

	//////////////////////////////////////////////////////////////////////////

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[ CHAT_MACRO_BG_LEFT_TOP ]		, 482 , 316 , 486 , 320 );
	SetRect( &m_rcBGSrc[ CHAT_MACRO_BG_CENTER_TOP ]		, 488 , 316 , 490 , 320 );
	SetRect( &m_rcBGSrc[ CHAT_MACRO_BG_RIGHT_TOP ]		, 492 , 316 , 496 , 320 );

	SetRect( &m_rcBGSrc[ CHAT_MACRO_BG_LEFT_MIDDLE ]	, 482 , 322 , 486 , 324 );
	SetRect( &m_rcBGSrc[ CHAT_MACRO_BG_CENTER_MIDDLE ]	, 488 , 322 , 490 , 324 );
	SetRect( &m_rcBGSrc[ CHAT_MACRO_BG_RIGHT_MIDDLE ]	, 492 , 322 , 496 , 324 );

	SetRect( &m_rcBGSrc[ CHAT_MACRO_BG_LEFT_BOTTOM ]	, 482 , 326 , 486 , 330 );
	SetRect( &m_rcBGSrc[ CHAT_MACRO_BG_CENTER_BOTTOM ]	, 488 , 326 , 490 , 330 );
	SetRect( &m_rcBGSrc[ CHAT_MACRO_BG_RIGHT_BOTTOM ]	, 492 , 326 , 496 , 330 );

	SetRect( &m_rcBGSrc[ CHAT_MACRO_BG_SUB_WHITE ]		, 488 , 337 , 490 , 339 );

	//////////////////////////////////////////////////////////////////////////

	SPWindowButton*	pWindowButton;
	SPWindowStatic*	pWindowStatic;

	bool	bNormalButtonEnable		=	true;
	bool	bSpecialButtonEnable	=	true;

	//////////////////////////////////////////////////////////////////////////

	if( GetMacroLdtData( 0 , CHAT_MACRO_TYPE_NORMAL ) == NULL )
		bNormalButtonEnable		=	false;

	if( GetMacroLdtData( 0 , CHAT_MACRO_TYPE_SPECIAL ) == NULL )
		bSpecialButtonEnable	=	false;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_IMG_MICRO , 6 , 4 , 18 , 19 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 453 , 473 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_IMG_MAIN_TITLE , 26 , 7 , 182 , 17 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 211 , 396 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHAT_MACRO_CLOSE_ICON , 229 , 8 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 217 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 230 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 243 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 256 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHAT_MACRO_NORMAL , 9 , 29 , 63 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 70 , 323 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 70 , 339 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 70 , 355 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 70 , 371 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 70 , 355 );
	pWindowButton->SetEnable( bNormalButtonEnable );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHAT_MACRO_SPECIAL , 72 , 29 , 63 , 15 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 134 , 323 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 134 , 339 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 134 , 355 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 134 , 371 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 134 , 355 );
	pWindowButton->SetEnable( bSpecialButtonEnable );
	pWindowButton	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowButton	=	new	SPWindowButton( WIID_CHAT_MACRO_SLIDER_UP , 226 , 63 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CHAT_MACRO_SLIDER_DOWN , 226 , 136 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );
	pWindowButton =	NULL;

	RECT rtRect;
	m_pSlider	=	new	SPWindowSlider( WIID_CHAT_MACRO_SLIDER , 226 , 74 , 12 , 62 , this );

	SetRect( &rtRect , 460 , 470 , 472 , 494 );	//	일반
	m_pSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 473 , 470 , 485 , 494 );	//	마우스 오버
	m_pSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 486 , 470 , 498 , 494 );	//	마우스 클릭
	m_pSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 499 , 470 , 511 , 494 );
	m_pSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 460 , 496 , 472 , 498 );	//	일반
	m_pSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 473 , 496 , 485 , 498 );	//	마우스 오버
	m_pSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 486 , 496 , 498 , 498 );	//	마우스 클릭
	m_pSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 499 , 496 , 511 , 498 );
	m_pSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	m_pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pSlider->SetCurpage( 0 );
	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );
	m_pSlider->Refresh();

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_IMG_TITLE , 10 , 167 , 122 , 17 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 300 , 431 );
	pWindowStatic	=	NULL;

	SPWindowEdit*	pWindowEdit;

	pWindowEdit		= new SPWindowEdit( WIID_CHAT_MACRO_TITLE_EDIT , 36 , 189 , 43 , 12 , this ) ;
	pWindowEdit->SetLimitText( 4 );

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_TITLE_EDIT_COMMENT , 83 , 187 , 138 , 15 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 198 , 362 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_IMG_CONTENTS , 10 , 206 , 122 , 16 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 300 , 414 );
	pWindowStatic	=	NULL;

	SPWindowEditMultiLine*	pWindowEditMultiLine;

	pWindowEditMultiLine		=	new SPWindowEditMultiLine( WIID_CHAT_MACRO_CONTENTS_EDIT , 36, 228 , 155 , 54 , this ) ;
	pWindowEditMultiLine->SetLimitText( 60 );
	pWindowEditMultiLine->SPSendMessage( SPIM_SET_MARGIN , 2 );

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_TIP , 22 , 298 , 204 , 27 , this );
	pWindowStatic->SetMultiLine( true , 1 );
	pWindowStatic->SetFontColor( D3DXCOLOR( 147.0f / 255.0f , 43.0f / 255.0f , 43.0f / 255.0f , 1.0f ) );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_CONTENTS_COUNT , 200 , 272 , 42 , 14 , this );
	pWindowStatic->SetWindowText( NULL );	// 60/60
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_MENU_BG_LEFT , 2 , 26 , 2 , 23 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 501 , 369 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_MENU_BG_MIDDLE , 4 , 26 , 240 , 23 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 505 , 369 );
	pWindowStatic->SetSrcSize( 2 , 23 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_MENU_BG_RIGHT , 244 , 26 , 2 , 23 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 509 , 369 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_TITLE_EDIT_BG_LEFT , 34 , 187 , 2 , 15 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 389 , 449 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_TITLE_EDIT_BG_MIDDLE , 36 , 187 , 42 , 15 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 393 , 449 );
	pWindowStatic->SetSrcSize( 2 , 15 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_TITLE_EDIT_BG_RIGHT , 78 , 187 , 2 , 15 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 397 , 449 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_CONTENTS_EDIT_BG_LEFT , 33 , 225 , 2 , 60 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 501 , 308 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_CONTENTS_EDIT_BG_MIDDLE , 35 , 225 , 157 , 60 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 505 , 308 );
	pWindowStatic->SetSrcSize( 2 , 60 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_CONTENTS_EDIT_BG_RIGHT , 192 , 225 , 2 , 60 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 509 , 308 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_TIP_BG_LEFT , 2 , 293 , 2 , 38 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 501 , 269 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_TIP_BG_MIDDLE , 4 , 293 , 240 , 38 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 505 , 269 );
	pWindowStatic->SetSrcSize( 2 , 38 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_TIP_BG_RIGHT , 244 , 293 , 2 , 38 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 509 , 269 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_CHAT_MACRO_LINE , 8 , 161 , 230 , 1 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 267 , 414 );
	pWindowStatic->SetSrcSize( 2 , 1 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowButton	=	new	SPWindowButton( WIID_CHAT_MACRO_CLOSE , 90 , 335 , 68 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 1 , 270 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 1 , 288 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 1 , 306 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 1 , 324 );
	pWindowButton	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	InitChatMacroUnit();

	m_iSelectIndex	=	-1;

	//////////////////////////////////////////////////////////////////////////

	m_bUse	=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_CHATMACRO ) ;

	CHAT_MACRO_TYPE	eType	=	CHAT_MACRO_TYPE_NORMAL;

	if( bSpecialButtonEnable == true && bNormalButtonEnable == false )
		eType	=	CHAT_MACRO_TYPE_SPECIAL;

	if( bNormalButtonEnable == false && bSpecialButtonEnable == false )
		m_bUse	=	false;
	else
		Update( eType );
}

void SPWindowChatMacro::ReposOwnImage()
{
	SetRect( &m_rcBGDest[ CHAT_MACRO_BG_LEFT_TOP ]		, m_iAX					, m_iAY					, m_iAX + 4			, m_iAY + 4 );
	SetRect( &m_rcBGDest[ CHAT_MACRO_BG_CENTER_TOP ]	, m_iAX + 4				, m_iAY					, m_iAX + 4 + 240	, m_iAY + 4 );
	SetRect( &m_rcBGDest[ CHAT_MACRO_BG_RIGHT_TOP ]		, m_iAX + 244			, m_iAY					, m_iAX + 244 + 4	, m_iAY + 4 );

	SetRect( &m_rcBGDest[ CHAT_MACRO_BG_LEFT_MIDDLE ]	, m_iAX					, m_iAY + 4				, m_iAX + 4			, m_iAY + 4 + 350 );
	SetRect( &m_rcBGDest[ CHAT_MACRO_BG_CENTER_MIDDLE ]	, m_iAX + 4				, m_iAY	+ 4				, m_iAX + 4 + 240	, m_iAY + 4 + 350 );
	SetRect( &m_rcBGDest[ CHAT_MACRO_BG_RIGHT_MIDDLE ]	, m_iAX + 244			, m_iAY + 4				, m_iAX + 244 + 4	, m_iAY + 4 + 350 );

	SetRect( &m_rcBGDest[ CHAT_MACRO_BG_LEFT_BOTTOM ]	, m_iAX					, m_iAY + 354			, m_iAX + 4			, m_iAY + 354 + 4 );
	SetRect( &m_rcBGDest[ CHAT_MACRO_BG_CENTER_BOTTOM ]	, m_iAX + 4				, m_iAY	+ 354			, m_iAX + 4 + 240	, m_iAY + 354 + 4 );
	SetRect( &m_rcBGDest[ CHAT_MACRO_BG_RIGHT_BOTTOM ]	, m_iAX + 244			, m_iAY + 354			, m_iAX + 244 + 4	, m_iAY + 354 + 4 );

	SetRect( &m_rcBGDest[ CHAT_MACRO_BG_SUB_WHITE ]		, m_iAX + 2				, m_iAY	+ 49			, m_iAX + 2 + 244	, m_iAY + 49 + 244 );
}

void SPWindowChatMacro::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );

	m_UIUnitManager.ClearAll( TRUE );
}

void SPWindowChatMacro::Show(bool bWithChild)
{
	if( m_bUse == false )
		return;

	if( m_bShow )
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Show( bWithChild );

	Update( GetMacroType() );
}

void SPWindowChatMacro::Hide(bool bSendServer)
{
	MacroDataMemToFile();
	SaveMacroFile();

	if( m_iSelectIndex >= 0 && m_pSlider != NULL )
		UpdateSelectIcon( m_iSelectIndex - m_pSlider->GetCurPage() * CHAT_MACRO_UNIT_X_COUNT );

	ReFreshQuickSlot();

	if( m_bShow )
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide( bSendServer );
}

void SPWindowChatMacro::Process(float fTime)
{
	if( m_bShow == FALSE ) return;

	SPWindow::Process( fTime );

	m_UIUnitManager.Process( fTime );

//	ProcessEditCtrl( fTime );
}

void SPWindowChatMacro::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < CHAT_MACRO_BG_COUNT ; ++i )
		{
			m_pBGTexture->SetColor( m_RGBA );
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	SPWindow::RenderReverse( fTime );

	g_pVideo->Flush();

	m_UIUnitManager.Render();

	g_pVideo->Flush();
}

void SPWindowChatMacro::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
	UpdatePosChatMacroUnit();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowChatMacro )
	SPIMESSAGE_COMMAND(	SPIM_MOVE_BEGIN			,												OnBeginMove			)

	SPIMESSAGE_COMMAND(	SPIM_DRAGNDROP_END		,												OnDragnDropEnd		)

	SPIMESSAGE_COMMAND( SPIM_CHATMACRO_GET_DATA	,												OnGetData			)
	SPIMESSAGE_COMMAND( SPIM_REFRESH			,												OnReFresh			)
	SPIMESSAGE_COMMAND( SPIM_KEY_TAB			,												OnTab				)
	SPIMESSAGE_COMMAND( SPIM_WHEEL_UP			,												OnSliderUp			)
	SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN			,												OnSliderDown		)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_MACRO_SLIDER_UP				,	OnSliderUp			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_MACRO_SLIDER_DOWN				,	OnSliderDown		)

	SPIMESSAGE_CONTROL( SPIM_SLIDER_UP			,	WIID_CHAT_MACRO_SLIDER					,	OnSlider			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN		,	WIID_CHAT_MACRO_SLIDER					,	OnSlider			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP		,	WIID_CHAT_MACRO_SLIDER					,	OnSlider			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN	,	WIID_CHAT_MACRO_SLIDER					,	OnSlider			)
	SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE		,	WIID_CHAT_MACRO_SLIDER					,	OnSlider			)

	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS			,	WIID_CHAT_MACRO_TITLE_EDIT				,	OnEditSetFocus		)
	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS			,	WIID_CHAT_MACRO_CONTENTS_EDIT			,	OnEditSetFocus		)

	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS			,	WIID_CHAT_MACRO_TITLE_EDIT				,	OnEditKillFocus		)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS			,	WIID_CHAT_MACRO_CONTENTS_EDIT			,	OnEditKillFocus		)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_MACRO_CLOSE_ICON				,	OnCloseLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_MACRO_CLOSE					,	OnCloseLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_MACRO_NORMAL					,	OnNormalLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_CHAT_MACRO_SPECIAL					,	OnSpecialLButtonUp	)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	,	WIID_CHAT_MACRO_UNIT_OUTLINE	,	WIID_CHAT_MACRO_UNIT_OUTLINE + CHAT_MACRO_UNIT_COUNT	,	OnUnitLButtonUp		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DOWN	,	WIID_CHAT_MACRO_UNIT_OUTLINE	,	WIID_CHAT_MACRO_UNIT_OUTLINE + CHAT_MACRO_UNIT_COUNT	,	OnUnitLButtonDown	)
	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_MOVE	,	WIID_CHAT_MACRO_UNIT_OUTLINE	,	WIID_CHAT_MACRO_UNIT_OUTLINE + CHAT_MACRO_UNIT_COUNT	,	OnUnitCursorMove	)
	SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP		,	WIID_CHAT_MACRO_UNIT_OUTLINE	,	WIID_CHAT_MACRO_UNIT_OUTLINE + CHAT_MACRO_UNIT_COUNT	,	OnUnitSliderUp		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN	,	WIID_CHAT_MACRO_UNIT_OUTLINE	,	WIID_CHAT_MACRO_UNIT_OUTLINE + CHAT_MACRO_UNIT_COUNT	,	OnUnitSliderDown	)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowChatMacro::OnBeginMove( WPARAM wParam, LPARAM lParam )
{
	int				iXPos		=	g_pInterfaceManager->GetCursorX();
	int				iYPos		=	g_pInterfaceManager->GetCursorY();
	SPUIUnit*		pUIUnit		=	m_UIUnitManager.GetUnitMouseOver( iXPos , iYPos );

	if( pUIUnit == NULL )
	{
		return SPWindow::OnBeginMove(wParam, lParam);
	}

	return 0;
}

int SPWindowChatMacro::OnDragnDropEnd( WPARAM wParam, LPARAM lParam )
{
	m_bLMouseDown	=	FALSE;
	m_lParam		=	0;

	// AJJIYA [9/8/2005]
	// 사운드 추가
	g_pInterfaceManager->RegisterSoundUnit( "DATA/SOUND/SYSTEM_ICON_PUT_DOWN.WAV" );

	return 1;
}

int SPWindowChatMacro::OnGetData( WPARAM wParam, LPARAM lParam )
{
	CHAT_MACRO_POS*		pChatMacroPos	=	(CHAT_MACRO_POS*)wParam;
	SPUIChatMacro*		pUIChatMacro	=	(SPUIChatMacro*)lParam;

	if( pChatMacroPos == NULL || pUIChatMacro == NULL )
		return 1;

	pUIChatMacro->SetChatMacro( GetMacroData( pChatMacroPos->m_iIndex , pChatMacroPos->m_eType ) );
	pUIChatMacro->SetChatMacroLDT( GetMacroLdtData( pChatMacroPos->m_iIndex , pChatMacroPos->m_eType ) );

	return 1;
}

int SPWindowChatMacro::OnReFresh( WPARAM wParam, LPARAM lParam )
{
	LoadMacroFile();
	MacroDataFileToMem();
	return 1;
}

int SPWindowChatMacro::OnTab( WPARAM wParam, LPARAM lParam )
{
	SPWindow*	pWindow;

	pWindow	=	g_pInterfaceManager->GetFocusWindow();

	if( pWindow == NULL )
		return 1;

	INSTANCE_ID	eInstanceID	=	WIID_CHAT_MACRO_TITLE_EDIT;

	if( pWindow->GetInstanceID() == WIID_CHAT_MACRO_TITLE_EDIT )
		eInstanceID	=	WIID_CHAT_MACRO_CONTENTS_EDIT;
	else if( pWindow->GetInstanceID() != WIID_CHAT_MACRO_TITLE_EDIT && pWindow->GetInstanceID() != WIID_CHAT_MACRO_CONTENTS_EDIT )
		return 1;
	
	g_pInterfaceManager->SetFocusWindow( Find( eInstanceID ) );

	return 1;
}

int SPWindowChatMacro::OnSliderUp( WPARAM wParam, LPARAM lParam )
{
	if( m_pSlider != NULL )
		m_pSlider->SPSendMessage( SPIM_SLIDER_UP );

	g_pInterfaceManager->SetFocusWindow( NULL );

	return 1;
}

int SPWindowChatMacro::OnSliderDown( WPARAM wParam, LPARAM lParam )
{
	if( m_pSlider != NULL )
		m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );

	g_pInterfaceManager->SetFocusWindow( NULL );

	return 1;
}

int SPWindowChatMacro::OnSlider( WPARAM wParam, LPARAM lParam )
{
//	int	iNewPage	=	(int)wParam;

	UpdateUnit( GetMacroType() );

	if( m_iSelectIndex >= 0 && m_pSlider != NULL )
		UpdateSelectIcon( m_iSelectIndex - m_pSlider->GetCurPage() * CHAT_MACRO_UNIT_X_COUNT );

	return 1;
}

int SPWindowChatMacro::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}

int SPWindowChatMacro::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	const char*	pszName		=	NULL;
	const char*	pszContents	=	NULL;

	SPWindow*	pWindow		=	NULL;

	pWindow	=	Find( WIID_CHAT_MACRO_TITLE_EDIT );

	if( pWindow != NULL )
		pszName	=	pWindow->GetWindowText();

	pWindow	=	Find( WIID_CHAT_MACRO_CONTENTS_EDIT );

	if( pWindow != NULL )
		pszContents	=	pWindow->GetWindowText();

	if( m_iSelectIndex >= 0 )
		SaveMacroDataMem( m_iSelectIndex , GetMacroType() , pszName , pszContents );

	MacroDataMemToFile();
	SaveMacroFile();

	Update( GetMacroType() );

	ReFreshQuickSlot();

	return 1;
}

int SPWindowChatMacro::OnCloseLButtonUp( WPARAM wParam, LPARAM lParam )
{
	Hide();

	return 1;
}

int SPWindowChatMacro::OnNormalLButtonUp( WPARAM wParam, LPARAM lParam )
{
	Update( CHAT_MACRO_TYPE_NORMAL );

	return 1;
}

int SPWindowChatMacro::OnSpecialLButtonUp( WPARAM wParam, LPARAM lParam )
{
	Update( CHAT_MACRO_TYPE_SPECIAL );

	return 1;
}

int SPWindowChatMacro::OnUnitLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	m_bLMouseDown		=	FALSE;
	m_lParam			=	0;

	return 1;
}

int SPWindowChatMacro::OnUnitLButtonDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	int				iXPos			=	LOWORD(lParam);
	int				iYPos			=	HIWORD(lParam);
	int				iUnitNumber		=	m_UIUnitManager.GetNumberMouseOver( iXPos ,iYPos );

	if( iUnitNumber >= 0 )
	{
		m_bLMouseDown	=	TRUE;
		m_lParam		=	lParam;
	}

	BOOL			bIsDragging		=	SPDragnDropManager::GetInstance()->IsDragging();

	if( bIsDragging == FALSE )
	{
		int	iSelectNumber	=	iID - WIID_CHAT_MACRO_UNIT_OUTLINE;

		UpdateSelectIcon( iSelectNumber );
	}

	return 1;
}

int SPWindowChatMacro::OnUnitCursorMove( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( IsShow() == FALSE )
		return 1;

	int				iXPos			=	LOWORD(lParam);
	int				iYPos			=	HIWORD(lParam);
	int				iXPosOld		=	LOWORD(m_lParam);
	int				iYPosOld		=	HIWORD(m_lParam);
	int				iUnitNumber		=	m_UIUnitManager.GetNumberMouseOver( iXPosOld , iYPosOld );
	SPUIUnit*		pUIUnit			=	m_UIUnitManager.GetUnit( iUnitNumber );
	BOOL			bIsDragging		=	SPDragnDropManager::GetInstance()->IsDragging();

	if( pUIUnit == NULL || bIsDragging == TRUE )
		return 1;

	int	iXStep	=	abs( iXPos - iXPosOld );
	int iYStep	=	abs( iYPos - iYPosOld );

	if( bIsDragging == FALSE )
	{
		SPUIChatMacro*	pUIChatMacroUnit	=	(SPUIChatMacro*)pUIUnit;

		if( pUIChatMacroUnit == NULL )
			return 1;

		CHAT_MACRO*	pChatMacro	=	pUIChatMacroUnit->GetChatMacro();

		if( pChatMacro == NULL || pUIChatMacroUnit->GetChatMacroLDT() == NULL )
			return 1;

		if( pChatMacro->m_strName.empty() == true && pChatMacro->m_strContents.empty() == true )
			return 1;

		if( m_bLMouseDown == TRUE && ( iXStep > 5 || iYStep > 5 ) )
		{
			if( SPDragnDropManager::GetInstance()->Set( this , pUIUnit ) == FALSE )
			{
				m_bLMouseDown	=	FALSE;
				m_lParam		=	0;
			}
		}
	}

	return 1;
}

int SPWindowChatMacro::OnUnitSliderUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnSliderUp( wParam , lParam );
}

int SPWindowChatMacro::OnUnitSliderDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnSliderDown( wParam , lParam );
}

void SPWindowChatMacro::LoadChatMacroLDT()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/CHAT_MACRO.LDT" , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [ DATA/LDT/CHAT_MACRO.LDT ] \n" );
#endif
		return;
	}

	LDT_Field		ldtFieldType;
	LDT_Field		ldtFieldSubType;
	LDT_Field		ldtFieldCount;
	LDT_Field		ldtFieldIconFile;
	LDT_Field		ldtFieldIconIndex;
	LDT_Field		ldtFieldRequireItem;
	LDT_Field		ldtFieldFactorType;

	int				iRecordCount	= pLDTFile->GetItemCount();
	int				iItemID;
	CHAT_MACRO_LDT	stChatMacroLdt;

	stChatMacroLdt.m_strAppName		=	"ChatMacro";

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetField( iItemID, 0,		ldtFieldType );			//	_Type
		pLDTFile->GetField( iItemID, 1,		ldtFieldSubType );		//	_SubType
		pLDTFile->GetField( iItemID, 2,		ldtFieldCount );		//	_Count
		pLDTFile->GetField( iItemID, 3,		ldtFieldIconFile );		//	_IconFile
		pLDTFile->GetField( iItemID, 4,		ldtFieldIconIndex );	//	_IconIndex
		pLDTFile->GetField( iItemID, 5,		ldtFieldRequireItem );	//	_RequireItem
		pLDTFile->GetField( iItemID, 6,		ldtFieldFactorType );	//	_FactorType

		stChatMacroLdt.m_eType			=	(CHAT_MACRO_TYPE)ldtFieldType.uData.lValue;
		stChatMacroLdt.m_iSubType		=	ldtFieldSubType.uData.lValue;
		stChatMacroLdt.m_iCount			=	ldtFieldCount.uData.lValue;
		stChatMacroLdt.m_iIconFile		=	ldtFieldIconFile.uData.lValue;
		stChatMacroLdt.m_iIconIndex		=	ldtFieldIconIndex.uData.lValue;
		stChatMacroLdt.m_iRequireItem	=	ldtFieldRequireItem.uData.lValue;

		if( ldtFieldFactorType.uData.pValue != NULL )
			stChatMacroLdt.m_strKeyName	=	ldtFieldFactorType.uData.pValue;

		m_vLDTBuffer[ stChatMacroLdt.m_eType ].push_back( stChatMacroLdt );
	}

	SAFE_RELEASE( pLDTFile );
}

void SPWindowChatMacro::InitChatMacroUnit()
{
	char				szResPath[]			=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
	SPUIChatMacro*		pUIChatMacroUnit	=	NULL;
	SPWindowStatic*		pWindowStatic		=	NULL;
	int					iXPos , iYPos;

	m_UIUnitManager.SetBufferSize( CHAT_MACRO_UNIT_COUNT );

	for( int y = 0 ; y < CHAT_MACRO_UNIT_Y_COUNT ; ++y )
	{
		for( int x = 0 ; x < CHAT_MACRO_UNIT_X_COUNT ; ++x )
		{
			iXPos	=	m_iAX + CHAT_MACRO_UNIT_X_OFFSET + ( x * CHAT_MACRO_UNIT_X_GAP );
			iYPos	=	m_iAY + CHAT_MACRO_UNIT_Y_OFFSET + ( y * CHAT_MACRO_UNIT_Y_GAP );

			pUIChatMacroUnit	=	new	SPUIChatMacro;

			pUIChatMacroUnit->SetTexture( szResPath );
			pUIChatMacroUnit->SetRectSrc( 400 , 20 , 432 , 52 );
			pUIChatMacroUnit->SetPos( iXPos , iYPos );
			pUIChatMacroUnit->SetTextRect( iXPos + 2 , iYPos + 18 , iXPos + 32 , iYPos + 32 );
			pUIChatMacroUnit->SetSize( CHAT_MACRO_UNIT_X_SIZE , CHAT_MACRO_UNIT_Y_SIZE );
			pUIChatMacroUnit->SetDisableTexture( szResPath );
			pUIChatMacroUnit->SetDisableRectSrc( 434 , 37 , 436 , 39 );
			pUIChatMacroUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
			pUIChatMacroUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIChatMacroUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_NO_TOOLTIP );
			pUIChatMacroUnit->SetInstanceID( WIID_CHAT_MACRO );

			m_UIUnitManager.AddUnit( (SPUIUnit*)pUIChatMacroUnit );

			//////////////////////////////////////////////////////////////////////////

			pWindowStatic	=	new	SPWindowStatic( WIID_CHAT_MACRO_UNIT_OUTLINE + x + y * CHAT_MACRO_UNIT_X_COUNT , iXPos - CHAT_MACRO_UNIT_X_OUTLINE - m_iAX , iYPos - CHAT_MACRO_UNIT_Y_OUTLINE - m_iAY , CHAT_MACRO_UNIT_X_OUTLINE_SIZE , CHAT_MACRO_UNIT_Y_OUTLINE_SIZE , this );
			pWindowStatic->SetImage		( (SPTexture*)NULL );
			pWindowStatic->SetImage		("DATA/INTERFACE/CONCEPT/UI107.PNG", 415 , 456 );
		}
	}
}

void SPWindowChatMacro::UpdatePosChatMacroUnit()
{
	int				iXPos , iYPos;
	SPUIChatMacro*	pUIChatMacroUnit	=	NULL;
	int				iUnitNumber			=	0;

	for( int y = 0 ; y < CHAT_MACRO_UNIT_Y_COUNT ; ++y )
	{
		for( int x = 0 ; x < CHAT_MACRO_UNIT_X_COUNT ; ++x )
		{
			iXPos	=	m_iAX + CHAT_MACRO_UNIT_X_OFFSET + ( x * CHAT_MACRO_UNIT_X_GAP );
			iYPos	=	m_iAY + CHAT_MACRO_UNIT_Y_OFFSET + ( y * CHAT_MACRO_UNIT_Y_GAP );

			pUIChatMacroUnit	=	(SPUIChatMacro*)( m_UIUnitManager.GetUnit( iUnitNumber++ ) );

			if( pUIChatMacroUnit != NULL )
			{
				pUIChatMacroUnit->SetPos( iXPos , iYPos );
				pUIChatMacroUnit->SetTextRect( iXPos + 2 , iYPos + 18 , iXPos + 32 , iYPos + 32 );
			}
		}
	}

	m_UIUnitManager.ReCalRect();
}

void SPWindowChatMacro::Update( CHAT_MACRO_TYPE eType )
{
	BOOL	bNoramlSelect	=	FALSE;
	BOOL	bSpecialSelect	=	FALSE;

	if( eType == CHAT_MACRO_TYPE_NORMAL )
		bNoramlSelect	=	TRUE;
	else if( eType == CHAT_MACRO_TYPE_SPECIAL )
		bSpecialSelect	=	TRUE;

	if( eType != GetMacroType() )
		ClearPage();

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_CHAT_MACRO_NORMAL );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_BTN_SELECT , bNoramlSelect );

	pWindow	=	Find( WIID_CHAT_MACRO_SPECIAL );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_BTN_SELECT , bSpecialSelect );

	UpdateUnit( eType );
}

void SPWindowChatMacro::UpdateUnit( CHAT_MACRO_TYPE eType )
{
	int	iTypeMaxCount	=	(int)m_vLDTBuffer[ eType ].size();

	if( iTypeMaxCount <= 0 || m_pSlider == NULL )
		return;

	int		iUnitMaxCount	=	0;

	STD_VECTOR_CHAT_MACRO_LDT::iterator		Iter	=	m_vLDTBuffer[ eType ].begin();

	while( Iter != m_vLDTBuffer[ eType ].end() )
	{
		iUnitMaxCount	+=	(*Iter).m_iCount;
		++Iter;
	}

	int	iMaxPage	=	iUnitMaxCount	/	CHAT_MACRO_UNIT_X_COUNT - 1;

	if( iUnitMaxCount % CHAT_MACRO_UNIT_X_COUNT )
		++iMaxPage;

	int	iCurPage	=	m_pSlider->GetCurPage();

	if( iCurPage > iMaxPage )
	{
		iCurPage	=	0;
		m_pSlider->SetCurpage( iCurPage );
	}

	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , iMaxPage );

	bool	bSliderShow	=	false;

	if( iUnitMaxCount > CHAT_MACRO_UNIT_COUNT )
		bSliderShow	=	true;

	SliderShow( bSliderShow );

	CHAT_MACRO*			pChatMacro;
	CHAT_MACRO_LDT*		pChatMacroLDT;
	SPUIChatMacro*		pUIChatMacro;
	int					iStartIndex	=	iCurPage * CHAT_MACRO_UNIT_X_COUNT;

	for( int i = 0 ; i < CHAT_MACRO_UNIT_COUNT ; ++i )
	{
		pUIChatMacro	=	(SPUIChatMacro*)( m_UIUnitManager.GetUnit( i ) );

		if( pUIChatMacro == NULL )
			continue;

		pChatMacro		=	GetMacroData( iStartIndex + i , eType );
		pChatMacroLDT	=	GetMacroLdtData( iStartIndex + i , eType );

		pUIChatMacro->SetChatMacro( pChatMacro );
		pUIChatMacro->SetChatMacroLDT( pChatMacroLDT );

		if( pChatMacro == NULL || pChatMacroLDT == NULL )
			pUIChatMacro->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		else
			pUIChatMacro->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );

		if( pChatMacroLDT != NULL )
			pUIChatMacro->SetResource( pChatMacroLDT->m_iIconFile , pChatMacroLDT->m_iIconIndex );
		else
			pUIChatMacro->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );

		if( pChatMacro != NULL )
			pUIChatMacro->SetText( pChatMacro->m_strName.c_str() );
		else
			pUIChatMacro->SetText( NULL );
	}
}

void SPWindowChatMacro::UpdateSelectIcon( int iSelectNumber )
{
	UINT32		uiInstanceID	=	WIID_CHAT_MACRO_UNIT_OUTLINE + iSelectNumber;
	UINT32		uiMax			=	WIID_CHAT_MACRO_UNIT_OUTLINE + CHAT_MACRO_UNIT_COUNT;
	SPWindow*	pWindow;
	int			iShow;
	int			iUnitShow		=	1;

	SPUIUnit*	pUIUnit			=	m_UIUnitManager.GetUnit( iSelectNumber );

	if( pUIUnit != NULL )
	{
		if( pUIUnit->GetStatus( SPUIUnit::STATUS_UI_UNIT_RENDER ) == FALSE )
			iUnitShow	=	0;
	}

	for( UINT32 ui = WIID_CHAT_MACRO_UNIT_OUTLINE ; ui < uiMax ; ++ui )
	{
		pWindow	=	Find( ui );

		if( pWindow == NULL )
			continue;

		if( uiInstanceID == ui && iUnitShow == 1 )
			iShow	=	1;
		else
			iShow	=	0;

		pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , iShow );
	}

	if( m_iSelectIndex < 0 )
	{
		pWindow	=	Find( WIID_CHAT_MACRO_TITLE_EDIT );

		if( pWindow != NULL )
		{
			pWindow->SetWindowText( NULL );
			pWindow->SetEnable( false );
		}

		pWindow	=	Find( WIID_CHAT_MACRO_CONTENTS_EDIT );

		if( pWindow != NULL )
		{
			pWindow->SetWindowText( NULL );
			pWindow->SetEnable( false );
		}
	}

	if( m_pSlider == NULL )
		return;

	if( iUnitShow == 0 )
	{
		pWindow	=	Find( WIID_CHAT_MACRO_UNIT_OUTLINE + m_iSelectIndex - m_pSlider->GetCurPage() * CHAT_MACRO_UNIT_X_COUNT );

		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );

		return;
	}

	if( iSelectNumber < 0 )
		return;

	CHAT_MACRO_TYPE eType	=	GetMacroType();

	int	iSelectIndex	=	m_pSlider->GetCurPage() * CHAT_MACRO_UNIT_X_COUNT + iSelectNumber;

	CHAT_MACRO*	pChatMacro	=	GetMacroData( iSelectIndex , eType );

	if( pChatMacro == NULL )
		return;

	pWindow	=	Find( WIID_CHAT_MACRO_TITLE_EDIT );

	if( pWindow != NULL )
		pWindow->SetWindowText( pChatMacro->m_strName.c_str() );

	pWindow	=	Find( WIID_CHAT_MACRO_CONTENTS_EDIT );

	if( pWindow != NULL )
		pWindow->SetWindowText( pChatMacro->m_strContents.c_str() );

	CHAT_MACRO_LDT*		pChatMacroLDT	=	GetMacroLdtData( iSelectIndex , eType );

	if( pChatMacroLDT == NULL )
		return;

	const char*		pszGlobalString	=	NULL;
	int				iGlobalString	=	5007205;
	std::string		strItemName;

	if( eType == CHAT_MACRO_TYPE_SPECIAL )
	{
		iGlobalString	=	5007204;

		SPItemAttr* pItem;
		g_pItemCluster->GetItemInfo( pChatMacroLDT->m_iRequireItem , pItem );

		if( pItem != NULL )
			strItemName	=	pItem->m_strName;
	}

	pszGlobalString	=	g_pResourceManager->GetGlobalString( iGlobalString );

	if( pszGlobalString == NULL )
		return;

	char	szBuf[ _MAX_PATH ];

	sprintf( szBuf , pszGlobalString , strItemName.c_str() );

	pWindow	=	Find( WIID_CHAT_MACRO_TIP );

	if( pWindow != NULL )
		pWindow->SetWindowText( szBuf );

	pWindow	=	Find( WIID_CHAT_MACRO_TITLE_EDIT );

	if( pWindow != NULL )
		pWindow->SetEnable( true );

	pWindow	=	Find( WIID_CHAT_MACRO_CONTENTS_EDIT );

	if( pWindow != NULL )
		pWindow->SetEnable( true );

	m_iSelectIndex	=	iSelectIndex;
}

void SPWindowChatMacro::SliderShow( bool bShow )
{
	SPWindow*	pWindow	=	NULL;

	if( bShow == true )
	{
		pWindow	=	Find( WIID_CHAT_MACRO_SLIDER );

		if( pWindow != NULL )
			pWindow->Show();

		pWindow	=	Find( WIID_CHAT_MACRO_SLIDER_UP );

		if( pWindow != NULL )
			pWindow->Show();

		pWindow	=	Find( WIID_CHAT_MACRO_SLIDER_DOWN );

		if( pWindow != NULL )
			pWindow->Show();
	}
	else
	{
		pWindow	=	Find( WIID_CHAT_MACRO_SLIDER );

		if( pWindow != NULL )
			pWindow->Hide();

		pWindow	=	Find( WIID_CHAT_MACRO_SLIDER_UP );

		if( pWindow != NULL )
			pWindow->Hide();

		pWindow	=	Find( WIID_CHAT_MACRO_SLIDER_DOWN );

		if( pWindow != NULL )
			pWindow->Hide();
	}
}

void SPWindowChatMacro::ClearPage()
{
	m_iSelectIndex	=	-1;

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_CHAT_MACRO_TITLE_EDIT );

	if( pWindow != NULL )
		pWindow->SetWindowText( NULL );

	pWindow	=	Find( WIID_CHAT_MACRO_CONTENTS_EDIT );

	if( pWindow != NULL )
		pWindow->SetWindowText( NULL );

	pWindow	=	Find( WIID_CHAT_MACRO_TIP );

	if( pWindow != NULL )
		pWindow->SetWindowText( NULL );

	if( m_pSlider != NULL )
	{
		m_pSlider->SetCurpage( 0 );
		m_pSlider->Refresh();
	}

	UpdateSelectIcon( -1 );

	OnDragnDropEnd( NULL , NULL );
}

CHAT_MACRO_TYPE SPWindowChatMacro::GetMacroType()
{
	CHAT_MACRO_TYPE eType	=	CHAT_MACRO_TYPE_NORMAL;

	SPWindowButton*	pButton	=	(SPWindowButton*)Find( WIID_CHAT_MACRO_NORMAL );

	if( pButton != NULL )
	{
		if( pButton->IsChecked() == false )
			eType	=	CHAT_MACRO_TYPE_SPECIAL;
	}
	else
	{
		pButton	=	(SPWindowButton*)Find( WIID_CHAT_MACRO_SPECIAL );

		if( pButton != NULL )
		{
			if( pButton->IsChecked() == true )
				eType	=	CHAT_MACRO_TYPE_SPECIAL;
		}
	}

	return eType;
}

void SPWindowChatMacro::AddMacroData( CHAT_MACRO_TYPE eType , CHAT_MACRO& stChatMacro )
{
	m_vFileBuffer[ eType ].push_back( stChatMacro );
	m_vMemBuffer[ eType ].push_back( stChatMacro );
}

CHAT_MACRO* SPWindowChatMacro::GetMacroData( int iIndex , CHAT_MACRO_TYPE eType )
{
	if( iIndex < 0 )
		return NULL;

	STD_VECTOR_CHAT_MACRO*	pBuffer	=	&m_vMemBuffer[ eType ];

	if( pBuffer == NULL )
		return NULL;

	int	iCount	=	(int)pBuffer->size();

	if( iCount <= iIndex )
		return NULL;

	return &(pBuffer->at( iIndex ));
}

CHAT_MACRO_LDT* SPWindowChatMacro::GetMacroLdtData( int iIndex , CHAT_MACRO_TYPE eType )
{
	if( iIndex < 0 )
		return NULL;

	STD_VECTOR_CHAT_MACRO_LDT*	pBuffer	=	&m_vLDTBuffer[ eType ];

	if( pBuffer == NULL )
		return NULL;

	STD_VECTOR_CHAT_MACRO_LDT::iterator	Iter	=	pBuffer->begin();

	int		iMaxCount	=	0;

	while( Iter != pBuffer->end() )
	{
		iMaxCount	+=	(*Iter).m_iCount;

		if( iIndex < iMaxCount )
			return &(*Iter);

		++Iter;
	}

	return NULL;
}

void SPWindowChatMacro::SaveMacroDataMem( int iIndex , CHAT_MACRO_TYPE eType , const char* pszName , const char* pszContents )
{
	CHAT_MACRO*	pChatMacro	=	GetMacroData( iIndex , eType );

	if( pChatMacro == NULL )
		return;

	if( pszName == NULL )
		pChatMacro->m_strName.clear();
	else
		pChatMacro->m_strName		=	pszName;

	if( pszContents == NULL )
		pChatMacro->m_strContents.clear();
	else
		pChatMacro->m_strContents	=	pszContents;
}

void SPWindowChatMacro::MacroDataMemToFile()
{
	for( int i = 0 ; i < CHAT_MACRO_TYPE_COUNT ; ++i )
	{
		m_vFileBuffer[ i ].clear();
		m_vFileBuffer[ i ]	=	m_vMemBuffer[ i ];
	}
}

void SPWindowChatMacro::MacroDataFileToMem()
{
	for( int i = 0 ; i < CHAT_MACRO_TYPE_COUNT ; ++i )
	{
		m_vMemBuffer[ i ].clear();
		m_vMemBuffer[ i ]	=	m_vFileBuffer[ i ];
	}
}

void SPWindowChatMacro::LoadMacroFile()
{
	CHAT_MACRO_LDT*	pChatMacroLDT;
	CHAT_MACRO		stChatMacro;

	TCHAR			szFullPathFileName[ _MAX_PATH ];
	::GetModuleFileName( NULL , szFullPathFileName , _MAX_PATH );

	std::string		strFindFileName	=	szFullPathFileName;
	int				iFindIndex		=	(int)strFindFileName.find_last_of( "\\" );
	std::string		strFileName		=	strFindFileName.substr( 0 , iFindIndex + 1 );

	strFileName		+=	g_pNetworkManager->GetLoginID();
	strFileName		+=	".MAC";

	std::string		strKeyName;
	std::string		strKeyContents;

	TCHAR			szDataName[ MAX_PATH ];
	TCHAR			szDataContents[ MAX_PATH ];

	TCHAR			szKeyName[ MAX_PATH ];

	DWORD			dwNameLength		=	5;
	DWORD			dwContentsLength	=	61;

	STD_VECTOR_CHAT_MACRO_LDT::iterator	Iter;

	int				iIndex;

	for( int i = 0 ; i < CHAT_MACRO_TYPE_COUNT ; ++i )
	{
		m_vFileBuffer[ i ].clear();

		Iter	=	m_vLDTBuffer[ i ].begin();
		iIndex	=	0;

		while( Iter != m_vLDTBuffer[ i ].end() )
		{
			pChatMacroLDT	=	(CHAT_MACRO_LDT*)(&(*Iter));

			if( pChatMacroLDT == NULL )
			{
				++Iter;
				continue;

			}

			for( int j = 0 ; j < pChatMacroLDT->m_iCount ; ++j )
			{
				sprintf( szKeyName , pChatMacroLDT->m_strKeyName.c_str() , j + 1 );

				strKeyName		=	szKeyName;
				strKeyContents	=	szKeyName;

				strKeyName		+=	"_Name";
				strKeyContents	+=	"_Macro";

				GetPrivateProfileString( pChatMacroLDT->m_strAppName.c_str() , strKeyName.c_str()		, "" , szDataName		, dwNameLength		, strFileName.c_str() );
				GetPrivateProfileString( pChatMacroLDT->m_strAppName.c_str() , strKeyContents.c_str()	, "" , szDataContents	, dwContentsLength	, strFileName.c_str() );

				stChatMacro.m_iIndex		=	j	+	iIndex;
				stChatMacro.m_strName		=	szDataName;
				stChatMacro.m_strContents	=	szDataContents;

				FilteringMacroData( stChatMacro );

				AddMacroData( (CHAT_MACRO_TYPE)i , stChatMacro );

				stChatMacro.m_strName.clear();
				stChatMacro.m_strContents.clear();
			}

			iIndex	+=	pChatMacroLDT->m_iCount;
			++Iter;
		}
	}
}

void SPWindowChatMacro::SaveMacroFile()
{
	CHAT_MACRO_LDT*	pChatMacroLDT;
	CHAT_MACRO*		pChatMacro;

	TCHAR			szFullPathFileName[ _MAX_PATH ];
	::GetModuleFileName( NULL , szFullPathFileName , _MAX_PATH );

	std::string		strFindFileName	=	szFullPathFileName;
	int				iFindIndex		=	(int)strFindFileName.find_last_of( "\\" );
	std::string		strFileName		=	strFindFileName.substr( 0 , iFindIndex + 1 );

	strFileName		+=	g_pNetworkManager->GetLoginID();
	strFileName		+=	".MAC";

	std::string		strKeyName;
	std::string		strKeyContents;

	TCHAR			szKeyName[ MAX_PATH ];

	STD_VECTOR_CHAT_MACRO_LDT::iterator	Iter;

	int				iIndex;

	bool			bSaveError	=	false;

	for( int i = 0 ; i < CHAT_MACRO_TYPE_COUNT ; ++i )
	{
		m_vFileBuffer[ i ].clear();

		Iter	=	m_vLDTBuffer[ i ].begin();
		iIndex	=	0;

		while( Iter != m_vLDTBuffer[ i ].end() )
		{
			pChatMacroLDT	=	(CHAT_MACRO_LDT*)(&(*Iter));

			if( pChatMacroLDT == NULL )
			{
				++Iter;
				continue;

			}

			for( int j = 0 ; j < pChatMacroLDT->m_iCount ; ++j )
			{
				pChatMacro	=	GetMacroData( j + iIndex , (CHAT_MACRO_TYPE)i );

				if( pChatMacro == NULL )
					continue;

				sprintf( szKeyName , pChatMacroLDT->m_strKeyName.c_str() , j + 1 );

				strKeyName		=	szKeyName;
				strKeyContents	=	szKeyName;

				strKeyName		+=	"_Name";
				strKeyContents	+=	"_Macro";

				FilteringMacroData( *pChatMacro );

				if( pChatMacro->m_strName.empty() == false )
				{
					if( WritePrivateProfileString( pChatMacroLDT->m_strAppName.c_str() , strKeyName.c_str()		, pChatMacro->m_strName.c_str()		, strFileName.c_str() ) == FALSE )
					{
						bSaveError	=	true;
					}
				}
				
				if( pChatMacro->m_strContents.empty() == false )
				{
					if( WritePrivateProfileString( pChatMacroLDT->m_strAppName.c_str() , strKeyContents.c_str()	, pChatMacro->m_strContents.c_str()	, strFileName.c_str() ) == FALSE )
					{
						bSaveError	=	true;
					}
				}
			}

			iIndex	+=	pChatMacroLDT->m_iCount;
			++Iter;
		}
	}

	if( bSaveError == true )
	{
		g_pInterfaceManager->SetNotice( g_pResourceManager->GetGlobalString( 5007201 ) , this , NOTICE_TYPE_YES );
	}
}

void SPWindowChatMacro::FilteringMacroData( CHAT_MACRO& stChatMacro )
{
	char szBuffer[ 1025 ];

	SPAbuseFilter*	pAbuseFilter	=	SPAbuseFilter::GetInstance();

	if( pAbuseFilter == NULL )
		return;

	if( stChatMacro.m_strName.empty() == false )
	{
		pAbuseFilter->Filter( stChatMacro.m_strName.c_str() , szBuffer , 1024 );
		stChatMacro.m_strName	=	szBuffer;
	}

	if( stChatMacro.m_strContents.empty() == false )
	{
		pAbuseFilter->Filter( stChatMacro.m_strContents.c_str() , szBuffer , 1024 );
		stChatMacro.m_strContents	=	szBuffer;
	}
}

void SPWindowChatMacro::ProcessEditCtrl( float fTime )
{
	SPIMEPool*	pIMEPool	=	g_pInterfaceManager->GetIMEPool();

	if( pIMEPool == NULL )
		return;

	if( pIMEPool->IsUpdate() == true )
	{
		UpdateUnit( GetMacroType() );

		if( m_iSelectIndex >= 0 && m_pSlider != NULL )
			UpdateSelectIcon( m_iSelectIndex - m_pSlider->GetCurPage() * CHAT_MACRO_UNIT_X_COUNT );
	}
}

void SPWindowChatMacro::ReFreshQuickSlot()
{
	CHAT_MACRO_POS	stChatMacroPos;

	stChatMacroPos.m_eType	=	GetMacroType();
	stChatMacroPos.m_iIndex	=	m_iSelectIndex;

	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_QUICK_SLOT_UPDATE , (WPARAM)-2 , (LPARAM)(&stChatMacroPos) );

	//int	iUnitNumber	=	m_iSelectIndex - m_pSlider->GetCurPage() * CHAT_MACRO_UNIT_X_COUNT;

	//SPUIChatMacro*	pUIChatMacroUnit	=	(SPUIChatMacro*)( m_UIUnitManager.GetUnit( iUnitNumber ) );

	//if( pUIChatMacroUnit == NULL )
	//	return;

	//CHAT_MACRO_LDT*		pChatMacroLDT	=	pUIChatMacroUnit->GetChatMacroLDT();

	//if( pChatMacroLDT == NULL )
	//	return;

	//g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_QUICK_SLOT_UPDATE , (WPARAM)0 , (LPARAM)pChatMacroLDT->m_iRequireItem );
}
