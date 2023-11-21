
#include "SPCommon.h"
#include "SPMainGameDEF.H"
#include "SPResourceDef.h"

#include "SPManagerDef.h"
#include "SPManager.h"

#include "SPJoyPadManager.h"

//#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPItemAttr.h"
#include "SPGOBManager.h"

#include "SPDebug.h"
#include "GlobalDefines_Share.h"

#include "SPGameObjectDEF.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPManager.h"

#include "SPGameObject.h"
#include "SPMouseCursor.h"
#include "SPSoundArchive.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"

#include "SPCommandConvert.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPBeAttackedList.h"
#include "SPPlayer.h"
#include "SPGuildArchive.h"
#include "SPGuildLDT.h"

#include "SPSubGameManager.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPWindowListDEF.h"
#include "SPWindow.h"
#include "SPWindowGauge.h"
#include "SPWindowButton.h"
#include "SPWindowToolTip.h"
#include "SPWindowEdit.h"
#include "SPWindowList.h"
#include "SPWindowStatic.h"
#include "SPNoticeBox.h"

#include "SPWindowCandidateVertical.h"

#include "SPWindowWorldMapShortcut.h"
#include "SPWindowMiniMap.h"
#include "SPWindowChatConsole.h"
#include "SPWindowChatHistory.h"
#include "SPWindowChat.h"
#include "SPWindowMenuBar.h"
#include "SPWindowStatPallet.h"
#include "SPWindowLogin.h"
#include "SPWindowLobby.h"
#include "SPWindowMessage.h"
#include "SPWindowNpcTalk.h"
#include "SPWindowPcTalk.h"
#include "SPWindowShopTalk.h"
#include "SPWindowNpcTrade.h"
#include "SPWindowStorage.h"
#include "SPWindowPcTrade.h"
#include "SPWindowGift.h"
#include "SPWindowCashShop.h"
#include "SPWindowMailList.h"
#include "SPWindowMailMsg.h"
#include "SPWindowUserShopSeller.h"
#include "SPWindowUserShopBuyer.h"
#include "SPWindowPetShopSeller.h"
#include "SPWindowWayPoint.h"
#include "SPWindowBeautyShop.h"
#include "SPWindowPet.h"
#include "SPWindowLoginSoftKey.h"
#include "SPWindowPcTradeResult.h"
#include "SPWindowMicrophone.h"
#include "SPWindowBugleSelect.h"
#include "SPWindowBugleMenu.h"
#include "SPWindowVisualBugle.h"
#include "SPWindowPushprop.h"

#include "SPWindowGuild.h"
#include "SPWindowGuildCreate.h"
#include "SPWindowGuildLevelup.h"
#include "SPWindowGuildEmblem.h"

#include "SPWindowCrops.h"
#include "SPWindowGuildDestroy.h"
#include "SPWindowJoyStick.h"
#include "SPWindowNpcPortal.h"
#include "SPWindowNpcEventList.h"
#include "SPWindowNpcEvent.h"
#include "SPWindowMenu.h"
#include "SPWindowRS.h"

#include "SPWindowPvpReserve.h"
#include "SPWindowPvpEnterWait.h"
#include "SPWindowPvpLoadWait.h"
#include "SPWindowPvpRank.h"
#include "SPWindowPvpResult.h"
#include "SPWindowPvpTimmer.h"

#include "SPWindowCreateMsg.h"
#include "SPWindowDestoryMsg.h"

#include "SPWindowLove.h"
#include "SPWindowLoveMsgInput.h"
#include "SPWindowLoveMsgView.h"

#include "SPUIUnit.h"
#include "SPUIChatMacro.h"
#include "SPUIUnitManager.h"

#include "SPQuestAttr.h"

#include "SPChatManager.h"

#include "SPWindowItem.h"
#include "SPWindowEquip.h"
#include "SPWindowSkill.h"
#include "SPWindowStatus.h"

#include "SPWindowCashTradeSelect.h"
#include "SPWindowCashTrade.h"
#include "SPWindowCashGift.h"
#include "SPWindowCashTradeResult.h"
#include "SPWindowCashTradeWait.h"

#include "SPWindowExit.h"
#include "SPWindowLooting.h"
#include "SPWindowChannel.h"
#include "SPWindowPlayerInfo.h"
#include "SPWindowQuickSlot.h"
#include "SPWindowFriend.h"
#include "SPWindowFriendFind.h"//[liuyang, 2008.8.29]
#include "SPWindowFindMessage.h"//[liuyang, 2008.8.31]
#include "SPWindowFindMessageOne.h"//[liuyang, 2008.9.1]
#include "SPWindowFindMessageTwo.h"//[liuyang, 2008.9.1]
#include "SPWindowFriendFollow.h"//[liuyang,2008.10.10]
#include "SPWindowFriendSnatch.h"//[liuyang,2008.10.10]
#include "SPWindowQuest.h"
#include "SPWindowQuestView.h"
#include "SPWindowQuestNotify.h"
#include "SPWindowBuff.h"
#include "SPWindowHelp.h"
#include "SPWindowTeam.h"
#include "SPWindowMiniTeam.h"
#include "SPWindowError.h"
#include "SPWindowWorldMap.h"
#include "SPWindowPlayerInfoEquip.h"
#include "spWindowPlayerInfoStatus.h"
#include "SPWindowPrologue.h"
#include "SPWindowStory.h"
#include "SPWindowLottery.h"
#include "SPWindowGamble.h"
#include "SPWindowSystemMessage.h"
#include "SPWindowChatMacro.h"
#include "SPWindowItemMix.h"
#include "SPWindowTextBook.h"
#include "SPWindowGuideBook.h"
#include "SPWindowChattingRoom.h"
#include "SPWIndowTalkingRoom.h"
#include "SPWindowChattingPassword.h"
#include "SPWindowAuction.h"
#include "SPWindowAuctionMsg.h"
#include "SPWindowAutoSet.h"

#include "SPWindowOption.h"
#include "SPWindowBeExact.h"
#include "SPWindowMessageBox.h"
#include "SPWindowMessageList.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPEventDEF.h"
#include "SPEventNpcHandler.h"
#include "SPMailManager.h"
#include "SPEventManager.h"

#include "SPCashTradeManager.h"

#include "SPPvpManager.h"

#include "SPCheckManager.h"

#include "SPInterfaceCoordManager.h"
#include "SPDragnDropManager.h"
#include "SPPlayerActionManager.h"

#include "SPTitleAttr.h"
#include "SPTitleArchive.h"
#include "SPTitleRenderDEF.h"
#include "SPTitleRenderBase.h"
#include "SPTitleManager.h"

#include "SPClientOptionArchive.h"

#include "SPInterfaceManager.h"
#include "SPLocalizeManager.h"

#include "SPUtil.h"
#include "SPStream.h"
#include "SPUIManager.h"

#ifdef _PHANTOM_EDITOR_ENABLE
#include "SPWindowPhantomEdit.h"
#endif

#include "SPWindowGreenerGuide.h"
#include "SPWindowPetChat.h"

SPInterfaceManager* g_pInterfaceManager = NULL;

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif


SPInterfaceManager::SPInterfaceManager()
: SPManager()
, m_pRoot			(NULL)
, m_pSubGameZone	(NULL)
, m_pSubGameLogin	(NULL)
, m_pSubGameLobby	(NULL)
, m_pSubGamePrologue(NULL)
, m_ppCurSubgame	(NULL)
, m_pIMEPool		(NULL)
, m_pSoundArchive	(NULL)
, m_bKeyInput		(FALSE)
, m_pWindowChat		(NULL)
, m_pWindowTalkingRoom(NULL)
, m_pWindowPcInfo	(NULL)
, m_pWindowNpcQuest (NULL)
, m_pWindowQuest	(NULL)
, m_pWindowQuestView(NULL)
, m_pWindowGift		(NULL)
, m_pWindowMailList (NULL)
, m_pWindowMailMsg	(NULL)
, m_pWindowNpcPortal(NULL)
, m_pWindowNpcEventList(NULL)
, m_pWindowNpcEvent	(NULL)
, m_pWindowBeauty	(NULL)
, m_pWindowPet		(NULL)
, m_pWindowSoftKB	(NULL)
, m_pWindowMove		(NULL)
, m_pCandVertical	(NULL)
, m_pWindowGuildInfo(NULL)
, m_pWindowCreateGuild(NULL)
, m_pwindowGuildEmblem(NULL)
, m_pWindowGuildLevelup(NULL)
, m_pWindowPvpReserve(NULL)
, m_pWindowPvpEnterWait(NULL)
, m_pWindowPvpResult(NULL)
, m_pWindowAuction(NULL) 
, m_pWindowAuctionMSG(NULL) 
{	
	SetMgrID(MGR_ID_INTERFACE);
	g_pInterfaceManager = this;		
	m_pIMEPool = new SPIMEPool;	
}

SPInterfaceManager::~SPInterfaceManager()
{
	Clean();
}

SPWindow* SPInterfaceManager::GetCursorWindow	()	
{ 
	return m_pWindowCursorIn; 	
}

SPWindow* SPInterfaceManager::GetFocusWindow	()	
{ 
	return m_pWindowFocusIn;		
}

SPWindow* SPInterfaceManager::GetActiveWindow	()	
{ 
	return m_pWindowActive;		
}

SPWindow* SPInterfaceManager::GetModalWindow	()	
{
	return m_pWindowModal;		
}
SPWindow* SPInterfaceManager::GetLobbyWindow	()	
{ 
	return m_pSubGameLobby;		
}

SPWindow* SPInterfaceManager::GetCandVertical()
{
	return m_pCandVertical;
}

SPWindow* SPInterfaceManager::GetStatPalletWindow()
{
	return m_pWindowStat;
}

SPWindow* SPInterfaceManager::GetInvenWindow()
{
	return m_pWindowInven;
}


SPWindow* SPInterfaceManager::GetEquipWindow()
{
	return m_pWindowEquip;
}


SPWindow* SPInterfaceManager::GetStorageWindow()
{
	return m_pWindowStorage;
}


SPWindow* SPInterfaceManager::GetPCInfoWindow()
{
	return m_pWindowPcInfo;
}

SPWindow* SPInterfaceManager::GetPcTradeWindow()
{
	return m_pWindowPcTrade;

}


SPWindow* SPInterfaceManager::GetNpcTalkWindow()
{
	return m_pWindowNpcTalk;
}


SPWindow* SPInterfaceManager::GetPetWindow()
{
	return m_pWindowPet;
}

SPWindow* SPInterfaceManager::GetSoftKeyWindow()
{
	return m_pWindowSoftKB ;
}


int	SPInterfaceManager::GetCursorX()
{ 
	return m_iCursorX; 
}

int	SPInterfaceManager::GetCursorY()
{ 
	return m_iCursorY; 
}

SPWindow* SPInterfaceManager::GetToolTipWindow() 
{ 
	return m_pWindowToolTip; 
}

SPWindow* SPInterfaceManager::GetLBToolTipWindow() 
{ 
	return m_pWindowLBToolTip; 
}

SPWindow* SPInterfaceManager::GetGuildInfoWindow() 
{ 
	return m_pWindowGuildInfo; 
}

SPWindow* SPInterfaceManager::GetCreateGuildWindow() 
{ 
	return m_pWindowCreateGuild; 
}
SPWindow* SPInterfaceManager::GetGuildEmblemWindow() 
{ 
	return m_pwindowGuildEmblem; 
}
SPWindow* SPInterfaceManager::GetDestroyGuildWindow() 
{ 
	return m_pWindowDestroyGuild; 
}

SPWindow* SPInterfaceManager::GetGuildLevelupWindow()
{
	return m_pWindowGuildLevelup;
}

SPWindow* SPInterfaceManager::GetAuctionWindow()
{
	return m_pWindowAuction ;
}
SPWindow* SPInterfaceManager::GetAuctionMsgWindow()
{
	return m_pWindowAuctionMSG ;
}

void SPInterfaceManager::Clean()
{
	SPWindow* pWindow;
	int iX, iY;
	POINT pt;


#define SAVE_START_POINT(__WIID__) \
	pWindow = m_pRoot->Find(__WIID__, true);  \
	if(pWindow)  { \
		pWindow->GetAbsCoord(iX, iY);  \
		pt.x = iX; \
		pt.y = iY; \
		SPInterfaceCoordManager::GetInstance()->SetWindowPT(__WIID__, pt); \
	}

	SAVE_START_POINT(	WIID_USERSHOP_SELL		)
	SAVE_START_POINT(	WIID_USERSHOP_BUY		)
	SAVE_START_POINT(	WIID_STATUS				)
	SAVE_START_POINT(	WIID_ITEM				)
	SAVE_START_POINT(	WIID_EQUIP				)
	SAVE_START_POINT(	WIID_TEAM				)
	SAVE_START_POINT(	WIID_FRIEND				)
	SAVE_START_POINT(	WIID_OPTION				)
	SAVE_START_POINT(	WIID_LOOTING			)
	SAVE_START_POINT(	WIID_PC_INFO			)
	SAVE_START_POINT(	WIID_SKILL				)
	SAVE_START_POINT(	WIID_QUEST				)
	SAVE_START_POINT(	WIID_QUESTVIEW			)
	SAVE_START_POINT(	WIID_HELP				)
	SAVE_START_POINT(	WIID_EXIT				)
	SAVE_START_POINT(	WIID_NOTICEBOX			)
	SAVE_START_POINT(	WIID_PC_TRADE			)
	SAVE_START_POINT(	WIID_CHANNEL_CHANGE		)



	SAFE_DELETE(m_pRoot);
	SAFE_DELETE(m_pIMEPool);
	
	CutInClear();

	SPDragnDropManager::DelInstance();		//	AJJIYA [7/4/2005]
	SPPlayerActionManager::DelInstance();	//	AJJIYA [7/13/2005]
	SPTitleManager::DelInstance();			//  [3/31/2006 AJJIYA]

	SPInterfaceCoordManager::DelInstance();

	map< unsigned int, vector< SPWindow* > >::iterator iterKey = m_mWindowToggleList.begin();
	while( iterKey != m_mWindowToggleList.end() )
	{
		iterKey->second.clear();
		++iterKey;
	}

	unsigned __int64 ui64Guid = (unsigned __int64)GetGlobalID(CLASS_ID_MANAGER, GetMgrID() );
	SPSoundBase::GetInstance()->FreeArchive(ui64Guid);
	m_pSoundArchive = NULL;	

	

}

bool SPInterfaceManager::Init()
{
#if defined(_DEBUG)
	DXUTOutputDebugString("SPInterfaceManager::Init()\n");
#endif
	SPManager::Init();



	m_iCursorX = 0;
	m_iCursorY = 0;

	m_MouseCursor.Init();

	if( g_pClientOption )
		m_MouseCursor.SetAutoHide(g_pClientOption->GetOptionStruct()->bAutoHideMouse);


	//m_bChat = false;

	//////////////////////////////////////////////////////////////////////////
	// Window Init.
	m_pWindowCursorIn	= NULL;
	m_pWindowFocusIn	= NULL;
	m_pWindowActive		= NULL;
	m_pWindowModal		= NULL;		
	m_ppCurSubgame		= NULL;

	m_pWindowInven		= NULL;
	m_pWindowEquip		= NULL;
	m_pWindowStatus		= NULL;	
	m_pWindowSystemMessage = NULL;
	
	unsigned __int64 ui64Guid = (unsigned __int64)GetGlobalID(CLASS_ID_MANAGER, GetMgrID() );
	SPSoundBase::GetInstance()->AllocateArchive(ui64Guid, &m_pSoundArchive );
	m_PtSoundPos.x = 0;
	m_PtSoundPos.y = 0;

	m_bActiveErrorMsg = false;
	m_bCashShop = false;
	m_bBlockHotKey = false ;

	SPInterfaceCoordManager::GetInstance()->Init();

	CutInInit();

	InitClassTextureInfo();
	
	return CreateSPWindowAll();
}

bool SPInterfaceManager::RegisterSoundUnit(const char* pszFileName, float fDelay, bool bLoop)
{
	if(m_pSoundArchive)
		return m_pSoundArchive->InsertItem(pszFileName);

	return false;
}

bool SPInterfaceManager::PurgeAll()
{
	// TODO: Purge Data List
	if( (*m_ppCurSubgame) )
	{
		SPWindow* pkWindow = (*m_ppCurSubgame)->Find(WNDID_MINIMAP, true);
		if( pkWindow )
			pkWindow->SPSendMessage(SPIM_PURGE);

		pkWindow = (*m_ppCurSubgame)->Find( WIID_SKILL  , true );
		if( pkWindow )
			pkWindow->SPSendMessage(SPIM_PURGE);

		pkWindow = (*m_ppCurSubgame)->Find( WIID_FRIEND , true );
		if( pkWindow )
			pkWindow->SPSendMessage(SPIM_PURGE);

		pkWindow = (*m_ppCurSubgame)->Find( WIID_LOTTERY , true );
		if( pkWindow )
			pkWindow->SPSendMessage(SPIM_PURGE);

		pkWindow = (*m_ppCurSubgame)->Find( WIID_GAMBLE , true );
		if( pkWindow )
			pkWindow->SPSendMessage(SPIM_PURGE);

		pkWindow = (*m_ppCurSubgame)->Find( WIID_LOVE , true );
		if( pkWindow )
			pkWindow->SPSendMessage(SPIM_PURGE);
	}

	if( m_pWindowCursorIn )
	{
		m_pWindowCursorIn->Hide();
		m_pWindowCursorIn = NULL;
	}

	if( m_pWindowFocusIn )
	{
		m_pWindowFocusIn->Hide();
		m_pWindowFocusIn = NULL;
	}
	
	if( m_pWindowActive )
	{
		m_pWindowActive->Hide();
		m_pWindowActive = NULL;
	}

	if( m_pWindowModal )
	{
		m_pWindowModal->Hide();
		m_pWindowModal = NULL;
	}

	if(m_pCandVertical) {
		m_pCandVertical->Hide();
	}

	if(m_pNoticeBox)
		m_pNoticeBox->Hide();


	return true;
}

SPIMEPool*	SPInterfaceManager::GetIMEPool()	
{	
	return m_pIMEPool;	
}


void SPInterfaceManager::SetEnglishOnly(bool bVal)
{
	m_pIMEPool->SetEnglishOnly(bVal);
}

BOOL SPInterfaceManager::IsKeyInput()
{
	return m_bKeyInput;
}

void SPInterfaceManager::SetKeyInput( BOOL bKeyInput )
{
	m_bKeyInput	=	bKeyInput;
}

void SPInterfaceManager::SetIMEModeEnglish( bool bEnglish, bool bOnlyButton )
{
	//TODO 한영 표시 루틴 처리

	if( bOnlyButton ) return;

	if( bEnglish )
		m_pIMEPool->EnableEnglishMode();
	else
		m_pIMEPool->EnableHagulMode();
}

bool SPInterfaceManager::SetSubGame(SUBGAME_ID SubGameID)
{	
	m_pRoot->Close();
	if( m_SubGameID == SUBGAME_PROLOGUE )
		ReleasePrologueWindow();

	m_ppCurSubgame = NULL;
	m_SubGameID = SubGameID;
	m_bActiveErrorMsg = false;

	if(m_pWindowCursorIn) {
		m_pWindowCursorIn->Hide();
		m_pWindowCursorIn = NULL;
	}

	if(m_pWindowFocusIn) {
		m_pWindowFocusIn->Hide();
		m_pWindowFocusIn = NULL;
	}

	if(m_pWindowActive) {
		m_pWindowActive->Hide();
		m_pWindowActive = NULL;
	}

	if(m_pWindowModal) {
		m_pWindowModal->Hide();
		m_pWindowModal = NULL;
	}	
	m_pWindowMove = NULL;

	switch(m_SubGameID) {
	case SUBGAME_LOGGIN		:
		{		
			m_ppCurSubgame = &m_pSubGameLogin;
			(*m_ppCurSubgame)->Show();
			SPWindow* pWindow = (*m_ppCurSubgame)->Find(WIID_LOGIN, true);
			if(pWindow) {
				SetActiveWindow(pWindow);
				pWindow = pWindow->Find(WIID_LOGGIN_EDIT_ID, true);
				if(pWindow) pWindow->SetFocus();
			}
		}
		break;

	case SUBGAME_ZONE		:
		{
			m_ppCurSubgame = &m_pSubGameZone;
			SPWindowBugleSelect *window = (SPWindowBugleSelect*)FindWindow( WIID_BUGLE_SELECT );
			SPPlayer *pLocalPlayer = g_pGOBManager->GetLocalPlayer();
			if (window&&pLocalPlayer)
				window->ReloadBugleLDT( pLocalPlayer->getIsPlayerGM() ); 

			PurgeAll();
			SetShowDefaultWindow( true );
			OnRefreshPlayerStat(0, 0);		
			PurgeAll();
		}

		break;

	case SUBGAME_LOADING	:
		break;

	case SUBGAME_LOBBY:
		{
			m_ppCurSubgame = &m_pSubGameLobby;
			(*m_ppCurSubgame)->Show();
			
			SPWindowLobby* pkWindow = (SPWindowLobby*)m_pSubGameLobby->Find(WIID_LOBBY);
			if( pkWindow )
				pkWindow->ShowLobbyType(0);
		}
		break;

	case SUBGAME_PROLOGUE:
		{
			CreatePrologueWindow();
			m_ppCurSubgame = &m_pSubGamePrologue;
			(*m_ppCurSubgame)->Show();
		}
		break;
	
	default:
		assert(0 && "Unknown Subgame.");
		return false;		
	}
	return true;
}


