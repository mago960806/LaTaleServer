// ***************************************************************
//  SPWindowLove		Version:  1.0		Date: 2008/09/25
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowGauge.h"
#include "SPWindowDigitBoard.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include "SPNoticeBox.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPInterfaceManager.h"

//#include "SPManager.h"

#include "SPItemAttr.h"
#include "SPItem.h"
#include "SPItemCluster.h"

#include "SPSubGameManager.h"
#include "SPCommandConvert.h"
#include "SPGOBStatus.h"
#include "SPGOBManager.h"
#include "SPGOBCoordPhysics.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayerInvenArchive.h"
#include "SPGuildArchive.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayer.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPStageLoader.h"

#include "SPUIUnit.h"
#include "SPUIItemUnit.h"
#include "SPUIUnitManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPLocalizeManager.h"

#include "SPCheckManager.h"
#include "GlobalDefines_Share.h"

#include "SPAbuseFilter.h"

#include "SPLoveManager.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPUtil.h"

#include "SPUIManager.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"

#include "SPWindowLoveMsgInput.h"
#include "SPWindowLoveMsgView.h"

#include "SPWindowLove.h"

SPWindowLove::SPWindowLove( WND_ID WndClassID , INSTANCE_ID InstanceID )
: SPWindow( WndClassID , InstanceID )
{	
	Init();	
}

SPWindowLove::SPWindowLove( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent /* = NULL  */ )
: SPWindow( WNDID_LOVE , InstanceID , iX , iY , iCX , iCY , pParent )
{	
	Init();	
}

SPWindowLove::~SPWindowLove()
{
	Clean();
}

void SPWindowLove::Init()
{
	m_strToolTip	= "[SPWindowLove]";

	//////////////////////////////////////////////////////////////////////////

	SPWindowButton*			pWindowButton	=	NULL;
	SPWindowStatic*			pWindowStatic	=	NULL;
	SPWindowEdit*			pWindowEdit		=	NULL;
	SPWindowGauge*			pWindowGage		=	NULL;
	SPWindowDigitBoard*		pWindowBoard	=	NULL;

	SPWindowStatic*			pWindowParent	=	NULL;
	SPWindowStatic*			pWindowParent1	=	NULL;
	SPWindowStatic*			pWindowParent2	=	NULL;
	SPWindowStatic*			pWindowParent3	=	NULL;
	SPWindowStatic*			pWindowParent4	=	NULL;
	SPWindowStatic*			pWindowParent5	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	SPWindow_Base*			pattrwnd_Map	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	m_bUse			=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_LOVE );

	//////////////////////////////////////////////////////////////////////////

	if( g_UIManager->Load( RES_FNAME_XML_UI_LOVE ) == true )
	{
		//////////////////////////////////////////////////////////////////////////

		pWindowParent1	=	(SPWindowStatic*)this;

		//////////////////////////////////////////////////////////////////////////

		pWindowParent	=	pWindowParent1;

		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_RB2				, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_MB2				, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_LB2				, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_MM2				, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

			if( pWindowStatic != NULL)
				pWindowStatic->SetNullWindow( 1 );

			pWindowParent2	=	pWindowStatic;
			pWindowParent	=	pWindowParent2;

			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_3RD_LAYER_BOARD_BOTTOM2	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG					, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

				if( pWindowStatic != NULL)
					pWindowStatic->SetNullWindow( 1 );

				pWindowParent3	=	pWindowStatic;
				pWindowParent	=	pWindowParent3;

				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG_MM		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

					if( pWindowStatic != NULL)
						pWindowStatic->SetNullWindow( 1 );

					pWindowParent4	=	pWindowStatic;
					pWindowParent	=	pWindowParent4;

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_POINT_BG_3RD_LAYER_R2	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_POINT_BG_3RD_LAYER_M2	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

						if( pWindowStatic != NULL)
							pWindowStatic->SetNullWindow( 1 );

						pWindowParent5	=	pWindowStatic;
						pWindowParent	=	pWindowParent5;

						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_POINT2	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

					pWindowParent	=	pWindowParent4;

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_POINT_BG_3RD_LAYER_L2	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CATION_SLASH			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_POINT_BG_3RD_LAYER_R	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_POINT_BG_3RD_LAYER_M	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

						if( pWindowStatic != NULL)
							pWindowStatic->SetNullWindow( 1 );

						pWindowParent5	=	pWindowStatic;
						pWindowParent	=	pWindowParent5;

						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_POINT1	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

					pWindowParent	=	pWindowParent4;

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_POINT_BG_3RD_LAYER_L	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CATION_POINT			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_POINT_BG_R				, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_POINT_BG_M				, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_POINT_BG_L				, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_STATUS_BG_3RD_LAYER_R	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_STATUS_BG_3RD_LAYER_M	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

						if( pWindowStatic != NULL)
							pWindowStatic->SetNullWindow( 1 );

						pWindowParent5	=	pWindowStatic;
						pWindowParent	=	pWindowParent5;

						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_STATUS	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

					pWindowParent	=	pWindowParent4;

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_STATUS_BG_3RD_LAYER_L	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CAPTION_STATUS			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_STATUS_BG_R				, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_STATUS_BG_M				, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_STATUS_BG_L				, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_DAY_BG_3RD_LAYER_R		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_DAY_BG_3RD_LAYER_M		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

						if( pWindowStatic != NULL)
							pWindowStatic->SetNullWindow( 1 );

						pWindowParent5	=	pWindowStatic;
						pWindowParent	=	pWindowParent5;

						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_DAY	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

					pWindowParent	=	pWindowParent4;

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_DAY_BG_3RD_LAYER_L	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CAPTION_DAY2		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CAPTION_DAY1		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_DAY_BG_R			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_DAY_BG_M			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_DAY_BG_L			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CAPTION_LOVEINFO3	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

				pWindowParent	=	pWindowParent3;

				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG_MB		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG_RB		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG_LB		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG_RM		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG_LM		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG_RT		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG_MT		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG_LT		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

			pWindowParent	=	pWindowParent2;

			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BTN_EXIT2					, SPWindowButton , pWindowButton , pattrwnd_Map , pWindowParent );
			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_EXPANSION_BTN2		, SPWindowButton , pWindowButton , pattrwnd_Map , pWindowParent );
			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_EXPANSION_BTN1		, SPWindowButton , pWindowButton , pattrwnd_Map , pWindowParent );
			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_EXPANSION_BG2		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_3RD_LAYER		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

				if( pWindowStatic != NULL)
					pWindowStatic->SetNullWindow( 1 );

				pWindowParent3	=	pWindowStatic;
				pWindowParent	=	pWindowParent3;

				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CAPTION_LOVEINFO1	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG_R		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG_M		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

					if( pWindowStatic != NULL)
						pWindowStatic->SetNullWindow( 1 );

					pWindowParent4	=	pWindowStatic;
					pWindowParent	=	pWindowParent4;

					pWindowEdit	=	new SPWindowEdit( WIID_LOVE_TARGET_EDIT , 71 , 5 , 180 , 17 , pWindowParent );
					pWindowEdit->SetWindowType( WND_TYPE_LEFTBASE );
					pWindowEdit->SetMargin( 3 , 4 );
					pWindowEdit->SetImageNormalHead	( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 501 , 46 , 503 , 63 );
					pWindowEdit->SetImageNormalBody	( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 505 , 46 , 507 , 63 );
					pWindowEdit->SetImageNormalTail	( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 509 , 46 , 511 , 63 );
					pWindowEdit->SetImageFocusHead	( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 1 , 261 , 3 , 278 );
					pWindowEdit->SetImageFocusBody	( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 5 , 261 , 7 , 278 );
					pWindowEdit->SetImageFocusTail	( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 9 , 261 , 11 , 278 );
					pWindowEdit->SetImageDeco		( "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG" , 501 , 35 , 511 , 45 );
					pWindowEdit->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
					pWindowEdit->SetFormat( DT_LEFT | DT_VCENTER );
					pWindowEdit->SetLimitText( LEN_NAME + 1 );

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAPSTRING_BG_RB	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAPSTRING_BG_MB	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAPSTRING_BG_LB	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAPSTRING_BG_RM	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAPSTRING_BG_MM	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

						if( pWindowStatic != NULL)
							pWindowStatic->SetNullWindow( 1 );

						pWindowParent5	=	pWindowStatic;
						pWindowParent	=	pWindowParent5;

						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAPSTRING2			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CAPTION_LOCALITY	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CAPTION_CONNECTION	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAPSTRING			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

					pWindowParent	=	pWindowParent4;

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAPSTRING_BG_LM	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAPSTRING_BG_RT	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAPSTRING_BG_MT	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAPSTRING_BG_LT	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAP_ARROW		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAP_BG			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

						if( pWindowStatic != NULL)
							pWindowStatic->SetNullWindow( 1 );

						pWindowParent5	=	pWindowStatic;
						pWindowParent	=	pWindowParent5;

						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAP_QUESTIONMARK	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_MAP					, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

					pWindowParent	=	pWindowParent4;

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BTN_PROPOSE2	, SPWindowButton , pWindowButton , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BTN_PROPOSE1	, SPWindowButton , pWindowButton , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_ELY_BG			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

						if( pWindowStatic != NULL)
							pWindowStatic->SetNullWindow( 1 );

						pWindowParent5	=	pWindowStatic;
						pWindowParent	=	pWindowParent5;

						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_ELY			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

					pWindowParent	=	pWindowParent4;

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_STRING_BG_R		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_STRING_BG_M		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

						if( pWindowStatic != NULL)
							pWindowStatic->SetNullWindow( 1 );

						pWindowParent5	=	pWindowStatic;
						pWindowParent	=	pWindowParent5;

						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_STRING		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

						if( pWindowStatic != NULL )
							pWindowStatic->SetMultiLine( true , 2 );

					pWindowParent	=	pWindowParent4;

					pWindowBoard = new SPWindowDigitBoard( WIID_LOVE_GAGE_PERCENTAGE , 107 , 30 , 41 ,  7 , pWindowParent );
					pWindowBoard->SetFormat( DT_LEFT );
					pWindowBoard->SetWindowText( "100.00%" );

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_STRING_BG_L		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_ITEMSLOT		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

						if( pWindowStatic != NULL)
							pWindowStatic->SetNullWindow( 1 );

						pWindowParent5	=	pWindowStatic;
						pWindowParent	=	pWindowParent5;

						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_ITEM		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

					pWindowParent	=	pWindowParent4;

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_GAUGE			, SPWindowGauge , pWindowGage , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CAPTION_ID		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

				pWindowParent	=	pWindowParent3;

				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_LOVEINFO_BG_L		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CAPTION_LOVEINFO2	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_NAME_BG_R			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_NAME_BG_M			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

					if( pWindowStatic != NULL)
						pWindowStatic->SetNullWindow( 1 );

					pWindowParent4	=	pWindowStatic;
					pWindowParent	=	pWindowParent4;

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_NAME_BG_3RD_LAYER_R	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_NAME_BG_3RD_LAYER_M	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

						if( pWindowStatic != NULL)
							pWindowStatic->SetNullWindow( 1 );

						pWindowParent5	=	pWindowStatic;
						pWindowParent	=	pWindowParent5;

						UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_NAME			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

					pWindowParent	=	pWindowParent4;

					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_NAME_BG_3RD_LAYER_L	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
					UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CAPTION_LOVER		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

				pWindowParent	=	pWindowParent3;

				UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_NAME_BG_L			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

			pWindowParent	=	pWindowParent2;

			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_3RD_LAYER_BOARD_TOP2	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CLOSE					, SPWindowButton , pWindowButton , pattrwnd_Map , pWindowParent );
			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CAPTION_INFO			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_CAPTION_ICON			, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

		pWindowParent	=	pWindowParent1;

		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_RM2		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_LM2		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_RT2		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_MT2		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_LT2		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_RB1		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_MB1		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_LB1		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_RM1		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_MM1		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

			if( pWindowStatic != NULL)
				pWindowStatic->SetNullWindow( 1 );

			pWindowParent2	=	pWindowStatic;
			pWindowParent	=	pWindowParent2;

			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_3RD_LAYER_BOARD_BOTTOM1	, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_3RD_LAYER_BOARD_TOP1		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BTN_EXIT1					, SPWindowButton , pWindowButton , pattrwnd_Map , pWindowParent );
