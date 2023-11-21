// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 2006-06-19 리뉴얼 AJJIYA
// Creation    : AJJIYA 2005-7-4    13:51 
//***************************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPLDTFile.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowSlider.h"
#include "SPWindowEdit.h"
#include "SPNoticeBox.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "SPGOBStatus.h"
#include "SPPlayerInvenArchive.h"

#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee

#include "SPQuestManager.h"
#include "SPQuestArchive.h"
#include "SPQuestAttr.h"

#include "SPTitleAttr.h"
#include "SPTitleArchive.h"
#include "SPTitleManager.h"

#include "SPUIUnit.h"
#include "SPUISkillUnit.h"
#include "SPUIUnitManager.h"
#include "SPDragnDropManager.h"

#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPSkillManager.h"
#include "SPMotionStatus.h"

#include "SPCheckManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPWindowToolTip.h"

#include "LT_Error.h"
#include "Packet.h"

//#define _SKILL_DEBUG_PRINT

#if defined( _DEBUG ) || defined( _SKILL_DEBUG_PRINT )
	#include "SPDebug.h"
#endif

#include "SPWindowSkill.h"

SPWindowSkill::SPWindowSkill(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowSkill::SPWindowSkill(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_SKILL, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowSkill::~SPWindowSkill()
{
	Clean();
}

void SPWindowSkill::Init()
{
	m_strToolTip = "[SPWindowSkill]";

	LoadLDTMainMenu();
	LoadLDTSubMenu();
	LoadLDTContent();
	LoadLDTLearnSkill();

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , &m_pBGTexture );

	ReposOwnImage();

	SetRect( &m_rcBGSrc[0]	, 1 ,  1 , 261 ,  5 );			//	배경 상단
	SetRect( &m_rcBGSrc[1]	, 1 ,  7 , 261 ,  9 );			//	배경 중단
	SetRect( &m_rcBGSrc[2]	, 1 , 11 , 261 , 15 );			//	배경 하단
	SetRect( &m_rcBGSrc[3]	, 349 , 312 , 355 , 318 );		//	옅은 초록 서브 백그라운드
	SetRect( &m_rcBGSrc[4]	, 349 , 312 , 355 , 318 );		//	옅은 초록 서브 백그라운드 2
	SetRect( &m_rcBGSrc[5]	, 349 , 321 , 355 , 327 );		//	중앙 흰색 서브 백그라운드
	SetRect( &m_rcBGSrc[6]	, 349 , 329 , 355 , 330 );		//	경계선 1
	SetRect( &m_rcBGSrc[7]	, 349 , 329 , 355 , 330 );		//	경계선 2
	SetRect( &m_rcBGSrc[8]	, 349 , 329 , 355 , 330 );		//	경계선 3

	SPWindowStatic*	pWindowStatic;

	pWindowStatic	=	new SPWindowStatic( WIID_SKILL_ICON_IMAGE , 4 , 2 , 35 , 24 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 32 , 313 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_SKILL_TEXT_IMAGE , 42 , 7 , 62 , 15 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 49 , 338 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_SKILL_POINT_LABEL , 138 , 326 , 62 , 11 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 328 , 359 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_SKILL_POINT_TEXT , 208 , 324 , 42 , 14 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 200 , 46 );
	pWindowStatic->SetWindowText( "0" );
	pWindowStatic->SetFormat( DT_CENTER | DT_TOP );
	pWindowStatic->SetFontColor( D3DXCOLOR( 49.0f / 255.0f , 76.0f / 255.0f , 97.0f / 255.0f , 1.0f ) );
	pWindowStatic	=	NULL;

	SPWindowButton*	pWindowButton;

	pWindowButton	=	new SPWindowButton( WIID_SKILL_CLOSE , 239 , 9 , 12 , 12 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 217 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 230 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 243 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 256 );
	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_SKILL_SCROLL_UP , 236 , 69 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );
	pWindowButton	=	NULL;

	pWindowButton	=	new SPWindowButton( WIID_SKILL_SCROLL_DOWN , 236 , 274 , 12 , 11 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );
	pWindowButton	=	NULL;

}

void SPWindowSkill::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );

	m_UIUnitManager.ClearAll( TRUE );
}

void SPWindowSkill::Show(bool bWithChild)
{
	SPWindow::Show( bWithChild );

	UpdateAll();

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();
	int	iUnitPos	=	m_UIUnitManager.GetNumberMouseOver( iCursorX , iCursorY );
	int	iStepID		=	( SCL_COUNT + SCGV_COUNT );

	if( iUnitPos >= 0 )
	{
		OnContentCursorEnter( iUnitPos * iStepID + SCL_BG + WIID_SKILL_CONTENT_START , NULL , NULL);
	}
}

void SPWindowSkill::Hide(bool bSendServer)
{
	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
		OnContentCursorOut( WIID_SKILL_CONTENT_START + SCL_BG , NULL , NULL );

	//	AJJIYA [9/8/2005]
	//	창 정렬을 위해서!!
	g_pInterfaceManager->OrderWindowPosMove();
}

void SPWindowSkill::Process(float fTime)
{
	SPWindow::Process( fTime );

	m_UIUnitManager.Process( fTime );
}

void SPWindowSkill::Render(float fTime)
{
	if(!m_bShow) return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		for( int i = 0 ; i < SKILL_BG_COUNT ; i++ )
		{
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	SPWindow::Render(fTime);

	g_pVideo->Flush();

	m_UIUnitManager.Render();

	g_pVideo->Flush();
}


void SPWindowSkill::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

//////////////////////////////////////////////////////////////////////////

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowSkill )

	SPIMESSAGE_COMMAND(	SPIM_MOVE_BEGIN			,										OnBeginMove			)	// metalgeni [6/2/2006]
	SPIMESSAGE_COMMAND(	SPIM_MOVE_END			,										OnEndMove			)	// Related to window move
	SPIMESSAGE_COMMAND(	SPIM_MOVE				,										OnMove				)

	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP			,										OnDragnDropEnd		)
	SPIMESSAGE_COMMAND(	SPIM_DRAGNDROP_END		,										OnDragnDropEnd		)

	SPIMESSAGE_COMMAND(	SPIM_WHEEL_UP			,										OnWheelUp			)
	SPIMESSAGE_COMMAND(	SPIM_WHEEL_DOWN			,										OnWheelDown			)
	
	SPIMESSAGE_COMMAND(	SPIM_PURGE				,										OnPurge				)
	SPIMESSAGE_COMMAND( SPIM_POS_UPDATE			,										OnWndPosUpdate		)

	SPIMESSAGE_COMMAND( SPIM_REFRESH			,										OnStatusRefresh		)	
	SPIMESSAGE_COMMAND(	SPIM_SKILL_UPDATE		,										OnSkillUpdate		)

	// Packet 관련 AJJIYA  [6/21/2006]
	SPIMESSAGE_COMMAND(	SPIM_GET_SKILL_LVUP		,										OnGetSkillLVUP		)
	SPIMESSAGE_COMMAND(	SPIM_GET_SKILL_LEARN	,										OnGetSkillLearn		)

	// End

	SPIMESSAGE_COMMAND(	SPIM_GET_SKILL_UIUNIT	,										OnGetUIUnitManager	)

	SPIMESSAGE_COMMAND( SPIM_NOTICE_YES			,										OnLearnSkillYes		)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_SKILL_CLOSE				,	OnClose				)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_SKILL_SCROLL_UP			,	OnSliderUp			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP			,	WIID_SKILL_SCROLL_DOWN			,	OnSliderDown		)

	SPIMESSAGE_CONTROL( SPIM_SLIDER_UP			,	WIID_SKILL_SCROLL_BAR			,	OnSliderBar			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN		,	WIID_SKILL_SCROLL_BAR			,	OnSliderBar			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP		,	WIID_SKILL_SCROLL_BAR			,	OnSliderBar			)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN	,	WIID_SKILL_SCROLL_BAR			,	OnSliderBar			)
	SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE		,	WIID_SKILL_SCROLL_BAR			,	OnSliderBar			)


SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

//////////////////////////////////////////////////////////////////////////

int SPWindowSkill::OnBeginMove		( WPARAM wParam, LPARAM lParam)
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

int SPWindowSkill::OnEndMove		( WPARAM wParam, LPARAM lParam)
{
	return SPWindow::OnEndMove(wParam, lParam);
}

int SPWindowSkill::OnMove			( WPARAM wParam, LPARAM lParam)
{
	return SPWindow::OnMove(wParam, lParam);
}

int SPWindowSkill::OnDragnDropEnd( WPARAM wParam, LPARAM lParam )
{
	m_bLMouseDown	=	FALSE;
	m_lParam		=	0;

	// AJJIYA [9/8/2005]
	// 사운드 추가
	g_pInterfaceManager->RegisterSoundUnit( "DATA/SOUND/SYSTEM_ICON_PUT_DOWN.WAV" );

	return 1;
}

int SPWindowSkill::OnWheelUp( WPARAM wParam, LPARAM lParam )
{
	int	iCursorX = g_pInterfaceManager->GetCursorX();
	int	iCursorY = g_pInterfaceManager->GetCursorY();

	if( iCursorX >= m_rcBGDest[5].left	&& iCursorY >= m_rcBGDest[5].top		&& 
		iCursorX <= m_rcBGDest[5].right	&& iCursorY <= m_rcBGDest[5].bottom	)
	{
		OnSliderUp( NULL , NULL );
	}

	return 1;
}

int SPWindowSkill::OnWheelDown( WPARAM wParam, LPARAM lParam )
{
	int	iCursorX = g_pInterfaceManager->GetCursorX();
	int	iCursorY = g_pInterfaceManager->GetCursorY();

	if( iCursorX >= m_rcBGDest[5].left	&& iCursorY >= m_rcBGDest[5].top		&& 
		iCursorX <= m_rcBGDest[5].right	&& iCursorY <= m_rcBGDest[5].bottom	)
	{
		OnSliderDown( NULL , NULL );
	}

	return 1;
}

int SPWindowSkill::OnPurge( WPARAM wParam, LPARAM lParam )
{
	DataReset();

	UpdateAll();

	return 1;
}

int SPWindowSkill::OnWndPosUpdate( WPARAM wParam, LPARAM lParam )
{
	RefreshRelationCoord();

	return 1;
}

int SPWindowSkill::OnStatusRefresh( WPARAM wParam, LPARAM lParam )
{
	OnSkillUpdate( 0 , 0 );

	return 1;
}

int SPWindowSkill::OnSkillUpdate( WPARAM wParam, LPARAM lParam )
{
	UpdateAll();

	g_pInterfaceManager->SPChildWndSendMessage( WIID_ITEM_MIX , SPIM_SKILL_UPDATE , NULL , NULL );

	return 1;
}

int SPWindowSkill::OnGetSkillLVUP( WPARAM wParam, LPARAM lParam )
{
	RESULTCODE*	pResult	=	(RESULTCODE*)wParam;
	CPacket*	pPacket =	(CPacket*)lParam;

	if( *pResult != LT_NO_ERROR || g_pGOBManager->GetLocalPlayer() == NULL )
		return 1;

	GU_ID		globalID		=	0;
	short		iSkillFreePoint;
	CHAR_SKILL	SkillData;

	pPacket->ExtractUINT64( &globalID );
	pPacket->Extract( &iSkillFreePoint , sizeof( iSkillFreePoint ) );
	pPacket->ExtractStruct( &SkillData , sizeof( SkillData ) );

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	SPSkillArchive*		pSkillArchive	=	pLocalPlayer->GetSkillArchive();

	if( pSkillArchive == NULL )
		return 1;

	SPSkillActivity*	pSkillActivity	=	pSkillArchive->GetSkillActivity( SkillData.iSkillID );

	if( pSkillActivity == NULL )
		return 1;

	pSkillActivity->SetLevel( SkillData.iSkillLv );
	pSkillActivity->SetCurLevel( SkillData.iSkillCurrLv );

	SPGOBStatus*	pPlayerStatus	=	pLocalPlayer->GetStatus();

	if( pPlayerStatus == NULL )
		return 1;
    
	pPlayerStatus->SetStatusValue( STATUS_SKILL_POINT , iSkillFreePoint );

	return 1;
}

int SPWindowSkill::OnGetSkillLearn( WPARAM wParam, LPARAM lParam )
{
	CPacket*	pPacket =	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	GLOBAL_STRING_ID	uiGlobalStringID	=	0;
	pPacket->ExtractUINT32( &uiGlobalStringID );

	if( uiGlobalStringID > 0 )
	{
		const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( uiGlobalStringID );

		if( pszGlobalString != NULL )
		{
			D3DXCOLOR	color	=	D3DCOLOR_ARGB( 255 , 255 , 0 , 0 );
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pszGlobalString , (LPARAM)&color );
		}

		return 1;
	}

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	GU_ID				ObjectGUID			=	0;
	pPacket->ExtractUINT64( &ObjectGUID );

	if( pLocalPlayer->GetGUID() != ObjectGUID )
		return 1;

	CHAR_SKILL	SkillData;
	pPacket->ExtractStruct( &SkillData , sizeof( SkillData ) );

	SPSkillArchive*		pSkillArchive	=	pLocalPlayer->GetSkillArchive();

	if( pSkillArchive != NULL )
	{
		SPSkill*	pkSkill	=	SPSkillManager::GetInstance()->GetSkill( SkillData.iSkillID );

		if( pkSkill != NULL )
			pSkillArchive->AddSkill( SkillData.iSkillNo , pkSkill , SkillData.iSkillLv , SkillData.iSkillCurrLv );
	}

	UINT64	iPCCurrEly;
	pPacket->ExtractUINT64( &iPCCurrEly );

	SPGOBStatus*			pPlayerStatus	=	pLocalPlayer->GetStatus();

	if( pPlayerStatus != NULL )
		pPlayerStatus->SetStatusValueEX( STATUS_EX_ELY , iPCCurrEly );

	TRASH_ITEM		tTrashITem;
	pPacket->ExtractStruct( &tTrashITem , sizeof( TRASH_ITEM ) );

	--tTrashITem.iContainerSlot.SlotIndex;

	SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();

	if( pInvenArchive != NULL )
		pInvenArchive->TrashItem( tTrashITem );

	SKILL_LEARN_LDT_DATA*	pSkillLearnLDT	=	GetLearnSKillLdt( SkillData.iSkillID );

	if( pSkillLearnLDT != NULL )
	{
		pLocalPlayer->SPGOBPerformMessage( SPGM_SET_FX_GROUP , (LPARAM)pSkillLearnLDT->m_iFXGroupID );

		D3DXCOLOR	color	=	D3DCOLOR_ARGB( 255 , 134 , 182 , 253 );
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pSkillLearnLDT->m_strSuccess.c_str() , (LPARAM)&color );
	}

	OnSkillUpdate( 0 , 0 );

	return 1;
}

int SPWindowSkill::OnLearnSkillYes( WPARAM wParam, LPARAM lParam )
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	SPSkillArchive*		pSkillArchive	=	pLocalPlayer->GetSkillArchive();

	if( pSkillArchive == NULL )
		return 1;

	SPSkillActivity*	pSkillActivity	=	pSkillArchive->GetSkillActivity( m_iLearnSkillID );

	if( pSkillActivity != NULL )
		return 1;

	SPSkillManager*		pSkillManager		=	SPSkillManager::GetInstance();

	if( pSkillManager == NULL )
		return 1;

	SPSkill*	pSkill	=	pSkillManager->GetSkill( m_iLearnSkillID );

	if( pSkill == NULL )
		return 1;

	if( pSkill->GetLearnSkill() == false )
		return 1;

	SKILL_LEARN_LDT_DATA*	pSkillLearnLDT	=	GetLearnSKillLdt( m_iLearnSkillID );

	if( pSkillLearnLDT == NULL )
		return 1;

	BOOL		bPacketSend	=	TRUE;
	D3DXCOLOR	cColor		=	D3DXCOLOR((255.0f / 255.0f), (0.0f / 255.0f), (0.0f / 255.0f), 1.0f);

	// 엘리 체크
	if( g_pCheckManager->CheckPCStatus( CHECK_STATUS_MONEY , pSkillLearnLDT->m_uiConsumptionEly , 0 ) == false )
	{
		bPacketSend	=	FALSE;

		if( pSkillLearnLDT->m_strErrorEly.empty() == false )
		{
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pSkillLearnLDT->m_strErrorEly.c_str() , (LPARAM)&cColor );
		}
	}

	// 아이템
