// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-7-4    15:10 
//***************************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowSlider.h"
#include "SPWindowButton.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPUIUnit.h"
#include "SPUISkillUnit.h"
#include "SPUIItemUnit.h"
#include "SPUIChatMacro.h"
#include "SPUIUnitManager.h"
#include "SPDragnDropManager.h"
#include "SPPlayerActionManager.h"
#include "SPInputManager.h"
#include "SPJoyPadManager.h"
#include "SPIMEPool.h"

#include "SPPlayerInvenArchive.h"
#include "SPGOBManager.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "SPItemAttr.h"
#include "SPItem.h"
#include "SPItemCluster.h"

#include "SPCheckManager.h"

#include "SPEffect.h"
#include "SPSkill.h"
#include "SPSkillManager.h"
#include "SPSkillArchive.h"

#include "Packet.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "GlobalDefines_Share.h"
#include "SPLocalizeManager.h"

#include "SPDebug.h"

#include "SPWindowQuickSlot.h"

SPWindowQuickSlot::SPWindowQuickSlot(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowQuickSlot::SPWindowQuickSlot(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_QUICK_SLOT, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowQuickSlot::~SPWindowQuickSlot()
{
	Clean();
}

void SPWindowQuickSlot::Init()
{
	ReposOwnImage();

	m_bLMouseDown	=	FALSE;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" , &m_pBGTexture );

	SetRect( &m_rcBGSrc[ 0 ]	, 501 , 1 , 503 , 37 );		//	배경 왼쪽
	SetRect( &m_rcBGSrc[ 1 ]	, 505 , 1 , 507 , 37 );		//	배경 가운데
	SetRect( &m_rcBGSrc[ 2 ]	, 509 , 1 , 511 , 37 );		//	배경 오른쪽
	SetRect( &m_rcBGSrc[ 3 ]	, 457 , 1 , 500 , 13 );		//	핫키 배경
	SetRect( &m_rcBGSrc[ 4 ]	, 501 , 80 , 503 , 114 );	//	배경 왼쪽
	SetRect( &m_rcBGSrc[ 5 ]	, 505 , 80 , 507 , 114 );	//	배경 가운데
	SetRect( &m_rcBGSrc[ 6 ]	, 509 , 80 , 511 , 114 );	//	배경 오른쪽
	SetRect( &m_rcBGSrc[ 7 ]	, 457 , 1 , 500 , 13 );		//	핫키 배경

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" , &m_pHotKeyImage );

	SetRect( &m_rtHotKeySrc[ 0 ] , 1 , 59 , 403 , 68 );
	SetRect( &m_rtHotKeySrc[ 1 ] , 1 , 69 , 403 , 78 );

	SPWindowButton*	pWindowButton;

	pWindowButton = new SPWindowButton( WIID_QUICK_SLOT_1LINE_SHOW , 377 , 37 , 10 , 10 , this );
	pWindowButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 467 , 14 );
	pWindowButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 478 , 14 );
	pWindowButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 489 , 14 );

	pWindowButton = new SPWindowButton( WIID_QUICK_SLOT_1LINE_HOTKEY_1 , 388 , 37 , 10 , 10 , this );
	pWindowButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 434 , 25 );
	pWindowButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 445 , 25 );
	pWindowButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 456 , 25 );

	pWindowButton = new SPWindowButton( WIID_QUICK_SLOT_1LINE_HOTKEY_2 , 399 , 37 , 10 , 10 , this );
	pWindowButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 467 , 25 );
	pWindowButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 478 , 25 );
	pWindowButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 489 , 25 );

	pWindowButton = new SPWindowButton( WIID_QUICK_SLOT_2LINE_HIDE , 377 , 34 + 37 , 10 , 10 , this );
	pWindowButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 434 , 14 );
	pWindowButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 445 , 14 );
	pWindowButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 456 , 14 );

	pWindowButton = new SPWindowButton( WIID_QUICK_SLOT_2LINE_HOTKEY_1 , 388 , 34 + 37 , 10 , 10 , this );
	pWindowButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 434 , 25 );
	pWindowButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 445 , 25 );
	pWindowButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 456 , 25 );

	pWindowButton = new SPWindowButton( WIID_QUICK_SLOT_2LINE_HOTKEY_2 , 399 , 34 + 37 , 10 , 10 , this );
	pWindowButton->SetImage		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 467 , 25 );
	pWindowButton->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 478 , 25 );
	pWindowButton->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 489 , 25 );

	//////////////////////////////////////////////////////////////////////////

	InitIcon();

	BtnCheck( 0 );
	LineCheck( 1 );

	InitActionKey();
	InitHotKey();

	PreTextureLoad();

	m_fLimitTime		=	0.5f;
	m_fAccumulateTime	=	0.0f;
	m_bUpdate			=	false;

	int	iTotalKeyCount	=	QUICK_X * QUICK_Y;

	for( int i = 0 ; i < iTotalKeyCount ; ++i )
	{
		m_fInputPressRepeatTime[ i ]	=	0.0f;
		m_bChatMessage[ i ]				=	false;
		m_fPadPressRepeatTime[i]		=	0.0f;
	}

	m_lParam				=	0;
	m_iDragDropUnitNumber	=	-1;
	m_bJoyPadZPress			=	false;
}

void SPWindowQuickSlot::Clean()
{
	SPWindow::Clean();

	CleanIcon();

	SAFE_RELEASE( m_pBGTexture );
	SAFE_RELEASE( m_pHotKeyImage );

	PreTextureClean();
}

void SPWindowQuickSlot::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);

	BtnCheck( m_iCurPage );
	LineCheck( m_iCurLine );
}

void SPWindowQuickSlot::Hide(bool bSendServer)
{
	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
		OnCursorOut( NULL ,NULL );
}

void SPWindowQuickSlot::Process(float fTime)
{
	ProcessUpdate( fTime );

	SPWindow::Process( fTime );

	m_UIQuickSlotUnitManager.Process( fTime );

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	if( pLocalPlayer->GetTradeState() == TRADE_STATE_TRADE || g_pInterfaceManager->IsCutIn() == TRUE )
		return;

	ProcessActionKey( fTime );
#ifdef _JOY_PAD	
	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_JOYPAD)) {
		ProcessJoyPad( fTime );
	}
#endif	
	ProcessHotKey();
}

void SPWindowQuickSlot::Render(float fTime)
{
	if(!m_bShow) return;

	g_pVideo->Flush();

	int i , j;

	if( m_pBGTexture )
	{
		int	iLine , iLineCount;

		for( i = 0 ; i <= m_iCurLine ; ++i )
		{
			iLine		=	i * QUICKSLOT_BG_COUNT;
			iLineCount	=	iLine + QUICKSLOT_BG_COUNT;

			for( j = iLine ; j < iLineCount ; ++j )
			{
				m_pBGTexture->RenderTexture( &m_rcBGDest[ j ] , &m_rcBGSrc[ j ] );
			}
		}

		g_pVideo->Flush();
	}

	SPWindow::Render(fTime);

	m_UIQuickSlotUnitManager.Render();

	if( m_pHotKeyImage != NULL )
	{
		for( i = 0 ; i <= m_iCurLine ; ++i )
		{
			if( i < QUICK_HOTKEY_IMG_COUNT )
			{
				m_pHotKeyImage->RenderTexture( &m_rtHotKeyDest[ i ] , &m_rtHotKeySrc[ i ] );
				g_pVideo->Flush();
			}
		}
	}
}

void SPWindowQuickSlot::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowQuickSlot )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER			,										OnCursorEnter		)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT				,										OnCursorOut			)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE			,										OnCursorMove		)

	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP				,										OnLButtonUp			)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN			,										OnLButtonDown		)

	SPIMESSAGE_COMMAND(	SPIM_DRAGNDROP_END			,										OnDragnDropEnd		)

	SPIMESSAGE_COMMAND( SPIM_GET_QUICK_SLOT			,										OnGetQuickSlot		)
	SPIMESSAGE_COMMAND( SPIM_QUICK_SLOT_UPDATE		,										OnQuickSlotUpdate	)
	SPIMESSAGE_COMMAND( SPIM_REFRESH				,										OnStatusRefresh		)

	SPIMESSAGE_COMMAND(	SPIM_PURGE					,										OnPurge				)

	SPIMESSAGE_COMMAND( SPIM_GET_QUICK_SLOT_UNIT	,										OnGetQuickSlotUnit	)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_QUICK_SLOT_1LINE_SHOW		,	On1LineShow			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_QUICK_SLOT_1LINE_HOTKEY_1	,	On1LineHotKey1		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_QUICK_SLOT_1LINE_HOTKEY_2	,	On1LineHotKey2		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_QUICK_SLOT_2LINE_HIDE		,	On2LineShow			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_QUICK_SLOT_2LINE_HOTKEY_1	,	On2LineHotKey1		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_QUICK_SLOT_2LINE_HOTKEY_2	,	On2LineHotKey2		)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowQuickSlot::OnPurge( WPARAM wParam, LPARAM lParam )
{
	InitIconData();

	BtnCheck( 0 );

	int			iLinePurge	=	(int)wParam;

	if( iLinePurge > 0 )
		LineCheck( 1 );

	return 1;
}