//			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_EXPANSION_BTN1				, SPWindowButton , pWindowButton , pattrwnd_Map , pWindowParent );
			UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_EXPANSION_BG1				, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

		pWindowParent	=	pWindowParent1;

		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_LM1		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_RT1		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_MT1		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );
		UI_SPWINDOW_DEFAULT_BASE( WIID_LOVE_BG_LT1		, SPWindowStatic , pWindowStatic , pattrwnd_Map , pWindowParent );

		//////////////////////////////////////////////////////////////////////////

		g_UIManager->Release();
	}

	InitUIItemUnit();
	UpdatePosUIItemUnit();

	SetCtrlItemBox( 180121001 );

	m_fAccumulateMemberUpdate	=	0.0f;
	m_fLimitMemberUpdate		=	1.0f;

	m_fAccumulateStatusUpdate	=	0.0f;
	m_fLimitStatusUpdate		=	0.5f;
	m_bStatusUpdate				=	false;

	SetCtrlGageValue( Find( WIID_LOVE_GAUGE , true ) , Find( WIID_LOVE_GAGE_PERCENTAGE , true ) , 0 , 100 );

	SetWindowState( (LOVE_WINDOW_STATE)(LOVE_WINDOW_FRIENDSHIP | LOVE_WINDOW_LOVE_INFO) , true );

	OnPurge( NULL , NULL );
}

void SPWindowLove::Clean()
{
	SPWindow::Clean();

	m_UIUnitManager.ClearAll( TRUE );
}

void SPWindowLove::Process(float fTime)
{
	ProcessButtonUpdate( fTime );

	if( !m_bShow )	return;

	SPWindow::Process( fTime );

	m_UIUnitManager.Process( fTime );

	ProcessMemberUpdate( fTime );
	ProcessStatusUpdate( fTime );
}

void SPWindowLove::Render(float fTime)
{
	if( !m_bShow )	return;

	g_pVideo->Flush();

	SPWindow::RenderReverse( fTime , 0 );

	g_pVideo->Flush();

	if( ( ( m_eLoveWindowState & LOVE_WINDOW_WEDDING ) == LOVE_WINDOW_WEDDING ) == false )
	{
		m_UIUnitManager.Render();

		g_pVideo->Flush();
	}
}

void SPWindowLove::Show(bool bWithChild)
{
	if( m_bUse == false )
		return;

	//SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	//if( pLoveManager != NULL )
	//{
	//	static bool bSet = false;

	//	WEDDING_INFO	stLoveInfo;

	//	stLoveInfo.iPartingNum	=	60 * 10;

	//	if( bSet == false )
	//	{
	//		pLoveManager->SetLoveInfo( stLoveInfo );
	//		bSet	=	true;
	//	}
	//}

//	SPWindow::Show( bWithChild );

	m_fAccumulateMemberUpdate	=	0.0f;

	UpdateWIndowAll( true );
}

void SPWindowLove::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide( bSendServer );

	SetInvenItemAllUnlock();
}

void SPWindowLove::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	UpdatePosUIItemUnit();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowLove )

	//	Common
	SPIMESSAGE_COMMAND(	SPIM_KEY_RETURN				,																	OnKeyReturn				)

	SPIMESSAGE_COMMAND( SPIM_NOTICE_YES				,																	OnNoticeYes				)
	SPIMESSAGE_COMMAND( SPIM_NOTICE_NO				,																	OnNoticeNo				)

	SPIMESSAGE_COMMAND( SPIM_REFRESH				,																	OnRefresh				)
	SPIMESSAGE_COMMAND(	SPIM_PURGE					,																	OnPurge					)

	SPIMESSAGE_COMMAND(	SPIM_LOVE_INFO				,																	OnLoveInfo				)
//	SPIMESSAGE_COMMAND(	SPIM_LOVE_REQUEST			,																	OnLoveRequest			)
	SPIMESSAGE_COMMAND(	SPIM_LOVE_REQUEST_RECV		,																	OnLoveRequestRecv		)
	SPIMESSAGE_COMMAND(	SPIM_LOVE_RESPONSE			,																	OnLoveResponse			)
	SPIMESSAGE_COMMAND(	SPIM_LOVE_UPDATEMEMBER		,																	OnLoveUpdateMember		)
//	SPIMESSAGE_COMMAND(	SPIM_LOVE_PARTING			,																	OnLoveParing			)
//	SPIMESSAGE_COMMAND(	SPIM_LOVE_CHAR_INFO			,																	OnLoveCharInfo			)
	SPIMESSAGE_COMMAND(	SPIM_LOVE_REFRESH_BUTTON	,																	OnLoveRefreshButton		)

	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS				,	WIID_LOVE_TARGET_EDIT										,	OnEditSetFocus			)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS				,	WIID_LOVE_TARGET_EDIT										,	OnEditKillFocus			)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_LOVE_CLOSE												,	OnClose					)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_LOVE_BTN_EXIT1											,	OnClose					)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_LOVE_BTN_EXIT2											,	OnClose					)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_LOVE_EXPANSION_BTN1									,	OnExpansion1			)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_LOVE_EXPANSION_BTN2									,	OnExpansion2			)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_LOVE_BTN_PROPOSE1										,	OnPropose1				)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_DOWN			,	WIID_LOVE_BTN_PROPOSE2										,	OnPropose2				)

