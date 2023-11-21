

#include "LataleVersionDef.h"
#include "SPCommon.h"
#include "DXUtil.h"
#include "SPResourceDef.h"
#include "SPUtil.h"

#include "SPDebug.h"

#include "SPGameObjectDef.h"
#include "SPGameObject.h"

#include "SPRenderModelDEF.h"

#include "SPTerrainCluster.h"
#include "SPMainGameDEF.H"

#include "SPManager.h"
#include "SPCameraManager.h"

#include "SPSubGameManager.h"
#include "SPLoggin.h"
#include "SPZone.h"
#include "SPLoading.h"
#include "SPLobby.h"
#include "SPPrologue.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPIMEPool.h"
#include "SPAbuseFilter.h"

#include "SPTerrainDEF.h"
#include "SPTerrainManager.h"
//#include "SPItem.h"
#include "SPItemCluster.h"		//[2005/6/16]
#include "SPGOBManager.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXManager.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"
#include "SPInputManager.h"
#include "SPWindowDEF.h"

#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBAnimation.h"
#include "SPGOBModel.h"
#include "SPAvatarModelUnitDef.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayer.h"
#include "SPMouseCursor.h"

#include "SPGuildArchive.h"

#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPNetworkDEF.h"
#include "SPNetworkManager.h"

#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"

#include "SPFrameSkipper.h"
#include "SPMainGameManager.h"

#include "SPEventDEF.h"						//[2005/4/20] - jinhee
#include "SPEvent.h"
#include "SPEventNpcHandler.h"
#include "SPQuestManager.h"
#include "SPEventManager.h"

#include "SPCheckManager.h"

#include "SPStage.h"						//[2005/6/10] - jinhee
#include "SPStageManager.h"

#include "SPSoundManager.h"
#include "SPSkill.h"
#include "SPSkillManager.h"
#include "SPEffectManager.h"
#include "SPCoolTimeManager.h"
#include "SPComboManager.h"
#include "SPStoryManager.h"
#include "SPTextBookManager.h"
#include "SPIndun.h"
#include "SPIndunManager.h"
#include "SPGuildLDT.h"
#include "SPPvpManager.h"
#include "SPCashTradeManager.h"
#include "SPAuctionManager.h"
#include "SPChatManager.h"
#include "SPLoveManager.h"

#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"

#include "SPLogoThread.h"
#include "_Error_log.h"
//#include "SPDebug.h"

//#include "SPSDRSClient.h"

#include "SPRefOLE.h" // for IngameWB
#include "SPIngameWebBrowser.h"
#include "SPStream.h"

#include "SPReport.h"
#include "lgclientlog.h"
#include "IGWInterface.h"
#include "IGWLoaderDx9.h"
#include "SPWindow.h"
#include "SPWindowOption.h"

extern char g_szGameServerIP[];
extern int g_iGameServerPort;
extern INITTHREAD_INFO g_InitialThreadInfo ; 

extern FLOAT  g_fFPS;
extern bool g_Loading;
//extern SPSDRSClient* g_pSDRSClient;
void WINAPI SetAudioSoundVolume(DWORD NewVolume);
DWORD WINAPI GetAudioSoundVolume();
void WINAPI SetAudioEffectVolume(DWORD NewVolume);
DWORD WINAPI GetAudioEffectVolume();

SPMainGameManager*	g_pMainGameManager	= NULL;
extern DWORD            g_RenderFrame;
void CALLBACK s_WebEnableCallback(bool bEnable)
{
	if(g_pMainGameManager)
		g_pMainGameManager->WebEnableCallBack(bEnable);
}


SPMainGameManager::SPMainGameManager()
: m_pMgrTerrain		(NULL)
, m_pMgrGob			(NULL)
, m_pMgrFX			(NULL)
, m_pMgrCamera		(NULL)
, m_pMgrInput		(NULL)
, m_pMgrInterface	(NULL)
, m_pMgrResource	(NULL)
, m_pMgrNetwork		(NULL)
, m_pMgrEvent		(NULL)
, m_pMgrCheck		(NULL)
, m_pLogoDisplay	(NULL)
, m_bActive			(false)
, m_fElapsedTime	(0.0f)
, m_fFPS			(0.0f)
, m_fUpdateTime		(0.0f)
, m_bPlayBGM		(false)	
, m_pMgrSubGame		(NULL)
, m_bRenderDebugInfo(false)
, m_bExit			(false)
, m_fAccumulateNotice(0.0f)
, m_fDelayNotice	(0.0f)
, m_fEndNotice		(0.0f)
//, m_pFont			(NULL)
, m_bShowAll		(false)
, m_iReportIndicate	(0)
, m_iNoticeOption (0)
{	
}

SPMainGameManager::~SPMainGameManager() 
{	
	while( !m_bInitialize )
	{
		Sleep(30);
	}

	//SAFE_DELETE(m_pFont);	

	SAFE_DELETE( m_pMgrTerrain	);
	SAFE_DELETE( m_pMgrGob		); 
	SAFE_DELETE( m_pMgrEvent	);
	SAFE_DELETE( m_pMgrCheck	);
	SAFE_DELETE(m_pMgrSubGame);

	SAFE_DELETE( m_pMgrCamera	);
	SAFE_DELETE( m_pMgrInput	);
	SAFE_DELETE( m_pMgrInterface);	
	SAFE_DELETE( m_pMgrNetwork	);
	SPCashTradeManager::Release();		//[2008/4/16] - ¾ÆÀÌÅÛ Å¬·¯½ºÅÍ¿Í ¿¬°á µÇ¾î ÀÖ¾î ¸ÕÀú Á¦°Å
	SAFE_DELETE( m_pMgrResource );
	SAFE_DELETE( m_pLogoDisplay );

	SAFE_DELETE( m_pMgrFX		);		/// Interface ¾Æ·¡¿¡ ÀÖ¾ûÚßÇÔ (Login¿¡¼­ »ç¿E

	SPSoundManager::Release();
	SPSkillManager::Release();
	SPEffectManager::Release();
	SPCoolTimeManager::Release();
	SPComboManager::Release();
	SPIndunManager::Release();
	SPPvpManager::Release();
	SPGuildLDT::Release() ;	
	SPAuctionManager::Release() ;
	SPChatManager::DelInstance();
	SPLoveManager::DelInstance();

	SAFE_DELETE( g_pClientOption );

	SPAbuseFilter::Release();
	SPLocalizeManager::Release();
	SPStoryManager::Release();
	SPTextBookManager::Release();

	m_vpManager.clear();	
}


/************************************************************************/
/*	For Thread, separate Init() function
	1) Main Thread allocation function
	2) Sub Thread Init() function
	3) Main Thread PostInit() function	
	# NetworkManager & InputManager must initialize in main thread		*/
/************************************************************************/
void SPMainGameManager::InitAllocate()
{
	m_pMgrTerrain	= new SPTerrainManager;
	m_pMgrGob		= new SPGOBManager;		
	m_pMgrFX		= new SPFXManager;
	m_pMgrCamera	= new SPCameraManager; 
	m_pMgrInput		= new SPInputManager;
	m_pMgrInterface = new SPInterfaceManager;
	m_pMgrResource  = new SPResourceManager;
	m_pMgrEvent		= new SPEventManager;
	m_pMgrCheck		= new SPCheckManager;

	//////////////////////////////////////////////////////////////////////////
	m_pMgrResource->Init();
	SPLocalizeManager::GetInstance()->Init();

	bool bEngishOnly = (SPLocalizeManager::GetInstance()->GetCountryType() == CT_OGP);
	m_pMgrInterface->SetEnglishOnly(bEngishOnly);

	SPGuildLDT::GetInstance()->Init();

#ifndef _ALONE
	m_pMgrNetwork	= new SPNetworkManager;
	m_pMgrNetwork->Init();
#endif
	m_pMgrCamera->Init();
	m_pMgrInput->Init();
	m_pMgrInput->SetMgrCamera( m_pMgrCamera );
	m_pMgrInput->SetJoyPadKeyName();

	// Client Option
	g_pClientOption = new SPClientOptionArchive;
	
	//ÉùÒôÉèÖÃ,ÔÝÊ±¹Ø±Õ
	if ( g_pCIGWLoaderDx9 )
	{
		g_pCIGWLoaderDx9->SetAudioSetting( GetAudioSoundVolume, SetAudioSoundVolume, GetAudioEffectVolume, SetAudioEffectVolume );
	}
}

