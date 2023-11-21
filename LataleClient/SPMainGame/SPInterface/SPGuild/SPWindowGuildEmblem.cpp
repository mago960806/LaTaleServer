// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD EMBLEM WINDOW
// Comment     : 
// Creation    : DURAGON 2007-8-22    11:18 
//***************************************************************************

#include "SPCommon.h"

#include <algorithm>
#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPMouseCursor.h"
#include "SPWindowNeedList.h"
#include "SPInterfaceManager.h"
#include "SPNoticeBox.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "SPGOBStatus.h"

#include "SPItemAttr.h"
#include "SPItemCluster.h"
#include "SPResourceManager.h"
#include "SPCheckManager.h"
#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPEventDEF.h"			// for close
#include "SPEventManager.h"
#include "SPLocalizeManager.h"
#include "SPAbuseFilter.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXFrameAnimationUnit.h"
#include "SPFXManager.h"
#include "SPFXDamageUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPUtil.h"
#include "SPDebug.h"

#include "SPGuildLDT.h"
#include "SPGuildArchive.h"
#include "SPWindowEmConfirm.h"
#include "SPWindowGuildEmblem.h"


SPWindowGuildEmblem::SPWindowGuildEmblem(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_GUILD_EMBLEM , InstanceID, iX, iY, iCX, iCY, pParent )	
, m_pBGTexture5(NULL)
, m_pBGTexture7(NULL)
, m_pkFXArchive(NULL)
, m_pWindowToolTip(NULL)
, m_pWindowTipNeed(NULL)
, m_pWndBG(NULL)
, m_pWndModal(NULL)
, m_pBuyEmblem(NULL)
{
	Init();
}

SPWindowGuildEmblem::~SPWindowGuildEmblem()
{
	Clean();
}

void SPWindowGuildEmblem::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture5 );
	SAFE_RELEASE( m_pBGTexture7 );
	SAFE_DELETE( m_pkFXArchive );			// Fx
}


void SPWindowGuildEmblem::Init()
{
	m_strToolTip	=	"[SPWindowGuildEmblem]";
	m_iShowPage[LAYER_GUILD_SYM]	=	0 ;
	m_iShowPage[LAYER_GUILD_BG]		=	0 ;
	m_iShowPage[LAYER_GUILD_FX]		=	0 ;
	m_iSelIndex[LAYER_GUILD_SYM]	=	0 ;
	m_iSelIndex[LAYER_GUILD_BG]		=	0 ;
	m_iSelIndex[LAYER_GUILD_FX]		=	0 ;
	m_iPreBeforeFxID				=	0 ;
	m_iPreSelectFxID				=	0 ;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pBGTexture5 );
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI107.PNG" , &m_pBGTexture7 );
	m_pkFXArchive = new SPFXArchive(NULL);		///<FX

	ReposOwnImage();

	SetRect( &m_rcBGSrc[0]	, 363 , 323 , 367 , 327 );			//	배경 7
	SetRect( &m_rcBGSrc[1]	, 368 , 323 , 372 , 327 );			//	배경 8
	SetRect( &m_rcBGSrc[2]	, 373 , 323 , 377 , 327 );			//	배경 9
	SetRect( &m_rcBGSrc[3]	, 363 , 328 , 367 , 332 );			//	배경 4
	SetRect( &m_rcBGSrc[4]	, 368 , 328 , 372 , 332 );			//	배경 5
	SetRect( &m_rcBGSrc[5]	, 373 , 328 , 377 , 332 );			//	배경 6
	SetRect( &m_rcBGSrc[6]	, 363 , 333 , 367 , 337 );			//	배경 1
	SetRect( &m_rcBGSrc[7]	, 368 , 333 , 372 , 337 );			//	배경 2
	SetRect( &m_rcBGSrc[8]	, 373 , 333 , 377 , 337 );			//	배경 3
	//SetRect( &m_rcBGSrc[9]	, 361 , 349 , 318 , 351 );		//	Sub BG(White)
	SetRect( &m_rcBGSrc[9]	, 488 , 307 , 490 , 309 );			//	Sub BG(White)

	SetRect( &m_rcBGSrc[10]	, 70 , 221 , 238 , 240 );			//	Title
	SetRect( &m_rcBGSrc[11]	, 449 , 420 , 451 , 421 );			//	Line

	SetRect( &m_rcBGSrc[12]	, 70 , 274 , 206 , 289 );			//	Symbol Select TEXT
	SetRect( &m_rcBGSrc[13]	, 423 , 440 , 471 , 455 );			//	Symbol Page Count BG
	SetRect( &m_rcBGSrc[14]	, 472 , 348 , 500 , 376 );			//	Symbol Show BG
	SetRect( &m_rcBGSrc[15]	, 449 , 422 , 451 , 423 );			//	Line

	SetRect( &m_rcBGSrc[16]	, 70 , 290 , 206 , 305 );			//	BG Select TEXT
	SetRect( &m_rcBGSrc[17]	, 423 , 440 , 471 , 455 );			//	BG Page Count BG
	SetRect( &m_rcBGSrc[18]	, 472 , 348 , 500 , 376 );			//	BG Show BG
	SetRect( &m_rcBGSrc[19]	, 449 , 420 , 451 , 421 );			//	Line

	SetRect( &m_rcBGSrc[20]	, 70 , 306 , 206 , 321 );			//	FX Select TEXT
	SetRect( &m_rcBGSrc[21]	, 423 , 424 , 471 , 439 );			//	FX Page Count BG
	SetRect( &m_rcBGSrc[22]	, 472 , 348 , 500 , 376  );			//	FX Show BG

	SetRect(&m_rcIconBGSrc, 423, 417, 447, 423) ;			// Icon BG SRC

	// Animation Source coordinate
	m_mouseAni.Clear() ;
	SetRect(&m_mouseAni.rcSrc[0], 472, 377, 500, 405) ;
	SetRect(&m_mouseAni.rcSrc[1], 472, 406, 500, 434) ;
	SetRect(&m_mouseAni.rcSrc[2], 472, 435, 500, 463) ;
	SetRect(&m_mouseAni.rcSrc[3], 472, 464, 500, 492) ;
	for( int i = 0; i < LAYER_GUILD_MAX; i++)
	{
		m_selAni[i].Clear();
		SetRect(&m_selAni[i].rcSrc[0], 200, 322, 219, 341) ;
		SetRect(&m_selAni[i].rcSrc[1], 220, 322, 239, 341) ;
		SetRect(&m_selAni[i].rcSrc[2], 200, 342, 219, 361) ;
		SetRect(&m_selAni[i].rcSrc[3], 220, 342, 239, 361) ;
	}

	Init_Window() ;
	Init_ListWindow() ;
	InitTooltipWindow() ;
	InitModalWindow() ;
}

