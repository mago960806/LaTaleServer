// ***************************************************************
//  SPWindowGamble   version:  1.0   ·  date: 10/29/2007
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
#include "SPNoticeBox.h"

#include "SPUIUnit.h"
#include "SPUIItemUnit.h"
#include "SPUIUnitManager.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPItemAttr.h"
#include "SPManager.h"
#include "SPItem.h"
#include "SPItemCluster.h"		//[2005/6/16]

#include "SPEventDEF.h"
#include "SPEventNpcHandler.h"
#include "SPEventManager.h"

#include "SPSubGameManager.h"
#include "SPCommandConvert.h"
#include "SPGOBStatus.h"
#include "SPGOBManager.h"
#include "SPGOBCoordPhysics.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPSoundArchive.h"
#include "SPSoundManager.h"

#include "SPWindowToolTip.h"

#include "SPCheckManager.h"
#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPWindowGamble.h"

SPWindowGamble::SPWindowGamble( WND_ID WndClassID , INSTANCE_ID InstanceID )
: SPWindow( WndClassID , InstanceID )
{
	Init();
}

SPWindowGamble::SPWindowGamble( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent /* = NULL  */ )
: SPWindow( WNDID_GAMBLE , InstanceID , iX , iY , iCX , iCY , pParent )	
{
	Init();
}

SPWindowGamble::~SPWindowGamble()
{
	Clean();
}

void SPWindowGamble::Init()
{
	m_strToolTip		=	"[SPWindowGamble]";

	//////////////////////////////////////////////////////////////////////////

	LoadGambleLDT();
	LoadGambleRewardLDT();

	m_iUnitCount	=	0;

	InitItemUnit();

	//////////////////////////////////////////////////////////////////////////

	m_bUse			=	false;
	m_pFXArchive	=	new SPFXArchive( NULL );

}

void SPWindowGamble::Clean()
{
	SPWindow::Clean();

	SAFE_DELETE( m_pFXArchive );

	m_UIUnitManager.ClearAll( TRUE );
}

void SPWindowGamble::Show( bool bWithChild /* = true  */ )
{
	if( m_bUse == false )
		return;

	if( m_bShow == true )
		g_pInterfaceManager->RegisterSoundUnit( "DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV" );

	SetFocus();

	SPWindow::Show( bWithChild );
}

void SPWindowGamble::Hide( bool bSendServer /* = true */ )
{
	if( m_bShow == true )
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide( bSendServer );

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		SPWindow*	pWindowToolTip	=	g_pInterfaceManager->GetToolTipWindow();

		if( pWindowToolTip != NULL )
			pWindowToolTip->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , -2 );
	}

	if( m_pFXArchive )
		m_pFXArchive->Clean();
}

void SPWindowGamble::Process( float fTime )
{
	if( m_bShow == false )
		return;

	SPWindow::Process( fTime );

	m_UIUnitManager.Process( fTime );

	if( m_pFXArchive != NULL )
		m_pFXArchive->Process( fTime );
}

void SPWindowGamble::Render( float fTime )
{
	if( m_bShow == false )
		return;

	g_pVideo->Flush();

	SPWindow::Render( fTime );

	g_pVideo->Flush();

	m_UIUnitManager.Render();

	g_pVideo->Flush();

	if( m_pFXArchive != NULL )
		m_pFXArchive->Render( fTime );

	g_pVideo->Flush();
}

//void SPWindowLottery::RefreshRelationCoord()
//{
//	SPWindow::RefreshRelationCoord();
//}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowGamble )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER			,										OnCursorEnter		)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT				,										OnCursorOut			)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE			,										OnCursorMove		)

	SPIMESSAGE_COMMAND(	SPIM_PURGE					,										OnPurge				)
	SPIMESSAGE_COMMAND(	SPIM_GAMBLE_USE				,										OnItemUse			)
	SPIMESSAGE_COMMAND(	SPIM_GAMBLE_RESULT			,										OnResult			)
	SPIMESSAGE_COMMAND( SPIM_NOTICE_YES				,										OnCashShopMoveYes	)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_GAMBLE_CLOSE_ICON			,	OnClose				)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowGamble::OnCursorEnter( WPARAM wParam, LPARAM lParam )
{
	OnCursorMove( wParam , lParam );
	return 1;
}

