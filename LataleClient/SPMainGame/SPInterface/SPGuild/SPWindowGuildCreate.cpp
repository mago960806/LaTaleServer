// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD CREATE WINDOW
// Comment     : 
// Creation    : DURAGON 2007-4-04    11:18 
//***************************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowSlider.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowList.h"
#include "SPWindowFriendUser.h"
#include "SPWindowChat.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPNoticeBox.h"

#include <algorithm>

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "SPGOBStatus.h"

#include "SPItemCluster.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPEventDEF.h"			// for close
#include "SPEventManager.h"
#include "SPLocalizeManager.h"
#include "SPAbuseFilter.h"

#include "SPUtil.h"
#include "SPDebug.h"

#include "SPGuildLDT.h"
#include "SPGuildArchive.h"
#include "SPWindowGuildCreate.h"


SPWindowCreateGuild::SPWindowCreateGuild(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_GUILD_CREATE , InstanceID, iX, iY, iCX, iCY, pParent )	
{
	Init();
}

SPWindowCreateGuild::~SPWindowCreateGuild()
{
	Clean();
}

void SPWindowCreateGuild::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
}


void SPWindowCreateGuild::Init()
{
	m_strToolTip	=	"[SPWindowCreateGuild]";
	m_iShowPage		=	SHOW_GUILD_SELECT ;
	m_pEditName		=	NULL ;
	m_pTypeWnd		=	NULL ;
	m_pMakeWnd		=	NULL ;
	m_iSelectedProperty = 0 ;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[0]	, 482 , 346 , 486 , 350 );			//	배경 7
	SetRect( &m_rcBGSrc[1]	, 488 , 346 , 490 , 350 );			//	배경 8
	SetRect( &m_rcBGSrc[2]	, 492 , 346 , 496 , 350 );			//	배경 9
	SetRect( &m_rcBGSrc[3]	, 482 , 352 , 486 , 354 );			//	배경 4
	SetRect( &m_rcBGSrc[4]	, 488 , 352 , 490 , 354 );			//	배경 5
	SetRect( &m_rcBGSrc[5]	, 492 , 352 , 496 , 354 );			//	배경 6
	SetRect( &m_rcBGSrc[6]	, 482 , 356 , 486 , 360 );			//	배경 1
	SetRect( &m_rcBGSrc[7]	, 488 , 356 , 490 , 360 );			//	배경 2
	SetRect( &m_rcBGSrc[8]	, 492 , 356 , 496 , 360 );			//	배경 3
	SetRect( &m_rcBGSrc[9]	, 488 , 346 , 490 , 347 );			//	SubBG Line
	SetRect( &m_rcBGSrc[10]	, 488 , 337 , 490 , 339 );			//	SubBG
	SetRect( &m_rcBGSrc[11]	,   1 , 216 , 138 , 235 );			//	Title
	SetRect( &m_rcTypeSrc[0]	,   1 , 236 , 138 , 249 );		//	Sub Title (크리스탈 선택)
	SetRect( &m_rcTypeSrc[1]	, 445 , 337 , 481 , 348 );		//	Slot Shadow1
	SetRect( &m_rcTypeSrc[2]	, 445 , 337 , 481 , 348 );		//	Slot Shadow2
	SetRect( &m_rcTypeSrc[3]	, 445 , 337 , 481 , 348 );		//	Slot Shadow3
	SetRect( &m_rcTypeSrc[4]	, 208 , 352 , 263 , 367 );		//	Slot Text1
	SetRect( &m_rcTypeSrc[5]	, 264 , 352 , 319 , 367 );		//	Slot Text2
	SetRect( &m_rcTypeSrc[6]	, 320 , 352 , 375 , 367 );		//	Slot Text3
	SetRect( &m_rcNameSrc[0]	,   1 , 250 , 138 , 265 );		//	Name
	SetRect( &m_rcNameSrc[1]	, 445 , 337 , 481 , 348 );		//	Slot Shoadow
	SetRect( &m_rcNameSrc[2]	, 208 , 338 , 414 , 351 );		//	Warning Text


	Init_SelectType() ;
	Init_MakeName() ;
}