bool SPInterfaceManager::CreateSPWindowAll()
{
	SPWindow*	pWindow = NULL;	
	SPTexture*	pImage	= NULL;	
	POINT		ptStart;

	g_UIManager = new SPUIManager();

	/*
	if(g_bUsePackFile) 
	{
		// R_SVR.
		//- 2008.06.27 현재 데이타 패킹이 안되어 있음. 실행 보류함.
		SPStream* strm_XML	= 0; 
		INT		FileLength	= 0;	
		CHAR*	XMLString	= 0;	

		SPResourceBase::GetInstance()->GetStreamData(RES_FNAME_XML_UI,&strm_XML);
		FileLength	= strm_XML->Length();
		XMLString	= new CHAR[FileLength];

		strm_XML	->Read(XMLString,FileLength);
		g_UIManager	->Init(XMLString);

		SAFE_RELEASE(strm_XML);
		SAFE_DELETE(XMLString);
	}
	else
	{
		// Belos,Elfa,Custom....
		g_UIManager->Init(CString(GetAddResPath(RES_FNAME_XML_UI,RES_WORK_RESOURCE_PATH)));
	}
	*/


	//////////////////////////////////////////////////////////////////////////
	// Root Window
	m_pRoot = new SPWindow( WNDID_ROOT, WIID_ROOT);	
	m_pRoot->Show();

	//////////////////////////////////////////////////////////////////////////
	// Subgame Manage window list 	
	m_pSubGameLogin		=  new SPWindow( WNDID_ROOT, WIID_SUBGAMELOGIN		, m_pRoot);
	m_pSubGameLobby		=  new SPWindow( WNDID_ROOT, WIID_SUBGAMELOBBY		, m_pRoot);
	m_pSubGameZone		=  new SPWindow( WNDID_ROOT, WIID_SUBGAMEZONE		, m_pRoot);
	m_pSubGamePrologue	=  new SPWindow( WNDID_ROOT, WIID_SUBGAMEPROLOGUE	, m_pRoot);


	//////////////////////////////////////////////////////////////////////////
	// Common Window

	m_pCandVertical		= new SPWindowCandidateVertical(WIID_CANDIVERTICAL, 0, 0, 113, 215, m_pRoot);
	
	// notice	
	SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_NOTICEBOX, ptStart);
	m_pNoticeBox = new SPNoticeBox( WIID_NOTICEBOX, ptStart.x, ptStart.y, 160, 80, m_pRoot);
	m_pNoticeBox->Hide();

	//////////////////////////////////////////////////////////////////////////	
	//messagebox
	for (int i=0; i<MAX_MESBOX; i++)
	{
		m_pMessageBox[i] = new SPWindowMessageBox(WIID_MESSAGEBOX+i, 0, 0, 800, 600, m_pSubGameZone);
		m_pMessageBox[i]->Hide();
	}

	//////////////////////////////////////////////////////////////////////////	
	//// SubGame Login 	
	//pWindow = new SPWindowLogin( WIID_LOGIN, 428, 114, 320, 410, m_pSubGameLogin);	//[2007/1/17] Show에서 Resize함
	pWindow = new SPWindowLogin( WIID_LOGIN, 0, 0, 800, 600, m_pSubGameLogin);	//[2007/1/17] Show에서 Resize함
	//pWindow->SetImage("DATA/LOGGIN/LOGIN001.PNG", 0, 0);
	//pWindow->SetSrcSize(320, 410) ;
	pWindow->Show();
	pWindow = NULL;

	m_pWindowSoftKB = new SPWindowVKeyboard(WIID_LOGGIN_SOFTKEYDLG, 428+30, 114+50, 254, 97, m_pSubGameLogin) ;
	m_pWindowSoftKB->SetMoveable(true) ;
	m_pWindowSoftKB->SetShowEnable(false) ;
	m_pWindowSoftKB->Hide();

	pWindow = new SPWindowError(WIID_ERROR, 280, 202, 254, 108, m_pSubGameLogin);
	pWindow->Hide();
	pWindow = NULL;

	//////////////////////////////////////////////////////////////////////////	
	// SubGame Zone 

	// 타이틀 매니저를 초기화 시킨다. [3/31/2006 AJJIYA]
	// 상태창 , 왼쪽 상단의 표시창 보다 먼픸E초기화 되엉雹 한다.
	SPTitleManager*	pTitleManager	=	SPTitleManager::GetInstance();

	if( pTitleManager == NULL )
	{
#if defined(_DEBUG)
		DXUTOutputDebugString("SPTitleManager::Init Fail()\n");
#endif
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 변컖E금햨E
	//    상위 4개윈도퓖E오더링 바꾸툈E안됩니다..
	//          SPWindow::SwapFocusWrap 와 컖E쳄?있습니다.  - metalgeni

	pWindow = new SPWindowError(WIID_ERROR, 280, 202, 254, 108, m_pSubGameZone);
	pWindow->Hide();
	pWindow = NULL;

	// 월드맵 게임내에서 최상단에 렌더링 되엉雹 한다 AJJIYA [4/7/2006]
	// 맵을 보면서 게임 인터페이스는 진행되엉雹 한다.
	pWindow = new SPWindowWorldMap( WIID_WORLDMAP , 0 , 0 , g_pVideo->GetScreenLenX() , g_pVideo->GetScreenLenY() , m_pSubGameZone);
	pWindow->Hide();
	pWindow = NULL;

	// ToolTip(Item) 이미지셋및 기타초기화는 내부에서 그냥 한다.
	// 툴팁 렌더는 제일 마지막에 해야 함AJJIYA [6/30/2006]
	// 이번트는 안먹으니콅E채팅 다음이다.
	m_pWindowToolTip = new SPWindowToolTip( WIID_TOOLTIP, 5, 338, 207, 207,  m_pSubGameZone);
	m_pWindowToolTip->SetMoveable(true);

	// For SubGameLobby Tooltip
	m_pWindowLBToolTip = new SPWindowToolTip( WIID_TOOLTIP, 5, 338, 207, 207,  m_pSubGameLobby);
	m_pWindowLBToolTip->SetMoveable(true);
	
	//// notice	
	//SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_NOTICEBOX, ptStart);
	//m_pNoticeBox = new SPNoticeBox( WIID_NOTICEBOX, ptStart.x, ptStart.y, 160, 80, m_pSubGameZone);
	//m_pNoticeBox->Hide();

	//////////////////////////////////////////////////////////////////////////
	

	////	Help
	pWindow	=	new SPWindowHelp( WIID_HELP , 144 , 119 , 512 , 362 , m_pSubGameZone );
	pWindow	=	NULL;


	//(613, 2) 영역(0,0)_(184, 37)
	pWindow = new SPWindowMiniMap( WNDID_MINIMAP, WIID_MINIMAP, 611 , 0 , 189 , 129 , m_pSubGameZone );
	//pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI_MAP.PNG");
	//pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI001.PNG", 164, 425);
	pWindow->Show();	
	pWindow = NULL;

	// Event Shop Talk Window
	pWindow = new SPWindowShopTalk( WIID_SHOPTALK, 542, 0, 258, 167, m_pSubGameZone);
	pWindow->SetImage("DATA/INTERFACE/CONCEPT/CUTUI002.PNG", 181, 340);
	m_pWindowShopTalk = pWindow;
	m_pWindowShopTalk->Hide();
	pWindow = NULL;	


	// Center Status
	//UI_main.png (6, 16) 영역(0,0)_(331, 96)
	//pWindow  = new SPWindowStatPallet( WNDID_CENTERSTATUS, WIID_STATPALLET, 6, 16, 332, 97, m_pSubGameZone);	
	pWindow  = new SPWindowStatPallet( WNDID_CENTERSTATUS, WIID_STATPALLET, 0, 0, 177, 69, m_pSubGameZone);	
	m_pWindowStat = pWindow;
	pWindow->Show();	
	pWindow = NULL;

	// Menu			
	//pWindow = new SPWindowMenu( WIID_MENU, 489, 570, 311, 30, m_pSubGameZone);
	//pWindow = new SPWindowMenu( WIID_MENU, 487, 564, 313, 36, m_pSubGameZone);   // 메뉴 컬러
	//pWindow = new SPWindowMenu( WIID_MENU, 495, 568, 313, 36, m_pSubGameZone); // 메뉴 단퍊E
	pWindow = new SPWindowMenu( WIID_MENU, 516, 572, 284, 28, m_pSubGameZone);
	pWindow->Show();
	pWindow = NULL;


	//////////////////////////////////////////////////////////////////////////
	// Sub Windows

	// Item

	SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_ITEM, ptStart);
	m_ptItemPos.x	= ptStart.x; m_ptItemPos.y	= ptStart.y;
	m_ptInven.x = m_ptItemPos.x; m_ptInven.y = m_ptItemPos.y;
	m_ptItemSize.x	= 198; m_ptItemSize.y	= 276;
	pWindow  = new SPWindowItem( WIID_ITEM, m_ptItemPos.x, m_ptItemPos.y, m_ptItemSize.x, m_ptItemSize.y, m_pSubGameZone);
	//pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI002.PNG", 352, 201);
	pWindow->SetMoveable(true);
	m_pWindowInven = pWindow;
	pWindow = NULL;
	
	pWindow = new SPWindowPushProp( WIID_PUSH_PROP, m_ptItemPos.x-42, m_ptItemPos.y, 42, m_ptItemSize.y, m_pSubGameZone);
	pWindow->SetMoveable( false );
	pWindow = NULL;

	pWindow = new SPWindowVisualBugle( WIID_VISUAL_BUGLE, 586, 114, 214, 192, m_pSubGameZone );
	pWindow->SetMoveable( false );
	pWindow->Hide();

	pWindow = new SPWindowVisualBugle( WIID_VISUAL_BUGLE_PREVIEW, 277, 388, 214, 192, m_pSubGameZone );
	pWindow->SetMoveable( false );
	pWindow->Hide();

	pWindow = new SPWindowBugleSelect( WIID_BUGLE_SELECT, 125, 381, 183, 199, m_pSubGameZone );
	pWindow->SetMoveable( false );
	pWindow = NULL;

	pWindow = new SPWindowBugleMenu( WIID_BUGLE_MENU, 5, 517, 119, 63, m_pSubGameZone );
	pWindow->SetMoveable( false );
	pWindow = NULL;

	// Equip
	SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_EQUIP, ptStart);
	m_ptEquipPos.x	= ptStart.x; m_ptEquipPos.y	= ptStart.y;	
	m_ptEquip.x = m_ptEquipPos.x; m_ptEquip.y = m_ptEquipPos.y;
	m_ptEquipSize.x	= 234; m_ptEquipSize.y	= 215;
	pWindow  = new SPWindowEquip( WIID_EQUIP, m_ptEquipPos.x, m_ptEquipPos.y, m_ptEquipSize.x, m_ptEquipSize.y, m_pSubGameZone);	
	pWindow->SetMoveable(true);
	m_pWindowEquip = pWindow;
	pWindow = NULL;

#ifdef _PHANTOM_EDITOR_ENABLE
	pWindow = new SPWindowPhantomEditor( WIID_PHANTOM_EDITOR, 400, 400, 240, 100, m_pSubGameZone );
	pWindow->SetMoveable( true );
	pWindow->Hide();
	pWindow = NULL;
#endif

	pWindow = new SPWindowPetChatEditor( WIID_PETCHAT_EDITOR, 400, 400, 364, 182, m_pSubGameZone );
	pWindow->SetMoveable( true );
	pWindow->Hide();
	pWindow = NULL;

	//pWindow = new SPWindowGreenerGuild( WIID_GREENERGUILD, 100, 100, 200, 200, m_pSubGameZone );
	//pWindow->SetMoveable( true );
	//pWindow->Show( true );
	//pWindow = NULL;
	
	// CashTradeSelect
	pWindow = new SPWindowCashTradeSelect( WIID_CASH_TRADE_SELECT, 150, 150, 164, 103, m_pSubGameZone);
	pWindow->SetMoveable(true);
	pWindow->Hide();
	pWindow = NULL;

	// CashTrade
	pWindow = new SPWindowCashTrade( WIID_CASH_TRADE, 266, 176, 268, 282, m_pSubGameZone);
	pWindow->SetMoveable(true);
	pWindow->Hide();
	pWindow = NULL;

	// CashGift
	pWindow = new SPWindowCashGift( WIID_CASH_GIFT, 266, 132, 449, 374, m_pSubGameZone);
	pWindow->SetMoveable(true);
	pWindow->Hide();
	pWindow = NULL;

	// CashTradeResult
	pWindow = new SPWindowCashTradeResult( WIID_CASH_TRADE_RESULT, 266, 187, 268, 226, m_pSubGameZone);
	pWindow->SetMoveable(true);
	pWindow->Hide();
	pWindow = NULL;

	// CashTradeWait
	pWindow = new SPWindowCashTradeWait( WIID_CASH_TRADE_WAIT, 237, 233, 325, 134, m_pSubGameZone);	
	pWindow->Hide();
	pWindow = NULL;

	// Skill
	SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_SKILL, ptStart);
	m_ptSkillPos.x	= ptStart.x; m_ptSkillPos.y = ptStart.y;		// AJJIYA [6/29/2005]
	m_ptSkillSize.x	= 260; m_ptSkillSize.y 	= 345;		// AJJIYA [6/29/2005]
	pWindow  = new SPWindowSkill( WIID_SKILL, m_ptSkillPos.x, m_ptSkillPos.y, m_ptSkillSize.x, m_ptSkillSize.y, m_pSubGameZone);
	pWindow->SetMoveable(true);
	pWindow = NULL;

	// SystemMessage
	m_ptOldSystemMessage.x		=	505;
	m_ptOldSystemMessage.y		=	492;
	pWindow  = new SPWindowSystemMessage( WIID_SYSTEM_MESSAGE , m_ptOldSystemMessage.x , m_ptOldSystemMessage.y , 295 , 80 , m_pSubGameZone );
	pWindow->SetMoveable(true);
	m_pWindowSystemMessage = pWindow;
	pWindow = NULL;
	m_ptCutInSystemMessage.x	=	514;
	m_ptCutInSystemMessage.y	=	492;

	SPWindowSystemMessage*	pSystemMessage	=	(SPWindowSystemMessage*)m_pWindowSystemMessage;

	if( pSystemMessage != NULL )
		m_bMinimizedSystemMessage	=	pSystemMessage->GetMinimized();

	// QuickSlot	
	m_ptQuickSlotPos.x	= 197; m_ptQuickSlotPos.y 	= 0;
	m_ptQuickSlotSize.x	= 414; m_ptQuickSlotSize.y 	= 48 + 46;
	pWindow  = new SPWindowQuickSlot( WIID_QUICK_SLOT, m_ptQuickSlotPos.x, m_ptQuickSlotPos.y, m_ptQuickSlotSize.x, m_ptQuickSlotSize.y, m_pSubGameZone );
	pWindow->Show();
	pWindow = NULL;

	// Status
	SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_STATUS, ptStart);
	m_ptStatusPos.x	= ptStart.x; m_ptStatusPos.y	= ptStart.y;
	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP)) {
		m_ptStatusSize.x= 260; m_ptStatusSize.y	= 303;		/// Old .y = 264;
	}
	else {
		m_ptStatusSize.x= 260; m_ptStatusSize.y	= 401;
	}
	pWindow  = new SPWindowStatus( WIID_STATUS, m_ptStatusPos.x, m_ptStatusPos.y, m_ptStatusSize.x, m_ptStatusSize.y, m_pSubGameZone);	
	pWindow->SetMoveable(true);
	m_pWindowStatus = pWindow;
	pWindow = NULL;

	// Quest
	SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_QUEST, ptStart);
	m_pWindowQuest	=	new SPWindowQuest( WIID_QUEST , ptStart.x , ptStart.y, 247 , 272 , m_pSubGameZone );	// AJJIYA [9/23/2005]
	m_pWindowQuest->SetMoveable(true);

	// QuestView
	SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_QUESTVIEW, ptStart);
	m_pWindowQuestView	=	new SPWindowQuestView( WIID_QUESTVIEW , ptStart.x, ptStart.y, 297 , 426 , m_pSubGameZone );	// AJJIYA [6/23/2006]
	m_pWindowQuestView->SetMoveable(true);

	// QuestNotify
	pWindow	= new SPWindowQuestNotify( WIID_QUESTNOTIFY , 613 ,129 , 187 , 388 , m_pSubGameZone );
	pWindow->SetMoveable(true);
	pWindow->Hide();
	pWindow = NULL;

	// Looting
	SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_LOOTING, ptStart);	
	m_ptLootingPos.x = ptStart.x;		m_ptLootingPos.y = ptStart.y;
	m_ptLootingSize.x = 210;	m_ptLootingSize.y = 265;
	m_pWindowLooting = new SPWindowLooting(WIID_LOOTING, m_ptLootingPos.x, m_ptLootingPos.y, m_ptLootingSize.x, m_ptLootingSize.y, m_pSubGameZone);
	m_pWindowLooting->SetMoveable(true);

	// Team	
	SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_TEAM, ptStart);
	pWindow = new SPWindowTeam(WIID_TEAM, ptStart.x, ptStart.y, 232, 279, m_pSubGameZone);	
	pWindow->SetMoveable(true);
	pWindow = NULL;

	pWindow = new SPWindowMiniTeam(WIID_MINI_TEAM, 6, 117, 138, 149, m_pSubGameZone);
	pWindow = NULL;

	// Exit
	SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_EXIT, ptStart);
	pWindow  = new SPWindowExit( WIID_EXIT, ptStart.x, ptStart.y, 134, 105, m_pSubGameZone);
	pWindow->SetMoveable(true);
	pWindow = NULL;


	// Option	
	SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_OPTION, ptStart);
	pWindow = new SPWindowOption( WIID_OPTION, ptStart.x, ptStart.y, 290, 323 , m_pSubGameZone );	
	pWindow->Hide(false);
	pWindow->SetMoveable(true);
	pWindow = NULL;


	// Event Npc Shop Trade List Window	
	pWindow = new SPWindowNpcTrade( WIID_NPCTRADE, 4, 29, 319, 477, m_pSubGameZone);
	m_pWindowNpcTrade = pWindow;
	m_pWindowNpcTrade->Hide();
	pWindow = NULL;

	// Event Npc Shop Ely Window
	//pWindow = new SPWindowEly( WIID_ELY, 207, 91, 120, 21, m_pSubGameZone);
	//pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI001.PNG", 201, 75);
	//m_pWindowEly = pWindow;
	//pWindow->Hide();
	//pWindow = NULL;

	// Event Player Sell Item Window
	//pWindow = new SPWindowItemSell( WIID_ITEMSELL, 166, 314, 160, 80, m_pSubGameZone);
	//pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI004.PNG", 161, 0);
	//m_pWindowItemSell = pWindow;
	//pWindow->Hide();
	//pWindow = NULL;

	// Storage 창컖E
	pWindow = new SPWindowStorage( WIID_STORAGE, 9, 8, 198, 241, m_pSubGameZone);
	m_pWindowStorage = pWindow;
	pWindow->Hide();
	pWindow = NULL;

	//[xialin 2008/07/30]add
	pWindow = new SPWindowBeExact(WIID_BEEXACT, 266, 130, 268, 220, m_pSubGameZone);
	pWindow->Hide();
	pWindow = NULL;

	//[xialin 2008/09/10]add
	pWindow = new SPWindowCreateMsg( WIID_CREATEMSG, 420, 140, 187, 262, m_pSubGameZone);
	pWindow->Hide();
	pWindow = NULL;

	//[xialin 2008/09/10]add
	pWindow = new SPWindowDestoryMsg( WIID_DESTORYMSG, 420, 140, 198, 241, m_pSubGameZone);
	pWindow->Hide();
	pWindow = NULL;

	// Gift
	pWindow = new SPWindowGift(WIID_GIFT, 166 - 100, 314 - 200, 505, 312, m_pSubGameZone );
	//pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI010.PNG", 1, 310);
	m_pWindowGift = pWindow;
	pWindow->Hide();
	pWindow = NULL;

	// Mail List
	pWindow = new SPWindowMailList(WIID_MAILLIST, 8, 9, 350, 269, m_pSubGameZone );
	//pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI010.PNG", 303, 1);
	m_pWindowMailList = pWindow;
	pWindow->Hide();
	pWindow = NULL;

	// Mail Msg
	pWindow = new SPWindowMailMsg(WIID_MAILMSG, 8, 283, 350, 202, m_pSubGameZone);
	m_pWindowMailMsg = pWindow;	
	pWindow->Hide();
	pWindow = NULL;

	// Pc Trade
	SPInterfaceCoordManager::GetInstance()->GetWindowPT( WIID_PC_TRADE, ptStart);
	int iPcTradeHeight = 323;
	
	////
	//int iPcTradeHeight = 258;		
	//if(SPLocalizeManager::GetInstance()->GetCashTradeDisplay() == CASH_TRADE_ALL) {
	//	iPcTradeHeight = 323;
	//}
	//else if(SPLocalizeManager::GetInstance()->GetCashTradeDisplay() == CASH_TRADE_CASH || 
	//	SPLocalizeManager::GetInstance()->GetCashTradeDisplay() == CASH_TRADE_POINT)
	//{
	//	iPcTradeHeight = 308;
	//}
	//pWindow = new SPWindowPcTrade( WIID_PC_TRADE, ptStart.x, ptStart.y, 384, iPcTradeHeight, m_pSubGameZone);
	
	pWindow = new SPWindowPcTrade( WIID_PC_TRADE, ptStart.x, ptStart.y, 395, 292, m_pSubGameZone);//[2008/3/11]-기존버전
	pWindow->SetMoveable(true);
	m_pWindowPcTrade = pWindow;
	pWindow->Hide();
	pWindow = NULL;


	//Pc Trade Result	
	pWindow = new SPWindowPcTradeResult(WIID_PC_TRADE_RESULT, 168, 168, 463, 264, m_pSubGameZone);
	pWindow->Hide();
	pWindow = NULL;

	//MicroPhone
	pWindow = new SPWindowMicropone(WIID_MICROPHONE, 250, 169, 237, 150, m_pSubGameZone);
	pWindow->SetMoveable(true);
	pWindow->Hide();
	pWindow = NULL;



	//FindMessage [2008.8.31]
	pWindow = new SPWindowFindMessage( WIID_FIND_MESSAGE, 1, 86, 295 - 90, 80, m_pSubGameZone );
	pWindow->SetMoveable(false);
	pWindow = NULL;

	//FindMessageOne
	pWindow = new SPWindowFindMessageOne( WIID_FIND_MESSAGE_ONE, 1, 167, 295 - 90, 80, m_pSubGameZone );
	pWindow->SetMoveable(false);
	pWindow = NULL;

	//FindMessageTwo
	pWindow = new SPWindowFindMessageTwo( WIID_FIND_MESSAGE_TWO, 1, 248, 295 - 90, 80, m_pSubGameZone );
	pWindow->SetMoveable(false);
	pWindow = NULL;
	
	//FriendFind
	//SPInterfaceCoordManager::GetInstance()->GetWindowPT( WIID_FRIEND, ptStart);
	pWindow	= new SPWindowFriendFind( WIID_FRIEND_FIND, 250, 169, 232 , 295 , m_pSubGameZone );
	pWindow->SetMoveable(true);
	pWindow = NULL;
	
	//Follow Friend
	pWindow = new SPWindowFriendFollow( WIID_FRIEND_FOLLOW, 300, 169, 232, 295, m_pSubGameZone );
	pWindow->SetMoveable(true);
	pWindow = NULL;
	
	//Snatch Friend
	pWindow = new SPWindowFriendSnatch( WIID_FRIEND_SNATCH, 340, 169, 232, 295, m_pSubGameZone );
	pWindow->SetMoveable(true);
	pWindow = NULL;

	// User Shop Sell
	SPInterfaceCoordManager::GetInstance()->GetWindowPT( WIID_USERSHOP_SELL, ptStart);	
	pWindow = new SPWindowUserShopSeller(WIID_USERSHOP_SELL, ptStart.x, ptStart.y, 312, 278, m_pSubGameZone);
	pWindow->SetMoveable(true); 
	pWindow->Hide();
	pWindow = NULL;

	// User Shop Buy
	SPInterfaceCoordManager::GetInstance()->GetWindowPT( WIID_USERSHOP_BUY, ptStart);	
	pWindow = new SPWindowUserShopBuyer(WIID_USERSHOP_BUY, ptStart.x, ptStart.y, 312, 278, m_pSubGameZone);
	pWindow->SetMoveable(true);
	pWindow->Hide();
	pWindow = NULL;

	// Pet shop sell
	SPInterfaceCoordManager::GetInstance()->GetWindowPT( WIID_USERSHOP_SELL, ptStart );
	pWindow = new SPWindowPetShopSeller( WIID_PETSHOP_SELL, ptStart.x, ptStart.y, 312, 278, m_pSubGameZone );
	pWindow->SetMoveable( true );
	pWindow->Hide();
	pWindow = NULL;

	// Way Point
	SPInterfaceCoordManager::GetInstance()->GetWindowPT( WIID_WAYPOINT, ptStart);	
	pWindow = new SPWindowWayPoint(WIID_WAYPOINT, ptStart.x, ptStart.y, 247, 317, m_pSubGameZone);
	pWindow->SetMoveable(true);
	pWindow->Hide();
	pWindow = NULL;

	// Pet Equip
	SPInterfaceCoordManager::GetInstance()->GetWindowPT( WIID_PET,		ptStart);
	pWindow = new SPWindowPet(WIID_PET, ptStart.x, ptStart.y, 252, 304, m_pSubGameZone);
	pWindow->SetMoveable(true);
	m_pWindowPet = pWindow;
	pWindow->Hide();
	pWindow = NULL;
	
	//// Event Shop Talk Window
	//pWindow = new SPWindowShopTalk( WIID_SHOPTALK, 263, 5, 274, 98, m_pSubGameZone);
	//m_pWindowShopTalk = pWindow;
	//m_pWindowShopTalk->Hide();
	//pWindow = NULL;	

	//// Npc Talk Window
	//pWindow = new SPWindowNpcTalk( WIID_NPCTALK, 263, 5, 274, 183, m_pSubGameZone);	
	//m_pWindowNpcTalk = pWindow;
	//m_pWindowNpcTalk->Hide();
	//pWindow = NULL;		

	SPInterfaceCoordManager::GetInstance()->GetWindowPT( WIID_CHANNEL_CHANGE, ptStart);	
	pWindow = new SPWindowChannel( WIID_CHANNEL_CHANGE, ptStart.x, ptStart.y, 208, 258, m_pSubGameZone);
	pWindow->SetMoveable(true);
	pWindow = NULL;	

	SPInterfaceCoordManager::GetInstance()->GetWindowPT( WIID_PC_INFO, ptStart);
	pWindow = new SPWindowPlayerInfo( WIID_PC_INFO, ptStart.x, ptStart.y, 160, 199, m_pSubGameZone);
	pWindow->SetMoveable(true);
	pWindow->Hide();
	m_pWindowPcInfo = pWindow;
	pWindow = NULL;	

	//pWindow  = new SPWindowPlayerInfoEquip( WIID_PC_EQUIP, m_ptEquipPos.x, m_ptEquipPos.y, m_ptEquipSize.x, m_ptEquipSize.y, m_pSubGameZone);
	//pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI002.PNG", 176, 0);	
	//m_pWindowPcEquip = pWindow;
	//pWindow = NULL;

	//pWindow  = new SPWindowPlayerInfoStatus( WIID_PC_STATUS, 507, 114, 176, 199, m_pSubGameZone);
	//pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI005.PNG", 0, 92);
	//pWindow = NULL;

	// Friend Window
	SPInterfaceCoordManager::GetInstance()->GetWindowPT( WIID_FRIEND, ptStart);
	pWindow	= new SPWindowFriend( WIID_FRIEND, ptStart.x, ptStart.y, 232 , 295 , m_pSubGameZone );
	pWindow->SetMoveable(true);
	pWindow = NULL;

	// Lottery Window
	//pWindow	= new SPWindowLottery( WIID_LOTTERY, 112 , 115 , 576 , 455 , m_pSubGameZone );
	pWindow	= new SPWindowLottery( WIID_LOTTERY, 0 , 0 , 800 , 600 , m_pSubGameZone );
	pWindow->Hide();
	pWindow = NULL;

	// Gamble Window
	pWindow	= new SPWindowGamble( WIID_GAMBLE, 112 , 115 , 576 , 455 , m_pSubGameZone );
	pWindow->Hide();
	pWindow = NULL;

	// Chat Macro Window
	pWindow = new SPWindowChatMacro( WIID_CHAT_MACRO , 72 , 111 , 248 , 358 , m_pSubGameZone );
	pWindow->SetMoveable(true);
	pWindow->Hide();
	pWindow = NULL;

	// Item Mix Window
	pWindow = new SPWindowItemMix( WIID_ITEM_MIX , 279 , 115 , 281 , 403 , m_pSubGameZone );
	pWindow->SetMoveable(true);
	pWindow->Hide();
	pWindow = NULL;

	//	Guide Book Window
	pWindow = new SPWindowGuideBook( WIID_GUIDE_BOOK , 313 , 248 , 487 , 324 , m_pSubGameZone );
	pWindow->SetMoveable( true );
	pWindow->Hide();
	pWindow = NULL;

	//	Chatting Room Window
	pWindow = new SPWindowChattingRoom( WIID_CHATTING_ROOM , 237 , 99 , 325 , 402 , m_pSubGameZone );
	pWindow->SetMoveable( true );
	pWindow->Hide();
	pWindow = NULL;

	//	Chatting AutoSet Window
	pWindow = new SPWindowAutoSet( WIID_AUTOSET , 105 , 120 , 591 , 359 , m_pSubGameZone );
	pWindow->SetMoveable( true );
	pWindow->Hide();
	pWindow = NULL;

	//	MessageList Window
	pWindow = new SPWindowMessageList( WIID_MESSAGELIST , 105 , 120 , 591 , 359 , m_pSubGameZone );
	pWindow->SetMoveable( false );
	pWindow->Hide();
	pWindow = NULL;

	//	Talking Room Window
	m_pWindowTalkingRoom = new SPWindowTalkingRoom( WIID_TALKING_ROOM , 138 , 111 , 373 , 377 , m_pSubGameZone );
	m_pWindowTalkingRoom->SetMoveable( true );
	m_pWindowTalkingRoom->Hide();

	//	Chatting Room Password
	pWindow = new SPWindowChattingPassword( WIID_CHATTING_PASSWORD , 49 , 170 , 226 , 61 , m_pSubGameZone );
	pWindow->SetMoveable( true );
	pWindow->Hide();
	pWindow = NULL;

	//	Love
	pWindow = new SPWindowLove( WIID_LOVE , 233 , 126 , 282 , 359 , m_pSubGameZone );
	pWindow->SetMoveable( true );
	pWindow->Hide();
	pWindow = NULL;

	//	LoveMsgInput
	pWindow	= new SPWindowLoveMsgInput( WIID_LOVEMSG_INPUT , 297 , 181 , 183 , 177 , m_pSubGameZone );
	pWindow->SetMoveable( true );
	pWindow->Hide();
	m_pWindowLoveMsgInput	=	pWindow;
	pWindow = NULL;

	//	LoveMsgView
	pWindow	= new SPWindowLoveMsgView( WIID_LOVEMSG_VIEW , 602 , 239 , 197 , 147 , m_pSubGameZone );
	pWindow->SetMoveable( false );
	pWindow->Hide();
	m_pWindowLoveMsgView	=	pWindow;
	pWindow = NULL;

	// NpcQuest Window
	pWindow = new SPWindowQuest( WIID_NPC_QUEST , 8 , 64 , 247 , 272 , m_pSubGameZone );
