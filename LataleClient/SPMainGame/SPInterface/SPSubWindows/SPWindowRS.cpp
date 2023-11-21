
//#ifdef _INCLUDE_RSCLIENT_

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowRS.h"

#include "SPWindowListDEF.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowRSList.h"
#include "SPWindowRSStatic.h"
#include "SPWindowSlider.h"

#include <WinSock2.h>
#include "PacketID.h"
#include "Packet.h"
#include "PacketHandler.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPRsManager.h"
#include "SPLocalizeManager.h"

#include "_Error_log.h"

//------------------------------------------------------------------------------------
SPWindowRS::SPWindowRS(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowRS::SPWindowRS(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_RSCLIENT, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pkSkinWindow(NULL)
, m_pkHotWindow(NULL)
, m_pkAnsWindow(NULL)
, m_pkJudgeWindow(NULL)
, m_pkAnsList(NULL)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowRS::~SPWindowRS()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowRS::Clean()
{
	SPWindow::Clean();
}

//------------------------------------------------------------------------------------
void SPWindowRS::Init()
{
	// Not china return ;
	if( SPLocalizeManager::GetInstance()->GetCountryType() != CT_CHINA )
		return ;

	m_eCurMode = RS_SHOW_WAIT ;

	SPWindowStatic* pkSkinParent = NULL ;
	SPWindowStatic* pkSkinWindow = NULL ;
	SPWindowStatic *pkChildWindow = NULL ;
	SPWindowButton* pkButton = NULL ;

	pkSkinParent	=	new SPWindowStatic( WIID_RS_SKIN_PARENT , 0 , 0 , 316 , 342 , this );
	pkSkinParent->SetWindowType(WND_TYPE_NINEBASE) ;
	pkSkinParent->SetImageNine( "DATA/INTERFACE/CONCEPT/UI105.PNG", 369, 251, 4, 1) ;
	m_pkSkinWindow = pkSkinWindow ;

	pkChildWindow = new SPWindowStatic(WIID_RS_SKIN_SUBBG, 2, 27, 312, 311, pkSkinParent);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI105.PNG", 487, 306);
	pkChildWindow->SetSrcSize(4, 4) ;

	pkSkinWindow	=	new SPWindowStatic( WIID_RS_SKIN_BG , -2 , -27 , 316 , 342 , pkChildWindow );
	m_pkSkinWindow = pkSkinWindow ;

	pkChildWindow = new SPWindowStatic(WIID_RS_TITLE_IMG, 10, 2, 172, 23, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 339, 394);
	pkChildWindow = NULL ;
	
	pkButton = new SPWindowButton(WIID_RS_EXIT, 295, 9, 12, 12, pkSkinWindow);
	pkButton->SetImage		 ("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 217);
	pkButton->SetImageHit	 ("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 230);
	pkButton->SetImagePush	 ("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 243);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 256);

	pkChildWindow = new SPWindowStatic(WIID_RS_SKIN_LINE, 2, 26, 312, 1, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 335, 510);
	pkChildWindow->SetSrcSize(2, 1) ;
	pkChildWindow = NULL ;

	pkButton = new SPWindowButton(WIID_RS_TAB_MENU_01, 10, 35, 96, 15, pkSkinWindow);
	pkButton->SetFormat(DT_VCENTER | DT_CENTER) ;
	pkButton->SetImage		 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 448);
	pkButton->SetImageHit	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 464);
	pkButton->SetImagePush	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 480);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 496);
	pkButton = NULL ;

	pkButton = new SPWindowButton(WIID_RS_TAB_MENU_02, 110, 35, 96, 15, pkSkinWindow);
	pkButton->SetFormat(DT_VCENTER | DT_CENTER) ;
	pkButton->SetImage		 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 448);
	pkButton->SetImageHit	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 464);
	pkButton->SetImagePush	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 480);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 496);
	pkButton = NULL ;

	pkButton = new SPWindowButton(WIID_RS_TAB_MENU_03, 210, 35, 96, 15, pkSkinWindow);
	pkButton->SetFormat(DT_VCENTER | DT_CENTER) ;
	pkButton->SetImage		 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 448);
	pkButton->SetImageHit	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 464);
	pkButton->SetImagePush	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 480);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 496);
	pkButton = NULL ;

	pkButton = new SPWindowButton(WIID_RS_TAB_MENU_04, 10, 54, 96, 15, pkSkinWindow);
	pkButton->SetFormat(DT_VCENTER | DT_CENTER) ;
	pkButton->SetImage		 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 448);
	pkButton->SetImageHit	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 464);
	pkButton->SetImagePush	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 480);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 496);
	pkButton = NULL ;

	pkButton = new SPWindowButton(WIID_RS_TAB_MENU_05, 110, 54, 96, 15, pkSkinWindow);
	pkButton->SetFormat(DT_VCENTER | DT_CENTER) ;
	pkButton->SetImage		 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 448);
	pkButton->SetImageHit	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 464);
	pkButton->SetImagePush	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 480);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 496);
	pkButton = NULL ;

	InitHotWindow() ;
	InitAnswerWindow() ;
	InitJudgeWindow() ;
}


/**
	HotQuesetion Menu
 */