void SPWindowGuildEmblem::Init_Window()
{
	SPWindowStatic*	pWindowStatic = NULL;
	SPWindowButton*	pWindowButton = NULL;

	m_pWndModal		=	new	SPWindowStatic( WIID_EMBLEMGUILD_MODAL , 0 , 0 , m_iSX , m_iSY , this );
	m_pWndBG		=	new	SPWindowStatic( WIID_EMBLEMGUILD_BG	   , 0 , 0 , m_iSX , m_iSY , this );
	

	pWindowButton	=	new	SPWindowButton( WIID_EMBLEMGUILD_CLOSE , 313 , 9 , 12 , 12 , m_pWndBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 301, 321 ) ;
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 314, 321 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 327, 321 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 340, 321 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	// SYMBOL Select =============================================================================
	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_SYM_PAGE , 174 , 45 , 48 , 12 , m_pWndBG );
	pWindowStatic->SetFormat(DT_TOP | DT_CENTER);
	pWindowStatic->SetFontColor( RGBA(119, 76, 95, 255) ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_SYM_LEFT_TAG , 35 , 62 , 4 , 60 , m_pWndBG );
	pWindowStatic->SetWindowType(WND_TYPE_TOPBASE) ;
	pWindowStatic->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI107.PNG", 452, 457, 456, 461 ) ;
	pWindowStatic->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI107.PNG", 452, 462, 456, 466 ) ;
	pWindowStatic->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI107.PNG", 452, 467, 456, 471 ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_SYM_RIGHT_TAG , 219 , 62 , 4 , 60 , m_pWndBG );
	pWindowStatic->SetWindowType(WND_TYPE_TOPBASE) ;
	pWindowStatic->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI107.PNG", 457, 457, 461, 461 ) ;
	pWindowStatic->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI107.PNG", 457, 462, 461, 466 ) ;
	pWindowStatic->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI107.PNG", 457, 467, 461, 471 ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILDEMBLEM_SYM_LEFT , 18 , 86 , 9 , 13 , m_pWndBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 156, 165 ) ;
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 176, 165 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 176, 165 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 179, 106 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_GUILDEMBLEM_SYM_RIGHT , 232 , 86 , 9 , 13 , m_pWndBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 166, 165 ) ;
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 186, 165 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 206, 165 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 189, 106 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_SYM_SHOW , 270 , 68 , 24 , 24 , m_pWndBG );
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILDEMBLEM_SYM_COMMIT , 248 , 101 , 68 , 17 , m_pWndBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 198 ) ;
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 216 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 234 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 252 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	// BG Select =============================================================================
	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_BG_PAGE , 174 , 140 , 48 , 12 , m_pWndBG );
	pWindowStatic->SetFormat(DT_TOP | DT_CENTER);
	pWindowStatic->SetFontColor( RGBA(119, 76, 95, 255) ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_BG_LEFT_TAG , 35 , 157 , 4 , 60 , m_pWndBG );
	pWindowStatic->SetWindowType(WND_TYPE_TOPBASE) ;
	pWindowStatic->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI107.PNG", 452, 457, 456, 461 ) ;
	pWindowStatic->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI107.PNG", 452, 462, 456, 466 ) ;
	pWindowStatic->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI107.PNG", 452, 467, 456, 471 ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_BG_RIGHT_TAG , 219 , 157 , 4 , 60 , m_pWndBG );
	pWindowStatic->SetWindowType(WND_TYPE_TOPBASE) ;
	pWindowStatic->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI107.PNG", 457, 457, 461, 461 ) ;
	pWindowStatic->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI107.PNG", 457, 462, 461, 466 ) ;
	pWindowStatic->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI107.PNG", 457, 467, 461, 471 ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILDEMBLEM_BG_LEFT , 18 , 181 , 9 , 13 , m_pWndBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 156, 165 ) ;
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 176, 165 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 176, 165 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 179, 106 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_GUILDEMBLEM_BG_RIGHT , 232 , 181 , 9 , 13 , m_pWndBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 166, 165 ) ;
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 186, 165 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 206, 165 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 189, 106 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_BG_SHOW , 270 , 163 , 24 , 24 , m_pWndBG );
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILDEMBLEM_BG_COMMIT , 248 , 196 , 68 , 17 , m_pWndBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 198 ) ;
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 216 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 234 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 252 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	// FX Select =============================================================================
	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_FX_PAGE , 174 , 235 , 48 , 12 , m_pWndBG );
	pWindowStatic->SetFormat(DT_TOP | DT_CENTER);
	pWindowStatic->SetFontColor( RGBA(119, 76, 95, 255) ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_FX_LEFT_TAG , 35 , 252 , 4 , 60 , m_pWndBG );
	pWindowStatic->SetWindowType(WND_TYPE_TOPBASE) ;
	pWindowStatic->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI107.PNG", 452, 457, 456, 461 ) ;
	pWindowStatic->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI107.PNG", 452, 462, 456, 466 ) ;
	pWindowStatic->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI107.PNG", 452, 467, 456, 471 ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_FX_RIGHT_TAG , 219 , 252 , 4 , 60 , m_pWndBG );
	pWindowStatic->SetWindowType(WND_TYPE_TOPBASE) ;
	pWindowStatic->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI107.PNG", 457, 457, 461, 461 ) ;
	pWindowStatic->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI107.PNG", 457, 462, 461, 466 ) ;
	pWindowStatic->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI107.PNG", 457, 467, 461, 471 ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILDEMBLEM_FX_LEFT , 18 , 276 , 9 , 13 , m_pWndBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 156, 165 ) ;
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 176, 165 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 176, 165 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 179, 106 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_GUILDEMBLEM_FX_RIGHT , 232 , 276 , 9 , 13 , m_pWndBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 166, 165 ) ;
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 186, 165 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 206, 165 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 189, 106 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_FX_SHOW , 270 , 258 , 24 , 24 , m_pWndBG );
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowButton	=	new	SPWindowButton( WIID_GUILDEMBLEM_FX_COMMIT , 248 , 290 , 68 , 17 , m_pWndBG );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 198 ) ;
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 216 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 234 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 252 );
	pWindowButton->Hide() ;
	pWindowButton =	NULL;

	// ETC =============================================================================
	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_SHOW_BG , 35 , 328 , 264 , 61 , m_pWndBG );
	pWindowStatic->SetWindowType(WND_TYPE_LEFTBASE) ;
	pWindowStatic->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI107.PNG", 402, 352, 404, 413 ) ;
	pWindowStatic->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI107.PNG", 404, 352, 406, 413 ) ;
	pWindowStatic->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI107.PNG", 406, 357, 408, 413 ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_SHOW_DARK1 , 84 , 328 , 62 , 61 , m_pWndBG );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG", 409, 352 ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;
	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_SHOW_DARK2 , 188 , 328 , 62 , 61 , m_pWndBG );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG", 409, 352 ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;
	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_SHOW_ARROW , 156 , 348 , 22 , 22 , m_pWndBG );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG", 304, 339 ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;
	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_SHOW_TEXT1 , 86 , 376 , 58 , 9 , m_pWndBG );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG", 243, 342 ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;
	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_SHOW_TEXT2 , 190 , 376 , 58 , 9 , m_pWndBG );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG", 243, 352 ) ;
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_BEFORE_BG , 103 , 343 , 24 , 24 , m_pWndBG );
	pWindowStatic->Hide() ;
	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_BEFORE_SYM , 103 , 343 , 24 , 24 , m_pWndBG );
	pWindowStatic->Hide() ;
	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_BEFORE_FX , 103 , 343 , 24 , 24 , m_pWndBG );
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_NEXT_BG , 207 , 343 , 24 , 24 , m_pWndBG );
	pWindowStatic->Hide() ;
	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_NEXT_SYM , 207 , 343 , 24 , 24 , m_pWndBG );
	pWindowStatic->Hide() ;
	pWindowStatic	=	new	SPWindowStatic( WIID_GUILDEMBLEM_NEXT_FX , 207 , 343 , 24 , 24 , m_pWndBG );
	pWindowStatic->Hide() ;
	pWindowStatic	=	NULL ;
	
}


void SPWindowGuildEmblem::Init_ListWindow()
{
	int iX, iY ;
	for( int i = 0 ; i < 6 ; i++)
	{
		iX = 47 + (28 * i) ;
		iY = 64 ;
		m_pStaticSym[i]	=	new	SPWindowStatic( WIID_GUILDEMBLEM_SYM_ICON_LIST+ i	, iX , iY		, 24 , 24 , m_pWndBG );
		m_pStaticBG[i]	=	new	SPWindowStatic( WIID_GUILDEMBLEM_BG_ICON_LIST + i	, iX , iY + 97	, 24 , 24 , m_pWndBG );
		m_pStaticFX[i]	=	new	SPWindowStatic( WIID_GUILDEMBLEM_FX_ICON_LIST + i	, iX , iY + 192	, 24 , 24 , m_pWndBG );

		iY = 64 + 28 ;
		m_pStaticSym[i+6]	=	new	SPWindowStatic( WIID_GUILDEMBLEM_SYM_ICON_LIST+i+6	, iX , iY		, 24 , 24 , m_pWndBG );
		m_pStaticBG[i+6]	=	new	SPWindowStatic( WIID_GUILDEMBLEM_BG_ICON_LIST +i+6	, iX , iY + 97	, 24 , 24 , m_pWndBG );
		m_pStaticFX[i+6]	=	new	SPWindowStatic( WIID_GUILDEMBLEM_FX_ICON_LIST +i+6	, iX , iY + 192	, 24 , 24 , m_pWndBG );
	}
}


void SPWindowGuildEmblem::InitTooltipWindow()
{
	// ToolTip > BG > LIST
	SPWindowStatic* pWindowStatic = NULL ;
	m_pWindowToolTip	=	new	SPWindowStatic( WIID_GUILD_EMBLEM_TOOLTIP , 30 , 30 , 206 , 94 , m_pWndBG );
	m_pWindowToolTip->Hide() ;

	pWindowStatic	=	new	SPWindowStatic( WIID_GUILD_EMBLEM_TOOLTIP_BG , 0 , 0 , 206 , 94 , m_pWindowToolTip );
	pWindowStatic->SetWindowType(WND_TYPE_NINEBASE) ;
	pWindowStatic->SetImageNine("DATA/INTERFACE/CONCEPT/UI102.PNG", 252, 76, 8, 0) ;
	pWindowStatic->Hide() ;

	m_iListHeight = 90 ;
	m_pWindowTipNeed = new SPWindowNeedList(WIID_GUILD_EMBLEM_TOOLTIP_LIST, 2, 0, 204, 90, pWindowStatic ) ;
}


/**
	구입확인을 위한 Modal Window
*/
void SPWindowGuildEmblem::InitModalWindow()
{
	SPWindow* pWindow = NULL ;
	pWindow = new SPWindowBuyConfirm(WIID_EMBLEMGUILD_BUYCONFIRM, 180, 80, 260, 211, m_pWndModal) ;
	pWindow->Hide() ;
}


void SPWindowGuildEmblem::Show(bool bWithChild)
{
	
	// Only GuildMaster
	if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) && g_pGOBManager->GetGuildArchive()->GetBasicAvailable() && g_pGOBManager->GetGuildArchive()->IsMaster() )
	{
		SPGuildArchive* pkGuildArchive = g_pGOBManager->GetGuildArchive() ;
		SetShowUserEmblem(pkGuildArchive->GetGuildEmblem(0), pkGuildArchive->GetGuildEmblem(1), pkGuildArchive->GetGuildEmblem(2)) ;

		SPWindow::Show(bWithChild);
		m_pWndModal->Hide() ;							// Modal Hide
		ShowToolTip(LAYER_GUILD_SYM, 0, false) ;		// ToolTip Hide	
	}
	else
	{
		if( g_pResourceManager->GetGlobalString(27000006) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000006) );
		OnClose(0, 0) ;
		return ;
	}
}