//	pWindow->SetMoveable(true);
	m_pWindowNpcQuest = pWindow;
	pWindow = NULL;

	// Event Message Window
	pWindow = new SPWindowMessage( WIID_MESSAGE, 273, 153, 254, 93, m_pSubGameZone);
	pWindow = NULL;

	// Npc Talk Window
	pWindow = new SPWindowNpcTalk( WIID_NPCTALK, 95, 0, 512, 311, m_pSubGameZone);
	m_pWindowNpcTalk = pWindow;
	m_pWindowNpcTalk->Hide();
	pWindow = NULL;

	// Pc Talk Window
	pWindow = new SPWindowPcTalk(WIID_PCTALK, 0, 309, 358, 240, m_pSubGameZone);
	m_pWindowPcTalk = pWindow;
	m_pWindowPcTalk->Hide();
	pWindow = NULL;

	// duragon [2006/10/09]
	// BeautyShop Window
	pWindow = new SPWindowBeautyShop(WIID_BEAUTYSHOP, 0, 0, 540, 430, m_pSubGameZone);
	m_pWindowBeauty = pWindow;
	pWindow->Hide();
	pWindow = NULL ;

	pWindow = new SPWindowGuildInfo(WIID_GUILD_INFO, 85, 110, 383, 400, m_pSubGameZone) ;
	m_pWindowGuildInfo = pWindow ;
	pWindow->SetMoveable(true) ;
	pWindow->Hide() ;
	pWindow = NULL ;

	pWindow = new SPWindowCreateGuild(WIID_GUILD_CREATE, 100, 100, 272, 255, m_pSubGameZone) ;
	m_pWindowCreateGuild = pWindow ;
	pWindow->Hide() ;
	pWindow = NULL ;

	pWindow = new SPWindowGuildLevelup(WIID_GUILD_LEVELUP, 85, 110, 260, 211, m_pSubGameZone) ;
	m_pWindowGuildLevelup = pWindow ;
	pWindow->Hide() ;
	pWindow = NULL ;

	pWindow = new SPWindowGuildEmblem(WIID_GUILD_EMBLEM, 48, 56, 334, 418, m_pSubGameZone) ;
	m_pwindowGuildEmblem = pWindow ;
	pWindow->Hide() ;
	pWindow = NULL ;

	pWindow = new SPWindowTextBook(WIID_TEXTBOOK, 0, 0, 800, 600, m_pSubGameZone) ;
	m_pWindowStoryBook = pWindow ;
	pWindow->Hide() ;
	pWindow = NULL ;

	pWindow = new SPWindowAuctionMsg(WIID_AUCTION_MSG, 0, 0, 800, 600, m_pSubGameZone) ;
	m_pWindowAuctionMSG = pWindow ;
	pWindow->Hide() ;
	pWindow = NULL ;

	pWindow = new SPWindowAuction(WIID_AUCTION, 0, 0, 800, 494, m_pSubGameZone) ;
	m_pWindowAuction = pWindow ;
	pWindow->Hide() ;
	pWindow = NULL ;


	if( SPLocalizeManager::GetInstance()->GetCountryType() == CT_CHINA )
	{
		pWindow = new SPWindowRS(WIID_RSCLIENT, 100, 100, 316, 342, m_pSubGameZone) ;
		pWindow->SetMoveable(true) ;
		pWindow->Hide() ;
		pWindow = NULL ;
	}

	pWindow = new SPWindowGuildDestroy(WIID_GUILD_DESTROY, 100, 100, 272, 133, m_pSubGameZone) ;
	m_pWindowDestroyGuild = pWindow ;
	pWindow->Hide() ;
	pWindow = NULL ;

	pWindow = new SPWindowGuildCrops(WIID_CROPS, 28, 32, 297, 337, m_pSubGameZone) ;
	//pWindow->SetMoveable(true) ;
	m_pWindowGuildCrop = pWindow;
	pWindow->Hide() ;
	pWindow = NULL ;

	pWindow = new SPWindowNpcPortal(WIID_NPC_PORTAL, 8, 90, 374, 368, m_pSubGameZone);
	m_pWindowNpcPortal = pWindow;
	//pWindow->SetMoveable(true);
	pWindow->Hide();
	pWindow = NULL;

	pWindow = new SPWindowNpcEventList(WIID_NPC_EVENTLIST, 8, 40, 303, 441, m_pSubGameZone);
	m_pWindowNpcEventList = pWindow;
	pWindow->Hide();

	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP)) {
		pWindow = new SPWindowNpcEvent(WIID_NPC_EVENT, 315, 40, 232, 441, m_pSubGameZone);
		m_pWindowNpcEvent = pWindow;
		pWindow->Hide();
	}
	else {
		pWindow = new SPWindowNpcEvent(WIID_NPC_EVENT, 315, 40, 232, 401, m_pSubGameZone);
		m_pWindowNpcEvent = pWindow;
		pWindow->Hide();
	}
	pWindow = NULL;

	pWindow = new SPWindowPvpReserve(WIID_PVPRESERVE, 8, 90, 288, 287, m_pSubGameZone);
	pWindow->Hide();
	m_pWindowPvpReserve = pWindow;
	pWindow = NULL;

	pWindow = new SPWindowPvpEnterWait(WIID_PVPENTERWAIT, 208, 110, 384, 161, m_pSubGameZone);
	pWindow->SetImage("DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG", 1, 350);
	pWindow->Hide();
	m_pWindowPvpEnterWait = pWindow;
	pWindow = NULL;

	pWindow = new SPWindowPvpLoadWait(WIID_PVP_LOADWAIT, 0, 0, 800, 600, m_pSubGameZone);
	pWindow->Hide();
	pWindow = NULL;

	pWindow = new SPWindowPvpRank(WIID_PVP_RANKING, 636, 129, 164, 201, m_pSubGameZone);
	pWindow->Hide();
	pWindow = NULL;

	pWindow = new SPWindowPvpResult(WIID_PVP_RESULT, 148, 119, 511, 420, m_pSubGameZone);
	pWindow->Hide();
	m_pWindowPvpResult = pWindow ;
	pWindow = NULL;

	pWindow = new SPWindowPvpTimmer(WIID_PVP_TIMMER, 323, 93, 142, 52, m_pSubGameZone);
	pWindow->Hide();
	pWindow = NULL;

	//	Buff
	pWindow	=	new SPWindowBuff( WIID_BUFF , 201 , 40 , 337 , 25 , m_pSubGameZone );
	pWindow	=	NULL;

	////	Help
	//pWindow	=	new SPWindowHelp( WIID_HELP , 144 , 119 , 512 , 362 , m_pSubGameZone );
	//pWindow	=	NULL;

	//	Shortcut
	//pWindow	=	new SPWindowShortcut( WIID_SHORTCUT , 762 , 566 , 34 , 30 , m_pSubGameZone );
	//pWindow	=	new SPWindowShortcut( WIID_SHORTCUT , 725 , 566 , 34 + 34, 30 , m_pSubGameZone );
	//pWindow	=	NULL;

	// AJJIYA [9/8/2005]
	//[2007/8/2]- 컷인에 들어가는 컨트롤은 이것보다 앞에 있어야 한다.
	// 채팅사이햨E이벤트 컖E철㏏??창이 제일 밑에 있엉雹 한다.!!!
	m_pWindowChat	=	new SPWindowChat( WNDID_CENTERCHAT, WIID_CHAT, 0 , 95 , 495 , 505 , m_pSubGameZone);

	pWindow = new SPWindowCashShop(WIID_CASHSHOP, 0, 0, 800, 600, m_pSubGameZone);
	pWindow->Hide();
	pWindow = NULL ;	

	pWindow = new SPWindowJoyStick(WIID_JOYSTICK, 100, 100, 398, 444, m_pSubGameZone);
	pWindow->SetMoveable(true);
	pWindow->Hide();
	pWindow = NULL;	

	pWindow = new SPWindowStory(WIID_STORY, 0, 0, 800, 600, m_pSubGameZone);
	pWindow->Hide();
	pWindow = NULL ;
	

	//SetToggleWindow();

	//////////////////////////////////////////////////////////////////////////	
	// SubGame Lobby 

	pWindow = new SPWindowLobby(WIID_LOBBY, 0, 0, 800, 600, m_pSubGameLobby);
	pWindow->Show();
	pWindow = NULL;

	pWindow = new SPWindowError(WIID_ERROR, 280, 202, 254, 108, m_pSubGameLobby);
	pWindow->Hide();
	pWindow = NULL;

	delete g_UIManager;

	return true;
}

void SPInterfaceManager::CreatePrologueWindow()
{	
	if( m_pSubGamePrologue == NULL )
		return;

	ReleasePrologueWindow();
	
	SPWindow* pkWindow;
	pkWindow = new SPWindowPrologue(WIID_PROLOGUE, 0, 0, 800, 600, m_pSubGamePrologue);

	pkWindow = new SPWindowError(WIID_ERROR, 280, 202, 254, 108, m_pSubGamePrologue);
	pkWindow->Hide();
}

void SPInterfaceManager::ReleasePrologueWindow()
{
	if( m_pSubGamePrologue )
		m_pSubGamePrologue->CleanChildAll();
}

void SPInterfaceManager::SetToggleWindow()
{
	//	AJJIYA [9/8/2005]
	//	순서창 ( 아이템 , 장틒E, 스킬 , 정보 , 퀘스트 , 팀 ) 은 순서에 맞게 뜬다.

	INSTANCE_ID		tStartID	=	WIID_ITEM;
	INSTANCE_ID		tStepID		=	0x00001000;
	INSTANCE_ID		tEndID		=	WIID_TEAM + tStepID;

	while( tStartID != tEndID )
	{
		InsertToggleWindow(	tStartID			,	WIID_CHANNEL_CHANGE	);
		InsertToggleWindow(	tStartID			,	WIID_FRIEND			);
		InsertToggleWindow(	tStartID			,	WIID_PC_TRADE		);
		InsertToggleWindow(	tStartID			,	WIID_PC_INFO		);
		InsertToggleWindow(	tStartID			,	WIID_EXIT			);
		InsertToggleWindow(	tStartID			,	WIID_OPTION			);
		InsertToggleWindow( tStartID			,	WIID_HELP			);

		InsertToggleWindow(	WIID_CHANNEL_CHANGE	,	tStartID			);
		InsertToggleWindow(	WIID_FRIEND			,	tStartID			);
		InsertToggleWindow(	WIID_PC_TRADE		,	tStartID			);
		InsertToggleWindow(	WIID_PC_INFO		,	tStartID			);
		InsertToggleWindow(	WIID_EXIT			,	tStartID			);
		InsertToggleWindow(	WIID_OPTION			,	tStartID			);
		InsertToggleWindow( WIID_HELP			,	tStartID			);

		tStartID	=	tStartID	+	tStepID;
	}

	//	AJJIYA [9/8/2005]
	//	독립창은 단독으로 뜬다. 순서창 닫콅E

	//	채널
	InsertToggleWindow(	WIID_CHANNEL_CHANGE	,		WIID_FRIEND			);
	InsertToggleWindow(	WIID_CHANNEL_CHANGE	,		WIID_PC_TRADE		);
	InsertToggleWindow(	WIID_CHANNEL_CHANGE	,		WIID_PC_INFO		);
	InsertToggleWindow(	WIID_CHANNEL_CHANGE	,		WIID_EXIT			);
	InsertToggleWindow(	WIID_CHANNEL_CHANGE	,		WIID_NPCTALK		);
	InsertToggleWindow(	WIID_CHANNEL_CHANGE	,		WIID_SHOPTALK		);
	InsertToggleWindow(	WIID_CHANNEL_CHANGE	,		WIID_NPCTRADE		);	
	InsertToggleWindow( WIID_CHANNEL_CHANGE	,		WIID_OPTION			);
	InsertToggleWindow( WIID_CHANNEL_CHANGE ,		WIID_HELP			);

	//	친구
	InsertToggleWindow(	WIID_FRIEND			,		WIID_CHANNEL_CHANGE	);
	InsertToggleWindow(	WIID_FRIEND			,		WIID_PC_TRADE		);
	InsertToggleWindow(	WIID_FRIEND			,		WIID_PC_INFO		);
	InsertToggleWindow(	WIID_FRIEND			,		WIID_EXIT			);
	InsertToggleWindow(	WIID_FRIEND			,		WIID_NPCTALK		);
	InsertToggleWindow(	WIID_FRIEND			,		WIID_SHOPTALK		);
	InsertToggleWindow(	WIID_FRIEND			,		WIID_NPCTRADE		);	
	InsertToggleWindow( WIID_FRIEND			,		WIID_OPTION			);
	InsertToggleWindow( WIID_FRIEND			,		WIID_HELP			);

	//	트레이탛E
	InsertToggleWindow(	WIID_PC_TRADE		,		WIID_CHANNEL_CHANGE	);
	InsertToggleWindow(	WIID_PC_TRADE		,		WIID_FRIEND			);
	InsertToggleWindow(	WIID_PC_TRADE		,		WIID_PC_INFO		);
	InsertToggleWindow(	WIID_PC_TRADE		,		WIID_EXIT			);
	InsertToggleWindow(	WIID_PC_TRADE		,		WIID_NPCTALK		);
	InsertToggleWindow(	WIID_PC_TRADE		,		WIID_SHOPTALK		);
	InsertToggleWindow(	WIID_PC_TRADE		,		WIID_NPCTRADE		);	
	InsertToggleWindow( WIID_PC_TRADE		,		WIID_OPTION			);
	InsertToggleWindow( WIID_PC_TRADE		,		WIID_HELP			);
	InsertToggleWindow( WIID_PC_TRADE		,		WIID_WAYPOINT		);
	InsertToggleWindow( WIID_PC_TRADE		,		WIID_PET			);

	//	사퓖E?정보창
	InsertToggleWindow(	WIID_PC_INFO		,		WIID_CHANNEL_CHANGE	);
	InsertToggleWindow(	WIID_PC_INFO		,		WIID_FRIEND			);
	InsertToggleWindow(	WIID_PC_INFO		,		WIID_PC_TRADE		);
	InsertToggleWindow(	WIID_PC_INFO		,		WIID_EXIT			);
	InsertToggleWindow(	WIID_PC_INFO		,		WIID_NPCTALK		);
	InsertToggleWindow(	WIID_PC_INFO		,		WIID_SHOPTALK		);
	InsertToggleWindow(	WIID_PC_INFO		,		WIID_NPCTRADE		);
	InsertToggleWindow( WIID_PC_INFO		,		WIID_OPTION			);
	InsertToggleWindow( WIID_PC_INFO		,		WIID_HELP			);

	//	종료창
	InsertToggleWindow(	WIID_EXIT			,		WIID_CHANNEL_CHANGE	);
	InsertToggleWindow(	WIID_EXIT			,		WIID_FRIEND			);
	InsertToggleWindow(	WIID_EXIT			,		WIID_PC_TRADE		);
	InsertToggleWindow(	WIID_EXIT			,		WIID_PC_INFO		);
	InsertToggleWindow(	WIID_EXIT			,		WIID_NPCTALK		);
	InsertToggleWindow(	WIID_EXIT			,		WIID_SHOPTALK		);
	InsertToggleWindow(	WIID_EXIT			,		WIID_NPCTRADE		);	
	InsertToggleWindow( WIID_EXIT			,		WIID_OPTION			);
	InsertToggleWindow( WIID_EXIT			,		WIID_HELP			);
	InsertToggleWindow( WIID_EXIT			,		WIID_WAYPOINT		);
	InsertToggleWindow( WIID_EXIT			,		WIID_PET			);

	// 옵션창
	InsertToggleWindow( WIID_OPTION			,		WIID_EXIT			);
	InsertToggleWindow(	WIID_OPTION			,		WIID_CHANNEL_CHANGE	);
	InsertToggleWindow(	WIID_OPTION			,		WIID_FRIEND			);
	InsertToggleWindow(	WIID_OPTION			,		WIID_PC_TRADE		);
	InsertToggleWindow(	WIID_OPTION			,		WIID_PC_INFO		);
	InsertToggleWindow( WIID_OPTION			,		WIID_HELP			);

	//	?E좍?
	InsertToggleWindow(	WIID_HELP			,		WIID_CHANNEL_CHANGE	);
	InsertToggleWindow(	WIID_HELP			,		WIID_FRIEND			);
	InsertToggleWindow(	WIID_HELP			,		WIID_PC_TRADE		);
	InsertToggleWindow(	WIID_HELP			,		WIID_PC_INFO		);
	InsertToggleWindow(	WIID_HELP			,		WIID_NPCTALK		);
	InsertToggleWindow(	WIID_HELP			,		WIID_SHOPTALK		);
	InsertToggleWindow(	WIID_HELP			,		WIID_NPCTRADE		);	
	InsertToggleWindow( WIID_HELP			,		WIID_EXIT			);
	InsertToggleWindow( WIID_HELP			,		WIID_OPTION			);
}

SPWindow* SPInterfaceManager::GetNoticeBox()
{
	return m_pNoticeBox;
}

bool SPInterfaceManager::SetNotice(const char* pszMsg, SPWindow* pSender)
{
	return ((SPNoticeBox*)m_pNoticeBox)->Notice(pszMsg, pSender, NOTICE_TYPE_YESNO);
}

bool SPInterfaceManager::SetNotice(const char* pszMsg, SPGameObject* pSender)
{
	return ((SPNoticeBox*)m_pNoticeBox)->Notice(pszMsg, pSender, NOTICE_TYPE_YESNO);
}

bool SPInterfaceManager::SetNumNotice(const char* pszMsg, SPWindow* pSender)
{
	return ((SPNoticeBox*)m_pNoticeBox)->Notice(pszMsg, pSender, NOTICE_TYPE_YESNOSLIDER);
}

bool SPInterfaceManager::SetNumNotice(const char* pszMsg, SPGameObject* pSender)
{
	return ((SPNoticeBox*)m_pNoticeBox)->Notice(pszMsg, pSender, NOTICE_TYPE_YESNOSLIDER);
}

bool SPInterfaceManager::SetNoticeMsg(const char* pszMsg, SPWindow* pSender)
{
	return ((SPNoticeBox*)m_pNoticeBox)->Notice(pszMsg, pSender, NOTICE_TYPE_MSG);
}

bool SPInterfaceManager::SetNoticeMsg(const char* pszMsg, SPGameObject* pSender)
{
	return ((SPNoticeBox*)m_pNoticeBox)->Notice(pszMsg, pSender, NOTICE_TYPE_MSG);
}

bool SPInterfaceManager::SetNotice(const char* pszMsg, SPWindow* pSender, NOTICE_TYPE iType, int iShowTime /*= 0*/, SPIM IMDefaultAnswer /*= SPIM_NOTICE_NO*/)
{
	return ((SPNoticeBox*)m_pNoticeBox)->Notice(pszMsg, pSender, iType, iShowTime, DT_CENTER | DT_VCENTER, IMDefaultAnswer);
}

bool SPInterfaceManager::SetNotice(const char* pszMsg, SPGameObject* pSender, NOTICE_TYPE iType, int iShowTime /*= 0*/ , SPGM GMDefaultAnswer /*= SPGM_NOTICE_NO*/)
{
	return ((SPNoticeBox*)m_pNoticeBox)->Notice(pszMsg, pSender, iType, iShowTime, DT_CENTER | DT_VCENTER, GMDefaultAnswer);
}

bool SPInterfaceManager::ForceReply()
{
	if(m_pNoticeBox && m_pNoticeBox->IsShow())
		return ((SPNoticeBox*)m_pNoticeBox)->ForceReply();

	return false;
}

bool SPInterfaceManager::SetNoticeHide()
{
	ForceReply();
	((SPNoticeBox*)m_pNoticeBox)->Hide();	
	return true;
}

void SPInterfaceManager::InsertToggleWindow(unsigned int uiKey, unsigned int uiValue)
{
	SPWindow* pkWindow;
	pkWindow = m_pSubGameZone->Find(uiValue, true);
	if( pkWindow == NULL )
		return;

	map< unsigned int, vector< SPWindow* > >::iterator iterKey;
	iterKey = m_mWindowToggleList.find(uiKey);

	if( iterKey == m_mWindowToggleList.end() )
	{
		vector< SPWindow* > vWindowList;
		vWindowList.push_back(pkWindow);
		
		m_mWindowToggleList.insert(make_pair(uiKey, vWindowList));
		vWindowList.clear();
	}
	else
		iterKey->second.push_back(pkWindow);
}

void SPInterfaceManager::RenderTextDebug()
{
	SPManager::RenderTextDebug();
	//m_pRoot->RenderTextDebug();
}

void SPInterfaceManager::RenderText()
{
	SPManager::RenderText();
	//m_pRoot->RenderText();
}

void SPInterfaceManager::Process(float fTime)
{	
	SPManager::Process(fTime);	
	ProcessHotKey();	
	
#ifdef _JOY_PAD
	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_JOYPAD)) {
		ProcessJoyPad(fTime);
		ProcessJoyPadButtons(fTime);	
	}
#endif
	
	
	m_MouseCursor.Process(fTime);	
	if(m_ppCurSubgame && (*m_ppCurSubgame)) {
		(*m_ppCurSubgame)->Process(fTime);
		if(m_bCutIn)
			CutInProcess(fTime);

		if(m_pNoticeBox)
			m_pNoticeBox->Process(fTime);
	}

	if(m_pSoundArchive)
		m_pSoundArchive->Process(fTime, m_PtSoundPos);
}

bool SPInterfaceManager::ToggleChat()
{
	if(g_pInterfaceManager->GetIMEPool()->IsComposition() == true)
		return true;

	if(m_pWindowFocusIn && m_pWindowFocusIn->GetClassID() == WNDID_CTRL_MULTILINE_EDIT) { // MultiLine Edit can use return key..
		BOOL BEnableEnter = m_pWindowFocusIn->SPSendMessage(SPIM_GET_ENTER_ENABLE, 0, 0) ;
		if(BEnableEnter == TRUE)  
			return true;
	}

	m_bKeyInput = m_pWindowChat->SPSendMessage( SPIM_WNDCHAT_TOGGLE ) ? true : false;

	if( m_bKeyInput == false && m_pWindowTalkingRoom != NULL )	//  [4/29/2008 AJJIYA]
	{
		if( m_pWindowTalkingRoom->SPSendMessage( SPIM_KEY_RETURN ) > 0 )
			m_bKeyInput = true;
	}

	return true;
}

void SPInterfaceManager::ProcessHotKey()
{
	if( m_bActiveErrorMsg )
		return;

	if(g_pInputManager->GetJoyPadManager()->IsKeyPass())
		return;

	SPInputStruct* pInputStruct = g_pInputManager->GetInputStruct();

	if( IsKeyInput() == FALSE )
	{
		m_iKeyShift			= pInputStruct->ikLShift.InputState;		
		m_iKeyItem			= pInputStruct->ikI.InputState;
		m_iKeyEquip			= pInputStruct->ikE.InputState;
		m_iKeySkill			= pInputStruct->ikK.InputState;
		m_iKeyStatus		= pInputStruct->ikT.InputState;
		m_iKeyFriend		= pInputStruct->ikB.InputState;
		m_iKeyQuest			= pInputStruct->ikQ.InputState;
		m_iKeyOption		= pInputStruct->ikO.InputState;
		m_iKeyTeam			= pInputStruct->ikP.InputState;
		m_iKeyHelp			= pInputStruct->ikH.InputState;
		m_iKeyWorldMap		= pInputStruct->ikW.InputState;
		m_iKeyWorldMapStage	= pInputStruct->ikM.InputState;
		m_iKeyEnchant		= pInputStruct->ikN.InputState;
		m_iKeyItemUpgrade	= pInputStruct->ikU.InputState;
//		m_iKeyUserShopSell  = pInputStruct->ikL.InputState;
#ifdef _DEBUG		
//		m_iKeyUserShopBuy	= pInputStruct->ikY.InputState;
		
#endif
		if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD)  )
			m_iKeyUserGuildInfo	= pInputStruct->ikG.InputState;
		m_iKeyWayPoint		= pInputStruct->ikY.InputState;
		m_iKeyPet			= pInputStruct->ikL.InputState;
		m_iKeyGuideBook		= pInputStruct->ikF10.InputState;
		m_iKeyChattingRoom	= pInputStruct->ikF11.InputState;
		m_iKeyLove			= pInputStruct->ikJ.InputState;
	}
	else
	{
		m_iKeyShift			= INPUT_UNPRESS_REPEAT;
		m_iKeyItem			= INPUT_UNPRESS_REPEAT;
		m_iKeyEquip			= INPUT_UNPRESS_REPEAT;
		m_iKeySkill			= INPUT_UNPRESS_REPEAT;
		m_iKeyStatus		= INPUT_UNPRESS_REPEAT;
		m_iKeyFriend		= INPUT_UNPRESS_REPEAT;
		m_iKeyQuest			= INPUT_UNPRESS_REPEAT;
		m_iKeyOption		= INPUT_UNPRESS_REPEAT;
		m_iKeyTeam			= INPUT_UNPRESS_REPEAT;
		m_iKeyHelp			= INPUT_UNPRESS_REPEAT;
		m_iKeyEnchant		= INPUT_UNPRESS_REPEAT;
		m_iKeyItemUpgrade	= INPUT_UNPRESS_REPEAT;
//		m_iKeyUserShopSell  = INPUT_UNPRESS_REPEAT;
#ifdef _DEBUG		
//		m_iKeyUserShopBuy	= INPUT_UNPRESS_REPEAT;
		
#endif
		if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD)  )
			m_iKeyUserGuildInfo	= INPUT_UNPRESS_REPEAT;
		m_iKeyWayPoint		= INPUT_UNPRESS_REPEAT;
		m_iKeyPet			= INPUT_UNPRESS_REPEAT;
		m_iKeyGuideBook		= INPUT_UNPRESS_REPEAT;
		m_iKeyChattingRoom	= INPUT_UNPRESS_REPEAT;
		m_iKeyLove			= INPUT_UNPRESS_REPEAT;
	}

	m_iKeyTab			= pInputStruct->ikTab.InputState;		// 채팅에서 아이탛E창에서 메세햨E창으로 넘기콅E위해 AJJIYA [9/6/2005]
	m_iKeyEscape		= pInputStruct->ikEscape.InputState;
	m_iKeyReturn		= pInputStruct->ikReturn.InputState;

	bool bRetVal;
	BOOL BShift = FALSE;
	if(m_iKeyShift >= INPUT_PRESS_REPEAT) { 
		BShift = TRUE;
	}

	if(m_iKeyTab == INPUT_PRESS) {
		if(m_pWindowActive) {
			m_pWindowActive->SPSendMessage(SPIM_KEY_TAB, BShift);
		}
	}

#ifdef _DEBUG			// 디버그코드 [4/2/2008 AJJIYA]
	if( pInputStruct != NULL )
	{
		if( pInputStruct->ikLShift.InputState >= INPUT_PRESS_REPEAT && pInputStruct->ikF10.InputState == INPUT_PRESS )
			SetSubGame( SUBGAME_ZONE );
	}