int SPWindowGamble::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, -2 );

	return 1;
}

int SPWindowGamble::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	if( IsCursorIn() == false )
		return 1;

	SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();

	if( pToolTipWindow == NULL )
		return 1;

	int				iXPos			=	LOWORD( lParam );
	int				iYPos			=	HIWORD( lParam );
	SPUIItemUnit*	pUIUnitToolTip	=	(SPUIItemUnit*)m_UIUnitManager.GetUnitMouseOver( iXPos , iYPos );

	return 1;
}

int SPWindowGamble::OnPurge( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowGamble::OnItemUse( WPARAM wParam, LPARAM lParam )
{
	SPItem*		pUseItem		=	(SPItem*)wParam;
	int			iItemID			=	(int)lParam;

	if( pUseItem == NULL || iItemID <= 0 )
		return 1;

	int	iBettingCount	=	1;

	RunGamble( iItemID , iBettingCount , NULL );

	return 1;
}

int SPWindowGamble::OnResult( WPARAM wParam, LPARAM lParam )
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	if( g_pCheckManager != NULL )
		g_pCheckManager->SetDBBlock( false );

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
		PrintErrorMsg( uiGlobalStringID , true , D3DCOLOR_ARGB( 255 , 228 , 3 , 3 ) );
		return 1;
	}

	int		iGambleID		=	0;

	pPacket->ExtractStruct( &iGambleID , sizeof( iGambleID ) );

	LDT_GAMBLE*		pLdtGamble		=	GetGamble( iGambleID );

	if( pLdtGamble == NULL )
		return 1;

	UINT8	iBettingCount	=	0;

	pPacket->ExtractUINT8( &iBettingCount );

	int							i					=	0;
	int							iGambleRewardID		=	0;
	STD_VECTOR_GAMBLE_REWARD	vGambleRewardBuffer;

	vGambleRewardBuffer.reserve( iBettingCount );

	for( i = 0 ; i < iBettingCount ; ++i )
	{
		pPacket->ExtractStruct( &iGambleRewardID , sizeof( iGambleRewardID ) );

		vGambleRewardBuffer.push_back( iGambleRewardID );
	}

	UINT8				iAutoSell					=	0;
	int					iGambleRewardBufferCount	=	(int)vGambleRewardBuffer.size();
	LDT_GAMBLE_REWARD*	pLdtGambleReward			=	NULL;

	for( i = 0 ; i < iBettingCount ; ++i )
	{
		pPacket->ExtractUINT8( &iAutoSell );

		if( iGambleRewardBufferCount > i )
			iGambleRewardID	=	vGambleRewardBuffer.at( i );
		else
			iGambleRewardID	=	0;

		pLdtGambleReward	=	GetGambleReward( iGambleRewardID );

		if( pLdtGambleReward == NULL )
			continue;

		if( iAutoSell > 0 )
			PrintErrorMsg( pLdtGambleReward->m_strMessage_Ely.c_str() , true , D3DCOLOR_ARGB( 255 , 228 , 3 , 3 ) );
		else
			PrintErrorMsg( pLdtGambleReward->m_strMessage_Item.c_str() , true , D3DCOLOR_ARGB( 255 , 228 , 3 , 3 ) );
	}

	UINT8			iNumOfItem		=	0;
	CONTAINER_ITEM	tItem;

	pPacket->ExtractUINT8( &iNumOfItem );

	SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();

	for( i = 0 ; i < iNumOfItem ; ++i )
	{
		pPacket->ExtractStruct( &tItem , sizeof( CONTAINER_ITEM ) );

		--tItem.SlotInfo.SlotIndex;

		if( pInvenArchive != NULL )
			pInvenArchive->ItemSetting( tItem , false );
	}

	UINT64	iPCCurrEly		=	0;

	pPacket->ExtractUINT64( &iPCCurrEly );

	SPGOBStatus*			pPlayerStatus	=	pLocalPlayer->GetStatus();

	if( pPlayerStatus != NULL )
		pPlayerStatus->SetStatusValueEX( STATUS_EX_ELY , iPCCurrEly );

	UINT8						iNumOfTrashItem	=	0;
	TRASH_ITEM					TrashItem;

	pPacket->ExtractUINT8( &iNumOfTrashItem );

	for( i = 0 ; i < iNumOfTrashItem ; ++i )
	{
		pPacket->ExtractStruct( &TrashItem , sizeof( TRASH_ITEM ) );

		--TrashItem.iContainerSlot.SlotIndex;

		if( pInvenArchive != NULL )
		{
			pInvenArchive->RunCoolTime( TrashItem );
			pInvenArchive->TrashItem( TrashItem );
		}
	}

	return 1;
}