//	if( g_pCheckManager->CheckPCItem( pSkillLearnLDT->m_iConsumptionItemID , 0 , pSkillLearnLDT->m_iConsumptionItemCount ) == false )
	if( pSkillLearnLDT->m_iConsumptionItemID > 0 && g_pCheckManager->CheckPCItem( pSkillLearnLDT->m_iConsumptionItemID , 0 , 1 ) == false )
	{
		bPacketSend	=	FALSE;

		if( pSkillLearnLDT->m_strErrorItem.empty() == false )
		{
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pSkillLearnLDT->m_strErrorItem.c_str() , (LPARAM)&cColor );
		}
	}


	//	필요 조건
	SKILL_LEARN_REQUIRE*	pSkillLearnRequire	=	NULL;

	for( int i = 0 ; i < LS_REQUIRE_COUNT ; ++i )
	{
		pSkillLearnRequire = &pSkillLearnLDT->m_stRequireCondition[ i ];

		if( pSkillLearnRequire->m_iRequireType == 0 || pSkillLearnRequire->m_iRequireType > 5 )
			continue;

		if( g_pCheckManager->CheckCondition( pSkillLearnRequire->m_iRequireType , pSkillLearnRequire->m_iRequireID , pSkillLearnRequire->m_iRequireValue1 , pSkillLearnRequire->m_iRequireValue2 ) == false )
		{
			bPacketSend	=	FALSE;

			if( pSkillLearnRequire->m_strErrorMsg.empty() == false )
			{
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pSkillLearnRequire->m_strErrorMsg.c_str() , (LPARAM)&cColor );
			}
		}
	}

	if( bPacketSend == FALSE )
		return 1;

	//	패킷 보내기
	CPacket kPacket( SKILL_CS_LEARN );
	kPacket.AddUINT32( m_iLearnSkillID );

	int	iConsumptionEly	=	pSkillLearnLDT->m_uiConsumptionEly;
	kPacket.AddUINT32( iConsumptionEly );

	TRASH_ITEM stRequireItem;

	stRequireItem.iContainerSlot.ContainerType	=	CONTAINER_TYPE_INVALID;
	stRequireItem.iContainerSlot.SlotIndex		=	0;
	stRequireItem.iCount						=	0;

	SPPlayerInvenArchive*	pLocalPlayerInvenArchive	=	pLocalPlayer->GetInvenArchive();

	if( pLocalPlayerInvenArchive != NULL )
	{
		SPItemAttr*	pItemAttr = NULL;

		if( g_pItemCluster->GetItemInfo( pSkillLearnLDT->m_iConsumptionItemID , pItemAttr ) == true )
		{
			CONTAINER_TYPE	eItemType = (CONTAINER_TYPE)( pItemAttr->m_eType - 1 );

			SPItem* pRequireItem = pLocalPlayerInvenArchive->GetSPItem( eItemType , pSkillLearnLDT->m_iConsumptionItemID );

			if( pRequireItem != NULL )
			{
				stRequireItem.iContainerSlot.ContainerType	=	pRequireItem->GetContainerType();
				stRequireItem.iContainerSlot.SlotIndex		=	pRequireItem->GetSlotIndex() + 1;
				stRequireItem.iCount						=	1;
			}
		}
	}

	kPacket.AddData( &stRequireItem , sizeof(TRASH_ITEM) );

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &kPacket );

	return 1;
}

int SPWindowSkill::OnClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

int SPWindowSkill::OnSliderUp( WPARAM wParam, LPARAM lParam )
{
	SPWindow*	pWindow	=	Find( WIID_SKILL_SCROLL_BAR , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SLIDER_UP );

	return 1;
}

int SPWindowSkill::OnSliderDown( WPARAM wParam, LPARAM lParam )
{
	SPWindow*	pWindow	=	Find( WIID_SKILL_SCROLL_BAR , true );

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_SLIDER_DOWN );

	return 1;
}

int SPWindowSkill::OnSliderBar( WPARAM wParam, LPARAM lParam )
{
	int	iNewPage	=	(int)wParam;

	UpdateContent( m_iCurSubMenu , iNewPage );

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();
	int	iUnitPos	=	m_UIUnitManager.GetNumberMouseOver( iCursorX , iCursorY );
	int	iStepID		=	( SCL_COUNT + SCGV_COUNT );

	if( iUnitPos != -1 )
	{
		OnContentCursorEnter( iUnitPos * iStepID + SCL_BG + WIID_SKILL_CONTENT_START , NULL , NULL);
	}
	else
	{
		OnContentCursorOut( WIID_SKILL_CONTENT_START + SCL_BG , NULL , NULL );
	}

	return 1;
}

int SPWindowSkill::OnGetUIUnitManager( WPARAM wParam, LPARAM lParam )
{
	*((SPUIUnitManager **)wParam) = &m_UIUnitManager;

	return 1;
}

int SPWindowSkill::OnMainMenuLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow*	pWindow	=	Find( iID , true );

	if( pWindow != NULL )
	{
		if( pWindow->IsShow() == false )
			return 1;
	}

	int	iItemID	=	iID - WIID_SKILL_MAIN_MENU_START;

	int	iKey	=	iItemID / SKILL_CLASS_SEPARATION;
	int	iNumber	=	iItemID % SKILL_CLASS_SEPARATION - 1;

	UI_MAIN_MENU* pUIMainMenu = GetMainMenu( iKey , iNumber );

	if( pUIMainMenu == NULL )
		return 1;

	if( pUIMainMenu->m_iItemID != iItemID )
		return 1;

	UpdateSubMenu( pUIMainMenu );

	for( int i = 0 ; i < SKILL_MAIN_MENU_MAX ; ++i )
	{
		if( m_pMainMenuButton[ i ] == NULL )
			continue;

		if( m_pMainMenuButton[ i ]->GetInstanceID() == iID )
		{
			m_pMainMenuButton[ i ]->SPSendMessage( SPIM_BTN_SELECT , TRUE );
		}
		else
		{
			m_pMainMenuButton[ i ]->SPSendMessage( SPIM_BTN_SELECT , FALSE );
		}
	}

	for( i = 0 ; i < SKILL_SUB_MENU_MAX ; ++i )
	{
		if( m_pSubMenuButton[ i ] == NULL )
			continue;

		m_pSubMenuButton[ i ]->SPSendMessage( SPIM_BTN_SELECT , FALSE );
	}

	if( m_pCurMainMenu != NULL )
	{
		int*	piCurSubMenuPage	=	GetSubMenuPage( m_pCurMainMenu->m_iItemID );
		int		iSubID;

		if( piCurSubMenuPage == NULL )
		{
			if( (int)m_pCurMainMenu->m_vUISubID.size() > 0 )
			{
				UI_SUBID&	stUISubID	=	m_pCurMainMenu->m_vUISubID.at( 0 );
				iSubID	=	stUISubID.m_iSubID;
			}
		}
		else
		{
			iSubID	=	*piCurSubMenuPage;
		}

		OnSubMenuLButtonUp( WIID_SKILL_SUB_MENU_START + iSubID , NULL , NULL );
	}

	return 1;
}

int SPWindowSkill::OnSubMenuLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SPWindow*	pWindow	=	Find( iID , true );

	if( pWindow != NULL )
	{
		if( pWindow->IsShow() == false )
			return 1;
	}

	int	iSubID	=	iID	-	WIID_SKILL_SUB_MENU_START;

	if( m_pCurMainMenu != NULL && iSubID != 0 )
	{
		int		iCurContentPageKey	=	GetContentPageKey( m_pCurMainMenu->m_iItemID , iSubID );
		int*	pCurContentPage		=	GetContentPage( iCurContentPageKey );

		if( pCurContentPage == NULL )
		{
			SetContentPage( iCurContentPageKey , 0 );
			pCurContentPage		=	GetContentPage( iCurContentPageKey );
		}

		UpdateContent( iSubID , *pCurContentPage );
	}

	for( int i = 0 ; i < SKILL_SUB_MENU_MAX ; ++i )
	{
		if( m_pSubMenuButton[ i ] == NULL )
			continue;

		if( m_pSubMenuButton[ i ]->GetInstanceID() == iID )
		{
			m_pSubMenuButton[ i ]->SPSendMessage( SPIM_BTN_SELECT , TRUE );
		}
		else
		{
			m_pSubMenuButton[ i ]->SPSendMessage( SPIM_BTN_SELECT , FALSE );
		}
	}

	return 1;
}

int SPWindowSkill::OnContentLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	m_bLMouseDown	=	FALSE;
	m_lParam		=	0;

	SPWindow*	pWindow	=	Find( iID , true );

	if( pWindow != NULL )
	{
		if( pWindow->IsShow() == false )
			return 1;
	}

	int	iLButtonUpID	=	( iID - WIID_SKILL_CONTENT_START );
	int	iStepID			=	( SCL_COUNT + SCGV_COUNT );
	int	iUIPos			=	iLButtonUpID / iStepID;
	int	iUIWindowID		=	iLButtonUpID % iStepID;

	if( iUIWindowID == SCL_LEVEL_UP )
	{
		int	iUIUnitCount	=	m_UIUnitManager.GetUnitCount();

		if( iUIPos < 0 || iUIUnitCount <= iUIPos )
			return 1;

		SPUISkillUnit*	pUISkillUnit	=	(SPUISkillUnit*)( m_UIUnitManager.GetUnit( iUIPos ) );

		if( pUISkillUnit == NULL )
			return 1;

		if( pUISkillUnit->GetStatus( SPUIUnit::STATUS_UI_UNIT_RENDER ) == FALSE )
			return 1;

		SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer == NULL )
			return 1;

		SPSkillArchive*		pSkillArchive	=	pLocalPlayer->GetSkillArchive();

		if( pSkillArchive == NULL )
			return 1;

		int					iSkillID		=	pUISkillUnit->GetSkillID();

		if( g_pCheckManager->CheckSkillUpRequire( iSkillID ) == true )
		{
			CPacket Packet;
			Packet.Add( (UINT32)SKILL_CS_LVUP );
			Packet.Add( (UINT32)iSkillID );
			g_pNetworkManager->SPPerformMessage( SKILL_CS_LVUP , 0 , (LPARAM)&Packet );
		}
	}

	if( iUIWindowID == SCL_BG )
	{
		SPUIUnit*	pUIUnit	=	m_UIUnitManager.GetUnit( iUIPos );

		if( pUIUnit != NULL )
			pUIUnit->Use();

		SPUISkillUnit*	pUISkillUnit	=	(SPUISkillUnit*)pUIUnit;

		if( pUISkillUnit != NULL )
			SetLearnSkillMsg( pUISkillUnit->GetSkillID() );
	}

	return 1;
}

int SPWindowSkill::OnContentLButtonDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	int				iXPos			=	LOWORD(lParam);
	int				iYPos			=	HIWORD(lParam);
	int				iUnitNumber		=	m_UIUnitManager.GetNumberMouseOver( iXPos ,iYPos );

	if( iUnitNumber >= 0 )
	{
		m_bLMouseDown	=	TRUE;
		m_lParam		=	lParam;
	}

	return 1;
}

int SPWindowSkill::OnContentCursorMove( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( IsShow() == FALSE )
		return 1;

	int				iXPos			=	LOWORD(lParam);
	int				iYPos			=	HIWORD(lParam);
	int				iXPosOld		=	LOWORD(m_lParam);
	int				iYPosOld		=	HIWORD(m_lParam);
	int				iUnitNumber		=	m_UIUnitManager.GetNumberMouseOver( iXPosOld , iYPosOld );
	SPUIUnit*		pUIUnit			=	m_UIUnitManager.GetUnit( iUnitNumber );
//	SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();
	BOOL			bIsDragging		=	SPDragnDropManager::GetInstance()->IsDragging();

	if( pUIUnit == NULL || bIsDragging == TRUE )
		return 1;

	int	iXStep	=	abs( iXPos - iXPosOld );
	int iYStep	=	abs( iYPos - iYPosOld );

	if( bIsDragging == FALSE )
	{
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

int SPWindowSkill::OnContentCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	int	iLButtonUpID	=	( iID - WIID_SKILL_CONTENT_START );
	int	iStepID			=	( SCL_COUNT + SCGV_COUNT );
	int	iUIPos			=	iLButtonUpID / iStepID;
	int	iUIWindowID		=	iLButtonUpID % iStepID;

	SPUIUnit*	pUIUnit	=	m_UIUnitManager.GetUnit( iUIPos );
	SPWindow*	pWindow =	g_pInterfaceManager->GetToolTipWindow();

	if( iUIWindowID == SCL_BG )
	{
		if( pUIUnit != NULL && pWindow != NULL )
			pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER , (WPARAM)pUIUnit , SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );
		else
			pWindow->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );
	}
	else if( iUIWindowID == SCL_LEVEL_UP )
	{
		if( pUIUnit != NULL && pWindow != NULL )
			pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER , (WPARAM)pUIUnit , SPWindowToolTip::TOOLTIP_SHOW_TYPE_SKILL_LVUP );
		else
			pWindow->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );
	}

	return 1;
}

int SPWindowSkill::OnContentCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( SPDragnDropManager::GetInstance()->IsDragging() == FALSE )
	{
		m_bLMouseDown	=	FALSE;
		m_lParam		=	0;
	}

	SPWindow*	pWindow =	g_pInterfaceManager->GetToolTipWindow();

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );

	return 1;
}

int SPWindowSkill::OnContentWheelUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnWheelUp( wParam , lParam );
}

int SPWindowSkill::OnContentWheelDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnWheelDown( wParam , lParam );
}

// End of event map func (by metalgeni)
//////////////////////////////////////////////////////////////////////////

