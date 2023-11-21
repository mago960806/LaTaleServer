// ***************************************************************
//  SPWindowItemMix   version:  1.0   ·  date: 09/03/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include <string>
#include <map>

#include "SPGOBManager.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "SPCommandConvert.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerInvenArchive.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPSkillManager.h"

#include "SPUIUnit.h"
#include "SPUIItemUnit.h"
#include "SPUIUnitManager.h"

#include "SPManager.h"
#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"

#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"
#include "SPTreeBoxItemQuest.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowGauge.h"
#include "SPWindowDigitBoard.h"
#include "SPWindowTreeBox.h"
#include "SPWindowToolTip.h"

#include <WinSock2.h>
#include "PacketID.h"
#include "Packet.h"
#include "PacketHandler.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPCheckManager.h"
#include "SPLocalizeManager.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPWindowItemMix.h"

SPWindowItemMix::SPWindowItemMix(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowItemMix::SPWindowItemMix(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_ITEM_MIX, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowItemMix::~SPWindowItemMix()
{
	Clean();
}

void SPWindowItemMix::Init()
{
	m_strToolTip		=	"[SPWindowItemMix]";

	m_bUse	=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_ITEMMIX ) ;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[ ITEM_MIX_BG_LEFT_TOP ]			, 482 , 346 , 486 , 350 );
	SetRect( &m_rcBGSrc[ ITEM_MIX_BG_CENTER_TOP ]		, 488 , 346 , 490 , 350 );
	SetRect( &m_rcBGSrc[ ITEM_MIX_BG_RIGHT_TOP ]		, 492 , 346 , 496 , 350 );

	SetRect( &m_rcBGSrc[ ITEM_MIX_BG_LEFT_MIDDLE ]		, 482 , 352 , 486 , 354 );
	SetRect( &m_rcBGSrc[ ITEM_MIX_BG_CENTER_MIDDLE ]	, 488 , 352 , 490 , 354 );
	SetRect( &m_rcBGSrc[ ITEM_MIX_BG_RIGHT_MIDDLE ]		, 492 , 352 , 496 , 354 );

	SetRect( &m_rcBGSrc[ ITEM_MIX_BG_LEFT_BOTTOM ]		, 482 , 341 , 486 , 345 );
	SetRect( &m_rcBGSrc[ ITEM_MIX_BG_CENTER_BOTTOM ]	, 488 , 341 , 490 , 345 );
	SetRect( &m_rcBGSrc[ ITEM_MIX_BG_RIGHT_BOTTOM ]		, 492 , 341 , 496 , 345 );

	SetRect( &m_rcBGSrc[ ITEM_MIX_BG_SUB_WHITE ]		, 488 , 337 , 490 , 339 );

	//////////////////////////////////////////////////////////////////////////

	int					i;
	RECT				rcTemp;
	SPWindowButton*		pWindowButton;
	SPWindowStatic*		pWindowStatic;
	SPWindowGauge*		pWindowGage;
	SPWindowDigitBoard*	pWindowDigitBoard;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_IMG_TITLE , 9 , 5 , 61 , 18 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 340 , 378 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_CLOSE , 260 , 10 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 301 , 321 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 314 , 321 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 327 , 321 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 340 , 321 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_IMG_SUBJECT , 8 , 28 , 18 , 13 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 221 , 307 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_TEXT_SUBJECT , 29 , 29 , 217 , 13 , this );
	pWindowStatic->SetFontColor( RGBA( 143 , 99 , 99 , 255 ) );
	pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_BOLD );
	pWindowStatic	=	NULL;

	pWindowGage = new SPWindowGauge( WIID_ITEM_MIX_GAGE_LEVEL , 27 , 45 , 203 , 11 , this );
	pWindowGage->SetWindowType( WND_TYPE_LEFTBASE );
	SetRect(&rcTemp, 14, 48, 15, 57); pWindowGage->SetImageGauge( "DATA/INTERFACE/CONCEPT/UI100.PNG", rcTemp);
	SetRect(&rcTemp, 23, 48, 25, 57); pWindowGage->SetImageEdge( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	SetRect(&rcTemp, 18, 48, 22, 57); pWindowGage->SetImageDeco( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	pWindowGage->SetImageFrameHead("DATA/INTERFACE/CONCEPT/UI100.PNG", 1, 47, 3, 58);
	pWindowGage->SetImageFrameBody("DATA/INTERFACE/CONCEPT/UI100.PNG", 6, 47, 8, 58);
	pWindowGage->SetImageFrameTail("DATA/INTERFACE/CONCEPT/UI100.PNG", 10, 47, 12, 58);

	pWindowGage->SPSendMessage( SPIM_SET_MAXVALUE , 100 );	
	pWindowGage->SPSendMessage( SPIM_SET_CURVALUE , 50 );

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_GAGE_BG_LEVEL_BG , 28 , 46 , 201 , 9 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 415 , 31 );
	pWindowStatic->SetSrcSize( 12 , 10 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowDigitBoard = new SPWindowDigitBoard( WIID_ITEM_MIX_GAGE_LEVEL_TEXT , 232 , 47 , 30 , 7 , this );
	pWindowDigitBoard->SetFormat( DT_LEFT );
	pWindowDigitBoard->SetWindowText( "100.00%" );

    //////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_IMG_LIST , 16 , 68 , 64 , 15 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 337 , 362 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_LIST_BTN + 0 , 17 , 84 , 48 , 16 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 353 , 294 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 353 , 345 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 353 , 328 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 353 , 345 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_LIST_BTN + 1 , 67 , 84 , 48 , 16 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 212 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 229 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 246 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 263 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_LIST_BTN + 2 , 117 , 84 , 48 , 16 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 212 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 229 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 246 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 263 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_LIST_BTN + 3 , 167 , 84 , 48 , 16 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 280 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 297 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 314 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 331 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_LIST_BTN + 4 , 217 , 84 , 48 , 16 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 280 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 297 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 314 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 331 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_LIST_BTN + 5 , 17 , 84 , 48 , 16 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 76 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 93 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 110 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 127 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_LIST_BTN + 6 , 67 , 84 , 48 , 16 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 76 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 93 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 110 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 127 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_LIST_BTN + 7 , 117 , 84 , 48 , 16 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 144 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 161 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 178 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 403 , 195 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_LIST_BTN + 8 , 167 , 84 , 48 , 16 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 144 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 161 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 178 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 452 , 195 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_LIST_BTN + 9 , 217 , 84 , 48 , 16 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 354 , 137 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 354 , 154 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 354 , 171 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 354 , 188 );
	pWindowButton =	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_LIST_BTN_LEFT , 221 , 70 , 7 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 238 , 378 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 246 , 378 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 254 , 378 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 262 , 378 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_ITEM_MIX_LIST_BTN_RIGHT , 256 , 70 , 7 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 269 , 378 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 277 , 378 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 285 , 378 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 293 , 378 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_LIST_BTN_PAGE , 229 , 69 , 26 , 13 , this );
	pWindowStatic->SetFontColor( RGBA( 143 , 99 , 99 , 255 ) );
	pWindowStatic->SetWindowText( "1/2" );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	SPWindowTreeBox*	pTreeBox	=	new SPWindowTreeBox( WIID_ITEM_MIX_LIST_TREE , 14 , 105 , 259 , 191 , this );
	pTreeBox->SetMargin( 8 , 6 , 8 , 6 );
	pTreeBox->UpdateBGPos();

	pTreeBox->SetScrollAutoHide( true );

	pTreeBox->SetScrollUpSize( 12 , 11 );
	pTreeBox->SetScrollUpTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 460 , 386 );
	pTreeBox->SetScrollUpTexture( DATA_TEXTURE_HIT			, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 473 , 386 );
	pTreeBox->SetScrollUpTexture( DATA_TEXTURE_PUSH			, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 486 , 386 );
	pTreeBox->SetScrollUpTexture( DATA_TEXTURE_DISABLE		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 499 , 386 );

	pTreeBox->SetScrollDownSize( 12 , 11 );
	pTreeBox->SetScrollDownTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 460 , 428 );
	pTreeBox->SetScrollDownTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 473 , 428 );
	pTreeBox->SetScrollDownTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 486 , 428 );
	pTreeBox->SetScrollDownTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 499 , 428 );

	pTreeBox->SetScrollSliderSize( 12 , 24 );
	pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 460 , 398 );
	pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 473 , 398 );
	pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 486 , 398 );
	pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 499 , 398 );

	pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 460 , 424 , 472 , 426 );
	pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_HIT	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 473 , 424 , 485 , 426 );
	pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 486 , 424 , 498 , 426 );
	pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_DISABLE, "DATA/INTERFACE/CONCEPT/UI103.PNG" , 499 , 424 , 511 , 426 );

	pTreeBox->SetScrollMargin( 0 , 2 , 4 , 2 );
	pTreeBox->UpdateScrollPos();

	pTreeBox->Refresh();

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_TEXT_RESULT , 19 , 302 , 244 , 13 , this );
	pWindowStatic->SetFontColor( RGBA( 143 , 99 , 99 , 255 ) );
	pWindowStatic->SetFormat( DT_LEFT | DT_VCENTER );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_IMG_RESULT_PIC , 30 , 325 , 13 , 19 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 327 , 323 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_IMG_RESULT_BOX , 20 , 319 , 32 , 32 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 1 , 165 );
	pWindowStatic	=	NULL;

	for( i = 0 ; i < MATERIAL_BOX_COUNT ; ++i )
	{
		pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_IMG_MATERIAL_PIC + i , 75 + i * 41 , 327 , 16 , 16 , this );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 328 , 345 );
		pWindowStatic	=	NULL;

		pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_IMG_MATERIAL_BOX + i , 67 + i * 41 , 319 , 32 , 32 , this );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 1 , 165 );
		pWindowStatic	=	NULL;

	}

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_IMG_BRACKET , 59 , 319 , 3 , 51 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 337 , 272 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowGage = new SPWindowGauge( WIID_ITEM_MIX_GAGE_RUN , 66 , 357 , 196 , 11 , this );
	pWindowGage->SetWindowType( WND_TYPE_LEFTBASE );
	SetRect(&rcTemp, 14, 48, 15, 57); pWindowGage->SetImageGauge( "DATA/INTERFACE/CONCEPT/UI100.PNG", rcTemp);
	SetRect(&rcTemp, 23, 48, 25, 57); pWindowGage->SetImageEdge( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	SetRect(&rcTemp, 18, 48, 22, 57); pWindowGage->SetImageDeco( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	pWindowGage->SetImageFrameHead("DATA/INTERFACE/CONCEPT/UI100.PNG", 1, 47, 3, 58);
	pWindowGage->SetImageFrameBody("DATA/INTERFACE/CONCEPT/UI100.PNG", 6, 47, 8, 58);
	pWindowGage->SetImageFrameTail("DATA/INTERFACE/CONCEPT/UI100.PNG", 10, 47, 12, 58);

	pWindowGage->SPSendMessage( SPIM_SET_MAXVALUE , 100 );	
	pWindowGage->SPSendMessage( SPIM_SET_CURVALUE , 90 );

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_GAGE_BG_RUN_BG , 67 , 358 , 194 , 9 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI103.PNG" , 241 , 391 );
	pWindowStatic->SetSrcSize( 2 , 9 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_TEXT_SUCCESS , 124 , 356 , 20 , 12 , this );
	pWindowStatic->SetFontColor( RGBA( 255 , 255 , 255 , 255 ) );
	pWindowStatic->SetFormat( DT_RIGHT | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_BOLD );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_IMG_SUCCESS , 66 , 355 , 95 , 15 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 240 , 308 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowButton = new SPWindowButton( WIID_ITEM_MIX_RUN , 69 , 378 , 68 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 346 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 346 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 346 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 346 , 494 );

	pWindowButton = new SPWindowButton( WIID_ITEM_MIX_CANCEL , 144 , 378 , 68 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI103.PNG", 415 , 440 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 415 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI103.PNG", 415 , 476 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI103.PNG", 415 , 494 );

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_BG_TITLE_LEFT , 2 , 26 , 2 , 35 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 329 , 236 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_BG_TITLE_CENTER , 4 , 26 , 273 , 35 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 333 , 236 );
	pWindowStatic->SetSrcSize( 2 , 35 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_BG_TITLE_RIGHT , 277 , 26 , 2 , 35 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 337 , 236 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_BG_LIST_LEFT , 12 , 67 , 2 , 36 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 341 , 308 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_BG_LIST_CENTER , 14 , 67 , 253 , 36 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 345 , 308 );
	pWindowStatic->SetSrcSize( 2 , 36 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_BG_LIST_RIGHT , 267 , 67 , 2 , 36 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 349 , 308 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_BG_SLOT_LEFT , 12 , 298 , 2 , 74 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 341 , 233 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_BG_SLOT_CENTER , 14 , 298 , 253 , 74 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 345 , 233 );
	pWindowStatic->SetSrcSize( 2 , 74 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_ITEM_MIX_BG_SLOT_RIGHT , 267 , 298 , 2 , 74 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 349 , 233 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	m_pCurItemMix				=	NULL;

	for( i = 0 ; i < MATERIAL_BOX_COUNT ; ++i )
	{
		m_pMaterialItem[ i ]	=	NULL;
	}

	m_iCurSkillID				=	-1;

	m_fProgressAccmulateTime	=	-1.0f;
	m_fProgressMaxTime			=	0.0f;

	m_iCurItemMixSkillPage		=	0;

	LoadIconLDT();

	InitUIItemUnit();
	UpdatePosUIItemUnit();

	ClearMaterial();

	//////////////////////////////////////////////////////////////////////////
}

void SPWindowItemMix::ReposOwnImage()
{
	SetRect( &m_rcBGDest[ ITEM_MIX_BG_LEFT_TOP ]		, m_iAX					, m_iAY					, m_iAX + 4			, m_iAY + 4 );
	SetRect( &m_rcBGDest[ ITEM_MIX_BG_CENTER_TOP ]		, m_iAX + 4				, m_iAY					, m_iAX + 4 + 273	, m_iAY + 4 );
	SetRect( &m_rcBGDest[ ITEM_MIX_BG_RIGHT_TOP ]		, m_iAX + 277			, m_iAY					, m_iAX + 277 + 4	, m_iAY + 4 );

	SetRect( &m_rcBGDest[ ITEM_MIX_BG_LEFT_MIDDLE ]		, m_iAX					, m_iAY + 4				, m_iAX + 4			, m_iAY + 4 + 395 );
	SetRect( &m_rcBGDest[ ITEM_MIX_BG_CENTER_MIDDLE ]	, m_iAX + 4				, m_iAY	+ 4				, m_iAX + 4 + 273	, m_iAY + 4 + 395 );
	SetRect( &m_rcBGDest[ ITEM_MIX_BG_RIGHT_MIDDLE ]	, m_iAX + 277			, m_iAY + 4				, m_iAX + 277 + 4	, m_iAY + 4 + 395 );

	SetRect( &m_rcBGDest[ ITEM_MIX_BG_LEFT_BOTTOM ]		, m_iAX					, m_iAY + 399			, m_iAX + 4			, m_iAY + 399 + 4 );
	SetRect( &m_rcBGDest[ ITEM_MIX_BG_CENTER_BOTTOM ]	, m_iAX + 4				, m_iAY	+ 399			, m_iAX + 4 + 273	, m_iAY + 399 + 4 );
	SetRect( &m_rcBGDest[ ITEM_MIX_BG_RIGHT_BOTTOM ]	, m_iAX + 277			, m_iAY + 399			, m_iAX + 277 + 4	, m_iAY + 399 + 4 );

	SetRect( &m_rcBGDest[ ITEM_MIX_BG_SUB_WHITE ]		, m_iAX + 2				, m_iAY	+ 61			, m_iAX + 2 + 277	, m_iAY + 61 + 338 );
}

void SPWindowItemMix::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );

	m_UIUnitManager.ClearAll( TRUE );
}