void SPWindowGuildEmblem::Hide(bool bSendServer)
{
	m_pWndModal->Hide() ;
	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		OnCursorOut( NULL , NULL );
	}
	KillFocus() ;
}


void SPWindowGuildEmblem::ReposOwnImage()
{
	// == 105
	SetRect( &m_rcBGDest[0]	, m_iAX			, m_iAY			, m_iAX + 4		, m_iAY + 4 );		// 배경 7
	SetRect( &m_rcBGDest[1]	, m_iAX + 4		, m_iAY			, m_iAX + 330	, m_iAY + 4 );		// 배경 8
	SetRect( &m_rcBGDest[2]	, m_iAX + 330	, m_iAY 		, m_iAX + 334	, m_iAY + 4 );		// 배경 9
	SetRect( &m_rcBGDest[3]	, m_iAX			, m_iAY + 4		, m_iAX + 4		, m_iAY + 414 );	// 배경 4
	SetRect( &m_rcBGDest[4]	, m_iAX + 4		, m_iAY + 4		, m_iAX + 330	, m_iAY + 414 );	// 배경 5
	SetRect( &m_rcBGDest[5]	, m_iAX	+ 330	, m_iAY + 4		, m_iAX + 334	, m_iAY + 414 );	// 배경 6
	SetRect( &m_rcBGDest[6]	, m_iAX			, m_iAY + 414	, m_iAX + 4		, m_iAY + 418 );	// 배경 1
	SetRect( &m_rcBGDest[7]	, m_iAX	+ 4		, m_iAY + 414	, m_iAX + 330	, m_iAY + 418 );	// 배경 2
	SetRect( &m_rcBGDest[8]	, m_iAX + 330	, m_iAY + 414	, m_iAX + 334	, m_iAY + 418 );	// 배경 3
	SetRect( &m_rcBGDest[9], m_iAX + 2		, m_iAY	+ 27	, m_iAX + 332	, m_iAY + 414 );	//	Sub BG(White) 

	// === 107
	SetRect( &m_rcBGDest[10], m_iAX + 10	, m_iAY + 5		, m_iAX + 178	, m_iAY + 24 );		//	Title 
	SetRect( &m_rcBGDest[11], m_iAX	+ 2		, m_iAY	+ 26	, m_iAX + 332	, m_iAY + 27 );		//	Line 

	SetRect( &m_rcBGDest[12], m_iAX + 35	, m_iAY + 43	, m_iAX + 171	, m_iAY + 58 );		//	Symbol Select TEXT 
	SetRect( &m_rcBGDest[13], m_iAX	+ 174	, m_iAY + 43	, m_iAX + 222	, m_iAY + 58 );		//	Symbol Page Count BG 
	SetRect( &m_rcBGDest[14], m_iAX + 268	, m_iAY + 66	, m_iAX + 296	, m_iAY + 94 );		//	Symbol Show BG 
	SetRect( &m_rcBGDest[15], m_iAX	+ 10	, m_iAY + 129	, m_iAX + 324	, m_iAY + 130 );	//	Line 

	SetRect( &m_rcBGDest[16], m_iAX	+ 35	, m_iAY + 138	, m_iAX + 171	, m_iAY + 153 );	//	BG Select TEXT 
	SetRect( &m_rcBGDest[17], m_iAX + 174	, m_iAY + 138	, m_iAX + 222	, m_iAY + 153 );	//	BG Page Count BG 
	SetRect( &m_rcBGDest[18], m_iAX + 268	, m_iAY + 161	, m_iAX + 296	, m_iAY + 189 );	//	BG Show BG 
	SetRect( &m_rcBGDest[19], m_iAX + 10	, m_iAY + 224	, m_iAX + 324	, m_iAY + 225 );	//	Line 

	SetRect( &m_rcBGDest[20], m_iAX + 35	, m_iAY + 233	, m_iAX + 171	, m_iAY + 248 );	//	FX Select TEXT 
	SetRect( &m_rcBGDest[21], m_iAX + 174	, m_iAY + 233	, m_iAX + 222	, m_iAY + 248 );	//	FX Page Count BG 
	SetRect( &m_rcBGDest[22], m_iAX + 268	, m_iAY + 256	, m_iAX + 296	, m_iAY + 284 );	//	FX Show BG 
	

	// Icon BG Start
	SetRect( &m_rcIconBGDst[0], m_iAX + 47	, m_iAY + 82	, m_iAX + 71	, m_iAY + 88 );		//	Icon 배경 
	SetRect( &m_rcIconBGDst[1], m_iAX + 47	, m_iAY + 104	, m_iAX + 71	, m_iAY + 110 );	//	Icon 배경 
	SetRect( &m_rcIconBGDst[2], m_iAX + 47	, m_iAY + 179	, m_iAX + 71	, m_iAY + 185 );	//	Icon 배경 
	SetRect( &m_rcIconBGDst[3], m_iAX + 47	, m_iAY + 201	, m_iAX + 71	, m_iAY + 207 );	//	Icon 배경 
	SetRect( &m_rcIconBGDst[4], m_iAX + 47	, m_iAY + 274	, m_iAX + 71	, m_iAY + 280 );	//	Icon 배경 
	SetRect( &m_rcIconBGDst[5], m_iAX + 47	, m_iAY + 296	, m_iAX + 71	, m_iAY + 302 );	//	Icon 배경 
}

void SPWindowGuildEmblem::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowGuildEmblem::Process(float fTime)
{
	if( m_pkFXArchive != NULL )
		m_pkFXArchive->Process( fTime );

	SPWindow::Process( fTime );
}


void SPWindowGuildEmblem::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	RenderBackGround(fTime) ;

	SPWindow::Render(fTime);

	RenderAnimation(fTime) ;

	if( m_pkFXArchive != NULL )
		m_pkFXArchive->Render( fTime );

	// 한번더 랜더링( ToolTip & Modal)
	if( m_pWindowToolTip->IsShow() )
		m_pWindowToolTip->Render(fTime) ;
	if( m_pWndModal->IsShow() )
		m_pWndModal->Render(fTime) ;
}


/**
	배경 이미지 출력 
	** UI105.PNG, UI107.PNG 두개가 사용됨
*/
void SPWindowGuildEmblem::RenderBackGround(float fTime)
{
	int i ;
	RECT rtDstIconBG ;
	if( m_pBGTexture5 )	{
		for( i = 0; i < 10; i++ )
			m_pBGTexture5->RenderTexture(&m_rcBGDest[i], &m_rcBGSrc[i]) ;
	}
	g_pVideo->Flush();
	if( m_pBGTexture7 )	{
		for( i = 10; i < GUILD_EMBLEM_BG_IMG; i++ )
			m_pBGTexture7->RenderTexture(&m_rcBGDest[i], &m_rcBGSrc[i]) ;
	}
	g_pVideo->Flush();

	// Icon BG
	if( m_pBGTexture7 )	{
		for( i = 0; i < 6; i++ )	{
			int iInc = 28 * i ;
			if( m_pStaticSym[i]->IsShowEnable() ) {
				SetRect(&rtDstIconBG, m_rcIconBGDst[0].left+iInc, m_rcIconBGDst[0].top, m_rcIconBGDst[0].right+iInc, m_rcIconBGDst[0].bottom) ;
				m_pBGTexture7->RenderTexture(&rtDstIconBG, &m_rcIconBGSrc) ;
			}
			if( m_pStaticSym[i+6]->IsShowEnable() ) {
				SetRect(&rtDstIconBG, m_rcIconBGDst[1].left+iInc, m_rcIconBGDst[1].top, m_rcIconBGDst[1].right+iInc, m_rcIconBGDst[1].bottom) ;
				m_pBGTexture7->RenderTexture(&rtDstIconBG, &m_rcIconBGSrc) ;
			}
			
			if( m_pStaticBG[i]->IsShowEnable() ) {
				SetRect(&rtDstIconBG, m_rcIconBGDst[2].left+iInc, m_rcIconBGDst[2].top, m_rcIconBGDst[2].right+iInc, m_rcIconBGDst[2].bottom) ;
				m_pBGTexture7->RenderTexture(&rtDstIconBG, &m_rcIconBGSrc) ;
			}
			if( m_pStaticBG[i+6]->IsShowEnable() ) {
				SetRect(&rtDstIconBG, m_rcIconBGDst[3].left+iInc, m_rcIconBGDst[3].top, m_rcIconBGDst[3].right+iInc, m_rcIconBGDst[3].bottom) ;
				m_pBGTexture7->RenderTexture(&rtDstIconBG, &m_rcIconBGSrc) ;
			}

			if( m_pStaticFX[i]->IsShowEnable() ) {
				SetRect(&rtDstIconBG, m_rcIconBGDst[4].left+iInc, m_rcIconBGDst[4].top, m_rcIconBGDst[4].right+iInc, m_rcIconBGDst[4].bottom) ;
				m_pBGTexture7->RenderTexture(&rtDstIconBG, &m_rcIconBGSrc) ;
			}
			if( m_pStaticFX[i+6]->IsShowEnable() ) {
				SetRect(&rtDstIconBG, m_rcIconBGDst[5].left+iInc, m_rcIconBGDst[5].top, m_rcIconBGDst[5].right+iInc, m_rcIconBGDst[5].bottom) ;
				m_pBGTexture7->RenderTexture(&rtDstIconBG, &m_rcIconBGSrc) ;
			}
		}
	}
	g_pVideo->Flush();
}