#endif

	switch(m_SubGameID) {
	case SUBGAME_LOGGIN	:	
	case SUBGAME_LOBBY	:		
	case SUBGAME_LOADING:		
		//{		
		//	if(m_iKeyReturn == INPUT_PRESS) {
		//		if(m_pWindowActive) m_pWindowActive->SPSendMessage(SPIM_KEY_RETURN);
		//	}
		//}
		break;

	case SUBGAME_ZONE:
		{
			if( m_bCashShop || m_bBlockHotKey )
				break;

			if( m_bCutIn )			break;			//[2005/5/21] - 컷인시 단축키 막음

			if( g_pGOBManager->GetLocalPlayer() )
			{
				if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE) break;	//트레이드시 키 막음
				if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP) break;		//내가 상점개설자
				if(g_pGOBManager->GetLocalPlayer()->GetUserShopID()) break;							//내가 상점구매자

				if(SPCashTradeManager::GetInstance()->GetCashTradeState() == CASH_TRADE_STATE_WAIT) break; //필드 캐시구매 응답 대기중

			}

			if( m_iKeyItem		== INPUT_PRESS )	ShowWindow( WIID_ITEM,	SP_NONE ,	SP_TOGGLE_ON, true);
			if( m_iKeyEquip		== INPUT_PRESS )
			{
				ShowWindow( WIID_EXIT , SP_HIDE );	// AJJIYA [5/25/2005]
				ShowWindow( WIID_EQUIP, SP_NONE ,	SP_TOGGLE_ON, true);
			}
			if( m_iKeySkill		== INPUT_PRESS )	ShowWindow( WIID_SKILL,  SP_NONE ,	SP_TOGGLE_ON, true);
			if( m_iKeyStatus 	== INPUT_PRESS )	ShowWindow( WIID_STATUS, SP_NONE ,	SP_TOGGLE_ON, true);
			if( m_iKeyFriend	== INPUT_PRESS )	ShowWindow( WIID_FRIEND, SP_NONE ,	SP_TOGGLE_ON, true);
			if( m_iKeyQuest		== INPUT_PRESS )	ShowWindow( WIID_QUEST , SP_NONE ,	SP_TOGGLE_ON, true);
			if( m_iKeyOption	== INPUT_PRESS )	ShowWindow( WIID_OPTION , SP_NONE ,	SP_TOGGLE_ON, true);
			if( m_iKeyTeam		== INPUT_PRESS )	ShowWindow( WIID_TEAM , SP_NONE ,	SP_TOGGLE_ON, true);

			if( m_iKeyEnchant	== INPUT_PRESS )
			{
				SPChildWndSendMessage( WIID_ITEM , SPIM_ITEM_ENCHANT_KEYUP , NULL , NULL );
			}

			if( m_iKeyItemUpgrade	== INPUT_PRESS )
			{
				SPChildWndSendMessage( WIID_ITEM , SPIM_ITEM_UPGRADE_KEYUP , NULL , NULL );
			}
			
//			if( m_iKeyUserShopSell	== INPUT_PRESS )	ShowWindow( WIID_USERSHOP_SELL , SP_NONE ,	SP_TOGGLE_ON, true);
#ifdef _DEBUG
//			if( m_iKeyUserShopBuy	== INPUT_PRESS )	ShowWindow( WIID_USERSHOP_BUY , SP_NONE ,	SP_TOGGLE_ON, true);
			
#endif
			if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) )
				if( m_iKeyUserGuildInfo	== INPUT_PRESS )	ShowWindow( WIID_GUILD_INFO , SP_NONE ,	SP_TOGGLE_ON, true);
			
			if(m_iKeyWayPoint		== INPUT_PRESS ) {
				if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_WAYPOINT) )
					ShowWindow( WIID_WAYPOINT, SP_NONE, SP_TOGGLE_ON, true);
			}
			
			if( m_iKeyPet == INPUT_PRESS ){
				if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_PET) )
					ShowWindow( WIID_PET, SP_NONE, SP_TOGGLE_ON, true);
			}

			if( m_iKeyGuideBook == INPUT_PRESS )
			{
				ShowWindow( WIID_GUIDE_BOOK	,	SP_NONE	,	SP_TOGGLE_ON	,	true	);
			}

			if( m_iKeyHelp		== INPUT_PRESS )
			{	
				ShowWindow( WIID_HELP ,	SP_NONE ,	SP_TOGGLE_ON, true);
				//SPChildWndSendMessage( WIID_SHORTCUT , SPIM_HELP_SHOW , NULL , NULL );
			}

			if( m_iKeyWorldMap	== INPUT_PRESS )
			{
				SPChildWndSendMessage( WIID_MINIMAP , SPIM_WORLDMAP_WORLD , NULL , NULL );
			}

			if( m_iKeyWorldMapStage	== INPUT_PRESS )
			{
				SPChildWndSendMessage( WIID_MINIMAP , SPIM_WORLDMAP_STAGE , NULL , NULL );
			}

			if( m_iKeyChattingRoom == INPUT_PRESS )
			{
				ShowWindow( WIID_CHATTING_ROOM	,	SP_NONE	,	SP_TOGGLE_ON	,	true	);
			}

			if( m_iKeyLove == INPUT_PRESS )
			{
				ShowWindow( WIID_LOVE	,	SP_NONE	,	SP_TOGGLE_ON	,	true	);
			}
		}
		break;
	}
}

void SPInterfaceManager::ProcessJoyPad(float fTime)
{
	g_pInputManager->GetJoyPadManager()->SetPadMouseMove(false);

	if(!g_pInput->IsEquipJoystick()) 
		return;

	SPInputStructJoyStick* pStick = g_pInputManager->GetInputStructJoyStick();
	if(pStick == NULL)	return;

	if(g_pInputManager->GetJoyPadManager()->GetJoyStickMode() == JOYSTICK_MODE_ANALOG){	
		if(pStick->ibLX.iValue < -100) {
			if(pStick->ibLX.iValue <= -1000){
				m_iCursorX -= 7;
			}
			//else if(pStick->ibLX.iValue <= -800){
			//	m_iCursorX -= 5;
			//}
			else if(pStick->ibLX.iValue <= -100){
				m_iCursorX -= 1;
			}

			if(m_iCursorX < 0)
				m_iCursorX = 0;

			g_pInputManager->GetJoyPadManager()->SetPadMouseMove(true);
		}	
		else if(pStick->ibLX.iValue > 100) {
			if(pStick->ibLX.iValue >= 1000){
				m_iCursorX += 7;
			}
			//else if(pStick->ibLX.iValue >= 800){
			//	m_iCursorX += 5;
			//}
			else if(pStick->ibLX.iValue >= 100){
				m_iCursorX += 1;
			}

			if(m_iCursorX > g_pVideo->GetScreenLenX() - 8)
				m_iCursorX = g_pVideo->GetScreenLenX() - 8;

			g_pInputManager->GetJoyPadManager()->SetPadMouseMove(true);
		}

		if(pStick->ibLY.iValue < -100) {
			if(pStick->ibLY.iValue <= -1000){
				m_iCursorY -= 7;
			}
			//else if(pStick->ibLY.iValue <= -800){
			//	m_iCursorY -= 5;
			//}
			else if(pStick->ibLY.iValue <= -100){
				m_iCursorY -= 1;
			}

			if(m_iCursorY < 0)
				m_iCursorY = 0;

			g_pInputManager->GetJoyPadManager()->SetPadMouseMove(true);
		}	
		else if(pStick->ibLY.iValue > 100) {
			if(pStick->ibLY.iValue >= 1000){
				m_iCursorY += 7;
			}
			//else if(pStick->ibLY.iValue >= 800){
			//	m_iCursorY += 5;
			//}
			else if(pStick->ibLY.iValue >= 100){
				m_iCursorY += 1;
			}

			if(m_iCursorY > g_pVideo->GetScreenLenY() - 8)
				m_iCursorY = g_pVideo->GetScreenLenY() - 8;

			g_pInputManager->GetJoyPadManager()->SetPadMouseMove(true);
		}

		if(g_pInputManager->GetJoyPadManager()->IsPadMouseMove()) {
			PerformMouseMove((float)m_iCursorX, (float) m_iCursorY);
			//POINT pt;
			//::GetCursorPos(&pt);
			//::SetCursorPos(m_iCursorX + pt.x, m_iCursorY + pt.y);
			
			POINT pt;
			RECT rect;			
			::GetWindowRect(g_hWnd, &rect);				
			pt.x = m_iCursorX;
			pt.y = m_iCursorY;
			pt.x += rect.left;
			pt.y += rect.top;
			pt.y += g_pInputManager->GetJoyPadManager()->GetFixHeight();
			pt.x += g_pInputManager->GetJoyPadManager()->GetFixWidth();
			::SetCursorPos(pt.x, pt.y);
		}

		std::vector<SPJoyPadButtonInfo*>* pvButton = g_pInputManager->GetJoyPadManager()->GetPadButton(PAD_BUTTON_KEY_TYPE_MOUSE);
		std::vector<SPJoyPadButtonInfo*>::iterator iter;
		iter = pvButton->begin();

		int iKeyIndex = 0xFFFFFFFF;
		for(; iter != pvButton->end(); ++iter) {
			//iKeyIndex = (*iter)->m_iKey - BUTTON_KEY_MOUSE_START;
			if(m_bJoyPadLPush == false && (*iter)->m_iKey == BUTTON_KEY_MOUSE_L && (*iter)->m_iInputState == INPUT_REPEAT) {
				PerformMouseLDown((float)m_iCursorX, (float) m_iCursorY);
				DXUTOutputDebugString("\t * JoyPad LDownPress... Down\n");
				m_bJoyPadLPush = true;
			}
			
			if(m_bJoyPadLPush && (*iter)->m_iKey == BUTTON_KEY_MOUSE_L && (*iter)->m_iInputState == INPUT_UNREPEAT){
				PerformMouseLUp((float)m_iCursorX, (float) m_iCursorY);
				DXUTOutputDebugString("\t * JoyPad LDownPress... Up\n");
				m_bJoyPadLPush = false;
			}

			if(m_bJoyPadRPush == false && (*iter)->m_iKey == BUTTON_KEY_MOUSE_R && (*iter)->m_iInputState == INPUT_REPEAT) {
				PerformMouseRDown((float)m_iCursorX, (float) m_iCursorY);
				DXUTOutputDebugString("\t * JoyPad RDownPress... Down\n");
				m_bJoyPadRPush = true;
			}
			
			if(m_bJoyPadRPush && (*iter)->m_iKey == BUTTON_KEY_MOUSE_R && (*iter)->m_iInputState == INPUT_UNREPEAT) {
				PerformMouseRUp((float)m_iCursorX, (float) m_iCursorY);
				DXUTOutputDebugString("\t * JoyPad RDownPress... Up\n");
				m_bJoyPadRPush = false;
			}
		}		
	}	
}


void SPInterfaceManager::ProcessJoyPadButtons(float fTime)
{
	if( m_bActiveErrorMsg )
		return;

	if(m_SubGameID != SUBGAME_ZONE)				return;	
	if(g_pGOBManager->GetLocalPlayer() == NULL) return;	
	if( m_bCashShop || m_bBlockHotKey ) 			return;

	std::vector<SPJoyPadButtonInfo*>* pvButton = g_pInputManager->GetJoyPadManager()->GetPadButton(PAD_BUTTON_KEY_TYPE_INTERFACE);
	std::vector<SPJoyPadButtonInfo*>::iterator iter;
	iter = pvButton->begin();
	for(; iter != pvButton->end(); ++iter) {
		if((*iter)->m_iKey == BUTTON_KEY_INTERFACE_ESC || (*iter)->m_iKey == BUTTON_KEY_INTERFACE_ENTER || 
			(*iter)->m_iKey == BUTTON_KEY_INTERFACE_TAB) 
		{
			switch((*iter)->m_iKey) {
				case BUTTON_KEY_INTERFACE_ESC:
					if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
						WndEscapeProc((WPARAM)0, (LPARAM)0);
						(*iter)->m_bPush = true;
					}
					if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
						(*iter)->m_bPush = false;
					}
					break;
				case BUTTON_KEY_INTERFACE_ENTER:
					if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
						WndReturnProc((WPARAM)0, (LPARAM)0);
						(*iter)->m_bPush = true;
					}
					if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
						(*iter)->m_bPush = false;
					}
					break;
				case BUTTON_KEY_INTERFACE_TAB:
					if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
						if( g_pGOBManager->GetLocalPlayer() && g_pGOBManager->GetLocalPlayer()->GetInvenArchive() )
							g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->SendReqWeaponExchange();
						(*iter)->m_bPush = true;
					}
					if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
						(*iter)->m_bPush = false;
					}
					break;
				default:
					break;
			}
		}
		else {			
			if( m_bCutIn )								continue;			//[2005/5/21] - 컷인시 단축키 막음	
			if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE) continue;	//트레이드시 키 막음
			if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP) continue;		//내가 상점개설자
			if(g_pGOBManager->GetLocalPlayer()->GetUserShopID()) continue;						//내가 상점구매자

			switch((*iter)->m_iKey) {
				//I
			case BUTTON_KEY_INTERFACE_I:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					ShowWindow( WIID_ITEM,	SP_NONE ,	SP_TOGGLE_ON, true);
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_E:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					ShowWindow( WIID_EQUIP,	SP_NONE ,	SP_TOGGLE_ON, true);
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_T:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					ShowWindow( WIID_STATUS,	SP_NONE ,	SP_TOGGLE_ON, true);
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_K:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					ShowWindow( WIID_SKILL,	SP_NONE ,	SP_TOGGLE_ON, true);
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_Q:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					ShowWindow( WIID_QUEST,	SP_NONE ,	SP_TOGGLE_ON, true);
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_O:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					ShowWindow( WIID_OPTION,	SP_NONE ,	SP_TOGGLE_ON, true);
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_P:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					ShowWindow( WIID_TEAM,	SP_NONE ,	SP_TOGGLE_ON, true);
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_H:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					ShowWindow( WIID_TEAM,	SP_NONE ,	SP_TOGGLE_ON, true);
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_W:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					SPChildWndSendMessage( WIID_MINIMAP , SPIM_WORLDMAP_WORLD , NULL , NULL );
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_M:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					SPChildWndSendMessage( WIID_MINIMAP , SPIM_WORLDMAP_STAGE , NULL , NULL );
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_N:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					SPChildWndSendMessage( WIID_ITEM , SPIM_ITEM_ENCHANT_KEYUP , NULL , NULL );
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_U:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					SPChildWndSendMessage( WIID_ITEM , SPIM_ITEM_UPGRADE_KEYUP , NULL , NULL );
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_Y:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_WAYPOINT) )
						ShowWindow( WIID_WAYPOINT, SP_NONE, SP_TOGGLE_ON, true);
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_L:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_PET) )
						ShowWindow( WIID_PET, SP_NONE, SP_TOGGLE_ON, true);
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			case BUTTON_KEY_INTERFACE_G:
				if((*iter)->m_iInputState == INPUT_REPEAT && (*iter)->m_bPush == false) {
					if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) )
						ShowWindow( WIID_GUILD_INFO , SP_NONE ,	SP_TOGGLE_ON, true);
					(*iter)->m_bPush = true;
				}
				if((*iter)->m_iInputState == INPUT_UNREPEAT && (*iter)->m_bPush){
					(*iter)->m_bPush = false;
				}
				break;
			default:
				break;
			}
		}		
	}
}


void SPInterfaceManager::Render(float fTime)
{
	SPManager::Render(fTime);	

	if( m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL )
		return;

	if(m_SubGameID == SUBGAME_ZONE) {		
		//(*m_ppCurSubgame)->RenderReverse(fTime);

		if(m_bCutIn) {
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SCROLL ||
				g_pEventManager->GetCutInState() == CUT_IN_STATE_RELEASE) 
			{
				(*m_ppCurSubgame)->RenderReverse(fTime);
				CutInRender(fTime);								//컷인 렌큱E
				m_pWindowChat->Render(fTime);					//컷인 위에 나오는 컨트롤 랜큱E
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SCROLL_END) {
				CutInRender(fTime);								//컷인 렌큱E
				m_pWindowChat->Render(fTime);					//컷인 위에 나오는 컨트롤 랜큱E
				m_pWindowPcTalk->Render(fTime);		//[2007/9/13]-Test
				m_pWindowNpcTalk->Render(fTime);				//NPC Talk 컨트롤 랜큱E				
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
			}			 
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_TALK){
				CutInRender(fTime);								//컷인 렌큱E
				m_pWindowChat->Render(fTime);					//컷인 위에 나오는 컨트롤 랜큱E
				m_pWindowPcTalk->Render(fTime);		//[2007/9/13]-Test
				m_pWindowNpcTalk->Render(fTime);				//NPC Talk 컨트롤 랜큱E				
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_TRADE) {
				CutInRender(fTime);								//컷인 렌큱E
				m_pWindowChat->Render(fTime);					//컷인 위에 나오는 컨트롤 랜큱E				
				m_pWindowNpcTrade->Render(fTime);
				//m_pWindowEly->Render(fTime);
				//m_pWindowItemSell->Render(fTime);
				m_pWindowInven->Render(fTime);
				m_pWindowEquip->Render(fTime);
				m_pWindowStatus->Render(fTime);				//상태창은 추후에...								
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}				
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowShopTalk->Render(fTime);
				m_pWindowToolTip->Render(fTime);				
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_STORAGE) {
				CutInRender(fTime);								//컷인 렌큱E
				m_pWindowChat->Render(fTime);					//컷인 위에 나오는 컨트롤 랜큱E				
				//m_pWindowEly->Render(fTime);
				m_pWindowInven->Render(fTime);
				m_pWindowEquip->Render(fTime);				
				m_pWindowStorage->Render(fTime);
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowShopTalk->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_GIFT) {
				CutInRender(fTime);								//컷인 렌큱E
				m_pWindowChat->Render(fTime);					//컷인 위에 나오는 컨트롤 랜큱E
				m_pWindowShopTalk->Render(fTime);
				//m_pWindowEly->Render(fTime);
				m_pWindowInven->Render(fTime);
				m_pWindowEquip->Render(fTime);				
				m_pWindowGift->Render(fTime);
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_QUEST) {
				CutInRender(fTime);								//컷인 렌큱E
				m_pWindowChat->Render(fTime);					//컷인 위에 나오는 컨트롤 랜큱E
				m_pWindowNpcQuest->Render(fTime);
				m_pWindowQuest->Render(fTime);
				m_pWindowQuestView->Render(fTime);
				m_pWindowShopTalk->Render(fTime);
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SAVEPOINT) {
				CutInRender(fTime);								//컷인 렌큱E
				m_pWindowChat->Render(fTime);					//컷인 위에 나오는 컨트롤 랜큱E
				m_pWindowPcTalk->Render(fTime);
				m_pWindowNpcTalk->Render(fTime);				//NPC Talk 컨트롤 랜큱E				
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
				m_pWindowSystemMessage->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_MAIL) {
				CutInRender(fTime);								//컷인 렌큱E
				m_pWindowChat->Render(fTime);					//컷인 위에 나오는 컨트롤 랜큱E
				m_pWindowShopTalk->Render(fTime);
				//m_pWindowEly->Render(fTime);
				m_pWindowMailMsg->Render(fTime);
				m_pWindowMailList->Render(fTime);				
				m_pWindowInven->Render(fTime);
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowToolTip->Render(fTime);			
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_BEAUTY) {
				CutInRender(fTime);								//컷인 렌큱E
				m_pWindowChat->Render(fTime);					//컷인 위에 나오는 컨트롤 랜큱E				
				m_pWindowBeauty->Render(fTime);
				m_pWindowInven->Render(fTime);
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
				m_pWindowShopTalk->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_GUILD_CREATE) {
				CutInRender(fTime);				
				m_pWindowChat->Render(fTime);
				m_pWindowShopTalk->Render(fTime);
				m_pWindowCreateGuild->Render(fTime);
				m_pwindowGuildEmblem->Render(fTime);
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_GUILD_DISSOLVE) {
				CutInRender(fTime);
				m_pWindowChat->Render(fTime);
				m_pWindowShopTalk->Render(fTime);
				m_pWindowDestroyGuild->Render(fTime);
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_GUILD_CROPS) {
				CutInRender(fTime);
				m_pWindowChat->Render(fTime);
				m_pWindowShopTalk->Render(fTime);
				m_pWindowGuildCrop->Render(fTime);
				m_pWindowInven->Render(fTime);
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NPC_PORTAL) {
				CutInRender(fTime);
				m_pWindowChat->Render(fTime);
				m_pWindowShopTalk->Render(fTime);
				m_pWindowNpcPortal->Render(fTime);
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NPC_CHANGE) {
				CutInRender(fTime);
				m_pWindowChat->Render(fTime);
				m_pWindowShopTalk->Render(fTime);
				m_pWindowNpcEventList->Render(fTime);
				m_pWindowNpcEvent->Render(fTime);
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_GUILD_LEVEL_UP) {
				CutInRender(fTime);
				m_pWindowChat->Render(fTime);
				m_pWindowShopTalk->Render(fTime);
				m_pWindowGuildLevelup->Render(fTime);
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_GUILD_EMBLEM) {
				CutInRender(fTime);
				m_pWindowChat->Render(fTime);				
				m_pwindowGuildEmblem->Render(fTime) ;
				if(g_pEventManager) {
					g_pEventManager->Render(fTime);
				}				
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowShopTalk->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_PVP_RESERVE) {
				CutInRender(fTime);
				m_pWindowChat->Render(fTime);
				if(m_pWindowPvpReserve) 
					m_pWindowPvpReserve->Render(fTime);				
				
				//if(g_pEventManager) {
				//	g_pEventManager->Render(fTime);
				//}

				if(m_pWindowPvpEnterWait)
					m_pWindowPvpEnterWait->Render(fTime);

				m_pWindowSystemMessage->Render(fTime);
				//m_pWindowShopTalk->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NPC_AUCTION) {
				CutInRender(fTime);
				m_pWindowChat->Render(fTime);			
				m_pWindowAuction->Render(fTime) ;
				// 인벤과 장비중에 활성화된것을 위에 그럼
				if( (m_pWindowFocusIn && (m_pWindowFocusIn->GetInstanceID() == m_pWindowEquip->GetInstanceID()) ) ||
					(m_pWindowActive && (m_pWindowActive->GetInstanceID() == m_pWindowEquip->GetInstanceID())) )	{
					m_pWindowInven->Render(fTime);
					m_pWindowEquip->Render(fTime);
				}
				else	{
					m_pWindowEquip->Render(fTime);
					m_pWindowInven->Render(fTime);
				}
				m_pWindowAuctionMSG->Render(fTime) ;
				
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
			else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_LOVE) {
				if( m_pWindowChat != NULL )
					m_pWindowChat->Render( fTime );

				if( m_pWindowLoveMsgView != NULL )
					m_pWindowLoveMsgView->Render( fTime );

				if( m_pWindowLoveMsgInput != NULL )
					m_pWindowLoveMsgInput->Render( fTime );
			}
			else {
				;
			}
			
			if(m_pNoticeBox)
				m_pNoticeBox->Render(fTime);

//			m_pWindowChat->Render(fTime);					//컷인 위에 나오는 컨트롤 랜큱E
		}
		else {
			// PVP Result Mode에서 채팅창 랜더링을 위해서 추가
			if( g_pkPvpManager->GetPvpStatus() == PVP_STATUS_RESULT )
			{
				if( m_pWindowPvpResult ) 
					m_pWindowPvpResult->Render(fTime);				
				m_pWindowChat->Render(fTime);
				m_pWindowSystemMessage->Render(fTime);
				m_pWindowToolTip->Render(fTime);
				if(m_pNoticeBox)
					m_pNoticeBox->Render(fTime);
			}
			else
			{
				(*m_ppCurSubgame)->RenderReverse(fTime);
				if(m_pNoticeBox)
					m_pNoticeBox->Render(fTime);
				m_pWindowToolTip->Render(fTime);
			}
		}
	} else {
		(*m_ppCurSubgame)->Render(fTime);

		if(m_pNoticeBox)
			m_pNoticeBox->Render(fTime);
		if( m_pWindowLBToolTip )
			m_pWindowLBToolTip->Render(fTime);
	}
	
	if(m_pCandVertical)
		m_pCandVertical->Render(fTime);
	
	m_MouseCursor.Render(fTime);

	g_pVideo->Flush();
}


//////////////////////////////////////////////////////////////////////////
// CutIn 컖E?
void SPInterfaceManager::CutInInit()
{
	m_bCutIn = false;

	m_fAccumulateRender = 0.0f;
	m_fCurDelay = 0.02f;
	
	m_iCutInAlpha = 0;
	m_iAlpha = 10;
	m_fCutInAlpha = 0.0f;

	SetRect(&m_rtDestTop, 0, 0, g_pVideo->GetScreenLenX(), g_pVideo->GetScreenLenY());

	//Load Texture Image
	std::string strTextureFile = "";
	//strTextureFile = RES_WORK_RESOURCE_PATH;
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	//g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pTexture);
	g_pVideo->GetTextureMgr()->GetTexture(strTextureFile.c_str(), &m_pTexture,TEX_INTERFACE);

	//SetRect(&m_rtSrcHide,	128, 392, 132, 396);
	SetRect(&m_rtSrcHide,	255, 21, 257, 23);
	ResetRect();
}


void SPInterfaceManager::CutInClear()
{
	SAFE_RELEASE(m_pTexture);
}