void SPMainGameManager::Init()
{
	//g_pMainGameManager = this;
	Sleep(20) ;
	m_iGameState = GS_INIT;	

	g_Loading = true;
	//g_Log.Log("Gob s");
	
	//ErrorFLogs->Error_Log("----->>> CalcTime Start") ;
	//////////////////////////////////////////////////////////////////////////
	// For Thread Time Check
	// This Time is about 1/8 Time in MaingameManager Loading
	g_InitialThreadInfo.SetProtege(1, 40) ;
	int iMGStartTime = timeGetTime() ;
	m_pMgrResource->InitItemCluster() ;
	g_InitialThreadInfo.m_iCheckTime = timeGetTime() - iMGStartTime ;
	Sleep(5) ;

	InitEquipPart();
	InitEquipRestore();
	g_InitialThreadInfo.m_strInitErr = "Equip Init Finish" ;
	Sleep(5) ;

	m_pMgrFX->Init();
	g_InitialThreadInfo.SetProtege(15, 70) ;
	g_InitialThreadInfo.SetInitMsg("FxManager Init Finish") ;
	Sleep(5) ;



	m_pMgrGob->Init();
	g_InitialThreadInfo.SetProtege(58, 85) ;
	g_InitialThreadInfo.SetInitMsg("GobManager Init Finish") ;
	Sleep(5) ;

	m_pMgrTerrain->Init();
	g_InitialThreadInfo.SetInitMsg("TerrainManager Init Finish") ;
	Sleep(5) ;

	m_pMgrEvent->Init();
	g_InitialThreadInfo.SetProtege(80, 98) ;
	g_InitialThreadInfo.SetInitMsg("EventManager Init Finish") ;
	Sleep(5) ;

	m_pMgrCheck->Init();
	m_pMgrInterface->Init();
	g_InitialThreadInfo.SetProtege(99, 100) ;
	g_InitialThreadInfo.SetInitMsg("InterfaceManager Init Finish") ;
	Sleep(5) ;
	
	//EnterCriticalSection(&g_TextureLoadSection) ;
	//m_pMgrInput->Init();
	//m_pMgrInput->SetMgrCamera( m_pMgrCamera );
	//LeaveCriticalSection(&g_TextureLoadSection) ;
	
	m_pMgrInterface->SetMgrCamera( m_pMgrCamera );
	m_pMgrTerrain->SetMgrCamera( m_pMgrCamera );
	m_pMgrGob->SetMgrCamera( m_pMgrCamera );
	m_pMgrFX->SetMgrCamera( m_pMgrCamera );
	m_pMgrEvent->SetMgrCamera( m_pMgrCamera );
	m_pMgrCheck->SetMgrCamera( m_pMgrCamera );
	g_InitialThreadInfo.SetInitMsg("Manager Set Finish") ;
	g_InitialThreadInfo.SetProtege(100, 100) ;

	//m_vpManager.push_back(m_pMgrCamera);
	//m_vpManager.push_back(m_pMgrResource);
	m_vpManager.push_back(m_pMgrGob);
	//m_vpManager.push_back(m_pMgrInput);				//	ÀÎÇ²¸Å´ÏÀú´Â ÀÎÅÍÆäÀÌ½º¸Å´ÏÀúº¸´Ù ¿E?ÃÊ±âÈ­ µÇ¾ûÚßÇÔ
	//m_vpManager.push_back(m_pMgrInterface);			//	AJJIYA  [7/14/2005]
	m_vpManager.push_back(m_pMgrTerrain);
	m_vpManager.push_back(m_pMgrFX);
	m_vpManager.push_back(m_pMgrEvent);
	m_vpManager.push_back(m_pMgrCheck);
	g_InitialThreadInfo.SetInitMsg("vpManager Insert Finish") ;
	Sleep(5) ;

	// ·Î°E³ª¿À°E³ªÁß¿¡ ¸±¸®ÁEµÉ¶§ »ç¿E
	//BeginInitialize();
	m_bInitialize = true;
	g_InitialThreadInfo.m_bFinish = true ;


	if( g_pIngameWebBrowser ) {
		g_pIngameWebBrowser->SetReadyCallBack(s_WebEnableCallback);
	}


	//g_Log.Log("Gob e");
}



void SPMainGameManager::PostInit()
{
	SPAbuseFilter::Create();
	SPAbuseFilter::GetInstance()->Init();
	SPStoryManager::GetInstance()->Init();
	SPTextBookManager::GetInstance()->Init();
	ProcessLogoDisplay() ;
	SPSoundManager::GetInstance()->Init();
	SPSkillManager::GetInstance()->Init();
	g_InitialThreadInfo.SetInitMsg("Skill Init Finish") ;
	ProcessLogoDisplay() ;
	SPIndunManager::GetInstance()->Init();
	SPPvpManager::GetInstance()->Init() ;
	SPCashTradeManager::GetInstance()->Init();
	g_InitialThreadInfo.SetInitMsg("SPCashTradeManager Init Finish") ;
	SPAuctionManager::GetInstance()->Init() ;
	g_InitialThreadInfo.SetInitMsg("SPAuctionManager Init Finish") ;	
	SPChatManager::GetInstance()->Init();
	g_InitialThreadInfo.SetInitMsg("SPChatManager Init Finish") ;
	SPLoveManager::GetInstance()->Init();
	g_InitialThreadInfo.SetInitMsg("SPLoveManager Init Finish") ;
	SPEffectManager::GetInstance()->Init();
	ProcessLogoDisplay() ;
	g_InitialThreadInfo.SetInitMsg("Effect Init Finish") ;	

	switch(g_ExcutionMode)
	{
	case EMODE_NORMAL:	
	default:			
		ChangeSubGame(SUBGAME_LOGGIN);
		break;

		// Ä³¸¯ÅÍ ¼¿·ºÆ® ¸ðµE- Ä³¸¯ÅÍ ¼±ÅÃÇÏ¿© Á÷Á¢ Á¸¿¡ µé¾ûÌ£´Ù.
	case EMODE_CHAR_SELECT:	
		{
			ChangeSubGame(SUBGAME_LOBBY);
			
			g_pNetworkManager->SetGameServerInfo(g_szGameServerIP,g_iGameServerPort);
			g_pNetworkManager->SetCharName(g_szCharName);
			//m_pMgrNetwork->EnterWorld();
		}
		break;
	}

	m_strNotice = "";
	m_strNotice.clear();
	m_strOrgNotice = "";
	m_strOrgNotice.clear();
	SetRect(&m_rtNotice, 200, 114, 600, 214);
	m_fAccumulateNotice = 0.0f;
	m_fDelayNotice = 1.0f;
	m_fEndNotice = 0.0f;	
	g_InitialThreadInfo.m_strInitErr = "" ;
	//ErrorFLogs->Error_Log("----->>> Complete End") ;

	FLOAT fCurTime		= DXUtil_Timer(TIMER_GETABSOLUTETIME);
	m_fElapsedTime		= DXUtil_Timer(TIMER_GETELAPSEDTIME);
}


