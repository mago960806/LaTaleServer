#include "SPCommon.h"
#include "SPUtil.h"
#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"
#include "SPItemAttr.h"
#include "SPManager.h"
#include "SPItem.h"
#include "SPItemCluster.h"
#include "SPLocalizeManager.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPGOBManager.h"
#include "SPWindowGreenerGuide.h"
#include "SPQuestManager.h"
#include "SPQuestArchive.h"
#include "SPQuestAttr.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPCommandConvert.h"
#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"
#include "SPPlayer.h"
#include "SPUIUnit.h"
#include "SPUIItemUnit.h"
#include "SPUIUnitManager.h"
#include <assert.h>
//--------------------------------------------------
const char *notify = "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG";
const int sizeX = 89;
const int sizeY = 62;
const int COUNT = 4;
const int LINE = 2;
const int startX = 1;
const int startY = 235;
const int offset = 1;
//--------------------------------------------------
enum FETCHSTATUS
{
	FETCHFAILED = 0,
	FETCHING	= 1,
	FETCHSUCCESS = 2
};
//--------------------------------------------------
SPWindowGreenerGuide::SPWindowGreenerGuide(WND_ID wndClassID, INSTANCE_ID instanceID)
: SPWindow( wndClassID, instanceID )
{
	Init();
}
SPWindowGreenerGuide::SPWindowGreenerGuide(INSTANCE_ID instanceID, 
										   int iX, int iY, int iCX, int iCY, 
										   SPWindow *pParent /* = NULL  */)
										   : SPWindow( WNDID_GREENERGUIDE, instanceID, iX, iY, iCX, iCY, pParent )
{
	Init();
}
//--------------------------------------------------
SPWindowGreenerGuide::~SPWindowGreenerGuide( void )
{
	Clean();
	for (int idx = 0; idx < GG_GIFT_COUNT; ++idx)
	{
		SAFE_DELETE( m_pC_itemGift[idx] );
	}
}
//--------------------------------------------------
void SPWindowGreenerGuide::Init( void )
{
	m_currStatus = GREENERGUIDE_NULL;
	m_pC_btnClose = NULL;
	m_vecGreenerGuideData.clear();
	m_iCurrLevel = -1;
	m_iToLevel = -1;
	m_fAccuTimeNofity = 0.0f;
	SPWindow::Show( true );

	if (!_initGUI())
		assert( false );

	if (!_loadLdtCfg())
		assert( false );

	changeStatus( GREENERGUIDE_NULL );
}
//--------------------------------------------------
void SPWindowGreenerGuide::Clean( void )
{
	SPWindow::Clean();
}
//--------------------------------------------------
void SPWindowGreenerGuide::Show(bool bWithChild /* = true  */)
{
	m_bShow = false;
	return;
	if (m_bShow)
	{
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	}
	SPWindow::Show( true );
	changeStatus( m_currStatus );
}
void SPWindowGreenerGuide::Hide(bool bSendServer /* = true  */)
{
	m_bShow = false;
	if (m_bShow)
	{
		g_pInterfaceManager->RegisterSoundUnit( "DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
	}
	SPWindow::Hide();
}
//--------------------------------------------------
void SPWindowGreenerGuide::Process(float fDeltaTime)
{
	if (m_pWndIcon && m_pWndIcon->IsShow())
		m_fAccuTimeNofity += fDeltaTime;

	SPWindow::Process( fDeltaTime );
}
void SPWindowGreenerGuide::Render(float fDeltaTime)
{
	static int NotifyIndex = 0;
	if (m_pWndIcon && m_pWndIcon->IsShow() && GREENERGUIDE_ICON == m_currStatus)
	{
		if (m_fAccuTimeNofity > 0.15f)
		{
			NotifyIndex++;
			if (NotifyIndex >= COUNT * LINE)
				NotifyIndex = 0;
			m_pI_Nofitication->SPSendMessage( SPIM_SET_IMAGEINDEX , NotifyIndex );
		//	m_pWndIcon->RenderReverse( fDeltaTime );
			m_fAccuTimeNofity = 0.0f;
		}
		m_pWndIcon->RenderReverse( fDeltaTime );
	}
	m_pWndContent->RenderReverse( fDeltaTime );
	if (m_pWndContent->IsShow())
	{
		for (int idx = 0; idx < GG_GIFT_COUNT; ++idx)
		{
			m_pC_itemGift[idx]->Render();
		}
	}
	m_pWndWaiting->Render( fDeltaTime );
}
void SPWindowGreenerGuide::RefreshRelationCoord( void )
{
	for (int idx = 0; idx < GG_GIFT_COUNT; ++idx)
	{
		m_pC_itemGift[idx]->SetPos( m_iAX+36+45*idx, m_iAY+260 );
		m_pC_itemGift[idx]->ProcessCount();
	}

	SPWindow::RefreshRelationCoord();
}
static void _initTxtObject( SPWindowStatic *obj )
{
	if (obj)
	{
		obj->SetFontColor(RGBA(53, 67, 78, 255));
		obj->SetFormat(DT_LEFT | DT_WORDBREAK);
		obj->SetTextPeriod(false) ;
		obj->SetMultiLine( false, 0 );
	}
}
bool SPWindowGreenerGuide::_initGUI( void )
{
	int iIDNoName = WIID_GREENERGUIDE_NONAME;
	m_pWndIcon = new SPWindowStatic( iIDNoName++, 0, 0, 0, 0, this );
	m_pWndContent = new SPWindowStatic( iIDNoName++, 0, 0, 0, 0, this );
	m_pWndWaiting = new SPWindowStatic( iIDNoName++, 0, 0, 0, 0, this );

	// fetching img
	m_pImgWaitingMask = new SPWindowStatic( iIDNoName++, 0, 0, 306, 335, m_pWndWaiting ); 
	m_pImgWaitingMask->SetImage("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 438, 47 );
	m_pImgWaitingMask->SetSrcSize( 2, 2 );

	m_pImgWaitingBG = new SPWindowStatic( iIDNoName++, 21, 87, 254, 110, m_pWndWaiting );	
	m_pImgWaitingBG->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 1, 123 );

	m_pImgStatus = new SPWindowStatic( WIID_GREENERGUIDE_PIC_STATUS, 21+58, 87+55, 125, 23, m_pWndWaiting );
	m_pImgStatus->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 258, 123 );
	m_pImgStatus->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 258, 148 );
	m_pImgStatus->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 258, 172 );

	// icon
	m_pI_Nofitication = new SPWindowStatic( WIID_GREENERGUIDE_NOTIFICATION, 0, 0, sizeX, sizeY, m_pWndIcon );
	//m_pI_Nofitication->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 0, 0 );
	for (int y = 0; y < LINE; ++y)
	{
		for (int x = 0; x < COUNT; ++x)
		{
			m_pI_Nofitication->SetImage( notify, startX + x * (sizeX+offset), startY + y *(sizeY+offset) );
		}
	}

	// content
	m_pC_btnClose = new SPWindowButton( WIID_GREENERGUIDE_CLOSE, 287, 10, 12, 12, m_pWndContent ); 
	m_pC_btnClose->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 217 );
	m_pC_btnClose->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 230 );
	m_pC_btnClose->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 244 );
	m_pC_btnClose->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 256 );


	m_pC_btnOK = new SPWindowButton( WIID_GREENERGUIDE_OK, 166, 305, 66, 17, m_pWndContent );
	m_pC_btnOK->SetImage("DATA/INTERFACE/CONCEPT/cashshop02.png", 322, 280);
	m_pC_btnOK->SetImageHit("DATA/INTERFACE/CONCEPT/cashshop02.png", 322, 298);
	m_pC_btnOK->SetImagePush("DATA/INTERFACE/CONCEPT/cashshop02.png", 322, 316);
	m_pC_btnOK->SetImageDisable("DATA/INTERFACE/CONCEPT/cashshop02.png", 322, 334);

	m_pC_btnFetchAward = new SPWindowButton( WIID_GREENERGUIDE_FETCHAWARD, 71, 305, 68, 17, m_pWndContent );
	m_pC_btnFetchAward->SetImage("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 368, 46);
	m_pC_btnFetchAward->SetImageHit("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 368, 64);
	m_pC_btnFetchAward->SetImagePush("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 368, 82);
	m_pC_btnFetchAward->SetImageDisable("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 368, 100);

	m_pC_txtInfo = new SPWindowStatic( WIID_GREENERGUIDE_TXFINFO, 36, 57, 262, 44, m_pWndContent );
	m_pC_txtInfo->SetFontColor(RGBA(53, 67, 78, 255));
	m_pC_txtInfo->SetFormat(DT_LEFT | DT_WORDBREAK);
	m_pC_txtInfo->SetTextPeriod(false) ;
	m_pC_txtInfo->SetMultiLine( true, 1 );

	for (int idx = 0; idx < GG_MONSTER_COUNT; ++idx)
	{
		m_pC_txtMap[idx] = new SPWindowStatic( WIID_GREENERGUIDE_TXTMAP_BEGIN+idx, 36, 126+15*idx, 92, 15, m_pWndContent );
		m_pC_txtMonster[idx] = new SPWindowStatic( WIID_GREENERGUIDE_TXTMONSTER_BEGIN+idx, 140, 126+15*idx, 92, 15, m_pWndContent );
		m_pC_txtLevel[idx] = new SPWindowStatic( WIID_GREENERGUIDE_TXTLEVEL_BEGIN+idx, 235, 126+15*idx, 40, 15, m_pWndContent );
		_initTxtObject( m_pC_txtMap[idx] );
		_initTxtObject( m_pC_txtMonster[idx] );
		_initTxtObject( m_pC_txtLevel[idx] );
	}

	for (int idx = 0; idx < GG_QUEST_COUNT; ++idx)
	{
		m_pC_txtQuest[idx] = new SPWindowStatic( WIID_GREENERGUIDE_TXTQUEST_BEGIN+idx, 36, 193+15*idx, 262, 15, m_pWndContent );
		_initTxtObject( m_pC_txtQuest[idx] );
	}

	for (int idx = 0; idx < GG_GIFT_COUNT; ++idx)
	{
		m_pC_itemGift[idx] = new SPUIItemUnit;
		m_pC_itemGift[idx]->SetBGTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" );
		m_pC_itemGift[idx]->SetBGRectSrc( 400 , 20 , 432  , 52 );
		m_pC_itemGift[idx]->SetSize( 32 , 32 );
		m_pC_itemGift[idx]->SetDisableTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" );
		m_pC_itemGift[idx]->SetDisableRectSrc( 434 , 37 , 436 , 39 );
		m_pC_itemGift[idx]->SetCoolTimeTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" );
		m_pC_itemGift[idx]->SetCoolTimeRectSrc( 434 , 37 , 436 , 39 );
		m_pC_itemGift[idx]->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
		m_pC_itemGift[idx]->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		m_pC_itemGift[idx]->SetInstanceID( WIID_ITEM );
		m_pC_itemGift[idx]->SetPos( m_iAX+36+45*idx, m_iAY+260 );
	}

	m_pC_Gift = new SPWindowStatic( WIID_GREENERGUIDE_TXTGIFT, 36, 245, 262, 35, m_pWndContent );
	m_pC_Gift->SetFontColor(RGBA(53, 67, 78, 255));
	m_pC_Gift->SetFormat(DT_LEFT | DT_WORDBREAK);
	m_pC_Gift->SetTextPeriod(false) ;
	m_pC_Gift->SetMultiLine( true, 3 );
	
	m_pC_picInfo = new SPWindowStatic( iIDNoName++, 15, 40, 78, 19, m_pWndContent );
	m_pC_picInfo->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 2, 102 );

	m_pC_picMapMonster = new SPWindowStatic( iIDNoName++, 15, 107, 129, 19, m_pWndContent );
	m_pC_picMapMonster->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 82, 102 );

	m_pC_picQuest = new SPWindowStatic( iIDNoName++, 15, 171, 90, 19, m_pWndContent );
	m_pC_picQuest->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 213, 102 );

	m_pC_picGift = new SPWindowStatic( iIDNoName++, 15, 224, 59, 19, m_pWndContent );
	m_pC_picGift->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 305, 102 );

	m_pC_picELY = new SPWindowStatic( iIDNoName++, 36, 247, 22, 8, m_pWndContent );
	m_pC_picELY->SetImage( "DATA/INTERFACE/CONCEPT/UI102.PNG", 96, 135 );

	m_pC_txtELY = new SPWindowStatic( iIDNoName++, 88, 242, 60, 16, m_pWndContent );
	m_pC_txtELY->SetWindowText( "0" );

	m_pC_BgHeader = new SPWindowStatic( iIDNoName++, 0, 0, 306, 29, m_pWndContent );
	m_pC_BgHeader->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 1, 46 );

	m_pC_BgBody = new SPWindowStatic( iIDNoName++, 0, 28, 306, 297, m_pWndContent );
	m_pC_BgBody->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 1, 77 );
	m_pC_BgBody->SetSrcSize( 306, 6 ); 

	m_pC_BgBottom = new SPWindowStatic( iIDNoName++, 0, 325, 306, 13, m_pWndContent );
	m_pC_BgBottom->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 1, 87 );

	m_pWndIcon->Hide();
	m_pWndContent->Hide();
	
	return true;
}
static void safeStringCpy( char *des, const char *src, int max_length )
{
	if (src)
	{
		strncpy( des, src, max_length-1 );
		des[max_length-1] = '\0';
	}
	else
		des[0] = '\0';
}
bool SPWindowGreenerGuide::_loadLdtCfg( void )
{
	SPLDTFile	*pFile = NULL;
	g_pResourceManager->GetLDTFromFileName( RES_GREENER_GUIDE, &pFile );
	if (!pFile)
	{
		assert( false && "greener guide ldt read failed." );
		return false;
	}

	m_vecGreenerGuideData.clear();
	LDT_Field field;
	GreenerGuideData data;

	int iRecordCount = pFile->GetItemCount();
	int id = 0;
	char mob_buf[32] = {0};
	char moblevel_buf[32] = {0};
	char mobmap_buf[32] = {0};
	char quest_buf[32] = {0};
	char gift_buf[32] = {0};
	for (int idx = 0; idx < iRecordCount; ++idx)
	{
		id = pFile->GetPrimaryKey( idx );
		if (0 == id)
			continue;

		pFile->GetFieldFromLable( id, "_level", field );
		data.iPlayerLevel = field.uData.lValue;

		pFile->GetFieldFromLable( id, "_help", field );
		safeStringCpy( data.strHelp, field.uData.pValue, MAX_STRING_SIZE );

		for (int idx = 0; idx < GG_MONSTER_COUNT; ++idx)
		{
			_snprintf( mob_buf, 31, "_mob%d", idx+1 );
			_snprintf( moblevel_buf, 31, "_moblevel%d", idx+1 );
			_snprintf( mobmap_buf, 31, "_map%d", idx+1 );

			pFile->GetFieldFromLable( id, mob_buf, field );
			safeStringCpy( data.strMonster[idx], field.uData.pValue, MAX_STRING_SIZE ); 

			pFile->GetFieldFromLable( id, moblevel_buf, field );
			safeStringCpy( data.strMonsterLevel[idx], field.uData.pValue, MAX_STRING_SIZE );

			pFile->GetFieldFromLable( id, mobmap_buf, field );
			safeStringCpy( data.strMonsterMap[idx], field.uData.pValue, MAX_STRING_SIZE );
		}

		for (int idx = 0; idx < GG_QUEST_COUNT; ++idx)
		{
			_snprintf( quest_buf, 31, "_quest%d", idx+1 );
			pFile->GetFieldFromLable( id, quest_buf, field );
			data.iQuestID[idx] = field.uData.lValue;
		}

		pFile->GetFieldFromLable( id, "_skill", field );
		safeStringCpy( data.strSkill, field.uData.pValue, MAX_STRING_SIZE );

		pFile->GetFieldFromLable( id, "_commend", field );
		safeStringCpy( data.strCommend, field.uData.pValue, MAX_STRING_SIZE );

		for (int idx = 0; idx < GG_GIFT_COUNT; ++idx)
		{
			_snprintf( gift_buf, 31, "_giftA%d", idx+1 );
			pFile->GetFieldFromLable( id, gift_buf, field );
			data.iGiftBoy[idx] = field.uData.lValue;

			_snprintf( gift_buf, 31, "_giftA%dNO", idx+1 );
			pFile->GetFieldFromLable( id, gift_buf, field );
			data.iGiftBoyNum[idx] = field.uData.lValue;

			_snprintf( gift_buf, 31, "_giftB%d", idx+1 );
			pFile->GetFieldFromLable( id, gift_buf, field );
			data.iGiftGirl[idx] = field.uData.lValue;

			_snprintf( gift_buf, 31, "_giftB%dNO", idx+1 );
			pFile->GetFieldFromLable( id, gift_buf, field );
			data.iGiftGirlNum[idx] = field.uData.lValue;
		}

		pFile->GetFieldFromLable( id, "_ely", field );
		data.iEly = field.uData.lValue;

		if (data.iPlayerLevel != 0)
			m_vecGreenerGuideData.push_back( data );
		data.reset();
	}

	SAFE_RELEASE( pFile );
	return true;
}
//--------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowGreenerGuide)
	SPIMESSAGE_COMMAND( SPIM_ITEM_LEVEL, _onLevelUp )
	SPIMESSAGE_COMMAND( SPIM_GREENERGUIDE_EVENT, _onShowGuide )
	SPIMESSAGE_COMMAND( SPIM_GREENERGUIDE_FETCHAWARD, _onFetchAwardAck )
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP, WIID_GREENERGUIDE_FETCHAWARD, _onBtnFetchAward )
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP, WIID_GREENERGUIDE_OK, _onBtnOK )
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP, WIID_GREENERGUIDE_NOTIFICATION, _onBtnNotification )
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP, WIID_GREENERGUIDE_CLOSE, _onBtnOK )
SPIMESSAGE_OBJ_MAP_END(SPWindow)
//--------------------------------------------------
int SPWindowGreenerGuide::_onBtnFetchAward(WPARAM wParam, LPARAM lParam)
{
	if (GREENERGUIDE_FETCHING == m_currStatus)
		return 1;

	CPacket Packet;
	Packet.Add( (UINT32)GREENERGUIDE_CS_FETCHAWARD );
	Packet.AddUINT32( (UINT32)m_iCurrLevel );
	g_pNetworkManager->SPPerformMessage( GREENERGUIDE_CS_FETCHAWARD, 0, (LPARAM)&Packet );
	changeStatus( GREENERGUIDE_FETCHING );
	return 0;
}
int SPWindowGreenerGuide::_onBtnOK(WPARAM wParam, LPARAM lParam)
{
	if (m_iCurrLevel <= 0)
		Hide();
	else
		changeStatus( GREENERGUIDE_ICON );
	return 0;
}
int SPWindowGreenerGuide::_onBtnNotification(WPARAM wParam, LPARAM lParam)
{
	if (makeContent( m_iCurrLevel ))
	{
		changeStatus( GREENERGUIDE_CONTENT );
	}
	return 0;
}
//--------------------------------------------------
int SPWindowGreenerGuide::_onShowGuide(WPARAM wParam, LPARAM lParam)
{
	if (m_currStatus == GREENERGUIDE_FETCHING)
		return 0;
	UINT32 iToLevel, iFromLevel;
	CPacket *pPacket = (CPacket *)lParam;
	if (!pPacket)
		return -1;
	pPacket->ExtractUINT32( &iFromLevel );
	iToLevel = (UINT32)g_pGOBManager->GetLocalPlayer()->GetCheckStatusValue(CHECK_STATUS_LV);

	if (iToLevel < iFromLevel)
		return -1;

	if (iToLevel - iFromLevel >= 5)
		iFromLevel = iToLevel - 4;

	if (!isLevelExist( iFromLevel ))
		return -1;

	m_iCurrLevel = iFromLevel;
	m_iToLevel = iToLevel;
	Show(true);
	changeStatus( GREENERGUIDE_ICON );
	return 0;
}
int SPWindowGreenerGuide::_onFetchAwardAck(WPARAM wParam, LPARAM lParam)
{
	static const int NOTIFY_MAIL = 1;
	static const int NOTIFY_SUCCESS = 2;
	static const int FAILED = 0;
	static const int SUCCESSED = 1;

	UINT32 iFetchLevel;
	GLOBAL_STRING_ID ErrorID;
	UINT32 iNotifyCode;
	CPacket *pPacket = (CPacket *)lParam;
	if (!pPacket)
		return -1;

	pPacket->ExtractUINT32( &iFetchLevel );
	pPacket->ExtractUINT32( &ErrorID );
	pPacket->ExtractUINT32( &iNotifyCode );

	if (ErrorID)
	{
		if(g_pResourceManager->GetGlobalString(ErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		changeStatus( GREENERGUIDE_ICON );
		return 1;
	}

	const char *msg = g_pResourceManager->GetGlobalString( 400500 );
	if (msg)
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING, (WPARAM)msg );

	if (m_iCurrLevel < m_iToLevel)
	{
		m_iCurrLevel++;
		changeStatus( GREENERGUIDE_ICON );
	}
	else
	{
		m_iCurrLevel = -1;
		changeStatus( GREENERGUIDE_NULL );
	}

	return 0;
}
int SPWindowGreenerGuide::_onLevelUp(WPARAM wParam, LPARAM lParam)
{
	m_iToLevel = (UINT32)g_pGOBManager->GetLocalPlayer()->GetCheckStatusValue(CHECK_STATUS_LV);
	if (m_iCurrLevel <= 0)
	{
		m_iCurrLevel = (UINT32)g_pGOBManager->GetLocalPlayer()->GetCheckStatusValue(CHECK_STATUS_LV);
		if (!isLevelExist( m_iCurrLevel ))
		{
			m_iCurrLevel = -1;
			changeStatus( GREENERGUIDE_NULL );
			return 0;
		}
		Show(true);
		changeStatus( GREENERGUIDE_ICON );
		return 0; 
	}

	if (m_iToLevel - m_iCurrLevel >= 5)
		m_iCurrLevel = m_iToLevel - 4;
	if (!isLevelExist( m_iCurrLevel ))
	{
		m_iCurrLevel = -1;
		changeStatus( GREENERGUIDE_NULL );
	}
}
//--------------------------------------------------
void SPWindowGreenerGuide::changeStatus(GREENERGUIDESTATUS status)
{
	m_currStatus = status;

	if (GREENERGUIDE_ICON == status)
	{
		setIconVisible( true );
		setContentVisible( false );
		setWaitingVisible( false );
		SetAbsCoord( 700, 420 );
		RefreshRelationCoord();
	}
	else if (GREENERGUIDE_CONTENT == status)
	{
		setIconVisible( false );
		setContentVisible( true );
		setWaitingVisible( false );
		SetAbsCoord( 216, 113 );
		RefreshRelationCoord();
	}
	else if (GREENERGUIDE_FETCHING == status)
	{
		setIconVisible( false );
		setContentVisible( true );
		setWaitingVisible( true );
		m_pImgStatus->SPSendMessage( SPIM_SET_IMAGEINDEX , FETCHING );
	}
	else
	{
		setWaitingVisible( false );
		setIconVisible( false );
		setContentVisible( false );
		SetAbsCoord( 1000, 800 );
		RefreshRelationCoord();
	}
}
void SPWindowGreenerGuide::setIconVisible( bool bVisible )
{
	if (m_pWndIcon)
	{
		if (bVisible)
			m_pWndIcon->Show( true );
		else
			m_pWndIcon->Hide();
	}
}
void SPWindowGreenerGuide::setContentVisible( bool bVisible )
{
	if (m_pWndContent)
	{
		if (bVisible)
			m_pWndContent->Show( true );
		else
			m_pWndContent->Hide();
	}
}
void SPWindowGreenerGuide::setWaitingVisible( bool bVisible )
{
	if (m_pWndWaiting)
	{
		if (bVisible)
			m_pWndWaiting->Show( true );
		else
			m_pWndWaiting->Hide();
	}
}
//--------------------------------------------------
static void safeSetText( SPWindowStatic *obj, const char *szText )
{
	if (obj)
		obj->SetWindowText( szText );
}
bool SPWindowGreenerGuide::isLevelExist( int iLevel, GreenerGuideData *data )
{
	bool bFound = false;
	for (int idx = 0; idx < (int)m_vecGreenerGuideData.size(); ++idx)
	{
		if (m_vecGreenerGuideData[idx].iPlayerLevel == iLevel)
		{
			if (data)
				*data = m_vecGreenerGuideData[idx];
			bFound = true;
			break;
		}
	}

	if (!bFound)
		return false;
}
bool SPWindowGreenerGuide::makeContent( int iGuideLevel )
{
	GreenerGuideData data;
	if (!isLevelExist( iGuideLevel, &data ))
		return false;

	// found level
	safeSetText( m_pC_txtInfo, data.strHelp );
	
	for (int idx = 0; idx < GG_MONSTER_COUNT; ++idx)
	{
		safeSetText( m_pC_txtMap[idx], data.strMonsterMap[idx] );
		safeSetText( m_pC_txtMonster[idx], data.strMonster[idx] );
		safeSetText( m_pC_txtLevel[idx], data.strMonsterLevel[idx] );
	}

	SPQuestAttr quest;
	char quest_buf[MAX_STRING_SIZE] = {0};
	for (int idx = 0; idx < GG_QUEST_COUNT; ++idx)
	{
		if (SPQuestManager::GetInstance()->LoadLDTFile( data.iQuestID[idx], &quest ))
		{
			_snprintf( quest_buf, MAX_STRING_SIZE-1, "在<%s>处开始<%s>任务。",
				quest.m_QuestNpc[0].m_strName.c_str(),
				quest.m_strQuestName.c_str() );
			safeSetText( m_pC_txtQuest[idx], quest_buf );
		}
	}

	char gift_buf[MAX_STRING_SIZE] = {0};
	if ((int)g_pGOBManager->GetLocalPlayer()->GetCheckStatusValue(CHECK_STATUS_GENDER) == MALE)
	{
		for (int idx = 0; idx < GG_GIFT_COUNT; ++idx)
		{
			m_pC_itemGift[idx]->SetItemID( data.iGiftBoy[idx] );
			m_pC_itemGift[idx]->SetCount( data.iGiftBoyNum[idx] );
			m_pC_itemGift[idx]->ReResourceLoad();
			if (data.iGiftBoy[idx] <= 0)
				m_pC_itemGift[idx]->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			else
				m_pC_itemGift[idx]->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		}
	}
	else 
	{
		for (int idx = 0; idx < GG_GIFT_COUNT; ++idx)
		{
			m_pC_itemGift[idx]->SetItemID( data.iGiftGirl[idx] );
			m_pC_itemGift[idx]->SetCount( data.iGiftGirlNum[idx] );
			m_pC_itemGift[idx]->ReResourceLoad();
			if (data.iGiftGirlNum[idx] <= 0)
				m_pC_itemGift[idx]->DelStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
			else
				m_pC_itemGift[idx]->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
		}
	}

	data.iEly = data.iEly >= 0 ? data.iEly : 0;
	_snprintf( gift_buf, MAX_STRING_SIZE-1, "%d", data.iEly );
	safeSetText( m_pC_txtELY, gift_buf );

	return true;
}
//--------------------------------------------------
// end of file
