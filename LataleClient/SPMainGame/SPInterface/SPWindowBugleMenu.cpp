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

#include "SPWindowBugleSelect.h"
#include "SPWindowBugleMenu.h"
#include "SPWindowChat.h"

#include <tinyxml.h>
//--------------------------------------------------
static const int SLOT_HEIGHT = 16;
static const int SLOT_WIDTH = 120;

static const int ICON_OFFSET_X = 18;
static const int ICON_OFFSET_Y = 3;
static const int ICON_SIZE = 13;

static const int NAME_OFFSET_X = 43;
static const int NAME_OFFSET_Y = 3;
static const int NAME_SIZE_X = 48;
static const int NAME_SIZE_Y = 16;

static const int SETTING_OFFSET_X = 106;
static const int SETTING_OFFSET_Y = 3;
static const int SETTING_SIZE_X = 11;
static const int SETTING_SIEZ_Y = 12;

BugleInfo SPWindowBugleMenu::m_bugleInfo[SLOT_COUNT];
//--------------------------------------------------
SPWindowBugleMenu::SPWindowBugleMenu(WND_ID wndClassID, INSTANCE_ID instanceID)
: SPWindow( wndClassID, instanceID )
{
	Init();
}
SPWindowBugleMenu::SPWindowBugleMenu(INSTANCE_ID instanceID, 
									 int iX, int iY, int iCX, int iCY, 
									 SPWindow *pParent /* = NULL  */)