/**
	Rendering : Select & Mouse Over Animation
*/
void SPWindowGuildEmblem::RenderAnimation(float fTime)
{
	if( m_mouseAni.bEnable )
	{
		m_mouseAni.Process(fTime) ;
		m_pBGTexture7->RenderTexture(&m_mouseAni.rcDst, &m_mouseAni.rcSrc[m_mouseAni.iCurFrame]) ;
	}
	for( int i = 0 ; i < LAYER_GUILD_MAX; i++)
	{
		if( m_selAni[i].bEnable && m_iShowPage[i] == m_iSelPage[i] )
		{
			m_selAni[i].Process(fTime) ;
			m_pBGTexture7->RenderTexture(&m_selAni[i].rcDst, &m_selAni[i].rcSrc[m_selAni[i].iCurFrame]) ;
		}
	}
	g_pVideo->Flush();
}


/**
	사용자가 가지고 있는 기본 Layer를 보여줘야 한다.
	기본으로 가지고 있는 Layer가 Select 된것으로 보여진다.
	## IconIndex -> Sort Index 로 변환 필요
*/
void SPWindowGuildEmblem::SetShowUserEmblem(int iSymIndex, int iBGIndex, int iFXIndex)
{
	int iIndex ;
	int iSortIndex ;
	EMBLEM_INFO* pEmblem = NULL ;

	g_pkGuildManager->FindEmblemSymbol( iSymIndex, pEmblem) ;
	iSortIndex = pEmblem->iSortIndex ;
	m_iMyEmblem[LAYER_GUILD_SYM] = iSortIndex ;
	iIndex = iSortIndex ? iSortIndex%MAX_ONEPAGE_SHOW : 0 ;
	SetBeforeEmblem(LAYER_GUILD_SYM, iSortIndex) ;
	GetEmblemLayerPage(LAYER_GUILD_SYM, IndexToPage(iSortIndex)) ;
	OnSymbolSelect( iIndex + WIID_GUILDEMBLEM_SYM_ICON_LIST, 0, 0) ;

	g_pkGuildManager->FindEmblemBG( iBGIndex, pEmblem) ;
	iSortIndex = pEmblem->iSortIndex ;
	m_iMyEmblem[LAYER_GUILD_BG] = iSortIndex ;
	iIndex = iSortIndex ? iSortIndex%MAX_ONEPAGE_SHOW : 0 ;
	SetBeforeEmblem(LAYER_GUILD_BG, iSortIndex) ;
	GetEmblemLayerPage(LAYER_GUILD_BG, IndexToPage(iSortIndex)) ;
	OnBGSelect( iIndex + WIID_GUILDEMBLEM_BG_ICON_LIST, 0, 0) ;

	g_pkGuildManager->FindEmblemFX( iFXIndex, pEmblem) ;
	iSortIndex = pEmblem->iSortIndex ;
	m_iMyEmblem[LAYER_GUILD_FX] = iSortIndex ;
	iIndex = iSortIndex ? iSortIndex%MAX_ONEPAGE_SHOW : 0 ;
	SetBeforeEmblem(LAYER_GUILD_FX, iSortIndex) ;
	GetEmblemLayerPage(LAYER_GUILD_FX, IndexToPage(iSortIndex)) ;
	OnFXSelect( iIndex + WIID_GUILDEMBLEM_FX_ICON_LIST, 0, 0) ;
}


/**
	하단의 Before에 보이는 엠블렘 변경
*/
void SPWindowGuildEmblem::SetBeforeEmblem(EMBLEM_LAYER eLayer, int iIndex)
{
	SPWindowStatic* pWindowStatic = NULL ;
	EMBLEM_INFO* pEmblem = NULL ;
	g_pkGuildManager->FindSortEmblemInfo(eLayer, iIndex, pEmblem) ;
	if( pEmblem == NULL || g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID) == NULL )
		return ;

	switch( eLayer )
	{
	case LAYER_GUILD_SYM:
		pWindowStatic = (SPWindowStatic*)Find(WIID_GUILDEMBLEM_BEFORE_SYM, true) ;
		if( pWindowStatic ) {
			pWindowStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
			pWindowStatic->SetImage( g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID), GetIconSrcX(pEmblem->iResIconIndex), GetIconSrcY(pEmblem->iResIconIndex)) ;
			m_iMyEmblem[LAYER_GUILD_SYM] = iIndex ;
		}
		break ;
	case LAYER_GUILD_BG:
		pWindowStatic = (SPWindowStatic*)Find(WIID_GUILDEMBLEM_BEFORE_BG, true) ;
		if( pWindowStatic ) {
			pWindowStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
			pWindowStatic->SetImage( g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID), GetIconSrcX(pEmblem->iResIconIndex), GetIconSrcY(pEmblem->iResIconIndex)) ;
			m_iMyEmblem[LAYER_GUILD_BG] = iIndex ;
		}
		break ;
	case LAYER_GUILD_FX:
		ClearEmblemFX() ;
		SetBeforeEmblemFx(pEmblem->iResFxID) ;
		SetAfterEmblemFx(m_iPreSelectFxID) ;
		m_iMyEmblem[LAYER_GUILD_FX] = iIndex ;
		break ;
	}
}


int	SPWindowGuildEmblem::GetIconSrcX(int iIndex)
{
	return iIndex ? (iIndex%MAX_ICON_HORIZEN)*EMBLEM_ICON_SIZE : 0 ;
}
int	SPWindowGuildEmblem::GetIconSrcY(int iIndex)
{
	return iIndex ? (iIndex/MAX_ICON_HORIZEN)*EMBLEM_ICON_SIZE : 0 ;
}


/**
	Layer와 Page를 가지고 GuildManager에서 해당 목록을 가져온다.
*/
bool SPWindowGuildEmblem::GetEmblemLayerPage(EMBLEM_LAYER eLayer, int iNextPage)
{
	int iTotalSize = g_pkGuildManager->GetEmblemCount(eLayer) ;
	if( MAX_ONEPAGE_SHOW * iNextPage > iTotalSize )
		return false ;

	int iMaxPage = iTotalSize / MAX_ONEPAGE_SHOW + 1 ;			// iTotalSize != 0 
	int iGetStart = iNextPage * MAX_ONEPAGE_SHOW ;
	int iGetEnd = min((iNextPage+1)*MAX_ONEPAGE_SHOW, iTotalSize) ;
	int iGetCount = iGetEnd - iGetStart ;
	
	SPWindow* pWindow = NULL ;
	TCHAR szPage[8] ;
	_stprintf(szPage, "%02d/%02d", iNextPage+1, iMaxPage) ;
	
	m_iShowPage[eLayer] = iNextPage ;
	if( eLayer == LAYER_GUILD_SYM )	{
		pWindow = Find(WIID_GUILDEMBLEM_SYM_PAGE, true) ;
		if( pWindow )	pWindow->SetWindowText(szPage) ;
		return GetSymbolPage(iGetStart, iGetEnd) ;
	}
	else if( eLayer == LAYER_GUILD_BG )	{
		pWindow = Find(WIID_GUILDEMBLEM_BG_PAGE, true) ;
		if( pWindow )	pWindow->SetWindowText(szPage) ;
		return GetBGPage(iGetStart, iGetEnd) ;
	}
	else if( eLayer == LAYER_GUILD_FX)	{
		pWindow = Find(WIID_GUILDEMBLEM_FX_PAGE, true) ;
		if( pWindow )	pWindow->SetWindowText(szPage) ;
		return GetFXPage(iGetStart, iGetEnd) ;
	}
	
	return false ;
}
bool SPWindowGuildEmblem::GetSymbolPage(int iGetStart, int iGetEnd)
{
	int iIndex = 0 ;
	EMBLEM_INFO* pEmblem = NULL ;
	SPWindow* pCurWindow = NULL ;

	iIndex = iGetStart ;
	for(  int j = 0; j < MAX_ONEPAGE_SHOW; j++ )
	{
		g_pkGuildManager->FindSortSymbol(iIndex, pEmblem) ;
		if( pEmblem == NULL || (g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID) == NULL) || iIndex >= iGetEnd )
		{
			m_pStaticSym[j]->SPSendMessage(SPIM_SET_IMAGECLEAR);
			m_pStaticSym[j]->SetShowEnable(false) ;
			++iIndex ;
			continue ;
		}
		m_pStaticSym[j]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pStaticSym[j]->SetImage( g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID), GetIconSrcX(pEmblem->iResIconIndex), GetIconSrcY(pEmblem->iResIconIndex)) ;
		m_pStaticSym[j]->SetShowEnable(true) ;
		m_pStaticSym[j]->Show() ;
		++iIndex ;
	}

	return true ;
}
bool SPWindowGuildEmblem::GetBGPage(int iGetStart, int iGetEnd)
{
	int iIndex = 0 ;
	EMBLEM_INFO* pEmblem = NULL ;
	SPWindow* pCurWindow = NULL ;

	iIndex = iGetStart ;
	for( int j = 0; j < MAX_ONEPAGE_SHOW; j++ )
	{
		g_pkGuildManager->FindSortBG(iIndex, pEmblem) ;
		if( pEmblem == NULL || (g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID) == NULL) || iIndex >= iGetEnd )
		{
			m_pStaticBG[j]->SPSendMessage(SPIM_SET_IMAGECLEAR);
			m_pStaticBG[j]->SetShowEnable(false) ;
			++iIndex ;
			continue ;
		}
		m_pStaticBG[j]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pStaticBG[j]->SetImage( g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID), GetIconSrcX(pEmblem->iResIconIndex), GetIconSrcY(pEmblem->iResIconIndex)) ;
		m_pStaticBG[j]->SetShowEnable(true) ;
		m_pStaticBG[j]->Show() ;
		++iIndex ;
	}
	return true ;
}
bool SPWindowGuildEmblem::GetFXPage(int iGetStart, int iGetEnd)
{
	int iIndex = 0 ;
	EMBLEM_INFO* pEmblem = NULL ;
	SPWindow* pCurWindow = NULL ;

	iIndex = iGetStart ;
	for( int j = 0; j < MAX_ONEPAGE_SHOW; j++ )
	{
		g_pkGuildManager->FindSortFX(iIndex, pEmblem) ;
		if( pEmblem == NULL || (g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID) == NULL) || iIndex >= iGetEnd )
		{
			m_pStaticFX[j]->SPSendMessage(SPIM_SET_IMAGECLEAR);
			m_pStaticFX[j]->SetShowEnable(false) ;
			++iIndex ;
			continue ;
		}
		m_pStaticFX[j]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pStaticFX[j]->SetImage( g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID), GetIconSrcX(pEmblem->iResIconIndex), GetIconSrcY(pEmblem->iResIconIndex)) ;
		m_pStaticFX[j]->SetShowEnable(true) ;
		m_pStaticFX[j]->Show() ;
		++iIndex ;
	}
	return true ;
}