bool SPMainGameManager::ChangeSubGame(SUBGAME_ID SubGameID, bool bPurgeAll /*= true*/)//´ó³¡¾°µÄÇÐ»»
{
	//if(m_pMgrSubGame) 
	//	m_pMgrSubGame->Clean();
	//SAFE_DELETE(m_pMgrSubGame);

	if(m_pMgrSubGame && bPurgeAll) 
		m_pMgrSubGame->Clean();
	//else if(m_pMgrSubGame && bPurgeAll == false){	//Stage°¡ º¯°æµÇ¾E·ÎµùÀÌ ÀÏ¾ûÏª´Â °æ¿E
	//	g_pInterfaceManager->PurgeAll();
	//	g_pTerrainManager->PurgeAll();
	//	g_pGOBManager->ResetObject();
	//}
	SAFE_DELETE(m_pMgrSubGame);

	SPSoundBase::GetInstance()->SetRoomType(0);

	int iStage = -1;
	int iGroup = -1;
	std::string strBGM = "";
	int iSoundEffect = 0;

	if(SubGameID == SUBGAME_LOADING)
	{
		g_Loading = true;
	}
	else
	{
		if(g_Loading)
			g_Loading = false;
	}

	switch(SubGameID) {
	case SUBGAME_LOGGIN	:

		m_pMgrSubGame = new SPLoggin;
		if( g_pCameraManagr )
			g_pCameraManagr->Init();
		m_pMgrSubGame->Init();
		m_iGameState = GS_GAME;	
		if( g_pkPvpManager )		// ¼­¹ö¿À·ù½Ã PVPÃÊ±âÈ­¸¦ À§ÇØ¼­ ¼³Á¤
			g_pkPvpManager->ClearPvpInfo() ;
		g_pInterfaceManager->SetSubGame( SubGameID );
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->SetVisible( false );
		}
		break;
	case SUBGAME_ZONE	:		
		m_pMgrSubGame = new SPZone;	
		if(g_pGOBManager->GetLocalPlayer()) {
		//[2005/6/10] BGM Setting
			iStage = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
			iGroup = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();

			SPStage* pStage;
			pStage = g_StageManager.GetCurStage(iStage);
			if( pStage )
			{
				SPMapGroup* pMapGroup;
				pMapGroup = pStage->GetMapGroup(iGroup);
				if( pMapGroup )
				{
					strBGM = pMapGroup->m_strBGMFile;
					iSoundEffect = pMapGroup->m_iSoundEffectType;
				}
			}

			if(strBGM != "") {
				m_pMgrSubGame->SPPerformMessage( SPGM_SUG_SETBGM , (WPARAM)strBGM.c_str());
			}
			SPSoundBase::GetInstance()->SetRoomType(iSoundEffect);
		}
		
		m_pMgrSubGame->Init();
		m_iGameState = GS_GAME;

		// PVP -> Indun ÀÌµ¿ÀÌ ÀÖÀ¸¸é ¼öÁ¤ÇØ¾ßÇÔ.
		if( g_pkPvpManager->GetPvpStatus() != PVP_STATUS_EMPTY )
		{
			g_pInterfaceManager->SetSubGame( SubGameID );
			m_pMgrSubGame->SetGameState(GS_PVPWAIT);
			m_pMgrSubGame->SPPerformMessage(SPGM_PVP_LOADWAIT_SHOW, 1) ;		// Show PVP LoadWait
			g_pCameraManagr->SetCameraMode(SPCameraManager::FOCUS_CHAR);		//[2005/11/25]
			g_pEventManager->SetEventBlock(true);			// ÇÊ¿äÇÏ´Ù¸é EventBlock
		}
		else
		{
			m_pMgrSubGame->SetGameState(GS_FADE_OUT);
			g_pCameraManagr->SetCameraMode(SPCameraManager::FOCUS_CHAR);
			g_pInterfaceManager->SetSubGame( SubGameID );
		}
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->SetVisible( true );
		}
		//}
		break;
		
	case SUBGAME_LOADING:

		SPSoundBase::GetInstance()->SetMainBGM(NULL);
		if(bPurgeAll == false){	//Stage°¡ º¯°æµÇ¾E·ÎµùÀÌ ÀÏ¾ûÏª´Â °æ¿E
			g_pInterfaceManager->PurgeAll();
			g_pTerrainManager->PurgeAll();
			g_pGOBManager->ResetObject();
			
			////ÀÌ¶§ QuestManagerÀÇ Archiveµµ ÆÛÁöAll ÇØÁÖÀÚ...
			////Æ÷Å»½Ã »õ·Î ¹Þ±E¶§¹®¿¡ ¸®¼ÂÇØÁÖ´ø°ÍÀÓ...
			//g_pEventManager->GetQuestManager()->PurgeAll();
			
			if(g_pGOBManager->GetLocalPlayer()) {				//[2005/10/31] ¿©±â¼­ ·ÎÄÃ ÇÃ·¹ÀÌ¾ûÜÇ À§Ä¡¸¦ ÀÌµ¿ÇØ¾ß ÇÒ°÷À¸·Î ¼³Á¤ÇÑ´Ù
				iStage = g_StageManager.GetNextStage();
				iGroup = g_StageManager.GetNextMapGroup();
				g_pGOBManager->GetLocalPlayer()->SetStage(iStage, iGroup);
				g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->SetCoordPhysicsMap(iStage, iGroup);
			}
		}

		//[2005/6/10] ¸¸ÀÏ ·ÎµùÈ­¸é¿¡¼­ ÀÌÀE¹è°æBGMÀ» »ç¿EÏÁE¾Ê´Â °æ¿E
		//m_pMgrSubGame->SPPerformMessage( SPGM_SUG_SETBGM , (WPARAM)0);

		if(g_pGOBManager->GetLocalPlayer()) {
			iStage = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
			iGroup = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();		
			m_pMgrSubGame = new SPLoading;
			m_pMgrSubGame->Init();
			m_pMgrSubGame->SPPerformMessage(SPGM_SUG_LOAD_SET_STAGE, 0, (LPARAM)iStage);
			m_pMgrSubGame->SPPerformMessage(SPGM_SUG_LOAD_SET_GROUP, 0, (LPARAM)iGroup);
		}		
		
		//½ºÅ×ÀÌÁö°¡ º¯°æµÇ´Â °æ¿E·ÎµùÈÄ ·ÎµE¿Ï·E¸Þ½ÃÁö¸¦ º¸³»¾ß ÇÒ ½ÃÁ¡¿¡ ´EØ¼?Á»´E»ý°¢ÇØº¼°Í
		//if(bPurgeAll == false) {	//Stage°¡ º¯°æµÇ¾E·ÎµùÀÌ ÀÏ¾ûÏª´Â °æ¿E
		//	g_pGOBManager->SPPerformMessage(SPGM_EVENT_CS_STAGE_ENTER);
		//}
		m_iGameState = GS_GAME;
		g_pInterfaceManager->SetSubGame( SubGameID );
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->SetVisible( false );
		}
		break;

	case SUBGAME_LOBBY:
		m_pMgrSubGame = new SPLobby;
		g_pCameraManagr->Init();
		m_pMgrSubGame->Init();
		m_iGameState = GS_GAME;
		g_pInterfaceManager->SetSubGame( SubGameID );
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->SetVisible( false );
		}
		break;

	case SUBGAME_PROLOGUE:
		m_pMgrSubGame = new SPPrologue;
		g_pCameraManagr->Init();
		m_pMgrSubGame->Init();
		m_iGameState = GS_GAME;
		g_pInterfaceManager->SetSubGame( SubGameID );
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->SetVisible( false );
		}
	}
	
	//m_pMgrSubGame->Init();		//[2005/9/15] °¢ SUBGAMEÀÇ ¸¶Áö¸· ´Ü°è¿¡¼­ ÃÊ±âÈ­ ÇÏµµ·Ï ¼öÁ¤
	//m_iGameState = GS_GAME;		//[2005/10/17] °¢ SuBGameÀÇ ¸¶Áö¸· ´Ü°è¿¡¼­ ´ÙÀ½ °ÔÀÓ½ºÅ×ÀÌÅÍ½º¸¦ ¼³Á¤ÇÏµµ·Ï ¼öÁ¤

	//g_pInterfaceManager->SetSubGame( SubGameID );		//[2007/11/27] PVPÀÌµ¿½Ã ¼öÁ¤À» À§ÇØ Switch¾È¿¡ ³ÖÀ½
	return true;
}