void SPWindowRS::InitHotWindow()
{
	SPWindowStatic* pkSkinWindow = NULL ;
	SPWindowStatic* pkChildWindow = NULL ;
	SPWindowRSStatic* pkRSChildWindow = NULL ;
	SPWindowButton* pkButton = NULL ;
	SPWindowEdit*	pkEdit = NULL ;

	pkSkinWindow	=	new SPWindowStatic( WIID_RS_WND_HOTMAIN , 2 , 69 , 312 , 269 , m_pkSkinWindow );
	m_pkHotWindow = pkSkinWindow ;

	pkChildWindow = new SPWindowStatic(WIID_RS_HOT_ICON, 16, 12, 35, 29, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 440, 336);		// Main 0 
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 476, 336);		// Sub 1
	pkChildWindow = NULL ;

	pkChildWindow = new SPWindowStatic(WIID_RS_HOT_TITLE, 59, 20, 237, 14, pkSkinWindow);
	pkChildWindow->SetFont( FONT_14_BOLD ) ;
	pkChildWindow->SetFormat(DT_TOP | DT_LEFT) ;
	pkChildWindow->SetFontColor(RGBA(49, 76, 97, 255));
	pkChildWindow->SetWindowText("Hot Title") ;
	pkChildWindow = NULL ;

	RECT rcBar ;
	SetRect(&rcBar, 231, 1, 511, 16) ;
	for( int i = 0 ; i < MAX_RS_URL_SIZE; i++ )
	{
		pkRSChildWindow = new SPWindowRSStatic(WIID_RS_HOT_TEXT_URL+i, 42, 52+i*17, 250, 12, pkSkinWindow);
		pkRSChildWindow->SetFont( FONT_12_NORMAL ) ;
		pkRSChildWindow->SetFormat(DT_TOP | DT_LEFT) ;
		pkRSChildWindow->SetFontColor(RGBA(49, 76, 97, 255));
		pkRSChildWindow->SetSelectFontcolor(false) ;										// CursorOn Font Change 사용한함
		pkRSChildWindow->SetSelectBar("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", rcBar, -26, -2) ;	// CursorOn BG 사용
		pkRSChildWindow = NULL ;

		pkRSChildWindow = new SPWindowRSStatic(WIID_RS_HOT_ICON_URL+i, 22, 50+i*17, 14, 14, pkSkinWindow);
		pkRSChildWindow->SetImage("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 324, 433);		// Main 0 
		pkRSChildWindow->SetImage("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 309, 433);		// Sub 1
		pkRSChildWindow = NULL ;
	}

	pkChildWindow = new SPWindowStatic(WIID_RS_QUESTION_TITLE, 8, 229, 297, 12, pkSkinWindow);
	pkChildWindow->SetFont( FONT_12_NORMAL ) ;
	pkChildWindow->SetFormat(DT_TOP | DT_LEFT) ;
	pkChildWindow->SetFontColor( RGBA(49, 76, 97, 255));
	pkChildWindow = NULL ;

	//---------------------------------------------------------------------
	pkChildWindow = new SPWindowStatic(WIID_RS_QUESTION_BG, 8, 246, 216, 17, pkSkinWindow);
	pkChildWindow->SetWindowType(WND_TYPE_LEFTBASE) ;
	pkChildWindow->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 309, 415, 311, 432);
	pkChildWindow->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 312, 415, 314, 432);
	pkChildWindow->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 315, 415, 317, 432);

	pkEdit = new SPWindowEdit(WIID_RS_QUESTION_EDIT, 5, 3, 206, 12, pkChildWindow) ;	// BG Child
	pkEdit->SetFontColor( RGBA(49, 76, 97, 255) );
	pkEdit->SetFormat(DT_TOP | DT_LEFT);
	pkEdit->SetEnableTabStop(true) ;
	pkEdit = NULL ;

	pkButton = new SPWindowButton(WIID_RS_QUESTION_BTN, 228, 246, 76, 17, pkSkinWindow);
	pkButton->SetImage		 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 376);
	pkButton->SetImageHit	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 394);
	pkButton->SetImagePush	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 412);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 1, 430);
	pkButton = NULL ;
}


/**
	MasterAnswer & OtherAnswer Menu
 */