SPIMESSAGE_OBJ_MAP_END(	SPWindow	)

int SPWindowLove::OnKeyReturn( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return -1;

	SPWindow*	pWindow	=	Find( WIID_LOVE_TARGET_EDIT , true );

	if( pWindow != NULL )
	{
		if( pWindow->IsFocus() == false )
			return - 1;
	}

	OnPropose1( NULL , NULL );

	return 1;
}

int SPWindowLove::OnNoticeYes( WPARAM wParam, LPARAM lParam )
{
	if( m_eNoticeType != LOVE_NOTICE_NULL )
		SendResponse( (UINT8)m_eNoticeType , 1 );

	ClearResponse();

	return 1;
}

int SPWindowLove::OnNoticeNo( WPARAM wParam, LPARAM lParam )
{
	if( m_eNoticeType != LOVE_NOTICE_NULL )
		SendResponse( (UINT8)m_eNoticeType , 0 );

	ClearResponse();

	return 1;
}

int SPWindowLove::OnRefresh( WPARAM wParam, LPARAM lParam )
{
	m_bStatusUpdate	=	true;
	return 1;
}

int SPWindowLove::OnPurge( WPARAM wParam, LPARAM lParam )
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
		pLoveManager->ClearVariables();

	m_pConsumeItem			=	NULL;

	ClearResponse();

	OnLoveRefreshButton( NULL , NULL );

	m_iStageID				=	-1;
	m_iMapGroupID			=	-1;
	m_iChannelID			=	-1;

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_LOVE_TARGET_EDIT , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( NULL );

	return 1;
}

int SPWindowLove::OnLoveInfo( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL || g_pGOBManager == NULL )
		return 1;

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	WEDDING_INFO			WeddingInfo;					// 웨딩정보

	pPacket->Extract( &WeddingInfo , sizeof( WeddingInfo ) );

	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager == NULL )
		return 1;

	pLoveManager->SetLoveInfo( WeddingInfo );

	SetPlayerStatus();

	OnRefresh( NULL , NULL ); 

	return 1;
}

//int SPWindowLove::OnLoveRequest( WPARAM wParam, LPARAM lParam )
//{
//	CPacket*		pPacket		=	(CPacket*)lParam;
//
//	if( pPacket == NULL )
//		return 1;
//
//	GU_ID				ObjectGUID	=	0;
//
//	pPacket->ExtractUINT64( &ObjectGUID );
//
//	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();
//
//	if( pLocalPlayer == NULL )
//		return 1;
//
//	if( pLocalPlayer->GetGUID() != ObjectGUID )
//		return 1;
//
//	return 1;
//}

int SPWindowLove::OnLoveRequestRecv( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL || g_pGOBManager == NULL )
		return 1;

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	UINT8	iWeddingType;					// 1:교제, 2:프로포즈, 3:결혼
	char	szCharName[ LEN_NAME + 1 ];		// 이름 

	pPacket->ExtractUINT8( &iWeddingType );
	pPacket->Extract( szCharName	,	sizeof( szCharName )	);

	int	iGlobalString	=	0;
	int	iDealyTime		=	10;

	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
		iDealyTime	=	pLoveManager->GetRequire( LOVE_LDT_REQUIRE_COMMON_MESSAGEBOX_REMAIN_TIME );

	if( iWeddingType == LOVE_NOTICE_FRIENDSHIP )
	{
		iGlobalString	=	5208102;
	}
	else if( iWeddingType == LOVE_NOTICE_PROPOSE )
	{
		iGlobalString	=	5208136;
	}
	else if( iWeddingType == LOVE_NOTICE_WEDDING )
	{
		iGlobalString	=	5208137;
	}
	
	SetCtrlNotice( (LOVE_NOTICE_TYPE)iWeddingType , iDealyTime , iGlobalString , szCharName );

	return 1;
}

int SPWindowLove::OnLoveResponse( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL || g_pGOBManager == NULL )
		return 1;

	GU_ID				ObjectGUID	=	0;

	pPacket->ExtractUINT64( &ObjectGUID );

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	//WEDDING_SC_RESPONSE,
	UINT8					iWeddingType;							// 1:교제, 2:프로포즈, 3:결혼
	GLOBAL_STRING_ID		ErrorID;								// 에러아이디(0이면 에러없음)
	char					szCharName[ LEN_NAME+1 ]	=	{ 0, };
	UINT8					byAccept;								// 1:수락, 2:거절

	pPacket->ExtractUINT8( &iWeddingType );
	pPacket->ExtractUINT32( &ErrorID );
	pPacket->Extract( szCharName	,	sizeof( szCharName )	);

	char		szLocalPlayer[ LEN_NAME + 1 ];
	pLocalPlayer->SPGOBPerformMessage( SPGM_GETGOBNAME , (LPARAM)szLocalPlayer );

	if( ErrorID != 0 )
	{
		SetError( ErrorID , szCharName );
		return 1;
	}

	pPacket->ExtractUINT8( &byAccept );

	if( iWeddingType == WEDDING_TYPE_LOVE || iWeddingType == WEDDING_TYPE_PROPOSE )
	{
		OnLoveRefreshButton( NULL , NULL );
	}

	if( byAccept == 1 )
	{
		SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();
		SPLoveManager*			pLoveManager	=	SPLoveManager::GetInstance();

		if( pLoveManager == NULL || pInvenArchive == NULL )
			return 1;

		WEDDING_INFO			WeddingInfo;						// 웨딩정보
		TRASH_ITEM				TrashItem;						// 사용할 아이템

		pPacket->ExtractStruct( &WeddingInfo , sizeof(WEDDING_INFO) );
		pPacket->ExtractStruct( &TrashItem , sizeof( TRASH_ITEM ) );

		--TrashItem.iContainerSlot.SlotIndex;

		pLoveManager->SetLoveInfo( WeddingInfo );
		pInvenArchive->TrashItem( TrashItem );

		SetPlayerStatus();

		OnRefresh( NULL , NULL );

		if( iWeddingType == WEDDING_TYPE_LOVE )
		{
			SetError( 5208109 , szCharName );

			SPWindow*	pWindow;

			pWindow	=	Find( WIID_LOVE_TARGET_EDIT , true );

			if( pWindow != NULL )
				pWindow->SetWindowText( NULL );
		}
		else if( iWeddingType == WEDDING_TYPE_PROPOSE )
		{
			SetError( 5208119 , szLocalPlayer , szCharName );
		}
		//else if( iWeddingType == WEDDING_TYPE_WEDDING )
		//{
		//	if( g_pInterfaceManager != NULL )
		//	{
		//		g_pInterfaceManager->ShowWindow( WIID_LOVEMSG_INPUT , true );
		//	}
		//}
	}
	else if( byAccept == 0 )
	{
		if( iWeddingType == WEDDING_TYPE_LOVE )
		{
			SetError( 5208110 , szCharName );
		}
		else if( iWeddingType == WEDDING_TYPE_PROPOSE )
		{
			SetError( 5208120 , szLocalPlayer , szCharName );
		}
		else if( iWeddingType == WEDDING_TYPE_WEDDING )
		{
			SetError( 5208124 , szCharName );
		}
	}

	return 1;
}

int SPWindowLove::OnLoveUpdateMember( WPARAM wParam, LPARAM lParam )
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

	GU_ID			PartnerID;							// 배우자 GUID, 오프라인일경우 0으로
	UINT16			iServerID;							// 서버아이디, 오프라인일경우 0으로 
	int				usStageID;							// stageID, 오프라인일경우나 다른채널일때 -1으로
	int				usMapGroupID;						// MapGroupID, 오프라인일경우나 다른채널일때 -1으로

	pPacket->ExtractUINT64( &PartnerID );
	pPacket->ExtractUINT16( &iServerID );
	pPacket->Extract( &usStageID		,	sizeof( usStageID )			);
	pPacket->Extract( &usMapGroupID		,	sizeof( usMapGroupID )		);

	m_iStageID		=	-1;
	m_iMapGroupID	=	-1;
	m_iChannelID	=	(int)iServerID;

	if( g_pNetworkManager != NULL )
	{
		if( g_pNetworkManager->GetServerID() == iServerID )
		{
			m_iStageID		=	usStageID;
			m_iMapGroupID	=	usMapGroupID;
		}
		else
		{
			PartnerID	=	0;
		}
	}

	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
	{
		pLoveManager->SetLifePartnerGUID( PartnerID );
	}

	SetPartnerInfo( m_iStageID , m_iMapGroupID , m_iChannelID );

	return 1;
}