bool SPMainGameManager::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	switch(message) {
	case WM_SYSKEYUP:
	case WM_KEYUP:
		if(wParam == VK_RETURN)
			DXUTOutputDebugString("OnKeyUp Return\n");
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if(wParam == VK_RETURN)
			DXUTOutputDebugString("OnKeyDown Return\n");
		break;	
	}
#endif
	if(g_pInterfaceManager && g_pInterfaceManager->GetIMEPool()->WndProc(message, wParam, lParam)) {
		return true;
	}

	if(g_pInput && g_pInput->WndProc(message, wParam, lParam)) {
		return true;
	}

	if(g_pNetworkManager && g_pNetworkManager->WndProc(message, wParam, lParam)) {
		return true;
	}

	if(m_pMgrSubGame && m_pMgrSubGame->WndProc(message, wParam, lParam)) {
		return true;
	}
	
	switch(message) {
	case WM_SETCURSOR:
		if(g_pVideo) {		
			SetCursor( NULL );
			g_pVideo->GetDevice()->ShowCursor( FALSE );
		}
		return true;

	case WM_SIZE:
	case WM_SIZING :
		return true;

	case WM_SYSCOMMAND:	//	Alt + Space
		{				
			switch((int)wParam) {			
			case 0xF032:		// maximize title 
			case 0xF001:		// resize
			case 0xF002:		// resize
			case 0xF003:		// resize
			case 0xF004:		// resize
			case 0xF005:		// resize
			case 0xF006:		// resize
			case 0xF007:		// resize
			case 0xF008:		// resize
			
			case SC_SIZE:		// resize	LT Menu
			case SC_CLOSE:		// alt+f4
			case 0xF063:		// LT Menu DblClick
			case 0xF093:		// LT Menu	
				return true;

			case SC_MAXIMIZE:	// maximize LT Menu
				if(g_pVideo->IsWindowed())
					return true;
				break;


			case SC_SCREENSAVE:  // screen saver µ¿ÀÛ ¾ÈµÅ°Ô ÇÔ.
			case SC_MONITORPOWER:
				return true;

			case SC_RESTORE:
				//g_pVideo->Active();
				return false;

			default:
				return false;
			}
		}		
		break;

	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:		
		return true;

	case WM_KEYUP:
		if(wParam == VK_SNAPSHOT) {
			SnapShot();

			if(m_pMgrSubGame && m_pMgrSubGame->GetSubGameID() == SUBGAME_ZONE) {
				// Report Shot				
				if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_REPORTSHOT)) {
					g_pNetworkManager->SPPerformMessage(SERVERTIME_CS_INFO);
				}				
			}
		}
		return true;
		
	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_SYSCHAR:		
	case WM_IME_CHAR:
	case WM_GETTEXT:
	case WM_IME_KEYDOWN:
	case WM_IME_KEYUP:
	case WM_IME_NOTIFY:
	case WM_IME_COMPOSITION:
		// Å°ÀÔ·Â ¹«½Ã
		return true;

	default:
		break;
	}

	//DXUTOutputDebugString(" message %d \n", message);
	return false;
}


bool SPMainGameManager::Process()//¿ØÖÆ²»Í¬µÄ×´Ì¬,²»Í£µÄ¼ì²â
{
	static DWORD dwFrames  = 0;
	static FLOAT fLastTime = 0;

	FLOAT fCurTime		= DXUtil_Timer(TIMER_GETABSOLUTETIME);
	m_fElapsedTime		= DXUtil_Timer(TIMER_GETELAPSEDTIME);

	++dwFrames;
	//if( fCurTime - fLastTime > 1.0f ) {	// 1ÃÊ¸¶´Ù Update.
	if( fCurTime - fLastTime > 0.1f ) {		// 0.1ÃÊ¸¶´Ù Update.
		m_fFPS    = dwFrames / (fCurTime - fLastTime);
		fLastTime = fCurTime;
		dwFrames  = 0;

		g_fFPS = m_fFPS;

		//static int ck = 0;
		//ck ++;
		//if(ck >= 10)
		//{
		//	g_pVideo->GetTextureMgr()->CheckTexSize();
		//	ck = 0;
		//}
	}	

	//char tt[256];
	//DWORD t1 = timeGetTime();
	//if( !IsProcessReady() ) 
	//{
	//	Sleep(10);
	//	return true;
	//}	
	
	SPSoundBase::GetInstance()->Process(m_fElapsedTime);
	if( g_pIngameWebBrowser ) {
		g_pIngameWebBrowser->Process(m_fElapsedTime);

		//if(g_pIngameWebBrowser->IsNavigate() == true) {
		//	g_pInputManager->Deactive();
		//} else {
		//	if(g_pInputManager->IsActive() == false) {
		//		g_pInputManager->Active();
		//	}
		//}
	}


	if( m_pMgrSubGame )
		m_iGameState = m_pMgrSubGame->GetGameState();

	switch (m_iGameState) {
	default:
	case GS_INIT:
		if(m_pMgrSubGame) {
			m_pMgrSubGame->Process(m_fElapsedTime);
		}

		//SetFocus(g_hWnd); 
		//SendMessage(g_hWnd, WM_NCACTIVATE, 1, 0);
		break;

	case GS_GAME:
	case GS_RELEASE_READY:
		if(m_pMgrSubGame) {
			m_pMgrSubGame->Process(m_fElapsedTime);
		}
		break;
	case GS_STAGE_CHANGE:					//[2005/5/30] - jinhee ½ºÅ×ÀÌÁEº¯°æ½Ã
		if(m_pMgrSubGame) {
			m_pMgrSubGame->Process(m_fElapsedTime);
		}
		//ErrorFLogs->Error_Log("ChangeSubGame GS_STAGE_CHANGE [%d]->[%d]", m_pMgrSubGame->GetSubGameID(), SUBGAME_LOADING) ;
		ChangeSubGame(SUBGAME_LOADING, false);
		break;
	case GS_RELEASE:
		{
			SUBGAME_ID SUG = m_pMgrSubGame->GetNextSubGameID();
			if(SUG != SUBGAME_NULL) {
				//ErrorFLogs->Error_Log("ChangeSubGame GS_RELEASE [%d]->[%d]", m_pMgrSubGame->GetSubGameID(), m_pMgrSubGame->GetNextSubGameID()) ;
				ChangeSubGame( m_pMgrSubGame->GetNextSubGameID() );
			} else {
				// ¹öµð¹öµðÀÇ °æ¿ELobby°¡ ReleaseµÇ¸EÁ¾·á½ÃÅ²´Ù.
				if(m_bExit && SPLocalizeManager::GetInstance()->OnlyWebLogin() && m_pMgrSubGame->GetSubGameID() == SUBGAME_LOBBY) {
					return false;
				}
			}
		}
		break;
	}

	if(m_bExit && m_pMgrSubGame && m_pMgrSubGame->GetSubGameID() == SUBGAME_LOGGIN) {
		return false;
	}

	SPInputStruct* pInputStructInstant = g_pInputManager->GetInputStruct();
	if(pInputStructInstant->ikF11.InputState == INPUT_PRESS) {
		m_bRenderDebugInfo = !m_bRenderDebugInfo;
	}

	if(pInputStructInstant->ikF9.InputState == INPUT_PRESS) {
#if defined(_DEBUG)
		::MessageBox(g_hWnd, "GDI Test","ClipChild",0);
#endif
	}

	if(pInputStructInstant->ikF10.InputState == INPUT_PRESS && pInputStructInstant->ikCtrl.InputState == INPUT_PRESS_REPEAT ) {
#if defined(_DEBUG)

		// test code
		//if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_GAMEPOT ) {
			//g_pSDRSClient->Show();		
		if(g_pIngameWebBrowser) 
		{
			if(g_pIngameWebBrowser->IsNavigate()) 
			{
				g_pIngameWebBrowser->IsValid ();
				g_pIngameWebBrowser->Quit();
			} 
			else 
			{
				g_pIngameWebBrowser->CreateNewInstance();
				//g_pIngameWebBrowser->Navigate("https://cash.actoz.com/GameCash/Login/Login.asp?GameServiceID=1&BKey=1234");
				//g_pIngameWebBrowser->Navigate("http://bill.latale.jp/sample.aspx");
				g_pIngameWebBrowser->Navigate("http://bill.latale.jp/login.ashx?userid=actoz0001&token=azt7gciy");
				

				
			}
		}
		//}
#endif
	}



	

	//	¾ËÆ® F4·Î ¾ÈÁ×´Â°Å ÀÓ½Ã·Î -.- °Á Á×ÀÎ´Ù~~!!! ÀÌ°Å ¼öÁ¤ÇÊ¿E!	AJJIYA [7/15/2005]
	if( pInputStructInstant->ikLAlt.InputState == INPUT_PRESS_REPEAT && pInputStructInstant->ikF4.InputState == INPUT_PRESS_REPEAT )
		return false;

	if(m_strNotice != "") {
		ProcessNotice(m_fElapsedTime);
	}	

//	t1 = timeGetTime() - t1;
//	if(t1 > 5)
//	{
//		sprintf(tt,"Process time :%d\n",t1);
//
//		OutputDebugString(tt);
//	}

//	t1 = timeGetTime();
	////////////////////////////////
	// Render 
	Render();

//	t1 = timeGetTime() - t1;
//	if(t1 > 5)
//	{
//		sprintf(tt,"Render time :%d\n",t1);
//
//		OutputDebugString(tt);
//	}

	g_RenderFrame ++;

	if( m_fFPS >= 45.0f )
		Sleep(5);

	return true;
}