void SPWindowSkill::LoadLDTMainMenu()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_SKILL_MAIN_MENU , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [%s] \n" , RES_FNAME_LDT_SKILL_MAIN_MENU );
#endif
		return;
	}

	LDT_Field ldtField_Class;
	LDT_Field ldtField_Image_Path;
	LDT_Field ldtField_Image_Left;
	LDT_Field ldtField_Image_Top;
	LDT_Field ldtField_Image_Right;
	LDT_Field ldtField_Image_Bottom;
	LDT_Field ldtField_PosX;
	LDT_Field ldtField_PosY;
	LDT_Field ldtField_SubID;
	LDT_Field ldtField_Sub_PosX;
	LDT_Field ldtField_Sub_PosY;

	int				iRecordCount	=	pLDTFile->GetItemCount();
	int				iItemID;
	int				iSubID;
	char			szBuf[_MAX_PATH];
	UI_SUBID		stSubIDData;
	UI_MAIN_MENU	stMainMenuData;

	for( int iRecord = 0 ; iRecord < iRecordCount ; ++iRecord )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		stMainMenuData.m_iItemID	=	iItemID;

		pLDTFile->GetFieldFromLable( iItemID, "_Class",				ldtField_Class			);

		stMainMenuData.m_iClassType	=	ldtField_Class.uData.lValue;

		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Image",		ldtField_Image_Path		);
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Left",		ldtField_Image_Left		);
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Top",		ldtField_Image_Top		);
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Right",		ldtField_Image_Right	);
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Bottom",		ldtField_Image_Bottom	);

		if( ldtField_Image_Path.uData.pValue != NULL )
		{
			if( strlen( ldtField_Image_Path.uData.pValue ) > 0 )
			{
				stMainMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_strImage		=	ldtField_Image_Path.uData.pValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.left	=	ldtField_Image_Left.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.top	=	ldtField_Image_Top.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.right	=	ldtField_Image_Right.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.bottom	=	ldtField_Image_Bottom.uData.lValue;

				stMainMenuData.m_iXSize	=	stMainMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.right - stMainMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.left;
				stMainMenuData.m_iYSize	=	stMainMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.bottom - stMainMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.top;
			}
			else
			{
#ifdef _SKILL_DEBUG_PRINT
				DXUTOutputDebugString( "-----> [ String NULL : _Enable_Image ] [ ITEMID : %d ] [%s] \n" , iItemID , RES_FNAME_LDT_SKILL_MAIN_MENU );
#endif
			}
		}

		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Image",		ldtField_Image_Path		);
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Left",		ldtField_Image_Left		);
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Top",		ldtField_Image_Top		);
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Right",		ldtField_Image_Right	);
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Bottom",	ldtField_Image_Bottom	);

		if( ldtField_Image_Path.uData.pValue != NULL )
		{
			if( strlen( ldtField_Image_Path.uData.pValue ) > 0 )
			{
				stMainMenuData.m_stButton[ SKILL_BUTTON_DISABLE ].m_strImage		=	ldtField_Image_Path.uData.pValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_DISABLE ].m_rcSrc.left		=	ldtField_Image_Left.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_DISABLE ].m_rcSrc.top		=	ldtField_Image_Top.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_DISABLE ].m_rcSrc.right		=	ldtField_Image_Right.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_DISABLE ].m_rcSrc.bottom	=	ldtField_Image_Bottom.uData.lValue;
			}
			else
			{
#ifdef _SKILL_DEBUG_PRINT
				DXUTOutputDebugString( "-----> [ String NULL : _Disable_Image ] [ ITEMID : %d ] [%s] \n" , iItemID , RES_FNAME_LDT_SKILL_MAIN_MENU );
#endif
			}
		}

		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Image",	ldtField_Image_Path		);
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Left",	ldtField_Image_Left		);
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Top",		ldtField_Image_Top		);
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Right",	ldtField_Image_Right	);
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Bottom",	ldtField_Image_Bottom	);

		if( ldtField_Image_Path.uData.pValue != NULL )
		{
			if( strlen( ldtField_Image_Path.uData.pValue ) > 0 )
			{
				stMainMenuData.m_stButton[ SKILL_BUTTON_HIT ].m_strImage		=	ldtField_Image_Path.uData.pValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_HIT ].m_rcSrc.left		=	ldtField_Image_Left.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_HIT ].m_rcSrc.top		=	ldtField_Image_Top.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_HIT ].m_rcSrc.right		=	ldtField_Image_Right.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_HIT ].m_rcSrc.bottom	=	ldtField_Image_Bottom.uData.lValue;
			}
			else
			{
#ifdef _SKILL_DEBUG_PRINT
				DXUTOutputDebugString( "-----> [ String NULL : _MouseOver_Image ] [ ITEMID : %d ] [%s] \n" , iItemID , RES_FNAME_LDT_SKILL_MAIN_MENU );
#endif
			}
		}

		pLDTFile->GetFieldFromLable( iItemID, "_Click_Image",		ldtField_Image_Path		);
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Left",		ldtField_Image_Left		);
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Top",			ldtField_Image_Top		);
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Right",		ldtField_Image_Right	);
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Bottom",		ldtField_Image_Bottom	);

		if( ldtField_Image_Path.uData.pValue != NULL )
		{
			if( strlen( ldtField_Image_Path.uData.pValue ) > 0 )
			{
				stMainMenuData.m_stButton[ SKILL_BUTTON_CLICK ].m_strImage		=	ldtField_Image_Path.uData.pValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_CLICK ].m_rcSrc.left	=	ldtField_Image_Left.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_CLICK ].m_rcSrc.top		=	ldtField_Image_Top.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_CLICK ].m_rcSrc.right	=	ldtField_Image_Right.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_CLICK ].m_rcSrc.bottom	=	ldtField_Image_Bottom.uData.lValue;
			}
			else
			{
#ifdef _SKILL_DEBUG_PRINT
				DXUTOutputDebugString( "-----> [ String NULL : _Click_Image ] [ ITEMID : %d ] [%s] \n" , iItemID , RES_FNAME_LDT_SKILL_MAIN_MENU );
#endif
			}
		}

		pLDTFile->GetFieldFromLable( iItemID, "_Select_Image",		ldtField_Image_Path		);
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Left",		ldtField_Image_Left		);
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Top",		ldtField_Image_Top		);
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Right",		ldtField_Image_Right	);
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Bottom",		ldtField_Image_Bottom	);

		if( ldtField_Image_Path.uData.pValue != NULL )
		{
			if( strlen( ldtField_Image_Path.uData.pValue ) > 0 )
			{
				stMainMenuData.m_stButton[ SKILL_BUTTON_CHECK ].m_strImage		=	ldtField_Image_Path.uData.pValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_CHECK ].m_rcSrc.left	=	ldtField_Image_Left.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_CHECK ].m_rcSrc.top		=	ldtField_Image_Top.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_CHECK ].m_rcSrc.right	=	ldtField_Image_Right.uData.lValue;
				stMainMenuData.m_stButton[ SKILL_BUTTON_CHECK ].m_rcSrc.bottom	=	ldtField_Image_Bottom.uData.lValue;
			}
			else
			{
#ifdef _SKILL_DEBUG_PRINT
				DXUTOutputDebugString( "-----> [ String NULL : _Select_Image ] [ ITEMID : %d ] [%s] \n" , iItemID , RES_FNAME_LDT_SKILL_MAIN_MENU );
#endif
			}
		}

		pLDTFile->GetFieldFromLable( iItemID, "_PosX",				ldtField_PosX			);
		pLDTFile->GetFieldFromLable( iItemID, "_PosY",				ldtField_PosY			);

		stMainMenuData.m_iDestXPos	=	ldtField_PosX.uData.lValue;
		stMainMenuData.m_iDestYPos	=	ldtField_PosY.uData.lValue;

		for( iSubID = 0 ; iSubID < 10 ; ++iSubID )
		{
			sprintf( szBuf, "_SubID%d", iSubID + 1 );
			pLDTFile->GetFieldFromLable( iItemID, szBuf ,			ldtField_SubID			);

			sprintf( szBuf, "_Sub_PosX%d", iSubID + 1 );
			pLDTFile->GetFieldFromLable( iItemID, szBuf ,			ldtField_Sub_PosX		);

			sprintf( szBuf, "_Sub_PosY%d", iSubID + 1 );
			pLDTFile->GetFieldFromLable( iItemID, szBuf ,			ldtField_Sub_PosY		);

			if( ldtField_SubID.uData.lValue <= 0 )
			{
#ifdef _SKILL_DEBUG_PRINT
				DXUTOutputDebugString( "-----> [ SubID ID %d <= 0 ] [ ITEMID : %d ] [ SUBID : %d ] [%s] \n" , ldtField_SubID.uData.lValue , iItemID , iSubID , RES_FNAME_LDT_SKILL_MAIN_MENU );
#endif
				continue;
			}

			stSubIDData.m_iSubID	=	ldtField_SubID.uData.lValue;
			stSubIDData.m_iPosX		=	ldtField_Sub_PosX.uData.lValue;
			stSubIDData.m_iPosY		=	ldtField_Sub_PosY.uData.lValue;

			stMainMenuData.m_vUISubID.push_back( stSubIDData );
		}

		AddMainMenu( stMainMenuData.m_iClassType , stMainMenuData );

		stMainMenuData.m_vUISubID.clear();
	}

	SAFE_RELEASE(pLDTFile);
}

void SPWindowSkill::LoadLDTSubMenu()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_SKILL_SUB_MENU , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [%s] \n" , RES_FNAME_LDT_SKILL_SUB_MENU );
#endif
		return;
	}

	LDT_Field ldtField_Type;
	LDT_Field ldtField_Image_Path;
	LDT_Field ldtField_Image_Left;
	LDT_Field ldtField_Image_Top;
	LDT_Field ldtField_Image_Right;
	LDT_Field ldtField_Image_Bottom;

	int				iRecordCount	=	pLDTFile->GetItemCount();
	int				iItemID;
	UI_SUB_MENU		stSubMenuData;

	for( int iRecord = 0 ; iRecord < iRecordCount ; ++iRecord )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetFieldFromLable( iItemID, "_Type",				ldtField_Type			);

		stSubMenuData.m_eMenuType	=	(SKILL_CONTENT_VIEW)( ldtField_Type.uData.lValue );

		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Image",		ldtField_Image_Path		);
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Left",		ldtField_Image_Left		);
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Top",		ldtField_Image_Top		);
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Right",		ldtField_Image_Right	);
		pLDTFile->GetFieldFromLable( iItemID, "_Enable_Bottom",		ldtField_Image_Bottom	);

		if( ldtField_Image_Path.uData.pValue != NULL )
		{
			if( strlen( ldtField_Image_Path.uData.pValue ) > 0 )
			{
				stSubMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_strImage		=	ldtField_Image_Path.uData.pValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.left	=	ldtField_Image_Left.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.top		=	ldtField_Image_Top.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.right	=	ldtField_Image_Right.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.bottom	=	ldtField_Image_Bottom.uData.lValue;

				stSubMenuData.m_iXSize	=	stSubMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.right - stSubMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.left;
				stSubMenuData.m_iYSize	=	stSubMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.bottom - stSubMenuData.m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.top;

			}
			else
			{
#ifdef _SKILL_DEBUG_PRINT
				DXUTOutputDebugString( "-----> [ String NULL : _Enable_Image ] [ ITEMID : %d ] [%s] \n" , iItemID , RES_FNAME_LDT_SKILL_SUB_MENU );
#endif
			}
		}

		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Image",		ldtField_Image_Path		);
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Left",		ldtField_Image_Left		);
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Top",		ldtField_Image_Top		);
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Right",		ldtField_Image_Right	);
		pLDTFile->GetFieldFromLable( iItemID, "_Disable_Bottom",	ldtField_Image_Bottom	);

		if( ldtField_Image_Path.uData.pValue != NULL )
		{
			if( strlen( ldtField_Image_Path.uData.pValue ) > 0 )
			{
				stSubMenuData.m_stButton[ SKILL_BUTTON_DISABLE ].m_strImage			=	ldtField_Image_Path.uData.pValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_DISABLE ].m_rcSrc.left		=	ldtField_Image_Left.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_DISABLE ].m_rcSrc.top		=	ldtField_Image_Top.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_DISABLE ].m_rcSrc.right		=	ldtField_Image_Right.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_DISABLE ].m_rcSrc.bottom		=	ldtField_Image_Bottom.uData.lValue;
			}
			else
			{
#ifdef _SKILL_DEBUG_PRINT
				DXUTOutputDebugString( "-----> [ String NULL : _Disable_Image ] [ ITEMID : %d ] [%s] \n" , iItemID , RES_FNAME_LDT_SKILL_SUB_MENU );
#endif
			}
		}

		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Image",	ldtField_Image_Path		);
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Left",	ldtField_Image_Left		);
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Top",		ldtField_Image_Top		);
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Right",	ldtField_Image_Right	);
		pLDTFile->GetFieldFromLable( iItemID, "_MouseOver_Bottom",	ldtField_Image_Bottom	);

		if( ldtField_Image_Path.uData.pValue != NULL )
		{
			if( strlen( ldtField_Image_Path.uData.pValue ) > 0 )
			{
				stSubMenuData.m_stButton[ SKILL_BUTTON_HIT ].m_strImage			=	ldtField_Image_Path.uData.pValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_HIT ].m_rcSrc.left		=	ldtField_Image_Left.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_HIT ].m_rcSrc.top		=	ldtField_Image_Top.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_HIT ].m_rcSrc.right		=	ldtField_Image_Right.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_HIT ].m_rcSrc.bottom		=	ldtField_Image_Bottom.uData.lValue;
			}
			else
			{
#ifdef _SKILL_DEBUG_PRINT
				DXUTOutputDebugString( "-----> [ String NULL : _MouseOver_Image ] [ ITEMID : %d ] [%s] \n" , iItemID , RES_FNAME_LDT_SKILL_SUB_MENU );
#endif
			}
		}

		pLDTFile->GetFieldFromLable( iItemID, "_Click_Image",		ldtField_Image_Path		);
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Left",		ldtField_Image_Left		);
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Top",			ldtField_Image_Top		);
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Right",		ldtField_Image_Right	);
		pLDTFile->GetFieldFromLable( iItemID, "_Click_Bottom",		ldtField_Image_Bottom	);

		if( ldtField_Image_Path.uData.pValue != NULL )
		{
			if( strlen( ldtField_Image_Path.uData.pValue ) > 0 )
			{
				stSubMenuData.m_stButton[ SKILL_BUTTON_CLICK ].m_strImage		=	ldtField_Image_Path.uData.pValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_CLICK ].m_rcSrc.left		=	ldtField_Image_Left.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_CLICK ].m_rcSrc.top		=	ldtField_Image_Top.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_CLICK ].m_rcSrc.right	=	ldtField_Image_Right.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_CLICK ].m_rcSrc.bottom	=	ldtField_Image_Bottom.uData.lValue;
			}
			else
			{
#ifdef _SKILL_DEBUG_PRINT
				DXUTOutputDebugString( "-----> [ String NULL : _Click_Image ] [ ITEMID : %d ] [%s] \n" , iItemID , RES_FNAME_LDT_SKILL_SUB_MENU );
#endif
			}
		}

		pLDTFile->GetFieldFromLable( iItemID, "_Select_Image",		ldtField_Image_Path		);
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Left",		ldtField_Image_Left		);
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Top",		ldtField_Image_Top		);
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Right",		ldtField_Image_Right	);
		pLDTFile->GetFieldFromLable( iItemID, "_Select_Bottom",		ldtField_Image_Bottom	);

		if( ldtField_Image_Path.uData.pValue != NULL )
		{
			if( strlen( ldtField_Image_Path.uData.pValue ) > 0 )
			{
				stSubMenuData.m_stButton[ SKILL_BUTTON_CHECK ].m_strImage		=	ldtField_Image_Path.uData.pValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_CHECK ].m_rcSrc.left		=	ldtField_Image_Left.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_CHECK ].m_rcSrc.top		=	ldtField_Image_Top.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_CHECK ].m_rcSrc.right	=	ldtField_Image_Right.uData.lValue;
				stSubMenuData.m_stButton[ SKILL_BUTTON_CHECK ].m_rcSrc.bottom	=	ldtField_Image_Bottom.uData.lValue;
			}
			else
			{
#ifdef _SKILL_DEBUG_PRINT
				DXUTOutputDebugString( "-----> [ String NULL : _Select_Image ] [ ITEMID : %d ] [%s] \n" , iItemID , RES_FNAME_LDT_SKILL_SUB_MENU );
#endif
			}
		}

		AddSubMenu( iItemID , stSubMenuData );
	}

	SAFE_RELEASE(pLDTFile);
}