: SPWindow( WNDID_BUGLE_SELECT, instanceID, iX, iY, iCX, iCY, pParent )
{
	Init();
}
//--------------------------------------------------
SPWindowBugleMenu::~SPWindowBugleMenu( void )
{
	_saveBugleMenuSetting();
	Clean();
}
//--------------------------------------------------
void SPWindowBugleMenu::Init( void )
{
	//--------------------------------------------------
	memset( m_pVecBugleIcon, 0, SLOT_COUNT );
	memset( m_pVecBugleName, 0, SLOT_COUNT );
	memset( m_pVecBugleSetting, 0, SLOT_COUNT );
	memset( m_pVecSlotBG, 0, SLOT_COUNT );
	m_iCurSlot = 0;
	//--------------------------------------------------
	m_strToolTip = "[SPWindowBugleMenu]";

	if (false == _initGUI())
	{
		assert( false && "GUI init failed." );
	}
	//--------------------------------------------------
	if (!_loadBugleMenuSetting())
	{/* 如果存储的配置加载失败,则从LDT里读取前四个喇叭 */
		for (int idx = 0; idx < SLOT_COUNT; ++idx)
		{
			m_iCurSlot = idx;
			if (idx < (int)SPWindowBugleSelect::m_vecBugleInfo.size())
				SetSlotBugleInfo( SPWindowBugleSelect::m_vecBugleInfo[idx] );
		}
	}
	m_iCurSlot = 0;
}
//--------------------------------------------------
void SPWindowBugleMenu::Clean( void )
{
	SPWindow::Clean();
}
//--------------------------------------------------
void SPWindowBugleMenu::Show(bool bWithChild /* = true  */)
{
	if (m_bShow)
	{
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	}
	SPWindow::Show( bWithChild );
}
void SPWindowBugleMenu::Hide(bool bSendServer /* = true  */)
{
	if (m_bShow)
	{
		g_pInterfaceManager->RegisterSoundUnit( "DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	}

	SPWindow *ptrWnd = 
		g_pInterfaceManager->FindWindow( WIID_BUGLE_SELECT );
	if (ptrWnd)
		ptrWnd->Hide();

	SPWindow::Hide();
}
//--------------------------------------------------
void SPWindowBugleMenu::Process(float fDeltaTime)
{
	if (!m_bShow)
		return;

	SPWindow::Process( fDeltaTime );
}
void SPWindowBugleMenu::Render(float fDeltaTime)
{
	if (!m_bShow)
		return;

	SPWindow::RenderReverse( fDeltaTime );
}
void SPWindowBugleMenu::RefreshRelationCoord( void )
{
	SPWindow::RefreshRelationCoord();
}
//--------------------------------------------------
bool SPWindowBugleMenu::SetSlotBugleInfo( const BugleInfo &info )
{
	if (m_iCurSlot < 0 || m_iCurSlot >= SLOT_COUNT)
		return false;

	m_bugleInfo[m_iCurSlot] = info;
	_updateSlot( m_iCurSlot );
	_onBugleSelect( m_iCurSlot );

	return true;
}
bool SPWindowBugleMenu::_updateSlot( int slot_index )
{
	if (m_iCurSlot < 0 || m_iCurSlot >= SLOT_COUNT)
		return false;

	BugleInfo &info = m_bugleInfo[slot_index];
	m_pVecBugleIcon[slot_index]->SetImage( 
		info.m_strIconFilename.c_str(), info.m_iIconX, info.m_iIconY );
	m_pVecBugleName[slot_index]->SetWindowText( info.m_strName.c_str() );
	return true;
}
//--------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowBugleMenu)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_BUGLE_MENU_ICON_START, WIID_BUGLE_MENU_ICON_END, _onBugleSelectIconBtn )
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_BUGLE_MENU_NAME_START, WIID_BUGLE_MENU_NAME_END, _onBugleSelectNameBtn )
	
	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER, WIID_BUGLE_MENU_ICON_START, WIID_BUGLE_MENU_ICON_END, _onBugleIconFocus )
	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER, WIID_BUGLE_MENU_NAME_START, WIID_BUGLE_MENU_NAME_END, _onBugleNameFocus )
	
	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT, WIID_BUGLE_MENU_ICON_START, WIID_BUGLE_MENU_ICON_END, _onBugleIconLostFocus )
	SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT, WIID_BUGLE_MENU_NAME_START, WIID_BUGLE_MENU_NAME_END, _onBugleNameLostFocus )
	
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_BUGLE_MENU_SLOT_BG_START, WIID_BUGLE_MENU_SLOT_BG_END, _onBugleSelectBg )
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_BUGLE_MENU_SETTING_START, WIID_BUGLE_MENU_SETTING_END, _onBugleSettingBtn )
SPIMESSAGE_OBJ_MAP_END( SPWindow )
//--------------------------------------------------
static bool verifySlotIndex( int slot_index )
{
	if (slot_index < 0
		||
		slot_index >= SLOT_COUNT)
	{
		return false;
	}
	else
	{
		return true;
	}
}
int SPWindowBugleMenu::_onBugleSelectIconBtn(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int slot_index = iID - WIID_BUGLE_MENU_ICON_START;
	if (!verifySlotIndex( slot_index ))
		return 0;
	m_pVecSlotBG[slot_index]->SPSendMessage( SPIM_LBUTTON_UP );
	return 1;
}
int SPWindowBugleMenu::_onBugleSelectNameBtn(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int slot_index = iID - WIID_BUGLE_MENU_NAME_START;
	if (!verifySlotIndex( slot_index ))
		return 0;
	m_pVecSlotBG[slot_index]->SPSendMessage( SPIM_LBUTTON_UP );
	return 1;
}
int SPWindowBugleMenu::_onBugleSelectBg( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	int slot_index = iID - WIID_BUGLE_MENU_SLOT_BG_START;
	if (!verifySlotIndex( slot_index ))
		return 0;
	_onBugleSelect( slot_index );
	return 1;
}
//--------------------------------------------------
int SPWindowBugleMenu::_onBugleNameFocus( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	int slot_index = iID - WIID_BUGLE_MENU_NAME_START;
	if (!verifySlotIndex( slot_index ))
		return 0;
	m_pVecSlotBG[slot_index]->SPSendMessage( SPIM_CURSOR_ENTER );
	return 1;
}
int SPWindowBugleMenu::_onBugleIconFocus( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	int slot_index = iID - WIID_BUGLE_MENU_ICON_START;
	if (!verifySlotIndex( slot_index ))
		return 0;
	m_pVecSlotBG[slot_index]->SPSendMessage( SPIM_CURSOR_ENTER );
	return 1;
}
//--------------------------------------------------
int SPWindowBugleMenu::_onBugleNameLostFocus( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	int slot_index = iID - WIID_BUGLE_MENU_NAME_START;
	if (!verifySlotIndex( slot_index ))
		return 0;
	m_pVecSlotBG[slot_index]->SPSendMessage( SPIM_CURSOR_OUT );
	return 1;
}
int SPWindowBugleMenu::_onBugleIconLostFocus( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	int slot_index = iID - WIID_BUGLE_MENU_ICON_START;
	if (!verifySlotIndex( slot_index ))
		return 0;
	m_pVecSlotBG[slot_index]->SPSendMessage( SPIM_CURSOR_OUT );
	return 1;
}
//--------------------------------------------------
int SPWindowBugleMenu::_onBugleSettingBtn(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int slot_index = iID - WIID_BUGLE_MENU_SETTING_START;
	if (!verifySlotIndex( slot_index ))
		return 0;
	_onBugleSetting( slot_index );
	return 1;
}
//--------------------------------------------------
bool SPWindowBugleMenu::_initGUI( void )
{

	static const char bgFilename[] ="DATA/INTERFACE/CONCEPT/BUGLE.PNG";
	static const POINT bgPtSrc[4] =
	{
		{0,0},
		{0,16},
		{0,0},
		{0,16}
	};
	static const POINT bgSelPtSrc = { 0, 32 };

	for (int idx = 0; idx < SLOT_COUNT; ++idx)
	{
		m_pVecBugleIcon[idx] = new SPWindowButton( 
			WIID_BUGLE_MENU_ICON_START+idx, ICON_OFFSET_X, ICON_OFFSET_Y + SLOT_HEIGHT*idx,
			ICON_SIZE, ICON_SIZE, this );
		m_pVecBugleIcon[idx]->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE.PNG", 0, 355 );

		m_pVecBugleName[idx] = new SPWindowButton(
			WIID_BUGLE_MENU_NAME_START+idx, NAME_OFFSET_X, NAME_OFFSET_Y + SLOT_HEIGHT*idx,
			NAME_SIZE_X, NAME_SIZE_Y, this );
		m_pVecBugleName[idx]->SetWindowText( "请选择" );
		m_pVecBugleName[idx]->SetFormat( DT_LEFT );

		m_pVecBugleSetting[idx] = new SPWindowButton(
			WIID_BUGLE_MENU_SETTING_START+idx, SETTING_OFFSET_X, SETTING_OFFSET_Y + SLOT_HEIGHT*idx,
			SETTING_SIZE_X, SETTING_SIEZ_Y, this );
		m_pVecBugleSetting[idx]->SetImage( bgFilename, 199, 62 );
		m_pVecBugleSetting[idx]->SetImagePush( bgFilename, 199, 75 );
		m_pVecBugleSetting[idx]->SetImageHit( bgFilename, 199, 88 );
		m_pVecBugleSetting[idx]->SetImageDisable( bgFilename, 199, 101 );
		
		m_pVecSlotBG[idx] = new SPWindowButton(
			WIID_BUGLE_MENU_SLOT_BG_START+idx, 0, 0 + SLOT_HEIGHT*idx,
			SLOT_WIDTH, SLOT_HEIGHT, this );
		if (m_pVecSlotBG[idx])
		{
			m_pVecSlotBG[idx]->SetImage( bgFilename, bgPtSrc[idx].x, bgPtSrc[idx].y );
            m_pVecSlotBG[idx]->SetImagePush( bgFilename, bgPtSrc[idx].x, bgPtSrc[idx].y );
			m_pVecSlotBG[idx]->SetImageHit( bgFilename, bgSelPtSrc.x, bgSelPtSrc.y );
		}
	}
	return true;
}
//--------------------------------------------------
bool SPWindowBugleMenu::_onBugleSelect( int slot_index )
{
	if (!verifySlotIndex( slot_index ))
		return false;

	SPWindowBugleSelect *pWndBugleSelect = 
		static_cast< SPWindowBugleSelect* >(g_pInterfaceManager->FindWindow( WIID_BUGLE_SELECT ));
	if (!pWndBugleSelect)
		return false;

	pWndBugleSelect->Hide();
	Hide();
	SPWindowChat	*pWndChat = 
		static_cast< SPWindowChat* >(g_pInterfaceManager->FindWindow( WIID_CHAT ) );
	if (!pWndChat)
		return false;

	pWndChat->SetBugleInfo( m_bugleInfo[slot_index] );
	return true;
}
bool SPWindowBugleMenu::_onBugleSetting( int slot_index )
{
	if (slot_index < 0 || slot_index >= SLOT_COUNT)
		return false;

	SPWindowBugleSelect *pWndBugleSelect = 
		static_cast< SPWindowBugleSelect* >(g_pInterfaceManager->FindWindow( WIID_BUGLE_SELECT ));

	if (!pWndBugleSelect)
		return false;

	pWndBugleSelect->Show();
	m_iCurSlot = slot_index;
	return true;
}
//--------------------------------------------------
bool SPWindowBugleMenu::_loadBugleMenuSetting( void )
{
	TiXmlDocument xmlDoc;
	TiXmlElement	*pRoot = NULL;
	TiXmlElement	*pBugleNode = NULL;
	if (!xmlDoc.LoadFile( "UserOptions.xml", TIXML_ENCODING_UTF8 ))
		return false;

	pRoot = xmlDoc.FirstChildElement();
	if (!pRoot)
		return false;

	pBugleNode = pRoot->FirstChildElement( _T("Bugles") );
	if (!pBugleNode)
		return false;

	TiXmlElement	*pNode = NULL;
	int iItemID = -1;
	m_iCurSlot = 0;
	pNode = pBugleNode->FirstChildElement( _T("slot") );
	while (pNode && m_iCurSlot < SLOT_COUNT)
	{
		pNode->QueryIntAttribute( _T("ItemID"), &iItemID );
		BugleInfo info = SPWindowBugleSelect::GetBugleInfo( iItemID );
		SetSlotBugleInfo( info );

		m_iCurSlot++;
		pNode = pNode->NextSiblingElement();
	}
	m_iCurSlot = 0;

	return true;
}
bool SPWindowBugleMenu::_saveBugleMenuSetting( void )
{
	TiXmlDocument	xmlDoc;
	TiXmlElement	*pRoot = NULL;
	TiXmlElement	*pBugleNode = NULL;
	xmlDoc.LoadFile( "UserOptions.xml", TIXML_ENCODING_UTF8 );
	
	pRoot = xmlDoc.FirstChildElement();
	if (!pRoot)
	{
		pRoot = new TiXmlElement( _T("UserOptions") );
		xmlDoc.LinkEndChild( pRoot );
	}

	pBugleNode = pRoot->FirstChildElement( _T("Bugles") );
	if (!pBugleNode)
	{
		pBugleNode = new TiXmlElement( _T("Bugles") );
		pRoot->LinkEndChild( pBugleNode );
	}
    
	TiXmlElement	*pNode = NULL;
	pNode = pBugleNode->FirstChildElement( _T("slot") );
	int iIdx = 0;
	int iItemID = -1;
	while (pNode && iIdx < SLOT_COUNT)
	{
		iItemID = m_bugleInfo[iIdx].m_iItemShopID;
		pNode->SetAttribute( _T("ItemID"), iItemID );

		iIdx++;
		pNode = pNode->NextSiblingElement();
	}

	for (; iIdx < SLOT_COUNT; iIdx++)
	{
		iItemID = m_bugleInfo[iIdx].m_iItemShopID;
		pNode = new TiXmlElement( _T("slot") );
		pNode->SetAttribute( _T("ItemID"), iItemID );
		pBugleNode->LinkEndChild( pNode );
	}
	
	xmlDoc.SaveFile( "UserOptions.xml" );

	return false;
}
//--------------------------------------------------
//--------------------------------------------------
// end of file