void SPWindowCreateGuild::Init_SelectType()
{
	SPWindowStatic*	pWindowStatic = NULL ;
	SPWindowButton*	pWindowButton = NULL ;

	m_pTypeWnd	=	new	SPWindowStatic( WIID_CREATEGUILD_SELECT_BG , 0 , 0 , m_iSX , m_iSY , this );

	pWindowButton	=	new	SPWindowButton( WIID_CREATEGUILD_SELECT_01 , 48 , 58 , 32 , 32 , m_pTypeWnd );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CREATEGUILD_SELECT_02 , 120 , 58 , 32 , 32 , m_pTypeWnd );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CREATEGUILD_SELECT_03 , 192 , 58 , 32 , 32 , m_pTypeWnd );
	pWindowButton =	NULL;

	pWindowStatic	=	new	SPWindowStatic( WIID_CREATEGUILD_SELECT_DESC_BG , 18 , 119 , 236 , 97 , m_pTypeWnd );
	pWindowStatic->SetWindowType	(WND_TYPE_TOPBASE);
	pWindowStatic->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI105.PNG", 208 , 368, 444, 370);
	pWindowStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI105.PNG", 208 , 370, 444, 372);
	pWindowStatic->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI105.PNG", 208 , 372, 444, 374);
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_CREATEGUILD_SELECT_DESCRIPT , 25 , 124 , 227 , 85 , m_pTypeWnd );
	pWindowStatic->SetFormat(DT_TOP | DT_LEFT | DT_WORDBREAK);
	pWindowStatic->SetFontColor( RGBA(119, 76, 95, 255) ) ;
	pWindowStatic->SetMultiLine(true, 5);
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_CREATEGUILD_SELECT_NEXT , 64 , 226 , 68 , 17 , m_pTypeWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 296 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 314 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 332 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 350 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CREATEGUILD_SELECT_CANCEL , 140 , 226 , 68 , 17 , m_pTypeWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 415 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 415 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 415 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 415 , 494 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

}

void SPWindowCreateGuild::Init_MakeName()
{
	SPWindowStatic*	pWindowStatic;
	SPWindowButton*	pWindowButton;

	m_pMakeWnd		=	new	SPWindowStatic( WIID_CREATEGUILD_NAME_BG , 0 , 0 , m_iSX , m_iSY , this );

	pWindowStatic	=	new	SPWindowStatic( WIID_CREATEGUILD_NAME_TAG_L, 25 , 72 , 4 , 85 , m_pMakeWnd );
	pWindowStatic->SetImage("DATA/INTERFACE/CONCEPT/UI107.PNG", 501 , 183) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;
	pWindowStatic	=	new	SPWindowStatic( WIID_CREATEGUILD_NAME_TAG_R, 243 , 72 , 4 , 85 , m_pMakeWnd );
	pWindowStatic->SetImage("DATA/INTERFACE/CONCEPT/UI107.PNG", 507 , 183) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_CREATEGUILD_NAME_SELECT_ICON, 62 , 97 , 32 , 32 , m_pMakeWnd );
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	//pWindowStatic	=	new	SPWindowStatic( WIID_CREATEGUILD_NAME_SELECT_IMG, 103 , 108 , 65 , 13 , m_pMakeWnd );
	//pWindowStatic->SetImage("DATA/INTERFACE/CONCEPT/UI107.PNG", 70 , 193) ;
	//pWindowStatic->Hide() ;
	//pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_CREATEGUILD_NAME_SELECT_TYPE, 103 , 108 , 128 , 13 , m_pMakeWnd );
	pWindowStatic->SetFontColor(RGBA(104, 113, 145, 255));
	pWindowStatic->SetFormat(DT_TOP | DT_LEFT);
	pWindowStatic->SetWindowText("타입설명 ") ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_CREATEGUILD_NAME_EDIT_BG , 33 , 176 , 206 , 17 , m_pMakeWnd );
	pWindowStatic->SetWindowType	(WND_TYPE_LEFTBASE);
	pWindowStatic->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI103.PNG", 501 , 451, 503, 468);
	pWindowStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI103.PNG", 506 , 451, 507, 468);
	pWindowStatic->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI103.PNG", 509 , 451, 511, 468);
	pWindowStatic->Hide() ;
	m_pEditName		= new SPWindowEdit(WIID_CREATEGUILD_NAME_EDIT, 3, 3, 187, 12, pWindowStatic) ;
	m_pEditName->SetFontColor( RGBA(119, 76, 95, 255) );
	m_pEditName->SetEnableTabStop(true);
	m_pEditName->SetLimitText(GUILD_MAX_NAME);		// 
	m_pEditName->Hide() ;

	pWindowButton	=	new	SPWindowButton( WIID_CREATEGUILD_OK , 64 , 226 , 68 , 17 , m_pMakeWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 1 , 368 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 1 , 386 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 1 , 404 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 1 , 422 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CREATEGUILD_CANCEL , 140 , 226 , 68 , 17 , m_pMakeWnd );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 368 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 386 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 404 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 422 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;
}