void SPWindowRS::InitAnswerWindow()
{
	SPWindowStatic* pkSkinWindow = NULL ;
	SPWindowStatic *pkChildWindow = NULL ;
	SPWindowButton* pkButton = NULL ;
	SPWindowSlider* pkSlider = NULL ;

	pkSkinWindow	=	new SPWindowStatic( WIID_RS_WND_MASTERANS , 2 , 69 , 312 , 269 , m_pkSkinWindow );
	m_pkAnsWindow = pkSkinWindow ;

	pkChildWindow = new SPWindowStatic(WIID_RS_MSTANS_ICON, 22, 8, 14, 14, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 309, 433);
	pkChildWindow = NULL ;

	pkChildWindow = new SPWindowStatic(WIID_RS_MSTANS_TITLE, 41, 8, 260, 14, pkSkinWindow);
	pkChildWindow->SetFont( FONT_14_BOLD ) ;
	pkChildWindow->SetFontColor( RGBA(49, 76, 97, 255) );
	pkChildWindow->SetFormat(DT_TOP | DT_LEFT);
	pkChildWindow = NULL ;

	pkChildWindow = new SPWindowStatic(WIID_RS_MSTANS_BRACKET_L, 12, 29, 4, 77, pkSkinWindow);
	pkChildWindow->SetWindowType(WND_TYPE_TOPBASE) ;
	pkChildWindow->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 318, 415, 322, 419);
	pkChildWindow->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 318, 421, 322, 425);
	pkChildWindow->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 318, 428, 322, 432);
	pkChildWindow = NULL ;

	pkChildWindow = new SPWindowStatic(WIID_RS_MSTANS_BRACKET_R, 300, 29, 4, 77, pkSkinWindow);
	pkChildWindow->SetWindowType(WND_TYPE_TOPBASE) ;
	pkChildWindow->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 323, 415, 327, 419);
	pkChildWindow->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 323, 421, 327, 425);
	pkChildWindow->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 323, 428, 327, 432);
	pkChildWindow = NULL ;

	//pkChildWindow = new SPWindowStatic(WIID_RS_MSTANS_TEXT, 27, 31, 257, 73, pkSkinWindow);
	//pkChildWindow->SetFontColor( RGBA(49, 76, 97, 255) );
	//pkChildWindow->SetFormat(DT_TOP | DT_LEFT | DT_WORDBREAK);
	//pkChildWindow = NULL ;

	//m_pkMasterAns = new SPWindowRSList( WIID_RS_MSTANS_TEXT , 27, 31, 257, 73 , pkSkinWindow );
	m_pkMasterAns = new SPWindowRSList( WIID_RS_MSTANS_TEXT , 21, 31, 257, 73 , pkSkinWindow );
	m_pkMasterAns->SetFontShadow( false );
	m_pkMasterAns->SetFontColor( RGBA( 49, 76, 97, 255 ) );
	m_pkMasterAns->SetMargin( 0 , 0 , 0 , 0 );
	m_pkMasterAns->SetMultiLine( TRUE , 2 );
	m_pkMasterAns->SetWheelUse( true );
	m_pkMasterAns->SetSelectFontcolor(false) ;

	//////////////////////////////////////////////////////////////////////////
	// MasterAnswer Slider
	pkButton	=	new	SPWindowButton( WIID_RS_MSTANS_LIST_UP , 284 , 29 , 12 , 11 , pkSkinWindow );
	pkButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	pkButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	pkButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	pkButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );
	pkButton =	NULL;

	pkButton	=	new	SPWindowButton( WIID_RS_MSTANS_LIST_DOWN , 284 , 95 , 12 , 11 , pkSkinWindow );
	pkButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	pkButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	pkButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	pkButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );
	pkButton =	NULL;

	//RECT rtRect;
	//pkSlider	=	new	SPWindowSlider( WIID_RS_OTHANS_LIST_SLIDER , 284 , 148 , 12 , 58 , pkSkinWindow );
	//SetRect( &rtRect , 460 , 470 , 472 , 494 );			//	일반
	//pkSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	//SetRect( &rtRect , 473 , 470 , 485 , 494 );			//	마우스 오버
	//pkSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	//SetRect( &rtRect , 486 , 470 , 498 , 494 );			//	마우스 클릭
	//pkSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	//SetRect( &rtRect , 499 , 470 , 511 , 494 );
	//pkSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	//SetRect( &rtRect , 460 , 496 , 472 , 498 );			//	일반
	//pkSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	//SetRect( &rtRect , 473 , 496 , 485 , 498 );			//	마우스 오버
	//pkSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	//SetRect( &rtRect , 486 , 496 , 498 , 498 );			//	마우스 클릭
	//pkSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	//SetRect( &rtRect , 499 , 496 , 511 , 498 );
	//pkSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	//pkSlider->SetWheelMouseUse(true) ;
	//pkSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	//pkSlider->SetCurpage( 0 );
	//pkSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );

	//---------------------------------------------------------------------
	pkChildWindow = new SPWindowStatic(WIID_RS_OTHANS_ICON, 20, 114, 172, 14, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 339, 379);
	pkChildWindow = NULL ;

	pkChildWindow = new SPWindowStatic(WIID_RS_OTHANS_LINE_T, 8, 132, 296, 1, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 329, 431);
	pkChildWindow->SetSrcSize(8, 1) ;
	pkChildWindow = NULL ;

	pkChildWindow = new SPWindowStatic(WIID_RS_OTHANS_LINE_B, 8, 221, 296, 1, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 329, 431);
	pkChildWindow->SetSrcSize(8, 1) ;
	pkChildWindow = NULL ;

	// Here OtherAns List
	RECT rcBar ;
	SetRect(&rcBar, 251, 17, 511, 32) ;
	m_pkAnsList = new SPWindowRSList( WIID_RS_OTHANS_LIST , 22 , 142 , 254 , 75 , pkSkinWindow );
	m_pkAnsList->SetFontShadow( false );
	m_pkAnsList->SetFontColor( RGBA( 49, 76, 97, 255 ) );
	m_pkAnsList->SetMargin( 0 , 0 , 0 , 0 );
	m_pkAnsList->SetMultiLine( TRUE , 2 );
	m_pkAnsList->SetWheelUse( true );
	m_pkAnsList->SetSelectFontcolor(false) ;										// CursorOn Font Change 사용한함
	m_pkAnsList->SetSelectBar("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", rcBar, -6, -2) ;	// CursorOn BG 사용

	//////////////////////////////////////////////////////////////////////////
	// OtherAnswer Slider
	pkButton	=	new	SPWindowButton( WIID_RS_OTHANS_LIST_UP , 284 , 137 , 12 , 11 , pkSkinWindow );
	pkButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	pkButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	pkButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	pkButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );
	pkButton =	NULL;

	pkButton	=	new	SPWindowButton( WIID_RS_OTHANS_LIST_DOWN , 284 , 206 , 12 , 11 , pkSkinWindow );
	pkButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	pkButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	pkButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	pkButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );
	pkButton =	NULL;

	RECT rtRect;
	pkSlider	=	new	SPWindowSlider( WIID_RS_OTHANS_LIST_SLIDER , 284 , 148 , 12 , 58 , pkSkinWindow );
	SetRect( &rtRect , 460 , 470 , 472 , 494 );			//	일반
	pkSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 473 , 470 , 485 , 494 );			//	마우스 오버
	pkSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 486 , 470 , 498 , 494 );			//	마우스 클릭
	pkSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 499 , 470 , 511 , 494 );
	pkSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 460 , 496 , 472 , 498 );			//	일반
	pkSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 473 , 496 , 485 , 498 );			//	마우스 오버
	pkSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 486 , 496 , 498 , 498 );			//	마우스 클릭
	pkSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
	SetRect( &rtRect , 499 , 496 , 511 , 498 );
	pkSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	pkSlider->SetWheelMouseUse(true) ;
	pkSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	pkSlider->SetCurpage( 0 );
	pkSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );


	pkChildWindow = new SPWindowStatic(WIID_RS_OTHANS_JUDGE_TEXT, 26, 230, 172, 12, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 339, 366);
	pkChildWindow = NULL ;

	pkButton = new SPWindowButton(WIID_RS_OTHANS_JUDGE_YES, 16, 246, 76, 17, pkSkinWindow);
	pkButton->SetImage		 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 78, 376);
	pkButton->SetImageHit	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 78, 394);
	pkButton->SetImagePush	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 78, 412);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 78, 430);
	pkButton = NULL ;

	pkButton = new SPWindowButton(WIID_RS_OTHANS_JUDGE_NO, 100, 246, 76, 17, pkSkinWindow);
	pkButton->SetImage		 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 155, 376);
	pkButton->SetImageHit	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 155, 394);
	pkButton->SetImagePush	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 155, 412);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 155, 430);
	pkButton = NULL ;

}


/**
	JudgeAnswer Menu
 */