int SPWindowLove::OnLoveRefreshButton( WPARAM wParam, LPARAM lParam )
{
	int		iFlag	=	(int)wParam;

	if( iFlag == 0 )
	{
		m_bButtonUpdate	=	false;
	}
	else
	{
		SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

		if( pLoveManager == NULL )
			return 1;

		m_bButtonUpdate				=	true;
		m_fAccumulateButtonUpdate	=	0.0f;
		m_fLimitButtonUpdate		=	(float)( pLoveManager->GetRequire( LOVE_LDT_REQUIRE_COMMON_MESSAGEBOX_REMAIN_TIME ) );
	}

	UpdateWIndowAll( m_bShow );

	return 1;
}

//int SPWindowLove::OnLoveParing( WPARAM wParam, LPARAM lParam )
//{
//	return 1;
//}

//int SPWindowLove::OnLoveCharInfo( WPARAM wParam, LPARAM lParam )
//{
//	CPacket*		pPacket		=	(CPacket*)lParam;
//
//	if( pPacket == NULL )
//		return 1;
//
//	GU_ID				ObjectGUID	=	0;
//
//	pPacket->ExtractUINT64( &ObjectGUID );
//
//	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();
//
//	if( pLocalPlayer == NULL )
//		return 1;
//
//	if( pLocalPlayer->GetGUID() != ObjectGUID )
//		return 1;
//
//	GLOBAL_STRING_ID		iError;						// 결혼 정보가 없는사람은 에러로 표시
//	int						iLv;
//	int						iDay;
//	char					szCharName[LEN_NAME+1];		// 교제자 캐릭터이름
//
//	pPacket->ExtractUINT32( &iError );
//
//	if( iError != 0 )
//	{
//		SetError( iError );
//		return 1;
//	}
//
//	pPacket->Extract( &iLv			,	sizeof( iLv )			);
//	pPacket->Extract( &iDay			,	sizeof( iDay )			);
//	pPacket->Extract( szCharName	,	sizeof( szCharName )	);
//
//	return 1;
//}

int SPWindowLove::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SetKeyInput( TRUE );
	return 1;
}

int SPWindowLove::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SetKeyInput( FALSE );
	return 1;
}

int SPWindowLove::OnClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

int SPWindowLove::OnPropose1( WPARAM wParam, LPARAM lParam )
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager == NULL )
		return 1;

	if( g_pCheckManager->CheckPCStatus( CHECK_STATUS_LOVE_STATE , WEDDING_STATUS_NONE , WEDDING_STATUS_NONE ) == false )
		return 1;

	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_LOVE_TARGET_EDIT , true );

	if( pWindow == NULL )
		return 1;

	const char*	pszTargetName	=	pWindow->GetWindowText();

	if( pszTargetName == NULL )
		return 1;

	std::string	strTargetName	=	pszTargetName;

	if( strTargetName.empty() == true )
		return 1;

	if( g_pCheckManager->IsCharNameCompare( NULL , strTargetName.c_str() , true , true ) == true )
		return 1;

	int	iConsumeEly		=	pLoveManager->GetRequire( LOVE_LDT_REQUIRE_FRIENDSHIP_CONSUME_ELY );

	if( g_pCheckManager->CheckPCStatus( CHECK_STATUS_MONEY , iConsumeEly , 0 ) == false )
	{
		return 1;
	}

	int	iConsumeItem	=	pLoveManager->GetRequire( LOVE_LDT_REQUIRE_FRIENDSHIP_CONSUME_ITEM );

	CONTAINER_SLOT		tItemSlot;					// 소비되야하는 아이템

	ZeroMemory( &tItemSlot , sizeof( tItemSlot ) );

	if( iConsumeItem > 0 )
	{
		if( IsConsumeItem() == false )
		{
			return 1;
		}

		tItemSlot.ContainerType	=	m_pConsumeItem->GetContainerType();
		tItemSlot.SlotIndex		=	m_pConsumeItem->GetSlotIndex() + 1;
	}

	pLoveManager->SendWeddingRequest( WEDDING_TYPE_LOVE , iConsumeEly , tItemSlot , strTargetName.c_str() );

	//////////////////////////////////////////////////////////////////////////

	//m_bButtonUpdate				=	true;
	//m_fAccumulateButtonUpdate	=	0.0f;
	//m_fLimitButtonUpdate		=	(float)( pLoveManager->GetRequire( LOVE_LDT_REQUIRE_COMMON_MESSAGEBOX_REMAIN_TIME ) );

	UpdateWIndowAll( m_bShow );

	//RequestFriendShip( strTargetName.c_str() );

	//g_pInterfaceManager->ShowWindow( WIID_LOVEMSG_INPUT , true );

	//LOVEMSG_VIEW_DATA	stLoveMsgViewData;

	//stLoveMsgViewData.m_strCharID	=	"AJJIYA";
	//stLoveMsgViewData.m_strMsg		=	"AJJIYAAJJIYAAJJIYAAJJIYAAJJIYAAJJIYAAJJIYA";

	//g_pInterfaceManager->SPChildWndSendMessage( WIID_LOVEMSG_VIEW , SPIM_ADD_ITEM , NULL , (LPARAM)&stLoveMsgViewData );

	//if( g_pInterfaceManager->IsCutIn() == true )
	//{
	//	g_pEventManager->OnCutInEnd();
	//}
	//else
	//{
	//	g_pEventManager->SetCutIn( true );
	//	g_pEventManager->SetCutInState( CUT_IN_STATE_LOVE );
	//}

	return 1;
}

int SPWindowLove::OnPropose2( WPARAM wParam, LPARAM lParam )
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager == NULL || g_pGOBManager == NULL || g_pCheckManager == NULL )
		return 1;

	if( g_pCheckManager->CheckPCStatus( CHECK_STATUS_LOVE_STATE , WEDDING_STATUS_LOVE , WEDDING_STATUS_LOVE ) == false )
		return 1;

	const char*	pstrLifePartner	=	pLoveManager->GetLifePartnerString();

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	SPPlayer*	pLifePartner	=	NULL;

	pLifePartner	=	(SPPlayer*)g_pGOBManager->Find( pLoveManager->GetLifePartnerGUID() );

	if( pLifePartner == NULL )
	{
		pLifePartner	=	g_pGOBManager->FindPlayerByName( pLoveManager->GetLifePartnerString() );
	}

	if( pLifePartner == NULL )
	{
		SetError( 5208111 );
		return 1;
	}

	char		szFindLifePartner[ LEN_NAME + 1 ];
	pLifePartner->SPGOBPerformMessage( SPGM_GETGOBNAME , (LPARAM)szFindLifePartner );

	if( g_pCheckManager->IsCharNameCompare( pstrLifePartner , szFindLifePartner , false , true ) == false )
	{
		SetError( 5208111 );
		return 1;
	}

	if( g_pCheckManager->IsCharNameCompare( NULL , pstrLifePartner , true , true ) == true )
	{
		SetError( 5208111 );
		return 1;
	}

	if( pLocalPlayer->GetCurStageID()		!=	pLifePartner->GetCurStageID() ||
		pLocalPlayer->GetCurMapGroupID()	!=	pLifePartner->GetCurMapGroupID() )
	{
		SetError( 5208111 );
		return 1;
	}

	float	fResultPosX			=	abs( pLocalPlayer->GetPosX() - pLifePartner->GetPosX() );
	float	fResultPosY			=	abs( pLocalPlayer->GetPosY() - pLifePartner->GetPosY() );
	float	fPossibleDistance	=	(float)( pLoveManager->GetRequire( LOVE_LDT_REQUIRE_PROPOSE_POSSIBLE_DISTANCE ) );

	if( fResultPosX > fPossibleDistance || fResultPosY > fPossibleDistance )
	{
		SetError( 5208111 );
		return 1;
	}

	int	iConsumeEly		=	pLoveManager->GetRequire( LOVE_LDT_REQUIRE_PROPOSE_COMSUME_ELY );

	if( g_pCheckManager->CheckPCStatus( CHECK_STATUS_MONEY , iConsumeEly , 0 ) == false )
	{
		SetError( 27000005 );
		return 1;
	}

	int	iConsumeItem	=	pLoveManager->GetRequire( LOVE_LDT_REQUIRE_PROPOSE_COMSUME_ITEM );

	CONTAINER_SLOT		tItemSlot;					// 소비되야하는 아이템

	ZeroMemory( &tItemSlot , sizeof( tItemSlot ) );

	if( iConsumeItem > 0 )
	{
		if( IsConsumeItem() == false )
		{
			return 1;
		}

		tItemSlot.ContainerType	=	m_pConsumeItem->GetContainerType();
		tItemSlot.SlotIndex		=	m_pConsumeItem->GetSlotIndex() + 1;
	}

	pLoveManager->SendWeddingRequest( WEDDING_TYPE_PROPOSE , iConsumeEly , tItemSlot , pstrLifePartner );

	//////////////////////////////////////////////////////////////////////////

	//m_bButtonUpdate				=	true;
	//m_fAccumulateButtonUpdate	=	0.0f;
	//m_fLimitButtonUpdate		=	(float)( pLoveManager->GetRequire( LOVE_LDT_REQUIRE_COMMON_MESSAGEBOX_REMAIN_TIME ) );

	UpdateWIndowAll( m_bShow );

	return 1;
}