void SPWindowCreateGuild::Show(bool bWithChild)
{

	if( g_pGOBManager->GetGuildArchive()->GetBasicAvailable() && SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) )
	{
		if( g_pResourceManager->GetGlobalString(27000002) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000002) );
		OnClose(0, 0) ;
		return ;
	}

	m_iSelectedProperty = 0 ;
	ShowGuildTypeIcon() ;

	SPWindow::Show(bWithChild);
	ChangeShowWnd(SHOW_GUILD_SELECT) ;			// Property Select 창이 먼저 보인다.
}


/**
길드 생성 아이콘은 Guild.LDT의 ConsumeItemID로 셋팅된다.
*/
void SPWindowCreateGuild::ShowGuildTypeIcon()
{
	SPWindowButton* pWindowBtn = NULL ;
	SPTexture* pTexture = NULL ;
	POINT	ptSrcPos ;
	UINT32 iConsumeItemID ;

	for( int i = 0 ; i < 3; i++	)
	{
		iConsumeItemID = g_pkGuildManager->FindGuildCreateItem(10 * GUILD_PROPERTY_NORMAL + i + 1) ;		// 10*레벨 + 타입
		if( iConsumeItemID == 0)	pTexture = g_pItemCluster->GetItemTexture(GUILD_CREATE_ITEM_ICON01 + i, ptSrcPos);
		else						pTexture = g_pItemCluster->GetItemTexture(iConsumeItemID, ptSrcPos);
		pWindowBtn = (SPWindowButton*)Find(WIID_CREATEGUILD_SELECT_01 + i, true) ;
		pWindowBtn->SetImage	( pTexture, ptSrcPos.x, ptSrcPos.y ) ;
		pWindowBtn->SetImageHit	( pTexture, ptSrcPos.x, ptSrcPos.y );
		pWindowBtn->SetImagePush( pTexture, ptSrcPos.x, ptSrcPos.y );
	}

	SPWindowStatic* pStatic = (SPWindowStatic*)Find(WIID_CREATEGUILD_SELECT_DESCRIPT, true) ;
	if( pStatic )
		pStatic->SetWindowText(g_pResourceManager->GetGlobalString(27000055+m_iSelectedProperty)) ;
}