void SPWindowItemMix::Show(bool bWithChild)
{
	if( m_bUse == false )
		return;

	if( m_bShow )
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Show( bWithChild );
}

void SPWindowItemMix::Hide(bool bSendServer)
{
	if( m_bShow )
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide( bSendServer );

	ClearMaterial();
}

void SPWindowItemMix::Process(float fTime)
{
	if( m_bShow == FALSE ) return;

	SPWindow::Process( fTime );

	ProcessProgress( fTime );

	m_UIUnitManager.Process( fTime );
}

void SPWindowItemMix::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < ITEM_MIX_BG_COUNT ; ++i )
		{
			m_pBGTexture->SetColor( m_RGBA );
			m_pBGTexture->RenderTexture( &m_rcBGDest[ i ] , &m_rcBGSrc[ i ] );
		}

		g_pVideo->Flush();
	}

	SPWindow::RenderReverse( fTime );

	g_pVideo->Flush();

	m_UIUnitManager.Render();

	g_pVideo->Flush();
}

void SPWindowItemMix::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
	UpdatePosUIItemUnit();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowItemMix )

	SPIMESSAGE_COMMAND( SPIM_ITEM_MIX_SHOW			,												OnWindowShow			)
	SPIMESSAGE_COMMAND( SPIM_ITEM_MIX_RESULT		,												OnItemMixResult			)
	SPIMESSAGE_COMMAND(	SPIM_ITEM_MIX_CANCEL		,												OnCancel				)
	SPIMESSAGE_COMMAND(	SPIM_SKILL_UPDATE			,												OnSkillUpdate			)
	SPIMESSAGE_COMMAND(	SPIM_ITEM_MIX_SKILL_INIT	,												OnItemMixSkillInit		)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_ITEM_MIX_CLOSE						,	OnCloseLButtonUp		)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_ITEM_MIX_LIST_TREE					,	OnTreeBoxLButtonUp		)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_ITEM_MIX_RUN						,	OnRun					)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_ITEM_MIX_CANCEL					,	OnCancel				)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_ITEM_MIX_LIST_BTN_LEFT				,	OnLeft					)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_ITEM_MIX_LIST_BTN_RIGHT			,	OnRight					)

	SPIMESSAGE_CONTROL( SPIM_CURSOR_ENTER			,	WIID_ITEM_MIX_IMG_RESULT_BOX			,	OnResultCursorEnter		)
	SPIMESSAGE_CONTROL( SPIM_CURSOR_OUT				,	WIID_ITEM_MIX_IMG_RESULT_BOX			,	OnResultCursorOut		)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_ITEM_MIX_IMG_MATERIAL_BOX	,	WIID_ITEM_MIX_IMG_MATERIAL_BOX + MATERIAL_BOX_COUNT	,	OnMaterial				)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DOWN		,	WIID_ITEM_MIX_IMG_MATERIAL_BOX	,	WIID_ITEM_MIX_IMG_MATERIAL_BOX + MATERIAL_BOX_COUNT	,	OnMaterialCancel		)

	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER		,	WIID_ITEM_MIX_IMG_MATERIAL_BOX	,	WIID_ITEM_MIX_IMG_MATERIAL_BOX + MATERIAL_BOX_COUNT	,	OnMaterialCursorEnter	)
	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT		,	WIID_ITEM_MIX_IMG_MATERIAL_BOX	,	WIID_ITEM_MIX_IMG_MATERIAL_BOX + MATERIAL_BOX_COUNT	,	OnMaterialCursorOut		)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_ITEM_MIX_LIST_BTN			,	WIID_ITEM_MIX_LIST_BTN + LIST_BTN_COUNT				,	OnItemMixSkillLButtonUp	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowItemMix::OnWindowShow( WPARAM wParam, LPARAM lParam )
{
	if( m_bUse == false )
		return 1;

	SPWindow*	pWindow;

	pWindow	=	g_pInterfaceManager->FindWindow( WIID_SKILL );

	if( pWindow != NULL && (int)lParam == 0 )
	{
		pWindow->AdjustCoord( 18 , 115 );
		pWindow->SPSendMessage( SPIM_POS_UPDATE );
		pWindow->Show();
	}

	pWindow	=	g_pInterfaceManager->FindWindow( WIID_ITEM );

	if( pWindow != NULL )
	{
		if( (int)lParam == 0 )
			pWindow->AdjustCoord( 561 , 115 );

		pWindow->Show();
	}


	if( (int)lParam == 0 )
		SetAbsCoord( 279 , 115 );

	RefreshRelationCoord();

	UpdateTreeList( (int)wParam );
	UpdateItemMixInfo();
	UpdateItemMixSkill();

	//////////////////////////////////////////////////////////////////////////

	m_bShow	=	false;
	g_pInterfaceManager->SPChildWndSendMessage( WIID_ITEM , SPIM_ITEM_UNLOCK , (WPARAM)GetInstanceID() , NULL );

	//////////////////////////////////////////////////////////////////////////

	ClearMaterial();

	return 1;
}