void SPWindowSkill::LoadLDTContent()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_SKILL_CONTENT , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [%s] \n" , RES_FNAME_LDT_SKILL_CONTENT );
#endif
		return;
	}

	LDT_Field ldtField_SubID;
	LDT_Field ldtField_Grid_Index;
	LDT_Field ldtField_Image_Setting;
	LDT_Field ldtField_SkillID;

	int			iRecordCount	=	pLDTFile->GetItemCount();
	int			iItemID;
	UI_CONTENT	stContent;

	for( int iRecord = 0 ; iRecord < iRecordCount ; ++iRecord )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetFieldFromLable( iItemID, "_SubID",				ldtField_SubID			);
		pLDTFile->GetFieldFromLable( iItemID, "_Grid_Index",		ldtField_Grid_Index		);
		pLDTFile->GetFieldFromLable( iItemID, "_Image_Setting",		ldtField_Image_Setting	);
		pLDTFile->GetFieldFromLable( iItemID, "_SkillID",			ldtField_SkillID		);

		stContent.m_iSubID			=	ldtField_SubID.uData.lValue;
		stContent.m_iGridIndex		=	ldtField_Grid_Index.uData.lValue - 1;
		stContent.m_dwImageSetting	=	ldtField_Image_Setting.uData.lValue;
		stContent.m_iSkillID		=	ldtField_SkillID.uData.lValue;

		AddContent( GetContentKey( stContent.m_iSubID , stContent.m_iGridIndex ) , stContent );
	}

	SAFE_RELEASE(pLDTFile);
}

void SPWindowSkill::LoadLDTTreeSetting( STD_VECTOR_UI_TREE& vTreeSetting )
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_SKILL_TREESETTING , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [%s] \n" , RES_FNAME_LDT_SKILL_TREESETTING );
#endif
		return;
	}

	LDT_Field ldtField_Image_Path;
	LDT_Field ldtField_Image_Left;
	LDT_Field ldtField_Image_Top;
	LDT_Field ldtField_Image_Right;
	LDT_Field ldtField_Image_Bottom;
	LDT_Field ldtField_PosX;
	LDT_Field ldtField_PosY;

	int			iRecordCount	=	pLDTFile->GetItemCount();
	int			iItemID;
	UI_TREE		stTree;

	vTreeSetting.clear();

	for( int iRecord = 0 ; iRecord < iRecordCount ; ++iRecord )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetFieldFromLable( iItemID, "_Image",			ldtField_Image_Path		);
		pLDTFile->GetFieldFromLable( iItemID, "_Left",			ldtField_Image_Left		);
		pLDTFile->GetFieldFromLable( iItemID, "_Top",			ldtField_Image_Top		);
		pLDTFile->GetFieldFromLable( iItemID, "_Right",			ldtField_Image_Right	);
		pLDTFile->GetFieldFromLable( iItemID, "_Bottom",		ldtField_Image_Bottom	);
		pLDTFile->GetFieldFromLable( iItemID, "_Image_PosX",	ldtField_PosX			);
		pLDTFile->GetFieldFromLable( iItemID, "_Image_PosY",	ldtField_PosY			);

		if( ldtField_Image_Path.uData.pValue != NULL )
		{
			if( strlen( ldtField_Image_Path.uData.pValue ) > 0 )
			{
				stTree.m_stImage.m_strImage		=	ldtField_Image_Path.uData.pValue;
			}
		}

		stTree.m_iItemID				=	iItemID;
		stTree.m_stImage.m_rcSrc.left	=	ldtField_Image_Left.uData.lValue;
		stTree.m_stImage.m_rcSrc.top	=	ldtField_Image_Top.uData.lValue;
		stTree.m_stImage.m_rcSrc.right	=	ldtField_Image_Right.uData.lValue;
		stTree.m_stImage.m_rcSrc.bottom	=	ldtField_Image_Bottom.uData.lValue;
		stTree.m_iDestXPos				=	ldtField_PosX.uData.lValue;
		stTree.m_iDestYPos				=	ldtField_PosY.uData.lValue;
		stTree.m_iXSize					=	stTree.m_stImage.m_rcSrc.right - stTree.m_stImage.m_rcSrc.left;
		stTree.m_iYSize					=	stTree.m_stImage.m_rcSrc.bottom - stTree.m_stImage.m_rcSrc.top;

		vTreeSetting.push_back( stTree );

		stTree.m_stImage.m_strImage.clear();
	}

	SAFE_RELEASE(pLDTFile);
}

void SPWindowSkill::LoadLDTLearnSkill()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_SKILL_LEARN , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [%s] \n" , RES_FNAME_LDT_SKILL_LEARN );
#endif
		return;
	}

	LDT_Field ldtField_Name;
	LDT_Field ldtField_Ely;
	LDT_Field ldtField_ItemID;
//	LDT_Field ldtField_ItemCount;
	LDT_Field ldtField_RequireType;
	LDT_Field ldtField_RequireID;
	LDT_Field ldtField_RequireValue1;
	LDT_Field ldtField_RequireValue2;
	LDT_Field ldtField_RequireErrorMsg;
	LDT_Field ldtField_Desc;
	LDT_Field ldtField_Success;
	LDT_Field ldtField_FX;
	LDT_Field ldtField_ErrorMsgEly;
	LDT_Field ldtField_ErrorMsgItem;

	int						iRecordCount	=	pLDTFile->GetItemCount();
	int						iItemID;
	int						iRequire , iFieldPos;
	SKILL_LEARN_LDT_DATA	stSkillLearnLDtData;

	for( int iRecord = 0 ; iRecord < iRecordCount ; ++iRecord )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		stSkillLearnLDtData.m_iKeyID	=	iItemID;

		pLDTFile->GetField( iItemID , 0 , ldtField_Name );			// _Name
		pLDTFile->GetField( iItemID , 1 , ldtField_Ely );			// _Consumption_Ely
		pLDTFile->GetField( iItemID , 2 , ldtField_ItemID );		// _Consumption_Item
//		pLDTFile->GetField( iItemID , 3 , ldtField_ItemCount );		// _Consumption_ItemCount

		for( iRequire = 0 ; iRequire < LS_REQUIRE_COUNT ; ++iRequire )
		{
			iFieldPos	=	3 + ( iRequire * 4 );
			pLDTFile->GetField( iItemID , iFieldPos++	, ldtField_RequireType		);		// _Require%d_Type
			pLDTFile->GetField( iItemID , iFieldPos++	, ldtField_RequireID		);		// _Require%d_ID
			pLDTFile->GetField( iItemID , iFieldPos++	, ldtField_RequireValue1	);		// _Require%d_Value1
			pLDTFile->GetField( iItemID , iFieldPos++	, ldtField_RequireValue2	);		// _Require%d_Value2
			pLDTFile->GetField( iItemID , 40 + iRequire	, ldtField_RequireErrorMsg	);		// _ErrorMessage_Require%d

			stSkillLearnLDtData.m_stRequireCondition[ iRequire ].m_iRequireType		=	ldtField_RequireType.uData.lValue;
			stSkillLearnLDtData.m_stRequireCondition[ iRequire ].m_iRequireID		=	ldtField_RequireID.uData.lValue;
			stSkillLearnLDtData.m_stRequireCondition[ iRequire ].m_iRequireValue1	=	ldtField_RequireValue1.uData.lValue;
			stSkillLearnLDtData.m_stRequireCondition[ iRequire ].m_iRequireValue2	=	ldtField_RequireValue2.uData.lValue;

			if( ldtField_RequireErrorMsg.uData.pValue != NULL )
			{
				if( strlen( ldtField_RequireErrorMsg.uData.pValue ) > 0 )
				{
					stSkillLearnLDtData.m_stRequireCondition[ iRequire ].m_strErrorMsg	=	ldtField_RequireErrorMsg.uData.pValue;
				}
			}
		}

		pLDTFile->GetField( iItemID , 35 , ldtField_Desc );			// _Description
		pLDTFile->GetField( iItemID , 36 , ldtField_Success );		// _Message
		pLDTFile->GetField( iItemID , 37 , ldtField_FX );			// _FX
		pLDTFile->GetField( iItemID , 38 , ldtField_ErrorMsgEly );	// _ErrorMessage_Ely
		pLDTFile->GetField( iItemID , 39 , ldtField_ErrorMsgItem );	// _ErrorMessage_Item

		if( ldtField_Name.uData.pValue != NULL )
		{
			if( strlen( ldtField_Name.uData.pValue ) > 0 )
			{
				stSkillLearnLDtData.m_strName	=	ldtField_Name.uData.pValue;
			}
		}

		stSkillLearnLDtData.m_uiConsumptionEly		=	ldtField_Ely.uData.lValue;
		stSkillLearnLDtData.m_iConsumptionItemID	=	ldtField_ItemID.uData.lValue;
//		stSkillLearnLDtData.m_iConsumptionItemCount	=	ldtField_ItemCount.uData.lValue;
		stSkillLearnLDtData.m_iFXGroupID			=	ldtField_FX.uData.lValue;

		if( ldtField_Desc.uData.pValue != NULL )
		{
			if( strlen( ldtField_Desc.uData.pValue ) > 0 )
			{
				stSkillLearnLDtData.m_strDesc	=	ldtField_Desc.uData.pValue;
			}
		}

		if( ldtField_Success.uData.pValue != NULL )
		{
			if( strlen( ldtField_Success.uData.pValue ) > 0 )
			{
				stSkillLearnLDtData.m_strSuccess	=	ldtField_Success.uData.pValue;
			}
		}

		if( ldtField_ErrorMsgEly.uData.pValue != NULL )
		{
			if( strlen( ldtField_ErrorMsgEly.uData.pValue ) > 0 )
			{
				stSkillLearnLDtData.m_strErrorEly	=	ldtField_ErrorMsgEly.uData.pValue;
			}
		}

		if( ldtField_ErrorMsgItem.uData.pValue != NULL )
		{
			if( strlen( ldtField_ErrorMsgItem.uData.pValue ) > 0 )
			{
				stSkillLearnLDtData.m_strErrorItem	=	ldtField_ErrorMsgItem.uData.pValue;
			}
		}

		AddLearnSkillLdt( stSkillLearnLDtData );
		ClearLearnSkillLdt( stSkillLearnLDtData );
	}

	SAFE_RELEASE(pLDTFile);
}

void SPWindowSkill::CreateWindowMainMenu()
{
	for( int i = 0 ; i < SKILL_MAIN_MENU_MAX ; ++i )
	{
		m_pMainMenuButton[ i ]	=	new SPWindowButton( WIID_SKILL_MAIN_MENU_START + i , 0 , 0 , 0 , 0 , this );
	}
}

void SPWindowSkill::CreateWindowSubMenu()
{
	for( int i = 0 ; i < SKILL_SUB_MENU_MAX ; ++i )
	{
		m_pSubMenuButton[ i ]	=	new SPWindowButton( WIID_SKILL_SUB_MENU_START + i , 0 , 0 , 0 , 0 , this );
	}
}

void SPWindowSkill::CreateWindowContent()
{
	char				szResPath[]		=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
	SPWindowButton*		pWindowButton	=	NULL;
	SPUISkillUnit*		pUISkillUnit	=	NULL;
	UI_CONTENT_WINDOW	stUIContentWindow;
	INSTANCE_ID			eInstanceID;
	int					iXPos , iYPos;
	int					iUnitNumber		=	0;

	UI_TREE*			pUITree			=	NULL;
	STD_VECTOR_UI_TREE	vUITreeSetting;

	vUITreeSetting.reserve( SCGV_COUNT );

	LoadLDTTreeSetting( vUITreeSetting );

	if( (int)vUITreeSetting.size() <= 0 )
		return;
    
	m_UIUnitManager.SetBufferSize( SKILL_GRID_X_COUNT * SKILL_GRID_Y_COUNT );

	for( int y = 0 ; y < SKILL_GRID_Y_COUNT ; ++y )
	{
		for( int x = 0 ; x < SKILL_GRID_X_COUNT ; ++x )
		{
			iXPos	=	SKILL_GRID_X_OFFSET + ( x * SKILL_GRID_X_SIZE );
			iYPos	=	SKILL_GRID_Y_OFFSET + ( y * SKILL_GRID_Y_SIZE );

			eInstanceID	=	WIID_SKILL_CONTENT_START + ( iUnitNumber * ( SCL_COUNT + SCGV_COUNT ) );
			++iUnitNumber;

			pUITree	=	&( vUITreeSetting.at( SCGV_SLOT_GENERAL ) );

			if( pUITree != NULL )
			{
				stUIContentWindow.m_pContent[ SCL_BG ]	=	new SPWindowStatic( eInstanceID + SCL_BG , iXPos + pUITree->m_iDestXPos , iYPos + pUITree->m_iDestYPos , pUITree->m_iXSize , pUITree->m_iYSize , this );
				stUIContentWindow.m_pContent[ SCL_BG ]->SetImage( pUITree->m_stImage.m_strImage.c_str() , pUITree->m_stImage.m_rcSrc.left , pUITree->m_stImage.m_rcSrc.top );		//	일반

				pUITree	=	&( vUITreeSetting.at( SCGV_SLOT_LEVEL0 ) );

				if( pUITree != NULL )
					stUIContentWindow.m_pContent[ SCL_BG ]->SetImage( pUITree->m_stImage.m_strImage.c_str() , pUITree->m_stImage.m_rcSrc.left , pUITree->m_stImage.m_rcSrc.top );	//	스킬레벨 0

				pUITree	=	&( vUITreeSetting.at( SCGV_SLOT_EMPTY ) );

				if( pUITree != NULL )
					stUIContentWindow.m_pContent[ SCL_BG ]->SetImage( pUITree->m_stImage.m_strImage.c_str() , pUITree->m_stImage.m_rcSrc.left , pUITree->m_stImage.m_rcSrc.top );	//	비어 있을때

			}

			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]	=	new SPWindowStatic( eInstanceID + SCL_CUR_LEVEL10 , iXPos + 14 , iYPos + 51 , 5 , 7 , this );
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]->SetImage( szResPath , 153 , 380 );	//	0
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]->SetImage( szResPath , 159 , 380 );	//	1
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]->SetImage( szResPath , 165 , 380 );	//	2
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]->SetImage( szResPath , 171 , 380 );	//	3
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]->SetImage( szResPath , 177 , 380 );	//	4
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]->SetImage( szResPath , 183 , 380 );	//	5
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]->SetImage( szResPath , 189 , 380 );	//	6
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]->SetImage( szResPath , 195 , 380 );	//	7
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]->SetImage( szResPath , 201 , 380 );	//	8
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]->SetImage( szResPath , 207 , 380 );	//	9

			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]	=	new SPWindowStatic( eInstanceID + SCL_CUR_LEVEL1 , iXPos + 20 , iYPos + 51 , 5 , 7 , this );
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]->SetImage( szResPath , 153 , 380 );	//	0
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]->SetImage( szResPath , 159 , 380 );	//	1
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]->SetImage( szResPath , 165 , 380 );	//	2
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]->SetImage( szResPath , 171 , 380 );	//	3
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]->SetImage( szResPath , 177 , 380 );	//	4
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]->SetImage( szResPath , 183 , 380 );	//	5
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]->SetImage( szResPath , 189 , 380 );	//	6
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]->SetImage( szResPath , 195 , 380 );	//	7
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]->SetImage( szResPath , 201 , 380 );	//	8
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]->SetImage( szResPath , 207 , 380 );	//	9

			stUIContentWindow.m_pContent[ SCL_LEVEL_SLASH ]	=	new SPWindowStatic( eInstanceID + SCL_LEVEL_SLASH , iXPos + 27 , iYPos + 52 , 3 , 6 , this );
			stUIContentWindow.m_pContent[ SCL_LEVEL_SLASH ]->SetImage( szResPath , 220 , 381 );	//	/

			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]=	new SPWindowStatic( eInstanceID + SCL_MAX_LEVEL10 , iXPos + 32 , iYPos + 51 , 5 , 7 , this );
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]->SetImage( szResPath , 153 , 380 );	//	0
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]->SetImage( szResPath , 159 , 380 );	//	1
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]->SetImage( szResPath , 165 , 380 );	//	2
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]->SetImage( szResPath , 171 , 380 );	//	3
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]->SetImage( szResPath , 177 , 380 );	//	4
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]->SetImage( szResPath , 183 , 380 );	//	5
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]->SetImage( szResPath , 189 , 380 );	//	6
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]->SetImage( szResPath , 195 , 380 );	//	7
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]->SetImage( szResPath , 201 , 380 );	//	8
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]->SetImage( szResPath , 207 , 380 );	//	9

			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]	=	new SPWindowStatic( eInstanceID + SCL_MAX_LEVEL1 , iXPos + 38 , iYPos + 51 , 5 , 7 , this );
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]->SetImage( szResPath , 153 , 380 );	//	0
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]->SetImage( szResPath , 159 , 380 );	//	1
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]->SetImage( szResPath , 165 , 380 );	//	2
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]->SetImage( szResPath , 171 , 380 );	//	3
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]->SetImage( szResPath , 177 , 380 );	//	4
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]->SetImage( szResPath , 183 , 380 );	//	5
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]->SetImage( szResPath , 189 , 380 );	//	6
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]->SetImage( szResPath , 195 , 380 );	//	7
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]->SetImage( szResPath , 201 , 380 );	//	8
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]->SetImage( szResPath , 207 , 380 );	//	9

			pWindowButton	=	new SPWindowButton( eInstanceID + SCL_LEVEL_UP, iXPos + 0 , iYPos + 50 , 11 , 11 , this );
			pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 262 , 188 );
			pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 262 , 200 );
			pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 262 , 212 );
			pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 239 , 170 );

			stUIContentWindow.m_pContent[ SCL_LEVEL_UP ]	=	pWindowButton;

			pUITree	=	&( vUITreeSetting.at( SCGV_SLOT_NOT_LEARN ) );

			if( pUITree != NULL )
			{
				stUIContentWindow.m_pContent[ SCL_NOT_LEARN ]	=	new SPWindowStatic( eInstanceID + SCL_NOT_LEARN , iXPos + pUITree->m_iDestXPos , iYPos + pUITree->m_iDestYPos , pUITree->m_iXSize , pUITree->m_iYSize , this );
				stUIContentWindow.m_pContent[ SCL_NOT_LEARN ]->SetImage( pUITree->m_stImage.m_strImage.c_str() , pUITree->m_stImage.m_rcSrc.left , pUITree->m_stImage.m_rcSrc.top );	//	안 배웠을때 ? / ?
			}

			for( int i = 0 ; i < SCGV_COUNT ; ++i )
			{
				pUITree	=	&( vUITreeSetting.at( i ) );

				if( pUITree == NULL )
					continue;

				stUIContentWindow.m_pImages[ i ]	=	new SPWindowStatic( eInstanceID + SCL_COUNT + i , iXPos + pUITree->m_iDestXPos , iYPos + pUITree->m_iDestYPos , pUITree->m_iXSize , pUITree->m_iYSize , this );
				stUIContentWindow.m_pImages[ i ]->SetImage( pUITree->m_stImage.m_strImage.c_str() , pUITree->m_stImage.m_rcSrc.left , pUITree->m_stImage.m_rcSrc.top );
			}

			m_vUIContentWindow.push_back( stUIContentWindow );

			pUISkillUnit	=	new SPUISkillUnit;

			pUISkillUnit->SetPos( m_iAX + iXPos + SKILL_UNIT_X_OFFSET , m_iAY + iYPos + SKILL_UNIT_Y_OFFSET );
			pUISkillUnit->SetSize( 32 , 32 );
			pUISkillUnit->SetDisableTexture( szResPath );
			pUISkillUnit->SetDisableRectSrc( 434 , 37 , 436 , 39 );
			pUISkillUnit->SetCoolTimeTexture( szResPath );
			pUISkillUnit->SetCoolTimeRectSrc( 434 , 37 , 436 , 39 );

			pUISkillUnit->SetComboTexture( "DATA/INTERFACE/CONCEPT/Combo_Skill_ICON.png" );
			pUISkillUnit->SetComboRectSrc( 0 , 0 , 32 , 32 );

			pUISkillUnit->SetToggleTexture( "DATA/INTERFACE/CONCEPT/ui_Icon_On.png" );

			pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
			pUISkillUnit->SetInstanceID( WIID_SKILL );

			m_UIUnitManager.AddUnit( pUISkillUnit );
		}
	}
}