void SPWindowCreateGuild::SetSelectedType(int iGType)
{
	POINT	ptSrcPos ;
	TCHAR	szType[64] ;
	SPTexture* pTexture = NULL ;
	SPWindowButton* pWindowBtn = NULL ;
	SPWindowStatic* pWindowStc = NULL ;
	pWindowBtn = (SPWindowButton*)Find(WIID_CREATEGUILD_NAME_SELECT_ICON, true) ;
	pWindowStc = (SPWindowStatic*)Find(WIID_CREATEGUILD_NAME_SELECT_TYPE, true) ;

	if( !pWindowBtn || !pWindowStc )
		return ;
	if( iGType == GUILD_PROPERTY_NORMAL ) {				// 중립
		pTexture = g_pItemCluster->GetItemTexture(GUILD_CREATE_ITEM_ICON01 + m_iSelectedProperty, ptSrcPos);
		pWindowBtn->SPSendMessage(SPIM_SET_IMAGECLEAR);
		pWindowBtn->SetImage	( pTexture, ptSrcPos.x, ptSrcPos.y ) ;
		_stprintf(szType, "%s : [%s]", g_pResourceManager->GetGlobalString(27000108), g_pResourceManager->GetGlobalString(27000011)) ;
		pWindowStc->SetWindowText(szType) ;
	}
	else if( iGType == GUILD_PROPERTY_ANGEL ) {			// 질서
		pTexture = g_pItemCluster->GetItemTexture(GUILD_CREATE_ITEM_ICON01 + m_iSelectedProperty, ptSrcPos);
		pWindowBtn->SPSendMessage(SPIM_SET_IMAGECLEAR);
		pWindowBtn->SetImage	( pTexture, ptSrcPos.x, ptSrcPos.y ) ;
		_stprintf(szType, "%s : [%s]", g_pResourceManager->GetGlobalString(27000108), g_pResourceManager->GetGlobalString(27000018)) ;
		pWindowStc->SetWindowText(szType) ;
	}
	else if( iGType == GUILD_PROPERTY_DAEMON ) {		// 혼돈
		pTexture = g_pItemCluster->GetItemTexture(GUILD_CREATE_ITEM_ICON01 + m_iSelectedProperty, ptSrcPos);
		pWindowBtn->SPSendMessage(SPIM_SET_IMAGECLEAR);
		pWindowBtn->SetImage	( pTexture, ptSrcPos.x, ptSrcPos.y ) ;
		_stprintf(szType, "%s : [%s]", g_pResourceManager->GetGlobalString(27000108), g_pResourceManager->GetGlobalString(27000020)) ;
		pWindowStc->SetWindowText(szType) ;
	}
}



void SPWindowCreateGuild::Hide(bool bSendServer)
{
	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorOut( NULL , NULL );
	}
	KillFocus() ;
}

void SPWindowCreateGuild::ReposOwnImage()
{
	SetRect( &m_rcBGDest[0]	, m_iAX			, m_iAY			, m_iAX + 4		, m_iAY + 4 );		//	배경 7
	SetRect( &m_rcBGDest[1]	, m_iAX + 4		, m_iAY			, m_iAX + 268	, m_iAY + 4 );		//	배경 8
	SetRect( &m_rcBGDest[2]	, m_iAX + 268	, m_iAY 		, m_iAX + 272	, m_iAY + 4 );		//	배경 9
	SetRect( &m_rcBGDest[3]	, m_iAX 		, m_iAY + 4		, m_iAX + 4		, m_iAY + 251 );	//	배경 4
	SetRect( &m_rcBGDest[4]	, m_iAX + 4		, m_iAY + 4		, m_iAX + 268	, m_iAY + 251 );	//	배경 5
	SetRect( &m_rcBGDest[5]	, m_iAX	+ 268	, m_iAY + 4		, m_iAX + 272	, m_iAY + 251 );	//	배경 6
	SetRect( &m_rcBGDest[6]	, m_iAX 		, m_iAY + 251	, m_iAX + 4		, m_iAY + 255 );	//	배경 1
	SetRect( &m_rcBGDest[7]	, m_iAX + 4		, m_iAY + 251	, m_iAX + 268	, m_iAY + 255 );	//	배경 2
	SetRect( &m_rcBGDest[8]	, m_iAX + 268	, m_iAY + 251	, m_iAX + 272	, m_iAY + 255 );	//	배경 3

	SetRect( &m_rcBGDest[9]	, m_iAX + 2		, m_iAY + 26	, m_iAX + 270	, m_iAY + 27 );		//	SubBG Line
	SetRect( &m_rcBGDest[10], m_iAX + 2		, m_iAY + 27	, m_iAX + 270	, m_iAY + 254 );	//	SubBG
	SetRect( &m_rcBGDest[11], m_iAX + 9		, m_iAY + 5		, m_iAX + 146	, m_iAY + 24 );		//	Title

	SetRect( &m_rcTypeDest[0]	, m_iAX + 15	, m_iAY + 38	, m_iAX + 152	, m_iAY + 51);		//	Sub Title (크리스탈 선택)
	SetRect( &m_rcTypeDest[1]	, m_iAX + 46	, m_iAY + 81	, m_iAX + 82	, m_iAY + 92);		//	Slot Shadow1
	SetRect( &m_rcTypeDest[2]	, m_iAX + 118	, m_iAY + 81	, m_iAX + 154	, m_iAY + 92);		//	Slot Shadow2
	SetRect( &m_rcTypeDest[3]	, m_iAX + 190	, m_iAY + 81	, m_iAX + 226	, m_iAY + 92);		//	Slot Shadow3

	SetRect( &m_rcTypeDest[4]	, m_iAX + 36	, m_iAY + 98	, m_iAX + 91	, m_iAY + 113);		//	Slot Text1
	SetRect( &m_rcTypeDest[5]	, m_iAX + 108	, m_iAY + 98	, m_iAX + 163	, m_iAY + 113);		//	Slot Text2
	SetRect( &m_rcTypeDest[6]	, m_iAX + 180	, m_iAY + 98	, m_iAX + 235	, m_iAY + 113);		//	Slot Text3

	SetRect( &m_rcNameDest[0]	, m_iAX + 18	, m_iAY + 38	, m_iAX + 155	, m_iAY + 53 );		//	Name
	SetRect( &m_rcNameDest[1]	, m_iAX + 60	, m_iAY + 120	, m_iAX + 96	, m_iAY + 131 );	//	Slot Shoadow
	SetRect( &m_rcNameDest[2]	, m_iAX + 27	, m_iAY + 196	, m_iAX + 233	, m_iAY + 209 );	//	Warning Text
}