int SPWindowQuickSlot::OnCursorEnter( WPARAM wParam, LPARAM lParam )
{
	OnCursorMove( wParam , lParam );
	return 1;
}

int SPWindowQuickSlot::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	if( SPDragnDropManager::GetInstance()->IsDragging() == FALSE )
	{
		m_bLMouseDown	=	FALSE;
		m_lParam		=	0;
	}

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, -2 );
	return 1;
}

int SPWindowQuickSlot::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	if( IsCursorIn() == false )
		return 1;

	int				iXPos			=	LOWORD(lParam);
	int				iYPos			=	HIWORD(lParam);
	int				iXPosOld		=	LOWORD(m_lParam);
	int				iYPosOld		=	HIWORD(m_lParam);
	int				iUnitNumber		=	m_UIQuickSlotUnitManager.GetNumberMouseOver( iXPosOld ,iYPosOld );
	SPUIUnit*		pUIUnit			=	m_UIQuickSlotUnitManager.GetUnit( iUnitNumber );
	SPUIUnit*		pUIUnitToolTip	=	m_UIQuickSlotUnitManager.GetUnitMouseOver( iXPos , iYPos );
	SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();

	if( SPDragnDropManager::GetInstance()->IsDragging() == TRUE )
	{
		pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, -2 );
		return 1;
	}
	else
	{
		pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pUIUnitToolTip, -2 );
	}

	if( iUnitNumber < 0 || pUIUnit == NULL )
		return 1;

	int	iXStep	=	abs( iXPos - iXPosOld );
	int iYStep	=	abs( iYPos - iYPosOld );

	if( SPDragnDropManager::GetInstance()->IsDragging() == FALSE )
	{
		if( m_bLMouseDown == TRUE && ( iXStep > 5 || iYStep > 5 ) )
		{
			if( SPDragnDropManager::GetInstance()->Set( this , pUIUnit ) == FALSE )
				m_bLMouseDown	=	FALSE;
			else
				m_iDragDropUnitNumber	=	iUnitNumber;
		}
	}
	
	return 1;
}

int SPWindowQuickSlot::OnLButtonUp( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;
	
	m_bLMouseDown				=	FALSE;

	int	iXPos					=	LOWORD(lParam);
	int	iYPos					=	HIWORD(lParam);
	int	iUnitNumber				=	m_UIQuickSlotUnitManager.GetNumberMouseOver( iXPos , iYPos );
	SPUIUnit*	pDestSourUIUnit	=	NULL;

	if( SPDragnDropManager::GetInstance()->IsDragging() == TRUE && iUnitNumber > -2 )
	{
		SPWindow*			pWindow;
		SPUIUnit*			pSourSourUIUnit;
		SPUIUnitManager*	pSourManager;
		SPUIUnitManager*	pDestManager;

		if( SPDragnDropManager::GetInstance()->Get( &pWindow , &pSourSourUIUnit ) == TRUE )
		{
			if( iUnitNumber == -1 )
				return 1;

			if( pSourSourUIUnit == NULL )
				return 1;

			if( pSourSourUIUnit->GetInstanceID() != WIID_ITEM && pSourSourUIUnit->GetInstanceID() != WIID_SKILL && pSourSourUIUnit->GetInstanceID() != WIID_CHAT_MACRO )
			{
				return 1;
			}

			// 시간제 아이템 아이템 퀵슬롯 등록 거부 [11/1/2006 AJJIYA]

			SPUIItemUnit*	pItemUnit	=	(SPUIItemUnit*)pSourSourUIUnit;

			if( pItemUnit != NULL )
			{
				SPItemAttr* pItemAttr;

				if( g_pItemCluster->GetItemInfo( pItemUnit->GetItemID() , pItemAttr ) == true )
				{
					if( pItemAttr != NULL )
					{
						if( pItemAttr->GetDurationType() != ITEM_DURATION_NULL )
						{
							D3DXCOLOR		stColor			= g_pResourceManager->GetGlobalColor( 9 );
							const char*		szGlobalString	= g_pResourceManager->GetGlobalString( 10000040 );

							if( szGlobalString != NULL )
							{
								g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szGlobalString , (LPARAM)&stColor );
							}

							return 1;
						}
					}
				}
			}

			if( pWindow->GetInstanceID() == WIID_QUICK_SLOT )
			{
				pDestSourUIUnit	=	m_UIQuickSlotUnitManager.GetUnit( iUnitNumber );

				pSourManager	=	GetUIUnitManager( pSourSourUIUnit->GetInstanceID() );
				pDestManager	=	GetUIUnitManager( pDestSourUIUnit->GetInstanceID() );

				if( pSourSourUIUnit->GetInstanceID() == pDestSourUIUnit->GetInstanceID() )
				{
					SPUIUnit*	pSwapUIUnit	=	pDestSourUIUnit->ClonePtr();

					pDestSourUIUnit->Copy( &pSwapUIUnit );
					pSourSourUIUnit->Copy( &pDestSourUIUnit );
					pSwapUIUnit->Copy( &pSourSourUIUnit );
					delete pSwapUIUnit;

					m_UIQuickSlotUnitManager.SetUnit( m_iDragDropUnitNumber , pSourSourUIUnit , FALSE );
					m_UIQuickSlotUnitManager.SetUnit( iUnitNumber , pDestSourUIUnit , FALSE );
				}
				else
				{
					SPUIUnit*	pSourDestUIUnit	=	pSourManager->GetUnit( iUnitNumber );
					SPUIUnit*	pDestDestUIUnit	=	pDestManager->GetUnit( m_iDragDropUnitNumber );

					SPUIUnit*	pSwapUIUnit	=	pSourDestUIUnit->ClonePtr();

					pSourDestUIUnit->Copy( &pSwapUIUnit );
					pSourSourUIUnit->Copy( &pSourDestUIUnit );
					pSwapUIUnit->Copy( &pSourSourUIUnit );
					delete pSwapUIUnit;

					pSwapUIUnit	=	pDestDestUIUnit->ClonePtr();

					pDestDestUIUnit->Copy( &pSwapUIUnit );
					pDestSourUIUnit->Copy( &pDestDestUIUnit );
					pSwapUIUnit->Copy( &pDestSourUIUnit );
					delete pSwapUIUnit;

					m_UIQuickSlotUnitManager.SetUnit( m_iDragDropUnitNumber , pDestDestUIUnit , FALSE );
					m_UIQuickSlotUnitManager.SetUnit( iUnitNumber , pSourDestUIUnit , FALSE );
				}

				SwapQuickSlot( m_iDragDropUnitNumber , iUnitNumber );
			}
			else
			{
				pSourManager	=	GetUIUnitManager( pSourSourUIUnit->GetInstanceID() );
				pDestSourUIUnit	=	pSourManager->GetUnit( iUnitNumber );

				pSourSourUIUnit->Copy( &pDestSourUIUnit );
				m_UIQuickSlotUnitManager.SetUnit( iUnitNumber , pDestSourUIUnit , FALSE );

				SwapQuickSlot( -1 , iUnitNumber );
			}
		}
	}

	if( iUnitNumber >= 0 )
	{
		pDestSourUIUnit	=	m_UIQuickSlotUnitManager.GetUnit( iUnitNumber );
		if( pDestSourUIUnit != NULL )
		{
			SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();
			pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pDestSourUIUnit, -2 );

			if( lParam == m_lParam )
				pDestSourUIUnit->Use();
		}
	}

	CheckItemDrag();

	m_lParam	=	0;

	return 1;
}

int SPWindowQuickSlot::OnLButtonDown( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	int				iXPos			=	LOWORD(lParam);
	int				iYPos			=	HIWORD(lParam);
	int				iUnitNumber		=	m_UIQuickSlotUnitManager.GetNumberMouseOver( iXPos ,iYPos );

	if( iUnitNumber >= 0 )
	{
		m_bLMouseDown	=	TRUE;
		m_lParam		=	lParam;
	}

	if( SPDragnDropManager::GetInstance()->IsDragging() == TRUE )
		return 1;

	return 1;
}