/**
	WindowInstanceID를 통해서 MouseOver 컨트롤 좌표에 Animation 출력
*/
void SPWindowGuildEmblem::SetMouseAnimation(int iID) 
{
	RECT rcDst ;
	SPWindow* pWindow = Find(iID, true) ;
	if( pWindow == NULL )
		return ;

	if( pWindow->IsShowEnable() == false )
		return ;

	// 애니메이션 될 좌표를 셋팅해야 함.
	pWindow->GetWindowRect(rcDst) ;
	SetRect(&m_mouseAni.rcDst, rcDst.left-2, rcDst.top-2, rcDst.right+2, rcDst.bottom+2) ;
	m_mouseAni.bEnable = true ;
}


/**
	ToolTip을 보여준다.
	@param eLayer : 선택 Layer
	@param int iWndID : Find Index ( Index + CurrentPage*One_Page )
	@param bool bShow : Show or Hide
*/
void SPWindowGuildEmblem::ShowToolTip(EMBLEM_LAYER eLayer, int iWndID, bool bShow)
{
	if( bShow )
	{
		TCHAR	szText[64] ;
		RECT	rcSrc ;
		int		iIndex = 0 ;
		int		iEmblemID = 0 ;
		EMBLEM_INFO* pEmblem = NULL ;
		SPWindowNeedList* pNeedList = NULL ;
		
		if( eLayer == LAYER_GUILD_SYM ) {
			iIndex = iWndID - WIID_GUILDEMBLEM_SYM_ICON_LIST ;
			iEmblemID = m_iShowPage[LAYER_GUILD_SYM] * MAX_ONEPAGE_SHOW + iIndex ;
		}
		else if( eLayer == LAYER_GUILD_BG )		{
			iIndex = iWndID - WIID_GUILDEMBLEM_BG_ICON_LIST ;
			iEmblemID = m_iShowPage[LAYER_GUILD_BG] * MAX_ONEPAGE_SHOW + iIndex ;
		}
		else if( eLayer == LAYER_GUILD_FX )		{
			iIndex = iWndID - WIID_GUILDEMBLEM_FX_ICON_LIST ;
			iEmblemID = m_iShowPage[LAYER_GUILD_FX] * MAX_ONEPAGE_SHOW + iIndex ;
		}

		g_pkGuildManager->FindSortEmblemInfo(eLayer, iEmblemID, pEmblem) ;
		if( !pEmblem || !m_pWindowTipNeed)		{
			return ;
		}
		pNeedList = (SPWindowNeedList*)m_pWindowTipNeed ;
		pNeedList->ClearListItem() ;

		// Item
		if( pEmblem->iConsumeItem > 0 && pEmblem->iConsumeItemCount > 0 )
			pNeedList->InsertItem(pEmblem->iConsumeItem, pEmblem->iConsumeItemCount) ;
		
		// Guild Level
		if( g_pResourceManager->GetGlobalString(27000106) )
			_stprintf(szText, g_pResourceManager->GetGlobalString(27000106), pEmblem->iMinLevel, pEmblem->iMaxLevel) ;
		else
			_stprintf(szText, "Lv.%d ~ Lv.%d", pEmblem->iMinLevel, pEmblem->iMaxLevel) ;
		SetRect(&rcSrc, 102, 199, 138, 212) ;
		pNeedList->InsertItem("DATA/INTERFACE/CONCEPT/UI105.PNG", rcSrc, szText ) ;

		// Ely
		if( pEmblem->iConsumeEly > 0 )
		{
			if( g_pResourceManager->GetGlobalString(27000099) )
				_stprintf(szText, "%s %s", ConvertMoneyToString(pEmblem->iConsumeEly), g_pResourceManager->GetGlobalString(27000099)) ;
			else
				_stprintf(szText, "%s Ely", ConvertMoneyToString(pEmblem->iConsumeEly)) ;
			SetRect(&rcSrc, 369, 280, 405, 293) ;
			pNeedList->InsertItem("DATA/INTERFACE/CONCEPT/UI105.PNG", rcSrc, szText ) ;
		}

		// WindowPos를 받아서 ToolTip 좌표 셋팅 필요함 -> Resize Message 처리 필요
		SPWindow* pWindowMouse = NULL ;
		pWindowMouse = Find(iWndID, true) ;
		if( pWindowMouse == NULL )
			return ;
		int iPosX = 0, iPosY = 0 ;
		pWindowMouse->GetRelCoord(iPosX, iPosY) ;

		if( m_pWindowToolTip )	{
			m_pWindowToolTip->AdjustCoord(iPosX + 16, iPosY + 32 ) ;
			m_pWindowToolTip->RefreshChildPos() ;
			m_pWindowTipNeed->RefreshRelationCoord() ;
			m_pWindowToolTip->Show() ;
		}
	}
	else
	{
		if( m_pWindowToolTip )
			m_pWindowToolTip->Hide() ;
	}
}



/**
	엠블렘 변경 체크
	1: Guild Level
	2: Guild Money
	3: Item
*/
int SPWindowGuildEmblem::CheckGuildEmblemChange( EMBLEM_LAYER eLayer , int& iRetValue)
{
	int iEmblemID = 0 ;
	EMBLEM_INFO* pEmblem = NULL ;
	SPWindowBuyConfirm* pBuyConfirm = NULL;
	TCHAR szMsg[256] ;

	if( eLayer == LAYER_GUILD_SYM ) {
		iEmblemID = m_iSelPage[LAYER_GUILD_SYM]*MAX_ONEPAGE_SHOW + m_iSelIndex[LAYER_GUILD_SYM] ;
		g_pkGuildManager->FindSortSymbol(iEmblemID, pEmblem) ;
	}
	else if( eLayer == LAYER_GUILD_BG )		{
		iEmblemID = m_iSelPage[LAYER_GUILD_BG]*MAX_ONEPAGE_SHOW + m_iSelIndex[LAYER_GUILD_BG] ;
		g_pkGuildManager->FindSortBG(iEmblemID, pEmblem) ;
	}
	else if( eLayer == LAYER_GUILD_FX )		{
		iEmblemID = m_iSelPage[LAYER_GUILD_FX]*MAX_ONEPAGE_SHOW + m_iSelIndex[LAYER_GUILD_FX] ;
		g_pkGuildManager->FindSortFX(iEmblemID, pEmblem) ;
	}

	if( pEmblem == NULL )
		return -1 ;

	// Check Require
	for( int i = 0; i < EMBLEM_REQUIRE_MAX; i++)
	{
		if( false == g_pCheckManager->CheckCondition(pEmblem->stRequire[i].iRequireType
			, pEmblem->stRequire[i].iRequireID
			, pEmblem->stRequire[i].iRequireValue1
			, pEmblem->stRequire[i].iRequireValue2) )
		{
			iRetValue = pEmblem->iConsumeItem ;
			return i + 1 ;
		}
	}

	pBuyConfirm = (SPWindowBuyConfirm*)Find(WIID_EMBLEMGUILD_BUYCONFIRM, true) ;
	if( pBuyConfirm )
	{
		m_pBuyEmblem = pEmblem ;
		if( pBuyConfirm->SetNeedList(m_pBuyEmblem) == true )
			m_pWndModal->Show() ;
	}
	else
	{
		_stprintf(szMsg, "Can't Create Emblem Confirm Window");
		g_pInterfaceManager->SetNotice(szMsg, this, NOTICE_TYPE_YES);
	}

	return GUILD_CREATE_OK ;
}