int SPWindowLove::OnExpansion1( WPARAM wParam, LPARAM lParam )
{
	SetWindowState( (LOVE_WINDOW_STATE)(m_eLoveWindowState | LOVE_WINDOW_LOVE_INFO) , true );
	SetRectSize( 282 , 359 );
	UpdateWIndowAll( m_bShow );

	return 1;
}

int SPWindowLove::OnExpansion2( WPARAM wParam, LPARAM lParam )
{
	SetWindowState( (LOVE_WINDOW_STATE)(m_eLoveWindowState ^ LOVE_WINDOW_LOVE_INFO) , true );
	SetRectSize( 282 , 241 );
	UpdateWIndowAll( m_bShow );

	return 1;
}

void SPWindowLove::InitUIItemUnit()
{
	int	iUIItemUnitCount = LOVE_ITEM_UNIT_COUNT;

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

void SPWindowLove::UpdatePosUIItemUnit()
{
	RECT			rcPos;
	SPUIItemUnit*	pUIItemUnit			=	NULL;
	SPWindow*		pWindow				=	NULL;
	int				iUIItemUnitCount	=	1;

	for( int i = 0 ; i < iUIItemUnitCount ; ++i )
	{
		pUIItemUnit	=	(SPUIItemUnit*)m_UIUnitManager.GetUnit( i );

		if( pUIItemUnit == NULL )
			continue;

		pWindow	=	Find( WIID_LOVE_ITEM , true );

		if( pWindow == NULL )
			continue;

		pWindow->GetWindowRect( rcPos );
		pUIItemUnit->SetPos( rcPos.left , rcPos.top );
		pUIItemUnit->ProcessCount();
	}
}

void SPWindowLove::SetCtrlWindowShow( SPWindow* pWindow , bool bShow )
{
	if( pWindow == NULL )
		return;

	if( bShow == true )
		pWindow->Show();
	else
		pWindow->Hide();
}

bool SPWindowLove::GetCtrlWindowShow( SPWindow* pWindow )
{
	if( pWindow == NULL )
		return false;

	return pWindow->IsShow();
}

void SPWindowLove::SetCtrlWindowEnable( SPWindow* pWindow , bool bEnable )
{
	if( pWindow == NULL )
		return;

	pWindow->SetEnable( bEnable );
}

bool SPWindowLove::GetCtrlWindowEnable( SPWindow* pWindow )
{
	if( pWindow == NULL )
		return false;

	return pWindow->IsEnable();
}

void SPWindowLove::SetCtrlWindowText( SPWindow* pWindow , int iValue )
{
	sprintf( m_szBuf , "%d" , iValue );
	SetCtrlWindowText( pWindow , m_szBuf );
}

void SPWindowLove::SetCtrlWindowText( SPWindow* pWindow , const char* pstrText )
{
	if( pWindow == NULL )
		return;

	pWindow->SetWindowText( pstrText );
}

void SPWindowLove::SetCtrlWindowText( SPWindow* pWindow , int iGlobalStringID , const char* pstrAddSting )
{
	if( g_pResourceManager == NULL )
		return;

	ZeroMemory( m_szBuf , sizeof( m_szBuf ) );

	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( iGlobalStringID );

	if( pszGlobalString != NULL )
	{
		if( pstrAddSting == NULL )
		{
			strncpy( m_szBuf , pszGlobalString , _MAX_PATH );
		}
		else
		{
			sprintf( m_szBuf , pszGlobalString , pstrAddSting );
		}
	}

	SetCtrlWindowText( pWindow , m_szBuf );
}

void SPWindowLove::SetCtrlWindowText( SPWindow* pWindow , int iGlobalStringID , int iAddNumber )
{
	if( g_pResourceManager == NULL )
		return;

	ZeroMemory( m_szBuf , sizeof( m_szBuf ) );

	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( iGlobalStringID );

	if( pszGlobalString != NULL )
	{
		if( iAddNumber < 0 )
		{
			strncpy( m_szBuf , pszGlobalString , _MAX_PATH );
		}
		else
		{
			sprintf( m_szBuf , pszGlobalString , iAddNumber );
		}
	}

	SetCtrlWindowText( pWindow , m_szBuf );
}

void SPWindowLove::SetCtrlGageValue( SPWindow* pWindowGage , SPWindow* pWindowNumber , int iMin , int iMax )
{
	if( iMin > iMax )
		iMin = iMax;

	if( pWindowGage != NULL )
	{
		pWindowGage->SPSendMessage( SPIM_SET_CURVALUE ,	iMin );
		pWindowGage->SPSendMessage( SPIM_SET_MAXVALUE ,	iMax );
	}

	if( pWindowNumber != NULL )
	{
		float	fPercent	=	((float)iMin / (float)iMax) * 100.0f;

		if( fPercent >= 99.99f && fPercent < 100.0f && iMin != iMax )
			fPercent	=	99.99f;

		sprintf( m_szBuf , "%3.2f%%", fPercent );
		SetCtrlWindowText( pWindowNumber , m_szBuf );
	}
}

void SPWindowLove::SetCtrlNotice( LOVE_NOTICE_TYPE eType , int iDealyTime , int iGlobalStringID , const char* pstrAddSting )
{
	if( g_pResourceManager == NULL || g_pInterfaceManager == NULL )
		return;

	SPNoticeBox*	pNoticeWindow	=	(SPNoticeBox*)g_pInterfaceManager->GetNoticeBox();

	if( pNoticeWindow == NULL )
		return;

	pNoticeWindow->ForceReply();

	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( iGlobalStringID );

	if( pszGlobalString != NULL )
	{
		if( pstrAddSting == NULL )
		{
			strncpy( m_szBuf , pszGlobalString , _MAX_PATH );
			ZeroMemory( m_szActCharName , sizeof( m_szActCharName ) );
		}
		else
		{
			sprintf( m_szBuf , pszGlobalString , pstrAddSting );
			strncpy( m_szActCharName , pstrAddSting , LEN_NAME + 1 );
		}
	}
	else
	{
		ZeroMemory( m_szBuf , sizeof( m_szBuf ) );
	}

	if( iDealyTime > 0 )
	{
		pszGlobalString	=	g_pResourceManager->GetGlobalString( 5208123 );

		if( pszGlobalString != NULL )
			strcat( m_szBuf , pszGlobalString );
	}

	m_eNoticeType	=	eType;
	pNoticeWindow->Notice( m_szBuf , this , NOTICE_TYPE_YESNO , iDealyTime , DT_CENTER | DT_VCENTER , SPIM_NOTICE_NO );
}

void SPWindowLove::SetCtrlItemBox( int iItemID )
{
	SPUIItemUnit* pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( LOVE_ITEM_UNIT_POS ) );

	if( pUIItemUnit == NULL )
		return;

	pUIItemUnit->SetItemID( iItemID );
	pUIItemUnit->ReResourceLoad();

	if( iItemID <= 0 )
		pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
	else
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
}

void SPWindowLove::SetCtrlItemBoxEnable( bool bEnable )
{
	SPUIItemUnit* pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( LOVE_ITEM_UNIT_POS ) );

	if( pUIItemUnit == NULL )
		return;

	if( bEnable == false )
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
	else
		pUIItemUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP );
}

bool SPWindowLove::GetCtrlItemBoxEnable()
{
	SPUIItemUnit* pUIItemUnit	=	(SPUIItemUnit*)( m_UIUnitManager.GetUnit( LOVE_ITEM_UNIT_POS ) );

	if( pUIItemUnit == NULL )
		return false;

	if( pUIItemUnit->GetItemID() <= 0 || pUIItemUnit->GetStatus( SPUIUnit::STATUS_UI_UNIT_RENDER ) == FALSE )
		return false;

	if( pUIItemUnit->GetStatus( SPUIUnit::STATUS_UI_UNIT_DRAGNDROP ) == TRUE )
		return false;

	return true;
}