int SPWindowItemMix::OnItemMixResult( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	if( g_pCheckManager != NULL )
		g_pCheckManager->SetDBBlock( false );

	if( g_pItemCluster != NULL )
		g_pItemCluster->SetItemUpgradeState( SPIU_STATE_NULL );

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	GLOBAL_STRING_ID	uiGlobalStringID	=	0;

	pPacket->ExtractUINT32( &uiGlobalStringID );

	if( uiGlobalStringID != 0 )
	{
		SetError( uiGlobalStringID );
		return 1;
	}

	SPPlayerInvenArchive*	pLocalPlayerInvenArchive	=	pLocalPlayer->GetInvenArchive();

	if( pLocalPlayer == NULL )
		return 1;

	//////////////////////////////////////////////////////////////////////////

	UINT8			uiMixed;
	int				iMixID	=	0;
	ITEMMIX_INFO	stItemMixInfo;			// 캐릭터의 조합정보
	UINT8			iNumOfMeterial;
	TRASH_ITEM		stMeterial;
	CONTAINER_ITEM	stResult;				// 결과아이템

	pPacket->ExtractUINT8( &uiMixed );
	pPacket->ExtractUINT32( (UINT*)&iMixID );

	SPItemMix*	pItemMix	=	g_pItemCluster->GetItemMixItemMixID( m_iCurSkillID , iMixID );

	if( pItemMix != NULL )
	{
		if( uiMixed == 0 && pItemMix->m_iWhenFails == 0 )
		{
			SetError( 16010011 );
		}
	}

	pPacket->ExtractStruct( &stItemMixInfo , sizeof( ITEMMIX_INFO ) );

	SPPlayerStatus*		pPlayerStatus	=	(SPPlayerStatus*)pLocalPlayer->GetStatus();

	if( pPlayerStatus != NULL )
		pPlayerStatus->SetItemMix( stItemMixInfo , true );
	
	pPacket->ExtractUINT8( &iNumOfMeterial );

	for( int i = 0 ; i < iNumOfMeterial ; ++i )
	{
		pPacket->ExtractStruct( &stMeterial , sizeof( TRASH_ITEM ) );
		
		--stMeterial.iContainerSlot.SlotIndex;
		
		pLocalPlayerInvenArchive->TrashItem( stMeterial );
	}

	pPacket->ExtractStruct( &stResult , sizeof( CONTAINER_ITEM ) );
	
	--stResult.SlotInfo.SlotIndex;

	if( pLocalPlayerInvenArchive->ItemSetting( stResult , false ) == true && pItemMix != NULL )
	{
		SPItemAttr*	pItemAttr = NULL;
		char		szMsg[ _MAX_PATH ];

		if( g_pItemCluster->GetItemInfo( stResult.Item.iItemID , pItemAttr ) == true )
		{				
			D3DXCOLOR	color	=	g_pCheckManager->GetItemColor( stResult.Item.iRare );

			if( pItemAttr->m_bNameColor == true )
				color = pItemAttr->GetNameColor();

			if( uiMixed == 0 && pItemMix->m_iWhenFails > 0 )
				sprintf( szMsg , g_pResourceManager->GetGlobalString( 16010012 ) , pItemAttr->m_strName.c_str() , pItemMix->m_iWhenFailsCount );	//	실패
			else if( uiMixed == 1 )
				sprintf( szMsg , g_pResourceManager->GetGlobalString( 16010013 ) , pItemAttr->m_strName.c_str() , pItemMix->m_iResultCount );	//	성공

			g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMsg , (LPARAM)&color );
		}
	}

	//////////////////////////////////////////////////////////////////////////

	UpdateItemMixInfo();

	SPItem*	pOldItem;

	for( i = 0 ; i < MATERIAL_BOX_COUNT ; ++i )
	{
		pOldItem	=	m_pMaterialItem[ i ];

		if( pOldItem == NULL )
			continue;

		ResetMaterialData( i );

		if( pOldItem->IsEmptyItem() == true )
			continue;

		SetMaterialItem( pOldItem );
	}

	return 1;
}

int SPWindowItemMix::OnSkillUpdate( WPARAM wParam, LPARAM lParam )
{
	UpdateItemMixSkill();

	return 1;
}

int SPWindowItemMix::OnItemMixSkillInit( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	SPPlayerInvenArchive*	pLocalPlayerInvenArchive	=	pLocalPlayer->GetInvenArchive();

	if( pLocalPlayer == NULL )
		return 1;

	//////////////////////////////////////////////////////////////////////////

	ITEMMIX_INFO	stItemMixInfo;			// 캐릭터의 조합정보

	pPacket->ExtractStruct( &stItemMixInfo , sizeof( ITEMMIX_INFO ) );

	SPPlayerStatus*		pPlayerStatus	=	(SPPlayerStatus*)pLocalPlayer->GetStatus();

	if( pPlayerStatus != NULL )
		pPlayerStatus->SetItemMix( stItemMixInfo , true );

	//////////////////////////////////////////////////////////////////////////

	SPSkillArchive*	pPlayerSkillArchive	=	pLocalPlayer->GetSkillArchive();

	if( pPlayerSkillArchive != NULL )
		pPlayerSkillArchive->ReInitSkill( SSC_ITEMMIX );

	UpdateItemMixSkill();

	// 스킬 변경시 스킬 윈도우로 메세지 통보 AJJIYA [7/28/2005]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_SKILL , SPIM_SKILL_UPDATE , -1 , -1 );	// AJJIYA [10/25/2005]

	// 스킬 변경시 퀵슬롯 윈도우로 메세지 통보 AJJIYA [7/28/2005]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_QUICK_SLOT_UPDATE , -1 , -2 );	// AJJIYA [10/25/2005]

	// 스킬 변경시 이모티콘 윈도우로 메세지 통보 AJJIYA [8/30/2005]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_UPDATE_EMOTICON , -1 , -1 );		// AJJIYA [10/25/2005]

	return 1;
}

int SPWindowItemMix::OnCloseLButtonUp( WPARAM wParam, LPARAM lParam )
{
	Hide();

	return 1;
}

int SPWindowItemMix::OnTreeBoxLButtonUp( WPARAM wParam, LPARAM lParam )
{
	TREEBOX_ITEM*	pSelectQuestItem	=	(TREEBOX_ITEM*)wParam;

	if( pSelectQuestItem == NULL )
		return 1;

	SPTreeBoxItemQuest*	pMixItem		=	(SPTreeBoxItemQuest*)( pSelectQuestItem->m_pItemBase );

	if( pMixItem == NULL )
		return 1;

	UpdateMaterial( m_iCurSkillID , pMixItem->GetReturnValue() );

	return 1;
}