void SPWindowSkill::UpdateAll()
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPGOBStatus* pPlayerStatus	=	pLocalPlayer->GetStatus();

	if( pPlayerStatus == NULL )
		return;

	int iClassType	=	(int)( pPlayerStatus->GetStatusValue( STATUS_CLASS_TYPE ) );

	if( iClassType != m_iCurClassType )
		m_iCurClassType	=	iClassType;

	UpdateMainMenu( m_iCurClassType );

	if( m_pCurMainMenu == NULL && m_iCurSubMenu == 0 )
	{
		OnMainMenuLButtonUp( WIID_SKILL_MAIN_MENU_START + ( m_iCurClassType * SKILL_CLASS_SEPARATION + 1 )  , NULL , NULL );

		if( m_pCurMainMenu != NULL )
		{
			if( (int)m_pCurMainMenu->m_vUISubID.size() > 0 )
			{
				UI_SUBID&	stUISubID	=	m_pCurMainMenu->m_vUISubID.at( 0 );
				OnSubMenuLButtonUp( WIID_SKILL_SUB_MENU_START + stUISubID.m_iSubID , NULL , NULL );
			}
		}
	}
	else if( m_pCurMainMenu != NULL && m_iCurSubMenu != 0 )
	{
		int		iCurContentPageKey	=	GetContentPageKey( m_pCurMainMenu->m_iItemID , m_iCurSubMenu );
		int*	pCurContentPage		=	GetContentPage( iCurContentPageKey );

		if( pCurContentPage == NULL )
		{
			SetContentPage( iCurContentPageKey , 0 );
			pCurContentPage		=	GetContentPage( iCurContentPageKey );
		}

		UpdateContent( m_iCurSubMenu , *pCurContentPage );
	}

	SetSkillPoints();

	int			iCursorX	=	g_pInterfaceManager->GetCursorX();
	int			iCursorY	=	g_pInterfaceManager->GetCursorY();
	int			iTotalCount	=	SKILL_GRID_X_COUNT	*	SKILL_GRID_Y_COUNT;
	INSTANCE_ID	eInstanceID;
	SPWindow*	pWindow;

	if( IsShow() == true && IsCursorIn( iCursorX , iCursorY ) == true )
	{
		OnContentCursorOut( WIID_SKILL_CONTENT_START + SCL_LEVEL_UP , NULL , NULL );

		for( int i = 0 ; i < iTotalCount ; ++i )
		{
			eInstanceID	=	WIID_SKILL_CONTENT_START + ( i * ( SCL_COUNT + SCGV_COUNT ) );

			pWindow		=	Find( eInstanceID + SCL_LEVEL_UP );

			if( pWindow != NULL )
			{
				if( pWindow->IsCursorIn( iCursorX , iCursorY ) == true && pWindow->IsShow() == true )
				{
					OnContentCursorEnter( pWindow->GetInstanceID() , NULL , NULL);
					break;
				}
			}

			pWindow		=	Find( eInstanceID + SCL_BG );

			if( pWindow != NULL )
			{
				if( pWindow->IsCursorIn( iCursorX , iCursorY ) == true && pWindow->IsShow() == true )
				{
					OnContentCursorEnter( pWindow->GetInstanceID() , NULL , NULL);
					break;
				}
			}
		}
	}
}

void SPWindowSkill::UpdateMainMenu( int iClassType )
{
	int	iMainMenuCount	=	GetMainMenuCount( iClassType );

	if( iMainMenuCount <= 0 )
		return;

	if( iMainMenuCount > SKILL_MAIN_MENU_MAX )
		iMainMenuCount = SKILL_MAIN_MENU_MAX;

	UI_MAIN_MENU*	pUIMainMenu	=	NULL;

	for( int i = 0 ; i < iMainMenuCount ; ++i )
	{
		pUIMainMenu	=	GetMainMenu( iClassType , i );

		if( pUIMainMenu == NULL )
			continue;

		m_pMainMenuButton[ i ]->SetWidnowInstanceID( WIID_SKILL_MAIN_MENU_START + pUIMainMenu->m_iItemID );
		m_pMainMenuButton[ i ]->AdjustCoord( pUIMainMenu->m_iDestXPos , pUIMainMenu->m_iDestYPos , pUIMainMenu->m_iXSize , pUIMainMenu->m_iYSize );
		m_pMainMenuButton[ i ]->SetImage		( pUIMainMenu->m_stButton[ SKILL_BUTTON_ENABLE ].m_strImage.c_str() ,	pUIMainMenu->m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.left ,	pUIMainMenu->m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.top	);
		m_pMainMenuButton[ i ]->SetImageHit		( pUIMainMenu->m_stButton[ SKILL_BUTTON_HIT ].m_strImage.c_str() ,		pUIMainMenu->m_stButton[ SKILL_BUTTON_HIT ].m_rcSrc.left ,		pUIMainMenu->m_stButton[ SKILL_BUTTON_HIT ].m_rcSrc.top		);
		m_pMainMenuButton[ i ]->SetImagePush	( pUIMainMenu->m_stButton[ SKILL_BUTTON_CLICK ].m_strImage.c_str() ,	pUIMainMenu->m_stButton[ SKILL_BUTTON_CLICK ].m_rcSrc.left ,	pUIMainMenu->m_stButton[ SKILL_BUTTON_CLICK ].m_rcSrc.top	);
		m_pMainMenuButton[ i ]->SetImageDisable	( pUIMainMenu->m_stButton[ SKILL_BUTTON_DISABLE ].m_strImage.c_str() ,	pUIMainMenu->m_stButton[ SKILL_BUTTON_DISABLE ].m_rcSrc.left ,	pUIMainMenu->m_stButton[ SKILL_BUTTON_DISABLE ].m_rcSrc.top	);
		m_pMainMenuButton[ i ]->SetImageCheck	( pUIMainMenu->m_stButton[ SKILL_BUTTON_CHECK ].m_strImage.c_str() ,	pUIMainMenu->m_stButton[ SKILL_BUTTON_CHECK ].m_rcSrc.left ,	pUIMainMenu->m_stButton[ SKILL_BUTTON_CHECK ].m_rcSrc.top	);
		m_pMainMenuButton[ i ]->SPSendMessage( SPIM_CURSOR_OUT );
		m_pMainMenuButton[ i ]->Show();
	}

	for( int j = i ; j < SKILL_MAIN_MENU_MAX ; ++j )
	{
		m_pMainMenuButton[ j ]->Hide();
	}

	UpdateSubMenu( m_pCurMainMenu );
}

void SPWindowSkill::UpdateSubMenu( UI_MAIN_MENU* pUIMainMenu )
{
	m_pCurMainMenu = pUIMainMenu;

	int	i = 0;

	if( pUIMainMenu != NULL )
	{
		STD_VECTOR_UI_SUBID& mUISubMenu	= pUIMainMenu->m_vUISubID;

		int	iSubMenuCount	=	(int)mUISubMenu.size();

		if( iSubMenuCount <= 0 )
			return;

		if( iSubMenuCount > SKILL_SUB_MENU_MAX )
			iSubMenuCount = SKILL_SUB_MENU_MAX;

		UI_SUBID*		pUISubID	=	NULL;
		UI_SUB_MENU*	pUISubMenu	=	NULL;

		for( i = 0 ; i < iSubMenuCount ; ++i )
		{
			pUISubID	=	&mUISubMenu.at( i );

			if( pUISubID == NULL )
				continue;

			pUISubMenu	=	GetSubMenu( pUISubID->m_iSubID );

			if( pUISubMenu == NULL )
				continue;

			m_pSubMenuButton[ i ]->SetWidnowInstanceID( WIID_SKILL_SUB_MENU_START + pUISubID->m_iSubID );
			m_pSubMenuButton[ i ]->AdjustCoord( pUISubID->m_iPosX , pUISubID->m_iPosY , pUISubMenu->m_iXSize , pUISubMenu->m_iYSize );
			m_pSubMenuButton[ i ]->SetImage			( pUISubMenu->m_stButton[ SKILL_BUTTON_ENABLE ].m_strImage.c_str() ,	pUISubMenu->m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.left ,	pUISubMenu->m_stButton[ SKILL_BUTTON_ENABLE ].m_rcSrc.top	);
			m_pSubMenuButton[ i ]->SetImageHit		( pUISubMenu->m_stButton[ SKILL_BUTTON_HIT ].m_strImage.c_str() ,		pUISubMenu->m_stButton[ SKILL_BUTTON_HIT ].m_rcSrc.left ,		pUISubMenu->m_stButton[ SKILL_BUTTON_HIT ].m_rcSrc.top		);
			m_pSubMenuButton[ i ]->SetImagePush		( pUISubMenu->m_stButton[ SKILL_BUTTON_CLICK ].m_strImage.c_str() ,		pUISubMenu->m_stButton[ SKILL_BUTTON_CLICK ].m_rcSrc.left ,		pUISubMenu->m_stButton[ SKILL_BUTTON_CLICK ].m_rcSrc.top	);
			m_pSubMenuButton[ i ]->SetImageDisable	( pUISubMenu->m_stButton[ SKILL_BUTTON_DISABLE ].m_strImage.c_str() ,	pUISubMenu->m_stButton[ SKILL_BUTTON_DISABLE ].m_rcSrc.left ,	pUISubMenu->m_stButton[ SKILL_BUTTON_DISABLE ].m_rcSrc.top	);
			m_pSubMenuButton[ i ]->SetImageCheck	( pUISubMenu->m_stButton[ SKILL_BUTTON_CHECK ].m_strImage.c_str() ,		pUISubMenu->m_stButton[ SKILL_BUTTON_CHECK ].m_rcSrc.left ,		pUISubMenu->m_stButton[ SKILL_BUTTON_CHECK ].m_rcSrc.top	);
			m_pSubMenuButton[ i ]->SPSendMessage( SPIM_CURSOR_OUT );
			m_pSubMenuButton[ i ]->Show();
		}
	}

	for( int j = i ; j < SKILL_SUB_MENU_MAX ; ++j )
	{
		m_pSubMenuButton[ j ]->Hide();
	}
}

void SPWindowSkill::UpdateContent( int iCurSubMenu , int iCurContentPage )
{
	UI_SUB_MENU*	pUISubMenu	=	GetSubMenu( iCurSubMenu );

	if( pUISubMenu == NULL )
		return;

	m_iCurSubMenu		=	iCurSubMenu;

	if( m_pCurMainMenu != NULL && m_iCurSubMenu != 0 )
	{
		SetSubMenuPage( m_pCurMainMenu->m_iItemID , m_iCurSubMenu );

		int		iCurContentPageKey	=	GetContentPageKey( m_pCurMainMenu->m_iItemID , m_iCurSubMenu );
		SetContentPage( iCurContentPageKey , iCurContentPage );
	}

	int	iEndPage = GetContentEndPage( m_iCurSubMenu );

	SPWindow*	pWindow	=	Find( WIID_SKILL_SCROLL_BAR , true );

	if( pWindow != NULL )
	{
		pWindow->SPSendMessage( SPIM_SET_CURVALUE , iCurContentPage );
		pWindow->SPSendMessage( SPIM_SET_MAXVALUE , iEndPage );
	}

	int	iGridCount		=	SKILL_GRID_X_COUNT * SKILL_GRID_Y_COUNT;

	for( int i = 0 ; i < iGridCount ; ++i )
	{
		HideContentWindow( i );
	}

	switch( pUISubMenu->m_eMenuType )
	{
		case SCV_TREE:	UpdateContentTree( m_iCurSubMenu , iCurContentPage );		break;
		case SCV_LIST:	UpdateContentList( m_iCurSubMenu , iCurContentPage );		break;
		case SCV_CARD:	UpdateContentCard( m_iCurSubMenu , iCurContentPage );		break;
		case SCV_ITEM:	UpdateContentItem( m_iCurSubMenu , iCurContentPage );		break;
	}
}