void SPWindowCreateGuild::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowCreateGuild::Process(float fTime)
{
	SPWindow::Process( fTime );
}

void SPWindowCreateGuild::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		RenderBackGround(fTime) ;
	}

	SPWindow::Render(fTime);
}


void SPWindowCreateGuild::RenderBackGround(float fTime)
{
	int i ;
	for( i = 0 ; i < GUILD_CREATE_BG_IMG ; i++ ) {
		m_pBGTexture->SetColor();
		m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
	}
	g_pVideo->Flush();

	//////////////////////////////////////////////////////////////////////////
	// Type 선택
	if( m_iShowPage == SHOW_GUILD_SELECT )
	{
		for( i = 0 ; i < GUILD_SELECT_TYPE_IMG ; i++ ) {
			m_pBGTexture->SetColor();
			m_pBGTexture->RenderTexture( &m_rcTypeDest[i] , &m_rcTypeSrc[i] );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 생성
	else if ( m_iShowPage == SHOW_GUILD_MAKE)
	{
		for( i = 0 ; i < GUILD_MAKE_NAME_IMG ; i++ ) {
			m_pBGTexture->SetColor();
			m_pBGTexture->RenderTexture( &m_rcNameDest[i] , &m_rcNameSrc[i] );
		}
	}
	g_pVideo->Flush();
}


/**
윈도우의 변경 (길드 타입선택) -> (길드명 입력)
*/
void SPWindowCreateGuild::ChangeShowWnd(int iShowPage)
{
	m_iShowPage = iShowPage ;
	if( m_iShowPage == SHOW_GUILD_SELECT )
	{
		m_pMakeWnd->Hide() ;
		m_pTypeWnd->Show() ;
	}
	else if( m_iShowPage == SHOW_GUILD_MAKE )
	{
		m_pTypeWnd->Hide() ;
		m_pMakeWnd->Show() ;

		if( m_pEditName )
		{
			m_pEditName->SetWindowText("") ;
			m_pEditName->SetFocus() ;
		}
		SetSelectedType(m_iSelectedProperty+1) ;
	}
}



/**
길드명에 욕설 체크
@return true : 통과, false : 실패
*/
bool SPWindowCreateGuild::CheckAbuseGuildName(const char* strGName)
{
	char szBuffer[128];
	int iReplace;
	SPAbuseFilter::GetInstance()->Filter(strGName, szBuffer, 127, &iReplace);

	bool bRetVal = false;

	static COUNTRY_TYPE CountryType = SPLocalizeManager::GetInstance()->GetCountryType();

	switch(CountryType) 
	{
	case CT_BUDDYBUDDY:			// SITE_GAMEPOT
	case CT_KOREA:				// SITE_ACTOZ
		bRetVal = SPAbuseFilter::GetInstance()->IsStringWithSpecialChar(strGName);
		break;

	case CT_JAPAN:				// SITE_GAMEPOT
		bRetVal = SPAbuseFilter::GetInstance()->IsStringWithSpecialCharJapan(strGName);
		break;

	case CT_CHINA:				// SITE_SANDA
		// 중국 버전에 합당한 코딩 필요
		//assert(false && "Please verify CT_CHINA value(CountryType).!!!");
		bRetVal = SPAbuseFilter::GetInstance()->IsStringWithSpecialCharJapan(strGName);
		break;
	}	

	bool bRetVal2 = false;	
	bRetVal2 = SPAbuseFilter::GetInstance()->IsBanName(strGName);


	if( iReplace || bRetVal || bRetVal2 )
		return false ;

	return true ;
}





SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowCreateGuild )
SPIMESSAGE_COMMAND(SPIM_GUILD_CREATE_RESULT,	OnCreateResult	)
SPIMESSAGE_COMMAND(SPIM_KEY_RETURN,				OnNameKeyReturn	)

SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP			, WIID_CREATEGUILD_CLOSE			,	OnClose	)