int SPWindowGamble::OnCashShopMoveYes( WPARAM wParam, LPARAM lParam )
{
	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_MENU , SPIM_CASHSHOP_ENTER , NULL , NULL );

	return 1;
}

int SPWindowGamble::OnClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

void SPWindowGamble::LoadGambleLDT()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_GAMBLE , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [ %s ] \n" , RES_FNAME_LDT_GAMBLE );
#endif
		return;
	}

	LDT_Field					ldtFieldDescription;
	LDT_Field					ldtFieldConsumption_ItemID;
	LDT_Field					ldtFieldConsumption_ItemCount;
	LDT_Field					ldtFieldConsumption_ErrorType;
	LDT_Field					ldtFieldConsumption_ErrorMessage;
	LDT_Field					ldtFieldSlotToken;

	int							iRecordCount	=	pLDTFile->GetItemCount();
	int							iItemID;
	LDT_GAMBLE					stGamble;
	int							iItemIDStep		=	0;
	GAMBLE_CONSUMPTION_ITEM		stConsumptionItem;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetField( iItemID, 0,		ldtFieldDescription						);			//	_Description

		iItemIDStep	=	-1;

		for( int i = 0 ; i < 3 ; ++i )
		{
			for( int j = 0 ; j < 2 ; ++j )
			{
				iItemIDStep	+=	2;

				pLDTFile->GetField( iItemID, iItemIDStep		,	ldtFieldConsumption_ItemID			);			//	_Consumption_ITEMID
				pLDTFile->GetField( iItemID, iItemIDStep + 1	,	ldtFieldConsumption_ItemCount		);			//	_Consumption_ITEMCount
				pLDTFile->GetField( iItemID, iItemIDStep + 4	,	ldtFieldConsumption_ErrorMessage	);			//	_Error_Message
				pLDTFile->GetField( iItemID, iItemIDStep + 5	,	ldtFieldConsumption_ErrorType		);			//	_Error_Type

				stConsumptionItem.m_iID				=	ldtFieldConsumption_ItemID.uData.lValue;
				stConsumptionItem.m_iCount			=	ldtFieldConsumption_ItemCount.uData.lValue;
				stConsumptionItem.m_iErrorType		=	ldtFieldConsumption_ErrorType.uData.lValue;

				if( ldtFieldConsumption_ErrorMessage.uData.pValue != NULL )
					stConsumptionItem.m_strErrorMsg	=	ldtFieldConsumption_ErrorMessage.uData.pValue;

				if( j % 2 == 0 )
					stGamble.m_vConsumptionItemKey.push_back( stConsumptionItem );
				else
					stGamble.m_vConsumptionItemBox.push_back( stConsumptionItem );
			}

			iItemIDStep	+=	4;
		}

		stGamble.m_iKeyGamble	=	iItemID;

		pLDTFile->GetFieldFromLable( iItemID, "_Equip", ldtFieldSlotToken );
		stGamble.m_iEquipSlotToken = ldtFieldSlotToken.uData.lValue;
		pLDTFile->GetFieldFromLable( iItemID, "_Consume", ldtFieldSlotToken );
		stGamble.m_iConsumeSlotToken = ldtFieldSlotToken.uData.lValue;
		pLDTFile->GetFieldFromLable( iItemID, "_Etc", ldtFieldSlotToken );
		stGamble.m_iEtcSlotToken = ldtFieldSlotToken.uData.lValue;
		pLDTFile->GetFieldFromLable( iItemID, "_Quest", ldtFieldSlotToken );
		stGamble.m_iQuestSlotToken = ldtFieldSlotToken.uData.lValue;
		pLDTFile->GetFieldFromLable( iItemID, "_Pet", ldtFieldSlotToken );
		stGamble.m_iPetSlotToken = ldtFieldSlotToken.uData.lValue;

		AddGamble( stGamble );

		stGamble.m_vConsumptionItemKey.clear();
		stGamble.m_vConsumptionItemBox.clear();
		stGamble.m_vGambleReward.clear();
	}

	SAFE_RELEASE(pLDTFile);
}