void SPWindowRS::InitJudgeWindow()
{
	SPWindowStatic* pkSkinWindow = NULL ;
	SPWindowStatic *pkChildWindow = NULL ;
	SPWindowButton* pkButton = NULL ;
	SPWindowEdit*	pkEdit = NULL ;

	pkSkinWindow	=	new SPWindowStatic( WIID_RS_WND_JUDGEANS , 2 , 69 , 312 , 269 , m_pkSkinWindow );
	m_pkJudgeWindow = pkSkinWindow ;

	pkChildWindow = new SPWindowStatic(WIID_RS_JUDGE_ICON, 16, 20, 172, 29, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 339, 418);
	pkChildWindow = NULL ;

	for( int i = 0 ; i < MAX_RS_RADIO_SIZE-1; i++ )
	{
		pkButton = new SPWindowButton(WIID_RS_JUDGE_RADIO+i, 24, 60+i*17, 10, 10, pkSkinWindow);
		pkButton->SetImage		 ("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 164);
		pkButton->SetImageHit	 ("DATA/INTERFACE/CONCEPT/UI102.PNG", 87, 164);
		pkButton->SetImagePush	 ("DATA/INTERFACE/CONCEPT/UI102.PNG", 98, 164);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 109, 164);
		pkButton->SetImageCheck	("DATA/INTERFACE/CONCEPT/UI102.PNG", 98, 164);
		pkButton = NULL ;

		pkChildWindow = new SPWindowStatic(WIID_RS_JUDGE_TEXT+i, 42, 60+i*17, 249, 12, pkSkinWindow);
		pkChildWindow->SetFontColor( RGBA(49, 76, 97, 255) );
		pkChildWindow->SetFormat(DT_TOP | DT_LEFT);
		pkChildWindow = NULL ;
	}

	pkButton = new SPWindowButton(WIID_RS_JUDGE_RADIO+MAX_RS_RADIO_SIZE-1, 24, 200, 10, 10, pkSkinWindow);
	pkButton->SetImage		 ("DATA/INTERFACE/CONCEPT/UI102.PNG", 76, 164);
	pkButton->SetImageHit	 ("DATA/INTERFACE/CONCEPT/UI102.PNG", 87, 164);
	pkButton->SetImagePush	 ("DATA/INTERFACE/CONCEPT/UI102.PNG", 98, 164);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 109, 164);
	pkButton->SetImageCheck	("DATA/INTERFACE/CONCEPT/UI102.PNG", 98, 164);
	pkButton = NULL ;

	//---------------------------------------------------------------------
	pkChildWindow = new SPWindowStatic(WIID_RS_JUDGE_EDIT_BG, 38, 197, 250, 17, pkSkinWindow);
	pkChildWindow->SetWindowType(WND_TYPE_LEFTBASE) ;
	pkChildWindow->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 309, 415, 311, 432);
	pkChildWindow->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 312, 415, 314, 432);
	pkChildWindow->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 315, 415, 317, 432);

	pkEdit = new SPWindowEdit(WIID_RS_JUDGE_EDIT, 4, 3, 241, 12, pkChildWindow) ;	// BG Child
	pkEdit->SetFontColor( RGBA(49, 76, 97, 255) );
	pkEdit->SetFormat(DT_TOP | DT_LEFT);
	pkEdit->SetEnableTabStop(true) ;
	pkEdit = NULL ;

	pkButton = new SPWindowButton(WIID_RS_JUDGE_BTN, 117, 230, 76, 17, pkSkinWindow);
	pkButton->SetImage		 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 232, 376);
	pkButton->SetImageHit	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 232, 394);
	pkButton->SetImagePush	 ("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 232, 412);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CN_UI100.PNG", 232, 430);
	pkButton = NULL ;

}


//------------------------------------------------------------------------------------
void SPWindowRS::Show(bool bWithChild)
{
	// Not china return ;
	if( SPLocalizeManager::GetInstance()->GetCountryType() != CT_CHINA )
		return ;

	m_iJudgeIndex = 0 ;

	g_pkRsManager->SendQMainMenu() ;
	g_pInterfaceManager->SetFocusWindow(this) ;

	m_eCurMode = RS_SHOW_HOTTOPIC ;

	//---------------------------------------------------------------------
	// OnRecvMainMenu() then ::Show()
	//---------------------------------------------------------------------
}

//------------------------------------------------------------------------------------
void SPWindowRS::Render(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Render(fTime);

}


//------------------------------------------------------------------------------------
void SPWindowRS::AddMasterAnswer(const char* pAddMesg)
{
	if( m_pkAnsList )
	{
		RSLIST_ITEM	stAddListItem;

		stAddListItem.m_stReturnValue.m_iReturnValue = 1 ;
		stAddListItem.m_stReturnValue.m_strReturnValue = "" ;

		stAddListItem.m_strText		=	pAddMesg;
		stAddListItem.m_bUseFontColor	= false ;
		stAddListItem.m_DefTextColor	=	RGBA(49, 76, 97, 255);
		stAddListItem.m_RvsTextColor	=	RGBA(255, 6, 255, 255);

		m_pkMasterAns->InsertItem( &stAddListItem );
	}
}


//------------------------------------------------------------------------------------
void SPWindowRS::AddOtherAnswer(const char* pAddMesg, int iRetValue /*= 0*/, const char* pRetStr /*= NULL*/)
{
	if( m_pkAnsList )
	{
		RSLIST_ITEM	stAddListItem;

		stAddListItem.m_stReturnValue.m_iReturnValue = iRetValue ;
		if( pRetStr != NULL )
			stAddListItem.m_stReturnValue.m_strReturnValue = pRetStr ;

		stAddListItem.m_strText		=	pAddMesg;
		stAddListItem.m_bUseFontColor	= false ;
		stAddListItem.m_DefTextColor	=	RGBA(49, 76, 97, 255);
		stAddListItem.m_RvsTextColor	=	RGBA(255, 6, 255, 255);

		m_pkAnsList->InsertItem( &stAddListItem );
	}
}

void SPWindowRS::ClearAnswerWindow()
{
	SPWindowStatic* pkStatic = NULL ;

	// Master Answer Clear
	if( m_pkMasterAns )
		m_pkMasterAns->SPSendMessage( SPIM_PURGE );

	// Other Answer Clear
	if( m_pkAnsList )
		m_pkAnsList->SPSendMessage( SPIM_PURGE );

	// Scroll Clear
	SPWindowSlider* pSlider = NULL ;
	pSlider = (SPWindowSlider*)Find(WIID_RS_OTHANS_LIST_SLIDER, true) ;
	if( pSlider )	{
		pSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );
	}
}

void SPWindowRS::ClearJudgeWindow()
{
	SPWindowButton* pButton = NULL ;
	SPWindowEdit* pEdit = NULL ;

	// Selection Clear
	pButton = (SPWindowButton*)Find(WIID_RS_JUDGE_RADIO+m_iJudgeIndex, true) ;
	if( pButton ) {
		pButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	}
	pButton = (SPWindowButton*)Find(WIID_RS_JUDGE_RADIO, true) ;
	if( pButton )	{
		pButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
	}
	if( pEdit ) {
		pEdit->SetWindowText("") ;			// Clear
		pEdit->SetEnable(false) ;
	}
	m_iJudgeIndex = 0 ;
}