void SPWindowSkill::UpdateContentTree( int iCurSubMenu , int iCurContentPage )
{
	int				iContentKey;
	UI_CONTENT*		pUIContent		=	NULL;

	int	iGridCount		=	SKILL_GRID_X_COUNT * SKILL_GRID_Y_COUNT;
	int	iGridStart		=	iCurContentPage * SKILL_GRID_X_COUNT;
	int iGridIndex;

	SPSkillManager*		pSkillManager		=	SPSkillManager::GetInstance();
	SPSkill*			pSkill				=	NULL;
	SKILL_UI_INFO*		pUIInfo				=	NULL;
	int					iCoolTimeID			=	-1;
	bool				bQuickSlotUse		=	false;
	int					iDisableIcon		=	-1;
	int					iDisableIconIndex	=	-1;
	int					iDisableAlpha		=	-1;

	for( int i = 0 ; i < iGridCount ; ++i )
	{
		iGridIndex	=	iGridStart + i;

		iContentKey	=	GetContentKey( iCurSubMenu , iGridIndex );
		pUIContent	=	GetContent( iContentKey );

		if( pUIContent == NULL )
			continue;

		if( pUIContent->m_iGridIndex != iGridIndex )
			continue;

		SetContentWindow( i , pUIContent->m_iSkillID , true , true );
		SetContentImageWindow( i , pUIContent->m_dwImageSetting );

		iCoolTimeID			=	-1;
		bQuickSlotUse		=	false;
		iDisableIcon		=	-1;
		iDisableIconIndex	=	-1;
		iDisableAlpha		=	-1;

		pSkill = pSkillManager->GetSkill( pUIContent->m_iSkillID );

		if( pSkill != NULL )
		{
			iCoolTimeID	=	pSkill->GetCoolTimeID();
			pUIInfo		=	pSkill->GetUIInfo();

			if( pUIInfo != NULL )
			{
				bQuickSlotUse		=	pUIInfo->bQuickSlotUse;
				iDisableIcon		=	pUIInfo->iDisableIcon;
				iDisableIconIndex	=	pUIInfo->iDisableIconIndex;
				iDisableAlpha		=	pUIInfo->iDisableAlpha;
			}
		}

		SetSkillUIInfo( pUIContent->m_iSkillID , i , iCoolTimeID , bQuickSlotUse , iDisableIcon , iDisableIconIndex , iDisableAlpha , -1 , -1 , -1 );
	}
}

void SPWindowSkill::UpdateContentList( int iCurSubMenu , int iCurContentPage )
{
	SKILL_INVENTORY_TYPE	eType	=	(SKILL_INVENTORY_TYPE)( iCurSubMenu	/	SKILL_SUB_SEPARATION );

	int	iGridCount		=	SKILL_GRID_X_COUNT * SKILL_GRID_Y_COUNT;
	int	iGridStart		=	iCurContentPage * SKILL_GRID_X_COUNT;
	int	iSkillCount		=	0;

	if( eType == SIT_ACTION )
	{
		SPSkillManager*		pSkillManager		=	SPSkillManager::GetInstance();
		SPActionInterface*	pActionInterface	=	pSkillManager->GetFirstActionInterface();
		SKILL_UI_INFO*		pSkillUIInfo		=	NULL;

		//	List 전체 스킬 갯수를 구한다.
		while( pActionInterface != NULL )
		{
			pSkillUIInfo		=	pActionInterface->GetUIInfo();
			pActionInterface	=	pSkillManager->GetNextActionInterface();

			if( pSkillUIInfo == NULL )
				continue;

			if( pSkillUIInfo->iSubID != iCurSubMenu )
				continue;

			++iSkillCount;
		}

		pActionInterface	=	pSkillManager->GetFirstActionInterface();

		for( int i = 0 ; i < iGridStart ; ++i )
		{
			if( pActionInterface == NULL )
				break;

			pSkillUIInfo		=	pActionInterface->GetUIInfo();

			if( pSkillUIInfo == NULL )
				break;

			if( pSkillUIInfo->iSubID != iCurSubMenu )
				--i;

			pActionInterface	=	pSkillManager->GetNextActionInterface();
		}

		for( i = 0 ; i < iGridCount ; ++i )
		{
			if( pActionInterface == NULL )
				break;

			pSkillUIInfo		=	pActionInterface->GetUIInfo();

			if( pSkillUIInfo == NULL )
				break;

			if( pSkillUIInfo->iSubID != iCurSubMenu )
			{
				--i;
				pActionInterface	=	pSkillManager->GetNextActionInterface();
				continue;
			}

			SetContentWindow( i , pActionInterface->GetActionID() , true , true );

			SetSkillUIInfo( pActionInterface->GetActionID() , i , -1 , pSkillUIInfo->bQuickSlotUse , -1 , -1 , -1 , -1 , -1 , -1 );

			pActionInterface	=	pSkillManager->GetNextActionInterface();
		}
	}
	else
	{
		SPPlayer*			pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer == NULL )
			return;

		SPSkillArchive*		pSkillArchive	=	pLocalPlayer->GetSkillArchive();

		if( pSkillArchive == NULL )
			return;

		SPSkillActivity*	pSkillActivity	=	pSkillArchive->GetFirstSkillActivity( eType );
		SPSkill*			pSkill			=	NULL;
		SKILL_UI_INFO*		pSkillUIInfo	=	NULL;

		//	List 전체 스킬 갯수를 구한다.
		while( pSkillActivity != NULL )
		{
			pSkill			=	pSkillActivity->GetSkill();
			pSkillActivity	=	pSkillArchive->GetNextSkillActivity( eType );

			if( pSkill == NULL )
				continue;

			pSkillUIInfo = pSkill->GetUIInfo();

			if( pSkillUIInfo == NULL )
				continue;

			if( pSkillUIInfo->iSubID != iCurSubMenu )
				continue;

			++iSkillCount;
		}

		pSkillActivity	=	pSkillArchive->GetFirstSkillActivity( eType );

		for( int i = 0 ; i < iGridStart ; ++i )
		{
			if( pSkillActivity == NULL )
				break;

			pSkill = pSkillActivity->GetSkill();

			if( pSkill == NULL )
				break;

			pSkillUIInfo = pSkill->GetUIInfo();

			if( pSkillUIInfo == NULL )
				break;

			if( pSkillUIInfo->iSubID != iCurSubMenu )
				--i;

			pSkillActivity	=	pSkillArchive->GetNextSkillActivity( eType );
		}

		for( i = 0 ; i < iGridCount ; ++i )
		{
			if( pSkillActivity == NULL )
				break;

			pSkill			=	pSkillActivity->GetSkill();

			if( pSkill == NULL )
				break;

			pSkillUIInfo	=	pSkill->GetUIInfo();

			if( pSkillUIInfo == NULL )
				break;

			pSkillActivity	=	pSkillArchive->GetNextSkillActivity( eType );

			if( pSkillUIInfo->iSubID != iCurSubMenu )
			{
				--i;
				continue;
			}

			SetContentWindow( i , pSkill->GetID() , true , true );

			SetSkillUIInfo( pSkill->GetID() , i , pSkill->GetCoolTimeID() , pSkillUIInfo->bQuickSlotUse , -1 , -1 , -1 , -1 , -1 , -1 );
		}
	}

	SPWindow*	pWindow	=	Find( WIID_SKILL_SCROLL_BAR , true );

	if( pWindow != NULL )
	{
		int	iSkillPage	=	iSkillCount / SKILL_GRID_X_COUNT - SKILL_GRID_Y_COUNT + 1;

		if( iSkillCount % SKILL_GRID_X_COUNT > 0 )
		{
			++iSkillPage;
		}

		if( iSkillPage <= 0 )
			iSkillPage	=	0;

		pWindow->SPSendMessage( SPIM_SET_MAXVALUE , iSkillPage );
	}
}

void SPWindowSkill::UpdateContentCard( int iCurSubMenu , int iCurContentPage )
{
	SKILL_INVENTORY_TYPE	eType	=	(SKILL_INVENTORY_TYPE)( iCurSubMenu	/	SKILL_SUB_SEPARATION );

	int	iGridCount		=	SKILL_GRID_X_COUNT * SKILL_GRID_Y_COUNT;
	int	iGridStart		=	iCurContentPage * SKILL_GRID_X_COUNT;
	int iGridIndex;

	SPSkillManager*		pSkillManager	=	SPSkillManager::GetInstance();
	
	SPWindow*			pWindow			=	Find( WIID_SKILL_SCROLL_BAR , true );

	if( pWindow != NULL )
	{
		pWindow->SPSendMessage( SPIM_SET_MAXVALUE , pSkillManager->GetSkillSubIDPage( iCurSubMenu , eType ) );
	}

	SPSkill*			pSkill			=	NULL;
	SKILL_UI_INFO*		pSkillUIInfo	=	NULL;
	int					iCoolTimeID		=	-1;
	SPUISkillUnit*		pUISkillUnit	=	NULL;

	for( int i = 0 ; i < iGridCount ; ++i )
	{
		iGridIndex	=	iGridStart + i;

		pSkillUIInfo	=	pSkillManager->GetSkillSubID( iCurSubMenu , iGridIndex , eType );

		if( pSkillUIInfo == NULL )
			break;

		SetContentWindow( i , pSkillUIInfo->iID , true , false );

		if( eType == SIT_ACTION )
			iCoolTimeID	=	-1;
		else
		{
			pSkill = pSkillManager->GetSkill( pSkillUIInfo->iID );

			if( pSkill != NULL )
				iCoolTimeID = pSkill->GetCoolTimeID();
		}

		SetSkillUIInfo( pSkillUIInfo->iID , i , iCoolTimeID , pSkillUIInfo->bQuickSlotUse , pSkillUIInfo->iDisableIcon , pSkillUIInfo->iDisableIconIndex , pSkillUIInfo->iDisableAlpha , -1 , -1 , -1 );
	}
}

void SPWindowSkill::UpdateContentItem( int iCurSubMenu , int iCurContentPage )
{
	SPPlayer*			pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPSkillArchive*		pSkillArchive	=	pLocalPlayer->GetSkillArchive();

	if( pSkillArchive == NULL )
		return;

	int	iGridCount		=	SKILL_GRID_X_COUNT * SKILL_GRID_Y_COUNT;
	int	iGridStart		=	iCurContentPage * SKILL_GRID_X_COUNT;
	int	iSkillCount		=	pSkillArchive->GetItemSkillCount();

	SPSkill*				pSkill				=	NULL;
	SKILL_UI_INFO*			pSkillUIInfo		=	NULL;
	SPItemSkillActivity*	pItemSkillActivity	=	NULL;

	for( int i = 0 ; i < iGridCount ; ++i )
	{
		pItemSkillActivity	=	pSkillArchive->GetItemSkillActivity( iGridStart + i );

		if( pItemSkillActivity == NULL )
			break;

		pSkill			=	pItemSkillActivity->GetSkill();

		if( pSkill == NULL )
			break;

		pSkillUIInfo	=	pSkill->GetUIInfo();

		if( pSkillUIInfo == NULL )
			break;

		//if( pSkillUIInfo->iSubID != iCurSubMenu )
		//{
		//	--i;
		//	continue;
		//}

		SetContentWindow( i , pSkill->GetID() , true , false );

		SetSkillUIInfo( pSkill->GetID() , i , pSkill->GetCoolTimeID() , pSkillUIInfo->bQuickSlotUse , -1 , -1 , -1 , pItemSkillActivity->GetItemID() , pItemSkillActivity->GetContainerType() ,  pItemSkillActivity->GetSlotIndex() );
	}

	SPWindow*	pWindow	=	Find( WIID_SKILL_SCROLL_BAR , true );

	if( pWindow != NULL )
	{
		int	iSkillPage	=	iSkillCount / SKILL_GRID_X_COUNT - SKILL_GRID_Y_COUNT + 1;

		if( iSkillCount % SKILL_GRID_X_COUNT > 0 )
		{
			++iSkillPage;
		}

		if( iSkillPage <= 0 )
			iSkillPage	=	0;

		pWindow->SPSendMessage( SPIM_SET_MAXVALUE , iSkillPage );
	}
}

//////////////////////////////////////////////////////////////////////////
//
//	MainMenu	Interface
//

void SPWindowSkill::AddMainMenu( int iKey , UI_MAIN_MENU& stMainMenuData )
{
	m_mmUIMainMenu.insert( STD_MULTIMAP_UI_MAIN_MENU::value_type( iKey , stMainMenuData ) );
}

int SPWindowSkill::GetMainMenuCount()
{
	return (int)m_mmUIMainMenu.size();
}

int SPWindowSkill::GetMainMenuCount( int iKey )
{
	return (int)m_mmUIMainMenu.count( iKey );
}

int SPWindowSkill::GetMainMenuSubCount( int iKey , int iNumber )
{
	UI_MAIN_MENU*	pUIMainMenu	=	GetMainMenu( iKey , iNumber );
	int				iSubCount	=	0;

	if( pUIMainMenu != NULL )
	{
		iSubCount	=	(int)pUIMainMenu->m_vUISubID.size();
	}

	return iSubCount;
}