void SPWindowGamble::LoadGambleRewardLDT()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_GAMBLE_REWARD , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [ %s ] \n" , RES_FNAME_LDT_GAMBLE_REWARD );
#endif
		return;
	}

	LDT_Field			ldtFieldGambleID;
	LDT_Field			ldtFieldItemID;
	LDT_Field			ldtFieldItemCount;
	LDT_Field			ldtFieldItemRare;
	LDT_Field			ldtFieldItemPoint;
	LDT_Field			ldtFieldIcon;
	LDT_Field			ldtFieldIconIndex;
	LDT_Field			ldtFieldEly;
	LDT_Field			ldtFieldMsgItem;
	LDT_Field			ldtFieldMsgEly;

	int					iRecordCount	= pLDTFile->GetItemCount();
	int					iItemID;
	LDT_GAMBLE_REWARD	stGambleReward;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetField( iItemID, 0,		ldtFieldGambleID	);		//	_Gamble_ID
		pLDTFile->GetField( iItemID, 1,		ldtFieldItemID		);		//	_ITEM_ID
		pLDTFile->GetField( iItemID, 2,		ldtFieldItemCount	);		//	_ITEM_Count
		pLDTFile->GetField( iItemID, 3,		ldtFieldItemRare	);		//	_ITEM_Rare
		pLDTFile->GetField( iItemID, 4,		ldtFieldItemPoint	);		//	_ITEM_Point
		pLDTFile->GetField( iItemID, 5,		ldtFieldIcon		);		//	_Icon
		pLDTFile->GetField( iItemID, 6,		ldtFieldIconIndex	);		//	_Icon_Index
		pLDTFile->GetField( iItemID, 8,		ldtFieldEly			);		//	_Ely
		pLDTFile->GetField( iItemID, 10,	ldtFieldMsgItem		);		//	_Reward_Message
		pLDTFile->GetField( iItemID, 11,	ldtFieldMsgEly		);		//	_Reward_Message_Ely

		stGambleReward.m_iKeyGambleReward	=	iItemID;
		stGambleReward.m_iGambleID			=	ldtFieldGambleID.uData.lValue;
		stGambleReward.m_iItemID			=	ldtFieldItemID.uData.lValue;
		stGambleReward.m_iItemCount			=	ldtFieldItemCount.uData.lValue;
		stGambleReward.m_iItemRare			=	ldtFieldItemRare.uData.lValue;
		stGambleReward.m_iItemPoint			=	ldtFieldItemPoint.uData.lValue;
		stGambleReward.m_iIcon				=	ldtFieldIcon.uData.lValue;
		stGambleReward.m_iIconIndex			=	ldtFieldIconIndex.uData.lValue;
		stGambleReward.m_iEly				=	ldtFieldEly.uData.lValue;

		if( ldtFieldMsgItem.uData.pValue != NULL )
			stGambleReward.m_strMessage_Item	=	ldtFieldMsgItem.uData.pValue;

		if( ldtFieldMsgEly.uData.pValue != NULL )
			stGambleReward.m_strMessage_Ely	=	ldtFieldMsgEly.uData.pValue;

		AddGambleReward( stGambleReward );
	}

	SAFE_RELEASE(pLDTFile);
}