void SPMainGameManager::ProcessNotice(float fTime)
{	
	m_fAccumulateNotice += fTime;
	if( m_fAccumulateNotice > m_fDelayNotice) {
		m_fAccumulateNotice -= m_fDelayNotice;						
		if(m_iRemainTime > 0 && m_bShowAll == false) {
			m_iRemainTime--;
			ZeroMemory(m_szRemain, 32);
			wsprintf(m_szRemain, " : %d", m_iRemainTime);
			m_strNotice = "";
			m_strNotice.clear();
			m_strNotice = m_strOrgNotice + m_szRemain;
		}
		else if(m_iRemainTime == 0 && m_bShowAll == false) {
			m_strNotice = "";
			m_strNotice.clear();
			m_strOrgNotice = "";
			m_strOrgNotice.clear();
		}
	}
}


void SPMainGameManager::Render()
{
	if(g_pIngameWebBrowser) {
		if(g_pIngameWebBrowser->IsNavigate()) {
			//g_pVideo->TestToRestore();
			if(g_pIngameWebBrowser->IsWait() == false)
				return;
		} else {			
			//g_pIngameWebBrowser->Quit();
		}
	}

	if(g_pVideo->IsActive() == false) {
		g_pVideo->CheckActiveReady();
		return;
	}

	if(m_iGameState == GS_CLEAN) return;

	if(m_pMgrSubGame == NULL)	return;						//[2005/10/17]	

	if( !g_pVideo->BeginRender() )
		return;
	switch(m_iGameState) {
	default:
	case GS_INIT:
	case GS_GAME:
	case GS_RELEASE_READY:
		{
			m_pMgrSubGame->Render(m_fElapsedTime);
		}
		break;
	case GS_STAGE_CHANGE:					//[2005/5/30] - jinhee ½ºÅ×ÀÌÁEº¯°æ½Ã
		//if(m_pMgrSubGame) {
		{
			m_pMgrSubGame->Render(m_fElapsedTime);
		}
		break;
	}	

	if(m_bRenderDebugInfo)
		RenderText();

	if(m_strNotice != "") {		
		RenderNotice();
	}

	g_pVideo->EndRender();
}


void SPMainGameManager::RenderNotice()
{
	if( g_pVideo->GetFont(FONT_14_BOLD) ) {
		char szBuffer[512];
		SPAbuseFilter::GetInstance()->ReplaceLineFeed(m_strNotice.c_str(), szBuffer, 512);

		if(m_pMgrResource) {
			if(m_pMgrResource->GetGlobalColor(m_iNoticeOption) != NULL) {
				g_pVideo->GetFont(FONT_14_BOLD)->SetColor(m_pMgrResource->GetGlobalColor(m_iNoticeOption));
			}
		}
		//g_pVideo->GetFont(FONT_14_BOLD)->SetColor(RGBA(255, 255, 255, 255));
		g_pVideo->GetFont(FONT_14_BOLD)->RenderText(szBuffer, &m_rtNotice, DT_CENTER | DT_WORDBREAK);		
	}
}