SPWindowSkill::UI_MAIN_MENU* SPWindowSkill::GetMainMenu( int iKey , int iNumber )
{
	STD_MULTIMAP_UI_MAIN_MENU::iterator	mmIter		=	m_mmUIMainMenu.find( iKey );
	UI_MAIN_MENU*						pUIMainMenu	=	NULL;
	int									iItemID		=	iKey * SKILL_CLASS_SEPARATION + iNumber + 1;

	while( mmIter != m_mmUIMainMenu.end() )
	{
		if( (*mmIter).first == iKey )
		{
			pUIMainMenu	=	&( (*mmIter).second );

			if( pUIMainMenu != NULL )
			{
				if( pUIMainMenu->m_iItemID == iItemID )
					return pUIMainMenu;
			}
		}

		if( (*mmIter).first != iKey )
		{
			break;
		}

		++mmIter;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//
//	SubMenu	Interface
//

void SPWindowSkill::AddSubMenu( int iKey , UI_SUB_MENU& stSubMenuData )
{
	if( GetSubMenu( iKey ) == NULL )
	{
		m_mUISubMenu.insert( STD_MAP_UI_SUB_MENU::value_type( iKey , stSubMenuData ) );
	}
	else
	{
#ifdef _SKILL_DEBUG_PRINT
		DXUTOutputDebugString( "-----> [ %d Overlapping ] [%s] \n" , iKey , RES_FNAME_LDT_SKILL_SUB_MENU );
#endif
	}
}

int SPWindowSkill::GetSubMenuCount()
{
	return	(int)m_mUISubMenu.size();
}

SPWindowSkill::UI_SUB_MENU* SPWindowSkill::GetSubMenu( int iKey )
{
	STD_MAP_UI_SUB_MENU::iterator	mIter		=	m_mUISubMenu.find( iKey );
	UI_SUB_MENU*					pUISubMenu	=	NULL;

	if( mIter != m_mUISubMenu.end() )
	{
		pUISubMenu	=	&( (*mIter).second );
	}

	return pUISubMenu;
}

//////////////////////////////////////////////////////////////////////////
//
//	Content	Interface
//

void SPWindowSkill::SetSubMenuPage( int iKey , int iPage )
{
	int*	pPage	=	GetSubMenuPage( iKey );

	if( pPage == NULL )
	{
		m_mCurSubMenuPage.insert( STD_MAP_UI_CONTENT_PAGE::value_type( iKey , iPage ) );
	}
	else
	{
		*pPage	=	iPage;
	}
}

int* SPWindowSkill::GetSubMenuPage( int iKey )
{
	STD_MAP_UI_CONTENT_PAGE::iterator	mIter	=	m_mCurSubMenuPage.find( iKey );
	int*	pPage	=	NULL;

	if( mIter != m_mCurSubMenuPage.end() )
	{
		pPage	=	&( (*mIter).second );
	}

	return pPage;
}

void SPWindowSkill::DelSubMenuPage()
{
	m_mCurSubMenuPage.clear();
}

//////////////////////////////////////////////////////////////////////////
//
//	Content	Interface
//

int SPWindowSkill::GetContentKey( int iSubID , int iGridIndex )
{
	return	iSubID * 10000 + iGridIndex;
}

int SPWindowSkill::GetContentKeyToSub( int iKey )
{
	return iKey / 10000;
}

void SPWindowSkill::AddContent( int iKey , UI_CONTENT& stContent )
{
	int					iSubID				=	GetContentKeyToSub( iKey );
	UI_CONTENT_BUNDLE*	pUIContentBundle	=	GetContentBundle( iSubID );

	int					iEndPage			=	( stContent.m_iGridIndex + 1 ) / SKILL_GRID_X_COUNT - SKILL_GRID_Y_COUNT + 1;

	if( ( stContent.m_iGridIndex + 1 ) % SKILL_GRID_X_COUNT > 0 )
		++iEndPage;

	if( iEndPage <= 0 )
		iEndPage = 0;

	if( pUIContentBundle == NULL )
	{
		UI_CONTENT_BUNDLE	stUIContentBundle;

		stUIContentBundle.m_iEndPage	=	iEndPage;

		if( GetContent( iKey ) == NULL )
		{
			stUIContentBundle.m_mUIContent.insert( STD_MAP_UI_CONTENT::value_type( iKey , stContent ) );
		}
		else
		{
#ifdef _SKILL_DEBUG_PRINT
			if( iKey != 0 )
				DXUTOutputDebugString( "-----> [ %d Overlapping ] [%s] \n" , iKey , RES_FNAME_LDT_SKILL_CONTENT );
#endif
		}

		m_mUIContent.insert( STD_MAP_UI_CONTENT_BUNDLE::value_type( iSubID , stUIContentBundle ) );
	}
	else
	{
		if( iEndPage > pUIContentBundle->m_iEndPage )
			pUIContentBundle->m_iEndPage	=	iEndPage;

		if( GetContent( iKey ) == NULL )
		{
			pUIContentBundle->m_mUIContent.insert( STD_MAP_UI_CONTENT::value_type( iKey , stContent ) );
		}
		else
		{
#ifdef _SKILL_DEBUG_PRINT
			if( iKey != 0 )
				DXUTOutputDebugString( "-----> [ %d Overlapping ] [%s] \n" , iKey , RES_FNAME_LDT_SKILL_CONTENT );
#endif
		}
	}
}

int SPWindowSkill::GetContentBundleCount()
{
	return (int)m_mUIContent.size();
}

int SPWindowSkill::GetContentEndPage( int iSubID )
{
	int					iEndPage	=	-1;
	UI_CONTENT_BUNDLE*	pUIContentBundle	=	GetContentBundle( iSubID );

	if( pUIContentBundle == NULL )
		return iEndPage;

	iEndPage	=	pUIContentBundle->m_iEndPage;

	return iEndPage;
}

int SPWindowSkill::GetContentCount( int iSubID )
{
	int					iCount	=	-1;
	UI_CONTENT_BUNDLE*	pUIContentBundle	=	GetContentBundle( iSubID );

	if( pUIContentBundle == NULL )
		return iCount;

	iCount	=	(int)( pUIContentBundle->m_mUIContent.size() );

	return iCount;
}

SPWindowSkill::UI_CONTENT_BUNDLE* SPWindowSkill::GetContentBundle( int iSubID )
{
	STD_MAP_UI_CONTENT_BUNDLE::iterator	mIter				=	m_mUIContent.find( iSubID );
	UI_CONTENT_BUNDLE*					pUIContentBundle	=	NULL;

	if( mIter != m_mUIContent.end() )
	{
		pUIContentBundle	=	&( (*mIter).second );
	}

	return pUIContentBundle;
}

SPWindowSkill::UI_CONTENT* SPWindowSkill::GetContent( int iKey )
{
	int					iSubID				=	GetContentKeyToSub( iKey );
	UI_CONTENT*			pUIContent			=	NULL;
	UI_CONTENT_BUNDLE*	pUIContentBundle	=	GetContentBundle( iSubID );

	if( pUIContentBundle == NULL )
		return pUIContent;

	STD_MAP_UI_CONTENT::iterator	mIter		=	pUIContentBundle->m_mUIContent.find( iKey );

	if( mIter != pUIContentBundle->m_mUIContent.end() )
	{
		pUIContent	=	&( (*mIter).second );
	}

	return pUIContent;
}

//////////////////////////////////////////////////////////////////////////
//
//	Content Page Interface
//

int SPWindowSkill::GetContentPageKey( int iMainID , int iSubID )
{
	return	iMainID * 10000 + iSubID;
}

void SPWindowSkill::SetContentPage( int iKey , int iPage )
{
	int*	pPage	=	GetContentPage( iKey );

	if( pPage == NULL )
	{
		m_mCurContentPage.insert( STD_MAP_UI_CONTENT_PAGE::value_type( iKey , iPage ) );
	}
	else
	{
		*pPage	=	iPage;
	}
}

int* SPWindowSkill::GetContentPage( int iKey )
{
	STD_MAP_UI_CONTENT_PAGE::iterator	mIter	=	m_mCurContentPage.find( iKey );
	int*	pPage	=	NULL;

	if( mIter != m_mCurContentPage.end() )
	{
		pPage	=	&( (*mIter).second );
	}

	return pPage;
}

void SPWindowSkill::DelContentPage()
{
	m_mCurContentPage.clear();
}

//////////////////////////////////////////////////////////////////////////

void SPWindowSkill::DataReset()
{
	m_iCurClassType		=	CLASS_NULL;
	m_pCurMainMenu		=	NULL;
	m_iCurSubMenu		=	0;
	DelSubMenuPage();
	DelContentPage();

	m_bLMouseDown		=	FALSE;
}

void SPWindowSkill::ReposOwnImage()
{
	SetRect( &m_rcBGDest[0]	, m_iAX					, m_iAY					, m_iAX + m_iSX			, m_iAY + 4 );						//	배경 상단
	SetRect( &m_rcBGDest[1]	, m_rcBGDest[0].left	, m_rcBGDest[0].bottom	, m_rcBGDest[0].right	, m_rcBGDest[0].bottom + 337 );		//	배경 중단
	SetRect( &m_rcBGDest[2]	, m_rcBGDest[1].left	, m_rcBGDest[1].bottom	, m_rcBGDest[1].right	, m_rcBGDest[1].bottom + 4 );		//	배경 하단
	SetRect( &m_rcBGDest[3]	, m_iAX + 2	, m_iAY + 26 , m_iAX + 2 + 256 , m_iAY + 26 + 34 );												//	옅은 초록 서브 백그라운드
	SetRect( &m_rcBGDest[4]	, m_iAX + 2	, m_iAY + 294 , m_iAX + 2 + 256 , m_iAY + 294 + 25 );											//	옅은 초록 서브 백그라운드 2
	SetRect( &m_rcBGDest[5]	, m_iAX + 2	, m_iAY + 61 , m_iAX + 2 + 256 , m_iAY + 61 + 232 );											//	중앙 흰색 서브 백그라운드
	SetRect( &m_rcBGDest[6]	, m_iAX + 2	, m_iAY + 60 , m_iAX + 2 + 256 , m_iAY + 60 + 1 );												//	경계선 1
	SetRect( &m_rcBGDest[7]	, m_iAX + 2	, m_iAY + 293 , m_iAX + 2 + 256 , m_iAY + 293 + 1 );											//	경계선 2
	SetRect( &m_rcBGDest[8]	, m_iAX + 2	, m_iAY + 319 , m_iAX + 2 + 256 , m_iAY + 319 + 1 );											//	경계선 3

	SPUIUnit*	pUIUnit;
	int			iUnitNumber	=	0;

	for( int y = 0 ; y < SKILL_GRID_Y_COUNT ; ++y )
	{
		for( int x = 0 ; x < SKILL_GRID_X_COUNT ; ++x )
		{
			pUIUnit	=	m_UIUnitManager.GetUnit( iUnitNumber );
			
			++iUnitNumber;

			if( pUIUnit == NULL )
				continue;

			pUIUnit->SetPos(	m_iAX + SKILL_GRID_X_OFFSET + ( x * SKILL_GRID_X_SIZE ) + SKILL_UNIT_X_OFFSET ,
								m_iAY + SKILL_GRID_Y_OFFSET + ( y * SKILL_GRID_Y_SIZE ) + SKILL_UNIT_Y_OFFSET );
		}
	}

	m_UIUnitManager.ReCalRect();
}

void SPWindowSkill::HideContentWindow( int iPos )
{
	if( iPos < 0 || (int)m_vUIContentWindow.size() <= iPos )
		return;

	UI_CONTENT_WINDOW&	stUIContentWindow	=	m_vUIContentWindow.at( iPos );

	for( int i = 0 ; i < SCL_COUNT ; ++i )
	{
		if( stUIContentWindow.m_pContent[ i ] != NULL )
			stUIContentWindow.m_pContent[ i ]->Hide();
	}

	for( i = 0 ; i < SCGV_COUNT ; ++i )
	{
		if( stUIContentWindow.m_pImages[ i ] != NULL )
			stUIContentWindow.m_pImages[ i ]->Hide();
	}

	SPUIUnit*	pUIUnit	=	m_UIUnitManager.GetUnit( iPos );

	if( pUIUnit != NULL )
		pUIUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
}

void SPWindowSkill::SetContentWindow( int iPos , int iSkillID , bool bShow , bool bLearnShow )
{
	if( iPos < 0 || (int)m_vUIContentWindow.size() <= iPos )
		return;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	UI_CONTENT_WINDOW&	stUIContentWindow	=	m_vUIContentWindow.at( iPos );

	for( int i = 0 ; i < SCL_COUNT ; ++i )
	{
		if( stUIContentWindow.m_pContent[ i ] != NULL )
			stUIContentWindow.m_pContent[ i ]->Hide();
	}

	if( iSkillID == 0 )
		return;

	SPSkillManager*		pSkillManager		=	SPSkillManager::GetInstance();
	SPActionInterface*	pActionInterface	=	pSkillManager->GetActionInterface( iSkillID );

	if( pActionInterface != NULL )
	{
		stUIContentWindow.m_pContent[ SCL_BG ]->Show();
		stUIContentWindow.m_pContent[ SCL_BG ]->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );

		return;
	}

	SPSkillArchive*		pSkillArchive	=	pLocalPlayer->GetSkillArchive();

	if( pSkillArchive == NULL )
		return;
	
	SPSkillActivity*	pSkillActivity	=	pSkillArchive->GetSkillActivity( iSkillID );

	if( pSkillActivity == NULL && bShow == false )
		return;

	if( pSkillActivity != NULL )
	{
		SPSkill*	pSkill	=	pSkillActivity->GetSkill();

		if( pSkill == NULL )
			return;

		int	iCurLevel	=	pSkillActivity->GetLevel();

		if( iCurLevel > 99 )
			iCurLevel = 99;

		int	iCurLevel10	=	iCurLevel / 10;
		int	iCurLevel1	=	iCurLevel % 10;

		if( iCurLevel10 > 0 )
		{
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]->Show();
			stUIContentWindow.m_pContent[ SCL_CUR_LEVEL10 ]->SPSendMessage( SPIM_SET_IMAGEINDEX , iCurLevel10 );
		}

		stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]->Show();
		stUIContentWindow.m_pContent[ SCL_CUR_LEVEL1 ]->SPSendMessage( SPIM_SET_IMAGEINDEX , iCurLevel1 );

		stUIContentWindow.m_pContent[ SCL_LEVEL_SLASH ]->Show();

		int	iMaxLevel	=	pSkill->GetMaxLevel();

		if( iMaxLevel > 99 )
			iMaxLevel = 99;

		int	iMaxLevel10	=	iMaxLevel / 10;
		int	iMaxLevel1	=	iMaxLevel % 10;

		if( iMaxLevel10 > 0 )
		{
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]->Show();
			stUIContentWindow.m_pContent[ SCL_MAX_LEVEL10 ]->SPSendMessage( SPIM_SET_IMAGEINDEX , iMaxLevel10 );
		}

		stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]->Show();
		stUIContentWindow.m_pContent[ SCL_MAX_LEVEL1 ]->SPSendMessage( SPIM_SET_IMAGEINDEX , iMaxLevel1 );

		if( iCurLevel < iMaxLevel )
		{
			bool	bLevelUpEnable	=	true;

			bLevelUpEnable	=	g_pCheckManager->CheckSkillUpRequire( iSkillID );

			stUIContentWindow.m_pContent[ SCL_LEVEL_UP ]->Show();
			stUIContentWindow.m_pContent[ SCL_LEVEL_UP ]->SetEnable( bLevelUpEnable );
		}
	}
	else
	{
		if( bLearnShow == true )
			stUIContentWindow.m_pContent[ SCL_NOT_LEARN ]->Show();
	}

	stUIContentWindow.m_pContent[ SCL_BG ]->Show();
	stUIContentWindow.m_pContent[ SCL_BG ]->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );
}

void SPWindowSkill::SetContentImageWindow( int iPos , DWORD dwImageSetting )
{
	if( iPos < 0 || (int)m_vUIContentWindow.size() <= iPos )
		return;

	UI_CONTENT_WINDOW&	stUIContentWindow	=	m_vUIContentWindow.at( iPos );

	int	iKey;

	for( int i = 0 ; i < SCGV_COUNT ; ++i )
	{
		iKey	=	1 << i;

		if( ( iKey & dwImageSetting ) > 0 )
		{
			if( stUIContentWindow.m_pImages[ i ] != NULL )
				stUIContentWindow.m_pImages[ i ]->Show();
		}
		else
		{
			if( stUIContentWindow.m_pImages[ i ] != NULL )
				stUIContentWindow.m_pImages[ i ]->Hide();
		}
	}
}

void SPWindowSkill::SetSkillPoints()
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPGOBStatus*	pGOBStatus	=	pLocalPlayer->GetStatus();

	if( pGOBStatus == NULL )
		return;

	int iSkillPoint = pGOBStatus->GetStatusValue( STATUS_SKILL_POINT );

	SPWindow*	pWindow	=	Find( WIID_SKILL_POINT_TEXT , true );

	if( pWindow != NULL )
	{
		char	szBuf[_MAX_PATH]	=	{};

		sprintf( szBuf , "%d" , iSkillPoint );

		pWindow->SetWindowText( szBuf );
	}
}