int SPWindowQuickSlot::OnDragnDropEnd(WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	SPUIUnit*	pDnDUIUnit	=	m_UIQuickSlotUnitManager.GetUnit( m_iDragDropUnitNumber );

	if( pDnDUIUnit != NULL )
	{
		if( m_bLMouseDown == TRUE && m_bHotKey == FALSE )
		{
			SPUIUnit*	pUINoneUnit	=	m_UINoneUnitManager.GetUnit( m_iDragDropUnitNumber );

			m_UIQuickSlotUnitManager.SetUnit( m_iDragDropUnitNumber , pUINoneUnit , FALSE );

			SwapQuickSlot( m_iDragDropUnitNumber , -1 );
		}
	}

	m_bLMouseDown	=	FALSE;

	return 1;
}

int SPWindowQuickSlot::OnGetQuickSlot( WPARAM wParam, LPARAM lParam )
{
	int			iQuickSlotSize	=	(int)wParam;

	if( iQuickSlotSize == 0 )
		return OnPurge( NULL , NULL );

	CPacket*	pPacket			=	(CPacket*)lParam;
	QUICK_SLOT	tQuickSlot		=	{0,};

	for( int i = 0 ; i < iQuickSlotSize ; ++i )
	{
		pPacket->ExtractStruct( &tQuickSlot , sizeof(QUICK_SLOT) );
		m_QuickSlot[ tQuickSlot.iSlotIndex ]	=	tQuickSlot;
		UpdateIconSlot( tQuickSlot.iSlotIndex );
	}

	return 1;
}

int SPWindowQuickSlot::OnQuickSlotUpdate( WPARAM wParam, LPARAM lParam )
{
	int	iType		=	(int)wParam;
	int	iID			=	(int)lParam;

	if( iType == 0 )
	{
		DeleteItemSlot( iID );
		UpdateIconChatMacro( iID );
	}
	else if( iType == -1 )
	{
		if( iID < 0 )
			DeleteSkillSlot( -iID );
		else
			UpdateIconID( iID );
	}
	else if( iType == -2 )
	{
		UpdateIconChatMacro( lParam );
	}

	return 1;
}

int SPWindowQuickSlot::OnStatusRefresh( WPARAM wParam, LPARAM lParam )
{
	m_bUpdate	=	true;

	UpdateIconPage( m_iCurPage );

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
		OnCursorMove( 0 , MAKELONG( iCursorX , iCursorY ) );

	return 1;
}

int SPWindowQuickSlot::On1LineShow( WPARAM wParam, LPARAM lParam )
{
	LineCheck( 1 );
	return 1;
}

int SPWindowQuickSlot::On1LineHotKey1( WPARAM wParam, LPARAM lParam )
{
	if( m_iCurLine != 0 )
		return 1;

	if( SPDragnDropManager::GetInstance()->IsDragging() == TRUE )
		return 1;

	CheckItemDrag();

	BtnCheck( 0 );

	return 1;
}

int SPWindowQuickSlot::On1LineHotKey2( WPARAM wParam, LPARAM lParam )
{
	if( m_iCurLine != 0 )
		return 1;

	if( SPDragnDropManager::GetInstance()->IsDragging() == TRUE )
		return 1;

	CheckItemDrag();

	BtnCheck( 1 );

	return 1;
}

int SPWindowQuickSlot::On2LineShow( WPARAM wParam, LPARAM lParam )
{
	LineCheck( 0 );

	return 1;
}

int SPWindowQuickSlot::On2LineHotKey1( WPARAM wParam, LPARAM lParam )
{
	if( m_iCurLine != 1 )
		return 1;

	if( SPDragnDropManager::GetInstance()->IsDragging() == TRUE )
		return 1;

	CheckItemDrag();

	BtnCheck( 0 );

	return 1;
}

int SPWindowQuickSlot::On2LineHotKey2( WPARAM wParam, LPARAM lParam )
{
	if( m_iCurLine != 1 )
		return 1;

	if( SPDragnDropManager::GetInstance()->IsDragging() == TRUE )
		return 1;

	CheckItemDrag();

	BtnCheck( 1 );

	return 1;
}

int SPWindowQuickSlot::OnGetQuickSlotUnit( WPARAM wParam, LPARAM lParam )
{
	*((SPUIUnitManager **)wParam) = &m_UIQuickSlotUnitManager;
	return 1;
}

void SPWindowQuickSlot::InitIcon()
{
	char			szResPath[]		=	"DATA/INTERFACE/CONCEPT/UI100.PNG";
	int				iXPos , iYPos;
	SPUIUnit*		pUIUnit			=	NULL;
	SPUISkillUnit*	pUISkillUnit	=	NULL;
	SPUIItemUnit*	pUIItemUnit		=	NULL;
	SPUIChatMacro*	pUIChatMacroUnit	=	NULL;

	//	None Imgae Init
	for( int y = 0 ; y < QUICK_Y ; ++y )
	{
		for( int x = 0 ; x < QUICK_X ; ++x )
		{
			iXPos	=	m_iAX + QUICK_X_OFFSET + ( x * QUICK_X_GAP );
			iYPos	=	m_iAY + QUICK_Y_OFFSET + ( y * QUICK_Y_GAP );

			pUIUnit			=	new	SPUIUnit;

			pUIUnit->SetTexture( szResPath );
			pUIUnit->SetRectSrc( 400 , 20 , 432 , 52 );
			pUIUnit->SetPos( iXPos , iYPos );
			pUIUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
			pUIUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
			pUIUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_NO_TOOLTIP );
			pUIUnit->SetInstanceID( WIID_QUICK_SLOT );

			m_UINoneUnitManager.AddUnit( pUIUnit );
			m_UIQuickSlotUnitManager.AddUnit( pUIUnit );

			pUISkillUnit	=	new SPUISkillUnit;

			pUISkillUnit->SetPos( iXPos , iYPos );
			pUISkillUnit->SetSize( 32 , 32 );
			pUISkillUnit->SetDisableTexture( szResPath );
			pUISkillUnit->SetDisableRectSrc( 434 , 37 , 436 , 39 );
			pUISkillUnit->SetCoolTimeTexture( szResPath );
			pUISkillUnit->SetCoolTimeRectSrc( 434 , 37 , 436 , 39 );

			pUISkillUnit->SetComboTexture( "DATA/INTERFACE/CONCEPT/Combo_Skill_ICON.png" );
			pUISkillUnit->SetComboRectSrc( 0 , 0 , 32 , 32 );

			pUISkillUnit->SetToggleTexture( "DATA/INTERFACE/CONCEPT/ui_Icon_On.png" );

			pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
			pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUISkillUnit->SetInstanceID( WIID_SKILL );

			m_UISkillUnitManager.AddUnit( pUISkillUnit );

			pUIItemUnit		=	new SPUIItemUnit;

			pUIItemUnit->SetBGTexture( szResPath );
			pUIItemUnit->SetBGRectSrc( 400 , 20 , 432  , 52 );
			pUIItemUnit->SetPos( iXPos , iYPos );
			pUIItemUnit->SetSize( 32 , 32 );
			pUIItemUnit->SetDisableTexture( szResPath );
			pUIItemUnit->SetDisableRectSrc( 434 , 37 , 436 , 39 );
			pUIItemUnit->SetCoolTimeTexture( szResPath );
			pUIItemUnit->SetCoolTimeRectSrc( 434 , 37 , 436 , 39 );
			pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
			pUIItemUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIItemUnit->SetInstanceID( WIID_ITEM );

			m_UIItemUnitManager.AddUnit( pUIItemUnit );

			pUIChatMacroUnit	=	new	SPUIChatMacro;

			pUIChatMacroUnit->SetTexture( szResPath );
			pUIChatMacroUnit->SetRectSrc( 400 , 20 , 432 , 52 );
			pUIChatMacroUnit->SetPos( iXPos , iYPos );
			pUIChatMacroUnit->SetTextRect( iXPos , iYPos + 19 , iXPos + 32 , iYPos + 32 );
			pUIChatMacroUnit->SetSize( 32 , 32 );
			pUIChatMacroUnit->SetDisableTexture( szResPath );
			pUIChatMacroUnit->SetDisableRectSrc( 434 , 37 , 436 , 39 );
			pUIChatMacroUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
			pUIChatMacroUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			pUIChatMacroUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_NO_TOOLTIP );
			pUIChatMacroUnit->SetInstanceID( WIID_CHAT_MACRO );

			m_UIChatMacroUnitManager.AddUnit( (SPUIUnit*)pUIChatMacroUnit );
		}
	}

	InitIconData();
}