SPIMESSAGE_CONTROL(	SPIM_SETFOCUS			,	WIID_CREATEGUILD_NAME_EDIT		,	OnEditSetFocus		)
SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS			,	WIID_CREATEGUILD_NAME_EDIT		,	OnEditKillFocus		)

SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP	, WIID_CREATEGUILD_SELECT_01	, WIID_CREATEGUILD_SELECT_03		,	OnSelectProperty	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP			, WIID_CREATEGUILD_SELECT_NEXT		,	OnPropertyCommit	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP			, WIID_CREATEGUILD_SELECT_CANCEL	,	OnClose	)

SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP			, WIID_CREATEGUILD_OK			,	OnCommit	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP			, WIID_CREATEGUILD_CANCEL		,	OnClose		)

SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP	, WIID_GUILD_CREATE	, WIID_CREATEGUILD_END		,	OnTest	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)




int SPWindowCreateGuild::OnClose( WPARAM wParam , LPARAM lParam  )
{
	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	return 1 ;
}


int SPWindowCreateGuild::OnCreateResult( WPARAM wParam , LPARAM lParam  )
{
	char szTemp[512] ;
	int iReault = (int)wParam ;

	if( iReault == 0 )		// Success
	{
		if( g_pResourceManager->GetGlobalString(27000029) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000029) );
		Hide() ;
		g_pInterfaceManager->ShowWindow(WIID_GUILD_EMBLEM, SP_SHOW);
		g_pInterfaceManager->SetFocusWindow(g_pInterfaceManager->GetGuildEmblemWindow()) ;
	}
	else		// Create Fail
	{
		if( g_pResourceManager->GetGlobalString(iReault) )
			g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(iReault), this, NOTICE_TYPE_YES);
		else	{
			sprintf(szTemp, "Guild create Fail[%d]", iReault);
			g_pInterfaceManager->SetNotice(szTemp, this, NOTICE_TYPE_YES);
		}
	}
	return 1 ;
}


int SPWindowCreateGuild::OnTest( unsigned int iID, WPARAM wParam , LPARAM lParam  )
{
	int iIndex = iID - WIID_GUILD_CREATE ;
	return 1 ;
}

/**
길드 성향을 선택한 경우
* 선택한 타입에 해당 아이템 유무 확인은 [Next]에서 -> 없으면 ErrorMsg
1) 선택 정보의 Text 보여주기	(WIID_CREATEGUILD_SELECT_DESCRIPT)
2) 선택정보 저장
*/
int SPWindowCreateGuild::OnSelectProperty( unsigned int iID, WPARAM wParam , LPARAM lParam  )
{
	m_iSelectedProperty = iID - WIID_CREATEGUILD_SELECT_01 ;
	SPWindowStatic* pStatic = (SPWindowStatic*)Find(WIID_CREATEGUILD_SELECT_DESCRIPT, true) ;
	if( pStatic == NULL)		return 0 ;

	pStatic->SetWindowText(g_pResourceManager->GetGlobalString(27000055+m_iSelectedProperty)) ;
	return 1 ;
}

/**
길드성향 -> [Next]를 누른경우
1) 해당 성향의 길드 생성 아이템이 있는지 확인 -> 없으면 Error Message
2) 아이템이 있다면 다음 페이지 보여주기
*/
int SPWindowCreateGuild::OnPropertyCommit( WPARAM wParam , LPARAM lParam  )
{
	int iRet ;
	if( (iRet = g_pGOBManager->GetGuildArchive()->CheckGuildCreate(m_iSelectedProperty+1)) == 0)
	{
		ChangeShowWnd(SHOW_GUILD_MAKE) ;
	}
	else
	{
		ShowGuildCreateNotice(iRet) ;
	}
	return 1 ;
}