void SPInterfaceManager::CutInProcess (float fTime)
{
	m_fAccumulateRender += fTime;
	if( m_fAccumulateRender > m_fCurDelay) {
		m_fAccumulateRender -= m_fCurDelay;		
		
		if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SCROLL) {
			m_iCutInAlpha += 10;
			if(m_iCutInAlpha >= MAX_CUTIN_ALPHA)
				m_iCutInAlpha = MAX_CUTIN_ALPHA;

			m_fCutInAlpha = (float)m_iCutInAlpha / (float)255;


			if(m_iCutInAlpha == MAX_CUTIN_ALPHA/*m_bBottomStop && m_bTopStop*/){
				g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL_END);
				DXUTOutputDebugString("CUT_IN_STATE_SCROLL_END\n");

				//Cut In Scrool End의 경퓖ENpc Talk Window를 보여준다 
				if(g_pEventManager->GetCutInEvnetType() < 5) {
					SPWindow* pWindow = m_pSubGameZone->Find(WIID_NPCTALK, true);
					if( pWindow ) {
						if(!pWindow->IsShow()) 
							pWindow->Show();
					}					
				}
				else if(g_pEventManager->GetCutInEvnetType() == EVENT_MAILBOX) {
					g_pEventManager->SetCutInState(CUT_IN_STATE_MAIL);
					
					SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_SHOPTALK);
					//std::string strTemp = "Mode Mail Box...";
					std::string strMsg = "";
					if(g_pResourceManager->GetGlobalString(5007002)) {
						strMsg = g_pResourceManager->GetGlobalString(5007002);
					}					
					pWindow->Show();
					pWindow->SPSendMessage(SPIM_SET_SHOPTALK_MSG, (WPARAM)strMsg.c_str(), 0);					
				}
				else if(g_pEventManager->GetCutInEvnetType() == EVENT_BEAUTY) {
					g_pEventManager->SetCutInState(CUT_IN_STATE_BEAUTY);

					SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_SHOPTALK);
					//std::string strMsg = "Mode Beauty Shop...";
					std::string strMsg = "";
					if(g_pResourceManager->GetGlobalString(9007005)) {
						strMsg = g_pResourceManager->GetGlobalString(9007005);
					}					
					pWindow->Show();
					pWindow->SPSendMessage(SPIM_SET_SHOPTALK_MSG, (WPARAM)strMsg.c_str(), 0);
				}
				else if(g_pEventManager->GetCutInEvnetType() == EVENT_CROPS) {
					g_pEventManager->SetCutInState(CUT_IN_STATE_GUILD_CROPS);
					DISPLAY_CROP_UNIT Crop;					
					g_pGOBManager->GetGuildArchive()->GetCropImageByEvent(g_pEventManager->GetCurEvent()->id, -1, Crop);
					g_pEventManager->GetEventHandler()->SetNpcTalkImage(Crop.iCutInImageID);
					SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_SHOPTALK);
					pWindow->Show();
					pWindow->SPSendMessage(SPIM_SET_SHOPTALK_MSG, (WPARAM)Crop.strTalk.c_str(), 0);
				}
				else if(g_pEventManager->GetCutInEvnetType() == EVENT_CORE) {
					CUT_IN_STATE iState = g_pEventManager->GetNextCutInStateByCoreEvent();
					g_pEventManager->SetCutInState(iState);
					SPWindow* pWindow = NULL;
					switch(iState) {
						case CUT_IN_STATE_TALK:
						pWindow = m_pSubGameZone->Find(WIID_NPCTALK, true);
						if( pWindow ) {							
							if(!pWindow->IsShow()) 
								pWindow->Show();
						}
						case CUT_IN_STATE_TRADE:
							OnShowNpcShopWindow(SPIM_SET_NPCSHOP_SHOW, int(CUT_IN_STATE_TRADE));
							break;
						case CUT_IN_STATE_QUEST:							
						case CUT_IN_STATE_STORAGE:
						case CUT_IN_STATE_GIFT:
						case CUT_IN_STATE_NPC_PORTAL:
						case CUT_IN_STATE_NPC_CHANGE:
						case CUT_IN_STATE_PVP_RESERVE:
							g_pInterfaceManager->SPPerformMessage(SPIM_SET_NPCSHOP_SHOW, int(iState));
							break;
						case CUT_IN_STATE_GUILD_CREATE:
							if( !(g_pGOBManager->GetGuildArchive()->GetBasicAvailable() && SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD)) ) {
								//길드 생성 Window 를 Show 시킨다 
								g_pInterfaceManager->SPPerformMessage(SPIM_SET_NPCSHOP_SHOW, int(CUT_IN_STATE_GUILD_CREATE));
							}	
							break;
						case CUT_IN_STATE_GUILD_DISSOLVE:
							if(g_pGOBManager->GetGuildArchive()->CheckGuildDestroy() == 0) {
								//길드 해체 Window 를 Show 시킨다
								g_pInterfaceManager->SPPerformMessage(SPIM_SET_NPCSHOP_SHOW, int(CUT_IN_STATE_GUILD_DISSOLVE));
							}	
							break;												
						default:
							//case CUT_IN_STATE_GUILD_LEVEL_UP:
							//case CUT_IN_STATE_GUILD_EMBLEM:
							break;
					}
				}
				else {
					//Error
				}

				OnCutInInterfaceShow(0, 0);
			}
		}
		else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_RELEASE){			
			m_iCutInAlpha -= 30;
			if(m_iCutInAlpha <= 0)
				m_iCutInAlpha = 0;
		
			m_fCutInAlpha = (float)m_iCutInAlpha / (float)255;
			
			if(m_iCutInAlpha <= 0){
				g_pEventManager->SetEventPacketSendFlag(false);
				g_pEventManager->SetCutInState(CUT_IN_STATE_NULL);
				m_iCutInAlpha = 0;
			}
		}
	}	
}


void SPInterfaceManager::CutInRender (float fTime)
{
	if(m_pTexture)
	{
		m_pTexture->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fCutInAlpha));
		m_pTexture->RenderTexture(&m_rtDestTop, &m_rtSrcHide);
		//m_pTexture->RenderTexture(&m_rtDestBottom, &m_rtSrcHide);
		m_pTexture->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	
	}
}


void SPInterfaceManager::ResetRect()
{
	//SetRect(&m_rtDestTop,		0, 0, 800, 0);
	SetRect(&m_rtDestBottom,	0, 600, 800, 600);
	SetRect(&m_rtDestNPC,		0, 0, 0, 0);

	m_iCutInAlpha = 100;
	m_iAlpha = 10;
	m_fCutInAlpha = float(m_iCutInAlpha) / float(255);

	m_bTopStop = false;
	m_bBottomStop = false;
}


void SPInterfaceManager::SetCutIn(bool bCutIn /*= false*/)
{
	m_bCutIn = bCutIn;

	if(m_bCutIn) {
		ResetRect();

		//[2005/8/1]	컷인 모드로 들엉瞼게 되툈E메시햨E창을 닫는다?
		//this->ForceReply();		
	}
	else {		
		m_pWindowNpcTalk->Hide();					//CutIn 상황이 아닌경퓖ENpcTalkWindow를 숨긴다
		//OnCutInInterfaceShow(1, 0);
	}
}


bool SPInterfaceManager::IsCutIn()
{
	return m_bCutIn;
}

bool SPInterfaceManager::ShowWindow(INSTANCE_ID wndID, bool bShow, bool bToggle, bool bSound /*= false*/)
{
	if( m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL )
		return false;

	if( m_bActiveErrorMsg )
		return false;

	SPWindow *pWindow = (*m_ppCurSubgame)->Find( wndID , true);
	if(pWindow)
	{
		if(bToggle)
		{
			if(pWindow->IsShow())	
				pWindow->Hide();
			else
				pWindow->Show();			
		}
		else
		{
			if(pWindow->IsShow() == bShow) {
				return false;
			}

			if(bShow)
				pWindow->Show();
			else
				pWindow->Hide();
		}
		
		CheckShowWindow(wndID, pWindow->IsShow());
		OrderWindowPosMove();	// AJJIYA [9/8/2005]
		return true;
	}
	return false;
}

void 	SPInterfaceManager::CheckShowWindow(INSTANCE_ID wndID, bool bShow)
{
	if( !bShow )
		return;

	////[2005/7/19]
	//if( wndID == WIID_SKILL ||
	//	wndID == WIID_CHANNEL_CHANGE ||
	//	wndID == WIID_PC_INFO ||
	//	wndID == WIID_STORAGE )
	//{
	//	ShowWindow(wndID, false);
	//	return;
	//}

	map< unsigned int, vector< SPWindow* > >::iterator iterKey;
	iterKey = m_mWindowToggleList.find(wndID);
	if( iterKey == m_mWindowToggleList.end() )
		return;

	vector< SPWindow* >::iterator iterValue = iterKey->second.begin();
	while( iterValue != iterKey->second.end() )
	{
		if( (*iterValue) && (*iterValue)->IsShow() )
			(*iterValue)->Hide();

		++iterValue;
	}
}

bool	SPInterfaceManager::IsShowWindow(INSTANCE_ID wndID)
{
	if( m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL )
		return false;

	SPWindow *pkWindow = (*m_ppCurSubgame)->Find(wndID , true);
	if( pkWindow == NULL )
		return false;

	return pkWindow->IsShow();
}

void	SPInterfaceManager::OrderWindowPosMove()
{
	return; //무틒E?필퓖E없다

	if( m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL )
		return;

	if( g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL )
		return;

	INSTANCE_ID		tStartID	=	WIID_ITEM;
	INSTANCE_ID		tStepID		=	0x00001000;
	INSTANCE_ID		tEndID		=	WIID_TEAM + tStepID;
	SPWindow*		pWindow		=	NULL;

	int				iNextPosX	=	166;
	int				iNextPosY	=	200;

	int				iMaxPosX	=	800;

	int				iStartPosX	=	166;
	int				iStartPosY	=	114;

	RECT			rcWindow;

	int				iWindowSizeX	,	iWindowSizeY;

	while( tStartID != tEndID )
	{
		pWindow		=	(*m_ppCurSubgame)->Find( tStartID , true );
		tStartID	=	tStartID	+	tStepID;

		if( pWindow == NULL )
			continue;

		if( pWindow->IsShow() == TRUE )
		{
			pWindow->GetWindowRect( rcWindow );

			iWindowSizeX	=	rcWindow.right	-	rcWindow.left + 1;
			iWindowSizeY	=	rcWindow.bottom	-	rcWindow.top;

			if( iWindowSizeX + iStartPosX >= iMaxPosX )
			{
				iStartPosX	=	iNextPosX;
				iStartPosY	+=	iNextPosY;
			}

			pWindow->AdjustCoord( iStartPosX , iStartPosY );
			pWindow->SPSendMessage( SPIM_POS_UPDATE );

			iStartPosX	+=	iWindowSizeX;
		}
	}
}

void	SPInterfaceManager::SetMousePickup(SPTexture *icon, int nXSrcPos, int nYSrcPos)
{
	m_MouseCursor.SetMoveIconPos( icon, nXSrcPos, nYSrcPos);
}

void	SPInterfaceManager::ClearMouse()
{
	m_MouseCursor.ClearMoveIcon();
}

INSTANCE_ID SPInterfaceManager::MessageBox(const char* pszMsgText, int iType, INSTANCE_ID tMsgWnd, SPIM eMsg)
{
	for (int i=0; i<MAX_MESBOX; i++)
	{
		if ( !m_pMessageBox[i]->IsShow() )
		{
			return static_cast<SPWindowMessageBox*>(m_pMessageBox[i])->MessageBox(pszMsgText, iType, tMsgWnd, eMsg);
		}
	}
	return 0;
}

INSTANCE_ID SPInterfaceManager::MessageBox(int iMsgID, int iType, INSTANCE_ID tMsgWnd, SPIM eMsg)
{
	return MessageBox(g_pResourceManager->GetGlobalString(iMsgID), iType, tMsgWnd, eMsg);
}

/**
	PVP Result에서 HotKey Block를 위해서 생성
	CashShop같은 곳에서 사용 (SPWindowPVPResult에서 사용)
*/
void SPInterfaceManager::SetHotKeyBlock(bool bBlock)
{
	m_bBlockHotKey = bBlock ;
}

bool SPInterfaceManager::GetHotKeyBlock()
{
	return m_bBlockHotKey ;
}


/**
	특정 Window만 보여주고 모두 Hide 시킴
	>> WIID_CASHSHOP, WIID_STORY, WIID_TEXTBOOK, WIID_PVP_LOADWAIT, WIID_PVP_RESULT
	@param INSTANCE_ID wndID	: 해당 Window만 보여줌
	@param bool bIn				: 보여줄지 여부
 */
void SPInterfaceManager::SetOnlyWindowView(INSTANCE_ID wndID, bool bIn, bool bHideChat/*=true*/ )
{
	SetHotKeyBlock( bIn ) ;

	// Chat을 막는다.
	if( m_pWindowChat )
		m_pWindowChat->SetHideEnable(bHideChat);
	if( m_pWindowSystemMessage )
		m_pWindowSystemMessage->SetHideEnable(bHideChat);

	if( bIn )
	{
		m_pSubGameZone->Hide(false);
		m_pSubGameZone->Show(false);
		if( !bHideChat && m_pWindowChat )
			m_pWindowChat->Show() ;

		ShowWindow(wndID, true);
		SPWindow* pWindow = NULL ;
		pWindow = FindWindow(wndID) ;
		if( pWindow )
			SetFocusWindow(pWindow) ;
	}
	else
	{
		// PvpLoad Wait을 닫고 SubGame Show
		ShowWindow(WIID_PVP_LOADWAIT, false);

		// Show Zone Interfaces
		SetSubGame(SUBGAME_ZONE);
	}
}


void SPInterfaceManager::SetShowDefaultWindow( bool bShow )
{
	SPWindow*	pHideWindow	=	m_pSubGameZone;

	if( m_ppCurSubgame != NULL && (*m_ppCurSubgame) != NULL )
	{
		pHideWindow	=	*m_ppCurSubgame;
	}

	if( pHideWindow != NULL )
	{
		SetNoticeHide();

		pHideWindow->Hide( false );
		
		if( bShow == true )
		{
			(*m_ppCurSubgame)->Show( false) ;

			bool			bShowWindow		=	SP_SHOW;
			SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

			if( pChatManager != NULL )
				bShowWindow	=	!pChatManager->IsChatRoom();

			ShowWindow( WIID_MINIMAP			, bShowWindow );
			ShowWindow( WIID_STATPALLET			, bShowWindow );
			ShowWindow( WIID_CHAT				, bShowWindow );
			ShowWindow( WIID_QUICK_SLOT			, bShowWindow );
			ShowWindow( WIID_BUFF				, bShowWindow );
			ShowWindow( WIID_MINI_TEAM			, bShowWindow );
			ShowWindow( WIID_WORLDMAP_SHORTCUT	, bShowWindow );
			ShowWindow( WIID_QUESTNOTIFY		, bShowWindow );
			ShowWindow( WIID_SYSTEM_MESSAGE		, bShowWindow );

			ShowWindow( WIID_MENU				, SP_SHOW );

			// ShowWindow를 안쓰는 이유는 커서,포커스,액티브에서 무조건 하이드 하기 때문에 강제로 쇼를 합니다. [5/30/2008 AJJIYA]
			if( m_pWindowTalkingRoom != NULL )
				m_pWindowTalkingRoom->Show();	// TalkingRoom내부에서 보여지고 안보여지고 결정합니다. [4/28/2008 AJJIYA]
			ShowWindow( WIID_VISUAL_BUGLE		, bShowWindow );
			ShowWindow( WIID_FIND_MESSAGE		, bShowWindow );
			ShowWindow( WIID_FIND_MESSAGE_ONE   , bShowWindow );
			ShowWindow( WIID_FIND_MESSAGE_TWO	, bShowWindow );
			ShowWindow( WIID_GREENERGUIDE		, bShowWindow );
		}
	}
}

void SPInterfaceManager::InitClassTextureInfo()
{
	m_strClassTextureName[ CLASS_NULL ]	=	"DATA/INTERFACE/CONCEPT/UI102.PNG";
	SetRect( &(m_rcClassTextureSrc[ CLASS_NULL ]) , 0 , 0 , 0 , 0 );

	m_strClassTextureName[ CLASS_A ]	=	"DATA/INTERFACE/CONCEPT/UI102.PNG";
	m_strClassTextureName[ CLASS_B ]	=	"DATA/INTERFACE/CONCEPT/UI102.PNG";
	m_strClassTextureName[ CLASS_C ]	=	"DATA/INTERFACE/CONCEPT/UI102.PNG";
	m_strClassTextureName[ CLASS_D ]	=	"DATA/INTERFACE/CONCEPT/UI102.PNG";

	m_strClassTextureName[ CLASS_E ]	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	m_strClassTextureName[ CLASS_F ]	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	m_strClassTextureName[ CLASS_G ]	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	m_strClassTextureName[ CLASS_H ]	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	m_strClassTextureName[ CLASS_I ]	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	m_strClassTextureName[ CLASS_J ]	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	m_strClassTextureName[ CLASS_K ]	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	m_strClassTextureName[ CLASS_L ]	=	"DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";

	SetRect( &(m_rcClassTextureSrc[ CLASS_A ]) , 395 , 324 , 411 , 340 );
	SetRect( &(m_rcClassTextureSrc[ CLASS_B ]) , 412 , 324 , 428 , 340 );
	SetRect( &(m_rcClassTextureSrc[ CLASS_C ]) , 395 , 307 , 411 , 323 );
	SetRect( &(m_rcClassTextureSrc[ CLASS_D ]) , 412 , 307 , 428 , 323 );

	SetRect( &(m_rcClassTextureSrc[ CLASS_E ]) , 229 , 141 , 245 , 157 );
	SetRect( &(m_rcClassTextureSrc[ CLASS_F ]) , 246 , 141 , 262 , 157 );
	SetRect( &(m_rcClassTextureSrc[ CLASS_G ]) , 161 , 158 , 177 , 174 );
	SetRect( &(m_rcClassTextureSrc[ CLASS_H ]) , 178 , 158 , 194 , 174 );
	SetRect( &(m_rcClassTextureSrc[ CLASS_I ]) , 195 , 158 , 211 , 174 );
	SetRect( &(m_rcClassTextureSrc[ CLASS_J ]) , 212 , 158 , 228 , 174 );
	SetRect( &(m_rcClassTextureSrc[ CLASS_K ]) , 229 , 158 , 245 , 174 );
	SetRect( &(m_rcClassTextureSrc[ CLASS_L ]) , 246 , 158 , 262 , 174 );

	//----------------------------------------------------
	//{
	m_strClassTextureNameEx[ CLASS_NULL ]	=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG"; //- 메모리를 쓸데 없이 차지하고 있지만
	m_strClassTextureNameEx[ CLASS_A ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG"; // 수정 가능성 있기 때문에 그대로 남겨둠. W.Fairy
	m_strClassTextureNameEx[ CLASS_B ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";
	m_strClassTextureNameEx[ CLASS_C ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";
	m_strClassTextureNameEx[ CLASS_D ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";
	m_strClassTextureNameEx[ CLASS_E ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";
	m_strClassTextureNameEx[ CLASS_F ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";
	m_strClassTextureNameEx[ CLASS_G ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";
	m_strClassTextureNameEx[ CLASS_H ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";
	m_strClassTextureNameEx[ CLASS_I ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";
	m_strClassTextureNameEx[ CLASS_J ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";
	m_strClassTextureNameEx[ CLASS_K ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";
	m_strClassTextureNameEx[ CLASS_L ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";
	m_strClassTextureNameEx[ CLASS_M ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";
	m_strClassTextureNameEx[ CLASS_N ]		=	"DATA/INTERFACE/CONCEPT/UI_Object_002.PNG";

	SetRect( &(m_rcClassTextureSrcEx[ CLASS_NULL	]) , 0 , 0 , 0 , 0 );
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_A		]) , 237,373,259,395);
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_B		]) , 260,373,282,395);
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_C		]) , 283,373,305,395);
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_D		]) , 306,373,328,395);
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_E		]) , 329,373,351,395);
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_F		]) , 352,373,374,395);
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_G		]) , 260,396,282,418);  
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_H		]) , 237,396,259,418);  
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_I		]) , 283,396,305,418);  
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_J		]) , 306,396,328,418);  
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_K		]) , 352,396,374,418);
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_L		]) , 329,396,351,418);
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_M		]) , 409,205,431,227);
	SetRect( &(m_rcClassTextureSrcEx[ CLASS_N		]) , 432,205,454,227);
}

const char* SPInterfaceManager::GetClassTextureName( CLASS_TYPE eClassType )
{
	int	iClassType	=	(int)eClassType;

	if( iClassType < 0 || iClassType >= CLASS_MAX )
		return NULL;

	return m_strClassTextureName[ iClassType ].c_str();
}

const char* SPInterfaceManager::GetClassTextureNameEx( CLASS_TYPE eClassType )
{
	int	iClassType	=	(int)eClassType;

	if( iClassType < 0 || iClassType >= CLASS_MAX )
		return NULL;

	return m_strClassTextureNameEx[ iClassType ].c_str();
}

RECT* SPInterfaceManager::GetClassTextureSrc( CLASS_TYPE eClassType )
{
	int	iClassType	=	(int)eClassType;

	if( iClassType < 0 || iClassType >= CLASS_MAX )
		return NULL;

	return &( m_rcClassTextureSrc[ iClassType ] );
}

RECT* SPInterfaceManager::GetClassTextureSrcEx( CLASS_TYPE eClassType )
{
	int	iClassType	=	(int)eClassType;

	if( iClassType < 0 || iClassType >= CLASS_MAX )
		return NULL;

	return &( m_rcClassTextureSrcEx[ iClassType ] );
}

const char* SPInterfaceManager::GetCalcLumpStringChat()
{
	return (const char*)(m_pWindowChat->SPSendMessage(SPIM_GET_CALC_LUMP_STRING));
}


//////////////////////////////////////////////////////////////////////////

void SPInterfaceManager::SetCursorWindow(SPWindow* pWindow)
{
	m_pWindowCursorIn = pWindow;
}

void SPInterfaceManager::SetFocusWindow(SPWindow* pWindow)
{
	//SPWindow* pWindowPreFocus = m_pWindowFocusIn->GetWrapWindow();
	//SPWindow* pWindowPostFocus = pWindow->GetWrapWindow();	

	//if(pWindowPreFocus != pWindowPostFocus) {
	//	SetActiveWindow(pWindowPostFocus);
	//}

	if(m_pWindowFocusIn == pWindow) {
		return;
	}
	

	if(m_pWindowFocusIn)
		m_pWindowFocusIn->SPSendMessage( SPIM_KILLFOCUS );

	m_pWindowFocusIn = pWindow;

	if(m_pWindowFocusIn)
		m_pWindowFocusIn->SPSendMessage( SPIM_SETFOCUS );
}



void SPInterfaceManager::SetActiveWindow(SPWindow* pWindow)
{
	if( m_pWindowActive && m_pWindowActive != pWindow )
		m_pWindowActive->SPSendMessage( SPIM_DEACTIVATE );

	m_pWindowActive = pWindow;

	if( m_pWindowActive )
		m_pWindowActive->SPSendMessage( SPIM_ACTIVATE );	
}

void SPInterfaceManager::SetModalWindow(SPWindow* pWindow)
{
	if(pWindow == m_pWindowModal)
		return;

	if(m_pWindowModal)	{
		if( m_pWindowModal->IsShow() )
			m_pWindowModal->Hide();
	}

	m_pWindowModal = pWindow;
}

void SPInterfaceManager::SetMoveWindow( SPWindow* pWindow )
{
	if(m_pWindowMove)
		m_pWindowMove->SPSendMessage(m_pWindowMove, SPIM_MOVE_END, 0,0);

	m_pWindowMove = pWindow;

	if(m_pWindowMove)
		m_pWindowMove->SPSendMessage(m_pWindowMove, SPIM_MOVE_BEGIN, 0,0);
}


int SPInterfaceManager::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{	
	//if(!isActive()) return 0;
	
	switch(message)
	{
	case WM_LBUTTONDOWN:
		{
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			return PerformMouseLDown(fX, fY);
		}		
		break;

	case WM_RBUTTONDOWN:		
		{
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			return PerformMouseRDown(fX, fY);
		}
		break;

	case WM_LBUTTONUP:		
		{		
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			return PerformMouseLUp(fX, fY);
		}
		break;

	case WM_RBUTTONUP:
		{
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			return PerformMouseRUp(fX, fY);
		}		
		break;

	case WM_MOUSEMOVE:
		{
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			if(!g_pInputManager->GetJoyPadManager()->IsPadMouseMove()) {
				PerformMouseMove(fX, fY);				
			}
			return 0;
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			return PerformMouseLDBLClick(fX, fY);			
		}
		break;

	case WM_RBUTTONDBLCLK:
		{
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			return PerformMouseRDBLClick(fX, fY);			
		}
		break;

	case WM_MOUSEWHEEL:
		{
			short iDelta = HIWORD(wParam);
			if ( iDelta < 0 )
				PerformMouseWheelDown(abs(iDelta));				
			else
				PerformMouseWheelUp(abs(iDelta));				
		}
		break;

	case WM_KEYDOWN:
		{
			if(m_pWindowFocusIn)
				m_pWindowFocusIn->SPSendMessage(SPIM_KEY_DOWN, wParam, lParam);
		}
		break;

	case WM_KEYUP:
		{
			switch(wParam) {
			case VK_ESCAPE:
				return WndEscapeProc(wParam, lParam);				
			case VK_RETURN:
				return WndReturnProc(wParam, lParam);			
			}
		}
		break;


	}
	return 0;
}