//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowRS)

SPIMESSAGE_COMMAND(	SPIM_SDRS_RECV_MAINMENU		,	OnRecvMainMenu	)
SPIMESSAGE_COMMAND(	SPIM_SDRS_RECV_SUBMENU		,	OnRecvSubMenu	)
SPIMESSAGE_COMMAND(	SPIM_SDRS_RECV_TOPTOPIC		,	OnRecvTopTopic	)
SPIMESSAGE_COMMAND(	SPIM_SDRS_RECV_ANSWER		,	OnRecvAnswer	)
SPIMESSAGE_COMMAND(	SPIM_SDRS_RECV_UNSATREASON	,	OnRecvUnSatReason	)

SPIMESSAGE_CONTROL		( SPIM_LBUTTON_UP	, WIID_RS_EXIT										, OnExit			)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	, WIID_RS_TAB_MENU_01	, WIID_RS_TAB_MENU_05		, OnTabMemuSelect	)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	, WIID_RS_HOT_TEXT_URL	, WIID_RS_HOT_TEXT_URL+MAX_RS_URL_SIZE	, OnHotURLClick	)
SPIMESSAGE_CONTROL		( SPIM_LBUTTON_UP	, WIID_RS_QUESTION_BTN								, OnSendQuestion	)

SPIMESSAGE_CONTROL		(	SPIM_SETFOCUS	, WIID_RS_QUESTION_EDIT				,	OnEditSetFocus		)
SPIMESSAGE_CONTROL		(	SPIM_KILLFOCUS	, WIID_RS_QUESTION_EDIT				,	OnEditKillFocus		)
SPIMESSAGE_CONTROL		(	SPIM_SETFOCUS	, WIID_RS_JUDGE_EDIT				,	OnEditSetFocus		)
SPIMESSAGE_CONTROL		(	SPIM_KILLFOCUS	, WIID_RS_JUDGE_EDIT				,	OnEditKillFocus		)

SPIMESSAGE_CONTROL		( SPIM_LBUTTON_UP	, WIID_RS_MSTANS_LIST_UP			, OnMASliderUp	)
SPIMESSAGE_CONTROL		( SPIM_LBUTTON_UP	, WIID_RS_MSTANS_LIST_DOWN			, OnMASliderDown)

SPIMESSAGE_CONTROL		( SPIM_LBUTTON_UP	, WIID_RS_OTHANS_LIST				, OnOtherAnsClick	)
SPIMESSAGE_CONTROL		( SPIM_WHEEL_UP		, WIID_RS_OTHANS_LIST				, OnWheelUp		)
SPIMESSAGE_CONTROL		( SPIM_WHEEL_DOWN	, WIID_RS_OTHANS_LIST				, OnWheelDown	)
SPIMESSAGE_CONTROL		( SPIM_WHEEL_UP		, WIID_RS_OTHANS_LIST_SLIDER		, OnOASliderUp	)
SPIMESSAGE_CONTROL		( SPIM_WHEEL_DOWN	, WIID_RS_OTHANS_LIST_SLIDER		, OnOASliderDown)
SPIMESSAGE_CONTROL		( SPIM_LBUTTON_UP	, WIID_RS_OTHANS_LIST_UP			, OnOASliderUp	)
SPIMESSAGE_CONTROL		( SPIM_LBUTTON_UP	, WIID_RS_OTHANS_LIST_DOWN			, OnOASliderDown)
SPIMESSAGE_CONTROL		( SPIM_LBUTTON_UP	, WIID_RS_OTHANS_JUDGE_YES			, OnJudgeYes	)
SPIMESSAGE_CONTROL		( SPIM_LBUTTON_UP	, WIID_RS_OTHANS_JUDGE_NO			, OnJudgeNo		)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	, WIID_RS_JUDGE_RADIO	, WIID_RS_JUDGE_RADIO+MAX_RS_RADIO_SIZE	, OnJudgeSelect	)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	, WIID_RS_JUDGE_TEXT	, WIID_RS_JUDGE_TEXT+MAX_RS_RADIO_SIZE	, OnJudgeSelectText	)
SPIMESSAGE_CONTROL		( SPIM_LBUTTON_UP	, WIID_RS_JUDGE_BTN									, OnJudgeSend	)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	, WIID_RS_SKIN_BG		, WIID_RS_JUDGE_BTN			, OnHitTest	)

SPIMESSAGE_OBJ_MAP_END(SPWindow)


//------------------------------------------------------------------------------------
// Message
int SPWindowRS::OnExit(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}

/**
	Select Main Menu
 */
int SPWindowRS::OnTabMemuSelect(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iIndex = iID - WIID_RS_TAB_MENU_01 ;

	if( iIndex == 0 )		
		g_pkRsManager->SendQHotTop10() ;
	else
		g_pkRsManager->SendQSubMenu(iIndex) ;
	
	return 1;
}

//---------------------------------------------------------------------
/**
	Hot
 */
int SPWindowRS::OnHotURLClick(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iIndex = iID - WIID_RS_HOT_TEXT_URL ;
	ErrorFLogs->Error_Log("HotURL Click Index[%d]", iIndex) ;
	SPWindowRSStatic* pRSStatic = NULL ;

	pRSStatic = (SPWindowRSStatic*)Find(iID, true) ;

	if( m_eCurMode == RS_SHOW_SUBMENU )
	{
		// SubMenu였을 경우 HotContents 호출
		g_pkRsManager->SendQSubMenuContents(g_pkRsManager->GetMainMenu(), iIndex) ;
	}
	else if( m_eCurMode == RS_SHOW_HOTTOPIC )
	{
		// HotTopic였을 경우 Answer 호출
		g_pkRsManager->SendQuestionText(pRSStatic->GetWindowText()) ;
	}
	
	return 1;
}


/**
	Write Question
*/
int SPWindowRS::OnSendQuestion(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = NULL ;
	pWindow = Find(WIID_RS_QUESTION_EDIT, true) ;
	if( pWindow )
		g_pkRsManager->SendQuestionText(pWindow->GetWindowText()) ;

	return 1;
}



//---------------------------------------------------------------------
/**
	MasterAnswer List Scroll
*/
int SPWindowRS::OnMASliderUp( WPARAM wParam, LPARAM lParam )
{
	m_pkMasterAns->ScrollList( SUB_SPIM_SCROLL_UP );
	return 1;
}