/**
	Emblem FX Clear
 */
void SPWindowGuildEmblem::ClearEmblemFX()
{
	if( m_pkFXArchive ) {
		m_pkFXArchive->Clean() ;
		m_pkFXArchive->SetNULL() ;
	}
}

/**
	Before FX Setting
	두개가 같은경우 Delete에 문제가 있어서 따로 처리함
*/
void SPWindowGuildEmblem::SetBeforeEmblemFx(int iFxID)
{
	RECT rcDst ;
	SPWindow* pWindow = NULL ;
	bool bReInsert = false ;

	// Bottom BeforeView
	pWindow = Find(WIID_GUILDEMBLEM_BEFORE_FX, true) ;
	if( pWindow )	{
		pWindow->GetWindowRect(rcDst) ;
		if( iFxID )	{
			int iMidX = (rcDst.left + rcDst.right) / 2 ;
			int iMidY = (rcDst.top + rcDst.bottom) / 2 ;
			// 카메라 좌표를 포함한 절대 좌표를 넘겨주어야 한다.
			m_pkFXArchive->SetOffsetPos(iMidX+g_pCameraManagr->GetCameraStruct()->fOffsetX, iMidY+g_pCameraManagr->GetCameraStruct()->fOffsetY) ;
			m_pkFXArchive->InsertFXGroup( (SPID_FX_GROUP)iFxID ) ;
		}
		m_iPreBeforeFxID = iFxID ;		// For Change
	}
}
/**
	After FX Setting
	두개가 같은경우 Delete에 문제가 있어서 따로 처리함
*/
void SPWindowGuildEmblem::SetAfterEmblemFx(int iFxID)
{
	RECT rcDst ;
	SPWindow* pWindow = NULL ;
	bool bReInsert = false ;

	// Bottom NextPreView
	pWindow = Find(WIID_GUILDEMBLEM_NEXT_FX, true) ;
	if( pWindow )	{
		pWindow->GetWindowRect(rcDst) ;
		if( iFxID )	{
			int iMidX = (rcDst.left + rcDst.right) / 2 ;
			int iMidY = (rcDst.top + rcDst.bottom) / 2 ;
			// 카메라 좌표를 포함한 절대 좌표를 넘겨주어야 한다.
			m_pkFXArchive->SetOffsetPos(iMidX+g_pCameraManagr->GetCameraStruct()->fOffsetX, iMidY+g_pCameraManagr->GetCameraStruct()->fOffsetY) ;
			m_pkFXArchive->InsertFXGroup( (SPID_FX_GROUP)iFxID ) ;
		}
		m_iPreSelectFxID = iFxID ;
	}
}



void SPWindowGuildEmblem::SetErrorNoticeDlg(int iErrorNo, int iNeedItemID /*= 0*/)
{
	int iMsgNo = 0 ;
	TCHAR szMsg[256];
	std::string strItemName ;
	strItemName.clear() ;

	if( iErrorNo == 1 )			{	// Guild Level
		iMsgNo = 27000111 ;
	}
	else if( iErrorNo == 2 )	{	// Money
		iMsgNo = 27000113 ;
	}
	else if( iErrorNo == 3 )	{	// Item ==> Name Set
		SPItemAttr* pItem = NULL ;
		g_pItemCluster->GetItemInfo(iNeedItemID, pItem) ;
		if( pItem )
			strItemName = pItem->m_strName ;
		iMsgNo = 27000112 ;
	}
	else	{
		iMsgNo = 27000006 ;		// Default
	}

	if( g_pResourceManager->GetGlobalString(iMsgNo) ){
		if( strItemName.empty() )
			_stprintf(szMsg, g_pResourceManager->GetGlobalString(iMsgNo));
		else
			_stprintf(szMsg, g_pResourceManager->GetGlobalString(iMsgNo), strItemName.c_str());
	}
	else
		_stprintf(szMsg, "[%d]Guild Emblem Change Error[%d]", iMsgNo, iErrorNo);

	g_pInterfaceManager->SetNotice(szMsg, this, NOTICE_TYPE_YES);
}


SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowGuildEmblem )

SPIMESSAGE_COMMAND(SPIM_GUILD_EM_CHANGE_RESULT	, OnChangeResult	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP			, WIID_EMBLEMGUILD_CLOSE		,	OnClose		)

SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP	, WIID_GUILDEMBLEM_SYM_ICON_LIST	, WIID_GUILDEMBLEM_SYM_ICON_LIST+MAX_ONEPAGE_SHOW,	OnSymbolSelect	)
SPIMESSAGE_CONTROL_RANGE(SPIM_CURSOR_ENTER	, WIID_GUILDEMBLEM_SYM_ICON_LIST	, WIID_GUILDEMBLEM_SYM_ICON_LIST+MAX_ONEPAGE_SHOW,	OnSymCursorEnter	)
SPIMESSAGE_CONTROL_RANGE(SPIM_CURSOR_OUT	, WIID_GUILDEMBLEM_SYM_ICON_LIST	, WIID_GUILDEMBLEM_SYM_ICON_LIST+MAX_ONEPAGE_SHOW,	OnSymCursorOut	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	, WIID_GUILDEMBLEM_SYM_LEFT		,	OnSymbolLeft	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	, WIID_GUILDEMBLEM_SYM_RIGHT	,	OnSymbolRight	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	, WIID_GUILDEMBLEM_SYM_COMMIT	,	OnSymbolCommit	)

SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP	, WIID_GUILDEMBLEM_BG_ICON_LIST	, WIID_GUILDEMBLEM_BG_ICON_LIST+MAX_ONEPAGE_SHOW,	OnBGSelect	)
SPIMESSAGE_CONTROL_RANGE(SPIM_CURSOR_ENTER	, WIID_GUILDEMBLEM_BG_ICON_LIST	, WIID_GUILDEMBLEM_BG_ICON_LIST+MAX_ONEPAGE_SHOW,	OnBGCursorEnter	)
SPIMESSAGE_CONTROL_RANGE(SPIM_CURSOR_OUT	, WIID_GUILDEMBLEM_BG_ICON_LIST	, WIID_GUILDEMBLEM_BG_ICON_LIST+MAX_ONEPAGE_SHOW,	OnBGCursorOut	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	, WIID_GUILDEMBLEM_BG_LEFT		,	OnBGLeft	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	, WIID_GUILDEMBLEM_BG_RIGHT		,	OnBGRight	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	, WIID_GUILDEMBLEM_BG_COMMIT	,	OnBGCommit	)

SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP	, WIID_GUILDEMBLEM_FX_ICON_LIST	, WIID_GUILDEMBLEM_FX_ICON_LIST+MAX_ONEPAGE_SHOW,	OnFXSelect	)
SPIMESSAGE_CONTROL_RANGE(SPIM_CURSOR_ENTER	, WIID_GUILDEMBLEM_FX_ICON_LIST	, WIID_GUILDEMBLEM_FX_ICON_LIST+MAX_ONEPAGE_SHOW,	OnFXCursorEnter	)
SPIMESSAGE_CONTROL_RANGE(SPIM_CURSOR_OUT	, WIID_GUILDEMBLEM_FX_ICON_LIST	, WIID_GUILDEMBLEM_FX_ICON_LIST+MAX_ONEPAGE_SHOW,	OnFXCursorOut	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	, WIID_GUILDEMBLEM_FX_LEFT		,	OnFXLeft	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	, WIID_GUILDEMBLEM_FX_RIGHT		,	OnFXRight	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	, WIID_GUILDEMBLEM_FX_COMMIT	,	OnFXCommit	)

SPIMESSAGE_CONTROL(SPIM_LIST_RESIZE			, WIID_GUILD_EMBLEM_TOOLTIP_LIST	, OnListResize	)
SPIMESSAGE_CONTROL(SPIM_GUILD_EM_CONFIRM_YES, WIID_EMBLEMGUILD_BUYCONFIRM		, OnBuyYes	)
SPIMESSAGE_CONTROL(SPIM_GUILD_EM_CONFIRM_NO	, WIID_EMBLEMGUILD_BUYCONFIRM		, OnBuyNo	)

SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP	, WIID_EMBLEMGUILD_CLOSE	, WIID_EMBLEMGUILD_END		,	OnTest	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)




int SPWindowGuildEmblem::OnClose( WPARAM wParam , LPARAM lParam  )
{	
	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	Hide() ;
	return 1 ;
}