int SPInterfaceManager::WndEscapeProc(WPARAM wParam, LPARAM lParam)
{
	if(g_pInputManager->GetJoyPadManager()->IsKeyPass())
		return 0;
	
	bool bRetVal;

	if( m_pWindowChat )
		if( m_pWindowChat->SPSendMessage( SPIM_KEY_ESCAPE ) == 1 )
			return 0;

	SPWindow* pWindowLogin = FindWindow( WIID_LOGIN );

	if( pWindowLogin != NULL )
		if( pWindowLogin->SPSendMessage( SPIM_KEY_ESCAPE ) == 1 )
			return 0;

	SPChildWndSendMessage( WIID_STATUS		, SPIM_COMBOBOX_LISTBOX_SHOW , (WPARAM)FALSE , NULL );

	if(m_bCutIn) {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

		if(g_pEventManager->GetEventHandler()->GetNpcID() == 0) {
			int iStage = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
			int iGroup = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();		
			std::string strBGM = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_strBGMFile;
			int iSoundEffect = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_iSoundEffectType;

			if(strBGM != ""){
				g_pSubGameManager->SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strBGM.c_str());
				SPSoundBase::GetInstance()->SetRoomType(iSoundEffect);
			}
		}
		if( g_pEventManager->GetCutInState() == CUT_IN_STATE_NPC_AUCTION )	{
			// 경매창에서는 ESC 키를 막는다 (진행중 나가는것을 막기위함)
			if( m_pWindowAuctionMSG->IsShow() )
				return 0 ;
		}

		if( g_pEventManager->GetCutInState() == CUT_IN_STATE_LOVE )	{
			// LOVE일때 ESC 키를 막는다.
				return 0 ;
		}

		if(g_pEventManager->GetCutInState() < CUT_IN_STATE_TALK || g_pEventManager->GetCutInState() == CUT_IN_STATE_SAVEPOINT || 
			g_pEventManager->GetCutInEvnetType() == EVENT_MAILBOX || g_pEventManager->GetCutInEvnetType() == EVENT_BEAUTY ||
			g_pEventManager->GetCutInEvnetType() == EVENT_CROPS || g_pEventManager->GetCutInEvnetType() == EVENT_CORE)
		{
			if(g_pEventManager->GetCutInEvnetType() != EVENT_CORE){
				g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);		//[2007/10/4] - 컷인 상태를 해제하도록 한다 <br>
			}			
		}
		else {
			//[2007/11/23] - PVP신청 대기중인 경우
			if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP)) {
				if(g_pEventManager->GetCutInState() == CUT_IN_STATE_PVP_RESERVE && 
					g_pkPvpManager->GetPvpStatus() == PVP_STATUS_ENTERWAIT)
				{			
					g_pkPvpManager->SendPVPEnterCancel();
					return 0;
				}
			}
			g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL_END);
			if(g_pEventManager->GetCutInEvnetType() < EVENT_MAILBOX) {
				SPWindow* pWindow = m_pSubGameZone->Find(WIID_NPCTALK, true);
				if( pWindow ) {
					if(!pWindow->IsShow()) {
						pWindow->Show();
					}
					else {
						//NPC와 큱E?상태인 경퓖E메뉴리스트로 간다
						g_pInterfaceManager->GetNpcTalkWindow()->SPSendMessage(SPIM_SET_NPCDATA, (WPARAM)true, (LPARAM)0);
					}
				}

				pWindow = m_pSubGameZone->Find(WIID_PCTALK, true);
				if(pWindow && pWindow->IsShow()) {
					pWindow->Hide();
				}
			}						
			else {
				//Error
			}
		}
		//g_pEventManager->SetCutInState(CUT_IN_STATE_NULL);		//[2005/8/26] - 컷인 상태를 해제하도록 한다 <br>
		return 0;			//[2005/5/21] 퓖E?ESC를 이거로 맊음
	}

	if(g_pGOBManager->GetLocalPlayer() == NULL) 
		return 0;	//트레이드시 키 막음// by metalgeni
	if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE) 
		return 0;	//트레이드시 키 막음				
	////[2007/5/21]- jinhee 기획팀 요청으로 막음
	//if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP) {		
	//	SPWindow* pWindow = FindWindow(WIID_USERSHOP_SELL);
	//	if(pWindow) {
	//		pWindow->SPSendMessage(SPIM_USERSHOP_SELL_SETCLOSE, (WPARAM)0, (LPARAM)0);
	//	}
	//	//return 0;		//내가 상점개설자		
	//}
	
	//if(g_pGOBManager->GetLocalPlayer()->GetUserShopID()) {
	//	return 0;							//내가 상점구매자
	//}

	if( m_bCashShop || m_bBlockHotKey )
		return 0;

	if(SPCashTradeManager::GetInstance()->GetCashTradeState() == CASH_TRADE_STATE_WAIT) //필드 캐시구매 응답 대기중
		return 0; 

	bRetVal = false;

	SPWindow* pWindow = FindWindow(WIID_MENU);
	if(pWindow) {
		//2인경우 성공적으로 윈도우를 닫음
		int iReturn = pWindow->SPSendMessage(SPIM_MENU_POPUP, (WPARAM)0, (LPARAM)0);
		(iReturn == 2)? bRetVal |= true : bRetVal |= false;
	}

	pWindow = FindWindow(WIID_USERSHOP_SELL);
	if(pWindow) {
		//2인경우 성공적으로 윈도우를 닫음
		int iReturn = pWindow->SPSendMessage(SPIM_USERSHOP_SELL_SETCLOSE, (WPARAM)0, (LPARAM)0);
		(iReturn == 2)? bRetVal |= true : bRetVal |= false;
	}

	pWindow = FindWindow(WIID_USERSHOP_BUY);
	if(pWindow) {
		int iReturn = pWindow->SPSendMessage(SPIM_USERSHOP_SELL_SETCLOSE, (WPARAM)0, (LPARAM)0);
		(iReturn == 2)? bRetVal |= true : bRetVal |= false;
	}

	bRetVal |= ShowWindow(WIID_ITEM		, 	SP_HIDE);	
	bRetVal |= ShowWindow(WIID_EQUIP	, 	SP_HIDE);
	bRetVal |= ShowWindow(WIID_SKILL	, 	SP_HIDE);
	bRetVal |= ShowWindow(WIID_STATUS	, 	SP_HIDE);
	bRetVal |= ShowWindow(WIID_QUEST	, 	SP_HIDE);
	bRetVal |= ShowWindow(WIID_NPC_QUEST, 	SP_HIDE);
	bRetVal |= ShowWindow(WIID_QUESTVIEW,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_TEAM		, 	SP_HIDE);
	//bRetVal |= ShowWindow(WIID_MENU		, 	SP_HIDE);
	bRetVal |= ShowWindow(WIID_PC_INFO	, 	SP_HIDE);
	bRetVal |= ShowWindow(WIID_CHANNEL_CHANGE		, 	SP_HIDE);
	bRetVal |= ShowWindow(WIID_FRIEND	, 	SP_HIDE);
	bRetVal |= ShowWindow(WIID_LOTTERY	, 	SP_HIDE);
	bRetVal |= ShowWindow(WIID_CHAT_MACRO	, 	SP_HIDE);
	bRetVal |= ShowWindow(WIID_OPTION	,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_HELP		,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_WORLDMAP	,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_WAYPOINT ,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_PET		,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_MICROPHONE,	SP_HIDE);
	//bRetVal |= ShowWindow( WIID_FIND_MESSAGE, SP_HIDE );//[liuyang 2008.8.29]
	//bRetVal |= ShowWindow( WIID_FIND_MESSAGE_ONE, SP_HIDE );//[liuyang 2008.9.1]
	//bRetVal |= ShowWindow( WIID_FIND_MESSAGE_TWO, SP_HIDE );//[liuyang 2008.9.1]
	//bRetVal |= ShowWindow( WIID_FRIEND_FIND, SP_HIDE);//[liuyang 2008.8.29]
	bRetVal |= ShowWindow(WIID_GUILD_INFO,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_ITEM_MIX	,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_GAMBLE	,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_GUIDE_BOOK,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_CHATTING_ROOM,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_CHATTING_PASSWORD,	SP_HIDE);	
	bRetVal |= ShowWindow(WIID_CASH_TRADE_SELECT,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_CASH_TRADE,			SP_HIDE);
	bRetVal |= ShowWindow(WIID_CASH_GIFT,			SP_HIDE);
	bRetVal |= ShowWindow(WIID_CASH_TRADE_RESULT,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_LOVE		,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_LOVEMSG_INPUT	,	SP_HIDE);
	bRetVal |= ShowWindow(WIID_LOVEMSG_VIEW		,	SP_HIDE);

	pWindow	=	FindWindow( WIID_TALKING_ROOM );
	if( pWindow != NULL )
		bRetVal	|=	pWindow->SPSendMessage( SPIM_KEY_ESCAPE , (WPARAM)bRetVal , NULL );

	if(bRetVal == false)
		ShowWindow( WIID_EXIT,	SP_NONE,	SP_TOGGLE_ON);

	return 1;

}



int SPInterfaceManager::WndReturnProc(WPARAM wParam, LPARAM lParam)
{	
	switch(m_SubGameID) {
	case SUBGAME_LOGGIN	:	
	case SUBGAME_LOBBY	:		
	case SUBGAME_LOADING:		
		{		
			if(m_pWindowActive) 
				m_pWindowActive->SPSendMessage(SPIM_KEY_RETURN);
		}
		break;
	case SUBGAME_ZONE:
		{
			if( m_pWindowModal ) {
				if( m_pWindowModal->SPSendMessage(SPIM_KEY_RETURN) == -1 ) {
					ToggleChat();
				}
			} else {
				if(m_pWindowActive) {
					if(m_pWindowActive->SPSendMessage(SPIM_KEY_RETURN) == -1 ) {
						ToggleChat();
					}
				} else {					
					ToggleChat();
				}
			}

		}
		break;
	}
	return 1;
}



int SPInterfaceManager::PerformMouseMove(int iX, int iY)
{
	m_iCursorX = iX;
	m_iCursorY = iY;
	m_MouseCursor.SetCurPos( (float)m_iCursorX, (float) m_iCursorY);

	SPWindow* pWindowPrevCursor = NULL;
	pWindowPrevCursor = m_pWindowCursorIn;

	int iRetVal = 0;

	// find current cursor Window
	SetCursorWindow( NULL );
	if ( m_ppCurSubgame && (*m_ppCurSubgame) && !m_pWindowModal ) {
		iRetVal |= (*m_ppCurSubgame)->PerformMouseMove( m_iCursorX, m_iCursorY );		
	}

	if( m_pWindowModal && m_pWindowModal->IsCursorIn(m_iCursorX, m_iCursorY) ) {		
		iRetVal |= m_pWindowModal->PerformMouseMove( m_iCursorX, m_iCursorY );		
	}


	// Focus Window - Slider, etc
	if ( m_pWindowFocusIn && m_pWindowFocusIn != m_pWindowCursorIn) {
		iRetVal |= (int)m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_CURSOR_MOVE, 0, MAKELONG(m_iCursorX, m_iCursorY) );		
	}

	if(m_pWindowMove)
		m_pWindowMove->SPSendMessage( m_pWindowMove, SPIM_MOVE, 0, MAKELONG(m_iCursorX, m_iCursorY) );

	// cursor out
	if ( pWindowPrevCursor && pWindowPrevCursor != m_pWindowCursorIn )	{
		iRetVal |= (int)pWindowPrevCursor->SPSendMessage( pWindowPrevCursor, SPIM_CURSOR_OUT, 0, MAKELONG(m_iCursorX, m_iCursorY) );
		pWindowPrevCursor->SPSendMessage( pWindowPrevCursor, SPIM_TOOLTIP_OUT, 0, MAKELONG(m_iCursorX, m_iCursorY) );
		GetToolTipWindow()->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , SPWindowToolTip::TOOLTIP_SHOW_TYPE_CTRL );
	}

	// cursor enter
	if ( m_pWindowCursorIn && pWindowPrevCursor != m_pWindowCursorIn )	{
		iRetVal |= (int)m_pWindowCursorIn->SPSendMessage( m_pWindowCursorIn, SPIM_CURSOR_ENTER, 0, MAKELONG(m_iCursorX, m_iCursorY) );

		if( m_pWindowCursorIn->IsShow() == true )
			GetToolTipWindow()->SPSendMessage( SPIM_TOOLTIP_ENTER , m_pWindowCursorIn->GetInstanceID() , SPWindowToolTip::TOOLTIP_SHOW_TYPE_CTRL );

		m_pWindowCursorIn->SPSendMessage( m_pWindowCursorIn, SPIM_TOOLTIP_ENTER, 0, MAKELONG(m_iCursorX, m_iCursorY) );
	}

	// cursor move
	if( m_pWindowCursorIn )	{
		iRetVal |= (int)m_pWindowCursorIn->SPSendMessage( m_pWindowCursorIn, SPIM_CURSOR_MOVE, 0, MAKELONG(m_iCursorX, m_iCursorY) );
		m_pWindowCursorIn->SPSendMessage( m_pWindowCursorIn, SPIM_TOOLTIP_MOVE, 0, MAKELONG(m_iCursorX, m_iCursorY) );

		if( GetToolTipWindow()->IsShow() == true )
			GetToolTipWindow()->SPSendMessage( SPIM_POS_UPDATE , NULL , MAKELONG(m_iCursorX, m_iCursorY) );
	}

	return iRetVal;
}

int SPInterfaceManager::PerformMouseLDBLClick( int iX, int iY)
{	
	// process for modal window should have priority.
	if( m_pWindowModal ) {
		m_pWindowModal->SPSendMessage( m_pWindowModal, SPIM_LBUTTON_DBLCLICK, 0, MAKELONG(m_iCursorX, m_iCursorY) );
		
		SetFocusWindow( m_pWindowCursorIn );
		if( m_pWindowFocusIn ) {
			m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_LBUTTON_DBLCLICK, 0, MAKELONG(m_iCursorX, m_iCursorY) );			
		}

		SetMoveWindow(m_pWindowFocusIn);

		if( m_pWindowCursorIn == NULL )
			m_pWindowFocusIn = m_pWindowModal;
		return true;
	}

	int iRet = 1;
	// find current cursor Window
	SetCursorWindow( NULL );	
	
	if ( m_ppCurSubgame && (*m_ppCurSubgame) )
		iRet = (*m_ppCurSubgame)->PerformMouseMove( m_iCursorX, m_iCursorY );

	// double clicked window become focus window.
	SetFocusWindow( m_pWindowCursorIn );

	// set activate window
	if( m_pWindowFocusIn ) SetActiveWindow( m_pWindowFocusIn->GetWrapWindow() );
	else SetActiveWindow( NULL );

	if( iRet ) {
		// send SPIM Message to focus window
		if( m_pWindowFocusIn ) m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_LBUTTON_DBLCLICK, 0, MAKELONG(m_iCursorX, m_iCursorY) );
		return true;	
	}

	return false;	
}

int SPInterfaceManager::PerformMouseRDBLClick( int iX, int iY)
{
	// process for modal window should have priority.
	if( m_pWindowModal ) {
		m_pWindowModal->SPSendMessage( m_pWindowModal, SPIM_RBUTTON_DBLCLICK, 0, MAKELONG(m_iCursorX, m_iCursorY) );

		SetFocusWindow( m_pWindowCursorIn );
		if( m_pWindowFocusIn ) {
			m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_RBUTTON_DBLCLICK, 0, MAKELONG(m_iCursorX, m_iCursorY) );			
		}

		SetMoveWindow(m_pWindowFocusIn);

		if( m_pWindowCursorIn == NULL )
			m_pWindowFocusIn = m_pWindowModal;
		return true;
	}

	int iRet = 1;
	// find current cursor Window
	SetCursorWindow( NULL );
	
	if ( m_ppCurSubgame && (*m_ppCurSubgame) )
		iRet = (*m_ppCurSubgame)->PerformMouseMove( m_iCursorX, m_iCursorY );

	// double clicked window become focus window.
	SetFocusWindow( m_pWindowCursorIn );

	// set activate window
	if( m_pWindowFocusIn ) SetActiveWindow( m_pWindowFocusIn->GetWrapWindow() );
	else SetActiveWindow( NULL );

	if( iRet ) {
		// send SPIM Message to focus window
		if( m_pWindowFocusIn ) m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_RBUTTON_DBLCLICK, 0, MAKELONG(m_iCursorX, m_iCursorY) );
		return true;
	}
	return false;
}

int SPInterfaceManager::PerformMouseLDown(int iX, int iY)
{
	// process for modal window should have priority.
	if( m_pWindowModal ) {
		m_pWindowModal->PerformMouseMove( m_iCursorX, m_iCursorY );
		SetFocusWindow( m_pWindowCursorIn );

		if( m_pWindowFocusIn ) {
			m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_LBUTTON_DOWN, 0, MAKELONG(m_iCursorX, m_iCursorY) );			
		}

		SetMoveWindow(m_pWindowFocusIn);

		if( m_pWindowCursorIn == NULL )
			m_pWindowFocusIn = m_pWindowModal;
		return true;
	}

	int iRet = 1;
	// find current cursor Window
	SetCursorWindow( NULL );	
	if ( m_ppCurSubgame && (*m_ppCurSubgame) )
		iRet = (*m_ppCurSubgame)->PerformMouseMove( m_iCursorX, m_iCursorY );

	// clicked window become focus window
	SetFocusWindow( m_pWindowCursorIn );

	// set activate window
	if( m_pWindowFocusIn ) SetActiveWindow( m_pWindowFocusIn->GetWrapWindow() );
	else SetActiveWindow( NULL );	

	if( iRet ) {
		// send SPIM Message to focus window
		if( m_pWindowFocusIn ) {
			m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_LBUTTON_DOWN, 0, MAKELONG(m_iCursorX, m_iCursorY) );
		}
		SetMoveWindow(m_pWindowFocusIn);
		return true;
	}

	////일반적인 바닥에 아이템을 내려 놓는경퓖E삭제
	////LButtonUp쪽으로 이동함...
	//if(m_SubGameID==SUBGAME_ZONE && g_pGOBManager->GetLocalPlayer()){
	//	if(m_pNoticeBox->IsShow())
	//		ForceReply();
	//}

	return false;
}

int SPInterfaceManager::PerformMouseLUp(int iX, int iY)
{
	SetMoveWindow(NULL);

	// process for modal window should have priority.
	if( m_pWindowModal ) {
		// find current cursor Window
		SetCursorWindow( NULL );
		m_pWindowModal->PerformMouseMove( m_iCursorX, m_iCursorY );

		SetFocusWindow( m_pWindowCursorIn );

		// send SPIM Message to focus window
		if( m_pWindowFocusIn ) {
			m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_LBUTTON_UP, 0, MAKELONG(m_iCursorX, m_iCursorY) );			
		}		

		SPDragnDropManager::GetInstance()->End();	//	드래그 앤 드턿E기능 추가 AJJIYA [7/1/2005]
		return true;
	}

	int iRet = 1;
		
	SetCursorWindow( NULL );	
	if ( m_ppCurSubgame && (*m_ppCurSubgame) )
		iRet = (*m_ppCurSubgame)->PerformMouseMove( m_iCursorX, m_iCursorY );


	//CHECK////////////////////////////////////////////////////////////////////////
	// clicked window become focus window
	SetFocusWindow( m_pWindowCursorIn );

	// set activate window
	if( m_pWindowFocusIn ) SetActiveWindow( m_pWindowFocusIn->GetWrapWindow() );
	else SetActiveWindow( NULL );
	//////////////////////////////////////////////////////////////////////////

	//일반적인 바닥에 아이템을 내려 놓는경퓖E삭제 //컷인모드에서 문제가 좀 있음...	
	if( m_pWindowCursorIn == NULL  && (m_SubGameID==SUBGAME_ZONE && g_pGOBManager->GetLocalPlayer())) {
		//[2005/10/10]	윈도퓖E영역외 클릭시 Notice box를 닫아버린다 
		if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_ITEM_PARTITION || 
			g_pCheckManager->GetNoticeUse() == NOTICE_USE_ITEM_TRASH) 
		{
			ForceReply();
		}
		
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_CS_TRASH );

		//if(g_pEventManager->GetCutInState() > CUT_IN_STATE_NULL) {
		//	g_pEventManager->GetEventHandler()->PerformMouseLUp(iX, iY);
		//}
	}	

	// send SPIM Message to focus window
	if( m_pWindowFocusIn ) {		
		m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_LBUTTON_UP, 0, MAKELONG(m_iCursorX, m_iCursorY) );
		if( IsItemEnableWindow(m_pWindowFocusIn) == false)
		{	
			if(g_pGOBManager->GetLocalPlayer())
				g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

			if( g_pSubGameManager && g_pSubGameManager->GetGameState() == GS_CASHSHOP )
			{
				if( m_pSubGameZone )
				{
					SPWindow* pkWindow = m_pSubGameZone->Find(WIID_CASHSHOP);
					if( pkWindow )
						pkWindow->SPSendMessage(SPIM_CASHSHOP_CLEAR_DRAG, 0, 0);
				}
			}
			else if( g_pSubGameManager && g_pSubGameManager->GetGameState() == GS_LOTTERY )
			{
				if( m_pSubGameZone )
				{
					SPWindow* pkWindow = m_pSubGameZone->Find(WIID_LOTTERY);
					if( pkWindow )
						pkWindow->SPSendMessage(SPIM_LOTTERY_CLEAR_DRAG, 0, 0);
				}
			}

			//[2005/10/10]	윈도퓖E영역외 클릭시 Notice box를 닫아버린다 
			if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_ITEM_PARTITION || 
				g_pCheckManager->GetNoticeUse() == NOTICE_USE_ITEM_TRASH) 
			{
				ForceReply();
			}
		}
		
		//[2006/1/2] - 인터페이스가 있는 경퓖ECutIn 동작을 제한하콅E위해...
		SPDragnDropManager::GetInstance()->End();	//	드래그 앤 드턿E기능 추가 AJJIYA [7/1/2005]
		return true;
	}	

	

	SPDragnDropManager::GetInstance()->End();	//	드래그 앤 드턿E기능 추가 AJJIYA [7/1/2005]
	return false;
}


bool SPInterfaceManager::IsItemEnableWindow(SPWindow *pWindow)
{
	if(pWindow == NULL)			return false;

	int iFlag = 0xFFFFF000;
	int iWndID = pWindow->GetInstanceID();
	switch(iWndID & iFlag){
		case WIID_ITEM:
		case WIID_EQUIP:
		case WIID_STORAGE:
		case WIID_PC_TRADE:
		case WIID_QUICK_SLOT:
		case WIID_NOTICEBOX:
		case WIID_NPCTRADE:
		case WIID_MAILLIST:	
		case WIID_USERSHOP_SELL:
		case WIID_ITEM_TIMEADD:
		case WIID_EFFECTITEM_UPGRADE:
			return true;
		default :
			return false;
	}
	
	return false;
}

BOOL SPInterfaceManager::IsShowLooting()
{
	if( m_pWindowLooting == NULL )
		return FALSE;

	return m_pWindowLooting->IsShow();
}

void SPInterfaceManager::PreDeleteWindow(SPWindow* pkWindow)
{
	if( m_pWindowCursorIn && m_pWindowCursorIn == pkWindow )
		m_pWindowCursorIn = NULL;
	if( m_pWindowFocusIn && m_pWindowFocusIn == pkWindow )
		m_pWindowFocusIn = NULL;
	if( m_pWindowActive && m_pWindowActive == pkWindow )
		m_pWindowActive = NULL;
	if( m_pWindowModal && m_pWindowModal == pkWindow )
		m_pWindowModal = NULL;
}

//////////////////////////////////////////////////////////////////////////
//
//	AJJIYA [10/25/2005]
//	메세지를 하위 차일탛E윈도퓖E?픸E貧?하는경퓖E사퓖E磯?
//	특별한 동작이 필요할경퓖E?기존의 SPPerformMessage를 사퓖E臼?처리하자.
//

int SPInterfaceManager::SPChildWndSendMessage( INSTANCE_ID tChildWnd , SPIM eMsg , WPARAM wParam, LPARAM lParam, bool bFindChild )
{
	SPWindow* pChildWindow	=	NULL;

	if ( m_ppCurSubgame && (*m_ppCurSubgame) )
		pChildWindow	=	(*m_ppCurSubgame)->Find(tChildWnd, bFindChild);

	if( pChildWindow == NULL )
		pChildWindow	=	m_pSubGameZone->Find(tChildWnd, bFindChild);

	if( pChildWindow == NULL )
		return 0;
    
	return pChildWindow->SPSendMessage( eMsg , wParam , lParam );
}

int SPInterfaceManager::PerformMouseRDown(int iX, int iY)
{
	// process for modal window should have priority.
	if( m_pWindowModal ) {
		m_pWindowModal->SPSendMessage( m_pWindowModal, SPIM_RBUTTON_DOWN, 0, MAKELONG(m_iCursorX, m_iCursorY) );
		m_pWindowFocusIn = m_pWindowModal;
		return true;
	}

	int iRet = 1;
	// find current cursor Window
	SetCursorWindow( NULL );

	//if ( m_pRoot != NULL )
	//	iRet = m_pRoot->PerformMouseMove( m_iCursorX, m_iCursorY );
	
	if ( m_ppCurSubgame && (*m_ppCurSubgame) )
		iRet = (*m_ppCurSubgame)->PerformMouseMove( m_iCursorX, m_iCursorY );

	// click 당한 윈도는 focus 윈도가 되컖E
	SetFocusWindow( m_pWindowCursorIn );

	// activate window 를 찾아서 설정한다.
	if( m_pWindowFocusIn ) SetActiveWindow( m_pWindowFocusIn->GetWrapWindow() );
	else SetActiveWindow( NULL );

	if( iRet ) {
		// send SPIM Message to focus window
		if( m_pWindowFocusIn ) m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_RBUTTON_DOWN, 0, MAKELONG(m_iCursorX, m_iCursorY) );
		return true;
	}
	return false;	
}

int SPInterfaceManager::PerformMouseRUp(int iX, int iY)
{
	// process for modal window should have priority.
	if( m_pWindowModal ) {
		m_pWindowModal->SPSendMessage( m_pWindowModal, SPIM_RBUTTON_UP, 0, MAKELONG(m_iCursorX, m_iCursorY) );
		if( m_pWindowFocusIn == m_pWindowModal ) {
			m_pWindowModal->SPSendMessage( m_pWindowModal, SPIM_RBUTTON_CLICK, 0, MAKELONG(m_iCursorX, m_iCursorY) );		
		}
		return true;
	}

	int iRet = 1;
	// find current cursor Window
	SetCursorWindow( NULL );

	if ( m_ppCurSubgame && (*m_ppCurSubgame) )
		iRet = (*m_ppCurSubgame)->PerformMouseMove( m_iCursorX, m_iCursorY );

	//CHECK////////////////////////////////////////////////////////////////////////
	// clicked window become focus window
	SetFocusWindow( m_pWindowCursorIn );
	// set activate window
	if( m_pWindowFocusIn ) SetActiveWindow( m_pWindowFocusIn->GetWrapWindow() );
	else SetActiveWindow( NULL );
	//////////////////////////////////////////////////////////////////////////

	// send SPIM Message to focus window
	if( m_pWindowFocusIn ) {
		m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_RBUTTON_UP, 0, MAKELONG(m_iCursorX, m_iCursorY) );
		if( m_pWindowFocusIn == m_pWindowCursorIn ) {
			m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_RBUTTON_CLICK, 0, MAKELONG(m_iCursorX, m_iCursorY) );
			return true;
		}
	}	
	return false;	
}

int SPInterfaceManager::PerformMouseWheelUp( int iDelta )
{
	// process for modal window should have priority.
	if( m_pWindowModal ) {
		m_pWindowModal->SPSendMessage( m_pWindowModal, SPIM_WHEEL_UP, 0, iDelta );

		SetFocusWindow( m_pWindowCursorIn );
		if( m_pWindowFocusIn ) {
			m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_WHEEL_UP, 0, iDelta );			
		}

		SetMoveWindow(m_pWindowFocusIn);

		if( m_pWindowCursorIn == NULL )
			m_pWindowFocusIn = m_pWindowModal;
		return true;
	}

	int iRet = 1;
	// find current cursor Window
	SetCursorWindow( NULL );
	
	if ( m_ppCurSubgame && (*m_ppCurSubgame) )
		iRet = (*m_ppCurSubgame)->PerformMouseMove( m_iCursorX, m_iCursorY );

	if( iRet ) {
		if( m_pWindowCursorIn )	{
			m_pWindowCursorIn->SPSendMessage( m_pWindowCursorIn, SPIM_WHEEL_UP, 0, iDelta );
		}
		return true;
	}
	return false;	

}