void SPWindowLove::SetError( int iGloalStringID , const char* pszAddString1 /* = NULL  */, const char* pszAddString2 /* = NULL */ )
{
	DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

	const char*	pszGlobalString	=	g_pResourceManager->GetGlobalString( iGloalStringID );

	if( pszGlobalString == NULL )
		return;

	if( pszAddString1 != NULL && pszAddString2 != NULL )
	{
		sprintf( szErrorMsg , pszGlobalString , pszAddString1 , pszAddString2 );
	}
	else if( pszAddString1 != NULL )
	{
		sprintf( szErrorMsg , pszGlobalString , pszAddString1 );
	}
	else
	{
		strcpy( szErrorMsg , pszGlobalString );
	}

	SetError( szErrorMsg );
}

void SPWindowLove::SetError( int iGloalStringID , int iValue )
{
	DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

	const char*	pszGlobalString	=	g_pResourceManager->GetGlobalString( iGloalStringID );

	if( pszGlobalString == NULL )
		return;

	sprintf( szErrorMsg , pszGlobalString , iValue );
	SetError( szErrorMsg );
}

void SPWindowLove::SetError( const char* pszErrorString )
{
	if( g_pInterfaceManager == NULL || pszErrorString == NULL )
		return;

	D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);

	g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pszErrorString , (LPARAM)&color );
}

void SPWindowLove::ProcessMemberUpdate( float fTime )
{
	if( ( m_eLoveWindowState & LOVE_WINDOW_WEDDING ) == LOVE_WINDOW_WEDDING )
	{
		m_fAccumulateMemberUpdate	+=	fTime;

		if( m_fAccumulateMemberUpdate >= m_fLimitMemberUpdate )
		{
			m_fAccumulateMemberUpdate	=	0.0f;

			CPacket	stPacketMemberUpdate;

			stPacketMemberUpdate.AddCommand( WEDDING_CS_UPDATEMEMBER );

			if( g_pNetworkManager != NULL )
				g_pNetworkManager->GetPacketHandler()->PostMsg( &stPacketMemberUpdate );
		}
	}
}

void SPWindowLove::ProcessStatusUpdate( float fTime )
{
	if( m_bStatusUpdate == false )
		return;

	m_fAccumulateStatusUpdate	+=	fTime;

	if( m_fAccumulateStatusUpdate >= m_fLimitStatusUpdate )
	{
		m_fAccumulateStatusUpdate	=	0.0f;
		UpdateWIndowAll( m_bShow );
	}
}

void SPWindowLove::ProcessButtonUpdate( float fTime )
{
	if( m_bButtonUpdate == false )
		return;

	m_fAccumulateButtonUpdate	+=	fTime;

	if( m_fAccumulateButtonUpdate >= m_fLimitButtonUpdate )
	{
		m_fAccumulateButtonUpdate	=	0.0f;
		m_bButtonUpdate				=	false;
		UpdateWIndowAll( m_bShow );
	}
}

void SPWindowLove::SendResponse( UINT8 uiWeddingType , UINT8 uiAccept )
{
	CPacket	stResponePacket;

	stResponePacket.AddCommand( WEDDING_CS_RESPONSE );
	stResponePacket.AddUINT8( uiWeddingType );
	stResponePacket.Add( (char*)m_szActCharName , LEN_NAME + 1 );
	stResponePacket.AddUINT8( uiAccept );

	if( g_pNetworkManager != NULL )
		g_pNetworkManager->GetPacketHandler()->PostMsg( &stResponePacket );
}

void SPWindowLove::ClearResponse()
{
	m_eNoticeType	=	LOVE_NOTICE_NULL;
	ZeroMemory( m_szActCharName , sizeof( m_szActCharName ) );
}

void SPWindowLove::SetWindowState( LOVE_WINDOW_STATE eLoveWindowState , bool bShowAll )
{
	m_eLoveWindowState	=	eLoveWindowState;

	std::vector<INSTANCE_ID>	vWindowInstanceIDBuffer;

	bool	bEditWindow		=	false;

	if( ( m_eLoveWindowState & LOVE_WINDOW_FRIENDSHIP ) == LOVE_WINDOW_FRIENDSHIP )
	{
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_LOVEINFO1 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BTN_PROPOSE2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAP_BG );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAP );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAP_QUESTIONMARK );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAP_ARROW );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_LT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_MT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_RT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_LM );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_MM );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_CONNECTION );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_LOCALITY );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_RM );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_LB );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_MB );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_RB );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_GAUGE );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_GAGE_PERCENTAGE );

		bEditWindow	=	true;
	}
	else if( ( m_eLoveWindowState & LOVE_WINDOW_PROPOSE_BEFORE ) == LOVE_WINDOW_PROPOSE_BEFORE )
	{
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_LOVEINFO1 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_ID );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BTN_PROPOSE1 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAP_BG );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAP );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAP_QUESTIONMARK );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAP_ARROW );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_LT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_MT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_RT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_LM );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_MM );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_RM );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_LB );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_MB );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_RB );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_TARGET_EDIT );
	}
	else if( ( m_eLoveWindowState & LOVE_WINDOW_PROPOSE_AFTER ) == LOVE_WINDOW_PROPOSE_AFTER )
	{
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_LOVEINFO1 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_ID );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BTN_PROPOSE1 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BTN_PROPOSE2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAP_BG );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAP );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAP_QUESTIONMARK );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAP_ARROW );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_LT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_MT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_RT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_LM );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_MM );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_RM );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_LB );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_MB );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_MAPSTRING_BG_RB );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_TARGET_EDIT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_GAUGE );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_GAGE_PERCENTAGE );
	}
	else if( ( m_eLoveWindowState & LOVE_WINDOW_WEDDING ) == LOVE_WINDOW_WEDDING )
	{
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_LOVEINFO2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_ID );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_GAUGE );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_GAGE_PERCENTAGE );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_ITEMSLOT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_STRING_BG_L );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_STRING_BG_M );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_STRING );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_STRING_BG_R );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_ELY_BG );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_ELY );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BTN_PROPOSE1 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BTN_PROPOSE2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_TARGET_EDIT );
	}

	if( ( m_eLoveWindowState & LOVE_WINDOW_LOVE_INFO ) == LOVE_WINDOW_LOVE_INFO )
	{
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_EXPANSION_BTN1 );
	}
	else
	{
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BG_MM1 );

		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BG_LM2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BG_RM2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BG_LB2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BG_MB2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BG_RB2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_BG_3RD_LAYER_BOARD_BOTTOM2 );

		//////////////////////////////////////////////////////////////////////////

		vWindowInstanceIDBuffer.push_back( WIID_LOVE_EXPANSION_BTN2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_LOVEINFO_BG );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_LOVEINFO_BG_LT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_LOVEINFO_BG_MT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_LOVEINFO_BG_RT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_LOVEINFO_BG_LM );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_LOVEINFO_BG_MM );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_LOVEINFO3 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_DAY_BG_L );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_DAY_BG_M );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_DAY_BG_R );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_DAY1 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_DAY2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_DAY_BG_3RD_LAYER_L );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_DAY_BG_3RD_LAYER_M );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_DAY );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_DAY_BG_3RD_LAYER_R );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_STATUS_BG_L );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_STATUS_BG_M );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_STATUS_BG_R );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CAPTION_STATUS );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_STATUS_BG_3RD_LAYER_L );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_STATUS_BG_3RD_LAYER_M );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_STATUS );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_STATUS_BG_3RD_LAYER_R );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_POINT_BG_L );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_POINT_BG_M );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_POINT_BG_R );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CATION_POINT );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_POINT_BG_3RD_LAYER_L );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_POINT_BG_3RD_LAYER_M );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_POINT1 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_POINT_BG_3RD_LAYER_R );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_CATION_SLASH );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_POINT_BG_3RD_LAYER_L2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_POINT_BG_3RD_LAYER_M2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_POINT2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_POINT_BG_3RD_LAYER_R2 );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_LOVEINFO_BG_RM );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_LOVEINFO_BG_LB );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_LOVEINFO_BG_MB );
		vWindowInstanceIDBuffer.push_back( WIID_LOVE_LOVEINFO_BG_RB );
	}

	//////////////////////////////////////////////////////////////////////////

	SPWindow*	pWindow;

	pWindow	=	Find( WIID_LOVE_TARGET_EDIT , true );

	if( pWindow != NULL )
		pWindow->SetShowEnable( bEditWindow );

	if( bShowAll == true )
		SPWindow::Show();

	//////////////////////////////////////////////////////////////////////////

	std::vector<INSTANCE_ID>::iterator		IterStart	=	vWindowInstanceIDBuffer.begin();
	std::vector<INSTANCE_ID>::iterator		IterEnd		=	vWindowInstanceIDBuffer.end();

	while ( IterStart != IterEnd )
	{
		pWindow	=	Find( (*IterStart) , true );

		if( pWindow != NULL )
			pWindow->Hide();

		++IterStart;
	}
}