int SPWindowRS::OnMASliderDown( WPARAM wParam, LPARAM lParam )
{
	m_pkMasterAns->ScrollList( SUB_SPIM_SCROLL_DOWN );
	return 1;
}


//---------------------------------------------------------------------
/**
	Other Answer
 */
int SPWindowRS::OnOtherAnsClick(WPARAM wParam, LPARAM lParam)
{
	if( m_pkAnsList != NULL )
	{
		int	iXPos	=	LOWORD( lParam );
		int	iYPos	=	HIWORD( lParam );

		RSLIST_ITEM* pFindData = m_pkAnsList->GetMouseOverItem() ;
		if( pFindData )	{
			g_pkRsManager->SetlUserLogIdx(pFindData->m_stReturnValue.m_strReturnValue.c_str()) ;
			g_pkRsManager->SendQuestionContext(pFindData->m_strText.c_str()) ;
		}
	}
	return 1;
}

/**
	OtherAnswer List Scroll
 */
int SPWindowRS::OnOASliderUp( WPARAM wParam, LPARAM lParam )
{
	SPWindowSlider* pSlider = NULL ;
	pSlider = (SPWindowSlider*)Find(WIID_RS_OTHANS_LIST_SLIDER, true) ;
	if( pSlider )	{
		if( !pSlider->IsShow() )		return 0 ;
		pSlider->SPSendMessage( SPIM_SLIDER_UP );
		m_pkAnsList->ScrollList( SUB_SPIM_SCROLL_UP );
	}
	
	return 1;
}

int SPWindowRS::OnOASliderDown( WPARAM wParam, LPARAM lParam )
{
	SPWindowSlider* pSlider = NULL ;
	pSlider = (SPWindowSlider*)Find(WIID_RS_OTHANS_LIST_SLIDER, true) ;
	if( pSlider )	{
		if( !pSlider->IsShow() )		return 0 ;
		pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
		m_pkAnsList->ScrollList( SUB_SPIM_SCROLL_DOWN );
	}

	return 1;
}

int SPWindowRS::OnWheelUp( WPARAM wParam, LPARAM lParam )
{
	SPWindowSlider* pSlider = NULL ;
	pSlider = (SPWindowSlider*)Find(WIID_RS_OTHANS_LIST_SLIDER, true) ;
	if( pSlider )	{
		if( !pSlider->IsShow() )		return 0 ;
		pSlider->SPSendMessage( SPIM_SLIDER_UP );
	}


	return 1;
}

int SPWindowRS::OnWheelDown( WPARAM wParam, LPARAM lParam )
{
	SPWindowSlider* pSlider = NULL ;
	pSlider = (SPWindowSlider*)Find(WIID_RS_OTHANS_LIST_SLIDER, true) ;
	if( pSlider )	{
		if( !pSlider->IsShow() )		return 0 ;
		pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	}

	return 1;
}

//---------------------------------------------------------------------
/**
	만족 선택시 -> HotTop10으로 이동
*/
int SPWindowRS::OnJudgeYes(WPARAM wParam, LPARAM lParam)
{
	g_pkRsManager->SendJudgeAnsYes() ;
	return 1;
}

/**
	불만족 선택시 -> 불만족 리스트를 받아온다.
*/
int SPWindowRS::OnJudgeNo(WPARAM wParam, LPARAM lParam)
{
	g_pkRsManager->SendGetUnsatReason() ;
	return 1;
}


//---------------------------------------------------------------------
/**
	Judge
 */
int SPWindowRS::OnJudgeSelect(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	SPWindowEdit* pEdit = NULL ;
	SPWindowButton* pButton = NULL ;

	// 이전 Radio Select FALSE
	pButton = (SPWindowButton*)Find(WIID_RS_JUDGE_RADIO+m_iJudgeIndex, true) ;
	if( pButton ) {
		pButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	}

	// 선택에 따라 Edit를 Enable or Disable
	pEdit = (SPWindowEdit*)Find(WIID_RS_JUDGE_EDIT, true) ;
	if( iID == WIID_RS_JUDGE_RADIO + MAX_RS_RADIO_SIZE -1 )	{
		if( pEdit ) {
			pEdit->SetEnable(true) ;
			pEdit->SetFocus() ;
		}
	}
	else	{
		if( pEdit )
			pEdit->SetEnable(false) ;
	}

	// 선택된 Radio Select TRUE
	pButton = (SPWindowButton*)Find(iID, true) ;
	if( pButton )	{
		pButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
		m_iJudgeIndex = iID - WIID_RS_JUDGE_RADIO ;
	}

	return 1;
}
int SPWindowRS::OnJudgeSelectText(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	SPWindowEdit* pEdit = NULL ;
	SPWindowButton* pButton = NULL ;
	// 이전 Radio Select FALSE
	pButton = (SPWindowButton*)Find(WIID_RS_JUDGE_RADIO+m_iJudgeIndex, true) ;
	if( pButton ) {
		pButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	}

	// 선택에 따라 Edit를 Enable or Disable
	pEdit = (SPWindowEdit*)Find(WIID_RS_JUDGE_EDIT, true) ;
	if( iID == WIID_RS_JUDGE_TEXT + MAX_RS_RADIO_SIZE -1 )	{
		if( pEdit )	{
			pEdit->SetEnable(true) ;
			pEdit->SetFocus() ;
		}
	}
	else	{
		if( pEdit )
			pEdit->SetEnable(false) ;
	}

	// 선택된 Radio Select TRUE
	m_iJudgeIndex = iID - WIID_RS_JUDGE_TEXT ;
	pButton = (SPWindowButton*)Find(WIID_RS_JUDGE_RADIO+m_iJudgeIndex, true) ;
	if( pButton )	{
		pButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
	}

	return 1;
}

int SPWindowRS::OnJudgeSend(WPARAM wParam, LPARAM lParam)
{
	SPWindowButton* pButton = NULL ;
	std::string strJudgeAnswer ;
	SPWindowEdit* pEdit = NULL ;

	if( m_iJudgeIndex == MAX_RS_RADIO_SIZE -1 )		// Select Edit Text
	{
		pEdit = (SPWindowEdit*)Find(WIID_RS_JUDGE_EDIT, true) ;
		if( pEdit )		{
			strJudgeAnswer = pEdit->GetWindowText() ;
			if( strJudgeAnswer.length() < 1 )
				return 0 ;
			g_pkRsManager->SendJudgeUnsatReason(m_iJudgeIndex, strJudgeAnswer.c_str()) ;
		}
	}
	else
		g_pkRsManager->SendJudgeUnsatReason(m_iJudgeIndex) ;
	// Send JudgeAnswer
	// ClearJudgeWindow() ;
	return 1;
}