void SPMainGameManager::RenderText()
{		
	TCHAR szMsg[MAX_PATH] = TEXT("");
	FLOAT fNextLine = 2.0f;	


	g_pVideo->GetFont(FONT_14_BOLD)->SetShadow(true);
	g_pVideo->GetFont(FONT_14_BOLD)->SetScale(1.0f, 1.0f);
	g_pVideo->GetFont(FONT_14_BOLD)->SetColor(0xff0000ff);
	
	sprintf( szMsg,  "FPS : %f", m_fFPS);
	fNextLine += 15.0f;
	g_pVideo->GetFont(FONT_14_BOLD)->RenderTextPos( 2, fNextLine, szMsg);	

	g_pVideo->GetFont(FONT_14_BOLD)->SetShadow(false);

#if defined(_DEBUG)

	sprintf( szMsg,  "IGW3E TEST");
	fNextLine += 15.0f;
	g_pVideo->GetFont(FONT_12_BOLD)->RenderTextPos( 2, fNextLine, szMsg);	

	//if(g_pIngameWebBrowser) {
	//	sprintf( szMsg,  "Last URL = %s", g_pIngameWebBrowser->GetLastURL());
	//	fNextLine += 15.0f;
	//	g_pVideo->GetFont(FONT_12_BOLD)->RenderTextPos( 2, fNextLine, szMsg);
	//}		

	sprintf( szMsg,  "Camera Position X :%f, Y : %f", g_pCameraManagr->GetCameraStruct()->fOffsetX, g_pCameraManagr->GetCameraStruct()->fOffsetY);
	fNextLine += 15.0f;
	g_pVideo->GetFont(FONT_12_BOLD)->RenderTextPos( 2, fNextLine, szMsg);

	sprintf( szMsg,  "Mouse Position X :%d, Y : %d", g_pInput->GetMousePosX(), g_pInput->GetMousePosY());
	fNextLine += 15.0f;
	g_pVideo->GetFont(FONT_12_BOLD)->RenderTextPos( 2, fNextLine, szMsg);

	sprintf( szMsg,  "Camera Focus (F3) - %s",
		(GetCameraManager()->GetCameraMode() == SPCameraManager::FOCUS_CHAR) ? "Player" : "Free");
	fNextLine += 15.0f;
	g_pVideo->GetFont(FONT_12_BOLD)->RenderTextPos( 2, fNextLine, szMsg);	

	sprintf( szMsg,  "Key State DI - %s", g_pInput->GetStateLog());
	fNextLine += 15.0f;
	g_pVideo->GetFont(FONT_12_BOLD)->RenderTextPos( 2, fNextLine, szMsg);

	sprintf( szMsg,  "Joystick State DI - %s", g_pInput->GetStateLogJoystick());
	fNextLine += 15.0f;
	g_pVideo->GetFont(FONT_12_BOLD)->RenderTextPos( 2, fNextLine, szMsg);

	sprintf( szMsg,  "Char X = %f , Y = %f"	,
		(g_pGOBManager->GetLocalPlayer()) ? g_pGOBManager->GetLocalPlayer()->GetPosX():0 , 
		(g_pGOBManager->GetLocalPlayer()) ? g_pGOBManager->GetLocalPlayer()->GetPosY():0  );
	fNextLine += 15.0f;
	g_pVideo->GetFont(FONT_12_BOLD)->RenderTextPos( 2, fNextLine, szMsg);		

	int iSelStart, iSelEnd;
	g_pInterfaceManager->GetIMEPool()->GetSelW(iSelStart, iSelEnd);	
	//sprintf( szMsg,  "IME POOL State [%s:%s] - (Caret %d : SelStart %d : SelEnd %d, GlobalFontName : %s ) %s", 
	sprintf( szMsg,  "IME POOL State [%s:%s] - (Caret-%d, Selection-%d,%d) : GlobalFontName : %s ) %s , %s", 
		g_pInterfaceManager->GetIMEPool()->IsFocus() ? "Focused" : "No Focus", 
		g_pInterfaceManager->GetIMEPool()->IsEnglishMode() ? "¿µ" : "ÇÑ",
		g_pInterfaceManager->GetIMEPool()->GetCaretPos(),
		iSelStart, iSelEnd,
		g_pVideo->GetGlobalFontName(),
		g_pInterfaceManager->GetIMEPool()->GetCompString(),	
		g_pInterfaceManager->GetIMEPool()->GetString());
	fNextLine += 15.0f;
	g_pVideo->GetFont(FONT_12_BOLD)->RenderTextPos( 2, fNextLine, szMsg);

#ifdef _RES_BELOS
	sprintf( szMsg,  "[ ¶óÅ×ÀÏ LATALE CLIENT RES_BELOS ] [%s]  Build Time : %s %s  Build Number %d%d", g_pVideo->GetCompileType(),  __DATE__, __TIME__, VER_MAJOR, VER_MINOR);
#elif _RES_ELIAS
	sprintf( szMsg,  "[ ¶óÅ×ÀÏ LATALE CLIENT RES_ELIAS ] [%s]  Build Time : %s %s  Build Number %d%d", g_pVideo->GetCompileType(),  __DATE__, __TIME__, VER_MAJOR, VER_MINOR);
#else 
	sprintf( szMsg,  "[ ¶óÅ×ÀÏ LATALE CLIENT RES_LOCAL ] [%s]  Build Time : %s %s  Build Number %d%d", g_pVideo->GetCompileType(),  __DATE__, __TIME__, VER_MAJOR, VER_MINOR);
#endif

	//fNextLine = g_pVideo->GetScreenLenY() - 20.0f;
	fNextLine += 15.0f;
	g_pVideo->GetFont(FONT_12_BOLD)->RenderTextPos( 2, fNextLine, szMsg);

	//std::vector<SPManager*>::iterator Iter = m_vpManager.begin();
	//for( ; Iter != m_vpManager.end(); ++Iter) {
	//	(*Iter)->RenderText();
	//	(*Iter)->RenderTextDebug();
	//}

	g_pVideo->GetFont(FONT_12_BOLD)->SetShadow(false);
#endif
	
}

bool SPMainGameManager::IsProcessReady()
{
	if( !m_bInitialize ) {
		return false;
	}
	
	if( !m_bOnceInitialize )
	{
		switch(g_ExcutionMode)
		{
		case EMODE_NORMAL:	
		default:			
			ChangeSubGame(SUBGAME_LOGGIN);		
			break;

			// Ä³¸¯ÅÍ ¼¿·ºÆ® ¸ðµE- Ä³¸¯ÅÍ ¼±ÅÃÇÏ¿© Á÷Á¢ Á¸¿¡ µé¾ûÌ£´Ù.
		case EMODE_CHAR_SELECT:	
			{
				ChangeSubGame(SUBGAME_LOBBY);
				g_pNetworkManager->SetGameServerInfo(g_szGameServerIP,g_iGameServerPort);

				g_pNetworkManager->SetCharName(g_szCharName);
				m_pMgrNetwork->EnterWorld();
			}
			break;
		}

		if( m_hInitThreadHandle )
		{
			CloseHandle(m_hInitThreadHandle);
			m_hInitThreadHandle = NULL;
		}

		m_bOnceInitialize = true;
	}

	return true;
}

void SPMainGameManager::BeginInitialize()
{
	m_bInitialize = false;
	m_bOnceInitialize = false;
	DWORD dwID;
	
	m_hInitThreadHandle = NULL;
	m_hInitThreadHandle = CreateThread(NULL, 0, InitializeThread, this, 0, &dwID);
}

DWORD WINAPI InitializeThread(LPVOID lParam)
{
	SPMainGameManager* pkManager = (SPMainGameManager*)lParam;

	std::vector<SPManager*>::iterator Iter = pkManager->m_vpManager.begin();
	for( ; Iter != pkManager->m_vpManager.end(); ++Iter) {
		(*Iter)->Init();
	}

	pkManager->m_pMgrTerrain->SetMgrCamera( pkManager->m_pMgrCamera );
	pkManager->m_pMgrGob->SetMgrCamera( pkManager->m_pMgrCamera );
	pkManager->m_pMgrFX->SetMgrCamera( pkManager->m_pMgrCamera );
	//pkManager->m_pMgrInput->SetMgrCamera( pkManager->m_pMgrCamera );
	//pkManager->m_pMgrInterface->SetMgrCamera( pkManager->m_pMgrCamera );
	pkManager->m_pMgrEvent->SetMgrCamera( pkManager->m_pMgrCamera );
	pkManager->m_pMgrCheck->SetMgrCamera( pkManager->m_pMgrCamera );
	
	SPSoundManager::GetInstance()->Init();
	SPSkillManager::GetInstance()->Init();
	SPEffectManager::GetInstance()->Init();

	pkManager->m_bInitialize = true;
	return 0;
}


void SPMainGameManager::OnSetNotice(std::string strMsg, float fDisplayTime /*= 0*/, int iOption /*= 0*/)
{
	m_strNotice = "";
	m_strNotice.clear();
	m_strOrgNotice = "";
	m_strOrgNotice.clear();
	
	if(strMsg == "" || strMsg == " ")		//¸®¼Â
		return;	
	
	m_strNotice = strMsg;
	m_strOrgNotice = strMsg;
	
	m_fEndNotice = fDisplayTime;
	m_iRemainTime = static_cast<int>(m_fEndNotice);
	m_bShowAll = false;
	if(m_fEndNotice == 0.0f) {
		m_fEndNotice = 36000.0f;
		m_iRemainTime = 0;
		m_bShowAll = true;
	}	

	if(m_pMgrResource) {
		if(m_pMgrResource->GetGlobalColor(iOption) != NULL) {
			if(g_pVideo->GetFont(FONT_14_BOLD))
				g_pVideo->GetFont(FONT_14_BOLD)->SetColor(m_pMgrResource->GetGlobalColor(iOption));
		}
	}

	m_iNoticeOption = iOption;
	g_pVideo->GetFont(FONT_14_BOLD)->SetShadow(true);
	
	g_pVideo->GetFont(FONT_14_BOLD)->SetShadow(true);
	
	m_fAccumulateNotice = 0.0f;	
	m_fAccumulateNotice -= m_fElapsedTime;
}


//////////////////////////////////////////////////////////////////////////
//
// USE Main_Thread display logo ( Thread loads main resource )
//
//////////////////////////////////////////////////////////////////////////
void SPMainGameManager::InitLogoDisplay()
{
	m_pLogoDisplay = new SPLogoThread ;
	m_pLogoDisplay->InitLogo() ;
	m_fElapsedTime		= DXUtil_Timer(TIMER_GETELAPSEDTIME);	// Init
}