void SPWindowGamble::InitItemUnit()
{
	char			szResPath[]		=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
	SPUIItemUnit*	pUIItemUnit		=	NULL;
	int				iXPos , iYPos;

	m_UIUnitManager.SetBufferSize( m_iUnitCount );

	for( int i = 0 ; i < m_iUnitCount ; ++i )
	{
		iXPos	=	m_iAX + 471;
		iYPos	=	m_iAY + 59 + ( i * 36 );

		pUIItemUnit		=	new SPUIItemUnit;

//		pUIItemUnit->SetBGTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" );
//		pUIItemUnit->SetBGRectSrc( 358 , 382 , 390 , 414 );
		pUIItemUnit->SetPos( iXPos , iYPos );
		pUIItemUnit->SetSize( 32 , 32 );
		pUIItemUnit->SetDisableTexture( szResPath );
		pUIItemUnit->SetDisableRectSrc( 434 , 37 , 436 , 39 );
		//pUIItemUnit->SetCoolTimeTexture( szResPath );
		//pUIItemUnit->SetCoolTimeRectSrc( 434 , 37 , 436 , 39 );
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
		pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
		pUIItemUnit->SetDuration( FALSE );
		pUIItemUnit->SetInstanceID( WIID_GAMBLE );
		pUIItemUnit->SetItemID( 180100501 );
		pUIItemUnit->ReResourceLoad();

		m_UIUnitManager.AddUnit( pUIItemUnit );
	}
}

bool SPWindowGamble::RunGamble( int iGambleID , UINT8 iBattingCount , STD_VECTOR_GAMBLE_REWARD* pvGambleRewardAutoSell )
{
	LDT_GAMBLE*	pLdtGamble		=	GetGamble( iGambleID );

	if( pLdtGamble == NULL )
		return false;

	if( iBattingCount <= 0 )
		return false;

	STD_VECTOR_GAMBLE_TRASH_ITEM	vTrashItem;

	//	겜블 조건 체크 ( 사망 , 컷인 , 개인상점 오픈 , 개인 거래 , 캐시샵 이용 , 루팅 상태 )
	if( CheckPlayerState( true ) == false )
		return false;

	//	베팅에 따른 필요 아이템 체크
	if( CheckConsumptionItem( &vTrashItem , iBattingCount , pLdtGamble->m_vConsumptionItemKey , pLdtGamble->m_vConsumptionItemBox , true ) == false )
		return false;

	//	인벤토리 공간 체크
	//if( CheckInventory( iBattingCount , true ) == false )
	//	return false;
	if (CheckInventory( *pLdtGamble) ==  false)
		return false;

	if( g_pCheckManager != NULL )
	{
		if( g_pCheckManager->IsDBBlock() == true )
		{
			PrintErrorMsg( 4000002 , true , D3DCOLOR_ARGB( 255 , 228 , 3 , 3 ) );
			return 1;
		}
	}

	m_iLastGambleID		=	iGambleID;

	//	서버 패킷 발송
	UINT8			iNumOfUseItem		=	(UINT8)vTrashItem.size();
	TRASH_ITEM*		pTrashItem;

	CPacket	Packet;
	Packet.AddUINT32( GAMBLE_CS_REQ );

	Packet.AddData( &(m_iLastGambleID) , sizeof(int) );
	Packet.AddUINT8( iBattingCount );

	Packet.AddUINT8( iNumOfUseItem );

	for( UINT8 i = 0 ; i < iNumOfUseItem ; ++i )
	{
		pTrashItem	=	&( vTrashItem.at( i ) );

		if( pTrashItem != NULL )
			Packet.AddData( pTrashItem , sizeof( TRASH_ITEM ) );
	}

	UINT8	iNumOfAutoSell			=	0;
	int		iGambleRewardAutoSell	=	0;

	if( pvGambleRewardAutoSell != NULL )
	{
		iNumOfAutoSell	=	(int)pvGambleRewardAutoSell->size();
	}

	Packet.AddUINT8( iNumOfAutoSell );

	if( pvGambleRewardAutoSell != NULL )
	{
		for( i = 0 ; i < iNumOfAutoSell ; ++i )
		{
			iGambleRewardAutoSell	=	pvGambleRewardAutoSell->at( i );

			Packet.AddData( &(iGambleRewardAutoSell) , sizeof(int) );
		}
	}

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

	if( g_pCheckManager != NULL )
		g_pCheckManager->SetDBBlock( true );

	return true;
}