int SPWindowItemMix::OnRun( WPARAM wParam, LPARAM lParam )
{
	if( g_pGOBManager == NULL )
		return 1;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetCurAction() != ACTION_STOP )
	{
//		SetError( GS_ITEM_UPGRADE_ACTION_STOP );
		return 1;
	}

	SPPlayerInvenArchive*	pLocalPlayerInvenArchive	=	pLocalPlayer->GetInvenArchive();

	if( pLocalPlayerInvenArchive != NULL && m_pCurItemMix != NULL )
	{
		SPItemAttr*	pItemAttr = NULL;

		if( g_pItemCluster->GetItemInfo( m_pCurItemMix->m_iResult , pItemAttr ) == true )
		{
			CONTAINER_TYPE	eItemType = (CONTAINER_TYPE)( pItemAttr->m_eType - 1 );

			if( pLocalPlayerInvenArchive->IsFullInventory( eItemType ) == true )
			{
				SetError( 16010031 + (int)eItemType );
				return 1;
			}
		}
	}

	m_fProgressAccmulateTime	=	0.0f;
	m_fProgressMaxTime			=	-1.0f;

	SPPlayerStatus*	pLocalPlayerStatus	=	(SPPlayerStatus*)pLocalPlayer->GetStatus();

	if( pLocalPlayerStatus != NULL )
	{
		float fSuccessPercent	=	CalSuccessPercent( pLocalPlayerStatus->GetItemMix() , m_pCurItemMix );

		if( fSuccessPercent > 0.0f )
            m_fProgressMaxTime	=	( 2.0f / fSuccessPercent ) * 100.0f;
	}

	SendParentAction( ACTION_ENCHANTTING , 0 , true );

	SPWindow*	pWindow	=	Find( WIID_ITEM_MIX_RUN , true );

	if( pWindow != NULL )
		pWindow->SetEnable( false );

	return 1;
}

int SPWindowItemMix::OnCancel( WPARAM wParam, LPARAM lParam )
{
	if( IsShow() == false )
		return 0;

	int	iType	=	(int)wParam;

	if( iType == 1 )
	{
//		SetError( GS_ITEM_UPGRADE_OBSTACLE_CANCEL );
	}

	ClearMaterial();

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_ITEM_MIX_LIST_TREE ) );

	if( pTreeBox == NULL )
		return 1;

	TREEBOX_ITEM*	pSelectQuestItem	=	pTreeBox->GetSelectItem();

	if( pSelectQuestItem == NULL )
		return 1;

	SPTreeBoxItemQuest*	pMixItem		=	(SPTreeBoxItemQuest*)( pSelectQuestItem->m_pItemBase );

	if( pMixItem == NULL )
		return 1;

	UpdateMaterial( m_iCurSkillID , pMixItem->GetReturnValue() );

	return 1;
}

int SPWindowItemMix::OnLeft( WPARAM wParam, LPARAM lParam )
{
	--m_iCurItemMixSkillPage;

	UpdateItemMixSkill();

	return 1;
}

int SPWindowItemMix::OnRight( WPARAM wParam, LPARAM lParam )
{
	++m_iCurItemMixSkillPage;

	UpdateItemMixSkill();

	return 1;
}

int SPWindowItemMix::OnResultCursorEnter( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	if( m_pCurItemMix == NULL )
		return 1;

	SPUIItemUnit*	pUIItemUnit	=	NULL;

	pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( 0 ) );

	if( pUIItemUnit == NULL )
		return 1;

	if( pUIItemUnit->GetStatus( SPUIUnit::STATUS_UI_UNIT_RENDER ) == false )
		return 1;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)m_pCurItemMix, SPWindowToolTip::TOOLTIP_SHOW_TYPE_ITEMMIX );

	return 1;
}

int SPWindowItemMix::OnResultCursorOut( WPARAM wParam, LPARAM lParam )
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );
	return 1;
}

int SPWindowItemMix::OnMaterial( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	//if( m_eCurState == ITEM_UPGRADE_STATE_READY_ITEM		||
	//	m_eCurState == ITEM_UPGRADE_STATE_READY_MATERIAL	||
	//	m_eCurState == ITEM_UPGRADE_STATE_READY_UPGRADE		)
	//{
		SPItem* pPickUpItem = GetInvenPickUpItem();

		if( pPickUpItem == NULL )
			return 1;

		SetMaterialItem( pPickUpItem );
	//}

	return 1;
}

int SPWindowItemMix::OnMaterialCancel( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	//if( m_eCurState == ITEM_UPGRADE_STATE_READY_ITEM		||
	//	m_eCurState == ITEM_UPGRADE_STATE_READY_MATERIAL	||
	//	m_eCurState == ITEM_UPGRADE_STATE_READY_UPGRADE		)
	//{
		ResetMaterialData( iID - WIID_ITEM_MIX_IMG_MATERIAL_BOX );

		SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_ITEM_MIX_LIST_TREE ) );

		if( pTreeBox == NULL )
			return 1;

		TREEBOX_ITEM*	pSelectQuestItem	=	pTreeBox->GetSelectItem();

		if( pSelectQuestItem == NULL )
			return 1;

		SPTreeBoxItemQuest*	pMixItem		=	(SPTreeBoxItemQuest*)( pSelectQuestItem->m_pItemBase );

		if( pMixItem == NULL )
			return 1;

		UpdateMaterial( m_iCurSkillID , pMixItem->GetReturnValue() );
	//}

	return 1;
}

int SPWindowItemMix::OnMaterialCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	SPUIUnit*	pUIUnit	=	m_UIUnitManager.GetUnit( iID - WIID_ITEM_MIX_IMG_MATERIAL_BOX + 1 );

	if( pUIUnit == NULL )
		return 1;

	if( pUIUnit->GetStatus( SPUIUnit::STATUS_UI_UNIT_RENDER ) == false )
		return 1;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pUIUnit, -2 );

	return 1;
}

int SPWindowItemMix::OnMaterialCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, -2 );
	return 1;
}

int SPWindowItemMix::OnItemMixSkillLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( g_pGOBManager == NULL )
		return 1;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	SPSkillArchive*	pLocalPlayerSkillArchive	=	pLocalPlayer->GetSkillArchive();

	if( pLocalPlayerSkillArchive == NULL )
		return 1;

	int	iSelectPos = iID - WIID_ITEM_MIX_LIST_BTN;

	SPSkillActivity*	pSkillActivity	=	pLocalPlayerSkillArchive->GetFirstSkillActivity( SIT_ITEMMIX );
	SPSkill*			pSkill			=	NULL;
	int					iSkillCount		=	0;

	//	List 전체 스킬 갯수를 구한다.
	while( pSkillActivity != NULL )
	{
		if( iSelectPos == iSkillCount )
		{
			pSkill = pSkillActivity->GetSkill();

			if( pSkill != NULL )
			{
				return OnWindowShow( (WPARAM)pSkill->GetID() , (LPARAM)1 );
			}
		}

		pSkillActivity	=	pLocalPlayerSkillArchive->GetNextSkillActivity( SIT_ITEMMIX );

		++iSkillCount;
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////

void SPWindowItemMix::LoadIconLDT()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/ITEM_CREATE_ICON.LDT" , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [ DATA/LDT/ITEM_CREATE_ICON.LDT ] \n" );
#endif
		return;
	}

	LDT_Field				ldtFieldFileName;
	LDT_Field				ldtFieldStartX;
	LDT_Field				ldtFieldStartY;
	LDT_Field				ldtFieldEndX;
	LDT_Field				ldtFieldEndY;

	int						iRecordCount	= pLDTFile->GetItemCount();
	int						iItemID;
	ITEMMIX_CATEGORY_ICON	stCategoryIcon;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetField( iItemID, 1,		ldtFieldFileName );		//	_Icon
		pLDTFile->GetField( iItemID, 2,		ldtFieldStartX );		//	_Start_X
		pLDTFile->GetField( iItemID, 3,		ldtFieldStartY );		//	_Start_Y
		pLDTFile->GetField( iItemID, 4,		ldtFieldEndX );			//	_End_X
		pLDTFile->GetField( iItemID, 5,		ldtFieldEndY );			//	_End_Y

		if( ldtFieldFileName.uData.pValue == NULL )
			continue;

		stCategoryIcon.m_iCategory		=	iItemID;
        stCategoryIcon.m_strFileName	=	ldtFieldFileName.uData.pValue;
		stCategoryIcon.m_iSrcSX			=	ldtFieldStartX.uData.lValue;
		stCategoryIcon.m_iSrcSY			=	ldtFieldStartY.uData.lValue;
		stCategoryIcon.m_iSrcEX			=	ldtFieldEndX.uData.lValue;
		stCategoryIcon.m_iSrcEY			=	ldtFieldEndY.uData.lValue;

		AddCategoryIcon( stCategoryIcon );
	}

	SAFE_RELEASE( pLDTFile );
}

void SPWindowItemMix::AddCategoryIcon( ITEMMIX_CATEGORY_ICON& stCategoryIcon )
{
	if( GetCategoryIcon( stCategoryIcon.m_iCategory ) == NULL )
	{
		m_mCategoryIcon.insert( STD_MAP_CATEGORY_ICON::value_type( stCategoryIcon.m_iCategory , stCategoryIcon ) );
		return;
	}
}