void SPWindowLove::SetPartnerInfo( int iStageID , int iMapGroupID , int iChannel )
{
	SPWindow*	pWindowMapImg			=	Find( WIID_LOVE_MAP			, true );
	SPWindow*	pWindowPartnerState		=	Find( WIID_LOVE_MAPSTRING	, true );
	SPWindow*	pWindowPartnerPos		=	Find( WIID_LOVE_MAPSTRING2	, true );
	SPWindow*	pWindowQuestionMark		=	Find( WIID_LOVE_MAP_QUESTIONMARK , true );

	if( pWindowQuestionMark != NULL )
		pWindowQuestionMark->Show();

	//////////////////////////////////////////////////////////////////////////

	SetCtrlWindowText( pWindowPartnerPos , 5208149 , NULL );

	iChannel	-=	800;

	if( iChannel < 0 )
		iChannel = 0;

	if( iChannel <= 0 )
	{
		SetCtrlWindowText( pWindowPartnerState , "OFF" );

		if( pWindowMapImg != NULL )
			pWindowMapImg->SPSendMessage( SPIM_SET_IMAGECLEAR );
	}
	else if( iChannel > 0 && iChannel < 50 )
	{
		SetCtrlWindowText( pWindowPartnerState , 5208146 , iChannel );
	}
	else
	{
		SetCtrlWindowText( pWindowPartnerState , 5208150 , iChannel );
	}

	if( iStageID < 0 || iMapGroupID < 0 )
		return;

	//////////////////////////////////////////////////////////////////////////

	SPStage*	pCurStage	=	g_StageManager.GetCurStage( iStageID );

	if( pCurStage == NULL )
		return;

	SPMapGroup* pMapGroup	=	pCurStage->GetMapGroup( iMapGroupID );

	if( pMapGroup == NULL )
		return;

	//////////////////////////////////////////////////////////////////////////

	if( pWindowQuestionMark != NULL )
		pWindowQuestionMark->Hide();

	SetCtrlWindowText( pWindowPartnerPos , pMapGroup->m_strGroupName.c_str() );
	SetMinimapIcon( pWindowMapImg , pMapGroup->m_iMiniMapResID , pMapGroup->m_iMiniMapIconID );
}

void SPWindowLove::SetMinimapIcon( SPWindow* pWindow , int iResID , int iResIndex )
{
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SET_IMAGECLEAR );

	if( iResID <= 0 || iResIndex <= 0 || pWindow == NULL )
		return;

	const char*	szFileName = g_pResourceManager->GetGlobalFileName( iResID );

	if( szFileName == NULL )
		return;

	std::string strResourceFile = szFileName;

	if( strResourceFile.empty() )
		return;

	SPTexture*	pThemeIcon	=	NULL;
	std::string strFullRes; //	=	RES_WORK_RESOURCE_PATH;
	strFullRes	=	strResourceFile;

	g_pVideo->GetTextureMgr()->LoadTexture( strFullRes.c_str() , &pThemeIcon );

	if( pThemeIcon == NULL )
		return;

	//	시작은 0이 아닌 1부터 시작하기에 1을 빼준다!!!
	iResIndex--;

	int iIconSizeX = 32;
	int	iIconSizeY = 32;

	int iIconWidth	= pThemeIcon->GetLenX() / iIconSizeX;
	int iIconYIndex = iResIndex / iIconWidth;
	int iIconXIndex = iResIndex - iIconYIndex * iIconWidth;

	if( pWindow )
		pWindow->SetImage( strFullRes.c_str() , iIconXIndex * iIconSizeX , iIconYIndex * iIconSizeY );
}

void SPWindowLove::FindConsumeItem( int iItemID )
{
	if( g_pGOBManager == NULL )
		return;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPPlayerInvenArchive*	pLocalPlayerInvenArchive	=	pLocalPlayer->GetInvenArchive();

	if( pLocalPlayerInvenArchive == NULL )
		return;

	SetInvenItemAllUnlock();

	m_pConsumeItem	=	pLocalPlayerInvenArchive->GetFindItem( iItemID , 1 , 0 , LOVE_ITEM_UNIT_POS );

	SetCtrlItemBox( iItemID );
	SetCtrlItemBoxEnable( IsConsumeItem() );
}

bool SPWindowLove::IsConsumeItem()
{
	if( m_pConsumeItem == NULL )
		return false;

	return true;
}

void SPWindowLove::SetInvenItemLock( SPItem* pkItem , int iIndex )
{
	if( pkItem == NULL )
		return;

	if( g_pGOBManager == NULL )
		return;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPPlayerInvenArchive* pkInven = pLocalPlayer->GetInvenArchive();

	if( pkInven )
		pkInven->SetBlockItem(iIndex, pkItem->GetContainerType(), pkItem->GetSlotIndex());
}

void SPWindowLove::SetInvenItemUnlock( int iIndex )
{
	if( g_pGOBManager == NULL )
		return;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPPlayerInvenArchive* pkInven = pLocalPlayer->GetInvenArchive();

	if( pkInven )
		pkInven->ClearBlockItem(iIndex);
}

void SPWindowLove::SetInvenItemAllUnlock()
{
	if( g_pGOBManager == NULL )
		return;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPPlayerInvenArchive* pkInven = pLocalPlayer->GetInvenArchive();

	if( pkInven )
		pkInven->ClearAllBlockItem();
}

void SPWindowLove::SetPlayerStatus()
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager == NULL || g_pGOBManager == NULL )
		return;

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPPlayerStatus*	pLocalPlayerStatus	=	(SPPlayerStatus*)( pLocalPlayer->GetStatus() );

	if( pLocalPlayerStatus == NULL )
		return;

	WEDDING_INFO*	pWeddingInfo	=	pLoveManager->GetLoveInfo();

	if( pWeddingInfo == NULL )
		return;

	pLocalPlayerStatus->SetLovePoint( pWeddingInfo->iLP , false );

	pLocalPlayerStatus->SetStatusValue( STATUS_LOVE_LV			, pWeddingInfo->iLv		);
	//pLocalPlayerStatus->SetStatusValue( STATUS_LOVE_POINT		, pWeddingInfo->iLP		);
	pLocalPlayerStatus->SetStatusValue( STATUS_LOVE_POINT_MAX	, pWeddingInfo->iLPM	);
	pLocalPlayerStatus->SetStatusValue( STATUS_LOVE_DAY			, pWeddingInfo->iDay	);
	pLocalPlayerStatus->SetStatusValue( STATUS_LOVE_STATE		, pWeddingInfo->iStatus	);
}

void SPWindowLove::UpdateWIndowAll( bool bWindowShow )
{
	if( bWindowShow == false )
		return;

	int				iLoveDay		=	0;
	int				iLoveLevel		=	0;
	int				iLovePoint		=	0;
	int				iLovePointMax	=	0;
	int				iLoveState		=	0;
	int				iLoveProposeMax	=	0;
	std::string		strLifePartner	=	"";
	std::string		strLoveState	=	"";

	if( g_pGOBManager != NULL )
	{
		SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer != NULL )
		{
			SPPlayerStatus*	pLocalPlayerStatus	=	(SPPlayerStatus*)( pLocalPlayer->GetStatus() );

			if( pLocalPlayerStatus != NULL )
			{
				iLoveState		=	pLocalPlayerStatus->GetStatusValue( STATUS_LOVE_STATE );
				iLoveDay		=	pLocalPlayerStatus->GetStatusValue( STATUS_LOVE_DAY );
				iLoveLevel		=	pLocalPlayerStatus->GetStatusValue( STATUS_LOVE_LV );
				iLovePoint		=	pLocalPlayerStatus->GetStatusValue( STATUS_LOVE_POINT );
				iLovePointMax	=	pLocalPlayerStatus->GetStatusValue( STATUS_LOVE_POINT_MAX );

				//////////////////////////////////////////////////////////////////////////

				SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

				if( pLoveManager != NULL )
				{
					LOVE_LEVEL_DATA*	pLoveLevelData	=	pLoveManager->GetLevelData( iLoveLevel );

					if( pLoveLevelData != NULL )
						strLoveState	=	pLoveLevelData->m_strTitle;
				}

				//////////////////////////////////////////////////////////////////////////
			}
		}
	}

	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager != NULL )
	{
		if( iLoveState > WEDDING_STATUS_NONE )
		{
			const char*	pstrLifePartner	=	pLoveManager->GetLifePartnerString();

			if( pstrLifePartner != NULL )
				strLifePartner	=	pstrLifePartner;
		}

		iLoveProposeMax	=	pLoveManager->GetRequire( LOVE_LDT_REQUIRE_PROPOSE_POSSIBLE_LOVEPOINT );
	}

	SetCtrlWindowText( Find( WIID_LOVE_NAME		, true ) , strLifePartner.c_str() );
	SetCtrlWindowText( Find( WIID_LOVE_DAY		, true ) , iLoveDay );
	SetCtrlWindowText( Find( WIID_LOVE_STATUS	, true ) , strLoveState.c_str() );
	SetCtrlWindowText( Find( WIID_LOVE_POINT1	, true ) , ConvertMoneyToString( iLovePoint ) );
	SetCtrlWindowText( Find( WIID_LOVE_POINT2	, true ) , ConvertMoneyToString( iLovePointMax ) );
	SetCtrlGageValue( Find( WIID_LOVE_GAUGE , true ) , Find( WIID_LOVE_GAGE_PERCENTAGE , true ) , iLovePointMax , iLoveProposeMax );

	UpdateRequireCondition( bWindowShow );

	m_bStatusUpdate		=	false;
}