//---------------------------------------------------------------------
// RSManager_Proc Packet
//---------------------------------------------------------------------
/**
	Receive Main Menu
	@param LPARAM lParam : RS_MENU
 */
int SPWindowRS::OnRecvMainMenu		(WPARAM wParam, LPARAM lParam)
{
	SPWindowStatic* pStatic = NULL ;
	RS_MENU*		pMenu = NULL ;

	pMenu = (RS_MENU*)lParam ;
	if( NULL == pMenu )
	{
		ErrorFLogs->Error_Log("OnRecvMainMenu lParam is NULL!!!") ;
		return 0 ;
	}
	
	for( int i = 0; i < 5; i++ )
	{
		pStatic = (SPWindowStatic*)Find(WIID_RS_TAB_MENU_01+i, true) ;
		if( pStatic ) {
			if( pMenu->szText[i] ) {
				pStatic->SetWindowText(pMenu->szText[i]) ;
				pStatic->SetShowEnable(true) ;
			}
			else	{
				pStatic->SetWindowText("") ;
				pStatic->SetShowEnable(false) ;
				pStatic->Hide() ;
			}
		}
	}

	// If MainMenu Receive the show
	if( this->IsShow() == false )
	{
		SPWindow::Show();
	}

	return 1 ;
}


/**
	SubMenu Setting
	@param LPARAM lParam : RS_MENU
	if it has return value then set data and selectable
 */
int SPWindowRS::OnRecvSubMenu		(WPARAM wParam, LPARAM lParam)
{
	SPWindowStatic* pkStatic = NULL ;
	SPWindowRSStatic* pRSStatic = NULL ;
	RS_MENU*		pMenu = NULL ;

	pMenu = (RS_MENU*)lParam ;
	if( NULL == pMenu )
	{
		ErrorFLogs->Error_Log("OnRecvSubMenu lParam is NULL!!!") ;
		return 0 ;
	}
	
	// Title Icon & Text
	pkStatic = (SPWindowStatic*)Find(WIID_RS_HOT_ICON, true) ;
	if( pkStatic != NULL )	{
		pkStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);
	}
	pkStatic = (SPWindowStatic*)Find(WIID_RS_HOT_TITLE, true) ;
	if( pkStatic != NULL )	{
		pkStatic->SetWindowText(g_pkRsManager->GetQuestionTitle()) ;
	}

	// Text Contents
	for( int i = 0; i < MAX_RS_URL_SIZE; i++)
	{
		pkStatic = (SPWindowStatic*)Find(WIID_RS_HOT_ICON_URL+i, true) ;
		pRSStatic = (SPWindowRSStatic*)Find(WIID_RS_HOT_TEXT_URL+i, true) ;
		if( i < pMenu->iCount )	// Set Text
		{
			if( pRSStatic ) {
				pMenu->szText[i] ? pRSStatic->SetWindowText(pMenu->szText[i]) : pRSStatic->SetWindowText("") ;
				pRSStatic->SetReturnValue(1, pMenu->szAC[i]) ;
			}
			if( pkStatic )	{
				pkStatic->SetShowEnable(true) ;
				pkStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;		// Icon Index 0 is Menu
			}
		}
		else		// Set NULL
		{
			if( pRSStatic ) {
				pRSStatic->SetWindowText("") ;
				pRSStatic->SetReturnValue(0, NULL) ;
			}
			if( pkStatic ) {
				pkStatic->SetShowEnable(false) ;
				pkStatic->Hide() ;
			}
		}
	}
	if( m_pkHotWindow )
	{
		m_pkAnsWindow->Hide() ;
		m_pkJudgeWindow->Hide() ;

		m_pkHotWindow->Show() ;
		m_eCurMode = RS_SHOW_SUBMENU ;

		SPWindow* pWindow = NULL ;
		pWindow = Find(WIID_RS_QUESTION_EDIT, true) ;
		if( pWindow )
			pWindow->SetFocus() ;
	}
	return 1 ;
}


/**
	HotTopic Setting
	@param LPARAM lParam : RS_MENU
	if it has return value then set data and selectable
*/
int SPWindowRS::OnRecvTopTopic		(WPARAM wParam, LPARAM lParam)
{
	SPWindowStatic* pkStatic = NULL ;
	SPWindowStatic* pkStaticIcon = NULL ;
	SPWindowRSStatic* pkRSStatic = NULL ;
	RS_CONTENTS*	pContents = NULL ;

	pContents = (RS_CONTENTS*)lParam ;
	if( NULL == pContents )
	{
		ErrorFLogs->Error_Log("OnRecvTopTopic lParam is NULL!!!") ;
		return 0 ;
	}

	// Title Icon & Text
	pkStaticIcon = (SPWindowStatic*)Find(WIID_RS_HOT_ICON, true) ;
	if( pkStaticIcon != NULL )	{
		pkStaticIcon->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);
	}
	pkStatic = (SPWindowStatic*)Find(WIID_RS_HOT_TITLE, true) ;
	if( pkStatic != NULL )	{
		pkStatic->SetWindowText(g_pkRsManager->GetQuestionTitle()) ;
	}

	// Text Contents
	for( int i = 0 ; i < MAX_RS_URL_SIZE; i++ )
	{
		pkStatic = (SPWindowStatic*)Find(WIID_RS_HOT_ICON_URL+i, true) ;
		pkRSStatic = (SPWindowRSStatic*)Find(WIID_RS_HOT_TEXT_URL+i, true) ;
		if( i < (pContents->iCount -1) )	// Set Text
		{
			if( pkRSStatic ) {
				pContents->szText[i] ? pkRSStatic->SetWindowText(pContents->szText[i]) : pkRSStatic->SetWindowText("") ;
				pkRSStatic->SetReturnValue(1, NULL) ;
			}
			if( pkStatic ) {
				pkStatic->SetShowEnable(true) ;
				pkStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 1) ;		// Icon Index 1 is Topic
			}
		}
		else		// Set NULL
		{
			if( pkRSStatic ) {
				pkRSStatic->SetWindowText("") ;
				pkRSStatic->SetReturnValue(0, NULL) ;
			}
			if( pkStatic )	{
				pkStatic->SetShowEnable(false) ;
				pkStatic->Hide() ;
			}
		}
	}

	// Close Other Window & Show HotTopic Window
	if( m_pkHotWindow )
	{
		m_pkAnsWindow->Hide() ;
		m_pkJudgeWindow->Hide() ;

		m_pkHotWindow->Show() ;
		m_eCurMode = RS_SHOW_HOTTOPIC ;

		SPWindow* pWindow = NULL ;
		pWindow = Find(WIID_RS_QUESTION_EDIT, true) ;
		if( pWindow )
			pWindow->SetFocus() ;
	}
	return 1 ;	
}