int SPInterfaceManager::PerformMouseWheelDown( int iDelta )
{
	// process for modal window should have priority.
	if( m_pWindowModal ) {
		m_pWindowModal->SPSendMessage( m_pWindowModal, SPIM_WHEEL_DOWN, 0, iDelta );

		SetFocusWindow( m_pWindowCursorIn );
		if( m_pWindowFocusIn ) {
			m_pWindowFocusIn->SPSendMessage( m_pWindowFocusIn, SPIM_WHEEL_DOWN, 0, iDelta );			
		}

		SetMoveWindow(m_pWindowFocusIn);

		if( m_pWindowCursorIn == NULL )
			m_pWindowFocusIn = m_pWindowModal;
		return true;
	}

	int iRet = 1;
	// find current cursor Window
	SetCursorWindow( NULL );
	
	if ( m_ppCurSubgame && (*m_ppCurSubgame) )
		iRet = (*m_ppCurSubgame)->PerformMouseMove( m_iCursorX, m_iCursorY );

	if( iRet ) {
		// send SPIM Message to focus window
		if( m_pWindowCursorIn ) {
			m_pWindowCursorIn->SPSendMessage( m_pWindowCursorIn, SPIM_WHEEL_DOWN, 0, iDelta );
		}
		return true;
	}
	return false;	

}

int SPInterfaceManager::SPPerformMessage	(UINT msg, WPARAM wparam, LPARAM lParam)
{
	return SPIMESSAGE_FUNC((SPIM)msg, wparam, lParam);	
}


SPWindow*	SPInterfaceManager::FindWindow(INSTANCE_ID InstanceID)
{
	if ( m_ppCurSubgame && (*m_ppCurSubgame) )	
		return (*m_ppCurSubgame)->Find( InstanceID , false);

	return NULL;
}

SPIMESSAGE_MAP_BEGIN( SPInterfaceManager )
	SPIMESSAGE_COMMAND_EX( SPIM_WNDCHAT_GET_BUGLE		,	OnChatGetBugle		)
	SPIMESSAGE_COMMAND_EX( SPIM_WNDCHAT_GET_VISUALBUGLE	,	OnChatGetVisualBugle)
	SPIMESSAGE_COMMAND_EX( SPIM_WNDCHAT_GET_MSG			,	OnChatGetMsg		)
	SPIMESSAGE_COMMAND_EX( SPIM_WNDCHAT_MESSAGE_STRING	,	OnChatMessage		)
	SPIMESSAGE_COMMAND_EX( SPIM_WNDCHAT_CLEAR			,	OnChatClear			)
	SPIMESSAGE_COMMAND_EX( SPIM_WNDCHAT_INSERT_MSGEDIT	,	OnChatInsertMsgEdit	)

	SPIMESSAGE_COMMAND_EX( SPIM_REFRESH_PLAYER_STAT		,	OnRefreshPlayerStat )
	SPIMESSAGE_COMMAND_EX( SPIM_TEAM_ICON_UPDATE		,	OnTeamIconUpdate )

	SPIMESSAGE_COMMAND_EX( SPIM_SET_STAGE				,	OnSetStage )
//	SPIMESSAGE_COMMAND_EX( SPIM_REFRESH_MINIMAP_GOB		,	OnRefreshMiniMapGOB )

	SPIMESSAGE_COMMAND_EX( SPIM_SET_MESSAGE_SHOW		,	OnMessageShow)
	SPIMESSAGE_COMMAND_EX( SPIM_SET_MESSAGE_TEXT		,	OnMessageTextSet)

	SPIMESSAGE_COMMAND_EX( SPIM_LOOTING_ITEM_SET		,	OnLootingItemSet)
	SPIMESSAGE_COMMAND_EX( SPIM_LOOTING_ITEM_DELETE		,	OnLootingItemDelete)
	SPIMESSAGE_COMMAND_EX( SPIM_LOOTING_ITEM_REDRAW		,	OnLootingItemReDraw)

	SPIMESSAGE_COMMAND_EX( SPIM_SET_NPCSHOP_SHOW		,	OnShowNpcShopWindow)
	SPIMESSAGE_COMMAND_EX( SPIM_SET_CUTIN_WINDOW		,	OnCutInInterfaceShow)

	SPIMESSAGE_COMMAND_EX( SPIM_ITEM_SELECT				,	OnItemSellSelect)	
	
	SPIMESSAGE_COMMAND_EX( SPIM_SET_EXIT_SHOW			,	OnExitButtonShow)
	SPIMESSAGE_COMMAND_EX( SPIM_CHANNEL_SET				,	OnChannelListSet)

	SPIMESSAGE_COMMAND_EX( SPIM_SET_PLAYER_INFO			,	OnSetPlayerInfo)

	//SPIMESSAGE_COMMAND_EX( SPIM_GET_QUICK_SLOT			,	OnGetQuickSlot		)	// AJJIYA [7/14/2005]
	//SPIMESSAGE_COMMAND_EX( SPIM_QUICK_SLOT_UPDATE		,	OnQuickSlotUpdate	)	// AJJIYA [7/21/2005]

	//SPIMESSAGE_COMMAND_EX( SPIM_GET_SKILL_LVUP			,	OnGetSkillLVUP		)	// AJJIYA [7/14/2005]
	//SPIMESSAGE_COMMAND_EX( SPIM_SKILL_UPDATE			,	OnSkillUpdate		)	// AJJIYA [7/14/2005]

	SPIMESSAGE_COMMAND_EX( SPIM_PC_INFO_SET_STATUS		,	OnSetPCInfoStatus	)

//	SPIMESSAGE_COMMAND_EX( SPIM_GET_EMOTICON			,	OnGetEmoticon		)	// AJJIYA [8/30/2005]
//	SPIMESSAGE_COMMAND_EX( SPIM_UPDATE_EMOTICON			,	OnUpdateEmoticon	)	// AJJIYA [8/30/2005]

	//SPIMESSAGE_COMMAND_EX( SPIM_GET_FRIEND				,	OnGetFriend			)	// AJJIYA [9/2/2005]
	//SPIMESSAGE_COMMAND_EX( SPIM_ADD_FRIEND				,	OnAddFriend			)	// AJJIYA [9/2/2005]
	//SPIMESSAGE_COMMAND_EX( SPIM_DEL_FRIEND				,	OnDelFriend			)	// AJJIYA [9/2/2005]
	//SPIMESSAGE_COMMAND_EX( SPIM_STATUS_FRIEND			,	OnUpdateStatusFriend			)	// HOSHIM	[9/6/2005]

	//SPIMESSAGE_COMMAND_EX( SPIM_REFRESH_QUEST			,	OnRefreshQuest		)	// AJJIYA [10/6/2005]

	SPIMESSAGE_COMMAND_EX( SPIM_TEAM_JOIN				,	OnTeamJoin	)
	SPIMESSAGE_COMMAND_EX( SPIM_TEAM_SECEDE				,	OnTeamSecede	)
	SPIMESSAGE_COMMAND_EX( SPIM_TEAM_UPDATE				,	OnTeamUpdate	)
	SPIMESSAGE_COMMAND_EX( SPIM_TEAM_UPDATE_STATUS		,	OnTeamUpdateStatus	)
	SPIMESSAGE_COMMAND_EX( SPIM_TEAM_UPDATE_LOOTING_RULE,	OnTeamUpdateLootingRule	)
	
	SPIMESSAGE_COMMAND_EX( SPIM_ITEM_ID_INFO,				OnItemIDInfo	)
	
	SPIMESSAGE_COMMAND_EX( SPIM_ERROR_SET_MSG,				OnSetErrorMsg	)

SPIMESSAGE_MAP_END_BASE()


int SPInterfaceManager::OnChatGetMsg( WPARAM wParam, LPARAM lParam )
{
	if( m_pWindowChat != NULL )
	{
		m_pWindowChat->SPSendMessage( SPIM_WNDCHAT_GET_MSG , wParam , lParam );
	}
	return 0;
}

int SPInterfaceManager::OnChatGetBugle( WPARAM wParam, LPARAM lParam )
{
	if (m_pWindowChat != NULL)
	{
		m_pWindowChat->SPSendMessage( SPIM_WNDCHAT_GET_BUGLE, wParam, lParam );
	}
	return 0;
}

int SPInterfaceManager::OnChatGetVisualBugle( WPARAM wParam, LPARAM lParam )
{
	if (m_pWindowChat != NULL)
	{
		m_pWindowChat->SPSendMessage( SPIM_WNDCHAT_GET_VISUALBUGLE, wParam, lParam );
	}
	return 0;
}

int SPInterfaceManager::OnChatMessage(WPARAM wParam, LPARAM lParam )
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pWindow = (*m_ppCurSubgame)->Find(WIID_CHAT, true);
	if(pWindow) {
		return pWindow->SPSendMessage(SPIM_WNDCHAT_MESSAGE_STRING, wParam, lParam);
	}
	return 0;
}

int SPInterfaceManager::OnChatClear(WPARAM wparam, LPARAM lparam)
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pWindow = (*m_ppCurSubgame)->Find(WIID_CHAT, true);
	if(pWindow) {
		return pWindow->SPSendMessage(SPIM_PURGE, wparam, lparam);
	}
	return 0;
}

int SPInterfaceManager::OnChatInsertMsgEdit(WPARAM wparam, LPARAM lparam)
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pWindow;

	if( m_pWindowChat != NULL )
		pWindow	=	m_pWindowChat;
	else
		pWindow = (*m_ppCurSubgame)->Find(WIID_CHAT, true);

	if( pWindow != NULL )
		return	pWindow->SPSendMessage( SPIM_WNDCHAT_INSERT_MSGEDIT , wparam , lparam );

	return 0;
}

//////////////////////////////////////////////////////////////////////////
int SPInterfaceManager::OnRefreshPlayerStat(WPARAM wParam, LPARAM lParam)
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pWindow = (*m_ppCurSubgame)->Find(WIID_STATPALLET, true);	
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_REFRESH, wParam, lParam);
	}

	pWindow = (*m_ppCurSubgame)->Find(WIID_STATUS, true);	
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_REFRESH, wParam, lParam);
	}

	// AJJIYA [7/22/2005]
	pWindow = (*m_ppCurSubgame)->Find(WIID_SKILL, true);
	if(pWindow)
		pWindow->SPSendMessage(SPIM_REFRESH, wParam, lParam);

	// AJJIYA [7/26/2005]
	pWindow = (*m_ppCurSubgame)->Find(WIID_QUICK_SLOT, true);
	if(pWindow)
		pWindow->SPSendMessage(SPIM_REFRESH, wParam, lParam);

	// Love [11/7/2008 AJJIYA]
	pWindow = (*m_ppCurSubgame)->Find(WIID_LOVE, true);
	if(pWindow)
		pWindow->SPSendMessage(SPIM_REFRESH, wParam, lParam);

	return 0;
}

int SPInterfaceManager::OnTeamIconUpdate(WPARAM wParam, LPARAM lParam)
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pWindow = (*m_ppCurSubgame)->Find(WIID_STATPALLET, true);	
	if(pWindow)
		pWindow->SPSendMessage(SPIM_TEAM_ICON_UPDATE, wParam, lParam);

	return 0;
}


int SPInterfaceManager::OnSetStage(WPARAM wParam, LPARAM lParam) 
{
	if( m_pSubGameZone == NULL )
		return 0;

	//SPWindow* pWindow = (*m_pSubGameZone)->Find(WIID_MINIMAP, true);
	SPWindow* pWindow = m_pSubGameZone->Find(WIID_MINIMAP, true);
	if(pWindow) {
		return pWindow->SPSendMessage(SPIM_SET_STAGE, wParam, lParam);
	}
	return 0;
}

int SPInterfaceManager::OnMessageShow(WPARAM wParam, LPARAM lParam)
{	
	if( m_pSubGameZone == NULL )
		return 0;

	SPWindow* pWindow = m_pSubGameZone->Find(WIID_MESSAGE, true);
	if(pWindow) {
		//return pWindow->SPSendMessage(SPIM_SET_MESSAGE_SHOW, wParam, lParam);
		wParam ? pWindow->Show() : pWindow->Hide();
		return 0;
	}
	return 0;
}