ITEMMIX_CATEGORY_ICON* SPWindowItemMix::GetCategoryIcon( int iCategory )
{
	STD_MAP_CATEGORY_ICON::iterator	mIter	=	m_mCategoryIcon.find( iCategory );

	if( mIter != m_mCategoryIcon.end() )
		return	&((*mIter).second);

	return NULL;
}

TREEBOX_ITEM* SPWindowItemMix::GetCategoryItem( int iCategory )
{
	STD_MAP_TREEBOX_ITEM::iterator	mIter	=	m_mpCategory.find( iCategory );

	if( mIter != m_mpCategory.end() )
		return	(*mIter).second;

	std::string	strCategory;
	int			iGSItemID	=	16000000;

	const char*	pszCategory	=	g_pResourceManager->GetGlobalString( iGSItemID + iCategory );

	if( pszCategory == NULL )
		pszCategory	=	g_pResourceManager->GetGlobalString( iGSItemID );

	if( pszCategory == NULL )
		strCategory	=	"No Data";
	else
		strCategory	=	pszCategory;

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_ITEM_MIX_LIST_TREE ) );

	if( pTreeBox == NULL )
		return NULL;

	SPTreeBoxItemQuest*	pQuestItem	=	CreateTreeBoxItem( true , iCategory );

	if( pQuestItem == NULL )
		return NULL;

	pQuestItem->SetText( strCategory.c_str() );
	pQuestItem->SetReturnValue( 0 );

	TREEBOX_ITEM*	pTreeBoxItem	=	pTreeBox->InsertItem( NULL , pQuestItem , iCategory - 1 );

	m_mpCategory.insert( STD_MAP_TREEBOX_ITEM::value_type( iCategory , pTreeBoxItem ) );

	return pTreeBoxItem;
}

bool SPWindowItemMix::DelCategoryItem( int iCategory )
{
	if( iCategory < 0 )
	{
		m_mpCategory.clear();
		return true;
	}

	STD_MAP_TREEBOX_ITEM::iterator	mIter	=	m_mpCategory.find( iCategory );

	if( mIter != m_mpCategory.end() )
	{
		m_mpCategory.erase( mIter );
		return	true;
	}

	return false;
}

SPTreeBoxItemQuest* SPWindowItemMix::CreateTreeBoxItem( bool bMenuType , int iCategory /* = 0  */ )
{
	SPTreeBoxItemQuest*	pQuestItem	=	new SPTreeBoxItemQuest;

	if( pQuestItem == NULL )
		return NULL;

	for( int j = 0 ; j < DATA_STRING_COUNT ; ++j )
	{
		pQuestItem->SetTextColor( j , D3DCOLOR_ARGB( 255 , 143 , 99 , 99 ) );
	}

	pQuestItem->SetSize( 219 , 16 );

	pQuestItem->SetQuestSize( 219 , 16 );
	pQuestItem->SetQuestOffset( 0 , 0 );

	if( bMenuType == true )
	{
		pQuestItem->SetTextMargin( 24 , 0 );


		//////////////////////////////////////////////////////////////////////////

		ITEMMIX_CATEGORY_ICON*	pCategoryIcon = GetCategoryIcon( iCategory );

		if( pCategoryIcon != NULL )
		{
			//	제일 앞의 아이콘
			pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2		, pCategoryIcon->m_strFileName.c_str() , pCategoryIcon->m_iSrcSX , pCategoryIcon->m_iSrcSY , pCategoryIcon->m_iSrcEX , pCategoryIcon->m_iSrcEY );	//	선택 안되었을때
			pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1	, pCategoryIcon->m_strFileName.c_str() , pCategoryIcon->m_iSrcSX , pCategoryIcon->m_iSrcSY , pCategoryIcon->m_iSrcEX , pCategoryIcon->m_iSrcEY );	//	선택 되었을때

			pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2		, 0 , 0 );
			pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1	, 0 , 0 );
		}
	}
	else
	{
		pQuestItem->SetTextMargin( 28 , 0 );

		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_MOUSE_OVER * 2 ,				"DATA/INTERFACE/CONCEPT/UI107.PNG" , 185 , 205 , 385 , 220 );

		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_MOUSE_OVER * 2 , 24 , 0 );
	}

	//////////////////////////////////////////////////////////////////////////

	return pQuestItem;
}

void SPWindowItemMix::InitUIItemUnit()
{
	int	iUIItemUnitCount = MATERIAL_BOX_COUNT + 1;

	m_UIUnitManager.SetBufferSize( iUIItemUnitCount );

	SPUIItemUnit*	pUIItemUnit	=	NULL;
	char			szResPath[]		=	"DATA/INTERFACE/CONCEPT/UI100.PNG";

	for( int i = 0 ; i < iUIItemUnitCount ; ++i )
	{
		pUIItemUnit		=	new	SPUIItemUnit;

		pUIItemUnit->SetPos( 0 ,0 );
		pUIItemUnit->SetSize( 32 , 32 );

		pUIItemUnit->SetDisableTexture( szResPath );
		pUIItemUnit->SetDisableRectSrc( 434 , 37 , 436 , 39 );

		pUIItemUnit->SetCoolTimeTexture( szResPath );
		pUIItemUnit->SetCoolTimeRectSrc( 434 , 37 , 436 , 39 );

		pUIItemUnit->SetInstanceID( WIID_ITEM );

		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );

		m_UIUnitManager.AddUnit( pUIItemUnit );
	}
}

void SPWindowItemMix::UpdatePosUIItemUnit()
{
	RECT			rcPos;
	SPUIItemUnit*	pUIItemUnit			=	NULL;
	SPWindow*		pWindow				=	NULL;
	int				iUIItemUnitCount	=	MATERIAL_BOX_COUNT + 1;

	for( int i = 0 ; i < iUIItemUnitCount ; ++i )
	{
		pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( i );

		if( pUIItemUnit == NULL )
			continue;

		if( i == 0 )
			pWindow	=	Find( WIID_ITEM_MIX_IMG_RESULT_BOX , true );
		else
            pWindow	=	Find( WIID_ITEM_MIX_IMG_MATERIAL_BOX + i - 1 , true );

		if( pWindow == NULL )
			continue;

		pWindow->GetWindowRect( rcPos );
		pUIItemUnit->SetPos( rcPos.left , rcPos.top );
		pUIItemUnit->ProcessCount();
	}
}

void SPWindowItemMix::UpdateTreeList( int iSkillID )
{
	if( iSkillID <= 0 )
		return;

	if( g_pItemCluster == NULL )
		return;

	std::map< int , std::map< int , SPItemMix > >*	pmmItemMixList	=	g_pItemCluster->GetItemMixSkillID( iSkillID );

	if( pmmItemMixList == NULL )
		return;

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_ITEM_MIX_LIST_TREE ) );

	if( pTreeBox == NULL )
		return;

	pTreeBox->DelItemAll();
	DelCategoryItem( -1 );

	m_iCurSkillID	=	iSkillID;

	std::map< int , std::map< int , SPItemMix > >::iterator	mmIter		=	pmmItemMixList->begin();
	std::map< int , std::map< int , SPItemMix > >::iterator	mmEndIter	=	pmmItemMixList->end();

	std::map< int , SPItemMix >::iterator	mIter;
	std::map< int , SPItemMix >::iterator	mIterEnd;

	SPItemMix*				pItemMix;
	TREEBOX_ITEM*			pTreeItem		=	NULL;
	SPTreeBoxItemQuest*		pItemMixData	=	NULL;

	while( mmIter != mmEndIter )
	{
		mIter		=	((*mmIter).second).begin();
		mIterEnd	=	((*mmIter).second).end();

		while( mIter != mIterEnd )
		{
			pItemMix = &((*mIter).second);

			if( pItemMix != NULL )
			{
				pTreeItem		=	GetCategoryItem( pItemMix->m_iCategory );
				pItemMixData	=	CreateTreeBoxItem( false );

				if( pItemMixData != NULL )
				{
					pItemMixData->SetText( pItemMix->m_strName.c_str() );
					pItemMixData->SetReturnValue( pItemMix->m_iItemMixID );

					pTreeBox->AddItem( pTreeItem , pItemMixData );
				}
			}

			++mIter;
		}

		++mmIter;
	}

	pTreeBox->Refresh();
}

