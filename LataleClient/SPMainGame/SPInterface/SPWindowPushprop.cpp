#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowEditMultiLine.h"
#include "SPWindowSlider.h"
#include "SPWindowGrid.h"
#include "SPWindowGridEx.h"

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

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPWindowToolTip.h"
#include "SPWindowPushprop.h"

#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "Packet.h"

#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPCashTradeManager.h"

#include "SPGOBManager.h"
#include "SPCommandConvert.h"

//--------------------------------------------------
vector< t_Quick_Buy_Info> SPWindowPushProp::m_vecQBInfo;	
const char *filename = "DATA/INTERFACE/CONCEPT/CNUI01.PNG";
//--------------------------------------------------
SPWindowPushProp::SPWindowPushProp(WND_ID wndClassID, INSTANCE_ID instanceID)
: SPWindow( wndClassID, instanceID )
{
	Init();
}
SPWindowPushProp::SPWindowPushProp(INSTANCE_ID instanceID, 
									 int iX, int iY, int iCX, int iCY, 
									 SPWindow *pParent /* = NULL  */)
									 : SPWindow( WNDID_PUSH_PROP, instanceID, iX, iY, iCX, iCY, pParent )
{
	Init();
}
//--------------------------------------------------
SPWindowPushProp::~SPWindowPushProp( void )
{
	Clean();
}
//--------------------------------------------------
void SPWindowPushProp::Init( void )
{
	m_ptrQB_Header = NULL;
	m_ptrQB_Body = NULL;
	m_ptrQB_Bottom = NULL;
	memset( m_ptrQB_SLOT, 0, QB_SLOT_NUMBER * sizeof(int) );
	memset( m_ptrQB_SLOT_BG, 0, QB_SLOT_NUMBER * sizeof(int) );
	m_ptrQB_RollUp = NULL;
	m_ptrQB_RollDown = NULL;
	m_vecQBInfo.clear();

	initQB_GUI();
}
void SPWindowPushProp::Clean( void )
{
	SPWindow::Clean();
}
//--------------------------------------------------
void SPWindowPushProp::Show(bool bWithChild /* = true  */)
{
	if (!g_bPushPropInited)
	{
		CPacket packet;
		packet.Add((UINT32)CASHSHOP_CS_GET_QUICK_BUY_INFO);
		g_pNetworkManager->SPPerformMessage( CASHSHOP_CS_GET_QUICK_BUY_INFO, 0, (LPARAM)&packet );
	}

	if (m_bShow)
	{
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	}
	SPWindow::Show( bWithChild );
}
void SPWindowPushProp::Hide(bool bSendServer /* = true  */)
{
	if (m_bShow)
	{
		g_pInterfaceManager->RegisterSoundUnit( "DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	}

	SPWindow::Hide();
}
//--------------------------------------------------
void SPWindowPushProp::Process(float fDeltaTime)
{
	if (!m_bShow)
		return;

	SPWindow::Process( fDeltaTime );
}
void SPWindowPushProp::Render(float fDeltaTime)
{
	return;
	if (!m_bShow)
		return;

	SPWindow::RenderReverse( fDeltaTime );
}
void SPWindowPushProp::RenderReverse(float fDeltaTime)
{
	return;
}
void SPWindowPushProp::SpecialRender( float fDeltaTime )
{
	if (!m_bShow)
		return;
	SPWindow::RenderReverse( fDeltaTime );
}
void SPWindowPushProp::RefreshChildPos( void )
{
	std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	for(;Iter != m_vpChildWindow.end(); ++Iter) {
		(*Iter)->RefreshChildPos();
	}
}
//--------------------------------------------------
//--------------------------------------------------
bool SPWindowPushProp::initQB_GUI( void )
{
	const int CELL_SIZE = 32;
	const int CELL_BETWEEN = 6;

	int ptXSlotStart = 6;
	int ptYSlotStart = 36;

	for (int idx = 0; idx < QB_SLOT_NUMBER; ++idx)
	{
		m_ptrQB_SLOT[idx] = new SPWindowStatic( WIID_ITEM_QB_SLOT_START+idx,
			ptXSlotStart, ptYSlotStart + idx * (CELL_SIZE+CELL_BETWEEN),
			CELL_SIZE, CELL_SIZE, this );
		if (m_ptrQB_SLOT[idx])
		{
			m_ptrQB_SLOT[idx]->SetImage( filename, 44, 1 ); 
		}
		else
			assert( false );
	}

	for (int idx = 0; idx < QB_SLOT_NUMBER; ++idx)
	{
		m_ptrQB_SLOT_BG[idx] = new SPWindowStatic( WIID_ITEM_QB_SLOT_BG_START+idx,
			ptXSlotStart, ptYSlotStart + idx * (CELL_SIZE+CELL_BETWEEN),
			CELL_SIZE, CELL_SIZE, this );
		if (m_ptrQB_SLOT_BG[idx])
		{
			m_ptrQB_SLOT_BG[idx]->SetImage( filename, 44, 1 ); 
		}
		else
			assert( false );
	}

	m_ptrQB_Header = new SPWindowStatic( WIID_ITEM_QB_HEADER, 0, 0, 42, 26, this );
	m_ptrQB_Header->SetImage( filename, 1, 1 );

	m_ptrQB_Body = new SPWindowStatic( WIID_ITEM_QB_BODY, 0, 26, 42, 245, this );
	m_ptrQB_Body->SetImage( filename, 1, 29 );
	m_ptrQB_Body->SetSrcSize( 38, 5 );

	m_ptrQB_Bottom = new SPWindowStatic( WIID_ITEM_QB_BOTTOM, 0, 271, 42, 5, this );
	m_ptrQB_Bottom->SetImage( filename, 1, 36 );
	return true;
}
//--------------------------------------------------
int	SPWindowPushProp::onQB_RollUp(WPARAM wParam, LPARAM lParam)
{
	return 1;
}
int SPWindowPushProp::onQB_RollDown(WPARAM wParam, LPARAM lParam)
{
	return 1;
}
//--------------------------------------------------
int SPWindowPushProp::onQB_TollTipEnter(int iID, WPARAM wParam, LPARAM lParam)
{
	int slot_index = iID - WIID_ITEM_QB_SLOT_START;
	if (slot_index < 0 || slot_index > QB_SLOT_NUMBER
		|| slot_index >= (int)m_vecQBInfo.size())
		return 0;

	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return 0;

	SPCashItemAttr* pkItemAttr = NULL;	
	t_Quick_Buy_Info &info = m_vecQBInfo[slot_index];
	pkItemAttr = g_pItemCluster->GetCashItem( info.m_iPackageID );
	if( pkItemAttr == NULL )
		return 0;

	pkWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)pkItemAttr, -15);
	return 1;
}
int SPWindowPushProp::onQB_TollTipMove(int iID, WPARAM wParam, LPARAM lParam)
{
	return onQB_TollTipEnter( iID, wParam, lParam );
}
int SPWindowPushProp::onQB_TollTipOut(int iID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, lParam);
	return 1;
}
//--------------------------------------------------
int SPWindowPushProp::onQB_SlotLBtnUp(int iID, WPARAM wParam, LPARAM lParam)
{
	int slot_index = iID - WIID_ITEM_QB_SLOT_START;
	if (slot_index < 0 || slot_index > QB_SLOT_NUMBER
		|| slot_index >= (int)m_vecQBInfo.size())
		return 0;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
	if( pLocalPlayer == NULL )
		return 0;

	if( g_pInterfaceManager->IsCutIn() )
		return 0;

	if( g_pGOBManager == NULL )
		return 0;

	if( pLocalPlayer->IsDead() )
		return 0;

	if( pLocalPlayer->GetCurAction() == ACTION_ENCHANTTING )
	{
		D3DXCOLOR	cColor	=	D3DXCOLOR((160.0f / 255.0f), (255.0f / 255.0f), (0.0f / 255.0f), 1.0f);
		g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)g_pResourceManager->GetGlobalString( 4000002 ) , (LPARAM)&cColor );
		return 0;
	}

	t_Quick_Buy_Info &info = m_vecQBInfo[slot_index];

	//SPCashTradeManager::GetInstance()->SetSelectCashItem( info.m_iPackageID, false );
	//SPCashTradeManager::GetInstance()->SendCashShopPrice(CASH_TRADE_TYPE_BUY);
	if (SPCashTradeManager::GetInstance()->SetSelectCashItem( info.m_iPackageID, false ))
		SPCashTradeManager::GetInstance()->SendCashShopPrice(CASH_TRADE_TYPE_BUY);

	return 1;
}
//--------------------------------------------------
//--------------------------------------------------
void SPWindowPushProp::OnQBInfoAck( CPacket* packet )
{
	if (!packet)
		return;

	UINT uInfoCount = 0;
	packet->ExtractUINT32( &uInfoCount );
	
	t_Quick_Buy_Info info;
	m_vecQBInfo.clear();
	for( UINT idx = 0; idx < uInfoCount; ++idx)
	{
		packet->ExtractStruct( &info, sizeof(t_Quick_Buy_Info) );
		m_vecQBInfo.push_back( info );
	}
	
	g_bPushPropInited = true;

	//--------------------------------------------------
	// render 
	for (int idx = 0; idx < (int)m_vecQBInfo.size() && idx < QB_SLOT_NUMBER; ++idx)
	{
		t_Quick_Buy_Info &info = m_vecQBInfo[idx];
		SPCashItemAttr *attr = g_pItemCluster->GetCashItem( info.m_iPackageID );
		if (!attr)
			continue;

		POINT ptIconSrcPos;
		SPTexture *tex =
			g_pItemCluster->GetItemTextureFromTextureInx( attr->iIcon, attr->iIconIndex, ptIconSrcPos );
		assert( tex );
		m_ptrQB_SLOT[idx]->SPSendMessage( SPIM_SET_IMAGECLEAR );
		if (tex)
		{
			m_ptrQB_SLOT[idx]->SetImage( tex, ptIconSrcPos.x, ptIconSrcPos.y );
		}
		else
		{
			m_ptrQB_SLOT[idx]->SetImage( filename, 44, 1 ); 
		}
	}
	//--------------------------------------------------
}
//--------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowPushProp )
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,			WIID_ITEM_QB_ROLLUP,		onQB_RollUp )
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,			WIID_ITEM_QB_ROLLDOWN,		onQB_RollDown )
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_ITEM_QB_SLOT_START, WIID_ITEM_QB_SLOT_END, onQB_SlotLBtnUp )
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_ITEM_QB_SLOT_START, WIID_ITEM_QB_SLOT_END,	onQB_TollTipEnter )
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_ITEM_QB_SLOT_START, WIID_ITEM_QB_SLOT_END,	onQB_TollTipOut	)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_ITEM_QB_SLOT_START, WIID_ITEM_QB_SLOT_END,	onQB_TollTipMove )
SPIMESSAGE_OBJ_MAP_END( SPWindow )
//--------------------------------------------------
//--------------------------------------------------