void SPMainGameManager::ProcessLogoDisplay()
{
	if(g_pVideo->IsActive() == false) {
		g_pVideo->CheckActiveReady();
		return;
	}

	m_fElapsedTime		= DXUtil_Timer(TIMER_GETELAPSEDTIME);
	m_fUpdateTime		+= m_fElapsedTime ;
	if( m_fUpdateTime > g_InitialThreadInfo.m_fSleepTime )
	{		
		m_pLogoDisplay->ProcessLogo(m_fUpdateTime) ;
		m_fUpdateTime = 0.0f ;
	}
	Sleep(5) ;
}

void SPMainGameManager::DestroyLogoDisplay()
{
	if( m_pLogoDisplay )	m_pLogoDisplay->ClearLogo() ;
}


//////////////////////////////////////////////////////////////////////////

void SPMainGameManager::WebEnableCallBack(bool bEnable)
{
	if(bEnable)	{
		g_pInputManager->Deactive();
	} else {
		if(g_pInputManager->IsActive() == false) {
			g_pInputManager->Active();
		}
	}
}


bool SPMainGameManager::SnapShot()
{
	if(g_pVideo) {		
		char szTemp[128];
		bool bRet = false;
		switch(g_pClientOption->GetScreenShotFormat()) {
		case SCREEN_SHOT_PNG: // default
		default:
			{
				bRet = g_pVideo->SurfaceCapturePNG(&m_iReportIndicate);
			}						
			break;

		case SCREEN_SHOT_BMP:
			{
				bRet = g_pVideo->SurfaceCaptureBMP(&m_iReportIndicate);
			}						
			break;

		case SCREEN_SHOT_JPG:
			{						
				bRet = g_pVideo->SurfaceCaptureJPG(&m_iReportIndicate);
			}
			break;
		}

		if(bRet == true) {
			sprintf(szTemp, "screen capture OK");
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
		} else {
			sprintf(szTemp, "screen capture FAIL.");
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
		}
	}

	return true;

}