void SPWindowItemMix::UpdateMaterial( int iSkillID , int iMixID )
{
	if( iSkillID <= 0 )
		return;

	if( g_pItemCluster == NULL )
		return;

	if( g_pCheckManager != NULL )
	{
		if( g_pCheckManager->IsDBBlock() == true )
			return;
	}

	SPItemMix*	pItemMix	=	g_pItemCluster->GetItemMixItemMixID( iSkillID , iMixID );

	if( pItemMix == NULL )
		return;

	ClearMaterial();

	m_pCurItemMix	=	pItemMix;

	//////////////////////////////////////////////////////////////////////////

	int				i			=	0;
	SPWindow*		pWindow		=	NULL;
	SPUIItemUnit*	pUIItemUnit	=	NULL;

	//////////////////////////////////////////////////////////////////////////
	//
	//	결과물 표시
	//

	pWindow	=	Find( WIID_ITEM_MIX_TEXT_RESULT );

	if( pWindow != NULL )
	{
		SPItemAttr*	pItemAttr = NULL;
		char		szMsg[ _MAX_PATH ];

		if( g_pItemCluster->GetItemInfo( pItemMix->m_iResult , pItemAttr ) == true )
		{				
			pWindow->SetWindowText( pItemAttr->m_strName.c_str() );
		}
	}

	pWindow	=	Find( WIID_ITEM_MIX_IMG_RESULT_PIC );

	if( pWindow != NULL )
		pWindow->Hide();

	pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( 0 ) );

	if( pUIItemUnit != NULL )
	{
		if( pItemMix->m_iResultCount > 1 )
			pUIItemUnit->SetCount( pItemMix->m_iResultCount );
		else
			pUIItemUnit->SetCount( 0 );

		pUIItemUnit->SetItemID( pItemMix->m_iResult );
		pUIItemUnit->ReResourceLoad();
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	재료 표시
	//

	SPItemAttr*		pMaterialItemAttr	=	NULL;

	for( i = 0 ; i < MATERIAL_BOX_COUNT ; ++i )
	{
		pWindow	=	Find( WIID_ITEM_MIX_IMG_MATERIAL_PIC + i );

		if( pWindow == NULL )
			continue;

		pWindow->Hide();

		if( pItemMix->m_stMaterial[ i ].m_iID <= 0 || pItemMix->m_stMaterial[ i ].m_iCount <= 0 )
		{
			pWindow	=	Find( WIID_ITEM_MIX_IMG_MATERIAL_BOX + i );

			if( pWindow != NULL )
				pWindow->Hide();

			continue;
		}

		pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( i + 1 ) );

		if( pUIItemUnit != NULL )
		{
			if( pItemMix->m_stMaterial[ i ].m_iCount > 1)
				pUIItemUnit->SetCount( pItemMix->m_stMaterial[ i ].m_iCount );
			else
				pUIItemUnit->SetCount( 0 );

			pUIItemUnit->SetItemID( pItemMix->m_stMaterial[ i ].m_iID );
			pUIItemUnit->ReResourceLoad();
			pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
		}
	}

	UpdateItemMixInfo();
}

void SPWindowItemMix::UpdateItemMixInfo()
{
	if( g_pGOBManager == NULL )
		return;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPPlayerStatus*	pLocalPlayerStatus	=	(SPPlayerStatus*)pLocalPlayer->GetStatus();

	if( pLocalPlayerStatus == NULL )
		return;

	ITEMMIX_INFO*	pItemMixInfo	=	pLocalPlayerStatus->GetItemMix();

	if( pItemMixInfo == NULL )
		return;

	SetLevelText( pItemMixInfo->iLv );
	SetLevelGage( CalLevelPercent( pItemMixInfo ) , 100.0f );
	SetSuccessProbability( (int)CalSuccessPercent( pItemMixInfo , m_pCurItemMix ) );
}

void SPWindowItemMix::UpdateItemMixSkill()
{
	if( g_pGOBManager == NULL )
		return;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPSkillArchive*	pLocalPlayerSkillArchive	=	pLocalPlayer->GetSkillArchive();

	if( pLocalPlayerSkillArchive == NULL )
		return;

	if( m_iCurItemMixSkillPage < 0 )
		m_iCurItemMixSkillPage	=	0;

	if( m_iCurItemMixSkillPage > 1 )
		m_iCurItemMixSkillPage	=	1;

	SPWindow*	pWindow;
	int			iBtnListCount	=	LIST_BTN_COUNT / 2;
	bool		bOnePage		=	false;
	bool		bTwoPage		=	false;
	int			iSkillCount		=	pLocalPlayerSkillArchive->GetSkillCount( SIT_ITEMMIX );

	if( iSkillCount <= 0 )
	{
		Hide();
		return;
	}

	if( m_iCurItemMixSkillPage == 0 )
	{
		bOnePage	=	true;
	}
	else
	{
		bTwoPage	=	true;
	}

	for( int i = 0 ; i < iBtnListCount ; ++i )
	{
		pWindow	=	Find( WIID_ITEM_MIX_LIST_BTN + i );

		if( pWindow != NULL )
		{
			if( iSkillCount > i )
				pWindow->SetShowEnable( bOnePage );
			else
				pWindow->SetShowEnable( false );

			pWindow->Hide();
			pWindow->Show();
		}

		pWindow	=	Find( WIID_ITEM_MIX_LIST_BTN + iBtnListCount + i );

		if( pWindow != NULL )
		{
			if( iSkillCount - iBtnListCount > i )
				pWindow->SetShowEnable( bTwoPage );
			else
				pWindow->SetShowEnable( false );

			pWindow->Hide();
			pWindow->Show();
		}
	}

	bool	bScroll			=	false;

	if( iSkillCount > iBtnListCount )
	{
		bScroll	=	true;
	}

	pWindow	=	Find( WIID_ITEM_MIX_LIST_BTN_LEFT );

	if( pWindow != NULL )
	{
		pWindow->SetShowEnable( bScroll );

		pWindow->Hide();
		pWindow->Show();
	}

	pWindow	=	Find( WIID_ITEM_MIX_LIST_BTN_RIGHT );

	if( pWindow != NULL )
	{
		pWindow->SetShowEnable( bScroll );

		pWindow->Hide();
		pWindow->Show();
	}

	pWindow	=	Find( WIID_ITEM_MIX_LIST_BTN_PAGE );

	if( pWindow != NULL )
	{
		if( bScroll == false )
		{
			pWindow->SetWindowText( NULL );
		}
		else
		{
			char	szBuf[ _MAX_PATH ];
			sprintf( szBuf , "%d/2" , m_iCurItemMixSkillPage + 1 );
			pWindow->SetWindowText( szBuf );
		}
	}
}

void SPWindowItemMix::ClearMaterial()
{
	int			i;
	SPUIUnit*	pUIUnit	=	NULL;
	SPWindow*	pWindow	=	NULL;

	for( i = 0 ; i < m_UIUnitManager.GetUnitCount() ; ++i )
	{
		pUIUnit	=	m_UIUnitManager.GetUnit( i );

		if( pUIUnit != NULL )
		{
			pUIUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
		}
	}

	pWindow	=	Find( WIID_ITEM_MIX_TEXT_RESULT );

	if( pWindow != NULL )
		pWindow->SetWindowText( NULL );

	pWindow	=	Find( WIID_ITEM_MIX_IMG_RESULT_BOX );

	if( pWindow != NULL )
		pWindow->Show();

	pWindow	=	Find( WIID_ITEM_MIX_IMG_RESULT_PIC );

	if( pWindow != NULL )
		pWindow->Show();

	for( i = 0 ; i < MATERIAL_BOX_COUNT ; ++i )
	{
		pWindow	=	Find( WIID_ITEM_MIX_IMG_MATERIAL_BOX + i );

		if( pWindow != NULL )
			pWindow->Show();

		pWindow	=	Find( WIID_ITEM_MIX_IMG_MATERIAL_PIC + i );

		if( pWindow != NULL )
			pWindow->Show();
	}

	SetSuccessProbability( -1 );

	ResetMaterialData();
}