void SPWindowGamble::PrintErrorMsg( GLOBAL_STRING_ID uiGlobalStringID , bool bChatWindow , D3DXCOLOR stColor , const char* pszName /* = NULL  */ )
{
	DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

	const char* pszGlobalString	=	g_pResourceManager->GetGlobalString( uiGlobalStringID );

	PrintErrorMsg( g_pResourceManager->GetGlobalString( uiGlobalStringID ) , bChatWindow , stColor , pszName );
}

void SPWindowGamble::PrintErrorMsg( const char* pszGlobalString , bool bChatWindow , D3DXCOLOR stColor , const char* pszName /* = NULL  */ )
{
	DECLARE_VAR( TCHAR , szErrorMsg , MAX_CHATMSG_LEN );

	if( pszGlobalString == NULL )
		return;

	if( pszName != NULL )
		sprintf( szErrorMsg , pszGlobalString , pszName );
	else
		strcpy( szErrorMsg , pszGlobalString );

	if( bChatWindow == false )
	{
		g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szErrorMsg , (LPARAM)&stColor );
	}
	else
	{
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHAT , SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szErrorMsg, (LPARAM)&stColor);
	}
}

void SPWindowGamble::SetCashShopDlg( const char* pszMsg )
{
	if( pszMsg == NULL || g_pInterfaceManager == NULL )
		return;

	SPNoticeBox*	pNoticeBox	=	(SPNoticeBox*)( g_pInterfaceManager->GetNoticeBox() );

	if( pNoticeBox == NULL )
		return;

	if( pNoticeBox != NULL )
		pNoticeBox->Notice( pszMsg, this , NOTICE_TYPE_YESNO );
}

void SPWindowGamble::AddGamble( LDT_GAMBLE& stGamble )
{
	m_mGamble.insert( STD_MAP_GAMBLE::value_type( stGamble.m_iKeyGamble , stGamble ) );
}

LDT_GAMBLE* SPWindowGamble::GetGamble( int iKey )
{
	LDT_GAMBLE*					pLdtGamble	=	NULL;
	STD_MAP_GAMBLE::iterator	mIter		=	m_mGamble.find( iKey );

	if( mIter != m_mGamble.end() )
		pLdtGamble	=	&((*mIter).second);

	return pLdtGamble;
}

void SPWindowGamble::LinkGamble( int iGambleID , int iGambleRewardID )
{
	LDT_GAMBLE*	pLdtGamble	=	GetGamble( iGambleID );

	if( pLdtGamble == NULL )
		return;

	pLdtGamble->m_vGambleReward.push_back( iGambleRewardID );
}

void SPWindowGamble::AddGambleReward( LDT_GAMBLE_REWARD& stGambleReward )
{
	m_mGambleReward.insert( STD_MAP_GAMBLE_REWARD::value_type( stGambleReward.m_iKeyGambleReward , stGambleReward ) );

	LinkGamble( stGambleReward.m_iGambleID , stGambleReward.m_iKeyGambleReward );
}