int SPInterfaceManager::OnMessageTextSet(WPARAM wParam, LPARAM lParam)
{
	if( m_pSubGameZone == NULL )
		return 0;

	SPWindow* pWindow = m_pSubGameZone->Find(WIID_MESSAGE, true);
	if(pWindow) {
		return pWindow->SPSendMessage(SPIM_SET_MESSAGE_TEXT, wParam, lParam);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
int SPInterfaceManager::OnLootingItemSet(WPARAM wParam, LPARAM lParam)
{
	if( m_pSubGameZone == NULL )
		return 0;

	SPWindow* pWindow = m_pSubGameZone->Find(WIID_LOOTING, true);
	if(pWindow) {
		return pWindow->SPSendMessage(SPIM_LOOTING_ITEM_SET, wParam, lParam);
	}
	return 0;
}

int SPInterfaceManager::OnLootingItemDelete(WPARAM wParam, LPARAM lParam)
{
	if( m_pSubGameZone == NULL )
		return 0;

	SPWindow* pWindow = m_pSubGameZone->Find(WIID_LOOTING, true);
	if(pWindow) {
		return pWindow->SPSendMessage(SPIM_LOOTING_ITEM_DELETE, wParam, lParam);
	}
	return 0;
}

int SPInterfaceManager::OnLootingItemReDraw(WPARAM wParam, LPARAM lParam)
{
	if( m_pSubGameZone == NULL )
		return 0;

	SPWindow* pWindow = m_pSubGameZone->Find(WIID_LOOTING, true);
	if( pWindow && pWindow->IsShow() ) {
		return pWindow->SPSendMessage(SPIM_LOOTING_ITEM_REDRAW, wParam, lParam);
	}
	return 0;
}



//////////////////////////////////////////////////////////////////////////
int SPInterfaceManager::OnShowNpcShopWindow(WPARAM wParam, LPARAM lParam)
{
	if( m_pSubGameZone == NULL )
		return 0;

	int iCutInType = (CUT_IN_STATE)wParam;

	SPWindow* pWindow = m_pSubGameZone->Find(WIID_SHOPTALK, true);
	if( pWindow ) {
		if(!pWindow->IsShow())
			pWindow->Show();
	}
	
	if(iCutInType == CUT_IN_STATE_TRADE) {
		pWindow = m_pSubGameZone->Find(WIID_NPCTRADE, true);
		if( pWindow ) {
			if(!pWindow->IsShow()){
				pWindow->Show();
				return pWindow->SPSendMessage(SPIM_SET_NPCSHOP_SHOW, wParam, lParam);
			}
		}
	}	

	return 0;
}


//////////////////////////////////////////////////////////////////////////
//void SPInterfaceManager::SetCutInInterfaceShow(bool bShow /*= false*/)
int SPInterfaceManager::OnCutInInterfaceShow(WPARAM wParam, LPARAM lParam)
{
	// 채팅 인던에서는 화면에 UI를 표시하지 않는다 전부 하이드 [5/21/2008 AJJIYA]
	bool			bShow			=	SP_SHOW;
	SPChatManager*	pChatManager	=	SPChatManager::GetInstance();

	if( pChatManager != NULL )
		bShow	=	!pChatManager->IsChatRoom();
	//////////////////////////////////////////////////////////////////////////

	//
	SPWindow* pWindow = NULL;
	SPWindowSystemMessage*	pWindowSystemMessage	=	(SPWindowSystemMessage*)m_pWindowSystemMessage;
	switch(g_pEventManager->GetCutInState()){		
		case CUT_IN_STATE_NULL:
		case CUT_IN_STATE_RELEASE:
			//[2006/6/28] - 컷인 종료시 이동가능 윈도퓖E?변컖E		
			m_pWindowInven->SetMoveable(true);			
			m_pWindowEquip->SetMoveable(true);
			m_pWindowStatus->SetMoveable(true);
			m_pWindowSystemMessage->SetMoveable(true);

			if( pWindowSystemMessage != NULL )
				pWindowSystemMessage->SetMinimized( m_bMinimizedSystemMessage );
			
			ShowWindow( WIID_MINIMAP			, bShow );
			ShowWindow( WIID_STATPALLET			, bShow );
			ShowWindow( WIID_CHAT				, bShow );
			ShowWindow( WIID_QUICK_SLOT			, bShow );
			ShowWindow( WIID_BUFF				, bShow );
			ShowWindow( WIID_MINI_TEAM			, bShow );
			ShowWindow( WIID_WORLDMAP_SHORTCUT	, bShow );
			ShowWindow( WIID_QUESTNOTIFY		, bShow );
			ShowWindow( WIID_SYSTEM_MESSAGE		, bShow );
			ShowWindow( WIID_VISUAL_BUGLE		, bShow );
			/*ShowWindow( WIID_FIND_MESSAGE		, bShow );
			ShowWindow( WIID_FIND_MESSAGE_ONE   , bShow );
			ShowWindow( WIID_FIND_MESSAGE_TWO	, bShow );*/

			m_pWindowInven->AdjustCoord(m_ptInven.x, m_ptInven.y);			
			m_pWindowInven->SPSendMessage(SPIM_ITEM_SUBMENU, 0, (LPARAM)true) ;		//컷인취소시 활성화 
			m_pWindowEquip->AdjustCoord(m_ptEquip.x, m_ptEquip.y);			
			m_pWindowSystemMessage->AdjustCoord( m_ptOldSystemMessage.x, m_ptOldSystemMessage.y );
			
			ShowWindow( WIID_ITEM,		SP_HIDE );
			OnExitButtonShow((WPARAM)WIID_ITEM, (LPARAM)true);
			ShowWindow( WIID_EQUIP,		SP_HIDE );
			OnExitButtonShow((WPARAM)WIID_EQUIP, (LPARAM)true);
			ShowWindow( WIID_SKILL,		SP_HIDE );			
			ShowWindow( WIID_STATUS,	SP_HIDE );			
			//ShowWindow( WIID_MENU,		SP_HIDE );
			ShowWindow( WIID_MENU,		SP_SHOW);
			ShowWindow( WIID_MESSAGE,	SP_HIDE );
			ShowWindow( WIID_LOOTING,	SP_HIDE );
			//m_pWindowStatus->AdjustCoord(m_ptStatusPos.x, m_ptStatusPos.y, m_ptStatusSize.x, m_ptStatusSize.y);
			ShowWindow( WIID_NPCTALK,	SP_HIDE );
			ShowWindow( WIID_SHOPTALK,	SP_HIDE );
			ShowWindow( WIID_NPCTRADE,	SP_HIDE );
			//ShowWindow( WIID_ELY,		SP_HIDE );			
			//ShowWindow( WIID_ITEMSELL,	SP_HIDE );
			ShowWindow( WIID_STORAGE,	SP_HIDE );
			ShowWindow( WIID_GIFT,		SP_HIDE );
			ShowWindow( WIID_PC_INFO,	SP_HIDE );
			ShowWindow( WIID_PC_TRADE,	SP_HIDE );
			ShowWindow( WIID_CHANNEL_CHANGE,	SP_HIDE );
			ShowWindow( WIID_FRIEND,	SP_HIDE );
			ShowWindow( WIID_LOTTERY,	SP_HIDE );
			ShowWindow( WIID_CHAT_MACRO,	SP_HIDE );
			ShowWindow( WIID_NPC_QUEST, SP_HIDE );
			ShowWindow( WIID_QUEST,		SP_HIDE );
			ShowWindow( WIID_NPC_QUEST,	SP_HIDE );
			ShowWindow( WIID_QUESTVIEW,	SP_HIDE );
			ShowWindow( WIID_OPTION,	SP_HIDE);
			ShowWindow( WIID_HELP,		SP_HIDE );
			//ShowWindow( WIID_SHORTCUT	, SP_SHOW);
			ShowWindow( WIID_MAILLIST	, SP_HIDE);
			ShowWindow( WIID_MAILMSG	, SP_HIDE);
			ShowWindow( WIID_USERSHOP_SELL,	SP_HIDE);
			ShowWindow( WIID_USERSHOP_BUY,	SP_HIDE);
			ShowWindow( WIID_WAYPOINT	, SP_HIDE);
			ShowWindow( WIID_BEAUTYSHOP,	SP_HIDE);
			ShowWindow( WIID_PET	, SP_HIDE);	
			ShowWindow( WIID_MICROPHONE, SP_HIDE);
			//ShowWindow( WIID_FIND_MESSAGE, SP_HIDE );//[liuyang, 2008.8.31]
			//ShowWindow( WIID_FIND_MESSAGE_ONE, SP_HIDE );//[liuyang 2008.9.1]
			//ShowWindow( WIID_FIND_MESSAGE_TWO, SP_HIDE );//[liuyang 2008.9.1]
			ShowWindow( WIID_FRIEND_FIND, SP_HIDE );//[liuyang, 2008.8.29]
			ShowWindow( WIID_FRIEND_FOLLOW, SP_HIDE );//[liuyang, 2008.10.10]
			ShowWindow( WIID_FRIEND_SNATCH, SP_HIDE );//[liuyang, 2008.10.11]
			ShowWindow(WIID_GUILD_CREATE, SP_HIDE);
			ShowWindow(WIID_GUILD_EMBLEM, SP_HIDE);
			ShowWindow(WIID_GUILD_DESTROY, SP_HIDE);
			ShowWindow(WIID_GUILD_LEVELUP, SP_HIDE);
			ShowWindow(WIID_GUILD_INFO,	SP_HIDE);
			ShowWindow(WIID_CROPS,		SP_HIDE);
			ShowWindow(WIID_JOYSTICK,	SP_HIDE);
			ShowWindow(WIID_NPC_PORTAL,	SP_HIDE);
			ShowWindow(WIID_NPC_EVENTLIST, SP_HIDE);
			ShowWindow(WIID_NPC_EVENT,	SP_HIDE);
			ShowWindow(WIID_ITEM_MIX,	SP_HIDE);
			ShowWindow(WIID_MESSAGE,	SP_HIDE);
			ShowWindow(WIID_GAMBLE,		SP_HIDE);
			ShowWindow(WIID_PVPRESERVE,	SP_HIDE);
			ShowWindow(WIID_PVPENTERWAIT, SP_HIDE);
			ShowWindow(WIID_GUIDE_BOOK,	SP_HIDE);
			ShowWindow(WIID_CHATTING_ROOM,	SP_HIDE);
			ShowWindow(WIID_TALKING_ROOM,	SP_HIDE);
			ShowWindow(WIID_CHATTING_PASSWORD,	SP_HIDE);
			ShowWindow(WIID_CASH_TRADE_SELECT,	SP_HIDE);
			ShowWindow(WIID_CASH_TRADE,	SP_HIDE);
			ShowWindow(WIID_CASH_GIFT,	SP_HIDE);
			ShowWindow(WIID_AUCTION,	SP_HIDE);
			ShowWindow(WIID_AUCTION_MSG,SP_HIDE);
			ShowWindow(WIID_LOVE,		SP_HIDE);
			ShowWindow(WIID_LOVEMSG_INPUT,	SP_HIDE);
			ShowWindow(WIID_LOVEMSG_VIEW,	SP_HIDE);
			ShowWindow(WIID_OPTIONRST,	SP_HIDE );
			ShowWindow(WIID_RANKING,			SP_HIDE);
			
			//	AJJIYA [10/12/2005]
			//	컷인이 종료될때 툴팁 표시도 없애준다.
			GetToolTipWindow()->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, -2 );
			break;
		case CUT_IN_STATE_SCROLL_END:
			ShowWindow(WIID_SHOPTALK,			SP_HIDE);
			ShowWindow(WIID_ITEM,				SP_HIDE);
			ShowWindow(WIID_EQUIP,				SP_HIDE);
			ShowWindow(WIID_NPCTRADE,			SP_HIDE);
			ShowWindow(WIID_STORAGE,			SP_HIDE);
			ShowWindow(WIID_GIFT,				SP_HIDE);
			ShowWindow(WIID_QUEST,				SP_HIDE);
			ShowWindow(WIID_NPC_QUEST,			SP_HIDE);
			ShowWindow(WIID_QUESTVIEW,			SP_HIDE);
			ShowWindow(WIID_QUESTNOTIFY,		SP_HIDE);
			ShowWindow(WIID_BEAUTYSHOP,			SP_HIDE);
			ShowWindow(WIID_NPC_QUEST,			SP_HIDE);
			ShowWindow(WIID_PC_INFO,			SP_HIDE);
			ShowWindow(WIID_MAILLIST,			SP_HIDE);
			ShowWindow(WIID_MAILMSG,			SP_HIDE);
			ShowWindow(WIID_SYSTEM_MESSAGE,		SP_HIDE);
			ShowWindow(WIID_GUILD_CREATE,		SP_HIDE);
			ShowWindow(WIID_GUILD_DESTROY,		SP_HIDE);
			ShowWindow(WIID_CROPS,				SP_HIDE);
			ShowWindow(WIID_NPC_PORTAL,			SP_HIDE);
			ShowWindow(WIID_NPC_EVENTLIST,		SP_HIDE);
			ShowWindow(WIID_NPC_EVENT,			SP_HIDE);
			ShowWindow(WIID_GUILD_LEVELUP,		SP_HIDE);
			ShowWindow(WIID_GUILD_EMBLEM,		SP_HIDE);
			ShowWindow(WIID_PCTALK,				SP_HIDE);
			ShowWindow(WIID_PVPRESERVE,			SP_HIDE);
			ShowWindow(WIID_PVPENTERWAIT,		SP_HIDE);
			ShowWindow(WIID_CASH_TRADE_SELECT,	SP_HIDE);
			ShowWindow(WIID_CASH_TRADE,	SP_HIDE);
			ShowWindow(WIID_CASH_GIFT,	SP_HIDE);
			ShowWindow(WIID_AUCTION,	SP_HIDE);
			ShowWindow(WIID_AUCTION_MSG,SP_HIDE);
			ShowWindow(WIID_OPTIONRST,			SP_HIDE );
			ShowWindow(WIID_RANKING,			SP_HIDE);
			break;		
		case CUT_IN_STATE_SCROLL:
			m_pWindowInven->GetAbsCoord((int&)m_ptInven.x, (int&)m_ptInven.y);
			m_pWindowEquip->GetAbsCoord((int&)m_ptEquip.x, (int&)m_ptEquip.y);			
			m_pWindowSystemMessage->GetAbsCoord( (int&)m_ptOldSystemMessage.x , (int&)m_ptOldSystemMessage.y );
			m_pWindowSystemMessage->AdjustCoord( m_ptCutInSystemMessage.x , m_ptCutInSystemMessage.y );
			m_pWindowSystemMessage->SetMoveable( false );

			if( pWindowSystemMessage != NULL )
			{
				m_bMinimizedSystemMessage	=	pWindowSystemMessage->GetMinimized();
				pWindowSystemMessage->SetMinimized( false );
			}

			//[2007/1/10]
			pWindow = FindWindow(WIID_MENU);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_MENU_POPUP, (WPARAM)0, (LPARAM)0);
			}
			
			ShowWindow( WIID_STATPALLET, SP_HIDE);
			ShowWindow( WIID_ITEM,		SP_HIDE );
			OnExitButtonShow((WPARAM)WIID_ITEM, (LPARAM)true);
			ShowWindow( WIID_EQUIP,		SP_HIDE );
			OnExitButtonShow((WPARAM)WIID_EQUIP, (LPARAM)true);
			ShowWindow( WIID_SKILL,		SP_HIDE );
			//m_pWindowStatus->AdjustCoord(m_ptStatusPos.x, m_ptStatusPos.y, m_ptStatusSize.x, m_ptStatusSize.y);
			ShowWindow( WIID_STATUS,	SP_HIDE );
			ShowWindow( WIID_MENU,		SP_HIDE );
			ShowWindow( WIID_MESSAGE,	SP_HIDE );
			ShowWindow( WIID_LOOTING,	SP_HIDE );
			ShowWindow( WIID_SHOPTALK,	SP_HIDE );
			ShowWindow( WIID_NPCTRADE,	SP_HIDE );
			//ShowWindow( WIID_ELY,		SP_HIDE );			
			//ShowWindow( WIID_ITEMSELL,	SP_HIDE );
			ShowWindow( WIID_STORAGE,	SP_HIDE );
			ShowWindow( WIID_GIFT,		SP_HIDE );
			ShowWindow( WIID_QUEST,		SP_HIDE );
			ShowWindow( WIID_NPC_QUEST,	SP_HIDE );
			ShowWindow( WIID_QUESTVIEW,	SP_HIDE );
			ShowWindow( WIID_QUESTNOTIFY,	SP_HIDE );
			ShowWindow( WIID_OPTION,	SP_HIDE);
			ShowWindow( WIID_NPC_QUEST, SP_HIDE );
			ShowWindow( WIID_TEAM	,	SP_HIDE );
			ShowWindow( WIID_FRIEND,	SP_HIDE );
			ShowWindow( WIID_LOTTERY,	SP_HIDE );
			ShowWindow( WIID_CHAT_MACRO,	SP_HIDE );
			ShowWindow( WIID_PC_INFO,	SP_HIDE );
			ShowWindow( WIID_PC_TRADE,	SP_HIDE );
			ShowWindow( WIID_CHANNEL_CHANGE,	SP_HIDE );
			ShowWindow( WIID_MINIMAP,	SP_HIDE );
			ShowWindow( WIID_QUICK_SLOT,		SP_HIDE );
			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_HIDE );
			ShowWindow( WIID_EXIT,		SP_HIDE );
			ShowWindow( WIID_MINI_TEAM, SP_HIDE);
			ShowWindow( WIID_HELP,		SP_HIDE );
			//ShowWindow( WIID_SHORTCUT	, SP_HIDE);
			ShowWindow( WIID_WORLDMAP_SHORTCUT , SP_HIDE );
			ShowWindow( WIID_MAILLIST	, SP_HIDE);
			ShowWindow( WIID_MAILMSG	, SP_HIDE);
			ShowWindow( WIID_USERSHOP_SELL,	SP_HIDE);
			ShowWindow( WIID_USERSHOP_BUY,	SP_HIDE);
			ShowWindow( WIID_WAYPOINT	, SP_HIDE);
			ShowWindow( WIID_PET	, SP_HIDE);
			ShowWindow( WIID_MICROPHONE, SP_HIDE);
			//ShowWindow( WIID_FIND_MESSAGE, SP_HIDE );//[liuyang, 2008.8.31]
			//ShowWindow( WIID_FIND_MESSAGE_ONE, SP_HIDE );//[liuyang 2008.9.1]
			//ShowWindow( WIID_FIND_MESSAGE_TWO, SP_HIDE );//[liuyang 2008.9.1]
			ShowWindow( WIID_FRIEND_FIND, SP_HIDE );//[liuyang, 2008.8.29]
			ShowWindow( WIID_FRIEND_FOLLOW, SP_HIDE );//[liuyang, 2008.10.10]
			ShowWindow( WIID_FRIEND_SNATCH, SP_HIDE );//[liuyang, 2008.10.11]
			ShowWindow(WIID_GUILD_CREATE, SP_HIDE);
			ShowWindow(WIID_GUILD_EMBLEM, SP_HIDE);
			ShowWindow(WIID_GUILD_DESTROY, SP_HIDE);
			ShowWindow(WIID_GUILD_INFO,	SP_HIDE);
			ShowWindow(WIID_JOYSTICK,	SP_HIDE);
			ShowWindow(WIID_NPC_PORTAL,	SP_HIDE);
			ShowWindow(WIID_NPC_EVENTLIST,	SP_HIDE);
			ShowWindow(WIID_NPC_EVENT,	SP_HIDE);
			ShowWindow(WIID_BUFF,		SP_HIDE);
			ShowWindow(WIID_ITEM_MIX,	SP_HIDE);
			ShowWindow(WIID_MESSAGE,	SP_HIDE);
			ShowWindow(WIID_GAMBLE,		SP_HIDE);
			ShowWindow(WIID_GUIDE_BOOK,	SP_HIDE);
			ShowWindow(WIID_CHATTING_ROOM,	SP_HIDE);
			ShowWindow(WIID_TALKING_ROOM,	SP_HIDE);
			ShowWindow(WIID_CHATTING_PASSWORD,	SP_HIDE);
			ShowWindow(WIID_CASH_TRADE_SELECT,	SP_HIDE);
			ShowWindow(WIID_CASH_TRADE,	SP_HIDE);
			ShowWindow(WIID_CASH_GIFT,	SP_HIDE);
			ShowWindow(WIID_AUCTION,	SP_HIDE);
			ShowWindow(WIID_AUCTION_MSG,SP_HIDE);
			ShowWindow(WIID_LOVE,		SP_HIDE);
			ShowWindow(WIID_LOVEMSG_INPUT,	SP_HIDE);
			ShowWindow(WIID_LOVEMSG_VIEW,	SP_HIDE);
			ShowWindow(WIID_OPTIONRST,	SP_HIDE);
			
			break;
		case CUT_IN_STATE_TALK:
			ShowWindow( WIID_STATPALLET, SP_HIDE);
			//ShowWindow( WIID_ELY,		SP_HIDE );
			ShowWindow( WIID_SHOPTALK,	SP_HIDE );
			break;
		case CUT_IN_STATE_TRADE:
			ShowWindow( WIID_STATPALLET, SP_HIDE);
			ShowWindow( WIID_NPCTALK,	SP_HIDE);
			ShowWindow( WIID_PCTALK,	SP_HIDE);
			
			m_pWindowInven->AdjustCoord(327+44, 248);
			ShowWindow( WIID_ITEM,		SP_SHOW );
			OnExitButtonShow((WPARAM)WIID_ITEM, (LPARAM)false);

			m_pWindowEquip->AdjustCoord(327, 29);
			ShowWindow( WIID_EQUIP,		SP_SHOW );
			OnExitButtonShow((WPARAM)WIID_EQUIP, (LPARAM)false);

			m_pWindowInven->SetMoveable(false);
			m_pWindowEquip->SetMoveable(false);

			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_SHOW );

			//m_pWindowStatus->AdjustCoord(327, 314);
			//m_pWindowStatus->SPSendMessage( SPIM_POS_UPDATE );			
			//ShowWindow( WIID_STATUS,	SP_SHOW );			//상태창은 추후에
			//OnExitButtonShow((WPARAM)WIID_STATUS, (LPARAM)false);			
			//ShowWindow( WIID_ELY,		SP_SHOW );
			//ShowWindow( WIID_ITEMSELL,	SP_SHOW );
			break;
		case CUT_IN_STATE_QUEST:
			ShowWindow( WIID_STATPALLET, SP_HIDE);
			ShowWindow( WIID_NPCTALK,	SP_HIDE );
			ShowWindow( WIID_NPCTRADE,	SP_HIDE );
			//ShowWindow( WIID_ELY,		SP_HIDE );

			ShowWindow( WIID_NPC_QUEST, SP_SHOW );
			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_SHOW );
			break;
		case CUT_IN_STATE_STORAGE:
			ShowWindow( WIID_STATPALLET, SP_HIDE);
			ShowWindow( WIID_NPCTALK,	SP_HIDE );
			
			ShowWindow( WIID_NPCTRADE,	SP_HIDE );
			
			m_pWindowInven->AdjustCoord(211+44, 8);
			ShowWindow( WIID_ITEM,		SP_SHOW );
			OnExitButtonShow((WPARAM)WIID_ITEM, (LPARAM)false);

			m_pWindowEquip->AdjustCoord(211, 267);
			ShowWindow( WIID_EQUIP,		SP_SHOW );
			OnExitButtonShow((WPARAM)WIID_EQUIP, (LPARAM)false);

			m_pWindowInven->SetMoveable(false);
			m_pWindowEquip->SetMoveable(false);

			//ShowWindow( WIID_ELY,		SP_SHOW );
			ShowWindow( WIID_STORAGE,	SP_SHOW );
			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_SHOW );
			break;
		case CUT_IN_STATE_GIFT:
			ShowWindow( WIID_STATPALLET, SP_HIDE);
			ShowWindow( WIID_NPCTALK,	SP_HIDE );
			ShowWindow( WIID_PCTALK,	SP_HIDE);
			ShowWindow( WIID_NPCTRADE,	SP_HIDE );

			//ShowWindow( WIID_ITEM,		SP_SHOW );
			//OnExitButtonShow((WPARAM)WIID_ITEM, (LPARAM)false);

			//m_pWindowEquip->AdjustCoord(327, 114);
			//ShowWindow( WIID_EQUIP,		SP_SHOW );
			//OnExitButtonShow((WPARAM)WIID_EQUIP, (LPARAM)false);
			//m_pWindowInven->SetMoveable(false);
			//m_pWindowEquip->SetMoveable(false);

			//ShowWindow( WIID_ELY,		SP_SHOW );
			ShowWindow( WIID_GIFT,		SP_SHOW );
			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_SHOW );
			break;
		case CUT_IN_STATE_SAVEPOINT:
			ShowWindow( WIID_SHOPTALK,	SP_HIDE );
			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_SHOW );
			break;
		case CUT_IN_STATE_MAIL:
			ShowWindow(WIID_MAILLIST,	SP_SHOW);
			//ShowWindow( WIID_ELY,		SP_SHOW );
			ShowWindow(WIID_SHOPTALK,	SP_SHOW);
			//ShowWindow(WIID_MAILMSG,	SP_SHOW);
			
			m_pWindowInven->AdjustCoord(362+44, 9);
			//ShowWindow( WIID_ITEM,		SP_SHOW );
			//OnExitButtonShow((WPARAM)WIID_ITEM, (LPARAM)false);
			m_pWindowInven->SetMoveable(false);			
			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_SHOW );
			break;
		case CUT_IN_STATE_BEAUTY:			
			ShowWindow(WIID_SHOPTALK,	SP_SHOW);
			ShowWindow(WIID_BEAUTYSHOP,	SP_SHOW);
			
			//m_pWindowInven->AdjustCoord(362, 9);
			//m_pWindowInven->SetMoveable(false);
			break ;
		case CUT_IN_STATE_GUILD_CREATE:
			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_SHOW );
			ShowWindow(WIID_SHOPTALK,	SP_SHOW);
			ShowWindow(WIID_GUILD_CREATE, SP_SHOW);
			m_pWindowCreateGuild->SetFocus();
			break;
		case CUT_IN_STATE_GUILD_DISSOLVE:
			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_SHOW );
			ShowWindow(WIID_SHOPTALK,	SP_SHOW);	
			ShowWindow(WIID_GUILD_DESTROY, SP_SHOW);
			m_pWindowDestroyGuild->SetFocus();
			break;
		case CUT_IN_STATE_GUILD_CROPS:
			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_SHOW );
			ShowWindow(WIID_CROPS,		SP_SHOW);
			m_pWindowInven->AdjustCoord(333+44, 32);
			ShowWindow( WIID_ITEM,		SP_SHOW );
			OnExitButtonShow((WPARAM)WIID_ITEM, (LPARAM)false);
			m_pWindowInven->SetMoveable(false);
			m_pWindowGuildCrop->SetFocus() ;
			break;
		case CUT_IN_STATE_NPC_PORTAL:
			ShowWindow( WIID_STATPALLET, SP_HIDE);
			ShowWindow( WIID_NPCTALK,	SP_HIDE );
			ShowWindow( WIID_PCTALK,	SP_HIDE);
			ShowWindow( WIID_NPCTRADE,	SP_HIDE );
			ShowWindow(WIID_NPC_PORTAL, SP_SHOW);
			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_SHOW );
			break;
		case CUT_IN_STATE_NPC_CHANGE:
			ShowWindow(WIID_STATPALLET,			SP_HIDE);
			ShowWindow(WIID_NPCTALK,			SP_HIDE);
			ShowWindow(WIID_PCTALK,				SP_HIDE);
			ShowWindow(WIID_NPCTRADE,			SP_HIDE);
			ShowWindow(WIID_NPC_EVENTLIST,		SP_SHOW);
			ShowWindow(WIID_NPC_EVENT,			SP_SHOW);
			ShowWindow(WIID_SYSTEM_MESSAGE,		SP_SHOW);
			break;
		case CUT_IN_STATE_GUILD_LEVEL_UP:			
			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_SHOW );
			ShowWindow(WIID_SHOPTALK,	SP_SHOW);
			ShowWindow(WIID_GUILD_LEVELUP, SP_SHOW) ;
			m_pWindowGuildLevelup->SetFocus();
			break;
		case CUT_IN_STATE_GUILD_EMBLEM:
			ShowWindow( WIID_SYSTEM_MESSAGE,	SP_SHOW );
			ShowWindow(WIID_SHOPTALK,	SP_SHOW);
			ShowWindow(WIID_GUILD_EMBLEM, SP_SHOW) ;
			m_pwindowGuildEmblem->SetFocus() ;
			break;
		case CUT_IN_STATE_PVP_RESERVE:
			ShowWindow( WIID_STATPALLET, SP_HIDE);
			ShowWindow( WIID_NPCTALK,	SP_HIDE );
			ShowWindow( WIID_PCTALK,	SP_HIDE);
			ShowWindow( WIID_NPCTRADE,	SP_HIDE );
			
			ShowWindow(WIID_SYSTEM_MESSAGE, SP_SHOW);
			ShowWindow(WIID_SHOPTALK,		SP_SHOW);
			ShowWindow(WIID_PVPRESERVE,		SP_SHOW);
			break ;
		case CUT_IN_STATE_NPC_AUCTION:
			ShowWindow( WIID_STATPALLET, SP_HIDE);
			ShowWindow( WIID_NPCTALK,	SP_HIDE );
			ShowWindow( WIID_PCTALK,	SP_HIDE);
			ShowWindow( WIID_NPCTRADE,	SP_HIDE );
			//ShowWindow(	WIID_AUCTION_MSG, SP_SHOW);
			ShowWindow(	WIID_AUCTION, SP_SHOW);
			m_pWindowInven->SPSendMessage(SPIM_ITEM_SUBMENU, 0, (LPARAM)false) ;		// 상점&업그레이드 안됨
			ShowWindow(	WIID_SYSTEM_MESSAGE, SP_SHOW);
		case CUT_IN_STATE_NPC_REOPTION:
			ShowWindow( WIID_STATPALLET, SP_HIDE);
			ShowWindow( WIID_NPCTALK,	SP_HIDE );
			ShowWindow( WIID_PCTALK,	SP_HIDE);
			ShowWindow( WIID_NPCTRADE,	SP_HIDE );
			
			m_pWindowInven->AdjustCoord(37, 11);
			ShowWindow(WIID_ITEM,		SP_SHOW);
			OnExitButtonShow((WPARAM)WIID_ITEM, (LPARAM)false);

			m_pWindowEquip->AdjustCoord(280, 50);
			ShowWindow(WIID_EQUIP,		SP_SHOW);
			OnExitButtonShow((WPARAM)WIID_EQUIP, (LPARAM)false);

			m_pWindowInven->SetMoveable(false);
			m_pWindowEquip->SetMoveable(false);
			
			ShowWindow(WIID_SHOPTALK,		SP_SHOW);
			ShowWindow(WIID_OPTIONRST,	SP_SHOW);
			ShowWindow(WIID_SYSTEM_MESSAGE,	SP_SHOW);
			break;
		case CUT_IN_STATE_LOVE:
			SetShowDefaultWindow( false );
			ShowWindow( WIID_CHAT				, true );
			ShowWindow( WIID_LOVEMSG_INPUT		, true );
			break;
			
			
			//- Ranking: W.Fairy 
			//----------------------------------------------------
			//{
		case CUT_IN_STATE_RANKING: 
			{
				ShowWindow(WIID_STATPALLET	,SP_HIDE);
				ShowWindow(WIID_NPCTALK		,SP_HIDE);
				ShowWindow(WIID_PCTALK		,SP_HIDE);
				ShowWindow(WIID_NPCTRADE	,SP_HIDE);

				ShowWindow(WIID_RANKING		,SP_SHOW);
			}
			break;
			//}
		default:
			break;
	}
	
	////
	//bool bShow = (bool)wParam;
	//if(g_pEventManager->GetCutInState() > CUT_IN_STATE_SCROLL){
	//	if(bShow) {			//보여줌
	//		ShowWindow( WIID_STATPALLET, SP_SHOW);	//기본 스텟 윈도퓖E<주의할것>

	//		ShowWindow( WIID_NPCTALK,	SP_HIDE );	//[2005/5/16] - jinhee
	//		ShowWindow( WIID_SHOPTALK,	SP_HIDE );	//[2005/5/16] - jinhee
	//		ShowWindow( WIID_NPCTRADE,	SP_HIDE );	//[2005/5/18] - jinhee
	//	}
	//	else {				//숨콅E
	//		ShowWindow( WIID_STATPALLET, SP_HIDE);	//기본 스텟 윈도퓖E<주의할것>

	//		//ShowWindow( WIID_ITEM,		SP_HIDE );
	//		//ShowWindow( WIID_EQUIP,		SP_HIDE );
	//		//ShowWindow( WIID_SKILL,		SP_HIDE );
	//		//ShowWindow( WIID_STATUS,	SP_HIDE );
	//		//ShowWindow( WIID_MENU,		SP_HIDE );
	//		//ShowWindow( WIID_MESSAGE,	SP_HIDE );
	//		//ShowWindow( WIID_LOOTING,	SP_HIDE );
	//	}
	//}	

	return 1;
}


int SPInterfaceManager::OnItemSellSelect(WPARAM wParam, LPARAM lParam)
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pWindow = (*m_ppCurSubgame)->Find(WIID_ITEM, true);	
	if(pWindow) 
	{
		return pWindow->SPSendMessage(SPIM_ITEM_SELECT, wParam, lParam);
	}
	return 0;
}


int SPInterfaceManager::OnExitButtonShow(WPARAM wParam, LPARAM lParam)
{	
	if( m_pSubGameZone == NULL )
		return 0;

	unsigned int uId = (unsigned int)wParam;	
	
	SPWindow* pWindow = m_pSubGameZone->Find( (INSTANCE_ID)wParam, true);
	if( pWindow && pWindow->IsShow() ) {
		pWindow->SPSendMessage(SPIM_SET_EXIT_SHOW, wParam, lParam);
	}
	
	//SPWindow* pWindow = m_pSubGameZone->Find(WIID_EQUIP, true);
	//if( pWindow && pWindow->IsShow() ) {
	//	pWindow->SPSendMessage(SPIM_SET_EXIT_SHOW, wParam, lParam);
	//}	
	
	return 0;
}

int SPInterfaceManager::OnChannelListSet(WPARAM wParam, LPARAM lParam)
{
	if( m_pSubGameZone == NULL )
		return 0;

	SPWindow* pWindow = m_pSubGameZone->Find(WIID_CHANNEL_CHANGE, true);
	if(pWindow) {
		return pWindow->SPSendMessage(SPIM_CHANNEL_SET, wParam, lParam);
	}
	return 0;
}

int SPInterfaceManager::OnSetPlayerInfo(WPARAM wParam, LPARAM lParam)
{
	if( m_pSubGameZone == NULL )
		return 0;

	SPWindow* pWindow = m_pSubGameZone->Find(WIID_PC_INFO, true);
	if(pWindow) {
		return pWindow->SPSendMessage(SPIM_SET_PLAYER_INFO, wParam, lParam);
	}
	return 0;
}

int SPInterfaceManager::OnSetPCInfoStatus(WPARAM wParam, LPARAM lParam)
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pWindow = (*m_ppCurSubgame)->Find( WIID_PC_STATUS , true );

	if(pWindow) 
		return pWindow->SPSendMessage( SPIM_PC_INFO_SET_STATUS , wParam, lParam );

	return 0;
}


int SPInterfaceManager::OnTeamJoin(WPARAM wParam, LPARAM lParam)
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pkWindow = (*m_ppCurSubgame)->Find(WIID_MINI_TEAM);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_TEAM_JOIN, wParam, lParam);

	pkWindow = (*m_ppCurSubgame)->Find(WIID_TEAM);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_TEAM_JOIN, wParam, lParam);

	return 1;
}

int SPInterfaceManager::OnTeamSecede(WPARAM wParam, LPARAM lParam)
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pkWindow = (*m_ppCurSubgame)->Find(WIID_MINI_TEAM);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_TEAM_SECEDE, wParam, lParam);

	pkWindow = (*m_ppCurSubgame)->Find(WIID_TEAM);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_TEAM_SECEDE, wParam, lParam);

	return 1;
}

int SPInterfaceManager::OnTeamUpdate(WPARAM wParam, LPARAM lParam)
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pkWindow = (*m_ppCurSubgame)->Find(WIID_MINI_TEAM);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_TEAM_UPDATE, wParam, lParam);

	pkWindow = (*m_ppCurSubgame)->Find(WIID_TEAM);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_TEAM_UPDATE, wParam, lParam);

	return 1;
}

int SPInterfaceManager::OnTeamUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pkWindow = (*m_ppCurSubgame)->Find(WIID_MINI_TEAM);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_TEAM_UPDATE_STATUS, wParam, lParam);

	return 1;
}

int SPInterfaceManager::OnTeamUpdateLootingRule(WPARAM wParam, LPARAM lParam)
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pkWindow = (*m_ppCurSubgame)->Find(WIID_TEAM);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_TEAM_UPDATE_LOOTING_RULE, wParam, lParam);

	return 1;
}


/**
	Shift+Click의 경우 아이템 이름을 셋팅해주는 기능
	사용 : SPWindowAuction
	//? 컷인에서만 적용되어야 하나 테스트를 위해 풀어놓음
 */
int SPInterfaceManager::OnItemIDInfo(WPARAM wParam, LPARAM lParam)
{
	int iItemID = (int)wParam;

	if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NPC_AUCTION) {
		SPWindow* pkWindow = (*m_ppCurSubgame)->Find(WIID_AUCTION);
		if( pkWindow )
			pkWindow->SPSendMessage(SPIM_AUCTION_SET_SEARCHNAME, (WPARAM)iItemID, lParam) ; ;
	}
	return 1;
}


int SPInterfaceManager::OnSetErrorMsg(WPARAM wParam, LPARAM lParam)
{
	if(m_ppCurSubgame == NULL || (*m_ppCurSubgame) == NULL)
		return 0;

	SPWindow* pkWindow = (*m_ppCurSubgame)->Find(WIID_ERROR);
	if( pkWindow )
	{
		pkWindow->SPSendMessage(SPIM_ERROR_SET_MSG, wParam, lParam);
		if( m_SubGameID == SUBGAME_LOBBY || m_SubGameID == SUBGAME_PROLOGUE ||
			(m_SubGameID == SUBGAME_ZONE && m_bCashShop) )
			SetModalWindow(pkWindow);
	}

	if( m_SubGameID == SUBGAME_ZONE )
	{
		vector< unsigned int > vHideWindowList;

		vHideWindowList.push_back(WIID_ITEM);
		vHideWindowList.push_back(WIID_EQUIP);
		vHideWindowList.push_back(WIID_SKILL);
		vHideWindowList.push_back(WIID_STATUS);
		vHideWindowList.push_back(WIID_QUEST);
		vHideWindowList.push_back(WIID_NPC_QUEST);
		vHideWindowList.push_back(WIID_QUESTVIEW);
		vHideWindowList.push_back(WIID_QUESTNOTIFY);
		vHideWindowList.push_back(WIID_TEAM);
		vHideWindowList.push_back(WIID_MESSAGE);
		vHideWindowList.push_back(WIID_EXIT);
		vHideWindowList.push_back(WIID_LOOTING);
		vHideWindowList.push_back(WIID_NPCTALK);
		vHideWindowList.push_back(WIID_SHOPTALK);
		vHideWindowList.push_back(WIID_NPCTRADE);
		//vHideWindowList.push_back(WIID_ITEMSELL);
		//vHideWindowList.push_back(WIID_ELY);
		vHideWindowList.push_back(WIID_CHANNEL_CHANGE);
		vHideWindowList.push_back(WIID_STORAGE);
		vHideWindowList.push_back(WIID_PC_INFO);
		vHideWindowList.push_back(WIID_PC_TRADE);
		vHideWindowList.push_back(WIID_NOTICEBOX);
		vHideWindowList.push_back(WIID_PC_EQUIP);
		vHideWindowList.push_back(WIID_PC_STATUS);
		vHideWindowList.push_back(WIID_FRIEND);
		vHideWindowList.push_back(WIID_LOTTERY);
		vHideWindowList.push_back(WIID_CHAT_MACRO);
		vHideWindowList.push_back(WIID_NPC_QUEST);
		vHideWindowList.push_back(WIID_OPTION);
		vHideWindowList.push_back(WIID_HELP);
		vHideWindowList.push_back(WIID_ITEM_MIX);
		vHideWindowList.push_back(WIID_GAMBLE);
		vHideWindowList.push_back(WIID_GUIDE_BOOK);
		vHideWindowList.push_back(WIID_CHATTING_ROOM);
		vHideWindowList.push_back(WIID_TALKING_ROOM);
		vHideWindowList.push_back(WIID_CHATTING_PASSWORD);
		vHideWindowList.push_back(WIID_LOVE);
		vHideWindowList.push_back(WIID_LOVEMSG_INPUT);
		vHideWindowList.push_back(WIID_LOVEMSG_VIEW);
		
		vector< unsigned int >::iterator iter = vHideWindowList.begin();
		while( iter != vHideWindowList.end() )
		{
			pkWindow = (*m_ppCurSubgame)->Find(*iter);
			if( pkWindow )
				pkWindow->Hide();

			++iter;
		}

		vHideWindowList.clear();
	}
	
	m_bActiveErrorMsg = true;
	return 1;
}

SPWindow* SPInterfaceManager::GetSubGameZone()//[liuyang. 2008.9.5]
{
	return m_pSubGameZone;
}