void SPWindowQuickSlot::InitIconData()
{
	int iMaxQuickSlot	=	QUICK_X * QUICK_Y * QUICK_HOTKEY_COUNT;

	for( int i = 0 ; i < iMaxQuickSlot ; ++i )
	{
		m_QuickSlot[ i ].iSlotIndex				=	i;
		m_QuickSlot[ i ].LinkedData.iFlag		=	QUIKC_LINK_FLAG_INVALID;
		m_QuickSlot[ i ].LinkedData.iObjTypeID	=	0;
	}
}

void SPWindowQuickSlot::CleanIcon()
{
	m_UINoneUnitManager.ClearAll( TRUE );
	m_UIItemUnitManager.ClearAll( TRUE );
	m_UISkillUnitManager.ClearAll( TRUE );
	m_UIChatMacroUnitManager.ClearAll( TRUE );
	m_UIQuickSlotUnitManager.ClearAll( FALSE );
}

void SPWindowQuickSlot::UpdateIconPage( int iPage )
{
	int		iSlotCount		=	QUICK_X	*	QUICK_Y;
	int		iStartSlotNum	=	iPage	*	iSlotCount;

	for( int i = 0 ; i < iSlotCount ; ++i )
		UpdateIconSlot( iStartSlotNum + i );
}

void SPWindowQuickSlot::UpdateIconSlot( int iSlotNum )
{
	int		iSlotCount			=	QUICK_X		*	QUICK_Y;
	int		iSlotTotalCount		=	iSlotCount	*	QUICK_HOTKEY_COUNT;

	if( iSlotNum < 0 || iSlotNum >= iSlotTotalCount )
		return;

	int iSlotUnitStartNum		=	m_iCurPage			*	iSlotCount;
	int iSlotUnitEndNum			=	iSlotUnitStartNum	+	iSlotCount;

	if( iSlotUnitStartNum > iSlotNum || iSlotUnitEndNum <= iSlotNum )
		return;

	int		iSlotNumPortion		=	iSlotNum	/	iSlotCount;
	int		iSlotUnitNum		=	( iSlotNum - iSlotNumPortion * iSlotCount )	%	iSlotCount;

	QUICK_LINK_FLAG	eFlag		=	(QUICK_LINK_FLAG)m_QuickSlot[ iSlotNum ].LinkedData.iFlag;
	int				iID			=	m_QuickSlot[ iSlotNum ].LinkedData.iObjTypeID;
	int				iSubID		=	m_QuickSlot[ iSlotNum ].LinkedData.iSubTypeID;
	int				iCoolTimeID	=	-1;

	SPUIUnit* pUIUnit	=	NULL;

	switch( eFlag )
	{
		case QUIKC_LINK_FLAG_INVALID:
			pUIUnit	=	m_UINoneUnitManager.GetUnit( iSlotUnitNum );
			break;
		case QUICK_LINK_FLAG_ITEM:
			pUIUnit	=	m_UIItemUnitManager.GetUnit( iSlotUnitNum );
			break;
		case QUICK_LINK_FLAG_ACTION:
		case QUICK_LINK_FLAG_SKILL:
		case QUICK_LINK_FLAG_ITEMSKILL:
		case QUICK_LINK_FLAG_PETSKILL:
			pUIUnit	=	m_UISkillUnitManager.GetUnit( iSlotUnitNum );
			break;
		case QUICK_LINK_FLAG_CHAT:
			pUIUnit	=	m_UIChatMacroUnitManager.GetUnit( iSlotUnitNum );
			break;
	}

	if( pUIUnit == NULL )
		return;

	if( eFlag == QUICK_LINK_FLAG_ITEM )
	{
		SPUIItemUnit*	pUIItemUnit	=	(SPUIItemUnit*)pUIUnit;
		if( pUIItemUnit == NULL )
			return;

		pUIItemUnit->SetItemID( iID );
		pUIItemUnit->SetItemType( INVENTORY_CONSUME );

		if( ( m_bUpdate == false && m_fAccumulateTime == 0.0f ) || pUIItemUnit->GetCount() <= 0 )
			pUIItemUnit->CheckCount();

		SPItemAttr*	pItemAttr	=	NULL;
		g_pItemCluster->GetItemInfo( iID , pItemAttr );

		if( pItemAttr != NULL )
			iCoolTimeID	=	pItemAttr->m_iItemCoolTimeID;

		// 기존에 아이템 삭제를 유지로 변경 AJJIYA [7/19/2006]
		//if( pUIItemUnit->GetCount() <= 0 )
		//{
		//	SwapQuickSlot( iSlotUnitNum , -1 );
		//	pUIUnit	=	m_UINoneUnitManager.GetUnit( iSlotUnitNum );
		//}
	}

	if( eFlag == QUICK_LINK_FLAG_ACTION || eFlag == QUICK_LINK_FLAG_SKILL || eFlag == QUICK_LINK_FLAG_ITEMSKILL || eFlag == QUICK_LINK_FLAG_PETSKILL )
	{
		SPUISkillUnit*	pUISkillUnit	=	(SPUISkillUnit*)pUIUnit;
		if( pUISkillUnit == NULL )
			return;

		SKILL_INVENTORY_TYPE	eSit	=	SIT_NULL;

		if( eFlag == QUICK_LINK_FLAG_ACTION )			eSit	=	SIT_ACTION;
		else if( eFlag == QUICK_LINK_FLAG_SKILL || eFlag == QUICK_LINK_FLAG_ITEMSKILL )
		{
			eSit	=	SIT_ETC;

			SPSkill*	pSkill	=	SPSkillManager::GetInstance()->GetSkill( iID );

			if( pSkill != NULL )
			{
				iCoolTimeID	=	pSkill->GetCoolTimeID();
			}
		}

		pUISkillUnit->SetType( eSit );
		pUISkillUnit->SetSkillID( iID );

		bool	bResult	=	false;
		bool	bToggle	=	false;

		SPPlayer*			pLocalPlayer				=	g_pGOBManager->GetLocalPlayer();
		SPSkillArchive*		pLocalPlayerSkillArchive	=	NULL;

		if( pLocalPlayer != NULL )
			pLocalPlayerSkillArchive	=	pLocalPlayer->GetSkillArchive();

		if( eFlag == QUICK_LINK_FLAG_ACTION )
			bResult	=	g_pCheckManager->CheckActionSkillUseRequire( iID );
		else if( eFlag == QUICK_LINK_FLAG_SKILL )
		{
			bResult	=	g_pCheckManager->CheckSkillUseRequire( iID , true );

			if( pLocalPlayerSkillArchive != NULL )
			{
				SPSkillActivity*	pSkillActivity	=	pLocalPlayerSkillArchive->GetSkillActivity( iID );

				if( pSkillActivity != NULL )
					bToggle	=	pSkillActivity->GetToggleOn();
			}
		}
		else if( eFlag == QUICK_LINK_FLAG_ITEMSKILL )
		{
			pUISkillUnit->SetItemID( iSubID );
			pUISkillUnit->ReItemInfoLoad();
			bResult	=	g_pCheckManager->CheckItemSkillUseRequire( pUISkillUnit->GetSkillID() , pUISkillUnit->GetItemID() , pUISkillUnit->GetContainerSlot().ContainerType , pUISkillUnit->GetContainerSlot().SlotIndex , true );

			if( pLocalPlayerSkillArchive != NULL )
			{
				SPItemSkillActivity*	pItemSkillActivity	=	pLocalPlayerSkillArchive->GetItemSkillActivity( pUISkillUnit->GetSkillID() , pUISkillUnit->GetContainerSlot().ContainerType , pUISkillUnit->GetContainerSlot().SlotIndex );

				if( pItemSkillActivity != NULL )
					bToggle	=	pItemSkillActivity->GetToggleOn();
			}
		}
		else if (eFlag == QUICK_LINK_FLAG_PETSKILL)
		{
			bResult = g_pCheckManager->CheckPetSkillUseRequire( pUISkillUnit->GetSkillID(), pUISkillUnit->GetItemID(), pUISkillUnit->GetContainerSlot().ContainerType, pUISkillUnit->GetContainerSlot().SlotIndex, true );
			if (pLocalPlayerSkillArchive)
			{
				SPItemSkillActivity*	pItemSkillActivity	=	pLocalPlayerSkillArchive->GetItemSkillActivity( pUISkillUnit->GetSkillID() , pUISkillUnit->GetContainerSlot().ContainerType , pUISkillUnit->GetContainerSlot().SlotIndex );

				if( pItemSkillActivity != NULL )
					bToggle	=	pItemSkillActivity->GetToggleOn();
			}
		}

		if( bResult == false )
			pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DISABLE );
		else
			pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );

		pUISkillUnit->SetToggle( bToggle );
	}

	if( eFlag == QUICK_LINK_FLAG_CHAT )
	{
		SPUIChatMacro*	pUIChatMacroUnit	=	(SPUIChatMacro*)pUIUnit;

		if( pUIChatMacroUnit == NULL )
			return;

		CHAT_MACRO_POS	stChatMacroPos;

		stChatMacroPos.m_eType	=	(CHAT_MACRO_TYPE)iID;
		stChatMacroPos.m_iIndex	=	iSubID;

		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHAT_MACRO , SPIM_CHATMACRO_GET_DATA , (WPARAM)(&stChatMacroPos) , (LPARAM)pUIChatMacroUnit );

		CHAT_MACRO*					pChatMacro			=	pUIChatMacroUnit->GetChatMacro();
		CHAT_MACRO_LDT*				pChatMacroLDT		=	pUIChatMacroUnit->GetChatMacroLDT();
		SPUIUnit::STATUS_UI_UNIT	eStatus				=	SPUIUnit::STATUS_UI_UNIT_DISABLE;

		if( pChatMacro != NULL && pChatMacroLDT != NULL )
		{
			if( pChatMacro->m_strName.empty() == false && pChatMacro->m_strContents.empty() == false )
			{
				eStatus	=	SPUIUnit::STATUS_UI_UNIT_ENABLE;
			}

			if( eStatus == SPUIUnit::STATUS_UI_UNIT_ENABLE && pChatMacroLDT->m_eType == CHAT_MACRO_TYPE_SPECIAL )
			{
				eStatus		=	SPUIUnit::STATUS_UI_UNIT_DISABLE;

				SPItemAttr*		pItemAttr	=	NULL;

				g_pItemCluster->GetItemInfo( pChatMacroLDT->m_iRequireItem , pItemAttr );

				if( pItemAttr != NULL)
				{
					CONTAINER_TYPE iContainer = (CONTAINER_TYPE)( pItemAttr->m_eType - 1 );

					if( g_pGOBManager != NULL || g_pItemCluster != NULL )
					{
						SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

						if( pLocalPlayer != NULL )
						{
							SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();

							if( pInvenArchive != NULL )
							{
								SPItem*	pRequireItem	=	pInvenArchive->GetSPItem( iContainer , pChatMacroLDT->m_iRequireItem );

								if( pRequireItem != NULL )
								{
									eStatus	=	SPUIUnit::STATUS_UI_UNIT_ENABLE;
								}
							}
						}
					}
				}
			}
		}

		pUIChatMacroUnit->AddStatus( eStatus );
	}

	pUIUnit->SetCoolTimeID( iCoolTimeID );
	if( pUIUnit->ReResourceLoad() == FALSE )
	{
		SwapQuickSlot( iSlotUnitNum , -1 );
	}

	m_UIQuickSlotUnitManager.SetUnit( iSlotUnitNum , pUIUnit , FALSE );
}