void SPWindowSkill::SetSkillUIInfo( int iID , int iUIUnitPos , int iCoolTimeID , bool bQuickSlotUse , int iDisableIcon , int iDisableIconIndex , int iDisableAlpha , int iItemID , int iContainerType , int iSlotIndex )
{
	int iUIUnitCount = m_UIUnitManager.GetUnitCount();

	if( iUIUnitPos < 0 || iUIUnitCount <= iUIUnitPos )
		return;

	SPUISkillUnit*	pUISkillUnit	=	(SPUISkillUnit*)( m_UIUnitManager.GetUnit( iUIUnitPos ) );

	if( pUISkillUnit == NULL )
		return;

	bool		bLearn			=	false;
	bool		bToggle			=	false;
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		SPSkillArchive*	pSkillArchive = pLocalPlayer->GetSkillArchive();

		if( pSkillArchive != NULL )
		{
			SPSkillActivity*	pSkillActivity = pSkillArchive->GetSkillActivity( iID );

			if( pSkillActivity != NULL )
			{
				bLearn	=	true;
				bToggle	=	pSkillActivity->GetToggleOn();
			}
		}
	}

	pUISkillUnit->SetStatus( SPUIUnit::STATUS_UI_UNIT_RENDER | SPUIUnit::STATUS_UI_UNIT_COOLTIME );

	if( pUISkillUnit->SetResource( iDisableIcon , iDisableIconIndex ) == FALSE || bLearn == true )
	{
		pUISkillUnit->SetDisableTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" );
		pUISkillUnit->SetDisableRectSrc( 434 , 37 , 436 , 39 );
	}
	else
	{
		pUISkillUnit->SetDisableTexture( pUISkillUnit->GetTexture() );
		pUISkillUnit->SetDisableRectSrc( pUISkillUnit->GetRectSrc() );
		pUISkillUnit->SetDisableColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , (float)( iDisableAlpha / 100.0f ) ) );
	}

	pUISkillUnit->SetSkillID( iID );
	pUISkillUnit->SetType( SIT_NULL );
	if( pUISkillUnit->ReResourceLoad() == FALSE )
	{
		pUISkillUnit->SetStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
		return;
	}

	if( iItemID > 0 && iContainerType >= 0 && iSlotIndex >= 0 )
	{
		CONTAINER_SLOT	stContainerSlot;

		stContainerSlot.ContainerType	=	(CONTAINER_TYPE)iContainerType;
		stContainerSlot.SlotIndex		=	(SLOT_INDEX)iSlotIndex;

		pUISkillUnit->SetItemID( iItemID );
		pUISkillUnit->SetContainerSlot( stContainerSlot );
		pUISkillUnit->SetType( SIT_PET );
	}

	pUISkillUnit->SetCoolTimeID( iCoolTimeID );
	pUISkillUnit->SetToggle( bToggle );

	if( bLearn == true )
	{
		if( bQuickSlotUse == true )
			pUISkillUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
		else
			pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
	}
	else
	{
		if( bQuickSlotUse == true && ( pUISkillUnit->GetType() == SIT_ACTION || pUISkillUnit->GetType() == SIT_PET ) )
			pUISkillUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
		else
			pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
	}

	bool	bEnalbe	=	true;

	pUISkillUnit->SetCombo( FALSE );	// 기획팀에서 콤보 이미지 사용하지 않음 [11/7/2007 AJJIYA]

	if( pUISkillUnit->GetType() == SIT_ACTION )
	{
		bEnalbe = g_pCheckManager->CheckActionSkillUseRequire( iID );
	}
	else if( pUISkillUnit->GetType() == SIT_PET )
	{
		bEnalbe = g_pCheckManager->CheckItemSkillUseRequire( iID , iItemID , (CONTAINER_TYPE)iContainerType , iSlotIndex , true );
	}
	else
	{
		bEnalbe = g_pCheckManager->CheckSkillUseRequire( iID , true );
	}

	if( bEnalbe == true )
		pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
	else
		pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DISABLE );
}

void SPWindowSkill::AddLearnSkillLdt( SKILL_LEARN_LDT_DATA& stSkillLearnLDtData )
{
	if( GetLearnSKillLdt( stSkillLearnLDtData.m_iKeyID ) == NULL )
	{
		m_mLearnSkill.insert( STD_MAP_LEARN_LDT_DATA::value_type( stSkillLearnLDtData.m_iKeyID , stSkillLearnLDtData ) );
	}
	else
	{
#ifdef _SKILL_DEBUG_PRINT
		DXUTOutputDebugString( "-----> [ %d Overlapping ] [%s] \n" , stSkillLearnLDtData.m_iKeyID , RES_FNAME_LDT_SKILL_LEARN );
#endif
	}
}

void SPWindowSkill::ClearLearnSkillLdt( SKILL_LEARN_LDT_DATA& stSkillLearnLDtData )
{
	stSkillLearnLDtData.m_iKeyID	=	0;

	for( int iRequire = 0 ; iRequire < LS_REQUIRE_COUNT ; ++iRequire )
	{
		stSkillLearnLDtData.m_stRequireCondition[ iRequire ].m_iRequireType		=	0;
		stSkillLearnLDtData.m_stRequireCondition[ iRequire ].m_iRequireID		=	0;
		stSkillLearnLDtData.m_stRequireCondition[ iRequire ].m_iRequireValue1	=	0;
		stSkillLearnLDtData.m_stRequireCondition[ iRequire ].m_iRequireValue2	=	0;

		stSkillLearnLDtData.m_stRequireCondition[ iRequire ].m_strErrorMsg.clear();
	}

	stSkillLearnLDtData.m_strName.clear();
	stSkillLearnLDtData.m_uiConsumptionEly		=	0;
	stSkillLearnLDtData.m_iConsumptionItemID	=	0;
//	stSkillLearnLDtData.m_iConsumptionItemCount	=	0;

	stSkillLearnLDtData.m_strDesc.clear();
	stSkillLearnLDtData.m_strErrorEly.clear();
	stSkillLearnLDtData.m_strErrorItem.clear();
}

SPWindowSkill::SKILL_LEARN_LDT_DATA* SPWindowSkill::GetLearnSKillLdt( int iSkillID )
{
	STD_MAP_LEARN_LDT_DATA::iterator	mIter	=	m_mLearnSkill.find( iSkillID );
	SKILL_LEARN_LDT_DATA*	pSkillLearnLdtData	=	NULL;

	if( mIter != m_mLearnSkill.end() )
	{
		pSkillLearnLdtData	=	&( (*mIter).second );
	}

	return pSkillLearnLdtData;
}

void SPWindowSkill::SetLearnSkillMsg( int iSkillID )
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPSkillArchive*		pSkillArchive	=	pLocalPlayer->GetSkillArchive();

	if( pSkillArchive == NULL )
		return;

	SPSkillActivity*	pSkillActivity	=	pSkillArchive->GetSkillActivity( iSkillID );

	if( pSkillActivity != NULL )
		return;

	SPSkillManager*		pSkillManager		=	SPSkillManager::GetInstance();

	if( pSkillManager == NULL )
		return;
    
	SPSkill*	pSkill	=	pSkillManager->GetSkill( iSkillID );

	if( pSkill == NULL )
		return;

	if( pSkill->GetLearnSkill() == false )
		return;

	SKILL_LEARN_LDT_DATA*	pSkillLearnLDT	=	GetLearnSKillLdt( iSkillID );

	if( pSkillLearnLDT == NULL )
		return;

	char			szBuf[ _MAX_PATH ];
	const char*		pszGlobalString	=	NULL;
	std::string		strMessage , strMessageLine;

	// 제목
	strMessage	=	pSkillLearnLDT->m_strName;
	strMessage	+=	"\\n";
	strMessage	+=	"\\n";

	// 엘리
	pszGlobalString	=	g_pResourceManager->GetGlobalString( 1800001 );

	if( pszGlobalString != NULL )
	{
		sprintf( szBuf , pszGlobalString , pSkillLearnLDT->m_uiConsumptionEly );
		strMessage	+=	szBuf;
		strMessage	+=	"\\n";
	}

	// 아이템
	pszGlobalString	=	g_pResourceManager->GetGlobalString( 1800002 );

	if( pszGlobalString != NULL )
	{
		SPItemAttr* pConsumptionItem	=	NULL;
		g_pItemCluster->GetItemInfo( pSkillLearnLDT->m_iConsumptionItemID , pConsumptionItem );

		if( pConsumptionItem != NULL )
		{
//			sprintf( szBuf , pszGlobalString , pConsumptionItem->m_strName.c_str() , pSkillLearnLDT->m_iConsumptionItemCount );
			sprintf( szBuf , pszGlobalString , pConsumptionItem->m_strName.c_str() , 1 );
			strMessage	+=	szBuf;
			strMessage	+=	"\\n";
		}
	}

	//	필요 조건
	SKILL_LEARN_REQUIRE*	pSkillLearnRequire	=	NULL;

	for( int i = 0 ; i < LS_REQUIRE_COUNT ; ++i )
	{
		pSkillLearnRequire = &pSkillLearnLDT->m_stRequireCondition[ i ];

		if( pSkillLearnRequire->m_iRequireType == 0 || pSkillLearnRequire->m_iRequireType > 5 )
			continue;

		strMessageLine.clear();

		SetLearnSkillRequire(	strMessageLine , pSkillLearnRequire->m_iRequireType , pSkillLearnRequire->m_iRequireID ,
								pSkillLearnRequire->m_iRequireValue1 , 0 ,pSkillLearnRequire->m_iRequireValue2 , 0 ,
								0 , 1220000 );

		if( strMessageLine.empty() == false )
		{
			strMessage	+=	strMessageLine;
			strMessage	+=	"\\n";
		}
	}

	//	설명
	if( pSkillLearnLDT->m_strDesc.empty() == false )
	{
		strMessage	+=	pSkillLearnLDT->m_strDesc;
	}

	m_iLearnSkillID	=	iSkillID;
	OnContentCursorOut( 0 , NULL , NULL );

	SPNoticeBox*	pNoticeBox	=	(SPNoticeBox*)( g_pInterfaceManager->GetNoticeBox() );

	if( pNoticeBox != NULL )
		pNoticeBox->Notice( strMessage.c_str() , this , NOTICE_TYPE_YESNO , DT_LEFT | DT_VCENTER );
}

void SPWindowSkill::SetLearnSkillRequire( std::string& strMessage , int iType , int iID , int iValue1 , int iValueSlv1 , int iValue2 , int iValueSlv2 , int iLevel , int iGSNumber )
{
	// 툴팁쪽을 기준으로 할것!!!!
	if( iType == 0 )
		return;

	int iRequireGlobalStringNum = iGSNumber + iType * 1000 + iID;
	const char*	pStrFormat		=	g_pResourceManager->GetGlobalString( iRequireGlobalStringNum );

	if( pStrFormat == NULL )
		return;

	int	iRequireState1	=	iValue1	+ iValueSlv1	* iLevel;
	int iRequireState2	=	iValue2	+ iValueSlv2	* iLevel;

	char			szBuf		[_MAX_PATH]	=	{};
	char			szBufLine	[_MAX_PATH]	=	{};
	char			szBufTemp	[_MAX_PATH]	=	{};
	const char*		pTemp;

	//	Stat일경우
	if( iType == 1 )
	{
		//	캐릭터 유형
		if( iID == 1 )
		{
			//	공 방 마 탐 순으로 표시!!
			int			iCompareClassType;

			for( int i = 0 ; i < CLASS_MAX ; i++ )
			{
				iCompareClassType	=	1	<<	i;

				if( ( iRequireState1 & iCompareClassType ) == iCompareClassType )
				{
					pTemp	=	g_pResourceManager->GetGlobalString( 6000019 + i );

					if( pTemp != NULL )
					{
						strcat( szBufLine , pTemp );
						strcat( szBufLine , ". " );
					}
				}
			}

			sprintf( szBuf , pStrFormat , szBufLine );
		}
		//	캐릭터 성별
		else if( iID == 2)
		{
			const char* pstrGender	=	g_pResourceManager->GetGlobalString( 1200000 + iRequireState1 );

			if( pstrGender != NULL )
				sprintf( szBuf , pStrFormat , pstrGender );
		}
		else
		{
			if( iValue1 != 0 || iValueSlv1 != 0 )
			{
				pTemp	=	g_pResourceManager->GetGlobalString( 1200010 );
				if( pTemp != NULL )
				{
					sprintf( szBufTemp , pTemp , iRequireState1 );
					strcat( szBufLine , szBufTemp );
				}
			}

			if( ( iValue1 != 0 || iValueSlv1 != 0 ) &&
				( iValue2 != 0 || iValueSlv2 != 0 ) )
			{
				pTemp	=	g_pResourceManager->GetGlobalString( 1200012 );
				if( pTemp != NULL )
				{
					strcat( szBufLine , pTemp );
				}
			}

			if( iValue2 != 0 || iValueSlv2 != 0 )
			{
				pTemp	=	g_pResourceManager->GetGlobalString( 1200011 );
				if( pTemp != NULL )
				{
					sprintf( szBufTemp , pTemp , iRequireState2 );
					strcat( szBufLine , szBufTemp );
				}
			}

			sprintf( szBuf , pStrFormat , szBufLine );
		}
	}
	//	아이템일경우
	else if( iType == 2 )
	{
		//	갯수체크
		if( iID == 1 )
		{
			int iItemCount	=	g_pCheckManager->GetTotalPCItemCount( iRequireState1 );

			SPItemAttr*	pItem;
			g_pItemCluster->GetItemInfo( iRequireState1 , pItem );

			if( pItem != NULL )
				sprintf( szBuf , pStrFormat , pItem->m_strName.c_str() , iRequireState2 );
		}
	}
	//	스킬일경우
	else if( iType == 3 )
	{
		SPSkill*	pSkill	=	SPSkillManager::GetInstance()->GetSkill( iRequireState1 );

		//	스킬 존재 유무
		if( iID == 1 )
		{
			if( pSkill != NULL )
				sprintf( szBuf , pStrFormat , pSkill->GetName() );
		}
		//	스킬 레벨 체크
		else if( iID == 2 )
		{
			if( pSkill != NULL )
				sprintf( szBuf , pStrFormat , pSkill->GetName() , iRequireState2 );
		}

	}
	//	퀘스트일경우
	else if( iType == 4 )
	{
		//	반복 횟수 이하
		if( iID == 1)
		{
			pTemp	=	g_pResourceManager->GetGlobalString( 1200011 );
			if( pTemp != NULL )
			{
				sprintf( szBufTemp , pTemp , iRequireState2 );
				strcat( szBufLine , szBufTemp );
			}
		}
		//	반복 횟수 초과
		else if( iID == 2)
		{
			pTemp	=	g_pResourceManager->GetGlobalString( 1200010 );
			if( pTemp != NULL )
			{
				sprintf( szBufTemp , pTemp , iRequireState1 );
				strcat( szBufLine , szBufTemp );
			}
		}

		SPQuestAttr*	pQuestAttr		=	new SPQuestAttr;

		if( SPQuestManager::GetInstance()->LoadLDTFile( iRequireState1 , pQuestAttr ) == true )
		{
			sprintf( szBuf , pStrFormat , szBufLine );
			sprintf( szBuf , pStrFormat , pQuestAttr->m_strQuestName.c_str() , iRequireState2 );
		}

		SAFE_DELETE( pQuestAttr );
	}
	//	타이틀일 경우
	else if( iType == 5 )
	{
		if( pStrFormat == NULL )
			return;

		SPTitleManager*	pTitleManager	=	SPTitleManager::GetInstance();

		if( pTitleManager == NULL )
			return;

		SPTitleArchive*	pTitleArchive	=	pTitleManager->GetTitleArchive();

		if( pTitleArchive == NULL )
			return;

		SPTitleAttr*	pTitleAttr	=	pTitleManager->GetTitleAttr( iRequireState1 );

		if( pTitleAttr == NULL )
			return;

		sprintf( szBuf , pStrFormat , pTitleAttr->GetName() );
	}
	else
		return;

	if( strlen( szBuf ) == 0 )
		return;

	strMessage	+=	szBuf;
}