/**
	엠블렘 변경에 성공한 경우
	@param wParam : Changed Layer
	GuildArchive에 저장된 IconIndex를 통해서 데이터를 찾아서 SortIndex 로 설정한다.
	## IconIndex -> Sort Index 로 변환 필요
*/
int SPWindowGuildEmblem::OnChangeResult( WPARAM wParam , LPARAM lParam  )
{
	char szTemp[512] ;
	int iReault = (int)wParam ;
	EMBLEM_INFO* pEmblem = NULL ;
	

	m_pWndModal->Hide() ;
	if( iReault < 0 )		return 0 ;			// Error Close Modal

	if( iReault == 0 )		// Guild Symbol
	{
		g_pkGuildManager->FindEmblemSymbol( g_pGOBManager->GetGuildArchive()->GetGuildEmblem(LAYER_GUILD_SYM), pEmblem) ;
		SetBeforeEmblem(LAYER_GUILD_SYM, pEmblem->iSortIndex) ;
		if( g_pResourceManager->GetGlobalString(27000114) )
			sprintf(szTemp, g_pResourceManager->GetGlobalString(27000114));
		else
			sprintf(szTemp, "[27000114]Guild Emblem Symbol Change Success");
	}
	else if(iReault == 1 )	// Guild BG
	{
		g_pkGuildManager->FindEmblemBG( g_pGOBManager->GetGuildArchive()->GetGuildEmblem(LAYER_GUILD_BG), pEmblem) ;
		SetBeforeEmblem(LAYER_GUILD_BG, pEmblem->iSortIndex) ;
		if( g_pResourceManager->GetGlobalString(27000115) )
			sprintf(szTemp, g_pResourceManager->GetGlobalString(27000115));
		else
			sprintf(szTemp, "[27000115]Guild Emblem BG Change Success");
			
	}
	else if( iReault == 2 )	// Guild FX
	{
		g_pkGuildManager->FindEmblemFX( g_pGOBManager->GetGuildArchive()->GetGuildEmblem(LAYER_GUILD_FX), pEmblem) ;
		SetBeforeEmblem(LAYER_GUILD_FX, pEmblem->iSortIndex) ;
		if( g_pResourceManager->GetGlobalString(27000116) )
			sprintf(szTemp, g_pResourceManager->GetGlobalString(27000116));
		else
			sprintf(szTemp, "[27000116]Guild Emblem FX Change Success");
	}
	else
	{
		sprintf(szTemp, "Guild Emblem Change Success Layer[%d]", iReault);
	}

	if( g_pGOBManager->GetGuildArchive()->IsMaster() )
		g_pInterfaceManager->SetNotice(szTemp, this, NOTICE_TYPE_YES);
	else
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
	return 1 ;
}


int SPWindowGuildEmblem::OnTest( unsigned int iID, WPARAM wParam , LPARAM lParam  )
{
	int iIndex = iID - WIID_GUILD_EMBLEM ;
	return 1 ;
}


// SYMBOL ===================================================================
/** 
	심볼 선택 - 해당 심볼 출력
	1) 이전 선택 비선택
	2) 현재 선택 Animation
	3) 미리보기 변경
*/
int SPWindowGuildEmblem::OnSymbolSelect( unsigned int iID, WPARAM wParam , LPARAM lParam  )
{
	int iSelectIndex =  iID - WIID_GUILDEMBLEM_SYM_ICON_LIST ;
	int iSelectPage = m_iShowPage[LAYER_GUILD_SYM] ;
	int iMaxIndex = g_pkGuildManager->GetEmblemCount(LAYER_GUILD_SYM) ;

	if( iSelectPage*MAX_ONEPAGE_SHOW + iSelectIndex >= iMaxIndex )
		return 0 ;

	m_iSelIndex[LAYER_GUILD_SYM] = iSelectIndex ;
	m_iSelPage[LAYER_GUILD_SYM]	= iSelectPage ;

	int iSrcID = m_iSelPage[LAYER_GUILD_SYM]*MAX_ONEPAGE_SHOW + m_iSelIndex[LAYER_GUILD_SYM];
	SPWindowStatic* pWindowStatic = (SPWindowStatic*)Find(WIID_GUILDEMBLEM_SYM_SHOW, true) ;
	EMBLEM_INFO* pEmblem = NULL ;
	g_pkGuildManager->FindSortSymbol(iSrcID, pEmblem) ;
	if( pWindowStatic && pEmblem && g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID) )
	{
		pWindowStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
		pWindowStatic->SetImage( g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID), GetIconSrcX(pEmblem->iResIconIndex), GetIconSrcY(pEmblem->iResIconIndex)) ;
		pWindowStatic->SetShowEnable(true) ;
		pWindowStatic->Show() ;
	}

	// Bottom PreView
	pWindowStatic = (SPWindowStatic*)Find(WIID_GUILDEMBLEM_NEXT_SYM, true) ;
	if( pWindowStatic )	{
		pWindowStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
		pWindowStatic->SetImage( g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID), GetIconSrcX(pEmblem->iResIconIndex), GetIconSrcY(pEmblem->iResIconIndex)) ;
	}

	RECT rcDst ;
	SPWindow* pWindow = Find(iID, true) ;
	if( pWindow == NULL )
		return 1 ;

	// 애니메이션 될 좌표를 셋팅해야 함.
	pWindow->GetWindowRect(rcDst) ;
	SetRect(&m_selAni[LAYER_GUILD_SYM].rcDst, rcDst.left-7, rcDst.top-7, rcDst.left+12, rcDst.top+12) ;
	m_selAni[LAYER_GUILD_SYM].bEnable = true ;
	return 1 ;
}


int SPWindowGuildEmblem::OnSymbolRight( WPARAM wParam , LPARAM lParam  )
{
	int iSymCount = g_pkGuildManager->GetEmblemCount(LAYER_GUILD_SYM) ;
	if( iSymCount < 1 )		
		return 0 ;
	int iMaxPage = (iSymCount % MAX_ONEPAGE_SHOW) ? iSymCount/MAX_ONEPAGE_SHOW + 1 : iSymCount/MAX_ONEPAGE_SHOW ;
	if( m_iShowPage[LAYER_GUILD_SYM] + 1 < iMaxPage )	// MAX_PAGE
		GetEmblemLayerPage(LAYER_GUILD_SYM, m_iShowPage[LAYER_GUILD_SYM] + 1) ;

	return 1 ;
}

int SPWindowGuildEmblem::OnSymbolLeft( WPARAM wParam , LPARAM lParam  )
{
	if( m_iShowPage[LAYER_GUILD_SYM] - 1 >= 0 )	// Min_PAGE
		GetEmblemLayerPage(LAYER_GUILD_SYM, m_iShowPage[LAYER_GUILD_SYM] - 1) ;
	return 1 ;
}

int SPWindowGuildEmblem::OnSymbolCommit( WPARAM wParam , LPARAM lParam  )
{
	int iCommitIndex = m_iSelPage[LAYER_GUILD_SYM]*MAX_ONEPAGE_SHOW + m_iSelIndex[LAYER_GUILD_SYM] ;
	if( m_iMyEmblem[LAYER_GUILD_SYM] == iCommitIndex )
		return 1 ;

	int iRet = 0 ;
	int iRetValue = 0 ;
	if( (iRet = CheckGuildEmblemChange(LAYER_GUILD_SYM, iRetValue)) != GUILD_CREATE_OK )
	{
		SetErrorNoticeDlg(iRet, iRetValue);
	}

	return 1 ;
}

// BG ===================================================================
/**
	배경 선택 - 해당 배경 출력
*/
int SPWindowGuildEmblem::OnBGSelect( unsigned int iID, WPARAM wParam , LPARAM lParam  )
{
	int iSelectIndex =  iID - WIID_GUILDEMBLEM_BG_ICON_LIST ;
	int iSelectPage = m_iShowPage[LAYER_GUILD_BG] ;
	int iMaxIndex = g_pkGuildManager->GetEmblemCount(LAYER_GUILD_BG) ;

	if( iSelectPage*MAX_ONEPAGE_SHOW + iSelectIndex >= iMaxIndex )
		return 0 ;

	m_iSelIndex[LAYER_GUILD_BG] = iSelectIndex ;
	m_iSelPage[LAYER_GUILD_BG]	= iSelectPage ;

	int iSrcID = m_iSelPage[LAYER_GUILD_BG]*MAX_ONEPAGE_SHOW + m_iSelIndex[LAYER_GUILD_BG];
	SPWindowStatic* pWindowStatic = (SPWindowStatic*)Find(WIID_GUILDEMBLEM_BG_SHOW, true) ;
	EMBLEM_INFO* pEmblem = NULL ;
	g_pkGuildManager->FindSortBG(iSrcID, pEmblem) ;
	if( pWindowStatic && pEmblem && g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID) )
	{
		pWindowStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
		pWindowStatic->SetImage( g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID), GetIconSrcX(pEmblem->iResIconIndex), GetIconSrcY(pEmblem->iResIconIndex)) ;
		pWindowStatic->SetShowEnable(true) ;
		pWindowStatic->Show() ;
	}

	// Bottom PreView
	pWindowStatic = (SPWindowStatic*)Find(WIID_GUILDEMBLEM_NEXT_BG, true) ;
	if( pWindowStatic )	{
		pWindowStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
		pWindowStatic->SetImage( g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID), GetIconSrcX(pEmblem->iResIconIndex), GetIconSrcY(pEmblem->iResIconIndex)) ;
	}

	RECT rcDst ;
	SPWindow* pWindow = Find(iID, true) ;
	if( pWindow == NULL )
		return 1 ;

	// 애니메이션 될 좌표를 셋팅해야 함.
	pWindow->GetWindowRect(rcDst) ;
	SetRect(&m_selAni[LAYER_GUILD_BG].rcDst, rcDst.left-7, rcDst.top-7, rcDst.left+12, rcDst.top+12) ;
	m_selAni[LAYER_GUILD_BG].bEnable = true ;

	return 1 ;
}