/**
@return	GUILD_CREATE_OK				: 성공
@return ERROR_GUILD_CREATE_STAT		: 제한 1 (스텟)
@return ERROR_GUILD_CREATE_MONEY	: 제한 2 (머니)
@return ERROR_GUILD_CREATE_ITEM		: 제한 3 (아이템)
@return ERROR_GUILD_CREATE_REQ4		: 제한 4
@return ERROR_GUILD_CREATE_REQ5		: 제한 5
@return ERROR_GUILD_CREATE_ISHAVE	: 이미 길드 가입
@return ERROR_GUILD_CREATE_GNAME	: 길드명 오류
@return ERROR_GUILD_CREATE_LDT		: LDT Error
*/
void SPWindowCreateGuild::ShowGuildCreateNotice(int iRet)
{
	switch( iRet )
	{
	case ERROR_GUILD_CREATE_MONEY:
		g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(27000005), this, NOTICE_TYPE_YES);
		break ;
	case ERROR_GUILD_CREATE_ITEM:
		g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(27000052+m_iSelectedProperty), this, NOTICE_TYPE_YES);
		break ;
	case ERROR_GUILD_CREATE_ISHAVE:
		g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(27000002), this, NOTICE_TYPE_YES);
		break ;
	case ERROR_GUILD_CREATE_GNAME:
		g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(27000063), this, NOTICE_TYPE_YES);
		break ;
	case ERROR_GUILD_CREATE_STAT:
	case ERROR_GUILD_CREATE_REQ4:
	case ERROR_GUILD_CREATE_REQ5:
	case ERROR_GUILD_CREATE_LDT:
	default:
		g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(27000006), this, NOTICE_TYPE_YES);
		break ;
	}
}

int SPWindowCreateGuild::OnNameKeyReturn( WPARAM wParam, LPARAM lParam )
{
	if( m_pEditName->IsFocus() == TRUE )
	{
		const char* pName = m_pEditName->GetWindowText() ;
		if( pName && strlen(pName) >= MIN_LEN_NAME )
			OnCommit(0, 0) ;
		return 1;
	}
	return -1;
}


/**
최종 생성버튼 선택
1) 길드명 체크 (욕설)
2) 길드 생성 조건 체크
>> 지정한 내용으로 길드 생성 패킷을 보내고 결과를 받는다.
*/
int SPWindowCreateGuild::OnCommit( WPARAM wParam , LPARAM lParam  )
{
	int iRet ;
	std::string strGName = m_pEditName->GetWindowText() ;
	Trim(strGName) ;
	if( SPLocalizeManager::GetInstance()->GetCountryType() == CT_CHINA )
	{
		if( strGName.length() <= MIN_LEN_GUILDNAME )		// China 2Byte Create Fail
		{
			g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(27000058), this, NOTICE_TYPE_YES);
			return 0 ;
		}
	}
	else
	{
		if( strGName.length() < MIN_LEN_GUILDNAME )
		{
			g_pInterfaceManager->SetNotice(g_pResourceManager->GetGlobalString(27000058), this, NOTICE_TYPE_YES);
			return 0 ;
		}
	}
	if( CheckAbuseGuildName(strGName.c_str()) == false )
	{
		ShowGuildCreateNotice(ERROR_GUILD_CREATE_GNAME) ;
		return 0 ;
	}

	if( (iRet = g_pGOBManager->GetGuildArchive()->CheckGuildCreate(m_iSelectedProperty+1)) == 0)
	{
		g_pGOBManager->GetGuildArchive()->SendGuildCreate(m_iSelectedProperty+1, 0, 0, strGName.c_str()) ;
	}
	else
	{
		ShowGuildCreateNotice(iRet) ;
	}
	return 1 ;
}



/**
Edit Window에 포커스를 가지지 않으면 단축키가 발동된다.
*/
int SPWindowCreateGuild::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}

int SPWindowCreateGuild::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}