void SPWindowQuickSlot::UpdateIconID( int iID )
{
	int		iSlotCount			=	QUICK_X		*	QUICK_Y;
	int		iSlotTotalCount		=	iSlotCount	*	QUICK_HOTKEY_COUNT;

	int		iSlotUnitStartNum	=	m_iCurPage			*	iSlotCount;
	int		iSlotUnitEndNum		=	iSlotUnitStartNum	+	iSlotCount;

	for( int i = iSlotUnitStartNum ; i < iSlotUnitEndNum ; ++i )
	{
		if( m_QuickSlot[ i ].LinkedData.iObjTypeID == iID )
			UpdateIconSlot( i );
	}
}

void SPWindowQuickSlot::UpdateIconChatMacro( int iID )
{
	int		iSlotCount				=	QUICK_X		*	QUICK_Y;
	int		iSlotTotalCount			=	iSlotCount	*	QUICK_HOTKEY_COUNT;

	int		iSlotUnitStartNum		=	m_iCurPage			*	iSlotCount;
	int		iSlotUnitEndNum			=	iSlotUnitStartNum	+	iSlotCount;

	SPUIChatMacro*	pUIChatMacro	=	NULL;
	CHAT_MACRO*		pChatMacro		=	NULL;
	CHAT_MACRO_LDT*	pChatMacroLDT	=	NULL;

	for( int i = iSlotUnitStartNum ; i < iSlotUnitEndNum ; ++i )
	{
		if( m_QuickSlot[ i ].LinkedData.iFlag != QUICK_LINK_FLAG_CHAT )
			continue;

		pUIChatMacro	=	(SPUIChatMacro*)( m_UIQuickSlotUnitManager.GetUnit( i - iSlotUnitStartNum ) );

		if( pUIChatMacro == NULL )
			continue;

		pChatMacro		=	pUIChatMacro->GetChatMacro();
		pChatMacroLDT	=	pUIChatMacro->GetChatMacroLDT();

		if( pChatMacro == NULL || pChatMacroLDT == NULL )
			continue;

		if( pChatMacroLDT->m_iRequireItem != iID )
			continue;

		UpdateIconSlot( i );
	}
}

void SPWindowQuickSlot::UpdateIconChatMacro( LPARAM lParam )
{
	CHAT_MACRO_POS*	pChatMacroPos	=	(CHAT_MACRO_POS*)lParam;

	if( pChatMacroPos == NULL )
		return;

	int		iSlotCount			=	QUICK_X		*	QUICK_Y;
	int		iSlotTotalCount		=	iSlotCount	*	QUICK_HOTKEY_COUNT;

	int		iSlotUnitStartNum	=	m_iCurPage			*	iSlotCount;
	int		iSlotUnitEndNum		=	iSlotUnitStartNum	+	iSlotCount;

	for( int i = iSlotUnitStartNum ; i < iSlotUnitEndNum ; ++i )
	{
		if( m_QuickSlot[ i ].LinkedData.iObjTypeID == pChatMacroPos->m_eType && m_QuickSlot[ i ].LinkedData.iSubTypeID == pChatMacroPos->m_iIndex )
			UpdateIconSlot( i );
	}
}

void SPWindowQuickSlot::BtnCheck( int iNum )
{
	if( iNum >= QUICK_HOTKEY_COUNT )
		return;

	m_bHotKey	=	TRUE;

	SPDragnDropManager::GetInstance()->End();

	m_iCurPage	=	iNum;

	//////////////////////////////////////////////////////////////////////////

	BOOL	bHotKey[ QUICK_HOTKEY_COUNT ]	=	{0,};

	bHotKey[ m_iCurPage ]	=	TRUE;

	SPWindow*	pWindow;

	for( int i = 0 ; i < QUICK_HOTKEY_COUNT ; ++i )
	{
		pWindow	=	Find( WIID_QUICK_SLOT_1LINE_HOTKEY_1 + i );

		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_BTN_SELECT , bHotKey[ i ] );

		pWindow	=	Find( WIID_QUICK_SLOT_1LINE_HOTKEY_1 + WIID_QUICK_SLOT_HOTKEY_DIFF + i );

		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_BTN_SELECT , bHotKey[ i ] );
	}

	//////////////////////////////////////////////////////////////////////////

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
		OnCursorMove( 0 , MAKELONG( iCursorX , iCursorY ) );

	UpdateIconPage( m_iCurPage );
}