int SPWindowGuildEmblem::OnBGRight( WPARAM wParam , LPARAM lParam  )
{
	int iSymCount = g_pkGuildManager->GetEmblemCount(LAYER_GUILD_BG) ;
	if( iSymCount < 1 )		
		return 0 ;
	int iMaxPage = (iSymCount % MAX_ONEPAGE_SHOW) ? iSymCount/MAX_ONEPAGE_SHOW + 1 : iSymCount/MAX_ONEPAGE_SHOW ;
	if( m_iShowPage[LAYER_GUILD_BG] + 1 < iMaxPage )	// MAX_PAGE
		GetEmblemLayerPage(LAYER_GUILD_BG, m_iShowPage[LAYER_GUILD_BG] + 1) ;
	return 1 ;
}


int SPWindowGuildEmblem::OnBGLeft( WPARAM wParam , LPARAM lParam  )
{
	if( m_iShowPage[LAYER_GUILD_BG] - 1 >= 0 )	// Min_PAGE
		GetEmblemLayerPage(LAYER_GUILD_BG, m_iShowPage[LAYER_GUILD_BG] - 1) ;
	return 1 ;
}

int SPWindowGuildEmblem::OnBGCommit( WPARAM wParam , LPARAM lParam  )
{
	int iCommitIndex = m_iSelPage[LAYER_GUILD_BG]*MAX_ONEPAGE_SHOW + m_iSelIndex[LAYER_GUILD_BG] ;
	if( m_iMyEmblem[LAYER_GUILD_BG] == iCommitIndex )
		return 1 ;

	int iRet = 0 ;
	int iRetValue = 0 ;
	if( (iRet = CheckGuildEmblemChange(LAYER_GUILD_BG, iRetValue)) != GUILD_CREATE_OK )
	{
		SetErrorNoticeDlg(iRet, iRetValue);
	}

	return 1 ;
}

// FX ===================================================================
/**
	FX 선택 - 해당 FX 출력
*/
int SPWindowGuildEmblem::OnFXSelect( unsigned int iID, WPARAM wParam , LPARAM lParam  )
{
	RECT rcDst ;
	int iSelectIndex =  iID - WIID_GUILDEMBLEM_FX_ICON_LIST ;
	int iSelectPage = m_iShowPage[LAYER_GUILD_FX] ;
	int iMaxIndex = g_pkGuildManager->GetEmblemCount(LAYER_GUILD_FX) ;

	if( iSelectPage*MAX_ONEPAGE_SHOW + iSelectIndex >= iMaxIndex )
		return 0 ;

	m_iSelIndex[LAYER_GUILD_FX] = iSelectIndex ;
	m_iSelPage[LAYER_GUILD_FX]	= iSelectPage ;

	int iSrcID = m_iSelPage[LAYER_GUILD_FX]*MAX_ONEPAGE_SHOW + m_iSelIndex[LAYER_GUILD_FX];
	SPWindowStatic* pWindowStatic = (SPWindowStatic*)Find(WIID_GUILDEMBLEM_FX_SHOW, true) ;
	EMBLEM_INFO* pEmblem = NULL ;
	g_pkGuildManager->FindSortFX(iSrcID, pEmblem) ;
	if( pWindowStatic && pEmblem && g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID) )
	{
		pWindowStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
		pWindowStatic->SetImage( g_pResourceManager->GetGlobalFileName(pEmblem->iResIconID), GetIconSrcX(pEmblem->iResIconIndex), GetIconSrcY(pEmblem->iResIconIndex)) ;
		pWindowStatic->SetShowEnable(true) ;
		pWindowStatic->Show() ;
	}

	// Bottom After View
	ClearEmblemFX() ;
	SetBeforeEmblemFx(m_iPreBeforeFxID) ;
	SetAfterEmblemFx(pEmblem->iResFxID) ;

	// 애니메이션 될 좌표를 셋팅해야 함.
	pWindowStatic = (SPWindowStatic*)Find(iID, true) ;
	if( pWindowStatic == NULL )
		return 1 ;
	pWindowStatic->GetWindowRect(rcDst) ;
	SetRect(&m_selAni[LAYER_GUILD_FX].rcDst, rcDst.left-7, rcDst.top-7, rcDst.left+12, rcDst.top+12) ;
	m_selAni[LAYER_GUILD_FX].bEnable = true ;

	return 1 ;
}

int SPWindowGuildEmblem::OnFXRight( WPARAM wParam , LPARAM lParam  )
{
	int iSymCount = g_pkGuildManager->GetEmblemCount(LAYER_GUILD_FX) ;
	if( iSymCount < 1 )		
		return 0 ;
	int iMaxPage = (iSymCount % MAX_ONEPAGE_SHOW) ? iSymCount/MAX_ONEPAGE_SHOW + 1 : iSymCount/MAX_ONEPAGE_SHOW ;
	if( m_iShowPage[LAYER_GUILD_FX] + 1 < iMaxPage )	// MAX_PAGE
		GetEmblemLayerPage(LAYER_GUILD_FX, m_iShowPage[LAYER_GUILD_FX] + 1) ;
	return 1 ;
}

int SPWindowGuildEmblem::OnFXLeft( WPARAM wParam , LPARAM lParam  )
{
	if( m_iShowPage[LAYER_GUILD_FX] - 1 >= 0 )	// Min_PAGE
		GetEmblemLayerPage(LAYER_GUILD_FX, m_iShowPage[LAYER_GUILD_FX] - 1) ;
	return 1 ;
}



int SPWindowGuildEmblem::OnFXCommit( WPARAM wParam , LPARAM lParam  )
{
	int iCommitIndex = m_iSelPage[LAYER_GUILD_FX]*MAX_ONEPAGE_SHOW + m_iSelIndex[LAYER_GUILD_FX] ;
	if( m_iMyEmblem[LAYER_GUILD_FX] == iCommitIndex )
		return 1 ;

	int iRet = 0 ;
	int iRetValue = 0 ;
	if( (iRet = CheckGuildEmblemChange(LAYER_GUILD_FX, iRetValue)) != GUILD_CREATE_OK )
	{
		SetErrorNoticeDlg(iRet, iRetValue);
	}

	return 1 ;
}


/**
	구입확인창에서의 결정
*/
int SPWindowGuildEmblem::OnBuyYes( WPARAM wParam , LPARAM lParam  )
{
	SPWindow* pWindow = NULL ;
	EMBLEM_INFO* pEmblem = NULL ;
	pEmblem = (EMBLEM_INFO*)wParam ;
	g_pGOBManager->GetGuildArchive()->SendGuildEmblemChange(pEmblem->iCIItemID, pEmblem->iConsumeItem, pEmblem->iConsumeItemCount) ;
	
	return 1 ;
}

int SPWindowGuildEmblem::OnBuyNo( WPARAM wParam , LPARAM lParam  )
{
	m_pWndModal->Hide() ;
	return 1 ;
}



/**
	ToolTip List Resize Message
	WIID_GUILD_EMBLEM_TOOLTIP_BG Resize
*/
int SPWindowGuildEmblem::OnListResize( WPARAM wParam , LPARAM lParam  )
{
	SPWindow* pWindow = NULL ;
	RECT rcTooltip ;
	int iListWidth = 0 ;
	int iListHeight = (int)lParam ;
	m_iListHeight = iListHeight ;

	if( m_pWindowToolTip )	{
		m_pWindowToolTip->GetWindowRect(rcTooltip) ;
		iListWidth = rcTooltip.right - rcTooltip.left ;
		m_pWindowToolTip->SetRectSize(iListWidth, iListHeight + 8) ;		// Margin(8)

		pWindow = Find(WIID_GUILD_EMBLEM_TOOLTIP_BG, true) ;
		if( pWindow == NULL )
			return 1 ;
		pWindow->GetWindowRect(rcTooltip) ;
		iListWidth = rcTooltip.right - rcTooltip.left ;
		pWindow->SetRectSize(iListWidth, iListHeight + 8) ;
	}

	return 1 ;
}


// Mouse Animation ===================================================================

int	SPWindowGuildEmblem::OnSymCursorEnter	( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SetMouseAnimation(iID) ;
	ShowToolTip(LAYER_GUILD_SYM, iID, true) ;
	return 1 ;
}
int	SPWindowGuildEmblem::OnSymCursorOut		( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	m_mouseAni.bEnable = false ;
	ShowToolTip(LAYER_GUILD_SYM, 0, false) ;
	return 1 ;
}
int	SPWindowGuildEmblem::OnBGCursorEnter		( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SetMouseAnimation(iID) ;
	ShowToolTip(LAYER_GUILD_BG, iID, true) ;
	return 1 ;
}
int	SPWindowGuildEmblem::OnBGCursorOut		( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	m_mouseAni.bEnable = false ;
	ShowToolTip(LAYER_GUILD_BG, 0, false) ;
	return 1 ;
}
int	SPWindowGuildEmblem::OnFXCursorEnter		( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SetMouseAnimation(iID) ;
	ShowToolTip(LAYER_GUILD_FX, iID, true) ;
	return 1 ;
}
int	SPWindowGuildEmblem::OnFXCursorOut		( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	m_mouseAni.bEnable = false ;
	ShowToolTip(LAYER_GUILD_FX, 0, false) ;
	return 1 ;
}