/**
	Answer Setting
	@param LPARAM lParam : RS_ANSWER
	Set Master Answer & Other Answer list
 */
int SPWindowRS::OnRecvAnswer		(WPARAM wParam, LPARAM lParam)
{
	SPWindowStatic* pkStatic = NULL ;
	SPWindowButton* pkButton = NULL ;
	RS_ANSWER*		pAnswer = NULL ;

	pAnswer = (RS_ANSWER*)lParam ;
	if( NULL == pAnswer )
	{
		ErrorFLogs->Error_Log("OnRecvAnswer lParam is NULL!!!") ;
		return 0 ;
	}

	ClearAnswerWindow() ;
	// Title
	pkStatic = (SPWindowStatic*)Find(WIID_RS_MSTANS_TITLE, true) ;
	if( pkStatic )		{
		pkStatic->SetWindowText(g_pkRsManager->GetQuestionTitle()) ;
	}

	// Master Answer & Scroll
	if( m_pkMasterAns ) {
		AddMasterAnswer(pAnswer->szMasterAns) ;
		int k = m_pkMasterAns->GetTotalLine() ;
		if( k > 5 ) {
			pkButton = (SPWindowButton*)Find(WIID_RS_MSTANS_LIST_UP, true) ;
			if( pkButton )		
				pkButton->SetShowEnable(true) ;
			pkButton = (SPWindowButton*)Find(WIID_RS_MSTANS_LIST_DOWN, true) ;
			if( pkButton )		
				pkButton->SetShowEnable(true) ;
			m_pkMasterAns->ScrollList(SUB_SPIM_SCROLL_FIRST) ;
		}
		else
		{
			pkButton = (SPWindowButton*)Find(WIID_RS_MSTANS_LIST_UP, true) ;
			if( pkButton )		{
				pkButton->SetShowEnable(false) ;
				pkButton->Hide() ;
			}
			pkButton = (SPWindowButton*)Find(WIID_RS_MSTANS_LIST_DOWN, true) ;
			if( pkButton )		{
				pkButton->SetShowEnable(false) ;
				pkButton->Hide() ;
			}
		}
	}

	// Other Answer
	for( int i = 0; i < pAnswer->iOtherCount; i++ )
	{
		AddOtherAnswer(pAnswer->szOtherAns[i], i+1, pAnswer->szLogIdx); 
	}
	// Set ScrollCount = TotalCount - ViewCount + 1 ;
	SPWindowSlider* pSlider = NULL ;
	pSlider = (SPWindowSlider*)Find(WIID_RS_OTHANS_LIST_SLIDER, true) ;
	if( pSlider )	{
		pSlider->SPSendMessage( SPIM_SET_MAXVALUE , 10 - 5 + 1 );
		pSlider->SPSendMessage(SPIM_SET_CURVALUE, 0) ;
	}
	m_pkAnsList->ScrollList(SUB_SPIM_SCROLL_FIRST) ;

	// Close Other Window & Show Answer Window
	if( m_pkAnsWindow )
	{
		m_pkHotWindow->Hide() ;
		m_pkJudgeWindow->Hide() ;

		m_pkAnsWindow->Show() ;
		m_eCurMode = RS_SHOW_ANSWER ;
	}
	return 1;
}


/**
	UnSatisfyReason Setting
	@param LPARAM lParam : RS_ANSWER
*/
int SPWindowRS::OnRecvUnSatReason(WPARAM wParam, LPARAM lParam)
{
	SPWindowButton* pButton = NULL ;
	SPWindow* pStatic = NULL ;
	RS_CONTENTS*	pReason = NULL ;

	pReason = (RS_CONTENTS*)lParam ;
	if( NULL == pReason )
	{
		ErrorFLogs->Error_Log("OnRecvUnSatReason lParam is NULL!!!") ;
		return 0 ;
	}

	ClearJudgeWindow() ;
	// Title

	// UnSatReason
	if( pReason->iCount != MAX_RS_RADIO_SIZE-1 )
	{
		ErrorFLogs->Error_Log("Reason size is Over [%d]", pReason->iCount) ;
	}
	for( int i = 0; i < MAX_RS_RADIO_SIZE-1; i++ )
	{
		pButton = (SPWindowButton*)Find(WIID_RS_JUDGE_RADIO+i, true) ;
		pStatic = (SPWindowButton*)Find(WIID_RS_JUDGE_TEXT+i, true) ;
		if( i < pReason->iCount )
		{
			if( pButton ) {
				pButton->SetShowEnable(true) ;
			}
			if( pStatic )	{
				pStatic->SetShowEnable(true) ;
				pStatic->SetWindowText(pReason->szText[i]) ;
			}
		}
		else
		{
			if( pButton ) {
				pButton->SetShowEnable(false) ;
				pButton->Hide() ;
			}
			if( pStatic )	{
				pStatic->SetShowEnable(false) ;
				pStatic->Hide() ;
			}
		}
	}


	// Close Other Window & Show Answer Window
	if( m_pkAnsWindow )
	{
		m_pkHotWindow->Hide() ;
		m_pkAnsWindow->Hide() ;

		m_pkJudgeWindow->Show() ;
		m_eCurMode = RS_SHOW_JUDGE ;
	}
	return 1;
}


//---------------------------------------------------------------------
// Edit Focus
int SPWindowRS::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}

int SPWindowRS::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}


/**
	Hit Test
*/
int SPWindowRS::OnHitTest(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iIndex = iID - WNDID_RSCLIENT ;
	ErrorFLogs->Error_Log("WindowRS Hit Index[%d]", iIndex) ;

	return 1;
}

//#endif