void SPWindowQuickSlot::LineCheck( int iNum )
{
	if( iNum >= QUICK_Y )
		return;

	//////////////////////////////////////////////////////////////////////////

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer != NULL )
	{
		pLocalPlayer->SetQuickSlotSize( iNum );
	}

	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_BUFF , SPIM_POS_UPDATE , NULL , MAKELONG( 201 , ( 40 + 34 * iNum ) ) );

	int	iYSize	=	48;

	if( iNum == 1 )
		iYSize	+=	34;

	SetRectSize( m_iSX , iYSize );

	//////////////////////////////////////////////////////////////////////////

	SPDragnDropManager::GetInstance()->End();

	m_iCurLine	=	iNum;

	//////////////////////////////////////////////////////////////////////////

	bool	bShow	=	false;

	if( m_iCurLine == 1 )
		bShow = true;

	int			i;
	SPWindow*	pWindow;

	//////////////////////////////////////////////////////////////////////////

	for( i = 0 ; i <= WIID_QUICK_SLOT_2LINE_COUNT ; ++i )
	{
		pWindow	=	Find( WIID_QUICK_SLOT_2LINE_START + i );

		if( pWindow != NULL )
		{
			if( bShow == true )
				pWindow->Show();
			else
				pWindow->Hide();
		}
	}

	//////////////////////////////////////////////////////////////////////////

	for( i = 0 ; i <= WIID_QUICK_SLOT_1LINE_COUNT ; ++i )
	{
		pWindow	=	Find( WIID_QUICK_SLOT_1LINE_START + i );

		if( pWindow != NULL )
		{
			if( bShow == false )
				pWindow->Show();
			else
				pWindow->Hide();
		}
	}

	//////////////////////////////////////////////////////////////////////////

	SPUIUnit*	pUIUnit	=	NULL;
	int			iCount	=	QUICK_X * QUICK_Y;

	for( i = QUICK_X ; i < iCount ; ++i )
	{
		pUIUnit	=	m_UINoneUnitManager.GetUnit( i );

		if( pUIUnit != NULL )
		{
			if( bShow == true )
				pUIUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			else
				pUIUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		}

		pUIUnit	=	m_UIItemUnitManager.GetUnit( i );

		if( pUIUnit != NULL )
		{
			if( bShow == true )
				pUIUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			else
				pUIUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		}

		pUIUnit	=	m_UISkillUnitManager.GetUnit( i );

		if( pUIUnit != NULL )
		{
			if( bShow == true )
				pUIUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			else
				pUIUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		}

		pUIUnit	=	m_UIChatMacroUnitManager.GetUnit( i );

		if( pUIUnit != NULL )
		{
			if( bShow == true )
				pUIUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			else
				pUIUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		}
	}

	//////////////////////////////////////////////////////////////////////////

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
		OnCursorMove( 0 , MAKELONG( iCursorX , iCursorY ) );

	UpdateIconPage( m_iCurPage );
}

SPUIUnitManager* SPWindowQuickSlot::GetUIUnitManager( INSTANCE_ID iID )
{
	SPUIUnitManager*	pUIManager = NULL;

	switch( iID )
	{
	case WIID_QUICK_SLOT:	pUIManager	=	&m_UINoneUnitManager;		break;
	case WIID_SKILL:		pUIManager	=	&m_UISkillUnitManager;		break;
	case WIID_ITEM:			pUIManager	=	&m_UIItemUnitManager;		break;
	case WIID_CHAT_MACRO:	pUIManager	=	&m_UIChatMacroUnitManager;	break;
	}

	return pUIManager;
}

void SPWindowQuickSlot::InitActionKey()
{
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_01_01 , SPKL_LCTRL	);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_01_02 , SPKL_A		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_01_03 , SPKL_S		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_01_04 , SPKL_D		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_01_05 , SPKL_F		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_01_06 , SPKL_LALT	);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_01_07 , SPKL_LSHIFT	);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_01_08 , SPKL_Z		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_01_09 , SPKL_X		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_01_10 , SPKL_C		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_01_11 , SPKL_V		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_01_12 , SPKL_SPACE	);

	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_01 , SPKL_1		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_02 , SPKL_2		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_03 , SPKL_3		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_04 , SPKL_4		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_05 , SPKL_5		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_06 , SPKL_6		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_07 , SPKL_7		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_08 , SPKL_8		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_09 , SPKL_9		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_10 , SPKL_0		);
	g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_11 , SPKL_MINUS	);

	SITE_TYPE	eSiteType	=	SPLocalizeManager::GetInstance()->GetSiteType();

	if( eSiteType == SITE_GAMEPOT || eSiteType == SITE_ATGAMES )
	{
		g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_12 , SPKL_PREVTRACK );
	}
	else
	{
		g_pInputManager->SetActionMap( KEY_AM_QUICKKEY_02_12 , SPKL_EQUALS );
	}
}

void SPWindowQuickSlot::ProcessActionKey( float fTime )
{
	if(g_pInputManager->GetJoyPadManager()->IsKeyPass())
		return;

	SPMInputKey*	pActionKey	=	NULL;
	SPUIUnit*		pUIUnit		=	NULL;
	DWORD			dwInputType	=	0;

	SPMInputBtn*	pActionJS	=	NULL;

	for( int i = 0 ; i < QUICK_X * QUICK_Y ; ++i )
	{
		pActionKey	=	g_pInputManager->GetActionMap( KEY_ACTION_MAP( ( (int)KEY_AM_QUICKKEY_01_01 ) + i ) );
		pUIUnit		=	m_UIQuickSlotUnitManager.GetUnit( i );

		if( pUIUnit == NULL || pActionKey == NULL )
			continue;

		dwInputType	=	INPUT_PRESS_REPEAT;

		if( pUIUnit->GetInstanceID() == WIID_ITEM || pUIUnit->GetInstanceID() == WIID_CHAT_MACRO )
			dwInputType	=	INPUT_PRESS;

		if( pActionKey->InputState == INPUT_UNPRESS )
		{
			m_fInputPressRepeatTime[ i ]	=	0.0f;
			m_bChatMessage[ i ]				=	false;
		}

		if( pActionKey->InputState == dwInputType )
		{
			if( g_pInterfaceManager->GetIMEPool()->IsFocus() == TRUE )
				continue;

			if( g_pInterfaceManager->IsShowLooting() == TRUE )
			{
				if( pActionKey->Key == SPKL_SPACE || pActionKey->Key == SPKL_LSHIFT )
					continue;
			}

			if( m_fInputPressRepeatTime[ i ] >= 60.0f )
			{
				if( m_bChatMessage[ i ] == false )
				{
					m_bChatMessage[ i ] = true;

					D3DXCOLOR		stColor			= g_pResourceManager->GetGlobalColor( 9 );
					const char*		szGlobalString	= g_pResourceManager->GetGlobalString( 4006001 );

					if( szGlobalString != NULL )
					{
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szGlobalString , (LPARAM)&stColor );
					}
				}

				continue;
			}

			pUIUnit->Use();

			m_fInputPressRepeatTime[ i ]	+=	fTime;
		}
	}		
}


void SPWindowQuickSlot::ProcessJoyPad(float fTime)
{
	//[2007/5/17]-jinhee
	if(!g_pInput->IsEquipJoystick()) 
		return;

	if(g_pInputManager->GetJoyPadManager()->GetJoyStickMode() == JOYSTICK_MODE_NULL)
		return;

	SPInputStructJoyStick* pStick = g_pInputManager->GetInputStructJoyStick();
	if(pStick == NULL)	return;

	if(g_pInputManager->GetJoyPadManager()->GetJoyStickMode() == JOYSTICK_MODE_ANALOG){
		if(!m_bJoyPadZPress && pStick->ibLZ.iValue <= -1000) {			
			BtnCheck( 0 );
			m_bJoyPadZPress = true;
		}
		else if(!m_bJoyPadZPress && pStick->ibLZ.iValue >= 1000) {			
			BtnCheck( 1 );
			m_bJoyPadZPress = true;
		}
		else if(!m_bJoyPadZPress && pStick->ibLRz.iValue >= 1000) {
			BtnCheck( 0 );
			m_bJoyPadZPress = true;
		}
		else if(!m_bJoyPadZPress && pStick->ibLRz.iValue <= -1000) {
			BtnCheck( 1 );
			m_bJoyPadZPress = true;
		}
		else if((pStick->ibLZ.iValue > -1000 && pStick->ibLZ.iValue < 1000) &&
			(pStick->ibLRz.iValue > -1000 && pStick->ibLRz.iValue < 1000))
		{
			//DXUTOutputDebugString("\t * ibLZ[%d] - ZPress[%d]\n", pStick->ibLZ.iValue, m_bJoyPadZPress);
			m_bJoyPadZPress = false;
		}		
	}

	SPUIUnit*		pUIUnit		=	NULL;
	DWORD			dwInputType	=	0;

	std::vector<SPJoyPadButtonInfo*>* pvButton = g_pInputManager->GetJoyPadManager()->GetPadButton(PAD_BUTTON_KEY_TYPE_QUICK);
	std::vector<SPJoyPadButtonInfo*>::iterator iter;
	iter = pvButton->begin();

	int iKeyIndex = 0xFFFFFFFF;
	for(; iter != pvButton->end(); ++iter) {
		if((*iter)->m_iKey > BUTTON_KEY_QUICK_S12) {
			if((*iter)->m_iInputState == INPUT_REPEAT)
				BtnCheck((*iter)->m_iKey - BUTTON_KEY_QUICK_F1);
			continue;
		}

		iKeyIndex = (*iter)->m_iKey - BUTTON_KEY_QUICK_START;
		pUIUnit = m_UIQuickSlotUnitManager.GetUnit( iKeyIndex );
		if(pUIUnit == NULL)
			continue;

		if((*iter)->m_iInputState == INPUT_REPEAT) {
			dwInputType	=	INPUT_REPEAT;
		}
		else if((*iter)->m_iInputState ==  INPUT_UNREPEAT )
		{
			m_fPadPressRepeatTime[ iKeyIndex ]	=	0.0f;
			//m_bChatMessage[ iKeyIndex ]				=	false;
			(*iter)->m_bPush						=	false;
		}

		if((*iter)->m_iInputState == dwInputType )
		{
			if( g_pInterfaceManager->GetIMEPool()->IsFocus() == TRUE )
				continue;

			if( g_pInterfaceManager->IsShowLooting() == TRUE )
			{
				//if( pActionJS->iButton == SPKL_SPACE || pActionJS->iButton == SPKL_LSHIFT )
				//	continue;
			}

			if( m_fPadPressRepeatTime[ iKeyIndex ] >= 60.0f )
			{
				if( m_bChatMessage[ iKeyIndex ] == false )
				{
					m_bChatMessage[ iKeyIndex ] = true;

					D3DXCOLOR		stColor			= g_pResourceManager->GetGlobalColor( 9 );
					const char*		szGlobalString	= g_pResourceManager->GetGlobalString( 4006001 );

					if( szGlobalString != NULL )
					{
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szGlobalString , (LPARAM)&stColor );
					}
				}

				continue;
			}
			
			if( pUIUnit->GetInstanceID() == WIID_ITEM ) {
				if((*iter)->m_bPush == false)
					pUIUnit->Use();
			}
			else 
				pUIUnit->Use();

			(*iter)->m_bPush = true;

			m_fPadPressRepeatTime[ iKeyIndex ]	+=	fTime;
		}
	}
}