LDT_GAMBLE_REWARD* SPWindowGamble::GetGambleReward( int iKey )
{
	LDT_GAMBLE_REWARD*				pLdtGambleReward	=	NULL;
	STD_MAP_GAMBLE_REWARD::iterator	mIter				=	m_mGambleReward.find( iKey );

	if( mIter != m_mGambleReward.end() )
		pLdtGambleReward	=	&((*mIter).second);

	return pLdtGambleReward;
}

bool SPWindowGamble::CheckPlayerState( bool bErrorShow )
{
	//	겜블 조건 체크 ( 사망 , 컷인 , 개인상점 오픈 , 개인 거래 , 캐시샵 이용 , 루팅 상태 )

	if( g_pGOBManager == NULL || g_pEventManager == NULL || g_pSubGameManager == NULL )
		return false;

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return false;

	bool		bWindowLootingShow	=	false;
	SPWindow*	pWindowLooting		=	g_pInterfaceManager->FindWindow( WIID_LOOTING );

	if( pWindowLooting != NULL )
		bWindowLootingShow	=	pWindowLooting->IsShow();

	if( pLocalPlayer->IsDead() == true																||
		pLocalPlayer->GetCurAction() & ACTION_USERSHOP												||
		pLocalPlayer->GetCurAction() & ACTION_LOOTING												||
		bWindowLootingShow == true																	||
		pLocalPlayer->GetUserShopID() > 0															||
		pLocalPlayer->GetTradeState() == TRADE_STATE_TRADE											||
		g_pSubGameManager->GetGameState() == GS_CASHSHOP											||
		g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL										)
	{
		PrintErrorMsg( 13002001 , true , D3DCOLOR_ARGB( 255 , 228 , 3 , 3 ) );
		return false;
	}

	return true;
}

bool SPWindowGamble::CheckConsumptionItem( STD_VECTOR_GAMBLE_TRASH_ITEM* pvTrashItem , UINT8 iBattingCount , STD_VECTOR_GAMBLE_CONSUMPTION_ITEM& vItemKey , STD_VECTOR_GAMBLE_CONSUMPTION_ITEM& vItemBox , bool bErrorShow )
{
	if( iBattingCount <= 0 )
		return false;

	STD_VECTOR_GAMBLE_CONSUMPTION_ITEM::iterator	Iter				=	vItemKey.begin();
	GAMBLE_CONSUMPTION_ITEM*						pConsumptionItem	=	NULL;
	int												iMaxBettingCount	=	0;

	while( Iter != vItemKey.end() )
	{
		pConsumptionItem	=	&(*Iter);

		if( pConsumptionItem != NULL )
		{
			if( pConsumptionItem->m_iID > 0 )
				++iMaxBettingCount;
			else
				break;
		}

		++Iter;
	}

	if( iMaxBettingCount < iBattingCount )
		return false;

	if( pvTrashItem != NULL )
		pvTrashItem->clear();

	int	iItemKeyCount	=	(int)vItemKey.size();

	if( iItemKeyCount >= iBattingCount )
	{
		pConsumptionItem	=	&( vItemKey.at( iBattingCount - 1 ) );

		if( CheckConsumptionItem( pvTrashItem , pConsumptionItem , true ) == false )
			return false;
	}

	int	iItemBoxCount	=	(int)vItemBox.size();

	if( iItemKeyCount >= iBattingCount )
	{
		pConsumptionItem	=	&( vItemBox.at( iBattingCount - 1 ) );

		if( CheckConsumptionItem( pvTrashItem , pConsumptionItem , false ) == false )
			return false;
	}

	return true;
}