void SPWindowLove::UpdateRequireCondition( bool bWindowShow )
{
	SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

	if( pLoveManager == NULL || g_pCheckManager == NULL )
		return;

	int				iLoveState		=	0;
	int				iLoveLevel		=	0;
	int				iLovePoint		=	0;
	int				iLovePointMax	=	0;

	if( g_pGOBManager != NULL )
	{
		SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer != NULL )
		{
			SPPlayerStatus*	pLocalPlayerStatus	=	(SPPlayerStatus*)( pLocalPlayer->GetStatus() );

			if( pLocalPlayerStatus != NULL )
			{
				iLoveState		=	pLocalPlayerStatus->GetStatusValue( STATUS_LOVE_STATE );
				iLoveLevel		=	pLocalPlayerStatus->GetStatusValue( STATUS_LOVE_LV );
				iLovePoint		=	pLocalPlayerStatus->GetStatusValue( STATUS_LOVE_POINT );
				iLovePointMax	=	pLocalPlayerStatus->GetStatusValue( STATUS_LOVE_POINT_MAX );
			}
		}
	}

	int					iConsumeEly			=	0;
	int					iConsumeItem		=	0;
	int					iLoveMsg			=	0;
	int					iLoveMsgNumber		=	-1;

	INSTANCE_ID			eRunButton			=	0;
	bool				bRunEnable			=	false;
	bool				bElyShow			=	true;

	DWORD				dwLoveWindowState	=	LOVE_WINDOW_FRIENDSHIP;

	//	결혼 중 상태
	if( ( iLoveState & WEDDING_STATUS_WEDDING ) == WEDDING_STATUS_WEDDING )
	{
		dwLoveWindowState	=	LOVE_WINDOW_WEDDING;
	}
	//	프로포즈 한 후 상태
	else if( ( iLoveState & WEDDING_STATUS_PROPOSE ) == WEDDING_STATUS_PROPOSE )
	{
		dwLoveWindowState	=	LOVE_WINDOW_PROPOSE_AFTER;
	}
	//	교제 중 상태
	else if( ( iLoveState & WEDDING_STATUS_LOVE ) == WEDDING_STATUS_LOVE )
	{
		iConsumeEly			=	pLoveManager->GetRequire( LOVE_LDT_REQUIRE_PROPOSE_COMSUME_ELY	);
		iConsumeItem		=	pLoveManager->GetRequire( LOVE_LDT_REQUIRE_PROPOSE_COMSUME_ITEM	);

		eRunButton			=	WIID_LOVE_BTN_PROPOSE2;
		dwLoveWindowState	=	LOVE_WINDOW_PROPOSE_BEFORE;
	}
	//	이도저도 아닌 상태
	else
	{
		iConsumeEly			=	pLoveManager->GetRequire( LOVE_LDT_REQUIRE_FRIENDSHIP_CONSUME_ELY	);
		iConsumeItem		=	pLoveManager->GetRequire( LOVE_LDT_REQUIRE_FRIENDSHIP_CONSUME_ITEM	);

		eRunButton			=	WIID_LOVE_BTN_PROPOSE1;
		dwLoveWindowState	=	LOVE_WINDOW_FRIENDSHIP;
	}

	//////////////////////////////////////////////////////////////////////////

	if( ( m_eLoveWindowState & LOVE_WINDOW_LOVE_INFO ) == LOVE_WINDOW_LOVE_INFO )
		dwLoveWindowState	|=	LOVE_WINDOW_LOVE_INFO;

	SetWindowState( (LOVE_WINDOW_STATE)dwLoveWindowState , bWindowShow );

	//////////////////////////////////////////////////////////////////////////

	if( ( iLoveState & WEDDING_STATUS_WEDDING ) == WEDDING_STATUS_WEDDING )
	{
		SetPartnerInfo( m_iStageID , m_iMapGroupID , m_iChannelID );
		return;
	}

	//////////////////////////////////////////////////////////////////////////

	FindConsumeItem( iConsumeItem );

	bRunEnable			=	GetCtrlItemBoxEnable();

	//////////////////////////////////////////////////////////////////////////

	//	프로포즈 한 후 상태
	if( ( iLoveState & WEDDING_STATUS_PROPOSE ) == WEDDING_STATUS_PROPOSE )
	{
		bRunEnable	=	false;
		iLoveMsg	=	5208143;
		iConsumeEly	=	0;
		bElyShow	=	false;
	}
	//	교제 중 상태
	else if( ( iLoveState & WEDDING_STATUS_LOVE ) == WEDDING_STATUS_LOVE )
	{
		if( iConsumeItem <= 0 )
			bRunEnable	=	true;

		if( bRunEnable == false )
		{
			if( iConsumeItem <= 0 )
				iLoveMsg	=	5208114;
			else
                iLoveMsg	=	5208115;
		}
		else
		{
			iLoveMsg	=	5208114;
		}

		if( iConsumeItem <= 0 && iConsumeEly <= 0 )
		{
			iLoveMsg	=	5208116;
		}

		int	iLoveProposeMax		=	pLoveManager->GetRequire( LOVE_LDT_REQUIRE_PROPOSE_POSSIBLE_LOVEPOINT );

		if( iLovePointMax < iLoveProposeMax )
		{
			bRunEnable	=	false;
			iLoveMsg	=	5208116;
		}
	}
	//	이도저도 아닌 상태
	else
	{
		if( iConsumeItem <= 0 )
			bRunEnable	=	true;

		if( bRunEnable == false )
			iLoveMsg	=	5208141;
		else
			iLoveMsg	=	5208142;

		//	페널티 타임 ( 분을 초로 변경 )
		int		iPartingTime	=	pLoveManager->GetRequire( LOVE_LDT_REQUIRE_PARTING_PENALTY_TIME ) * 60;

		if( iPartingTime > 0 )
		{
			//	초 타임
			int		iCurPartingTimeSec	=	pLoveManager->GetCurPartingTime();

			if( iCurPartingTimeSec < iPartingTime )
			{
				iLoveMsg	=	5208132;
				bRunEnable	=	false;

				int	iRemainTimeHour	=	( iPartingTime - iCurPartingTimeSec ) / 3600;

				if( iRemainTimeHour <= 0 )
					iRemainTimeHour = 1;

				iLoveMsgNumber	=	iRemainTimeHour;
			}
		}
	}

	//if( bRunEnable ==  true )
	//	bRunEnable	=	g_pCheckManager->CheckPCStatus( CHECK_STATUS_MONEY , iConsumeEly , 0 );

	//////////////////////////////////////////////////////////////////////////

	if( iConsumeEly <= 0 )
		bElyShow	=	false;

	//////////////////////////////////////////////////////////////////////////

	if( m_bButtonUpdate == true )
		bRunEnable	=	false;

	//////////////////////////////////////////////////////////////////////////

	SetCtrlWindowEnable( Find( eRunButton , true ) , bRunEnable );

	//////////////////////////////////////////////////////////////////////////

	SetCtrlWindowShow( Find( WIID_LOVE_ELY_BG , true ) , bElyShow );

	//////////////////////////////////////////////////////////////////////////

	SetCtrlWindowText( Find( WIID_LOVE_ELY		, true ) , ConvertMoneyToString( iConsumeEly ) );
	SetCtrlWindowText( Find( WIID_LOVE_STRING	, true ) , iLoveMsg		, iLoveMsgNumber );
}