void SPWindowQuickSlot::InitHotKey()
{
	g_pInputManager->SetActionMap( KEY_AM_QUICKHOTKEY_01	, SPKL_F1	);
	g_pInputManager->SetActionMap( KEY_AM_QUICKHOTKEY_02	, SPKL_F2	);

	m_bHotKey	=	FALSE;
}

void SPWindowQuickSlot::ProcessHotKey()
{
	if(g_pInputManager->GetJoyPadManager()->IsKeyPass())
		return;

	SPMInputKey*	pHotKey	=	NULL;

	for( int i = 0 ; i < QUICK_HOTKEY_COUNT ; ++i )
	{
		pHotKey	=	g_pInputManager->GetActionMap( KEY_ACTION_MAP( ( (int)KEY_AM_QUICKHOTKEY_01 ) + i ) );

		if( pHotKey == NULL )
			continue;

		if( pHotKey->InputState == INPUT_PRESS_REPEAT )
		{
			if( g_pInterfaceManager->GetIMEPool()->IsFocus() == TRUE )
				continue;

			BtnCheck( i );
		}
	}

	m_bHotKey	=	FALSE;
}

void SPWindowQuickSlot::SwapQuickSlot( int iSourNum , int iDestNum )
{
	int			iQuickSlotCount		=	1;
	QUICK_SLOT*	pQuickSlot			=	NULL;

	int iQuickSlotStartNum	=	m_iCurPage	*	QUICK_X	*	QUICK_Y;
	int	iSourceQuickSlotNum	=	iSourNum	+	iQuickSlotStartNum;
	int iDestQuickSlotNum	=	iDestNum	+	iQuickSlotStartNum;

	if( iSourNum == -1 )
	{
		SPUIUnit*	pUIUnit	=	m_UIQuickSlotUnitManager.GetUnit( iDestNum );

		if( pUIUnit == NULL )
			return;

		QUICK_LINK_FLAG	eQuickLinkFlag		=	QUIKC_LINK_FLAG_INVALID;
		int				iQuickLinkObjTypeID	=	0;
		int				iQuickLinkSubTypeID	=	0;

		switch( pUIUnit->GetInstanceID() )
		{
		case WIID_ITEM:
			{
				SPUIItemUnit*	pUIItemUnit		=	(SPUIItemUnit*)pUIUnit;

				if( pUIItemUnit == NULL )
					return;

				eQuickLinkFlag		=	QUICK_LINK_FLAG_ITEM;
				iQuickLinkObjTypeID	=	pUIItemUnit->GetItemID();
			}
			break;
		case WIID_SKILL:
			{
				SPUISkillUnit*	pUISkillUnit	=	(SPUISkillUnit*)pUIUnit;

				if( pUISkillUnit == NULL )
					return;

				if( pUISkillUnit->GetType() == SIT_ACTION )
					eQuickLinkFlag	=	QUICK_LINK_FLAG_ACTION;
				else if( pUISkillUnit->GetType() == SIT_PET )
				{
					eQuickLinkFlag		=	QUICK_LINK_FLAG_ITEMSKILL;
					iQuickLinkSubTypeID	=	pUISkillUnit->GetItemID();
				}
				else if (SIT_NEWPET == pUISkillUnit->GetType())
				{
					eQuickLinkFlag		=	QUICK_LINK_FLAG_PETSKILL;
					iQuickLinkSubTypeID	=	pUISkillUnit->GetItemID();
				}
				else
					eQuickLinkFlag	=	QUICK_LINK_FLAG_SKILL;

				iQuickLinkObjTypeID	=	pUISkillUnit->GetSkillID();
			}
			break;
		case WIID_CHAT_MACRO:
			{
				SPUIChatMacro*	pUIChatMacroUnit		=	(SPUIChatMacro*)pUIUnit;

				if( pUIChatMacroUnit == NULL )
					return;

				CHAT_MACRO*		pChatMacro		=	pUIChatMacroUnit->GetChatMacro();
				CHAT_MACRO_LDT*	pChatMacroLDT	=	pUIChatMacroUnit->GetChatMacroLDT();

				if( pChatMacro == NULL || pChatMacroLDT == NULL )
					return;

				eQuickLinkFlag		=	QUICK_LINK_FLAG_CHAT;
				iQuickLinkObjTypeID	=	pChatMacroLDT->m_eType;
				iQuickLinkSubTypeID	=	pChatMacro->m_iIndex;
			}
			break;
		}

		pQuickSlot	=	new	QUICK_SLOT[ iQuickSlotCount ];
		pQuickSlot[0].iSlotIndex			=	iDestQuickSlotNum;
		pQuickSlot[0].LinkedData.iFlag		=	eQuickLinkFlag;
		pQuickSlot[0].LinkedData.iObjTypeID	=	iQuickLinkObjTypeID;
		pQuickSlot[0].LinkedData.iSubTypeID	=	iQuickLinkSubTypeID;
	}
	else if( iDestNum == -1 )
	{
		pQuickSlot		=	new QUICK_SLOT[ iQuickSlotCount ];
		pQuickSlot[0].iSlotIndex			=	iSourceQuickSlotNum;
		pQuickSlot[0].LinkedData.iFlag		=	QUIKC_LINK_FLAG_INVALID;
		pQuickSlot[0].LinkedData.iObjTypeID	=	0;
		pQuickSlot[0].LinkedData.iSubTypeID	=	0;
	}
	else
	{
		iQuickSlotCount	=	2;
		pQuickSlot		=	new QUICK_SLOT[ iQuickSlotCount ];

		pQuickSlot[ 0 ]	=	m_QuickSlot[ iSourceQuickSlotNum ];
		pQuickSlot[ 1 ]	=	m_QuickSlot[ iDestQuickSlotNum ];

		pQuickSlot[ 0 ].iSlotIndex	=	iDestQuickSlotNum;
		pQuickSlot[ 1 ].iSlotIndex	=	iSourceQuickSlotNum;
	}

	CPacket Packet;

	Packet.Add( (UINT32)CHAR_CS_SET_QUICK_SLOT );
	Packet.Add( (UINT8)iQuickSlotCount );
	Packet.Add( pQuickSlot , (int)sizeof( *pQuickSlot ) * iQuickSlotCount );

	g_pNetworkManager->SPPerformMessage( CHAR_CS_SET_QUICK_SLOT , 0 , (LPARAM)&Packet );

	SAFE_DELETE_ARRAY( pQuickSlot );

	//	AJJIYA [9/8/2005]
	//	사운드 추가
	if( iDestNum != -1 )
		g_pInterfaceManager->RegisterSoundUnit( "DATA/SOUND/SYSTEM_BUTTON_CLICK.WAV" );
}