bool SPWindowGamble::CheckConsumptionItem( STD_VECTOR_GAMBLE_TRASH_ITEM* pvTrashItem , GAMBLE_CONSUMPTION_ITEM* pConsumptionItem , bool bIDZeroCheck )
{
	if( pConsumptionItem == NULL || g_pGOBManager == NULL )
		return false;

	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return false;

	SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();

	if( pInvenArchive == NULL )
		return false;

	SPSelectItem	stFindConsumptionItem;
	TRASH_ITEM		tTrashItem;

	if( pConsumptionItem->m_iID > 0 )
	{
		if( pInvenArchive->FindInvenItem( pConsumptionItem->m_iID , pConsumptionItem->m_iCount , &stFindConsumptionItem ) == false )
		{
			if( pConsumptionItem->m_iErrorType == 1 )
			{
				PrintErrorMsg( pConsumptionItem->m_strErrorMsg.c_str() , true , D3DCOLOR_ARGB( 255 , 228 , 3 , 3 ) );
			}
			else if( pConsumptionItem->m_iErrorType == 2 )
			{
				SetCashShopDlg( pConsumptionItem->m_strErrorMsg.c_str() );
			}

			return false;
		}
		else
		{
			if( pvTrashItem != NULL )
			{
				tTrashItem.iContainerSlot.ContainerType	=	stFindConsumptionItem.m_iContainer;
				tTrashItem.iContainerSlot.SlotIndex		=	stFindConsumptionItem.m_iSlotIndex + 1;
				tTrashItem.iCount						=	pConsumptionItem->m_iCount;

				pvTrashItem->push_back( tTrashItem );
			}
		}
	}
	else
	{
		if( bIDZeroCheck == true )
			return false;
	}

	return true;
}

bool SPWindowGamble::CheckInventory( int iSlotCount , bool bErrorShow )
{
	//	슬롯 체크
	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return false;

	SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();

	if( pInvenArchive == NULL )
		return false;

	for( int i = INVENTORY_EQUIP ; i <= INVENTORY_HOUSING ; ++i )
	{
		if( pInvenArchive->GetEmptySlotCount( (CONTAINER_TYPE)i ) < iSlotCount )
		{
			if( bErrorShow == true )
				PrintErrorMsg( 13002101 + i , true , D3DCOLOR_ARGB( 255 , 228 , 3 , 3 ) );

			return false;
		}
	}

	return true;
}

bool SPWindowGamble::CheckInventory( const LDT_GAMBLE& gamble_info )
{
	SPPlayer	*pLocalPlayer = g_pGOBManager->GetLocalPlayer();
	if (!pLocalPlayer)
		return false;

	SPPlayerInvenArchive *pInvenArchive = pLocalPlayer->GetInvenArchive();
	if (!pInvenArchive)
		return false;

	// equip, consume, tec, event, pet
	if (pInvenArchive->GetEmptySlotCount( INVENTORY_EQUIP ) < gamble_info.m_iEquipSlotToken )
	{
		PrintErrorMsg( 13002101+INVENTORY_EQUIP, true, D3DCOLOR_ARGB( 255 , 228 , 3 , 3 ) );
		return false;
	}
	if (pInvenArchive->GetEmptySlotCount( INVENTORY_CONSUME ) < gamble_info.m_iConsumeSlotToken )
	{
		PrintErrorMsg( 13002101+INVENTORY_CONSUME, true, D3DCOLOR_ARGB( 255 , 228 , 3 , 3 ) );
		return false;
	}
	if (pInvenArchive->GetEmptySlotCount( INVENTORY_ETC ) < gamble_info.m_iEtcSlotToken)
	{
		PrintErrorMsg( 13002101+INVENTORY_ETC, true, D3DCOLOR_ARGB( 255 , 228 , 3 , 3 ) );
		return false;
	}
	if (pInvenArchive->GetEmptySlotCount( INVENTORY_EVENT ) < gamble_info.m_iQuestSlotToken)
	{
		PrintErrorMsg( 13002101+INVENTORY_EVENT, true, D3DCOLOR_ARGB( 255 , 228 , 3 , 3 ) );
		return false;
	}
	if (pInvenArchive->GetEmptySlotCount( INVENTORY_PET ) < gamble_info.m_iPetSlotToken )
	{
		PrintErrorMsg( 13002101+INVENTORY_PET, true, D3DCOLOR_ARGB( 255 , 228 , 3 , 3 ) );
		return false;
	}

	return true;
}