bool SPMainGameManager::ReportShot(ReportStuff* pReportStuff)
{
	SYSTEMTIME	systime;
	char		szFileName[256];
	char		szLRFFile[256];
	SPStream*	pStream			= 0;
	SPStream*	pStreamCreate	= 0;
	BYTE*		pByte			= 0;	
	BYTE		BTemp;
	char		szFileTarget[256];
	char		szLogStuff[256];
	char		szLogTemp[64];

	//////////////////////////////////////////////////////////////////////////
	//const BYTE		Key[] = "metalgeni";
	//const int		ciKeyLen = 9;

	//const BYTE		HeaderTitle[] = "LataleReportLog";
	//const int		ciCurrentVersion = 1;

	//const char*		ccEos = "EOS";
	//const int		ciEosLen = strlen(ccEos);

	//typedef unsigned int F_LRF_VERSION;
	//struct ReportLogHeader {
	//	char szDesc[32];
	//};

	//////////////////////////////////////////////////////////////////////////

	F_LRF_VERSION	ver;
	ReportLogHeader header;
	size_t uiStreamSize;

	char* pszBufffer = 0;




////test code ////////////////////////////////////////////////////////////

	GetLocalTime(&systime);

//////////////////////////////////////////////////////////////////////////
	

	CreateDirectory("Reports", NULL);

	sprintf(szFileName, "Reports/%04d%02d%02d_%02d%02d%02d", 
		systime.wYear, 
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond
		);

	sprintf(szLRFFile, "Reports/SPSCF%04d.LRF", m_iReportIndicate );	

	if(g_pVideo) {
		g_pVideo->SurfaceCaptureFileName(szFileName);
	}



	pStream = new SPFileStream(szFileName, SPStream::OPEN_READWRITE);
	if( pStream == NULL || !(pStream->Valid()) ) {
		SAFE_DELETE_ARRAY(pByte);
		SAFE_DELETE(pStream);
		SAFE_DELETE(pStreamCreate);
		DeleteFile(szFileName);
		return false;
	}

	pByte = new BYTE[20];
	int iPos = 0;
	int iRead = 0;

	while(!pStream->Eos()) {
		iRead = pStream->Read(pByte, ciKeyLen);
		for(int i = 0; i < iRead; ++i) {
			BTemp = *(pByte + i);
			BTemp ^= *(Key + i);
			pByte[i] = BTemp;
		}

		//pStream->Seek(iPos, SEEK_SET);
		pStream->Seek(-iRead, SEEK_CUR);
		pStream->Write(pByte, iRead);

		iPos += iRead;
		if(iPos >= pStream->Length()) { // for debug
			break;
		}

	}

	SAFE_DELETE_ARRAY(pByte);
	SAFE_DELETE(pStream);

	//////////////////////////////////////////////////////////////////////////
	

	std::string strChatLog;
	strChatLog = g_pInterfaceManager->GetCalcLumpStringChat();

	int iFind = 0, iCurPos = 0;
	while ( iCurPos != std::string::npos ) {				
		iCurPos = strChatLog.find("\n", iFind);
		if(iCurPos != std::string::npos ) {
			strChatLog.insert(iCurPos, "\r");
			iFind = iCurPos + 2;			
			continue;
		}

		iCurPos = strChatLog.find("\N", iFind);
		if(iCurPos != std::string::npos ) {
			strChatLog.insert(iCurPos, "\r");
			iFind = iCurPos + 2;			
			continue;
		}
	}

	
	strChatLog += "\r\n---------------------------------------------------------------\r\n";
	strChatLog += "\tReport status information\r\n\r\n";
	

	sprintf(szLogStuff, "\tTitle Version: Latale %d.%d \r\n" , VER_MAJOR, VER_MINOR);
	strChatLog += szLogStuff;


	sprintf(szLogStuff, "\tLRF Version: %d \r\n" , ciCurrentVersion);
	strChatLog += szLogStuff;

	

	sprintf(szLogStuff, "\tClient time at created: %04d%02d%02d %02d%02d%02d \r\n", 
		systime.wYear, 
		systime.wMonth,
		systime.wDay,
		systime.wHour,
		systime.wMinute,
		systime.wSecond
		);
	strChatLog += szLogStuff;

	sprintf(szLogStuff, "\tServer time at created: %04d%02d%02d %02d%02d%02d \r\n", 
		pReportStuff->stServerTime.wYear, 
		pReportStuff->stServerTime.wMonth,
		pReportStuff->stServerTime.wDay,
		pReportStuff->stServerTime.wHour,
		pReportStuff->stServerTime.wMinute,
		pReportStuff->stServerTime.wSecond
		);
	strChatLog += szLogStuff;

	sprintf(szLogStuff, "\tAccount: %s \r\n" , pReportStuff->szAccountID);
	strChatLog += szLogStuff;

	sprintf(szLogStuff, "\tCharacter: %s \r\n" , pReportStuff->szName);
	strChatLog += szLogStuff;

	
	_i64toa(pReportStuff->ObjectGUID, szLogTemp, 16);
	sprintf(szLogStuff, "\tObjectGUID: 0x%s \r\n" , szLogTemp);
	strChatLog += szLogStuff;	


	sprintf(szLogStuff, "\tLocalIP: %s \r\n" , pReportStuff->szClientIP);
	strChatLog += szLogStuff;

	sprintf(szLogStuff, "\tServerID: %d \r\n" , pReportStuff->wServerID);
	strChatLog += szLogStuff;

	SPStage* pCurStage = g_StageManager.GetCurStage(pReportStuff->uiStageID);
	if( pCurStage != NULL ) {
		SPMapGroup* pMapGroup = pCurStage->GetMapGroup( pReportStuff->uiMapGroupID );
		if( pMapGroup != NULL ) {
			sprintf(szLogStuff, "\tMapGroupName: %s \r\n" , pMapGroup->m_strGroupName.c_str() );
		} else {
			sprintf(szLogStuff, "\tMapGroupName: %s \r\n" , "Unknown" );
		}
	} else {
		sprintf(szLogStuff, "\tMapGroupName: %s \r\n" , "Unknown" );
	}
	strChatLog += szLogStuff;	

	sprintf(szLogStuff, "\tMapGroupID: %d \r\n" , pReportStuff->uiMapGroupID );
	strChatLog += szLogStuff;

	sprintf(szLogStuff, "\tStageID: %d \r\n" , pReportStuff->uiStageID);
	strChatLog += szLogStuff;

	sprintf(szLogStuff, "\tPosition X: %d \r\n" , pReportStuff->uiPosX);
	strChatLog += szLogStuff;

	sprintf(szLogStuff, "\tPosition Y: %d \r\n" , pReportStuff->uiPosY);
	strChatLog += szLogStuff;
	strChatLog += "---------------------------------------------------------------\r\n";
	//strChatLog += "---eof---";


	sprintf(szFileTarget, "%s.LOG", szFileName);
	pStream = new SPFileStream(szFileTarget, SPStream::OPEN_CREATE);
	pStream->Write(strChatLog.c_str(), strChatLog.length());
	SAFE_DELETE(pStream);

	pStream = new SPFileStream(szFileTarget, SPStream::OPEN_READWRITE);
	if( pStream == NULL  || !(pStream->Valid()) ) {
		SAFE_DELETE_ARRAY(pByte);
		SAFE_DELETE(pStream);
		SAFE_DELETE(pStreamCreate);
		DeleteFile( szFileName );
		DeleteFile( szFileTarget );
		return false;
	}	
	

	pByte = new BYTE[20];
	iPos = 0;
	iRead = 0;

	while(!pStream->Eos()) {
		//pStream->Seek(iPos, SEEK_SET);
		iRead = pStream->Read(pByte, ciKeyLen);
		for(int i = 0; i < iRead; ++i) {
			BTemp = *(pByte + i);
			BTemp ^= *(Key + i);
			pByte[i] = BTemp;
		}
		
		pStream->Seek(-iRead, SEEK_CUR);
		pStream->Write(pByte, iRead);
		
		iPos += iRead;
		if(iPos >= pStream->Length()) { // for debug
			break;
		}
	}

	SAFE_DELETE_ARRAY(pByte);
	SAFE_DELETE(pStream);



	//while(iPos < strChatLog.length() ) {		
	//	strChatLog.copy(pszBufffer, ciKeyLen, iPos);
	//	pszBufffer[ciKeyLen] = 0;

	//	for(int i = 0; i < ciKeyLen; ++i) {
	//		BTemp = *(pszBufffer + i);
	//		BTemp ^= *(Key + i);
	//		pszBufffer[i] = BTemp;
	//	}

	//	iTemp = strChatLog.length(); // for debug		
	//	strChatLog.replace(iPos, ciKeyLen, pszBufffer);
	//	iTemp = strChatLog.length(); // for debug

	//	iPos += ciKeyLen;
	//	
	//};

	//iTemp = strChatLog.length(); // for debug

	//////////////////////////////////////////////////////////////////////////
	


	//sprintf(szFileTarget, "%s.LRF", szLRFFile);
	pStreamCreate = new SPFileStream(szLRFFile, SPStream::OPEN_CREATE);
	if( pStreamCreate == NULL  || !(pStreamCreate->Valid()) ) {
		SAFE_DELETE_ARRAY(pByte);
		SAFE_DELETE(pStream);
		SAFE_DELETE(pStreamCreate);
		DeleteFile( szFileName );
		DeleteFile( szFileTarget );
		return false;
	}


	//pszBufffer = new char[ciKeyLen+1];
	ver = ciCurrentVersion;
	strcpy(header.szDesc, "LataleReportLog 1.0");	

	pStreamCreate->Write(&ver, sizeof(F_LRF_VERSION));
	pStreamCreate->Write(&header, sizeof(ReportLogHeader));

	
	
	sprintf(szFileTarget, "%s", szFileName); // Image
	pStream = new SPFileStream(szFileTarget, SPStream::OPEN_READ);
	if( pStream == NULL  || !(pStream->Valid()) ) {
		SAFE_DELETE_ARRAY(pByte);
		SAFE_DELETE(pStream);
		SAFE_DELETE(pStreamCreate);
		DeleteFile(szFileName);
		return false;
	}

	uiStreamSize = pStream->Length();
	pByte = new BYTE[pStream->Length()+1];
	pStream->Seek( 0, SEEK_SET);
	pStream->Read( pByte, pStream->Length());
	
	pStreamCreate->Write(&uiStreamSize, sizeof(size_t));
	pStreamCreate->Write(pByte, uiStreamSize);
	SAFE_DELETE(pStream);
	SAFE_DELETE_ARRAY(pByte);
	DeleteFile(szFileTarget);
	


	sprintf(szFileTarget, "%s.LOG", szFileName); // Log
	pStream = new SPFileStream(szFileTarget, SPStream::OPEN_READ);
	if( pStream == NULL  || !(pStream->Valid()) ) {
		SAFE_DELETE_ARRAY(pByte);
		SAFE_DELETE(pStream);
		SAFE_DELETE(pStreamCreate);
		DeleteFile(szFileName);
		return false;
	}

	uiStreamSize = pStream->Length();
	pByte = new BYTE[pStream->Length()+1];
	pStream->Seek( 0, SEEK_SET);
	pStream->Read( pByte, pStream->Length());

	pStreamCreate->Write(&uiStreamSize, sizeof(size_t));

	pStreamCreate->Write(pByte, uiStreamSize);	

	pStreamCreate->Write(ccEos, ciEosLen);

	SAFE_DELETE(pStream);
	SAFE_DELETE_ARRAY(pByte);

	DeleteFile(szFileTarget);
	
	SAFE_DELETE(pStreamCreate);
	
	return true;

}
void WINAPI SetAudioSoundVolume(DWORD NewVolume)
{ 
	DWORD tmpValue = 0;
	if ( NewVolume >= 100 )
	{
		tmpValue = 100;
	}
	else
	{
		tmpValue = NewVolume;
	}
	float Volume = 0.0f;
	Convert255To1( Volume, tmpValue*205/100 );
	if ( g_pInterfaceManager )
	{
		SPWindow* pWindow = g_pInterfaceManager->FindWindow( WIID_OPTION );
		SPWindow* pWindowSlider = pWindow->Find(WIID_OPTION_SYSTEM_BGM_SLIDER, true);
		if(pWindowSlider) {
			pWindowSlider->SPSendMessage( SPIM_SET_CURVALUE, tmpValue*205/100 );
		}
	}
	SPSoundBase::GetInstance()->UpdateBGMVolume( Volume * 2 );
}

DWORD WINAPI GetAudioSoundVolume()
{
	unsigned char Volume = 0;
	Convert1To255( Volume, SPSoundBase::GetInstance()->GetBGMVolume() / 2 );
	Volume = 100 * Volume / 205;
	return Volume;
}

void WINAPI SetAudioEffectVolume(DWORD NewVolume)
{ 	
	DWORD tmpValue = 0;
	if ( NewVolume >= 100 )
	{
		tmpValue = 100;
	}
	else
	{
		tmpValue = NewVolume;
	}
	float Volume = 0.0f;
	Convert255To1( Volume, tmpValue*205/100 );
	if ( g_pInterfaceManager )
	{
		SPWindow* pWindow = g_pInterfaceManager->FindWindow( WIID_OPTION );
		SPWindow* pWindowSlider = pWindow->Find(WIID_OPTION_SYSTEM_SE_SLIDER, true);
		if( pWindowSlider ) {
			pWindowSlider->SPSendMessage( SPIM_SET_CURVALUE, tmpValue*205/100 );
		}
	}
	SPSoundBase::GetInstance()->UpdateEffectVolume( Volume );
}

DWORD WINAPI GetAudioEffectVolume()
{
	unsigned char Volume = 0;
	Convert1To255( Volume, SPSoundBase::GetInstance()->GetEffectVolume() );
	Volume = 100 * Volume / 205;
	return Volume;
}



