void SPWindowQuickSlot::DeleteItemSlot( int iID )
{
	int	iTotalCount	=	QUICK_X * QUICK_Y * QUICK_HOTKEY_COUNT;

	for( int i = 0 ; i < iTotalCount ; ++i )
	{
		if( m_QuickSlot[ i  ].LinkedData.iObjTypeID == iID )
		{
			UpdateIconSlot( m_QuickSlot[i].iSlotIndex );
		}
	}
}

void SPWindowQuickSlot::DeleteSkillSlot( int iSaveCheck )
{
	int					iTotalCount		=	QUICK_X * QUICK_Y * QUICK_HOTKEY_COUNT;
	SPSkillManager*		pSkillManager	=	SPSkillManager::GetInstance();
	SPSkill*			pSkill			=	NULL;
	std::vector<int>	vClearSlots;

	for( int i = 0 ; i < iTotalCount ; ++i )
	{
		if( m_QuickSlot[i].LinkedData.iFlag == QUICK_LINK_FLAG_SKILL )
		{
			pSkill	=	pSkillManager->GetSkill( m_QuickSlot[i].LinkedData.iObjTypeID );

			if( pSkill != NULL )
			{
				if( iSaveCheck == SSC_SKILL )
				{
					if( pSkill->GetSaveCheck() == SSC_NULL )
						vClearSlots.push_back( m_QuickSlot[i].iSlotIndex );
					else
						UpdateIconSlot( m_QuickSlot[i].iSlotIndex );
				}
				else
				{
					if( pSkill->GetSaveCheck() == iSaveCheck )
						vClearSlots.push_back( m_QuickSlot[i].iSlotIndex );
				}
			}

		}
	}

	ClearQuickSlots( &vClearSlots );
}

void SPWindowQuickSlot::ClearQuickSlots( std::vector<int>* pClearSlots )
{
	if( pClearSlots == NULL )
		return;

	CPacket		Packet;
	QUICK_SLOT*	pQuickSlot			=	NULL;
	int			iQuickSlotCount		=	(int)pClearSlots->size();
	int			iQuickSlotNum;

	if( iQuickSlotCount <= 0 )
		return;

	pQuickSlot	=	new	QUICK_SLOT[ iQuickSlotCount ];

	for( int i = 0 ; i < iQuickSlotCount ; ++i )
	{
		iQuickSlotNum	=	pClearSlots->at( i );
		pQuickSlot[i].iSlotIndex			=	iQuickSlotNum;
		pQuickSlot[i].LinkedData.iFlag		=	QUIKC_LINK_FLAG_INVALID;
		pQuickSlot[i].LinkedData.iObjTypeID	=	0;
		pQuickSlot[i].LinkedData.iSubTypeID	=	0;
	}

	Packet.Add( (UINT32)CHAR_CS_SET_QUICK_SLOT );
	Packet.Add( (UINT8)iQuickSlotCount );
	Packet.Add( pQuickSlot , (int)sizeof( *pQuickSlot ) * iQuickSlotCount );

	g_pNetworkManager->SPPerformMessage( CHAR_CS_SET_QUICK_SLOT , 0 , (LPARAM)&Packet );

	SAFE_DELETE_ARRAY( pQuickSlot );
}

void SPWindowQuickSlot::CheckItemDrag()
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	if( pLocalPlayer->GetInvenArchive()->m_bPickup == true )
		pLocalPlayer->SPGOBPerformMessage( SPGM_INVEN_RECOVER_ICON );

}

//////////////////////////////////////////////////////////////////////////
//
//	퀵슬롯에서 사용될 이미지 미리 로딩한다.
//

void SPWindowQuickSlot::PreTextureLoad()
{
	//SPLDTFile*	pLDTFile	=	NULL;

	//g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_GLOBAL_RES , &pLDTFile );

	//if( pLDTFile == NULL )
	//	return;

	//LDT_Field	ldtFieldFileName;
	//SPTexture*	pTexture	=	NULL;

	//int		iRecordCount	= pLDTFile->GetItemCount();
	//int		iItemID;

	//for( int iRecord = 0 ; iRecord < iRecordCount ; ++iRecord )
	//{
	//	iItemID = pLDTFile->GetPrimaryKey( iRecord );

	//	if( iItemID < 10000000 || iItemID > 20000000 )
	//		continue;

	//	pLDTFile->GetFieldFromLable( iItemID, "_FILENAME",		ldtFieldFileName );

	//	if ( ldtFieldFileName.uData.pValue == NULL || (int)strlen( ldtFieldFileName.uData.pValue ) <= 0 )
	//		continue;

	//	g_pVideo->GetTextureMgr()->LoadTexture( ldtFieldFileName.uData.pValue , &pTexture );

	//	m_vpPreTexture.push_back( pTexture );

	//	pTexture	=	NULL;
	//}

	//SAFE_RELEASE( pLDTFile );
}

void SPWindowQuickSlot::PreTextureClean()
{
	STD_VECTOR_SPTEXTURE::iterator	Iter	=	m_vpPreTexture.begin();

	while( Iter != m_vpPreTexture.end() )
	{
		SAFE_RELEASE( (*Iter) );
		++Iter;
	}

	m_vpPreTexture.clear();
}

void SPWindowQuickSlot::ProcessUpdate( float fTime )
{
	if( m_bUpdate == true )
	{
		m_fAccumulateTime	+=	fTime;

		if( m_fAccumulateTime >= m_fLimitTime )
		{
			m_fAccumulateTime	=	0.0f;
			m_bUpdate			=	false;

			int	iUnitCount = m_UIQuickSlotUnitManager.GetUnitCount();
			SPUIUnit*		pUnit;
			SPUIItemUnit*	pItemUnit;

			for( int i = 0 ; i < iUnitCount ; ++i )
			{
				pUnit	=	m_UIQuickSlotUnitManager.GetUnit( i );

				if( pUnit == NULL || pUnit->GetInstanceID() != WIID_ITEM )
					continue;

				pItemUnit	=	(SPUIItemUnit*)pUnit;

				if( pItemUnit == NULL )
					continue;

				pItemUnit->CheckCount();
			}
		}
	}
}

void SPWindowQuickSlot::ReposOwnImage()
{
	SetRect( &m_rcBGDest[ 0 ]	, m_iAX					, m_iAY	, m_iAX + 2						, m_iAY + 36 );							//	배경 왼쪽
	SetRect( &m_rcBGDest[ 1 ]	, m_rcBGDest[ 0 ].right	, m_iAY	, m_rcBGDest[ 0 ].right + 410	, m_rcBGDest[0].bottom );				//	배경 가운데
	SetRect( &m_rcBGDest[ 2 ]	, m_rcBGDest[ 1 ].right	, m_iAY	, m_rcBGDest[ 1 ].right + 2		, m_rcBGDest[1].bottom );				//	배경 오른쪽
	SetRect( &m_rcBGDest[ 3 ]	, m_iAX + 371			, m_iAY + 36							, m_iAX + 371 + 43	, m_iAY + 36 + 12 );//	핫키 배경

	SetRect( &m_rcBGDest[ 4 ]	, m_iAX					, m_iAY + 34 + 2	, m_iAX + 2						, m_iAY + 34 + 2 + 34 );			//	배경 왼쪽
	SetRect( &m_rcBGDest[ 5 ]	, m_rcBGDest[ 4 ].right	, m_iAY	+ 34 + 2	, m_rcBGDest[ 4 ].right + 410	, m_rcBGDest[4].bottom );		//	배경 가운데
	SetRect( &m_rcBGDest[ 6 ]	, m_rcBGDest[ 5 ].right	, m_iAY + 34 + 2	, m_rcBGDest[ 5 ].right + 2		, m_rcBGDest[5].bottom );		//	배경 오른쪽
	SetRect( &m_rcBGDest[ 7 ]	, m_iAX + 371			, m_iAY + 34 + 36									, m_iAX + 371 + 43	, m_iAY + 34 + 36 + 12 );//	핫키 배경

	SetRect( &m_rtHotKeyDest[ 0 ] , m_iAX + 5	, m_iAY + 2			, m_iAX + 5 + 402 , m_iAY + 2 + 9		);
	SetRect( &m_rtHotKeyDest[ 1 ] , m_iAX + 4	, m_iAY + 34 + 2	, m_iAX + 4 + 402 , m_iAY + 34 + 2 + 9	);
}