void SPWindowItemMix::SetMaterialItem( SPItem* pMaterialItem )
{
	if( pMaterialItem == NULL || m_pCurItemMix == NULL || g_pItemCluster == NULL )
		return;

	if( pMaterialItem->GetContainerType() > INVENTORY_HOUSING )
	{
//		SetError( GS_ITEM_UPGRADE_ONLY_ITEM_WINDOW );
		return;
	}

	SPItemStatus*	pMaterialItemStatus	=	pMaterialItem->GetItemStatus();

	if( pMaterialItemStatus == NULL )
		return;

	// 아이템 유효성 체크 [10/31/2006 AJJIYA]
	if( pMaterialItemStatus->IsTimeAvailable() == false )
	{
		SPItemAttr*	pMaterialItemAttr	=	pMaterialItem->GetItemAttr();

		if( pMaterialItemAttr != NULL )
		{
//			SetError( GS_ITEM_UPGRADE_ITEM_EXPIRE , pMaterialItemAttr->m_strName.c_str() );
		}

		return;
	}

	int	iMaterialCount	=	0;
	int	iPos			=	0;

	if( CheckMaterialCondition( iMaterialCount , iPos , m_pCurItemMix , pMaterialItem , true ) == false )
		return;

	if( m_pMaterialItem[ iPos ] != NULL )
	{
		SetInvenItemUnlock( m_pMaterialItem[ iPos ] , iPos + 1 );
	}

	SPUIItemUnit*	pUIItemUnit	=	NULL;

	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( iPos + 1 );

	if( pUIItemUnit != NULL )
		pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );

	m_pMaterialItem[ iPos ]	=	pMaterialItem;

	//////////////////////////////////////////////////////////////////////////

	int iMaterialDataCount	=	0;

	for( int i = 0 ; i < MATERIAL_BOX_COUNT ; ++i )
	{
		if( m_pMaterialItem[ i ] != NULL )
			++iMaterialDataCount;
	}

	bool	bResultBox	=	false;

	if( iMaterialCount == iMaterialDataCount )
	{
		bool	bRequire	=	true;

		for( i = 0 ; i < MAX_ITEM_MIX_REQUIRE ; ++i )
		{
			if( g_pCheckManager->CheckCondition( m_pCurItemMix->m_stRequireType[ i ].m_iType , m_pCurItemMix->m_stRequireType[ i ].m_iID , m_pCurItemMix->m_stRequireType[ i ].m_iValue1 , m_pCurItemMix->m_stRequireType[ i ].m_iValue2 ) == false )
			{
				bRequire	=	false;
			}
		}

		bResultBox	=	bRequire;
	}

	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( 0 );

	if( pUIItemUnit != NULL )
	{
		if( bResultBox == true )
			pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
		else
			pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
	}

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_ITEM_MIX_RUN );

	if( pWindow != NULL )
	{
		pWindow->SetEnable( bResultBox );

		if( bResultBox == true )
		{
//			m_eCurState = ITEM_UPGRADE_STATE_READY_UPGRADE;
		}
		else
		{
//			m_eCurState = ITEM_UPGRADE_STATE_READY_MATERIAL;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	SetInvenItemLock( m_pMaterialItem[ iPos ] , iPos + 1 );
}

void SPWindowItemMix::ResetMaterialData()
{
	for( int i = 0 ; i < MATERIAL_BOX_COUNT ; ++i )
	{
		ResetMaterialData( i );
	}
}

void SPWindowItemMix::ResetMaterialData( int iMaterialPos )
{
	if( iMaterialPos < 0 || iMaterialPos >= MATERIAL_BOX_COUNT )
		return;

	if( g_pCheckManager != NULL )
	{
		if( g_pCheckManager->IsDBBlock() == true )
			return;
	}

	//////////////////////////////////////////////////////////////////////////

	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_ITEM_MIX_RUN , true );

	if( pWindow != NULL )
		pWindow->SetEnable( false );

	SPUIItemUnit*	pUIItemUnit	=	NULL;

	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( iMaterialPos + 1 );

	if( pUIItemUnit != NULL )
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );

	pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( 0 );

	if( pUIItemUnit != NULL )
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );

	if( m_pMaterialItem[ iMaterialPos ] == NULL )
		return;

	SetInvenItemUnlock( m_pMaterialItem[ iMaterialPos ] , iMaterialPos + 1 );

	m_pMaterialItem[ iMaterialPos ]	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	if( g_pGOBManager != NULL )
	{
		SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer != NULL )
		{
			if( pLocalPlayer->GetCurAction() == ACTION_ENCHANTTING )
			{
				SendParentAction( ACTION_STOP );
			}
		}
	}

	SetSuccessProbability( -1 );

	m_fProgressAccmulateTime	=	-1.0f;
	m_fProgressMaxTime			=	0.0f;

	UpdateItemMixInfo();
}

bool SPWindowItemMix::CheckMaterialCondition()
{
	int		iMaterialPos		=	0;
	int		iMaterialCount		=	0;
	int		iCurMaterialCount	=	0;

	for( int i = 0 ; i < MATERIAL_BOX_COUNT ; ++i )
	{
		if( m_pMaterialItem[ i ] != NULL )
			++iCurMaterialCount;
		else
			continue;

		if( CheckMaterialCondition( iMaterialCount , iMaterialPos , m_pCurItemMix , m_pMaterialItem[ i ] , false ) == false )
		{
			return false;
		}
	}

	if( iCurMaterialCount != 0 && iCurMaterialCount == iMaterialCount )
		return true;

	return false;
}

bool SPWindowItemMix::CheckMaterialCondition( int& iOutMaterialCount , int& iOutMaterialPos , SPItemMix* pMixItem , SPItem* pMaterialItem , bool bMultiCheck )
{
	iOutMaterialCount	=	0;
	iOutMaterialPos		=	0;

	if( pMixItem == NULL || pMaterialItem == NULL || g_pItemCluster == NULL )
		return false;

	SPItemStatus*	pMaterialItemStatus	=	pMaterialItem->GetItemStatus();

	if( pMaterialItemStatus == NULL )
		return false;

	if( pMaterialItemStatus->GetItemID()		<= 0 ||
		pMaterialItemStatus->GetRareLevel()		<= 0 || 
		pMaterialItemStatus->GetStackCount()	<=	0 )
	{
		return false;
	}

	bool	bFind					=	false;
	bool	bErrorMaterialDifferent	=	false;
	bool	bErrorMaterialCount		=	false;
	bool	bErrorMaterialRare		=	false;
	bool	bErrorIsMaterial		=	false;

	for( int i = 0 ; i < MAX_ITEM_UPGRADE_MATERIAL ; ++i )
	{
		if( pMixItem->m_stMaterial[ i ].m_iID			<= 0 ||
			pMixItem->m_stMaterial[ i ].m_eRare			<= 0 ||
			pMixItem->m_stMaterial[ i ].m_iCount		<= 0 )
		{
			continue;
		}

		++iOutMaterialCount;

		if( pMaterialItemStatus->GetItemID() != pMixItem->m_stMaterial[ i ].m_iID )
		{
			bErrorMaterialDifferent	=	true;
			continue;
		}

		if( pMaterialItemStatus->GetStackCount() < pMixItem->m_stMaterial[ i ].m_iCount )
		{
			bErrorMaterialCount	=	true;
			continue;
		}

		if( pMaterialItemStatus->GetRareLevel() != (int)(pMixItem->m_stMaterial[ i ].m_eRare) )
//		if( pMaterialItemStatus->GetRareLevel() != (int)(pMixItem->m_stMaterial[ i ].m_eRare) && (int)(pMixItem->m_stMaterial[ i ].m_eRare) != 0 )
		{
			bErrorMaterialRare	=	true;
			continue;
		}

		if( m_pMaterialItem[ i ] != NULL && bMultiCheck == true )
		{
			bErrorIsMaterial	=	true;
			continue;
		}

		bFind			=	true;
		iOutMaterialPos	=	i;
	}

	if( bFind == false )
	{
		//GLOBAL_STRING_ITEM_UPGRADE	eErrorID	=	GS_ITEM_UPGRADE_NULL;

		//if( bErrorMaterialRare == true )
		//	eErrorID	=	GS_ITEM_UPGRADE_MATERIAL_RARE;

		//if( bErrorMaterialCount == true )
		//	eErrorID	=	GS_ITEM_UPGRADE_MATERIAL_COUNT;

		//if( bErrorMaterialDifferent == true )
		//	eErrorID	=	GS_ITEM_UPGRADE_MATERIAL_DIFFERENT;

		//if( bErrorIsMaterial == true )
		//	eErrorID	=	GS_ITEM_UPGRADE_IS_MATERIAL;

		//SetError( eErrorID );
	}

	return bFind;
}

void SPWindowItemMix::SetLevelText( int iLevel )
{
	std::string		strSkillName	=	"No Data";
	SPSkill*		pSkill			=	SPSkillManager::GetInstance()->GetSkill( m_iCurSkillID );
	
	if( pSkill != NULL )
		strSkillName	=	pSkill->GetName();

	const char*		pszString	=	g_pResourceManager->GetGlobalString( 16010001 );
	char			szBuf[ _MAX_PATH ];

	if( pszString != NULL )
	{
		sprintf( szBuf , pszString , strSkillName.c_str() , iLevel );
	}
	else
	{
		sprintf( szBuf , "%s %d" , strSkillName.c_str() , iLevel );
	}

	SPWindow*	pWindow	=	Find( WIID_ITEM_MIX_TEXT_SUBJECT );

	if( pWindow != NULL )
		pWindow->SetWindowText( szBuf );
}

void SPWindowItemMix::SetLevelGage( float fCurValue , float fMaxValue )
{
	SPWindow*	pWindow	=	Find( WIID_ITEM_MIX_GAGE_LEVEL );

	if( pWindow != NULL )
	{
		pWindow->SPSendMessage( SPIM_SET_MAXVALUE , (int)fMaxValue );	
		pWindow->SPSendMessage( SPIM_SET_CURVALUE , (int)fCurValue );
	}

	pWindow	=	Find( WIID_ITEM_MIX_GAGE_LEVEL_TEXT );

	if( pWindow != NULL )
	{
		char	szBuf[ _MAX_PATH ];
		sprintf( szBuf , "%3.2f%%", fCurValue );
		pWindow->SetWindowText( szBuf );
	}
}

void SPWindowItemMix::SetRunGage( int iCurValue , int iMaxValue )
{
	SPWindow*	pWindow	=	Find( WIID_ITEM_MIX_GAGE_RUN );

	if( pWindow != NULL )
	{
		pWindow->SPSendMessage( SPIM_SET_MAXVALUE , iMaxValue );	
		pWindow->SPSendMessage( SPIM_SET_CURVALUE , iCurValue );
		pWindow->Show();
	}

	pWindow	=	Find( WIID_ITEM_MIX_GAGE_BG_RUN_BG );

	if( pWindow != NULL )
		pWindow->Show();

	pWindow	=	Find( WIID_ITEM_MIX_TEXT_SUCCESS );

	if( pWindow != NULL  )
		pWindow->Hide();

	pWindow	=	Find( WIID_ITEM_MIX_IMG_SUCCESS );

	if( pWindow != NULL  )
		pWindow->Hide();
}

void SPWindowItemMix::SetSuccessProbability( int iProbability )
{
	SPWindow*	pWindow	=	Find( WIID_ITEM_MIX_GAGE_RUN );

	if( pWindow != NULL )
		pWindow->Hide();

	pWindow	=	Find( WIID_ITEM_MIX_GAGE_BG_RUN_BG );

	if( pWindow != NULL )
		pWindow->Hide();

	pWindow	=	Find( WIID_ITEM_MIX_TEXT_SUCCESS );

	if( pWindow != NULL  )
	{
		char		szBuf[ _MAX_PATH ];
		const char*	pszString	=	g_pResourceManager->GetGlobalString( 16010003 );

		if( pszString != NULL )
		{
			sprintf( szBuf , pszString , iProbability );
		}
		else
		{
			sprintf( szBuf , "%d" , iProbability );
		}

		pWindow->SetWindowText( szBuf );

		if( iProbability >= 0 )
			pWindow->Show();
		else
			pWindow->Hide();
	}

	pWindow	=	Find( WIID_ITEM_MIX_IMG_SUCCESS );

	if( pWindow != NULL  )
	{
		if( iProbability >= 0 )
			pWindow->Show();
		else
			pWindow->Hide();
	}
}

void SPWindowItemMix::SendServerRun()
{
	if( m_pCurItemMix == NULL )
		return;

	if( g_pItemCluster->GetItemUpgradeState() != SPIU_STATE_NULL ) 
		return;

	if( g_pCheckManager != NULL )
	{
		if( g_pCheckManager->IsDBBlock() == true )
			return;
	}

	CPacket kPacket( ITEM_CS_MIX );
	kPacket.AddUINT32( m_pCurItemMix->m_iItemMixID );

	//////////////////////////////////////////////////////////////////////////

	CONTAINER_ITEM	Item;
	MATERIAL_DATA	stMaterialData[ MAX_ITEM_MIX_MATERIAL ];
	UINT8			uiMaterialCount		=	0;
	SPItemStatus*	pMaterialItemStatus	=	NULL;
	int				iMaterialCount		=	0;
	int				iMaterialPos		=	0;

	ZeroMemory( stMaterialData , sizeof( MATERIAL_DATA ) * MAX_ITEM_MIX_MATERIAL );

	for( int i = 0 ; i < MAX_ITEM_UPGRADE_MATERIAL ; ++i )
	{
		if( m_pMaterialItem[ i ] == NULL )
			continue;

		if( CheckMaterialCondition( iMaterialCount , iMaterialPos , m_pCurItemMix , m_pMaterialItem[ i ] , false ) == false )
		{
			return;
		}

		m_pMaterialItem[ i ]->ConvertContainerItem( &Item );

		stMaterialData[ i ].iContainerType	=	m_pMaterialItem[ i ]->GetContainerType();
		stMaterialData[ i ].iContainerSlot	=	m_pMaterialItem[ i ]->GetSlotIndex() + 1;
		stMaterialData[ i ].iItemCount		=	m_pCurItemMix->m_stMaterial[ i ].m_iCount;
		stMaterialData[ i ].iItemNo			=	Item.Item.iItemNo;

		++uiMaterialCount;
	}

	if( uiMaterialCount == 0 )
		return;

	kPacket.AddUINT8( uiMaterialCount );
	kPacket.AddData( stMaterialData , sizeof( MATERIAL_DATA ) * uiMaterialCount );

	//////////////////////////////////////////////////////////////////////////

	for( i = 0 ; i < MAX_ITEM_UPGRADE_MATERIAL ; ++i )
	{
		if( g_pCheckManager->CheckCondition( m_pCurItemMix->m_stRequireType[ i ].m_iType , m_pCurItemMix->m_stRequireType[ i ].m_iID , m_pCurItemMix->m_stRequireType[ i ].m_iValue1 , m_pCurItemMix->m_stRequireType[ i ].m_iValue2 ) == false )
			return;
	}

	//////////////////////////////////////////////////////////////////////////

	g_pNetworkManager->GetPacketHandler()->PostMsg(&kPacket);
	g_pItemCluster->SetItemUpgradeState( SPIU_STATE_ITEM_MIX );

	if( g_pCheckManager != NULL )
		g_pCheckManager->SetDBBlock( true );
}

void SPWindowItemMix::ProcessProgress( float fTime )
{
	if( m_fProgressMaxTime <= 0.0f )
		return;

	m_fProgressAccmulateTime	+=	fTime;

	if( m_fProgressAccmulateTime > m_fProgressMaxTime )
	{
		m_fProgressMaxTime	=	-1.0f;
		SetRunGage( 100 , 100 );
		SendServerRun();
		return;
	}

	int	iCurValue	=	(int)( m_fProgressAccmulateTime	*	100.f );
	int	iMaxValue	=	(int)( m_fProgressMaxTime	*	100.f );

	SetRunGage( iCurValue , iMaxValue );
}

float SPWindowItemMix::CalLevelPercent( ITEMMIX_INFO* pItemMixInfo )
{
	if( pItemMixInfo == NULL )
		return 0.0f;

	float	fLevelPercent	=	0.0f;
	float	fZeroPercent	=	(float)( pItemMixInfo->iMaxExp - pItemMixInfo->iMinExp );

	if( fZeroPercent > 0.0f )
		fLevelPercent	=	(float)( pItemMixInfo->iCurExp - pItemMixInfo->iMinExp ) / fZeroPercent * 100.0f;

	return fLevelPercent;
}

float SPWindowItemMix::CalSuccessPercent( ITEMMIX_INFO* pItemMixInfo , SPItemMix* pCurItemMix )
{
	if( pItemMixInfo == NULL || pCurItemMix == NULL )
		return 0.0f;
	
	float	fSuccessPercent	=	(float)pCurItemMix->m_iSuccess_Probability + (float)( pItemMixInfo->iSuccessProb + 10 ) * (float)( ( pItemMixInfo->iLv - pCurItemMix->m_iBase_Lv ) * pCurItemMix->m_iSuccess_Probability_Revision * 0.01f );
	float	fMinPercent		=	(float)pCurItemMix->m_iSuccess_Probability_Min;
	float	fMaxPercent		=	(float)pCurItemMix->m_iSuccess_Probability_Max;

	if( fSuccessPercent < fMinPercent )
		fSuccessPercent = fMinPercent;

	if( fSuccessPercent > fMaxPercent )
		fSuccessPercent = fMaxPercent;

	return fSuccessPercent;
}

//////////////////////////////////////////////////////////////////////////

void SPWindowItemMix::SetError( int iGloalStringID , const char* pszItemString /* = NULL  */ )
{
	DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

	const char*	pszGlobalString	=	g_pResourceManager->GetGlobalString( iGloalStringID );

	if( pszGlobalString == NULL )
		return;

	if( pszItemString != NULL )
	{
		sprintf( szErrorMsg , pszGlobalString , pszItemString );
	}
	else
	{
		strcpy( szErrorMsg , pszGlobalString );
	}

	SetError( szErrorMsg );
}

void SPWindowItemMix::SetError( const char* pszErrorString )
{
	if( g_pInterfaceManager == NULL || pszErrorString == NULL )
		return;

	D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pszErrorString , (LPARAM)&color );
}

SPItem* SPWindowItemMix::GetInvenPickUpItem()
{
	SPPlayer* pkPlayer = g_pGOBManager->GetLocalPlayer();
	if( pkPlayer == NULL )
		return NULL;

	SPPlayerInvenArchive* pkInvenArchive = pkPlayer->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return NULL;

	SPSelectItem SelItemInfo = pkInvenArchive->GetPickupItem();
	return pkInvenArchive->GetInventoryItem(SelItemInfo.m_iContainer, SelItemInfo.m_iSlotIndex);
}

void SPWindowItemMix::SetInvenItemLock( SPItem* pkItem , int iIndex )
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	if( pkItem == NULL )
		return;

	SPPlayerInvenArchive* pkInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInven ) {
		//pkInven->SetUserShopItemFake(iIndex, pkItem->GetContainerType(), pkItem->GetSlotIndex());
		pkInven->SetBlockItem(iIndex, pkItem->GetContainerType(), pkItem->GetSlotIndex());
	}
}

void SPWindowItemMix::SetInvenItemUnlock( SPItem* pkItem , int iIndex )
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	if( pkItem == NULL )
		return;

	SPPlayerInvenArchive* pkInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInven ) {
		//pkInven->ClearUserShopItem(iIndex);
		pkInven->ClearBlockItem(iIndex);
	}
}

void SPWindowItemMix::SetInvenItemAllUnlock()
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	SPPlayerInvenArchive* pkInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInven ) {
		//pkInven->ClearAllUserShopItem();
		pkInven->ClearAllBlockItem();
	}
}

void SPWindowItemMix::SendParentAction( UINT64 uiAction , unsigned int uiIndex /* = 0  */, bool bSendInit /* = false  */ )
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	if( bSendInit )
		g_pGOBManager->GetLocalPlayer()->InitSendAction();

	if( uiIndex )
		g_pGOBManager->GetLocalPlayer()->SetSendAttackIndex(uiIndex);

	g_pGOBManager->GetLocalPlayer()->SetControlAction(uiAction);
	g_pGOBManager->GetLocalPlayer()->SendActionPacket();
}
